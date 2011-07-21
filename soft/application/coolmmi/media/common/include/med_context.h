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
 *	med_global.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes global context definitions of media task.
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#ifndef MED_NOT_PRESENT

#ifndef _MED_CONTEXT_H
#define _MED_CONTEXT_H
#include "med_struct.h"
#include "custom_nvram_editor_data_item.h"
#include "l1audio.h"
#include "stack_timer.h"
#include "kal_release.h"

#include "stacklib.h"
#include "lcd_dll.h"
#include "med_global.h"  
#include "custom_nvram_editor_data_item.h"
#include "med_struct.h" 
#include "aud_defs.h"
#include "l1audio.h"
#include "med_api.h"
#include "event_shed.h"

/* MMA struct */
typedef struct
{
   kal_bool    used;
   kal_int32   handle;
   kal_int32   handler_func;
   kal_uint16*  data_p;
   void*       param_p;
   kal_int16*  buf_p;
}
aud_player_struct;

/* Audio Context Struct */
typedef struct
{
   kal_uint16  my_mod_id;
   kal_uint8   state;
   kal_uint8   src_id;
   module_type   src_mod;
   kal_bool    startup;
   /* for audio block, to switch between MMI and Java */
   module_type   access_mod;
   kal_uint8   block_level;

   kal_eventgrpid aud_event;
   kal_eventgrpid mma_event;
   #ifdef __MED_VRSI_MOD__
   kal_eventgrpid vrsi_event;
   #endif
#ifdef __MED_GENERAL_TTS__
    kal_eventgrpid tts_event;
#endif 

   kal_semid  media_sem;
   kal_bool    media_playing;
   aud_source_type_enum source_type;
   kal_uint8   audio_id;
   kal_uint8   play_style;
   kal_uint8*  string_p;
   kal_uint16  identifier;
   
   kal_bool    vibrator_enabled;
   kal_bool    vibrator_on;
   kal_bool    led_on;
   kal_bool    backlight_on;

   kal_bool    vibrator_on_stored;
   kal_bool    led_on_stored;
   kal_bool    backlight_on_stored;

   kal_uint8    min_ec_level;
   kal_uint8    max_ec_level;

   kal_uint8    min_fixed_imy_id;
   kal_uint8    max_fixed_imy_id;

   kal_uint8    min_fixed_midi_id;
   kal_uint8    max_fixed_midi_id;

   kal_uint8    min_fixed_sound_id;
   kal_uint8    max_fixed_sound_id;

   kal_uint8    min_fixed_message_sound_id;
   kal_uint8    max_fixed_message_sound_id;

   kal_uint8    *melody_string_p;

#ifdef SLIM_AUDIO_PROFILE
   kal_wchar   current_file_name[128];
#else
   kal_wchar   current_file_name[256];
#endif
   kal_int32   current_file_handle;
   kal_int16   current_format;

   kal_uint16*  ring_buf;
   kal_uint16*  buf_p;
   kal_uint32   buf_len;
   kal_uint32  offset;
   kal_bool    processing_data;
   kal_bool    audio_end;           /* For streaming play to end of file(last fetch) */

   kal_bool speech_on;

   /* from uem for acoustic data */
   kal_uint8   audio_mode;   
   kal_uint8   func_id;
   custom_acoustic_struct      acoustic_data;

   /* Audio parameters */
   audio_param_struct audio_param;

   kal_bool    tone_playing;
   kal_uint8   melody_output_device;


   /* for multiple play */
#ifdef __MED_MMA_MOD__
   kal_uint32  mplay_flag;
   aud_player_struct player[MMA_NO_OF_PLAYER]; 
   kal_bool    midi_device_opening;
   kal_int32   midi_device_handle;
   
#endif

   kal_uint8   play_mode;
   kal_bool    disc_full;

#ifdef DEDI_AMR_REC_BUFFER
   void*       amr_buffer_p;
   kal_uint32   amr_buffer_size;
#endif


#ifdef VM_SUPPORT
   aud_vm_local_info_struct vm_info;
   kal_uint8   vm_rec_dir;
   kal_uint8   vm_sp_type;
   kal_bool    delete_all;

#ifdef VM_LOG
   kal_bool    vm_logging;
   kal_uint8   vm_id;
#endif

#endif /* VM_SUPPORT */

   /* for real resume mechanism */
   kal_bool is_data_consumed;
   kal_uint32 last_seq_no;
   kal_uint32  last_offset;
   kal_int32 last_time;
   Media_Handle* last_handle;

   /* for play file portion */
   kal_uint32 start_offset;
   kal_uint32 end_offset;

   /*For record input source*/
   kal_uint8 input_device;
   kal_bool use_default_input;

   kal_bool audio_mute;
}
aud_context_struct;

