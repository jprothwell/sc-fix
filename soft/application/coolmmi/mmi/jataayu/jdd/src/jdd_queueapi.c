#define MODULE_NAME "JDD_QUEUE"
#define FILE_NAME "jdd_queueapi.c"
#ifdef JATAAYU_SUPPORT
/***************************************************************************
 *
 * File Name jdd_queueapi.c
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
 ***************************************************************************/

/**
 * @file	jdd_queueapi.c
 * @ingroup	jdd_queueapi
 * @brief	Describes the queue related API's.
 **/
 /***************************************************************************
 * System Include Files
 **************************************************************************/
#include "cos.h"
#include "stack_config.h"
#include "mmitask.h"
#include "stack_common.h"
#include "stack_msgs.h"
 /***************************************************************************
 * User Include Files
 **************************************************************************/
#include <ddl.h>
#include <jcal.h>

 /***************************************************************************
 * Macros
 **************************************************************************/
#ifdef JDD_LOG_ENABLED
#define __MODULE_ID__					QUEUE_MODULE
#else
#define __MODULE_ID__					0
#endif



 /***************************************************************************
 * Global Variable declarations and definitions
 **************************************************************************/

 /***************************************************************************
 * Local Function Prototypes
 **************************************************************************/


 /***************************************************************************
 * All Local Function Definitions
 **************************************************************************/

static HANDLE GetTaskHandle(JC_INT32 iTaskID)
{
	HANDLE hTask = 0 ;
	
	if(E_TASK_TRANSPORT == iTaskID)
	{
		hTask = GetMmiTaskHandle(MOD_TRANSPORT) ;
	}
	else
	{
		hTask = GetMmiTaskHandle(MOD_MMI) ;
	}
	return hTask ;
}

/***************************************************************************
 * All Global Function Definitions
 **************************************************************************/


/**
 * @ingroup		jdd_queueapi
 * @brief		The function initializes the queue.
 * @retval		JC_OK on Success.
 * @retval		JC_ERR_QUEUE_INIT on error.
 * This function initializes the queue manager. Any queue resource
 *	initialization can be done in this function depending on the platform.
 * @note		This function should be part of the application startup code.
 **/
JC_RETCODE	jdd_QueueInit (void)
{
	JC_RETCODE rCode = JC_OK ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	//JDD_LOG_INFO0 ("jdd_QueueInit: Entry") ;

	/*--- Platform Specific Implementation Start ---*/


	/* Platform call for Initialize */

	/*
	if ((Platform API succeeds))
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = JC_ERR_QUEUE_INIT ;
		JDD_LOG_INFO1 ("jdd_QueueInit: Returns [%d]", rCode) ;
	}
	*/
	/*--- Platform Specific Implementation End ---*/

	//JDD_LOG_INFO0 ("jdd_QueueInit: Exit") ;
	return (rCode) ;
}

/**
 * @ingroup		jdd_queueapi
 * @brief		This function deinitializes all the resource associated to
 *				the queue.
 * @see			#jdd_QueueInit
 **/
void jdd_QueueDeInit (void)
{
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	//JDD_LOG_INFO0 ("jdd_QueueDeInit: Entry") ;

	/*--- Platform Specific Implementation Start ---*/


	/* Platform call for DeInitialize */

	/*--- Platform Specific Implementation End ---*/

	//JDD_LOG_INFO0 ("jdd_QueueDeInit: Exit") ;
	return ;
}

