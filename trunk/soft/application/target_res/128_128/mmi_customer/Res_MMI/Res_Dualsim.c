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

/**************************************************************

	FILENAME	: Res_.c

  	PURPOSE		: Populate Resource for 

 

 

	DATE		: August-11-2003

**************************************************************/
#include "cs_types.h"
#include "custdatares.h"
#include "stdc.h"
#include "customcfg.h"
#include "custresdef.h"
#include "mmi_features.h"

#ifdef FIRST_PASS
#include "buildcfg.h"
#endif

#ifdef __MMI_MULTI_SIM__
#include "phonebookdef.h"
#include "mtpnp_ad_resdef.h"
#include "messagesresourcedata.h"
#include "settingdefs.h"


#ifdef DEVELOPER_BUILD_FIRST_PASS
#include "populateres.h"

/* Populate the mercury string : note: the function can be auto generate by RefListMerger.exe*/
static void Dualsim_StrPopulate(void)
{
    /* engineer/factory mode begin */
    ADD_APPLICATION_STRING2(STRING_MTPNP_EM, "Dualmode Engineer Mode", "Dualmode Engineer Mode");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CHK_RSSI, "Check rssi", "Check rssi");
    ADD_APPLICATION_STRING2(STRING_MTPNP_PSN_CALLBACK, "PSN Callback", "PSN Callback");
    ADD_APPLICATION_STRING2(STRING_MTPNP_ASSERT_SLAVE, "Assert slave", "Assert slave");
    ADD_APPLICATION_STRING2(STRING_MTPNP_FM_COMM, "DG Communication", "Factory Mode DG Communication");
    ADD_APPLICATION_STRING2(STRING_MTPNP_FM_MIC, "Slave Mic", "Factory Mode Slave Mic");
    ADD_APPLICATION_STRING2(STRING_MTPNP_FM_SPEAKER, "Slave Speaker", "Factory Mode Slave Speaker");
    ADD_APPLICATION_STRING2(STRING_MTPNP_FM_RECEIVER, "Slave Receiver", "Factory Mode Slave Receiver");
    ADD_APPLICATION_STRING2(STRING_MTPNP_FM_HEADSET, "Slave Headset", "Factory Mode Slave Headset");
    /* engineer/factory mode end */

//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_EM_OP_SIDE, "Set Side", "Select Master or Slave");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_MASTER_TEST, "Master", "Master");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SLAVE_TEST, "Slave", "Slave");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SWITCH_SAME_MODE, "Switch to the same mode", "Switch to the same mode");
    ADD_APPLICATION_STRING2(STRING_MTPNP_HELPINFO, "MTPNP Help Information", "MTPNP Help Information");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_REGISTER, "Register", "Register");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_REGISTER_TIP,
//del cont.                            "Register Tips: Please wait the PSN tool report succeed, plug out the cable and restart phone",
//del cont.                            "Register Tips");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_PSNCALLBACK_TIP,
//del cont.                            "PSN Callback Tips: Please wait the PSN tool report succeed, plug out the cable and restart phone",
//del cont.                            "PSN Callback Tips");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SETTING, "Dual SIM setting", "Dual SIM setting");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_REGISTER_CONFIRM, "Register mercury", "Register mercury");
    ADD_APPLICATION_STRING2(STRING_MTPNP_DM_SETTING, "Dual SIM setting", "Dual SIM setting");
    ADD_APPLICATION_STRING2(STRING_MTPNP_DM_DUALCARD, "Dual SIM open", "Dual SIM open");
    ADD_APPLICATION_STRING2(STRING_MTPNP_DM_CARD1, "Only SIM1 open", "Only SIM1 open");
    ADD_APPLICATION_STRING2(STRING_MTPNP_DM_CARD2, "Only SIM2 open", "Only SIM2 open");
    ADD_APPLICATION_STRING2(STRING_MTPNP_DM_FLIGHT, "Flight mode", "Flight mode");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_DM_ERROR_MODE, "Error mode", "Error mode");
    ADD_APPLICATION_STRING2(STRING_MTPNP_DM_SWITCH, "Dual mode switch", "Dual mode switch");
    ADD_APPLICATION_STRING2(STRING_MTPNP_PHONE_SETTING, "Phone setup", "Phone setup");
    ADD_APPLICATION_STRING2(STRING_MTPNP_PHB_EXTRA, "Phonebook extra", "Phonebook extra");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_OWNER_NUMBER, "SIM1 owner number", "owner number");
    ADD_APPLICATION_STRING2(STRING_MTPNP_OWNER_NUMBER, "SIM2 owner number", "owner number");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_SDL_NUMBER, "SIM1 SDL number", "SDL number");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SDL_NUMBER, "SIM2 SDL number", "SDL number");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_SOS_NUMBER, "SIM1 SOS number", "SOS number");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SOS_NUMBER, "SIM2 SOS number", "SOS number");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD1_SHOW_OWNER_NUMBER, "Show Card1 Owner Number", "Show owner number");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SHOW_OWNER_NUMBER, "Show Card2 Owner Number", "Show owner number");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SEL_POWERMODE, "Select power mode", "Select power mode");
    ADD_APPLICATION_STRING2(STRING_MTPNP_DM_DISABLESWTH, "Disable switch mode", "Disable switch mode");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SWITCH_DC, "Switch to dual card", "Switch to dual card");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SWITCH_CARD1, "Switch to card1", "Switch to card1");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SWITCH_CARD2, "Switch to card2", "Switch to card2");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SWITCH_QUERY, "Switch to query", "Switch to query");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SWITCH_FLIGHT, "Switch to flight", "Switch to flight");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD1_CLOSED, "SIM1 Closed", "SIM1 Closed");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD2_CLOSED, "SIM2 Closed", "SIM2 Closed");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD1_DURUNGCLOSE, "SIM1 Closing", "SIM1 Closing");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD2_DURINGCLOSE, "SIM2 Closing", "SIM2 Closing");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SEARCHING_NW, "Searching", "Searching");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_NOSERVICE, "No service", "No service");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_LIMITSERVICE, "Emergency", "Limit service");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD1ABSENT, "Insert SIM1", "Insert SIM1");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD2ABSENT, "Insert SIM2", "Insert SIM2");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD2_NOTSUPPORT, "Card2 not support!", "Card2 not support!");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD1_CALL_SETTING, "Card1 Call setting", "Call setting");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CALL_SETTING, "Card2 Call setting", "Call setting");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CALL_ID, "Call ID", "Call ID");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CLI_SET_NETWORK, "Set by network", "Set by network");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CLI_HIDE_ID, "Hide ID", "Hide ID");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CLI_SEND_ID, "Send ID", "Send ID");
    ADD_APPLICATION_STRING2(STRING_MTPNP_LINE_SWITCH, "Line switch", "Line switch");
    ADD_APPLICATION_STRING2(STRING_MTPNP_LSW_LINE1, "Line 1", "Line 1");
    ADD_APPLICATION_STRING2(STRING_MTPNP_LSW_LINE2, "Line 2", "Line 2");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_LINE1_SET, "Line 1 Selected", "Line 1 Selected");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_LINE1_SET_ERR, "Unknown Error", "Unknown Error");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_LINE2_SET, "Line 2 Selected", "Line 2 Selected");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_LINE2_SET_ERR, "Unknown Error", "Unknown Error");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SETTING_LINE_SEL, "Line Select Message", "Sel");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_LINE_SET_TITLE, "Line set", "Line set");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD1_SIM_BLOCK, "SIM1 Blocked", "SIM1 Blocked");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD2_SIM_BLOCK, "SIM2 Blocked", "SIM2 Blocked");
    ADD_APPLICATION_STRING2(STRING_MTPNP_ENTER_CARD1PIN, "Card1 PIN Verify", "SIM1 PIN Verify");
    ADD_APPLICATION_STRING2(STRING_MTPNP_ENTER_CARD2PIN, "Card2 PIN Verify", "SIM2 PIN Verify");
    ADD_APPLICATION_STRING2(STRING_MTPNP_ENTER_CARD1PIN2, "Card1 PIN2 Verify", "SIM1 PIN2 Verify");
    ADD_APPLICATION_STRING2(STRING_MTPNP_ENTER_CARD2PIN2, "Card2 PIN2 Verify", "SIM2 PIN2 Verify");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CANCEL_CARD1PIN, "Cancel SIM1 PIN", "Cancel SIM1 PIN");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CANCEL_CARD2PIN, "Cancel SIM2 PIN", "Cancel SIM2 PIN");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD1_SECURITY_SETTING, "Card1 Security Setting", "Security Setting");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SECURITY_SETTING, "Card2 Security Setting", "Security Setting");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD1_PINLOCK, "Card1 SIM Lock", "Card1 SIM Lock");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD2_PINLOCK, "Card2 SIM Lock", "Card2 SIM Lock");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CHANGE_PASSWORD, "Change password", "Change password");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CP_PIN1, "Card2 PIN1", "Card2 PIN1");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CP_PIN2, "Card2 PIN2", "Card2 PIN2");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CP_SIM1PIN1, "Card1 PIN1", "Card1 PIN1");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CP_SIM1PIN2, "Card1 PIN2", "Card1 PIN2");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SECURITY_CAPTION, "Security setting", "Security setting");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CHANGE_PASSWORD_CAPTION, "Card2 change password", "Card2 change password");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_PIN_BLOCKED, "Card1 PIN Blocked!", "Card1 PIN Blocked!");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_PIN2_BLOCKED, "Card1 PIN2 Blocked!", "Card1 PIN Blocked!");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_PIN_BLOCKED_PUK, "Card2 PIN Blocked!", "Card2 PIN Blocked!");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_PIN_BLOCKED_PUK2, "Card2 PIN2 Blocked!", "Card2 PIN Blocked!");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_ENTER_CARD1PUK, "Enter Card1 PUK:", "Enter Card1 PUK Screen MSG.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_ENTER_CARD1_NEW_PIN, "Enter Card1 New PIN:",
