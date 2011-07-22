//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spp_egprs_tx.c
///
/// Channel Encoding and Decoding of information bits.
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

#include "sppp_gsm_private.h"
#include "sppp_egprs_private.h"
#include "spal_cap.h"
#include "spal_itlv.h"
#include "sppp_debug.h"


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------
PRIVATE VOID spp_EgprsItlv(UINT8* pD,
                           UINT8* pD1,
                           UINT8* pHd,
                           UINT8* pUsfm,
                           UINT8* ItLvDataOut,
                           UINT8* ItLvHeadOut,
                           UINT8  ChMod);


//=============================================================================
// spp_EgprsTxMapping
//-----------------------------------------------------------------------------
// mapping to a burst
//
// @param pDi UINT8*. INPUT. input data part
// @param pHi UINT8*. INPUT. input header part
// @param pU UINT8*. INPUT. input usf part
// @param pOut UINT8*. OUTPUT
// @param mB INT32. burst number
// @param chSubMode UNIT8. mcs coding mode
//
// @author
// @date
//=============================================================================
INLINE VOID spp_EgprsTxMapping(UINT8* pDi,
                                        UINT8* pHi,
                                        UINT8* pU,
                                        UINT8* pOut,
                                        INT32 mB,
                                        UINT8 chSubMode)
{
    INT32 i;
    UINT8 q14[2] = {0x68, 0x0};
    UINT8 q79[2] = {0xe7, 0x0};
    UINT8 q56[2] = {0x0, 0x0};
    UINT8 ta, tb;

    for (i = 0; i < 44; i++)
    {
        pOut[i] = 0;
    }

    switch (chSubMode)
    {
        case CS_MCS1:
        case CS_MCS2:
        case CS_MCS3:
        case CS_MCS4:
            for (i = 0; i < 57; i++)
            {
                MOVEBITESPP(pDi, i + mB * 128, pOut, i);
            }
            for (i = 57; i < 59; i++)
            {
                MOVEBITESPP(q14, i - 57 + 2 * mB, pOut, i);
            }
            for (i = 59; i < 116; i++)
            {
                MOVEBITESPP(pDi, i - 2 + mB * 128, pOut, i);
            }
            break;
         case CS_MCS5:
        case CS_MCS6:
            for (i = 0; i < 156; i++)
            {
                MOVEBITESPP(pDi, i + 312 * mB, pOut, i);  // m is B
            }
            for (i = 156; i < 174; i++)
            {
                MOVEBITESPP(pHi, i + 34 * mB - 156, pOut, i);
            }
            for (i = 174; i < 176; i++)
            {
                MOVEBITESPP(q56, i + 2 * mB - 174, pOut, i);
            }
            for (i = 176; i < 192; i++)
            {
                MOVEBITESPP(pHi, i + 34 * mB - 158, pOut, i);
            }
            for (i = 192; i < 348; i++)
            {
                MOVEBITESPP(pDi, i + 312 * mB - 36, pOut, i);
            }

            SWAP_EN(pOut, 142, 155, ta, tb);
            SWAP_EN(pOut, 144, 158, ta, tb);
            SWAP_EN(pOut, 145, 161, ta, tb);
            SWAP_EN(pOut, 147, 164, ta, tb);
            SWAP_EN(pOut, 148, 167, ta, tb);
            SWAP_EN(pOut, 150, 170, ta, tb);
            SWAP_EN(pOut, 151, 173, ta, tb);
            SWAP_EN(pOut, 176, 195, ta, tb);
            SWAP_EN(pOut, 179, 196, ta, tb);
            SWAP_EN(pOut, 182, 198, ta, tb);
            SWAP_EN(pOut, 185, 199, ta, tb);
            SWAP_EN(pOut, 188, 201, ta, tb);
            SWAP_EN(pOut, 191, 202, ta, tb);
            SWAP_EN(pOut, 194, 204, ta, tb);
            break;
        case CS_MCS7:
        case CS_MCS8:
        case CS_MCS9:
            for (i = 0; i < 153; i++)
            {
                MOVEBITESPP(pDi, i + 306 * mB, pOut, i);  // m is B
            }
            for (i = 153; i < 174; i++)
            {
                MOVEBITESPP(pHi, i + 40 * mB - 153, pOut, i);
            }
            for (i = 174; i < 176; i++)
            {
                MOVEBITESPP(q79, i + 2 * mB - 174, pOut, i);
            }
            for (i = 176; i < 195; i++)
            {
                MOVEBITESPP(pHi, i + 40 * mB - 155, pOut, i);
            }
            for (i = 195; i < 348; i++)
            {
                MOVEBITESPP(pDi, i + 306 * mB - 42, pOut, i);
            }


            SWAP_EN(pOut, 142, 155, ta, tb);
            SWAP_EN(pOut, 144, 158, ta, tb);
            SWAP_EN(pOut, 145, 161, ta, tb);
            SWAP_EN(pOut, 147, 164, ta, tb);
            SWAP_EN(pOut, 148, 167, ta, tb);
            SWAP_EN(pOut, 150, 170, ta, tb);
            SWAP_EN(pOut, 151, 173, ta, tb);
            SWAP_EN(pOut, 176, 195, ta, tb);
            SWAP_EN(pOut, 179, 196, ta, tb);
            SWAP_EN(pOut, 182, 198, ta, tb);
            SWAP_EN(pOut, 185, 199, ta, tb);
            SWAP_EN(pOut, 188, 201, ta, tb);
            SWAP_EN(pOut, 191, 202, ta, tb);
            SWAP_EN(pOut, 194, 204, ta, tb);
            break;

        default:
            break;
    }
}


