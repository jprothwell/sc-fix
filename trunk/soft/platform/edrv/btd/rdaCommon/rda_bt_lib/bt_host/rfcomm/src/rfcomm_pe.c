/***********************************************************************
 *
 * MODULE NAME:    rfcomm_pe.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Generic Port Entity code
 * MAINTAINER:     David Airlie
 * CREATION DATE:  1 May 2000
 *
 * SOURCE CONTROL: $Id: rfcomm_pe.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * NOTES:
 *  This file contains functions that are part of the port entity
 *  but generic to every OS. They are to be considered helper functions
 *  and a new port entity need not use these functions.
 *
 ***********************************************************************/
#include "rfcomm_lower_layer.h"

#if PRH_RFCOMM_PE_SUPPORT == 1

#include "rfcomm_ue_api.h"
#include "rfcomm_core.h"
#include "rfcomm_debug.h"
#include "rfcomm_proto.h"

t_api port_ent_multi_start_conf_cb(t_bdaddr bd_addr, prh_t_rfcomm_start_sys_params *system_parameters, prh_t_rfcomm_accept accept, void *rfcomm_inst);
t_api port_ent_dlc_estab_ind_cb(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, void *multi_inst);
t_api port_ent_dlc_rel_ind_cb(prh_t_rfcomm_dlci dlci, void *multi_inst);
t_api port_ent_dlc_data_ind_cb(prh_t_rfcomm_dlci DLCI, struct host_buf *buffer, void *multi_inst);
t_api port_ent_dlc_linestat_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_line_stat_params ls_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype);
t_api port_ent_dlc_portneg_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_port_params *port_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype);
t_api port_ent_dlc_control_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_control_params *ctrl_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype);
t_api port_ent_dlc_flow_ind_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_flow_state flow_state, void *multi_inst);
t_api port_ent_dlc_paramneg_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype);
t_api port_ent_dlc_credit_flow_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_flow_state flow_state, void *multi_inst);

/* handle for the RFCOMM system wide instance */
extern void *pe_rfcomm_mgr_handle;


/* structure used to hold a semaphore and status for a multiplexer start */
void prh_rfcomm_init_pe_os_struct(struct prh_rfcomm_pe_port *pe_info);

void prh_rfcomm_init_pe_struct_params_default(struct prh_rfcomm_pe_port *pe_info)
{
  pe_info->agg_flow_state=PRH_RFCOMM_FLOW_DATA_GO;
  pe_info->local_flow_state=PRH_RFCOMM_FLOW_DATA_GO;

  pe_info->port_params.port_speed=RF9600;
  pe_info->port_params.data_bits=RFDS8;
  pe_info->port_params.stop_bits=RFSTOP1;
  pe_info->port_params.parity_enable=RFNONE;
  pe_info->port_params.paritytype=RFODD;
  pe_info->port_params.xon=0x11;
  pe_info->port_params.xoff=0x13;
  pe_info->port_params.flow_mask=0;
  pe_info->port_params.param_mask=0;
  pe_info->dlc_params.max_frame_size=127;
  pe_info->dlci=0;
}

void prh_rfcomm_init_pe_struct(struct prh_rfcomm_pe_port *pe_info)
{
  prh_rfcomm_init_pe_struct_params_default(pe_info);
  prh_rfcomm_init_pe_os_struct(pe_info);
}

void prh_rfcomm_free_pe_os_struct(struct prh_rfcomm_pe_port *pe_info);

void prh_rfcomm_free_pe_struct(struct prh_rfcomm_pe_port *pe_info)
{
  if (pe_info->portname)
    pFree(pe_info->portname);
  prh_rfcomm_free_pe_os_struct(pe_info);
  pFree(pe_info);
}

/*
 * prh_rfcomm_pe_connect_port
 * -- Port Entity DLC Establish helper function
 *
 * Allocate a Port entity structure, sets it up, 
 * finds a multiplexer, (either gets current one or starts one)
 * Sets up callbacks, sends Parameter negotiation, and then does 
 * the DLC establish.
 */