#ifdef __MED_CAM_MOD__

typedef struct
{
   kal_uint8   state;                           /* Camera module state */
   module_type   src_mod;                    /* the source module of request */
   kal_eventgrpid cam_event;                  /* the event group of camera module */

   kal_uint8   media_mode;                   /* media mode, FILE, ARRAY, STREAM */
   kal_int32   file_handle;                     /* the file handle of the file */
   kal_wchar*   file_name_p;                     /* the file name pointer */
   kal_uint32*   file_size_p;                     /* the file size pointer for capture result */
   kal_uint16  seq_num;                         /* sequence num */

   kal_uint8    source_device;
   kal_uint8   image_quality;                   /* image quality */
   kal_int16   lcd_rotate;                      /* LCD rotate value */

#if defined (CAMERA_MODULE)
	kal_bool    delay_power_down;					    /* delay power down flag, True: delay power down is on */
   kal_uint16   power_up_check_conut;             /* the power up retry count in case of camera module failed */
   kal_uint16   preview_check_conut;             /* the preview check count in case of camera module failed */
   kal_uint16   preview_retry_conut;             /* the preview retry count in case of camera module failed */
   kal_uint16   capture_check_conut;             /* the capture check count in case of camera module failed */
   kal_uint16   capture_retry_conut;             /* the capture retry count in case of camera module failed */
#endif

   kal_uint32   intmem_start_address;         /* internal memory start address for hardware engine buffer */
   kal_uint32   intmem_size;                  /* internal memory size for hardware engine buffer */
   kal_uint32   extmem_start_address;         /* external memroy start address for hardware engine buffer */
   kal_uint32   extmem_size;                  /* external memory size for hardware engine buffer */

   kal_int16  flash_mode;    		      /* flash, LED light */
   kal_int16   zoom_factor;				/* digital zoom factor from 10(1X) ~ 40 (4X) */
   kal_uint8   image_mirror;				/* horizonal/vertical mirror from image sensor */
   kal_uint8   contrast_level;			/* ISP contrast gain level */
   kal_uint8   brightness_level;			/* ISP brightness level */
   kal_uint8   saturation_value;			/* image processor saturation value */
   kal_uint8   wb_mode;						/* white balance mode */
   kal_uint8   ev_value;					/* EV adjust value */
   kal_uint8   banding_freq;				/* Banding frequency selection */
   kal_uint8   image_effect;				/* image effect for preview */
   kal_bool    night_mode;					/* enable/disable camera night mode */
   kal_uint16  hue_value;					/* image processor hue value */
   kal_uint8   frame_rate;				   /* frame rate */

   /* preview screen parameters */
   kal_uint16   preview_width;         /* preview screen width */
   kal_uint16   preview_height;        /* preview screen height */
   kal_uint16   preview_offset_x;      /* preview screen offset x */
   kal_uint16   preview_offset_y;      /* preview screen offset y */
   kal_uint32   frame_buffer_p;        /* pointer of the preview image frame buffer */
   kal_uint32   int_frame_buffer_p;         
   kal_uint32   frame_buffer_size;      /* size of the preview image frame buffer */

   kal_uint32   capture_buffer_p;        /* pointer of the capture image buffer */
   void**   capture_buffer_pp;        /* pointer of the capture image buffer */
   kal_uint8   snapshot_number;         /* snapshot number */
   jpegs_struct jpeg_file_info;
   jpegs_struct *jpeg_file_info_p;
   /* lcd layer parameters */
   kal_uint8   lcd_id;                 /* LCD ID */
   kal_uint16  lcd_start_x;            /* the start x axis of LCM to be updated */
   kal_uint16  lcd_start_y;            /* the start y axis of LCM to be updated */
   kal_uint16  lcd_end_x;             /* the end x axis of LCM to be updated */
   kal_uint16  lcd_end_y;              /* the end y axis of LCM to be updated */
   kal_uint16  roi_offset_x;            /* the ROI window offset x from main window */
   kal_uint16  roi_offset_y;           /* the ROI window offset y from main window */
   kal_uint32  update_layer;          /* the updated layer */
   kal_uint32  hw_update_layer;       
#ifdef CAMERA_MODULE_WITH_LCD
   kal_uint32  osd_buffer_p;         /* osd buffer, for overlay LCD layers and output to OSD buffer of LCM */       
#endif
   /* LCD Setting */
   kal_uint32   roi_background_color;         /* set the background color */

#if (defined(MT6228)||defined(MT6229))
   /* ISP Setting */
   kal_bool    manual_wb;                     /* KAL_TRUE or KAL_FALSE to enable or disable manual white balance */
   kal_uint8   ae_metering_mode;               /* AUTO, SPOT, CENTRAL WEIGHT, AVERAGE */
   kal_uint8   iso_setting;                  /* AUTO, 100, 200, 400 */
   kal_uint8	scene_mode;							/* NORMALSCENE, PORTRAIT, LANDSCAPE, SPORT, FLOWER, NIGHTSCENE */
   /* IPP Setting */
   kal_bool   image_pitch_mode;               /* KAL_TRUE or KAL_FALSE for block mode of decoded image */
   kal_uint8   image_data_format;            /* IMGDMA_IBW_OUTPUT_RGB565 or IMGDMA_IBW_OUTPUT_RGB888 */
   kal_uint16   image_pitch_bytes;            /* pitch width */

   kal_bool    overlay_frame_mode;            /* KAL_TRUE or KAL_FALSE to enable or disable overlay function */
   kal_uint8   overlay_color_depth;            /* The color depth of overlay frame buffer */
   kal_uint8   overlay_frame_source_key;
   kal_uint16  overlay_frame_width;            /* Image width of overlay frame buffer */
   kal_uint16  overlay_frame_height;         /* Image height of overlay frame buffer */
   kal_uint32  overlay_frame_buffer_address;   /* The starting address of overlay frame buffer */
   /* TV Setting */
   kal_bool     tv_output;
   kal_uint16   tv_output_width;               /* image width for TV output */
   kal_uint16   tv_output_height;               /* image height for TV output */
   kal_uint32    tv_output_buffer1_address;      /* image buffer1 for TV source buffer */
   kal_uint32    tv_output_buffer2_address;      /* image buffer2 for TV source buffer */
   kal_uint32    tv_output_buffer_size;         /* image buffer size for TV source buffer */
#endif
}
cam_context_struct;

