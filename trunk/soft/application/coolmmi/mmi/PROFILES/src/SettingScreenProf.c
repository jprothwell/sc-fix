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
 * SettingScreenProf.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Screens for Profiles application
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

/***************** Settings Application implementation *********************/

/**************************************************************

	FILENAME	: SettingScreenProf.c

  	PURPOSE		: Screens for Profiles application

 

 

	DATE		: 5/5/2003

**************************************************************/
#include "mmi_features.h"
#include "mmi_trace.h"

#define __NEWSIMULATOR
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "mainmenudef.h"
#include "settingdefs.h"
#include "settingprot.h"
#include "profiles_prot.h"
#include "custdatares.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "custmenures.h"
#include "wgui_categories.h"
#include "wgui_categories_popup.h"
#include "hardwareevents.h"
#include "thrdosinterface.h"
#include "osthreadinterfacegprot.h"
#include "settinggprots.h"
#include "queuegprot.h"
#include "protocolevents.h"
#include "unicodexdcl.h"
#include "settingprofile.h"
#include "profilesenum.h"
#include "globaldefs.h"
#include "profilesgdcl.h"
#include "profilegprots.h"
#include "gpioinc.h"
#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"
#include "debuginitdef.h"
#include "volumehandler.h"
#include "wgui_status_icons.h"
#include "downloadprotos.h"
#include "commonscreens.h"
#include "idleappprot.h"
#include "keybrd.h"
#include "idleappdef.h"
#include "athandlerprot.h"
 
#include "filesystemdef.h" //ritesh
 
#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
#include "frameworkstruct.h"
#include "filemgr.h"
#include "conversions.h"
#include "profilesdefs.h"
#include "profilemgr.h"         /* profile magager general include file*/
#include "profilemgrgprot.h"    /* profile magager extern include file*/
#endif
#include "phonebookdef.h"

#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "callmanagementgprot.h"
#include "simdetectiondef.h"   //added by jinzh:20070715
#include "simdetectiongexdcl.h"

#ifdef __DRM_SUPPORT__
#ifdef MMI_ON_HARDWARE_P
#include "fs_errcode.h"
#include "drm_gprot.h"
#endif
#endif

#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"     // modified by kecx for fixing bug 12633 on 20090703
#endif

#undef __NEWSIMULATOR
 
#include "mmithemes.h" //ritesh
#include "audioplayerdef.h"  //added by jinzh:20070703
#include "audioplayertype.h" //added by jinzh:20070703
/*Jinzh Add Start For Delete phone Item Ver: TBM780  on 2007-8-10 19:34 */
#include "mmi_features_camera.h"
#include "cameraresdefs.h"
/*Jinzh Add End  For Delete phone Item Ver: TBM780  on 2007-8-10 19:34 */

extern audply_struct g_audply;  //Added by jinzh:20070703
 
void (*gfnprofilelsk)(void);
void (*gfnprofilersk)(void);

BOOL isSetKeypadVolume = FALSE; //Set the keypad volume, the key pad tone must be stop

extern void EntryScrSetKeyPadVolumeLevel(void);
extern void GetRingToneRange(U8 *startId, U8*totalId);
extern void GetMidiRange(U8 *startId, U8*totalId);
extern void GetToneRange(U8 *startId, U8*totalId);
extern S32 volume_level_UI;
#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__   
extern void PmgInitExtMelodyStruct(void);
#endif
extern void PlayIsSettingKeypadVolumeReq(U16 soundId, U8 style);
extern void PlayIsSettingKeypadVolumeReq_fromIdle(U16 soundId, U8 style);
extern U8 GetHandsetInPhone();

#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
static PRFSET_MELODY_CONF_TYPE prfset_conf_type = 0;
static U8 g_ext_melody_idx=0;
static U8 extMldHlitItem = 0;
#endif

#if defined(__MMI_PHB_CALLER_RES_SETTING__)
extern void mmi_phb_caller_ring_forward_to_entry(U16 ringToneId);
#endif
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
static U16 selected_downloadable_theme_melody_id = 0;
pBOOL AudioFileDeletedFromFmgr(U16 tone_id);
#endif

#ifdef __MMI_BT_PROFILE__
#ifdef __MMI_HFP_SUPPORT__
extern void mmi_profiles_bt_open_stream_callback(U8 profile, mdi_result result);
extern void mmi_profiles_bt_close_stream_callback(U8 profile, mdi_result result);
extern int mmi_profiles_bt_reset(void);

#endif
#endif /* __MMI_BT_PROFILE__*/
#if defined( __DRM_SUPPORT__) && defined(MMI_ON_HARDWARE_P)
/**************************************************************
	FUNCTION NAME		: mmi_profiles_check_DRM_content_forwardable
  	PURPOSE				: check does the file is DRM prohibted to forward
	INPUT PARAMETERS	: UI_string_type filename
	OUTPUT PARAMETERS	: 
	RETURNS				: BOOL
 
**************************************************************/
BOOL mmi_profiles_check_DRM_content_forwardable( UI_string_type filename )
{
	FS_HANDLE handle;
	DRM_RIGHTS_STRUCT *rights;
	U8 fwd_flag = FMGR_DRM_FWD_APP_FWD_OUT;

	if( (handle = DRM_open_file( filename, FS_READ_ONLY, DRM_PERMISSION_NONE )) > FS_NO_ERROR )
	{
		if( (rights = DRM_get_rights( handle )) != NULL )
		{
			DRM_CONSTRAINT_STRUCT *cons;
			U8 permission = DRM_PERMISSION_NONE;

			cons = &rights->play;
			permission |= DRM_PERMISSION_PLAY;

			if( rights->method & DRM_METHOD_SEPARATE_DELIVERY )
			{
				if( !(rights->status == DRM_STATUS_RIGHTS_PRESENT && (rights->permission & permission) &&
					cons != NULL && cons->type == DRM_CONSTRAINT_NONE) )
				{
					if( rights->method & DRM_METHOD_FORWARD_LOCK )
						fwd_flag = FMGR_DRM_NO_FWD_APP_NO_FWD_OUT; /* can't forward, can't set as ringtine wallpaper */
					else
						fwd_flag = FMGR_DRM_NO_FWD_APP_FWD_OUT; /* can forward, can't set as ringtone wallpaper */
				}
			}
			else if( rights->method & DRM_METHOD_COMBINED_DELIVERY )
			{
				if ( (rights->permission & permission) && cons!= NULL && cons->type == DRM_CONSTRAINT_NONE )
					fwd_flag = FMGR_DRM_FWD_APP_NO_FWD_OUT; /* can't forward, can set as ringtone wallpaper */
				else 
					fwd_flag = FMGR_DRM_NO_FWD_APP_NO_FWD_OUT; /* can't forward, can't set as ringtine wallpaper */
			}
			else
				fwd_flag = FMGR_DRM_FWD_APP_NO_FWD_OUT; /* can't forward, can set as ringtone wallpaper */
		}
		DRM_close_file( handle );
	}
	else
		fwd_flag = FMGR_DRM_NO_FWD_APP_NO_FWD_OUT;

	if( fwd_flag == FMGR_DRM_NO_FWD_APP_NO_FWD_OUT || fwd_flag == FMGR_DRM_NO_FWD_APP_FWD_OUT )
		return FALSE;
	else
		return TRUE;
}
#endif

/**************************************************************

	FUNCTION NAME		: InitProfileApp(void)

  	PURPOSE				: Initialize Profile Application

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: S16

	RETURNS				: void

 

**************************************************************/
S16 InitProfileApp(void)
{
    if(gprofilenvram == 0)
	{
		initializeProfiles();
		ActivatefirstProfile();
	    #ifndef USE_NVRAM_FOR_PROFILES
       	 gprofilenvram = 1 ;
	    #endif
	
	}

	SetHiliteHandler(MAIN_MENU_PROFILES_GENERAL,HighlightGeneralProfile);
	SetHiliteHandler(MAIN_MENU_PROFILES_MEETING,HighlightMeetingProfile);
	SetHiliteHandler(MAIN_MENU_PROFILES_OUTDOOR,HighlightOutdoorProfile);
	SetHiliteHandler(MAIN_MENU_PROFILES_INDOOR,HighlightIndoorProfile);
	SetHiliteHandler(MAIN_MENU_PROFILES_HEADSET,HighlightHeadsetProfile);

	SetHiliteHandler(MENU_LIGHT_SETTIMER,HighlightProfilesSetTimer);

#ifndef __MMI_PROF_MENU_ACTIVE_SHRINK__
	SetHiliteHandler(MENU_PROFILES_OPTIONS,HighlightProfilesOptions);
	SetHiliteHandler(MENU_PROFILES_CUSTOMIZE,HighlightProfilesCustomize);
#endif /* __MMI_PROF_MENU_ACTIVE_SHRINK__ */
#ifdef __MMI_BT_PROFILE__
    SetHiliteHandler(MAIN_MENU_PROFILES_BT, mmi_profiles_hilite_hdlr_bt_mode);
#endif 
	SetHiliteHandler(MENU_PROFILES_ACTIVATE,HighlightProfileActivateOptions);
	SetHiliteHandler(MENU_VOLUME_LEVEL,HighlightProfilesVolumeLevel);
	SetHiliteHandler(MENU_MT_CALL_ALERT_TYPE,HighlightProfilesCallAlert);
	SetHiliteHandler(MENU_RING_TYPE,HighlightProfilesRingType);
	SetHiliteHandler(MENU_ANSEWRING_MODE,HighlightProfilesAnsweringMode);
//CSD Brian added for wrapping for Changable font size, 2003/11/11
#ifdef __MMI_CHANGABLE_FONT__
	SetHiliteHandler(MENU_FONT_SIZE,HighlightProfilesFontSize);
#endif
#ifdef __MMI_STATUS_LED__
	SetHiliteHandler(MENU_LIGHT,HighlightProfilesLight);
	SetHiliteHandler(MENU_LIGHT_STATUS,HighlightProfilesStatus);
#endif
	SetHiliteHandler(MENU_TONE_SETUP,HighlightProfilesToneSetup);
	SetHiliteHandler(MENU_TONE_SETUP_RING_TONE,HighlightProfilesIncomingCallToneSetup);  //Modified by Jinzh:20070617
	SetHiliteHandler(MENU_TONE_SETUP_POWERON_TONE,HighlightProfilesPoweronToneSetup);
	SetHiliteHandler(MENU_TONE_SETUP_POWEROFF_TONE,HighlightProfilesPoweroffToneSetup);
//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
	SetHiliteHandler(MENU_TONE_SETUP_COVEROPEN_TONE,HighlightProfilesCoverOpenToneSetup);
	SetHiliteHandler(MENU_TONE_SETUP_COVERCLOSE_TONE,HighlightProfilesCoverCloseToneSetup);
#endif
//CSD end
	SetHiliteHandler(MENU_TONE_SETUP_MESSAGE_TONE,HighlightProfilesMessageToneSetup);
	SetHiliteHandler(MENU_TONE_SETUP_KEYPAD_TONE,HighlightProfilesKeypadToneSetup);
//qiff 2008/10/9 greenstone: bug 9863
#ifndef __MMI_ALM_CUSTOMIZE_TONE__
	SetHiliteHandler(MENU_TONE_SETUP_ALARM_TONE,HighlightAlarmToneSetup);
#endif
	SetHiliteHandler(MENU_EXTRA_TONE,HighlightExtraToneSetup);

	SetHiliteHandler(MENU_RING_VOLUME_LEVEL,HighlightRingToneVolume);
	SetHiliteHandler(MENU_KEYPAD_VOLUME_LEVEL,HighlightKeyToneVolume);
#if defined(__CALL_VOLUME_LEVEL__)
//wug add used to pc300 start 20071010
  SetHiliteHandler(MENU_CALL_VOLUME_LEVEL,HighlightCallToneVolume);
//wug add used to pc300 end 20071010
#endif

	SetHiliteHandler(MENU_RING,HighlightCallAlertRing);
	SetHiliteHandler(MENU_VIBRATION_ONLY,HighlightCalAlertVibrationOnly);
	SetHiliteHandler(MENU_VIBRATION_AND_RING,HighlightCallAlertVibrationAndRing);
	SetHiliteHandler(MENU_VIBRATION_THEN_RING,HighlightCallAlertVibrationThenRing);
	SetHiliteHandler(MENU_SILENT,HighlightCallAlertSilent);

	SetHiliteHandler(MENU_RING_TYPE_SINGLE,HighlightRingTypeSingle);
	SetHiliteHandler(MENU_RING_TYPE_REPEAT,HighlightRingTypeRepeat);
	SetHiliteHandler(MENU_RING_TYPE_ASCENDING,HighlightRingTypeAscending);

#ifdef __MMI_CONNECT_NOTICE__
	SetHiliteHandler(MENU_CONNECT_NOTICE,HighlightProfilesConnectNotice);
	SetHiliteHandler(MENU_NOTICE_NONE,HighlightConnectNoticeNone);
	SetHiliteHandler(MENU_NOTICE_TONE_ONLY,HighlightConnectNoticeToneOnly);
	SetHiliteHandler(MENU_NOTICE_VIB_ONLY,HighlightConnectNoticeVibOnly);
	SetHiliteHandler(MENU_NOTICE_TONE_AND_VIB,HighlightConnectNoticeToneAndVib);
#endif
//CSD Brian added for wrapping for Changable font size, 2003/11/11
#ifdef __MMI_CHANGABLE_FONT__
	SetHiliteHandler(MENU_FONT_SIZE_THEME_DEFAULT,HighlightFontSizeDefault);
	SetHiliteHandler(MENU_FONT_SIZE_SMALL,HighlightFontSizeSmall);
	SetHiliteHandler(MENU_FONT_SIZE_MEDIUM,HighlightFontSizeMedium);
//CSD Brian added for wrapping for DIALING FONT, 2003/10/31
#ifndef __MMI_DIALLING_FONT__
	SetHiliteHandler(MENU_FONT_SIZE_LARGE,HighlightFontSizeLarge);
#endif
#endif

	SetHiliteHandler(MENU_KEYPAD_TONE_SILENT,HighlightKeyPadToneSilent);
	SetHiliteHandler(MENU_KEYPAD_TONE_CLICK,HighlightKeyPadToneClick);
	SetHiliteHandler(MENU_KEYPAD_TONE_TONE,HighlightKeyPadToneTone);

#if defined( __MMI_HUMAN_VOICE_KEYPAD_TONE__ )
	SetHiliteHandler(MENU_KEYPAD_TONE_HUMAN_VOICE_1,HighlightKeyPadToneHumanVoice1);
#if defined(__PROJECT_GALLITE_C01__)
#else
	SetHiliteHandler(MENU_KEYPAD_TONE_HUMAN_VOICE_2,HighlightKeyPadToneHumanVoice2);
	SetHiliteHandler(MENU_KEYPAD_TONE_HUMAN_VOICE_3,HighlightKeyPadToneHumanVoice3);
	SetHiliteHandler(MENU_KEYPAD_TONE_HUMAN_VOICE_4,HighlightKeyPadToneHumanVoice4);
	SetHiliteHandler(MENU_KEYPAD_TONE_HUMAN_VOICE_5,HighlightKeyPadToneHumanVoice5);
	SetHiliteHandler(MENU_KEYPAD_TONE_HUMAN_VOICE_6,HighlightKeyPadToneHumanVoice6);
#endif
#endif

#if defined( __MMI_CUST_KEYPAD_TONE__ )
	SetHiliteHandler(MENU_KEYPAD_TONE_CUST_1,HighlightKeyPadToneCust1);
	SetHiliteHandler(MENU_KEYPAD_TONE_CUST_2,HighlightKeyPadToneCust2);
	SetHiliteHandler(MENU_KEYPAD_TONE_CUST_3,HighlightKeyPadToneCust3);
	SetHiliteHandler(MENU_KEYPAD_TONE_CUST_4,HighlightKeyPadToneCust4);
	SetHiliteHandler(MENU_KEYPAD_TONE_CUST_5,HighlightKeyPadToneCust5);
	SetHiliteHandler(MENU_KEYPAD_TONE_CUST_6,HighlightKeyPadToneCust6);
#endif

#if defined( __MMI_INTELLIGENT_CALL_ALERT__ )
	SetHiliteHandler(MENU_INTELLIGENT_CALL_ALERT,HighlightHandlerIntelligentCallAlert);
	SetHintHandler(MENU_INTELLIGENT_CALL_ALERT,HintIntelligentCallAlert);
	SetHiliteHandler(MENU_INTELLIGENT_CALL_ALERT_ON,HighlightHandlerIntelligentCallAlertOn);
	SetHiliteHandler(MENU_INTELLIGENT_CALL_ALERT_OFF,HighlightHandlerIntelligentCallAlertOff);
#endif

	SetHintHandler(MENU_TONE_SETUP_RING_TONE,HintHandlerIncomingCall);
#ifndef __MMI_ALM_CUSTOMIZE_TONE__
	SetHintHandler(MENU_TONE_SETUP_ALARM_TONE,HintHandlerAlarmTone);
#endif
	SetHintHandler(MENU_TONE_SETUP_POWERON_TONE,HintHandlerPowerOnTone);
	SetHintHandler(MENU_TONE_SETUP_POWEROFF_TONE,HintHandlerPowerOffTone);

	SetHintHandler(MENU_TONE_SETUP_MESSAGE_TONE,HintHandlerMessgaeTone);
	SetHintHandler(MENU_TONE_SETUP_KEYPAD_TONE,HintHandlerKeypadTone);

//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
	SetHintHandler(MENU_TONE_SETUP_COVEROPEN_TONE,HintHandlerCoverOpenTone);
	SetHintHandler(MENU_TONE_SETUP_COVERCLOSE_TONE,HintHandlerCoverCloseTone);
#endif
//CSD end
	SetHintHandler(MENU_MT_CALL_ALERT_TYPE,HintHandlerAlertType);
	SetHintHandler(MENU_RING_TYPE,HintHandlerRingType);
#ifdef __MMI_CONNECT_NOTICE__
	SetHintHandler(MENU_CONNECT_NOTICE,HintHandlerConnectNotice);
#endif
//CSD Brian added for wrapping for Changable font size, 2003/11/11
#ifdef __MMI_CHANGABLE_FONT__
	SetHintHandler(MENU_FONT_SIZE,HintHandlerFontSize);
#endif
	SetHintHandler(MENU_RING_VOLUME_LEVEL,HintHandlerRingVolume);
	SetHintHandler(MENU_KEYPAD_VOLUME_LEVEL,HintHandlerKeyVolume);
#if defined(__CALL_VOLUME_LEVEL__)
//wug add used to pc300 start 20071010
  SetHintHandler(MENU_CALL_VOLUME_LEVEL,HintHandlerCallVolume);
//wug add used to pc300 end 20071010
#endif
	SetHintHandler(MENU_LIGHT_SETTIMER,HintHandlerLCDBacklight);
#ifdef __MMI_STATUS_LED__
	SetHintHandler(MENU_LIGHT_STATUS,HintHandlerStatusLED);
#endif

#ifdef __MMI_BT_PROFILE__  //added by Zhujianguo, 2009.6.16
#ifdef __MMI_HFP_SUPPORT__
        mdi_audio_bt_init(BT_HFP, mmi_profiles_bt_open_stream_callback, mmi_profiles_bt_close_stream_callback);
#endif 
#endif
#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__   //added by liwh:20070713
	PmgInitExtMelodyStruct();
#endif

#ifdef __MMI_DUAL_PROFILE_SUPPORT__    //Add by Renyh for 221 project   20090213
    SetHiliteHandler(MENU_TONE_SETUP_CARD2_RING_TONE, mmi_profiles_hilite_hdlr_card2_ring_tone);    
    SetHintHandler(MENU_TONE_SETUP_CARD2_RING_TONE, mmi_profiles_hint_hdlr_card2_incoming_call);
    SetHiliteHandler(MENU_TONE_SETUP_CARD2_MESSAGE_TONE, mmi_profiles_hilite_hdlr_card2_message_tone);    
    SetHintHandler(MENU_TONE_SETUP_CARD2_MESSAGE_TONE, mmi_profiles_hint_hdlr_card2_message_tone);
#endif                                                           //Add by Renyh for 221 project   20090213

return 0;
}

#if defined(__CALL_VOLUME_LEVEL__)
//wug add used to pc300 start 20071010

/**************************************************************

	FUNCTION NAME		: initnvramprofapp(void)

  	PURPOSE				: Initialize NVRAM for Profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: S16

	RETURNS				: void

 

**************************************************************/
void WriteCallToneVal(UINT8  level)
{
	S16 error;
  
	switch(prevMode)
	{
      /*Jinzh Modify Start For 6485 Ver: TBM780  on 2007-8-29 14:30 */
    case AUD_MODE_NORMAL:
         WriteValue(NVRAM_NORMAL_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
    case AUD_MODE_LOUDSPK:
         WriteValue(NVRAM_LDSPK_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
         break;
    case AUD_MODE_HEADSET:
         WriteValue(NVRAM_HDSET_MODE_VOICE_LEVEL,&level,DS_BYTE,&error);
         break;
    default:
      mmi_trace(1, "WriteCallToneVal prevMode ERROR!!");
      break;

	}

}
//wug add used to pc300 end 20071010
#endif

/**************************************************************

	FUNCTION NAME		: initnvramprofapp(void)

  	PURPOSE				: Initialize NVRAM for Profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: S16

	RETURNS				: void

 

**************************************************************/
void initnvramprofapp(void)
{
#ifdef USE_NVRAM_FOR_PROFILES
	S16 error ;
	U16	j;
	gprofilenvram = 1;

		j = 1;
	ReadRecord (NVRAM_EF_PROFILES_LID, j , &gprofiles[0], MAX_ELEMENTS*sizeof(PROFILE)/*NVRAM_PROFILES_RECORD_SIZE*/,
		        &error);
#ifdef MMI_ON_WIN32
	if(gprofiles[1].toneSetup.ringTone == 65535)
	{
		initializeProfiles();

		glastactivatedprofile = 0;
		WriteValue( NVRAM_PROFILES_LASTACTIVATED, &glastactivatedprofile, DS_BYTE , &error);
		gactivatedprofile =0;
		WriteValue( NVRAM_PROFILES_ACTIVATED, &gactivatedprofile, DS_BYTE , &error);

		j = 1;
		if(WriteRecord (NVRAM_EF_PROFILES_LID,j, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
					& error)<NVRAM_PROFILES_RECORD_SIZE)
		{
			PRINT_INFORMATION (("## ERROR: InitProfiles: WriteRecord "
								"to NVRAM failed (ecode%d)\n", (int) error));
		}
	}
#endif
   	ReadValue( NVRAM_PROFILES_ACTIVATED, &gactivatedprofile, DS_BYTE , &error);
	ReadValue( NVRAM_PROFILES_LASTACTIVATED, &glastactivatedprofile, DS_BYTE , &error);
#endif
   	ActivateProfile();
	showAlertStatusIcons();  /*function to show status icon on ilde screen*/
}

/**************************************************************

	FUNCTION NAME		: EntryScrProfiles(void)

  	PURPOSE				: Entry fn for main profile screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfiles(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U8* guiBuffer;       /* Buffer holding history data */
	U16 gimagearray2[MAX_SUB_MENUS]; //array for profilenames
	U16 nDispAttribute;  /* Stores display attribue */
	U8 i=0,data;
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16	ItemIcons[MAX_SUB_MENUS];
	static U8 ProfileHeadsetFirst=0;
/*** This array has been added to handle the problem of highlighting the activated profile.
	This array will provide the menu id corresponding to activated profile****/
	U16 ProfileMenuID[]={
						MAIN_MENU_PROFILES_GENERAL,
						MAIN_MENU_PROFILES_MEETING,
						MAIN_MENU_PROFILES_OUTDOOR,
						MAIN_MENU_PROFILES_INDOOR,
						MAIN_MENU_PROFILES_HEADSET,
                                           0,
					    #ifdef __MMI_BT_PROFILE__
						 MAIN_MENU_PROFILES_BT,
					    #endif /* __MMI_BT_PROFILE__ */ 

					};

	TBM_ENTRY(0x28D9);
	mmi_trace(g_sw_SETTING, TSTXT("###### in EntryScrProfiles() ############\n") ); 

	PRINT_INFORMATION (("<<------- profile list ------->>"));

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_PROFILES, ExitScrProfiles, EntryScrProfiles, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_PROFILES);

	/* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MAIN_MENU_PROFILES_MENUID);

	/* 5. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MAIN_MENU_PROFILES_MENUID);

	/* 6. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MAIN_MENU_PROFILES_MENUID,nStrItemList);

	GetSequenceImageIds(MAIN_MENU_PROFILES_MENUID,ItemIcons);

	SetParentHandler(MAIN_MENU_PROFILES_MENUID);

	for(i=0;i<nNumofItem;i++)
	{
		gimagearray2[i] = IMG_PROFILES_NO_IMAGE;
	}

	for(i=0;i<nNumofItem;i++)
	{
		memset((S8*) subMenuDataPtrs[i],0,MAX_SUB_MENU_SIZE);
		pfnUnicodeStrncpy((S8*)subMenuDataPtrs[i],GetString(nStrItemList[i]),MAX_SUBMENU_CHARACTERS);
	}
/****** Get the index of activated profile menu ID*****/
      mmi_trace(g_sw_BT, "gactivatedprofile= %d", gactivatedprofile);
	data = GetIndexOfStringId(MAIN_MENU_PROFILES_MENUID,ProfileMenuID[gactivatedprofile]);
	mmi_trace(g_sw_BT, "GetIndexOfStringId = %d", data);
/******If Earfone is connected on this screen, then gui beffer needs to be refreshed.*******/
	if(gactivatedprofile==4)
	{
		if(!ProfileHeadsetFirst)
		{
			if( guiBuffer != NULL)
				change_list_menu_category_history(guiBuffer,data,nNumofItem,0);
			ProfileHeadsetFirst=1;
		}
	}
/******If headset is last activated profile then GuiBufeer should be set as null***/
	else if(glastactivatedprofile == 4)
	{
		if(ProfileHeadsetFirst)
		{
			ProfileHeadsetFirst=0;
			guiBuffer = NULL;
		}
	}
/********** If activated profile is silent then no profile should be seen as activated*****/
	if(gactivatedprofile != 5)
		gimagearray2[data] = IMG_PROFILES_ACTIVATED;


	RegisterHighlightHandler (ExecuteCurrHiliteHandler);

 
#ifdef __MMI_WGUI_MINI_TAB_BAR__
	mmi_wgui_enable_mini_tab_bar(MAIN_MENU_PROFILES_MENUID);
#endif
 

	ShowCategory73Screen(STR_PROFILE_CAPTION,GetRootTitleIcon(MAIN_MENU_PROFILES_MENUID),
						 STR_GLOBAL_OPTIONS,0,
						 STR_GLOBAL_BACK, 0,
						 nNumofItem,(U8**)subMenuDataPtrs,(U16*)gimagearray2,ItemIcons,
						 (U16)data,guiBuffer,1);


	/* 9. Register Exit handler */
	//SetExitHandler(SCR_PROFILES, ExitScrProfiles);
	TBM_EXIT(0x28D9);
}

/**************************************************************

	FUNCTION NAME		: ExitScrProfiles(void)

  	PURPOSE				: Exit profile screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitScrProfiles(void)
{
	gprofileId = (U8)gcurrHiliteIndexOne;

 
#ifdef __MMI_WGUI_MINI_TAB_BAR__
	mmi_wgui_disable_mini_tab_bar();
#endif
 
}

/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_is_device_profile
 * DESCRIPTION
 *  check does the selected or activated profile a device profile which can't be activated manually
 * PARAMETERS
 *  void
 *  BOOL(?)     [OUT]       
 * RETURNS
 *  void
 *****************************************************************************/
