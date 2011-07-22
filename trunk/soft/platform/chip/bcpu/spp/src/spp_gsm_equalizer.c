//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spp_equalizer.c
///
/// This file contains equalization functions of the Signal Processing Package
///
/// @date 16/05/07
/// @author Laurent Bastide
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

// include
#include "cs_types.h"

#include "chip_id.h"

// common/include
#include "gsm.h"
#include "baseband_defs.h"
// SPAL public headers
#include "spal_vitac.h"
#include "spal_cap.h"
#include "spal_xcor.h"
#include "spal_itlv.h"
// SPP public headers
#include "spp_profile_codes.h"
// SPP private headers
#include "sppp_cfg.h"
#include "sppp_gsm_private.h"
#include "sppp_debug.h"


#ifdef __MBOX_DEBUG__ // DEBUG STUB
#include "spc_mailbox.h"
#include "spal_comregs.h"
#include "global_macros.h"
#endif



//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------

#define MAXU_NBIT(n) ((1<<n)-1)
#define SV_MAX MAXU_NBIT(7)


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

//=============================================================================
// spp_ChannelEstimation
//-----------------------------------------------------------------------------
/// This function performs Least Square Error (LSE) channel estimation.
///
/// @param WinType UINT8. Toggles channel estimation function to either
///                       normal burst or SCH.
/// @param burst SPP_EQU_BURST_T*. Pointer to the burst structure.
///
//=============================================================================
PRIVATE VOID spp_ChannelEstimation(UINT8 WinType,
                                   SPP_EQU_BURST_T* burst,
                                   SPP_UNCACHED_BUFFERS_T* UncachedBuffers)
{
    UINT32 Xcorr_Energy[SCH_TSC_XCOR_NBR]; // Sized for SCH
    UINT8 search_size;
    int i;
    UINT8 TOf = 0;
    INT16 *taps = (INT16*)burst->taps;
    UINT16 tapWindow [3] = {6,7,8};
    volatile INT32 *Xcorr_I= UncachedBuffers->SchVitac.Xcorr_I;
    volatile INT32 *Xcorr_Q= UncachedBuffers->SchVitac.Xcorr_Q;

    SPP_PROFILE_FUNCTION_ENTRY(spp_ChannelEstimation);
    // ******************************
    // Search Time Synchronization
    // ******************************
    // Use XCOR to compute Xcorrelation with Training Sequence
    if (WinType == GSM_WIN_TYPE_NBURST)
    {
        volatile INT16 *Xcorr= UncachedBuffers->NbVitac.Xcorr;
        // The first five bits of the training sequence are not
        // used to perform the following correlation, as they are
        // only a repetition of the last 5 bits of the training
        // sequence.
        // Therefore, the received pilots of interest start at earliest
        // on bit 65.
        // The correlation is performed using 16 consecutive bits of the
        // training sequence and it is performed on 8+SPAL_ISI=13
        // consecutive time slot.
        // We can detect a ToF of +/- 4 samples.
        spal_XcorTscxcNb(
                g_sppGsmTscNburst[burst->tsc],
                (UINT32*)(burst->samples + 60 + 4), // 4 samples margin
                (INT16*)Xcorr);

        // Calculate I^2 + Q^2 for every coeffs
        for(i=0;i<NB_TSC_XCOR_NBR;i++)
        {
            Xcorr_Energy[i] =   Xcorr[2*i]     * Xcorr[2*i] +
                                Xcorr[2*i + 1] * Xcorr[2*i + 1];
        }
        search_size = NB_TSC_XCOR_NBR - SPAL_ISI + 1;
    }
    else // SCH
    {

        spal_XcorTscxcSch(
                (UINT32*)(burst->samples + 28 + 12), // 12 samples margin
                (UINT32*)(Xcorr_I),
                (UINT32*)(Xcorr_Q));

        // Calculate I^2 + Q^2 for every coeffs
        // Xcorr results are 18bits -> rescale to avoid ovflw
        for(i=0;i<SCH_TSC_XCOR_NBR;i++)
        {
            Xcorr_Energy[i] =   (((Xcorr_I[i]>>2) * (Xcorr_I[i])>>2)) +
                                (((Xcorr_Q[i]>>2) * (Xcorr_Q[i])>>2));
        }
        search_size = SCH_TSC_XCOR_NBR - SPAL_ISI + 1;
    }

    // Now that we have the correlation on 13 consecutive time
    // slots, we find the maximum power profile considering that
    // the channel is 5 taps long with the main tap in the
    // middle (this assumptions corresponds to a GMSK on an AWGN
    // channel).

    // the correlation window could be flatter at the expense
    // of a little snr decrease (~1dB) in static conditions
       spp_TofSearch( Xcorr_Energy,
                      tapWindow,
                     search_size,
                  &(burst->tof),
                           &TOf);

    // TODO : see if this saturation is usefull
    // TODO : if yes -> do it for SCH also
    // if(burst->tof>32) burst->tof=32;


    // Shift Symbol Buffer Origin for remaining of equalization process
    // After origin shift we have the following bits:
    //
    // NORMAL BURST
    // 0:2     -> guard bits
    // 3:59    -> data bits
    // 60      -> steal bit
    // 61:86   -> pilot bits
    // 87      -> steal bit
    // 88:144  -> data bits
    // 145:147 -> guard bits
    burst->samples += (TOf);
    burst->samples_offset = TOf; // Keep track of symbol offset for HBURST mode

    // ******************************
    // Channel taps Estimation
    // ******************************

    // We skip 2 symbols on both sides of the NB TSC
    // NOTE : the Matrix for SCH is calculated for the same
    //        burst position as NB (it doesn't correspond to
    //        SCH Tsc border)
    spal_XcorChest((INT16*)&burst->samples[63 + CHIP_CHEST_LEN - SPAL_ISI], (INT16*)&g_sppChestTab[burst->tsc][0][0], taps, SPAL_ISI, 15);



    // approximative weightening
    taps[0]=(taps[0]*25231)>>15;
    taps[1]=(taps[1]*25231)>>15;
    taps[2]=(taps[2]*30749)>>15;
    taps[3]=(taps[3]*30749)>>15;
    taps[4]= taps[4];
    taps[5]= taps[5];
    taps[6]=(taps[6]*30749)>>15;
    taps[7]=(taps[7]*30749)>>15;
    taps[8]=(taps[8]*25231)>>15;
    taps[9]=(taps[9]*25231)>>15;
    SPP_PROFILE_FUNCTION_EXIT(spp_ChannelEstimation);
}



