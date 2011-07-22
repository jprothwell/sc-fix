//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file patch_spp_searchFcch.c
///
/// This file contains reception functions that are not part of the
///   equalizing or decoding processes
///
/// notes:
/// the fch detection algrithm Gallite used remove the sliding energy windows which are used to "normalize" the correlation, 
/// this may result in the wrong peak position in fading channel. it is neccesary to use the algrithm
/// JB & Laurent Bastide improved on Greenstone.
/// @date 16/05/07
/// @author Laurent Bastide
//-----------------------------------------------------------------------------
///
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/trunk/patch/gallite/src/patch_spp_SearchFcch.c $ //
//  $Author: huazeng $                                                        // 
//  $Date: 2010-11-06 11:16:11 +0800 (星期�? 2010-11-06) $                     //   
//  $Revision: 3466 $                                                        //   
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

#ifdef ENABLE_PATCH_SPP_SEARCHFCCH

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

// TOP
// include
#include "cs_types.h"
// common/include
#include "gsm.h"
#include "baseband_defs.h"
#include "bb_irq.h"
// spal public headers
#include "spal_xcor.h"
#include "spal_cap.h"
#include "spal_itlv.h"
#include "spal_debug.h"
#include "spal_mem.h"
#include "spal_tcu.h"
#include "spal_irq.h"
#include "spal_comregs.h"
// spp public headers
#include "spp_gsm.h"
#include "spp_profile_codes.h"
// spp private headers
#include "sppp_gsm_private.h"
#include "sppp_cfg.h"
#include "sppp_debug.h"

#ifdef __MBOX_DEBUG__ // DEBUG STUB
#include "spc_mailbox.h"
#include "global_macros.h"
#endif
#include "spc_ctx.h"


//------------------------------------------------------------------------------
// Global variables
//------------------------------------------------------------------------------

PRIVATE SPP_FCCH_PARAM_T g_sppFcchParam SPAL_DATA_INTERNAL;
PRIVATE UINT16 g_sppFcchBurstNb  SPAL_DATA_INTERNAL = 0;
PRIVATE UINT32* g_sppPtrNew SPAL_DATA_INTERNAL;

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------
extern PROTECTED VOID spal_InvalidateDcache(VOID);