#endif


#ifdef __MED_IMG_MOD__
typedef struct
{
   kal_uint8   state;                         /* image module state */
   module_type   src_mod;                  /* the source module of request */
   kal_eventgrpid img_event;                  /* the event group of image module */

   kal_uint8   media_mode;                   /* the media mode like file, array, stream */
   kal_uint8   media_type;                    /* JPG, GIF, ... */
   kal_int32   file_handle;                     /* the file handle of the file */
   kal_wchar*   file_name_p;                     /* the file name pointer */
   kal_uint32   *file_size_p;                   /* file size pointer after encoding */

   kal_uint8   retry_conut;                    /* the retry count for decode/encode in case of hardware failed */
   kal_bool   processing;                    /* flag to indicate the decoding process is on */

   kal_uint16   seq_num;                     /* sequence num from the request */
   kal_uint32   file_buffer_address;            /* the start address of file located */
   kal_uint32   file_buffer_size;               /* the buffer size of jpeg file */
   kal_uint32   file_size;                      /* the size of file to be decoded or encoded */
   kal_uint32   intmem_start_address;         /* internal memory start address for hardware engine buffer */
   kal_uint32   intmem_size;                  /* internal memory size for hardware engine buffer */
   kal_uint32   extmem_start_address;         /* external memroy start address for hardware engine buffer */
   kal_uint32   extmem_size;                  /* external memory size for hardware engine buffer */
   kal_uint32   image_buffer_address;         /* image buffer address for jpeg decode result */
   kal_uint32   image_buffer_size;            /* image buffer size for jpeg decode result */
   kal_uint16   image_width;                  /* the image width after decoder and resizer */
   kal_uint16   image_height;                  /* the image height after decoder and resizer */
   kal_uint16   display_width;                  /* the display width after decoder and resizer */
   kal_uint16   display_height;                  /* the display height after decoder and resizer */
	kal_uint16  image_clip_x1;						/* clip window start x for clipping decoded image */
	kal_uint16  image_clip_x2;						/* clip window end x for clipping decoded image */
	kal_uint16	image_clip_y1;						/* clip window start y for clipping decoded image */
	kal_uint16	image_clip_y2;						/* clip window end y for clipping decoded image */
   kal_uint8   blocking;                        /* blocking decode or not */
   void      (*callback)(kal_int32 result);
}
img_context_struct;

