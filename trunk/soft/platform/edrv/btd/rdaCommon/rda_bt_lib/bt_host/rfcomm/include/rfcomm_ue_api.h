/*
 * Module NAME:    rfcomm_ue_api.h
 * PROJECT CODE:   
 * DESCRIPTION:    RFCOMM Upper Edge API used by the Port Entity of the OS
 * MAINTAINER:     David Airlie
 * DATE:           14 Jan 2000
 *
 * SOURCE CONTROL: $Id: rfcomm_ue_api.h 1532 2010-08-23 05:47:34Z huazeng $ 
 *
 * LICENSE:
 *     This source code is copyright (c) 1999-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */
#ifndef _PRH_RFCOMM_UE_API
#define _PRH_RFCOMM_UE_API

#include "host_config.h"

#include "rfcomm_types.h"

/* the P.E. calls the RFCOMM with Requests and Responses */
/*
  t_api8 PRH_RFCOMM_UE_Start_Ind_cb(t_bdaddr bd_addr, t_start_sys_params *system_parameters, t_accept accept, void *multi_inst);

  t_api8 PRH_RFCOMM_UE_Start_Conf_cb(t_bdaddr bd_addr,  t_start_sys_params *system_parameters, t_accept accept, void *rfcomm_inst);
*/

t_api PRH_RFCOMM_UE_Disable_Credit(void);
t_api PRH_RFCOMM_UE_Enable_Credit(void);

t_api PRH_RFCOMM_UE_Allocate_Server_Channel(prh_t_rfcomm_schan *schan_ptr);
t_api PRH_RFCOMM_UE_Deallocate_Server_Channel(prh_t_rfcomm_schan schan);

/* Register/Deregister a server channel with RFCOMM */
t_api PRH_RFCOMM_UE_Register_Server(prh_t_rfcomm_schan schan, struct prh_rfcomm_dlc_cb *dlc_cbs);
t_api PRH_RFCOMM_UE_Deregister_Server(prh_t_rfcomm_schan schan);

/* Set the state of a server channel to Open/Closed */
t_api PRH_RFCOMM_UE_Set_Channel_Open(prh_t_rfcomm_schan schan);
t_api PRH_RFCOMM_UE_Set_Channel_Closed(prh_t_rfcomm_schan schan);

/* Retrieve the Channel State */
t_api PRH_RFCOMM_UE_Get_Channel_State(prh_t_rfcomm_schan schan);

/* RFCOMM Start - Ref TS 07.10 5.1.2 */
t_api PRH_RFCOMM_UE_Start_Req(t_bdaddr bd_addr, prh_t_rfcomm_start_sys_params *system_parameters, struct prh_rfcomm_multi_cb *callbacks, void **multi_inst);
t_api PRH_RFCOMM_UE_Start_Resp(t_bdaddr bd_addr, prh_t_rfcomm_start_sys_params *system_parameters, prh_t_rfcomm_accept accept, struct prh_rfcomm_multi_cb *callbacks, void *multi_inst);

/* call back function types for Indication and Confirm */
/* RFCOMM DLC Establish - Ref TS 07.10 5.1.3 */
t_api PRH_RFCOMM_UE_DLC_Estab_Req(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, struct prh_rfcomm_dlc_cb *cbs, void *multi_inst);
t_api PRH_RFCOMM_UE_DLC_Estab_Resp(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, prh_t_rfcomm_accept accept, void *multi_inst);

/* RFCOMM Data Call - Ref TS 07.10 5.1.4 */
t_api PRH_RFCOMM_UE_Data_Req(prh_t_rfcomm_dlci dlci, struct host_buf *buffer, void *multi_inst);

/* RFCOMM DLC Release - Ref TS 07.10 5.1.6 */
t_api PRH_RFCOMM_UE_DLC_Rel_Req(prh_t_rfcomm_dlci dlci, void *multi_inst);
/* t_api8 PRH_RFCOMM_UE_DLC_Rel_Ind(t_DLCI dlci, void *multi_inst); */

/* RFCOMM Close Down - Ref TS 07.10 5.1.7 */
t_api PRH_RFCOMM_UE_Close_Req(void *multi_inst);

/* Control Services */
/* RFCOMM DLC Param Negotiation - Ref TS 07.10 5.1.8.1.1 */
t_api PRH_RFCOMM_UE_Parneg_Req(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *DLC_parameters, void *multi_inst);
t_api PRH_RFCOMM_UE_Parneg_Resp(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *DLC_parameters, prh_t_rfcomm_accept accept, void *multi_inst);

t_api PRH_RFCOMM_UE_Parneg_Credit_Req(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, u_int8 init_credits, void *multi_inst);
t_api PRH_RFCOMM_UE_Parneg_Credit_Resp(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_dlc_params *dlc_params, prh_t_rfcomm_accept accept, u_int8 init_credits, void *multi_inst);

/* RFCOMM Test - Ref TS 07.10 5.1.8.1.3 */
t_api PRH_RFCOMM_UE_Test_Req(struct host_buf *frame, void *multi_inst);

/* RFCOMM Flow Control - Ref TS 07.10 5.1.8.1.4 */
t_api PRH_RFCOMM_UE_Flow_Req(prh_t_rfcomm_flow_state state, void *multi_inst);

/* Port Emulation Services */
/* RFCOMM Port Negotiation - Ref TS 07.10 5.1.8.2.1 */
t_api PRH_RFCOMM_UE_Portneg_Req(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_port_params *port_parameters, void *multi_inst);
t_api PRH_RFCOMM_UE_Portneg_Resp(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_port_params *port_parameters, prh_t_rfcomm_accept accept, void *multi_inst);

/* RFCOMM Control - Ref TS 07.10 5.1.8.2.2 */
t_api PRH_RFCOMM_UE_Control_Req(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_control_params *control_parameters, void *multi_inst);

/* RFCOMM Line Status - Ref TS 07.10 5.1.8.2.3 */
t_api PRH_RFCOMM_UE_Linestatus_Req(prh_t_rfcomm_dlci dlci, prh_t_rfcomm_line_stat_params line_status_parameters, void *multi_inst);

t_api PRH_RFCOMM_UE_register_rfstart_ind_cb(int (*PRH_RFCOMM_UE_Start_Ind_cb)(t_bdaddr, prh_t_rfcomm_start_sys_params *, prh_t_rfcomm_accept, void *));

/* Buffer management - needed to allow Credit Flow to work later */
t_api PRH_RFCOMM_UE_Get_Write_Buffer(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci, u_int16 frame_size, u_int32 flags, struct host_buf **frame);
t_api PRH_RFCOMM_UE_Get_Test_Buffer(t_bdaddr bd_addr, u_int16 frame_size, struct host_buf **frame);

t_api PRH_RFCOMM_UE_Release_Write_Buffer(struct host_buf *frame);

t_api PRH_RFCOMM_UE_Get_Multiplexer_State(t_bdaddr bd_addr, void **multi_inst);
t_api PRH_RFCOMM_UE_Get_L2CAP_CID_From_Multiplexer(void *multi_inst, u_int16 *cid);
#include "rfcomm_pe.h"

#endif
