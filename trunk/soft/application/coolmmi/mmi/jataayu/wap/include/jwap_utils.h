 /***************************************************************************
 *
 * File Name : jwap_utils.h
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
 * @file		jwap_inc.h
 * @ingroup	jWAP
 * @brief		Describes the Task related API's.
 *
 **/ 
#ifndef __JWAP_UTILS_INCLUDE
#define __JWAP_UTILS_INCLUDE

 /***************************************************************************
 * System Include Files
 **************************************************************************/


 /***************************************************************************
 * User Include Files
 **************************************************************************/
#include "jwap_inc.h"



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

void	jWap_Register_KeyHandler_Funcs () ;
void 	jWap_freePageInfo (ST_BE_PAGE_INFO* pstPageInfo) ;
void	jWap_freeFetchInfo (ST_BE_FETCH_REQ* pStFetchInfo) ;
void 	jWap_setSoftKeys () ;
void 	jWap_showErrorPopup (JC_RETCODE iErr) ;
JC_CHAR	*jWap_GetUniqueFileName (JC_INT8 *pcPath) ;
JC_RETCODE jWap_SaveToFile (JC_CHAR *pmFileName, JC_UINT32 uiBufLen, JC_UINT8 *pucBuffer) ;
void	jWap_GetFileNameFromURL (JC_CHAR  *pmURL, JC_CHAR  *pmFileName, JC_UINT32 uiFileNameLen) ;
JC_BOOLEAN jWap_IsValidFileName (const JC_CHAR *pmFileName, JC_UINT16 *uhSpecialCharIndex) ;
JC_RETCODE jWap_CopyToFile (JC_CHAR *pmSrcFileName, JC_CHAR *pmDestFileName) ;
INT		jWap_copyAuthInfo (ST_BE_PAGE_INFO* pstPageInfo) ;
JC_BOOLEAN appIsValidShortcut (JC_CHAR *pmText, JC_BOOLEAN bIsScheme) ;
JC_BOOLEAN aapIsValidTcharUrl (JC_CHAR	*pURLText, JC_CHAR **ppEditedURL, JC_BOOLEAN *pbIsFileScheme) ;
JC_BOOLEAN appIsValidKeyword (JC_INT8 *pucSchemeText) ;
JC_BOOLEAN appIsValidScheme (JC_INT8 *pucSchemeText) ;
JC_BOOLEAN IsStrHavingSpecialChars(JC_CHAR* pmStr) ;
JC_RETCODE appRemoveLeadTrailSpacesWord (JC_INT8 *pucSourceWord, JC_INT8 **ppucDestWord) ;
JC_RETCODE appRemoveDuplicateWord (JC_CHAR **ppmStringText, JC_UINT8 ucSeparateText);

JC_RETCODE TruncateLeadingAndTrailingSpaces (JC_CHAR *pucSourceWord, JC_CHAR **ppucDestWord);
JC_BOOLEAN IsValidUrl (JC_INT8 *pcURLText, JC_BOOLEAN *pbIsFileScheme);
JC_BOOLEAN appCheckValidHostname (JC_INT8 *pcURLText, JC_INT32 iStartIndex, JC_INT32 iEndIndex);
JC_BOOLEAN appCheckValidIpAddress (JC_INT8 *pcURLText, JC_INT32 iStartIndex, JC_INT32 iEndIndex);
JC_BOOLEAN appCheckValidPort (JC_INT8 *pcURLText, JC_INT32 iStartIndex, JC_INT32 iEndIndex);
JC_BOOLEAN appCheckValidScheme (JC_INT8 *pcURLText, JC_INT32 iSchemeIndex);
JC_CHAR *jwap_validateBMTitle () ;
void jwap_UpdatePushIcon();
void jwap_showPushIcon(JC_UINT32 uiUnreadMsg, JC_UINT32 uiTotalMsg);
JC_RETCODE jwap_GetOfflinePageCountFromFolder();
//JC_RETCODE jwap_CheckFileStatus(JC_BOOLEAN *bIsFileExist, JC_CHAR *pmFileName );
//JC_CHAR* MapFileNameToDrive(const JC_CHAR *pmFilePath);

#endif // __JWAP_INCLUDE

