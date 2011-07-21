/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2006
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

/*****************************************************************************
 *
 * Filename:
 * ---------
 * av_bt.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 * This file implements av app by using Bluetooth A2DP
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#include "mmi_features.h"

#if defined(__MMI_BT_COOLSAND_SUPPORT__) && defined(__MMI_A2DP_SUPPORT__)

/***************************************************************************** 
* Included files
*****************************************************************************/
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "protocolevents.h"
#include "eventsgprot.h"
#include "timerevents.h"

/* Trace Header Files */

#include "debuginitdef.h"       /* define trace class type */


#include "med_struct.h"
#include "med_global.h"
/* a2dp header files */
#include "bt_a2dp_struct.h"
#include "bt_a2dp.h"

#include "btmmiscrgprots.h"

#include "btmmicm.h"
#include "btmmicmgprots.h"
#include "btmmia2dp.h"
#ifdef __MMI_AVRCP_SUPPORT__
#include "btmmiavrcpgprot.h"
#endif

#include "profilesdefs.h"
#include "profilegprots.h"

#include "mdi_datatype.h"
#include "mdi_audio.h"


#include "av_bt.h"
#include "aud_main.h"
#include "mmi_trace.h"
#include "audioplayerprot.h"
#include "med_global.h"
#include "fs_func.h"
#include "mmi_trace.h"
#include "audioplayertype.h"
#include "audioplayerdef.h"
#if !defined(MT6205B) && !defined(MT6208)
#define AMR_DECODE
#endif  

typedef struct
{
    kal_uint8 cap_index;
    kal_uint8 cap_count;
    bt_sep_info_struct info;
    bt_a2dp_audio_cap_struct cap_list[MAX_NUM_REMOTE_CAPABILITIES];
} av_bt_sep_struct;

typedef struct
{
    kal_uint8 sep_index;
    kal_uint8 sep_count;
    av_bt_sep_struct sep_list[MAX_NUM_REMOTE_SEIDS];
    mmi_bt_dev_addr bt_addr;
} av_bt_dev_struct;

typedef struct
{
    void (*headset_selected_callback)(mmi_bt_dev_struct* dev);
    AV_BT_CALLBACK  retry_success_callback; /* this is a temp patch for MT6601 */
    AV_BT_CALLBACK  callback;               /* current callback */
    AV_BT_CALLBACK  default_callbacks[1];   /* if current callback is NULL, use default callback */
    kal_uint32 codec_seq_no;
    mmi_bt_dev_addr bt_addr;
    kal_uint16 connect_id;
    kal_bool is_inquiry;
    kal_bool is_codec_open;
    kal_bool need_stop;
    kal_bool need_reconnect;
    kal_bool need_reconfig;
    kal_bool support_reconfig;
    kal_bool bad_acp_seid_retry; /*to indicate if av_bt has handled bad_acp_seid error casue once*/
    kal_bool bad_state_retry; /* to indicate if av_bt has handled bad_state error casue once*/
    kal_bool sep_ind_use_retry; /* to indicate if av_bt has handled sep_ind_use error casue once*/
    kal_bool connect_post_proc;
    kal_bool hfp_only;	
    kal_uint8 local_seid;
    kal_uint8 state;
    kal_uint8 wait;
    kal_uint8 stream_handle;
    kal_uint8 req_cfg_index;
    av_bt_dev_struct dev;
    bt_a2dp_audio_cap_struct req_cfg[2];
    bt_a2dp_audio_cap_struct tmp_cfg;
    bt_a2dp_audio_cap_struct cfg;
	    kal_bool is_inconhfp;
} av_bt_ctx_struct;

av_bt_ctx_struct av_bt_ctx;

const char HP_Headphones[] = "BT-Headphones";

#define __av_bt_func_proto__

/* open/close functions */
extern kal_bool av_bt_open_dev(kal_bool done_callback);
extern kal_bool av_bt_close_dev(void);
extern void av_bt_start_disc_timer(void);
extern void av_bt_stop_disc_timer(void);

/* signaling functions */
extern void av_bt_connect(void);
extern void av_bt_disconnect(void);
extern kal_bool av_bt_discover_stream(void);
extern kal_bool av_bt_get_stream_cap(void);
extern void av_bt_config_stream(void);
extern void av_bt_open_stream(void);
extern void av_bt_start_stream(void);
extern void av_bt_pause_stream(void);
extern void av_bt_close_stream(void);
extern void av_bt_abort_stream(void);
/* msg hdlrs */
extern void av_bt_connect_cnf(void* msg);
extern void av_bt_connect_ind(void *msg);
extern void av_bt_disconnect_cnf(void *msg);
extern void av_bt_disconnect_ind(void *msg);
extern void av_bt_discover_stream_cnf(void* msg);
extern void av_bt_discover_stream_ind(void *msg);
extern void av_bt_get_stream_cap_cnf(void *msg);
extern void av_bt_get_stream_cap_ind(void *msg);
extern void av_bt_config_stream_cnf(void* msg);
extern void av_bt_config_stream_ind(void *msg);
extern void av_bt_open_stream_cnf(void* msg);
extern void av_bt_open_stream_ind(void* msg);
extern void av_bt_start_stream_cnf(void *msg);
extern void av_bt_start_stream_ind(void *msg);
extern void av_bt_pause_stream_cnf(void *msg);
extern void av_bt_pause_stream_ind(void *msg);
extern void av_bt_abort_stream_cnf(void *msg);
extern void av_bt_abort_stream_ind(void *msg);
extern void av_bt_close_stream_cnf(void *msg);
extern void av_bt_close_stream_ind(void *msg);
extern void av_bt_close_codec_ind(void *msg);
extern void av_bt_reconfig_stream_cnf(void* msg);

#ifdef __MMI_BT_AUDIO_VIA_SCO__
extern av_bt_open_result_enum av_bt_open_audio_via_sco(void);
#endif

extern void aud_send_bt_open_codec_req(module_type src_mod_id, 
    bt_a2dp_audio_cap_struct *cfg,
    kal_uint8 stream_handle,
    kal_bool immediate,
    kal_uint32 seq_no);
extern void aud_send_bt_close_codec_req(module_type src_mod_id);

extern kal_int16 med_get_media_type(kal_wchar *file_name);

extern void rda_bt_a2dp_send_sep_discover_res( kal_uint8 src_mod_id, kal_uint16 connect_id, kal_uint16 result, kal_uint8 sep_num, bt_sep_info_struct* sep_list);
extern void rda_bt_a2dp_send_capabilities_get_res( kal_uint8 src_mod_id, kal_uint16 connect_id, kal_uint16 result, kal_uint8 codec_type, bt_a2dp_audio_codec_cap_struct* audio_cap );
extern void rda_bt_a2dp_send_stream_config_res( kal_uint8 src_mod_id, kal_uint16 result, kal_uint8 stream_handle);
extern void rda_bt_a2dp_send_stream_open_res( kal_uint8 src_mod_id, kal_uint16 result, kal_uint8 stream_handle );
extern void rda_bt_a2dp_send_stream_start_res( kal_uint8 src_mod_id, kal_uint16 result, kal_uint8 stream_handle );
extern const bt_a2dp_sbc_codec_cap_struct sbc_cap[1];
extern const bt_a2dp_mp3_codec_cap_struct mp3_cap[1];
U8 new_connet_a2dp=0;
extern U8 g_ssc_bt_a2dp_mode;
extern audply_struct g_audply;
#define __av_bt_codes__

/*****************************************************************************
 * FUNCTION
 *  av_bt_init
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_init(void)
{
    /* init state & callback */
    av_bt_ctx.codec_seq_no = 0;
    av_bt_ctx.is_inquiry = KAL_FALSE;
    av_bt_ctx.is_codec_open = KAL_FALSE;
    av_bt_ctx.support_reconfig = KAL_FALSE;
    av_bt_ctx.need_reconfig = KAL_FALSE;
    av_bt_ctx.need_reconnect = KAL_FALSE;
    av_bt_ctx.connect_post_proc = KAL_FALSE;
    av_bt_ctx.state = AV_BT_STATE_IDLE;
    av_bt_ctx.wait = AV_BT_WAIT_NONE_CNF;
    av_bt_ctx.headset_selected_callback = NULL;
    av_bt_ctx.callback = NULL;
    av_bt_ctx.retry_success_callback = NULL;
    av_bt_ctx.default_callbacks[0] = NULL;
    av_bt_ctx.local_seid = 0x01;  /* valid between 0x01 ~ 0x3E */
    av_bt_ctx.stream_handle = 0;
    /* init dev */
    memset(&av_bt_ctx.dev, 0, sizeof(av_bt_dev_struct));
    /* init cfg */
    memset(&av_bt_ctx.cfg, 0, sizeof(bt_a2dp_audio_cap_struct));
    memset(&av_bt_ctx.req_cfg[0], 0, 2*sizeof(bt_a2dp_audio_cap_struct));
    av_bt_ctx.req_cfg_index = 0;
    /* set msg hdlrs */
    SetProtocolEventHandler(av_bt_discover_stream_cnf, MSG_ID_BT_A2DP_SEP_DISCOVER_CNF);
    SetProtocolEventHandler(av_bt_discover_stream_ind, MSG_ID_BT_A2DP_SEP_DISCOVER_IND);
    SetProtocolEventHandler(av_bt_get_stream_cap_cnf, MSG_ID_BT_A2DP_CAPABILITIES_GET_CNF);
    SetProtocolEventHandler(av_bt_get_stream_cap_ind, MSG_ID_BT_A2DP_CAPABILITIES_GET_IND);
    SetProtocolEventHandler(av_bt_config_stream_cnf, MSG_ID_BT_A2DP_STREAM_CONFIG_CNF);
    SetProtocolEventHandler(av_bt_config_stream_ind, MSG_ID_BT_A2DP_STREAM_CONFIG_IND);
    SetProtocolEventHandler(av_bt_reconfig_stream_cnf, MSG_ID_BT_A2DP_STREAM_RECONFIG_CNF);
    SetProtocolEventHandler(av_bt_open_stream_cnf, MSG_ID_BT_A2DP_STREAM_OPEN_CNF);
    SetProtocolEventHandler(av_bt_open_stream_ind, MSG_ID_BT_A2DP_STREAM_OPEN_IND);
    SetProtocolEventHandler(av_bt_start_stream_cnf, MSG_ID_BT_A2DP_STREAM_START_CNF);
    SetProtocolEventHandler(av_bt_start_stream_ind, MSG_ID_BT_A2DP_STREAM_START_IND);
    SetProtocolEventHandler(av_bt_pause_stream_cnf, MSG_ID_BT_A2DP_STREAM_PAUSE_CNF);
    SetProtocolEventHandler(av_bt_pause_stream_ind, MSG_ID_BT_A2DP_STREAM_PAUSE_IND);
    SetProtocolEventHandler(av_bt_close_stream_cnf, MSG_ID_BT_A2DP_STREAM_CLOSE_CNF);
    SetProtocolEventHandler(av_bt_close_stream_ind, MSG_ID_BT_A2DP_STREAM_CLOSE_IND);
    SetProtocolEventHandler(av_bt_abort_stream_cnf, MSG_ID_BT_A2DP_STREAM_ABORT_CNF);
    SetProtocolEventHandler(av_bt_abort_stream_ind, MSG_ID_BT_A2DP_STREAM_ABORT_IND);
    SetProtocolEventHandler(av_bt_close_codec_ind, MSG_ID_MEDIA_BT_CLOSE_CODEC_IND);

}

/*****************************************************************************
 * FUNCTION
 *  av_bt_set_default_callback
 * DESCRIPTION
 *  This function is to set default callback handler.
 * PARAMETERS
 *  handler     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_set_default_callback(AV_BT_CALLBACK handler)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /*currently only support audio player*/
    av_bt_ctx.default_callbacks[0] = handler;
}


/*****************************************************************************
 * FUNCTION
 *  av_bt_open_result
 * DESCRIPTION
 *  
 * PARAMETERS
 *  result    [IN]
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_audply_press_play_button_down();
extern void mmi_audply_press_play_button_up();
U8   wait_play=0;
void av_bt_open_result(kal_int32 result)
{
    void(*callback)(S32 res) = av_bt_ctx.callback;
    void(*retry_success_callback)(S32 res) = av_bt_ctx.retry_success_callback;

 mmi_trace(g_sw_BT, "av_bt_open_result: result=%d,callback=0x%x,new_connet_a2dp=%d,g_audply.state=%d\n", result,callback,new_connet_a2dp,g_audply.state);
 //   mdi_audio_set_pseudo_play_file_callback(NULL);
    av_bt_ctx.need_reconfig = KAL_FALSE;
    if(new_connet_a2dp&&(g_audply.state==STATE_PLAY)&&(result == AV_BT_CALLBACK_EVENT_OPEN_OK ))//zhou siyou 20100604
    {
       mmi_audply_press_play_button_down();
	 mmi_audply_press_play_button_up();  
	 wait_play=TRUE;
    }
     else
     {
    if (callback)
    {
        if (result != AV_BT_CALLBACK_EVENT_OPEN_OK)
        {

        #ifdef __MMI_BT_AUDIO_VIA_SCO__
            if(result == AV_BT_CALLBACK_EVENT_OPEN_FAILED)
            {
                if(av_bt_open_audio_via_sco() == AV_BT_OPEN_WAITING)
                {    
                    return;
                }
            }
        #endif
            if(result == AV_BT_CALLBACK_EVENT_SECURITY_BLOCK)
            {
                result = AV_BT_CALLBACK_EVENT_OPEN_FAILED;
            }
	
            av_bt_ctx.callback = NULL;
        }
        callback(result);
    }
    else if (result == AV_BT_CALLBACK_EVENT_OPEN_OK && retry_success_callback)
    {
        av_bt_ctx.callback = retry_success_callback;
        retry_success_callback(result);
	    }
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_calc_audio_config
 * DESCRIPTION
 *  
 * PARAMETERS
 *  aud_info    [IN]
 *  aud_cfg     [IN]
 *  aud_fmt     [IN]
 *  codec_type  [IN]
 * RETURNS
 *  kal_bool
 *****************************************************************************/
