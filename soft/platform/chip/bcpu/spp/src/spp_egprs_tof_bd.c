//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spp_egprs_tof_bd.c
///
///
/// @date 30/11/07
/// @author Nadia Touliou
///
/// @date   27/10/06 15/06/07
/// @author Huanglun ZhangChangchun
//-----------------------------------------------------------------------------

#if (CHIP_EDGE_SUPPORTED)

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "sppp_gsm_private.h"
#include "sppp_egprs_private.h"
#include "spal_xcor.h"
#include "sppp_debug.h"


//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------

#define NB_TSC_CORREL          (SPAL_EGPRS_XCOR_TSC_SIZE + SPAL_EGPRS_XCOR_TSC_NBR)


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

//======================================================================
// spp_EgprsBlindDetection
//----------------------------------------------------------------------
/// Performs modulation blind detection and TOf estimation:
///.
/// @param RxBuffer (UINT32*): Pointer to the beginning of the RF samples buffer
/// @param burstIdx (UINT8): burst index number
/// @param tsc UINT8 : training sequence Id
/// @param modulation (SPP_MODULATION_TYPE_T*): Pointer to modulation result
/// @param TOf_in_Qb (UINT8*): Pointer to timing offset result in quarter of bit
/// @param TOf (UINT8*): Pointer to timing offset result
//======================================================================

PUBLIC VOID spp_EgprsBlindDetection(UINT32* RxBuffer,
                                    UINT8 burstIdx,
                                    UINT8 tsc,
                                    SPP_MODULATION_TYPE_T* modulation,
                                    UINT8* TOf_in_Qb,
                                    UINT8* TOf_sample)
{
    // In_Array holding the input in Q11 format.
       UINT16 channelTapsEnergy[2][SPAL_EGPRS_XCOR_TSC_NBR];

    UINT16 tofSearchWindow[2][3] =  {{5,6,12},  // 8PSK window
                                     {6,7,8}};  // GMSK window

    GSM_MODULATION_T modMin;
    GSM_MODULATION_T modMax;
    GSM_MODULATION_T modIs = PSK8;
    UINT32 minEnergy = MAX_INT32;

    UINT8 i;
    UINT8 j;

    SPP_PROFILE_FUNCTION_ENTRY(spp_EgprsBlindDetection);

    // We are interested for the 16 middle bits of the TSC bits [5 to 20].
    // Since the TSC in the received sequence stars at bit 65 for TOf =4, we assume that
    // the first TSC bit of our interest lies on input[70].
    // We start the scanning for input[64].

    modMin = (!burstIdx) ? PSK8 : (*modulation == SPP_8PSK_MOD) ? PSK8 : GMSK;
    modMax = (!burstIdx) ? GMSK : (*modulation == SPP_8PSK_MOD) ? PSK8 : GMSK;

    // Search over the possible modulations,
    // if (burstIdx) the modulation is known,
    // so BD is not performed and there is only
    // one iteration.
    for (i = modMin; i <= modMax; ++i)
    {
        UINT32 noisePwr = 0;
        SPAL_COMPLEX_T srecOut[22];
        SPAL_COMPLEX_T derotOut[NB_TSC_CORREL];
        SPAL_COMPLEX_T channelTaps[SPAL_EGPRS_XCOR_TSC_NBR];

        // Derotation 3pi/8 or pi/2
        spal_XcorDerotate ((INT16*)&RxBuffer[i + 64], (INT16*)derotOut, NB_TSC_CORREL, i);

        // Get the channel taps by correlation with the training sequence
        spal_XcorTscxcNb(g_sppGsmTscNburst[tsc], (UINT32*)derotOut, (INT16*)channelTaps);
        for (j=0;j<13;j++)
        {
            channelTaps[j].i >>= 4;
            channelTaps[j].q >>= 4;
        }

        // Calculate I^2 + Q^2 for every coeffs (note : shift limited to 15)
        spal_XcorComplexPower((INT16*)channelTaps, channelTapsEnergy[i], SPAL_EGPRS_XCOR_TSC_NBR, 15);

        if (!burstIdx)
        {
            // We reconstruct the recieved signal from estimated
            // channel and known training sequence
            spal_XcorSrec(  (INT16*) &channelTaps[1],                    // Channel Taps
                            (UINT8) 18,                                    // Reconstruction length
                            (UINT32*) &(g_sppGsmTscNburst[tsc]),        // ref. hardbit sequence
                            (UINT32*) srecOut,                        // output
                            9);                                          // reconstructed output

            noisePwr = spal_XcorErrCalc((UINT32*) srecOut, (UINT32*) &(derotOut[4]), (UINT8) 16);
        }

        // If (burst), there is only one iteration of the loop.
        // In the first iteration minEnergy = MAX_INT32,
        // so modIs is always updated with the current modulation.
        // In case of BD search, there are two iterations, and modIs is
        // updated with the modulation minimizing the noisePwr.

        if (minEnergy >= noisePwr)
        {
            minEnergy = noisePwr;
            modIs = i;
        }
    }

    spp_TofSearch (     (UINT32*)channelTapsEnergy[modIs],
                        tofSearchWindow[modIs],
                        SPAL_EGPRS_XCOR_TSC_NBR - 5  + 1, // search size
                        TOf_in_Qb,
                        TOf_sample);


    *modulation = (modIs == PSK8) ? SPP_8PSK_MOD : SPP_GMSK_MOD;

    SPP_PROFILE_FUNCTION_EXIT(spp_EgprsBlindDetection);
}

#endif // (CHIP_EDGE_SUPPORTED)