//======================================================================
// spp_EgprsEncodeBlock
//----------------------------------------------------------------------
/// Normal BLOCK encoding
///
/// @param InTxBuffer UINT32*. Pointer to the data to be encoded.
/// @param MCS UINT8. coding scheme
/// @param BufferOut UINT32*. Encoded data output pointer. The function
/// stores the output (the interleaved and encoded bits) in the Tx interleaver buffer.
/// header_type SPP_HEADER_TYPE_T. type of the header.
/// BufferOut should point to the beginning of the Tx interleaver buffer.
//======================================================================
PUBLIC VOID spp_EgprsEncodeBlock(UINT32* InData0,
                                 UINT32* InData1,
                                 UINT32* InHeader,
                                 SPP_HEADER_TYPE_T headerType,
                                 UINT32* BufferOut)
{
    INT32 i, k;
    UINT8 mode, block;
    UINT8 usfm[5] = {0};
    UINT8 tailm[77] = {0};

    UINT8 data[2][230] = {{0},{0}};
    UINT8 head[23] = {0};

    UINT8 ItLvDataOut[230] = {0};
    UINT8 ItLvHeadOut[23] = {0};

    UINT32 crcm = 0;
    UINT8  cps[2], cpsIdx;

    //UINT8 * BuffOut = (UINT8*)BufferOut;
    UINT8 * HeaderIn = (UINT8*)InHeader;
    UINT8 * InData[2] = {(UINT8*)InData0,(UINT8*)InData1};

    SPP_CODING_SCHEME_T spalHeaderType=0; // initialized just to avoid warnings
    SPP_CODING_SCHEME_T spalCodingType=0; // initialized just to avoid warnings

    SPP_PROFILE_FUNCTION_ENTRY(spp_EgprsEncodeBlock);

    switch (headerType)
    {
        case SPP_HEADER_TYPE1 :

            cpsIdx = (HeaderIn[4] >> 3) & 0x1f;

            spalHeaderType = CS_HEAD_1;
            spalCodingType = g_sppHeaderDecodeMcs789[cpsIdx][0];
            cps[0]         = g_sppHeaderDecodeMcs789[cpsIdx][1];
            cps[1]         = g_sppHeaderDecodeMcs789[cpsIdx][2];

            break;
        case SPP_HEADER_TYPE2 :

            cpsIdx  = ((HeaderIn[3] ) >> 1) & 0x07;

            spalHeaderType = CS_HEAD_2;
            spalCodingType = g_sppHeaderDecodeMcs56[cpsIdx][0];
            cps[0]         = g_sppHeaderDecodeMcs56[cpsIdx][1];

            break;
        case SPP_HEADER_TYPE3 :

            cpsIdx = (HeaderIn[3] >> 1 )  & 0x0f;

            spalHeaderType = CS_HEAD_3;
            spalCodingType = g_sppHeaderDecodeMcs1234[cpsIdx][0];
            cps[0]         = g_sppHeaderDecodeMcs1234[cpsIdx][1];

        default :
            break;
    }

    mode = (spalCodingType - CS_MCS1) ;

    spal_CapEncodeCrc(g_sppCsCrcParam[spalHeaderType], InHeader, (UINT32*) &crcm);


    // prepare for header encoding with tail biting
    // --------------------------------------------

    tailm[0] = (crcm >> 2) & 0x3f;

    for (k = 0, i = 6; k < g_sppEgprsDInSPos[mode]-g_sppEgprsHInSPos[0]; k++, i++)
    {
        tailm[i / 8] |= ((!!(HeaderIn[k / 8] & 1 << (k % 8))) << (i % 8));
    }

    for (k = 0; i < g_sppEgprsHTailLen[mode]; k++, i++)
    {
        tailm[i / 8] |= ((!!(crcm & ( 1 << (k)))) <<(i % 8));
    }


    spal_CapConvoluate(g_sppCsConvParam[spalHeaderType], NULL, (UINT32*)tailm, (UINT32*) head);


    for (block = 0; block < ((spalCodingType < CS_MCS7) ? 1 : 2); block++)
    {
        // TODO : remove buffer clear
        crcm = 0x0000;
        BUFFER_CLEAR(tailm, 616);

        *(UINT8 *)data[block] = cps[block];

        spal_CapEncodeCrc(g_sppCsCrcParam[spalCodingType], (UINT32*) InData[block], (UINT32*) &crcm);

        // prepare for data encoding
        // -------------------------

        for (i = 0, k = 0; i < (g_sppEgprsDInLng[mode] - g_sppEgprsDInSPos[mode])/8; i++, k+=8)
        {
            // fast copy
            tailm[i] = InData[block][i];
        }

        for (; k <= g_sppEgprsDInLng[mode] - g_sppEgprsDInSPos[mode]; k++)
        {
            tailm[k / 8] |= ((!!(InData[block][k / 8] & 1 << (k % 8))) << (k % 8));
        }

        for (i = 0; i < 12; k++, i++)
        {
            tailm[k / 8] |=  ((!!(crcm & ( 1 << (i)))) << (k % 8));
        }

        for (; k < g_sppEgprsDTailLen[mode]; k++)
        {
            tailm[k / 8] &= ~ (1 << (k % 8));
        }

        spal_CapConvoluate(g_sppCsConvParam[spalCodingType], NULL, (UINT32*) tailm, (UINT32*)data[block]);
    }


    spp_EgprsItlv(data[0], data[1], head, usfm, ItLvDataOut, ItLvHeadOut, (UINT8) spalCodingType);

    // mapping
    for (i = 0; i < 4; i++)   // i is m
    {
        // pTxBufs->DataBuf need to consider, two dimension array
        spp_EgprsTxMapping(ItLvDataOut,
                           ItLvHeadOut,
                           usfm,
                           ((UINT8*)BufferOut) + SPP_EGPRS_TX_BURST_LEN*i,
                           i,
                           (UINT8) spalCodingType);
    }
    SPP_PROFILE_FUNCTION_EXIT(spp_EgprsEncodeBlock);
}


