/***********************************************************************
 *
 * MODULE NAME:    port_ent_api.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Generic Port Entity code
 * MAINTAINER:     David Airlie
 * CREATION DATE:  1 May 2000
 *
 * SOURCE CONTROL: $Id: port_ent_api.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    01.May.2000 -   DA - first revision
 *
 *
 * ISSUES:
 *
 ***********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>

#include "host_types.h"
#include "l2cap_primitives.h"
#include "papi.h"
#include "host_buf.h"
#include "rfcomm_ue_api.h"
#include "rfcomm_debug.h"
#include "rfcomm_core.h"
#include "rfcomm_pe.h"

void *prh_test_pe_rfcomm_mgr_handle;
extern struct prh_rfcomm_pe_port *prh_rfcomm_pe_sys;
int prh_test_rfcomm_pe_rfstart_ind_cb(t_bdaddr bd_addr, prh_t_rfcomm_start_sys_params *system_parameters, prh_t_rfcomm_accept accept, void *multi_inst);


int TEST_PE_Init(void)
{
  prh_rfcomm_init(&prh_test_pe_rfcomm_mgr_handle, 0);
  
  PRH_RFCOMM_UE_register_rfstart_ind_cb(prh_test_rfcomm_pe_rfstart_ind_cb, prh_test_pe_rfcomm_mgr_handle);

  return 0;
}

t_api TEST_PE_SrvRegisterPort(const char *port, prh_t_rfcomm_schan *serv_chan,  t_api (*port_callback)(prh_t_rfcomm_schan schan, t_bdaddr bd_addr, u_int8 connection_state))
{
  struct prh_rfcomm_pe_port *pe_info;
  int schan;
  int ret=0;
  int portnum;
  
  /* Check is passed in server channel available? */
  if (*serv_chan>0 && *serv_chan<32)
    {
      ret=prh_rfcomm_pe_check_schan(*serv_chan);
      if (ret==1)
    return BT_NORESOURCES;
      
    }

  ret=prh_rfcomm_pe_alloc_schan(serv_chan);
  portnum=*serv_chan;  

  if (portnum!=0)
    {
      /* Add port to list of registered port numbers */
      pe_info=(struct prh_rfcomm_pe_port *)pMalloc(sizeof(struct prh_rfcomm_pe_port));
      pe_info->portname=(char *)malloc(pStrlen((const char *)port));
      
      pStrcpy(pe_info->portname, port);
      
      pe_info->portnum=portnum;
      pe_info->dlci=0;
      ret=prh_rfcomm_pe_add_port_entity(pe_info);

      //      prh_rfcomm_pe_sys=pe_info;
#if USE_LINUX_SERIAL
      pe_info->fd=open(port, O_RDWR|O_NONBLOCK);
      if (pe_info->fd<0)
    {
      /* Failed to open the requested port */
      perror("failed to open port\n");
      pExit(-1);
    }
      
      system_set_tioc(pe_info->fd);
      test_system_add_fd(pe_info->fd);
#endif
      return BT_NOERROR;
    }
  else
    {
      return BT_NORESOURCES;
    }
}

t_api TEST_PE_SrvDeregisterPort(prh_t_rfcomm_schan schan)
{
  struct prh_rfcomm_pe_port *pe_info;
  int ret;

  if (schan==0 || schan>31)
    return BT_INVALIDPARAM;
  
  ret=prh_rfcomm_pe_remove_port_entity_by_portnum(&pe_info, schan);
  if (ret==BT_NOERROR)
    {
#if USE_LINUX_SERIAL
      test_system_rem_fd(pe_info->fd);
      close(pe_info->fd);
#endif
      pFree(pe_info);
      prh_rfcomm_pe_free_schan(schan);  
      return BT_NOERROR;
    }
  else
    return BT_INVALIDPARAM;
}

t_api TEST_PE_SrvDisconnectPort(prh_t_rfcomm_schan schan)
{
  struct prh_rfcomm_pe_port *pe_info;
  int ret;
  if (schan==0 || schan>31)
    return BT_INVALIDPARAM;
  
  ret=prh_rfcomm_pe_disconnect_local_server_channel(&pe_info, schan);
  return ret;
}

t_api TEST_PE_CliConnectPort(t_bdaddr remote_bd_addr, prh_t_rfcomm_schan rem_schan, char *pName, t_api (*port_callback)(prh_t_rfcomm_schan schan, t_bdaddr bd_addr, u_int8 connection_state))
{
  prh_t_rfcomm_dlci dlci;
  struct prh_rfcomm_pe_port *pe_info;
  int ret;

   /* Call connect port with semaphore */
  ret=prh_rfcomm_pe_connect_port(remote_bd_addr, rem_schan, pName, &dlci, &pe_info);

  if (ret==BT_NOERROR)
    {
      //      prh_rfcomm_pe_sys=(struct prh_rfcomm_pe_port *)(*port_id);
#if USE_LINUX_SERIAL
      //      pe_info=(struct prh_rfcomm_pe_port *)(*port_id);
      pe_info->fd=open(pName, O_RDWR|O_NONBLOCK);
      
      if (pe_info->fd<0)
    pExit(-1);
      system_set_tioc(pe_info->fd);      
      test_system_add_fd(pe_info->fd);
#endif
      return BT_NOERROR;
    }
  else
    return BT_INVALIDPARAM;
 
}

int TEST_PE_CliDisconnectPort(t_bdaddr remote_bd_addr, prh_t_rfcomm_schan serverchannel)
{
  int dlci;
  int ret;
  struct prh_rfcomm_pe_port *pe_info;
  
   ret=prh_rfcomm_pe_disconnect_remote_serverchannel(remote_bd_addr, serverchannel, &pe_info);
  if (ret==BT_NOERROR)
    {
      /* Close Linux port */    
#if USE_LINUX_SERIAL
      test_system_rem_fd(pe_info->fd);
      close(pe_info->fd);
#endif
      pFree(pe_info);
      return BT_NOERROR;
    }
  else
    return BT_INVALIDPARAM;
  
}
 


