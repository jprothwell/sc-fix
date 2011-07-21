/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
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
 * BTMMISPP.c
 *
 * Project:
 * --------
 *   Maui
 *
 * Description:
 * ------------
 *   This file is for SPP application.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
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
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#include "mmi_include.h"
#ifdef __MMI_BT_MTK_SUPPORT__
#ifdef __MMI_FAX_SUPPORT__
/***************************************************************************** 
* Included files
*****************************************************************************/
#include "protocolevents.h"
#include "commonscreens.h"
#include "idleappprot.h"  
#include "gpioinc.h"  

/* MMI Nvram access */

/* Trace Header Files */


/*Call Management related API */
#include "callmanagementstruct.h"
#include "callmanagementgprot.h"

/* MMI BT profile related files */
#include "profilesdefs.h"
#include "profilegprots.h"

/*External Device Header Files*/
#include "extdevicedefs.h"
#include "btmmicm.h"

/* HFP */
#include "bt_dm_struct.h"
#include "bluetooth_struct.h"
#include "btmmifax.h"
#include "btmmifaxgprots.h"

#include "bluetooth_bm_struct.h"
#include "btbm_adp.h"




/***************************************************************************** 
* External Variable
*****************************************************************************/

extern U8 *BTBMGetLocalStoredName(U8 *bdAddr);

extern void mmi_bth_activate_cnf_hdler(U32 profile_id);
extern void mmi_bth_deactivate_cnf_hdler(U32 profile_id);
extern void mmi_bth_connect_cnf_hdler(
                MMI_BOOL result,
                U32 lap,
                U8 uap,
                U16 nap,
                U8 *name_p /* utf8 encoding */ ,
                U32 profile_id,
                U32 connection_id);
extern void mmi_bth_connect_ind_hdler(
                U32 lap,
                U8 uap,
                U16 nap,
                U8 *name_p /* utf8 encoding */ ,
                U32 profile_id,
                U32 connection_id);
extern void mmi_bth_disconnect_cnf_hdler(MMI_BOOL result, U32 lap, U8 uap, U16 nap, U32 profile_id, U32 connection_id);
extern void mmi_bth_disconnect_ind_hdler(U32 lap, U8 uap, U16 nap, U32 profile_id, U32 connection_id);


/***************************************************************************** 
* Globol Variable
*****************************************************************************/
mmi_fax_cntx_struct g_mmi_fax_cntx[MMI_BTH_FAX_MAX_CONNECTION];

/***************************************************************************** 
* Function
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	mmi_bth_spp_init
* DESCRIPTION
*	 This function is to
*
* PARAMETERS
*	a	IN/OUT		first variable, used as returns
*	b	IN 		second variable
* RETURNS
*	NONE.
* GLOBALS AFFECTED
*	 external_global
*****************************************************************************/
void mmi_bth_fax_init(void)
{
    kal_int8 i = 0;
	mmi_bth_init_fax_event_handler();
	for(i = 0; i < MMI_BTH_FAX_MAX_CONNECTION; i++)
	{
	    g_mmi_fax_cntx[i].state = MMI_BTH_FAX_STAT_IDLE;
	    g_mmi_fax_cntx[i].connect_id = 0;
	    g_mmi_fax_cntx[i].port = 0;
        g_mmi_fax_cntx[i].lap = 0;
	    g_mmi_fax_cntx[i].uap = 0;
	    g_mmi_fax_cntx[i].nap = 0;

	}
}


