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
 * aud_bt_hfp.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes BT HFP service functions
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

#ifdef __AUD_TRACE_ON__
#include "med_trc.h"
#endif 

#ifdef __MED_BT_HFP_MOD__
#include "bt_dm_struct.h"
#include "bt_hfg_struct.h"
//#include "rda_bt_hfg.h"
#include "bt_hfg.h"
#include "dm.h"
#include "mmi_trace.h"

extern kal_bool aud_bt_hfp_sco_connect_req(void);
extern kal_bool aud_bt_hfp_sco_disconnect_req(void);
extern void mmi_bth_send_msg(U32 dest_module, U32 sap_id, U32 msg_id, void *p_local_para, void *p_peer_buff);
extern void SendAudioModeRequest(U8 mode);

typedef struct
{
    module_type src_mod_id;
    kal_uint16 connect_id;
    kal_bool disabled;  /* Disable set audio path and not allow turn on SCO */
    kal_bool audio_path_on;
    kal_uint8 state;
    kal_uint8 next_cmd;
    kal_uint8 speech_mode;
} aud_bt_hfp_context_struct;

aud_bt_hfp_context_struct aud_bt_hfp_ctx;


/*****************************************************************************
 * FUNCTION
 *  aud_bt_hfp_reset_state
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_hfp_reset_state(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
  //  AUD_BT_HFP_FUNC_ENTRY(AUD_BT_HFP_RESET_STATE)
    
    aud_bt_hfp_ctx.src_mod_id = MOD_MMI;  /* set MMI as the default receiver */
    
    AUD_BT_HFP_ENTER_STATE(AUD_BT_HFP_STATE_IDLE);
    aud_bt_hfp_ctx.audio_path_on = KAL_FALSE;
    aud_bt_hfp_ctx.next_cmd = AUD_BT_HFP_CMD_NONE;
    aud_bt_hfp_ctx.disabled = KAL_FALSE;
    aud_bt_hfp_ctx.speech_mode = BT_HFP_EARPHONE_MODE;
}


/*****************************************************************************
 * FUNCTION
 *  aud_bt_hfp_init
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_hfp_init(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    aud_bt_hfp_reset_state();
}


/*****************************************************************************
 * FUNCTION
 *  aud_bt_hfp_close_req_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ilm_ptr     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_hfp_close_req_hdlr(ilm_struct *ilm_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* media_bt_audio_close_req_struct* msg_p = (media_bt_audio_close_req_struct*)ilm_ptr->local_para_ptr; */


    aud_bt_hfp_set_audio_path(KAL_FALSE);

    if (!aud_bt_hfp_sco_disconnect_req())
    {
        aud_send_bt_audio_close_cnf(aud_bt_hfp_ctx.src_mod_id, BT_HFP, MED_RES_OK);
    }

    /* To make sure message send to BT task before MMI task continues */
    /* Because there may have sleep function when change audio path */
    AUD_SET_EVENT(AUD_EVT_BT_CLOSE_AUDIO);
}


/*****************************************************************************
 * FUNCTION
 *  aud_bt_hfp_open_req_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ilm_ptr     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_hfp_open_req_hdlr(ilm_struct *ilm_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    media_bt_audio_open_req_struct *msg_p = (media_bt_audio_open_req_struct*) ilm_ptr->local_para_ptr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    
    aud_bt_hfp_ctx.src_mod_id = ilm_ptr->src_mod_id;
    aud_bt_hfp_ctx.connect_id = msg_p->connect_id;
    aud_bt_hfp_ctx.speech_mode = msg_p->mode;
	
    mmi_trace(1,"aud_bt_hfp_open_req_hdlr aud_bt_hfp_ctx.state=%d\n",aud_bt_hfp_ctx.state);
	
    if( aud_bt_hfp_ctx.state == AUD_BT_HFP_STATE_IDLE )
    {
        ASSERT(aud_bt_hfp_ctx.next_cmd == AUD_BT_HFP_CMD_NONE);

        aud_bt_hfp_sco_connect_req();
    }
    else
    {
        aud_send_bt_audio_open_cnf(aud_bt_hfp_ctx.src_mod_id, BT_HFP, MED_RES_BUSY);
    }
}


/*****************************************************************************
 * FUNCTION
 *  aud_bt_hfp_turn_on_req_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ilm_ptr     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_hfp_turn_on_req_hdlr(ilm_struct *ilm_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    /* Turn on path if path is not turn on yet */
    if(!aud_bt_hfp_ctx.audio_path_on)
    {
        aud_bt_hfp_ctx.audio_path_on = KAL_TRUE;
        aud_bt_hfp_set_audio_path_on();
    }
}


