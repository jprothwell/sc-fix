/***********************************************************************
 *
 * MODULE NAME:    stack_ent_api.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    RSE API code
 * MAINTAINER:     David Airlie
 * CREATION DATE:  1 May 2001
 *
 * SOURCE CONTROL: $Id: stack_ent_api.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Inc.
 *     All rights reserved.
 *
 ***********************************************************************/
#include "rfcomm_lower_layer.h"

#if PRH_RFCOMM_RSE_SUPPORT==1
#include "rfcomm_ue_api.h"
#include "rfcomm_debug.h"
#include "rfcomm_core.h"
#include "stack_ent_core.h"

int prh_rfcomm_entity_connect_port(t_bdaddr remote_bd_addr, prh_t_rfcomm_schan rem_schan, prh_t_rfcomm_dlci *dlci, prh_t_rfcomm_dlc_params *dlc_params, struct prh_rfcomm_dlc_cb *cbs, void **multi_inst_ptr);

t_api prh_stack_ent_dlc_estab_conf_cb(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, prh_t_rfcomm_accept accept, void *multi_inst);
t_api prh_stack_ent_dlc_estab_ind_cb(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, void *multi_inst);
t_api prh_stack_ent_dlc_rel_ind_cb(prh_t_rfcomm_dlci dlci, void *multi_inst);
t_api prh_stack_ent_dlc_data_ind_cb(prh_t_rfcomm_dlci DLCI, struct host_buf *buffer, void *multi_inst);
t_api prh_stack_ent_dlc_linestat_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_line_stat_params ls_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype);
t_api prh_stack_ent_dlc_portneg_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_port_params *port_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype);
t_api prh_stack_ent_dlc_control_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_control_params *ctrl_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype);
t_api prh_stack_ent_dlc_flow_ind_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_flow_state flow_state, void *multi_inst);
t_api prh_stack_ent_dlc_paramneg_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype);
t_api prh_stack_ent_dlc_credit_flow_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_flow_state flow_state, void *multi_inst);

t_api prh_rfcomm_entity_init(u_int32 flags);

t_api prh_rse_layer_ctrl(u_int8 flags)
{
  if ((flags & PRH_HOST_SHUTDOWN) || (flags & PRH_HOST_RESET) || (flags & PRH_HOST_STARTUP))  //zhu jianguo changed it. 2008.12.5
    prh_rfcomm_se_remove_all_stack_entity();
  if ((flags & PRH_HOST_STARTUP) || (flags & PRH_HOST_RESET))
    prh_rfcomm_entity_init(flags);

  return BT_NOERROR;
}

