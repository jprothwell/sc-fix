#ifndef MMI_ON_HARDWARE_P
#include "stdafx.h"
#include "mci.h"
#include "stack_config.h"
#include "mmi_jpeg_dec.h"
// Image
UINT32  MCI_ImgDecode (img_decode_req_struct *imgPara)  // MCI_IMG_DECODE_REQ
{
	return 0;
}
UINT32  MCI_ImgDecodeEvenInd (VOID)  // MCI_IMG_DECODE_EVENT_IND
{
	return 0;
}
UINT32  MCI_ImgDecodeFinishInd (VOID)  //MCI_IMG_DECODE_FINISH_IND
{
	return 0;
}
UINT32  MCI_ImgResize (img_decode_req_struct *imgPara)  // MCI_IMG_RESIZE_REQ
{
	return 0;
}
UINT32  MCI_ImgStop (VOID)  //MCI_IMG_STOP_REQ
{
	return 0;
}

// Init
BOOL    MCI_TaskInit(VOID)     //  MCI_MEDIA_PLAY_REQ,         
{
	return 0;
}

// Video Record
UINT32  MCI_VideoRecordAdjustSetting(INT32 adjustItem, INT32 value)
{
	return 0;
}
UINT32  MCI_VideoRecordPause(VOID)
{
	return 0;
}
UINT32  MCI_VideoRecordPreviewStart(MMC_VDOREC_SETTING_STRUCT *previewPara)
{
	return 0;
}
UINT32  MCI_VideoRecordPreviewStop(VOID)
{
	return 0;
}
UINT32  MCI_VideoRecordResume(VOID)
{
	return 0;
}
UINT32  MCI_VideoRecordStart(char *pfilename)
{
	return 0;
}
UINT32  MCI_VideoRecordStop(VOID)
{
	return 0;
}
UINT32  MCI_DisplayVideoInterface (VOID)
{
	return 0;
}

// MCI Task
HANDLE  GetMCITaskHandle(COSMBOXID mod)
{
	return 0;
}
BOOL    SetMCITaskHandle(COSMBOXID mod, HANDLE hTask)
{
	return 0;
}

// Ring
UINT32  MCI_RingSetVolume(UINT16 volume)
{
	return 0;
}

// Misc
INT32 mci_webcamera_start()
{
	return 0;
}
UINT32 MCI_SetLilyGPIOLevel(UINT32 Level)
{
	return 0;
}

UINT32 MCI_AudioPlayBuffer(UINT32 *pBuffer, UINT32 len, UINT8 loop,MCI_AUDIO_BUFFER_PLAY_CALLBACK_T callback)
{
  return 0;
}

UINT32 MCI_AudioStopBuffer(void)
{
  return 0;
}

UINT32 MCI_AudioGetPlayInformation(MCI_PlayInf* PlayInformation)
{
  return 0;
}

void aud_send_ilm(module_type dest_id, 
                     UINT32 msg_id, 
                     void *local_param_ptr, 
                     void *peer_buf_ptr)
{

}

BOOL aud_bt_hfp_is_audio_path_on(void)
{
  return FALSE;
}

MCI_ERR_T MCI_AudioGetFileInformation (CONST HANDLE FileHander,
											AudDesInfoStruct  * CONST DecInfo,
											CONST mci_type_enum FileType   )
{
  return 0;
}
#endif //MMI_ON_HARDWARE_P