BOOL mmi_profiles_is_device_profile(void)
{
#ifdef __MMI_BT_PROFILE__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((gprofileId == MMI_PROFILE_HEADSET) || (gprofileheadset == 1) || (gprofileId == MMI_PROFILE_BT) ||
        (gactivatedprofile == MMI_PROFILE_BT))
    {
        return TRUE;
    }
#else /* __MMI_BT_PROFILE__ */ 
    if ((gprofileId == MMI_PROFILE_HEADSET) || (gprofileheadset == 1))
    {
        return TRUE;
    }
#endif /* __MMI_BT_PROFILE__ */ 
    return FALSE;
}


/**************************************************************

	FUNCTION NAME		: EntryScrProfilesOptions(void)

  	PURPOSE				: Entry Function of Profiles Options
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesOptions(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem = 0;      /* Stores no of children in the submenu*/
	U16 nDispAttribute = 0;  /* Stores display attribue */
	U8* guiBuffer = NULL;       /* Buffer holding history data */
	U8* PopUpList[MAX_SUB_MENUS];
	S32 i=0;
	U16 ParentId = 0;

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_PROFILES_OPTIONS, NULL, EntryScrProfilesOptions, NULL);

/****If headset is connected then options menu should show Customize only*******/
       if (mmi_profiles_is_device_profile())
		ParentId=MENU_PROFILES_OPTIONS_HEADSET;
	else
		ParentId=MENU_PROFILES_OPTIONS;

	/* 1 Set parent handler */
	SetParentHandler(ParentId);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_PROFILES_OPTIONS);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(ParentId);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(ParentId);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(ParentId,nStrItemList);

	for(i=0;i<nNumofItem;i++)
	{
		PopUpList[i] = NULL;
	}

	RegisterHighlightHandler (ExecuteCurrHiliteHandler);

	/* 8 Display Category Screen */

	ShowCategory52Screen(STR_PROFILE_OPTION_CAPTION,GetRootTitleIcon(ParentId),
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem,
						  nStrItemList,
						  (PU16)gIndexIconsImageList,
						  PopUpList ,
						  0,0,
						  guiBuffer);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/* 10. Register Exit handler */
	//SetExitHandler(SCR_PROFILES_OPTIONS, ExitScrProfilesOptions);
}

/**************************************************************

	FUNCTION NAME		: EntryScrProfilesActivate(void)

  	PURPOSE				: Activates the Highlighted profile

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesActivate(void)
{
	S16 error=0;

//	SetParentHandler(MENU_PROFILES_ACTIVATE);
	gactivatedprofile =gprofileId;
	DisplayPopup((PU8)GetString(STR_GENERAL_ACTIVATE_MESSAGE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);

	WriteValue( NVRAM_PROFILES_ACTIVATED, &gactivatedprofile, DS_BYTE , &error);
	ActivateProfile();
	DeleteNHistory(1);//qiff add for greenstone bug :9441
}

/**************************************************************

	FUNCTION NAME		: EntryScrProfilesActivateById( U8 profileId )

  	PURPOSE				: Activates the profileId

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesActivateById( U8 profileId )
{
	S8 str_activate[100];
	U16 msg_id= 0;
	S16 error = 0;

	switch( profileId )
	{
	case 0:
		msg_id = STR_PROFILES_GENERAL;
		break;
	case 1:
		msg_id = STR_PROFILES_MEETING;
		break;
	case 2:
		msg_id = STR_PROFILES_OUTDOOR;
		break;
	case 3:
		msg_id = STR_PROFILES_INDOOR;
		break;
	case 4:
		msg_id = STR_PROFILES_HEADSET;
		break;
	case 5:
		msg_id = STR_PROFILES_SILENT;
		break;
    #ifdef __MMI_BT_PROFILE__
        case MMI_PROFILE_BT:
            msg_id = STR_PROFILES_BT;
            break;
    #endif /* __MMI_BT_PROFILE__ */ 
	default:
		MMI_ASSERT( 0 );
	}

	pfnUnicodeStrcpy( str_activate, GetString(msg_id) );
	pfnUnicodeStrcat( str_activate, (S8*)L" " );
	pfnUnicodeStrcat( str_activate, GetString( STR_GENERAL_ACTIVATE_MESSAGE ) );

	gactivatedprofile = gprofileId = profileId;

	DisplayPopup( (U8*)str_activate, IMG_GLOBAL_ACTIVATED, TRUE, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE );

	WriteValue( NVRAM_PROFILES_ACTIVATED, &gactivatedprofile, DS_BYTE, &error );
	ActivateProfile();
}

/**************************************************************

	FUNCTION NAME		: EntryScrProfilesCustomize(void)

  	PURPOSE				: Showing profile customize screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesCustomize(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	U8* PopUpList[MAX_SUB_MENUS];
	U16 Parentid;

	/* 1 Call Exit Handler*/
	/* Shall leave the exit handler for AT command set vibrator mode */
	EntryNewScreen(SCR_PROFILES_CUSTOMIZE, ExitScrProfilesCustomize, NULL, NULL);

/**** If Chinese is set then Menu item Font Size should not be displayed******/
#ifndef __MMI_MAINLCD_220X176__
	if(IsChineseSet())
	{
#ifdef __MMI_PROF_MENU_ACTIVE_SHRINK__
	    if((gprofileId == 4)||(gprofileheadset == 1))
		  Parentid=MENU_PROFILES_CUSTOMIZE_CHINESE_HEADSET;
	    else
#endif /* __MMI_PROF_MENU_ACTIVE_SHRINK__ */

		Parentid=MENU_PROFILES_CUSTOMIZE_CHINESE;
        }
	else
#endif /*__MMI_MAINLCD_220X176__*/		
	{
#ifdef __MMI_PROF_MENU_ACTIVE_SHRINK__
	    if((gprofileId == 4)||(gprofileheadset == 1))
		  Parentid=MENU_PROFILES_CUSTOMIZE_HEADSET;
	else
#endif /* __MMI_PROF_MENU_ACTIVE_SHRINK__ */
		Parentid=MENU_PROFILES_CUSTOMIZE;
        }

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_PROFILES_CUSTOMIZE);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(Parentid);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(Parentid);
	TRACE_EF(g_sw_AUDIO, "parentid = %d", Parentid);
	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(Parentid,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(Parentid);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);


	ConstructHintsList(Parentid, PopUpList);

    ShowCategory52Screen(STR_PROFILE_CUSTOMIZE_CAPTION,GetRootTitleIcon(Parentid),
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem,
						  nStrItemList,
						  (PU16)gIndexIconsImageList,
						  PopUpList ,
						  0,0,
						  guiBuffer);

	gcommonvolumehandler = 0;
	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/* 10. Register Exit handler */
	//SetExitHandler(SCR_PROFILES_CUSTOMIZE, ExitScrProfilesCustomize);
}

/***************************************************************

	FUNCTION NAME		: ExitScrProfilesCustomize(void)

  	PURPOSE				: Exit profile Customize screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
/* Shall leave the exit handler for AT command set vibrator mode */
void ExitScrProfilesCustomize(void)
{
	history_t   Scr; /* Variable to hold the history data*/
	S16		nHistory	=	0;
	Scr.scrnID = SCR_PROFILES_CUSTOMIZE;
	Scr.entryFuncPtr = EntryScrProfilesCustomize;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)&nHistory);
	GetCategoryHistory(Scr.guiBuffer);
	AddHistory(Scr);
}


/**************************************************************

	FUNCTION NAME		: EntryScrProfilesVolumeType(void)

  	PURPOSE				: Entry function for Volume option

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesVolumeType(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
    U8* PopUpList[MAX_SUB_MENUS];
	mmi_trace(g_sw_SETTING, TSTXT("###### in EntryScrProfilesVolumeType() \n")); 
	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_PROFILES_VOLUME_LEVEL, NULL, EntryScrProfilesVolumeType, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_PROFILES_VOLUME_LEVEL);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_VOLUME_LEVEL);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_VOLUME_LEVEL);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_VOLUME_LEVEL,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_VOLUME_LEVEL);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);


	ConstructHintsList(MENU_VOLUME_LEVEL, PopUpList);

    ShowCategory52Screen(STR_VOLUME_LEVEL_CAPTION,GetRootTitleIcon(MENU_VOLUME_LEVEL),
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem,
						  nStrItemList,
						  (PU16)gIndexIconsImageList,
						  PopUpList ,
						  0,gcommonvolumehandler,
						  guiBuffer);

	/* 9.Register function with right softkey */
	//SetExitHandler(SCR_PROFILES_VOLUME_LEVEL,ExitScrProfilesVolumeType );
}


/**************************************************************

	FUNCTION NAME		: EntryScrProfilesCommonVolumeLevel1(void)

  	PURPOSE				: Does all settings before entering into the entry fn of volume screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesCommonVolumeLevel1(void)
{

	switch(gcommonvolumehandler)
	{
		case 0:
			gcommonvolumelevel = gprofiles[gprofileId].ringVolumeLevel;

		break;
		case 1:
			gcommonvolumelevel = gprofiles[gprofileId].keypadVolumeLevel;
		break;
#if defined(__CALL_VOLUME_LEVEL__)
//wug add used to pc300 start 20071010
    case 2:
			gcommonvolumelevel = GetModeSpeechVolume();
		break;
//wug add used to pc300 end 20071010
#endif
	}
	EntryScrProfilesCommonVolumeLevel();
}
//CSD Brian added for play ring tone after MT message, 2003/11/10
/**************************************************************

	FUNCTION NAME		: ContinueRingTone(void)

  	PURPOSE				: Continue Ring tone after message received on ring tone volume screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ContinueRingTone(void *info)
{
	SetRingerVolume(gcommonvolumelevel);
	if( !IsSilentModeActivated() )
#ifdef __MMI_SILENT_MEETING_PROFILE__
		if( !IsMeetingModeActivated() )
#endif /* __MMI_SILENT_MEETING_PROFILE__ */
	AudioPlayToneWithCallBackVolPath( gprofiles[gprofileId].toneSetup.ringTone, REPEAT, INCOMING_CALL_TONE, (U8)gcommonvolumelevel, MDI_DEVICE_SPEAKER2 );
}


/**************************************************************

	FUNCTION NAME		: EntryScrProfilesCommonVolumeLevel(void)

  	PURPOSE				: Set all Voulem screen handler functions and call the general entry screen of volume level.
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesCommonVolumeLevel(void)
{

	VOLUMEHANDLER_TYPE type;
	TRACE_AUD_FUNCTION();

	//ExecuteCurrExitHandler();
	EntryNewScreen(SCR_PROFILES_COMMON_VOLUME_LEVEL, NULL, NULL, NULL);

switch(gcommonvolumehandler)
	{
	case 0:
		mdi_audio_suspend_background_play();	/* stop all because it might be play file or play id */
	SetRingerVolume(gcommonvolumelevel);
	type = VOLUMEHANDLER_RING_TONE;
/*** Set all function handlers volume level screen *****/
	SetVolumeKeyHandlerFunctions(VolumeIncreaseDecreaseFunction, 
								VolumeIncreaseDecreaseFunction, 
								NULL, NULL, STR_COMMON_VOLUME_LEVEL_RING_CAPTION, 
								&gcommonvolumelevel, 
								STR_GLOBAL_OK, STR_GLOBAL_BACK,
								EntryScrSetCommonVolumeLevel, 
								GoBackCommonVolume,
                                 0, ExitScrProfilesCommonVolumeLevel, 1,type);

	EntryScrSetKeyPadVolumeLevel();
	if( !IsSilentModeActivated() )
#ifdef __MMI_SILENT_MEETING_PROFILE__
		if( !IsMeetingModeActivated() )
#endif /* __MMI_SILENT_MEETING_PROFILE__ */
	AudioPlayToneWithCallBackVolPath( gprofiles[gprofileId].toneSetup.ringTone, REPEAT, INCOMING_CALL_TONE, (U8)gcommonvolumelevel, MDI_DEVICE_SPEAKER2 );
	//CSD Brian added for play ring tone after MT message, 2003/11/10
	SetProtocolEventHandler(ContinueRingTone,PRT_EQ_PLAY_AUDIO_FINISH_IND);

	break;
	case 1:
	TRACE_EF(g_sw_AUDIO, "gcommonvolumelevel = %d", gcommonvolumelevel);
	mdi_audio_suspend_background_play();	/* stop all because it might be play file or play id */
	isSetKeypadVolume = TRUE; //add by panxu 2007-2-27 for the keypad volume
	
	SetKeyPadVolume(gcommonvolumelevel);
    type = VOLUMEHANDLER_KEY_TONE;
/*** Set all function handlers volume level screen *****/
	SetVolumeKeyHandlerFunctions(VolumeIncreaseDecreaseFunction, 
								VolumeIncreaseDecreaseFunction, 
								NULL, NULL, STR_COMMON_VOLUME_LEVEL_KEY_CAPTION, 
								&gcommonvolumelevel, 
                                                STR_GLOBAL_OK,STR_GLOBAL_BACK,

                                                EntryScrSetCommonVolumeLevel, GoBackCommonVolume,

                                                0, ExitScrProfilesCommonVolumeLevel, 1,type);

	EntryScrSetKeyPadVolumeLevel();
	AudioStopReq((U16)gprofiles[gprofileId].toneSetup.keypadTone );
	break;
#if defined(__CALL_VOLUME_LEVEL__)
//wug add used to pc300 start 20071010
  case 2:	
		SetSpeechVolume(gcommonvolumelevel);//g_audply.volume    .wug..??????
    type = VOLUMEHANDLER_SPEECH_TONE;
/*** Set all function handlers volume level screen *****/
	SetVolumeKeyHandlerFunctions(VolumeIncreaseDecreaseFunction, 
								VolumeIncreaseDecreaseFunction, 
								NULL, NULL, STR_COMMON_VOLUME_LEVEL_CALL_CAPTION, 
								&gcommonvolumelevel, 
                                                STR_GLOBAL_OK,STR_GLOBAL_BACK,

                                                EntryScrSetCommonVolumeLevel, GoBackCommonVolume,

                                                0, ExitScrProfilesCommonVolumeLevel, 1,type);

	EntryScrSetKeyPadVolumeLevel();
	//AudioStopReq((U16)gprofiles[gprofileId].toneSetup.keypadTone );   what????
	break;
//wug add used to pc300 end 20071010
#endif
	}


	SetKeyHandler(GoIdleScreenCommonVolume,KEY_END,KEY_EVENT_DOWN);
#if ( !defined(__NO_SIDE_VOLUME_KEY_46__)&&!defined(__NO_SIDE_VOLUME_KEY_STAR_POUND__) )
#ifndef __MMI_MAINLCD_220X176__
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
#endif
#endif
}

/**************************************************************

	FUNCTION NAME		: VolumeIncreaseDecreaseFunction(void)

  	PURPOSE				: Call Back function for Volume increase and decrease.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void VolumeIncreaseDecreaseFunction(void)
{
	TRACE_SYS_FUNCTION();
	TRACE_EF(g_sw_AUDIO, "VolumeIncreaseDecreaseFunction ");

	if (gcommonvolumehandler && !IsSilentModeActivated()) //changed by panxu 20061228
#ifdef __MMI_SILENT_MEETING_PROFILE__
		if( !IsMeetingModeActivated() )
#endif /* __MMI_SILENT_MEETING_PROFILE__ */
	{
		if(gprofiles[gprofileId].toneSetup.keypadTone==KEYPAD_CLICK)
		{
			PlayIsSettingKeypadVolumeReq(TONE_KEY_CLICK,DEVICE_AUDIO_PLAY_ONCE); //changed by panxu on 200702027 to play the key pad volume
		}
		else if(gprofiles[gprofileId].toneSetup.keypadTone!=KEYPAD_SILENT)
		{
			PlayIsSettingKeypadVolumeReq(TONE_KEY_NORMAL,DEVICE_AUDIO_PLAY_ONCE);//changed by panxu on 200702027 to play the key pad volume
		}
	}
}
/**************************************************************

	FUNCTION NAME		: ChangeKeyVolumeFromIdle(void)

  	PURPOSE				: Call Back function for Volume increase and decrease from Idle screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ChangeKeyVolumeFromIdle(void)  //added by jinzh:20070721
{
	//PlayIsSettingKeypadVolumeReq_fromIdle(TONE_KEY_NORMAL,DEVICE_AUDIO_PLAY_ONCE);
	//chenhe,fix bug #6361,B
	if(gprofiles[gprofileId].toneSetup.keypadTone==KEYPAD_CLICK)
	{
		PlayIsSettingKeypadVolumeReq_fromIdle(TONE_KEY_CLICK,DEVICE_AUDIO_PLAY_ONCE); 
	}
	else if(gprofiles[gprofileId].toneSetup.keypadTone!=KEYPAD_SILENT)
	{
		PlayIsSettingKeypadVolumeReq_fromIdle(TONE_KEY_NORMAL,DEVICE_AUDIO_PLAY_ONCE);
	}
	//chenhe,fix bug #6361,E	
}

/**************************************************************

	FUNCTION NAME		: EntryScrProfilesCommonVolumeLevel(void)

  	PURPOSE				: Set keypad volume level for press of OK on volume level screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrSetCommonVolumeLevel(void)
{
	
	U8* guiBuffer = NULL;       /* Buffer holding history data */
//  MMI_VOLUME_LEVEL_ENUM callvolume;//wug add

	TRACE_EF(g_sw_AUDIO, "gcommonvolumehandler = %d", gcommonvolumehandler);
	guiBuffer = GetCurrGuiBuffer(SCR_PROFILES_RING_TONE_SETUP);
	switch(gcommonvolumehandler)
	{
	case 0:
		{
		gringvol_level = volume_level_UI;
		gprofiles[gprofileId].ringVolumeLevel = (U8)gcommonvolumelevel;
		SetRingVolumeLevel(gringvol_level);
		}
		break;
	case 1:
		{
			isSetKeypadVolume = FALSE;
		gkeypadvol_level = volume_level_UI;
		gprofiles[gprofileId].keypadVolumeLevel = (U8)gcommonvolumelevel;
		SetKeypadVolumeLevel(gkeypadvol_level);
		}
		break;
#if defined(__CALL_VOLUME_LEVEL__)
//wug add used to pc300 start 20071010
	case 2:
		{
		callvolume = volume_level_UI;
		WriteCallToneVal(callvolume);
		}
		break;
//wug add used to pc300 end 20071010
#endif
	}
#ifdef __MMI_MAINLCD_220X176__
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_COMMON_VOLUME_LEVEL );
#else

	GoBackHistory();
#endif

}

/**************************************************************

	FUNCTION NAME		: GoBackCommonVolume(void)

  	PURPOSE				: Function for go back in common volume screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GoBackCommonVolume(void)
{
	MMI_VOLUME_LEVEL_ENUM ringVolumeLevel,keypadVolumelevel;
	isSetKeypadVolume = FALSE;

/**** Restore Volume levels on press of BACK key on volume level screen*****/

	switch(gcommonvolumehandler)
	{
		case 0:
			ringVolumeLevel = gprofiles[gactivatedprofile].ringVolumeLevel;
			SetRingerVolume(ringVolumeLevel);
			break;
		case 1:
			keypadVolumelevel = gprofiles[gactivatedprofile].keypadVolumeLevel;
			SetKeyPadVolume(keypadVolumelevel);
			break;
#if defined(__CALL_VOLUME_LEVEL__)
//wug add used to pc300 start 20071010
    case 2:
      SetSpeechVolume(gcommonvolumelevel);
			break;
//wug add used to pc300 end 20071010
#endif
	}

	GoBackHistory();
}

/**************************************************************

	FUNCTION NAME		: GoIdleScreenCommonVolume(void)

  	PURPOSE				: Go back from Volume screen through End Key

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GoIdleScreenCommonVolume(void)
{
	MMI_VOLUME_LEVEL_ENUM ringVolumeLevel,keypadVolumelevel;
/**** Restore Volume levels on press of END key on volume level screen*****/
	switch(gcommonvolumehandler)
	{
		case 0:
			ringVolumeLevel = gprofiles[gactivatedprofile].ringVolumeLevel;
			SetRingerVolume(ringVolumeLevel);
			break;
		case 1:
			keypadVolumelevel = gprofiles[gactivatedprofile].keypadVolumeLevel;
			SetKeyPadVolume(keypadVolumelevel);
			break;
#if defined(__CALL_VOLUME_LEVEL__)
//wug add used to pc300 start 20071010
    case 2:
      SetSpeechVolume(gcommonvolumelevel);
			break;
//wug add used to pc300 end 20071010
#endif
	}

	DisplayIdleScreen();
}
/**************************************************************

	FUNCTION NAME		: ExitScrProfilesCommonVolumeLevel(void)

  	PURPOSE				: Exit profile Volume Level screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitScrProfilesCommonVolumeLevel(void)
{
	MMI_VOLUME_LEVEL_ENUM ringVolumeLevel,keypadVolumelevel;

	history_t   Scr; /*Variable to hold the history data*/
	S16		nHistory	=	0;
	Scr.scrnID = SCR_PROFILES_COMMON_VOLUME_LEVEL;
	Scr.entryFuncPtr = EntryScrProfilesCommonVolumeLevel;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)&nHistory);
	GetCategoryHistory(Scr.guiBuffer);
	AddHistory(Scr);

	switch(gcommonvolumehandler)
	{
		case 0:
			mdi_audio_stop_all(); // use stop all because it might be play file or play id
			ringVolumeLevel = gprofiles[gactivatedprofile].ringVolumeLevel;
			SetRingerVolume(ringVolumeLevel);
            mdi_audio_resume_background_play();
			//CSD Brian added for play ring tone after MT message, 2003/11/10
			SetProtocolEventHandler(NULL,PRT_EQ_PLAY_AUDIO_FINISH_IND);
			break;
		case 1:
			AudioStopReq((U16)gprofiles[gprofileId].toneSetup.keypadTone);
			keypadVolumelevel = gprofiles[gactivatedprofile].keypadVolumeLevel;
			SetKeyPadVolume(keypadVolumelevel);
            mdi_audio_resume_background_play();
			break;
#if defined(__CALL_VOLUME_LEVEL__)
 //wug add used to pc300 start 20071010
    case 2:
      SetSpeechVolume(gcommonvolumelevel);
			break;
//wug add used to pc300 end 20071010
#endif
	}

}

/**************************************************************

	FUNCTION NAME		: EntryScrProfilesCallAlert(void)

  	PURPOSE				: Showing profile Call Alert screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesCallAlert(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	S8 i=0;
	//U8* PopUpList[MAX_SUB_MENUS];
	MMI_ALERT_TYPE data=0;
	U16 ProfileMenuID[]={
							MENU_RING,
							MENU_VIBRATION_ONLY,
							MENU_VIBRATION_AND_RING,
							MENU_VIBRATION_THEN_RING,
							MENU_SILENT
						};
	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_PROFILES_CALL_ALERT, NULL, EntryScrProfilesCallAlert, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_PROFILES_CALL_ALERT);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_MT_CALL_ALERT_TYPE);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_MT_CALL_ALERT_TYPE);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_MT_CALL_ALERT_TYPE,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_MT_CALL_ALERT_TYPE);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

//CSD Brian modified for showing radio button, 2003/11/15

/****** Get the index of activated alert type menu ID*****/
	i = gprofiles[gprofileId].mtCallAlertTypeEnum ;
	data = GetIndexOfStringId(MENU_MT_CALL_ALERT_TYPE,ProfileMenuID[i-1]);

	/* 8 Display Category Screen */
	ShowCategory11Screen(STR_CALL_ALERT_CAPTION,GetRootTitleIcon(MENU_MT_CALL_ALERT_TYPE),
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						 nNumofItem, nStrItemList,(U16)data,guiBuffer);
//CSD end


	//SetExitHandler(SCR_PROFILES_CALL_ALERT, ExitScrProfilesCallAlert);
}

/**************************************************************
	FUNCTION NAME		: GuardSilentMode(void)
  	PURPOSE				: GuardSilentMode
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: U8
 
**************************************************************/
U8 GuardSilentMode( void )
{
	if( gprofileId == 5 )
	{
		MMI_fixed_list_menu.highlighted_item = GetIndexOfStringId(MENU_MT_CALL_ALERT_TYPE,MENU_VIBRATION_ONLY);
		DisplayPopup( (PU8)GetString( STR_PROFILES_CANNOT_CHANGE_SILENT_MODE_SETTING ), IMG_GLOBAL_WARNING, 0, 1000, (U8)WARNING_TONE );
		return 1;
	}

	return 0;
}

/**************************************************************

	FUNCTION NAME		: SetMtCallAlertRing(void)

  	PURPOSE				: Setting Call Alert type as ring for the current
						  Profile.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetMtCallAlertRing(void)
{
	if( GuardSilentMode() )
		return;
	SetMtCallAlertTypeEnum(MMI_RING);
//CSD Brian modified to skip writing nvram if no change, 2003/11/26
   //SendSetVibrationMode(0);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_CALL_ALERT );
}
/**************************************************************

	FUNCTION NAME		: SetMtCallAlertVibrationOnly(void)

  	PURPOSE				: Setting Call Alert type as ring for the current
						  Profile.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetMtCallAlertVibrationOnly(void)
{
	SetMtCallAlertTypeEnum(MMI_VIBRATION_ONLY);
//CSD Brian modified to skip writing nvram if no change, 2003/11/26
   //SendSetVibrationMode(1);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_CALL_ALERT );
}
/**************************************************************

	FUNCTION NAME		: SetMtCallAlertVibrationAndRing(void)

  	PURPOSE				: Setting Call Alert type as ring for the current
						  Profile.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetMtCallAlertVibrationAndRing(void)
{
	if( GuardSilentMode() )
		return;
	SetMtCallAlertTypeEnum(MMI_VIBRATION_AND_RING);
//CSD Brian modified to skip writing nvram if no change, 2003/11/26
   //SendSetVibrationMode(1);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_CALL_ALERT );
}
/**************************************************************

	FUNCTION NAME		: SetMtCallAlertVibrationThenRing(void)

  	PURPOSE				: Setting Call Alert type as ring for the current
						  Profile.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetMtCallAlertVibrationThenRing(void)
{
	if( GuardSilentMode() )
		return;
	SetMtCallAlertTypeEnum(MMI_VIBRATION_THEN_RING);
//CSD Brian modified to skip writing nvram if no change, 2003/11/26
   //SendSetVibrationMode(1);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_CALL_ALERT );
}
/**************************************************************

	FUNCTION NAME		: SetMtCallAlertSilent(void)

  	PURPOSE				: Setting Call Alert type as ring for the current
						  Profile.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetMtCallAlertSilent(void)
{
	if( GuardSilentMode() )
		return;
	SetMtCallAlertTypeEnum(MMI_SILENT);
//CSD Brian modified to skip writing nvram if no change, 2003/11/26
   //SendSetVibrationMode(0);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_CALL_ALERT );
}

/**************************************************************

	FUNCTION NAME		: EntryScrProfilesRingType(void)

  	PURPOSE				: Showing profile Ring Type screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesRingType(void)
{
	U16 nStrItemList[15]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	U8 i;
	MMI_RING_TYPE_ENUM data=0;
	#if 0 //delete by panxu 20061229, becuase csw don't support MENU_RING_TYPE_ASCENDING setting
	U16 ProfileMenuID[]={
							MENU_RING_TYPE_SINGLE,
							MENU_RING_TYPE_REPEAT,
							MENU_RING_TYPE_ASCENDING
						};
	#endif
	//BEGIN: add by panxu 20061229
	U16 ProfileMenuID[]={
							MENU_RING_TYPE_SINGLE,
							MENU_RING_TYPE_REPEAT
						};
	//END: add by panxu 20061229
	TRACE_EF(g_sw_AUDIO, "MENU_RING_TYPE = %d", MENU_RING_TYPE);
	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_PROFILES_RING_TYPE, NULL, EntryScrProfilesRingType, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_PROFILES_RING_TYPE);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_RING_TYPE);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_RING_TYPE);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_RING_TYPE,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_RING_TYPE);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

/****** Get the index of activated ring type menu ID*****/
	i = gprofiles[gprofileId].ringTypeEnum;
	data = GetIndexOfStringId(MENU_RING_TYPE,ProfileMenuID[i]);

	/* 8 Display Category Screen */
	ShowCategory11Screen(STR_RING_TYPE_CAPTION, GetRootTitleIcon(MENU_RING_TYPE),
						 STR_GLOBAL_OK, 0,
						 STR_GLOBAL_BACK, 0,
						 nNumofItem, nStrItemList,(U16)data,guiBuffer);


	//SetExitHandler(SCR_PROFILES_RING_TYPE, ExitScrProfilesRingType);
}

