/***************************************************************************
 * $Id: jdd_semapi.h,v 1.2 2006/05/02 11:19:21 kumardevjtbase Exp $
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
 * $Log: jdd_semapi.h,v $
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

#ifndef _JDD_SEMAPHORE_H
#define _JDD_SEMAPHORE_H

/**
 * @file jdd_semapi.h
 * @ingroup Semaphore
 * @brief Describes the semaphore interfaces.
 */

#ifdef __cplusplus
extern "C"
{
#endif

/** 
 * @brief This function initializes the semaphore management. 
 *
 * If in some platforms there are no specific initialization is required then this 
 * call can return JC_OK.
 */
JC_RETCODE jdd_SemaphoreInit (void) ;

/**
 * @brief This function de-initializes the semaphore management. 
 *
 * If in some platforms there are no specific deinitialization is required then this 
 * call can return JC_OK.
 */
JC_RETCODE jdd_SemaphoreDeInit (void) ;

/** 
 * @brief This function creates a semaphore for a given identification name and semaphore ID.
 * @param[in] uiSemaphoreID Specifies the semaphore identifier.
 * @param[in] pcSemaphoreName Specifies the name of the semaphore.
 * @param[in] uhSemaphoreCnt Specifies the maximum number of tasks that can obtain the semaphore without getting suspended.
 * @param[in] bSyncFlag Specifies whether the semaphore created is blocking or non-blocking, E_TRUE specifies blocking.
 */
JC_RETCODE jdd_SemaphoreCreate (JC_UINT32	uiSemaphoreID,
								JC_INT8		*pcSemaphoreName,
								JC_UINT16	uhSemaphoreCnt,
								JC_BOOLEAN	bSyncFlag) ;

/**
 * @brief This function destroys the created semaphore identified by the semaphore id and frees any resource associated with this semaphore.
 * @param[in] uiSemaphoreID Specifies the semaphore identifier.
 */
JC_RETCODE jdd_SemaphoreDestroy (JC_UINT32 uiSemaphoreID) ;

/** 
 * @brief This function attempts to obtain an instance of the specified semaphore. 
 * @param[in] uiSemaphoreID Specifies the semaphore identifier.
 * @param[in] uiTimeoutVal Specifies the maximum time till which the calling task will be suspended.
 */
JC_RETCODE jdd_SemaphoreGet (JC_UINT32 uiSemaphoreID, 
							 JC_UINT32 uiTimeoutVal) ;

/**
 * @brief This function releases the previously obtained semaphore.
 * @param[in] uiSemaphoreID Specifies the semaphore identifier.
 */
JC_RETCODE jdd_SemaphoreRelease (JC_UINT32 uiSemaphoreID) ;

#ifdef __cplusplus
}
#endif

#endif

/* END OF FILE */


