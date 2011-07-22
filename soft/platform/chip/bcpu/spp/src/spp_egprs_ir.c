//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spp_egprs_ir.c
///
/// Functions for Incremental Redundancy.
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

#include "sppp_egprs_private.h"
#include "sppp_gsm_private.h"
#include "spal_dmasc.h"

#define NO_USE_HASH_TABLE  0

//------------------------------------------------------------------------------
// functions
//------------------------------------------------------------------------------

//=============================================================================
//  spp_EgprsIrInitCtx
//-----------------------------------------------------------------------------
/// for IR initial.
///
/// @param IR SPP_EGPRS_IR_CTX_T*. Incremental Redundancy related structure.
///
/// @author zhangchangchun
/// @date 2007/09/18
//=============================================================================
PUBLIC VOID spp_EgprsIrInitCtx(SPP_EGPRS_IR_CTX_T *IR, GSM_CS_T ChSubMode)
{
    UINT16 i;

    IR->Vr = 0;
    IR->Vq = 0;
    IR->OverFlow = 0;
    IR->pw = 0;
    IR->MaxBufferLength = 0;
    IR->CptSearch = 0;


    switch(ChSubMode)
    {
    case EGPRS_MCS1 :
    case EGPRS_MCS2 :
    case EGPRS_MCS3 :
    case EGPRS_MCS4 :
        IR->BufLeft = MCS_1_TO_4_BUFFER_LEN;
        break;
    case EGPRS_MCS5 :
    case EGPRS_MCS6 :
        IR->BufLeft = MCS_5_TO_6_BUFFER_LEN;
        break;
    case EGPRS_MCS7 :
    case EGPRS_MCS8 :
    case EGPRS_MCS9 :
    default:
        IR->BufLeft = MCS_7_TO_9_BUFFER_LEN;
        break;
    }


    for(i = 0; i < (MCS_MAX_BUFFER_LEN+31)/32; i++)
    {
        IR->receiveBufState[i] = 0;

    }


    for(i = 0; i < (SPP_EGPRS_IR_SNS+3)/4; i++)
    {
        IR->ReceiveState[i] = 0;
    }


    for(i = 0; i < (MCS_MAX_BUFFER_LEN+3)/4; i++)
    {
        IR->cps[i] = 0;
    }

    for(i = 0; i < HASH_LEN; i++)
    {
        IR->hashEntry[i] = 0x1FF;
    }

    for(i = 0; i < MCS_MAX_BUFFER_LEN; i++)
    {
        // set header to unknown mode
        IR->header[i].mcs     = 0xf;
        IR->header[i].mod     = PSK8;

        IR->tabHash[i].next   = 0x1FF;
        IR->tabHash[i].spb    = 0;
    }

}


//=============================================================================
//  spp_IrUpdateVr
//-----------------------------------------------------------------------------
// update the BSN which has a value one higher than the highest BSN yet received.
//
// @param NewBSN INT16. INPUT. the new coming BSN.
// @param IR SPP_EGPRS_IR_CTX_T*. Incremental Redundancy related structure.
//
// @author zhangchangchun
// @date 2007/08/14
//=============================================================================
PROTECTED VOID spp_IrUpdateVr(INT16 NewBSN, SPP_EGPRS_IR_CTX_T *IR)
{
    INT16 DisVrVq, DisVq;
    UINT16 BsnIdx;
    DisVrVq = ROUND_DISTANCE(IR_MOD_SNS(IR->Vr), IR_MOD_SNS(IR->Vq), SPP_EGPRS_IR_SNS);
    DisVq = ROUND_DISTANCE(NewBSN, IR_MOD_SNS(IR->Vq), SPP_EGPRS_IR_SNS);
    if (DisVq < EDGE_WS && DisVq >= DisVrVq)
    {
        BsnIdx = IR_MOD_SNS(IR->Vr);
        while (ROUND_DISTANCE(NewBSN, BsnIdx, SPP_EGPRS_IR_SNS) != 0)
        {
            CLR2BITS8(IR->ReceiveState,BsnIdx);
            BsnIdx = IR_MOD_SNS(BsnIdx + 1);
        }
        CLR2BITS8(IR->ReceiveState,NewBSN);
        IR->Vr = IR_MOD_SNS(NewBSN + 1);
        CLR2BITS8(IR->ReceiveState,IR->Vr);
    }
}


