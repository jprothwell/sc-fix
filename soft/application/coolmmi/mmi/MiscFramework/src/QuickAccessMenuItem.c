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
 * QuickAccessMenuItem.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is Contains functionality for Quick Access Menu Items
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
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: QuickAccessMenuItem.c

  	PURPOSE		: Contains functionality for Quick Access Menu Items

 
				   options are put are under construction 	

 

	DATE		: Feb 28, 2003

**************************************************************/

#include "mmi_features.h"
#include "stdc.h"
#include "globalconstants.h"
#include "debuginitdef.h"
#include "mmi_data_types.h"
#include "quickaccessmenuitem_def.h"
#include "custdatares.h"
#include "frameworkstruct.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "wgui_categories.h"
#include "wgui_categories_cm.h"
#include "wgui_categories_popup.h"
#include "custmenures.h"
#include "miscfunctions.h"
#include "wgui_status_icons.h"
#include "unicodexdcl.h"
#include "debuginitdef.h"
#include "callmanagementgprot.h"
#include "quickaccessmenuitemprot.h"
#include "timerevents.h"
//#include "quickaccessmenuitemsgdcl.h"
#include "globaldefs.h"
#include "queuegprot.h"
#include "simdetectiongexdcl.h"
#include "idleappdef.h"
#include "keybrd.h"
#include "profilegprots.h"
#ifdef __MMI_TOUCH_SCREEN__
#include "statusiconres.h"
#endif
#ifdef __MOD_SMSAL__
#include "messagesexdcl.h"
#endif

#include "phonebookgprot.h"
#include "simdetectiongprot.h"
//#include "nvramexdcl.h"
#ifdef MMI_ON_HARDWARE_P
#include "l4dr.h"
#include "custom_util.h"
#endif
#include "gpioinc.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim_call.h"
#endif
#include "simdetectiongexdcl.h"
#if defined(__MMI_MAINLCD_220X176__)
extern S32 is_on_idlescreen(void);
#endif
U8 isEmergencyCallDropped=0;  
U16   gKeyPadLockFlag = 0;
// U16   gEnteredBufferNull = 0;
/*not treating as an array of characters these
two are array of integers so length need not be 
multiplied by ENCODING LENGTH */
U16 aligned_emgNumberArr[(EMERGENCY_NUMBER_LENGTH +1)*ENCODING_LENGTH/2];

S8 *emgNumberArr = (S8*) &aligned_emgNumberArr;

/*To pass no to call management it is required to pass string */
//S16 curDigit;
U8  gEmerencyNoDialed = 0;

/**********************************************************

	FUNCTION NAME:			HandlePoundKeyForUnlock

	PURPOSE:				This fn handle the press of 
							Pound key , after RSK has been pressed
	
	INPUT PARAMETERS:		nil

	OUTPUT PARAMETERS:		nil

	RETURNS:				void

 

**********************************************************/

void HandlePoundKeyForUnlock(void)
{
	StopTimer(KEYPADUNLOCKEDMESSAGE_TIMER);
	gKeyPadLockFlag = 0;

//Lisen 02182005
#ifdef __MMI_KEYPADLOCK_WITH_KEYTONE__
      
#else 
      mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);
#endif
	
	playRequestedTone(SUCCESS_TONE);     //dyj del 20060515 //add by panxu 20061228
#if defined(__MMI_MAINLCD_220X176__)&& defined(__MMI_TOUCH_SCREEN__)
	mmi_pen_unblock();
	EntryIdleScreen();
#else
	EntryScrKeyPadUnlockMessage();
#endif
#ifdef __MMI_TOUCH_SCREEN__
	ChangeStatusIconImage(STATUS_ICON_KEYPAD_LOCK, IMG_SI_KEYPAD_UNLOCK);
	UpdateStatusIcons(); 
#else
	IdleResetStatusIcon(STATUS_ICON_KEYPAD_LOCK);	
#endif
	TurnOnBacklight(1);
}

