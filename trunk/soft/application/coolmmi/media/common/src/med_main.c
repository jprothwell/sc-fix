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
 *   med_main.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes primary functions of media task.
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifndef MED_NOT_PRESENT

/*==== INCLUDES =========*/

/* system includes */
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
#include "nvram_struct.h"
#include "nvram_user_defs.h"
#include "nvram_data_items.h"
#include "custom_nvram_editor_data_item.h"

/* local include */
#include "med_global.h"
#include "med_status.h"
#include "aud_defs.h"
#include "med_struct.h"
#include "med_api.h"
#include "med_context.h"
#include "med_utility.h"

#include "aud_main.h"
#include "med_main.h"
#ifdef __MED_IMG_MOD__
#include "img_main.h"
#endif
#ifdef __MED_CAM_MOD__
#include "cam_main.h"
#endif
#if defined (__MED_VID_MOD__) || defined (__MED_MJPG_MOD__)
#include "vid_main.h"
#endif
#ifdef __MED_WEBCAM_MOD__
#include "webcam_main.h"
#endif
#if defined (__MED_TVO_MOD__)  
#include "tvo_main.h"
#endif

#include "oslmemory.h"
#include "queuegprot.h"
#include "mmi_trace.h"
extern void * kal_mem_cpy(void* dest, const void* src, kal_uint32 size);
/* global variables */
med_context_struct   med_context;
med_context_struct*   med_context_p = &med_context;
#if 0
task_info_struct    *task_info_g;  
#endif 
//delect by licheng for code clean
//#pragma arm section rwdata = "INTERNRW1", zidata = "INTERNZI1"
//__align(4)  /* wufasong delete 2007.06.13 */
#if 0
kal_uint8 med_int_mem[MED_INT_MEM_SIZE];
#else
kal_uint8 med_int_mem[4];
#endif
//delect by licheng for code clean
//#pragma arm section rwdata , zidata

//__align(4)  /* wufasong delete 2007.06.13 */
#if 0
kal_uint8 med_ext_mem[MED_EXT_MEM_SIZE];
#else
kal_uint8 med_ext_mem[4];
#endif

/*==== FUNCTIONS ===========*/

extern void med_send_get_buffer_req(module_type src_mod_id, void** buffer_p, kal_uint32 buffer_size, kal_uint8 location);
extern void med_send_free_buffer_req(module_type src_mod_id, void** buffer_p, kal_uint8 location);

