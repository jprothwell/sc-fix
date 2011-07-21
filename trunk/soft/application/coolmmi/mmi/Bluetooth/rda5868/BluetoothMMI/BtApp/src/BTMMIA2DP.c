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
 * BTMMIA2DP.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is for Bluetooth A2DP MMI Connector
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

/* Trace Header Files */
//#include "kal_trace.h"  /* kal_trace() prototype */
#include "debuginitdef.h"       /* define trace class type */
//#include "mmi_trc.h"    /* trace map definition */

/* avrcp header files */
#include "bt_a2dp_struct.h"
#include "bt_a2dp.h"


#include "btmmicm.h"
#include "btmmicmgprots.h"
#include "btmmia2dp.h"
#include "profilesdefs.h"
#include "profilegprots.h"

extern void av_bt_connect_cnf(void* msg);
extern void av_bt_connect_ind(void *msg);
extern void av_bt_disconnect_cnf(void *msg);
extern void av_bt_disconnect_ind(void *msg);
extern void mmi_profiles_bt_deactivate_cnf_hdlr(void);

/***************************************************************************** 
* External Variable
*****************************************************************************/

/***************************************************************************** 
* Globol Variable
*****************************************************************************/
mmi_a2dp_cntx_struct g_mmi_a2dp_cntx;

/***************************************************************************** 
* External Functions
*****************************************************************************/

/***************************************************************************** 
* Functions
*****************************************************************************/


