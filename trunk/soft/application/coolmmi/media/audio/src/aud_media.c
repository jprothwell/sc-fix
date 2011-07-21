/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *   aud_media.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes media related functions of audio manager task.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/******************************************************************
 * MODULE
 *    AUD_MEDIA
 * DESCRIPTION
 *    This module defines the media related function for Audio Manager.
 *  
 ******************************************************************/


#ifndef MED_NOT_PRESENT



/*==== INCLUDES =========*/

#include "kal_release.h"  
#include "stack_common.h"  
#include "stack_msgs.h"
#include "app_ltlcom.h"          /* task message communiction */
#include "stacklib.h"           /* basic type for dll, evshed, stacktimer */
#include "event_shed.h"          /* event scheduler */
#include "stack_timer.h"         /* stack timer */


/* global includes */
#include "l1audio.h"
#include "device.h"
#include "resource_audio.h"
#include "fat_fs.h"
/* DRM_REPLACE */
#include "drm_gprot.h"

#include "nvram_struct.h"
#include "nvram_user_defs.h"
#include "nvram_data_items.h"
#include "custom_nvram_editor_data_item.h"
#include "custom_equipment.h"

/* local includes */
#include "med_global.h"
//#include "med_status.h"
#include "aud_defs.h"
#include "med_struct.h"
#include "med_api.h"
#include "med_context.h"
#include "med_main.h"
#include "aud_main.h"
#include "med_utility.h"
#if defined (__MED_VID_MOD__) || defined (__MED_MJPG_MOD__)
#include "vid_main.h"
#endif
#include "mdi_audio.h"
#define AUD_PAUSE_CLOSE_HANDLE

#include "aud_daf_parser.h"
#include "aud_amr_parser.h"

#include "media_others.h"
#include "mmi_trace.h"
#include "string.h"
//#include "recorder.h"
#include "cpu_share.h"//jiashuo add for set output path
#include "mci.h"
#include "oslmemory.h"
#include "soundeffect.h"

extern U16 UnicodeToAnsii(S8 *pOutBuffer, S8 *pInBuffer );
extern U16 AnsiiToUnicodeString(S8 *pOutBuffer, S8 *pInBuffer );
#ifdef __RICH_AUDIO_PROFILE__
extern volatile kal_mutexid  audio_input_stream_mutex; // from aud_input_stream.c
kal_int8 amr_seek_count = 0;
#endif // __RICH_AUDIO_PROFILE__
UINT8 audioplayer_restore_flag = 0;/////////dirty patch,temporarily.
extern kal_uint32 SaveAndSetIRQMask( void );
extern void RestoreIRQMask( kal_uint32 );

#define kal_mem_cpy(a,b,c) memcpy(a,b,c)

#ifdef __FF_AUDIO_SET__
extern void MusicStartAudioMode();
extern void MusicEndAudioMode();
#endif

extern U8 GetHandsetInPhone();  
extern void GpioSpeakerModeStart(void);
extern void GpioSpeakerModeEnd(void);

//extern UINT32* Amr_GetDataBuffer();


#define UNKNOWN_MEDIA_FORMAT 255

#define AMR_HEADER   "#!AMR\n"
#define AWB_HEADER   "#!AMR-WB\n"

#define RECORD_MEM_MARGIN (512)
#define GET_DWORD(x) (((kal_uint32)(x)[0]) | (((kal_uint32)(x)[1])<<16))
#define SET_DWORD(x,y) ((x)[0]=(kal_uint16)((y)&0xffff),x[1]=(kal_uint16)((y)>>16))
#define GET_BIG_DWORD(x) ((((kal_uint32)(x)[0])<<24) | (((kal_uint32)(x)[1])<<16) | (((kal_uint32)(x)[2])<<8) | ((kal_uint32)(x)[3]))
#define SET_BIG_DWORD(x,y) ((x)[0]=(kal_uint8)((y)>>24),x[1]=(kal_uint8)(((y)>>16)&0xff),x[2]=(kal_uint8)(((y)>>8)&0xff),(x)[3]=(kal_uint8)((y)&0xff))

extern kal_bool mediaIsDataConsumed( void );
extern int mediaGetDataCount( void );
extern int amrGetDataCount( void );
extern void wavSetAdpcmState(kal_bool realResume, kal_uint32 mode);
extern void wavGetAdpcmState(kal_uint32 mode);
extern void wavCopyAdpcmState(kal_uint32 src_mode, kal_uint32 dst_mode);

MCI_PlayInf audiofile_lastPlayInfo ;
MCI_PlayInf  PlayInformation;

#include "csw_sound_recorder.h"
#if (CSW_EXTENDED_API_AUDIO_VIDEO == 1)
#include "mci_audio.h"
#endif






// for store temp info in real resume mechanism
kal_bool temp_is_data_consumed;
kal_int32 temp_last_time;
kal_uint32  temp_last_offset;
UINT8 fileclosed = 1;
kal_int32 AudFileHandle;

/*========== Local Variables =============*/

typedef struct
{
   kal_uint8 *header;
   kal_uint16  size;
}
aud_file_header_struct;


aud_file_header_struct aud_file_header[2];

const kal_uint8 amr_header[] = AMR_HEADER;
const kal_uint8 awb_header[] = AWB_HEADER;

#ifdef __MED_VID_MOD__
extern STFSAL stFSAL;
extern STMp4Parser stMp4Parser;
extern STFSAL fsalAudioFile;
Media_Format eAudioFormat;
#endif /* __MED_VID_MOD__ */

#ifdef __MED_BT_A2DP_MOD__
#if (defined(__BTCOOLSAND__)||defined(__BTRDA__))
extern void aud_bt_a2dp_open_codec(kal_int32 audio_format);
extern kal_bool aud_bt_a2dp_is_mute_phone(void);
#endif
#endif

#ifdef __MMI_SOUND_EFFECT__
extern audio_equalizer_struct g_settings_audio_equalizer;
#endif

#ifdef __AUD_NEW_INTERFACE__
extern void aud_media_ievent_callback(Media_Handle *handle, Media_Event event);
STFSAL current_file_stream;
#endif // __AUD_NEW_INTERFACE__
extern void aud_media_close_handle( void ); // from aud_melody.c
/*========== Functions =============*/
/*************************chenhe+++++++++*/
//chenhe 200706278 add,to get recorder data by timer
//recorder++
//extern HANDLE sem_rec_keep_pace_fs;//chenhe,070828
//for greenstone modify on 20080807
void aud_media_record_getdata(MCI_ERR_T result) 
{
   aud_media_read_data_ind_struct* ind_p = NULL;


   ind_p = (aud_media_read_data_ind_struct*)construct_local_para(sizeof(aud_media_read_data_ind_struct), TD_CTRL);


    switch (result)
    {
        case MCI_ERR_END_OF_FILE:
		mmi_trace(1,"aud_media_record_getdata: State %d",result );

               // Send indicator to the MEDIA task 
   		ind_p->event = MED_ERROR;
			   
        break;

        case MCI_ERR_ERROR:
		mmi_trace(1,"aud_media_record_getdata: State %d",result );
    		
               // Send indicator to the MEDIA task 
   		ind_p->event = MED_ERROR;

        break;

        default:
    		mmi_trace(1,"aud_media_record_getdata: Unknown result %d",result);
        break;
    }
	   

   
   aud_send_ilm(MOD_MED, 
               MSG_ID_AUD_MEDIA_READ_DATA_IND, 
               ind_p, 
               NULL);
}
#if 0
INT32 aud_rec_readLastData()
{
		UINT32 len;
		INT32 result=0;
		aud_context_p->buf_p=	(kal_uint16*)REC_GetDataBuffer((UINT32 *)&(aud_context_p->buf_len) );
		 result=MMI_FS_Write(aud_context_p->current_file_handle, 
		                  aud_context_p->buf_p,
		                  aud_context_p->buf_len,
		                  &len);
		 return result;
}
BOOL aud_rec_start()//chenhe,20070629
{
	BOOL ret=FALSE;  
	ret=Rec_StartRecord();
	//med_start_timer(RECORDER_GET_DATA, 500, aud_media_record_getdata, 0);//chenhe delete tmp for delete warning
	return ret;
}
INT32 aud_rec_pause()//chenhe,20070629
{
	BOOL ret=FALSE;
	Rec_Pause();
	med_stop_timer(RECORDER_GET_DATA);
	//aud_media_record_getdata();//to get last data when pause
	ret=aud_rec_readLastData();
	return ret;

}
BOOL aud_rec_resume()//chenhe,20070629
{
	BOOL ret=FALSE;
	ret=Rec_Resume();
	//med_start_timer(RECORDER_GET_DATA, 500, aud_media_record_getdata, 0);////chenhe delete tmp for delete warning
	return ret;
}
INT32 aud_rec_stop()//chenhe,20070629
{
	INT32 ret=FALSE;	
	Rec_StopRecord();
	med_stop_timer(RECORDER_GET_DATA);
	//aud_media_record_getdata();//to get last data when stop
	ret=aud_rec_readLastData();

	return ret;
}

#endif
//recorder--
#if 0
//AMR++
void aud_media_amr_getdata_callback(void *data)
{
   aud_media_write_data_ind_struct* ind_p = NULL;

   ind_p = (aud_media_write_data_ind_struct*)
            construct_local_para(sizeof(aud_media_write_data_ind_struct), TD_CTRL);
   
   if(aud_context_p->audio_end != KAL_TRUE) 
   {
	   ind_p->event = MEDIA_DATA_REQUEST;
   }
   else
   {
   	 ind_p->event = MEDIA_END;
   }
   ind_p->seq_no = aud_context_p->last_seq_no;
   
   aud_send_ilm(MOD_MED, 
               MSG_ID_AUD_MEDIA_WRITE_DATA_IND, 
               ind_p, 
               NULL);
}

Media_Status aud_amr_filePlay(FS_HANDLE fHandle) //chenhe,070703
{
	UINT32 fileSize=0;
	FS_HANDLE iRet;
	//fHandle=MMI_FS_Open(fileName, FS_READ_ONLY);
	mmi_trace(1,"chenhe,aud_amr_filePlay,aud_context_p->current_format is %d",aud_context_p->current_format);
	MMI_FS_GetFileSize(fHandle,&fileSize);
	if(fileSize>0)
	{
		switch(aud_context_p->current_format)
		{
			case MED_TYPE_AMR:
				iRet=Amr_FilePlay(fHandle,fileSize,AMR_FILETYPE_AMR);

				if(TRUE == iRet)
				{
					med_start_timer(AMR_PLAY_TIMER, 500, aud_media_amr_getdata_callback, 0);//chenhe
				}
				else
				{
					return MEDIA_FAIL;
				}
				return MEDIA_SUCCESS;
			
				break;
	
			case MED_TYPE_GSM_EFR:
				iRet=Amr_FilePlay(fHandle,fileSize,AMR_FILETYPE_REC);
				if(TRUE == iRet)
				{
					med_start_timer(AMR_PLAY_TIMER, 500, aud_media_amr_getdata_callback, 0);//chenhe
				}
				else
				{
					return MEDIA_FAIL;
				}
				return MEDIA_SUCCESS;
				
				break;
			
			default:
				break;
		}
	}
	else
	{
		return MEDIA_FAIL;
	}
}

void aud_amr_stop()
{
	med_stop_timer(AMR_PLAY_TIMER);
	Amr_FileStop();
	aud_media_amr_getdata_callback();
}
			
void aud_amr_pause()
{
	med_stop_timer(AMR_PLAY_TIMER);
	Amr_FilePause();
	aud_media_amr_getdata_callback();
}

void aud_amr_resume()
{
	Amr_FileResume();
	med_start_timer(AMR_PLAY_TIMER, 500, aud_media_amr_getdata_callback, 0);//chenhe

}
//AMR--
#endif
/*************************chenhe------------*/

/******************************************************************
* FUNCTION
*    aud_media_init
* DESCRIPTION
*    This function is used to init aud_media module
* PARAMETERS
*    
*    
* RETURNS
*
* GLOBALS AFFECTED
*  
*  
******************************************************************/
kal_bool aud_media_init(void)
{
   aud_file_header[0].header = (kal_uint8*)amr_header;
   aud_file_header[0].size = sizeof(AMR_HEADER)-1; 

   aud_file_header[1].header = (kal_uint8*)awb_header;
   aud_file_header[1].size = sizeof(AWB_HEADER)-1;

   aud_context_p->processing_data = KAL_FALSE;

#ifdef DEDI_AMR_REC_BUFFER
   aud_context_p->amr_buffer_p=NULL;
   aud_context_p->amr_buffer_size=0;
#endif

   aud_context_p->start_offset = aud_context_p->end_offset = 0;
   aud_context_p->current_file_handle = -1;
   AudFileHandle = -1;
#ifdef __RICH_AUDIO_PROFILE__
   audio_input_stream_mutex = kal_create_mutex( "audio_input_stream_mutex" );
#endif // __RICH_AUDIO_PROFILE__
   
   return KAL_TRUE;
}

/*
 * 0: none, 1: amr, 2: awb
 */
kal_uint8 aud_check_header_if_amr_or_awb( FS_HANDLE fd )
{
	aud_file_header_struct* file_header;
	kal_uint8 header[10];
	INT32 len;
	kal_int32 result;
         
	file_header = &(aud_file_header[MED_TYPE_AMR_WB - MED_TYPE_AMR]);
     result = DRM_read_file( fd, header, file_header->size, &len );
	if( result != FS_NO_ERROR || len != file_header->size )
		return 0;
	
	/* check if the AMR-WB header */
	if( strncmp( (const char*)header, (const char*)file_header->header, file_header->size ) == 0 )
		return 2;
	else
	{
		/* check if the AMR header */
		file_header = &(aud_file_header[MED_TYPE_AMR - MED_TYPE_AMR]);
		if( strncmp( (const char*)header, (const char*)file_header->header, file_header->size ) == 0 )
			return 1;
		else
			return 0;
	}
}

/******************************************************************
* FUNCTION
*    aud_open_record_file
* DESCRIPTION
*    This function is to open file for recording
* PARAMETERS
*    kal_wchar* file_name, kal_uint8 format
*    
* RETURNS
*  kal_uint8
* GLOBALS AFFECTED
*  
*  
******************************************************************/
typedef struct{
	uint8		szRiff[4];			/* "RIFF" */
	uint32		dwFileSize;			/* file size */
	uint8		szWaveFmt[8];		/* "WAVEfmt " */
	uint32		dwFmtSize;			/* 16 */
	uint16        wFormatTag;         /* format type */
	uint16        nChannels;          /* number of channels (i.e. mono, stereo...) */
	uint32       nSamplesPerSec;     /* sample rate */
	uint32       nAvgBytesPerSec;    /* for buffer estimation */
	uint16        nBlockAlign;        /* block size of data */
	uint16        wBitsPerSample;     /* number of bits per sample of mono data */
}WAVE_HEADER_MMI;