#endif



#ifdef __MED_VID_MOD__

typedef struct {
   kal_bool    short_header_mode;
   kal_uint32  video_bitstream_size;
   kal_uint32  audio_bitstream_size;
   kal_uint32  video_meta_size;
   kal_uint32  audio_meta_size;
   kal_uint32  video_rm_size;
   kal_uint32  audio_rm_size;
   kal_uint32  file_size_limit;
   kal_uint32  disk_cluster_size;
} video_file_info_struct;


typedef struct
{
   kal_uint8   state;
   kal_uint8   aud_state;
   module_type   src_mod;
   kal_eventgrpid vid_event;

   kal_uint8   media_mode;                   /* the media mode like file, array, stream */
   kal_uint8   media_type;                    /* 3GP, ... */
   kal_int32   file_handle;                     /* the file handle of the file */
   kal_uint32   audio_frame_num;             /* the frame number of audio */
   kal_uint32   total_frame_num;              /* the total frame number of video file */
   kal_uint64   current_time;                  /* the current time duration of the video play */
   kal_uint64   total_audio_time;               /* the total audio time duration of the video file */
   kal_uint64   total_video_time;               /* the total video duration of the video file */
   kal_uint64   total_time;                     /* the total time duration of the video file */
   kal_uint8   video_track;                   /* video track: audio, visual or both */
   kal_uint8   image_path_open;              /* image path open/closed */

   kal_uint8   get_frame;                     /* get frame when seek */
   kal_uint8   encode_quality;                  /* encode quality */
   kal_uint8   bit_stream_type;                 /* bit stream type, 3GP or MP4 */
   kal_uint8   record_audio;                  /* record audio, 1: yes, 0: no */
   kal_uint32   file_size_limit;                /* the limit of the video file size */
   
   kal_uint16   max_width;                  /* the max image width expected for the play */
   kal_uint16   max_height;                  /* the max image height expected for the play */
   kal_bool   use_pseudo_merge;           /* Use pseudo merge or not */
   kal_bool   audio_end;                   /* seek to audio end */
   kal_uint8   open_audio;                   /* open audio, 1: yes, 0: no */
   kal_uint8   play_audio;                   /* play audio, 1: yes, 0: no */
   kal_uint8   display_device;               /* video play display output device */
   kal_uint16   repeats;                      /* play repeats, 0xffff: infinite */
   kal_int16   play_speed;                   /* play speed, normal:100 */
   kal_int16   lcd_rotate;                      /* LCD rotate value */
   
   kal_uint16   seq_num;
   kal_uint32   file_buffer_address;            /* the start address of file located */
   kal_uint32   file_size;                      /* the size of file to be decoded */
   kal_uint32   intmem_start_address;         /* internal memory start address for hardware engine buffer */
   kal_uint32   intmem_size;                  /* internal memory size for hardware engine buffer */
   kal_uint32   extmem_start_address;         /* external memroy start address for hardware engine buffer */
   kal_uint32   extmem_size;                  /* external memory size for hardware engine buffer */
   kal_uint32   image_buffer_address;         /* image buffer address for jpeg decode result */
   kal_uint32   image_buffer_size;            /* image buffer size for jpeg decode result */
   kal_uint16   image_width;                  /* the image width after decoder and resizer */
   kal_uint16   image_height;                  /* the image height after decoder and resizer */
   kal_uint16   display_width;                  /* the display width after decoder and resizer */
   kal_uint16   display_height;                  /* the display height after decoder and resizer */
   kal_uint8   blocking;                        /* blocking decode or not */
   kal_uint8   em_mode;                      /* video engineering mode */

   kal_wchar   current_file_name[MAX_FILE_NAME_LEN];     /* file name for recording */
   kal_wchar   storing_path[MAX_FILE_NAME_LEN];          /* file path for recording */ 

   video_file_info_struct file_size_info;           /* video file size info for recording */
}
vid_context_struct;

#endif

#ifdef __MED_MJPG_MOD__


typedef struct {
   kal_uint32  video_bitstream_size;
   kal_uint32  audio_bitstream_size;
   kal_uint32  file_size_limit;
   kal_uint32  residual_size;
   kal_uint16  frame_size_table[10];
} video_file_info_struct;



