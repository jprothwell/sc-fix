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
 *
 * Filename:
 * ---------
 * media.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Media file recording/playback
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
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
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
#include "media.h" 
//#include "afe.h" /* wufasong deleted 2007.06.17 */
//#include "media_others.h"
#include "med_status.h"
#include "mci.h"
#include "mdi_audio.h"
#include "mmi_trace.h"

#if defined( SW_SYN_8K ) || defined( SW_SYN_16K )
#include "midisyn.h"
#elif defined( DSP_WT_SYN )
#include "dspsyn.h"
#endif

#if IS_EV_BOARD
//#include "alerter_sw.h"
#endif

#if VERIFY_DATA_TO_DSP
extern void VERIFY_DATA_TO_DSP_START( void );
extern void VERIFY_DATA_TO_DSP_RESUME( void );
extern void VERIFY_DATA_TO_DSP_STOP( void );
#endif

static struct {
   mediaControl   ctrl;
   kal_bool           waiting;
   uint16         aud_id;
   kal_bool           paused;
   void           (*media_handler)( Media_Event event );
} media;


/* Functions for Ring Buffer Access */

/* Reset the read/write pointers and odd bye flags of ring buffer */
/* This function should be called in Media_SetBuffer to reset the media buffer */
// Rey: Checked
void rb_ClearRB( void )
{
   media.ctrl.read = 0;
   media.ctrl.write = 0;
   media.ctrl.ob_flag_r = FALSE;
   media.ctrl.ob_flag_w = FALSE;
}

/* Return the count of data (in byte) in ring buffer */
/* Note that the data unit of return value is BYTE!!! */
// Rey: Checked
int32 rb_GetByteCount( void )
{

   return 0;
}

/* Return the remaining space (in byte) of ring buffer */
/* Note that the data unit of return value is BYTE!!! */
// Rey: Checked
int32 rb_GetFreeSpace( void )
{

   return 0;
}

/* Get a byte from ring buffer */
// Rey: Checked
kal_bool rb_GetByte( uint8* data_8 )
{

   return KAL_TRUE;
}

/* Get a word from ring buffer */
// Rey: Checked
kal_bool rb_GetWord( uint16* data_16 )
{

   return KAL_TRUE;
}

/* Get a double word from ring buffer */
// Rey: Checked
kal_bool rb_GetDWord( uint32* data_32 )
{

   return KAL_TRUE;
}

/* Peek the value of the incoming byte in ring buffer, without moving forward the read pointer */
// Rey: Checked
kal_bool rb_PeekByte( uint8* data_8 )
{

   return KAL_TRUE;
}

/* Peek the value of the incoming word in ring buffer, without moving forward the read pointer */
// Rey: Checked
kal_bool rb_PeekWord( uint16* data_16 )
{

   return KAL_TRUE;
}

/* Peek the value of the incoming double word in ring buffer, without moving forward the read pointer */
// Rey: Checked
kal_bool rb_PeekDWord( uint32* data_32 )
{

   return KAL_TRUE;
}

/* Skip N byte in ring buffer, return the number of bytes skipped */
// Rey: Checked
uint32 rb_SkipNByte( uint32 skip_count )
{

   return 0;
}

/* Skip N Words in ring buffer, return the number of words skipped */
// Rey: Checked
uint32 rb_SkipNWord( uint32 skip_count )
{

   return 0;
}

/* Get len bytes from ring buffer to buffer */
/* The return value is the actual number of bytes moved */
int rb_GetNByte( uint8 *buffer, int len )
{

   return 0;
}

/* Put bytes from buffer into ring buffer */
/* The return value is the actual number of bytes moved */
int rb_PutNByte( const uint8 *buffer, int len )
{

   return 0;
}

/* Put a byte into ringer buffer */
kal_bool rb_PutByte( uint8 data_8 )
{

   return KAL_TRUE;
}

/* Put a word into ring buffer */
kal_bool rb_PutWord( uint16 data_16 )
{

   return KAL_TRUE;
}

/* Put a double word into ring buffer */
kal_bool rb_PutDWord( uint32 data_32 )
{

   return KAL_TRUE;
}

