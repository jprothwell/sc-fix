//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spp_egprs_ddfse.c
///
/// @author Zhang mingjie
///
/// @date 16/08/06
//-----------------------------------------------------------------------------

#if (CHIP_EDGE_SUPPORTED)

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "sppp_gsm_private.h"
#include "sppp_egprs_private.h"
#include "spal_xcor.h"
#include "spal_vitac.h"
#include "sppp_debug.h"

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

//=============================================================================
// spp_EqualizeDdfseBck()
//-----------------------------------------------------------------------------
// Realize the equalizer function for the first half burst
//
// @param lpOut INT32* prefilter output buffer.
// @param DdfseBufs SPP_EGPRS_DDFSE_BUFS_T*. equalizer work buffers.
// @param logNoisePwr UINT8. noise power used for the equalizer metrics shift.
// @param OutSoft PUINT8. OUTPUT. soft output.
// @author Zhang mingjie, modified by Yordan tabakov
// @date 10/06/08
//
//=============================================================================
PROTECTED VOID spp_EqualizeDdfseBck(INT32*                  lpOut,
                                    SPP_EGPRS_DDFSE_BUFS_T* DdfseBufs,
                                    UINT8                   logNoisePwr,
                                    PUINT8                  OutSoft)
{

  UINT8 i, j, traceback;
  SPAL_COMPLEX_T* lpOutCmp = (SPAL_COMPLEX_T*) lpOut;

  SPP_PROFILE_FUNCTION_ENTRY(spp_EqualizeDdfse);

  // -----------------------------------------
  // prefilter and equalize first half burst
  // -----------------------------------------

  // invert the channel taps in order to compute the inverse of the metrics
  for (i = 0; i < ALL_TAP_LEN; i++)
  {
    DdfseBufs->LpChan[i].i = -DdfseBufs->LpChan[i].i;
    // DdfseBufs->LpChan[i].q = DdfseBufs->LpChan[i].q;

  }

  // prefilter
  spal_XcorComplexConvolution(  (INT16*)(DdfseBufs->InArray),     // format 3.9
                                74,
                                (INT16*)(DdfseBufs->PrefiltMax),  // format Q(LPCH)
                                QP + SPP_LSE_TAP_NB,
                                (INT16*)lpOutCmp,                 // format 3.9
                                SPP_LSE_TAP_NB - 2,
                                74,
                                LPCH);


  // calculate the BMML
  spal_XcorBmmlZf(                (INT16*)DdfseBufs->LpChan,                // format Q(LPCH)
                                (INT16*)DdfseBufs->BmmlZfMetric,        // format 3.9
                                SVAL_FRAC_BITS + LPCH - 9);

  // equalize half burst
  spal_VitacDdfseHalfBurst(        (UINT32*)&lpOutCmp[HALF_BURST_LEN - 1],
                                HALF_BURST_LEN,
                                -1, // backward
                                logNoisePwr,
                                &DdfseBufs->TscSym[ZF_TAP_LEN],
                                (UINT32*)&DdfseBufs->BmmlZfMetric[0],
                                (UINT32*)&DdfseBufs->BmmlZfMetric[STATE_LEN * STATE_LEN + 1],
                                DdfseBufs->SoftOutputBck);


  // traceback from 7 (1,1,1)
  traceback = 7;
  for (j = HALF_BURST_LEN - 1; j > HALF_BURST_LEN - 3 ; j--)
  {
      traceback = (UINT8)DdfseBufs->SoftOutputBck[traceback*HALF_BURST_LEN*4 + 4*j + 3];
  }

  for (j = HALF_BURST_LEN - 3; j > 0 ; j--)
  {
      for (i = 0; i < 3; i++)
      {
          OutSoft[3 * (HALF_BURST_LEN - 3 - j) + i] = DdfseBufs->SoftOutputBck[traceback*HALF_BURST_LEN*4 + 4*j + i];
      }
      traceback = (UINT8)DdfseBufs->SoftOutputBck[traceback*HALF_BURST_LEN*4 + 4*j + 3];
  }


  SPP_PROFILE_FUNCTION_EXIT(spp_EqualizeDdfse);
}


