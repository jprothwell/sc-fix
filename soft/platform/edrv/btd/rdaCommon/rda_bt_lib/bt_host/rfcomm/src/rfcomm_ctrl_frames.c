/***********************************************************************
 *
 * MODULE NAME:    rfcomm_ctrl_frames.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    RFCOMM Control Channel Handler Frame Assembly
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  18 April 2000
 *
 * SOURCE CONTROL: $Id: rfcomm_ctrl_frames.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Inc.
 *     All rights reserved.
 *
 ***********************************************************************/

#include "rfcomm_lower_layer.h"
#include "rfcomm_ue_api.h"
#include "rfcomm_core.h"
#include "rfcomm_frame.h"
#include "rfcomm_proto.h"
#include "rfcomm_debug.h"

/* All functions within this file encode control channel messages from
 * structures as per GSM 07.10 v7.1.
 *
 * Most of this code has been optimised to use a pointer (ptr) to put the bytes * into the structures.
 * All functions take a frame pointer which is the host buffer into which 
 * the data is to be encoded, and a cresp byte which denotes whether the 
 * frame to be encoded is a command or a response frame.
 *
 */

/*
 * prh_rfcomm_cc_create_test
 * -- Create a Test Command from a frame
 *
 * Takes the test data puts a test command header on it and sets the length
 * field accordingly
 */
int prh_rfcomm_cc_create_test(struct host_buf *frame, u_int8 cresp)
{
  int data_len=frame->len;

  if (data_len>0x7f)
    host_buf_release_header(frame, 3);
  else
    host_buf_release_header(frame, 2);

  frame->data[PRH_RFCOMM_CC_TYPE_FIELD] = PRH_RFCOMM_CC_TYPE_TEST;
  frame->data[PRH_RFCOMM_CC_TYPE_FIELD] |= (cresp<<1) | PRH_RFCOMM_EA_BIT;
  if (data_len>0x7f)
    {
      frame->data[PRH_RFCOMM_CC_LENGTH_FIELD]= ((data_len & 0x7f) << 1);
      frame->data[PRH_RFCOMM_CC_LENGTH_FIELD+1]= 1 | (((data_len>>7) & 0x7f) << 1);
    }
  else
    {
      frame->data[PRH_RFCOMM_CC_LENGTH_FIELD]= 1 | ((data_len & 0x7f) << 1);
    }
  

  return 0;
}

int prh_rfcomm_cc_destroy_test(struct host_buf *frame)
{
  if (frame->data[PRH_RFCOMM_CC_LENGTH_FIELD] & 1)
    host_buf_reserve_header(frame, 2);
  else
    host_buf_reserve_header(frame, 3);
  return 0;
}

/* 
 * prh_rfcomm_cc_create_fcon
 * -- Create an FCON frame.
 */
int prh_rfcomm_cc_create_fcon(struct host_buf *frame, u_int8 cresp)
{
  unsigned char *ptr=frame->data;

  *ptr++=PRH_RFCOMM_CC_TYPE_FCON | (cresp<<1) | PRH_RFCOMM_EA_BIT;
  *ptr++=PRH_RFCOMM_CC_FCON_LEN | PRH_RFCOMM_EA_BIT;
  frame->len=2;

  return 0;
}

/* 
 * prh_rfcomm_cc_create_fcoff
 * -- Create an FCOFF frame
 */
int prh_rfcomm_cc_create_fcoff(struct host_buf *frame, u_int8 cresp)
{
  unsigned char *ptr=frame->data;

  *ptr++=PRH_RFCOMM_CC_TYPE_FCOFF | (cresp<<1) | PRH_RFCOMM_EA_BIT;
  *ptr++=PRH_RFCOMM_CC_FCOFF_LEN | PRH_RFCOMM_EA_BIT;
  frame->len=2;
  return 0;
}

/* 
 * prh_rfcomm_cc_create_nsc
 * -- Create an NSC frame.
 */
int prh_rfcomm_cc_create_nsc(struct host_buf *frame, u_int8 cresp, u_int8 nscom, u_int8 nscomm_cr)
{
  unsigned char *ptr=frame->data;
  
  *ptr++ = PRH_RFCOMM_CC_TYPE_NSC | (cresp<<1) | PRH_RFCOMM_EA_BIT;
  *ptr++ = 1<<1 | PRH_RFCOMM_EA_BIT;
  *ptr = (nscom) | (nscomm_cr << 1) | PRH_RFCOMM_EA_BIT;
  frame->len=3;
  return 0;
}

