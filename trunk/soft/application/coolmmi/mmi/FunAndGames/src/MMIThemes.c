/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *	MMIthemes.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file implements Theme application.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
 
/**************************************************************
	FILENAME	: MMIThemes.c
  	PURPOSE		: Theme application
 
	DATE		: Restructured on May-24-2003
**************************************************************/
#ifndef _MMI_THEME_C
#define _MMI_THEME_C

/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "mmi_features.h"
#ifdef __MMI_THEMES_APPLICATION__
#include "mmi_data_types.h"

#include "funandgamesdefs.h"		/* application */
#include "funandgamesprots.h"
#include "mmithemes.h"
#include "unicodexdcl.h"

#include "timerevents.h"			/* timer */

#include "nvramtype.h"				/* nvram read/write */
#include "nvramprot.h"
#include "nvramenum.h"

#include "eventsgprot.h"			/* screen, history, and resource*/
#include "historygprot.h"
#include "custmenures.h"
#include "wgui_categories.h"
#include "mainmenudef.h"
#include "globaldefs.h"
#include "commonscreens.h"
//nclude "SettingDefs.h"
#include "helpprot.h"

#include "alarmframeworkprot.h"
#include "datetimegprot.h"

#include "debuginitdef.h"			/* debug */
#include "globalscrenum.h"
#include "sublcdhistorygprot.h"

 
#include "phonebookdef.h"
#include "mmithemes.h"
#include "wgui_categories_inputsenum.h"
#include "wgui_categories_inputs.h"
#ifdef MMI_ON_HARDWARE_P
#include "wap_ps_struct.h"
//#include "wapadp.h" //del by lin@20070619
#endif

#include "filemanagergprot.h"
#include "filemanagerdef.h"
 
#undef __NEWSIMULATOR 
#include "mmi_trace.h"

/* 
** Define
*/

/* 
** Typedef 
*/

/* 
** Local Variable
*/

/* 
** Local Function
*/

/* 
** Global Variable
*/
thm_contex_struct	g_thm_cntx;
extern wgui_inline_item wgui_inline_items[];	/* shall be move to header file later */

											
#ifdef __MMI_THEMES_V2_SUPPORT__ 

theme_list_struct *thm_details;
U16 delete_flag=1;
U32 setorderIDList[TOTAL_THEMES_COUNT];  //it is only for stub testing);
U8 g_temp_thm_index;
U8 currorderindex; //this is only for set order list

//U8 thm_file_check=0;
#endif

 
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
#define MAX_URL_LIST_ENTRIES 7
//S8 g_thm_recent_url[MAX_URL_ENTRIES][MAX_ENTER_ADDRESS_SIZE];
thm_nvram_download_list recent_url_list;
S8 g_thm_download_address[MAX_ENTER_ADDRESS_SIZE*ENCODING_LENGTH + ENCODING_LENGTH];
S16 g_total_recent_list;
U16 g_recent_item_index;
U8 dontSaveURLinHistory;
void Add_To_URL_List(void);
void HighlightThmDownloadOption(int index);
void Edit_URL_address(void);
void theme_goto_url(void);
void theme_connect_wap(void);
void preEntryThmDownload(void);
void edit_url_address(void);
void thm_add_to_URL_List(void);
void entry_url_done_input_option(void);
void exit_url_done_input_option(void);
void EntryInputMethodScreen(void);
void Download2GoBackHistory(void);
void ShowThemeList(void);
extern void mmi_frm_highlight_input_method_generic (void);
#ifdef MMI_ON_HARDWARE_P
extern void wap_entry_widget_not_ready(void);
#endif
#endif
 
/* 
** Global Function
*/
extern void UpdateUITheme(void);
extern const S8 *gDefaultDLT_URL;

#ifdef __MMI_UI_TAB_PANE__
extern void check_and_set_title_theme_changed(void);
#endif

 
#ifdef MMI_ON_HARDWARE_P
extern int MSF_wapIsReady(void);
#endif
 
#define __MMI_THEME_AUTO_UPDATE__

/*****************************************************************************
* FUNCTION
*	InitThemes
* DESCRIPTION
*  	Initialize themes
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
//#define MENU3101_THEMES1 519 //added by lin@20070619

void InitThemes(void)
{
	SetHiliteHandler(MENU3101_THEMES1, HighlightThmMenu);
 
#ifdef __MMI_THEMES_V2_SUPPORT__  //ritesh
	ConstructThemeList();

	//ConstructThemeOrderList();

	tm_activate_theme_at_bootup();
#else
	FillThemesList();
#endif  //ritesh
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
	g_total_recent_list=0;
#endif
 
}

	

extern void ExecSubLCDCurrExitHandler(void);
extern void GoBackSubLCDHistory(void);

/*****************************************************************************
* FUNCTION
*	SetBothLCDThemes
* DESCRIPTION
*  	Set themes of both main and sub lcd
* PARAMETERS
*	index	IN		index of theme
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void SetBothLCDThemes(U8 index)
{
	
#ifdef __MMI_THEMES_V2_SUPPORT__
	ExecSubLCDCurrExitHandler();
	GoBackSubLCDHistory();
#else
	SetThemes(index); 
	UpdateUITheme();
 
#ifdef __MMI_UI_TAB_PANE__
	check_and_set_title_theme_changed();
#endif
 
 
//	SUBLCD_RedrawCategoryFunction();
	ExecSubLCDCurrExitHandler();
	GoBackSubLCDHistory();
#endif
}

/*****************************************************************************
* FUNCTION
*	ConstructThemeList
* DESCRIPTION
*  	Get the themes and construct set order list
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/

#ifdef __MMI_THEMES_V2_SUPPORT__
void ConstructThemeList(void)
{
//	U8 i,j;
	U8 no_of_setorder_themes,themecount;
	U32 *p;
        U8 set_flag=0;

	
	no_of_setorder_themes=GetIDList(&p,&g_thm_cntx.CurrOrderIndex);
	g_thm_cntx.TotalTheme=tm_get_total_themes_count();

	switch(no_of_setorder_themes)
	{
	case 0:
			for(no_of_setorder_themes=0;no_of_setorder_themes<g_thm_cntx.TotalTheme;no_of_setorder_themes++)
			{
			g_thm_cntx.TmpOrderList[no_of_setorder_themes]=g_thm_cntx.TotalTheme;
			g_thm_cntx.SetOrderIDList[no_of_setorder_themes]=p[no_of_setorder_themes];
			}
			break;

	default:
			for(no_of_setorder_themes=0;no_of_setorder_themes<g_thm_cntx.TotalTheme;no_of_setorder_themes++)
			{
				g_thm_cntx.SetOrderIDList[no_of_setorder_themes]=p[no_of_setorder_themes];
				for(themecount=0;themecount<g_thm_cntx.TotalTheme;themecount++)
				{
					if(g_thm_cntx.SetOrderIDList[no_of_setorder_themes]==0)
					{
					g_thm_cntx.TmpOrderList[no_of_setorder_themes]=g_thm_cntx.TotalTheme;
					set_flag=1;
					break;
					}
					else if(g_thm_cntx.SetOrderIDList[no_of_setorder_themes]==thm_details[themecount].theme_id)
					{
					g_thm_cntx.TmpOrderList[no_of_setorder_themes]=themecount;
					set_flag=1;
					break;
					}
				}
				if(set_flag==0)
				g_thm_cntx.TmpOrderList[no_of_setorder_themes]=g_thm_cntx.TotalTheme;
				
				set_flag=0;
			}
}

	for(no_of_setorder_themes=0;no_of_setorder_themes<g_thm_cntx.TotalTheme;no_of_setorder_themes++)
	{
		g_thm_cntx.ThemeName[no_of_setorder_themes] = thm_details[no_of_setorder_themes].theme_name;
	}

	g_thm_cntx.ThemeName[g_thm_cntx.TotalTheme] = (PU8)GetString(STR_THEME_NONE);
}
#else
/*****************************************************************************
* FUNCTION
*	FillThemesList
* DESCRIPTION
*  	Fill inline item selection items
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void FillThemesList(void)
{
	U8 index = 0;

	g_thm_cntx.TmpOrderList[0] =  (S32) g_thm_cntx.CurrOrderList[0];
	g_thm_cntx.TmpOrderList[1] =  (S32) g_thm_cntx.CurrOrderList[1];
	g_thm_cntx.TmpOrderList[2] =  (S32) g_thm_cntx.CurrOrderList[2];
	g_thm_cntx.TmpOrderList[3] =  (S32) g_thm_cntx.CurrOrderList[3];	

	g_thm_cntx.TotalTheme = 3;
	for(index = 0; index<g_thm_cntx.TotalTheme;index++)
		g_thm_cntx.ThemeName[index] = (PU8)GetString((U16)(STR_THEME_DEFAULT + index));
	TRACE_EF(1, "STR_THEME_DEFAULT = %d", STR_THEME_DEFAULT);
	g_thm_cntx.ThemeName[index] = (PU8)GetString(STR_THEME_NONE);
}

#endif
/*****************************************************************************
* FUNCTION
*	SaveOrderThemes
* DESCRIPTION
*  	set order themes and save in NVRAM 
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void SaveOrderThemes()
{
	U8 index;
	U8 flag = 0;
 
#ifdef __MMI_THEMES_V2_SUPPORT__ 
	U32 TempID;
	U8 count=0;
#endif
 

#ifdef __MMI_THEMES_V2_SUPPORT__ 

	for(index=0;index<g_thm_cntx.TotalTheme;index++)
	{
		TempID=thm_details[g_thm_cntx.TmpOrderList[index]].theme_id;
		if(g_thm_cntx.SetOrderIDList[index] != TempID)
		{
			flag = 1;
			break;
		}
	}

#else
	for(index=0;index<NO_OF_ORDERS;index++)
	{
		if(g_thm_cntx.CurrOrderList[index] != (U8)g_thm_cntx.TmpOrderList[index])
		{
			flag = 1;
			break;
		}
	}
#endif

	if(flag)	/* order changed */
	{
		g_thm_cntx.CurrOrderIndex = -1;
 
		g_thm_cntx.IsOrderChanged = 1;
 

	#ifdef __MMI_THEMES_V2_SUPPORT__ 
		for(index=0;index<g_thm_cntx.TotalTheme;index++)
		{
			if(g_thm_cntx.TmpOrderList[index]==TOTAL_THEMES_COUNT)
			{
				g_thm_cntx.SetOrderIDList[index]=0;
			}
			else 
			{
				g_thm_cntx.SetOrderIDList[index]= thm_details[g_thm_cntx.TmpOrderList[index]].theme_id;
			}
			if(g_thm_cntx.SetOrderIDList[index]!=0)
				count++;
		}
	#else
		g_thm_cntx.CurrOrderList[0] = (S8)g_thm_cntx.TmpOrderList[0];
		g_thm_cntx.CurrOrderList[1] = (S8)g_thm_cntx.TmpOrderList[1];
		g_thm_cntx.CurrOrderList[2] = (S8)g_thm_cntx.TmpOrderList[2];
		g_thm_cntx.CurrOrderList[3] = (S8)g_thm_cntx.TmpOrderList[3];
		g_thm_cntx.IsOrderChanged = 1;
	#endif
		SetStartPositionofCurIndex();
		WriteThemesToNVRAM();
#ifdef __MMI_THEMES_V2_SUPPORT__ 
		SetOrderIDList(g_thm_cntx.SetOrderIDList,count,g_thm_cntx.CurrOrderIndex);
#endif
	}
	DisplayPopup((PU8) GetString(STR_THEME_OFF_UPADTE_SUCCESS),IMG_GLOBAL_ACTIVATED,TRUE, UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);//modify by niej
	DeleteUptoScrID(SCR_ID_THM_OPTIONS);
	g_thm_cntx.IsInThmApp = 0;	
}


