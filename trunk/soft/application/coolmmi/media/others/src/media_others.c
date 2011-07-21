#include "mmi_data_types.h"
#include "cswtype.h"

#include "lcddr.h"
#include "kal_non_specific_general_types.h"
#include "app_ltlcom.h"
#include "kal_release.h"
#include "kal_common_defs.h"
#include "lcd_if.h"
#include "stack_timer.h"
#include "stack_config.h"

/* wufasong added 2007.06.14 */
#include "stacklib.h"
#include "lcd_dll.h"

//add by WeiD
#include "med_global.h"  
#include "custom_nvram_editor_data_item.h"
#include "med_struct.h" 
#include "aud_defs.h"
#include "l1audio.h"
#include "med_api.h"
#include "event_shed.h"
#include "ext_camera.h"

#include "med_context.h"

#include "event_shed.h"

#include "mmi_trace.h"
#include "mdi_audio.h"
#include "queuegprot.h"
#include "frameworkstruct.h"
#include "audioplayertype.h"
#include "media_others.h"
#include "datamgr.h"
//#include "jpeg.h"
#include "gdi_mutex.h"
#include "gdi_lcd_config.h"
//#include "gdi_primitive.h"
#include "gdi_const.h"
#include "gdi_datatype.h"
#include "resource_audio.h"     //added by jinzh:20070623
#include "csw.h"
#include "ucs2prot.h"
const unsigned char  L1SP_MICROPHONE1  = L1SP_LNA_0;
const unsigned char  L1SP_MICROPHONE2  = L1SP_LNA_1;

const unsigned char  L1SP_SPEAKER1     = L1SP_BUFFER_0;
const unsigned char  L1SP_SPEAKER2     = L1SP_BUFFER_1;
const unsigned char  L1SP_LOUD_SPEAKER = L1SP_BUFFER_1|L1SP_BUFFER_EXT;

extern gdi_mutex_struct gdi_mutex;
//U32	gdi_image_decoder_frame_buf[32]; /* don't using this var. */
#ifdef __MMI_CAMERA__
ext_cam_module_func cam_func = 
{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};
#endif
uint8 L1SM_GetHandle(void)
{
    return 0;
}

void L1SM_SleepEnable(uint8 handle)
{
}

void L1SM_SleepDisable(uint8 handle)
{
}

void cam_module_func_config(void)
{
}

void* construct_int_peer_buff(kal_uint16 pdu_len, kal_uint16 header_len, 
                                     kal_uint16 tail_len, kal_char* file_name_ptr, 
                                     kal_uint32 line)
{
	return NULL;
}

void* construct_int_local_para(kal_uint16 local_para_size,
                                      kal_char* file_ptr, kal_uint32 line)
{
	return NULL;
}

#if 0
void free_int_peer_buff(peer_buff_struct *pdu_ptr, kal_char* file,
                               kal_uint32 line)
{
}
#endif
void free_int_local_para(local_para_struct *local_para_ptr,
                                kal_char* file, kal_uint32 line)
{

}
#if 0
void* get_pdu_ptr(peer_buff_struct *peer_buff_ptr, kal_uint16 *length_ptr)
{
	return NULL;
}
#endif

void kal_get_time( kal_uint32* ticks_ptr)
{
  /*wangrui modify begin 20081121 for fix 10429  */

   *ticks_ptr =  csw_TMGetTick();
  /*wangrui modify end  */

}
void Aud_get_time( kal_uint32*tmpTime )
{
	*tmpTime=TM_GetTime();
}
/* This routine creates a Event Group. This should be called during the Creation phase */
kal_eventgrpid kal_create_event_group(kal_char* eventgrp_name)
{
	return NULL;
}

/*************************************************************************
 * Kal Initialize
 *    These routines are to be called during the initialization phase.
 * Call the following three routines in the very beginning.
 *************************************************************************/

/* Initializes the kal layer. It should be called during initialization phase */
void kal_initialize(void)
{
}

/*
 * Initializes kal for the Memory requirement. It should be called
 * during initialization phase
 */
void kal_initialize_mem(void* start_address_ptr)
{
}

/* Query system stage */
kal_bool kal_query_systemInit(void)
{
	return KAL_FALSE;
}

 

 /*************************************************************************
 * ITC Management
 *    This routine creates a queue.
 *    This should be called during the Creation phase
 *************************************************************************/