/*
 * Functions to convert from the RFCOMM enums to ints... 
 * currently doesn't need anything other than a cast
 */
#define PRH_RFCOMM_SPEED_TO_RPN_RATE(port_speed) ((int)port_speed)
#define PRH_RFCOMM_DATABITS_TO_DBITS(data_bits) ((int)data_bits)
#define PRH_RFCOMM_STOPBITS_TO_SBITS(stop_bits) ((int)stop_bits)
#define PRH_RFCOMM_PARENBITS_TO_PBIT(parity_enb) ((int)parity_enb)
#define PRH_RFCOMM_PARTYPE_TO_PTBITS(paritytype) ((int)paritytype)

/*
 * prh_rfcomm_cc_create_rpn_req
 * -- Create an RPN Request Frame
 *
 * This encode an RPN Request frame for a specific DLCI.
 */
int prh_rfcomm_cc_create_rpn_req(struct host_buf *frame, u_int8 cresp, prh_t_rfcomm_dlci dlci)
{
  unsigned char *ptr=frame->data;
  
  (*ptr++) = PRH_RFCOMM_CC_TYPE_RPN | (cresp << 1) | PRH_RFCOMM_EA_BIT;
  (*ptr++) = (PRH_RFCOMM_CC_RPN_LEN_REQ << 1) | PRH_RFCOMM_EA_BIT;
  (*ptr++) = (dlci<<2) | PRH_RFCOMM_CR_BIT | PRH_RFCOMM_EA_BIT;

  frame->len=3;
  return 0;
}

/* 
 * prh_rfcomm_cc_create_rpn_comm
 * -- Create an RPN Command type frame (can be a command or respone)
 *
 */
int prh_rfcomm_cc_create_rpn_comm(struct host_buf *frame, u_int8 cresp, prh_t_rfcomm_dlci dlci ,prh_t_rfcomm_port_params *port_parameters)
{
  unsigned char *ptr=frame->data;
 
  /* Command Byte */
  *ptr++ = PRH_RFCOMM_CC_TYPE_RPN | (cresp << 1)| PRH_RFCOMM_EA_BIT;

  /* Length Byte */
  *ptr++ = (PRH_RFCOMM_CC_RPN_LEN_COMM << 1) | PRH_RFCOMM_EA_BIT;
  
  /* DLCI */
   *ptr++ = (dlci<<2) | PRH_RFCOMM_CR_BIT | PRH_RFCOMM_EA_BIT;
  
  /* Octet1 Optional */  /* Baud Rate */
  *ptr++=PRH_RFCOMM_SPEED_TO_RPN_RATE(port_parameters->port_speed);
  
  /* DATA BITS */
  *ptr = PRH_RFCOMM_DATABITS_TO_DBITS(port_parameters->data_bits);
  *ptr |= (PRH_RFCOMM_STOPBITS_TO_SBITS(port_parameters->stop_bits)<<2);
  *ptr |= (PRH_RFCOMM_PARENBITS_TO_PBIT(port_parameters->parity_enable)<<3);
  *ptr++ |= (PRH_RFCOMM_PARTYPE_TO_PTBITS(port_parameters->paritytype)<<4);

  /* Flow Control Bytes x 3*/
  *ptr++=port_parameters->flow_mask;
  *ptr++=port_parameters->xon; /* DC11 */
  *ptr++=port_parameters->xoff; /* DC13 */
   
  /* Parameters Masks */
  /* Set to denote that baud, data bits, stop and parity  are changing
     need to find a nice way to do this */
  /* Many issues lie herein */
  *ptr++=port_parameters->param_mask & 0xFF;
  *ptr++=(port_parameters->param_mask & 0xFF00) >> 8;
  frame->len=0xA;
  return 0;
}


/*
 * prh_rfcomm_cc_create_msc
 * -- Create a Modem Status Command frame
 * 
 * decides if it has to encode a break signal which is a longer frame, 
 * sets the length appropriately.
 */