/**************************************************************

	FUNCTION NAME		: SetRingTypeAscending(void)

  	PURPOSE				: Set Ring Type as Ascending
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetRingTypeAscending(void)
{
	SetRingTypeEnum(ASCENDING);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_RING_TYPE );
}

/**************************************************************

	FUNCTION NAME		: SetRingTypeSingle(void)

  	PURPOSE				: Set Ring Type as Single

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetRingTypeSingle(void)
{
	SetRingTypeEnum(SINGLE);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_RING_TYPE );
}
/**************************************************************

	FUNCTION NAME		: SetRingTypeRepeat(void)

  	PURPOSE				: Set Ring Type as Repeat

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetRingTypeRepeat(void)
{
	SetRingTypeEnum(REPEAT);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_RING_TYPE );
}

#ifdef __MMI_CONNECT_NOTICE__
/**************************************************************

	FUNCTION NAME		: EntryScrProfilesConnectNotice(void)

  	PURPOSE				: Showing profile Call Alert screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesConnectNotice(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	S8 i=0;
	MMI_CONNECT_NOTICE_TYPE data=0;
	U16 ProfileMenuID[]={
                     	MENU_NOTICE_NONE,
                     	MENU_NOTICE_TONE_ONLY,
                     	MENU_NOTICE_VIB_ONLY,
                     	MENU_NOTICE_TONE_AND_VIB
	      					};
	/* 1 Call Exit Handler*/
   EntryNewScreen(SCR_PROFILES_CONNECT_NOTICE, NULL, EntryScrProfilesConnectNotice, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_PROFILES_CONNECT_NOTICE);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_CONNECT_NOTICE);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_CONNECT_NOTICE);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_CONNECT_NOTICE,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_CONNECT_NOTICE);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

//CSD Brian modified for showing radio button, 2003/11/15

/****** Get the index of activated alert type menu ID*****/
	i = gprofiles[gprofileId].extraTone.connectTone;
	data = GetIndexOfStringId(MENU_CONNECT_NOTICE,ProfileMenuID[i]);

	/* 8 Display Category Screen */
	ShowCategory11Screen(STR_CONNECT_NOTICE,IMG_PROFILES_TITLEE,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						 nNumofItem, nStrItemList,(U16)data,guiBuffer);
//CSD end


	//SetExitHandler(SCR_PROFILES_CONNECT_NOTICE, ExitScrProfilesConnectNotice);
}
/**************************************************************

	FUNCTION NAME		: SetConnectNoticeNone(void)

  	PURPOSE				: Setting Connect Notice type as none for the current
						  Profile.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetConnectNoticeNone(void)
{
	SetConnectNotice(MMI_NOTICE_NONE);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_CONNECT_NOTICE );
}
/**************************************************************

	FUNCTION NAME		: SetConnectNoticeToneOnly(void)

  	PURPOSE				: Setting Connect Notice type as tone only for the current
						  Profile.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetConnectNoticeToneOnly(void)
{
	SetConnectNotice(MMI_NOTICE_TONE_ONLY);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_CONNECT_NOTICE );
}
/**************************************************************

	FUNCTION NAME		: SetConnectNoticeVibOnly(void)

  	PURPOSE				: Setting Connect Notice type as vib only for the current
						  Profile.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetConnectNoticeVibOnly(void)
{
	SetConnectNotice(MMI_NOTICE_VIB_ONLY);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_CONNECT_NOTICE );
}
/**************************************************************

	FUNCTION NAME		: SetConnectNoticeToneAndVib(void)

  	PURPOSE				: Setting Connect Notice type as tone and vib for the current
						  Profile.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetConnectNoticeToneAndVib(void)
{
	SetConnectNotice(MMI_NOTICE_TONE_AND_VIB);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_CONNECT_NOTICE );
}
#endif

void ProfileGoBack2History(void)
{
	GoBacknHistory(1);
}

/**************************************************************

	FUNCTION NAME		: EntryScrProfilesAnsweringMode(void)

  	PURPOSE				: Showing profile Answering Mode screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesAnsweringMode(void)
{
	U16 nStrItemList[15]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
    U16 i;
	U16 ParentId;

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_PROFILES_ANSWERING_MODE, NULL, EntryScrProfilesAnsweringMode, NULL);

/***** If earphone is connected then also show auto mode in answering mode list */
	if(gprofileId != 4)
		ParentId=MENU_ANSEWRING_MODE;
	else
		ParentId=MENU_ANSEWRING_MODE_HEADSET;



	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_PROFILES_ANSWERING_MODE);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(ParentId);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(ParentId);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(ParentId,nStrItemList);

	for(i=0;i<nNumofItem;i++)
	{
		memset((S8*) subMenuDataPtrs[i],0,MAX_SUB_MENU_SIZE);
		pfnUnicodeStrncpy((S8*)subMenuDataPtrs[i],GetString(nStrItemList[i]),MAX_SUBMENU_CHARACTERS);
	}

	/* 6 Set current parent id*/
	SetParentHandler(ParentId);

//CSD Brian added for SUBLCD freature, 2003/10/18
#ifdef __MMI_CLAMSHELL__

#ifdef __MMI_MAINLCD_220X176__ 
	fieldStates[0] = (U8)gprofiles[0].answeringMode.coverAnswer;
	fieldStates[1] = (U8)gprofiles[0].answeringMode.anyKey;
	fieldStates[2] = (U8)gprofiles[0].answeringMode.automatic;
#else
	fieldStates[0] = (U8)gprofiles[gprofileId].answeringMode.coverAnswer;
	fieldStates[1] = (U8)gprofiles[gprofileId].answeringMode.anyKey;
	fieldStates[2] = (U8)gprofiles[gprofileId].answeringMode.automatic;
#endif	
#else
	fieldStates[0] = (U8)gprofiles[gprofileId].answeringMode.anyKey;
	fieldStates[1] = (U8)gprofiles[gprofileId].answeringMode.automatic;
#endif


	SetCheckboxToggleRightSoftkeyFunctions(ProfilesAnsweringModePopup,GoBackHistory);

	/* 8 Display Category Screen */

	ShowCategory140Screen(STR_ANSEWRING_MODE_CAPTION,GetRootTitleIcon(ParentId),
						  nNumofItem,(U8**)subMenuDataPtrs,fieldStates, 0, guiBuffer);


	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	//SetExitHandler(SCR_PROFILES_ANSWERING_MODE,ExitScrProfilesAnsweringMode);

}

/**************************************************************

	FUNCTION NAME		: EntryScrSetAnsweringMode(void)

  	PURPOSE				: Set answering mode

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrSetAnsweringMode(void)
{
 MMI_ANSWERING_MODE answeringmode;

//CSD Brian added for SUBLCD freature, 2003/10/18
#ifdef __MMI_CLAMSHELL__

#ifdef __MMI_MAINLCD_220X176__
	answeringmode.coverAnswer = gprofiles[0].answeringMode.coverAnswer = fieldStates[0];
	answeringmode.anyKey = gprofiles[0].answeringMode.anyKey = fieldStates[1];
	answeringmode.automatic = gprofiles[0].answeringMode.automatic = fieldStates[2];
#else
	answeringmode.coverAnswer = gprofiles[gprofileId].answeringMode.coverAnswer = fieldStates[0];
	answeringmode.anyKey = gprofiles[gprofileId].answeringMode.anyKey = fieldStates[1];
	answeringmode.automatic = gprofiles[gprofileId].answeringMode.automatic = fieldStates[2];
#endif	
#else
	answeringmode.anyKey = gprofiles[gprofileId].answeringMode.anyKey = fieldStates[0];
	answeringmode.automatic = gprofiles[gprofileId].answeringMode.automatic = fieldStates[1];
#endif

	SetAnsweringMode(answeringmode);
	//DisplayPopupCheckBox((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_ANSWERING_MODE );
}

/**************************************************************

	FUNCTION NAME		: ProfilesAnsweringModePopup(void)

  	PURPOSE				: Set call back functions for Answering mode.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ProfilesAnsweringModePopup(void)
{
	EntryScrProfilesQueryPopup(EntryScrSetAnsweringMode,ProfileGoBack2History);
}

//CSD Brian added for wrapping for Changable font size, 2003/11/11
#ifdef __MMI_CHANGABLE_FONT__
/**************************************************************

	FUNCTION NAME		: EntryScrProfilesFontSize(void)

  	PURPOSE				: Showing profile Font size screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesFontSize(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	S8  i=0;
	//U8* PopUpList[MAX_SUB_MENUS];
    MMI_FONT_SIZE_ENUM data=0;
	U16 ProfileMenuID[]={
							MENU_FONT_SIZE_THEME_DEFAULT,
							MENU_FONT_SIZE_SMALL,
							MENU_FONT_SIZE_MEDIUM,
							MENU_FONT_SIZE_LARGE,
						};

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_PROFILES_FONT_SIZE, NULL, EntryScrProfilesFontSize, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_PROFILES_FONT_SIZE);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_FONT_SIZE);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_FONT_SIZE);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_FONT_SIZE,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_FONT_SIZE);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

//CSD Brian modified for showing radio button, 2003/11/3
/****** Get the index of activated font size menu ID*****/
	i = gprofiles[gprofileId].fontSizeEnum;
	data = GetIndexOfStringId(MENU_FONT_SIZE,ProfileMenuID[i]);

	/* 8 Display Category Screen */
	ShowCategory11Screen(STR_FONT_SIZE_CAPTION,IMG_PROFILES_TITLEE,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						 nNumofItem, nStrItemList,(U16)data,guiBuffer);
//CSD end


	//SetExitHandler(SCR_PROFILES_FONT_SIZE,ExitScrProfilesFontSize);

}

/**************************************************************

	FUNCTION NAME		: SetFontSizeDefault(void)

  	PURPOSE				: Set Font Size as Default
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetFontSizeDefault(void)
{
	SetFontSizeEnum(THEME_DEFAULT);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_FONT_SIZE );
}
/**************************************************************

	FUNCTION NAME		: SetFontSizeSmall(void)

  	PURPOSE				: Set Font Size as Small
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetFontSizeSmall(void)
{
	SetFontSizeEnum(SMALL);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_FONT_SIZE );
}
/**************************************************************

	FUNCTION NAME		: SetFontSizeMedium(void)

  	PURPOSE				: Set Font Size as Medium
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetFontSizeMedium(void)
{
	SetFontSizeEnum(MEDIUM);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_FONT_SIZE );
}
//CSD Brian added for wrapping for DIALING FONT, 2003/10/31
#ifndef __MMI_DIALLING_FONT__
/**************************************************************

	FUNCTION NAME		: SetFontSizeLarge(void)

  	PURPOSE				: Set Font Size as Large
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetFontSizeLarge(void)
{
	SetFontSizeEnum(LARGE);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_FONT_SIZE );
}
#endif

#endif

#ifdef __MMI_STATUS_LED__
/**************************************************************

	FUNCTION NAME		: EntryScrProfilesLight(void)

  	PURPOSE				: Showing profile Light Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesLight(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	U8* PopUpList[MAX_SUB_MENUS];

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_PROFILES_LIGHT, NULL, EntryScrProfilesLight, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_PROFILES_LIGHT);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_LIGHT);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_LIGHT);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_LIGHT,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_LIGHT);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);


	ConstructHintsList(MENU_LIGHT, PopUpList);

	/* 8 Display Category Screen */
	ShowCategory52Screen(STR_LIGHT_CAPTION,IMG_PROFILES_TITLEE,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem,
						  nStrItemList,
						  (PU16)gIndexIconsImageList,
						  PopUpList ,
						  0,0,
						  guiBuffer);
	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/* 10. Register Exit handler */
	//SetExitHandler(SCR_PROFILES_LIGHT, ExitScrProfilesLight);
}
#endif
/**************************************************************

	FUNCTION NAME		: EntryScrProfilesSetTimer(void)

  	PURPOSE				: Showing profile SetTimer Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesSetTimer(void)
{
	U16 nStrItemList[15]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
    U16 data=0;

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_PROFILES_SET_TIMER, NULL, EntryScrProfilesSetTimer, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_PROFILES_SET_TIMER);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_LIGHT_SETTIMER);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_LIGHT_SETTIMER);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_LIGHT_SETTIMER,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_LIGHT_SETTIMER);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(GetCurrScreenHandler);

	/* 8 Display Category Screen */

#ifdef __MMI_MAINLCD_220X176__ 
	data = gprofiles[0].light.timer.setsec;
#else
	data = gprofiles[gprofileId].light.timer.setsec;
#endif

/****** Set the index as per the selected time******/
#ifdef __MMI_NOKIA_STYLE_N800__
	switch(data)
	{
//	case 5:
//		data = 0;
//		break;
	case 15:
		data = 0;
		break;
	case 30:
		data = 1;
		break;
	case 45:
		data = 2;
		break;
        default:
              data = 0;
	}
#else
	switch(data)
	{
	case 5:
		data = 0;
		break;
	case 15:
		data = 1;
		break;
	case 30:
		data = 2;
		break;
	case 45:
		data = 3;
		break;
	}

#endif

   ShowCategory11Screen(STR_LIGHT_SETTIMER_CAPTION, GetRootTitleIcon(MENU_LIGHT_SETTIMER),
						 STR_GLOBAL_OK, 0,
						 STR_GLOBAL_BACK, 0,
						 nNumofItem, nStrItemList,data,guiBuffer);


    /* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(EntryScrSetTime,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	//SetExitHandler(SCR_PROFILES_SET_TIMER,ExitScrProfilesSetTimer);
}
/**************************************************************

	FUNCTION NAME		: EntryScrSetTime(void)

  	PURPOSE				: Set time

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrSetTime(void)
{
	LIGHT temp;

#ifdef __MMI_NOKIA_STYLE_N800__
	switch(gcurrHiliteIndexOne)
	{
	case 0:
		gSec = temp.timer.setsec = 15;
		gprofiles[gprofileId].light.timer.setsec = gSec;
		set_lcdbacklighttimer();
		break;
	case 1:
		gSec  = temp.timer.setsec = 30;
		set_lcdbacklighttimer();
		break;
	case 2:
		gSec = temp.timer.setsec = 45;
		set_lcdbacklighttimer();
		break;
	//case 3:
	//	gSec = temp.timer.setsec = 45;
	//	set_lcdbacklighttimer();
      //  break;
	}

#else
	switch(gcurrHiliteIndexOne)
	{
	case 0:
		gSec = temp.timer.setsec = 5;

		#ifdef __MMI_MAINLCD_220X176__ 
		gprofiles[0].light.timer.setsec = gSec;
		#else
		gprofiles[gprofileId].light.timer.setsec = gSec;
		#endif
		set_lcdbacklighttimer();
		break;
	case 1:
		gSec  = temp.timer.setsec = 15;
		set_lcdbacklighttimer();
		break;
	case 2:
		gSec = temp.timer.setsec = 30;
		set_lcdbacklighttimer();
		break;
	case 3:
		gSec = temp.timer.setsec = 45;
		set_lcdbacklighttimer();
        break;
	}
    #endif
	//TurnOnBacklight(1); //delete by panxu 20061226 for DisplayPopup will call TurnOnBacklight(1) function
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_SET_TIMER );

}

#ifdef __MMI_STATUS_LED__
/**************************************************************

	FUNCTION NAME		: EntryScrStatusLEDPatterns(void)

  	PURPOSE				: Showing profile Status Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrStatusLEDPatterns(void)
{
	U16 nStrItemList[15]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	MMI_LEDLIGHT_STATUS_ENUM data=0;

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_PROFILES_STATUS, ExitScrStatusLEDPatterns, EntryScrStatusLEDPatterns, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_PROFILES_STATUS);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_LIGHT_STATUS);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_LIGHT_STATUS);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_LIGHT_STATUS,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_LIGHT_STATUS);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(GetCurrScreenHandlerfour);

    data =gprofiles[gprofileId].light.status;

	/* 8 Display Category Screen */
	ShowCategory11Screen(STR_LIGHT_STATUS_CAPTION, IMG_PROFILES_TITLEE,
						STR_GLOBAL_OK, 0,
						STR_GLOBAL_BACK, 0,
					    nNumofItem, nStrItemList,(U16)data, guiBuffer);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/* 10.Register function with Left softkey */
	SetLeftSoftkeyFunction(EntryScrSetLightColor,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	//SetExitHandler(SCR_PROFILES_STATUS,ExitScrStatusLEDPatterns);
}

/**************************************************************

	FUNCTION NAME		: ExitScrStatusLEDPatterns(void)

  	PURPOSE				: Exit profile LED Status Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitScrStatusLEDPatterns(void)
{
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
	StopColorId();
}


/**************************************************************

	FUNCTION NAME		: EntryScrSetLightColor(void)

  	PURPOSE				: Set Light color

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrSetLightColor(void)
{
	MMI_LEDLIGHT_STATUS_ENUM lightstaus;

	switch(gcurrHiliteIndexOne)
	{
		case 0:
			lightstaus = NO_COLOR;
			SetLightStatus(lightstaus );
		break;
		case 1:
			lightstaus = COLOR_PATTERN1;
			SetLightStatus(lightstaus );
		break;
		case 2:
			lightstaus = COLOR_PATTERN2;
			SetLightStatus(lightstaus );
		break;
		case 3:
			lightstaus = COLOR_PATTERN3;
			SetLightStatus(lightstaus );
		break;
		case 4:
			lightstaus = COLOR_PATTERN4;
			SetLightStatus(lightstaus );
		break;
		case 5:
			lightstaus = COLOR_PATTERN5;
			SetLightStatus(lightstaus );
		break;
		case 6:
			lightstaus = COLOR_PATTERN6;
			SetLightStatus(lightstaus );
		break;
	}

	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_STATUS );
}


#endif

/**************************************************************

	FUNCTION NAME		: EntryScrProfilesToneSetup(void)

  	PURPOSE				: Showing profile Tone Setup Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesToneSetup(void)
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
    U8* PopUpList[MAX_SUB_MENUS];

// beginning--added by kecx for fixing bug 12633 on 20090703
#ifdef __MMI_DUAL_PROFILE_SUPPORT__
	mmi_frm_unhide_menu_item(MENU_TONE_SETUP_RING_TONE);
	mmi_frm_unhide_menu_item(MENU_TONE_SETUP_CARD2_RING_TONE);
    	mmi_frm_unhide_menu_item(MENU_TONE_SETUP_MESSAGE_TONE);
	mmi_frm_unhide_menu_item(MENU_TONE_SETUP_CARD2_MESSAGE_TONE);

	if(MTPNP_AD_Is_Flight_Mode())
	{
		mmi_frm_hide_menu_item(MENU_TONE_SETUP_RING_TONE);
		mmi_frm_hide_menu_item(MENU_TONE_SETUP_MESSAGE_TONE);
		mmi_frm_hide_menu_item(MENU_TONE_SETUP_CARD2_RING_TONE);
		mmi_frm_hide_menu_item(MENU_TONE_SETUP_CARD2_MESSAGE_TONE);
	}
	else
	{
		if(!MTPNP_PFAL_Is_Card_Usable(0))
		{
			mmi_frm_hide_menu_item(MENU_TONE_SETUP_RING_TONE);
			mmi_frm_hide_menu_item(MENU_TONE_SETUP_MESSAGE_TONE);
		}
		if(!MTPNP_PFAL_Is_Card_Usable(1))
		{
			mmi_frm_hide_menu_item(MENU_TONE_SETUP_CARD2_RING_TONE);
			mmi_frm_hide_menu_item(MENU_TONE_SETUP_CARD2_MESSAGE_TONE);
		}        
		if(MTPNP_AD_Is_Flight_Mode())
		{
			mmi_frm_hide_menu_item(MENU_TONE_SETUP_CARD2_RING_TONE);
			mmi_frm_hide_menu_item(MENU_TONE_SETUP_CARD2_MESSAGE_TONE);
		}
	}
#endif
// end--added by kecx for fixing bug 12633 on 20090703

#ifdef __MMI_BT_PROFILE__
	mmi_frm_unhide_menu_item(MENU_TONE_SETUP_POWERON_TONE);
	if(gprofileId == MMI_PROFILE_BT)
	{
		mmi_frm_hide_menu_item(MENU_TONE_SETUP_POWERON_TONE);
	}
#endif

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_PROFILES_TONE_SETUP, NULL, EntryScrProfilesToneSetup, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_PROFILES_TONE_SETUP);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild_Ext(MENU_TONE_SETUP);     // modified by kecx for fixing bug 12633 on 20090703

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_TONE_SETUP);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds_Ext(MENU_TONE_SETUP,nStrItemList);     // modified by kecx for fixing bug 12633 on 20090703

	/* 6 Set current parent id*/
	SetParentHandler(MENU_TONE_SETUP);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	/* 8 Display Category Screen */
	ConstructHintsList(MENU_TONE_SETUP, PopUpList);
    ShowCategory52Screen(STR_PROFILES_TONE_SETUP_CAPTION,GetRootTitleIcon(MENU_TONE_SETUP),
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem,
						  nStrItemList,
						  (PU16)gIndexIconsImageList,
						  PopUpList ,
						  0,0,
						  guiBuffer);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/* 10. Register Exit handler */
	//SetExitHandler(SCR_PROFILES_TONE_SETUP,ExitScrProfilesToneSetup);
}

#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
BOOL mmi_profiles_is_melody_from_downloadable_theme( U16 melody_id )
{
	S16 index;
	DYNAUDIOINFO *tones;
	U8 num = GetThemeV2TonesInfo( &tones );
	if( tm_get_current_theme_ringtone() )
	{
		for( index = 0; index < num; index++ )
		{
			if( melody_id == tones[index].AudioId )
				return TRUE;
		}
	}
	return FALSE;
}

/**************************************************************

	FUNCTION NAME		: AudioFileDeletedFromFmgr

  	PURPOSE				: IF audio folder get deleted or fomatted
						  from File manager activate default audio as incoming tone

	INPUT PARAMETERS	: Audio ID

	OUTPUT PARAMETERS	: nil

	RETURNS				: BOOL

 

**************************************************************/
pBOOL AudioFileDeletedFromFmgr(U16 tone_id)
{
	S8 buf[(FMGR_MAX_PATH_LEN+1)*ENCODING_LENGTH];
	FILE_HANDLE file;
	GetAudioNameWithPathInFileSystem(tone_id,(PS8)buf );
	file = MMI_FS_Open((const WCHAR *)buf,FS_READ_ONLY);
	if(file < 0)
	{
		MMI_FS_Close(file);
		return TRUE;
	}
	else
	{
		MMI_FS_Close(file);
		return FALSE;
	}
}
#endif

/**************************************************************

	FUNCTION NAME		: mmi_profiles_get_melody_list(U16 CurMelodyID, U16 *selecteditem)
  	PURPOSE				: Build melody list
	INPUT PARAMETERS	: U16 CurMelodyID, U16 *selecteditem
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
 
**************************************************************/
U8 mmi_profiles_get_melody_list( U16 CurMelodyID, U16 *selecteditem )
{
#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
	DYNEXTMELODYINFO *ExtMelodyInfo;
#endif
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
	DYNAUDIOINFO *DownloadThemeTones;
#endif
	int index, data;
	U16 nNumofItem = gtotalRingId + gtotalMidiId;
	mmi_trace(g_sw_FMGR, "chenxia!!! fun: mmi_profiles_get_melody_list");

	// init hint data
	memset( hintData, 0, sizeof(hintData) );
	for( data = 0; data < MAX_SUB_MENUS; data++ )
		hintDataPtrs[data] = hintData[data];

	//copy the names of fixed list (IMY)
	for( index = 0; index < gtotalRingId; index++ )
		pfnUnicodeStrcpy( (PS8)subMenuDataPtrs[index], GetString( (U16)(STR_PROFILES_RING_1 + index) ) );

	//copy the names of fixed list (MIDI)
	for(index = 0; index < gtotalMidiId; index++ )
		pfnUnicodeStrcpy( (PS8)subMenuDataPtrs[index + gtotalRingId], GetString( (U16)(STR_PROFILES_MIDI_1 + index) ) );

	//make a listing of ext melodies.
#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
	PmgGetExtMelodyInfo( &ExtMelodyInfo );
	for( index = nNumofItem, data = 0; data < MAXIMUM_EXT_MELODY_NUM; index++, data++ )
	{
		if( PmgIsEmptySlot( (U8)data ) == MMI_FALSE )
		{
			if( PmgQueryFileExistBySlot( (U8)data ) == MMI_FALSE )
			{
				pfnUnicodeStrcpy( (S8*) hintData[index], (S8*) GetString(STR_TONE_FILE_NOT_EXIST) );
			
			}
			if( IS_EXTMELODY_SHORTNAME( ExtMelodyInfo->info[data].filepath ) )
			{
				/*Jinzh Modify End  For 6189 Ver: TBM780  on 2007-8-12 14:51 */
				mmi_trace(g_sw_FMGR, "chenxia!!! fun:mmi_profiles_get_melody_list; is short name by slot!");
				mmi_chset_mixed_text_to_ucs2_str( subMenuDataPtrs[index], (FMGR_MAX_FILE_LEN+1) * ENCODING_LENGTH, (U8*)ExtMelodyInfo->info[data].filename, g_chset_text_encoding );
			}
			else
			{
				mmi_trace(g_sw_FMGR, "chenxia!!! fun:mmi_profiles_get_melody_list; is not short name by slot!");
				#if 0//modified by gdm 081128 fix bug10498
				pfnUnicodeStrcpy( (S8*) subMenuDataPtrs[index], (S8*)((ExtMelodyInfo->info[data]).filename) );
				#else
				pfnUnicodeStrncpy((S8*) subMenuDataPtrs[index], (S8*)((ExtMelodyInfo->info[data]).filename), FMGR_MAX_FILE_LEN-1);
				#endif
			}
#if (!defined(__MMI_SHOW_FILE_EXT__))
			PmgRemoveFileExt( (S8*) subMenuDataPtrs[index] );
#elif (!defined(__MMI_SHOW_DAF_FILE_EXT__))
			PmgRemoveFileDAFExt( (S8*) subMenuDataPtrs[index] );
#endif
		}
		else
			{
			pfnUnicodeStrcpy( (S8*) subMenuDataPtrs[index], (S8*) GetString(STR_GLOBAL_EMPTY_LIST) );
			}
	}
#endif // __MMI_PROFILE_EXTMELODY_SUPPORT__

#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
	selected_downloadable_theme_melody_id = 0;
	if( GetThemeV2TonesInfo( &DownloadThemeTones ) != 0 && tm_get_current_theme_ringtone() )
		pfnUnicodeStrcpy((S8*) subMenuDataPtrs[index++],(S8*) GetString(STR_PROFILES_CURR_THEME) );
#endif

	/* find highlight selection */
	if( CurMelodyID >= gstartRingId && CurMelodyID < gstartRingId + gtotalRingId )
		*selecteditem = CurMelodyID - gstartRingId;
	else if( CurMelodyID >= gstartMidiId && CurMelodyID < gstartMidiId + gtotalMidiId )
		*selecteditem = CurMelodyID - gstartMidiId + gtotalRingId;
	else
	{

		BOOL selected = FALSE;
#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
		for( data = 0; data < MAXIMUM_EXT_MELODY_NUM; data++ )
		{
			if( PmgIsEmptySlot( (U8)data) == MMI_FALSE && CurMelodyID == (ExtMelodyInfo->info[data]).id )
			{
				*selecteditem = nNumofItem + data;
				selected = TRUE;
				break;
			}
		}
		nNumofItem += MAXIMUM_EXT_MELODY_NUM;
#endif // __MMI_PROFILE_EXTMELODY_SUPPORT__
#if defined(__MMI_DOWNLOADABLE_THEMES_SUPPORT__)
		if( !selected )
		{
			selected = mmi_profiles_is_melody_from_downloadable_theme( CurMelodyID );
			if( selected )
			{
				*selecteditem = nNumofItem;
				selected_downloadable_theme_melody_id = CurMelodyID;
				if( AudioFileDeletedFromFmgr( CurMelodyID ) )
					pfnUnicodeStrcpy( (S8*) hintData[nNumofItem], GetString(STR_TONE_FILE_NOT_EXIST) );
			}
		}
#endif
 if(!selected)
        {
            /*when there is no mapping selected item, assgin midi1 to both ring tone and alarm tone*/
            gprofiles[gprofileId].toneSetup.ringTone = gstartMidiId ;
            gprofiles[gprofileId].toneSetup.alarmTone = gstartMidiId ;
            *selecteditem = gtotalRingId;
        }
	}

	return (U8)index;
}

