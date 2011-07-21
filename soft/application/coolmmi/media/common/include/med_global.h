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
 *   This file includes primary global definitions of media task.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------

 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#ifndef MED_NOT_PRESENT

#ifndef _MED_GLOBAL_H
#define _MED_GLOBAL_H

#if !defined(MEM_ULC_3216) && !defined(__PROJECT_GALLITE_C01__)
#define __RICH_AUDIO_PROFILE__ /* wufasong added 2007.06.22 since DAF_DECODE is open. */
#endif
/*==== CONSTANTS ==========*/

#define MT6217_SERIES  (defined(MT6217)||defined(MT6218B))
#define MT6219_SERIES  (defined(MT6219)||defined(MT6226)||defined(MT6226M)||defined(MT6227))
#define MT6228_SERIES  (defined(MT6228)||defined(MT6229))

/* for Catcher to inject test messages */
//#define TST_HANDLER
/* for vm logging */
//#define VM_LOG


#if defined( __BT_HFG_PROFILE__ ) 
#define __MED_BT_HFP_MOD__

#if defined(__BT_AUDIO_VIA_SCO__)
#define __MED_BT_AUDIO_VIA_SCO__
#endif

#if defined(__BT_FM_VIA_SCO__)
#define __MED_BT_FM_VIA_SCO__
#endif

#endif /* #if defined( __BT_HFG_PROFILE__ ) && !defined(__BTVCSR_HP__) */

#if defined(__BT_A2DP_PROFILE__) 
#define __MED_BT_A2DP_MOD__
#endif 

#if defined(__MED_BT_HFP_MOD__) || defined(__MED_BT_A2DP_MOD__)
#define __MED_BT_MOD__
#endif 

#if defined(PLUTO_MMI)
#define __MMI_SUPPORT_BACKLIGHT_SYNC__
#define __MMI_SUPPORT_LED_SYNC__
#endif

#if defined(__POC__)
#define __MED_POC_MOD__
#endif

#if !defined(MT6205B) && !defined(MT6208)
#define __L1_SUPPORT_AUDIO_EQUALIZER__
#define __RICH_AUDIO_PROFILE__
#endif
//jiashuo del for jasper 20080102
#if 0
#if defined( WMA_DECODE ) || defined( AAC_DECODE )
#define __AUD_NEW_INTERFACE__
#endif
#endif
//jiashuo del for jasper 20080102
#if defined(MED_LOW)
#define SLIM_AUDIO_PROFILE
#endif

#if defined (CAMERA_MODULE) || defined (ISP_SUPPORT)
#define __MED_CAM_MOD__

#ifdef WEBCAM_SUPPORT
#define __MED_WEBCAM_MOD__
#endif

#if MT6228_SERIES
#define __CAM_AUTO_FOCUS__
#endif
#endif

#if defined(TV_OUT_SUPPORT)
#define __MED_TVO_MOD__
#endif

#if defined (JPG_DECODE) || defined (JPG_ENCODE) || defined (GIF_DECODE)
#define __MED_IMG_MOD__
#endif

#if defined (MP4_CODEC) && defined (ISP_SUPPORT)
#define __MED_VID_MOD__
#endif

#if defined (MJPG_SUPPORT)
#define __MED_MJPG_MOD__
#if defined (DSP_WT_SYN) 
#define __MED_MJPG_AUDIO__
#endif
#endif

#if defined (DSP_WT_SYN) 
#define __MED_MMA_MOD__
#endif

#if defined (FM_RADIO_ENABLE)
#define __MED_FMR_MOD__
#endif

#if defined(VR_ENABLE)
#define __MED_VR_MOD__
#endif

#if defined(VRSI_ENABLE)
#define __MED_VRSI_MOD__
#endif

#ifdef DEDI_AMR_REC
#define DEDI_AMR_REC_BUFFER
#endif
#if defined(__GENERAL_TTS__)
#define __MED_GENERAL_TTS__
#endif

#define SET_VOLUME_WHEN_PLAY 

#define  AUD_IDLE  0
#define  AUD_VM_IDLE                   0
#define  AUD_VM_RECORD                 1
#define  AUD_VM_RECORD_PAUSED          4
#define  AUD_VM_PLAY                      5
#define  AUD_VM_PLAY_PAUSED            6
#define  AUD_VM_PLAY_FINISH            7