kal_bool av_bt_aud_info_to_cfg(
            AudDesInfoStruct *aud_info,
            bt_a2dp_audio_cap_struct *aud_cfg,
            Media_Format aud_fmt,
            kal_uint8 codec_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /*kal_bool stereo = aud_info->stereo;*/
    kal_uint32 samplerate = aud_info->sampleRate;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    aud_cfg->codec_type = codec_type;
	mmi_trace(1,"av_bt_aud_info_to_cfg is called codec_type=%d",codec_type);
    if (codec_type == BT_A2DP_SBC)
    {   /* SBC transcoding is necessary */
        bt_a2dp_sbc_codec_cap_struct *sbc = &aud_cfg->codec_cap.sbc;

        samplerate /= 1000;
        switch (samplerate)
        {
            case 7:
            case 8:
            case 15:
            case 16:

            case 31:
            case 32:
                sbc->sample_rate = 0x4; /* convert to 16000 sps */
                break;
            case 10:
            case 11:
            case 21:
            case 22:
            case 43:
            case 44:
                sbc->sample_rate = 0x2; /* convert to 16000 sps */
                break;
            case 12:
            case 23:
            case 24:
            case 47:
            case 48:
                sbc->sample_rate = 0x1; /* convert to 16000 sps */
                break;
            default:
                return KAL_FALSE;
        }

        sbc->channel_mode = 0x1;    /* default: joint-stereo */

        sbc->block_len = 0x1;       /* 16: fixed */
        sbc->subband_num = 0x1;     /* 8: fixed */
        sbc->alloc_method = 0x1;    /* loudness: fixed */
	sbc->min_bit_pool =	19;
	sbc->max_bit_pool =	75;
//        SBC_InitiateBitPool(sbc->channel_mode, sbc->sample_rate, &sbc->min_bit_pool, &sbc->max_bit_pool);
    }
    else
    {                           /* not support transcoding here, media format shall be the same with codec type */
        if (codec_type == BT_A2DP_MP3 && aud_fmt == MEDIA_FORMAT_DAF)
        {                       /* MP3 streaming */
            bt_a2dp_mp3_codec_cap_struct *mp3 = &aud_cfg->codec_cap.mp3;

            mp3->layer = 0x1;
            mp3->CRC = (kal_bool)aud_info->details.DAF.CRC;
            if (aud_info->details.DAF.channel_mode == 0x00)
               mp3->channel_mode = 0x2;
            else if (aud_info->details.DAF.channel_mode == 0x01)
               mp3->channel_mode = 0x1;
            else if (aud_info->details.DAF.channel_mode == 0x02)
               mp3->channel_mode = 0x4;
            else if (aud_info->details.DAF.channel_mode == 0x03)
               mp3->channel_mode = 0x8;
            else
               return KAL_FALSE;
            mp3->MPF = KAL_FALSE;
            if (samplerate == 48000)
               mp3->sample_rate = 0x1;
            else if (samplerate == 44100)
               mp3->sample_rate = 0x2;
            else if (samplerate == 32000)
               mp3->sample_rate = 0x4;
            else if (samplerate == 24000)
               mp3->sample_rate = 0x8;
            else if (samplerate == 22050)
               mp3->sample_rate = 0x10;
            else if (samplerate == 16000)
               mp3->sample_rate = 0x20;
            else
               return KAL_FALSE;
            mp3->VBR = KAL_TRUE;
            mp3->bit_rate = 0x4000;
        }
        else if (codec_type == BT_A2DP_AAC && aud_fmt == MEDIA_FORMAT_AAC)
        {                       /* AAC streaming */
            /* bt_a2dp_aac_codec_cap_struct *aac = &aud_cfg->codec_cap.aac; */
            return KAL_FALSE;   /* not support now */
        }
        else
        {   /* not supported streaming codec */
            return KAL_FALSE;
        }
    }

    return KAL_TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  av_bt_get_aud_cfg
 * DESCRIPTION
 *  
 * PARAMETERS
 *  filename    [IN]
 *  aud_cfg     [IN]
 *  sbc_only    [IN]
 * RETURNS
 *  kal_int32
 *****************************************************************************/
kal_int32 av_bt_get_aud_cfg(kal_wchar* filename, bt_a2dp_audio_cap_struct *aud_cfg,
    kal_bool sbc_only)
{
    kal_int16 format, i, count;
    kal_uint8 codec_type[2] = {BT_A2DP_SBC, 0};
    AudDesInfoStruct aud_info;
     FS_HANDLE FileHander;
   mci_type_enum FileType;
   BOOL bGetInfoFlag;


    aud_cfg[0].codec_type = 0;
    aud_cfg[1].codec_type = 0;
   bGetInfoFlag = TRUE;
    format = med_get_media_type(filename);

     mmi_trace(g_sw_BT, "av_bt_get_aud_cfg::format = %d", format);
    switch (format)
    {
#ifdef AMR_DECODE
        case MDI_FORMAT_AMR:
    #ifdef AMRWB_DECODE
        case MDI_FORMAT_AMR_WB:
    #endif 
	FileType = format;
            break;
#endif /* AMR_DECODE */ 
        case MDI_FORMAT_GSM_FR:
        case MDI_FORMAT_GSM_HR:
        case MDI_FORMAT_GSM_EFR:
#ifdef __MED_VR_MOD__
        case MDI_FORMAT_VR:
#endif 
	FileType = format;
            break;
        case MDI_FORMAT_PCM_16K:
        case MDI_FORMAT_PCM_8K:
        case MDI_FORMAT_DVI_ADPCM:
	FileType = format;
            break;
#ifdef DAF_DECODE
        case MDI_FORMAT_DAF:
	FileType = format;

            if (!sbc_only)
            {
                codec_type[0] = BT_A2DP_MP3;
                codec_type[1] = BT_A2DP_SBC;
            }
            break;
#ifdef MUSICAM_DECODE        
        case MDI_FORMAT_MUSICAM:
	FileType = format;
            break;
#endif /* MUSICAM_DECODE */
#endif /* DAF_DECODE */ 
        case MDI_FORMAT_WAV:
        case MDI_FORMAT_WAV_ALAW:
        case MDI_FORMAT_WAV_ULAW:
        case MDI_FORMAT_WAV_DVI_ADPCM:
	FileType = format;

            break;
#ifdef AU_DECODE
        case MDI_FORMAT_AU:
	FileType = format;
            break;
#endif /* AU_DECODE */
#ifdef AIFF_DECODE
        case MDI_FORMAT_AIFF:
	FileType = format;
            break;
#endif /* AIFF_DECODE */
#ifdef WMA_DECODE
        case MDI_FORMAT_WMA:
	FileType = format;
            break;
#endif /* WMA_DECODE */ 
#ifdef AAC_DECODE
        case MDI_FORMAT_AAC:
   	FileType = format;
            break;
        case MDI_FORMAT_M4A:
	FileType = format;
            break;
#endif /* AAC_DECODE */ 
#if defined(__MMI_PURE_AUDIO__)
        case MDI_FORMAT_3GP:
        case MDI_FORMAT_MP4:        
	FileType = format;
            break;
#endif /* defined(__MMI_PURE_AUDIO__) */ 
        default:
	        bGetInfoFlag = FALSE;
            aud_info.sampleRate = 32000;
            aud_info.stereo = KAL_TRUE;
            break;
    }

	if(bGetInfoFlag)
	{
             FileHander = MMI_FS_Open((const UINT8 * )filename, FS_READ_ONLY);
	       if(FileHander < 0) return -1;
		MCI_AudioGetFileInformation(FileHander, &aud_info,FileType);

		if(aud_info.stereo !=2)	 aud_info.stereo =2; // In SBC encoder will transfer mono to stereo
		
		MMI_FS_Close(FileHander);
	}
	else  return -1;
		

mmi_trace(g_sw_BT,"aud_cfg[0]=%d,aud_cfg[1]=%d",aud_cfg[0],aud_cfg[1]);
    for( count = i = 0; i < 2 && codec_type[i]; i++ )
    {
        if (av_bt_aud_info_to_cfg(&aud_info, &aud_cfg[count], format, codec_type[i]))
        {
            if( aud_cfg[count].codec_type == BT_A2DP_SBC )
            {

              mmi_trace(g_sw_BT,"av_bt_aud_info_to_cfg: count=%d,min_bit_pool=%d, max_bit_pool=%d, sample_rate=%d, channel_mode=%d\n", \
			count,\
			aud_cfg[count].codec_cap.sbc.min_bit_pool, \
                    aud_cfg[count].codec_cap.sbc.max_bit_pool, \
                    aud_cfg[count].codec_cap.sbc.sample_rate, \
                    aud_cfg[count].codec_cap.sbc.channel_mode);

            }
            else if (aud_cfg[count].codec_type == BT_A2DP_MP3)
            {

            	mmi_trace(g_sw_BT, "av_bt_aud_info_to_cfg: count=%d,bit_rate=%d, layer=%d, sample_rate=%d, channel_mode=%d, VBR=%d, MPF=%d\n", \
			count,\
                    aud_cfg[count].codec_cap.mp3.bit_rate, \
                    aud_cfg[count].codec_cap.mp3.layer, \
                    aud_cfg[count].codec_cap.mp3.sample_rate, \
                    aud_cfg[count].codec_cap.mp3.channel_mode, \
                    aud_cfg[count].codec_cap.mp3.VBR, \
                    aud_cfg[count].codec_cap.mp3.MPF);

            }
            /* enforce a bitrate limit to 320K, otherwise, use SBC encode */
            if (codec_type[i] == BT_A2DP_MP3 && aud_info.bitRate >= 320000)
            {
                continue;
            }
            count++;
        }
        else
        {
            continue;
        }
    }
    return (count > 0) ? 0: -1;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_get_aud_cfg_ex
 * DESCRIPTION
 *  
 * PARAMETERS
 *  sample_rate [IN]
 *  stereo      [IN]
 *  aud_cfg     [IN]
 * RETURNS
 *  kal_int32
 *****************************************************************************/
kal_int32 av_bt_get_aud_cfg_ex(kal_uint16 sample_rate, kal_bool stereo, bt_a2dp_audio_cap_struct *aud_cfg)
{
    AudDesInfoStruct aud_info;

    mmi_trace(g_sw_BT,"av_bt_aud_info_to_cfg_ex: sample_rate=%d, channel_mode=%d\n",sample_rate, stereo);

    aud_cfg[0].codec_type = 0;
    aud_cfg[1].codec_type = 0;

    aud_info.sampleRate = (kal_uint32)sample_rate;
    aud_info.stereo = stereo;

    if (!av_bt_aud_info_to_cfg(&aud_info, &aud_cfg[0], 0, BT_A2DP_SBC))
        return -1;

	mmi_trace(g_sw_BT,"av_bt_aud_info_to_cfg_ex: min_bit_pool=%d, max_bit_pool=%d, sample_rate=%d, channel_mode=%d\n", \
        aud_cfg[0].codec_cap.sbc.min_bit_pool, \
        aud_cfg[0].codec_cap.sbc.max_bit_pool, \
        aud_cfg[0].codec_cap.sbc.sample_rate, \
        aud_cfg[0].codec_cap.sbc.channel_mode);


    return 0;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_match_aud_cfg
 * DESCRIPTION
 *  
 * PARAMETERS
 *  dev         [IN]
 *  cfg         [IN]
 *  sep_index   [IN]
 *  cap_index   [IN]
 * RETURNS
 *  kal_int32
 *****************************************************************************/
kal_bool av_bt_match_aud_cfg(av_bt_dev_struct *dev, bt_a2dp_audio_cap_struct *cfg,
    kal_uint8 *sep_index, kal_uint8 *cap_index)
{
    int i, j;
    kal_uint16 result;
    av_bt_sep_struct *sep;
    bt_a2dp_audio_cap_struct *cap;

    for (i = 0; i < dev->sep_count; i++)
    {
        sep = &dev->sep_list[i];

        for (j = 0; j < sep->cap_count; j++)
        {
            cap = &sep->cap_list[j];

            if (rda_bt_a2dp_match_audio_capabilities(cap, cfg, &result))//bt_a2dp_match_audio_capabilities
            {
                *sep_index = (kal_uint8)i;
                *cap_index = (kal_uint8)j;
                if( cfg->codec_type == BT_A2DP_SBC )
                {
#if 0
                  mmi_trace(g_sw_BT,"BT_A2DP_SBC OK av_bt_match_aud_cfg: min_bit_pool=%d, max_bit_pool=%d, sample_rate=%d, channel_mode=%d\n", \
                        cfg->codec_cap.sbc.min_bit_pool, \
                        cfg->codec_cap.sbc.max_bit_pool, \
                        cfg->codec_cap.sbc.sample_rate, \
                        cfg->codec_cap.sbc.channel_mode);
#endif
                }
                else if (cfg->codec_type == BT_A2DP_MP3)
                {
#if 0
                	mmi_trace(g_sw_BT,"BT_A2DP_MP3 OK av_bt_match_aud_cfg: bit_rate=%d, layer=%d, sample_rate=%d, channel_mode=%d, VBR=%d, MPF=%d\n", \
                        cfg->codec_cap.mp3.bit_rate, \
                        cfg->codec_cap.mp3.layer, \
                        cfg->codec_cap.mp3.sample_rate, \
                        cfg->codec_cap.mp3.channel_mode, \
                        cfg->codec_cap.mp3.VBR, \
                        cfg->codec_cap.mp3.MPF);
#endif
                }
                return KAL_TRUE;
            }
        }
    }

    return KAL_FALSE;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_cmp_aud_cfg
 * DESCRIPTION
 *  
 * PARAMETERS
 *  aud_cfg1    [IN]
 *  aud_cfg2    [IN]
 * RETURNS
 *  kal_bool
 *****************************************************************************/
kal_bool av_bt_cmp_aud_cfg(bt_a2dp_audio_cap_struct *aud_cfg1, bt_a2dp_audio_cap_struct *aud_cfg2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_bool result = KAL_FALSE;
   bt_a2dp_sbc_codec_cap_struct *sbc_cfg1, *sbc_cfg2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	mmi_trace(1,"av_bt_cmp_aud_cfg is called ud_cfg1->codec_type=%d,aud_cfg2->codec_type=%d",\
	aud_cfg1->codec_type,aud_cfg2->codec_type);
    if (aud_cfg1->codec_type == aud_cfg2->codec_type)
    {
        switch (aud_cfg1->codec_type)
        {
            case BT_A2DP_SBC://zhou siyou modified091030
                sbc_cfg1 = &aud_cfg1->codec_cap.sbc;
                sbc_cfg2 = &aud_cfg2->codec_cap.sbc;
                result = (sbc_cfg1->block_len == sbc_cfg2->block_len &&
                    sbc_cfg1->subband_num == sbc_cfg2->subband_num &&
                    sbc_cfg1->alloc_method == sbc_cfg2->alloc_method &&
                    sbc_cfg1->sample_rate == sbc_cfg2->sample_rate &&
                    sbc_cfg1->channel_mode == sbc_cfg2->channel_mode);
                  //  result=FALSE;
                break;
            case BT_A2DP_MP3:
                result = (memcmp(
                            (const void*)&aud_cfg1->codec_cap.mp3,
                            (const void*)&aud_cfg2->codec_cap.mp3,
                            sizeof(bt_a2dp_mp3_codec_cap_struct)) == 0);
                break;
            case BT_A2DP_AAC:
       /*         result = (memcmp(
                            (const void*)&aud_cfg1->codec_cap.aac,
                            (const void*)&aud_cfg2->codec_cap.aac,
                            sizeof(bt_a2dp_aac_codec_cap_struct)) == 0);
          */      break;
            case BT_A2DP_ATRAC:
/*                result = (memcmp(
                            (const void*)&aud_cfg1->codec_cap.atrac,
                            (const void*)&aud_cfg2->codec_cap.atrac,
                            sizeof(bt_a2dp_atrac_codec_cap_struct)) == 0);
    */            break;
            default:
                ASSERT(0);
        }
    }
	mmi_trace(1,"av_bt_cmp_aud_cfg will return %d",result);

    return result;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_cmp_req_cfg
 * DESCRIPTION
 *  
 * PARAMETERS
 *  cfg    [IN]
 * RETURNS
 *  kal_bool
 *****************************************************************************/
//U8 new_connect_cfg=0;
kal_bool av_bt_cmp_req_cfg(bt_a2dp_audio_cap_struct *cfg)
{
	int i;
//	mmi_trace(1,"av_bt_cmp_req_cfg is called,new_connect_cfg=%d",new_connect_cfg);
	for (i = 0; i < 2 && av_bt_ctx.req_cfg[i].codec_type; i++)
    {
        if (av_bt_cmp_aud_cfg(&av_bt_ctx.req_cfg[i], cfg))
            return KAL_TRUE;
    }

    return KAL_FALSE;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_cmp_bt_addr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  bt_addr1    [IN]
 *  bt_addr2    [IN]
 * RETURNS
 *  kal_bool
 *****************************************************************************/
kal_bool av_bt_cmp_bt_addr(mmi_bt_dev_addr *bt_addr1, mmi_bt_dev_addr *bt_addr2)
{
    kal_bool result;

    if( (bt_addr1->lap & 0x00FFFFFF) == (bt_addr2->lap & 0x00FFFFFF) &&
        bt_addr1->uap == bt_addr2->uap && bt_addr1->nap == bt_addr2->nap)
    {
        result = KAL_TRUE;
    }
    else
    {
        result = KAL_FALSE;
    }

    return result;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_open_interrupt_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  cause    [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_open_interrupt_hdlr(mdi_result cause)
{
    PRINT_INFORMATION(("av_bt_open_interrupt_hdlr\n"));

    av_bt_ctx.need_stop = KAL_TRUE;
    av_bt_ctx.need_reconnect = KAL_FALSE;
    av_bt_ctx.connect_post_proc = KAL_FALSE;
    av_bt_start_disc_timer();
    // stop timer
    StopTimer(AV_BT_START_TIMER);
#ifdef __MMI_BT_AUDIO_VIA_SCO__
    if(av_bt_ctx.wait == AV_BT_WAIT_HFP_CONNECT_CNF || av_bt_ctx.wait == AV_BT_WAIT_SCO_CONNECT_CNF)
    {
        av_bt_ctx.wait = AV_BT_WAIT_NONE_CNF;
    }
#endif
	
    av_bt_open_result(AV_BT_CALLBACK_EVENT_OPEN_CANCELED);
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_is_mp3_codec_open
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  kal_bool
 *****************************************************************************/
kal_bool av_bt_is_mp3_codec_open(void)
{
    return av_bt_ctx.is_codec_open && av_bt_ctx.cfg.codec_type == BT_A2DP_MP3;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_is_codec_open
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  kal_bool
 *****************************************************************************/
kal_bool av_bt_is_codec_open(void)
{
    return av_bt_ctx.is_codec_open ;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_is_streaming
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  kal_bool
 *****************************************************************************/
kal_bool av_bt_is_streaming(void)
{
    return av_bt_ctx.state == AV_BT_STATE_STREAMING;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_is_mp3_cfg
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  kal_bool
 *****************************************************************************/
kal_bool av_bt_is_mp3_cfg(void)
{
    kal_uint8 codec_type = 0;

    if (!av_bt_ctx.need_stop)
    {
        if (av_bt_ctx.state < AV_BT_STATE_CONFIGURED)
        {
            if (av_bt_ctx.wait == AV_BT_WAIT_CONFIG_CNF)
                codec_type = av_bt_ctx.tmp_cfg.codec_type;
            else
                codec_type = av_bt_ctx.req_cfg[0].codec_type;
        }
        else
        {
            codec_type = av_bt_ctx.cfg.codec_type;
        }
    }

    return codec_type == BT_A2DP_MP3;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_get_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  AV_BT_CALLBACK
 *****************************************************************************/
AV_BT_CALLBACK av_bt_get_callback(void)
{
    return av_bt_ctx.callback;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_open_internal
 * DESCRIPTION
 *  
 * PARAMETERS
 *  bt_dev              [IN]
 *  filename            [IN]
 *  sample_rate         [IN]
 *  stereo              [IN]
 *  callback            [IN]
 *  sbc_only            [IN]
 *  immediate_callback  [IN]
 * RETURNS
 *  kal_int32
 *****************************************************************************/
kal_int32 av_bt_open_internal(mmi_bt_dev_struct *bt_dev, kal_wchar* filename, 
    kal_uint16 sample_rate, kal_uint8 stereo, AV_BT_CALLBACK callback, 
    kal_bool sbc_only, kal_bool immediate_callback)
{
    kal_bool close_dev = KAL_FALSE;
    kal_bool dev_changed;

sbc_only =1;
    // stop timer
    StopTimer(AV_BT_START_TIMER);

    mmi_trace(g_sw_BT, "entry av_bt_open_internal,av_bt_ctx.state =%d,av_bt_ctx.wait%d",av_bt_ctx.state,av_bt_ctx.wait );

    av_bt_ctx.callback = callback;
    av_bt_ctx.retry_success_callback = callback;
 //   mdi_audio_set_pseudo_play_file_callback(av_bt_open_interrupt_hdlr);
    av_bt_ctx.need_reconfig = KAL_FALSE;


    av_bt_ctx.hfp_only = !mmi_bt_is_service_support_with_dev_addr(&bt_dev->bd_addr, MMI_BTH_ADVANCED_AUDIO_PROFILE_UUID);


/*for PTS test. Dongle might not reveal what they really support, so this flag could be wrong while testing*/
#ifdef __MMI_EM_BT_AVRCP_CONNECT_TG_ACTIVELY__
    if(mmi_bt_avrcp_get_actively_connect_tg_flag())
    {
        av_bt_ctx.hfp_only = KAL_FALSE;
    }
#endif

    mmi_trace(g_sw_BT, "entry av_bt_open_internal,av_bt_ctx.hfp_only .=%d",av_bt_ctx.hfp_only );

    /* compare the prev bt addr with requested bt addr */
    if (av_bt_ctx.state == AV_BT_STATE_IDLE)
    {
        if (av_bt_ctx.wait == AV_BT_WAIT_CONNECT_CNF)
        {
            /* check if connecting bt addr different */
            dev_changed = !av_bt_cmp_bt_addr(&av_bt_ctx.dev.bt_addr, &bt_dev->bd_addr);
            if (dev_changed)
            {
                close_dev = KAL_TRUE;
            }
        }
        else
        {
            /* check if connected bt addr different */
            dev_changed = !av_bt_cmp_bt_addr(&av_bt_ctx.bt_addr, &bt_dev->bd_addr);
        }
    }
    else
    {
        /* check if connected bt addr different */
        dev_changed = !av_bt_cmp_bt_addr(&av_bt_ctx.bt_addr, &bt_dev->bd_addr);
        if (dev_changed)
        {
            close_dev = KAL_TRUE;
        }
    }
    
    if (close_dev|| av_bt_ctx.hfp_only)
    {
        /* close dev */
        close_dev = av_bt_close_dev();
    }

    if (dev_changed)
    {
        /* reset dev */
        memset(&av_bt_ctx.dev, 0, sizeof(av_bt_dev_struct));
    }
    memcpy(&av_bt_ctx.dev.bt_addr, &bt_dev->bd_addr, sizeof(mmi_bt_dev_addr));

#ifndef __MMI_BT_AUDIO_VIA_SCO__
    if(av_bt_ctx.hfp_only)
    {
        av_bt_ctx.callback = NULL;
        av_bt_ctx.retry_success_callback = NULL;
  //      mdi_audio_set_pseudo_play_file_callback(NULL);
        callback(AV_BT_CALLBACK_EVENT_OPEN_FAILED);
        return AV_BT_OPEN_FAILED;
    }
#endif

  if(!av_bt_ctx.hfp_only)
  {
    if (filename != NULL)
    {
        if (av_bt_get_aud_cfg(filename, &av_bt_ctx.req_cfg[0], sbc_only) < 0)
        {
            mmi_trace(g_sw_BT, "av_bt_open_internal:: av_bt_get_aud_cfg fail!!!!!!!!");
             av_bt_ctx.need_stop = KAL_TRUE;
            av_bt_close(immediate_callback);
            return AV_BT_OPEN_FAILED;
        }

		
              mmi_trace(g_sw_BT, "av_bt_open_internal: 			av_bt_ctx.req_cfg[0].codec_type=%d, av_bt_ctx.req_cfg[0].codec_cap.mp3.channel_mode =%d, av_bt_ctx.req_cfg[0].codec_cap.mp3.layer=%d\n,  av_bt_ctx.req_cfg[0].codec_cap.mp3.sample_rate =%d\n", \
			av_bt_ctx.req_cfg[0].codec_type, av_bt_ctx.req_cfg[0].codec_cap.mp3.channel_mode, av_bt_ctx.req_cfg[0].codec_cap.mp3.layer,  av_bt_ctx.req_cfg[0].codec_cap.mp3.sample_rate );
              mmi_trace(g_sw_BT, "av_bt_open_internal: 			av_bt_ctx.req_cfg[1].codec_type=%d, av_bt_ctx.req_cfg[1].codec_cap.mp3.channel_mode=%d, av_bt_ctx.req_cfg[1].codec_cap.mp3.layer=%d\n,  av_bt_ctx.req_cfg[1].codec_cap.mp3.sample_rate =%d\n", \
			av_bt_ctx.req_cfg[1].codec_type, av_bt_ctx.req_cfg[1].codec_cap.mp3.channel_mode, av_bt_ctx.req_cfg[1].codec_cap.mp3.layer,  av_bt_ctx.req_cfg[1].codec_cap.mp3.sample_rate );
	
    }
    else
    {
        if (av_bt_get_aud_cfg_ex(sample_rate, stereo, &av_bt_ctx.req_cfg[0]) < 0)
        {
            mmi_trace(g_sw_BT, "av_bt_open_internal:: av_bt_get_aud_cfg_ex fail!!!!!!!!");
	     av_bt_ctx.need_stop = KAL_TRUE;		
            av_bt_close(immediate_callback);
            return AV_BT_OPEN_FAILED;
        }
    }

    if (av_bt_ctx.state == AV_BT_STATE_CONNECTED && av_bt_ctx.wait == AV_BT_WAIT_CONFIG_CNF)
    {
        if (!av_bt_cmp_req_cfg(&av_bt_ctx.tmp_cfg))
            av_bt_ctx.need_reconfig = KAL_TRUE;
    }

    /* handle IOT issues */
    if (strcmp(HP_Headphones, (const char*)bt_dev->name) == 0)
    {
        av_bt_ctx.support_reconfig = KAL_FALSE;
    }
    else
    {
        av_bt_ctx.support_reconfig = KAL_TRUE;
    }

#ifdef __MMI_BT_AUDIO_VIA_SCO__
    /* Disconnect SCO if connect to A2DP */
    mmi_profiles_bt_disable_sco_connection(MMI_PROFILE_BT_SCO_INT_A2DP);
#endif

    av_bt_ctx.need_reconnect = close_dev;
    av_bt_ctx.need_stop = KAL_FALSE;
    av_bt_ctx.bad_acp_seid_retry = KAL_FALSE;
    av_bt_ctx.bad_state_retry = KAL_FALSE;
    av_bt_ctx.sep_ind_use_retry = KAL_FALSE;
    av_bt_stop_disc_timer();
    if (av_bt_open_dev(immediate_callback))
        return AV_BT_OPEN_DONE;
}
#ifdef __MMI_BT_AUDIO_VIA_SCO__
    else
    {
        av_bt_open_result_enum open_result ;

        open_result = av_bt_open_audio_via_sco();

        if( open_result == AV_BT_OPEN_WAITING)
        {    
            return AV_BT_OPEN_WAITING;
        }

    //    mdi_audio_set_pseudo_play_file_callback(NULL);

        if(open_result == AV_BT_OPEN_DONE)
        {
            callback(AV_BT_CALLBACK_EVENT_OPEN_SCO);
            return AV_BT_OPEN_DONE;
        }
        else
        {
            av_bt_ctx.callback = NULL;
            av_bt_ctx.retry_success_callback = NULL;
            callback(AV_BT_CALLBACK_EVENT_OPEN_FAILED);
            return AV_BT_OPEN_FAILED;
        }

    }
#endif /*__MMI_BT_AUDIO_VIA_SCO__*/

     mmi_trace(g_sw_BT, "av_bt_open_internal:: AV_BT_OPEN_WAITING!!!!!!!!");

    return AV_BT_OPEN_WAITING;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_open_ex
 * DESCRIPTION
 *  
 * PARAMETERS
 *  bt_dev              [IN]
 *  sample_rate         [IN]
 *  stereo              [IN]
 *  callback            [IN]
 *  immediate_callback  [IN]
 * RETURNS
 *  kal_int32
 *****************************************************************************/
kal_int32 av_bt_open_ex(mmi_bt_dev_struct *bt_dev, kal_uint16 sample_rate, 
    kal_uint8 stereo, AV_BT_CALLBACK callback, kal_bool immediate_callback)
{
 #if 0   
  PRINT_INFORMATIONi(("av_bt_open_ex: state=%d, wait=%d, immediate_callback=%d\n",
        av_bt_ctx.state, av_bt_ctx.wait, immediate_callback));
#endif
    return av_bt_open_internal(bt_dev, NULL, sample_rate, stereo, callback, KAL_TRUE, immediate_callback);
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_open
 * DESCRIPTION
 *  
 * PARAMETERS
 *  bt_dev              [IN]
 *  filename            [IN]
 *  callback            [IN]
 *  sbc_only            [IN]
 *  immediate_callback  [IN]
 * RETURNS
 *  kal_int32
 *****************************************************************************/
kal_int32 av_bt_open(mmi_bt_dev_struct *bt_dev, kal_wchar* filename, 
    AV_BT_CALLBACK callback, kal_bool sbc_only, kal_bool immediate_callback)
{

mmi_trace(g_sw_BT, "av_bt_open: state=%d, wait=%d, sbc_only=%d, immediate_callback=%d\n",
       av_bt_ctx.state, av_bt_ctx.wait, sbc_only, immediate_callback);
    return av_bt_open_internal(bt_dev, filename, 0, 0, callback, sbc_only, immediate_callback);
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_close
 * DESCRIPTION
 *  
 * PARAMETERS
 *  notify_opener   [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_close(kal_bool notify_opener)
{
    void(*callback)(S32 res) = av_bt_ctx.callback;
    kal_bool need_stop = av_bt_ctx.need_stop;

    mmi_trace(g_sw_BT, "av_bt_close: notify_opener=%d,av_bt_ctx.state=%d,av_bt_ctx.wait=%d,callback=0x%x\n",\
		notify_opener,av_bt_ctx.state,av_bt_ctx.wait,callback);

    av_bt_ctx.need_reconfig = KAL_FALSE;
    av_bt_ctx.need_reconnect = KAL_FALSE;
    av_bt_ctx.need_stop = KAL_TRUE;
    av_bt_ctx.callback = NULL;
    av_bt_ctx.retry_success_callback = NULL;
//    mdi_audio_set_pseudo_play_file_callback(NULL);
    av_bt_start_disc_timer();

    // stop timer
    StopTimer(AV_BT_START_TIMER);

    if (g_ssc_bt_a2dp_mode)
    {
        if (av_bt_ctx.state >= AV_BT_STATE_OPEN && av_bt_ctx.wait == AV_BT_WAIT_NONE_CNF)
        {
            av_bt_close_stream();
        }
    }

    if (av_bt_ctx.state >= AV_BT_STATE_STREAMING && av_bt_ctx.wait == AV_BT_WAIT_NONE_CNF)
    {
        av_bt_pause_stream();
    }

#ifdef __MMI_BT_AUDIO_VIA_SCO__
    if(av_bt_ctx.wait == AV_BT_WAIT_HFP_CONNECT_CNF || av_bt_ctx.wait == AV_BT_WAIT_SCO_CONNECT_CNF)
    {
        av_bt_ctx.wait = AV_BT_WAIT_NONE_CNF;
    }
#endif


    if (notify_opener && callback)
    {
        if (need_stop || av_bt_ctx.state != AV_BT_STATE_STREAMING)
        {
            callback(AV_BT_CALLBACK_EVENT_OPEN_STOPPED);
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_pause
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_pause(void)
{
    PRINT_INFORMATION(("av_bt_pause:\n"));

    av_bt_ctx.need_stop = KAL_TRUE;
    // stop timer
    StopTimer(AV_BT_START_TIMER);

    if (av_bt_ctx.state >= AV_BT_STATE_STREAMING && av_bt_ctx.wait == AV_BT_WAIT_NONE_CNF)
    {
        av_bt_pause_stream();
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_open_codec
 * DESCRIPTION
 *  
 * PARAMETERS
 *  immediate      [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_open_codec(kal_bool immediate)
{
      mmi_trace(g_sw_BT, "av_bt_open_codec: is_open=%d, seq_no=%d, \n", 
        av_bt_ctx.is_codec_open, av_bt_ctx.codec_seq_no);

    if (av_bt_ctx.state == AV_BT_STATE_STREAMING)
    {
        if (!av_bt_ctx.is_codec_open)
        {
              mmi_trace(g_sw_BT, "av_bt_open_codec: av_bt_ctx.cfg.codec_type=%d, \n", av_bt_ctx.cfg.codec_type);

            aud_send_bt_open_codec_req(MOD_MMI, &av_bt_ctx.cfg,
                av_bt_ctx.stream_handle, immediate, av_bt_ctx.codec_seq_no);
            av_bt_ctx.is_codec_open = KAL_TRUE;
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_close_codec
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_close_codec(void)
{
    mmi_trace(1,"av_bt_close_codec: is_open=%d, seq_no=%d\n", 
        av_bt_ctx.is_codec_open, av_bt_ctx.codec_seq_no);

    if (av_bt_ctx.is_codec_open)
    {
        aud_send_bt_close_codec_req(MOD_MMI);
        av_bt_ctx.codec_seq_no++;
        av_bt_ctx.is_codec_open = KAL_FALSE;
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_close_codec_ind
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msg     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_close_codec_ind(void *msg)
{
    media_bt_close_codec_ind_struct *msg_p = (media_bt_close_codec_ind_struct*)msg;

  //  PRINT_INFORMATION(("av_bt_close_codec_ind: is_open=%d, seq_no=%d\n", 
   //     av_bt_ctx.is_codec_open, av_bt_ctx.codec_seq_no));

    if (av_bt_ctx.is_codec_open && av_bt_ctx.codec_seq_no == msg_p->seq_no)
    {
        av_bt_ctx.is_codec_open = KAL_FALSE;
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_open_dev
 * DESCRIPTION
 *  
 * PARAMETERS
 *  kal_bool
 * RETURNS
 *  kal_bool
 *****************************************************************************/
 void av_bt_reconfig_stream(void);
kal_bool av_bt_open_dev(kal_bool done_callback)
{
    kal_bool need_reconfig;
    kal_bool done = KAL_FALSE;

    mmi_trace(g_sw_BT, "av_bt_open_dev: done_callback=%d  av_bt_ctx.need_reconnect = %d,av_bt_ctx.state=%d,av_bt_ctx.wait=%d\n", \
		done_callback,av_bt_ctx.need_reconnect,av_bt_ctx.state,av_bt_ctx.wait);

    if (av_bt_ctx.need_reconnect)
    {
        return done;
    }

  /*  mmi_trace(g_sw_BT, "av_bt_open_dev:av_bt_ctx.state=%d",av_bt_ctx.state);
      mmi_trace(g_sw_BT, "av_bt_open_dev: 		av_bt_ctx.req_cfg[0].codec_type=%d, av_bt_ctx.req_cfg[0].codec_cap.mp3.channel_mode = %d	,av_bt_ctx.req_cfg[0].codec_cap.mp3.layer=%d\n,  av_bt_ctx.req_cfg[0].codec_cap.mp3.sample_rate =%d\n", \
	av_bt_ctx.req_cfg[0].codec_type, av_bt_ctx.req_cfg[0].codec_cap.mp3.channel_mode, av_bt_ctx.req_cfg[0].codec_cap.mp3.layer,  av_bt_ctx.req_cfg[0].codec_cap.mp3.sample_rate );
      mmi_trace(g_sw_BT, "av_bt_open_dev: 		av_bt_ctx.req_cfg[1].codec_type=%d, av_bt_ctx.req_cfg[1].codec_cap.mp3.channel_mode = %d	, av_bt_ctx.req_cfg[1].codec_cap.mp3.layer=%d\n,  av_bt_ctx.req_cfg[1].codec_cap.mp3.sample_rate =%d\n", \
	av_bt_ctx.req_cfg[1].codec_type,av_bt_ctx.req_cfg[1].codec_cap.mp3.channel_mode, av_bt_ctx.req_cfg[1].codec_cap.mp3.layer,  av_bt_ctx.req_cfg[1].codec_cap.mp3.sample_rate );
	*/
    switch (av_bt_ctx.state)
    {
    case AV_BT_STATE_IDLE:
        if (av_bt_ctx.wait == AV_BT_WAIT_NONE_CNF)
        {
            av_bt_connect();
        }
        break;
    case AV_BT_STATE_CONNECTED:
        if (av_bt_ctx.wait == AV_BT_WAIT_NONE_CNF)
        {
            if (!av_bt_discover_stream())
            {
                if (!av_bt_get_stream_cap())
                {
                    av_bt_config_stream();
                }
            }
        }
        break;
    case AV_BT_STATE_CONFIGURED:
        need_reconfig = !av_bt_cmp_req_cfg(&av_bt_ctx.cfg);
		mmi_trace(g_sw_BT, "configed::need_reconfig = %d", need_reconfig);
        if (av_bt_ctx.wait == AV_BT_WAIT_NONE_CNF)
        {
            if (need_reconfig)
            {
                av_bt_abort_stream();
            }
            else
            {
                av_bt_open_stream();
            }
        }
        else
            av_bt_ctx.need_reconfig = need_reconfig;
        break;
    case AV_BT_STATE_OPEN:
        need_reconfig = !av_bt_cmp_req_cfg(&av_bt_ctx.cfg);
	mmi_trace(1," opened need_reconfig=%d",need_reconfig);
        if (av_bt_ctx.wait == AV_BT_WAIT_NONE_CNF)
        {
            if (need_reconfig)
            {
            	if(!av_bt_ctx.dev.sep_count)//headset first auto connect,in av_bt_ctx.dev has no it cap.zhou siyou
			av_bt_close_stream();
		else
               	av_bt_reconfig_stream();
            }
            else
            {
                av_bt_start_stream();
            }
        }
        else
            av_bt_ctx.need_reconfig = need_reconfig;
        break;
    case AV_BT_STATE_STREAMING:
        need_reconfig = !av_bt_cmp_req_cfg(&av_bt_ctx.cfg);
	mmi_trace(1,"streaming need_reconfig=%d,done_callback =%d",need_reconfig,done_callback);
        if (av_bt_ctx.wait == AV_BT_WAIT_NONE_CNF)
        {
            if (need_reconfig)
            {
                av_bt_close_stream();
            }
            else
            {
                done = KAL_TRUE;
				
                if (done_callback)
                {
                    av_bt_open_result(AV_BT_CALLBACK_EVENT_OPEN_OK);
                }
            }
        }
        else
            av_bt_ctx.need_reconfig = need_reconfig;
        break;
    }

    return done;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_close_dev
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  kal_bool
 *****************************************************************************/
kal_bool av_bt_close_dev()
{
    kal_bool wait_connect = (av_bt_ctx.wait == AV_BT_WAIT_CONNECT_CNF);

    mmi_trace(1,"av_bt_close_dev\n");

    av_bt_ctx.connect_post_proc = KAL_FALSE;

    if (av_bt_ctx.wait != AV_BT_WAIT_DISCONNECT_CNF && 
        (av_bt_ctx.state != AV_BT_STATE_IDLE || wait_connect))
    {
        if (wait_connect)
        {
            av_bt_ctx.connect_post_proc = KAL_TRUE;
        }
        else
        {
            av_bt_disconnect();
        }
        /* init cfg */
        memset(&av_bt_ctx.cfg, 0, sizeof(bt_a2dp_audio_cap_struct));
        memset(&av_bt_ctx.req_cfg[0], 0, 2*sizeof(bt_a2dp_audio_cap_struct));
        av_bt_ctx.req_cfg_index = 0;

        return KAL_TRUE;
    }

    return KAL_FALSE;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_start_disc_timer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_start_disc_timer(void)
{

	
    mmi_trace(1,"av_bt_start_disc_timer is called\n");

#ifdef __MMI_BT_AUDIO_VIA_SCO__
        mmi_profiles_bt_enable_sco_connection(MMI_PROFILE_BT_SCO_INT_A2DP);
#endif    
     //   StartTimer(AV_BT_DISC_TIMER, 30000, (FuncPtr)av_bt_close_dev);
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_stop_disc_timer
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_stop_disc_timer(void)
{
    PRINT_INFORMATION(("av_bt_stop_disc_timer\n"));
    StopTimer(AV_BT_DISC_TIMER);
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_select_headset
 * DESCRIPTION
 *  
 * PARAMETERS
 *  callback     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_select_headset(void (*callback)(mmi_bt_dev_struct* dev))
{
    av_bt_ctx.headset_selected_callback = callback;
 
//    GoBackHistory();
    mmi_bt_entry_a2dp_device_select_screen();
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_headset_selected_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  dev     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_headset_selected_callback(mmi_bt_dev_struct* dev)
{
    void (*headset_selected_callback)(mmi_bt_dev_struct*) = av_bt_ctx.headset_selected_callback;

    if (headset_selected_callback)
    {
        av_bt_ctx.headset_selected_callback = NULL;
        headset_selected_callback(dev);
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_connect
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_connect(void)
{
    mmi_trace(g_sw_BT, "av_bt_connect\n");

    ASSERT(av_bt_ctx.state == AV_BT_STATE_IDLE);

    mmi_bt_a2dp_connect_req(*((mmi_bth_bt_addr*)&av_bt_ctx.dev.bt_addr));
    av_bt_ctx.wait = AV_BT_WAIT_CONNECT_CNF;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_disconnect
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_disconnect(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    PRINT_INFORMATION(("av_bt_disconnect\n"));

    ASSERT(av_bt_ctx.state >= AV_BT_STATE_CONNECTED);

    // stop timer
    StopTimer(AV_BT_START_TIMER);

    mmi_bt_a2dp_disconnect_req(*((mmi_bth_bt_addr*)&av_bt_ctx.bt_addr), av_bt_ctx.connect_id);
    av_bt_ctx.wait = AV_BT_WAIT_DISCONNECT_CNF;
    
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_discover_stream
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  kal_bool
 *****************************************************************************/
kal_bool av_bt_discover_stream(void)
{
     mmi_trace(g_sw_BT,"av_bt_discover_stream\n");

    ASSERT(av_bt_ctx.state == AV_BT_STATE_CONNECTED);

    if (av_bt_ctx.dev.sep_count == 0)
    {
        bt_a2dp_send_sep_discover_req(MOD_MMI, av_bt_ctx.connect_id);
        av_bt_ctx.wait = AV_BT_WAIT_DISCOVER_CNF;
        return KAL_TRUE;
    }

    return KAL_FALSE;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_get_stream_cap
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  kal_bool
 *****************************************************************************/
kal_bool av_bt_get_stream_cap(void)
{
    int i;
    av_bt_sep_struct *sep;

	mmi_trace(g_sw_BT, "av_bt_get_stream_cap");

    ASSERT(av_bt_ctx.state == AV_BT_STATE_CONNECTED);

    for( i = 0; i < av_bt_ctx.dev.sep_count; i++ )
    {
        sep = &av_bt_ctx.dev.sep_list[i];
				mmi_trace(g_sw_BT, "i=%d,in_use=%d,media_type%d,seid=%d,sep_type=%d",\
				i,\
				av_bt_ctx.dev.sep_list[i].info.in_use,\
				av_bt_ctx.dev.sep_list[i].info.media_type,\
				av_bt_ctx.dev.sep_list[i].info.seid,\
				av_bt_ctx.dev.sep_list[i].info.sep_type);

        if( sep->cap_count == 0 )
        {
            bt_a2dp_send_capabilities_get_req(MOD_MMI, av_bt_ctx.connect_id, 
                sep->info.seid);
            av_bt_ctx.dev.sep_index = i;
            av_bt_ctx.wait = AV_BT_WAIT_GET_CAP_CNF;
            return KAL_TRUE;
        }
    }

    return KAL_FALSE;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_config_stream
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_config_stream(void)
{
    kal_bool match_cfg = KAL_FALSE;
    kal_uint8 sep_index, cap_index, i;
    av_bt_dev_struct *dev = &av_bt_ctx.dev;
    av_bt_sep_struct *sep;

    mmi_trace(g_sw_BT, "av_bt_config_stream");

    ASSERT(av_bt_ctx.state == AV_BT_STATE_CONNECTED);

	  mmi_trace(g_sw_BT, "av_bt_config_stream: 		av_bt_ctx.req_cfg[0].codec_type=%d, av_bt_ctx.req_cfg[0].codec_cap.mp3.channel_mode = %d	,av_bt_ctx.req_cfg[0].codec_cap.mp3.layer=%d\n,  av_bt_ctx.req_cfg[0].codec_cap.mp3.sample_rate =%d\n", \
	av_bt_ctx.req_cfg[0].codec_type, av_bt_ctx.req_cfg[0].codec_cap.mp3.channel_mode, av_bt_ctx.req_cfg[0].codec_cap.mp3.layer,  av_bt_ctx.req_cfg[0].codec_cap.mp3.sample_rate );

    for (i = 0; i < 2 && av_bt_ctx.req_cfg[i].codec_type; i++)
    {
    	mmi_trace(g_sw_BT, "av_bt_config_stream::av_bt_ctx.req_cfg[i].codec_type = %d", av_bt_ctx.req_cfg[i].codec_type);
        match_cfg = av_bt_match_aud_cfg(dev, &av_bt_ctx.req_cfg[i], &sep_index, &cap_index);
        if (match_cfg)
        {
        	mmi_trace(g_sw_BT,"av_bt_config_stream::av_bt_ctx.req_cfg[i].codec_type=%dsep_index=%d", av_bt_ctx.req_cfg[i].codec_type,sep_index);

		if(sep_index==0)
			av_bt_ctx.local_seid = 2;  //sbc
		else if(sep_index==1)
			av_bt_ctx.local_seid = 1;  //mp3
            av_bt_ctx.req_cfg_index = i;
            break;
        }
    }

    if (match_cfg)
    {
        mmi_trace(g_sw_BT, "match_cfg success av_bt_config_stream::av_bt_ctx.req_cfg_index=%d", av_bt_ctx.req_cfg_index);
        sep = &dev->sep_list[sep_index];
        sep->cap_index = cap_index;
        dev->sep_index = sep_index;
        memcpy(&av_bt_ctx.tmp_cfg, &av_bt_ctx.req_cfg[av_bt_ctx.req_cfg_index],
            sizeof(bt_a2dp_audio_cap_struct));

        /*the flag shall not be on before configuration*/
        if(av_bt_ctx.need_reconfig)
        {
            av_bt_ctx.need_reconfig = KAL_FALSE ;
        }

        bt_a2dp_send_stream_config_req(MOD_MMI,
            av_bt_ctx.connect_id, 
            sep->info.seid, 
            av_bt_ctx.local_seid,
            &av_bt_ctx.tmp_cfg);
        av_bt_ctx.wait = AV_BT_WAIT_CONFIG_CNF;
    }
    else
    {
        mmi_trace(g_sw_BT,"av_bt_config_stream, no match\n");

    #ifdef __MMI_BT_AUDIO_VIA_SCO__                    
        /* disable to make sure sco will not be activated*/
        mmi_profiles_bt_enable_sco_connection(MMI_PROFILE_BT_SCO_INT_A2DP);
    #endif
        av_bt_open_result(AV_BT_CALLBACK_EVENT_OPEN_FAILED);

    }
}
void av_bt_reconfig_stream(void)
{
	kal_bool match_cfg = KAL_FALSE;
	kal_uint8 sep_index, cap_index, i;
	av_bt_dev_struct *dev = &av_bt_ctx.dev;
	av_bt_sep_struct *sep;

	mmi_trace(g_sw_BT, "av_bt_reconfig_stream");

	//ASSERT(av_bt_ctx.state == AV_BT_STATE_CONNECTED);

	mmi_trace(g_sw_BT, "av_bt_reconfig_stream: 		av_bt_ctx.req_cfg[0].codec_type=%d, av_bt_ctx.req_cfg[0].codec_cap.mp3.channel_mode = %d	,av_bt_ctx.req_cfg[0].codec_cap.mp3.layer=%d\n,  av_bt_ctx.req_cfg[0].codec_cap.mp3.sample_rate =%d\n", \
	av_bt_ctx.req_cfg[0].codec_type, av_bt_ctx.req_cfg[0].codec_cap.mp3.channel_mode, av_bt_ctx.req_cfg[0].codec_cap.mp3.layer,  av_bt_ctx.req_cfg[0].codec_cap.mp3.sample_rate );

	for (i = 0; i < 2 && av_bt_ctx.req_cfg[i].codec_type; i++)
	{
		mmi_trace(g_sw_BT, "av_bt_reconfig_stream::av_bt_ctx.req_cfg[i].codec_type = %d", av_bt_ctx.req_cfg[i].codec_type);
		match_cfg = av_bt_match_aud_cfg(dev, &av_bt_ctx.req_cfg[i], &sep_index, &cap_index);
		if (match_cfg)
		{
			mmi_trace(g_sw_BT,"av_bt_reconfig_stream::av_bt_ctx.req_cfg[i].codec_type=%dsep_index=%d", av_bt_ctx.req_cfg[i].codec_type,sep_index);

			if(sep_index==0)
				av_bt_ctx.local_seid = 2;  //sbc
			else if(sep_index==1)
				av_bt_ctx.local_seid = 1;  //mp3
			av_bt_ctx.req_cfg_index = i;
			break;
		}
	}

	if (match_cfg)
	{
		mmi_trace(g_sw_BT, "match_cfg success av_bt_reconfig_stream::av_bt_ctx.req_cfg_index=%d", av_bt_ctx.req_cfg_index);
		sep = &dev->sep_list[sep_index];
		sep->cap_index = cap_index;
		dev->sep_index = sep_index;
		memcpy(&av_bt_ctx.tmp_cfg, &av_bt_ctx.req_cfg[av_bt_ctx.req_cfg_index],	sizeof(bt_a2dp_audio_cap_struct));
		mmi_trace(g_sw_BT, "av_bt_ctx.tmp_cfg.codec_type=%d",av_bt_ctx.tmp_cfg.codec_type);

		
			if(av_bt_ctx.tmp_cfg.codec_type==BT_A2DP_SBC)
		 		mmi_trace(g_sw_BT,"SBC av_bt_ctx.cf galloc_method=%d,block_len=%d, channel_mode=%d, max_bit_pool=%d, min_bit_pool=%d, sample_rate=%d, subband_num=%d",\
				av_bt_ctx.tmp_cfg.codec_cap.sbc.alloc_method,
				av_bt_ctx.tmp_cfg.codec_cap.sbc.block_len,
				av_bt_ctx.tmp_cfg.codec_cap.sbc.channel_mode,
				av_bt_ctx.tmp_cfg.codec_cap.sbc.max_bit_pool,
				av_bt_ctx.tmp_cfg.codec_cap.sbc.min_bit_pool,
				av_bt_ctx.tmp_cfg.codec_cap.sbc.sample_rate,
				av_bt_ctx.tmp_cfg.codec_cap.sbc.subband_num);
	if(av_bt_ctx.tmp_cfg.codec_type==BT_A2DP_MP3)
			 	mmi_trace(g_sw_BT,"MP3 av_bt_ctx.cfg,bit_rate=%d,channel_mode=%d, CRC=%d, layer=%d, MPF=%d,sample_rate=%d, VBR=%d,",\
				av_bt_ctx.tmp_cfg.codec_cap.mp3.bit_rate,
				av_bt_ctx.tmp_cfg.codec_cap.mp3.channel_mode,
				av_bt_ctx.tmp_cfg.codec_cap.mp3.CRC,
				av_bt_ctx.tmp_cfg.codec_cap.mp3.layer,
				av_bt_ctx.tmp_cfg.codec_cap.mp3.MPF,
				av_bt_ctx.tmp_cfg.codec_cap.mp3.sample_rate,
				av_bt_ctx.tmp_cfg.codec_cap.mp3.VBR);


	
		/*the flag shall not be on before configuration*/
		if(av_bt_ctx.need_reconfig)
		{
			av_bt_ctx.need_reconfig = KAL_FALSE ;
		}

		bt_a2dp_send_stream_reconfig_req(MOD_MMI,
											av_bt_ctx.connect_id, 
											 sep->info.seid,
											&av_bt_ctx.tmp_cfg);
		av_bt_ctx.wait = AV_BT_WAIT_RECONFIG_CNF;
	}
	else
	{
		mmi_trace(g_sw_BT,"av_bt_reconfig_stream, no match\n");
	}
	
}
void av_bt_reconfig_stream_cnf(void* msg)
{
    bt_a2dp_stream_reconfig_cnf_struct *msg_p = (bt_a2dp_stream_reconfig_cnf_struct*)msg;
   // kal_bool need_reconfig = av_bt_ctx.need_reconfig;
    kal_bool go_on = !(av_bt_ctx.need_stop || av_bt_ctx.need_reconnect);


  mmi_trace(g_sw_BT, "entry av_bt_reconfig_stream_cnf!go_on=%d,msg_p->result =%d,av_bt_ctx.wait =%d,av_bt_ctx.state=%d",\
  	go_on,msg_p->result,av_bt_ctx.wait,av_bt_ctx.state  );

    if (av_bt_ctx.wait == AV_BT_WAIT_RECONFIG_CNF)
    {
        av_bt_ctx.wait = AV_BT_WAIT_NONE_CNF;
        av_bt_ctx.need_reconfig = KAL_FALSE;

        if (msg_p->result == BT_A2DP_RESULT_OK)
        {
            av_bt_ctx.stream_handle = msg_p->stream_handle;
            av_bt_ctx.state = AV_BT_STATE_CONFIGURED;
            memcpy(&av_bt_ctx.cfg, &av_bt_ctx.tmp_cfg, sizeof(bt_a2dp_audio_cap_struct));
	     mmi_trace(g_sw_BT, "entry av_bt_reconfig_stream_cnf!av_bt_ctx.cfg=%d,mp3.bitrate=%d",av_bt_ctx.cfg.codec_type,av_bt_ctx.cfg.codec_cap.mp3.bit_rate);

        }

        if (av_bt_ctx.state == AV_BT_STATE_CONFIGURED)
        {
            if (go_on)
            {
              /*  if (need_reconfig)
                {
                    av_bt_abort_stream();
                }
                else*/
                {
                   av_bt_ctx.state = AV_BT_STATE_OPEN;
                    av_bt_start_stream(); // next step
                }
            }
        }
        else if (msg_p->result != BT_A2DP_RESULT_NO_CONNECTION)
        {
//            PRINT_INFORMATION(("av_bt_config_stream_cnf: bad_acp_seid_retry %d\n",av_bt_ctx.bad_acp_seid_retry));
            
            if(msg_p->result == BT_A2DP_RESULT_SEP_IND_USE)//zhou siyou 0927
            {
                /*disconnect and connect again to see if we could fix headset's bad state situatoin*/
               // av_bt_ctx.need_reconnect = KAL_TRUE;
              //  av_bt_ctx.sep_ind_use_retry = KAL_TRUE;
              //  av_bt_disconnect();
               // return;
                av_bt_ctx.need_stop =  TRUE ;

            }
            else if(msg_p->result == BT_A2DP_RESULT_BAD_ACP_SEID && !av_bt_ctx.bad_acp_seid_retry)
            {                 
                /*reset sep information and get all the information and config again*/
                /*config info*/
                memset(&av_bt_ctx.cfg, 0, sizeof(bt_a2dp_audio_cap_struct));
                memset(&av_bt_ctx.tmp_cfg, 0, sizeof(bt_a2dp_audio_cap_struct));
                av_bt_ctx.req_cfg_index = 0;

                /*device info, only the address part is left*/
                av_bt_ctx.dev.sep_count = 0;
                av_bt_ctx.dev.sep_index = 0;
                memset(&av_bt_ctx.dev.sep_list[0],0,MAX_NUM_REMOTE_SEIDS*sizeof(av_bt_sep_struct));

                av_bt_open_dev(KAL_TRUE);
                
                av_bt_ctx.bad_acp_seid_retry = KAL_TRUE ;
                return;
            }
            else if(msg_p->result == BT_A2DP_RESULT_BAD_STATE && !av_bt_ctx.bad_state_retry)
            {
                mmi_bth_bt_addr dummy_address = {0,0,0};
                
                /*disconnect and connect again to see if we could fix headset's bad state situatoin*/
                av_bt_ctx.need_reconnect = KAL_TRUE;
                av_bt_ctx.bad_state_retry = KAL_TRUE;
                av_bt_disconnect();
                
            #ifdef __MMI_AVRCP_SUPPORT__
                mmi_bt_avrcp_disconnect_tg_req(dummy_address,0);
            #endif
                return;
            }

            if(av_bt_ctx.sep_ind_use_retry)
            {
                PRINT_INFORMATION(("av_bt: sep_ind_use_retry, FALSE\n"));
                av_bt_ctx.sep_ind_use_retry = KAL_FALSE;
            }
            
            if(av_bt_ctx.bad_acp_seid_retry)
            {
                PRINT_INFORMATION(("av_bt: bad_acp_seid_retry, FALSE\n"));
                av_bt_ctx.bad_acp_seid_retry = KAL_FALSE ;
            }

            if(av_bt_ctx.bad_state_retry)
            {
                PRINT_INFORMATION(("av_bt: bad_state_retry, FALSE\n"));
                av_bt_ctx.bad_state_retry = KAL_FALSE;
            }
            
            if (go_on)
            {
                av_bt_open_result(AV_BT_CALLBACK_EVENT_OPEN_FAILED);
            }
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_open_stream
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_open_stream(void)
{
    mmi_trace(g_sw_BT,"av_bt_open_stream\n");

    ASSERT(av_bt_ctx.state == AV_BT_STATE_CONFIGURED);

    bt_a2dp_send_stream_open_req(MOD_MMI, av_bt_ctx.stream_handle);
    av_bt_ctx.wait = AV_BT_WAIT_OPEN_CNF;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_start_stream
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_start_stream(void)
{
    mmi_trace(g_sw_BT,"av_bt_start_stream\n");

    ASSERT(av_bt_ctx.state == AV_BT_STATE_OPEN);

    bt_a2dp_send_stream_start_req(MOD_MMI, av_bt_ctx.stream_handle);
    av_bt_ctx.wait = AV_BT_WAIT_START_CNF;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_pause_stream
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
 #ifdef MMI_ON_HARDWARE_P
extern  A2DP_codec_struct *codec;
 #else
A2DP_codec_struct *codec;
 #endif
void av_bt_pause_stream(void)
{
    PRINT_INFORMATION(("av_bt_pause_stream\n"));

    ASSERT(av_bt_ctx.state == AV_BT_STATE_STREAMING);
  
    bt_a2dp_send_stream_pause_req(MOD_MMI, av_bt_ctx.stream_handle,codec);
    av_bt_ctx.wait = AV_BT_WAIT_PAUSE_CNF;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_close_stream
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_close_stream(void)
{
    mmi_trace(1,"av_bt_close_stream\n");

    ASSERT(av_bt_ctx.state >= AV_BT_STATE_OPEN);

    // stop timer
    StopTimer(AV_BT_START_TIMER);

    bt_a2dp_send_stream_close_req(MOD_MMI, av_bt_ctx.stream_handle);
    av_bt_ctx.wait = AV_BT_WAIT_CLOSE_CNF;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_abort_stream
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_abort_stream(void)
{
    PRINT_INFORMATION(("av_bt_abort_stream\n"));

    ASSERT(av_bt_ctx.state >= AV_BT_STATE_CONFIGURED);

    // stop timer
    StopTimer(AV_BT_START_TIMER);

    bt_a2dp_send_stream_abort_req(MOD_MMI, av_bt_ctx.stream_handle);
    av_bt_ctx.wait = AV_BT_WAIT_ABORT_CNF;
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_close_or_start_stream
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_close_or_start_stream(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
mmi_trace(1,"av_bt_close_or_start_stream,mmi_audply_is_output_to_bt()=%d",mmi_audply_is_output_to_bt());
    if(!mmi_audply_is_output_to_bt())
    {
        av_bt_ctx.need_stop = KAL_TRUE;
        av_bt_start_disc_timer();
        return;
    }
    if (av_bt_cmp_req_cfg(&av_bt_ctx.cfg))
    {
        av_bt_start_stream();
    }
    else
    {
        av_bt_ctx.need_stop = KAL_TRUE;
        av_bt_start_disc_timer();
        av_bt_close_stream();
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_connect_post_proc
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_connect_post_proc(void)
{
    kal_bool is_connected = (av_bt_ctx.state == AV_BT_STATE_CONNECTED);
    mmi_trace(1,"av_bt_connect_post_proc is called,connect_post_proc%d,av_bt_ctx.need_reconnect=%dis_connected=%d",\
		av_bt_ctx.connect_post_proc,av_bt_ctx.need_reconnect,is_connected);
    if (av_bt_ctx.connect_post_proc)
    {
        av_bt_ctx.connect_post_proc = KAL_FALSE;
        if (av_bt_ctx.need_reconnect)
        {
            if (is_connected)
            {
                // check if the connected and reconnect bt addr are the same
                // if they are the same, no need to reconnect it
                if (av_bt_cmp_bt_addr(&av_bt_ctx.dev.bt_addr, &av_bt_ctx.bt_addr))
                {
                    av_bt_ctx.need_reconnect = KAL_FALSE;
                    // open dev
                    av_bt_open_dev(KAL_TRUE);
                }
                else
                {
                    av_bt_disconnect();
                }
            }
            else
            {
                av_bt_ctx.need_reconnect = KAL_FALSE;
                av_bt_connect();
            }
        }
        else if (is_connected)
        {
            av_bt_disconnect();
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_connect_cnf
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msg     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_connect_cnf(void* msg)
{
    bt_a2dp_signal_connect_cnf_struct *msg_p = (bt_a2dp_signal_connect_cnf_struct*)msg;
    kal_bool go_on = !(av_bt_ctx.need_stop || av_bt_ctx.need_reconnect || 
        av_bt_ctx.connect_post_proc);
    mmi_trace(1,"av_bt_connect_cnf go_on=%d,msg_p->result=0x%x,av_bt_ctx.wait=%d",\
		go_on,msg_p->result,av_bt_ctx.wait);
	if(!go_on)
		mmi_trace(1,"av_bt_ctx.need_reconnect =%d av_bt_ctx.connect_post_proc=%d,av_bt_ctx.need_stop=%d", \
		av_bt_ctx.need_stop,av_bt_ctx.need_reconnect ,av_bt_ctx.connect_post_proc);
    if (msg_p->result == BT_A2DP_RESULT_OK)
    {
        av_bt_ctx.connect_id = msg_p->connect_id;
        memcpy(&av_bt_ctx.bt_addr, &msg_p->device_addr, sizeof(mmi_bt_dev_addr));
        av_bt_ctx.state = AV_BT_STATE_CONNECTED;
    }
    
    if (av_bt_ctx.wait == AV_BT_WAIT_CONNECT_CNF)
    {
        av_bt_ctx.wait = AV_BT_WAIT_NONE_CNF;
        if (msg_p->result == BT_A2DP_RESULT_OK)
        {
            if (go_on)
            {
                av_bt_open_dev(KAL_TRUE);
            }
            else
            {
                av_bt_connect_post_proc();
            }
        }
        else
        {
            if (go_on)
            {
            #ifdef __MMI_BT_AUDIO_VIA_SCO__
                /* Reconnect SCO if A2DP connect fail */
            	mmi_profiles_bt_enable_sco_connection(MMI_PROFILE_BT_SCO_INT_A2DP);
            #endif
			
                av_bt_open_result(AV_BT_CALLBACK_EVENT_OPEN_FAILED);   
			
            }
            else
            {
                av_bt_connect_post_proc();
            }

			
 //       #ifdef __MMI_BT_AUDIO_VIA_SCO__
  //          if (av_bt_ctx.wait != AV_BT_WAIT_CONNECT_CNF)
   //         {
				/* Reconnect SCO if A2DP disconnect */        
  //          	mmi_profiles_bt_enable_sco_connection();
  //          }
   //     #endif

		
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_connect_ind
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msg     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_connect_ind(void *msg)
{
    bt_a2dp_signal_connect_ind_struct *msg_p = (bt_a2dp_signal_connect_ind_struct*)msg;

    memcpy(&av_bt_ctx.bt_addr, &msg_p->device_addr, sizeof(mmi_bt_dev_addr));
    av_bt_ctx.connect_id = msg_p->connect_id;
    av_bt_ctx.state = AV_BT_STATE_CONNECTED;
    av_bt_ctx.wait=AV_BT_WAIT_NONE_CNF;// zhou siyou add 20100604

    av_bt_connect_post_proc();

    /* if currently we are not output to a2dp, and post_proc hss not disconnect a2dp yet, 
       we sahll disconnect a2dp*/
    if(av_bt_ctx.wait != AV_BT_WAIT_DISCONNECT_CNF && !mmi_audply_is_output_to_bt())
    {
        /*make sure it will not connect again*/
        av_bt_ctx.need_stop = KAL_TRUE;
        av_bt_ctx.need_reconnect = KAL_FALSE;
        av_bt_disconnect();
    }

#ifdef __MMI_BT_AUDIO_VIA_SCO__
  //  if (av_bt_ctx.wait != AV_BT_WAIT_DISCONNECT_CNF)
 //   {
		/* Reconnect SCO if A2DP disconnect */        
  //  	mmi_profiles_bt_disable_sco_connection(MMI_PROFILE_BT_SCO_INT_A2DP);
  //  }
#endif


}

/*****************************************************************************
 * FUNCTION
 *  av_bt_disconnect_cnf
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msg     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_disconnect_cnf(void *msg)
{
    kal_bool need_stop = av_bt_ctx.need_stop;
    kal_bool need_reconnect = av_bt_ctx.need_reconnect;
    void(*callback)(S32 res) = av_bt_ctx.callback;

    av_bt_ctx.state = AV_BT_STATE_IDLE;
    av_bt_ctx.need_reconnect = KAL_FALSE;
    av_bt_ctx.need_stop = KAL_FALSE;
mmi_trace(1,"av_bt_disconnect_cnf,av_bt_ctx.wait=%d,need_stop=%d,need_reconnect=%d,callback=0x%x",\
	av_bt_ctx.wait,need_stop,need_reconnect,callback);
    // stop timer
    StopTimer(AV_BT_START_TIMER);
    av_bt_stop_disc_timer();

    /* disconnect ind may come right before disconnect cnf */
    if (av_bt_ctx.wait == AV_BT_WAIT_DISCONNECT_CNF)
    {
        av_bt_ctx.wait = AV_BT_WAIT_NONE_CNF;

        if (!need_stop || need_reconnect)
        {
            /* connect */
	     if(!((av_bt_ctx.dev.bt_addr.lap==0)&&(av_bt_ctx.dev.bt_addr.nap==0)&&(av_bt_ctx.dev.bt_addr.uap)==0))		
	     	{
				mmi_trace(1,"av_bt_connect will be called ");
				av_bt_connect();
	     	}
        }
    #ifdef __MMI_BT_AUDIO_VIA_SCO__
        else
        {
            /* Reconnect SCO if A2DP disconnect */    
            mmi_profiles_bt_enable_sco_connection(MMI_PROFILE_BT_SCO_INT_A2DP);        
        }
    #endif        
    }
    else
    {
        // unexpected disconnect cnf
        if (callback)
        {
            callback(AV_BT_CALLBACK_EVENT_UNEXPECTED_DISCONNECT_IND);
        }
        
    #ifdef __MMI_BT_AUDIO_VIA_SCO__
        /* Reconnect SCO if A2DP disconnect */
        mmi_profiles_bt_enable_sco_connection(MMI_PROFILE_BT_SCO_INT_A2DP);        
    #endif         
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_disconnect_ind
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msg     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_disconnect_ind(void *msg)
{
    kal_bool need_stop = av_bt_ctx.need_stop;

    av_bt_ctx.state = AV_BT_STATE_IDLE;
    av_bt_ctx.need_reconnect = KAL_FALSE;
    av_bt_ctx.need_stop = KAL_FALSE;
mmi_trace(1,"av_bt_disconnect_ind,need_stop=%d,mmi_audply_is_output_to_bt()=%d",\
	need_stop,mmi_audply_is_output_to_bt());
    // stop timer
    StopTimer(AV_BT_START_TIMER);
    av_bt_stop_disc_timer();

    if (need_stop || !mmi_audply_is_output_to_bt())
    {
        /* init cfg */

	mmi_trace(1,"av_bt_disconnect_ind, Reconnect SCO? ");	
		
        memset(&av_bt_ctx.cfg, 0, sizeof(bt_a2dp_audio_cap_struct));
        memset(&av_bt_ctx.req_cfg[0], 0, 2*sizeof(bt_a2dp_audio_cap_struct));
        av_bt_ctx.req_cfg_index = 0;

    #ifdef __MMI_BT_AUDIO_VIA_SCO__
        /* Reconnect SCO if A2DP disconnect */    
        mmi_profiles_bt_enable_sco_connection(MMI_PROFILE_BT_SCO_INT_A2DP);
    #endif
    }
    else
    {
        /* connect */
	if(!((av_bt_ctx.dev.bt_addr.lap==0)&&(av_bt_ctx.dev.bt_addr.nap==0)&&(av_bt_ctx.dev.bt_addr.uap)==0))		
	{
		mmi_trace(1,"av_bt_connect will be called ");
		av_bt_connect();
	}
	
	else
	{
				mmi_trace(1,"av_bt_connect not will becalled ");

		memset(&av_bt_ctx.cfg, 0, sizeof(bt_a2dp_audio_cap_struct));
		memset(&av_bt_ctx.req_cfg[0], 0, 2*sizeof(bt_a2dp_audio_cap_struct));
		av_bt_ctx.req_cfg_index = 0;
	}
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_discover_stream_cnf
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msg     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_discover_stream_cnf(void* msg)
{
    bt_a2dp_sep_discover_cnf_struct *msg_p = (bt_a2dp_sep_discover_cnf_struct *)msg;
    int i;
    int sep_num = (int)msg_p->sep_num;
    bt_sep_info_struct* sep_list;
    kal_bool go_on = !(av_bt_ctx.need_stop || av_bt_ctx.need_reconnect);

    mmi_trace(g_sw_BT, "av_bt_discover_stream_cnf");
	
	mmi_trace(g_sw_BT, "msg_p.sep_list[0].,in_use=%d,media_type%d,seid=%d,sep_type=%d",\
	msg_p->sep_list[0].in_use,\
	msg_p->sep_list[0].media_type,\
	msg_p->sep_list[0].seid,\
	msg_p->sep_list[0].sep_type);
	mmi_trace(g_sw_BT, "msg_p.sep_list[0].,in_use=%d,media_type%d,seid=%d,sep_type=%d",\
	msg_p->sep_list[1].in_use,\
	msg_p->sep_list[1].media_type,\
	msg_p->sep_list[1].seid,\
	msg_p->sep_list[1].sep_type);

    if (av_bt_ctx.wait == AV_BT_WAIT_DISCOVER_CNF)
    {
        av_bt_ctx.wait = AV_BT_WAIT_NONE_CNF;

        if (msg_p->result == BT_A2DP_RESULT_OK)
        {
            for (i = 0; i < sep_num; i++)
            {
                sep_list = &msg_p->sep_list[i];
                if (sep_list->sep_type == BT_A2DP_SINK && sep_list->media_type == BT_A2DP_MEDIA_AUDIO)
                {
                    memcpy(&av_bt_ctx.dev.sep_list[i].info, sep_list, sizeof(bt_sep_info_struct));
			mmi_trace(g_sw_BT, "i=%d,in_use=%d,media_type%d,seid=%d,sep_type=%d",\
				i,\
				av_bt_ctx.dev.sep_list[i].info.in_use,\
				av_bt_ctx.dev.sep_list[i].info.media_type,\
				av_bt_ctx.dev.sep_list[i].info.seid,\
				av_bt_ctx.dev.sep_list[i].info.sep_type);
	
                    av_bt_ctx.dev.sep_list[i].cap_count = 0;
                }
            }
            av_bt_ctx.dev.sep_count = (kal_uint8)i;

            if (go_on)
            {
                av_bt_open_dev(KAL_TRUE);
            }
        }
        else if (msg_p->result != BT_A2DP_RESULT_NO_CONNECTION)
        {
            if (go_on)
            {
            #ifdef __MMI_BT_AUDIO_VIA_SCO__
                /* Reconnect SCO if A2DP connect fail */
            	mmi_profiles_bt_enable_sco_connection(MMI_PROFILE_BT_SCO_INT_A2DP);
            #endif
			
                av_bt_open_result(AV_BT_CALLBACK_EVENT_OPEN_FAILED);
            }
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_discover_stream_ind
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msg     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_discover_stream_ind(void *msg)
{
    bt_a2dp_sep_discover_ind_struct *msg_p = (bt_a2dp_sep_discover_ind_struct*)msg;
    bt_sep_info_struct sep_info;

    mmi_trace(g_sw_BT, "entry av_bt_discover_stream_ind!av_bt_ctx.local_seid=%d",av_bt_ctx.local_seid);

    sep_info.in_use = (av_bt_ctx.state > AV_BT_STATE_CONNECTED);
    sep_info.seid = 1;//av_bt_ctx.local_seid;//zhou siyou change as 1;
    sep_info.media_type = BT_A2DP_MEDIA_AUDIO;
    sep_info.sep_type = BT_A2DP_SOURCE;
	
    rda_bt_a2dp_send_sep_discover_res(MOD_MMI, msg_p->connect_id, BT_A2DP_RESULT_OK, 1, &sep_info);

    //bt_a2dp_send_sep_discover_res(MOD_MMI, msg_p->connect_id, BT_A2DP_RESULT_OK, 1, &sep_info);
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_get_stream_cap_cnf
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msg     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_get_stream_cap_cnf(void *msg)
{
    bt_a2dp_capabilities_get_cnf_struct *msg_p = (bt_a2dp_capabilities_get_cnf_struct*)msg;
    av_bt_sep_struct *sep = &av_bt_ctx.dev.sep_list[av_bt_ctx.dev.sep_index];
    kal_bool go_on = !(av_bt_ctx.need_stop || av_bt_ctx.need_reconnect);

  mmi_trace(g_sw_BT, "entry av_bt_get_stream_cap_cnf!");

    if (av_bt_ctx.wait == AV_BT_WAIT_GET_CAP_CNF)
    {
        av_bt_ctx.wait = AV_BT_WAIT_NONE_CNF;

        if (msg_p->result == BT_A2DP_RESULT_OK)
        {
            sep->cap_count = msg_p->audio_cap_num;
            memcpy(&sep->cap_list[0], msg_p->audio_cap_list,
                msg_p->audio_cap_num * sizeof(bt_a2dp_audio_cap_struct));

            if (go_on)
            {
                av_bt_open_dev(KAL_TRUE);
            }
        }
        else if (msg_p->result != BT_A2DP_RESULT_NO_CONNECTION)
        {
            if (go_on)
            {
           #ifdef __MMI_BT_AUDIO_VIA_SCO__
                /* Reconnect SCO if A2DP connect fail */
            	mmi_profiles_bt_enable_sco_connection(MMI_PROFILE_BT_SCO_INT_A2DP);
            #endif
			
                av_bt_open_result(AV_BT_CALLBACK_EVENT_OPEN_FAILED);
            }
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_get_stream_cap_ind
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msg     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_get_stream_cap_ind(void *msg)
{
    bt_a2dp_capabilities_get_ind_struct *msg_p = (bt_a2dp_capabilities_get_ind_struct*)msg;
    bt_a2dp_audio_cap_struct local_caps[2];
    kal_uint8 acp_sep = msg_p->acp_seid-1;
    /* sbc */
    local_caps[0].codec_type = BT_A2DP_SBC;
    memcpy(&(local_caps[0].codec_cap.sbc), &sbc_cap[0], sizeof(bt_a2dp_sbc_codec_cap_struct));
 
    /* mp3 */
    local_caps[1].codec_type = BT_A2DP_MP3;
    memcpy(&(local_caps[1].codec_cap.mp3), &mp3_cap[0], sizeof(bt_a2dp_mp3_codec_cap_struct));
    

    mmi_trace(1, "av_bt_get_stream_cap_ind acp_sep=%d", acp_sep);
    rda_bt_a2dp_send_capabilities_get_res(MOD_MMI, msg_p->connect_id, BT_A2DP_RESULT_OK, local_caps[acp_sep].codec_type, &local_caps[acp_sep].codec_cap);




   // bt_a2dp_send_capabilities_get_res(MOD_MMI, msg_p->connect_id, BT_A2DP_RESULT_OK, 1, &local_caps[0]);
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_config_stream_cnf
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msg     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_config_stream_cnf(void* msg)
{
    bt_a2dp_stream_config_cnf_struct *msg_p = (bt_a2dp_stream_config_cnf_struct*)msg;
    kal_bool need_reconfig = av_bt_ctx.need_reconfig;
    kal_bool go_on = !(av_bt_ctx.need_stop || av_bt_ctx.need_reconnect);


  mmi_trace(g_sw_BT, "entry av_bt_config_stream_cnf!,msg_p->result =%d",msg_p->result );

    if (av_bt_ctx.wait == AV_BT_WAIT_CONFIG_CNF)
    {
        av_bt_ctx.wait = AV_BT_WAIT_NONE_CNF;
        av_bt_ctx.need_reconfig = KAL_FALSE;

        if (msg_p->result == BT_A2DP_RESULT_OK)
        {
            av_bt_ctx.stream_handle = msg_p->stream_handle;
            av_bt_ctx.state = AV_BT_STATE_CONFIGURED;
            memcpy(&av_bt_ctx.cfg, &av_bt_ctx.tmp_cfg, sizeof(bt_a2dp_audio_cap_struct));
	     mmi_trace(g_sw_BT, "entry av_bt_config_stream_cnf!av_bt_ctx.cfg=%d,mp3.bitrate=%d",av_bt_ctx.cfg.codec_type,av_bt_ctx.cfg.codec_cap.mp3.bit_rate);

        }

        if (av_bt_ctx.state == AV_BT_STATE_CONFIGURED)
        {
            if (go_on)
            {
                if (need_reconfig)
                {
                    av_bt_abort_stream();
                }
                else
                {
                    av_bt_open_stream(); // next step
                }
            }
        }
        else if (msg_p->result != BT_A2DP_RESULT_NO_CONNECTION)
        {
//            PRINT_INFORMATION(("av_bt_config_stream_cnf: bad_acp_seid_retry %d\n",av_bt_ctx.bad_acp_seid_retry));
            
            if(msg_p->result == BT_A2DP_RESULT_SEP_IND_USE)//zhou siyou 0927
            {
                /*disconnect and connect again to see if we could fix headset's bad state situatoin*/
               // av_bt_ctx.need_reconnect = KAL_TRUE;
              //  av_bt_ctx.sep_ind_use_retry = KAL_TRUE;
              //  av_bt_disconnect();
               // return;
                av_bt_ctx.need_stop =  TRUE ;

            }
            else if(msg_p->result == BT_A2DP_RESULT_BAD_ACP_SEID && !av_bt_ctx.bad_acp_seid_retry)
            {                 
                /*reset sep information and get all the information and config again*/
                /*config info*/
                memset(&av_bt_ctx.cfg, 0, sizeof(bt_a2dp_audio_cap_struct));
                memset(&av_bt_ctx.tmp_cfg, 0, sizeof(bt_a2dp_audio_cap_struct));
                av_bt_ctx.req_cfg_index = 0;

                /*device info, only the address part is left*/
                av_bt_ctx.dev.sep_count = 0;
                av_bt_ctx.dev.sep_index = 0;
                memset(&av_bt_ctx.dev.sep_list[0],0,MAX_NUM_REMOTE_SEIDS*sizeof(av_bt_sep_struct));

                av_bt_open_dev(KAL_TRUE);
                
                av_bt_ctx.bad_acp_seid_retry = KAL_TRUE ;
                return;
            }
            else if(msg_p->result == BT_A2DP_RESULT_BAD_STATE && !av_bt_ctx.bad_state_retry)
            {
                mmi_bth_bt_addr dummy_address = {0,0,0};
                
                /*disconnect and connect again to see if we could fix headset's bad state situatoin*/
                av_bt_ctx.need_reconnect = KAL_TRUE;
                av_bt_ctx.bad_state_retry = KAL_TRUE;
                av_bt_disconnect();
                
            #ifdef __MMI_AVRCP_SUPPORT__
                mmi_bt_avrcp_disconnect_tg_req(dummy_address,0);
            #endif
                return;
            }

            if(av_bt_ctx.sep_ind_use_retry)
            {
                PRINT_INFORMATION(("av_bt: sep_ind_use_retry, FALSE\n"));
                av_bt_ctx.sep_ind_use_retry = KAL_FALSE;
            }
            
            if(av_bt_ctx.bad_acp_seid_retry)
            {
                PRINT_INFORMATION(("av_bt: bad_acp_seid_retry, FALSE\n"));
                av_bt_ctx.bad_acp_seid_retry = KAL_FALSE ;
            }

            if(av_bt_ctx.bad_state_retry)
            {
                PRINT_INFORMATION(("av_bt: bad_state_retry, FALSE\n"));
                av_bt_ctx.bad_state_retry = KAL_FALSE;
            }
            
            if (go_on)
            {
          #ifdef __MMI_BT_AUDIO_VIA_SCO__
                /* Reconnect SCO if A2DP connect fail */
            	mmi_profiles_bt_enable_sco_connection(MMI_PROFILE_BT_SCO_INT_A2DP);
            #endif
			
                av_bt_open_result(AV_BT_CALLBACK_EVENT_OPEN_FAILED);
            }
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_config_stream_ind
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msg     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_config_stream_ind(void *msg)
{
	bt_a2dp_stream_config_ind_struct *msg_p = (bt_a2dp_stream_config_ind_struct*)msg;
	kal_uint16 response;
	kal_uint16 result;
	mmi_trace(g_sw_BT, "entry av_bt_config_stream_ind!av_bt_ctx.state=%d",av_bt_ctx.state);

	if (av_bt_ctx.state == AV_BT_STATE_CONNECTED)
	{
		response = BT_A2DP_RESULT_OK;
		av_bt_ctx.state = AV_BT_STATE_CONFIGURED;
		av_bt_ctx.stream_handle = msg_p->stream_handle;
		memcpy(&av_bt_ctx.cfg , &msg_p->audio_cap, sizeof(bt_a2dp_audio_cap_struct));
		if(av_bt_ctx.cfg.codec_type== BT_A2DP_SBC)
		{
			mmi_trace(1, "SBC cap sample_rate:%d, ch_mode:%x, block_len:%d, subband_num:%d, method:%d, min_pool:%d,max_pool:%d",
			av_bt_ctx.cfg.codec_cap.sbc.sample_rate ,
			av_bt_ctx.cfg.codec_cap.sbc.channel_mode ,
			av_bt_ctx.cfg.codec_cap.sbc.block_len ,
			av_bt_ctx.cfg.codec_cap.sbc.subband_num,
			av_bt_ctx.cfg.codec_cap.sbc.alloc_method,
			av_bt_ctx.cfg.codec_cap.sbc.min_bit_pool,
			av_bt_ctx.cfg.codec_cap.sbc.max_bit_pool);
		}
		else if(av_bt_ctx.cfg.codec_type == BT_A2DP_MP3)
		{
			mmi_trace(1, "MP3 cap layer:%d, CRC:%x, channel_mode:%d, MPF:%d, sample_rate:%d, VBR:%d,bit_rate:%d",
			av_bt_ctx.cfg.codec_cap.mp3.layer ,
			av_bt_ctx.cfg.codec_cap.mp3.CRC ,
			av_bt_ctx.cfg.codec_cap.mp3.channel_mode,
			av_bt_ctx.cfg.codec_cap.mp3.MPF,
			av_bt_ctx.cfg.codec_cap.mp3.sample_rate,
			av_bt_ctx.cfg.codec_cap.mp3.VBR,
			av_bt_ctx.cfg.codec_cap.mp3.bit_rate);
		}   
	}
	else
	{
		response = BT_A2DP_BAD_STATE;
	}

	if(response==BT_A2DP_RESULT_OK)
		result = BT_A2DP_MESSAGE_TYPE_ACCEPT;
	else
		result = BT_A2DP_MESSAGE_TYPE_REJECT;
	rda_bt_a2dp_send_stream_config_res(MOD_MMI, result, msg_p->stream_handle);
	// bt_a2dp_send_stream_config_res(MOD_MMI, response, msg_p->stream_handle);
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_open_stream_cnf
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msg     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_open_stream_cnf(void* msg)
{
    bt_a2dp_stream_open_cnf_struct *msg_p = (bt_a2dp_stream_open_cnf_struct*)msg;
    kal_bool need_reconfig = av_bt_ctx.need_reconfig;
    kal_bool go_on = !(av_bt_ctx.need_stop || av_bt_ctx.need_reconnect);

  mmi_trace(g_sw_BT, "entry av_bt_open_stream_cnf!");

    if (av_bt_ctx.wait == AV_BT_WAIT_OPEN_CNF)
    {
        av_bt_ctx.wait = AV_BT_WAIT_NONE_CNF;
        av_bt_ctx.need_reconfig = KAL_FALSE;

        if (msg_p->result == BT_A2DP_RESULT_OK)
        {
            av_bt_ctx.state = AV_BT_STATE_OPEN;
        }

        if (av_bt_ctx.state == AV_BT_STATE_OPEN)
        {
            if (go_on)
            {
                if (need_reconfig)
                {
                    av_bt_close_stream();
                }
                else
                {
                    av_bt_start_stream(); // next step
                }
            }
        }
        else if (msg_p->result != BT_A2DP_RESULT_NO_CONNECTION)
        {
            if (go_on)
            {
            #ifdef __MMI_BT_AUDIO_VIA_SCO__
                /* Reconnect SCO if A2DP connect fail */
            	mmi_profiles_bt_enable_sco_connection(MMI_PROFILE_BT_SCO_INT_A2DP);
            #endif
			
                av_bt_open_result(AV_BT_CALLBACK_EVENT_OPEN_FAILED);
            }
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_open_stream_ind
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msg     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_open_stream_ind(void* msg)
{
    bt_a2dp_stream_open_ind_struct *msg_p = (bt_a2dp_stream_open_ind_struct*)msg;
    kal_uint16 response;
    kal_uint16 result;
  mmi_trace(g_sw_BT, "entry av_bt_open_stream_ind!,av_bt_ctx.state=%d",av_bt_ctx.state);

    if (av_bt_ctx.state == AV_BT_STATE_CONFIGURED)
    {
        av_bt_ctx.state = AV_BT_STATE_OPEN;
        response = BT_A2DP_RESULT_OK;
        // start the timer for start streaming if remote side wouldn't send start_ind
      //  StartTimer(AV_BT_START_TIMER, 500, (FuncPtr)av_bt_close_or_start_stream);//zhou siyou
    }
    else
    {
        response = BT_A2DP_BAD_STATE;
    }
    
    if(response==BT_A2DP_RESULT_OK)
        result = BT_A2DP_MESSAGE_TYPE_ACCEPT;
    else
        result = BT_A2DP_MESSAGE_TYPE_REJECT;
    rda_bt_a2dp_send_stream_open_res(MOD_MMI, result, msg_p->stream_handle);
	
   // bt_a2dp_send_stream_open_res(MOD_MMI, response, msg_p->stream_handle);
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_start_stream_cnf
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msg     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_start_stream_cnf(void *msg)
{
    bt_a2dp_stream_start_cnf_struct *msg_p = (bt_a2dp_stream_start_cnf_struct*)msg;
    kal_bool need_reconfig = av_bt_ctx.need_reconfig;
    kal_bool go_on = !(av_bt_ctx.need_stop || av_bt_ctx.need_reconnect);

  mmi_trace(g_sw_BT, "entry av_bt_start_stream_cnf!av_bt_ctx.wait=%d,av_bt_ctx.state=%d,msg_p->result=%d",av_bt_ctx.wait,av_bt_ctx.state,msg_p->result);

    if (av_bt_ctx.wait == AV_BT_WAIT_START_CNF)
    {
        av_bt_ctx.wait = AV_BT_WAIT_NONE_CNF;
        av_bt_ctx.need_reconfig = KAL_FALSE;

        if (av_bt_ctx.state == AV_BT_STATE_STREAMING)
        {
            /* it's ACP role */
        }
        else if (msg_p->result == BT_A2DP_RESULT_OK)
        {
            av_bt_ctx.state = AV_BT_STATE_STREAMING;
            if (go_on)
            {
                if (need_reconfig)
                {
                    av_bt_close_stream();
                }
                else
                {
               #ifdef __MMI_BT_AUDIO_VIA_SCO__                    
                    /* disable to make sure sco will not be activated*/
                    mmi_profiles_bt_disable_sco_connection(MMI_PROFILE_BT_SCO_INT_A2DP);
                #endif
				
                    av_bt_open_result(AV_BT_CALLBACK_EVENT_OPEN_OK); // next step
                }
            }


			
        }
        else if (msg_p->result != BT_A2DP_RESULT_NO_CONNECTION)
        {
            if (go_on)
            {
            #ifdef __MMI_BT_AUDIO_VIA_SCO__
                /* Reconnect SCO if A2DP connect fail */
            	mmi_profiles_bt_enable_sco_connection(MMI_PROFILE_BT_SCO_INT_A2DP);
            #endif
			
                av_bt_open_result(AV_BT_CALLBACK_EVENT_OPEN_FAILED);
            }
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_start_stream_ind
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msg     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_start_stream_ind(void *msg)
{
    bt_a2dp_stream_start_ind_struct *msg_p = (bt_a2dp_stream_start_ind_struct*)msg;
    kal_uint16 response;
    void(*callback)(S32 res) = av_bt_ctx.callback;
	  kal_uint16 result;

    mmi_trace(g_sw_BT, "entry av_bt_start_stream_ind!");
    // stop timer
    StopTimer(AV_BT_START_TIMER);
    
    if (av_bt_ctx.state == AV_BT_STATE_OPEN && mmi_audply_is_output_to_bt())
    {
        av_bt_ctx.state = AV_BT_STATE_STREAMING;
        response = BT_A2DP_RESULT_OK;
    }
    else
    {
        response = BT_A2DP_BAD_STATE;
    }


  
    if(response==BT_A2DP_RESULT_OK)
        result = BT_A2DP_MESSAGE_TYPE_ACCEPT;
    else
        result = BT_A2DP_MESSAGE_TYPE_REJECT;
    rda_bt_a2dp_send_stream_start_res(MOD_MMI, result, msg_p->stream_handle);

    // let appliction know a stream is started and need to play a audio stuff
    if (callback)
    {
        callback(AV_BT_CALLBACK_EVENT_STREAM_START_IND);
    }
    else
    {
        /* default callback: currently only support audio player */
        if(av_bt_ctx.default_callbacks[0] != NULL)
        {
            av_bt_ctx.default_callbacks[0](AV_BT_CALLBACK_EVENT_STREAM_START_IND);
        }
    }
    
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_pause_stream_cnf
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msg     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_pause_stream_cnf(void *msg)
{
	bt_a2dp_stream_pause_cnf_struct *msg_p = (bt_a2dp_stream_pause_cnf_struct*)msg;
	kal_bool need_reconfig = av_bt_ctx.need_reconfig;
	kal_bool go_on = !(av_bt_ctx.need_stop || av_bt_ctx.need_reconnect);

	mmi_trace(g_sw_BT, "entry av_bt_pause_stream_cnf!av_bt_ctx.wait =%d,msg_p->result =%d.av_bt_ctx.state=%d,go_on=%d.need_reconfig=%d",\
	av_bt_ctx.wait ,msg_p->result,av_bt_ctx.state,go_on ,need_reconfig);

	if (av_bt_ctx.wait == AV_BT_WAIT_PAUSE_CNF)
	{
		av_bt_ctx.wait = AV_BT_WAIT_NONE_CNF;

		if (msg_p->result == BT_A2DP_RESULT_OK)
		{
			av_bt_ctx.state = AV_BT_STATE_OPEN;
		}
		if(new_connet_a2dp&&wait_play)
		{
			mmi_audply_press_play_button_down();
			mmi_audply_press_play_button_up();  
			wait_play=FALSE;
			new_connet_a2dp=FALSE;
		}
		else
		{
			if (av_bt_ctx.state == AV_BT_STATE_OPEN)
			{
				if (go_on)
				{
					if (need_reconfig)
					{
						//av_bt_close_stream();
						if(!av_bt_ctx.dev.sep_count)//headset first auto connect,in av_bt_ctx.dev has no it cap
							av_bt_close_stream();
						else
							av_bt_reconfig_stream();
					}
					else
					{
						av_bt_start_stream(); // next step
					}
				}
			}
			else if (msg_p->result != BT_A2DP_RESULT_NO_CONNECTION)
			{
				if (go_on)
				{
			           #ifdef __MMI_BT_AUDIO_VIA_SCO__
			                /* Reconnect SCO if A2DP connect fail */
			            	mmi_profiles_bt_enable_sco_connection(MMI_PROFILE_BT_SCO_INT_A2DP);
			            #endif
				
					av_bt_open_result(AV_BT_CALLBACK_EVENT_OPEN_FAILED);
				}
			}
		}	
	}
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_pause_stream_ind
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msg     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_pause_stream_ind(void *msg)
{
    bt_a2dp_stream_pause_ind_struct *msg_p = (bt_a2dp_stream_pause_ind_struct*)msg;
    kal_uint16 response;
    void(*callback)(S32 res) = av_bt_ctx.callback;

  mmi_trace(g_sw_BT, "entry av_bt_pause_stream_ind!");

    if (av_bt_ctx.state == AV_BT_STATE_STREAMING)
    {
        av_bt_ctx.state = AV_BT_STATE_OPEN;
        response = BT_A2DP_RESULT_OK;
    }
    else
    {
        response = BT_A2DP_BAD_STATE;
    }

    bt_a2dp_send_stream_pause_res(MOD_MMI, response, msg_p->stream_handle);

    // let appliction know a stream is paused and need to stop playing
    if (callback)
    {
        callback(AV_BT_CALLBACK_EVENT_STREAM_SUSPEND_IND);
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_close_stream_cnf
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msg     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_close_stream_cnf(void *msg)
{
    kal_bool go_on = !(av_bt_ctx.need_stop || av_bt_ctx.need_reconnect);

  mmi_trace(g_sw_BT, "entry av_bt_close_stream_cnf!");

    if (av_bt_ctx.wait == AV_BT_WAIT_CLOSE_CNF)
    {
        av_bt_ctx.wait = AV_BT_WAIT_NONE_CNF;
        av_bt_ctx.state = AV_BT_STATE_CONNECTED;
        if (go_on)
        {
            av_bt_open_dev(KAL_TRUE);
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_close_stream_ind
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msg     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_close_stream_ind(void *msg)
{
    bt_a2dp_stream_close_ind_struct *msg_p = (bt_a2dp_stream_close_ind_struct*)msg;
    void(*callback)(S32 res) = av_bt_ctx.callback;

  mmi_trace(g_sw_BT, "entry av_bt_close_stream_ind!");

    av_bt_ctx.state = AV_BT_STATE_CONNECTED;

    bt_a2dp_send_stream_close_res(MOD_MMI, BT_A2DP_RESULT_OK, msg_p->stream_handle);
    
    // stop timer
    StopTimer(AV_BT_START_TIMER);

    // let appliction know a stream is paused and need to stop playing
    if (callback)
    {
        callback(AV_BT_CALLBACK_EVENT_STREAM_CLOSE_IND);
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_abort_stream_cnf
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msg     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_abort_stream_cnf(void *msg)
{
    kal_bool go_on = !(av_bt_ctx.need_stop || av_bt_ctx.need_reconnect);

  mmi_trace(g_sw_BT, "entry av_bt_abort_stream_cnf!");

    if (av_bt_ctx.wait == AV_BT_WAIT_ABORT_CNF)
    {
        av_bt_ctx.wait = AV_BT_WAIT_NONE_CNF;
        av_bt_ctx.state = AV_BT_STATE_CONNECTED;
        if (go_on)
            av_bt_open_dev(KAL_TRUE);
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_abort_stream_ind
 * DESCRIPTION
 *  
 * PARAMETERS
 *  msg     [IN]
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_abort_stream_ind(void *msg)
{
    bt_a2dp_stream_abort_ind_struct *msg_p = (bt_a2dp_stream_abort_ind_struct*)msg;
    void(*callback)(S32 res) = av_bt_ctx.callback;

  mmi_trace(g_sw_BT, "entry av_bt_abort_stream_ind!");

    av_bt_ctx.state = AV_BT_STATE_CONNECTED;

    bt_a2dp_send_stream_abort_res(MOD_MMI, msg_p->stream_handle);
    
    // stop timer
    StopTimer(AV_BT_START_TIMER);

    // let appliction know a stream is paused and need to stop playing
    if (callback)
    {
        callback(AV_BT_CALLBACK_EVENT_STREAM_ABORT_IND);
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_inquiry_start_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_inquiry_start_callback(void)
{
    void(*callback)(S32 res) = av_bt_ctx.callback;

 mmi_trace(1,"av_bt_inquiry_start_callback: is_inquriy=%d, callback=%x,av_bt_ctx.state=%d,g_audply.state=%d\n",
        av_bt_ctx.is_inquiry, callback,av_bt_ctx.state,g_audply.state);
 if((av_bt_ctx.state==AV_BT_STATE_STREAMING)&&(g_audply.state==STATE_PLAY))
 {

    if (!av_bt_ctx.is_inquiry)
    {
        av_bt_ctx.is_inquiry = KAL_TRUE;
        if (callback)
        {
            callback(AV_BT_CALLBACK_EVENT_INQUIRY_ORCONHFP_START_IND);
        }
    }
}
}
void av_bt_connect_hfp_callback(void)//zhou siyou 091104
{
    void(*callback)(S32 res) = av_bt_ctx.callback;

 mmi_trace(1,"av_bt_connect_hfp_callback: is_inconhfp=%d, callback=%x,av_bt_ctx.state=%d,g_audply.state\n",
        av_bt_ctx.is_inconhfp, callback,av_bt_ctx.state,g_audply.state);
if((av_bt_ctx.state==AV_BT_STATE_STREAMING)&&(g_audply.state==STATE_PLAY))
{
    if (!av_bt_ctx.is_inconhfp)
    {
        av_bt_ctx.is_inconhfp = KAL_TRUE;
        if (callback)
        {
            callback(AV_BT_CALLBACK_EVENT_INQUIRY_ORCONHFP_START_IND);
        }
    }
}
}
/*****************************************************************************
 * FUNCTION
 *  av_bt_inquiry_stop_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_inquiry_stop_callback(void)
{

    void(*callback)(S32 res) = av_bt_ctx.callback;

   // PRINT_INFORMATION(("av_bt_inquiry_stop_callback: is_inquriy=%d, callback=%x\n",
    //    av_bt_ctx.is_inquiry, callback));
   mmi_trace(1,"av_bt_inquiry_stop_callback called,av_bt_ctx.is_inquiry=%d,callback=0x%x",av_bt_ctx.is_inquiry,callback);

    if (av_bt_ctx.is_inquiry)
    {
        av_bt_ctx.is_inquiry = KAL_FALSE;
        if (callback)
        {
            callback(AV_BT_CALLBACK_EVENT_INQUIRY_ORCONHFP_STOP_IND);
        }
        else
        {
            /* default callback: currently only support audio player, in cause during inquiry 
            time, callback is deleted so that application will not acting well*/
            if(av_bt_ctx.default_callbacks[0] != NULL)
            {
                av_bt_ctx.default_callbacks[0](AV_BT_CALLBACK_EVENT_INQUIRY_ORCONHFP_STOP_IND);
            }
        }
    }
}
void av_bt_conhfp_cnf_callback(void)//zhou siyou 091104
{

    void(*callback)(S32 res) = av_bt_ctx.callback;

   // PRINT_INFORMATION(("av_bt_inquiry_stop_callback: is_inquriy=%d, callback=%x\n",
    //    av_bt_ctx.is_inquiry, callback));
   mmi_trace(1,"av_bt_conhfp_cnf_callback called,av_bt_ctx.is_inconhfp=%d,callback=0x%x",av_bt_ctx.is_inconhfp,callback);

    if (av_bt_ctx.is_inconhfp)
    {
        av_bt_ctx.is_inconhfp = KAL_FALSE;
        if (callback)
        {
            callback(AV_BT_CALLBACK_EVENT_INQUIRY_ORCONHFP_STOP_IND);
        }
        else
        {
            /* default callback: currently only support audio player, in cause during inquiry 
            time, callback is deleted so that application will not acting well*/
            if(av_bt_ctx.default_callbacks[0] != NULL)
            {
                av_bt_ctx.default_callbacks[0](AV_BT_CALLBACK_EVENT_INQUIRY_ORCONHFP_STOP_IND);
            }
        }
    }
}
void av_bt_set_wait_get_cfg(void)
{
	    av_bt_ctx.wait = AV_BT_WAIT_CONNECT_CNF;// zhou siyou 1027 
	    av_bt_get_aud_cfg_ex(44100, 1, &av_bt_ctx.req_cfg[0]) ;
	  // new_connect_cfg=TRUE;
}
void If_clean_av_bt_ctx_callback(void)// zhou siyou 20100601
{
      mmi_trace(1,"If_clean_av_bt_ctx_callback is called g_audply.state=%d,av_bt_ctx.callback=0x%x,av_bt_ctx.retry_success_callback=0x%x",g_audply.state,av_bt_ctx.callback,av_bt_ctx.retry_success_callback);
	if(g_audply.state == STATE_PLAY)
		return;
	av_bt_ctx.callback=NULL;
	av_bt_ctx.retry_success_callback=NULL;

}

#ifdef __MMI_BT_AUDIO_VIA_SCO__
/*****************************************************************************
 * FUNCTION
 *  av_bt_connect_sco_callback_hdlr
 * DESCRIPTION
 *  sco connection callback hdlr
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_connect_sco_callback_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    void(*callback)(S32 res) = av_bt_ctx.callback;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

   // MMI_TRACE(MMI_MEDIA_TRC_G2_APP,MMI_AV_BT_TRC_SCO_CALLBACK,av_bt_ctx.wait,callback);
	
  mmi_trace(1,"av_bt_connect_sco_callback_hdlr ,   av_bt_ctx.wait=%d,callback=0x%x",av_bt_ctx.wait,callback);


    if(av_bt_ctx.wait == AV_BT_WAIT_SCO_CONNECT_CNF)
    {
        av_bt_ctx.wait = AV_BT_WAIT_NONE_CNF;

        if(callback)
        {
            av_bt_ctx.callback = NULL;
            av_bt_ctx.retry_success_callback = NULL;
            callback(AV_BT_CALLBACK_EVENT_OPEN_SCO);
        }
    //    mdi_audio_set_pseudo_play_file_callback(NULL);
    }
    
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_connect_hfp_callback_hdlr
 * DESCRIPTION
 *  connect HFP callback hdlr
 * PARAMETERS
 *  MMI_BOOL    [IN] indicate if connect success
 * RETURNS
 *  void
 *****************************************************************************/
void av_bt_connect_hfp_callback_hdlr(MMI_BOOL success)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    void(*callback)(S32 res) = av_bt_ctx.callback;
    MMI_BOOL output_to_bt = MMI_TRUE;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

  //  MMI_TRACE(MMI_MEDIA_TRC_G2_APP,MMI_AV_BT_TRC_HFP_CALLBACK,av_bt_ctx.wait,success,callback);

   mmi_trace(1,"av_bt_connect_hfp_callback_hdlr ,   av_bt_ctx.wait=%d, success = %d, callback=0x%x",av_bt_ctx.wait,success,callback);


    mmi_bt_suppress_profile_popup(MMI_BT_HF_PROFILE_UUID,MMI_FALSE);

    if(av_bt_ctx.wait == AV_BT_WAIT_HFP_CONNECT_CNF)
    {

        av_bt_ctx.wait = AV_BT_WAIT_NONE_CNF;

        if(!callback)
        {
            /*bt open request has been stopped*/
    //        mdi_audio_set_pseudo_play_file_callback(NULL);
            return;
        }

    #ifndef __MMI_A2DP_HIDE_BT_SETTINGS__
        output_to_bt = mmi_bt_is_aud_path_2_hf();
    #endif

        if(!success || !output_to_bt)
        {
            av_bt_ctx.callback = NULL;
            av_bt_ctx.retry_success_callback = NULL;
    //        mdi_audio_set_pseudo_play_file_callback(NULL);
            callback(AV_BT_CALLBACK_EVENT_OPEN_FAILED);
        }
        else
        {
            /*connect SCO with current hs conection*/
            if (mmi_profiles_bt_connect_sco(av_bt_connect_sco_callback_hdlr))
            {
                /*start connecting*/
                av_bt_ctx.wait = AV_BT_WAIT_SCO_CONNECT_CNF;
            }
            else
            {
                /*not connect sco for some reason*/
                av_bt_ctx.callback = NULL;
                av_bt_ctx.retry_success_callback = NULL;
      //          mdi_audio_set_pseudo_play_file_callback(NULL);
                callback(AV_BT_CALLBACK_EVENT_OPEN_FAILED);
            }   
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  av_bt_open_audio_via_sco
 * DESCRIPTION
 *  check if need to connect HFP / SCO
 * PARAMETERS
 *  void
 * RETURNS
 *  av_bt_open_result_enum    AV_BT_OPEN_WAITING: need to wait HFP / SCO connect; 
 *                            AV_BT_OPEN_FAILED: do not need to wait HFP / SCO connect
 *                            AV_BT_OPEN_DONE: sco already connectd
 *****************************************************************************/
av_bt_open_result_enum av_bt_open_audio_via_sco(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/


    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

 //  MMI_TRACE(MMI_MEDIA_TRC_G2_APP,MMI_AV_BT_TRC_AUDIO_VIA_SCO);

  mmi_trace(1,"av_bt_open_audio_via_sco ");


    if(!mmi_bt_is_default_hs_existed())
    {
        /*default earphone not existed*/
        
        if(!mmi_bth_is_hs_connected())
        {
            /*no existed hs connection*/
            return AV_BT_OPEN_FAILED;
        }

    #ifndef __MMI_A2DP_HIDE_BT_SETTINGS__
        if(!mmi_bt_is_aud_path_2_hf())
        {
           return AV_BT_OPEN_FAILED;
        }
    #endif

        if(mmi_bth_is_hs_audio_link_connected())
        {
            /* SCO already connected*/
            return AV_BT_OPEN_DONE;
        }

        /*connect SCO with current hs conection*/
        if (mmi_profiles_bt_connect_sco(av_bt_connect_sco_callback_hdlr))
        {
            /*start connecting*/
            av_bt_ctx.wait = AV_BT_WAIT_SCO_CONNECT_CNF;
            return AV_BT_OPEN_WAITING;
        }
        else
        {
            /*not connect sco for some reason*/
            return AV_BT_OPEN_FAILED;
        }
    }
    else
    {
        /*default earphone existed*/

        if(!mmi_bth_is_hs_connected())
        {
            /*no existed hs connection*/
            mmi_bt_suppress_profile_popup(MMI_BT_HF_PROFILE_UUID,MMI_TRUE);
            if(!mmi_bth_connect_default_headset(av_bt_connect_hfp_callback_hdlr))
            {
                mmi_bt_suppress_profile_popup(MMI_BT_HF_PROFILE_UUID,MMI_FALSE);
                return AV_BT_OPEN_FAILED;  
            }
            av_bt_ctx.wait = AV_BT_WAIT_HFP_CONNECT_CNF;
            return AV_BT_OPEN_WAITING;
        }

    #ifndef __MMI_A2DP_HIDE_BT_SETTINGS__
        if(!mmi_bt_is_aud_path_2_hf())
        {
           return AV_BT_OPEN_FAILED;
        }
    #endif

        if(mmi_bth_is_hs_audio_link_connected())
        {
            /* SCO already connected*/
            return AV_BT_OPEN_DONE;
        }

        /*connect SCO with current hs conection*/
        if (mmi_profiles_bt_connect_sco(av_bt_connect_sco_callback_hdlr))
        {
            /*start connecting*/
            av_bt_ctx.wait = AV_BT_WAIT_SCO_CONNECT_CNF;
            return AV_BT_OPEN_WAITING;
        }
        else
        {
            /*not connect sco for some reason*/
            return AV_BT_OPEN_FAILED;
        }

    }


}
#endif /*__MMI_BT_AUDIO_VIA_SCO__*/


#endif /* __MMI_BT_COOLSAND_SUPPORT__ && __MMI_A2DP_SUPPORT__ */