//=============================================================================
// spp_Equalize
//-----------------------------------------------------------------------------
/// This function performs Viterbi equalization of a burst.
///
/// @param WinType UINT8. INPUT. Toggles channel estimation function to either
///                       normal burst or SCH.
/// @param burst SPP_EQU_BURST_T*. INPUT/OUTPUT Pointer to the burst structure.
/// @param UncachedBuffers SPP_UNCACHED_BUFFERS_T*. INPUT/OUTPUT. Pointer to
/// the work buffers designated by SPC layer.
//=============================================================================
PRIVATE VOID spp_Equalize(UINT8 WinType,
                          SPP_EQU_BURST_T* burst,
                          SPP_UNCACHED_BUFFERS_T* UncachedBuffers)
{
    INT16 H[4];
    int i,survivor;
    SPAL_VITAC_EQU_PARAM_T equ_param;
    UINT32 SumTable[2*2*8/2];
    volatile UINT8  *path_diff_bkw;
    volatile UINT8  *path_diff_fwd;
    volatile UINT16 *PathMetric;
    UINT8* tb_wr_ptr = &(burst->sv[1]);
    UINT8   tb_start,tb_end,tb_idx,tb_size;
    INT16* taps = (INT16*) burst->taps;
    SPAL_DC_OFFSET_T Dco;
    BOOL fwd_vitac_done = FALSE;

#define BM_SHIFT_MAX    17

    SPP_PROFILE_FUNCTION_ENTRY(spp_Equalize);
    // ***********************************
    // Derive the VITAC shifters settings
    // ***********************************
    equ_param.shift_bm = spp_Log2(burst->noise);
    if (equ_param.shift_bm>SHIFT_BM_OFFSET) equ_param.shift_bm -= SHIFT_BM_OFFSET;

    equ_param.shift_sb = SHIFT_SB_OFFSET;

    if (equ_param.shift_bm>BM_SHIFT_MAX)
    {
        equ_param.shift_sb += (equ_param.shift_bm-BM_SHIFT_MAX);
        equ_param.shift_bm=BM_SHIFT_MAX;
    }
    else if (equ_param.shift_bm<4) equ_param.shift_bm = 4;
    if (equ_param.shift_sb>9) equ_param.shift_sb = 9;


    // ***********************************
    // Precalculation of Sum terms for BMs
    // ***********************************
    // The branch metric for a given node can be precalculated
    // since the register state is directly linked to the node.
    spal_XcorBmsum(taps, (INT16*)SumTable);

    // Compute H0 and HL for Viterbi Accelerator
    H[0] = taps[0];
    H[1] = taps[1];
    H[2] = taps[SPAL_DELAY*2];
    H[3] = taps[SPAL_DELAY*2+1];

    // Saturation on SYMB_SIZE bits signed
    for (i=0;i<4;i++)
    {
        H[i] = (H[i] > SYMB_MAX) ? SYMB_MAX : H[i];
        H[i] = (H[i] < SYMB_MIN) ? SYMB_MIN : H[i];
    }

    equ_param.H0 = (UINT32)((H[1]<<16)|((0xFFFF)&H[0]));
    equ_param.HL = (UINT32)((H[3]<<16)|((0xFFFF)&H[2]));
    equ_param.bmSum     = (INT16*)SumTable;

    // Setup for first half-burst equalization (backward)
    if (WinType == GSM_WIN_TYPE_NBURST)
    {
        equ_param.length = 3 + NB_HPAYLD_SIZE + 1;
        equ_param.in     = (burst->samples +3 +NB_TS_START +SPAL_DELAY -1 -2);
        equ_param.out    =
                     (UINT8*)UncachedBuffers->NbVitac.path_diff_bkw;
        path_diff_bkw    =
            (volatile UINT8*)UncachedBuffers->NbVitac.path_diff_bkw;
        path_diff_fwd    =
            (volatile UINT8*)UncachedBuffers->NbVitac.path_diff_fwd;
        equ_param.pm     =
                     (UINT16*)UncachedBuffers->NbVitac.PathMetric;
        PathMetric       =
            (volatile UINT16*)UncachedBuffers->NbVitac.PathMetric;
        equ_param.first_state = g_sppNbBwequStartSt[burst->tsc];
    }
    else // SCH
    {
        equ_param.length = 3 + SCH_HPAYLD_SIZE;
        equ_param.in     = (burst->samples +3 +SCH_TS_START +SPAL_DELAY -1 -2);
        equ_param.out    =
                     (UINT8*)UncachedBuffers->SchVitac.path_diff_bkw;
        path_diff_bkw    =
            (volatile UINT8*)UncachedBuffers->SchVitac.path_diff_bkw;
        path_diff_fwd    =
            (volatile UINT8*)UncachedBuffers->SchVitac.path_diff_fwd;
        equ_param.pm     =
                     (UINT16*)UncachedBuffers->SchVitac.PathMetric;
        PathMetric       =
            (volatile UINT16*)UncachedBuffers->SchVitac.PathMetric;
        equ_param.first_state = SCH_BWEQU_START_ST;
    }

    // Run the viterbi for the backward part of the burst (1st half)
    spal_VitacEqualize(BACKWARD_TRELLIS, &equ_param);

    // Poll till end of processing
    while(spal_VitacBusy());
    //-------------------------------------------------------------------

    // ****************************************
    // Traceback Processing of backward Viterbi
    // ****************************************
    // Update node 0 for transition 62 with node 0 and 8
    // And get the first payload bit
    // Due to Vitac architecture (PM double buffering), the actual
    // PM position depends on nb_symbol parity.
    PathMetric = (equ_param.length & 0x1) ? PathMetric + VIT_NSTATES:
                                            PathMetric;
    if (PathMetric[0]<PathMetric[8])
    {
        UINT16 diff;

        diff = (PathMetric[8]-PathMetric[0]) >>  equ_param.shift_sb;
        burst->sv[0]=((diff>SV_MAX)? SV_MAX: diff);
        survivor=0;
    }
    else
    {
        UINT16 diff;

        diff = (PathMetric[0]-PathMetric[8]) >>  equ_param.shift_sb;
        burst->sv[0]=0x80|((diff>SV_MAX)? SV_MAX: diff);
        survivor=8;
    }

    // ***************************************************
    // Launching HW Viterbi Trellis processing of the 2nd
    // burst during the Trace-back of the first half-burst
    // if we are not in EQU_HBURST_MODE
    // ***************************************************
    if (WinType == GSM_WIN_TYPE_NBURST)
    {
        // Setup for second half-burst equalization (forward)
        // No change needed for the other parameters
        equ_param.in     = (burst->samples +3 +NB_TS_START +NB_TS_SIZE -2);
        equ_param.out    = (UINT8 *)path_diff_fwd;
        equ_param.first_state = g_sppNbFwequStartSt[burst->tsc];

        if (!EQU_HBURST_MODE)
        {
            // Run the viterbi now only in non-HalfBurst mode
            spal_VitacEqualize(FORWARD_TRELLIS, &equ_param);
        }
        // Start now even in EQU_BURST_MODE if the full Nburst
        // has been received
        else if (spp_FullBurstReady() && (WinType == GSM_WIN_TYPE_NBURST))
        {
            // Run DCOC and Derotation on remaining samples
            // (take into account offset induced by TOf)
            Dco.I=burst->dco.i;
            Dco.Q=burst->dco.q;
            spal_XcorDcocHburst((UINT32*)&(burst->samples[BB_BURST_HALF_SIZE-
                                                         burst->samples_offset]),
                                BB_BURST_TOTAL_SIZE-BB_BURST_HALF_SIZE,
                                TRUE, // Derotation
                                BYPASS_DCOC,
                                Dco);
            // Run the viterbi (Setup for second half-burst
            // equalization has already been done)
            spal_VitacEqualize(FORWARD_TRELLIS, &equ_param);
            // Update "servicing" flags
            spp_OneFullBurstDone();
            fwd_vitac_done = TRUE;
        }
    }
    else // SCH
    {
        // SCH doesn't support HalfBurst mode
        equ_param.in     = (burst->samples +3 +SCH_TS_START +SCH_TS_SIZE -2);
        equ_param.out    = (UINT8 *)path_diff_fwd;
        equ_param.first_state = SCH_FWEQU_START_ST;
        // Run the viterbi
        spal_VitacEqualize(FORWARD_TRELLIS, &equ_param);
    }


    // ****************************************
    // Back to the trace-back of 1st half-burst
    // ****************************************
    //  Setup the traceback loop bounds
    //  Note that for NB stealing bits are handled separately
    if (WinType == GSM_WIN_TYPE_NBURST)
    {
        tb_start = (56 + 1) + SPAL_DELAY - 1; // +1 for steal bit shift
        tb_end   = ( 0 + 1) + SPAL_DELAY - 1;
        tb_size  = 2*57; // steal bits not accounted for
    }
    else //SCH
    {
        tb_start = 38 + SPAL_DELAY - 1;
        tb_end   =  0 + SPAL_DELAY - 1;
        tb_size  = 2*39;
    }

    // Move traceback pointer to beginning of first part of payload
    tb_wr_ptr = &(burst->sv[1]);

    // Get the remaining payload bits
    for (tb_idx=tb_start ; tb_idx>tb_end ; tb_idx--)
    {
        *tb_wr_ptr = (path_diff_bkw[VIT_NSTATES*(tb_idx) + survivor]);

        if (hard8(*tb_wr_ptr++))    survivor = survivor/2+(VIT_NSTATES/2);
        else                        survivor = survivor/2;
    }
    // Store the first stealing bit (hl) at the end of the burst
    if (WinType == GSM_WIN_TYPE_NBURST)
        burst->sv[114] = (path_diff_bkw[VIT_NSTATES*SPAL_DELAY+survivor]);
    //-------------------------------------------------------------------

    // In case of EQU_HBURST_MODE we need to wait for the samples of
    // the second half burst to be ready
    if (EQU_HBURST_MODE && (!fwd_vitac_done))
    {
        if (WinType == GSM_WIN_TYPE_NBURST)
        {
            // Wait for EndOfBurst Interrupt
            while(!spp_FullBurstReady());
            // Run DCOC and Derotation on remaining samples
            // (take into account offset induced by TOf)
            Dco.I=burst->dco.i;
            Dco.Q=burst->dco.q;
            spal_XcorDcocHburst((UINT32*)&(burst->samples[BB_BURST_HALF_SIZE-
                                                         burst->samples_offset]),
                                BB_BURST_TOTAL_SIZE-BB_BURST_HALF_SIZE,
                                TRUE, // Derotation
                                BYPASS_DCOC,
                                Dco);

            // Run the viterbi (Setup for second half-burst
            // equalization has already been done)
            spal_VitacEqualize(FORWARD_TRELLIS, &equ_param);
            // Update "servicing" flag
            spp_OneFullBurstDone();
        }
    }

    // Poll till end of processing
    while(spal_VitacBusy());
    //-------------------------------------------------------------------

    // ****************************************
    // Traceback Processing of forward Viterbi
    // ****************************************
    // Update node 0 for transition 62 with node 0 and 8
    // And get first bit of payload
    if (PathMetric[0]<PathMetric[8])
    {
        UINT16 diff;

        diff = (PathMetric[8]-PathMetric[0]) >>  equ_param.shift_sb;
        burst->sv[tb_size-1]=((diff>SV_MAX)? SV_MAX: diff);
        survivor=0;
    }
    else
    {
        UINT16 diff;

        diff = (PathMetric[0]-PathMetric[8]) >>  equ_param.shift_sb;
        burst->sv[tb_size-1]=0x80|((diff>SV_MAX)? SV_MAX: diff);
        survivor=8;
    }

    // Move traceback pointer to the end of second half of payload
    tb_wr_ptr = &(burst->sv[tb_size - 2]);

    for (tb_idx=tb_start ; tb_idx>tb_end ; tb_idx--)
    {
        *tb_wr_ptr = (path_diff_fwd[VIT_NSTATES*tb_idx + survivor]);

        if (hard8(*tb_wr_ptr--))    survivor = survivor/2+(VIT_NSTATES/2);
        else                        survivor = survivor/2;
    }
    // Store the second stealing bit (hu) at the end of the burst
    if (WinType == GSM_WIN_TYPE_NBURST)
        burst->sv[115] = (path_diff_fwd[VIT_NSTATES*SPAL_DELAY+survivor]);

    SPP_PROFILE_FUNCTION_EXIT(spp_Equalize);
}