/**
 * @ingroup		jdd_queueapi
 * @brief		It creates the queue.
 * @param[in]	iSourceTaskID Specifies the task identifier for which the
 *             	queue needs to be created.
 * @param[in]	pcQueueName Specifies the name associated with the
 *             	created Queue. This is just an information parameter
 * @param[in]	uiEventDataSize Specifies the maximum size of the event
 *             	data that will be processed.
 * @param[in]	uiMaxEvents Specifies the maximum number of events that can be
 *             	queued up at one point of time.
 * @param[in]	bSyncFlag Specifies whether the queue operation is blocking or
 *             	non-blocking.
 * @retval		JC_OK on Success.
 * @retval		JC_ERR_QUEUE_EXISTS Specifies that the queue has already  been
 *				created by a call to #jdd_QueueCreate()
 * @retval		JC_ERR_QUEUE_CREATE Specifies the queue resource could not be
 *             	created
 *
 * This function creates a queue with the specified queue options, which is
 * later referred using the iSourceTaskID.
 * @see			#jdd_QueueSend
 * @see			#jdd_QueueRecv
 **/

/*Queus is created with the task itself*/

JC_RETCODE	jdd_QueueCreate (JC_INT32 iSourceTaskID, JC_INT8 *pcQueueName,\
							 JC_UINT32 uiEventDataSize, \
							 JC_UINT32 uiMaxEvents, JC_BOOLEAN bSyncFlag)
{
	JC_RETCODE rCode = JC_OK ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/
	PARAM_INTENTIONALLY_NOT_USED(pcQueueName);
	PARAM_INTENTIONALLY_NOT_USED(uiEventDataSize);
	PARAM_INTENTIONALLY_NOT_USED(uiMaxEvents);

	//JDD_LOG_INFO0 ("jdd_QueueCreate: Entry") ;
	jdd_AssertOnFail ((0 <= iSourceTaskID) ,\
		"jdd_QueueCreate: iSourceTaskID is invalid") ;

	/*--- Platform Specific Implementation Start ---*/
	/*---Platform call for Create queue---*/
	/*
	if ((Platform API succeeds))
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = JC_ERR_QUEUE_CREATE ;
		JDD_LOG_INFO1 ("jdd_QueueCreate: Returns [%d]", rCode) ;
	}
	*/
	/*--- Platform Specific Implementation End ---*/

	//JDD_LOG_INFO0 ("jdd_QueueCreate: Exit") ;
	return (rCode) ;
}

/**
 * @ingroup		jdd_queueapi
 * @brief		This function destroys the specified queue associated with the
 *				ID.
 * @param[in]	iSourceTaskID Specifies the task identifier.
 * @retval		JC_OK on Success
 * @retval		JC_ERR_QUEUE_DESTROY on fail
 * @see			#jdd_QueueSend
 **/

/*Queus is destroyed with the task itself*/

JC_RETCODE	jdd_QueueDestroy (JC_INT32 iSourceTaskID)
{
	JC_RETCODE rCode = JC_ERR_NOT_IMPLEMENTED ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	//JDD_LOG_INFO0 ("jdd_QueueDestroy: Entry") ;
	jdd_AssertOnFail ((0 <= iSourceTaskID) ,\
		"jdd_QueueDestroy: iSourceTaskID is invalid") ;

	/*--- Platform Specific Implementation Start ---*/

	/* Platform call for Queue Destroy */

	/*
	if ((Platform API succeeds))
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = JC_ERR_QUEUE_DESTROY ;
		JDD_LOG_INFO1 ("jdd_QueueDestroy: Returns [%d]", rCode) ;
	}
	*/

	/*--- Platform Specific Implementation End ---*/
	//JDD_LOG_INFO0 ("jdd_QueueDestroy: Exit") ;
	return (rCode) ;
}

/**
 * @ingroup		jdd_queueapi
 * @brief		This function sends the event.
 * @param[in]	pstEventData Specifies the event data and information about
 *             	the source and destination task.
 * @retval		JC_ERR_QUEUE_SEND Specifies the event could not be sent to
 *             	the queue.
 * @retval		JC_OK on Success.
 * This function sends event to the queue identified by the queue ID.
 * This function adds the event data to the end of the queue.
 * @see			#jdd_QueueRecv
 **/
