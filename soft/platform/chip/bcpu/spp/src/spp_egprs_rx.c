//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spp_egprs_rx.c
///
/// Channel Encoding of information bits.
///
/// @date 2007/01/04
/// @author peng,lang
//-----------------------------------------------------------------------------

//
// DESIGN THOUGHT DESCRIPTION
// According to EDGE channel coding specification
// Input to the CODEC module  is
// |----|-----------|-------------|-------------|
// |usf |header part|data part one|data part two|
// |----|-----------|-------------|-------------|
// Output from the CODEC module is
// |--------|---------|-------|-------|
// |burst 1 | burst 2 |burst 3|burst 4|
// |--------|---------|-------|-------|
//


#if (CHIP_EDGE_SUPPORTED)


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "baseband_defs.h"
#include "sppp_cfg.h"
#include "sppp_gsm_private.h"
#include "sppp_egprs_private.h"
#include "spal_xcor.h"
#include "spal_vitac.h"
#include "spal_itlv.h"
#include "spal_cap.h"
#include "sppp_debug.h"

//------------------------------------------------------------------------------
// functions
//------------------------------------------------------------------------------


//=============================================================================
// spp_EgprsEqualizeNBurst
//-----------------------------------------------------------------------------
/// demodulation module
///
// @param InFixed T_COMPLEX_INT32    input complex sequence
// @param TSC UINT8                  pointer to TSC
// @param pDemod UINT8*               output complex sequence
//
/// @date 09/06/08
/// @author yordan
//=============================================================================
PUBLIC VOID spp_EgprsEqualizeNBurst(UINT32 *RxBuffer,
                                    UINT8 Tsc,
                                    UINT32* EqBufferOut,
                                    SPP_UNCACHED_BUFFERS_T* UncachedBuffers,
                                    SPP_N_BURST_RESULT_T* Res)
{
    UINT32 NoisePwr=0;
    UINT32 Power=0;
    UINT8 LogNoisePwr;
    UINT8 i;
    UINT8 correctDcIq;
    SPP_COMPLEX_T Iq;

    SPP_PROFILE_FUNCTION_ENTRY(spp_EgprsEqualizeNBurst);


    // -----------------------------------------
    // This part can be started at half burst IT
    // -----------------------------------------

    // DC/IQ estimation (for 8PSK modulation only!)
    correctDcIq = spp_Psk8DcIqEstimation(
                            (INT16*)RxBuffer,
                             g_sppGlobalParams.flag_enableDC,
                             g_sppGlobalParams.flag_enableIQ,
                             UncachedBuffers->EgprsPrefilterBufs.DcIqBufs,
                             &Res->Dco,
                             &Iq);

    if (correctDcIq)
    {
        // DC/IQ correct first half burst
        spal_XcorDcIqCompensate (   (INT16*)RxBuffer,
                                    (INT16*)&Res->Dco,
                                    (INT16*)&Iq,
                                    BB_BURST_HALF_SIZE,
                                    11);
    }

    // de-rotate first half burst
    spal_XcorDerotate(    (INT16*)RxBuffer,
                        (INT16*)(UncachedBuffers->EgprsPrefilterBufs.InArray),
                        BB_BURST_HALF_SIZE,
                        PSK8);


    // lse channel and prefilter estimation
    spp_LsePrefilter(    Tsc,
                        &UncachedBuffers->EgprsPrefilterBufs,
                        &NoisePwr,
                        &LogNoisePwr);


    // map training sequence (to be used in equalizer
    for (i = 0; i < TRAIN_SEQ_LENGTH; i++)
    {
      UncachedBuffers->EgprsDdfseBufs.TscSym[i] = ((g_sppGsmTscNburst[Tsc] >> i) & 1) ? 1 : 7;
    }


    // filter and equalize first half burst
    spp_EqualizeDdfseBck(RxBuffer,
                        &UncachedBuffers->EgprsDdfseBufs,
                        LogNoisePwr,
                        (UINT8*)EqBufferOut);


    // -----------------------------------------
    // This part need to wait for
    // second half burst IT
    // -----------------------------------------

    // TODO : synchronization...

    if (correctDcIq)
    {
        // DC/IQ correct second half burst
        spal_XcorDcIqCompensate (   (INT16*)RxBuffer[BB_BURST_HALF_SIZE],
                                    (INT16*)&Res->Dco,
                                    (INT16*)&Iq,
                                    GSM_BURST_LEN-BB_BURST_HALF_SIZE,
                                    11);
    }

    // de-rotate second half burst
    spal_XcorDerotate(    (INT16*)&RxBuffer[BB_BURST_HALF_SIZE],
                        (INT16*)(&UncachedBuffers->EgprsPrefilterBufs.InArray[BB_BURST_HALF_SIZE]),
                        GSM_BURST_LEN-BB_BURST_HALF_SIZE,
                        PSK8);


    // filter and equalize second half burst
    spp_EqualizeDdfseFwd(RxBuffer,
                        &UncachedBuffers->EgprsDdfseBufs,
                        LogNoisePwr,
                        (UINT8*)EqBufferOut);


    // Snr estimation
    Power = spal_XcorComplexPower ((INT16 *) ((UncachedBuffers->EgprsPrefilterBufs).InArray),
                                      NULL,
                                      148,
                                      0);
    Power /= 148;
    Res->Pwr = spp_10log10(Power);
    if (Power > NoisePwr)
        Res->Snr =  spp_10log10(Power-NoisePwr)-spp_10log10(NoisePwr);
    else
        Res->Snr =- spp_10log10(NoisePwr);


    // frequency offset estimation
    spp_EgprsFofEstimation ( (UINT32*)((UncachedBuffers->EgprsPrefilterBufs).InArray),
                             (UINT8*)EqBufferOut,
                              Tsc,
                              (UncachedBuffers->EgprsPrefilterBufs).RLS_W,
                              NoisePwr,
                              Power,
                              &Res->FOf);

    SPP_PROFILE_FUNCTION_EXIT(spp_EgprsEqualizeNBurst);
}