#define AMR_FRAME_SIZE_TMP (32)
kal_uint8 aud_open_record_file(kal_wchar* file_name, kal_int8 format)
{
	INT32 len;
	kal_uint8 res;
	kal_int32 result;
//	UINT32 vi = 0;
	kal_bool file_exist=KAL_FALSE;
	WAVE_HEADER_MMI waveFormatHeader;
#if 0
	UINT8 SilenceAmr122Frame[32]={0x3c,0x55,0x00,0x88,0xb6,0x66,0x79,0xe1,\
								  0xe0,0x01,0xe7,0xcf,0xf0,0x00,0x00,0x00,\
								  0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
								  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
#endif
   if ((res=aud_check_disc_space(file_name, RECORD_MEM_MARGIN))!=MED_RES_OK)
   {
      return res;
   }
  mmi_trace(1,"chenhe,aud_open_record_file,res is %d",res);
   /* set current file foramt */
   aud_context_p->current_format = med_get_media_type((kal_wchar*)file_name);
      //TRACE_EF(1,"chenhe,aud_open_record_file,aud_context_p->current_format  is %d,file_exist is %d",aud_context_p->current_format ,file_exist);
   
   /* check if the format valid */
   if (aud_context_p->current_format==MED_TYPE_NONE)
   { 
      if (format==MED_TYPE_NONE)
      {
         return MED_RES_INVALID_FORMAT;
      }   
      else
         aud_context_p->current_format = format;
   }
   else if (aud_context_p->current_format==MED_TYPE_WAV)
   {
      if(format==MED_TYPE_WAV)
         aud_context_p->current_format=MED_TYPE_WAV;
      else
         aud_context_p->current_format=MED_TYPE_WAV_DVI_ADPCM;
   }   
   else if (aud_context_p->current_format==MED_TYPE_GSM_EFR)
   {
      if(format==MED_TYPE_GSM_FR)
   	     aud_context_p->current_format=MED_TYPE_GSM_FR;
   }
     // TRACE_EF(1,"chenhe,aud_open_record_file,aud_context_p->current_file_handle is %d,before open",aud_context_p->current_file_handle);
      //TRACE_EF(1,"chenhe,aud_open_record_file,aud_context_p->current_format 2222 is %d,format is %d",aud_context_p->current_format,format );

   /* open the file */
   if ((aud_context_p->current_file_handle = MMI_FS_Open((const UINT8 *)file_name, FS_READ_WRITE))>=0)
      file_exist=KAL_TRUE;
   else
   {
      aud_context_p->current_file_handle = MMI_FS_Open((const UINT8*)file_name, FS_CREATE);
   }
   TRACE_EF(1,"chenhe,aud_open_record_file,aud_context_p->current_file_handle is %d,file_exist is %d,format is %d",aud_context_p->current_file_handle,file_exist,aud_context_p->current_format);
	//aud_context_p->current_file_handle=0;
   if (aud_context_p->current_file_handle>=0)
   {
   //mmi_trace(1,"aud_open_record_file,aud_context_p->current_file_handle is %d,file_exist is %d",aud_context_p->current_file_handle,file_exist);
  // mmi_trace(1,"aud_open_record_file,aud_context_p->current_format is %d",aud_context_p->current_format);
   
      if (file_exist)
      {
		if(aud_context_p->current_format == MED_TYPE_WAV||MED_TYPE_WAV_DVI_ADPCM==aud_context_p->current_format)
		{
			MMI_FS_Read(aud_context_p->current_file_handle, &waveFormatHeader, sizeof(WAVE_HEADER_MMI), &len);
			mmi_trace(1,"chenhe,wave,sample is %d,bit is %d,channel is %d",waveFormatHeader.nSamplesPerSec,waveFormatHeader.wBitsPerSample,waveFormatHeader.nChannels);
			if(waveFormatHeader.nSamplesPerSec!=8000 ||waveFormatHeader.wBitsPerSample!=16)
			{
				MMI_FS_Close(aud_context_p->current_file_handle);
						aud_context_p->current_file_handle=-1;
				return MED_RES_UNSUPPORTED_FREQ;
			}
			else if( waveFormatHeader.nChannels !=1/*MONO*/)
			{
				MMI_FS_Close(aud_context_p->current_file_handle);
						aud_context_p->current_file_handle=-1;
				return MED_RES_UNSUPPORTED_CHANNEL;
			}
		}
	         MMI_FS_Seek(aud_context_p->current_file_handle, (int)0, FS_FILE_END);
      }
      else if (aud_context_p->current_format == MED_TYPE_AMR ||
         aud_context_p->current_format == MED_TYPE_AMR_WB)
      {
		 aud_file_header_struct* file_header;
		 
		 file_header = &(aud_file_header[aud_context_p->current_format-MED_TYPE_AMR]);
		 result = MMI_FS_Write(aud_context_p->current_file_handle, 
		                  file_header->header,
		                  file_header->size,
		                  &len);
		if(result!=FS_NO_ERROR || len!=file_header->size)
		{

		
			MMI_FS_Close(aud_context_p->current_file_handle);
			MMI_FS_Delete((const UINT8 *)file_name);
			aud_context_p->current_file_handle=-1;
			if(FS_DISK_FULL == result)
			{
				return MED_RES_DISC_FULL;
			}
			else
			{
				return MED_RES_OPEN_FILE_FAIL;
			}
	     }
	     
	  }
	       TRACE_EF(1,"chenhe,aud_open_record_file,44444444->current_file_handle is %d,",aud_context_p->current_file_handle);
	 
	  kal_mem_cpy(aud_context_p->current_file_name, 
	                 file_name, 
#ifdef SLIM_AUDIO_PROFILE
		   128*2);
#else
		   256*2);
#endif                     //chenhe,070813
			
	                 //MAX_FILE_NAME_LEN*ENCODE_BYTE);
	     //  TRACE_EF(1,"chenhe,aud_open_record_file,5555555555aud_context_p->current_file_handle is %d,MAX_FILE_NAME_LEN*ENCODE_BYTE is %d",aud_context_p->current_file_handle,MAX_FILE_NAME_LEN*ENCODE_BYTE);

	  return MED_RES_OK;
	}
	else if(aud_context_p->current_file_handle==FS_WRITE_PROTECTION)
	{
		return MED_RES_WRITE_PROTECTION;
	}
	else if(aud_context_p->current_file_handle==FS_DISK_FULL)
	{
		return MED_RES_DISC_FULL;
	}
	else
	{
		return MED_RES_OPEN_FILE_FAIL;
	}

}


/******************************************************************
* FUNCTION
*    aud_close_record_file
* DESCRIPTION
*    This function is to close the recorded file.
* PARAMETERS
*    None
*    
* RETURNS
*  kal_uint8
* GLOBALS AFFECTED
*  
*  
******************************************************************/
kal_uint8 aud_close_record_file(void)
{
#if !defined(MT6205B) && !defined(MT6208)
   kal_uint32 len;
   kal_int32 result;
      TRACE_EF(1,"chenhe,aud_close_record_file,ud_context_p->current_format  is %d,aud_context_p->current_file_handle is %d",aud_context_p->current_format,aud_context_p->current_file_handle);

   if (aud_context_p->current_format == MED_TYPE_WAV ||
      aud_context_p->current_format == MED_TYPE_WAV_ALAW ||
      aud_context_p->current_format == MED_TYPE_WAV_ULAW ||
      aud_context_p->current_format == MED_TYPE_WAV_DVI_ADPCM)
   {
      kal_uint8* header_p;
      kal_uint32 data_len;
      TRACE_EF(1,"chenhe,aud_close_record_file,enter,wav");

      Media_GetFileHeader(&header_p, &aud_context_p->buf_len);
      data_len=(kal_uint32)*(header_p+4);
      
      data_len+=(((kal_uint32)*(header_p+5))<<8);
      data_len+=(((kal_uint32)*(header_p+6))<<16);
      data_len+=(((kal_uint32)*(header_p+7))<<24);
      /* To check the file size consistency */
      result=FS_GetFileSize(aud_context_p->current_file_handle, &len);
      if(result==FS_NO_ERROR && 
         (data_len+8!=aud_context_p->offset || aud_context_p->offset!=len))
      {
         ASSERT(data_len+8 == aud_context_p->offset);
      }
      
      if (aud_context_p->buf_len>0)
      {
         MMI_FS_Seek(aud_context_p->current_file_handle, (int)0, FS_FILE_BEGIN);
         result = MMI_FS_Write(aud_context_p->current_file_handle, 
                              header_p,
                              aud_context_p->buf_len,
                              &len);
         if (result!=FS_NO_ERROR||len!=aud_context_p->buf_len)
         {
            MMI_FS_Close(aud_context_p->current_file_handle);
            aud_context_p->current_file_handle=-1;
            return MED_RES_FAIL;
         }
      }
      else
      {
         MMI_FS_Close(aud_context_p->current_file_handle);
         aud_context_p->current_file_handle=-1;
         return MED_RES_FAIL;
      }

   }
#ifdef DEDI_AMR_REC_BUFFER
   else if(aud_context_p->current_format == MED_TYPE_AMR ||
         aud_context_p->current_format == MED_TYPE_GSM_EFR||
         aud_context_p->current_format == MED_TYPE_AMR_WB)
   {   
         TRACE_EF(1,"chenhe,aud_close_record_file,enter,amr");

      if(aud_context_p->amr_buffer_p)
      {
         med_free_int_mem((void**)&aud_context_p->amr_buffer_p);
      }
   }      
#endif
#endif
  TRACE_EF(1,"chenhe,aud_close_record_file,end");

   MMI_FS_Close(aud_context_p->current_file_handle);
   aud_context_p->current_file_handle=-1;
aud_context_p->current_format=MED_TYPE_NONE;//chenhe , 081016
   return MED_RES_OK;
      

}

/******************************************************************
* FUNCTION
*    aud_open_play_file
* DESCRIPTION
*    This function is to open file for playing.
* PARAMETERS
*    kal_wchar* file_name
*    
* RETURNS
*    kal_uint8
* GLOBALS AFFECTED
*  
*  
******************************************************************/
kal_uint8 aud_open_play_file(kal_wchar* file_name)
{

   kal_int32 result;

#if 0
	aud_context_p->current_format = MED_TYPE_DAF;
	//kal_mem_cpy(aud_context_p->current_file_name, file_name, MAX_FILE_NAME_LEN*ENCODE_BYTE);
	kal_mem_cpy(aud_context_p->current_file_name, file_name, 128);
	return MED_RES_OK;
	#endif

#if 1
   /* set current file foramt */
   aud_context_p->current_format = med_get_media_type((kal_wchar*)file_name);
   /* check if the format valid */
   if (aud_context_p->current_format==MED_TYPE_NONE)
      return MED_RES_INVALID_FORMAT;
       mmi_trace(TRUE, "chenhe : aud_open_play_file,aud_context_p->current_format is %d",aud_context_p->current_format);
 
   /* open the file */
   /* MP4 doesn't apply DRM */
#ifdef MP4_CODEC
   if (aud_context_p->current_format==MED_TYPE_M4A)
   {
      if ((aud_context_p->current_file_handle = MMI_FS_Open((const UINT8*)file_name, FS_READ_ONLY))>=0)
      {
         if(vid_open_m4a_file_for_play(aud_context_p->current_file_handle)!=MED_RES_OK)
         {
            MMI_FS_Close(aud_context_p->current_file_handle);
            aud_context_p->current_file_handle=-1;
            return MED_RES_BAD_FORMAT;
         }
         kal_mem_cpy(aud_context_p->current_file_name, 
                        file_name, 
                        MAX_FILE_NAME_LEN*ENCODE_BYTE);
         return MED_RES_OK;
      }
      else
         return MED_RES_OPEN_FILE_FAIL;

   }   
#endif /* MP4_CODEC */

   /* DRM_REPLACE */   
   if ((aud_context_p->current_file_handle = MMI_FS_Open((const UINT8*)file_name, FS_READ_ONLY))>=0)
   //if ((aud_context_p->current_file_handle = DRM_open_file((kal_wchar*)file_name, FS_READ_ONLY, 0))>=0)      
   {
#ifdef AMRWB_DECODE      
      if (aud_context_p->current_format == MED_TYPE_AMR ||
         aud_context_p->current_format == MED_TYPE_AMR_WB)
      {
         aud_file_header_struct* file_header;
         kal_uint8 header[10];
         
         file_header = &(aud_file_header[MED_TYPE_AMR_WB-MED_TYPE_AMR]);
         /* DRM_REPLACE */
         /*result = FS_Read(aud_context_p->current_file_handle, 
                          header,
                          file_header->size,
                          &len);*/
         result = DRM_read_file(aud_context_p->current_file_handle, 
                          header,
                          file_header->size,
                          &len);
         if (result!=FS_NO_ERROR||len!=file_header->size)
         {
            /* DRM_REPLACE */
            //FS_Close(aud_context_p->current_file_handle);
            DRM_close_file(aud_context_p->current_file_handle);
            aud_context_p->current_file_handle=-1;
            return MED_RES_BAD_FORMAT;
         }
         /* check if the AMR-WB header */
         if(strncmp((const char*)header, (const char*)file_header->header, file_header->size)==0)
         {
            aud_context_p->current_format = MED_TYPE_AMR_WB;
         }
         else
         {
         /* check if the AMR header */
         file_header = &(aud_file_header[MED_TYPE_AMR-MED_TYPE_AMR]);
         if(strncmp((const char*)header, (const char*)file_header->header, file_header->size)==0)
         {
            /* DRM_REPLACE */
            //FS_Seek(aud_context_p->current_file_handle, file_header->size, FS_FILE_BEGIN);
            DRM_seek_file(aud_context_p->current_file_handle, file_header->size, FS_FILE_BEGIN);
            aud_context_p->current_format = MED_TYPE_AMR;
         }
         else
         {
            /* DRM_REPLACE */
            //FS_Close(aud_context_p->current_file_handle);
            DRM_close_file(aud_context_p->current_file_handle);
            aud_context_p->current_file_handle=-1;
            return MED_RES_BAD_FORMAT;
         }
      }
         DRM_seek_file( aud_context_p->current_file_handle, aud_context_p->offset, FS_FILE_CURRENT );
      }
#else /* only support AMR not WB-AMR */
      if (aud_context_p->current_format == MED_TYPE_AMR || aud_context_p->current_format == MED_TYPE_GSM_EFR)
      {


#if 0         
         file_header = &(aud_file_header[0]);
         /* DRM_REPLACE */
         /*result = FS_Read(aud_context_p->current_file_handle, 
                          header,
                          file_header->size,
                          &len);*/
         result = DRM_read_file(aud_context_p->current_file_handle, 
                          header,
                          file_header->size,
                          &len);
		 #endif //chenhe
	result=FS_NO_ERROR;//chenhe
         if (result!=FS_NO_ERROR)
         {
            /* DRM_REPLACE */
            //FS_Close(aud_context_p->current_file_handle);
            DRM_close_file(aud_context_p->current_file_handle);            
            aud_context_p->current_file_handle=-1;
            return MED_RES_BAD_FORMAT;
         }
		 #if 0
         else if(strncmp((const char*)header, (const char*)file_header->header, file_header->size)!=0)
         {
            /* DRM_REPLACE */
            //FS_Close(aud_context_p->current_file_handle);
            DRM_close_file(aud_context_p->current_file_handle);            
            aud_context_p->current_file_handle=-1;
            return MED_RES_BAD_FORMAT;

         }
         DRM_seek_file( aud_context_p->current_file_handle, aud_context_p->offset, FS_FILE_CURRENT );
		 #endif
      }
#endif
#if  1 //chenhe
      else if (aud_context_p->current_format==MED_TYPE_DAF )
      {
	#if 0//chenhe
         /* go to daf data frame */
         kal_uint32 offset = aud_context_p->offset;
         audio_input_stream_struct *input_stream = audio_file_input_stream_alloc( aud_context_p->current_file_handle );
         if( !daf_seek( input_stream, &offset ) )
         {
            /* DRM_REPLACE */
            //FS_Close(aud_context_p->current_file_handle);
            DRM_close_file(aud_context_p->current_file_handle);
            aud_context_p->current_file_handle=-1;
            audio_input_stream_free( input_stream );
            return MED_RES_BAD_FORMAT;
         }
         audio_input_stream_free( input_stream );
         aud_context_p->offset = offset;
	#endif
		 fileclosed = 0;
		AudFileHandle = aud_context_p->current_file_handle;
      }
#endif     

#if 0
      kal_mem_cpy(aud_context_p->current_file_name, 
                     file_name, 
                     MAX_FILE_NAME_LEN*ENCODE_BYTE);
#endif
#ifdef SLIM_AUDIO_PROFILE
	   kal_mem_cpy(aud_context_p->current_file_name, 
                     file_name, 
                     256);
#else
	   kal_mem_cpy(aud_context_p->current_file_name, 
                     file_name, 
                     512);
#endif

                     
      return MED_RES_OK;
   }
   else
      return MED_RES_OPEN_FILE_FAIL;
#endif
}


/******************************************************************
* FUNCTION
*    aud_close_play_file
* DESCRIPTION
*    This function is to close the playing file.
* PARAMETERS
*    None
*    
* RETURNS
*    kal_uint8
* GLOBALS AFFECTED
*  
*  
******************************************************************/
kal_uint8 aud_close_play_file(void)
{
   /* close the file */
   /* MP4 doesn't apply DRM */
#ifdef MP4_CODEC
   if (aud_context_p->current_format==MED_TYPE_M4A)
   {
      if (aud_context_p->current_file_handle!=-1)
      {
         MMI_FS_Close(aud_context_p->current_file_handle);
		 		aud_context_p->current_file_handle=-1;

      }
   }
   else
#endif /* MP4_CODEC */
   {
   
   
   if (aud_context_p->current_format==MED_TYPE_DAF)
   {
   	//aud_context_p->current_file_handle = AudFileHandle;
      if (aud_context_p->current_file_handle!=-1)
      {
         MMI_FS_Close(aud_context_p->current_file_handle);
      }
	aud_context_p->current_file_handle=-1;
	return MED_RES_OK;  
   }
   
   if(aud_context_p->current_file_handle!=-1)
      /* DRM_REPLACE */         
      //FS_Close(aud_context_p->current_file_handle);      
      MMI_FS_Close(aud_context_p->current_file_handle);
   }   
   aud_context_p->current_file_handle=-1;
   
   return MED_RES_OK;
      
}

