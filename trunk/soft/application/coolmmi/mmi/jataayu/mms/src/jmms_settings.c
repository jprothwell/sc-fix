#define MODULE_NAME "Jmms"
#define FILE_NAME "jmms_settings.c"
 /***************************************************************************
 *
 * File Name : jmms_settings.c
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
 * @file		jmms_settings.c
 * @ingroup		jMMS
 * @brief		Describes the MMS settings APIs.
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
#include "jmms_settings.h"


#include "phonebookgprot.h"
#include "jmms_struct_list.h"
#include "custom_nvram_editor_data_item.h"
#include "dataaccountgprot.h"
#include "jprofilehandlertypes.h"
#include "jprofilehandlerprots.h"
#include "fs.h"
#include "messagesresourcedata.h"
#ifdef  __MMI_MULTI_SIM__
#include "dual_sim.h"
#endif

 /***************************************************************************
 * Macros
 **************************************************************************/

/***************************************************************************
* Global variables declarations
**************************************************************************/
extern void GBSetInputboxLSKFunction(FuncPtr f);
static JC_BOOLEAN g_bIsProfileChanged ;
global_context_struct g_setting_context;

/***************************************************************************
* Local Function Prototypes
***************************************************************************/

/***************************************************************************
* All Local Function Definitions
***************************************************************************/

/***************************************************************************
* All Global Function Definitions
**************************************************************************/

void jMms_server_profile_entry_handler()
{
//	jMMS_SettinginitProfiles () ;
	mmi_ph_pre_entry_show_profile_list_menu(PH_MMS_PROFILE);
	g_bIsProfileChanged  = E_TRUE ;
}

JC_RETCODE  jmms_WriteToSettingsFile ()
{
	JC_CHAR *pmMMSSettngsFileName = JC_NULL ;
	JDD_FILE hSettFile ;
	JC_RETCODE  rCode = JC_OK ;	 

	rCode = jdi_CUtilsCharToTchar (JC_NULL, JMMS_SETTINGS_FILE, &pmMMSSettngsFileName) ;
	if (pmMMSSettngsFileName)
	{
		hSettFile = jdd_FSOpen (JC_NULL, pmMMSSettngsFileName, E_CREATE_WRITE_MODE) ;
		if (hSettFile)
		{
			mmi_trace(1,"Jdd_log: jmms_WriteToSettingsFile fs open success");
			rCode = jdd_FSWrite ((void *)&(g_pstJMmsAppInfo->stMMSSetting), \
								sizeof (JMMS_SETTINGS), 1, hSettFile) ;
			rCode = jdd_FSClose (hSettFile) ;
		}
		else
		{
			mmi_trace(1,"Jdd_log: jmms_WriteToSettingsFile fs open fail");
			rCode = JC_ERR_FILE_OPEN ;
		}
		jdd_MemFree (pmMMSSettngsFileName) ;
	}
	else
	{
		mmi_trace(1,"JDD_LOG: jmms_WriteToSettingsFile Memory Allocation Error");
		rCode = JC_ERR_MEMORY_ALLOCATION ;
	}
	mmi_trace (1, "@@@ jMMs_updateSettingsFile returned: %x", JRET(rCode)) ;
	return (rCode) ;
}

void jMms_settings_Exit_handler()
{
// The updation of data connection profile need not be done for dual SIM , as the 
// profile is always updated in send/recv

	if(g_bIsProfileChanged )
	{
#ifndef __MMI_MULTI_SIM__
		jMMS_SettingreadUpdateDataConnInfo () ;
#endif
		g_bIsProfileChanged = E_FALSE ;
	}

	jMMS_Setting_deInitProfiles ();
	jmms_ReadFromSettingsFile ();
	GoBackHistory() ;
}

JC_RETCODE jmms_ReadFromSettingsFile ()
{
	JC_CHAR *pmMMSSettngsFileName = JC_NULL ;
	JDD_FILE hSettFile  = JC_NULL;
	JC_RETCODE  rCode = JC_OK ;
	JC_BOOLEAN isFileExist;

	rCode = jdi_CUtilsCharToTchar (JC_NULL, JMMS_SETTINGS_FILE, &pmMMSSettngsFileName) ;
	if (pmMMSSettngsFileName)
	{
		isFileExist = jdd_FSIsFileExist (hSettFile, pmMMSSettngsFileName);
		mmi_trace(1,"Jdd_log: settings file exist[%d]",isFileExist);
		if (isFileExist == E_TRUE)
		{	  	
			hSettFile = jdd_FSOpen (JC_NULL, pmMMSSettngsFileName, E_OPEN_READ_MODE) ;		
			if (hSettFile)
			{
				if (1 != jdd_FSRead ((void *)&(g_pstJMmsAppInfo->stMMSSetting), sizeof (JMMS_SETTINGS), 1, hSettFile))
				{
					rCode = JC_ERR_FILE_READ ;
					jmms_SetDefaultSettings();
				}
				jdd_FSClose (hSettFile) ;
			}
			else
			{
				rCode = JC_ERR_FILE_OPEN ;
				jmms_SetDefaultSettings () ;
			}		 
		}
		else
		{
			mmi_trace(1,"Jdd_log: settings file else part file not exist");
			jmms_SetDefaultSettings();
			hSettFile = jdd_FSOpen (JC_NULL, pmMMSSettngsFileName, E_CREATE_WRITE_MODE) ;	
			if(hSettFile)
			{
				jdd_FSWrite ((void *)&(g_pstJMmsAppInfo->stMMSSetting), sizeof (JMMS_SETTINGS), 1, hSettFile);
				jdd_FSClose (hSettFile) ;
			}
		}
		jdd_MemFree (pmMMSSettngsFileName) ;
	}
	else
	{
		rCode = JC_ERR_MEMORY_ALLOCATION ;
	}
	mmi_trace (1, "JDD_LOG: jMmms_updateSettingsFile returned: %x", JRET(rCode)) ;
	mmi_trace(1, "Settings data DR[%d], RR[%d], RDR[%d], prio [%d], DT[%d],ET[%d], RRR[%d], AS[%d], ST[%d]",
		g_pstJMmsAppInfo->stMMSSetting.bDeliveryReport,
	g_pstJMmsAppInfo->stMMSSetting.bReadReport,
	g_pstJMmsAppInfo->stMMSSetting.bRetrieveDeliveryReport,
	g_pstJMmsAppInfo->stMMSSetting.ePriority,		
	g_pstJMmsAppInfo->stMMSSetting.uiDeliveryTime,
	g_pstJMmsAppInfo->stMMSSetting.uiExpiryTime,
	g_pstJMmsAppInfo->stMMSSetting.uiRetrieveReadReport,
	g_pstJMmsAppInfo->stMMSSetting.bIsAutoSignatureOn,
	g_pstJMmsAppInfo->stMMSSetting.uiDefaultSlideTime);
	return (rCode) ;
}