/*****************************************************************************
 * FUNCTION
 *  aud_bt_hfp_turn_off_req_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ilm_ptr     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_hfp_turn_off_req_hdlr(ilm_struct *ilm_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    /* Turn off path if path is on */
    if(aud_bt_hfp_ctx.audio_path_on)
    {
        aud_bt_hfp_ctx.audio_path_on = KAL_FALSE;
        aud_bt_hfp_set_audio_path_off();
    }

    /* To make sure message send to BT task before MMI task continues */
    /* Because there may have sleep function when change audio path */
    AUD_SET_EVENT(AUD_EVT_BT_TURN_OFF_AUDIO);
}


/*****************************************************************************
 * FUNCTION
 *  aud_bt_hfp_sco_connect_req
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
kal_bool aud_bt_hfp_sco_connect_req(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_bool processed = KAL_TRUE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (aud_bt_hfp_ctx.next_cmd == AUD_BT_HFP_CMD_NONE)
    {
        if (aud_bt_hfp_ctx.state == AUD_BT_HFP_STATE_IDLE)
        {
            /* Set path before connect request */
            //aud_bt_hfp_set_audio_path(KAL_TRUE);
//#if defined(__MMI_BT_RDA_SUPPORT__) 
            {
                bt_hfg_sco_connect_req_struct  *msg_req;
                msg_req = (bt_hfg_sco_connect_req_struct *)construct_local_para( (kal_uint16)sizeof(bt_hfg_sco_connect_req_struct), TD_CTRL);
                msg_req->connection_id = aud_bt_hfp_ctx.connect_id;
                mmi_bth_send_msg(MOD_BT, BT_APP_SAP, MSG_ID_MEDIA_BT_AUDIO_OPEN_REQ, (void *)msg_req, NULL);
            }
//#else
     //       bt_hfg_send_sco_connect_req(MOD_MED, aud_bt_hfp_ctx.connect_id);
//#endif
        }
        else
        {
            processed = KAL_FALSE;
        }
    }

    if (processed)
    {
        aud_bt_hfp_ctx.next_cmd = AUD_BT_HFP_CMD_CONNECT_SCO;
    }

    return processed;
}


