/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2003
*
*******************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *   img_msg_handler.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes message handle functions of image module.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------

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

/* DRM_REPLACE */
#include "drm_gprot.h"

/* local includes */
#include "med_global.h"
#include "med_main.h"
#include "aud_defs.h"
#include "med_struct.h"
#include "med_api.h"
#include "med_context.h"
#include "med_utility.h"
//#include "med_smalloc.h"
#include "img_main.h"
#include "mmi_trace.h"

//#include "vim_high_api.h"
//#include "vim_common.h"

media_img_decode_req_struct img_context_last_req;

#if (defined(EXIF_SUPPORT))	
//#include "exif.h"
#endif


#include "fat_fs.h"

#include "media_others.h"


#ifdef __MED_IMG_MOD__

//#include "jpeg.h"

/*************************************************************************
* FUNCTION
*   img_release_memory
*
* DESCRIPTION
*   This function is to release memory for image decoding/encoding. 
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void img_release_memory(void)
{
   if (img_context_p->file_buffer_address)
      med_free_ext_mem((void**)&img_context_p->file_buffer_address);
}



/*************************************************************************
* FUNCTION
*   img_decode_req_hdlr
*
* DESCRIPTION
*   This function is to handle thd decode request. 
*
* PARAMETERS
*   ilm_struct *ilm_ptr
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void img_decode_req_hdlr(ilm_struct *ilm_ptr)
{
	kal_int16 result;
	media_img_decode_req_struct *req_p;


	img_context_p->src_mod = ilm_ptr->src_mod_id;

	req_p = (media_img_decode_req_struct*)ilm_ptr->local_para_ptr;

	img_context_p->seq_num=req_p->seq_num;
	mmi_trace(TRUE, "img_decode_req_hdlr()::img_context_p->state is %d", img_context_p->state);

	if(!IMG_IN_STATE(IMG_IDLE))
	{
		img_set_result(MED_RES_BUSY);
		IMG_SET_EVENT(IMG_EVT_DECODE);
		img_send_decode_finish_ind(MED_RES_BUSY, img_context_p->seq_num);
		return;
	}

	img_context_p->media_mode=req_p->media_mode;
	img_context_p->media_type=req_p->media_type;

	{
		result = MED_RES_INVALID_FORMAT;
	  mmi_trace(TRUE, "MED_RES_INVALID_FORMAT");
	}

	img_send_decode_finish_ind(result, img_context_p->seq_num);
	img_set_result(result);
	IMG_SET_EVENT(IMG_EVT_DECODE);

}





/*************************************************************************
* FUNCTION
*   img_resize_req_hdlr
*
* DESCRIPTION
*   This function is to handle thd resize request. 
*
* PARAMETERS
*   ilm_struct *ilm_ptr
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void img_resize_req_hdlr(ilm_struct *ilm_ptr)
{

 //     UINT16 decoderesult;
   media_img_resize_req_struct *req_p;
      kal_int16 result;

   img_context_p->src_mod = ilm_ptr->src_mod_id;

   req_p = (media_img_resize_req_struct*)ilm_ptr->local_para_ptr;

   img_context_p->seq_num=req_p->seq_num;

	if (img_context_p->state==IMG_IDLE)
	{


#if 0
		VIM_HAPI_SetWorkMode(VIM_HAPI_MODE_PHOTOPROCESS);
		//licheng fixed for clean code
		decoderesult = VIM_HAPI_Decode_Jpeg(VIM_HAPI_RAM_SAVE,VIM_DECODE_RESIZE_TO_RGB,(HUGE void *)req_p->src_image_address,
		                             (HUGE void *)req_p->dest_image_address, req_p->src_width*req_p->src_height*2,
		                              req_p->dest_buffer_size,
		                              req_p->target_width,req_p->target_height);

		VIM_HAPI_SetWorkMode(VIM_HAPI_MODE_BYPASS);

		if(decoderesult == VIM_SUCCEED)
			result=MED_RES_OK;
		else
#endif			
			result=MED_RES_ERROR;

		img_send_resize_done_ind((kal_int16)result, img_context_p->seq_num);
		img_set_result(result);               
      
	}
	else
	{
	     img_send_resize_done_ind(MED_RES_BUSY, img_context_p->seq_num);
	     img_set_result(MED_RES_BUSY);               
	}

	IMG_SET_EVENT(IMG_EVT_RESIZE);
      
}


void img_exif_decode_req_hdlr(ilm_struct *ilm_ptr)
{

	IMG_SET_EVENT(IMG_EVT_EXIF_DECODE);

}

#endif /* __MED_IMG_MOD__ */

#endif /* MED_NOT_PRESENT */