//=============================================================================
//  spp_IrUpdateVq
//-----------------------------------------------------------------------------
// update the BSN not yet received.
//
// @param NewBSN INT16. INPUT. the new coming BSN.
// @param IR SPP_EGPRS_IR_CTX_T*. Incremental Redundancy related structure.
//
// @author zhangchangchun
// @date 2007/08/14
//=============================================================================
PROTECTED VOID spp_IrUpdateVq(INT16 NewBSN, SPP_EGPRS_IR_CTX_T *IR)
{
    if (NewBSN == IR_MOD_SNS(IR->Vq))
    {
        while (ROUND_DISTANCE(IR->Vr, IR->Vq, SPP_EGPRS_IR_SNS) != 0)
        {
            if (RD2BITS8(IR->ReceiveState,IR->Vq) == 3)
            {
                IR->Vq = IR_MOD_SNS(IR->Vq + 1);
            }
            else
            {
                break;
            }
        }
    }
}


#if  (NO_USE_HASH_TABLE == 1)
//=============================================================================
//  spp_IrCombine
//-----------------------------------------------------------------------------
// Combine all the data in the buffer in the same family.
//
// @param pIn                 UINT8*.                IN  input data to be combined
// @param sel                 UINT8.                IN  input combine selection
// @param Header              SPP_HEADER_RESULT_T*    IN  header information
// @param IR                  SPP_EGPRS_IR_CTX_T*    IN  IR information.
// @param pTemp               UINT8*.                IN  temporary buffer used for recombination
// @param pOut                UINT8*.                OUT combined output
// @param dataPuncturingTable UINT32*                OUT puncturing scheme of the output buffer
//
// @author zhangchangchun
// @date 2007/08/14
//=============================================================================
PROTECTED VOID spp_IrCombine(    UINT8                *pIn,
                                UINT8                 sel,
                                SPP_HEADER_RESULT_T *Header,
                                SPP_EGPRS_IR_CTX_T    *IR,
                                SPP_EGPRS_IR_BUF_T    *irData,
                                UINT8                *pTemp,
                                UINT8                *pOut,
                                UINT32                *dataPuncturingTable)
{
    SPP_HEADER_CTX_T irHeader;

    INT32 i, j, ktd = 0;

    UINT8  * irBuf[2][3] = {{0},{0}}; // input buffers
    UINT8  * irOut[3]    = {0}; // output buffer
    UINT32 * irPps[3]    = {0}; // input PS table

    irHeader.bsn = Header->bsn[sel];

    if (Header->mcs < EGPRS_MCS5)
    {
        irHeader.mod = GMSK;
        irHeader.mcs = (Header->mcs - EGPRS_MCS1) & (Header->spb << 2);
    }
    else
    {
        irHeader.mod = PSK8;
        irHeader.mcs = (Header->mcs - EGPRS_MCS5);
    }

    // Search for the available blocks
    // TODO : optimize search by smart indexing
    // TODO : check the max number of saved blocks

    ktd = 0;

    switch(Header->mcs)
    {
    case EGPRS_MCS9:
    case EGPRS_MCS8:
    case EGPRS_MCS7:
        for (i = 0; i < MCS_7_TO_9_BUFFER_LEN; i++)
        {
            if (IR->header[i].ID == irHeader.ID)
            {
                irBuf[1][ktd] = irData->mcs7To9[i];
                irPps[ktd++] = (UINT32 *)g_sppCsPuncturingTable[Header->mcs - EGPRS_MCS1+CS_MCS1]+ 58*(RD2BITS8(IR->cps,i) - 1);
            }
        }
        break;

    case EGPRS_MCS6:
    case EGPRS_MCS5:
        for (i = 0; i < MCS_5_TO_6_BUFFER_LEN; i++)
        {
            if (IR->header[i].ID == irHeader.ID)
            {
                irBuf[1][ktd] = irData->mcs5To6[i];
                irPps[ktd++] = (UINT32 *)g_sppCsPuncturingTable[Header->mcs - EGPRS_MCS1+CS_MCS1]+ 58*(RD2BITS8(IR->cps,i) - 1);
            }
        }
        break;

    case EGPRS_MCS4:
    case EGPRS_MCS3:
    case EGPRS_MCS2:
    case EGPRS_MCS1:
    default:
        for (i = 0; i < MCS_1_TO_4_BUFFER_LEN; i++)
        {
            if (IR->header[i].ID == irHeader.ID)
            {
                irBuf[1][ktd] = irData->mcs1To4[i];
                irPps[ktd++] = (UINT32 *)g_sppCsPuncturingTable[Header->mcs - EGPRS_MCS1+CS_MCS1]+ 58*(RD2BITS8(IR->cps,i) - 1);
            }
        }
        break;
    }

    // End search

    for (j = 0; j < 58; j++)
    {
        dataPuncturingTable[j] = *((UINT32 *)g_sppCsPuncturingTable[Header->mcs - EGPRS_MCS1+CS_MCS1]+ 58*(Header->cps[sel] - 1) + j);
    }

    ktd = MAX(1,ktd);

    // organize recombination so that it always finishes on pOut
    switch(ktd)
    {
        case 1:
            irOut[0] = pOut;
            break;
        case 2:
            irOut[0] = pTemp;
            irOut[1] = pOut;
            break;
        case 3:
        default:
            irOut[0] = pOut;
            irOut[1] = pTemp;
            irOut[2] = pOut;
            break;
    }

    irBuf[0][0] = pIn;
    irBuf[0][1] = irOut[0];
    irBuf[0][2] = irOut[1];

    for (i = 0; i < ktd; i++)
    {
        // Recombine
        spal_DmascRecombine(    irBuf[0][i],
                                dataPuncturingTable,
                                irBuf[1][i],
                                irPps[i],
                                (UINT16)(g_sppEgprsDTailLen[(Header->mcs-EGPRS_MCS1)]*3),
                                irOut[i]);
    }
}