/******************************************************************
* FUNCTION
*    aud_media_stop_and_store_info_to_temp
* DESCRIPTION
*    This function is to stop media play and store play info to temp.
* PARAMETERS
*    void
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
extern UINT32 MCI_AudioGetPlayInformation(MCI_PlayInf* );
extern BOOL mmi_audply_is_playing(void);
extern BOOL mmi_audio_need_replay(void);

void aud_media_stop_and_store_info_to_temp( void )
{
   // avoid other type of stop procedures
#ifdef __MED_GENERAL_TTS__
    if (aud_context_p->source_type == AUD_TTS)
    {
        aud_tts_stop_req_hdlr(NULL);
        return;
    }
    else
#endif
   if( aud_context_p->source_type > AUD_STRING )
    {
      return;
    }
      mmi_trace(1,"chenhe,aud_media_stop_and_store_info_to_temp,aud_context_p->current_format is %d,aud_context_p->state is %d",aud_context_p->current_format,aud_context_p->state);
   
   switch( aud_context_p->current_format )
   {
#ifdef __MED_MMA_MOD__
      case MED_TYPE_SMF:
      case MED_TYPE_SMF_SND:
         ASSERT( aud_context_p->last_handle != NULL );
         temp_last_time = JSmf_GetCurrentTime( aud_context_p->last_handle );
         JSmf_Stop( aud_context_p->last_handle );
         JSmf_Close( aud_context_p->last_handle );
         aud_context_p->last_handle = NULL;
         kal_prompt_trace( MOD_MED, "stop and store info to temp: temp_last_time = %d", temp_last_time );
         break;
      case MED_TYPE_IMELODY:
         ASSERT( aud_context_p->last_handle != NULL );
         temp_last_time = JImy_GetCurrentTime( aud_context_p->last_handle );
         JImy_Stop( aud_context_p->last_handle );
         JImy_Close( aud_context_p->last_handle );
         aud_context_p->last_handle = NULL;
         kal_prompt_trace( MOD_MED, "stop and store info to temp: temp_last_time = %d", temp_last_time );
         break;
#ifdef MMF_DECODE
      case MED_TYPE_MMF:
         ASSERT( aud_context_p->last_handle != NULL );
         temp_last_time = JMmf_GetCurrentTime( aud_context_p->last_handle );
         JMmf_Stop( aud_context_p->last_handle );
         JMmf_Close( aud_context_p->last_handle );
         aud_context_p->last_handle = NULL;
         kal_prompt_trace( MOD_MED, "stop and store info to temp: temp_last_time = %d", temp_last_time );
         break;
#endif
      case MED_TYPE_AMR:
      case MED_TYPE_GSM_EFR:	  	
      case MED_TYPE_AMR_WB:
	case MED_TYPE_MID:	  	
	  	 #if 0//jiashuo del for jessamine.20080705
         Media_Stop();
	 	 aud_amr_stop();//chenhe
         temp_last_offset = aud_context_p->offset  - amrGetDataCount();
         kal_prompt_trace( MOD_MED, "stop and store info to temp: offset = %d, temp_last_offset = %d", aud_context_p->offset, temp_last_offset );
#ifdef __RICH_AUDIO_PROFILE__
         amr_seek_count = 0;
#endif
         break;
		 #endif
#endif
      case MED_TYPE_WAV:
	  	 #if 0//jiashuo del for jessamine,20080705
         temp_is_data_consumed = mediaIsDataConsumed();
         Media_Stop();
         temp_last_offset = aud_context_p->offset  - (mediaGetDataCount() << 1);
         wavGetAdpcmState( 0 );
         kal_prompt_trace( MOD_MED, "stop and store info to temp: offset = %d, temp_last_offset = %d", aud_context_p->offset, temp_last_offset );
         break;
		 #endif
#ifdef WMA_DECODE
	case MED_TYPE_WMA:
		 #if 0//jiashuo del for jasper 20080102
         ASSERT( aud_context_p->last_handle != NULL );
         temp_last_time = WMA_GetCurrentTime( aud_context_p->last_handle );
         WMA_Stop( aud_context_p->last_handle );
         WMA_Close( aud_context_p->last_handle );
         FSAL_Close( &current_file_stream );
         aud_context_p->last_handle = NULL;
         kal_prompt_trace( MOD_MED, "stop and store info to temp: temp_last_time = %d", temp_last_time );
         break;
		 #endif
#endif // WMA_DECODE
#ifdef AAC_DECODE
	case MED_TYPE_AAC:
		 #if 0//jiashuo del for jasper 20080102
         ASSERT( aud_context_p->last_handle != NULL );
         temp_last_time = AAC_GetCurrentTime( aud_context_p->last_handle );
         AAC_Stop( aud_context_p->last_handle );
         AAC_Close( aud_context_p->last_handle );
         FSAL_Close( &current_file_stream );
         aud_context_p->last_handle = NULL;
         kal_prompt_trace( MOD_MED, "stop and store info to temp: temp_last_time = %d", temp_last_time );
         break;
		 #endif
#endif // AAC_DECODE
#ifdef DAF_DECODE
	case MED_TYPE_DAF:
		if(aud_context_p->state == AUD_MEDIA_PLAY)
		{
			if( aud_context_p->play_mode == AUD_MEDIA_PLAY_AS_RING )
			{
				MCI_AudioStopBuffer();
			}
			else
			{
			if(!audioplayer_restore_flag &&(mmi_audply_is_playing()||mmi_audio_need_replay()))
			{
				audioplayer_restore_flag = 1;
			       MCI_AudioGetPlayInformation(&audiofile_lastPlayInfo);
	   			   mmi_trace(1,"chenhe,aud_media_stop_and_store_info_to_temp,111,audiofile_lastPlayInfo is %d,audioplayer_restore_flag is %d",audiofile_lastPlayInfo.PlayProgress,audioplayer_restore_flag);

			}
			   mmi_trace(1,"chenhe,aud_media_stop_and_store_info_to_temp,audiofile_lastPlayInfo is %d,audioplayer_restore_flag is %d,mmi_audply_is_playing() is %d,mmi_audio_need_replay() is %d",audiofile_lastPlayInfo.PlayProgress,audioplayer_restore_flag,mmi_audply_is_playing(),mmi_audio_need_replay());
				MCI_AudioStop();
			}
		}
		#ifdef __FF_AUDIO_SET__
		MusicEndAudioMode();   //Added by jinzh:20070730
		#endif
		aud_context_p->current_format=MED_TYPE_NONE;
                //jiashuo add end
		break;
#endif // AAC_DECODE
      default:
         temp_is_data_consumed = mediaIsDataConsumed();
         Media_Stop();
         temp_last_offset = aud_context_p->offset  - (mediaGetDataCount() << 1);
         kal_prompt_trace( MOD_MED, "stop and store info to temp: offset = %d, temp_last_offset = %d", aud_context_p->offset, temp_last_offset );
         break;
   }
   aud_context_p->last_seq_no++;
}

/******************************************************************
* FUNCTION
*    aud_stop_unfinished_process
* DESCRIPTION
*    This function is to stop unfinished media process
* PARAMETERS
*    void
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void aud_stop_unfinished_process(void)
{
   TBM_ENTRY(0x2A02);
   mmi_trace(1,"chenhe,aud_stop_unfinished_process,aud_context_p->state is %d",aud_context_p->state);
   switch (aud_context_p->state)
   {
      case AUD_MEDIA_PLAY:
      {
         /* Stop voice memo recording process */
         aud_media_stop_and_store_info_to_temp();
         /* stop volume crescendo timer */
         med_stop_timer(AUD_TIMER_CRESCENDO);
         /* stop the driver like led, backlight and vibrator */
         aud_melody_stop_driver();
         /* close the file */
         aud_close_play_file();
         /* free file buffer */
         aud_free_melody_file_buffer();
         /* enter AUD_MEDIA_IDLE state */
         AUD_ENTER_STATE(AUD_MEDIA_IDLE);

         break;
      }
      case AUD_MEDIA_PLAY_PAUSED:
      {
         /* Stop voice memo recording process */
         aud_media_stop_and_store_info_to_temp();
         /* close the file */
         aud_close_play_file();
         /* free file buffer */
         aud_free_melody_file_buffer();
         /* enter AUD_MEDIA_IDLE state */
         AUD_ENTER_STATE(AUD_MEDIA_IDLE);
         break;
      }
      case AUD_MEDIA_PLAY_FINISH:
      {
         /* Stop voice memo recording process */
         aud_media_stop_and_store_info_to_temp();
         /* stop volume crescendo timer */
         med_stop_timer(AUD_TIMER_CRESCENDO);
         /* enter AUD_MEDIA_IDLE state */
         AUD_ENTER_STATE(AUD_MEDIA_IDLE);
         break;
      }
#ifdef SOUND_RECORDER_SUPPORT	        
      case AUD_VM_RECORD:
      case AUD_MEDIA_RECORD:
      case AUD_MEDIA_RECORD_PAUSED:
      {
	// chenhe,081015,comment and add stop record
	MCI_AudioRecordStop();	
#if 0
         INT32  len;
         kal_int32   result;
         /* Stop voice memo recording process */
         Media_Stop();

         /*Restore input source when stop recording*/ 
         L1SP_SetInputSource( aud_context_p->input_device);

         /* Write the remaining data to file */
         Media_GetReadBuffer(&(aud_context_p->buf_p), 
                              (kal_uint32 *)&(aud_context_p->buf_len));
         ASSERT(aud_context_p->buf_p != NULL);
         ASSERT(aud_context_p->buf_len<=AUD_RING_BUFFER_LEN);

         result = MMI_FS_Write(aud_context_p->current_file_handle, 
                           aud_context_p->buf_p,
                           aud_context_p->buf_len*2,
                           &len);
                     
         /* update offset */
         aud_context_p->offset += len;

         Media_ReadDataDone(len>>1);
         
         if (result == FS_NO_ERROR)
         {
            /* in case that encounter ring buffer boundary */
            Media_GetReadBuffer(&(aud_context_p->buf_p), 
                                 (kal_uint32 *)&(aud_context_p->buf_len));
            if(aud_context_p->buf_len>0)
            {            
               ASSERT(aud_context_p->buf_p != NULL);
               ASSERT(aud_context_p->buf_len<=AUD_RING_BUFFER_LEN);

               result = MMI_FS_Write(aud_context_p->current_file_handle, 
                                 aud_context_p->buf_p,
                                 aud_context_p->buf_len*2,
                                 &len);
                           
               /* update offset */
               aud_context_p->offset += len;

               Media_ReadDataDone(len>>1);
               
            }
         }
#endif
         /* close record file, add header if wav */
         aud_close_record_file();

         /* enter AUD_MEDIA_IDLE state */
         AUD_ENTER_STATE(AUD_MEDIA_IDLE);
         break;   
      }         
#endif	  
      default:
      {
         break;
      }
   }
	aud_context_p->current_format=MED_TYPE_NONE;//chenhe , 081016
   	TBM_EXIT(0x2A02);
}

/******************************************************************
* FUNCTION
*    aud_media_record_callback
* DESCRIPTION
*    This function is the callback function for recording audio.
* PARAMETERS
*    Media_Event event
*    
* RETURNS
*    None
* GLOBALS AFFECTED
*  
*  
******************************************************************/
void aud_media_record_callback(Media_Event event)
{
   aud_media_read_data_ind_struct* ind_p = NULL;

   if (event==MEDIA_TERMINATED)
   {  
      return;
   }
   ind_p = (aud_media_read_data_ind_struct*)
            construct_local_para(sizeof(aud_media_read_data_ind_struct), TD_CTRL);

   ind_p->event = event;
   
   aud_send_ilm(MOD_MED, 
               MSG_ID_AUD_MEDIA_READ_DATA_IND, 
               ind_p, 
               NULL);
}

#if (CSW_EXTENDED_API_AUDIO_VIDEO == 1)
VOID ConvertRelative2Full(UINT8 *pFullName, UINT8 *pRelativeName);
INT32 ConvertDosName2Unix(UINT8 *pUnixName, CONST UINT8 *pDosName);
#endif /* (CSW_EXTENDED_API_AUDIO_VIDEO == 1) */

/******************************************************************
* FUNCTION
*    aud_media_record_req_hdlr
* DESCRIPTION
*    This function is to handle the audio record request.
* PARAMETERS
*    ilm_struct *ilm_ptr
*    
* RETURNS
*    None
* GLOBALS AFFECTED
*  
*  
******************************************************************/
void aud_media_record_req_hdlr(ilm_struct *ilm_ptr)
{
	l4aud_media_record_req_struct* msg_p = NULL;
	kal_uint8 result = 0;

	mmi_trace(1,"chenhe,received start recorder message");

	aud_context_p->src_mod = ilm_ptr->src_mod_id;

	msg_p = (l4aud_media_record_req_struct*)ilm_ptr->local_para_ptr;

	aud_context_p->src_id = msg_p->src_id;

	//jiashuo add start
	/* if keytone is playing, stop it */
	aud_keytone_stop(); 
	/* if tone is playing, stop it */
	if (aud_context_p->tone_playing)
	aud_tone_stop();

	switch (aud_context_p->state)
   	{
	      	case AUD_MEDIA_IDLE:

			//record the file format
			aud_context_p->current_format = med_get_media_type(msg_p->file_name);
#if (CSW_EXTENDED_API_AUDIO_VIDEO == 1)
            // FIXME Update ?
            // aud_context_p->current_format
            // aud_context_p->current_file_handle
            // aud_context_p->current_file_name
            // Nothing to do here, as the start record function
            // takes a filename as parameter
            result = MED_RES_OK;
#else
			result=aud_open_record_file((kal_wchar*)msg_p->file_name, (kal_uint8)msg_p->format);
#endif
			if (result!=MED_RES_OK) 
			{
				goto recorder_end;
			}
#if (CSW_EXTENDED_API_AUDIO_VIDEO == 1)
            {
            // Convert file name
                UINT8* fileNameRec; 	 	 
                UINT8* unixFileNameRec; 	 	 
                fileNameRec = OslMalloc(MAX_MEDIA_FILE_NAME);  
                unixFileNameRec =  OslMalloc(MAX_MEDIA_FILE_NAME);
                ConvertRelative2Full(fileNameRec, (UINT8*)msg_p->file_name );
                ConvertDosName2Unix(unixFileNameRec, fileNameRec);
            // and call function.
                result=mci_SndRecRecord(unixFileNameRec, aud_context_p->current_format);
             
                OslMfree(fileNameRec);
                OslMfree(unixFileNameRec);
            }
#else

    mci_type_enum audio_record_fromat_type;

    switch (aud_context_p->current_format)
    {
    
        case MED_TYPE_WAV_DVI_ADPCM:
		   audio_record_fromat_type=MCI_TYPE_WAV_DVI_ADPCM;
        break;

        case MED_TYPE_AMR:
		   audio_record_fromat_type=MCI_TYPE_AMR;
        break;


        default:
		audio_record_fromat_type=MCI_TYPE_NONE;
    		mmi_trace(1,"aud_media_record_req_hdlr: Unknown Multimedia Type %d",aud_context_p->current_format);
        break;
    }
#ifdef SOUND_RECORDER_SUPPORT
	result = MCI_AudioRecordStart(aud_context_p->current_file_handle,audio_record_fromat_type,msg_p->quality, aud_media_record_getdata);
#endif	
#endif
		  break;
		  default:
			aud_set_result(MED_RES_BUSY);
			AUD_SET_EVENT(AUD_EVT_RECORD);
		  	break;
	}

	mmi_trace(1,"chenhe,aud_media_record_req_hdlr,result is %d",result);
	
recorder_end:
		if(result == 0)
		{
			aud_set_result(MED_RES_OK);
			AUD_SET_EVENT(AUD_EVT_RECORD);
			if(aud_context_p->src_mod!=MOD_MMI)
			aud_send_media_record_cnf(MED_RES_OK);  

			/* enter AUD_MEDIA_RECORD state */
			AUD_ENTER_STATE(AUD_MEDIA_RECORD);
		}
		else
		{
			aud_set_result(result);
			aud_context_p->current_format=MED_TYPE_NONE;//chenhe , 081016
			AUD_SET_EVENT(AUD_EVT_RECORD);
		}
   #if 0
	UCS2Strcpy((S8 *)filename, (S8 *)(&msg_p->file_name[2]));
	
	switch (aud_context_p->state)
   	{
      	case AUD_MEDIA_IDLE:
      	{
			if(msg_p->input_source == AUDIO_DEVICE_FMRR)
			{
				//RDA5800 I2S transmit start
				FM_RDA5800_I2S();
				mmi_trace(1,"######RDA5800 I2S transmit start######");
				result=MCI_FmrRecordStart(filename);
				if(result == 0)
				{
					is_fmr_recording = 1;
				}
				else
				{
					FM_RDA5800_STOPI2S();
				}
			}
			else
			{
				result=MCI_RecordStart(filename);
			}
			if(result == 0)
			{
				aud_set_result(MED_RES_OK);
				AUD_SET_EVENT(AUD_EVT_RECORD);
				if(aud_context_p->src_mod!=MOD_MMI)
				aud_send_media_record_cnf(MED_RES_OK);  

				/* enter AUD_MEDIA_RECORD state */
				AUD_ENTER_STATE(AUD_MEDIA_RECORD);
			}
			else
			{
		            aud_set_result(result);
		            AUD_SET_EVENT(AUD_EVT_RECORD);
			}
			break;
      	}
		default:
        {
 	   	 	TRACE_EF(1,"chenhe,aud_media_record_req_hdlr,result is %d,at last,aud_context_p->current_file_handle is %d",result,aud_context_p->current_file_handle);
 
 	          
 	         aud_set_result(MED_RES_BUSY);
 	         AUD_SET_EVENT(AUD_EVT_RECORD);
 	         if(aud_context_p->src_mod!=MOD_MMI)
 	            aud_send_media_record_cnf(MED_RES_BUSY);
 	         break;
        }
	}
#endif	
}