/* This routine creates a message queue with variable length messages
 * or fixed length message depending on the value passed at q_type. if
 * q_type == KAL_FIXED_LEN_MSG_Q, msg_size will contain the
 * size of message passed to the queue. if q_type ==
 * KAL_VARIABLE_LEN_MSG_Q, msg_size will contain the maximum size
 * of messge that can be posted in the queue. */
kal_msgqid kal_create_msg_q(kal_char* queue_name_ptr, kal_uint16 msg_size,
									 kal_uint16 max_msgs)
 {
 	return 0;
 }

/* This routine deletes the specified queue.
	This should be called during termination phase */
void kal_delete_msg_q( kal_msgqid queue_id)
{
}

/* To Enqueue message : Execution Phase */
kal_status kal_enque_msg(kal_msgqid ext_msg_q_id, void* buffer_ptr,
								 kal_uint16 msg_size, kal_wait_mode wait_mode, kal_bool message_to_head)
{
	return 0;
}

/* To De-queue message : Execution Phase */
kal_status kal_deque_msg(kal_msgqid ext_msg_q_id, void* buffer_ptr, kal_uint16 *msg_size,
								 kal_wait_mode wait_mode)
{
	return 0;
}

/* To get queue's information */
kal_bool kal_get_queue_info(kal_msgqid task_ext_qid, kal_msgq_info *msgq_info)
{
	return 0;
}

/* This routine creates a Mutex. This should be called during the Creation phase */
kal_mutexid kal_create_mutex( kal_char* mutex_name_ptr)
{
	return NULL;
}

/* This routine deletes the specified Mutex. This should be called during the termination phase */
void kal_delete_mutex( kal_mutexid mutex_id)
{
}

/* This routine captures the created mutex. This should be called during the Execution phase */
void kal_take_mutex( kal_mutexid    mutex_id )
{
}

/* This routine releases the mutex.This should be called during the Execution phase */
void kal_give_mutex( kal_mutexid    mutex_id )
{
}

/* This routine creates a Semaphore. This should be called during the Creation phase */
kal_semid kal_create_sem( kal_char*    sem_name_ptr, kal_uint32 initial_count )
{
	return NULL;
}

/* This routine deletes the specified Sem. This should be called during the termination phase */
void kal_delete_sem( kal_semid sem_id)
{
}

/* This routine captures the created sem. This should be called during the Execution phase */
kal_status kal_take_sem(kal_semid sem_id, kal_wait_mode wait_mode)
{	
	return 0;
}

/* This routine releases the sem. This should be called during the Execution phase */
void kal_give_sem( kal_semid    sem_id )
{
}

/* This routine returns the current counting value of the given semaphore */
kal_uint32 kal_query_sem_value (kal_semid ext_sem_id_ptr)
{
	return 0;
}
 

/* This routine deletes a Event Group. This should be called during the Termination phase */
void kal_delete_eventgrp(kal_eventgrpid eg_id)
{
}

/* This routine sets Events to a Event Group. This should be called during the Execution phase */
kal_status kal_set_eg_events(kal_eventgrpid eg_id, kal_uint32 events,
									  kal_uint8 operation)
{
	return 0;
}

/* This routine retrieves Events to a Event Group. This should be called during the Execution phase */
kal_status kal_retrieve_eg_events(kal_eventgrpid eg_id, kal_uint32 requested_events,
											 kal_uint8 operation, kal_uint32 *retrieved_events,
											 kal_uint32 suspend)
{
	return 0;
}

void lcd_fb_update_to_memory(lcd_frame_update_to_mem_struct *lcd_para)
{
}



void* get_local_para_ptr( local_para_struct *local_para_ptr,
                                 kal_uint16 *local_para_len_ptr)
{
	return NULL;
}

kal_status receive_msg_ext_q( kal_msgqid task_ext_qid,
                                     ilm_struct *ilm_ptr)
{
	return 0;
}

kal_bool msg_get_ext_queue_info(kal_msgqid task_ext_qid,
                                       kal_uint32 *messages)
{
	return 0;
}

kal_bool msg_get_ext_queue_length(kal_msgqid task_ext_qid, kal_uint32 *length)
{
	return 0;
}

module_type stack_int_get_active_module_id( void )
{
	return 0;
}

void stack_int_set_active_module_id( kal_uint32 task_index, module_type module_id )
{
}

void free_ilm(ilm_struct* ilm_ptr)
{
}