//=============================================================================
//  spp_IrWriteBuffer
//-----------------------------------------------------------------------------
// write data to the buffer while it isn`t crc correctly.
//
// @param data UINT8*. INPUT. data need saved
// @param sel UINT8. INPUT.  input selection
// @param IR SPP_EGPRS_IR_CTX_T*. the IR information.
// @param Header SPP_HEADER_RESULT_T*.  header information
//
// @author zhangchangchun
// @date 2007/08/14
//=============================================================================
PROTECTED VOID spp_IrWriteBuffer(UINT8                *data,
                                UINT8                 sel,
                                SPP_EGPRS_IR_CTX_T    *IR,
                                SPP_EGPRS_IR_BUF_T    *irData,
                                SPP_HEADER_RESULT_T    *Header)
{
  UINT16 modulo, len, i;
  UINT8 existOldBuf;



  SPP_HEADER_CTX_T irHeader;
  UINT8 *pData;

  UINT8 CPS = Header->cps[sel];



  // check if space is available
  if ((IR->BufLeft < 1))
  {
     return;
  }

  // extract the header
  irHeader.bsn = Header->bsn[sel];


  if (Header->mcs < EGPRS_MCS5)
  {
      irHeader.mod = GMSK;
      irHeader.mcs = ((Header->mcs - EGPRS_MCS1)) & (Header->spb << 2);
  }
  else
  {
      irHeader.mod = PSK8;
      irHeader.mcs = (Header->mcs - EGPRS_MCS5) ;
  }

  switch (Header->mcs)
  {
    case EGPRS_MCS9:
    case EGPRS_MCS8:
    case EGPRS_MCS7:
        modulo = MCS_7_TO_9_BUFFER_LEN;
        len    = MCS_7_TO_9_BLOCK_LEN;
        break;

    case EGPRS_MCS6:
    case EGPRS_MCS5:
        modulo = MCS_5_TO_6_BUFFER_LEN;
        len    = MCS_5_TO_6_BLOCK_LEN;
        break;

    case EGPRS_MCS4:
    case EGPRS_MCS3:
    case EGPRS_MCS2:
    case EGPRS_MCS1:
    default:
        modulo = MCS_1_TO_4_BUFFER_LEN;
        len    = MCS_1_TO_4_BLOCK_LEN;
        break;
  }


  // search for old buffer
  existOldBuf = 0;
  for(i = 0; ((i < modulo) && (!existOldBuf)); i++)
  {
      if (IR->header[i].ID == irHeader.ID)
      {
          if (RD2BITS8(IR->cps,i) == CPS)
          {
              existOldBuf = 1;
              IR->pw = i;
          }
      }
  }


  //  if old does not exist, search empty buffer
  if (!existOldBuf)
  {
      /*i = 0;

      // 32-bit accelerate search
      while (IR->receiveBufState[i] == (UINT32)0xffffffff)
      {
          i++;
      }

      IR->pw = i << 5;

      // 32-bit hi or lo
      if ((IR->receiveBufState[i] & (UINT32)0xffff) == (UINT32)0xffff)
      {
          IR->pw += 16;

          // 16-bit hi or lo
          if ((IR->receiveBufState[i] & (UINT32)0xff0000) == (UINT32)0xff0000)
          {
              IR->pw += 8;
          }

      }
      else
      {
          // 16-bit hi or lo
          if ((IR->receiveBufState[i] & (UINT32)0xff) == (UINT32)0xff)
          {
              IR->pw += 8;
          }
      }

      // bit search
      while (RDBIT32(IR->receiveBufState,IR->pw))
      {
          IR->pw++;
      }*/

      // bit search
     while (RDBIT32(IR->receiveBufState,IR->pw))
      {
          IR->pw = (IR->pw + 1) % modulo;
      }


  }


  // get the buffer address
  switch (Header->mcs)
  {
    case EGPRS_MCS9:
    case EGPRS_MCS8:
    case EGPRS_MCS7:
        pData  = (UINT8*)&irData->mcs7To9[IR->pw];
        break;

    case EGPRS_MCS6:
    case EGPRS_MCS5:
        pData  = (UINT8*)&irData->mcs5To6[IR->pw];
        break;

    case EGPRS_MCS4:
    case EGPRS_MCS3:
    case EGPRS_MCS2:
    case EGPRS_MCS1:
    default:
        pData  = (UINT8*)&irData->mcs1To4[IR->pw];
        break;
  }


  // compress & save the buffer
  spal_DmascSoftCompressSave(data,len,pData);


  // update the context
  if (!existOldBuf)
  {
    SETBIT32(IR->receiveBufState,IR->pw);
    SET2BITS8(IR->cps,IR->pw,CPS);
    IR->header[IR->pw] = irHeader;

    IR->BufLeft--;
    IR->pw = (IR->pw + 1) % modulo;
    IR->MaxBufferLength = MAX((modulo - IR->BufLeft), IR->MaxBufferLength);
    IR->OverFlow = (IR->BufLeft < OverFlow_Threshold) ? 1 : 0;
    //IR->CptWriter++;
  }


}

