 /***************************************************************************
 *
 * File Name : jmms_addrdlg.h
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
 * @file		jmms_addrdlg.h
 * @ingroup		jMMS
 * @brief		
 *
 **/ 
#ifndef __JMMS_ADDRDLG
#define __JMMS_ADDRDLG

/***************************************************************************
 * System Include Files
 **************************************************************************/


/***************************************************************************
 * User Include Files
 **************************************************************************/


/***************************************************************************
* Macros Definitions
**************************************************************************/


#define __JATAAYU_APP__


/***************************************************************************
* Global variables declarations
**************************************************************************/


/***************************************************************************
* Local Function Prototypes
***************************************************************************/
void jMms_AddressPhoneBook_highlight_handler (int iSelectIndex) ;
void jMms_AddressPhoneBook_entry_handler (void);
void jMms_AddressSubMenu_highlight_handler (int iSelectIndex) ;
void jMms_AddressSubMenu_entry_handler (void);
void jMms_Address_highlight_handler (int iSelectIndex) ;
void jmms_ComposeMainScreenEntryHandler (void);
void jMMS_GetMessageHeader(void);
void jMms_addrGetValueFromList (void);
void jmms_MMIComposeDeInit();
void jmms_MMIGotoMMSMainScreen();
void jMms_addrGetValueFromList ();
void jmms_initaddrInfo();
void jmms_MMIComposeDeInit();
void jmms_ComposeMainScreenEndKeyHandler();
JC_CHAR* jMms_addrFormAddrBuffFromList (JC_CHAR **addr_list, JC_INT32 iCount) ;
JC_RETCODE jmms_PrepareCurrSlideForDisplay( void ) ;
JC_BOOLEAN isDuplicateEntry (JC_UINT8 *pmInputEntry) ;
JC_BOOLEAN jmms_IsValidPhoneNumber(JC_CHAR *pPhoneNumber);
void jmms_ComposeRecipientNumberLSKhandler();
void finished_phonebook_entry();
void jMms_AddressInputPhoneBook_entry_handler();
BOOL jmms_ComposeWithModification(void);
void GoBackFromInputMethod(void);
JC_INT32	jMms_addrFormListFromAddrBuff (JC_CHAR *pmAddrBuff, JC_CHAR **addr_list);
#endif // __JMMS_ADDRDLG

