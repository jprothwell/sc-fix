// ---------------------------------------------------------------------------
// RDA Microelectronics, Inc. Company Confidential Strictly Private
//
// $RCSfile: rdamtv_app.h,v $
// $Revision: 0 $
// $Author: wuhp $
// $Date: 2009-05-25 $
//
// ---------------------------------------------------------------------------
// >>>>>>>>>>>>>>>>>>>>>>>>> COPYRIGHT NOTICE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ---------------------------------------------------------------------------
// Copyright 2008-2009 (c) RDA Microelectronics, Inc.
//
// RDA owns the sole copyright to this software. Under international
// copyright laws you (1) may not make a copy of this software except for
// the purposes of maintaining a single archive copy, (2) may not derive
// works herefrom, (3) may not distribute this work to others. These rights
// are provided for information clarification, other restrictions of rights
// may apply as well.
//
// This is an unpublished work.
// ---------------------------------------------------------------------------

#ifndef __RDAMTV_APP_H__
#define __RDAMTV_APP_H__

#define RDAMTV_APP_VERSION    "2.8.8"

//#define RDAMTV_TASK_SUPPORT
//#define RDA5888_SOFTBLENDING_ENABLE
//#define RDA5888_SYNC_CHECK_ENABLE
//#define RDA5888_HISR_CHECK_ENABLE
//#define RDA5888_OUTPUT_SP1_ENABLE  // 搜台过程输出雪花点处理
//#define RDA5888_VIDENHANCE_ENABLE
//#define RDA5888_INCHFILTER_ENABLE

typedef struct rdamtv_channel_s {
	int     chn;
	uint32  hz;
	int     bw;
	int     chstd;
} rdamtv_channel_t, *p_rdamtv_channel_t;

typedef struct 
{
	uint16 Lim_Low;
	uint16 Lim_UP;
	uint8 yc_adjust;
	uint8 vt_fail_th;
	uint16 frame_coef;
} BlendStruct;

// -------------------------------------
// global variables
// -------------------------------------
extern int g_nCurChCnt;  // current channel counter.
extern int g_nRdamtvInitDone;
extern rdamtv_ctx_t g_stRdamtvCtx;
extern uint8 g_nIspHisrCnt;
extern uint8 g_nRdamtvTimer;

#ifdef RDA5888_VIDENHANCE_ENABLE
extern uint8 g_nVideoEnhanceEnable;
extern uint8 g_nVideoEnSet;
extern uint8 g_nDiscardFrameNum;
extern uint32 g_nFrameBufSize;
extern uint8 g_nFirstBufferFlag;
#endif

void rdamtv_app_init(void);
int rdamtv_app_set_channel_map(int area);
int rdamtv_app_set_channel(int chn);
int rdamtv_app_get_area(void);
int rdamtv_app_exit(void);
void rdamtv_app_mute(BOOL bMute);

#endif
