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
 * SettingProf.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for profile API
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/***************** Settings Application implementation *********************/

/**************************************************************

	FILENAME	: Profiles.c

  	PURPOSE		: API's to all applications.

 

	AUTHOR		: Ashima Bajaj, Deepali

	DATE		: 5/5/2003

**************************************************************/

#include "mmi_trace.h"
#include "mmi_data_types.h"
#include "commonscreens.h"
#include "settingprofile.h"
#include "custdatares.h"
#include "settingdefs.h"
#include "profilegprots.h"
#include "eventsgprot.h"
#include "profilesenum.h"
#include "profilesgexdcl.h"
#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"
#include "debuginitdef.h"
#include "gpioinc.h"
#include "audioinc.h"
#include "globaldefs.h"
#include "idleappdef.h"
#include "profilesgexdcl.h"
#include "callmanagementgprot.h"
#include "keybrd.h"
#include "profiles_prot.h"
#include "profilesdefs.h"  
#include "athandlerprot.h"
 
#include "downloaddefs.h"
 
/* Jinzh add for mp3 : 20070731 */
#include "mdi_audio.h"
#include "audioplayerdef.h"
#include "audioplayertype.h"
#include "audioplayerprot.h"
#ifdef __MMI_BT_PROFILE__
#include "btmmicm.h"
#include "btmmicmgprots.h"
#include "btmmiscr.h"
#include "btmmiscrgprots.h"
#ifdef __MMI_A2DP_SUPPORT__
#include "btmmia2dp.h"
#endif
#include "med_struct.h"
#endif /* __MMI_BT_PROFILE__ */ 

/* Jinzh end */
#ifdef __MMI_USB_SUPPORT__
#define MMI_PROF_GENERAL_KEYPAD_TIMER_DUR 15 /* sec */
   #ifdef MMI_ON_HARDWARE_P
   extern kal_bool INT_USBBoot(void);
   #endif
#endif

#ifdef __MMI_AUDIO_PLAYER__
extern BOOL mmi_audply_save_volume( U8 volume );
#endif // __MMI_AUDIO_PLAYER__


#if defined(__MMI_ALM_AUDIO_OPTIONS__) || defined(__MMI_ALM_CUSTOMIZE_TONE__)
#include "alarmdef.h"
#include "alarmprot.h"

extern alm_context_struct g_alm_cntx;
extern alm_framework_context_struct g_alm_frm_cntx;
#endif
extern audply_struct g_audply;			//Added by jinzh:20070731
extern void change_font_Theme(void);
extern U8 GetHandsetInPhone();
#ifdef __MMI_BT_PROFILE__
extern void mmi_bt_a2dp_deactivate_req(void);
U8 g_mmi_profiles_bt_reset_pending = 0;
BOOL g_mmi_profiles_bt_in_call = FALSE;
static U8 g_mmi_profiles_bt_disalbe_sco = 0; /* Disable SCO connection */
static MMI_BOOL g_mmi_profiles_bt_inquerying = MMI_FALSE; /* No SCO connection when inquerying */

#ifdef __MMI_HFP_SUPPORT__
BOOL g_mmi_profiles_bt_always_use_in_call = FALSE;
static void (*mmi_profiles_bt_connect_sco_app_callback) (void) = NULL;
BOOL g_mmi_profiles_bt_app_in_use = FALSE;
#endif /* __MMI_HFP_SUPPORT__ */ 
#endif /* __MMI_BT_PROFILE__ */ 

#ifdef __MMI_FM_RADIO__
extern BOOL mmi_fmrdo_is_power_on(void);
#endif
#ifdef __MMI_SOUND_RECORDER__
extern MMI_BOOL mmi_sndrec_record_is_on(void);
#endif
#if 0 // defined(__MMI_BT_AUDIO_VIA_SCO__) || defined(__MMI_BT_FM_VIA_SCO__)
#include "mdi_video.h"

#define BT_SCO_CONNECT_TIMEOUT_DURATION 20000 /* Timeout to disconnect duration */
static MMI_BOOL g_mmi_profiles_bt_sco_timeout = MMI_TRUE; /* Timeout mechanism */
static void (*g_mmi_profiles_bt_inquery_callback) (void) = NULL;
#endif /* #if defined(__MMI_BT_AUDIO_VIA_SCO__) || defined(__MMI_BT_FM_VIA_SCO__) */

#if defined(__MMI_BT_COOLSAND_SUPPORT__) && defined(__MMI_A2DP_SUPPORT__)
extern void mmi_bt_a2dp_init(void);
extern void mmi_audply_bt_delete_device_callback(mmi_bt_dev_addr* bd_addr);
extern void mmi_audply_bt_power_off_callback(BOOL success);
extern void mmi_audply_bt_power_on_callback(BOOL success);
extern void av_bt_headset_selected_callback(mmi_bt_dev_struct* dev);
extern void av_bt_inquiry_start_callback(void);
extern void av_bt_inquiry_stop_callback(void);

#endif /* __MMI_BT_COOLSAND_SUPPORT__ */ 

#if defined(__MMI_BT_AUDIO_VIA_SCO__) || defined(__MMI_BT_FM_VIA_SCO__)
#include "mdi_video.h"
//#include "mdi_bitstream.h"

#define BT_SCO_CONNECT_TIMEOUT_DURATION 4000 /* Timeout to disconnect duration */
#define BT_SCO_RECONNECT_TIMER_DURATION 3000 /* Timeout to disconnect duration */

static MMI_BOOL g_mmi_profiles_bt_sco_timeout = MMI_TRUE; /* Timeout mechanism */
static void (*g_mmi_profiles_bt_inquery_callback) (void) = NULL;
#endif /* #if defined(__MMI_BT_AUDIO_VIA_SCO__) || defined(__MMI_BT_FM_VIA_SCO__) */

/*****************************************************************************

	FUNCTION NAME		: void initializeProfiles(void)

  	PURPOSE				: Set default values in profiles structure.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************************/
 void initializeProfiles(void)
{
	GetDynamicToneIds();  //dyj del 20060514 //add by panxu 2006-12-20
	/*Initialising General Profile */
	gprofiles[0].toneSetup.ringTone = gstartMidiId;
	gprofiles[0].toneSetup.alarmTone = (gstartMidiId+ 1);
	gprofiles[0].toneSetup.powerOnTone = (gstartGeneralToneId );
	gprofiles[0].toneSetup.powerOffTone = (gstartGeneralToneId + 1);
//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
	gprofiles[0].toneSetup.coverOpenTone = (gstartGeneralToneId + 2);
	gprofiles[0].toneSetup.coverCloseTone = (gstartGeneralToneId + 3);
#endif
//CSD end
	gprofiles[0].toneSetup.messageTone = gstartMiscToneId ;
	gprofiles[0].toneSetup.keypadTone = KEYPAD_TONE;
	gprofiles[0].mtCallAlertTypeEnum = MMI_RING;
	gprofiles[0].ringTypeEnum = REPEAT;
	gprofiles[0].extraTone.connectTone = 0;
	gprofiles[0].extraTone.errorTone = 1;
	gprofiles[0].extraTone.campOnTone = 1;
	gprofiles[0].extraTone.warningTone = 1;
//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
	gprofiles[0].answeringMode.coverAnswer = 1;	
#endif
//CSD end
	gprofiles[0].answeringMode.anyKey = 0;
	gprofiles[0].answeringMode.automatic = 1;
	gprofiles[0].light.timer.setsec = 15;
	gprofiles[0].light.status = COLOR_PATTERN1;
	gprofiles[0].fontSizeEnum = THEME_DEFAULT;
	gprofiles[0].keypadVolumeLevel = LEVEL2;
	gprofiles[0].ringVolumeLevel = LEVEL4;
	gprofiles[0].loudSpeakerVolumeLevel = LEVEL4;
	gprofiles[0].intelligentCallAlert = MMI_INTELLIGENT_CALL_ALERT_OFF;

	gprofiles[1].toneSetup.ringTone = gstartMidiId;
	gprofiles[1].toneSetup.alarmTone = (gstartMidiId + 1);
	gprofiles[1].toneSetup.powerOnTone = (POWERON_SILENT);
	gprofiles[1].toneSetup.powerOffTone = (POWEROFF_SILENT);
//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
	gprofiles[1].toneSetup.coverOpenTone = (COVER_OPEN_SILENT);
	gprofiles[1].toneSetup.coverCloseTone = (COVER_CLOSE_SILENT);
#endif
//CSD end
	gprofiles[1].toneSetup.messageTone = (MESSAGE_SILENT);
	gprofiles[1].toneSetup.keypadTone = KEYPAD_TONE;//Modified by jinzh:20070621 原来的值KEYPAD_SILENT;
	gprofiles[1].mtCallAlertTypeEnum = MMI_VIBRATION_ONLY;
	gprofiles[1].ringTypeEnum = SINGLE;
	gprofiles[1].extraTone.connectTone = 0;
	gprofiles[1].extraTone.errorTone = 0;
	gprofiles[1].extraTone.campOnTone = 0;
	gprofiles[1].extraTone.warningTone = 0;
//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
	gprofiles[1].answeringMode.coverAnswer = 1;	
#endif
//CSD end
	gprofiles[1].answeringMode.anyKey = 0;
	gprofiles[1].answeringMode.automatic = 1;
	gprofiles[1].light.timer.setsec = 15;
	gprofiles[1].light.status = COLOR_PATTERN1;
	gprofiles[1].fontSizeEnum = THEME_DEFAULT;
	gprofiles[1].keypadVolumeLevel = LEVEL1;
	gprofiles[1].ringVolumeLevel = LEVEL4;
	gprofiles[1].loudSpeakerVolumeLevel = LEVEL4;
	gprofiles[1].intelligentCallAlert = MMI_INTELLIGENT_CALL_ALERT_OFF;

	/*Initialising Outdoor Profile */
	gprofiles[2].toneSetup.ringTone = gstartMidiId;
	gprofiles[2].toneSetup.alarmTone = (gstartMidiId + 1);
	gprofiles[2].toneSetup.powerOnTone = (gstartGeneralToneId + 0);
	gprofiles[2].toneSetup.powerOffTone = (gstartGeneralToneId + 1);
//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
	gprofiles[2].toneSetup.coverOpenTone = (gstartGeneralToneId + 2);
	gprofiles[2].toneSetup.coverCloseTone = (gstartGeneralToneId + 3);
#endif
//CSD end
	gprofiles[2].toneSetup.messageTone = gstartMiscToneId ;
	gprofiles[2].toneSetup.keypadTone = KEYPAD_TONE;
	gprofiles[2].mtCallAlertTypeEnum = MMI_VIBRATION_AND_RING;
	gprofiles[2].ringTypeEnum = REPEAT;
	gprofiles[2].extraTone.connectTone = 0;
	gprofiles[2].extraTone.errorTone = 1;
	gprofiles[2].extraTone.campOnTone = 1;
	gprofiles[2].extraTone.warningTone = 1;
//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
	gprofiles[2].answeringMode.coverAnswer = 1;	
#endif
//CSD end
	gprofiles[2].answeringMode.anyKey = 0;
	gprofiles[2].answeringMode.automatic = 1;
	gprofiles[2].light.timer.setsec = 15;
	gprofiles[2].light.status = COLOR_PATTERN1;
	gprofiles[2].fontSizeEnum = THEME_DEFAULT;
	gprofiles[2].keypadVolumeLevel = LEVEL7;
	gprofiles[2].ringVolumeLevel = LEVEL7;
	gprofiles[2].loudSpeakerVolumeLevel = LEVEL4;	
	gprofiles[2].intelligentCallAlert = MMI_INTELLIGENT_CALL_ALERT_OFF;

	/*Initialising Indoor Profile */
	gprofiles[3].toneSetup.ringTone = gstartMidiId;
	gprofiles[3].toneSetup.alarmTone = (gstartMidiId + 1);
	gprofiles[3].toneSetup.powerOnTone = (gstartGeneralToneId + 0);
	gprofiles[3].toneSetup.powerOffTone = (gstartGeneralToneId + 1);
//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
	gprofiles[3].toneSetup.coverOpenTone = (gstartGeneralToneId + 2);
	gprofiles[3].toneSetup.coverCloseTone = (gstartGeneralToneId + 3);
#endif
//CSD end
	gprofiles[3].toneSetup.messageTone = gstartMiscToneId ;
	gprofiles[3].toneSetup.keypadTone = KEYPAD_TONE;
	gprofiles[3].mtCallAlertTypeEnum = MMI_RING;
	gprofiles[3].ringTypeEnum = ASCENDING;
	gprofiles[3].extraTone.connectTone = 0;
	gprofiles[3].extraTone.errorTone = 1;
	gprofiles[3].extraTone.campOnTone = 1;
	gprofiles[3].extraTone.warningTone = 1;
//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
	gprofiles[3].answeringMode.coverAnswer = 1;	
#endif
//CSD end
	gprofiles[3].answeringMode.anyKey = 0;
	gprofiles[3].answeringMode.automatic = 1;
	gprofiles[3].light.timer.setsec = 15;
	gprofiles[3].light.status = COLOR_PATTERN1;
	gprofiles[3].fontSizeEnum = THEME_DEFAULT;
	gprofiles[3].keypadVolumeLevel = LEVEL1;
	gprofiles[3].ringVolumeLevel = LEVEL3;
	gprofiles[3].loudSpeakerVolumeLevel = LEVEL4;
	gprofiles[3].intelligentCallAlert = MMI_INTELLIGENT_CALL_ALERT_OFF;
	
	/*Initialising Headset Profile */
	gprofiles[4].toneSetup.ringTone = gstartMidiId;
	gprofiles[4].toneSetup.alarmTone = (gstartMidiId + 1);
	gprofiles[4].toneSetup.powerOnTone = (gstartGeneralToneId + 0);
	gprofiles[4].toneSetup.powerOffTone = (gstartGeneralToneId + 1);
//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
	gprofiles[4].toneSetup.coverOpenTone = (gstartGeneralToneId + 2);
	gprofiles[4].toneSetup.coverCloseTone = (gstartGeneralToneId + 3);
#endif
//CSD end
	gprofiles[4].toneSetup.messageTone = gstartMiscToneId ;
	gprofiles[4].toneSetup.keypadTone = KEYPAD_TONE;
	gprofiles[4].mtCallAlertTypeEnum = MMI_RING;
	gprofiles[4].ringTypeEnum = REPEAT;
	gprofiles[4].extraTone.connectTone = 0;
	gprofiles[4].extraTone.errorTone = 1;
	gprofiles[4].extraTone.campOnTone = 1;
	gprofiles[4].extraTone.warningTone = 1;
//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
	gprofiles[4].answeringMode.coverAnswer = 1;	
#endif
//CSD end

	gprofiles[4].answeringMode.anyKey = 0;
//CSD: Brian added for KLM
	/*Jinzh Modify Start For 6502 Ver: TBM780  on 2007-8-29 9:55 */
	gprofiles[4].answeringMode.automatic = 0;
	/*Jinzh Modify End  For 6502 Ver: TBM780  on 2007-8-29 9:55 */
//CSD end
	gprofiles[4].light.timer.setsec = 15;
	gprofiles[4].light.status = COLOR_PATTERN1;
	gprofiles[4].fontSizeEnum = THEME_DEFAULT;
	gprofiles[4].keypadVolumeLevel = LEVEL2;
	gprofiles[4].ringVolumeLevel = LEVEL4;
	gprofiles[4].loudSpeakerVolumeLevel = LEVEL4;
	gprofiles[4].intelligentCallAlert = MMI_INTELLIGENT_CALL_ALERT_OFF;
	
	/*Initialising Silent Profile */
	gprofiles[5].toneSetup.ringTone = gstartMidiId;
	gprofiles[5].toneSetup.alarmTone = (gstartMidiId + 1);
	gprofiles[5].toneSetup.powerOnTone = (POWERON_SILENT);
	gprofiles[5].toneSetup.powerOffTone = (POWEROFF_SILENT);
//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
	gprofiles[5].toneSetup.coverOpenTone = (COVER_OPEN_SILENT);
	gprofiles[5].toneSetup.coverCloseTone = (COVER_CLOSE_SILENT);
#endif
//CSD end
	gprofiles[5].toneSetup.messageTone = (MESSAGE_SILENT);
	gprofiles[5].toneSetup.keypadTone = KEYPAD_SILENT;/*Modified by liuxl:20090616 原来的值KEYPAD_TONE--导致静音模式下开机按键有音*/
	gprofiles[5].mtCallAlertTypeEnum = MMI_RING;
	gprofiles[5].ringTypeEnum = REPEAT;
	gprofiles[5].extraTone.connectTone = 0;
	gprofiles[5].extraTone.errorTone = 0;//JiaShuo for bug#15858 on 20091215
	gprofiles[5].extraTone.campOnTone = 0;    //Modified by Jinzh:20070617  以前值为1，导致静音模式下开机有铃声
	gprofiles[5].extraTone.warningTone = 0; //chenhe,070817,for bug#6251
//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
	gprofiles[5].answeringMode.coverAnswer = 1;	
#endif
//CSD end
	gprofiles[5].answeringMode.anyKey = 0;
	gprofiles[5].answeringMode.automatic = 1;
	gprofiles[5].light.timer.setsec = 15;
	gprofiles[5].light.status = COLOR_PATTERN1;
	gprofiles[5].fontSizeEnum = THEME_DEFAULT;
	gprofiles[5].keypadVolumeLevel = LEVEL5;
	gprofiles[5].ringVolumeLevel = LEVEL3;
	gprofiles[5].loudSpeakerVolumeLevel = LEVEL4;
	gprofiles[5].intelligentCallAlert = MMI_INTELLIGENT_CALL_ALERT_OFF;
	
	/*Initialising LoudSpeaker Profile */
	gprofiles[6].toneSetup.ringTone = gstartMidiId;
	gprofiles[6].toneSetup.alarmTone = (gstartMidiId + 1);
	gprofiles[6].toneSetup.powerOnTone = (gstartGeneralToneId + 0);
	gprofiles[6].toneSetup.powerOffTone = (gstartGeneralToneId + 1);
//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
	gprofiles[6].toneSetup.coverOpenTone = (gstartGeneralToneId + 2);
	gprofiles[6].toneSetup.coverCloseTone = (gstartGeneralToneId + 3);
#endif
//CSD end
	gprofiles[6].toneSetup.messageTone = gstartMiscToneId ;
	gprofiles[6].toneSetup.keypadTone = KEYPAD_TONE;
	gprofiles[6].mtCallAlertTypeEnum = MMI_RING;
	gprofiles[6].ringTypeEnum = REPEAT;
	gprofiles[6].extraTone.connectTone = 0;
	gprofiles[6].extraTone.errorTone = 1;
	gprofiles[6].extraTone.campOnTone = 1;
	gprofiles[6].extraTone.warningTone = 1;
//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
	gprofiles[6].answeringMode.coverAnswer = 1;	
#endif
//CSD end
	gprofiles[6].answeringMode.anyKey = 0;
	gprofiles[6].answeringMode.automatic = 1;
	gprofiles[6].light.timer.setsec = 15;
	gprofiles[6].light.status = COLOR_PATTERN1;
	gprofiles[6].fontSizeEnum = THEME_DEFAULT;
	gprofiles[6].keypadVolumeLevel = LEVEL5;
	gprofiles[6].ringVolumeLevel = LEVEL3;
	gprofiles[6].loudSpeakerVolumeLevel = LEVEL4;	
	gprofiles[6].intelligentCallAlert = MMI_INTELLIGENT_CALL_ALERT_OFF;
/*add the initialization of Dual sim profiles item by liuxl on 20090821 */	
#ifdef __MMI_DUAL_PROFILE_SUPPORT__
	gprofiles[0].toneSetup.card2_ringTone = gstartMidiId;
	gprofiles[1].toneSetup.card2_ringTone = gstartMidiId;
	gprofiles[2].toneSetup.card2_ringTone = gstartMidiId;
	gprofiles[3].toneSetup.card2_ringTone = gstartMidiId;
	gprofiles[4].toneSetup.card2_ringTone = gstartMidiId;
	gprofiles[5].toneSetup.card2_ringTone = gstartMidiId;
	gprofiles[6].toneSetup.card2_ringTone = gstartMidiId;

	gprofiles[0].toneSetup.card2_messageTone = gstartMiscToneId ;
	gprofiles[1].toneSetup.card2_messageTone = (MESSAGE_SILENT);
	gprofiles[2].toneSetup.card2_messageTone = gstartMiscToneId ;
	gprofiles[3].toneSetup.card2_messageTone = gstartMiscToneId ;
	gprofiles[4].toneSetup.card2_messageTone = gstartMiscToneId ;
	gprofiles[5].toneSetup.card2_messageTone = (MESSAGE_SILENT);
	gprofiles[6].toneSetup.card2_messageTone = gstartMiscToneId ;
#endif

}

