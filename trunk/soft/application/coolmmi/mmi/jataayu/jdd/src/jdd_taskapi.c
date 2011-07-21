#define MODULE_NAME "JDD_TASK"
#define FILE_NAME "jdd_taskapi.c"
#ifdef JATAAYU_SUPPORT
 /***************************************************************************
 *
 * File Name : jdd_taskapi.c
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
 * @file		jdd_taskapi.c
 * @ingroup		jdd_taskapi
 * @brief		Describes the Task related API's.
 *
 **/
 
 
 /***************************************************************************
 * System Include Files
 **************************************************************************/
#include "cos.h" 
 /***************************************************************************
 * User Include Files
 **************************************************************************/
# include <ddl.h>
# include <jcal.h>

 /***************************************************************************
 * Macros
 **************************************************************************/
#ifdef JDD_LOG_ENABLED
#define __MODULE_ID__					TASK_MODULE
#else
#define __MODULE_ID__					0
#endif


 /***************************************************************************
 * Global variables declarations
 **************************************************************************/

 /***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

 /***************************************************************************
 * All Local Function Definitions
 ***************************************************************************/

static void newTaskHandler(void *pArg)
{
	TaskEntryHandler pTaskEntryHandler = (TaskEntryHandler) pArg;
if(pTaskEntryHandler)
	pTaskEntryHandler(0, JC_NULL);
//	return JC_NULL;
}
 /***************************************************************************
 * All Global Function Definitions
 **************************************************************************/
			
/**
 * @ingroup		jdd_taskapi
 * @brief		This function creates a task.
 * @param[out]	pvTask Returns the reference to the task created.
 * @param[in]	pcTaskName Specifies the name associated to the task.
 * @param[in]	uhTaskSlice Indicates the maximum amount of time in 
 *				milliseconds that can expire while executing this task.
 * @param[in]	uhStackSize Specifies the stack size that needs to be
 *				assigned to this task.
 * @param[in]	ucTaskPriority Specifies the priority of the task.
 * @param[in]	pTaskEntryHandler Specifies the task entry point.
 * @retval		JC_OK on success.
 * @retval		JC_ERR_TASK_CREATE on failure.
 * 
 * This function creates a task and sets the entry point routine. Once the
 * task is created it starts executing the specified entry point routine. 
 * The created task comes to an end once this entry point routine returns 
 * or when it is destroyed.
 * @see			#jdd_TaskDestroy
 **/
