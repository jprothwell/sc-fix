/***********************************************************************
 *
 * MODULE NAME:    rfcomm_frames.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    RFCOMM Frame creation 
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  02 Feb 2000
 *
 * SOURCE CONTROL: $Id: rfcomm_frames.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Inc.
 *     All rights reserved.
 *
 *
 ***********************************************************************/

#include "rfcomm_lower_layer.h"
#include "rfcomm_ue_api.h"
#include "rfcomm_core.h"
#include "rfcomm_frame.h"
#include "rfcomm_crc.h"
#include "rfcomm_proto.h"
#include "rfcomm_debug.h"
#if COMBINED_HOST==1
#include "papi.h"
#endif
/*
 * prh_rfcomm_create_sabm
 * -- Create an SABM frame from the info passed in.
 *
 */
int prh_rfcomm_create_sabm(struct host_buf *frame, prh_t_rfcomm_dlci DLCI, u_int8 pbit, u_int32 cresp)
{
    /* Put DLCI into address field, SABM is a command and EA is always set */
    frame->data[PRH_RFCOMM_FR_ADDR_FIELD]=((DLCI & 0x3F)<<2) | (cresp << 1) | PRH_RFCOMM_EA_BIT;
    
    frame->data[PRH_RFCOMM_FR_CTRL_FIELD] = PRH_RFCOMM_FR_CTRL_SABM;

    if (pbit)
    frame->data[PRH_RFCOMM_FR_CTRL_FIELD] |= PRH_RFCOMM_FR_CTRL_PF_BIT;
    
    /* Set EA bit to 1 in length field and length to 0 */
    frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] = PRH_RFCOMM_EA_BIT;

    frame->len=3;
    /* Need to calculate F.C.S. for frame */
    prh_rfcomm_add_fcs(frame, frame->len);

    return 0;
}
   
/*
 * prh_rfcomm_create_ua
 * -- Create a UA frame from the info passed in.
 *
 */
int prh_rfcomm_create_ua(struct host_buf *frame, prh_t_rfcomm_dlci DLCI, u_int8 fbit, u_int32 cresp)
{
    frame->data[PRH_RFCOMM_FR_ADDR_FIELD]=((DLCI & 0x3F)<<2) | (cresp << 1) | PRH_RFCOMM_EA_BIT;
    frame->data[PRH_RFCOMM_FR_CTRL_FIELD] = PRH_RFCOMM_FR_CTRL_UA;

    if (fbit)
    frame->data[PRH_RFCOMM_FR_CTRL_FIELD] |= PRH_RFCOMM_FR_CTRL_PF_BIT;

    /* Set EA bit to 1 in length field and length to 0 */
    frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] = PRH_RFCOMM_EA_BIT;

    frame->len=3;

    /* Need to calculate F.C.S. for frame */
    prh_rfcomm_add_fcs(frame, frame->len);

    return 0;
}

/*
 * prh_rfcomm_create_dm
 * -- Create a DM from the passed in information.
 *
 */
int prh_rfcomm_create_dm(struct host_buf *frame, prh_t_rfcomm_dlci DLCI, u_int8 fbit, u_int32 cresp)
{
    frame->data[PRH_RFCOMM_FR_ADDR_FIELD]=((DLCI & 0x3F)<<2) | (cresp << 1) | PRH_RFCOMM_EA_BIT;

    frame->data[PRH_RFCOMM_FR_CTRL_FIELD] = PRH_RFCOMM_FR_CTRL_DM;

    if (fbit)
    frame->data[PRH_RFCOMM_FR_CTRL_FIELD] |= PRH_RFCOMM_FR_CTRL_PF_BIT;

    /* Set EA bit to 1 in length field and length to 0 */
    frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] = PRH_RFCOMM_EA_BIT;
 
    /* Need to calculate F.C.S. for frame */
    frame->len=3;
    prh_rfcomm_add_fcs(frame, frame->len);

    return 0;
}

/*
 * prh_rfcomm_create_disc
 * -- Create a DISC frame from the info passed in 
 */
int prh_rfcomm_create_disc(struct host_buf *frame, prh_t_rfcomm_dlci DLCI, u_int8 fbit, u_int32 cresp)
{
  /* Address field, DLC, Command/Respone bit, EA Bit */
  frame->data[PRH_RFCOMM_FR_ADDR_FIELD]= ((DLCI & 0x3F)<<2) | (cresp << 1) | PRH_RFCOMM_EA_BIT;

  /* Control Field - DISC Command + P/F BIT */
  frame->data[PRH_RFCOMM_FR_CTRL_FIELD] = PRH_RFCOMM_FR_CTRL_DISC;
  
  if (fbit)
    frame->data[PRH_RFCOMM_FR_CTRL_FIELD] |= PRH_RFCOMM_FR_CTRL_PF_BIT;
  
  /* Set EA bit to 1 in length field and length to 0 */
  frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] = PRH_RFCOMM_EA_BIT;
  frame->len=3;    
  /* Need to calculate F.C.S. for frame */
  prh_rfcomm_add_fcs(frame, frame->len);
  
  return 0;
}