#if COMBINED_HOST==1
struct prh_rfcomm_se_user rfcomm_se_user[7];
int rse_num=0;
#endif
APIDECL1 t_api APIDECL2 RSE_SrvRegisterPort(prh_t_rfcomm_schan *serv_chan, struct st_t_RSE_Callbacks *cbs)
{
  struct prh_rfcomm_se_user *se_info;
  struct prh_rfcomm_dlc_cb dlc_cbs;
  int ret=0;
  int portnum;

#if COMBINED_HOST==1
    //zhu Jianguo added it. 2008.11.18
    se_info = &rfcomm_se_user[rse_num];
    rse_num++;
#endif
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

#if COMBINED_HOST==0
  se_info=(struct prh_rfcomm_se_user *)pMalloc(sizeof(struct prh_rfcomm_se_user));
#endif
  if (se_info==NULL)
    {
      ret=BT_NORESOURCES;
      goto err_out_dealloc;
    }

  pMemset(se_info, 0, sizeof(struct prh_rfcomm_se_user));
  prh_rfcomm_init_se_struct(se_info);
  se_info->cb=*cbs;
  se_info->portnum=portnum;
  
  dlc_cbs.dlc_estab_ind=prh_stack_ent_dlc_estab_ind_cb;
  dlc_cbs.dlc_estab_conf=prh_stack_ent_dlc_estab_conf_cb;
  dlc_cbs.dlc_rel_ind=prh_stack_ent_dlc_rel_ind_cb;
  dlc_cbs.dlc_data_ind=prh_stack_ent_dlc_data_ind_cb;
  dlc_cbs.linestat=prh_stack_ent_dlc_linestat_cb;
  dlc_cbs.portneg=prh_stack_ent_dlc_portneg_cb;
  dlc_cbs.control=prh_stack_ent_dlc_control_cb;
  dlc_cbs.agg_flow_ind=prh_stack_ent_dlc_flow_ind_cb;
  dlc_cbs.dlc_paramneg=prh_stack_ent_dlc_paramneg_cb;
  dlc_cbs.credit_flow_ind=prh_stack_ent_dlc_credit_flow_cb;

  ret=prh_rfcomm_se_add_stack_entity(se_info);
  if (ret==BT_NORESOURCES)
    {
      goto err_out_free_info;
    }
  
  ret=PRH_RFCOMM_UE_Register_Server(*serv_chan, &dlc_cbs);
  if (ret!=BT_NOERROR)
    {
      prh_rfcomm_se_remove_stack_entity_by_portnum(&se_info, *serv_chan);
      goto err_out_free_info;
    }
  
  return ret;

 err_out_free_info:
  pFree(se_info);
 err_out_dealloc:
  PRH_RFCOMM_UE_Deallocate_Server_Channel(*serv_chan);
  return ret;
}


APIDECL1 t_api APIDECL2 RSE_SrvDeregisterPort(prh_t_rfcomm_schan schan)
{
  struct prh_rfcomm_se_user *se_info;
  int ret;
  if (schan==0 || schan>PRH_RFCOMM_MAX_SCHAN)
    return BT_INVALIDPARAM;
  
  /* need to lookup by port num here */
  ret=prh_rfcomm_se_remove_stack_entity_by_portnum(&se_info, schan);
  if (ret==BT_NOERROR)
    {
      if (se_info->dlci)
	{
	  PRH_RFCOMM_UE_DLC_Rel_Req(se_info->dlci, se_info->multi_inst);
	}
      PRH_RFCOMM_UE_Deregister_Server(schan);
      pFree(se_info);
      ret=BT_NOERROR;
    }
  else
    ret=BT_INVALIDPARAM;
  return ret;
}

APIDECL1 t_api APIDECL2 RSE_SrvDisconnectPort(prh_t_rfcomm_schan schan)
{
  struct prh_rfcomm_se_user *se_info;
  int ret;
  if (schan==0 || schan>PRH_RFCOMM_MAX_SCHAN)
    return BT_INVALIDPARAM;
  
  ret=prh_rfcomm_se_lookup_stack_entity_by_portnum(&se_info, schan);
  if (ret==BT_NOERROR)
    {
      if (se_info->dlci>0)
	{
	  PRH_RFCOMM_UE_DLC_Rel_Req(se_info->dlci, se_info->multi_inst);
	  prh_rfcomm_init_se_struct(se_info);
	  se_info->multi_inst=NULL;
	  ret=BT_NOERROR;
	}
      else
	ret=BT_INVALIDPARAM;
    }
  else
    ret=BT_INVALIDPARAM;
  return ret;
}

