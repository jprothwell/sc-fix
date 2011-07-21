 /***************************************************************************
 *
 * File Name : jmms_init.h
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
 * @file		jmms_init.h
 * @ingroup		jMMS
 * @brief		
 *
 **/ 
#ifndef __JMMS_INIT
#define __JMMS_INIT

 /***************************************************************************
 * System Include Files
 **************************************************************************/

 /***************************************************************************
 * User Include Files
 **************************************************************************/
#include <jwap_mms_common.h>
/***************************************************************************
* Macros Definitions
**************************************************************************/


#define __JATAAYU_APP__ //?

#ifdef __JATAAYU_APP__

/***************************************************************************
 * Global variables declarations
 **************************************************************************/
#define	INBOX_FOLDER	"Inbox"
#define	SENT_FOLDER	    "Sent"
#define	OUTBOX_FOLDER	"Outbox"
#define	DRAFT_FOLDER	"Draft"

/*use by MMI_FS_GetDirSize*/
#define JMMS_DEFAULT_STORAGE_PATH "c:/jmms/"

 
#define CS_MMS_MAX_SPACE (400*1024)

/*	This is the configuration for the minimum space (in Bytes) to be available always in
	the jmms folder for it to work properly ,for mms templete use*/
#define	CS_FS_THRESHOLD_LIMIT	(50 * 1024)

/***************************************************************************
* Local Function Prototypes
***************************************************************************/

void initJMms () ;
void jMMS_processAppEvent(JC_EVENT_DATA *pEventData);
void jMMS_SetMMSAppStatus(JC_BOOLEAN bIsActive);
JC_BOOLEAN jMMS_GetMMSAppStatus();
void  jMMS_HandlePresenatationEvnts(void* pvEventInfo);
void jMms_SetMmsViewTitle();

#endif // __JATAAYU_APP__

#endif // __JMMS_INIT