void jmms_SetDefaultSettings()
{
	g_pstJMmsAppInfo->stMMSSetting.bDeliveryReport = E_FALSE;
	g_pstJMmsAppInfo->stMMSSetting.bReadReport = E_FALSE;
	g_pstJMmsAppInfo->stMMSSetting.bRetrieveDeliveryReport= E_FALSE;
	g_pstJMmsAppInfo->stMMSSetting.ePriority = E_MSG_SETTINGS_PRIORITY_NORMAL;
	jc_memset(g_pstJMmsAppInfo->stMMSSetting.pszAutoSignature, 0,
					MAX_AUTOSIGN_BUFF_SIZE*sizeof(JC_CHAR));
	g_pstJMmsAppInfo->stMMSSetting.uiDeliveryTime = E_MSG_IMMIDIATE;
	g_pstJMmsAppInfo->stMMSSetting.uiExpiryTime = E_MSG_MAX_EXPIRYTIME;
	g_pstJMmsAppInfo->stMMSSetting.uiRetrieveReadReport = E_MSG_SETTINGS_READ_REPORT_ONREQUEST;
	g_pstJMmsAppInfo->stMMSSetting.bIsAutoSignatureOn = E_FALSE;
	g_pstJMmsAppInfo->stMMSSetting.uiDefaultSlideTime = MMIMMS_SLIDE_DUARATION_DEFAULT;
	mmi_trace(1,"jdd_log DefaultSlideTime %d", g_pstJMmsAppInfo->stMMSSetting.uiDefaultSlideTime);
}

void GoBackToSettingsScreen()
{
	DeleteUptoScrID(SCR_ID_MMS_SETTINGS);
}

