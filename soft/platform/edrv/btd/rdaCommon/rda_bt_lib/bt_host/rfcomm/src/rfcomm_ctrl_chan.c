/***********************************************************************
 *
 * MODULE NAME:    rfcomm_ctrl_chan.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    RFCOMM Control Channel Handler
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  15 March 2000
 *
 * SOURCE CONTROL: $Id: rfcomm_ctrl_chan.c 1532 2010-08-23 05:47:34Z huazeng $
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
#if COMBINED_HOST==1
#include "papi.h"
#endif

/* 
 * prh_rfcomm_cc_data_ind
 * -- Control Channel Data Indication Handler
 *
 * Receives control channel messages, deals with cancelling the timer,
 * and dispatches the messages to its respective handler function.
 */
t_api prh_rfcomm_cc_data_ind(prh_t_rfcomm_dlci dlci0, struct host_buf *buffer, void *multi_inst)
{
  u_int8 type;
  int ret;
  struct prh_rfcomm_l2cap_inst *self_inst=(struct prh_rfcomm_l2cap_inst *)multi_inst;
  struct prh_rfcomm_dlc_inst *dlc0_inst;
  struct host_buf *out_buf;
  u_int8 cr_recv;

  type=buffer->data[PRH_RFCOMM_CC_TYPE_FIELD] & 0xFC;
  /* get the Command/Response bit from received command */
  cr_recv=(buffer->data[PRH_RFCOMM_CC_TYPE_FIELD] & 2) >> 1;
  /* Cancel the timer */
  ret=prh_rfcomm_lookup_dlci_instance(self_inst, PRH_RFCOMM_DLCI0, &dlc0_inst);

  if (cr_recv==PRH_RFCOMM_CR_UNSET)
    {
      prh_rfcomm_cc_cancel_timer(dlc0_inst);
    }
  
  /* 
   * The following block could be implemented as a switch statement,
   * but on the ARM 30 bytes is saved by having it implemented as is.
   * Not the NSC case is not handled, if we receive an NSC we don't know
   * what we've done wrong to do anything about it so if we get an NSC
   * we ignore it and hope the other side deals with it.
   */
  if (type==PRH_RFCOMM_CC_TYPE_PN)
    prh_rfcomm_cc_process_pn(self_inst, buffer);
  else if (type==PRH_RFCOMM_CC_TYPE_TEST)
    prh_rfcomm_cc_process_test(self_inst, buffer);
  else if (type==PRH_RFCOMM_CC_TYPE_FCON)
    prh_rfcomm_cc_process_fcon(self_inst, buffer);
  else if (type==PRH_RFCOMM_CC_TYPE_FCOFF)
    prh_rfcomm_cc_process_fcoff(self_inst, buffer);
  else if (type==PRH_RFCOMM_CC_TYPE_MSC)
    prh_rfcomm_cc_process_msc(self_inst, buffer);
  else if (type==PRH_RFCOMM_CC_TYPE_RPN)
    prh_rfcomm_cc_process_rpn(self_inst, buffer);
  else if (type==PRH_RFCOMM_CC_TYPE_RLS)
    prh_rfcomm_cc_process_rls(self_inst, buffer);
  else if (type!=PRH_RFCOMM_CC_TYPE_NSC)
    {
      ret=PRH_RFCOMM_GET_L2_CONTROL_FRAME(self_inst->cid, PRH_RFCOMM_CC_NSC_LEN, &out_buf);
      if (ret!=BT_NOERROR)
	goto out;
      host_buf_reserve_header(out_buf, PRH_RFCOMM_BUF_CONTROL_HEADERS);

      prh_rfcomm_cc_create_nsc(out_buf, PRH_RFCOMM_CR_UNSET, type, cr_recv);
      prh_rfcomm_cc_transmit_cmesg(self_inst, out_buf, PRH_RFCOMM_CR_UNSET);       
    }
  ret=BT_NOERROR;
out:
  host_buf_free(buffer);
  return ret;
}

/* prh_rfcomm_cc_cancel_timer
 * -- Remove a timer
 *
 */
