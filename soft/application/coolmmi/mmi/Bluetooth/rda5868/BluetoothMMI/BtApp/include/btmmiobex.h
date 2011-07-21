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
 * BTMMIObex.h
 *
 * Project:
 * --------
 *   MMI
 *
 * Description:
 * ------------
 *   
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

#ifndef __BT_MMI_OBEX_H__
#define __BT_MMI_OBEX_H__
#ifndef __MMI_FEATURES__
#error "MMI_features.h should be included"
#endif 

#include "oppmmigprots.h"
#include "btmmiopp.h"
#include "btmmiftpgprots.h"

#if defined (__MMI_FTS_SUPPORT__) || defined(__MMI_FTC_SUPPORT__) || defined(__MMI_OPP_SUPPORT__)


/*****************************************************************
    MACROS
*****************************************************************/
#ifdef WIN32
#define MMI_OBEX_DEBUG(x) x
#else  
#define MMI_OBEX_DEBUG(x) x
#endif /*WIN32*/

#define MMI_OBEX_ASSERT ASSERT


#if defined(__MMI_FTC_SUPPORT__) && defined(__MMI_OPP_SUPPORT__)
#define MMI_OBEX_PROFILE_SHARE_BUF_SIZE ( ((FTPC_MEM_BUFF_SIZE) > (OPPC_MAX_OBEX_PACKET_LENGTH))     \
                                                               ? (FTPC_MEM_BUFF_SIZE) : (OPPC_MAX_OBEX_PACKET_LENGTH) )
#elif defined(__MMI_FTC_SUPPORT__)
#define MMI_OBEX_PROFILE_SHARE_BUF_SIZE    (FTPC_MEM_BUFF_SIZE)
#elif defined(__MMI_OPP_SUPPORT__)
#define MMI_OBEX_PROFILE_SHARE_BUF_SIZE    (OPPC_MAX_OBEX_PACKET_LENGTH)
#endif


/******************************************************************************
    MMI FTP STRUCTURES
******************************************************************************/
typedef struct
{
    kal_uint8 ref_count;
    kal_uint16 msg_len;
    kal_uint8 uuid[GOEP_MAX_UUID_SIZE];
    kal_uint8 uuid_len;
} mmi_obex_msg_struct;

/******************************************************************************
    MMI FTP FUNCTIONS
******************************************************************************/
#if defined(__MMI_FTC_SUPPORT__) || defined(__MMI_OPP_SUPPORT__)
U8 *mmi_bt_obex_get_profile_shared_buffer(void);
#endif

#endif /* (__MMI_FTS_SUPPORT__) || (__MMI_FTC_SUPPORT__) || (__MMI_OPP_SUPPORT__) */
#endif /* __BT_MMI_OBEX_H__ */ 

