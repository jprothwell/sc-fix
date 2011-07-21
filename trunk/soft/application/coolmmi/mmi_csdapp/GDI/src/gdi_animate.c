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
 *	 gdi_animate.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  GDI animate handler
 * Author:
 * -------
 * -------
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#include "gdi_internal.h"
#include "globalconstants.h"
#include "ucs2prot.h"
#include "wrappergprot.h"

#ifdef __MMI_TVOUT__
	#include "mdi_datatype.h"
	#include "mdi_tv.h"
#endif

/****************************************************************************
* Local Variable
*****************************************************************************/
static gdi_anim_cntx_struct 	GDI_ANIM;
static U8					gdi_anim_filename[GDI_ANIM_COUNT][	FS_MAX_PATH];
static U32 				gdi_anim_serial;
static U32 				gdi_anim_delay_time;

//qifeifan add for gif play problem
gdi_handle gif_play_handle_tmp = 0;

//qiff added for catch current animation timer
extern U16 currTimerID;

void gdi_anim_init(void)
{
	int i;
	
	// init animation structure
	for(i = 0; i < GDI_ANIM_COUNT; i++)
	{
		GDI_ANIM.anim_list[i].is_used			= GDI_ANIM_FLAG_IS_FREE;
	}
	gdi_anim_serial = 0;
    GDI_ANIM.disable_blt = MMI_FALSE;
}

U32 gdi_anim_get_delay(void)
{
	return gdi_anim_delay_time;
}

void gdi_anim_set_delay(U32 delay)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_anim_get_delay)	
	if(delay==0) delay=1;
	gdi_anim_delay_time = delay;
	GDI_EXIT_CRITICAL_SECTION(gdi_anim_get_delay)	
}
static GDI_RESULT gdi_anim_stop_internal(gdi_handle a_handle)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_anim_stop_internal)	
		
		if(GDI_ANIM.anim_list[a_handle].is_used==GDI_ANIM_FLAG_IS_FREE)
			GDI_RETURN(GDI_FAILED);

		GDI_ANIM.anim_list[a_handle].is_used	= GDI_ANIM_FLAG_IS_FREE;

		StopMyTimer((U16)(GDI_TIMER_ANIM_BASE + a_handle));
		StopMyTimer((U16)(GDI_TIMER_ANIM_CALLBACK_BASE + a_handle));

		if(gdi_image_codecs[GDI_ANIM.anim_list[a_handle].image_identifier].draw_after)
			gdi_image_codecs[GDI_ANIM.anim_list[a_handle].image_identifier].draw_after(
					0,
					GDI_ANIM.anim_list[a_handle].frame_index,
					GDI_ANIM.anim_list[a_handle].offset_x,
					GDI_ANIM.anim_list[a_handle].offset_y,
					GDI_ANIM.anim_list[a_handle].resized_width,
					GDI_ANIM.anim_list[a_handle].resized_height,
					GDI_ANIM.anim_list[a_handle].image_ptr,
					GDI_ANIM.anim_list[a_handle].image_length);
	
		GDI_RETURN(GDI_SUCCEED);
	GDI_EXIT_CRITICAL_SECTION(gdi_anim_stop_internal)

}
static void gdi_anim_callback_timer(void *arg)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_anim_callback_timer)
#ifdef MMI_ON_WIN32		
		S32 index = ((S32)arg) - GDI_TIMER_ANIM_CALLBACK_BASE;
#else
		S32 index = ((S32)currTimerID) - GDI_TIMER_ANIM_CALLBACK_BASE;
#endif
		if(GDI_ANIM.anim_list[index].is_used != GDI_ANIM_FLAG_IS_FREE)
		{
			if(GDI_ANIM.anim_list[index].callback)
			{
				if(GDI_ANIM.anim_list[index].frames == 1)
					GDI_ANIM.anim_list[index].callback(GDI_IMAGE_IS_STILL_IMAGE);
				else
					GDI_ANIM.anim_list[index].callback(GDI_SUCCEED);
			}
			
			if(GDI_ANIM.anim_list[index].is_used == GDI_ANIM_FLAG_IS_STOP_AFTER_CALLBACK)
				gdi_anim_stop_internal(index);
		}
	GDI_EXIT_CRITICAL_SECTION(gdi_anim_callback_timer)
}


