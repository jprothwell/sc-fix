/***************************************************************************
 * $Id: jdi_dmem.h,v 1.8 2008/11/18 07:44:09 browserdevjtbase Exp $
 * $Revision: 1.8 $
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
 * $Log: jdi_dmem.h,v $
 * Revision 1.8  2008/11/18 07:44:09  browserdevjtbase
 * Core updates
 *
 * Revision 1.12  2008/11/04 08:12:37  kumardevhtmlbrow
 * Memory module with more logs.
 *
 * Revision 1.11  2008/09/18 08:04:29  kumardevhtmlbrow
 * Memory Utility Optimization
 *
 * Revision 1.10  2007/05/29 16:01:48  kumardevhtmlbrow
 * Added the debug allocation for cmem and dmem.
 *
 * Revision 1.9  2005/09/12 12:20:44  kumardevhtmlbrow
 * no message
 *
 * Revision 1.8  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.7  2005/06/01 06:56:21  kumardevhtmlbrow
 * Updated the memory utils and test program
 *
 * Revision 1.6  2005/05/27 13:26:01  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.5  2005/05/09 11:52:12  kumardevhtmlbrow
 * Fixed the lint warning/info
 *
 * Revision 1.4  2005/04/05 13:24:19  kumardevhtmlbrow
 * Cookie initial source code added ..
 *
 * Revision 1.3  2005/03/19 14:52:18  kumardevhtmlbrow
 * no message
 *
 * Revision 1.2  2005/03/17 07:11:36  kavithadevhtmlbrow
 * Updated the interface prototype and JC_NULL
 *
 * Revision 1.1  2005/03/15 15:00:33  kumardevhtmlbrow
 * Added the initial file for the CMem and DMem
 *
 *  
 ***************************************************************************/

#ifndef _JDI_DMEM_H
#define _JDI_DMEM_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MEM_LOG_ENABLED
#define jdi_DMemInitialize(X,Y)			jdi_DMemInitializeDebug(__FILE__, __LINE__, X, Y)
#define jdi_DMemAlloc(X,Y,Z)			jdi_DMemAllocDebug(__FILE__, __LINE__, X, Y, Z)
#define jdi_DMemResAlloc(X,Y,Z)			jdi_DMemResAllocDebug(__FILE__, __LINE__, X, Y, Z)
#define jdi_DMemResRealloc(X,Y,Z,Z1)	jdi_DMemResReallocDebug(__FILE__, __LINE__, X, Y, Z, Z1)
#define jdi_DMemReset(X)				jdi_DMemResetDebug(__FILE__, __LINE__, X)
#define jdi_DMemDeinitialize(X)			jdi_DMemDeinitializeDebug(__FILE__, __LINE__, X)
#endif

typedef struct dMemInfo 
{
	JC_UINT8			*pucBuf ;
	JC_UINT8			*pucCurrBuf ;
	JC_UINT32			uiBufPtr ; 
	JC_UINT32			uiBufSize ;
	struct dMemInfo		*pstActiveMemInfo ;
	struct dMemInfo		*pstNext ;	
} ST_DMEM_INFO ;

/**
 * @brief Initializing the dmemory routine and returns the handle.
 * @param[in] uiLen Specifies the length of the memory that needs to be allocated.
 * @param[out] *pvHandle Provides the handle to the application which 
 * needs to be passed to the other API calls of the dmemory routine.
 * @return JC_RETCODE Returns JC_OK on success otherwise specific 
 * error value.
 *
 * This function should be used to initialiaze the dynamic memory routine.
 * It takes the chunk of memory that needs to be allocated for one time. If the 
 * chunk of memory allocation is greater than 10KB then the maximum that is 
 * allocated one time will be 10KB. 
 */
#ifdef MEM_LOG_ENABLED
JC_RETCODE jdi_DMemInitializeDebug (const JC_INT8 *pcFileName, JC_INT32 iLine, 
									JC_UINT32 uiLen, JC_HANDLE *pvHandle) ;
#else
JC_RETCODE jdi_DMemInitialize (JC_UINT32 uiLen, JC_HANDLE *pvHandle) ;
#endif

/**
 * @brief Allocates the memory from the dynamic memory which was allocated.
 * @param[in] vHandle Specifies the handle of the dynamic memory routine
 * @param[in] uiLen Specifies the length of the memory that is required to 
 *                    be allocated.
 * @param[in] uiSize Specifies the size of the memory which needs to be allocated.
 * @return void * Returns the valid pointer on successful allocation of memory 
 *				  otherwise returns JC_NULL.
 *
 * This function allocates the memory from the already allocated pool. If the 
 * memory that is available in the pool is not sufficient allocates another new 
 * poll of memory and returns the valid pointer. In case of memory allocation error
 * this function returns JC_NULL.
 */
