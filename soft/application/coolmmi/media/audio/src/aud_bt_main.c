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
 * aud_bt_main.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes BT service functions
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
//#include "kal_trace.h"
//#include "stack_common.h"
//#include "stack_msgs.h"
#include "app_ltlcom.h" /* task message communiction */
//#include "syscomp_config.h"
//#include "task_config.h"        /* task creation */
#include "stacklib.h"   /* basic type for dll, evshed, stacktimer */
#include "event_shed.h" /* event scheduler */
#include "stack_timer.h"        /* stack timer */

/* global includes */
#include "l1audio.h"
#include "device.h"
#include "resource_audio.h"
#include "fat_fs.h"
//#include "nvram_enums.h"
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
#include "mmi_trace.h"

#ifdef __AUD_TRACE_ON__
#include "med_trc.h"
#endif 

extern void aud_bt_a2dp_open_codec_req_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_close_codec_req_hdlr(ilm_struct *ilm_ptr);

#ifdef __MED_BT_MOD__
#ifdef __MED_BT_A2DP_MOD__
extern void aud_bt_a2dp_init(void);
extern void aud_bt_a2dp_stream_start_req(void);
extern kal_uint16 aud_bt_a2dp_stream_reconfig_req(void);
extern void aud_bt_a2dp_stream_close_req(void);
extern void aud_bt_a2dp_stream_pause_req(void);
extern void aud_bt_a2dp_stream_abort_req(void);
extern void aud_bt_a2dp_close_req_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_open_req_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_turn_on_req_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_turn_off_req_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_sep_discover_cnf_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_sep_discover_ind_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_capabilities_get_cnf_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_capabilities_get_ind_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_stream_config_cnf_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_stream_config_ind_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_stream_open_cnf_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_stream_open_ind_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_stream_start_cnf_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_stream_start_ind_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_stream_reconfig_cnf_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_stream_reconfig_ind_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_stream_pause_cnf_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_stream_pause_ind_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_stream_close_cnf_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_stream_close_ind_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_stream_abort_cnf_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_stream_abort_ind_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_a2dp_stream_qos_ind_hdlr(ilm_struct *ilm_ptr);
#endif /* __MED_BT_A2DP_MOD__ */ 
extern void aud_bt_avrcp_status_ind_hdlr(ilm_struct *ilm_ptr);

#ifdef __MED_BT_HFP_MOD__
extern void aud_bt_hfp_init(void);
extern void aud_bt_hfp_close_req_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_hfp_open_req_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_hfp_sco_connect_cnf_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_hfp_sco_connect_ind_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_hfp_sco_disconnect_cnf_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_hfp_sco_disconnect_ind_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_hfp_turn_on_req_hdlr(ilm_struct *ilm_ptr);
extern void aud_bt_hfp_turn_off_req_hdlr(ilm_struct *ilm_ptr);
#endif /* __MED_BT_HFP_MOD__ */ 


/*****************************************************************************
 * FUNCTION
 *  aud_bt_init
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_init(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MED_BT_HFP_MOD__
    aud_bt_hfp_init();
#endif 

#ifdef __MED_BT_A2DP_MOD__
    aud_bt_a2dp_init();
#endif 
}


/*****************************************************************************
 * FUNCTION
 *  aud_bt_audio_open_req_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ilm_ptr     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_audio_open_req_hdlr(ilm_struct *ilm_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_bt_audio_open_req_struct *msg_p = (media_bt_audio_open_req_struct*) ilm_ptr->local_para_ptr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_BT,"In  aud_bt_audio_open_req_hdlr!! ");
	
    switch (msg_p->profile)
    {
    #ifdef __MED_BT_HFP_MOD__
        case BT_HFP:
            aud_bt_hfp_open_req_hdlr(ilm_ptr);
            break;
    #endif /* __MED_BT_HFP_MOD__ */ 
    #if defined(__BTVCSR_HCI_BCHS__) && defined(__MED_BT_A2DP_MOD__)
        case BT_A2DP:
            aud_bt_a2dp_open_req_hdlr(ilm_ptr);
            break;
    #endif /* __BTVCSR_HCI_BCHS__ && __MED_BT_A2DP_MOD__ */ 
        default:
            ASSERT(0);
    }
}


