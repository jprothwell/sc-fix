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

//------------------------------------------------------------------------------
// Global variables
//------------------------------------------------------------------------------

PRIVATE SPP_FCCH_PARAM_T g_sppFcchParam SPAL_BBSRAM_GLOBAL;
PRIVATE UINT16 g_sppFcchBurstNb  SPAL_BBSRAM_GLOBAL = 0;
PRIVATE UINT32* g_sppPtrNew SPAL_BBSRAM_GLOBAL;

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

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
PUBLIC SPP_FCCH_RETURN_T spp_SearchFcch(UINT32* RxBuffer, BOOL Init, SPP_FCCH_MODE_T Mode, SPP_FCCH_RESULT_T* Res)
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
        return FCCH_NOT_FOUND;
    }

    // Update pointers
    Ptr_old = g_sppPtrNew;
    g_sppPtrNew += SPP_FCCH_SIZE;
    g_sppPtrNew = (g_sppPtrNew==MEM_ACCESS_CACHED(RxBuffer+3*SPP_FCCH_SIZE))?
                                        MEM_ACCESS_CACHED(RxBuffer):
                                        g_sppPtrNew;

    g_sppFcchParam.Acc_Energy = 
        spal_XcorDcoc(  (UINT32*)g_sppPtrNew,
                        SPP_FCCH_SIZE,
                        FALSE, // No derotation
                        BYPASS_DCOC, // do not bypass dcoc,
                        &Dco);

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

//======================================================================
// spp_DeinterleaveBlock
//----------------------------------------------------------------------
// DeInterleaving of a data block
//
// @param BufferIn UINT32*. INPUT. Pointer to equalized bursts buffer.
// @param InterleaverType UINT8. INPUT. InterleaverType may take the following values:
// - 0 : TYPE_1A. Valid for TCH/FS, TCH/EFS, FACCH/F, TCH/F2.4,
// TCH/AFS (speech, ratscch, sid_first).
// - 1 : TYPE_1B. Valid for SACCH, SDCCH, TCH/AFS(sid_update), PDTCH(CS-1 to CS-4),
// BCCH, PCH, AGCH, PACCH, PBCCH, PAGCH, PPCH, PNCH, PTCCH/D.
// - 2 : TYPE_1C. Valid for FACCH/H.
// - 3 : TYPE_2A. Valid for TCH/HS and TCH/AHS (speech, ratscch, sid_first).
// - 4 : TYPE_2B. Valid for TCH/AHS (sid_update).
// - 5 : TYPE_3. Valid for TCH/F14.4, TCH/F9.6, TCH/F4.8, TCH/H4.8
// and TCH/H2.4.
// .
// @param BurstOffset UINT8. INPUT. Offset position for deinterleaving (see ITLV
//                      documentation).
// @param BufferOut UINT32*. OUTPUT. Output pointer.
//======================================================================
PUBLIC VOID spp_DeinterleaveBlock(UINT32* BufferIn, UINT8 InterleaverType, UINT8 BurstOffset, UINT32* BufferOut)
{
    if (InterleaverType == ITLV_TYPE_2A)
        spal_ItlvDitlv(InterleaverType, BurstOffset, 228, BufferIn, BufferOut);
    else
        spal_ItlvDitlv(InterleaverType, BurstOffset, 456, BufferIn, BufferOut);

    // Mandatory as it freezes the module
    spal_ItlvIntClear();
}


//======================================================================
// spp_GetUSF
//----------------------------------------------------------------------
/// Uplink State Flag extraction
///
/// @param BufferIn UINT32*. Pointer to the beginning of the RF samples buffer.
/// @param CS UINT8. Coding scheme.
///
/// @return UINT8. Uplink State Flag
//======================================================================
PUBLIC UINT8 spp_GetUSF(UINT32* BufferIn, UINT8 CS)
{
    UINT8 i, j, nb_bits;
    UINT8 Usf = 0;
    UINT8* softBit = (UINT8*)BufferIn;
    INT16 distance, usfDistance = (INT16)0x8000;
    INT16 distance_max=0;
    UINT16 USF_pattern[8] = {0x000, 0xD0B, 0x6EC, 0xBE7, 0xBB0, 0x6BB, 0xD5C, 0x057};

    if (CS == GSM_CS1){
        nb_bits = 6;
    }
    else    // CS2 & CS3 & CS4
    {
        nb_bits = 12;
    }

    for (j = 0; j < nb_bits; j++) distance_max+=(softBit[j]&0x7F);

    for (i = 0; i < 8; i++)
    {
        distance = 0;
        for (j = 0; j < nb_bits; j++)
        {
            if (((USF_pattern[i] >> j) & 1) ^ ((softBit[j] >> 7) & 1))
                distance -= (softBit[j] & 0x7f);
            else
                distance += (softBit[j] & 0x7f);
        }
        if (distance > usfDistance)
        {
            usfDistance = distance;
            Usf = i;
        }
    }


    //rau = usfDistance/distance_max;
    //if (rau>0.875) && (distance_max>24) return Usf
    if(((usfDistance*16)>(distance_max*14))&&(distance_max>24)) return Usf;
    else return 0xFF;

}