/*****************************************************************************
* FUNCTION
*  mmi_bth_init_event_handler
* DESCRIPTION
*   This function is to
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  NONE.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_bth_init_fax_event_handler(void)
{
  	MMI_TRACE(MMI_TRACE_G7_MISC, MMI_BT_G7_INIT_EVENT_HDLR );

    /* Init protocol event handler */
     SetProtocolEventHandler(mmi_bth_fax_activate_cnf_hdler, MSG_ID_BT_FAX_ACTIVATE_CNF);
     SetProtocolEventHandler(mmi_bth_fax_deactivate_cnf_hdler, MSG_ID_BT_FAX_DEACTIVATE_CNF);
     SetProtocolEventHandler(mmi_bth_fax_disconnect_cnf_hdler, MSG_ID_BT_FAX_DISCONNECT_CNF);
     SetProtocolEventHandler(mmi_bth_fax_connect_ind_hdler, MSG_ID_BT_FAX_CONNECT_IND);
     SetProtocolEventHandler(mmi_bth_fax_disconnect_ind_hdler, MSG_ID_BT_FAX_DISCONNECT_IND);

    

}
/*****************************************************************************
* FUNCTION
*  mmi_bth_spp_activate_req_hdler
* DESCRIPTION
*   This function is to 
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  NONE.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_bth_fax_activate_req_hdler(void)
{
	mmi_bth_send_msg(MOD_BT, BT_APP_SAP, MSG_ID_BT_FAX_ACTIVATE_REQ, NULL, NULL);
}
/*****************************************************************************
* FUNCTION
*  mmi_bth_spp_activate_cnf_hdler
* DESCRIPTION
*   This function is to 
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  NONE.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_bth_fax_activate_cnf_hdler(void *msg)
{
    kal_uint8 i;
	
    mmi_bth_activate_cnf_hdler((U32)MMI_BTH_FAX_PROFILE_UUID);
	
	for(i = 0; i < MMI_BTH_FAX_MAX_CONNECTION; i++)
	{
	    g_mmi_fax_cntx[i].state = MMI_BTH_FAX_STAT_ACTIVATE;
	}
}
/*****************************************************************************
* FUNCTION
*  mmi_bth_spp_deactivate_req_hdler
* DESCRIPTION
*   This function is to 
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  NONE.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_bth_fax_deactivate_req_hdler(void)
{
	mmi_bth_send_msg(MOD_BT, BT_APP_SAP, MSG_ID_BT_FAX_DEACTIVATE_REQ, NULL, NULL);

	

}
/*****************************************************************************
* FUNCTION
*  mmi_bth_spp_deactivate_cnf_hdler
* DESCRIPTION
*   This function is to 
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  NONE.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_bth_fax_deactivate_cnf_hdler(void *msg)
{
    kal_uint8 i;
	
    mmi_bth_deactivate_cnf_hdler((U32)MMI_BTH_FAX_PROFILE_UUID);

	for(i = 0; i < MMI_BTH_FAX_MAX_CONNECTION; i++)
	{
	    g_mmi_fax_cntx[i].state = MMI_BTH_FAX_STAT_IDLE;
	    g_mmi_fax_cntx[i].connect_id = 0;
	    g_mmi_fax_cntx[i].port = 0;
        g_mmi_fax_cntx[i].lap = 0;
	    g_mmi_fax_cntx[i].uap = 0;
	    g_mmi_fax_cntx[i].nap = 0;
	}

}


/*****************************************************************************
* FUNCTION
*  mmi_bth_spp_connect_ind_hdler
* DESCRIPTION
*   This function is to 
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  NONE.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_bth_fax_connect_ind_hdler(void *msg)
{
    bt_fax_connect_ind_struct *msg_p = (bt_fax_connect_ind_struct *)msg;
	kal_uint8 i;
	kal_uint8 bd_addr[6];
	kal_uint8* name = NULL;
	

	for(i = 0; i < MMI_BTH_FAX_MAX_CONNECTION; i++)
	{
	    if(g_mmi_fax_cntx[i].state == MMI_BTH_FAX_STAT_ACTIVATE)
	    {
	        g_mmi_fax_cntx[i].state = MMI_BTH_FAX_STAT_CONNECT;
		    g_mmi_fax_cntx[i].connect_id = msg_p->cid;
		    g_mmi_fax_cntx[i].port = (U16)msg_p->port;
  		    g_mmi_fax_cntx[i].lap = msg_p->lap;
		    g_mmi_fax_cntx[i].uap = msg_p->uap;
		    g_mmi_fax_cntx[i].nap = msg_p->nap;
		    break;
	    }

	}

       CONVERT_BDADDR2ARRAY(bd_addr, msg_p->lap, msg_p->uap, msg_p->nap);
	name = BTBMGetLocalStoredName(bd_addr);
       //PRINT_INFORMATION("mmi_bth_spp_connect_ind_hdler(): get device name=%s",name);
	
	/* inform CM connect result */
	mmi_bth_connect_ind_hdler(  
		msg_p->lap,
		msg_p->uap,
		msg_p->nap,
		name,
		MMI_BTH_FAX_PROFILE_UUID,
		msg_p->cid);
	/* callback user profiles */


}
/*****************************************************************************
* FUNCTION
*  mmi_bth_spp_disconnect_req_hdler
* DESCRIPTION
*   This function is to 
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  NONE.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_bth_fax_disconnect_req_hdler(mmi_bth_bt_addr device_addr, U32 connect_id)
{
    kal_uint8 i;
	bt_fax_disconnect_req_struct* msg_p;

	for(i = 0; i < MMI_BTH_FAX_MAX_CONNECTION; i++)
	{
	    if(g_mmi_fax_cntx[i].connect_id == connect_id)
	    {
	        
		    msg_p = (bt_fax_disconnect_req_struct*)construct_local_para(sizeof(bt_fax_disconnect_req_struct), TD_CTRL);
		    msg_p->port = g_mmi_fax_cntx[i].port;
		
		    mmi_bth_send_msg(MOD_BT, BT_APP_SAP, MSG_ID_BT_FAX_DISCONNECT_REQ, msg_p, NULL);

		    //g_mmi_spp_cntx[i].connect_id = 0;
		    //g_mmi_spp_cntx[i].port = 0;
		    g_mmi_fax_cntx[i].state = MMI_BTH_FAX_STAT_DISCONNECT;
		    //g_mmi_spp_cntx[i].lap = 0;
		    //g_mmi_spp_cntx[i].uap = 0;
		    //g_mmi_spp_cntx[i].nap = 0;
		    break;
	    }

	}
		
}
/*****************************************************************************
* FUNCTION
*  mmi_bth_spp_disconnect_cnf_hdler
* DESCRIPTION
*   This function is to 
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  NONE.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_bth_fax_disconnect_cnf_hdler(void *msg)
{
       kal_uint8 i;	
       for(i = 0; i < MMI_BTH_FAX_MAX_CONNECTION; i++)
       {
           if(g_mmi_fax_cntx[i].state == MMI_BTH_FAX_STAT_DISCONNECT)
           {
               mmi_bth_disconnect_cnf_hdler(KAL_TRUE, 
			   	                            g_mmi_fax_cntx[i].lap,
			   	                            g_mmi_fax_cntx[i].uap,
			   	                            g_mmi_fax_cntx[i].nap, 
			   	                            MMI_BTH_FAX_PROFILE_UUID, 
			   	                            g_mmi_fax_cntx[i].connect_id);
               
	        g_mmi_fax_cntx[i].connect_id = 0;
	        g_mmi_fax_cntx[i].port = 0;
	        g_mmi_fax_cntx[i].state = MMI_BTH_FAX_STAT_ACTIVATE;
	        g_mmi_fax_cntx[i].lap = 0;
	        g_mmi_fax_cntx[i].uap = 0;
	        g_mmi_fax_cntx[i].nap = 0;
		    break;
           }
      	}
       
	//mmi_bth_send_msg(MOD_BT, BT_APP_SAP, MSG_ID_BT_SPP_ACTIVATE_REQ, NULL, NULL);

    
}
/*****************************************************************************
* FUNCTION
*  mmi_bth_spp_disconnect_ind_hdler
* DESCRIPTION
*   This function is to 
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  NONE.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_bth_fax_disconnect_ind_hdler(void *msg)
{
    bt_fax_disconnect_ind_struct *msg_p = (bt_fax_disconnect_ind_struct *)msg;
    kal_uint8 i;

    for(i = 0; i < MMI_BTH_FAX_MAX_CONNECTION; i++)
    {
        if(msg_p->port == g_mmi_fax_cntx[i].port)
        {
            if(g_mmi_fax_cntx[i].state == MMI_BTH_FAX_STAT_DISCONNECT)
            {
            
                mmi_bth_disconnect_cnf_hdler(
                   KAL_TRUE, 
	            g_mmi_fax_cntx[i].lap,
	            g_mmi_fax_cntx[i].uap,
	            g_mmi_fax_cntx[i].nap,
	            MMI_BTH_FAX_PROFILE_UUID,
	            g_mmi_fax_cntx[i].connect_id);
            }
            else
            mmi_bth_disconnect_ind_hdler(  
	            g_mmi_fax_cntx[i].lap,
	            g_mmi_fax_cntx[i].uap,
	            g_mmi_fax_cntx[i].nap,
	            MMI_BTH_FAX_PROFILE_UUID,
	            g_mmi_fax_cntx[i].connect_id);
				
            g_mmi_fax_cntx[i].connect_id = 0;
	        g_mmi_fax_cntx[i].port = 0;
	        g_mmi_fax_cntx[i].state = MMI_BTH_FAX_STAT_ACTIVATE;
	        g_mmi_fax_cntx[i].lap = 0;
	        g_mmi_fax_cntx[i].uap = 0;
	        g_mmi_fax_cntx[i].nap = 0;
		 
            break;
        }

    }


}

#endif //__MMI_FAX_SUPPORT__
#endif //__MMI_BT_MTK_SUPPORT__
