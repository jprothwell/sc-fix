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
 * Profiles_prot.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Settings application function prototypes
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

	FILENAME	: Profiles_prot.h

  	PURPOSE		: Settings application function prototypes

 

	AUTHOR		: Ashima Bajaj

	DATE		: 

**************************************************************/

#ifndef _PIXCOM_PROFILESPROT_H
#define _PIXCOM_PROFILESPROT_H

#include "mmi_data_types.h"

void EntryScrProfiles(void);
void ExitScrProfiles(void);

void EntryScrProfilesOptions(void);
void ExitScrProfilesOptions(void);

void EntryScrProfilesActivate(void);
void EntryScrProfilesActivateById( U8 profileId );

void EntryScrProfilesCustomize(void);
void ExitScrProfilesCustomize(void);

void EntryScrProfilesVolumeType(void);
void ExitScrProfilesVolumeType(void);

void EntryScrProfilesCallAlert(void);
void ExitScrProfilesCallAlert(void);

void EntryScrProfilesRingType(void);
void ExitScrProfilesRingType(void);

#ifdef __MMI_CONNECT_NOTICE__
void EntryScrProfilesConnectNotice(void);
void ExitScrProfilesConnectNotice(void);
#endif

void GetProfileVolumeLevelHiliteIndex(S32);
void EntryScrProfilesCommonVolumeLevel(void);
void EntryScrProfilesCommonVolumeLevel1(void);
void ExitScrProfilesCommonVolumeLevel(void);

void EntryScrProfilesAnsweringMode(void);
void ExitScrProfilesAnsweringMode(void);

//CSD Brian added for wrapping for Changable font size, 2003/11/11
#ifdef __MMI_CHANGABLE_FONT__
void EntryScrProfilesFontSize(void);
void ExitScrProfilesFontSize(void);
#endif
//void ExitScrProfilesAnsweringMode(void);

void EntryScrProfilesLoudSpeaker(void);
void ExitScrProfilesLoudSpeaker(void);

void EntryScrProfilesSetTimer(void);
void ExitScrProfilesSetTimer(void);

#ifdef __MMI_STATUS_LED__
void EntryScrProfilesLight(void);
void ExitScrProfilesLight(void);

void EntryScrStatusLEDPatterns(void);
void ExitScrStatusLEDPatterns(void);
#endif

void EntryScrProfilesToneSetup(void);
U8 mmi_profiles_get_melody_list( U16 CurMelodyID, U16 *selecteditem );
void ExitScrProfilesToneSetup(void);
#ifdef __MMI_DUAL_PROFILE_SUPPORT__  //Add by Renyh for 221 project   20090213
void mmi_profiles_entry_card2_ring_tone_list(void);
void mmi_profiles_exit_card2_ring_tone_list(void);
void mmi_profiles_set_card2_ring_tone_id_callback(U8 ExtMelodyFlag, U16 ringTone);
void mmi_profiles_entry_card2_ring_tone_list_hdlr(void);
void mmi_profiles_entry_card2_message_tone(void);
void mmi_profiles_exit_card2_message_tone(void);
void mmi_profiles_entry_card2_message_tone_hdlr(void);
#endif                                                        //Add by Renyh for 221 project   20090213
void mmi_profiles_entry_ring_tone_list(void);
void mmi_profiles_exit_ring_tone_list(void);

void EntryScrProfilesPoweronToneSetup(void);
void ExitScrProfilesPoweronToneSetup(void);

void EntryScrProfilesPoweroffToneSetup(void);
void ExitScrProfilesPoweroffToneSetup(void);

//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
void HighlightProfilesCoverOpenToneSetup(void);
void EntryScrProfilesCoverOpenToneSetup(void);
void ExitScrProfilesCoverOpenToneSetup(void);

void HighlightProfilesCoverCloseToneSetup(void);
void EntryScrProfilesCoverCloseToneSetup(void);
void ExitScrProfilesCoverCloseToneSetup(void);
#endif
//CSD end

void EntryScrProfilesMessageToneSetup(void);
void ExitScrProfilesMessageToneSetup(void);