int prh_rfcomm_pe_connect_port(t_bdaddr remote_bd_addr, prh_t_rfcomm_schan rem_schan, const char *pName, prh_t_rfcomm_dlci *dlci, struct prh_rfcomm_pe_port **pe_info_ptr, u_int16 max_frame_size,  void (*port_callback)(prh_t_rfcomm_schan schan, t_bdaddr bd_addr, u_int8 connection_state))
{
  t_api ret;
  struct prh_rfcomm_pe_port *pe_info;
  struct prh_rfcomm_dlc_cb dlc_cbs;  

  /* Allocate memory for pe_info */
  pe_info=(struct prh_rfcomm_pe_port *)pMalloc(sizeof(struct prh_rfcomm_pe_port));
  if (pe_info==pNULL)
    return BT_NORESOURCES;
  pMemset(pe_info, 0, sizeof(struct prh_rfcomm_pe_port));

  dlc_cbs.dlc_estab_ind=port_ent_dlc_estab_ind_cb;
  dlc_cbs.dlc_estab_conf=port_ent_dlc_estab_conf_cb;
  dlc_cbs.dlc_rel_ind=port_ent_dlc_rel_ind_cb;
  dlc_cbs.dlc_data_ind=port_ent_dlc_data_ind_cb;
  dlc_cbs.linestat=port_ent_dlc_linestat_cb;
  dlc_cbs.portneg=port_ent_dlc_portneg_cb;
  dlc_cbs.control=port_ent_dlc_control_cb;
  dlc_cbs.agg_flow_ind=port_ent_dlc_flow_ind_cb;
  dlc_cbs.dlc_paramneg=port_ent_dlc_paramneg_cb;
  dlc_cbs.credit_flow_ind=port_ent_dlc_credit_flow_cb;


  pe_info->portname=(char *)pMalloc(pStrlen(pName)+1);
  if (pe_info->portname==pNULL)
    {
      ret=BT_NORESOURCES;
      goto out_free_pe_struct;
    }

  /* copy in portname */
  pStrcpy(pe_info->portname, pName);

  prh_rfcomm_init_pe_struct(pe_info);
  pe_info->port_cbfunc=port_callback;
  pe_info->remote_device_addr=remote_bd_addr;
  pe_info->portnum=0;
  pe_info->dlc_params.max_frame_size=max_frame_size;
  pe_info->dlc_params.local_credits=PRH_RFCOMM_INITIAL_CREDITS;

  *pe_info_ptr=pe_info;

  /* This has been split into to separate functions for P6
      1. Allocate the port, this checks the port is available for use.
         if not it fails here.
      2. If the port is available later PE_OS_CliConnectPort_Enable is
         called to enable the port for use.
	 
	 If no enable mechanism is require only Allocate needs to be called.
  */
    
  ret=PE_OS_CliConnectPort_Allocate(pe_info);
  if (ret!=BT_NOERROR)
    {
      goto out_free_pe_struct;
    }
  
  /* Lock out the callback and other connect req from touching pending callbacks lists */  
  
  ret=prh_rfcomm_pe_add_port_entity(pe_info);
  if (ret!=BT_NOERROR)
    {
      goto out_free_pe_struct;
    }
  
  ret=prh_rfcomm_entity_connect_port(remote_bd_addr, rem_schan, &pe_info->dlci, &pe_info->dlc_params, &dlc_cbs, &pe_info->multi_inst);
  if (ret!=BT_PENDING && ret!=BT_NOERROR)
    {
      struct prh_rfcomm_pe_port *pe_info_ptr;
      int status;
      
      status=prh_rfcomm_pe_remove_port_entity_by_addr_and_dlci(&pe_info_ptr, pe_info->remote_device_addr, pe_info->dlci);
      if (status==BT_NOERROR)
	{
	  PE_OS_CliConnectPort_DeAllocate(pe_info);
	  goto out_free_pe_struct;
	}
    }
  else
    /* always return BT_PENDING in non-error case to upper layer */
    ret=BT_PENDING;

  return ret;
 out_free_pe_struct:
  prh_rfcomm_free_pe_struct(pe_info);
  return ret;
}