void jmms_ComposeSettingsDone()
{	
	JC_RETCODE rCode;

	g_pstJMmsAppInfo->stMMSSetting.bIsAutoSignatureOn = g_setting_context.currAutoSignItem;
	jc_wstrcpy((S8*)g_pstJMmsAppInfo->stMMSSetting.pszAutoSignature,\
				(S8*)g_setting_context.pcAutoSignBuff);	
	
	rCode = jmms_WriteToSettingsFile() ;
	
	if (!rCode)	
	{
		DisplayPopup(GetString (STR_GLOBAL_SAVED), IMG_GLOBAL_SAVE, 1, 1500, 1) ;
		GoBackToSettingsScreen();
	}
	else	
	{
		DisplayPopup(GetString (STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 1, 2000, 1) ;	
		GoBackToSettingsScreen();
	}
}

void jmms_SendSettingsDone()
{	
	JC_RETCODE rCode;
	
	g_pstJMmsAppInfo->stMMSSetting.uiExpiryTime = g_setting_context.currValidityPeriodItem;
	g_pstJMmsAppInfo->stMMSSetting.bDeliveryReport = g_setting_context.currSendDeliveryReportItem;
	g_pstJMmsAppInfo->stMMSSetting.bReadReport = g_setting_context.currSendReadReportItem;
	g_pstJMmsAppInfo->stMMSSetting.ePriority = g_setting_context.currPriortlistItem; 
	g_pstJMmsAppInfo->stMMSSetting.uiDeliveryTime = g_setting_context.currSendDeliveryTimeItem;	
	g_pstJMmsAppInfo->stMMSSetting.uiDefaultSlideTime = jdi_CUtilsTtoI (JC_NULL, \
														(JC_CHAR*)(g_setting_context.szSlideTimeBuffer));
	g_pstJMmsAppInfo->stMMSSetting.uiDefaultSlideTime *= 1000;//converting into millisecond	
	rCode = jmms_WriteToSettingsFile() ;	
	if (!rCode)	
	{
		DisplayPopup(GetString (STR_GLOBAL_SAVED), IMG_GLOBAL_SAVE, 1, 1500, 1) ;	
		GoBackToSettingsScreen();
	}
	else	
	{
		DisplayPopup(GetString (STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 1, 2000, 1) ;	
		GoBackToSettingsScreen();
	}
}

void jmms_RetrieveSettingsDone()
{	
	JC_RETCODE rCode;
	
	g_pstJMmsAppInfo->stMMSSetting.uiRetrieveReadReport = g_setting_context.currRcvReadReportItem;
	g_pstJMmsAppInfo->stMMSSetting.bRetrieveDeliveryReport = g_setting_context.currRcvDeliveryReportItem;	
	rCode = jmms_WriteToSettingsFile() ;	
	if (!rCode)	
	{
		DisplayPopup(GetString (STR_GLOBAL_SAVED), IMG_GLOBAL_SAVE, 1, 1500, 1) ;	
		GoBackToSettingsScreen();
	}
	else	
	{
		DisplayPopup(GetString (STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 1, 2000, 1) ;	
		GoBackToSettingsScreen();
	}
}


void jmms_SettingsComposeHandler()
{	
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction(jmms_ComposeSettingsDone);
}

void jmms_ComposeLskHandle()
{
	ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS);
	SetLeftSoftkeyFunction (EntryScrForInputMethodAndDone, KEY_EVENT_UP);
}

void ExitComposeSettingScreen(void)
{
	history_t   Scr; 
	S16 nHistory = 0;
	U16 inputBufferSize;  //added for inline edit history

	Scr.scrnID = SCR_ID_MMS_COMPOSE;
	CloseCategory57Screen();
	Scr.entryFuncPtr = jmms_SettingComposeEntryHandler;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)&nHistory);
	GetCategory57History(Scr.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();     //added for inline edit history
	GetCategory57Data ((U8*) Scr.inputBuffer );         //added for inline edit history
	AddNHistory(Scr, (U16)inputBufferSize);                       //added for inline edit history
}

void jmms_SettingsComposeFillInlineStruct(void *pComposeInlineStruct) 
{
	InlineItem *InlineStruct = (InlineItem *)pComposeInlineStruct;
	INT iPos = 0;		

	//fill  auto sign on/off
	g_setting_context.compose_items_list.auto_signature[iPos++] = GetString(STR_ID_GLOBAL_OFF);
	g_setting_context.compose_items_list.auto_signature[iPos++] = GetString(STR_ID_GLOBAL_ON);	

	g_setting_context.currAutoSignItem = g_pstJMmsAppInfo->stMMSSetting.bIsAutoSignatureOn;
	SetInlineItemCaption(&InlineStruct[E_COMPOSE_AUTO_SIGN_CAPTION], (U8 *) GetString(STR_ID_MMS_AUTO_SIGNATURE));
	SetInlineItemActivation(&InlineStruct[E_COMPOSE_AUTO_SIGN_ITEM],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0 );
	SetInlineItemSelect(&InlineStruct[E_COMPOSE_AUTO_SIGN_ITEM],iPos,
		(U8**)g_setting_context.compose_items_list.auto_signature, 
		(S32 *)&g_setting_context.currAutoSignItem );

	// fill auto sign buffer
	jc_wstrcpy((S8*)g_setting_context.pcAutoSignBuff,\
				(S8*)g_pstJMmsAppInfo->stMMSSetting.pszAutoSignature);
	SetInlineItemCaption(&InlineStruct[E_COMPOSE_SIGNATURE_CAPTION], (U8 *) GetString(STR_ID_MMS_SIGNATURE));	
	SetInlineItemActivation(&InlineStruct[E_COMPOSE_SIGNATURE_ITEM], KEY_LSK, KEY_EVENT_UP);
	SetInlineItemFullScreenEdit(&InlineStruct[E_COMPOSE_SIGNATURE_ITEM],STR_ID_MMS_TEXT,0,
								(PU8)g_setting_context.pcAutoSignBuff, 
								MAX_AUTOSIGN_BUFF_SIZE, 
								(INPUT_TYPE_ALPHANUMERIC_UPPERCASE | INPUT_TYPE_ALPHANUMERIC_LOWERCASE));
	
	SetInlineFullScreenEditCustomFunction(&InlineStruct[E_COMPOSE_SIGNATURE_ITEM], jmms_ComposeLskHandle);
	
}

void jmms_SettingComposeEntryHandler()
{
	U8* guiBuffer;
	U8* inputBuffer;     
	U16 inputBufferSize;    
	U16 icon_list[COMPOSE_SCREEN_ICON_TOTAL] = {0};	
	InlineItem wgui_inline_list_items[COMPOSE_SCREEN_ICON_TOTAL];
	
	EntryNewScreen(SCR_ID_MMS_COMPOSE, ExitComposeSettingScreen, NULL, NULL);
	InitializeCategory57Screen();
	guiBuffer = GetCurrGuiBuffer(SCR_ID_MMS_COMPOSE);
	inputBuffer = GetCurrNInputBuffer (SCR_ID_MMS_COMPOSE, &inputBufferSize );    //added for inline edit history
	
	//RegisterHighlightHandler(jmms_SettingsComposeHandler);	
	jmms_SettingsComposeFillInlineStruct((void *)wgui_inline_list_items);	

	if(inputBuffer != NULL)
		SetCategory57Data(wgui_inline_list_items, 4, inputBuffer);	
	
	icon_list[E_COMPOSE_AUTO_SIGN_CAPTION] = gIndexIconsImageList[0];
	icon_list[E_COMPOSE_SIGNATURE_CAPTION] = gIndexIconsImageList[1];

	ShowCategory57Screen(STR_ID_MMS_SETTINGS,IMG_SMS_ENTRY_SCRN_CAPTION,
                        STR_GLOBAL_OK,IMG_GLOBAL_OK,
                        STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
                        COMPOSE_SCREEN_ICON_TOTAL,
                        icon_list,wgui_inline_list_items,0,guiBuffer);
	 
	#if defined(__MMI_GB__)
	mmi_frm_set_highlight_handler();
	GBSetInputboxLSKFunction(EntryScrForInputMethodAndDone); 
	#endif
	SetCategory57RightSoftkeyFunctions(jmms_ComposeSettingsDone,GoBackHistory);
	//DisableCategory57ScreenDone();		
	//SetKeyHandler(jmms_SettingsEndKeyHandler, KEY_END, KEY_EVENT_DOWN) ;	
}

/*
void jmms_SettingsEndKeyHandler()
{
	jmms_ExitHandler();
}
*/

void SettingSendHltHandler()
{	
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction(jmms_SendSettingsDone);
}

extern U8 wgui_inline_full_screen_text_edit_buffer[];

void jmms_SlideTimingDoneHandler()
{
	JC_UINT32 uiSlideTimer = 0;	
	
	uiSlideTimer = jdi_CUtilsTtoI (NULL, (const JC_CHAR *)wgui_inline_full_screen_text_edit_buffer) ;
	mmi_trace(1," uiSlideTimer value is %d",uiSlideTimer);
	if((0 == uiSlideTimer) || (60 < uiSlideTimer))
	{			
		//warning to show "Enter Between 1 and 60"	
		DisplayPopup(GetString(STR_ID_MMS_ENTER_WARNING), IMG_GLOBAL_INFO, 1, 2000, 1);
	}
	else
	{
		ConfirmInlineFullScreenEdit();		
	}
}

void jmms_SendLskHandle()
{
	ChangeLeftSoftkey(STR_GLOBAL_DONE, IMG_GLOBAL_OPTIONS);
	SetLeftSoftkeyFunction (jmms_SlideTimingDoneHandler, KEY_EVENT_UP);
}

void SendFillInlineStruct(void *pSendInlineStruct)
{
	InlineItem *sendInlinestuct = (InlineItem *)pSendInlineStruct;
	INT iPos = 0;	
	JC_CHAR * pszTemp = JC_NULL;

	g_setting_context.currValidityPeriodItem = g_pstJMmsAppInfo->stMMSSetting.uiExpiryTime;
	g_setting_context.send_items_list.validity_period_list[iPos++] = GetString(STR_ID_MMS_MAX);
	g_setting_context.send_items_list.validity_period_list[iPos++] = GetString(STR_ID_MMS_1HOUR);
	g_setting_context.send_items_list.validity_period_list[iPos++] = GetString(STR_ID_MMS_12HOUR);
	g_setting_context.send_items_list.validity_period_list[iPos++] = GetString(STR_ID_MMS_1DAY);
	g_setting_context.send_items_list.validity_period_list[iPos++] = GetString(STR_ID_MMS_1WEEK);

	SetInlineItemCaption(&sendInlinestuct[E_VAlIDITY_PERIOD_CAPTION], (U8 *) GetString(STR_ID_MMS_VALIDITY_PERIOD));
	SetInlineItemActivation(&sendInlinestuct[E_VALITY_PERIOD_ITEM],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0 );
	SetInlineItemSelect(&sendInlinestuct[E_VALITY_PERIOD_ITEM],iPos,
		(U8**)g_setting_context.send_items_list.validity_period_list,
		(S32 *)&g_setting_context.currValidityPeriodItem);

	//-------------validity period added
	
	iPos = 0;
	g_setting_context.currSendDeliveryReportItem = g_pstJMmsAppInfo->stMMSSetting.bDeliveryReport;
	g_setting_context.send_items_list.send_delivert_report[iPos++]= GetString(STR_ID_GLOBAL_OFF);
	g_setting_context.send_items_list.send_delivert_report[iPos++]= GetString(STR_ID_GLOBAL_ON);


	SetInlineItemCaption(&sendInlinestuct[E_SEND_DELIVERY_REPORT_CAPTION], (U8 *) GetString(STR_ID_MMS_DELIVERY_REPORT));
	SetInlineItemActivation(&sendInlinestuct[E_SEND_DELIVERY_REPORT_ITEM],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0 );
	SetInlineItemSelect(&sendInlinestuct[E_SEND_DELIVERY_REPORT_ITEM],iPos,
		(U8**)g_setting_context.send_items_list.send_delivert_report,
		(S32 *)&g_setting_context.currSendDeliveryReportItem);

	//-----------------delivery Reprot ON/OFF added

	iPos = 0;
	g_setting_context.currSendReadReportItem = g_pstJMmsAppInfo->stMMSSetting.bReadReport;
	g_setting_context.send_items_list.send_read_report[iPos++]= GetString(STR_ID_GLOBAL_OFF);
	g_setting_context.send_items_list.send_read_report[iPos++]= GetString(STR_ID_GLOBAL_ON);
	
	SetInlineItemCaption(&sendInlinestuct[E_SEND_READ_REPORT_CAPTION], (U8 *) GetString(STR_ID_MMS_READ_REPORT));
	SetInlineItemActivation(&sendInlinestuct[E_SEND_READ_REPORT_ITEM],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0 );
	SetInlineItemSelect(&sendInlinestuct[E_SEND_READ_REPORT_ITEM],iPos,
		(U8**)g_setting_context.send_items_list.send_read_report,
		(S32 *)&g_setting_context.currSendReadReportItem);

	//--------------- Send read report ON/OFF added 

	iPos = 0;
	g_setting_context.currPriortlistItem = g_pstJMmsAppInfo->stMMSSetting.ePriority;
	g_setting_context.send_items_list.priority_list[iPos++] = GetString(STR_ID_MMS_LOW);
	g_setting_context.send_items_list.priority_list[iPos++] = GetString(STR_ID_MMS_NORMAL);
	g_setting_context.send_items_list.priority_list[iPos++] = GetString(STR_ID_MMS_HIGH);	

	SetInlineItemCaption(&sendInlinestuct[E_PRIORITY_CAPTION], (U8 *) GetString(STR_ID_MMS_PRIORITY));
	SetInlineItemActivation(&sendInlinestuct[E_PRIORITY_ITEM],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0 );
	SetInlineItemSelect(&sendInlinestuct[E_PRIORITY_ITEM],iPos,
						(U8**)g_setting_context.send_items_list.priority_list,
						(S32 *)&g_setting_context.currPriortlistItem);	

	//------------priority list added	
	mmi_trace(1, " g_pstJMmsAppInfo->stMMSSetting.uiDefaultSlideTime value is %d", g_pstJMmsAppInfo->stMMSSetting.uiDefaultSlideTime);
	pszTemp = jdi_CUtilsItoT (NULL, g_pstJMmsAppInfo->stMMSSetting.uiDefaultSlideTime/1000);
	if(pszTemp)
	{
		jc_tcsncpy(g_setting_context.szSlideTimeBuffer, pszTemp,SENDSET_SLIDE_TIME_BUFFER_LEN-1);
		jdd_MemFree(pszTemp);
	}
	SetInlineItemCaption(&sendInlinestuct[E_SLIDE_TIMING_CAPTION], (U8 *) GetString(STR_ID_MMS_SLIDE_TIMING));	
	SetInlineItemActivation(&sendInlinestuct[E_SLIDE_TIMING_ITEM], KEY_LSK, KEY_EVENT_UP);
	SetInlineItemFullScreenEdit(&sendInlinestuct[E_SLIDE_TIMING_ITEM],STR_ID_MMS_TEXT,0,
								(PU8)g_setting_context.szSlideTimeBuffer, 
								SENDSET_SLIDE_TIME_BUFFER_LEN, INPUT_TYPE_NUMERIC);
	SetInlineFullScreenEditCustomFunction(&sendInlinestuct[E_SLIDE_TIMING_ITEM], jmms_SendLskHandle);
	
	//-----------slide timing added
	
	iPos = 0;
	g_setting_context.currSendDeliveryTimeItem = g_pstJMmsAppInfo->stMMSSetting.uiDeliveryTime;
	g_setting_context.send_items_list.delivery_time_list[iPos++] = GetString(STR_ID_MMS_IMMEDIATE);
	g_setting_context.send_items_list.delivery_time_list[iPos++] = GetString(STR_ID_MMS_IN1HOUR);
	g_setting_context.send_items_list.delivery_time_list[iPos++] = GetString(STR_ID_MMS_IN12HOUR);
	g_setting_context.send_items_list.delivery_time_list[iPos++] = GetString(STR_ID_MMS_IN24HOUR);		

	SetInlineItemCaption(&sendInlinestuct[E_SEND_DELIVERY_TIME_CAPTION], (U8 *) GetString(STR_ID_MMS_DELIVERY_TIME));
	SetInlineItemActivation(&sendInlinestuct[E_SEND_DELIVERY_TIME_ITEM],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0 );
	SetInlineItemSelect(&sendInlinestuct[E_SEND_DELIVERY_TIME_ITEM],iPos,
						(U8**)g_setting_context.send_items_list.delivery_time_list,
						(S32 *)&g_setting_context.currSendDeliveryTimeItem);	
}

void ExitSendScreen(void)
{
	history_t   Scr; 
	S16 nHistory = 0;
	U16 inputBufferSize;                //added for inline edit history

	Scr.scrnID = SCR_ID_MMS_SEND;
	CloseCategory57Screen();
	Scr.entryFuncPtr = jMms_SettingSendEntryHandler;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)&nHistory);
	GetCategory57History(Scr.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();     //added for inline edit history
	GetCategory57Data ((U8*) Scr.inputBuffer );         //added for inline edit history
	AddNHistory(Scr, (U16)inputBufferSize);                       //added for inline edit history
}

