/***********************************************************************
 *
 * MODULE NAME:    rfcomm_test_gen.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:
 * MAINTAINER:     Dave Airlie
 * CREATION DATE: 
 *
 * SOURCE CONTROL: $Id: rftest_pdu_gen.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    02.Feb.2000 -   DA - first import
 *
 *
 * ISSUES:
 *
 ***********************************************************************/
#include "host_config.h"

#include "host_types.h"
#include "host_error.h"
#include "papi.h"
#include "host_buf.h"

#include "l2cap_primitives.h"
#include "rfcomm_ue_api.h"
#include "rfcomm_core.h"
#include "rfcomm_frame.h"
#include "rfcomm_proto.h"
#include "rfcomm_debug.h"

#define LP_HEADER_LENS 10

void *rftest;
extern int test_num;
struct st_l2cap_inst
{
  t_bdaddr bd_addr;
  prh_t_rfcomm_cid cid;  
};

extern struct st_l2cap_inst *test_l2_inst;
extern int rftest_enabled;

int rftest_sendua(prh_t_rfcomm_dlci dlci, u_int8 fbit, u_int8 cresp)
{
  struct host_buf *frame;

  frame=host_buf_alloc(10+LP_HEADER_LENS);
  host_buf_reserve_header(frame, LP_HEADER_LENS);

  prh_rfcomm_create_ua(frame, dlci, fbit, cresp);
  
  L2_Write(test_l2_inst->cid, frame);
}

int rftest_sendsabm(prh_t_rfcomm_dlci dlci, u_int8 fbit, u_int8 cresp)
{
  struct host_buf *frame;
   frame=host_buf_alloc(10+LP_HEADER_LENS);
  host_buf_reserve_header(frame, LP_HEADER_LENS);

  prh_rfcomm_create_sabm(frame, dlci, fbit, cresp);
  
  L2_Write(test_l2_inst->cid, frame);
}

int rftest_senddisc(prh_t_rfcomm_dlci dlci, u_int8 fbit, u_int8 cresp)
{
  struct host_buf *frame;
   frame=host_buf_alloc(10+LP_HEADER_LENS);
  host_buf_reserve_header(frame, LP_HEADER_LENS);

  prh_rfcomm_create_disc(frame, dlci, fbit, cresp);
  
  L2_Write(test_l2_inst->cid, frame);
}

int rftest_sendmsc_reply(prh_t_rfcomm_dlci dlci, u_int8 cresp, char *inframe)
{
  struct host_buf *frame;
  
  frame=host_buf_alloc(20+LP_HEADER_LENS);
  host_buf_reserve_header(frame, LP_HEADER_LENS+5);
  
  frame->data[0]=PRH_RFCOMM_CC_TYPE_MSC | (cresp<<1) | PRH_RFCOMM_EA_BIT;
  frame->data[1]=PRH_RFCOMM_CC_MSC_SHORT_LEN << 1 | PRH_RFCOMM_EA_BIT;
  frame->data[2]=inframe[0];
  frame->data[3]=inframe[1];

  frame->len=4;

  prh_rfcomm_create_uih(frame, 0,0,0);
  L2_Write(test_l2_inst->cid, frame);
}

int rftest_sendmsc(prh_t_rfcomm_dlci dlci, u_int8 cresp, prh_t_rfcomm_control_params *ctrl_params)
{
  struct host_buf *frame;

  frame=host_buf_alloc(20+LP_HEADER_LENS);
  host_buf_reserve_header(frame, LP_HEADER_LENS+5);

  prh_rfcomm_cc_create_msc(frame, cresp, dlci, ctrl_params);
  prh_rfcomm_create_uih(frame, 0, 0, 0);
  L2_Write(test_l2_inst->cid, frame);
}