/*****************************************************************************
 * FUNCTION
 *  aud_bt_audio_close_req_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ilm_ptr     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_audio_close_req_hdlr(ilm_struct *ilm_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_bt_audio_close_req_struct *msg_p = (media_bt_audio_close_req_struct*) ilm_ptr->local_para_ptr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (msg_p->profile)
    {
    #ifdef __MED_BT_HFP_MOD__
        case BT_HFP:
            aud_bt_hfp_close_req_hdlr(ilm_ptr);
            break;
    #endif /* __MED_BT_HFP_MOD__ */ 
    #if defined(__BTVCSR_HCI_BCHS__) && defined(__MED_BT_A2DP_MOD__)
        case BT_A2DP:
            aud_bt_a2dp_close_req_hdlr(ilm_ptr);
            break;
    #endif /* __BTVCSR_HCI_BCHS__ && __MED_BT_A2DP_MOD__ */ 
        default:
            ASSERT(0);
    }
}


/*****************************************************************************
 * FUNCTION
 *  aud_bt_audio_turn_on_req_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ilm_ptr     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_audio_turn_on_req_hdlr(ilm_struct *ilm_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_bt_audio_turn_on_req_struct *msg_p = (media_bt_audio_turn_on_req_struct*) ilm_ptr->local_para_ptr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (msg_p->profile)
    {
    #ifdef __MED_BT_HFP_MOD__
        case BT_HFP:
            aud_bt_hfp_turn_on_req_hdlr(ilm_ptr);
            break;
    #endif /* __MED_BT_HFP_MOD__ */ 
    #if defined(__BTVCSR_HCI_BCHS__) && defined(__MED_BT_A2DP_MOD__)
        case BT_A2DP:
            aud_bt_a2dp_turn_on_req_hdlr(ilm_ptr);
            break;
    #endif /* __BTVCSR_HCI_BCHS__ && __MED_BT_A2DP_MOD__ */ 
        default:
            ASSERT(0);
    }
}


/*****************************************************************************
 * FUNCTION
 *  aud_bt_audio_turn_off_req_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ilm_ptr     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_audio_turn_off_req_hdlr(ilm_struct *ilm_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_bt_audio_turn_off_req_struct *msg_p = (media_bt_audio_turn_off_req_struct*) ilm_ptr->local_para_ptr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (msg_p->profile)
    {
    #ifdef __MED_BT_HFP_MOD__
        case BT_HFP:
            aud_bt_hfp_turn_off_req_hdlr(ilm_ptr);
            break;
    #endif /* __MED_BT_HFP_MOD__ */ 
    #if defined(__BTVCSR_HCI_BCHS__) && defined(__MED_BT_A2DP_MOD__)
        case BT_A2DP:
            aud_bt_a2dp_turn_off_req_hdlr(ilm_ptr);
            break;
    #endif /* __BTVCSR_HCI_BCHS__ && __MED_BT_A2DP_MOD__ */ 
        default:
            ASSERT(0);
    }
}


/*****************************************************************************
 * FUNCTION
 *  aud_bt_main
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ilm_ptr     [?]     
 * RETURNS
 *  
 *****************************************************************************/