t_api prh_rfcomm_cc_cancel_timer(struct prh_rfcomm_dlc_inst *dlc0_inst)
{
#if (pDEBUG==0)
u_int8 temp[4];
#endif

  dlc0_inst->resp_count--;


#if pDEBUG
   pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"RFCOMM: resp_count is %d", dlc0_inst->resp_count));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)

    BT_WRITE_LE_UINT32(temp,dlc0_inst->resp_count);
    SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM, LOG_RFC_RFCOMM_CTRL_CHAN_INDEX, temp, 4,BT_CODE_DWORD);   
#endif

  if (dlc0_inst->resp_count==0)
    {
#if pDEBUG
      pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"RFCOMM: Cancelling PF timer\n"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
      SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM, (LOG_RFC_RFCOMM_CTRL_CHAN_INDEX+1), NULL,0,0);   
#endif
    if(dlc0_inst->pfbit_timer)
      pTimerCancel(dlc0_inst->pfbit_timer);
      dlc0_inst->pfbit_timer=0;
    }
  return BT_NOERROR;
}

/*
 * prh_rfcomm_cc_process_msc
 * -- Process an MSC command or response
 *
 * Decodes the MSC into a prh_t_rfcomm_control_params structure and indicates 
 * to the port entity with the contents, and sends a response to the remote 
 * device. Mainly used for flow control.
 */
t_api prh_rfcomm_cc_process_msc(struct prh_rfcomm_l2cap_inst *self, struct host_buf *frame)
{
  prh_t_rfcomm_control_params ctrl_params;
  struct host_buf *msc_resp;
  struct prh_rfcomm_dlc_inst *dlc_inst;
  int cresp;
  int ret;
  u_int8 break_byte;
  prh_t_rfcomm_dlci dlci;
  int ct_signals, dlc_exist;

#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
  u_int8 temp[2];
  #endif
  prh_t_rfcomm_cb_type cbtype=PRH_RFCOMM_CB_CFM;

  cresp=frame->data[PRH_RFCOMM_CC_TYPE_FIELD] & PRH_RFCOMM_CR_BIT;
  if (cresp==PRH_RFCOMM_CR_BIT)
    {
      /* Extract DLCI this MSC is for */
      dlci = frame->data[PRH_RFCOMM_CC_MSC_DLCI] >> 2;
      if (dlci==0)
	return BT_INVALIDPARAM;

      /* Check does this DLC exist */
      ret=prh_rfcomm_lookup_dlci_instance(self, dlci, &dlc_inst);
      if (ret!=BT_NOERROR)
    return BT_INVALIDPARAM;
      dlc_exist=ret;
      
      ct_signals = frame->data[PRH_RFCOMM_CC_MSC_BITS];
#if pDEBUG
      pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"RFCOMM: rx DLC%d:  MSC frame sig: %d\n", dlci, ct_signals));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
      temp[0]=dlci;
      temp[1]=(u_int8)ct_signals;
      SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM, (LOG_RFC_RFCOMM_CTRL_CHAN_INDEX+2), temp, 2,(BT_CODE_BYTE | (BT_CODE_BYTE<<4)));   
#endif
      
      if (ct_signals & PRH_RFCOMM_EA_BIT)
	{
	  ctrl_params.break_signal=0;     
	}
      else
	{
	  /* break byte is encoded */
	  break_byte=frame->data[PRH_RFCOMM_CC_MSC_BITS+1];
	  if (break_byte & PRH_RFCOMM_CC_MSC_BS_B1)
	    ctrl_params.break_signal=break_byte>>4;
	  else
	    ctrl_params.break_signal=0;
	}
      ctrl_params.modem_signal=0;
      
      /*
       * If the FC bit is set, stop data, unset-start data
       */
      /* If CREDIT FLOW is enabled, ignore the FC bit in the MSC
	 BT Spec RFCOMM F1:6.5.3 */
      if (self->credit_state==CREDIT_ON)
	ctrl_params.flow_state=PRH_RFCOMM_FLOW_DATA_GO;
      else
	if (ct_signals & PRH_RFCOMM_CC_MSC_CSF_FC)
    ctrl_params.flow_state=PRH_RFCOMM_FLOW_DATA_STOP;
      else
    ctrl_params.flow_state=PRH_RFCOMM_FLOW_DATA_GO;

      if (ct_signals & PRH_RFCOMM_CC_MSC_CSF_RTC)
    ctrl_params.modem_signal |= PRH_RFCOMM_MSC_DTRDSR;
      if (ct_signals & PRH_RFCOMM_CC_MSC_CSF_RTR)
    ctrl_params.modem_signal |= PRH_RFCOMM_MSC_RTSCTS;
      
      if (ct_signals & PRH_RFCOMM_CC_MSC_CSF_IC)
    ctrl_params.modem_signal |= PRH_RFCOMM_MSC_RI;
      if (ct_signals & PRH_RFCOMM_CC_MSC_CSF_DV)
    ctrl_params.modem_signal |= PRH_RFCOMM_MSC_DCD;
      
      if (dlc_inst->cb.control)
    (*dlc_inst->cb.control)(dlci, &ctrl_params, self, cbtype);
      
      ret=PRH_RFCOMM_GET_L2_CONTROL_FRAME(self->cid, PRH_RFCOMM_CC_MSC_LONG_LEN, &msc_resp);
      if (ret!=BT_NOERROR)
	return ret;
      host_buf_reserve_header(msc_resp, PRH_RFCOMM_BUF_CONTROL_HEADERS);

      prh_rfcomm_cc_create_msc(msc_resp, PRH_RFCOMM_CR_UNSET, dlci, &ctrl_params);
      prh_rfcomm_cc_transmit_cmesg(self, msc_resp, PRH_RFCOMM_CR_UNSET);
    }
  else
    {
    }
   return BT_NOERROR;
  
}

