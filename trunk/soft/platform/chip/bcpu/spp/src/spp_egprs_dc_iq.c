//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spp_egprs_dc_iq.c
///
/// @author Zhao Yang
///
/// @date 12/06/07
//-----------------------------------------------------------------------------

#if (CHIP_EDGE_SUPPORTED)

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "sppp_gsm_private.h"
#include "sppp_egprs_private.h"
#include "spal_xcor.h"

//======================================================================
//    @function spp_Psk8DcIqEstimation.c
//
//    This function is used to estimate the DC offset and IQ imbalance.
//
//  @param inOut SPP_COMPLEX_T* data buffer
//  @param enableDc UINT8 enable the DC offset correction
//  @param enableIq UINT8 enable the IQ impairment correction
//  @param dc SPP_COMPLEX_T* DC offset estimation
//  @param iq SPP_COMPLEX_T* IQ imbalance estimation
//  @return dc/iq correction necessary
//  @author Zhao Yang, modified by Yordan Tabakov, modified by Nadia Touliou
//
//  @date 30/11/07
//======================================================================
PROTECTED UINT8 spp_Psk8DcIqEstimation(INT16*                   inOut,
                                      UINT8                    enableDc,
                                      UINT8                    enableIq,
                                      SPP_COMPLEX_T*           DcIqBufs,
                                      SPP_COMPLEX_T*           dc_est,
                                      SPP_COMPLEX_T*           iq_est)
{

    SPAL_COMPLEX_T* inOutCmp = (SPAL_COMPLEX_T*) inOut;
    INT32 Den = 0;

    if((enableDc) || (enableIq))
    {

        SPP_COMPLEX_T dc_estimat;

        // Estimation
        spal_XcorComplexMatrixMultiply((INT16 *)&inOutCmp[68],(INT16 *)g_sppEgprsInvTrain, (INT16 *)DcIqBufs, 11, 16, 13);

        dc_estimat.i =  DcIqBufs[10].i;
        dc_estimat.q =  DcIqBufs[10].q;

        Den = (INT32) spal_XcorComplexPower((INT16 *)DcIqBufs, NULL, 5, 0);

        if (Den)
        {
            SPAL_CMPLX32_T Num;
            SPP_COMPLEX_T iq_estimat;

            Num = spal_XcorComplexMatrixMultiply ((INT16*)DcIqBufs, (INT16*)&DcIqBufs[5], NULL, 1, 5, 0);

            iq_estimat.i = spp_DivL(Num.i, Den, 11);
            iq_estimat.q = spp_DivL(Num.q, Den, 11);

            dc_est->i = (enableDc) ? dc_estimat.i : 0;
            dc_est->q = (enableDc) ? dc_estimat.q : 0;

            iq_est->i = (!enableIq) ? 0 : (iq_estimat.i > 410) ? 410 : (iq_estimat.i < -410) ? -410 : iq_estimat.i;
            iq_est->q = (!enableIq) ? 0 : (iq_estimat.q >  82) ?  82 : (iq_estimat.q <  -82) ?  -82 : iq_estimat.q;

        }// End if(Den)

    }// End if((enableDc) || (enableIq))


    return ((enableDc) || (enableIq) || (Den));
}


#endif // (CHIP_EDGE_SUPPORTED)
