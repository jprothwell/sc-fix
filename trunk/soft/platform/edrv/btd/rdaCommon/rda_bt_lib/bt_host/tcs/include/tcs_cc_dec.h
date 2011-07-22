/*
 * MODULE NAME:    tcs_cc_dec.h
 * DESCRIPTION:    TCS Call Control Decoder Header File
 * AUTHOR:         Thomas Howley
 * DATE:           08 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_cc_dec.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies.
 *     All rights reserved.
 *
 */

#ifndef __TCS_CC_DEC_H
#define __TCS_CC_DEC_H

#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)
#include "host_types.h"

struct st_t_TCS_callParams;
t_api prh_tcs_cc_decodeMsg(struct host_buf* h_buff, struct st_t_TCS_callParams *callParams);

#endif /*(PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/



#endif /* __TCS_CC_DEC_H */