APIDECL1 t_api APIDECL2 RSE_CliConnectPort(t_bdaddr remote_bd_addr, prh_t_rfcomm_schan rem_schan, u_int16 max_frame_size, struct st_t_RSE_Callbacks *cbs)
{
  struct prh_rfcomm_se_user *se_info;
  t_api ret;
  struct prh_rfcomm_dlc_cb dlc_cbs;  

  /* Allocate memory for pe_info */
#if COMBINED_HOST==0
  se_info=(struct prh_rfcomm_se_user *)pMalloc(sizeof(struct prh_rfcomm_se_user));
#endif
  if (se_info==pNULL)
    return BT_NORESOURCES;
  pMemset(se_info, 0, sizeof(struct prh_rfcomm_se_user));
  dlc_cbs.dlc_estab_ind=prh_stack_ent_dlc_estab_ind_cb;
  dlc_cbs.dlc_estab_conf=prh_stack_ent_dlc_estab_conf_cb;
  dlc_cbs.dlc_rel_ind=prh_stack_ent_dlc_rel_ind_cb;
  dlc_cbs.dlc_data_ind=prh_stack_ent_dlc_data_ind_cb;
  dlc_cbs.linestat=prh_stack_ent_dlc_linestat_cb;
  dlc_cbs.portneg=prh_stack_ent_dlc_portneg_cb;
  dlc_cbs.control=prh_stack_ent_dlc_control_cb;
  dlc_cbs.agg_flow_ind=prh_stack_ent_dlc_flow_ind_cb;
  dlc_cbs.dlc_paramneg=prh_stack_ent_dlc_paramneg_cb;
  dlc_cbs.credit_flow_ind=prh_stack_ent_dlc_credit_flow_cb;

  prh_rfcomm_init_se_struct(se_info);
  se_info->cb=*cbs;
  se_info->remote_device_addr=remote_bd_addr;
  se_info->portnum=0;
  se_info->dlc_params.max_frame_size=max_frame_size;

  se_info->dlc_params.local_credits=PRH_RFCOMM_INITIAL_CREDITS;

  ret=prh_rfcomm_se_add_stack_entity(se_info);
  if (ret==BT_NORESOURCES)
    {
      pFree(se_info);
      return BT_NORESOURCES;
    }

  /* Call the multiplexer control entity to do the connection setup */
  ret=prh_rfcomm_entity_connect_port(remote_bd_addr, rem_schan, &se_info->dlci, &se_info->dlc_params, &dlc_cbs, &se_info->multi_inst);
  pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"RSE_CliConnectPort aft prh_rfcomm_entity_connect_port ret = %d, se_info->dlci=%d\n", ret,se_info->dlci));
  if (ret!=BT_PENDING && ret!=BT_NOERROR)
    {
      struct prh_rfcomm_se_user *se_info_ptr;
      int status;

      status=prh_rfcomm_se_remove_stack_entity_by_addr_and_dlci(&se_info_ptr, se_info->remote_device_addr, se_info->dlci);
      if (status==BT_NOERROR)
	{
		pFree(se_info);	
	}
    }
  else
    ret=BT_PENDING; /* always return pending to the upper layer */
  return ret;
  
}

APIDECL1 t_api APIDECL2 RSE_CliDisconnectPort(t_bdaddr remote_bd_addr, prh_t_rfcomm_schan schan)
{
  struct prh_rfcomm_se_user *se_info;
  int ret;
  int dbit;
  struct prh_rfcomm_l2cap_inst *l2cap_inst;
  prh_t_rfcomm_dlci dlci;

  ret=PRH_RFCOMM_UE_Get_Multiplexer_State(remote_bd_addr, (void **)&l2cap_inst);
  if (ret==BT_NOERROR)
    {
      dbit=l2cap_inst->initiating;
      
      /* create DLCI from dbit/schan */
      dlci=(!dbit) | (schan<<1);

      ret=prh_rfcomm_se_lookup_stack_entity_by_addr_and_dlci(&se_info, remote_bd_addr, dlci);
      if (ret==BT_NOERROR)
	{
	  ret=PRH_RFCOMM_UE_DLC_Rel_Req(se_info->dlci, se_info->multi_inst);
	  if (ret==BT_NOERROR)
	    {
	      ret=prh_rfcomm_se_remove_stack_entity_by_addr_and_dlci(&se_info, remote_bd_addr, dlci);
	      if (ret==BT_NOERROR)
		pFree(se_info);
	    }
	}
      else
	ret=BT_INVALIDPARAM;
    }
  else
    ret=BT_INVALIDPARAM;
  return ret;
}

