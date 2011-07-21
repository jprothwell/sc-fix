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
 *    t9def.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   string id, image id, and screen id definition
 *
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

#ifndef __T9DEF_H__
#define __T9DEF_H__

#include "mmi_data_types.h"

//screen ids
typedef enum {
    SCR_INPUT_METHOD = INPUT_METHOD_BASE+1
} SCREENID_LIST_T9_INPUT_METHOD;

//string ids
typedef enum {
    STR_T9_EXP_UP_ABC = INPUT_METHOD_BASE+1,
 STR_T9_EXP_LO_abc,
    STR_T9_AMB_UP_ABC,
    STR_T9_AMB_LO_abc,
    STR_T9_BOPOMOFO,
    STR_T9_TR_STROKE,
    STR_T9_PINYIN,
    STR_T9_SM_STROKE,

    //STR_T9_CLEAR, ==>replaced by Global String
    //STR_T9_BACK,==>replaced by Global String
    //STR_T9_SELECT,==>replaced by Global String
    STR_T9_123,
    STR_INPUT_METHOD_THAI,
     
    STR_INPUT_METHOD_ARABIC,
    STR_INPUT_METHOD_french,
    STR_INPUT_METHOD_FRENCH,
    STR_INPUT_METHOD_RUSSIAN,
    STR_INPUT_METHOD_russian,
    STR_INPUT_METHOD_ITALIAN,
    STR_INPUT_METHOD_italian,
    //CSD end
    STR_INPUT_METHOD_REACH_MAX_LEN
} STRINGID_LIST_T9_INPUT_METHOD;

//image ids
typedef enum {
    IMG_T9 = INPUT_METHOD_BASE+1
} IMAGEID_LIST_T9_INPUT_METHOD;

#endif /*__T9DEF_H__*/



















