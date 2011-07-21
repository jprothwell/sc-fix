/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
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
 * aud_bt_a2dp.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes BT A2DP service functions
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
#include "kal_release.h"

#include "app_ltlcom.h" /* task message communiction */

#include "stacklib.h"   /* basic type for dll, evshed, stacktimer */
#include "event_shed.h" /* event scheduler */
#include "stack_timer.h"        /* stack timer */

/* global includes */
#include "l1audio.h"
#include "device.h"
#include "resource_audio.h"
#include "fat_fs.h"

#include "nvram_struct.h"
#include "nvram_user_defs.h"
#include "nvram_data_items.h"
#include "custom_nvram_editor_data_item.h"
#include "custom_equipment.h"

/* local includes */
#include "med_global.h"
#include "aud_defs.h"
#include "med_struct.h"
#include "med_api.h"
#include "med_context.h"
#include "med_main.h"
#include "aud_main.h"
#include "med_utility.h"
#include "med_global.h"

#include "mci.h"

#ifdef __AUD_TRACE_ON__
#include "med_trc.h"
#endif 

#if defined(__MMI_FTC_SUPPORT__) || defined(__MMI_OPP_SUPPORT__) || defined(__MED_BT_A2DP_MOD__) 
#define SBC_ENCODE_MEM_SIZE (18*1024)
#define MP3_ENCODE_MEM_SIZE (18*1024)//(4*1024)
kal_uint8 SBC_buf[SBC_ENCODE_MEM_SIZE]; //The obex buffer shared with this buffer,please leave it lager than 14KB
#endif

#ifdef __MED_BT_A2DP_MOD__
#include "bt_a2dp_struct.h"
#include "bt_a2dp.h"

#ifdef __MED_VID_MOD__
#include "vid_main.h"
#endif

#include "mmi_trace.h"

#if defined(__BTCOOLSAND__)


static kal_bool mute_phone;
static kal_uint8 stream_handle;
static kal_bool need_open_codec;
 A2DP_codec_struct *codec;
static kal_uint32 codec_seq_no = 0;
static bt_a2dp_audio_cap_struct audio_config;

static volatile kal_mutexid codec_mutex;

extern aud_context_struct *aud_context_p;
extern kal_uint8 current_playing_melody_volume_level;
#if defined(TV_OUT_SUPPORT)
extern kal_bool audio_tv_cable_in;
extern kal_bool audio_tv_loud_speaker;
#endif

A2DP_codec_struct* aud_bt_a2dp_get_codec(void)
{
    return codec;
}

void aud_bt_a2dp_lock_codec(void)
{
    kal_take_mutex(codec_mutex);
}

void aud_bt_a2dp_unlock_codec(void)
{
    kal_give_mutex(codec_mutex);
}

/*****************************************************************************
 * FUNCTION
 *  aud_bt_a2dp_init
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_a2dp_init(void)
{
    codec = NULL;
    need_open_codec = KAL_FALSE;
    mute_phone = KAL_FALSE;
    codec_mutex = kal_create_mutex("a2dp_codec_mutex");
}

/*****************************************************************************
 * FUNCTION
 *  aud_bt_a2dp_is_mute_phone
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  kal_bool
 *****************************************************************************/
kal_bool aud_bt_a2dp_is_mute_phone(void)
{
    return mute_phone;
}

