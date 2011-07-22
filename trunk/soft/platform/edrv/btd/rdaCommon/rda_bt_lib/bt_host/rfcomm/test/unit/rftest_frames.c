/***********************************************************************
 *
 * MODULE NAME:    rfcomm_frames.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    RFCOMM Frame creation 
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  02 Feb 2000
 *
 * SOURCE CONTROL: $Id: rftest_frames.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    02.Feb.2000 -   DA - first import
 *
 *
 ***********************************************************************/

#include "bt.h"
#include "papi.h"

#include "rftest_types.h"
#include "rftest_pdu_gen.h"

#include "rftest_crc.h"

int prh_rftest_add_fcs(t_DataBuf *frame, int fcs_len);
int prh_rftest_dump_frame(t_DataBuf *frame, int tx);

int rftest_data_buf_reserve_header(t_DataBuf *buf, u_int32 size)
{
  buf->data+=size;
  if (buf->dataLen < size)
    buf->dataLen=0;
  else
    buf->dataLen-=size;
  return BT_NOERROR;
}

int rftest_data_buf_release_header(t_DataBuf *buf, u_int32 size)
{
  buf->data-=size;

  buf->dataLen+=size;
  return BT_NOERROR;
}


/*
 * prh_rftest_create_sabm
 * -- Create an SABM frame from the info passed in.
 *
 */
int prh_rftest_create_sabm(t_DataBuf *frame, prh_t_rfcomm_dlci DLCI, u_int8 pbit, u_int32 cresp)
{
    /* Put DLCI into address field, SABM is a command and EA is always set */
    frame->data[PRH_RFCOMM_FR_ADDR_FIELD]=((DLCI & 0x3F)<<2) | (cresp << 1) | PRH_RFCOMM_EA_BIT;
    
    frame->data[PRH_RFCOMM_FR_CTRL_FIELD] = PRH_RFCOMM_FR_CTRL_SABM;

    if (pbit)
    frame->data[PRH_RFCOMM_FR_CTRL_FIELD] |= PRH_RFCOMM_FR_CTRL_PF_BIT;
    
    /* Set EA bit to 1 in length field and length to 0 */
    frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] = PRH_RFCOMM_EA_BIT;

    frame->dataLen=3;
    /* Need to calculate F.C.S. for frame */
    prh_rftest_add_fcs(frame, frame->dataLen);

    return 0;
}
   
/*
 * prh_rftest_create_ua
 * -- Create a UA frame from the info passed in.
 *
 */
int prh_rftest_create_ua(t_DataBuf *frame, prh_t_rfcomm_dlci DLCI, u_int8 fbit, u_int32 cresp)
{
    frame->data[PRH_RFCOMM_FR_ADDR_FIELD]=((DLCI & 0x3F)<<2) | (cresp << 1) | PRH_RFCOMM_EA_BIT;
    frame->data[PRH_RFCOMM_FR_CTRL_FIELD] = PRH_RFCOMM_FR_CTRL_UA;

    if (fbit)
    frame->data[PRH_RFCOMM_FR_CTRL_FIELD] |= PRH_RFCOMM_FR_CTRL_PF_BIT;

    /* Set EA bit to 1 in length field and length to 0 */
    frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] = PRH_RFCOMM_EA_BIT;

    frame->dataLen=3;

    /* Need to calculate F.C.S. for frame */
    prh_rftest_add_fcs(frame, frame->dataLen);

    return 0;
}

/*
 * prh_rftest_create_dm
 * -- Create a DM from the passed in information.
 *
 */
int prh_rftest_create_dm(t_DataBuf *frame, prh_t_rfcomm_dlci DLCI, u_int8 fbit, u_int32 cresp)
{
    frame->data[PRH_RFCOMM_FR_ADDR_FIELD]=((DLCI & 0x3F)<<2) | (cresp << 1) | PRH_RFCOMM_EA_BIT;

    frame->data[PRH_RFCOMM_FR_CTRL_FIELD] = PRH_RFCOMM_FR_CTRL_DM;

    if (fbit)
    frame->data[PRH_RFCOMM_FR_CTRL_FIELD] |= PRH_RFCOMM_FR_CTRL_PF_BIT;

    /* Set EA bit to 1 in length field and length to 0 */
    frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] = PRH_RFCOMM_EA_BIT;
 
    /* Need to calculate F.C.S. for frame */
    frame->dataLen=3;
    prh_rftest_add_fcs(frame, frame->dataLen);

    return 0;
}

