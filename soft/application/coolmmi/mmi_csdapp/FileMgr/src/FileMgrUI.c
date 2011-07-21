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
 *	 FileMgrUI.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  File Manager.
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
 *
 *
 *
 *
 *
 *
 *
 *
 ********************************************************************************/

/****************************************************************************
* Include Files                                                                
*****************************************************************************/
#ifndef MMI_ON_HARDWARE_P
#undef __DRM_SUPPORT__
#undef __DRM_V01__
#endif

#define __NEWSIMULATOR 
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"

#include "timerevents.h"
#include "commonscreens.h"
#include "phonesetupgprots.h"

#include "gdi_include.h"		/* for gdi_assert */
#include "lcd_sw_rnd.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "fat_fs.h"
#include "conversions.h"

#include "filemgr.h"
#include "debuginitdef.h"

#ifdef __DRM_SUPPORT__
#include "drm_gprot.h"
#include "drm_main.h"
#endif /*__DRM_SUPPORT__*/

#include "filemanagerdef.h"
#include "filemgrstruct.h"
#include "audioplayerdef.h"

#ifdef __MMI_EBOOK_READER__
#include "ebookgprot.h"
#include "ebookprot.h"
#include "ebookmanager.h"
#endif /* __MMI_EBOOK_READER__ */ 
#include "callmanagementstruct.h"
#include "callstructuremanagementprot.h"
/*UZC Add Start For MMIFS Ver:    on 2007-1-22 12:26 */
#include "mmi_trace.h"
/*WUZC Add End  For MMIFS Ver:    on 2007-1-22 12:26 */
#undef __NEWSIMULATOR

#ifdef __MMI_FILE_MANAGER__

/***************************************************************************** 
* Define
*****************************************************************************/
#ifdef __MMI_TOUCH_SCREEN__
#define FMGR_UI_DELAY_DECODE_DURATION 	(1000)
#else
#define FMGR_UI_DELAY_DECODE_DURATION 	(100)
#endif
#define FMGR_UI_IMAGE_BOX_SPACING		(1)/*(2)*/
#define FMGR_UI_TEXT_SPACING				(5)

/***************************************************************************** 
* Typedef 
*****************************************************************************/
typedef enum {
	FMGR_UI_LIST_STYLE,
	FMGR_UI_INFO_STYLE,
	FMGR_UI_MATRIX_STYLE
} fmgr_ui_style_enum;


/*****************************************************************************
* Global Variable - Extern                                                                 
*****************************************************************************/
extern fmgr_context_struct    g_fmgr_context;
extern fmgr_context_struct    *fmgr_p;
extern BOOL 						r2lMMIFlag;
extern MMI_theme					*current_MMI_theme;

/*****************************************************************************
* Global Function                                                           
*****************************************************************************/

/***************************************************************************** 
* Local Variable
*****************************************************************************/
#if defined(FMGR_USING_CACHE)

	/* this buffer should allocate dynamically */
	static U8   fm_thumb_image_cache_buffer[FMGR_THUMB_IMAGE_BUFFER_SIZE*1024]; /* tmp 30k */

	/* cache structure */
	fmgr_thumb_image_cache_struct		image_cache_list_buffer[FMGR_THUMB_IMAGE_CACHE_MAX_COUNT];
	fmgr_thumb_image_handle_struct	image_handle_list_buffer[FMGR_THUMB_IMAGE_HANDLE_MAX_COUNT];

	fmgr_image_cache_struct				*fmgr_image_cache_p = &g_fmgr_context.image_cache;

#endif /* FMGR_USING_CACHE */


/*****************************************************************************
* Local Function 
*****************************************************************************/
static void mmi_fmgr_draw_free_space_icon(S32 box_x1, S32 box_y1, S32 box_height, U32 ratio);

#if defined(FMGR_USING_CACHE)
	static BOOL mmi_fmgr_show_thumb_image(	S32 item_idx, 
														S32 menuitem_offset_x, 
														S32 menuitem_offset_y, 
														S32 box_offset_x, 
														S32 box_offset_y,
														S32 box_width, 
														S32 box_height,
														UI_image_type icon_ptr,
														BOOL is_hilighted,
														U16 ui_style);


	static void mmi_fmgr_decode_thumb_image_reulst_hdlr(GDI_RESULT decode_result, gdi_handle handle);

	static void mmi_fmgr_delay_decode_image(	S32 item_idx, 
												 			S32 menuitem_offset_x, 
															S32 menuitem_offset_y, 
															S32 box_offset_x, 
															S32 box_offset_y,
															S32 box_width, 
															S32 box_height,
															S32 icon_offset_x, 
															S32 icon_offset_y,
															S32 icon_width,
															S32 icon_height,
															UI_image_type icon_ptr,
															BOOL is_hilighted,
															U16 ui_style);
	
	static void mmi_fmgr_delay_decode_0(void);
	static void mmi_fmgr_delay_decode_1(void);
	static void mmi_fmgr_delay_decode_2(void);
	static void mmi_fmgr_delay_decode_3(void);
	static void mmi_fmgr_delay_decode_4(void);
	static void mmi_fmgr_delay_decode_5(void);
	static void mmi_fmgr_delay_decode_6(void);
	static void mmi_fmgr_delay_decode_7(void);	
	static void mmi_fmgr_delay_decode_8(void);	
	static void mmi_fmgr_delay_decode_9(void);	
#endif /* FMGR_USING_CACHE */


													 





/*****************************************************************************
*
* Thumb Image Caching 
*
*****************************************************************************/

/*****************************************************************************
*
*	[fm_thumb_image_handle_struct] is used to record which image is send to decoder.
*  These information is used when decoder successfully decode the image and used
*  to calcuate which region should be cached.
*
*	BOOL			is_used;			<------ this entry is used or not
*	gdi_handle	handle;			<------ gdi_handle of this image given by decoder
*	S16			menuitem_idx;	<------ menu item index
*	S32			offset_x;		<------ image offset x
*	S32			offset_y;		<------ image offset y
*
*	If decoded image is larger than thumb image size, it will be shrink to fit
*  the thumb image size and persist its ratio (will not distort).
*
*****************************************************************************/


/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_create_thumb_image_cache()
* DESCRIPTION
*   create thumb image cache resource.
* PARAMETERS
*	 image_size	 IN	 menuitem height, this is used to caculate image resource.
* RETURNS
*   BOOL
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_fmgr_create_thumb_image_cache(S32 image_size)
{
#if defined(FMGR_USING_CACHE)
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U32	cache_size;
	S32	i;
 
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* calcuate thumb image size */
	fmgr_image_cache_p->image_size = image_size;
	cache_size = (fmgr_image_cache_p->image_size*fmgr_image_cache_p->image_size*GDI_LAYER.act_bit_per_pixel)>>3;
	MMI_ASSERT(cache_size > 0);

	fmgr_image_cache_p->image_cache_item_count = (U16)(FMGR_THUMB_IMAGE_BUFFER_SIZE*1024/cache_size);

	if(fmgr_image_cache_p->image_cache_item_count > FMGR_THUMB_IMAGE_CACHE_MAX_COUNT)
		fmgr_image_cache_p->image_cache_item_count = FMGR_THUMB_IMAGE_CACHE_MAX_COUNT;

	/* init seq number */
	fmgr_image_cache_p->image_decode_seq_num = 0;

	/* TODO: allocate fm_thumb_image_cache_buffer from buffer pool */
	/* currently use static allocate memory */

	/* allocate buffer for list data */
	for(i = 0; i < fmgr_image_cache_p->image_cache_item_count ; i++)
	{
		fmgr_image_cache_p->image_cache_list[i]						= &image_cache_list_buffer[i];
		fmgr_image_cache_p->image_cache_list[i]->is_used			= FALSE;
		fmgr_image_cache_p->image_cache_list[i]->unused_duration	= 0;
		fmgr_image_cache_p->image_cache_list[i]->bmp.buf_ptr		= fm_thumb_image_cache_buffer+i*cache_size;
	}

	for(i = 0; i < FMGR_THUMB_IMAGE_HANDLE_MAX_COUNT; i++)
	{
		fmgr_image_cache_p->image_handle_list[i]				= &image_handle_list_buffer[i];
		fmgr_image_cache_p->image_handle_list[i]->is_used	= FALSE;
	}

#endif /* FMGR_USING_CACHE */
}



/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_free_thumb_image_cache()
* DESCRIPTION
*   free thumb image cache resource.
* PARAMETERS
*	 void
* RETURNS
*   BOOL
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_fmgr_free_thumb_image_cache(void)
{
#if defined(FMGR_USING_CACHE)
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/

	/* reset delay decoder */
	g_fmgr_context.delay_decode_index = 0;
	StopTimer(FMGR_DELAY_DECODE_0);
	StopTimer(FMGR_DELAY_DECODE_1);
	StopTimer(FMGR_DELAY_DECODE_2);
	StopTimer(FMGR_DELAY_DECODE_3);	
	StopTimer(FMGR_DELAY_DECODE_4);
	StopTimer(FMGR_DELAY_DECODE_5);
	StopTimer(FMGR_DELAY_DECODE_6);
	StopTimer(FMGR_DELAY_DECODE_7);
	StopTimer(FMGR_DELAY_DECODE_8);
	StopTimer(FMGR_DELAY_DECODE_9);	
	
#endif /* FMGR_USING_CACHE */
}