/*****************************************************************************
 * FUNCTION
 *  aud_bt_hfp_sco_connect_cnf_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ilm_ptr     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_hfp_sco_connect_cnf_hdlr(ilm_struct *ilm_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    bt_hfg_sco_connect_cnf_struct *msg_p = (bt_hfg_sco_connect_cnf_struct*) ilm_ptr->local_para_ptr;
    kal_bool next_disc = (aud_bt_hfp_ctx.next_cmd == AUD_BT_HFP_CMD_DISCONNECT_SCO);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    
    aud_bt_hfp_ctx.next_cmd = AUD_BT_HFP_CMD_NONE;

    if (msg_p->result == BT_HFG_RESULT_OK)
    {
        AUD_BT_HFP_ENTER_STATE(AUD_BT_HFP_STATE_OPEN);

        aud_send_bt_audio_open_ind(aud_bt_hfp_ctx.src_mod_id, BT_HFP);

        if (next_disc)
        {
            aud_bt_hfp_sco_disconnect_req();
        }
        else
        {
        	/* check if it's disabled */
		    if (aud_bt_hfp_ctx.disabled)
		    {
	        	return;
	        }

            /* Set path after the connect request */
            aud_bt_hfp_set_audio_path(KAL_TRUE);
        }
    }
    else
    {
       
        aud_send_bt_audio_open_cnf(aud_bt_hfp_ctx.src_mod_id, BT_HFP, MED_RES_FAIL);

        if (next_disc)
        {
            aud_send_bt_audio_close_ind(aud_bt_hfp_ctx.src_mod_id, BT_HFP, MED_RES_OK);
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  aud_bt_hfp_sco_connect_ind_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ilm_ptr     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_hfp_sco_connect_ind_hdlr(ilm_struct *ilm_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* bt_hfg_sco_connect_ind_struct *msg_p = (bt_hfg_sco_connect_ind_struct*)ilm_ptr->local_para_ptr; */
    kal_bool next_disc = (aud_bt_hfp_ctx.next_cmd == AUD_BT_HFP_CMD_DISCONNECT_SCO);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    
    ASSERT(aud_bt_hfp_ctx.state == AUD_BT_HFP_STATE_IDLE);

    aud_bt_hfp_ctx.next_cmd = AUD_BT_HFP_CMD_NONE;
    AUD_BT_HFP_ENTER_STATE(AUD_BT_HFP_STATE_OPEN);

    aud_send_bt_audio_open_ind(aud_bt_hfp_ctx.src_mod_id, BT_HFP);

    if (next_disc)
    {
        aud_bt_hfp_sco_disconnect_req();
    }
#ifdef __MED_BT_FM_VIA_SCO__
    /* Can not accept SCO connect while FM recording. */
    else if(aud_fmr_is_power_on() &&
           (aud_context_p->state == AUD_MEDIA_RECORD || aud_context_p->state == AUD_MEDIA_RECORD_PAUSED))
    {
        aud_bt_hfp_sco_disconnect_req();    
    }
#endif /* __MED_BT_FM_VIA_SCO__ */
    else
    {
    	/* check if it's disabled */
	    if (aud_bt_hfp_ctx.disabled)
	    {
        	return;
        }
        
        aud_bt_hfp_set_audio_path(KAL_TRUE);
    }
}


/*****************************************************************************
 * FUNCTION
 *  aud_bt_hfp_sco_disconnect_req
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
kal_bool aud_bt_hfp_sco_disconnect_req(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_bool processed = KAL_TRUE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (aud_bt_hfp_ctx.next_cmd == AUD_BT_HFP_CMD_NONE)
    {
        if (aud_bt_hfp_ctx.state == AUD_BT_HFP_STATE_OPEN)
        {
//#if defined(__MMI_BT_RDA_SUPPORT__)       
            {
                bt_hfg_sco_disconnect_req_struct  *msg_req;
                msg_req = (bt_hfg_sco_disconnect_req_struct *)construct_local_para( (kal_uint16)sizeof(bt_hfg_sco_disconnect_req_struct), TD_CTRL);
                msg_req->connection_id = aud_bt_hfp_ctx.connect_id;
                mmi_bth_send_msg(MOD_BT, BT_APP_SAP, MSG_ID_MEDIA_BT_AUDIO_CLOSE_REQ, (void *)msg_req, NULL);
            }
//#else
 //           bt_hfg_send_sco_disconnect_req(MOD_MED, aud_bt_hfp_ctx.connect_id);
//#endif
        }
        else
        {
            processed = KAL_FALSE;
        }
    }

    if (processed)
    {
        aud_bt_hfp_ctx.next_cmd = AUD_BT_HFP_CMD_DISCONNECT_SCO;
    }

    return processed;
}


/*****************************************************************************
 * FUNCTION
 *  aud_bt_hfp_sco_disconnect_cnf_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ilm_ptr     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_hfp_sco_disconnect_cnf_hdlr(ilm_struct *ilm_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    bt_hfg_sco_disconnect_cnf_struct *msg_p = (bt_hfg_sco_disconnect_cnf_struct*) ilm_ptr->local_para_ptr;
    kal_uint16 result;
    kal_bool next_conn = (aud_bt_hfp_ctx.next_cmd == AUD_BT_HFP_CMD_CONNECT_SCO);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//	AUD_BT_HFP_FUNC();
	mmi_trace(g_sw_BT, " aud_bt_hfp_ctx.state=%d, aud_bt_hfp_ctx.audio_path_on=%d,  msg_p->result=%d", 
						 aud_bt_hfp_ctx.state, aud_bt_hfp_ctx.audio_path_on,  msg_p->result);

    if (aud_bt_hfp_ctx.state == AUD_BT_HFP_STATE_IDLE)
    {
        return;
    }

    aud_bt_hfp_ctx.next_cmd = AUD_BT_HFP_CMD_NONE;
    AUD_BT_HFP_ENTER_STATE(AUD_BT_HFP_STATE_IDLE);

    if (msg_p->result == BT_HFG_RESULT_OK)
    {
        result = MED_RES_OK;
    }
    else
    {
        result = MED_RES_FAIL;
    }

    aud_bt_hfp_set_audio_path(KAL_FALSE);

    aud_send_bt_audio_close_ind(aud_bt_hfp_ctx.src_mod_id, BT_HFP, result);

	/* check if it's disabled */
    if (aud_bt_hfp_ctx.disabled)
    {
        return;
    }

    if (next_conn)
    {
        aud_bt_hfp_sco_connect_req();
    }
}


/*****************************************************************************
 * FUNCTION
 *  aud_bt_hfp_sco_disconnect_ind_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ilm_ptr     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_hfp_sco_disconnect_ind_hdlr(ilm_struct *ilm_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* bt_hfg_sco_disconnect_ind_struct *msg_p = (bt_hfg_sco_disconnect_ind_struct*)ilm_ptr->local_para_ptr; */
    kal_bool next_conn = (aud_bt_hfp_ctx.next_cmd == AUD_BT_HFP_CMD_CONNECT_SCO);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

	mmi_trace(g_sw_BT, " aud_bt_hfp_sco_disconnect_ind_hdlr  aud_bt_hfp_ctx.state=%d, aud_bt_hfp_ctx.audio_path_on=%d", 
						 aud_bt_hfp_ctx.state, aud_bt_hfp_ctx.audio_path_on);

  

    if (aud_bt_hfp_ctx.state == AUD_BT_HFP_STATE_IDLE)
    {
        return;    
    }

    aud_bt_hfp_ctx.next_cmd = AUD_BT_HFP_CMD_NONE;
    AUD_BT_HFP_ENTER_STATE(AUD_BT_HFP_STATE_IDLE);

    aud_bt_hfp_set_audio_path(KAL_FALSE);

    aud_send_bt_audio_close_ind(aud_bt_hfp_ctx.src_mod_id, BT_HFP, MED_RES_HFP_CLOSE);

	/* check if it's disabled */
    if (aud_bt_hfp_ctx.disabled)
    {
        return;
    }

    if (next_conn)
    {
        aud_bt_hfp_sco_connect_req();
    }
}