//=============================================================================
// spp_BuildRefNBurst
//-----------------------------------------------------------------------------
/// Rebuilds the theoretical received normal burst
/// from the hard bits decisions and the estimated channel taps.
///
/// @param burst SPP_EQU_BURST_T*. Pointer to the burst structure.
///
//=============================================================================
INLINE VOID spp_BuildRefNBurst(SPP_EQU_BURST_T* burst)
{
    UINT32 hard_refBurst_tab[4];
    UINT32* hard_refBurst;
    UINT32 tmp_hardRef[2];

    hard_refBurst = MEM_ACCESS_UNCACHED(hard_refBurst_tab);

    // First extract hard values from equalized output
    // 116 SoftValues => 29 words
    spal_XcorHvextract( burst->sv,
                        (UINT8) 29,
                        hard_refBurst);

    // Build the first part of the burst
    // add the 2 needed tail bits ...
    tmp_hardRef[0] = hard_refBurst[0] << 2;
    // ...to the first 57bits of payload...
    tmp_hardRef[1] = (hard_refBurst[1] << 2)|(hard_refBurst[0] >> 30);
    // ...mask...
    tmp_hardRef[1] &= 0xFFFFFFFFU >> (32-(57+2-32));
    // ...and add the first stealing flag...
    tmp_hardRef[1] |= (hard8(burst->sv[114]) >> 7 ) << (57+2-32);
    // ...and the beginning of the TS
    tmp_hardRef[1] |= (g_sppGsmTscNburst[burst->tsc]) << (57+2-32+1);

    // Reconstruct reference symbols of first part of the burst:
    // 57bits payload + 1 stealing flag + 2 TS bits not done during snr calc.
    spal_XcorSrec((INT16*)burst->taps, // channel taps
                    (NB_HPAYLD_SIZE+1+2), // number of symbols
                    tmp_hardRef, // hardbit reference
                    &(burst->ref[0]),// output
                    SPAL_ISI); // channel tap number

    // Build the second part of the burst
    // add the 4 last TS bits
    tmp_hardRef[0] = (g_sppGsmTscNburst[burst->tsc]) >> (NB_TS_SIZE-4);
    // add the second stealing flag
    tmp_hardRef[0] |= (hard8(burst->sv[115])>>7) << 4;
    // ...to the second 57bits of payload...
    tmp_hardRef[0] |= (hard_refBurst[1] >> (NB_HPAYLD_SIZE-32-5)) & 0xFFFFFFE0;
    tmp_hardRef[0] |= hard_refBurst[2] << (64-NB_HPAYLD_SIZE+5);
    tmp_hardRef[1] = (hard_refBurst[2] >> (NB_HPAYLD_SIZE-32-5));
    tmp_hardRef[1] |= (hard_refBurst[3] << (64-NB_HPAYLD_SIZE+5));
    // add mask for the 2 needed tail bits ...
    tmp_hardRef[1] &= 0xFFFFFFFFU >> (32-(4+1+57-32));

    // Reconstruct reference symbols of second half part of the burst:
    spal_XcorSrec(   (INT16*)burst->taps, // channel taps
                    (NB_HPAYLD_SIZE+1+2), // number of symbols
                    tmp_hardRef, // hardbit reference
                    &(burst->ref[142-(NB_HPAYLD_SIZE+1+2)]),// output
                    SPAL_ISI); // channel tap number

}