/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_delay_decode_x()
* DESCRIPTION
*   delay decode callback function for timer expire
* PARAMETERS
*	 void
* RETURNS
*   BOOL
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(FMGR_USING_CACHE)

	/* start of delay decoder function macro */
	#define FMGR_DELAY_DECODE_FUNCTION(_idx_)\
		static void mmi_fmgr_delay_decode_##_idx_##(void)\
		{\
			mmi_fmgr_delay_decode_image(	g_fmgr_context.delay_decoder[##_idx_##].item_idx,\
													g_fmgr_context.delay_decoder[##_idx_##].menuitem_offset_x,\
													g_fmgr_context.delay_decoder[##_idx_##].menuitem_offset_y,\
													g_fmgr_context.delay_decoder[##_idx_##].box_offset_x,\
													g_fmgr_context.delay_decoder[##_idx_##].box_offset_y,\
													g_fmgr_context.delay_decoder[##_idx_##].box_width,\
													g_fmgr_context.delay_decoder[##_idx_##].box_height,\
													g_fmgr_context.delay_decoder[##_idx_##].icon_offset_x,\
													g_fmgr_context.delay_decoder[##_idx_##].icon_offset_y,\
													g_fmgr_context.delay_decoder[##_idx_##].icon_width,\
													g_fmgr_context.delay_decoder[##_idx_##].icon_height,\
													g_fmgr_context.delay_decoder[##_idx_##].icon_ptr,\
													g_fmgr_context.delay_decoder[##_idx_##].is_hilighted,\
													g_fmgr_context.delay_decoder[##_idx_##].ui_style);\
		}
	/* end of macro */

	/* delay decode function */
	FMGR_DELAY_DECODE_FUNCTION(0)
	FMGR_DELAY_DECODE_FUNCTION(1)
	FMGR_DELAY_DECODE_FUNCTION(2)
	FMGR_DELAY_DECODE_FUNCTION(3)
	FMGR_DELAY_DECODE_FUNCTION(4)
	FMGR_DELAY_DECODE_FUNCTION(5)
	FMGR_DELAY_DECODE_FUNCTION(6)
	FMGR_DELAY_DECODE_FUNCTION(7)
	FMGR_DELAY_DECODE_FUNCTION(8)
	FMGR_DELAY_DECODE_FUNCTION(9)

#endif /* FMGR_USING_CACHE */


/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_delay_decode_image()
* DESCRIPTION
*   decode image for delay decoder, avoid UI hang too long
* PARAMETERS
*	 item_idx	 				IN	 item index
*	 menuitem_offset_x	 	IN	 menu item's offset x
*	 menuitem_offset_y	  	IN	 menu item's offset y	
*	 box_offset_x	 			IN	 image box's offset x 
*	 box_offset_y	 			IN	 image box's offset y
*	 box_width 					IN	 image box's width
*	 box_height	 				IN	 image box's height	
*	 icon_offset_x	 			IN	 icon's offset x 
*	 icon_offset_y	 			IN	 icon's offset y
*	 icon_width 				IN	 icon's width
*	 icon_height	 			IN	 icon's height	
*	 icon_ptr	 				IN	 image ptr
*	 is_hilighted	 			IN	 is highlight flag
*	 ui_style	 				IN	 UI style
* RETURNS
*   BOOL
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(FMGR_USING_CACHE)
static void mmi_fmgr_delay_decode_image(	S32 item_idx, 
											 			S32 menuitem_offset_x, 
														S32 menuitem_offset_y, 
														S32 box_offset_x, 
														S32 box_offset_y,
														S32 box_width,
														S32 box_height,
														S32 icon_offset_x, 
														S32 icon_offset_y,
														S32 icon_width,
														S32 icon_height,														
														UI_image_type icon_ptr,
														BOOL is_hilighted,
														U16 ui_style)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/	
	FMGR_FILE_INFO_STRUCT	file_info;
	S8								buffer_UCS2[FMGR_PATH_BUFFER_SIZE];
	GDI_RESULT					ret;
	S32							img_width, img_height;
	S32							resized_width, resized_height;
	S32							resized_offset_x, resized_offset_y;
	S32							handle_idx;
	S32							draw_pos_x, draw_pos_y;

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/		
	/* get file info */
	mmi_fmgr_kernel_get_file_info((U16)(item_idx), &file_info);
	
	/* check if has DRM right */
	#ifdef __DRM_SUPPORT__
		if(fmgr_check_drm_rights(&file_info, DRM_PERMISSION_DISPLAY) != FMGR_DRM_GRANT_PERMISSION)
		{
			/* do not have permission, skip */
			return;
		}
	#endif /* __DRM_SUPPORT__ */

	gdi_layer_push_clip();
	gdi_layer_reset_clip();

	/* find a vlid decode handle slot */
	for( handle_idx = 0 ; handle_idx < FMGR_THUMB_IMAGE_HANDLE_MAX_COUNT; handle_idx++)
	{
		if(fmgr_image_cache_p->image_handle_list[handle_idx]->is_used == FALSE)
			break;
	}
	
	/* valid handle chcek */
	MMI_ASSERT(handle_idx < FMGR_THUMB_IMAGE_HANDLE_MAX_COUNT);

	mmi_fmgr_kernel_get_path_name((PS8)buffer_UCS2);
	pfnUnicodeStrcat((PS8)buffer_UCS2, (PS8)file_info.file_name);

	/* get jpeg width and height */
	ret = gdi_image_get_dimension_file((PS8)buffer_UCS2, &img_width, &img_height);

	/* check if is a valid image or not */
	/* TODO: Adapt to File limit */
	if( 	(ret < 0) || 
			(img_width <= 0) || (img_height <= 0) || 	/* invalid image size */
			(file_info.file_size > 2048*1024) )			/* file too large */
	{
		fmgr_p->file_cache[item_idx-fmgr_p->cache_start_idx].is_able_display = FALSE;
		gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);
		gdi_layer_pop_clip();
		return;
	}

	/* can get dimesion from file, must be a valid jpeg file, then draw it */

	/* fit it into a bounding box */

	if(ui_style == FMGR_UI_MATRIX_STYLE)
	{
		/* reserver 2 pixel at each side to draw boundry and highlight rectangle */		
		gdi_image_util_fit_bbox(box_width-4, box_height-4,
										img_width, img_height,
										&resized_offset_x, &resized_offset_y,
										&resized_width, &resized_height);
		
		draw_pos_x = box_offset_x+2+resized_offset_x;
		draw_pos_y = box_offset_y+2+resized_offset_y;		
	}
	else
	{
		/* reserver 1 pixel at each side to draw boundry */				
		gdi_image_util_fit_bbox(box_width-2, box_height-2,
										img_width, img_height,
										&resized_offset_x, &resized_offset_y,
										&resized_width, &resized_height);

		draw_pos_x = box_offset_x+1+resized_offset_x;
		draw_pos_y = box_offset_y+1+resized_offset_y;		
	}



	#ifndef MMI_ON_WIN32
		ret = gdi_image_decoder_decode_file(	draw_pos_x,
															draw_pos_y,
															resized_width,
															resized_height,
															FALSE,
															(PS8)buffer_UCS2,
															&(fmgr_image_cache_p->image_handle_list[handle_idx]->handle));
	#endif /* MMI_ON_WIN32 */
	
	if(ret >= 0)
	{
		fmgr_image_cache_p->image_handle_list[handle_idx]->is_used					= TRUE;
		fmgr_image_cache_p->image_handle_list[handle_idx]->draw_pos_x				= draw_pos_x;
		fmgr_image_cache_p->image_handle_list[handle_idx]->draw_pos_y				= draw_pos_y;
		fmgr_image_cache_p->image_handle_list[handle_idx]->resized_offset_x		= resized_offset_x;
		fmgr_image_cache_p->image_handle_list[handle_idx]->resized_offset_y		= resized_offset_y;
		fmgr_image_cache_p->image_handle_list[handle_idx]->resized_width			= resized_width;
		fmgr_image_cache_p->image_handle_list[handle_idx]->resized_height			= resized_height;
		fmgr_image_cache_p->image_handle_list[handle_idx]->box_offset_x			= box_offset_x;
		fmgr_image_cache_p->image_handle_list[handle_idx]->box_offset_y			= box_offset_y;		
		fmgr_image_cache_p->image_handle_list[handle_idx]->box_width				= box_width;
		fmgr_image_cache_p->image_handle_list[handle_idx]->box_height				= box_height;
		fmgr_image_cache_p->image_handle_list[handle_idx]->icon_offset_x			= icon_offset_x;
		fmgr_image_cache_p->image_handle_list[handle_idx]->icon_offset_y			= icon_offset_y;
		fmgr_image_cache_p->image_handle_list[handle_idx]->icon_width				= icon_width;
		fmgr_image_cache_p->image_handle_list[handle_idx]->icon_height				= icon_height;		
		fmgr_image_cache_p->image_handle_list[handle_idx]->icon_ptr					= icon_ptr;
		fmgr_image_cache_p->image_handle_list[handle_idx]->menuitem_idx			= (U16)item_idx;
		fmgr_image_cache_p->image_handle_list[handle_idx]->is_hilighted			= is_hilighted;
		fmgr_image_cache_p->image_handle_list[handle_idx]->ui_style					= ui_style;
	}
	else
	{
		fmgr_p->file_cache[item_idx-fmgr_p->cache_start_idx].is_able_display = FALSE;
		gdi_layer_blt_previous(0,0, UI_device_width-1, UI_device_height-1);
	}

	gdi_layer_pop_clip();
	return;

}
#endif /* FMGR_USING_CACHE */



/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_show_thumb_image()
* DESCRIPTION
*   show thumb image 
* PARAMETERS
*	 item_idx	 				IN	 item index
*	 menuitem_offset_x	 	IN	 menu item's offset x
*	 menuitem_offset_y	  	IN	 menu item's offset y	
*	 box_offset_x	 			IN	 image box offset x 
*	 box_offset_y	 			IN	 image box offset y
*	 box_width	 				IN	 image box width 
*	 box_height	 				IN	 image box height
*	 icon_ptr	 				IN	 icon's image ptr
*	 is_hilighted	 			IN	 is highlight flag
* RETURNS
*   BOOL
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(FMGR_USING_CACHE)
static BOOL mmi_fmgr_show_thumb_image(	S32 item_idx, 
													S32 menuitem_offset_x,
													S32 menuitem_offset_y,
													S32 box_offset_x, 
													S32 box_offset_y, 
													S32 box_width, 
													S32 box_height,
													UI_image_type icon_ptr,
													BOOL is_hilighted,
													U16  ui_style)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/	
	FMGR_FILE_INFO_STRUCT	file_info;
	BOOL							is_image_in_cache;
	S32							icon_offset_x, icon_offset_y;
	S32							icon_width, icon_height;	
	S32							cache_index;
	S32							draw_pos_x, draw_pos_y;
	S32							i;

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/		

   #ifndef MMI_ON_WIN32
  		/* create jpeg decoder */
   	if(!fmgr_p->is_decoder_create)
   	{
   		 
			gdi_image_decoder_create(100, 100, mmi_fmgr_decode_thumb_image_reulst_hdlr);
   		fmgr_p->is_decoder_create = TRUE;
   	}
   #endif

	gdi_layer_push_clip();
	gdi_layer_reset_clip();

	gdi_image_get_dimension((PU8)icon_ptr, &icon_width, &icon_height);
	
	icon_offset_x = ((box_width-icon_width)>>1)+menuitem_offset_x+box_offset_x;
	icon_offset_y = ((box_height-icon_height)>>1)+menuitem_offset_y+box_offset_y;

	box_offset_x += menuitem_offset_x;
	box_offset_y += menuitem_offset_y;

	/* if is corrupted file, show as icon */
	if(fmgr_p->file_cache[item_idx-fmgr_p->cache_start_idx].is_able_display == FALSE)
	{
		if(is_hilighted)
		{
			gdi_image_draw(icon_offset_x, icon_offset_y, icon_ptr);
		}
		else
		{
			gdi_image_draw(icon_offset_x, icon_offset_y, icon_ptr);
		}
		
		/* corrupted file */
		gdi_layer_pop_clip();
		return FALSE;
	}

	/* get file info */
	mmi_fmgr_kernel_get_file_info((U16)(item_idx), &file_info);

	/* test if is in the cache */
	is_image_in_cache = FALSE;

	for(cache_index = 0 ; cache_index < fmgr_image_cache_p->image_cache_item_count; cache_index++)
	{
		if((fmgr_image_cache_p->image_cache_list[cache_index]->is_used == TRUE) &&
			(fmgr_image_cache_p->image_cache_list[cache_index]->menuitem_idx == item_idx))
		{
			is_image_in_cache = TRUE;
			break;
		}
	}

	/* if can find image in cache, draw it from cache */
	if(is_image_in_cache == TRUE)
	{
		if(ui_style == FMGR_UI_MATRIX_STYLE)
		{
			draw_pos_x = box_offset_x+fmgr_image_cache_p->image_cache_list[cache_index]->resized_offset_x+2;
			draw_pos_y = box_offset_y+fmgr_image_cache_p->image_cache_list[cache_index]->resized_offset_y+2;

			/* draw frame */
			gdi_draw_rect(	draw_pos_x-1,
								draw_pos_y-1,
								draw_pos_x+fmgr_image_cache_p->image_cache_list[cache_index]->resized_width,
								draw_pos_y+fmgr_image_cache_p->image_cache_list[cache_index]->resized_height,
								GDI_COLOR_BLACK);

			gdi_image_cache_bmp_draw(	draw_pos_x,
												draw_pos_y,
												&fmgr_image_cache_p->image_cache_list[cache_index]->bmp);	
			
		}
		else
		{
			draw_pos_x = box_offset_x+fmgr_image_cache_p->image_cache_list[cache_index]->resized_offset_x+1;
			draw_pos_y = box_offset_y+fmgr_image_cache_p->image_cache_list[cache_index]->resized_offset_y+1;

			/* draw frame */
			gdi_draw_rect(	draw_pos_x-1,
								draw_pos_y-1,
								draw_pos_x+fmgr_image_cache_p->image_cache_list[cache_index]->resized_width,
								draw_pos_y+fmgr_image_cache_p->image_cache_list[cache_index]->resized_height,
								GDI_COLOR_BLACK);

			gdi_image_cache_bmp_draw(	draw_pos_x,
												draw_pos_y,
												&fmgr_image_cache_p->image_cache_list[cache_index]->bmp);			
		}

		/* make other unused duration increase one */
		for (i = 0; i < fmgr_image_cache_p->image_cache_item_count; i++)
		{
			fmgr_image_cache_p->image_cache_list[i]->unused_duration++;
		}

		fmgr_image_cache_p->image_cache_list[cache_index]->unused_duration = 0;

	}
	else
	{
		/* not in cache */
		if(is_hilighted)
		{
			gdi_image_draw(icon_offset_x, icon_offset_y, icon_ptr);
		}
		else
		{
			gdi_image_draw(icon_offset_x, icon_offset_y,	icon_ptr);
		}

		/* check file name too long, exceed 260 char */
		if((pfnUnicodeStrlen((PS8)fmgr_p->file_path)+pfnUnicodeStrlen((PS8)file_info.file_name))>=260)
		{
			/* filename too long */
			gdi_layer_pop_clip();
			return FALSE;	
		}

		/* Start of Marco for switch case */
		#define FMGR_DELAY_DECODER_SWITCH(_idx_)\
			case _idx_:\
			{\
				StartTimer(FMGR_DELAY_DECODE_##_idx_, FMGR_UI_DELAY_DECODE_DURATION, mmi_fmgr_delay_decode_##_idx_##);\
				g_fmgr_context.delay_decoder[##_idx_##].item_idx 				= item_idx;\
				g_fmgr_context.delay_decoder[##_idx_##].menuitem_offset_x 	= menuitem_offset_x;\
				g_fmgr_context.delay_decoder[##_idx_##].menuitem_offset_y 	= menuitem_offset_y;\
				g_fmgr_context.delay_decoder[##_idx_##].box_offset_x 			= box_offset_x;\
				g_fmgr_context.delay_decoder[##_idx_##].box_offset_y 			= box_offset_y;\
				g_fmgr_context.delay_decoder[##_idx_##].box_width 				= box_width;\
				g_fmgr_context.delay_decoder[##_idx_##].box_height 			= box_height;\
				g_fmgr_context.delay_decoder[##_idx_##].icon_offset_x 		= icon_offset_x;\
				g_fmgr_context.delay_decoder[##_idx_##].icon_offset_y 		= icon_offset_y;\
				g_fmgr_context.delay_decoder[##_idx_##].icon_width 			= icon_width;\
				g_fmgr_context.delay_decoder[##_idx_##].icon_height 			= icon_height;\
				g_fmgr_context.delay_decoder[##_idx_##].icon_ptr 				= icon_ptr;\
				g_fmgr_context.delay_decoder[##_idx_##].is_hilighted 			= is_hilighted;\
				g_fmgr_context.delay_decoder[##_idx_##].ui_style 				= ui_style;\
				break;\
			}
		/* end of Macro */

		/* start delay decode timer */
		switch(g_fmgr_context.delay_decode_index)
		{
			FMGR_DELAY_DECODER_SWITCH(0);
			FMGR_DELAY_DECODER_SWITCH(1);
			FMGR_DELAY_DECODER_SWITCH(2);
			FMGR_DELAY_DECODER_SWITCH(3);
			FMGR_DELAY_DECODER_SWITCH(4);
			FMGR_DELAY_DECODER_SWITCH(5);
			FMGR_DELAY_DECODER_SWITCH(6);
			FMGR_DELAY_DECODER_SWITCH(7);
			FMGR_DELAY_DECODER_SWITCH(8);
			FMGR_DELAY_DECODER_SWITCH(9);			
			default:
				MMI_ASSERT(0);
		}
		
		g_fmgr_context.delay_decode_index++;

	}

	coolsand_UI_pop_clip();

	return TRUE;
}
#endif /* FMGR_USING_CACHE */



/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_decode_thumb_image_reulst_hdlr()
* DESCRIPTION
*   decode image result handler
* PARAMETERS
*	 decode_result	 	IN	 return cause
*	 handle				IN	 image's handle
* RETURNS
*   BOOL
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(FMGR_USING_CACHE)
static void mmi_fmgr_decode_thumb_image_reulst_hdlr(GDI_RESULT decode_result, gdi_handle handle)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	S32  			i, j;
	S32			cache_entry_idx;
	S32			max_unused_duration = 0;
	S32			unused_duration;
	S32			min_seq_number;
	BOOL			has_cache_entry;
	gdi_color	bg_color;
	color_t 		*color_ptr;
	S32 			image_x1, image_y1, image_x2, image_y2;
	S32 			clear_x1, clear_y1, clear_x2, clear_y2;	/* previous region need to clear */
	S32 			blt_x1, blt_y1, blt_x2, blt_y2;

	fmgr_thumb_image_handle_struct *handle_ptr;

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/	

	coolsand_UI_push_clip();
	coolsand_UI_reset_clip();

	if(decode_result != GDI_JPEG_SUCCEED)
	{
		for(i = 0 ; i < FMGR_THUMB_IMAGE_HANDLE_MAX_COUNT; i++)
		{		
			if((fmgr_image_cache_p->image_handle_list[i]->is_used == TRUE) &&
				(fmgr_image_cache_p->image_handle_list[i]->handle == handle))
			{
				/* decode end */
				handle_ptr = fmgr_image_cache_p->image_handle_list[i];
			
				fmgr_p->file_cache[handle_ptr->menuitem_idx-fmgr_p->cache_start_idx].is_able_display = FALSE;
				fmgr_image_cache_p->image_handle_list[i]->is_used	= FALSE;
				break; /* break the loop */
			}
		}
	}
	else
	{
		for(i = 0 ; i < FMGR_THUMB_IMAGE_HANDLE_MAX_COUNT; i++)
		{
			if((fmgr_image_cache_p->image_handle_list[i]->is_used == TRUE) &&
				(fmgr_image_cache_p->image_handle_list[i]->handle == handle))
			{
				/* 
					decode success,
					(1) Stop animated icon
					(1) Wipe out icon by backgroup color.
					(2) Draw photo frame.
					(3) Find an oldest image cache, and cache the image. 
				*/

				handle_ptr = fmgr_image_cache_p->image_handle_list[i];
				
				image_x1 = handle_ptr->draw_pos_x;
				image_y1 = handle_ptr->draw_pos_y;
				image_x2 = handle_ptr->draw_pos_x+handle_ptr->resized_width-1;
				image_y2 = handle_ptr->draw_pos_y+handle_ptr->resized_height-1;

				/* clear icon by bg color */
				/* since photo is already draw, only draw region not coverd by photo */
				if((handle_ptr->ui_style == FMGR_UI_LIST_STYLE) ||
					(handle_ptr->ui_style == FMGR_UI_INFO_STYLE))
				{
					clear_x1 = handle_ptr->icon_offset_x;
					clear_y1 = handle_ptr->icon_offset_y;
					clear_x2 = handle_ptr->icon_offset_x+handle_ptr->icon_width-1;
					clear_y2 = handle_ptr->icon_offset_y+handle_ptr->icon_height-1;
				}
				else /* matrix */
				{
					clear_x1 = handle_ptr->box_offset_x;
					clear_y1 = handle_ptr->box_offset_y;
					clear_x2 = handle_ptr->box_offset_x+handle_ptr->box_width-1;
					clear_y2 = handle_ptr->box_offset_y+handle_ptr->box_height-1;
				}

				/* stop animated icon */
//				if(handle_ptr->is_hilighted)
//				{
//					gdi_image_stop_animation_all();
//				}

				/* clear icon by bg color */
				/* since photo is already draw, only draw region not coverd by photo */
				if(handle_ptr->ui_style == FMGR_UI_LIST_STYLE)
				{
					/* get color from theme */
					if(handle_ptr->is_hilighted)
					{
						color_ptr = &(current_MMI_theme->list_selected_item_filler->c);
						bg_color = GDI_RGB_TO_BUFFER_FORMAT(color_ptr->r, color_ptr->g, color_ptr->b);
					}
					else
					{
					#ifdef __MMI_UI_STYLE_4__
						bg_color = GDI_COLOR_TRANSPARENT;
					#else /* __MMI_MAINLCD_240X320__ */
						color_ptr = &(current_MMI_theme->list_background_filler->c);
						bg_color = GDI_RGB_TO_BUFFER_FORMAT(color_ptr->r, color_ptr->g, color_ptr->b);
					#endif /* __MMI_MAINLCD_240X320__ */
					}
				}
				else if(handle_ptr->ui_style == FMGR_UI_INFO_STYLE)
				{
					/* this bg color is the same in wgui_categories_fmgr.c's RedrawCategory212Screen() */
					bg_color = GDI_RGB_TO_BUFFER_FORMAT(255, 255, 255);
				}
				else if(handle_ptr->ui_style == FMGR_UI_MATRIX_STYLE)
				{
					if(handle_ptr->is_hilighted)
					{
						color_ptr = &(current_MMI_theme->list_selected_item_filler->c);
						bg_color = GDI_RGB_TO_BUFFER_FORMAT(color_ptr->r, color_ptr->g, color_ptr->b);
					}
					else
					{
						color_ptr = &(current_MMI_theme->list_background_filler->c);
						bg_color = GDI_RGB_TO_BUFFER_FORMAT(color_ptr->r, color_ptr->g, color_ptr->b);
					}
				}				
				else
				{
					MMI_ASSERT(0);
				}

				/* left region */
				if(clear_x1 < image_x1-1)
				{
					gdi_draw_solid_rect(clear_x1, clear_y1, image_x1-1, clear_y2, bg_color);
				}

				/* right region */
				if(clear_x2 > image_x2+1)
				{
					gdi_draw_solid_rect(image_x2+1, clear_y1, clear_x2, clear_y2, bg_color);
				}

				/* up region */
				if(clear_y1 < image_y1-1)
				{
					gdi_draw_solid_rect(clear_x1, clear_y1, clear_x2, image_y1-1, bg_color);
				}			

				/* down region */
				if(clear_y2 > image_y2+1)
				{
					gdi_draw_solid_rect(clear_x1, image_y2+1, clear_x2, clear_y2, bg_color);
				}		
				
				/* draw frame */
				gdi_draw_rect(	image_x1-1, image_y1-1,	image_x2+1, image_y2+1, GDI_COLOR_BLACK);


				if(handle_ptr->ui_style == FMGR_UI_MATRIX_STYLE)
				{
					/* 2 pixels . 1 for border, 1 for spacing */
					blt_x1 = (image_x1-2>clear_x1)?clear_x1:image_x1-2;
					blt_y1 = (image_y1-2>clear_y1)?clear_y1:image_y1-2;
					blt_x2 = (image_x2+2>clear_x2)?image_x2+2:clear_x2;
					blt_y2 = (image_y2+2>clear_y2)?image_y2+2:clear_y2;					
				}	
				else
				{
					 
					blt_x1 = (image_x1-1>clear_x1)?clear_x1:image_x1-1;
					blt_y1 = (image_y1-1>clear_y1)?clear_y1:image_y1-1;
					blt_x2 = (image_x2+1>clear_x2)?image_x2+1:clear_x2;
					blt_y2 = (image_y2+1>clear_y2)?image_y2+1:clear_y2;
				}


				gdi_layer_blt_previous(blt_x1, blt_y1, blt_x2, blt_y2);

				/* check if has valid cache index */
				has_cache_entry = FALSE;
				for (j = 0; j < fmgr_image_cache_p->image_cache_item_count; j++)
				{
					if(fmgr_image_cache_p->image_cache_list[j]->is_used == FALSE)
					{
						has_cache_entry = TRUE;
						cache_entry_idx = j;
						break;
					}
				}

				/* if not in cache, we will replace the oldest one in cache */
				if(!has_cache_entry)
				{
					 
					max_unused_duration = 0;
					for(j = 0 ; j < fmgr_image_cache_p->image_cache_item_count; j++)
					{
						if(fmgr_image_cache_p->image_cache_list[j]->is_used == TRUE)
						{
							unused_duration	= fmgr_image_cache_p->image_cache_list[j]->unused_duration;
							max_unused_duration = (max_unused_duration > unused_duration) ? max_unused_duration : unused_duration;
						}
					}

					min_seq_number = 65535;
					 
					for(j = 0 ; j < fmgr_image_cache_p->image_cache_item_count; j++)
					{
						if((fmgr_image_cache_p->image_cache_list[j]->is_used == TRUE) &&
							(fmgr_image_cache_p->image_cache_list[j]->unused_duration == max_unused_duration))
						{
							/* find the one with smallest seq number, which means is the oldest */
							if(fmgr_image_cache_p->image_cache_list[j]->seq_number < min_seq_number)
							{
								cache_entry_idx	= j;
								min_seq_number		= fmgr_image_cache_p->image_cache_list[j]->seq_number;
							}
						}
					}
				}

				/* increase decode seq number */
				if(fmgr_image_cache_p->image_decode_seq_num >= 65534)
					fmgr_image_cache_p->image_decode_seq_num = 0;
				else
					fmgr_image_cache_p->image_decode_seq_num++;


				gdi_image_cache_bmp_get(handle_ptr->draw_pos_x,
												handle_ptr->draw_pos_y,
												handle_ptr->draw_pos_x+handle_ptr->resized_width-1,
												handle_ptr->draw_pos_y+handle_ptr->resized_height-1,
												&fmgr_image_cache_p->image_cache_list[cache_entry_idx]->bmp);

				fmgr_image_cache_p->image_cache_list[cache_entry_idx]->is_used				= TRUE;
				fmgr_image_cache_p->image_cache_list[cache_entry_idx]->unused_duration	= 0;
				fmgr_image_cache_p->image_cache_list[cache_entry_idx]->menuitem_idx		= handle_ptr->menuitem_idx;
				fmgr_image_cache_p->image_cache_list[cache_entry_idx]->resized_offset_x	= handle_ptr->resized_offset_x;
				fmgr_image_cache_p->image_cache_list[cache_entry_idx]->resized_offset_y	= handle_ptr->resized_offset_y;
				fmgr_image_cache_p->image_cache_list[cache_entry_idx]->resized_width		= handle_ptr->resized_width;
				fmgr_image_cache_p->image_cache_list[cache_entry_idx]->resized_height	= handle_ptr->resized_height;
				fmgr_image_cache_p->image_cache_list[cache_entry_idx]->seq_number			= fmgr_image_cache_p->image_decode_seq_num;

				fmgr_image_cache_p->image_handle_list[i]->is_used	= FALSE;

				break; /* break the for loop */
			} /* if */
		} /* for */
	} /* if */

	coolsand_UI_pop_clip();

}
#endif /* FMGR_USING_CACHE */







/*****************************************************************************
*
* File Manager [UI Drawing]
*
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_draw_free_space_icon()
* DESCRIPTION
*   draw free space icon
* PARAMETERS
*	 box_x1	 		IN	 bounding box offset x
*	 box_y1			IN	 bounding box offset y 
*	 box_height		IN	 bounding box's height
*	 ratio			IN	 free space %
* RETURNS
*   void 
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_fmgr_draw_free_space_icon(S32 box_x1, S32 box_y1, S32 box_height, U32 ratio)
{
#if defined(__MMI_SLIM_FILE_MANAGER__)	
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	S32				x_spacing;
	S32				y_spacing;
	S32				offset_x;
	S32 				offset_y;
	S32				width;
	S32 				height;
	gdi_color 		green_start_color;
	gdi_color		green_end_color;
	gdi_color 		red_start_color;
	gdi_color		red_end_color;	
	S32				green_width;
	S32				red_width;

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	x_spacing 	= 4;
	y_spacing 	= 8;
	offset_x 	= box_x1+x_spacing;
	offset_y 	= box_y1+y_spacing;
	width 		= box_height;
	height 		= box_height-2*y_spacing;
	
	green_width = width*ratio/10;
	red_width	= width-green_width;
	
	green_start_color = GDI_RGB_TO_BUFFER_FORMAT(30, 255, 30);
	green_end_color 	= GDI_RGB_TO_BUFFER_FORMAT(200, 255, 200);

	red_start_color 	= GDI_RGB_TO_BUFFER_FORMAT(255, 30, 30);
	red_end_color 		= GDI_RGB_TO_BUFFER_FORMAT(255, 200, 200);	
	
 	gdi_draw_gradient_rect(	offset_x, 
 									offset_y, 
 									offset_x+red_width, 
 									offset_y+height,
 									red_start_color, 
 									red_end_color, 
 									GDI_COLOR_BLACK,
 									1,
 									GDI_GRADIENT_RECT_VER);
 	
 	gdi_draw_gradient_rect(	offset_x+red_width, 
 									offset_y, 
 									offset_x+width, 
 									offset_y+height,
 									green_start_color, 
 									green_end_color, 
 									GDI_COLOR_BLACK,
 									1,
 									GDI_GRADIENT_RECT_VER);
 	
 #else 
	UI_image_type	icon_ptr;
	S32				img_width, img_height;
	S32				spacing;
	
	icon_ptr = (UI_image_type)GetImage((U16)(IMG_ID_FMGR_DRIVE_SPACE_0+ratio));
	coolsand_UI_measure_image(icon_ptr, &img_width, &img_height);
	spacing = (box_height - img_height) >> 1;
	
	if(r2lMMIFlag)
		coolsand_UI_show_transparent_image(box_x1, box_y1+spacing+1, icon_ptr, 0);
	else
		coolsand_UI_show_transparent_image(box_x1+spacing, box_y1+spacing+1, icon_ptr, 0);
#endif 
}





/******************************************************************************
* Drv UI related callback
******************************************************************************/

/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_get_drv_item_callback()
* DESCRIPTION
*   callback function for getting dynamic list item
* PARAMETERS
*	 item_idx	 		IN	 	index
*	 str_buff			OUT	string buffer
*	 img_buff_p			OUT	image buffer ptr
*	 str_img_mask		IN	   mask
* RETURNS
*   void 
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
pBOOL mmi_fmgr_get_drv_item_callback(S32 item_idx, UI_string_type str_buff, UI_image_type *img_buff_p, U8 str_img_mask)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/	
   S16 	i;
	U16	drv_str_id = 0; 

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/ 
	*img_buff_p = NULL;

	/* TODO: FS_GetDrive */
	if (item_idx > fmgr_p->drv_count)
	   return FALSE;

   i = fmgr_get_drive_index(item_idx);

	if (i < 0)  /* device not exist */
	   return FALSE;

	switch(i)
   {
	/*WUZC Del Start For MMIFS Ver:    on 2007-1-22 12:19 */
	#if 0
      case FMGR_NAND_STORAGE:
      	drv_str_id = STR_FMGR_NAND_FLASH;
      	break;
	#endif
	/*WUZC Del End  For MMIFS Ver:    on 2007-1-22 12:19 */
      case FMGR_NOR_STORAGE:
	/*Wuzc Modify Start For FMGR Ver:    on 2007-2-2 18:29 */
	//   drv_str_id = STR_FMGR_NAND_FLASH;
	      drv_str_id = STR_FMGR_NOR_FLASH;
	/*Wuzc Modify End  For FMGR Ver:    on 2007-2-2 18:29 */
         break;
#ifdef __DRM_V01__
      case FMGR_DRM_STORAGE:
         drv_str_id = STR_FMGR_DRM_DATA;
         break;
#endif
#ifdef __DRM_SUPPORT__         
      case FMGR_DRM_RO_STORAGE:
         drv_str_id = STR_FMGR_DRM_RIGHTS;
         break;
#endif         
      case FMGR_REMOVABLE_STORAGE:
         drv_str_id = STR_FMGR_REM_DEVICE;
         break;
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
    case FMGR_STORAGE_TFCARD_SECOND:
         drv_str_id = STR_FMGR_SEC_REM_DEVICE;
         break;
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
      default:
         MMI_ASSERT(0);
         break;
   }

	pfnUnicodeStrcpy((PS8)str_buff, (PS8)(GetString(drv_str_id)));
 /*+ zhouqin modify for dual t-flash card 20110503*/   
#if defined(DUAL_TFLASH_SUPPORT)
    if (i == FMGR_STORAGE_TFCARD_SECOND)
    {
        if(fmgr_is_sec_msdc_present() == TRUE)
        {
#ifdef __MMI_NUMBER_MENU__
		*img_buff_p = (UI_image_type)GetImage(IMG_GLOBAL_L1+item_idx);
#else
		*img_buff_p = (UI_image_type)GetImage(IMG_ID_FMGR_DRIVE_MEM_CARD2);
#endif
        }
        else
	        return FALSE;
    }
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
/*WUZC MOD Start For MMIFS Ver:    on 2007-1-22 14:7 */
	if(
		#if 0
		i == FMGR_NAND_STORAGE || 
		#endif
/*WUZC MOD End  For MMIFS Ver:    on 2007-1-22 14:7 */
		i == FMGR_NOR_STORAGE 
		#ifdef __DRM_SUPPORT__
			|| i == FMGR_DRM_RO_STORAGE
		#endif
		)
	{
#ifdef __MMI_NUMBER_MENU__
			*img_buff_p = (UI_image_type)GetImage(IMG_GLOBAL_L1);
#else
			*img_buff_p = (UI_image_type)GetImage(IMG_ID_FMGR_DRIVE_PHONE);
#endif
	}
	else if (i == FMGR_REMOVABLE_STORAGE)
	{
		/* test if card is actually present */
		if(fmgr_is_msdc_present() == TRUE)
		{
#ifdef __MMI_NUMBER_MENU__
			*img_buff_p = (UI_image_type)GetImage(IMG_GLOBAL_L2);
#else
			*img_buff_p = (UI_image_type)GetImage(IMG_ID_FMGR_DRIVE_MEM_CARD);
#endif
		}
		else
			return FALSE;
	}

	return TRUE;
}




/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_get_drv_item_callback()
* DESCRIPTION
*   callback function for getting dynamic list hint
* PARAMETERS
*	 item_idx	 		IN	 	index
*	 hint_array			OUT	hint string
* RETURNS
*   void 
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
S32 mmi_fmgr_get_drv_hint_callback(S32 item_idx, UI_string_type *hint_array)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	return 0;
}



/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_draw_drv_info_callback()
* DESCRIPTION
*   callback function for draw seleted drv info
* PARAMETERS
*	 item_idx	 		IN	 	index
*	 x1					IN		x1 string
*	 y1	 				IN	 	y1
*	 x2					IN		x2
*	 y2					IN		y2
* RETURNS
*   void 
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_fmgr_draw_drv_info_callback(S32 item_idx, S32 x1, S32 y1, S32 x2, S32 y2)
	{
		/*----------------------------------------------------------------*/
		/* Local Variables																*/
		/*----------------------------------------------------------------*/
		S32 			char_height;
		S8					buffer[128];
		S8					buffer_UCS2[128];
		S8			   path[128];
		U8			   index;
		S32 			str_width, str_height;
		S32 			box_width, box_height;
		FS_DiskInfo 	disk_info;
		S32 			fs_ret;
		U64 			disk_free_space;
		U64 			disk_total_space;
		S32 			spacing;
		U32 			ratio;
		S32 			total_size_str_offset_y;
		S32 			free_size_str_offset_y;
		S32 			total_size_offset_y;
		S32 			free_size_offset_y; 
		S32 			left_align_offset_x;
		S32 			right_align_offset_x;
		S32 w,h;
		/*----------------------------------------------------------------*/
		/* Code Body																		*/
		/*----------------------------------------------------------------*/
		/* lock and push clip */
		coolsand_UI_lock_double_buffer();
		coolsand_UI_push_text_clip();
		coolsand_UI_push_clip();
	
		coolsand_UI_reset_clip();
		coolsand_UI_reset_text_clip();
	
		TRACE_FMGR_FUNCTION();
	   index = fmgr_get_drive_index(item_idx);
		AnsiiToUnicodeString((PS8)path, (PS8)fmgr_p->drv_list[index]);
		
		TRACE_EF(g_sw_FMGR, "index = %d %s", index, fmgr_p->drv_list[index]);
	   /*----------------------------------------------------------------*/
	   /* Get Drive info														*/
	   /*----------------------------------------------------------------*/
		fs_ret	= MMI_FS_GetDiskInfo((PU8)path, &disk_info, FS_DI_BASIC_INFO|FS_DI_FREE_SPACE);
	
		disk_free_space = 0;
		disk_total_space	= 0;
		ratio					= 0;
		
		if(fs_ret >= 0)
		{
			disk_free_space = disk_info.FreeClusters*disk_info.SectorsPerCluster*disk_info.BytesPerSector;
			disk_total_space  = disk_info.TotalClusters*disk_info.SectorsPerCluster*disk_info.BytesPerSector;
			TRACE_EF(g_sw_FMGR, "FreeClusters=ox%x TotalClusters= 0x%x",(U32)(disk_free_space), (U32)(disk_total_space));
			if(disk_info.TotalClusters > 0)
			{
				ratio	= (U32)(disk_info.FreeClusters/(disk_info.TotalClusters/100));
				ratio	+= 5;//fix bug 13159,gdm 090916
				ratio	/= 10;
				if(ratio > 10 )
					ratio = 0;
			}
		}
	
	   /*----------------------------------------------------------------*/
	   /* Calcuate text position											*/
	   /*----------------------------------------------------------------*/
		box_height			= y2-y1+1;
		box_width			= x2-x1+1;
	
		coolsand_UI_set_font(&MMI_medium_font);
		char_height 	= coolsand_UI_get_string_height((UI_string_type)GetString(STR_ID_FMGR_DRV_FREE_SPACE));
		spacing 		= (box_height-char_height*2)/3;
	
		total_size_str_offset_y = spacing+MMI_title_height+MMI_title_y+1;						/* +1 make it looks better */
		free_size_str_offset_y	= spacing*2+char_height+MMI_title_height+MMI_title_y+1; 	/* +1 make it looks better */
	
		char_height 	= coolsand_UI_get_string_height((UI_string_type)L"1");
		spacing 		= (box_height-char_height*2)/3;
	
		total_size_offset_y = spacing+MMI_title_height+MMI_title_y+1;							/* +1 make it looks better */
		free_size_offset_y	= spacing*2+char_height+MMI_title_height+MMI_title_y+1; 		/* +1 make it looks better */
	
		left_align_offset_x = x1+box_height+8;
		right_align_offset_x	= x2-box_height-8;
	
		/*----------------------------------------------------------------*/
	   /* Draw Total Size													*/
	   /*----------------------------------------------------------------*/
		coolsand_UI_reset_text_clip();
		
		coolsand_UI_set_text_color(coolsand_UI_color(0,0,0));
		if(disk_total_space > 1024*1024*10)
			sprintf(buffer, " %dM", (U16)(disk_total_space/(1024*1024)));
		else
			sprintf(buffer, " %dK",  (U16)(disk_total_space/(1024)));
	
		AnsiiToUnicodeString(buffer_UCS2, buffer);
		coolsand_UI_measure_string((UI_string_type)buffer_UCS2, &str_width, &str_height);
	
		if(r2lMMIFlag)
			coolsand_UI_move_text_cursor(x1+str_width+2, total_size_offset_y);	
		else
			coolsand_UI_move_text_cursor(x2-str_width-2, total_size_offset_y);
		coolsand_UI_print_text((UI_string_type)buffer_UCS2);
	
	
		coolsand_UI_measure_string((UI_string_type)GetString(STR_ID_FMGR_DRV_TOTAL_SPACE), &w, &h);
		
		if(w > (x2 - str_width - 2 - left_align_offset_x))
		{
			if(r2lMMIFlag)
				gui_print_truncated_text(x2 - str_width - 2,total_size_str_offset_y,x2-str_width-2-left_align_offset_x,(UI_string_type)GetString(STR_ID_FMGR_DRV_TOTAL_SPACE));
			else
				gui_print_truncated_text(left_align_offset_x,total_size_str_offset_y,x2-str_width-2-left_align_offset_x,(UI_string_type)GetString(STR_ID_FMGR_DRV_TOTAL_SPACE));
		}					
		else
		{
			if(r2lMMIFlag)
				coolsand_UI_move_text_cursor(right_align_offset_x, total_size_str_offset_y);
			else
				coolsand_UI_move_text_cursor(left_align_offset_x, total_size_str_offset_y);
			coolsand_UI_print_text((UI_string_type)GetString(STR_ID_FMGR_DRV_TOTAL_SPACE));
		}
		/*----------------------------------------------------------------*/
	   /* Draw Free Size																 */
	   /*----------------------------------------------------------------*/
	
		if(disk_free_space > 1024*1024*10)
			sprintf(buffer, " %dM", (U16)(disk_free_space/(1024*1024)));
		else
			sprintf(buffer, " %dK", (U16)(disk_free_space/(1024)));
	
		AnsiiToUnicodeString(buffer_UCS2, buffer);
		coolsand_UI_measure_string((UI_string_type)buffer_UCS2, &str_width, &str_height);
		
		if(r2lMMIFlag)
			coolsand_UI_move_text_cursor(x1+str_width+2, free_size_offset_y);
		else
			coolsand_UI_move_text_cursor(x2-str_width-2, free_size_offset_y);
		
		coolsand_UI_print_text((UI_string_type)buffer_UCS2);
	
		coolsand_UI_measure_string((UI_string_type)GetString(STR_ID_FMGR_DRV_FREE_SPACE), &w, &h);
		if(w > (x2 - str_width - 2 - left_align_offset_x))
		{
			if(r2lMMIFlag)
				gui_print_truncated_text(x2 - str_width - 2,free_size_str_offset_y,x2-str_width-2-left_align_offset_x,(UI_string_type)GetString(STR_ID_FMGR_DRV_FREE_SPACE));
			else
				gui_print_truncated_text(left_align_offset_x,free_size_str_offset_y,x2-str_width-2-left_align_offset_x,(UI_string_type)GetString(STR_ID_FMGR_DRV_FREE_SPACE));
		}
		else
		{
			if(r2lMMIFlag)
				coolsand_UI_move_text_cursor(right_align_offset_x, free_size_str_offset_y);
			else
				coolsand_UI_move_text_cursor(left_align_offset_x, free_size_str_offset_y);
	
			coolsand_UI_print_text((UI_string_type)GetString(STR_ID_FMGR_DRV_FREE_SPACE));
		}
	
		/*----------------------------------------------------------------*/
	   /* Draw Icon 																 */
	   /*----------------------------------------------------------------*/
		if(r2lMMIFlag)
			mmi_fmgr_draw_free_space_icon(x2-box_height, y1, box_height, ratio);
		else
			mmi_fmgr_draw_free_space_icon(x1, y1, box_height, ratio);
		
	
		/* pop clip , unlock and blt */
		coolsand_UI_pop_clip();
		coolsand_UI_pop_text_clip();
		coolsand_UI_unlock_double_buffer();
		coolsand_UI_BLT_double_buffer(0, 0, UI_device_width-1, UI_device_height-1);
	}




/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_hide_drv_info_callback()
* DESCRIPTION
*   callback function for stop draw seleted drv info
*	 this shall be called to stop animation, timer or non-blocking decoder
* PARAMETERS
* 	 void
* RETURNS
*   void 
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_fmgr_hide_drv_info_callback(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/

}


/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_draw_drv_menuitem_callback()
* DESCRIPTION
*   draw drv menitem callback function
* PARAMETERS
* 	 item_idx		IN		index
* 	 is_hilighted	IN		hilight flag
* 	 x1				IN		x1
* 	 y1				IN		y1
* 	 x2				IN		x2
* 	 y2				IN		y2
* RETURNS
*   void 
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_fmgr_draw_drv_menuitem_callback(S32 item_idx, BOOL is_hilighted, S32 x1, S32 y1, S32 x2, S32 y2)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/	
	FS_DiskInfo		disk_info;
	S32				fs_ret;
	U64				disk_free_space;
	U64				disk_total_space;
	U32				ratio;
	S32				left_align_offset_x;
	S32				right_align_offset_x;
	S8					buffer[128];
	S8					buffer_UCS2[128];
	S32				str_width, str_height;
	S32				small_font_height;
	S32				medium_font_height;
	S32				menuitem_height;
	S32				text_spacing;
	S32				drv_name_offset_y;
	S32				date_size_offset_y;
	color_t				text_color;
	color_t				text_shadow_color;
	S32				text_box_y1;
	S32				text_box_y2;
	S32				drv_index;
   U16 				drv_str_id = 0;

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	/* lock and push clip */
	coolsand_UI_lock_double_buffer();
	coolsand_UI_push_text_clip();
	coolsand_UI_push_clip();

	coolsand_UI_reset_clip();
	coolsand_UI_reset_text_clip();

   drv_index = fmgr_get_drive_index(item_idx);
	AnsiiToUnicodeString((PS8)buffer_UCS2, (PS8)fmgr_p->drv_list[drv_index]);
	
	MMI_ASSERT(drv_index >= 0); /* device must exist */

   /*----------------------------------------------------------------*/
   /* Get Drive info					                                    */
   /*----------------------------------------------------------------*/
	fs_ret	= MMI_FS_GetDiskInfo((PU8)buffer_UCS2, &disk_info, FS_DI_BASIC_INFO|FS_DI_FREE_SPACE);

	disk_free_space 	= 0;
	disk_total_space 	= 0;
	ratio 				= 0;
	
	if(fs_ret >= 0)
	{
		disk_free_space	= disk_info.FreeClusters*disk_info.SectorsPerCluster*disk_info.BytesPerSector;
		disk_total_space  = disk_info.TotalClusters*disk_info.SectorsPerCluster*disk_info.BytesPerSector;

		if(disk_info.TotalClusters > 0)
		{
			ratio	= (U32)(disk_info.FreeClusters/(disk_info.TotalClusters/100));
			ratio	+= 5;
			ratio	/= 10;

			if(ratio > 10 )
				ratio = 0;
		}
	}

	switch(drv_index)
   {
/*WUZC Del Start For MMIFS Ver:    on 2007-1-22 14:11 */
      #if 0
      case FMGR_NAND_STORAGE:
      	drv_str_id = STR_FMGR_NAND_FLASH;
      	break;
      #endif
/*WUZC Del End  For MMIFS Ver:    on 2007-1-22 14:11 */
      case FMGR_NOR_STORAGE:
         drv_str_id = STR_FMGR_NOR_FLASH;//change by gdm 081208
         break;
#ifdef __DRM_V01__
      case FMGR_DRM_STORAGE:
         drv_str_id = STR_FMGR_DRM_DATA;
         break;
#endif
#ifdef __DRM_SUPPORT__         
      case FMGR_DRM_RO_STORAGE:
         drv_str_id = STR_FMGR_DRM_RIGHTS;
         break;
#endif         
      case FMGR_REMOVABLE_STORAGE:
         drv_str_id = STR_FMGR_REM_DEVICE;
         break;
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
      case FMGR_STORAGE_TFCARD_SECOND:
         drv_str_id = STR_FMGR_SEC_REM_DEVICE;
         break;
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
      default:
         MMI_ASSERT(0);
         break;
   }
	
	pfnUnicodeStrcpy((PS8)buffer_UCS2, (PS8)(GetString(drv_str_id)));


   /*----------------------------------------------------------------*/
   /* Calcuate text position		                                    */
   /*----------------------------------------------------------------*/
	text_box_y1 = y1+3;
	text_box_y2 = y2-1;

	menuitem_height = text_box_y2-text_box_y1+1;

	coolsand_UI_set_font(&MMI_small_font);
	small_font_height = coolsand_UI_get_character_height();

	coolsand_UI_set_font(&MMI_medium_font);
	medium_font_height = coolsand_UI_get_character_height();

	text_spacing = (menuitem_height - medium_font_height-small_font_height)/3;

	if(text_spacing < 0)
		text_spacing = 0;

	left_align_offset_x	= x1+(y2-y1)+10;
	right_align_offset_x	= x2-(y2-y1)-5;
		
	drv_name_offset_y		= text_box_y1+text_spacing;
	date_size_offset_y	= text_box_y1+text_spacing*2+medium_font_height;

	if(is_hilighted)
	{
		text_color			= current_UI_theme->fixed_text_menuitem_theme->selected_text_color;
		text_shadow_color = current_UI_theme->fixed_text_menuitem_theme->normal_text_color;
	}
	else
	{
		text_color			= current_UI_theme->fixed_text_menuitem_theme->normal_text_color;
		text_shadow_color = current_UI_theme->fixed_text_menuitem_theme->selected_text_color;
	}

   /*----------------------------------------------------------------*/
   /* Draw Drvie Name				                                    */
   /*----------------------------------------------------------------*/
	coolsand_UI_set_font(&MMI_medium_font);

	/* set text clip, avoid draw to overlap scroll bar */
	if(r2lMMIFlag)
	{
		coolsand_UI_set_text_clip(x1+1,
										y1,
										right_align_offset_x,
										y2);
	}
	else
	{
		coolsand_UI_set_text_clip(left_align_offset_x,
										y1,
										x2-1,
										y2);
	}

	coolsand_UI_set_text_color(text_shadow_color);

	if(is_hilighted)
	{		
		if(r2lMMIFlag)
			coolsand_UI_move_text_cursor(right_align_offset_x+1, drv_name_offset_y+1);
		else
			coolsand_UI_move_text_cursor(left_align_offset_x+1, drv_name_offset_y+1);

		coolsand_UI_print_text((UI_string_type)buffer_UCS2);
	}

	coolsand_UI_set_text_color(text_color);

	if(r2lMMIFlag)
		coolsand_UI_move_text_cursor(right_align_offset_x, drv_name_offset_y);
	else
		coolsand_UI_move_text_cursor(left_align_offset_x, drv_name_offset_y);
	
	coolsand_UI_print_text((UI_string_type)buffer_UCS2);

   /*----------------------------------------------------------------*/
   /* Draw File Size		                                             */
   /*----------------------------------------------------------------*/
	coolsand_UI_reset_text_clip();
	coolsand_UI_set_text_color(text_color);
	coolsand_UI_set_font(&MMI_small_font);

	if(disk_free_space > 1024*1024*10)
		sprintf(buffer, "%dM/", (U16)(disk_free_space/(1024*1024)));
	else
		sprintf(buffer, "%dK/", (U16)(disk_free_space/(1024)));

	if(disk_total_space > 1024*1024*10)
		sprintf(buffer_UCS2, "%dM", (U16)(disk_total_space/(1024*1024)));
	else
		sprintf(buffer_UCS2, "%dK", (U16)(disk_total_space/(1024)));

	strcat(buffer, buffer_UCS2);

	AnsiiToUnicodeString((PS8)buffer_UCS2, (PS8)buffer);
	coolsand_UI_measure_string((UI_string_type)buffer_UCS2, &str_width, &str_height);

	if(r2lMMIFlag)
		coolsand_UI_move_text_cursor(x1+str_width+10, date_size_offset_y);
	else
		coolsand_UI_move_text_cursor(x2-str_width-4, date_size_offset_y);
	
	coolsand_UI_print_text((UI_string_type)buffer_UCS2);

	/* restore default font size */
	coolsand_UI_set_font(&MMI_medium_font);

	/*----------------------------------------------------------------*/
   /* Draw Icon															         */
   /*----------------------------------------------------------------*/
	if(r2lMMIFlag)
		mmi_fmgr_draw_free_space_icon(right_align_offset_x+10, y1, y2-y1+1, ratio);
	else
		mmi_fmgr_draw_free_space_icon(x1, y1, y2-y1+1, ratio);


	/* pop clip , unlock and blt */
	coolsand_UI_pop_clip();
	coolsand_UI_pop_text_clip();
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0, 0, UI_device_width-1, UI_device_height-1);

}




/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_hide_drv_menuitem_callback()
* DESCRIPTION
*   hide drv menitem's callback function
* PARAMETERS
*	 void
* RETURNS
*   void 
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_fmgr_hide_drv_menuitem_callback(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
//	gdi_image_stop_animation_all();
}




/******************************************************************************
* File\Folder UI related callback
******************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_get_small_file_type_icon_image_ptr()
* DESCRIPTION
*   get small file icon image
* PARAMETERS
* 	 file_info		IN		file info
* RETURNS
*   void 
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
UI_image_type mmi_fmgr_get_small_file_type_icon_image_ptr(FMGR_FILE_INFO_STRUCT *file_info)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/	
	if(file_info->attribute & FS_ATTR_DIR)
	{
		if( (FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_FOLDER_DOT)))
			return (UI_image_type)GetImage(IMG_ID_FMGR_ICON_FILE_TYPE_FOLDERUP);
		else 
			return (UI_image_type)GetImage(IMG_ID_FMGR_ICON_FILE_TYPE_FOLDER);
	}
	else
	{
#if !defined(MEM_ULC_3216)		
		/* image */
		if( (FMGR_FILTER_IS_SET_IMAGE(&file_info->file_type) ))
		{
			return (UI_image_type)GetImage(IMG_ID_FMGR_ICON_FILE_TYPE_IMG);
		}
		/* message */
		else if ( (FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_EMS)) || 
		   			(FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_ANM)) )
			
		{
			return (UI_image_type)GetImage(IMG_ID_FMGR_ICON_FILE_TYPE_MSG);
		}			
		/* audio */
		else if( FMGR_FILTER_IS_SET_AUDIO(&file_info->file_type))
		{
			U8 cmp_ret;
			S8 file_ext_amr[FMGR_MAX_EXT_LEN + 1] = {'a','m','r',0,0};
			S8 file_ext_wav[FMGR_MAX_EXT_LEN + 1] = {'w','a','v',0,0};
			S8 file_ext_AMR[FMGR_MAX_EXT_LEN + 1] = {'A','M','R',0,0};
			S8 file_ext_WAV[FMGR_MAX_EXT_LEN + 1] = {'W','A','V',0,0};
			S8 file_ext_tmp[FMGR_MAX_EXT_LEN + 1];
			memset(file_ext_tmp,0,sizeof(file_ext_tmp));
			UnicodeToAnsii((S8 *)file_ext_tmp , (S8 *)file_info->file_ext);
			#if defined (__MMI_AUDIO_PLAYER__)  && !defined(MMI_ULC)
			if((0 == (cmp_ret = strncmp(file_ext_tmp,file_ext_amr,sizeof(file_ext_amr)))) || 
			(0 == (cmp_ret = strncmp(file_ext_tmp,file_ext_AMR,sizeof(file_ext_AMR)))))
			{
				return (UI_image_type)GetImage(IMG_ID_AUDPLY_FILE_AMR_ICON);
			}
			else if((0 == (cmp_ret = strncmp(file_ext_tmp,file_ext_wav,sizeof(file_ext_wav))))||
			(0 == (cmp_ret = strncmp(file_ext_tmp,file_ext_WAV,sizeof(file_ext_WAV)))))
			{
				return (UI_image_type)GetImage(IMG_ID_AUDPLY_FILE_WAV_ICON);
			}
			else
			#endif
			{
				return (UI_image_type)GetImage(IMG_ID_FMGR_ICON_FILE_TYPE_AUD);
			}
		}
		/* video */
		else if( FMGR_FILTER_IS_SET_VIDEO(&file_info->file_type))
		{
			#ifdef __MMI_VIDEO__
				return (UI_image_type)GetImage(IMG_ID_FMGR_ICON_FILE_TYPE_VDO);
			#else
				return (UI_image_type)GetImage(IMG_ID_FMGR_ICON_FILE_TYPE_UNKNOWN);
			#endif /* __MMI_VIDEO__ */
		}
		else if ( (FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_EMS)) || 
		   			(FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_ANM)) )
			
		{
			return (UI_image_type)GetImage(IMG_ID_FMGR_ICON_FILE_TYPE_MSG);
		}	
		else if( (FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_JAD)) ||
					(FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_JAR)))
		{
			#ifdef __J2ME__	
				return  (UI_image_type)GetImage(IMG_ID_FMGR_ICON_FILE_TYPE_JAVA);
			#else
				return (UI_image_type)GetImage(IMG_ID_FMGR_ICON_FILE_TYPE_UNKNOWN);
			#endif /* __J2ME__ */
  		}
		else if(FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_VCF))
		{
			#ifdef __MMI_VCARD__
				return  (UI_image_type)GetImage(IMG_ID_FMGR_ICON_FILE_TYPE_VCF);
			#else
				return (UI_image_type)GetImage(IMG_ID_FMGR_ICON_FILE_TYPE_UNKNOWN);
			#endif /* __MMI_VCARD__ */
  		}
		else if(FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_VCS))
		{
			#ifdef __MMI_VCALENDAR__
				return  (UI_image_type)GetImage(IMG_ID_FMGR_ICON_FILE_TYPE_VCS);
			#else
				return (UI_image_type)GetImage(IMG_ID_FMGR_ICON_FILE_TYPE_UNKNOWN);
			#endif /* __MMI_VCALENDAR__ */
  		}
#ifdef __MMI_VCARD__
		else if(FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_CSV))
		{
				return (UI_image_type)GetImage(IMG_ID_FMGR_ICON_FILE_TYPE_VCF);
  		}
#endif        
#ifdef __MMI_EBOOK_READER__
	    	else if (FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_EBOOK_TXT))
	    	{
	       	 return (PU8) GetImage(IMG_EBOOK_FMGR_TXT_SMALL_TYPE);

	    	}
	    	else if (FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_EBOOK_PDB))
	    	{
	       	 return (PU8) GetImage(IMG_EBOOK_FMGR_PDB_SMALL_TYPE);

	   	 }
#endif /* __MMI_EBOOK_READER__ */ 
		else 
#endif
		{
			return (UI_image_type)GetImage(IMG_ID_FMGR_ICON_FILE_TYPE_UNKNOWN);
		}
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_get_large_file_type_icon_image_ptr()
* DESCRIPTION
*   get large file icon image
* PARAMETERS
* 	 file_info		IN		file info
* RETURNS
*   void 
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
UI_image_type mmi_fmgr_get_large_file_type_icon_image_ptr(FMGR_FILE_INFO_STRUCT *file_info)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
/* MACRO */
#define FMGR_GET_IMAGE_ICON_ELSEIF(a, b) \
	else if(FMGR_FILTER_IS_SET(&file_info->file_type, a))\
		return (UI_image_type)GetImage(b);


	if(file_info->attribute & FS_ATTR_DIR)
	{
		return (UI_image_type)GetImage(IMG_ID_FMGR_FILE_TYPE_FOLDER);
	}
	else
	{
//		mmi_trace(TRUE, "IMG_ID_FMGR_FILE_TYPE_BMP =%d", IMG_ID_FMGR_FILE_TYPE_BMP);
//		mmi_trace(TRUE, "IMG_ID_FMGR_FILE_TYPE_JPG =%d", IMG_ID_FMGR_FILE_TYPE_JPG);
//		mmi_trace(TRUE, "IMG_ID_FMGR_FILE_TYPE_JPG =%d", IMG_ID_FMGR_FILE_TYPE_GIF);

#if !defined(MEM_ULC_3216)		
		/* image */
		if (FMGR_FILTER_IS_SET(&file_info->file_type, FMGR_TYPE_BMP))
			return (UI_image_type)(UI_image_type)GetImage(IMG_ID_FMGR_FILE_TYPE_BMP);

		//#ifdef JPG_DECODE		/* wufasong added 2007.02.08 */
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_JPG, IMG_ID_FMGR_FILE_TYPE_JPG)
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_JPEG, IMG_ID_FMGR_FILE_TYPE_JPG)		
		//#endif
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_GIF, IMG_ID_FMGR_FILE_TYPE_GIF)	
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_PNG, IMG_ID_FMGR_FILE_TYPE_PNG)
 		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_WBM, IMG_ID_FMGR_FILE_TYPE_WBMP)
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_WBMP, IMG_ID_FMGR_FILE_TYPE_WBMP)	
		/* message */
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_EMS, IMG_ID_FMGR_FILE_TYPE_EMS)
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_ANM, IMG_ID_FMGR_FILE_TYPE_ANM)
		/* audio*/
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_IMY, IMG_ID_FMGR_FILE_TYPE_IMY)
#ifdef MDI_MIDI_SUPPORT		
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_MID, IMG_ID_FMGR_FILE_TYPE_MID)
#endif		
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_WAV, IMG_ID_FMGR_FILE_TYPE_WAV)
		//FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_WMA, IMG_ID_FMGR_FILE_TYPE_WMA) //fix bug12600
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_AMR, IMG_ID_FMGR_FILE_TYPE_AMR)
		#ifdef AAC_DECODE	 
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_AAC, IMG_ID_FMGR_FILE_TYPE_AAC)
		#endif
		#ifdef DAF_DECODE
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_DAF, IMG_ID_FMGR_FILE_TYPE_DAF)
		#endif
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_VM, IMG_ID_FMGR_FILE_TYPE_VM)
		#ifdef AMRWB_DECODE
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_AWB, IMG_ID_FMGR_FILE_TYPE_AWB)
		#endif
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_AIF, IMG_ID_FMGR_FILE_TYPE_AIF)
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_AIFF, IMG_ID_FMGR_FILE_TYPE_AIF)
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_AIFC, IMG_ID_FMGR_FILE_TYPE_AIF)
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_AU, IMG_ID_FMGR_FILE_TYPE_AU)
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_SND, IMG_ID_FMGR_FILE_TYPE_AU)
		#ifdef MP4_CODEC
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_M4A, IMG_ID_FMGR_FILE_TYPE_M4A)		
		#endif		
		#if defined( EXT_AUDIO_CHIP ) || defined( MMF_DECODE) 
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_MMF, IMG_ID_FMGR_FILE_TYPE_DAF)		
		#endif				
		#ifdef WMA_DECODE
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_WMA, IMG_ID_FMGR_FILE_TYPE_DAF)
		#endif
		/* video */
