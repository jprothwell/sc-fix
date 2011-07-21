/***************************************************************************
 * $Id: jdd_queueapi.h,v 1.2 2006/05/02 11:19:21 kumardevjtbase Exp $
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
 * $Log: jdd_queueapi.h,v $
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

#ifndef _JDD_QUEUE_H
#define _JDD_QUEUE_H

#ifdef __cplusplus
extern "C"
{
#endif

/** 
 * @file jdd_queueapi.h
 * @ingroup Queue
 * @brief Describes the queue related API's.
 *
 */

/**
 * @brief It initializes the queue.
 * 
 * This function initializes the queue manager. Any queue resource initialization can be 
 * done in this function depending on the platform. 
 @ note This function should be part of the application startup code.
 */
JC_RETCODE	jdd_QueueInit (void) ;

/**
 * @brief This function deinitializes all the resource associated to the queue.
 */
void		jdd_QueueDeInit (void) ;

/**
 * @brief It creates the queue.
 * @param[in] iSourceTaskID Specifies the task identifier for which the queue needs to be created.
 * @param[in] pcQueueName Specifies the name associated with the created Queue. This is just an information parameter
 * @param[in] uiEventDataSize Specifies the maximum size of the event data that will be processed. If the size specifies zero then there is no limit.
 * @param[in] uiMaxEvents Specifies the maximum number of events that can be queued up at one point of time.
 * @param[in] bSyncFlag Specifies whether the queue operation is blocking or non-blocking. 
 * @retval JC_ERR_QUEUE_EXISTS Specifies that the queue has already  been created by a call to jdd_QueueCreate()
 * @retval JC_ERR_QUEUE_CREATE Specifies the queue resource could not be created
 *
 * This function creates a queue with the specified queue options, which is later referred 
 * using the iSourceTaskID.
 * @see jdd_QueueSend
 * @see jdd_QueueRecv
 */
JC_RETCODE	jdd_QueueCreate (JC_INT32 iSourceTaskID, JC_INT8 *pcQueueName,
							 JC_UINT32 uiEventDataSize, JC_UINT32 uiMaxEvents,
							 JC_BOOLEAN bSyncFlag) ;

/**
 * @brief This function destroys the specified queue associated by this task ID.
 * @param[in] iSourceTaskID Specifies the task identifier.
 */
JC_RETCODE	jdd_QueueDestroy (JC_INT32 iSourceTaskID) ;

/**
 * @brief This function sends the event.
 * @param[in] pstEventData Specifies the event data and information about the source and destination task.
 * @retval JC_ERR_QUEUE_SEND Specifies the event could not be sent to the queue.
 *
 * This function sends event to the queue identified by the queue ID. This function adds the event data to the end of the queue. 
 * @see jdd_QueueRecv
 */
JC_RETCODE	jdd_QueueSend (JC_EVENT_DATA *pstEventData) ;

/**
 * @brief This function receives the event.
 * @param[in] pstSrcTaskInfo Specifies the task information which it needs to receive.
 * @param[out] pstEventData Specifies the event data which is received.
 * @retval JC_ERR_QUEUE_RECV Specifies the event could not be received from the queue
 * @retval JC_ERR_QUEUE_EMPTY Specifies the event could not be read from the queue as the queue is currently empty. This is used in non-blocking queues.
 *
 * This function receives event from the queue identified by the task information. 
 * This function receives the event that is added in the beginning of the queue. 
 * The queue is implemented as per FIFO.
 * @see jdd_QueueSend
 */
JC_RETCODE	jdd_QueueRecv (JC_TASK_INFO *pstSrcTaskInfo, JC_EVENT_DATA *pstEventData) ;

/**
 * @brief This function clears all the events from the queue identified by the source task identifier.
 * @param[in] iSourceTaskID Specifies the task identifier.
 */
JC_RETCODE	jdd_QueuePurge (JC_INT32 iSourceTaskID) ;

/**
 * @brief This function allocates memory for the event data.
 * @param[in, out] pstEventData Specifies the event data structure whose pEventData 
						   member should contain the allocated valid memory
 *
 * This function allocates memory of size, pstEventData->uiEventInfoSize for the pEventData member of the JC_EVENT_DATA structure. 
 * This function is responsible for allocating the memory (which is valid across all tasks/processes) 
 * for the pEventData member of this structure.  
 * The memory allocated for this shall be de-allocated using jdd_QueueFree.
 * @see jdd_QueueFree
 */
JC_RETCODE	jdd_QueueAlloc (JC_EVENT_DATA *pstEventData) ;

/**
 * @brief This function frees the memory for the specified event data.
 * @param[in] pstEventData Specifies the event data information where pEventData needs to be freed.
 *
 * This function de-allocates memory for the pEventData member of the JC_EVENT_DATA structure. The memory pointed by pEventData should be allocated previously by a call to jdd_QueueAlloc ().
 * @see jdd_QueueAlloc
 */
JC_RETCODE	jdd_QueueFree (JC_EVENT_DATA *pstEventData) ;

#ifdef __cplusplus
}
#endif

#endif

/* END OF FILE */