//=============================================================================
//  spp_IrClearBuffer
//-----------------------------------------------------------------------------
// clear data in the buffer while it is crc correctly.
//
// @param sel UINT8. INPUT.  input selection
// @param IR SPP_EGPRS_IR_CTX_T*. the IR information.
// @param Header SPP_HEADER_RESULT_T*.  header information
//
// @author zhangchangchun
// @date 2007/08/14
//=============================================================================
PROTECTED VOID spp_IrClearBuffer(UINT8                 sel,
                                SPP_EGPRS_IR_CTX_T    *IR,
                                SPP_HEADER_RESULT_T *Header)
{
  UINT16 i;
  UINT16 modulo;

  INT16 BSN = Header->bsn[sel];
  UINT8 SPB = Header->spb;

  // update the receive state
  switch (Header->mcs)
  {
    case EGPRS_MCS9:
    case EGPRS_MCS8:
    case EGPRS_MCS7:
      modulo = MCS_7_TO_9_BUFFER_LEN;
      SET2BITS8(IR->ReceiveState,BSN,3);
      break;

    case EGPRS_MCS6:
    case EGPRS_MCS5:
      modulo = MCS_5_TO_6_BUFFER_LEN;
      SET2BITS8(IR->ReceiveState,BSN,3);
      break;

    case EGPRS_MCS4:
    case EGPRS_MCS3:
    case EGPRS_MCS2:
    case EGPRS_MCS1:
    default:
      modulo = MCS_1_TO_4_BUFFER_LEN;
      SET2BITS8(IR->ReceiveState,BSN,(SPB - 1) & 0x3);
      break;
  }


  // clear the buffer(s)
  for (i = 0; i < modulo; i++)
  {
      if ((IR->header[i].bsn == (UINT32)BSN)
           &&  (IR->header[i].mcs != 0xf) )

      {
          CLRBIT32(IR->receiveBufState,i);
          CLR2BITS8(IR->cps,i);

          // set header to unknown mode
          IR->header[i].mcs = 0xf;
          IR->header[i].mod = PSK8;

          IR->BufLeft++;
          IR->pw = i;
      }
  }

  // recompute the overflow information
  IR->OverFlow = (IR->BufLeft < OverFlow_Threshold) ? 1 : 0;

}