//		#ifdef MP4_CODEC  //guojian deleted for jassmine 20080508
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_3GP, IMG_ID_FMGR_FILE_TYPE_3GP)	
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_MP4, IMG_ID_FMGR_FILE_TYPE_MP4)	
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_RM, IMG_ID_FMGR_FILE_TYPE_MP4)	
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_RMVB, IMG_ID_FMGR_FILE_TYPE_MP4)
//		#endif
		#ifdef MJPG_SUPPORT
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_AVI, IMG_ID_FMGR_FILE_TYPE_AVI)	
		#endif
		/* others */
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_FOLDER, IMG_ID_FMGR_FILE_TYPE_FOLDER)	
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_FOLDER_DOT, IMG_ID_FMGR_FILE_TYPE_FOLDERUP)	
		#ifdef __J2ME__	
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_JAD, IMG_ID_FMGR_FILE_TYPE_JAD)		
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_JAR, IMG_ID_FMGR_FILE_TYPE_JAR)		
		#endif /* __J2ME__ */
		#ifdef __MMI_VCARD__
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_VCF, IMG_ID_FMGR_FILE_TYPE_VCF)
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_CSV, IMG_ID_FMGR_FILE_TYPE_CSV)		
		#endif /* __MMI_VCARD__ */
		#ifdef __MMI_VCALENDAR__
		FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_VCS, IMG_ID_FMGR_FILE_TYPE_VCS)
		#endif /* __MMI_VCALENDAR__ */
 #ifdef __MMI_EBOOK_READER__
            	FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_EBOOK_TXT, IMG_EBOOK_FMGR_TXT_LARGE_TYPE)
           	FMGR_GET_IMAGE_ICON_ELSEIF(FMGR_TYPE_EBOOK_PDB, IMG_EBOOK_FMGR_PDB_LARGE_TYPE)
 #endif /* __MMI_EBOOK_READER__ */ 
		else