int prh_rfcomm_cc_create_msc(struct host_buf *frame, u_int8 cresp, prh_t_rfcomm_dlci dlci, prh_t_rfcomm_control_params *ctrl_params)
{
  unsigned char *ptr=frame->data;

  /* message type */
  *ptr++ =  PRH_RFCOMM_CC_TYPE_MSC | (cresp<<1) | PRH_RFCOMM_EA_BIT;
  
  /* Length byte */
  if (ctrl_params->break_signal)
    *ptr++ = PRH_RFCOMM_CC_MSC_LONG_LEN << 1 | PRH_RFCOMM_EA_BIT;
  else
    *ptr++ = PRH_RFCOMM_CC_MSC_SHORT_LEN << 1 | PRH_RFCOMM_EA_BIT;

  /* Channel ID */
  *ptr++ = (dlci << 2) | PRH_RFCOMM_CR_BIT | PRH_RFCOMM_EA_BIT;
  
  if (!ctrl_params->break_signal)
    *ptr=PRH_RFCOMM_EA_BIT;
  else
    *ptr=0;

  if (ctrl_params->flow_state==PRH_RFCOMM_FLOW_DATA_STOP)
    {
      *ptr |= PRH_RFCOMM_CC_MSC_CSF_FC;
    }

  if ((ctrl_params->modem_signal & PRH_RFCOMM_MSC_DTRDSR) == PRH_RFCOMM_MSC_DTRDSR)
    *ptr |= PRH_RFCOMM_CC_MSC_CSF_RTC;
  if ((ctrl_params->modem_signal & PRH_RFCOMM_MSC_RTSCTS) == PRH_RFCOMM_MSC_RTSCTS)
    *ptr |= PRH_RFCOMM_CC_MSC_CSF_RTR;
  if ((ctrl_params->modem_signal & PRH_RFCOMM_MSC_RI) == PRH_RFCOMM_MSC_RI)
    *ptr |= PRH_RFCOMM_CC_MSC_CSF_IC;
  if ((ctrl_params->modem_signal & PRH_RFCOMM_MSC_DCD) == PRH_RFCOMM_MSC_DCD)
    *ptr |= PRH_RFCOMM_CC_MSC_CSF_DV;
  ptr++;
   
  if (ctrl_params->break_signal)
    {
      *ptr = PRH_RFCOMM_EA_BIT | PRH_RFCOMM_CC_MSC_BS_B1;
      *ptr++ |= (ctrl_params->break_signal & 0xF) << 4;
    }

  frame->len=ptr-frame->data;
  return 0;
}

/* 
 * prh_rfcomm_cc_create_rls
 * -- Creates a Remote Line Status frame
 * 
 */
int prh_rfcomm_cc_create_rls(struct host_buf *frame, u_int8 cresp, prh_t_rfcomm_dlci dlci, prh_t_rfcomm_line_stat_params line_stat_params)
{
  unsigned char *ptr=frame->data;
  *ptr++ = PRH_RFCOMM_CC_TYPE_RLS | cresp<<1 | PRH_RFCOMM_EA_BIT;

  *ptr++ = PRH_RFCOMM_CC_LSR_LEN << 1 | PRH_RFCOMM_EA_BIT;

  *ptr++ = (dlci<<2) | PRH_RFCOMM_EA_BIT | PRH_RFCOMM_CR_BIT;

  *ptr=line_stat_params;
  
  frame->len=4;
  return 0;
}

/* 
 * prh_rfcomm_cc_create_credit_pn
 * -- Creates a Port Negotiation Frame
 *
 * This function uses default values from the RFCOMM Spec.
 */
int prh_rfcomm_cc_create_credit_pn(struct host_buf *frame, u_int8 cresp, prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, u_int8 clbits, u_int8 kbits)
{
  unsigned char *ptr=frame->data;

  *ptr++ = PRH_RFCOMM_CC_TYPE_PN | PRH_RFCOMM_EA_BIT | cresp<<1;

  *ptr++ = PRH_RFCOMM_CC_PN_LEN << 1 | PRH_RFCOMM_EA_BIT;
  
  /* DLCI */
  *ptr++ = dlci;
  
  /* Frame type and convergence layer */
  /* v1.1 changes this, encode c bits for Credit Based Flow Control */
  *ptr++ = (clbits&0x0F) << 4;
  
  /* DLC Priority */
  *ptr++ = 0; /*dlc_params->priority;*/ /* needs changing */

  /* Acknowledgement Timer T1 - non neg in RFCOMM */
  *ptr++ = 0;
  
  /* Max Frame Size - two bytes */
  *ptr++ = (dlc_params->max_frame_size & 0xFF);
  *ptr++ = (dlc_params->max_frame_size & 0xFF00) >> 8;
  
  /* NA1->NA8 number of retrans N2 always 0 for RFCOMM */
  *ptr++ = 0;
  
  /* K1 -> K3 window size for error mode, 0 for RFCOMM */
  if (clbits)
    *ptr++ = kbits;
  else
    *ptr++ = 0;
 

  frame->len=0xA;
  return 0;
}