#else

PROTECTED VOID spp_IrClearBuffer(UINT8                 sel,
                                SPP_EGPRS_IR_CTX_T    *IR,
                                SPP_HEADER_RESULT_T *Header)
{

  INT16 BSN = Header->bsn[sel];
  UINT8 SPB = Header->spb;
  UINT8 tmp_bsn;

  // update the receive state
  switch (Header->mcs)
  {
    case EGPRS_MCS9:
    case EGPRS_MCS8:
    case EGPRS_MCS7:
      SET2BITS8(IR->ReceiveState,BSN,3);
      break;

    case EGPRS_MCS6:
    case EGPRS_MCS5:
      SET2BITS8(IR->ReceiveState,BSN,3);
      break;

    case EGPRS_MCS4:
    case EGPRS_MCS3:
    case EGPRS_MCS2:
    case EGPRS_MCS1:
    default:
      SET2BITS8(IR->ReceiveState,BSN,(SPB - 1) & 0x3);
      break;
  }

  tmp_bsn = BSN%HASH_LEN;
  if (IR->hashEntry[tmp_bsn]!=0x1FF)
  {
      UINT16 tmp    = IR->hashEntry[tmp_bsn];

      while (  (tmp != 0x1FF) )
      {
          if ((((IR->tabHash[IR->hashEntry[tmp_bsn]].bsnresi<<6)|(UINT32)tmp_bsn) == (UINT32)Header->bsn[sel])
             && (IR->tabHash[IR->hashEntry[tmp_bsn]].spb == (UINT32)SPB))
          {
              CLRBIT32(IR->receiveBufState ,IR->hashEntry[tmp_bsn]);                   //Clear the corresponding bit in the tabHashState
              IR->BufLeft++;
              IR->pw = IR->hashEntry[tmp_bsn];

              CLR2BITS8(IR->cps,IR->hashEntry[tmp_bsn]);

              IR->hashEntry[tmp_bsn] = IR->tabHash[IR->hashEntry[tmp_bsn]].next;
              IR->tabHash[tmp].next  = 0x1FF;                                       //The block is deleted
              tmp = IR->hashEntry[tmp_bsn];
          }
          else
          {
              tmp = 0x1FF;
          }

          IR->CptSearch++;
      }

      tmp    = IR->hashEntry[tmp_bsn];
      UINT16 before = IR->hashEntry[tmp_bsn];

      while(tmp != 0x1FF)
      {
          UINT16 ant = IR->tabHash[tmp].next;
          if ( (((IR->tabHash[tmp].bsnresi<<6)|(UINT32)tmp_bsn) == (UINT32)Header->bsn[sel]) && (IR->tabHash[tmp].spb == (UINT32)SPB) )
          {
              IR->tabHash[before].next = IR->tabHash[tmp].next;
              IR->tabHash[tmp].next  = 0x1FF;                       //The block has beeen deleted

              CLRBIT32(IR->receiveBufState ,tmp);                   //Clear the corresponding bit in the tabHashState
              IR->BufLeft++;
              IR->pw = tmp;

              CLR2BITS8(IR->cps,tmp);

              if (IR->tabHash[tmp].next  == ant)
              {
                  before = tmp;
              }

              tmp    = IR->tabHash[before].next;
          }
          else
          {
              before = tmp;
              tmp = IR->tabHash[tmp].next;
          }

          IR->CptSearch++;
      }
  }


  // recompute the overflow information
  IR->OverFlow = (IR->BufLeft < OverFlow_Threshold) ? 1 : 0;

}