//=============================================================================
// spp_GetMetric
//-----------------------------------------------------------------------------
// used in viterbi decoding compute path metric using received data
//
// @param received UINT8*. received noised data
// @param expected UINT8*.  expected data
// @param depth INT16. bits number
//
// @author peng lang
// @date 2007/01/04
//=============================================================================
PRIVATE INT32 spp_GetMetric(UINT8 *received, UINT8 *expected, INT16 depth)
{
    INT32 metric;
    UINT8 i;
    UINT8 rd[36] = {0};

    INT8 first;
    INT8 rx;
    INT8 expand2;

    for(i = 0; i < depth; i++)
    {
      rd[i] = received[i];
    }

    metric = 0;
    for (i = 0; i < depth; i++)
    {
        first = rd[i] & 0x80;

        if (!first)
        {
            rx = (INT8)(rd[i]);
        }
        else
        {
            rx = (INT8)((rd[i] & 0x7f) * (-1));
        }

        if (expected[i / 8] & (1 << (i % 8)))
        {
            expand2 = -(INT8)0x7f;
        }
        else
        {
            expand2 = (INT8)0x7f;
        }

        metric += (rx - expand2) * (rx - expand2);
    }

    return metric;
}

//=============================================================================
// spp_EgprsGetUSF
//-----------------------------------------------------------------------------
// usf decoding
//
// @param DecodeIn UINT8[4][348] INPUT. input received data
// @param DecodeOut UINT8* OUTPUT. output decoded data
// @param BPS UINT8, bit per symbol
//
// @author zhangchangchun
// @date 2007/09/14
//=============================================================================
PUBLIC UINT8 spp_EgprsGetUSF(UINT32* BufferIn)
{
    INT32 i;
    UINT8 k;
    INT32 maxi;
    INT32 count[8] = {0};
    UINT8 usf[36];
    UINT8 *pBurst = (UINT8*)BufferIn;

    for(k = 0; k < 36; k++)
    {
        usf[k] = pBurst[g_sppUsfTables[0][k]];
    }

    for (i = 0; i < 8; i++)
    {
        count[i] = spp_GetMetric(usf, (UINT8*)g_sppEgprsUsf36[i], 36);
    }
    MIN_METRIC(count, 8, maxi);

       return (UINT8)(maxi & 0x7);
}



