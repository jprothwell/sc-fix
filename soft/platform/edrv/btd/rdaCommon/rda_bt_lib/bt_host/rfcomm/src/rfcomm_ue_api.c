/***********************************************************************
 *
 * MODULE NAME:    rfcomm_ue_api.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    RFCOMM Upper Edge API 
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  15 March 2000
 *
 * SOURCE CONTROL: $Id: rfcomm_ue_api.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Inc.
 *     All rights reserved.
 *
 ***********************************************************************/

#include "rfcomm_lower_layer.h"
#include "rfcomm_frame.h"

#include "rfcomm_ue_api.h"
#include "rfcomm_core.h"
#include "rfcomm_proto.h"
#include "rfcomm_debug.h"


t_api PRH_RFCOMM_UE_Allocate_Server_Channel(prh_t_rfcomm_schan *schan_ptr)
{
  int i=1;
  int ret=BT_NOERROR;
  prh_t_rfcomm_schan schan=0;
  
  pMutexLock(prh_rfcomm_core_lock);

  while(i<=PRH_RFCOMM_MAX_SCHAN && !schan)
    {
      if (!((prh_rfcomm_core->channel_allocation_state & 1U<<i) == 1U<<i))
	schan=i;
      else
	i++;
    }
  if (i<=PRH_RFCOMM_MAX_SCHAN)
    {
      *schan_ptr=schan;
      prh_rfcomm_core->channel_allocation_state |= 1<<schan;
      goto out;
    }
  else
    {
      *schan_ptr=0;
      ret=BT_NORESOURCES;
    }
out:
  pMutexUnlock(prh_rfcomm_core_lock);
  return ret;
}

/*
 * PRH_RFCOMM_UE_Deallocate_Server_Channel
 * -- Upper Edge API to deallocate a previously allocated server channel
 *
 */
t_api PRH_RFCOMM_UE_Deallocate_Server_Channel(prh_t_rfcomm_schan schan)
{
  if (schan==0 || schan>PRH_RFCOMM_MAX_SCHAN)
    return BT_INVALIDPARAM;
    
  pMutexLock(prh_rfcomm_core_lock);
  prh_rfcomm_core->channel_allocation_state &= ~(1<<schan);
  pMutexUnlock(prh_rfcomm_core_lock);
  return BT_NOERROR;
}

/* 
 * PRH_RFCOMM_UE_Register_Server
 * -- Register a Server application (port or system entity) with RFCOMM
 * 
 */

#if COMBINED_HOST==1
 struct prh_rfcomm_server_inst _serv_inst[7];
int prh_num=0;
#endif
t_api PRH_RFCOMM_UE_Register_Server(prh_t_rfcomm_schan schan, struct prh_rfcomm_dlc_cb *dlc_cbs)
{
#if COMBINED_HOST==1
  struct prh_rfcomm_server_inst *serv_inst = &_serv_inst[prh_num];
#else
  struct prh_rfcomm_server_inst *serv_inst;
#endif
  int ret;
  
  pMutexLock(prh_rfcomm_core_lock);

  /* user should have asked for channel with Allocate_Server_Channel */
  
  if (schan==0 || schan>PRH_RFCOMM_MAX_SCHAN)
    {
      ret=BT_INVALIDPARAM;
      goto out;
    }

  if ((prh_rfcomm_core->channel_registered_state & (1U<<schan)) == (1U<<schan))
    {
      ret=BT_INVALIDPARAM;
      goto out;
    }
#if COMBINED_HOST==0

  serv_inst=(struct prh_rfcomm_server_inst *)pMalloc(sizeof(struct prh_rfcomm_server_inst));
  if (serv_inst==pNULL)
    {
      ret=BT_NORESOURCES;
      goto out;
    }
#endif
  serv_inst->server_chan=schan;
  serv_inst->dlc_cbs=*dlc_cbs;
  serv_inst->lock=pMutexCreate(FALSE);

  prh_rfcomm_core->channel_registered_state |= 1<<schan;
  ret=prh_host_ll_add(&(prh_rfcomm_core->server_list), (void *)serv_inst);
  if (ret!=BT_NOERROR)
    {
      prh_rfcomm_core->channel_registered_state &= ~(1<<schan);
      pMutexFree(serv_inst->lock);
      pFree(serv_inst);
      ret=BT_NORESOURCES;
      goto out;
    }
  /* reaffirm allocation of channel */
  prh_rfcomm_core->channel_allocation_state |= 1<<schan;
  ret=BT_NOERROR;

#if COMBINED_HOST==1
  prh_num++;
#endif
out:

  pMutexUnlock(prh_rfcomm_core_lock);
  return ret;
}

/*
 * PRH_RFCOMM_UE_Deregister_Server
 * -- Remove a registered server application from RFCOMM 
 */