//=============================================================================
// spp_BuildRefSCHburst
//-----------------------------------------------------------------------------
/// Rebuilds the theoretical received SCH burst
/// from the hard bits decisions and the estimated channel taps.
///
/// @param burst SPP_EQU_BURST_T*. Pointer to the burst structure.
///
//=============================================================================
INLINE VOID spp_BuildRefSCHburst(SPP_EQU_BURST_T* burst)
{
    UINT32 hard_refBurst_tab[3];
    UINT32* hard_refBurst;
    UINT32 tmp_hardRef[2];

    hard_refBurst = MEM_ACCESS_UNCACHED(hard_refBurst_tab);

    // First extract hard values from equalized output
    // 78 SoftValues => 20 words
    spal_XcorHvextract( burst->sv,
                        (UINT8)20,
                        hard_refBurst);

    // Build the first part of the burst
    // add the 2 needed tail bits ...
    tmp_hardRef[0] = hard_refBurst[0] << 2;
    // ...to the first 39bits of payload...
    tmp_hardRef[1] = (hard_refBurst[1] << 2)|(hard_refBurst[0] >> 30);
    // ...mask...
    tmp_hardRef[1] &= 0xFFFFFFFFU >> (32-(39+2-32));
    // ...and the beginning of the TS
    tmp_hardRef[1] |= (GSM_TSC_SCH0) << (39+2-32+1);

    // Reconstruct reference symbols of first part of the burst:
    // 39bits payload + 2 TS bits not done during snr calc.
    spal_XcorSrec(   (INT16*)burst->taps, // channel taps
                    (SCH_HPAYLD_SIZE+2), // number of symbols
                    tmp_hardRef,  // hardbit reference
                    &(burst->ref[0]), // output
                    SPAL_ISI); // channel tap number

    // Build the second part of the burst
    // add the 5 last TS bits
    tmp_hardRef[0] = (GSM_TSC_SCH1) >> ((SCH_TS_SIZE-32)-5);
    // ...to the second 39bits of payload...
    tmp_hardRef[0] |= (hard_refBurst[1] >> (SCH_HPAYLD_SIZE-32-5)) & 0xFFFFFFE0;
    tmp_hardRef[0] |= hard_refBurst[2] << (64-SCH_HPAYLD_SIZE+5);
    tmp_hardRef[1] = (hard_refBurst[2] >> (SCH_HPAYLD_SIZE-32-5));
    // add mask for the 2 needed tail bits ...
    tmp_hardRef[1] &= 0xFFFFFFFFU >> (32-(2*SCH_HPAYLD_SIZE-64+2));

    // Reconstruct reference symbols of second half part of the burst:
    spal_XcorSrec(   (INT16*)burst->taps, // channel taps
                    (SCH_HPAYLD_SIZE+2), // number of symbols
                    tmp_hardRef, // hardbit reference
                    &(burst->ref[142-(SCH_HPAYLD_SIZE+2)]), // output
                    SPAL_ISI); // channel tap number
}