//=============================================================================
// spp_MappingD12
//-----------------------------------------------------------------------------
// mapping to a burst
//
// @param pIn UINT8* input data
// @param pDi UINT8* OUTPUT. output data part
// @param pHi UINT8* OUTPUT. output header part
// @param mb INT32 burst number
// @param chSubMode UNIT8 mcs coding mode
//
// @author peng lang
// @date 2007/01/04
//=============================================================================
INLINE VOID spp_MappingD12(UINT8* pIn, UINT8* pDi, UINT8* pHi, INT32 mB, SPP_HEADER_TYPE_T HeadType)
{
    INT32 i;

    switch (HeadType)
    {
        case SPP_HEADER_TYPE2: // for MCS 5,6
            SWAP_DE(pIn, 142, 155);
            SWAP_DE(pIn, 144, 158);
            SWAP_DE(pIn, 145, 161);
            SWAP_DE(pIn, 147, 164);
            SWAP_DE(pIn, 148, 167);
            SWAP_DE(pIn, 150, 170);
            SWAP_DE(pIn, 151, 173);
            SWAP_DE(pIn, 176, 195);
            SWAP_DE(pIn, 179, 196);
            SWAP_DE(pIn, 182, 198);
            SWAP_DE(pIn, 185, 199);
            SWAP_DE(pIn, 188, 201);
            SWAP_DE(pIn, 191, 202);
            SWAP_DE(pIn, 194, 204);

            for (i = 0; i < 156; i++)
            {
                // MOVEBIT(pIn, i, pDi, i+312*mB);
                pDi[i + 312 * mB] = pIn[i];
            }

            for (i = 156; i < 168; i++)
            {
                // MOVEBIT(pIn, i, pHi, i+25*mB-156);
                pHi[i + 25 * mB - 156] = pIn[i];
            }

            for (i = 179; i < 192; i++)
            {
                // MOVEBIT(pIn, i, pHi, i+25*mB-167);
                pHi[i + 25 * mB - 167] = pIn[i];
            }
            for (i = 192; i < 348; i++)
            {
                // MOVEBIT(pIn, i, pDi, i+312*mB-36);
                pDi[i + 312 * mB - 36] = pIn[i];
            }
            break;

        case SPP_HEADER_TYPE1: // for MCS 7,8,9
            SWAP_DE(pIn, 142, 155);
            SWAP_DE(pIn, 144, 158);
            SWAP_DE(pIn, 145, 161);
            SWAP_DE(pIn, 147, 164);
            SWAP_DE(pIn, 148, 167);
            SWAP_DE(pIn, 150, 170);
            SWAP_DE(pIn, 151, 173);
            SWAP_DE(pIn, 176, 195);
            SWAP_DE(pIn, 179, 196);
            SWAP_DE(pIn, 182, 198);
            SWAP_DE(pIn, 185, 199);
            SWAP_DE(pIn, 188, 201);
            SWAP_DE(pIn, 191, 202);
            SWAP_DE(pIn, 194, 204);

            for (i = 0; i < 153; i++)
            {
                // MOVEBIT(pIn, i, pDi, i+306*mB);
                pDi[i + 306 * mB] = pIn[i];
            }
            for (i = 153; i < 168; i++)
            {
                // MOVEBIT(pIn, i, pHi, i+31*mB-153);
                pHi[i + 31 * mB - 153] = pIn[i];
            }
            for (i = 179; i < 195; i++)
            {
                // MOVEBIT(pIn, i, pHi, i+31*mB-164);
                pHi[i + 31 * mB - 164] = pIn[i];
            }
            for (i = 195; i < 348; i++)
            {
                // MOVEBIT(pIn, i, pDi, i+306*mB-42);
                pDi[i + 306 * mB - 42] = pIn[i];
            }
            break;

        default:
            break;
    }
}