/*************************************************************************
* FUNCTION
*	med_nvram_read_data_cnf_hdlr
*
* DESCRIPTION
*	This function is to handle nvram read data confirm. 
*
* PARAMETERS
*	local_para_struct  *local_para_ptr
*  peer_buff_struct *peer_buff_ptr
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void med_nvram_read_data_cnf_hdlr(local_para_struct  *local_para_ptr,
                                       peer_buff_struct *peer_buff_ptr)
{
   nvram_read_cnf_struct *parm_ptr = NULL;
   kal_uint16  pdu_len;
   kal_uint8   *pdu_ptr;
   kal_uint16 cnt;
   audio_param_struct* param_p;
 
   //MED_FUNC_ENTRY(MED_NVRAM_READ_DATA_CNF_HDLR)


   parm_ptr = (nvram_read_cnf_struct *) local_para_ptr;
   pdu_ptr = get_pdu_ptr(peer_buff_ptr, &pdu_len);
   switch(parm_ptr->file_idx)
   {
      case NVRAM_EF_CUST_ACOUSTIC_DATA_LID:
         if (parm_ptr->length != sizeof(custom_acoustic_struct))
         {
            /* error reading length */
            ASSERT(parm_ptr->length != sizeof(custom_acoustic_struct))
            return ;
         }
         kal_mem_cpy(&(aud_context_p->acoustic_data), (custom_acoustic_struct *) pdu_ptr, sizeof(custom_acoustic_struct) );

         if (med_context_p->nvram_state==MED_POWERON_READING_NVRAM)
            aud_send_msg_to_nvram(MSG_ID_NVRAM_READ_REQ, NVRAM_EF_AUDIO_PARAM_LID, 0, 0);
         break;

      case NVRAM_EF_AUDIO_PARAM_LID:
         if (parm_ptr->length != NVRAM_EF_AUDIO_PARAM_SIZE )
         {
            /* error reading length */
            ASSERT(parm_ptr->length != NVRAM_EF_AUDIO_PARAM_SIZE);
            return ;
         }

         param_p = &(aud_context_p->audio_param);
         cnt=0;
         kal_mem_cpy(param_p->FIR_input_coeff[0], (pdu_ptr+cnt), (sizeof(kal_uint16)*30));
         cnt+= sizeof(kal_uint16)*30;
      	kal_mem_cpy(param_p->FIR_output_coeff[0], (pdu_ptr+cnt), (sizeof(kal_uint16)*30));
         cnt+= sizeof(kal_uint16)*30;
         kal_mem_cpy(param_p->FIR_output_coeff[1], (pdu_ptr+cnt), (sizeof(kal_uint16)*30));
         cnt+= sizeof(kal_uint16)*30;
         kal_mem_cpy(param_p->FIR_output_coeff[2], (pdu_ptr+cnt), (sizeof(kal_uint16)*30));
         cnt+= sizeof(kal_uint16)*30;
         kal_mem_cpy(param_p->FIR_output_coeff[3], (pdu_ptr+cnt), (sizeof(kal_uint16)*30));
         cnt+= sizeof(kal_uint16)*30;
         kal_mem_cpy(param_p->FIR_output_coeff[4], (pdu_ptr+cnt), (sizeof(kal_uint16)*30));
         cnt+= sizeof(kal_uint16)*30;
         kal_mem_cpy(param_p->FIR_output_coeff[5], (pdu_ptr+cnt), (sizeof(kal_uint16)*30));
         cnt+= sizeof(kal_uint16)*30;
         kal_mem_cpy(param_p->FIR_input_coeff[1], (pdu_ptr+cnt), (sizeof(kal_uint16)*30));
         cnt+= sizeof(kal_uint16)*30;
         kal_mem_cpy(param_p->FIR_output_coeff[6], (pdu_ptr+cnt), (sizeof(kal_uint16)*30));
         cnt+= sizeof(kal_uint16)*30;
         kal_mem_cpy(&(param_p->selected_FIR_output_index), (pdu_ptr+cnt),  sizeof(kal_uint16)); 
         cnt+= sizeof(kal_uint16);
         kal_mem_cpy(&(param_p->es_time_const), (pdu_ptr+cnt),  sizeof(kal_uint16)); 
         cnt+= sizeof(kal_uint16);
         kal_mem_cpy(&(param_p->es_vol_const), (pdu_ptr+cnt), sizeof(kal_uint16));
         cnt+= sizeof(kal_uint16);
         kal_mem_cpy(&(param_p->es_time_const2), (pdu_ptr+cnt),  sizeof(kal_uint16)); 
         cnt+= sizeof(kal_uint16);
         kal_mem_cpy(&(param_p->es_vol_const2), (pdu_ptr+cnt), sizeof(kal_uint16));
         cnt+= sizeof(kal_uint16);
         kal_mem_cpy(&(param_p->max_swing), (pdu_ptr+cnt), sizeof(kal_uint16));
         cnt+= sizeof(kal_uint16);
         kal_mem_cpy(param_p->Speech_FIR_Input_Coeff_16k_Tbl1, (pdu_ptr+cnt), (sizeof(kal_int16)*62));
         cnt+= sizeof(kal_int16)*62;
         kal_mem_cpy(param_p->Speech_FIR_Output_Coeff_16k_Tbl1, (pdu_ptr+cnt), (sizeof(kal_int16)*62));
         cnt+= sizeof(kal_int16)*62;
         kal_mem_cpy(param_p->Speech_FIR_Input_Coeff_16k_Tbl2, (pdu_ptr+cnt), (sizeof(kal_int16)*62));
         cnt+= sizeof(kal_int16)*62;
         kal_mem_cpy(param_p->Speech_FIR_Output_Coeff_16k_Tbl2, (pdu_ptr+cnt), (sizeof(kal_int16)*62));
         cnt+= sizeof(kal_int16)*62;
         kal_mem_cpy(param_p->Melody_FIR_Output_Coeff_32k_Tbl1, (pdu_ptr+cnt), (sizeof(kal_int16)*45));
         cnt+= sizeof(kal_int16)*45;
         kal_mem_cpy(param_p->Melody_FIR_Output_Coeff_32k_Tbl2, (pdu_ptr+cnt), (sizeof(kal_int16)*45));
         cnt+= sizeof(kal_int16)*45;

         if (med_context_p->nvram_state==MED_POWERON_READING_NVRAM)
         {
            aud_set_startup_param();
#ifdef ISP_SUPPORT
            aud_send_msg_to_nvram(MSG_ID_NVRAM_READ_REQ, NVRAM_EF_CAMERA_PARA_LID, 0, 0);
#else
            aud_send_startup_cnf(MED_RES_OK);
            med_context_p->nvram_state=MED_POWERON_READING_NVRAM_FINISH;
#endif
         }
         else
            ASSERT(med_context_p->nvram_state==MED_POWERON_READING_NVRAM);
         break;