/*wuzc Add Start For GDI/GUI Portting Ver:    on 2008-3-18 11:36 */
static void gdi_anim_draw_internal_handler1(void * arg)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_anim_draw_internal_handler)
		gdi_handle a_handle;
		GDI_RESULT	ret = GDI_FAILED;
		gdi_handle	old_layer_handle;
		gdi_handle	old_lcd_handle;
		BOOL		stop_cyclic = FALSE;

#ifdef MMI_ON_WIN32
		a_handle = ((S32)arg) - GDI_TIMER_ANIM_BASE;
#else

		/*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-18 11:13 */
//qiff  modify for gif play problem
		 a_handle = currTimerID -GDI_TIMER_ANIM_BASE;
#endif
		if(a_handle < 0)
			mmi_trace(g_sw_GDI," gdi_anim_draw_internal_handler1 failed~~~~~!!!a_handle should >=0 %d  ",a_handle);
		/*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-18 11:13 */
		
		if(GDI_ANIM.anim_list[a_handle].is_used == GDI_ANIM_FLAG_IS_FREE)
			GDI_RETURN_VOID;

		// push current LCD , and set to new LCD
		gdi_lcd_get_active(&old_lcd_handle);
		gdi_lcd_set_active(GDI_ANIM.anim_list[a_handle].lcd_handle);

		//push the current layer of the new LCD 
		gdi_layer_get_active(&old_layer_handle);
		
		#ifdef __MMI_TVOUT__
		if (GDI_LCD->lcd_type == GDI_LCD_TYPE_TVOUT)
			gdi_layer_set_active(mdi_tvout_get_and_dupe_active_layer());
		else
		#endif
			gdi_layer_set_active(GDI_ANIM.anim_list[a_handle].layer_handle);

		gdi_layer_push_clip();
		gdi_layer_set_clip(	GDI_ANIM.anim_list[a_handle].clipx1,
							GDI_ANIM.anim_list[a_handle].clipy1,
							GDI_ANIM.anim_list[a_handle].clipx2,
							GDI_ANIM.anim_list[a_handle].clipy2);

		// drawing
		{

			if (GDI_ANIM_POS_TYPE_TIME == GDI_ANIM.anim_list[a_handle].pos_type)
			{
				U32 current_time;
				kal_get_time(&current_time);
				GDI_ANIM.anim_list[a_handle].frame_index = (U32)((current_time - GDI_ANIM.anim_list[a_handle].start_frame) * 4.615);
				gdi_anim_delay_time=1;
			}
			ret = gdi_image_codec_draw(
							GDI_ANIM.anim_list[a_handle].src_type,
							GDI_ANIM.anim_list[a_handle].image_ptr, // image_ptr
							GDI_ANIM.anim_list[a_handle].image_identifier, // image_type
							GDI_ANIM.anim_list[a_handle].image_length, // img_size
							GDI_ANIM.anim_list[a_handle].offset_x,
							GDI_ANIM.anim_list[a_handle].offset_y,
							GDI_ANIM.anim_list[a_handle].resized_width,  // no resize is 0
							GDI_ANIM.anim_list[a_handle].resized_height,// no resize is 0
							GDI_ANIM.anim_list[a_handle].img_flag, // flag
							GDI_ANIM.anim_list[a_handle].frame_index);
		}

		
		if(GDI_ANIM.anim_list[a_handle].blt_before_callback != NULL)
		{
			GDI_ANIM.anim_list[a_handle].blt_before_callback(ret);
		}

        if(ret >= 0 && !(GDI_ANIM.anim_list[a_handle].img_flag & GDI_IMAGE_CODEC_FLAG_DISABLE_BLT)) // succeed , we should update the lcd
		{
			int lx,ly;
			lx = gdi_act_layer_info->x_offset - GDI_LAYER_OFFSET;
			ly = gdi_act_layer_info->y_offset - GDI_LAYER_OFFSET;
			
			#ifdef __MMI_TVOUT__
			if (GDI_LCD->lcd_type == GDI_LCD_TYPE_TVOUT)
			{
				mdi_tvout_blt();
			}
			else
			#endif
			{
				int w,h;
				if(GDI_ANIM.anim_list[a_handle].resized_width 
				&& GDI_ANIM.anim_list[a_handle].resized_height)
				{
					w = (int)GDI_ANIM.anim_list[a_handle].resized_width;
					h = (int)GDI_ANIM.anim_list[a_handle].resized_height;
				}
				else
				{
					w = (int)GDI_ANIM.anim_list[a_handle].width;
					h = (int)GDI_ANIM.anim_list[a_handle].height;
				}

				gdi_lcd_repaint(
					lx+GDI_ANIM.anim_list[a_handle].offset_x,
					ly+GDI_ANIM.anim_list[a_handle].offset_y,
					lx+GDI_ANIM.anim_list[a_handle].offset_x+w-1,
					ly+GDI_ANIM.anim_list[a_handle].offset_y+h-1);
			}

		}

        if(GDI_ANIM.anim_list[a_handle].blt_after_callback != NULL)
		{
			GDI_ANIM.anim_list[a_handle].blt_after_callback(ret);
		}

		gdi_layer_pop_clip(); // restore old clip
		gdi_layer_set_active(old_layer_handle); // restore layer of this new LCD
		gdi_lcd_set_active(old_lcd_handle); // restore to old LCD

		if(ret < 0)
		{
			if(GDI_ANIM.anim_list[a_handle].frames == 0 
			&& GDI_ANIM.anim_list[a_handle].frame_index > 0) // first time fail
			{
				GDI_ANIM.anim_list[a_handle].frames = GDI_ANIM.anim_list[a_handle].frame_index;
				gdi_anim_delay_time=1; // re try use very soon timer
				GDI_ANIM.anim_list[a_handle].frame_index --;
			}
			else // there is something wrong
			{
				stop_cyclic=TRUE;
                GDI_ANIM.anim_list[a_handle].frame_index --;

				if(GDI_ANIM.anim_list[a_handle].callback != NULL)
				{
					GDI_ANIM.anim_list[a_handle].callback(GDI_FAILED);
					GDI_ANIM.anim_list[a_handle].callback = NULL;
				}
			}
		}

		if(GDI_ANIM.anim_list[a_handle].frame_index >= GDI_ANIM.anim_list[a_handle].frames-1)
		{
			// if this animator has callback fucntion, the animator will be destroyed after calling the callback func
			if(GDI_ANIM.anim_list[a_handle].callback != NULL)
			{
				StartMyTimerInt((U16)(GDI_TIMER_ANIM_CALLBACK_BASE+a_handle),
						1,
						gdi_anim_callback_timer,TIMER_IS_NO_ALIGNMENT);
			}

			if(GDI_ANIM.anim_list[a_handle].is_draw_once // play once
			|| GDI_ANIM.anim_list[a_handle].frames == 1) // still image
	 		{
                if(GDI_ANIM.anim_list[a_handle].callback != NULL)
				    GDI_ANIM.anim_list[a_handle].is_used = GDI_ANIM_FLAG_IS_STOP_AFTER_CALLBACK;
				stop_cyclic = TRUE;	// stop cyclic playing
			}
			else
			{
				if (GDI_ANIM.anim_list[a_handle].frames && (0 != (GDI_ANIM.anim_list[a_handle].frame_index + 1)))
				{
    				GDI_ANIM.anim_list[a_handle].frame_index = GDI_ANIM.anim_list[a_handle].start_frame;
				    if (GDI_ANIM_POS_TYPE_TIME == GDI_ANIM.anim_list[a_handle].pos_type)
				    {
					    U32 current_time;
					    kal_get_time(&current_time);
					    GDI_ANIM.anim_list[a_handle].start_frame		= current_time;
				    }
				}
				else
				{
				    GDI_ANIM.anim_list[a_handle].frame_index		= 0;
				}
			}
		}
		else
		{
			GDI_ANIM.anim_list[a_handle].frame_index++;
		}

		if(stop_cyclic)
		{
			if(GDI_ANIM.anim_list[a_handle].is_used == GDI_ANIM_FLAG_IS_ANIMATING)
				gdi_anim_stop_internal(a_handle); // and no callback will destory 
		}
		else
		{
			StartMyTimerInt((U16)(GDI_TIMER_ANIM_BASE+a_handle),
								gdi_anim_delay_time,
								gdi_anim_draw_internal_handler1,TIMER_IS_NO_ALIGNMENT);
		}


	GDI_EXIT_CRITICAL_SECTION(gdi_anim_draw_internal_handler)
}
/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-18 11:36 */