t_api PRH_RFCOMM_UE_Deregister_Server(prh_t_rfcomm_schan schan)
{
  struct prh_rfcomm_server_inst *serv_inst;
  int ret;

  PRH_RFCOMM_UE_Set_Channel_Closed(schan);
  PRH_RFCOMM_UE_Deallocate_Server_Channel(schan);
  ret=prh_rfcomm_remove_server_inst_by_schan(schan, &serv_inst);

  pMutexLock(serv_inst->lock);
  pMutexUnlock(serv_inst->lock);

  pMutexFree(serv_inst->lock);
  pFree(serv_inst);

  return BT_NOERROR;
}

/*
 * PRH_RFCOMM_UE_Set_Channel_Open
 * -- Upper Edge API set a channel state to open
 *
 * unused function is part of RegisterServer
 */

/*
 * PRH_RFCOMM_UE_Set_Channel_Closed
 * -- Upper Edge API set a channel state to closed
 *
 */
t_api PRH_RFCOMM_UE_Set_Channel_Closed(prh_t_rfcomm_schan schan)
{
  pMutexLock(prh_rfcomm_core_lock);
  prh_rfcomm_core->channel_registered_state &= ~(1<<schan);
  pMutexUnlock(prh_rfcomm_core_lock);
  return BT_NOERROR;
}

/*
 * PRH_RFCOMM_UE_Get_Channel_State
 * -- Get State of a Channel
 *
 * Returns BT_NOERROR if channel is registered, otherwise returns BT_INVALIDPARAM
 */
t_api PRH_RFCOMM_UE_Get_Channel_State(prh_t_rfcomm_schan schan)
{
  int ret=BT_NOERROR;
  pMutexLock(prh_rfcomm_core_lock);
  if (!(prh_rfcomm_core->channel_registered_state & (1<<schan)))
    ret=BT_INVALIDPARAM;

  pMutexUnlock(prh_rfcomm_core_lock);
  return ret;
}

/*
 * PRH_RFCOMM_UE_Start_Req
 * -- Upper Edge API Start Request
 *
 * Connect an RFCOMM Multiplexer session using the supplied parameters to
 * the supplied bd address and return a handle to it
 */
t_api PRH_RFCOMM_UE_Start_Req(t_bdaddr bd_addr, prh_t_rfcomm_start_sys_params *system_parameters, struct prh_rfcomm_multi_cb *callbacks, void **multi_inst)
{
  struct prh_rfcomm_l2cap_inst *l2cap_inst;

  struct prh_rfcomm_event_info info;
  t_api ret;

  /* A start request to initiate a connection to the remote RFCOMM entity on bd_addr bd_addr */
  /* do an prh_rfcomm_l2cap_init here to create a new RFCOMM instance,     if one already exists prh_rfcomm_l2cap_init should return that one */

  /* Start a new RFCOMM L2CAP instance or get a handle to one with
   * this bd_addr all ready.
   */
  /* N.B. this creates the l2cap instance locked */
  ret=prh_rfcomm_l2cap_init(&l2cap_inst, bd_addr);
  
  if (ret==BT_NORESOURCES)
    return BT_NORESOURCES;
  
  if (ret==BT_INVALIDPARAM)
    {
      info.bd_addr=bd_addr;
      info.priv=(void *)system_parameters;
      
      *multi_inst=l2cap_inst;
      info.cb.multi=*callbacks;
      l2cap_inst->sys_params=*system_parameters;

      /* Send PRH_RFCOMM_RFSTART_Request to the RFCOMM */
      ret=prh_rfcomm_l2cap_do_event(l2cap_inst, PRH_RFCOMM_RFSTART_REQ, &info);
      
      /* need to unlock the l2cap mutex */
      pMutexUnlock(l2cap_inst->lock);
      
      if (ret!=BT_PENDING && ret != BT_NOERROR)
			prh_rfcomm_remove_l2cap_inst(l2cap_inst, 0);
    }
  return ret;
}

/*
 * PRH_RFCOMM_UE_Start_Resp
 * -- Respond to a start multiplexer request.
 * 
 * Respond to a multiplexer start indication for the specified bd address, 
 * with the specified parameters
 */
t_api PRH_RFCOMM_UE_Start_Resp(t_bdaddr bd_addr, prh_t_rfcomm_start_sys_params *system_parameters, prh_t_rfcomm_accept accept,  struct prh_rfcomm_multi_cb *callbacks, void *multi_inst)
{
  struct prh_rfcomm_event_info info;
  struct prh_rfcomm_l2cap_inst *l2cap_inst=(struct prh_rfcomm_l2cap_inst *)multi_inst;
  struct prh_rfcomm_dlc_inst *dlc_inst;
  int ret;

  ret=prh_rfcomm_validate_and_lock_l2cap_inst_by_ptr(l2cap_inst);
  if (ret==BT_NOERROR)
    {
      ret=prh_rfcomm_lookup_dlci_instance_and_lock(l2cap_inst, PRH_RFCOMM_DLCI0, &dlc_inst);
      pMutexUnlock(l2cap_inst->lock);
      if (ret==BT_NOERROR)
	{
	  info.cb.multi=*callbacks;
	  info.accept=accept;
	  info.priv=NULL;
	  /* unlock happens internally */
	  prh_rfcomm_dlc_do_event(dlc_inst, PRH_RFCOMM_RFSTART_RESP, &info);
	}
      else
	return BT_UNKNOWNERROR;
    }

  return ret;

}

