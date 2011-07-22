/***********************************************************************
 *
 * MODULE NAME:    port_ent_api.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Generic Port Entity API code
 * MAINTAINER:     David Airlie
 * CREATION DATE:  1 May 2000
 *
 * SOURCE CONTROL: $Id: port_ent_api.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Inc.
 *     All rights reserved.
 *
 *
 ***********************************************************************/
#include "rfcomm_lower_layer.h"

#if PRH_RFCOMM_PE_SUPPORT==1
#include "rfcomm_ue_api.h"
#include "rfcomm_debug.h"
#include "rfcomm_core.h"
#include "rfcomm_pe.h"

void prh_rfcomm_free_pe_struct(struct prh_rfcomm_pe_port *pe_info);
t_api port_ent_dlc_estab_conf_cb(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, prh_t_rfcomm_accept accept, void *multi_inst);
t_api port_ent_dlc_estab_ind_cb(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, void *multi_inst);
t_api port_ent_dlc_rel_ind_cb(prh_t_rfcomm_dlci dlci, void *multi_inst);
t_api port_ent_dlc_data_ind_cb(prh_t_rfcomm_dlci DLCI, struct host_buf *buffer, void *multi_inst);
t_api port_ent_dlc_linestat_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_line_stat_params ls_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype);
t_api port_ent_dlc_portneg_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_port_params *port_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype);
t_api port_ent_dlc_control_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_control_params *ctrl_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype);
t_api port_ent_dlc_flow_ind_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_flow_state flow_state, void *multi_inst);
t_api port_ent_dlc_paramneg_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype);
t_api port_ent_dlc_credit_flow_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_flow_state flow_state, void *multi_inst);

int prh_rfcomm_pe_rfstart_ind_cb(t_bdaddr bd_addr, prh_t_rfcomm_start_sys_params *system_parameters, prh_t_rfcomm_accept accept, void *multi_inst);

t_api prh_rfcomm_entity_init(u_int32 flags);

t_api prh_pe_layer_ctrl(u_int8 flags)
{
  if (flags & PRH_HOST_STARTUP)
    prh_rfcomm_entity_init(flags);
  if (flags & PRH_HOST_SHUTDOWN)
    prh_rfcomm_pe_remove_all_port_entity();
  return 0;
}


APIDECL1 t_api APIDECL2 PE_SrvRegisterPort(char *port, prh_t_rfcomm_schan *serv_chan, void (*port_callback)(prh_t_rfcomm_schan schan, t_bdaddr bd_addr, u_int8 connection_state))
{
  struct prh_rfcomm_pe_port *pe_info;
  struct prh_rfcomm_dlc_cb dlc_cbs;
  int ret=0;
  int portnum;

  /* three traps, illegal server channel number, 0 server channel number (user requested), 1-30 */
  if (*serv_chan>PRH_RFCOMM_MAX_SCHAN)
    return BT_INVALIDPARAM;
  
  /* Has user requested a server channel */
  if (*serv_chan==0)
    {
      ret=PRH_RFCOMM_UE_Allocate_Server_Channel(serv_chan);
      if (ret!=BT_NOERROR)
	return ret;
    }
  else
    {
      ret=PRH_RFCOMM_UE_Get_Channel_State(*serv_chan);
      if (ret==BT_NOERROR)
	/*Channel Exists already */
	return BT_INVALIDPARAM;
    }
  
  portnum=*serv_chan;  

  /* Add port to list of registered port numbers */
  pe_info=(struct prh_rfcomm_pe_port *)pMalloc(sizeof(struct prh_rfcomm_pe_port));
  if (pe_info==pNULL)
    {
      ret=BT_NORESOURCES;
      goto err_out_dealloc;
    }

  pMemset(pe_info, 0, sizeof(struct prh_rfcomm_pe_port));      
  prh_rfcomm_init_pe_struct(pe_info);
  
  pe_info->portname=(char *)pMalloc(pStrlen(port)+1);
  if (pe_info->portname==pNULL)
    {
      ret=BT_NORESOURCES;
      goto err_out_free_info;
    }
  
  pStrcpy(pe_info->portname, port);
  pe_info->port_cbfunc=port_callback;
  pe_info->portnum=portnum;
  
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
  
  ret=prh_rfcomm_pe_add_port_entity(pe_info);
  if (ret==BT_NORESOURCES)
    {
      goto err_out_free_info;
    }
  
  ret=PRH_RFCOMM_UE_Register_Server(*serv_chan, &dlc_cbs);
  if (ret!=BT_NOERROR)
    {
      goto err_out_remove_pe;
    }

  /* Do OS Specific Registering */
  ret=PE_OS_SrvRegisterPort(pe_info);
  if (ret!=BT_NOERROR)
    {
      PRH_RFCOMM_UE_Deregister_Server(*serv_chan);
      goto err_out_remove_pe;
    }
  return BT_NOERROR;
  
  /* handle different error conditions .. make sure to clean up */
 err_out_remove_pe:
  prh_rfcomm_pe_remove_port_entity_by_portnum(&pe_info, *serv_chan);
 err_out_free_info:
  prh_rfcomm_free_pe_struct(pe_info);
 err_out_dealloc:
  PRH_RFCOMM_UE_Deallocate_Server_Channel(*serv_chan);
  return ret;
}