//del cont.                            "Enter Card1 New Pin First Time Screen MSG.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_ENTER_CARD2PUK, "Enter Card2 PUK:", "Enter Card2 PUK Screen MSG.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_ENTER_CARD2_NEW_PIN, "Enter Card2 New PIN:",
//del cont.                            "Enter Card2 New Pin First Time Screen MSG.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_ENTER_PIN_AGAIN, "Enter Card2 Again:", "Card2 Confirm Pin Screen MSG.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD1_PIN_DONOT_MATCH, "Different Card1 PIN code!",
//del cont.                            "Running Text of CARD1 PIN Mismatch Screen.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD1_PIN2_DONOT_MATCH, "Different Card1 PIN2 code!",
//del cont.                            "Running Text of CARD1 PIN2 Mismatch Screen.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD2_PIN_DONOT_MATCH, "Different Card2 PIN code!",
//del cont.                            "Running Text of CARD2 PIN Mismatch Screen.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD2_PIN2_DONOT_MATCH, "Different Card2 PIN2 code!",
//del cont.                            "Running Text of CARD2 PIN2 Mismatch Screen.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CODE_VERIFIED, "Card2 Code Accepted!",
//del cont.                            "String associated with Code Verified Screen.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_INCORRECT_CARD1_PUK, "Wrong Card1 PUK!",
//del cont.                            "Running text for incorrct Card1 PUK.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_INCORRECT_CARD1_PUK2, "Wrong Card1 PUK2!",
//del cont.                            "Running text for incorrct Card1 PUK.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_INCORRECT_CARD2_PUK, "Wrong Card2 PUK!",
//del cont.                            "Running text for incorrct Card2 PUK.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_INCORRECT_CARD2_PUK2, "Wrong Card2 PUK2!",
//del cont.                            "Running text for incorrct Card2 PUK.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD1_PIN_CHANGE, "Card1 PIN changed successfully!",
//del cont.                            "Card1 PIN changed successfully Message.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD1_PIN2_CHANGE, "Card1 PIN2 changed successfully!",
//del cont.                            "Card1 PIN2 changed successfully Message.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD2_PIN_CHANGE, "Card2 PIN changed successfully!",
//del cont.                            "Card2 PIN changed successfully Message.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD2_PIN2_CHANGE, "Card2 PIN2 changed successfully!",
//del cont.                            "Card2 PIN2 changed successfully Message.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_WRONG_CARD1_PIN, "Wrong Card1 PIN Code!", "Wrong Card1 PIN Code Message.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_WRONG_CARD1_PIN2, "Wrong Card1 PIN2 Code!", "Wrong Card1 PIN2 Code Message.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_WRONG_CARD2_PIN, "Wrong Card2 PIN Code!", "Wrong Card2 PIN Code Message.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_WRONG_CARD2_PIN2, "Wrong Card2 PIN2 Code!", "Wrong Card2 PIN2 Code Message.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SETTING_CARD1_SIM_LOCK, "Card1 SIM Locked!", "card1 SIM Locked.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SETTING_CARD1_SIM_UNLOCK, "Card1 SIM Unlocked!", "card1 SIM UnLocked.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SETTING_CARD2_SIM_LOCK, "Card2 SIM Locked!", "card2 SIM Locked.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SETTING_CARD2_SIM_UNLOCK, "Card2 SIM Unlocked!", "card2 SIM UnLocked.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_ENABLE_CARD1_PIN, "Card1 PIN Lock Must Be Enabled!",
//del cont.                            "Enable CARD1 PIN Lock String.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_ENABLE_CARD2_PIN, "Card2 PIN Lock Must Be Enabled!",
//del cont.                            "Enable CARD2 PIN Lock String.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SETTING_CARD2_PUK, "Enter Card2 PUK", "Enter CARD2 PUK Screen String.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SETTING_OLD_CARD1_PIN, "Enter Card1 Old PIN", "Enter CARD1 Old PIN.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SETTING_NEW_CARD1_PIN, "Card1 New PIN", "CARD1 New PIN.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SETTING_OLD_CARD1_PIN2, "Enter Card1 Old PIN2", "Enter CARD1 Old PIN2.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SETTING_NEW_CARD1_PIN2, "Card1 New PIN2", "CARD1 New PIN2.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SETTING_OLD_CARD2_PIN, "Enter Card2 Old PIN", "Enter CARD2 Old PIN.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SETTING_NEW_CARD2_PIN, "Card2 New PIN", "CARD2 New PIN.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SETTING_OLD_CARD2_PIN2, "Enter Card2 Old PIN2", "Enter CARD2 Old PIN2.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SETTING_NEW_CARD2_PIN2, "Card2 New PIN2", "CARD2 New PIN2.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SETTING_PIN2_BLOCK_MSG, "Please Contact Service Provider!",
//del cont.                            "Please Contact Service Provider!");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SETTING_SIM1_PUK2, "Enter Card1 PUK2", "Enter PUK2 Message.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SETTING_SIM1_NEW_PIN2, "Enter Card1 New PIN2", "Enter New PIN2 Message.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SETTING_ENTER_PUK2, "Enter Card2 PUK2", "Enter PUK2 Message.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SETTING_ENTER_NEW_PIN2, "Enter Card2 New PIN2", "Enter New PIN2 Message.");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SMS_MASTER_DONE, "Master Done", "Master Done");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SMS_SLAVE_DONE, "Slave Done", "Slave Done");
    ADD_APPLICATION_STRING2(STRING_MTPNP_MASTER_SEND_OPTIONS_CAPTION, "Master Send Option", "Master Send Option");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SLAVE_SEND_OPTIONS_CAPTION, "Slave Send Option", "Slave Send Option");
    ADD_APPLICATION_STRING2(STRING_MTPNP_MASTER_SENDING_SMS, "Master: Sending SMS", "Master: Sending SMS");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SLAVE_SENDING_SMS, "Slave: Sending SMS", "Slave: Sending SMS");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_QUERY_DELETEALL_CARD2, "Delete All card2 phonebook?",
//del cont.                            "Delete All card2 phonebook?");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_MASTER_MSGBOX_FORWARD, "Master Forward", "Master Forward");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SLAVE_MSGBOX_FORWARD, "Slave Forward", "Slave Forward");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SMS_COPY_TO_SIM2, "Copy to SIM2", "SMS copy function Copy to SIM String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SMS_MOVE_TO_SIM2, "Move to SIM2", "SMS move function Move to SIM String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SMS_FROM_SIM2, "From SIM2", "SMS copy function From SIM String");

    /* plmn begin */
    ADD_APPLICATION_STRING2(STRING_MTPNP_CARD1_NETWORK_SETUP, "Card1 Network Setup", "Menu item string for Network Setup");
    ADD_APPLICATION_STRING2(STRING_MTPNP_NETWORK_SETUP, "Card2 Network Setup", "Menu item string for Network Setup");
    ADD_APPLICATION_STRING2(STRING_MTPNP_NETWORK_SETUP_CAPTION, "Network Setup", "Title string for Network Setup menu");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SELECTION_MODE, "Network Selection", "Menu item string for Selection Mode");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SELECTION_MODE_CAPTION, "Network Selection",
//del cont.                            "Title string for Selection Mode menu");
    ADD_APPLICATION_STRING2(STRING_MTPNP_PREFERRED_NETWORKS, "Preferred Networks",
                            "Menu item string for Preferred Networks");
    ADD_APPLICATION_STRING2(STRING_MTPNP_GPRS_CONNECTION, "GPRS Connection",
                            "Menu item string for GPRS Connection");
    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_MANUAL, "Manual", "Menu item string for Selection Mode->Manual");
    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_AUTOMATIC, "Automatic",
                            "Menu item string for Selection Mode->Automatic");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_NEW_SEARCH, "New Search", "New Search");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_SELECT_NETWORK, "Select Network", "Select Network");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_CHOICE, "Selection Mode", "Network Selection Mode");
    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_OPTIONS, "Preferred Networks Options",
                            "String for options button in preferred network");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_PRF_NWK_ADD, "Add From List", "Menu Item Add Preferred Network");
    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_PRF_NWK_NEW, "Create New", "Create New preferred network");
    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_PRF_NWK_EDIT, "Change Priority", "Menu Item Edit Preferred Network");
    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_SIM2_INVALID, "Not avaliable for invalid SIM2.",
                            "Invalid SIM network selection text.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_CHANGE, "Change", "LSK Display for Change");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_MANUAL_MODE, "Select Network", "Select Network As Manual Mode");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_PRF_NWK_ADDED_ERR, "Unable To Add Network",
//del cont.                            "Message String for Deleted Preferred Network Error");
    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_PRF_ADD_SHW_NWK_TITLE, "PLMN LIST",
                            "Title String for Error Screen Preferred Network");
    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_PRF_ADD_SHW_NWK_PRIORITY_TITLE, "Priority",
                            "Title String for User Priority Preferred Network");
    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_PRR_MCC_CAPTION, "MCC/MNC", "MCC/MNC String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_PRR_PR_CAPTION, "Priority", "Priority String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_PRR_SV_CAPTION, "Service", "Service String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_PLMN_LIST_SORT_BY_ALPHABET, "Sort by alphabet", "Sort PLMN list");
    ADD_APPLICATION_STRING2(STRING_MTPNP_PLMN_LIST_SORT_BY_DEFAULT, "Sort by default", "Sort PLMN list");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_SEARCHIN_NW, "Searching", "Searching Network Message.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_REGISTERING_NW, "Registering", "Registering Network Message.");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_NW_SELECT_FAIL, "Fail to register!", "Fail to register!");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_NW_LIST_FAIL, "Fail to search network!", "Fail to search network!");
    ADD_APPLICATION_STRING2(STRING_MTPNP_NETSET_SELECT_NETWORK_CAPTION, "Network List",
                            "Title string for Select Network screen");
    /* plmn end */

    /* cell broadcast messages begin */
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_CB_MENUENTRY, "SIM1 Broadcast Message",
//del cont.                            "Cell Broadcast menu item string");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_MENUENTRY, "SIM2 Broadcast Message", "Cell Broadcast menu item string");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_SERVICE, "Receive Mode", "CB Menu Item Service Cell Broadcast String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_READ_MESSAGE, "Read Message", "CB Menu Read Message Cell Broadcast String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_CHANNELS, "Channel Setting", "CB Menu Channels Cell Broadcast String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_LANGUAGE, "Languages", "CB Menu Language Cell Broadcast String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_MESSAGE_BODY, "Message Body", "CB Menu Message Body String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_SELECT_CHNL, "Select Channel", "CB Menu Select Channel String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_CHNL_EDITOR, "Channel Editor", "CB Menu Channel Editor String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_CHNL_TITLE, "Channel Title:", "CB Menu Channel Title: String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_CHNL_TITLE_FULL_EDIT, "Channel Title", "CB Menu Channel Title String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_CHNL_NUMBER, "Channel Number:", "CB Menu Channel Number String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_CHNL_LIST, "Channel List", "CB Menu Channel List String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_LANGUAGE_CAPTION, "Languages",
//del cont.                            "CB Menu Item Cell Broadcast Language Caption String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_CHANNELS_CAPTION, "Channels",
//del cont.                            "CB Menu Item Cell Broadcast Channels Caption String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_CHANNEL_ENBL_OPTIONS, "Channel Options",
                            "CB Menu Item Cell Broadcast Channels Options Enable String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_CHANNEL_ENBL_OPTIONS_CAPTION, "Channels",