#ifdef ISP_SUPPORT
      case NVRAM_EF_CAMERA_PARA_LID:
         /* set camera parameters to driver */
  			set_camera_para((nvram_camera_para_struct *)pdu_ptr);
    
         if (med_context_p->nvram_state==MED_POWERON_READING_NVRAM)
         {
#if (defined(MT6219))
            aud_send_startup_cnf(MED_RES_OK);
            med_context_p->nvram_state=MED_POWERON_READING_NVRAM_FINISH;
#else  
            aud_send_msg_to_nvram(MSG_ID_NVRAM_READ_REQ, NVRAM_EF_CAMERA_DEFECT_LID, 0, 0);
#endif
         }
         else
            ASSERT(med_context_p->nvram_state==MED_POWERON_READING_NVRAM);
         break;
         
#if (!defined(MT6219))
      case NVRAM_EF_CAMERA_DEFECT_LID:
         /* set camera defect parameters to driver */
  	   set_camera_defect((nvram_camera_defect_struct *)pdu_ptr);
    
         if (med_context_p->nvram_state==MED_POWERON_READING_NVRAM)
         {
            aud_send_msg_to_nvram(MSG_ID_NVRAM_READ_REQ, NVRAM_EF_CAMERA_LENS_LID, 0, 0);
         }
         else
            ASSERT(med_context_p->nvram_state==MED_POWERON_READING_NVRAM);
         break;
      case NVRAM_EF_CAMERA_LENS_LID:
         /* set camera lens parameters to driver */
  	   set_camera_lens((nvram_camera_lens_struct *)pdu_ptr);
    
         if (med_context_p->nvram_state==MED_POWERON_READING_NVRAM)
         {
#if defined(MT6228)
            aud_send_startup_cnf(MED_RES_OK);
            med_context_p->nvram_state=MED_POWERON_READING_NVRAM_FINISH;
#else
            aud_send_msg_to_nvram(MSG_ID_NVRAM_READ_REQ, NVRAM_EF_CAMERA_GAMMA_LID, 0, 0);
#endif
         }
         else
            ASSERT(med_context_p->nvram_state==MED_POWERON_READING_NVRAM);
         break;
#if (!defined(MT6228))
      case NVRAM_EF_CAMERA_GAMMA_LID:
         /* set camera lens parameters to driver */
  	   set_camera_gamma((nvram_camera_gamma_struct *)pdu_ptr);
    
         if (med_context_p->nvram_state==MED_POWERON_READING_NVRAM)
         {
            aud_send_startup_cnf(MED_RES_OK);
            med_context_p->nvram_state=MED_POWERON_READING_NVRAM_FINISH;
         }
         else
            ASSERT(med_context_p->nvram_state==MED_POWERON_READING_NVRAM);
#endif  

#endif	 


#endif /* ISP_SUPPORT */
      default:
         break;
   }
}

