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
 * DateAndTime.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for date/time setting application
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
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: DateAndTime.c

  	PURPOSE		: nil

 

 

	DATE		: Oct 24, 2003

**************************************************************/
#ifndef _DATEANDTIME_C
#define _DATEANDTIME_C

/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"

#include "mainmenudef.h"
#include "alarmframeworkprot.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "wgui_categories.h"
#include "wgui_datetime.h"
#include "wgui_categories_sublcd.h"
#include "globaldefs.h"
#include "idleappdef.h"
#include "hardwareevents.h"
#include "debuginitdef.h"
#include "audioinc.h"
#include "thrdosinterface.h"
#include "osthreadinterfacegprot.h"
#include "callsdefs.h"
#include "protocolevents.h"
#include "settingstructs.h"
#include "settingsgexdcl.h"
#include "settinggprots.h"
#include "settingprot.h"
#include "simdetectiongprot.h"
#include "queuegprot.h"
#include "protocolevents.h"
#include "settingprofile.h"
#include "l4dr.h"
#include "commonscreens.h"
#include "worldclock.h"
#include "alarmgprot.h"
#include "idleappprot.h"
#include "datetimegprot.h"
#include "wallpaperdefs.h"
#include "downloaddefs.h"
#include "screensaverdefs.h"
#include "phonesetup.h"
#include "phonesetupgprots.h"
#include "nitz.h"
#include "wgui_inline_edit.h"  /* for max_inline_items */
/*  Include: PS header file */
#include "mmi_trace.h"
#undef __NEWSIMULATOR 

/* 
** Define
*/
#define NUM_CITYS (NUMBER_OF_CITY-1)
#define DOUBLE_DIGIT 10


typedef enum {

#ifndef __MMI_HIDE_HOMECITY__
   PHNSET_DT_MNU_HOMECT,
#endif

#ifndef __MMI_SEPARATE_DT_SETTING__
	PHNSET_DT_MNU_DT,
	PHNSET_DT_MNU_DATEANDFORMAT=PHNSET_DT_MNU_DT,
	PHNSET_DT_MNU_FMT,
	PHNSET_DT_MNU_TIMEANDFORMAT=PHNSET_DT_MNU_FMT,
#else   
	PHNSET_DT_MNU_DT,
	PHNSET_DT_MNU_FMT,
	PHNSET_DT_MNU_DATEANDFORMAT,
	PHNSET_DT_MNU_TIMEANDFORMAT,
#endif

#if (defined(__MMI_NITZ__) && defined(__MMI_AUTOTIMEZONE_IN_DATEANDTIME__) )
   PHNSET_DT_MNU_NITZ,
#endif

   PHNSET_DT_MNU_LAST
} phnset_DT_menu_enum;


#ifndef YEARFORMATE
	#define YEARFORMATE 2000
#endif

#ifndef NEGATIVE
	#define NEGATIVE 0
#endif

#ifndef POSITIVE
	#define POSITIVE 1
#endif
/* 
** Typedef 
*/


/* 
** Local Variable
*/
static U8 *date_formats[NUM_DATE_FORMATS] ;
static U8 *time_formats[NUM_TIME_FORMATS] ;
//static U8 *menu_time_date_list[PHNSET_DT_MNU_LAST];
static U8 *DSTString[2];

//static S8 tmpDSTHour[MAX_LENGTH * ENCODING_LENGTH];
/* 
** Global Variable
*/
extern MYTIME MyTime;
extern BOOL SetFromAT;
extern rtc_format_struct gclockData;
extern PU8 subMenuDataPtrs[];
BOOL SetFromAT=FALSE;// wangbei add 20060706
#if defined(__MMI_NITZ__)
extern nitz_context_struct g_nitz_cntx;// wangbei add 20060706
#endif
/* 
** Global Function
*/
extern void EntryIdleScreen(void);
extern void DateTimerProc(void);
//extern void updateSPOFDateChanged(void) ;
extern void updateAlarmsDateChanged(void) ;

//add by zough 061121 start
#ifndef __MOD_ORGANIZER__
extern FLOAT GetTimeZone(U8 cityIndex);
#else
extern S32 GetTimeZone(U8 cityIndex);
#endif
//add by zough 061121 end

#ifdef __MMI_TODOLIST__
extern void InitToDoListApp(void);
#endif



/* 
** Time and Date
*/


/*****************************************************************************
* FUNCTION
*  HighlightPhnsetTimeAndDate
* DESCRIPTION
*   This function is Highlight handler for Phonesetup-> Time and Date
*   Follow functions: EntryPhnsetTimeAndDate
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightPhnsetTimeAndDate(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK,0);
	SetLeftSoftkeyFunction(EntryPhnsetTimeAndDate,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetTimeAndDate,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	//datetimeindex=0;
	g_phnset_cntx_p->curHightlightItem = 0;
}



/*****************************************************************************
* FUNCTION
*  EntryPhnsetIdleScrDisp
* DESCRIPTION
*   This function is Entry function for Phonesetup->Time and Date
*   Following functions: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void EntryPhnsetTimeAndDate(void)
{
	U8* guiBuffer;       /* Buffer holding history data */
	U16 nNumofItem=0;
	U8*	pop_up_datetime[MAX_SUB_MENUS]; /*Pointers to HintHandlers */
	U16 menu_time_date_list[MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	
	TBM_ENTRY(0x28D0);

	/*Call Exit Handler*/
	EntryNewScreen(SCR9041_TIMEANDDATE, ExitPhnsetTimeAndDate, EntryPhnsetTimeAndDate, NULL);

	/*Get current screen to gui buffer  for history purposes*/
	guiBuffer = GetCurrGuiBuffer(SCR9041_TIMEANDDATE);	

	/*Retrieve no of child of menu item to be displayed */
	nNumofItem = GetNumOfChild(MENU9141_TIME_AND_DATE);

	/*Retrieve string ids in sequence of given menu item to be displayed */
	GetSequenceStringIds(MENU9141_TIME_AND_DATE, menu_time_date_list);

	/*Set current parent id*/
	SetParentHandler(MENU9141_TIME_AND_DATE);


	/*Register highlight handler to be called in menu screen */
	//RegisterHighlightHandler(PhnsetTimeAndDateHiliteHandler);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	//if this list is not constructed before entry home city then,after bootup popup will be empty--vanita
	ConstructHintsList(MENU9141_TIME_AND_DATE, pop_up_datetime);



	/*Display Category Screen */
	ShowCategory52Screen(  STR_MENU9141_TIME_AND_DATE, MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, menu_time_date_list,(PU16)gIndexIconsImageList,
						  pop_up_datetime,
						  0,
						  g_phnset_cntx_p->curHightlightItem,
						  guiBuffer);
	
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	TBM_EXIT(0x28D0);

}

/*****************************************************************************
* FUNCTION
*  mmi_dt_set_unset_dst
* DESCRIPTION
*   This function is Highlight handler for Phonesetup-> Time and Date
*   Follow functions: EntryPhnsetMainMenuStyle
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
#if 0
/* under construction !*/
/* under construction !*/
#ifdef MMI_ON_HARDWARE_P
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
/* under construction !*/
#endif	

/*****************************************************************************
* FUNCTION
*  ExitPhnsetTimeAndDate
* DESCRIPTION
*   This function is Exit function for Phonesetup->Time and Date
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void ExitPhnsetTimeAndDate(void)
{
#if (defined(__MMI_NITZ__) && defined(__MMI_AUTOTIMEZONE_IN_DATEANDTIME__) )
    S16 error;

    if( g_phnset_cntx_p->NITZDirtyFlag == TRUE )
    {
        WriteValue( NVRAM_SETTING_AUTOUPDATE_DT_TIME, &(g_phnset_cntx_p->curNITZStatus), DS_BYTE , &error);
        g_phnset_cntx_p->NITZDirtyFlag = FALSE;
    }
#endif /* #if (defined(__MMI_NITZ__) && defined(__MMI_AUTOTIMEZONE_IN_DATEANDTIME__) ) */
}


#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#ifndef __MMI_HIDE_HOMECITY__
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if (defined(__MMI_NITZ__) && defined(__MMI_AUTOTIMEZONE_IN_DATEANDTIME__) )
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /* 0 */


/* 
** Set Date/Time
*/

extern void Category53ChangeItemDescription(s32 index,U8 *text);

/*****************************************************************************
* FUNCTION
*  HintPhnsetTimeAndDate
* DESCRIPTION
*   This function is hint handling function 
*     for Phonesetup -> Time and Data ( Set Home City, Auto Update Date and Time)
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HintPhnsetTimeAndDate(U16 index) 
{
	if(index == 0) /* Time and Date */
	{
		  
	        subMenuDataPtrs[g_phnset_cntx_p->currentCity]=(PU8)((GetString)((U16)(TimeZoneData[g_phnset_cntx_p->currentCity+1].CityName)));
	        g_phnset_cntx_p->CurHomeCityStrPtr = subMenuDataPtrs[g_phnset_cntx_p->currentCity];
	        
	       
		pfnUnicodeStrcpy( ( S8 * ) hintData[index], ( S8 * ) g_phnset_cntx_p->CurHomeCityStrPtr );
	}
#if( defined( __MMI_AUTOTIMEZONE_IN_DATEANDTIME__) && defined(__MMI_NITZ__) )
	else if( index == 3 ) /* NITZ */
	{
		if(g_phnset_cntx_p->curNITZStatus)
		{
			pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON));
			Category53ChangeItemDescription(index,hintData[index]);
			ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
		}
		else
		{
			pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));
			Category53ChangeItemDescription(index,hintData[index]);
			ChangeLeftSoftkey(STR_GLOBAL_ON,0);
		}
		RedrawCategoryFunction();
	}
#endif  /* #if( defined( __MMI_AUTOTIMEZONE_IN_DATEANDTIME__) && defined(__MMI_NITZ__) ) */
	else  /* Others, no hint */
		pfnUnicodeStrcpy( ( S8 * ) hintData[index], NULL);
}