//del cont.                            "CB Menu Item Cell Broadcast Channels Options Caption String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_LANGUAGE_UNSPECIFIED, "unspecified",
//del cont.                            "CB Menu Item Cell Broadcast Language Option Unspecified Language String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_ERROR_MESSAGE, "Failed!", "CB error String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_NOT_READY_YET, "CB Not Ready Yet!", "CB CB Not Ready Yet! String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SMS_CB_CHNL_FULL, "Channel Full!", "Channel Full Message SMS String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_CHNL_DUPLICATE_NUMBER_NOT_ALLOWED, "Duplicate number not allowed!",
//del cont.                            "CB Duplicate number not allowed String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_CHNL_NUMBER_LEN_EXCEEDED, "Number Exceeded!", "Number Exceeded! String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_ALL_CHANNELS_FOR_SELECT, "All Channels", "CB All Channels String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_ALL_LANGUAGES_FOR_SELECT, "All Languages", "CB All Languages String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_CB_MSG_IND_INFORMATION_CAPTION, "Information", "CB  Information String");
    /* cell broadcast messages end */

    /* voice mail begin */
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_VMAIL_MENUENTRY, "SIM1 Voice Mail Server", "Voice Mail menu item string");
    ADD_APPLICATION_STRING2(STRING_MTPNP_VMAIL_MENUENTRY, "SIM2 Voice Mail Server", "Voice Mail menu item string");
    ADD_APPLICATION_STRING2(STRING_MTPNP_VOICE_MAIL_NOT_READY_YET, "VM Not Ready Yet!",
                            "Message Voice Mail Not Ready String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_VOICE_MAIL_OPTION_ID, "Voice Mail Option", "Message Voice Mail Option String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_VM_CONNECT_TO_VOICE_ID, "Connect to Voice", "Message Connect to Voice String");
    /* voice mail end */

    /* message setup begin */
    ADD_APPLICATION_STRING2(STRING_MTPNP_MESSAGESETUP_MENUENTRY, "SIM2 Message Settings",
                            "Message setup menu item string");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_MESSAGESETUP_MENUENTRY, "SIM1 Message Settings",
                            "Message setup menu item string");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SMS_MEMSTATUS_PREFIX_SIM, "SIM: ", "Memory Status display prefix for SIM");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SMS_MEMSTATUS_PREFIX_PHONE, "PHONE: ",
//del cont.                            "Memory Status display prefix for PHONE");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_ME_STORAGE_MENUENTRY, "Phone", "Message ME Memory Setting SMS String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM_STORAGE_MENUENTRY, "SIM", "Message SIM Memory Setting SMS String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_PROFILE_SETTING_MENUENTRY, "Profile Setting",
                            "Message Profile Setting  SMS  String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_COMMONSETTING_MENUENTRY, "Common Setting",
                            "Message Setup Common Settings SMS  String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_PREFEREDSTORAGE_MENUENTRY, "Preferred Storage",
                            "Message Setup Preferred Storage SMS  String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SMSSTATUS_MENUENTRY, "Memory Status", "SMS Status SMS menu item String");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SMSSTATUS_TITLE, "SMS Status", "SMS Status SMS menu item String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_PROFILE_NAME, "Profile Name", "SMS Profile Name String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_PROFILE_SC, "SC Address", "SMS Profile Name String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_PROFILE_VP, "Valid Period", "SMS Profile Name String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_PROFILE_MSG_TYPE, "Message Type", "SMS Profile Name String");
    /* message setup end */

     /* adn begin */
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_PHB_COPYALL_SIM1TOPHONE, "From SIM1 To Phone", "CopyAll For Slave");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_PHB_COPYALL_PHONETOSIM1, "From Phone To SIM1", "CopyAll For Slave");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2PHB_COPYALL_SIM2TOPHONE, "From SIM2 To Phone", "CopyAll For Slave");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2PHB_COPYALL_PHONETOSIM2, "From Phone To SIM2", "CopyAll For Slave");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2PHB_COPYALL_SIM1TOSIM2, "From SIM1 To SIM2", "CopyAll For Slave");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2PHB_COPYALL_SIM2TOSIM1, "From SIM2 To SIM1", "CopyAll For Slave");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2PHB_COPY_SIM2TOPHONE, "Copy From SIM2 To Phone",
                            "Copy For Slave From SIM2 to Phone");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2PHB_COPY_SIM2TOSIM1, "Copy From SIM2 To SIM1",
                            "Copy For Slave From Phone to SIM2");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2PHB_MOVE_SIM2TOPHONE, "Move From SIM2 To Phone",
                            "Move For Slave From SIM2 to Phone");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2PHB_MOVE_SIM2TOSIM1, "Move From SIM2 To SIM1",
                            "Move For Slave From Phone to SIM2");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_VCARD_SAVE_TO_SIM1, "Save to SIM1", "Save to SIM1");
    ADD_APPLICATION_STRING2(STRING_MTPNP_VCARD_SAVE_TO_SIM2, "Save to SIM2", "Save to SIM2");

//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_STORAGE_FULL, "SIM1 IS Full!", "Message for SIM1 IS Full");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2_STORAGE_FULL, "SIM2 IS Full!", "Message for SIM2 IS Full");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1PHB_ADD_NEW_ENTRY, "To SIM1", "Add New Entry To SIM1");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2PHB_ADD_NEW_ENTRY, "To SIM2", "Add New Entry To SIM2");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2PHB_DELETEALL, "From SIM2", "Delete All From SIM2");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1PHB_DELETEALL, "SIM1", "Memory Status of SIM1");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_NLF_SIM1, "Only SIM1", "NLF SIM1");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_NLF_ALL, "All", "NLF ALL");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_MEMORY_STATUS, "SIM1", "Memory Status of SIM1");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SLAVE_MEMORY_STATUS, "SIM2", "Memory Status of SIM2");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2PHB_PRESTORAGE, "SIM2", "Memory Status of SIM2");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2PHB_NLF_SIM2, "SIM2 Only", "Menu item string for List Filter SIM2");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_PHB_STORAGE_SIM2, "SIM2", "Menu item string for Storage Location SIM2");

    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2PHB_COPY_PHONETOSIM2, "Copy From Phone To SIM2",
                            "Copy For Slave From Phone to SIM2");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2PHB_COPY_SIM1TOSIM2, "Copy From SIM1 To SIM2",
                            "Copy For Slave From SIM1 to SIM2");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2PHB_MOVE_PHONETOSIM2, "Move From Phone To SIM2",
                            "Move For Slave From Phone to SIM2");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2PHB_MOVE_SIM1TOSIM2, "Move From SIM1 To SIM2",
                            "Move For Slave From SIM1 to SIM2");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2PHB_COPY_SIM1TOPHONE, "Copy From SIM1 To Phone",
                            "Copy For Slave From SIM1 To Phone");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2PHB_COPY_PHONETOSIM1, "Copy From Phone To SIM1",
                            "Copy For Slave From Phone To SIM1");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2PHB_MOVE_SIM1TOPHONE, "Move From SIM1 To Phone",
                            "Move For Slave From SIM1 To Phone");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2PHB_MOVE_PHONETOSIM1, "Move From Phone To SIM1",
                            "Move For Slave From Phone To SIM1");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_PHB_COPY_ALL_FROM_SIM1, "From SIM1", "CopyAll From SIM1");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_PHB_COPY_ALL_FROM_SIM2, "From SIM2", "CopyAll From SIM2");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_PHB_COPY_TO_SIM1_DONE, "Copied to SIM1", "Copied to SIM1");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_PHB_COPY_TO_SIM2_DONE, "Copied to SIM2", "Copied to SIM2");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_PHB_MOVE_TO_SIM1_DONE, "Moved to SIM1", "Moved to SIM1");    
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_PHB_MOVE_TO_SIM2_DONE, "Moved to SIM2", "Moved to SIM2");    
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_PHB_SOSLIST_INSIM1, "in sim1", "in sim1");    
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_PHB_SOSLIST_INSIM2, "in sim2", "in sim2");    

     /* adn end */

     /* adn begin */
    ADD_APPLICATION_STRING2(STRING_MTPNP_SLAVE_FIXED_DIAL, "Slave Fixed Dial", "Fixed Dial Menu.");
     /* adn end */

     /*mtpnp call set */
    ADD_APPLICATION_STRING2(STRING_MTPNP_CALL_FORWARD, "Call Forward", "Call Forward");
    ADD_APPLICATION_STRING2(STRING_MTPNP_LINE_WAIT, "Call Wait", "Call Wait");
    ADD_APPLICATION_STRING2(STRING_MTPNP_LINE_BARRING, "Call Barring", "Call Barring");
    /*mtpnp call set */

    /* sms show begin */
    ADD_APPLICATION_STRING2(STRING_MTPNP_SMS_MASTER_INBOX, "Master Inbox", "Inbox SMS menu item String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SMS_SLAVE_INBOX, "Slave Inbox", "Inbox SMS menu item String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SMS_MASTER_OUTBOX, "Master Outbox", "Outbox SMS menu item String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SMS_SLAVE_OUTBOX, "Slave Outbox", "Outbox SMS menu item String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SMS_SHOW_STYLE, "Show Style", "Outbox SMS menu item String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SMS_SHOW_MIXED, "Show Mixed", "Outbox SMS menu item String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SMS_SHOW_DIVIDED, "Show Divided", "Outbox SMS menu item String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_NEW_MASTER_MESSAGE_FROM_ID, "Master Message From ",
                            "SMS New Message From String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_NEW_SLAVE_MESSAGE_FROM_ID, "Slave Message From ",
                            "SMS New Message From String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_MASTER_RECEIVED_SMS, "New Master Message!",
                            "SMS while receiving sms Message String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SLAVE_RECEIVED_SMS, "New Slave Message!",
                            "SMS while receiving sms Message String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_MASTER_DELIVERY_REPORT_MENUENTRY, "Master Delivery Report",
                            "Message Delivery report SMS String");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SLAVE_DELIVERY_REPORT_MENUENTRY, "Slave Delivery Report",
                            "Message Delivery report SMS String");
	ADD_APPLICATION_STRING2 (STRING_MTPNP_MASTER_DELETE_INBOX_MENUENTRY,	"Delete SIM1 Inbox?",	"SMS Delete SIM1 Inbox Query String");
	ADD_APPLICATION_STRING2 (STRING_MTPNP_MASTER_DELETE_OUTBOX_MENUENTRY,	"Delete SIM1 Outbox?",	"SMS Delete SIM1 Outbox Query String");
	ADD_APPLICATION_STRING2 (STRING_MTPNP_SLAVE_DELETE_INBOX_MENUENTRY,	"Delete SIM2 Inbox?",	"SMS Delete SIM2 Inbox Query String");
	ADD_APPLICATION_STRING2 (STRING_MTPNP_SLAVE_DELETE_OUTBOX_MENUENTRY,	"Delete SIM2 Outbox?",	"SMS Delete SIM2 Outbox Query String");

	/* sms show end */

	/* sms message waiting begin*/
	ADD_APPLICATION_STRING2 (STRING_MTPNP_MASTER_VOICEMAIL_IND,			"New SIM1 Voice Mail!",	"New SIM1 voice Mail SMS Notification Caption String");
	ADD_APPLICATION_STRING2 (STRING_MTPNP_MASTER_EMAIL_IND,			"New SIM1 Fax!",	"New SIM1 Fax SMS Notification Caption String");
	ADD_APPLICATION_STRING2 (STRING_MTPNP_MASTER_FAX_IND,			"New SIM1 E-Mail!",	"New SIM1 E-Mail SMS Notification Caption String");
//del by reslim v0.1	ADD_APPLICATION_STRING2 (STRING_MTPNP_MASTER_NET_OTHER_IND,			"New SIM1 network message",	"New SIM1 other SMS Notification Caption String");
	
