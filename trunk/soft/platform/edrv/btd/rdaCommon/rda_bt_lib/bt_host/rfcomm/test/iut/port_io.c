/*
 * MODULE:         port_ent_em.c
 * MAINTAINERS:    Dave Airlie
 * DATE:           08 March 2000
 * SOURCE CONTROL: $Id: port_io.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "host_types.h"

#include "l2cap_primitives.h"
#include "papi.h"
#include "host_buf.h"
#include "rfcomm_ue_api.h"
#include "rfcomm_debug.h"

#include "rfcomm_core.h"
#include <errno.h>

extern void *prh_test_pe_rfcomm_mgr_handle;
struct prh_rfcomm_pe_port *prh_test_rfcomm_pe_sys;
void *prh_test_rfcomm_multi_inst;
t_api test_port_ent_dlc_rel_ind_cb(prh_t_rfcomm_dlci dlci, void *multi_inst);

int test_port_ent_dlc_data_ind_cb(prh_t_rfcomm_dlci DLCI, struct host_buf *buffer, void *multi_inst);
int test_port_ent_dlc_linestat_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_line_stat_params ls_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype);
int test_port_ent_dlc_portneg_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_port_params *port_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype);
t_api test_port_ent_dlc_estab_conf_cb(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, prh_t_rfcomm_accept accept, void *multi_inst);
t_api TEST_PRH_RFCOMM_UE_Start_Conf(t_bdaddr bd_addr, prh_t_rfcomm_start_sys_params *system_parameters, prh_t_rfcomm_accept accept, void *rfcomm_inst);
t_api test_port_ent_dlc_estab_ind_cb(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, prh_t_rfcomm_accept rem_accept, void *multi_inst);

static struct prh_rfcomm_multi_start 
{
  pSem_t sem;
  int status;
} test_prh_rfcomm_ms;


int test_port_ent_dlc_data_ind_cb(prh_t_rfcomm_dlci DLCI, struct host_buf *p_buf, void *multi_inst) 
{
  static int test_count=0;
  int ret;
  

    host_buf_free(p_buf);
  return 0;
}

int test_port_ent_dlc_linestat_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_line_stat_params ls_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype)
{
  return 0;
}

int test_port_ent_dlc_portneg_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_port_params *port_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype)
{
  return 0;
}

t_api test_port_ent_dlc_flow_ind_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_flow_state flow_state, void *multi_inst)
{
  int ret;
  struct prh_rfcomm_pe_port *pe_info;

  ret=prh_rfcomm_pe_lookup_portinfo_by_dlci(&pe_info, dlci);
  if (ret!=BT_NOERROR)
    return 0;

  if (flow_state==PRH_RFCOMM_FLOW_DATA_STOP)
    {
      pe_info->agg_flow_state=PRH_RFCOMM_FLOW_DATA_STOP;
      test_system_set_enb_flag(pe_info->fd, 0);
    }
  else
    {
      pe_info->agg_flow_state=PRH_RFCOMM_FLOW_DATA_GO;
      test_system_set_enb_flag(pe_info->fd, 1);
    }
    
      
}

t_api test_port_ent_dlc_control_cb(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_control_params *ctrl_params, void *multi_inst, prh_t_rfcomm_cb_type cbtype)
{
  int ret;
  struct prh_rfcomm_pe_port *pe_info;

  ret=prh_rfcomm_pe_lookup_portinfo_by_dlci(&pe_info, dlci);
  if (ret!=BT_NOERROR)
    return 0;

  if (ctrl_params->flow_state==PRH_RFCOMM_FLOW_DATA_STOP)
    {
      pe_info->local_flow_state=PRH_RFCOMM_FLOW_DATA_STOP;
      test_system_set_enb_flag(pe_info->fd, 0);
    }
  else
    {
      pe_info->local_flow_state=PRH_RFCOMM_FLOW_DATA_GO;
      test_system_set_enb_flag(pe_info->fd, 1);
    }
}

int prh_test_rfcomm_pe_rfstart_ind_cb(t_bdaddr bd_addr, prh_t_rfcomm_start_sys_params *system_parameters, prh_t_rfcomm_accept accept, void *multi_inst)
{
  struct prh_rfcomm_multi_cb multi_cbs;

  multi_cbs.rfstart_conf=TEST_PRH_RFCOMM_UE_Start_Conf;
  multi_cbs.dlc_estab_ind=test_port_ent_dlc_estab_ind_cb;

  //  pe_info.pe_rfcomm_conn_handle=multi_inst;
  
  PRH_RFCOMM_UE_Start_Resp(bd_addr, NULL, 1, &multi_cbs, multi_inst);
  return 0;
}


t_api test_port_ent_dlc_estab_ind_cb(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, prh_t_rfcomm_accept rem_accept, void *multi_inst)
{
  struct prh_rfcomm_dlc_cb callbacks;
  int ret;
  int accept=1;
  prh_t_rfcomm_schan loc_chan;
  struct prh_rfcomm_pe_port *pe_info;
  prh_t_rfcomm_control_params control_params;

  /* Search for this in the port entity... extract server channel from dlci */
  loc_chan=dlci>>1;

  /* Need to check is it a registered server channel */
  /* Return 0 for no channel, 1 if a channel exists */
  ret=prh_rfcomm_pe_check_schan(loc_chan);
  if (ret==0)
    accept=0;
  else
    {
      /* Check is it a registered port number */
      ret=prh_rfcomm_pe_lookup_portinfo_by_portnum(&pe_info, loc_chan);
      if (ret==BT_NOERROR)
    {
      callbacks.dlc_estab_conf=test_port_ent_dlc_estab_conf_cb;
      callbacks.dlc_rel_ind=test_port_ent_dlc_rel_ind_cb;
      callbacks.linestat=test_port_ent_dlc_linestat_cb;
      callbacks.portneg=test_port_ent_dlc_portneg_cb;
      callbacks.dlc_data_ind=test_port_ent_dlc_data_ind_cb;
      callbacks.control=test_port_ent_dlc_control_cb;
      callbacks.agg_flow_ind=test_port_ent_dlc_flow_ind_cb;
      /* Has it a DLCI already assigned? if so it is connected */
      if (pe_info->dlci!=0)
        accept=0;
      else
        {
          if (dlci>0)
        pe_info->dlci=dlci;
          pe_info->multi_inst=multi_inst;
          accept=1;
        }
    }
      else
    {
      accept=0;
    }
    }

  PRH_RFCOMM_UE_DLC_Estab_Resp(bd_addr, dlci, NULL, accept, &callbacks, multi_inst);
  
  if (accept!=0)
    {
      /* Chuck out an MSC now also */
      control_params.flow_state=PRH_RFCOMM_FLOW_DATA_GO;
      control_params.break_signal=0;
      control_params.modem_signal=PRH_RFCOMM_MSC_DTRDSR | PRH_RFCOMM_MSC_RTSCTS | PRH_RFCOMM_MSC_DCD;
      
      PRH_RFCOMM_UE_Control_Req(dlci, &control_params, multi_inst);
    }
    
  return 0;
}