/**********************************************************

	FUNCTION NAME:			EntryScrKeyPadUnlockMessage

	PURPOSE:				Entry fn to show that Keypad has been unlocked.
	
	INPUT PARAMETERS:		nil

	OUTPUT PARAMETERS:		nil

	RETURNS:				void

 

**********************************************************/

void EntryScrKeyPadUnlockMessage(void)
{
	EntryNewScreen(KPAD_SCR_UNLOCK_MESSAGE, ExitScrKeyPadUnlockMessage, NULL, NULL);

	ShowCategory64Screen(KPAD_SCR_UNLOCKED_MESSAGE_RUNNING_STR,IMG_KPAD_SCR_UNLOCKED_HELP_MESSAGE,0);
	/* Start Timer */
	StartTimer(KEYPADUNLOCKEDMESSAGE_TIMER, KEYPAD_UNLOCK_SCR_TIMEOUT, GoBackHistory);

	SetGroupKeyHandler(DisplayIdleScreen,(PU16)PresentAllKeys,(U8)TOTAL_KEYS,(U16)KEY_EVENT_DOWN);
	
	/*Set the Exit handler , called in case of async events ,when transient screen on display*/
}


/**********************************************************

	FUNCTION NAME:			ExitScrKeyPadUnlockMessage

	PURPOSE:				Exit function Key Pad Unlocked Screen
	
	INPUT PARAMETERS:		nil

	OUTPUT PARAMETERS:		nil

	RETURNS:				void

 

**********************************************************/
void ExitScrKeyPadUnlockMessage(void)
{
	StopTimer(KEYPADUNLOCKEDMESSAGE_TIMER);
	stopRequestedTone(SUCCESS_TONE);   //dyj del 20060514 //add by panxu 20061228
}

 
#ifdef __MMI_TOUCH_SCREEN__
void ScrAfterPressingRSKTimeout(void)
{
	mmi_pen_block();
	GoBackHistory();
}
#endif
 
#if defined(__MMI_MAINLCD_220X176__) && defined(__MMI_TOUCH_SCREEN__)
void e71_idle_info_timer_over(void)
{
	StopTimer(KEYPADUNLOCKEDMESSAGE_TIMER);
	mmi_pen_unblock();
	EntryIdleScreen();
}

void mmi_e71_idle_lsk_down_info(void)
{
	mmi_pen_block();
	gui_push_clip();
	gui_set_clip(0,UI_device_height - MMI_button_bar_height,UI_device_width,UI_device_height);
	if(is_on_idlescreen())
		gdi_draw_solid_rect(0,UI_device_height - MMI_button_bar_height,UI_device_width,UI_device_height,GDI_COLOR_TRANSPARENT);
	else
		gdi_image_draw_id(0,UI_device_height-MMI_button_bar_height,IMG_NOKIA_E_SERIES_BUTTON_BAR_BG);
	gui_move_text_cursor(2,UI_device_height - MMI_button_bar_height);
	gui_set_font(&MMI_medium_font);
	gui_print_bordered_text((UI_string_type)GetString(STR_E71_UNLOCK_INFO));
	gui_pop_clip();
	gui_BLT_double_buffer(0,UI_device_height - MMI_button_bar_height,UI_device_width,UI_device_height);
	//SetGroupKeyHandler(PopupTimerFlagEnable,(PU16)PresentAllKeys,TOTAL_KEYS,KEY_EVENT_DOWN);
	//SetGroupKeyHandler(PopupCloseByPressAnyKey,(PU16)PresentAllKeys,TOTAL_KEYS,KEY_EVENT_UP);
	StartTimer(KEYPADUNLOCKEDMESSAGE_TIMER, KEYPADUNLOCK_TIMEOUT,e71_idle_info_timer_over);
	SetKeyHandler(HandlePoundKeyForUnlock,KEY_SEND,KEY_EVENT_DOWN);
}
#endif
/**************************************************************

	FUNCTION NAME		: EntryScrAfterPessingRSK

  	PURPOSE				: shows the screen after pressing RSK indicating 
						  messge " press # to unlock "	

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void EntryScrAfterPessingRSK(void)
{
	EntryNewScreen(KPAD_SCR_UNLOCK_HELP_MESSAGE, ExitScrAfterPessingRSK, NULL, NULL);

 
#ifdef __MMI_TOUCH_SCREEN__
	//ShowCategory264Screen(get_string(TOUCH_SCR_UNLOCKED_HELP_MESSAGE_RUNNING_STR), get_image(IMG_TOUCH_SCR_PEN_CLICK_AREA), NULL, NULL);
	ShowCategory64Screen(KPAD_SCR_UNLOCKED_HELP_MESSAGE_RUNNING_STR,IMG_KPAD_SCR_UNLOCKED_HELP_MESSAGE,0);
	/* Start Timer */
	StartTimer(KEYPADUNLOCKEDHELPMESSAGE_TIMER, KEYPADUNLOCK_TIMEOUT,ScrAfterPressingRSKTimeout/*GoBackHistory*/);