/*
 * prh_rfcomm_pe_disconnect_remote_serverchannel
 *
 * Helper function to disconnect a remote server channel that we
 * previously connected to.
 */
t_api prh_rfcomm_pe_disconnect_remote_serverchannel(t_bdaddr remote_bd_addr, prh_t_rfcomm_schan serverchannel, struct prh_rfcomm_pe_port **pe_info_ptr)
{
  int ret;
  int dbit;
  struct prh_rfcomm_l2cap_inst *l2cap_inst;
  struct prh_rfcomm_pe_port *pe_info;
  prh_t_rfcomm_dlci dlci;
  
  ret=PRH_RFCOMM_UE_Get_Multiplexer_State(remote_bd_addr, (void **)&l2cap_inst);
  if (ret==BT_NOERROR)
    {
      dbit=l2cap_inst->initiating;
      
      /* create DLCI from dbit/serverchannel */
      dlci=(!dbit) | (serverchannel<<1);
      
      ret=prh_rfcomm_pe_lookup_port_entity_by_addr_and_dlci(&pe_info, remote_bd_addr, dlci);
      if (ret==BT_NOERROR)
	{
	  ret=PRH_RFCOMM_UE_DLC_Rel_Req(pe_info->dlci, pe_info->multi_inst);
	  if (ret==BT_NOERROR)
	    {
	      ret=prh_rfcomm_pe_remove_port_entity_by_addr_and_dlci(&pe_info, remote_bd_addr, dlci);
	      if (ret==BT_NOERROR)
		*pe_info_ptr=pe_info;
	      
	    }
	  return ret;
	}
      else
	return BT_INVALIDPARAM;
    }
  else
    return BT_INVALIDPARAM;

}

#if 0
/* 
 * prh_rfcomm_pe_disconnect_local_server_channel
 */
int prh_rfcomm_pe_disconnect_local_server_channel(struct prh_rfcomm_pe_port **pe_info_ptr, prh_t_rfcomm_schan schan)
{
  struct prh_rfcomm_pe_port *pe_info;
  int ret;
  ret=prh_rfcomm_pe_lookup_port_entity_by_portnum(&pe_info, schan);
  if (ret==BT_NOERROR)
    {
      PRH_RFCOMM_UE_DLC_Rel_Req(pe_info->dlci, pe_info->multi_inst);
      prh_rfcomm_init_pe_struct_params_default(pe_info);
      *pe_info_ptr=pe_info;
      return BT_NOERROR;
    }
  else
    return BT_INVALIDPARAM;
}
#endif
/*
 * port_ent_dlc_estab_ind_cb
 * -- Port Ent Callback for DLC Establishment Indiciations
 *
 * When a DLC is established on a multiplexer this callback is called
 * with the parameters for the DLC. It checks the server channel is registered
 * and responds with an accept or reject depending, and sends an MSC after
 * accepting.
 */
t_api port_ent_dlc_estab_ind_cb(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, void *multi_inst)
{
  int ret;
  u_int8 accept=1;
  prh_t_rfcomm_schan loc_chan;
  struct prh_rfcomm_pe_port *pe_info;
  prh_t_rfcomm_control_params control_params;

  /* Search for this in the port entity... extract server channel from dlci */
  loc_chan=dlci>>1;

  /* Need to check is it a registered server channel */
  /* Return 0 for no channel, 1 if a channel exists */

  ret=prh_rfcomm_pe_lookup_port_entity_by_portnum(&pe_info, loc_chan);
  if (ret==BT_NOERROR)
    {
      /* Has it a DLCI already assigned? if so it is connected */
      if (pe_info->dlci!=0)
	accept=0;
      else
	{
	  if (dlci>0)
	    pe_info->dlci=dlci;
	  pe_info->multi_inst=multi_inst;
	  pe_info->remote_device_addr=bd_addr;
	  
	  accept=1;
	  if (pe_info->port_cbfunc)
	    (*pe_info->port_cbfunc)((prh_t_rfcomm_schan)pe_info->portnum, bd_addr, PRH_PORT_ENT_CONNECT);
	}
    }
  else
    {
      accept=0;
    }

  ret=PRH_RFCOMM_UE_DLC_Estab_Resp(bd_addr, dlci, &pe_info->dlc_params, accept, multi_inst);
  if (ret==BT_NOERROR)
    {
      if (accept!=0)
	{
	  control_params.flow_state=PRH_RFCOMM_FLOW_DATA_GO;
	  control_params.break_signal=0;
	  control_params.modem_signal=PRH_RFCOMM_MSC_DTRDSR | PRH_RFCOMM_MSC_RTSCTS | PRH_RFCOMM_MSC_DCD;
	  
	  PRH_RFCOMM_UE_Control_Req(dlci, &control_params, multi_inst);
	}
    }
  
  return 0;
}