//=============================================================================
//  spp_EgprsDecodeHeader
//-----------------------------------------------------------------------------
// header decoding
//
// @param head UINT8*. INPUT. input header received data
// @param mode UINT8 coding mode
// @param EBL_Num UINT8* crc check result return
// @param pDecc UINT8* OUTPUT. output header decoded data
//
// @author zhangchangchun
// @date 2007/08/14
//=============================================================================
//======================================================================
PROTECTED VOID spp_EgprsDecodeHeader(UINT8 *BufferIn,
                                    SPP_HEADER_TYPE_T HeadType,
                                    UINT8 Usf,
                                    UINT8 *BufferOut,
                                    SPP_UNCACHED_BUFFERS_T* UncachedBuffers,
                                    SPP_HEADER_RESULT_T* Res)
{
    UINT8 loop;
    UINT8 punc_length;
    INT32 i, k;
    UINT8 CPS;
    UINT16 deltaBSN;

    SPP_CODING_SCHEME_T coding_type;
    SPAL_VITAC_DEC_CFG_T VitacDecCfg;

    UINT8 *pVitacIn  = (UINT8 *)UncachedBuffers->EgprsDecodeVitac.CombinedData; // 102 UINT32 required, 459 available
    UINT8 *pVitacOut = (UINT8 *)UncachedBuffers->EgprsDecodeVitac.PunctTable;   //  20 UINT32 required,  58 available

    SPP_PROFILE_FUNCTION_ENTRY(spp_EgprsDecodeHeader);

    switch (HeadType)
    {
    case SPP_HEADER_TYPE3:
        punc_length=40;
        coding_type = CS_HEAD_3;
        break;
    case SPP_HEADER_TYPE2:
        punc_length=0;
        coding_type = CS_HEAD_2;
        break;
    case SPP_HEADER_TYPE1:
    default:
        punc_length=11;
        coding_type = CS_HEAD_1;
        break;
    }


    VitacDecCfg.inSoftBit     = (UINT32*) pVitacIn;
    VitacDecCfg.punctTable    = (UINT32*) g_sppCsPuncturingTable [coding_type]; // + 58 Offset for the Downlink.
    VitacDecCfg.vitacDecParam = g_sppVitacDecParam [coding_type];
    VitacDecCfg.convPoly0     = g_sppVitacPolyres[g_sppVitacDecParam[coding_type].resPoly0];
    VitacDecCfg.convPoly1     = g_sppVitacPolyres[g_sppVitacDecParam[coding_type].resPoly1];
    VitacDecCfg.convPoly2     = g_sppVitacPolyres[g_sppVitacDecParam[coding_type].resPoly2];

    // prepare header data for F3T2 decoding
    for (loop=0; loop<3; loop++)
    {
        for (i=0;i<((UINT16) VitacDecCfg.vitacDecParam.nbSymbol-punc_length);i++)
        {
            pVitacIn[i+loop*((UINT16) VitacDecCfg.vitacDecParam.nbSymbol-punc_length)]=BufferIn[i];
        }
    }

    // clear output buffer
    for (i = 0; i < 10; ++i)
    {
        pVitacOut[i]= 0;
    }

    spal_VitacFullDecoding(&VitacDecCfg, (UINT32*) pVitacOut);

    // TODO : check if this shift is necessary, and if it can be done outside this function
    for (k = 3, i = 6; i < ((UINT16) VitacDecCfg.vitacDecParam.nbSymbol / 3) + 6 - 8; k++, i++)
    {
        BufferOut[k / 8] &= (~(1 << (k % 8)));
        BufferOut[k / 8] |= ((!!(pVitacOut[(i-6) / 8] & 1 << ((i-6) % 8))) << (k % 8));
    }

    spal_CapDecodeCrc(g_sppCsCrcParam[coding_type], (UINT32*) pVitacOut, (UINT32*) &Res->bfi);


    // If CRC Correct
    if (!Res->bfi )
    {
        switch (HeadType)
        {
            case SPP_HEADER_TYPE1:        // for MCS7,8,9

            Res->bsn[0] = IR_GET_BSN1(BufferOut);
            deltaBSN = IR_GET_BSN2(BufferOut);
            Res->bsn[1] = IR_MOD_SNS(Res->bsn[0] + deltaBSN);
            Res->spb = 0;
            CPS = ((BufferOut[4] & 0xf8) >> 3) & 0x1f;

            if( CPS < SPP_EGPRS_SIZE_HEAD_DECODE_MCS789)
            {
               Res->mcs    = g_sppHeaderDecodeMcs789[CPS][0];
               Res->cps[0] = g_sppHeaderDecodeMcs789[CPS][1];
               Res->cps[1] = g_sppHeaderDecodeMcs789[CPS][2];
            }

            break;

            case SPP_HEADER_TYPE2:        // for MCS5,6

            Res->bsn[0] = IR_GET_BSN1(BufferOut);
            Res->spb = 0;
            CPS = ((BufferOut[3] & 0x0e) >> 1) & 0x07;

            if( CPS < SPP_EGPRS_SIZE_HEAD_DECODE_MCS56)
            {
               Res->mcs    = g_sppHeaderDecodeMcs56[CPS][0];
               Res->cps[0] = g_sppHeaderDecodeMcs56[CPS][1];
            }
            break;

            case SPP_HEADER_TYPE3:          // for MCS1,2,3,4

            Res->bsn[0] = IR_GET_BSN1(BufferOut);
            CPS = ((BufferOut[3] & 0x1e) >> 1) & 0x0f;

            if ( CPS <SPP_EGPRS_SIZE_HEAD_DECODE_MCS1234 )
            {
                Res->mcs    = g_sppHeaderDecodeMcs1234[CPS][0];
                Res->cps[0] = g_sppHeaderDecodeMcs1234[CPS][1];
            }

            Res->spb = ((BufferOut[3] & 0x60) >> 5) & 0x3;
            break;

            default:
            break;
        }

    }
    SPP_PROFILE_FUNCTION_EXIT(spp_EgprsDecodeHeader);
}



