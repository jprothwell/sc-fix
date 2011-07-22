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

#ifdef ENABLE_PATCH_SPP_EQUALIZENBURST

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
#include "spc_ctx.h"
#include "spal_mem.h"


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


// The following addresses of local functions can be derived from ROM elf via mips-elf-nm
#define spp_ChannelEstimation ((VOID (*)(UINT8 WinType, \
                                   SPP_EQU_BURST_T* burst, \
                                   SPP_UNCACHED_BUFFERS_T* UncachedBuffers))0x81e079b1)

#define spp_Equalize ((VOID (*)(UINT8 WinType, \
                          SPP_EQU_BURST_T* burst, \
                          SPP_UNCACHED_BUFFERS_T* UncachedBuffers))0x81e07bc1)

#define spp_FofEstimation ((VOID (*)(UINT8 WinType, SPP_EQU_BURST_T* burst))0x81e08081)


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
PUBLIC VOID SPAL_FUNC_INTERNAL patch_spp_EqualizeNBurst(UINT32* RxBuffer,
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

    if ((Res->Snr <= 0) && (g_mailbox.pal2spc.win[g_spcCtx->currentSnap].rx.nBurst.bufIdx == 4))
    {
        UINT32 i;
        for (i=0; i<29; i++)
        {
            EqBufferOut[i] &= 0x80808080;
        }
    }

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

#endif // ENABLE_PATCH_SPP_EQUALIZENBURST