/* audio state */
#define  AUD_MEDIA_IDLE             0
#define  AUD_MEDIA_PLAY             30
#define  AUD_MEDIA_PLAY_FINISH      31
#define  AUD_MEDIA_PLAY_PAUSED      32
#define  AUD_MEDIA_RECORD             33
#define  AUD_MEDIA_RECORD_PAUSED      34

/* camera state */
#define CAM_IDLE        0
#define CAM_READY      1
#define CAM_START_PREVIEW      2
#define CAM_PREVIEW      3
#define CAM_POWER_UP     4
#define CAM_CAPTURE      5

/* image state */
#define IMG_IDLE        0
#define IMG_JPG_DECODE      1
#define IMG_JPG_ENCODE      2

/* video state */
#define VID_IDLE                 0
#define VID_PREVIEW                 1
#define VID_RECORD               2
#define VID_RECORD_FINISH      3
#define VID_RECORD_PAUSED     4
#define VID_OPEN_FILE          5
#define VID_FILE_READY        6
#define VID_PLAY_INIT          7
#define VID_PLAY               8
#define VID_AUDIO_PLAY_FINISH       9
#define VID_AUDIO_PLAY_ERROR       10
#define VID_VISUAL_PLAY_FINISH       11 
#define VID_PLAY_PAUSED          12
#define VID_FILE_MERGE          13
#define VID_SEEK                 14
#define VID_GET_IFRAME          15
#define VID_PLAY_STOP_VISUAL          16
#define VID_RECORD_STOP_VISUAL          17
#define VID_DELAY_OPEN                 18
#define VID_RECORD_ABORTING                 19
#define VID_PLAY_ABORTING                 20
#define VID_START_PREVIEW                 21

#define VID_AUDIO_IDLE                 0
#define VID_AUDIO_PLAY_NORMAL         1
#define VID_AUDIO_PLAY_HIGH_SPEED       2
#define VID_AUDIO_PLAY_DUMMY           3
#define VID_AUDIO_RECORD           4
#define VID_AUDIO_RECORD_PAUSED           5

/* Web camera state */
#define WEBCAM_IDLE        0
#define WEBCAM_START_PREVIEW  1
#define WEBCAM_PREVIEW      2
#define WEBCAM_CAPTURE      3
#define WEBCAM_PAUSING      4
#define WEBCAM_PAUSED      5
#define WEBCAM_STOPPING      6

/* TV output state */
#define TVO_IDLE        0
#define TVO_DETECTION  1
#define TVO_READY      2
#define TVO_UPDATE      3


#define INVALID_VM_ID      255
#define MAX_FILE_NAME_LEN  261
#define MAX_VM_LOG_LEN  (200*1024)
#define IMY_DEFAULT_INSTRUMENT   74 // Flute

/* audio ring buffer */
#ifdef SLIM_AUDIO_PROFILE
#define AUD_RING_BUFFER_LEN     (1*1024)
#else
   #ifdef __MED_MJPG_AUDIO__            
   #define AUD_RING_BUFFER_LEN     (8*1024)
   #define MJPG_AUDIO_CHUNK_SIZE    (AUD_RING_BUFFER_LEN*2)
   #else
   #define AUD_RING_BUFFER_LEN     (4*1024)
   #endif
#endif


/******************************************************
 * 2. WEBCAM support
 ******************************************************/
#if defined(WEBCAM_SUPPORT)
    #if MT6238_SERIES
    #define MAX_WEBCAM_BUFFER_SIZE      (300*1024)       
    #else
    #define MAX_WEBCAM_BUFFER_SIZE      (145*1024)       /* (0xC000+12)*3 - Make sure sync with webcam */
    #endif
    #if defined(__SW_JPEG_CODEC_SUPPORT__)
    #define WEBCAM_JPEG_BUFFER_SIZE (640*480)
    #else
    #define WEBCAM_JPEG_BUFFER_SIZE (0)
    #endif
#else /* defined(WEBCAM_SUPPORT) */
    #define MAX_WEBCAM_BUFFER_SIZE      (0)
    #define WEBCAM_JPEG_BUFFER_SIZE     (0)
#endif /* defined(WEBCAM_SUPPORT) */

#define MAX_MED_WEBCAM_EXT_MEM_SIZE (WEBCAM_JPEG_BUFFER_SIZE+MAX_WEBCAM_BUFFER_SIZE)

