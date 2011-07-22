/***********************************************************************
 *
 * MODULE NAME:    a2dp_register.h
 * DESCRIPTION:    a2dp init and register sep
 * MAINTAINER:     Zhu Jianguo
 * CREATION DATE:  17 Sept. 2008
 *
 * *
 * LICENSE:
 *     This source code is copyright (c) 2008 RDA Inc.
 *     All rights reserved.
 *
 ***********************************************************************/
#ifndef _A2DP_REGISTER_H_
#define _A2DP_REGISTER_H_

#if COMBINED_HOST==1
#include "sys_hal_types.h"
#endif

#include "a2dp.h"

#define A2DP_DEFAULT_SUPPORTED_FEATURES  0x0001  //Amplifier:Not supported, Recorder:Not supported, Speaker:Not supported, Headphone: Supported
#define A2DP_DEFAULT_VERSION_INFORMATION 0x0100
#define AVDTP_DEFAULT_VERSION_INFORMATION 0x0100

t_A2DP_STATE Get_A2dp_State(void);
void A2dp_Set_State(t_A2DP_STATE state);
void A2dp_Init(void);
//void headset_db_init(void);
//int sdp_init_headset(void);
//int sdp_init_A2DP(void);
boolean A2dp_Get_BdAddr(t_bdaddr *addr);
void A2dp_Suspend(void);
void A2dp_Start(void);
void A2dp_Codec_Enable(void);
void A2dp_Codec_Disable(void);
void A2dp_Codec_configure(void);
int A2dp_ServiceDiscover(t_bdaddr address);
t_a2dp_Supported_feature Get_A2DP_Feature(void);
int A2DP_Setup_Connect(t_bdaddr address);
int A2DP_Send_Data(u_int16 length, u_int8* p_data, u_int32 timestamp,u_int16 seq_num, u_int8 payload_type, u_int8 marker );

#endif 
