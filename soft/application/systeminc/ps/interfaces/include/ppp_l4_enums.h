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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   ppp_l4_enums.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/********************************************************************************
*  Copyright Statement:
*  --------------------
*
*  This product has been developed using a protocol stack
*  developed by Sasken Communication Technologies Limited.
*
********************************************************************************/


#ifndef _PPP_L4_ENUMS_H
#define _PPP_L4_ENUMS_H

typedef enum
{
    NULL_L2P = 1,
    PPP,
    MANUFACTURER_PROTOCOL    
} mmi_ltwoh_protocol_enum;

typedef enum
{
    NORMAL_LINK_TERMINATION,
    ABNORMAL_LINK_TERMINATION
} mmi_link_free_enum;

typedef enum
{
   PPPE_MOBILE_DEACT,
   PPPE_NW_DEACT
} pppcsd_deact_cause_enum;

enum
{
   PPPE_RESULT_SUCCESS,
   PPPE_RESULT_FAILED
};

typedef enum {
   FSM_INITIAL,
   FSM_STARTING,
   FSM_CLOSED,
   FSM_STOPPED,
   FSM_CLOSING,
   FSM_STOPPING,
   FSM_REQSENT,
   FSM_ACKRCVD,
   FSM_ACKSENT,
   FSM_OPENED,
   FSM_STATE_SIZE
} ppp_fsm_state_enum;

typedef enum {
    VENDOREXT = 0,
    CONF_REQ = 1,
    CONF_ACK = 2,
    CONF_NAK = 3,
    CONF_REJ = 4,
   TERM_REQ        =5,
   TERM_ACK        =6,
   CODE_REJ        =7,
    PROTO_REJ = 8,
   ECHO_REQ        =9,
    ECHO_RPLY = 10,
   DISCARD_REQ     =11,
   IDENTIFICATION  =12,
    TIMEREMAIN = 13,
    RESET_REQ = 14,
    RESET_ACK = 15,
    NULL_CODE = 0xFF
} ppp_fsm_code_enum;

/*
 * PPP Phase
 */
typedef enum
{
   PPP_DEAD_ST,
   PPP_ESTABLISH_ST,
   PPP_AUTHENTICATE_ST,
   PPP_NETWORK_ST,
   PPP_TERMINATE_ST
} ppp_state_enum;

/*
 * PPP FSM index  
 */
typedef enum
{
   LCP,
   AUTH,	
   IPCP,
   PPP_FSM_SIZE
} ppp_fsm_idx_enum;

#endif