/*
 * prh_rfcomm_cc_process_pn
 * -- Process a Port Neg message from the peer entity.
 *
 * Extract PN parameters into a pn_info structure and either
 * queue the information for the DLC when it is opened or pass
 * it do it now.
 * Transmit a response.
 */
t_api prh_rfcomm_cc_process_pn(struct prh_rfcomm_l2cap_inst *self, struct host_buf *frame)
{
  /* Got a pn for a DLC need to set info in RFCOMM */
  struct host_buf *pn_resp;
  struct prh_rfcomm_dlc_inst *dlc_inst;
  struct prh_rfcomm_event_info info;
  int cresp;
  int ret;

  #if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))

  u_int8 temp;
  #endif
  prh_t_rfcomm_dlci dlci;
  prh_t_rfcomm_dlc_params dlc_params;

  u_int8 clbits;

  dlci=frame->data[PRH_RFCOMM_CC_PN_DLCI];
  if (dlci==0) /* illegal DLCI 0 */
    return BT_INVALIDPARAM;
  /* is this frame a command or response */
  cresp=frame->data[PRH_RFCOMM_CC_TYPE_FIELD] & PRH_RFCOMM_CR_BIT;
 
  dlc_params.max_frame_size=(frame->data[PRH_RFCOMM_CC_PN_FRAME_SIZE1] & 0xFF) + (frame->data[PRH_RFCOMM_CC_PN_FRAME_SIZE2] << 8);
  
  dlc_params.credit_flow_enabled=0;
  /* Was it a command or a response */
  if (cresp==PRH_RFCOMM_CR_BIT)
    {
      /* Command */
      /* We need to check do we want credit flow */
      if (self->credit_state!=CREDIT_OFF)
	{
	  /* Extract the CL / Kbits */
	  clbits=frame->data[PRH_RFCOMM_CC_PN_CLBITS]>>4;
	  if (clbits==PRH_RFCOMM_CC_PN_CREDIT_FLOW_REQ)
	    {
	      /* Remote device has requested Credit flow control v1.1 */
	      dlc_params.peer_credits=frame->data[PRH_RFCOMM_CC_PN_KBITS] & 0x7;
	
	      if (self->credit_state==CREDIT_REQUESTED || self->credit_state==CREDIT_NOTSELECTED || self->credit_state==CREDIT_ON)
		{
		  dlc_params.credit_flow_enabled=1;
		  self->credit_state=CREDIT_ON;
		}
	    }
	  else
	    {
	      if (self->credit_state==CREDIT_REQUESTED || self->credit_state==CREDIT_NOTSELECTED)
		self->credit_state=CREDIT_OFF;
	      /*else */
	      /* I think we should send a DM back to them */
	    }
	}
    }
  else
    {
      /* Need to deal with a PN response with 0 CL-bits from peer if 
	 the channel is established - UPF 5 .. ah feck ... will add to list */
      /* Response */
      if (self->credit_state!=CREDIT_OFF)
	{
	  clbits=frame->data[PRH_RFCOMM_CC_PN_CLBITS]>>4;
	  if (clbits==PRH_RFCOMM_CC_PN_CREDIT_FLOW_RESP)
	    {
	      dlc_params.peer_credits=frame->data[PRH_RFCOMM_CC_PN_KBITS] & 0x7;
	      /* if we have requested credit or not selected credit yet?? 
		 hmm should this not only be if we have requested it */
	      if (self->credit_state==CREDIT_REQUESTED || self->credit_state==CREDIT_NOTSELECTED)
		self->credit_state=CREDIT_ON;
	    }
	  else if (self->credit_state!=CREDIT_ON) 
	    self->credit_state=CREDIT_OFF;
	}
    }

  /* 
   * If a DLCI exists, notify it and send the req upwards
   * If no DLCI exists but the channel exists send the req upwards
   * If no DLCI or no channel exists send a DM response 
   */
  ret=prh_rfcomm_lookup_dlci_instance(self, dlci, &dlc_inst);
  if (ret==BT_NOERROR)
    {
      info.priv=(void *)&dlc_params;
      info.cb_valid=0;
      prh_rfcomm_dlc_do_event(dlc_inst, PRH_RFCOMM_DLC_PN, &info);
      
      if (dlc_inst->cb.dlc_paramneg)
	(*dlc_inst->cb.dlc_paramneg)(dlci, &dlc_params, self, cresp>>1);
      return BT_NOERROR;
    }
  else if (prh_rfcomm_check_channel_state((u_int8)(dlci>>1)))
    {
      struct prh_rfcomm_server_inst *serv_inst;
      
      ret=prh_rfcomm_lookup_server_inst_by_schan((u_int8)(dlci>>1), &serv_inst);
      if (ret==BT_NOERROR)
	{
	  if (serv_inst->dlc_cbs.dlc_paramneg)
	    (*serv_inst->dlc_cbs.dlc_paramneg)(dlci, &dlc_params, self, cresp>>1);
	  return BT_NOERROR;
	}
    }
  
  if (cresp==PRH_RFCOMM_CR_BIT)
    {
      /* Need to transmit a DM for the DLCI */
      ret=PRH_RFCOMM_GET_L2_FRAME(self->cid, PRH_RFCOMM_FR_BUFSIZE, &pn_resp);
      if (ret!=BT_NOERROR)
	return ret;
#if pDEBUG
      
    
      
      pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"RFCOMM: rx: PN DLC %d, tx: DM\n", dlci));
     
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
       temp=dlci;
       SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM, (LOG_RFC_RFCOMM_CTRL_CHAN_INDEX+3), &temp, 1,BT_CODE_BYTE);   