/*****************************************************************

	FUNCTION NAME		: void ringvolume_level(S8*)

  	PURPOSE				: Setting Ring Volume Level for a Profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
MMI_VOLUME_LEVEL_ENUM GetRingVolumeLevel(void)
{
	MMI_VOLUME_LEVEL_ENUM ringVolumeLevel;
   	
	ringVolumeLevel = gcurrentprofile.ringVolumeLevel;

	PRINT_INFORMATION(("File: Setting Prof.c <<GetRingVolumeLevel>\
		               the ring volume level is : %d \n",ringVolumeLevel));
	
	return ringVolumeLevel;
}
/*****************************************************************

	FUNCTION NAME		: void ringvolume_level(S8*)

  	PURPOSE				: Setting Ring Volume Level for a Profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
MMI_VOLUME_LEVEL_ENUM GetLoudspkrVolumeLevel(void)
{
	MMI_VOLUME_LEVEL_ENUM loudspkr;
   	
	loudspkr = gprofiles[6].loudSpeakerVolumeLevel;

	PRINT_INFORMATION(("File: Setting Prof.c <<GetLoudspkrVolumeLevel>\
		               the loud volume level is : %d \n",loudspkr));
	
	return loudspkr;
}
/*****************************************************************

	FUNCTION NAME		: void keypad_volume_level(S8*)

  	PURPOSE				: Setting Keypad Volume Level for a Profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
MMI_VOLUME_LEVEL_ENUM GetKeypadVolumeLevel(void)
{
	MMI_VOLUME_LEVEL_ENUM keypadVolumeLevel;

	keypadVolumeLevel = gcurrentprofile.keypadVolumeLevel;

	PRINT_INFORMATION(("File: Setting Prof.c <<GetKeypadVolumeLevel>\
		               the keypadVolumeLevel is : %d \n",keypadVolumeLevel));
		
	return keypadVolumeLevel;
}

/*****************************************************************

	FUNCTION NAME		: void mt_call_alert_type_enum(S8*)

  	PURPOSE				: Setting Call Alert type for a Profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
MMI_ALERT_TYPE GetMtCallAlertTypeEnum(void)
{
MMI_ALERT_TYPE mtCallAlertTypeEnum;

	mtCallAlertTypeEnum = gcurrentprofile.mtCallAlertTypeEnum;

	PRINT_INFORMATION(("File: Setting Prof.c <<GetMtCallAlertTypeEnum>\
		               the mtCallAlertTypeEnum is : %d \n",mtCallAlertTypeEnum));

	return mtCallAlertTypeEnum;
}
/*****************************************************************

	FUNCTION NAME		: void Light(S8*)

  	PURPOSE				: Setting Light for a Profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
MMI_LEDLIGHT_STATUS_ENUM GetLightStatus(void)
{
LIGHT templight;

	templight.status = 	gcurrentprofile.light.status;

	PRINT_INFORMATION(("File: Setting Prof.c <<GetLightStatus>\
		               the light color_t is : %d \n",templight.status));

	return templight.status;
}
/*****************************************************************

	FUNCTION NAME		: void Ring_Type_Enum(S8*)

  	PURPOSE				: Setting Ring_Type for a Profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
MMI_RING_TYPE_ENUM GetRingTypeEnum(void)
{
MMI_RING_TYPE_ENUM ringType;

	ringType = gcurrentprofile.ringTypeEnum;
	switch (ringType)
	{
	case SINGLE:
		return 2;
	case REPEAT:
		return 1;
	case ASCENDING:
		return 0;
	default:
		return 1;
	}


	PRINT_INFORMATION(("File: Setting Prof.c <<GetRingTypeEnum>\
		               the ringType is : %d \n",ringType));

}
#ifdef __MMI_CONNECT_NOTICE__
/*****************************************************************

	FUNCTION NAME		: void GetConnectNoticeType(S8*)

  	PURPOSE				: Setting Call Alert type for a Profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
U16 GetConnectNoticeType(void)
{
   MMI_CONNECT_NOTICE_TYPE connectNotice;

	connectNotice = gcurrentprofile.extraTone.connectTone;

	PRINT_INFORMATION(("File: Setting Prof.c <<GetConnectNoticeType>\
		               the GetConnectNoticeType is : %d \n", connectNotice));

	return (U16)connectNotice;
}
#endif
/*****************************************************************

	FUNCTION NAME		: void Answering_Mode_Enum(S8*)

  	PURPOSE				: Setting Answering mode for a Profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
MMI_ANSWERING_MODE * GetAnsweringMode(void)
{
static MMI_ANSWERING_MODE answeringMode;	


#ifdef __MMI_MAINLCD_220X176__ 

//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
	answeringMode.coverAnswer = gprofiles[0].answeringMode.coverAnswer;
#endif
//CSD end

//CSD: Brian modified for auto answer if test sim, 031020
#ifdef MMI_ON_HARDWARE_P
   if (is_test_sim() || gactivatedprofile==4)
	  /*Jinzh Add Start For 耳机模式自动应答 Ver: TBM780  on 2007-8-27 14:58 */
	  answeringMode.automatic = gprofiles[0].answeringMode.automatic;
	  /*Jinzh Add End  For 耳机模式自动应答 Ver: TBM780  on 2007-8-27 14:58 */
   else
      answeringMode.automatic = FALSE;
#else
   if (gactivatedprofile==4)
	/*Jinzh Add Start For 耳机模式自动应答 Ver: TBM780  on 2007-8-27 14:58 */
	answeringMode.automatic = gprofiles[0].answeringMode.automatic;
	/*Jinzh Add End  For 耳机模式自动应答 Ver: TBM780  on 2007-8-27 14:58 */
   else
      answeringMode.automatic = FALSE;
#endif
//CSD: Brian end
	   //answeringMode.automatic = gcurrentprofile.answeringMode.automatic;
	answeringMode.anyKey = gprofiles[0].answeringMode.anyKey;


#else /* __MMI_MAINLCD_220X176__ */
//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
	answeringMode.coverAnswer = gcurrentprofile.answeringMode.coverAnswer;
#endif
//CSD end

//CSD: Brian modified for auto answer if test sim, 031020
#ifdef MMI_ON_HARDWARE_P
   if (is_test_sim() || gactivatedprofile==4)
	  /*Jinzh Add Start For 耳机模式自动应答 Ver: TBM780  on 2007-8-27 14:58 */
	  answeringMode.automatic = gcurrentprofile.answeringMode.automatic;
	  /*Jinzh Add End  For 耳机模式自动应答 Ver: TBM780  on 2007-8-27 14:58 */
   else
      answeringMode.automatic = FALSE;
#else
   if (gactivatedprofile==4)
	/*Jinzh Add Start For 耳机模式自动应答 Ver: TBM780  on 2007-8-27 14:58 */
	answeringMode.automatic = gcurrentprofile.answeringMode.automatic;
	/*Jinzh Add End  For 耳机模式自动应答 Ver: TBM780  on 2007-8-27 14:58 */
   else
      answeringMode.automatic = FALSE;
#endif
//CSD: Brian end
	   //answeringMode.automatic = gcurrentprofile.answeringMode.automatic;
	answeringMode.anyKey = gcurrentprofile.answeringMode.anyKey;

#endif
	return &answeringMode;
}
/*****************************************************************

	FUNCTION NAME		: void loud_speaker(S8*)

  	PURPOSE				: Setting loud speaker for a Profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
pBOOL LoudSpeaker(void)
{
	pBOOL loudSpeaker;
	
	loudSpeaker = gloudSpeaker;
	
	PRINT_INFORMATION(("File: Setting Prof.c <<LoudSpeaker>\
		               the loudSpeaker is : %d \n",loudSpeaker));

	return loudSpeaker;

}
/*****************************************************************

	FUNCTION NAME		: void Font_Size_Enum(S8*)

  	PURPOSE				: Setting Font Size for a Profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
MMI_FONT_SIZE_ENUM FontSizeEnum(void)
{
	MMI_FONT_SIZE_ENUM fontSize;
	
	fontSize = gcurrentprofile.fontSizeEnum;

	PRINT_INFORMATION(("File: Setting Prof.c <<FontSizeEnum>\
		               the fontSize is : %d \n",fontSize));
	
	return fontSize;
}
/*****************************************************************

	FUNCTION NAME		: void extra_tone(S8*)

  	PURPOSE				: Setting Extra Tone for a Profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
pBOOL GetConnectToneStatus(void)
{
	EXTRA_TONE tempExtraTone;

	tempExtraTone.connectTone = gcurrentprofile.extraTone.connectTone;

	PRINT_INFORMATION(("File: Setting Prof.c <<GetConnectToneStatus>\
		               the connectTone is : %d \n",tempExtraTone.connectTone));

	return tempExtraTone.connectTone ;
}
/*****************************************************************

	FUNCTION NAME		: void get_errortone_status(void)

  	PURPOSE				: Setting Extra Tone for a Profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
pBOOL GetErrorToneStatus(void)
{
	EXTRA_TONE tempExtraTone;

	tempExtraTone.errorTone = gcurrentprofile.extraTone.errorTone;

	PRINT_INFORMATION(("File: Setting Prof.c <<GetErrorToneStatus>\
		               the errorTone is : %d \n",tempExtraTone.errorTone));
	
	return tempExtraTone.errorTone;
}
/*****************************************************************

	FUNCTION NAME		: void GetRingToneID(void)

  	PURPOSE				: Setting ringtone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
U16 GetRingToneID(void)
{
	TONE_SETUP temptoneSetup;
	
	 temptoneSetup.ringTone = gcurrentprofile.toneSetup.ringTone;

	 PRINT_INFORMATION(("File: Setting Prof.c <<GetRingToneID>\
		               the RingToneID is : %d \n",temptoneSetup.ringTone));

	return temptoneSetup.ringTone ;
}

#ifdef __MMI_DUAL_PROFILE_SUPPORT__  //Add by Renyh for 221 project   20090213
/*****************************************************************************
 * FUNCTION
 *  GetCard2RingToneID
 * DESCRIPTION
 *  Get card2 ring tone id of current profile
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
U16 GetCard2RingToneID(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    TONE_SETUP temptoneSetup;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    temptoneSetup.card2_ringTone = gcurrentprofile.toneSetup.card2_ringTone;
	
    PRINT_INFORMATION(("File: Setting Prof.c <<GetCard2RingToneID>\
		               the Card2RingToneID is : %d \n",temptoneSetup.card2_ringTone));
		               
    return temptoneSetup.card2_ringTone;
}
#endif                                                        //Add by Renyh for 221 project   20090213
/*****************************************************************

	FUNCTION NAME		: void get_powerontone(void)

  	PURPOSE				: Setting power tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
POWERON_TONE_ENUM GetPowerOnToneType(void)
{
	TONE_SETUP temptoneSetup;
	
	temptoneSetup.powerOnTone = gcurrentprofile.toneSetup.powerOnTone;

	PRINT_INFORMATION(("File: Setting Prof.c <<GetPowerOnToneType>\
		               the PowerOnTone is : %d \n",temptoneSetup.powerOnTone));
	
	return temptoneSetup.powerOnTone;
}
/*****************************************************************

	FUNCTION NAME		: void get_powerofftone(void)

  	PURPOSE				: Setting power tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
POWEROFF_TONE_ENUM GetPowerOffToneType(void)
{
	TONE_SETUP temptoneSetup;
	
	temptoneSetup.powerOffTone = gcurrentprofile.toneSetup.powerOffTone;

	PRINT_INFORMATION(("File: Setting Prof.c <<GetPowerOffToneType>\
		               the PowerOffTone is : %d \n",temptoneSetup.powerOffTone));
	
	return temptoneSetup.powerOffTone;
}

//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
/*****************************************************************

	FUNCTION NAME		: void get_coveropentone(void)

  	PURPOSE				: Setting coveropentone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
COVER_OPEN_TONE_ENUM GetCoverOpenToneType(void)
{
	TONE_SETUP temptoneSetup;
	
	temptoneSetup.coverOpenTone = gcurrentprofile.toneSetup.coverOpenTone;

	PRINT_INFORMATION(("File: Setting Prof.c <<GetCoverOpenToneType>\
		               the CoverOpenTone is : %d \n",temptoneSetup.coverOpenTone ));
	
	return temptoneSetup.coverOpenTone;
}
/*****************************************************************

	FUNCTION NAME		: void get_coverclose(void)

  	PURPOSE				: Setting power tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
COVER_CLOSE_TONE_ENUM GetCoverCloseToneType(void)
{
	TONE_SETUP temptoneSetup;
	
	temptoneSetup.coverCloseTone = gcurrentprofile.toneSetup.coverCloseTone;

	PRINT_INFORMATION(("File: Setting Prof.c <<GetCoverCloseToneType>\
		               the CoverCloseTone is : %d \n",temptoneSetup.coverCloseTone));
	
	return temptoneSetup.coverCloseTone;
}
#endif
//CSD end

/*****************************************************************

	FUNCTION NAME		: void get_keypadtone(void)

  	PURPOSE				: Setting Keypad tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
KEYPAD_TONE_ENUM GetKeypadToneType(void)
{
	TONE_SETUP temptoneSetup;
	
	temptoneSetup.keypadTone = gcurrentprofile.toneSetup.keypadTone;
	
	return temptoneSetup.keypadTone;
}
/*****************************************************************

	FUNCTION NAME		: void get_warningtone(void)

  	PURPOSE				: Setting warning tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
pBOOL GetWarningToneType(void)
{
	EXTRA_TONE temptoneSetup;
	
	temptoneSetup.warningTone = gcurrentprofile.extraTone.warningTone;

	PRINT_INFORMATION(("File: Setting Prof.c <<GetWarningToneType>\
		               the warningTone is : %d \n",temptoneSetup.warningTone));
	
	return temptoneSetup.warningTone;
}
/*****************************************************************

	FUNCTION NAME		: void get_warningtone(void)

  	PURPOSE				: Setting warning tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
MESSAGE_TONE_ENUM GetMessageToneType(void)
{
	TONE_SETUP temptoneSetup;
	
	temptoneSetup.messageTone = gcurrentprofile.toneSetup.messageTone;
	
	PRINT_INFORMATION(("File: Setting Prof.c <<GetMessageToneType>\
		               the messageTone is : %d \n",temptoneSetup.messageTone));

	return temptoneSetup.messageTone;
}

#ifdef __MMI_DUAL_PROFILE_SUPPORT__  //Add by Renyh for 221 project   20090213
MESSAGE_TONE_ENUM GetCard2MessageToneType(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    TONE_SETUP temptoneSetup;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    temptoneSetup.card2_messageTone = gcurrentprofile.toneSetup.card2_messageTone;

    return temptoneSetup.card2_messageTone;
}
#endif  //Add by Renyh for 221 project   20090213

/*****************************************************************

	FUNCTION NAME		: void get_warningtone(void)

  	PURPOSE				: Setting warning tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
ALARM_TONE_ENUM GetAlarmToneType(void)
{
	TONE_SETUP temptoneSetup;
	
	temptoneSetup.alarmTone = gcurrentprofile.toneSetup.alarmTone;
	
	PRINT_INFORMATION(("File: Setting Prof.c <<GetAlarmToneType>\
		               the alarmTone is : %d \n",temptoneSetup.alarmTone));

	return (ALARM_TONE_ENUM)(temptoneSetup.alarmTone);
}

/*****************************************************************

	FUNCTION NAME		: void get_lcdbacklighttimer(void)

  	PURPOSE				: Setting lcdbacklight timer value

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
U8 get_lcdbacklighttimer(void)
{
	LIGHT tempLight;
	
#ifdef __MMI_USB_SUPPORT__
	if( INT_USBBoot() )
	   return MMI_PROF_GENERAL_KEYPAD_TIMER_DUR;
#endif

#ifdef __MMI_MAINLCD_220X176__ 
      tempLight.timer.setsec = gprofiles[0].light.timer.setsec;
#else
	tempLight.timer.setsec = gcurrentprofile.light.timer.setsec;
#endif	
	/*Guojian Add Start For timer's duration can't be 0 Ver: GreenStone  on 2009-4-24 10:40 */
	if (tempLight.timer.setsec == 0)
		return MMI_PROF_GENERAL_KEYPAD_TIMER_DUR;
	/*Guojian Add End  For timer's duration can't be 0 Ver: GreenStone  on 2009-4-24 10:40 */
	return tempLight.timer.setsec;
}
/*****************************************************************

	FUNCTION NAME		: void Activate(void)

  	PURPOSE				: Activating a profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
void ActivateProfile(void)
{
	S16 error =0 ;
 
	ReadValue( NVRAM_PROFILES_ACTIVATED, &gactivatedprofile, DS_BYTE , &error);
	mmi_trace(g_sw_SETTING, TSTXT("###### in ActivateProfile() gactivatedprofile = %d############\n"), gactivatedprofile); 
 
	PRINT_INFORMATION (("Activated Profile : %d ", (int) gactivatedprofile));

	if(gactivatedprofile == 0xff)
	{
		gactivatedprofile =0;
		WriteValue( NVRAM_PROFILES_ACTIVATED, &gactivatedprofile, DS_BYTE , &error);
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));

		/*SetRingerVolume(GetRingVolumeLevel());  deleted by jinzh:20070731
		mdi_audio_set_volume_no_apply(MDI_VOLUME_MEDIA, GetRingVolumeLevel() );

		SetKeyPadVolume(GetKeypadVolumeLevel());
		SetLoudSpkrVolumeLevel(GetLoudspkrVolumeLevel());*/
		
	}
	else
	{
	 memcpy(&gcurrentprofile,&gprofiles[gactivatedprofile],sizeof(PROFILE));

	 	/*SetRingerVolume(GetRingVolumeLevel());   deleted by jinzh:20070731
		mdi_audio_set_volume_no_apply(MDI_VOLUME_MEDIA, GetRingVolumeLevel() );

		SetKeyPadVolume(GetKeypadVolumeLevel());
		SetLoudSpkrVolumeLevel(GetLoudspkrVolumeLevel());*/

	}
	//mdi_audio_set_mic_volume();  deleted by jinzh:20070731
	showAlertStatusIcons();  //解决更改情景模式后，idle界面上方铃声提示图标无变化的bug， opened by zhoumn @2007/03/01
	change_font_Theme();