int kal_wstrlen(const WCHAR *wstr)
{
	return UCS2Strlen((const S8*)wstr);
}

/*
WCHAR *kal_wstrcpy(WCHAR *to, const WCHAR *from)
{
	return NULL;
}
*/

WCHAR *kal_wstrncpy(WCHAR *to, const WCHAR *from, int n)
{
	return (WCHAR *)UCS2Strncpy((S8*)to,(const S8*)from,n);
}
int kal_wstrcmp(const WCHAR *s1, const WCHAR *s2)
{
	return UCS2Strcmp((const S8*)s1,(const S8*)s2);
}
int kal_wstrncmp(const WCHAR *s1, const WCHAR *s2, int n)
{
	return UCS2Strncmp((const S8*)s1,(const S8*)s2,n);
}

/*************************************************************************
 * Exported Function Prototypes
 *************************************************************************/
void stack_init_timer(stack_timer_struct *stack_timer, 
                      kal_char *timer_name, 
                      module_type module_id)
{
    
}

void stack_start_timer(stack_timer_struct *stack_timer, 
                       kal_uint16 timer_indx, 
                       kal_uint32 init_time)
{
    
}

stack_timer_status_type stack_stop_timer(stack_timer_struct *stack_timer)
{
	return 0;
}

stack_timer_status_type stack_timer_status(stack_timer_struct *stack_timer, kal_uint32 *time_remaining)
{
	return 0;
}

kal_bool stack_is_time_out_valid(stack_timer_struct *stack_timer)
{
	return 0;
}

void stack_process_time_out(stack_timer_struct *stack_timer)
{
    
}

kal_uint8 stack_time_out_messages_in_queue(stack_timer_struct *stack_timer)
{
	return 0;
}

void stack_deinit_timer(stack_timer_struct *stack_timer)
{
    
}

void kal_evshed_create_pool( void )
{
    
}

void* kal_evshed_get_mem( kal_uint32 size )
{
	return NULL;
}

void kal_evshed_free_mem( void* buff_ptr )
{
    
}

/*
 * Important:
 *  Current implementation max_delay_ticks only support 0, and non-0.
 */
event_scheduler *new_evshed(void *timer_id,
                                   void (*start_timer)(void *, unsigned int),
                                   void (*stop_timer)(void *),
                                   kal_uint32 fuzz,
                                   malloc_fp_t alloc_fn_p,
                                   free_fp_t free_fn_p,
                                   kal_uint16 max_delay_ticks)
{
	return NULL;
}

/*
 * Important
 *  System will allocate memory for event id, and return to caller.
 *  If caller need to save event id, please be careful to reset when
 *  cancel the event or the event expired.
 */
eventid evshed_set_event(event_scheduler *es,
                                kal_timer_func_ptr event_hf,
                                void *event_hf_param,
                                kal_uint32 elapse_time)
{
	return NULL;
}

/*
 * Important
 *  System would reset *eid to NULL before return, however, caller
 *  should pay attention to saved event id.
 */
kal_int32 evshed_cancel_event(event_scheduler *es, eventid *eid)
{
    return 0;
}

kal_uint32 evshed_get_rem_time(event_scheduler *es, eventid eid)
{
    return 0;
}

void evshed_suspend_all_events(event_scheduler *es)
{
    
}

void evshed_resume_all_events(event_scheduler *es)
{
    
}

void evshed_delete_all_events(event_scheduler *es)
{
    
}

void delete_evshed(event_scheduler *es)
{
    
}

/*
 * Important
 *  In evshed_timer_handler(), system would execute event regisited timeout callback function.
 *  Caller should reset saved event id in regisited timeout callback function,
 *  or cause potential bug to cancel wrong timer event.
 */
void evshed_timer_handler(event_scheduler *es)
{
    
}