#endif /* MEM_ULC_3216 */
			return (UI_image_type)GetImage(IMG_ID_FMGR_FILE_TYPE_UNKNOWN);
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_get_file_item_callback()
* DESCRIPTION
*   get large file icon image
* PARAMETERS
* 	 item_idx			IN		index
* 	 str_buff			IN		string buffer
* 	 img_buff_p			IN		image buffer 
* 	 str_img_mask		IN		mask
* RETURNS
*   void 
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
pBOOL mmi_fmgr_get_file_item_callback(S32 item_idx, UI_string_type str_buff, UI_image_type *img_buff_p, U8	str_img_mask)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
	FMGR_FILE_INFO_STRUCT file_info;

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/	
	TRACE_FMGR_FUNCTION();
	mmi_fmgr_kernel_get_file_info(item_idx, &file_info);

	memset(str_buff, 0, sizeof(MAX_SUB_MENU_SIZE));
#ifdef MMI_ON_HARDWARE_P
	if (file_info.is_short)
	   mmi_chset_mixed_text_to_ucs2_str( (U8*)str_buff, (MAX_SUBMENU_CHARACTERS-1) * ENCODING_LENGTH, (U8*)file_info.file_name, PhnsetGetDefEncodingType() );
	else
#endif
		pfnUnicodeStrncpy((PS8)str_buff, (PS8)file_info.file_name, MAX_SUBMENU_CHARACTERS-1);

	/* hide ext name for diaplay */
	// mmi_fmgr_kernel_hide_ext_name((PS8)str_buff, &file_info);

	*img_buff_p = mmi_fmgr_get_small_file_type_icon_image_ptr(&file_info);

	return TRUE;
}



