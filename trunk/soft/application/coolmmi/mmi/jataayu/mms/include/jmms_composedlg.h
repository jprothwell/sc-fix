/***************************************************************************
 *
 * File Name : jmms_composedlg.h
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
 * @file		jmms_composedlg.h
 * @ingroup		jMMS
 * @brief		Describes the Task related API's.
 *
 **/ 
#ifndef __JMMS_COMPOSEDLG
#define __JMMS_COMPOSEDLG

/***************************************************************************
 * System Include Files
 **************************************************************************/


/***************************************************************************
 * User Include Files
 **************************************************************************/

/***************************************************************************
* Macros Definitions
**************************************************************************/
#define JRET(X)	(X * -1)

#ifdef __JATAAYU_APP__

/***************************************************************************
* Global variables declarations
**************************************************************************/
typedef enum
{
	SEND_ONLY,
	SEND_AND_SAVE,
	SAVE_ONLY,
}MMS_send_require;

extern U16 g_MMS_send_require;
/***************************************************************************
* Local Function Prototypes
***************************************************************************/
void jmms_ComposeEditContentOptions (void);
void jmms_EditContentEntryHandler ();
void jmms_PreviewMsg();
void jMMS_POPupMessage(JC_INT8 *pcMessage);
void jmms_SetSlideTimerHandler();
JC_RETCODE jmms_AddTextMediaToCurrSlide(JC_CHAR *psTextBuff);
void jmms_DisplayComposeSlideMMI();
void jmms_DisplayComposeSlideExitHanler();
void Go_to_slide_screen();
void Go_to_slide_screen_cb();
JC_RETCODE jmms_MMIAddPage ();
JC_RETCODE	jmms_SetCurrPageSlideDuration(JC_UINT32 uiSlideTimer);
JC_RETCODE	jmms_GetCurPageSlideDuration(JC_UINT32* pSlideTime);
void jmms_ComposeEditTextEntryHandler ();
void jmms_ComposeEditTextOptionsHandler (void);
void jmms_ComposeEditTextDone();
JC_RETCODE jmms_ComposeSlideClose(void);
JC_RETCODE jmms_MMIViewMmsPlaySlide (JC_UINT8 PageIndex, JC_BOOLEAN bPlayPresentation);
void jmms_ComposeEditTextHandler();
void jmms_ShowConfirmationForRestrictedMediaAdd (void) ;
void jmms_ComposeDoneOption(void);
void jMms_Highlight_Send(void);
void jMms_Highlight_Send_And_Save(void);
void jMms_Highlight_Save(void);
void jMms_Highlight_Exit_Compose(void);
void jMms_Highlight_App_Exit_Compose(void);
void jMms_Set_Edit_Content_Highlight_Handler(void);
void jmms_FreeAttachmentListMemory();
void jMms_ComposeFormAttachmentList (JC_UINT8	**pListItems, \
										MEDIA_INFO *pMediaList, JC_UINT8 uiAttachListCount);


#endif // __JATAAYU_APP__

#endif //__JMMS_COMPOSEDLG