APIDECL1 t_api APIDECL2 RSE_Write(t_bdaddr remote_bd_addr, prh_t_rfcomm_dlci dlci, struct host_buf *p_buf)
{
  int ret;
  struct prh_rfcomm_se_user *se_info;
  pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"RSE_Write dlci=%d\n", dlci));
  ret=prh_rfcomm_se_lookup_stack_entity_by_addr_and_dlci(&se_info, remote_bd_addr, dlci);
  pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"RSE_Write ret = %d, se_info->dlci=%d\n", ret,se_info->dlci));
  if (ret==BT_NOERROR)
    {
      /* Fix for bug_id 406 - return BT_INVALIDPARAM if data is larger than frame size */
      if (p_buf->len > se_info->dlc_params.max_frame_size)
      	{
		return BT_INVALIDPARAM;
      	}
      ret=PRH_RFCOMM_UE_Data_Req(se_info->dlci, p_buf, se_info->multi_inst);
    }
  return ret;
}

APIDECL1 t_api APIDECL2 RSE_SrvConnectResponse(t_bdaddr remote_bd_addr, prh_t_rfcomm_dlci dlci, u_int8 accept)
{
    struct prh_rfcomm_se_user *se_info;
    prh_t_rfcomm_control_params control_params;
    int ret;

    ret=prh_rfcomm_se_lookup_stack_entity_by_portnum(&se_info, dlci>>1);
    if (ret==BT_NOERROR)
    {
        ret=PRH_RFCOMM_UE_DLC_Estab_Resp(remote_bd_addr, dlci, &se_info->dlc_params, accept, se_info->multi_inst);
        if (ret==BT_NOERROR)
        {
            if (accept!=0)
            {
                /* Chuck out an MSC now also */
                control_params.flow_state=PRH_RFCOMM_FLOW_DATA_GO;
                control_params.break_signal=0;
                control_params.modem_signal=PRH_RFCOMM_MSC_DTRDSR | PRH_RFCOMM_MSC_RTSCTS | PRH_RFCOMM_MSC_DCD;

                PRH_RFCOMM_UE_Control_Req(se_info->dlci, &control_params, se_info->multi_inst);
            }
            else
                prh_rfcomm_init_se_struct(se_info); /* just reset the structure */
        }
        else
            prh_rfcomm_init_se_struct(se_info); /* just reset the structure */
    }
    else
        return BT_INVALIDPARAM;
    
    return ret;
}

APIDECL1 t_api APIDECL2 RSE_GetWriteBuffer(t_bdaddr bdaddr, int dlci, int length, struct host_buf **p_buf)
{
  return PRH_RFCOMM_UE_Get_Write_Buffer(bdaddr, (prh_t_rfcomm_dlci)dlci, (u_int16)length, 0, p_buf);
}

APIDECL1 t_api APIDECL2 RSE_FreeWriteBuffer(struct host_buf *frame)
{
  return PRH_RFCOMM_UE_Release_Write_Buffer(frame);
}

APIDECL1 t_api APIDECL2 RSE_GetCIDFromConnection(t_bdaddr bdaddr, prh_t_rfcomm_dlci dlci, u_int16 *cid)
{
  u_int16 internal_cid;
  struct prh_rfcomm_se_user *se_info;
  int ret;

  ret=prh_rfcomm_se_lookup_stack_entity_by_addr_and_dlci(&se_info, bdaddr, dlci);
  if (ret==BT_NOERROR)
    {
      ret=PRH_RFCOMM_UE_Get_L2CAP_CID_From_Multiplexer(se_info->multi_inst, &internal_cid);
      if (ret==BT_NOERROR)
	*cid=internal_cid;
    }
  return ret;

}

#if PRH_RFCOMM_RSE_DATA_PUMP_ENABLE==1