/*****************************************************************************
* FUNCTION
*	HighlightThmSetOrder
* DESCRIPTION
*  	Set LSK of set order list 
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void HighlightThmSetOrder(S32 nIndex)
{
    ChangeLeftSoftkey ( STR_GLOBAL_OK, IMG_GLOBAL_OK );    // Force LSK label to "Ok" each time
    SetCategory57LeftSoftkeyFunction (SaveOrderThemes);
}


/*****************************************************************************
* FUNCTION
*	EntryThmSetOrder
* DESCRIPTION
*	Entry function of set order screen 
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void EntryThmSetOrder()
{
	U8* guiBuffer;
	U8* inputBuffer;                    //added for inline edit history
	U16 inputBufferSize;                //added for inline edit history
 
#ifdef __MMI_THEMES_V2_SUPPORT__ 
	S16 i;
#endif
 

	EntryNewScreen(SCR_ID_THM_SET_ORDER, ExitThmSetOrder, NULL, NULL);

 
#ifdef __MMI_THEMES_V2_SUPPORT__ 
	g_thm_cntx.IsInThmApp = 1;
#else
	g_thm_cntx.IsInThmApp = 2;
#endif
 

#ifdef __MMI_THEMES_V2_SUPPORT__ 	
	ConstructThemeList();
#endif
	
	InitializeCategory57Screen();

	guiBuffer = GetCurrGuiBuffer(SCR_ID_THM_SET_ORDER);

	if(guiBuffer == NULL)
	{
		g_thm_cntx.TmpOrderList[0] =  (S32) g_thm_cntx.CurrOrderList[0];
		g_thm_cntx.TmpOrderList[1] =  (S32) g_thm_cntx.CurrOrderList[1];
		g_thm_cntx.TmpOrderList[2] =  (S32) g_thm_cntx.CurrOrderList[2];
		g_thm_cntx.TmpOrderList[3] =  (S32) g_thm_cntx.CurrOrderList[3];
	}

	inputBuffer = GetCurrNInputBuffer (SCR_ID_THM_SET_ORDER, &inputBufferSize );    //added for inline edit history


#ifdef __MMI_THEMES_V2_SUPPORT__ 
	for(i=0;i<g_thm_cntx.TotalTheme;i++)
		SetInlineItemActivation(&wgui_inline_items[i],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);	

	for(i=0;i<g_thm_cntx.TotalTheme;i++)
		SetInlineItemSelect(&wgui_inline_items[i],(g_thm_cntx.TotalTheme+1),g_thm_cntx.ThemeName,(PS32)(&g_thm_cntx.TmpOrderList[i]));
#else
	SetInlineItemActivation(&wgui_inline_items[0],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemActivation(&wgui_inline_items[1],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemActivation(&wgui_inline_items[2],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemActivation(&wgui_inline_items[3],INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemSelect(&wgui_inline_items[0],(g_thm_cntx.TotalTheme+1),g_thm_cntx.ThemeName,(PS32)(&g_thm_cntx.TmpOrderList[0]));
	SetInlineItemSelect(&wgui_inline_items[1],(g_thm_cntx.TotalTheme+1),g_thm_cntx.ThemeName,(PS32)&g_thm_cntx.TmpOrderList[1]);
	SetInlineItemSelect(&wgui_inline_items[2],(g_thm_cntx.TotalTheme+1),g_thm_cntx.ThemeName,(PS32)&g_thm_cntx.TmpOrderList[2]);
	SetInlineItemSelect(&wgui_inline_items[3],(g_thm_cntx.TotalTheme+1),g_thm_cntx.ThemeName,(PS32)&g_thm_cntx.TmpOrderList[3]);
#endif

	RegisterHighlightHandler(HighlightThmSetOrder);

#ifdef __MMI_THEMES_V2_SUPPORT__ 
	if(inputBuffer!=NULL)
		SetCategory57Data(wgui_inline_items, TOTAL_THEMES_COUNT+1, inputBuffer);
	ShowCategory57Screen(STR_SCR3004_SET_ORDER_CAPTION,MAIN_MENU_TITLE_SETTINGS_ICON,STR_GLOBAL_OK,IMG_GLOBAL_OK,
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
		g_thm_cntx.TotalTheme,(PU16)gIndexIconsImageList,wgui_inline_items,
		0,guiBuffer);
#else
	if(inputBuffer!=NULL)
		SetCategory57Data(wgui_inline_items, NO_OF_ORDERS, inputBuffer);

	ShowCategory57Screen(STR_SCR3004_SET_ORDER_CAPTION,MAIN_MENU_TITLE_SETTINGS_ICON,STR_GLOBAL_OK,IMG_GLOBAL_OK,
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
		4,(PU16)gIndexIconsImageList,wgui_inline_items,
		0,guiBuffer);
#endif

	SetKeyHandler(ThmHandleEndKey,KEY_END,KEY_EVENT_DOWN);

	DisableCategory57ScreenDone();
	SetCategory57RightSoftkeyFunctions(SaveOrderThemes,GoBackHistory);
}


/*****************************************************************************
* FUNCTION
*	ExitThmSetOrder
* DESCRIPTION
*  	Exit function of set order screen 
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void ExitThmSetOrder(void)
{
	history_t h;
	U16 inputBufferSize;                //added for inline edit history

#ifdef __MMI_THEMES_V2_SUPPORT__ 	
	g_thm_cntx.IsInThmApp = 0;
#endif
	h.scrnID = SCR_ID_THM_SET_ORDER;
	CloseCategory57Screen();

	h.entryFuncPtr = EntryThmSetOrder;
	GetCategoryHistory(h.guiBuffer);
	inputBufferSize =(U16)  GetCategory57DataSize();     //added for inline edit history
	GetCategory57Data ((U8*) h.inputBuffer );         //added for inline edit history
	AddNHistory(h, inputBufferSize);                       //added for inline edit history
}

#ifdef __MMI_THEMES_V2_SUPPORT__
/*****************************************************************************
* FUNCTION
*	ActivateDefaultTheme
* DESCRIPTION
*  	function to activate default theme
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void ActivateDefaultTheme(void)
{
	tm_activate_theme(DefaultThmID);
}
#endif

#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
/*****************************************************************************
* FUNCTION
*	preEntryThmDownload
* DESCRIPTION
*  	It download a theme 
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void preEntryThmDownload(void)
{
	
	S16 error = 0;
	ReadRecord(NVRAM_EF_THEME_DOWNLOAD_LIST_LID,1,&recent_url_list,/*sizeof(thm_nvram_download_list)*/NVRAM_THEMES_DOWNLOAD_SIZE,&error);
	
	EntryThmDownload();
}