/*
 * port_ent_dlc_estab_conf_cb
 * -- DLC Estab confirm callback
 * 
 */
t_api port_ent_dlc_estab_conf_cb(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, prh_t_rfcomm_accept accept, void *multi_inst)
{
  /* Need to lookup DLCI / BD Address and call callback */
  struct prh_rfcomm_pe_port *pe_info;
  prh_t_rfcomm_control_params control_params;
  int ret;

  ret=prh_rfcomm_pe_lookup_port_entity_by_addr_and_dlci(&pe_info, bd_addr, dlci);
  if (ret==BT_NOERROR)
    {
      if (pe_info->port_cbfunc)
	{
	  if (accept)
	    {
	      ret=PE_OS_CliConnectPort_Enable(pe_info);
	      if (ret==BT_NOERROR)
		{

		  /* Should send an MSC to peer after we are connected */
		  control_params.flow_state=PRH_RFCOMM_FLOW_DATA_GO;
		  control_params.break_signal=0;
		  control_params.modem_signal=PRH_RFCOMM_MSC_DTRDSR | PRH_RFCOMM_MSC_RTSCTS | PRH_RFCOMM_MSC_DCD;
		  
		  PRH_RFCOMM_UE_Control_Req(dlci, &control_params, multi_inst);
		  (*pe_info->port_cbfunc)((prh_t_rfcomm_schan)(pe_info->dlci>>1), bd_addr, PRH_PORT_ENT_CONNECT);
		}
	      else
		goto out_connect_failed;
	    }
	  else
	    goto out_connect_failed;
	}

    }

  return BT_NOERROR;
out_connect_failed:
  {
    void (*_cbfunc)(prh_t_rfcomm_schan, t_bdaddr, u_int8) = pe_info->port_cbfunc;
    prh_t_rfcomm_schan _schan = pe_info->dlci >> 1;
    struct prh_rfcomm_pe_port *pe_info_ptr;
    
    prh_rfcomm_pe_remove_port_entity_by_addr_and_dlci(&pe_info_ptr, pe_info->remote_device_addr, pe_info->dlci);
    PE_OS_CliConnectPort_DeAllocate(pe_info);
    prh_rfcomm_free_pe_struct(pe_info);
    (*_cbfunc)(_schan, bd_addr, PRH_PORT_ENT_CONNECTFAILED);
  }
  return BT_NOERROR;
}

/* 
 * Callback for when remote wants to reneg port values
 *
 * This programs the received parameters down to the serial port... - P6 Release.
 */