//======================================================================
// spp_DecodeStealingBits
//----------------------------------------------------------------------
// Decode Stealing Flags of Normal Bursts.
//
// @param RxBuffer UINT32*. Pointer to the beginning of the RF samples buffer.
// @param StealMode BOOL. Takes following values:
// - TRUE for PDCH Steal flags type,
// - FALSE for TCH Steal flags type.
// .
// @param StealingResult UINT8*. Output pointer.
// @param Threshold INT16. Decision threshold (for TCH only).
//======================================================================
PUBLIC VOID spp_DecodeStealingBits(UINT32* RxBuffer, BOOL StealMode, UINT8* StealingResult, INT16 Threshold)
{
    int i, j;

    UINT8 cs_value[4]={GSM_CS1,GSM_CS2,GSM_CS3,GSM_CS4};
    UINT8 cs[4] = {0xff, 0x13, 0x84, 0x68};

    if (StealMode)//PDCH
    {
        UINT8* softBit;
        INT16 distance, csDistance = (INT16)0x8000;
        UINT8 csMode = 0;

        softBit = (UINT8*)(RxBuffer);

        for (i = 0; i < 4; i++)
        {
            distance = 0;
            for (j = 0; j < 8; j++)
            {
                if (((cs[i] >> j) & 1) ^ ((softBit[j] >> 7) & 1))
                    distance -= (softBit[j] & 0x7f);
                else
                    distance += (softBit[j] & 0x7f);

            }
            if (distance > csDistance)
            {
                csDistance = distance;
                csMode = cs_value[i];
            }
        }
        *StealingResult = csMode;
    }
    else//TCH
    {
        INT16 sum = 0;
        INT8* softBit = (INT8*)RxBuffer;

        for (i = 0; i < 8; i++)
        {
            if (softBit[i]&0x80){
                sum -= (softBit[i] & 0x7f);
                sum--;
            }
            else{
                sum += (softBit[i] & 0x7f);
                sum++;
            }
        }
        *StealingResult = (sum > Threshold ? 0 : 1);
    }
}


//======================================================================
// spp_GetMonPower
//----------------------------------------------------------------------
// Calculate Power for Monitoring (and Interference) windows.
//
// @param MonIdx UINT8. Index of the monitoring (or interference) window.
// @param power UINT32*. Pointer to the resulting calculated power.
// @param step UINT8*. Pointer to the reached step (see AGC mechanism).
//======================================================================
PUBLIC VOID spp_GetMonPower(UINT8 MonIdx, UINT32* RxBuffer, UINT32 *ResPower)
{
    UINT32 pow;
    SPAL_DC_OFFSET_T Dco;
    pow = spal_XcorDcoc( (UINT32*)RxBuffer,
                         MON_WIN_SIZE,
                         FALSE,
                         BYPASS_DCOC,
                         &Dco);
    *ResPower = spp_10log10(pow / MON_WIN_SIZE);
}

