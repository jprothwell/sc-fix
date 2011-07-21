/***************************************************************************
 *
 * File Name : jwap_mmi-cb.h
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
 * @file		jwap_mmi_cb.h
 * @ingroup	jWAP
 * @brief
 *
 **/
 #ifndef __JWAP_MMI_CB_INCLUDE
#define __JWAP_MMI_CB_INCLUDE
 
 /***************************************************************************
 * System Include Files
 **************************************************************************/
 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/

 /***************************************************************************
 * Macros
 **************************************************************************/

 /***************************************************************************
 * Global variables declarations
 **************************************************************************/

 /***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

 /***************************************************************************
 * All Local Function Definitions
 ***************************************************************************/

 /***************************************************************************
 * All Global Function Definitions
 **************************************************************************/
	
#ifdef __JATAAYU_APP__
	
void 	jWap_mainMenu_entry_handler (void) ;
void 	jWap_highlight_handler (void) ;
void 	jWap_keyUpArrHandler () ;
void 	jWap_keyDownArrHandler () ;
void 	jWap_keyLeftArrHandler () ;
void 	jWap_keyRightArrHandler () ;
void	jWap_keyEnterHandler () ;
void 	jWap_key2Handler () ;
void 	jWap_key3Handler () ;
#if defined(__MMI_TOUCH_SCREEN__)
void	jWap_PenDownHandler (mmi_pen_point_struct stPenCoordinates) ;
void	jWap_PenUpHandler (mmi_pen_point_struct stPenCoordinates) ;
void	jWap_PenMoveHandler (mmi_pen_point_struct stPenCoordinates) ;
#endif
void	jWap_priMmi_cancel_handler () ;
void    jWap_recentPages_entry_handler();
void 	jWap_showAuthUserNameScreen () ;
void	jWap_set_priMmiScreen () ;
void jWap_exitOptionsScreen (void) ;
void 	jWap_options_quit_handler () ;
void 	jWap_options_add_bookmark_handler() ;
void jWap_options_SaveImage_Handler (void) ;
void jWap_options_SavePage_Handler (void) ;
void jWap_SavePage (void) ;
void jWap_SaveImage (void) ;
void jWap_setConnectingTitle (JC_CHAR *pszTitile) ;
void jWap_Options_fetchUrl() ;
void jWap_fetch_options_add_bookmark_handler() ;
void jWap_fetch_options_highlight_handler(JC_INT32 iIndex) ;
void jWap_fetchUrl () ;
void jWap_options_navigation_highlight_handler(JC_INT32 iIndex) ;
void jWap_optoins_navigation_handler() ;
void jWap_optNavigation_back_handler ();
void jWap_keyEndHandler (void) ;
JC_UINT8 jwap_ApplicationExitHandler(void);
void jWap_options_Show_URL();
void jWap_options_Set_URL_As_Homepage();
void jwap_PreapreMMIScreen();
void jwap_ViewSavedPages ();
void jwap_DeleteSavedPages();
void jwap_DeleteAllSavedPages();
pBOOL jwap_GetSavedpagesListItems( S32 item_index, UI_string_type str_buff, \
					UI_image_type* img_buff_p, U8 str_img_mask );
void jWap_options_Set_URL_As_Homepage_EntryHandler();
void jwap_DeleteSavedPagesEntryHandler();
void jwap_DeleteAllSavedPagesEntryHandler();
JC_RETCODE jWap_FetchPushUrl(void);


#endif //__JATAAYU_APP__
#endif //__JWAP_MMI_CB_INCLUDE 