t_api port_ent_dlc_portneg_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_port_params *port_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype)
{
  /* Need to write the values received from the remote to our port? */
  prh_t_rfcomm_port_params *resp_params;
  struct prh_rfcomm_pe_port *pe_info;
  int ret;
  struct prh_rfcomm_l2cap_inst *l2cap_inst=(struct prh_rfcomm_l2cap_inst *)multi_inst;
  u_int8 dbit;
  
  ret=prh_rfcomm_pe_lookup_port_entity_by_addr_and_dlci(&pe_info, l2cap_inst->bd_addr, dlci);
  if (ret!=BT_NOERROR)
    {
      /* If not check is a local server channel */
      dbit=l2cap_inst->initiating;
      if (!((dlci&1)^(dbit&1)))
	{
	  ret=prh_rfcomm_pe_lookup_port_entity_by_portnum(&pe_info, dlci>>1);
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
      resp_params=&pe_info->port_params;
    }
      else
    {
      /* Decode the parameter mask bits */
      if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_BITRATE)
          pe_info->port_params.port_speed=port_params->port_speed;

      if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_DATABITS)
          pe_info->port_params.data_bits=port_params->data_bits;

      if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_STOPBITS)
          pe_info->port_params.stop_bits=port_params->stop_bits;
      
      if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_PARITY)
          pe_info->port_params.parity_enable=port_params->parity_enable;

      if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_PARITY_TYPE)
	  pe_info->port_params.paritytype=port_params->paritytype;

      if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_XON_CH)
	pe_info->port_params.xon=port_params->xon;

      if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_XOFF_CH)
          pe_info->port_params.xoff=port_params->xoff;

      if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_XONXOFF_INPUT)
        {
          if (port_params->flow_mask & PRH_RPN_FLOW_INPUT_XONXOFF)
	      pe_info->port_params.flow_mask |= PRH_RPN_FLOW_INPUT_XONXOFF;
          else
	      pe_info->port_params.flow_mask &= ~PRH_RPN_FLOW_INPUT_XONXOFF;
	  
	}

      if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_XONXOFF_OUTPUT)
        {
          if (port_params->flow_mask & PRH_RPN_FLOW_OUTPUT_XONXOFF)
	      pe_info->port_params.flow_mask |= PRH_RPN_FLOW_OUTPUT_XONXOFF;
          else
	      pe_info->port_params.flow_mask &= ~PRH_RPN_FLOW_OUTPUT_XONXOFF;
        }

      if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_RTR_INPUT)
        {
          if (port_params->flow_mask & PRH_RPN_FLOW_INPUT_RTR)
	      pe_info->port_params.flow_mask |= PRH_RPN_FLOW_INPUT_RTR;
          else
	      pe_info->port_params.flow_mask &= ~PRH_RPN_FLOW_INPUT_RTR;
        }
      
      if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_RTR_OUTPUT)
        {
          if (port_params->flow_mask & PRH_RPN_FLOW_OUTPUT_RTR)
	      pe_info->port_params.flow_mask |= PRH_RPN_FLOW_OUTPUT_RTR;
          else
	      pe_info->port_params.flow_mask &= ~PRH_RPN_FLOW_OUTPUT_RTR;
        } 
      
      if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_RTC_INPUT)
        {
          if (port_params->flow_mask & PRH_RPN_FLOW_INPUT_RTC)
	      pe_info->port_params.flow_mask |= PRH_RPN_FLOW_INPUT_RTC;
          else
	      pe_info->port_params.flow_mask &= ~PRH_RPN_FLOW_INPUT_RTC;
        }
      
      if (port_params->param_mask & PRH_RFCOMM_CC_RPN_PM_RTC_OUTPUT)
        {
          if (port_params->flow_mask & PRH_RPN_FLOW_OUTPUT_RTC)
	      pe_info->port_params.flow_mask |= PRH_RPN_FLOW_OUTPUT_RTC;
          else
	      pe_info->port_params.flow_mask &= ~PRH_RPN_FLOW_OUTPUT_RTC;
        }

      resp_params=&pe_info->port_params;
      resp_params->param_mask=port_params->param_mask;
      
      /* call separate function to update port configuration */
      /* this can modify what it wants before it is returned to the remote device */
      port_ent_set_port_config(pe_info);

    }
      PRH_RFCOMM_UE_Portneg_Resp(dlci, resp_params, PRH_RFCOMM_ACCEPT, multi_inst);
      resp_params->param_mask=0;
    }
  return BT_NOERROR;
}


/*
 * Linked list of Port Entitys
 * Handler functions
 */
struct prh_host_gen_ll *prh_rfcomm_pe_head;

