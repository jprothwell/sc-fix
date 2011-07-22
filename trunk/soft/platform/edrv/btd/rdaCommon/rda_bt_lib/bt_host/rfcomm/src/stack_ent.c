/***********************************************************************
 *
 * MODULE NAME:    stack_ent.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Generic Stack Entity code
 * MAINTAINER:     David Airlie
 * CREATION DATE:  1 May 2001
 *
 * SOURCE CONTROL: $Id: stack_ent.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Inc.
 *     All rights reserved.
 *
 *
 ***********************************************************************/
#include "rfcomm_lower_layer.h"

#if PRH_RFCOMM_RSE_SUPPORT==1
#include "rfcomm_ue_api.h"
#include "rfcomm_core.h"
#include "rfcomm_debug.h"
#include "rfcomm_proto.h"
#include "stack_ent_core.h"

t_api prh_stack_ent_dlc_estab_conf_cb(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci_t, prh_t_rfcomm_dlc_params *dlc_params, prh_t_rfcomm_accept accept, void *multi_inst);
t_api prh_stack_ent_dlc_estab_ind_cb(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, void *multi_inst);
t_api prh_stack_ent_dlc_rel_ind_cb(prh_t_rfcomm_dlci dlci, void *multi_inst);
t_api prh_stack_ent_dlc_data_ind_cb(prh_t_rfcomm_dlci dlci, struct host_buf *buffer, void *multi_inst);
t_api prh_stack_ent_dlc_linestat_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_line_stat_params ls_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype);
t_api prh_stack_ent_dlc_portneg_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_port_params *port_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype);
t_api prh_stack_ent_dlc_control_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_control_params *ctrl_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype);
t_api prh_stack_ent_dlc_flow_ind_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_flow_state flow_state, void *multi_inst);
t_api prh_stack_ent_dlc_paramneg_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype);
t_api prh_stack_ent_dlc_credit_flow_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_flow_state flow_state, void *multi_inst);

/* handle for the RFCOMM system wide instance */
extern void *pe_rfcomm_mgr_handle;

void prh_rfcomm_init_se_struct(struct prh_rfcomm_se_user *se_info)
{
  se_info->agg_flow_state=PRH_RFCOMM_FLOW_DATA_GO;
  se_info->local_flow_state=PRH_RFCOMM_FLOW_DATA_GO;

  se_info->port_params.port_speed=RF9600;
  se_info->port_params.data_bits=RFDS8;
  se_info->port_params.stop_bits=RFSTOP1;
  se_info->port_params.parity_enable=RFNONE;
  se_info->port_params.paritytype=RFODD;
  se_info->port_params.xon=0x11;
  se_info->port_params.xoff=0x13;
  se_info->port_params.flow_mask=0;
  se_info->port_params.param_mask=0;
  se_info->dlc_params.max_frame_size=127;
  se_info->dlci=0;
  se_info->dlc_params.credit_flow_enabled=0;
}

/*
 * prh_stack_ent_dlc_estab_ind_cb
 * -- Port Ent Callback for DLC Establishment Indiciations
 *
 * When a DLC is established on a multiplexer this callback is called
 * with the parameters for the DLC. It checks the server channel is registered
 * and responds with an accept or reject depending, and sends an MSC after
 * accepting.
 */
t_api prh_stack_ent_dlc_estab_ind_cb(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, void *multi_inst)
{
    int ret;
    u_int8 accept=1;
    int called_upper=0;
    prh_t_rfcomm_schan loc_chan;
    struct prh_rfcomm_se_user *se_info;
    /* Search for this in the port entity... extract server channel from dlci */
    loc_chan=dlci>>1;

    /* Need to check is it a registered server channel */
    /* Return 0 for no channel, 1 if a channel exists */

    ret=prh_rfcomm_se_lookup_stack_entity_by_portnum(&se_info, loc_chan);
#if pDEBUG
        pDebugPrintfEX((pLOGNOTICE,pLOGRFCOMM,"prh_stack_ent_dlc_estab_ind_cb  loc_chan=%d, ret=%d\n",loc_chan, ret));
#endif 
    if (ret==BT_NOERROR)
    {
        /* Has it a DLCI already assigned? if so it is connected */
        if (se_info->dlci!=0)
            accept=0;
        else
        {
            if (dlci>0)
                se_info->dlci=dlci;
            
            se_info->multi_inst=multi_inst;
            se_info->remote_device_addr=bd_addr;
            
            if (dlc_params)
                if (dlc_params->max_frame_size>0)
                    se_info->dlc_params=*dlc_params;
                
            accept=1;
            called_upper=1;
            
            if (se_info->cb.connectionIndication)
                (*se_info->cb.connectionIndication)((prh_t_rfcomm_schan)se_info->portnum, bd_addr, dlci, se_info->dlc_params.max_frame_size);
        }
    }
    else
    {
        accept=0;
    }

    if (called_upper==0)
    {
        PRH_RFCOMM_UE_DLC_Estab_Resp(bd_addr, dlci, &se_info->dlc_params, 0, multi_inst);
    }

    return 0;
}

