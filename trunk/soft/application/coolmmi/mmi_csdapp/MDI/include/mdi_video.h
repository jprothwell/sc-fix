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
 *	 mdi_video.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	 Interface to access video related functions.
 *
 * Author:
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi_CSDapp/MDI/include/mdi_video.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 11:05:56   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:45:50   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:31:16   zhangxb
 * Initial revision.
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef _MDI_VIDEO_H
#define _MDI_VIDEO_H

#ifndef _MDI_DATATYPE_H_
#error "Please include mdi_datatype.h before mdi_video.h"
#endif 

#ifdef MMI_ON_HARDWARE_P
	#include "med_api.h"
#endif /* MMI_ON_HARDWARE_P */


/***************************************************************************** 
* Define
*****************************************************************************/
#ifdef MMI_ON_HARDWARE_P
	/* Parameters settings */
	#define MDI_VIDEO_EV_P4								CAM_EV_POS_4_3
	#define MDI_VIDEO_EV_P3								CAM_EV_POS_3_3
	#define MDI_VIDEO_EV_P2								CAM_EV_POS_2_3
	#define MDI_VIDEO_EV_P1								CAM_EV_POS_1_3
	#define MDI_VIDEO_EV_0								CAM_EV_ZERO
	#define MDI_VIDEO_EV_N1								CAM_EV_NEG_1_3
	#define MDI_VIDEO_EV_N2								CAM_EV_NEG_2_3
	#define MDI_VIDEO_EV_N3								CAM_EV_NEG_3_3
	#define MDI_VIDEO_EV_N4								CAM_EV_NEG_4_3	
	#define MDI_VIDEO_EV_NIGHT							CAM_EV_NIGHT_SHOT
	#define MDI_VIDEO_EV_COUNT							CAM_NO_OF_EV	

	#define MDI_VIDEO_EFFECT_NOMRAL					CAM_EFFECT_ENC_NORMAL
	#define MDI_VIDEO_EFFECT_GRAYSCALE 				CAM_EFFECT_ENC_GRAYSCALE	
	#define MDI_VIDEO_EFFECT_SEPIA					CAM_EFFECT_ENC_SEPIA
	#define MDI_VIDEO_EFFECT_SEPIA_GREEN			CAM_EFFECT_ENC_SEPIAGREEN
	#define MDI_VIDEO_EFFECT_SEPIA_BLUE				CAM_EFFECT_ENC_SEPIABLUE
	#define MDI_VIDEO_EFFECT_COLOR_INVERT			CAM_EFFECT_ENC_COLORINV
	#define MDI_VIDEO_EFFECT_GRAY_INVERT			CAM_EFFECT_ENC_GRAYINV
	#define MDI_VIDEO_EFFECT_BLACKBOARD				CAM_EFFECT_ENC_BLACKBOARD
	#define MDI_VIDEO_EFFECT_WHITEBOARD				CAM_EFFECT_ENC_WHITEBOARD
	#define MDI_VIDEO_EFFECT_COPPER_CARVING		CAM_EFFECT_ENC_COPPERCARVING
	#define MDI_VIDEO_EFFECT_BLUE_CARVING			CAM_EFFECT_ENC_BLUECARVING
	#define MDI_VIDEO_EFFECT_EMBOSSMENT				CAM_EFFECT_ENC_EMBOSSMENT
	#define MDI_VIDEO_EFFECT_CONTRAST				CAM_EFFECT_ENC_CONTRAST
	#define MDI_VIDEO_EFFECT_JEAN						CAM_EFFECT_ENC_JEAN
	#define MDI_VIDEO_EFFECT_SKETCH					CAM_EFFECT_ENC_SKETCH
	#define MDI_VIDEO_EFFECT_OIL						CAM_EFFECT_ENC_OIL
	#define MDI_VIDEO_EFFECT_COUNT					CAM_NO_OF_EFFECT_ENC

	//add by zhangl
      #define MDI_VIDEO_EFFECT_ANTIQUE                                  CAM_EFFECT_ENC_ANTIQUE
      #define MDI_VIDEO_EFFECT_REDRISH                                 CAM_EFFECT_ENC_REDRISH
      #define MDI_VIDEO_EFFECT_GREENRISH                              CAM_EFFECT_ENC_GREENRISH
      #define MDI_VIDEO_EFFECT_BLUERISH                                 CAM_EFFECT_ENC_BLUERISH
      #define MDI_VIDEO_EFFECT_BLACKWHITE                            CAM_EFFECT_ENC_BLACKWHITE
      #define MDI_VIDEO_EFFECT_NEGATIVENESS                        CAM_EFFECT_ENC_NEGATIVENESS

	#define MDI_CAMERA_EFFECT_MONOCHROME   				VIM_CAM_EFFECT_ENC_MONOCHROME
	#define MDI_CAMERA_EFFECT_NEGATIVE     				VIM_CAM_EFFECT_ENC_NEGATIVE
       #define MDI_CAMERA_EFFECT_REDONLY      					VIM_CAM_EFFECT_ENC_REDONLY
	

	#define MDI_VIDEO_WB_AUTO							CAM_WB_AUTO	
	#define MDI_VIDEO_WB_DAYLIGHT						CAM_WB_DAYLIGHT		
	#define MDI_VIDEO_WB_TUNGSTEN						CAM_WB_TUNGSTEN	
	#define MDI_VIDEO_WB_FLUORESCENT					CAM_WB_FLUORESCENT	
	#define MDI_VIDEO_WB_CLOUD							CAM_WB_CLOUD
	#define MDI_VIDEO_WB_INCANDESCENCE 				CAM_WB_INCANDESCENCE
	#define MDI_VIDEO_WB_COUNT							CAM_NO_OF_WB

	#define MDI_VIDEO_BANDING_50HZ 					CAM_BANDING_50HZ
	#define MDI_VIDEO_BANDING_60HZ 					CAM_BANDING_60HZ
	#define MDI_VIDEO_BANDING_COUNT 					CAM_NO_OF_BANDING

	#define MDI_VIDEO_VIDEO_SIZE_SQCIF				0
	#define MDI_VIDEO_VIDEO_SIZE_QQVGA				1	
	#define MDI_VIDEO_VIDEO_SIZE_QCIF				2
	#define MDI_VIDEO_VIDEO_SIZE_CIF					3

	#define MDI_VIDEO_VIDEO_QTY_FINE					VID_REC_QTY_FINE
	#define MDI_VIDEO_VIDEO_QTY_HIGH					VID_REC_QTY_HIGH
	#define MDI_VIDEO_VIDEO_QTY_NORMAL				VID_REC_QTY_NORMAL
	#define MDI_VIDEO_VIDEO_QTY_LOW					VID_REC_QTY_LOW
	#define MDI_VIDEO_VIDEO_QTY_COUNT 				VID_NO_OF_REC_QTY

	#define MDI_VIDEO_VIDEO_FORMAT_3GP				VID_3GP_BIT_STREAM
	#define MDI_VIDEO_VIDEO_FORMAT_MP4				VID_MP4_BIT_STREAM
	#define MDI_VIDEO_VIDEO_FORMAT_COUNT			VID_NO_OF_BIT_STREAM

	#define MDI_VIDEO_PREVIEW_ROTATE_0				CAM_IMAGE_NORMAL
	#define MDI_VIDEO_PREVIEW_ROTATE_90				CAM_IMAGE_ROTATE_90
	#define MDI_VIDEO_PREVIEW_ROTATE_180			CAM_IMAGE_ROTATE_180
	#define MDI_VIDEO_PREVIEW_ROTATE_270			CAM_IMAGE_ROTATE_270
	#define MDI_VIDEO_PREVIEW_ROTATE_0_MIRROR		CAM_IMAGE_MIRROR
	#define MDI_VIDEO_PREVIEW_ROTATE_90_MIRROR	CAM_IMAGE_MIRROR_ROTATE_90
	#define MDI_VIDEO_PREVIEW_ROTATE_180_MIRROR	CAM_IMAGE_MIRROR_ROTATE_180
	#define MDI_VIDEO_PREVIEW_ROTATE_270_MIRROR	CAM_IMAGE_MIRROR_ROTATE_270

	#if defined(ISP_SUPPORT)
		#define MDI_VIDEO_LCD_ROTATE_0				LCD_LAYER_ROTATE_NORMAL
		#define MDI_VIDEO_LCD_ROTATE_90				LCD_LAYER_ROTATE_90
		#define MDI_VIDEO_LCD_ROTATE_180				LCD_LAYER_ROTATE_180
		#define MDI_VIDEO_LCD_ROTATE_270				LCD_LAYER_ROTATE_270
		#define MDI_VIDEO_LCD_ROTATE_0_MIRROR		LCD_LAYER_MIRROR
		#define MDI_VIDEO_LCD_ROTATE_90_MIRROR		LCD_LAYER_MIRROR_ROTATE_90
		#define MDI_VIDEO_LCD_ROTATE_180_MIRROR	LCD_LAYER_MIRROR_ROTATE_180
		#define MDI_VIDEO_LCD_ROTATE_270_MIRROR	LCD_LAYER_MIRROR_ROTATE_270
	#else
		#define MDI_VIDEO_LCD_ROTATE_0				0
		#define MDI_VIDEO_LCD_ROTATE_90				1
		#define MDI_VIDEO_LCD_ROTATE_180				2
		#define MDI_VIDEO_LCD_ROTATE_270				3
		#define MDI_VIDEO_LCD_ROTATE_0_MIRROR		4
		#define MDI_VIDEO_LCD_ROTATE_90_MIRROR		5
		#define MDI_VIDEO_LCD_ROTATE_180_MIRROR	6
		#define MDI_VIDEO_LCD_ROTATE_270_MIRROR	7
	#endif