#else
#ifdef __MMI_KEYPAD_LOCK_PATTERN_1__
	ShowCategory64Screen(KPAD_SCR_UNLOCKED_HELP_MESSAGE_RUNNING_STR,IMG_KPAD_SCR_UNLOCKED_HELP_MESSAGE,0);
#else
	ShowCategory64Screen(KPAD_SCR_UNLOCKED_HELP_MESSAGE_RUNNING_STR2,IMG_KPAD_SCR_UNLOCKED_HELP_MESSAGE,0);
#endif
	/* Start Timer */
	StartTimer(KEYPADUNLOCKEDHELPMESSAGE_TIMER, KEYPADUNLOCK_TIMEOUT,GoBackHistory);
#endif
 

	SetGroupKeyHandler(GoBackHistory,(PU16)PresentAllKeys,(U8)TOTAL_KEYS,(U16)KEY_EVENT_DOWN);

#ifdef __MMI_TOUCH_SCREEN__
	//SetKeyHandler(EntryScrAfterPessingRSK,KEY_SEND,KEY_EVENT_DOWN);
	mmi_pen_unblock();
	SetKeyHandler(HandlePoundKeyForUnlock,KEY_POUND,KEY_EVENT_DOWN);
#else
#ifdef __MMI_KEYPAD_LOCK_PATTERN_1__    
	//SetKeyHandler(EntryScrAfterPessingRSK,KEY_RSK,KEY_EVENT_DOWN);
	SetKeyHandler(HandlePoundKeyForUnlock,KEY_POUND,KEY_EVENT_DOWN);
#ifdef __MMI_SLIDE__  
	SetKeyHandler(HandlePoundKeyForUnlock,KEY_LSK,KEY_EVENT_DOWN);
#endif
#else
	//SetKeyHandler(EntryScrAfterPessingRSK,KEY_LSK,KEY_EVENT_DOWN);
	SetKeyHandler(HandlePoundKeyForUnlock,KEY_STAR,KEY_EVENT_DOWN);
#ifdef __MMI_SLIDE__  
	SetKeyHandler(HandlePoundKeyForUnlock,KEY_RSK,KEY_EVENT_DOWN);
#endif
#endif
#endif
	playRequestedTone(SUCCESS_TONE);   //dyj del 20060515 //add by panxu 20061228

	TurnOnBacklight(1);

}

/**********************************************************

	FUNCTION NAME:			ExitScrAfterPessingRSK

	PURPOSE:				Exit function for the scrren after
							that give message messge " press # to unlock "	
	
	INPUT PARAMETERS:		nil

	OUTPUT PARAMETERS:		nil

	RETURNS:				void

 

**********************************************************/


void ExitScrAfterPessingRSK(void)
{
	StopTimer(KEYPADUNLOCKEDHELPMESSAGE_TIMER);
	stopRequestedTone(SUCCESS_TONE);  //dyj del 20060514 ////add by panxu 20061228
#ifdef __MMI_TOUCH_SCREEN__
	if ( gKeyPadLockFlag == 1 )
		mmi_pen_block();
#endif

}