/*
 * prh_stack_ent_dlc_estab_conf_cb
 * -- DLC Estab confirm callback
 * 
 */
t_api prh_stack_ent_dlc_estab_conf_cb(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci_t, prh_t_rfcomm_dlc_params *dlc_params, prh_t_rfcomm_accept accept, void *multi_inst)
{
  /* Need to lookup DLCI / BD Address and call callback */
  struct prh_rfcomm_se_user *se_info;
  prh_t_rfcomm_control_params control_params;
  int ret;

  ret=prh_rfcomm_se_lookup_stack_entity_by_addr_and_dlci(&se_info, bd_addr, dlci_t);
  if (ret==BT_NOERROR)
    {
      if (se_info->cb.connectionConfirmation)
	{
	  if (accept)
	    {
	      control_params.flow_state=PRH_RFCOMM_FLOW_DATA_GO;
	      control_params.break_signal=0;
	      control_params.modem_signal=PRH_RFCOMM_MSC_DTRDSR | PRH_RFCOMM_MSC_RTSCTS | PRH_RFCOMM_MSC_DCD;
	      
	      PRH_RFCOMM_UE_Control_Req(dlci_t, &control_params, multi_inst);
	      (*se_info->cb.connectionConfirmation)((unsigned char)(se_info->dlci>>1), bd_addr, PRH_STACK_ENT_ACCEPT, dlci_t, se_info->dlc_params.max_frame_size);
	    }
	  else
	    goto out_connect_failed;
	}
    }
  
  return BT_NOERROR;
 out_connect_failed:    
  {
    t_api (*_connect_conf_cb)(prh_t_rfcomm_schan schan, t_bdaddr address, u_int8 status, prh_t_rfcomm_dlci dlci_s, u_int16 max_frame_size) = se_info->cb.connectionConfirmation;
    prh_t_rfcomm_schan _schan = se_info->dlci>>1;
    struct prh_rfcomm_se_user *se_info_ptr;
    
    prh_rfcomm_se_remove_stack_entity_by_addr_and_dlci(&se_info_ptr, se_info->remote_device_addr, se_info->dlci);
    
    pFree(se_info);
    
    (*_connect_conf_cb)((unsigned char)_schan, bd_addr, PRH_STACK_ENT_REFUSE, dlci_t, 0);
    return BT_NOERROR;
  }
}

t_api prh_stack_ent_dlc_data_ind_cb(prh_t_rfcomm_dlci dlci, struct host_buf *p_buf, void *multi_inst) 
{
  struct prh_rfcomm_se_user *se_info;
  int ret;
  struct prh_rfcomm_l2cap_inst *l2cap_inst=(struct prh_rfcomm_l2cap_inst *)multi_inst;

  ret=prh_rfcomm_se_lookup_stack_entity_by_addr_and_dlci(&se_info, l2cap_inst->bd_addr, dlci);
  if (ret==BT_NOERROR)
    {
      if (se_info->cb.dataIndication)
	(*se_info->cb.dataIndication)(l2cap_inst->bd_addr, se_info->dlci, p_buf);
    }
  else
    PRH_RFCOMM_UE_Release_Write_Buffer(p_buf);
  return 0;
}

t_api prh_stack_ent_dlc_linestat_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_line_stat_params ls_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype)
{
  return 0;
}

