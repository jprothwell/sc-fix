/***************************************************************************
 *
 * File Name : jmms_mmi_cb.h
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
 * @file		jmms_mmi_cb.h
 * @ingroup		jMMS
 * @brief
 *
 **/

#ifndef __JMMS_MMI_CB_INCLUDE
#define __JMMS_MMI_CB_INCLUDE
 
 /***************************************************************************
 * System Include Files
 **************************************************************************/
 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/

/***************************************************************************
* Macros Definitions
**************************************************************************/

/***************************************************************************
* Global variables declarations
**************************************************************************/


/***************************************************************************
* Local Function Prototypes
***************************************************************************/
	
#ifdef __JATAAYU_APP__
	
void	jMms_mainMenu_entry_handler (void) ;
void	jMms_highlight_handler (void) ;
void jMms_inbox_hint_handler(U16 index);
void jMms_sent_highlight_handler(U16 index);
void jMms_outbox_highlight_handler(U16 index);
void jMms_draft_highlight_handler(U16 index);
void	jMms_template_entry_handler();
void	jMms_settings_entry_handler();
void	jMms_SendMsgHandler (JC_BOOLEAN bIsEdit) ;
void	jMms_saveMsgHandler (void) ;
void 	show_ExitProgress_screen() ;
void	jMms_Aboutus_entry_handler();
void	jmms_ComposeSlideRegisterKeyHandler ();
void 	jmms_CallbackGoBackHistory();
void 	jMms_Write_Msg_entry_handler();
void 	jmms_ResetGlobalVal();
void 	jmms_MainMenuExitHandler();
void	show_progress_screen (E_JMMS_QUEUE_NODE_STATE eNodeState, JC_UINT32 uiResultValue) ;
void jmms_update_progress_screen(E_JMMS_QUEUE_NODE_STATE eNodeState, JC_UINT32 uiResultValue);
void    jMms_SendMsgEntryHandler();
//void    jmms_ExitHandler();
void    jmms_SendProgressEndKeyHandler();
//void    jmms_ComposeSlideEndKeyHandler();
JC_UINT8 jmms_ApplicationExitHandler(void* p);

#ifdef __MMI_TOUCH_SCREEN__
void jMMS_PenDownHandler (mmi_pen_point_struct stPenCoordinates);
void jMMS_PenMoveHandler (mmi_pen_point_struct stPenCoordinates);
#endif


#endif //__JATAAYU_APP__

#endif //__JWAP_MMI_CB_INCLUDE 