extern int checkUsbstate(void);
extern VOID mmi_display_usb_busy(void);
extern BOOL is_phb_init_done(void);

/**************************************************************

	FUNCTION NAME		: mmi_profiles_entry_ring_tone_list(void)

  	PURPOSE				: Showing profile Ring Tone Setup Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_profiles_entry_ring_tone_list(void )
{
	U8* guiBuffer;       /* Buffer holding history data */
	U16 nDispAttribute;  /* Stores display attribue */
	U16 selecteditem =0;
	U8 totalItems=0;
	U16 str_id=0;
	/*Jinzh Add Start For 6095 Ver: TBM780  on 2007-8-13 14:55 */
	if (!is_phb_init_done())  //have no SIM or phb is ready
	{
	    //开机初始化 stringid = 25803  modified by renyh  2008/8/1
		DisplayPopup((PU8)GetString(STR_GLOBAL_POWER_ON_PROCESSING) ,IMG_GLOBAL_PROGRESS, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
		return;
	}
	/*Jinzh Add End  For 6095 Ver: TBM780  on 2007-8-13 14:55 */
	if(checkUsbstate())
	{
		mmi_display_usb_busy();
		return;
	}


	mmi_trace(g_sw_SETTING, TSTXT("###### in mmi_profiles_entry_ring_tone_list() ############\n") ); 
	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_PROFILES_RING_TONE_SETUP, mmi_profiles_exit_ring_tone_list, mmi_profiles_entry_ring_tone_list, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_PROFILES_RING_TONE_SETUP);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_TONE_SETUP_RING_TONE);

	/* 3. Retrieve no of child of menu item to be displayed */
	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	totalItems = mmi_profiles_get_melody_list( gprofiles[gprofileId].toneSetup.ringTone, &selecteditem);
	mmi_trace(g_sw_FMGR, "chenxia!!! fun:mmi_profiles_entry_ring_tone_list; totalItems is %d", totalItems);
	/* 6 Set current parent id*/
	SetParentHandler(MENU_TONE_SETUP_RING_TONE);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(mmi_profiles_melody_list_highlight_hdlr);
	#if defined( __MMI_DUAL_PROFILE_SUPPORT__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	str_id = STR_PROFILES_SIM1_RING_TONE_SETUP_CAPTION;
	#else
	str_id = STR_PROFILES_RING_TONE_SETUP_CAPTION;
	#endif

	/* 8 Display Category Screen */
	ShowCategory89Screen(str_id,
						 GetRootTitleIcon(MENU_TONE_SETUP_RING_TONE),
						 STR_GLOBAL_OK,IMG_GLOBAL_OK,
						 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						 totalItems,subMenuDataPtrs, hintDataPtrs, 0,selecteditem, guiBuffer);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(StopRingToneBack,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(EntryScrSetRingToneId,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}


/**************************************************************

	FUNCTION NAME		: mmi_profiles_exit_ring_tone_list(void)

  	PURPOSE				: Exit profile Ring ToneSetup Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void mmi_profiles_exit_ring_tone_list(void)
{
	StopToneAudio();
}


/**************************************************************

	FUNCTION NAME		: void StopRingToneBack(void)

  	PURPOSE				: StopRingToneBack

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void StopRingToneBack(void)
{
	StopToneAudio();
	GoBackHistory();
}


#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__

void PrfCallBackExtMldReplace(void *path, int is_short)
{
   S8 *filepath = (S8*)path;
   PMG_ADDFILE_ERRNO err = PMG_ADDFILE_ERR_UNKNOWN;

   if(filepath)
   {
#if defined( __DRM_SUPPORT__ ) && defined( MMI_ON_HARDWARE_P )
      if( !mmi_profiles_check_DRM_content_forwardable( (UI_string_type)filepath ) )
         err = PMG_ADDFILE_ERR_DRM_PROHIBTED;
      else
#endif
      if( PmgCheckImyMidFileSize(filepath) == PMG_ADDFILE_ERR_FILE_TOO_LARGE )
      {
         err = PMG_ADDFILE_ERR_FILE_TOO_LARGE;
      }
      else
      {
         PmgSetShortFileName( is_short );
         if(PmgIsEmptySlot(g_ext_melody_idx) == MMI_FALSE)
            err = PmgReplaceExtMelodyBySlotPathFileName( filepath , g_ext_melody_idx);
         else
            err = PmgAddExtMelodyBySlotPathFileName( filepath , g_ext_melody_idx);
      }

      switch( err )
      {
      case PMG_ADDFILE_ERR_SUCCESS:
         DisplayPopup((PU8)GetString(STR_GLOBAL_DONE),
                                                 IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
           break;

      case PMG_ADDFILE_ERR_STR_TOO_LONG:
      case PMG_ADDFILE_ERR_STRFILE_TOO_LONG:
      case PMG_ADDFILE_ERR_STRPATH_TOO_LONG:
          DisplayPopup((PU8)GetString(STR_PROFILES_RING_TONE_SETUP_NAMETOOLONG), \
                                                IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
          break;

      case PMG_ADDFILE_ERR_SLOT_FULL:
          DisplayPopup((PU8)GetString(STR_PROFILES_RING_TONE_SETUP_LISTFULL), \
                                                IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
          break;

      case PMG_ADDFILE_ERR_FILE_TOO_LARGE:
          DisplayPopup((PU8)GetString(STR_PROFILES_RING_TONE_SETUP_FILETOOLARGE), \
                                                IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
          break;

      case PMG_ADDFILE_ERR_DUPLICATED:
          DisplayPopup((PU8)GetString(STR_PROFILES_RING_TONE_SETUP_DUPLICATED), \
                                                IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
          break;
#if defined( __DRM_SUPPORT__ ) && defined( MMI_ON_HARDWARE_P )
      case PMG_ADDFILE_ERR_DRM_PROHIBTED:
          DisplayPopup( (PU8)GetString( STR_GLOBAL_DRM_PROHIBITED ), \
		  	IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
	   break;
#endif
      default:
          DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED), \
                                                IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
          break;
       }

       DeleteUptoScrID(MENU_TONE_SETUP_RING_TONE_EXT_MELODY);
       DeleteScreenIfPresent( MENU_TONE_SETUP_RING_TONE_EXT_MELODY );

   }
   else
   {
       if( GetExitScrnID() == POPUP_SCREENID )
       {
          DeleteUptoScrID(MENU_TONE_SETUP_RING_TONE_EXT_MELODY);
          DeleteScreenIfPresent( MENU_TONE_SETUP_RING_TONE_EXT_MELODY );
       }
	else
          GoBackToHistory(MENU_TONE_SETUP_RING_TONE_EXT_MELODY);
   }


}

static U8 PrfSetRingToneConvertEmptySlotMenuList( U8 idx )
{
   if(PmgIsEmptySlot(g_ext_melody_idx) == MMI_FALSE)
   { /* Not Empty slot */
      return idx;
   }
   else
   {
      if( idx == 0 )
        return PRF_MENU_EXTMLD_ADD; /* PRF_MENU_EXTMLD_DONE */

#if MMI_ON_HARDWARE_P
      else
         MMI_ASSERT( MMI_FALSE );
#endif
   }

   return PRF_MENU_EXTMLD_MAX;
}

extern void mmi_show_qq_select_confirm(void (*default_entry) (void));
extern BOOL checkVMOn(void);

void PrfSetRingToneExtMelodyDrvSelect(void)
{
#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__) )
        S16 phoneDrvLetter, cardDrvLetter;
        S8 temp[3];
        UI_character_type dir_phone_root[4];
        UI_character_type dir_card_root[4];
	FMGR_FILTER filter;
        /*+ zhouqin modify for dual t-flash card 20110617*/
        #if defined(DUAL_TFLASH_SUPPORT)
        S16  seccardDrvLetter;
        UI_character_type dir_sec_card_root[4];
        #endif
        /*- zhouqin modify for dual t-flash card 20110617*/
	if(checkVMOn())
	{
	    mmi_show_qq_select_confirm(PrfSetRingToneExtMelodyDrvSelect);
		return;
	}

        FMGR_FILTER_INIT(&filter);
	FMGR_FILTER_SET_AUDIO(&filter);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER);
	FMGR_FILTER_SET(&filter, FMGR_TYPE_FOLDER_DOT);

         //#define MMI_PUBLIC_DRV  FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM)
         //#define MMI_CARD_DRV  FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE)
        phoneDrvLetter = MMI_FS_GetDrive( FS_DRIVE_V_NORMAL, 2, FS_DRIVE_I_SYSTEM | FS_DRIVE_V_NORMAL );
        cardDrvLetter = MMI_FS_GetDrive( FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE );
        /*+ zhouqin modify for dual t-flash card 20110617*/
        #if defined(DUAL_TFLASH_SUPPORT)
        seccardDrvLetter = MMI_FS_GetDrive( FS_DRIVE_TFCARD_SECOND, 1, FS_NO_ALT_DRIVE );
        sprintf( temp, "%c:/", (S8) seccardDrvLetter );	//change by liwh:20070731
        AnsiiToUnicodeString( (S8*)dir_sec_card_root, temp );
        #endif
        /*- zhouqin modify for dual t-flash card 20110617*/
        sprintf( temp, "%c:/", (S8) phoneDrvLetter );   //change by liwh:20070731
        AnsiiToUnicodeString( (S8*)dir_phone_root, temp );
        sprintf( temp, "%c:/", (S8) cardDrvLetter );	//change by liwh:20070731
        AnsiiToUnicodeString( (S8*)dir_card_root, temp );

   if( extMldHlitItem == PRF_MENU_EXTMLD_DRV_CARD)
	{
#if defined(__MMI_PHB_CALLER_RES_SETTING__)	/*Forward res to phonebook*/
		if( prfset_conf_type == PRFSET_MELODY_TYPE_PHB_RING)
		{
			mmi_fmgr_select_path_and_enter (APP_PHONEBOOK, FMGR_SELECT_FILE, \
                                      filter, \
                                      (PS8)dir_card_root, PrfCallBackExtMldReplace);
		}
		else
#endif         	  
      mmi_fmgr_select_path_and_enter (APP_PROFILES, FMGR_SELECT_FILE,  \
                                      filter,  \
                                      (PS8)dir_card_root, PrfCallBackExtMldReplace);
	}
/*+ zhouqin modify for dual t-flash card 20110617*/
#if defined(DUAL_TFLASH_SUPPORT)
else if( extMldHlitItem == PRF_MENU_EXTMLD_DRV_SEC_CARD)
	{
#if defined(__MMI_PHB_CALLER_RES_SETTING__)	/*Forward res to phonebook*/
		if( prfset_conf_type == PRFSET_MELODY_TYPE_PHB_RING)
		{
			mmi_fmgr_select_path_and_enter (APP_PHONEBOOK, FMGR_SELECT_FILE, \
                                      filter, \
                                      (PS8)dir_sec_card_root, PrfCallBackExtMldReplace);
		}
		else
#endif         	  
      mmi_fmgr_select_path_and_enter (APP_PROFILES, FMGR_SELECT_FILE,  \
                                      filter,  \
                                      (PS8)dir_sec_card_root, PrfCallBackExtMldReplace);
	}
#endif
/*- zhouqin modify for dual t-flash card 20110617*/
   else /* if( extMldHlitItem == PRF_MENU_EXTMLD_DRV_PHONE) */
	{
#if defined(__MMI_PHB_CALLER_RES_SETTING__)	/*Forward res to phonebook*/
		if( prfset_conf_type == PRFSET_MELODY_TYPE_PHB_RING)
		{
			mmi_fmgr_select_path_and_enter (APP_PHONEBOOK, FMGR_SELECT_FILE, \
                                      filter, \
                                      (PS8)dir_phone_root, PrfCallBackExtMldReplace);
		}
		else
#endif         	  
      mmi_fmgr_select_path_and_enter (APP_PROFILES, FMGR_SELECT_FILE, \
                                      filter, \
                                      (PS8)dir_phone_root, PrfCallBackExtMldReplace);
	}

#endif /* (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__)  */
}

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
#endif
extern int checkUsbstate(void);
extern VOID mmi_display_usb_busy(void);


/*****************************************************************************
 * FUNCTION
 *  EntryPrfSetRingToneExtMelodyDrv
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryPrfSetRingToneExtMelodyDrv(void)
{

    U8 *nStrItemList[PRF_MENU_EXTMLD_DRV_MAX];
    U8 nNumofItem = (U8) PRF_MENU_EXTMLD_DRV_MAX;
    U8 *guiBuffer;
    S16 CardDrv = -1;
    /*+ zhouqin modify for dual t-flash card 20110617*/
    #if defined(DUAL_TFLASH_SUPPORT)
    S16 SecCardDrv = -1;
    #endif
    /*- zhouqin modify for dual t-flash card 20110617*/
	if(checkUsbstate()) //chenhe add for bug 9640
	{
		mmi_display_usb_busy();
		return;
	}

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__))
    // #define MMI_CARD_DRV  FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE)
       CardDrv = MMI_FS_GetDrive( FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE );
       /*+ zhouqin modify for dual t-flash card 20110617*/
       #if defined(DUAL_TFLASH_SUPPORT)
        SecCardDrv = MMI_FS_GetDrive( FS_DRIVE_TFCARD_SECOND, 1, FS_NO_ALT_DRIVE );
       #endif
       /*- zhouqin modify for dual t-flash card 20110617*/
#endif /* (defined(MMI_ON_HARDWARE_P) && defined(__MMI_FILE_MANAGER__)) */ 

    if (CardDrv > 0
         /*+ zhouqin modify for dual t-flash card 20110617*/
       #if defined(DUAL_TFLASH_SUPPORT)
        || (SecCardDrv>0)
       #endif
       /*- zhouqin modify for dual t-flash card 20110617*/
       )    /* Card Storage is present */
    {
        U16 StrIDTitle = 0;

        if (prfset_conf_type == PRFSET_MELODY_TYPE_RING)
        {
             #if defined( __MMI_DUAL_PROFILE_SUPPORT__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
                StrIDTitle = STR_PROFILES_SIM1_RING_TONE_SETUP_CAPTION;    
             #else  //Incoming Call
                StrIDTitle = STR_PROFILES_RING_TONE_SETUP_CAPTION;    
             #endif	
        }
	#ifdef __MMI_DUAL_PROFILE_SUPPORT__
        else if(prfset_conf_type == PRFSET_MELODY_TYPE_CARD2_RING)
        {
                StrIDTitle = STR_PROFILES_SIM2_RING_TONE_SETUP_CAPTION;    
        }
    #endif
    #ifndef __MMI_ALM_CUSTOMIZE_TONE__
        else if (prfset_conf_type == PRFSET_MELODY_TYPE_ALARM)
        {
            StrIDTitle = STR_ALARM_TONE_CAPTION;
        }
    #endif /* __MMI_ALM_CUSTOMIZE_TONE__ */ 
    #if defined(__MMI_PHB_CALLER_RES_SETTING__)     /* Forward res to phonebook */
        else if (prfset_conf_type == PRFSET_MELODY_TYPE_PHB_RING)
        {
            StrIDTitle = STR_ID_PHB_CALLER_RING;
        }
    #endif /* defined(__MMI_PHB_CALLER_RES_SETTING__) */ 
        else
        {
            ASSERT(0);
        }

        nStrItemList[PRF_MENU_EXTMLD_DRV_PHONE] = (U8*) GetString(STR_PROFILES_RING_TONE_SETUP_DRV_PHONE);
        nStrItemList[PRF_MENU_EXTMLD_DRV_CARD] = (U8*) GetString(STR_PROFILES_RING_TONE_SETUP_DRV_CARD);
        /*+ zhouqin modify for dual t-flash card 20110617*/
        #if defined(DUAL_TFLASH_SUPPORT)
        nStrItemList[PRF_MENU_EXTMLD_DRV_SEC_CARD] = (U8*) GetString(STR_PROFILES_RING_TONE_SETUP_DRV_SEC_CARD);
        #endif
        /*- zhouqin modify for dual t-flash card 20110617*/
        EntryNewScreen(MENU_TONE_SETUP_RING_TONE_EXT_MELODY_DRV, NULL, EntryPrfSetRingToneExtMelodyDrv, NULL);

        guiBuffer = GetCurrGuiBuffer(MENU_TONE_SETUP_RING_TONE_EXT_MELODY_DRV);

        RegisterHighlightHandler(HighlightPrfSetRingToneExtMelody);

        ShowCategory56Screen(
            StrIDTitle,
            (U16) (prfset_conf_type == PRFSET_MELODY_TYPE_PHB_RING ? IMG_SCR_PBOOK_CAPTION : IMG_PROFILES_TITLEE),
            STR_GLOBAL_OK,
            IMG_GLOBAL_OK,
            STR_GLOBAL_BACK,
            IMG_GLOBAL_BACK,
            (S32) nNumofItem,
            nStrItemList,
            NULL,
            0,
            guiBuffer);

        SetLeftSoftkeyFunction(PrfSetRingToneExtMelodyDrvSelect, KEY_EVENT_UP);
        SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

        /* SetExitHandler (MENU_TONE_SETUP_RING_TONE_EXT_MELODY_DRV, ExitPrfSetRingToneExtMelodyDrv); */
    }
    else    /* Card Storage not present */
    {
        extMldHlitItem = PRF_MENU_EXTMLD_DRV_PHONE;
        PrfSetRingToneExtMelodyDrvSelect();
    }
}

void PrfSetRingToneExtMelody(void)
{
   U8 idx = PrfSetRingToneConvertEmptySlotMenuList( extMldHlitItem );

   if( idx == PRF_MENU_EXTMLD_DONE )
   {
      if(PmgIsEmptySlot(g_ext_melody_idx) == MMI_FALSE)
      {
         if( prfset_conf_type == PRFSET_MELODY_TYPE_RING)
            SetRingToneID( (U16)(PMG_EXT_MELODY_BEGIN+g_ext_melody_idx) );
	#ifdef __MMI_DUAL_PROFILE_SUPPORT__
         else if(prfset_conf_type == PRFSET_MELODY_TYPE_CARD2_RING)
            {
                SetCard2RingToneID((U16)(PMG_EXT_MELODY_BEGIN + g_ext_melody_idx));
            }
    #endif	/* __MMI_DUAL_PROFILE_SUPPORT__ */
#ifndef __MMI_ALM_CUSTOMIZE_TONE__
         else if(prfset_conf_type == PRFSET_MELODY_TYPE_ALARM )
            SetAlarmToneID( (U16)(PMG_EXT_MELODY_BEGIN+g_ext_melody_idx) );
#endif
#if defined(__MMI_PHB_CALLER_RES_SETTING__)	/*Forward res to phonebook*/
         else if(prfset_conf_type == PRFSET_MELODY_TYPE_PHB_RING)
         	  mmi_phb_caller_ring_forward_to_entry((U16)(PMG_EXT_MELODY_BEGIN+g_ext_melody_idx));
#endif
         else
            MMI_ASSERT(0);

         if(prfset_conf_type != PRFSET_MELODY_TYPE_PHB_RING)
	         DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
       }
      else
         DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);

   }
   else if( idx == PRF_MENU_EXTMLD_DEL )
   {
         if( PmgDelExtMelodyById( (U16)(PMG_EXT_MELODY_BEGIN+g_ext_melody_idx) ) == MMI_TRUE )
            DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
         else
            DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
   }
   else if( idx == PRF_MENU_EXTMLD_ADD )
   {
         EntryPrfSetRingToneExtMelodyDrv();
   }
   else
   {
         DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
   }

}


void HighlightPrfSetRingToneExtMelody(S32 index)
{
	extMldHlitItem = (U8)index;
}

