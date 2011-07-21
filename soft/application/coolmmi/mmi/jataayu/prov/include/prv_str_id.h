/***************************************************************************
 *
 * File Name : jmms_str_id.h
 *
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software Ltd., Bangalore ("Jataayu") 
 * or Jataayu's suppliers as the case may be. The Software is protected by 
 * copyright, including without limitation by applicable copyright laws, 
 * international treaty provisions, other intellectual property laws and 
 * applicable laws in the country in which the Software is being used. 
 * You shall not modify, adapt or translate the Software, without prior 
 * express written consent from Jataayu. You shall not reverse engineer, 
 * decompile, disassemble or otherwise alter the Software, except and 
 * only to the extent that such activity is expressly permitted by 
 * applicable law notwithstanding this limitation. Unauthorized reproduction 
 * or redistribution of this program or any portion of it may result in severe 
 * civil and criminal penalties and will be prosecuted to the maximum extent 
 * possible under the law. Jataayu reserves all rights not expressly granted. 
 * 
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY KIND 
 * AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY ACCURACY OF INFORMATIONAL CONTENT, AND/OR FITNESS 
 * FOR A PARTICULAR PURPOSE OR USE, TITLE OR INFRINGEMENT ARE EXPRESSLY 
 * DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. YOU ASSUME THE 
 * ENTIRE RISK AS TO THE ACCURACY AND THE USE OF THIS SOFTWARE. JATAAYU 
 * SHALL NOT BE LIABLE FOR ANY CONSEQUENTIAL, INCIDENTAL, INDIRECT, 
 * EXEMPLARY, SPECIAL OR PUNITIVE DAMAGES INCLUDING WITHOUT LIMITATION 
 * ANY LOSS OF DATA, OR; LOSS OF PROFIT, SAVINGS BUSINESS OR GOODWILL 
 * OR OTHER SIMILAR LOSS RESULTING FROM OR OUT OF THE USE OR INABILITY 
 * TO USE THIS SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 *
 ***************************************************************************/
 
 /** 
 * @file		prv_str_id.h
 * @ingroup		provisioning
 * @brief		Describes the String ID API's.
 *
 **/ 
#include "mmi_data_types.h"


#ifndef __JPROV_STR_ID
#define __JPROV_STR_ID

/* MMS String ID */

enum {
	STR_ID_PROV_NEWPROV_ALERT	= JAT_MMS_BASE + 300,
	STR_ID_PROV_ENTER_PIN,
	STR_ID_PROV_DISCARD_CONFIRMATION,
	STR_ID_PROV_DISCARD_SUCC_MSG,
	STR_ID_PROV_SAVE_CONFIRMATION,
	STR_ID_PROV_SAVE_SUCC_MSG,
	STR_ID_PROV_FAIL_MSG,
	STR_ID_PROV_NET_ERROR,
	STR_ID_PROV_DOWNLOAD,
	STR_ID_PROV_TITLE,
	STR_ID_PROV_DISCARD,
	STR_ID_PROV_BROW_PROFILE,
	STR_ID_PROV_MMS_PROFILE,
	STR_ID_PROV_INAVLID_PIN_TRYAGAIN,
	STR_ID_PROV_ERR_UNKNOWN,
	STR_ID_PROV_USEPIN_AUTH_FAIL,
	STR_ID_PROV_INSTALL,
	STR_ID_PROV_INSTALL_SETTINGS,
	STR_ID_PROV_NEW_SETTINGS,
	STR_ID_PROV_SUMMARY_REPORT,
	STR_ID_PROV_DATA_ACCOUNT,
	STR_ID_PROV_INVALID_SETTING,
	STR_ID_PROV_REPLACE_PROFILE,
	STR_ID_PROV_SETTINGSFOR,
	STR_ID_PROV_DOWNLOAD_SETTINGS,
	STR_ID_PROV_PROFILE_NAME,
	STR_ID_PROV_PROFILE_ID,
	STR_ID_PROV_USERNAME,
	STR_ID_PROV_PASSWORD,
	STR_ID_PROV_PROXYIP,
	STR_ID_PROV_PORT,
	STR_ID_PROV_NAP_ID,
	STR_ID_PROV_HOMEPAGE,
	STR_ID_PROV_MMSC_URL,
	STR_ID_PROV_DNS1,
	STR_ID_PROV_DNS2,
	STR_ID_PROV_APN,
	STR_ID_PROV_REPLACE_CONFIRMATION
};


enum {
	SCR_ID_PROV_NEW_ALERT_SCREEN = JAT_MMS_BASE + 300,
	SCR_ID_PROV_DETAILS_SCREEN,
	SCR_ID_PROV_DISCARD_CONFIRM_SCREEN,
	SCR_ID_PROV_DISCARD_SUCCESS_SCREEN,
	SCR_ID_CONTPROV_DISCARD_CONFIRM_SCREEN,
	SCR_ID_PROV_SAVE_CONFIRM_SCREEN,
	SCR_ID_PROV_SAVE_SUCCESS_SCREEN,
	SCR_ID_PROV_SAVE_INVALID_SCREEN,
	SCR_ID_PROV_FAIL_SCREEN,
	SCR_ID_PROV_USERPIN_SCREEN,
	SCR_ID_PROV_USERPIN_OPTIONS_SCREEN,
	SCR_ID_PROV_PROVDISCARD_CONFIRM_SCREEN,
	SCR_ID_PROV_NEWSETTINGS_SUMMARY_SCREEN,
	SCR_ID_PROV_CONT_DETAILS_SCREEN,
	SCR_ID_PROV_SHOW_PROXYPROFLIST_SCREEN,
	SCR_ID_PROV_SHOW_DATACONNLIST_SCREEN,
	SCR_ID_PROV_REPLACE_CONFIRM_SCREEN,
	SCR_ID_PROF_DISCARD_CONFIRM_SCREEN,
	SCR_ID_PROV_END
};

enum
{
	IMG_PROV_MAIN_ICON = JAT_MMS_BASE  + 300,

};
#endif //__JPROV_STR_ID