#else
	/* PC Simulator */
	/* Parameters settings */
	#define MDI_VIDEO_EV_P4								0
	#define MDI_VIDEO_EV_P3								1
	#define MDI_VIDEO_EV_P2								2
	#define MDI_VIDEO_EV_P1								3
	#define MDI_VIDEO_EV_0								4
	#define MDI_VIDEO_EV_N1								5
	#define MDI_VIDEO_EV_N2								6
	#define MDI_VIDEO_EV_N3								7
	#define MDI_VIDEO_EV_N4								8	
	#define MDI_VIDEO_EV_NIGHT							9
	#define MDI_VIDEO_EV_COUNT							10	

	#define MDI_VIDEO_EFFECT_NOMRAL					0
	#define MDI_VIDEO_EFFECT_GRAYSCALE 				1	
	#define MDI_VIDEO_EFFECT_SEPIA					2
	#define MDI_VIDEO_EFFECT_SEPIA_GREEN			3
	#define MDI_VIDEO_EFFECT_SEPIA_BLUE				4
	#define MDI_VIDEO_EFFECT_COLOR_INVERT			5
	#define MDI_VIDEO_EFFECT_GRAY_INVERT			6
	#define MDI_VIDEO_EFFECT_BLACKBOARD				7
	#define MDI_VIDEO_EFFECT_WHITEBOARD				8
	#define MDI_VIDEO_EFFECT_COPPER_CARVING		9
	#define MDI_VIDEO_EFFECT_BLUE_CARVING			10
	#define MDI_VIDEO_EFFECT_EMBOSSMENT				11
	#define MDI_VIDEO_EFFECT_CONTRAST				12
	#define MDI_VIDEO_EFFECT_JEAN						13
	#define MDI_VIDEO_EFFECT_SKETCH					14
	#define MDI_VIDEO_EFFECT_OIL						15
	#define MDI_VIDEO_EFFECT_COUNT					16

	//add by zhangl
      #define MDI_VIDEO_EFFECT_ANTIQUE                                  17
      #define MDI_VIDEO_EFFECT_REDRISH                                 18
      #define MDI_VIDEO_EFFECT_GREENRISH                              19
      #define MDI_VIDEO_EFFECT_BLUERISH                                 20
      #define MDI_VIDEO_EFFECT_BLACKWHITE                            21
      #define MDI_VIDEO_EFFECT_NEGATIVENESS                        22	

	#define MDI_VIDEO_WB_AUTO							0	
	#define MDI_VIDEO_WB_DAYLIGHT						1		
	#define MDI_VIDEO_WB_TUNGSTEN						2	
	#define MDI_VIDEO_WB_FLUORESCENT					3	
	#define MDI_VIDEO_WB_CLOUD							4
	#define MDI_VIDEO_WB_INCANDESCENCE 				5
	#define MDI_VIDEO_WB_COUNT							6

	#define MDI_VIDEO_BANDING_50HZ 					0
	#define MDI_VIDEO_BANDING_60HZ 					1
	#define MDI_VIDEO_BANDING_COUNT 					2

	#define MDI_VIDEO_VIDEO_SIZE_SQCIF				0
	#define MDI_VIDEO_VIDEO_SIZE_QQVGA				1	
	#define MDI_VIDEO_VIDEO_SIZE_QCIF				2
	#define MDI_VIDEO_VIDEO_SIZE_CIF					3

	#define MDI_VIDEO_VIDEO_QTY_FINE					0
	#define MDI_VIDEO_VIDEO_QTY_HIGH					1
	#define MDI_VIDEO_VIDEO_QTY_NORMAL				2
	#define MDI_VIDEO_VIDEO_QTY_LOW					3
	#define MDI_VIDEO_VIDEO_QTY_COUNT 				4

	#define MDI_VIDEO_VIDEO_FORMAT_3GP				0
	#define MDI_VIDEO_VIDEO_FORMAT_MP4				1
	#define MDI_VIDEO_VIDEO_FORMAT_COUNT			2

	#define MDI_VIDEO_PREVIEW_ROTATE_0				0
	#define MDI_VIDEO_PREVIEW_ROTATE_90				1
	#define MDI_VIDEO_PREVIEW_ROTATE_180			2
	#define MDI_VIDEO_PREVIEW_ROTATE_270			3
	#define MDI_VIDEO_PREVIEW_ROTATE_0_MIRROR		4
	#define MDI_VIDEO_PREVIEW_ROTATE_90_MIRROR	5
	#define MDI_VIDEO_PREVIEW_ROTATE_180_MIRROR	6
	#define MDI_VIDEO_PREVIEW_ROTATE_270_MIRROR	7

	#define MDI_VIDEO_LCD_ROTATE_0					0
	#define MDI_VIDEO_LCD_ROTATE_90					1
	#define MDI_VIDEO_LCD_ROTATE_180					2
	#define MDI_VIDEO_LCD_ROTATE_270					3
	#define MDI_VIDEO_LCD_ROTATE_0_MIRROR			4
	#define MDI_VIDEO_LCD_ROTATE_90_MIRROR			5
	#define MDI_VIDEO_LCD_ROTATE_180_MIRROR		6
	#define MDI_VIDEO_LCD_ROTATE_270_MIRROR		7