#ifdef MEM_LOG_ENABLED
void *jdi_DMemAllocDebug (const JC_INT8 *pcFileName, JC_INT32 iLine, JC_HANDLE vHandle,
						  JC_UINT32 uiLen, JC_UINT32 uiSize) ;
#else
void *jdi_DMemAlloc (JC_HANDLE vHandle, JC_UINT32 uiLen, JC_UINT32 uiSize) ;
#endif

/**
 * @brief Reserves the memory from the dynamic memory.
 * @param[in] vHandle Specifies the handle of the dynamic memory routine
 * @param[in] uiLen Specifies the length of the memory that is required to 
 *                    be allocated.
 * @param[in] uiSize Specifies the size of the memory which needs to be allocated.
 * @return void * Returns the valid pointer on successful reservation of memory 
 *				  otherwise returns JC_NULL.
 *
 * This function reserves the memory from the already allocated pool. If the 
 * memory that is available in the pool is not sufficient allocates another new 
 * poll of memory and returns the valid pointer. In case of memory allocation error
 * this function returns JC_NULL.
 * jdi_DMemResOccupied function needs to be called inorder to use it permanently.
 */
#ifdef MEM_LOG_ENABLED
void * jdi_DMemResAllocDebug (const JC_INT8 *pcFileName, JC_INT32 iLine, JC_HANDLE vHandle,
							  JC_UINT32 uiLen, JC_UINT32 uiSize) ;
#else
void * jdi_DMemResAlloc (JC_HANDLE vHandle, JC_UINT32 uiLen, JC_UINT32 uiSize) ;
#endif

/**
 * @brief Reserves the additional memory from the dynamic memory.
 * @param[in] vHandle Specifies the handle of the dynamic memory routine
 * @param[in] uiPreviousLen Specifies the previous length of the memory that is reserved. 
 * @param[in] uiMoreLen Specifies the more length of the memory that is reserved. 
 * @param[in] uiSize Specifies the size of the memory which needs to be allocated.
 * @return void * Returns the valid pointer on successful reservation of memory 
 *				  otherwise returns JC_NULL.
 *
 * This function reserves the additional memory from the already allocated pool. If the 
 * memory that is available in the pool is not sufficient allocates another new 
 * poll of memory and returns the valid pointer. In case of memory allocation error
 * this function returns JC_NULL.
 * If new memory address is assigned, the old memory content is copied to the new address.
 * jdi_DMemResOccupied function needs to be called inorder to use it permanently.
 */
#ifdef MEM_LOG_ENABLED
void * jdi_DMemResReallocDebug (const JC_INT8 *pcFileName, JC_INT32 iLine, JC_HANDLE vHandle,
								JC_UINT32 uiPreviousLen, JC_UINT32 uiMoreLen, JC_UINT32 uiSize) ;
#else
void * jdi_DMemResRealloc (JC_HANDLE vHandle, JC_UINT32 uiPreviousLen,
						   JC_UINT32 uiMoreLen, JC_UINT32 uiSize) ;
#endif

/**
 * @brief Permanenlty assigns the memory has occupied. 
 * @param[in] vHandle Specifies the handle of the dynamic memory routine
 * @param[in] uiLen Specifies the length of the memory that is required to 
 *                    be allocated.
 * @param[in] uiSize Specifies the size of the memory which needs to be allocated.
 * @return void 
 *
 */
void jdi_DMemResOccupied (JC_HANDLE vHandle, JC_UINT32 uiLen, JC_UINT32 uiSize) ;

/**
 * @brief Resets the memory allocated by the dynamic memory.
 * @param[in] vHandle Specifies the handle of the dynamic memory routine
 * @return void 
 *
 * This function resets the memory and assigns to the starting of the memory 
 * allocated. Only one memory poll is maintained and other memory poll are 
 * freed.
 */
#ifdef MEM_LOG_ENABLED
void jdi_DMemResetDebug (const JC_INT8 *pcFileName, JC_INT32 iLine, JC_HANDLE vHandle) ;
#else
void jdi_DMemReset (JC_HANDLE vHandle) ;
#endif

/**
 * @brief Free all the memory associated to the dynamic memory handle.
 * @param[in] vHandle Specifies the handle of the dynamic memory routine
 * @return void 
 *
 * This function free all the resource allocated for the dynamic memory 
 * and also free all the memory poll allocated.
 */
#ifdef MEM_LOG_ENABLED
void jdi_DMemDeinitializeDebug (const JC_INT8 *pcFileName, JC_INT32 iLine, JC_HANDLE vHandle) ;
#else
void jdi_DMemDeinitialize (JC_HANDLE vHandle) ;
#endif
#ifdef __cplusplus
}
#endif

#endif

/* End of File */