t_api prh_rse_data_pump_flow_ind(t_bdaddr address, prh_t_rfcomm_dlci dlci, u_int8 flow_state);
t_api prh_rse_data_pump_data_ind(t_bdaddr address, prh_t_rfcomm_dlci dlci, struct host_buf *p_buf);

APIDECL1 t_api APIDECL2 RSE_DataPumpTx(t_bdaddr bdaddr, int dlci, u_int8 pattern_num, u_int16 pkt_size, u_int32 num_packets)
{
  struct prh_rfcomm_se_user *se_info;
  int ret;
  
  ret=prh_rfcomm_se_lookup_stack_entity_by_addr_and_dlci(&se_info, bdaddr, dlci);
  if (ret==BT_NOERROR)
    {
      if (pkt_size > se_info->dlc_params.max_frame_size)
	return BT_INVALIDPARAM;
      
      /* time to start pumping */
      se_info->pre_data_pump_cbs = se_info->cb;
      
      se_info->cb.flowIndication = prh_rse_data_pump_flow_ind;
      /* need to replace some callbacks with pump ones */
      se_info->tx_pump_enabled=1;
      ret=prh_rse_data_pump_start(se_info, pattern_num, pkt_size, num_packets);
    }
  return ret;
}

APIDECL1 t_api APIDECL2 RSE_DataPumpTxStop(t_bdaddr bdaddr, int dlci)
{ 
  struct prh_rfcomm_se_user *se_info;
  int ret;
  
  ret=prh_rfcomm_se_lookup_stack_entity_by_addr_and_dlci(&se_info, bdaddr, dlci);
  if (ret==BT_NOERROR)
    {
      se_info->tx_pump_enabled=0;
    }
}

APIDECL1 t_api APIDECL2 RSE_DataPumpRxPrepare(t_bdaddr bdaddr, int dlci, u_int32 num_packets_report, void *reserved)
{
  struct prh_rfcomm_se_user *se_info;
  int ret;
  
  ret=prh_rfcomm_se_lookup_stack_entity_by_addr_and_dlci(&se_info, bdaddr, dlci);
  if (ret==BT_NOERROR)
    {
      se_info->rx_stats_lock=pMutexCreate(FALSE);
      /* time to start pumping */
      se_info->pre_data_pump_cbs = se_info->cb;
      
      se_info->cb.dataIndication = prh_rse_data_pump_data_ind;
      se_info->rx_pump_enabled=1;
    }
  return ret;
}

APIDECL1 t_api APIDECL2 RSE_DataPumpRxStop(t_bdaddr bdaddr, int dlci)
{
  struct prh_rfcomm_se_user *se_info;
  int ret;
  
  ret=prh_rfcomm_se_lookup_stack_entity_by_addr_and_dlci(&se_info, bdaddr, dlci);
  if (ret==BT_NOERROR)
    {
      /* time to start pumping */
      se_info->cb = se_info->pre_data_pump_cbs;
      se_info->rx_pump_enabled=0;
    }
  return ret;
}

APIDECL1 t_api APIDECL2 RSE_DataPumpRxGetStats(t_bdaddr bdaddr, int dlci, u_int32 *total_data, u_int32 *rx_time_diff)
{
  struct prh_rfcomm_se_user *se_info;
  int ret;
  
  ret=prh_rfcomm_se_lookup_stack_entity_by_addr_and_dlci(&se_info, bdaddr, dlci);
  if (ret==BT_NOERROR)
    {
      if (se_info->rx_pump_enabled==0)
	return BT_INVALIDPARAM;

      pMutexLock(se_info->rx_stats_lock);
      *total_data = se_info->rx_traffic;
      *rx_time_diff = pExGetTickDifference(se_info->rx_time_first, se_info->rx_time_last);
      pMutexUnlock(se_info->rx_stats_lock);
    }
  return ret;
}
#endif
#endif
