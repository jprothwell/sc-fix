/**************************************************************************
 * MODULE NAME:    l2_channel.h
 * PROJECT CODE:
 * DESCRIPTION:    L2CAP Channel manipulation functions
 * AUTHOR:         Gary Fleming
 * DATE:           20-07-2000
 *
 * SOURCE CONTROL: $Id: l2_channel.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000 Parthus Technologies, Ltd.
 *     All rights reserved.
 *
 **************************************************************************/

#ifndef __PARTHUS_L2CAP_CHANNEL
#define __PARTHUS_L2CAP_CHANNEL

#include "l2_types.h"

#if COMBINED_HOST==0
extern prh_t_l2_channel*  prh_l2_channel_list[];
#else
extern prh_t_l2_channel  prh_l2_channel_list[];
#endif

prh_t_l2_channel* prh_l2_chn_tid2channel(u_int8 identifier);
prh_t_l2_channel* prh_l2_chn_get_p_channel(u_int16 cid);
prh_t_l2_channel* prh_l2_chn_alloc_channel(void);

int prh_l2_chn_handle_in_use(u_int16 handle);

u_int8 prh_l2_chn_set_qos(prh_t_l2_channel* p_channel,t_L2_ChannelFlowSpec *outflow);
void prh_l2_chn_free_channels_on_handle(u_int16 handle);
void prh_l2_chn_free_channels_on_psm(u_int16 psm);
void prh_l2_chn_free_channel(prh_t_l2_channel* p_channel, u_int8 release_acl);
void  prh_l2_chn_data_error_for_channels_on_handle(u_int16 handle,u_int8 data_error);

/***************************************************
 *  methods for L2CAP Channel timers.
 ***************************************************/

void prh_l2_chn_start_RTX_timer(prh_t_l2_channel* p_channel);
void prh_l2_chn_disable_RTX_timer(prh_t_l2_channel* p_channel);
void prh_l2_chn_start_ERTX_timer(prh_t_l2_channel* p_channel);
void prh_l2_chn_disable_ERTX_timer(prh_t_l2_channel* p_channel);
void prh_l2_chn_timer_expired(void *arg);

void prh_l2_chn_stop_cfg_timer(prh_t_l2_channel* p_channel);
void prh_l2_chn_start_cfg_timer(prh_t_l2_channel* p_channel);

#endif