//=============================================================================
// spp_FofEstimation
//-----------------------------------------------------------------------------
/// Reconstruct the reference burst (softvalues), part of it
/// (training sequence) has already been done during SNR estimation.
///
/// Once the reference burst has been rebuilt, determines the Fof by
/// comparing the first FOF_NB_SYMB symbols of reference burst and
/// received burst, as well as the last FOF_NB_SYMB symbols of reference
/// burst and received burst.
///
/// The Fof is returned in Hertz.
///
/// @param WinType UINT8. Toggles channel estimation function to either
///                       normal burst or SCH.
/// @param burst SPP_EQU_BURST_T*. Pointer to the burst structure.
///
//=============================================================================
PRIVATE VOID spp_FofEstimation(UINT8 WinType, SPP_EQU_BURST_T* burst)
{
    INT32 Icorr,divider;

    SPP_PROFILE_FUNCTION_ENTRY(spp_FofEstimation);
    // Reconstruct the reference burst (softvalues), part of it
    // (training sequence) has already been done during SNR estimation.
    // NOTE: It is not necessary to rebuild the full burst as we only
    // need FOF_NB_SYMB on each side to estimate FOF, but this is more
    // flexible and doesn't cost much thanks to the accelerators.
    if (WinType == GSM_WIN_TYPE_NBURST)
    {
        spp_BuildRefNBurst(burst);
    }
    else // SCH
    {
        spp_BuildRefSCHburst(burst);
    }


    // Calculate Imaginary part of correlation on start of burst
    // '+3' because tail bits are not present in refBurst
    Icorr = - spal_XcorIcorCalc(&(burst->samples[FOF_START_1+3]),
                                &(burst->ref[FOF_START_1]),
                                FOF_NB_SYMB);

    // Calculate Imaginary part of correlation on end of burst
    // '+3' because tail bits are not present in refBurst
    Icorr += spal_XcorIcorCalc( &(burst->samples[FOF_START_2+3]),
                                &(burst->ref[FOF_START_2]),
                                FOF_NB_SYMB);

    // FOf is given by:
    //
    //                           CorrI x Fs
    // FOF = _______________________________________________________
    //
    //       corr_gap x 2 PI x Norm(Power-NoisePower) x nb_symb_corr
    divider= 43104 / (FOF_CORR_GAP*FOF_NB_SYMB);// Fs/(2*PI)=43104
    divider=(INT32)((burst->power-burst->noise)/divider);
    if(divider)
        burst->fof = (INT16)(Icorr / divider);
    else
        burst->fof = 0;
    SPP_PROFILE_FUNCTION_EXIT(spp_FofEstimation);
}

