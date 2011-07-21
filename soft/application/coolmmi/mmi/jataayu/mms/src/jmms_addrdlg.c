#define MODULE_NAME "JMMS"
#define FILE_NAME "jmms_addrdlg.c"
 /***************************************************************************
 *
 * File Name : jmms_addrdlg.c
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
 * @file		jmms_addrdlg.c
 * @ingroup	jMMS
 * @brief		Describes the MMI Address related API's.
 *
 **/ 
 #ifdef JATAAYU_SUPPORT
 /***************************************************************************
 * System Include Files
 **************************************************************************/ 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/
#include "jmms_inc.h"


#include "jmms_cb.h"
#include "jmms_composeutils.h"
#include "jmms_addrdlg.h"
#include "jmms_composedlg.h"
#include "phonebookgprot.h"
#include "jmms_composeutils.h"
#include "messagesresourcedata.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#endif

 /***************************************************************************
 * Macros
 **************************************************************************/

 /***************************************************************************
 * Global variables declarations
 **************************************************************************/
#define 	jMMS_TO_INPUT_SIZE		41
#define		JMMS_ADDR_MAX_COUNT		10
#define 	JMMS_ADDR_MAX_MENU_ITEMS 10

typedef enum _jmms_addr_menu
{
	E_JMMS_ADDR_TO,
	E_JMMS_ADDR_CC,
	E_JMMS_ADDR_BCC,
	E_JMMS_ADDR_SUB,
	E_JMMS_INSERT_CONTENT,	
	E_JMMS_CONTENT_MODE,
	E_JMMS_CONTENT_CLASS,
	//E_JMMS_MESSAGE_SAVE,
	E_JMMS_ADDR_MENU_COUNT
} E_JMMS_ADDR_MENU ;

typedef enum _jmms_addr_options_menu
{
	E_JMMS_ADDROPT_ADD_NUMBER,
	E_JMMS_ADDROPT_ADD_EMAIL,
	E_JMMS_ADDROPT_EDIT,
	E_JMMS_ADDROPT_DELETE,
	E_JMMS_ADDROPT_DELETE_ALL,
	E_JMMS_ADDROPT_MENU_COUNT
}E_JMMS_ADDR_OPTIONS_MENU ;

typedef struct _jmms_addr_info
{	
	JC_INT8 cToCount ;
	JC_INT8 cCcCount ;
	JC_INT8 cBccCount ;
	JC_INT8 *pcCurrCount ;
	JC_INT8 cSlideCount ;

	JC_CHAR	*addr_list_to [JMMS_ADDR_MAX_COUNT] ;
	JC_CHAR	*addr_list_cc [JMMS_ADDR_MAX_COUNT] ;
	JC_CHAR	*addr_list_bcc [JMMS_ADDR_MAX_COUNT] ;
	JC_CHAR	**addr_list ;

	JC_BOOLEAN	bIsEdited ;
	JC_INT8		cSelIndex ;

	JC_BOOLEAN	bIsForward ;
	JC_BOOLEAN	bIsPhonebookEntry ;
	JC_BOOLEAN	bIsPhonebookEMail ;
	U16		currStrId ;

	JC_INT8 cHighLightedRecipientList;
	JC_CHAR 	*item_texts [E_JMMS_ADDR_MENU_COUNT] ;
} ST_JMMS_ADDR_INFO ;
 /***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

 /***************************************************************************
 * All Local Function Definitions
 ***************************************************************************/

 /***************************************************************************
 * All Global Function Definitions
 **************************************************************************/
static JC_UINT16 g_OptionStringId = 0 ;
static ST_JMMS_ADDR_INFO *g_pstAddrInfo ;

extern JC_CHAR* jMms_addrFormAddrBuffFromList (JC_CHAR **addr_list, JC_INT32 iCount) ;

void jMms_addrGetValueFromList (void)
{
	if(g_pstJMmsAppInfo->stMmsHeader.pTo)
	{
		jdd_MemFree(g_pstJMmsAppInfo->stMmsHeader.pTo);
		g_pstJMmsAppInfo->stMmsHeader.pTo = NULL;
	}

	if(g_pstJMmsAppInfo->stMmsHeader.pCc)
	{
		jdd_MemFree(g_pstJMmsAppInfo->stMmsHeader.pCc);
		g_pstJMmsAppInfo->stMmsHeader.pCc = NULL;
	}

	if(g_pstJMmsAppInfo->stMmsHeader.pBcc)
	{
		jdd_MemFree(g_pstJMmsAppInfo->stMmsHeader.pBcc);
		g_pstJMmsAppInfo->stMmsHeader.pBcc= NULL;
	}
	
	g_pstJMmsAppInfo->stMmsHeader.pTo = jMms_addrFormAddrBuffFromList (g_pstAddrInfo->addr_list_to, g_pstAddrInfo->cToCount) ;
	g_pstJMmsAppInfo->stMmsHeader.pCc = jMms_addrFormAddrBuffFromList (g_pstAddrInfo->addr_list_cc, g_pstAddrInfo->cCcCount) ;
	g_pstJMmsAppInfo->stMmsHeader.pBcc = jMms_addrFormAddrBuffFromList (g_pstAddrInfo->addr_list_bcc, g_pstAddrInfo->cBccCount) ;
}

void jmms_ComposeSubDoneHandle()
{
	if(g_pstJMmsAppInfo->stMmsHeader.pSubject)
	{
	  jdd_MemFree(g_pstJMmsAppInfo->stMmsHeader.pSubject);
	  g_pstJMmsAppInfo->stMmsHeader.pSubject = NULL;
	}
	jMMS_MMIGetInputBoxValue (&(g_pstJMmsAppInfo->stMmsHeader.pSubject));	
	DeleteNHistory(1) ;
	GoBackHistory();
}

void jmms_ComposeSubCancelHandle()
{
	DeleteNHistory(1) ;
	GoBackHistory();
}
void GoBackFromInputMethod(void)
{
	GoBacknHistory(1);
}
void jmms_ComposeSubOptionHltHandler (S32 index)
{
	switch(index)
	{
		case 0:
			SetLeftSoftkeyFunction (jmms_ComposeSubDoneHandle, KEY_EVENT_UP ) ;
			break;
			
		case 1:
			SetLeftSoftkeyFunction (jmms_ComposeSubCancelHandle, KEY_EVENT_UP ) ;
			break;

		case 2:
			SetLeftSoftkeyFunction (EntryInputMethodScreen, KEY_EVENT_UP ) ;
			RegisterInputMethodScreenCloseFunction (GoBackFromInputMethod);
			break ;
	}	
}

void jmms_ComposeSubLSKhandler ()
{
	U16 item_texts[] = {STR_GLOBAL_DONE, \
						STR_GLOBAL_CANCEL, \
						STR_GLOBAL_INPUT_METHOD} ;
	
	U16 item_icons[] = {IMG_GLOBAL_L1, \
						IMG_GLOBAL_L2, \
						IMG_GLOBAL_L3} ;
	
	EntryNewScreen (SCR_ID_MMS_SUBJECT_OPTION_SCREEN, 0, jmms_ComposeSubLSKhandler, 0) ;
	SetParentHandler (SCR_ID_MMS_SUBJECT_SCREEN);
	
	RegisterHighlightHandler (jmms_ComposeSubOptionHltHandler);

	ShowCategory52Screen (STR_GLOBAL_OPTIONS, IMG_SMS_ENTRY_SCRN_CAPTION, 
		STR_GLOBAL_OK, 0,
		STR_GLOBAL_BACK, 0, 
		3, item_texts, item_icons,
		0, 0, 0, 0);	
	
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP ) ;	
	
}

extern void GBSetInputboxLSKFunction(FuncPtr f);