/*****************************************************************************
* FUNCTION
*	EntryThmDownload
* DESCRIPTION
*  	It displays URL list
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void EntryThmDownload(void)
{
	int number_of_items=0,i;
	U8*	guiBuffer;
//	U16 max_sub_menu_length= (MAX_SUB_MENU_SIZE-ENCODING_LENGTH)/ENCODING_LENGTH;

	EntryNewScreen(SCR_ID_DOWNLOAD_ENTER_ADDRESS, NULL, EntryThmDownload, NULL);

	RegisterHighlightHandler(HighlightThmDownloadOption);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_DOWNLOAD_ENTER_ADDRESS);

	
	memset(subMenuData,0,sizeof(subMenuData));
	pfnUnicodeStrcpy((PS8)subMenuData[number_of_items],(PS8)get_string(STR_DOWNLOAD_ENTER_ADDRESS));
	number_of_items++;
	AnsiiNToUnicodeString((PS8)subMenuData[number_of_items],(PS8)gDefaultDLT_URL,MAX_SUBMENU_CHARACTERS);
	number_of_items++;
	for (i=0;number_of_items < (MAX_URL_ENTRIES+2) && i < recent_url_list.total_entries;number_of_items++,i++)
	{
		pfnUnicodeStrncpy((PS8)subMenuData[number_of_items],(PS8 )recent_url_list.g_thm_recent_url[i],MAX_SUB_MENU_SIZE-ENCODING_LENGTH);
	}
	
	ShowCategory353Screen((U8*)GetString(STR_DOWNLOAD_ENTER_ADDRESS), 0, STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK, number_of_items, (U8**)subMenuDataPtrs, (U16*)gIndexIconsImageList,NULL , 0, 0, guiBuffer);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	
	AnsiiToUnicodeString((PS8)g_thm_download_address,(PS8)"http://");


}

/*****************************************************************************
* FUNCTION
*	mmi_frm_thm_highlight_handler
* DESCRIPTION
*  
* PARAMETERS
*	int
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void mmi_frm_thm_highlight_handler(int index)
{
	if(index==0)
		SetLeftSoftkeyFunction(thm_add_to_URL_List,KEY_EVENT_UP);
	else
	{
		SetLeftSoftkeyFunction(EntryInputMethodScreen,KEY_EVENT_UP);
		RegisterInputMethodScreenCloseFunction (Download2GoBackHistory);
	}
}
/*****************************************************************************
* FUNCTION
*	HighlightThmDownloadOption
* DESCRIPTION
*  
* PARAMETERS
*	int
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void HighlightThmDownloadOption(int index)
{
	g_recent_item_index=index;
	switch(index)
	{
		case 0:
				SetLeftSoftkeyFunction (edit_url_address, KEY_EVENT_UP);
			break;
		default:
			
			SetLeftSoftkeyFunction (theme_goto_url, KEY_EVENT_UP);
			break;

	}
}
/*****************************************************************************
* FUNCTION
*	edit_url_address
* DESCRIPTION
*  	It download a theme 
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void edit_url_address(void)
{
	U8* guiBuffer;


	EntryNewScreen(SCR_ID_DOWNLOAD_EDIT_ADDRESS, NULL, edit_url_address, NULL);		
		guiBuffer=GetCurrGuiBuffer(SCR_ID_DOWNLOAD_EDIT_ADDRESS);

	ShowCategory5Screen(STR_DOWNLOAD_ENTER_ADDRESS, 0,
						  STR_GLOBAL_OPTIONS, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,(S16)(INPUT_TYPE_ALPHANUMERIC_SENTENCECASE),(PU8)g_thm_download_address,MAX_ENTER_ADDRESS_SIZE+1,guiBuffer);
	SetLeftSoftkeyFunction (entry_url_done_input_option, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);

}

void Download2GoBackHistory(void)
{
	GoBacknHistory(1);

}
/*****************************************************************************
* FUNCTION
*	entry_url_done_input_option
* DESCRIPTION
*  	Entry function to download theme 
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void entry_url_done_input_option(void)
{
	U8* guiBuffer;
	U16 nStrItemList[10];
	U16 numItems;
	dontSaveURLinHistory=0;
	EntryNewScreen(SCR_ID_DOWNLOAD_DONE_OPTION, exit_url_done_input_option, NULL, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_DOWNLOAD_DONE_OPTION);
	SetParentHandler (MENU_THM_DOWNLOAD_INPUT_METHOD_AND_DONE_OPTION_ID);
	RegisterHighlightHandler (mmi_frm_thm_highlight_handler);
	numItems = GetNumOfChild (MENU_THM_DOWNLOAD_INPUT_METHOD_AND_DONE_OPTION_ID);
	GetSequenceStringIds(MENU_INPUT_METHOD_AND_DONE_OPTION_ID, nStrItemList);
	ShowCategory52Screen (STR_GLOBAL_OPTIONS, 0,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  numItems, nStrItemList, (PU16)gIndexIconsImageList,
						  NULL , 0, 0, guiBuffer );

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
/*****************************************************************************
* FUNCTION
*	exit_url_done_input_option
* DESCRIPTION
*  	Exit function to download theme 
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void exit_url_done_input_option()
{
	history_t currHistory;

	currHistory.scrnID = SCR_ID_DOWNLOAD_DONE_OPTION;
	currHistory.entryFuncPtr = entry_url_done_input_option;
	GetCategoryHistory (currHistory.guiBuffer);
	if (dontSaveURLinHistory!=1)
		AddHistory (currHistory);

}
/*****************************************************************************
* FUNCTION
*	thm_add_to_URL_List
* DESCRIPTION
*  	Function to add url in history
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void thm_add_to_URL_List(void)
{
	S16 error = 0;
	signed char  index= 0;

	dontSaveURLinHistory=1;
	if(g_thm_download_address!=NULL)
	{
		if(recent_url_list.total_entries == MAX_URL_ENTRIES)
			recent_url_list.total_entries--;

		if(recent_url_list.total_entries > 0)
		{
			for(index=recent_url_list.total_entries-1;index >= 0;index--)
			{
				pfnUnicodeStrcpy((PS8)recent_url_list.g_thm_recent_url[index+1],(PS8)recent_url_list.g_thm_recent_url[index]);

			}

		}
		pfnUnicodeStrcpy((PS8)recent_url_list.g_thm_recent_url[0],(PS8)g_thm_download_address);
		recent_url_list.total_entries++;
	}

/*	current_index=MAX_URL_ENTRIES-1;
	if(g_thm_download_address!=NULL)
	{
		if(recent_url_list.total_entries<MAX_URL_ENTRIES)
			recent_url_list.total_entries++;
		current_index=recent_url_list.total_entries-1;
		while(current_index>0)
		{
			pfnUnicodeStrcpy((PS8)recent_url_list.g_thm_recent_url[current_index],(PS8)recent_url_list.g_thm_recent_url[current_index-1]);
			current_index--;
		}

		pfnUnicodeStrcpy((PS8)recent_url_list.g_thm_recent_url[current_index],(PS8)g_thm_download_address);
*/
		
		WriteRecord(NVRAM_EF_THEME_DOWNLOAD_LIST_LID,1,&recent_url_list,NVRAM_THEMES_DOWNLOAD_SIZE/*sizeof(thm_nvram_download_list)*/,&error);
		g_recent_item_index=0;
		theme_goto_url();
	

}

void theme_goto_url(void)
{
#ifdef MMI_ON_HARDWARE_P
	if (!MSF_wapIsReady())
	{
		//WGUI_CTX->not_ready_for_MMS = 0;
		wap_entry_widget_not_ready();
	}
	else 
	{

		theme_connect_wap();
	}
#endif
		DeleteUptoScrID(SCR_ID_DOWNLOAD_ENTER_ADDRESS);
#ifndef MMI_ON_HARDWARE_P
		GoBackHistory();
#endif
}
/*****************************************************************************
* FUNCTION
*	theme_connect_wap
* DESCRIPTION
*  	Function to connect through wap
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void theme_connect_wap(void){

#ifdef MMI_ON_HARDWARE_P
	struct ilm_struct *ilm_ptr = NULL;

	wap_browser_startup_req_struct	*p=
			(wap_browser_startup_req_struct*) construct_local_para(
			(kal_uint16)(sizeof(wap_browser_startup_req_struct)),0);

			ClearAllKeyHandler();
	
			ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);

	if (bra_win_any_win_exists())
	{
		/* Abnormal case. BRA should already terminate its UI. */
		WAP_DBG_ASSERT(0);

		ilm_ptr = allocate_ilm( MOD_MMI );
		ilm_ptr->msg_id = MSG_ID_WAP_BROWSER_SHUTDOWN_REQ;
		ilm_ptr->peer_buff_ptr = NULL;
		ilm_ptr->local_para_ptr = (local_para_struct *) NULL;
		SEND_ILM( MOD_MMI, MOD_WAP, WAP_MMI_SAP, ilm_ptr);
	}
		
	p->type=2;
	p->msg_len=0;
	p->ref_count=0;
	
	if(g_recent_item_index==0)
		UnicodeToAnsii((PS8)p->url, (PS8)recent_url_list.g_thm_recent_url[g_recent_item_index]);
	else if(g_recent_item_index==1)
		strcpy((char *)p->url, (char *)gDefaultDLT_URL);
	else 
		UnicodeToAnsii((PS8)p->url, (PS8)recent_url_list.g_thm_recent_url[g_recent_item_index-2]);

	ilm_ptr = allocate_ilm(MOD_MMI);
   ilm_ptr->local_para_ptr = (local_para_struct *)p;
   ilm_ptr->msg_id         = MSG_ID_WAP_BROWSER_STARTUP_REQ;
   ilm_ptr->peer_buff_ptr  = NULL;
	

	SEND_ILM(MOD_MMI,MOD_WAP,WAP_MMI_SAP,ilm_ptr);