PROTECTED VOID spp_IrCombine(    UINT8                *pIn,
                                UINT8                 sel,
                                SPP_HEADER_RESULT_T *Header,
                                SPP_EGPRS_IR_CTX_T    *IR,
                                SPP_EGPRS_IR_BUF_T    *irData,
                                UINT8                *pTemp,
                                UINT8                *pOut,
                                UINT32                *dataPuncturingTable)
{
    UINT16 BSN = Header->bsn[sel];
    INT32 i, j, ktd = 0;
    UINT8 tmp_bsn;

    UINT8  * irBuf[2][3] = {{0},{0}}; // input buffers
    UINT8  * irOut[3]    = {0}; // output buffer
    UINT32 * irPps[3]    = {0}; // input PS table

    // Search for the available blocks
    ktd = 0;

    tmp_bsn= BSN%HASH_LEN;

    switch(Header->mcs)
    {
    case EGPRS_MCS9:
    case EGPRS_MCS8:
    case EGPRS_MCS7:
        if (IR->hashEntry[tmp_bsn]!=0x1FF)
        {
            UINT16 tmp = IR->hashEntry[tmp_bsn];
            while(tmp != 0x1FF)
            {
                if ( (((IR->tabHash[tmp].bsnresi<<6)|(UINT32)tmp_bsn)== (UINT32)BSN)  && (IR->tabHash[tmp].spb == (UINT32)Header->spb) )
                {
                    irBuf[1][ktd] = irData->mcs7To9[tmp];
                    irPps[ktd++] = (UINT32 *)g_sppCsPuncturingTable[Header->mcs - EGPRS_MCS1+CS_MCS1]+ 58*(RD2BITS8(IR->cps,tmp) - 1);
                }
                tmp = IR->tabHash[tmp].next;
            }
        }
        break;

    case EGPRS_MCS6:
    case EGPRS_MCS5:
        if (IR->hashEntry[tmp_bsn]!=0x1FF)
        {
            UINT16 tmp = IR->hashEntry[tmp_bsn];
            while(tmp != 0x1FF)
            {
                if ( (((IR->tabHash[tmp].bsnresi<<6)|(UINT32)tmp_bsn)== (UINT32)BSN)  && (IR->tabHash[tmp].spb == (UINT32)Header->spb) )
                {
                    irBuf[1][ktd] = irData->mcs5To6[tmp];
                    irPps[ktd++] = (UINT32 *)g_sppCsPuncturingTable[Header->mcs - EGPRS_MCS1+CS_MCS1]+ 58*(RD2BITS8(IR->cps,tmp) - 1);
                }
                tmp = IR->tabHash[tmp].next;
            }
        }
        break;

    case EGPRS_MCS4:
    case EGPRS_MCS3:
    case EGPRS_MCS2:
    case EGPRS_MCS1:
    default:
        if (IR->hashEntry[tmp_bsn]!=0x1FF)
        {
            UINT16 tmp = IR->hashEntry[tmp_bsn];
            while(tmp != 0x1FF)
            {
                if ( (((IR->tabHash[tmp].bsnresi<<6)|(UINT32)tmp_bsn)== (UINT32)BSN)  && (IR->tabHash[tmp].spb == (UINT32)Header->spb) )
                {
                    irBuf[1][ktd] = irData->mcs1To4[tmp];
                    irPps[ktd++] = (UINT32 *)g_sppCsPuncturingTable[Header->mcs - EGPRS_MCS1+CS_MCS1]+ 58*(RD2BITS8(IR->cps,tmp) - 1);
                }
                tmp = IR->tabHash[tmp].next;
            }
        }
        break;
    }

    // End search


    // End search

    for (j = 0; j < 58; j++)
    {
        dataPuncturingTable[j] = *((UINT32 *)g_sppCsPuncturingTable[Header->mcs - EGPRS_MCS1+CS_MCS1]+ 58*(Header->cps[sel] - 1) + j);
    }

    ktd = MAX(1,ktd);

    // organize recombination so that it always finishes on pOut
    switch(ktd)
    {
        case 1:
            irOut[0] = pOut;
            break;
        case 2:
            irOut[0] = pTemp;
            irOut[1] = pOut;
            break;
        case 3:
        default:
            irOut[0] = pOut;
            irOut[1] = pTemp;
            irOut[2] = pOut;
            break;
    }

    irBuf[0][0] = pIn;
    irBuf[0][1] = irOut[0];
    irBuf[0][2] = irOut[1];

    for (i = 0; i < ktd; i++)
    {
        // Recombine
        spal_DmascRecombine(    irBuf[0][i],
                                dataPuncturingTable,
                                irBuf[1][i],
                                irPps[i],
                                (UINT16)(g_sppEgprsDTailLen[(Header->mcs-EGPRS_MCS1)]*3),
                                irOut[i]);
    }
}