/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_get_file_hint_callback()
* DESCRIPTION
*   callback function for getting dynamic list hint
* PARAMETERS
* 	 item_idx			IN		index
* 	 hint_array			IN		hint buffer
* RETURNS
*   void 
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
S32 mmi_fmgr_get_file_hint_callback(S32 item_idx, UI_string_type *hint_array)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/	
	return 0;
}

#ifdef __MMI_EBOOK_READER__


/*****************************************************************************
 * FUNCTION
 *  ebr_get_file_item_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  item_idx            [IN]        
 *  str_buff            [IN]        
 *  img_buff_p          [?]         
 *  str_img_mask        [IN]        
 * RETURNS
 *  
 *****************************************************************************/
pBOOL ebr_get_file_item_callback(S32 item_idx, UI_string_type str_buff, PU8 *img_buff_p, U8 str_img_mask)
{

    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FMGR_FILE_INFO_STRUCT file_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    memset(str_buff, 0, sizeof(MAX_SUB_MENU_SIZE));
    mmi_fmgr_kernel_get_file_info(item_idx, &file_info);
#ifdef MMI_ON_HARDWARE_P
    if (file_info.is_short)
    {
        mmi_chset_mixed_text_to_ucs2_str(
            (U8*) str_buff,
            (MAX_SUBMENU_CHARACTERS - 1) * ENCODING_LENGTH,
            (U8*) file_info.file_name,
            PhnsetGetDefEncodingType());
    }
    else
#endif /* MMI_ON_HARDWARE_P */ 
	pfnUnicodeStrncpy((PS8) str_buff, (PS8) file_info.file_name, MAX_SUBMENU_CHARACTERS - 1);
    ebr_fmgr_kernel_hide_ext_name((PS8) str_buff, &file_info);
	*img_buff_p = mmi_fmgr_get_small_file_type_icon_image_ptr(&file_info);
    return TRUE;
}
#endif /* __MMI_EBOOK_READER__ */ 


