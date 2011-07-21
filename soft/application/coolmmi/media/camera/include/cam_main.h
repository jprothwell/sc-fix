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
 *	cam_main.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes primary global functions/variables of image module.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#ifndef MED_NOT_PRESENT

#ifdef __MED_CAM_MOD__

#ifndef _CAM_MAIN_H
#define _CAM_MAIN_H

#include "media_others.h"
#include "csw.h"

#define CAM_POWER_UP_INIT_TIME      200   /* 200ms, Power up polling first time period */
#define CAM_POWER_UP_POLLING_TIME   50   /* 20ms, Power up periodic polling time out period *///fengzb
#define CAM_PREVIEW_INIT_TIME      20   /* 10ms, Preview polling first time period *///fengzb

#ifdef CAMERA_MODULE_WITH_LCD
#define CAM_PREVIEW_POLLING_TIME    200   /* 200ms, Preview periodic polling time period */
#else
#define CAM_PREVIEW_POLLING_TIME    200   /* 10ms, Preview periodic polling time period *///fengzb
#endif
#define CAM_CAPTURE_INIT_TIME      200   /* 20ms, Capture polling first time period *///fengzb
#define CAM_CAPTURE_POLLING_TIME    200   /* 20ms, Capture periodic polling time period *///fengzb

#define MAX_POWER_UP_CHECK_COUNT 4         //4  fengzb
#define MAX_PREVIEW_CHECK_COUNT 150     //150    fengzb
#define MAX_PREVIEW_RETRY_COUNT 30       //3 fengzb
#define MAX_CAPTURE_CHECK_COUNT 400
#define MAX_CAPTURE_RETRY_COUNT 3


#define CAPTURE_MEM_MARGIN  (512)   /* Minimal necessary storage space for capture. */
#define MIN_JPG_FILE_SIZE (128)       /* Minimal necessary storage space for capture. */  

typedef enum
{
   CAM_EVT_NOWAIT    = 0,
   CAM_EVT_PREVIEW	     = 0x0001,
   CAM_EVT_STOP     = 0x0002,
   CAM_EVT_CAPTURE      = 0x0004,
   CAM_EVT_SET_PARAM      = 0x0008,
   CAM_EVT_POWER_UP      = 0x0010,
   CAM_EVT_POWER_DOWN      = 0x0020,
      CAM_EVT_SAVE_PHOTO      = 0x0021

} 
cam_wait_event_enum;

#if 0
#define CAM_WAIT_EVENT(evt_) do{ \
         kal_uint32 retrieved_events;\
         CAM_WAIT_EVT(evt_, __LINE__);\
         kal_retrieve_eg_events(cam_context_p->cam_event, (evt_), \
         KAL_OR_CONSUME, &retrieved_events, KAL_SUSPEND); }while(0) 


#define CAM_SET_EVENT(evt_) do{ \
                              kal_set_eg_events(cam_context_p->cam_event, (evt_),KAL_OR);\
                              CAM_SET_EVT(evt_,__LINE__); \
                              } while(0)

#elif 0
	extern kal_uint32 cam_sam;

#define CAM_WAIT_EVENT(evt_) do{ \
	mmi_trace(g_sw_CAM, "CAM_WAIT_EVENT is waiting for semaphore (0x%x)", evt_); \
	while(1) \
	{ \
	 	if(cam_sam & ((kal_uint32)evt_)) \
	 	{ \
	 		cam_sam &= !((kal_uint32)evt_); \
			mmi_trace(g_sw_CAM, "CAM_WAIT_EVENT has taken semaphore (0x%x)", evt_); \
			break; \
	 	} \
		COS_Sleep(5);\
	} \
 }while(0) 


#define CAM_SET_EVENT(evt_) do{ \
	mmi_trace(g_sw_CAM,"CAM_WAIT_EVENT has given semaphore (0x%x)", evt_); \
	cam_sam |= (kal_uint32)evt_; \
    } while(0)