void jmms_ComposeSubEntryhandler()
{
	U8* guiBuffer = JC_NULL;	
	EntryNewScreen (SCR_ID_MMS_SUBJECT_SCREEN, JC_NULL, \
					jmms_ComposeSubEntryhandler, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_SUBJECT_SCREEN);			

	ShowCategory5Screen (STR_ID_MMS_ADDR_SUB, IMG_SMS_ENTRY_SCRN_CAPTION,
						STR_GLOBAL_OPTIONS, 0,
						STR_GLOBAL_BACK, 0,
						INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
						(U8*)jMms_getInputBuff (), jMMS_TO_INPUT_SIZE, 
						guiBuffer);
	
#ifdef __MMI_GB__
	GBSetInputboxLSKFunction (jmms_ComposeSubLSKhandler);
#endif
	
	SetLeftSoftkeyFunction (jmms_ComposeSubLSKhandler, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	RegisterInputMethodScreenCloseFunction (GoBackHistory );
	
}

#define LOWER_CASE_ALPHA_BEGIN				97
#define LOWER_CASE_ALPHA_END				122
#define UPPER_CASE_ALPHA_BEGIN				65
#define UPPER_CASE_ALPHA_END				90
#define NUMERIC_BEGIN						48
#define NUMERIC_END							57
#define MAX_EMAIL_LENGTH					50
#define MAX_PH_NUMBER_LENGTH				40
#define MIN_PH_NUMBER_LENGTH				4

JC_BOOLEAN jmms_IsValidEmailID (JC_CHAR	*pEmailID)
{
	JC_INT32	iIndex ;
	JC_INT32	iLength ;
	JC_BOOLEAN	bIsAt = E_FALSE;
	JC_BOOLEAN	bIsDomain = E_FALSE;

	iLength = jc_tcslen (pEmailID) ;
	if (iLength == 0 || iLength > MAX_EMAIL_LENGTH)
	{
		return E_FALSE;
	}
	/* coolsand change
	if ((pEmailID[0] >= NUMERIC_BEGIN && pEmailID[0] <= NUMERIC_END)
		|| pEmailID[0] == (JC_CHAR)'_' )
	{
		return E_FALSE;
	}
	*/
	if (pEmailID[iLength - 1] == (JC_CHAR)'_')
	{
		return E_FALSE;
	}
	for(iIndex = 0; iIndex < iLength; iIndex++)
	{
		if(pEmailID[iIndex] == (JC_CHAR)'@')
		{
			if (!((pEmailID[iIndex-1] >= LOWER_CASE_ALPHA_BEGIN && pEmailID[iIndex-1] <= LOWER_CASE_ALPHA_END) || (pEmailID[iIndex-1] >= UPPER_CASE_ALPHA_BEGIN && pEmailID[iIndex-1] <= UPPER_CASE_ALPHA_END)) )
			{
				if (!(pEmailID[iIndex-1] >= NUMERIC_BEGIN && pEmailID[iIndex-1] <= NUMERIC_END))
				{
					return E_FALSE;
				}
			}
			if (!((pEmailID[iIndex+1] >= LOWER_CASE_ALPHA_BEGIN && pEmailID[iIndex+1] <= LOWER_CASE_ALPHA_END) || (pEmailID[iIndex+1] >= UPPER_CASE_ALPHA_BEGIN && pEmailID[iIndex+1] <= UPPER_CASE_ALPHA_END)) )
			{
				if (!(pEmailID[iIndex+1] >= NUMERIC_BEGIN && pEmailID[iIndex+1] <= NUMERIC_END))
				{
					return E_FALSE;
				}
			}
			if(bIsAt == E_FALSE)
			{
				bIsAt = E_TRUE;
			}
			else
			{
				return E_FALSE;
			}
		}
		else if(pEmailID[iIndex] == (JC_CHAR)'.')
		{
			if (!((pEmailID[iIndex-1] >= LOWER_CASE_ALPHA_BEGIN && pEmailID[iIndex-1] <= LOWER_CASE_ALPHA_END) || (pEmailID[iIndex-1] >= UPPER_CASE_ALPHA_BEGIN && pEmailID[iIndex-1] <= UPPER_CASE_ALPHA_END)) )
			{
				if (!(pEmailID[iIndex-1] >= NUMERIC_BEGIN && pEmailID[iIndex-1] <= NUMERIC_END))
				{
					return E_FALSE;
				}
			}
			if (!((pEmailID[iIndex+1] >= LOWER_CASE_ALPHA_BEGIN && pEmailID[iIndex+1] <= LOWER_CASE_ALPHA_END) || (pEmailID[iIndex+1] >= UPPER_CASE_ALPHA_BEGIN && pEmailID[iIndex+1] <= UPPER_CASE_ALPHA_END)) )
			{
				if (!(pEmailID[iIndex+1] >= NUMERIC_BEGIN && pEmailID[iIndex+1] <= NUMERIC_END))
				{
					return E_FALSE;
				}
			}
			if(bIsDomain == E_FALSE)
			{
				bIsDomain = E_TRUE;
			}
		}
		else
		{
			if (!((pEmailID[iIndex] >= LOWER_CASE_ALPHA_BEGIN && pEmailID[iIndex] <= LOWER_CASE_ALPHA_END) || (pEmailID[iIndex] >= UPPER_CASE_ALPHA_BEGIN && pEmailID[iIndex] <= UPPER_CASE_ALPHA_END)) )
			{
				if (!((pEmailID[iIndex] >= NUMERIC_BEGIN && pEmailID[iIndex] <= NUMERIC_END) || pEmailID[iIndex] == '_' || pEmailID[iIndex] == '-'))
				{
					return E_FALSE;
				}
			}
		}
	}
	if(bIsAt == E_FALSE || bIsDomain == E_FALSE)
	{
		return E_FALSE;
	}
	return E_TRUE;
}

JC_BOOLEAN jmms_IsValidPhoneNumber(JC_CHAR *pPhoneNumber)
{
	JC_INT32 iIndex ;
	JC_INT32 iLength ;

	iLength = jc_tcslen(pPhoneNumber) ;
	mmi_trace(1, " phone no length is %d",  iLength);
	if (iLength < MIN_PH_NUMBER_LENGTH
		|| iLength > MAX_PH_NUMBER_LENGTH)
	{
		return E_FALSE ;
	}
	if (!((pPhoneNumber[0] >= NUMERIC_BEGIN && pPhoneNumber[0] <= NUMERIC_END) || pPhoneNumber[0] == (JC_CHAR)'+'))
	{
		return E_FALSE;
	}
	if (!(pPhoneNumber[iLength - 1] >= NUMERIC_BEGIN && pPhoneNumber[iLength-1] <= NUMERIC_END))
	{
		return E_FALSE;
	}
	for(iIndex = 0; iIndex < iLength; iIndex++)
	{
		if(pPhoneNumber[iIndex] == (JC_CHAR)'+')
		{
			if(iIndex != 0)
			{
				return E_FALSE;
			}
		}
		else if(pPhoneNumber[iIndex] == (JC_CHAR)'-')
		{
			if (!(pPhoneNumber[iIndex-1] >= NUMERIC_BEGIN && pPhoneNumber[iIndex-1] <= NUMERIC_END))
			{
				return E_FALSE;
			}
			if (!(pPhoneNumber[iIndex+1] >= NUMERIC_BEGIN && pPhoneNumber[iIndex+1] <= NUMERIC_END))
			{
				return E_FALSE;
			}
		}
		else if(!(pPhoneNumber[iIndex] >= NUMERIC_BEGIN && pPhoneNumber[iIndex] <= NUMERIC_END))
		{
			return E_FALSE;
		}
	}
	return E_TRUE;
}

void jmms_VerifyDuplicateRecipient()
{
	if (E_TRUE == isDuplicateEntry (g_pstJMmsAppInfo->pcInputBuff))
	{			
		DisplayPopup ((PU8)GetString(STR_ID_MMS_RECIPIENT_EXITS), IMG_GLOBAL_INFO,\
							1, 1000, 1); 
	}
	else
	{	
		finished_phonebook_entry();
	}
}
void jmms_VerifyDuplicateEmailRecipient()
{
	if (E_TRUE == isDuplicateEntry (g_pstJMmsAppInfo->pcInputBuff))
	{			
		DisplayPopup ((PU8)GetString(STR_ID_MMS_RECIPIENT_EXITS), IMG_GLOBAL_INFO,\
							1, 1000, 1); 
	}
	else
	{
		if(E_TRUE == g_pstAddrInfo->bIsPhonebookEntry)
		{
			if (E_TRUE == jmms_IsValidEmailID((JC_CHAR*)g_pstJMmsAppInfo->pcInputBuff))
			{
				jMms_AddressPhoneBook_entry_handler();
			}
			else
			{				
				DisplayPopup((PU8)GetString(STR_ID_MMS_ENTER_VALID_EMAIL_ID), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
			}
		}
		else
		{
			jMms_AddressPhoneBook_entry_handler();
		}	
	}
}

void jms_GoBackFromEmailEntry()
{
	g_pstAddrInfo->bIsPhonebookEMail = E_FALSE;
	GoBackHistory();
}
void jMms_AddressMailInputBox_entry_handler ()
{
	U8* guiBuffer; 
	S16 pRequired_input_modes [] = {INPUT_MODE_MULTITAP_UPPERCASE_ABC,
									INPUT_MODE_MULTITAP_LOWERCASE_ABC,
									INPUT_MODE_123,
									-1} ;
	
	EntryNewScreen (SCR_ID_MMS_ADDRESSINPUTBOX_SCREEN, NULL, jMms_AddressMailInputBox_entry_handler, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_ADDRESSINPUTBOX_SCREEN) ;

	g_pstAddrInfo->bIsPhonebookEMail = E_TRUE;
	g_pstAddrInfo->bIsPhonebookEntry = E_FALSE;
	
	ShowCategory5Screen_ext(g_pstAddrInfo->currStrId,IMG_SMS_ENTRY_SCRN_CAPTION,
							STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							(INPUT_TYPE_ALPHANUMERIC_UPPERCASE|INPUT_TYPE_ALPHANUMERIC_LOWERCASE|INPUT_TYPE_NUMERIC) , 
							jMms_getInputBuff (), jMMS_TO_INPUT_SIZE, 
							guiBuffer, 0,
							pRequired_input_modes, NULL);
	
	SetLeftSoftkeyFunction (jmms_VerifyDuplicateEmailRecipient, KEY_EVENT_UP);	
    #if defined(__MMI_GB__)  
       GBSetInputboxLSKFunction(jmms_VerifyDuplicateEmailRecipient);
    #endif
	SetCategory5RightSoftkeyFunction(jms_GoBackFromEmailEntry, KEY_EVENT_UP);
	
}

void jmms_RecipientNumberOptionsDoneHandle()
{
	jmms_VerifyDuplicateRecipient();
}

void jmms_RecipientNumberOptionsCancelHandler()
{
	if(g_pstAddrInfo->bIsPhonebookEntry)
	{
		g_pstAddrInfo->bIsPhonebookEntry = E_FALSE;
		jc_memset (jMms_getInputBuff(), 0, jMMS_TO_INPUT_SIZE);
		DeleteNHistory (1) ; //Pop out input box screen
		GoBackToHistory(SCR_ID_MMS_ADDRESSSUBMENU_SCREEN);			
	}
	else
	{
		jc_memset (jMms_getInputBuff(), 0, jMMS_TO_INPUT_SIZE);
		DeleteNHistory(1) ;
		GoBackHistory();
	}
}

void jmms_ComposeRecipientNumberOptionHltHandler (S32 index)
{
	switch(index)
	{
		case 0:
			SetLeftSoftkeyFunction (jmms_RecipientNumberOptionsDoneHandle, KEY_EVENT_UP ) ;
			break;
			
		case 1:
			SetLeftSoftkeyFunction (jmms_RecipientNumberOptionsCancelHandler, KEY_EVENT_UP ) ;
			break;

		case 2:
			SetLeftSoftkeyFunction (jMms_AddressInputPhoneBook_entry_handler, KEY_EVENT_UP ) ;
			break ;
	}	
}

void jmms_ComposeRecipientNumberLSKhandler ()
{
	U16 item_texts[] = {STR_GLOBAL_DONE, \
						STR_GLOBAL_CANCEL, \
						STR_ID_MMS_ADDR_TO_ADDNUMBER_PHONEBOOK} ;
	
	U16 item_icons[] = {IMG_GLOBAL_L1, \
						IMG_GLOBAL_L2, \
						IMG_GLOBAL_L3
						} ;
	
	EntryNewScreen (SCR_ID_MMS_RECIPIENTNUMBER_OPTION_SCREEN, 0, jmms_ComposeRecipientNumberLSKhandler, 0) ;
	SetParentHandler (SCR_ID_MMS_ADDRESSINPUTBOX_SCREEN);	
	RegisterHighlightHandler (jmms_ComposeRecipientNumberOptionHltHandler);
	ShowCategory52Screen (STR_GLOBAL_OPTIONS, IMG_SMS_ENTRY_SCRN_CAPTION, 
							STR_GLOBAL_OK, IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
							3, item_texts, item_icons,
							0, 0, 0, 0);	
	
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP ) ;
}

void jMms_AddressNumInputBox_entry_handler ()
{
	U8* guiBuffer; 
		
	EntryNewScreen (SCR_ID_MMS_ADDRESSINPUTBOX_SCREEN, NULL, \
					jMms_AddressNumInputBox_entry_handler, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_ADDRESSINPUTBOX_SCREEN) ;

	g_pstAddrInfo->bIsPhonebookEntry = E_FALSE;
	
	ShowCategory5Screen (g_pstAddrInfo->currStrId, IMG_SMS_ENTRY_SCRN_CAPTION,
						STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						INPUT_TYPE_PHONE_NUMBER|INPUT_TYPE_USE_ONLY_ENGLISH_MODES,
						(U8*)jMms_getInputBuff (), jMMS_TO_INPUT_SIZE, 
						guiBuffer);

#ifdef __MMI_GB__
	GBSetInputboxLSKFunction (jmms_ComposeRecipientNumberLSKhandler);
#endif		
	SetLeftSoftkeyFunction (jmms_ComposeRecipientNumberLSKhandler, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	RegisterInputMethodScreenCloseFunction (GoBackHistory );	

}

void jMms_AddressEditInputBox_entry_handler ()
{
	if (g_pstAddrInfo->bIsEdited)
	{
		jc_wstrcpy (jMms_getInputBuff (), g_pstAddrInfo->addr_list [(JC_INT32)g_pstAddrInfo->cSelIndex]) ;

		//if (0 == jdi_CUtilsTcsOccurenceCount ((JC_CHAR*)jMms_getInputBuff (), '@', 0) )
		if (0 == jmms_IsValidEmailID((JC_CHAR*)g_pstJMmsAppInfo->pcInputBuff))
		{
			jMms_AddressNumInputBox_entry_handler () ;
		}
		else
		{
			jMms_AddressMailInputBox_entry_handler () ;
		}
	}
}

JC_BOOLEAN isDuplicateEntry (JC_UINT8 *pcInputEntry)
{
	JC_UINT32  uiIndex = 0 ;
	JC_BOOLEAN bIsDuplicate = E_FALSE ;
	JC_BOOLEAN bIsHighLightedRecipientList = E_FALSE; // this flag is TRUE if the cuurent iteration is done on the high lighted recipeint list
	JC_CHAR **ppmEntryList = g_pstAddrInfo->addr_list_to ;

	if(g_pstAddrInfo->cHighLightedRecipientList == E_JMMS_ADDR_TO)
	{		
		bIsHighLightedRecipientList = E_TRUE;
	}
	if ((JC_NULL != pcInputEntry) && (0 < jc_tcslen (pcInputEntry)))	
	{		
		for ( ;JC_NULL != ppmEntryList && E_FALSE == bIsDuplicate; )
		{
			uiIndex = 0 ;
			while((uiIndex < JMMS_ADDR_MAX_COUNT) && (JC_NULL != ppmEntryList [uiIndex]) )
			{				
				if((g_pstAddrInfo->bIsEdited == E_TRUE)&& (bIsHighLightedRecipientList == E_TRUE) && \
					(g_pstAddrInfo->cSelIndex == uiIndex))
				{
					uiIndex++ ;
					continue;
				}
				if (0 == jc_tcscasecmp (ppmEntryList [uiIndex], (JC_CHAR *)pcInputEntry))
				{
					bIsDuplicate = E_TRUE ;
					break ;
				}
				uiIndex++ ;
			}
			bIsHighLightedRecipientList = E_FALSE;
			if (ppmEntryList == g_pstAddrInfo->addr_list_to)
			{
				ppmEntryList = g_pstAddrInfo->addr_list_cc ;
				if(g_pstAddrInfo->cHighLightedRecipientList == E_JMMS_ADDR_CC)
				{
					bIsHighLightedRecipientList = E_TRUE;
				}
			}
			else if (ppmEntryList == g_pstAddrInfo->addr_list_cc)
			{
				ppmEntryList = g_pstAddrInfo->addr_list_bcc ;
				if(g_pstAddrInfo->cHighLightedRecipientList == E_JMMS_ADDR_BCC)
				{
					bIsHighLightedRecipientList = E_TRUE;
				}
			}
			else
			{
				ppmEntryList = JC_NULL ;
			}
		}		
	}	
	return bIsDuplicate ;
}

void jmms_CopyEditBoxData()
{
	if((JC_INT32)*g_pstAddrInfo->pcCurrCount < JMMS_ADDR_MAX_COUNT)
	{
		g_pstAddrInfo->addr_list [(JC_INT32)*g_pstAddrInfo->pcCurrCount] = jdd_MemAlloc (
								jc_wstrlen (jMms_getInputBuff ()) + 1, sizeof (JC_CHAR)) ;
		jc_wstrcpy (g_pstAddrInfo->addr_list [(JC_INT32)*g_pstAddrInfo->pcCurrCount], 
														jMms_getInputBuff ()) ;
		(*g_pstAddrInfo->pcCurrCount)++ ;
		jc_memset (jMms_getInputBuff(), 0, jMMS_TO_INPUT_SIZE);	
	}
}
void finished_phonebook_entry()
{	
	if (g_pstAddrInfo->bIsEdited)
	{					
		if ((E_TRUE == jmms_IsValidEmailID((JC_CHAR*)g_pstJMmsAppInfo->pcInputBuff)) ||
			(E_TRUE == jmms_IsValidPhoneNumber((JC_CHAR*)g_pstJMmsAppInfo->pcInputBuff)))
		{
			g_pstAddrInfo->bIsEdited = E_FALSE ;
			jdd_MemFree (g_pstAddrInfo->addr_list [(JC_INT32)g_pstAddrInfo->cSelIndex]) ;
			if(jc_tcslen(g_pstJMmsAppInfo->pcInputBuff))
			{
				g_pstAddrInfo->addr_list [(JC_INT32)g_pstAddrInfo->cSelIndex] = jdd_MemAlloc (
					jc_wstrlen (jMms_getInputBuff ()) + 1, sizeof (JC_CHAR)) ;
				jc_wstrcpy (g_pstAddrInfo->addr_list [(JC_INT32)g_pstAddrInfo->cSelIndex], jMms_getInputBuff ()) ;
				jc_memset (jMms_getInputBuff(), 0, jMMS_TO_INPUT_SIZE) ;
			}
			if(g_pstAddrInfo->bIsPhonebookEMail)
			{
				g_pstAddrInfo->bIsPhonebookEMail = E_FALSE;
			}
			if(g_pstAddrInfo->bIsPhonebookEntry)
			{
				g_pstAddrInfo->bIsPhonebookEntry = E_FALSE;
				DeleteNHistory (1) ; //Pop out input box screen
				GoBackToHistory(SCR_ID_MMS_ADDRESSSUBMENU_SCREEN);			
			}
			else
			{
				DeleteNHistory (1) ; //Pop out input box screen
				GoBackHistory () ;
			}
		}				
		else
		{		
			if(E_TRUE == g_pstAddrInfo->bIsPhonebookEMail)
			{
				DisplayPopup((PU8)GetString(STR_ID_MMS_ENTER_VALID_EMAIL_ID), IMG_GLOBAL_ERROR, 1, 2000, 1);
			}
			else
			{
				DisplayPopup((PU8)GetString(STR_ID_MMS_ENTER_VALID_INPUT), IMG_GLOBAL_ERROR, 1, 2000, 1);
			}	
		}	
	}
	else
	{
		if(jc_tcslen(g_pstJMmsAppInfo->pcInputBuff))
		{
			if(g_pstAddrInfo->bIsPhonebookEntry)
			{
				if (E_TRUE == jmms_IsValidEmailID((JC_CHAR*)g_pstJMmsAppInfo->pcInputBuff) ||
					(E_TRUE == jmms_IsValidPhoneNumber((JC_CHAR*)g_pstJMmsAppInfo->pcInputBuff)))
				{
					jmms_CopyEditBoxData();	
					g_pstAddrInfo->bIsPhonebookEntry = E_FALSE;
					if(g_pstAddrInfo->bIsPhonebookEMail)
					{
						g_pstAddrInfo->bIsPhonebookEMail = E_FALSE;
					}											
					DeleteNHistory (1) ; //Pop out input box screen
					GoBackToHistory(SCR_ID_MMS_ADDRESSSUBMENU_SCREEN);
				}
				else
				{					
					DisplayPopup((PU8)GetString(STR_ID_MMS_ENTER_VALID_INPUT), \
								IMG_GLOBAL_ERROR, 1, 2000, 1);
				}					
			}
			else
			{
				if(E_TRUE == g_pstAddrInfo->bIsPhonebookEMail)
				{				
					if (E_TRUE == jmms_IsValidEmailID((JC_CHAR*)g_pstJMmsAppInfo->pcInputBuff))
					{
						jmms_CopyEditBoxData();
						g_pstAddrInfo->bIsPhonebookEMail = E_FALSE;							
						DeleteNHistory (1) ; //Pop out input box screen
						GoBackToHistory(SCR_ID_MMS_ADDRESSSUBMENU_SCREEN);
					}
					else
					{						
						DisplayPopup((PU8)GetString(STR_ID_MMS_ENTER_VALID_EMAIL_ID), \
									IMG_GLOBAL_ERROR, 1, 2000, 1);
					}							
				}
				else
				{
					if (E_TRUE == jmms_IsValidPhoneNumber((JC_CHAR*)g_pstJMmsAppInfo->pcInputBuff))
					{
						jmms_CopyEditBoxData();
						g_pstAddrInfo->bIsPhonebookEMail = E_FALSE;	
						DeleteNHistory (1) ; //Pop out input box screen
						GoBackHistory () ;
					}
					else
					{
						
                        DisplayPopup((PU8)GetString(STR_ID_MMS_ENTER_VALID_PHONE_NUMBER), \
									IMG_GLOBAL_ERROR, 1, 2000, 1);
					}
				}				
			}									
		}
		else
		{
			DeleteNHistory (1) ; //Pop out input box screen
			GoBackHistory () ;
		}			
	}			
	
}

/*External Call from mmi_phb_mms_send_data_to_mms*/
void jMms_AddressInputPhoneBookGetValue(S8* number)
{
	jc_tcscpy(g_pstJMmsAppInfo->pcInputBuff,number) ;

	if (g_pstAddrInfo->bIsPhonebookEMail)
	{
		if(!IsScreenPresent(SCR_ID_MMS_ADDRESSINPUTBOX_SCREEN))
		{
			jMms_AddressMailInputBox_entry_handler();
		}
		else
		{
			HistoryReplace(SCR_ID_MMS_ADDRESSINPUTBOX_SCREEN, SCR_ID_MMS_ADDRESSINPUTBOX_SCREEN, jMms_AddressMailInputBox_entry_handler);
			GoBackToHistory(SCR_ID_MMS_ADDRESSINPUTBOX_SCREEN);
		}
	}
	else
	{
		if(!IsScreenPresent(SCR_ID_MMS_ADDRESSINPUTBOX_SCREEN))
		{
			jMms_AddressNumInputBox_entry_handler();
		}
		else
		{
			HistoryReplace(SCR_ID_MMS_ADDRESSINPUTBOX_SCREEN, SCR_ID_MMS_ADDRESSINPUTBOX_SCREEN, jMms_AddressNumInputBox_entry_handler);
			GoBackToHistory(SCR_ID_MMS_ADDRESSINPUTBOX_SCREEN);
		}
	}
}

void jMms_AddressInputPhoneBook_entry_handler()
{
	g_pstAddrInfo->bIsPhonebookEntry = E_TRUE;
	if(g_pstAddrInfo->bIsPhonebookEMail)
		{
			EntryPhbFromMMSEmail(); 
		}
	else
		{
			EntryPhbFromMMSNumber() ;
		}
}

void jMms_AddressPhoneBook_highlight_handler (int iSelectIndex) 
{	
	switch (iSelectIndex)
	{
		case 0:
			SetLeftSoftkeyFunction (finished_phonebook_entry, KEY_EVENT_UP) ;
			break ;
		case 1:
			SetLeftSoftkeyFunction (jmms_RecipientNumberOptionsCancelHandler, KEY_EVENT_UP ) ;
			break;
		case 2:
			SetLeftSoftkeyFunction (jMms_AddressInputPhoneBook_entry_handler, KEY_EVENT_UP) ;
			break ;
	}
}

JC_CHAR* jMms_addrFormAddrBuffFromList (JC_CHAR **addr_list, JC_INT32 iCount)
{
	JC_CHAR pmTempAddrBuff [jMMS_TO_INPUT_SIZE * JMMS_ADDR_MAX_COUNT] = {0, } ;
	JC_CHAR *pmAddrBuff ;
	JC_INT32	iIndex ;

	iIndex = 0 ;
	while (iIndex < iCount)
	{
		jc_wstrcat (pmTempAddrBuff, addr_list [iIndex++]) ;
		jdi_CUtilsTcsStrCat (";", pmTempAddrBuff) ;
	}

	pmAddrBuff = jdd_MemAlloc (jc_wstrlen (pmTempAddrBuff) + 1, sizeof (JC_CHAR)) ;

	if (pmAddrBuff)
		jc_wstrcpy (pmAddrBuff, pmTempAddrBuff) ;

	return (pmAddrBuff) ;
}

/* not compeleted  */
JC_INT32	jMms_addrFormListFromAddrBuff (JC_CHAR *pmAddrBuff, JC_CHAR **addr_list)
{
	JC_CHAR 	pmTempAddrString [jMMS_TO_INPUT_SIZE + 1] = {0,} ;
	JC_INT32	iCount, iBuffIndex, iListIndex ;
	JC_INT16	ch ;

	iCount = 0 ;
	iBuffIndex = 0 ;
	iListIndex = 0 ;
	
	if (addr_list && pmAddrBuff)
	{
		ch = pmAddrBuff [iBuffIndex] ;

		while (iCount < JMMS_ADDR_MAX_COUNT)
		{			
			if ((ch == ';') || (ch == '\0'))
			{
				addr_list [iCount] = jdd_MemAlloc (jc_wstrlen (pmTempAddrString) + 1, \
											sizeof (JC_CHAR)) ;
				jc_wstrcpy (addr_list [iCount], pmTempAddrString) ;
				iCount ++ ;
				//to handle the if thebuffer is not terminatedby ';' charcter and terminatedby ';' 
				if((ch == '\0') || (pmAddrBuff [iBuffIndex+1] == '\0'))
					break;
				iListIndex = 0 ;
				jc_memset (pmTempAddrString, 0, jMMS_TO_INPUT_SIZE+1) ;
			}
			else
			{
				pmTempAddrString [iListIndex] = ch ;
				iListIndex ++ ;
			}

			iBuffIndex ++ ;
			ch = pmAddrBuff [iBuffIndex] ;
		}		
	}
	return (iCount) ;
}


void jMMS_GetMessageHeader(void)
{
	MESSAGE_HEADERS * pMsgHeaders  = JC_NULL;
	MESSAGE_SEND	*pstSendHeader = JC_NULL ;
	JC_RETCODE rCode = JC_OK ;

	rCode = jdi_MsgGetMessageHeaders(g_pstJMmsAppInfo->hCurrMsg,&pMsgHeaders) ;
	mmi_trace(1,"jdd_log: GetMessageHeader[%d] type[%d]", rCode, pMsgHeaders->eMsgType);
	if((JC_OK == rCode) && pMsgHeaders && (E_MSG_TYPE_SEND == pMsgHeaders->eMsgType))
	{	 	 
		pstSendHeader = (MESSAGE_SEND	*)pMsgHeaders->pHeaders ;
		if(pstSendHeader)
		{
			if(jc_tcslen(pstSendHeader->pTo))
			{
				g_pstJMmsAppInfo->stMmsHeader.pTo = jdi_CUtilsTcsDuplicate(JC_NULL, \
													(const JC_CHAR *)pstSendHeader->pTo) ;				
			}
			if(jc_tcslen(pstSendHeader->pCc))
			{
				g_pstJMmsAppInfo->stMmsHeader.pCc =jdi_CUtilsTcsDuplicate(JC_NULL, \
													(const JC_CHAR *)pstSendHeader->pCc );				
			}
			if(jc_tcslen(pstSendHeader->pBcc))
			{
				g_pstJMmsAppInfo->stMmsHeader.pBcc = jdi_CUtilsTcsDuplicate(JC_NULL, \
													(const JC_CHAR *)pstSendHeader->pBcc );				
			}
			
			if(jc_tcslen(pstSendHeader->pSubject))
			{
				g_pstJMmsAppInfo->stMmsHeader.pSubject = jdi_CUtilsTcsDuplicate(JC_NULL, \
												(const JC_CHAR *)pstSendHeader->pSubject) ;	
				if(jc_tcslen(g_pstJMmsAppInfo->stMmsHeader.pSubject) > (jMMS_TO_INPUT_SIZE - 1))
					*(g_pstJMmsAppInfo->stMmsHeader.pSubject + (jMMS_TO_INPUT_SIZE - 1)) = 0;
			}
		}
		
	}
	
	// fill the global strcture g_pstAddrInfo which is used in  UI of compsemain screen
	if(g_pstJMmsAppInfo->stMmsHeader.pTo || g_pstJMmsAppInfo->stMmsHeader.pCc || 
		g_pstJMmsAppInfo->stMmsHeader.pBcc || g_pstJMmsAppInfo->stMmsHeader.pSubject)
	{
		//Generate list ....
		if(jc_tcslen(g_pstJMmsAppInfo->stMmsHeader.pTo))
		{
			g_pstAddrInfo->cToCount = jMms_addrFormListFromAddrBuff ((JC_CHAR *)g_pstJMmsAppInfo->stMmsHeader.pTo, g_pstAddrInfo->addr_list_to) ;
		}
		if(jc_tcslen(g_pstJMmsAppInfo->stMmsHeader.pCc))
		{
			g_pstAddrInfo->cCcCount = jMms_addrFormListFromAddrBuff ((JC_CHAR *)g_pstJMmsAppInfo->stMmsHeader.pCc, g_pstAddrInfo->addr_list_cc) ;
		}
		if(jc_tcslen(g_pstJMmsAppInfo->stMmsHeader.pBcc))
		{
			g_pstAddrInfo->cBccCount = jMms_addrFormListFromAddrBuff ((JC_CHAR *)g_pstJMmsAppInfo->stMmsHeader.pBcc, g_pstAddrInfo->addr_list_bcc) ;
		}
	}
}


void jMms_AddressPhoneBook_entry_handler (void)
{
	U8* guiBuffer; 
	S32 number_of_items ;
	U16 *pitem_texts;
	S8 temp[10]= {0,};

	jc_memcpy (temp, GetString(STR_ID_MMS_ADDR_SUB), 10) ;	
	
	if (g_OptionStringId == STR_ID_MMS_ADDR_SUB)
	{
	  	pitem_texts = (U16 *)jdd_MemAlloc(1,sizeof(U16) ) ;
		pitem_texts[0] =  STR_ID_MMS_EDIT_DONE ;
		number_of_items = 1 ;
	}
	else
	{
		pitem_texts = (U16 *)jdd_MemAlloc(3,sizeof(U16) ) ;
		pitem_texts[0] =  STR_ID_MMS_EDIT_DONE ;
		pitem_texts[1] = STR_GLOBAL_CANCEL;
		pitem_texts[2] =  STR_ID_MMS_ADDR_TO_ADDNUMBER_PHONEBOOK ;		
				number_of_items = 3 ;
	}
	mmi_trace (1, "@@@ jMms_AddressPhoneBook_entry_handler @@@") ;

	EntryNewScreen (SCR_ID_MMS_ADDRESSPHONEBOOK_SCREEN, NULL, jMms_AddressPhoneBook_entry_handler, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_ADDRESSPHONEBOOK_SCREEN) ;
	RegisterHighlightHandler (jMms_AddressPhoneBook_highlight_handler) ;
	ShowCategory15Screen (STR_ID_MMS_NEW_MMS, IMG_SMS_ENTRY_SCRN_CAPTION, 
							STR_GLOBAL_OK, 0, 
							STR_GLOBAL_BACK, 0,
							number_of_items, pitem_texts, (PU16)gIndexIconsImageList, 
							1, 0, guiBuffer);

	jdd_MemFree(pitem_texts) ;		
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
}

void jMms_addrListOpt_delete_handler ()
{
	JC_INT32 	iIndex ;

	iIndex = g_pstAddrInfo->cSelIndex ;

	mmi_trace (1, "@@@ Deleting %d @@@", iIndex) ;
		
	jdd_MemFree (g_pstAddrInfo->addr_list [iIndex]) ;

	(*g_pstAddrInfo->pcCurrCount) -- ;

	for (; iIndex < *(g_pstAddrInfo->pcCurrCount); iIndex++)
	{
		g_pstAddrInfo->addr_list [iIndex] = g_pstAddrInfo->addr_list [iIndex + 1] ;
	}
	g_pstAddrInfo->addr_list [(JC_INT32)*(g_pstAddrInfo->pcCurrCount)] = JC_NULL ;

	GoBackHistory () ;
}


void jMms_addrListOpt_deleteAll_handler ()
{
	JC_INT32	iIndex ;

	mmi_trace (1, "@@@ Deleting all @@@") ;
	
	for (iIndex = 0; iIndex < *(g_pstAddrInfo->pcCurrCount); iIndex++) 
	{
		jdd_MemFree (g_pstAddrInfo->addr_list [iIndex]) ;
		g_pstAddrInfo->addr_list [iIndex] = JC_NULL ;
	}

	*(g_pstAddrInfo->pcCurrCount) = 0 ;
	GoBackHistory () ;
}

void jmms_RecipientNumberEntryHandler()
{
	if((JC_INT32)*g_pstAddrInfo->pcCurrCount >= JMMS_ADDR_MAX_COUNT)
	{
		DisplayPopup((PU8)GetString(STR_ID_MMS_MAX_RECIPIENT_ADDED), IMG_GLOBAL_INFO, 
										1,	2000, 1);
	}
	else
	{
		jMms_AddressNumInputBox_entry_handler();
	}
}

void jmms_RecipientEMailEntryHandler()
{
	if((JC_INT32)*g_pstAddrInfo->pcCurrCount >= JMMS_ADDR_MAX_COUNT)
	{
		DisplayPopup((PU8)GetString(STR_ID_MMS_MAX_RECIPIENT_ADDED), IMG_GLOBAL_INFO, 
										1,	2000, 1);
	}
	else
	{
		jMms_AddressMailInputBox_entry_handler();
	}
}

void jMms_AddressSubMenu_highlight_handler (int iSelectIndex) 
{	
	g_pstAddrInfo->bIsEdited = E_FALSE ;
	switch (iSelectIndex)
	{
		case E_JMMS_ADDROPT_ADD_NUMBER:			
			SetLeftSoftkeyFunction (jmms_RecipientNumberEntryHandler, KEY_EVENT_UP) ;			
			g_pstAddrInfo->currStrId = STR_ID_MMS_ADDR_TO_ADDNUMBER ;
			break ;
		
		case E_JMMS_ADDROPT_ADD_EMAIL:
			SetLeftSoftkeyFunction (jmms_RecipientEMailEntryHandler, KEY_EVENT_UP) ;
			g_pstAddrInfo->currStrId = STR_ID_MMS_ADDR_TO_ADDEMAIL ;
			break ;

		case E_JMMS_ADDROPT_EDIT:
			g_pstAddrInfo->bIsEdited = E_TRUE ;
			mmi_trace (1, "@@@ %d set to Edit @@@", g_pstAddrInfo->cSelIndex) ;
			SetLeftSoftkeyFunction (jMms_AddressEditInputBox_entry_handler, KEY_EVENT_UP) ;
			g_pstAddrInfo->currStrId = STR_GLOBAL_EDIT ;
			break ;

		case E_JMMS_ADDROPT_DELETE:
			SetLeftSoftkeyFunction (jMms_addrListOpt_delete_handler, KEY_EVENT_UP) ;
			g_pstAddrInfo->currStrId = STR_GLOBAL_DELETE ;
			break ;
			
		case E_JMMS_ADDROPT_DELETE_ALL:
			SetLeftSoftkeyFunction (jMms_addrListOpt_deleteAll_handler, KEY_EVENT_UP) ;
			g_pstAddrInfo->currStrId = STR_GLOBAL_DELETE_ALL ;
			break ;
			
		default:
			break ;
	}
}

void jMms_AddrOption_Goback(void)
{
	if(*g_pstAddrInfo->pcCurrCount == 0)
		GoBackToHistory(SCR_ID_MMS_ADDRESS_SCREEN);
	else
		GoBackHistory();
}

void jMms_addrOptions_entry_handler ()
{	
	INT	iCount = 0;
	U16 item_texts[E_JMMS_ADDROPT_MENU_COUNT] ;	

	item_texts [E_JMMS_ADDROPT_ADD_NUMBER] = STR_ID_MMS_ADDR_TO_ADDNUMBER ;
	iCount++ ;
	item_texts [E_JMMS_ADDROPT_ADD_EMAIL] = STR_ID_MMS_ADDR_TO_ADDEMAIL ;
	iCount++ ;

	if (*g_pstAddrInfo->pcCurrCount > 0)
	{
		item_texts [E_JMMS_ADDROPT_DELETE] = STR_ID_MMS_DELETE ;
		iCount ++ ;
		item_texts [E_JMMS_ADDROPT_EDIT] = STR_GLOBAL_EDIT ;
		iCount ++ ;
	}
	if (*g_pstAddrInfo->pcCurrCount > 1)
	{
		item_texts [E_JMMS_ADDROPT_DELETE_ALL] = STR_ID_MMS_DELETE_ALL ;
		iCount ++ ;
	}
	
	g_pstAddrInfo->bIsPhonebookEntry = E_FALSE;
	g_pstAddrInfo->bIsPhonebookEMail = E_FALSE; 
		
	EntryNewScreen (SCR_ID_MMS_ADDRESSSUBMENUOPT_SCREEN, NULL, jMms_addrOptions_entry_handler, NULL) ;
	RegisterHighlightHandler (jMms_AddressSubMenu_highlight_handler) ;
	ShowCategory15Screen (STR_ID_MMS_NEW_MMS, IMG_SMS_ENTRY_SCRN_CAPTION, 
						STR_GLOBAL_OK, 0, 
						STR_GLOBAL_BACK, 0,
						iCount, item_texts, (PU16)gIndexIconsImageList, 
						1, 0, 0);
	SetKeyHandler(jMms_AddrOption_Goback, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	SetRightSoftkeyFunction (jMms_AddrOption_Goback, KEY_EVENT_UP);	
}

void jMms_addrList_highlight_handler (INT iIndex)
{
	g_pstAddrInfo->cSelIndex = iIndex ;
	mmi_trace (1, "@@@ HighLighted %d @@@", g_pstAddrInfo->cSelIndex) ;
}
void jMms_AddressSubMenu_entry_handler (void)
{
	U8				*guiBuffer;

	EntryNewScreen (SCR_ID_MMS_ADDRESSSUBMENU_SCREEN, NULL, jMms_AddressSubMenu_entry_handler, NULL) ;
	if(*g_pstAddrInfo->pcCurrCount == 0)
	{	
		jMms_addrOptions_entry_handler();
	}
	else
	{
		guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_ADDRESSSUBMENU_SCREEN) ;
		RegisterHighlightHandler (jMms_addrList_highlight_handler) ;
		ShowCategory84Screen(STR_ID_MMS_NEW_MMS, IMG_SMS_ENTRY_SCRN_CAPTION,
							 STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS, 
							 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						     *g_pstAddrInfo->pcCurrCount,
						     (U8**)g_pstAddrInfo->addr_list,
							 (PU16)gIndexIconsImageList, LIST_MENU, 0, guiBuffer);
	
		SetLeftSoftkeyFunction(jMms_addrOptions_entry_handler, KEY_EVENT_UP);
		SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	}
}

void jMms_Address_highlight_handler (int iSelectIndex) 
{
	mmi_trace (1, "@@@ jMms_Address_highlight_handler Index: %d @@@", iSelectIndex) ;
	jc_memset(jMms_getInputBuff(),0,255 ) ;
	g_OptionStringId =  0 ;
	switch (iSelectIndex)
	{
	case E_JMMS_ADDR_TO:
		ChangeTitle (IMG_SMS_ENTRY_SCRN_CAPTION, GetString (STR_ID_MMS_ADDR_TO)) ;		
		g_pstAddrInfo->pcCurrCount = &g_pstAddrInfo->cToCount ;
		g_pstAddrInfo->currStrId = STR_ID_MMS_ADDR_TO ;
		g_pstAddrInfo->addr_list = g_pstAddrInfo->addr_list_to ;
		g_pstAddrInfo->cHighLightedRecipientList = E_JMMS_ADDR_TO;
		SetLeftSoftkeyFunction (jMms_AddressSubMenu_entry_handler, KEY_EVENT_UP) ;
		break ;
		
	case E_JMMS_ADDR_CC:
		ChangeTitle (IMG_SMS_ENTRY_SCRN_CAPTION, GetString (STR_ID_MMS_ADDR_CC)) ;
		g_pstAddrInfo->pcCurrCount = &g_pstAddrInfo->cCcCount ;
		g_pstAddrInfo->currStrId = STR_ID_MMS_ADDR_CC ;
		g_pstAddrInfo->cHighLightedRecipientList = E_JMMS_ADDR_CC;
		g_pstAddrInfo->addr_list = g_pstAddrInfo->addr_list_cc ;
		SetLeftSoftkeyFunction (jMms_AddressSubMenu_entry_handler, KEY_EVENT_UP) ;
		break ;
		
	case E_JMMS_ADDR_BCC:
		ChangeTitle (IMG_SMS_ENTRY_SCRN_CAPTION, GetString (STR_ID_MMS_ADDR_BCC)) ;
		g_pstAddrInfo->pcCurrCount = &g_pstAddrInfo->cBccCount ;
		g_pstAddrInfo->currStrId = STR_ID_MMS_ADDR_BCC ;
		g_pstAddrInfo->cHighLightedRecipientList = E_JMMS_ADDR_BCC;
		g_pstAddrInfo->addr_list = g_pstAddrInfo->addr_list_bcc ;
		SetLeftSoftkeyFunction (jMms_AddressSubMenu_entry_handler, KEY_EVENT_UP) ;
		break ;	
		
	case E_JMMS_ADDR_SUB:
		ChangeTitle (IMG_SMS_ENTRY_SCRN_CAPTION, GetString (STR_ID_MMS_ADDR_SUB)) ;
		g_OptionStringId =  STR_ID_MMS_ADDR_SUB ;
		if(0 < jc_tcslen(g_pstJMmsAppInfo->stMmsHeader.pSubject))
		{
			mmi_trace(1 , "Subject Length %d", jc_wstrlen(g_pstJMmsAppInfo->stMmsHeader.pSubject));
			jc_tcsncpy((JC_CHAR*)jMms_getInputBuff (),\
					g_pstJMmsAppInfo->stMmsHeader.pSubject, (jMMS_TO_INPUT_SIZE-1)) ;
		}
		SetLeftSoftkeyFunction (jmms_ComposeSubEntryhandler, KEY_EVENT_UP) ;
		break ;
		
	case E_JMMS_INSERT_CONTENT:
		ChangeTitle (IMG_SMS_ENTRY_SCRN_CAPTION, GetString (STR_ID_MMS_ADDR_INSERTCONTENT)) ;			
		SetLeftSoftkeyFunction (jmms_EditContentEntryHandler, KEY_EVENT_UP) ;
		break ;	

	case E_JMMS_CONTENT_MODE:
		ChangeTitle (IMG_SMS_ENTRY_SCRN_CAPTION, GetString (STR_ID_MMS_CREATE_MODE)) ;
		SetLeftSoftkeyFunction (jMms_compose_createMode_handler, KEY_EVENT_UP) ;
		break ;

	case E_JMMS_CONTENT_CLASS:
		ChangeTitle (IMG_SMS_ENTRY_SCRN_CAPTION, GetString (STR_ID_MMS_CONTENT_CLASS)) ;
		SetLeftSoftkeyFunction (jMms_compose_contentClass_handler, KEY_EVENT_UP) ;
		break ;
/*
	case E_JMMS_MESSAGE_SAVE:
		ChangeTitle (0, GetString (STR_GLOBAL_SAVE)) ;
		ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_GLOBAL_OK) ;
		SetLeftSoftkeyFunction (jMms_saveMsgHandler, KEY_EVENT_UP) ;
		break ;
*/
	default:
		break ;
	}
	draw_title () ;
}

void jmms_FreeMemoryReciepientList()
{
	JC_INT32 i = 0;
	
	if(g_pstAddrInfo && g_pstAddrInfo->item_texts)
    {
	    for( i =0 ; i < E_JMMS_ADDR_MENU_COUNT ; i++)
	    {
	       if(g_pstAddrInfo->item_texts[i])
	       	{
	    		jdd_MemFree(g_pstAddrInfo->item_texts[i]);
	       	}
	    }		
    }
}
//MK : function forming the ENTER_ADRESS_SCREEN , dynamic menus
void jMms_addrFormMenuList ()
{
	JC_CHAR		amTemp [50] ;
	JC_INT8		acTemp [10] ;
	
	// Free previously allocated memory
	jmms_FreeMemoryReciepientList();	
	
	/* Form To field */
	jc_memset (amTemp, 0, sizeof (amTemp)) ;
	jc_memset (acTemp, 0, sizeof (acTemp)) ;
	
	jc_wstrcpy (amTemp, GetString (STR_ID_MMS_ADDR_TO)) ;
	if(g_pstAddrInfo->cToCount !=1)
	{
		jc_sprintf (acTemp, " [%d]", g_pstAddrInfo->cToCount) ;
	}
	else
	{
		jdi_CUtilsTcsStrCat(" [", amTemp);
		jc_wstrcat(amTemp, g_pstAddrInfo->addr_list_to[0]);
		jdi_CUtilsTcsStrCat("]", amTemp);
	}

	jdi_CUtilsTcsStrCat (acTemp, amTemp) ;

	g_pstAddrInfo->item_texts [E_JMMS_ADDR_TO] = \
		jdd_MemAlloc (jc_wstrlen (amTemp) + 1, sizeof (JC_CHAR)) ;
	jc_wstrcpy (g_pstAddrInfo->item_texts [E_JMMS_ADDR_TO], amTemp) ;

	/* Form CC field*/
	jc_memset (amTemp, 0, sizeof (amTemp)) ;
	jc_memset (acTemp, 0, sizeof (acTemp)) ;
	
	jc_wstrcpy (amTemp, GetString (STR_ID_MMS_ADDR_CC)) ;
	if(g_pstAddrInfo->cCcCount != 1)
	{
		jc_sprintf (acTemp, " [%d]", g_pstAddrInfo->cCcCount) ;
	}
	else
	{
		jdi_CUtilsTcsStrCat (" [", amTemp);
		jc_wstrcat(amTemp,g_pstAddrInfo->addr_list_cc[0]);
		jdi_CUtilsTcsStrCat("]",amTemp);
	}

	jdi_CUtilsTcsStrCat (acTemp, amTemp) ;

	g_pstAddrInfo->item_texts [E_JMMS_ADDR_CC] = jdd_MemAlloc (jc_wstrlen (amTemp) + 1, sizeof (JC_CHAR)) ;
	jc_wstrcpy (g_pstAddrInfo->item_texts [E_JMMS_ADDR_CC], amTemp) ;

	/* Form BCC field*/
	jc_memset (amTemp, 0, sizeof (amTemp)) ;
	jc_memset (acTemp, 0, sizeof (acTemp)) ;
	
	jc_wstrcpy (amTemp, GetString (STR_ID_MMS_ADDR_BCC)) ;
	if(g_pstAddrInfo->cBccCount != 1)
	{
		jc_sprintf (acTemp, " [%d]", g_pstAddrInfo->cBccCount) ;
	}
	else
	{
		jdi_CUtilsTcsStrCat(" [", amTemp);
		jc_wstrcat(amTemp, g_pstAddrInfo->addr_list_bcc[0]);
		jdi_CUtilsTcsStrCat("]", amTemp);
	}

	jdi_CUtilsTcsStrCat (acTemp, amTemp) ;

	g_pstAddrInfo->item_texts [E_JMMS_ADDR_BCC] = jdd_MemAlloc (jc_wstrlen (amTemp) + 1, sizeof (JC_CHAR)) ;
	jc_wstrcpy (g_pstAddrInfo->item_texts [E_JMMS_ADDR_BCC], amTemp) ;
	

	/* Form Subject field */
	jc_memset (amTemp, 0, sizeof (amTemp)) ;
	jc_wstrncpy (amTemp, GetString(STR_ID_MMS_ADDR_SUB), \
							jc_wstrlen(GetString(STR_ID_MMS_ADDR_SUB))) ;	

	g_pstAddrInfo->item_texts [E_JMMS_ADDR_SUB] = jdd_MemAlloc \
				((jc_wstrlen(amTemp)+jMMS_TO_INPUT_SIZE), sizeof (JC_CHAR)) ;
	
	jc_wstrcpy (g_pstAddrInfo->item_texts [E_JMMS_ADDR_SUB], amTemp) ;
	if(jc_tcslen(g_pstJMmsAppInfo->stMmsHeader.pSubject))
	{
		mmi_trace(1, " lenght [%d]" ,jc_wstrlen(g_pstJMmsAppInfo->stMmsHeader.pSubject));
		jc_wstrncat(g_pstAddrInfo->item_texts [E_JMMS_ADDR_SUB], \
				g_pstJMmsAppInfo->stMmsHeader.pSubject,	(jMMS_TO_INPUT_SIZE-1));
	}	

	/* Insert Content */
	jc_memset (amTemp, 0, sizeof (amTemp)) ;		
	jc_wstrcpy (amTemp, GetString (STR_ID_MMS_ADDR_INSERTCONTENT)) ;
	g_pstAddrInfo->item_texts [E_JMMS_INSERT_CONTENT] = jdd_MemAlloc (jc_wstrlen (amTemp) + 1, sizeof (JC_CHAR)) ;
	jc_wstrcpy (g_pstAddrInfo->item_texts [E_JMMS_INSERT_CONTENT], amTemp) ;
	
    /* Create Mode */
	jc_memset (amTemp, 0, sizeof (amTemp)) ;
	jc_wstrcpy (amTemp, GetString (STR_ID_MMS_CREATE_MODE)) ;
	g_pstAddrInfo->item_texts [E_JMMS_CONTENT_MODE] = \
		jdd_MemAlloc (jc_wstrlen (amTemp) + 1, sizeof (JC_CHAR)) ;
	jc_wstrcpy (g_pstAddrInfo->item_texts [E_JMMS_CONTENT_MODE], amTemp) ;

	/* Content Class */
	jc_memset (amTemp, 0, sizeof (amTemp)) ;
	jc_wstrcpy (amTemp, GetString (STR_ID_MMS_CONTENT_CLASS)) ;
	g_pstAddrInfo->item_texts [E_JMMS_CONTENT_CLASS] = \
		jdd_MemAlloc (jc_wstrlen (amTemp) + 1, sizeof (JC_CHAR)) ;
	jc_wstrcpy (g_pstAddrInfo->item_texts [E_JMMS_CONTENT_CLASS], amTemp) ;

	/* Save to Draft */
	/*
	jc_memset (amTemp, 0, sizeof (amTemp)) ;
	jc_wstrcpy (amTemp, GetString (STR_GLOBAL_SAVE)) ;
	g_pstAddrInfo->item_texts [E_JMMS_MESSAGE_SAVE] = \
		jdd_MemAlloc (jc_wstrlen (amTemp) + 1, sizeof (JC_CHAR)) ;
	jc_wstrcpy (g_pstAddrInfo->item_texts [E_JMMS_MESSAGE_SAVE], \
		GetString (STR_GLOBAL_SAVE)) ;
		*/
	
}

void jmms_MMIGotoMMSMainScreen()
{
	jmms_MMIComposeDeInit();	
	GoBackHistory();
}

void jmms_DeInitAddrInfo()
{
 	JC_UINT16 i;
	if( g_pstAddrInfo )
	{    
	    for( i =0 ; i < JMMS_ADDR_MAX_COUNT ; i++)
	    {
	       if(g_pstAddrInfo->addr_list_to[i])
	       	{
	    		jdd_MemFree(g_pstAddrInfo->addr_list_to[i]);
	       	}
	    }
	    
	  	for( i =0 ; i < JMMS_ADDR_MAX_COUNT ; i++)
	    {
	       if(g_pstAddrInfo->addr_list_cc[i])
	       	{
	    		jdd_MemFree(g_pstAddrInfo->addr_list_cc[i]);
	       	}
	    }
	    
		for( i =0 ; i < JMMS_ADDR_MAX_COUNT ; i++)
	    {
	       if(g_pstAddrInfo->addr_list_bcc[i])
	       	{
	    		jdd_MemFree(g_pstAddrInfo->addr_list_bcc[i]);
	       	}
	    }	    
		
		for( i =0 ; i < E_JMMS_ADDR_MENU_COUNT ; i++)
	    {
	       if(g_pstAddrInfo->item_texts[i])
	       	{
	    		jdd_MemFree(g_pstAddrInfo->item_texts[i]);
	       	}
	    }
     jdd_MemFree(g_pstAddrInfo);
	 g_pstAddrInfo = NULL;
	}
 
}

void jmms_MMIComposeDeInit()
{
     //Destory MMS Main Window
	//jmms_MMIDestoryMMSMainWindow();
   	mmi_trace(g_sw_MMS, "JDD_LOG: jmms_MMIComposeDeInit");
 	if (g_pstJMmsAppInfo->hCurrMsg)
	{
		jdi_MsgDeInit(g_pstJMmsAppInfo->hCurrMsg) ;
		g_pstJMmsAppInfo->hCurrMsg = JC_NULL ;
	}
	if(g_pstJMmsAppInfo)
	{	
		if(g_pstJMmsAppInfo->pcInputBuff)
		{
			jc_memset(g_pstJMmsAppInfo->pcInputBuff,0,255) ;
		}

		g_pstJMmsAppInfo->ActivePageIndex = 0;
		jc_memset ( g_pstJMmsAppInfo->ActivePageMediaIds , 0 , 
			sizeof (g_pstJMmsAppInfo->ActivePageMediaIds) );  
		jc_memset(g_pstJMmsAppInfo->pActiveTextBuffer,0,
				sizeof(JC_CHAR)*(JMMS_MAX_TEXT_PER_SLIDE+1));
		//jc_memset(&g_pstJMmsAppInfo->stMMSSetting,0,sizeof(JMMS_SETTINGS)) ;
		
		jdd_MemFree(g_pstJMmsAppInfo->stMmsHeader.pTo);
		g_pstJMmsAppInfo->stMmsHeader.pTo = JC_NULL;
		jdd_MemFree(g_pstJMmsAppInfo->stMmsHeader.pCc);
		g_pstJMmsAppInfo->stMmsHeader.pCc = JC_NULL;
		jdd_MemFree(g_pstJMmsAppInfo->stMmsHeader.pBcc);
		g_pstJMmsAppInfo->stMmsHeader.pBcc = JC_NULL;
		jdd_MemFree(g_pstJMmsAppInfo->stMmsHeader.pSubject);		
		jc_memset(&g_pstJMmsAppInfo->stMmsHeader,0,sizeof(JMMS_HEADER) ) ;

		jmms_DeInitAddrInfo();
		jdd_MemFree(g_pstAddrInfo);
		g_pstAddrInfo = NULL;
		g_pstJMmsAppInfo->bIsAutoSignatureAdded = E_FALSE;
	}

}

JC_RETCODE jmms_PrepareCurrSlideForDisplay( void ) 
{
	JC_RETCODE	eRet			= JC_OK ;
	MEDIA_INFO	*pMediaInfo		= JC_NULL ;
	JC_UINT8	ucMediaCount	= 0 ;
	JC_UINT8	ucPageIndex		= 0 ;
	JC_UINT32	uiMediaID		= 0 ;
	MEDIA_INFO	*pTempMediaList = JC_NULL ;
    JC_UINT32   strLenth = 0;

       
	ucPageIndex = g_pstJMmsAppInfo->ActivePageIndex ;
	
	eRet = jdi_MsgGetMediaList ( g_pstJMmsAppInfo->hCurrMsg, ucPageIndex , &pMediaInfo , 
									&ucMediaCount, E_FALSE) ;
	
	jc_memset ( g_pstJMmsAppInfo->ActivePageMediaIds , 0 , sizeof (g_pstJMmsAppInfo->ActivePageMediaIds) );     
    jc_memset(g_pstJMmsAppInfo->pActiveTextBuffer,0,sizeof(JC_CHAR)*(JMMS_MAX_TEXT_PER_SLIDE+1));
	pTempMediaList = pMediaInfo;

	while(pTempMediaList)
	{
        uiMediaID = pTempMediaList->eMediaType;
              
		g_pstJMmsAppInfo->ActivePageMediaIds [uiMediaID] = pTempMediaList->uiMediaID ;
        
		if (uiMediaID == E_MEDIA_TEXT)
		{	
            if(pTempMediaList->pMimeInfo->ContentData.pBuffer != JC_NULL)
            {
                strLenth = pTempMediaList->pMimeInfo->ContentData.uiTotalSize;
                
        		if (strLenth > JMMS_MAX_TEXT_PER_SLIDE*sizeof(JC_CHAR))
        		{
        			strLenth = JMMS_MAX_TEXT_PER_SLIDE*sizeof(JC_CHAR);
        		}                                     
               	jc_memcpy((JC_UINT8 *)g_pstJMmsAppInfo->pActiveTextBuffer,
					pTempMediaList->pMimeInfo->ContentData.pBuffer,	strLenth);
			}                  
		}		
		pTempMediaList = pTempMediaList->pNext ;
	}
   
	return eRet ;
}


void jMms_Write_Msg_entry_handler(void)
{
	g_pstAddrInfo = NULL ;
	g_pstJMmsAppInfo->eCurrentFolderID = E_JMMS_NONE_FOLDER ;
	jMms_msgComposeInit ();	
	jmms_UpdateMessageHeaders();
	jmms_initaddrInfo();
	jmms_ComposeMainScreenEntryHandler();
}

void jmms_initaddrInfo()
{
	g_pstAddrInfo = jdd_MemAlloc (1, sizeof (ST_JMMS_ADDR_INFO)) ; 
}

BOOL jmms_ComposeWithModification(void)
{
	JC_UINT8 ucTotalPages = 0;
	JC_UINT32 msgSize = 0;
	
	//content
	jdi_MsgGetTotalPages (g_pstJMmsAppInfo->hCurrMsg, &ucTotalPages);
	if(ucTotalPages != 0)
	{
		return TRUE;
	}
	//size
	jdi_MsgGetSize(g_pstJMmsAppInfo->hCurrMsg, 0, &msgSize);
	if(msgSize != 0)
	{
		return TRUE;
	}
	
	//To, CC, BCC
	if(g_pstAddrInfo->cToCount || g_pstAddrInfo->cCcCount ||g_pstAddrInfo->cBccCount)
	{
		return TRUE;
	}
	//subject
	if(jc_tcslen(g_pstJMmsAppInfo->stMmsHeader.pSubject))
	{
		return TRUE;
	}
	return FALSE;
}

void jmms_ComposeMainScreenEntryHandler (void)
{	
	if (g_pstAddrInfo)
	{
		U8* guiBuffer; 
		g_OptionStringId = 0;		
		jMms_addrFormMenuList () ;	
			
		EntryNewScreen (SCR_ID_MMS_ADDRESS_SCREEN, NULL, jmms_ComposeMainScreenEntryHandler, NULL) ;
		guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_ADDRESS_SCREEN) ;	
		RegisterHighlightHandler (jMms_Address_highlight_handler) ;
		SetParentHandler(0);
		ShowCategory84Screen(STR_ID_MMS_ADDR_TO, IMG_SMS_ENTRY_SCRN_CAPTION,
			                 STR_GLOBAL_OK, IMG_GLOBAL_OK,
			                 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
			                 E_JMMS_ADDR_MENU_COUNT,
			                 (U8**)g_pstAddrInfo->item_texts,
			   				 (PU16)gIndexIconsImageList,
			   				 LIST_MENU, 0, guiBuffer);
		if(jmms_ComposeWithModification())
		{
			ChangeRightSoftkey(STR_GLOBAL_DONE, 0);
			SetKeyHandler(jmms_ComposeDoneOption, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
			SetRightSoftkeyFunction(jmms_ComposeDoneOption, KEY_EVENT_UP);
		}
		else
		{
			ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
			SetKeyHandler(jmms_MMIGotoMMSMainScreen, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
			SetRightSoftkeyFunction (jmms_MMIGotoMMSMainScreen, KEY_EVENT_UP) ;		
		}
	}
}

void jMMS_SendFromApp(S8* number)
{
    JC_RETCODE rCode ;

    if (E_FALSE == jmms_IsValidPhoneNumber((JC_CHAR*)number))
    {
        DisplayPopup((PU8)GetString(STR_ID_MMS_ENTER_VALID_PHONE_NUMBER), \
        	IMG_GLOBAL_ERROR, 1, 2000, 1);
        
        return;
    }
    
    if (JC_NULL == g_pstJMmsAppInfo->hMsgStore)
    {
    	rCode = jdi_StoreInit (&g_pstJMmsAppInfo->hMsgStore, 0) ;
    	mmi_trace (1, "JDD_LOG: jmms_MMSEntryHandler::jdi_StoreInit [%X]", -rCode) ;
    }
    jMms_createMmsViewWindow();
    jMMS_SetMMSAppStatus(E_TRUE);	
    jmms_ResetGlobalVal();
    g_pstAddrInfo = NULL ;
    g_pstJMmsAppInfo->eCurrentFolderID = E_JMMS_NONE_FOLDER ;
    jMms_msgComposeInit ();	
    jmms_UpdateMessageHeaders();
    jmms_initaddrInfo();           
    g_pstAddrInfo->pcCurrCount = &g_pstAddrInfo->cToCount ;
    g_pstAddrInfo->addr_list = g_pstAddrInfo->addr_list_to ;
    if (E_TRUE == jmms_IsValidPhoneNumber((JC_CHAR*)number))
    {
        pfnUnicodeStrcpy( g_pstJMmsAppInfo->pcInputBuff, number);
        jmms_CopyEditBoxData();
    }
    
    SetHiliteHandler(MMS_EXIT_COMPOSE, jMms_Highlight_App_Exit_Compose);
    SetDelScrnIDCallbackHandler(SCR_ID_MMS_ADDRESS_SCREEN,jmms_ApplicationExitHandler); 
    jmms_ComposeMainScreenEntryHandler();
}

UINT8 entry_from_image_view = 0;//E_TRUE if entry MMS from image view menu
void jmms_content_insert_hdlr(char * content_source)
{
    JC_RETCODE rCode ;

    if (NULL == content_source)
    {
        DisplayPopup((PU8)GetString(STR_ID_MMS_ENTER_VALID_INPUT), \
        	IMG_GLOBAL_ERROR, 1, 2000, 1);
        
        return;
    }
    
    if (JC_NULL == g_pstJMmsAppInfo->hMsgStore)
    {
    	rCode = jdi_StoreInit (&g_pstJMmsAppInfo->hMsgStore, 0) ;
    	mmi_trace (1, "JDD_LOG: jmms_MMSEntryHandler::jdi_StoreInit [%X]", -rCode) ;
    }
    jMms_createMmsViewWindow();
    jMMS_SetMMSAppStatus(E_TRUE);	
    jmms_ResetGlobalVal();
    g_pstAddrInfo = NULL ;
    g_pstJMmsAppInfo->eCurrentFolderID = E_JMMS_NONE_FOLDER ;
    jMms_msgComposeInit ();	
    jmms_UpdateMessageHeaders();
    jmms_initaddrInfo();           
	#if 0
    g_pstAddrInfo->pcCurrCount = &g_pstAddrInfo->cToCount ;
    g_pstAddrInfo->addr_list = g_pstAddrInfo->addr_list_to ;
    if (E_TRUE == jmms_IsValidPhoneNumber((JC_CHAR*)number))
    {
        pfnUnicodeStrcpy( g_pstJMmsAppInfo->pcInputBuff, number);
        jmms_CopyEditBoxData();
    }
    #endif

    SetHiliteHandler(MMS_EXIT_COMPOSE, jMms_Highlight_App_Exit_Compose);
    SetDelScrnIDCallbackHandler(SCR_ID_MMS_ADDRESS_SCREEN,jmms_ApplicationExitHandler); 
    jmms_ComposeMainScreenEntryHandler();
    jMms_AddNewSlide_entry_handler();
    entry_from_image_view = 1;
    MediaPickerImageCallback(content_source,0);
    entry_from_image_view = 0;
}


#endif //__JATAAYU_APP__

