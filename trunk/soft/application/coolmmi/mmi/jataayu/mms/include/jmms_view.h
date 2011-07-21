/***************************************************************************
 *
 * File Name : jmms_view.h
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
 * @file		jmms_view.h
 * @ingroup		jMMS
 * @brief		Describes the MMS view related API's.
 *
 **/ 
#ifndef	MMS_VIEW_H
#define	MMS_VIEW_H


/***************************************************************************
 * System Include Files
 **************************************************************************/


/***************************************************************************
 * User Include Files
 **************************************************************************/
#include "jmms_inc.h"

/***************************************************************************
* Macros Definitions
**************************************************************************/



/***************************************************************************
* Global variables declarations
**************************************************************************/

/***************************************************************************
* Local Function Prototypes
***************************************************************************/
void jmms_ViewMsgFromFolder ();
void jmms_MMIViewScreenHandler();
void jmms_MMIViewExitScreenHandler () ;
void jmms_HandleViewPresentationEnd();
void jmms_HandleStopView();
void jmms_HandlePauseView();
void jmms_HandleResumeView();
void jmms_HandleReplayView();
void jmms_HandleExitView();
void jmms_MMIViewDeInit();
void jmms_MMIReportScreenEntryHandler();
void jmms_MMIReportScreenExitHandler() ;
JC_RETCODE jmms_MMISetViewCallback (MSG_HANDLE hMsgHandle) ;
void jMMS_ViewUpKeyHandler();
void jMMS_ViewDownKeyHandler();
void jMMS_ViewLeftKeyHandler();
void jMMS_ViewRightKeyHandler();
void jMMS_ViewRegisterKeyHandler();
void jmms_HandleMsgViewScrolling (U16 keyCode);

JC_RETCODE jmms_MMIDisplayDeliveryReport(MESSAGE_HEADERS *pMessageHdr);
JC_RETCODE jmms_MMIDisplayReadResponse(MESSAGE_HEADERS *pMessageHdr);
JC_RETCODE jmms_ViewMms (CONTENT_DATA *pstContent, JC_BOOLEAN bViewPageOnly);

JC_CHAR* ConvertDateValueToDateString(JC_UINT32 uiDateValue);
JC_RETCODE jmms_ViewMmsStop (JC_BOOLEAN bAudioVideoStatus);
void jmms_MMIReadreportCancel();
void jmms_MMISendReadreport();
void jmms_ShowSubject(JC_CHAR *pSubject);
void jmms_ShowVersion(JC_INT8 *pVersion);
void jmms_ShowFrom(JC_CHAR *pFrom);
void jmms_ShowCC(JC_CHAR *pCc);
void jmms_ShowTo(JC_CHAR *pTo);
void jmms_ShowBcc(JC_CHAR *pBcc);
void jmms_ShowClass(EMessageClass eMsgClass);
void jmms_ShowPriority(EMessagePriority eMsgPriority);
void jmms_ShowDateTime(JC_UINT32 nDateTime, JC_BOOLEAN bRelative);
void jmms_ShowSize(JC_UINT32 uiBufferLen);
void jmms_ShowDeliveryReport(JC_BOOLEAN bDeliveryReport);
void jmms_ShowReadReport(JC_BOOLEAN bReadReport);
void jmms_ShowRetrieveStatus(JC_CHAR *pStatusText);
void jmms_ShowDeliveryStatus(EMessageStatus eDeliveryStatus);
void jmms_ShowReadReportStatus(EMessageReadStatus eReadStatus);

void jmms_MMIViewSlideOptionsEntryHandler();
void jmms_view_option_highlight_handler (int iSelectIndex) ;
void jmms_SaveAttachmentExitHandler();
void jMms_SaveAttachmentHighlightHandler (int iSelectIndex);
void jmms_SaveAttachmentOptionEntryHandler (void);
void jmms_SaveMediaOptionEntryHandler();
void jmms_SaveAttachment();
JC_RETCODE jmms_SaveAttachmentWitetoFile (JC_CHAR *pPath, MEDIA_INFO *pMedia);
JC_RETCODE jMMS_MMIWriteToFile (JDD_FSHANDLE hFileSysHandle, \
									JC_CHAR *pPath, JC_UINT8 *pBuffer, JC_UINT32 uiBufLen);
void jmms_ViewSetPlayRSK();
void jmms_MMIViewStartSlidePlay();
void jmms_getMediaFilenameFromUser();
void jmms_SetFilename ();

#endif //MMS_VIEW_H