#ifdef JPG_DECODE
   #if MT6217_SERIES
   #define MAX_IMG_DEC_INT_MEM_SIZE   (40320)   /* to support zoom */   /* Internal decode memory size for hardware engine buffer */
   #define MAX_IMG_DEC_EXT_MEM_SIZE   (4096)       /* External decode memory size for hardware engine buffer */
   #define MAX_IMG_DEC_FILE_BUFFER_LEN   (100*1024) /* Buffer size for multi-run JPG decode. */
   #elif MT6219_SERIES
   #define MAX_IMG_DEC_INT_MEM_SIZE   (150*1024)   /* to support zoom */   /* Internal decode memory size for hardware engine buffer */
   #define MAX_IMG_DEC_EXT_MEM_SIZE   (4096)       /* External decode memory size for hardware engine buffer */
   #define MAX_IMG_DEC_FILE_BUFFER_LEN   (100*1024) /* Buffer size for multi-run JPG decode. */
   #else  
   #define MAX_IMG_DEC_INT_MEM_SIZE   (150*1024)     /* to support zoom */ /* Internal decode memory size for hardware engine buffer */
   #define MAX_IMG_DEC_EXT_MEM_SIZE   (164*1024)       /* External decode memory size for hardware engine buffer */
   #define MAX_IMG_DEC_FILE_BUFFER_LEN   (100*1024) /* Buffer size for multi-run JPG decode. */
   #endif
#else
   #define MAX_IMG_DEC_INT_MEM_SIZE   (0)          /* Internal decode memory size for hardware engine buffer */
   #define MAX_IMG_DEC_EXT_MEM_SIZE   (0)          /* External decode memory size for hardware engine buffer */
   #define MAX_IMG_DEC_FILE_BUFFER_LEN   (0)       /* Buffer size for multi-run JPG decode. */
#endif

#ifdef JPG_ENCODE
   #if MT6219_SERIES
   /* +ven */
	#define MAX_IMG_ENC_INT_MEM_SIZE   (50000)  /* 176x220 */ /* Internal encode memory size for hardware engine buffer */
   /* end +ven */
	#define MAX_IMG_ENC_EXT_MEM_SIZE   (0)          /* External encode memory size for hardware engine buffer */
   #define MAX_IMG_ENC_FILE_BUFFER_LEN   (100*1024) /* File buffer size for JPG encode. */
   #else  
   #define MAX_IMG_ENC_INT_MEM_SIZE   (0)  /* 176x220 */ /* Internal encode memory size for hardware engine buffer */
   #define MAX_IMG_ENC_EXT_MEM_SIZE   (40960)          /* External encode memory size for hardware engine buffer */
   #define MAX_IMG_ENC_FILE_BUFFER_LEN   (100*1024) /* File buffer size for JPG encode. */
   #endif
#else
   #define MAX_IMG_ENC_INT_MEM_SIZE   (0)       /* Internal encode memory size for hardware engine buffer */
   #define MAX_IMG_ENC_EXT_MEM_SIZE   (0)       /* External encode memory size for hardware engine buffer */
   #define MAX_IMG_ENC_FILE_BUFFER_LEN   (0)    /* File buffer size for JPG encode. */
#endif

#ifdef MP4_CODEC
   #if MT6219_SERIES
      #define MAX_VID_DEC_INT_MEM_SIZE   (100*1024)    /* Internal decode memory size for hardware engine buffer */
      #define MAX_VID_DEC_EXT_MEM_SIZE   (235*1024)   /* External decode memory size for hardware engine buffer */
   
      #define MAX_VID_ENC_INT_MEM_SIZE   (165*1024)   /* Internal encode memory size for hardware engine buffer */
      #define MAX_VID_ENC_EXT_MEM_SIZE   (500*1024)   /* External encode memory size for hardware engine buffer */  
   #else  
      #define MAX_VID_DEC_INT_MEM_SIZE   (45*1024)    /* Internal decode memory size for hardware engine buffer */
      #define MAX_VID_DEC_EXT_MEM_SIZE   (1510*1024)   /* External decode memory size for hardware engine buffer */
   
      #define MAX_VID_ENC_INT_MEM_SIZE   (4424)   /* Internal encode memory size for hardware engine buffer */
      #define MAX_VID_ENC_EXT_MEM_SIZE   (1500*1024)   /* External encode memory size for hardware engine buffer */  
   #endif