#endif

}
#endif
/*****************************************************************************
* FUNCTION
*	HighlightThmMenu
* DESCRIPTION
*	Hilite function of main themes menu 
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void HighlightThmMenu(void)
{
	SetLeftSoftkeyFunction(EntryThmMenu,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(EntryThmMenu, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	HighlightThmUpdatePeriod
* DESCRIPTION
*	Hilite function of pupdate period
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void HighlightThmUpdatePeriod(S32 nIndex)
{
	/* set IsSetFromMenu flag */
	g_thm_cntx.IsSetFromMenu = 1;
	g_thm_cntx.CurrHiliteItem = (U8)nIndex;
	
	if(nIndex > 0) /* update theme */
		SetLeftSoftkeyFunction(UpdateThemePeriod,KEY_EVENT_UP);
	else	/* OFF */
		SetLeftSoftkeyFunction(UpdateThemeOff,KEY_EVENT_UP);
}


/*****************************************************************************
* FUNCTION
*	HighlightThmItems
* DESCRIPTION
*	Highlight handler of Theme menu,
*	function show the current selected theme 
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void HighlightThmItems(S32 index)
{
#ifdef __MMI_THEMES_V2_SUPPORT__
	g_thm_cntx.CurrHiliteTheme = thm_details[index].theme_id;
	g_temp_thm_index = index;
#else
	g_thm_cntx.CurrHiliteTheme=(U8)index;
	SetBothLCDThemes(g_thm_cntx.CurrHiliteTheme);
 
#if (defined __MMI_UI_STYLE_3__) && (defined __MMI_UI_DALMATIAN_STATUSBAR__)
	gOnFullScreen |= MMI_LEAVE_FULL_SCREEN;// for showing the updated status bar on Theme menu. 
#endif
 
	RedrawCategoryFunction();
#endif
}


/*****************************************************************************
* FUNCTION
*	ThmHandleRSK
* DESCRIPTION
*	Change theme to original one and go back history. 
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void ThmHandleRSK(void)
{
	if(g_thm_cntx.CurrHiliteTheme!=g_thm_cntx.CurrTheme)
		SetBothLCDThemes(g_thm_cntx.CurrTheme);

	g_thm_cntx.IsInThmApp = 0;
	GoBackHistory();
}


/*****************************************************************************
* FUNCTION
*	ThmRedrawTheme
* DESCRIPTION
*	Redraw current hilite theme in option menu to make the user preceive current 
*  selecting theme
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void ThmRedrawTheme(void)
{
	if(g_thm_cntx.CurrHiliteTheme != g_thm_cntx.CurrTheme)
		SetBothLCDThemes(g_thm_cntx.CurrHiliteTheme);
}


/*****************************************************************************
* FUNCTION
*	ExitThmMenu
* DESCRIPTION
*	Exit function for theme menu, restore theme after exit theme screen.
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void ExitThmMenu(void)
{
	if(g_thm_cntx.CurrHiliteTheme != g_thm_cntx.CurrTheme)
 
#if (defined __MMI_UI_STYLE_3__) && (defined __MMI_UI_DALMATIAN_STATUSBAR__)
	{
		gOnFullScreen |= MMI_LEAVE_FULL_SCREEN;// for showing the updated status bar on Theme menu.
#endif
		SetBothLCDThemes(g_thm_cntx.CurrTheme);
#if (defined __MMI_UI_STYLE_3__) && (defined __MMI_UI_DALMATIAN_STATUSBAR__)
	}
#endif
 
}


/*****************************************************************************
* FUNCTION
*	EntryThmMenu
* DESCRIPTION
*	Show a lsit of themes
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void EntryThmMenu(void)
{
#ifdef __MMI_THEMES_V2_SUPPORT__
	U8 i = 0;
	U8 thm_file_check=0;
		
	
	EntryNewScreen(SCR_ID_THM_MENU, ExitThmMenu, EntryThmMenu, NULL);
	ConstructThemeList();
	
	
	g_thm_cntx.CurrTheme=tm_get_current_theme_id();
	for(i=0;i<g_thm_cntx.TotalTheme;i++)
	{
		if(g_thm_cntx.CurrTheme==thm_details[i].theme_id)
		{
			thm_file_check=1;
		}
	}
	if(thm_file_check==0)
	{
		ActivateDefaultTheme();
		reset_title_status_icon();
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
			ShowCategory7Screen(STR_SCR3001_THEMES_CAPTION,0,STR_GLOBAL_OK,IMG_GLOBAL_OK,
		0,0, (U8*)GetString(STR_DEFAULT_THEME_ACTIVATION),NULL);
#endif		
		SetLeftSoftkeyFunction(ShowThemeList,KEY_EVENT_UP);
		SetKeyHandler(ShowThemeList, KEY_RIGHT_ARROW,KEY_EVENT_DOWN );	
	}
	else
		ShowThemeList();
#else

	U16	nItems;
	U8*	guiBuffer;
	U16 i = 0;
	U16 *list_icons1 = NULL;
	U16 *list_icons2 = NULL;
		
	g_thm_cntx.IsInThmApp = 1;
	
	EntryNewScreen(SCR_ID_THM_MENU, ExitThmMenu, EntryThmMenu, NULL);
	
	FillThemesList();
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_THM_MENU);	

	nItems=(U16)g_thm_cntx.TotalTheme;
	list_icons1=(U16*)OslMalloc(nItems *sizeof(PU16));
	list_icons2=(U16*)OslMalloc(nItems *sizeof(PU16));

	for(i=0; i<nItems; i++)
	{
		if(i == g_thm_cntx.CurrTheme)
			list_icons1[i]= IMG_VICON;
		else
			list_icons1[i]= IMAGE_NOIMAGE_THEME;
	}

	for(i=0; i<nItems; i++)
	{
		list_icons2[i]= IMAGE_NOIMAGE_THEME;
	}

	RegisterHighlightHandler(HighlightThmItems);

	ShowCategory73Screen(STR_MENU3101_THEMES,MAIN_MENU_TITLE_SETTINGS_ICON,
		STR_GLOBAL_OPTIONS,IMG_GLOBAL_OPTIONS,
		STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
		nItems,(U8**)g_thm_cntx.ThemeName,list_icons1,list_icons2,
		(U16)g_thm_cntx.CurrTheme,guiBuffer,0);

	SetLeftSoftkeyFunction(EntryThmOptions,KEY_EVENT_UP);
	SetKeyHandler(EntryThmOptions, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	
	SetRightSoftkeyFunction(ThmHandleRSK,KEY_EVENT_UP);
	SetKeyHandler(ThmHandleRSK, KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(ThmHandleEndKey,KEY_END,KEY_EVENT_DOWN);

	if(list_icons1)
		OslMfree(list_icons1);

	if(list_icons2)
		OslMfree(list_icons2);
#endif
}

/*****************************************************************************
* FUNCTION
*	ShowThemeList
* DESCRIPTION
*	Function to show theme list from theme application
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
#ifdef __MMI_THEMES_V2_SUPPORT__
void ShowThemeList(void)
{
	U8*	guiBuffer;
	U16 i = 0;
	U16	list_icons1[MAX_SUB_MENUS];
	U16	list_icons2[MAX_SUB_MENUS];

	for(i=0;i<g_thm_cntx.TotalTheme;i++)
	{
		if(thm_details[i].theme_type==THEME_TYPE_SYSTEM)
				list_icons2[i]= IMAGE_SYSTEM_THEME;
		else if(thm_details[i].theme_type==THEME_TYPE_DOWNLOADED_PHONE)
				list_icons2[i]= IMAGE_PHONE_THEME;
		else if(thm_details[i].theme_type==THEME_TYPE_DOWNLOADED_CARD)
				list_icons2[i]= IMAGE_CARD_THEME;
	}

	for(i=0;i<g_thm_cntx.TotalTheme;i++)
	{
		list_icons1[i]= IMAGE_NOIMAGE_THEME;
		if(g_thm_cntx.CurrTheme==thm_details[i].theme_id)
		{
			g_thm_cntx.CurrThemeIndex=i;
			list_icons1[i]= IMAGE_ACTIVATED_THEME;
		}
	}

	guiBuffer = GetCurrGuiBuffer(SCR_ID_THM_MENU);	
	RegisterHighlightHandler(HighlightThmItems);
	ShowCategory73Screen(STR_MENU3101_THEMES,MAIN_MENU_TITLE_SETTINGS_ICON,
		STR_GLOBAL_OPTIONS,IMG_GLOBAL_OPTIONS,
		STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
		g_thm_cntx.TotalTheme,(U8**)g_thm_cntx.ThemeName,list_icons2,list_icons1,
		(U16)g_thm_cntx.CurrThemeIndex,guiBuffer,0);

	SetLeftSoftkeyFunction(EntryThmOptions,KEY_EVENT_UP);
	SetKeyHandler(EntryThmOptions, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	
	SetRightSoftkeyFunction(ThmHandleRSK,KEY_EVENT_UP);
	SetKeyHandler(ThmHandleRSK, KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetKeyHandler(ThmHandleEndKey,KEY_END,KEY_EVENT_DOWN);

}

#endif

/*****************************************************************************
* FUNCTION
*	ThmHandleEndKey
* DESCRIPTION
*	Function handle end key
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void ThmHandleEndKey(void)
{
	if(g_thm_cntx.CurrHiliteTheme != g_thm_cntx.CurrTheme)
		SetBothLCDThemes(g_thm_cntx.CurrTheme);

	g_thm_cntx.IsInThmApp = 0;
	DisplayIdleScreen();
}


/*****************************************************************************
* FUNCTION
*	HighlightThmOption
* DESCRIPTION
*	Highlight handler of theme menu
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void HighlightThmOption(S32 nIndex)
{	
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__

	switch(nIndex)
	{
		case 0:	
			SetLeftSoftkeyFunction(ActivateTheme,KEY_EVENT_UP);
			ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
			break;

		case 1:
			SetLeftSoftkeyFunction(EntryThmUpdatePeriod,KEY_EVENT_UP);
			SetKeyHandler(EntryThmUpdatePeriod, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);	
			break;

		case 2:
			SetLeftSoftkeyFunction(EntryThmSetOrder,KEY_EVENT_UP);
			SetKeyHandler(EntryThmSetOrder, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);				
			break;

		case 3:
			SetLeftSoftkeyFunction(preEntryThmDownload,KEY_EVENT_UP);
			SetKeyHandler(EntryThmDownload, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);				
			break;

		case 4:
			SetLeftSoftkeyFunction(ThemeDeleteConfirmation,KEY_EVENT_UP);
			SetKeyHandler(DeleteTheme, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);				
			break;
	}
#else
	switch(nIndex)
	{
		case 0:	
			SetLeftSoftkeyFunction(ActivateTheme,KEY_EVENT_UP);
			ClearKeyHandler(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
			break;

		case 1:
			SetLeftSoftkeyFunction(EntryThmUpdatePeriod,KEY_EVENT_UP);
			SetKeyHandler(EntryThmUpdatePeriod, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);	
			break;

		case 2:
			SetLeftSoftkeyFunction(EntryThmSetOrder,KEY_EVENT_UP);
			SetKeyHandler(EntryThmSetOrder, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);				
			break;
	}
#endif

}


/*****************************************************************************
* FUNCTION
*	EntryThmOptions
* DESCRIPTION
*	Entry func of themes option menu 
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void EntryThmOptions(void)
{
	U8*	guiBuffer;
	U16	ItemList[16];
	U16	nItems;

	EntryNewScreen(SCR_ID_THM_OPTIONS, NULL, EntryThmOptions, NULL);

	ThmRedrawTheme();

#ifndef __MMI_THEMES_V2_SUPPORT__
	g_thm_cntx.IsInThmApp = 2;
#endif

#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__ 
	if(g_thm_cntx.CurrHiliteTheme == g_thm_cntx.CurrTheme)
	{
		nItems = GetNumOfChild(MENU3001_THEMES_LIST_1);
		GetSequenceStringIds(MENU3001_THEMES_LIST_1,ItemList);
	}
	else
	{
		if(g_temp_thm_index < MAX_SYSTEM_THEMES)
		{
			nItems = GetNumOfChild(MENU3001_THEMES_LIST_1);
			GetSequenceStringIds(MENU3001_THEMES_LIST_1,ItemList);
		}
		else
		{
	nItems = GetNumOfChild(MENU3001_THEMES_LIST);
	GetSequenceStringIds(MENU3001_THEMES_LIST,ItemList);
		}
	}
#else
	nItems = GetNumOfChild(MENU3001_THEMES_LIST); 
	GetSequenceStringIds(MENU3001_THEMES_LIST,ItemList);
#endif
	
	SetParentHandler(MENU3001_THEMES_LIST);
	RegisterHighlightHandler(HighlightThmOption);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_THM_OPTIONS);

	RegisterHighlightHandler(HighlightThmOption);

	ShowCategory52Screen(	STR_GLOBAL_OPTIONS,MAIN_MENU_TITLE_SETTINGS_ICON,
							STR_GLOBAL_OK,IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							nItems, ItemList,(PU16) gIndexIconsImageList, NULL, 0, 0, guiBuffer	);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(ThmHandleEndKey,KEY_END,KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	EntryThmUpdatePeriod
* DESCRIPTION
*	entry func of themes update period menu 
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void EntryThmUpdatePeriod(void)
{
	U16		ItemList[16];
	U16		nItems;
	U8*		guiBuffer;

	EntryNewScreen(SCR_ID_THM_UPDATE_PERIOD, ExitThmUpdatePeriod, NULL, NULL);

#ifdef __MMI_THEMES_V2_SUPPORT__
	g_thm_cntx.IsInThmApp = 1;
#else
	g_thm_cntx.IsInThmApp = 2;
#endif

	guiBuffer = GetCurrGuiBuffer(SCR_ID_THM_UPDATE_PERIOD);
	nItems = GetNumOfChild(MENU3201_UPDATE_PERIOD);
	GetSequenceStringIds(MENU3201_UPDATE_PERIOD,ItemList);
	SetParentHandler(MENU3201_UPDATE_PERIOD);

	RegisterHighlightHandler(HighlightThmUpdatePeriod);

	ShowCategory11Screen(	STR_SCR3003_UPDATE_PERIOD_CAPTION,MAIN_MENU_TITLE_SETTINGS_ICON,
							STR_GLOBAL_OK,IMG_GLOBAL_OK,
							STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
							nItems, ItemList, (U16)g_thm_cntx.CurrUpdatePeriod, guiBuffer	);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(ThmHandleEndKey,KEY_END,KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	ExitThmUpdatePeriod
* DESCRIPTION
*	Exit func of themes update period menu
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void ExitThmUpdatePeriod(void)
{
	history_t h;
	S16 nHistory = 0;

	/* reset IsSetFromMenu flag */