JC_RETCODE	jdd_QueueSend (JC_EVENT_DATA *pstEventData)
{
	JC_RETCODE rCode = JC_OK;
	/*--- Platform Specific Variables ---*/
	  HANDLE hTask  = 0;	
	  BOOL bIsEventSent ;
	  COS_EVENT Event ={0,} ;
	/*-----------------------------------*/

	//JDD_LOG_INFO0 ("jdd_QueueSend: Entry") ;
	jdd_AssertOnFail ((JC_NULL != pstEventData) ,\
		"jdd_QueueSend: pstEventData is NULL") ;

	/*--- Platform Specific Implementation Start ---*/
	hTask = GetTaskHandle(pstEventData->destTaskInfo.iTaskID) ;

	Event.nEventId = MSG_ID_TRANSP_TOMMI_IND ;
	Event.nParam1 = (UINT32)jdd_MemAlloc(1,sizeof(JC_EVENT_DATA)) ;
	if(JC_NULL != Event.nParam1)
	{
		jc_memcpy((JC_INT8*)Event.nParam1, pstEventData, sizeof(JC_EVENT_DATA)) ;

		bIsEventSent = COS_SendEvent(hTask,&Event,COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL) ;
		if(bIsEventSent)
		{
			rCode = JC_OK ;
			mmi_trace (3, "@@@ Queue Send: %d -> %d  Success @@@", \
				pstEventData->srcTaskInfo.iTaskID, pstEventData->destTaskInfo.iTaskID) ;
		}
		else
		{
		
//		errorVal = COS_GetLastError();
			mmi_trace (3, "### Queue Send: %d -> %d  FAIL ###", \
				pstEventData->srcTaskInfo.iTaskID, pstEventData->destTaskInfo.iTaskID) ;
			rCode = JC_ERR_QUEUE_SEND ;
			//JDD_LOG_INFO1 ("jdd_QueueSend: Returns [%d]", rCode) ;
		}
	}
	else
	{
		rCode = JC_ERR_QUEUE_SEND ;
	}


    /* code for switching depending on task */

	/*
	 *   The following switch case will change based on the
	 *	platform. It may switch on srcTaskInfo OR dstTaskInfo.
	 *	The below mentioned switch-case block is just for
	 *	reference and to be implemented based on platform requirement
	 */
	/*
	switch (pstEventData->srcTaskInfo.iTaskID)
    {
        case E_TASK_BROWSER_CORE:
            <platform Variable = flag>
            break;
        case E_TASK_MMS_CORE:
            <platform Variable = flag>
            break;
        case E_TASK_EMAIL_CORE:
            <platform Variable = flag>
            break;
        case E_TASK_TRANSPORT:
            <platform Variable = flag>
            break;
        default:
            rCode = JC_ERR_QUEUE_SEND ;
			JDD_LOG_INFO1 ("jdd_QueueSend: Returns [%d]", rCode) ;
    }*/

	/* ---Platform call for queue send--- */

	/*
	if ((Platform API succeeds))
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = JC_ERR_QUEUE_SEND ;
		JDD_LOG_INFO1 ("jdd_QueueSend: Returns [%d]", rCode) ;
	}
	*/

	/*--- Platform Specific Implementation End ---*/
	//JDD_LOG_INFO0 ("jdd_QueueSend: Exit") ;
	return (rCode) ;
}

/**
 * @ingroup		jdd_queueapi
 * @brief		This function receives the event.
 * @param[in]	pstSrcTaskInfo Specifies the task information which it needs
 *              to receive.
 * @param[out]	pstEventData Specifies the event data which is received.
 * @retval		JC_OK on success
 * @retval		JC_ERR_QUEUE_RECV Specifies the event could not be
 *              received from the queue
 * @retval		JC_ERR_QUEUE_EMPTY Specifies the event could not be read from
 *              the queue as the queue is currently empty.
 *              This is used in non-blocking queues.
 *
 * This function receives event from the queue identified by the task
 * information. It receives the event that is added in the beginning of the
 * queue. The queue is implemented as per FIFO.
 * @see			#jdd_QueueSend
 **/