/*******************************************************/
 
/* AT command will query MMI when needed               */
/*******************************************************/
   /*
   if (gactivatedprofile==5)
	   SilentModeOn();
   else
      SilentModeOff();
   */
	
}
/*****************************************************************

	FUNCTION NAME		: void ActivatefirstProfile(void)

  	PURPOSE				: Activating the profile first time bootup

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
void ActivatefirstProfile()
{ 
	gactivatedprofile = gprofileId;
	memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
}
/*****************************************************************

	FUNCTION NAME		: void ActivateHeadset(void)

  	PURPOSE				: Activating Headset profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
/*JINZH Modify Start For 6233 Ver: TBM780  on 2007-8-22 14:57 */
U8 g_prevSilentProfile = 0;
/*JINZH Modify End  For 6233 Ver: TBM780  on 2007-8-22 14:57 */
extern U8 isIdleScreenReached; //qiff add for bug 14932

void ActivateHeadset(void)
{
	S16 error =0 ;

	gprofileheadset = 1;
 
	/*Jinzh Modify Start For 6189 Ver: TBM780  on 2007-8-12 17:7 */
	if(gactivatedprofile !=MMI_PROFILE_HEADSET)
	{
		/*JINZH Add Start For 6233 Ver: TBM780  on 2007-8-22 15:0 */
		if(gactivatedprofile == MMI_PROFILE_SILENT)  //如果插入耳机前是静音模式
		{
			g_prevSilentProfile = glastactivatedprofile;
		}
		/*JINZH Add End  For 6233 Ver: TBM780  on 2007-8-22 15:0 */
		glastactivatedprofile = gactivatedprofile;
	}
	/*Jinzh Modify End  For 6189 Ver: TBM780  on 2007-8-12 17:7 */
 	gactivatedprofile = MMI_PROFILE_HEADSET;
	memcpy(&gcurrentprofile,&gprofiles[gactivatedprofile],sizeof(PROFILE));
	WriteValue( NVRAM_PROFILES_LASTACTIVATED,&glastactivatedprofile ,DS_BYTE, &error);
	showAlertStatusIcons();
	if(isIdleScreenReached)//qiff add for bug 14932
	{
	change_font_Theme();
	}
	/*jinzh Add Start For 6383 Ver: TBM780  on 2007-8-25 16:36 */
	if(isInCall() == FALSE)
	{
	/*jinzh Add End  For 6383 Ver: TBM780  on 2007-8-25 16:36 */
		if(gloudSpeaker == 1)
		{
			gloudSpeaker = 0;
			SetLSpkNowOff(TRUE);
		}
	}
}
/*****************************************************************

	FUNCTION NAME		: void DeactivateHeadset(void)

  	PURPOSE				: Deactivate Headset profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
void DeactivateHeadset(void)
{
	S16 error =0 ;
	gprofileheadset = 0;
	ReadValue( NVRAM_PROFILES_LASTACTIVATED,&glastactivatedprofile ,DS_BYTE, &error);
 	gactivatedprofile = glastactivatedprofile;
	memcpy(&gcurrentprofile,&gprofiles[gactivatedprofile],sizeof(PROFILE));
	WriteValue( NVRAM_PROFILES_ACTIVATED,&gactivatedprofile,DS_BYTE, &error);
    showAlertStatusIcons();
	if(isIdleScreenReached)//qiff add for bug 14932
	{
	change_font_Theme();
	}
#if 0	//Added by jinzh:20070730
	SetRingerVolume(GetRingVolumeLevel());   //reopened by jinzh:20070730
	mdi_audio_set_volume_no_apply(MDI_VOLUME_MEDIA, GetRingVolumeLevel() );

	SetKeyPadVolume(GetKeypadVolumeLevel());
	SetLoudSpkrVolumeLevel(GetLoudspkrVolumeLevel());
	mdi_audio_set_mic_volume();
#endif
/*Jinzh Del Start For P3板开机铃音正确的模式 Ver: TBM780  on 2007-8-18 17:52 */
	#if 0
	if(STATE_PLAY == g_audply.state)    //Added by jinzh:20070730
	{
		SetRingerVolume(g_audply.volume);
	}
	#endif
/*Jinzh Del End  For P3板开机铃音正确的模式 Ver: TBM780  on 2007-8-18 17:52 */
	/*JINZH Add Start For 6233 Ver: TBM780  on 2007-8-22 15:6 */
	if(glastactivatedprofile == 5)
		glastactivatedprofile = g_prevSilentProfile;
	else
	/*JINZH Add End  For 6233 Ver: TBM780  on 2007-8-22 15:6 */
		glastactivatedprofile = 4;

}
/*****************************************************************

	FUNCTION NAME		: void StopSilentProfileVibrator(void)

  	PURPOSE				: Stop Silent profile Vibrator

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
void StopSilentProfileVibrator(void)
{
	StopTimer(TIMER_PROFILES_TONE_PLAY);
	//added by PanXu on 20061230
 	VibratorOff();                         
 }

/*****************************************************************
	FUNCTION NAME		: void mmi_profiles_restore_activated_profile(void)
  	PURPOSE				: restore audio mode & current activated profile after factory mode
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
 
******************************************************************/
void mmi_profiles_restore_activated_profile( void )
{
	if( mmi_gpio_is_earphone_plug_in() )
	{
		// enter headset mode
		SendAudioModeRequest( AUD_MODE_HEADSET );
/*Jinzh Del Start For 6249,6246 Ver: TBM780  on 2007-8-17 11:27 */
		#if 0
		SetModeSpeechVolume();
		#endif
/*Jinzh Del End  For 6249,6246 Ver: TBM780  on 2007-8-17 11:27 */
		ActivateHeadset();
	}
	else
	{
		// enter non-headset mode
		SendAudioModeRequest( AUD_MODE_NORMAL );
/*Jinzh Del Start For 6249,6246 Ver: TBM780  on 2007-8-17 11:27 */
		#if 0
		SetModeSpeechVolume();
		#endif
/*Jinzh Del End  For 6249,6246 Ver: TBM780  on 2007-8-17 11:27 */
		ActivateProfile();
	}
}

/*****************************************************************

	FUNCTION NAME		: void ActivateSilentProfile(void)

  	PURPOSE				: Silent Profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/

void ActivateSilentProfile(void)
{
	S16 error =0 ;
	U16 j=1;
	ClearKeyEvents();
#ifdef __MMI_BT_PROFILE__
    if ((gactivatedprofile != MMI_PROFILE_SILENT) && (gprofileheadset != 1) && (gactivatedprofile != MMI_PROFILE_BT))
#else 
	if((gactivatedprofile != MMI_PROFILE_SILENT)&&(gprofileheadset != 1))
#endif 
	{
		memcpy(&gcurrentprofile,&gprofiles[gactivatedprofile],sizeof(PROFILE));
		glastactivatedprofile = gactivatedprofile;
 		gactivatedprofile = gprofileId = MMI_PROFILE_SILENT;
		WriteValue( NVRAM_PROFILES_ACTIVATED,&gactivatedprofile,DS_BYTE, &error);
		WriteValue( NVRAM_PROFILES_LASTACTIVATED,&glastactivatedprofile,DS_BYTE, &error);
//CSD brian modified to make it vibrate in silent mode, 2003/10/28
		gcurrentprofile.mtCallAlertTypeEnum = MMI_VIBRATION_ONLY ;
//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
		gcurrentprofile.toneSetup.coverCloseTone = COVER_CLOSE_SILENT;
		gcurrentprofile.toneSetup.coverOpenTone = COVER_OPEN_SILENT;
#endif
//CSD end
		gcurrentprofile.toneSetup.keypadTone = KEYPAD_SILENT;
		gcurrentprofile.toneSetup.messageTone = MESSAGE_SILENT;
		gcurrentprofile.toneSetup.powerOffTone = POWEROFF_SILENT;
		gcurrentprofile.toneSetup.powerOnTone = POWERON_SILENT;
		
		gcurrentprofile.extraTone.campOnTone = 0;
		gcurrentprofile.extraTone.errorTone = 0;
		gcurrentprofile.extraTone.connectTone = 0;
		gcurrentprofile.extraTone.warningTone = 0;

		gcurrentprofile.intelligentCallAlert = 	MMI_INTELLIGENT_CALL_ALERT_OFF;

		memcpy(&gprofiles[gactivatedprofile],&gcurrentprofile,sizeof(PROFILE));
		WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,&error);
		DeleteUptoScrID(IDLE_SCREEN_ID);

		DisplayPopup((U8*)GetString(STR_SILENT_ACTIVATE_MESSAGE),IMG_GLOBAL_ACTIVATED,1,2000,(U8)SUCCESS_TONE);
		
		showAlertStatusIcons();
		
		/************************************************/
		 
		/* AT command will query MMI when needed                   */
		/************************************************/
		//SilentModeOn();

		VibratorOn() ;
		StartTimer(TIMER_PROFILES_TONE_PLAY, 1000,StopSilentProfileVibrator);
	}


}
/*****************************************************************

	FUNCTION NAME		: void DeactivateSilentProfile(void)

  	PURPOSE				: DeactivateSilent Profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
void DeactivateSilentProfile(void)
{
	S16 error =0 ;

	if(gactivatedprofile == MMI_PROFILE_SILENT)
	{
		gprofileId = 0;
		/*Jinzh Modify Start For 6496 Ver: TBM780  on 2007-8-29 15:32 */
		if(MMI_PROFILE_SILENT == glastactivatedprofile)
			gactivatedprofile = MMI_PROFILE_GENERAL;
		else
 			gactivatedprofile = glastactivatedprofile;
		/*Jinzh Modify End  For 6496 Ver: TBM780  on 2007-8-29 15:32 */
		
		memcpy(&gcurrentprofile,&gprofiles[gactivatedprofile],sizeof(PROFILE));
		WriteValue( NVRAM_PROFILES_ACTIVATED,&gactivatedprofile,DS_BYTE, &error);	

		//SetRingerVolume(GetRingVolumeLevel());
  #if 0//gdm 080610 fix bug 8632
 		mdi_audio_set_volume_no_apply(MDI_VOLUME_MEDIA, GetRingVolumeLevel() );
  #endif
/*JINZH Del Start For 正确音量 Ver: TBM780  on 2007-8-22 11:44 */
		#if 0
		SetKeyPadVolume(GetKeypadVolumeLevel());
		SetLoudSpkrVolumeLevel(GetLoudspkrVolumeLevel());
		#endif
/*JINZH Del End  For 正确音量 Ver: TBM780  on 2007-8-22 11:44 */
		change_font_Theme();
		DeleteUptoScrID(IDLE_SCREEN_ID);

		switch(gactivatedprofile)
		{
		case 0:
			DisplayPopup((U8*)GetString(STR_GENERAL_RESTORED_MESSAGE),IMG_GLOBAL_ACTIVATED,1,2000,(U8)SUCCESS_TONE);
			break;
		case 1:
			DisplayPopup((U8*)GetString(STR_MEETING_RESTORED_MESSAGE),IMG_GLOBAL_ACTIVATED,1,2000,(U8)SUCCESS_TONE);
			break;
		case 2:
			DisplayPopup((U8*)GetString(STR_OUTDOOR_RESTORED_MESSAGE),IMG_GLOBAL_ACTIVATED,1,2000,(U8)SUCCESS_TONE);
			break;
		case 3:
			DisplayPopup((U8*)GetString(STR_INDOOR_RESTORED_MESSAGE),IMG_GLOBAL_ACTIVATED,1,2000,(U8)SUCCESS_TONE);
			break;
		case 4:
//			DisplayPopup((U8*)GetString(STR_HEADSET_RESTORED_MESSAGE),IMG_GLOBAL_ACTIVATED,1,2000,(U8)SUCCESS_TONE);
			break;

		case 5:
//			DisplayPopup((U8*)GetString(STR_SILENT_RESTORED_MESSAGE),IMG_GLOBAL_ACTIVATED,1,2000,(U8)SUCCESS_TONE);
			break;

		case 6:
//			DisplayPopup((U8*)GetString(STR_LOUDSPEAKER_RESTORED_MESSAGE),IMG_GLOBAL_ACTIVATED,1,2000,(U8)SUCCESS_TONE);
			break;
		}
		showAlertStatusIcons();
		
		/************************************************/
		 
		/* AT command will query MMI when needed                   */
		/************************************************/
		//SilentModeOff();
	}
}
/*************************************************************************
					Set Functions
**************************************************************************/