//=============================================================================
//  spp_EgprsDecodeData
//-----------------------------------------------------------------------------
// data decoding with incremental redundancy
//
// @param data UINT8[1836]. INPUT. first part data
// @param data1 UINT8[1836]. INPUT. second part data
// @param Header SPP_HEADER_RESULT_T*  header information
// @param EBL_Num UINT8[3]. crc check result return
// @param IR SPP_EGPRS_IR_CTX_T* Incremental Redundancy related structure
// @param pDecc UINT8* OUTPUT. output decoded data
//
// @author zhangchangchun
// @date 2007/08/14
//=============================================================================
PUBLIC VOID spp_EgprsDecodeData(UINT8                       *BufferIn,
                                SPP_HEADER_RESULT_T             Header,
                                UINT8                        *BufferOut0,
                                UINT8                        *BufferOut1,
                                SPP_EGPRS_IR_CTX_T            *irCtx,
                                SPP_EGPRS_IR_BUF_T            *irBuf,
                                SPP_UNCACHED_BUFFERS_T        *UncachedBuffers,
                                SPP_N_BLOCK_RESULT_T        *Res)

{
    UINT8   nb_blocks=0;
    UINT8   i=0;
    UINT8   j=0;
    UINT8   mode = Header.mcs;
    UINT8   EBL_Num[2] = {0};

    SPP_CODING_SCHEME_T spal_coding=0;
    SPAL_VITAC_DEC_CFG_T VitacDecCfg;

    UINT8  *SurvivorData = (UINT8* )UncachedBuffers->EgprsDecodeVitac.SurvivorData;
    UINT8  *CombinedData = (UINT8* )UncachedBuffers->EgprsDecodeVitac.CombinedData;
    UINT32 *PunctTable   = (UINT32*)UncachedBuffers->EgprsDecodeVitac.PunctTable;
    UINT32 *DecodedData  = (UINT32* )UncachedBuffers->EgprsDecodeVitac.DecodedData;
    UINT32 *BufferOut[2] = {(UINT32 *)BufferOut0,(UINT32 *)BufferOut1};

    SPP_PROFILE_FUNCTION_ENTRY(spp_EgprsDecodeData);

    switch(mode)
    {
    case EGPRS_MCS1:
        spal_coding = CS_MCS1;
        nb_blocks = 1;
        break;
    case EGPRS_MCS2:
        spal_coding = CS_MCS2;
        nb_blocks = 1;
        break;
    case EGPRS_MCS3:
        spal_coding = CS_MCS3;
        nb_blocks = 1;
        break;
    case EGPRS_MCS4:
        spal_coding = CS_MCS4;
        nb_blocks = 1;
        break;
    case EGPRS_MCS5:
        spal_coding = CS_MCS5;
        nb_blocks = 1;
        break;
    case EGPRS_MCS6:
        spal_coding = CS_MCS6;
        nb_blocks = 1;
        break;
    case EGPRS_MCS7:
        spal_coding = CS_MCS7;
        nb_blocks = 2;
        break;
    case EGPRS_MCS8:
        spal_coding = CS_MCS8;
        nb_blocks = 2;
        break;
    case EGPRS_MCS9:
        spal_coding = CS_MCS9;
        nb_blocks = 2;
        break;
    default:
        break;
    }


    VitacDecCfg.vitacDecParam =  g_sppVitacDecParam [spal_coding];
    VitacDecCfg.convPoly0     =  g_sppVitacPolyres[g_sppVitacDecParam[spal_coding].resPoly0];
    VitacDecCfg.convPoly1     =  g_sppVitacPolyres[g_sppVitacDecParam[spal_coding].resPoly1];
    VitacDecCfg.convPoly2     =  g_sppVitacPolyres[g_sppVitacDecParam[spal_coding].resPoly2];

    for (i = 0; i < nb_blocks; ++i)
    {
        if (irCtx->irOn)
        {
            spp_IrUpdateVr(Header.bsn[i], irCtx);

            if (RD2BITS8(irCtx->ReceiveState,Header.bsn[i]) != 3)
            {

                VitacDecCfg.inSoftBit =  (UINT32*) CombinedData;
                VitacDecCfg.punctTable = (UINT32*) PunctTable;

                spp_IrCombine(                (UINT8*)&BufferIn[MCS_7_TO_9_UNCOMPRESSED_BLOCK_LEN*i],
                                            i,
                                            &Header,
                                            irCtx,
                                            irBuf,
                                            SurvivorData,
                                            CombinedData,
                                            PunctTable);


                spal_VitacFullDecoding(&VitacDecCfg, DecodedData);

                spal_CapDecodeCrc (g_sppCsCrcParam[spal_coding], (UINT32*) DecodedData,     (UINT32*) &EBL_Num[i]);

                if (FAIL == EBL_Num[i])
                {
                    spp_IrWriteBuffer(        (UINT8*)&BufferIn[MCS_7_TO_9_UNCOMPRESSED_BLOCK_LEN*i],
                                            i,
                                            irCtx,
                                            irBuf,
                                            &Header);
                }
                else
                {
                    spp_IrClearBuffer(i, irCtx, &Header);
                    spp_IrUpdateVq(Header.bsn[i], irCtx);

                    // copy from work buffer to destination
                    for (j = 0; j < 20; ++j)
                    {
                        BufferOut[i][j] = DecodedData[j];
                    }

                }
            }
            else
            {
                // received block already decoded
                EBL_Num[i] = SUCCESS;
            }
        }
        else
        {


            VitacDecCfg.inSoftBit =  (UINT32*) &BufferIn[MCS_7_TO_9_UNCOMPRESSED_BLOCK_LEN*i];
            VitacDecCfg.punctTable = (UINT32*)(g_sppCsPuncturingTable [spal_coding] + 58 * (Header.cps[i]-1)) ;


             spal_VitacFullDecoding(&VitacDecCfg, DecodedData);

            spal_CapDecodeCrc (g_sppCsCrcParam[spal_coding], (UINT32*) DecodedData,(UINT32*) &EBL_Num[i]);


            // copy from work buffer to destination
            for (j = 0; j < 20; ++j)
            {
                BufferOut[i][j] = DecodedData[j];
            }

        }
    }

    Res->Bfi = EBL_Num[0] | (EBL_Num[1] << 1);

    SPP_PROFILE_FUNCTION_EXIT(spp_EgprsDecodeData);
}