/*****************************************************************************
* FUNCTION
*  HighlightPhnsetSetHomeCity
* DESCRIPTION
*   This function is Highlight handler for Phonesetup-> Time And Date -> Set Home City
*   Follow functions: EntryPhnsetSetHomeCity
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
#ifndef __MMI_HIDE_HOMECITY__
void HighlightPhnsetSetHomeCity(void)
{
	//datetimeindex = 0;
     g_phnset_cntx_p->curHightlightItem = PHNSET_DT_MNU_HOMECT;
	
	SetLeftSoftkeyFunction(EntryPhnsetSetHomeCity,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetSetHomeCity, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);  
}
#endif
/*****************************************************************************
* FUNCTION
*  PhnsetTimeEditorCallback
* DESCRIPTION
*   This function is to fills the Data to set the time in case Set Time is called from AT.
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
//static void setting_timeedit_callback(U8 *par1, U8 *par2,U8 *par3,U8 *par4)
void PhnsetTimeEditorCallback(U8 *par1, U8 *par2,U8 *par3,U8 *par4)
{
	if((par2[0]== 0 && par3[0] == 0) )
	{
	#ifdef __UCS2_ENCODING	
		pfnUnicodeStrcat((PS8)par1,(PS8)gHour);
		pfnUnicodeStrcat((PS8)par1,(PS8)":");
		pfnUnicodeStrcat((PS8)par1,(PS8)gMinute);
	#else 
		strcat((PS8)par1,(PS8)gHour);
		strcat((PS8)par1,(PS8)"/");
		strcat((PS8)par1,(PS8)gMinute);
	#endif
	}
	else
	{
	#ifdef __UCS2_ENCODING	
		pfnUnicodeStrcpy((PS8)par1,(PS8)par2);
		pfnUnicodeStrcat((PS8)par1,(PS8)":");
		pfnUnicodeStrcat((PS8)par1,(PS8)par3);
	#else 
		strcpy((PS8)par1,(PS8)par2);
		strcat((PS8)par1,(PS8)":");
		strcat((PS8)par1,(PS8)par3);
	#endif
	}
	
}




/*****************************************************************************
* FUNCTION
*  PhnsetDateEditorCallback
* DESCRIPTION
*   This function is to fills the Data to set the time in case Set Time is called from AT.
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
//void setting_dateedit_callback(U8 *par1, U8 *par2,U8 *par3,U8 *par4)
void PhnsetDateEditorCallback(U8 *par1, U8 *par2,U8 *par3,U8 *par4)
{
	if((par2[0]== 0 && par3[0] == 0) )
	{
	#ifdef __UCS2_ENCODING	
		pfnUnicodeStrcpy((PS8)par1,(PS8)gYear);
		pfnUnicodeStrcat((PS8)par1,(PS8)"/");
		pfnUnicodeStrcat((PS8)par1,(PS8)gMonth);
		pfnUnicodeStrcat((PS8)par1,(PS8)"/");
		pfnUnicodeStrcat((PS8)par1,(PS8)gDate);
	#else 
		strcpy((PS8)par1,(PS8)gYear);
		strcat((PS8)par1,(PS8)"/");
		strcat((PS8)par1,(PS8)gMonth);
		strcat((PS8)par1,(PS8)"/");
		strcat((PS8)par1,(PS8)gDate);
	#endif
	}
	else
	{
	#ifdef __UCS2_ENCODING	
		pfnUnicodeStrcpy((PS8)par1,(PS8)par4);
		pfnUnicodeStrcat((PS8)par1,(PS8)"/");
		pfnUnicodeStrcat((PS8)par1,(PS8)par3);
		pfnUnicodeStrcat((PS8)par1,(PS8)"/");
		pfnUnicodeStrcat((PS8)par1,(PS8)par2);
	#else 
		strcpy((PS8)par1,(PS8)par4);
		strcat((PS8)par1,(PS8)"/");
		strcat((PS8)par1,(PS8)par3);
		strcat((PS8)par1,(PS8)"/");
		strcat((PS8)par1,(PS8)par2);
	#endif
	}
}
/*****************************************************************************
* FUNCTION
*  HighlightPhnsetSetDST
* DESCRIPTION
*   This function is Highlight handler for Phonesetup-> Time And Date -> Set Format
*   Follow functions: EntryPhnsetSetFormat
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightPhnsetSetDST(S32 index)
{

	/* no change */
	if (index != g_phnset_cntx_p->prevDST)
	{
		MYTIME incTime;
		MYTIME currTime;
		MYTIME result;
		U8 dummyString[5]; 

		memset(&incTime, 0, sizeof(MYTIME));
		memset(&currTime, 0, sizeof(MYTIME));		
		incTime.nHour = 1;

		currTime.nHour	=(U8) coolsand_UI_atoi((UI_string_type)gHour);
		currTime.nMin	=(U8) coolsand_UI_atoi((UI_string_type)gMinute);
		currTime.nMonth=(U8) coolsand_UI_atoi((UI_string_type)gMonth);
		currTime.nDay	=(U8) coolsand_UI_atoi((UI_string_type)gDate);
		currTime.nYear	=(U16)coolsand_UI_atoi((UI_string_type)gYear);
		if(index==0)
		{
			/* on -> off */
			DecrementTime(currTime, incTime, &result);
		}
		else
		{
			/* off -> on */
			result = IncrementTime(currTime, incTime);
		}
			

		sprintf(gHour,"%02d",result.nHour);
		sprintf(gMinute,"%02d",result.nMin);
		sprintf(gYear,"%2d",result.nYear);
		sprintf(gMonth,"%02d",result.nMonth);
		sprintf(gDate,"%02d", result.nDay);

		strcpy((PS8)dummyString,gHour);
		AnsiiToUnicodeString((PS8)gHour,(PS8)dummyString);

		strcpy((PS8)dummyString,gMinute);
		AnsiiToUnicodeString((PS8)gMinute,(PS8)dummyString);

		strcpy((PS8)dummyString,gYear);
		AnsiiToUnicodeString((PS8)gYear,(PS8)dummyString);

		strcpy((PS8)dummyString,gMonth);
		AnsiiToUnicodeString((PS8)gMonth,(PS8)dummyString);

		strcpy((PS8)dummyString,gDate);
		AnsiiToUnicodeString((PS8)gDate,(PS8)dummyString);

		g_phnset_cntx_p->prevDST = index;

		
	}
	RedrawCategory57Screen();
}


static U16 setting_date_time_list_of_icons[NUM_ITEMS_DATE_TIME_ENTRY_SCREEN] = { \
   IMG_TIME, IMG_TIME_AND_DATE_BLANK, IMG_CAL, IMG_TIME_AND_DATE_BLANK, SUN_ICON, IMG_TIME_AND_DATE_BLANK };
static U16 setting_date_time_format_list_of_icons[] = { \
   IMG_TIME, IMG_TIME_AND_DATE_BLANK, IMG_CAL, IMG_TIME_AND_DATE_BLANK };
