//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file sppp_egprs_rx_tx.h
///
/// @date 15/06/07
/// @author peng,lang
//-----------------------------------------------------------------------------


#ifndef __SPP_EGPRS_CODEC_H__
#define __SPP_EGPRS_CODEC_H__


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "spp_gsm.h"
#include "spal_xcor.h"
#include "global_macros.h"

//------------------------------------------------------------------------------
// LOCAL CONSTANT
//------------------------------------------------------------------------------


// Misc defines
// ------------
#define BUFFER_CLEAR(buffer, len) \
{\
    UINT16 idx;\
    for (idx = 0; idx< (len+7)/8; idx++)  \
    {\
        buffer[idx] = 0;\
    }\
}

#define SWAP_DE(pIn, a, b)    \
{ \
    UINT8 swap = pIn[a]; \
    pIn[a] = pIn[b];     \
    pIn[b] = swap;       \
}

#define SWAP_EN(pIn, a, b, ta, tb)          \
                                            \
    ta = (!!(pIn[a / 8] & 1 << (a % 8)));   \
    tb = (!!(pIn[b / 8] & 1 << (b % 8)));   \
                                            \
    if (ta^tb)                              \
    {                                       \
        if (ta)                             \
        {                                   \
            pIn[a / 8] &= ~(1 << (a % 8));  \
            pIn[b / 8] |=  (1 << (b % 8));  \
        }                                   \
        else                                \
        {                                   \
            pIn[a / 8] |=  (1 << (a % 8));  \
            pIn[b / 8] &= ~(1 << (b % 8));  \
        }                                   \
    }


#define MOVEBITESPP(a, i, b, j) b[(j)/8]|=((!!(a[(i)/8]&1<<((i)%8)))<<((j)%8))
#define CLEARBITESPP(b, j)      b[(j)/8]&=(~(1<<((j)%8)))

#define MOVEBIT2(a, i, b, k, j) b[(k)+((j)/8)]|=((!!(a[(i)/8]&1<<((i)%8)))<<((j)%8))
#define CLEARBIT2(b, i, j)      b[(i)+((j)/8)]&=(~(1<<((j)%8)))

#define RDBIT32(b,j)          ((b[(j)/32] >>  ((j)%32)) & 1)
#define CLRBIT32(b,j)           b[(j)/32] &= (~(1<<((j)%32)))
#define SETBIT32(b,j)           b[(j)/32] |= ( (1<<((j)%32)))

#define RD2BITS8(b,j)        (((b)[(j)/4] >> (2*((j)%4))) & 0x3)
#define CLR2BITS8(b,j)         (b)[(j)/4] &= (~((0x3)<<(2*((j)%4))))
#define SET2BITS8(b,j,val)     (b)[(j)/4] &= (~((0x3)<<(2*((j)%4)))); \
                               (b)[(j)/4] |= ( ((val)<<(2*((j)%4))))



// Prefiler defines
// ----------------
// the LPQP is the scale for LP prefilter.
// the LPSFL is the sub of LPQP and
// the LPCH  is the scale of the channel taps
#define LPQP             11
#define LPSFL             1
#define LPCH             11


// IR defines
// ----------
#define OverFlow_Threshold 6

#define EDGE_WS 512
#define IR_MOD_SNS(_val)  ((_val) & 2047)
#define ROUND_DISTANCE(_V1,_V2, _Step)     (((INT32)(_V1-_V2)>=0)?  (_V1- _V2): (_V1-_V2+_Step))
#define IR_GET_BSN1(data)  ( ((data[1]&0xC0)<<3)|(data[2]<<1)|(data[3]&1))
#define IR_GET_BSN2(data)  (((data[3]&0xFE)<<2) |(data[4]&7))

#define MIN_METRIC(_in, _num, _out) { UINT8 idx; _out = 0;\
    for (idx = 0; idx < _num; idx++) \
    _out = _in[idx] < _in[_out] ? idx : _out;}



//------------------------------------------------------------------------------
// Global variables and constants
//------------------------------------------------------------------------------


EXPORT CONST UINT16        g_sppEgprsHInSPos[1];
EXPORT CONST UINT16        g_sppEgprsDInSPos[9];
EXPORT CONST UINT16        g_sppEgprsHTailLen[9];
EXPORT CONST UINT16        g_sppEgprsDTailLen[9];
EXPORT CONST UINT16        g_sppEgprsDInLng[9];
EXPORT CONST UINT16        g_sppEgprsDHalfLth[3];
EXPORT CONST UINT8         g_sppEgprsUsf36[8][5];
EXPORT CONST INT16* CONST  g_sppUsfTables[2];
EXPORT CONST SPP_COMPLEX_T g_sppEgprsInvTrain[8][11][16];
EXPORT CONST UINT8         g_sppHeaderDecodeMcs789[29][3];
EXPORT CONST UINT8         g_sppHeaderDecodeMcs56[6][2];
EXPORT CONST UINT8         g_sppHeaderDecodeMcs1234[13][2];
// TODO : to be removed


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