t_api prh_stack_ent_dlc_portneg_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_port_params *port_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype)
{
  prh_t_rfcomm_port_params resp_params;
  struct prh_rfcomm_se_user *se_info;
  int ret;
  struct prh_rfcomm_l2cap_inst *l2cap_inst=(struct prh_rfcomm_l2cap_inst *)multi_inst;
  int dbit;

  /* Check is it a connected DLC */
  ret=prh_rfcomm_se_lookup_stack_entity_by_addr_and_dlci(&se_info, l2cap_inst->bd_addr, dlci);
  if (ret!=BT_NOERROR)
    {
      /* If not check is a local server channel */
      dbit=l2cap_inst->initiating;
      if (!((dlci&1)^(dbit&1)))
	{
	  ret=prh_rfcomm_se_lookup_stack_entity_by_portnum(&se_info, dlci>>1);
	}
      if (ret!=BT_NOERROR)
	return BT_NOERROR;
    }
  
  if (cbtype==PRH_RFCOMM_CB_IND)
    {
      /* Send a response */
      if (port_params==NULL)
	{
	  /* Need to send back my parameters */
	  resp_params=se_info->port_params;
	}
      else
	{
	  /* Decode the parameter mask bits */
	  if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_BITRATE)
	    {
	      /* Remote device wishes to change baudrate */
	      se_info->port_params.port_speed=port_params->port_speed;
	      
	    }
	  
	  if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_DATABITS)
	    {
	      se_info->port_params.data_bits=port_params->data_bits;
	    }
	  
	  if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_STOPBITS)
	    {
	      se_info->port_params.stop_bits=port_params->stop_bits;
	    }
	  
	  if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_PARITY)
	    {
	      se_info->port_params.parity_enable=port_params->parity_enable;
	    }
	  
	  if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_PARITY_TYPE)
	    {
	      se_info->port_params.paritytype=port_params->paritytype;
	    }
	  
	  if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_XON_CH)
	    {
	      se_info->port_params.xon=port_params->xon;
	    }
	  
	  if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_XOFF_CH)
	    {
	      se_info->port_params.xoff=port_params->xoff;
	    }
	  
	  if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_XONXOFF_INPUT)
	    {
	      if (port_params->flow_mask & PRH_RPN_FLOW_INPUT_XONXOFF)
		se_info->port_params.flow_mask |= PRH_RPN_FLOW_INPUT_XONXOFF;
	      else
		se_info->port_params.flow_mask &= ~PRH_RPN_FLOW_INPUT_XONXOFF;
	    }
	  
	  if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_XONXOFF_OUTPUT)
	    {
	      if (port_params->flow_mask & PRH_RPN_FLOW_OUTPUT_XONXOFF)
		se_info->port_params.flow_mask |= PRH_RPN_FLOW_OUTPUT_XONXOFF;
	      else
		se_info->port_params.flow_mask &= ~PRH_RPN_FLOW_OUTPUT_XONXOFF;
	    }
	  
	  if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_RTR_INPUT)
	    {
	      if (port_params->flow_mask & PRH_RPN_FLOW_INPUT_RTR)
		se_info->port_params.flow_mask |= PRH_RPN_FLOW_INPUT_RTR;
	      else
		se_info->port_params.flow_mask &= ~PRH_RPN_FLOW_INPUT_RTR;
	    }
	  
	  if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_RTR_OUTPUT)
	    {
	      if (port_params->flow_mask & PRH_RPN_FLOW_OUTPUT_RTR)
		se_info->port_params.flow_mask |= PRH_RPN_FLOW_OUTPUT_RTR;
	      else
		se_info->port_params.flow_mask &= ~PRH_RPN_FLOW_OUTPUT_RTR;
	    } 
	  
	  if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_RTC_INPUT)
	    {
	      if (port_params->flow_mask & PRH_RPN_FLOW_INPUT_RTC)
		se_info->port_params.flow_mask |= PRH_RPN_FLOW_INPUT_RTC;
	      else
		se_info->port_params.flow_mask &= ~PRH_RPN_FLOW_INPUT_RTC;
	    }
	  
	  if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_RTC_OUTPUT)
	    {
	      if (port_params->flow_mask & PRH_RPN_FLOW_OUTPUT_RTC)
		se_info->port_params.flow_mask |= PRH_RPN_FLOW_OUTPUT_RTC;
	      else
		se_info->port_params.flow_mask &= ~PRH_RPN_FLOW_OUTPUT_RTC;
	    }
	  
	  resp_params=se_info->port_params;
	  resp_params.param_mask=port_params->param_mask;
	}
      PRH_RFCOMM_UE_Portneg_Resp(dlci, &resp_params, PRH_RFCOMM_ACCEPT, multi_inst);
    }
  return 0;
}