//=============================================================================
// spp_EqualizeNBurst
//-----------------------------------------------------------------------------
// Performs normal burst equalization including:
// - DC offset estimation and correction,
// - channel estimation,
// - SNR estimation,
// - Viterbi equalization and traceback,
// - Frequency Offset estimation.
// .
//
// @param RxBuffer UINT32*. Address of the received samples.
// @param Tsc UINT8. Training sequence number.
// @param EqBufferOut UINT32*.
// @param Res SPP_N_BURST_RESULT_T*. Pointer to the burst equalization structure.
//
//=============================================================================
PUBLIC VOID spp_EqualizeNBurst(UINT32* RxBuffer,
                               UINT8 Tsc,
                               UINT32* EqBufferOut,
                               SPP_UNCACHED_BUFFERS_T* UncachedBuffers,
                               SPP_N_BURST_RESULT_T* Res)
{
    UINT32 refBurst[142]; // stores reconstructed burst

    // TODO : see if this structure can be used one level higher
    // to avoid passing too much parameters and keep structured datas.
    SPP_EQU_BURST_T burst; // equalisation structure
    SPAL_DC_OFFSET_T Dco;

    SPP_PROFILE_FUNCTION_ENTRY(spp_EqualizeNBurst);
    burst.tsc       = Tsc;
    burst.samples   = RxBuffer;
    burst.ref       = refBurst;
    burst.sv        = (UINT8*)EqBufferOut;

#ifdef __MBOX_DEBUG__
    UINT8 snap = spal_ComregsGetSnap();
    // DEBUG STUB : use external samples
    if (g_mailbox.dbg[snap].req & MBOX_DBG_EXT_SAMPLES)
    {
        int i;
        for (i=0;i<BB_BURST_TOTAL_SIZE;i++)
            RxBuffer[i] = g_mailbox.dbg[snap].ext_samples[i];
        g_mailbox.dbg[snap].req &= ~MBOX_DBG_EXT_SAMPLES;
    }
    // DEBUG STUB : record incoming samples
    if (g_mailbox.dbg[snap].req & MBOX_DBG_REC_SAMPLES)
    {
        int i;
        UINT32 *tmp = MEM_ACCESS_UNCACHED(RxBuffer);
        for (i=0;i<BB_BURST_TOTAL_SIZE;i++)
            g_mailbox.dbg[snap].samples[i] = tmp[i];
        g_mailbox.dbg[snap].req &= ~MBOX_DBG_REC_SAMPLES;
    }
#endif

    // DC Offset Correction
    // --------------------
    if (EQU_HBURST_MODE)
    {
        burst.power = spal_XcorDcoc((UINT32*)RxBuffer,
                                    BB_BURST_HALF_SIZE,
                                    TRUE, // Derotation
                                    BYPASS_DCOC,
                                    &Dco);
        burst.dco.i=Dco.I;
        burst.dco.q=Dco.Q;
        burst.power /= BB_BURST_HALF_SIZE; // Normalised power
    }
    else
    {
        burst.power = spal_XcorDcoc((UINT32*)RxBuffer,
                                    BB_BURST_TOTAL_SIZE,
                                    TRUE, // Derotation
                                    BYPASS_DCOC,
                                    &Dco);
        burst.dco.i=Dco.I;
        burst.dco.q=Dco.Q;
        burst.power /= BB_BURST_TOTAL_SIZE; // Normalised power
    }

    // Channel Estimation Process
    // --------------------------
    spp_ChannelEstimation(GSM_WIN_TYPE_NBURST, &burst, UncachedBuffers);

    // SNR Estimation
    // --------------
    // First reconstruct TrainingSequence (minus 2 tail symbols on each side)
    spal_XcorSrec( (INT16*)burst.taps, // Channel Taps
                  (NB_TS_SIZE-4), // Reconstruction length
                  (UINT32*) &(g_sppGsmTscNburst[burst.tsc]), // ref. hardbit sequence
                  &(burst.ref[NB_TS_START+2]), // reconstructed output
                  SPAL_ISI); // channel tap number

    // Run power of the error calculation upon Training Sequence
    // The three guard bits are not included in the reference (reconstructed) burst
    burst.noise = spal_XcorErrCalc(
                    (UINT32*)&(burst.samples[NB_TS_START+3+2]), // original
                    (UINT32*)&(burst.ref[NB_TS_START  +2]), // reference
                    (NB_TS_SIZE-4)); // Number of samples

    // Normalise NoisePower and calculate Snr
    burst.noise /= (NB_TS_SIZE-4);

    if (burst.power > burst.noise)
        Res->Snr =  spp_10log10(burst.power-burst.noise)-spp_10log10(burst.noise);
    else
        Res->Snr =- spp_10log10(burst.noise);

    // Equalize : Viterbi + traceback
    // ------------------------------
    spp_Equalize(GSM_WIN_TYPE_NBURST,&burst, UncachedBuffers);

    // Frequency error estimation
    // --------------------------
    spp_FofEstimation(GSM_WIN_TYPE_NBURST,&burst);

    // Fill result structure
    Res->Pwr = spp_10log10(burst.power);
    Res->TOf = burst.tof;
    Res->FOf = burst.fof;
    Res->Dco.i = burst.dco.i;
    Res->Dco.q = burst.dco.q;

#ifdef __MBOX_DEBUG__ // DEBUG STUB : record part
    if (g_mailbox.dbg[snap].req & MBOX_DBG_REC_REF)
    {
        int i;
        for (i=0;i<BB_BURST_TOTAL_SIZE;i++)
            g_mailbox.dbg[snap].ref[i] = burst.ref[i];
        g_mailbox.dbg[snap].req &= ~MBOX_DBG_REC_REF;
    }
    if (g_mailbox.dbg[snap].req & MBOX_DBG_REC_TAPS)
    {
        int i;
        for (i=0;i<5;i++)
            g_mailbox.dbg[snap].taps[i] = burst.taps[i];
        g_mailbox.dbg[snap].req &= ~MBOX_DBG_REC_TAPS;
    }
#endif
    SPP_PROFILE_FUNCTION_EXIT(spp_EqualizeNBurst);
}