JC_RETCODE	jdd_QueueRecv (JC_TASK_INFO *pstSrcTaskInfo, \
						   JC_EVENT_DATA *pstEventData)
{
	JC_RETCODE rCode ;
	/*--- Platform Specific Variables ---*/
	HANDLE hTask =0 ;
	COS_EVENT Event ={0,} ;
	BOOL	bIsRecvSuccess ;
	/*-----------------------------------*/

	//JDD_LOG_INFO0 ("jdd_QueueRecv: Entry") ;
	jdd_AssertOnFail ((JC_NULL != pstSrcTaskInfo),\
		"jdd_QueueRecv: pstSrcTaskInfo is NULL") ;
	jdd_AssertOnFail ((JC_NULL != pstEventData),\
		"jdd_QueueRecv : pstEventData is null") ;

	/*--- Platform Specific Implementation Start ---*/
	hTask = GetTaskHandle(E_TASK_TRANSPORT) ;//will always be transport

	bIsRecvSuccess = COS_WaitEvent(hTask, &Event, COS_WAIT_FOREVER);
	if(bIsRecvSuccess)
	{
		if(Event.nParam1)
		{
			jc_memcpy(pstEventData, (const JC_UINT8 *)Event.nParam1, sizeof(JC_EVENT_DATA)) ;
			jdd_MemFree((JC_UINT8 *)Event.nParam1) ;
		}
		rCode = JC_OK ;
		mmi_trace (2, "@@@ Queue Recv: Task -> %d  Success @@@", \
			pstSrcTaskInfo->iTaskID) ;
	}
	else
	{
		rCode = JC_ERR_QUEUE_RECV ;
		mmi_trace (1, "@@@ Queue Recv: Task -> %d  Fail @@@", \
			pstSrcTaskInfo->iTaskID) ;
		//JDD_LOG_INFO1 ("jdd_QueueRecv: Returns [%d]", rCode) ;
	}


	/*---Platform call for queue recv---*/
	/*
	if ((Platform API succeeds))
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = JC_ERR_QUEUE_RECV ;
		JDD_LOG_INFO1 ("jdd_QueueRecv: Returns [%d]", rCode) ;
	}
	*/
	/*--- Platform Specific Implementation End ---*/

	//JDD_LOG_INFO0 ("jdd_QueueRecv: Exit") ;
	return (rCode) ;
}

/**
 * @ingroup		jdd_queueapi
 * @brief		This function clears all the events from the queue
 *             	identified by the source task identifier.
 * @param[in]	iSourceTaskID Specifies the task identifier.
 * @retval		JC_OK on Success
 * @retval		JC_ERR_QUEUE_PURGE on failure
 **/
JC_RETCODE	jdd_QueuePurge (JC_INT32 iSourceTaskID)
{
	JC_RETCODE rCode ;
    /*--- Platform Specific Variables ---*/
	HANDLE hTask =0 ;
	/*-----------------------------------*/

	//JDD_LOG_INFO0 ("jdd_QueuePurge: Entry") ;
	jdd_AssertOnFail ((0 <= iSourceTaskID),\
		"jdd_QueuePurge: iSourceTaskID is invalid") ;

	/*--- Platform Specific Implementation Start ---*/
	/*---Platform call for queue Purge---*/
	hTask = GetTaskHandle(iSourceTaskID) ; 

	if(COS_ResetEventQueue(hTask))
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = JC_ERR_QUEUE_PURGE ;
	}

	/*
	if ((Platform API succeeds))
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = JC_ERR_QUEUE_PURGE ;
		JDD_LOG_INFO1 ("jdd_QueuePurge: Returns [%d]", rCode) ;
	}
	*/
	/*--- Platform Specific Implementation End ---*/

	//JDD_LOG_INFO0 ("jdd_QueuePurge: Exit") ;
	return (rCode) ;
}

