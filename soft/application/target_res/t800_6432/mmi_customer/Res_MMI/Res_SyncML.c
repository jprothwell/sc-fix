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
 *    Res_SyncML.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *    This file populates resource for SyncML appliction.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 
 *============================================================================== 
 *******************************************************************************/

#ifndef _MMI_RES_SYNCML_C
#define _MMI_RES_SYNCML_C
#include "stdc.h"
#include "mmi_features.h"

#ifdef __SYNCML_SUPPORT__


#include "customcfg.h"

#ifdef FIRST_PASS
#include "buildcfg.h"
#endif

#include "custom_mmi_default_value.h"
#include "syncmldef.h"

#ifdef DEVELOPER_BUILD_FIRST_PASS
#include "populateres.h"


/*****************************************************************************
 * FUNCTION
 *  SyncMLResourceData
 * DESCRIPTION
 *  Populate all the resources required by SyncML application
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void SyncMLResourceData(void)
{
    ADD_APPLICATION_MENUITEM((MENU_ID_SYNCML_MAIN,
                              MAIN_MENU_SERVICES_MENUID,
                              2,
                              MENU_ID_SYNCML_SYNC,
                              MENU_ID_SYNCML_SETTINGS,
                              SHOW, NONMOVEABLE, DISP_LIST,
                              STR_ID_SYNC_MENU, IMG_ID_SYNC_MENU));

    ADD_APPLICATION_MENUITEM((MENU_ID_SYNCML_SYNC,
                              MENU_ID_SYNCML_MAIN,
                              0,
                              SHOW, NONMOVEABLE, DISP_LIST,
                              STR_ID_SYNC_NOW, IMG_ID_SYNC_MENU));

    ADD_APPLICATION_MENUITEM((MENU_ID_SYNCML_SETTINGS,
                              MENU_ID_SYNCML_MAIN,
                          #ifdef __MMI_SYNCML_REFRESH_SYNC_SUPPORT__
                              5,
                          #else
                              4,
                          #endif
                              MENU_ID_SYNCML_PROFILE,
                              MENU_ID_SYNCML_APPLICATIONS,
                          #ifdef __MMI_SYNCML_REFRESH_SYNC_SUPPORT__
                              MENU_ID_SYNCML_TYPE,
                          #endif
                              MENU_ID_SYNCML_REGULARITY,
                              MENU_ID_SYNCML_REPORT,
                              SHOW, NONMOVEABLE, DISP_LIST,
                              STR_ID_SYNC_SETTINGS, IMG_ID_SYNC_SETTINGS));

    ADD_APPLICATION_MENUITEM((MENU_ID_SYNCML_PROFILE,
                              MENU_ID_SYNCML_SETTINGS,
                              0,
                              SHOW, NONMOVEABLE, DISP_LIST,
                              STR_ID_SYNC_PROFILE, 0));

    ADD_APPLICATION_MENUITEM((MENU_ID_SYNCML_APPLICATIONS,
                              MENU_ID_SYNCML_SETTINGS,
                              0,
                              SHOW, NONMOVEABLE, DISP_LIST,
                              STR_ID_SYNC_APPLICATIONS, 0));

#ifdef __MMI_SYNCML_REFRESH_SYNC_SUPPORT__
    ADD_APPLICATION_MENUITEM((MENU_ID_SYNCML_TYPE,
                              MENU_ID_SYNCML_SETTINGS,
                              0,
                              SHOW, NONMOVEABLE, DISP_LIST,
                              STR_ID_SYNC_TYPE, 0));
#endif /* __MMI_SYNCML_REFRESH_SYNC_SUPPORT__ */

    ADD_APPLICATION_MENUITEM((MENU_ID_SYNCML_REGULARITY,
                              MENU_ID_SYNCML_SETTINGS,
                              0,
                              SHOW, NONMOVEABLE, DISP_LIST,
                              STR_ID_SYNC_REGULARITY, 0));

    ADD_APPLICATION_MENUITEM((MENU_ID_SYNCML_REPORT,
                              MENU_ID_SYNCML_SETTINGS,
                              0,
                              SHOW, NONMOVEABLE, DISP_LIST,
                              STR_ID_SYNC_REPORT, 0));

    ADD_APPLICATION_MENUITEM((MENU_ID_SYNCML_PROFILE_OPTIONS,
                              0,
                              2,
                              MENU_ID_SYNCML_PROFILE_ACTIVATE,
                              MENU_ID_SYNCML_PROFILE_EDIT,
                              SHOW, NONMOVEABLE, DISP_LIST,
                              STR_GLOBAL_OPTIONS, 0));

    ADD_APPLICATION_MENUITEM((MENU_ID_SYNCML_PROFILE_ACTIVATE,
                              MENU_ID_SYNCML_PROFILE_OPTIONS,
                              0,
                              SHOW, NONMOVEABLE, DISP_LIST,
                              STR_GLOBAL_ACTIVATE, 0));

    ADD_APPLICATION_MENUITEM((MENU_ID_SYNCML_PROFILE_EDIT,
                              MENU_ID_SYNCML_PROFILE_OPTIONS,
                              5,
                              MENU_ID_SYNCML_PROFILE_EDIT_NAME,
                              MENU_ID_SYNCML_PROFILE_EDIT_SERVER,
                              MENU_ID_SYNCML_PROFILE_EDIT_CONTACT,
                              MENU_ID_SYNCML_PROFILE_EDIT_CALENDAR,
                              MENU_ID_SYNCML_PROFILE_EDIT_CONNECTION,
                              SHOW, NONMOVEABLE, DISP_LIST,
                              STR_GLOBAL_EDIT, 0));

    ADD_APPLICATION_MENUITEM((MENU_ID_SYNCML_PROFILE_EDIT_NAME,
                              MENU_ID_SYNCML_PROFILE_EDIT,
                              0,
                              SHOW, NONMOVEABLE, DISP_LIST,
                              STR_ID_SYNC_PROFILE_NAME, IMG_ID_SYNC_PROFILE));

    ADD_APPLICATION_MENUITEM((MENU_ID_SYNCML_PROFILE_EDIT_SERVER,
                              MENU_ID_SYNCML_PROFILE_EDIT,
                              0,
                              SHOW, NONMOVEABLE, DISP_LIST,
                              STR_ID_SYNC_PROFILE_SERVER, IMG_ID_SYNC_SERVER));

    ADD_APPLICATION_MENUITEM((MENU_ID_SYNCML_PROFILE_EDIT_CONTACT,
                              MENU_ID_SYNCML_PROFILE_EDIT,
                              0,
                              SHOW, NONMOVEABLE, DISP_LIST,
                              STR_ID_SYNC_PROFILE_CONTACT, IMG_ID_SYNC_CONTACT));

    ADD_APPLICATION_MENUITEM((MENU_ID_SYNCML_PROFILE_EDIT_CALENDAR,
                              MENU_ID_SYNCML_PROFILE_EDIT,
                              0,
                              SHOW, NONMOVEABLE, DISP_LIST,
                              STR_ID_SYNC_PROFILE_CALENDAR, IMG_ID_SYNC_CALENDAR));

    ADD_APPLICATION_MENUITEM((MENU_ID_SYNCML_PROFILE_EDIT_CONNECTION,
                              MENU_ID_SYNCML_PROFILE_EDIT,
                              0,
                              SHOW, NONMOVEABLE, DISP_LIST,
                              STR_ID_SYNC_PROFILE_CONNECTION, IMG_ID_SYNC_CONNECTION));

    ADD_APPLICATION_MENUITEM((MENU_ID_SYNCML_PROFILE_EDIT_NAME_OPTION,
                              0,
                              2,
                              MENU_ID_SYNCML_PROFILE_EDIT_NAME_DONE,
                              MENU_ID_SYNCML_PROFILE_EDIT_NAME_INPUT,
                              SHOW, NONMOVEABLE, DISP_LIST,
                              STR_GLOBAL_OPTIONS, 0));

    ADD_APPLICATION_MENUITEM((MENU_ID_SYNCML_PROFILE_EDIT_NAME_DONE,
                              MENU_ID_SYNCML_PROFILE_EDIT_NAME_OPTION,
                              0,
                              SHOW, NONMOVEABLE, DISP_LIST,
                              STR_GLOBAL_DONE, 0));

    ADD_APPLICATION_MENUITEM((MENU_ID_SYNCML_PROFILE_EDIT_NAME_INPUT,
                              MENU_ID_SYNCML_PROFILE_EDIT_NAME_OPTION,
                              0,
                              SHOW, NONMOVEABLE, DISP_LIST,
                              STR_GLOBAL_INPUT_METHOD, 0));

    ADD_APPLICATION_STRING2(STR_ID_SYNC_MENU,              "Synchronization",           "synchronization");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_MENU_SHORT_FORM,   "Sync.",                     "synchronization");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_NOW,               "Sync Now",                  "menu item: sync now");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_SETTINGS,          "Sync Settings",             "menu item: sync settings");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_PROGRESS,          "Synchronizing...",          "synchronizing...");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_NO_PROFILE,        "No profile!",               "no profile!");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_EDIT_QUREY,        "Edit profile now?",         "edit profile now?");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_PROFILE,           "Profile",                   "menu item: profile");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_APPLICATIONS,      "Applications to Sync",      "menu item: applications to sync");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_REGULARITY,        "Regular Sync",              "menu item: regular sync");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_PROFILE_NAME,      "Profile Name",              "menu item: profile name");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_PROFILE_SERVER,    "Server Settings",           "menu item: server settings");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_PROFILE_CONTACT,   "Contact Database Settings", "menu item: contact database settings");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_PROFILE_CALENDAR,  "Calendar Database Settings","menu item: calendar database settings");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_PROFILE_CONNECTION,"Connection Settings",       "menu item: connection settings");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_APP_PHB,           "Phonebook",                 "selection: phonebook");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_APP_TDL,           "To do list",                "selection: to do list");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_APP_EMAIL,         "Email",                     "selection: email");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_REGULARITY_NONE,   "None",                      "selection: none");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_REGULARITY_DATA,   "When data is changed",      "selection: when data is changed");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_REGULARITY_POWERON,"When handset is turned on", "selection: when handset is turned on");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_REGULARITY_DAY,    "Every day",                 "selection: every day");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_REGULARITY_WEEK,   "Every week",                "selection: every week");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_REGULARITY_MONTH,  "Every month",               "selection: every month");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_SERVER_ADDRESS,    "Server address",            "Server address");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_CONTACT_ADDRESS,   "Contact database address",  "Contact database address");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_CALENDAR_ADDRESS,  "Calendar database address", "Calendar database address");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_USERNAME,          "Username",                  "Username");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_PASSWORD,          "Password",                  "Password");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_BEARER,            "Local bearer",              "Local bearer");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_BEARER_USB,        "Data cable",                "selection: data cable");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_BEARER_BT,         "Bluetooth",                 "selection: bluetooth");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_BEARER_IRDA,       "Infrared",                  "selection: irda");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_TYPE,              "Sync Type",                 "menu item: sync type");

    ADD_APPLICATION_STRING2(STR_ID_SYNC_HINT_PHB_ONLY,     "Phonebook only",            "hint: Phonebook only");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_HINT_TDL_ONLY,     "To-do-list only",           "hint: To-do-list only");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_HINT_PHB_TDL,      "Phonebook and to-do-list",  "hint: Phonebook and to-do-list");

    ADD_APPLICATION_STRING2(STR_ID_SYNC_PROXY_USAGE,        "Use Proxy",                "proxy");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_PROXY_SERVER,       "Proxy IP",                 "proxy");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_PROXY_PORT,         "Proxy Port",               "proxy");

    ADD_APPLICATION_STRING2(STR_ID_SYNC_PHB_BUSY,           "Phonebook busy now!",      "warning: phonebook busy");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_INCOMPLETE_PROFILE, "Incomplete profile!",      "warning: incomplete profile");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_NO_APPLICATION,     "No application!",          "no application!");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_BACKGROUND_QUERY,   "Do you want to abort the sync?\n Press Yes to abort or press No to keep the sync in the background.", "background sync");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_PLEASE_WAIT,        "Sync now,\nplease wait...","background sync");

    ADD_APPLICATION_STRING2(STR_ID_SYNC_INVALID_CREDENTIAL, "Invalid credentials!",     "Invalid credentials!");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_MISSING_CREDENTIAL, "Missing credentials!",     "Missing credentials!");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_PAYMENT_REQUIRED,   "Payment Required!",        "Payment Required!");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_DONE,               "Sync Done",                "Sync done");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_FAIL,               "Sync Unfinished",          "Sync Unfinished");

    ADD_APPLICATION_STRING2(STR_ID_SYNC_ALARM_EXPIRED_QUERY,"Sync alarm expired!\nDo you want start sync?", "sync regularity");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_POWER_ON_QUERY,     "Do you want start sync for this power on?",    "sync regularity");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_DATA_CHANGED_QUERY, "Data was changed!\nDo you want start sync?",   "sync regularity");

    ADD_APPLICATION_STRING2(STR_ID_PROGRESS_LINK_ESTABLISHED,           "Connected",             "Progression info");
    ADD_APPLICATION_STRING2(STR_ID_PROGRESS_LINK_DISCONNECTED,          "Disconnected",          "Progression info");
    ADD_APPLICATION_STRING2(STR_ID_PROGRESS_RX_FAILED,                  "Receiving failed",      "Progression info");
    ADD_APPLICATION_STRING2(STR_ID_PROGRESS_TX_FAILED,                  "Sending failed",        "Progression info");
    ADD_APPLICATION_STRING2(STR_ID_PROGRESS_TASK_DATASTORE_SENDING,     "Sending to-do-list",    "Progression info");
    ADD_APPLICATION_STRING2(STR_ID_PROGRESS_CONTACT_DATASTORE_SENDING,  "Sending phonebook",     "Progression info");
    ADD_APPLICATION_STRING2(STR_ID_PROGRESS_TASK_DATASTORE_RECEIVED,    "Receiving to-do-list",  "Progression info");
    ADD_APPLICATION_STRING2(STR_ID_PROGRESS_CONTACT_DATASTORE_RECEIVED, "Receiving phonebook",   "Progression info");
    ADD_APPLICATION_STRING2(STR_ID_PROGRESS_TASK_DATASTORE_DONE,        "To-do-list sync done",  "Progression info");
    ADD_APPLICATION_STRING2(STR_ID_PROGRESS_CONTACT_DATASTORE_DONE,     "Phonebook sync done",   "Progression info");
    ADD_APPLICATION_STRING2(STR_ID_PROGRESS_TASK_DATASTORE_FAILED,      "To-do-list sync failed","Progression info");
    ADD_APPLICATION_STRING2(STR_ID_PROGRESS_CONTACT_DATASTORE_FAILED,   "Phonebook sync failed", "Progression info");
    ADD_APPLICATION_STRING2(STR_ID_PROGRESS_LINK_REDIRECTED,            "URL redirected",        "Progression info");
    ADD_APPLICATION_STRING2(STR_ID_PROGRESS_SESSION_DONE,               "Sync session done",     "Progression info");
    ADD_APPLICATION_STRING2(STR_ID_PROGRESS_TASK_DATASTORE_UPDATING,    "Updating to-do-list",   "Progression info");
    ADD_APPLICATION_STRING2(STR_ID_PROGRESS_CONTACT_DATASTORE_UPDATING, "Updating phonebook",    "Progression info");

    ADD_APPLICATION_STRING2(STR_ID_SYNC_TWOWAY,             "Two-way Sync",                      "sync type: two way");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_BACKUP,             "Backup to server",                  "sync type: backup");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_RESTORE,            "Restore from server",               "sync type: restore");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_BACKUP_WARNING,     "All remote records will be deleted before sync! Continue?", "sync type: backup");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_RESTORE_WARNING,    "All local records will be deleted before sync! Continue?", "sync type: restore");

    ADD_APPLICATION_STRING2(STR_ID_SYNC_REPORT,             "Sync Report",                       "Sync Report");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_REPORT_CON_OK,      "Contact done",                      "Sync Report");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_REPORT_CAL_OK,      "Calendar done",                     "Sync Report");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_REPORT_CON_NG,      "Contact unfinished",                "Sync Report");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_REPORT_CAL_NG,      "Calendar unfinished",               "Sync Report");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_REPORT_CON_CK,      "Please check contact settings",     "Sync Report");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_REPORT_CAL_CK,      "Please check calendar settings",    "Sync Report");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_REPORT_CON_NO,      "Contact not sync",                  "Sync Report");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_REPORT_CAL_NO,      "Calendar not sync",                 "Sync Report");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_REPORT_REPLACE,     "Replace",                           "Sync Report");

