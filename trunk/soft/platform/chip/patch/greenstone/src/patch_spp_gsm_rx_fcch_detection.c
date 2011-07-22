//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spp_rx.c
///
/// This file contains reception functions that are not part of the
///   equalizing or decoding processes
///
/// @date 16/05/07
/// @author Laurent Bastide
//-----------------------------------------------------------------------------



//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

// TOP
// include
#include "cs_types.h"
// common/include
#include "gsm.h"
#include "baseband_defs.h"
// spal public headers
#include "xcor.h"
#include "spal_xcor.h"
#include "spal_cap.h"
#include "spal_itlv.h"
#include "spal_debug.h"
#include "spal_mem.h"
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

// Enable frame-based rather than sliding window energy computation
// Do not forget to enable the equivalent flag in spp_fcch_detect.c (!)
// Gain at 39M : 45 µs

#include "spc_mailbox.h"
#define SPP_FCCH_SIZE       (BB_FCCH_SIZE)
#define SPP_FCCH_NB_WIN     (((625*9)/(SPP_FCCH_SIZE*4) + 1))

//------------------------------------------------------------------------------
// Global variables
//------------------------------------------------------------------------------

EXPORT SPP_FCCH_PARAM_T g_sppFcchParam SPAL_BBSRAM_GLOBAL;
EXPORT UINT16 g_sppFcchBurstNb  SPAL_BBSRAM_GLOBAL;
EXPORT UINT32* g_sppPtrNew SPAL_BBSRAM_GLOBAL;

PRIVATE UINT32 g_spalRkm1 SPAL_BBSRAM_GLOBAL=0;
PRIVATE UINT32 g_spalRkm2 SPAL_BBSRAM_GLOBAL=0;

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------
//=============================================================================
// patch_spal_XcorFchxc
//-----------------------------------------------------------------------------
// Xcorrelation mode for FCH burst detection
// Power term overwrites symbols,
// CI term is stored at Offset fom Power term
// and CQ term is stored at Offset from CI term.
//
// @param symb_ptr UINT32*. ??
//=============================================================================
PROTECTED VOID spal_InvalidateDcache(VOID);

INLINE VOID patch_spal_XcorFchxc(UINT32* symb_ptr)
{
    //SPAL_PROFILE_FUNCTION_ENTER(spal_XcorFchxc);
    UINT32 cmd;
    #define offset (3*SPP_FCCH_SIZE)

    cmd =  XCOR_FCHXC_EN |
           XCOR_DEROTATION_EN | // for datapath setting
           XCOR_NB_SYMB(SPP_FCCH_SIZE);

    hwp_xcor->data[0] = (UINT32)symb_ptr[0];           // R(k)
    hwp_xcor->hv0     = (UINT32)g_spalRkm1;                  // R(k-1)
    hwp_xcor->data[1] = (UINT32)g_spalRkm2;                  // R(k-2)
    hwp_xcor->addr0   = (UINT32)symb_ptr;              // @R(k)
    hwp_xcor->data[2] = (UINT32)&(symb_ptr[0*offset]); // @ResPW
    hwp_xcor->data[3] = (UINT32)&(symb_ptr[1*offset]); // @ResCI
    hwp_xcor->data[5] = (UINT32)&(symb_ptr[2*offset]); // @ResCQ
    // Following calls are supposed to be done on successive
    // buffers representing a continuous symbol reception.
    g_spalRkm1 = symb_ptr[SPP_FCCH_SIZE-1]; // last symbol becomes R(k-1)
    g_spalRkm2 = symb_ptr[SPP_FCCH_SIZE-2]; // before last symbol becomes R(k-2)
    // Run XCOR
    hwp_xcor->command = cmd;

    // Flush the Dcache to enable later cached accesses
    spal_InvalidateDcache();
    
    // Poll until the end of processing
    while (((hwp_xcor->status) & XCOR_OP_PENDING));
}

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
PUBLIC SPP_FCCH_RETURN_T patch_spp_SearchFcch(UINT32* RxBuffer, BOOL Init, SPP_FCCH_MODE_T Mode, SPP_FCCH_RESULT_T* Res)
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

    if (Init) // First call for this FCCH search
    {
        UINT16 i;
        register UINT32* temp_ptr;
        register UINT32 temp_accu = 0;
        // Init pointers
        g_sppPtrNew = MEM_ACCESS_CACHED(RxBuffer);
        temp_ptr = g_sppPtrNew + 3*SPP_FCCH_SIZE; // point to CI

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

        // First call for an FCCH detection : R(k-1) and R(k-2)
        // are not defined
        g_spalRkm1 = 0;
        g_spalRkm2 = 0;

        // Run DCOC
        g_sppFcchParam.Acc_Energy =
            spal_XcorDcoc((UINT32*)MEM_ACCESS_CACHED(RxBuffer),
                                                  SPP_FCCH_SIZE,
                                                  FALSE, // No derotation
                                                  BYPASS_DCOC,
                                                  &Dco);

        // Run XCOR in FCCH mode to generate Power, CI and CQ terms
        // Get the energy
        // This function flushes the Dcache
        patch_spal_XcorFchxc( MEM_ACCESS_CACHED(RxBuffer));

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
        return FCCH_NOT_FOUND;
    }

    // Update pointers
    Ptr_old = g_sppPtrNew;
    g_sppPtrNew += SPP_FCCH_SIZE;
    g_sppPtrNew = (g_sppPtrNew==MEM_ACCESS_CACHED(RxBuffer+3*SPP_FCCH_SIZE))?
                                        MEM_ACCESS_CACHED(RxBuffer):
                                        g_sppPtrNew;

    // Run DCOC
    g_sppFcchParam.Acc_Energy = 
        spal_XcorDcoc(  (UINT32*)g_sppPtrNew,
                        SPP_FCCH_SIZE,
                        FALSE, // No derotation
                        BYPASS_DCOC,
                        &Dco);

    // Run XCOR in FCCH mode to generate Power, CI and CQ terms
    // This function flushes the Dcache
    patch_spal_XcorFchxc(g_sppPtrNew);

    // Prevents the search mechanism to wait for a next IT
    // when we are in the last search window (slice mode only).
    if ((Mode == FCCH_SLICED) && (g_sppFcchBurstNb==(SPP_FCCH_NB_WIN-1)))
    {
        g_sppFcchParam.lastIT = TRUE;
    }

    SPP_PROFILE_FUNCTION_ENTRY(spp_FcchDetect);
    fcchDetected = spp_FcchDetect(Ptr_old,g_sppPtrNew,&g_sppFcchParam,Res);
    SPP_PROFILE_FUNCTION_EXIT(spp_FcchDetect);

    if (fcchDetected)
    {
#define SYMB_MARGIN 11
#define CI_PTR_OFFSET 3*SPP_FCCH_SIZE
#define CQ_PTR_OFFSET 2*3*SPP_FCCH_SIZE
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
            return FCCH_INVALID;
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
            return FCCH_INVALID;
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
                return FCCH_INVALID;
            }
        }

        // TOF CALCULATION
        // ---------------
        // Calculate overall offset since the window's start
        Res->TOf = (INT16) ((g_sppFcchBurstNb-1)*SPP_FCCH_SIZE // passed bursts
                            + (INT16) Res->TOf // offset found
                            - ((BB_BURST_ACTIVE_SIZE-SPP_FCCH_SIZE)/2 - 1)); // win bias
        return FCCH_FOUND;
    }

    g_sppFcchBurstNb++;
    return FCCH_NOT_FOUND;
}