void EntryScrProfilesKeypadToneSetup(void);
void ExitScrProfilesKeypadToneSetup(void);

void EntryScrProfilesAlarmToneSetup(void);
void ExitScrProfilesAlarmToneSetup(void);

void EntryScrSetRingToneId(void);
#ifdef __MMI_DUAL_PROFILE_SUPPORT__  //Add by Renyh for 221 project   20090213
void mmi_profiles_hint_hdlr_card2_incoming_call(U16 index);
#endif                                                         //Add by Renyh for 221 project   20090213
void EntryScrSetRingToneIdCountId(void (*Func_Callback)(U8, U16));
void EntryScrSetRingToneId_Ext(U8 ExtMelodyFlag, U16 ringTone);
void PrfSetRingToneExtMelody(void);
void ExitPrfSetRingToneExtMelody(void);
void EntryPrfSetRingToneExtMelody( void );


void EntryScrSetPoweronTone(void);
void EntryScrSetPoweroffTone(void);
//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
void EntryScrSetCoverOpenTone(void);
void EntryScrSetCoverCloseTone(void);
#endif
//CSD end

void EntryScrSetMessageTone(void);
#ifdef __MMI_DUAL_PROFILE_SUPPORT__  //Add by Renyh for 221 project   20090213
void mmi_profiles_hint_hdlr_card2_message_tone(U16 index);
#endif                                                         //Add by Renyh for 221 project   20090213
void EntryScrSetAnsweringMode(void);
#ifdef __MMI_STATUS_LED__
void EntryScrSetLightColor(void);
#endif
void EntryScrSetCalltimereminder(void);
void EntryScrSetKeypadTone(void);
void EntryScrSetwarningtone(void);
void EntryScrSetAlarmtone(void);
void EntryScrSetCommonVolumeLevel(void);
void EntryScrSetTime(void);
void EntryScrProfilesExtraToneSetup(void);
void ExitScrProfilesExtraToneSetup(void);

void GetCurrScreenHandler(S32);
//void GetCurrScreenHandlertwo(S32);
void mmi_profiles_melody_list_highlight_hdlr( S32 index );
void GetCurrScreenHandlerfour(S32);
void GetCurrScreenHandlerfive(S32);
void GetCurrScreenHandlersix(S32 item_index);

void EntryScrProfilesQueryPopup(void (*fn_lsk)(void),void (*fn_rsk)(void));
void ExitScrProfilesQueryPopup(void);
void ProfilesCallAlertPopup(void);
void ProfilesRingTypePopup(void);
void ProfilesFontSizePopup(void);
void ProfilesRingToneSelectPopup(void);
void ProfilesAnsweringModePopup(void);
void ProfilesExtraTonePopup(void);
void EntryPorfPopup(void);

#if defined( __MMI_INTELLIGENT_CALL_ALERT__ )
void EntryScrIntelligentCallAlert( void );
#endif


S8* Ringtonestring(void);
S8* Alarmtonestring(void);
S8* PowerOntonestring(void);
S8* CoverOpentonestring(void);
S8* PowerOfftonestring(void);
S8* CoverClosetonestring(void);
S8* MessageToneString(void);
S8* KeypadToneString(void);
S8* AlertTypeString(void);
S8* RingTypeString(void);
S8* FontSizeString(void);
S8* AnsweringModeString(void);

void StopRingToneBack(void);
void PlayToneAudio(void);
void StopToneAudio(void);
void VolumeIncreaseDecreaseFunction(void);
void GoBackCommonVolume(void);
void showAlertStatusIcons(void);
void GoIdleScreenCommonVolume(void);

void playcolorID(void);
void StopColorId(void);
void GetDynamicToneIds(void);
void StopSilentProfileVibrator(void);