//=============================================================================
// spp_EqualizeDdfseFwd()
//-----------------------------------------------------------------------------
// Realize the equalizer function for the second half burst
//
// @param lpOut INT32* prefilter output buffer.
// @param DdfseBufs SPP_EGPRS_DDFSE_BUFS_T*. equalizer work buffers.
// @param logNoisePwr UINT8. noise power used for the equalizer metrics shift.
// @param OutSoft PUINT8. OUTPUT. soft output.
// @author Zhang mingjie, modified by Yordan tabakov
// @date 10/06/08
//
//=============================================================================

PROTECTED VOID spp_EqualizeDdfseFwd(INT32*                  lpOut,
                                    SPP_EGPRS_DDFSE_BUFS_T* DdfseBufs,
                                    UINT8                   logNoisePwr,
                                    PUINT8                  OutSoft)
{

  UINT8 i, j, traceback;
  SPAL_COMPLEX_T* lpOutCmp = (SPAL_COMPLEX_T*) lpOut;

  SPP_PROFILE_FUNCTION_ENTRY(spp_EqualizeDdfse);


  // This buffer is used as destination for the filtered samples.
  // The tail needs to be cleaned
  for (i = 2*74; i < 157; i++)
  {
        lpOut[i] = 0;
  }

  // -----------------------------------------
  // prefilter and equalize second half burst
  // -----------------------------------------

  // invert the channel taps in order to compute the inverse of the metrics
  for (i = 0; i < ALL_TAP_LEN; i++)
  {
    // DdfseBufs->LpChan[i].i = DdfseBufs->LpChan[i].i;
    DdfseBufs->LpChan[i].q = -DdfseBufs->LpChan[i].q;
  }

  // prefilter
  spal_XcorComplexConvolution(  (INT16*)(DdfseBufs->InArray + 74),// format 3.9
                                74,
                                (INT16*)DdfseBufs->PrefiltMin,      // format Q(LPCH)
                                QP + SPP_LSE_TAP_NB,
                                (INT16*)(lpOutCmp + 74),          // format 3.9
                                SPP_LSE_TAP_NB - 2 + QP,
                                74,
                                LPCH);

  // calculate the BMML
  spal_XcorBmmlZf(                (INT16*)DdfseBufs->LpChan,            // format 1.11
                                (INT16*)DdfseBufs->BmmlZfMetric,    // format 3.9
                                SVAL_FRAC_BITS + LPCH - 9);


  // equalize second half burst
  spal_VitacDdfseHalfBurst(        (UINT32*)&lpOutCmp[HALF_BURST_LEN + TRAIN_SEQ_LENGTH],
                                HALF_BURST_LEN,
                                1, // forward
                                logNoisePwr,
                                &DdfseBufs->TscSym[TRAIN_SEQ_LENGTH - 1 - ZF_TAP_LEN],
                                (UINT32*)&DdfseBufs->BmmlZfMetric[0],
                                (UINT32*)&DdfseBufs->BmmlZfMetric[STATE_LEN * STATE_LEN + 1],
                                DdfseBufs->SoftOutputFwd);


  // traceback from 7 (1,1,1)
  traceback = 7;
  for (j = HALF_BURST_LEN - 1; j > HALF_BURST_LEN - 3 ; j--)
  {
      traceback = (UINT8)DdfseBufs->SoftOutputFwd[traceback*HALF_BURST_LEN*4 + 4*j + 3];
  }

  for (j = HALF_BURST_LEN - 3; j > 0 ; j--)
  {
      for (i = 0; i < 3; i++)
      {
          OutSoft[3 * (HALF_BURST_LEN - 4 + j) + i] = DdfseBufs->SoftOutputFwd[traceback*HALF_BURST_LEN*4 + 4*j + i];
      }
      traceback = (UINT8)DdfseBufs->SoftOutputFwd[traceback*HALF_BURST_LEN*4 + 4*j + 3];
  }


  SPP_PROFILE_FUNCTION_EXIT(spp_EqualizeDdfse);
}



#endif // (CHIP_EDGE_SUPPORTED)























