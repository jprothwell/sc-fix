#ifndef MMI_ON_HARDWARE_P

#include "stdafx.h"
#include "mci.h"
#include "med_main.h"

// Video
UINT32  MCI_VideoClose (VOID)   //MCI_VID_CLOSE_REQ
{
	return 0;
}
#if (CSW_EXTENDED_API_AUDIO_VIDEO==1)
UINT32  MCI_VideoOpenFile (INT32 OutputPath,UINT8* filename)
{
	return 0;
}
#else
UINT32  MCI_VideoOpenFile (INT32 OutputPath,UINT8 open_audio, HANDLE fhd,med_type_enum type)   //MCI_VID_OPEN_FILE_REQ
{
	return 0;
}
#endif /* CSW_EXTENDED_API_AUDIO_VIDEO */
VOID    MCI_VideoOpenFileInd (mci_vid_file_ready_ind_struct *vdoOpen)     //MCI_VID_FILE_READY_IND;
{
}
UINT32  MCI_VideoPlay (UINT16 startX, UINT16 startY)   //MCI_VID_PLAY_REQ
{
	return 0;
}
VOID    MCI_VideoPlayInd (VOID)   // MCI_VID_PLAY_FINISH_IND
{
}
UINT32  MCI_VideoStop (VOID)    // MCI_VID_STOP_REQ
{
	return 0;
}
UINT32  MCI_VideoPause (VOID)  //  MCI_VID_PAUSE_REQ
{
	return 0;
}
UINT32  MCI_VideoResume (VOID)  // MCI_VID_RESUME_REQ
{
	return 0;
}
UINT32  MCI_VideoSeek (INT32 playtime, INT32 time_mode, UINT16 startX, UINT16 startY)  // MCI_VID_SEEK_REQ
{
	return 0;
}
VOID    MCI_VideoSeekInd (VOID) // MCI_VID_SEEK_DONE_IND
{
}
UINT32  MCI_VideoSet (UINT16 ZoomWidth, UINT16 ZoomHeight, UINT16 startX, UINT16 startY)  // MCI_VID_SET_MODE_REQ
{
	return 0;
}
VOID    MCI_VideoGetInfo ( UINT16  *image_width, UINT16  *image_height, UINT32  *total_time)  
{
	return ;
}
UINT32  MCI_VideoGetPlayTime (VOID) 
{
	return 0;
}

// Camera
UINT32  MCI_CamPowerUp (INT32  vediomode) // MCI_CAM_POWER_UP_REQ
{
	return 0;
}
UINT32  MCI_CamPowerDown (VOID)  //MCI_CAM_POWER_DOWN_REQ;
{
	return 0;
}
UINT32  MCI_CamPreviewOpen(CAM_PREVIEW_STRUCT* data)//MCI_CAM_PREVIEW_REQ
{
	return 0;
}
UINT32  MCI_CamPreviewClose (VOID) //MCI_CAM_STOP_REQ
{
	return 0;
}
UINT32  MCI_CamCapture (CAM_CAPTURE_STRUCT *data)    //MCI_CAM_CAPTURE_REQ
{
	return 0;
}
UINT32  MCI_CamSetPara(INT32 effectCode,INT32 value)  // MCI_CAM_SET_PARAM_REQ
{
	return 0;
}
INT32 AudioMp3StartStream(UINT32 *pBuffer, UINT32 len, UINT8 loop)
{
  return -1;
}
INT32 AudioMp3StopStream(void)
{
return -1;
}
VOID vdorec_cancel_saveFile(VOID)
{
 return ;
}
#endif MMI_ON_HARDWARE_P