kal_bool evshed_events_pending(event_scheduler *es)
{
    return 0;
}
/*
void cam_module_bypass(void)
{
}

void cam_module_power_on(void)
{
}

void cam_module_cmd_mapping(void)
{
}

kal_uint8 cam_module_reset_status(void)
{
	return 0;
}

kal_uint8 cam_module_preview_ready_check(void)
{
	return 0;
}

kal_uint32 cam_module_capture_ready_check(void)
{
	return 0;
}

void cam_module_power_off(void)
{
}

void cam_module_preview(ext_camera_para_struct *ext_cam_para)
{
	mmi_trace(TRUE, "Excute func : cam_module_preview()");
}

void exit_cam_module_preview(void)
{
}

void cam_module_capture(ext_camera_para_struct *ext_cam_para)
{
}

kal_uint32 cam_module_jpeg_encode(ext_camera_para_struct *ext_cam_para, kal_bool back_to_preview)
{
	return 0;
}

void cam_module_set_para(kal_uint8 cmd, kal_uint8 para)
{
}
void cam_module_write_para(kal_uint16 cmd, kal_uint16 para)
{
}

kal_uint16 cam_module_read_para(kal_uint16 cmd)
{
	return 0;
}

kal_uint8 cam_module_frame_rate(void)
{
	return 0;
}


extern void cam_module_bypass(void);
extern void cam_module_power_on(void);
extern void cam_module_cmd_mapping(void);
extern kal_uint8 cam_module_reset_status(void);
extern kal_uint8 cam_module_preview_ready_check(void);
extern kal_uint32 cam_module_capture_ready_check(void);
extern void cam_module_power_off(void);
extern void cam_module_preview(ext_camera_para_struct *ext_cam_para);
extern void exit_cam_module_preview(void);
extern void cam_module_capture(ext_camera_para_struct *ext_cam_para);
extern kal_uint32 cam_module_jpeg_encode(ext_camera_para_struct *ext_cam_para, kal_bool back_to_preview);
extern void cam_module_set_para(kal_uint8 cmd, kal_uint8 para);
extern void cam_module_write_para(kal_uint16 cmd, kal_uint16 para);
extern kal_uint16 cam_module_read_para(kal_uint16 cmd);
extern kal_uint8 cam_module_frame_rate(void);

ext_cam_module_func cam_func = 
{
	cam_module_power_on,
	cam_module_cmd_mapping,
	cam_module_reset_status,
	cam_module_preview_ready_check,
	cam_module_capture_ready_check,
	cam_module_power_off,
	cam_module_preview,
	exit_cam_module_preview,
	cam_module_capture,
	cam_module_jpeg_encode,
	cam_module_set_para,
	cam_module_write_para,
	cam_module_read_para,
	cam_module_frame_rate
};
*/

void kal_get_my_task_index(kal_uint32 *index)
{
}

/*
int32 mediaGetDataCount( void )
{
   return 0;
}
*/
void wavGetAdpcmState(uint32 mode)
{
}

void L1SP_SetInputSource( uint8 src )
{
   
}

/*void Media_GetReadBuffer( uint16 **buffer, uint32 *buf_len )
{
}

void Media_GetWriteBuffer( uint16 **buffer, uint32 *buf_len )
{
}

void Media_ReadDataDone( uint32 len )
{

}

void Media_WriteDataDone( uint32 len )
{
}

void Media_SetBuffer( uint16 *buffer, uint32 buf_len )
{
}

Media_Status Media_Record( Media_Format format, void (*media_handler)( Media_Event event ), void *param )
{
    return 1;
}

void Media_DataFinished( void )
{
}

Media_Status Media_Play( Media_Format format, void (*media_handler)( Media_Event event ), void *param )
{
    return 1;
}

Media_Status Media_Pause( void )
{
    
}

Media_Status Media_Resume( void )
{
    return 1;
}

Media_Status Media_Control( Media_Ctrl ctrl_no, int32 ctrl_val )
{
    return 1;
}
*/
kal_uint8 custom_cfg_hw_aud_input_path(  kal_uint8 mic_id )
{
    return 0;
}

void wavSetAdpcmState(BOOL realResume, uint32 mode)
{
}

uint32   SaveAndSetIRQMask( void )
{
    return 0;
}

void   RestoreIRQMask( uint32 x)
{
}
void custom_get_fixed_imelody_range(kal_uint8 *min_id, kal_uint8 *max_id)
{
	*min_id = MIN_RING_TONE_ID;   //added by jinzh:20070623
	*max_id = MAX_RING_TONE_ID;

}

void custom_get_fixed_sound_range(kal_uint8 *min_id, kal_uint8 *max_id)
{
	*min_id = MIN_SND_ID;//added by jinzh:20070623
   	*max_id = MAX_SND_ID;
}

void custom_get_fixed_midi_range(kal_uint8 *min_id, kal_uint8 *max_id)
{
	*min_id = MIN_MIDI_ID;  //added by jinzh:20070623
   	*max_id = MAX_MIDI_ID;
}