/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_get_file_hint_callback()
* DESCRIPTION
*   callback function for draw seleted drv info
* PARAMETERS
* 	 item_idx		IN		index
* 	 x1				IN		x1
* 	 y1				IN		y1
* 	 x2				IN		x2
* 	 y2				IN		y2
* RETURNS
*   void 
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_fmgr_draw_file_info_callback(S32 item_idx, S32 x1, S32 y1, S32 x2, S32 y2)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/	
	S32				char_height;
	S8					buffer[64];
	S8					buffer_UCS2[64];
	S32				str_width, str_height;
	S32				box_width, box_height;
	S32				box_offset_x, box_offset_y;	
	S32				img_width, img_height;
	S32				spacing;
	UI_image_type	icon_ptr;
	S32				size_offset_y;
	S32				date_offset_y;
	S32				size_str_offset_y;
	S32				date_str_offset_y;	
	S32				left_align_offset_x;
	S32				right_align_offset_x;
	U32				file_size;
	BOOL				is_drm;

	FMGR_FILE_INFO_STRUCT file_info;

#ifdef __DRM_SUPPORT__   
	FS_HANDLE		file_h;
	S8					buffer_path[FMGR_PATH_BUFFER_SIZE];
#endif 

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	is_drm = FALSE;
	
	/* lock and push clip */
	gdi_layer_lock_frame_buffer();
	gdi_layer_push_clip();
	gdi_layer_reset_clip();

   /*-------------------------*/
   /* Get File info				*/
   /*-------------------------*/
	/* draw Date */

	mmi_fmgr_kernel_get_file_info(item_idx, &file_info);

   /*renwy Modify Start For bug 8634 2008/06/20 */
   if ((fmgr_p->src_application == APP_PROFILES || fmgr_p->src_application == APP_PHONEBOOK) &&
   /*renwy Modify End  For bug 8634 2008/06/20 */
   	(FMGR_FILTER_IS_SET_AUDIO(&file_info.file_type))&&
   	!isInCall()) //chenhe add,081106
   {
      StartTimer(FMGR_AUDIO_TIMER, 1000, fmgr_play_audio_hdlr);
   }

   /*-------------------------*/
   /* Calculate text position	*/
   /*-------------------------*/
	box_height			= y2-y1-FMGR_UI_IMAGE_BOX_SPACING*2+1;
	box_width			= box_height;

	box_offset_y 		= FMGR_UI_IMAGE_BOX_SPACING;

	if(r2lMMIFlag)
	{
		box_offset_x = x2-box_width-FMGR_UI_IMAGE_BOX_SPACING;
	}
	else
	{
		box_offset_x = FMGR_UI_IMAGE_BOX_SPACING;		
	}
	
	
	coolsand_UI_set_font(&MMI_medium_font);
	char_height			= coolsand_UI_get_string_height((UI_string_type)GetString(STR_ID_FMGR_FILE_DATE));
	spacing				= (y2-y1-1-char_height*2)/3;

	size_str_offset_y = spacing+MMI_title_height+MMI_title_y+1;						/* +1 make it looks better */
	date_str_offset_y	= spacing*2+char_height+MMI_title_height+MMI_title_y+1;	/* +1 make it looks better */

	char_height			= coolsand_UI_get_string_height((UI_string_type)L"1");
	spacing				= (y2-y1-1-char_height*2)/3;

	size_offset_y 		= spacing+MMI_title_height+MMI_title_y+1;						/* +1 make it looks better */
	date_offset_y		= spacing*2+char_height+MMI_title_height+MMI_title_y+1;	/* +1 make it looks better */


	left_align_offset_x	= x1+box_width+FMGR_UI_TEXT_SPACING;
	right_align_offset_x = x2-box_width-FMGR_UI_TEXT_SPACING;

	coolsand_UI_set_text_color(coolsand_UI_color(0,0,0));


   /*-------------------------*/
   /* Draw Size Text				*/
   /*-------------------------*/   
	if(!(file_info.attribute & FS_ATTR_DIR))
	{
		if(UI_device_width > 128)
		{
			if(r2lMMIFlag)	
				coolsand_UI_move_text_cursor(right_align_offset_x, size_str_offset_y);
			else
				coolsand_UI_move_text_cursor(left_align_offset_x, size_str_offset_y);
			
			coolsand_UI_print_text((UI_string_type)GetString(STR_ID_FMGR_FILE_SIZE));
		}

		if(file_info.file_size > 1024*1024)
		{
			file_size = file_info.file_size/1024;
			sprintf(buffer, "%d.%dM", (U16)(file_size/1024), (U16)((file_size%1024)/103));
		}
		else if(file_info.file_size > 1024)
		{
			sprintf(buffer, "%d.%dK", (U16)(file_info.file_size/1024), (U16)((file_info.file_size%1024)/103));
		}
		else	/* less than 1024 */
		{
			sprintf(buffer, "%dB", (U16)(file_info.file_size));
		}

		AnsiiToUnicodeString((PS8)buffer_UCS2, (PS8)buffer);
		coolsand_UI_measure_string((UI_string_type)buffer_UCS2, &str_width, &str_height);

		if(r2lMMIFlag)	
			coolsand_UI_move_text_cursor(x1+str_width+FMGR_UI_TEXT_SPACING, size_offset_y);
		else
			coolsand_UI_move_text_cursor(x2-str_width-FMGR_UI_TEXT_SPACING, size_offset_y);	
		
		coolsand_UI_print_text((UI_string_type)buffer_UCS2);
	}

   /*----------------------------------------------------------------*/
   /* Draw Date Text 			   				                        */
   /*----------------------------------------------------------------*/
	coolsand_UI_set_font(&MMI_medium_font);

	coolsand_UI_reset_text_clip();
	coolsand_UI_set_text_color(coolsand_UI_color(0,0,0));

	if(UI_device_width > 128)
	{
		if(r2lMMIFlag)
			coolsand_UI_move_text_cursor(right_align_offset_x, date_str_offset_y);
		else
			coolsand_UI_move_text_cursor(left_align_offset_x, date_str_offset_y);
		
		coolsand_UI_print_text((UI_string_type)GetString(STR_ID_FMGR_FILE_DATE));
	}

	/* we do not have enough room to put all dat format, we use sub's foramt */
	date_string(&file_info.time, (PU16)buffer, DT_FILEMANAGER);

	coolsand_UI_measure_string((UI_string_type)buffer, &str_width, &str_height);

	if(r2lMMIFlag)	
		coolsand_UI_move_text_cursor(x1+str_width+FMGR_UI_TEXT_SPACING, date_offset_y);
	else
		coolsand_UI_move_text_cursor(x2-str_width-FMGR_UI_TEXT_SPACING, date_offset_y);	
	
	coolsand_UI_print_text((UI_string_type)buffer);


   /*-------------------------*/
   /* Draw Large Icon			*/
   /*-------------------------*/     
	icon_ptr = mmi_fmgr_get_large_file_type_icon_image_ptr(&file_info);
mmi_trace(1,"icon_ptr is %x",icon_ptr);
	gdi_image_get_dimension(icon_ptr, &img_width, &img_height);
	spacing = (box_height - img_height) >> 1;

#ifdef __DRM_SUPPORT__   
	if((FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPG) || 
		(FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPEG))))
	{
		mmi_fmgr_kernel_get_path_name((PS8)buffer_path);
		pfnUnicodeStrcat((PS8)buffer_path, (PS8)file_info.file_name);

		file_h = DRM_open_file((PU16)buffer_path, FS_READ_ONLY, DRM_PERMISSION_NONE);

		if(file_h >= 0)
		{
			if(DRM_get_object_method(file_h) != DRM_METHOD_NONE)
			{
				/* is drm file */
				is_drm = TRUE;
			}
			DRM_close_file(file_h);		
		}
	}
#endif /* __DRM_SUPPORT__ */

	/* draw thumb image */
#if defined(FMGR_USING_CACHE)
	if((!is_drm) && 
		(FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPG) || 
		 FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPEG)))
	{
		if(r2lMMIFlag)	
		{
			mmi_fmgr_show_thumb_image(	item_idx, 
												x1, y1, 
												x2-box_width-1, box_offset_y, 
												box_height, box_height,
												icon_ptr, TRUE, FMGR_UI_INFO_STYLE);
		}
		else
		{
			mmi_fmgr_show_thumb_image(	item_idx, 
												x1, y1, 
												box_offset_x, box_offset_y, 
												box_height, box_height,
												icon_ptr, TRUE, FMGR_UI_INFO_STYLE);
		}	
	}
	else
	{
		if(r2lMMIFlag)			
			gdi_image_draw(x2-box_height+2, y1+spacing+1, icon_ptr);
		else
		{	
			/* align at center */
			gdi_image_draw(	x1+box_offset_x+((box_height-img_width)>>1), 
												y1+box_offset_y+((box_height-img_height)>>1), 
												icon_ptr);
		}
	}
#else

	if(r2lMMIFlag)	
		gdi_image_draw(x2-box_height+2, y1+spacing+1, icon_ptr);
	else
	{
		/* align at center */	
		gdi_image_draw(	x1+box_offset_x+((box_height-img_width)>>1), 
											y1+box_offset_y+((box_height-img_height)>>1), 
											icon_ptr);
	}
		
#endif /* FMGR_USING_CACHE */

	/* pop clip , unlock and blt */
	gdi_layer_pop_clip();
	gdi_layer_unlock_frame_buffer();
	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);
}