//=============================================================================
//  spp_EgprsDecodeStealingBits
//-----------------------------------------------------------------------------
// get the header type according the stealing flag.
//
// @param RxBuffer UINT32*. INPUT.  demod data.
// @param headertype SPP_HEADER_TYPE_T*. OUTPUT. header type.
//
// @author jba
// @date 2008/05/29
//=============================================================================
PUBLIC VOID spp_EgprsDecodeStealingBits(UINT32* RxBuffer, SPP_HEADER_TYPE_T* headertype)
{
    int i, j;

    SPP_HEADER_TYPE_T header_type_value[2]={SPP_HEADER_TYPE1,SPP_HEADER_TYPE2};
    UINT8 q[2] = {0xe7, 0x00};

    UINT8* softBit;
    INT16 metric;
    INT16 best_metric = (INT16)0x8000;
    INT16 metric_max=0;
    SPP_HEADER_TYPE_T type=SPP_HEADER_UNKNOWN;

    softBit = (UINT8*)(RxBuffer);

    for (i = 0; i < 8; i++) metric_max+=(softBit[i]&0x7F);

    for (i = 0; i < 2; i++)
    {
        metric = 0;
                    for(j=0; j<8; j++)
        {
            if (((q[i] >> j) & 1) ^ ((softBit[j] >> 7) & 1))
                metric -= (softBit[j] & 0x7f);
            else
                metric += (softBit[j] & 0x7f);

        }
        if (metric > best_metric)
        {
            best_metric = metric;
            type = header_type_value[i];
        }
    }

    *headertype = type;

//    //rau = (double)best_metric/(double)metric_max;
//    //if (rau>0.5) && (metric_max>24) return detected type
//    if(((best_metric*2)>(metric_max))&&(metric_max>24))
//    {
//        *headertype = type;
//    }
//    else
//    {
//        *headertype = SPP_HEADER_UNKNOWN;
//    }
}


//=============================================================================
//  spp_EgprsBurstDemapping
//-----------------------------------------------------------------------------
// split RLC Block to three parts, they are the usf, head, and data.
//
// @param DecoderIn UINT8*. INPUT. input data.
// @param usf UINT8*. OUTPUT.  usf part after split.
// @param head UINT8*. OUTPUT. head part after split.
// @param data UINT8*. OUTPUT. data part after split.
// @param HeadType SPP_HEADER_TYPE_T. INPUT. type of the block header.
//
// @author zhangchangchun
// @date 2007/08/14
//=============================================================================
PUBLIC VOID spp_EgprsBurstDemapping(UINT32* BufferIn,
                                    UINT32* HeaderBufferOut,
                                    UINT32* DataBufferOut,
                                    SPP_HEADER_TYPE_T headerType)