/******************************************************************
* FUNCTION
*    aud_media_play_callback
* DESCRIPTION
*    This function is the callback function for playing audio.
* PARAMETERS
*    Media_Event event
*    
* RETURNS
*    None
* GLOBALS AFFECTED
*  
*  
******************************************************************/
void aud_media_play_callback(Media_Event event)
{
   aud_media_write_data_ind_struct* ind_p = NULL;

   if (event==MEDIA_TERMINATED)
   {  
      /* enter AUD_MEDIA_IDLE state */
      AUD_ENTER_STATE(AUD_MEDIA_IDLE);
      return;
   }

   ind_p = (aud_media_write_data_ind_struct*)
            construct_local_para(sizeof(aud_media_write_data_ind_struct), TD_CTRL);

   ind_p->event = event;
   ind_p->seq_no = aud_context_p->last_seq_no;
   
   aud_send_ilm(MOD_MED, 
               MSG_ID_AUD_MEDIA_WRITE_DATA_IND, 
               ind_p, 
               NULL);
}

/******************************************************************
* FUNCTION
*    aud_media_read_file_and_make_wav_header
* DESCRIPTION
*    This function is to read wav file, parse wav header and update data offset or size.
* PARAMETERS
*    void
* RETURNS
*    kal_uint32
* GLOBALS AFFECTED
*  
******************************************************************/
kal_uint32 aud_media_read_file_and_make_wav_header( void )
{
   // use little endian
   kal_uint16* ptr = aud_context_p->buf_p;
   kal_uint32 chunk_id;
   kal_uint32 chunk_format;
   kal_uint32 chunk_size; // its unit is byte
   kal_uint32 header_len; // its unit is byte
   kal_uint32 sub_chunk_id;
   kal_uint32 sub_chunk_size; // its unit is byte
   kal_uint32 offset; // its unit is byte
   kal_uint16* buffer_read_head;
   kal_uint32 buffer_data_count; // its unit is byte
   INT32 buffer_read_count; // its unit is byte
   kal_uint32 buffer_read_size; // its unit is byte
   INT32 len;
   kal_int32 result;

   // read data
   DRM_read_file( aud_context_p->current_file_handle, aud_context_p->buf_p, aud_context_p->buf_len * 2, &len );
   if( len == 0 )
      return 0;
   
   // parse FORM and format chunk
   chunk_id = (kal_uint32) GET_DWORD(ptr);
   ptr += 2;
   chunk_size = (kal_uint32) GET_DWORD(ptr);
   ptr += 2;
   chunk_format = (kal_uint32) GET_DWORD(ptr);
   ptr += 2;
   sub_chunk_id = (kal_uint32) GET_DWORD(ptr);
   if( chunk_id != 0x46464952 || chunk_format != 0x45564157 || 
      sub_chunk_id != 0x20746D66 ) // check "RIFF", "WAVE", and "fmt "
      return 0xFFFFFFFF;
   
   ptr += 2;
   // get format chunk size
   sub_chunk_size = (kal_uint32) GET_DWORD(ptr);

   // move ptr to the first byte after format chunk
   offset = header_len = 20 + sub_chunk_size;
   buffer_read_head = ptr = aud_context_p->buf_p + (header_len >> 1);

   // search data chunk in buffer
   buffer_read_count = buffer_read_size = len - header_len;
   buffer_data_count = 0;
   do
   {
      kal_int32 bytes_to_end = (kal_int32) buffer_read_count - (kal_int32) buffer_data_count;
      if( bytes_to_end < 8 ) // check if buffer full, we need 8 bytes for chunk id and size
      {
         buffer_data_count = 0;
         ptr = buffer_read_head;
         result = DRM_seek_file( aud_context_p->current_file_handle, offset, FS_FILE_BEGIN );
         if( result < FS_NO_ERROR )
            return 0xFFFFFFFF;
         result = DRM_read_file( aud_context_p->current_file_handle, ptr, buffer_read_size, &buffer_read_count );
         if( result != FS_NO_ERROR )
            return 0xFFFFFFFF;
      }
      sub_chunk_id = GET_DWORD(ptr);
      ptr += 2;
      sub_chunk_size = GET_DWORD(ptr);
      ptr += 2;
      offset += 8; // found data offset
      buffer_data_count += 8;
      if( sub_chunk_id == 0x61746164 ) // check "data"
      {
         buffer_read_size -= 8;
         ptr = buffer_read_head + 4; // its unit is word
         
         ASSERT( offset <= aud_context_p->offset );
         
         // read data from file and append data to header in buffer
         DRM_seek_file( aud_context_p->current_file_handle, aud_context_p->offset, FS_FILE_BEGIN );
         DRM_read_file( aud_context_p->current_file_handle, ptr, buffer_read_size, &buffer_read_count );
         sub_chunk_size -= (aud_context_p->offset - offset);
         len = header_len + 8 + buffer_read_count;
         aud_context_p->offset += buffer_read_count;

         // update header info
         ptr = buffer_read_head; // move ptr to "data" chunk id field
         SET_DWORD(ptr, 0x61746164);
         ptr += 2; // move ptr to data chunk size field
         SET_DWORD(ptr, sub_chunk_size);

         // l1audio wav state restore
         wavSetAdpcmState( KAL_TRUE, 1 );
         break;
      }
      else
      {
         if( (sub_chunk_size & 0x01) != 0 ) // check if it's odd bytes, then pad to even bytes
            sub_chunk_size++;
         buffer_data_count += sub_chunk_size;
         offset += sub_chunk_size;
         ptr += (sub_chunk_size >> 1);
      }
   }
   while( 1 );

   return len;
}

/******************************************************************
* FUNCTION
*    aud_media_read_file_and_make_au_header
* DESCRIPTION
*    This function is to read au file, parse au header and update data offset or size.
* PARAMETERS
*    void
* RETURNS
*    kal_uint32
* GLOBALS AFFECTED
*  
******************************************************************/
kal_uint32 aud_media_read_file_and_make_au_header( void )
{
   // use big endian
   kal_uint8* ptr = (kal_uint8*)aud_context_p->buf_p;
   kal_uint32 magic_id;
   kal_uint32 data_offset;
   kal_uint32 data_size; // its unit is byte
   INT32 buffer_read_count; // its unit is byte
   INT32 len;

   // read data
   DRM_read_file( aud_context_p->current_file_handle, aud_context_p->buf_p, aud_context_p->buf_len * 2, &len );
   if( len == 0 )
      return 0;
   
   // parse header
   magic_id = (kal_uint32) GET_BIG_DWORD(ptr);
   ptr += 4;
   data_offset = (kal_uint32) GET_BIG_DWORD(ptr);
   ptr += 4;
   data_size = (kal_uint32) GET_BIG_DWORD(ptr);
   if( magic_id != 0x2E736E64 ) // check ".snd"
      return 0xFFFFFFFF;
   
   ASSERT( data_offset <= aud_context_p->offset );
   
   // move ptr to the first byte after header
   ptr = ((kal_uint8*)aud_context_p->buf_p) + 28;
   
   // read data chunk in buffer
   DRM_seek_file( aud_context_p->current_file_handle, aud_context_p->offset, FS_FILE_BEGIN );
   DRM_read_file( aud_context_p->current_file_handle, ptr, len - 28, &buffer_read_count );
   data_size -= (aud_context_p->offset - data_offset);
   len = buffer_read_count + 28;
   aud_context_p->offset += buffer_read_count;

   // move ptr to data offset
   ptr = ((kal_uint8*)aud_context_p->buf_p) + 4;
   SET_BIG_DWORD(ptr, 28);

   // move ptr to data chunk size
   ptr += 4;
   SET_BIG_DWORD(ptr, data_size);

   return len;
}

/******************************************************************
* FUNCTION
*    aud_media_read_file_and_make_aiff_header
* DESCRIPTION
*    This function is to read aiff file, parse aiff header and update data offset or size.
* PARAMETERS
*    void
* RETURNS
*    kal_uint32
* GLOBALS AFFECTED
*  
******************************************************************/
kal_uint32 aud_media_read_file_and_make_aiff_header( void )
{
   // use little endian
   kal_uint8* ptr = (kal_uint8*)aud_context_p->buf_p;
   kal_uint8* data_internal_offset_p;
   kal_uint32 form_size;
   kal_uint32 chunk_id;
   kal_uint32 chunk_format;
   kal_uint32 chunk_size;
   kal_uint32 offset;
   kal_uint32 data_offset;
   kal_uint32 data_size;
   kal_uint32 data_internal_offset;
   INT32 len;

   // read data
   DRM_read_file( aud_context_p->current_file_handle, aud_context_p->buf_p, aud_context_p->buf_len * 2, &len );
   if( len == 0 )
      return 0;
   
   // parse FORM and format
   chunk_id = (kal_uint32) GET_BIG_DWORD(ptr);
   ptr += 4;
   form_size = (kal_uint32) GET_BIG_DWORD(ptr);
   ptr += 4;
   chunk_format = (kal_uint32) GET_BIG_DWORD(ptr);
   if( chunk_id != 0x464F524D || (chunk_format != 0x41494646 && chunk_format != 0x41494643) ) // check "FORM", then check "AIFF" or "AIFC"
      return 0xFFFFFFFF;
   ptr += 4;
   offset = 12;
   
   // search sound data chunk in buffer
   data_size = 0;
   do
   {
      chunk_id = (kal_uint32) GET_BIG_DWORD(ptr);
      ptr += 4;
      chunk_size = (kal_uint32) GET_BIG_DWORD(ptr);
      ptr += 4;
      offset += 8;
      if( chunk_id == 0x53534E44 ) // check "SSND"
      {
         data_size = chunk_size;
         data_internal_offset = (kal_uint32) GET_BIG_DWORD(ptr);
         data_internal_offset_p = ptr;
         ptr += 8;
         offset += 8;
         data_offset = offset;
         offset += data_internal_offset; // point to previous data offset when play it
         break;
      }
      else
      {
         if( (chunk_size & 0x01) != 0 ) // check if the chunk size is an odd number
            chunk_size++; // pad the chunk size to an even number
         ptr += chunk_size;
         offset += chunk_size;
      }
   }
   while( offset < len );
   
   if( data_size != 0 && data_offset < len ) // found data chunk
   {
      /* read data from file to buffer */
      DRM_seek_file( aud_context_p->current_file_handle, aud_context_p->offset, FS_FILE_BEGIN );
      DRM_read_file( aud_context_p->current_file_handle, ptr, len - data_offset, &len );
      data_size -= (aud_context_p->offset - offset);
      form_size -= (aud_context_p->offset - offset);
      aud_context_p->offset += len;
      len += data_offset;
      ptr = data_internal_offset_p; // move ptr to data offset field
      SET_BIG_DWORD(ptr, 0);
      ptr -= 4;	// move ptr to chunk size field
      SET_BIG_DWORD(ptr, data_size);
      ptr = ((kal_uint8*)aud_context_p->buf_p) + 4; // move ptr to form size
      SET_BIG_DWORD(ptr, form_size);
   }
   else
      return 0xFFFFFFFF;

   return len;
}

//extern void (*mp3_task_aud_media_event_callback)(unsigned char event);

///extern MP3_INFO Mp3Info;
kal_uint8 aud_media_play_file( kal_bool is_resume )
{
	return MED_RES_OK;
#if 0
  Media_Status result;
  MP3_API_ERR iRet;
   switch (aud_context_p->current_format)
   {
   	case MED_TYPE_DAF:	
	{
		mp3_task_aud_media_event_callback = aud_media_event_callback;
	   iRet = MMF_Mp3Play(aud_context_p->current_file_handle, 1, 100);//chenhe
		if(iRet == MP3_ERR_SUCCESS)
		{
			AUD_ENTER_STATE(AUD_MEDIA_PLAY);
			//result = MEDIA_SUCCESS;
			result = MED_RES_OK;
		}
		else
		{
			//AUD_ENTER_STATE(AUD_MEDIA_PLAY);
			result = MED_RES_FAIL;
		}
   	}	   
	break;
	case MED_TYPE_GSM_EFR:
	case MED_TYPE_AMR:
		
		//result = Media_Play(aud_context_p->current_format, aud_media_play_callback, MEDIA_VMP_AS_RINGTONE);
		result=aud_amr_filePlay(aud_context_p->current_file_handle);//chenhe,070703
		aud_context_p->tone_playing = TRUE;
#ifdef WIN32
		result=MEDIA_SUCCESS;
#endif
	      if (result!=MEDIA_SUCCESS)
	      {
	         /* close the file */
	         aud_close_play_file();

	         /* stop volume crescendo timer */
	         med_stop_timer(AUD_TIMER_CRESCENDO);
	                  
	         /* enter AUD_MEDIA_IDLE state */
	         AUD_ENTER_STATE(AUD_MEDIA_IDLE);

	         return aud_get_res(result);
	      }
	               
	      /* enter AUD_MEDIA_PLAY state */
	      AUD_ENTER_STATE(AUD_MEDIA_PLAY);
	      return MED_RES_OK;
	  
		break;
		
	default:
	   return MEDIA_FAIL;
   }
   return result;
   
   #if 0
   kal_bool is_portion_data_done;
   kal_uint8 result;
   kal_uint8 padding_byte = 0;
   kal_uint32 len;
   kal_uint32 portion_len;
   kal_uint32 offset = aud_context_p->offset;
   
   /* call L1AUD to set buffer */
   Media_SetBuffer(aud_context_p->ring_buf, AUD_RING_BUFFER_LEN);
   Media_GetWriteBuffer(&(aud_context_p->buf_p), (kal_uint32 *)&(aud_context_p->buf_len));
   ASSERT(aud_context_p->buf_p != NULL);
   ASSERT(aud_context_p->buf_len<=AUD_RING_BUFFER_LEN);

   if( !is_resume ) // no need to process audio file header
   {
      DRM_read_file( aud_context_p->current_file_handle, aud_context_p->buf_p, aud_context_p->buf_len*2, &len );
      aud_context_p->offset += len;
      if( aud_context_p->current_format == MED_TYPE_WAV )
         wavSetAdpcmState( KAL_FALSE, 1 );
   }
   else
   {
      /* reader file to buffer and process its header ,and ,if possible, seek pos to previous offset  */
      switch( aud_context_p->current_format )
      {
      case MED_TYPE_WAV:
         len = aud_media_read_file_and_make_wav_header();
         break;
      case MED_TYPE_AU:
         len = aud_media_read_file_and_make_au_header();
         break;
      case MED_TYPE_AIFF:
         len = aud_media_read_file_and_make_aiff_header();
         break;
#ifdef AMRWB_DECODE
      case MED_TYPE_AMR_WB:
#endif
      case MED_TYPE_AMR:
         padding_byte = 0x7C; // padding byte is a AMR "no data" frame if possible
         // continue the following codes, don't add a "break" here
      default:
         DRM_read_file( aud_context_p->current_file_handle, aud_context_p->buf_p, aud_context_p->buf_len*2, &len );
         if( len & 1 != 0 ) // it will be a odd number only when len * 2 < aud_context_p->buf_len
         {
            kal_prompt_trace( MOD_MED, "found a odd number -- %d and add a padding byte -- %d", len, padding_byte );
            ((kal_uint8*)aud_context_p->buf_p)[len++] = padding_byte; // add a padding byte
         }
         aud_context_p->offset += len;
         break;
      }
      if( len == 0xFFFFFFFF ) // this means parse something fail in above statements
      {
         /* close the file */
         aud_close_play_file();
         /* stop volume crescendo timer */
         med_stop_timer(AUD_TIMER_CRESCENDO);
         return MED_RES_BAD_FORMAT;
      }
   }

#ifdef __RICH_AUDIO_PROFILE__
   if( aud_context_p->end_offset != 0 ) // end_offset = 0 means no specific end point, play until file end
   {
      portion_len = aud_context_p->end_offset - offset;
      if( portion_len != 0 && portion_len <= len )
      {
         aud_context_p->offset -= (len - portion_len);
         len = portion_len;
         is_portion_data_done = KAL_TRUE;
      }
      else
         is_portion_data_done = KAL_FALSE;
   }
   else
#endif // __RICH_AUDIO_PROFILE__
      is_portion_data_done = KAL_FALSE;

   len = len >> 1;
   if (len == 0)
   {
      /* close the file */
      aud_close_play_file();
#ifdef WIN32
      /* enter AUD_MEDIA_PLAY state */
      AUD_ENTER_STATE(AUD_MEDIA_PLAY);
      return MED_RES_OK;
#endif
      /* stop volume crescendo timer */
      med_stop_timer(AUD_TIMER_CRESCENDO);

       
       return MED_RES_BAD_FORMAT;
   }
   else if (len < aud_context_p->buf_len || is_portion_data_done)
   {
      aud_context_p->buf_len = len;

      /* inform L1AUD the amount of data read from file to ring buffer */
      Media_WriteDataDone(len);

      Media_DataFinished();

      /* start to play */
      result = Media_Play(aud_context_p->current_format, aud_media_play_callback, MEDIA_VMP_AS_RINGTONE);
#ifdef WIN32
      result=MEDIA_SUCCESS;
#endif
      if (result!=MEDIA_SUCCESS)
      {
         /* close the file */
         aud_close_play_file();

         /* stop volume crescendo timer */
         med_stop_timer(AUD_TIMER_CRESCENDO);
                  
         /* enter AUD_MEDIA_IDLE state */
         AUD_ENTER_STATE(AUD_MEDIA_IDLE);

         return aud_get_res(result);
      }
                
      /* close the file */
      aud_close_play_file();
  
      /* enter AUD_MEDIA_PLAY_FINISH state */
      AUD_ENTER_STATE(AUD_MEDIA_PLAY_FINISH);
      aud_context_p->audio_end=KAL_TRUE;
      return MED_RES_OK;
   }
   else
   {
      /* inform L1AUD the amount of data read from file to ring buffer */
      Media_WriteDataDone(len);

      /* start to play */
      result = Media_Play(aud_context_p->current_format, aud_media_play_callback, MEDIA_VMP_AS_RINGTONE);
#ifdef WIN32
      result=MEDIA_SUCCESS;
#endif
      if (result!=MEDIA_SUCCESS)
      {
         /* close the file */
         aud_close_play_file();

         /* stop volume crescendo timer */
         med_stop_timer(AUD_TIMER_CRESCENDO);
                  
         /* enter AUD_MEDIA_IDLE state */
         AUD_ENTER_STATE(AUD_MEDIA_IDLE);

         return aud_get_res(result);
      }
               
      /* enter AUD_MEDIA_PLAY state */
      AUD_ENTER_STATE(AUD_MEDIA_PLAY);
      return MED_RES_OK;
   }
   #endif
#endif
}