#endif
      prh_rfcomm_create_dm(pn_resp, dlci, PRH_RFCOMM_PF_SET, !(self->initiating));
      L2_Write(self->cid, (t_DataBuf *)pn_resp);
    }
  return BT_NOERROR;
}

/* 
 * prh_rfcomm_cc_process_test
 * 
 * Process the a test command from the remote side, create a reply buffer,
 * transfer contents into buffer, fill out header and send back to remote
 * entity 
 *
 */
t_api prh_rfcomm_cc_process_test(struct prh_rfcomm_l2cap_inst *self, struct host_buf *frame)
{
  /* Received a test command */
  /* Respond to remote device */
  struct host_buf *out_buf;
  u_int16 length;
  u_int8 sz_len;
  int ret;

  /* Check is the Command bit set in type field */
  if (!((frame->data[PRH_RFCOMM_CC_TYPE_FIELD] & (PRH_RFCOMM_CC_CR_COMM<<1)) == (PRH_RFCOMM_CC_CR_COMM<<1)))
    return BT_NOERROR;

  if (frame->data[PRH_RFCOMM_CC_LENGTH_FIELD] & PRH_RFCOMM_EA_BIT)
    {
      length=frame->data[PRH_RFCOMM_CC_LENGTH_FIELD]>>1;
      sz_len=1;
    }
  else
    {
      length=(frame->data[PRH_RFCOMM_CC_LENGTH_FIELD] >> 1) + ((frame->data[PRH_RFCOMM_CC_LENGTH_FIELD+1] >> 1) << 7);
      sz_len=2;
    }
  
  ret=PRH_RFCOMM_GET_L2_CONTROL_FRAME(self->cid, length, &out_buf);
  if (ret!=BT_NOERROR)
    return ret;
  host_buf_reserve_header(out_buf, PRH_RFCOMM_BUF_CONTROL_HEADERS+sz_len+1);

  pMemcpy(out_buf->data, &(frame->data[sz_len+1]), length);

  out_buf->len=length;
  
  /* test response */
  prh_rfcomm_cc_create_test(out_buf, PRH_RFCOMM_CR_UNSET);
  prh_rfcomm_cc_transmit_cmesg(self, out_buf, PRH_RFCOMM_CR_UNSET);
  
  return BT_NOERROR;
}