/*************************************************************************

	FUNCTION NAME		: void  SetRingVolumeLevel(MMI_VOLUME_LEVEL_ENUM)

  	PURPOSE				: Setting Ring Volume Level

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
S16 SetRingVolumeLevel(MMI_VOLUME_LEVEL_ENUM volumelevel )
{
	S16 error=0;
	U16 j=1;
	MMI_VOLUME_LEVEL_ENUM ringVolumeLevel;

	ringVolumeLevel = gprofiles[gprofileId].ringVolumeLevel;

	gprofiles[gprofileId].ringVolumeLevel = volumelevel ;

	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	gprofiles[gprofileId].ringVolumeLevel = ringVolumeLevel ;
	 PRINT_INFORMATION (("## ERROR: SetRingVolumeLevel: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	 return error;
	}

	if( gactivatedprofile == gprofileId)
	{
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
	}

	SetRingerVolume(gprofiles[gactivatedprofile].ringVolumeLevel);

return 0;
}
//CSD Brian added for setting activated ring tone volume, 2003/12/02
/*************************************************************************

	FUNCTION NAME		: void  SetActivatedRingVolumeLevel(MMI_VOLUME_LEVEL_ENUM)

  	PURPOSE				: Setting Ring Volume Level of the Activated Profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
S16 SetActivatedRingVolumeLevel(MMI_VOLUME_LEVEL_ENUM volumelevel )
{
	S16 error=0;
	U16 j=1;
       MMI_VOLUME_LEVEL_ENUM ringVolumeLevel;
	   
	ringVolumeLevel = gprofiles[gactivatedprofile].ringVolumeLevel;
	gprofiles[gactivatedprofile].ringVolumeLevel = volumelevel ;
   

	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	gprofiles[gactivatedprofile].ringVolumeLevel = ringVolumeLevel;
	 PRINT_INFORMATION (("## ERROR: SetRingVolumeLevel: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	 return error;
	}
	gcurrentprofile.ringVolumeLevel = volumelevel ;
	SetRingerVolume(volumelevel);

return 0;
}

/*************************************************************************

	FUNCTION NAME		: void  SetKeypadVolumeLevel(MMI_VOLUME_LEVEL_ENUM)

  	PURPOSE				: Setting Keypad Volume Level

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
S16  SetKeypadVolumeLevel(MMI_VOLUME_LEVEL_ENUM volumelevel)
{
	S16 error=0;
	U16 j=1;
	MMI_VOLUME_LEVEL_ENUM keypadVolumelevel;

	gprofiles[gprofileId].keypadVolumeLevel = volumelevel ;

	if( gactivatedprofile == gprofileId)
	{
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
	}

	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	 PRINT_INFORMATION (("## ERROR: SetKeypadVolumeLevel: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	 return error;
	}
/***** restore the keypad tone volume of activated profile******/
	keypadVolumelevel = gprofiles[gactivatedprofile].keypadVolumeLevel;

	SetKeyPadVolume(keypadVolumelevel);

return 0;
}
//CSD Brian added for setting activated keypad volume, 2003/12/02
/*************************************************************************

	FUNCTION NAME		: void  SetKeypadVolumeLevelActivated(MMI_VOLUME_LEVEL_ENUM)

  	PURPOSE				: Setting Keypad Volume Level of the Activated Profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
S16  SetActivatedKeypadVolumeLevel(MMI_VOLUME_LEVEL_ENUM volumelevel)
{
	S16 error=0;
	U16 j=1;

	gprofiles[gactivatedprofile].keypadVolumeLevel = volumelevel ;
   gcurrentprofile.keypadVolumeLevel = volumelevel ;

	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	 PRINT_INFORMATION (("## ERROR: SetActivatedKeypadVolumeLevel: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	 return error;
	}

	SetKeyPadVolume(volumelevel);

return 0;
}
/*************************************************************************

	FUNCTION NAME		: void  SetLoudSpkrVolumeLevel(MMI_VOLUME_LEVEL_ENUM)

  	PURPOSE				: Setting Earphone Volume Level

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************************/
S16  SetLoudSpkrVolumeLevel(MMI_VOLUME_LEVEL_ENUM volumelevel)
{
	return 1;
}
/*************************************************************************

	FUNCTION NAME		: void  SetMtCallAlertTypeEnum(MT_CALL_ALERT_TYPE_ENUM)

  	PURPOSE				: Setting Call Alert Type 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************************/
S16  SetMtCallAlertTypeEnum( MMI_ALERT_TYPE callalerttype)
{
S16 error=0;
U16 j=1;

//CSD Brian modified to skip writing nvram if no change, 2003/11/26   
   if (gprofiles[gprofileId].mtCallAlertTypeEnum != callalerttype)
   {
      gprofiles[gprofileId].mtCallAlertTypeEnum = callalerttype;

   	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
   						& error)<NVRAM_PROFILES_RECORD_SIZE)
   	{
   	 PRINT_INFORMATION (("## ERROR: SetMtCallAlertTypeEnum: WriteRecord "
   									"to NVRAM failed (ecode%d)\n", (int) error));
   	 return error;
   	}
   	if( gactivatedprofile == gprofileId)
   	{
   		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
   	}
   	showAlertStatusIcons();

	/************************************************/
	 
	/* AT command will query MMI when needed                   */
	/************************************************/
	/*
         if (callalerttype == MMI_VIBRATION_ONLY || 
            callalerttype == MMI_VIBRATION_AND_RING||
            callalerttype == MMI_VIBRATION_THEN_RING)
            SendSetVibrationMode(1);
         else      
      	   SendSetVibrationMode(0);
      	 */
   }	
   return 0;
}
/*************************************************************************

	FUNCTION NAME		: void  SetRingToneID(U16)

  	PURPOSE				: Setting Ring tone ID. 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************************/