JC_RETCODE jdd_TaskCreate (JDD_TASK * pvTask, JC_INT8 * pcTaskName, \
						   JC_UINT16 uhTaskSlice, JC_UINT16 uhStackSize, \
						   JC_UINT8 ucTaskPriority, \
						   TaskEntryHandler pTaskEntryHandler)
{
	JC_RETCODE  rCode ;
	/*--- Platform Specific Variables ---*/
	HANDLE hHandleTask ;
	/*-----------------------------------*/

	JDD_LOG_INFO0 ("jdd_TaskCreate : Entry") ;
	jdd_AssertOnFail (( JC_NULL != pvTask), \
						"jdd_TaskCreate: pvTask is NULL") ;
	jdd_AssertOnFail (( JC_NULL != pTaskEntryHandler), \
						"jdd_TaskCreate: pTaskEntryHandler is NULL") ;

	/*--- Platform Specific Implementation Start ---*/

	hHandleTask =  COS_CreateTask((PTASK_ENTRY) newTaskHandler, pTaskEntryHandler,
									JC_NULL, uhStackSize,ucTaskPriority,
									COS_CREATE_DEFAULT, 0,pcTaskName);


	/* PlatForm Specific Function for Task Create */

	
	if (JC_NULL == hHandleTask)
	{
		rCode = JC_ERR_TASK_CREATE ;
		JDD_LOG_INFO1 ("jdd_TaskCreate: Returns [%d]", rCode) ;
	}
	else
	{
		*pvTask = (JDD_TASK)hHandleTask ;
		rCode = JC_OK ;
	}
/**/		
	

	/*--- Platform Specific Implementation End ---*/
	JDD_LOG_INFO0 ("jdd_TaskCreate : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup		jdd_taskapi
 * @brief		This function destroys the specified task.
 * @param[in]	vTaskHandle Specifies the task reference which is returned
 *			  	by #jdd_TaskCreate function.
 * @retval		JC_OK on success.
 * @retval		JC_ERR_TASK_DESTROY on failure.
 * 
 * This function destroys the created task and frees any resource associated
 * with the task.
 **/
JC_RETCODE jdd_TaskDestroy (JDD_TASK vTaskHandle)
{
	JC_RETCODE      rCode ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	JDD_LOG_INFO0 ("jdd_TaskDestroy : Entry") ;
	jdd_AssertOnFail ((JC_NULL != vTaskHandle), \
						"jdd_TaskDestroy: vTaskHandle is NULL") ;

	/*--- Platform Specific Implementation Start ---*/

	/* PlatForm Specific Function for Task Destroy */
	 ;

	
	if (COS_DeleteTask((HANDLE) vTaskHandle))
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = JC_ERR_TASK_DESTROY ;
		JDD_LOG_INFO1 ("jdd_TaskDestroy: Returns [%d]", rCode) ;
	}
	/**/		

	/*--- Platform Specific Implementation End ---*/
	JDD_LOG_INFO0 ("jdd_TaskDestroy : Exit") ;
	return (rCode) ;
}

/** 
 * @ingroup		jdd_taskapi
 * @brief		This function suspends a task. 
 * @param[in]	vTaskHandle Specifies the task reference which is returned
 *			  	by #jdd_TaskCreate function.
 * @retval		JC_OK on success.
 * @retval		JC_ERR_TASK_SUSPEND on failure.
 * 
 * When this function is executed the task will remain suspended until it
 * is resumed or destroyed. 
 **/
JC_RETCODE jdd_TaskSuspend (JDD_TASK vTaskHandle)
{
	JC_RETCODE      rCode ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	JDD_LOG_INFO0 ("jdd_TaskSuspend : Entry") ;
	jdd_AssertOnFail ((JC_NULL != vTaskHandle), \
						"jdd_TaskSuspend: vTaskHandle is NULL") ;

	/*--- Platform Specific Implementation Start ---*/

	/* PlatForm Specific Function for Task Suspend */
	
	if (COS_SuspendTask((HANDLE)vTaskHandle))
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = JC_ERR_TASK_SUSPEND ;
		JDD_LOG_INFO1 ("jdd_TaskSuspend: Returns [%d]", rCode) ;
	}
	/**/		

	/*--- Platform Specific Implementation End ---*/
	JDD_LOG_INFO0 ("jdd_TaskSuspend : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup		jdd_taskapi
 * @brief		This function resumes a suspended task. 
 * @param[in]	vTaskHandle Specifies the task reference which is returned
 *			  	by #jdd_TaskCreate function.
 * @retval		JC_OK on success.
 * @retval		JC_ERR_TASK_RESUME on failure.
 *
 * The task should be in suspended state before this function is invoked.
 **/
JC_RETCODE jdd_TaskResume (JDD_TASK vTaskHandle)
{
	JC_RETCODE      rCode ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	JDD_LOG_INFO0 ("jdd_TaskResume : Entry") ;

	jdd_AssertOnFail ((JC_NULL != vTaskHandle), \
						"jdd_TaskResume: vTaskHandle is NULL") ;

	/*--- Platform Specific Implementation Start ---*/

	/* PlatForm Specific Function for Task Resume */

	
	if (COS_ResumeTask ((HANDLE) vTaskHandle))
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = JC_ERR_TASK_RESUME ;
		JDD_LOG_INFO1 ("jdd_TaskResume: Returns [%d]", rCode) ;
	}
	/**/		

	/*--- Platform Specific Implementation End ---*/
	JDD_LOG_INFO0 ("jdd_TaskResume : Exit") ;
	return (rCode) ;
}
/* END OF FILE */
#endif