#if defined(__MMI_KEYPAD_LOCK_PROMPT_TEXT__)				
/**************************************************************

	FUNCTION NAME		: EntryScrKeyPadLockOnPessingAnyKey

  	PURPOSE				: shows the screen after pressing any key indicating 
						  messge " press RSK to unlock "	

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryScrKeyPadLockOnPessingAnyKey(void)
{
	EntryNewScreen(KPAD_SCR_UNLOCK_FIRST_HELP_MESSAGE, ExitScrKeyPadLockOnPessingAnyKey, NULL, NULL);

#ifdef __MMI_TOUCH_SCREEN__
	ShowCategory64Screen(TOUCH_SCR_UNLOCKED_FIRST_HELP_MESSAGE_RUNNING_STR,IMG_KPAD_SCR_UNLOCKED_HELP_MESSAGE,0);
#else
#ifdef __MMI_KEYPAD_LOCK_PATTERN_1__
	ShowCategory64Screen(KPAD_SCR_UNLOCKED_FIRST_HELP_MESSAGE_RUNNING_STR,IMG_KPAD_SCR_UNLOCKED_HELP_MESSAGE,0);
#else
	ShowCategory64Screen(KPAD_SCR_UNLOCKED_FIRST_HELP_MESSAGE_RUNNING_STR2,IMG_KPAD_SCR_UNLOCKED_HELP_MESSAGE,0);
#endif
#endif

	/* Start Timer */
	StartTimer(KEYPADUNLOCKEDHELPMESSAGE_TIMER, KEYPADUNLOCK_TIMEOUT,GoBackHistory);

	// SetGroupKeyHandler(GoBackHistory,(PU16)PresentAllKeys,(U8)TOTAL_KEYS,(U16)KEY_EVENT_DOWN);

       SetGroupKeyHandler(MMI_dummy_function,(PU16)PresentAllKeys,(U8)TOTAL_KEYS,(U16)KEY_EVENT_DOWN);
	
#ifdef __MMI_TOUCH_SCREEN__
	SetKeyHandler(EntryScrAfterPessingRSK,KEY_SEND,KEY_EVENT_DOWN);
#else
#ifdef __MMI_KEYPAD_LOCK_PATTERN_1__   
	SetKeyHandler(EntryScrAfterPessingRSK,KEY_RSK,KEY_EVENT_DOWN);
#else
	SetKeyHandler(EntryScrAfterPessingRSK,KEY_LSK,KEY_EVENT_DOWN);
#endif
#endif

	playRequestedTone(SUCCESS_TONE);   //dyj del 20060515 //add by panxu 20061228

	TurnOnBacklight(1);

}


/**********************************************************

	FUNCTION NAME:			ExitScrKeyPadLockOnPessingAnyKey

	PURPOSE:				Exit function for the scrren after
							that give message messge " press # to unlock "	
	
	INPUT PARAMETERS:		nil

	OUTPUT PARAMETERS:		nil

	RETURNS:				void

 

**********************************************************/


void ExitScrKeyPadLockOnPessingAnyKey(void)
{
	StopTimer(KEYPADUNLOCKEDHELPMESSAGE_TIMER);
	stopRequestedTone(SUCCESS_TONE);  //dyj del 20060514 //add by panxu 20061228
}

#endif

#if (MMI_SIM_NUMBER > 2)
BOOL bChannelSet = FALSE;
void MakeEmergencyCall_sim1()
{
	MakeEmergencyCall();
}
void MakeEmergencyCall_sim2()
{
	MakeEmergencyCall();
}
void MakeEmergencyCall_sim3()
{
	MakeEmergencyCall();
}
#else


#if(defined __SENDKEY2_SUPPORT__ &&(MMI_SIM_NUMBER == 2))

BOOL bChannelSet = FALSE;
void MakeEmergencyCall_sim1()
{
	E_MTPNP_AD_SIMCARD_USABLE_SIDE side = MTPNP_AD_Get_UsableSide();
	switch (side)
	{
		case MTPNP_AD_DUALSIM_USABLE:
		case MTPNP_AD_SIMCARD1_USABLE:
		case MTPNP_AD_SIMCARD1_NOSIM_USABLE:
		{
			MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_MASTER);
			bChannelSet = TRUE;
			MakeEmergencyCall();
			break;
		}
		case MTPNP_AD_SIMCARD_NOSIM_USABLE:
		{
			bChannelSet = FALSE;
			MakeEmergencyCall();
			break;
		}	
		case MTPNP_AD_SIMCARD2_NOSIM_USABLE:
		case MTPNP_AD_SIMCARD2_USABLE:
			MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_SLAVE);
			bChannelSet = TRUE;
			MakeEmergencyCall();
			break;
		default:
			break;
	}
}