// SPP PUBLIC FUNCTIONS
///@defgroup egprs_rxtx_func Rx/Tx processing functions
///@{



//=============================================================================
// spp_EgprsItlv
//-----------------------------------------------------------------------------
// Function performing the interleaving.
//
// @param pD             UINT8*   INPUT    Pointer to Data.
// @param pD1            UINT8*   INPUT    Pointer to Data1.
// @param pHd            UINT8*   INPUT    Pointer to Header.
// @param pUsfm          UINT8*   INPUT    Pointer to Usfm.
// @param ItLvDataOut    UINT8*   OUTPUT   Pointer to the Data after Interleaving.
// @param ItLvHeadOut    UINT8*   OUTPUT   Pointer to the Header after Interleaving.
// @param ChMod          UINT8    OUTPUT   Coding Scheme.
//
// @author Nadia Touliou
// @date 2008/02/01
//=============================================================================
/*PRIVATE VOID spp_EgprsItlv(UINT8* pD,
                           UINT8* pD1,
                           UINT8* pHd,
                           UINT8* pUsfm,
                           UINT8* ItLvDataOut,
                           UINT8* ItLvHeadOut,
                           UINT8  ChMod);*/



///@}


///@defgroup egprs_lse_prefilter_func Lse and prefilter fuctions
///@{


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

PROTECTED VOID spp_LsePrefilter( UINT8                       tsc,
                                 SPP_EGPRS_PREFILTER_BUFS_T* PrefilterBufs,
                                 UINT32*                     NoisePower,
                                 UINT8*                      plogNoisePwr  );

//=============================================================================
// spp_LpChannelEstimation
//-----------------------------------------------------------------------------
// This function perform Linear-prediction for minimization of the outout power of the error filter
//
// @param RLS_W INT16*.INPUT.Address of the channel coefficients
// @param P_Array INT16*.OUTPUT.Address of the P_Array
// @param H_CorArray INT16*.OUTPUT.Address of the H_CorArray
// @param A1 INT16*.OUTPUT.Address of the conjugated channel coefficients
//
// @author Chang Yuguang, modified by Yordan Tabakov
// @date 30/11/2007
//=============================================================================

PROTECTED VOID spp_LpChannelEstimation( INT16 *RLS_W,
                                         INT16 *P_Array,
                                         INT16 *H_CorArray,
                                         INT16 *A1,
                                         INT8 LogChanPwr   );



///@}


///@defgroup egprs_equalizer_func Egprs equalizer fuctions
///@{


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
                                    PUINT8                  OutSoft);


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
                                    PUINT8                  OutSoft);

///@}


///@defgroup egprs_dciq_func Egprs DC offset and IQ imbalance correction fuctions
///@{



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

PROTECTED UINT8 spp_Psk8DcIqEstimation(INT16*         inOut,
                                   UINT8                    enableDc,
                                   UINT8                    enableIq,
                                   SPP_COMPLEX_T*           DcIqBufs,
                                   SPP_COMPLEX_T*           dc_est,
                                   SPP_COMPLEX_T*           iq_est);

///@}


///@defgroup egprs_ir_func Egprs Incremental Redundancy fuctions
///@{


//=============================================================================
//  spp_IrWriteBuffer
//-----------------------------------------------------------------------------
/// write data to the buffer while it isn`t crc correctly.
///
/// @param data UINT8*. INPUT. data need saved
/// @param sel UINT8. INPUT.  input selection
/// @param IR SPP_EGPRS_IR_CTX_T*. the IR information.
/// @param Header SPP_HEADER_RESULT_T*.  header information
///
/// @author zhangchangchun
/// @date 2007/08/14
//=============================================================================
PROTECTED VOID spp_IrWriteBuffer(UINT8 *data,
                                UINT8 sel,
                                SPP_EGPRS_IR_CTX_T *IR,
                                SPP_EGPRS_IR_BUF_T *irData,
                                SPP_HEADER_RESULT_T* Header);