#ifdef __MMI_THEMES_V2_SUPPORT__
	g_thm_cntx.IsInThmApp = 0;
#endif
	g_thm_cntx.IsSetFromMenu = 0;	
	
	h.scrnID = SCR_ID_THM_UPDATE_PERIOD;
	h.entryFuncPtr = EntryThmUpdatePeriod;
	pfnUnicodeStrcpy((S8*)h.inputBuffer, ( S8*)&nHistory);
	GetCategoryHistory(h.guiBuffer);
	AddHistory(h);
}


/*****************************************************************************
* FUNCTION
*	ActivateTheme
* DESCRIPTION
*	Activate theme select bu yser and write to NVRAM 
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void ActivateTheme()
{
#ifdef __MMI_THEMES_V2_SUPPORT__
	U16 result;
	EntryNewScreen(0, NULL, NULL, NULL);
	ShowCategory66Screen(STR_SCR3001_THEMES_CAPTION,0, 0, 0, 0, 0, (PU8)GetString(STR_ID_DLT_THM_PROGRESS), IMG_THEME_ACTIVATION, NULL);
	
	result=tm_activate_theme(g_thm_cntx.CurrHiliteTheme);
			
	
		switch(result)
	{
		case THEME_ERROR_SUCCESS:
			g_thm_cntx.CurrTheme = g_thm_cntx.CurrHiliteTheme;
			g_thm_cntx.CurrThemeIndex=g_temp_thm_index;
			if(g_thm_cntx.CurrUpdatePeriod >0)
			SetStartPositionofCurIndex();
			SetCurrOrderIndex(g_thm_cntx.CurrOrderIndex);
			DisplayPopup((PU8)GetString(STR_THEME_ACTIVATE_SUCCESS),  IMG_GLOBAL_ACTIVATED,TRUE, UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
			DeleteUptoScrID(SCR_ID_THM_MENU);
			break;
			case THEME_ERROR_ALREADY_ACTIVATED:
			DisplayPopup((PU8)GetString(STR_ID_FMGR_ALREADY_ACTIVE),  IMG_GLOBAL_ACTIVATED,TRUE, UI_POPUP_NOTIFYDURATION_TIME,ERROR_TONE);
			break;
	
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
		case THEME_ERROR_THEME_NOT_AVAILABLE:
			DisplayPopup((PU8)GetString(STR_THEME_NOT_AVAILABLE),  IMG_GLOBAL_ERROR,TRUE, UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
			break;
		case THEME_ERROR_FILE_FORMAT_BAD:
			DisplayPopup((PU8)GetString(STR_THEME_FORMAT_NOT_SUPPORTED),  IMG_GLOBAL_ERROR,TRUE, UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
			break;
		case THEME_ERROR_FAILURE:
			DisplayPopup((PU8)GetString(STR_THEME_UNKNOWN_ERROR),  IMG_GLOBAL_ERROR,TRUE, UI_POPUP_NOTIFYDURATION_TIME,ERROR_TONE);
			break;
		
		case THEME_ERROR_VERSION_FAILURE:
			DisplayPopup((PU8)GetString(STR_DOWNLOAD_THEME_UNSUPPORTED), IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
			break;
		case THEME_ERROR_PROCESS_QUEUE_SUCESS:
				DisplayPopup(NULL ,IMG_PROCESSING_PHONEBOOK, 1,UI_POPUP_NOTIFYDURATION_TIME,0);
			break;
		 default:
			DisplayPopup((PU8)GetString(STR_THEME_UNKNOWN_ERROR),  IMG_GLOBAL_ERROR,TRUE, UI_POPUP_NOTIFYDURATION_TIME,ERROR_TONE);
			break;
#endif
		}
	g_thm_cntx.IsInThmApp = 0;	 

#else

	if(g_thm_cntx.CurrHiliteTheme!=g_thm_cntx.CurrTheme)
	{
		SetBothLCDThemes(g_thm_cntx.CurrHiliteTheme);
		g_thm_cntx.CurrTheme = g_thm_cntx.CurrHiliteTheme;

		if(g_thm_cntx.CurrUpdatePeriod >0)
			SetStartPositionofCurIndex();

		WriteThemesToNVRAM();
	}

	DisplayPopup((PU8)GetString(STR_THEME_ACTIVATE_SUCCESS),  IMG_GLOBAL_ACTIVATED,TRUE, UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);//niej modify 
 	DeleteUptoScrID(SCR_ID_THM_MENU);
	g_thm_cntx.IsInThmApp = 0;	 	
#endif
}

#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__ 
/*****************************************************************************
* FUNCTION
*	ThemeDeleteConfirmation
* DESCRIPTION
*	Function for theme delete confirmation
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void ThemeDeleteConfirmation(void)
{

	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						  STR_GLOBAL_NO, IMG_GLOBAL_NO,
						 (UI_string_type)GetString(STR_GLOBAL_DELETE), IMG_GLOBAL_QUESTION, WARNING_TONE); 
	
	SetLeftSoftkeyFunction(DeleteTheme,KEY_EVENT_UP);
	SetKeyHandler(DeleteTheme, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);	
	
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
	
	SetKeyHandler(ThmHandleEndKey,KEY_END,KEY_EVENT_DOWN);
	
}
/*****************************************************************************
* FUNCTION
*	ThemeDelete
* DESCRIPTION
*	Function to delete a theme from theme application
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/

void DeleteTheme()
{
	theme_error_enum result;
	S32 error_type;

	result=tm_delete_theme(g_thm_cntx.CurrHiliteTheme, &error_type);
	switch(result)
	{
		case THEME_ERROR_THEME_NOT_AVAILABLE:
			DisplayPopup((PU8) GetString(STR_THEME_NOT_AVAILABLE),IMG_GLOBAL_ERROR, TRUE, UI_POPUP_NOTIFYDURATION_TIME,ERROR_TONE);
			break;
		case THEME_ERROR_SUCCESS:
		UpdateOrder(g_thm_cntx.CurrHiliteTheme);	
	//	DisplayPopup((PU8) GetString(STR_THEME_DELETE_SUCCESS),IMG_GLOBAL_ACTIVATED, TRUE, UI_POPUP_NOTIFYDURATION_TIME,0);
		DisplayPopup((PU8) GetString(STR_GLOBAL_DONE),IMG_GLOBAL_ACTIVATED, TRUE, UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
			break;
		case THEME_ERROR_FAILURE:
			DisplayPopup((PU8)GetString(GetFileSystemErrorString(error_type)),IMG_GLOBAL_ERROR, TRUE, UI_POPUP_NOTIFYDURATION_TIME,ERROR_TONE);
			break;
	}
	DeleteUptoScrID(SCR_ID_THM_MENU);
}

/*****************************************************************************
* FUNCTION
*	ThemeDelete
* DESCRIPTION
*	Function to update set order during deletion
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void UpdateOrder(U32 thm_del_ID)
{
	U16 i=0;
	U8 count=0;
	U16 tmpidlist[TOTAL_THEMES_COUNT];

	g_thm_cntx.TotalTheme=tm_get_total_themes_count();

	if(g_thm_cntx.CurrTheme==thm_del_ID)
	{
		g_thm_cntx.CurrTheme=DefaultThmID;
	        g_thm_cntx.CurrThemeIndex=DefaultThmIndex;
	}

	for(i=0;i<=g_thm_cntx.CurrOrderIndex;i++)
	{
	if(thm_del_ID==g_thm_cntx.SetOrderIDList[i])
	count++;
	}
	if(g_thm_cntx.CurrOrderIndex >=count)
	{
	g_thm_cntx.CurrOrderIndex=g_thm_cntx.CurrOrderIndex-count;
	}
	count=0;

	for(i=0;i<g_thm_cntx.TotalTheme+1;i++)
	{
		if(thm_del_ID!=g_thm_cntx.SetOrderIDList[i])
		{
			tmpidlist[count]=g_thm_cntx.SetOrderIDList[i];
			count++;
		}
	}
	for(i=0;i<=count;i++)
	{
		g_thm_cntx.SetOrderIDList[i]=tmpidlist[i];

	}
	for(i=count;i<g_thm_cntx.TotalTheme;i++)
	{

		g_thm_cntx.SetOrderIDList[i]=0;

	}


		SetOrderIDList(g_thm_cntx.SetOrderIDList,count,g_thm_cntx.CurrOrderIndex);
	WriteThemesToNVRAM();
}
#endif

/*****************************************************************************
* FUNCTION
*	UpdateThemeOff
* DESCRIPTION
*	Cancel the update alarm set by user and write to NVRAM 
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void UpdateThemeOff(void)
{
	g_thm_cntx.CurrOrderIndex = -1;
	g_thm_cntx.IsOrderChanged = 0;

	/* if previous setting is not off, some theme-alarm is set before, cancel the alarm */
	if(g_thm_cntx.CurrUpdatePeriod >0)
		AlmCancelAlarm(ALM_THM_START);
		
	g_thm_cntx.CurrUpdatePeriod = THM_UPDATE_OFF;

	WriteThemesToNVRAM();

	DisplayPopup((PU8) GetString(STR_THEME_OFF_UPADTE_SUCCESS),IMG_GLOBAL_ACTIVATED, TRUE, UI_POPUP_NOTIFYDURATION_TIME,0);
	DeleteUptoScrID(SCR_ID_THM_OPTIONS);
	g_thm_cntx.IsInThmApp = 0;		
}

