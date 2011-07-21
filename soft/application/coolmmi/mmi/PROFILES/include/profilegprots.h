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
 * ProfileGprots.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Profiles application global function prototypes
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: ProfileGprots.h

  	PURPOSE		: Profiles application global function prototypes

 

	AUTHOR		: Ashima Bajaj

	DATE		: 

**************************************************************/

#ifndef _PIXCOM_PROFILESGPROTS_H
#define _PIXCOM_PROFILESGPROTS_H

#include "audioinc.h"
#ifdef MMI_ON_HARDWARE_P
	#include "resource_audio.h"
#endif

#include "settingprofile.h"
#include "profilesenum.h"

BOOL mmi_profiles_check_DRM_content_forwardable( UI_string_type filename );
void initnvramprofapp(void);
S16 InitProfileApp(void);
#ifdef __MMI_BT_PROFILE__
extern BOOL mmi_profiles_bt_is_activated(void);
extern void mmi_profiles_bt_activate_profile(void);
extern void mmi_profiles_bt_deactivate_profile(void);
extern void mmi_profiles_bt_connect_req(U8 connect_type);
extern void mmi_profiles_bt_disconnect_req(U8 connect_type);
extern void mmi_profiles_bt_connect_callback(U8 connect_type, U8 callback_type, U16 result);
extern void mmi_profiles_bt_disconnect_callback(U8 connect_type, U8 callback_type, U16 result);
extern BOOL mmi_profiles_bt_is_connected(U8 connect_type);
extern BOOL mmi_profiles_bt_is_sco_connect_allowed(void);
extern void mmi_profiles_bt_call_start_callback(void);
extern void mmi_profiles_bt_call_end_callback(void);
extern BOOL mmi_profiles_bt_connect_sco(void (*callback) (void));
extern BOOL mmi_profiles_bt_disconnect_sco(void);

#if defined(__MMI_BT_AUDIO_VIA_SCO__) || defined(__MMI_BT_FM_VIA_SCO__)
#define MMI_PROFILE_BT_SCO_INT_A2DP     0x01
#define MMI_PROFILE_BT_SCO_INT_FMRADIO  0x02
#define MMI_PROFILE_BT_SCO_INT_CAMERA   0x04
#define MMI_PROFILE_BT_SCO_INT_VIDEO   0x08

extern void mmi_profiles_bt_disable_sco_timeout(void);
extern void mmi_profiles_bt_enable_sco_timeout(void);
extern void mmi_profiles_bt_disable_sco_connection(U8 app_id);
extern void mmi_profiles_bt_enable_sco_connection(U8 app_id);
extern void mmi_profiles_bt_sco_timeout(void);
extern MMI_BOOL mmi_profiles_bt_inquery_start_callback(void (*callback) (void));
extern void mmi_profiles_bt_inquery_stop_callback(void);
extern void mmi_profiles_bt_connect_audio_via_sco(void);
#endif /* #if defined(__MMI_BT_AUDIO_VIA_SCO__) || defined(__MMI_BT_FM_VIA_SCO__) */


#endif /* __MMI_BT_PROFILE__ */ 
void ActivateHeadset(void);
void DeactivateHeadset(void);
void ActivateSilentProfile(void);
void DeactivateSilentProfile(void);
void initializeProfiles(void);
void mmi_profiles_restore_activated_profile( void );

//Interface functions for other applications
//Interface for ringvolume level
MMI_VOLUME_LEVEL_ENUM GetRingVolumeLevel(void);
//Interface for keypad_volume_level
MMI_VOLUME_LEVEL_ENUM GetKeypadVolumeLevel(void);
//Interface for loudspeaker volume level
MMI_VOLUME_LEVEL_ENUM GetLoudspkrVolumeLevel(void);
//Interface for mt_call_alert_type_enum
MMI_ALERT_TYPE GetMtCallAlertTypeEnum(void);
//Interface for Ring_ToneID
U16 GetRingToneID(void);
#ifdef __MMI_DUAL_PROFILE_SUPPORT__  //Add by Renyh for 221 project   20090213
U16  GetCard2RingToneID(void);
#endif                                                        //Add by Renyh for 221 project   20090213

//Interface for Light
MMI_LEDLIGHT_STATUS_ENUM GetLightStatus(void);
//Interface for Ring_Type_Enum
MMI_RING_TYPE_ENUM GetRingTypeEnum(void);
#ifdef __MMI_CONNECT_NOTICE__
//Interface for MMI_CONNECT_NOTICE_TYPE
U16 GetConnectNoticeType(void);
#endif
//Interface for Answering_Mode_Enum
MMI_ANSWERING_MODE * GetAnsweringMode(void);
//Interface for loud_speaker
pBOOL LoudSpeaker(void);
//Interface for Font_Size_Enum
MMI_FONT_SIZE_ENUM FontSizeEnum(void);
//Activating a profile by name
void ActivateProfile(void);
void ActivatefirstProfile(void);
//Interface function for error tone status
pBOOL GetErrorToneStatus(void);
//Interface function for connect tone status
pBOOL GetConnectToneStatus(void);
//Interface function for powertone type
POWERON_TONE_ENUM	GetPowerOnToneType(void);
//Interface function for poweroff tone type
POWEROFF_TONE_ENUM	GetPowerOffToneType(void);
//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
//Interface function for coveropen tone type
COVER_OPEN_TONE_ENUM	GetCoverOpenToneType(void);
//Interface function for coverclose tone type
COVER_CLOSE_TONE_ENUM	GetCoverCloseToneType(void);
#endif
//CSD end
//Interface function for Keypadtone type
KEYPAD_TONE_ENUM	GetKeypadToneType(void);
//Interface function for warning tone type
pBOOL GetWarningToneType(void);
//Interface function message tone type
MESSAGE_TONE_ENUM	GetMessageToneType(void);
#ifdef __MMI_DUAL_PROFILE_SUPPORT__  //Add by Renyh for 221 project   20090213
MESSAGE_TONE_ENUM	GetCard2MessageToneType(void);
#endif                                                        //Add by Renyh for 221 project   20090213
//Interface function alarmtone type
ALARM_TONE_ENUM		GetAlarmToneType(void);
//interface function for lcd timer
U8 get_lcdbacklighttimer(void);