void custom_get_fixed_message_sound_range(kal_uint8 *min_id, kal_uint8 *max_id)
{
	*min_id = MIN_MSG_SND_ID;//added by jinzh:20070623
   	*max_id = MAX_MSG_SND_ID;
}

void custom_get_fixed_ems_imelody_range(kal_uint8 *min_id, kal_uint8 *max_id)
{
	*min_id = MIN_EMS_IMY_ID;//added by jinzh:20070623
   	*max_id = MAX_EMS_IMY_ID;
}

void custom_get_fixed_mms_sound_range(kal_uint8 *min_id, kal_uint8 *max_id)
{
	*min_id = MIN_MMS_SND_ID;//added by jinzh:20070623
   	*max_id = MAX_MMS_SND_ID;
}
/*
void Media_SetOutputDevice( uint8 device )
{
}


void TONE_SetOutputDevice( uint8 device )
{
   
}*/

kal_uint8 custom_cfg_hw_aud_output_path(  kal_uint8 speaker_id )
{
	kal_uint8 out = 0;
	switch(speaker_id)
	{
		case 0:
			break;
		case 1:
			break;
		case 4:
		case 5:
			break;
		case 6:
			out =AUDIO_DEVICE_SPEAKER_BOTH;
			break;
		default:
			out =AUDIO_DEVICE_SPEAKER2;
			break;			
	}

	return out;
}

void wavCopyAdpcmState(kal_uint32 src_mode, kal_uint32 dst_mode)
{
}

/*
void  TONE_SetFIR( kal_bool enable )
{
}
*/
void        L1SP_Write_Audio_Coefficients( const kal_int16 in_coeff[30], const kal_int16 out_coeff[30] )
{
}

void        L1SP_SetEchoSuppression( kal_bool ec )
{
}

kal_bool    L1SP_IsMicrophoneMuted( void )
{
    return KAL_FALSE;
}

void        L1SP_MuteMicrophone( kal_bool mute )
{
}

void        L1SP_SetEchoSuppressionParameter( kal_uint16 timeConst, kal_uint16 volConst )
{
}

/*
void KT_SetOutputDevice( uint8 device )
{
}


void TONE_Play( const L1SP_Tones *tonelist )
{
}

void  TONE_Stop( void )
{
}

void  KT_Play( kal_uint16 freq1, kal_uint16 freq2, kal_uint16 duration )
{
}
void KT_Stop( void )
{
}

void KT_SetFIR( kal_bool enable )
{
}*/

void custom_cfg_audio_ec_range
(
 kal_uint8 *min_vol, /* min volume level */
 kal_uint8 *max_vol /* max volume level */
 )
{   
}

/*
void Media_SetFilter( Media_Filter_Type type, uint16 len, const int16 *filter )
{
}
*/


extern S32 gcommonvolumelevel;
extern U8 GetHandsetInPhone(void);

