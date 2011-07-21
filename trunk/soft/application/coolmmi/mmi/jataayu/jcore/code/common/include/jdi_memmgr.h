/***************************************************************************
 * $Id: jdi_memmgr.h,v 1.7 2008/06/16 05:52:17 browserdevjtbase Exp $
 * $Revision: 1.7 $
 * $Date: 2008/06/16 05:52:17 $
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
 * $Log: jdi_memmgr.h,v $
 * Revision 1.7  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.8  2007/12/05 13:53:23  kumardevhtmlbrow
 * Update for cache chunk and interface file updated.
 *
 * Revision 1.7  2005/10/10 14:09:02  kumardevhtmlbrow
 * Updated for the jdi_MemAlloc and bug fixing
 *
 * Revision 1.6  2005/09/12 12:20:44  kumardevhtmlbrow
 * no message
 *
 * Revision 1.5  2005/08/31 15:23:48  kumardevhtmlbrow
 * no message
 *
 * Revision 1.4  2005/08/31 13:48:21  kumardevhtmlbrow
 * no message
 *
 * Revision 1.3  2005/08/24 15:46:14  kumardevhtmlbrow
 * no message
 *
 * Revision 1.2  2005/08/20 10:36:07  kumardevhtmlbrow
 * Added time utils
 *
 * Revision 1.1  2005/08/12 12:11:04  kumardevhtmlbrow
 * no message
 *
 *  
 ***************************************************************************/

#ifndef _JDI_MEMMGR_H
#define _JDI_MEMMGR_H

#ifdef __cplusplus
extern "C" {
#endif

JC_RETCODE jdi_MemInitialize (JC_UINT32 uiBlock, JC_UINT32 uiSize, JC_HANDLE *pvMemHandle) ;

#ifdef MEM_INTERNAL_POOL_ENABLE

#ifdef MEM_LOG_ENABLED

#define jdi_MemAlloc(x,y,z) jdi_MemDebugAlloc(__FILE__, __LINE__, x, y, z)

void * jdi_MemDebugAlloc (const JC_INT8 *pcFileName, JC_INT32 iLineNum, JC_HANDLE vMemHandle, JC_UINT32 uiLen, JC_UINT32 uiSize) ;

#define jdi_MemFree(x,y) jdi_MemDebugFree(__FILE__, __LINE__, x, y)

void jdi_MemDebugFree (const JC_INT8 *pcFileName, JC_INT32 iLineNum, JC_HANDLE vMemHandle, void *pvFreeRef) ;

#else

void * jdi_MemAlloc (JC_HANDLE vMemHandle, JC_UINT32 uiLen, JC_UINT32 uiSize) ;
void jdi_MemFree (JC_HANDLE	vMemHandle, void *pvFreeRef) ;

#endif /** MEM_LOG_ENABLED **/

#else

#define jdi_MemAlloc(a,b,c)			jdd_MemAlloc(b,c);((void)(a))
#define jdi_MemFree(a,b)			((void)(a));jdd_MemFree(b)

#endif   /** MEM_INTERNAL_POOL_ENABLE **/

void jdi_MemDeinitialize (JC_HANDLE vMemHandle) ;

#ifdef __cplusplus
}
#endif

#endif

/* End of File */