/*
 * prh_rfcomm_tfunc_flow_on_dlci
 * 
 * Linked List Traverse Function to flow on all DLCs
 */
int prh_rfcomm_tfunc_flow_dlci(void *data, void *func_info)
{
  struct prh_rfcomm_dlc_inst *inst=(struct prh_rfcomm_dlc_inst *)data;
  prh_t_rfcomm_flow_state flow_state=(prh_t_rfcomm_flow_state)(long)func_info;

  if (inst->dlci==0)
    return BT_NOERROR;

  if (inst->cb.agg_flow_ind)
    inst->cb.agg_flow_ind(inst->dlci, flow_state, inst->rfcomm_l2cap_ptr);
  return BT_NOERROR;
}

/*
 * prh_rfcomm_cc_process_fcon
 * 
 * Process an Aggregate Flow Control On from remote side 
 *
 * Callback every DLC on this L2CAP Channel with an agg flow ind.
 */
t_api prh_rfcomm_cc_process_fcon(struct prh_rfcomm_l2cap_inst *self, struct host_buf *frame)
{
  /* Need to send a Flow indication on ALL DLC's open on this multiplexer */
  struct host_buf *out_buf;
  int ret;

  if (!((frame->data[PRH_RFCOMM_CC_TYPE_FIELD] & (PRH_RFCOMM_CC_CR_COMM<<1)) == (PRH_RFCOMM_CC_CR_COMM<<1)))
    return 0;

  /* Errata submitted by me send NSC if FCON/FCOFF received in CBFC */
  
  if (self->credit_state==CREDIT_ON)
    {
      u_int8 cr_recv;

      cr_recv=(frame->data[PRH_RFCOMM_CC_TYPE_FIELD] & 2) >> 1;
      ret=PRH_RFCOMM_GET_L2_CONTROL_FRAME(self->cid, PRH_RFCOMM_CC_NSC_LEN, &out_buf);
      if (ret!=BT_NOERROR)
	return ret;
      host_buf_reserve_header(out_buf, PRH_RFCOMM_BUF_CONTROL_HEADERS);

      prh_rfcomm_cc_create_nsc(out_buf, PRH_RFCOMM_CR_UNSET, PRH_RFCOMM_CC_TYPE_FCON, cr_recv);
      prh_rfcomm_cc_transmit_cmesg(self, out_buf, PRH_RFCOMM_CR_UNSET);  
      return BT_NOERROR;
    }

  prh_host_ll_traverse(self->dlc_head, prh_rfcomm_tfunc_flow_dlci, (void *)PRH_RFCOMM_FLOW_DATA_GO);
  
  ret=PRH_RFCOMM_GET_L2_CONTROL_FRAME(self->cid, PRH_RFCOMM_CC_FCON_LEN, &out_buf);
  if (ret!=BT_NOERROR)
    return ret;
  host_buf_reserve_header(out_buf, PRH_RFCOMM_BUF_CONTROL_HEADERS);

  prh_rfcomm_cc_create_fcon(out_buf, PRH_RFCOMM_CR_UNSET);
  prh_rfcomm_cc_transmit_cmesg(self, out_buf, PRH_RFCOMM_CR_UNSET);
  return BT_NOERROR;
}

/* 
 * prh_rfcomm_cc_process_fcoff
 * -- Process an Aggregate Flow Control Off from remote side 
 *
 * Switch off flow control at L2CAP multiplexer level, 
 */