/*
 * prh_rfcomm_create_uih
 * -- Create a UIH frame from the buffer passed in
 *
 */
int prh_rfcomm_create_uih(struct host_buf *frame, prh_t_rfcomm_dlci DLCI, u_int32 cresp, u_int8 pfbit)  //zhu jianguo changed it. 2008.12.5
{
    /* check frame length */
    if (frame->len>127)
    {
        host_buf_release_header(frame, 4);
        /* From length indicator in GSM 07.10 5.2.1.5 */
        frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] = (((frame->len-4) & 0x7F)<<1);

        frame->data[PRH_RFCOMM_FR_LENGTH_FIELD2] = ((frame->len-4) >> 7);
        frame->data[PRH_RFCOMM_FR_ADDR_FIELD]=((DLCI & 0x3f)<<2) | (cresp << 1) | PRH_RFCOMM_EA_BIT;
        frame->data[PRH_RFCOMM_FR_CTRL_FIELD] = PRH_RFCOMM_FR_CTRL_UIH;
    }
    else
    {
        //zhu jianguo changed to align the address. 2008.11.19
        int i=0;
        //host_buf_release_header(frame, 3);  
        host_buf_release_header(frame, 4);
        frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] = ((frame->len-4)<<1) | PRH_RFCOMM_EA_BIT;
        frame->data[PRH_RFCOMM_FR_ADDR_FIELD]=((DLCI & 0x3f)<<2) | (cresp << 1) | PRH_RFCOMM_EA_BIT;
       // frame->data[PRH_RFCOMM_FR_CTRL_FIELD] = PRH_RFCOMM_FR_CTRL_UIH;

        /*if(pfbit==1)
        {
            frame->data[PRH_RFCOMM_FR_CTRL_FIELD] = PRH_RFCOMM_FR_CTRL_UIH| PRH_RFCOMM_FR_CTRL_PF_BIT; 
//            frame->data[PRH_RFCOMM_FR_LENGTH_FIELD2] = 0x1e;
            frame->data[PRH_RFCOMM_FR_LENGTH_FIELD2] = 0x00;
        }
        else*/
        {
            frame->data[PRH_RFCOMM_FR_CTRL_FIELD] = PRH_RFCOMM_FR_CTRL_UIH;
            for(i=3;i<frame->len;i++)
                frame->data[i] = frame->data[i+1];
            
            frame->len--;
        }

    }

    //zhu jianguo changed to align the address. 2008.11.19
    //frame->data[PRH_RFCOMM_FR_ADDR_FIELD]=((DLCI & 0x3f)<<2) | (cresp << 1) | PRH_RFCOMM_EA_BIT;
    //frame->data[PRH_RFCOMM_FR_CTRL_FIELD] = PRH_RFCOMM_FR_CTRL_UIH;

    /* according to TS spec 5.4.3.1 both stations always set p-bit to 0 */
    /* according to RFCOMM in CBFC we set pbit to 0 if no credit in packet */
    /*  if (pbit)
    frame->data[PRH_RFCOMM_FR_CTRL_FIELD] |= PRH_RFCOMM_FR_CTRL_PF_BIT; */

    /* Add F.C.S. */
    prh_rfcomm_add_fcs(frame, 2);
    return 0;
}


/*
 * prh_rfcomm_create_uih_credit
 * -- Create a UIH frame from the buffer passed in with CREDIT info
 *
 */
int prh_rfcomm_create_uih_credit(struct host_buf *frame, prh_t_rfcomm_dlci DLCI, u_int32 cresp, u_int8 num_credits)
{
  /* check frame length */
  if (frame->len>127)
    {
      host_buf_release_header(frame, 5);
      /* From length indicator in GSM 07.10 5.2.1.5 */
     frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] = (((frame->len-5) & 0x7F)<<1);

     frame->data[PRH_RFCOMM_FR_LENGTH_FIELD2] = ((frame->len-5) >> 7);
     frame->data[PRH_RFCOMM_FR_LENGTH_FIELD2+1] = num_credits;
    }
  else
    {
      host_buf_release_header(frame, 4);
      frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] = ((frame->len-4)<<1) | PRH_RFCOMM_EA_BIT;
      frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1+1] = num_credits;
    }

  frame->data[PRH_RFCOMM_FR_ADDR_FIELD]=((DLCI & 0x3f)<<2) | (cresp << 1) | PRH_RFCOMM_EA_BIT;
  frame->data[PRH_RFCOMM_FR_CTRL_FIELD] = PRH_RFCOMM_FR_CTRL_UIH;

  /* according to TS spec 5.4.3.1 both stations always set p-bit to 0 */
  /* in RFCOMM if CBFC and credits the pbit is set to 1 always */
  frame->data[PRH_RFCOMM_FR_CTRL_FIELD] |= PRH_RFCOMM_FR_CTRL_PF_BIT;
  
  /* Add F.C.S. */
  prh_rfcomm_add_fcs(frame, 2);
  return 0;
}