//=============================================================================
//  spp_IrClearBuffer
//-----------------------------------------------------------------------------
/// clear data in the buffer while it is crc correctly.
///
/// @param sel UINT8. INPUT.  input selection
/// @param IR SPP_EGPRS_IR_CTX_T*. the IR information.
/// @param Header SPP_HEADER_RESULT_T*.  header information
///
/// @author zhangchangchun
/// @date 2007/08/14
//=============================================================================
PROTECTED VOID spp_IrClearBuffer(UINT8 sel,
                                SPP_EGPRS_IR_CTX_T *IR,
                                SPP_HEADER_RESULT_T* Header);


//=============================================================================
//  spp_IrCombine
//-----------------------------------------------------------------------------
// Combine all the data in the buffer in the same family.
//
// @param pIn                 UINT8*.      IN  input data to be combined
// @param sel                 UINT8.       IN  input combine selection
// @param Header              SPP_HEADER_RESULT_T* IN  header information
// @param IR                  SPP_EGPRS_IR_CTX_T*     IN  IR information.
// @param pTemp               UINT8*.      IN  temporary buffer used for recombination
// @param pOut                UINT8*.      OUT combined output
// @param dataPuncturingTable UINT32*      OUT puncturing scheme of the output buffer
//
// @author zhangchangchun
// @date 2007/08/14
//=============================================================================
PROTECTED VOID spp_IrCombine(UINT8                    *pIn,
                              UINT8                     sel,
                              SPP_HEADER_RESULT_T    *Header,
                              SPP_EGPRS_IR_CTX_T    *IR,
                              SPP_EGPRS_IR_BUF_T    *irData,
                              UINT8                    *pTemp,
                              UINT8                    *pOut,
                              UINT32                *dataPuncturingTable);


//=============================================================================
//  spp_IrUpdateVr
//-----------------------------------------------------------------------------
/// update the BSN which has a value one higher than the highest BSN yet received.
///
/// @param NewBSN INT16. INPUT. the new coming BSN.
/// @param IR SPP_EGPRS_IR_CTX_T*. Incremental Redundancy related structure.
///
/// @author zhangchangchun
/// @date 2007/08/14
//=============================================================================
PROTECTED VOID spp_IrUpdateVr(INT16 NewBSN, SPP_EGPRS_IR_CTX_T *IR);

//=============================================================================
//  spp_IrUpdateVq
//-----------------------------------------------------------------------------
/// update the BSN not yet received.
///
/// @param NewBSN INT16. INPUT. the new coming BSN.
/// @param IR SPP_EGPRS_IR_CTX_T*. Incremental Redundancy related structure.
///
/// @author zhangchangchun
/// @date 2007/08/14
//=============================================================================
PROTECTED VOID spp_IrUpdateVq(INT16 NewBSN, SPP_EGPRS_IR_CTX_T *IR);

///@}

///@defgroup egprs_fof_estimation FOF estimation related fuctions
///@{

//======================================================================
/// spp_EgprsFofEstimation
///
///    This function is used to estimate the frequence offset by burst.
///
/// @param  InSamples   UINT32*        INPUT. Pointer to received input samples.
/// @param  InSoftBits  UINT8*         INPUT. Pointer to the soft decision values.
/// @param  Tsc         UINT8          INPUT. The training sequence number.
/// @param  ChTaps      SPP_COMPLEX_T* INPUT. Pointer to the estimated Channel Taps.
/// @param  Fof         INT16*         OUTPUT. Pointer to the FOF estimated.
///
/// @author Nadia Touliou
///
/// @date 20/04/08
//======================================================================
PROTECTED VOID spp_EgprsFofEstimation (  UINT32* InSamples,
                                       UINT8* InSoftBits,
                                       UINT8 Tsc,
                                       SPP_COMPLEX_T* ChTaps,
                                       UINT32 NoisePwr,
                                       UINT32 Power,
                                       INT16* Fof);

//=============================================================================
// spp_EgprsBuildRefNBurst
//-----------------------------------------------------------------------------
/// Rebuilds the theoretical received normal burst from the hard bits decisions
/// and the estimated channel taps.
///
/// @param Insoftvalues UINT8* INPUT.  Pointer to the soft decision values.
/// @param tsc          UINT8* INPUT.  Variable that determines the training sequence.
/// @param OutrefBurst  UINT8* OUTPUT. Pointer to the theoritical reference burst.
///
/// @author Nadia Touliou
///
/// @date 20/04/08
//=============================================================================

PROTECTED VOID spp_EgprsBuildRefNBurst( UINT8* Insoftvalues,
                                     UINT8  tsc,
                                     UINT32* OutrefBurst);





///@}


#endif  // __CODEC_H__






