t_api prh_rfcomm_cc_process_fcoff(struct prh_rfcomm_l2cap_inst *self, struct host_buf *frame)
{
  struct host_buf *out_buf;
  int ret;

  /* Errata submitted by me send NSC if FCON/FCOFF received in CBFC */
  if (self->credit_state==CREDIT_ON)
    {
      u_int8 cr_recv;

      cr_recv=(frame->data[PRH_RFCOMM_CC_TYPE_FIELD] & 2) >> 1;
      ret=PRH_RFCOMM_GET_L2_CONTROL_FRAME(self->cid, PRH_RFCOMM_CC_NSC_LEN, &out_buf);
      if (ret!=BT_NOERROR)
	return ret;
      host_buf_reserve_header(out_buf, PRH_RFCOMM_BUF_CONTROL_HEADERS);

      prh_rfcomm_cc_create_nsc(out_buf, PRH_RFCOMM_CR_UNSET, PRH_RFCOMM_CC_TYPE_FCOFF, cr_recv);
      prh_rfcomm_cc_transmit_cmesg(self, out_buf, PRH_RFCOMM_CR_UNSET);  
      return BT_NOERROR;
    }

  if (!((frame->data[PRH_RFCOMM_CC_TYPE_FIELD] & (PRH_RFCOMM_CC_CR_COMM<<1)) == (PRH_RFCOMM_CC_CR_COMM<<1)))
    return 0;

  prh_host_ll_traverse(self->dlc_head, prh_rfcomm_tfunc_flow_dlci, (void *)PRH_RFCOMM_FLOW_DATA_STOP);

  ret=PRH_RFCOMM_GET_L2_CONTROL_FRAME(self->cid, PRH_RFCOMM_CC_FCOFF_LEN, &out_buf);
  if (ret!=BT_NOERROR)
    return ret;
  host_buf_reserve_header(out_buf, PRH_RFCOMM_BUF_CONTROL_HEADERS);

  prh_rfcomm_cc_create_fcoff(out_buf, PRH_RFCOMM_CR_UNSET);
  prh_rfcomm_cc_transmit_cmesg(self, out_buf, PRH_RFCOMM_CR_UNSET);
  return BT_NOERROR;
}

/*
 * prh_rfcomm_cc_process_rls
 * -- Process a Remote Line Status 
 * 
 * Decodes the remote line status into a line_stat_params structure
 * Callbacks the structure to the port entity
 *
 * The port entity needs to transmit the response
 */
 
t_api prh_rfcomm_cc_process_rls(struct prh_rfcomm_l2cap_inst *self, struct host_buf *frame)
{
  struct host_buf *out_buf;
  prh_t_rfcomm_dlci dlci;
  prh_t_rfcomm_line_stat_params ls_params;
  int ret;
  struct prh_rfcomm_dlc_inst *dlc_inst;
  prh_t_rfcomm_cb_type cbtype;

  dlci=frame->data[PRH_RFCOMM_CC_LSR_DLCI]>>2;
  if (dlci==0)
    return BT_INVALIDPARAM;
  ls_params=frame->data[PRH_RFCOMM_CC_LSR_DLCI+1];
  ret=prh_rfcomm_lookup_dlci_instance(self, dlci, &dlc_inst);
  if (ret==BT_NOERROR)
    {
      cbtype=(frame->data[PRH_RFCOMM_CC_TYPE_FIELD] & (PRH_RFCOMM_CC_CR_COMM<<1)) >> 1;
      if (dlc_inst->cb.linestat)
	(*dlc_inst->cb.linestat)(dlci, ls_params, self, cbtype);
      else
    {
      return BT_INVALIDPARAM;
    }

      /* If this is a command */
      if ((frame->data[PRH_RFCOMM_CC_TYPE_FIELD] & (PRH_RFCOMM_CC_CR_COMM<<1)) == (PRH_RFCOMM_CC_CR_COMM<<1))
    {
      ret=PRH_RFCOMM_GET_L2_CONTROL_FRAME(self->cid, PRH_RFCOMM_CC_LSR_LEN, &out_buf);
      if (ret!=BT_NOERROR)
	return ret;
      host_buf_reserve_header(out_buf, PRH_RFCOMM_BUF_CONTROL_HEADERS);

      prh_rfcomm_cc_create_rls(out_buf, PRH_RFCOMM_CR_UNSET, dlci, ls_params);
      prh_rfcomm_cc_transmit_cmesg(self, out_buf, PRH_RFCOMM_CR_UNSET);
      
    }

    }
  else
    return BT_INVALIDPARAM;
  return BT_NOERROR;
}