int rftest_sendtest(int cresp)
{
  struct host_buf *frame;
  int x;

  frame=host_buf_alloc(50+LP_HEADER_LENS);
  host_buf_reserve_header(frame, LP_HEADER_LENS+5);

  for (x=0; x<10; x++)
    frame->data[x]=x+'A';
  
  frame->len=10;
  prh_rfcomm_cc_create_test(frame, cresp);
  prh_rfcomm_create_uih(frame, 0, 0, 0);
  L2_Write(test_l2_inst->cid, frame);
}

int rftest_sendrls(prh_t_rfcomm_dlci dlci, u_int8 cresp, prh_t_rfcomm_line_stat_params lsp)
{
  struct host_buf *frame;
  frame=host_buf_alloc(50+LP_HEADER_LENS);
  host_buf_reserve_header(frame, LP_HEADER_LENS+5);

  prh_rfcomm_cc_create_rls(frame, cresp, dlci, lsp);
  prh_rfcomm_create_uih(frame, 0,0 ,1);
  L2_Write(test_l2_inst->cid, frame);
}

int rftest_sendpn(prh_t_rfcomm_dlci dlci, u_int8 cresp, u_int16 max_frame_size)
{
  struct host_buf *frame;
  prh_t_rfcomm_dlc_params dlc_params;

  dlc_params.max_frame_size=max_frame_size;
  dlc_params.priority=0;
  frame=host_buf_alloc(50+LP_HEADER_LENS);
  host_buf_reserve_header(frame, LP_HEADER_LENS+5);

  
  prh_rfcomm_cc_create_pn(frame, cresp, dlci, &dlc_params);
  prh_rfcomm_create_uih(frame, 0,0,1);
  L2_Write(test_l2_inst->cid, frame);

}

int rftest_sendrpnc(prh_t_rfcomm_dlci dlci, u_int8 cresp, prh_t_rfcomm_port_params *port_params)
{
  struct host_buf *frame;

  frame=host_buf_alloc(50+LP_HEADER_LENS);
  host_buf_reserve_header(frame, LP_HEADER_LENS+5);
  
  prh_rfcomm_cc_create_rpn_comm(frame, cresp, dlci, port_params);
  prh_rfcomm_create_uih(frame, 0,0,1);
  L2_Write(test_l2_inst->cid, frame);
  
}

int rftest_sendrpnr(prh_t_rfcomm_dlci dlci, u_int8 cresp)
{
  struct host_buf *frame;

  frame=host_buf_alloc(50+LP_HEADER_LENS);
  host_buf_reserve_header(frame, LP_HEADER_LENS+5);
  
  prh_rfcomm_cc_create_rpn_req(frame, cresp, dlci);
  prh_rfcomm_create_uih(frame, 0,0,1);
  L2_Write(test_l2_inst->cid, frame);
  
}

int rftest_send_fcon(int cresp)
{
 struct host_buf *frame;

  frame=host_buf_alloc(50+LP_HEADER_LENS);
  host_buf_reserve_header(frame, LP_HEADER_LENS+5);
  
  prh_rfcomm_cc_create_fcon(frame,cresp);
  prh_rfcomm_create_uih(frame, 0,0,1);
  L2_Write(test_l2_inst->cid, frame);
}

int rftest_send_fcoff(int cresp)
{
 struct host_buf *frame;

  frame=host_buf_alloc(50+LP_HEADER_LENS);
  host_buf_reserve_header(frame, LP_HEADER_LENS+5);
  
  prh_rfcomm_cc_create_fcoff(frame,cresp);
  prh_rfcomm_create_uih(frame, 0,0,1);
  L2_Write(test_l2_inst->cid, frame);
}