void EntryPrfSetRingToneExtMelody( void )
{
	U8 *nStrItemList[PRF_MENU_EXTMLD_MAX];
	U8 nNumofItem = 0;
	U8* guiBuffer;
	U16 StrIDToneSet=0, StrIDToneDel = 0, StrIDToneAdd = 0, StrIDTitle = 0;

	if( prfset_conf_type == PRFSET_MELODY_TYPE_RING ||
	    prfset_conf_type == PRFSET_MELODY_TYPE_PHB_RING ||
	    prfset_conf_type == PRFSET_MELODY_TYPE_CARD2_RING)
	{
	   if(prfset_conf_type == PRFSET_MELODY_TYPE_PHB_RING)
	   {
		   StrIDToneSet = STR_TONE_SETUP_RING_TONE_PHB_EXT_SET;
#if defined(__MMI_PHB_CALLER_RES_SETTING__)	/*Forward res to phonebook*/
		   StrIDTitle = STR_ID_PHB_CALLER_RING;
#endif
	   }
	   else
	   {
		   StrIDToneSet = STR_TONE_SETUP_RING_TONE_EXT_SET;
		 #if defined( __MMI_DUAL_PROFILE_SUPPORT__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
		   	if(prfset_conf_type == PRFSET_MELODY_TYPE_CARD2_RING)
			{
               		StrIDTitle = STR_PROFILES_SIM2_RING_TONE_SETUP_CAPTION;    
            		}
            		else if(prfset_conf_type == PRFSET_MELODY_TYPE_RING) //SIM1 Incoming Call
			{
               		StrIDTitle = STR_PROFILES_SIM1_RING_TONE_SETUP_CAPTION;    
            		}
		 #else
			if(prfset_conf_type == PRFSET_MELODY_TYPE_RING) //Incoming Call
			{
				   StrIDTitle = STR_PROFILES_RING_TONE_SETUP_CAPTION;
            		}
              #endif
	    }
	   StrIDToneDel = STR_TONE_SETUP_RING_TONE_EXT_DEL;
	   StrIDToneAdd = STR_TONE_SETUP_RING_TONE_EXT_ADD;
	}
#ifndef __MMI_ALM_CUSTOMIZE_TONE__
	else
	{
	   StrIDToneSet = STR_TONE_SETUP_ALARM_TONE_EXT_SET;
	   StrIDToneDel = STR_TONE_SETUP_ALARM_TONE_EXT_DEL;
	   StrIDToneAdd = STR_TONE_SETUP_ALARM_TONE_EXT_ADD;
	   StrIDTitle = STR_ALARM_TONE_CAPTION;
	}
#endif

     if(PmgIsEmptySlot(g_ext_melody_idx) == MMI_FALSE)
     {
        /* Not Empty Slot */
        nStrItemList[PRF_MENU_EXTMLD_DONE] = (U8 *)GetString( StrIDToneSet );
        nStrItemList[PRF_MENU_EXTMLD_DEL] = (U8 *)GetString( StrIDToneDel );
        nStrItemList[PRF_MENU_EXTMLD_ADD] = (U8 *)GetString( StrIDToneAdd );
        nNumofItem = (U8)PRF_MENU_EXTMLD_MAX;
     }
     else /* Empty slot, show replace only */
     {
        nStrItemList[0] = (U8 *)GetString( StrIDToneAdd );
        nNumofItem = 1;
     }

	EntryNewScreen(MENU_TONE_SETUP_RING_TONE_EXT_MELODY, NULL, EntryPrfSetRingToneExtMelody, NULL);

	guiBuffer = GetCurrGuiBuffer (MENU_TONE_SETUP_RING_TONE_EXT_MELODY);

	RegisterHighlightHandler (HighlightPrfSetRingToneExtMelody);

	ShowCategory56Screen (StrIDTitle,
							GetRootTitleIcon(MAIN_MENU_PROFILES_MENUID),
							STR_GLOBAL_OK,IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							(S32)nNumofItem, nStrItemList, NULL, 0, guiBuffer);

	SetLeftSoftkeyFunction(PrfSetRingToneExtMelody,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	//SetExitHandler (MENU_TONE_SETUP_RING_TONE_EXT_MELODY, ExitPrfSetRingToneExtMelody);
}

void PrfSetRingToneConfType(PRFSET_MELODY_CONF_TYPE type)
{
	prfset_conf_type = type;
}

#endif

/**************************************************************

	FUNCTION NAME		: EntryScrSetRingToneId(void)

  	PURPOSE				: Setting Ringtone ID.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrSetRingToneId(void)
{
	TRACE_SYS_FUNCTION();
	EntryScrSetRingToneIdCountId(EntryScrSetRingToneId_Ext);
}

/**************************************************************

	FUNCTION NAME		: EntryScrSetRingToneIdCountId(void)

  	PURPOSE				: Count Ringtone ID.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrSetRingToneIdCountId( void (*Func_Callback)(U8, U16) )
{
	U8 is_ext_melody = MMI_FALSE;
	U16 numOfItem = gtotalRingId + gtotalMidiId;
	U16 ring_id = 0;

	if( g_profiles_melody_list_index < gtotalRingId )
		ring_id = g_profiles_melody_list_index + gstartRingId ;
	else if( g_profiles_melody_list_index < numOfItem )
		ring_id = g_profiles_melody_list_index + gstartMidiId - gtotalRingId;
#if defined(__MMI_PROFILE_EXTMELODY_SUPPORT__)
	else if( g_profiles_melody_list_index < numOfItem + MAXIMUM_EXT_MELODY_NUM )
	{
		g_ext_melody_idx = (U8)(g_profiles_melody_list_index - numOfItem);
		ring_id = (U16)g_ext_melody_idx + PMG_EXT_MELODY_BEGIN;
		is_ext_melody = MMI_TRUE;
	}
#endif
	else
	{
#if defined( __MMI_DOWNLOADABLE_THEMES_SUPPORT__ )
		BOOL selected = FALSE;
#endif
#if defined(__MMI_PROFILE_EXTMELODY_SUPPORT__)
		numOfItem += MAXIMUM_EXT_MELODY_NUM;
#endif
#if defined( __MMI_DOWNLOADABLE_THEMES_SUPPORT__ )
		if( g_profiles_melody_list_index == numOfItem )
		{
			if( selected_downloadable_theme_melody_id != 0 )
			{
				ring_id = selected_downloadable_theme_melody_id;
				selected = TRUE;
			}
		}
		ASSERT( selected );
#endif
	}
	Func_Callback( is_ext_melody, ring_id );
}

/**************************************************************

	FUNCTION NAME		: EntryScrSetRingToneId_Ext(void)

  	PURPOSE				: Setting Ringtone ID.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrSetRingToneId_Ext(U8 ExtMelodyFlag, U16 ringTone)
{

#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
	if( ExtMelodyFlag == MMI_TRUE )
	{		
		prfset_conf_type = PRFSET_MELODY_TYPE_RING;
		EntryPrfSetRingToneExtMelody();
	}
	else
#endif
	{
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
		SetRingToneID(ringTone);
		DeleteScreenIfPresent( SCR_PROFILES_RING_TONE_SETUP );
	}
}

#ifdef __MMI_DUAL_PROFILE_SUPPORT__  //Add by Renyh for 221 project   20090213
/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_entry_card2_ring_tone_list
 * DESCRIPTION
 *  Showing profile Card2 Ring Tone Setup Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_entry_card2_ring_tone_list(void)
{

    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    U8 *guiBuffer;      /* Buffer holding history data */
    U16 nDispAttribute;  /* Stores display attribue */
    U16 selecteditem = 0;
    U8 totalItems = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* 1 Call Exit Handler */
    	/*Jinzh Add Start For 6095 Ver: TBM780  on 2007-8-13 14:55 */
	if (!is_phb_init_done())  //have no SIM or phb is ready
	{
	    //开机初始化 stringid = 25803  modified by renyh  2008/8/1
		DisplayPopup((PU8)GetString(STR_GLOBAL_POWER_ON_PROCESSING) ,IMG_GLOBAL_PROGRESS, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
		return;
	}
	/*Jinzh Add End  For 6095 Ver: TBM780  on 2007-8-13 14:55 */
	if(checkUsbstate())
	{
		mmi_display_usb_busy();
		return;
	}
	EntryNewScreen(SCR_PROFILES_CARD2_RING_TONE_SETUP,
                   mmi_profiles_exit_card2_ring_tone_list, 
                   mmi_profiles_entry_card2_ring_tone_list, 
                   NULL);

    /* 2 Get current screen to gui buffer  for history purposes */
    guiBuffer = GetCurrGuiBuffer(SCR_PROFILES_CARD2_RING_TONE_SETUP);

    /* 3. Retrieve no of child of menu item to be displayed */
    /* 4. Get attribute of menu to be displayed */
    nDispAttribute = GetDispAttributeOfItem(MENU_TONE_SETUP_CARD2_MESSAGE_TONE);
	
    /* 5. Retrieve string ids in sequence of given menu item to be displayed */
    totalItems = mmi_profiles_get_melody_list(gprofiles[gprofileId].toneSetup.card2_ringTone, &selecteditem);

    /* 6 Set current parent id */
    SetParentHandler(MENU_TONE_SETUP_CARD2_RING_TONE);

    /* 7 Register highlight handler to be called in menu screen */
    RegisterHighlightHandler(mmi_profiles_melody_list_highlight_hdlr);

    /* 8 Display Category Screen */
    ShowCategory89Screen(STR_PROFILES_SIM2_RING_TONE_SETUP_CAPTION,
					#ifdef __MMI_DUAL_PROFILE_SUPPORT__
	                                GetRootTitleIcon(MENU_TONE_SETUP_CARD2_RING_TONE),
                                 #else
						GetRootTitleIcon(MENU_TONE_SETUP_CARD2_RING_TONE),								 
					#endif
					     STR_GLOBAL_OK, 
       					     IMG_GLOBAL_OK,
     					     STR_GLOBAL_BACK, 
       					     IMG_GLOBAL_BACK,
       					     totalItems, subMenuDataPtrs, hintDataPtrs,0,selecteditem,guiBuffer);

    /* 9.Register function with right softkey */
    SetRightSoftkeyFunction(StopRingToneBack, KEY_EVENT_UP);
    SetLeftSoftkeyFunction(mmi_profiles_entry_card2_ring_tone_list_hdlr, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_exit_card2_ring_tone_list
 * DESCRIPTION
 *  Exit profile Card2 Ring ToneSetup Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_exit_card2_ring_tone_list(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    StopToneAudio();
}
/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_set_card2_ring_tone_id_callback
 * DESCRIPTION
 *  set card2 incoming call tone id callback.
 * PARAMETERS
 *  ExtMelodyFlag       [IN]        
 *  ringTone            [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_set_card2_ring_tone_id_callback(U8 ExtMelodyFlag, U16 ringTone)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
    if (ExtMelodyFlag == MMI_TRUE)
    {
        prfset_conf_type = PRFSET_MELODY_TYPE_CARD2_RING;
        EntryPrfSetRingToneExtMelody();
    }
    else
#endif /* __MMI_PROFILE_EXTMELODY_SUPPORT__ */ 
    {
        DisplayPopup(
            (PU8) GetString(STR_GLOBAL_DONE),
            IMG_GLOBAL_ACTIVATED,
            1,
            UI_POPUP_NOTIFYDURATION_TIME,
            SUCCESS_TONE);
        
        SetCard2RingToneID(ringTone);
        DeleteScreenIfPresent(SCR_PROFILES_CARD2_RING_TONE_SETUP);
     }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_entry_card2_ring_tone_list_hdlr
 * DESCRIPTION
 *  card2 ring tone list hdlr
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_entry_card2_ring_tone_list_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryScrSetRingToneIdCountId(mmi_profiles_set_card2_ring_tone_id_callback);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_entry_card2_message_tone
 * DESCRIPTION
 *  Entry function for Card2 Message Tone Setup
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_entry_card2_message_tone(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 UnicodenStrItemList[10]; /* Stores the strings id of submenus returned */
    U8 AsciinStrItemList[4];    /* Stores the strings id of submenus returned */
    U16 nNumofItem;             /* Stores no of children in the submenu */
    U8 *guiBuffer;              /* Buffer holding history data */
    U8 i = 0;
    MESSAGE_TONE_ENUM data = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* 1 Call Exit Handler */
    EntryNewScreen(SCR_CARD2_MESSAGE_TONE_SETUP, 
                            mmi_profiles_exit_card2_message_tone, 
                            mmi_profiles_entry_card2_message_tone,
                            NULL);

    /* 2 Get current screen to gui buffer  for history purposes */
    guiBuffer = GetCurrGuiBuffer(SCR_CARD2_MESSAGE_TONE_SETUP);

    /* 3. Retrieve no of child of menu item to be displayed */
    nNumofItem = gtotalMiscToneId + 1;

    /* 5. Retrieve string ids in sequence of given menu item to be displayed */
    memset((S8*) subMenuDataPtrs[0], 0, MAX_SUB_MENU_SIZE);
    pfnUnicodeStrncpy((S8*) subMenuDataPtrs[0], (S8*) GetString(STR_PROFILES_SILENT), MAX_SUBMENU_CHARACTERS);

    for (i = 1; i <= gtotalMiscToneId; i++)
    {
        S32 len = 0;

        memset((S8*) subMenuDataPtrs[i], 0, MAX_SUB_MENU_SIZE);
        sprintf((PS8) AsciinStrItemList, " %d", i);
        AnsiiToUnicodeString((PS8) UnicodenStrItemList, (PS8) AsciinStrItemList);
	len=pfnUnicodeStrlen((PS8)UnicodenStrItemList);
	len=MAX_SUBMENU_CHARACTERS-len;
	pfnUnicodeStrncpy((S8*)subMenuDataPtrs[i], ( S8*)GetString(STR_PROFILES_TONE),MAX_SUBMENU_CHARACTERS);
	pfnUnicodeStrcat((PS8)subMenuDataPtrs[i],(PS8)UnicodenStrItemList);

    }

    /* 6 Set current parent id */
    SetParentHandler(MENU_TONE_SETUP_CARD2_MESSAGE_TONE);

    /* 7 Register highlight handler to be called in menu screen */

    RegisterHighlightHandler(GetCurrScreenHandlersix);

    /* 8 Display Category Screen */
    if (gprofiles[gprofileId].toneSetup.card2_messageTone != MESSAGE_SILENT)
    {
        data = (gprofiles[gprofileId].toneSetup.card2_messageTone - gstartMiscToneId) + 1;
    }
    else
    {
        data = 0;
    }

    ShowCategory89Screen(
        STR_PROFILES_SIM2_MESSAGE_TONE_SETUP_CAPTION,
	#ifdef __MMI_DUAL_PROFILE_SUPPORT__
	          GetRootTitleIcon(MENU_TONE_SETUP_CARD2_MESSAGE_TONE),
       #else
		GetRootTitleIcon(MENU_TONE_SETUP_CARD2_MESSAGE_TONE),								 
	 #endif
	 STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        nNumofItem,
        subMenuDataPtrs,
        NULL,
        0,
        (U16) data,
        guiBuffer);

    /* 9.Register function with right softkey */
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

    SetLeftSoftkeyFunction(mmi_profiles_entry_card2_message_tone_hdlr, KEY_EVENT_UP);

    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_exit_card2_message_tone
 * DESCRIPTION
 *  Exit profile Card2 Ring ToneSetup Screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_exit_card2_message_tone(void)
{
	if(glastaudioId != POWERON_SILENT);
		StopToneAudio();
}

/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_entry_card2_message_tone_hdlr
 * DESCRIPTION
 *  card2 message tone hdlr
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_entry_card2_message_tone_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    TONE_SETUP tonesetup;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (gcurrHiliteIndexOne == 0)
    {
        tonesetup.card2_messageTone = MESSAGE_SILENT;
    }
    else
    {
        tonesetup.card2_messageTone = (gcurrHiliteIndexOne + gstartMiscToneId) - 1;
    }

    DisplayPopup((PU8) GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);
    SetCard2MessageToneType(tonesetup);
    DeleteScreenIfPresent(SCR_CARD2_MESSAGE_TONE_SETUP);
}
#endif                                         //Add by Renyh for 221 project   20090213

/**************************************************************

	FUNCTION NAME		: EntryScrProfilesPoweronToneSetup(void)

  	PURPOSE				: Showing profile Poweron Tone Setup Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesPoweronToneSetup(void)
{
	U8 UnicodenStrItemList[10]; /* Stores the strings id of submenus returned */
	U8 AsciinStrItemList[4]; /* Stores the strings id of submenus returned */

	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	U8 i=0;
    POWERON_TONE_ENUM data=0;


	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_POWERON_TONE_SETUP, ExitScrProfilesPoweronToneSetup, EntryScrProfilesPoweronToneSetup, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_POWERON_TONE_SETUP);

    /* 3. Retrieve no of child of menu item to be displayed */
	//nNumofItem = GetNumOfChild(MENU_TONE_SETUP_POWERON_TONE);
	nNumofItem = gtotalGeneralToneId+1;

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_TONE_SETUP_POWERON_TONE);

	memset((S8*) subMenuDataPtrs[0],0,MAX_SUB_MENU_SIZE);
	pfnUnicodeStrncpy((S8*)subMenuDataPtrs[0], ( S8*)GetString(STR_PROFILES_SILENT),MAX_SUBMENU_CHARACTERS);

	for(i=1;i<=gtotalGeneralToneId;i++)
	{
		S32 len=0;
		memset((S8*) subMenuDataPtrs[i],0,MAX_SUB_MENU_SIZE);
		sprintf((PS8)AsciinStrItemList , "%d",i);
		AnsiiToUnicodeString((PS8)UnicodenStrItemList,(PS8)AsciinStrItemList);
		len=pfnUnicodeStrlen((PS8)UnicodenStrItemList);
		len=MAX_SUBMENU_CHARACTERS-len;
		pfnUnicodeStrncpy((S8*)subMenuDataPtrs[i], ( S8*)GetString(STR_PROFILES_TONE),MAX_SUBMENU_CHARACTERS);
		pfnUnicodeStrcat((PS8)subMenuDataPtrs[i],(PS8)UnicodenStrItemList);

	}
	/* 6 Set current parent id*/
	SetParentHandler(MENU_TONE_SETUP_POWERON_TONE);

	/* 7 Register highlight handler to be called in menu screen */

	RegisterHighlightHandler(GetCurrScreenHandlerfive);

    if(gprofiles[gprofileId].toneSetup.powerOnTone != POWERON_SILENT)
	    data = (gprofiles[gprofileId].toneSetup.powerOnTone - gstartGeneralToneId)+1;
	else
		data = 0;

	/* 8 Display Category Screen */
	ShowCategory89Screen(STR_POWERON_TONE_SETUP_CAPTION,GetRootTitleIcon(MENU_TONE_SETUP_POWERON_TONE),
						 STR_GLOBAL_OK,IMG_GLOBAL_OK,
						 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						 nNumofItem,subMenuDataPtrs, NULL ,0,(U16)data, guiBuffer);
	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(EntryScrSetPoweronTone,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	//SetExitHandler(SCR_POWERON_TONE_SETUP, ExitScrProfilesPoweronToneSetup);
}
/**************************************************************

	FUNCTION NAME		: EntryScrSetPoweronTone(void)

  	PURPOSE				: SetPoweronTone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrSetPoweronTone(void)
{
	TONE_SETUP tonesetup;

	if(gcurrHiliteIndexOne == 0)
		tonesetup.powerOnTone = POWERON_SILENT;
	else
		tonesetup.powerOnTone = (gcurrHiliteIndexOne + gstartGeneralToneId)-1;

	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	SetPowerOnToneType(tonesetup);
	DeleteScreenIfPresent( SCR_POWERON_TONE_SETUP );
}
/**************************************************************

	FUNCTION NAME		: ExitScrProfilesPoweronToneSetup(void)

  	PURPOSE				: Exit profile Poweron ToneSetup Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitScrProfilesPoweronToneSetup(void)
{
	if(glastaudioId != POWERON_SILENT);
		StopToneAudio();
}


/**************************************************************

	FUNCTION NAME		: EntryScrProfilesPoweroffToneSetup(void)

  	PURPOSE				: Showing profile Poweroff Tone Setup Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesPoweroffToneSetup(void)
{
	U8 UnicodenStrItemList[10]; /* Stores the strings id of submenus returned */
	U8 AsciinStrItemList[4]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	U8 i=0;
    POWEROFF_TONE_ENUM data=0;


	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_POWEROFF_TONE_SETUP, ExitScrProfilesPoweroffToneSetup, EntryScrProfilesPoweroffToneSetup, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_POWEROFF_TONE_SETUP);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = gtotalGeneralToneId+1;

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_TONE_SETUP_POWEROFF_TONE);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	memset((S8*) subMenuDataPtrs[0],0,MAX_SUB_MENU_SIZE);
	pfnUnicodeStrncpy((S8*)subMenuDataPtrs[0], ( S8*)GetString(STR_PROFILES_SILENT),MAX_SUBMENU_CHARACTERS);

	for(i=1;i<=gtotalGeneralToneId;i++)
	{
		S32 len=0;
		memset((S8*) subMenuDataPtrs[i],0,MAX_SUB_MENU_SIZE);
		sprintf((PS8)AsciinStrItemList, "%d",i);
		AnsiiToUnicodeString((PS8)UnicodenStrItemList,(PS8)AsciinStrItemList);
		len=pfnUnicodeStrlen((PS8)UnicodenStrItemList);
		len=MAX_SUBMENU_CHARACTERS-len;
		pfnUnicodeStrncpy((S8*)subMenuDataPtrs[i], ( S8*)GetString(STR_PROFILES_TONE),MAX_SUBMENU_CHARACTERS);
		pfnUnicodeStrcat((PS8)subMenuDataPtrs[i],(PS8)UnicodenStrItemList);
	}

	/* 6 Set current parent id*/
	SetParentHandler(MENU_TONE_SETUP_POWEROFF_TONE);

	/* 7 Register highlight handler to be called in menu screen */

	RegisterHighlightHandler(GetCurrScreenHandlerfive);

	/* 8 Display Category Screen */

	if(gprofiles[gprofileId].toneSetup.powerOffTone != POWEROFF_SILENT)
		data = (gprofiles[gprofileId].toneSetup.powerOffTone - gstartGeneralToneId)+1;
	else
		data = 0;

	ShowCategory89Screen(STR_POWEROFF_TONE_SETUP_CAPTION,GetRootTitleIcon(MENU_TONE_SETUP_POWEROFF_TONE),
						 STR_GLOBAL_OK,IMG_GLOBAL_OK,
						 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						 nNumofItem,subMenuDataPtrs, NULL, 0,(U16)data, guiBuffer);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(EntryScrSetPoweroffTone,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	//SetExitHandler(SCR_POWEROFF_TONE_SETUP, ExitScrProfilesPoweroffToneSetup);
}

/**************************************************************

	FUNCTION NAME		: EntryScrSetPoweroffTone(void)

  	PURPOSE				: Set PowerOff Tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrSetPoweroffTone(void)
{
	TONE_SETUP tonesetup;

	if(gcurrHiliteIndexOne == 0)
		tonesetup.powerOffTone = POWEROFF_SILENT;
	else
		tonesetup.powerOffTone = (gcurrHiliteIndexOne + gstartGeneralToneId)-1;

	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	SetPowerOffToneType(tonesetup);
	DeleteScreenIfPresent( SCR_POWEROFF_TONE_SETUP );
}
/**************************************************************

	FUNCTION NAME		: ExitScrProfilesPoweronToneSetup(void)

  	PURPOSE				: Exit profile Poweron ToneSetup Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitScrProfilesPoweroffToneSetup(void)
{
	if(glastaudioId != POWERON_SILENT);
		StopToneAudio();
}

//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
/**************************************************************

	FUNCTION NAME		: HighlightProfilesCoverOpenToneSetup(void)

  	PURPOSE				: Highlight profile Cover Open Setup menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightProfilesCoverOpenToneSetup(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesCoverOpenToneSetup,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfilesCoverOpenToneSetup,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
/**************************************************************

	FUNCTION NAME		: EntryScrProfilesCoverOpenToneSetup(void)

  	PURPOSE				: Showing profile Cover Open Tone Setup Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesCoverOpenToneSetup(void)
{
	U8 UnicodenStrItemList[10]; /* Stores the strings id of submenus returned */
	U8 AsciinStrItemList[4]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	U8 i=0;
	COVER_OPEN_TONE_ENUM data=0;

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_COVEROPEN_TONE_SETUP, ExitScrProfilesCoverOpenToneSetup, EntryScrProfilesCoverOpenToneSetup, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_COVEROPEN_TONE_SETUP);

    /* 3. Retrieve no of child of menu item to be displayed */
#if defined(__MMI_MAINLCD_220X176__)
	nNumofItem = 3;
#else
	nNumofItem = gtotalGeneralToneId+1;
#endif
	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_TONE_SETUP_COVEROPEN_TONE);

	memset((S8*) subMenuDataPtrs[0],0,MAX_SUB_MENU_SIZE);
	pfnUnicodeStrncpy((S8*)subMenuDataPtrs[0], ( S8*)GetString(STR_PROFILES_SILENT),MAX_SUBMENU_CHARACTERS);

	for(i=1;i<=
#if defined(__MMI_MAINLCD_220X176__)
	2
#else
	gtotalGeneralToneId
#endif
	;i++)
	{
		S32 len=0;
		memset((S8*) subMenuDataPtrs[i],0,MAX_SUB_MENU_SIZE);
		sprintf((PS8)AsciinStrItemList, "%d",i);
		AnsiiToUnicodeString((PS8)UnicodenStrItemList,(PS8)AsciinStrItemList);
		len=pfnUnicodeStrlen((PS8)UnicodenStrItemList);
		len=MAX_SUBMENU_CHARACTERS-len;
		pfnUnicodeStrncpy((S8*)subMenuDataPtrs[i], ( S8*)GetString(STR_PROFILES_TONE),MAX_SUBMENU_CHARACTERS);
		pfnUnicodeStrcat((PS8)subMenuDataPtrs[i],(PS8)UnicodenStrItemList);
	}

	/* 6 Set current parent id*/
	SetParentHandler(MENU_TONE_SETUP_COVEROPEN_TONE);

	/* 7 Register highlight handler to be called in menu screen */

	RegisterHighlightHandler(GetCurrScreenHandlerfive);

	/* 8 Display Category Screen */
	if(gprofiles[gprofileId].toneSetup.coverOpenTone != COVER_OPEN_SILENT)
		data = (gprofiles[gprofileId].toneSetup.coverOpenTone - gstartGeneralToneId)+1;
	else
		data = 0;

	ShowCategory89Screen(STR_COVEROPEN_TONE_CAPTION,IMG_PROFILES_TITLEE,
						 STR_GLOBAL_OK,IMG_GLOBAL_OK,
						 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						 nNumofItem,subMenuDataPtrs, NULL, 0,(U16)data, guiBuffer);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(EntryScrSetCoverOpenTone,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	//SetExitHandler(SCR_COVEROPEN_TONE_SETUP, ExitScrProfilesCoverOpenToneSetup);
}
/**************************************************************

	FUNCTION NAME		: EntryScrSetCoverOpenTone(void)

  	PURPOSE				: Set Cover open tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrSetCoverOpenTone(void)
{
	TONE_SETUP tonesetup;
	if(gcurrHiliteIndexOne == 0)
		tonesetup.coverOpenTone = COVER_OPEN_SILENT;
	else
#if defined(__MMI_MAINLCD_220X176__)
		tonesetup.coverOpenTone = (gcurrHiliteIndexOne + gstartGeneralToneId)+3;
#else
		tonesetup.coverOpenTone = (gcurrHiliteIndexOne + gstartGeneralToneId)-1;
#endif
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	SetCoverOpenToneType(tonesetup);
	DeleteScreenIfPresent( SCR_COVEROPEN_TONE_SETUP );
}
/**************************************************************

	FUNCTION NAME		: ExitScrProfilesCoverOpenToneSetup(void)

  	PURPOSE				: Exit profile Cover Open ToneSetup Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitScrProfilesCoverOpenToneSetup(void)
{
	if(glastaudioId != POWERON_SILENT);
		StopToneAudio();
}
/**************************************************************

	FUNCTION NAME		: HighlightProfilesCoverCloseToneSetup(void)

  	PURPOSE				: Highlight profile Cover Close Setup menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightProfilesCoverCloseToneSetup(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesCoverCloseToneSetup,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfilesCoverCloseToneSetup,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
/**************************************************************

	FUNCTION NAME		: EntryScrProfilesCoverCloseToneSetup(void)

  	PURPOSE				: Showing profile Cover Close Tone Setup Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesCoverCloseToneSetup(void)
{
	U8 UnicodenStrItemList[10]; /* Stores the strings id of submenus returned */
	U8 AsciinStrItemList[4]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	U8 i=0;
	COVER_CLOSE_TONE_ENUM data=0;

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_COVERCLOSE_TONE_SETUP, ExitScrProfilesCoverCloseToneSetup, EntryScrProfilesCoverCloseToneSetup, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_COVERCLOSE_TONE_SETUP);
#if defined(__MMI_MAINLCD_220X176__)
	nNumofItem = 3;
#else
    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = gtotalGeneralToneId+1;
#endif
	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_TONE_SETUP_COVERCLOSE_TONE);

	memset((S8*) subMenuDataPtrs[0],0,MAX_SUB_MENU_SIZE);
	pfnUnicodeStrncpy((S8*)subMenuDataPtrs[0], ( S8*)GetString(STR_PROFILES_SILENT),MAX_SUBMENU_CHARACTERS);
#if defined(__MMI_MAINLCD_220X176__)
	for(i=1;i<=2;i++)
#else
	for(i=1;i<=gtotalGeneralToneId;i++)
#endif
	{
		S32 len=0;
		memset((S8*) subMenuDataPtrs[i],0,MAX_SUB_MENU_SIZE);
		sprintf((PS8)AsciinStrItemList, "%d",i);
		AnsiiToUnicodeString((PS8)UnicodenStrItemList,(PS8)AsciinStrItemList);
		len=pfnUnicodeStrlen((PS8)UnicodenStrItemList);
		len=MAX_SUBMENU_CHARACTERS-len;
		pfnUnicodeStrncpy((S8*)subMenuDataPtrs[i], ( S8*)GetString(STR_PROFILES_TONE),MAX_SUBMENU_CHARACTERS);
		pfnUnicodeStrcat((PS8)subMenuDataPtrs[i],(PS8)UnicodenStrItemList);
	}

	/* 6 Set current parent id*/
	SetParentHandler(MENU_TONE_SETUP_COVERCLOSE_TONE);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(GetCurrScreenHandlerfive);

	/* 8 Display Category Screen */
    if(gprofiles[gprofileId].toneSetup.coverCloseTone != COVER_CLOSE_SILENT)
		data = (gprofiles[gprofileId].toneSetup.coverCloseTone - gstartGeneralToneId)+1;
	else
		data = 0;

	ShowCategory89Screen(STR_COVERCLOSE_TONE_CAPTION,IMG_PROFILES_TITLEE,
						 STR_GLOBAL_OK,IMG_GLOBAL_OK,
						 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						 nNumofItem,subMenuDataPtrs, NULL, 0,(U16)data, guiBuffer);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(EntryScrSetCoverCloseTone,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	//SetExitHandler(SCR_COVERCLOSE_TONE_SETUP,ExitScrProfilesCoverCloseToneSetup);

}

/**************************************************************

	FUNCTION NAME		: EntryScrSetCoverCloseTone(void)

  	PURPOSE				: Set Cover close tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrSetCoverCloseTone(void)
{
	TONE_SETUP tonesetup;
	if(gcurrHiliteIndexOne == 0)
		tonesetup.coverCloseTone = COVER_CLOSE_SILENT;
	else
#if defined(__MMI_MAINLCD_220X176__)
		tonesetup.coverCloseTone = (gcurrHiliteIndexOne + gstartGeneralToneId)+3;
#else
		tonesetup.coverCloseTone = (gcurrHiliteIndexOne + gstartGeneralToneId)-1;
#endif
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	SetCoverCloseToneType(tonesetup);
	DeleteScreenIfPresent( SCR_COVERCLOSE_TONE_SETUP );
}
/**************************************************************

	FUNCTION NAME		: ExitScrProfilesCoverCloseToneSetup(void)

  	PURPOSE				: Exit profile Cover Close ToneSetup Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitScrProfilesCoverCloseToneSetup(void)
{
	if(glastaudioId != POWERON_SILENT);
		StopToneAudio();
}
#endif

/**************************************************************

	FUNCTION NAME		: EntryScrProfilesMessageToneSetup(void)

  	PURPOSE				: Showing profile Message Tone Setup Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesMessageToneSetup(void)
{
	U8 UnicodenStrItemList[10]; /* Stores the strings id of submenus returned */
	U8 AsciinStrItemList[4]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	U8 i=0;
	U16 str_id=0;
	MESSAGE_TONE_ENUM data=0;


	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_MESSAGE_TONE_SETUP, ExitScrProfilesMessageToneSetup, EntryScrProfilesMessageToneSetup, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_MESSAGE_TONE_SETUP);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = gtotalMiscToneId+1;

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_TONE_SETUP_MESSAGE_TONE);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	memset((S8*) subMenuDataPtrs[0],0,MAX_SUB_MENU_SIZE);
	pfnUnicodeStrncpy((S8*)subMenuDataPtrs[0], ( S8*)GetString(STR_PROFILES_SILENT),MAX_SUBMENU_CHARACTERS);

	for(i=1;i<=gtotalMiscToneId;i++)
	{
		S32 len=0;
		memset((S8*) subMenuDataPtrs[i],0,MAX_SUB_MENU_SIZE);
		sprintf((PS8)AsciinStrItemList, "%d",i);
		AnsiiToUnicodeString((PS8)UnicodenStrItemList,(PS8)AsciinStrItemList);
		len=pfnUnicodeStrlen((PS8)UnicodenStrItemList);
		len=MAX_SUBMENU_CHARACTERS-len;
		pfnUnicodeStrncpy((S8*)subMenuDataPtrs[i], ( S8*)GetString(STR_PROFILES_TONE),MAX_SUBMENU_CHARACTERS);
		pfnUnicodeStrcat((PS8)subMenuDataPtrs[i],(PS8)UnicodenStrItemList);

	}

	/* 6 Set current parent id*/
	SetParentHandler(MENU_TONE_SETUP_MESSAGE_TONE);

	/* 7 Register highlight handler to be called in menu screen */