/*
 * prh_rfcomm_cc_process_rpn
 * -- Process a Remote Port Negotiation
 *
 * Decode the parameters into an rfcomm_port_params struct
 * Callback the port entity
 * RPN responses should be done by the port entity
 * ISSUE: need to decode mask bits 
 */
t_api prh_rfcomm_cc_process_rpn(struct prh_rfcomm_l2cap_inst *self, struct host_buf *frame)
{
  int ret;
  u_int8 rx_len, param_set;
  prh_t_rfcomm_dlci dlci;
  prh_t_rfcomm_port_params port_params;
  struct prh_rfcomm_dlc_inst *dlc_inst;
  struct host_buf *buffer;

  #if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
  u_int8 temp;
  #endif
  

  prh_t_rfcomm_cb_type cbtype;

  dlci=frame->data[PRH_RFCOMM_CC_RPN_DLCI]>>2;
  if (dlci==0)
    return BT_INVALIDPARAM;

  /* Need to decode what has been requested */
  rx_len=frame->data[PRH_RFCOMM_CC_LENGTH_FIELD]>>1;
  if (rx_len==PRH_RFCOMM_CC_RPN_LEN_REQ)
    {
      param_set=0;
    }
  else
    {
      /* Decode the port parameters */
      port_params.port_speed=frame->data[PRH_RFCOMM_CC_RPN_BAUD];
      port_params.data_bits=frame->data[PRH_RFCOMM_CC_RPN_DSPB] & 0x3;
      port_params.stop_bits=(frame->data[PRH_RFCOMM_CC_RPN_DSPB] & 0x4) >> 2;
      port_params.parity_enable=(frame->data[PRH_RFCOMM_CC_RPN_DSPB] & 0x8) >> 3;
      if (port_params.parity_enable)
	{
	  port_params.paritytype=(frame->data[PRH_RFCOMM_CC_RPN_DSPB] & 0x30) >> 4;
	}
      else
	port_params.paritytype=RFODD;

      port_params.flow_mask = frame->data[PRH_RFCOMM_CC_RPN_FLC] & 0x3f;
      port_params.xon = frame->data[PRH_RFCOMM_CC_RPN_XON];
      port_params.xoff = frame->data[PRH_RFCOMM_CC_RPN_XOFF];
      port_params.param_mask = (frame->data[PRH_RFCOMM_CC_RPN_PM1] + (frame->data[PRH_RFCOMM_CC_RPN_PM2] << 8));
      
      param_set=1;
    }
  
  /*
   */

  cbtype=(frame->data[PRH_RFCOMM_CC_TYPE_FIELD] & (PRH_RFCOMM_CC_CR_COMM<<1)) >> 1;

  /* Transmit this up to the port entity if
   *  a) There is an open DLC
   *  b) the server channel is registered locally and this is a command
   * else send back a DM response
   */
  
  ret=prh_rfcomm_lookup_dlci_instance(self, dlci, &dlc_inst);
  if (ret==BT_NOERROR)
    {
      if (dlc_inst->cb.portneg)
	{
	  if (param_set)
	    (*dlc_inst->cb.portneg)(dlci, &port_params, self, cbtype);
	  else
	    (*dlc_inst->cb.portneg)(dlci, NULL, self, cbtype);
	}
    }
  else
    {
      /* Is this a command or response? */
      if (cbtype)
	{
	  /* If this is a command is the channel open? */
	  if (prh_rfcomm_check_channel_state((u_int8)(dlci>>1)))
	    {
	      struct prh_rfcomm_server_inst *serv_inst;
	      
	      ret=prh_rfcomm_lookup_server_inst_by_schan((u_int8)(dlci>>1), &serv_inst);
	      if (ret==BT_NOERROR)
		{
		  if (serv_inst->dlc_cbs.portneg)
		    {
		      if (param_set)
			(*serv_inst->dlc_cbs.portneg)(dlci, &port_params, self, cbtype);
		      else
			(*serv_inst->dlc_cbs.portneg)(dlci, NULL, self, cbtype);
		    }
		  return BT_NOERROR;
		}
	    }

	  /* Need to transmit a DM for the DLCI */
	  ret=PRH_RFCOMM_GET_L2_FRAME(self->cid, PRH_RFCOMM_FR_BUFSIZE, &buffer);
	  if (ret!=BT_NOERROR)
	    return ret;
#if pDEBUG
	  pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"RFCOMM: rx: PN DLC %d, tx: DM\n", dlci));

#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
         temp=dlci;
         SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM, (LOG_RFC_RFCOMM_CTRL_CHAN_INDEX+3), &temp, 1,BT_CODE_BYTE); 
