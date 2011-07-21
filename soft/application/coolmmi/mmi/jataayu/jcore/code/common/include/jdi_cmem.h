/***************************************************************************
 * $Id: jdi_cmem.h,v 1.7 2008/11/18 07:44:09 browserdevjtbase Exp $
 * $Revision: 1.7 $
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
 * $Log: jdi_cmem.h,v $
 * Revision 1.7  2008/11/18 07:44:09  browserdevjtbase
 * Core updates
 *
 * Revision 1.9  2008/11/04 08:12:37  kumardevhtmlbrow
 * Memory module with more logs.
 *
 * Revision 1.8  2008/11/04 06:52:44  kumardevhtmlbrow
 * More memory Log added
 *
 * Revision 1.7  2007/05/29 16:01:48  kumardevhtmlbrow
 * Added the debug allocation for cmem and dmem.
 *
 * Revision 1.6  2005/10/10 14:09:02  kumardevhtmlbrow
 * Updated for the jdi_MemAlloc and bug fixing
 *
 * Revision 1.5  2005/09/12 12:20:44  kumardevhtmlbrow
 * no message
 *
 * Revision 1.4  2005/06/24 14:31:53  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.3  2005/05/09 11:52:12  kumardevhtmlbrow
 * Fixed the lint warning/info
 *
 * Revision 1.2  2005/03/17 07:11:36  kavithadevhtmlbrow
 * Updated the interface prototype and JC_NULL
 *
 * Revision 1.1  2005/03/15 15:00:33  kumardevhtmlbrow
 * Added the initial file for the CMem and DMem
 *
 *  
 ***************************************************************************/

/**
 * @file jdi_ChunkInfo.h
 * @It contains defination of the routines for allocating the chunk of memory.
 */

#ifndef _JDI_CMEM_H
#define _JDI_CMEM_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MEM_LOG_ENABLED
#define jdi_CMemInitialize(X,Y,Z)	jdi_CMemInitializeDebug(__FILE__, __LINE__, X, Y, Z)
#define jdi_CMemAlloc(X)			jdi_CMemAllocDebug(__FILE__, __LINE__, X)
#define jdi_CMemFree(X, Y)			jdi_CMemFreeDebug(__FILE__, __LINE__, X, Y)
#define jdi_CMemReset(X)			jdi_CMemResetDebug(__FILE__, __LINE__, X)
#define jdi_CMemDeinitialize(X)		jdi_CMemDeinitializeDebug(__FILE__, __LINE__, X)
#endif

/**
 * @brief Initializing the chunk module and returns the handle.
 * @param[in] uiBlock Specifies the number of block
 * @param[in] uiSize Specifies the size of each block
 * @param[out] pvHandle Provides the handle to the application which 
 * needs to be passed to the other API calls.
 * @return JC_RETCODE Returns JC_OK on success otherwise specific 
 * error value.
 *
 * This function should be used to initialiaze the chunk handle.
 * It takes the number of blocks and the size of the block as the 
 * parameters and returns the handle.
 */
#ifdef MEM_LOG_ENABLED
JC_RETCODE jdi_CMemInitializeDebug (const JC_INT8 *pcFileName, JC_INT32 iLine, JC_UINT32 uiBlock, JC_UINT32 uiSize, JC_HANDLE *pvHandle) ;
#else
JC_RETCODE jdi_CMemInitialize (JC_UINT32 uiBlock, JC_UINT32 uiSize, JC_HANDLE *pvHandle) ;
#endif

/**
 * @brief Allocate Memory from the chunk of memory allocated.
 * @param[in] vHandle Specifies the handle from which the memory needs to be allocated
 * @return void *  Returns the Valid memory reference on success or NULL.
 *
 * This function is first of all checks whether the allocated chunk of memory buffer
 * has free/available area then it will assign that one in the pCurrPtr, otherwise 
 * it will allocate the chunk of memory of size (uiSize) with (uiBlock) number of 
 * blocks.
 */
#ifdef MEM_LOG_ENABLED
void * jdi_CMemAllocDebug (const JC_INT8 *pcFileName, JC_INT32 iLine, JC_HANDLE vHandle) ;
#else
void * jdi_CMemAlloc (JC_HANDLE vHandle) ;
#endif

/**
 * @brief Free the specified chunk of memory
 * @param[in] vHandle Specifies the handle in which the memory needs to be freed.
 * @param[in] pvPtr Specifies the memory reference which needs to be noted for free.
 * @return void
 *
 * This routine resets the memory and assigns to the starting of the memory poll.
 * Only the root memory pool is maintained and other chunk memory are freed.
 */
#ifdef MEM_LOG_ENABLED
JC_BOOLEAN jdi_CMemFreeDebug (const JC_INT8 *pcFileName, JC_INT32 iLine, JC_HANDLE vHandle, void *pvPtr) ;
#else
JC_BOOLEAN jdi_CMemFree (JC_HANDLE vHandle, void *pvPtr) ;
#endif

/**
 * @brief Reset the chunk memory
 * @param[in] vHandle Specifies the handle from which the memory needs to be reset
 * @return void
 *
 * This routine resets the memory and assigns to the starting of the memory poll.
 * Only the root memory pool is maintained and other chunk memory are freed.
 */

#ifdef MEM_LOG_ENABLED
void jdi_CMemResetDebug (const JC_INT8 *pcFileName, JC_INT32 iLine, JC_HANDLE vHandle) ;
#else
void jdi_CMemReset (JC_HANDLE vHandle) ;
#endif

/**
 * @brief Free the memory associated to the chunk memory
 * @param[in] vHandle Specifies the handle which needs to be deinitialized.
 * @return void  
 * This function free the memory associated to the chunk memory
 * and also it free all the memory node. 
 */

#ifdef MEM_LOG_ENABLED
void jdi_CMemDeinitializeDebug (const JC_INT8 *pcFileName, JC_INT32 iLine, JC_HANDLE vHandle) ;
#else
void jdi_CMemDeinitialize (JC_HANDLE vHandle) ;
#endif

#ifdef __cplusplus
}
#endif

#endif

/* End of File */