void rftest_l2cap_data_ind_cb(u_int16 cid, struct host_buf *p_buf)
{
  int ret=0;
  prh_t_rfcomm_dlci dlci=0;
  int type;
  int pfbit;
  if ((p_buf->data[PRH_RFCOMM_FR_CTRL_FIELD] & ~PRH_RFCOMM_FR_CTRL_PF_BIT)==PRH_RFCOMM_FR_CTRL_UIH)
    ret=prh_rfcomm_rx_fcs(p_buf, 2);
  else
    ret=prh_rfcomm_rx_fcs(p_buf, 3);
  
  if (ret==0)
    {
      
      dlci=(p_buf->data[PRH_RFCOMM_FR_ADDR_FIELD]&0xFC) >> 2;
      type=p_buf->data[PRH_RFCOMM_FR_CTRL_FIELD] & ~PRH_RFCOMM_FR_CTRL_PF_BIT;
      pfbit=(p_buf->data[PRH_RFCOMM_FR_CTRL_FIELD] & PRH_RFCOMM_FR_CTRL_PF_BIT)>>4;
      /* check whether data is an SABM,DISC, UA, DM etc... */
      switch(type)
    {
    case PRH_RFCOMM_FR_CTRL_SABM:
      /* Shouldn't get an SABM do what? */
      pDebugPrintf((pLOGNOTICE, "RFTEST: Got SABM %d\n", dlci));
      if (dlci==0)
        {
          if (rftest_enabled==1)
        {
          if (pfbit==1)
            rftest_passed();
          else
            rftest_failed();
        }
          else
        rftest_sendua(dlci, 1, 1);
        }
      else
        if (dlci>0 && rftest_enabled==5)
          {
        if (pfbit==1)
           rftest_passed();
          else
            rftest_failed();

          }
        else
          rftest_sendua(dlci, 1, 1);
      //      rfcomm_dlc_do_event(instance, PRH_RFCOMM_DLC_SABM, info);
      break;
    case PRH_RFCOMM_FR_CTRL_UA:
      pDebugPrintf((pLOGNOTICE, "RFTEST: Got UA DLCI: %d, Fbit: %d\n",dlci, pfbit ));
      if (dlci==0 && (rftest_enabled==2 || rftest_enabled==3))
        {
          if (pfbit==1)
        rftest_passed();
          else
        rftest_failed();
        }
      else
        if (rftest_enabled==6 || rftest_enabled==8)
          {
        if (pfbit==1)
          rftest_passed();
        else
          rftest_failed();
          }

      /* Unnumbered acknowledgement */
      //      rfcomm_dlc_do_event(instance, PRH_RFCOMM_DLC_UA, info);
      break;
    case PRH_RFCOMM_FR_CTRL_DM:
      pDebugPrintf((pLOGNOTICE, "RFTEST: Got DM %d\n",dlci));
      //      rfcomm_dlc_do_event(instance, PRH_RFCOMM_DLC_DM, info);
      break;
    case PRH_RFCOMM_FR_CTRL_DISC: 
      pDebugPrintf((pLOGNOTICE, "RFTEST: Got DISC DLCI: %d, pbit: %d\n",dlci, pfbit));
      if (rftest_enabled==7)
        {
          if (pfbit==1)
        rftest_passed();
          else
        rftest_failed();
        }
      
      rftest_sendua(dlci, 1, 1);
      
      //      rfcomm_dlc_do_event(instance, PRH_RFCOMM_DLC_DISC, info);
      break;
    case PRH_RFCOMM_FR_CTRL_UIH:
      pDebugPrintf((pLOGNOTICE, "RFTEST: Got UIH %d\n",dlci));
      rftest_handle_uih(p_buf, dlci);
      //      rfcomm_dlc_do_event(instance, PRH_RFCOMM_DLC_UIH, info);
      break;
    default:
      /* Unknown data type .. uggh */
      break;
    }
    }
  else
    rftest_failed();
}