void MakeEmergencyCall_sim2()
{
	E_MTPNP_AD_SIMCARD_USABLE_SIDE side = MTPNP_AD_Get_UsableSide();
	switch (side)
	{
		case MTPNP_AD_DUALSIM_USABLE:
		case MTPNP_AD_SIMCARD2_USABLE:
		case MTPNP_AD_SIMCARD2_NOSIM_USABLE:
		{
			MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_SLAVE);
			bChannelSet = TRUE;
			MakeEmergencyCall();
			break;
		}
		case MTPNP_AD_SIMCARD_NOSIM_USABLE:
		{
			bChannelSet = FALSE;
			MakeEmergencyCall();
			break;
		}	
		case MTPNP_AD_SIMCARD1_NOSIM_USABLE:
		case MTPNP_AD_SIMCARD1_USABLE:
			MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_MASTER);
			bChannelSet = TRUE;
			MakeEmergencyCall();
			break;
		default:
			break;
	}
}

#endif
#endif
/*****************************************************************************
 * FUNCTION
 *  MakeEmergencyCall
 * DESCRIPTION
 *  Make the emergency call in case of
 *  Keypad lock
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void MakeEmergencyCall(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    StopTimer(EMERGENCY_NO_DIALED_TRANSIANT_SCR_TIMER);
#if defined(__MMI_WLAN_FEATURES__) && defined(__MMI_VOIP__)
    if (mmi_netset_get_active_preferred_mode() == P_WLAN_ONLY || mmi_netset_get_active_preferred_mode() == P_WLAN_PREFERRED)
    {
        mmi_voip_app_make_call((U8*) emgNumberArr);
    }
    else
#endif /* defined(__MMI_WLAN_FEATURES__) && defined(__MMI_VOIP__) */
    {
    #ifdef __MMI_MULTI_SIM__
	#ifdef __SENDKEY2_SUPPORT__
		if (!bChannelSet)
	#endif
		{
        		mmi_sim_error_set_emergency_call_path();
		}
	#ifdef __SENDKEY2_SUPPORT__
		bChannelSet = FALSE;
	#endif
    #endif
        MakeCall(emgNumberArr);
    }
    gKeyPadLockFlag = 1;

    /* Lisen 02182005 */
#ifdef __MMI_KEYPADLOCK_WITH_KEYTONE__

#else 
    mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);
#endif 

    memset(emgNumberArr, 0, (EMERGENCY_NUMBER_LENGTH + 1) * ENCODING_LENGTH);
}

/**********************************************************

	FUNCTION NAME:			DropEmergencyCall

	PURPOSE:				Drop Emergency call 
							and back to idle screen

	INPUT PARAMETERS:		nil

	OUTPUT PARAMETERS:		nil

	RETURNS:				void

 

**********************************************************/


void DropEmergencyCall(void)
{
  isEmergencyCallDropped=1;  
  StopTimer(EMERGENCY_NO_DIALED_TRANSIANT_SCR_TIMER);
	gKeyPadLockFlag=1;
      
	 
#ifdef __MMI_TOUCH_SCREEN__
	ChangeStatusIconImage(STATUS_ICON_KEYPAD_LOCK, IMG_SI_KEYPAD_LOCK);
	UpdateStatusIcons();
#else 
	IdleSetStatusIcon(STATUS_ICON_KEYPAD_LOCK);	
#endif

//Lisen 02182005
#ifndef __MMI_KEYPADLOCK_WITH_KEYTONE__
  mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);
#endif
  
  memset(emgNumberArr,0,(EMERGENCY_NUMBER_LENGTH+1) * ENCODING_LENGTH);
  DisplayIdleScreen();	
}