/**
 * @ingroup			jdd_queueapi
 * @brief			This function allocates memory for the event data.
 * @param[in, out]	pstEventData Specifies the event data structure whose
 *					pEventData member should contain the allocated valid
 *					memory.
 * @retval			JC_OK on Success.
 * @retval			JC_ERR_QUEUE_ALLOC on failure
 * This function allocates memory of size pstEventData->uiEventInfoSize for
 *  the pEventData member of the JC_EVENT_DATA structure.
 * @see				#jdd_QueueFree
 **/
JC_RETCODE	jdd_QueueAlloc (JC_EVENT_DATA *pstEventData)
{
	JC_RETCODE rCode ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	//JDD_LOG_INFO0 ("jdd_QueueAlloc: Entry") ;
	jdd_AssertOnFail ((JC_NULL != pstEventData),\
		"jdd_QueueAlloc: pstEventData is NULL") ;

	/*--- Platform Specific Implementation Start ---*/
	/*---Platform call for queue Alloc---*/
	/*
	if ((Platform API succeeds))
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = JC_ERR_QUEUE_ALLOC ;
		JDD_LOG_INFO1 ("jdd_QueueAlloc: Returns [%d]", rCode) ;
	}
	*/

	/*--- Platform Specific Implementation End ---*/
	/*
	 * Below mentioned jdd_MemAlloc based Work-around can be used if
	 * "The heap is visible to all the processes and the heap will remain
	 * active even after the application exits"
	 */

   if(pstEventData->uiEventInfoSize >0)
   	{
		pstEventData->pEventInfo = jdd_MemAlloc (1, \
									pstEventData->uiEventInfoSize) ;
		if (JC_NULL != pstEventData->pEventInfo)
		{
			rCode = JC_OK ;
		}
		else
		{
			rCode = JC_ERR_QUEUE_ALLOC ;
			//JDD_LOG_INFO1 ("jdd_QueueAlloc: Returns [%d]", rCode) ;
		}
   	}
    else
    {
		rCode = JC_OK ;
		pstEventData->pEventInfo = JC_NULL;
    }
/*	*/

	//JDD_LOG_INFO0 ("jdd_QueueAlloc: Exit") ;
	return (rCode) ;
}

/**
 * @ingroup		jdd_queueapi
 * @brief		This function frees the memory for the specified event data.
 * @param[in]	pstEventData Specifies the event data information where
 *             	pEventData needs to be freed.
 * @retval		JC_OK on Success
 * @retval		JC_ERR_QUEUE_FREE on failure.
 *
 * This function de-allocates memory for the pEventData member of the
 *  JC_EVENT_DATA structure.
 * @see			#jdd_QueueAlloc
 */
JC_RETCODE	jdd_QueueFree (JC_EVENT_DATA *pstEventData)
{
	JC_RETCODE rCode ;
    /*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	//JDD_LOG_INFO0 ("jdd_QueueFree: Entry") ;
	jdd_AssertOnFail ((JC_NULL != pstEventData),\
		"jdd_QueueFree: pstEventData is NULL") ;

	/*--- Platform Specific Implementation Start ---*/

	/*---Platform call for queue Free---*/
	/*
	if ((Platform API succeeds))
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = JC_ERR_QUEUE_FREE ;
		JDD_LOG_INFO1 ("jdd_QueueFree: Returns [%d]", rCode) ;
	}
	*/

	/*--- Platform Specific Implementation End ---*/
	/*Work-around to be used if jdd_MemAlloc is used in jdd_QueueAlloc*/

	jdd_MemFree (pstEventData->pEventInfo) ;
    pstEventData->pEventInfo = JC_NULL ;
	rCode = JC_OK ;
	/**/
	//JDD_LOG_INFO0 ("jdd_QueueFree: Exit") ;
	return (rCode) ;
}
/*END OF FILE*/
#endif