//=============================================================================
// spp_GetChannelType
//-----------------------------------------------------------------------------
// TODO : temporary REALLY dirty Mode selection ...
//
// @param ChMode UINT16. ?? See spp_rx.c.
// @param AMRType UINT32. ?? See spp_rx.c.
// @param AMRMode UINT32. ?? See spp_rx.c.
//
// @return SPP_CODING_SCHEME_T.
//
//=============================================================================
PROTECTED SPP_CODING_SCHEME_T spp_GetChannelType(UINT16 ChMode, UINT32 AMRType, UINT32 AMRMode)
{
    switch ((ChMode >> 2) & 0x3)
    {
        case 0:     // Control channel
            switch ((ChMode >> 8) & 0x3)
            {
                case GSM_CS1:
                    return CS_CS1;
                break;
                case GSM_CS2:
                    return CS_CS2;
                break;
                case GSM_CS3:
                    return CS_CS3;
                break;
                case GSM_CS4:
                    return CS_CS4;
                break;
           }
        break;

        case 1:     // speech channel
            if (((ChMode >> 4) & 0x3) == 0) // HR or FR
            {
                if ((ChMode & 0x1) == 1) // HR
                {
                    return CS_HR;
                }
                else // FR
                {
                    return CS_FR;
                }
            }
            else if (((ChMode >> 4) & 0x3) == 1) // EFR
            {
                return CS_EFR;
            }
            else // AMR
            {
                if ((ChMode & 0x1) == 1) // AHS
                {
                    switch (AMRType)
                    {
                        case SPP_AHS_SID_UPDATE:
                            return CS_AHS_SID_UPDATE;
                        case SPP_AHS_SID_UPDATE_INH:
                            return CS_AHS_SID_UPDATE_INH;
                        case SPP_AHS_SID_FIRST_P1:
                            return CS_AHS_SID_FIRST_P1;
                        case SPP_AHS_SID_FIRST_P2:
                            return CS_AHS_SID_FIRST_P2;
                        case SPP_AHS_SID_FIRST_INH:
                            return CS_AHS_SID_FIRST_INH;
                        case SPP_AHS_ONSET:
                            return CS_AHS_ONSET;
                        case SPP_AHS_SPEECH:
                            switch (AMRMode)
                            {
                                case SPP_MR475_MODE:
                                    return CS_AHS_SPEECH_475;
                                case SPP_MR515_MODE:
                                    return CS_AHS_SPEECH_515;
                                case SPP_MR59_MODE:
                                    return CS_AHS_SPEECH_59;
                                case SPP_MR67_MODE:
                                    return CS_AHS_SPEECH_67;
                                case SPP_MR74_MODE:
                                    return CS_AHS_SPEECH_74;
                                case SPP_MR795_MODE:
                                    return CS_AHS_SPEECH_795;
                            }
                        case SPP_AHS_RATSCCH_MARKER:
                            return CS_AHS_RATSCCH_MARKER;
                        case SPP_AHS_RATSCCH_DATA:
                            return CS_AHS_RATSCCH_DATA;
                    }

                }
                else // AFS
                {
                    switch (AMRType)
                    {
                        case SPP_AFS_SID_UPDATE:
                            return CS_AFS_SID_UPDATE;
                        case SPP_AFS_SID_FIRST:
                            return CS_AFS_SID_FIRST;
                        case SPP_AFS_ONSET:
                            return CS_AFS_ONSET;
                        case SPP_AFS_SPEECH:
                            switch (AMRMode)
                            {
                                case SPP_MR475_MODE:
                                    return CS_AFS_SPEECH_475;
                                case SPP_MR515_MODE:
                                    return CS_AFS_SPEECH_515;
                                case SPP_MR59_MODE:
                                    return CS_AFS_SPEECH_59;
                                case SPP_MR67_MODE:
                                    return CS_AFS_SPEECH_67;
                                case SPP_MR74_MODE:
                                    return CS_AFS_SPEECH_74;
                                case SPP_MR795_MODE:
                                    return CS_AFS_SPEECH_795;
                                case SPP_MR102_MODE:
                                    return CS_AFS_SPEECH_102;
                                case SPP_MR122_MODE:
                                    return CS_AFS_SPEECH_122;
                            }
                        case SPP_AFS_RATSCCH:
                            return CS_AFS_RATSCCH;
                    }
                }
            }
        break;

        case 2:     // Data channel
            if (((ChMode&0x1)==0) && (((ChMode>>6)&0x3)==2))        // H96
            {
                return CS_F96;
            }
            else if(((ChMode&0x1) == 1) && (((ChMode>>6)&0x3)==1))  // H48
            {
                return CS_H48;
            }
            else if (((ChMode&0x1)==0) && (((ChMode>>6)&0x3)==1))   // F48
            {
                return CS_F48;
            }
            else if (((ChMode&0x1) == 1) && (((ChMode>>6)&0x3)==0)) // H24
            {
                return CS_H24;
            }
            else if (((ChMode&0x1)==0) && (((ChMode>>6)&0x3)==0))   // F24
            {
                return CS_F24;
            }
            else if (((ChMode&0x1)==0) && (((ChMode>>6)&0x3)==3))   // F144
            {
                return CS_F144;
            }
        break;
    }
    return 0;
}