static void gdi_anim_draw_internal_handler(void * arg)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_anim_draw_internal_handler)
		gdi_handle a_handle;
		GDI_RESULT	ret = GDI_FAILED;
		gdi_handle	old_layer_handle;
		gdi_handle	old_lcd_handle;
		BOOL		stop_cyclic = FALSE;
		a_handle = ((S32)arg) - GDI_TIMER_ANIM_BASE;
		
		if(GDI_ANIM.anim_list[a_handle].is_used == GDI_ANIM_FLAG_IS_FREE)
			GDI_RETURN_VOID;

		// push current LCD , and set to new LCD
		gdi_lcd_get_active(&old_lcd_handle);
		gdi_lcd_set_active(GDI_ANIM.anim_list[a_handle].lcd_handle);

		//push the current layer of the new LCD 
		gdi_layer_get_active(&old_layer_handle);
		
		#ifdef __MMI_TVOUT__
		if (GDI_LCD->lcd_type == GDI_LCD_TYPE_TVOUT)
			gdi_layer_set_active(mdi_tvout_get_and_dupe_active_layer());
		else
		#endif
			gdi_layer_set_active(GDI_ANIM.anim_list[a_handle].layer_handle);

		gdi_layer_push_clip();
		gdi_layer_set_clip(	GDI_ANIM.anim_list[a_handle].clipx1,
							GDI_ANIM.anim_list[a_handle].clipy1,
							GDI_ANIM.anim_list[a_handle].clipx2,
							GDI_ANIM.anim_list[a_handle].clipy2);

		// drawing
		{

			if (GDI_ANIM_POS_TYPE_TIME == GDI_ANIM.anim_list[a_handle].pos_type)
			{
				U32 current_time;
				kal_get_time(&current_time);
				GDI_ANIM.anim_list[a_handle].frame_index = (U32)((current_time - GDI_ANIM.anim_list[a_handle].start_frame) * 4.615);
				gdi_anim_delay_time=1;
			}
			ret = gdi_image_codec_draw(
							GDI_ANIM.anim_list[a_handle].src_type,
							GDI_ANIM.anim_list[a_handle].image_ptr, // image_ptr
							GDI_ANIM.anim_list[a_handle].image_identifier, // image_type
							GDI_ANIM.anim_list[a_handle].image_length, // img_size
							GDI_ANIM.anim_list[a_handle].offset_x,
							GDI_ANIM.anim_list[a_handle].offset_y,
							GDI_ANIM.anim_list[a_handle].resized_width,  // no resize is 0
							GDI_ANIM.anim_list[a_handle].resized_height,// no resize is 0
							GDI_ANIM.anim_list[a_handle].img_flag, // flag
							GDI_ANIM.anim_list[a_handle].frame_index);
		}

		
		if(GDI_ANIM.anim_list[a_handle].blt_before_callback != NULL)
		{
			GDI_ANIM.anim_list[a_handle].blt_before_callback(ret);
		}

        if(ret >= 0 && !(GDI_ANIM.anim_list[a_handle].img_flag & GDI_IMAGE_CODEC_FLAG_DISABLE_BLT)) // succeed , we should update the lcd
		{
			int lx,ly;
			lx = gdi_act_layer_info->x_offset - GDI_LAYER_OFFSET;
			ly = gdi_act_layer_info->y_offset - GDI_LAYER_OFFSET;
			
			#ifdef __MMI_TVOUT__
			if (GDI_LCD->lcd_type == GDI_LCD_TYPE_TVOUT)
			{
				mdi_tvout_blt();
			}
			else
			#endif
			{
				int w,h;
				if(GDI_ANIM.anim_list[a_handle].resized_width 
				&& GDI_ANIM.anim_list[a_handle].resized_height)
				{
					w = (int)GDI_ANIM.anim_list[a_handle].resized_width;
					h = (int)GDI_ANIM.anim_list[a_handle].resized_height;
				}
				else
				{
					w = (int)GDI_ANIM.anim_list[a_handle].width;
					h = (int)GDI_ANIM.anim_list[a_handle].height;
				}

				gdi_lcd_repaint(
					lx+GDI_ANIM.anim_list[a_handle].offset_x,
					ly+GDI_ANIM.anim_list[a_handle].offset_y,
					lx+GDI_ANIM.anim_list[a_handle].offset_x+w-1,
					ly+GDI_ANIM.anim_list[a_handle].offset_y+h-1);
			}

		}

        if(GDI_ANIM.anim_list[a_handle].blt_after_callback != NULL)
		{
			GDI_ANIM.anim_list[a_handle].blt_after_callback(ret);
		}

		gdi_layer_pop_clip(); // restore old clip
		gdi_layer_set_active(old_layer_handle); // restore layer of this new LCD
		gdi_lcd_set_active(old_lcd_handle); // restore to old LCD

		if(ret < 0)
		{
			if(GDI_ANIM.anim_list[a_handle].frames == 0 
			&& GDI_ANIM.anim_list[a_handle].frame_index > 0) // first time fail
			{
				GDI_ANIM.anim_list[a_handle].frames = GDI_ANIM.anim_list[a_handle].frame_index;
				gdi_anim_delay_time=1; // re try use very soon timer
				GDI_ANIM.anim_list[a_handle].frame_index --;
			}
			else // there is something wrong
			{
				stop_cyclic=TRUE;
                GDI_ANIM.anim_list[a_handle].frame_index --;

				if(GDI_ANIM.anim_list[a_handle].callback != NULL)
				{
					GDI_ANIM.anim_list[a_handle].callback(GDI_FAILED);
					GDI_ANIM.anim_list[a_handle].callback = NULL;
				}
			}
		}

		if(GDI_ANIM.anim_list[a_handle].frame_index >= GDI_ANIM.anim_list[a_handle].frames-1)
		{
			// if this animator has callback fucntion, the animator will be destroyed after calling the callback func
			if(GDI_ANIM.anim_list[a_handle].callback != NULL)
			{
				StartMyTimerInt((U16)(GDI_TIMER_ANIM_CALLBACK_BASE+a_handle),
						1,
						gdi_anim_callback_timer,TIMER_IS_NO_ALIGNMENT);
			}

			if(GDI_ANIM.anim_list[a_handle].is_draw_once // play once
			|| GDI_ANIM.anim_list[a_handle].frames == 1) // still image
	 		{
                if(GDI_ANIM.anim_list[a_handle].callback != NULL)
				    GDI_ANIM.anim_list[a_handle].is_used = GDI_ANIM_FLAG_IS_STOP_AFTER_CALLBACK;
				stop_cyclic = TRUE;	// stop cyclic playing
			}
			else
			{
				if (GDI_ANIM.anim_list[a_handle].frames && (0 != (GDI_ANIM.anim_list[a_handle].frame_index + 1)))
				{
    				GDI_ANIM.anim_list[a_handle].frame_index = GDI_ANIM.anim_list[a_handle].start_frame;
				    if (GDI_ANIM_POS_TYPE_TIME == GDI_ANIM.anim_list[a_handle].pos_type)
				    {
					    U32 current_time;
					    kal_get_time(&current_time);
					    GDI_ANIM.anim_list[a_handle].start_frame		= current_time;
				    }
				}
				else
				{
				    GDI_ANIM.anim_list[a_handle].frame_index		= 0;
				}
			}
		}
		else
		{
			GDI_ANIM.anim_list[a_handle].frame_index++;
		}

		if(stop_cyclic)
		{
			if(GDI_ANIM.anim_list[a_handle].is_used == GDI_ANIM_FLAG_IS_ANIMATING)
				gdi_anim_stop_internal(a_handle); // and no callback will destory 
		}
		else
		{
			StartMyTimerInt((U16)(GDI_TIMER_ANIM_BASE+a_handle),
								gdi_anim_delay_time,
								/*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-18 11:37 */
								gdi_anim_draw_internal_handler1,TIMER_IS_NO_ALIGNMENT);
								/*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-18 11:37 */
		}


	GDI_EXIT_CRITICAL_SECTION(gdi_anim_draw_internal_handler)
}