/* Return media.ctrl.read */
uint32 rb_GetReadIndex( void )
{
   return 0;
}

/* Return media.ctrl.write */
uint32 rb_GetWriteIndex( void )
{
   return 0;
}

/* Return media.ctrl.rb_size */
uint32 rb_GetSize( void )
{
   return 0;
}

/* Return media.ctrl.odd_byte_r | ((uint16)media.ctrl.odd_byte_w)<<8 */
uint16 rb_GetOddByte( void )
{
   return 0;
}

uint16 rb_GetOBFlag( void )
{
   uint16 ret=0;

   return ret;
}

/* ------------------------------------------------------------------------------ */
	//deflect by licheng for code clean
/*
static void mediaDataRequest( void )
{

}

static void mediaDataNotification( void )
{

}
*/
void mediaHisr( uint16 flag )
{

}

/* ------------------------------------------------------------------------------ */
/*  Media File Playback/Recording Interface                                       */
/* ------------------------------------------------------------------------------ */
void Media_SetOutputDevice( uint8 device )
{

}

void  Media_SetOutputVolume( uint8 volume1, uint8 volume2 )
{

}

void Media_SetBuffer( uint16 *buffer, uint32 buf_len )
{
   media.ctrl.rb_base = buffer;
   media.ctrl.rb_size = buf_len*2;
   media.ctrl.read = 0;
   media.ctrl.write = 0;
   rb_ClearRB();
   media.ctrl.eof     = FALSE;
   media.ctrl.file_len = 0;
  MCI_SetBuffer((UINT32*)buffer, buf_len*2);
}

void  Media_GetWriteBuffer( kal_uint16 **buffer, kal_uint32 *buf_len )
{

}

void Media_GetWriteBuffer_ext( kal_uint8 **buffer, uint32 *buf_len )
{
   int32 count;
	INT32 BufferRemainingBytes =MCI_GetRemain();


	   mmi_trace(g_sw_DEBUG, "Entry Media_GetWriteBuffer_ext ");
	   media.ctrl.read =(INT32) MCI_GetBufPosition()-(INT32)(media.ctrl.rb_base);
	   
	if((media.ctrl.read+BufferRemainingBytes) >= media.ctrl.rb_size)
	{
	   media.ctrl.write  =  media.ctrl.read+BufferRemainingBytes-media.ctrl.rb_size;
	}
	else 
	{
		media.ctrl.write  =  media.ctrl.read+BufferRemainingBytes;
	}
	mmi_trace(g_sw_DEBUG, " media.ctrl.read=%d write=%d ",  media.ctrl.read,  media.ctrl.write);

   if( media.ctrl.read > media.ctrl.write )
      count = (int32)media.ctrl.read - (int32)media.ctrl.write ;
   else if( media.ctrl.read == 0 )
      count = (int32)media.ctrl.rb_size - (int32)media.ctrl.write ;
   else
      count = (int32)media.ctrl.rb_size - (int32)media.ctrl.write;

   *buffer = (kal_uint8 *)(media.ctrl.rb_base) + media.ctrl.write;
   *buf_len = (uint32)count;

}

void Media_WriteDataDone( uint32 len )
{
	mmi_trace(g_sw_DEBUG, "Media_WriteDataDone = %d", len);
	MCI_AddedData(len);
}



void Media_DataFinished( void )
{
	MCI_DataFinished();
}


void Media_GetReadBuffer( uint16 **buffer, uint32 *buf_len )
{

}

void Media_ReadDataDone( uint32 len )
{

}

int32 mediaGetFreeSpace( void )
{
	return 0;
}

int32 mediaGetDataCount( void )
{
	return 0;
}

kal_bool mediaIsDataConsumed( void )
{
   return KAL_TRUE;
}

mediaControl *mediaGetControl( void )
{
   return &media.ctrl;
}