void jMms_SettingSendEntryHandler()
{
	U8* guiBuffer;
	U8* inputBuffer;                       //added for
	U16 inputBufferSize;                //added for
    InlineItem wgui_inline_send_screen_items[SEND_SCREEN_ICON_TOTAL];
	U16 icon_list[SEND_SCREEN_ICON_TOTAL] = {0};
	
	EntryNewScreen(SCR_ID_MMS_SEND, ExitSendScreen, NULL, NULL);
	InitializeCategory57Screen();
	guiBuffer = GetCurrGuiBuffer(SCR_ID_MMS_SEND);
	inputBuffer = GetCurrNInputBuffer (SCR_ID_MMS_SEND, &inputBufferSize );    //added for inline edit history	
	SendFillInlineStruct((void *)wgui_inline_send_screen_items);	

	if(inputBuffer != NULL)
		SetCategory57Data(wgui_inline_send_screen_items, SEND_SCREEN_ICON_TOTAL, inputBuffer);
	
	icon_list[E_VAlIDITY_PERIOD_CAPTION] = gIndexIconsImageList[0];
	icon_list[E_SEND_DELIVERY_REPORT_CAPTION] = gIndexIconsImageList[1];
	icon_list[E_SEND_READ_REPORT_CAPTION] = gIndexIconsImageList[2];
	icon_list[E_PRIORITY_CAPTION] = gIndexIconsImageList[3];
	icon_list[E_SLIDE_TIMING_CAPTION] = gIndexIconsImageList[4];
	icon_list[E_SEND_DELIVERY_TIME_CAPTION] = gIndexIconsImageList[5];
	ShowCategory57Screen(STR_ID_MMS_SETTINGS,IMG_SMS_ENTRY_SCRN_CAPTION,
                        STR_GLOBAL_OK,IMG_GLOBAL_OK,
                        STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
                        SEND_SCREEN_ICON_TOTAL,
                        icon_list,wgui_inline_send_screen_items,0,guiBuffer);
	#if defined(__MMI_GB__)                                                          //add by yaosq 20061110 for leftsoft in profile
	mmi_frm_set_highlight_handler();
	GBSetInputboxLSKFunction(EntryScrForInputMethodAndDone); 
	#endif
	SetCategory57RightSoftkeyFunctions(jmms_SendSettingsDone,GoBackHistory);	
}