/*************************************************************************
* FUNCTION
*	med_nvram_write_data_cnf_hdlr
*
* DESCRIPTION
*	This function is to handle nvram write data confirm. 
*
* PARAMETERS
*	local_para_struct  *local_para_ptr
*  peer_buff_struct *peer_buff_ptr
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void med_nvram_write_data_cnf_hdlr(local_para_struct  *local_para_ptr,
                                        peer_buff_struct *peer_buff_ptr)
{
   nvram_write_cnf_struct *parm_ptr = NULL;

   parm_ptr = (nvram_write_cnf_struct *) local_para_ptr;
 
 
   //MED_FUNC_ENTRY(MED_NVRAM_WRITE_DATA_CNF_HDLR)


   switch(parm_ptr->file_idx)
   {
   case NVRAM_EF_CUST_ACOUSTIC_DATA_LID:
      aud_send_set_audio_profile_cnf(MED_RES_OK);
      break;
   case NVRAM_EF_AUDIO_PARAM_LID:
      aud_send_set_audio_param_cnf(MED_RES_OK);
      break;
   default:
      break;
   }
   
   
}

/******************************************************************
* FUNCTION
*    med_startup_hdlr
* DESCRIPTION
*    This function is handle startup procedure of media task.
* PARAMETERS
*    
*    
* RETURNS
*
* GLOBALS AFFECTED
*  
*  
******************************************************************/
void med_startup_hdlr(ilm_struct *ilm_ptr)
{
	mmi_trace(TRUE, "Excute func : med_startup_hdlr(src_mod_id = %d, dest_mod_id = %d)", ilm_ptr->src_mod_id, ilm_ptr->dest_mod_id);
	
//#if 0
   /* start base timer */
   stack_init_timer(&med_context_p->base_timer, "MED Base Timer", MOD_MED);

   med_context_p->event_scheduler_ptr = 
         new_evshed(&med_context_p->base_timer, 
                     med_start_base_timer, 
                     med_stop_base_timer, 
                     0 , kal_evshed_get_mem, 
                     kal_evshed_free_mem, 0);
  med_context_p->med_event = kal_create_event_group("med_events");
                    
//#if 0
#ifdef __MED_CAM_MOD__
   cam_startup_hdlr(ilm_ptr);
#endif   
#if 1
#ifdef __MED_IMG_MOD__
   img_startup_hdlr(ilm_ptr);
#endif   
#if defined (__MED_VID_MOD__) || defined (__MED_MJPG_MOD__)
   vid_startup_hdlr(ilm_ptr);
#endif   
#ifdef __MED_WEBCAM_MOD__
   webcam_startup_hdlr(ilm_ptr);
#endif   
#if defined (__MED_TVO_MOD__)
   tvo_startup_hdlr(ilm_ptr);
#endif   

   aud_startup_hdlr(ilm_ptr);

   /* start to read nvram data for media task */
//   aud_startup_read_nvram_data(); /* by zhaoyong */

#endif   
}