//CSD Brian modified for playing message tone, 2003/11/7
	RegisterHighlightHandler(GetCurrScreenHandlersix);

	/* 8 Display Category Screen */
	if(gprofiles[gprofileId].toneSetup.messageTone != MESSAGE_SILENT)
		data = (gprofiles[gprofileId].toneSetup.messageTone - gstartMiscToneId)+1;
	else
		data = 0;
	#if defined( __MMI_DUAL_PROFILE_SUPPORT__) && !defined(__MMI_DUAL_SIM_SINGLE_OPENED_)
	str_id = STR_PROFILES_SIM1_MESSAGE_TONE_SETUP_CAPTION;
	#else
	str_id = STR_MESSAGE_TONE_CAPTION;
	#endif                                                         //Add by Renyh for 221 project   20090213
	ShowCategory89Screen(str_id,GetRootTitleIcon(MENU_TONE_SETUP_MESSAGE_TONE),
						 STR_GLOBAL_OK,IMG_GLOBAL_OK,
						 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						 nNumofItem,subMenuDataPtrs, NULL, 0,(U16)data, guiBuffer);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(EntryScrSetMessageTone,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	/* 10. Register Exit handler */
	//SetExitHandler(SCR_MESSAGE_TONE_SETUP,ExitScrProfilesMessageToneSetup);
}
/**************************************************************

	FUNCTION NAME		: EntryScrSetMessageTone(void)

  	PURPOSE				: Set Message Tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrSetMessageTone(void)
{
	TONE_SETUP tonesetup;
	if(gcurrHiliteIndexOne == 0)
		tonesetup.messageTone = MESSAGE_SILENT;
	else
		tonesetup.messageTone = (gcurrHiliteIndexOne + gstartMiscToneId)-1;

	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	SetMessageToneType(tonesetup);
	DeleteScreenIfPresent( SCR_MESSAGE_TONE_SETUP );
}
/**************************************************************

	FUNCTION NAME		: ExitScrProfilesMessageToneSetup(void)

  	PURPOSE				: Exit profile Message ToneSetup Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitScrProfilesMessageToneSetup(void)
{
	if(glastaudioId != POWERON_SILENT);
		StopToneAudio();
}

/**************************************************************

	FUNCTION NAME		: EntryScrProfilesKeypadToneSetup(void)

  	PURPOSE				: Showing profile Keypad Tone Setup Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesKeypadToneSetup(void)
{
	U16 nStrItemList[15]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	KEYPAD_TONE_ENUM  data=0;
	U8 i=0;
	U16 ProfileMenuID;

	TRACE_SYS_FUNCTION();
	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_KEYPAD_TONE_SETUP, NULL, EntryScrProfilesKeypadToneSetup, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_KEYPAD_TONE_SETUP);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_TONE_SETUP_KEYPAD_TONE);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_TONE_SETUP_KEYPAD_TONE);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_TONE_SETUP_KEYPAD_TONE,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_TONE_SETUP_KEYPAD_TONE);

	/* 7 Register highlight handler to be called in menu screen */


	/* 8 Display Category Screen */
    i = gprofiles[gprofileId].toneSetup.keypadTone;
	switch( i )
	{
	case KEYPAD_SILENT:
		ProfileMenuID = MENU_KEYPAD_TONE_SILENT;
		break;
	case KEYPAD_CLICK:
		ProfileMenuID = MENU_KEYPAD_TONE_CLICK;
		break;
	case KEYPAD_TONE:
		ProfileMenuID = MENU_KEYPAD_TONE_TONE;
		break;
#if defined( __MMI_HUMAN_VOICE_KEYPAD_TONE__ )
	case KEYPAD_TONE_HUMAN_VOICE_1:
		ProfileMenuID = MENU_KEYPAD_TONE_HUMAN_VOICE_1;
		break;
#if defined(__PROJECT_GALLITE_C01__)
#else
	case KEYPAD_TONE_HUMAN_VOICE_2:
		ProfileMenuID = MENU_KEYPAD_TONE_HUMAN_VOICE_2;
		break;
	case KEYPAD_TONE_HUMAN_VOICE_3:
		ProfileMenuID = MENU_KEYPAD_TONE_HUMAN_VOICE_3;
		break;
	case KEYPAD_TONE_HUMAN_VOICE_4:
		ProfileMenuID = MENU_KEYPAD_TONE_HUMAN_VOICE_4;
		break;
	case KEYPAD_TONE_HUMAN_VOICE_5:
		ProfileMenuID = MENU_KEYPAD_TONE_HUMAN_VOICE_5;
		break;
	case KEYPAD_TONE_HUMAN_VOICE_6:
		ProfileMenuID = MENU_KEYPAD_TONE_HUMAN_VOICE_6;
		break;
#endif
#endif
#if defined( __MMI_CUST_KEYPAD_TONE__ )
	case KEYPAD_TONE_CUST_1:
		ProfileMenuID = MENU_KEYPAD_TONE_CUST_1;
		break;
	case KEYPAD_TONE_CUST_2:
		ProfileMenuID = MENU_KEYPAD_TONE_CUST_2;
		break;
	case KEYPAD_TONE_CUST_3:
		ProfileMenuID = MENU_KEYPAD_TONE_CUST_3;
		break;
	case KEYPAD_TONE_CUST_4:
		ProfileMenuID = MENU_KEYPAD_TONE_CUST_4;
		break;
	case KEYPAD_TONE_CUST_5:
		ProfileMenuID = MENU_KEYPAD_TONE_CUST_5;
		break;
	case KEYPAD_TONE_CUST_6:
		ProfileMenuID = MENU_KEYPAD_TONE_CUST_6;
		break;
#endif
	default:
		ProfileMenuID = 0;
		break;
	}
	data = GetIndexOfStringId(MENU_TONE_SETUP_KEYPAD_TONE,ProfileMenuID);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory11Screen(STR_KEYPAD_TONE_CAPTION, GetRootTitleIcon(MENU_TONE_SETUP_KEYPAD_TONE),
						 STR_GLOBAL_OK, 0,
						 STR_GLOBAL_BACK, 0,
						 nNumofItem, nStrItemList,(U16)data, guiBuffer);

	//SetExitHandler(SCR_KEYPAD_TONE_SETUP,ExitScrProfilesKeypadToneSetup);

}
/**************************************************************

	FUNCTION NAME		: SetKeyPadToneSilent(void)

  	PURPOSE				: Set KeyPad Tone as Silent

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetKeyPadToneSilent(void)
{
	SetKeypadToneType(KEYPAD_SILENT);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_KEYPAD_TONE_SETUP );
}
/**************************************************************

	FUNCTION NAME		: SetKeyPadToneClick(void)

  	PURPOSE				: Set KeyPad Tone as Click

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetKeyPadToneClick(void)
{
	SetKeypadToneType(KEYPAD_CLICK);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_KEYPAD_TONE_SETUP );
}
/**************************************************************

	FUNCTION NAME		: SetKeyPadToneTone(void)

  	PURPOSE				: Set KeyPad Tone as Tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SetKeyPadToneTone(void)
{
	SetKeypadToneType(KEYPAD_TONE);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_KEYPAD_TONE_SETUP );
}

#if defined( __MMI_HUMAN_VOICE_KEYPAD_TONE__ )
void SetKeyPadToneHumanVoice1(void)
{
	SetKeypadToneType(KEYPAD_TONE_HUMAN_VOICE_1);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_KEYPAD_TONE_SETUP );
}
void SetKeyPadToneHumanVoice2(void)
{
	SetKeypadToneType(KEYPAD_TONE_HUMAN_VOICE_2);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_KEYPAD_TONE_SETUP );
}
void SetKeyPadToneHumanVoice3(void)
{
	SetKeypadToneType(KEYPAD_TONE_HUMAN_VOICE_3);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_KEYPAD_TONE_SETUP );
}
void SetKeyPadToneHumanVoice4(void)
{
	SetKeypadToneType(KEYPAD_TONE_HUMAN_VOICE_4);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_KEYPAD_TONE_SETUP );
}
void SetKeyPadToneHumanVoice5(void)
{
	SetKeypadToneType(KEYPAD_TONE_HUMAN_VOICE_5);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_KEYPAD_TONE_SETUP );
}
void SetKeyPadToneHumanVoice6(void)
{
	SetKeypadToneType(KEYPAD_TONE_HUMAN_VOICE_6);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_KEYPAD_TONE_SETUP );
}
#endif

#if defined( __MMI_CUST_KEYPAD_TONE__ )
void SetKeyPadToneCust1(void)
{
	SetKeypadToneType(KEYPAD_TONE_CUST_1);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_KEYPAD_TONE_SETUP );
}
void SetKeyPadToneCust2(void)
{
	SetKeypadToneType(KEYPAD_TONE_CUST_2);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_KEYPAD_TONE_SETUP );
}
void SetKeyPadToneCust3(void)
{
	SetKeypadToneType(KEYPAD_TONE_CUST_3);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_KEYPAD_TONE_SETUP );
}
void SetKeyPadToneCust4(void)
{
	SetKeypadToneType(KEYPAD_TONE_CUST_4);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_KEYPAD_TONE_SETUP );
}
void SetKeyPadToneCust5(void)
{
	SetKeypadToneType(KEYPAD_TONE_CUST_5);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_KEYPAD_TONE_SETUP );
}
void SetKeyPadToneCust6(void)
{
	SetKeypadToneType(KEYPAD_TONE_CUST_6);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_KEYPAD_TONE_SETUP );
}
#endif

 
#ifndef __MMI_ALM_CUSTOMIZE_TONE__
/**************************************************************

	FUNCTION NAME		: EntryScrProfilesAlarmToneSetup(void)

  	PURPOSE				: Showing profile Alarm Tone Setup Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesAlarmToneSetup(void)
{
	U8* guiBuffer;       /* Buffer holding history data */
	U16 nDispAttribute;  /* Stores display attribue */
	U16 selecteditem =0;
	U8 totalItems=0;


//chenhe +
	if (!is_phb_init_done())  //have no SIM or phb is ready
	{
	    //开机初始化 stringid = 25803  modified by renyh  2008/8/1
		DisplayPopup((PU8)GetString(STR_GLOBAL_POWER_ON_PROCESSING) ,IMG_GLOBAL_PROGRESS, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
		return;
	}


	if(checkUsbstate())
	{
		mmi_display_usb_busy();
		return;
	}

//chenhe -
	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_ALARM_TONE_SETUP, ExitScrProfilesAlarmToneSetup, EntryScrProfilesAlarmToneSetup, NULL);

	gcurrHiliteIndexOne = 0;

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_ALARM_TONE_SETUP);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_TONE_SETUP_ALARM_TONE);

	/* 3. Retrieve no of child of menu item to be displayed */
	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	totalItems = mmi_profiles_get_melody_list( gprofiles[gprofileId].toneSetup.alarmTone, &selecteditem);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_TONE_SETUP_ALARM_TONE);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(mmi_profiles_melody_list_highlight_hdlr);

	/* 8 Display Category Screen */
	ShowCategory89Screen(STR_ALARM_TONE_CAPTION,GetRootTitleIcon(MENU_TONE_SETUP_ALARM_TONE),
						 STR_GLOBAL_OK,IMG_GLOBAL_OK,
						 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						 totalItems,subMenuDataPtrs, hintDataPtrs, 0,selecteditem, guiBuffer);

     /* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(EntryScrSetAlarmtone,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}


/**************************************************************

	FUNCTION NAME		: EntryScrSetAlarmtone(void)

  	PURPOSE				: Set Alarm Tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrSetAlarmtone(void)
{
	U8 is_ext_melody = MMI_FALSE;
	U16 numOfItem = gtotalRingId + gtotalMidiId;
	U16 ring_id = 0;

	if( g_profiles_melody_list_index < gtotalRingId )
		ring_id = g_profiles_melody_list_index + gstartRingId ;
	else if( g_profiles_melody_list_index < numOfItem )
		ring_id = g_profiles_melody_list_index + gstartMidiId - gtotalRingId;
#if defined(__MMI_PROFILE_EXTMELODY_SUPPORT__)
	else if( g_profiles_melody_list_index < numOfItem + MAXIMUM_EXT_MELODY_NUM )
	{
		g_ext_melody_idx = (U8)(g_profiles_melody_list_index - numOfItem);
		ring_id = (U16)g_ext_melody_idx + PMG_EXT_MELODY_BEGIN;
		is_ext_melody = MMI_TRUE;
	}
#endif
	else
	{
#if defined( __MMI_DOWNLOADABLE_THEMES_SUPPORT__ )
		BOOL selected = FALSE;
#endif
#if defined(__MMI_PROFILE_EXTMELODY_SUPPORT__)
		numOfItem += MAXIMUM_EXT_MELODY_NUM;
#endif
#if defined( __MMI_DOWNLOADABLE_THEMES_SUPPORT__ )
		if( g_profiles_melody_list_index == numOfItem )
		{
			if( selected_downloadable_theme_melody_id != 0 )
			{
				ring_id = selected_downloadable_theme_melody_id;
				selected = TRUE;
			}
		}
		ASSERT( selected );
#endif
	}
#if defined(__MMI_PROFILE_EXTMELODY_SUPPORT__)
	if( is_ext_melody == MMI_TRUE )
	{
		prfset_conf_type = PRFSET_MELODY_TYPE_ALARM;
		EntryPrfSetRingToneExtMelody();
	}
	else
#endif
	{
		DisplayPopup( (PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE );
		SetAlarmToneID( ring_id );
		DeleteScreenIfPresent( SCR_ALARM_TONE_SETUP );
	}
}

/**************************************************************

	FUNCTION NAME		: ExitScrProfilesAlarmToneSetup(void)

  	PURPOSE				: Exit profile Alarm ToneSetup Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitScrProfilesAlarmToneSetup(void)
{
	StopToneAudio();
}
#endif //__MMI_ALM_CUSTOMIZE_TONE__

/**************************************************************

	FUNCTION NAME		: EntryScrProfilesExtraToneSetup(void)

  	PURPOSE				: Showing profile Extra Tone Setup Screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesExtraToneSetup(void)
{
	U16 nStrItemList[15]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8 i=0;
	U8* guiBuffer;       /* Buffer holding history data */

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_EXTRA_TONE_SETUP, NULL, EntryScrProfilesExtraToneSetup, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_EXTRA_TONE_SETUP);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_EXTRA_TONE);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_EXTRA_TONE);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_EXTRA_TONE,nStrItemList);

	for(i=0;i<nNumofItem;i++)
	{
		memset((S8*)subMenuDataPtrs[i],0,MAX_SUB_MENU_SIZE);
		pfnUnicodeStrncpy((S8*)subMenuDataPtrs[i],GetString(nStrItemList[i]),MAX_SUBMENU_CHARACTERS);

	}

	/* 6 Set current parent id*/
	SetParentHandler(MENU_EXTRA_TONE);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	fieldStatesExtraTone[0] = (U8)gprofiles[gprofileId].extraTone.warningTone ;
	fieldStatesExtraTone[1] = (U8)gprofiles[gprofileId].extraTone.errorTone  ;
	fieldStatesExtraTone[2] = (U8)gprofiles[gprofileId].extraTone.campOnTone ;
	fieldStatesExtraTone[3] = (U8)gprofiles[gprofileId].extraTone.connectTone;


	SetCheckboxToggleRightSoftkeyFunctions(ProfilesExtraTonePopup,GoBackHistory);

	/* 8 Display Category Screen */
	ShowCategory140Screen(STR_EXTRA_TONE_CAPTION,GetRootTitleIcon(MENU_EXTRA_TONE),
						  nNumofItem,(U8**)subMenuDataPtrs,fieldStatesExtraTone, 0, guiBuffer);


	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	/* 10. Register Exit handler */
	//SetExitHandler(SCR_EXTRA_TONE_SETUP,ExitScrProfilesExtraToneSetup);
}

/**************************************************************

	FUNCTION NAME		: SetExtraToneValues(void)

  	PURPOSE				: Set Extra Tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void SetExtraToneValues(void)
{
EXTRA_TONE extraTone;

	extraTone.warningTone = gprofiles[gprofileId].extraTone.warningTone = fieldStatesExtraTone[0];
	extraTone.errorTone   = gprofiles[gprofileId].extraTone.errorTone   = fieldStatesExtraTone[1];
	extraTone.campOnTone  = gprofiles[gprofileId].extraTone.campOnTone  = fieldStatesExtraTone[2];
	extraTone.connectTone = gprofiles[gprofileId].extraTone.connectTone = fieldStatesExtraTone[3];

	SetExtraTone(extraTone);

	//DisplayPopupCheckBox((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);

	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_EXTRA_TONE_SETUP );
}
/**************************************************************

	FUNCTION NAME		: ProfilesExtraTonePopup(void)

  	PURPOSE				: Set the call back functios for Extra tone setup

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ProfilesExtraTonePopup(void)
{
	EntryScrProfilesQueryPopup(SetExtraToneValues,ProfileGoBack2History);
}


/**************************************************************

	FUNCTION NAME		: PlayToneAudio(void)

  	PURPOSE				: PlayToneAudio

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PlayToneAudio(void)
{
	StopTimer(TIMER_PROFILES_TONE_PLAY);
	glastaudioId = gcurrentaudioId;

	if( !IsSilentModeActivated() && !isInCall() ) // need to check if it's in call because phonebook use this function, too
	{
#ifdef __MMI_SILENT_MEETING_PROFILE__
		if( !IsMeetingModeActivated() )
#endif /* __MMI_SILENT_MEETING_PROFILE__ */
		AudioPlayReq(gcurrentaudioId,(U8)DEVICE_AUDIO_PLAY_INFINITE);
	}
}

/**************************************************************

	FUNCTION NAME		: StopToneAudio(void)

  	PURPOSE				: Stop Tone Audio

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void StopToneAudio(void)
{
	StopTimer(TIMER_PROFILES_TONE_PLAY);
	/*Jinzh Modify Start For 6233 Ver: TBM780  on 2007-8-23 9:32 */
	if( /*!IsSilentModeActivated() && */!isInCall() ) // need to check if it's in call because phonebook use this function, too
	/*Jinzh Modify End  For 6233 Ver: TBM780  on 2007-8-23 9:32 */
	{
#ifdef __MMI_SILENT_MEETING_PROFILE__
		if( !IsMeetingModeActivated() )
#endif /* __MMI_SILENT_MEETING_PROFILE__ */
		{
			if( glastaudioId != 0 )
				AudioStopReq( glastaudioId );
		}
	}
}
/**************************************************************

	FUNCTION NAME		: profileAlertStatusIcons(void)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void showAlertStatusIcons(void)
{
	static s16 currentStatusIcon;

	//CSD Brian added for wrap sublcd display, 2003/10/18
	#ifdef __MMI_SUBLCD__
	static s16 currentStatusSubLcdIcon;
		if( currentStatusSubLcdIcon != 0)
			HideStatusIcon(currentStatusSubLcdIcon);
	#endif

	if( currentStatusIcon != 0)
		HideStatusIcon(currentStatusIcon);
	if(gactivatedprofile == 5)
	{
//qiff modify for gs.prj 10625
		currentStatusIcon = STATUS_ICON_SILENT;
		ShowStatusIcon(currentStatusIcon);
	//CSD Brian added for wrap sublcd display, 2003/10/18
	#ifdef __MMI_SUBLCD__
		currentStatusSubLcdIcon = STATUS_ICON_SUBLCD_VIBRATE;
		ShowStatusIcon(currentStatusSubLcdIcon );
	#endif
	}
	else
	{
		switch(gcurrentprofile.mtCallAlertTypeEnum)
		{
		case 1:
			currentStatusIcon = STATUS_ICON_RING;
			ShowStatusIcon(currentStatusIcon);
			//CSD Brian added for wrap sublcd display, 2003/10/18
			#ifdef __MMI_SUBLCD__
			currentStatusSubLcdIcon = STATUS_ICON_SUBLCD_RING;
			ShowStatusIcon(currentStatusSubLcdIcon);
			#endif
			break;
		case 2:
			currentStatusIcon = STATUS_ICON_VIBRATION;
			ShowStatusIcon(currentStatusIcon);
			//CSD Brian added for wrap sublcd display, 2003/10/18
			#ifdef __MMI_SUBLCD__
			currentStatusSubLcdIcon = STATUS_ICON_SUBLCD_VIBRATE;
			ShowStatusIcon(currentStatusSubLcdIcon);
			#endif
			break;
		case 3:
			currentStatusIcon = STATUS_ICON_VIBRATE_AND_RING;
			ShowStatusIcon(currentStatusIcon);
			//CSD Brian added for wrap sublcd display, 2003/10/18
			#ifdef __MMI_SUBLCD__
			currentStatusSubLcdIcon = STATUS_ICON_SUBLCD_VIBRATE_AND_RING;
			ShowStatusIcon(currentStatusSubLcdIcon);
			#endif
			break;
		case 4:
			currentStatusIcon = STATUS_ICON_VIBRATE_THEN_RING;
			ShowStatusIcon(currentStatusIcon);
			//CSD Brian added for wrap sublcd display, 2003/10/18
			#ifdef __MMI_SUBLCD__
			currentStatusSubLcdIcon = STATUS_ICON_SUBLCD_VIBRATE_THEN_RING;
			ShowStatusIcon(currentStatusSubLcdIcon );
			#endif
			break;
		case 5:
			currentStatusIcon = STATUS_ICON_SILENT;
			ShowStatusIcon(currentStatusIcon);
			//CSD Brian added for wrap sublcd display, 2003/10/18
			#ifdef __MMI_SUBLCD__
			currentStatusSubLcdIcon = STATUS_ICON_SUBLCD_SILENT;
			ShowStatusIcon(currentStatusSubLcdIcon );
			#endif
			break;
		default:
			currentStatusIcon = STATUS_ICON_RING;
			ShowStatusIcon(currentStatusIcon);
			#ifdef __MMI_SUBLCD__
			currentStatusSubLcdIcon = STATUS_ICON_SUBLCD_RING;
			ShowStatusIcon(currentStatusSubLcdIcon);
			#endif
			break;
		}
	}
	UpdateStatusIcons();
}
/**************************************************************

	FUNCTION NAME		: ReturnRequiredNumberImageId(U8 imageId)

  	PURPOSE				:

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U16 ReturnRequiredNumberImageId(U8 imageId)
{
	PRINT_INFORMATION(("\n@@@@@ [.c] ReturnRequiredImageId @@@@@"));

	switch(imageId+1)
	{
		case 1: return IMG_GLOBAL_L1; break;
		case 2: return IMG_GLOBAL_L2; break;
		case 3: return IMG_GLOBAL_L3; break;
		case 4: return IMG_GLOBAL_L4; break;
		case 5: return IMG_GLOBAL_L5; break;
		case 6: return IMG_GLOBAL_L6; break;
		case 7: return IMG_GLOBAL_L7; break;
		case 8: return IMG_GLOBAL_L8; break;
		case 9: return IMG_GLOBAL_L9; break;
		case 10: return IMG_GLOBAL_L10; break;
		case 11: return IMG_GLOBAL_L11; break;
		case 12: return IMG_GLOBAL_L12; break;
		case 13: return IMG_GLOBAL_L13; break;
		case 14: return IMG_GLOBAL_L14; break;
		case 15: return IMG_GLOBAL_L15; break;
		case 16: return IMG_GLOBAL_L16; break;
		case 17: return IMG_GLOBAL_L17; break;
		case 18: return IMG_GLOBAL_L18; break;
		case 19: return IMG_GLOBAL_L19; break;
		case 20: return IMG_GLOBAL_L20; break;
		case 21: return IMG_GLOBAL_L21; break;
		case 22: return IMG_GLOBAL_L22; break;
		case 23: return IMG_GLOBAL_L23; break;
		case 24: return IMG_GLOBAL_L24; break;
		case 25: return IMG_GLOBAL_L25; break;
		case 26: return IMG_GLOBAL_L26; break;
		case 27: return IMG_GLOBAL_L27; break;
		case 28: return IMG_GLOBAL_L28; break;
		case 29: return IMG_GLOBAL_L29; break;
		case 30: return IMG_GLOBAL_L30; break;
		//Fix Bug#12068 for M301WapMMS by cong.li on 2009.05.14.
		case 31: return IMG_GLOBAL_L31; break;
		case 32: return IMG_GLOBAL_L32; break;
		case 33: return IMG_GLOBAL_L33; break;
		case 34: return IMG_GLOBAL_L34; break;
		case 35: return IMG_GLOBAL_L35; break;
		case 36: return IMG_GLOBAL_L36; break;
		case 37: return IMG_GLOBAL_L37; break;
		case 38: return IMG_GLOBAL_L38; break;		
		case 39: return IMG_GLOBAL_L39; break;				
		default: return IMG_GLOBAL_QUESTION; break;
	}
}

/**************************************************/
/*				General Functions							  */
/**************************************************/
/**************************************************************

	FUNCTION NAME		: playcolorID(void)

  	PURPOSE				: play color id

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void playcolorID(void)
{
	glastColorID = gcurrentColorId;
	PlayPatternCallerGroup(gcurrentColorId) ;
}
/**************************************************************

	FUNCTION NAME		: StopColorId(void)

  	PURPOSE				: stop color id

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void StopColorId(void)
{
   StopTimer(TIMER_PROFILES_TONE_PLAY);
   if(glastColorID)
   {
      StopPatternCallerGroup(glastColorID);
   }
}

/**************************************************************

	FUNCTION NAME		: GetDynamicToneIds(void)

  	PURPOSE				: Get Dynamic tone ids
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GetDynamicToneIds(void)
{
	GetRingToneRange((PU8)&gstartRingId,(PU8)&gtotalRingId);  //dyj del 20060514 //add by panxu 20061222
#ifdef MMI_ON_WIN32
	gstartRingId = 101;
	gtotalRingId = 10;
#endif
		GetMidiRange((PU8)&gstartMidiId, (PU8)&gtotalMidiId);
#ifdef MMI_ON_WIN32
	gstartMidiId = 151;
#endif
	GetToneRange((PU8)&gstartGeneralToneId,(PU8)&gtotalGeneralToneId);
#ifdef MMI_ON_WIN32
	gstartGeneralToneId = 166;
#endif
	GetMiscToneRange((PU8)&gstartMiscToneId,(PU8)&gtotalMiscToneId);
#ifdef MMI_ON_WIN32
	gstartMiscToneId = 166;
#endif

#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
     PmgGetExtMelodyRange(&gstartExtMelodyId, &gtotalExtMelodyId );
#endif

}
/**************************************************************

	FUNCTION NAME		: EntryScrProfilesQueryPopup(void)

  	PURPOSE				: General Pop Up function for Query screens of profiles

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrProfilesQueryPopup(void (*fn_lsk)(void),void (*fn_rsk)(void))
{
	gfnprofilelsk = fn_lsk;
	gfnprofilersk = fn_rsk;
	EntryPorfPopup();
}

/**************************************************************

	FUNCTION NAME		: EntryPorfPopup(void)

  	PURPOSE				: entry fn of query screens

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryPorfPopup(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						  STR_GLOBAL_NO, IMG_GLOBAL_NO,
						  get_string(STR_GLOBAL_SAVE), IMG_GLOBAL_QUESTION, WARNING_TONE);

	SetLeftSoftkeyFunction(gfnprofilelsk,KEY_EVENT_UP);
	SetRightSoftkeyFunction(gfnprofilersk,KEY_EVENT_UP);

	//SetExitHandler(SCR_POPUP_PROFILES_QEURY, ExitScrProfilesQueryPopup);
}
/**************************************************************

	FUNCTION NAME		: ExitScrProfilesQueryPopup(void)

  	PURPOSE				: exit fn

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitScrProfilesQueryPopup(void)
{
	stopRequestedTone(WARNING_TONE);
}


/**************************************************************

	FUNCTION NAME		: GetCurrScreenHandler(void)

  	PURPOSE				: Getting Current Screen Handler

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GetCurrScreenHandler(S32 item_index)
{
	gcurrHiliteIndexOne = item_index;
}

/**************************************************************
	FUNCTION NAME		: mmi_profiles_melody_list_highlight_hdlr
  	PURPOSE				: Getting Current Screen Handler
	INPUT PARAMETERS	: S32 index_index
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
 
**************************************************************/
void mmi_profiles_melody_list_highlight_hdlr( S32 item_index )
{
	U16 numOfItem = gtotalRingId + gtotalMidiId;

	g_profiles_melody_list_index = (U16)item_index;

	StopToneAudio();

	if( g_profiles_melody_list_index < gtotalRingId )
		gcurrentaudioId = g_profiles_melody_list_index + gstartRingId ;
	else if( g_profiles_melody_list_index < numOfItem )
		gcurrentaudioId = g_profiles_melody_list_index + gstartMidiId - gtotalRingId;
#if defined(__MMI_PROFILE_EXTMELODY_SUPPORT__)
	else if( g_profiles_melody_list_index < numOfItem + MAXIMUM_EXT_MELODY_NUM )
	{
		g_ext_melody_idx = (U8)(g_profiles_melody_list_index - numOfItem);
		gcurrentaudioId = (U16)g_ext_melody_idx + PMG_EXT_MELODY_BEGIN;
	}
#endif
	else
	{
#if defined( __MMI_DOWNLOADABLE_THEMES_SUPPORT__ )
		BOOL selected = FALSE;
#endif
#if defined(__MMI_PROFILE_EXTMELODY_SUPPORT__)
		numOfItem += MAXIMUM_EXT_MELODY_NUM;
#endif
#if defined( __MMI_DOWNLOADABLE_THEMES_SUPPORT__ )
		if( g_profiles_melody_list_index == numOfItem )
		{
			if( selected_downloadable_theme_melody_id != 0 )
			{
				gcurrentaudioId = selected_downloadable_theme_melody_id;
				selected = TRUE;
			}
		}
		ASSERT( selected );
#endif
	}
	StartTimer( TIMER_PROFILES_TONE_PLAY, 1000, PlayToneAudio );
}

