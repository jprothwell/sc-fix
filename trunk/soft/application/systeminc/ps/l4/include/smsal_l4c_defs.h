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
 *	smsal_l4c_defs.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file contains the defined constants which related to 
 *   interface of SMSAL and L4C.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/********************************************************************************
*  Copyright Statement:
*  --------------------
*
*  This product has been developed using a protocol stack
*  developed by Sasken Communication Technologies Limited.
*
********************************************************************************/

#ifndef _SMSAL_L4C_DEFS_H
#define _SMSAL_L4C_DEFS_H

#include "mmi_features.h"	//wangwb @2007.06.12
#define SMSAL_PAGE_SIZE             (40)        /* 40 messages/page */

#ifdef __MMI_MESSAGES_CONCAT_SMS__
#define SMSAL_MAX_MSG_LEN           (700) 
#else
#define SMSAL_MAX_MSG_LEN           (160) 
#endif                                                               //wangwb @2007.06.12
#define SMSAL_MAX_CMD_LEN           (156)        
#define SMSAL_ONE_MSG_LEN           (160)
#define SMSAL_ADDR_LEN              (11)
#define SMSAL_INVALID_INDEX         (0xffff)
#define SMSAL_MAX_TPDU_SIZE         (175)


/* 
 * !! Attention !!
 *
 * the following constants are related to CB : 
 * the CB in RMC/MMI MUST be consistent with this defined values !! 
 *
 */
#define SMSAL_CB_MAX_ENTRY             (15)    

#define SMSAL_PS_CBMI_MAX_ENTRY        (60)
#define SMSAL_CBMI_SIM_ENTRY           (20)
#define SMSAL_CBMI_ME_ENTRY            (20)
#define SMSAL_DEFAULT_CBMI_ENTRY       (10)

#define SMSAL_MMI_CBMI_MAX_ENTRY       (SMSAL_CBMI_SIM_ENTRY + SMSAL_CBMI_ME_ENTRY)
#define SMSAL_MMI_DCS_MAX_ENTRY        (SMSAL_CB_MAX_ENTRY)

#define SMSAL_CB_UNUSED_CBMI        (0xffff) /* unused CBMIR 0xffffffff */
#define SMSAL_CB_UNUSED_DCS         (0x0080)

#define SMSAL_CB_HOMEZONE_MI        (0x00dd) /* HomeZone Message ID 221 */
#define SMSAL_CB_HOMEZONE_SN        (0x0010) /* HomeZone Series Number */
#define SMSAL_CB_HOMEZONE_DCS       (0x0000) /* HomeZone DCS */

#endif /* _SMSAL_L4C_DEFS_H */