void PlayIsSettingKeypadVolumeReq(U16 soundId, U8 style)
{
	aud_play_id_struct *myMsgPtr = (aud_play_id_struct*)OslConstructDataPtr(sizeof(aud_play_id_struct));
	TRACE_AUD_FUNCTION();
	ASSERT(NULL != myMsgPtr);
	memset(myMsgPtr, 0, sizeof(aud_play_id_struct));
	
	myMsgPtr->volume = gcommonvolumelevel;
	myMsgPtr->audio_id = (U8)soundId;
	myMsgPtr->output_path = MDI_DEVICE_SPEAKER2;
	myMsgPtr->play_style = style;

#ifdef MMI_ON_HARDWARE_P				//Added by jinzh:20070624
	 mdi_result  result;						//Added by jinzh:20070624
	 //mdi_audio_set_volume(MDI_VOLUME_KEY  ,gcommonvolumelevel);
	 if(GetHandsetInPhone()==1)
	 {
		mdi_audio_set_volume(MDI_VOLUME_KEY  ,gcommonvolumelevel);
	 }
	 else
	 {
	 	DM_SetAudioVolume((gcommonvolumelevel) + 1);
	 }
   	 result=media_aud_play_id(MOD_MMI, myMsgPtr);		//Added by jinzh:20070624
#else									//Added by jinzh:20070624
{
	MYQUEUE Message;
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_L4C;
	Message.oslMsgId = MSG_ID_L4AUD_AUDIO_PLAY_REQ;
	Message.oslDataPtr = (oslParaType*)myMsgPtr;
	Message.oslPeerBuffPtr = NULL;
	OslMsgSendExtQueue(&Message);
}
#endif									//Added by jinzh:20070624
}
///////////////////////////////
extern S32 volume_level_UI;   //Added by jinzh:20070721
void PlayIsSettingKeypadVolumeReq_fromIdle(U16 soundId, U8 style) //Added by jinzh:20070721
{
	aud_play_id_struct *myMsgPtr = (aud_play_id_struct*)OslConstructDataPtr(sizeof(aud_play_id_struct));
	TRACE_AUD_FUNCTION();
	ASSERT(NULL != myMsgPtr);
	memset(myMsgPtr, 0, sizeof(aud_play_id_struct));
	
	myMsgPtr->volume = volume_level_UI;
	myMsgPtr->audio_id = (U8)soundId;
	myMsgPtr->output_path = MDI_DEVICE_SPEAKER2;
	myMsgPtr->play_style = style;

#ifdef MMI_ON_HARDWARE_P				
	 mdi_result  result;						
	 //mdi_audio_set_volume(MDI_VOLUME_KEY  ,volume_level_UI);
	 if(GetHandsetInPhone()==1)
	 {
		mdi_audio_set_volume(MDI_VOLUME_KEY  ,volume_level_UI);
	 }
	 else
	 {
	 	DM_SetAudioVolume((volume_level_UI) );
	 }
   	 result=media_aud_play_id(MOD_MMI, myMsgPtr);		
#else									
	 {
	MYQUEUE Message;
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_L4C;
	Message.oslMsgId = MSG_ID_L4AUD_AUDIO_PLAY_REQ;
	Message.oslDataPtr = (oslParaType*)myMsgPtr;
	Message.oslPeerBuffPtr = NULL;
	OslMsgSendExtQueue(&Message);
	 }
#endif									
}


/*
kal_int32 media_vid_record(module_type src_mod_id, void* record)
{
    return 0;
}

void media_vid_stop(module_type src_mod_id)
{
}

kal_int32 media_vid_pause(module_type src_mod_id)
{
    return 0;
}

kal_int32 media_vid_resume(module_type src_mod_id)
{
    return 0;
}
*/
void StopAudioFileSystem(U16 AudioId)
{
}

/*
void media_vid_get_current_record_time(module_type src_mod_id, kal_uint64* time)
{
}

void media_vid_clean_temp_files(module_type src_mod_id)
{
}

void media_vid_file_merge(module_type src_mod_id, kal_wchar* path)
{
}

kal_bool media_vid_check_unfinished_file(module_type src_mod_id)
{
    return KAL_FALSE;
}

kal_bool get_aud_info( kal_wchar* filename, aud_info_struct* aud_info_p )
{
}
*/

Media_Handle *JSmf_Open( void (*handler)( Media_Handle *handle, Media_Event event ), Media_SMF_Param **param )
{


   return NULL;
}

Media_Status JSmf_Close( Media_Handle *handle )
{

   return MEDIA_SUCCESS;
}

Media_Status JSmf_Play( Media_Handle *handle )
{


   return MEDIA_FAIL;
}

Media_Status JSmf_Stop( Media_Handle *handle )
{

   return MEDIA_SUCCESS;
}

Media_Status JSmf_Pause( Media_Handle *handle )
{

   return MEDIA_SUCCESS;
}

Media_Status JSmf_Resume( Media_Handle *handle )
{

   return MEDIA_SUCCESS;
}

Media_Status JSmf_SetStartTime( Media_Handle *handle, kal_int32 msStartTime )
{

   return MEDIA_SUCCESS;
}

Media_Status JSmf_SetStopTime( Media_Handle *handle, kal_int32 msStopTime )
{
   return MEDIA_SUCCESS;
}

kal_int32 JSmf_GetCurrentTime( Media_Handle *handle )
{
	return 0;
}
int32 JSmf_GetDuration( Media_Handle *handle )
{
   return 0;
}

int32 JSmf_GetMidiDuration( const uint8 *mid )
{
	return 0;
}

uint8 JMidi_GetLevel( Media_Handle *handle )
{
   return 0;
}

kal_int32 JMidi_GetRate( Media_Handle *handle )
{
	return 0;
}