/*****************************************************************************
 * FUNCTION
 *  aud_bt_a2dp_is_media_playing
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
kal_bool aud_bt_a2dp_is_media_playing(void)
{
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//    AUD_STATE_TRACE(aud_context_p->state);
    
    return (aud_context_p->state == AUD_MEDIA_PLAY || 
        aud_context_p->state == AUD_MEDIA_PLAY_FINISH);
}

/*****************************************************************************
 * FUNCTION
 *  aud_bt_a2dp_is_media_available
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
kal_bool aud_bt_a2dp_is_media_available(void)
{
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//    AUD_STATE_TRACE(aud_context_p->state);

    return (aud_context_p->state == AUD_MEDIA_PLAY || 
        aud_context_p->state == AUD_MEDIA_PLAY_PAUSED ||
        aud_context_p->state == AUD_MEDIA_PLAY_FINISH);
}

/*****************************************************************************
 * FUNCTION
 *  aud_bt_a2dp_set_mute_phone
 * DESCRIPTION
 *  
 * PARAMETERS
 *  on      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
extern   void    GpioSpeakerModeEnd();
extern   void    GpioSpeakerModeStart();
extern   U8 GetHandsetInPhone();
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)
#ifdef AUD_MUTE_FOR_BT_WORKAROUND
extern VOID aud_MuteOutputDevice(BOOL mute);
#endif // AUD_MUTE_FOR_BT_WORKAROUND
#endif // (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)

void aud_bt_a2dp_set_mute_phone(kal_bool on)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
  //  kal_uint8 output_device;
    //kal_uint8 audio_mode;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    mmi_trace(g_sw_BT, "entry aud_bt_a2dp_set_mute_phone::mute_phone= %d,aud_context_p->audio_mode= %d  ", mute_phone,aud_context_p->audio_mode);

    if (on)
    {
        if (!mute_phone)
        {
            mute_phone = TRUE;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)
#ifdef AUD_MUTE_FOR_BT_WORKAROUND
            aud_MuteOutputDevice(TRUE);
#endif // AUD_MUTE_FOR_BT_WORKAROUND
#else // (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE)
            GpioSpeakerModeEnd();
#endif // (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE)
#ifdef TGT_GALLITE_G800
            MusicEndAudioMode();
#endif
        }
    }
    else
    {
        if (mute_phone)
        {		
            mute_phone = FALSE;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)
#ifdef AUD_MUTE_FOR_BT_WORKAROUND
            aud_MuteOutputDevice(FALSE);
#endif // AUD_MUTE_FOR_BT_WORKAROUND
#else // (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE)
            if(0 == GetHandsetInPhone())
            {
                GpioSpeakerModeStart();
            }
#endif // (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE)
#ifdef TGT_GALLITE_G800
        MusicStartAudioMode();
#endif

        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  aud_bt_a2dp_stream_qos_ind_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ilm_ptr     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_a2dp_stream_qos_ind_hdlr(ilm_struct *ilm_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    bt_a2dp_stream_qos_ind_struct *msg_p = (bt_a2dp_stream_qos_ind_struct*) ilm_ptr->local_para_ptr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//    AUD_BT_A2DP_FUNC_ENTRY(AUD_BT_A2DP_STREAM_QOS_IND_HDLR);
    mmi_trace(g_sw_BT, "Entry aud_bt_a2dp_stream_qos_ind_hdlr!!");

    if (codec != NULL)
    {
        codec->AdjustBitRateFromQos(msg_p->qos);
    }
}

/*****************************************************************************
 * FUNCTION
 *  aud_bt_a2dp_codec_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  event       [IN]        
 *  param       [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_a2dp_codec_callback(A2DP_Event event, void *param)
{
    A2DP_codec_struct* codec_p = (A2DP_codec_struct*)param;
    module_type src_mod_id=0;
    
  //  if(kal_if_hisr() != KAL_FALSE)
     //   src_mod_id = MOD_L1SPHISR;
  //  else
  //      src_mod_id = stack_get_active_module_id();
    bt_a2dp_send_stream_data_send_req(src_mod_id, stream_handle, codec_p);
}

/*****************************************************************************
 * FUNCTION
 *  aud_bt_a2dp_open_codec
 * DESCRIPTION
 *  
 * PARAMETERS
 *  audio_format    [IN]
 * RETURNS
 *  void
 *****************************************************************************/

void aud_bt_a2dp_open_codec(kal_int32 audio_format)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_BT, "entry aud_bt_a2dp_open_codec::audio_config.codec_type= %d  need_open_codec= %d", audio_config.codec_type, need_open_codec);

    if (!need_open_codec)
       return;

 
    ASSERT(codec == NULL);
   mmi_trace(g_sw_BT, "entry aud_bt_a2dp_open_codec::audio_config.codec_type= %d", audio_config.codec_type);
   
    switch (audio_config.codec_type)
    {
        case BT_A2DP_SBC:

           codec = MCI_SBCOpen(
                aud_bt_a2dp_codec_callback,
                &audio_config,
                SBC_buf,
                SBC_ENCODE_MEM_SIZE);

            break;
        case BT_A2DP_MP3:
			
            ASSERT(audio_format == MED_TYPE_DAF);

            codec = MCI_A2dpDafOpen(
                aud_bt_a2dp_codec_callback,
                &audio_config,
                SBC_buf,
                MP3_ENCODE_MEM_SIZE);
            break;
        default:
            ASSERT(0);
    }
    need_open_codec = KAL_FALSE;

    aud_bt_a2dp_set_mute_phone(KAL_TRUE);
}