/*****************************************************************************
 * FUNCTION
 *  mmi_bt_a2dp_init
 * DESCRIPTION
 *  init a2dp connector
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_bt_a2dp_init(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_A2DP_STATE_TRANS(MMI_A2DP_STATE_IDLE);

    SetProtocolEventHandler(mmi_bt_a2dp_activate_cnf_hdlr, MSG_ID_BT_A2DP_ACTIVATE_CNF);
    SetProtocolEventHandler(mmi_bt_a2dp_deactivate_cnf_hdlr, MSG_ID_BT_A2DP_DEACTIVATE_CNF);
    SetProtocolEventHandler(mmi_bt_a2dp_connect_cnf_hdlr, MSG_ID_BT_A2DP_SIGNAL_CONNECT_CNF);
    SetProtocolEventHandler(mmi_bt_a2dp_connect_ind_hdlr, MSG_ID_BT_A2DP_SIGNAL_CONNECT_IND);
    SetProtocolEventHandler(mmi_bt_a2dp_disconnect_cnf_hdlr, MSG_ID_BT_A2DP_SIGNAL_DISCONNECT_CNF);
    SetProtocolEventHandler(mmi_bt_a2dp_disconnect_ind_hdlr, MSG_ID_BT_A2DP_SIGNAL_DISCONNECT_IND);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_bt_a2dp_activate_req
 * DESCRIPTION
 *  send a activate req to bluetooth task
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_bt_a2dp_activate_req(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ASSERT(g_mmi_a2dp_cntx.state == MMI_A2DP_STATE_IDLE);

    bt_a2dp_send_activate_req(MOD_MMI, BT_A2DP_SOURCE);
 
    MMI_A2DP_STATE_TRANS(MMI_A2DP_STATE_ACTIVATING);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_bt_a2dp_activate_cnf_hdlr
 * DESCRIPTION
 *  handle a activate cnf from bluetooth task
 * PARAMETERS
 *  msg     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_bt_a2dp_activate_cnf_hdlr(void *msg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    bt_a2dp_activate_cnf_struct *msg_p = (bt_a2dp_activate_cnf_struct*) msg;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* Should not activate fail */
    if (msg_p->result == BT_A2DP_RESULT_OK)
    {
        if (g_mmi_a2dp_cntx.state == MMI_A2DP_STATE_ACTIVATING)
        {
            MMI_A2DP_STATE_TRANS(MMI_A2DP_STATE_ACTIVATED);

            mmi_bth_activate_cnf_hdler(MMI_BTH_ADVANCED_AUDIO_PROFILE_UUID);
        }
    }
    else
    {
        ASSERT(0);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_bt_a2dp_deactivate_req
 * DESCRIPTION
 *  if it's in 'DEACTIVATED' state, then send a deactivate req to bluetooth task;
 *  otherwise, move on to 'DEACTIVATING' state and wait for connect or disconnect cnf
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_bt_a2dp_deactivate_req(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     mmi_trace(g_sw_BT,"mmi_bt_a2dp_deactivate_req is called g_mmi_a2dp_cntx.state=%d",g_mmi_a2dp_cntx.state);
    switch (g_mmi_a2dp_cntx.state)
    {
        case MMI_A2DP_STATE_ACTIVATED:
            bt_a2dp_send_deactivate_req(MOD_MMI);
            MMI_A2DP_STATE_TRANS(MMI_A2DP_STATE_DEACTIVATING);
            break;
        case MMI_A2DP_STATE_CONNECTED:
             bt_a2dp_send_signal_disconnect_req(MOD_MMI, g_mmi_a2dp_cntx.connect_id);
            MMI_A2DP_STATE_TRANS(MMI_A2DP_STATE_DEACTIVATING);

            break;
        case MMI_A2DP_STATE_CONNECTING:
        case MMI_A2DP_STATE_DISCONNECTING:
            MMI_A2DP_STATE_TRANS(MMI_A2DP_STATE_DEACTIVATING);
            break;
	default:
	    break;
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_bt_a2dp_deactivate_cnf_hdlr
 * DESCRIPTION
 *  This function is to handle a deactivate cnf from bluetooth task
 * PARAMETERS
 *  msg     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_bt_a2dp_deactivate_cnf_hdlr(void *msg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    bt_a2dp_deactivate_cnf_struct *msg_p = (bt_a2dp_deactivate_cnf_struct*) msg;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_mmi_a2dp_cntx.state == MMI_A2DP_STATE_DEACTIVATING)
    {
        if (msg_p->result == BT_A2DP_RESULT_OK || msg_p->result == BT_A2DP_RESULT_PANIC)
        {
            MMI_A2DP_STATE_TRANS(MMI_A2DP_STATE_IDLE);

            mmi_bth_deactivate_cnf_hdler(MMI_BTH_ADVANCED_AUDIO_PROFILE_UUID);
        #ifdef __MMI_BT_PROFILE__
            mmi_profiles_bt_deactivate_cnf_hdlr();
        #endif
        }
        else
        {
            ASSERT(0);
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_bt_a2dp_connect_req_by_cm
 * DESCRIPTION
 *  send a connect req to bluetooth task
 * PARAMETERS
 *  dev_addr        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
 extern void av_bt_set_wait_get_cfg(void);
extern void If_clean_av_bt_ctx_callback(void);
void mmi_bt_a2dp_connect_req_by_cm(mmi_bth_bt_addr dev_addr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    bt_device_addr_struct bd_addr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	mmi_trace(1,"mmi_bt_a2dp_connect_req_by_cm g_mmi_a2dp_cntx.state=%d",g_mmi_a2dp_cntx.state);
    if(g_mmi_a2dp_cntx.state != MMI_A2DP_STATE_ACTIVATED)
	 mmi_trace(1,"mmi_bt_a2dp_connect_req_by_cm g_mmi_a2dp_cntx.state not  is MMI_A2DP_STATE_ACTIVATED !!!!!!!!!!!!!!!!!!!");
  //  ASSERT(g_mmi_a2dp_cntx.state == MMI_A2DP_STATE_ACTIVATED);

    bd_addr.lap = dev_addr.lap;
    bd_addr.uap = dev_addr.uap;
    bd_addr.nap = dev_addr.nap;
   av_bt_set_wait_get_cfg();
   If_clean_av_bt_ctx_callback();

    g_mmi_a2dp_cntx.conn_req_flag |= MMI_A2DP_REQ_FLAG_CM;

   mmi_trace(g_sw_BT,"mmi_bt_a2dp_connect_req_by_cm:: send connect req message");
    bt_a2dp_send_signal_connect_req(MOD_MMI, &bd_addr, BT_A2DP_SOURCE);

    MMI_A2DP_STATE_TRANS(MMI_A2DP_STATE_CONNECTING);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_bt_a2dp_connect_req
 * DESCRIPTION
 *  send a connect req to bluetooth task
 * PARAMETERS
 *  dev_addr        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_bt_a2dp_connect_req(mmi_bth_bt_addr dev_addr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    bt_device_addr_struct bd_addr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ASSERT(g_mmi_a2dp_cntx.state == MMI_A2DP_STATE_ACTIVATED);

    bd_addr.lap = dev_addr.lap;
    bd_addr.uap = dev_addr.uap;
    bd_addr.nap = dev_addr.nap;

    g_mmi_a2dp_cntx.conn_req_flag |= MMI_A2DP_REQ_FLAG_APP;

    bt_a2dp_send_signal_connect_req(MOD_MMI, &bd_addr, BT_A2DP_SOURCE);

    MMI_A2DP_STATE_TRANS(MMI_A2DP_STATE_CONNECTING);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_bt_a2dp_connect_cnf_hdlr
 * DESCRIPTION
 *  handle a connect cnf from bluetooth task
 * PARAMETERS
 *  msg     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_bt_a2dp_connect_cnf_hdlr(void *msg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    bt_a2dp_signal_connect_cnf_struct *msg_p = (bt_a2dp_signal_connect_cnf_struct*) msg;
    bt_device_addr_struct *bd_addr = &msg_p->device_addr;
    U8 flag = g_mmi_a2dp_cntx.conn_req_flag;
    BOOL success;
    U8 res;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_mmi_a2dp_cntx.conn_req_flag = 0;
    mmi_trace(g_sw_BT,"Entry mmi_bt_a2dp_connect_cnf_hdlr::g_mmi_a2dp_cntx.state=%d", g_mmi_a2dp_cntx.state);
    if (msg_p->result == BT_A2DP_RESULT_OK)
    {
        success = TRUE;
        res = MMI_PROFILES_BT_OK;
        g_mmi_a2dp_cntx.bd_addr.lap = bd_addr->lap;
        g_mmi_a2dp_cntx.bd_addr.uap = bd_addr->uap;
        g_mmi_a2dp_cntx.bd_addr.nap = bd_addr->nap;
        g_mmi_a2dp_cntx.connect_id = msg_p->connect_id;
    }
    else
    {
        success = FALSE;
        res = MMI_PROFILES_BT_FAIL;
    }

    switch (g_mmi_a2dp_cntx.state)
    {
        case MMI_A2DP_STATE_CONNECTING:
            if (success)
            {
                MMI_A2DP_STATE_TRANS(MMI_A2DP_STATE_CONNECTED);
            }
            else
            {
                MMI_A2DP_STATE_TRANS(MMI_A2DP_STATE_ACTIVATED);
            }
            /* inform CM connect result */
            if (flag & MMI_A2DP_REQ_FLAG_CM)
            {
                mmi_bth_connect_cnf_hdler(
                    success,
                    bd_addr->lap,
                    bd_addr->uap,
                    bd_addr->nap,
                    NULL,
                    MMI_BTH_ADVANCED_AUDIO_PROFILE_UUID,
                    msg_p->connect_id);
            }
            else if (success)
            {
                mmi_bth_connect_ind_hdler(
                    bd_addr->lap,
                    bd_addr->uap,
                    bd_addr->nap,
                    NULL,
                    MMI_BTH_ADVANCED_AUDIO_PROFILE_UUID,
                    msg_p->connect_id);
            }
            av_bt_connect_cnf(msg);
            
            break;
        case MMI_A2DP_STATE_DEACTIVATING:
            av_bt_connect_cnf(msg);

            if (success)
            {
                bt_a2dp_send_signal_disconnect_req(MOD_MMI, msg_p->connect_id);
            }
            else    /* send a activate req */
            {
                bt_a2dp_send_deactivate_req(MOD_MMI);
            }
            break;
        default:
            //ASSERT(0);
            /*the assert because in many cases, the cnf is fail because bad state 
              so we shall just neglect this cnf*/
            break;
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_bt_a2dp_connect_ind_hdlr
 * DESCRIPTION
 *  handle a connect ind from bluetooth task
 * PARAMETERS
 *  msg     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_bt_a2dp_connect_ind_hdlr(void *msg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    bt_a2dp_signal_connect_ind_struct *msg_p = (bt_a2dp_signal_connect_ind_struct*) msg;
    bt_device_addr_struct *bd_addr = &msg_p->device_addr;
    U8 flag = g_mmi_a2dp_cntx.conn_req_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_mmi_a2dp_cntx.bd_addr.lap = bd_addr->lap;
    g_mmi_a2dp_cntx.bd_addr.uap = bd_addr->uap;
    g_mmi_a2dp_cntx.bd_addr.nap = bd_addr->nap;
    g_mmi_a2dp_cntx.conn_req_flag = 0;
	If_clean_av_bt_ctx_callback();

   mmi_trace(g_sw_BT, "mmi_bt_a2dp_connect_ind_hdlr::g_mmi_a2dp_cntx.state=%d", g_mmi_a2dp_cntx.state);
    switch (g_mmi_a2dp_cntx.state)
    {
        case MMI_A2DP_STATE_ACTIVATED:
            MMI_A2DP_STATE_TRANS(MMI_A2DP_STATE_CONNECTED);
            /* inform CM connect result */
            mmi_bth_connect_ind_hdler(
                bd_addr->lap,
                bd_addr->uap,
                bd_addr->nap,
                NULL,
                MMI_BTH_ADVANCED_AUDIO_PROFILE_UUID,
                msg_p->connect_id);
            av_bt_connect_ind(msg);

            break;
        case MMI_A2DP_STATE_CONNECTING:
            MMI_A2DP_STATE_TRANS(MMI_A2DP_STATE_CONNECTED);
            /* inform CM connect result */
            if (flag & MMI_A2DP_REQ_FLAG_CM)
            {
                mmi_bth_connect_cnf_hdler(
                    TRUE,
                    bd_addr->lap,
                    bd_addr->uap,
                    bd_addr->nap,
                    NULL,
                    MMI_BTH_ADVANCED_AUDIO_PROFILE_UUID,
                    msg_p->connect_id);
            }
            else
            {
                mmi_bth_connect_ind_hdler(
                    bd_addr->lap,
                    bd_addr->uap,
                    bd_addr->nap,
                    NULL,
                    MMI_BTH_ADVANCED_AUDIO_PROFILE_UUID,
                    msg_p->connect_id);
            }
            av_bt_connect_ind(msg);

            break;
        case MMI_A2DP_STATE_DEACTIVATING:
            av_bt_connect_ind(msg);

            /* send a activate req */
            bt_a2dp_send_signal_disconnect_req(MOD_MMI, msg_p->connect_id);
            break;
        default:
            ASSERT(0);
            break;
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_bt_a2dp_disconnect_req_by_cm
 * DESCRIPTION
 *  send a disconnect req to bluetooh task
 * PARAMETERS
 *  dev_addr        [IN]        
 *  connect_id      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_bt_a2dp_disconnect_req_by_cm(mmi_bth_bt_addr dev_addr, U32 connect_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_mmi_a2dp_cntx.disc_req_flag |= MMI_A2DP_REQ_FLAG_CM;

    switch (g_mmi_a2dp_cntx.state)
    {
        case MMI_A2DP_STATE_CONNECTED:
            bt_a2dp_send_signal_disconnect_req(MOD_MMI, connect_id);
            MMI_A2DP_STATE_TRANS(MMI_A2DP_STATE_DISCONNECTING);
            break;
        case MMI_A2DP_STATE_DEACTIVATING:
            break;
        case MMI_A2DP_STATE_DISCONNECTING:
            break;
        default:
            ASSERT(0);
            break;
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_bt_a2dp_disconnect_req
 * DESCRIPTION
 *  send a disconnect req to bluetooh task
 * PARAMETERS
 *  dev_addr        [IN]        
 *  connect_id      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_bt_a2dp_disconnect_req(mmi_bth_bt_addr dev_addr, U32 connect_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_mmi_a2dp_cntx.disc_req_flag |= MMI_A2DP_REQ_FLAG_APP;

    switch (g_mmi_a2dp_cntx.state)
    {
        case MMI_A2DP_STATE_CONNECTED:
            bt_a2dp_send_signal_disconnect_req(MOD_MMI, connect_id);
            MMI_A2DP_STATE_TRANS(MMI_A2DP_STATE_DISCONNECTING);
            break;
        case MMI_A2DP_STATE_DEACTIVATING:
            break;
        case MMI_A2DP_STATE_DISCONNECTING:
            break;
        default:
            ASSERT(0);
            break;
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_bt_a2dp_disconnect_cnf_hdlr
 * DESCRIPTION
 *  handle a disconnect cnf from bluetooth task
 * PARAMETERS
 *  msg     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_bt_a2dp_disconnect_cnf_hdlr(void *msg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    bt_a2dp_signal_disconnect_cnf_struct *msg_p = (bt_a2dp_signal_disconnect_cnf_struct*) msg;
    bt_device_addr_struct *bd_addr = (bt_device_addr_struct*) & g_mmi_a2dp_cntx.bd_addr;
    U8 flag = g_mmi_a2dp_cntx.disc_req_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_mmi_a2dp_cntx.disc_req_flag = 0;
mmi_trace(1,"mmi_bt_a2dp_disconnect_cnf_hdlr   g_mmi_a2dp_cntx.state   =%d",g_mmi_a2dp_cntx.state);
    switch (g_mmi_a2dp_cntx.state)
    {
        case MMI_A2DP_STATE_DISCONNECTING:
            MMI_A2DP_STATE_TRANS(MMI_A2DP_STATE_ACTIVATED);
            /* inform CM connect result */
            if (flag & MMI_A2DP_REQ_FLAG_CM)
            {
                mmi_bth_disconnect_cnf_hdler(
                    TRUE,
                    bd_addr->lap,
                    bd_addr->uap,
                    bd_addr->nap,
                    MMI_BTH_ADVANCED_AUDIO_PROFILE_UUID,
                    msg_p->connect_id);
            }
            else
            {
                mmi_bth_disconnect_ind_hdler(
                    bd_addr->lap,
                    bd_addr->uap,
                    bd_addr->nap,
                    MMI_BTH_ADVANCED_AUDIO_PROFILE_UUID,
                    msg_p->connect_id);
            }
            av_bt_disconnect_cnf(msg);
            break;
        case MMI_A2DP_STATE_DEACTIVATING:
            av_bt_disconnect_cnf(msg);
	     mmi_bth_disconnect_cnf_hdler(
            TRUE,
            bd_addr->lap,
            bd_addr->uap,
            bd_addr->nap,
            MMI_BTH_ADVANCED_AUDIO_PROFILE_UUID,
            msg_p->connect_id);
            /* send a activate req */
            bt_a2dp_send_deactivate_req(MOD_MMI);
            break;
        default:
            break;
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_bt_a2dp_disconnect_ind_hdlr
 * DESCRIPTION
 *  handle a disconnect ind from bluetooth task
 * PARAMETERS
 *  msg     [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_bt_a2dp_disconnect_ind_hdlr(void *msg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    bt_a2dp_signal_disconnect_ind_struct *msg_p = (bt_a2dp_signal_disconnect_ind_struct*) msg;
    bt_device_addr_struct *bd_addr = (bt_device_addr_struct*) & g_mmi_a2dp_cntx.bd_addr;
    U8 flag = g_mmi_a2dp_cntx.disc_req_flag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_mmi_a2dp_cntx.disc_req_flag = 0;
mmi_trace(1,"mmi_bt_a2dp_disconnect_ind_hdlr,g_mmi_a2dp_cntx.state=%d",g_mmi_a2dp_cntx.state);
    switch (g_mmi_a2dp_cntx.state)
    {
        case MMI_A2DP_STATE_CONNECTED:
            MMI_A2DP_STATE_TRANS(MMI_A2DP_STATE_ACTIVATED);
            /* inform CM connect result */
            mmi_bth_disconnect_ind_hdler(
                bd_addr->lap,
                bd_addr->uap,
                bd_addr->nap,
                MMI_BTH_ADVANCED_AUDIO_PROFILE_UUID,
                msg_p->connect_id);
            av_bt_disconnect_ind(msg);
            break;
        case MMI_A2DP_STATE_DISCONNECTING:
            MMI_A2DP_STATE_TRANS(MMI_A2DP_STATE_ACTIVATED);
            /* inform CM connect result */
            if (flag & MMI_A2DP_REQ_FLAG_CM)
            {
                mmi_bth_disconnect_cnf_hdler(
                    TRUE,
                    bd_addr->lap,
                    bd_addr->uap,
                    bd_addr->nap,
                    MMI_BTH_ADVANCED_AUDIO_PROFILE_UUID,
                    msg_p->connect_id);
            }
            else
            {
                mmi_bth_disconnect_ind_hdler(
                    bd_addr->lap,
                    bd_addr->uap,
                    bd_addr->nap,
                    MMI_BTH_ADVANCED_AUDIO_PROFILE_UUID,
                    msg_p->connect_id);
            }
            av_bt_disconnect_ind(msg);
            break;
        case MMI_A2DP_STATE_DEACTIVATING:
            av_bt_disconnect_ind(msg);
            /* send a activate req */
            bt_a2dp_send_deactivate_req(MOD_MMI);
            break;
        default:
            break;
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_bt_a2dp_query_state
 * DESCRIPTION
 *  query a2dp state
 * PARAMETERS
*   voide    
 * RETURNS
 *  void
 *****************************************************************************/
mmi_a2dp_state_enum mmi_bt_a2dp_query_state()
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

//    PRINT_INFORMATION("mmi_bt_a2dp_query_state: %d\n", g_mmi_a2dp_cntx.state);
    
    return g_mmi_a2dp_cntx.state;
    
}
U8 mmi_bt_is_a2dp_connect(void)
{
      mmi_trace(1,"mmi_bt_is_a2dp_connect is called g_mmi_a2dp_cntx.state=%d",g_mmi_a2dp_cntx.state);
	if(g_mmi_a2dp_cntx.state==MMI_A2DP_STATE_CONNECTED)
		return 1;
	return 0;
}
#endif /* defined(__MMI_BT_COOLSAND_SUPPORT__) && defined(__MMI_A2DP_SUPPORT__) */ 