/*
 * prh_rftest_create_disc
 * -- Create a DISC frame from the info passed in 
 */
int prh_rftest_create_disc(t_DataBuf *frame, prh_t_rfcomm_dlci DLCI, u_int8 fbit, u_int32 cresp)
{
  /* Address field, DLC, Command/Respone bit, EA Bit */
  frame->data[PRH_RFCOMM_FR_ADDR_FIELD]= ((DLCI & 0x3F)<<2) | (cresp << 1) | PRH_RFCOMM_EA_BIT;

  /* Control Field - DISC Command + P/F BIT */
  frame->data[PRH_RFCOMM_FR_CTRL_FIELD] = PRH_RFCOMM_FR_CTRL_DISC;
  
  if (fbit)
    frame->data[PRH_RFCOMM_FR_CTRL_FIELD] |= PRH_RFCOMM_FR_CTRL_PF_BIT;
  
  /* Set EA bit to 1 in length field and length to 0 */
  frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] = PRH_RFCOMM_EA_BIT;
  frame->dataLen=3;    
  /* Need to calculate F.C.S. for frame */
  prh_rftest_add_fcs(frame, frame->dataLen);
  
  return 0;
}

/*
 * prh_rftest_create_uih
 * -- Create a UIH frame from the buffer passed in
 *
 */
int prh_rftest_create_uih(t_DataBuf *frame, prh_t_rfcomm_dlci DLCI, u_int8 pbit, u_int32 cresp)
{
  /* check frame length */
  if (frame->dataLen>127)
    {
      rftest_data_buf_release_header(frame, 4);
      /* From length indicator in GSM 07.10 5.2.1.5 */
     frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] = (((frame->dataLen-4) & 0x7F)<<1);

     frame->data[PRH_RFCOMM_FR_LENGTH_FIELD2] = ((frame->dataLen-4) >> 7);
    }
  else
    {
      rftest_data_buf_release_header(frame, 3);
      frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] = ((frame->dataLen-3)<<1) | PRH_RFCOMM_EA_BIT;
    }

  frame->data[PRH_RFCOMM_FR_ADDR_FIELD]=((DLCI & 0x3f)<<2) | (cresp << 1) | PRH_RFCOMM_EA_BIT;
  frame->data[PRH_RFCOMM_FR_CTRL_FIELD] = PRH_RFCOMM_FR_CTRL_UIH;

  if (pbit)
    frame->data[PRH_RFCOMM_FR_CTRL_FIELD] |= PRH_RFCOMM_FR_CTRL_PF_BIT;

  /* Add F.C.S. */
  prh_rftest_add_fcs(frame, 2);
  return 0;
}


#if PRH_RFTEST_CREDIT_FLOW_CONTROL
/*
 * prh_rftest_create_uih_credit
 * -- Create a UIH frame from the buffer passed in with CREDIT info
 *
 */
int prh_rftest_create_uih_credit(t_DataBuf *frame, prh_t_rfcomm_dlci DLCI, u_int8 pbit, u_int32 cresp, u_int8 num_credits)
{
  /* check frame length */
  if (frame->dataLen>127)
    {
      rftest_data_buf_release_header(frame, 5);
      /* From length indicator in GSM 07.10 5.2.1.5 */
     frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] = (((frame->dataLen-5) & 0x7F)<<1);

     frame->data[PRH_RFCOMM_FR_LENGTH_FIELD2] = ((frame->dataLen-5) >> 7);
     frame->data[PRH_RFCOMM_FR_LENGTH_FIELD2+1] = num_credits;
    }
  else
    {
      rftest_data_buf_release_header(frame, 4);
      frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] = ((frame->dataLen-4)<<1) | PRH_RFCOMM_EA_BIT;
      frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1+1] = num_credits;
    }

  frame->data[PRH_RFCOMM_FR_ADDR_FIELD]=((DLCI & 0x3f)<<2) | (cresp << 1) | PRH_RFCOMM_EA_BIT;
  frame->data[PRH_RFCOMM_FR_CTRL_FIELD] = PRH_RFCOMM_FR_CTRL_UIH;

  if (pbit)
    frame->data[PRH_RFCOMM_FR_CTRL_FIELD] |= PRH_RFCOMM_FR_CTRL_PF_BIT;
  
  /* Add F.C.S. */
  prh_rftest_add_fcs(frame, 2);
  return 0;
}
#endif