kal_bool aud_bt_main(ilm_struct *ilm_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

       mmi_trace(g_sw_BT,"<aud_bt_main> msg_id: %d \r\n", ilm_ptr->msg_id ); 

	
    switch (ilm_ptr->msg_id)
    {
    #ifdef __MED_BT_HFP_MOD__
        case MSG_ID_BT_HFG_SCO_CONNECT_CNF:
            aud_bt_hfp_sco_connect_cnf_hdlr(ilm_ptr);
            break;
        case MSG_ID_BT_HFG_SCO_CONNECT_IND:
            aud_bt_hfp_sco_connect_ind_hdlr(ilm_ptr);
            break;
        case MSG_ID_BT_HFG_SCO_DISCONNECT_CNF:
            aud_bt_hfp_sco_disconnect_cnf_hdlr(ilm_ptr);
            break;
        case MSG_ID_BT_HFG_SCO_DISCONNECT_IND:
            aud_bt_hfp_sco_disconnect_ind_hdlr(ilm_ptr);
            break;
    #endif /* __MED_BT_HFP_MOD__ */ 
    #if defined(__BTVCSR_HCI_BCHS__) && defined(__MED_BT_A2DP_MOD__)
        case MSG_ID_BT_A2DP_SEP_DISCOVER_CNF:
            aud_bt_a2dp_sep_discover_cnf_hdlr(ilm_ptr);
            break;
        case MSG_ID_BT_A2DP_SEP_DISCOVER_IND:
            aud_bt_a2dp_sep_discover_ind_hdlr(ilm_ptr);
            break;
        case MSG_ID_BT_A2DP_CAPABILITIES_GET_CNF:
            aud_bt_a2dp_capabilities_get_cnf_hdlr(ilm_ptr);
            break;
        case MSG_ID_BT_A2DP_CAPABILITIES_GET_IND:
            aud_bt_a2dp_capabilities_get_ind_hdlr(ilm_ptr);
            break;
        case MSG_ID_BT_A2DP_STREAM_CONFIG_CNF:
            aud_bt_a2dp_stream_config_cnf_hdlr(ilm_ptr);
            break;
        case MSG_ID_BT_A2DP_STREAM_CONFIG_IND:
            aud_bt_a2dp_stream_config_ind_hdlr(ilm_ptr);
            break;
        case MSG_ID_BT_A2DP_STREAM_RECONFIG_CNF:
            aud_bt_a2dp_stream_reconfig_cnf_hdlr(ilm_ptr);
            break;
        case MSG_ID_BT_A2DP_STREAM_RECONFIG_IND:
            aud_bt_a2dp_stream_reconfig_ind_hdlr(ilm_ptr);
            break;
        case MSG_ID_BT_A2DP_STREAM_OPEN_CNF:
            aud_bt_a2dp_stream_open_cnf_hdlr(ilm_ptr);
            break;
        case MSG_ID_BT_A2DP_STREAM_OPEN_IND:
            aud_bt_a2dp_stream_open_ind_hdlr(ilm_ptr);
            break;
        case MSG_ID_BT_A2DP_STREAM_START_CNF:
            aud_bt_a2dp_stream_start_cnf_hdlr(ilm_ptr);
            break;
        case MSG_ID_BT_A2DP_STREAM_START_IND:
            aud_bt_a2dp_stream_start_ind_hdlr(ilm_ptr);
            break;
        case MSG_ID_BT_A2DP_STREAM_PAUSE_CNF:
            aud_bt_a2dp_stream_pause_cnf_hdlr(ilm_ptr);
            break;
        case MSG_ID_BT_A2DP_STREAM_PAUSE_IND:
            aud_bt_a2dp_stream_pause_ind_hdlr(ilm_ptr);
            break;
        case MSG_ID_BT_A2DP_STREAM_QOS_IND:
            aud_bt_a2dp_stream_qos_ind_hdlr(ilm_ptr);
            break;
        case MSG_ID_BT_A2DP_STREAM_CLOSE_CNF:
            aud_bt_a2dp_stream_close_cnf_hdlr(ilm_ptr);
            break;
        case MSG_ID_BT_A2DP_STREAM_CLOSE_IND:
            aud_bt_a2dp_stream_close_ind_hdlr(ilm_ptr);
            break;
        case MSG_ID_BT_A2DP_STREAM_ABORT_CNF:
            aud_bt_a2dp_stream_abort_cnf_hdlr(ilm_ptr);
            break;
        case MSG_ID_BT_A2DP_STREAM_ABORT_IND:
            aud_bt_a2dp_stream_abort_ind_hdlr(ilm_ptr);
            break;
        case MSG_ID_MEDIA_BT_AVRCP_STATUS_IND:
            aud_bt_avrcp_status_ind_hdlr(ilm_ptr);
            break;
    #endif /* __BTVCSR_HCI_BCHS__ && __MED_BT_A2DP_MOD__ */ 
        case MSG_ID_MEDIA_BT_AUDIO_OPEN_REQ:
            aud_bt_audio_open_req_hdlr(ilm_ptr);
            break;
        case MSG_ID_MEDIA_BT_AUDIO_CLOSE_REQ:
            aud_bt_audio_close_req_hdlr(ilm_ptr);
            break;
        case MSG_ID_MEDIA_BT_AUDIO_TURN_ON_REQ:
            aud_bt_audio_turn_on_req_hdlr(ilm_ptr);
            break;
        case MSG_ID_MEDIA_BT_AUDIO_TURN_OFF_REQ:
            aud_bt_audio_turn_off_req_hdlr(ilm_ptr);
            break;
    #if defined(__BTCOOLSAND__) && defined(__MED_BT_A2DP_MOD__)
        case MSG_ID_BT_A2DP_STREAM_QOS_IND:
            aud_bt_a2dp_stream_qos_ind_hdlr(ilm_ptr);
            break;
        case MSG_ID_MEDIA_BT_OPEN_CODEC_REQ:
            aud_bt_a2dp_open_codec_req_hdlr(ilm_ptr);
            break;
        case MSG_ID_MEDIA_BT_CLOSE_CODEC_REQ:
            aud_bt_a2dp_close_codec_req_hdlr(ilm_ptr);
            break;
    #endif /* __BTCOOLSAND__ && __MED_BT_A2DP_MOD__ */
        default:
            return KAL_FALSE;
    }

    return KAL_TRUE;
}
#endif /* __MED_BT_MOD__ */ 

#endif /* MED_NOT_PRESENT */ 