//======================================================================
// spp_EgprsBuildNBurst
//----------------------------------------------------------------------
/// Normal Burst building and mapping
///
/// @param TxBuffer UINT32*. Pointer to the encoded data.
/// @param Tsc UINT8. Training sequence number.
/// header_type SPP_HEADER_TYPE_T. type of the header.
/// @param BufferOut UINT32*. Burst output pointer.
//======================================================================
PUBLIC VOID spp_EgprsBuildNBurst(UINT32* TxBuffer,
                                 UINT8 Tsc,
                                 SPP_HEADER_TYPE_T headerType,
                                 UINT32* BufferOut)
{

    UINT16    i;
    UINT32 tmp=0;

    SPP_PROFILE_FUNCTION_ENTRY(spp_EgprsBuildNBurst);

    if (headerType == SPP_HEADER_TYPE3) // MCS1, MCS2, MCS3, MCS4
    {
        // 4 guard bits, 3 tail bits, Encrypted bits(0-24 of word0)
        BufferOut[0]  = ((TxBuffer[0] & 0x01ffffff) << 7) | 0xf ;

        // Encrypted bits(25-31 of word0), Encrypted bits(0-24 of word1)
        BufferOut[1]  = ((TxBuffer[1] & 0x01ffffff) << 7) | ((TxBuffer[0] & 0xfe000000) >> 25);

        // Encrypted bits(25-25 of word1), TsNb[0-25], Encrypted bits(26-30 of word1)
        BufferOut[2] = ((TxBuffer[1] & 0x78000000) << 1) |
                       ((g_sppGsmTscNburst[Tsc] & 0x03ffffff) << 1) | ((TxBuffer[1] & 0x4000000) >> 25);

        // Encrypted bits(31-31 of word1), Encrypted bits(0-30 of word2)
        BufferOut[3] = ((TxBuffer[2] & 0x7fffffff) << 1) | ((TxBuffer[1] & 0x80000000) >> 31) ;

        // Encrypted bits(31-31 of word2), Encrypted bits(0-19 of word3), 3 tail bits, 4 guard bits
        BufferOut[4] = (0xf<<24) | ((TxBuffer[3] & 0x000fffff) << 1) | ((TxBuffer[2] & 0x80000000) >> 31) ;
        //UINT8 tmp[157];
    }

    else    //MCS5, MCS6, MCS7, MCS8, MCS9
    {
        //UINT8 tmp[471];
        // add the 3 needed tail bits ...
        BufferOut[0] = ((TxBuffer[0] & 0x7fffff) << 9) | 0x1ff;
        // ...to the first 55 bits of payload...
        BufferOut[1] = ((TxBuffer[1] & 0x7fffff) << 9) | ((TxBuffer[0] & 0xff800000) >> 23);
        // ...to the first 87 bits of payload...
        BufferOut[2] = ((TxBuffer[2] & 0x7fffff) << 9) | ((TxBuffer[1] & 0xff800000) >> 23);
        // ...to the first 119 bits of payload...
        BufferOut[3] = ((TxBuffer[3] & 0x7fffff) << 9) | ((TxBuffer[2] & 0xff800000) >> 23);
        // ...to the first 151 bits of payload...
        BufferOut[4] = ((TxBuffer[4] & 0x7fffff) << 9) | ((TxBuffer[3] & 0xff800000) >> 23);
        // ...to the first 174 bits of payload...
        BufferOut[5] = ((TxBuffer[5] & 0x7fffff) << 9) | ((TxBuffer[4] & 0xff800000) >> 23);
        // ...mask...
        BufferOut[5] &= 0xffffffff >> 9;//(32-(174+9-32*5));
        // ...add the the TS
        for (i=0;i<3;i++)
        {
            tmp |= ((((g_sppGsmTscNburst[Tsc])>>i)&0x1)?0x4:0x7) << (3*i);
        }
        BufferOut[5] |= tmp<<23; //(174+9-32*5)
        tmp=0;
        for (i=0;i<10;i++)
        {
            tmp |= ((((g_sppGsmTscNburst[Tsc])>>(i+3))&0x1)?0x4:0x7) << (3*i);
        }
        tmp |= (((((g_sppGsmTscNburst[Tsc])>>(13))&0x1)?0x4:0x7)&0x3) << 30;
        BufferOut[6] = tmp;
        tmp = (((((g_sppGsmTscNburst[Tsc])>>(13))&0x1)?0x4:0x7)&0x4) >> 2;
        for (i=0;i<10;i++)
        {
            tmp |= ((((g_sppGsmTscNburst[Tsc])>>(i+14))&0x1)?0x4:0x7) << (3*i+1);
        }
        tmp |= (((((g_sppGsmTscNburst[Tsc])>>(24))&0x1)?0x4:0x7)&0x1) << 31;
        BufferOut[7] = tmp;
        tmp = (((((g_sppGsmTscNburst[Tsc])>>(24))&0x1)?0x4:0x7)&0x6) >> 1;
        // last TS symbol
        tmp |= ((((g_sppGsmTscNburst[Tsc])>>(25))&0x1)?0x4:0x7) << 2;
        BufferOut[8] = tmp;
        // ...to the first 201 bits of paylod...
        BufferOut[8] |= ((TxBuffer[6] & 0x1ff) << 23) | (((TxBuffer[5] & 0xffffc000) >> 14) << 5);
        // ...to the first 233 bits of paylod...
        BufferOut[9] = ((TxBuffer[7] & 0x1ff) << 23) | ((TxBuffer[6] & 0xfffffe00) >> 9);
        // ...to the first 265 bits of paylod...
        BufferOut[10] = ((TxBuffer[8] & 0x1ff) << 23) | ((TxBuffer[7] & 0xfffffe00) >> 9);
        // ...to the first 297 bits of paylod...
        BufferOut[11] = ((TxBuffer[9] & 0x1ff) << 23) | ((TxBuffer[8] & 0xfffffe00) >> 9);
        // ...to the first 329 bits of paylod...
        BufferOut[12] = ((TxBuffer[10] & 0x1ff) << 23) | ((TxBuffer[9] & 0xfffffe00) >> 9);
        // ...to the first 348 bits of paylod...
        BufferOut[13] = ((TxBuffer[10] & 0xfffffe00) >> 9);
        // add the 3 needed tail bits ...
        BufferOut[13] |= (0x1ff << 19);
        // add the 4 last guard bits
        BufferOut[13] |= 0xf0000000;
        BufferOut[14] = 0xff;
        // add the 4 first guard bits
        for(i=14;i>0;i--)
        {
            BufferOut[i]=(BufferOut[i])<<12 | ((BufferOut[i-1])>>20);
        }
        BufferOut[0] = (BufferOut[0])<<12 | 0xfff;

    }

    SPP_PROFILE_FUNCTION_EXIT(spp_EgprsBuildNBurst);
}



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
PRIVATE VOID spp_EgprsItlv(UINT8* pD,
                           UINT8* pD1,
                           UINT8* pHd,
                           UINT8* pUsfm,
                           UINT8* ItLvDataOut,
                           UINT8* ItLvHeadOut,
                           UINT8  ChMod)
{
    INT32 i, k;
    UINT8 datatmp[1248] = {0};
    UINT8 ItlvHeaderType=0; // initialized just to avoid warnings
    UINT8 ItlvDataType=0; // initialized just to avoid warnings
    UINT8* ItLvDataIn=NULL;


    switch (ChMod)
    {
    case CS_MCS1:
    case CS_MCS2:
    case CS_MCS3:
    case CS_MCS4:
        k = 0;
        for (i = 0; i <= 79; i++)
        {
            if (25 == k)
            {
                CLEARBITESPP((datatmp), k);
                k++;
            }
            CLEARBITESPP((datatmp), k);
            MOVEBITESPP(pHd, i, (datatmp), k);
            k++;
        }

        for (i = 0; i <= 371; i++)
        {
            if (82 == k || 139 == k || 424 == k)
            {
                CLEARBITESPP((datatmp), k);
                k++;
            }
            CLEARBITESPP((datatmp), k);
            MOVEBITESPP(pD, i, (datatmp), k);
            k++;
        }
        ItlvDataType =ITLV_TYPE_1B;
        ItLvDataIn = datatmp;
        break;
    case CS_MCS5:
    case CS_MCS6:
        ItlvDataType   = ITLV_TYPE_D1;
        ItlvHeaderType = ITLV_TYPE_H2;
        ItLvDataIn = pD;
        break;
    case CS_MCS7:
        // // Prepare data for interleaving
        k = 0;
        for (i = 0; i <= 611; i++)
        {
            CLEARBITESPP((datatmp), k);
            MOVEBITESPP(pD, i, (datatmp), k);
            k++;
        }

        for (i = 0; i <= 611; i++)
        {
            CLEARBITESPP((datatmp), k);
            MOVEBITESPP(pD1, i, (datatmp), k);
            k++;
        }
        ItlvDataType   = ITLV_TYPE_D2;
        ItlvHeaderType = ITLV_TYPE_H4;
        ItLvDataIn = datatmp;
        break;
    case CS_MCS8:
    case CS_MCS9:
        // // Prepare data for interleaving
        k = 0;
        for (i = 0; i <= 611; i++)
        {
            CLEARBITESPP((datatmp), k);
            MOVEBITESPP(pD, i, (datatmp), k);
            k++;
        }

        for (i = 0; i <= 611; i++)
        {
            CLEARBITESPP((datatmp), k);
            MOVEBITESPP(pD1, i, (datatmp), k);
            k++;
        }
        ItlvDataType   = ITLV_TYPE_D3;
        ItlvHeaderType = ITLV_TYPE_H4;
        ItLvDataIn = datatmp;
        break;
    }


    // Data Interleaving
    spal_ItlvItlv (ItlvDataType, 0, 0, (UINT32*)ItLvDataOut, (UINT32*)ItLvDataIn);

    if(ChMod >= CS_MCS5)
    {
       // Header Interleaving
       spal_ItlvItlv (ItlvHeaderType, 0, 0, (UINT32*) ItLvHeadOut, (UINT32*) pHd);
    }


}


#endif // (CHIP_EDGE_SUPPORTED)




















































































































































































































