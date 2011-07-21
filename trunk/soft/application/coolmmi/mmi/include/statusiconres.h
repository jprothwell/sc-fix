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
 *  StatusIconRes.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
 
 /**************************************************************
	FILENAME	: CustDataRes.h

   	PURPOSE		: Image id definitions for status icons

 

 

	DATE		: Dec-16-2002.

*************************************************************/

#ifndef STATUSICONSRES_H
#define STATUSICONSRES_H

 
enum STATUS_ICON_ID_LIST
{
	IMG_SI_SIGNAL_STRENGTH = 400,
	IMG_SI_BATTERY_STRENGTH,
	IMG_SI_KEYPAD_LOCK,
	IMG_SI_VIBRATE,
	IMG_SI_SILENT,
	IMG_SI_RING,
	IMG_SI_VIBRATE_AND_RING,
	IMG_SI_VIBRATE_THEN_RING,
	IMG_SI_ALARM_ACTIVATED,
	IMG_SI_ROAMING_INDICATOR,
	IMG_SI_MISSED_CALL_INDICATOR,
	IMG_SI_LINE_L1,
	IMG_SI_LINE_L2,
	IMG_SI_GPRS_INDICATOR,
	IMG_SI_IR_INDICATOR,
	IMG_SI_UNREAD_VOICE_L1,
	IMG_SI_UNREAD_VOICE_L2,
	IMG_SI_UNREAD_VOICE_L1L2,
	IMG_SI_UNREAD_FAX_L1,
	IMG_SI_UNREAD_FAX_L2,
	IMG_SI_UNREAD_FAX_L1L2,
	IMG_SI_UNREAD_EMAIL_L1,
	IMG_SI_UNREAD_EMAIL_L2,
	IMG_SI_UNREAD_EMAIL_L1L2,
	IMG_SI_SMS_INDICATOR,
	IMG_SI_CALL_DIVERT_L1,
	IMG_SI_CALL_DIVERT_L2,
	IMG_SI_CALL_DIVERT_L1L2,
	IMG_SLSI_SIGNAL_STRENGTH,
	IMG_SLSI_BATTERY_STRENGTH,
	IMG_SLSI_SMS_INDICATOR,
	IMG_SLSI_RING,
	IMG_SLSI_VIBRATE,
	IMG_SLSI_VIBRATE_AND_RING,
	IMG_SLSI_VIBRATE_THEN_RING,
	IMG_SI_WMAL,
	IMG_SLSI_SILENT,
	IMG_SI_VOICE_RECORD,
	IMG_SI_WAP,
	IMG_SI_MUTE,
	IMG_SI_EARPHONE_INDICATOR,
	IMG_SI_GPRS_ATT_NO_PDP_INDICATOR,
	IMG_SI_MMS_UNREAD_INDICATOR,
	IMG_SI_MMS_SLAVE_UNREAD_INDICATOR,
	IMG_SI_MMS_BUSY_INDICATOR,
	IMG_SLSI_ALARM,
	IMG_SLSI_MISSED_CALL,
	IMG_BLINKINGICON_CHAT_ROOM1,
	IMG_BLINKINGICON_CHAT_ROOM2,
	IMG_SI_STOPWATCH,
	IMG_SI_BT,//449
	IMG_SI_BT_CON,
	IMG_SI_BT_ND,
	IMG_SI_NETWORK_CIPHER_GSM,
	IMG_SI_NETWORK_CIPHER_GPRS,
	IMG_SI_SIGNAL_STRENGTH_L1,
	IMG_SI_SIGNAL_STRENGTH_L2,
	IMG_SI_POC,
	IMG_SI_JAVA_PAUSE,
	IMG_SI_IMPS_MSG,
	IMG_SI_IMPS_LOGIN,
	IMG_SI_KEYPAD_UNLOCK,
#ifdef __MMI_BG_SOUND_EFFECT__
	IMG_SI_BGSND,
#endif
#ifdef __MMI_HOMEZONE_STATUS_ICON__
	IMG_SI_HZONE,
	IMG_SI_CZONE,
#endif
#ifdef __CTM_SUPPORT__
	IMG_SI_TTY_INDICATOR,
#endif
#if defined(__MMI_UI_TECHNO_STATUS_ICON__)
	IMG_SI_SIGNAL_STRENGTH_IDLE_L1,
	IMG_SI_SIGNAL_STRENGTH_IDLE_L2,
	IMG_SI_BATTERY_STRENGTH_IDLE,
	IMG_SI_SIGNAL_STRENGTH_IDLE,
#endif
  IMG_SLSI_REJECTED_CALL,
  IMG_SLSI_USB_CONNECTED,
  IMG_SI_SIM_SWITCH,
  IMG_SI_SIM_SWITCH2
};
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if defined(__MMI_UI_TECHNO_STATUS_ICON__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
/* under construction !*/
#endif
 

#endif