/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_hide_file_info_callback()
* DESCRIPTION
*   callback function for stop draw seleted drv info
* PARAMETERS
*	 void
* RETURNS
*   void 
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_fmgr_hide_file_info_callback(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/	
#if defined(FMGR_USING_CACHE)
	S32 i;
#endif /* FMGR_USING_CACHE */

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
   StopTimer(FMGR_AUDIO_TIMER);
   if (fmgr_p->audio_state == TRUE)
   {
   	fmgr_p->audio_state = FALSE;
#ifdef __MMI_AUDIO_PLAYER__ 
      mdi_audio_stop_file();
#endif
   }

#if defined(FMGR_USING_CACHE)

	/* stop all decoding jpeg files */
	#ifndef MMI_ON_WIN32	
		gdi_image_decoder_stop_all();
	#endif /* MMI_ON_WIN32 */
	
	/* reset delay decoder */
	g_fmgr_context.delay_decode_index = 0;
	
	StopTimer(FMGR_DELAY_DECODE_0);
	StopTimer(FMGR_DELAY_DECODE_1);
	StopTimer(FMGR_DELAY_DECODE_2);
	StopTimer(FMGR_DELAY_DECODE_3);	
	StopTimer(FMGR_DELAY_DECODE_4);
	StopTimer(FMGR_DELAY_DECODE_5);
	StopTimer(FMGR_DELAY_DECODE_6);
	StopTimer(FMGR_DELAY_DECODE_7);
	StopTimer(FMGR_DELAY_DECODE_8);
	StopTimer(FMGR_DELAY_DECODE_9);	
	
	/* clear image cache handle entries */
	for( i = 0 ; i < FMGR_THUMB_IMAGE_HANDLE_MAX_COUNT; i++)
		fmgr_image_cache_p->image_handle_list[i]->is_used	= FALSE;

	#ifndef MMI_ON_WIN32	
		/* free jpeg decoder */
		if(fmgr_p->is_decoder_create)
		{
			gdi_image_decoder_free();
			fmgr_p->is_decoder_create = FALSE;			
		}
	#endif /* MMI_ON_WIN32 */

#endif /* FMGR_USING_CACHE */

	/* stop all animations */
//	gdi_image_stop_animation_all();
}


#if defined(__PROJECT_GALLITE_C01__)
gdi_image_cache_bmp_struct list_item_cache;
U16 cache_buf[2650];//119x22
S32 cache_x,cache_y;
#endif
/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_draw_hilight_menuitem_bg_callback()
* DESCRIPTION
*   call back function for draw hilight menuitem, used by scrolling text
* PARAMETERS
* 	 x1	IN		x1
* 	 y1	IN		y1
* 	 x2	IN		x2
* 	 y2	IN		y2
* RETURNS
*   void 
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_fmgr_draw_hilight_menuitem_bg_callback(S32 x1, S32 y1, S32 x2, S32 y2)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
#if defined(__PROJECT_GALLITE_C01__)
	gdi_image_cache_bmp_draw(cache_x, cache_y, &list_item_cache);
#else	
	coolsand_UI_push_clip();
	coolsand_UI_set_clip(x1, y1, x2, y2);
	coolsand_UI_fill_rectangle(x1,y1,x2,y2,MMI_fixed_icontext_list_menuitem.selected_filler->c);
	coolsand_UI_pop_clip();
#endif
}


/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_draw_hilight_menuitem_timer_callback()
* DESCRIPTION
*   draw hilight menuitem text, callback function for scrolling text
* PARAMETERS
*   void
* RETURNS
*   void 
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_fmgr_draw_hilight_menuitem_timer_callback(void)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/	
	coolsand_UI_handle_scrolling_text( &fmgr_p->scroll_text);
}



/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_draw_file_menuitem_callback()
* DESCRIPTION
*   draw file menuitem callack
* PARAMETERS
* 	 item_idx		IN		index
* 	 is_hilighted	IN		hilighted flag
* 	 x1				IN		x1
* 	 y1				IN		y1
* 	 x2				IN		x2
* 	 y2				IN		y2
* RETURNS
*   void 
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_fmgr_draw_file_menuitem_callback(S32 item_idx, BOOL is_hilighted, S32 x1, S32 y1, S32 x2, S32 y2)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/	
	S8					buffer[128];			/* this buffer should larger than (FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH */
	S8					buffer_UCS2[128];
	S32				str_width, str_height;
	S32				img_width, img_height;
	UI_image_type	icon_ptr;
	S32				small_font_height;
	S32				medium_font_height;
	S32				text_spacing;
	S32				left_align_offset_x;
	S32				r2l_x1;
	S32				r2l_x2;
	S32				filename_offset_y;
	S32				date_size_offset_y;
	color_t				text_color;
	color_t				text_shadow_color;
	S32				file_size;
	S32				box_offset_x;
	S32				box_offset_y;	
	S32				box_width;
	S32				box_height;	
	S32				image_offset_x;
	S32				image_offset_y;	
	BOOL				is_drm;

	FMGR_FILE_INFO_STRUCT file_info;

#ifdef __DRM_SUPPORT__   
	FS_HANDLE		file_h;
	S8					buffer_path[FMGR_PATH_BUFFER_SIZE];	
#endif 

    TRACE_EF(g_sw_FMGR, "Excute func : mmi_fmgr_draw_file_menuitem_callback()");

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	is_drm = FALSE;
	
	/* set index to fm's global index */
	if(is_hilighted)
		fmgr_p->cur_index = item_idx;

	mmi_fmgr_kernel_get_file_info(item_idx, &file_info);

   if (fmgr_p->src_application == APP_PROFILES && 
  		(FMGR_FILTER_IS_SET_AUDIO(&file_info.file_type)))
   {
      StartTimer(FMGR_AUDIO_TIMER, 1000, fmgr_play_audio_hdlr);
   }

	/* lock and push clip */
	gdi_layer_lock_frame_buffer();
	gdi_layer_push_clip();
	gdi_layer_reset_clip();
	
	/*
	* this function will draw thumb image menuitem .
	*
	* --------------------------------------
	* | ##### FILENAME_HERE.EXT		    	 |		<--- Medium Font
	* | ##### 									    |
	* | ##### Date						  Size    |		<--- Small Font
	* --------------------------------------
	* Info. (1) Image, it will be the dimension(ex: 640x480) of the file.
	*       (2) Video, Audio. it will be duration of the file.
	*/

	box_height			= y2-y1-FMGR_UI_IMAGE_BOX_SPACING*2+1;
	box_width			= box_height;

	box_offset_y 		= FMGR_UI_IMAGE_BOX_SPACING;
	
	if(r2lMMIFlag)
	{
		box_offset_x = x2-box_width-FMGR_UI_IMAGE_BOX_SPACING;	
	}
	else
	{
		box_offset_x = FMGR_UI_IMAGE_BOX_SPACING;	
	}



   /*----------------------------------------------------------------*/
   /* Calcuate text position here                                    */
   /*----------------------------------------------------------------*/
  	coolsand_UI_set_font(&MMI_small_font);
	small_font_height = coolsand_UI_get_character_height();

	coolsand_UI_set_font(&MMI_medium_font);
	medium_font_height = coolsand_UI_get_string_height((UI_string_type)file_info.file_name);

	text_spacing = ((y2-y1+1)-medium_font_height-small_font_height)/3;

	if(text_spacing < 0)
		text_spacing = 0;

	left_align_offset_x	= x1+box_width+FMGR_UI_TEXT_SPACING;
	
	r2l_x1 = x1+MMI_fixed_list_menu.vbar.width+2;
	r2l_x2 = x2-(y2-y1+1)-text_spacing;
		
	filename_offset_y		= y1+text_spacing;
	date_size_offset_y	= y1+text_spacing*2+medium_font_height;

	if(is_hilighted)
	{
		text_color			= current_UI_theme->fixed_text_menuitem_theme->selected_text_color;
		text_shadow_color = current_UI_theme->fixed_text_menuitem_theme->normal_text_color;
	}
	else
	{
		text_color			= current_UI_theme->fixed_text_menuitem_theme->normal_text_color;
		text_shadow_color = current_UI_theme->fixed_text_menuitem_theme->selected_text_color;
	}


   /*----------------------------------------------------------------*/
   /* Draw File Name					                                    */
   /*----------------------------------------------------------------*/
#ifdef MMI_ON_HARDWARE_P
		if (file_info.is_short)
            {
			mmi_chset_mixed_text_to_ucs2_str( (U8*)buffer_UCS2, sizeof (buffer_UCS2), (U8*)file_info.file_name, PhnsetGetDefEncodingType() );
            }
		else
#endif
            {
                //pfnUnicodeStrcpy((PS8)buffer_UCS2, (PS8)file_info.file_name);
                pfnUnicodeStrncpy((PS8)buffer_UCS2, (PS8)file_info.file_name, MAX_SUBMENU_CHARACTERS-1);
            }	
	/* hide ext name for diaplay */
	mmi_fmgr_kernel_hide_ext_name((PS8)buffer_UCS2, &file_info);

	if(is_hilighted)
	{
		/* scrolling text has to be stored in global variable */
		/* we use new_file_name buffer since it is not used in this scenario */
		pfnUnicodeStrcpy((PS8)fmgr_p->new_file_name, (PS8)buffer_UCS2);

		if(r2lMMIFlag)
		{
			coolsand_UI_create_scrolling_text(	&fmgr_p->scroll_text, 
													  	r2l_x1+4, 														/* x */
													  	filename_offset_y,	 										/* y */
													   r2l_x2-r2l_x1-4,												/* width */
													  	medium_font_height+2,										/* height */
														(UI_string_type)fmgr_p->new_file_name, 				/* string */
														mmi_fmgr_draw_hilight_menuitem_timer_callback, 		/* timer hdlr*/
														mmi_fmgr_draw_hilight_menuitem_bg_callback,			/* draw background hdlr */
														text_color, 													/* text color */
														MMI_fixed_icontext_list_menuitem.selected_filler->c); 		/* bg */
		}
		else
		{
			coolsand_UI_create_scrolling_text(	&fmgr_p->scroll_text, 
													  	left_align_offset_x, 											/* x */
													  	filename_offset_y,	 											/* y */
													  	x2-left_align_offset_x-4, 		 								/* width */
													  	medium_font_height+2,											/* height */
														(UI_string_type)fmgr_p->new_file_name, 					/* string */
														mmi_fmgr_draw_hilight_menuitem_timer_callback, 			/* timer hdlr*/
														mmi_fmgr_draw_hilight_menuitem_bg_callback,				/* draw background hdlr */
														text_color, 														/* text color */
														MMI_fixed_icontext_list_menuitem.selected_filler->c); /* bg */
		}

		fmgr_p->scroll_text.text_font	= &MMI_medium_font;
#if defined(__PROJECT_GALLITE_C01__)
		list_item_cache.buf_ptr = &cache_buf;
		gdi_image_cache_bmp_get(fmgr_p->scroll_text.x, fmgr_p->scroll_text.y, fmgr_p->scroll_text.x + fmgr_p->scroll_text.width -1, fmgr_p->scroll_text.y + fmgr_p->scroll_text.height - 1, &list_item_cache);
		cache_x = fmgr_p->scroll_text.x;
		cache_y = fmgr_p->scroll_text.y;
#endif
		coolsand_UI_show_scrolling_text( &fmgr_p->scroll_text );
	}
	else
	{
		coolsand_UI_set_font(&MMI_medium_font);

		/* set text clip, avoid draw to overlap scroll bar */
		coolsand_UI_set_text_color(text_color);		
		
		if(r2lMMIFlag)
		{
			coolsand_UI_set_text_clip(r2l_x1,
											filename_offset_y,
											r2l_x2,
											filename_offset_y+medium_font_height+1);

			coolsand_UI_move_text_cursor(r2l_x2, filename_offset_y);
		}
		else
		{
			coolsand_UI_set_text_clip(left_align_offset_x,
											filename_offset_y,
											x2-1,
											filename_offset_y+medium_font_height+1);

			coolsand_UI_move_text_cursor(left_align_offset_x, filename_offset_y);	
		}

		coolsand_UI_print_text((UI_string_type)buffer_UCS2);
	}

   /*----------------------------------------------------------------*/
   /* Draw File Creation Date                                        */
   /*----------------------------------------------------------------*/
	/* if is directory or is file but lcd is large enough to fit date and size on one line  */
	if( (file_info.attribute & FS_ATTR_DIR) || 
		 (!(file_info.attribute & FS_ATTR_DIR) && UI_device_width > 128) )
	{
		coolsand_UI_set_font(&MMI_small_font);
		coolsand_UI_reset_text_clip();
		coolsand_UI_set_text_color(text_color);

		/* we do not have enough room to put all dat format, we use sub's foramt */
		date_string(&file_info.time, (PU16)buffer, DT_FILEMANAGER);

		if(r2lMMIFlag)
			coolsand_UI_move_text_cursor(r2l_x2, date_size_offset_y);
		else
			coolsand_UI_move_text_cursor(left_align_offset_x, date_size_offset_y);
			
		
		coolsand_UI_print_text((UI_string_type)buffer);
	}


   /*----------------------------------------------------------------*/
   /* Draw File Size		                                             */
   /*----------------------------------------------------------------*/
	if( !(file_info.attribute & FS_ATTR_DIR))
	{
		coolsand_UI_reset_text_clip();
		coolsand_UI_set_text_color(text_color);
		coolsand_UI_set_font(&MMI_small_font);

		if(file_info.file_size > 1024*1024)
		{
			file_size = file_info.file_size/1024;
			sprintf(buffer, "%d.%dM", (U16)(file_size/1024), (U16)((file_size%1024)/103));
		}
		else if(file_info.file_size > 1024)
		{
			sprintf(buffer, "%d.%dK", (U16)(file_info.file_size/1024), (U16)((file_info.file_size%1024)/103));
		}
		else	/* less than 1024 */
		{
			sprintf(buffer, "%dB", (U16)(file_info.file_size));
		}

		AnsiiToUnicodeString((PS8)buffer_UCS2, (PS8)buffer);
		coolsand_UI_measure_string((UI_string_type)buffer_UCS2, &str_width, &str_height);

		if(r2lMMIFlag)
			coolsand_UI_move_text_cursor(r2l_x1+str_width, date_size_offset_y);
		else
			coolsand_UI_move_text_cursor(x2-str_width-4, date_size_offset_y);
		
		coolsand_UI_print_text((UI_string_type)buffer_UCS2);
	}

	/* restore font to default size */
	coolsand_UI_set_font(&MMI_medium_font);

   /*----------------------------------------------------------------*/
   /* Draw Large Icon	                                             */
   /*----------------------------------------------------------------*/
	icon_ptr = mmi_fmgr_get_large_file_type_icon_image_ptr(&file_info);
	
		
	gdi_image_get_dimension(icon_ptr, &img_width, &img_height);
	image_offset_x = (box_width-img_width)>>1;
	image_offset_y = (box_height-img_height)>>1;
