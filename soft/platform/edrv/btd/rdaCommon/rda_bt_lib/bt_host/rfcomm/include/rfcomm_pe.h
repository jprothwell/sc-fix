/***********************************************************************
 *
 * MODULE NAME:    rfcomm_pe.h
 * PROJECT CODE:   Port Entity include file
 * DESCRIPTION:    eCos Startup Code
 * MAINTAINER:     David Airlie
 * CREATION DATE:  1 May 2000
 *
 * SOURCE CONTROL: $Id: rfcomm_pe.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Inc.
 *     All rights reserved.
 *
 ***********************************************************************/
#ifndef PRH_RFCOMM_PE_H
#define PRH_RFCOMM_PE_H

//#include "platform_pe.h"
struct prh_rfcomm_os_port 
{
	int dummy;
};

struct prh_rfcomm_pe_port
{
  /* Bluetooth device address of the remote device - used if we are connector or connected */
  t_bdaddr remote_device_addr;

  /* Locally registered port number 0 for a port we have connected */
  u_int32 portnum;

  /* Multiplexer Instance */
  void *multi_inst;

  /* Local port name */
  char *portname;
  
  /* DLCI for a connected port */
  prh_t_rfcomm_dlci dlci;

  /* Status information for a port */
  /* For CREDIT_FLOW_CONTROL we can overload local_flow_state, 
     as this is not used in CREDIT_FLOW_CONTROL */
  prh_t_rfcomm_flow_state local_flow_state;
  prh_t_rfcomm_flow_state agg_flow_state;

  prh_t_rfcomm_port_params port_params;
  prh_t_rfcomm_dlc_params dlc_params;
  
  /* Callback function for notification of a connection to this entity */
  void (*port_cbfunc)(prh_t_rfcomm_schan, t_bdaddr, u_int8);

  struct prh_rfcomm_os_port os_spec;
};

#define PRH_PORT_ENT_SERVER_REG 1
#define PRH_PORT_ENT_CLIENT_CONN 2

/* These values must reflect values in rfcomm.h API header file */
#define PRH_PORT_ENT_CONNECT 0
#define PRH_PORT_ENT_DISCONNECT 1
#define PRH_PORT_ENT_CONNECTFAILED 2

t_api PE_Init(void);
APIDECL1 t_api APIDECL2 PE_SrvRegisterPort(char *port, prh_t_rfcomm_schan *serv_chan, void (*port_callback)(prh_t_rfcomm_schan schan, t_bdaddr bd_addr, u_int8 connection_state));
APIDECL1 t_api APIDECL2 PE_SrvDeregisterPort(prh_t_rfcomm_schan schan);
APIDECL1 t_api APIDECL2 PE_SrvDisconnectPort(prh_t_rfcomm_schan schan);
APIDECL1 t_api APIDECL2 PE_CliConnectPort(t_bdaddr remote_bd_addr, prh_t_rfcomm_schan rem_schan, char *pName, void(*port_callback)(prh_t_rfcomm_schan schan, t_bdaddr bd_addr, u_int8 connection_state));
APIDECL1 t_api APIDECL2 PE_CliDisconnectPort(t_bdaddr remote_bd_addr, prh_t_rfcomm_schan serverchannel);
APIDECL1 t_api APIDECL2 PE_GetCIDFromConnection(t_bdaddr bdaddr, prh_t_rfcomm_dlci dlci, u_int16 *cid);

t_api PE_OS_SrvRegisterPort(struct prh_rfcomm_pe_port *pe_info);
t_api PE_OS_SrvDeregisterPort(struct prh_rfcomm_pe_port *pe_info);
t_api PE_OS_SrvDisconnectPort(struct prh_rfcomm_pe_port *pe_info);
t_api PE_OS_CliConnectPort_Allocate(struct prh_rfcomm_pe_port *pe_info);
t_api PE_OS_CliConnectPort_DeAllocate(struct prh_rfcomm_pe_port *pe_info);
t_api PE_OS_CliConnectPort_Enable(struct prh_rfcomm_pe_port *pe_info);
t_api PE_OS_CliDisconnectPort(struct prh_rfcomm_pe_port *pe_info);
t_api PE_OS_ReleasePort(struct prh_rfcomm_pe_port *pe_info);

void prh_rfcomm_init_pe_struct(struct prh_rfcomm_pe_port *pe_info);
void prh_rfcomm_init_pe_struct_params_default(struct prh_rfcomm_pe_port *pe_info);
void prh_rfcomm_free_pe_struct(struct prh_rfcomm_pe_port *pe_info);
t_api prh_rfcomm_pe_alloc_schan(prh_t_rfcomm_schan *p_schan);
int prh_rfcomm_pe_check_schan(prh_t_rfcomm_schan server_channel);
int prh_rfcomm_pe_free_schan(prh_t_rfcomm_schan server_channel);
int prh_rfcomm_pe_add_port_entity(struct prh_rfcomm_pe_port *self);
int prh_rfcomm_pe_remove_port_entity_by_portnum(struct prh_rfcomm_pe_port **port, u_int32 portnum);
int prh_rfcomm_pe_connect_port(t_bdaddr remote_bd_addr, prh_t_rfcomm_schan rem_schan, const char *pName, prh_t_rfcomm_dlci *dlci, struct prh_rfcomm_pe_port **pe_info_ptr, u_int16 max_frame_size, void (*port_callback)(prh_t_rfcomm_schan schan, t_bdaddr bd_addr, u_int8 connection_state));
int prh_rfcomm_pe_lookup_port_entity_by_portnum(struct prh_rfcomm_pe_port **port, u_int32 portnum);
int prh_rfcomm_pe_lookup_port_entity_by_dlci(struct prh_rfcomm_pe_port **port, prh_t_rfcomm_dlci dlci);
int prh_rfcomm_pe_lookup_port_entity_by_fd(struct prh_rfcomm_pe_port **port, int fd);
int prh_rfcomm_pe_lookup_port_entity_by_addr_and_dlci(struct prh_rfcomm_pe_port **port, t_bdaddr bd_addr, u_int8 dlci);
int prh_rfcomm_pe_remove_port_entity_by_addr_and_dlci(struct prh_rfcomm_pe_port **port, t_bdaddr bd_addr, u_int8 dlci);
t_api prh_rfcomm_pe_disconnect_remote_serverchannel(t_bdaddr remote_bd_addr, prh_t_rfcomm_schan serverchannel, struct prh_rfcomm_pe_port **pe_info_ptr);
int prh_rfcomm_pe_disconnect_local_server_channel(struct prh_rfcomm_pe_port **pe_info_ptr, prh_t_rfcomm_schan schan);
int prh_rfcomm_pe_remove_all_port_entity(void);
int port_ent_set_port_config(struct prh_rfcomm_pe_port *pe_info);

#endif

