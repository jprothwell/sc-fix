/***********************************************************************
 *
 * MODULE NAME:    init.h
 * DESCRIPTION:    init
 * MAINTAINER:     Zhu Jianguo
 * CREATION DATE:  17 Sept. 2008
 *
 * *
 * LICENSE:
 *     This source code is copyright (c) 2008 RDA Inc.
 *     All rights reserved.
 *
 ***********************************************************************/
#ifndef _HEADSET_SCHED_H_
#define _HEADSET_SCHED_H_

void Headset_Init(void);

#ifdef HFP_PRESENT
#if HFP_HF
u_int8 Headset_Hfp_Sched(void);
#endif
#endif

#if A2DP_PRESENT
u_int8 Headset_A2dp_Sched(void);
#endif

void Headset_AvStream_Stop(boolean send_suspend);
void Headset_AvStream_Start(void);
boolean Headset_Is_A2dp_SourceAnAg(void);
void Headset_A2dp_Suspend_Ind(void);
void Headset_A2dp_Start_Ind(void);
void Headset_Update_Volume(u_int8 gain);

#endif 