//del by reslim v0.1	ADD_APPLICATION_STRING2 (STRING_MTPNP_MASTER_VOICEMAIL_NUM_IND,			"New SIM1 Voice Mail(s)!",	"Number of new SIM1 voice Mail SMS Notification Caption String");
//del by reslim v0.1	ADD_APPLICATION_STRING2 (STRING_MTPNP_MASTER_FAX_NUM_IND,			"New SIM1 Fax(es)!",	"Number of new Fax SIM1 SMS Notification Caption String");
//del by reslim v0.1	ADD_APPLICATION_STRING2 (STRING_MTPNP_MASTER_EMAIL_NUM_IND,			"New SIM1 E-Mail(s)!",	"Number of new SIM1 E-Mail SMS Notification Caption String");
//del by reslim v0.1    ADD_APPLICATION_STRING2 (STRING_MTPNP_MASTER_NET_OTHER_NUM_IND,			"New SIM1 network message(s)!",	"Number of New SIM1 other SMS Notification Caption String");

	ADD_APPLICATION_STRING2 (STRING_MTPNP_SLAVE_VOICEMAIL_IND,			"New SIM2 Voice Mail!",	"New SIM2 voice Mail SMS Notification Caption String");
	ADD_APPLICATION_STRING2 (STRING_MTPNP_SLAVE_EMAIL_IND,			"New SIM2 Fax!",	"New SIM2 Fax SMS Notification Caption String");
	ADD_APPLICATION_STRING2 (STRING_MTPNP_SLAVE_FAX_IND,			"New SIM2 E-Mail!",	"New SIM2 E-Mail SMS Notification Caption String");
//del by reslim v0.1	ADD_APPLICATION_STRING2 (STRING_MTPNP_SLAVE_NET_OTHER_IND,			"New SIM2 network message",	"New SIM2 other SMS Notification Caption String");
	
//del by reslim v0.1	ADD_APPLICATION_STRING2 (STRING_MTPNP_SLAVE_VOICEMAIL_NUM_IND,			"New SIM2 Voice Mail(s)!",	"Number of new SIM2 voice Mail SMS Notification Caption String");
//del by reslim v0.1	ADD_APPLICATION_STRING2 (STRING_MTPNP_SLAVE_FAX_NUM_IND,			"New SIM2 Fax(es)!",	"Number of new Fax SIM2 SMS Notification Caption String");
//del by reslim v0.1	ADD_APPLICATION_STRING2 (STRING_MTPNP_SLAVE_EMAIL_NUM_IND,			"New SIM2 E-Mail(s)!",	"Number of new SIM2 E-Mail SMS Notification Caption String");
//del by reslim v0.1    ADD_APPLICATION_STRING2 (STRING_MTPNP_SLAVE_NET_OTHER_NUM_IND,			"New SIM2 network message(s)!",	"Number of New SIM2 other SMS Notification Caption String");
	/* sms message waiting end*/

	/* sms memory exceeded begin */
	ADD_APPLICATION_STRING2 (STRING_MTPNP_MASTER_SMS_MEMORY_EXCEEDED,		"SIM1 phone Storage Full for New Incoming Message!",	"Memory Exceeded SIM1 Message SMS String");
//del by reslim v0.1	ADD_APPLICATION_STRING2 (STRING_MTPNP_MASTER_SMS_SIM_MEMORY_EXCEEDED,		"SIM1 SIM Storage Full for New Incoming Message!",	"SIM Memory Exceeded SIM1 Message SMS String");
	ADD_APPLICATION_STRING2 (STRING_MTPNP_SLAVE_SMS_MEMORY_EXCEEDED,		"SIM2 phone Storage Full for New Incoming Message!",	"Memory Exceeded SIM2 Message SMS String");
//del by reslim v0.1	ADD_APPLICATION_STRING2 (STRING_MTPNP_SLAVE_SMS_SIM_MEMORY_EXCEEDED,		"SIM2 SIM Storage Full for New Incoming Message!",	"SIM Memory Exceeded SIM2 Message SMS String");
	/* sms memory exceeded end */


    /* ucm begin */
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_ID_UCM_VOICE_ACTIVE, "SIM2 Voice Active", "Voice Active string");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_ID_UCM_VOICE_HOLD, "SIM2 Voice Hold", "Voice Hold string");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_ID_UCM_DIAL_SIM2, "Dial Card2", "Dial Card2");
    /* ucm end */


//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1DIAL, "SIM1 Dial", "Global String- Dial");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2DIAL, "SIM2 Dial", "Global String- Dial");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_PHB_OPTION_IP_DIAL_SIM1, "SIM1 IP Dial",
//del cont.                            "Menu item string for View options->IP1Dial");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_PHB_OPTION_IP_DIAL_SIM2, "SIM2 IP Dial",
//del cont.                            "Menu item string for View options->IP2 Dial");

#ifdef __MMI_MESSAGES_CHAT__
    ADD_APPLICATION_STRING2(STRING_MTPNP_CHOOSE_MASTER_CHAT, "Choose Master", "Choose Master");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CHOOSE_SLAVE_CHAT, "Choose Slave", "Choose Slave");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CHAT_NOT_SUPPORT, "Not Support", "Not Support");
#endif /* __MMI_MESSAGES_CHAT__ */
    ADD_APPLICATION_STRING2(STRING_MTPNP_CALL_SIM1, "SIM1 Dialing", "SIM1 Dialing");
    ADD_APPLICATION_STRING2(STRING_MTPNP_CALL_SIM2, "SIM2 Dialing", "SIM2 Dialing");
    ADD_APPLICATION_STRING2(STRING_MTPNP_INCOMING_SIM1, "SIM1-Incoming", "SIM1-Incoming");
    ADD_APPLICATION_STRING2(STRING_MTPNP_INCOMING_SIM2, "SIM2-Incoming", "SIM2-Incoming");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1, "SIM1", "SIM1");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2, "SIM2", "SIM2");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SRC_SELECT, "Select source", "Select source");
    ADD_APPLICATION_STRING2(STRING_MTPNP_DST_SELECT, "Select destination", "Select destination");

    ADD_APPLICATION_STRING2(STRING_MTPNP_OUTBOX_MASTER_SEND_SMS, "Send From SIM1", "Send From SIM1");
    ADD_APPLICATION_STRING2(STRING_MTPNP_OUTBOX_SLAVE_SEND_SMS, "Send From SIM2", "Send From SIM2");    
    ADD_APPLICATION_STRING2(STRING_MTPNP_SAVE_SMS_TO_SIM1, "Save to SIM1", "Save to SIM1");
    ADD_APPLICATION_STRING2(STRING_MTPNP_SAVE_SMS_TO_SIM2, "Save to SIM2", "Save to SIM1");
    ADD_APPLICATION_STRING2(STRING_MTPNO_SMS_NOT_USABLE, "SMS not usable", "SMS not usable");

    /* SIM-ME-Lock */
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_ENTER_NP_MSG, "Input SIM1 NP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_ENTER_NSP_MSG, "Input SIM1 NSP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_ENTER_SP_MSG, "Input SIM1 SP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_ENTER_CP_MSG, "Input SIM1 CP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_ENTER_SIMP_MSG, "Input SIM1 SIMP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_ENTER_NSSP_MSG, "Input SIM1 NS-SP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_ENTER_SIMC_MSG, "Input SIM1 SIM-C Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2_ENTER_NP_MSG, "Input SIM2 NP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2_ENTER_NSP_MSG, "Input SIM2 NSP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2_ENTER_SP_MSG, "Input SIM2 SP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2_ENTER_CP_MSG, "Input SIM2 CP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2_ENTER_SIMP_MSG, "Input SIM2 SIMP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2_ENTER_NSSP_MSG, "Input SIM2 NS-SP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2_ENTER_SIMC_MSG, "Input SIM2 SIM-C Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_WRONG_NP_MSG, "Wrong SIM1 NP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_WRONG_NSP_MSG, "Wrong SIM1 NSP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_WRONG_SP_MSG, "Wrong SIM1 SP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_WRONG_CP_MSG, "Wrong SIM1 CP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_WRONG_SIMP_MSG, "Wrong SIM1 SIMP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_WRONG_NSSP_MSG, "Wrong SIM1 NS-SP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_WRONG_SIMC_MSG, "Wrong SIM1 SIM-C Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2_WRONG_NP_MSG, "Wrong SIM2 NP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2_WRONG_NSP_MSG, "Wrong SIM2 NSP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2_WRONG_SP_MSG, "Wrong SIM2 SP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2_WRONG_CP_MSG, "Wrong SIM2 CP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2_WRONG_SIMP_MSG, "Wrong SIM2 SIMP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2_WRONG_NSSP_MSG, "Wrong SIM2 NS-SP Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2_WRONG_SIMC_MSG, "Wrong SIM2 SIM-C Code", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_AUTO_LOCKED, "SIM1 auto locked", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2_AUTO_LOCKED, "SIM2 auto locked", "SIM-ME-Lock");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_SIM2_AUTO_LOCKED, "SIM1 and SIM2 auto locked", "SIM-ME-Lock");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_INC_MHLD_OPT_END_CARD1, "End All Card1", "String for option End Held Call");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_IAMH_ALL_HELD_END_CARD2, "End All Held Card2", "End All Held Card2");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_IAMH_ALL_HELD_END_CARD1, "End All Held Card1", "End All Held Card1");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_IAH_OPT_END_ALL_CARD1, "End All Card1", "End All Card1");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_IAH_OPT_END_HLD_CARD2, "End Held Card2", "End Held Card2");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_IAH_OPT_END_HLD_CARD1, "End Held Card1", "End Held Card1");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_IAH_OPT_END_ACT_CARD2, "End Active Card2", "End Active Card2");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_IAH_OPT_END_ACT_CARD1, "End Active Card1", "End Active Card1");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_IAH_OPT_CONFER_CARD2, "Conference Card2", "Conference Card2");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_IAH_OPT_CONFER_CARD1, "Conference Card1", "Conference Card1");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_IAH_OPT_SWAP_CARD2, "Swap Card2", "Swap Card2");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_IAH_OPT_SWAP_CARD1, "Swap Card1", "Swap Card1");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_INC_MHLD_OPT_END_CARD2, "End All Card2", "End All Card2");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_INC_MHLD_OPT_RTRV_CARD2, "Retrieve All Card2", "Retrieve All Card2");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_INC_MHLD_OPT_RTRV_CARD1, "Retrieve All Card1", "Retrieve All Card1");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_IMA_OPT_END_ALL_ACT_CARD2, "End All Card2", "End All Card2");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_IMA_OPT_END_ONE_ACT_CARD2, "End Single Card2", "End Single Card2");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_IMA_OPT_END_ONE_ACT_CARD1, "End Single Card1", "End Single Card1");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_IMA_OPT_SPLIT_CARD2, "Split Card2", "Split Card2");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_IMA_OPT_HOLD_CONF_CARD2, "Hold All Card2", "Hold All Card2");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_IMA_OPT_HOLD_CONF_CARD1, "Hold All Card1", "Hold All Card1");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_INC_HLD_OPT_RTRV_CARD2, "Retrieve Card2", "Retrieve Card2");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_INC_HLD_OPT_RTRV_CARD1, "Retrieve Card1", "Retrieve Card1");
	ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_INC_ACT_OPT_END_ACT_CARD2, "End Card2", "End Card2");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_INC_ACT_OPT_END_ACT_CARD1, "End Card1", "End Card1");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_INC_ACT_OPT_HOLD_ACT_CARD2, "Hold Card2", "Hold Card2");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_INC_ACT_OPT_HOLD_ACT_CARD1, "Hold Card1", "Hold Card1");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_INC_OPT_REJECT_CARD2, "Reject Card2", "Reject Card2");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_INC_OPT_REJECT_CARD1, "Reject Card1", "Reject Card1");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_IMA_OPT_SPLIT_CARD1, "Split Card1", "Split Card1");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_IMA_OPT_END_ALL_ACT_CARD1, "End All Card1", "End All Card1");

	ADD_APPLICATION_STRING2(STRING_MTPNP_SIM1_NOT_AVAILABLE, "SIM1 not available", "SIM1 not available");
	ADD_APPLICATION_STRING2(STRING_MTPNP_SIM2_NOT_AVAILABLE, "SIM2 not available", "SIM2 not available");

    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_IAH_OPT_END_ALL_CARD2, "End All Card2", "End All Card2");
