/******************************************************************
 * MODULE NAME:     l2_config_helper.h
 * PROJECT CODE:
 * DESCRIPTION:     L2CAP Configuration Helper
 *    
 * AUTHOR:          Dave Airlie
 * DATE:            21 July 1999 
 *
 * SOURCE CONTROL: $Id: l2_config_helper.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies, Ltd.
 *     All rights reserved.
 *    
 ******************************************************************/

#ifndef _PRH_L2_CONFIG_HELPER_H
#define _PRH_L2_CONFIG_HELPER_H

#if PRH_L2CAP_AUTO_CONFIG==1
#define PRH_L2CAP_MTU_MIN 48
int prh_l2_config_helper_startup(u_int32 flags);
int prh_l2_config_helper_shutdown(u_int32 flags);
void prh_l2_configure_helper_conn_confirm(u_int8 eventType,t_L2_ControlCallbackArgs *args);
void prh_l2_configure_helper_indication(u_int8 eventType,t_L2_ControlCallbackArgs *args);
void prh_l2_configure_helper_confirmation(u_int8 eventType,t_L2_ControlCallbackArgs *args);
int prh_l2_config_helper_remove_cid(u_int16 cid);
int prh_l2cap_config_helper_remove_all_connections(void);
#endif
#endif