//=============================================================================
// spp_EqualizeDecodeSch
//-----------------------------------------------------------------------------
// Performs SCH burst equalization including:
// - DC offset estimation and correction,
// - channel estimation,
// - SNR estimation,
// - Viterbi equalization and traceback,
// - Frequency Offset estimation.
// .
//
// @param RxBuffer UINT32*. Address of the received samples.
// @param Res SPP_SCH_RESULT_T*. Pointer to the burst equalization structure.
//
//=============================================================================
PUBLIC BOOL spp_EqualizeDecodeSch(UINT32* RxBuffer,
                                  SPP_UNCACHED_BUFFERS_T* UncachedBuffers,
                                  SPP_SCH_RESULT_T* Res)
{
    UINT32 refBurst[142]; // stores reconstructed burst
    UINT32 equalized_out[20]; // stores 78 equalized softvalues
    UINT32 crccode[2];
    int i;
    UINT8 decoded_out_tab[5];
    UINT8 *decoded_out = (UINT8*)MEM_ACCESS_UNCACHED(decoded_out_tab);

    // TODO : see if this structure can be used one level higher
    // to avoid passing too much parameters and keep structured datas.
    SPP_EQU_BURST_T burst; // equalisation structure
    SPAL_DC_OFFSET_T Dco;
    SPAL_VITAC_DEC_CFG_T vitacCfg;

    SPP_PROFILE_FUNCTION_ENTRY(spp_EqualizeDecodeSch);

    burst.samples   = RxBuffer;
    burst.ref       = refBurst;
    burst.sv        = (UINT8*)equalized_out;
    burst.tsc       = SCH_TSC_IDX;

    // DC Offset Correction
    // --------------------
    burst.power = spal_XcorDcoc((UINT32*)burst.samples,
                                BB_SCH_TOTAL_SIZE,
                                TRUE, // Derotation
                                BYPASS_DCOC,
                                &Dco);
    burst.power /= BB_SCH_TOTAL_SIZE; // Normalised power

    // Channel Estimation Process
    // --------------------------
    spp_ChannelEstimation(GSM_WIN_TYPE_SCH, &burst, UncachedBuffers);

    // SNR Estimation
    // --------------
    // First reconstruct TrainingSequence (the 2 tail symbols on each side of the
    // TSC are not used as surrounding symbols are unknown)
    spal_XcorSrec( (INT16*)burst.taps, // Channel Taps
                  (SCH_TS_SIZE-4), // Reconstruction length
                  (UINT32*)g_sppGsmTscSch, // ref. hardbit sequence
                  &(burst.ref[SCH_TS_START+2]), // reconstructed output
                    SPAL_ISI); // channel tap number

    // Run Error Calculation upon Training Sequence
    burst.noise = spal_XcorErrCalc(
                    (UINT32*)&(burst.samples[SCH_TS_START+3+2]), // original
                    (UINT32*)&(burst.ref[SCH_TS_START + 2]), // reference
                    (SCH_TS_SIZE-4)); // Number of samples

    // Normalise NoisePower and calculate Snr
    burst.noise /= (SCH_TS_SIZE-4);

    if (burst.power > burst.noise)
        Res->Snr =  spp_10log10(burst.power-burst.noise) -
                    spp_10log10(burst.noise);
    else
        Res->Snr =- spp_10log10(burst.noise);

    // Equalize : Viterbi + traceback
    // ------------------------------
    spp_Equalize(GSM_WIN_TYPE_SCH,&burst, UncachedBuffers);

    // Frequency error estimation
    // --------------------------
    spp_FofEstimation(GSM_WIN_TYPE_SCH,&burst);

    // Fill result structure
    Res->Pwr = spp_10log10(burst.power);
    Res->TOf = burst.tof;
    Res->FOf = burst.fof;

    //-------------------------------------------------------------------

    // ******************************
    // Channel Decoding of SCH
    // ******************************

    vitacCfg.inSoftBit     = (UINT32*) burst.sv;
    vitacCfg.survivorMem   = (UINT32*) UncachedBuffers->DecodeVitac.SurvivorMEM;
    vitacCfg.punctTable    = (UINT32*) g_sppCsPuncturingTable[CS_SCH];
    vitacCfg.convPoly0     = g_sppVitacPolyres[g_sppVitacDecParam[CS_SCH].resPoly0];
    vitacCfg.convPoly1     = g_sppVitacPolyres[g_sppVitacDecParam[CS_SCH].resPoly1];
    vitacCfg.convPoly2     = g_sppVitacPolyres[g_sppVitacDecParam[CS_SCH].resPoly2];
    vitacCfg.vitacDecParam = g_sppVitacDecParam[CS_SCH];

    spal_VitacFullDecoding(&vitacCfg, (UINT32*)decoded_out);

    // Verify CRC
    spal_CapDecodeCrc(g_sppCsCrcParam[CS_SCH], (UINT32*)decoded_out, crccode);

    // Fill Result structure
    for(i=0;i<4;i++){
        Res->Data[i] = decoded_out[i];
    }

    SPP_PROFILE_FUNCTION_EXIT(spp_EqualizeDecodeSch);
    if (crccode[0]!=0x3FF)  // Failed
    {
        return FALSE;
    }
    return TRUE;
}