#else
   #define MAX_VID_DEC_INT_MEM_SIZE   (0)    /* Internal decode memory size for hardware engine buffer */
   #define MAX_VID_DEC_EXT_MEM_SIZE   (0)    /* External decode memory size for hardware engine buffer */
   
   #define MAX_VID_ENC_INT_MEM_SIZE   (0)    /* Internal encode memory size for hardware engine buffer */
   #define MAX_VID_ENC_EXT_MEM_SIZE   (0)    /* External encode memory size for hardware engine buffer */
#endif

#if defined(VRSI_ENABLE)  
#define VRSI_MEM_SIZE (400*1024)
#else
#define VRSI_MEM_SIZE (0)
#endif

#define MEM_OVERHEAD  256

#ifdef ISP_SUPPORT

   /* audio melody buffer length */
   #define AUD_MELODY_BUFFER_LEN     (100*1024)   /* 100K */
   
   #if ( defined(MT6219) || defined(MT6226) || defined(MT6226M)  )
      #define MAX_CAM_FILE_BUFFER_LEN   (750*1024) /* 900KB for 1280x1024 */
      #define MAX_CAM_CAP_INT_MEM_SIZE   (128*1024)  /* 1600x1200*/
      #define MAX_CAM_CAP_EXT_MEM_SIZE   (0)
   #elif (defined(MT6227) )
      #define MAX_CAM_FILE_BUFFER_LEN   (1500*1024) /* 900KB for 1280x1024 */
      #define MAX_CAM_CAP_INT_MEM_SIZE   (128*1024)  /* 1600x1200*/
      #define MAX_CAM_CAP_EXT_MEM_SIZE   (0)
   #else  
      #define MAX_CAM_FILE_BUFFER_LEN   (1536*1024) /* 900KB for 1280x1024 */
      #define MAX_CAM_CAP_INT_MEM_SIZE   (109824)  /* 2048x1536 */
      #define MAX_CAM_CAP_EXT_MEM_SIZE   (0)
   #endif
   #ifdef MP4_CODEC
      #if MT6219_SERIES
      #define MED_INT_MEM_SIZE   (MAX_VID_ENC_INT_MEM_SIZE+MEM_OVERHEAD)
      #define MED_EXT_MEM_SIZE   (MAX_CAM_FILE_BUFFER_LEN+MAX_IMG_DEC_EXT_MEM_SIZE+VRSI_MEM_SIZE+MEM_OVERHEAD)
      #define MAX_PROG_JPG_DEC_EXT_MEM_SIZE   (MAX_CAM_FILE_BUFFER_LEN+MAX_IMG_DEC_EXT_MEM_SIZE-AUD_MELODY_BUFFER_LEN-MAX_IMG_DEC_FILE_BUFFER_LEN)
      #else  
      #define MAX_TVO_BUFFER_SIZE   (1000*1024)
      #define MED_INT_MEM_SIZE   (MAX_CAM_CAP_INT_MEM_SIZE+MEM_OVERHEAD)
      #define MED_EXT_MEM_SIZE   (MAX_CAM_FILE_BUFFER_LEN+MAX_IMG_DEC_EXT_MEM_SIZE+MAX_TVO_BUFFER_SIZE+VRSI_MEM_SIZE+MEM_OVERHEAD)
      #define MAX_PROG_JPG_DEC_EXT_MEM_SIZE   (MAX_CAM_FILE_BUFFER_LEN+MAX_IMG_DEC_EXT_MEM_SIZE-AUD_MELODY_BUFFER_LEN-MAX_IMG_DEC_FILE_BUFFER_LEN)
      #endif
   #else
      #define MED_INT_MEM_SIZE   (MAX_CAM_CAP_INT_MEM_SIZE+MEM_OVERHEAD)
      #define MED_EXT_MEM_SIZE   (MAX_CAM_FILE_BUFFER_LEN+MAX_IMG_DEC_EXT_MEM_SIZE+VRSI_MEM_SIZE+MEM_OVERHEAD)
      #define MAX_PROG_JPG_DEC_EXT_MEM_SIZE   (MAX_CAM_FILE_BUFFER_LEN+MAX_IMG_DEC_EXT_MEM_SIZE-AUD_MELODY_BUFFER_LEN-MAX_IMG_DEC_FILE_BUFFER_LEN)
   #endif 
   
   
