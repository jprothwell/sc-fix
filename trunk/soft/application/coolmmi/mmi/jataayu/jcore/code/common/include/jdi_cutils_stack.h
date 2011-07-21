/***************************************************************************
 * $Id: jdi_cutils_stack.h,v 1.5 2008/06/16 05:52:17 browserdevjtbase Exp $
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
 * $Log: jdi_cutils_stack.h,v $
 * Revision 1.5  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.5  2005/09/12 12:20:44  kumardevhtmlbrow
 * no message
 *
 * Revision 1.4  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.3  2005/06/22 09:32:32  kumardevhtmlbrow
 * no message
 *
 * Revision 1.2  2005/05/21 11:20:48  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.1  2005/05/09 15:13:37  kumardevhtmlbrow
 * Stack Utility Function added
 *
 *  
 ***************************************************************************/

#ifndef _JDI_CUTILS_STACK_H
#define _JDI_CUTILS_STACK_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _utils_stack
{
	void				*pvNodeInfo ;
	struct _utils_stack *pstNext ;
} ST_UTILS_STACK ;

typedef struct _utils_stack_info
{
	ST_UTILS_STACK		*pstTopNode ;
	JC_HANDLE			vCMemHandle ;
} ST_UTILS_STACK_INFO ;

JC_RETCODE jdi_CUtilsStackInitialize (JC_UINT32 uiDefaultCount, ST_UTILS_STACK_INFO *pstStackInfo) ;
JC_RETCODE jdi_CUtilsStackPush (void *pvNewNode, ST_UTILS_STACK_INFO *pstStackInfo) ;
void * jdi_CUtilsStackPop (ST_UTILS_STACK_INFO *pstStackInfo) ;
void * jdi_CUtilsStackGet (const ST_UTILS_STACK_INFO *pstStackInfo) ;
void jdi_CUtilsStackDeinitialize (ST_UTILS_STACK_INFO *pstStackInfo) ;

#ifdef __cplusplus
}
#endif

#endif

/* End of File */