S16  SetRingToneID(U16 ringtoneid)
{
	S16 error=0;
	U16 j=1;
	TRACE_SYS_FUNCTION();
	gprofiles[gprofileId].toneSetup.ringTone = ringtoneid;

	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	 PRINT_INFORMATION (("## ERROR: SetRingToneID: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	 return error;
	}
	if( gactivatedprofile == gprofileId)
	{
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
	}

	TRACE_SYS_FUNCTION();
	return 0;
}

#ifdef __MMI_DUAL_PROFILE_SUPPORT__  //Add by Renyh for 221 project   20090213
/*****************************************************************************
 * FUNCTION
 *  SetCard2RingToneID
 * DESCRIPTION
 *  Setting Card2 Ring tone ID.
 * PARAMETERS
 *  ringtoneid      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
S16 SetCard2RingToneID(U16 ringtoneid)
{
    S16 error = 0;
    U16 j = 1;

    gprofiles[gprofileId].toneSetup.card2_ringTone = ringtoneid;

    if (WriteRecord(NVRAM_EF_PROFILES_LID,
                    j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE, &error) < NVRAM_PROFILES_RECORD_SIZE)
    {
    	PRINT_INFORMATION (("## ERROR: SetCard2RingToneID: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
        return error;
    }
    if (gactivatedprofile == gprofileId)
    {
        memcpy(&gcurrentprofile, &gprofiles[gprofileId], sizeof(PROFILE));
    }

    return 0;
}

#endif                                                        //Add by Renyh for 221 project   20090213

#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
void ThemeManagerSetRingTone(U16 ringtoneid)
{
	S16 error=0;
	gprofiles[gactivatedprofile].toneSetup.ringTone = ringtoneid;
	WriteRecord(NVRAM_EF_PROFILES_LID, 1, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE, &error);
	memcpy(&gcurrentprofile,&gprofiles[gactivatedprofile],sizeof(PROFILE));
}

void ThemeManagerResetRingTone(void)
{
	U8 flag = 0, i;
	S16 error;

	for(i = 0; i < 5; ++i)
	{
		if(gprofiles[i].toneSetup.ringTone == CURRENT_THEME_INCOMING_CALL_TONE)
		{
			flag=1;
			gprofiles[i].toneSetup.ringTone = MIDI_1;	
		}
	}
	if(flag)
	{
		WriteRecord(NVRAM_EF_PROFILES_LID,1, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE, &error);
		memcpy(&gcurrentprofile,&gprofiles[gactivatedprofile],sizeof(PROFILE));
	}
}
#ifdef __MMI_DUAL_PROFILE_SUPPORT__  //Add by Renyh for 221 project   20090213
/*****************************************************************************
 * FUNCTION
 *  ThemeManagerSetCard2RingTone
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ringtoneid      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void ThemeManagerSetCard2RingTone(U16 ringtoneid)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 error = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gprofiles[gactivatedprofile].toneSetup.card2_ringTone = ringtoneid;
    WriteRecord(NVRAM_EF_PROFILES_LID, 1, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE, &error);
    memcpy(&gcurrentprofile, &gprofiles[gactivatedprofile], sizeof(PROFILE));
}

/*****************************************************************************
 * FUNCTION
 *  ThemeManagerResetCard2RingTone
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ThemeManagerResetCard2RingTone(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 flag = 0, i;
    S16 error;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MAX_ELEMENTS; ++i)
    {
        if (gprofiles[i].toneSetup.card2_ringTone == CURRENT_THEME_INCOMING_CALL_TONE)
        {
            flag = 1;
            gprofiles[i].toneSetup.card2_ringTone = MIDI_1;
        }
    }
    if (flag)
    {
        WriteRecord(NVRAM_EF_PROFILES_LID, 1, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE, &error);
        memcpy(&gcurrentprofile, &gprofiles[gactivatedprofile], sizeof(PROFILE));
    }
}
#endif /*__MMI_DUAL_PROFILE_SUPPORT__*/ //Add by Renyh for 221 project   20090213

#endif /* __MMI_DOWNLOADABLE_THEMES_SUPPORT__ */ 
/*************************************************************************

	FUNCTION NAME		: void  SetLightStatus(MMI_BACKLIGHT_STATUS_ENUM)

  	PURPOSE				: Setting Light status (Color) 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************************/
S16  SetLightStatus(MMI_LEDLIGHT_STATUS_ENUM backlightstatus)
{
S16 error=0;
U16 j=1;
gprofiles[gprofileId].light.status = backlightstatus;                   

	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	 PRINT_INFORMATION (("## ERROR: SetLightStatus: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	 return error;
	}
	if( gactivatedprofile == gprofileId)
	{
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
	}
return 0;
}
/*************************************************************************

	FUNCTION NAME		: void  SetRingTypeEnum(RING_TYPE_ENUM)

  	PURPOSE				: Setting Ring Type 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

*******************************************************************************/
S16  SetRingTypeEnum(MMI_RING_TYPE_ENUM ringtype)
{
S16 error=0;
U16 j=1;
gprofiles[gprofileId].ringTypeEnum = ringtype;                   

	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	 PRINT_INFORMATION (("## ERROR: SetRingTypeEnum: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	 return error;
	}
	if( gactivatedprofile == gprofileId)
	{
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
	}
return 0;
}
#ifdef __MMI_CONNECT_NOTICE__
/*************************************************************************

	FUNCTION NAME		: void  SetConnectNoticeEnum(MMI_CONNECT_NOTICE_TYPE)

  	PURPOSE				: Setting Connect Notice Type 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

*******************************************************************************/
S16  SetConnectNotice(U16 type)
{
   S16 error=0;
   U16 j=1;
   gprofiles[gprofileId].extraTone.connectTone = type;                   

	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	 PRINT_INFORMATION (("## ERROR: SetConnectNoticeEnum: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	 return error;
	}
	if( gactivatedprofile == gprofileId)
	{
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
	}
return 0;
}
#endif
/******************************************************************************

	FUNCTION NAME		: void  SetAnsweringModeEnum(MMI_ANSWERING_MODE_ENUM)

  	PURPOSE				: Setting Answering Mode

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************************/
S16  SetAnsweringMode(MMI_ANSWERING_MODE answeringmode)
{
	S16 error = 0;
	U16 j = 1;

#ifdef __MMI_MAINLCD_220X176__

	/* 应答模式挪到通话设置中，且保存在gprofiles[0]中*/
	memcpy(&gprofiles[0].answeringMode, &answeringmode, sizeof(MMI_ANSWERING_MODE));                   

	if (WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	 PRINT_INFORMATION (("## ERROR: SetAnsweringModeEnum: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	 return error;
	}

#else
memcpy(&gprofiles[gprofileId].answeringMode,&answeringmode,sizeof(MMI_ANSWERING_MODE));                   

	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	 PRINT_INFORMATION (("## ERROR: SetAnsweringModeEnum: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	 return error;
	}
	if( gactivatedprofile == gprofileId)
	{
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
	}
#endif
	
return 0;
}
/******************************************************************************

	FUNCTION NAME		: void  SetLoudSpeaker(pBOOL loudspeaker)

  	PURPOSE				: Setting Loud Speaker (On/Off)

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************************/
S16  SetLoudSpeaker(pBOOL loudspeaker)
{

	if(loudspeaker ==1)
	{
		gloudSpeaker = loudspeaker;
	}
	else
	{  
		gloudSpeaker = loudspeaker;
	}

/*JINZH Del Start For 6277 Ver: TBM780  on 2007-8-22 11:38 */
	#if 0
	SetRingerVolume(GetRingVolumeLevel());
	SetKeyPadVolume(GetKeypadVolumeLevel());
	SetLoudSpkrVolumeLevel(GetLoudspkrVolumeLevel());
	#endif
/*JINZH Del End  For 6277 Ver: TBM780  on 2007-8-22 11:38 */

	return 0;

}

#ifdef __MMI_CHANGABLE_FONT__
/******************************************************************************

	FUNCTION NAME		: void  SetFontSizeEnum(MMI_FONT_SIZE_ENUM)

  	PURPOSE				: Setting Font Size Enum

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************************/
S16  SetFontSizeEnum(MMI_FONT_SIZE_ENUM fontsize)
{
S16 error=0;
U16 j=1;
gprofiles[gprofileId].fontSizeEnum = fontsize;                   

	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	  PRINT_INFORMATION (("## ERROR: SetFontSizeEnum: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	  return error;
	}
	if( gactivatedprofile == gprofileId)
	{
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
	}
	change_font_Theme();
	return 0;
}
#endif

/******************************************************************************

	FUNCTION NAME		: void  SetErrorToneStatus(pBOOL)

  	PURPOSE				: Setting Error Tone status (On/Off)

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

*****************************************************************************/
S16  SetErrorToneStatus(pBOOL errortonestatus)
{
S16 error=0;
U16 j=1;
gprofiles[gprofileId].extraTone.errorTone = errortonestatus;  

	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	  PRINT_INFORMATION (("## ERROR: SetErrorToneStatus: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	  return error;
	}
	if( gactivatedprofile == gprofileId)
	{
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
	}
return 0;
}
#ifndef __MMI_CONNECT_NOTICE__
/*****************************************************************************

	FUNCTION NAME		: void  SetConnectToneStatus(pBOOL)

  	PURPOSE				: Setting Connect Tone status (On/Off)

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************************/
S16  SetConnectToneStatus(pBOOL connecttone)
{
S16 error=0;
U16 j=1;
gprofiles[gprofileId].extraTone.connectTone = connecttone;                   

	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	  PRINT_INFORMATION (("## ERROR: SetConnectToneStatus: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	  return error;
	}
	if( gactivatedprofile == gprofileId)
	{
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
	}
return 0;
}
#endif
/*****************************************************************************

	FUNCTION NAME		: S16  SetPowerOnToneType(TONE_SETUP tone)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************************/
S16  SetPowerOnToneType(TONE_SETUP tone)
{
S16 error=0;
U16 j=1;
gprofiles[gprofileId].toneSetup.powerOnTone = tone.powerOnTone;                   

	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	  PRINT_INFORMATION (("## ERROR: SetPowerOnToneType: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	  return error;
	}
	if( gactivatedprofile == gprofileId)
	{
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
	}
return 0;
}
/*****************************************************************************

	FUNCTION NAME		: void  SetPowerOffToneType(POWEROFF_TONE_ENUM powerofftone)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************************/
S16  SetPowerOffToneType(TONE_SETUP tone)
{
S16 error=0;
U16 j=1;
gprofiles[gprofileId].toneSetup.powerOffTone = tone.powerOffTone;                   

	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	  PRINT_INFORMATION (("## ERROR: SetPowerOffToneType: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	  return error;
	}
	if( gactivatedprofile == gprofileId)
	{
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
	}
return 0;
}
//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
/*****************************************************************************

	FUNCTION NAME		: S16  SetCoverOpenToneType(TONE_SETUP tone);

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************************/
S16  SetCoverOpenToneType(TONE_SETUP tone)
{
S16 error=0;
U16 j=1;
gprofiles[gprofileId].toneSetup.coverOpenTone = tone.coverOpenTone;

	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	  PRINT_INFORMATION (("## ERROR: SetCoverOpenToneType: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	  return error;
	}
	if( gactivatedprofile == gprofileId)
	{
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
	}
return 0;
}
/*****************************************************************************

	FUNCTION NAME		:S16  SetCoverCloseToneType(TONE_SETUP);

  	PURPOSE				: 
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************************/
S16  SetCoverCloseToneType(TONE_SETUP tone)
{
S16 error=0;
U16 j=1;
gprofiles[gprofileId].toneSetup.coverCloseTone = tone.coverCloseTone;                   

	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	  PRINT_INFORMATION (("## ERROR: SetCoverCloseToneType: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	  return error;
	}
	if( gactivatedprofile == gprofileId)
	{
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
	}
return 0;
}
#endif
//CSD end
/*****************************************************************************

	FUNCTION NAME		: void  SetKeypadToneType(KEYPAD_TONE_ENUM keypadtone)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************************/
S16  SetKeypadToneType(KEYPAD_TONE_ENUM keypadtone)
{
S16 error=0;
U16 j=1;
gprofiles[gprofileId].toneSetup.keypadTone = keypadtone;                   

	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	  PRINT_INFORMATION (("## ERROR: SetKeypadToneType: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	  return error;
	}
	if( gactivatedprofile == gprofileId)
	{
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
	}
return 0;
}

/*****************************************************************************
	FUNCTION NAME		: MMI_INTELLIGENT_CALL_ALERT_TYPE GetIntelligentCallAert(void)
  	PURPOSE				:
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: MMI_INTELLIGENT_CALL_ALERT_TYPE
 
******************************************************************************/
MMI_INTELLIGENT_CALL_ALERT_TYPE GetIntelligentCallAert( void )
{
	MMI_INTELLIGENT_CALL_ALERT_TYPE intelligentCallAlert = gcurrentprofile.intelligentCallAlert;

	PRINT_INFORMATION(("File: Setting Prof.c <<GetIntelligentCallAlert> the intelligentCallAlert is : %d \n",intelligentCallAlert));
	
	return intelligentCallAlert;	
}

/*****************************************************************************
	FUNCTION NAME		: void  SetIntelligentCallAert(MMI_INTELLIGENT_CALL_ALERT_TYPE intelligentCallAlert)
  	PURPOSE				:
	INPUT PARAMETERS	: intelligentCallAlert
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
 
******************************************************************************/
S16 SetIntelligentCallAert(MMI_INTELLIGENT_CALL_ALERT_TYPE intelligentCallAlert)
{
	S16 error=0;
	U16 j=1;
	gprofiles[gprofileId].intelligentCallAlert = intelligentCallAlert;                   

	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	  PRINT_INFORMATION (("## ERROR: SetIntelligentCallAert: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	  return error;
	}
	if( gactivatedprofile == gprofileId)
	{
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
	}
return 0;
}

/*****************************************************************************

	FUNCTION NAME		: S16  SetMessageToneType(TONE_SETUP);

  	PURPOSE				: Setting Connect Tone status (On/Off)

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************************/
S16  SetMessageToneType(TONE_SETUP tone)
{
S16 error=0;
U16 j=1;

TRACE_EF(1,"gprofileId = %d" ,gprofileId);
gprofiles[gprofileId].toneSetup.messageTone = tone.messageTone;                   

	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	  PRINT_INFORMATION (("## ERROR: SetMessageToneType: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	  TRACE_SYS_FUNC("## ERROR: SetMessageToneType: WriteRecord ");
	  return error;
	}
	if( gactivatedprofile == gprofileId)
	{
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
	}
return 0;
}

#ifdef __MMI_DUAL_PROFILE_SUPPORT__  //Add by Renyh for 221 project   20090213
/*****************************************************************************
 * FUNCTION
 *  SetCard2MessageToneType
 * DESCRIPTION
 *  Setting Card2 Message Tone
 * PARAMETERS
 *  tone        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
S16 SetCard2MessageToneType(TONE_SETUP tone)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 error = 0;
    U16 j = 1;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gprofiles[gprofileId].toneSetup.card2_messageTone = tone.card2_messageTone;

    if (WriteRecord(
            NVRAM_EF_PROFILES_LID,
            j,
            &gprofiles[0],
            NVRAM_PROFILES_RECORD_SIZE,
            &error) < NVRAM_PROFILES_RECORD_SIZE)
    {
        PRINT_INFORMATION(("## ERROR: SetCard2MessageToneType: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int)error));
        return error;
    }
    if (gactivatedprofile == gprofileId)
    {
        memcpy(&gcurrentprofile, &gprofiles[gprofileId], sizeof(PROFILE));
    }
    return 0;
}
#endif                                                        //Add by Renyh for 221 project   20090213

/*****************************************************************************

	FUNCTION NAME		: void  SetAlarmToneType(TONE_SETUP alarmtone)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************************/
S16  SetAlarmToneID(U16 id)
{
S16 error=0;
U16 j=1;
gprofiles[gprofileId].toneSetup.alarmTone = id;                   

	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	  PRINT_INFORMATION (("## ERROR: SetAlarmToneType: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	  return error;
	}
	if( gactivatedprofile == gprofileId)
	{
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
	}
return 0;
}

#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__ //ritesh
void ThemeManagerSetAlarmTone(U16 ringtoneid)
{
	S16 error=0;
	gprofiles[gactivatedprofile].toneSetup.alarmTone = MIDI_2;                   
	WriteRecord(NVRAM_EF_PROFILES_LID,1, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE, &error);
	memcpy(&gcurrentprofile,&gprofiles[gactivatedprofile],sizeof(PROFILE));
}

void ThemeManagerResetAlarmTone(void)
{
	U8 flag = 0, i;
	S16 error;

	for(i = 0; i < 5; ++i)
	{
		if(gprofiles[i].toneSetup.alarmTone == CURRENT_THEME_ALARM_EXPIRY_TONE)
		{
			flag=1;
			gprofiles[i].toneSetup.alarmTone = MIDI_2;	
		}
	}
	if(flag)
	{
		WriteRecord(NVRAM_EF_PROFILES_LID,1, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE, &error);
		memcpy(&gcurrentprofile,&gprofiles[gactivatedprofile],sizeof(PROFILE));
	}
}
#endif
/*****************************************************************

	FUNCTION NAME		: void set_lcdbacklighttimer(void)

  	PURPOSE				: Setting lcdbacklight timer value

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
U8 set_lcdbacklighttimer(void)
{
S16 error=0;
U16 j=1;

#ifdef __MMI_MAINLCD_220X176__ 
    gprofiles[0].light.timer.setsec = gSec;

	
    if (WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
    {
		PRINT_INFORMATION (("## ERROR: set_lcdbacklighttimer: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
			return ((U8)error);
    }
#else /* __MMI_MAINLCD_220X176__ */
    gprofiles[gprofileId].light.timer.setsec = gSec;

	
	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
		PRINT_INFORMATION (("## ERROR: set_lcdbacklighttimer: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
			return ((U8)error);
	}
	if( gactivatedprofile == gprofileId)
	{
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
	}
#endif	
	return gSec ;
}

/*****************************************************************

	FUNCTION NAME		: SetProfileDefault(void)

  	PURPOSE				: SetProfileDefault

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************/
pBOOL SetProfileDefault(void)
{
S16 error ;

	if(gprofileheadset !=1)
	{
   	     PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== SetProfileDefault(): Headset mode. ===\n"));

		ReadRecord (NVRAM_EF_PROFILES_LID, 2 , &gprofiles[0],MAX_ELEMENTS*sizeof(PROFILE)/*NVRAM_PROFILES_RECORD_SIZE*/,&error);
		gactivatedprofile = 0;
		memcpy(&gcurrentprofile,&gprofiles[gactivatedprofile],sizeof(PROFILE));
		showAlertStatusIcons();  /*function to show status icon on ilde screen*/		
		WriteRecord (NVRAM_EF_PROFILES_LID, 1, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
					& error);
		WriteValue( NVRAM_PROFILES_ACTIVATED, &gactivatedprofile, DS_BYTE , &error);
				
/*				
#ifdef __MMI_AUDIO_PLAYER__
		mmi_audply_save_volume( gactivatedprofile );
#endif // __MMI_AUDIO_PLAYER__

		SetRingerVolume(GetRingVolumeLevel());
*/
           mdi_audio_set_volume_no_apply(MDI_VOLUME_MEDIA, GetRingVolumeLevel() );

		SetKeyPadVolume(GetKeypadVolumeLevel());
		SetLoudSpkrVolumeLevel(GetLoudspkrVolumeLevel());
		change_font_Theme();
		if(error == NVRAM_WRITE_SUCCESS)
		return TRUE;
		else
		return FALSE;
	}
	else
	{
   	     PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "=== SetProfileDefault(): Not Headset mode. ===\n"));

		ReadRecord (NVRAM_EF_PROFILES_LID, 2 , &gprofiles[0],MAX_ELEMENTS*sizeof(PROFILE)/*NVRAM_PROFILES_RECORD_SIZE*/,&error);
		memcpy(&gcurrentprofile,&gprofiles[gactivatedprofile],sizeof(PROFILE));
		showAlertStatusIcons();  /*function to show status icon on ilde screen*/		
		WriteRecord (NVRAM_EF_PROFILES_LID, 1, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
					& error);
		WriteValue( NVRAM_PROFILES_ACTIVATED, &gactivatedprofile, DS_BYTE , &error);
				
/*				
#ifdef __MMI_AUDIO_PLAYER__
		mmi_audply_save_volume( gactivatedprofile );
#endif // __MMI_AUDIO_PLAYER__

		SetRingerVolume(GetRingVolumeLevel());
*/
           mdi_audio_set_volume_no_apply(MDI_VOLUME_MEDIA, GetRingVolumeLevel() );


		SetKeyPadVolume(GetKeypadVolumeLevel());
		SetLoudSpkrVolumeLevel(GetLoudspkrVolumeLevel());
		change_font_Theme();
		if(error == NVRAM_WRITE_SUCCESS)
		return TRUE;
		else
		return FALSE;
	}	
	
}
/*****************************************************************************

	FUNCTION NAME		: void SetExtraTone(EXTRA_TONE extraTone)

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************************/
S16  SetExtraTone(EXTRA_TONE extraTone)
{
S16 error=0;
U16 j=1;

gprofiles[gprofileId].extraTone.warningTone = extraTone.warningTone;                   
gprofiles[gprofileId].extraTone.errorTone = extraTone.errorTone;     
#ifndef __MMI_CONNECT_NOTICE__
gprofiles[gprofileId].extraTone.connectTone = extraTone.connectTone;                   
#endif
gprofiles[gprofileId].extraTone.campOnTone = extraTone.campOnTone;
                                 

	if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	  PRINT_INFORMATION (("## ERROR: SetWarningToneType: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	  return error;
	}
	if( gactivatedprofile == gprofileId)
	{
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
	}
return 0;
}
/*****************************************************************************

	FUNCTION NAME		: void playRequestedTone()

  	PURPOSE				: API to all applications to play tone.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************************/
void playRequestedTone(ALL_TONE_ENUM playtone)
{
//	if(GetTotalActiveCallCount()==0 && GetTotalHoldCallCount()==0)
#if defined(__MMI_ANALOGTV__)
    if(mmi_atv_is_analogtv_scr())
        return;
#endif    

	if( !isInCall() )
	{
		SetPlayReqToneFlag(TRUE);
		switch(playtone)
		{
		case ERROR_TONE:
			if(gcurrentprofile.extraTone.errorTone == 1)
				AudioPlayReq(TONE_ERROR1,DEVICE_AUDIO_PLAY_ONCE);		
			break;

		case CONNECT_TONE:
#ifndef __MMI_CONNECT_NOTICE__
			if(gcurrentprofile.extraTone.connectTone == 1)
#endif
				AudioPlayReq(TONE_CALL_CONNECT,DEVICE_AUDIO_PLAY_ONCE);	
			break;
#if !defined (__MMI_RESOURCE_SLIM_ON_128X160__)
		case CAMP_ON_TONE:
			if(gcurrentprofile.extraTone.campOnTone == 1)
				AudioPlayReq(SOUND_3,DEVICE_AUDIO_PLAY_ONCE);	/*wangrui modify*///chenhe change SOUND_3 TO TONE_BUSY_CALL_GSM,for the SOUND_3 is to long and device is speaker
			break;
#else
		case CAMP_ON_TONE:
			if(gcurrentprofile.extraTone.campOnTone == 1)
				AudioPlayReq(SOUND_1,DEVICE_AUDIO_PLAY_ONCE);	/*wangrui modify*///chenhe change SOUND_3 TO TONE_BUSY_CALL_GSM,for the SOUND_3 is to long and device is speaker
			break;

#endif
		case WARNING_TONE:
			if(gcurrentprofile.extraTone.warningTone == 1)
				AudioPlayReq(TONE_WARNING1,DEVICE_AUDIO_PLAY_ONCE);	
			break;

          /**************************************************************************************/
          /***** Shall not come to here, Play ring tone in StartRingTone() use AudioPlayToneWithCallBack()  *****/
		case INCOMING_CALL_TONE:
			if(gactivatedprofile!=5)
				AudioPlayReq(gcurrentprofile.toneSetup.ringTone,(U8)GetRingTypeEnum());	
			break;
          /**************************************************************************************/

		case ALARM_TONE:
 
#ifdef __MMI_ALM_CUSTOMIZE_TONE__  
			if(g_alm_frm_cntx.CurrAlmIndex >= ALM_ALM_START && g_alm_frm_cntx.CurrAlmIndex < ALM_SPOF_START)
			{
				AudioPlayToneWithCallBack( g_alm_cntx.AlmList[g_alm_frm_cntx.CurrAlmIndex - ALM_ALM_START].AudioOptionValue,  DEVICE_AUDIO_PLAY_CRESCENDO, ALARM_TONE);
			}
			else
			{
				AudioPlayToneWithCallBack( gcurrentprofile.toneSetup.alarmTone,  DEVICE_AUDIO_PLAY_CRESCENDO, ALARM_TONE);
			}
#else
			AudioPlayToneWithCallBack( gcurrentprofile.toneSetup.alarmTone,  DEVICE_AUDIO_PLAY_INFINITE/*DEVICE_AUDIO_PLAY_CRESCENDO*/, ALARM_TONE);//dongwg modify 20070726
#endif			//__MMI_ALM_CUSTOMIZE_TONE__
 
				
			break;

		case POWER_ON_TONE:
			if(gcurrentprofile.toneSetup.powerOnTone != POWERON_SILENT)
				AudioPlayReq(gcurrentprofile.toneSetup.powerOnTone,DEVICE_AUDIO_PLAY_ONCE);	
			break;

		case POWER_OFF_TONE:
			if(gcurrentprofile.toneSetup.powerOffTone != POWEROFF_SILENT)
				AudioPlayReq(gcurrentprofile.toneSetup.powerOffTone,DEVICE_AUDIO_PLAY_ONCE);	
			break;

#ifdef __MMI_CLAMSHELL__
		case COVER_OPEN_TONE:
			if(gcurrentprofile.toneSetup.coverOpenTone != COVER_OPEN_SILENT)
				AudioPlayReq(gcurrentprofile.toneSetup.coverOpenTone,DEVICE_AUDIO_PLAY_ONCE);	
			break;

		case COVER_CLOSE_TONE:
			if(gcurrentprofile.toneSetup.coverCloseTone != COVER_CLOSE_SILENT)
				AudioPlayReq(gcurrentprofile.toneSetup.coverCloseTone,DEVICE_AUDIO_PLAY_ONCE);	
			break;
		#endif

		case MESSAGE_TONE:
			if(gcurrentprofile.toneSetup.messageTone != MESSAGE_SILENT)
			{
			   //AudioPlayReq(gcurrentprofile.toneSetup.messageTone,DEVICE_AUDIO_PLAY_ONCE);	
			   AudioPlayToneWithCallBack( gcurrentprofile.toneSetup.messageTone,  DEVICE_AUDIO_PLAY_ONCE, ALARM_TONE);
			}
			break;
		#ifdef __MMI_DUAL_PROFILE_SUPPORT__
		// TODO: 改变播放的真正声音
		case CARD2_MESSAGE_TONE:
			if(gcurrentprofile.toneSetup.card2_messageTone != MESSAGE_SILENT)
			{
			   AudioPlayToneWithCallBack( gcurrentprofile.toneSetup.card2_messageTone,  DEVICE_AUDIO_PLAY_ONCE, ALARM_TONE);
			}
			break;
		#endif
		case SUCCESS_TONE:
			break;

		case SAVE_TONE:
		//CSD Brian added for disable save tone */
			//AudioPlayReq(	TONE_GENERAL_BEEP,DEVICE_AUDIO_PLAY_ONCE);	
			break;

		case EMPTY_LIST_TONE:
		//	AudioPlayReq(	TONE_GENERAL_BEEP,DEVICE_AUDIO_PLAY_ONCE);	
			if(gcurrentprofile.extraTone.warningTone == 1)
				AudioPlayReq(TONE_WARNING1,DEVICE_AUDIO_PLAY_ONCE);	
			break;

		case GENERAL_TONE:
			if(gcurrentprofile.extraTone.warningTone == 1)
				AudioPlayReq(	TONE_GENERAL_BEEP,DEVICE_AUDIO_PLAY_ONCE);	
			break;
		case AUX_TONE:
			if(gcurrentprofile.extraTone.warningTone == 1)
				AudioPlayReq(TONE_AUX_IND,DEVICE_AUDIO_PLAY_ONCE);
			break;
//Brian added for battery indication, 2003/11/17
		case BATTERY_LOW_TONE:
			if(gcurrentprofile.extraTone.warningTone == 1)
				AudioPlayReq(TONE_BATTERY_LOW,DEVICE_AUDIO_PLAY_ONCE);	
			break;
		case BATTERY_WARNING_TONE:
			if(gcurrentprofile.extraTone.warningTone == 1)
				AudioPlayReq(TONE_BATTERY_WARNING,DEVICE_AUDIO_PLAY_ONCE);	
			break;
		case WARNING_TONE_IN_CALL:
			if(gcurrentprofile.extraTone.warningTone == 1)
				AudioPlayReq(TONE_WARNING2,DEVICE_AUDIO_PLAY_ONCE);	
			break;
		case ERROR_TONE_IN_CALL:
			if(gcurrentprofile.extraTone.errorTone == 1)
				AudioPlayReq(TONE_ERROR2,DEVICE_AUDIO_PLAY_ONCE);	
			break;
		/*add the process to the missed call sound warning by liuxl on 20090624*/			
		case MISSED_CALL_WARNING_TONE:
				AudioPlayReq(TONE_WARNING1,DEVICE_AUDIO_PLAY_ONCE);	
			break;

		}
	}
	else   //No active call, No on hold call    Deleted by jinzh:20070720
	{TRACE_EF(g_sw_MIDI,"特殊音--通话状态");
		
		switch(playtone)
		{
          
         //Shall play power off tone no matter in call or not 
         //See CANNON+ SCR 3573, CSD connection, long press END key won't will power off first, then drop call 
		   case POWER_OFF_TONE:
			{TRACE_EF(g_sw_MIDI,"特殊音--通话状态 POWER_OFF_TONE");
			   if(gcurrentprofile.toneSetup.powerOffTone != POWEROFF_SILENT)
				   AudioPlayReq(gcurrentprofile.toneSetup.powerOffTone,DEVICE_AUDIO_PLAY_ONCE);	
			   break;
		   }

			case SMS_IN_CALL_TONE:
			{TRACE_EF(g_sw_MIDI,"特殊音--通话状态 SMS_IN_CALL_TONE");
				if(gcurrentprofile.toneSetup.messageTone != MESSAGE_SILENT)
				{
				   SetPlayReqToneFlag(TRUE);
				   AudioPlayReq(	TONE_MT_SMS_IN_CALL,DEVICE_AUDIO_PLAY_ONCE);
				}   
				break;
			}

			case WARNING_TONE_IN_CALL:
			{TRACE_EF(g_sw_MIDI,"特殊音--通话状态 WARNING_TONE_IN_CALL");
			   if(gcurrentprofile.extraTone.warningTone == 1)
				   AudioPlayReq(TONE_WARNING2,DEVICE_AUDIO_PLAY_ONCE);			
				break;			
			}
			case ERROR_TONE_IN_CALL:
			{TRACE_EF(g_sw_MIDI,"特殊音--通话状态 ERROR_TONE_IN_CALL");
			   if(gcurrentprofile.extraTone.errorTone == 1)
				   AudioPlayReq(TONE_ERROR2,DEVICE_AUDIO_PLAY_ONCE);			
				break;			
			}		
			case CONNECT_TONE_IN_CALL:
			{TRACE_EF(g_sw_MIDI,"特殊音--通话状态 CONNECT_TONE_IN_CALL");
//CSD Brian added for AUX TONE and Connect tone, 2003/11/19
			   if(gcurrentprofile.extraTone.connectTone == 1)
				AudioPlayReq(TONE_CALL_CONNECT,DEVICE_AUDIO_PLAY_ONCE);			
				break;			
			}		
			case SUCCESS_TONE_IN_CALL:
			{TRACE_EF(g_sw_MIDI,"特殊音--通话状态 SUCCESS_TONE_IN_CALL");
				break;			
			}		
//CSD Brian added for battery indication, 2003/11/17
			case BATTERY_LOW_TONE:
			{TRACE_EF(g_sw_MIDI,"特殊音--通话状态 BATTERY_LOW_TONE");
			   if(gcurrentprofile.extraTone.warningTone == 1)
				   AudioPlayReq(TONE_WARNING2,DEVICE_AUDIO_PLAY_ONCE);			
				break;			
			}		
			case BATTERY_WARNING_TONE:
			{TRACE_EF(g_sw_MIDI,"特殊音--通话状态 BATTERY_WARNING_TONE");
			   if(gcurrentprofile.extraTone.warningTone == 1)
				   AudioPlayReq(TONE_WARNING2,DEVICE_AUDIO_PLAY_ONCE);			
				break;			
			}		
//CSD Brian added for AUX TONE and Connect tone, 2003/11/19
		   case AUX_TONE:
			{TRACE_EF(g_sw_MIDI,"特殊音--通话状态 AUX_TONE");
			   if(gcurrentprofile.extraTone.warningTone == 1)
				   AudioPlayReq(TONE_WARNING2,DEVICE_AUDIO_PLAY_ONCE);
			   break;
			}		
		   case CONNECT_TONE:
		   {TRACE_EF(g_sw_MIDI,"特殊音--通话状态 CONNECT_TONE");
#ifndef __MMI_CONNECT_NOTICE__
			   if(gcurrentprofile.extraTone.connectTone == 1)
#endif
				   AudioPlayReq(TONE_CALL_CONNECT,DEVICE_AUDIO_PLAY_ONCE);	
			   break;
			}   
		   case CALL_REMINDER_TONE:
		   case CCBS_TONE:
			{TRACE_EF(g_sw_MIDI,"特殊音--通话状态--CALL_REMINDER_TONE CCBS_TONE");
				AudioPlayReq(TONE_WARNING2,DEVICE_AUDIO_PLAY_ONCE);
			   break;
			}
		}
	}
}
/*****************************************************************************

	FUNCTION NAME		: void stopRequestedTone()

  	PURPOSE				: eneral API to all applications to stop tone.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************************/
void stopRequestedTone(ALL_TONE_ENUM playtone)
{
	TRACE_EF(g_sw_AUDIO, "playtone = %d", playtone);
	if(GetPlayReqToneFlag()==TRUE)
	{
		SetPlayReqToneFlag(FALSE);
		switch(playtone)
		{
		case ERROR_TONE:
			if(gcurrentprofile.extraTone.errorTone == 1)
			   AudioStopReq(TONE_ERROR1);
			break;

		case CONNECT_TONE:
			if(gcurrentprofile.extraTone.connectTone == 1)
				AudioStopReq(TONE_CALL_CONNECT);
			break;
#if !defined (__MMI_RESOURCE_SLIM_ON_128X160__)
		case CAMP_ON_TONE:
			if(gcurrentprofile.extraTone.campOnTone == 1)
				AudioStopReq( SOUND_3);
			break;
#else
		case CAMP_ON_TONE:
			if(gcurrentprofile.extraTone.campOnTone == 1)
				AudioStopReq( SOUND_1);
			break;
#endif
		case WARNING_TONE:
			if(gcurrentprofile.extraTone.warningTone == 1)
				AudioStopReq( TONE_WARNING1);
			break;

		case INCOMING_CALL_TONE:
			if(gactivatedprofile!=5)
				AudioStopReq( gcurrentprofile.toneSetup.ringTone);
			break;

		case ALARM_TONE:
		//CSD Brian modified to play alarm tone in all profiels, 2003/11/16
			//if(!IsSilentModeActivated())
				//AudioStopReq( gcurrentprofile.toneSetup.alarmTone);
				mdi_audio_stop_all();
			break;

		case POWER_ON_TONE:
			if(gcurrentprofile.toneSetup.powerOnTone != POWERON_SILENT)
				AudioStopReq( gcurrentprofile.toneSetup.powerOnTone);
			break;

		case POWER_OFF_TONE:
			if(gcurrentprofile.toneSetup.powerOffTone != POWEROFF_SILENT)
				AudioStopReq( gcurrentprofile.toneSetup.powerOffTone);
			break;

		//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
		case COVER_OPEN_TONE:
			if(gcurrentprofile.toneSetup.coverOpenTone != COVER_OPEN_SILENT)
				AudioStopReq( gcurrentprofile.toneSetup.coverOpenTone);
			break;

		case COVER_CLOSE_TONE:
			if(gcurrentprofile.toneSetup.coverCloseTone != COVER_CLOSE_SILENT)
				AudioStopReq( gcurrentprofile.toneSetup.coverCloseTone);
			break;
		#endif
		//CSD end

		case MESSAGE_TONE:
			if(gcurrentprofile.toneSetup.messageTone != MESSAGE_SILENT)
				AudioStopReq( gcurrentprofile.toneSetup.messageTone);
			break;

		case SUCCESS_TONE:
			break;

		case SAVE_TONE:
			if(gactivatedprofile!=5)
				AudioStopReq( TONE_GENERAL_BEEP );
			break;

		case EMPTY_LIST_TONE:
			if(gcurrentprofile.extraTone.warningTone == 1)
				AudioStopReq( TONE_WARNING1 );
			break;

		case GENERAL_TONE:
			if(gactivatedprofile!=5)
				AudioStopReq( TONE_GENERAL_BEEP );
			break;

		case SMS_IN_CALL_TONE:
				AudioStopReq( TONE_MT_SMS_IN_CALL );
			break;
//CSD Brian added for battery indication, 2003/11/17
		case BATTERY_LOW_TONE:
				AudioStopReq(TONE_BATTERY_LOW);
			break;
		case BATTERY_WARNING_TONE:
				AudioStopReq(TONE_BATTERY_WARNING);
			break;
			}
	}
}
/*****************************************************************************

	FUNCTION NAME		: void SetPlayReqToneFlag(pBOOL)

  	PURPOSE				: 

	INPUT PARAMETERS	: pBOOL

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************************/
void SetPlayReqToneFlag(pBOOL flag)
{
	gPlayReqToneFlag = flag;
}


/*****************************************************************************

	FUNCTION NAME		: void GetPlayReqToneFlag()

  	PURPOSE				: 

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************************/
pBOOL GetPlayReqToneFlag(void)
{
	return gPlayReqToneFlag;
}

/*****************************************************************************

	FUNCTION NAME		: void UpdateDeleteDownloadedRingTone()

  	PURPOSE				: set default tone when selected downloaded Ring tone is deleted.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************************/
void UpdateDeleteDownloadedRingTone(U16 ToneId)
{
	if(gcurrentprofile.toneSetup.ringTone==ToneId)
		SetRingToneID(gstartMidiId);
}


/*****************************************************************************

	FUNCTION NAME		: void UpdateDeleteExtMldRingTone()

  	PURPOSE				: set default tone when selected extented Melody Ring tone is deleted.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************************/
#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
void UpdateDeleteExtMldToneSetup(U16 ToneId)
{
     U8 i, dirtyFlag=0;
     S16 error=0;
	
	 /*Update alarm ring tone list*/
#if defined(__MMI_ALM_AUDIO_OPTIONS__) && defined(__MMI_ALM_CUSTOMIZE_TONE__)
	mmi_alm_tone_setup_delete_ext_melody(ToneId);
#endif	

     for( i=0; i< MAX_ELEMENTS; i++ )
     {
        if(gprofiles[i].toneSetup.ringTone==ToneId)
        {
             gprofiles[i].toneSetup.ringTone = gstartMidiId;
             dirtyFlag = 1;
        }
        if(gprofiles[i].toneSetup.alarmTone==ToneId)
        {
             gprofiles[i].toneSetup.alarmTone = gstartMidiId+1;
             dirtyFlag = 1;
        }
#ifdef __MMI_DUAL_PROFILE_SUPPORT__  //Add by Liuxl  for CS2324C project on 20090603
	 if(gprofiles[i].toneSetup.card2_ringTone==ToneId)
        {
             gprofiles[i].toneSetup.card2_ringTone = gstartMidiId;
             dirtyFlag = 1;
        }
#endif	
        
     }

     if(dirtyFlag == 1 )
	if(WriteRecord (NVRAM_EF_PROFILES_LID, 1, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
	 PRINT_INFORMATION (("## ERROR: SetRingToneID: WriteRecord "
									"to NVRAM failed (ecode%d)\n", (int) error));
	 return;
	}

     if( gactivatedprofile == gprofileId)
     {
 	memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
     }
}
#endif

/*****************************************************************************

	FUNCTION NAME		: void UpdateDeleteDownloadedAlarmTone()

  	PURPOSE				: set default tone when selected downloaded alarm tone is deleted.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************************/
void UpdateDeleteDownloadedAlarmTone(U16 ToneId)
{
	if(gcurrentprofile.toneSetup.alarmTone==ToneId)
	{
		TONE_SETUP tonesetup;
		tonesetup.alarmTone=gstartMidiId + 1;
		SetAlarmToneID(tonesetup.alarmTone);
	}
}

/*****************************************************************************

	FUNCTION NAME		: void SetCurrentProfileAsActivatedProfile(void)

  	PURPOSE				: Sets Current Profile As Activated Profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

******************************************************************************/
void SetCurrentProfileAsActivatedProfile(void)
{
	gprofileId = gactivatedprofile;
}

/*****************************************************************************
	FUNCTION NAME		: BOOL IsHeadsetModeActivated(void)
  	PURPOSE				: Tells if headset mode is activated
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: BOOL
 
******************************************************************************/
BOOL IsHeadsetModeActivated(void )
{
	if(gactivatedprofile == MMI_PROFILE_HEADSET)
		return TRUE;
	else
		return FALSE;
}

/*****************************************************************************

	FUNCTION NAME		: pBOOL IsSilentModeActivated(void)

  	PURPOSE				: Tells if silent mode is activated

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

 

******************************************************************************/
pBOOL IsSilentModeActivated(void)
{
	if(gactivatedprofile == MMI_PROFILE_SILENT)
		return TRUE;
	else
		return FALSE;
}

/*****************************************************************************

	FUNCTION NAME		: BOOL IsMeetingModeActivated(void)

  	PURPOSE				: Tells if meeting mode is activated

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: BOOL

 

******************************************************************************/
BOOL IsMeetingModeActivated( void )
{
	if( gactivatedprofile == MMI_PROFILE_MEETING)
		return TRUE;
	else
		return FALSE;
}

 
BOOL AtSetProfileTone( U8 profileID, PROFILE_TONE_TYPE_ENUM tone_type, U8 tone_idx )
{
      S16 error=0;
      U16 j=1;
      if( profileID >= MAX_PROFILE_NUM || tone_type >= TONE_MAX_TONE_TYPE || tone_idx > 10 ) 
         return MMI_FALSE;
      
      switch(tone_type )
         {
            case TONE_POWERONTONE: 
               if( tone_idx == 0 )
                  gprofiles[profileID].toneSetup.powerOnTone = POWERON_SILENT;
               else
                  gprofiles[profileID].toneSetup.powerOnTone = gstartGeneralToneId+tone_idx-1;
               break;
               
             default:
               return MMI_FALSE;
         }
      
      if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE, &error) < NVRAM_PROFILES_RECORD_SIZE)
      {
      	  PRINT_INFORMATION (("## ERROR: SetPowerOnToneType: WriteRecord "
      									"to NVRAM failed (ecode%d)\n", (int) error));
      	  return MMI_FALSE;
      }
      	
      if( gactivatedprofile == gprofileId)
      {
      	memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
      }

	return MMI_TRUE;

}
//CSD end
/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_check_set_mute
 * DESCRIPTION
 *  
 * PARAMETERS
 *
 * RETURNS
 *  
 *****************************************************************************/
BOOL mmi_profiles_check_set_mute()
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    /*Audio Player*/
#ifdef __MMI_AUDIO_PLAYER__
     if(mmi_audply_is_playing())
     {
        return MMI_FALSE;
     }
#endif

    /*Java Player*/
#ifdef __J2ME__
#if 0//JiaShuo Delete for J2ME
    if(jma_audio_player_is_really_active())
    {
        return MMI_FALSE;
    }
#endif
#endif

    /*FM Radio*/
#ifdef __MMI_FM_RADIO__
    if(mmi_fmrdo_is_power_on())
    {
        return MMI_FALSE;
    }
#endif

    /*Sound Recorder*/
#ifdef __MMI_SOUND_RECORDER__
    if(mmi_sndrec_record_is_on())
    {
        return MMI_FALSE;
    }
#endif

    return MMI_TRUE;

}

/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_headset_activate_profile
 * DESCRIPTION
 *  Activating Headset profile
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_headset_activate_profile(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 error = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
 

    gprofileheadset = 1;
#ifdef __MMI_BT_PROFILE__
    if (gactivatedprofile != MMI_PROFILE_SILENT && gactivatedprofile != MMI_PROFILE_HEADSET &&
        gactivatedprofile != MMI_PROFILE_BT)
#else /* __MMI_BT_PROFILE__ */ 
    if (gactivatedprofile != MMI_PROFILE_SILENT && gactivatedprofile != MMI_PROFILE_HEADSET)
#endif /* __MMI_BT_PROFILE__ */ 
        glastactivatedprofile = gactivatedprofile;
    gactivatedprofile = MMI_PROFILE_HEADSET;
    memcpy(&gcurrentprofile, &gprofiles[gactivatedprofile], sizeof(PROFILE));
    WriteValue(NVRAM_PROFILES_LASTACTIVATED, &glastactivatedprofile, DS_BYTE, &error);
    change_font();
    
#if defined(__MMI_IMPS__) && defined(IMPS_WV13_ENHANCEMENT)
/* under construction !*/
#endif    

    /* SetRingerVolume(GetRingVolumeLevel()); */
    mdi_audio_set_volume_no_apply(MDI_VOLUME_MEDIA, GetRingVolumeLevel());

    SetKeyPadVolume(GetKeypadVolumeLevel());
    mdi_audio_set_mic_volume();
    if (gloudSpeaker == 1)
    {
        gloudSpeaker = 0;
        SetLSpkNowOff(TRUE);
    }
}


#ifdef __MMI_BT_PROFILE__


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_allow_popup
 * DESCRIPTION
 *  check is it allowed to popup
 * PARAMETERS
 *  void
 * RETURNS
 * BOOL
 *****************************************************************************/
BOOL mmi_profiles_bt_allow_popup(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return g_mmi_profiles_bt_in_call;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_connect_sco
 * DESCRIPTION
 *  connect sco link
 * PARAMETERS
 *  callback        [IN]        )(void)
 * RETURNS
 * BOOL
 *****************************************************************************/
BOOL mmi_profiles_bt_connect_sco(void (*callback) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    BOOL connect = FALSE;
    S32 connect_id;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_HFP_SUPPORT__

  kal_prompt_trace(0,"mmi_profiles_bt_connect_sco: disalbe SCO=[%d], BT Inquery=[%d]\n", g_mmi_profiles_bt_disalbe_sco, g_mmi_profiles_bt_inquerying);
  
    if(mmi_profiles_bt_is_sco_connect_allowed() &&
       gactivatedprofile == MMI_PROFILE_BT )
    {
    	if( mmi_bt_is_hs_connected() )
    	{
    		connect_id = mmi_bt_get_active_signal_connection_id( MMI_BT_HFG_PROFILE_UUID );
    		ASSERT( connect_id >= 0 );
    		connect = mdi_audio_bt_open_stream( BT_HFP, (U16)connect_id );
    		if( connect )
    		{
    			mmi_profiles_bt_connect_sco_app_callback = callback;
    		}
        #ifdef __MMI_BT_AUDIO_VIA_SCO__
            /* Already Connected, start a timeout timer */
            else if(g_mmi_profiles_bt_sco_timeout && 
                    mmi_profiles_bt_is_connected(MMI_PROFILES_BT_HFP_SCO))
            {
                StartTimer(BT_SCO_CONNECT_TIMER, BT_SCO_CONNECT_TIMEOUT_DURATION, mmi_profiles_bt_sco_timeout);
            }
        #endif /* __MMI_BT_AUDIO_VIA_SCO__ */
    	}

    	g_mmi_profiles_bt_app_in_use = TRUE;
    }
#endif /* __MMI_HFP_SUPPORT__ */ 
    return connect;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_disconnect_sco
 * DESCRIPTION
 *  disconnect sco link
 * PARAMETERS
 *  void
 * RETURNS
 * BOOL
 *****************************************************************************/
BOOL mmi_profiles_bt_disconnect_sco(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    BOOL disconnect = FALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_HFP_SUPPORT__
//    MMI_TRACE(MMI_MEDIA_TRC_G2_APP,MMI_USER_PROF_BT_DISCONNECT_SCO,g_mmi_profiles_bt_in_call,g_mmi_profiles_bt_always_use_in_call,mmi_bt_is_aud2hf());

    if (!g_mmi_profiles_bt_in_call || !(g_mmi_profiles_bt_always_use_in_call || mmi_bt_is_aud2hf()))
    {
        disconnect = mdi_audio_bt_is_stream_open(BT_HFP);
        mdi_audio_bt_close_stream(BT_HFP);
        mmi_profiles_bt_connect_sco_app_callback = NULL;
    }
    g_mmi_profiles_bt_app_in_use = FALSE;
#endif /* __MMI_HFP_SUPPORT__ */ 
    return disconnect;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_deactivate_cnf_hdlr
 * DESCRIPTION
 *  the cnf handler for bt a2dp deactivate req
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_bt_deactivate_cnf_hdlr(void)
{
    if (g_mmi_profiles_bt_reset_pending != 0)
    {
        g_mmi_profiles_bt_reset_pending &= 0x0D;
        if (g_mmi_profiles_bt_reset_pending == 0)
        {
            mmi_bt_panic_ind_post_process();
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_reset
 * DESCRIPTION
 *  to reset bt profile
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
 extern void av_bt_disconnect_cnf(void *msg);

void mmi_profiles_bt_reset(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
  
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if defined(__MMI_A2DP_SUPPORT__) && defined(__MMI_AUDIO_PLAYER__)	
     void *arg=NULL;
#endif
    mmi_trace(1,"MMI_USER_PROF_BT_RESET g_mmi_profiles_bt_reset_pending =0x%x",g_mmi_profiles_bt_reset_pending);

#if defined(__MMI_A2DP_SUPPORT__) && defined(__MMI_AUDIO_PLAYER__)

    mmi_audply_bt_power_off_callback(TRUE);

 
    mmi_bt_a2dp_deactivate_req();
	
    if (mmi_bt_a2dp_query_state() != MMI_A2DP_STATE_IDLE)
    {
        g_mmi_profiles_bt_reset_pending |= 2;
    	mmi_trace(1,"MMI_USER_PROF_BT_RESET2 g_mmi_profiles_bt_reset_pending =0x%x",g_mmi_profiles_bt_reset_pending);
		
 	    av_bt_disconnect_cnf(arg );	
    	    MMI_A2DP_STATE_TRANS(MMI_A2DP_STATE_IDLE);
            mmi_bth_deactivate_cnf_hdler(MMI_BTH_ADVANCED_AUDIO_PROFILE_UUID);
	     g_mmi_profiles_bt_reset_pending &= 0x0D;
		
    }
#endif
#ifdef __MMI_HFP_SUPPORT__
    if (mmi_profiles_bt_is_connected(MMI_PROFILES_BT_HFP_SCO))
    {
        g_mmi_profiles_bt_reset_pending |= 1;
        mmi_profiles_bt_disconnect_req(MMI_PROFILES_BT_HFP_SCO);
    }
    mmi_profiles_bt_connect_sco_app_callback = NULL;
#endif /* __MMI_HFP_SUPPORT__ */ 
    if (gactivatedprofile == MMI_PROFILE_BT)
    {
        if (mmi_gpio_is_earphone_plug_in())
        {
            SendAudioModeRequest(AUD_MODE_HEADSET);
            SetModeSpeechVolume();
            mmi_profiles_headset_activate_profile();
      #if defined ( __MMI_DUAL_SIM_MASTER__ )&& defined ( __MMI_DUAL_SIM_DUAL_CALL__ )
/* under construction !*/
      #endif
        }
        else
        {
            mmi_profiles_bt_deactivate_profile();
        }
    }
    mmi_trace(1,"MMI_USER_PROF_BT_RESET3 g_mmi_profiles_bt_reset_pending =0x%x",g_mmi_profiles_bt_reset_pending);
    if (g_mmi_profiles_bt_reset_pending == 0)
    {
        mmi_bt_panic_ind_post_process();
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_is_activated
 * DESCRIPTION
 *  to check if bt profile is activated
 * PARAMETERS
 *  void
 * RETURNS
 * BOOL
 *****************************************************************************/
BOOL mmi_profiles_bt_is_activated(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	mmi_trace(g_sw_BT,"gactivatedprofile = %d", gactivatedprofile);
    return (gactivatedprofile == MMI_PROFILE_BT);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_open_stream_callback
 * DESCRIPTION
 *  a callback function for open bt stream cnf
 * PARAMETERS
 *  profile     [IN]        
 *  result      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_bt_open_stream_callback(U8 profile, mdi_result result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 res;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//    MMI_TRACE(MMI_MEDIA_TRC_G2_APP,MMI_USER_PROF_BT_OPEN_STREAM_CALLBACK,profile, result);

    mmi_trace(1, "mmi_profiles_bt_open_stream_callback, profile = %d , result = %d     ",profile,result); 

    if (result == MDI_AUDIO_SUCCESS)
    {
        res = MMI_PROFILES_BT_OK;
    }
    else if (result == MDI_AUDIO_BUSY)
    {
        res = MMI_PROFILES_BT_BUSY;
    }
    else
    {
        res = MMI_PROFILES_BT_FAIL;
    }
    switch (profile)
    {
    #ifdef __MMI_HFP_SUPPORT__
        case BT_HFP:
            mmi_profiles_bt_connect_callback(MMI_PROFILES_BT_HFP_SCO, MMI_PROFILES_BT_RSP, res);
            if( result == MDI_AUDIO_SUCCESS )
            {
				mmi_bth_sco_connect_ind_hdler(mmi_bt_get_active_signal_connection_id(MMI_BT_HFG_PROFILE_UUID));

    #ifdef __MMI_BT_AUDIO_VIA_SCO__
	         mmi_trace(1,"mmi_profiles_bt_open_stream_callback  BT_START_SCO_TIMER  g_mmi_profiles_bt_sco_timeout = %d     \n  ",g_mmi_profiles_bt_sco_timeout);
    		    if(g_mmi_profiles_bt_sco_timeout)
    		    {
                    StopTimer(BT_SCO_CONNECT_TIMER);
                    StartTimer(BT_SCO_CONNECT_TIMER, BT_SCO_CONNECT_TIMEOUT_DURATION, mmi_profiles_bt_sco_timeout);
                }
    #endif /* #ifdef __MMI_BT_AUDIO_VIA_SCO__ */
			}
			else
			{
				mmi_bth_sco_connect_cnf_hdler(1, mmi_bt_get_active_signal_connection_id(MMI_BT_HFG_PROFILE_UUID));
			}
            if (mmi_profiles_bt_connect_sco_app_callback != NULL)
            {
                mmi_profiles_bt_connect_sco_app_callback();
            }
            break;
    #endif /* __MMI_HFP_SUPPORT__ */ 
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_close_stream_callback
 * DESCRIPTION
 *  a callback function for close bt stream cnf
 * PARAMETERS
 *  profile     [IN]        
 *  result      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_bt_close_stream_callback(U8 profile, mdi_result result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
#if defined(__MMI_BT_AUDIO_VIA_SCO__) || defined(__MMI_BT_FM_VIA_SCO__)
    void (*inquery_callback) (void);
#endif    
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//    MMI_TRACE(MMI_MEDIA_TRC_G2_APP,MMI_USER_PROF_BT_CLOSE_STREAM_CALLBACK,profile, result);
mmi_trace(1, "mmi_profiles_bt_close_stream_callback, profile = %d , result = %d     ",profile,result); 

    switch (profile)
    {
    #ifdef __MMI_HFP_SUPPORT__
        case BT_HFP:
            mmi_profiles_bt_disconnect_callback(MMI_PROFILES_BT_HFP_SCO, MMI_PROFILES_BT_RSP, MMI_PROFILES_BT_OK);
    		mmi_bth_sco_disconnect_ind_hdler(mmi_bt_get_active_signal_connection_id(MMI_BT_HFG_PROFILE_UUID));

    #if defined(__MMI_BT_AUDIO_VIA_SCO__) || defined(__MMI_BT_FM_VIA_SCO__)
            /* Callback inquery function because the SCO disconnect is called by BT inquery */
            if(g_mmi_profiles_bt_inquery_callback)
            {
                inquery_callback = g_mmi_profiles_bt_inquery_callback;
                g_mmi_profiles_bt_inquery_callback = NULL;                
                inquery_callback();
            }
    #endif

            break;
    #endif /* __MMI_HFP_SUPPORT__ */ 
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_is_connected
 * DESCRIPTION
 *  to check if bt is connected
 * PARAMETERS
 *  connect_type        [IN]        
 * RETURNS
 * BOOL
 *****************************************************************************/
BOOL mmi_profiles_bt_is_connected(U8 connect_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    BOOL bt_connect = FALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (connect_type)
    {
    #ifdef __MMI_HFP_SUPPORT__
        case MMI_PROFILES_BT_HFP_ACL:
            bt_connect = (BOOL) mmi_bt_is_hs_connected();
            break;
        case MMI_PROFILES_BT_HFP_SCO:
            bt_connect = mdi_audio_bt_is_stream_open(BT_HFP);
            break;
    #endif /* __MMI_HFP_SUPPORT__ */ 
        default:
            ASSERT(0);
    }
	mmi_trace(g_sw_BT, "mmi_profiles_bt_is_connected::connect_type=%d, bt_connect=%d", connect_type, bt_connect);

    return bt_connect;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_is_sco_connect_allowed
 * DESCRIPTION
 *  check if bt sco connect request is allowed
 * PARAMETERS
 *  void
 * RETURNS
 *  TURE if allowed
 *****************************************************************************/
BOOL mmi_profiles_bt_is_sco_connect_allowed(void)
{
    /* Can not connect SCO while FM is recording */
#ifdef __MMI_BT_FM_VIA_SCO__
    if((mdi_audio_get_audio_mode() == MDI_AUDIO_RECORD_FM_RADIO) &&
       (mdi_audio_get_state()== MDI_AUDIO_RECORD))
    {
        return FALSE;
    }
#endif

    return (!g_mmi_profiles_bt_disalbe_sco && !g_mmi_profiles_bt_inquerying);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_connect_req
 * DESCRIPTION
 *  bt connect request
 * PARAMETERS
 *  connect_type        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_bt_connect_req(U8 connect_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 connect_id;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

  kal_prompt_trace(g_sw_BT, "mmi_profiles_bt_connect_req: connect_type =[%d], disalbeSCO=[%d], BT Inquery=[%d]\n", 
                        connect_type, g_mmi_profiles_bt_disalbe_sco, g_mmi_profiles_bt_inquerying);

    switch (connect_type)
    {
    #ifdef __MMI_HFP_SUPPORT__
        case MMI_PROFILES_BT_HFP_SCO:
            if(mmi_profiles_bt_is_sco_connect_allowed())
            {
                connect_id = mmi_bt_get_active_signal_connection_id(MMI_BT_HFG_PROFILE_UUID);
                ASSERT(connect_id >= 0);
                mdi_audio_bt_open_stream(BT_HFP, (U16) connect_id);
                if (gactivatedprofile != MMI_PROFILE_BT)
                {
                    mmi_profiles_bt_activate_profile();
                }                
            }
            break;
    #endif /* __MMI_HFP_SUPPORT__ */ 
        default:
            ASSERT(0);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_disconnect_req
 * DESCRIPTION
 *  bt disconnect request
 * PARAMETERS
 *  connect_type        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
#ifdef __MMI_FM_RADIO__
extern BOOL mmi_fmrdo_state_is_idle(void);
#endif
void mmi_profiles_bt_disconnect_req(U8 connect_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/


  mmi_trace(g_sw_BT,"mmi_profiles_bt_disconnect_req::connect_type=%d", connect_type);
    switch (connect_type)
    {
    #ifdef __MMI_HFP_SUPPORT__
        case MMI_PROFILES_BT_HFP_SCO:
            mdi_audio_bt_close_stream(BT_HFP);
            mmi_profiles_bt_connect_sco_app_callback = NULL;
	   // for fix14749		

            if (mmi_gpio_is_earphone_plug_in()
#ifdef __MMI_FM_RADIO__
                && (mmi_fmrdo_state_is_idle())
#endif
                )
            {
                SendAudioModeRequest(AUD_MODE_HEADSET);
                SetModeSpeechVolume();
                mmi_profiles_headset_activate_profile();
            }
			
            break;
    #endif /* __MMI_HFP_SUPPORT__ */ 
        default:
            ASSERT(0);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_call_start_callback
 * DESCRIPTION
 *  the call back function for call start
 * PARAMETERS
 *  void
 *  BOOL(?)     [IN]        Turn on
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_bt_call_start_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    BOOL speech_on = mdi_audio_is_speech_mode();
    speech_on = speech_on;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_BT," In function mmi_profiles_bt_call_start_callback!! gactivatedprofile=%d",gactivatedprofile);
    if (!g_mmi_profiles_bt_in_call)
    {
//        if (gactivatedprofile != MMI_PROFILE_BT)  //Zhou siyou added it. 2009.6.28
//        {
//            mmi_profiles_bt_activate_profile();
//		mmi_trace(g_sw_BT, "mmi_profiles_bt_call_start_callback::mmi_profiles_bt_activate_profile");
//        } 
        if (gactivatedprofile == MMI_PROFILE_BT)
        {
        #ifdef __MMI_HFP_SUPPORT__
            if (mmi_profiles_bt_is_connected(MMI_PROFILES_BT_HFP_ACL))
            {
                if (g_mmi_profiles_bt_always_use_in_call || mmi_bt_is_aud2hf())
                {
                    mmi_profiles_bt_connect_req(MMI_PROFILES_BT_HFP_SCO);
                #ifdef __MMI_BT_AUDIO_VIA_SCO__
                    /* Stop SCO timeout timer during call */
                    StopTimer(BT_SCO_CONNECT_TIMER);
                #endif /* #ifdef __MMI_BT_AUDIO_VIA_SCO__ */
                }
            }
        #endif /* __MMI_HFP_SUPPORT__ */  
        }
        g_mmi_profiles_bt_in_call = TRUE;
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_call_end_callback
 * DESCRIPTION
 *  the call back function for call end
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_bt_call_end_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
//    BOOL speech_on = mdi_audio_is_speech_mode();

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//    MMI_TRACE(MMI_MEDIA_TRC_G2_APP,MMI_USER_PROF_BT_CALL_END_CALLBACK,g_mmi_profiles_bt_in_call,speech_on);

    if (g_mmi_profiles_bt_in_call)
    {
        if (gactivatedprofile == MMI_PROFILE_BT)
        {
        #ifdef __MMI_HFP_SUPPORT__
            if (mmi_profiles_bt_is_connected(MMI_PROFILES_BT_HFP_ACL))
            {
                #ifdef __MMI_BT_AUDIO_VIA_SCO__
                    if(mmi_bt_is_aud2hf())
                    {
                        /* Disconnect later if support audio via BT SCO */
                        if(g_mmi_profiles_bt_sco_timeout)
                        {
                            StartTimer(BT_SCO_CONNECT_TIMER, BT_SCO_CONNECT_TIMEOUT_DURATION, mmi_profiles_bt_sco_timeout);
                        }
                    }
                    else
                    {
                        mmi_profiles_bt_disconnect_req(MMI_PROFILES_BT_HFP_SCO);                    
                    }
                #else
                    mmi_profiles_bt_disconnect_req(MMI_PROFILES_BT_HFP_SCO);
                #endif
            }
        #endif /* __MMI_HFP_SUPPORT__ */ 
        }
        g_mmi_profiles_bt_in_call = FALSE;
    #ifdef __MMI_HFP_SUPPORT__
        g_mmi_profiles_bt_always_use_in_call = FALSE;
    #endif 
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_connect_callback
 * DESCRIPTION
 *  the call back function for bt connect indication
 * PARAMETERS
 *  connect_type        [IN]        
 *  callback_type       [IN]        
 *  result              [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
 
extern pBOOL GetRingingFlag(void);
extern 	MMI_BOOL mmi_bt_start_hf_ring_req(void);
void mmi_profiles_bt_connect_callback(U8 connect_type, U8 callback_type, U16 result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
mmi_trace(g_sw_BT,"mmi_profiles_bt_connect_callback::connect_type=%d callback_type=%d result = %d", connect_type, callback_type, result);
    switch (connect_type)
    {
    #ifdef __MMI_HFP_SUPPORT__
        case MMI_PROFILES_BT_HFP_ACL:
            if (callback_type == MMI_PROFILES_BT_IND || callback_type == MMI_PROFILES_BT_RSP)
            {
                if (result == MMI_PROFILES_BT_OK)
                {
                    if (gactivatedprofile != MMI_PROFILE_BT)
                    {
	                    mmi_trace(g_sw_BT,"MMI_PROFILES_BT_HFP_ACL::mmi_profiles_bt_activate_profile");
                        mmi_profiles_bt_activate_profile();
                    }
                    
            #ifdef __MMI_BT_AUDIO_VIA_SCO__
                    /* Connect SCO according to HFP audio path setting */
                    if (mmi_bt_is_aud2hf())
            #else /*__MMI_BT_AUDIO_VIA_SCO__*/
                #ifdef __MMI_BT_FM_VIA_SCO__
                    /* Need to check FM radio if FM via SCO on  */
                    if ((g_mmi_profiles_bt_in_call || mdi_audio_is_playing(MDI_AUDIO_PLAY_FM_RADIO)) &&
                        (g_mmi_profiles_bt_always_use_in_call || mmi_bt_is_aud2hf()))
                #else
                    if (g_mmi_profiles_bt_in_call && (g_mmi_profiles_bt_always_use_in_call || mmi_bt_is_aud2hf()))
			{
			  mmi_trace(g_sw_BT,"mmi_profiles_bt_connect_callback ::111111");
                        mmi_profiles_bt_connect_req(MMI_PROFILES_BT_HFP_SCO);
                     }
			else if(GetRingingFlag())	// zhou siyou add 20100604 ,during incoming ring Hfp connect indication ,send the ring to headset
					mmi_bt_start_hf_ring_req();
			//gui_start_timer(1000,mdi_HfpConInd_sendRing_timeout);
			///mmi_bt_start_hf_ring_req();
                #endif
            #endif /*__MMI_BT_AUDIO_VIA_SCO__*/
                    {//zhou siyou 20100604
                 			  mmi_trace(g_sw_BT,"mmi_profiles_bt_connect_callback ::2222222");
                    
                       mmi_profiles_bt_connect_req(MMI_PROFILES_BT_HFP_SCO);
                    }
                }
            }
            break;
        case MMI_PROFILES_BT_HFP_SCO:
            if (callback_type == MMI_PROFILES_BT_IND || callback_type == MMI_PROFILES_BT_RSP)
            {
                if (result == MMI_PROFILES_BT_OK)
                {
                    /* add a condition check for SCO, if it's not allowed, then disconnect it */
            #if !defined(__MMI_BT_AUDIO_VIA_SCO__)
                #ifdef __MMI_BT_FM_VIA_SCO__
                    if( !g_mmi_profiles_bt_app_in_use && !g_mmi_profiles_bt_in_call && !mmi_bt_is_doing_far_end_loopback_test()
                        && !mdi_audio_is_playing(MDI_AUDIO_PLAY_FM_RADIO) )
                #else
                    if( !g_mmi_profiles_bt_app_in_use && !g_mmi_profiles_bt_in_call && !mmi_bt_is_doing_far_end_loopback_test())
                #endif            
                    {
                        mmi_profiles_bt_disconnect_req( MMI_PROFILES_BT_HFP_SCO );
                        return;
                    }
            #endif /* #if !defined(__MMI_BT_AUDIO_VIA_SCO__) */
                
                    if (gactivatedprofile != MMI_PROFILE_BT)
                    {
                    	                    mmi_trace(g_sw_BT,"MMI_PROFILES_BT_HFP_SCO::mmi_profiles_bt_activate_profile");

                        mmi_profiles_bt_activate_profile();
                    }
                    if (g_mmi_profiles_bt_in_call)
                    {
                        g_mmi_profiles_bt_always_use_in_call = TRUE;
                    }
                }
            }
            break;
    #endif /* __MMI_HFP_SUPPORT__ */ 
        default:
            ASSERT(0);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_disconnect_callback
 * DESCRIPTION
 *  the call back function for bt disconnect indication
 * PARAMETERS
 *  connect_type        [IN]        
 *  callback_type       [IN]        
 *  result              [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_bt_disconnect_callback(U8 connect_type, U8 callback_type, U16 result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//    MMI_TRACE(MMI_MEDIA_TRC_G2_APP,MMI_USER_PROF_BT_DISCONNECT_CALLBACK,connect_type, callback_type, result);
           mmi_trace(g_sw_BT,"mmi_profiles_bt_disconnect_callback::connect_type=%d, callback_type=%d", connect_type, callback_type);
    switch (connect_type)
    {
    #ifdef __MMI_HFP_SUPPORT__
        case MMI_PROFILES_BT_HFP_ACL:
            if (callback_type == MMI_PROFILES_BT_IND || callback_type == MMI_PROFILES_BT_RSP)
            {
                mmi_profiles_bt_disconnect_req(MMI_PROFILES_BT_HFP_SCO);
	           mmi_trace(g_sw_BT,"mmi_profiles_bt_disconnect_callback::gactivatedprofile=%d", gactivatedprofile);
                if (gactivatedprofile == MMI_PROFILE_BT)
                {
                    if (mmi_gpio_is_earphone_plug_in()
#ifdef __MMI_FM_RADIO__
                        && mmi_fmrdo_state_is_idle()
#endif
                        )
                    {
                        SendAudioModeRequest(AUD_MODE_HEADSET);
                    #if defined ( __MMI_DUAL_SIM_MASTER__ )&& defined ( __MMI_DUAL_SIM_DUAL_CALL__ )
/* under construction !*/
                    #endif
                        mmi_profiles_headset_activate_profile();
                    }
                    else
                    {
                        mmi_profiles_bt_deactivate_profile();
                    }
                }
            }
            /* Change audio path back to phone before disconnect HFP ACL link */
            else if (callback_type == MMI_PROFILES_BT_REQ)
            {
                mdi_audio_bt_hfp_turn_off();
            }
           /* Resume audio background play if HFP ACL disconnected while BT inquirying */
            if(g_mmi_profiles_bt_inquerying)
            {
                mdi_audio_resume_background_play();
            }            
            break;
        case MMI_PROFILES_BT_HFP_SCO:
            if (callback_type == MMI_PROFILES_BT_IND || callback_type == MMI_PROFILES_BT_RSP)
            {
                if (g_mmi_profiles_bt_reset_pending != 0)
                {
                    g_mmi_profiles_bt_reset_pending &= 0x0E;
                    if (g_mmi_profiles_bt_reset_pending == 0)
                    {
                        mmi_bt_panic_ind_post_process();
                    }
                }
            }
            break;
    #endif /* __MMI_HFP_SUPPORT__ */ 
        default:
            ASSERT(0);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_activate_profile
 * DESCRIPTION
 *  activate BT profile
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_bt_activate_profile(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 error = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//    MMI_TRACE(MMI_MEDIA_TRC_G2_APP,MMI_USER_PROF_BT_ACTIVATE_PROFILE,gactivatedprofile, glastactivatedprofile);

	gprofileheadset = 0;
    if (gactivatedprofile != MMI_PROFILE_SILENT && gactivatedprofile != MMI_PROFILE_HEADSET &&
        gactivatedprofile != MMI_PROFILE_BT)
    {
        glastactivatedprofile = gactivatedprofile;
    }
    gactivatedprofile = MMI_PROFILE_BT;
    memcpy(&gcurrentprofile, &gprofiles[gactivatedprofile], sizeof(PROFILE));
    WriteValue(NVRAM_PROFILES_LASTACTIVATED, &glastactivatedprofile, DS_BYTE, &error);
    change_font();

#if defined(__MMI_IMPS__) && defined(IMPS_WV13_ENHANCEMENT)
/* under construction !*/
#endif   

    //mdi_audio_set_volume_no_apply(MDI_VOLUME_MEDIA, GetRingVolumeLevel());
   // SetKeyPadVolume(GetKeypadVolumeLevel());
  //  mdi_audio_set_mic_volume();
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_deactivate_profile
 * DESCRIPTION
 *  deactivate BT profile
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_bt_deactivate_profile(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 error = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//    MMI_TRACE(MMI_MEDIA_TRC_G2_APP,MMI_USER_PROF_BT_DEACTIVATE_PROFILE,gactivatedprofile, glastactivatedprofile);
mmi_trace(g_sw_BT, "mmi_profiles_bt_deactivate_profile::gactivatedprofile=%d", gactivatedprofile);
    ReadValue(NVRAM_PROFILES_ACTIVATED, &gactivatedprofile, DS_BYTE, &error);

	mmi_trace(g_sw_BT, "mmi_profiles_bt_deactivate_profile::readvalue gactivatedprofile=%d, gcurrentprofile = %d", gactivatedprofile, gcurrentprofile);

    memcpy(&gcurrentprofile, &gprofiles[gactivatedprofile], sizeof(PROFILE));
    change_font();

#if defined(__MMI_IMPS__) && defined(IMPS_WV13_ENHANCEMENT)
/* under construction !*/
#endif   

    if(gactivatedprofile == MMI_PROFILE_SILENT)
    {
        if(mmi_profiles_check_set_mute())
        {
            mdi_audio_set_mute(MDI_VOLUME_MEDIA,TRUE);
        }
    }

//    mdi_audio_set_volume_no_apply(MDI_VOLUME_MEDIA, GetRingVolumeLevel());
//    SetKeyPadVolume(GetKeypadVolumeLevel());
//    mdi_audio_set_mic_volume();
}
#if defined(__MMI_BT_AUDIO_VIA_SCO__) || defined(__MMI_BT_FM_VIA_SCO__)
/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_disable_sco_timeout
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_bt_disable_sco_timeout(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(g_sw_BT, "mmi_profiles_bt_disable_sco_timeout:");
    StopTimer(BT_SCO_CONNECT_TIMER);
    g_mmi_profiles_bt_sco_timeout = MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_enable_sco_timeout
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_bt_enable_sco_timeout(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//    MMI_TRACE(MMI_MEDIA_TRC_G2_APP,MMI_USER_PROF_BT_ENABLE_SCO_TIEMOUT);

    mmi_trace(g_sw_BT, "mmi_profiles_bt_enable_sco_timeout:");
    g_mmi_profiles_bt_sco_timeout = MMI_TRUE;

    if(mmi_profiles_bt_is_connected(MMI_PROFILES_BT_HFP_SCO))
    {
        StartTimer(BT_SCO_CONNECT_TIMER, BT_SCO_CONNECT_TIMEOUT_DURATION, mmi_profiles_bt_sco_timeout);
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_enable_sco_connect_delay_timer_callback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_bt_enable_sco_connect_delay_timer_callback(void)
{
    mmi_profiles_bt_connect_sco(NULL);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_disable_sco_connection
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_bt_disable_sco_connection(U8 app_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
   // MMI_TRACE(MMI_MEDIA_TRC_G2_APP,MMI_USER_PROF_BT_DISABLE_SCO_CONNECTION, g_mmi_profiles_bt_disalbe_sco, app_id);

	  mmi_trace(g_sw_BT, "mmi_profiles_bt_disable_sco_connection:");	
    StopTimer(BT_SCO_CONNECT_TIMER);
    StopTimer(BT_SCO_ENABLE_CONNECT_DELAY_TIMER);
    g_mmi_profiles_bt_disalbe_sco |= app_id;

    mmi_profiles_bt_disconnect_sco();
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_enable_sco_connection
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_bt_enable_sco_connection(U8 app_id)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
   // MMI_TRACE(MMI_MEDIA_TRC_G2_APP,MMI_USER_PROF_BT_ENABLE_SCO_CONNECTION, g_mmi_profiles_bt_disalbe_sco, app_id);

    g_mmi_profiles_bt_disalbe_sco &= ~app_id;

    if(!g_mmi_profiles_bt_disalbe_sco)
    {
        /* Connect SCO if the audio path is "forward to BT", or the application is runnung */
        if(mmi_bt_is_aud2hf()/* && !mmi_profiles_bt_is_sco_app_idle()*/)
        {
            StartTimer(BT_SCO_ENABLE_CONNECT_DELAY_TIMER, BT_SCO_RECONNECT_TIMER_DURATION, mmi_profiles_bt_enable_sco_connect_delay_timer_callback);
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_sco_timeout
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_bt_sco_timeout(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
 //   MMI_TRACE(MMI_MEDIA_TRC_G2_APP,MMI_USER_PROF_BT_SCO_TIMEOUT,mmi_profiles_bt_is_connected(MMI_PROFILES_BT_HFP_SCO), g_mmi_profiles_bt_in_call);
     mmi_trace(g_sw_BT, "mmi_profiles_bt_sco_timeout     g_mmi_profiles_bt_in_call = %d  ",g_mmi_profiles_bt_in_call);	
	
    if(mmi_profiles_bt_is_connected(MMI_PROFILES_BT_HFP_SCO))
    {
    mmi_trace(g_sw_BT, "mmi_profiles_bt_sco_timeout  TRUE:");	
	
        /* If audio/video is not playing or not in call */
        if(!g_mmi_profiles_bt_in_call
            && mdi_audio_is_idle()
           
        #if defined(__MMI_VIDEO_PLAYER__)
            && !mdi_video_is_playing()
        #endif
        #if defined(__MMI_VIDEO_RECORDER__)
            && !mmi_vdorec_is_in_app()
        #endif
        /*
        #if defined(BGSND_ENABLE)
            && mdi_audio_snd_is_idle()
        #endif
            && !mdi_audio_mma_is_open() 
		*/
            )
        {
            mmi_profiles_bt_disconnect_sco();
        }
        /* still using audio resource, disconnect later */
        else if(g_mmi_profiles_bt_sco_timeout && !g_mmi_profiles_bt_in_call)
        {
            StartTimer(BT_SCO_CONNECT_TIMER, BT_SCO_CONNECT_TIMEOUT_DURATION, mmi_profiles_bt_sco_timeout);
        }
    }
}
                
/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_inquery_start_callback
 * DESCRIPTION
 *  Callback function for BT inquery
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
MMI_BOOL mmi_profiles_bt_inquery_start_callback(void (*callback) (void))
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 sco_status;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    sco_status = mdi_audio_bt_get_stream_status(BT_HFP);

 //   MMI_TRACE(MMI_MEDIA_TRC_G2_APP,MMI_USER_PROF_BT_INQUERY_START_CALLBACK,sco_status);
  mmi_trace(g_sw_BT, "mmi_profiles_bt_inquery_start_callback , sco_status = %d,", sco_status);	

    g_mmi_profiles_bt_inquery_callback = NULL;
    g_mmi_profiles_bt_inquerying = MMI_TRUE;

  /* When HFP is connected, suspend audio background play */
    if(mmi_bt_is_hs_connected() && mmi_bt_is_aud2hf())
    {
         mdi_audio_suspend_background_play();
    switch(sco_status)
    {
        case MDI_AUDIO_BT_STATE_IDLE:
            return MMI_FALSE;
            break;
            
        case MDI_AUDIO_BT_STATE_WAIT_CLOSE:
            g_mmi_profiles_bt_inquery_callback = callback;
            break;
            
        case MDI_AUDIO_BT_STATE_OPEN:
        case MDI_AUDIO_BT_STATE_WAIT_OPEN:
              if(g_mmi_profiles_bt_in_call)
                {
                    g_mmi_profiles_bt_inquerying = MMI_FALSE;

                  mdi_audio_resume_background_play();
                    /* May let caller always call stop inquiry function to resume background play*/
       //             mdi_audio_resume_background_play_by_app(MDI_AUD_SUSPEND_BG_SCO_INQ); 
                //    return MMI_PROFILES_BT_INQ_SCO_CONNECTED_IN_CALL;
                      return MMI_TRUE;
                }
            
            StopTimer(BT_SCO_CONNECT_TIMER);
            g_mmi_profiles_bt_inquery_callback = callback;    
            mdi_audio_suspend_background_play();
            mmi_profiles_bt_disconnect_sco();
            break;

        default:
            ASSERT(0);
            break;
    }
  }
 else
    {
      return MMI_FALSE;
    }
    return MMI_TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_inquery_stop_callback
 * DESCRIPTION
 *  Callback function for BT inquery
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_bt_inquery_stop_callback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
//    MMI_TRACE(MMI_MEDIA_TRC_G2_APP,MMI_USER_PROF_BT_INQUERY_STOP_CALLBACK);
    mmi_trace(g_sw_BT, "mmi_profiles_bt_inquery_stop_callback");	

    g_mmi_profiles_bt_inquery_callback = NULL;

    /* Connect SCO after background play resume */
    g_mmi_profiles_bt_inquerying = MMI_FALSE;
    
    mdi_audio_resume_background_play();
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_bt_connect_audio_via_sco
 * DESCRIPTION
 *  connect sco link
 * PARAMETERS
 *  void
 * RETURNS
 * void
 *****************************************************************************/
void mmi_profiles_bt_connect_audio_via_sco(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
 //   MMI_TRACE(MMI_MEDIA_TRC_G2_APP,MMI_USER_PROF_BT_CONNECT_AUDIO_VIA_SCO,g_mmi_profiles_bt_in_call);

 mmi_trace(1, "mmi_profiles_bt_connect_audio_via_sco   g_mmi_profiles_bt_in_call =%d ",g_mmi_profiles_bt_in_call);	

    /* Press key or play audio in call will not connect SCO,
    *   call setup may leave the voice in phone */
    if(!g_mmi_profiles_bt_in_call && mmi_bt_is_aud2hf())
    {
        mmi_profiles_bt_connect_sco(NULL);
    }
}


#endif /* #if defined(__MMI_BT_AUDIO_VIA_SCO__) || defined(__MMI_BT_FM_VIA_SCO__) */
#endif /* __MMI_BT_PROFILE__ */ 