/*
 * PRH_RFCOMM_UE_register_rfstart_ind_cb
 * -- Register a callback to receive multiplexer start indiciations
 */
t_api PRH_RFCOMM_UE_register_rfstart_ind_cb(int (*PRH_RFCOMM_UE_Start_Ind_cb)(t_bdaddr, prh_t_rfcomm_start_sys_params *, prh_t_rfcomm_accept, void *))
{
  prh_rfcomm_core->rfstart_ind_cb=PRH_RFCOMM_UE_Start_Ind_cb; 
  return BT_NOERROR;
}

/*
 * PRH_RFCOMM_UE_DLC_Estab_Req
 * -- Establish a DLC
 *
 * Establish a DLC to remote bd address using the DLCI and supplied parameters
 */
t_api PRH_RFCOMM_UE_DLC_Estab_Req(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, struct prh_rfcomm_dlc_cb *callbacks, void *multi_inst)
{
  struct prh_rfcomm_l2cap_inst *l2cap_inst=(struct prh_rfcomm_l2cap_inst *)multi_inst;
  
  struct prh_rfcomm_event_info info;
  int ret;
  struct prh_rfcomm_dlc_inst *dlc_inst;

  ret=prh_rfcomm_validate_and_lock_l2cap_inst_by_ptr(l2cap_inst);
  if (ret==BT_NOERROR)
    {
      ret=prh_rfcomm_lookup_dlci_instance_and_lock(multi_inst, dlci, &dlc_inst);
      if (ret==BT_INVALIDPARAM)
	{
	  info.bd_addr=bd_addr;
	  info.dlci=dlci;
	  
	  /* If the upper layer requests a max frame size larger than the L2CAP MTU,
	     make it the size of the L2CAP enforced frame size. */
	  if (dlc_params->max_frame_size>l2cap_inst->sys_params.max_frame_size)
	    {
	      dlc_params->max_frame_size=l2cap_inst->sys_params.max_frame_size;
	    }
	  info.priv=(void *)dlc_params;
	  
	  info.cb.dlc=*callbacks;
	  info.cb_valid=1;
	  
	  /* Send PRH_RFCOMM_DLC_ESTAB_REQ_Request to the RFCOMM L2CAP Handler*/
	  prh_rfcomm_l2cap_do_event(l2cap_inst, PRH_RFCOMM_DLC_ESTAB_REQ, &info);

	  return BT_NOERROR;
	}
      else
	pMutexUnlock(dlc_inst->lock);
      pMutexUnlock(l2cap_inst->lock);
    }
  return BT_INVALIDPARAM;
}

/*
 * PRH_RFCOMM_UE_DLC_Estab_Resp
 * -- Respond to a DLC Establish Indication
 * 
 * Send a DLC Establish response as a reply to a DLC establish indication.
 */
t_api PRH_RFCOMM_UE_DLC_Estab_Resp(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, prh_t_rfcomm_accept accept, void *multi_inst)
{
  struct prh_rfcomm_event_info info;
  struct prh_rfcomm_l2cap_inst *l2cap_inst=(struct prh_rfcomm_l2cap_inst *)multi_inst;
  struct prh_rfcomm_dlc_inst *dlc_inst;
  struct prh_rfcomm_server_inst *server_inst;
  int ret=BT_NOERROR;
  
  ret=prh_rfcomm_validate_and_lock_l2cap_inst_by_ptr(l2cap_inst);
  if (ret==BT_NOERROR)
    {
      ret=prh_rfcomm_lookup_dlci_instance_and_lock(l2cap_inst, dlci, &dlc_inst);
      pMutexUnlock(l2cap_inst->lock);
      if (ret==BT_NOERROR)
	{
	  ret=prh_rfcomm_lookup_server_inst_by_schan((prh_t_rfcomm_schan)(dlci>>1), &server_inst);
	  if (ret==BT_NOERROR)
	    {
	      info.priv=(void *)dlc_params;
	      info.accept=accept;
	      info.cb.dlc=server_inst->dlc_cbs;
	      info.cb_valid=1;
	      /* unlocked in do_event */
	      prh_rfcomm_dlc_do_event(dlc_inst, PRH_RFCOMM_DLC_ESTAB_RESP, &info);
	    }
	  else
	    {
	      pMutexUnlock(dlc_inst->lock);
	      return BT_INVALIDPARAM;
	    }
	}
      else
	return BT_INVALIDPARAM;
    }
  return ret;
}

/*
 * PRH_RFCOMM_UE_DLC_Rel_Req
 * -- Send a DLC Release Request
 *
 * Transmit a DLC release request for the DLC on DLCI dlci
 */