GDI_RESULT gdi_anim_codec_draw_internal(
						gdi_image_src_enum src_type,
						U8* img_src,U8 img_type,S32 img_size,
						S32 x,S32 y,S32 w,S32 h,U32 flag,
						gdi_handle * handle_ptr,
						U32 frame_pos, BOOL is_draw_once)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_anim_codec_draw_internal)

		U8* data_ptr;
		GDI_RESULT ret;
		const gdi_image_codecs_struct *codec;
		void	(*callback)(GDI_RESULT result);
		void	(*blt_before_callback)(GDI_RESULT result);
		void	(*blt_after_callback)(GDI_RESULT result);
		S32 width,height;

        	flag |= gdi_image_codec_get_flag();
		// clean the global callback variable
		callback = GDI_ANIM.anim_callback;
		GDI_ANIM.anim_callback = NULL; 

		blt_before_callback = GDI_ANIM.anim_blt_before_callback;
		GDI_ANIM.anim_blt_before_callback = NULL; 

		blt_after_callback = GDI_ANIM.anim_blt_after_callback;
		GDI_ANIM.anim_blt_after_callback = NULL; 

		// initialize the return handle
		if(handle_ptr != NULL)
			*handle_ptr = GDI_ERROR_HANDLE;
		
		// parsing the image header/information
		ret = gdi_image_parse_info(src_type,img_src,&data_ptr,&img_type,&img_size);
		if(ret != GDI_SUCCEED) GDI_RETURN(ret);
		
		if(GDI_IMAGE_TYPE_INVALID == img_type || img_type >= GDI_IMAGE_TYPE_SUM)
			GDI_RETURN(GDI_IMAGE_ERR_INVALID_IMG_TYPE);

		codec = &gdi_image_codecs[img_type];
		if(src_type == GDI_IMAGE_SRC_FROM_FILE)
			flag |= GDI_IMAGE_CODEC_FLAG_IS_FILE;

		if (flag & GDI_IMAGE_CODEC_FLAG_DRAW_ONCE)
		{
			is_draw_once = TRUE;
		}
		if(codec->get_pos_info ==  NULL) // is still image
		{
         		TRACE_GDI_FUNCTION();
			ret = gdi_image_codec_draw(
							src_type,
							img_src,img_type,img_size,
							x,y,w,h,flag,frame_pos);
			
			if(ret == GDI_SUCCEED) ret = GDI_IMAGE_IS_STILL_IMAGE;
		}
		else // is animation
		{
			TRACE_GDI_FUNCTION();
			ret = gdi_image_codec_get_dimension(
							src_type,
							img_src,img_type,img_size,
							&width,&height,flag);
			if(ret==GDI_SUCCEED)
			{
				int i;
				for(i = 0 ; i < GDI_ANIM_COUNT ; i++)
				if(GDI_ANIM.anim_list[i].is_used == GDI_ANIM_FLAG_IS_FREE)
				{
					mmi_trace(g_sw_GDI, "GDI_ANIM.anim_list[%d].is_used = %d", i, GDI_ANIM.anim_list[i].is_used );

					gdi_anim_serial++;
					GDI_ANIM.anim_list[i].is_used  = GDI_ANIM_FLAG_IS_ANIMATING;
					GDI_ANIM.anim_list[i].serial_id = gdi_anim_serial*GDI_ANIM_COUNT + i;
					GDI_ANIM.anim_list[i].src_type = src_type;
					if(GDI_ANIM.anim_list[i].src_type == GDI_IMAGE_SRC_FROM_FILE)
					{
						GDI_ANIM.anim_list[i].image_ptr		= gdi_anim_filename[i];
						UCS2Strncpy((PS8)GDI_ANIM.anim_list[i].image_ptr, (PS8)img_src, FS_MAX_PATH/ENCODING_LENGTH);
					}
					else
						GDI_ANIM.anim_list[i].image_ptr		= img_src;

					GDI_ANIM.anim_list[i].img_flag			= flag;
					GDI_ANIM.anim_list[i].start_frame		= frame_pos;
					GDI_ANIM.anim_list[i].frame_index		= frame_pos;
					GDI_ANIM.anim_list[i].frames			= 0;
					GDI_ANIM.anim_list[i].offset_x			= x;
					GDI_ANIM.anim_list[i].offset_y			= y;
					GDI_ANIM.anim_list[i].width				= width;
					GDI_ANIM.anim_list[i].height			= height;
					GDI_ANIM.anim_list[i].image_length		= img_size;
					GDI_ANIM.anim_list[i].layer_handle		= (gdi_handle)gdi_act_layer;
					GDI_ANIM.anim_list[i].lcd_handle		= GDI_LCD->act_handle;
					GDI_ANIM.anim_list[i].clipx1			= GDI_LAYER.clipx1;
					GDI_ANIM.anim_list[i].clipy1			= GDI_LAYER.clipy1;
					GDI_ANIM.anim_list[i].clipx2			= GDI_LAYER.clipx2;
					GDI_ANIM.anim_list[i].clipy2			= GDI_LAYER.clipy2;
					GDI_ANIM.anim_list[i].image_identifier	= img_type;
					GDI_ANIM.anim_list[i].callback			= callback;
					GDI_ANIM.anim_list[i].blt_before_callback			= blt_before_callback;
					GDI_ANIM.anim_list[i].blt_after_callback			= blt_after_callback;
					GDI_ANIM.anim_list[i].is_draw_once 		= is_draw_once;
					GDI_ANIM.anim_list[i].resized_width		= w;
					GDI_ANIM.anim_list[i].resized_height	= h;
					GDI_ANIM.anim_list[i].pos_type		    = GDI_ANIM_POS_TYPE_INDEX;
					//gdi_image_gif_get_pos_info_handler
					ret = codec->get_pos_info(flag,data_ptr,img_size,TRUE, (S32*)&(GDI_ANIM.anim_list[i].frames),&(GDI_ANIM.anim_list[i].pos_type));
					mmi_trace(g_sw_GDI, "GDI_ANIM.anim_list[%d].pos_type = %d", i, GDI_ANIM.anim_list[i].pos_type );
					if (GDI_ANIM_POS_TYPE_TIME == GDI_ANIM.anim_list[i].pos_type)
					{
						U32 current_time;
						kal_get_time(&current_time);
						GDI_ANIM.anim_list[i].start_frame		= current_time;
						GDI_ANIM.anim_list[i].frame_index		= current_time;
						if (ret)
						{
						    break;
						}
					}

					// start animation cyclic
					gdi_layer_lock_frame_buffer();
						// first frame need draw_before(), draw()
                       		 if (GDI_ANIM.disable_blt)
                       		 {
    						GDI_ANIM.anim_list[i].img_flag |= GDI_IMAGE_CODEC_FLAG_DISABLE_BLT;
                        		 }
					GDI_ANIM.anim_list[i].img_flag		|=GDI_IMAGE_CODEC_FLAG_DISABLE_DRAW_AFTER;
					gdi_anim_draw_internal_handler((void*)(i + GDI_TIMER_ANIM_BASE));
					
					// then frames only need draw()
					GDI_ANIM.anim_list[i].img_flag		|=GDI_IMAGE_CODEC_FLAG_DISABLE_DRAW_BEFORE;
					
					gdi_layer_unlock_frame_buffer();

					// if this image is animation, frame_index should be start_frame+1
		                    if (0 == GDI_ANIM.anim_list[i].frame_index && 1 != GDI_ANIM.anim_list[i].frames)
		                    {
		                        ret = GDI_FAILED;
		                    }
					else if(GDI_ANIM.anim_list[i].frame_index==frame_pos) 
					{
						ret = GDI_IMAGE_IS_STILL_IMAGE;
					}
					else
					{
						if(handle_ptr != NULL)
							*handle_ptr = (gdi_handle)GDI_ANIM.anim_list[i].serial_id;
						ret = GDI_SUCCEED;
					}
					break;
				}
			}
		}
        GDI_ANIM.disable_blt = MMI_FALSE;
		GDI_RETURN(ret);
	GDI_EXIT_CRITICAL_SECTION(gdi_anim_codec_draw_internal)
}