//======================================================================
// FCCH Detection function
//----------------------------------------------------------------------
// This function implements FCCH detection over large reception windows
// (usually 9 Time Slots in @b sliced mode or 12 TDMA frames in @b Full
// mode). For both modes, the reception window must be sliced in sub
// parts. The function must be called for each sub part of the window
// and will return an FCCH detection status each time. See FCCH
// detection memory requirements.
// @param RxBuffer UINT32*. Start address of the reception buffer.
// @param Init BOOL. Takes following values:
// - TRUE : first call for this FCCH window,
// - FALSE: subsequent calls for a same FCCH window.
// .
// @param Mode SPP_FCCH_MODE_T. Takes following values:
// - #FCCH_SLICED for detection during dedicated mode,
// - #FCCH_FULL for initial cell syncronization process.
// .
// @param Res SPP_FCCH_RESULT_T*. Pointer to the FCCH Result structure.
//
// @return Takes following values:
// - #FCCH_FOUND if fcch has been detected (results valid),
// - #FCCH_NOT_FOUND if no fcch has been detected,
// - #FCCH_INVALID if fcch has been detected but conditions
//   of detection (window boundaries, suspect frequency error
//   ...) are imposing to discard the detection results.
// .
//======================================================================
PUBLIC SPP_FCCH_RETURN_T SPAL_FUNC_INTERNAL patch_spp_SearchFcch(UINT32* RxBuffer, BOOL Init, SPP_FCCH_MODE_T Mode, SPP_FCCH_RESULT_T* Res)
{
    // FCCH Detection Process uses 3 floating windows (one for Power, one
    // for I-part correlation and one for Q-part correlation)
    //
    // Each window is divided in 3 parts of SPP_FCCH_SIZE symbols, one part
    // Wx is processed at every call to this function(x being 0->1->2->0->1...).
    //
    //   ----------------------------------------------------------------
    //   |      |      |      |      |      |      |      |      |      |
    //   | W0   |  W1  |  W2  |  W0  |  W1  |  W2  |  W0  |  W1  |  W2  |
    //   |      |      |      |      |      |      |      |      |      |
    //   ---------------------|--------------------|--------------------|
    //   |      Power         |         CI         |        CQ          |
    //   |                    |
    //   |< 3 * SPP_FCCH_SIZE >|
    //   |
    //   |
    //   *RxBuffer

    UINT32* Ptr_old;
    SPAL_DC_OFFSET_T Dco;
    BOOL fcchDetected=FALSE;
    SPP_FCCH_RETURN_T error_code;

#if (CHIP_XCOR_HAS_SLIDING_WINDOW)
    UINT32* accu_outBuf =  (!g_sppGlobalParams.fcch_hw_sliding_window)? NULL :
                          (g_sppGlobalParams.fcch_sliding_energy)?
                          (UINT32*)(MEM_ACCESS_CACHED(RxBuffer) + 9*SPP_FCCH_SIZE):
                          (UINT32*)(MEM_ACCESS_CACHED(RxBuffer) + 6*SPP_FCCH_SIZE);
#endif //(CHIP_XCOR_HAS_SLIDING_WINDOW) 
    
    if (Init) // First call for this FCCH search
    {
        UINT16 i;
        register UINT32* temp_ptr;
        register UINT32 temp_accu = 0;
       
        // Init pointers
        g_sppPtrNew = MEM_ACCESS_CACHED(RxBuffer);

        // point to CI
        temp_ptr = (g_sppGlobalParams.fcch_sliding_energy) ? g_sppPtrNew + 3*SPP_FCCH_SIZE : g_sppPtrNew; 

       
        // Initialize globals
        g_sppFcchBurstNb = 1;
        g_sppFcchParam.FCCH_detected=0;
        g_sppFcchParam.Acc_Corr_I   =0;
        g_sppFcchParam.Acc_Corr_Q   =0;
        g_sppFcchParam.Acc_Energy   =0;
        g_sppFcchParam.samples_left =0;
        g_sppFcchParam.Max_Abs_Corr =0;
        g_sppFcchParam.Corr_order_I =0;
        g_sppFcchParam.Corr_order_Q =0;
        g_sppFcchParam.max_index    =0;
        g_sppFcchParam.counter_high =0;
        g_sppFcchParam.counter_low  =0;
        g_sppFcchParam.lastIT       =0;
		
        // Run DCOC
        g_sppFcchParam.Acc_Energy =
            spal_XcorDcoc((UINT32*)MEM_ACCESS_CACHED(RxBuffer),
                                                  SPP_FCCH_SIZE,
                                                  FALSE, // No derotation
                                                  BYPASS_DCOC, // no bypass dcoc,
                                                  &Dco);

        spal_InvalidateDcache();
        
        // Run XCOR in FCCH mode to generate Power, CI and CQ terms
        // This function flushes the Dcache
        spal_XcorFchxc( MEM_ACCESS_CACHED(RxBuffer),
                        NULL,                                   // init 
#if (CHIP_XCOR_HAS_SLIDING_WINDOW)
                        NULL,                                   // output
                           0,                                   // accu_Energy
                           0,                                   // accu_CorrI
                           0,                                   // accu_CorrQ
#endif //(CHIP_XCOR_HAS_SLIDING_WINDOW)
                        SPP_FCCH_SIZE,                          // size
                        g_sppGlobalParams.fcch_sliding_energy); // compute energy

        
        // Calculate initial accumulated CI and CQ
        temp_accu = 0;
        for(i=0;i<SPP_FCCH_SIZE;i++)
            temp_accu += *(temp_ptr++);
        g_sppFcchParam.Acc_Corr_I = temp_accu;
        temp_ptr += 2*SPP_FCCH_SIZE; // point to CQ
        temp_accu = 0;
        for(i=0;i<SPP_FCCH_SIZE;i++)
            temp_accu += *(temp_ptr++);
        g_sppFcchParam.Acc_Corr_Q = temp_accu;

        // No detection the first time
        error_code = FCCH_NOT_FOUND;
        goto exit;
    }


    // Update pointers
    Ptr_old = g_sppPtrNew;
    g_sppPtrNew += SPP_FCCH_SIZE;
    g_sppPtrNew = (g_sppPtrNew==MEM_ACCESS_CACHED(RxBuffer+3*SPP_FCCH_SIZE))?
                                        MEM_ACCESS_CACHED(RxBuffer):
                                        g_sppPtrNew;
										
    if(g_sppGlobalParams.fcch_sliding_energy == FALSE){	
        g_sppFcchParam.Acc_Energy = 
            spal_XcorDcoc(  (UINT32*)g_sppPtrNew,
                        SPP_FCCH_SIZE,
                        FALSE, // No derotation
                        BYPASS_DCOC, // do not bypass dcoc,
                        &Dco);
     }
     else{
            spal_XcorDcoc(  (UINT32*)g_sppPtrNew,
                        SPP_FCCH_SIZE,
                        FALSE, // No derotation
                        BYPASS_DCOC, // do not bypass dcoc,
                        &Dco);		
    }
	
    spal_InvalidateDcache();
    
    // Run XCOR in FCCH mode to generate Power, CI and CQ terms
    // This function flushes the Dcache
    
    spal_XcorFchxc( g_sppPtrNew,
                    Ptr_old,                                // init 
#if (CHIP_XCOR_HAS_SLIDING_WINDOW)                           
                    accu_outBuf,                           // output
                    g_sppFcchParam.Acc_Energy,             //
                    g_sppFcchParam.Acc_Corr_I,             //
                    g_sppFcchParam.Acc_Corr_Q,             //
#endif //(CHIP_XCOR_HAS_SLIDING_WINDOW)  
                    SPP_FCCH_SIZE,                          // size
                    g_sppGlobalParams.fcch_sliding_energy); // compute energy

    // Prevents the search mechanism to wait for a next IT
    // when we are in the last search window (slice mode only).
    if ((Mode == FCCH_SLICED) && (g_sppFcchBurstNb==(SPP_FCCH_NB_WIN-1)))
    {
        g_sppFcchParam.lastIT = TRUE;
    }

    SPP_PROFILE_FUNCTION_ENTRY(spp_FcchDetect);

    fcchDetected = (g_sppGlobalParams.fcch_detect_func)(
                            Ptr_old, 
#if (CHIP_XCOR_HAS_SLIDING_WINDOW)  
                            (g_sppGlobalParams.fcch_hw_sliding_window)? accu_outBuf:g_sppPtrNew,
#else //(!CHIP_XCOR_HAS_SLIDING_WINDOW)
                            g_sppPtrNew,
#endif //(CHIP_XCOR_HAS_SLIDING_WINDOW)
                            &g_sppFcchParam,Res);


    SPP_PROFILE_FUNCTION_EXIT(spp_FcchDetect);

    if (fcchDetected)
    {
#define SYMB_MARGIN 11
#define CI_PTR_OFFSET ((g_sppGlobalParams.fcch_sliding_energy) ? 1*3*SPP_FCCH_SIZE : 0*3*SPP_FCCH_SIZE)
#define CQ_PTR_OFFSET ((g_sppGlobalParams.fcch_sliding_energy) ? 2*3*SPP_FCCH_SIZE : 1*3*SPP_FCCH_SIZE)
        INT32 register temp_CI;
        INT32 register temp_CQ;
        INT32* temp_ptr;
        INT32* fcch_start;
        INT32* fcch_end;
        int start_cnt;                    //GLTODO Should it be INT8, INT16 or INT32 ??
        int end_cnt;                      //GLTODO Should it be INT8, INT16 or INT32 ??
        int cnt;                          //GLTODO Should it be INT8, INT16 or INT32 ??

        // DETECTION FILTERING
        // -------------------
        // Discard detections too close from the window's border
        if (((g_sppFcchBurstNb == 1) && // window's beginning
             (Res->TOf <= (BB_BURST_ACTIVE_SIZE-SPP_FCCH_SIZE)))
            ||
            ( g_sppFcchParam.lastIT && // End of sliced mode window
             (Res->TOf >= (2*SPP_FCCH_SIZE-BB_BURST_ACTIVE_SIZE)))
           )
        {
            error_code = FCCH_INVALID;
            goto exit;
        }

        // FOF CALCULATION
        // ---------------
        // The FOf estimation window must be reduced to avoid taking "wrong"
        // symbols into account in the FOf calculation
        fcch_start = Ptr_old + Res->TOf;
        fcch_end = g_sppPtrNew + Res->TOf - 1;

        if (Res->TOf < 0)
        {
            end_cnt =(SPP_FCCH_SIZE - SPP_ABS(Res->TOf) - SYMB_MARGIN);
            start_cnt = (SPP_ABS(Res->TOf) - SYMB_MARGIN);

            if (SPP_ABS(Res->TOf) < SYMB_MARGIN)
                    end_cnt = SPP_FCCH_SIZE-(2*SYMB_MARGIN);

            if (g_sppPtrNew==MEM_ACCESS_CACHED(RxBuffer))
                fcch_end += 3*SPP_FCCH_SIZE;
            if (Ptr_old==MEM_ACCESS_CACHED(RxBuffer))
                fcch_start += 3*SPP_FCCH_SIZE;
        }
        else
        {
            start_cnt =(SPP_FCCH_SIZE - (Res->TOf) - SYMB_MARGIN);
            end_cnt = ((Res->TOf) - SYMB_MARGIN);

            if ((Res->TOf) < SYMB_MARGIN)
                start_cnt = SPP_FCCH_SIZE-(2*SYMB_MARGIN);
        }

        temp_CI = temp_CQ = 0;

        temp_ptr= (INT32*)(CI_PTR_OFFSET + fcch_start + SYMB_MARGIN);
        for(cnt=0;cnt<start_cnt;cnt++)
            temp_CI += *(temp_ptr++);
        temp_ptr= (INT32*)(CQ_PTR_OFFSET + fcch_start + SYMB_MARGIN);
        for(cnt=0;cnt<start_cnt;cnt++)
            temp_CQ += *(temp_ptr++);

        temp_ptr= (INT32*)(CI_PTR_OFFSET + fcch_end - SYMB_MARGIN);
        for(cnt=0;cnt<end_cnt;cnt++)
            temp_CI += *(temp_ptr--);
        temp_ptr= (INT32*)(CQ_PTR_OFFSET + fcch_end - SYMB_MARGIN);
        for(cnt=0;cnt<end_cnt;cnt++)
            temp_CQ += *(temp_ptr--);
        // Fof calculation
        temp_CI = temp_CI / 0x5430; //  CI / (270833/(4*PI))
        if (temp_CI == 0)
        {
            error_code = FCCH_INVALID;
            goto exit;        
        }
        else
        {
            UINT32 algo_error;
            INT32 valid_thres;
            UINT32 reduced_Acc_Energy = (g_sppFcchParam.Acc_Energy >> 15);

            // FOf calculation
            Res->FOf = (INT16)(temp_CQ / temp_CI);

            // This is to reduce arctan approx error for large angles
            algo_error = (Res->FOf * Res->FOf) >> 17; // ~ 8E-6 x FOf^2
            Res->FOf += ((Res->FOf >0)? (-((INT16)algo_error)):(((INT16)algo_error)));

            // Validity threshold : avoid "wrapped out" FOf to be detected
            // This happens when Frequency error exceed +/- 28kHz
            temp_CI = (temp_CI > 0) ? temp_CI : (-temp_CI);
            reduced_Acc_Energy = reduced_Acc_Energy?reduced_Acc_Energy:1;
            valid_thres = (temp_CI << 3) / reduced_Acc_Energy;

            // Saturate FOf returned for large results
            Res->FOf = (Res->FOf > 13000) ? 13000 : Res->FOf;
            Res->FOf = (Res->FOf <-13000) ?-13000 : Res->FOf;

            // Discard FOf detection if we are out of validity bounds
            if (valid_thres < 5)
            {
                error_code = FCCH_INVALID;
                goto exit;            
            }
        }

        // TOF CALCULATION
        // ---------------
        // Calculate overall offset since the window's start
        Res->TOf = (INT16) ((g_sppFcchBurstNb-1)*SPP_FCCH_SIZE // passed bursts
                            + (INT16) Res->TOf // offset found
                            - ((BB_BURST_ACTIVE_SIZE-SPP_FCCH_SIZE)/2 - 1)); // win bias
        error_code = FCCH_FOUND;
        goto exit;                            

    }

    g_sppFcchBurstNb++;
    error_code = FCCH_NOT_FOUND;

exit:

    if ( error_code != FCCH_NOT_FOUND)
    {
        // This could be risky: we might disable the IFC after it was started under interrupt
        // Disable IFC as we are done
        UINT32 critical_sec = spal_IrqEnterCriticalSection();
        // Disable IFC2 only if there is no other window programmed
        if ( (g_spcCtx->bcpuIrqCause&BB_IRQ_BB_IRQ_FRAME) == 0 ||
            g_mailbox.pal2spc.win[spal_ComregsGetSnap()].rx.qty == 0)
        {
            spal_TcuClrTco(g_mailbox.rxOnTcoSettings); //xcv.PDN
        }
        spal_IrqExitCriticalSection(critical_sec);  
    }

    return error_code;
}


#endif // ENABLE_PATCH_SPP_SEARCHFCCH