APIDECL1 t_api APIDECL2 PE_SrvDeregisterPort(prh_t_rfcomm_schan schan)
{
  struct prh_rfcomm_pe_port *pe_info;
  int ret;
  if (schan==0 || schan>PRH_RFCOMM_MAX_SCHAN)
    return BT_INVALIDPARAM;
  
  /* need to lookup by port num here */
  ret=prh_rfcomm_pe_remove_port_entity_by_portnum(&pe_info, schan);
  if (ret==BT_NOERROR)
    {
      if (pe_info->dlci)
	{
	  PRH_RFCOMM_UE_DLC_Rel_Req(pe_info->dlci, pe_info->multi_inst);
	}
      PRH_RFCOMM_UE_Deregister_Server(schan);
      PE_OS_SrvDeregisterPort(pe_info);
      prh_rfcomm_free_pe_struct(pe_info);
      ret=BT_NOERROR;
    }
  else
    ret=BT_INVALIDPARAM;
  return ret;
}

APIDECL1 t_api APIDECL2 PE_SrvDisconnectPort(prh_t_rfcomm_schan schan)
{
  struct prh_rfcomm_pe_port *pe_info;
  int ret;

  if (schan==0 || schan>PRH_RFCOMM_MAX_SCHAN)
    return BT_INVALIDPARAM;
 
  ret=prh_rfcomm_pe_lookup_port_entity_by_portnum(&pe_info, schan);
  if (ret==BT_NOERROR)  
    {
      /* check the link is connected at all */
      if (pe_info->dlci>0)
	{
	  PE_OS_SrvDisconnectPort(pe_info);
	  PRH_RFCOMM_UE_DLC_Rel_Req(pe_info->dlci, pe_info->multi_inst);
	  prh_rfcomm_init_pe_struct_params_default(pe_info);
	  ret=BT_NOERROR;
	}
      else
	ret=BT_INVALIDPARAM;
      }
  return ret;
}

APIDECL1 t_api APIDECL2 PE_CliConnectPort(t_bdaddr remote_bd_addr, prh_t_rfcomm_schan rem_schan, char *pName, void (*port_callback)(prh_t_rfcomm_schan schan, t_bdaddr bd_addr, u_int8 connection_state))
{
  prh_t_rfcomm_dlci dlci;
  struct prh_rfcomm_pe_port *pe_info;
   /* Call connect port with semaphore */
  return prh_rfcomm_pe_connect_port(remote_bd_addr, rem_schan, pName, &dlci, &pe_info, PRH_RFCOMM_DEFAULT_MTU, port_callback);
}

APIDECL1 t_api APIDECL2 PE_CliDisconnectPort(t_bdaddr remote_bd_addr, prh_t_rfcomm_schan serverchannel)
{
  struct prh_rfcomm_pe_port *pe_info;
  int ret;

  
  /* need to do a lookup using BD address and remote server channel */
  /* we don't have the d bit so we can't tell that much at the moment */
  ret=prh_rfcomm_pe_disconnect_remote_serverchannel(remote_bd_addr, serverchannel, &pe_info);
  if (ret==BT_NOERROR)
    {
      PE_OS_CliDisconnectPort(pe_info);
      prh_rfcomm_free_pe_struct(pe_info);
      ret=BT_NOERROR;
    }
  else
    ret=BT_INVALIDPARAM;
  return ret;
}


APIDECL1 t_api APIDECL2 PE_GetCIDFromConnection(t_bdaddr bdaddr, prh_t_rfcomm_dlci dlci, u_int16 *cid)
{
  u_int16 internal_cid;
  struct prh_rfcomm_pe_port *pe_info;
  int ret;
  
  ret=prh_rfcomm_pe_lookup_port_entity_by_addr_and_dlci(&pe_info, bdaddr, dlci);
  if (ret==BT_NOERROR)
    {
      ret=PRH_RFCOMM_UE_Get_L2CAP_CID_From_Multiplexer(pe_info->multi_inst, &internal_cid);
      if (ret==BT_NOERROR)
	*cid=internal_cid;
    }
  return ret;
}
#endif
