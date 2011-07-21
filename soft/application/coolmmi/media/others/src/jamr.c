#include "media.h"

#if 1
//#if  IS_CHIP_MT6218_AND_LATTER_VERSION || IS_EDGE_CHIP_MT6229_AND_LATTER_VERSION

#include "amr_table.h"
#include "am.h"
#include "mmi_trace.h"
#include "oslmemory.h"

#define AMR_STATE_READY       0
#define AMR_STATE_NB_PLAY     1
#define AMR_STATE_NB_PAUSED   2
#define AMR_STATE_WB_PLAY     4
#define AMR_STATE_WB_PAUSED   8

#define DP_D2C_SD_DONE        0xDD01      /* magic number for l1audio HISR */

uint32 SaveAndSetIRQMask( void );
void   RestoreIRQMask( uint32 );

typedef struct media_handle_internal {
   Media_Handle      mh;      /* this should be the first line in this structure */
   Media_AMR_Param   param;
   const uint8       *orgpos;
   const uint8       *curpos;
   const uint8       *endpos;
   uint16            aud_id;
   int16             repeat_count;
   uint16            orderingBuffer[30];
   uint16            Tx_data_buffer[30];
   uint8             bCodecMode; /* Codec Mode for DSP Sherif Setting */
   uint8             play_style;
} Media_Handle_Internal;

//deflect by licheng for code clean
#if 0
static void jamrWriteSpeechMode( Media_Handle_Internal *handle, uint8 bCodecMode, uint8 bTxType )
{
}

static void jamrWriteSpeechData( Media_Handle_Internal *handle, uint16 *data, uint32 len )
{
}

static void jamrOrderConvert( Media_Handle_Internal *handle, uint32 bTxType, uint32 bFrameType,
                              uint16 *dataIn, uint16 *dataOut, uint32 bits)
{

}

/* FUNCTION:  jamrPass20ms
      return value:
         0 -> ok
         1 -> end
         2 -> error
*/
static int32 jamrPass20ms( const uint8 **curpos, const uint8 *endpos )
{
   return 0;
}

#if 1
//#if IS_CHIP_MT6219_AND_LATTER_VERSION || IS_EDGE_CHIP_MT6229_AND_LATTER_VERSION
/* FUNCTION:  jamrwbPass20ms
      return value:
         0 -> ok
         1 -> end
         2 -> error
*/
static int32 jamrwbPass20ms( const uint8 **curpos, const uint8 *endpos )
{
   return 0;
}
#endif /* IS_CHIP_MT6219_AND_LATTER_VERSION || IS_EDGE_CHIP_MT6229_AND_LATTER_VERSION */

static void jamrHisr( void *data );

static void jamrOpenDevice( Media_Handle_Internal *handle, uint16 wCodecMode )
{
}

static void jamrCloseDevice( Media_Handle_Internal *handle )
{
}

static void jamrTask( void *data )
{
}

static void jamrException( Media_Handle_Internal *handle, Media_Event event ) 
{
}

static void jamrHisr( void *data )
{
}
#endif
Media_Handle *JAmr_Open( void (*handler)( Media_Handle *handle, Media_Event event ), Media_AMR_Param **param )
{
   Media_Handle_Internal *handle;

   handle = (Media_Handle_Internal*)OslMalloc( sizeof(Media_Handle_Internal) );
   handle->mh.handler      = handler;
   handle->mh.start_time   = 0;
   handle->mh.stop_time    = 0x7FFFFFFF;
   handle->mh.volume_level = 100;
   handle->mh.state        = AMR_STATE_READY;
   handle->aud_id          = 0;//L1Audio_GetAudioID();
   handle->param.amrfile   = 0;
   *param = &handle->param;
 //  L1Audio_SetEventHandler( handle->aud_id, jamrTask );
   return (Media_Handle*)handle;


   return (Media_Handle*)1;
}

Media_Status JAmr_Close( Media_Handle *handle )
{

   return MEDIA_SUCCESS;
}
BOOL HA_audio_StartStream(UINT32*	buffer, UINT32	len, DM_PlayRingType codec, BOOL	loop_mode);
BOOL HA_audio_StopStream(VOID);

Media_Status JAmr_Play( Media_Handle *hdl )
{

   Media_Handle_Internal *handle = (Media_Handle_Internal *)hdl;
   const unsigned int *amrfile = (unsigned int *)(handle->param.amrfile);
   mmi_trace(1, "<<<<<<<<<<<%x, %x, %x, %x, %x, %x, %x, %x, handle->param.filesize =%d>>>>>>>>>>>", *amrfile, *(amrfile + 1),
   	*(amrfile + 2),*(amrfile + 3),*(amrfile + 4),*(amrfile + 5),*(amrfile + 6),*(amrfile + 7),handle->param.filesize);
   HA_audio_StartStream((UINT32 *)amrfile, handle->param.filesize >>2, DM_DICTAPHONE_MR122, 1);
   return MEDIA_SUCCESS;
}

