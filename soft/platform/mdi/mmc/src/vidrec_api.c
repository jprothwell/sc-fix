////////////////////////////////////////////////////////////////////////////////
//        Copyright (C) 2002-2007, Coolsand Technologies, Inc.
//                       All Rights Reserved
//
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmisison of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
//
// FILENAME: vidrec_api.c
//
// DESCRIPTION:
//   TODO...
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   baoyh           20071130        create
//
////////////////////////////////////////////////////////////////////////////////
#include "cs_types.h"
#include "cpu_share.h"
#include "mcip_debug.h"

#include "mmc.h"
//#include "lily_lcd_api.h"

//#include "cpu_sleep.h"
#include "lily_statemachine.h"
//#include "cpu_sleep.h"
#include "lily_statemachine.h"
#include "mmc_videorecord.h"

//extern boolean camInPreviewState;
int32 LILY_VidrecPreviewStartReq(MMC_VDOREC_SETTING_STRUCT *previewParam, void (*vid_rec_finish_ind)(uint16 msg_result) )
{
	int32 result;
	MCI_TRACE(MCI_VIDREC_TRC , 0,"[MMC_VIDREC]LILY_VidrecPreviewStartReq!");
	
	if (STA_RES_FAIL == lily_changeToState(STM_MOD_VIRECD))
	{
		MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MMC_VIDREC_ERROR]ERROR STATE!");
		return -1;
	}
	
	mmc_SetCurrMode(MMC_MODE_VIDREC);
	
	result = VidRec_PreviewStart(previewParam, vid_rec_finish_ind);
	
	return result;
}

extern  UINT32 MCI_CamSetPara(INT32 effectCode,INT32 value) ;
int32 LILY_VidrecAdjustSettingReq (int32 adjustItem, int32 value)
{
	int32 result;
	result = (int32)MCI_CamSetPara(adjustItem,value);
	return result;
}

int32 LILY_VidrecPreviewStopReq(void)
{
  int32 result;
  result = VidRec_PreviewStop();
  mmc_ExitMode();
  lily_exitModule(STM_MOD_VIRECD);
  return result;
}

int32 LILY_VidrecRecordStartReq (HANDLE filehandle) 	//	MCI_MEDIA_PLAY_REQ,			
{
	int32 result;
  result = VidRec_RecordStart(filehandle);    
  return result;
}

int32 LILY_VidrecRecordPauseReq(void)
{
  	int32 result;

    result = VidRec_RecordPause();

  	return result;
}

int32 LILY_VidrecRecordResumeReq(void)
{
  	int32 result;

    result = VidRec_RecordResume();

  	return result;
}

int32 LILY_VidrecRecordStopReq(void)
{
  int32 ret=0;
	ret = VidRec_RecordStop();
    mmc_ExitMode();
  return ret;
}

int32 LILY_VidrecAudioSampleReadyReq(uint32 nEvent)
{
  int32 ret = 0;
//  ret = Vidrec_Sample(EVENT_GetDataAddr(nEvent), EVENT_GetDataSize(nEvent) >> 1);
  return ret;
}

int32 LILY_VidrecApplyAudioSampleStop(void)
{
	return 0;
}