/***********************Proto Types for Set functions***********/
S16  SetRingVolumeLevel(MMI_VOLUME_LEVEL_ENUM); 

//CSD Brian added for setting activated ring tone volume, 2003/12/02
S16 SetActivatedRingVolumeLevel(MMI_VOLUME_LEVEL_ENUM volumelevel );

S16  SetKeypadVolumeLevel(MMI_VOLUME_LEVEL_ENUM); 

//CSD Brian added for setting activated keypad volume, 2003/12/02
S16  SetActivatedKeypadVolumeLevel(MMI_VOLUME_LEVEL_ENUM volumelevel);

S16  SetLoudSpkrVolumeLevel(MMI_VOLUME_LEVEL_ENUM);

S16  SetMtCallAlertTypeEnum(MMI_ALERT_TYPE);  

S16  SetRingToneID(U16);  
#ifdef __MMI_DUAL_PROFILE_SUPPORT__  //Add by Renyh for 221 project   20090213
S16  SetCard2RingToneID(U16);  
#endif                                                        //Add by Renyh for 221 project   20090213
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
void ThemeManagerSetRingTone(U16 ringtoneid);
void ThemeManagerResetRingTone(void);
#ifdef __MMI_DUAL_PROFILE_SUPPORT__  //Add by Renyh for 221 project   20090213
void ThemeManagerSetCard2RingTone(U16 ringtoneid);
void ThemeManagerResetCard2RingTone(void);
#endif                                                        //Add by Renyh for 221 project   20090213
#endif

S16  SetLightStatus(MMI_LEDLIGHT_STATUS_ENUM); 

S16  SetRingTypeEnum(MMI_RING_TYPE_ENUM);  

#ifdef __MMI_CONNECT_NOTICE__
S16  SetConnectNotice(U16 type);
#endif

S16  SetAnsweringMode(MMI_ANSWERING_MODE); 

S16  SetLoudSpeaker(pBOOL); 

//CSD Brian added for wrapping for Changable font size, 2003/11/11
#ifdef __MMI_CHANGABLE_FONT__
S16  SetFontSizeEnum(MMI_FONT_SIZE_ENUM); 
#endif

S16  SetErrorToneStatus(pBOOL);	

#ifndef __MMI_CONNECT_NOTICE__
S16  SetConnectToneStatus(pBOOL); 
#endif

S16  SetPowerOnToneType(TONE_SETUP); 

S16	 SetPowerOffToneType(TONE_SETUP); 

//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
S16  SetCoverOpenToneType(TONE_SETUP); 

S16  SetCoverCloseToneType(TONE_SETUP);
#endif
//CSD end

S16  SetKeypadToneType(KEYPAD_TONE_ENUM); 
MMI_INTELLIGENT_CALL_ALERT_TYPE GetIntelligentCallAert( void );
S16 SetIntelligentCallAert(MMI_INTELLIGENT_CALL_ALERT_TYPE);
S16  SetMessageToneType(TONE_SETUP);
#ifdef __MMI_DUAL_PROFILE_SUPPORT__  //Add by Renyh for 221 project   20090213
S16  SetCard2MessageToneType(TONE_SETUP);
#endif                                                        //Add by Renyh for 221 project   20090213
S16  SetAlarmToneID(U16); 

#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
void ThemeManagerSetAlarmTone(U16 ringtoneid);
void ThemeManagerResetAlarmTone(void);
#endif

U8 set_lcdbacklighttimer(void);

S16  SetExtraTone(EXTRA_TONE);

void playRequestedTone(ALL_TONE_ENUM);

void stopRequestedTone(ALL_TONE_ENUM);
void SetPlayReqToneFlag(pBOOL flag);
pBOOL GetPlayReqToneFlag(void);
void UpdateDeleteDownloadedRingTone(U16);
void UpdateDeleteDownloadedAlarmTone(U16);
void SetCurrentProfileAsActivatedProfile(void);
BOOL IsHeadsetModeActivated(void );
pBOOL IsSilentModeActivated(void);
BOOL IsMeetingModeActivated(void);
pBOOL SetProfileDefault(void);

#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
void EntryPrfSetRingToneExtMelodyDrv(void);
void HighlightPrfSetRingToneExtMelody(S32 index);
void UpdateDeleteExtMldToneSetup(U16 ToneId);
#endif /* __MMI_PROFILE_EXTMELODY_SUPPORT__ */

#endif