void SetExtraToneValues(void);
U8 GuardSilentMode( void );
void SetMtCallAlertRing(void);
void SetMtCallAlertVibrationOnly(void);
void SetMtCallAlertVibrationAndRing(void);
void SetMtCallAlertVibrationThenRing(void);
void SetMtCallAlertSilent(void);
void SetRingTypeAscending(void);
void SetRingTypeSingle(void);
void SetRingTypeRepeat(void);
#ifdef __MMI_CONNECT_NOTICE__
void SetConnectNoticeNone(void);
void SetConnectNoticeToneOnly(void);
void SetConnectNoticeVibOnly(void);
void SetConnectNoticeToneAndVib(void);
#endif
//CSD Brian added for wrapping for Changable font size, 2003/11/11
#ifdef __MMI_CHANGABLE_FONT__
void SetFontSizeDefault(void);
void SetFontSizeSmall(void);
void SetFontSizeMedium(void);
#ifndef __MMI_DIALLING_FONT__
void SetFontSizeLarge(void);
#endif
#endif
void SetKeyPadToneSilent(void);
void SetKeyPadToneClick(void);
void SetKeyPadToneTone(void);

#if defined( __MMI_HUMAN_VOICE_KEYPAD_TONE__ )
void SetKeyPadToneHumanVoice1(void);
void SetKeyPadToneHumanVoice2(void);
void SetKeyPadToneHumanVoice3(void);
void SetKeyPadToneHumanVoice4(void);
void SetKeyPadToneHumanVoice5(void);
void SetKeyPadToneHumanVoice6(void);
#endif

#if defined( __MMI_CUST_KEYPAD_TONE__ )
void SetKeyPadToneCust1(void);
void SetKeyPadToneCust2(void);
void SetKeyPadToneCust3(void);
void SetKeyPadToneCust4(void);
void SetKeyPadToneCust5(void);
void SetKeyPadToneCust6(void);
#endif

void ContinueRingTone(void*);


void HintHandlerIncomingCall(U16 index);
void HintHandlerAlarmTone(U16 index);
void HintHandlerPowerOnTone(U16 index);
void HintHandlerPowerOffTone(U16 index);
//CSD brian added to wrap cover open/close tone setup, 2003/10/18
#ifdef __MMI_CLAMSHELL__
void HintHandlerCoverOpenTone(U16 index);
void HintHandlerCoverCloseTone(U16 index);
#endif
//CSD end
void HintHandlerMessgaeTone(U16 index);
void HintHandlerKeypadTone(U16 index);
void HintHandlerAlertType(U16 index);
void HintHandlerRingType(U16 index);
#ifdef __MMI_CONNECT_NOTICE__
void HintHandlerConnectNotice(U16 index);
#endif
//CSD Brian added for wrapping for Changable font size, 2003/11/11
#ifdef __MMI_CHANGABLE_FONT__
void HintHandlerFontSize(U16 index);
#endif
void HintHandlerRingVolume(U16 index);
void HintHandlerKeyVolume(U16 index);
#if defined(__CALL_VOLUME_LEVEL__)
//wug add used to pc300 start 20071010
void HintHandlerCallVolume(U16 index);
//wug add used to pc300 end 20071010
#endif
void HintHandlerLCDBacklight(U16 index);
#ifdef __MMI_STATUS_LED__
void HintHandlerStatusLED(U16 index);
#endif

void HighlightKeyPadToneSilent(void);
void HighlightKeyPadToneClick(void);
void HighlightKeyPadToneTone(void);
#if defined( __MMI_HUMAN_VOICE_KEYPAD_TONE__ )
void HighlightKeyPadToneHumanVoice1(void);
void HighlightKeyPadToneHumanVoice2(void);
void HighlightKeyPadToneHumanVoice3(void);
void HighlightKeyPadToneHumanVoice4(void);
void HighlightKeyPadToneHumanVoice5(void);
void HighlightKeyPadToneHumanVoice6(void);
#endif
#if defined( __MMI_CUST_KEYPAD_TONE__ )
void HighlightKeyPadToneCust1(void);
void HighlightKeyPadToneCust2(void);
void HighlightKeyPadToneCust3(void);
void HighlightKeyPadToneCust4(void);
void HighlightKeyPadToneCust5(void);
void HighlightKeyPadToneCust6(void);
#endif
void HighlightRingToneVolume(void);
void HighlightKeyToneVolume(void);
void HighlightRingTypeSingle(void);
void HighlightRingTypeRepeat(void);
void HighlightRingTypeAscending(void);