#ifdef __MMI_WLAN_FEATURES__
    ADD_APPLICATION_STRING2(STR_ID_SYNC_INVALID_DATA_ACCOUNT, "Invalid data account!", "Invalid data account!");
#endif

    ADD_APPLICATION_STRING2(STR_ID_SYNC_PROFILE_ID1,        "Profile 1",                "Profile");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_PROFILE_ID2,        "Profile 2",                "Profile");
    ADD_APPLICATION_STRING2(STR_ID_SYNC_PROFILE_ID3,        "Profile 3",                "Profile");

    ADD_APPLICATION_IMAGE2(IMG_ID_SYNC_MENU,       CUST_IMG_PATH"/MainLCD/Submenu/Services/SyncML/SML_Sync.pbm",     "Sync image");
    ADD_APPLICATION_IMAGE2(IMG_ID_SYNC_NOW,        CUST_IMG_PATH"/MainLCD/Submenu/Services/SyncML/Synchronzing.gif", "Sync image");
    ADD_APPLICATION_IMAGE2(IMG_ID_SYNC_SETTINGS,   CUST_IMG_PATH"/MainLCD/Submenu/Services/SyncML/SML_SyncSet.pbm",  "Sync image");

    ADD_APPLICATION_IMAGE2(IMG_ID_SYNC_CONTACT,    CUST_IMG_PATH"/MainLCD/Phonebook/SB_ADD.PBM",  "Shared from Phonebook");
    ADD_APPLICATION_IMAGE2(IMG_ID_SYNC_CALENDAR,   CUST_IMG_PATH"/MainLCD/ToDoList/SB_OLIST.PBM", "Shared from To Do List");
    ADD_APPLICATION_IMAGE2(IMG_ID_SYNC_PROFILE,    CUST_IMG_PATH"/MainLCD/Submenu/SERVICES/DataAccount/SV_ACCN.PBM", "Shared from GPRS Data Account");
    ADD_APPLICATION_IMAGE2(IMG_ID_SYNC_SERVER,     CUST_IMG_PATH"/MainLCD/Submenu/SERVICES/DataAccount/SV_TYPE.PBM", "Shared from GPRS Data Account");
    ADD_APPLICATION_IMAGE2(IMG_ID_SYNC_CONNECTION, CUST_IMG_PATH"/MainLCD/submenu/SERVICES/DataAccount/SB_DATA.PBM", "Shared from GPRS Data Account");
    ADD_APPLICATION_IMAGE2(IMG_ID_SYNC_USERNAME,   CUST_IMG_PATH"/MainLCD/Submenu/SERVICES/DataAccount/SV_NAME.PBM", "Shared from GPRS Data Account");
    ADD_APPLICATION_IMAGE2(IMG_ID_SYNC_PASSWORD,   CUST_IMG_PATH"/MainLCD/Submenu/SERVICES/DataAccount/SV_PASS.PBM", "Shared from GPRS Data Account");
}

#endif /* DEVELOPER_BUILD_FIRST_PASS */
#endif /* __SYNCML_SUPPORT__ */
#endif /* _MMI_RES_SYNCML_C */