/**********************************************************

	FUNCTION NAME:			EmergencyCallCheck

	PURPOSE:				Check the number entered on idel scrreen 
							if it is Emergency Number

	INPUT PARAMETERS:		S16 KeyCode, S16 KeyType

	OUTPUT PARAMETERS:		nil

	RETURNS:				void

 

**********************************************************/
static U8 EmergengyNoArrIndex=0;  
void EmergencyCallCheck(S16 keyCode, S16 keyType)
{
	mmi_trace(g_sw_CC,"MMI_Call: Func: %s ", __FUNCTION__);
	if(keyType==KEY_EVENT_DOWN)
	{
		char	aTemp[5];
		 
		U8 AnsValidateEmgNo;
		U8 i;

		if((keyCode>=0)&&(keyCode<=9))
		{
			sprintf(aTemp,"%d",keyCode);
			emgNumberArr[EmergengyNoArrIndex] = *aTemp;
  
		  #ifdef __UCS2_ENCODING
				emgNumberArr[++EmergengyNoArrIndex] = 0;
		  #endif

			emgNumberArr[++EmergengyNoArrIndex] = 0;
			for(i=0;(EmergengyNoArrIndex>0)||emgNumberArr!=NULL;i++)
			{	
				AnsValidateEmgNo=ValidateEmergencyNo(emgNumberArr);	
				mmi_trace(g_sw_CC,"MMI_Call: Func: %s AnsValidateEmgNo=%d", __FUNCTION__, AnsValidateEmgNo);
				switch(AnsValidateEmgNo)
				{
				case 0://Number not Matched, so shift number by one digit and try again.
					memcpy(emgNumberArr,emgNumberArr+ENCODING_LENGTH,(EMERGENCY_NUMBER_LENGTH*ENCODING_LENGTH));
					 
					if( EmergengyNoArrIndex <=1 || EmergengyNoArrIndex > (EMERGENCY_NUMBER_LENGTH*ENCODING_LENGTH) )
					   {
         					memset(emgNumberArr,0,(EMERGENCY_NUMBER_LENGTH+1)*ENCODING_LENGTH);
         					EmergengyNoArrIndex=0;
         					break;
					   }
					EmergengyNoArrIndex=EmergengyNoArrIndex-ENCODING_LENGTH;
					continue;
				case 1:// Number Partially Matched.
					break;
				case 2://Number found as Emergency Number.
					isEmergencyCallDropped=0;  
					EntryNewScreen(EMERENCY_CALL_SCR_DISPLAY_ID, ExitEmergencyCallScr, NULL, NULL);
					EmergengyNoArrIndex=0;
					gEmerencyNoDialed=1;

					 
#ifdef __MMI_TOUCH_SCREEN__
					ChangeStatusIconImage(STATUS_ICON_KEYPAD_LOCK, IMG_SI_KEYPAD_UNLOCK);
#else
					HideStatusIcon(STATUS_ICON_KEYPAD_LOCK);	
#endif

#if defined  (__MMI_TOUCH_DIAL_SCREEN_WITH_FUNCTION__)
#if defined(__DUALMODE_TOUCH_DIAL_SCREEN_WITH_SLAVE_FUNCTION__)&& defined(__SENDKEY2_SUPPORT__)				    
					mmi_idle_set_dual_sim_dialing_keypad_call_handler(MakeEmergencyCall_sim1, MakeEmergencyCall_sim2,NULL,NULL);
#else
					SetDialingKeypadCallHandler(MakeEmergencyCall);
#endif
#endif					
					ShowCategory16Screen(0,0,
										 STR_GLOBAL_BACK ,IMG_GLOBAL_BACK,
										 (U8*)emgNumberArr, 
										 4,
										NULL);
					gKeyPadLockFlag=0;
#ifdef __MMI_WGUI_CSK_ENABLE__
					SetCenterSoftkeyFunction(DropEmergencyCall, KEY_EVENT_DOWN);
#endif
//Lisen 02182005
#ifdef __MMI_KEYPADLOCK_WITH_KEYTONE__
      
#else 
                                   mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);
#endif					
					SetGroupKeyHandler(DropEmergencyCall,(PU16)PresentAllKeys,TOTAL_KEYS,KEY_EVENT_UP); /* KEY_EVENT_DOWN, keytone will play non-stop */
					StartTimer(EMERGENCY_NO_DIALED_TRANSIANT_SCR_TIMER,
						EMERGENCY_NO_DIALED_TRANSIANT_SCR_TIMEOUT,DropEmergencyCall);
					ChangeRightSoftkey(STR_GLOBAL_BACK ,IMG_GLOBAL_BACK);

					SetRightSoftkeyFunction(DropEmergencyCall,KEY_EVENT_DOWN);

					SetKeyHandler(MakeEmergencyCall,KEY_SEND,KEY_EVENT_DOWN);
			#ifdef __SENDKEY2_SUPPORT__
					SetKeyHandler(MakeEmergencyCall_sim1,KEY_SEND1,KEY_EVENT_DOWN);
					SetKeyHandler(MakeEmergencyCall_sim2,KEY_SEND2,KEY_EVENT_DOWN);
					#ifdef __MMI_USE_LSK_AS_SEND2__
					SetKeyHandler(get_softkey_function(KEY_EVENT_UP, MMI_LEFT_SOFTKEY),KEY_ENTER,KEY_EVENT_UP);
					SetKeyHandler(MakeEmergencyCall_sim1, KEY_SEND, KEY_EVENT_DOWN);
					ClearKeyHandler(KEY_LSK, KEY_EVENT_UP);
					SetKeyHandler(MakeEmergencyCall_sim2, KEY_LSK, KEY_EVENT_DOWN);
					#endif
#ifdef TGT_TD_4SIM
					SetKeyHandler(MakeEmergencyCall_sim3,KEY_CAMERA,KEY_EVENT_DOWN);
#endif
					
			#endif
					SetKeyHandler(DropEmergencyCall,KEY_END,KEY_EVENT_DOWN);
					TurnOnBacklight(1);
					break;
				default:
					memset(emgNumberArr,0,(EMERGENCY_NUMBER_LENGTH+1)*ENCODING_LENGTH);
					EmergengyNoArrIndex=0;
					break;
				}
				break;
			}

		}
		else
		{
			memset(emgNumberArr,0,(EMERGENCY_NUMBER_LENGTH+1)*ENCODING_LENGTH);
			EmergengyNoArrIndex=0;
		}
	}		
}