/******************************************************************
* FUNCTION
*    med_main
* DESCRIPTION
*    This function is main message dispatching function of media task.
* PARAMETERS
*    
*    
* RETURNS
*
* GLOBALS AFFECTED
*  
*  
******************************************************************/
void med_main( ilm_struct *ilm_ptr)
{

 //   mmi_trace(TRUE, "Excute func : med_main(%d)", ilm_ptr->msg_id);
//   mmi_trace(TRUE, "MSG_ID_MED_CODE_BEGIN's id is %d", MSG_ID_MED_CODE_BEGIN);  
   
   if(ilm_ptr->msg_id==MSG_ID_MED_STARTUP_REQ)
   {
      med_startup_hdlr(ilm_ptr);
   }
   else if(ilm_ptr->msg_id==MSG_ID_MEDIA_GET_BUFFER_REQ)
   {
      med_get_buffer_req_hdlr(ilm_ptr);
   }
   else if(ilm_ptr->msg_id==MSG_ID_MEDIA_FREE_BUFFER_REQ)
   {
      med_free_buffer_req_hdlr(ilm_ptr);
   }
#ifdef TST_HANDLER
   else if(ilm_ptr->msg_id==MSG_ID_TST_INJECT_STRING)
   {
//      aud_tst_handler(ilm_ptr);  //deleted by WeiD
   }
#endif  /* TST_HANDLER */
#ifdef __MED_CAM_MOD__
   else if(ilm_ptr->msg_id>=MSG_ID_MED_CAM_CODE_BEGIN &&
         ilm_ptr->msg_id<=MSG_ID_MED_CAM_CODE_END)
   {
      cam_main(ilm_ptr);
   }
#endif
#ifdef __MED_IMG_MOD__
   else if(ilm_ptr->msg_id>=MSG_ID_MED_IMG_CODE_BEGIN &&
         ilm_ptr->msg_id<=MSG_ID_MED_IMG_CODE_END)
   {
      img_main(ilm_ptr);    //deleted by WeiD
   }
#endif
#if defined (__MED_VID_MOD__) || defined (__MED_MJPG_MOD__)
   else if(ilm_ptr->msg_id>=MSG_ID_MED_VID_CODE_BEGIN &&
         ilm_ptr->msg_id<=MSG_ID_MED_VID_CODE_END)
   {
      vid_main(ilm_ptr); 
   }
#if 0
   else if(ilm_ptr->msg_id>=MSG_ID_MED_V_CODE_BEGIN &&
         ilm_ptr->msg_id<=MSG_ID_MED_V_CODE_END)
   {
      vid_main(ilm_ptr); 
   }
#endif
#endif
#ifdef __MED_WEBCAM_MOD__
   else if((ilm_ptr->msg_id>=MSG_ID_MED_WEBCAM_CODE_BEGIN &&
         ilm_ptr->msg_id<=MSG_ID_MED_WEBCAM_CODE_END) || 
         (ilm_ptr->msg_id>=MSG_ID_USB_MED_SET_CAMERA_ATTR_REQ &&
         ilm_ptr->msg_id<=MSG_ID_USB_MED_STOP_CNF))
   {
      webcam_main(ilm_ptr);
   }
#endif
#ifdef __MED_TVO_MOD__
   else if(ilm_ptr->msg_id>=MSG_ID_MED_TVO_CODE_BEGIN &&
         ilm_ptr->msg_id<=MSG_ID_MED_TVO_CODE_END)
   {
//      tvo_main(ilm_ptr);   //deleted by WeiD
   }
#endif
   else if(ilm_ptr->msg_id==MSG_ID_NVRAM_READ_CNF)
       med_nvram_read_data_cnf_hdlr(ilm_ptr->local_para_ptr, ilm_ptr->peer_buff_ptr);
   else if(ilm_ptr->msg_id==MSG_ID_NVRAM_WRITE_CNF)
       med_nvram_write_data_cnf_hdlr(ilm_ptr->local_para_ptr, ilm_ptr->peer_buff_ptr);
   else
   {
      aud_main(ilm_ptr);
   }
}

/******************************************************************
* FUNCTION
*    med_task_main
* DESCRIPTION
*    This function is main function of media task.
* PARAMETERS
*    
*    
* RETURNS
*
* GLOBALS AFFECTED
*  
*  
******************************************************************/
kal_status receive_msg_ext_q( kal_msgqid task_ext_qid,
                                     ilm_struct *ilm_ptr);
void free_ilm(ilm_struct* ilm_ptr);
#if 0

void med_task_main( task_entry_struct *task_entry_ptr)
{
   ilm_struct current_ilm;
   kal_uint32 my_index;

   kal_get_my_task_index(&my_index);

   while (1) 
   {
      receive_msg_ext_q( task_info_g[task_entry_ptr->task_indx].task_ext_qid, 
                           &current_ilm);
      stack_set_active_module_id( my_index, current_ilm.dest_mod_id);

      med_main((void *) &current_ilm);
            
      free_ilm( &current_ilm);
   }

}
#endif 


void med_send_startup_req(void)
{
    ilm_struct  ilm_ptr;
    med_startup_req_struct *pMedStartupReq = NULL;

	mmi_trace(TRUE, "Excute func : med_send_startup_req()");
    
    pMedStartupReq = (med_startup_req_struct *)OslMalloc(sizeof(med_startup_req_struct));
    ASSERT(pMedStartupReq != NULL);
    memset(pMedStartupReq, 0x00, sizeof(med_startup_req_struct));
    
    ilm_ptr.dest_mod_id = MOD_MED;
    ilm_ptr.src_mod_id = MOD_MED;
    ilm_ptr.local_para_ptr= (local_para_struct*)pMedStartupReq;
    ilm_ptr.peer_buff_ptr = NULL;
    ilm_ptr.msg_id = MSG_ID_MED_STARTUP_REQ;
	mmi_trace(TRUE, "before send mesg");
    OslMsgSendExtQueue(&ilm_ptr);
	mmi_trace(TRUE, "after send mesg");
}
/******************************************************************
* FUNCTION
*    med_init
* DESCRIPTION
*    This function is used to init media task.
* PARAMETERS
*    
*    
* RETURNS
*
* GLOBALS AFFECTED
*  
*  
******************************************************************/