t_api PRH_RFCOMM_UE_DLC_Rel_Req(prh_t_rfcomm_dlci dlci, void *multi_inst)
{
  struct prh_rfcomm_l2cap_inst *l2cap_inst=(struct prh_rfcomm_l2cap_inst *)multi_inst;
  struct prh_rfcomm_dlc_inst *dlc_inst;
  struct prh_rfcomm_event_info info;

  int ret;
  ret=prh_rfcomm_validate_and_lock_l2cap_inst_by_ptr(l2cap_inst);
  if (ret==BT_NOERROR)
    {
      info.dlci=dlci;
      
      ret=prh_rfcomm_lookup_dlci_instance_and_lock(l2cap_inst, dlci, &dlc_inst);
      pMutexUnlock(l2cap_inst->lock);
      if (ret==BT_NOERROR)
	{
	  ret=prh_rfcomm_dlc_do_event(dlc_inst, PRH_RFCOMM_DLC_RELEASE_REQ, &info);
	  pMutexUnlock(dlc_inst->lock);
	}
    }
  return ret;
}

/*
 * PRH_RFCOMM_UE_Close_Req
 * -- Close the multiplexer represented by multi_inst handle
 */
t_api PRH_RFCOMM_UE_Close_Req(void *multi_inst)
{
  struct prh_rfcomm_l2cap_inst *l2cap_inst=(struct prh_rfcomm_l2cap_inst *)multi_inst;

  int ret;
  ret=prh_rfcomm_validate_and_lock_l2cap_inst_by_ptr(l2cap_inst);
  if (ret==BT_NOERROR)
  {
    prh_rfcomm_l2cap_do_event(l2cap_inst, PRH_RFCOMM_RFCLOSE_REQ, NULL);
  }
  pMutexUnlock(l2cap_inst->lock);
  return BT_NOERROR;
}

/*
 * PRH_RFCOMM_UE_Test_Req
 * -- Transmit a test frame on the specified multiplexer handle multi_inst
 * 
 * The data in host_buf frame is sent as the test pattern.
 */
t_api PRH_RFCOMM_UE_Test_Req(struct host_buf *frame, void *multi_inst)
{
  struct prh_rfcomm_l2cap_inst *l2cap_inst=(struct prh_rfcomm_l2cap_inst *)multi_inst;
  int ret;

  ret=prh_rfcomm_validate_and_lock_l2cap_inst_by_ptr(l2cap_inst);
  if (ret==BT_NOERROR)
  {
    /* Test Command */
    prh_rfcomm_cc_create_test(frame, PRH_RFCOMM_CR_SET);
    ret=prh_rfcomm_dlci0_lock_and_transmit_cmesg(multi_inst, frame, PRH_RFCOMM_CR_SET);
    if (ret!=BT_NOERROR)
      prh_rfcomm_cc_destroy_test(frame);
  }
  return ret;
}

/*
 * PRH_RFCOMM_UE_Flow_Req
 * -- Send a aggregate flow message on the specified multiplexer multi_inst.
 * 
 * The flow state is either stop or go.
 */
t_api PRH_RFCOMM_UE_Flow_Req(prh_t_rfcomm_flow_state state, void *multi_inst)
{
  int ret;
  struct host_buf *buffer;
  struct prh_rfcomm_l2cap_inst *l2cap_inst= (struct prh_rfcomm_l2cap_inst *)multi_inst;

  ret=prh_rfcomm_validate_and_lock_l2cap_inst_by_ptr(l2cap_inst);
  if (ret==BT_NOERROR)
    {
      if (l2cap_inst->credit_state!=CREDIT_OFF && l2cap_inst->credit_state!=CREDIT_NOTSELECTED)
	{
	  ret=BT_INVALIDPARAM;
	  goto unlock_out;
	}
      
      /* allocate a buffer as a control frame */
      ret=PRH_RFCOMM_GET_L2_CONTROL_FRAME(l2cap_inst->cid, PRH_RFCOMM_CC_FCON_LEN, &buffer);
      if (ret!=BT_NOERROR)
	goto unlock_out;

      host_buf_reserve_header(buffer, PRH_RFCOMM_BUF_CONTROL_HEADERS);
      
      if (state==PRH_RFCOMM_FLOW_DATA_GO)
	prh_rfcomm_cc_create_fcon(buffer, PRH_RFCOMM_CR_SET);
      else
	prh_rfcomm_cc_create_fcoff(buffer, PRH_RFCOMM_CR_SET);
      
      ret=prh_rfcomm_dlci0_lock_and_transmit_cmesg(multi_inst, buffer, PRH_RFCOMM_CR_SET);
      if (ret!=BT_NOERROR)
	{
	  prh_l2_FreeWriteBuffer(buffer);
	  ret=BT_UNKNOWNERROR;
	  goto out;
	}
      else
	ret=BT_NOERROR;
    }
  else
    ret=BT_INVALIDPARAM;

  goto out;
unlock_out:
  pMutexUnlock(l2cap_inst->lock);
 out:
  return ret;
}