void ExitEmergencyCallScr(void)
{
	if( isEmergencyCallDropped == 0 )  
	{
	   StopTimer(EMERGENCY_NO_DIALED_TRANSIANT_SCR_TIMER);
		 
		//gKeyPadLockFlag=1;
//Lisen 02182005
//#ifndef __MMI_KEYPADLOCK_WITH_KEYTONE__
//		mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);
//#endif
       
       memset(emgNumberArr,0,(EMERGENCY_NUMBER_LENGTH+1) * ENCODING_LENGTH);
	}

	gKeyPadLockFlag=1;
}


/*****************************************************************************
 * FUNCTION
 *  ValidateEmergencyNo
 * DESCRIPTION
 *  Validate the Emergency Number.
 * PARAMETERS
 *  tempbuffer      [?]     
 * RETURNS
 *  void
 *****************************************************************************/
U8 ValidateEmergencyNo(S8 *tempbuffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 nNumLen;
    S32 nLen;
    U8 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    nNumLen = pfnUnicodeStrlen(tempbuffer);

    for (i = 0; i < MAX_EMERGENCYNUMBER; i++)
    {
        nLen = pfnUnicodeStrlen((PS8) EmergencyNumber[i]);

        if (nLen == nNumLen)
        {
            if ((pfnUnicodeStrcmp((PS8) EmergencyNumber[i], tempbuffer) == 0))
            {
                return 2;
            }
        }
        else if (nLen > nNumLen)
        {
            if ((pfnUnicodeStrncmp((PS8) EmergencyNumber[i], tempbuffer, nNumLen) == 0))
            {
                return 1;
            }
        }
    }

    return mmi_phb_check_valid_sos_number(tempbuffer);
}