/******************************************************************
* FUNCTION
*    aud_media_play_as_song
* DESCRIPTION
*    This function is to handle resume playing audio streamingly.
* PARAMETERS
*    kal_wchar* file_name, kal_uint8 play_style, kal_uint8 first_time, kal_bool is_resume
*    
* RETURNS
*    kal_uint8
* GLOBALS AFFECTED
*  
*  
******************************************************************/
kal_uint8 aud_media_play_as_song(kal_wchar* file_name, kal_uint8 play_style, kal_uint8 first_time, kal_bool is_resume)
{
   kal_uint8 result;


   aud_context_p->source_type = AUD_FILE;
   aud_context_p->play_style = play_style; 
    mmi_trace(TRUE, "chenhe : aud_media_play_as_song,aud_context_p->state is %d",aud_context_p->state);

   /* apply volume and play style only in first loop */
   if(first_time)
   {
      /* follow ring tone volume */
      aud_set_active_ring_tone_volume();
      /* To start ascending timer */
      aud_melody_play_style_convert(play_style);
   }   
   
   switch (aud_context_p->state)
   {
      case AUD_MEDIA_PLAY_PAUSED:
      {
         /* close the file */
         aud_close_play_file();

         /* enter AUD_MEDIA_IDLE state */
         AUD_ENTER_STATE(AUD_MEDIA_IDLE);
         /* continue  to play the file */
         //break;
      }
	  break;
      case AUD_MEDIA_IDLE:
      {
         /* open file for playing */
         aud_context_p->audio_end = KAL_FALSE;
         if( is_resume )
            aud_context_p->offset = aud_context_p->last_offset;
         else
#ifdef __RICH_AUDIO_PROFILE__
            aud_context_p->offset = aud_context_p->start_offset;

         if( aud_context_p->end_offset != 0 && aud_context_p->offset > aud_context_p->end_offset )
         {
            /* stop volume crescendo timer */
            med_stop_timer(AUD_TIMER_CRESCENDO);
            
            return MED_RES_FAIL;
         }
#else
            aud_context_p->offset = 0;
#endif // __RICH_AUDIO_PROFILE__

         result = aud_open_play_file((kal_wchar*)file_name);
    mmi_trace(TRUE, "chenhe : aud_media_play_as_song,result is %d",result);

         if( result != MED_RES_OK )
         {
            /* stop volume crescendo timer */
            med_stop_timer(AUD_TIMER_CRESCENDO);
            
            return result;
         }

         return aud_media_play_file( is_resume );
      }
      default:
      {
      	  /* stop volume crescendo timer */
         med_stop_timer(AUD_TIMER_CRESCENDO);
      	  
         return MED_RES_BUSY;
      }
   }
   return MED_RES_FAIL;
}

#if (CSW_EXTENDED_API_AUDIO_VIDEO==1)
/******************************************************************
* FUNCTION
*    aud_media_play_ind_hdlr
* DESCRIPTION
*    This function is to handle the audio play indicator in case of CSW_EXTENDED_API
*    This is the callback function called by MCI_Audio in case of change of
*    state of the player
* PARAMETERS
*    MCI_STATE_T state : current state of the player
*    
* RETURNS
*    None
* GLOBALS AFFECTED
*  
*  
******************************************************************/
VOID aud_media_play_ind_hdlr(MCI_STATE_T state)
{
    l4aud_media_play_finish_ind_struct *ind_p;

    switch (state)
    {
        case MCI_STATE_EOF:
    		mmi_trace(1,"aud_media_play_ind_hdlr: State %d",state );

            // Send indicator to the MEDIA task 
            ind_p = (l4aud_media_play_finish_ind_struct*)
                construct_local_para(sizeof(l4aud_media_play_finish_ind_struct), TD_CTRL);
            ind_p->src_id = aud_context_p->src_id;
            ind_p->result = MED_RES_END_OF_FILE;
            ind_p->identifier = aud_context_p->identifier;
            aud_send_ilm(MOD_MED, 
               MSG_ID_L4AUD_MEDIA_PLAY_FINISH_IND, 
               ind_p, 
               NULL);
          
        break;
        default:
    		mmi_trace(1,"aud_media_play_ind_hdlr: Unknown State %d",state);
        break;
    }
}
#else

/******************************************************************
* FUNCTION
*    aud_media_play_ind_hdlr
* DESCRIPTION
*    This function is to handle the audio play indicator in case of CSW_EXTENDED_API
*    This is the callback function called by MCI_Audio in case of change of
*    state of the player
* PARAMETERS
*    MCI_STATE_T state : current state of the player
*    
* RETURNS
*    None
* GLOBALS AFFECTED
*  
*  
******************************************************************/

VOID aud_media_play_ind_hdlr(MCI_ERR_T result) //         MCI_MEDIA_PLAY_FINISH_IND,
{


    l4aud_media_play_finish_ind_struct *ind_p;

    
   ind_p = ( l4aud_media_play_finish_ind_struct*)
            construct_local_para(sizeof( l4aud_media_play_finish_ind_struct), 2);  

   
    switch (result)
    {
        case MCI_ERR_END_OF_FILE:
		mmi_trace(1,"aud_media_play_ind_hdlr: State %d",result );

               // Send indicator to the MEDIA task 
   		ind_p->result = MED_RES_END_OF_FILE;
  		ind_p->identifier = 0; 
        break;

        case MCI_ERR_INVALID_FORMAT:
		mmi_trace(1,"aud_media_play_ind_hdlr: State %d",result );
    		
               // Send indicator to the MEDIA task 
   		ind_p->result = MED_RES_INVALID_FORMAT;
  		ind_p->identifier = 0; 
        break;

        default:
    		mmi_trace(1,"aud_media_play_ind_hdlr: Unknown State %d",result);
        break;
    }


   MusicEndAudioMode();
   
   aud_send_ilm(MOD_MED, 
               //MSG_ID_L4AUD_AUDIO_PLAY_FINISH_IND, 
               MSG_ID_L4AUD_MEDIA_PLAY_FINISH_IND,
               ind_p, 
               NULL);

}




#endif

/******************************************************************
* FUNCTION
*    aud_media_play_req_hdlr
* DESCRIPTION
*    This function is to handle the audio play request.
* PARAMETERS
*    ilm_struct *ilm_ptr
*    
* RETURNS
*    None
* GLOBALS AFFECTED
*  
*  
******************************************************************/
extern S8 *UCS2Strcpy(S8 *strDestination, const S8 *strSource );
void aud_media_play_req_hdlr(ilm_struct *ilm_ptr)
{
	int32 result;

	l4aud_media_play_req_struct* msg_p = NULL;
	msg_p = (l4aud_media_play_req_struct*)ilm_ptr->local_para_ptr;
		mmi_trace(1,"aud_media_play_req_hdlr,entry,aud_context_p->state  is %d",aud_context_p->state );


		    if (aud_context_p->speech_on
    #if defined(__MED_VID_MOD__)  || defined(__MED_MJPG_MOD__)
        || !vid_is_audio_channel_available()
    #endif 
        )
    {
        aud_set_result(MED_RES_BUSY);
        AUD_SET_EVENT(AUD_EVT_PLAY);
	result= MED_RES_BUSY;
        if (aud_context_p->src_mod != MOD_MMI)
        {
            aud_send_media_play_cnf(MED_RES_BUSY);
        }
    }
	else
{
      if (!(aud_context_p->state == AUD_MEDIA_RECORD ||
         aud_context_p->state == AUD_MEDIA_RECORD_PAUSED ||
         aud_context_p->state == AUD_VM_RECORD ||
         aud_context_p->state == AUD_VM_RECORD_PAUSED))
      {
         aud_stop_unfinished_process();
      	}
	
	aud_context_p->current_file_handle = MMI_FS_Open((const UINT8*)msg_p->file_name, FS_READ_ONLY);
	UCS2Strcpy((S8*)aud_context_p->current_file_name,(const UINT8*)msg_p->file_name);
	mmi_trace(g_sw_MED_Aud,"aud_media_play_req_hdlr,aud_context_p->current_file_handle is %d",aud_context_p->current_file_handle);

	if(aud_context_p->current_file_handle<0)
	{
		result = -1;
		goto play_req_end;
	}
	
	aud_keytone_stop();//chenhe,20081015,add for to stop key tone


    //jiashuo add for jessamine debug start,20080705
	aud_context_p->source_type = AUD_FILE;
	/* set current file foramt */
	aud_context_p->current_format = med_get_media_type((kal_wchar*)msg_p->file_name);
    //jiashuo add end	
    mmi_trace(g_sw_MED_Aud,"aud_media_play_req_hdlr,aud_context_p->current_format is %d",aud_context_p->current_format);

            #ifdef __MED_BT_A2DP_MOD__
            #if (defined(__BTCOOLSAND__)||defined(__BTRDA__))

            if (aud_context_p->current_format  != MED_TYPE_AMR )  // AMR cann't use A2DP 
                {
			mmi_trace(g_sw_BT, "aud_bt_a2dp_open_codec::aud_context_p->current_format= %d", aud_context_p->current_format);
        	    aud_bt_a2dp_open_codec(aud_context_p->current_format);
                }
            #endif
            #endif /* __MED_BT_A2DP_MOD__ */ 

	// MCI_AudioSetVolume(msg_p->volume+1);
#if (CSW_EXTENDED_API_AUDIO_VIDEO==1)
    {
     UINT8* fileName; 	 	 
     UINT8* unixFileName; 	 	 
     fileName = OslMalloc(MAX_MEDIA_FILE_NAME);  
     unixFileName =  OslMalloc(MAX_MEDIA_FILE_NAME);
     ConvertRelative2Full(fileName, (UINT8*) msg_p->file_name );
     ConvertDosName2Unix(unixFileName, fileName);
	 result=MCI_AudioPlay(1,unixFileName,aud_media_play_ind_hdlr);
     MCI_AudioSetVolume(msg_p->volume+1);
     OslMfree(fileName);
     OslMfree(unixFileName);
    }
#else
{


    mci_type_enum audio_fromat_type;

    switch (aud_context_p->current_format)
    {
    
        case MED_TYPE_DAF:
		   audio_fromat_type=MCI_TYPE_DAF;
        break;

        case MED_TYPE_AMR:
		   audio_fromat_type=MCI_TYPE_AMR;
        break;

	 case MED_TYPE_AAC:
		   audio_fromat_type=MCI_TYPE_AAC;
        break;
		
        case MED_TYPE_WAV:
		   audio_fromat_type=MCI_TYPE_WAV;
        break;
	case MED_TYPE_MID:
		audio_fromat_type = MCI_TYPE_MIDI;
		break;
        default:
		audio_fromat_type=MCI_TYPE_NONE;
    		mmi_trace(1,"aud_media_play_req_hdlr: Unknown Multimedia Type %d",aud_context_p->current_format);
        break;
    }




	result=MCI_AudioPlay(0,aud_context_p->current_file_handle,audio_fromat_type,aud_media_play_ind_hdlr,msg_p->start_offset);
	mmi_trace(g_sw_MED_Aud,"aud_media_play_req_hdlr,result is %d,msg_p->volume+1 is %d",result,msg_p->volume+1);


 }
#endif
}
play_req_end:	 
	if(result == MED_RES_OK)	
	{
		DM_SetAudioVolume(msg_p->volume);
		MusicStartAudioMode();
		SET_CURRENT_VOLUME_LEVEL(AUD_VOLUME_MEDIA, msg_p->volume);//jiashuo add 20080908,save the volume level for resume operation
		aud_context_p->tone_playing = TRUE;
		AUD_ENTER_STATE(AUD_MEDIA_PLAY);
		aud_context_p->play_mode = AUD_MEDIA_PLAY_AS_SONG;

		aud_set_result((kal_int32)MED_RES_OK);
		AUD_SET_EVENT(AUD_EVT_PLAY);
	}
	else
	{
		aud_context_p->tone_playing = FALSE;
		MusicEndAudioMode();
		AUD_ENTER_STATE(AUD_MEDIA_IDLE);
		aud_context_p->play_mode = AUD_MEDIA_PLAY_MODE_NONE;
		 aud_set_result((kal_int32)result);
		 aud_close_play_file();
		 AUD_SET_EVENT(AUD_EVT_PLAY);
	}


 }

//chenhe add for replay a file in infinite style, not for player's replay a file
void mmi_replay_last_file(void)
{
	mci_type_enum audio_fromat_type;
	INT32 result=-1;
	switch (aud_context_p->current_format)
	{

		case MED_TYPE_DAF:
		   audio_fromat_type=MCI_TYPE_DAF;
		break;

		case MED_TYPE_AMR:
		   audio_fromat_type=MCI_TYPE_AMR;
		break;

		case MED_TYPE_AAC:
		   audio_fromat_type=MCI_TYPE_AAC;
		break;

		case MED_TYPE_WAV:
		   audio_fromat_type=MCI_TYPE_WAV;
		break;
		case MED_TYPE_MID:
		   audio_fromat_type=MCI_TYPE_MIDI;
		break;
			
		default:
		audio_fromat_type=MCI_TYPE_NONE;
			mmi_trace(1,"mmi_replay_last_file: Unknown Multimedia Type %d",aud_context_p->current_format);
	break;
	}

	result=MCI_AudioPlay(0,aud_context_p->current_file_handle,audio_fromat_type,aud_media_play_ind_hdlr,0);
	//DM_SetAudioVolume(5+1);



	if(result == MED_RES_OK)	
	{
		MusicStartAudioMode();
		//SET_CURRENT_VOLUME_LEVEL(AUD_VOLUME_MEDIA, 5+1);//jiashuo add 20080908,save the volume level for resume operation
		AUD_ENTER_STATE(AUD_MEDIA_PLAY);	
		aud_context_p->play_mode = AUD_MEDIA_PLAY_AS_SONG;
	}
	else
	{
		AUD_ENTER_STATE(AUD_MEDIA_IDLE);	
		aud_context_p->play_mode = AUD_MEDIA_PLAY_MODE_NONE;

	}
	mmi_trace(1,"chenhe,mmi_replay_last_file,result is %d,aud_context_p->current_file_handle is %d",result,aud_context_p->current_file_handle);
}
/******************************************************************
* FUNCTION
*    aud_media_stop_req_hdlr
* DESCRIPTION
*    This function is to handle the audio stop request.
* PARAMETERS
*    ilm_struct *ilm_ptr
*    
* RETURNS
*    None
* GLOBALS AFFECTED
*  
*  
******************************************************************/
UINT32 aud_getFilePlayPercent(void)
{
	return audiofile_lastPlayInfo.PlayProgress;
}