typedef struct
{
   kal_uint8   state;
   kal_uint8   aud_state;
   module_type   src_mod;
   kal_eventgrpid vid_event;

   kal_uint8   media_mode;                   /* the media mode like file, array, stream */
   kal_uint8   media_type;                    /* 3GP, ... */
   kal_int32   file_handle;                     /* the file handle of the file */
   kal_int32   current_frame_num;              /* the current frame number of video file */
   kal_int32   display_frame_num;              /* the current displayed frame number of video file */
   kal_uint32   total_frame_num;              /* the total frame number of video file */
   kal_uint64   current_time;                  /* the current time duration of the video play */
   kal_uint64   total_time;                     /* the total time duration of the video file */

   kal_uint8   open_audio;                   /* open audio, 1: yes, 0: no */
   kal_uint8   play_audio;                   /* play audio, 1: yes, 0: no */
   kal_uint8   display_device;               /* video play display output device */
   kal_uint16   repeats;                      /* play repeats, 0xffff: infinite */
   kal_uint8   cache_in_mem;                 /* cache video clip in memory or not */
   kal_uint8   get_frame;                     /* get frame when seek */
   kal_uint8   encode_quality;                  /* encode quality */
   kal_uint32   file_size_limit;                /* the limit of the video file size */
   kal_uint8   image_path_open;              /* image path open/closed */
   kal_uint8   record_audio;                  /* record audio, 1: yes, 0: no */
   
   kal_int16  play_speed;                   /* play speed, normal:100 */
   kal_int16   lcd_rotate;                      /* LCD rotate value */
   kal_uint32   period;                      /* the period(ms) of the video clip */
   kal_uint32   period_fraction;                 /* the period fraction of the video clip */
   kal_uint32   time_elapsed;                 /* the time elapsed during a period of the video clip */
   kal_uint32   frame_rate;                      /* the frame rate fps of the video clip */
   
   kal_uint16   seq_num;
   kal_uint32   file_buffer_address;            /* the start address of file located */
   kal_uint32   file_size;                      /* the size of file to be decoded */
   kal_uint32   intmem_start_address;         /* internal memory start address for hardware engine buffer */
   kal_uint32   intmem_size;                  /* internal memory size for hardware engine buffer */
   kal_uint32   extmem_start_address;         /* external memroy start address for hardware engine buffer */
   kal_uint32   extmem_size;                  /* external memory size for hardware engine buffer */
   kal_uint32   image_buffer_address;         /* image buffer address for jpeg decode result */
   kal_uint32   image_buffer_size;            /* image buffer size for jpeg decode result */
   kal_uint32   decode_buffer_address;         /* intermediate image buffer address for jpeg decode result */
   kal_uint32   decode_buffer_size;            /* intermediate image buffer size for jpeg decode result */
   kal_uint16   image_width;                  /* the image width after decoder and resizer */
   kal_uint16   image_height;                  /* the image height after decoder and resizer */
   kal_uint16   display_width;                  /* the display width after decoder and resizer */
   kal_uint16   display_height;                  /* the display height after decoder and resizer */
   kal_uint8   blocking;                        /* blocking decode or not */
   kal_uint8   em_mode;                      /* video engineering mode */

   kal_wchar   current_file_name[MAX_FILE_NAME_LEN];     /* file name for recording */
   kal_wchar   storing_path[MAX_FILE_NAME_LEN];          /* file path for recording */ 

#ifdef __MED_MJPG_AUDIO__
   aud_player_struct aud_player; 
#endif   
   video_file_info_struct file_size_info;           /* video file size info for recording */
}
vid_context_struct;

#endif

#ifdef __MED_WEBCAM_MOD__

typedef struct
{
   kal_uint8   state;                           /* Camera module state */
    module_type src_mod;            /* the source module of request */
    kal_eventgrpid webcam_event;    /* the event group of wedcam module */
    kal_uint16 seq_num;             /* sequence num */

    kal_uint8 *buffer_addr;
    kal_uint32 buffer_size;
    kal_uint8 image_mirror; /* horizonal/vertical mirror from image sensor */
    kal_uint16 banding;
    kal_uint32 intmem_start_address;    /* internal memory start address for hardware engine buffer */
    kal_uint32 intmem_size;             /* internal memory size for hardware engine buffer */
    kal_uint32 extmem_start_address;    /* external memroy start address for hardware engine buffer */
    kal_uint32 extmem_size;             /* external memory size for hardware engine buffer */

    webcam_default_jpeg_file_info default_jpeg_files;   /* default JPEG files for displaying when interrupt */

#ifdef __SW_JPEG_CODEC_SUPPORT__ 
    kal_uint32 jpeg_y_buf_p;
    kal_uint32 jpeg_y_buf_size;
    kal_uint32 jpeg_u_buf_p;    
    kal_uint32 jpeg_u_buf_size;    
    kal_uint32 jpeg_v_buf_p;        
    kal_uint32 jpeg_v_buf_size;        
#endif /* __SW_JPEG_CODEC_SUPPORT__ */
}
webcam_context_struct;