void g_MediaUserHandler (APBS_STREAM_STATUS_T status)
{
	APBS_STREAM_STATUS_T stream_status = (APBS_STREAM_STATUS_T)status;
	mmi_trace(g_sw_DEBUG, " g_MediaUserHandler:;stream_status=%d ",  stream_status);
     switch(stream_status)
     	{
     	 case     STREAM_STATUS_REQUEST_DATA:
	  	media.media_handler(MEDIA_DATA_REQUEST);
		break;
	case     STREAM_STATUS_NO_MORE_DATA:
	  	media.media_handler(MEDIA_ERROR);
		break;
       case  STREAM_STATUS_END:
	  	media.media_handler(MEDIA_END);
	   	break;
      case STREAM_STATUS_ERR:
	  	media.media_handler(MEDIA_ERROR);
	  	break;

     	}
		
}

/* ------------------------------------------------------------------------------ */
Media_Status Media_Play( Media_Format format, void (*media_handler)( Media_Event event ), void *param )
{
	UINT32 mciFormat = 0;
	
	 if( media_handler == 0 )
    		  return MEDIA_NO_HANDLER;

		
		switch(format)
		{
			case    MDI_FORMAT_AMR:
			case   MDI_FORMAT_AMR_WB:
				mciFormat = MCI_PLAY_MODE_AMR_RING;
				break;
			case   MDI_FORMAT_DAF:
				mciFormat = MCI_PLAY_MODE_MP3;
				break;
			case MDI_FORMAT_AAC:
				mciFormat = MCI_PLAY_MODE_AAC;
				break;
			case MDI_FORMAT_PCM_8K:
			case MDI_FORMAT_PCM_16K  :
				mciFormat = MCI_PLAY_MODE_PCM;
				break;
			case MDI_FORMAT_WAV:
				mciFormat = MCI_PLAY_MODE_WAV;
				break;
			case MDI_FORMAT_MID:
				mciFormat =MCI_PLAY_MODE_MID;				
				break;
			default:
				mmi_trace(g_sw_DEBUG,"chenhe,aud_melody_play_by_string,wrong format is %d",format);
				return -1;
				break;
		}

	media.media_handler = media_handler;
	media.ctrl.format = format;

	MCI_AUD_StreamPlay((INT32 *)(media.ctrl.rb_base),media.ctrl.rb_size, mciFormat,g_MediaUserHandler);


   return MEDIA_SUCCESS;
}

Media_Status Media_Record( Media_Format format, void (*media_handler)( Media_Event event ), void *param )
{

   return MEDIA_SUCCESS;
}

Media_Status Media_Pause( void )
{
	MCI_AudioPlayPause();	
   return MEDIA_SUCCESS;
}

Media_Status Media_Resume( void )
{
	MCI_AudioPlayResume();
  return MEDIA_SUCCESS;
}

extern void MusicEndAudioMode(); //Added by Jinzh:20070616

void Media_Stop( void )
{
	MCI_AudioStopBuffer();
	if(media.media_handler!= NULL)
	{
	  	media.media_handler(MEDIA_TERMINATED);
		media.media_handler = NULL;

		#ifdef __FF_AUDIO_SET__
		MusicEndAudioMode();   //Added by jinzh:20070730
		#endif
	}

}

Media_Status Media_Control( Media_Ctrl ctrl_no, int32 ctrl_val )
{

   return MEDIA_SUCCESS;
}

void Media_SetFilter( Media_Filter_Type type, uint16 len, const int16 *filter )
{

}

Media_Status Media_SetLevel( kal_uint8 level )
{

   return MEDIA_SUCCESS;
}

kal_uint8 Media_GetLevel( void )
{
   return media.ctrl.level;
}

/* ------------------------------------------------------------------------------ */
void mediaHandler( void *data )     /* This function works in L1Audio Task */
{
   Media_Event event = (Media_Event)(int32)data;


   media.media_handler( event );
}
/* ------------------------------------------------------------------------------ */

void mediaInit( uint16 aud_id )
{
}

/* If the event is MEDIA_END or MEDIA_ERROR, make sure DSP has been stopped
   before calling this function. Refer to the function: mediaHandler */
void mediaSetEvent( Media_Event event )
{

}

kal_bool mediaKeytoneForbidden( void )
{

   return KAL_FALSE;
}

void Media_SetRBThreshold( uint16 threshold )
{

}

/* ------------------------------------------------------------------------------ */