void InitAllocMediaMemory(void);
extern HANDLE g_audioSem;//chenhe,20100417
extern HANDLE g_vid_sem ;//chenhe,20100419
HANDLE g_WebcamSem ;

kal_bool med_init( task_indx_type task_indx )
{
	//U8 i =0;
	//HANDLE handle=0;
 
	mmi_trace(TRUE, "Excute func 2 : med_init()");
	/* init memory pool for multimedia app, total memory size is 128 * 1024 Bytes. */
/*
   for(i = 0; i<MAX_CAM_SEM; i++)
   	{
	   Cam_Semaphore[i] = COS_CreateSemaphore(0);
	   handle =Cam_Semaphore[i];
	   mmi_trace(TRUE,"The Cam_Semaphore[i] is %d, 0x%x",i,handle);
   	}

  for(i = 0; i<MAX_VID_SEM; i++)
   	{
	   Vid_Semaphore[i] = COS_CreateSemaphore(0);
	   handle =Vid_Semaphore[i];
	   mmi_trace(TRUE,"The Vid_Semaphore[i] is %d, 0x%x",i,handle);
   	}	
*/
	   Cam_Semaphore[0] = COS_CreateSemaphore(0); //chenhe use 1 semphore for camera

  InitAllocMediaMemory();
	
   med_context_p->int_mem_p=med_int_mem;
   med_context_p->ext_mem_p=med_ext_mem;

   if(!aud_init())
      return KAL_FALSE;
#ifdef __MED_IMG_MOD__
   if(!img_init())
      return KAL_FALSE;
#endif

#ifdef __MED_CAM_MOD__
   if(!cam_init())
      return KAL_FALSE;
#endif

#if defined (__MED_VID_MOD__) || defined (__MED_MJPG_MOD__)
   if(!vid_init())
      return KAL_FALSE;
#endif

#ifdef __MED_WEBCAM_MOD__
   if(!webcam_init())
      return KAL_FALSE;
#endif
#if defined (__MED_TVO_MOD__) 
   if(!tvo_init())
      return KAL_FALSE;
#endif

   if (!med_utility_init())
      return KAL_FALSE;

	/* wufasong added 2007.06.15. */
	/* 向(Media Task)自身发送 MSG_ID_MED_STARTUP_REQ */
	med_send_startup_req();


	g_audioSem = COS_CreateSemaphore(0);
#if defined (__MED_VID_MOD__) || defined (__MED_MJPG_MOD__)
	g_vid_sem = COS_CreateSemaphore(0);
#endif
#ifdef __MED_WEBCAM_MOD__
	g_WebcamSem = COS_CreateSemaphore(0);
#endif
   return KAL_TRUE;
}

/******************************************************************
* FUNCTION
*    med_reset
* DESCRIPTION
*    This function is used to reset media task.
* PARAMETERS
*    
*    
* RETURNS
*
* GLOBALS AFFECTED
*  
*  
******************************************************************/
kal_bool med_reset( task_indx_type task_indx )
{
   return KAL_TRUE;
}

/******************************************************************
* FUNCTION
*    med_create
* DESCRIPTION
*    This function is used to create media task configuration info.
* PARAMETERS
*    
*    
* RETURNS
*
* GLOBALS AFFECTED
*  
*  
******************************************************************/
#if 0
kal_bool med_create(comptask_handler_struct **handle)
{
   static const comptask_handler_struct med_handler_info = 
   {
      med_task_main,         /* task entry function */
      med_init,         /* task initialization function */
      NULL,      /* task configuration function */
      med_reset,            /* task reset handler */
      NULL,            /* task termination handler */
   };

   *handle = (comptask_handler_struct*)&med_handler_info;
   
   return KAL_TRUE;
}
#endif 



#endif

