/***************************************************************************
 * $Id: jdd_memapi.h,v 1.3 2008/11/18 07:44:09 browserdevjtbase Exp $
 * $Revision: 1.3 $
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
 * $Log: jdd_memapi.h,v $
 * Revision 1.3  2008/11/18 07:44:09  browserdevjtbase
 * Core updates
 *
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.11  2006/01/25 07:22:16  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.9  2005/10/10 14:09:03  kumardevhtmlbrow
 * Updated for the jdi_MemAlloc and bug fixing
 *
 * Revision 1.8  2005/09/14 12:27:24  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.7  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.6  2005/07/29 13:27:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.5  2005/07/27 16:06:46  kumardevhtmlbrow
 * no message
 *
 * Revision 1.4  2005/04/05 13:07:24  kavithadevhtmlbrow
 * Updating for XML Parser
 *
 * Revision 1.3  2005/03/24 10:57:50  kumardevhtmlbrow
 * Changed as per the coding standard
 *
 *  
 ***************************************************************************/

#ifndef _JDD_MEMAPI_H_
#define _JDD_MEMAPI_H_

#ifdef __cplusplus
extern "C"
{
#endif

//#define ENABLE_MEM_LEAK_FINDER
/** 
 * @file jdd_memapi.h
 * @ingroup Memory
 * @brief Defines the memory abstraction API calls.
 *
 * All the memory API calls are assumed to be synchronous. When the call returns it 
 * is assumed that the operation is attempted and the result of the operation is 
 * available at that moment.
 */

/**
 * @brief The Memory layer shall be initialized using this API call. 
 *
 * This function needs to be invoked by the application before calling any of the 
 * memory API calls specified in this document or invoking the core interface layer. 
 * If the platform does not require any initialization this call can return the 
 * success value. 
 * @Note This API should be invoked as part of the startup code.
 * @see jdd_MemDeinit 
 */
 
JC_RETCODE jdd_MemInit (void) ;

/**
 * @brief The Memory layer shall be deinitialized using this API call. 
 *
 * This function needs to be invoked by the application during the device shutdown
 * or exiting all the application in case of the independent application.
 * 
 * @see jdd_MemInit 
 */
JC_RETCODE jdd_MemDeinit (void) ;



#ifdef ENABLE_MEM_LEAK_FINDER
void *	jdd_MemAlloc_Debug(JC_UINT32 uiItemSize, JC_UINT32 uiSize,const JC_UINT8 *pc_fname, JC_INT32 l_line);
void jdd_MemFree_Debug(void *vpPtr, const JC_UINT8 *pc_fname, JC_INT32 l_line);

#define jdd_MemAlloc(x,y)	jdd_MemAlloc_Debug(x,y,(const JC_UINT8 *)__FILE__,__LINE__)
#define jdd_MemFree(x)	    jdd_MemFree_Debug(x,(const JC_UINT8 *)__FILE__,__LINE__)

#else
/**
 * @brief Allocates an array in the memory.
 * @param[in] uiNum Specifies the number of elements
 * @param[in] uiSize Specifies the length in bytes of each elements
 * @return Returns the valid memory reference or NULL in case of error.
 *
 * The requested memory is allocated and returned by this function. 
 * On success the starting address of the memory allocated is returned, 
 * otherwise NULL is returned. 
 * @Note This function should return the zero initialized memory.
 * @see jdd_MemFree
 */

void * jdd_MemAlloc (JC_UINT32 uiNum, JC_UINT32 uiSize) ;

/** 
 * @brief Deallocates the specified memory block.
 * @param[in] pvMemRef Specifies the memory reference which needs to be deallocated
 * 
 * The memory reference that are allocated using #jdd_MemAlloc shall be freed
 * using this function. After calling this function, the memory reference is invalid
 * and the application should not refer it.
 * @see jdd_MemAlloc 
 */
void jdd_MemFree (void *pvMemRef) ;
#endif 

/** 
 * @brief Reallocates the memory.
 * @param[in] pvMemRef Specifies memory reference which needs to be reallocated
 * @param[in] uiSize Specifies the new size that is required.
 * @return Returns the valid memory reference or NULL in case of error.
 *
 * The requested memory is reallocated and returned by this function. 
 * On success the starting address of the memory allocated is returned, otherwise NULL is returned. 
 * @warning Recommended not to use this function.
 * @see jdd_MemFree
 */
void * jdd_MemRealloc (void* pvMemRef, JC_UINT32 uiSize) ;

#ifdef MEMSET_REQUIRED
#define JDD_MEMSET_ZERO(a, b) jc_memset(a, 0, b) 
#else
/**
 * @brief Specifies the macro which needs to be mapped to the memset API 
 * if the #jdd_MemAlloc does not initialize to zero.
 * 
 * This macro is provided to avoid double memset by memory allocation and also by the
 * application. If the memalloc is already memset the content to zero, then this macro
 * can be declared to dummy.
 */
#define JDD_MEMSET_ZERO(a, b)
#endif


#ifdef __cplusplus
}
#endif

#endif

/* END OF FILE */