/*****************************************************************************
* FUNCTION
*	UpdateThemePeriod
* DESCRIPTION
*	Set update period and write to NVRAM 
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void UpdateThemePeriod(void)
{
 
#ifdef __MMI_THEMES_V2_SUPPORT__

	if(g_thm_cntx.IsSetFromMenu == 1 && g_thm_cntx.CurrHiliteItem!=g_thm_cntx.CurrUpdatePeriod )
	{
		g_thm_cntx.CurrUpdatePeriod = g_thm_cntx.CurrHiliteItem;
		g_thm_cntx.IsOrderChanged = 1;
	}
#else
	if(g_thm_cntx.IsSetFromMenu == 1)
		g_thm_cntx.CurrUpdatePeriod = g_thm_cntx.CurrHiliteItem;
#endif	
	SetStartPositionofCurIndex();

	WriteThemesToNVRAM();

	AlmSetAlarm(ALM_THM_START);
	
	if(g_thm_cntx.IsSetFromMenu == 1) /* not OFF */
	{
		DisplayPopup((PU8) GetString(STR_THEME_OFF_UPADTE_SUCCESS),IMG_GLOBAL_ACTIVATED,TRUE, UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
		DeleteUptoScrID(SCR_ID_THM_OPTIONS);
		g_thm_cntx.IsInThmApp = 0;			
	}
}
 