#if 0
    ADD_APPLICATION_MENUITEM((MITEM2019_CM_ALL_ACTIVE_END, SLAVE_MENU_CM_INCOMING_AND_ACTIVE_MIX_OPTION,
                              0, SHOW, MOVEABLEWITHINPARENT, DISP_LIST, SLAVE_STR_MITEM2019_CM_ALL_ACTIVE_END_CARD1,
                              0));
#endif
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM2019_CM_ALL_ACTIVE_END_CARD1, "End All Active Card1",
                            "End All Active Card1");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM2019_CM_ALL_ACTIVE_END_CARD2, "End All Active Card2",
                            "End All Active Card2");


//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_TITLE_SCR_USSN_MSG_CARD1, "Card 1 Information", "Title String for USSN Msg Display");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_TITLE_SCR_USSD_MSG_CARD1, "Card 1 Information", "Title String for USSD Msg Display");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_USSD_REQ_CAPTION_CARD1, "Card 1 Sending Request",
//del cont.                            "Display Caption for USSD req screen");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_TITLE_SCR_USSR_EDIT_MSG_CARD1, "Card 1 USSR-Edit", "Title String for USSR Msg Edit");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_TITLE_SCR_USSD_MSG_CARD2, "Card 2 Information", "Title String for USSN Msg Display");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_TITLE_SCR_USSN_MSG_CARD2, "Card 2 Information", "Title String for USSD Msg Display");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_TITLE_SCR_USSR_MSG_CARD1, "Card 1 Information", "Title String for USSR Msg Display");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_TITLE_SCR_USSR_MSG_CARD2, "Card 2 Information", "Title String for USSR Msg Display");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_TITLE_SCR_USSR_EDIT_MSG_CARD2, "Card 2 Sending Request",
//del cont.                            "Display Caption for USSD req screen");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_USSD_REQ_CAPTION_CARD2, "Card 2 USSR-Edit", "Title String for USSR Msg Edit");
//del by reslim v0.1    ADD_APPLICATION_STRING2(SLAVE_STR_CALL_FORBIDDEN, "Dial Forbidden", "Slave Dial Forbidden");
    ADD_APPLICATION_STRING2(SLAVE_STR_ID_CFT_SWAP_TO_MASTER, "Swap to Card1", "Swap to Master Call");
//del by reslim v0.1    ADD_APPLICATION_STRING2(SLAVE_STR_ID_CFT_ACTIVE_MASTER_CALL, "Active Master Call", "Active Master Call");
//del by reslim v0.1    ADD_APPLICATION_STRING2(SLAVE_STR_ID_CFT_ACTIVE_SLAVE_CALL, "Active Slave Call", "Active Slave Call");


//del by reslim v0.1    ADD_APPLICATION_STRING2(SLAVE_STR_CC_MO_SELETECT_SELECT_NET, "Net Selection", "Net Selection");
    ADD_APPLICATION_STRING2(SLAVE_STR_CC_MO_SELETECT_NET_CARD1, "Card 1 Call", "Card 1 Call");
    ADD_APPLICATION_STRING2(SLAVE_STR_CC_MO_SELETECT_NET_CARD2, "Card 2 Call", "Card 2 Call");
//del by reslim v0.1    ADD_APPLICATION_STRING2(SLAVE_STR_CC_SP_SELETECT_SELECT_NET, "Net Selection", "Net Selection");
//del by reslim v0.1    ADD_APPLICATION_STRING2(SLAVE_STR_CC_CSK_SELECT_NET, "Net Selection", "Net Selection");
    ADD_APPLICATION_STRING2(SLAVE_STR_CC_CSK_SELETECT_CARD1, "Card 1 Call", "Card 1 Call");
    ADD_APPLICATION_STRING2(SLAVE_STR_CC_CSK_SELETECT_CARD2, "Card 2 Call", "Card 2 Call");

    ADD_APPLICATION_STRING2(SLAVE_STR_CALL_INCOMING_MIX_OPT_ACCEPT_BOTH, "Accept Both", "Accept Both");
    ADD_APPLICATION_STRING2(SLAVE_STR_CALL_INCOMING_MIX_OPT_REJECT_BOTH, "Reject Both", "Reject Both");
    ADD_APPLICATION_STRING2(SLAVE_STR_CALL_INCOMING_MIX_OPT_ACCEPTCARD1, "Accept Card1", "Accept Card1");
    ADD_APPLICATION_STRING2(SLAVE_STR_CALL_INCOMING_MIX_OPT_ACCEPTCARD2, "Accept Card2", "Accept Card2");

//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_BOTH_INCOMING, CUST_IMG_PATH "/mercury/active/Incoming/CL_BOTH_INCOM.gif",
//del cont.                           "Image for Both Incoming");


    ADD_APPLICATION_STRING2(SLAVE_STR_CALL_INCOMING_OPT_CARD1_IP_DIALBACK, "Card1 IP Dial back", "Card1 IP Dial back");
    ADD_APPLICATION_STRING2(SLAVE_STR_CALL_INCOMING_OPT_CARD2_IP_DIALBACK, "Card2 IP Dial back", "Card2 IP Dial back");
    ADD_APPLICATION_STRING2(SLAVE_STR_CALL_INCOMING_OPT_ACCEPT, "Accept", "Accept");
    ADD_APPLICATION_STRING2(SLAVE_STR_CALL_INCOMING_OPT_REJECT, "Reject", "Reject");
    ADD_APPLICATION_STRING2(SLAVE_STR_CALL_INCOMING_OPT_CARD1_DIALBACK, "Card1 Dial back", "Card1 Dial back");
    ADD_APPLICATION_STRING2(SLAVE_STR_CALL_INCOMING_OPT_CARD2_DIALBACK, "Card2 Dial back", "Card2 Dial back");
    /*mtpnp ip dial */
#ifdef __IP_NUMBER__
    ADD_APPLICATION_MENUITEM((MENU_MTPNP_IP_NUM_ACTIVATE, MENU_SETTING_IP_NUM_OPN, 0,
                              0, NONMOVEABLE, 1, STRING_SLAVE_IP_ACTIVATE, NULL));
    ADD_APPLICATION_STRING2(STRING_SLAVE_IP_ACTIVATE, "Active Slave", "Active Slave");
   ADD_APPLICATION_STRING2(STRING_MASTER_IP_ACTIVATE, "Active Master", "Active Master");
    ADD_APPLICATION_STRING2(STRING_SLAVE_IP_DEACTIVATE, "Deactive Slave", "Deactive Slave");
   ADD_APPLICATION_STRING2(STRING_MASTER_IP_DEACTIVATE, "Deactive Master", "Deactive Slave");
    ADD_APPLICATION_IMAGE2(IMG_MTPNP_VICON_1, CUST_IMG_PATH "/MainLCD/DualSIM/IPdial/IP_0.gif", "master ip");
    ADD_APPLICATION_IMAGE2(IMG_MTPNP_VICON_2, CUST_IMG_PATH "/MainLCD/DualSIM/IPdial/IP_0.gif", "slave ip");
    ADD_APPLICATION_IMAGE2(IMG_MTPNP_VICON_12, CUST_IMG_PATH "/MainLCD/DualSIM/IPdial/IP_0.gif","both ip");