/*****************************************************************************
 * FUNCTION
 *  aud_bt_hfp_is_audio_path_on
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
kal_bool aud_bt_hfp_is_audio_path_on(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//	AUD_BT_HFP_FUNC();
	mmi_trace(g_sw_BT, " aud_bt_hfp_is_audio_path_on aud_bt_hfp_ctx.state=%d, aud_bt_hfp_ctx.audio_path_on=%d", 
						 aud_bt_hfp_ctx.state, aud_bt_hfp_ctx.audio_path_on);

    return aud_bt_hfp_ctx.audio_path_on;
}


/*****************************************************************************
 * FUNCTION
 *  aud_bt_hfp_set_audio_path_on
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/

extern void mmc_audio_bt_sco(bool flag);

void aud_bt_hfp_set_audio_path_on(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//	AUD_BT_HFP_FUNC();
	mmi_trace(g_sw_BT, " aud_bt_hfp_set_audio_path_on aud_bt_hfp_ctx.state=%d, aud_bt_hfp_ctx.audio_path_on=%d", 
						 aud_bt_hfp_ctx.state, aud_bt_hfp_ctx.audio_path_on);


   
    /* Set speech mode*/


    if (aud_context_p->speech_on)
    {

//	    DM_GetAudioMode(&pre_audio_mode);
	    mmi_trace(g_sw_BT ,"  Turn audio path to blutooth!!!     ");

   	  SendAudioModeRequest( AUD_MODE_BLUETOOTH ); 


    }
    else
        {
         mmc_audio_bt_sco(TRUE);

        }
	
}


/*****************************************************************************
 * FUNCTION
 *  aud_bt_hfp_set_audio_path_off
 * DESCRIPTION
 *  
 * PARAMETERS
 *  on      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_hfp_set_audio_path_off(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
 //   kal_uint8 audio_path = 0;
   
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//	AUD_BT_HFP_FUNC();
	mmi_trace(g_sw_BT, " aud_bt_hfp_ctx.state=%d, aud_bt_hfp_ctx.audio_path_on=%d,aud_context_p->audio_mode= %d", 
						 aud_bt_hfp_ctx.state, aud_bt_hfp_ctx.audio_path_on,aud_context_p->audio_mode );
    
      /* Set speech output device */
//	ASSERT( (aud_context_p->audio_mode >=AUD_MODE_NORMAL)&&( aud_context_p->audio_mode <= AUD_MODE_LOUDSPK));
        if (aud_context_p->speech_on)
            {
        	  mmi_trace(g_sw_BT ,"  Turn audio path to Handset!!!     ");
        	//SendAudioModeRequest( aud_context_p->audio_mode );
                 SendAudioModeRequest(aud_context_p->audio_mode);//AUD_MODE_NORMAL 

        }
         else
            {
                mmc_audio_bt_sco(FALSE);
            }

}


/*****************************************************************************
 * FUNCTION
 *  aud_bt_hfp_set_audio_path
 * DESCRIPTION
 *  
 * PARAMETERS
 *  on      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void aud_bt_hfp_set_audio_path(kal_bool on)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//	AUD_BT_HFP_FUNC();
	mmi_trace(g_sw_BT, " aud_bt_hfp_ctx.state=%d, aud_bt_hfp_ctx.audio_path_on=%d, on=%d", 
						 aud_bt_hfp_ctx.state, aud_bt_hfp_ctx.audio_path_on, on);

    if ((aud_bt_hfp_ctx.audio_path_on && !on) || (!aud_bt_hfp_ctx.audio_path_on && on))
    {
        aud_bt_hfp_ctx.audio_path_on = on;
    
        if (on)
        {
            aud_bt_hfp_set_audio_path_on();
        }
        /* Off */
        else
        {
            aud_bt_hfp_set_audio_path_off();
        }
    }
}

#endif /* __MED_BT_HFP_MOD__ */ 

#endif /* MED_NOT_PRESENT */ 

