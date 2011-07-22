//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spp_egprs_lse_prefilter.c
///
///This file contains prefilter functions of EDGE signal processing
/// @author Chang Yuguang
///
/// @date 07/06/07
//-----------------------------------------------------------------------------

#if (CHIP_EDGE_SUPPORTED)


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "sppp_gsm_private.h"
#include "sppp_egprs_private.h"
#include "spal_xcor.h"
#include "spal_mem.h"
#include "sppp_debug.h"


//------------------------------------------------------------------------------
// Global variables
//------------------------------------------------------------------------------


/// used for estimating the order of estimated H(n)
PRIVATE INT32 g_sppEgprsTapPwrMax[4][SPP_LSE_TAP_NB] SPAL_BBSRAM_GLOBAL;


//=============================================================================
// spp_LpChannelEstimation
//-----------------------------------------------------------------------------
// This function perform Linear-prediction for minimization of the outout power of the error filter
//
// @param RLS_W SPAL_COMPLEX_T*.INPUT.Address of the channel coefficients
// @param P_Array SPAL_COMPLEX_T*.OUTPUT.Address of the P_Array
// @param H_CorArray SPAL_COMPLEX_T*.OUTPUT.Address of the H_CorArray
// @param A1 SPAL_COMPLEX_T*.OUTPUT.Address of the conjugated channel coefficients
//
// @author Chang Yuguang, modified by Yordan Tabakov
// @date 30/11/2007
//=============================================================================

PROTECTED VOID spp_LpChannelEstimation(INT16 *RLS_W,
                                      INT16 *P_Array,
                                      INT16 *H_CorArray,
                                      INT16 *A1,
                                      INT8 LogChanPwr)
{

    // Here we are using a conjugated P_Array compared to the initial algo
    SPAL_COMPLEX_T LD_E, LD_K;
    SPAL_COMPLEX_T P_Array_New[QP];

    SPAL_COMPLEX_T* RLS_W_Cmp      = (SPAL_COMPLEX_T*) RLS_W;
    SPAL_COMPLEX_T* P_Array_Cmp    = (SPAL_COMPLEX_T*) P_Array ;
    SPAL_COMPLEX_T* H_CorArray_Cmp = (SPAL_COMPLEX_T*) H_CorArray;
    SPAL_COMPLEX_T* A1_Cmp         = (SPAL_COMPLEX_T*) A1;

    UINT8 i, j;

    //calculate the A1(n)=conjugate(RLS(-n))
    //conjugate the RLS
    for (i = 0; i < SPP_LSE_TAP_NB; i++)
    {
        A1_Cmp[SPP_LSE_TAP_NB - 1 - i].i =  RLS_W_Cmp[i].i;
        A1_Cmp[SPP_LSE_TAP_NB - 1 - i].q = -RLS_W_Cmp[i].q;
    }

    //convolution(h(k),h*(-k))
    spal_XcorComplexConvolution(    (INT16*)RLS_W,                // format Q(LogChanPwr)
        SPP_LSE_TAP_NB,
        (INT16*)A1_Cmp,                    // format Q(LPCH)
        SPP_LSE_TAP_NB,
        (INT16*)H_CorArray_Cmp,            // format Q(LPQP+LPSFL)
        0,
        0,
        (INT8)(LPCH + LogChanPwr - LPQP - LPSFL));


    // the recursive equations of LD algorithm are as follows:
    //
    // P_Array<m+1,k> = P<m,k> + LD_K<m+1>*P_Array<m,m+1-k>
    // P_Array<m+1,m+1> = LD_K<m+1>
    // LD_E<m+1> = (1-|LD_K<m+1>|^2)*LD_E<m>
    // LD_K(m+1) = (H_CorArray<m+1> + sum(P<m,k>*H<m+1-k>))/LD_E<m>
    // LD_E<0> = H_CorArray<0>

    LD_E = H_CorArray_Cmp[SPP_LSE_TAP_NB - 1];

    for (i = 0; i < QP; i++)
    {
        INT32 num, denom;
        INT16 delta;

        UINT8 k = MIN(i,SPP_LSE_TAP_NB - 1);

        spal_XcorComplexMatrixMultiply ((INT16*)&H_CorArray_Cmp[SPP_LSE_TAP_NB - 1 - k],    // format Q(LPQP+LPSFL)
            (INT16*)&P_Array_Cmp[i - k + 1],                    // format Q(LPQP)
            (INT16*)&LD_K,                                    // format Q(LPQP+LPSFL)
            1,
            k,
            LPQP);

        LD_K.q = -LD_K.q;

        if (i < SPP_LSE_TAP_NB - 1)
        {
            LD_K.i += H_CorArray_Cmp[SPP_LSE_TAP_NB - 2 - i].i;
            LD_K.q -= H_CorArray_Cmp[SPP_LSE_TAP_NB - 2 - i].q;
        }

        num   = - LD_K.i*LD_E.i - LD_K.q*LD_E.q;
        denom =   LD_E.i*LD_E.i + LD_E.q*LD_E.q;

        LD_K.i = spp_DivL(num,denom,LPQP);

        num   = - LD_K.i*LD_E.q - LD_K.q*LD_E.i;

        LD_K.q = spp_DivL(num,denom,LPQP);

        delta = SPP_SAT_INT16((((1<<(2*LPQP)) - 1) - LD_K.i*LD_K.i - LD_K.q*LD_K.q) >> LPQP);

        // avoid overflows
        if (delta < 0)
        {
            LD_K.i = 0;
            LD_K.q = 0;
            delta = ((1 << LPQP) - 1);
        }

        LD_E.i = SPP_SAT_INT16((delta*LD_E.i) >> LPQP);
        LD_E.q = SPP_SAT_INT16((delta*LD_E.q) >> LPQP);

        spal_XcorComplexMatrixMultiply ((INT16*)&LD_K,                // format Q(LPQP)
            (INT16*)&P_Array_Cmp[1],        // format Q(LPQP)
            (INT16*)&P_Array_New[1],    // format Q(LPQP)
            i,
            1,
            LPQP);

        for(j = 0; j < i; j++)
        {
            P_Array_Cmp[j+1].i += P_Array_New[i-j].i;
            P_Array_Cmp[j+1].q -= P_Array_New[i-j].q;
        }

        P_Array_Cmp[i+1].i =  LD_K.i;
        P_Array_Cmp[i+1].q = -LD_K.q;
  }

  P_Array_Cmp[0].i = ((1 << LPQP) - 1);
  P_Array_Cmp[0].q = 0;

}