#ifdef __MMI_CONNECT_NOTICE__
void HighlightProfilesConnectNotice(void);
void HighlightConnectNoticeNone(void);
void HighlightConnectNoticeToneOnly(void);
void HighlightConnectNoticeVibOnly(void);
void HighlightConnectNoticeToneAndVib(void);
#endif
//CSD Brian added for wrapping for Changable font size, 2003/11/11
#ifdef __MMI_CHANGABLE_FONT__
void HighlightFontSizeDefault(void);
void HighlightFontSizeSmall(void);
void HighlightFontSizeMedium(void);
//CSD Brian added for wrapping for DIALING FONT, 2003/10/31
#ifndef __MMI_DIALLING_FONT__
void HighlightFontSizeLarge(void);
#endif
#endif
void HighlightCallAlertRing(void);
void HighlightCalAlertVibrationOnly(void);
void HighlightCallAlertVibrationAndRing(void);
void HighlightCallAlertVibrationThenRing(void);
void HighlightCallAlertSilent(void);
void HighlightProfilesOptions(void);
void HighlightProfileActivateOptions(void);
void HighlightProfilesCustomize(void);
void HighlightProfilesVolumeLevel(void);
void HighlightProfilesCallAlert(void);
void HighlightProfilesRingType(void);
void HighlightProfilesAnsweringMode(void);
#ifdef __MMI_BT_PROFILE__
extern void mmi_profiles_hilite_hdlr_bt_mode(void);
#endif 

//CSD Brian added for wrapping for Changable font size, 2003/11/11
#ifdef __MMI_CHANGABLE_FONT__
void HighlightProfilesFontSize(void);
#endif
void HighlightProfilesLoudSpeaker(void);
void HighlightProfilesLcdoff(void);
#ifdef __MMI_STATUS_LED__
void HighlightProfilesLight(void);
void HighlightProfilesStatus(void);
#endif
void HighlightProfilesSetTimer(void);
void HighlightProfilesToneSetup(void);
void HighlightProfilesIncomingCallToneSetup(void);   //Modified by Jinzh:20070617
#ifdef __MMI_DUAL_PROFILE_SUPPORT__  //Add by Renyh for 221 project   20090213
void mmi_profiles_hilite_hdlr_card2_ring_tone(void);
#endif                                                        //Add by Renyh for 221 project   20090213
void HighlightProfilesPoweronToneSetup(void);
void HighlightProfilesPoweroffToneSetup(void);
void HighlightProfilesMessageToneSetup(void);
#ifdef __MMI_DUAL_PROFILE_SUPPORT__  //Add by Renyh for 221 project   20090213
void mmi_profiles_hilite_hdlr_card2_message_tone(void);
#endif                                                        //Add by Renyh for 221 project   20090213
void HighlightProfilesKeypadToneSetup(void);
void HighlightAlarmToneSetup(void);
void HighlightExtraToneSetup(void);
void HighlightHeadsetProfile(void);
void HighlightIndoorProfile(void);
void HighlightOutdoorProfile(void);
void HighlightMeetingProfile(void);
void HighlightGeneralProfile(void);

#if defined( __MMI_INTELLIGENT_CALL_ALERT__ )
void HighlightHandlerIntelligentCallAlert( void );
void HintIntelligentCallAlert( U16 index );
void HighlightHandlerIntelligentCallAlertOn( void );
void HighlightHandlerIntelligentCallAlertOff( void );
void SetIntelligentCallAlertOn(void);
void SetIntelligentCallAlertOff(void);
#endif

//void DisplayProfilePopup(U8 * string, U16 imageId, U8 imageOnBottom, U32 popupDuration, U8 toneId);
//void ExitDisplayProfilePopup(void);
//void PopupProfileTimerOver(void);

//void DisplayProfilePopupCheckBox(U8 * string, U16 imageId, U8 imageOnBottom, U32 popupDuration, U8 toneId);
//void ExitDisplayProfilePopupCheckBox(void);
//void PopupProfileTimerOverCheckBox(void);


#endif