t_api prh_stack_ent_dlc_flow_ind_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_flow_state flow_state, void *multi_inst)
{
  int ret;
  struct prh_rfcomm_se_user *se_info;
  struct prh_rfcomm_l2cap_inst *l2cap_inst=(struct prh_rfcomm_l2cap_inst *)multi_inst;
  
  ret=prh_rfcomm_se_lookup_stack_entity_by_addr_and_dlci(&se_info, l2cap_inst->bd_addr, dlci);
  if (ret!=BT_NOERROR)
    return BT_NOERROR;

  if (flow_state==PRH_RFCOMM_FLOW_DATA_STOP)
    {
      se_info->agg_flow_state=PRH_RFCOMM_FLOW_DATA_STOP;
      if (se_info->cb.flowIndication)
	se_info->cb.flowIndication(l2cap_inst->bd_addr, dlci, RSE_FLOW_STOP);

    }
  else
    {
      se_info->agg_flow_state=PRH_RFCOMM_FLOW_DATA_GO;
      if (se_info->local_flow_state==PRH_RFCOMM_FLOW_DATA_GO)
	if (se_info->cb.flowIndication)
	  se_info->cb.flowIndication(l2cap_inst->bd_addr, dlci, RSE_FLOW_GO);
    }
  return BT_NOERROR;
      
}

t_api prh_stack_ent_dlc_control_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_control_params *ctrl_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype)
{
  int ret;
  struct prh_rfcomm_se_user *se_info;
  struct prh_rfcomm_l2cap_inst *l2cap_inst=(struct prh_rfcomm_l2cap_inst *)multi_inst;

  ret=prh_rfcomm_se_lookup_stack_entity_by_addr_and_dlci(&se_info, l2cap_inst->bd_addr, dlci);
  if (ret!=BT_NOERROR)
    return BT_NOERROR;

  /* ignore FC bit in CBFC states */
  if (l2cap_inst->credit_state!=CREDIT_ON)
    {
      if (ctrl_params->flow_state==PRH_RFCOMM_FLOW_DATA_STOP)
	{
	  se_info->local_flow_state=PRH_RFCOMM_FLOW_DATA_STOP;
	  if (se_info->cb.flowIndication)
	    se_info->cb.flowIndication(l2cap_inst->bd_addr, dlci, RSE_FLOW_STOP);
	}
      else
	{
	  se_info->local_flow_state=PRH_RFCOMM_FLOW_DATA_GO;
	  if (se_info->agg_flow_state==PRH_RFCOMM_FLOW_DATA_GO)
	    if (se_info->cb.flowIndication)
	      se_info->cb.flowIndication(l2cap_inst->bd_addr, dlci, RSE_FLOW_GO);
	  
	}
    }
  return BT_NOERROR;
}

t_api prh_stack_ent_dlc_credit_flow_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_flow_state credit_state, void *multi_inst)
{
  int ret;
  struct prh_rfcomm_se_user *se_info;
  struct prh_rfcomm_l2cap_inst *l2cap_inst=(struct prh_rfcomm_l2cap_inst *)multi_inst;

  ret=prh_rfcomm_se_lookup_stack_entity_by_addr_and_dlci(&se_info, l2cap_inst->bd_addr, dlci);
  if (ret!=BT_NOERROR)
    return BT_NOERROR;

  if (credit_state==PRH_RFCOMM_FLOW_DATA_STOP)
    {
      se_info->local_flow_state=PRH_RFCOMM_FLOW_DATA_STOP;
      if (se_info->cb.flowIndication)
	se_info->cb.flowIndication(l2cap_inst->bd_addr, dlci, RSE_FLOW_STOP);
    }
  else
    {
      se_info->local_flow_state=PRH_RFCOMM_FLOW_DATA_GO;
      if (se_info->cb.flowIndication)
	se_info->cb.flowIndication(l2cap_inst->bd_addr, dlci, RSE_FLOW_GO);
    }
  return BT_NOERROR;
}