{
    INT32 j, i;
    UINT8* DecoderIn = (UINT8*)BufferIn;
    UINT8* HeaderOut = (UINT8*)HeaderBufferOut;


    if(headerType == SPP_HEADER_TYPE1 || headerType == SPP_HEADER_TYPE2)
    {
        for (j = 0; j < 4; j++)
        {
            spp_MappingD12(&DecoderIn[348*j], (UINT8*)DataBufferOut, (UINT8*)HeaderBufferOut, j, headerType);
        }

    }
    else
    {
        for (j = 0; j < 4; j++)
        {
            for (i = 0; i < 57; i++)
            {
                // MOVEBIT(pIn, i, pDi, i+114*mB);
                HeaderOut[i + 114 * j] = DecoderIn[348*j + i];
            }

            for (i = 59; i < 116; i++)
            {
                // MOVEBIT(pIn, i, pDi, i-2+114*mB);
                HeaderOut[i - 2 + 114 * j] = DecoderIn[348*j + i];
            }
        }
    }
}




//======================================================================
// spp_DeinterleaveBlock
//----------------------------------------------------------------------
/// DeInterleaving of a data block
///
/// @param BufferIn UINT32*. INPUT. Pointer to equalized bursts buffer.
/// @param InterleaverType UINT8. INPUT. InterleaverType may take the following values:
/// - 6 : TYPE_H1. Valid for EGPRS HEADER TYPE2 in Rx.
/// - 7 : TYPE_H2. Valid for EGPRS HEADER TYPE2 in Tx.
/// - 8 : TYPE_H3. Valid for EGPRS HEADER TYPE1 in Rx.
/// - 9 : TYPE_H4. Valid for EGPRS HEADER TYPE1 in Tx.
/// - 10 : TYPE_D1. Valid for EGPRS MCS5/6.
/// - 11 : TYPE_D2. Valid for EGPRS MCS7.
/// - 12 : TYPE_D3. Valid for EGPRS MCS8/9.
/// @param BufferOut UINT32*. OUTPUT. Output pointer.
//======================================================================
PUBLIC VOID spp_EgprsDeinterleaveBlock(UINT32* BufferIn,
                                       SPP_ITLV_TYPE_T InterleaverType,
                                       UINT32* BufferOut)
{

    UINT8 OutDeItlv [456] = {0};
    UINT8 InDeItlv [512] = {0};
    UINT32 j,k = 0;
    UINT8 *pB = (UINT8 *)BufferOut;

    if (InterleaverType == ITLV_TYPE_1B)
    {
        for (k=0; k<4; k++)
        {
            for (j=0; j<114; j++)
            {
                InDeItlv [j + k*128] =  *((UINT8*) BufferIn + j + k*114);
            }

            for (j=114; j<128; j++)
            {
                InDeItlv [j + k*128] = 0;
            }

        }

        spal_ItlvDitlv((UINT8)InterleaverType, 0, 0, (UINT32*)InDeItlv , (UINT32*) OutDeItlv);

        k = 12;

        for (j = 0; j <=67; j++)
        {
            if (25 == k)
            {
                k++;
            }


            *pB++ = OutDeItlv[k];
            k++;
        }

        for (j = 0; j <= 371; j++)
        {
            if (82 == k || 139 == k || 424 == k)
            {
               k++;
            }
            *pB++ = OutDeItlv[k];
             k++;
        }

    }


    else
    {
        spal_ItlvDitlv((UINT8)InterleaverType, 0, 0, BufferIn , BufferOut);
    }

}

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
                                       INT16* Fof)
{
    UINT32 divider = 0;
    INT32 Icor= 0;

    UINT32 HardRefBurst [15] = {0};
    UINT32 RecRefBurst [148] = {0};

    // Reconstruction of the Burst from the soft values.
    spp_EgprsBuildRefNBurst (InSoftBits, Tsc,  HardRefBurst);

    // Reconstruct reference symbols of the burst.
    spal_XcorSrecPSK8( (INT16*)ChTaps, // channel taps
                                 148, // number of symbols
                        HardRefBurst, // hardbit reference
                         RecRefBurst,// output
                      SPP_LSE_TAP_NB, // channel tap number
                                  13);

    Icor  -= spal_XcorIcorCalc(&(InSamples[FOF_START_1+6]),
                               &(RecRefBurst[FOF_START_1]),
                               FOF_NB_SYMB);
    Icor  += spal_XcorIcorCalc(&(InSamples[FOF_START_2+6]),
                               &(RecRefBurst[FOF_START_2]),
                               FOF_NB_SYMB);
    // FOf is given by:
    //
    //                           CorrI x Fs
    // FOF = _______________________________________________________
    //
    //       corr_gap x 2 PI x |Power-NoisePower| x nb_symb_corr

    // Fs/(2*PI)=43104
    divider=(UINT32)(((Power-NoisePwr)*FOF_CORR_GAP)/43104)*FOF_NB_SYMB;

    if(divider)
    {
        *Fof =(INT16)(Icor/(INT32)divider);
    }
    else
    {
         *Fof = 32767;
    }
}

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
PROTECTED VOID spp_EgprsBuildRefNBurst(UINT8* Insoftvalues, UINT8 tsc,UINT32* OutRefBurst)
{
    UINT32 hard_refBurst_tab[11] = {0};
    UINT32* hard_refBurst;
    UINT16 i;
    UINT32 tmp=0;

   hard_refBurst = MEM_ACCESS_UNCACHED(hard_refBurst_tab);

    // First extract hard values from equalized output
    // 348 SoftValues => 87 words
    spal_XcorHvextract( Insoftvalues,
                        (UINT8) 87,
                        hard_refBurst);

    // add the 3 needed tail bits ...
    OutRefBurst[0] = (hard_refBurst[0] << 9)|0x1FF;
    // ...to the first 55 bits of payload...
    OutRefBurst[1] = (hard_refBurst[1] << 9)|(hard_refBurst[0] >> 23);
    // ...to the first 87 bits of payload...
    OutRefBurst[2] = (hard_refBurst[2] << 9)|(hard_refBurst[1] >> 23);
    // ...to the first 119 bits of payload...
    OutRefBurst[3] = (hard_refBurst[3] << 9)|(hard_refBurst[2] >> 23);
    // ...to the first 151 bits of payload...
    OutRefBurst[4] = (hard_refBurst[4] << 9)|(hard_refBurst[3] >> 23);
    // ...to the first 174 bits of payload...
    OutRefBurst[5] = (hard_refBurst[5] << 9)|(hard_refBurst[4] >> 23);
    // ...mask...
    OutRefBurst[5] &= 0xFFFFFFFFU >> 9;//(32-(174+9-32*5));
    // ...add the the TS
    for (i=0;i<3;i++)
    {
        tmp |= ((((g_sppGsmTscNburst[tsc])>>i)&0x1)?0x4:0x7)<<(3*i);
    }
    OutRefBurst[5] |= tmp<<23; //(174+9-32*5)
    tmp=0;
    for (i=0;i<10;i++)
    {
        tmp |= ((((g_sppGsmTscNburst[tsc])>>(i+3))&0x1)?0x4:0x7)<<(3*i);
    }
    tmp |= (((((g_sppGsmTscNburst[tsc])>>(13))&0x1)?0x4:0x7)&0x3)<<30;
    OutRefBurst[6] = tmp;
    tmp = (((((g_sppGsmTscNburst[tsc])>>(13))&0x1)?0x4:0x7)&0x4)>>2;
    for (i=0;i<10;i++)
        {
        tmp |= ((((g_sppGsmTscNburst[tsc])>>(i+14))&0x1)?0x4:0x7)<<(3*i+1);
            }
    tmp |= (((((g_sppGsmTscNburst[tsc])>>(24))&0x1)?0x4:0x7)&0x1)<<31;
    OutRefBurst[7] = tmp;
    tmp = (((((g_sppGsmTscNburst[tsc])>>(24))&0x1)?0x4:0x7)&0x6)>>1;
    // last TS symbol
    tmp |= ((((g_sppGsmTscNburst[tsc])>>(25))&0x1)?0x4:0x7)<<(2);
    OutRefBurst[8] = tmp;
    // ...to the first 201 bits of paylod...
    OutRefBurst[8] |= (hard_refBurst[6] << 23)|((hard_refBurst[5] >> 14))<<5;
    // ...to the first 233 bits of paylod...
    OutRefBurst[9] = (hard_refBurst[7] << 23)|(hard_refBurst[6] >> 9);
    // ...to the first 265 bits of paylod...
    OutRefBurst[10] = (hard_refBurst[8] << 23)|(hard_refBurst[7] >> 9);
    // ...to the first 297 bits of paylod...
    OutRefBurst[11] = (hard_refBurst[9] << 23)|(hard_refBurst[8] >> 9);
    // ...to the first 329 bits of paylod...
    OutRefBurst[12] = (hard_refBurst[10] << 23)|(hard_refBurst[9] >> 9);
    // ...to the first 348 bits of paylod...
    OutRefBurst[13] = (hard_refBurst[10] >> 9);
    // add the 3 needed tail bits ...
    OutRefBurst[13] |= (0x1FF << 19);

   }

#endif // (CHIP_EDGE_SUPPORTED)
























