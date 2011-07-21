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
/*****************************************************************************
 *
 * Filename:
 * ---------
 * BTMMIHfp.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is the header file for HFP application.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef BT_MMI_HFP_H
#define BT_MMI_HFP_H

//#ifndef BT_DM_STRUCT_H
//#error "bt_dm_struct.h should be included"
//#endif

#ifndef BT_MMI_CM_H
#error "BTMMICm.h should be included"
#endif

 
/***************************************************************************** 
* Macro
*****************************************************************************/
#define MMI_BTH_HFG_LOG_STATE_TRANS(x)                                                        \
{                                                                                             \
}

//    MMI_TRACE(MMI_TRACE_G7_MISC, MMI_BT_G7_BT_HFG_STATE_TRANS, g_mmi_bth_hfg_cntx.state, x);

#define MMI_BTH_HFG_STATE_TRANS(x)  \
{                                   \
    MMI_BTH_HFG_LOG_STATE_TRANS(x); \
    g_mmi_bth_hfg_cntx.state = x;   \
}

/***************************************************************************** 
* Definations
*****************************************************************************/
#define MMI_BTH_HFG_MAX_CONNECTION        1

/***************************************************************************** 
* typedef 
*****************************************************************************/
typedef enum
{
	 MMI_BTH_HFG_STAT_IDLE,
    MMI_BTH_HFG_STAT_ACTIVATED,
    MMI_BTH_HFG_STAT_CONNECTED,

    MMI_BTH_HFG_STAT_ACTIVATING,
    MMI_BTH_HFG_STAT_DEACTIVATING,
    MMI_BTH_HFG_STAT_CONNECTING,
    MMI_BTH_HFG_STAT_DISCONNECTING
} mmi_bth_hfg_state_enum;

/***************************************************************************** 
* Structure
*****************************************************************************/
typedef struct
{
	 /*  Connection identification  */
	 MMI_BOOL						active;
	 U32								conn_id;
    //U32                                                       connected_device_cod;
    //U8                                                                name[MMI_BTH_BD_FNAME_LEN];
	 mmi_bth_bt_addr	 			bd_addr;
    /*  Feature config */
	 U32								supported_features;
	 /*  Connection status	*/
	 U8								speaker_gain;
	 U8								mic_gain;
	 MMI_BOOL						audio_on;
	 
}mmi_bth_hfg_connect_struct;

typedef struct
{
	 /* hfg state */
	 mmi_bth_hfg_state_enum			state;
	 /* hfg authorise setting */	
	 U8					 				authorise;   
	 /* Features supported by device */
	 U16									own_supported_features; 
	 /* active connections */
    U8 active_conn_num;
    mmi_bth_hfg_connect_struct active_conn_list[MMI_BTH_HFG_MAX_CONNECTION];
 
} mmi_bth_hfg_cntx_struct;

/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/

/***************************************************************************** 
* Functions
*****************************************************************************/
extern void mmi_bth_init_hfp_event_handler(void);
extern void mmi_bth_hfg_activate_cnf_hdler(void *msg);
extern void mmi_bth_hfg_deactivate_cnf_hdler(void *msg);
extern void mmi_bth_hfg_connect_cnf_hdler(void *msg);
extern void mmi_bth_hfg_connect_ind_hdler(void *msg);
extern void mmi_bth_hfg_disconnect_cnf_hdler(void *msg);
extern void mmi_bth_hfg_disconnect_ind_hdler(void *msg);
extern void mmi_bth_hfg_speaker_gain_cnf_hdler(void *msg);
extern void mmi_bth_hfg_speaker_gain_ind_hdler(void *msg);
extern void mmi_bth_hfg_mic_gain_cnf_hdler(void *msg);
extern void mmi_bth_hfg_mic_gain_ind_hdler(void *msg);
extern MMI_BOOL mmi_bth_hfg_disconnect_all(void);
extern void mmi_bth_hfg_hangup_req_hdler(void *msg);
extern void mmi_bth_hfg_incoming_accept_req_hdler(void *msg);
extern void mmi_bth_hfg_incoming_reject_req_hdler(void *msg);
extern void mmi_bth_hfg_redial_lastnum_req_hdler(void *msg);

#endif /* BT_MMI_HFP_H */ /* end of BT_MMI_HFP_H */