//=============================================================================
// spp_LsePrefilter
//-----------------------------------------------------------------------------
// Pre-filter function for each burst.
//
// @param PrefilterBufs  SPP_EGPRS_PREFILTER_BUFS_T  *. INPUT.  Pointer to prefilter coefficients' structure.
//
// @author Chang Yuguang, modified by Yordan Tabakov
// @date 30/11/2007
//=============================================================================


PROTECTED VOID spp_LsePrefilter(UINT8                        tsc,
                                 SPP_EGPRS_PREFILTER_BUFS_T* PrefilterBufs,
                                 UINT32*                     NoisePower,
                                 UINT8*                      plogNoisePwr)

{
    INT32 i, j;
    UINT8 AdptOrder;
    UINT32 NoisePwr;
    UINT32 TapsPwr;
    INT8  LogChanPwr;

    SPP_PROFILE_FUNCTION_ENTRY(spp_LsePrefilter);

    //    the overall transfer function of the FIR prefilter is below in Z domain.
    //             F(Z) = z-(qh+qp)A1(z)A2(z)
    //             A1(n) = conjugate(h(-n))
    //               A2(n) = P(n)
    //        so,
    //               Hmin(n) = A1(n)*A2(n)
    //               Hmax(n) = A1(n)*conjugate(A2(n))
    //
    //        so the prefilter result is,
    //               (the back half of input)*Hmin(n)
    //               (the front half of input)*Hmax(n)



    spal_XcorChest(     (INT16 *) &PrefilterBufs->InArray[59 + SPP_LSE_TAP_NB], // format 3.9
                        (INT16 *) g_sppChestTab,
                        (INT16 *) PrefilterBufs->RLS_W,
                        SPP_LSE_TAP_NB,
                        9+15 - LPCH);


    // step 1. calculate each tap's power, and store it in g_sppEgprsTapPwrMax.
    for (i = 3; i > 0; i--)
    {
        for (j = 0; j < SPP_LSE_TAP_NB; j++)
        {
            g_sppEgprsTapPwrMax[i][j] = g_sppEgprsTapPwrMax[i - 1][j];
        }
    }

    for (i = 0; i < SPP_LSE_TAP_NB; i++)
    {
        g_sppEgprsTapPwrMax[0][i] = ( PrefilterBufs->RLS_W[i].i*PrefilterBufs->RLS_W[i].i +
                                      PrefilterBufs->RLS_W[i].q*PrefilterBufs->RLS_W[i].q ) >> 4;
    }


    // step 2. use spal_XcorComplexConvolution to compute the convolution of channel
    // with training sequenceconvolution(TS(n),H(n))

    spal_XcorSrec( (INT16*)PrefilterBufs->RLS_W, // Channel Taps
        (TRAIN_SEQ_LENGTH-7),                    // Reconstruction length
        (UINT32*) &(g_sppGsmTscNburst[tsc]),     // ref. hardbit sequence
        (UINT32*) &(PrefilterBufs->LocalTsc[0]), // output
        SPP_LSE_TAP_NB);                         // reconstructed output

    // step 3. get the training sequence error

    for (i = 0; i < 16; i++)
    {
        PrefilterBufs->LocalTsc[i].i >>= 2;
        PrefilterBufs->LocalTsc[i].q >>= 2;
    }

    // step 4. compute the noise power

    NoisePwr = spal_XcorErrCalc((UINT32*) PrefilterBufs->LocalTsc, (UINT32*) &(PrefilterBufs->InArray[67]), (UINT8) 16);
    NoisePwr >>= 4;
    // output noise power per sample
    *NoisePower=NoisePwr;

    // step 5, get the channel order

    AdptOrder = SPP_LSE_TAP_NB-1;

    while ((AdptOrder > 1) &&
            (g_sppEgprsTapPwrMax[0][AdptOrder] < NoisePwr) &&
            (g_sppEgprsTapPwrMax[1][AdptOrder] < NoisePwr) &&
            (g_sppEgprsTapPwrMax[2][AdptOrder] < NoisePwr) &&
            (g_sppEgprsTapPwrMax[3][AdptOrder] < NoisePwr))
    {
        AdptOrder--;
    }

    // step 6, null the noise taps

    for (i = AdptOrder + 1; i < SPP_LSE_TAP_NB; i++)
    {
        PrefilterBufs->RLS_W[i].i = 0;
        PrefilterBufs->RLS_W[i].q = 0;
    }

    TapsPwr   = spal_XcorComplexPower((INT16*)PrefilterBufs->RLS_W,NULL,SPP_LSE_TAP_NB,LPCH);

    LogChanPwr = spp_Log2(TapsPwr);

    // the shifter is limited to [0, 15]
    #define LP_CHAN_PWR_MIN (2*LPQP + LPSFL - 15)
    #define LP_CHAN_PWR_MAX (LPQP + LPSFL + 15 - LPCH)

    LogChanPwr = (LogChanPwr > LP_CHAN_PWR_MAX) ? LP_CHAN_PWR_MAX :
                 (LogChanPwr < LP_CHAN_PWR_MIN) ? LP_CHAN_PWR_MIN :
                  LogChanPwr;

    if (2*LPQP + LPSFL - 15 > LogChanPwr)
    {
        LogChanPwr = 2*LPQP + LPSFL - 15;
    }

    // LP algorithm to compute the P(n)
    spp_LpChannelEstimation(    (INT16*)PrefilterBufs->RLS_W,        // format Q(LPCH)
                                (INT16*)PrefilterBufs->P_Array,        // format Q(LPQP+LPSFL)
                                (INT16*)PrefilterBufs->H_CorArray,    // format Q(LPQP)
                                (INT16*)PrefilterBufs->A1,            // format Q(LPCH)
                                LogChanPwr);

    // ----------------------------------
    // compute the prefilter coefficients
    // ----------------------------------

    for (i = QP; i >= 0; i--)
    {
        PrefilterBufs->A2[QP-i].i   =  PrefilterBufs->P_Array[i].i;
        PrefilterBufs->A2[QP-i].q   =  PrefilterBufs->P_Array[i].q;
        PrefilterBufs->P_Array[i].q = -PrefilterBufs->P_Array[i].q;
    }


    // compute the Hmin using the A1(z),A2(z),and H(z)
    spal_XcorComplexConvolution((INT16*)PrefilterBufs->H_CorArray,    // format Q(LPQP+LPSFL)+LPSFL2
                                2*SPP_LSE_TAP_NB-1,
                                (INT16*)PrefilterBufs->A2,            // format Q(LPQP2)13
                                QP + 1,
                                (INT16*)PrefilterBufs->LpChan,        // format Q(LPCH)
                                QP + SPP_LSE_TAP_NB - 1,
                                0,
                                (INT8)(2*LPQP + LPSFL - LogChanPwr));


    spal_XcorComplexConvolution((INT16*)PrefilterBufs->A1,            // format Q(LPCH)11
                                SPP_LSE_TAP_NB,
                                (INT16*)PrefilterBufs->A2,            // format Q(LPQP2)13
                                QP + 1,
                                (INT16*)PrefilterBufs->PrefiltMin,    // format Q(LPCH)
                                0,
                                0,
                                LPQP);


    spal_XcorComplexConvolution((INT16*)PrefilterBufs->A1,            // format Q(LPCH)
                                SPP_LSE_TAP_NB,
                                (INT16*)PrefilterBufs->P_Array,        // format Q(LPQP)
                                QP + 1,
                                (INT16*)PrefilterBufs->PrefiltMax,    // format Q(LPCH)
                                0,
                                0,
                                LPQP);


    // ----------------------------------
    // log of the Noise power after
    // correction due to LP filter
    // ----------------------------------


    *plogNoisePwr  = spp_Log2(NoisePwr);

    NoisePwr       = spal_XcorComplexPower((INT16*)PrefilterBufs->LpChan,NULL,8,3);


    *plogNoisePwr += spp_Log2(NoisePwr);

    NoisePwr       = spal_XcorComplexPower((INT16*)PrefilterBufs->RLS_W,NULL,8,3);

    *plogNoisePwr -= spp_Log2(NoisePwr);


    SPP_PROFILE_FUNCTION_EXIT(spp_LsePrefilter);
}

#endif // (CHIP_EDGE_SUPPORTED)