#elif defined(CAMERA_MODULE) 

   /* audio melody buffer length */
   #define AUD_MELODY_BUFFER_LEN     (100*1024)   /* 100K */

   #ifdef __MED_MJPG_MOD__
      #ifdef CAMERA_MODULE_WITH_LCD
         #ifdef AIT800
         #define MAX_CAM_FILE_BUFFER_LEN   (800*1024)   /* Image file buffer size for camera capture */
         #else
         #define MAX_CAM_FILE_BUFFER_LEN   (500*1024)   /* Image file buffer size for camera capture */
         #endif
         #define MAX_OSD_BUFFER_LEN       (100*1024) 
      #else
         #ifdef AIT800
         #define MAX_CAM_FILE_BUFFER_LEN   (800*1024)   /* Image file buffer size for camera capture */
         #else
         #define MAX_CAM_FILE_BUFFER_LEN   (500*1024)   /* Image file buffer size for camera capture */
         #endif
         #define MAX_OSD_BUFFER_LEN       (0) 
      #endif
      #define FILE_CACHE_SIZE              (300*1024)
   #else
      #ifdef AIT800
      #define MAX_CAM_FILE_BUFFER_LEN   (800*1024)   /* Image file buffer size for camera capture */
      #else
      #define MAX_CAM_FILE_BUFFER_LEN   (200*1024)   /* Image file buffer size for camera capture */
      #endif
      #ifdef CAMERA_MODULE_WITH_LCD
      #define MAX_OSD_BUFFER_LEN       (100*1024) 
      #else
      #define MAX_OSD_BUFFER_LEN       (0) 
      #endif
   #endif
   #define MED_INT_MEM_SIZE   (MAX_IMG_DEC_INT_MEM_SIZE+MEM_OVERHEAD)
   #define MED_EXT_MEM_SIZE   (MAX_CAM_FILE_BUFFER_LEN+MAX_OSD_BUFFER_LEN+MAX_IMG_DEC_EXT_MEM_SIZE+VRSI_MEM_SIZE+MEM_OVERHEAD)
   #define MAX_PROG_JPG_DEC_EXT_MEM_SIZE   (MAX_CAM_FILE_BUFFER_LEN+MAX_IMG_DEC_EXT_MEM_SIZE-AUD_MELODY_BUFFER_LEN-MAX_IMG_DEC_FILE_BUFFER_LEN)

#else /* no CAMERA_MODULE nor ISP */

   #define MAX_CAM_FILE_BUFFER_LEN   (0)           /* Image file buffer size for camera capture */
   #ifdef SLIM_AUDIO_PROFILE
   #define AUD_MELODY_BUFFER_LEN     (512) /* 100KB */
   #else
   #define AUD_MELODY_BUFFER_LEN     (100*1024) /* 100KB */
   #endif
   /* internal memory size */
   #if defined(MMS_SUPPORT) && defined(JPG_ENCODE)
   #define MED_INT_MEM_SIZE   (MAX_IMG_ENC_INT_MEM_SIZE+MEM_OVERHEAD)
   #elif (defined(DEDI_AMR_REC_BUFFER) && (MAX_IMG_DEC_INT_MEM_SIZE < 6108))
   #define MED_INT_MEM_SIZE   (6108+MEM_OVERHEAD)
   #else
   #define MED_INT_MEM_SIZE   (MAX_IMG_DEC_INT_MEM_SIZE+MEM_OVERHEAD)
   #endif
   /* external memory size */
   #if defined(MMS_SUPPORT) && defined(JPG_ENCODE)
   #define MED_EXT_MEM_SIZE   (750*1024+MEM_OVERHEAD)
   #else
   #define MED_EXT_MEM_SIZE   (MAX_IMG_DEC_EXT_MEM_SIZE+MAX_IMG_ENC_FILE_BUFFER_LEN+AUD_MELODY_BUFFER_LEN+VRSI_MEM_SIZE+MEM_OVERHEAD)
   #endif

   #define MAX_PROG_JPG_DEC_EXT_MEM_SIZE   (MAX_IMG_DEC_EXT_MEM_SIZE+VRSI_MEM_SIZE)

#endif


/*==== MACROS ========*/