//======================================================================
/// spp_TofSearch
//----------------------------------------------------------------------
/// This function is used in order to calculate from the 13 correlations
/// of the ouput of spal_XcorCorrTsc(), the maximum power profile.
/// The channel is consider to be 5 taps long with the main tap in the middle.
///
/// @param in          UINT32*  INPUT.  Pointer to the input matrix.
/// @param pWindow     UINT16*  INPUT.  Pointer to a table containing the taps of the channel window.
/// @param search_size UINT8    INPUT.  Size to search
/// @param BurstTOF    UINT8*   OUTPUT. Address containing the calculated TOf in Quarter bits.
/// @param SampleTOF   UINT8*   OUTPUT. Address containing the calculated TOf per sample.
///
/// @author Nadia Touliou based on the code on the spp_ChannelEstimation().
/// @date 30/11/07
//======================================================================
PROTECTED VOID spp_TofSearch(     UINT32* in,
                                  UINT16* pWindow,
                                  UINT8   search_size,
                                  UINT8*  BurstTOF,
                                  UINT8*  SampleTOF)
{

    UINT8 i;
    UINT32 CorrAccu [SCH_TSC_XCOR_NBR - SPAL_ISI + 1]={0};

    UINT8  TOf = 0;
    UINT32 max_accu = 0;

    for(i=0;i<search_size;i++)
    {
        register UINT32 accu;

        accu  = pWindow[0]*in[i] >>3;
        accu += pWindow[1]*in[i+1] >>3;
        accu += pWindow[2]*in[i+2] >>3;
        accu += pWindow[1]*in[i+3] >>3;
        accu += pWindow[0]*in[i+4] >>3;
        CorrAccu[i]=accu;

        if (accu>=max_accu)
        {
            max_accu = accu;
            TOf = i;
        }
    }

    *SampleTOF = TOf;

    // Besides recentering the received buffer, we report the
    // TOF to the stack to recenter the next receive window.
    // The stack requires the TOF in quarter of a sample.
    //
    // parabolic interpolation for half-symbol precision:
    //                 (Corr[TOf+1]-Corr[TOf-1])
    // dTOf = ________________________________________________
    //        (2*(Corr[TOf]-Corr[TOf+1]+Corr[TOf]-Corr[TOf-1]))
    if( (TOf != 0) && (TOf < (search_size-1)))
    {
        INT8 delta=0;
        INT32 diff1=( CorrAccu[TOf+1]-CorrAccu[TOf-1])<<1;
        INT32 diff2=(max_accu-CorrAccu[TOf+1])+(max_accu-CorrAccu[TOf-1]);
        diff1 += diff2 >> 1; // +0.5 for rounding
        if (diff2>0)
            delta=(INT8)(diff1/diff2);
        // Store result (in Quarterbit)
        *BurstTOF = (UINT8)((INT8)(4*TOf)+delta);
    }
    else
    {
        // Store result (in Quarterbit)
        *BurstTOF = 4*TOf;
    }
}


