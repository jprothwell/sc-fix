/***************************************************************************
 *
 * File Name : jmms_composeutils.h
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
 * @file		jmms_composeutils.h
 * @ingroup		jMMS
 * @brief		Describes the MMS utils API's.
 *
 **/ 
#ifndef __JMMS_COMPOSEUTILS
#define __JMMS_COMPOSEUTILS

 /***************************************************************************
 * System Include Files
 **************************************************************************/


 /***************************************************************************
 * User Include Files
 **************************************************************************/




/***************************************************************************
* Macros Definitions
**************************************************************************/
//#define JRET(X)	(X * -1)

#ifdef __JATAAYU_APP__

/***************************************************************************
* Global variables declarations
**************************************************************************/


/***************************************************************************
* Local Function Prototypes
***************************************************************************/

JC_RETCODE AddObjectMedia(JC_CHAR *pFileName,EMediaType eMediaType,JC_UINT8 ucSlideNo,JC_BOOLEAN bIsAttachment);
JC_RETCODE AddTextMedia(JC_CHAR *psTextBuffer,JC_UINT8 ucSlideNo);
JC_RETCODE jMMS_MMIGetInputBoxValue (JC_CHAR **ppsInputValue);
JC_RETCODE jMms_ComposeAutoSignature(void) ;
void jMms_DeletePage_Msg();
void jMms_DeleteImage_Msg();
void jMms_DeleteAudio_Msg();
void jMms_DeleteVideo_Msg();
void jMms_compose_createMode_handler() ;
void jMms_compose_contentClass_handler ()  ;
JC_RETCODE jmms_MMISetComposeCallback (MSG_HANDLE hMsgHandle) ;
void jMMS_HandleMediaError(JC_RETCODE rCode);
void jMMS_HandleTextMediaError(JC_RETCODE rCode);
JC_RETCODE jmms_UpdateMessageHeaders();
JC_INT8* GetContentTypeFromFile (JC_CHAR *szFileName);

#endif // __JATAAYU_APP__


#endif //__JMMS_COMPOSEUTILS