/*
 * PRH_RFCOMM_UE_Data_Req
 * -- Send data on the specified DLC
 *
 * Transmit data on the DLC dlci on multiplexer multi_inst
 */
t_api PRH_RFCOMM_UE_Data_Req(prh_t_rfcomm_dlci dlci, struct host_buf *buffer, void *multi_inst)
{
  struct prh_rfcomm_dlc_inst *dlc_inst;
  struct prh_rfcomm_event_info info;
  int ret;
  struct prh_rfcomm_l2cap_inst *l2cap_inst= (struct prh_rfcomm_l2cap_inst *)multi_inst;

  ret=prh_rfcomm_validate_and_lock_l2cap_inst_by_ptr(l2cap_inst);
  if (ret==BT_NOERROR)
    {
      ret=prh_rfcomm_lookup_dlci_instance_and_lock(multi_inst, dlci, &dlc_inst);
      pMutexUnlock(l2cap_inst->lock);
      if(ret==BT_NOERROR)
	{
	  info.buffer=buffer;
	  ret=prh_rfcomm_dlc_do_event(dlc_inst, PRH_RFCOMM_DATA_REQ, &info);
	  pMutexUnlock(dlc_inst->lock);
	  if (ret==BT_FLOWCTRL)
	    {
	      return BT_FLOWCTRL;
	    }
	}
      return BT_NOERROR;
    }
  return ret;
}

/* 
 * PRH_RFCOMM_UE_Portneg_Req
 * -- Send an RPN on the DLC specified.
 *
 * if port_parameters is NULL an RPN Request is sent
 * if port_parameters is specified an RPN Command is sent
 */