#endif
	  prh_rfcomm_create_dm(buffer, dlci, PRH_RFCOMM_PF_SET, !(self->initiating));
	  L2_Write(self->cid, (t_DataBuf *)buffer);
	      
	 
	}
    }

  return BT_NOERROR;
}

/* 
 * prh_rfcomm_cc_timeout_cb
 * -- Callback used when a Control message timeout occurs
 *
 * This function is called back when a control message reply is not 
 * received within N2 secs. (60).
 */
void prh_rfcomm_cc_timeout_cb(void *param)
{
  struct prh_rfcomm_dlc_inst *dlc_inst=(struct prh_rfcomm_dlc_inst *)param;
  
  /* TBC : Timer needs more thought */

#if pDEBUG
  pDebugPrintfEX((pLOGFATAL,pLOGRFCOMM,"RFCOMM: Contrl Chan Timed out occured"));

#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
  SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM, (LOG_RFC_RFCOMM_CTRL_CHAN_INDEX+4), NULL,0,0); 
#endif
  dlc_inst->pfbit_timer=0;
  prh_rfcomm_dlc_do_event(dlc_inst, PRH_RFCOMM_TIMEOUT, NULL);
  return;

}

/*
 * prh_rfcomm_cc_transmit_cmesg
 * -- Transmit a control channel message
 *
 * Transmit a message on the control channel .. all message on DLCI0
 * should pass through this function. Deals with setting the control 
 * channel message reply timer.
 */
t_api prh_rfcomm_cc_transmit_cmesg(struct prh_rfcomm_l2cap_inst *self, struct host_buf *buffer, u_int8 cresp)
{
  struct prh_rfcomm_dlc_inst *dlc0_inst;
  struct prh_rfcomm_event_info info;
  int ret;

  #if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
  u_int8 temp[4];
  #endif

  ret=prh_rfcomm_lookup_dlci_instance(self, PRH_RFCOMM_DLCI0, &dlc0_inst);
  if (ret!=BT_NOERROR)
    {
      /* Can't lookup DLCI 0 scary stuff happening */
      return BT_INVALIDPARAM;
    }
  info.buffer=buffer;

  /* Only set the timer if we are transmitting a command packet */
  /* if we are already expecting a response to a command */
  if (cresp==PRH_RFCOMM_CR_SET)
    {
      if (dlc0_inst->resp_count>0)
    {
      /* remove the currently running timer and re-add */
      #if pDEBUG
      //pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"RFCOMM: Cancelling PF timer\n"));
      #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
      SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM, (LOG_RFC_RFCOMM_CTRL_CHAN_INDEX+5), NULL,0,0);
      #endif
     if(dlc0_inst->pfbit_timer)
      pTimerCancel(dlc0_inst->pfbit_timer);
      dlc0_inst->pfbit_timer=0;
    }
      /* set the timer for the pfbit */
      #if pDEBUG
     // pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"RFCOMM: Setting PF timer\n"));
      #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
      SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM, (LOG_RFC_RFCOMM_CTRL_CHAN_INDEX+6), NULL,0,0); 
      #endif
      dlc0_inst->pfbit_timer=pTimerCreate(TIMER_RESP_T2, prh_rfcomm_cc_timeout_cb, (void *)dlc0_inst, pTIMER_ONESHOT); 
      dlc0_inst->resp_count++;
      #if pDEBUG
      //pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"RFCOMM: resp_count is %d", dlc0_inst->resp_count));
      #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
      BT_WRITE_LE_UINT32(temp,dlc0_inst->resp_count);      
      SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM, LOG_RFC_RFCOMM_CTRL_CHAN_INDEX, temp,4,BT_CODE_DWORD); 
      #endif
    }
  prh_rfcomm_dlc_do_event(dlc0_inst, PRH_RFCOMM_DATA_REQ, &info);
  return BT_NOERROR;
}