/**************************************************************

	FUNCTION NAME		: gdi_image_stop_animation_all

  	PURPOSE				: stop all animation

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: GDI_RESULT

 

***************************************************************/
GDI_RESULT gdi_anim_stop_all(void)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_image_stop_animation_all)
		gdi_handle act_lcd_handle;
		int index;

		// get current active lcd
		gdi_lcd_get_active(&act_lcd_handle);
		for(index = 0;index < GDI_ANIM_COUNT;index++)
		{
			if(GDI_ANIM.anim_list[index].is_used != GDI_ANIM_FLAG_IS_FREE)
			{
				if(GDI_ANIM.anim_list[index].lcd_handle == act_lcd_handle)
					gdi_anim_stop_internal(index);
			}
		}
		GDI_RETURN(GDI_SUCCEED);
	GDI_EXIT_CRITICAL_SECTION(gdi_image_stop_animation_all)
}

/**************************************************************

	FUNCTION NAME		: gdi_anim_stop_by_layer

  	PURPOSE				: stop all animation in spec layer

	INPUT PARAMETERS	: layer

	OUTPUT PARAMETERS	: nil

	RETURNS				: GDI_RESULT

 

***************************************************************/
GDI_RESULT gdi_anim_stop_all_by_layer(gdi_handle layer)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_anim_stop_all_by_layer)
		int index;

		for(index = 0;index < GDI_ANIM_COUNT;index++)
		{
			if(GDI_ANIM.anim_list[index].is_used != GDI_ANIM_FLAG_IS_FREE)
			{
				if(GDI_ANIM.anim_list[index].layer_handle == layer)
					gdi_anim_stop_internal(index);
			}
		}
		GDI_RETURN(GDI_SUCCEED);
	GDI_EXIT_CRITICAL_SECTION(gdi_anim_stop_all_by_layer)
}