#elif 1
#define MAX_CAM_SEM 1
extern HANDLE Cam_Semaphore[MAX_CAM_SEM];
#define CAM_WAIT_EVENT(evt_) do{COS_WaitForSemaphore(Cam_Semaphore[0],COS_WAIT_FOREVER); mmi_trace(0,"CAM_WAIT_EVENT[0x%x].",evt_);}while(0)
#define CAM_SET_EVENT(evt_)  do{COS_ReleaseSemaphore(Cam_Semaphore[0]);mmi_trace(0,"CAM_SET_EVENT[0x%x].",evt_);}while(0)
#else

	#define MAX_CAM_SEM 1

	extern HANDLE Cam_Semaphore[MAX_CAM_SEM];
	
	#define CAM_WAIT_EVENT(evt_) switch(evt_){ \
		case CAM_EVT_PREVIEW: \
			TRACE_CAM_FUNCTION();\
			mmi_trace(0,"Wait_Semaphore is taking CAM_EVT_PREVIEW semphore,Cam_Semaphore[0] = 0x%x.",Cam_Semaphore[0]); \
			COS_WaitForSemaphore(Cam_Semaphore[0],COS_WAIT_FOREVER); \
			TRACE_CAM_FUNCTION(); \
			mmi_trace(0,"Wait_Semaphore has taken CAM_EVT_PREVIEW semphore Cam_Semaphore[0] = 0x%x.",Cam_Semaphore[0]); \
			break; \
		case CAM_EVT_STOP: \
			COS_WaitForSemaphore(Cam_Semaphore[0],COS_WAIT_FOREVER); \
			mmi_trace(0,"The Cam_evt_Wait_Semaphore is TRUE,Cam_Semaphore[0] = 0x%x",Cam_Semaphore[0]); \
			break; \
		case CAM_EVT_CAPTURE: \
			mmi_trace(0,"Wait_Semaphore is taking CAM_EVT_CAPTURE semphore.Cam_Semaphore[0] "); \
		 	COS_WaitForSemaphore(Cam_Semaphore[0],COS_WAIT_FOREVER); \
		 	mmi_trace(0,"The Cam_evt_Wait_Semaphore is TRUE"); \
			break; \
		case CAM_EVT_SET_PARAM: \
			COS_WaitForSemaphore(Cam_Semaphore[0],COS_WAIT_FOREVER); \
			break; \
		case CAM_EVT_POWER_UP: \
			TRACE_CAM_FUNCTION();\
			mmi_trace(0,"Wait_Semaphore is taking CAM_EVT_POWER_UP semphore."); \
			COS_WaitForSemaphore(Cam_Semaphore[0],COS_WAIT_FOREVER); \
			TRACE_CAM_FUNCTION(); \
			mmi_trace(0,"Wait_Semaphore has taken CAM_EVT_POWER_UP semphore."); \
			break; \
		case CAM_EVT_NOWAIT: \
			COS_WaitForSemaphore(Cam_Semaphore[0],COS_WAIT_FOREVER); \
			break; \
		case CAM_EVT_POWER_DOWN: \
			TRACE_CAM_FUNCTION();\
			COS_WaitForSemaphore(Cam_Semaphore[0],COS_WAIT_FOREVER); \
			TRACE_CAM_FUNCTION(); \
			break; \
			}

#define CAM_SET_EVENT(evt_)  switch(evt_){ \
		case CAM_EVT_PREVIEW: \
			COS_ReleaseSemaphore(Cam_Semaphore[0]); \
			TRACE_CAM_FUNCTION(); \
			mmi_trace(0,"COS_ReleaseSemaphore has given CAM_EVT_PREVIEW semphore."); \
			break; \
		case CAM_EVT_STOP: \
			COS_ReleaseSemaphore(Cam_Semaphore[0]); \
			mmi_trace(0,"The Cam_Stop_Set_Semaphore is TRUE"); \
			break; \
		case CAM_EVT_CAPTURE: \
		 	COS_ReleaseSemaphore(Cam_Semaphore[0]); \
		 	mmi_trace(0,"The Cam_Evt_Set_Semaphore is TRUE"); \
			break; \
		case CAM_EVT_SET_PARAM: \
			COS_ReleaseSemaphore(Cam_Semaphore[0]); \
			break; \
		case CAM_EVT_POWER_UP: \
			COS_ReleaseSemaphore(Cam_Semaphore[0]); \
			TRACE_CAM_FUNCTION(); \
			mmi_trace(0,"COS_ReleaseSemaphore has given CAM_EVT_POWER_UP semphore."); \
			break; \
		case CAM_EVT_NOWAIT: \
			COS_ReleaseSemaphore(Cam_Semaphore[0]); \
			break; \
		case CAM_EVT_POWER_DOWN: \
			COS_ReleaseSemaphore(Cam_Semaphore[0]); \
			mmi_trace(0,"COS_ReleaseSemaphore has given CAM_EVT_POWER_DOWN semphore."); \
			break; \
         }