#ifdef __MMI_UI_TAB_PANE__
S32 theme_alarm_update_ui_theme=0;
/*****************************************************************************
* FUNCTION
*	SetThemeAlarmCallBackUpdateUITheme
* DESCRIPTION
*	Sets Theme Ui Update to TRUE. This is required so that UI theme is updated
*	on screen exit
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void SetThemeAlarmCallBackUpdateUITheme(void)
{
	theme_alarm_update_ui_theme=1;
}
/*****************************************************************************
* FUNCTION
*	ThemeAlarmCallBackUpdateUITheme
* DESCRIPTION
*	The function updates the UI Theme on common screen exit
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void ThemeAlarmCallBackUpdateUITheme(void)
{
	if(theme_alarm_update_ui_theme)
	{
		theme_alarm_update_ui_theme=0;
		check_and_set_title_theme_changed();
	}
}
#endif
 

/*****************************************************************************
* FUNCTION
*	ThemeAlarmCallBack
* DESCRIPTION
*  	Call back function when theme alarm expires
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void ThemeAlarmCallBack(void)
{

#ifdef __MMI_THEMES_V2_SUPPORT__ 
	U8 no_of_times,result;
	U16 order_theme_id,count,Index;
	U32 *p;
	
	count=GetIDList(&p,&g_thm_cntx.CurrOrderIndex);
	g_thm_cntx.CurrTheme=tm_get_current_theme_id(); //Ritesh

	g_thm_cntx.TotalTheme=tm_get_total_themes_count();
	for(no_of_times =0; no_of_times < g_thm_cntx.TotalTheme; no_of_times++)
	{
		g_thm_cntx.SetOrderIDList[no_of_times]=p[no_of_times];
	}
	
	
	Index=g_thm_cntx.CurrOrderIndex;
	if(count==0)
		UpdateThemeOff();
	else
		for(no_of_times =0; no_of_times < g_thm_cntx.TotalTheme; no_of_times++)
		{
			if((g_thm_cntx.CurrOrderIndex == (g_thm_cntx.TotalTheme)-1) || g_thm_cntx.CurrOrderIndex >=g_thm_cntx.TotalTheme)
				g_thm_cntx.CurrOrderIndex = 0;
			else
				g_thm_cntx.CurrOrderIndex++;

			if(g_thm_cntx.CurrOrderIndex==Index)
				break;

			order_theme_id=g_thm_cntx.SetOrderIDList[g_thm_cntx.CurrOrderIndex]; 

			if(order_theme_id==g_thm_cntx.CurrTheme)
			{
				SetCurrOrderIndex(g_thm_cntx.CurrOrderIndex);
				break;
			}
			if(!g_alm_frm_cntx.IsPwronAlarm)
			{
				if(g_thm_cntx.IsInThmApp == 0)  //is not in theme menu 
				{		
					result=tm_activate_theme(order_theme_id);

					if(result==THEME_ERROR_SUCCESS)
					{
						g_thm_cntx.CurrTheme=order_theme_id;
						DisplayPopup((PU8)GetString(STR_THEMES_ALARM_ACTIVATION),  IMG_GLOBAL_ACTIVATED,TRUE, UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
						SetCurrOrderIndex(g_thm_cntx.CurrOrderIndex);
						break;
					}
				}
		}
	}
 
	WriteThemesToNVRAM();
 
#else

	U8 no_of_times;
	U8 order_theme;
	/* find the next index of theme to be updated */
	for(no_of_times =0; no_of_times < NO_OF_ORDERS; no_of_times++)
	{
		/* move order to next one */
		if(g_thm_cntx.CurrOrderIndex == (NO_OF_ORDERS)-1)
			g_thm_cntx.CurrOrderIndex = 0;
		else
			g_thm_cntx.CurrOrderIndex++;

		order_theme = g_thm_cntx.CurrOrderList[g_thm_cntx.CurrOrderIndex];
		
		/* if found theme different to current one, found the first one */
		if (order_theme != g_thm_cntx.CurrTheme && order_theme < g_thm_cntx.TotalTheme)
			break;					
	}

	if(no_of_times < NO_OF_ORDERS)
	{
		g_thm_cntx.CurrTheme = order_theme;
		WriteThemesToNVRAM();

		if(!g_alm_frm_cntx.IsPwronAlarm)
		{
			if(g_thm_cntx.IsInThmApp == 0) /* is not in theme menu */
			{
				SetThemes(g_thm_cntx.CurrTheme); 
 
#ifdef __MMI_UI_TAB_PANE__
	SetThemeAlarmCallBackUpdateUITheme();
#endif
 
 
//				EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
//				GoBackHistory();
			}
			else if(g_thm_cntx.IsInThmApp == 1)
			{
				//SetExitHandler(SCR_ID_THM_MENU, NULL);
				ClearExitHandler();
				EntryThmMenu();
			}
			order_theme++;
		}
    }
#endif
 
//	WriteThemesToNVRAM();
 

}

/*****************************************************************************
* FUNCTION
*	ReadThemesFromNVRAM
* DESCRIPTION
*	Read theme settings from NVRAM 
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void ReadThemesFromNVRAM(void)
{
 
#ifndef __MMI_THEMES_V2_SUPPORT__ 
	S16 error = 0;
	U8  index= 0;
	U8 temp_name=0;
	U8* tmp_ptr = &temp_name;
	U8** tmp_ptr_ptr = &tmp_ptr;
	thm_nvram_struct nvram_theme;

	/* initialize global contex */
	memset(&g_thm_cntx, 0, sizeof(g_thm_cntx));
	g_thm_cntx.TotalTheme=(U8)GetThemesNames(&tmp_ptr_ptr);

	ReadRecord(NVRAM_EF_THEMES_VALUES,1,&nvram_theme,8,&error);

	g_thm_cntx.CurrUpdatePeriod = (U8)(nvram_theme.freq_type - ALM_FREQ_THM_OFF);

	g_thm_cntx.CurrTheme = nvram_theme.current_theme_index;

	/* set order list */
//	if(nvram_theme.set_order_flag >= 0)	/* order has be set before */
	if(nvram_theme.set_order_flag != 255)	/* order has be set before */
	{	
		g_thm_cntx.IsOrderChanged = 1;

		g_thm_cntx.CurrOrderIndex = nvram_theme.set_order_flag;

		for(index = 0; index<NO_OF_ORDERS;index++)
			g_thm_cntx.CurrOrderList[index] = nvram_theme.themes_order[index];
	}
	else
	{
		g_thm_cntx.IsOrderChanged = 0;
		g_thm_cntx.CurrOrderIndex = 0;
		for(index = 0;index <NO_OF_ORDERS;index++)
			g_thm_cntx.CurrOrderList[index] = index;
	}

	/* set and apply current theme */
	if(g_thm_cntx.CurrTheme >= g_thm_cntx.TotalTheme)
	{
		g_thm_cntx.CurrOrderIndex = 0;
		g_thm_cntx.CurrTheme = 0;
	}

	SetThemes(g_thm_cntx.CurrTheme);
#endif
 
}

/*****************************************************************************
* FUNCTION
*	ConstructThemeOrderList
* DESCRIPTION
*	function to construct themes order list
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
#ifdef __MMI_THEMES_V2_SUPPORT__
void ConstructThemeOrderList(void)
{
	S16 error = 0;
	U8  index= 0;
	thm_nvram_struct nvram_theme;

	/* initialize global contex */

	
	g_thm_cntx.TotalTheme=tm_get_total_themes_count();
#if (defined(__MMI_THEMES_V2_SUPPORT__) || defined(__MMI_DOWNLOADABLE_THEMES_SUPPORT__)) //ritesh	
	ReadRecord(NVRAM_EF_THEMES_VALUES,1,&nvram_theme,96,&error);
#endif

	g_thm_cntx.CurrUpdatePeriod = (U8)(nvram_theme.freq_type );

	/* set order list */
	if(nvram_theme.set_order_flag >= 0)	/* order has be set before */
	{	
		g_thm_cntx.IsOrderChanged = 1;
		g_thm_cntx.CurrOrderIndex = (U8)nvram_theme.set_order_flag;
		currorderindex=(U8)nvram_theme.set_order_flag;
		
		for(index = 0; index<TOTAL_THEMES_COUNT;index++)//ritesh
		{
			//g_thm_cntx.CurrOrderList[index] = nvram_theme.themes_order[index];
			setorderIDList[index]=nvram_theme.themes_order[index];
		}
	}
	else
	{
		g_thm_cntx.IsOrderChanged = 0;
}

	/* set and apply current theme */
}
#endif

/*****************************************************************************
* FUNCTION
*	WriteThemesToNVRAM
* DESCRIPTION
*  	write theme settings to NVRAM 
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void WriteThemesToNVRAM()
{
	S32 index =0;
	S16 error = 0;
	thm_nvram_struct nvram_theme;

#ifdef __MMI_THEMES_V2_SUPPORT__ 
	nvram_theme.freq_type = g_thm_cntx.CurrUpdatePeriod;
#else
	nvram_theme.freq_type = g_thm_cntx.CurrUpdatePeriod + ALM_FREQ_THM_OFF;
#endif	
//	if((g_thm_cntx.IsOrderChanged == 1) &&( g_thm_cntx.CurrOrderIndex == -1))
//		nvram_theme.set_order_flag = 0;
//	else
	if(g_thm_cntx.IsOrderChanged == 1)
#ifdef __MMI_THEMES_V2_SUPPORT__ 
		nvram_theme.set_order_flag =  currorderindex;	//changes made on saturday 09/04/05
#else
		nvram_theme.set_order_flag = g_thm_cntx.CurrOrderIndex;
#endif
	
	if(g_thm_cntx.IsOrderChanged == 0)
		nvram_theme.set_order_flag = 255;//-1;

#ifndef __MMI_THEMES_V2_SUPPORT__ 
	if(g_thm_cntx.CurrTheme < g_thm_cntx.TotalTheme)
		nvram_theme.current_theme_index =(U8)g_thm_cntx.CurrTheme;
	else
		g_thm_cntx.CurrTheme = nvram_theme.current_theme_index = 0;
#endif

#ifdef __MMI_THEMES_V2_SUPPORT__ 
	for(index = 0; index<TOTAL_THEMES_COUNT;index++)//ritesh
		nvram_theme.themes_order[index]= setorderIDList[index];
#else
	for(index = 0; index<NO_OF_ORDERS;index++)
		nvram_theme.themes_order[index] =  g_thm_cntx.CurrOrderList[index];
#endif

#if (defined(__MMI_THEMES_V2_SUPPORT__) || defined(__MMI_DOWNLOADABLE_THEMES_SUPPORT__)) //ritesh
	WriteRecord(NVRAM_EF_THEMES_VALUES,1,&nvram_theme,96,&error);
#else
	WriteRecord(NVRAM_EF_THEMES_VALUES,1,&nvram_theme,8,&error);
#endif
}

BOOL RstThemeDefault(void)
{
	S16 error = 0;
	thm_nvram_struct nvram_theme;	
	nvram_theme.freq_type = 0x5;
	nvram_theme.current_theme_index = 0;
	nvram_theme.set_order_flag = 0xffff;
	memset(nvram_theme.themes_order, 0xff, 4);
	
	WriteRecord(NVRAM_EF_THEMES_VALUES,1,&nvram_theme, sizeof(nvram_theme),&error);

	if (error == NVRAM_WRITE_SUCCESS)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*****************************************************************************
* FUNCTION
*	UpdateThemeAlarm
* DESCRIPTION
*  	Call by settings application when time/date change
*	and set the alam of update theme according to current time/date 
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void UpdateThemeAlarm(void)
{
	g_thm_cntx.IsSetFromMenu = 0;

	if(g_thm_cntx.CurrUpdatePeriod > 0) /* not OFF */
		UpdateThemePeriod();
}