/**************************************************************

	FUNCTION NAME		: GetCurrScreenHandlerfour(void)

  	PURPOSE				: Getting Current Screen Handler

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GetCurrScreenHandlerfour(S32 item_index)
{
	gcurrHiliteIndexOne = item_index;
	StopColorId();
	if(gcurrHiliteIndexOne)
	{
		gcurrentColorId = (MMI_LEDLIGHT_STATUS_ENUM)gcurrHiliteIndexOne;
		StartTimer(TIMER_PROFILES_TONE_PLAY,1000,playcolorID);
	}
}
/**************************************************************

	FUNCTION NAME		: GetCurrScreenHandlerfive(void)

  	PURPOSE				: Getting Current Screen Handler

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GetCurrScreenHandlerfive(S32 item_index)
{
	gcurrHiliteIndexOne = item_index;

	if(glastaudioId > 0)
		StopToneAudio();

	if(gcurrHiliteIndexOne == 0)
	{
		gcurrentaudioId = 0;
		glastaudioId = gcurrentaudioId;
//CSD Brian added for stop timer in case of 0-->0, 2003/11/18
		StopTimer(TIMER_PROFILES_TONE_PLAY);
	}
	else
	{
#if defined(__MMI_MAINLCD_220X176__)
		if((SCR_COVERCLOSE_TONE_SETUP == GetActiveScreenId()) || (SCR_COVEROPEN_TONE_SETUP == GetActiveScreenId()))
			gcurrentaudioId = gcurrHiliteIndexOne + gstartGeneralToneId+3;
		else
#endif
		gcurrentaudioId = gcurrHiliteIndexOne + gstartGeneralToneId-1;
		StartTimer(TIMER_PROFILES_TONE_PLAY,1000,PlayToneAudio);
	}
}
//CSD Brian added for playing message tone, 2003/11/7
/**************************************************************

	FUNCTION NAME		: GetCurrScreenHandlerfive(void)

  	PURPOSE				: Getting Current Screen Handler

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GetCurrScreenHandlersix(S32 item_index)
{
	gcurrHiliteIndexOne = item_index;

	if(glastaudioId > 0)
		StopToneAudio();

	if(gcurrHiliteIndexOne == 0)
	{
		gcurrentaudioId = 0;
		glastaudioId = gcurrentaudioId;
//CSD Brian added for stop timer in case of 0-->0, 2003/11/18
		StopTimer(TIMER_PROFILES_TONE_PLAY);
	}
	else
	{
		gcurrentaudioId = gcurrHiliteIndexOne + gstartMiscToneId-1;
		StartTimer(TIMER_PROFILES_TONE_PLAY,1000,PlayToneAudio);
	}
}
//CSD end

#if defined( __MMI_INTELLIGENT_CALL_ALERT__ )
/**************************************************************
	FUNCTION NAME		: EntryScrIntelligentCallAlert( void )
  	PURPOSE				: EntryScrIntelligentCallAlert
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
 
**************************************************************/
void EntryScrIntelligentCallAlert( void )
{
	U16 nStrItemList[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U8* guiBuffer;       /* Buffer holding history data */
	S8 i=0;
	//U8* PopUpList[MAX_SUB_MENUS];
	MMI_INTELLIGENT_CALL_ALERT_TYPE data=0;
	U16 ProfileMenuID[]={MENU_INTELLIGENT_CALL_ALERT_OFF, MENU_INTELLIGENT_CALL_ALERT_ON};

	/* 1 Call Exit Handler*/
	EntryNewScreen(SCR_PROFILES_INTELLIGENT_CALL_ALERT, NULL, EntryScrIntelligentCallAlert, NULL);

	/* 2 Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR_PROFILES_INTELLIGENT_CALL_ALERT);

    /* 3. Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU_INTELLIGENT_CALL_ALERT);

	/* 4. Get attribute of menu to be displayed */
	nDispAttribute = GetDispAttributeOfItem(MENU_INTELLIGENT_CALL_ALERT);

	/* 5. Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU_INTELLIGENT_CALL_ALERT,nStrItemList);

	/* 6 Set current parent id*/
	SetParentHandler(MENU_INTELLIGENT_CALL_ALERT);

	/* 7 Register highlight handler to be called in menu screen */
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	i = gprofiles[gprofileId].intelligentCallAlert;
	data = GetIndexOfStringId(MENU_INTELLIGENT_CALL_ALERT,ProfileMenuID[i]);

	/* 8 Display Category Screen */
	ShowCategory11Screen(STR_PROFILES_INTELLIGENT_CALL_ALERT,IMG_PROFILES_TITLEE,
		STR_GLOBAL_OK, IMG_GLOBAL_OK,
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
		nNumofItem, nStrItemList,(U16)data,guiBuffer);
}
#endif

/*********************************************************
*
*
*				For Hint Handlers
*
*
**********************************************************/
/**************************************************************

	FUNCTION NAME		: HandleMelodyHintFunc( U16 ToneID, U16 index )

  	PURPOSE				: Hint handlers for Incoming call / Alarm tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HandleMelodyHintFunc( U16 ToneID, U16 index )
{
	U8 data = 0;
#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
	DYNEXTMELODYINFO*ExtMelodyStruct;
#endif
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
	DYNAUDIOINFO *DownloadThemeTones;
#endif

	if( ToneID >= gstartRingId && ToneID < gstartRingId + gtotalRingId )
		pfnUnicodeStrcpy( (S8 *)hintData[index], GetString( (U16)(STR_PROFILES_RING_1 + ToneID - gstartRingId) ) );
	else if( ToneID >= gstartMidiId && ToneID < gstartMidiId + gtotalMidiId )
		pfnUnicodeStrcpy( (S8 *)hintData[index], GetString( (U16)(STR_PROFILES_MIDI_1 + ToneID - gstartMidiId) ) );
	else
	{
		BOOL selected = FALSE;
#if defined(__MMI_PROFILE_EXTMELODY_SUPPORT__)
		PmgGetExtMelodyInfo( &ExtMelodyStruct );
		for( data = 0; data < MAXIMUM_EXT_MELODY_NUM; data++)
		{
			if( PmgIsEmptySlot(data) == MMI_FALSE && ToneID == ExtMelodyStruct->info[data].id )
 			{
				if( IS_EXTMELODY_SHORTNAME( ExtMelodyStruct->info[data].filepath ) )
					mmi_chset_mixed_text_to_ucs2_str( hintData[(index)], (FMGR_MAX_FILE_LEN+1) * ENCODING_LENGTH, (U8*)ExtMelodyStruct->info[data].filename, g_chset_text_encoding );
				else
					pfnUnicodeStrcpy(( PS8 ) hintData[(index)],( PS8 )(ExtMelodyStruct->info[data].filename) );
#if (!defined(__MMI_SHOW_FILE_EXT__))
				PmgRemoveFileExt( (S8*) hintData[(index)] );
#elif (!defined(__MMI_SHOW_DAF_FILE_EXT__))
				PmgRemoveFileDAFExt( (S8*) hintData[(index)] );
#endif
				selected = TRUE;
				break;
			}
		}
#endif
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
		if( !selected )
		{
			selected = mmi_profiles_is_melody_from_downloadable_theme( ToneID );
			if( selected )
				pfnUnicodeStrcpy( (PS8) hintData[(index)], GetString( (U16) STR_PROFILES_CURR_THEME )  );
		}
#endif
 if(!selected)
    {
        /*when there is no mapping selected item, assgin midi1 to both ring tone and alarm tone*/
        gprofiles[gprofileId].toneSetup.ringTone = gstartMidiId ;
        gprofiles[gprofileId].toneSetup.alarmTone = gstartMidiId ;
        pfnUnicodeStrcpy((PS8) hintData[index], GetString((U16) (STR_PROFILES_MIDI_1)));
    }
	}
}


/**************************************************************

	FUNCTION NAME		: HintHandlerIncomingCall(void)

  	PURPOSE				: Hint handlers for Incoming call tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintHandlerIncomingCall(U16 index)
{
   HandleMelodyHintFunc(gprofiles[gprofileId].toneSetup.ringTone, index );
}

#ifdef __MMI_DUAL_PROFILE_SUPPORT__
/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_hint_hdlr_card2_incoming_call
 * DESCRIPTION
 *  Hint handlers for card2 incoming call tone
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_hint_hdlr_card2_incoming_call(U16 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    HandleMelodyHintFunc(gprofiles[gprofileId].toneSetup.card2_ringTone, index);
}
#endif

#ifndef __MMI_ALM_CUSTOMIZE_TONE__
/**************************************************************

	FUNCTION NAME		: HintHandlerAlarmTone(void)

  	PURPOSE				: Hint handlers for Alarm Tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintHandlerAlarmTone(U16 index)
{
   HandleMelodyHintFunc(gprofiles[gprofileId].toneSetup.alarmTone, index );
}

#endif


/**************************************************************

	FUNCTION NAME		: HintHandlerPowerOnTone(void)

  	PURPOSE				: Hint handlers for Power On Tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintHandlerPowerOnTone(U16 index)
{
	U8 UnicodenStrItemList[10]; /* Stores the strings id of submenus returned */
	U8 AsciinStrItemList[4]; /* Stores the strings id of submenus returned */
	U8 data = 0;
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
    if((gprofiles[gprofileId].toneSetup.powerOnTone)!= POWERON_SILENT)
    {
		S32 len=0;
		data = ((gprofiles[gprofileId].toneSetup.powerOnTone) - (gstartGeneralToneId));
		memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
		sprintf((PS8)AsciinStrItemList , "%d",data+1);
		AnsiiToUnicodeString((PS8)UnicodenStrItemList,(PS8)AsciinStrItemList);
		len=pfnUnicodeStrlen((PS8)UnicodenStrItemList);
		len=MAX_SUBMENU_CHARACTERS-len;
		pfnUnicodeStrncpy((S8 *)hintData[(index)],GetString(STR_PROFILES_TONE),MAX_SUBMENU_CHARACTERS);
		pfnUnicodeStrcat((S8 *)hintData[(index)],(PS8)UnicodenStrItemList);
	}
	else
	{
		pfnUnicodeStrncpy((S8*)hintData[(index)], ( S8*)GetString(STR_PROFILES_SILENT),MAX_SUBMENU_CHARACTERS);
	}
}
/**************************************************************

	FUNCTION NAME		: HintHandlerPowerOffTone(void)

  	PURPOSE				: Hint handlers for Power Off Tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintHandlerPowerOffTone(U16 index)
{
	U8 UnicodenStrItemList[10]; /* Stores the strings id of submenus returned */
	U8 AsciinStrItemList[4]; /* Stores the strings id of submenus returned */
	U8 data = 0;
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	if((gprofiles[gprofileId].toneSetup.powerOffTone)!= POWEROFF_SILENT)
	{
		S32 len=0;
		data = ((gprofiles[gprofileId].toneSetup.powerOffTone) - (gstartGeneralToneId));
		memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
		sprintf((PS8)AsciinStrItemList , "%d",data+1);
		AnsiiToUnicodeString((PS8)UnicodenStrItemList,(PS8)AsciinStrItemList);
		len=pfnUnicodeStrlen((PS8)UnicodenStrItemList);
		len=MAX_SUBMENU_CHARACTERS-len;
		pfnUnicodeStrncpy((S8 *)hintData[(index)],GetString(STR_PROFILES_TONE),MAX_SUBMENU_CHARACTERS);
		pfnUnicodeStrcat((S8 *)hintData[(index)],(PS8)UnicodenStrItemList);
	}
	else
	{
		pfnUnicodeStrncpy((S8*)hintData[(index)], ( S8*)GetString(STR_PROFILES_SILENT),MAX_SUBMENU_CHARACTERS);
	}
}

//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
/**************************************************************

	FUNCTION NAME		: HintHandlerCoverOpenTone(void)

  	PURPOSE				: Hint handlers for Cover Open

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintHandlerCoverOpenTone(U16 index)
{
	U8 UnicodenStrItemList[10]; /* Stores the strings id of submenus returned */
	U8 AsciinStrItemList[4]; /* Stores the strings id of submenus returned */
	U8 data = 0;
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	if((gprofiles[gprofileId].toneSetup.coverOpenTone)!= COVER_OPEN_SILENT)
	{
		S32 len=0;
		data = ((gprofiles[gprofileId].toneSetup.coverOpenTone) - (gstartGeneralToneId));
		memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
		#ifdef __MMI_MAINLCD_220X176__
		sprintf((PS8)AsciinStrItemList , "%d",data+1-4);
		#else
		sprintf((PS8)AsciinStrItemList , "%d",data+1);
		#endif
		AnsiiToUnicodeString((PS8)UnicodenStrItemList,(PS8)AsciinStrItemList);
		len=pfnUnicodeStrlen((PS8)UnicodenStrItemList);
		len=MAX_SUBMENU_CHARACTERS-len;
		pfnUnicodeStrncpy((S8 *)hintData[(index)],GetString(STR_PROFILES_TONE),MAX_SUBMENU_CHARACTERS);
		pfnUnicodeStrcat((S8 *)hintData[(index)],(PS8)UnicodenStrItemList);
	}
	else
	{
		pfnUnicodeStrncpy((S8*)hintData[(index)], ( S8*)GetString(STR_PROFILES_SILENT),MAX_SUBMENU_CHARACTERS);
	}
}
/**************************************************************

	FUNCTION NAME		: HintHandlerCoverCloseTone(void)

  	PURPOSE				: Hint handlers for Cover Close

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintHandlerCoverCloseTone(U16 index)
{
	U8 UnicodenStrItemList[10]; /* Stores the strings id of submenus returned */
	U8 AsciinStrItemList[4]; /* Stores the strings id of submenus returned */
	U8 data = 0;
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	if((gprofiles[gprofileId].toneSetup.coverCloseTone)!=COVER_CLOSE_SILENT)
	{
		S32 len=0;
		data = ((gprofiles[gprofileId].toneSetup.coverCloseTone) - (gstartGeneralToneId));
		memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
		#ifdef __MMI_MAINLCD_220X176__
		sprintf((PS8)AsciinStrItemList , "%d",data+1-4);
		#else
		sprintf((PS8)AsciinStrItemList , "%d",data+1);
		#endif
		AnsiiToUnicodeString((PS8)UnicodenStrItemList,(PS8)AsciinStrItemList);
		len=pfnUnicodeStrlen((PS8)UnicodenStrItemList);
		len=MAX_SUBMENU_CHARACTERS-len;
		pfnUnicodeStrncpy((S8 *)hintData[(index)],GetString(STR_PROFILES_TONE),MAX_SUBMENU_CHARACTERS);
		pfnUnicodeStrcat((S8 *)hintData[(index)],(PS8)UnicodenStrItemList);
	}
	else
	{
		pfnUnicodeStrncpy((S8*)hintData[(index)], ( S8*)GetString(STR_PROFILES_SILENT),MAX_SUBMENU_CHARACTERS);
	}
}
#endif
//CSD end

/**************************************************************

	FUNCTION NAME		: HintHandlerMessgaeTone(void)

  	PURPOSE				: Hint handlers for Message Tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintHandlerMessgaeTone(U16 index)
{
	U8 UnicodenStrItemList[10]; /* Stores the strings id of submenus returned */
	U8 AsciinStrItemList[4]; /* Stores the strings id of submenus returned */
	U8 data = 0;
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	if((gprofiles[gprofileId].toneSetup.messageTone)!= MESSAGE_SILENT)
	{
		S32 len=0;
		data = ((gprofiles[gprofileId].toneSetup.messageTone) - (gstartMiscToneId));
		memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
		sprintf((PS8)AsciinStrItemList , "%d",data+1);
		AnsiiToUnicodeString((PS8)UnicodenStrItemList,(PS8)AsciinStrItemList);
		len=pfnUnicodeStrlen((PS8)UnicodenStrItemList);
		len=MAX_SUBMENU_CHARACTERS-len;
		pfnUnicodeStrncpy((S8 *)hintData[(index)],GetString(STR_PROFILES_TONE),MAX_SUBMENU_CHARACTERS);
		pfnUnicodeStrcat((S8 *)hintData[(index)],(PS8)UnicodenStrItemList);
	}
	else
	{
		pfnUnicodeStrncpy((S8*)hintData[(index)], ( S8*)GetString(STR_PROFILES_SILENT),MAX_SUBMENU_CHARACTERS);
	}
}

#ifdef __MMI_DUAL_PROFILE_SUPPORT__
/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_hint_hdlr_card2_message_tone
 * DESCRIPTION
 *  Hint handler for card2 message tone
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_hint_hdlr_card2_message_tone(U16 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 UnicodenStrItemList[10]; /* Stores the strings id of submenus returned */
    U8 AsciinStrItemList[4];    /* Stores the strings id of submenus returned */
    U8 data = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    memset((S8*) hintData[(index)], 0, MAX_SUB_MENU_SIZE);
    
    if ((gprofiles[gprofileId].toneSetup.card2_messageTone) != MESSAGE_SILENT)
    {
        S32 len = 0;
        data = ((gprofiles[gprofileId].toneSetup.card2_messageTone) - (gstartMiscToneId));
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	sprintf((PS8)AsciinStrItemList , "%d",data+1);
	AnsiiToUnicodeString((PS8)UnicodenStrItemList,(PS8)AsciinStrItemList);
	len=pfnUnicodeStrlen((PS8)UnicodenStrItemList);
	len=MAX_SUBMENU_CHARACTERS-len;
	pfnUnicodeStrncpy((S8 *)hintData[(index)],GetString(STR_PROFILES_TONE),MAX_SUBMENU_CHARACTERS);
	pfnUnicodeStrcat((S8 *)hintData[(index)],(PS8)UnicodenStrItemList);
    }
    else
    {
	pfnUnicodeStrncpy((S8*)hintData[(index)], ( S8*)GetString(STR_PROFILES_SILENT),MAX_SUBMENU_CHARACTERS);
    }
}
#endif

/**************************************************************

	FUNCTION NAME		: HintHandlerKeypadTone(void)

  	PURPOSE				: Hint handlers for Keypad Tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintHandlerKeypadTone(U16 index)
{
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	switch(gprofiles[gprofileId].toneSetup.keypadTone)
		{
	case KEYPAD_SILENT:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_KEYPAD_TONE_SILENT),MAX_SUBMENU_CHARACTERS);
		break;
	case KEYPAD_CLICK:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_KEYPAD_TONE_CLICK),MAX_SUBMENU_CHARACTERS);
    	break;
	case KEYPAD_TONE:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_KEYPAD_TONE_TONE),MAX_SUBMENU_CHARACTERS);
	    break;
#if defined( __MMI_HUMAN_VOICE_KEYPAD_TONE__ )
	case KEYPAD_TONE_HUMAN_VOICE_1:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_KEYPAD_TONE_HUMAN_VOICE_1),MAX_SUBMENU_CHARACTERS);
		break;
	case KEYPAD_TONE_HUMAN_VOICE_2:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_KEYPAD_TONE_HUMAN_VOICE_2),MAX_SUBMENU_CHARACTERS);
		break;
	case KEYPAD_TONE_HUMAN_VOICE_3:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_KEYPAD_TONE_HUMAN_VOICE_3),MAX_SUBMENU_CHARACTERS);
		break;
	case KEYPAD_TONE_HUMAN_VOICE_4:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_KEYPAD_TONE_HUMAN_VOICE_4),MAX_SUBMENU_CHARACTERS);
		break;
	case KEYPAD_TONE_HUMAN_VOICE_5:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_KEYPAD_TONE_HUMAN_VOICE_5),MAX_SUBMENU_CHARACTERS);
		break;
	case KEYPAD_TONE_HUMAN_VOICE_6:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_KEYPAD_TONE_HUMAN_VOICE_6),MAX_SUBMENU_CHARACTERS);
		break;
#endif
#if defined( __MMI_CUST_KEYPAD_TONE__ )
	case KEYPAD_TONE_CUST_1:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_KEYPAD_TONE_CUST_1),MAX_SUBMENU_CHARACTERS);
		break;
	case KEYPAD_TONE_CUST_2:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_KEYPAD_TONE_CUST_2),MAX_SUBMENU_CHARACTERS);
		break;
	case KEYPAD_TONE_CUST_3:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_KEYPAD_TONE_CUST_3),MAX_SUBMENU_CHARACTERS);
		break;
	case KEYPAD_TONE_CUST_4:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_KEYPAD_TONE_CUST_4),MAX_SUBMENU_CHARACTERS);
		break;
	case KEYPAD_TONE_CUST_5:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_KEYPAD_TONE_CUST_5),MAX_SUBMENU_CHARACTERS);
		break;
	case KEYPAD_TONE_CUST_6:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_KEYPAD_TONE_CUST_6),MAX_SUBMENU_CHARACTERS);
		break;
#endif
	}
}
/**************************************************************

	FUNCTION NAME		: HintHandlerAlertType(void)

  	PURPOSE				: Hint handlers for Alert Type
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintHandlerAlertType(U16 index)
{
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	if(gprofiles[gprofileId].mtCallAlertTypeEnum == MMI_RING)
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_RING),MAX_SUBMENU_CHARACTERS);
	else if(gprofiles[gprofileId].mtCallAlertTypeEnum == MMI_VIBRATION_ONLY)
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_VIBRATION_ONLY),MAX_SUBMENU_CHARACTERS);
	else if(gprofiles[gprofileId].mtCallAlertTypeEnum == MMI_VIBRATION_AND_RING)
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_VIBRATION_AND_RING),MAX_SUBMENU_CHARACTERS);
	else if(gprofiles[gprofileId].mtCallAlertTypeEnum == MMI_VIBRATION_THEN_RING)
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_VIBRATION_THEN_RING),MAX_SUBMENU_CHARACTERS);
	else
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_SILENT),MAX_SUBMENU_CHARACTERS);
}
/**************************************************************

	FUNCTION NAME		: HintHandlerRingType(void)

  	PURPOSE				: Hint handlers for Ring Type

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintHandlerRingType(U16 index)
{
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	switch(gprofiles[gprofileId].ringTypeEnum)
		{
	case SINGLE:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_RING_TYPE_SINGLE),MAX_SUBMENU_CHARACTERS);
		break;
	case REPEAT:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_RING_TYPE_REPEAT),MAX_SUBMENU_CHARACTERS);
		break;
	case ASCENDING:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_RING_TYPE_ASCENDNG),MAX_SUBMENU_CHARACTERS);
        break;
	}
}
#ifdef __MMI_CONNECT_NOTICE__

/**************************************************************

	FUNCTION NAME		: HintHandlerConnectNotice(void)

  	PURPOSE				: Hint handlers for Connect Notice Type

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintHandlerConnectNotice(U16 index)
{
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	pfnUnicodeStrncpy(( PS8 ) hintData[(index)],
	                  GetString(STR_NOTICE_NONE+gprofiles[gprofileId].extraTone.connectTone),
	                  MAX_SUBMENU_CHARACTERS);
}
#endif
//CSD Brian added for wrapping for Changable font size, 2003/11/11
#ifdef __MMI_CHANGABLE_FONT__
/**************************************************************

	FUNCTION NAME		: HintHandlerFontSize(void)

  	PURPOSE				: Hint handlers for Font Size

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintHandlerFontSize(U16 index)
{
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);
	switch(gprofiles[gprofileId].fontSizeEnum)
		{
	case THEME_DEFAULT:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_FONT_SIZE_THEME_DEFAULT),MAX_SUBMENU_CHARACTERS);
		break;
	case SMALL:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_FONT_SIZE_SMALL),MAX_SUBMENU_CHARACTERS);
		break;
	case MEDIUM:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_FONT_SIZE_MEDIUM),MAX_SUBMENU_CHARACTERS);
        break;
	case LARGE:
		pfnUnicodeStrncpy(( PS8 ) hintData[(index)],GetString(STR_FONT_SIZE_LARGE),MAX_SUBMENU_CHARACTERS);
	    break;
	}
}
#endif
/**************************************************************

	FUNCTION NAME		: HintHandlerRingVolume(void)

  	PURPOSE				: Hint handlers for Ring Volume

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintHandlerRingVolume(U16 index)
{
	U8 value[MAX_SUB_MENU_HINT_SIZE];

	sprintf( ( S8 * ) value, "%d", (gprofiles[gprofileId].ringVolumeLevel ));

	#ifdef __UCS2_ENCODING
		AnsiiToUnicodeString( ( PS8 ) hintData[(index)], ( PS8 ) value );
	#else
		pfnUnicodeStrcpy( ( S8 * ) hintData[index], ( S8 * ) value);
	#endif
}
/**************************************************************

	FUNCTION NAME		: HintHandlerKeyVolume(void)

  	PURPOSE				: Hint handlers for Key Volume

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintHandlerKeyVolume(U16 index)
{
	U8 value[MAX_SUB_MENU_HINT_SIZE];

	sprintf( ( S8 * ) value, "%d", (gprofiles[gprofileId].keypadVolumeLevel ));

	#ifdef __UCS2_ENCODING
		AnsiiToUnicodeString( ( PS8 ) hintData[(index)], ( PS8 ) value );
	#else
		pfnUnicodeStrcpy( ( S8 * ) hintData[index], ( S8 * ) value);
	#endif
}
#if defined(__CALL_VOLUME_LEVEL__)
//wug add used to pc300 start 20071010
/**************************************************************

	FUNCTION NAME		: HintHandlerCallVolume(void)

  	PURPOSE				: Hint handlers for Call Volume

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintHandlerCallVolume(U16 index)
{
	U8 value[MAX_SUB_MENU_HINT_SIZE];
	sprintf( ( S8 * ) value, "%d", GetModeSpeechVolume()+1);
	#ifdef __UCS2_ENCODING
		AnsiiToUnicodeString( ( PS8 ) hintData[(index)], ( PS8 ) value );
	#else
		pfnUnicodeStrcpy( ( S8 * ) hintData[index], ( S8 * ) value);
	#endif
}
//wug add used to pc300 end 20071010
#endif
/**************************************************************

	FUNCTION NAME		: HintHandlerLCDBacklight(void)

  	PURPOSE				: Hint handlers for LCD BackLight

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintHandlerLCDBacklight(U16 index)
{
	U8 value[MAX_SUB_MENU_HINT_SIZE];
	mmi_trace(g_sw_SETTING, TSTXT("###### in HintHandlerLCDBacklight() ############\n") ); 

#ifdef __MMI_MAINLCD_220X176__ 	
       sprintf( ( S8 * ) value, "%d", gprofiles[0].light.timer.setsec);
#else
	sprintf( ( S8 * ) value, "%d", gprofiles[gprofileId].light.timer.setsec);
#endif
	#ifdef __UCS2_ENCODING
		AnsiiToUnicodeString( ( PS8 ) hintData[(index)], ( PS8 ) value );
	#else
		pfnUnicodeStrcpy( ( S8 * ) hintData[index], ( S8 * ) value);
	#endif
}
#ifdef __MMI_STATUS_LED__
/**************************************************************

	FUNCTION NAME		: HintHandlerStatusLED(void)

  	PURPOSE				: Hint handlers for Status LED

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HintHandlerStatusLED(U16 index)
{
	memset((S8 *)hintData[(index)],0,MAX_SUB_MENU_SIZE);

	switch(gprofiles[gprofileId].light.status)
	{

	case NO_COLOR:
		pfnUnicodeStrncpy((S8*) hintData[(index)],GetString(STR_LIGHT_STATUS_NONE),MAX_SUBMENU_CHARACTERS);
	    break;
	case COLOR_PATTERN1:
		pfnUnicodeStrncpy((S8*) hintData[(index)],GetString(STR_LIGHT_STATUS_COLOR1),MAX_SUBMENU_CHARACTERS);
		break;
	case COLOR_PATTERN2:
		pfnUnicodeStrncpy((S8*) hintData[(index)],GetString(STR_LIGHT_STATUS_COLOR2),MAX_SUBMENU_CHARACTERS);
		break;
	case COLOR_PATTERN3:
		pfnUnicodeStrncpy((S8*) hintData[(index)],GetString(STR_LIGHT_STATUS_COLOR3),MAX_SUBMENU_CHARACTERS);
		break;
	case COLOR_PATTERN4:
		pfnUnicodeStrncpy((S8*) hintData[(index)],GetString(STR_LIGHT_STATUS_COLOR4),MAX_SUBMENU_CHARACTERS);
		break;
	case COLOR_PATTERN5:
		pfnUnicodeStrncpy((S8*) hintData[(index)],GetString(STR_LIGHT_STATUS_COLOR5),MAX_SUBMENU_CHARACTERS);
		break;
	case COLOR_PATTERN6:
		pfnUnicodeStrncpy((S8*) hintData[(index)],GetString(STR_LIGHT_STATUS_COLOR6),MAX_SUBMENU_CHARACTERS);
		break;
	}
}
#endif

/************************************HighLight Handlers***************************/