/**************************************************************

	FUNCTION NAME		: gdi_image_stop_animation

  	PURPOSE				: stop a animation by handle

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: GDI_RESULT

 

***************************************************************/
GDI_RESULT gdi_anim_stop(gdi_handle serial_handle)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_anim_stop)
		
		U32 a_handle = serial_handle % GDI_ANIM_COUNT;
		if(  GDI_ANIM.anim_list[a_handle].is_used != GDI_ANIM_FLAG_IS_FREE
		&&  GDI_ANIM.anim_list[a_handle].serial_id == serial_handle)
			GDI_RETURN(gdi_anim_stop_internal(a_handle));
		else
			GDI_RETURN(GDI_FAILED);
		
	GDI_EXIT_CRITICAL_SECTION(gdi_anim_stop)
}

GDI_RESULT gdi_anim_set_callback(void (*last_frame_callback_ptr)(GDI_RESULT result), void (*blt_before_callback_ptr)(GDI_RESULT result), void (*blt_after_callback_ptr)(GDI_RESULT result))
{
	GDI_ENTER_CRITICAL_SECTION(gdi_anim_set_callback)
		GDI_ANIM.anim_callback = last_frame_callback_ptr;
		GDI_ANIM.anim_blt_before_callback = blt_before_callback_ptr;
		GDI_ANIM.anim_blt_after_callback = blt_after_callback_ptr;
		GDI_RETURN(GDI_SUCCEED);
	GDI_EXIT_CRITICAL_SECTION(gdi_anim_set_callback)
}