#endif
    ADD_APPLICATION_STRING2(SLAVE_STR_ID_CFT_SWAP_TO_SLAVE, "Swap to Slave", "Swap to Slave");

    ADD_APPLICATION_STRING2(SLAVE_STR_ID_CFT_SWAP_TO_MASTER, "Swap to Master", "Swap to Master");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_INC_OPT_ANSWER_CARD2, "Accept Card2", "Accept Card2");
    ADD_APPLICATION_STRING2(SLAVE_STR_MITEM_INC_OPT_ANSWER_CARD1, "Accept Card1", "Accept Card1");

	ADD_APPLICATION_STRING2(STRING_MTPNP_DM_CARD1_OPEN, "SIM1 open", "SIM1 open");
	ADD_APPLICATION_STRING2(STRING_MTPNP_DM_CARD2_OPEN, "SIM2 open", "SIM2 open");
	#if (MMI_SIM_NUMBER >= 3)
	ADD_APPLICATION_STRING2(STRING_MTPNP_CARD3_CLOSED, "SIM3 Closed", "SIM3 Closed");
	ADD_APPLICATION_STRING(STRING_MTPNP_CARD3ABSENT, "Insert SIM3");
	ADD_APPLICATION_STRING(STRING_MTPNP_CARD3_SIM_BLOCK, "SIM3 Blocked");
	ADD_APPLICATION_STRING(STRING_MTPNP_CARD3_NETWORK_SETUP,"Card3 Network Setup");
	ADD_APPLICATION_STRING2(STRING_MTPNP_DM_CARD3_OPEN, "SIM3 open", "SIM3 open");
	ADD_APPLICATION_STRING2(STR_SIM3_FIXED_DAIL_CAPTION,"SIM3 Fixed Dial","SIM2 Fixed Dial Menu.");
	ADD_APPLICATION_STRING2(STR_SIM3_BARRED_DIAL_CAPTION,"SIM3 Barred Dial","SIM2 Barred Dial Menu.");
	ADD_APPLICATION_STRING2(STRING_MTPNP_CARD3_PINLOCK, "SIM3 Lock", "SIM2 Lock menu");
	ADD_APPLICATION_STRING2(STRING_MTPNP_CP_SIM3_PIN1, "SIM3 PIN", "SIM3 PIN menu");
	ADD_APPLICATION_STRING2(STRING_MTPNP_CP_SIM3_PIN2, "SIM4 PIN", "SIM4 PIN2 menu");
	ADD_APPLICATION_STRING2 (STRING_MTPNP_SIM3_SMS_MEMORY_EXCEEDED, "SIM3 phone Storage Full for New Incoming Message!",	"Memory Exceeded SIM1 Message SMS String");
	ADD_APPLICATION_STRING2(STR_STORAGE_LOCATION_SIM3, "To SIM3", "Storage location To SIM3");
	ADD_APPLICATION_STRING2(STRING_MTPNP_SIM3, "SIM3", "SIM3");
	ADD_APPLICATION_STRING2(STR_DELETE_ALL_SIM3 ,"From SIM3" ,"Menu item string for Delete All->SIM");
	ADD_APPLICATION_STRING2(STRING_MTPNP_SIM3_OWNER_NUMBER, "SIM3 owner number", "owner number");
	ADD_APPLICATION_STRING2(STRING_MTPNP_SIM3_SDL_NUMBER, "SIM3 SDL number", "SDL number");
	ADD_APPLICATION_STRING2(STRING_MTPNP_SIM3_SOS_NUMBER, "SIM3 SOS number", "SOS number");
	ADD_APPLICATION_STRING2(STRING_MTPNP_SIM3_VMAIL_MENUENTRY, "SIM3 Voice Mail Server", "Voice Mail menu item string");
	ADD_APPLICATION_STRING2(STRING_MTPNP_SMS_SIM3_DONE, "SIM3 Done", "SIM3 Done");
	ADD_APPLICATION_STRING2(STR_ID_PHB_OPTION_IP_DIAL3,"SIM3 IP Dial","Menu item string for View options->IP Dial");
	ADD_APPLICATION_STRING2(STRING_SIM3_IP_ACTIVATE, "Active SIM3", "Active Slave");
	ADD_APPLICATION_STRING2(STRING_SIM3_IP_DEACTIVATE, "Deactive Slave", "Deactive Slave");
	ADD_APPLICATION_STRING2(STR_SIM3_FULL_MSG,"SIM3 Storage Full!","SIM4 full message");
	#endif
	#if (MMI_SIM_NUMBER >=4)
	ADD_APPLICATION_STRING2(STRING_MTPNP_CARD4_CLOSED, "SIM4 Closed", "SIM4 Closed");
	ADD_APPLICATION_STRING(STRING_MTPNP_CARD4ABSENT, "Insert SIM4");
	ADD_APPLICATION_STRING(STRING_MTPNP_CARD4_SIM_BLOCK, "SIM4 Blocked");
	ADD_APPLICATION_STRING(STRING_MTPNP_CARD4_NETWORK_SETUP,"Card4 Network Setup");
	ADD_APPLICATION_STRING2(STRING_MTPNP_DM_CARD4_OPEN, "SIM4 open", "SIM4 open");
	ADD_APPLICATION_STRING2(STR_SIM4_FIXED_DAIL_CAPTION,"SIM4 Fixed Dial","SIM4 Fixed Dial Menu.");
	ADD_APPLICATION_STRING2(STR_SIM3_BARRED_DIAL_CAPTION,"SIM3 Barred Dial","SIM4 Barred Dial Menu.");
	ADD_APPLICATION_STRING2(STRING_MTPNP_CARD4_PINLOCK, "SIM4 Lock", "SIM4 Lock menu");
	ADD_APPLICATION_STRING2(STRING_MTPNP_CP_SIM4_PIN1, "SIM4 PIN", "SIM4 PIN menu");
	ADD_APPLICATION_STRING2(STRING_MTPNP_CP_SIM4_PIN2, "SIM4 PIN", "SIM4 PIN2 menu");
	ADD_APPLICATION_STRING2 (STRING_MTPNP_SIM4_SMS_MEMORY_EXCEEDED, "SIM4 phone Storage Full for New Incoming Message!",	"Memory Exceeded SIM1 Message SMS String");
	ADD_APPLICATION_STRING2(STR_STORAGE_LOCATION_SIM4, "To SIM4", "Storage location To SIM4");
	ADD_APPLICATION_STRING2(STRING_MTPNP_SIM4, "SIM4", "SIM4");
	ADD_APPLICATION_STRING2(STR_DELETE_ALL_SIM4 ,"From SIM4" ,"Menu item string for Delete All->SIM");
	ADD_APPLICATION_STRING2(STRING_MTPNP_SIM4_OWNER_NUMBER, "SIM4 owner number", "owner number");
	ADD_APPLICATION_STRING2(STRING_MTPNP_SIM4_SDL_NUMBER, "SIM4 SDL number", "SDL number");
	ADD_APPLICATION_STRING2(STRING_MTPNP_SIM4_SOS_NUMBER, "SIM4 SOS number", "SOS number");
	ADD_APPLICATION_STRING2(STRING_MTPNP_SIM4_VMAIL_MENUENTRY, "SIM4 Voice Mail Server", "Voice Mail menu item string");
	ADD_APPLICATION_STRING2(STRING_MTPNP_SMS_SIM4_DONE, "SIM4 Done", "SIM4 Done");
	ADD_APPLICATION_STRING2(STR_ID_PHB_OPTION_IP_DIAL4,"SIM4 IP Dial","Menu item string for View options->IP Dial");
	ADD_APPLICATION_STRING2(STRING_SIM4_IP_ACTIVATE, "Active SIM3", "Active Slave");
	ADD_APPLICATION_STRING2(STRING_SIM4_IP_DEACTIVATE, "Deactive SIM4", "Deactive Slave");
	ADD_APPLICATION_STRING2(STR_SIM4_FULL_MSG,"SIM4 Storage Full!","SIM4 full message");
	#endif
		
}