kal_int32 JMidi_GetTempo( Media_Handle *handle )
{
   return 0;
}

kal_int32 JMidi_GetPitchTranspose( Media_Handle *handle )
{
   return 0;
}

kal_int8 JMidi_GetChannelVolume( Media_Handle *handle, kal_int8 channel )
{
   return 0;
}

void JMidi_GetProgram( Media_Handle *handle, kal_int8 channel, kal_int16 *bank, kal_int8 *program )
{
}

kal_bool JMidi_isBankQuerySupported( void )
{
   return KAL_TRUE;
}

void JMidi_GetBankList( kal_bool custom, const kal_int16 **buf, kal_int16 *len)
{

}

void JMidi_GetProgramList( kal_int16 bank, const kal_int8 **buf, kal_int16 *len)
{
}


const kal_uint8 *JMidi_GetProgramName( kal_int16 bank, kal_int8 prog )
{
      return NULL;
}

const kal_uint8 *JMidi_GetKeyName( kal_int16 bank, kal_int8 prog, kal_int8 key )
{

      return NULL;
}

Media_Status JMidi_SetChannelVolume( Media_Handle *handle, kal_int8 channel, kal_uint8 level )
{
   return MEDIA_SUCCESS;
}

Media_Status JMidi_SetProgram( Media_Handle *handle, kal_int8 channel, kal_int16 bank, kal_int8 program )
{

   return MEDIA_SUCCESS;
}

Media_Status JMidi_SetRate( Media_Handle *handle, kal_int32 milliRate )
{
   return MEDIA_SUCCESS;
}

Media_Status JMidi_SetTempo( Media_Handle *handle, kal_int32 milliTempo )
{
   return MEDIA_SUCCESS;
}

Media_Status JMidi_SetPitchTranspose( Media_Handle *handle, kal_int32 milliSt )
{
   return MEDIA_SUCCESS;
}

Media_Status JMidi_SetLevel( Media_Handle *handle, kal_uint8 level )
{
   return MEDIA_SUCCESS;
}

Media_Handle *JMidi_OpenDevice( void )
{
      return 0;
}

Media_Status JMidi_CloseDevice( Media_Handle *handle )
{

   return MEDIA_SUCCESS;
}

Media_Status JMidi_SendLongMsg( Media_Handle *handle, const uint8 *buf, uint16 len )
{

   return MEDIA_SUCCESS;
}

Media_Status JMidi_SendShortMsg( Media_Handle *handle, uint8 type, uint8 data1, uint8 data2 )
{
   uint8 buf[3];

   buf[0] = type;
   buf[1] = data1;
   buf[2] = data2;
   return JMidi_SendLongMsg( handle, buf, 3 );
}

Media_Handle  *JTone_Open( void (*handler)( Media_Handle *handle, Media_Event event ), Media_ToneSeq_Param **param )
{
	return NULL;
}

Media_Status JTone_Close( Media_Handle *handle)
{
   return MEDIA_SUCCESS;
}



Media_Status  JTone_Play( Media_Handle *handle )
{
   return MEDIA_SUCCESS;   
}         


Media_Status  JTone_Stop( Media_Handle *handle )
{
   return MEDIA_SUCCESS;
}


Media_Status  JTone_Pause( Media_Handle *handle ) 
{
   return MEDIA_SUCCESS;     
}

Media_Status  JTone_Resume( Media_Handle *handle )    
{
   return MEDIA_SUCCESS;
}  


Media_Status  JTone_SetStartTime( Media_Handle *handle, kal_int32 msStartTime )
{
   return MEDIA_SUCCESS;
}

 
Media_Status  JTone_SetStopTime( Media_Handle *handle, kal_int32 msStopTime )
{
   return MEDIA_SUCCESS;    
}

kal_int32 JTone_GetCurrentTime( Media_Handle *handle )
{     
   return 0;
}

kal_int32  JTone_GetDuration( Media_Handle *handle )
{
      return 0; // wrong tone sequence data 
}   

Media_Status JTone_SetLevel( Media_Handle *handle, kal_uint8 level )
{   
   return MEDIA_SUCCESS;
}


kal_uint8 JTone_GetLevel( Media_Handle *handle )           
{
   return 0;
}        
  
Media_Status JTone_SetPitchShift(Media_Handle *handle, kal_int8 pitch_shift)
{
   return MEDIA_SUCCESS;
   
}   
        