Media_Status JAmr_Stop( Media_Handle *hdl )
{
	//deflect by licheng for code clean
   //Media_Handle_Internal *handle = (Media_Handle_Internal *)hdl;
   //uint32 savedMask;

   if( hdl->state & (AMR_STATE_NB_PAUSED|AMR_STATE_WB_PAUSED|AMR_STATE_READY) ) 
   {
      hdl->state = AMR_STATE_READY;
      return MEDIA_SUCCESS;
   }
   //savedMask = SaveAndSetIRQMask();
   if( hdl->state & (AMR_STATE_NB_PLAY|AMR_STATE_WB_PLAY) ) 
   	{
      		hdl->state = AMR_STATE_READY;    /* Changing state before closing device is to block HISR entry */
		HA_audio_StopStream();
		//RestoreIRQMask( savedMask );
      		//jamrCloseDevice( handle, KAL_TRUE );
   	}
   else 
   	HA_audio_StopStream();
      //RestoreIRQMask( savedMask );
   
   return MEDIA_SUCCESS;
}

Media_Status JAmr_Pause( Media_Handle *hdl )
{
	//deflect by licheng for code clean
   //Media_Handle_Internal *handle = (Media_Handle_Internal *)hdl;
   //uint32 savedMask;

  // savedMask = SaveAndSetIRQMask();
   if( !(hdl->state & (AMR_STATE_NB_PLAY|AMR_STATE_WB_PLAY)) ) {
   //   RestoreIRQMask( savedMask );
      return MEDIA_FAIL;
   }
   if( hdl->state == AMR_STATE_NB_PLAY )
      hdl->state = AMR_STATE_NB_PAUSED;
   else
      hdl->state = AMR_STATE_WB_PAUSED;
 //  RestoreIRQMask( savedMask );
 //  jamrCloseDevice( handle, KAL_TRUE ); 
 DM_Audio_PauseStream();
   return MEDIA_SUCCESS;
}

Media_Status JAmr_Resume( Media_Handle *hdl )
{
	//deflect by licheng for code clean
	//uint16 wCodecMode;
   //Media_Handle_Internal *handle = (Media_Handle_Internal *)hdl;

   if( (hdl->state & (AMR_STATE_NB_PAUSED|AMR_STATE_WB_PAUSED)) == 0 )
      return MEDIA_FAIL;

//#if IS_CHIP_MT6219_AND_LATTER_VERSION || IS_EDGE_CHIP_MT6229_AND_LATTER_VERSION
//   if( hdl->state & AMR_STATE_WB_PAUSED ) {
//      handle->bCodecMode = 0x20;
//      wCodecMode = (0x20<<2);
//      hdl->state = AMR_STATE_WB_PLAY;
//   } else
//#endif
//   {
//      handle->bCodecMode = 0x03;
//      wCodecMode = (0x03<<2) | (0x03<<6);
      hdl->state = AMR_STATE_NB_PLAY;
//   }

   /* write 'No-Data' speech frame to dsp */
//   jamrWriteSpeechMode( handle, handle->bCodecMode, 0 );
   /* Set Digital Gain for AMR/AMR-WB */
//   *DP_VOL_OUT_PCM = DG_AMR * handle->mh.volume_level / 100;
 DM_Audio_ResumeStream();
  // jamrOpenDevice( handle, wCodecMode );

   return MEDIA_SUCCESS;
}

Media_Status JAmr_SetStartTime( Media_Handle *hdl, kal_int32 msStartTime )
{
   hdl->current_time = hdl->start_time = msStartTime;
   return MEDIA_SUCCESS;
}

Media_Status JAmr_SetStopTime( Media_Handle *hdl, kal_int32 msStopTime )
{
   return MEDIA_SUCCESS;
}

kal_int32 JAmr_GetCurrentTime( Media_Handle *hdl )
{
   return hdl->current_time;
}

Media_Status JAmr_SetLevel( Media_Handle *hdl, kal_uint8 level )
{
   return MEDIA_SUCCESS;
}

kal_uint8 JAmr_GetLevel( Media_Handle *hdl )
{
   return hdl->volume_level;
}

Media_Status JAmr_GetDuration( const uint8 *amrfile, int32 amrsize, int32 *duration )
{
   return MEDIA_SUCCESS;
}

#endif   // IS_CHIP_MT6218_AND_LATTER_VERSION || IS_EDGE_CHIP_MT6229_AND_LATTER_VERSION