/*****************************************************************************
 * FUNCTION
 *  aud_bt_a2dp_close_codec
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_a2dp_close_codec(void)
{
    kal_bool playing;

    if (codec == NULL)
        return;

    //AUD_BT_A2DP_FUNC_ENTRY(AUD_BT_A2DP_CLOSE_CODEC);

    playing = aud_bt_a2dp_is_media_playing();
    
    aud_bt_a2dp_lock_codec();

    switch (audio_config.codec_type)
    {
        case BT_A2DP_SBC:
           MCI_SBCClose();
            break;
        case BT_A2DP_MP3:
	  MCI_A2dpDafClose();
            break;
        default:
            ASSERT(0);
    }
    codec = NULL;

    aud_bt_a2dp_unlock_codec();

    aud_bt_a2dp_set_mute_phone(KAL_FALSE);
}

/*****************************************************************************
 * FUNCTION
 *  aud_bt_a2dp_close_codec_ind
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_a2dp_close_codec_ind(void)
{
    if (codec != NULL)
    {
        aud_bt_a2dp_close_codec();
    }

    if (need_open_codec)
    {
        aud_send_bt_close_codec_ind(MOD_MMI, stream_handle, codec_seq_no);
        need_open_codec = KAL_FALSE;
    }
}

/*****************************************************************************
 * FUNCTION
 *  aud_bt_a2dp_close_codec_for_open
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
 /*
void aud_bt_a2dp_close_codec_for_open(void)
{
 mmi_trace(g_sw_BT,"aud_bt_a2dp_close_codec_for_open,need_open_codec=%d", need_open_codec);
    if (codec != NULL)
    {
        aud_bt_a2dp_close_codec();
      
    }
  need_open_codec = KAL_TRUE;
	
}
*/
/*****************************************************************************
 * FUNCTION
 *  aud_bt_a2dp_open_codec_req_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ilm_ptr
 * RETURNS
 *  void
 *****************************************************************************/
extern bool mmi_vdoply_aud_with_bt(void);
void aud_bt_a2dp_open_codec_req_hdlr(ilm_struct *ilm_ptr)
{
    media_bt_open_codec_req_struct *msg_p = (media_bt_open_codec_req_struct*)ilm_ptr->local_para_ptr;

    ASSERT(codec == NULL);

    stream_handle = msg_p->stream_handle;
    memcpy(&audio_config, &msg_p->cfg, sizeof(bt_a2dp_audio_cap_struct));
    need_open_codec = KAL_TRUE;
    codec_seq_no = msg_p->seq_no;

   mmi_trace(g_sw_BT,"aud_bt_a2dp_open_codec_req_hdlr::msg_p->immediate= %d,audio_config.code_tye=%d", msg_p->immediate,audio_config.codec_type);
   
    if (msg_p->immediate)
    {
        if (aud_bt_a2dp_is_media_playing())
        {
            aud_bt_a2dp_open_codec(aud_context_p->current_format);
        }
    #ifdef __MED_VID_MOD__
        else if(vid_is_aud_playing())
        {
            aud_bt_a2dp_open_codec(vid_get_aud_format());
        }
    #endif /* __MED_VID_MOD__ */
#if defined(__MMI_VIDEO_PLAYER__)
       else if (mmi_vdoply_aud_with_bt())
       {
          aud_bt_a2dp_open_codec(BT_A2DP_SBC);
       }
#endif
    }
}

/*****************************************************************************
 * FUNCTION
 *  aud_bt_a2dp_close_codec_req_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ilm_ptr
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_a2dp_close_codec_req_hdlr(ilm_struct *ilm_ptr)
{
    if (codec != NULL)
    {
        aud_bt_a2dp_close_codec();
    }
    need_open_codec = KAL_FALSE;
}



#endif

#endif /* __MED_BT_A2DP_MOD__ */

#endif /* MED_NOT_PRESENT */ 