void Dualsim_ResPopulate(void)
{
    /* mercury string */
    Dualsim_StrPopulate();
    /* mercury string */

    // mercury image
#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
    ADD_APPLICATION_IMAGE2(IMG_STORAGE_SIM_1,
                           CUST_IMG_PATH "/MainLCD/Phonebook/PBView/PV_INSIM.pbm",
                           "IMG_STORAGE_SIM_1");
#else
    ADD_APPLICATION_IMAGE2(IMG_STORAGE_SIM_1,
                           CUST_IMG_PATH "/MainLCD/SubMenu/Services/SIMToolKit/SB_SIM1.gif",
                           "IMG_STORAGE_SIM_1");
#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
    ADD_APPLICATION_IMAGE2(IMG_STORAGE_SIM_2,
                           CUST_IMG_PATH "/MainLCD/SubMenu/Services/SIMToolKit/SB_SIM2.gif",
                           "IMG_STORAGE_SIM_2");
#ifdef __PRODUCT_QPAD__
    ADD_APPLICATION_IMAGE2(IMG_SI_MASTER_SIGNAL_STRENGTH,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/qpad/DualSIM/Master/SI_SIG1",
                           "Slave Status Icon: Signal strength");
    ADD_APPLICATION_IMAGE2(IMG_SI_MASTER_SIGNAL_STRENGTH_L1,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/qpad/DualSIM/Master/SI_SIG1_L1",
                           "Status Icon: Signal strength with L1");
    ADD_APPLICATION_IMAGE2(IMG_SI_MASTER_SIGNAL_STRENGTH_L2,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/qpad/DualSIM/Master/SI_SIG1_L2",
                           "Status Icon: Signal strength with L2");
    ADD_APPLICATION_IMAGE2(IMG_SI_MASTER_SIGNAL_CLOSED,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/qpad/DualSIM/Master/SI_SIG1_CLOSE/SI_SIG_1_CLOSE.gif",
                           "Slave Status Icon: rf closed");
    ADD_APPLICATION_IMAGE2(IMG_SI_SLAVE_SIGNAL_STRENGTH,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/qpad/DualSIM/Slave/SI_SIG2",
                           "Slave Status Icon: Signal strength");
    ADD_APPLICATION_IMAGE2(IMG_SI_SLAVE_SIGNAL_STRENGTH_L1,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/qpad/DualSIM/Slave/SI_SIG2_L1",
                           "Status Icon: Signal strength with L1");
    ADD_APPLICATION_IMAGE2(IMG_SI_SLAVE_SIGNAL_STRENGTH_L2,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/qpad/DualSIM/Slave/SI_SIG2_L2",
                           "Status Icon: Signal strength with L2");
    ADD_APPLICATION_IMAGE2(IMG_SI_SLAVE_SIGNAL_CLOSED,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/qpad/DualSIM/Slave/SI_SIG2_CLOSE/SI_SIG_2_CLOSE.gif",
                           "Slave Status Icon: rf closed");
#else
#ifdef MEM_ULC_3216
    ADD_APPLICATION_IMAGE2(IMG_SI_MASTER_SIGNAL_STRENGTH,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Master/SI_SIG1",
                           "Slave Status Icon: Signal strength");
    ADD_APPLICATION_IMAGE2(IMG_SI_MASTER_SIGNAL_STRENGTH_L1,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Master/SI_SIG1",
                           "Status Icon: Signal strength with L1");
    ADD_APPLICATION_IMAGE2(IMG_SI_MASTER_SIGNAL_STRENGTH_L2,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Master/SI_SIG1",
                           "Status Icon: Signal strength with L2");
    ADD_APPLICATION_IMAGE2(IMG_SI_MASTER_SIGNAL_CLOSED,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Master/SI_SIG1_CLOSE/SI_SIG_1_CLOSE.gif",
                           "Slave Status Icon: rf closed");

    ADD_APPLICATION_IMAGE2(IMG_SI_SLAVE_SIGNAL_STRENGTH,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Master/SI_SIG1",
                           "Slave Status Icon: Signal strength");
    ADD_APPLICATION_IMAGE2(IMG_SI_SLAVE_SIGNAL_STRENGTH_L1,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Master/SI_SIG1",
                           "Status Icon: Signal strength with L1");
    ADD_APPLICATION_IMAGE2(IMG_SI_SLAVE_SIGNAL_STRENGTH_L2,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Master/SI_SIG1",
                           "Status Icon: Signal strength with L2");
    ADD_APPLICATION_IMAGE2(IMG_SI_SLAVE_SIGNAL_CLOSED,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Slave/SI_SIG2_CLOSE/SI_SIG_2_CLOSE.gif",
                           "Slave Status Icon: rf closed");
#else
    ADD_APPLICATION_IMAGE2(IMG_SI_MASTER_SIGNAL_STRENGTH,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Master/SI_SIG1",
                           "Slave Status Icon: Signal strength");
    ADD_APPLICATION_IMAGE2(IMG_SI_MASTER_SIGNAL_STRENGTH_L1,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Master/SI_SIG1_L1",
                           "Status Icon: Signal strength with L1");
    ADD_APPLICATION_IMAGE2(IMG_SI_MASTER_SIGNAL_STRENGTH_L2,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Master/SI_SIG1_L2",
                           "Status Icon: Signal strength with L2");
    ADD_APPLICATION_IMAGE2(IMG_SI_MASTER_SIGNAL_CLOSED,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Master/SI_SIG1_CLOSE/SI_SIG_1_CLOSE.gif",
                           "Slave Status Icon: rf closed");

    ADD_APPLICATION_IMAGE2(IMG_SI_SLAVE_SIGNAL_STRENGTH,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Slave/SI_SIG2",
                           "Slave Status Icon: Signal strength");
    ADD_APPLICATION_IMAGE2(IMG_SI_SLAVE_SIGNAL_STRENGTH_L1,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Slave/SI_SIG2_L1",
                           "Status Icon: Signal strength with L1");
    ADD_APPLICATION_IMAGE2(IMG_SI_SLAVE_SIGNAL_STRENGTH_L2,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Slave/SI_SIG2_L2",
                           "Status Icon: Signal strength with L2");
    ADD_APPLICATION_IMAGE2(IMG_SI_SLAVE_SIGNAL_CLOSED,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Slave/SI_SIG2_CLOSE/SI_SIG_2_CLOSE.gif",
                           "Slave Status Icon: rf closed");
#endif
#endif
    
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_MASTER_NETWORK_CIPHER_GSM,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Callforward/Master/SI_Ncipher.gif",
//del cont.                           "Slave Status Icon: master gsm");
    
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_SLAVE_NETWORK_CIPHER_GSM,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Callforward/Slave/SI_Ncipher.gif",
//del cont.                           "Slave Status Icon: slave gsm");

//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_VOICE_L1_MASTER,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Master/SI_VOML1.JPG",
//del cont.                           "Master Status Icon: master unread voice L1");
    
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_VOICE_L2_MASTER,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Master/SI_VOML2.JPG",
//del cont.                           "Master Status Icon: master unread voice L2");
    
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_VOICE_L1L2_MASTER,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Master/SI_VOML12.JPG",
//del cont.                           "Master Status Icon: master unread voice L12");

//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_FAX_L1_MASTER,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Master/SI_FM1.JPG",
//del cont.                           "Master Status Icon: master unread fax L1");
    
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_FAX_L2_MASTER,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Master/SI_FM2.JPG",
//del cont.                           "Master Status Icon: master unread fax L2");
    
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_FAX_L1L2_MASTER,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Master/SI_FM12.JPG",
//del cont.                           "Master Status Icon: master unread fax L12");

//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_EMAIL_L1_MASTER,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Master/SI_EM1.JPG",
//del cont.                           "Master Status Icon: master unread email L1");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_EMAIL_L2_MASTER,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Master/SI_EM2.JPG",
//del cont.                           "Master Status Icon: master unread email L2");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_EMAIL_L1L2_MASTER,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Master/SI_EM12.JPG",
//del cont.                           "Master Status Icon: master unread email L12");

//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_NET_L1_MASTER,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Master/SI_NETL1.JPG",
//del cont.                           "Master Status Icon: master unread net L1");
    
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_NET_L2_MASTER,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Master/SI_NETL2.JPG",
//del cont.                           "Master Status Icon: master unread net L2");
    
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_NET_L1L2_MASTER,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Master/SI_NETL12.JPG",
//del cont.                           "Master Status Icon: master unread net L12");
    
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_VOICE_L1_SLAVE,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Slave/SI_VOML1.JPG",
//del cont.                           "Master Status Icon: slave unread voice L1");
    
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_VOICE_L2_SLAVE,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Slave/SI_VOML2.JPG",
//del cont.                           "Slave Status Icon: slave unread voice L2");
    
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_VOICE_L1L2_SLAVE,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Slave/SI_VOML12.JPG",
//del cont.                           "Slave Status Icon: slave unread voice L12");

//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_FAX_L1_SLAVE,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Slave/SI_FM1.JPG",
//del cont.                           "Slave Status Icon: slave unread fax L1");
    
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_FAX_L2_SLAVE,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Slave/SI_FM2.JPG",
//del cont.                           "Slave Status Icon: slave unread fax L2");
    
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_FAX_L1L2_SLAVE,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Slave/SI_FM12.JPG",
//del cont.                           "Slave Status Icon: slave unread fax L12");

//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_EMAIL_L1_SLAVE,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Slave/SI_EM1.JPG",
//del cont.                           "Slave Status Icon: slave unread email L1");
    
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_EMAIL_L2_SLAVE,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Slave/SI_EM2.JPG",
//del cont.                           "Slave Status Icon: slave unread email L2");
    
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_EMAIL_L1L2_SLAVE,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Slave/SI_EM12.JPG",
//del cont.                           "Slave Status Icon: slave unread email L12");

//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_NET_L1_SLAVE,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Slave/SI_NETL1.JPG",
//del cont.                           "Slave Status Icon: slave unread net L1");
    
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_NET_L2_SLAVE,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Slave/SI_NETL2.JPG",
//del cont.                           "Slave Status Icon: slave unread net L2");
    
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_UNREAD_NET_L1L2_SLAVE,
//del cont.                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/SMSwaitting/Slave/SI_NETL12.JPG",
//del cont.                           "Slave Status Icon: slave unread net L12");

    ADD_APPLICATION_IMAGE2(IMG_MTPNP_SETTING, CUST_IMG_PATH "/MainLCD/DualSIM/SETUP/DM_SETUP.gif",
                           "mercury setting");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_SLAVE_CALLSETTING,
//del cont.                           CUST_IMG_PATH "/MainLCD/DualSIM/SETUP/CALL_SETUP_2.gif",
//del cont.                           "Image for slave call setting");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_CARD2_PHONESETTING,
//del cont.                           CUST_IMG_PATH "/MainLCD/DualSIM/SETUP/PHONE_SETUP_2.gif",
//del cont.                           "Image for slave phone setting");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_SLAVE_NETWORKSETTING,
//del cont.                           CUST_IMG_PATH "/MainLCD/DualSIM/SETUP/NETWORK_SETUP_2.JPG",
//del cont.                           "Image for slave network setting");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_SLAVE_SECURITYSETTING,
//del cont.                           CUST_IMG_PATH "/MainLCD/DualSIM/SETUP/SECURITY_SETUP_2.gif",
//del cont.                           "Image for slave security setting");

    ADD_APPLICATION_IMAGE2(IMG_SLAVE_CALL_FORWARD, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Callforward/Slave/SI_FOR_2.gif",
                           "Status Icon: slave Call Divert");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SLAVE_CALL_FORWARD_L1, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Callforward/Slave/SI_FOR_L1_2.gif",
//del cont.                           "Status Icon: slave Call Divert L1");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SLAVE_CALL_FORWARD_L2, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Callforward/Slave/SI_FOR_L2_2.gif",
//del cont.                           "Status Icon: slave Call Divert L2");
    
    ADD_APPLICATION_IMAGE2(IMG_SI_SLAVE_ROAMING_INDICATOR, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Callforward/Slave/SI_RM_2.gif",
                           "Status Icon: Slave Roaming");

#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
	ADD_APPLICATION_IMAGE2(IMG_SI_MASTER_SMS_INDICATOR, CUST_IMG_PATH "/MainLCD/IdleScreen/StatusIcons/SI_MGE.BMP",
                           "Status Icon: MASTER Message");
#else
	ADD_APPLICATION_IMAGE2(IMG_SI_MASTER_SMS_INDICATOR, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Callforward/Master/SI_MGE_1.gif",
                           "Status Icon: MASTER Message");
#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
    ADD_APPLICATION_IMAGE2(IMG_SI_SLAVE_SMS_INDICATOR, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Callforward/Slave/SI_MGE_2.gif",
                           "Status Icon: SLAVE Message");

//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_MMS_UNREAD_INDICATOR_SIM1, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/MMS/Master/SI_UNREAD_1.gif",
//del cont.                           "Status Icon: Master unread mms");

//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_MMS_UNREAD_INDICATOR_SIM2, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/MMS/Slave/SI_UNREAD_2.gif",
//del cont.                           "Status Icon: Slave unread mms");

//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_MMS_BUSY_INDICATOR_SIM1, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/MMS/Master/SI_BUSY_1.gif",
//del cont.                           "Status Icon: Master unread mms");

//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_MMS_BUSY_INDICATOR_SIM2, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/MMS/Slave/SI_BUSY_2.gif",
//del cont.                           "Status Icon: Slave unread mms");

//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_EDGE_SIM1, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/GPRS/Master/SI_EDGE_SIM1.gif",
//del cont.                           "Status Icon: Master EDGE");
    
#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
    ADD_APPLICATION_IMAGE2(IMG_SI_GPRS_SERVICE_SIM1, CUST_IMG_PATH "/MainLCD/IdleScreen/StatusIcons/SI_GPRS.BMP",
                           "Status Icon: Master GPRS");
#else
    ADD_APPLICATION_IMAGE2(IMG_SI_GPRS_SERVICE_SIM1, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/GPRS/Master/SI_GPRS_SIM1.gif",
                           "Status Icon: Master GPRS");
#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
    
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_EDGEC_SIM1, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/GPRS/Master/SI_EDGEC_SIM1.gif",
//del cont.                           "Status Icon: Master EDGEC");
    
#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
    ADD_APPLICATION_IMAGE2(IMG_SI_GPRS_ATT_NO_PDP_INDICATOR_SIM1, CUST_IMG_PATH "/MainLCD/IdleScreen/StatusIcons/SI_GPRSC.BMP",
                           "Status Icon: Master GPRS_ATT_NO_PDP_INDICATOR");
#else
    ADD_APPLICATION_IMAGE2(IMG_SI_GPRS_ATT_NO_PDP_INDICATOR_SIM1, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/GPRS/Master/SI_GPRSC_SIM1.gif",
                           "Status Icon: Master GPRS_ATT_NO_PDP_INDICATOR");
#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */

//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_EDGE_SIM2, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/GPRS/Slave/SI_EDGE_SIM2.gif",
//del cont.                           "Status Icon: Slave EDGE");
    
//    ADD_APPLICATION_IMAGE2(IMG_SI_GPRS_SERVICE_SIM2, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/GPRS/Slave/SI_GPRS_SIM2.gif",
//                           "Status Icon: Slave GPRS");
    
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SI_EDGEC_SIM2, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/GPRS/Slave/SI_EDGEC_SIM2.gif",
//del cont.                           "Status Icon: Slave EDGEC");
    
//    ADD_APPLICATION_IMAGE2(IMG_SI_GPRS_ATT_NO_PDP_INDICATOR_SIM2, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/GPRS/Slave/SI_GPRSC_SIM2.gif",
//                           "Status Icon: Slave GPRS_ATT_NO_PDP_INDICATOR");

//del by reslim v0.1    ADD_APPLICATION_IMAGE2(MAIN_MENU_TITLE_DM_SETTINGS_ICON,
//del cont.                           CUST_IMG_PATH "/MainLCD/DualSIM/SETUP/DM_SETUP.gif", "mercury setting");
    ADD_APPLICATION_IMAGE2(IMG_SLAVE_LSW_LINE1, CUST_IMG_BASE_PATH "/EmptyImage.bmp", "Image for Line 1 option");
    ADD_APPLICATION_IMAGE2(IMG_SLAVE_LSW_LINE2, CUST_IMG_BASE_PATH "/EmptyImage.bmp", "Image for Line 2 option");

//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SLAVE_LINE_SET_TITLE, CUST_IMG_BASE_PATH "/EmptyImage.gif", "Image for Line 2 option");

//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_MASTER_MESSAGE_READ,
//del cont.                           CUST_IMG_PATH "/MainLCD/DualSIM/MESSAGE/CARD1/INBOX/SS_READ_1.gif",
//del cont.                           "read message");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_MASTER_MESSAGE_UNREAD,
//del cont.                           CUST_IMG_PATH "/MainLCD/DualSIM/MESSAGE/CARD1/INBOX/SS_UNREAD_1.gif",
//del cont.                           "unread message");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_MASTER_MESSAGE_SENT,
//del cont.                           CUST_IMG_PATH "/MainLCD/DualSIM/MESSAGE/CARD1/OUTBOX/SS_SENT_1.gif",
//del cont.                           "sent message");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_MASTER_MESSAGE_UNSENT,
//del cont.                           CUST_IMG_PATH "/MainLCD/DualSIM/MESSAGE/CARD1/OUTBOX/SS_UNSENT_1.gif",
//del cont.                           "unsent message");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_MASTER_MESSAGE_SS_NCOMP,
//del cont.                           CUST_IMG_PATH "/MainLCD/DualSIM/MESSAGE/CARD1/INBOX/SS_NCOMP_1.gif",
//del cont.                           "ncomp message");

//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SLAVE_MESSAGE_READ,
//del cont.                           CUST_IMG_PATH "/MainLCD/DualSIM/MESSAGE/CARD2/INBOX/SS_READ_2.gif",
//del cont.                           "read message");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SLAVE_MESSAGE_UNREAD,
//del cont.                           CUST_IMG_PATH "/MainLCD/DualSIM/MESSAGE/CARD2/INBOX/SS_UNREAD_2.gif",
//del cont.                           "unread message");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SLAVE_MESSAGE_SENT,
//del cont.                           CUST_IMG_PATH "/MainLCD/DualSIM/MESSAGE/CARD2/OUTBOX/SS_SENT_2.gif",
//del cont.                           "sent message");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SLAVE_MESSAGE_UNSENT,
//del cont.                           CUST_IMG_PATH "/MainLCD/DualSIM/MESSAGE/CARD2/OUTBOX/SS_UNSENT_2.gif",
//del cont.                           "unsent message");
//del by reslim v0.1    ADD_APPLICATION_IMAGE2(IMG_SLAVE_MESSAGE_SS_NCOMP,
//del cont.                           CUST_IMG_PATH "/MainLCD/DualSIM/MESSAGE/CARD2/INBOX/SS_NCOMP_2.gif",
//del cont.                           "ncomp message");


#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
    ADD_APPLICATION_IMAGE2(IMG_SI_CARD1MISSED_CALL_INDICATOR,
                            CUST_IMG_PATH"/MainLCD/IdleScreen/StatusIcons/SI_MCA.BMP",
                           "Status Icon: Missed Call");
#else
    ADD_APPLICATION_IMAGE2(IMG_SI_CARD1MISSED_CALL_INDICATOR,
                            CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/DualSIM/Callforward/Master/SI_MCA_1.gif",
                           "Status Icon: Missed Call");
#endif /* __MMI_DUAL_SIM_SINGLE_OPENED_ */
    
    ADD_APPLICATION_IMAGE2(IMG_SI_CARD2MISSED_CALL_INDICATOR,
                           CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Callforward/Slave/SI_MCA_2.gif",
                           "Status Icon: Missed Call");

    //ADD_APPLICATION_IMAGE2(IMG_ID_CARD1_PHB_STORAGE_SIM,CUST_IMG_PATH "/MainLCD/DualSIM/PHONEBOOK/PHB_CHOOSE_CARD1.bmp", "sim card entry");
    //ADD_APPLICATION_IMAGE2(IMG_ID_CARD2_PHB_STORAGE_SIM,CUST_IMG_PATH "/MainLCD/DualSIM/PHONEBOOK/PHB_CHOOSE_CARD2.bmp", "sim card entry");
    //ADD_APPLICATION_IMAGE2(IMG_ID_MTPNP_PHB_DELALL_FROM_SIM1,CUST_IMG_PATH "/MainLCD/DualSIM/PHONEBOOK/PHB_DEL_FROM1.bmp", "delete all entries form card1");
    //ADD_APPLICATION_IMAGE2(IMG_ID_MTPNP_PHB_DELALL_FROM_SIM2,CUST_IMG_PATH "/MainLCD/DualSIM/PHONEBOOK/PHB_DEL_FROM2.bmp","delete all entries form card2");
    //ADD_APPLICATION_IMAGE2(IMG_ID_MTPNP_PHB_COPYALL_FROM_SIM1TOPHONE,CUST_IMG_PATH "/MainLCD/DualSIM/PHONEBOOK/PHB_COPY_FROM1TOPHONE.bmp","copyall form sim1 to phone");
    //ADD_APPLICATION_IMAGE2(IMG_ID_MTPNP_PHB_COPYALL_FROM_PHONETOSIM1,CUST_IMG_PATH "/MainLCD/DualSIM/PHONEBOOK/PHB_COPY_FROMPHONETO1.bmp","copyall form phone to sim");
    //ADD_APPLICATION_IMAGE2(IMG_ID_MTPNP_PHB_COPYALL_FROM_PHONETOSIM2,CUST_IMG_PATH "/MainLCD/DualSIM/PHONEBOOK/PHB_COPY_FROMPHONETO2.bmp","copyall form sim1 to phone");
    //ADD_APPLICATION_IMAGE2(IMG_ID_MTPNP_PHB_COPYALL_FROM_SIM2TOPHONE,CUST_IMG_PATH "/MainLCD/DualSIM/PHONEBOOK/PHB_COPY_FROM2TOPHONE.bmp","copyall form phone to sim");
    //ADD_APPLICATION_IMAGE2(IMG_ID_MTPNP_PHB_COPYALL_FROM_SIM1TOSIM2,CUST_IMG_PATH "/MainLCD/DualSIM/PHONEBOOK/PHB_COPY_FROM1TO2.bmp","copyall form sim1 to sim2");
    //ADD_APPLICATION_IMAGE2(IMG_ID_MTPNP_PHB_COPYALL_FROM_SIM2TOSIM1,CUST_IMG_PATH "/MainLCD/DualSIM/PHONEBOOK/PHB_COPY_FROM2TO1.bmp","copyall form sim2 to sim1");
    ADD_APPLICATION_IMAGE2(IMG_DM_SWITCH_PROGRESS,CUST_IMG_PATH "/MainLCD/UIElement/Progress/PROGRESS.gif","copyall form sim2 to sim1");
/*
	ADD_APPLICATION_IMAGE2(IMG_DM_SWITCH_PROGRESS,
						   CUST_IMG_PATH "/mercury/active/ModeSwitch/MODESWITCHPROGRESS.gif",
                           "copyall form sim2 to sim1");
*/

    /* voice mail begin */
    ADD_APPLICATION_MENUITEM((MENU_MTPNP_DM_VMAIL,
                              MAIN_MENU_MESSAGES_MENUID, 
                              MMI_SIM_NUMBER,
                              MULTI_MENU(MESSAGES_MENU_VMAIL_MENUID),
                              SHOW, MOVEABLEWITHINPARENT, DISP_LIST, STR_VMAIL_MENUENTRY, IMG_SMS_VMAIL_MENUENTRY));
    /* voice mail end */

#if (MMI_SIM_NUMBER >= 3)
	ADD_APPLICATION_IMAGE2(IMG_SI_CARD3MISSED_CALL_INDICATOR,CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Callforward/Slave/SI_MCA_3.gif","Status Icon: Missed Call");
	ADD_APPLICATION_IMAGE2(IMG_STORAGE_SIM_3,CUST_IMG_PATH "/MainLCD/SubMenu/Services/SIMToolKit/SB_SIM3.gif","IMG_STORAGE_SIM_3");
	ADD_APPLICATION_IMAGE2(IMG_SI_CARD3_SIGNAL_CLOSED,CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Slave/SI_SIG2_CLOSE/SI_SIG_3_CLOSE.gif", "Slave Status Icon: rf closed");
	ADD_APPLICATION_IMAGE2(IMG_SI_SIM3_SMS_INDICATOR, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Callforward/Slave/SI_MGE_3.gif","Status Icon: SIM3 Message");
	ADD_APPLICATION_IMAGE2(IMG_SI_CARD3MISSED_CALL_INDICATOR, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Callforward/Slave/SI_MCA_3.gif", "Status Icon: Missed Call");
	ADD_APPLICATION_IMAGE2(IMG_MTPNP_VICON_3, CUST_IMG_PATH "/MainLCD/DualSIM/IPdial/IP_0.gif", "ip");
	ADD_APPLICATION_IMAGE2(IMG_MTPNP_VICON_13, CUST_IMG_PATH "/MainLCD/DualSIM/IPdial/IP_0.gif", "ip");
	ADD_APPLICATION_IMAGE2(IMG_MTPNP_VICON_23, CUST_IMG_PATH "/MainLCD/DualSIM/IPdial/IP_0.gif", "ip");
	ADD_APPLICATION_IMAGE2(IMG_MTPNP_VICON_123, CUST_IMG_PATH "/MainLCD/DualSIM/IPdial/IP_0.gif", "ip");
#endif
#if (MMI_SIM_NUMBER >= 4)
	ADD_APPLICATION_IMAGE2(IMG_STORAGE_SIM_4,CUST_IMG_PATH "/MainLCD/Phonebook/PBView/storage_sim_4.pbm","IMG_STORAGE_SIM_4");
	ADD_APPLICATION_IMAGE2(IMG_SI_CARD4MISSED_CALL_INDICATOR,CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Callforward/Slave/SI_MCA_4.BMP", "Status Icon: Missed Call");
	ADD_APPLICATION_IMAGE2(IMG_SI_CARD4_SIGNAL_CLOSED,CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Slave/SI_SIG2_CLOSE/SI_SIG_4_CLOSE.bmp", "Slave Status Icon: rf closed");
	ADD_APPLICATION_IMAGE2(IMG_SI_SIM4_SMS_INDICATOR, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Callforward/Slave/SI_MGE_4.bmp","Status Icon: SIM3 Message");
	ADD_APPLICATION_IMAGE2(IMG_SI_CARD4MISSED_CALL_INDICATOR, CUST_IMG_PATH "/MainLCD/IdleScreen/Statusicons/DualSIM/Callforward/Slave/SI_MCA_4.BMP", "Status Icon: Missed Call");
	ADD_APPLICATION_IMAGE2(IMG_MTPNP_VICON_4, CUST_IMG_PATH "/MainLCD/DualSIM/IPdial/IP_4.gif", "ip");
	ADD_APPLICATION_IMAGE2(IMG_MTPNP_VICON_14, CUST_IMG_PATH "/MainLCD/DualSIM/IPdial/IP_14.gif", "ip");
	ADD_APPLICATION_IMAGE2(IMG_MTPNP_VICON_24, CUST_IMG_PATH "/MainLCD/DualSIM/IPdial/IP_24.gif", "ip");
	ADD_APPLICATION_IMAGE2(IMG_MTPNP_VICON_34, CUST_IMG_PATH "/MainLCD/DualSIM/IPdial/IP_34.gif", "ip");
	ADD_APPLICATION_IMAGE2(IMG_MTPNP_VICON_124, CUST_IMG_PATH "/MainLCD/DualSIM/IPdial/IP_124.gif", "ip");
	ADD_APPLICATION_IMAGE2(IMG_MTPNP_VICON_134, CUST_IMG_PATH "/MainLCD/DualSIM/IPdial/IP_134.gif", "ip");
	ADD_APPLICATION_IMAGE2(IMG_MTPNP_VICON_234, CUST_IMG_PATH "/MainLCD/DualSIM/IPdial/IP_234.gif", "ip");
	ADD_APPLICATION_IMAGE2(IMG_MTPNP_VICON_1234, CUST_IMG_PATH "/MainLCD/DualSIM/IPdial/IP_1234.gif", "ip");
#endif

}
#endif

#endif