void RetrieveFillInlineStruct(void *pRcvInlineStruct)
{
	InlineItem *RcvInlineStruct = (InlineItem *)pRcvInlineStruct;	
	INT iPos = 0;

	g_setting_context.currHomeNwkItem = 0;
	g_setting_context.retrieve_items_list.home_network_list[iPos++] = (U8*)GetString(STR_ID_MMS_IMMEDIATE);
	//g_setting_context->retrieve_items_list.home_network_list[iPos++] = (U8*)GetString(STR_ID_MMS_DELAYED);
	//g_setting_context->retrieve_items_list.home_network_list[iPos++] = (U8*)GetString(STR_ID_MMS_REJECTED);

	SetInlineItemCaption(&RcvInlineStruct[E_HOME_NETWORK_CAPTION], (U8 *) GetString(STR_ID_MMS_HOME_NETWORK));
	SetInlineItemActivation(&RcvInlineStruct[E_HOME_NETWORK_ITEM],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0 );
	SetInlineItemSelect(&RcvInlineStruct[E_HOME_NETWORK_ITEM],iPos,
		(U8**)g_setting_context.retrieve_items_list.home_network_list, 
		(S32 *)&g_setting_context.currHomeNwkItem);

	//-----------------homenetwork added
	iPos = 0;

	g_setting_context.currRcvReadReportItem = g_pstJMmsAppInfo->stMMSSetting.uiRetrieveReadReport;	
	g_setting_context.retrieve_items_list.read_report_list[iPos++] = (U8*)GetString(STR_ID_MMS_SEND);
	g_setting_context.retrieve_items_list.read_report_list[iPos++] = (U8*)GetString(STR_ID_MMS_NEVER_SEND);
	g_setting_context.retrieve_items_list.read_report_list[iPos++] = (U8*)GetString(STR_ID_MMS_ON_REQUEST);

	SetInlineItemCaption(&RcvInlineStruct[E_RTV_READ_REPORT_CAPTION], (U8 *) GetString(STR_ID_MMS_READ_REPORT));
	SetInlineItemActivation(&RcvInlineStruct[E_RTV_READ_REPORT_ITEM],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0 );
	SetInlineItemSelect(&RcvInlineStruct[E_RTV_READ_REPORT_ITEM],iPos,
						(U8**)g_setting_context.retrieve_items_list.read_report_list, 
						(S32 *)&g_setting_context.currRcvReadReportItem);
		
	//---------------read report added
	iPos = 0;
	g_setting_context.currRcvDeliveryReportItem = g_pstJMmsAppInfo->stMMSSetting.bRetrieveDeliveryReport;
	g_setting_context.retrieve_items_list.rcv_DeliveryReport_List[iPos++] = (U8*)GetString(STR_ID_GLOBAL_OFF);
	g_setting_context.retrieve_items_list.rcv_DeliveryReport_List[iPos++] = (U8*)GetString(STR_ID_GLOBAL_ON);		

	SetInlineItemCaption(&RcvInlineStruct[E_RTV_DELIVERY_REPORT_CAPTION], (U8 *) GetString(STR_ID_MMS_DELIVERY_REPORT));
	SetInlineItemActivation(&RcvInlineStruct[E_RTV_DELIVERY_REPORT_ITEM],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0 );
	SetInlineItemSelect(&RcvInlineStruct[E_RTV_DELIVERY_REPORT_ITEM],iPos,
		(U8**)g_setting_context.retrieve_items_list.rcv_DeliveryReport_List, 
		(S32 *)&g_setting_context.currRcvDeliveryReportItem);		

}

