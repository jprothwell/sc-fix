/******************************************************************
 * MODULE NAME:    l2_psm.h
 * PROJECT CODE:
 * DESCRIPTION:    Functions for L2CAP connection oriented PSMs
 * MAINTAINER:     Gary Fleming
 * DATE:           18 November 1999
 *
 * SOURCE CONTROL: 
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies, Ltd.
 *     All rights reserved.
 *
 *    
 ******************************************************************/

#ifndef __PARTHUS_L2_PSM_H
#define __PARTHUS_L2_PSM_H


u_int16 prh_l2_psm_store_psm(t_psm psm);
u_int16 prh_l2_psm_free_psm(t_psm psm);

t_psm    prh_l2_psm_get_psm(u_int8 psm_index);
u_int8   prh_l2_psm_get_psm_ident(t_psm psm_value);
t_api prh_l2_psm_event(u_int16 psm_index, u_int8 event,t_L2_ControlCallbackArgs*  eventinfo);
u_int8 prh_l2_psm_get_clt_state(u_int16 psm);

extern struct l2cap_psm prh_l2_psm_cb_list[]; 
extern u_int16 prh_l2_psm_id_array[];  


#endif
