/*
 * MODULE NAME:    tcs_cc_enc.h
 * DESCRIPTION:    TCS Call Control Encoder Header File
 * AUTHOR:         Thomas Howley
 * DATE:           05 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_cc_enc.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies.
 *     All rights reserved.
 *
 */

#ifndef __TCS_CC_ENC_H
#define __TCS_CC_ENC_H

#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)

#include "host_types.h"
#include "tcs_const.h"
#include "tcs_cc_types.h"

#define PRH_TCS_MSG_SIZE_HEADER 2

t_api prh_tcs_cc_encodeMsg(u_int8 tcsMsg, struct st_t_TCS_callParams *callParams, u_int16 cid);


#endif /*(PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/




#endif /* __TCS_CC_ENC_H */