void SettingRetrieveHltHandler()
{	
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction(jmms_RetrieveSettingsDone);
}

void jMms_SettingMemoryStatusEntryHandler()
{
	U8* guiBuffer;
	S8 memoryStatus[128];
	S8 tmpStr[64];
	S8 tmpStr_UCS2[64];
	FS_INFO fsinfo;
	UINT32	mms_free_space = jMMS_GetMMSFreeSpace (TRUE) ;
	UINT32	mms_total_space = jMMS_GetMMSTotalSpace () ;
	UINT32	mms_used_space = mms_total_space - mms_free_space ;
	UINT32	mms_total_num_msgs = jMMS_GetTotalNumberOfMsgs () ;

	/* If there are no messages, then the used space will be shown as 0 */
	mms_used_space = (0 == mms_total_num_msgs)? 0:(mms_total_space - mms_free_space) ;
	//to trace vdso space,not actual use
	FS_GetFSInfo(CS_FS_MMS_IDENTIFIER,&fsinfo);
	mmi_trace(1,"chenhe,vds0 total size is %dK,free is %dK",(UINT32)fsinfo.iTotalSize/1024,((UINT32)fsinfo.iTotalSize-(UINT32)fsinfo.iUsedSize)/1024);
	if(mms_total_space > 0)
	{
		memset(memoryStatus, 0, sizeof(memoryStatus));
		
		pfnUnicodeStrncat(memoryStatus, GetString(STR_ID_MMS_SETTINGS_USAGE), 16);
		memset(tmpStr, 0, 10);
		sprintf(tmpStr, ": %d", (U16)(100*mms_used_space/mms_total_space));
		AnsiiToUnicodeString(tmpStr_UCS2, tmpStr);
		pfnUnicodeStrcat(memoryStatus, tmpStr_UCS2);
		memset(tmpStr,0,10);
		AnsiiToUnicodeString(tmpStr,"%\n");
		pfnUnicodeStrcat(memoryStatus, tmpStr);
	}	
	pfnUnicodeStrncat(memoryStatus, GetString(STR_ID_MMS_SETTINS_USED), 16);
	memset(tmpStr, 0, 10);
	if((mms_used_space) > 1024*1024*10)
		sprintf(tmpStr, ":  %dM\n", (U16)(mms_used_space/(1024*1024)));
	else
		sprintf(tmpStr, ":  %dK\n",  (U16)(mms_used_space/(1024)));

	AnsiiToUnicodeString(tmpStr_UCS2, tmpStr);
	pfnUnicodeStrcat(memoryStatus, tmpStr_UCS2);


	pfnUnicodeStrncat(memoryStatus, GetString(STR_ID_MMS_SETTINGS_FREE), 16);
	memset(tmpStr, 0, 10);
	if(mms_free_space > 1024*1024*10)
		sprintf(tmpStr, ":  %dM", (U16)(mms_free_space/(1024*1024)));
	else
		sprintf(tmpStr, ":  %dK",  (U16)(mms_free_space/(1024)));
	AnsiiToUnicodeString(tmpStr_UCS2, tmpStr);
	pfnUnicodeStrcat(memoryStatus, tmpStr_UCS2);

	EntryNewScreen(SCR_ID_MMS_MEMORY_STATUS, NULL, jMms_SettingMemoryStatusEntryHandler, NULL);

	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_MEMORY_STATUS);
	ShowCategory7Screen(STR_ID_MMS_MEMORY_STATUS,IMG_SMS_ENTRY_SCRN_CAPTION, 0, 0,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK, (PU8)memoryStatus, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	
}