t_api prh_stack_ent_dlc_paramneg_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype)
{

#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
u_int8 temp[3];
#endif

  int ret;
  struct prh_rfcomm_se_user *se_info;
  struct prh_rfcomm_l2cap_inst *l2cap_inst=(struct prh_rfcomm_l2cap_inst *)multi_inst;
  u_int8 dbit;

  ret=prh_rfcomm_se_lookup_stack_entity_by_addr_and_dlci(&se_info, l2cap_inst->bd_addr, dlci);
  if (ret!=BT_NOERROR)
    {
      dbit=l2cap_inst->initiating;
      if (!((dlci&1)^(dbit&1)))
	{
	  ret=prh_rfcomm_se_lookup_stack_entity_by_portnum(&se_info, dlci>>1);
	}
      if (ret!=BT_NOERROR)
	return BT_NOERROR;
    }
  
  if (cbtype==PRH_RFCOMM_CB_IND)
    {
#if pDEBUG
      pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"RFCOMM: rx DLC%d: PN frame %d\n", dlci, dlc_params->max_frame_size));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
      temp[0]= dlci;
      BT_WRITE_LE_UINT16(&temp[1],dlc_params->max_frame_size);
      SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM,LOG_RFC_STACK_ENT_INDEX,temp,3,(BT_CODE_BYTE | (BT_CODE_WORD<<4)));
#endif


      if (se_info->dlc_params.credit_flow_enabled==0)
	{
	  if (dlc_params->max_frame_size>PRH_RFCOMM_MAX_FRAME_SIZE)
	    dlc_params->max_frame_size=PRH_RFCOMM_MAX_FRAME_SIZE;
	  se_info->dlc_params.max_frame_size=dlc_params->max_frame_size;
	}
      else
	{
	  /* always return the original negotiated packet size */
	  dlc_params->max_frame_size=se_info->dlc_params.max_frame_size;
	}

      /* Only look at the CF enabled stuff if we are disconnected */
      if (dlc_params->credit_flow_enabled && se_info->dlci==0)
	{
	  se_info->dlc_params.credit_flow_enabled=1;
	  se_info->dlc_params.peer_credits=dlc_params->peer_credits;
	  se_info->dlc_params.local_credits=PRH_RFCOMM_CREDIT_ISSUE;
	  /* UPF-5 D.A. if peer gives us no credits... set the flow to STOP */
	  if (se_info->dlc_params.peer_credits==0)
	    se_info->local_flow_state=PRH_RFCOMM_FLOW_DATA_STOP;
	  PRH_RFCOMM_UE_Parneg_Credit_Resp(dlci, dlc_params, 0, se_info->dlc_params.local_credits, multi_inst);
	}
      else
	PRH_RFCOMM_UE_Parneg_Resp(dlci, dlc_params, 0, multi_inst);
    }
  else
    {
      /* UPF6 -- if peer returns value less than requested... use that
         value. - D.A. */
      if (dlc_params->max_frame_size<se_info->dlc_params.max_frame_size)
	se_info->dlc_params.max_frame_size=dlc_params->max_frame_size;
    }
  return BT_NOERROR; 
}

/*
 * prh_stack_ent_dlc_rel_ind_cb
 * -- DLC callback to handle a remote release indication
 *
 * lookup up the port and callback the application with a disconnect callback
 */
t_api prh_stack_ent_dlc_rel_ind_cb(prh_t_rfcomm_dlci dlci, void *multi_inst)
{
  int ret;
  struct prh_rfcomm_se_user *se_info;
  struct prh_rfcomm_l2cap_inst *l2cap_inst=(struct prh_rfcomm_l2cap_inst *)multi_inst;
  int dbit;

  dbit=l2cap_inst->initiating;
  
  if ((dlci&1)^(dbit&1))
    {
      ret=prh_rfcomm_se_remove_stack_entity_by_addr_and_dlci(&se_info, l2cap_inst->bd_addr,dlci);
    }
  else
    {
      ret=prh_rfcomm_se_lookup_stack_entity_by_portnum(&se_info, dlci>>1);
    }
  if (ret==BT_NOERROR)
    {	
      /* Reset the structure after a release */
      if (se_info->cb.disconnectionIndication)
        (*se_info->cb.disconnectionIndication)(l2cap_inst->bd_addr, dlci);

      if ((dlci&1)^(dbit&1))
        {
	  pFree(se_info);
        }
      else
	prh_rfcomm_init_se_struct(se_info); /* just reset the structure */
    }
  else
    return BT_INVALIDPARAM;
  
  return BT_NOERROR;
}

