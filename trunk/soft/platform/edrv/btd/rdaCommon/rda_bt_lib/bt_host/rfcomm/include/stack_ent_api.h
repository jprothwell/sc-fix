/***********************************************************************
 *
 * MODULE NAME:    stack_ent_api.h
 * PROJECT CODE:   Host Side Stack
 * DESCRIPTION:    Stack Entity API
 * MAINTAINER:     David Airlie
 * CREATION DATE:  1 May 2000
 *
 * SOURCE CONTROL: $Id: stack_ent_api.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Inc.
 *     All rights reserved.
 *
 ***********************************************************************/
#ifndef PRH_STACK_ENT_API_H
#define PRH_STACK_ENT_API_H

#include "host_types.h"
#include "rfcomm_types.h"

#if PRH_RFCOMM_RSE_SUPPORT == 1
#define PRH_STACK_ENT_REFUSE 0
#define PRH_STACK_ENT_ACCEPT 1

#define RSE_FLOW_STOP 0
#define RSE_FLOW_GO 1

typedef struct st_t_RSE_Callbacks
{
  t_api (*connectionConfirmation)(prh_t_rfcomm_schan schan, t_bdaddr address, u_int8 status, prh_t_rfcomm_dlci dlci_s, u_int16 max_frame_size);
  t_api (*connectionIndication)(prh_t_rfcomm_schan schan, t_bdaddr address, prh_t_rfcomm_dlci dlci, u_int16 max_frame_size);
  t_api (*dataIndication)(t_bdaddr address, prh_t_rfcomm_dlci dlci, struct host_buf *p_buf);
  t_api (*flowIndication)(t_bdaddr address, prh_t_rfcomm_dlci dlci, u_int8 flow_state);  
  t_api (*disconnectionIndication)(t_bdaddr address, prh_t_rfcomm_dlci dlci);
  t_api (*framesizeIndication)(t_bdaddr address, prh_t_rfcomm_dlci dlci, u_int16 max_frame_size);
} t_RSE_Callbacks;

t_api prh_rse_layer_ctrl(u_int8 flags);
APIDECL1 t_api APIDECL2 RSE_SrvRegisterPort(prh_t_rfcomm_schan *serv_chan, t_RSE_Callbacks *cbs);
APIDECL1 t_api APIDECL2 RSE_SrvDeregisterPort(prh_t_rfcomm_schan schan);
APIDECL1 t_api APIDECL2 RSE_SrvDisconnectPort(prh_t_rfcomm_schan schan);
APIDECL1 t_api APIDECL2 RSE_CliConnectPort(t_bdaddr remote_bd_addr, prh_t_rfcomm_schan rem_schan, u_int16 max_frame_size, t_RSE_Callbacks *cbs);
APIDECL1 t_api APIDECL2 RSE_CliDisconnectPort(t_bdaddr remote_bd_addr, prh_t_rfcomm_schan schan);
APIDECL1 t_api APIDECL2 RSE_Write(t_bdaddr remote_bd_addr, prh_t_rfcomm_dlci dlci, struct host_buf *p_buf);
APIDECL1 t_api APIDECL2 RSE_SrvConnectResponse(t_bdaddr remote_bd_addr, prh_t_rfcomm_dlci dlci, u_int8 accept);
APIDECL1 t_api APIDECL2 RSE_GetWriteBuffer(t_bdaddr bdaddr, int dlci, int length, struct host_buf **p_buf);
APIDECL1 t_api APIDECL2 RSE_FreeWriteBuffer(struct host_buf *frame);
APIDECL1 t_api APIDECL2 RSE_GetCIDFromConnection(t_bdaddr bdaddr, prh_t_rfcomm_dlci dlci, u_int16 *cid);

#if PRH_RFCOMM_RSE_DATA_PUMP_ENABLE 
APIDECL1 t_api APIDECL2 RSE_DataPumpTx(t_bdaddr bdaddr, int dlci, u_int8 pattern_num, u_int16 pkt_size, u_int32 num_packets);
APIDECL1 t_api APIDECL2 RSE_DataPumpTxStop(t_bdaddr bdaddr, int dlci);
APIDECL1 t_api APIDECL2 RSE_DataPumpRxPrepare(t_bdaddr bdaddr, int dlci, u_int32 num_packets_report, void *reserved);
APIDECL1 t_api APIDECL2 RSE_DataPumpRxStop(t_bdaddr bdaddr, int dlci);
APIDECL1 t_api APIDECL2 RSE_DataPumpRxGetStats(t_bdaddr bdaddr, int dlci, u_int32 *total_data, u_int32 *rx_time_diff);

#endif

#endif /* PRH_RFCOMM_RSE_SUPPORT */

#endif