kal_int8 JTone_GetPitchShift(Media_Handle *handle)
{
   return 0;  
}   


Media_Status JTone_SetSpeed(Media_Handle *handle, kal_uint32 new_rate)
{   

   return MEDIA_SUCCESS;
}

Media_Status JTone_SetProgram(Media_Handle *handle, kal_uint8 program)
{

   return MEDIA_SUCCESS;   
}

Media_Status JTone_SetPlayStyle(Media_Handle *handle, kal_uint8 play_style)
{
   return MEDIA_SUCCESS;            
}

kal_int32 JWav_GetCurrentTime( Media_Handle *handle )                
{
   return 0;
}         

kal_int32 JWav_GetDuration( Media_Handle *handle )
{
   
   return 0;     
}   

Media_Status  JWav_SetLevel( Media_Handle *handle, uint8 level )
{     
   return MEDIA_SUCCESS;            
}     
     
kal_uint8 JWav_GetLevel( Media_Handle *handle )
{
   return 0;         
}     

Media_Handle *JImy_Open( void (*handler)( Media_Handle *handle, Media_Event event ), Media_iMelody_Param **param )
{

   return NULL;
}

Media_Status JImy_Close( Media_Handle *handle )
{

   return MEDIA_SUCCESS;
}

Media_Status JImy_Play( Media_Handle *handle )
{
   Media_Status retval=0;

   return retval;
}
/*
static void imyDspWait( Media_Handle *handle, uint16 timeout )
{

}
*/
Media_Status JImy_Stop( Media_Handle *handle )
{

   return MEDIA_SUCCESS;
}

Media_Status JImy_Pause( Media_Handle *handle )
{

   return MEDIA_SUCCESS;
}

Media_Status JImy_Resume( Media_Handle *handle )
{

   return MEDIA_SUCCESS;
}

Media_Status JImy_SetStartTime( Media_Handle *handle, kal_int32 msStartTime )
{

   return MEDIA_SUCCESS;
}

Media_Status JImy_SetStopTime( Media_Handle *handle, kal_int32 msStopTime )
{

   return MEDIA_SUCCESS;
}

kal_int32 JImy_GetCurrentTime( Media_Handle *handle )
{
   return 0;
}

int32 JImy_GetDuration( Media_Handle *handle )
{

   
   return 0;
}

Media_Status JImy_SetLevel( Media_Handle *handle, kal_uint8 level )
{

   return MEDIA_SUCCESS;
}

kal_uint8 JImy_GetLevel( Media_Handle *handle )
{
   return 0;
}


Media_Handle  *JWav_Open( void (*handler)( Media_Handle *handle, Media_Event event ), Media_Wav_Param **param )
{

   Media_Handle *handle = NULL;

     
   return handle;          
}   

Media_Status  JWav_Close( Media_Handle *handle )
{   

   return MEDIA_SUCCESS;
}         


Media_Status JWav_Load(Media_Handle *handle, uint32 *requiredBufSize) 
{

   return MEDIA_SUCCESS;             
}

void JWav_SetBuffer(Media_Handle *handle, int16 *buffer, uint32 buf_len)
{

}   
     
Media_Status  JWav_Play( Media_Handle *handle )
{
      
   return MEDIA_SUCCESS;
}               
  
Media_Status  JWav_Stop( Media_Handle *handle )
{

   return MEDIA_SUCCESS;         
}   


Media_Status JWav_Pause( Media_Handle *handle )
{

   return MEDIA_SUCCESS;               
}
 


Media_Status JWav_Resume( Media_Handle *handle )
{

   return MEDIA_SUCCESS;      
} 


Media_Status  JWav_SetStartTime( Media_Handle *handle, kal_int32 msStartTime )
{  

   return MEDIA_SUCCESS;
}

 
Media_Status  JWav_SetStopTime( Media_Handle *handle, kal_int32 msStopTime )
{        

   return MEDIA_SUCCESS;    
}    
  

Media_Handle  *JXWav_Open( void (*handler)( Media_Handle *handle, Media_Event event ))
{
	return NULL;
}

Media_Status  JXWav_Stop( Media_Handle *handle )
{
      return MEDIA_SUCCESS;
}

Media_Status  JXWav_Close( Media_Handle *handle )
{   

   return MEDIA_SUCCESS;
}   



GDI_RESULT	gdi_image_decoder_init(void)
{
	return 0;
}


