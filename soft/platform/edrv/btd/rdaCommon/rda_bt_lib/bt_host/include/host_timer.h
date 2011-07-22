/***********************************************************************
 *
 * MODULE NAME:    host_timer.h
 * DESCRIPTION:    Host System buffers
 * MAINTAINER:     Dave Airlie
 *
 * SOURCE CONTROL: $Id: host_timer.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000 Parthus Technologies.
 *     All rights reserved.
 *
 ***********************************************************************/
#ifndef PRH_HOST_TIMER_H
#define PRH_HOST_TIMER_H


#include "host_config.h"
#include "host_types.h"

#if COMBINED_HOST==1
t_pTimer host_timer_create(u_int32 timeoutTenths, void(*callbackFunc)(void *), void *callbackArg, u_int8 flags);
int host_timer_cancel(t_pTimer timerHandle);
void host_handle_expired_timers(void);
#endif

#endif