/* 
 * prh_rfcomm_destroy_uih
 * -- Remove UIH headers + FCS for Flow control
 */
int prh_rfcomm_destroy_uih(struct host_buf *frame)
{
  int len;

  if (frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] & PRH_RFCOMM_EA_BIT)
      {
	/* length field is single byte */
	len = frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] >> 1;
	host_buf_reserve_header(frame, PRH_RFCOMM_BUF_UIH_SHORT);
      }
    else
      {
	/* length field is two bytes */
	len = (frame->data[PRH_RFCOMM_FR_LENGTH_FIELD1] >> 1) + (frame->data[PRH_RFCOMM_FR_LENGTH_FIELD2] << 7);
	host_buf_reserve_header(frame, PRH_RFCOMM_BUF_UIH_LONG);
      }
  /* Remove FCS from end of frame */
  frame->len--;
  return BT_NOERROR;
}

/*
 * prh_rfcomm_add_fcs
 * -- Add Frame Check Sequence to frame.
 *
 * This function generates the FCS bytes for fcs_len bytes in frame frame.
 * and puts it at the end of the frame.
 */
int prh_rfcomm_add_fcs(struct host_buf *frame, int fcs_len)
{
  unsigned char FCS=0xFF;

  unsigned char len=fcs_len;

  unsigned char *p=frame->data;

  while (len--) {
    FCS=crctable[FCS^*p++];
  }
  
  FCS=0xFF-FCS;

  
  frame->data[frame->len]=FCS;
  frame->len+=1;
//#if pDEBUG
//  prh_rfcomm_dump_frame(frame, 1);
//#endif
  return 0;
}

/*
 * prh_rfcomm_rx_fcs
 * -- Check FCS sequence is correct
 * 
 * This function checks the FCS for fcs_len bytes of frame is equal to the 
 * FCS received at the end of the frame
 */
int prh_rfcomm_rx_fcs(struct host_buf *frame, int fcs_len)
{
  unsigned char FCS=0xFF;
  unsigned char len;
  unsigned char *p;
  unsigned char gotFCS;

  #if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))

u_int8 temp;
  #endif
  
  
  len=fcs_len;
  gotFCS=frame->data[frame->len-1];
  p=frame->data;
  while(len--) {
    FCS=crctable[FCS^*p++];
  }
//#if pDEBUG
//  prh_rfcomm_dump_frame(frame, 0);
//#endif
  FCS=crctable[FCS^gotFCS];

  if (FCS==0xCF) {
    /* remove FCS byte from frame */
    frame->len--;
    return 0;
  }
  else
    {
#if pDEBUG
      pDebugPrintfEX((pLOGCRITICAL,pLOGRFCOMM,"RFCOMM: FCS: %02X BAD\n", gotFCS));

#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
     SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM,LOG_RFC_RFCOMM_FRAMES_INDEX,&gotFCS,1,BT_CODE_BYTE);
#endif
//#if pDEBUG
//      prh_rfcomm_dump_frame(frame, 0);
//#endif
      return -1;
    }
}

/*
 * prh_rfcomm_dump_frame
 * -- Debugging function to dump contents of a frame
 */
int prh_rfcomm_dump_frame(struct host_buf *frame, int tx)
{
#if pDEBUG
  static int tx_cnt=0, rx_cnt=0;
  
  if (tx!=0)
    {
      pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM, "RFCOMM: tx: frame %02d: %02d: ", rx_cnt++, frame->len));
    }
  else
    {
      pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM, "RFCOMM: rx: frame %02d: %02d: ", tx_cnt++, frame->len));
    }

  pDebugDumpEX(pLOGDEBUG,pLOGRFCOMM,frame->data,frame->len);


#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    static int tx_cnt=0, rx_cnt=0;
    u_int8 temp[8];
  if (tx!=0)
    {
      BT_WRITE_LE_UINT32(&(temp[0]),(rx_cnt++));
      BT_WRITE_LE_UINT32(&(temp[4]),frame->len);
      SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM, (LOG_RFC_RFCOMM_FRAMES_INDEX+1), temp,8,(BT_CODE_DWORD|(BT_CODE_DWORD<<4)));
    }
  else
    {
      BT_WRITE_LE_UINT32(&(temp[0]),(rx_cnt++));
      BT_WRITE_LE_UINT32(&(temp[4]),frame->len);
      SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM,(LOG_RFC_RFCOMM_FRAMES_INDEX+2),temp, 8,(BT_CODE_DWORD|(BT_CODE_DWORD<<4)));
    }
#endif

  return 0;
}