PROTECTED VOID spp_IrWriteBuffer(UINT8                *data,
                                UINT8                 sel,
                                SPP_EGPRS_IR_CTX_T    *IR,
                                SPP_EGPRS_IR_BUF_T    *irData,
                                SPP_HEADER_RESULT_T    *Header)
{
  UINT16 modulo, len;
  UINT8 existOldBuf;
  UINT8 tmp_bsn;

  UINT8 *pData;
  UINT16 BSN = Header->bsn[sel];
  UINT8 CPS = Header->cps[sel];



  // check if space is available
  if ((IR->BufLeft < 1))
  {
     return;
  }


  switch (Header->mcs)
  {
    case EGPRS_MCS9:
    case EGPRS_MCS8:
    case EGPRS_MCS7:
        modulo = MCS_7_TO_9_BUFFER_LEN;
        len    = MCS_7_TO_9_BLOCK_LEN;
        break;

    case EGPRS_MCS6:
    case EGPRS_MCS5:
        modulo = MCS_5_TO_6_BUFFER_LEN;
        len    = MCS_5_TO_6_BLOCK_LEN;
        break;

    case EGPRS_MCS4:
    case EGPRS_MCS3:
    case EGPRS_MCS2:
    case EGPRS_MCS1:
    default:
        modulo = MCS_1_TO_4_BUFFER_LEN;
        len    = MCS_1_TO_4_BLOCK_LEN;
        break;
  }


  // search for old buffer
  existOldBuf = 0;

  tmp_bsn= BSN%HASH_LEN;
  if (IR->hashEntry[tmp_bsn]!=0x1FF)
  {
      UINT16 tmp = IR->hashEntry[tmp_bsn];
      while((tmp != 0x1FF) && (!existOldBuf))
      {
          if ( (((IR->tabHash[tmp].bsnresi<<6)|(UINT32)tmp_bsn)== (UINT32)BSN)  && (IR->tabHash[tmp].spb == (UINT32)Header->spb) )
          {
              if (RD2BITS8(IR->cps,tmp) == CPS)
              {
                  existOldBuf = 1;
                  IR->pw      = tmp;
              }
          }
          tmp = IR->tabHash[tmp].next;
      }
  }




  //  if old does not exist, search empty buffer
  if (!existOldBuf)
  {
      // bit search
      while (RDBIT32(IR->receiveBufState,IR->pw))
      {
          IR->pw = (IR->pw + 1) % modulo;
      }
  }


  // get the buffer address
  switch (Header->mcs)
  {
    case EGPRS_MCS9:
    case EGPRS_MCS8:
    case EGPRS_MCS7:
        pData  = (UINT8*)&irData->mcs7To9[IR->pw];
        break;

    case EGPRS_MCS6:
    case EGPRS_MCS5:
        pData  = (UINT8*)&irData->mcs5To6[IR->pw];
        break;

    case EGPRS_MCS4:
    case EGPRS_MCS3:
    case EGPRS_MCS2:
    case EGPRS_MCS1:
    default:
        pData  = (UINT8*)&irData->mcs1To4[IR->pw];
        break;
  }


  // compress & save the buffer
  spal_DmascSoftCompressSave(data,len,pData);


  // update the context
  if (!existOldBuf)
  {
      SPP_EGPRS_IR_HASH ihirHeader;
      ihirHeader.bsnresi = (BSN>>6)&0x1F;
      ihirHeader.spb     = Header->spb;
      ihirHeader.next    = 0x1FF;

      SETBIT32(IR->receiveBufState,IR->pw);
      SET2BITS8(IR->cps,IR->pw,CPS);

      if ( IR->hashEntry[tmp_bsn] == 0x1FF )
      {
          IR->hashEntry[tmp_bsn] = IR->pw;
      }
      else
      {
          UINT16 before = IR->hashEntry[tmp_bsn];
          UINT16 tmp    = IR->hashEntry[tmp_bsn];
          while (tmp != 0x1FF)
          {
              before = tmp;
              tmp    = IR->tabHash[tmp].next;
          }
          IR->tabHash[before].next = IR->pw;
      }

      IR->tabHash[IR->pw]      = ihirHeader;

      IR->BufLeft--;
      IR->pw = (IR->pw + 1) % modulo ;
      IR->MaxBufferLength = MAX((modulo - IR->BufLeft), IR->MaxBufferLength);
      IR->OverFlow = (IR->BufLeft < OverFlow_Threshold) ? 1 : 0;
  }

}



#endif




#endif // (CHIP_EDGE_SUPPORTED)


