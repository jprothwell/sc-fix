/***************************************************************************
 * $Id: jdd_taskapi.h,v 1.2 2006/05/02 11:19:21 kumardevjtbase Exp $
 * $Revision: 1.2 $
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
 * $Log: jdd_taskapi.h,v $
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.5  2006/01/25 07:22:16  kumardevhtmlbrow
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


#ifndef _JDD_TASK_H
#define _JDD_TASK_H

/** 
 * @file jdd_taskapi.h
 * @ingroup Task
 * @brief Describes the basic task API calls.
 *
 */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief This function serves as the task entry point for the created task. 
 * The execution begins from this routine for the created task.
 */
typedef void (*TaskEntryHandler) (unsigned long nParams, void *	pParams) ;

/**
 * @brief This pointer is used to store the handle obtained during the creation of a task.
 */
typedef void *	JDD_TASK ;

/**
 * @brief This function creates a task.
 * @param[out] pvTask Returns the reference to the task created.
 * @param[in] pcTaskName Specifies the name associated to the task.
 * @param[in] uhTaskSlice Indicates the maximum amount of time in milli seconds that can expire while executing this task.
 * @param[in] uhStackSize Specifies the stack size that needs to be assigned to this task.
 * @param[in] ucTaskPriority Specifies the priority of the task.
 * @param[in] cbTaskEntryHandler Specifies the task entry point.
 * 
 * This function creates a task and sets the entry point routine. 
 * Once the task is created it starts executing the specified entry point routine. 
 * The created task comes to an end once this entry point routine returns or when it is destroyed.
 * @see jdd_TaskDestroy
 */
JC_RETCODE jdd_TaskCreate (JDD_TASK *pvTask, JC_INT8 *pcTaskName,
						   JC_UINT16 uhTaskSlice, JC_UINT16 uhStackSize,
						   JC_UINT8 ucTaskPriority,
						   TaskEntryHandler cbTaskEntryHandler) ;

/** 
 * @brief This function suspends a task. 
 * @param[in] vTaskHandle Specifies the task reference which is returned by #jdd_TaskCreate function.
 *
 * When this function is executed the task will remain suspended until it is resumed or destroyed. 
 */
JC_RETCODE jdd_TaskSuspend (JDD_TASK vTaskHandle) ;

/**
 * @brief This function resumes a suspended task. 
 * @param[in] vTaskHandle Specifies the task reference which is returned by #jdd_TaskCreate function.
 * The task should be suspended state before this function is invoked.
 */
JC_RETCODE jdd_TaskResume (JDD_TASK vTaskHandle) ;

/**
 * @brief This function destroys the specified task.
 * @param[in] vTaskHandle Specifies the task reference which is returned by #jdd_TaskCreate function.
 * This function destroys the created task and frees any resource associated with the task.
 */
JC_RETCODE jdd_TaskDestroy (JDD_TASK vTaskHandle) ;

#ifdef __cplusplus
}
#endif


#endif

/* END OF FILE */