/*****************************************************************************
* FUNCTION
*	SetStartPositionofCurIndex
* DESCRIPTION
*  	set the starting position of update order index
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void SetStartPositionofCurIndex(void)
{
	U8 index = 0,flag = 0;
	U8 curr_order;

#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
	if(g_thm_cntx.CurrOrderIndex < 0 && g_thm_cntx.CurrOrderIndex >= TOTAL_THEMES_COUNT) 
#else
	if(g_thm_cntx.CurrOrderIndex < 0)
#endif
		curr_order = 0;
	else
		curr_order = (U8)g_thm_cntx.CurrOrderIndex;
	
	/* find the position of current selected theme */
#ifdef __MMI_THEMES_V2_SUPPORT__ 
	for(index = 0; index<g_thm_cntx.TotalTheme;index++)//ritesh
	{
		if(g_thm_cntx.SetOrderIDList[curr_order] == g_thm_cntx.CurrTheme)
		{
			flag = 1;
			break;
		}
		if(curr_order == (g_thm_cntx.TotalTheme)-1)
			curr_order = 0;
		else
			curr_order++;
	}
#else
	for(index= 0 ;index<NO_OF_ORDERS ;index++)
	{
		if(g_thm_cntx.CurrOrderList[curr_order] == g_thm_cntx.CurrTheme)
		{
			flag = 1;
			break;
		}

		if(curr_order == (NO_OF_ORDERS)-1)
			curr_order = 0;
		else
			curr_order++;
	}
#endif
	if(flag == 1) 	/* it's in the list */
		g_thm_cntx.CurrOrderIndex = curr_order;
	else
		g_thm_cntx.CurrOrderIndex = -1;

}

/*****************************************************************************
* FUNCTION
*	ATSetTheme
* DESCRIPTION
*  	To change theme setting from AT command
* PARAMETERS
*	index  IN		index of theme to be set
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
BOOL ATSetTheme(U8 index)
{
	/* added by ritesh for V2 support*/
#ifdef __MMI_THEMES_V2_SUPPORT__
	U16 result;
	if(g_thm_cntx.IsInThmApp == 0)
	{
		tm_get_themes(&thm_details);
		result=tm_activate_theme(thm_details[index].theme_id);
	
		switch(result)
		{
			case THEME_ERROR_SUCCESS:
				g_thm_cntx.CurrTheme = thm_details[index].theme_id;
				if(g_thm_cntx.CurrUpdatePeriod >0)
				{
					SetStartPositionofCurIndex();
				}
				SetCurrOrderIndex(g_thm_cntx.CurrOrderIndex);
				DisplayPopup((PU8)GetString(STR_THEME_ACTIVATE_SUCCESS),  IMG_GLOBAL_ACTIVATED,TRUE, UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE);
				DeleteUptoScrID(SCR_ID_THM_MENU);
				return MMI_TRUE;
			//	break;
			case THEME_ERROR_ALREADY_ACTIVATED:
				DisplayPopup((PU8)GetString(STR_ID_FMGR_ALREADY_ACTIVE),  IMG_GLOBAL_ACTIVATED,TRUE, UI_POPUP_NOTIFYDURATION_TIME,ERROR_TONE);
				return MMI_FALSE;
			//	break;
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
			case THEME_ERROR_THEME_NOT_AVAILABLE:
				DisplayPopup((PU8)GetString(STR_THEME_NOT_AVAILABLE),  IMG_GLOBAL_ERROR,TRUE, UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
				return MMI_FALSE;
				//break;
			case THEME_ERROR_FILE_FORMAT_BAD:
				DisplayPopup((PU8)GetString(STR_THEME_FORMAT_NOT_SUPPORTED),  IMG_GLOBAL_ERROR,TRUE, UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE);
				return MMI_FALSE;
				//break;
			case THEME_ERROR_FAILURE:
				DisplayPopup((PU8)GetString(STR_THEME_UNKNOWN_ERROR),  IMG_GLOBAL_ERROR,TRUE, UI_POPUP_NOTIFYDURATION_TIME,ERROR_TONE);
				return MMI_FALSE;
				//break;
			
			case THEME_ERROR_VERSION_FAILURE:
				DisplayPopup((PU8)GetString(STR_DOWNLOAD_THEME_UNSUPPORTED), IMG_GLOBAL_WARNING, 0, UI_POPUP_NOTIFYDURATION_TIME, WARNING_TONE);
				return MMI_FALSE;
				//break;
			case THEME_ERROR_PROCESS_QUEUE_SUCESS:
				DisplayPopup(NULL ,IMG_PROCESSING_PHONEBOOK, 1,UI_POPUP_NOTIFYDURATION_TIME,0);
				return MMI_FALSE;
				//break;
			 default:
				DisplayPopup((PU8)GetString(STR_THEME_UNKNOWN_ERROR),  IMG_GLOBAL_ERROR,TRUE, UI_POPUP_NOTIFYDURATION_TIME,ERROR_TONE);
				return MMI_FALSE;
				//break;
#endif
	
		}

		WriteThemesToNVRAM();
	}
	else
	{
		return MMI_FALSE;
	}
#else
	/* index exceeds boundary or theme is not changed */
	if(index >= g_thm_cntx.TotalTheme|| index  == g_thm_cntx.CurrTheme)
		return MMI_FALSE;

	g_thm_cntx.CurrTheme = index;

	/* reset postion index of update period */
	if(g_thm_cntx.CurrUpdatePeriod > 0)
		SetStartPositionofCurIndex();

	/* write change to NVRAM */
	WriteThemesToNVRAM();

	/* change theme */
	if(g_thm_cntx.IsInThmApp == 0) /* is not in theme menu */
	{
		SetBothLCDThemes(index);
 
		EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
		GoBackHistory();
	}
	else if(g_thm_cntx.IsInThmApp == 1)
	{
//		SetExitHandler(SCR_ID_THM_MENU, NULL);
		ClearExitHandler();
		EntryThmMenu();
	}	

	return MMI_TRUE;
#endif
}
#ifdef __MMI_THEMES_V2_SUPPORT__ 
/*****************************************************************************
* FUNCTION
*	SetOrderIDList
* DESCRIPTION
*  	To construct set order ID list
* PARAMETERS
*	three:
*pointer to array of ID list
*total no of themes in set order list
*index of the activated theme
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void SetOrderIDList(U32 IDList[],U8 SetNoOfThemes, U8 index)
{
	U8 i,count=0;
	for(i=0;i<g_thm_cntx.TotalTheme;i++)
	{
		setorderIDList[i]=IDList[i];
	}
	currorderindex=index;
	WriteThemesToNVRAM();
}
/*****************************************************************************
* FUNCTION
*	GetIDList
* DESCRIPTION
*  	function to get set order ID List
* PARAMETERS
*	pointer to array of ID list and pointer to current order index
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
U8 GetIDList(U32 **GetIDList, U8 *index)
{
U8 count;
count=CheckForCorrectOrder();

if(count==0)
currorderindex=-1;
*GetIDList=setorderIDList;
*index=currorderindex;
WriteThemesToNVRAM();
return count;
}
/*****************************************************************************
* FUNCTION
*	CheckForCorrectOrder
* DESCRIPTION
*  	function to check for correct order in set order list
* PARAMETERS
*	none
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
U8 CheckForCorrectOrder(void)
{
	U16 i,j=0,set_flag=0;
	U8 count=0,totaltheme;
	U32 tempIDList[TOTAL_THEMES_COUNT];
	totaltheme=tm_get_themes(&thm_details);
	
	
	ConstructThemeOrderList();
	
	for(i=0;i<TOTAL_THEMES_COUNT;i++)
		{
		while(j!=totaltheme)
			{
			
			if(setorderIDList[i]==0)
			{
			tempIDList[i]=setorderIDList[i];
			set_flag=1;
			break;
			}
			if(setorderIDList[i]==thm_details[j].theme_id)
			{
			tempIDList[i]=setorderIDList[i];
			count++;
			set_flag=1;
			break;
			}
			j++;	
		}
		if(set_flag==0)
			{
				
				tempIDList[i]=-1;
				if(currorderindex >=i)
				currorderindex--;
			}
		set_flag=0;
		j=0;
		}
			

	/*	if(j==g_thm_cntx.TotalTheme-1 && setorderIDList[i]!=thm_details[j].theme_id )
			{
			if(currorderindex>0 && i<currorderindex)
			currorderindex=currorderindex-1;
			break;
			}*/
	 for(i=0;i<TOTAL_THEMES_COUNT;i++)
	 {
		 if(tempIDList[i]!=-1)
			setorderIDList[set_flag++]=tempIDList[i];
	 }
	 for(i=totaltheme;i<TOTAL_THEMES_COUNT;i++)
			setorderIDList[i]=-1;
	  return count;
}
/*****************************************************************************
* FUNCTION
*	SetCurrOrderIndex
* DESCRIPTION
*  	function to set current order index
* PARAMETERS
*	index of current theme in set order list
* RETURNS
*	none
* GLOBALS AFFECTED
*	none
*****************************************************************************/
void SetCurrOrderIndex(U8 index)
{
	currorderindex=index;
	WriteThemesToNVRAM();
}

#endif
#endif //__MMI_THEMES_APPLICATION__
#endif // _MMI_THEME_C