#endif
/*==== PROTOTYPE ===========*/
/* main */
void cam_startup_hdlr( ilm_struct *ilm_ptr);
void cam_main( ilm_struct *ilm_ptr);
kal_bool cam_init(void);

/* msg handler */
kal_int16 cam_set_flash(kal_int16 flash_mode);
void cam_set_flash_req_hdlr(ilm_struct *ilm_ptr);
#if defined (ISP_SUPPORT)
void cam_release_memory(void);
#endif
#ifdef CAMERA_MODULE
void cam_power_up_check(void* arg);
#endif
void cam_power_up_req_hdlr(ilm_struct *ilm_ptr);
void cam_power_down_req_hdlr(ilm_struct *ilm_ptr);
void cam_preview_callback(kal_uint8 cause);
void cam_preview_req_hdlr(ilm_struct *ilm_ptr);
void cam_stop_req_hdlr(ilm_struct *ilm_ptr);
void cam_capture_req_hdlr(ilm_struct *ilm_ptr);
void cam_set_param_req_hdlr(ilm_struct *ilm_ptr);
void cam_abort_to_idle(void);
#if defined (ISP_SUPPORT)
void cam_event_ind_hdlr(ilm_struct *ilm_ptr);
#endif
void cam_preview_restart(void);


kal_int16 cam_set_zoom_factor(kal_int16 value);
kal_int16 cam_set_contrast(kal_int16 value);
kal_int16 cam_set_brightness(kal_int16 value);
kal_int16 cam_set_hue(kal_int16 value);
kal_int16 cam_set_gamma(kal_int16 value);
kal_int16 cam_set_wb(kal_int16 value);
kal_int16 cam_set_exposure(kal_int16 value);
kal_int16 cam_set_effect(kal_int16 value);
kal_int16 cam_set_night_mode(kal_int16 value);
//add by WeiD
kal_int16 cam_set_banding(kal_int16 value);
kal_int16 cam_set_quality(kal_int16 value);
kal_int16 cam_set_imagesize(kal_int16 value);

/* ilm */
void cam_send_power_up_req(module_type src_mod_id, kal_uint16  seq_num);
void cam_send_power_down_req(module_type src_mod_id, kal_int16 delay_time);
void cam_send_preview_req(module_type src_mod_id, void* data);
void cam_send_capture_req(module_type src_mod_id, void* data);
void cam_send_stop_req(module_type src_mod_id);
void cam_send_set_param_req(module_type src_mod_id, kal_uint16 param_id, kal_int16 value);
void cam_send_set_flash_req(module_type src_mod_id, kal_int16 flash_mode);
void cam_send_ready_ind(kal_int16 result);
void cam_send_preview_fail_ind(kal_int16 result);
void cam_send_event_ind(kal_uint8 event, kal_uint8 cause);
void cam_send_capture_event_ind(kal_int16 result, kal_uint16 count);
void cam_send_md_open_req(module_type src_mod_id, kal_uint32 sensLevel, void(*md_callback)(int32));
void cam_send_md_close_req(module_type src_mod_id);
void cam_send_md_work_ind(int32 result);
void cam_send_cam_save_photo_req(module_type src_mod_id,UINT32 filehandle);
void cam_save_photo_hdlr(ilm_struct *ilm_ptr);


/* api */
void cam_set_result(kal_int32 result);

//for motion detect
void cam_md_hdlr(ilm_struct *ilm_ptr);
void cam_md_open_hdlr(ilm_struct *ilm_ptr);
void cam_md_close_hdlr(ilm_struct *ilm_ptr);

#endif /* _CAM_MAIN_H */

#endif /* __MED_CAM_MOD__ */

#endif /* MED_NOT_PRESENT */