/**************************************************************

	FUNCTION NAME		: HighlightProfiles(void)

  	PURPOSE				: Highlight profile menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightProfiles(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfiles,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfiles,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

}
/**************************************************************

	FUNCTION NAME		: HighlightGeneralProfile(void)

  	PURPOSE				: Highlight General profile menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightGeneralProfile(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OPTIONS,IMG_GLOBAL_OPTIONS);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
#ifdef __MMI_PROF_MENU_ACTIVE_SHRINK__
	SetLeftSoftkeyFunction(EntryScrProfilesCustomize,KEY_EVENT_UP);
	SetKeyHandler(EntryScrProfilesCustomize,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#else
	SetLeftSoftkeyFunction(EntryScrProfilesOptions,KEY_EVENT_UP);
        SetKeyHandler(EntryScrProfilesOptions,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#endif
	gcurrHiliteIndexOne = MMI_PROFILE_GENERAL;

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}
/**************************************************************

	FUNCTION NAME		: HighlightMeetingProfile(void)

  	PURPOSE				: Highlight Meeting profile menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightMeetingProfile(void)
{
		/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OPTIONS,IMG_GLOBAL_OPTIONS);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	gcurrHiliteIndexOne = MMI_PROFILE_MEETING;

	/*3 Register function for left soft key */
#ifdef __MMI_PROF_MENU_ACTIVE_SHRINK__
	SetLeftSoftkeyFunction(EntryScrProfilesCustomize,KEY_EVENT_UP);
	SetKeyHandler(EntryScrProfilesCustomize,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#else
	SetLeftSoftkeyFunction(EntryScrProfilesOptions,KEY_EVENT_UP);
	SetKeyHandler(EntryScrProfilesOptions,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#endif

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}
/**************************************************************

	FUNCTION NAME		: HighlightOutdoorProfile(void)

  	PURPOSE				: Highlight Outdoor profile menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightOutdoorProfile(void)
{
		/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OPTIONS,IMG_GLOBAL_OPTIONS);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	gcurrHiliteIndexOne = MMI_PROFILE_OUTDOOR;

	/*3 Register function for left soft key */
#ifdef __MMI_PROF_MENU_ACTIVE_SHRINK__
	SetLeftSoftkeyFunction(EntryScrProfilesCustomize,KEY_EVENT_UP);
        SetKeyHandler(EntryScrProfilesCustomize,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#else
	SetLeftSoftkeyFunction(EntryScrProfilesOptions,KEY_EVENT_UP);
	SetKeyHandler(EntryScrProfilesOptions,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#endif

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}
/**************************************************************

	FUNCTION NAME		: HighlightIndoorProfile(void)

  	PURPOSE				: Highlight Indoor profile menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightIndoorProfile(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OPTIONS,IMG_GLOBAL_OPTIONS);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	gcurrHiliteIndexOne = MMI_PROFILE_INDOOR;

	/*3 Register function for left soft key */
#ifdef __MMI_PROF_MENU_ACTIVE_SHRINK__
	SetLeftSoftkeyFunction(EntryScrProfilesCustomize,KEY_EVENT_UP);
	SetKeyHandler(EntryScrProfilesCustomize,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#else
	SetLeftSoftkeyFunction(EntryScrProfilesOptions,KEY_EVENT_UP);
	SetKeyHandler(EntryScrProfilesOptions,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#endif

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}
/**************************************************************

	FUNCTION NAME		: HighlightHeadsetProfile(void)

  	PURPOSE				: Highlight Headset profile menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightHeadsetProfile(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OPTIONS,IMG_GLOBAL_OPTIONS);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	gcurrHiliteIndexOne = MMI_PROFILE_HEADSET;

	/*3 Register function for left soft key */
#ifdef __MMI_PROF_MENU_ACTIVE_SHRINK__
	SetLeftSoftkeyFunction(EntryScrProfilesCustomize,KEY_EVENT_UP);
	SetKeyHandler(EntryScrProfilesCustomize,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#else
	SetLeftSoftkeyFunction(EntryScrProfilesOptions,KEY_EVENT_UP);
	SetKeyHandler(EntryScrProfilesOptions,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#endif

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}
#ifdef __MMI_BT_PROFILE__


/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_hilite_hdlr_bt_mode
 * DESCRIPTION
 *  Highlight handler for bt mode
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_hilite_hdlr_bt_mode(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
		/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OPTIONS,IMG_GLOBAL_OPTIONS);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

    gcurrHiliteIndexOne = MMI_PROFILE_BT;

    /* Register function for left soft key */
#ifdef __MMI_PROF_MENU_ACTIVE_SHRINK__
	SetLeftSoftkeyFunction(EntryScrProfilesCustomize,KEY_EVENT_UP);
	SetKeyHandler(EntryScrProfilesCustomize,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#else /* __MMI_PROF_MENU_ACTIVE_SHRINK__ */ 
	SetLeftSoftkeyFunction(EntryScrProfilesOptions,KEY_EVENT_UP);
	SetKeyHandler(EntryScrProfilesOptions,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
#endif /* __MMI_PROF_MENU_ACTIVE_SHRINK__ */ 


	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}
#endif /* __MMI_BT_PROFILE__ */ /* MMI_BT_PROFILE__ */

/**************************************************************

	FUNCTION NAME		: HighlightProfilesOptions(void)

  	PURPOSE				: Highlight profile Options menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightProfilesOptions(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
/**********************************************************************

	FUNCTION NAME		: HighlightProfileActivateOptions(void)

  	PURPOSE				: Highlight profile Activate Options menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**********************************************************************/
void HighlightProfileActivateOptions(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesActivate,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	 
	ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	// CSD end


}
/**************************************************************

	FUNCTION NAME		: HighlightProfilesCustomize(void)

  	PURPOSE				: Highlight profile Customize menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightProfilesCustomize(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesCustomize,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfilesCustomize,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

}
/**************************************************************

	FUNCTION NAME		: HighlightProfilesVolumeLevel(void)

  	PURPOSE				: Highlight profile Volume Level

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightProfilesVolumeLevel(void)
{
	mmi_trace(g_sw_SETTING, TSTXT("###### in HighlightProfilesVolumeLevel() \n")); 
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesVolumeType,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfilesVolumeType,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
/**************************************************************

	FUNCTION NAME		: HighlightProfilesCallAlert(void)

  	PURPOSE				: Highlight profile Call Alert menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightProfilesCallAlert(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesCallAlert,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfilesCallAlert,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);


}
/**************************************************************

	FUNCTION NAME		: HighlightProfilesRingType(void)

  	PURPOSE				: Highlight profile Call Alert menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightProfilesRingType(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesRingType,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfilesRingType,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
/**************************************************************

	FUNCTION NAME		: HighlightProfilesAnsweringMode(void)

  	PURPOSE				: Highlight profile Answering mode menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightProfilesAnsweringMode(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesAnsweringMode,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfilesAnsweringMode,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
//CSD Brian added for wrapping for Changable font size, 2003/11/11
#ifdef __MMI_CHANGABLE_FONT__
/**************************************************************

	FUNCTION NAME		: HighlightProfilesFontSize(void)

  	PURPOSE				: Highlight profile Font Size menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightProfilesFontSize(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesFontSize,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfilesFontSize,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
#endif

#ifdef __MMI_STATUS_LED__
/**************************************************************

	FUNCTION NAME		: HighlightProfilesLight(void)

  	PURPOSE				: Highlight profile Light menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightProfilesLight(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesLight,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfilesLight,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
#endif
/**************************************************************

	FUNCTION NAME		: HighlightProfilesSetTimer(void)

  	PURPOSE				: Highlight profile SetTimer menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightProfilesSetTimer(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesSetTimer,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfilesSetTimer,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
#ifdef __MMI_STATUS_LED__
/**************************************************************

	FUNCTION NAME		: HighlightProfilesStatus(void)

  	PURPOSE				: Highlight profile Status menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightProfilesStatus(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrStatusLEDPatterns,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrStatusLEDPatterns,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
#endif
/**************************************************************

	FUNCTION NAME		: HighlightProfilesToneSetup(void)

  	PURPOSE				: Highlight profile Tone Setup menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightProfilesToneSetup(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesToneSetup,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfilesToneSetup,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

mmi_trace(1,"MENU ID->MENU_TONE_SETUP  %d",MENU_TONE_SETUP);
mmi_trace(1,"MENU ID->MENU_TONE_SETUP_RING_TONE  %d",MENU_TONE_SETUP_RING_TONE);
mmi_trace(1,"MENU ID->MENU_TONE_SETUP_ALARM_TONE  %d",MENU_TONE_SETUP_ALARM_TONE);
mmi_trace(1,"MENU ID->MENU_TONE_SETUP_POWERON_TONE  %d",MENU_TONE_SETUP_POWERON_TONE);
mmi_trace(1,"MENU ID->MENU_TONE_SETUP_POWEROFF_TONE  %d",MENU_TONE_SETUP_POWEROFF_TONE);
mmi_trace(1,"MENU ID->MENU_TONE_SETUP_MESSAGE_TONE  %d",MENU_TONE_SETUP_MESSAGE_TONE);
mmi_trace(1,"MENU ID->MENU_TONE_SETUP_KEYPAD_TONE  %d",MENU_TONE_SETUP_KEYPAD_TONE);
}
/**************************************************************

	FUNCTION NAME		: HighlightProfilesIncomingCallToneSetup(void)

  	PURPOSE				: Highlight profile Ring Tone Setup menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightProfilesIncomingCallToneSetup(void)   //Modified by Jinzh:20070617
{
	mmi_trace(g_sw_SETTING, TSTXT("###### in HighlightProfilesIncomingCallToneSetup() ############\n") ); 
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(mmi_profiles_entry_ring_tone_list,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(mmi_profiles_entry_ring_tone_list,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}

#ifdef __MMI_DUAL_PROFILE_SUPPORT__  //Add by Renyh for 221 project   20090213
/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_hilite_hdlr_card2_ring_tone
 * DESCRIPTION
 *  Highlight handler for card2 ring tone setup
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_hilite_hdlr_card2_ring_tone(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    /*1 Change left soft key icon and label */
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

    /*2 Change right soft key icon and label */
    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);

    /*3 Register function for left soft key */
    SetLeftSoftkeyFunction(mmi_profiles_entry_card2_ring_tone_list, KEY_EVENT_UP);

    /*4 Register function for right soft key */
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

    SetKeyHandler(mmi_profiles_entry_card2_ring_tone_list, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}
#endif

/**************************************************************

	FUNCTION NAME		: HighlightProfilesPoweronToneSetup(void)

  	PURPOSE				: Highlight profile Poweron Tone Setup menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightProfilesPoweronToneSetup(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesPoweronToneSetup,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfilesPoweronToneSetup,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
/**************************************************************

	FUNCTION NAME		: HighlightProfilesPoweroffToneSetup(void)

  	PURPOSE				: Highlight profile Poweroff Tone Setup menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightProfilesPoweroffToneSetup(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesPoweroffToneSetup,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfilesPoweroffToneSetup,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
/**************************************************************

	FUNCTION NAME		: HighlightProfilesMessageToneSetup(void)

  	PURPOSE				: Highlight profile Message Tone Setup menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightProfilesMessageToneSetup(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesMessageToneSetup,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfilesMessageToneSetup,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}

#ifdef __MMI_DUAL_PROFILE_SUPPORT__      //Add by Renyh for 221 project   20090213
/*****************************************************************************
 * FUNCTION
 *  mmi_profiles_hilite_hdlr_card2_message_tone
 * DESCRIPTION
 *  Highlight handler for card2 message tone setup
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_profiles_hilite_hdlr_card2_message_tone(void)
{
    /*1 Change left soft key icon and label */
    ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

    /*2 Change right soft key icon and label */
    ChangeRightSoftkey(STR_GLOBAL_BACK, 0);

    /*3 Register function for left soft key */
    SetLeftSoftkeyFunction(mmi_profiles_entry_card2_message_tone, KEY_EVENT_UP);

    /*4 Register function for right soft key */
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

    SetKeyHandler(mmi_profiles_entry_card2_message_tone, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}
#endif      //Add by Renyh for 221 project   20090213

/**************************************************************

	FUNCTION NAME		: HighlightProfilesKeypadToneSetup(void)

  	PURPOSE				: Highlight profile Keypad Tone Setup menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightProfilesKeypadToneSetup(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesKeypadToneSetup,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfilesKeypadToneSetup,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}

#ifndef __MMI_ALM_CUSTOMIZE_TONE__
/**************************************************************

	FUNCTION NAME		: HighlightAlarmToneSetup(void)

  	PURPOSE				: Highlight profile Alarm Tone Setup menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightAlarmToneSetup(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesAlarmToneSetup,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfilesAlarmToneSetup,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
#endif

/**************************************************************

	FUNCTION NAME		: HighlightExtraToneSetup(void)

  	PURPOSE				: Highlight profile Extra Tone Setup menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightExtraToneSetup(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesExtraToneSetup,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfilesExtraToneSetup,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
/**************************************************************

	FUNCTION NAME		: HighlightRingToneVolume(void)

  	PURPOSE				: Adjust Ring Tone Volume

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightRingToneVolume(void)
{
	mmi_trace(g_sw_SETTING, TSTXT("###### in HighlightRingToneVolume() ############\n") ); 
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesCommonVolumeLevel1,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfilesCommonVolumeLevel1,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	gcommonvolumehandler=0;
}
/**************************************************************

	FUNCTION NAME		: HighlightKeyToneVolume(void)

  	PURPOSE				: Adjust KeyPad Tone Volume

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightKeyToneVolume(void)
{
	mmi_trace(g_sw_SETTING, TSTXT("###### in HighlightKeyToneVolume() ############\n") ); 
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesCommonVolumeLevel1,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfilesCommonVolumeLevel1,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	gcommonvolumehandler=1;
}

#if defined(__CALL_VOLUME_LEVEL__)
//wug add used to pc300 start 20071010
/**************************************************************

	FUNCTION NAME		: HighlightCallToneVolume(void)

  	PURPOSE				: Adjust Call Tone Volume

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCallToneVolume(void)
{
	mmi_trace(1, "HighlightCallToneVolume" ); 
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesCommonVolumeLevel1,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfilesCommonVolumeLevel1,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	gcommonvolumehandler=2;
}
//wug add used to pc300 end 20071010
#endif
/**************************************************************

	FUNCTION NAME		: HighlightCallAlertRing(void)

  	PURPOSE				: Set Call Alert as Ring

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCallAlertRing(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(SetMtCallAlertRing,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}
/**************************************************************

	FUNCTION NAME		: HighlightCalAlertVibrationOnly(void)

  	PURPOSE				: Set Call Alert as Vibration Only

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCalAlertVibrationOnly(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(SetMtCallAlertVibrationOnly,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}
/**************************************************************

	FUNCTION NAME		: HighlightCallAlertVibrationAndRing(void)

  	PURPOSE				: Set Call Alert as Vibration and Ring

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCallAlertVibrationAndRing(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(SetMtCallAlertVibrationAndRing,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
/**************************************************************

	FUNCTION NAME		: HighlightCallAlertVibrationThenRing(void)

  	PURPOSE				: Set Call Alert as Vibration then Ring

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCallAlertVibrationThenRing(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(SetMtCallAlertVibrationThenRing,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
/**************************************************************

	FUNCTION NAME		: HighlightCallAlertSilent(void)

  	PURPOSE				: Set Call Alert as Silent

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightCallAlertSilent(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(SetMtCallAlertSilent,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
/**************************************************************

	FUNCTION NAME		: HighlightRingTypeSingle(void)

  	PURPOSE				: Highlight Ring type Single

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightRingTypeSingle(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(SetRingTypeSingle,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}
/**************************************************************

	FUNCTION NAME		: HighlightRingTypeRepeat(void)

  	PURPOSE				: Highlight Ring type Repeat

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightRingTypeRepeat(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(SetRingTypeRepeat,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
/**************************************************************

	FUNCTION NAME		: HighlightRingTypeAscending(void)

  	PURPOSE				: Highlight Ring type Ascending

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightRingTypeAscending(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(SetRingTypeAscending,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

//CSD Brian added for wrapping for Changable font size, 2003/11/11
#ifdef __MMI_CHANGABLE_FONT__
/**************************************************************

	FUNCTION NAME		: HighlightFontSizeDefault(void)

  	PURPOSE				: Highlight Font Size Default

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightFontSizeDefault(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(SetFontSizeDefault,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}
/**************************************************************

	FUNCTION NAME		: HighlightFontSizeSmall(void)

  	PURPOSE				: Highlight Font Size Small

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightFontSizeSmall(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(SetFontSizeSmall,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}
/**************************************************************

	FUNCTION NAME		: HighlightFontSizeMedium(void)

  	PURPOSE				: Highlight Font Size Medium

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightFontSizeMedium(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(SetFontSizeMedium,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}
//CSD Brian added for wrapping for DIALING FONT, 2003/10/31
#ifndef __MMI_DIALLING_FONT__
/**************************************************************

	FUNCTION NAME		: HighlightFontSizeLarge(void)

  	PURPOSE				: Highlight Font Size Large

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightFontSizeLarge(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetLeftSoftkeyFunction(SetFontSizeLarge,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}
#endif
#endif
/**************************************************************

	FUNCTION NAME		: HighlightKeyPadToneSilent(void)

  	PURPOSE				: Highlight Keypad Tone Silent

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightKeyPadToneSilent(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* 10.Register function with Left softkey */
	SetLeftSoftkeyFunction(SetKeyPadToneSilent,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}
/**************************************************************

	FUNCTION NAME		: HighlightKeyPadToneClick(void)

  	PURPOSE				: Highlight Keypad Tone Click

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightKeyPadToneClick(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* 10.Register function with Left softkey */
	SetLeftSoftkeyFunction(SetKeyPadToneClick,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}
/**************************************************************

	FUNCTION NAME		: HighlightKeyPadToneTone(void)

  	PURPOSE				: Highlight Keypad Tone as Tone

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightKeyPadToneTone(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* 10.Register function with Left softkey */
	SetLeftSoftkeyFunction(SetKeyPadToneTone,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}

#if defined( __MMI_HUMAN_VOICE_KEYPAD_TONE__ )
void HighlightKeyPadToneHumanVoice1(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/* 10.Register function with Left softkey */
	SetLeftSoftkeyFunction(SetKeyPadToneHumanVoice1,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
void HighlightKeyPadToneHumanVoice2(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/* 10.Register function with Left softkey */
	SetLeftSoftkeyFunction(SetKeyPadToneHumanVoice2,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
void HighlightKeyPadToneHumanVoice3(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/* 10.Register function with Left softkey */
	SetLeftSoftkeyFunction(SetKeyPadToneHumanVoice3,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
void HighlightKeyPadToneHumanVoice4(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/* 10.Register function with Left softkey */
	SetLeftSoftkeyFunction(SetKeyPadToneHumanVoice4,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
void HighlightKeyPadToneHumanVoice5(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/* 10.Register function with Left softkey */
	SetLeftSoftkeyFunction(SetKeyPadToneHumanVoice5,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
void HighlightKeyPadToneHumanVoice6(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/* 10.Register function with Left softkey */
	SetLeftSoftkeyFunction(SetKeyPadToneHumanVoice6,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
#endif

#if defined( __MMI_CUST_KEYPAD_TONE__ )
void HighlightKeyPadToneCust1(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/* 10.Register function with Left softkey */
	SetLeftSoftkeyFunction(SetKeyPadToneCust1,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
void HighlightKeyPadToneCust2(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/* 10.Register function with Left softkey */
	SetLeftSoftkeyFunction(SetKeyPadToneCust2,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
void HighlightKeyPadToneCust3(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/* 10.Register function with Left softkey */
	SetLeftSoftkeyFunction(SetKeyPadToneCust3,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
void HighlightKeyPadToneCust4(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/* 10.Register function with Left softkey */
	SetLeftSoftkeyFunction(SetKeyPadToneCust4,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
void HighlightKeyPadToneCust5(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/* 10.Register function with Left softkey */
	SetLeftSoftkeyFunction(SetKeyPadToneCust5,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
void HighlightKeyPadToneCust6(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/* 10.Register function with Left softkey */
	SetLeftSoftkeyFunction(SetKeyPadToneCust6,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
#endif

#ifdef __MMI_CONNECT_NOTICE__
/**************************************************************

	FUNCTION NAME		: HighlightProfilesConnectNotice(void)

  	PURPOSE				: Highlight profile Call Alert menu item

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightProfilesConnectNotice(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrProfilesConnectNotice,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(EntryScrProfilesConnectNotice,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);


}

/**************************************************************

	FUNCTION NAME		: HighlightConnectNoticeNone(void)

  	PURPOSE				: Highlight Connect Notice None

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightConnectNoticeNone(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* 10.Register function with Left softkey */
	SetLeftSoftkeyFunction(SetConnectNoticeNone,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}

/**************************************************************

	FUNCTION NAME		: HighlightConnectNoticeToneOnly(void)

  	PURPOSE				: Highlight Connect Notice Tone Only

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightConnectNoticeToneOnly(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* 10.Register function with Left softkey */
	SetLeftSoftkeyFunction(SetConnectNoticeToneOnly,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}

/**************************************************************

	FUNCTION NAME		: HighlightConnectNoticeVibOnly(void)

  	PURPOSE				: Highlight Connect Notice Vib Only

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightConnectNoticeVibOnly(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* 10.Register function with Left softkey */
	SetLeftSoftkeyFunction(SetConnectNoticeVibOnly,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}

/**************************************************************

	FUNCTION NAME		: HighlightConnectNoticeToneAndVib(void)

  	PURPOSE				: Highlight Connect Notice Tone & Vib

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void HighlightConnectNoticeToneAndVib(void)
{
	/*1 Change left soft key icon and label */
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);

	/*2 Change right soft key icon and label */
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* 9.Register function with right softkey */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	/* 10.Register function with Left softkey */
	SetLeftSoftkeyFunction(SetConnectNoticeToneAndVib,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

}
#endif

#if defined( __MMI_INTELLIGENT_CALL_ALERT__ )
/**************************************************************
	FUNCTION NAME		: HighlightHandlerIntelligentCallAlert(void)
  	PURPOSE				: HighlightHandlerIntelligentCallAlert
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
 
**************************************************************/
void HighlightHandlerIntelligentCallAlert( void )
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(EntryScrIntelligentCallAlert,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EntryScrIntelligentCallAlert,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}

/**************************************************************
	FUNCTION NAME		: HintHandlerIntelligentCallAlert(U16 index)
  	PURPOSE				: HintHandlerIntelligentCallAlert
	INPUT PARAMETERS	: U16 index
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
 
**************************************************************/
void HintIntelligentCallAlert( U16 index )
{
	U16 str_id;

	memset( (S8 *)hintData[(index)], 0, MAX_SUB_MENU_SIZE );
	if( gprofiles[gprofileId].intelligentCallAlert == MMI_INTELLIGENT_CALL_ALERT_ON )
		str_id = STR_GLOBAL_ON;
	else
		str_id = STR_GLOBAL_OFF;
	pfnUnicodeStrcpy( (S8 *)hintData[(index)], GetString( str_id ) );
}

/**************************************************************
	FUNCTION NAME		: HighlightHandlerIntelligentCallAlertOn(void)
  	PURPOSE				: HighlightHandlerIntelligentCallAlertOn
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
 
**************************************************************/
void HighlightHandlerIntelligentCallAlertOn( void )
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(SetIntelligentCallAlertOn,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

/**************************************************************
	FUNCTION NAME		: HighlightHandlerIntelligentCallAlertOff(void)
  	PURPOSE				: HighlightHandlerIntelligentCallAlertOff
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
 
**************************************************************/
void HighlightHandlerIntelligentCallAlertOff( void )
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(SetIntelligentCallAlertOff,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

/**************************************************************
	FUNCTION NAME		: SetIntelligentCallAlertOn(void)
  	PURPOSE				: SetIntelligentCallAlertOn
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
 
**************************************************************/
void SetIntelligentCallAlertOn(void)
{
	SetIntelligentCallAert( MMI_INTELLIGENT_CALL_ALERT_ON );
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_INTELLIGENT_CALL_ALERT );
}

/**************************************************************
	FUNCTION NAME		: SetIntelligentCallAlertOff(void)
  	PURPOSE				: SetIntelligentCallAlertOff
	INPUT PARAMETERS	: nil
	OUTPUT PARAMETERS	: nil
	RETURNS				: void
 
**************************************************************/
void SetIntelligentCallAlertOff(void)
{
	SetIntelligentCallAert( MMI_INTELLIGENT_CALL_ALERT_OFF );
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
	DeleteScreenIfPresent( SCR_PROFILES_INTELLIGENT_CALL_ALERT );
}
#endif

/*********************End (Hilite HAndlers)*********************************************/
