/***************************************************************************
*
 
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
 
* @ingroup	jWAP
* @brief		Describes the Task related API's.
*
**/


/***************************************************************************
* System Include Files
**************************************************************************/


/***************************************************************************
* User Include Files
**************************************************************************/ 

#ifndef __JWAP_BOOKMARK
#define __JWAP_BOOKMARK

#ifdef __JATAAYU_APP__

void jWap_bmInitBookMark() ;
void jWap_BookMark_entry_handler () ;
ST_CATEGORY_LIST* jWap_bmGetCatagoryListPtr() ;

JC_INT32 jWap_bmFillBookMarkToList(JC_UINT8 **puBookMarkList) ;

void jWap_bmfetchBookMark() ;
void jWapAddBookMark();
void jWapAddBookSaveTitle() ;
void jWap_bookmark_saveURL() ;
void jWap_BM_options_highlight_handler(int iSelectIndex) ;
void jWap_bmAddBookMark() ;
void jWap_bmClearBookMark() ;
void jWap_bmClearAllBookMark() ;
void jWap_bmOptionBookMark() ;
void jWap_bmoptions_highlight_handler(int iSelectIndex) ;
void jWap_bmEditBookMark() ;
void jWap_saved_editedBM() ;
void jWap_editBM_URL() ;
void jWap_EditBMTitle() ;
void FreeBookmarkList(JC_UINT8 **puBookMarkList,JC_INT32 iListCount) ;
void GetBmClearUserConfirmation() ;
void GetBmClearAllUserConfirmation() ;
void HandleBmClearYes() ;
void HandleBmClearAllYes() ;
void HandleBmNo() ;
void jWap_addEditBm_addInterface (JC_CHAR *pmUrl, JC_CHAR *pmTitle) ; 
void jWap_freeBookmarkInfo () ;
void jwap_SaveBMRSKCallBack();
void HandleYes();
void HandleNo();
void jWap_GoBackToOption();
void jwap_PrepareBMLaunch(JC_CHAR	 *pmPushUrl);

#endif // __JATAAYU_APP__
#endif  