t_api TEST_PRH_RFCOMM_UE_Start_Conf(t_bdaddr bd_addr, prh_t_rfcomm_start_sys_params *system_parameters, prh_t_rfcomm_accept accept, void *rfcomm_inst)
{
  /* Check semaphore list and unlock it */
  test_prh_rfcomm_ms.status=accept;
  pSemUnlock(test_prh_rfcomm_ms.sem);
  return 0;
}

t_api test_port_ent_dlc_estab_conf_cb(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, prh_t_rfcomm_accept accept, void *multi_inst)
{
  test_prh_rfcomm_ms.status=accept;
  pSemUnlock(test_prh_rfcomm_ms.sem);
  return 0;
}

t_api test_port_ent_dlc_rel_ind_cb(prh_t_rfcomm_dlci dlci, void *multi_inst)
{
  int ret;
  struct prh_rfcomm_pe_port *pe_info;
  prh_t_rfcomm_schan loc_chan;

  loc_chan=dlci>>1;
  ret=prh_rfcomm_pe_lookup_portinfo_by_portnum(&pe_info, loc_chan);
  if (ret==BT_NOERROR)
    {
      pe_info->dlci=0;
    }
  return 0;
}

int prh_test_rfcomm_open_multiplexer(t_bdaddr remote_bd_addr, void **multi_inst)
{
  struct prh_rfcomm_multi_cb multi_cbs;
  int ret;
  int dbit;
  test_prh_rfcomm_ms.sem=pSemCreate(0);
  
  multi_cbs.rfstart_conf=TEST_PRH_RFCOMM_UE_Start_Conf;
  multi_cbs.dlc_estab_ind=test_port_ent_dlc_estab_ind_cb;
  
  ret=PRH_RFCOMM_UE_Start_Req(remote_bd_addr, NULL, &multi_cbs, prh_test_pe_rfcomm_mgr_handle, multi_inst);
  
  pSemLock(test_prh_rfcomm_ms.sem, 0);
  
  if (test_prh_rfcomm_ms.status==0)
    {
      pSemFree(test_prh_rfcomm_ms.sem);
      return BT_UNKNOWNERROR;
    }
  return BT_NOERROR;
}

int prh_test_rfcomm_send_parneg(prh_t_rfcomm_dlci dlci, int max_frame_size,  void *multi_inst)
{
  prh_t_rfcomm_dlc_params dlc_params;
  dlc_params.max_frame_size=max_frame_size;
  dlc_params.priority=0;
  PRH_RFCOMM_UE_Parneg_Req(dlci, &dlc_params, multi_inst);

}