void aud_media_stop_req_hdlr(ilm_struct *ilm_ptr)
	{


	kal_int32   result = MED_RES_OK;


	TBM_ENTRY(0x2A11);
	aud_context_p->src_mod = ilm_ptr->src_mod_id;
	mmi_trace(1,"chenhe,aud_media_stop_req_hdlr->aud_context_p->state:%d",aud_context_p->state);
	//chenhe test jasperII +
	switch(aud_context_p->state)
	{
		case AUD_MEDIA_IDLE:
		break;
		case AUD_MEDIA_PLAY_PAUSED://jiashuo add 
		aud_context_p->current_format=MED_TYPE_NONE;//jiashuo add 20080705
		aud_context_p->tone_playing = FALSE; 
		MusicEndAudioMode();
		aud_close_play_file();
		break;
		
		case AUD_MEDIA_PLAY:
		
			if(!audioplayer_restore_flag &&(mmi_audply_is_playing()||mmi_audio_need_replay()))
			{
				audioplayer_restore_flag = 1;
				mmi_trace(1,"chenhe,aud_media_stop_req_hdlr,111,audiofile_lastPlayInfo.PlayProgress is %d",audiofile_lastPlayInfo.PlayProgress);
				MCI_AudioGetPlayInformation(&audiofile_lastPlayInfo);
			}
			mmi_trace(1,"chenhe,aud_media_stop_req_hdlr,audiofile_lastPlayInfo is %d,mmi_audio_need_replay() is %d",audiofile_lastPlayInfo.PlayProgress,mmi_audio_need_replay());
			MCI_AudioStop();
			aud_context_p->current_format=MED_TYPE_NONE;//jiashuo add 20080705
			aud_context_p->tone_playing = FALSE; 
			MusicEndAudioMode();
			aud_close_play_file();
			break;
		
#ifdef SOUND_RECORDER_SUPPORT	  		
		case AUD_MEDIA_RECORD:
		case AUD_MEDIA_RECORD_PAUSED:
			MCI_AudioRecordStop();
			aud_close_record_file();
			break;
#endif

		default:
		break;
	}
	aud_set_result(result);
	AUD_SET_EVENT(AUD_EVT_STOP);
	/* enter AUD_MEDIA_IDLE state */
	AUD_ENTER_STATE(AUD_MEDIA_IDLE);
	//chenhe test jasperII -
	}


/******************************************************************
* FUNCTION
*    aud_media_pause_req_hdlr
* DESCRIPTION
*    This function is to handle the audio pause request.
* PARAMETERS
*    ilm_struct *ilm_ptr
*    
* RETURNS
*    None
* GLOBALS AFFECTED
*  
*  
******************************************************************/

void aud_media_pause_req_hdlr(ilm_struct *ilm_ptr)
{
   TBM_ENTRY(0x2A12);


   kal_uint32   res = MED_RES_OK;
   l4aud_media_pause_req_struct* msg_p = NULL;

   aud_context_p->src_mod = ilm_ptr->src_mod_id;

   msg_p = (l4aud_media_pause_req_struct*)ilm_ptr->local_para_ptr;

   aud_context_p->src_id = msg_p->src_id;
mmi_trace(1,"chenhe,aud_media_pause_req_hdlr,aud_context_p->state is %d",aud_context_p->state);
	switch(aud_context_p->state)
	{
		case AUD_MEDIA_IDLE:
			res = MED_RES_OK;
			break;
		case AUD_MEDIA_PLAY:
		case AUD_MEDIA_PLAY_FINISH:
			//res= MCI_AudioPause();
			MCI_AudioGetPlayInformation(&PlayInformation);
			res= MCI_AudioStop();
			#ifdef AUD_PAUSE_CLOSE_HANDLE
			aud_close_play_file();
			MusicEndAudioMode();
			#endif
			mmi_trace(1,"chenhe,aud_media_pause_req_hdlr,stat play,pause result is %d",res);
			AUD_ENTER_STATE(AUD_MEDIA_PLAY_PAUSED);
			aud_context_p->tone_playing = FALSE; 
		//	MusicEndAudioMode();
			if(GetHandsetInPhone()!=1)
				GpioSpeakerModeEnd();

			break;
#ifdef SOUND_RECORDER_SUPPORT			
		case AUD_MEDIA_RECORD:	
			res=MCI_AudioRecordPause();
			
			AUD_ENTER_STATE(AUD_MEDIA_RECORD_PAUSED);			
			break;
#endif			
		default:
				break;
	}
	mmi_trace(1,"chenhe,aud_media_pause_req_hdlr,aud_context_p->state is %d,res is %d",aud_context_p->state,res);

   aud_set_result((kal_int32)res);
   AUD_SET_EVENT(AUD_EVT_PAUSE);


}


/******************************************************************
* FUNCTION
*    aud_media_resume_req_hdlr
* DESCRIPTION
*    This function is to handle the audio resume request.
* PARAMETERS
*    ilm_struct *ilm_ptr
*    
* RETURNS
*    None
* GLOBALS AFFECTED
*  
*  
******************************************************************/
void aud_media_resume_req_hdlr(ilm_struct *ilm_ptr)
{
   TBM_ENTRY(0x2A13);
   l4aud_media_resume_req_struct* msg_p = NULL;
   kal_uint8 res=MED_RES_OK;
   mci_type_enum audio_fromat_type;

 mmi_trace(1,"chenhe,aud_media_resume_req_hdlr,aud_context_p->state is %d",aud_context_p->state);
   aud_context_p->src_mod = ilm_ptr->src_mod_id;

   msg_p = (l4aud_media_resume_req_struct*)ilm_ptr->local_para_ptr;

   aud_context_p->src_id = msg_p->src_id;
    switch (aud_context_p->current_format)
    {
    
        case MED_TYPE_DAF:
		   audio_fromat_type=MCI_TYPE_DAF;
        break;

        case MED_TYPE_AMR:
		   audio_fromat_type=MCI_TYPE_AMR;
        break;

	 case MED_TYPE_AAC:
		   audio_fromat_type=MCI_TYPE_AAC;
        break;
		
        case MED_TYPE_WAV:
		   audio_fromat_type=MCI_TYPE_WAV;
        break;
        case MED_TYPE_MID:
		audio_fromat_type=MCI_TYPE_MIDI;
        break;
			
        default:
		audio_fromat_type=MCI_TYPE_NONE;
    		mmi_trace(1,"aud_media_resume_req_hdlr: Unknown Multimedia Type %d",aud_context_p->current_format);
        break;
    }
	switch(aud_context_p->state)
	{
		case AUD_MEDIA_IDLE:
			mmi_trace(1,"aud is idle");
			res = MED_RES_FAIL;
			break;
		case AUD_MEDIA_PLAY_PAUSED:
			#ifdef AUD_PAUSE_CLOSE_HANDLE
			aud_context_p->current_file_handle = MMI_FS_Open((const UINT8*)aud_context_p->current_file_name, FS_READ_ONLY);
			#endif
			#if (defined(__BTCOOLSAND__)||defined(__BTRDA__)) && defined(__MED_BT_A2DP_MOD__)
                          if (aud_context_p->current_format  != MED_TYPE_AMR )  // AMR cann't use A2DP 
                        {
    				aud_bt_a2dp_open_codec((kal_int32)aud_context_p->current_format);
                           }
			#endif
		    DM_SetAudioVolume(aud_get_volume_level(aud_context_p->audio_mode, AUD_VOLUME_MEDIA));//jiashuo add 20080908,restore the volume level from the value saved in play function
			mmi_trace(g_sw_AUDIO,"######volume:%d,aud_context_p->current_file_handle is %d",aud_get_volume_level(aud_context_p->audio_mode, AUD_VOLUME_MEDIA),aud_context_p->current_file_handle);
			COS_Sleep(200);
			#ifdef AUD_PAUSE_CLOSE_HANDLE
			if(aud_context_p->current_file_handle>=0)
			{
			//	res = MCI_AudioResume(aud_context_p->current_file_handle);
			res = MCI_AudioPlay(0,aud_context_p->current_file_handle,audio_fromat_type,aud_media_play_ind_hdlr,PlayInformation.PlayProgress);
			MusicStartAudioMode();
			}
			#else
			//res = MCI_AudioResume();
			res = MCI_AudioPlay(0,aud_context_p->current_file_handle,audio_fromat_type,aud_media_play_ind_hdlr,PlayInformation.PlayProgress);
			#endif
			mmi_trace(1,"aud is paused,resume result is %d",res);
			AUD_ENTER_STATE(AUD_MEDIA_PLAY);
			aud_context_p->tone_playing = TRUE; //add by jiashuo
			break;
#ifdef SOUND_RECORDER_SUPPORT			
		case AUD_MEDIA_RECORD_PAUSED:
			res=MCI_AudioRecordResume();
			AUD_ENTER_STATE(AUD_MEDIA_RECORD);			
			break;
#endif			
		default:
			break;
	}

      aud_set_result((kal_int32)res);
   AUD_SET_EVENT(AUD_EVT_RESUME);
   if(aud_context_p->src_mod!=MOD_MMI)
      aud_send_media_resume_cnf(res);

   TBM_EXIT(0x2A13);
}

/******************************************************************
* FUNCTION
*    aud_media_stop_and_store_info
* DESCRIPTION
*    This function is to stop media play and store play info
* PARAMETERS
*    void
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void aud_media_stop_and_store_info( void )
{
   // avoid other type of stop procedures
   if( aud_context_p->source_type > AUD_STRING )
      return;
   
   switch( aud_context_p->current_format )
   {
#ifdef __MED_MMA_MOD__
      case MED_TYPE_SMF:
      case MED_TYPE_SMF_SND:
         ASSERT( aud_context_p->last_handle != NULL );
         aud_context_p->last_time = temp_last_time = JSmf_GetCurrentTime( aud_context_p->last_handle );
         JSmf_Stop( aud_context_p->last_handle );
         JSmf_Close( aud_context_p->last_handle );
         aud_context_p->last_handle = NULL;
         kal_prompt_trace( MOD_MED, "stop and store info: temp_last_time = %d", temp_last_time );
         break;
      case MED_TYPE_IMELODY:
         ASSERT( aud_context_p->last_handle != NULL );
         aud_context_p->last_time = temp_last_time = JImy_GetCurrentTime( aud_context_p->last_handle );
         JImy_Stop( aud_context_p->last_handle );
         JImy_Close( aud_context_p->last_handle );
         aud_context_p->last_handle = NULL;
         kal_prompt_trace( MOD_MED, "stop and store info: temp_last_time = %d", temp_last_time );
         break;
#ifdef MMF_DECODE
      case MED_TYPE_MMF:
         ASSERT( aud_context_p->last_handle != NULL );
         aud_context_p->last_time = temp_last_time = JMmf_GetCurrentTime( aud_context_p->last_handle );
         JMmf_Stop( aud_context_p->last_handle );
         JMmf_Close( aud_context_p->last_handle );
         aud_context_p->last_handle = NULL;
         kal_prompt_trace( MOD_MED, "stop and store info: temp_last_time = %d", temp_last_time );
         break;
#endif
      case MED_TYPE_AMR:
      case MED_TYPE_AMR_WB:
         Media_Stop();
         aud_context_p->last_offset = temp_last_offset = aud_context_p->offset  - amrGetDataCount();
         kal_prompt_trace( MOD_MED, "stop and store info: offset = %d, temp_last_offset = %d", aud_context_p->offset, temp_last_offset );
#ifdef __RICH_AUDIO_PROFILE__
         amr_seek_count = 0;
#endif
         break;
#endif
      case MED_TYPE_WAV:
         aud_context_p->is_data_consumed = temp_is_data_consumed = mediaIsDataConsumed();
         Media_Stop();
         aud_context_p->last_offset = temp_last_offset = aud_context_p->offset  - (mediaGetDataCount() << 1);
         wavGetAdpcmState( 0 );
         wavCopyAdpcmState( 0, 1 );
         kal_prompt_trace( MOD_MED, "stop and store info: offset = %d, temp_last_offset = %d", aud_context_p->offset, temp_last_offset );
         break;
#ifdef WMA_DECODE
      case MED_TYPE_WMA:
	  	 #if 0//jiashuo del for jasper 20080102
         ASSERT( aud_context_p->last_handle != NULL );
         aud_context_p->last_time = temp_last_time = WMA_GetCurrentTime( aud_context_p->last_handle );
         WMA_Stop( aud_context_p->last_handle );
         WMA_Close( aud_context_p->last_handle );
         FSAL_Close( &current_file_stream );
         aud_context_p->last_handle = NULL;
         kal_prompt_trace( MOD_MED, "stop and store info: temp_last_time = %d", temp_last_time );
		 #else
		 #endif 
         break;
#endif // WMA_DEBUG
#ifdef AAC_DECODE
      case MED_TYPE_AAC:
		 #if 0//jiashuo del for jasper 20080102
         ASSERT( aud_context_p->last_handle != NULL );
         aud_context_p->last_time = temp_last_time = AAC_GetCurrentTime( aud_context_p->last_handle );
         AAC_Stop( aud_context_p->last_handle );
         AAC_Close( aud_context_p->last_handle );
         FSAL_Close( &current_file_stream );
         aud_context_p->last_handle = NULL;
         kal_prompt_trace( MOD_MED, "stop and store info: temp_last_time = %d", temp_last_time );
		 #else
		 #endif 
         break;
#endif // AAC_DECODE
      default:
         aud_context_p->is_data_consumed = temp_is_data_consumed = mediaIsDataConsumed();
         Media_Stop();
         aud_context_p->last_offset = temp_last_offset = aud_context_p->offset  - (mediaGetDataCount() << 1);
         kal_prompt_trace( MOD_MED, "stop and store info: offset = %d, temp_last_offset = %d", aud_context_p->offset, temp_last_offset );
         break;
   }
   aud_context_p->last_seq_no++;
}

kal_int32 aud_media_get_stream_progress( kal_uint32 *progress )
{
   kal_uint32 savedMask;
   
   switch( aud_context_p->current_format )
   {
#ifdef __MED_MMA_MOD__
      case MED_TYPE_SMF:
      case MED_TYPE_SMF_SND:
      case MED_TYPE_IMELODY:
         *progress = 0;
         return MED_RES_FAIL;
#ifdef MMF_DECODE
      case MED_TYPE_MMF:
         *progress = 0;
         return MED_RES_FAIL;
#endif
      case MED_TYPE_AMR:
      case MED_TYPE_AMR_WB:
         savedMask = SaveAndSetIRQMask();
         *progress = aud_context_p->offset  - amrGetDataCount();
         RestoreIRQMask( savedMask );
         break;
#endif
      default:
         if( mediaIsDataConsumed() )
         {
            savedMask = SaveAndSetIRQMask();
            *progress = aud_context_p->offset  - (mediaGetDataCount() << 1);
            RestoreIRQMask( savedMask );
         }
         else
            *progress = 0;
         break;
   }

   return MED_RES_OK;
}


/*****************************************************************************
 * FUNCTION
 *  aud_get_file_data_length_req_hdlr
 * DESCRIPTION
 *  This function is to get the time length of the audio file playing
 * PARAMETERS
 *  ilm_ptr     [?]     
 * RETURNS
 *  kal_uint8(?)
 *****************************************************************************/