void ExitRetrieveScreen(void)
{
	history_t   Scr; 
	S16 nHistory = 0;
	U16 inputBufferSize;                //added for inline edit history

	Scr.scrnID = SCR_ID_MMS_RETRIEVE;
	CloseCategory57Screen();
	Scr.entryFuncPtr = jMms_SettingRetrieveEntryHandler;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer, ( S8*)&nHistory);
	GetCategory57History(Scr.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();     //added for inline edit history
	GetCategory57Data ((U8*) Scr.inputBuffer );         //added for inline edit history
	AddNHistory(Scr, (U16)inputBufferSize);                       //added for inline edit history
}
void jMms_SettingRetrieveEntryHandler()
{
	U8* guiBuffer;
	U8* inputBuffer;					   //added for
	U16 inputBufferSize;				//added for
	InlineItem wgui_inline_retrieve_screen_items[RETRIEVE_SCREEN_ICON_TOTAL];
	U16 icon_list[RETRIEVE_SCREEN_ICON_TOTAL] = {0};
	
	EntryNewScreen(SCR_ID_MMS_RETRIEVE, ExitRetrieveScreen, NULL, NULL);
	InitializeCategory57Screen();
	guiBuffer = GetCurrGuiBuffer(SCR_ID_MMS_RETRIEVE);	
	RetrieveFillInlineStruct((void *)wgui_inline_retrieve_screen_items);	
	inputBuffer = GetCurrNInputBuffer (SCR_ID_MMS_RETRIEVE, &inputBufferSize );    //added for inline edit history

	if(inputBuffer != NULL)
		SetCategory57Data(wgui_inline_retrieve_screen_items, RETRIEVE_SCREEN_ICON_TOTAL, inputBuffer);
	
	icon_list[E_HOME_NETWORK_CAPTION] = gIndexIconsImageList[0];
	icon_list[E_RTV_READ_REPORT_CAPTION] = gIndexIconsImageList[1];
	icon_list[E_RTV_DELIVERY_REPORT_CAPTION] = gIndexIconsImageList[2];
	ShowCategory57Screen(STR_ID_MMS_SETTINGS,IMG_SMS_ENTRY_SCRN_CAPTION,
						STR_GLOBAL_OK,IMG_GLOBAL_OK,
                        STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
						RETRIEVE_SCREEN_ICON_TOTAL,
						icon_list,wgui_inline_retrieve_screen_items,0,guiBuffer);
	
	SetCategory57RightSoftkeyFunctions(jmms_RetrieveSettingsDone,GoBackHistory);		
}

void jMms_setting_Hlt_handler(int iSelectIndex) 
{	
	switch (iSelectIndex)
	{
		case 0:
	#ifdef __MMI_MULTI_SIM__
			SetLeftSoftkeyFunction (MTPNP_mms_setting_select_sim_entry_handler ,KEY_EVENT_UP) ;
	#else
			SetLeftSoftkeyFunction (jMms_server_profile_entry_handler ,KEY_EVENT_UP) ;
	#endif	
			break ;
		case 1:
			SetLeftSoftkeyFunction (jmms_SettingComposeEntryHandler ,KEY_EVENT_UP) ;
			break ;			
		case 2:
			SetLeftSoftkeyFunction (jMms_SettingSendEntryHandler ,KEY_EVENT_UP) ;
			break ; 
		case 3:
			SetLeftSoftkeyFunction (jMms_SettingRetrieveEntryHandler ,KEY_EVENT_UP) ;
			break ; 	
		case 4:
			SetLeftSoftkeyFunction (jMms_SettingMemoryStatusEntryHandler ,KEY_EVENT_UP) ;
			break ; 	
			
		default:
			break ; 
	}
}

void	jMms_settings_entry_handler ()
{
	U8* guiBuffer; 	
	U16 item_texts[] = {STR_ID_MMS_SERVER_PROFILE, \
						STR_ID_MMS_COMPOSE,
						STR_ID_MMS_SEND,
						STR_ID_MMS_RETRIEVE,
						STR_ID_MMS_MEMORY_STATUS
						} ;
	
	U16 item_Image[] =  {IMG_GLOBAL_L1,\
						IMG_GLOBAL_L2,
						IMG_GLOBAL_L3,
						IMG_GLOBAL_L4,
						IMG_GLOBAL_L5
					    };	

	EntryNewScreen (SCR_ID_MMS_SETTINGS, NULL, jMms_settings_entry_handler, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_SETTINGS) ;
	RegisterHighlightHandler (jMms_setting_Hlt_handler) ;
	ShowCategory15Screen (STR_ID_MMS_SETTINGS, IMG_SMS_ENTRY_SCRN_CAPTION, 
							STR_GLOBAL_OK, 0, 
							STR_GLOBAL_BACK, 0,
							5, item_texts, item_Image, 
							1, 0, guiBuffer);	
	
	SetRightSoftkeyFunction (jMms_settings_Exit_handler, KEY_EVENT_UP);	
}

#if 0
/*Profile Settings*/
void jMMS_SettinginitProfiles () 
{
#ifdef __MMI_MULTI_SIM__
	MTPNP_mmi_ph_init();
#else
	mmi_ph_init () ;
#endif
}
#endif /* Guoj delete. It's not used now at 2009-12-3 */