/* 
 * prh_rftest_destroy_uih
 * -- Remove UIH headers + FCS for Flow control
 */
int prh_rftest_destroy_uih(t_DataBuf *frame)
{
  int len;

  if (frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] & PRH_RFCOMM_EA_BIT)
      {
	/* length field is single byte */
	len = frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] >> 1;
	rftest_data_buf_reserve_header(frame, 3);
      }
    else
      {
	/* length field is two bytes */
	len = (frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] >> 1) + (frame->data[PRH_RFCOMM_FR_LENGTH_FIELD2] << 7);
	rftest_data_buf_reserve_header(frame, 4);
      }
  /* Remove FCS from end of frame */
  frame->dataLen--;
  return BT_NOERROR;
}

/*
 * prh_rftest_add_fcs
 * -- Add Frame Check Sequence to frame.
 *
 * This function generates the FCS bytes for fcs_len bytes in frame frame.
 * and puts it at the end of the frame.
 */
int prh_rftest_add_fcs(t_DataBuf *frame, int fcs_len)
{
  unsigned char FCS=0xFF;

  unsigned char len=fcs_len;

  unsigned char *p=frame->data;

  while (len--) {
    FCS=rftest_crctable[FCS^*p++];
  }
  
  FCS=0xFF-FCS;

  
  frame->data[frame->dataLen]=FCS;
  frame->dataLen+=1;
#if pDEBUG
  prh_rftest_dump_frame(frame, 1);
#endif
  return 0;
}

/*
 * prh_rftest_rx_fcs
 * -- Check FCS sequence is correct
 * 
 * This function checks the FCS for fcs_len bytes of frame is equal to the 
 * FCS received at the end of the frame
 */
int prh_rftest_rx_fcs(t_DataBuf *frame, int fcs_len)
{
  unsigned char FCS=0xFF;
  unsigned char len;
  unsigned char *p;
  unsigned char gotFCS;
  
  len=fcs_len;
  gotFCS=frame->data[frame->dataLen-1];
  p=frame->data;
  while(len--) {
    FCS=rftest_crctable[FCS^*p++];
  }
#if pDEBUG
  prh_rftest_dump_frame(frame, 0);
#endif
  FCS=rftest_crctable[FCS^gotFCS];

  if (FCS==0xCF) {
    /* remove FCS byte from frame */
    frame->dataLen--;
    return 0;
  }
  else
    {
      pDebugPrintf((pLOGCRITICAL,"RFCOMM: FCS: %02X BAD\n", gotFCS));
#if pDEBUG
      prh_rftest_dump_frame(frame, 0);
#endif
      return -1;
    }
}

/*
 * prh_rftest_dump_frame
 * -- Debugging function to dump contents of a frame
 */
int prh_rftest_dump_frame(t_DataBuf *frame, int tx)
{
#if pDEBUG
  unsigned int i;
  static int tx_cnt=0, rx_cnt=0;
  
  if (tx!=0)
    {
      pDebugPrintf((pLOGDEBUG, "RFCOMM: tx: frame %02d: %02d: ", rx_cnt++, frame->dataLen));
    }
  else
    {
      pDebugPrintf((pLOGDEBUG, "RFCOMM: rx: frame %02d: %02d: ", tx_cnt++, frame->dataLen));
    }

  for (i=0; i<frame->dataLen; i++)
    pDebugPrintf((pLOGDEBUG, "%02X ", frame->data[i]));
  pDebugPrintf((pLOGDEBUG, "\n"));
#endif
  return 0;
}


