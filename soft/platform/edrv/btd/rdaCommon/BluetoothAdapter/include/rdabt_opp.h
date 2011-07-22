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
 *
 * Project:
 * --------
 * MAUI
 *
 * Description:
 * ------------
 * This file defines global types and function prototypes for memory management utilities
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
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#ifndef _RDA_OPP_H
#define _RDA_OPP_H
#include "uart_sw.h"
#include "bluetooth_struct.h"

/***************************************************************************** 
* Define
*****************************************************************************/

#define MGR_HCI_CONNREJECT_REASON 0x0D /* Reason code used for automatic rejection - 0x0d=limited resources*/
#define RDA_FTPS_Server_Channel 0x03 /*  */
#define RDA_OPPS_Server_Channel 0x04 /*  */
#define Timer_Re_Out_CREDITS 0x20 /*  */

/***************************************************************************** 
* Typedef 
*****************************************************************************/

typedef enum
{
    GOEP_Client_Idle,            /* GOEP Initial State */
    GOEP_Client_Connect,        /* Connection established state */
    GOEP_Client_Disconnect,   /* GOEP is disconnecting the connection == GOEP_Client_active_rfcomm,*/
    GOEP_Client_pending_rfcomm,    
    GOEP_Client_pending_sdp,
    GOEP_Client_wait,
    GOEP_Client_pending_Disconnect,
} mmi_GOEP_state_enum;

////////////////opp/ftp client//////////////////////
typedef struct
{
   kal_uint32 goep_tidClient;  /* goep_tid of bt */
   kal_uint32 goep_tidClient_mmi;    /* goep_tid of mmi */
   kal_uint32 goep_cidClient;
   kal_uint32 req_idClient;
   t_bdaddr ClientAddress;
   kal_uint8 ClientStatus;
   kal_uint8 isFtpClient;
   kal_uint8* buf_ptrFTC;
   kal_uint16 buf_sizeFTC;
   kal_uint8 FTCfirst;
}rdabt_client_struct;

////////////////opp server//////////////////////
typedef struct
{
   kal_uint32 goep_tid_OPPS;  /* goep_tid of bt */
   kal_uint32 req_idOPPS;    /* goep_tid of mmi */
   kal_uint32 OPPStid;
   kal_uint8* buf_ptrOPPS;
   kal_uint16 buf_sizeOPPS;
   t_bdaddr OPPSaddress;
   kal_uint8 opps_obj_name[80];
   kal_uint16 OPPSmax_len;
   kal_uint16 OPPSoffsetVar;
   kal_uint8 OPPSstatus;
   kal_uint8 OPPSdlci;
   kal_uint8 NAME_obex;
   kal_uint8 OPPSfirst;
}rdabt_opps_struct;

////////////////ftp server//////////////////////
typedef struct
{
   kal_uint32 goep_tid_FTPS;    /* goep_tid of mmi */
   kal_uint32 req_idFTPS;
   kal_uint32 FTPStid;
   kal_uint8* buf_ptrFTPS;
   kal_uint16 buf_sizeFTPS;
   t_bdaddr FTPSaddress;
   kal_uint8 ftps_objs_mime_type[80];
   kal_uint8 ftps_obj_name[256];
   kal_uint16 FTPSmax_len;
   kal_uint16 FTPSoffsetVar;
   kal_uint16 FTPS_offset_len;
   kal_uint8 FTPSstatus;
   kal_uint8 FTPSfirst;
   kal_uint8 FTPSdlci;
}rdabt_ftps_struct;

/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/
extern void rdabt_goep_connect_req(t_bdaddr address,goep_connect_req_struct *msg_p);
extern void rdabt_goep_disconnect_req(goep_disconnect_req_struct *msg_p);
extern void rdabt_goep_push_req(goep_push_req_struct *msg_p);
extern void rdabt_goep_abort_req(goep_abort_req_struct *msg_p);
extern void rdabt_goep_register_server_req(goep_register_server_req_struct *msg_p);
extern void rdabt_bt_sdpdb_register_req(bt_sdpdb_register_req_struct *msg_p);
extern void rdabt_bt_link_connect_accept_rsp(bt_bm_link_connect_accept_rsp_struct *msg_p);
extern void rdabt_goep_authorize_res(goep_authorize_res_struct *msg_p);
extern void rdabt_goep_connect_res(goep_connect_res_struct *msg_p);
extern void rdabt_goep_push_res(goep_push_res_struct *msg_p);
extern void rdabt_goep_abort_res(goep_abort_res_struct *msg_p);
extern void rdabt_goep_pull_res(goep_pull_res_struct *msg_p);
extern void rdabt_goep_set_floder_res(goep_set_folder_res_struct *msg_p);
extern void rdabt_notify_evm_ind(void);
extern void rdabt_sdpdb_deregister_req(bt_sdpdb_deregister_req_struct *msg_p);
extern void rdabt_goep_deregister_req(goep_deregister_server_req_struct *msg_p);
extern void rdabt_goep_link_disallow(void);
extern void rdabt_opp_msg_hdler(ilm_struct *ext_message);


#endif /* _RDA_OPP_H */
