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
 * ProfilesDefs.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends forSettings application definitions
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

/**************************************************************

	FILENAME	: ProfilesDefs.h

  	PURPOSE		: Settings application definitions

 

	AUTHOR		: Ashima Bajaj

	DATE		: 

**************************************************************/

#ifndef _PIXCOM_PROFILESDEFS_H
#define _PIXCOM_PROFILESDEFS_H
#define VIB_THEN_RING_DURATION 3000

 
#define MAX_PROFILE_NUM 7

typedef enum {
  TONE_RINGTONE,
  TONE_POWEROFFTONE,
  TONE_POWERONTONE,
  TONE_CONVEROPENTONE,
  TONE_COVERCLOSETONE,
  TONE_MESSAGETONE,
  TONE_KEYPADTONE,
  TONE_ALARMTONE,
  
  TONE_MAX_TONE_TYPE
} PROFILE_TONE_TYPE_ENUM;
//CSD end

#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__

typedef enum {

PRF_MENU_EXTMLD_DONE,
PRF_MENU_EXTMLD_DEL,
PRF_MENU_EXTMLD_ADD,

PRF_MENU_EXTMLD_MAX

} PRF_SET_EXTMLD_MENU_ITEM;

typedef enum {

PRF_MENU_EXTMLD_DRV_PHONE,
PRF_MENU_EXTMLD_DRV_CARD,
/*+ zhouqin modify for dual t-flash card 20110617*/
#if defined(DUAL_TFLASH_SUPPORT)
PRF_MENU_EXTMLD_DRV_SEC_CARD,
#endif
/*- zhouqin modify for dual t-flash card 20110617*/
PRF_MENU_EXTMLD_DRV_MAX

} PRF_SET_EXTMLD_DRV_MENU_ITEM;

#endif /* __MMI_PROFILE_EXTMELODY_SUPPORT__ */

typedef enum {
   PRFSET_MELODY_TYPE_UNKNOW,
   PRFSET_MELODY_TYPE_RING,
   PRFSET_MELODY_TYPE_CARD2_RING,
   PRFSET_MELODY_TYPE_ALARM,
   PRFSET_MELODY_TYPE_PHB_RING,
      
   PRFSET_MELODY_TYPE_MAX
} PRFSET_MELODY_CONF_TYPE;

typedef enum
{
    MMI_PROFILES_BT_HFP_ACL,
    MMI_PROFILES_BT_HFP_SCO
} MMI_PROFILES_BT_CONNECT_TYPE_ENUM;

typedef enum
{
    MMI_PROFILES_BT_IND,
    MMI_PROFILES_BT_RSP,
    MMI_PROFILES_BT_REQ
} MMI_PROFILES_BT_IND_TYPE_ENUM;

typedef enum
{
    MMI_PROFILES_BT_OK,
    MMI_PROFILES_BT_FAIL,
    MMI_PROFILES_BT_TIMEOUT,
    MMI_PROFILES_BT_BUSY
} MMI_PROFILES_BT_RES_ENUM;

#endif