int prh_rfcomm_pe_add_port_entity(struct prh_rfcomm_pe_port *self)
{
  int ret;
  ret=prh_host_ll_add(&prh_rfcomm_pe_head, (void *)self);
  return ret;
}

int prh_rfcomm_pe_portnum_cmp(void *cmpin, void *compwith)
{
  struct prh_rfcomm_pe_port *pe_info=(struct prh_rfcomm_pe_port *)compwith;
  u_int32 portnum=(u_int32)cmpin;

  return (pe_info->portnum==portnum);
}


int prh_rfcomm_pe_remove_port_entity_by_portnum(struct prh_rfcomm_pe_port **port, u_int32 portnum)
{
  int ret;
  ret=prh_host_ll_remove(&prh_rfcomm_pe_head, (void **)port, (void *)(u_int32)portnum, &prh_rfcomm_pe_portnum_cmp);
  return ret;
}

int prh_rfcomm_pe_lookup_port_entity_by_portnum(struct prh_rfcomm_pe_port **port, u_int32 portnum)
{
  int ret;
  struct prh_rfcomm_pe_port *returned;

  ret=prh_host_ll_search(&prh_rfcomm_pe_head, (void *)&returned, (void *)(u_int32)portnum, &prh_rfcomm_pe_portnum_cmp);
  if (ret==BT_NOERROR)
    *port=returned;
  return ret;
}

struct prh_st_rfcomm_lookupstructure 
{
  t_bdaddr bdaddr;
  prh_t_rfcomm_dlci dlci;
};

int prh_rfcomm_pe_addrdlci_cmp(void *cmpin, void *compwith)
{
  struct prh_rfcomm_pe_port *pe_info=(struct prh_rfcomm_pe_port *)compwith;
  struct prh_st_rfcomm_lookupstructure *addrdlci=(struct prh_st_rfcomm_lookupstructure *)cmpin;
  int ret;

  ret=((BD_ADDR_CMP(addrdlci->bdaddr, pe_info->remote_device_addr)) && (addrdlci->dlci==pe_info->dlci));
  return ret; 
}

int prh_rfcomm_pe_lookup_port_entity_by_addr_and_dlci(struct prh_rfcomm_pe_port **port, t_bdaddr bdaddr, u_int8 dlci)
{
  int ret;
  struct prh_rfcomm_pe_port *returned;
  struct prh_st_rfcomm_lookupstructure lookfor;
  
  lookfor.dlci=dlci;
  lookfor.bdaddr=bdaddr;

  ret=prh_host_ll_search(&prh_rfcomm_pe_head, (void *)&returned, (void *)(u_int32)&lookfor, &prh_rfcomm_pe_addrdlci_cmp);
  if (ret==BT_NOERROR)
    *port=returned;
  return ret;
}

int prh_rfcomm_pe_remove_port_entity_by_addr_and_dlci(struct prh_rfcomm_pe_port **port, t_bdaddr bdaddr, u_int8 dlci)
{
  int ret;
  struct prh_rfcomm_pe_port *returned;
  struct prh_st_rfcomm_lookupstructure lookfor;
  
  lookfor.dlci=dlci;
  lookfor.bdaddr=bdaddr;

  ret=prh_host_ll_remove(&prh_rfcomm_pe_head, (void *)&returned, (void *)(u_int32)&lookfor, &prh_rfcomm_pe_addrdlci_cmp);
  if (ret==BT_NOERROR)
    *port=returned;
  return ret;
}

int prh_rfcomm_pe_remove_all_port_entity_helper(void *data, void *func_info)
{
  struct prh_rfcomm_pe_port *inst=(struct prh_rfcomm_pe_port *)data;

  PE_OS_ReleasePort(inst);
  prh_rfcomm_free_pe_struct(inst);
  return BT_NOERROR;
}

int prh_rfcomm_pe_remove_all_port_entity(void)
{
  int ret;
  
  ret=prh_host_ll_remove_all(&prh_rfcomm_pe_head, &prh_rfcomm_pe_remove_all_port_entity_helper, NULL);
  
  return ret;
}

#endif