/*
 * Linked list of Port Entitys
 * Handler functions
 */
struct prh_host_gen_ll *prh_rfcomm_se_head = NULL;

int prh_rfcomm_se_add_stack_entity(struct prh_rfcomm_se_user *self)
{
  int ret;
  ret=prh_host_ll_add(&prh_rfcomm_se_head, (void *)self);
  return ret;
}

static int prh_rfcomm_se_portnum_cmp(void *cmpin, void *compwith)
{
  struct prh_rfcomm_se_user *se_info=(struct prh_rfcomm_se_user *)compwith;
  u_int32 portnum=(u_int32)cmpin;

  return (se_info->portnum==portnum);
}

int prh_rfcomm_se_remove_stack_entity_by_portnum(struct prh_rfcomm_se_user **port, u_int32 portnum)
{
  int ret;
  struct prh_rfcomm_se_user *returned;
  ret=prh_host_ll_remove(&prh_rfcomm_se_head, (void *)&returned, (void *)(u_int32)portnum, &prh_rfcomm_se_portnum_cmp);
  if (ret==BT_NOERROR)
    *port=returned;
  return ret;
}

int prh_rfcomm_se_lookup_stack_entity_by_portnum(struct prh_rfcomm_se_user **port, u_int32 portnum)
{
	int ret;
	struct prh_rfcomm_se_user *returned;

	ret=prh_host_ll_search(&prh_rfcomm_se_head, (void *)&returned, (void *)(u_int32)portnum, &prh_rfcomm_se_portnum_cmp);
	if (ret==BT_NOERROR)
		*port=returned;
	
	return ret;
}

struct prh_st_rfcomm_lookupstructure 
{
  t_bdaddr bdaddr;
  prh_t_rfcomm_dlci dlci;
};

static int prh_rfcomm_se_addrdlci_cmp(void *cmpin, void *compwith)
{
  struct prh_rfcomm_se_user *se_info=(struct prh_rfcomm_se_user *)compwith;
  struct prh_st_rfcomm_lookupstructure *addrdlci=(struct prh_st_rfcomm_lookupstructure *)cmpin;
  int ret;

  ret=((BD_ADDR_CMP(addrdlci->bdaddr, se_info->remote_device_addr)) && (addrdlci->dlci==se_info->dlci));
  return ret; 
}

int prh_rfcomm_se_lookup_stack_entity_by_addr_and_dlci(struct prh_rfcomm_se_user **port, t_bdaddr bdaddr, prh_t_rfcomm_dlci dlci)
{
  int ret;
  struct prh_rfcomm_se_user *returned;
  struct prh_st_rfcomm_lookupstructure lookfor;
  
  lookfor.dlci=dlci;
  lookfor.bdaddr=bdaddr;

  ret=prh_host_ll_search(&prh_rfcomm_se_head, (void *)&returned, (void *)(u_int32)&lookfor, &prh_rfcomm_se_addrdlci_cmp);
  if (ret==BT_NOERROR)
    *port=returned;
  return ret;
}

int prh_rfcomm_se_remove_stack_entity_by_addr_and_dlci(struct prh_rfcomm_se_user **port, t_bdaddr bdaddr, prh_t_rfcomm_dlci dlci)
{
  int ret;
  struct prh_rfcomm_se_user *returned;
  struct prh_st_rfcomm_lookupstructure lookfor;
  
  lookfor.dlci=dlci;
  lookfor.bdaddr=bdaddr;

  ret=prh_host_ll_remove(&prh_rfcomm_se_head, (void *)&returned, (void *)(u_int32)&lookfor, &prh_rfcomm_se_addrdlci_cmp);
  if (ret==BT_NOERROR)
    *port=returned;
  return ret;
}

int prh_rfcomm_se_remove_all_stack_entity_helper(void *data, void *func_info)
{
  struct prh_rfcomm_se_user *inst=(struct prh_rfcomm_se_user *)data;
  pFree(inst);
  return 0;
}

int prh_rfcomm_se_remove_all_stack_entity(void)
{
  int ret;

  ret=prh_host_ll_remove_all(&prh_rfcomm_se_head, &prh_rfcomm_se_remove_all_stack_entity_helper, NULL);
  return ret;
}
#endif