//	mmi_trace(TRUE, "icon_ptr =%d,img_width =%d, img_height = %d", icon_ptr, img_width, img_height);

#ifdef __DRM_SUPPORT__   
	if((FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPG) || 
		(FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPEG))))
	{
		mmi_fmgr_kernel_get_path_name((PS8)buffer_path);
		pfnUnicodeStrcat((PS8)buffer_path, (PS8)file_info.file_name);

		file_h = DRM_open_file((PU16)buffer_path, FS_READ_ONLY, DRM_PERMISSION_NONE);
		
		if(file_h >= 0)
		{
			if(DRM_get_object_method(file_h) != DRM_METHOD_NONE)
			{
				/* is drm file */
				is_drm = TRUE;
			}
			DRM_close_file(file_h);		
		}
	}
#endif /* __DRM_SUPPORT__ */

	if(is_hilighted)
	{
		#if defined(FMGR_USING_CACHE)
			/* draw thumb image */
			if((!is_drm) && 
				(FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPG) || 
				 FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPEG)))
			{
				mmi_fmgr_show_thumb_image(	item_idx, 
													x1, y1, 
													box_offset_x, box_offset_y, 
													box_width, box_height,
													icon_ptr, is_hilighted, FMGR_UI_LIST_STYLE);
			}
			else
			{
				mmi_trace(TRUE, "icon_ptr =%d,x1+box_offset_x+image_offset_x =%d, y1+box_offset_y+image_offset_y = %d", icon_ptr, x1+box_offset_x+image_offset_x, y1+box_offset_y+image_offset_y);

				gdi_image_draw(x1+box_offset_x+image_offset_x, 
													y1+box_offset_y+image_offset_y, 
									icon_ptr);
			}
			
		#else
			gdi_image_draw(x1+box_offset_x+image_offset_x, 
												y1+box_offset_y+image_offset_y, 
								icon_ptr);
		#endif /* FMGR_USING_CACHE */

	}
	else
	{

		#if defined(FMGR_USING_CACHE)
			/* draw thumb image */
			if(FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPG) ||
				FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPEG))
			{
				mmi_fmgr_show_thumb_image(	item_idx, 
													x1, y1, 
													box_offset_x, box_offset_y, 
													box_width, box_height,
													icon_ptr, is_hilighted, FMGR_UI_LIST_STYLE);
			}
			else
			{
				gdi_image_draw(x1+box_offset_x+image_offset_x, 
									y1+box_offset_y+image_offset_y, 
									icon_ptr);
			}
		#else
			gdi_image_draw(x1+box_offset_x+image_offset_x, 
								y1+box_offset_y+image_offset_y, 
								icon_ptr);
		#endif /* FMGR_USING_CACHE */
	 
	}

	/* pop clip , unlock and blt */
	gdi_layer_pop_clip();
	gdi_layer_unlock_frame_buffer();
	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);

}



/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_hide_file_menuitem_callback()
* DESCRIPTION
*   hide file menuitem callack
* PARAMETERS
* 	 void
* RETURNS
*   void 
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_fmgr_hide_file_menuitem_callback(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/
#if defined(FMGR_USING_CACHE)
	S32 i;
#endif /* FMGR_USING_CACHE */

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
   StopTimer(FMGR_AUDIO_TIMER);
   if (fmgr_p->audio_state == TRUE)
   {
   	fmgr_p->audio_state = FALSE;
#ifdef __MMI_AUDIO_PLAYER__ 
      mdi_audio_stop_file();
#endif
   }

	/* stop scroll text */
	coolsand_UI_scrolling_text_stop(&fmgr_p->scroll_text);

#if defined(FMGR_USING_CACHE)

	/* stop all decoding jpeg files */
	#ifndef MMI_ON_WIN32	
		gdi_image_decoder_stop_all();
	#endif /* MMI_ON_WIN32 */

	/* reset delay decoder */
	g_fmgr_context.delay_decode_index = 0;
	StopTimer(FMGR_DELAY_DECODE_0);
	StopTimer(FMGR_DELAY_DECODE_1);
	StopTimer(FMGR_DELAY_DECODE_2);
	StopTimer(FMGR_DELAY_DECODE_3);	
	StopTimer(FMGR_DELAY_DECODE_4);
	StopTimer(FMGR_DELAY_DECODE_5);
	StopTimer(FMGR_DELAY_DECODE_6);
	StopTimer(FMGR_DELAY_DECODE_7);	
	StopTimer(FMGR_DELAY_DECODE_8);		
	StopTimer(FMGR_DELAY_DECODE_9);			

	/* clear image cache handle entries */
	for( i = 0 ; i < FMGR_THUMB_IMAGE_HANDLE_MAX_COUNT; i++)
		fmgr_image_cache_p->image_handle_list[i]->is_used	= FALSE;

	#ifndef MMI_ON_WIN32	
		/* free jpeg decoder */
		if(fmgr_p->is_decoder_create)
		{
			gdi_image_decoder_free();
			fmgr_p->is_decoder_create = FALSE;
		}
	#endif /* MMI_ON_WIN32 */

#endif /* FMGR_USING_CACHE */

	/* stop all animations */
//	gdi_image_stop_animation_all();

}




/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_draw_file_matrix_callback()
* DESCRIPTION
*   draw matrix file item
* PARAMETERS
*   idx					IN		selected index
*   page_start_idx	IN		page's start index
*   page_end_idx		IN		page's end index
*	 is_highlghted 	IN		is this selection highlighted
*	 is_need_update 	IN		is thie item need update
*	 is_new_page 		IN		is first time draw this page
*	 x1					IN		draw region's x1
*	 y1					IN		draw region's y1
*	 x2					IN		draw region's x2
*	 y2					IN		draw region's y2
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_fmgr_draw_file_matrix_callback(	S32 idx, S32 page_start_idx, S32 page_end_idx, 
														BOOL is_hilighted, BOOL is_need_update, BOOL is_new_page,
														S32 x1, S32 y1, S32 x2, S32 y2)
{
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/	
	S32	box_width;
	S32	box_height;
	S32	box_offset_x;
	S32	box_offset_y;	
	S32	image_width;
	S32	image_height;
	PU8	icon_ptr;
	BOOL	is_image_in_cache; 
	BOOL	is_prev_image_in_cache; 	
	BOOL	is_corrupted_file;
	BOOL	is_drm;


#if defined(FMGR_USING_CACHE)
	S32	prev_idx;
	S32	prev_cache_index;	
	S32	cache_index;
#endif /* FMGR_USING_CACHE */

#ifdef __DRM_SUPPORT__   
	FS_HANDLE	file_h;
	S8				buffer_path[FMGR_PATH_BUFFER_SIZE];
#endif 

	FMGR_FILE_INFO_STRUCT 	file_info;

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	if(is_hilighted)
		fmgr_p->cur_index = idx;

	mmi_fmgr_kernel_get_file_info(idx, &file_info);
	
	is_image_in_cache 		= FALSE;
	is_prev_image_in_cache	= FALSE;
	is_corrupted_file			= FALSE;
	is_drm						= FALSE;

	#if defined(FMGR_USING_CACHE)
		/* if not need update, and it is not jpeg, the icon shall already draw */
		if(!is_need_update && 
			!FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPG) &&
			!FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPEG))
		{
			return;
		}

		for(cache_index = 0 ; cache_index < fmgr_image_cache_p->image_cache_item_count; cache_index++)
		{
			if((fmgr_image_cache_p->image_cache_list[cache_index]->is_used == TRUE) &&
				(fmgr_image_cache_p->image_cache_list[cache_index]->menuitem_idx == idx))
			{
				is_image_in_cache = TRUE;
				break;
			}
		}

		/* if this item no need update and is already in cache, no need draw */
		/* if is is_need_update, means hilight is changed or page is changed in cat224 */
		if(!is_need_update && is_image_in_cache)
			return;

		/* if is corrupted file, show as icon */
		if(fmgr_p->file_cache[idx-fmgr_p->cache_start_idx].is_able_display == FALSE)
		{
			is_corrupted_file = TRUE;
		}

		/* if not need update, and it the file is corrupted, the icon shall already draw */
		if(!is_need_update && is_corrupted_file)
			return;

		/* check if previous image is in cache or not */
		/* if previous image not in cache, will remove this image from cache */
		/* 
			this is used to avoid showing images on a page, some images with larger idx 
			are decoded aready, and some image with smaller idx are not decoded 

			if is_need_update == TRUE, means first time show the matrix page.
		*/
		if( (idx > 0) && (idx != page_start_idx) && is_new_page)
		{
			prev_idx = idx-1;
			for(prev_cache_index = 0 ; prev_cache_index < fmgr_image_cache_p->image_cache_item_count; prev_cache_index++)
			{
				if((fmgr_image_cache_p->image_cache_list[prev_cache_index]->is_used == TRUE) &&
					(fmgr_image_cache_p->image_cache_list[prev_cache_index]->menuitem_idx == prev_idx))
				{
					is_prev_image_in_cache = TRUE;
					break;
				}
			}

			if(!is_prev_image_in_cache && is_image_in_cache)
			{
				/* if previous not in cache */
				/* remove this from cache */
				fmgr_image_cache_p->image_cache_list[cache_index]->is_used = FALSE;			
			}
		}
	#endif /* FMGR_USING_CACHE */
	
	/* set index to fm's global index */
	if(is_hilighted)
		fmgr_p->cur_index = idx;


	/* lock and push clip */
	gdi_layer_lock_frame_buffer();
	gdi_layer_push_clip();
	gdi_layer_reset_clip();

	/* spare 1 pixel with boundry */
	box_offset_x 	= x1;
	box_offset_y 	= y1;
	box_width 		= x2-x1+1;
	box_height		= y2-y1+1;
	
	icon_ptr = (PU8)mmi_fmgr_get_large_file_type_icon_image_ptr(&file_info);

	gdi_image_get_dimension(icon_ptr, &image_width, &image_height);

#ifdef __DRM_SUPPORT__  
	if((FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPG) || 
		(FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPEG))))
	{
		mmi_fmgr_kernel_get_path_name((PS8)buffer_path);
		pfnUnicodeStrcat((PS8)buffer_path, (PS8)file_info.file_name);

		file_h = DRM_open_file((PU16)buffer_path, FS_READ_ONLY, DRM_PERMISSION_NONE);
		
		if(file_h >= 0)
		{
			if(DRM_get_object_method(file_h) != DRM_METHOD_NONE)
			{
				/* is drm file */
				is_drm = TRUE;
			}
			DRM_close_file(file_h);		
		}
	}
#endif /* __DRM_SUPPORT__ */

	#if defined(FMGR_USING_CACHE)
		/* draw thumb image */
		if((!is_drm) && 
			(FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPG) || 
			 FMGR_FILTER_IS_SET(&file_info.file_type, FMGR_TYPE_JPEG)))
		{
			mmi_fmgr_show_thumb_image(	idx, 
												box_offset_x, box_offset_y, 
												0, 0, box_width, box_height,
												icon_ptr, is_hilighted, FMGR_UI_MATRIX_STYLE);
		}
		else
		{
			if(is_hilighted)
			{
				gdi_image_draw(	box_offset_x+((box_width-image_width)>>1), 
													box_offset_y+((box_height-image_height)>>1), 
										icon_ptr);
			}
			else
			{
				gdi_image_draw(box_offset_x+((box_width-image_width)>>1), 
									box_offset_y+((box_height-image_height)>>1),	
									icon_ptr);
			}
		}
		
	#else
		if(is_hilighted)
		{
			gdi_image_draw(box_offset_x+((box_width-image_width)>>1), 
												box_offset_y+((box_height-image_height)>>1), 
								icon_ptr);
		}
		else
		{
			gdi_image_draw(box_offset_x+((box_width-image_width)>>1), 
								box_offset_y+((box_height-image_height)>>1), 
								icon_ptr);
		}
	#endif /* FMGR_USING_CACHE */


	gdi_layer_pop_clip();
	gdi_layer_unlock_frame_buffer();	
	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);


	
}


/*****************************************************************************
* FUNCTION
*	 mmi_fmgr_hide_file_matrix_callback()
* DESCRIPTION
*   hide matrix file item
* PARAMETERS
*   idx			IN		selected index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_fmgr_hide_file_matrix_callback(S32 idx)
{
	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
	/*----------------------------------------------------------------*/
	/* Local Variables																*/
	/*----------------------------------------------------------------*/	
#if defined(FMGR_USING_CACHE)
	S32 i;
#endif /* FMGR_USING_CACHE */

	/*----------------------------------------------------------------*/
	/* Code Body																		*/
	/*----------------------------------------------------------------*/
   StopTimer(FMGR_AUDIO_TIMER);

   if (fmgr_p->audio_state == TRUE)
   {
   		fmgr_p->audio_state = FALSE;
#ifdef __MMI_AUDIO_PLAYER__ 
        mdi_audio_stop_file();
#endif
   }

#if defined(FMGR_USING_CACHE)

	/* stop all decoding jpeg files */
	#ifndef MMI_ON_WIN32	
		gdi_image_decoder_stop_all();
	#endif /* MMI_ON_WIN32 */
	
	/* reset delay decoder */
	g_fmgr_context.delay_decode_index = 0;
	
	StopTimer(FMGR_DELAY_DECODE_0);
	StopTimer(FMGR_DELAY_DECODE_1);
	StopTimer(FMGR_DELAY_DECODE_2);
	StopTimer(FMGR_DELAY_DECODE_3);	
	StopTimer(FMGR_DELAY_DECODE_4);
	StopTimer(FMGR_DELAY_DECODE_5);
	StopTimer(FMGR_DELAY_DECODE_6);
	StopTimer(FMGR_DELAY_DECODE_7);
	StopTimer(FMGR_DELAY_DECODE_8);
	StopTimer(FMGR_DELAY_DECODE_9);	
	
	/* clear image cache handle entries */
	for( i = 0 ; i < FMGR_THUMB_IMAGE_HANDLE_MAX_COUNT; i++)
		fmgr_image_cache_p->image_handle_list[i]->is_used	= FALSE;

	#ifndef MMI_ON_WIN32	
		/* free jpeg decoder */
		if(fmgr_p->is_decoder_create)
		{
			gdi_image_decoder_free();
			fmgr_p->is_decoder_create = FALSE;			
		}
	#endif /* MMI_ON_WIN32 */

#endif /* FMGR_USING_CACHE */

	/* stop all animations */
//	gdi_image_stop_animation_all();

}


#endif