/* audio */
#define AUD_ENTER_STATE(_s_) (aud_context_p->state = _s_)
                              

#define AUD_IN_STATE(_s_) (aud_context_p->state == _s_)
#ifdef __MED_CAM_MOD__
/* camera */
#define CAM_ENTER_STATE(_s_)    (cam_context_p->state = _s_)
                              
#define CAM_IN_STATE(_s_) (cam_context_p->state == _s_)
#endif

#ifdef __MED_IMG_MOD__
/* image */
#define IMG_ENTER_STATE(_s_)                           (img_context_p->state = _s_)
                              
#define IMG_IN_STATE(_s_) (img_context_p->state == _s_)
#endif


#if defined (__MED_VID_MOD__) || defined(__MED_MJPG_MOD__)
/* video */
//#define AUDIO_SYNC_WITH_VISUAL /* audio play sync with visual play */
#define VID_ENTER_STATE(_s_)                          (vid_context_p->state = _s_)
#define VID_AUD_ENTER_STATE(_s_)                      (vid_context_p->aud_state = _s_)
                              
#define VID_IN_STATE(_s_) (vid_context_p->state == _s_)
#define VID_AUD_IN_STATE(_s_) (vid_context_p->aud_state == _s_)
#endif

#if defined (__MED_WEBCAM_MOD__) 
/* web camera */
#define WEBCAM_ENTER_STATE(_s_)                    (webcam_context_p->state = _s_)
                              
#define WEBCAM_IN_STATE(_s_) (webcam_context_p->state == _s_)
#endif


#if defined (__MED_TVO_MOD__) 
/* tv output */
#define TVO_ENTER_STATE(_s_)  (tvo_context_p->state = _s_)
                              
#define TVO_IN_STATE(_s_) (tvo_context_p->state == _s_)
#endif

#ifdef __MED_BT_A2DP_MOD__
#define AUD_BT_A2DP_ENTER_STATE(_s_)         (aud_bt_a2dp_ctx.state = _s_)

#define AUD_BT_A2DP_IN_STATE(_s_) (aud_bt_a2dp_ctx.state == _s_)
#endif /* __MED_BT_A2DP_MOD__ */ 
#ifdef __MED_BT_HFP_MOD__
#define AUD_BT_HFP_ENTER_STATE(_s_)            (aud_bt_hfp_ctx.state = _s_)

#define AUD_BT_HFP_IN_STATE(_s_) (aud_bt_hfp_ctx.state == _s_)
#endif /* __MED_BT_HFP_MOD__ */ 
/*==== TYPES =========*/

typedef enum
{
   AUD_MEDIA_PLAY_MODE_NONE,
   AUD_VM_PLAY_AS_RING,
   AUD_VM_PLAY_AS_MEMO,
   AUD_MEDIA_PLAY_AS_RING,
   AUD_MEDIA_PLAY_AS_SONG,
   NUM_OF_MEDIA_PLAY_MODE
   
}
aud_media_play_mode_enum;


typedef enum
{
   AUD_ID,
   AUD_FILE,
   AUD_STRING,
   AUD_FM_RADIO,
       AUD_TTS,
   AUD_UNKNOWN_SOURCE_TYPE
}
aud_source_type_enum;



typedef enum
{
  MED_NVRAM_IDLE,
  MED_POWERON_READING_NVRAM,
  MED_POWERON_READING_NVRAM_FINISH
} 
med_nvram_state_enum;

typedef enum
{
   MMA_PLAYER_IMY=0,
   MMA_PLAYER_SMF_1,
   MMA_PLAYER_SMF_2,
   MMA_PLAYER_SMF_3,
   MMA_PLAYER_SMF_4,
   MMA_PLAYER_TONE_S1,
   MMA_PLAYER_TONE_S2,
   MMA_PLAYER_TONE_S3,
   MMA_PLAYER_TONE_S4,
   MMA_PLAYER_WAV_1,
   MMA_PLAYER_WAV_2,
   MMA_PLAYER_WAV_3,
   MMA_PLAYER_XWAV,
   MMA_PLAYER_AMR,
   MMA_PLAYER_DAF,
   MMA_PLAYER_AAC,
   MMA_PLAYER_AU,
   MMA_NO_OF_PLAYER
}
mma_player_enum;


#endif /* _MED_GLOBAL_H */

#endif /* MED_NOT_PRESENT */