#endif /* MMI_ON_HARDWARE_P */

/***************************************************************************** 
* Typedef
*****************************************************************************/
typedef struct {
	U16	width;
	U16	height;
	U32	total_frame_count;
	U64	total_time_duration;
	U16	media_type;
       U16	aud_channel;
	U16	aud_sample_rate;
	U16  track;
	
} mdi_video_info_struct;

typedef struct {

	/* preview */	
	U16	wb;					
	U16	ev;					
	U16	banding;
	U16	effect;				
	U16	zoom;		
	U16	brightness;			
	U16	contrast;	
	U16	saturation;
	U16	hue;
	U16	rotate;
	U16	lcd_rotate;	
	U16	night;
	
	/* record */
	U16	video_size;
	U16	video_qty;
	U16	size_limit;
	U16	time_limit;
	U16	record_aud;
	U16	video_format;
			
} mdi_video_setting_struct;

 typedef struct {
	mdi_video_setting_struct	setting;
	U64								record_time;
	U16								rec_state;
	U16								open_seq_num;
	U16								play_seq_num;
	U16								record_seq_num;
	U16								seek_seq_num;
	BOOL								is_play_audio;
	BOOL								is_seeking;
	U16								tvout_prev_owner;
} mdi_video_context_struct;


/* Video track type enum */
typedef enum
{
    MDI_VIDEO_TRACK_NONE,   /* Video has no track in it */
    MDI_VIDEO_TRACK_AV,     /* Video has both audio and video tracks */
    MDI_VIDEO_TRACK_A_ONLY, /* Video has audio track only */
    MDI_VIDEO_TRACK_V_ONLY, /* Video has video track only */
    MDI_VIDEO_TRACK_COUNT   /* Count of video track enum */
} mdi_video_track_enum;

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/
/**** RECORDER ****/
extern MDI_RESULT mdi_video_rec_init(void);
extern MDI_RESULT mdi_video_rec_power_on(void);
extern MDI_RESULT mdi_video_rec_power_off(void);
extern MDI_RESULT mdi_video_rec_record_start(FS_HANDLE filehandle, void (*record_result_callback)(MDI_RESULT));
extern MDI_RESULT mdi_video_rec_record_stop(void);
extern MDI_RESULT	mdi_video_rec_record_pause(void);
extern MDI_RESULT	mdi_video_rec_record_resume(void);
extern MDI_RESULT	mdi_video_rec_preview_start(	gdi_handle						preview_layer_handle,	
																U32								blt_layer_flag,				/* which layer shall be blt by driver */
																U32								preview_layer_flag,			/* preview on which layer */
																mdi_video_setting_struct	*setting_p);					/* video setting */