int prh_test_rfcomm_dlc_estab(t_bdaddr remote_bd_addr, prh_t_rfcomm_dlci dlci, void *multi_inst, prh_t_rfcomm_dlc_params *dlc_params_ptr)
{
  struct prh_rfcomm_dlc_cb dlc_cbs;
 /* Now to establish the DLC */
  dlc_cbs.dlc_estab_conf=test_port_ent_dlc_estab_conf_cb;
  dlc_cbs.dlc_rel_ind=test_port_ent_dlc_rel_ind_cb;
  dlc_cbs.dlc_data_ind=test_port_ent_dlc_data_ind_cb;
  dlc_cbs.linestat=test_port_ent_dlc_linestat_cb;
  dlc_cbs.portneg=test_port_ent_dlc_portneg_cb;
  dlc_cbs.control=test_port_ent_dlc_control_cb;
  dlc_cbs.agg_flow_ind=test_port_ent_dlc_flow_ind_cb;
  PRH_RFCOMM_UE_DLC_Estab_Req(remote_bd_addr, dlci, dlc_params_ptr, &dlc_cbs, multi_inst);
}

int prh_test_rfcomm_pe_connect_port(t_bdaddr remote_bd_addr, prh_t_rfcomm_schan rem_schan, char *pName, prh_t_rfcomm_dlci *dlci, struct prh_rfcomm_pe_port **pe_info_ptr, u_int16 max_frame_size)
{
  struct prh_rfcomm_multi_cb multi_cbs;
  struct prh_rfcomm_dlc_cb dlc_cbs;
  prh_t_rfcomm_dlc_params dlc_params;
  int dbit;
  t_api ret;
  struct prh_rfcomm_l2cap_inst *l2cap_inst;
  void *multi_inst;
  struct prh_rfcomm_pe_port *pe_info;

  /* Allocate memory for pe_info */
  pe_info=(struct prh_rfcomm_pe_port *)pMalloc(sizeof(struct prh_rfcomm_pe_port));
  if (pe_info==pNULL)
    return BT_NORESOURCES;

  pe_info->portname=(char *)pMalloc(pStrlen(pName)+1);
  if (pe_info->portname==pNULL)
    {
      pFree(pe_info);
      return BT_NORESOURCES;
    }

  /* copy in portname */
  pStrcpy(pe_info->portname, pName);
    
  /* Check does a multiplexer to the remote BD Address exist already */
  ret=prh_rfcomm_lookup_l2cap_inst_by_bd_addr(remote_bd_addr, &l2cap_inst);
  if (ret==BT_INVALIDPARAM)
    {
      ret=prh_test_rfcomm_open_multiplexer(remote_bd_addr, &multi_inst);
      if (ret!=BT_NOERROR)
    return ret;
      dbit=1;
    }
  else
    {
      dbit=l2cap_inst->initiating;
      multi_inst=(void *)l2cap_inst;
    }
  
  /* Either Multiplexer is started or we've returned */
  dlc_params.max_frame_size=max_frame_size;
  dlc_params.priority=0;
  
  /* Generate the DLCI from direction and remote server channel */
  pe_info->dlci=*dlci=((!dbit) | (rem_schan<<1));
  pe_info->multi_inst=multi_inst;
  pe_info->flow_sem=pSemCreate(0);
  pe_info->remote_device_addr=remote_bd_addr;
  pe_info->portnum=0;
  pe_info->mode=PRH_PORT_ENT_CLIENT_CONN;
  prh_rfcomm_init_pe_struct(pe_info);
  
  prh_test_rfcomm_dlc_estab(remote_bd_addr, *dlci,multi_inst, &dlc_params);

  pSemLock(test_prh_rfcomm_ms.sem, 0);

  
  pSemFree(test_prh_rfcomm_ms.sem);
  
  if (test_prh_rfcomm_ms.status==PRH_RFCOMM_REFUSE)
    return BT_UNKNOWNERROR;
  else
    {
      prh_rfcomm_pe_add_port_entity(pe_info);
      *pe_info_ptr=pe_info;
      return BT_NOERROR;
    }

}

int test_port_ent_data_req(int fd)
{
  int ret;
  struct host_buf *buffer;
  struct prh_rfcomm_pe_port *pe_port;

  ret=prh_rfcomm_pe_lookup_portinfo_by_fd(&pe_port, fd);
  if (ret!=BT_NOERROR)
    return 0;
  
  
  buffer=host_buf_alloc(160);
  host_buf_reserve_header(buffer, 30);
  ret=read(fd, buffer->data, 120);
  if (ret<0)
    {
      host_buf_free(buffer);
      return 0;
    }
  if (ret==0)
    {
      pExit(-1);
    }
  if (ret>0)
    {
      buffer->len=ret;
      PRH_RFCOMM_UE_Data_Req(pe_port->dlci, buffer, pe_port->multi_inst);
    }
  else
    {
      host_buf_free(buffer);
    }

  return 0;
}