#ifndef __MMI_SEPARATE_DT_SETTING__
/*****************************************************************************
* FUNCTION
*  HighlightPhnsetSetDT
* DESCRIPTION
*   This function is Highlight handler for Phonesetup-> Time And Date -> Set Time/Date
*   Follow functions: EntryPhnsetSetDT
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightPhnsetSetDT(void)
{
	//datetimeindex = 1;
	g_phnset_cntx_p->curHightlightItem = PHNSET_DT_MNU_DT;

	memset(gHour,0,sizeof(gHour));
	memset(gMinute,0,sizeof(gMinute));
	memset(gDate,0,sizeof(gDate));
	memset(gMonth,0,sizeof(gMonth));
	memset(gYear,0,sizeof(gYear));
	
	//highlightedDateSetItem =0;
	//g_phnset_cntx_p->curHightlightItem = 0;
	SetLeftSoftkeyFunction(EntryPhnsetSetDT,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetSetDT, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);   
}


/*****************************************************************************
* FUNCTION
*  HighlightPhnsetSetFormat
* DESCRIPTION
*   This function is Highlight handler for Phonesetup-> Time And Date -> Set Format
*   Follow functions: EntryPhnsetSetFormat
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightPhnsetSetFormat(void)
{
	//datetimeindex = 2;
     g_phnset_cntx_p->curHightlightItem = PHNSET_DT_MNU_FMT;
	
//	highlightedDateFormatItem = 0;
	SetLeftSoftkeyFunction(EntryPhnsetSetFormat,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetSetFormat, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);   
}

#endif //__MMI_SEPARATE_DT_SETTING__

/*****************************************************************************
* FUNCTION
*  PhnsetInlineSetDTHiliteHdlr
* DESCRIPTION
*   This function is Inline line editor Highlight handler for Phonesetup-> Time and Date -> Set Time/Date
*   Follow functions: PhnsetSetDT
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
static void PhnsetInlineSetDTHiliteHdlr(S32 currId)
{
	
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction(PhnsetSetDT);	
	g_phnset_cntx_p->curHightlightItem = (U8)currId;
}


/**************************************************************

	FUNCTION NAME		: EntryPhnsetSetDT(void)

  	PURPOSE				: Set Date And Time Entry Function

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryPhnsetSetDT(void)
{
	U8* guiBuffer;
	U8* inputBuffer;                       //added for inline edit history
	U16 inputBufferSize;                //added for inline edit history
     extern wgui_inline_item wgui_inline_items[];
	
	EntryNewScreen(SCR_ID_PHNSET_SET_DT, ExitPhnsetSetDT, NULL, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_PHNSET_SET_DT);
	InitializeCategory57Screen();
	
	PhnsetSetDTFillInlineStruct( (void *)wgui_inline_items);
	
	inputBuffer = GetCurrNInputBuffer (SCR_ID_PHNSET_SET_DT, &inputBufferSize );    //added for inline edit history

	if(inputBuffer != NULL)
		SetCategory57Data(wgui_inline_items, NUM_ITEMS_DATE_TIME_ENTRY_SCREEN, inputBuffer);

	SetLeftSoftkeyFunction(PhnsetSetDT,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	RegisterHighlightHandler(PhnsetInlineSetDTHiliteHdlr);


	ShowCategory57Screen(STR_ID_PHNSET_SET_DT,IMG_SCR_SETTING_CAPTION,
	                                        STR_GLOBAL_OK,IMG_GLOBAL_OK,
	                                        STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
	                                        NUM_ITEMS_DATE_TIME_ENTRY_SCREEN,
	                                        setting_date_time_list_of_icons,wgui_inline_items,0,guiBuffer);
	
	DisableCategory57ScreenDone();
	SetCategory57RightSoftkeyFunctions(PhnsetSetDT,GoBackHistory);

}


/*****************************************************************************
* FUNCTION
*  ExitPhnsetSetDT
* DESCRIPTION
*   This function is Exit function for Phonesetup-> Time and Date -> Set Time/Date
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void ExitPhnsetSetDT(void)
{
	history_t   Scr; 
	S16 nHistory =0;
	U16 inputBufferSize;                //added for inline edit history
	Scr.scrnID = SCR_ID_PHNSET_SET_DT;
	CloseCategory57Screen();
	Scr.entryFuncPtr = EntryPhnsetSetDT;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer,(S8*)&nHistory);
	GetCategory57History(Scr.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();     //added for inline edit history
	GetCategory57Data ((U8*) Scr.inputBuffer );         //added for inline edit history
	AddNHistory(Scr, (U16)inputBufferSize);                       //added for inline edit history

}





/*****************************************************************************
* FUNCTION
*  PhnsetSetDTFillInlineStruct
* DESCRIPTION
*   This function is to fill inline selection structure for Phonesetup-> Time and Date -> Set Time/Date
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/

void PhnsetSetDTFillInlineStruct( void *p )
{
	MYTIME currTime;
	U8 dummyString[5]; 
	wgui_inline_item *setting_inline_edit_items = (wgui_inline_item *) p;

	GetDateTime(&currTime);
//tangjie del begin 20060620
#if 1 // wangbei open 20060706
#if defined(__MMI_NITZ__)
	//If Change In Date Time Triggered From NITZ
	if(g_nitz_cntx.IsAccepted == TRUE)
	{
		memcpy(&currTime, &g_nitz_cntx.NITZTime, sizeof(MYTIME));
	}
#endif /* __MMI_NITZ__ */
#endif
//tangjie del end 20060620

	sprintf(gHour,"%02d",currTime.nHour);
	sprintf(gMinute,"%02d",currTime.nMin);
	sprintf(gYear,"%2d",currTime.nYear);
	sprintf(gMonth,"%02d",currTime.nMonth);
	sprintf(gDate,"%02d", currTime.nDay);
	
	strcpy((PS8)dummyString,gHour);
	AnsiiToUnicodeString((PS8)gHour,(PS8)dummyString);

	strcpy((PS8)dummyString,gMinute);
	AnsiiToUnicodeString((PS8)gMinute,(PS8)dummyString);

	strcpy((PS8)dummyString,gYear);
	AnsiiToUnicodeString((PS8)gYear,(PS8)dummyString);

	strcpy((PS8)dummyString,gMonth);
	AnsiiToUnicodeString((PS8)gMonth,(PS8)dummyString);

	strcpy((PS8)dummyString,gDate);
	AnsiiToUnicodeString((PS8)gDate,(PS8)dummyString);

	DSTString[0] = (PU8)GetString(STR_GLOBAL_OFF);
	DSTString[1] = (PU8)GetString(STR_GLOBAL_ON);
	//tangjie del begin 20060620
	#if 1 // wangbei open 20060706
	if(SetFromAT == FALSE)
	{
		S16 Err;
		
		SetInlineItemActivation(&(setting_inline_edit_items[1]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,KEY_EVENT_UP);
		SetInlineItemActivation(&(setting_inline_edit_items[3]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,KEY_EVENT_UP);
		 
		SetInlineItemActivation(&(setting_inline_edit_items[5]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,KEY_EVENT_UP);		

		SetInlineItemCaption(&(setting_inline_edit_items[0]),(U8*)GetString(STR_ENTER_TIME_FORMAT_DISP_CAPTION));
		SetInlineItemTime(&(setting_inline_edit_items[1]),(U8*)gHour,(U8*)gMinute, g_phnset_cntx_p->AM_PM_flag,PhnsetTimeEditorCallback);
		SetInlineItemFullWidth(&setting_inline_edit_items[1]);
		EnableInlineItemBoundary(&setting_inline_edit_items[1]);
		RightJustifyInlineItem(&setting_inline_edit_items[1]);
		SetInlineItemCaption(&(setting_inline_edit_items[2]),(U8*)GetString(STR_ENTER_DATE_FORMAT_DISP_CAPTION));
		SetInlineItemDate(&(setting_inline_edit_items[3]),(U8*)gDate,(U8*)gMonth,(U8*)gYear,PhnsetDateEditorCallback);
		SetInlineItemFullWidth(&setting_inline_edit_items[3]);
		EnableInlineItemBoundary(&setting_inline_edit_items[3]);
		RightJustifyInlineItem(&setting_inline_edit_items[3]);
		set_inline_date_boundary(2030, 2000, 12, 1);
		 
		ReadValue(NVRAM_SETTING_DT_DST, &(g_phnset_cntx_p->currentDST), DS_BYTE, &Err);
		SetInlineItemCaption(&(setting_inline_edit_items[4]),(U8*)GetString(STR_ID_PHNSET_SET_DST));
		SetInlineItemSelect(&(setting_inline_edit_items[5]),2,(U8**)DSTString, &g_phnset_cntx_p->currentDST);
		SetInlineItemFullWidth(&setting_inline_edit_items[5]);
		RegisterInlineSelectHighlightHandler(&setting_inline_edit_items[5],HighlightPhnsetSetDST);
		EnableInlineItemBoundary(&setting_inline_edit_items[5]);

		if(!IsScreenPresent(SCR_ID_PHNSET_SET_DT))
			g_phnset_cntx_p->prevDST = g_phnset_cntx_p->currentDST;
		
	}
	#endif
	//tangjie del end 20060620
}



/*****************************************************************************
* FUNCTION
*  PhnsetSetDT
* DESCRIPTION
*   This function is to set date and time for Phonesetup-> Time and Date -> Set Time/DAte
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetSetDT(void)
{
	if( (coolsand_UI_atoi((UI_string_type)gHour) > 23)  || (coolsand_UI_atoi((UI_string_type)gMinute) > 59) 
		|| (coolsand_UI_atoi((UI_string_type)gYear) > 2030) || (coolsand_UI_atoi((UI_string_type)gYear) < 2000)
		|| (coolsand_UI_atoi((UI_string_type)gMonth) > 12) || (coolsand_UI_atoi((UI_string_type)gMonth) < 1)
		|| (coolsand_UI_atoi((UI_string_type)gDate) > 31) || 
			(LastDayOfMonth((U8)coolsand_UI_atoi((UI_string_type)gMonth),(U8)coolsand_UI_atoi((UI_string_type)gYear)) < coolsand_UI_atoi((UI_string_type)gDate)) )
	{
		DisplayPopup((PU8)GetString(STR_SETTIME_UNFINISHED) ,IMG_GLOBAL_WARNING, 1,ST_NOTIFYDURATION,ERROR_TONE);
	}
	else
	{ 
		U8 flag;
		S16 error;

		ReadValue(NVRAM_SETTING_DT_DST, &flag, DS_BYTE, &error);
		if (flag != g_phnset_cntx_p->currentDST)
		{
			WriteValue(NVRAM_SETTING_DT_DST, &g_phnset_cntx_p->currentDST, DS_BYTE, &error);
		}

                PhnsetSendSetTimeReqMessage();
	}
}


/* 
** Set Format
*/



/*****************************************************************************
* FUNCTION
*  EntryPhnsetIdleScrDisp
* DESCRIPTION
*   This function is Entry function for Phonesetup-> Time and Date -> Set Format
*   Following functions: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void EntryPhnsetSetFormat(void)
{
	U8* guiBuffer;
	U8* inputBuffer;                       //added for inline edit history
	U16 inputBufferSize;                //added for inline edit history
	extern wgui_inline_item wgui_inline_items[];
 
	EntryNewScreen(SETTINGS_SET_TIME_FORMAT_SCREENID, ExitPhnsetSetFormat, NULL, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SETTINGS_SET_TIME_FORMAT_SCREENID);

	InitializeCategory57Screen();
	
	memset( wgui_inline_items, 0, sizeof(wgui_inline_item) * MAX_INLINE_ITEMS );
	PhnsetSetFormatFillInlineStruct( (void *) wgui_inline_items );
	
	inputBuffer = GetCurrNInputBuffer (SETTINGS_SET_TIME_FORMAT_SCREENID, &inputBufferSize );    //added for inline edit history
	
	if(inputBuffer!=NULL)                                            //added for inline edit history
	    SetCategory57Data(wgui_inline_items, NUM_ITEMS_DATE_TIME_FORMAT_ENTRY_SCREEN, inputBuffer);//sets the data 
 
	RegisterHighlightHandler(PhnsetSetFormatHiliteHandler);

	
	ShowCategory57Screen(STR_DATE_TIME_SET_FORMAT_CAPTION, IMG_SCR_SETTING_CAPTION,
						STR_GLOBAL_OK,IMG_GLOBAL_OK, STR_GLOBAL_BACK,STR_GLOBAL_BACK,
						NUM_ITEMS_DATE_TIME_FORMAT_ENTRY_SCREEN, setting_date_time_format_list_of_icons,
						wgui_inline_items, 0,guiBuffer);


	SetCategory57RightSoftkeyFunctions(PhnsetSetFormat,GoBackHistory);
	DisableCategory57ScreenDone();
}


/*****************************************************************************
* FUNCTION
*  ExitPhnsetSetFormat
* DESCRIPTION
*   This function is Exit function for Phonesetup->Time and Date -> Set Format
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void ExitPhnsetSetFormat(void)
{
	history_t   Scr; 
	S16 nHistory =0;
	U16 inputBufferSize;                //added for inline edit history
	Scr.scrnID = SETTINGS_SET_TIME_FORMAT_SCREENID;
	Scr.entryFuncPtr = EntryPhnsetSetFormat;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer,(S8*)&nHistory);
	GetCategory57History(Scr.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize( );     //added for inline edit history
	GetCategory57Data ((U8*) Scr.inputBuffer );         //added for inline edit history
	AddNHistory(Scr, (U16)inputBufferSize);                       //added for inline edit history
}



/*****************************************************************************
* FUNCTION
*  PhnsetSetFormatFillInlineStruct
* DESCRIPTION
*   This function is to fill inline selection structure for Phonesetup-> Time and Date -> Set Format
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetSetFormatFillInlineStruct( void *p)
{
	wgui_inline_item *setting_inline_format_items = (wgui_inline_item *)p;

	time_formats[0] = (PU8)GetString(STR_12);
	time_formats[1] = (PU8)GetString(STR_24);

#if defined(__PROJECT_GALLITE_C01__)
	date_formats[0] = (PU8)GetString(STR_MM_DD);
	date_formats[1] = (PU8)GetString(STR_DD_MM);
#else
	date_formats[0] = (PU8)GetString(STR_DD_MMM_YYYY_H);
	date_formats[1] = (PU8)GetString(STR_DD_MM_YYYY_S);
	date_formats[2] = (PU8)GetString(STR_MM_DD_YYYY_S);
	date_formats[3] = (PU8)GetString(STR_YYYY_MM_DD_S);
	date_formats[4] = (PU8)GetString(STR_YYYY_MM_DD_H);
	#if (!defined( __MMI_NOKIA_STYLE_N800__))&&(!defined(__MMI_MAINLCD_220X176__))
	date_formats[5] = (PU8)GetString(STR_MMM_DD_YYYY);
#endif
      #endif
	g_phnset_cntx_p->CurTimeFmt = PhnsetGetTimeFormat();
	g_phnset_cntx_p->CurDateFmt = PhnsetGetDateFormat();
	SetInlineItemActivation(&(setting_inline_format_items[1]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemActivation(&(setting_inline_format_items[3]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemCaption(&(setting_inline_format_items[0]),(U8*)GetString(STR_TIME_FORMAT_CAPTION));
	SetInlineItemSelect(&(setting_inline_format_items[1]),NUM_TIME_FORMATS,time_formats,(PS32)&(g_phnset_cntx_p->CurTimeFmt));
	SetInlineItemCaption(&(setting_inline_format_items[2]),(U8*)GetString(STR_DATE_FORMAT_CAPTION));
	SetInlineItemSelect(&(setting_inline_format_items[3]),NUM_DATE_FORMATS,date_formats,(PS32)&(g_phnset_cntx_p->CurDateFmt) );
}




/*****************************************************************************
* FUNCTION
*  PhnsetSetFormat
* DESCRIPTION
*   This function is to set date/time format for PhoneSetup->Time and Date->Set Format
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetSetFormat(void)
{
	//chgcnt = 0;
	//oneoccured = 0;
	if(((U8)g_phnset_cntx_p->CurTimeFmt != PhnsetGetTimeFormat()) ||
		((U8)(g_phnset_cntx_p->CurDateFmt) != PhnsetGetDateFormat())) 
	{
		//Save New Time Format To NVRAM if Value is Changed.
		PhnsetSetTimeFormat((U8)g_phnset_cntx_p->CurTimeFmt);
		//Save New Date Format To NVRAM if Value is Changed.
		PhnsetSetDateFormat((U8)g_phnset_cntx_p->CurDateFmt);
	}
	SUBLCD_RedrawCategoryFunction();
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,SUCCESS_TONE);
	DeleteUptoScrID(SCR9041_TIMEANDDATE);
}



/*****************************************************************************
* FUNCTION
*  PhnsetSetFormatHiliteHandler
* DESCRIPTION
*   This function is Highlight handler for Phonesetup->Time and Date->Set Format
*   Follow functions: PhnsetSetFormat
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
//void hilite_handler_set_format(S32 highliteIndex)
void PhnsetSetFormatHiliteHandler(S32 highliteIndex)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction(PhnsetSetFormat);
//	highlightedDateFormatItem=(U8)highliteIndex;
}

//#else // __MMI_SEPARATE_DT_SETTING__
#define NUMBER_OF_ITEMS_IN_DATEANDFORMAT 4
/* 1. title of date format
 * 2. format of date
 * 3. title of date
 * 4. value of date
 */
#define NUMBER_OF_ITEMS_IN_TIMEANDFORMAT 6
/* 1. title of time format
 * 2. format of time
 * 3. title of time
 * 4. value of time
 * 5. title of DST
 * 6. value of DST
 */

/*****************************************************************************
* FUNCTION
*  HighlightPhnsetSetTimeAndFormat
* DESCRIPTION
*   This function is Highlight handler for Phonesetup-> Time And Date -> Set Format
*   Follow functions: EntryPhnsetSetFormat
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightPhnsetSetTimeAndFormat(void)
{
	//datetimeindex = 2;
     g_phnset_cntx_p->curHightlightItem = PHNSET_DT_MNU_TIMEANDFORMAT;
	
//	highlightedDateFormatItem = 0;
	SetLeftSoftkeyFunction(EntryPhnsetSetTimeAndFormat,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetSetTimeAndFormat, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);   
}

/*****************************************************************************
* FUNCTION
*  HighlightPhnsetSetDateAndFormat
* DESCRIPTION
*   This function is Highlight handler for Phonesetup-> Time And Date -> Set Time/Date
*   Follow functions: EntryPhnsetSetDT
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void HighlightPhnsetSetDateAndFormat(void)
{
	//datetimeindex = 1;
	g_phnset_cntx_p->curHightlightItem = PHNSET_DT_MNU_DATEANDFORMAT;
	
	//highlightedDateSetItem =0;
	//g_phnset_cntx_p->curHightlightItem = 0;
	SetLeftSoftkeyFunction(EntryPhnsetSetDateAndFormat,KEY_EVENT_UP);
	SetKeyHandler(EntryPhnsetSetDateAndFormat, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);   
}
/*****************************************************************************
* FUNCTION
*  PhnsetInlineSetDTHiliteHdlr
* DESCRIPTION
*   This function is Inline line editor Highlight handler for Phonesetup-> Time and Date -> Set Time/Date
*   Follow functions: PhnsetSetDT
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
static void PhnsetInlineSetDTAndFormatHiliteHdlr(S32 currId)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	SetCategory57LeftSoftkeyFunction(PhnsetSetDTAndFormat);	
	
}

/**************************************************************

	FUNCTION NAME		: EntryPhnsetSetDateAndFormat(void)

  	PURPOSE				: Set Date And Time Entry Function

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryPhnsetSetDateAndFormat(void)
{
	U8* guiBuffer;
	U8* inputBuffer;                       //added for inline edit history
	U16 inputBufferSize;                //added for inline edit history
	extern wgui_inline_item wgui_inline_items[];
	U16 setting_date_time_list_of_icons_t[NUMBER_OF_ITEMS_IN_DATEANDFORMAT] = 
	{
		IMG_CAL, 
		IMG_TIME_AND_DATE_BLANK, 
		IMG_CAL, 
		IMG_TIME_AND_DATE_BLANK, 
	};

	EntryNewScreen(SCR_ID_PHNSET_SET_DATEANDFORMAT, ExitPhnsetSetDateAndFormat, NULL, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_PHNSET_SET_DATEANDFORMAT);
	InitializeCategory57Screen();
	
	PhnsetSetDTAndFormatFillInlineStruct( (void *)wgui_inline_items);
	
	inputBuffer = GetCurrNInputBuffer (SCR_ID_PHNSET_SET_DATEANDFORMAT, &inputBufferSize );    //added for inline edit history

	if(inputBuffer != NULL)
		SetCategory57Data(wgui_inline_items, NUMBER_OF_ITEMS_IN_DATEANDFORMAT, inputBuffer);

	SetLeftSoftkeyFunction(PhnsetSetDTAndFormat,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	RegisterHighlightHandler(PhnsetInlineSetDTAndFormatHiliteHdlr);


	ShowCategory57Screen(STR_ID_PHNSET_SET_DATEANDFORMAT,IMG_SCR_SETTING_CAPTION,
	                                        STR_GLOBAL_OK,IMG_GLOBAL_OK,
	                                        STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
	                                        NUMBER_OF_ITEMS_IN_DATEANDFORMAT,
	                                        setting_date_time_list_of_icons_t,wgui_inline_items,0,guiBuffer);
	
	DisableCategory57ScreenDone();
	SetCategory57RightSoftkeyFunctions(PhnsetSetDTAndFormat,GoBackHistory);

}


/*****************************************************************************
* FUNCTION
*  ExitPhnsetSetDateAndFormat
* DESCRIPTION
*   This function is Exit function for Phonesetup-> Time and Date -> Set Time/Date
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void ExitPhnsetSetDateAndFormat(void)
{
	history_t   Scr; 
	S16 nHistory =0;
	U16 inputBufferSize;                //added for inline edit history
	Scr.scrnID = SCR_ID_PHNSET_SET_DATEANDFORMAT;
	CloseCategory57Screen();
	Scr.entryFuncPtr = EntryPhnsetSetDateAndFormat;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer,(S8*)&nHistory);
	GetCategory57History(Scr.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();     //added for inline edit history
	GetCategory57Data ((U8*) Scr.inputBuffer );         //added for inline edit history
	AddNHistory(Scr, (U16)inputBufferSize);                       //added for inline edit history

}
/**************************************************************

	FUNCTION NAME		: EntryPhnsetSetTimeAndFormat(void)

  	PURPOSE				: Set Date And Time Entry Function

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryPhnsetSetTimeAndFormat(void)
{
	U8* guiBuffer;
	U8* inputBuffer;                       //added for inline edit history
	U16 inputBufferSize;                //added for inline edit history
	extern wgui_inline_item wgui_inline_items[];
	U16 setting_date_time_list_of_icons_t[NUM_ITEMS_DATE_TIME_ENTRY_SCREEN] = 
	{
		IMG_TIME, 
		IMG_TIME_AND_DATE_BLANK, 
		IMG_TIME, 
		IMG_TIME_AND_DATE_BLANK, 
		SUN_ICON, 
		IMG_TIME_AND_DATE_BLANK 
	};
	
	EntryNewScreen(SCR_ID_PHNSET_SET_TIMEANDFORMAT, ExitPhnsetSetTimeAndFormat, NULL, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_PHNSET_SET_TIMEANDFORMAT);
	InitializeCategory57Screen();
	
	PhnsetSetDTAndFormatFillInlineStruct( (void *)wgui_inline_items);
	
	inputBuffer = GetCurrNInputBuffer (SCR_ID_PHNSET_SET_TIMEANDFORMAT, &inputBufferSize );    //added for inline edit history

	if(inputBuffer != NULL)
		SetCategory57Data(wgui_inline_items, NUMBER_OF_ITEMS_IN_TIMEANDFORMAT, inputBuffer);

	SetLeftSoftkeyFunction(PhnsetSetDTAndFormat,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	RegisterHighlightHandler(PhnsetInlineSetDTAndFormatHiliteHdlr);


	ShowCategory57Screen(STR_ID_PHNSET_SET_TIMEANDFORMAT,IMG_SCR_SETTING_CAPTION,
	                                        STR_GLOBAL_OK,IMG_GLOBAL_OK,
	                                        STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
	                                        NUMBER_OF_ITEMS_IN_TIMEANDFORMAT,
	                                        setting_date_time_list_of_icons_t,wgui_inline_items,0,guiBuffer);
	
	DisableCategory57ScreenDone();
	SetCategory57RightSoftkeyFunctions(PhnsetSetDTAndFormat,GoBackHistory);

}


/*****************************************************************************
* FUNCTION
*  ExitPhnsetSetDateAndFormat
* DESCRIPTION
*   This function is Exit function for Phonesetup-> Time and Date -> Set Time/Date
*   Functionality: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void ExitPhnsetSetTimeAndFormat(void)
{
	history_t   Scr; 
	S16 nHistory =0;
	U16 inputBufferSize;                //added for inline edit history
	Scr.scrnID = SCR_ID_PHNSET_SET_TIMEANDFORMAT;
	CloseCategory57Screen();
	Scr.entryFuncPtr = EntryPhnsetSetTimeAndFormat;
	pfnUnicodeStrcpy((S8*)Scr.inputBuffer,(S8*)&nHistory);
	GetCategory57History(Scr.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();     //added for inline edit history
	GetCategory57Data ((U8*) Scr.inputBuffer );         //added for inline edit history
	AddNHistory(Scr, (U16)inputBufferSize);                       //added for inline edit history

}
/*****************************************************************************
* FUNCTION
*  PhnsetSetDTFillInlineStruct
* DESCRIPTION
*   This function is to fill inline selection structure for Phonesetup-> Time and Date -> Set Time/Date
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/

void PhnsetSetDTAndFormatFillInlineStruct( void *p )
{
	MYTIME currTime;
	U8 dummyString[5]; 
	wgui_inline_item *setting_inline_edit_items = (wgui_inline_item *) p;

	GetDateTime(&currTime);
//tangjie del begin 20060620
#if 1  // wangbei open 20060706
#if defined(__MMI_NITZ__)
	//If Change In Date Time Triggered From NITZ
	if(g_nitz_cntx.IsAccepted == TRUE)
	{
		memcpy(&currTime, &g_nitz_cntx.NITZTime, sizeof(MYTIME));
	}
#endif /* __MMI_NITZ__ */
#endif
//tangjie del end 20060620

	if(g_phnset_cntx_p->curHightlightItem == PHNSET_DT_MNU_DATEANDFORMAT)
	{
#if defined(__PROJECT_GALLITE_C01__)
		date_formats[0] = (PU8)GetString(STR_MM_DD);
		date_formats[1] = (PU8)GetString(STR_DD_MM);
#else
		date_formats[0] = (PU8)GetString(STR_DD_MMM_YYYY_H);
		date_formats[1] = (PU8)GetString(STR_DD_MM_YYYY_S);
		date_formats[2] = (PU8)GetString(STR_MM_DD_YYYY_S);
		date_formats[3] = (PU8)GetString(STR_YYYY_MM_DD_S);
		date_formats[4] = (PU8)GetString(STR_YYYY_MM_DD_H);
	#if !defined( __MMI_NOKIA_STYLE_N800__)||!defined(__MMI_MAINLCD_220X176__)

		date_formats[5] = (PU8)GetString(STR_MMM_DD_YYYY);
#endif
#endif
		g_phnset_cntx_p->CurDateFmt = PhnsetGetDateFormat();

		sprintf(gYear,"%2d",currTime.nYear);
		sprintf(gMonth,"%02d",currTime.nMonth);
		sprintf(gDate,"%02d", currTime.nDay);

		strcpy((PS8)dummyString,gYear);
		AnsiiToUnicodeString((PS8)gYear,(PS8)dummyString);

		strcpy((PS8)dummyString,gMonth);
		AnsiiToUnicodeString((PS8)gMonth,(PS8)dummyString);

		strcpy((PS8)dummyString,gDate);
		AnsiiToUnicodeString((PS8)gDate,(PS8)dummyString);
		//tangjie del begin 20060620
		#if 1// wangbei open 20060706
		if(SetFromAT == FALSE)
		{
			
			SetInlineItemActivation(&(setting_inline_edit_items[1]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,KEY_EVENT_UP);
			SetInlineItemActivation(&(setting_inline_edit_items[3]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,KEY_EVENT_UP);

			SetInlineItemCaption(&(setting_inline_edit_items[0]),(U8*)GetString(STR_DATE_FORMAT_CAPTION));
			SetInlineItemSelect(&(setting_inline_edit_items[1]),NUM_DATE_FORMATS,date_formats,(PS32)&(g_phnset_cntx_p->CurDateFmt) );
			
			SetInlineItemCaption(&(setting_inline_edit_items[2]),(U8*)GetString(STR_ENTER_DATE_FORMAT_DISP_CAPTION));
			SetInlineItemDate(&(setting_inline_edit_items[3]),(U8*)gDate,(U8*)gMonth,(U8*)gYear,PhnsetDateEditorCallback);
			SetInlineItemFullWidth(&setting_inline_edit_items[3]);
			EnableInlineItemBoundary(&setting_inline_edit_items[3]);
			RightJustifyInlineItem(&setting_inline_edit_items[3]);
			set_inline_date_boundary(2030, 2000, 12, 1);
			
		}
		#endif
		//tangjie del end 20060620

	}
	else if(g_phnset_cntx_p->curHightlightItem == PHNSET_DT_MNU_TIMEANDFORMAT)
	{
		time_formats[0] = (PU8)GetString(STR_12);
		time_formats[1] = (PU8)GetString(STR_24);

		g_phnset_cntx_p->CurTimeFmt = PhnsetGetTimeFormat();

		sprintf(gHour,"%02d",currTime.nHour);
		sprintf(gMinute,"%02d",currTime.nMin);

		strcpy((PS8)dummyString,gHour);
		AnsiiToUnicodeString((PS8)gHour,(PS8)dummyString);

		strcpy((PS8)dummyString,gMinute);
		AnsiiToUnicodeString((PS8)gMinute,(PS8)dummyString);


		DSTString[0] = (PU8)GetString(STR_GLOBAL_OFF);
		DSTString[1] = (PU8)GetString(STR_GLOBAL_ON);
		//tangjie del begin 20060620
		#if 1 // wangbei open 20060706
		if(SetFromAT == FALSE)
		{
			S16 Err;
			
			SetInlineItemActivation(&(setting_inline_edit_items[1]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,KEY_EVENT_UP);
			SetInlineItemActivation(&(setting_inline_edit_items[3]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,KEY_EVENT_UP);
			 
			SetInlineItemActivation(&(setting_inline_edit_items[5]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,KEY_EVENT_UP);		

			SetInlineItemCaption(&(setting_inline_edit_items[0]),(U8*)GetString(STR_TIME_FORMAT_CAPTION));
			SetInlineItemSelect(&(setting_inline_edit_items[1]),NUM_TIME_FORMATS,time_formats,(PS32)&(g_phnset_cntx_p->CurTimeFmt));

			SetInlineItemCaption(&(setting_inline_edit_items[2]),(U8*)GetString(STR_ENTER_TIME_FORMAT_DISP_CAPTION));
			SetInlineItemTime(&(setting_inline_edit_items[3]),(U8*)gHour,(U8*)gMinute, g_phnset_cntx_p->AM_PM_flag,PhnsetTimeEditorCallback);
			SetInlineItemFullWidth(&setting_inline_edit_items[3]);
			EnableInlineItemBoundary(&setting_inline_edit_items[3]);
			RightJustifyInlineItem(&setting_inline_edit_items[3]);
			 
			ReadValue(NVRAM_SETTING_DT_DST, &(g_phnset_cntx_p->currentDST), DS_BYTE, &Err);
			SetInlineItemCaption(&(setting_inline_edit_items[4]),(U8*)GetString(STR_ID_PHNSET_SET_DST));
			SetInlineItemSelect(&(setting_inline_edit_items[5]),2,(U8**)DSTString, &g_phnset_cntx_p->currentDST);
			SetInlineItemFullWidth(&setting_inline_edit_items[5]);
			RegisterInlineSelectHighlightHandler(&setting_inline_edit_items[5],HighlightPhnsetSetDST);
			EnableInlineItemBoundary(&setting_inline_edit_items[5]);

			if(!IsScreenPresent(SCR_ID_PHNSET_SET_DT))			
				g_phnset_cntx_p->prevDST = g_phnset_cntx_p->currentDST;
			
		}
		#endif
              //tangjie del end 20060620
              
	}
	
 }

/*****************************************************************************
* FUNCTION
*  PhnsetSetDTAndFormat
* DESCRIPTION
*   This function is to set date/time format for PhoneSetup->Time and Date->Set Format
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetSetDTAndFormat(void)
{
if(
		(
			(g_phnset_cntx_p->curHightlightItem == PHNSET_DT_MNU_TIMEANDFORMAT)&&
			((coolsand_UI_atoi((UI_string_type)gHour) > 23)  || (coolsand_UI_atoi((UI_string_type)gMinute) > 59))
		)
|| 	(
			(g_phnset_cntx_p->curHightlightItem == PHNSET_DT_MNU_DATEANDFORMAT)&&
		   (	(coolsand_UI_atoi((UI_string_type)gYear) > 2030) || (coolsand_UI_atoi((UI_string_type)gYear) < 2000)
				|| (coolsand_UI_atoi((UI_string_type)gMonth) > 12) || (coolsand_UI_atoi((UI_string_type)gMonth) < 1)
				|| (coolsand_UI_atoi((UI_string_type)gDate) > 31)
				|| (LastDayOfMonth((U8)coolsand_UI_atoi((UI_string_type)gMonth),(U8)coolsand_UI_atoi((UI_string_type)gYear)) < coolsand_UI_atoi((UI_string_type)gDate)) 
			)
		)	
	)
	{
		DisplayPopup((PU8)GetString(STR_SETTIME_UNFINISHED) ,IMG_GLOBAL_WARNING, 1,ST_NOTIFYDURATION,ERROR_TONE);
	}
	else
	{ 
		U8 flag;
		S16 error;

		ReadValue(NVRAM_SETTING_DT_DST, &flag, DS_BYTE, &error);
		if (flag != g_phnset_cntx_p->currentDST)
		{
			WriteValue(NVRAM_SETTING_DT_DST, &g_phnset_cntx_p->currentDST, DS_BYTE, &error);
		}

                PhnsetSendSetTimeReqMessage();
	}


	if(((U8)g_phnset_cntx_p->CurTimeFmt != PhnsetGetTimeFormat()) ||
		((U8)(g_phnset_cntx_p->CurDateFmt) != PhnsetGetDateFormat())) 
	{
		//Save New Time Format To NVRAM if Value is Changed.
		PhnsetSetTimeFormat((U8)g_phnset_cntx_p->CurTimeFmt);
		//Save New Date Format To NVRAM if Value is Changed.
		PhnsetSetDateFormat((U8)g_phnset_cntx_p->CurDateFmt);
	}
	SUBLCD_RedrawCategoryFunction();
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,SUCCESS_TONE);
	DeleteUptoScrID(SCR9041_TIMEANDDATE);
}



/* 
** Home City
*/


/*****************************************************************************
* FUNCTION
*  PhnsetInitTimeNDate
* DESCRIPTION
*   This function is to 
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetInitTimeNDate(void)
{
/*	
	int i =0;
	for(i =0;i< NUM_CITYS;i++)
		city_list[i]=STR_WCLOCK_CITY1 +i;

	for(i =0;i< NUM_CITYS;i++)
		city_number_icon[i] = IMG_TIME_AND_DATE_BLANK;
*/
	PhnsetConstructCityList();
     g_phnset_cntx_p->CurHomeCityStrPtr = subMenuDataPtrs[g_phnset_cntx_p->currentCity];
	
}


 
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif



/*****************************************************************************
* FUNCTION
*  PhnsetReadNvmHomeCityIdxNStoreCityName
* DESCRIPTION
*   This function is to (1) Read home city index from nvram
*                             (2) Store city name (digits) in global variable
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  localHomeCityPtr
*  currentCity
*****************************************************************************/
void PhnsetReadNvmHomeCityIdxNStoreCityName(void)  /* Used on Language change */
{
	S16 error;
	ReadValue(CURRENT_CITY,&(g_phnset_cntx_p->currentCity) ,DS_BYTE,&error);
	if(error != NVRAM_READ_SUCCESS)
		g_phnset_cntx_p->currentCity  = 0;
	else if (g_phnset_cntx_p->currentCity > NUM_CITYS)
		g_phnset_cntx_p->currentCity  = 0;
	g_phnset_cntx_p->CurHomeCityStrID  = TimeZoneData[g_phnset_cntx_p->currentCity+1].CityName;
}


/*****************************************************************************
* FUNCTION
*  PhnsetReinitHomeCity
* DESCRIPTION
*   This function is to Restore the city index when language change
*   Functionality: !!!NOTICE!!! the function should use together with PhnsetReadNvmHomeCityIdxNStoreCityName()
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  localHomeCityPtr
*  currentCity
*  HomeCityStrPtr
*****************************************************************************/
void PhnsetReinitHomeCity(void) /* Used on Language change, together with PhnsetReadNvmHomeCityIdxNStoreCityName()  */
{
	S16 error;
	U8 i;
	U8 num_city = NUMBER_OF_CITY;
	PhnsetConstructCityList();
	for( i = 0; i < num_city; i++ )
	{
		if( TimeZoneData[i].CityName == g_phnset_cntx_p->CurHomeCityStrID )
		{
			g_phnset_cntx_p->currentCity = i - 1;
			WriteValue(CURRENT_CITY,&(g_phnset_cntx_p->currentCity),DS_BYTE,&error);
			break;
		}
      }
	g_phnset_cntx_p->CurHomeCityStrPtr = subMenuDataPtrs[g_phnset_cntx_p->currentCity];

}



/*****************************************************************************
* FUNCTION
*  PhnsetInitSetTimeNDateNvram
* DESCRIPTION
*   This function is to read global variable from nvram 
*                             for Phonesetup -> Time and Date ->Set Home City
*                                                                               ->Set Time/Date
*                                                                               ->Set Format
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetInitSetTimeNDateNvram(void)
{
       
      //GetDateTimeSetupNvRam();
	PhnsetReadNvmHomeCityIdxNStoreCityName();
	g_phnset_cntx_p->CurTimeFmt = PhnsetGetTimeFormat();
	g_phnset_cntx_p->CurDateFmt = PhnsetGetDateFormat();
}



/*****************************************************************************
* FUNCTION
*  PhnsetConstructCityList
* DESCRIPTION
*   This function is to 
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetConstructCityList(void)
{
       int i, maxCities;
       maxCities = ((NUM_CITYS >  MAX_SUB_MENUS) ?  MAX_SUB_MENUS :  NUM_CITYS );
       for (i=0; i<maxCities; i++)
	      subMenuDataPtrs[i]=(PU8)((GetString)((U16)(TimeZoneData[i+1].CityName)));
}



/*****************************************************************************
* FUNCTION
*  EntryPhnsetSetHomeCity
* DESCRIPTION
*   This function is Entry function for Phonesetup->Time and Date->Set Homecity
*   Following functions: PhnsetSetHomeCity
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
#ifndef __MMI_HIDE_HOMECITY__
void EntryPhnsetSetHomeCity(void)
{
	U8* guiBuffer;
	U8 maxCities=0;

	EntryNewScreen(SETTINGS_SETHOMECITY_SCREENID, NULL, EntryPhnsetSetHomeCity, NULL);

	guiBuffer = GetCurrGuiBuffer(SETTINGS_SETHOMECITY_SCREENID);
	
	PhnsetReadNvmHomeCityIdxNStoreCityName();
	PhnsetConstructCityList();
	
	g_phnset_cntx_p->curHightlightItem = (U32) g_phnset_cntx_p->currentCity;

   maxCities = ((NUM_CITYS >  MAX_SUB_MENUS) ?  MAX_SUB_MENUS :  NUM_CITYS ); 
	ShowCategory6Screen(STR_MENU9142_SETHOMECITY,IMG_SCR_SETTING_CAPTION,
	                                      STR_GLOBAL_OK, IMG_GLOBAL_OK,
	                                      STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
	                                      NUM_CITYS,subMenuDataPtrs,NULL,g_phnset_cntx_p->curHightlightItem,guiBuffer);
	
	RegisterHighlightHandler (PhnsetHomeCityIdxHiliteHandler);

	if( !IsChineseSet() )
	{
		set_numberless_multitap();
		register_multitap_no_draw_key_handlers();
	   set_multitap_functions(PhnsetHCMultiTapHdlr,PhnsetHCMultiTapCompleteHdlr);
	   change_multitap_mode(0);
	}

	SetLeftSoftkeyFunction(PhnsetSetHomeCity,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	
	SetKeyHandler(NULL ,KEY_RIGHT_ARROW,KEY_EVENT_DOWN); 
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}
#endif


/*****************************************************************************
* FUNCTION
*  PhnsetSetHomeCity
* DESCRIPTION
*   This function is to set home city for Phonesetup->Time and Date->Set Homecity
*   Following functions: 
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
#ifndef __MMI_HIDE_HOMECITY__
void PhnsetSetHomeCity(void)
{
	//tangjie del begin 20060620
	
	S16 error;
	U8 tmpCity;
	g_phnset_cntx_p->currentCity = (U8)g_phnset_cntx_p->curHightlightItem;
	
	if( !IsChineseSet() )
	{
	   disable_active_multitap();
	}

	ReadValue(CURRENT_CITY,&tmpCity, DS_BYTE,&error);

	if(tmpCity != (U8) g_phnset_cntx_p->curHightlightItem )
	{
	    g_phnset_cntx_p->CurHomeCityStrPtr = subMenuDataPtrs[g_phnset_cntx_p->currentCity];
	    WriteValue(CURRENT_CITY,&(g_phnset_cntx_p->currentCity), DS_BYTE,&error);
	}

	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,SUCCESS_TONE);

    //NITZ Indication Change
	//tmpCity = (U8)(GetTimeZone(PhnsetGetHomeCity())*4);           //tangjie del 20060620
	WriteValue( NVRAM_NITZTIMEZONE, &tmpCity, DS_BYTE, &error);
#if 1
	DeleteNHistory(1);

      
	//ShowIdleAppSubLcd();
	SUBLCD_RedrawCategoryFunction();
     #endif
     //tangjie del end 20060620
}
#endif


/*****************************************************************************
* FUNCTION
*  PhnsetHomeCityIdxHiliteHandler
* DESCRIPTION
*   This function is Highlight handler for Phonesetup->Time and Date->Set Homecity -> (list)
*   Follow functions: EntryPhnsetMainMenuStyle
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetHomeCityIdxHiliteHandler(S32 cityIndex)
{
	g_phnset_cntx_p->curHightlightItem= (U32)cityIndex;
}




/*****************************************************************************
* FUNCTION
*  PhnsetGetHomeCity
* DESCRIPTION
*   This function is to get current home city setting
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
U8 PhnsetGetHomeCity(void)
{
	return g_phnset_cntx_p->currentCity;
}




/*****************************************************************************
* FUNCTION
*  PhnsetHCGetNearbyIdx
* DESCRIPTION
*   This function is to get the nearby city index when multitap presing at browsing home city in english mode
*                       for Phonesetup->Time and Date->Home City
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
U8 PhnsetHCGetNearbyIdx(UI_character_type inp)
{
	U16 n1=0;
	U8 i, maxCities=0;

	if( (inp >= (UI_character_type)'A' && inp <= (UI_character_type)'Z') ||(inp >= (UI_character_type)'a' && inp <= (UI_character_type)'z') ) 
	{
        maxCities = ((NUM_CITYS >  MAX_SUB_MENUS) ?  MAX_SUB_MENUS :  NUM_CITYS ); //haha
		for (i=0; i<maxCities;  i++)
		{
			memcpy(&n1,subMenuDataPtrs[i],ENCODING_LENGTH);
			if(n1 >= inp )
				return (U8)(i);
		}

		if( i == maxCities )
		  return i-1;
		else
	      return 0;	
	}
	return 0;
}



/*****************************************************************************
* FUNCTION
*  PhnsetHCMultiTapHdlr
* DESCRIPTION
*   This function is multitap handler when browsing home city in english mode
*                       for Phonesetup->Time and Date->Home City
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetHCMultiTapHdlr(UI_character_type inp)
{
	g_phnset_cntx_p->curHightlightItem = (U32)PhnsetHCGetNearbyIdx(inp);
	fixed_list_goto_item((S32)g_phnset_cntx_p->curHightlightItem);
}




/*****************************************************************************
* FUNCTION
*  PhnsetHCMultiTapCompleteHdlr
* DESCRIPTION
*   This function is to 
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetHCMultiTapCompleteHdlr(void)
{
	fixed_list_goto_item((S32)g_phnset_cntx_p->curHightlightItem);
}


/*****************************************************************************
* FUNCTION
*  PhnsetSetDTFromAT
* DESCRIPTION
*   This function is to set date and time from AT command
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetSetDTFromAT(MYTIME mytime)
{
#ifdef MMI_ON_HARDWARE_P
	MYQUEUE msgStruct;
#endif
	RTCTIMEFORMAT *time;
	U8 dummyString[5*ENCODING_LENGTH]; 

	if(mytime.nYear < 1970 || mytime.nYear >2030
		|| mytime.nMonth > 12 ||
		mytime.nDay > LastDayOfMonth(mytime.nMonth,mytime.nYear)
		|| mytime.nHour > 23 || mytime.nMin > 59)
		return;

      sprintf(gHour,"%d",mytime.nHour);
      sprintf(gMinute,"%d",mytime.nMin);
      sprintf(gYear,"%d",mytime.nYear);
      sprintf(gMonth,"%d",mytime.nMonth);
      sprintf(gDate,"%d",mytime.nDay);

      AnsiiToUnicodeString((PS8)dummyString,(PS8)gHour);
      pfnUnicodeStrcpy((PS8)gHour,(PS8) dummyString);
		
      AnsiiToUnicodeString((PS8)dummyString,(PS8)gMinute);
      pfnUnicodeStrcpy((PS8)gMinute,(PS8) dummyString);
			
      AnsiiToUnicodeString((PS8)dummyString,(PS8)gYear);
      pfnUnicodeStrcpy((PS8)gYear,(PS8) dummyString);
			
      AnsiiToUnicodeString((PS8)dummyString,(PS8)gMonth);
      pfnUnicodeStrcpy((PS8)gMonth,(PS8) dummyString);
			
      AnsiiToUnicodeString((PS8)dummyString,(PS8)gDate);
      pfnUnicodeStrcpy((PS8)gDate,(PS8) dummyString);

      time = OslConstructDataPtr(sizeof (RTCTIMEFORMAT));
      time->info.alarm_format = 0;
      time->info.alarm_index = 0;
      time->info.type = 0;
      time->info.text[0] = '\0';
      time->info.recurr = 0;
      time->info.data_time.rtc_sec = mytime.nSec;
      time->info.data_time.rtc_min = mytime.nMin;
      time->info.data_time.rtc_hour = mytime.nHour;
   		
#ifdef MMI_ON_HARDWARE_P
      time->rtc_type = RTC_TIME_CLOCK_IND;
      time->set_type = RTC_SETTING_TYPE_DATETIME;
      time->info.data_time.rtc_day = mytime.nDay;//gclockData.rtc_hour;
      time->info.data_time.rtc_mon = mytime.nMonth;//gclockData.rtc_mon;
      time->info.data_time.rtc_wday = mytime.DayIndex;//gclockData.rtc_wday;
      time->info.data_time.rtc_year =  mytime.nYear - YEARFORMATE;//gclockData.rtc_year;

      msgStruct.oslSrcId = MOD_MMI;
      msgStruct.oslDestId = MOD_L4C;
      msgStruct.oslMsgId = HW_SET_TIME_REQ;
      msgStruct.oslSapId = 0;
      msgStruct.oslDataPtr = (oslParaType *)time;
      msgStruct.oslPeerBuffPtr= NULL;
      OslMsgSendExtQueue(&msgStruct);
		
      PhnsetSendSetTimeReqMessage();

#else
      time->rtc_type = 0;
      time->set_type = 2;
      time->info.data_time.rtc_day = mytime.nDay;
      time->info.data_time.rtc_mon = mytime.nMonth;
      time->info.data_time.rtc_wday = 0;
      time->info.data_time.rtc_year = (U8)(mytime.nYear -1900);
      {
	      RESULTSTRUCT resp;
      	resp.result = 1;
	      PhnsetSetTimeResHdlr(&resp);
      }
#endif

		
}





/*****************************************************************************
* FUNCTION
*  PhnsetSendSetTimeReqMessage
* DESCRIPTION
 
*                                      for Phonesetup->Time and Date->Set Time/Date
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetSendSetTimeReqMessage(void)
{
#ifdef MMI_ON_HARDWARE_P
	MYQUEUE msgStruct;
#endif
	RTCTIMEFORMAT *time;

	g_phnset_cntx_p->settime.hour        =(U8) coolsand_UI_atoi((UI_string_type)gHour);
	g_phnset_cntx_p->settime.minutes	=(U8)coolsand_UI_atoi((UI_string_type)gMinute);
	g_phnset_cntx_p->settime.month     =(U8)coolsand_UI_atoi((UI_string_type)gMonth);
	g_phnset_cntx_p->settime.date		=(U8)coolsand_UI_atoi((UI_string_type)gDate);
	g_phnset_cntx_p->settime.year		=(U16)coolsand_UI_atoi((UI_string_type)gYear);
	//tangjie del begin 20060620
	#if 0
	if(SetFromAT == FALSE) 
		SetProtocolEventHandler(PhnsetSetTimeResHdlr, HW_SET_TIME_REQ_SUCCESS);
	else
	{
		SetProtocolEventHandler(PhnsetSetTimeResHdlrForAT, HW_SET_TIME_REQ_SUCCESS);
		SetFromAT = FALSE;
		return;
	}
	#endif
	//tangjie del end 20060620
	SetProtocolEventHandler(PhnsetSetTimeResHdlr, HW_SET_TIME_REQ_SUCCESS);   //tangjie add 20060620
	time = OslConstructDataPtr(sizeof (RTCTIMEFORMAT));
	time->info.alarm_format = 0;
	time->info.alarm_index = 0;
	time->info.type = 0;
	time->info.text[0] = '\0';
	time->info.recurr = 0;
	time->info.data_time.rtc_sec = 0;
	time->info.data_time.rtc_min = g_phnset_cntx_p->settime.minutes;
	time->info.data_time.rtc_hour = g_phnset_cntx_p->settime.hour;

#ifdef MMI_ON_HARDWARE_P
	time->rtc_type = RTC_TIME_CLOCK_IND;
	time->set_type = RTC_SETTING_TYPE_DATETIME;
	time->info.data_time.rtc_day = g_phnset_cntx_p->settime.date;
	time->info.data_time.rtc_mon = g_phnset_cntx_p->settime.month;
	time->info.data_time.rtc_wday = gclockData.rtc_wday;
	time->info.data_time.rtc_year =  g_phnset_cntx_p->settime.year - YEARFORMATE;

	msgStruct.oslSrcId = MOD_MMI;
	msgStruct.oslDestId = MOD_L4C;
	msgStruct.oslMsgId = HW_SET_TIME_REQ;
	msgStruct.oslSapId = 0;
	msgStruct.oslDataPtr = (oslParaType *)time;
	msgStruct.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&msgStruct);

#else
	time->rtc_type = 0;
	time->set_type = 2;
	time->info.data_time.rtc_day = g_phnset_cntx_p->settime.date;
	time->info.data_time.rtc_mon = g_phnset_cntx_p->settime.month;
	time->info.data_time.rtc_wday = 0;
	time->info.data_time.rtc_year = (U8)(g_phnset_cntx_p->settime.year -1900);
	{
		RESULTSTRUCT resp;
		resp.result = 1;
		PhnsetSetTimeResHdlr(&resp);
	}
#endif

}




/*****************************************************************************
* FUNCTION
*  PhnsetSetTimeResHdlr
* DESCRIPTION
*   This function is Set time response handler (MSG_ID_MMI_EQ_SET_RTC_TIME_RSP) 
*                                               for Phonesetup->Time and Date->Set Time/Date
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetSetTimeResHdlr(void *info)
{
	RESULTSTRUCT *resp;
	resp = (RESULTSTRUCT *)(info);

	if(resp->result == 1)
	{	
#ifdef MMI_ON_HARDWARE_P
		gclockData.rtc_year = g_phnset_cntx_p->settime.year - YEARFORMATE ;
		gclockData.rtc_mon = g_phnset_cntx_p->settime.month ;
		gclockData.rtc_day = g_phnset_cntx_p->settime.date;
		gclockData.rtc_hour = g_phnset_cntx_p->settime.hour ;
		gclockData.rtc_min = g_phnset_cntx_p->settime.minutes;
#else
		MyTime.nHour = g_phnset_cntx_p->settime.hour ;
		MyTime.nMin = g_phnset_cntx_p->settime.minutes;
		MyTime.nYear = g_phnset_cntx_p->settime.year;
		MyTime.nMonth = g_phnset_cntx_p->settime.month;
		MyTime.nDay = g_phnset_cntx_p->settime.date;
		DateTimerProc();
#endif

                // Lisen0522
       	        update_mainlcd_dt_display();
	        update_sublcd_dt_display();
//		AsyncEvent = FALSE;
//		oneoccured=1;
		AlmReInitialize();
		DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,ST_NOTIFYDURATION,SUCCESS_TONE);

		DeleteNHistory(1);
	}
	else
	{
//		AsyncEvent = FALSE;
		DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,ST_NOTIFYDURATION,ERROR_TONE);
//		oneoccured = 1;
	}
}




/*****************************************************************************
* FUNCTION
*  PhnsetSetTimeResHdlrForAT
* DESCRIPTION
*   This function is Set time response handler (MSG_ID_MMI_EQ_SET_RTC_TIME_RSP) 
*                                               for AT command, update idle screen, no popup
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
void PhnsetSetTimeResHdlrForAT(void *info)
{
	RESULTSTRUCT *resp;
	resp = (RESULTSTRUCT *)(info);
	
	if(resp->result == 1)
	{	
#ifdef MMI_ON_HARDWARE_P
		gclockData.rtc_hour = g_phnset_cntx_p->settime.hour;
		gclockData.rtc_min = g_phnset_cntx_p->settime.minutes;
		gclockData.rtc_mon = g_phnset_cntx_p->settime.month;
		gclockData.rtc_day = g_phnset_cntx_p->settime.date;
		gclockData.rtc_year = g_phnset_cntx_p->settime.year - YEARFORMATE;
#else
		MyTime.nHour = g_phnset_cntx_p->settime.hour;
		MyTime.nMin = g_phnset_cntx_p->settime.minutes;
		MyTime.nMonth = g_phnset_cntx_p->settime.month ;
		MyTime.nDay = g_phnset_cntx_p->settime.date;
		MyTime.nYear = g_phnset_cntx_p->settime.year;
		DateTimerProc();
#endif
		update_mainlcd_dt_display();
		update_sublcd_dt_display();
		AlmReInitialize();
	}
}



/*****************************************************************************
* FUNCTION
*  PhnsetGetTimeFormat
* DESCRIPTION
*   This function is to get time format
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
U8 PhnsetGetTimeFormat(void)
{
	
	S16 error;
	U8 output;

	ReadValue(TIME_FORMAT,&output,DS_BYTE,&error);
	//output = output & 0x0F;
	if((error !=  NVRAM_READ_SUCCESS) ||  output > NUM_TIME_FORMATS) {
		return 1;/*default format*/
	} else {
		return output ;
		
	}


}


/*****************************************************************************
* FUNCTION
*  PhnsetGetDateFormat
* DESCRIPTION
*   This function is to get date format
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
U8 PhnsetGetDateFormat(void)
{
	
	S16 error;
	U8 output;

	ReadValue(DATE_FORMAT,&output,DS_BYTE,&error);
	//output = output >> 4;
	if((error !=  NVRAM_READ_SUCCESS || output > NUM_DATE_FORMATS) ) {
		return 0;/*default format*/
	} else {
		return output ;/*first four bits*/
	}


}


/*****************************************************************************
* FUNCTION
*  PhnsetSetTimeFormat
* DESCRIPTION
*   This function is to set time format
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
S16 PhnsetSetTimeFormat(U8 timeformat) 
{
	
	S16 error = NVRAM_WRITE_SUCCESS;
	U8 input = timeformat;
	
	if(timeformat != PhnsetGetTimeFormat()) {
		WriteValue(TIME_FORMAT,&input, DS_BYTE,&error);
		gCurrentOnOff = input;
		//chgcnt ++;
	}
	
	return error;

}

/*****************************************************************************
* FUNCTION
*  PhnsetSetDateFormat
* DESCRIPTION
*   This function is to set date format
*   Functionality:
*
* PARAMETERS
*  a  IN/OUT 
*  b  IN       
* RETURNS
*  void
* GLOBALS AFFECTED
*  NULL
*****************************************************************************/
S16 PhnsetSetDateFormat(U8 dateformat)
{
	
	
	S16 error = NVRAM_WRITE_SUCCESS;
	U8 input = dateformat;

	if(dateformat != PhnsetGetDateFormat()) 
	{
		WriteValue(DATE_FORMAT,&input, DS_BYTE,&error);
		gCurrentOnOff = input;
		//SendMessageDateAndShowScreen();
		//chgcnt ++;
	} 
	return error;

}


/**************************************************************

	FUNCTION NAME		: ATSetHomeCity

  	PURPOSE				: Set home city from AT command

	INPUT PARAMETERS	: U8: index of city

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
BOOL ATSetHomeCity(U8  CityIndex)
{
	//tangjie del begin 20060620
	#if 0
	U8 PrevCity;
	S16 error;
	
	/* index exceeds boundary */
	if(CityIndex > NUMBER_OF_CITY /*|| CityIndex < 0 (unsigned comparison with 0 is always false)*/)
		return MMI_FALSE;

	/* read current city index from NVRAM */
	ReadValue(CURRENT_CITY,&PrevCity, DS_BYTE,&error);

	if(PrevCity != CityIndex)
	{
		g_phnset_cntx_p->currentCity = CityIndex;

		/* retrieve string pointer of city name */
		/* The index of menu is bigger than that one of TimeZoneData*/
		g_phnset_cntx_p->CurHomeCityStrPtr = 
		(PU8)((GetString)((U16)(TimeZoneData[g_phnset_cntx_p->currentCity+1].CityName)));

		/* write city index to NVRAM */
		WriteValue(CURRENT_CITY,&(g_phnset_cntx_p->currentCity), DS_BYTE,&error);

		/* write NITZ setting to NVRAM */
		PrevCity = (U8)(GetTimeZone(PhnsetGetHomeCity())*4);
		WriteValue( NVRAM_NITZTIMEZONE, &PrevCity, DS_BYTE, &error);

		/* update sub lcd display */
		ShowIdleAppSubLcd();
	}
	else
		return MMI_FALSE;

	return MMI_TRUE;
	#endif
	//tangjie del end 20060620
	return MMI_TRUE;
}