void jMMS_SettingreadUpdateDataConnInfo ()
{
	nvram_mms_profile_content_struct *profCont ;
	ST_COMM_PROFILE 		stProxySett ;	
	ST_GENERIC_DATA_CONN	 stGenDataConn ;
	JC_CHAR				 amApn [MAX_DATA_CONN_APN_SIZE] = {0, } ;
	
	profCont =  mmi_ph_get_activated_mms_profile () ;	
	if (profCont)
	{
		jc_memset (&stProxySett, 0, sizeof (ST_COMM_PROFILE)) ;
		jc_memset (&stGenDataConn, 0, sizeof (ST_GENERIC_DATA_CONN)) ;
		
		/*  Fill Proxy Info */
		jc_snprintf (stProxySett.acHostName, 255, "%d.%d.%d.%d", \
			profCont->proxy_ip[0],
			profCont->proxy_ip[1],
			profCont->proxy_ip[2],
			profCont->proxy_ip[3]) ;
		
		/* Secured Connection is not yet added in UI */
		if (PH_CONN_TYPE_CONNECTION_OREINTED == profCont->conn_type)
		{
			stProxySett.uiPortNumber = 9201 ; /* No UI is provided for WAP Port. It is always 9201 */
			stProxySett.eConnMode = E_COMM_WSP_CONNECTION_ORIENTED ;
		}
#ifdef CS_SECURITY_ENABLE
		else if (PH_CONN_TYPE_CONNECTION_OREINTED_SECURE== profCont->conn_type)
		{
			stProxySett.uiPortNumber = 9203 ; 
			stProxySett.eConnMode = E_COMM_WSP_CONNECTION_ORIENTED_SECURE;
		}
#endif
		else
		{
			if (!jc_strcmp ("0.0.0.0", stProxySett.acHostName))
			{
				stProxySett.eConnMode = E_COMM_WPHTTP_DIRECT ;
			}
			else
			{
				stProxySett.eConnMode = E_COMM_WPHTTP_PROXY ;
				stProxySett.uiPortNumber = profCont->proxy_port ;
			}
		}
		
		mmi_trace (1,"settings: MMSC URL :[%s]",profCont->mmsc_url);

		if(g_pstJMmsAppInfo->pcMMSCUrl)
		{
			jdd_MemFree(g_pstJMmsAppInfo->pcMMSCUrl);
			g_pstJMmsAppInfo->pcMMSCUrl = NULL;
		}
		g_pstJMmsAppInfo->pcMMSCUrl = (JC_INT8 *)jdi_CUtilsStrDuplicate(JC_NULL,(const JC_INT8 *)profCont->mmsc_url) ;
		
		
		/* Fill Data Conn Info */
		stGenDataConn.eBearerType = E_DATA_CONN_GPRS_BEARER_TYPE ;
		mmi_dtcnt_get_apn (profCont->data_account, \
			(S8*)amApn, MAX_DATA_CONN_APN_SIZE) ;
		jdi_CUtilsTcharToCharMem (amApn, MAX_DATA_CONN_APN_SIZE, \
			stGenDataConn.uDataConnType.stGPRSDataConn.acAPN) ;
		mmi_trace (1,"settings: Apn name :[%s]",stGenDataConn.uDataConnType.stGPRSDataConn.acAPN);
		#if 0
		jc_strncpy (stGenDataConn.acUserName, profCont->username, MAX_DATA_CONN_USERNAME_SIZE) ;
		jc_strncpy (stGenDataConn.acPassword, profCont->password, MAX_DATA_CONN_PASSWORD_SIZE) ;
		#else
		      mmi_dtcnt_get_user_pass(profCont->data_account, \
			 (S8*)stGenDataConn.acUserName, (S8*)stGenDataConn.acPassword, \
		 	MAX_DATA_CONN_PASSWORD_SIZE) ;
		#endif
		
#ifdef __MMI_MULTI_SIM__
		stGenDataConn.uiConnID = WAP_MMS_UICONNID_SIM1_FLAG<<g_pstJMmsAppInfo->send_simid ;	
		stGenDataConn.uiConnID |= WAP_MMS_UICONNID_MMS_FLAG ;		
#else
		stGenDataConn.uiConnID = 2 ;	 /* This should be always 2 for MMS */
#endif
		mmi_trace (1,"settings: ConnID :[%d]",stGenDataConn.uiConnID);
		jMMS_updateDataConnSettings (&stProxySett, &stGenDataConn) ;
	}
	else
	{
		DisplayPopup ((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 1, \
			5000, 1); //Last argument is tone id. 1 = ERROR tone
	}	
}

void jMMS_Setting_deInitProfiles ()
{
	mmi_ph_deinit_profiles () ;
}

JC_UINT32 jmms_GetPriorityValue()
{
	JC_UINT32 uiProirty;
	switch(g_pstJMmsAppInfo->stMMSSetting.ePriority)
	{		
		case 0 :
		uiProirty = 1; // core ePriority starts from 1
		break ;
		
		case 1 :
		uiProirty = 2;
		break ;
		
		case 2:
		uiProirty = 3;
		break ;		
		
		default :
		uiProirty = 2 ; // default value is normal
		break ;
		
	}	
	return uiProirty;
}

JC_UINT32 jmms_GetExpiryTimeValue()
{
	JC_UINT32 uiExpiryTime ;
	switch(g_pstJMmsAppInfo->stMMSSetting.uiExpiryTime)
	{		
		case E_MSG_MAX_EXPIRYTIME :
		uiExpiryTime = 0 ;
		break ;
		
		case E_MSG_1HOUR_EXPIRYTIME :
		uiExpiryTime = 1 * 60 * 60 ;
		break ;
		
		case E_MSG_12HOUR_EXPIRYTIME:
		uiExpiryTime = 12 * 60 * 60 ;
		break ;
		
		case E_MSG_1DAY_EXPIRYTIME:
		uiExpiryTime = 24 * 60 * 60 ;
		break ;

		case E_MSG_1WEEK_EXPIRYTIME:
		uiExpiryTime = 7 * 24 * 60 * 60 ;
		break ;
		
		default :
		uiExpiryTime = 0 ;
		break ;
		
	}
	return uiExpiryTime;
}

JC_UINT32 jmms_GetDeliveryTimeValue()
{
	JC_UINT32 uiDeliveryTime ;	

	switch(g_pstJMmsAppInfo->stMMSSetting.uiDeliveryTime)
		{
			case E_MSG_IMMIDIATE :
			uiDeliveryTime = 0 ;
			break ;
			
			case E_MSG_1HOUR :
			uiDeliveryTime = 1 * 60 * 60 ;
			break ;
			
			case E_MSG_12HOUR :
			uiDeliveryTime = 12 * 60 * 60 ;
			break ;
			
			case E_MSG_24HOUR:
			uiDeliveryTime = 24 * 60 * 60 ;
			break ;		
					
			default :
			uiDeliveryTime = 0 ;
			break ;
		}
	return uiDeliveryTime;
}

#endif //__JATAAYU_APP__

