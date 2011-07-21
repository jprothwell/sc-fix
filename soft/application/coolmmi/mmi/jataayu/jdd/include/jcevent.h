/***************************************************************************
 * $Id: jcevent.h,v 1.5 2007/03/20 06:20:58 browserdevjtbase Exp $
 * $Revision: 1.5 $
 * $DateTime: $
 * 
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software (P) Ltd., Bangalore ("Jataayu") 
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
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY 
 * KIND AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
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
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: jcevent.h,v $
 * Revision 1.5  2007/03/20 06:20:58  browserdevjtbase
 * Update to comments
 *
 * Revision 1.4  2006/11/16 11:32:26  browserdevjtbase
 * Update
 *
 * Revision 1.3  2006/06/20 13:28:37  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.4  2006/01/25 07:22:16  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.3  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.2  2005/03/24 10:57:50  kumardevhtmlbrow
 * Changed as per the coding standard
 *
 *  
 ***************************************************************************/


#ifndef _JCEVENT_H
#define _JCEVENT_H

/** 
 * @file jcevent.h
 * @ingroup Queue
 * @brief Defines the datatype's that are used in the Queue.
 *
 */

/** @cond */

#define JC_USER_APP_TASK_ID			0

typedef enum e_semaphore_index
{
	E_SEM_HISTORY,
	E_SEM_CONTROLDATA,
	E_SEM_TOTAL
} ESemaphoreID ;

typedef enum e_task_index
{
	E_TASK_BROWSER_CORE = JC_USER_APP_TASK_ID,
	E_TASK_BROWSER_UI,

	E_TASK_MMS_CORE,
	E_TASK_MMS_UI,
	
	E_TASK_WAP_PROVISIONING_CORE,
	E_TASK_WAP_PROVISIONING_UI,

	E_TASK_SYNCML_CORE,
	E_TASK_SYNCML_UI,
	
	E_TASK_EMAIL_CORE,
	E_TASK_EMAIL_UI,
	E_TASK_TRANSPORT,
	E_TASK_USERAPP
} ETaskID ;

/** @endcond */

/** 
 * @brief This structure specifies the task information.
 * 
 * Depending on the environment, each application can have different task. 
 * In this case, iAppID have no impact, where as a single task includes more than one 
 * application then the event is passed on to the application based on iAppID.
 */
typedef struct st_jc_task_info
{
	JC_INT32			iTaskID ; /**< Specifies the task identifier */
	JC_INT32			iAppID ; /**< Specifies the application identifier in that task. */
} JC_TASK_INFO ;

/**
 * @brief This structure contains information about the event that needs to be sent.
 * 
 */
typedef struct st_jc_event_data
{
	JC_TASK_INFO		srcTaskInfo ; /**< Specifies the source task information. */
	JC_TASK_INFO		destTaskInfo ; /**< Specifies the destination task information. */
	JC_INT32			iEventType ; /**< Specifies the event type. */
	JC_UINT32			uiEventInfoSize ; /**< Specifies the size of the event data. */
	void				*pEventInfo ; /**< Specifies the event data which needs to be transmitted.*/
} JC_EVENT_DATA ;


#endif

/* END OF FILE */