extern MDI_RESULT	mdi_video_rec_preview_stop(void);
extern MDI_RESULT mdi_video_rec_save_file(S8 *filepath, void (*save_result_callback)(MDI_RESULT));
extern BOOL			mdi_video_rec_has_unsaved_file(S8 *filepath);
extern void 		mdi_video_rec_delete_unsaved_file(S8 *filepath);
extern void	 		mdi_video_rec_get_cur_record_time(U32 *cur_record_time);
extern void		  	mdi_video_rec_update_para_ev(U16 para);
extern void  		mdi_video_rec_update_para_zoom(U16 para);
extern void			mdi_video_rec_update_para_effect(U16 para);
extern void			mdi_video_rec_update_para_wb(U16 para);
extern void			mdi_video_rec_update_para_night(U16 para);
extern void 		mdi_video_rec_update_para_ct(U16 para);

/**** PLAYER ****/
extern MDI_RESULT	mdi_video_ply_init(void);
extern MDI_RESULT	mdi_video_ply_open_file(const S8 *filename, void (*open_result_callback)(MDI_RESULT, mdi_video_info_struct*),void (*videoplayer_drawtime_hdlr)(U32));
extern MDI_RESULT	mdi_video_ply_close_file(void);
extern MDI_RESULT	mdi_video_ply_play(	gdi_handle 	player_layer_handle,
										  			U32		  	blt_layer_flag,								/* which layer shall be blt by driver */
													U32		 	play_layer_flag,
													U16			repeat_count,
													BOOL			is_visual_update,
													U8				audio_path,
													U16			roate,
													S16			play_speed,
													void		  	(*play_finish_callback)(MDI_RESULT));	/* call back when play finied */