#endif /* __MED_WEBCAM_MOD__ */ 

#ifdef __MED_TVO_MOD__

typedef struct
{
   kal_uint8   state;                           /* Camera module state */
   module_type   src_mod;                    /* the source module of request */
   kal_eventgrpid tvo_event;                  /* the event group of camera module */
   kal_uint16  seq_num;                         /* sequence num */

   /* TV Setting */
   kal_uint8     tv_output_mode;              /* tv output mode, MED_TVO_MODE1,... */
   kal_uint8     tv_output_format;              /* tv output format, MED_TV_FORMAT_NTSC,... */
   kal_bool     tv_output;
   kal_uint16   tv_output_width;               /* image width for TV output */
   kal_uint16   tv_output_height;               /* image height for TV output */
   kal_uint32    tv_output_buffer1_address;      /* image buffer1 for TV source buffer */
   kal_uint32    tv_output_buffer2_address;      /* image buffer2 for TV source buffer */
   kal_uint32    tv_output_buffer_size;         /* image buffer size for TV source buffer */
   /* LCD Setting */
   kal_uint32   roi_background_color;         /* set the background color */
}
tvo_context_struct;

#endif


/* Media task Context Struct */
typedef struct
{
   kal_uint16  my_mod_id;
   med_nvram_state_enum    nvram_state;

   stack_timer_struct base_timer;
   event_scheduler *event_scheduler_ptr;
   kal_eventgrpid med_event;                  /* the event group of media task */

   void* int_mem_p;
   void* ext_mem_p;
   
   aud_context_struct   aud_context;
#ifdef __MED_IMG_MOD__
   img_context_struct   img_context;
#endif
#ifdef __MED_CAM_MOD__
   cam_context_struct   cam_context;
#endif
#if defined( __MED_VID_MOD__) || defined (__MED_MJPG_MOD__)
   vid_context_struct   vid_context;
#endif
#if defined( __MED_WEBCAM_MOD__) 
   webcam_context_struct   webcam_context;
#endif
#if defined( __MED_TVO_MOD__) 
   tvo_context_struct   tvo_context;
#endif
}
med_context_struct;

/*==== GLOBAL VARIABLES ============*/

/* context of media task */
extern med_context_struct   med_context;
extern med_context_struct*   med_context_p;

/* context of audio manager tast */
extern aud_context_struct*   aud_context_p;
#ifdef __MED_IMG_MOD__
extern img_context_struct*   img_context_p;
#endif
#ifdef __MED_CAM_MOD__
extern cam_context_struct*   cam_context_p;
#endif
#if defined( __MED_VID_MOD__) || defined (__MED_MJPG_MOD__)
extern vid_context_struct*   vid_context_p;
#endif
#if defined( __MED_WEBCAM_MOD__) 
extern webcam_context_struct*   webcam_context_p;
#endif
#if defined( __MED_TVO_MOD__) 
extern tvo_context_struct*   tvo_context_p;
#endif


/* for media common use */
typedef enum
{
   MED_EVT_NOWAIT    = 0,
   MED_EVT_GET_BUFFER      = 0x0001,
   MED_EVT_FREE_BUFFER      = 0x0002
} 
med_wait_event_enum;


#define MED_WAIT_EVENT(evt_) do{ \
         kal_uint32 retrieved_events;\
         kal_retrieve_eg_events(med_context_p->med_event, (evt_), \
         KAL_OR_CONSUME, &retrieved_events, KAL_SUSPEND); }while(0) 


#define MED_SET_EVENT(evt_) do{ \
                              kal_set_eg_events(med_context_p->med_event, (evt_),KAL_OR);\
                              } while(0)


#endif /* _MED_CONTEXT_H */

#endif /* MED_NOT_PRESENT */