t_api PRH_RFCOMM_UE_Portneg_Req(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_port_params *port_parameters, void *multi_inst)
{
  struct host_buf *buffer;
  int ret;
  struct prh_rfcomm_l2cap_inst *l2cap_inst= (struct prh_rfcomm_l2cap_inst *)multi_inst;

  ret=prh_rfcomm_validate_and_lock_l2cap_inst_by_ptr(l2cap_inst);
  if (ret==BT_NOERROR)
    {
      ret=PRH_RFCOMM_GET_L2_CONTROL_FRAME(l2cap_inst->cid, PRH_RFCOMM_CC_RPN_LEN_COMM, &buffer);
      if (ret!=BT_NOERROR)
	{
	  pMutexUnlock(l2cap_inst->lock);
	  return ret;
	}
      host_buf_reserve_header(buffer, PRH_RFCOMM_BUF_CONTROL_HEADERS);
      
      /* Sending an RPN prior to DLC existance is fine */
#if pDEBUG
      pDebugPrintfEX((pLOGDEBUG, pLOGRFCOMM,"RFCOMM: tx: CC RPN\n"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
      SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM,LOG_RFC_RFCOMM_UE_API_INDEX,NULL,0,0);
#endif
      if (port_parameters)
	prh_rfcomm_cc_create_rpn_comm(buffer, PRH_RFCOMM_CR_SET, dlci, port_parameters);
      else
	prh_rfcomm_cc_create_rpn_req(buffer, PRH_RFCOMM_CR_SET, dlci);
      ret=prh_rfcomm_dlci0_lock_and_transmit_cmesg(multi_inst, buffer, PRH_RFCOMM_CR_SET);
      if (ret!=BT_NOERROR)
	prh_l2_FreeWriteBuffer(buffer);
    }
  return ret;
}

t_api PRH_RFCOMM_UE_Portneg_Resp(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_port_params *port_parameters, prh_t_rfcomm_accept accept, void *multi_inst)
{
  struct host_buf *buffer;
  int ret;
  struct prh_rfcomm_l2cap_inst *l2cap_inst= (struct prh_rfcomm_l2cap_inst *)multi_inst;

  if (port_parameters==NULL)
    return BT_INVALIDPARAM;

  ret=prh_rfcomm_validate_and_lock_l2cap_inst_by_ptr(l2cap_inst);
  if (ret==BT_NOERROR)
  {
    ret=PRH_RFCOMM_GET_L2_CONTROL_FRAME(l2cap_inst->cid, PRH_RFCOMM_CC_RPN_LEN_COMM, &buffer);
    if (ret!=BT_NOERROR)
      {
	pMutexUnlock(l2cap_inst->lock);
	return ret;
      }
    host_buf_reserve_header(buffer, PRH_RFCOMM_BUF_CONTROL_HEADERS);

    prh_rfcomm_cc_create_rpn_comm(buffer, PRH_RFCOMM_CR_UNSET, dlci, port_parameters);
    ret=prh_rfcomm_dlci0_lock_and_transmit_cmesg(multi_inst, buffer, PRH_RFCOMM_CR_UNSET);
    if (ret!=BT_NOERROR)
      prh_l2_FreeWriteBuffer(buffer);
  }
  return ret;
}

/*
 * PRH_RFCOMM_UE_Control_Req
 * -- Send an MSC on the DLC specified with the parameters specified
 */
t_api PRH_RFCOMM_UE_Control_Req(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_control_params *control_params, void *multi_inst)
{
  struct prh_rfcomm_dlc_inst *dlc_inst;
  struct host_buf *buffer;
  int ret;
  struct prh_rfcomm_l2cap_inst *l2cap_inst= (struct prh_rfcomm_l2cap_inst *)multi_inst;

  ret=prh_rfcomm_validate_and_lock_l2cap_inst_by_ptr(l2cap_inst);
  if (ret==BT_NOERROR)
    {
      ret=PRH_RFCOMM_GET_L2_CONTROL_FRAME(l2cap_inst->cid, PRH_RFCOMM_CC_MSC_LONG_LEN, &buffer);
      if (ret!=BT_NOERROR)
	{
	  pMutexUnlock(l2cap_inst->lock);
	  return ret;
	}
      host_buf_reserve_header(buffer, PRH_RFCOMM_BUF_CONTROL_HEADERS);

      ret=prh_rfcomm_lookup_dlci_instance_and_lock(multi_inst, dlci, &dlc_inst);
      if (ret==BT_NOERROR)
	{
	  /* DLCI exists this is all we wanted to know */
	  pMutexUnlock(dlc_inst->lock);
#if pDEBUG
	  pDebugPrintfEX((pLOGDEBUG, pLOGRFCOMM,"RFCOMM: tx: CC: MSC\n"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
          SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM,(LOG_RFC_RFCOMM_UE_API_INDEX+1),NULL,0,0);
#endif
	  prh_rfcomm_cc_create_msc(buffer, PRH_RFCOMM_CR_SET, dlci, control_params);
	  ret=prh_rfcomm_dlci0_lock_and_transmit_cmesg(multi_inst, buffer, PRH_RFCOMM_CR_SET);
	  if (ret!=BT_NOERROR)
	    prh_l2_FreeWriteBuffer(buffer);
	}
      return BT_NOERROR;
    }
  return ret;
}

/*
 * PRH_RFCOMM_UE_Linestatus_Req
 * -- Send an RLS on the DLC specified with the parameters specified
 */
t_api PRH_RFCOMM_UE_Linestatus_Req(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_line_stat_params line_stat_params, void *multi_inst)
{
  struct prh_rfcomm_dlc_inst *dlc_inst;
  struct host_buf *buffer;
  int ret;
  struct prh_rfcomm_l2cap_inst *l2cap_inst= (struct prh_rfcomm_l2cap_inst *)multi_inst;

  ret=prh_rfcomm_validate_and_lock_l2cap_inst_by_ptr(l2cap_inst);
  if (ret==BT_NOERROR)
    {
      ret=prh_rfcomm_lookup_dlci_instance_and_lock(multi_inst, dlci, &dlc_inst);
      if (ret==BT_NOERROR)
	{
	  /* DLCI exists */
	  pMutexUnlock(dlc_inst->lock);
	  ret=PRH_RFCOMM_GET_L2_CONTROL_FRAME(l2cap_inst->cid, PRH_RFCOMM_CC_LSR_LEN, &buffer);
	  if (ret!=BT_NOERROR)
	    {
	      pMutexUnlock(l2cap_inst->lock);
	      return ret;
	    }
	  host_buf_reserve_header(buffer, PRH_RFCOMM_BUF_CONTROL_HEADERS);
#if pDEBUG	  
	  pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"RFCOMM: tx: CC: LSR\n"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
          SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM,(LOG_RFC_RFCOMM_UE_API_INDEX+2),NULL,0,0);
#endif

	  if (line_stat_params)
	    line_stat_params |= PRH_RFCOMM_LSTAT_ERROR;
	  prh_rfcomm_cc_create_rls(buffer, PRH_RFCOMM_CR_SET, dlci, line_stat_params);
	  ret=prh_rfcomm_dlci0_lock_and_transmit_cmesg(multi_inst, buffer, PRH_RFCOMM_CR_SET);
	  if (ret!=BT_NOERROR)
	    prh_l2_FreeWriteBuffer(buffer);
	}
     
    }
  return ret;
}

/*
 * PRH_RFCOMM_UE_Parneg_Req
 * -- Send a PN on the DLC specified with the parameters specified
 */
t_api PRH_RFCOMM_UE_Parneg_Req(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, void *multi_inst)
{
  return __prh_rfcomm_ue_Parneg_Req(dlci, dlc_params, multi_inst, 1);
}

t_api __prh_rfcomm_ue_Parneg_Req(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, void *multi_inst, u_int8 locking)
{
  struct host_buf *buffer;
  struct prh_rfcomm_l2cap_inst *l2cap_inst= (struct prh_rfcomm_l2cap_inst *)multi_inst;
  int ret;
  
  if (locking==1)
    ret=prh_rfcomm_validate_and_lock_l2cap_inst_by_ptr(l2cap_inst);
  else
    ret=BT_NOERROR;
  
  if (ret==BT_NOERROR)
    {
      ret=PRH_RFCOMM_GET_L2_CONTROL_FRAME(l2cap_inst->cid, PRH_RFCOMM_CC_PN_LEN, &buffer);
      if (ret!=BT_NOERROR)
	{
	  if (locking==1)
	    pMutexUnlock(l2cap_inst->lock);
	  return ret;
	}
      host_buf_reserve_header(buffer, PRH_RFCOMM_BUF_CONTROL_HEADERS);
      prh_rfcomm_cc_create_credit_pn(buffer, PRH_RFCOMM_CR_SET, dlci, dlc_params, 0, 0);
      if (locking==1)
	ret=prh_rfcomm_dlci0_lock_and_transmit_cmesg(multi_inst, buffer, PRH_RFCOMM_CR_SET);
      else
	ret=prh_rfcomm_cc_transmit_cmesg(multi_inst, buffer, PRH_RFCOMM_CR_SET);
      if (ret!=BT_NOERROR)
	prh_l2_FreeWriteBuffer(buffer);
    }
  return ret;
}

/*
 * PRH_RFCOMM_UE_Parneg_Resp
 * -- Send a PN Respsonse on the DLC 
 */ 
t_api PRH_RFCOMM_UE_Parneg_Resp(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, prh_t_rfcomm_accept accept, void *multi_inst)
{
  struct host_buf *buffer;
  struct prh_rfcomm_l2cap_inst *l2cap_inst= (struct prh_rfcomm_l2cap_inst *)multi_inst;
  int ret;

  ret=prh_rfcomm_validate_and_lock_l2cap_inst_by_ptr(l2cap_inst);
  if (ret==BT_NOERROR)
    {
      ret=PRH_RFCOMM_GET_L2_CONTROL_FRAME(l2cap_inst->cid, PRH_RFCOMM_CC_PN_LEN, &buffer);
      if (ret!=BT_NOERROR)
	{
	  pMutexUnlock(l2cap_inst->lock);
	  return ret;
	}
      host_buf_reserve_header(buffer, PRH_RFCOMM_BUF_CONTROL_HEADERS);

      prh_rfcomm_cc_create_credit_pn(buffer, PRH_RFCOMM_CR_UNSET, dlci, dlc_params, 0, 0);
      ret=prh_rfcomm_dlci0_lock_and_transmit_cmesg(multi_inst, buffer, PRH_RFCOMM_CR_UNSET);
      if (ret!=BT_NOERROR)
	prh_l2_FreeWriteBuffer(buffer);
    }
  return ret;
}

/*
 * PRH_RFCOMM_UE_Get_Multiplexer_State
 * -- Get the state of a multiplexer to a device
 * returns 
 *    BT_NOERROR - Multiplexer is connected
 *    BT_PENDING - Multiplexer connection is pending
 *    BT_INVALIDPARAM - Multiplexer doesn't exist
 */
t_api PRH_RFCOMM_UE_Get_Multiplexer_State(t_bdaddr bd_addr, void **multi_inst)
{
  int ret;
  struct prh_rfcomm_l2cap_inst *l2cap_inst;
  struct prh_rfcomm_dlc_inst *dlc0_inst;

  ret=prh_rfcomm_lookup_l2cap_inst_by_bd_addr(bd_addr, &l2cap_inst);
  if (ret==BT_NOERROR)
    {
      *multi_inst=(void *)l2cap_inst;
      if (l2cap_inst->l2cap_state==PRH_RFCOMM_L2CAP_CONN)
	{
	  /* check DLCI 0 is connected */
	  ret=prh_rfcomm_lookup_dlci_instance(l2cap_inst, 0, &dlc0_inst);
	  if (ret==BT_NOERROR)
	    {
	      if (dlc0_inst->dlc_state==PRH_RFCOMM_DLC_CONN)
		ret=BT_NOERROR;
	      else
		ret=BT_PENDING;
	    }
	  else
	    ret=BT_INVALIDPARAM;
	}
      else
	ret=BT_PENDING;
    }
  return ret;
}

/*
 * PRH_RFCOMM_UE_Parneg_Req
 * -- Send a PN on the DLC specified with the parameters specified
 */
t_api PRH_RFCOMM_UE_Parneg_Credit_Req(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, u_int8 init_credits, void *multi_inst)
{
  return __prh_rfcomm_ue_Parneg_Credit_Req(dlci, dlc_params, init_credits, multi_inst, 1);
}

t_api __prh_rfcomm_ue_Parneg_Credit_Req(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, u_int8 init_credits, void *multi_inst, u_int8 locking)
{
  struct host_buf *buffer;
  struct prh_rfcomm_l2cap_inst *l2cap_inst= (struct prh_rfcomm_l2cap_inst *)multi_inst;
  int ret;
  
  if (locking==1)
    {
      ret=prh_rfcomm_validate_and_lock_l2cap_inst_by_ptr(l2cap_inst);
    }
  else
    ret=BT_NOERROR;
  
  if (ret==BT_NOERROR)
    {
      ret=PRH_RFCOMM_GET_L2_CONTROL_FRAME(l2cap_inst->cid, PRH_RFCOMM_CC_PN_LEN, &buffer);
      if (ret!=BT_NOERROR)
	{
	  if (locking==1)
	    pMutexUnlock(l2cap_inst->lock);
	  return ret;
	}
      host_buf_reserve_header(buffer, PRH_RFCOMM_BUF_CONTROL_HEADERS);

      prh_rfcomm_cc_create_credit_pn(buffer, PRH_RFCOMM_CR_SET, dlci, dlc_params, PRH_RFCOMM_PN_CREDIT_FLOW_REQ, init_credits);
      if (locking==1)
	ret=prh_rfcomm_dlci0_lock_and_transmit_cmesg(multi_inst, buffer, PRH_RFCOMM_CR_SET);
      else
	ret=prh_rfcomm_cc_transmit_cmesg(multi_inst, buffer, PRH_RFCOMM_CR_SET);
      if (ret!=BT_NOERROR)
	prh_l2_FreeWriteBuffer(buffer);      
    }
  return ret;
}

/*
 * PRH_RFCOMM_UE_Parneg_Resp
 * -- Send a PN Respsonse on the DLC 
 */ 
t_api PRH_RFCOMM_UE_Parneg_Credit_Resp(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, prh_t_rfcomm_accept accept, u_int8 init_credits, void *multi_inst)
{
  struct host_buf *buffer;
  struct prh_rfcomm_l2cap_inst *l2cap_inst= (struct prh_rfcomm_l2cap_inst *)multi_inst;
  int ret;

  ret=prh_rfcomm_validate_and_lock_l2cap_inst_by_ptr(l2cap_inst);
  if (ret==BT_NOERROR)
    {
      ret=PRH_RFCOMM_GET_L2_CONTROL_FRAME(l2cap_inst->cid, PRH_RFCOMM_CC_PN_LEN, &buffer);
      if (ret!=BT_NOERROR)
	{
	  pMutexUnlock(l2cap_inst->lock);
	  return ret;
	}
      host_buf_reserve_header(buffer, PRH_RFCOMM_BUF_CONTROL_HEADERS);

      prh_rfcomm_cc_create_credit_pn(buffer, PRH_RFCOMM_CR_UNSET, dlci, dlc_params, PRH_RFCOMM_PN_CREDIT_FLOW_RESP, init_credits);
      ret=prh_rfcomm_dlci0_lock_and_transmit_cmesg(multi_inst, buffer, PRH_RFCOMM_CR_UNSET);
      if (ret!=BT_NOERROR)
	prh_l2_FreeWriteBuffer(buffer);
    }
  return ret;
}

t_api PRH_RFCOMM_UE_Enable_Credit(void)
{
  pMutexLock(prh_rfcomm_core_lock);
  prh_rfcomm_core->initial_credit_state=1;
  pMutexUnlock(prh_rfcomm_core_lock);
  return BT_NOERROR;
}

t_api PRH_RFCOMM_UE_Disable_Credit(void)
{
  pMutexLock(prh_rfcomm_core_lock);
  prh_rfcomm_core->initial_credit_state=0;
  pMutexUnlock(prh_rfcomm_core_lock);
  return BT_NOERROR;
}

t_api PRH_RFCOMM_UE_Get_L2CAP_CID_From_Multiplexer(void *multi_inst, u_int16 *cid)
{
  struct prh_rfcomm_l2cap_inst *l2cap_inst=(struct prh_rfcomm_l2cap_inst *)multi_inst;
  int ret;
  ret=prh_rfcomm_validate_and_lock_l2cap_inst_by_ptr(l2cap_inst);
  if (ret==BT_NOERROR)
    {  
      *cid=l2cap_inst->cid;
      pMutexUnlock(l2cap_inst->lock);
    }
  return ret;
}

/* this function is entered with the L2CAP instance locked, it looks up the DLCI 0 instance,
   locks it, unlocks L2CAP, transmits the CMESG, unlocks DLCI 0 returns. the callee must return
   to the app straight away */
/* prereq: locked L2CAP
   postreq: unlocked L2CAP locked DLCI0 */
t_api prh_rfcomm_dlci0_lock_and_transmit_cmesg(struct prh_rfcomm_l2cap_inst *self, struct host_buf *buffer, u_int8 cresp)
{
  struct prh_rfcomm_dlc_inst *dlci0_inst;
  int ret;

  ret=prh_rfcomm_lookup_dlci_instance_and_lock(self, PRH_RFCOMM_DLCI0, &dlci0_inst);
  pMutexUnlock(self->lock);
  if (ret==BT_NOERROR)
    {
      ret=prh_rfcomm_cc_transmit_cmesg(self, buffer, cresp);
      pMutexUnlock(dlci0_inst->lock);
    }
  return ret;
}