void aud_get_file_data_length_req_hdlr( ilm_struct* ilm_ptr )
{
/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int32 result= MED_RES_OK;
    media_aud_get_file_data_length_req_struct* msg_p = msg_p = (media_aud_get_file_data_length_req_struct*)ilm_ptr->local_para_ptr;
    mci_type_enum audio_fromat_type;
    MCI_ProgressInf PlayTimeInformation;

	
	PlayInformation.PlayProgress = msg_p->progress;

	aud_context_p->current_file_handle = MMI_FS_Open((const UINT8*)msg_p->file_name, FS_READ_ONLY);
	
	UCS2Strcpy((S8*)aud_context_p->current_file_name,(const UINT8*)msg_p->file_name);

	if(aud_context_p->current_file_handle<0)
	{
		result = -1;
		goto play_req_end;
	}
	aud_context_p->source_type = AUD_FILE;

	aud_context_p->current_format = med_get_media_type((kal_wchar*)msg_p->file_name);

#ifdef __MED_BT_A2DP_MOD__
#if (defined(__BTCOOLSAND__)||defined(__BTRDA__))
          if (aud_context_p->current_format  != MED_TYPE_AMR )  // AMR cann't use A2DP 
                {
		mmi_trace(g_sw_BT, "aud_bt_a2dp_open_codec::aud_context_p->current_format= %d", aud_context_p->current_format);
		aud_bt_a2dp_open_codec(aud_context_p->current_format);
            }
#endif
#endif /* __MED_BT_A2DP_MOD__ */ 
    
    switch (aud_context_p->current_format)
    {
    
        case MED_TYPE_DAF:
		   audio_fromat_type=MCI_TYPE_DAF;
        break;

        case MED_TYPE_AMR:
		   audio_fromat_type=MCI_TYPE_AMR;
        break;

	 case MED_TYPE_AAC:
		   audio_fromat_type=MCI_TYPE_AAC;
        break;
		
        case MED_TYPE_WAV:
		   audio_fromat_type=MCI_TYPE_WAV;
        break;
        case MED_TYPE_MID:
		   audio_fromat_type=MCI_TYPE_MIDI;
        break;
			
        default:
		audio_fromat_type=MCI_TYPE_NONE;
    		mmi_trace(1,"aud_get_file_data_length_req_hdlr: Unknown Multimedia Type %d",aud_context_p->current_format);
        break;
    }
if(aud_context_p->state != AUD_MEDIA_PLAY)
{
	result =MCI_AudioGetDurationTime(aud_context_p->current_file_handle, audio_fromat_type,0,
										msg_p->progress,&PlayTimeInformation);
}
	play_req_end:	 
	if(result == MED_RES_OK)	
	{
		aud_set_pair_results(MED_RES_OK, PlayTimeInformation.DurationTime);
	}
	else
	{
		 aud_set_result((kal_int32)result);
	}
     AUD_SET_EVENT(AUD_EVT_GET_FILE_DATA_LEN);

}



U32 Get_Audio_duration(U8 *pFileName) 
{
/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	UINT32 ret = 0;
    mci_type_enum audio_fromat_type;
    MCI_ProgressInf PlayTimeInformation;
	INT32   current_file_handle;
	INT16   current_format;

	current_file_handle = MMI_FS_Open((const UINT8*)pFileName, FS_READ_ONLY);
	

	if(current_file_handle<0)
	{
		mmi_trace(TRUE," Func: %s current_file_handle=%d", __FUNCTION__, current_file_handle);
		return 0;
	}

	current_format = med_get_media_type((kal_wchar*)pFileName);
    
    switch (current_format)
    {
    
        case MED_TYPE_DAF:
		   audio_fromat_type=MCI_TYPE_DAF;
        break;

        case MED_TYPE_AMR:
		   audio_fromat_type=MCI_TYPE_AMR;
        break;

	 case MED_TYPE_AAC:
		   audio_fromat_type=MCI_TYPE_AAC;
        break;
		
        case MED_TYPE_WAV:
		   audio_fromat_type=MCI_TYPE_WAV;
        break;
        case MED_TYPE_MID:
		   audio_fromat_type=MCI_TYPE_MIDI;
        break;
			
        default:
		audio_fromat_type=MCI_TYPE_NONE;
    		mmi_trace(1,"Get_Audio_duration: Unknown Multimedia Type %d",aud_context_p->current_format);
        break;
    }
	mdi_audio_suspend_background_play();
	ret = MCI_AudioGetDurationTime(current_file_handle, audio_fromat_type,0,
										AUD_FULL_PROGRESS_CNT,&PlayTimeInformation);
	mdi_audio_resume_background_play();

	MMI_FS_Close(current_file_handle);
	mmi_trace(TRUE,"Func: %s ret=%d, audio_fromat_type=%d, PlayTimeInformation.DurationTime=%d", __FUNCTION__, ret, audio_fromat_type, PlayTimeInformation.DurationTime);
	if (ret == 0)
		return PlayTimeInformation.DurationTime;
	else
		return 0;

}

/*****************************************************************************
 * FUNCTION
 *  aud_get_file_data_progress_req_hdlr
 * DESCRIPTION
 *  This function is to get the progress of the audio file playing
 * PARAMETERS
 *  ilm_ptr     [?]     
 * RETURNS
 *  kal_uint8(?)
 *****************************************************************************/
void aud_get_file_data_progress_req_hdlr(ilm_struct *ilm_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MCI_PlayInf Play_Information;

	
     MCI_AudioGetPlayInformation(&Play_Information);
     aud_set_pair_results(MED_RES_OK, Play_Information.PlayProgress);
     AUD_SET_EVENT(AUD_EVT_GET_FILE_DATA_PROGRESS);
}
void aud_set_file_data_progress_req_hdlr( ilm_struct* ilm_ptr )
	{

    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int32 result= MED_RES_OK;
    mci_type_enum audio_fromat_type;
    MCI_ProgressInf PlayTimeInformation;
    media_aud_set_file_data_progress_req_struct* msg_p = (media_aud_set_file_data_progress_req_struct*)ilm_ptr->local_para_ptr;

	if(msg_p->progress >AUD_FULL_PROGRESS_CNT)
	{
		 if(aud_context_p->current_file_handle!=-1)
		 	{
     			 MMI_FS_Close(aud_context_p->current_file_handle);
	  		 aud_context_p->current_file_handle=-1;
		 	}
		AUD_SET_EVENT(AUD_EVT_SET_FILE_DATA_PROGRESS);
		return;
	}


    PlayInformation.PlayProgress = msg_p->progress;


	switch (aud_context_p->current_format)
    {
    
        case MED_TYPE_DAF:
		   audio_fromat_type=MCI_TYPE_DAF;
        break;

        case MED_TYPE_AMR:
		   audio_fromat_type=MCI_TYPE_AMR;
        break;

	 case MED_TYPE_AAC:
		   audio_fromat_type=MCI_TYPE_AAC;
        break;
		
        case MED_TYPE_WAV:
		   audio_fromat_type=MCI_TYPE_WAV;
        break;
        case MED_TYPE_MID:
		   audio_fromat_type=MCI_TYPE_MIDI;
        break;
			
        default:
		audio_fromat_type=MCI_TYPE_NONE;
    		mmi_trace(1,"aud_set_file_data_progress_req_hdlr: Unknown Multimedia Type %d",aud_context_p->current_format);
        break;
    }
if(aud_context_p->state != AUD_MEDIA_PLAY &&aud_context_p->current_file_handle>= 0)
{
    result =MCI_AudioGetDurationTime(aud_context_p->current_file_handle, audio_fromat_type,0,
										msg_p->progress,&PlayTimeInformation);
}
if(result == MED_RES_OK)	
	{
		aud_set_pair_results(MED_RES_OK, PlayTimeInformation.DurationTime);
	}
    AUD_SET_EVENT(AUD_EVT_SET_FILE_DATA_PROGRESS);
}
/******************************************************************
* FUNCTION
*    aud_media_store_info_from_temp
* DESCRIPTION
*    This function is to store play info from temp
* PARAMETERS
*    void
* RETURNS
*    void
* GLOBALS AFFECTED
*  
******************************************************************/
void aud_media_store_info_from_temp( void )
{
   aud_context_p->last_time = temp_last_time;
   aud_context_p->last_offset = temp_last_offset;
   aud_context_p->is_data_consumed = temp_is_data_consumed;
   wavCopyAdpcmState( 0, 1 );
   kal_prompt_trace( MOD_MED, "store info from temp: last_time = %d, last_offset = %d, is_data_consumed = %d", temp_last_time, temp_last_offset, temp_is_data_consumed );
}

#ifdef __RICH_AUDIO_PROFILE__
/******************************************************************
* FUNCTION
*    aud_media_amr_seek_retry
* DESCRIPTION
*    This function is to retry the seek operation of amr file.
* PARAMETERS
*    void
* RETURNS
*    None
* GLOBALS AFFECTED
*  
******************************************************************/
kal_bool aud_media_amr_seek_retry( void )
{
	kal_int32 offset;
	kal_uint8 result;
	kal_bool is_wb_amr = KAL_FALSE;

	if( amr_seek_count > 0 && amr_seek_count < 5 )
	{
		switch( aud_context_p->current_format )
		{
#ifdef AMRWB_DECODE
		case MED_TYPE_AMR_WB:
			is_wb_amr = KAL_TRUE;
#endif
		case MED_TYPE_AMR:
			if( aud_context_p->current_file_handle < 0 )
			{
				aud_context_p->current_file_handle = DRM_open_file( (const UINT8*)aud_context_p->current_file_name, FS_READ_ONLY, 0 );
				aud_context_p->audio_end = KAL_FALSE;
			}
			// skip amr header
			if( is_wb_amr )
				offset = (kal_int32)aud_context_p->offset + 9;
			else
				offset = (kal_int32)aud_context_p->offset + 6;
			// search from next byte
			offset = goto_amr_data_frame( aud_context_p->current_file_handle, (kal_uint32)offset + 1, is_wb_amr );

			kal_prompt_trace( MOD_MED, "aud_media_amr_seek_retry: seek_count=%d, offset=%d, is_wb_amr=%d, result = %d",
				amr_seek_count, aud_context_p->offset, is_wb_amr, offset );

			if( offset < 0 )
				aud_close_play_file();
			else
			{
				aud_context_p->offset += (offset + 1);
				amr_seek_count++;
				result = aud_media_play_file( KAL_FALSE );
				if( result == MED_RES_OK )
					return KAL_TRUE;
			}
			break;
		}

		amr_seek_count = 0;
	}

	return KAL_FALSE;
}
#endif

/******************************************************************
* FUNCTION
*    aud_media_store_req_hdlr
* DESCRIPTION
*    This function is to handle the audio pause request.
* PARAMETERS
*    ilm_struct *ilm_ptr
*    
* RETURNS
*    None
* GLOBALS AFFECTED
*  
*  
******************************************************************/
void aud_media_store_req_hdlr(ilm_struct *ilm_ptr)
{
   kal_uint8   res;
   l4aud_media_store_req_struct* msg_p = NULL;

   aud_context_p->src_mod = ilm_ptr->src_mod_id;

   msg_p = (l4aud_media_store_req_struct*)ilm_ptr->local_para_ptr;

   aud_context_p->src_id = msg_p->src_id;

   switch (aud_context_p->state)
   {
      case AUD_MEDIA_PLAY:
      {
         /* Stop voice memo recording process */
         aud_media_stop_and_store_info();
         /* stop volume crescendo timer */
         med_stop_timer(AUD_TIMER_CRESCENDO);
         /* stop the driver like led, backlight and vibrator */
         aud_melody_stop_driver();
         /* close the file */
         aud_close_play_file();
         /* free file buffer */
         aud_free_melody_file_buffer();
         /* set event to let caller go */
//         AUD_SET_EVENT(AUD_EVT_STOP);
         /* enter AUD_MEDIA_IDLE state */
         AUD_ENTER_STATE(AUD_MEDIA_IDLE);
         res=MED_RES_OK;
         break;
      }
      case AUD_MEDIA_PLAY_PAUSED:
      {
         /* Stop voice memo recording process */
         aud_media_stop_and_store_info();
         /* close the file */
         aud_close_play_file();
         /* free file buffer */
         aud_free_melody_file_buffer();
         /* set event to let caller go */
//         AUD_SET_EVENT(AUD_EVT_STOP);

         /* enter AUD_MEDIA_IDLE state */
         AUD_ENTER_STATE(AUD_MEDIA_IDLE);
         res=MED_RES_OK;
         break;
      }
      case AUD_MEDIA_PLAY_FINISH:
      {
         /* stop the play, in case too many data in the buffer */
         aud_media_stop_and_store_info();
         /* stop volume crescendo timer */
         med_stop_timer(AUD_TIMER_CRESCENDO);
         /* set event to let caller go */
//         AUD_SET_EVENT(AUD_EVT_STOP);
         /* enter AUD_MEDIA_IDLE state */
         AUD_ENTER_STATE(AUD_MEDIA_IDLE);
         res=MED_RES_END_OF_FILE;
         break;
      }
      case AUD_MEDIA_IDLE:
      {
     	  aud_media_store_info_from_temp();
         /* set event to let caller go */
//         AUD_SET_EVENT(AUD_EVT_STOP);
         res=MED_RES_OK;
         break;
      }
      default:
      {
         /* Stop voice memo recording process */
         Media_Stop();

         /* set event to let caller go */
//         AUD_SET_EVENT(AUD_EVT_STOP);

         /* enter AUD_MEDIA_IDLE state */
         AUD_ENTER_STATE(AUD_MEDIA_IDLE);
         res=MED_RES_FAIL;
         break;
      }

   }
   aud_set_result((kal_int32)res);
   AUD_SET_EVENT(AUD_EVT_PAUSE);
    

}
/******************************************************************
* FUNCTION
*    aud_media_restore_req_hdlr
* DESCRIPTION
*    This function is to handle the audio resume request.
* PARAMETERS
*    ilm_struct *ilm_ptr
*    
* RETURNS
*    None
* GLOBALS AFFECTED
*  
*  
******************************************************************/
void aud_media_restore_req_hdlr(ilm_struct *ilm_ptr)
{

   l4aud_media_restore_req_struct* msg_p = NULL;
   kal_uint8 result;
    mci_type_enum audio_fromat_type;

   aud_context_p->src_mod = ilm_ptr->src_mod_id;

   msg_p = (l4aud_media_restore_req_struct*)ilm_ptr->local_para_ptr;

   aud_context_p->src_id = msg_p->src_id;
mmi_trace(1,"chenhe,aud_media_restore_req_hdlr");
   if (aud_context_p->speech_on)
   {
      aud_set_result(MED_RES_BUSY);
      AUD_SET_EVENT(AUD_EVT_RESUME);
   }
   else 
   {
      /* if keytone is playing, stop it */
      aud_keytone_stop();

      /* if tone is playing, stop it */
      if (aud_context_p->tone_playing)
         aud_tone_stop();
      
#ifdef __MED_MMA_MOD__
      /* close all mma tasks */
      aud_mma_close_all();
#endif



      if (!(aud_context_p->state == AUD_MEDIA_RECORD ||
         aud_context_p->state == AUD_MEDIA_RECORD_PAUSED ||
         aud_context_p->state == AUD_VM_RECORD ||
         aud_context_p->state == AUD_VM_RECORD_PAUSED))
      {
         aud_stop_unfinished_process();

         aud_context_p->play_mode=AUD_MEDIA_PLAY_AS_SONG;
	aud_context_p->current_file_handle = MMI_FS_Open((const UINT8*)msg_p->file_name, FS_READ_ONLY);
	UCS2Strcpy((S8*)aud_context_p->current_file_name,(const UINT8*)msg_p->file_name);
mmi_trace(1,"chenhe,aud_media_restore_req_hdlr,aud_context_p->current_file_handle is %d",aud_context_p->current_file_handle);

         /* set volume and path if from LMMI */
         if (aud_context_p->src_mod==MOD_MMI)
         {
            SET_CURRENT_VOLUME_LEVEL(AUD_VOLUME_MEDIA, msg_p->volume);
            aud_context_p->melody_output_device=msg_p->output_path;
            if (aud_context_p->audio_mode==AUD_MODE_HEADSET)
               aud_melody_set_output_device(custom_cfg_hw_aud_output_path(msg_p->output_path));
         }
         else
         {
            aud_context_p->melody_output_device=AUDIO_DEVICE_SPEAKER_BOTH;
         }
	aud_context_p->source_type = AUD_FILE;

         aud_context_p->current_format = med_get_media_type(msg_p->file_name);
         aud_context_p->identifier = msg_p->identifier;
         aud_context_p->start_offset = msg_p->start_offset>=9999?9999: msg_p->start_offset;
         aud_context_p->end_offset = msg_p->end_offset;


			#if (defined(__BTCOOLSAND__)||defined(__BTRDA__)) && defined(__MED_BT_A2DP_MOD__)
                      if (aud_context_p->current_format  != MED_TYPE_AMR )  // AMR cann't use A2DP 
                      {
    				aud_bt_a2dp_open_codec((kal_int32)aud_context_p->current_format);
                        }
			#endif

    switch (aud_context_p->current_format)
    {
    
        case MED_TYPE_DAF:
		   audio_fromat_type=MCI_TYPE_DAF;
        break;

        case MED_TYPE_AMR:
		   audio_fromat_type=MCI_TYPE_AMR;
        break;

	 case MED_TYPE_AAC:
		   audio_fromat_type=MCI_TYPE_AAC;
        break;
		
        case MED_TYPE_WAV:
		   audio_fromat_type=MCI_TYPE_WAV;
        break;
			
        case MED_TYPE_MID:
		   audio_fromat_type=MCI_TYPE_MIDI;
        break;	
        default:
		audio_fromat_type=MCI_TYPE_NONE;
    		mmi_trace(1,"aud_media_restore_req_hdlr: Unknown Multimedia Type %d",aud_context_p->current_format);
        break;
    }

 	 result=MCI_AudioPlay(0,aud_context_p->current_file_handle,audio_fromat_type,aud_media_play_ind_hdlr, aud_context_p->start_offset);
mmi_trace(1,"chenhe,aud_media_restore_req_hdlr,result is %d,msg_p->volume is %d,aud_context_p->current_format is %d,aud_context_p->start_offset is %d,msg_p->volume is %d",result,msg_p->volume,aud_context_p->current_format,aud_context_p->start_offset,msg_p->volume);

	DM_SetAudioVolume(msg_p->volume);

	if(result == MED_RES_OK)	
	{
		MusicStartAudioMode();
		SET_CURRENT_VOLUME_LEVEL(AUD_VOLUME_MEDIA, msg_p->volume);//jiashuo add 20080908,save the volume level for resume operation
        #ifdef MMI_ON_HARDWARE_P
#ifdef __MMI_SOUND_EFFECT__
        MCI_AudioSetEQ(g_settings_audio_equalizer.index);
#endif
        #endif
		aud_context_p->tone_playing = TRUE;
		AUD_ENTER_STATE(AUD_MEDIA_PLAY);
		aud_set_result((kal_int32)MED_RES_OK);
		AUD_SET_EVENT(AUD_EVT_RESUME);
	}
	else
	{
		aud_context_p->tone_playing = FALSE;
		 aud_set_result((kal_int32)result);
 		aud_context_p->tone_playing = FALSE;
		MusicEndAudioMode();
		AUD_ENTER_STATE(AUD_MEDIA_IDLE);
		aud_context_p->play_mode = AUD_MEDIA_PLAY_MODE_NONE;
		 aud_close_play_file();
		 AUD_SET_EVENT(AUD_EVT_RESUME);
	}
      }  
      else
      {
         aud_set_result((kal_int32)MED_RES_BUSY);
         AUD_SET_EVENT(AUD_EVT_RESUME);
      }
   }
}