int rftest_handle_uih(struct host_buf *buffer, prh_t_rfcomm_dlci dlci)
{
  int type;
  int ret;
  struct host_buf *out_buf;
  int cr_recv;
  int len;
  int dlci_recv;

  if (buffer->data[PRH_RFCOMM_FR_LENGTH_FIELD1] & PRH_RFCOMM_EA_BIT)
    {
      /* length field is single byte */
      len = buffer->data[PRH_RFCOMM_FR_LENGTH_FIELD1] >> 1;
      host_buf_reserve_header(buffer, 3);
    }
  else
    {
      /* length field is two bytes */
      len = (buffer->data[PRH_RFCOMM_FR_LENGTH_FIELD1] >> 1) + (buffer->data[PRH_RFCOMM_FR_LENGTH_FIELD2] << 7);
      host_buf_reserve_header(buffer, 4);
    }
  
  if (dlci==0)
    {
      type=buffer->data[PRH_RFCOMM_CC_TYPE_FIELD] & 0xFC;
      /* get the Command/Response bit from received command */
      cr_recv=(buffer->data[PRH_RFCOMM_CC_TYPE_FIELD] & 2) >> 1;
      pDebugPrintf((pLOGNOTICE, "RFTEST: Got Control %d\n", type));
      switch(type)
    {
    case PRH_RFCOMM_CC_TYPE_PN:
      dlci_recv=buffer->data[2];
      pDebugPrintf((pLOGNOTICE, "RFTEST: Got PN %d\n", dlci));
      if (cr_recv)
        rftest_sendpn(dlci_recv, 0, 127);
      //    prh_rfcomm_cc_process_pn(self_inst, buffer);
      break;
    case PRH_RFCOMM_CC_TYPE_TEST:      
      pDebugPrintf((pLOGNOTICE, "RFTEST: Got Test\n"));
      if (cr_recv==0)
        if (rftest_enabled==11)
          {
        rftest_passed();
          }
      //      prh_rfcomm_cc_process_test(self_inst,buffer);
      break;
    case PRH_RFCOMM_CC_TYPE_FCON:
      pDebugPrintf((pLOGNOTICE, "RFTEST: Got FCON\n"));
      //      prh_rfcomm_cc_process_fcon(self_inst, buffer);
      break;    
    case PRH_RFCOMM_CC_TYPE_FCOFF:
      pDebugPrintf((pLOGNOTICE, "RFTEST: Got FCOFF\n"));
      //      prh_rfcomm_cc_process_fcoff(self_inst, buffer);
      break;
    case PRH_RFCOMM_CC_TYPE_MSC:
      pDebugPrintf((pLOGNOTICE, "RFTEST: Got MSC\n"));
      if (cr_recv)
        {
          rftest_sendmsc_reply(dlci, 0, &buffer->data[2]);

        }
      //      prh_rfcomm_cc_process_msc(self_inst, buffer);
      break;
    case PRH_RFCOMM_CC_TYPE_NSC:
      pDebugPrintf((pLOGNOTICE, "RFTEST: Got NSC\n"));
      /* If I got a NSC what do I do? */
      break;
    case PRH_RFCOMM_CC_TYPE_RPN:
      if (cr_recv==0)
        if (rftest_enabled==17 || rftest_enabled==19)
          {
        rftest_passed();
          }
      pDebugPrintf((pLOGNOTICE, "RFTEST: Got RPN\n"));
      //      prh_rfcomm_cc_process_rpn(self_inst, buffer);
      break;
    case PRH_RFCOMM_CC_TYPE_RLS:
      if (cr_recv==0)
        if (rftest_enabled==13)
          {
        rftest_passed();
          }
      pDebugPrintf((pLOGNOTICE, "RFTEST: Got RLS\n"));
      //      prh_rfcomm_cc_process_rls(self_inst, buffer);
      break;
    default:
      /* transmit back an NSC */
      break;
    }
    }
  else
    {
      pDebugPrintf((pLOGNOTICE, "RFTEST: Got Data on dlci\n",dlci));
    }
    
  return BT_NOERROR;
  
}

#include <stdio.h>

int rftest_passed(void)
{
  //pDebugPrintf((pLOGNOTICE, "TEST: %d Test Passed\n", test_num));
  fprintf(stderr,"TEST: %d Test Passed\n", test_num);

  pDebugSleep(5);
  pExit(50);
}

int rftest_failed(void)
{
  // pDebugPrintf((pLOGNOTICE, "TEST: %d Test Failed\n", test_num));
  fprintf(stderr,"TEST: %d Test Failed\n", test_num);
  pDebugSleep(5);
  pExit(40);
}