extern MDI_RESULT	mdi_video_ply_stop(void);
extern MDI_RESULT	mdi_video_ply_seek(U64 time);
extern MDI_RESULT	mdi_video_ply_seek_and_get_frame(U64 time, gdi_handle	player_layer_handle);
extern MDI_RESULT	mdi_video_ply_snapshot(gdi_handle layer_handle, PS8 file_name);
extern void			mdi_video_ply_get_cur_play_time(U32 *cur_play_time);
extern MDI_RESULT mdi_video_ply_set_lcm_update(BOOL is_update);

extern MDI_RESULT	mdi_video_ply_non_block_seek(U64 time, void (*seek_result_callback)(MDI_RESULT));
extern MDI_RESULT	mdi_video_ply_non_block_seek_and_get_frame(	U64 time, 
																					gdi_handle player_layer_handle,
																					void (*seek_result_callback)(MDI_RESULT));

extern MDI_RESULT	mdi_video_ply_stop_non_block_seek(void);

/**** CLIP ****/
extern MDI_RESULT mdi_video_ply_open_clip_file(const S8 *filename, mdi_video_info_struct *info, BOOL is_aud_on);
extern MDI_RESULT	mdi_video_ply_close_clip_file(void);
extern MDI_RESULT mdi_video_ply_open_clip_id(U16 video_id, mdi_video_info_struct *info, BOOL is_aud_on);
extern MDI_RESULT	mdi_video_ply_close_clip_id(void);
void mdi_video_get_cur_play_time_req(void (*playtime_result_callback)(UINT32));
void mdi_video_get_cur_play_time_block_req(void (*playtime_result_callback)(UINT32));
MDI_RESULT mdi_video_ply_pause(void);
MDI_RESULT mdi_video_ply_fullscreen_display_set(U16 ZoomWidth, U16 ZoomHeight, S16 startX, S16 startY, U16 rotate);
MDI_RESULT mdi_video_rec_record_previewStop(void);
MDI_RESULT mdi_video_display_interface(void);
void clear_vdoRec_result_cb(void);


#endif /* _MDI_VIDEO_H */