GDI_RESULT gdi_anim_set_last_frame_callback(void (*callback_ptr)(GDI_RESULT result))
{
	GDI_ENTER_CRITICAL_SECTION(gdi_anim_set_last_frame_callback)
		GDI_RETURN(gdi_anim_set_callback(callback_ptr, NULL, NULL));
	GDI_EXIT_CRITICAL_SECTION(gdi_anim_set_last_frame_callback)
}

GDI_RESULT gdi_anim_set_blt_before_callback(void (*callback_ptr)(GDI_RESULT result))
{
	GDI_ENTER_CRITICAL_SECTION(gdi_anim_set_last_frame_callback)
		GDI_RETURN(gdi_anim_set_callback(NULL, callback_ptr, NULL));
	GDI_EXIT_CRITICAL_SECTION(gdi_anim_set_last_frame_callback)
}

GDI_RESULT gdi_anim_set_blt_after_callback(void (*callback_ptr)(GDI_RESULT result))
{
	GDI_ENTER_CRITICAL_SECTION(gdi_anim_set_last_frame_callback)
		GDI_RETURN(gdi_anim_set_callback(NULL, NULL, callback_ptr));
	GDI_EXIT_CRITICAL_SECTION(gdi_anim_set_last_frame_callback)
}

GDI_RESULT gdi_anim_set_disable_blt(void)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_anim_set_last_frame_callback)
        GDI_ANIM.disable_blt = MMI_TRUE;
		GDI_RETURN(GDI_SUCCEED);
	GDI_EXIT_CRITICAL_SECTION(gdi_anim_set_last_frame_callback)
}