/******************************************************************
* FUNCTION
*    aud_media_read_data_ind_hdlr
* DESCRIPTION
*    This function is to handle the audio read data indication.
* PARAMETERS
*    ilm_struct *ilm_ptr
*    
* RETURNS
*    None
* GLOBALS AFFECTED
*  
*  
******************************************************************/
void aud_media_read_data_ind_hdlr(ilm_struct *ilm_ptr)
{

   aud_media_read_data_ind_struct *msg_p;


   kal_uint8   res;
   //kal_uint8   cnf_result;                         
 
   msg_p= (aud_media_read_data_ind_struct*)ilm_ptr->local_para_ptr;
	  	TRACE_EF(1,"chenhe,aud_media_read_data_ind_hdlr,aud_context_p->current_file_handle is %d",aud_context_p->current_file_handle);

  
   switch (aud_context_p->state)
   {
      case AUD_MEDIA_RECORD:
      {
	  	TRACE_EF(1,"chenhe,aud_media_read_data_ind_hdlr,msg_p->event is %d",msg_p->event);
		
		if(msg_p->event==MEDIA_ERROR)
		{

			/* close record file, add header if wav */
			res = aud_close_record_file();

			 
			aud_send_media_record_finish_ind(MED_RES_NOT_ENOUGH_SPACE);

			/* enter AUD_MEDIA_IDLE state */
			AUD_ENTER_STATE(AUD_MEDIA_IDLE);
		}
    
         break;
      }
      default:
      {
	  mmi_trace(1,"aud_media_read_data_ind_hdlr,default");
         break;
      }
   }
      

}


/******************************************************************
* FUNCTION
*    aud_media_write_data_ind_hdlr
* DESCRIPTION
*    This function is to handle the audio write data indication.
* PARAMETERS
*    ilm_struct *ilm_ptr
*    
* RETURNS
*    None
* GLOBALS AFFECTED
*  
*  
******************************************************************/
void aud_media_write_data_ind_hdlr(ilm_struct *ilm_ptr)
{

   aud_media_write_data_ind_struct *msg_p;


 
   msg_p = (aud_media_write_data_ind_struct*)ilm_ptr->local_para_ptr;
      TRACE_EF(1,"chenhe,aud_media_write_data_ind_hdlr,msg_p->event is %d,aud_context_p->state is %d",msg_p->event,aud_context_p->state);

   if( aud_context_p->last_seq_no != msg_p->seq_no )
   {
      return;
   }

   switch (aud_context_p->state)
   {
   	case AUD_MEDIA_IDLE:
		aud_close_play_file();
		break;
      case AUD_MEDIA_PLAY:
      {

         if (msg_p->event == MEDIA_END)
         {
#ifdef __RICH_AUDIO_PROFILE__
            amr_seek_count = 0;
#endif
            /* enter AUD_MEDIA_IDLE state */
            AUD_ENTER_STATE(AUD_MEDIA_IDLE);

            /* stop volume crescendo timer */
            med_stop_timer(AUD_TIMER_CRESCENDO);

            aud_context_p->last_seq_no++;

            /* close the file */
            aud_close_play_file();
            
#ifdef __MED_VID_MOD__
            if (aud_context_p->current_format==MED_TYPE_M4A)
            {
               if (aud_context_p->play_style==DEVICE_AUDIO_PLAY_ONCE)
                  aud_send_media_play_finish_ind(MED_RES_END_OF_FILE);
               else
                  aud_m4a_play_as_song(aud_context_p->current_file_name, aud_context_p->play_style, 0);
            }
            else
#endif /* __MED_VID_MOD__ */

            if (aud_context_p->play_mode==AUD_MEDIA_PLAY_AS_SONG)
            {
               if (aud_context_p->play_style==DEVICE_AUDIO_PLAY_ONCE)
                  aud_send_media_play_finish_ind(MED_RES_END_OF_FILE);
               else
                  aud_media_play_as_song(aud_context_p->current_file_name, aud_context_p->play_style, 0, KAL_FALSE);
            }   
            else
            {
               if (aud_context_p->play_style!=DEVICE_AUDIO_PLAY_ONCE)
                  aud_media_play(aud_context_p->current_file_name, aud_context_p->play_style, 0);
               else
                  aud_send_audio_play_finish_ind(MED_RES_END_OF_FILE);
            }   
         }  
         else if (msg_p->event == MEDIA_ERROR)
         {
            /* enter AUD_MEDIA_IDLE state */
            AUD_ENTER_STATE(AUD_MEDIA_IDLE);

            /* stop volume crescendo timer */
            med_stop_timer(AUD_TIMER_CRESCENDO);

#ifdef __RICH_AUDIO_PROFILE__
            if( aud_media_amr_seek_retry() )
               return;
#endif

            aud_context_p->last_seq_no++;

            /* close the file */
            aud_close_play_file();
            
             
            if (aud_context_p->play_mode==AUD_MEDIA_PLAY_AS_SONG)
               aud_send_media_play_finish_ind(MED_RES_ERROR);
            else
               aud_send_audio_play_finish_ind(MED_RES_ERROR);
         }
		 #if 0
         else if (msg_p->event == MEDIA_DATA_REQUEST)
         {
         //chenhe+
	         INT32 ReadLen,iRet;
		 UINT32 *pAmr;
              if( aud_context_p->current_format == MED_TYPE_AMR ||aud_context_p->current_format == MED_TYPE_GSM_EFR)
          	{
          		TRACE_EF(1,"chenhe,aud_media_write_data_ind_hdlr,amr read data,Amr_checkOver is %d",Amr_checkOver());
	         	if(!Amr_checkOver())
	     		{
				//med_start_timer(AMR_PLAY_TIMER, 500, aud_media_amr_getdata_callback, 0);//chenhe
				pAmr= Amr_GetDataBuffer();
	     			if(NULL != pAmr)
     				{
     				

     					iRet=MMI_FS_Read(aud_context_p->current_file_handle, pAmr, RECORDER_BUFFER_SIZE/2, &ReadLen);
					if(0 == ReadLen)
					{
						Media_DataFinished();

						/* close the file */
						aud_close_play_file();
						med_stop_timer(AMR_PLAY_TIMER);
						/* enter AUD_MEDIA_PLAY_FINISH state */
						AUD_ENTER_STATE(AUD_MEDIA_PLAY_FINISH);
						aud_context_p->audio_end=KAL_TRUE;
						aud_context_p->tone_playing = FALSE;
						//aud_media_amr_getdata_callback();//if it's over, message again to enter AUD_MEDIA_PLAY_FINISH state
					}
		          		TRACE_EF(1,"chenhe,aud_media_write_data_ind_hdlr,pAmr is %p,ReadLen is %d,iRet is %d",pAmr,ReadLen,iRet);

     				}
	     		}
			else
			{
				
//med_stop_timer(AMR_PLAY_TIMER);		

				Media_DataFinished();

				/* close the file */
				aud_close_play_file();

				/* enter AUD_MEDIA_PLAY_FINISH state */
				AUD_ENTER_STATE(AUD_MEDIA_PLAY_FINISH);
				aud_context_p->audio_end=KAL_TRUE;
				aud_context_p->tone_playing = FALSE;
				//aud_media_amr_getdata_callback();//if it's over, message again to enter AUD_MEDIA_PLAY_FINISH state

			}

          	}
           //chenhe-
         }   
#endif
         break;
      }
      case AUD_MEDIA_PLAY_FINISH:
      {
         /* if not the terminated, stop again */
         if (msg_p->event == MEDIA_END)
         {
            kal_uint8 result;

#ifdef __RICH_AUDIO_PROFILE__
            amr_seek_count = 0;
#endif
            /* enter AUD_MEDIA_IDLE state */
            AUD_ENTER_STATE(AUD_MEDIA_IDLE);

            /* stop volume crescendo timer */
            med_stop_timer(AUD_TIMER_CRESCENDO);

            aud_context_p->last_seq_no++;

#ifdef __MED_VID_MOD__
            if (aud_context_p->current_format==MED_TYPE_M4A)
            {
               if (aud_context_p->play_style==DEVICE_AUDIO_PLAY_ONCE)
                  aud_send_media_play_finish_ind(MED_RES_END_OF_FILE);
               else
                  aud_m4a_play_as_song(aud_context_p->current_file_name, aud_context_p->play_style, 0);
            }
            else
#endif /* __MED_VID_MOD__ */

            if (aud_context_p->play_mode==AUD_MEDIA_PLAY_AS_SONG)
            {
               if (aud_context_p->play_style!=DEVICE_AUDIO_PLAY_ONCE)
               {
                  result=aud_media_play_as_song(aud_context_p->current_file_name, aud_context_p->play_style, 0, KAL_FALSE);
                  if(result!=MED_RES_OK)
                     aud_send_media_play_finish_ind(result);
               }
               else
                  aud_send_media_play_finish_ind(MED_RES_END_OF_FILE);
            }   
            else
            {
               if (aud_context_p->play_style!=DEVICE_AUDIO_PLAY_ONCE)
               {
                  result=aud_media_play(aud_context_p->current_file_name, aud_context_p->play_style, 0);
                  if(result!=MED_RES_OK)
                     aud_send_audio_play_finish_ind(result);
               }
               else
                  aud_send_audio_play_finish_ind(MED_RES_END_OF_FILE);
            }   
         }
         else if (msg_p->event == MEDIA_ERROR)
         {
            /* enter AUD_MEDIA_IDLE state */
            AUD_ENTER_STATE(AUD_MEDIA_IDLE);

            /* stop volume crescendo timer */
            med_stop_timer(AUD_TIMER_CRESCENDO);

#ifdef __RICH_AUDIO_PROFILE__
            if( aud_media_amr_seek_retry() )
               return;
#endif

            aud_context_p->last_seq_no++;

            if (aud_context_p->play_mode==AUD_MEDIA_PLAY_AS_SONG)
            {
               aud_send_media_play_finish_ind(MED_RES_ERROR);
            }   
            else
            {
               aud_send_audio_play_finish_ind(MED_RES_ERROR);
            }   
         }
         break;
      }
      default:
         break;
   }


}

/******************************************************************
* FUNCTION
*    aud_media_play
* DESCRIPTION
*    This function is to handle the audio play from file. This is old interface.
* PARAMETERS
*    kal_wchar* file_name, kal_uint8 play_style, kal_uint8 first_time
*    
* RETURNS
*    kal_uint8
* GLOBALS AFFECTED
*  
*  
******************************************************************/
kal_uint8 aud_media_play(kal_wchar* file_name, kal_uint8 play_style, kal_uint8 first_time)
{
   kal_uint8 result;


   aud_context_p->source_type = AUD_FILE;
   aud_context_p->play_style = play_style;
   aud_context_p->start_offset = aud_context_p->end_offset = 0;

   /* open file for playing */
   result=aud_open_play_file((kal_wchar*)file_name);

   if (result!=MED_RES_OK) 
   {
      if (!first_time)
         aud_send_audio_play_finish_ind(result);
      return result;
   }
   else
   {
      INT32 len;
      
      /* apply volume and play style only in first loop */
      if(first_time)
      {
         /* follow ring tone volume */
         aud_set_active_ring_tone_volume();
         /* To start ascending timer */
         aud_melody_play_style_convert(play_style);
      }   

      /* call L1AUD to set buffer */
      Media_SetBuffer(aud_context_p->ring_buf, AUD_RING_BUFFER_LEN);
      Media_GetWriteBuffer(&(aud_context_p->buf_p), (kal_uint32 *)&(aud_context_p->buf_len));
      ASSERT(aud_context_p->buf_p != NULL);
      ASSERT(aud_context_p->buf_len<=AUD_RING_BUFFER_LEN);

      /* read data from file to buffer */
      /* DRM_REPLACE */
      /*FS_Read(aud_context_p->current_file_handle, 
               aud_context_p->buf_p, 
               aud_context_p->buf_len * 2,
               &len);*/
      DRM_read_file(aud_context_p->current_file_handle, 
               aud_context_p->buf_p, 
               aud_context_p->buf_len * 2,
               &len);

      /* update offset */
      aud_context_p->offset = len;

      // handle a odd bytes data case
      if( (len & 1) != 0 ) // it will be a odd number only when len * 2 < aud_context_p->buf_len
      {
         kal_uint8 padding_byte = 0;
#ifdef AMRWB_DECODE
         if( aud_context_p->current_format == MED_TYPE_AMR || aud_context_p->current_format == MED_TYPE_AMR_WB )
#else
         if( aud_context_p->current_format == MED_TYPE_AMR )
#endif
            padding_byte = 0x7C;
         kal_prompt_trace( MOD_MED, "read a odd number -- %d and add a padding byte -- %d", len, padding_byte );
         ((kal_uint8*)aud_context_p->buf_p)[len++] = padding_byte; // add a padding byte
      }

      len = len>>1;
      if (len == 0)
      {
         /* close the file */
         aud_close_play_file();

         /* stop volume crescendo timer */
         med_stop_timer(AUD_TIMER_CRESCENDO);

          
         if (!first_time)
            aud_send_audio_play_finish_ind(MED_RES_OPEN_FILE_FAIL);
         return MED_RES_OPEN_FILE_FAIL;
      }
      else if (len < aud_context_p->buf_len)
      {
         aud_context_p->buf_len = len;

         /* inform L1AUD the amount of data read from file to ring buffer */
         Media_WriteDataDone(len);

         /* start to play */
         result = Media_Play((Media_Format)aud_context_p->current_format,
                           aud_media_play_callback,
                           MEDIA_VMP_AS_RINGTONE);
         if (result!=MEDIA_SUCCESS)
         {
            /* close the file */
            aud_close_play_file();

            /* stop volume crescendo timer */
            med_stop_timer(AUD_TIMER_CRESCENDO);
            
            /* enter AUD_MEDIA_IDLE state */
            AUD_ENTER_STATE(AUD_MEDIA_IDLE);

            if (!first_time)
               aud_send_audio_play_finish_ind(MED_RES_FAIL);

            return MED_RES_FAIL;
         }
         
         Media_DataFinished();
 
         /* close the file */
         aud_close_play_file();
   
         /* enter AUD_MEDIA_PLAY_FINISH state */
         AUD_ENTER_STATE(AUD_MEDIA_PLAY_FINISH);
         aud_context_p->audio_end=KAL_TRUE;
         return MED_RES_OK;
      }
      else
      {
         /* inform L1AUD the amount of data read from file to ring buffer */
         Media_WriteDataDone(len);

         /* start to play */
         result = Media_Play((Media_Format)aud_context_p->current_format,
                           aud_media_play_callback,
                           MEDIA_VMP_AS_RINGTONE);

         if (result!=MEDIA_SUCCESS)
         {
            /* close the file */
            aud_close_play_file();

            /* stop volume crescendo timer */
            med_stop_timer(AUD_TIMER_CRESCENDO);
            
            /* enter AUD_MEDIA_IDLE state */
            AUD_ENTER_STATE(AUD_MEDIA_IDLE);

            if (!first_time)
               aud_send_audio_play_finish_ind(MED_RES_FAIL);
               
            return MED_RES_FAIL;
         }
                                    
         /* enter AUD_MEDIA_PLAY state */
         AUD_ENTER_STATE(AUD_MEDIA_PLAY);

         return MED_RES_OK;
      }
   }
}
#endif