#if ( defined(__MMI_NITZ__) && defined(__MMI_AUTOTIMEZONE_IN_DATEANDTIME__)   )
/*****************************************************************************
 * FUNCTION
 *  PhnsetGetNITZFuncStatus
 * DESCRIPTION
 *  This function is to get NITZ function on/off status
 *  Functionality: Only exist when __MMI_NITZ__
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
MMI_BOOL PhnsetGetNITZFuncStatus(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 data = 0;
    S16 error;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    ReadValue( NVRAM_SETTING_AUTOUPDATE_DT_TIME, &data, DS_BYTE , &error);

    if(data == 1)		
    {   
        return MMI_TRUE;
    }
    else if(data == 0xff)
    {
        data = RstGetNITZStatusDefault();		
        WriteValue( NVRAM_SETTING_AUTOUPDATE_DT_TIME, &data, DS_BYTE , &error);
        return MMI_FALSE;
    }
    else
    {   
        return MMI_FALSE;
    }
}


/*****************************************************************************
 * FUNCTION
 *  HintPhnsetNITZ
 * DESCRIPTION
 *  This function is hint handling function
 *  for Phonesetup -> Auto Update of Date and Time
 *  Functionality:
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void HintPhnsetNITZ(U16 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(PhnsetGetNITZFuncStatus())
    {
        g_phnset_cntx_p->curNITZStatus = 1;
        pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON));
    }
    else
    {
        g_phnset_cntx_p->curNITZStatus = 0;
        pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));
    }
}


/*****************************************************************************
 * FUNCTION
 *  HighlightPhnsetNITZ
 * DESCRIPTION
 *  This function is highlight handler for Phonesetup-> Auto Update Date and Time
 *  Follow functions:tEntryAutoUpdateDtTime
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void HighlightPhnsetNITZ(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     g_phnset_cntx_p->curHightlightItem = PHNSET_DT_MNU_NITZ;
	
    /*Set the LSK to Off when Auto Update is ON*/
    if( g_phnset_cntx_p->curNITZStatus == 1 )
    {
        ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
    }
    else
    {
        ChangeLeftSoftkey(STR_GLOBAL_ON,0);
    }

    /* Change right soft key icon and label */
    ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);

    /* Change left soft key icon and label */
    SetKeyHandler(0, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

    /* Register function for left soft key */
    SetLeftSoftkeyFunction(EntryPhnsetNITZ,KEY_EVENT_UP);

    /* Register function for right soft key */
    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


/*****************************************************************************
 * FUNCTION
 *  EntryPhnsetNITZ
 * DESCRIPTION
 *  This function is Left softkey handler for Phonesetup-> Auto Update Date and Time
 *  Functionality: Toggle status only
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryPhnsetNITZ(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 index;
	S16 error;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    index = GetCurrHiliteID();

    /*Toggle Auto Update of Date And Time*/
    if( g_phnset_cntx_p->curNITZStatus == 0 )
    {
        g_phnset_cntx_p->curNITZStatus = 1;
    }
    else 
    {
        g_phnset_cntx_p->curNITZStatus = 0;
    }

    /* Store the updated Value */
    g_phnset_cntx_p->NITZDirtyFlag = TRUE;

    if(g_phnset_cntx_p->curNITZStatus)
    {
        pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_ON));
        Category53ChangeItemDescription(index, hintData[index]);
        ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
    }
    else
    {
        pfnUnicodeStrcpy((PS8)hintData[index],(PS8)GetString(STR_GLOBAL_OFF));
        Category53ChangeItemDescription(index, hintData[index]);
        ChangeLeftSoftkey(STR_GLOBAL_ON,0);
    }
    
    WriteValue( NVRAM_SETTING_AUTOUPDATE_DT_TIME, &(g_phnset_cntx_p->curNITZStatus), DS_BYTE , &error);

    RedrawCategoryFunction();
    HighlightPhnsetNITZ();
}
#endif  /* #if ( defined(__MMI_NITZ__) && defined(__MMI_AUTOTIMEZONE_IN_DATEANDTIME__)   )  */


#endif /* _DATEANDTIME_C */


