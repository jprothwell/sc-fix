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
 *	Worldclock.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file implements world clock application.
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

/**********************************************************************************
	Filename:		Worldclock.c
 
	Date Created:	June-14-2003
	Contains:		World Clock
**********************************************************************************/					
#ifndef _MMI_WORLDCLOCK_C
#define _MMI_WORLDCLOCK_C

/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "mmi_features.h"
#include "debuginitdef.h"
#include "organizerdef.h"
#include "mmi_data_types.h"
#include "worldclock.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "keybrd.h"
#include "idleappprot.h"
#include "custmenures.h"
#include "custdataprots.h"
#include "debuginitdef.h"
#include "worldclock.h"
#include "historygprot.h"
#include "globaldefs.h"
#include "unicodexdcl.h"
#include "globalmenuitems.h"
#include "organizerdef.h"
#include "commonscreens.h"
#include "wgui_categories.h"
#include "commonscreens.h"
#include "datetimegprot.h"
#ifdef __MMI_WORLDCLOCK_DST__
#include "wallpaperdefs.h"
#include "downloaddefs.h"
#include "phonesetup.h"
#endif

//... Add More MMI header
#undef __NEWSIMULATOR 

/*  Include: PS header file */
//... Add More PS header
/* 
** Define
*/
#ifdef __MMI_UI_STYLE_2__
	#define	WC_CITY_NAME_HEIGHT	MMI_TITLE_HEIGHT
	#define	WC_HEIGHT_160
#elif defined (__MMI_MAINLCD_176X220__)
	#define WC_CITY_NAME_HEIGHT		(MMI_TITLE_Y+MMI_TITLE_HEIGHT)
#else
	#define WC_CITY_NAME_HEIGHT	0
#endif
#if defined(__MMI_UI_STYLE_3__)		
#define WC_V_LINE_Y		WC_CITY_NAME_HEIGHT
#elif defined(__MMI_MAINLCD_128X128__)
#define WC_V_LINE_Y (MMI_TITLE_Y+MMI_TITLE_HEIGHT+WC_CITY_NAME_HEIGHT)
#else
#define WC_V_LINE_Y		(MMI_TITLE_Y+MMI_TITLE_HEIGHT+WC_CITY_NAME_HEIGHT+20)	/* y-coordinate of vertical line */
#endif
#define WC_H_LINE_X		0	/* x-coordinate of horizontal line */
#define WC_LINE_WIDTH	2
#define WC_NUM_CITY STR_TOTAL_NUM_CITY - STR_WCLOCK_CITY_NONE

 
#if defined(__MMI_UI_STYLE_4__) 
#define	WC_TITLE_HEIGHT	40
#define	WC_DATE_HEIGHT	35
#define	WC_TIME_HEIGHT	35
#else
#define	WC_TITLE_HEIGHT	24
#define	WC_DATE_HEIGHT	22
#define	WC_TIME_HEIGHT	23
#endif
 

/* 
** Typedef 
*/
#ifdef __MMI_TOUCH_SCREEN__
typedef enum
{
	WC_TOUCH_NONE,
	WC_TOUCH_ON_LEFT_ARROW_UP,
	WC_TOUCH_ON_LEFT_ARROW_DOWN,
	WC_TOUCH_ON_RIGHT_ARROW_UP,
	WC_TOUCH_ON_RIGHT_ARROW_DOWN,
	WC_TOUCH_ON_MAP,
	WC_TOUCH_ON_RSK
} WC_TOUCH_ON_E;
#endif

/* 
** Local Variable
*/

static U8 gWcTimeZoneIndex[WC_NUM_CITY];

#ifdef __MMI_MAINLCD_220X176__
	S32 image_Width = 0,image_Height = 0;
	S32 image_X_Begin = 0,image_Y_Begin = 0;
	#define GAP_BETWEEN_STRING 13
	#define OFFSET_X_STRING  8
#endif	
/*
** Local Function
*/



/*
** Global Variable
*/
#ifdef __MMI_WORLD_CLOCK__
wc_context_struct g_wc_cntx;
#endif


/* 
** Global Function
*/
extern void _show_bitmap(s32 x1,s32 y1,bitmap *b);
extern void _get_bitmap(s32 x1,s32 y1,s32 x2,s32 y2,bitmap *b);
extern U8 PhnsetGetHomeCity(void);

 
extern BOOL r2lMMIFlag;
//CSD end

#define WC_INITIALIZE
/*****************************************************************************
* FUNCTION
*  WcInit
* DESCRIPTION
*   Initailze world clock 
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void WcInit(void)
{	
#ifdef __MMI_WORLD_CLOCK__
	memset(&g_wc_cntx, 0, sizeof(wc_context_struct));
	g_wc_cntx.CurrCityIndex = 0xff;
#if defined( __MMI_UI_STYLE_3__) || defined (__MMI_UI_STYLE_4__)  
#ifndef __MMI_MAINLCD_220X176__
	coolsand_UI_measure_image(get_image(WORLD_CLOCK_LEFT_ARROW_UP),&(g_wc_cntx.arrow_width),&(g_wc_cntx.arrow_height));
#endif
#endif
#ifdef __MMI_TOUCH_SCREEN__
	g_wc_cntx.event_on_object=WC_TOUCH_NONE;
#endif

	SetHiliteHandler(ORGANIZER_WORLDCLOCK_MENU,HighlightWCMenu);
	SetHiliteHandler(MITEM_WORLDCLOCK_BROWSECITY,HighlightWcBrowseCity);

#if defined(__MMI_WORLD_CLOCK_SUBLCD_SETTING__)	
	SetHiliteHandler(MITEM_WORLDCLOCK_SUBLCDDISPLAY,HighlightWcSubLcd);
	SetHintHandler(MITEM_WORLDCLOCK_SUBLCDDISPLAY_CITY1,WcHintCity1);
	SetHintHandler(MITEM_WORLDCLOCK_SUBLCDDISPLAY_CITY2,WcHintCity2);
#endif
#endif
	WcGenerateTimeZoneIndex();
}



#define WC_UTILITY_FUNCTION
/*****************************************************************************
* FUNCTION
*  GetTimeZone
* DESCRIPTION
*   Return the time zone of a city	
* PARAMETERS
*	cityIndex	IN		index of city
* RETURNS
*	Time zone
* GLOBALS AFFECTED
*	None
*****************************************************************************/
//add by zough 061121 start
#ifndef __MOD_ORGANIZER__	
FLOAT GetTimeZone(U8 cityIndex) 
{
	return TimeZoneData[cityIndex+1].CityTimeZone;
}
#else
S32 GetTimeZone(U8 cityIndex) 
{
	return TimeZoneData[cityIndex+1].CityTimeZone;
}
#endif
//add by zough 061121 end


/*****************************************************************************
* FUNCTION
*	EntryWcSubLcd
* DESCRIPTION
*   Generate indexing table of world clock. The table is sorted by time zone and x/y-coordinate.
* PARAMETERS
*	nIndex	IN	index of item.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616

void WcGenerateTimeZoneIndex(void)
{
	S32 i,j;
	U8 tmp;
	U8 swap = FALSE;
	U8 index1, index2;
	U8 num_city = NUMBER_OF_CITY;
	for(i=0;i<num_city-1;i++)
		gWcTimeZoneIndex[i] = i+1;

	for(i=0; i<num_city-1; i++) 
	{
		for(j=0; j<(num_city-1)-i-1; j++)
		{
			index1 = gWcTimeZoneIndex[j+1];
			index2 = gWcTimeZoneIndex[j];
			if(TimeZoneData[index1].CityTimeZone < TimeZoneData[index2].CityTimeZone) 
	    	{  
				swap = TRUE;
			}
			else if(TimeZoneData[index1].CityTimeZone == TimeZoneData[index2].CityTimeZone) 
			{
				if(TimeZoneData[index1].CityX < TimeZoneData[index2].CityX)
					swap = TRUE;
				else if(TimeZoneData[index1].CityX == TimeZoneData[index2].CityX)
					if(TimeZoneData[index1].CityY < TimeZoneData[index2].CityY)
						swap = TRUE;
			}

			if(swap)
			{
   				tmp = gWcTimeZoneIndex[j];         /* swap a[j] and a[j+1]      */
				gWcTimeZoneIndex[j] = gWcTimeZoneIndex[j+1];
				gWcTimeZoneIndex[j+1] = tmp;				
				swap = FALSE;
			}
		}
	}			
}

//add by liuxn 060616 start
#else
void WcGenerateTimeZoneIndex(void)
{
	//add by zough 061121 start
	S32 i, j;
	U8 tmp;
	U8 swap = FALSE;
	U8 index1, index2;
	U8 num_city = NUMBER_OF_CITY;

	for (i = 0; i < num_city - 1; i++)
		gWcTimeZoneIndex[i] = i + 1;

	for (i=0; i < num_city - 1; i++) 
	{
		for (j = 0; j < (num_city - 1) - i - 1; j++)
		{
			index1 = gWcTimeZoneIndex[j + 1];
			index2 = gWcTimeZoneIndex[j];
			if (TimeZoneData[index1].MapIndex < TimeZoneData[index2].MapIndex) 
	    		{  
				swap = TRUE;
			}
			else if (TimeZoneData[index1].MapIndex == TimeZoneData[index2].MapIndex) 
			{
				if (TimeZoneData[index1].CityX < TimeZoneData[index2].CityX)
					swap = TRUE;
				else if (TimeZoneData[index1].CityX == TimeZoneData[index2].CityX)
					if (TimeZoneData[index1].CityY < TimeZoneData[index2].CityY)
						swap = TRUE;
			}

			if (swap)
			{
   				tmp = gWcTimeZoneIndex[j];         /* swap a[j] and a[j+1]      */
				gWcTimeZoneIndex[j] = gWcTimeZoneIndex[j + 1];
				gWcTimeZoneIndex[j + 1] = tmp;				
				swap = FALSE;
			}
		}
	}	
	//add by zough 061121 end
}
#endif
//add by liuxn 060616 end

/*****************************************************************************
* FUNCTION
*	mmi_wc_get_home_city
* DESCRIPTION
*   Return string of home city
* PARAMETERS
*	None.
* RETURNS
*	String pointer to home city name
* GLOBALS AFFECTED
*	None
*****************************************************************************/
S8* mmi_wc_get_home_city(void)
{
	return GetString(TimeZoneData[PhnsetGetHomeCity()+1].CityName);
}

/*****************************************************************************
* FUNCTION
*  WcSetCityIndex
* DESCRIPTION
*   Set current city.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void WcSetCityIndex(U16 index)
{
#if defined(__MMI_WORLD_CLOCK__)
	g_wc_cntx.CurrCityIndex = (U8)index;
#endif	
}

#ifdef __MMI_WORLD_CLOCK__

#define WC_BROWSE_CITY
/*****************************************************************************
* FUNCTION
*  HighlightWCMenu
* DESCRIPTION
*   Highlight handler of World Clock menu item. 
*	Register key handlers.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightWCMenu(void)
{
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
#if defined(__MMI_WORLD_CLOCK_SUBLCD_SETTING__	)
	SetKeyHandler(EntryWcApp,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetLeftSoftkeyFunction(EntryWcApp,KEY_EVENT_UP);
#else
	SetLeftSoftkeyFunction(EntryWcBrowseCity,KEY_EVENT_UP);
    /*+ zhouqin 2011-02-16 modify for q6 320X240 */	
	#ifndef __MMI_BLACK_BERRY_MAIN_MENU__
	SetKeyHandler(EntryWcBrowseCity,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	#endif
    /*- zhouqin 2011-02-16 modify for q6 320X240 */
#endif
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
    /*+ zhouqin 2011-02-16 modify for q6 320X240 */	
	#ifndef __MMI_BLACK_BERRY_MAIN_MENU__
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	#endif
    /*- zhouqin 2011-02-16 modify for q6 320X240 */
}

/*****************************************************************************
* FUNCTION
*	HighlightWcBrowseCity
* DESCRIPTION
*   Highlight handler of browse city menu
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightWcBrowseCity(void)
{	
	SetLeftSoftkeyFunction(EntryWcBrowseCity,KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*	EntryWcBrowseCity
* DESCRIPTION
*   Display browse city screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifdef __MMI_DRAW_MANAGER__
void WcShowCategory(void);
#endif
void EntryWcBrowseCity(void)
{
#ifdef __MMI_WORLDCLOCK_DST__
	S16 error;
	ReadValue(NVRAM_WC_DST, &g_wc_cntx.DSTForeign, DS_BYTE, &error);

	if(g_wc_cntx.DSTForeign == 0xff)
		g_wc_cntx.DSTForeign = 0; 
#endif
	EntryWcBrowseCityEx(EntryWcBrowseCity, ExitWcBrowseCity);
}

/*****************************************************************************
* FUNCTION
*	EntryWcBrowseCityEx
* DESCRIPTION
*   Display browse city screen
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryWcBrowseCityEx(void (*entry_func)(void), void (*exit_func)(void))
{
	/* initialize UI related variables */
	g_wc_cntx.CurrMap = 0xff;
	MMI_title_icon = NULL;
	MMI_menu_shortcut_number = -1;

	if(g_wc_cntx.CurrCityIndex==0xff)
		g_wc_cntx.CurrCityIndex = PhnsetGetHomeCity()+1;

	if(g_wc_cntx.VLine == NULL)
	{
		g_wc_cntx.VLine=OslMalloc(sizeof(bitmap));
		g_wc_cntx.VLine->data=OslMalloc(UI_device_height*WC_LINE_WIDTH);
	}
	
	if(g_wc_cntx.HLine == NULL)	
	{
		g_wc_cntx.HLine=OslMalloc(sizeof(bitmap));
		g_wc_cntx.HLine->data=OslMalloc(UI_device_width*WC_LINE_WIDTH);
	}
	
	EntryNewScreen(SCR_ID_WC_BROWSE_CITY, exit_func, entry_func, NULL);
#ifdef __MMI_MAINLCD_220X176__
	entry_full_screen();
#endif
	/* draw UI */
#ifdef __MMI_DRAW_MANAGER__
	WcShowCategory();
#else
	WcRedrawWorldClock(0,UI_device_height);
#endif

	SetRightSoftkeyFunction(WcHandleRSK,KEY_EVENT_UP);	
#ifdef __MMI_WORLDCLOCK_DST__
	SetLeftSoftkeyFunction(WcEntryDST,KEY_EVENT_UP);	
#endif

	SetKeyHandler(WcHandleKeyLeft,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(WcHandleKeyLeft,KEY_LEFT_ARROW,KEY_LONG_PRESS);	
	SetKeyHandler(WcHandleKeyRight,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetKeyHandler(WcHandleKeyRight,KEY_RIGHT_ARROW,KEY_LONG_PRESS);

	SetKeyHandler(WcHandleKeyEnd,KEY_END,KEY_EVENT_DOWN);

	RedrawCategoryFunction=WcRedrawCategory;
}

/*****************************************************************************
* FUNCTION
*	ExitWcBrowseCity
* DESCRIPTION
*   Exit function of browse city screen. Free allocated data.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void ExitWcBrowseCity(void)
{
	if(g_wc_cntx.VLine != NULL)
	{
		OslMfree(g_wc_cntx.VLine->data);
		OslMfree(g_wc_cntx.VLine);
		g_wc_cntx.VLine = NULL;
	}

	if(g_wc_cntx.HLine != NULL)
	{
		OslMfree(g_wc_cntx.HLine->data);
		OslMfree(g_wc_cntx.HLine);
		g_wc_cntx.HLine = NULL;
	}
#ifdef __MMI_MAINLCD_220X176__	
	leave_full_screen();
#endif
}


/*****************************************************************************
* FUNCTION
*	WcHandleRSK
* DESCRIPTION
*   LSK handler of browse city screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void WcHandleRSK(void)
{
	GoBackHistory();
	g_wc_cntx.CurrCityIndex=0xff;
}

#ifdef __MMI_WORLDCLOCK_DST__

void WcChangeDST(void)
{
	if(g_wc_cntx.DSTForeign)
	  g_wc_cntx.DSTForeign = FALSE;
	else
	  g_wc_cntx.DSTForeign = TRUE;

	if(g_wc_cntx.DSTForeign)
	{
		pfnUnicodeStrcpy((PS8)hintData[0],(PS8)GetString(STR_GLOBAL_ON));
		Category52ChangeItemDescription(0,hintData[0]);
		ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
	}
	else
	{
		pfnUnicodeStrcpy((PS8)hintData[0],(PS8)GetString(STR_GLOBAL_OFF));
		Category52ChangeItemDescription(0,hintData[0]);
		ChangeLeftSoftkey(STR_GLOBAL_ON,0);
	}
	RedrawCategoryFunction();
}

void WcDSTBackAndSave(void)
{
	S16 error;
	WriteValue( NVRAM_WC_DST, &g_wc_cntx.DSTForeign, DS_BYTE, &error );
	GoBackHistory();
}


void WcEntryDST(void)
{
	S32 num_of_items;
	U16 cityarray[1];
	U8 *guiBuffer;

	EntryNewScreen(SCR_ID_WC_DST, NULL, WcEntryDST, NULL);	
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_WC_DST);
	
	num_of_items = GetNumOfChild(MENU_ID_WC_DST_OPT);
	GetSequenceStringIds(MENU_ID_WC_DST_OPT, cityarray);
	subMenuDataPtrs[0] = (U8*)GetString(cityarray[0]);
	SetParentHandler(MENU_ID_WC_DST_OPT);

	if(g_wc_cntx.DSTForeign)
	   hintDataPtrs[0] = (U8*)GetString(STR_GLOBAL_ON);
	else
	   hintDataPtrs[0] = (U8*)GetString(STR_GLOBAL_OFF);

	ShowCategory53Screen(STR_WORLD_CLOCL_DST_TITLE,GetRootTitleIcon(ORGANIZER_WORLDCLOCK_MENU),
			STR_GLOBAL_ON,0,STR_GLOBAL_BACK,0,num_of_items,subMenuDataPtrs,
			(U16*)gIndexIconsImageList,hintDataPtrs,0,0,guiBuffer);

	if(g_wc_cntx.DSTForeign)
	   ChangeLeftSoftkey(STR_GLOBAL_OFF, IMG_GLOBAL_BACK);

	SetLeftSoftkeyFunction(WcChangeDST,KEY_EVENT_UP);
	SetRightSoftkeyFunction(WcDSTBackAndSave,KEY_EVENT_UP);	

}


#endif

/*****************************************************************************
* FUNCTION
*  WcTimeZoneDiff
* DESCRIPTION
*   Calculate the time zone difference between 2 cities.
* PARAMETERS
*	City1	IN		index of city 1.
*	City2	IN		index of city 2.
* RETURNS
*	time tone of city 1 - time zone of city 2.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616

FLOAT WcTimeZoneDiff(U8 City1, U8 City2)
{	
	if(City1 == 0 || City2 == 0)	/* of one the cities is "None" */
		return 25;
	else
		return (TimeZoneData[City1].CityTimeZone-TimeZoneData[City2].CityTimeZone);
}
//add by liuxn 060616 start
#else
//add by zough 061122 start
FLOAT WcTimeZoneDiff(U8 City1, U8 City2)
{
	
	if (City1 == 0 || City2 == 0)	/* of one the cities is "None" */
		return 50;
	else
		return (TimeZoneData[City1].CityTimeZone - TimeZoneData[City2].CityTimeZone);
//add by zough 061122 end
}
#endif
//add by liuxn 060616 end



/*****************************************************************************
* FUNCTION
*	WcGetCityPosition
* DESCRIPTION
*   Calculate the position of city in timezone order.
* PARAMETERS
*	index	IN		index of city.
* RETURNS
*	Position on gWcTimeZoneIndex array.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
U8 WcGetCityPosition(U8 index)
{
	U8 i, num_city;
	num_city = NUMBER_OF_CITY;
	for (i=0; i<num_city; i++)
		if (gWcTimeZoneIndex[i]==index)
			break;

	return i;
}


/*****************************************************************************
* FUNCTION
*	WcShowDateTime
* DESCRIPTION
*   Display date/time in world map
* PARAMETERS
*	timediff	IN	time difference between current city and home city.
* RETURNS
*	Position on gWcTimeZoneIndex array.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
#ifndef __MOD_ORGANIZER__	//add by liuxn 060616

void WcShowDateTime(void)
{
	MYTIME currTime, incTime;
#ifdef __MMI_WORLDCLOCK_DST__
   MYTIME dstTime;
#endif
	U8 Ucs2Date[30];
	U8 Ucs2Time[30];
#if !defined( __MMI_UI_STYLE_3__) && !defined (__MMI_UI_STYLE_4__)  
	U8 AsciiDate[30];
#endif
	UI_image_type imageday;
	color_t c;
	U8 HomeCity;
	FLOAT TimeDiff;
	S32 temp;
	U8 TimeFlag = 0;
	color_t cblue={160,205,250,100};

	coolsand_UI_lock_double_buffer();

	coolsand_UI_reset_clip();
	coolsand_UI_reset_text_clip();

	GetDateTime(&currTime);

	/* get time zone difference */
	HomeCity = PhnsetGetHomeCity()+1;
	TimeDiff = WcTimeZoneDiff(g_wc_cntx.CurrCityIndex, HomeCity);

	memset(&incTime, 0, sizeof(MYTIME));

	if(TimeDiff < 0)
	{
		TimeDiff = TimeDiff * (-1);		
		TimeFlag = 1;
	}

	temp = (S32)(TimeDiff*60);

	incTime.nDay = (U8)(TimeDiff/24);
	incTime.nHour = ((S32)TimeDiff)%24;
	incTime.nMin = temp%60;

	if(TimeFlag)	/* decrement */
		DecrementTime(currTime, incTime, &currTime);
	else
		currTime = IncrementTime(currTime, incTime);

#ifdef __MMI_WORLDCLOCK_DST__
   if(HomeCity!=g_wc_cntx.CurrCityIndex)  /* not home city */
   {
      memset(&dstTime, 0, sizeof(MYTIME));
      dstTime.nHour = 1;
      if(g_wc_cntx.DSTForeign && g_phnset_cntx_p->currentDST) /* both date light saving is on */
      {
         /* do nothing */
      }
      else if(g_phnset_cntx_p->currentDST) /* only home city turns on DST */
      {
   		DecrementTime(currTime, dstTime, &currTime);      
      }
      else if(g_wc_cntx.DSTForeign)/* only foreign city turns on DST */
      {
         currTime = IncrementTime(currTime, dstTime);
      }   
   }
#endif

	time_string(&currTime, (UI_string_type)Ucs2Time, DT_IDLE_SCREEN);
#if defined( __MMI_UI_STYLE_3__) || defined (__MMI_UI_STYLE_4__)  
	date_string(&currTime, (UI_string_type)Ucs2Date, DT_IDLE_SCREEN);
#else
	sprintf((S8*)(AsciiDate),"%02d/%02d",currTime.nMonth,currTime.nDay);
	AnsiiToUnicodeString((S8*)Ucs2Date,(S8*)AsciiDate);
#endif

	if ((currTime.nHour>=6) && (currTime.nHour<18)) 
		imageday=get_image(SUN_ICON);
	else
		imageday=get_image(MOON_ICON);
#ifndef __MMI_TOUCH_SCREEN__
#ifdef __MMI_MAINLCD_220X176__
	coolsand_UI_fill_rectangle(0,35,image_X_Begin,UI_device_height - MMI_SOFTKEY_HEIGHT,cblue);
	coolsand_UI_fill_rectangle(UI_device_width - image_X_Begin,35,UI_device_width,UI_device_height - MMI_SOFTKEY_HEIGHT,cblue);
#else
	coolsand_UI_fill_rectangle(0,MMI_content_height-7,UI_device_width-1,MMI_content_height+WC_V_LINE_Y,cblue);
#endif
#endif
	c=coolsand_UI_color(0,0,0);
	coolsand_UI_set_text_color(c);
	coolsand_UI_set_font(&MMI_medium_font);

#if defined( __MMI_UI_STYLE_3__) || defined (__MMI_UI_STYLE_4__)  
	{
	#ifdef __MMI_TOUCH_SCREEN__
		S32 str_w, str_h, str_h1,img_w,img_h,pos_x,pos_y;
		#if defined (__MMI_UI_STYLE_4__) 
			color_t date_bg=coolsand_UI_color(106,133,98);
		#else
		color_t date_bg=coolsand_UI_color(80,103,75);
		#endif
		color_t time_bg=coolsand_UI_color(0,0,0);
		color_t date_time_text_color=coolsand_UI_color(255,255,255);

		coolsand_UI_set_text_color(date_time_text_color);
		coolsand_UI_measure_string((UI_string_type)Ucs2Date, &str_w, &str_h1);
		coolsand_UI_measure_image(imageday,&img_w,&img_h);

		/* show date */
		coolsand_UI_fill_rectangle(0,UI_device_height-MMI_button_bar_height-WC_DATE_HEIGHT-WC_TIME_HEIGHT,UI_device_width,UI_device_height-MMI_button_bar_height-WC_TIME_HEIGHT,date_bg);
		if(r2lMMIFlag)
			coolsand_UI_move_text_cursor(((UI_device_width+str_w)>>1),UI_device_height-MMI_button_bar_height-WC_DATE_HEIGHT-WC_TIME_HEIGHT+((WC_DATE_HEIGHT-str_h1)>>1));
		else
			coolsand_UI_move_text_cursor(((UI_device_width-str_w)>>1),UI_device_height-MMI_button_bar_height-WC_DATE_HEIGHT-WC_TIME_HEIGHT+((WC_DATE_HEIGHT-str_h1)>>1));
		coolsand_UI_print_text((U16*)Ucs2Date);

		/* show time */
		coolsand_UI_measure_string((UI_string_type)Ucs2Time, &str_w, &str_h );
		pos_x=((UI_device_width-str_w-img_w-4)>>1);
		pos_y=UI_device_height-MMI_button_bar_height-WC_TIME_HEIGHT;
		coolsand_UI_fill_rectangle(0,pos_y,UI_device_width,pos_y+WC_TIME_HEIGHT,time_bg);
		if(r2lMMIFlag)
			coolsand_UI_move_text_cursor(pos_x+str_w,pos_y+((WC_TIME_HEIGHT-str_h)>>1));
		else
			coolsand_UI_move_text_cursor(pos_x,pos_y+((WC_TIME_HEIGHT-str_h)>>1));
		coolsand_UI_print_text((U16*)Ucs2Time);
		/* show day/night image */
		coolsand_UI_show_transparent_image(pos_x+str_w+2,pos_y+((WC_TIME_HEIGHT-img_h)>>1),imageday,0);
	#else
		S32 str_w, str_h, str_h1;
		coolsand_UI_measure_string((UI_string_type)Ucs2Date, &str_w, &str_h1);

		/* show date */
		 
		if(r2lMMIFlag)
			coolsand_UI_move_text_cursor(2+str_w,MMI_content_height);
		else
			coolsand_UI_move_text_cursor(2,MMI_content_height);
		//CSD end
		coolsand_UI_print_text((U16*)Ucs2Date);

		str_h1 += MMI_content_height;
		 
		if(r2lMMIFlag)
			coolsand_UI_move_text_cursor(2 + coolsand_UI_get_string_width((UI_string_type)Ucs2Time),str_h1+2);
		else
			coolsand_UI_move_text_cursor(2,str_h1+2);
		//CSD end
		coolsand_UI_print_text((U16*)Ucs2Time);

		coolsand_UI_measure_string((UI_string_type)Ucs2Time, &str_w, &str_h );
		coolsand_UI_show_transparent_image(str_w+15,str_h1+2,imageday,0);
	#endif
	}
#else
	{
		 
		if(r2lMMIFlag)
			coolsand_UI_move_text_cursor(2 + coolsand_UI_get_string_width((UI_string_type)Ucs2Date),MMI_content_height);
		else
			coolsand_UI_move_text_cursor(2,MMI_content_height);
		//CSD end
		coolsand_UI_print_text((U16*)Ucs2Date);
		
		 
		if(r2lMMIFlag)
			coolsand_UI_move_text_cursor(48 + coolsand_UI_get_string_width((UI_string_type)Ucs2Time),MMI_content_height);
		else
			coolsand_UI_move_text_cursor(48,MMI_content_height);
		//CSD end
		coolsand_UI_print_text((U16*)Ucs2Time);
		coolsand_UI_show_transparent_image(110,MMI_content_height,imageday,0);
	}
#endif

	#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0,MMI_content_height-5,UI_device_width-1,MMI_content_height+WC_V_LINE_Y);
	#endif

}

//add by liuxn 060616
#else
void WcShowDateTime(void)
{
	MYTIME currTime, incTime;
#ifdef __MMI_WORLDCLOCK_DST__
   MYTIME dstTime;
#endif
	U8 Ucs2Date[30];
	U8 Ucs2Time[30];
#if !defined( __MMI_UI_STYLE_3__) && !defined (__MMI_UI_STYLE_4__)  
	U8 AsciiDate[30];
#endif
	UI_image_type imageday;
	color_t c;
	U8 HomeCity;
	FLOAT TimeDiff;
	S32 temp;
	U8 TimeFlag = 0;

#ifdef __MMI_MAINLCD_220X176__
	FLOAT timezone;
	S32 iTimezone;
	U8 AsciiTimeZone[30];
	U8 Ucs2TimeZone[30];
#endif	
	
#ifndef __MMI_TOUCH_SCREEN__
	color_t cblue = {160, 205, 250, 100};
#endif

	coolsand_UI_lock_double_buffer();

	coolsand_UI_reset_clip();
	coolsand_UI_reset_text_clip();

	GetDateTime(&currTime);

	/* get time zone difference */
	HomeCity = PhnsetGetHomeCity() + 1;
	TimeDiff = WcTimeZoneDiff(g_wc_cntx.CurrCityIndex, HomeCity);

#ifdef __MMI_MAINLCD_220X176__
	timezone = TimeZoneData[g_wc_cntx.CurrCityIndex].CityTimeZone;
	iTimezone = (S32)(timezone*10);
	
	if(iTimezone >= 0)
	{
		sprintf((S8*)(AsciiTimeZone), "DST +%d.%d", iTimezone/10, iTimezone%10);
	}
	else
	{
		iTimezone *= -1;
		sprintf((S8*)(AsciiTimeZone), "DST -%d.%d", iTimezone/10, iTimezone%10);
	}
	AnsiiToUnicodeString((S8*)Ucs2TimeZone, (S8*)AsciiTimeZone);

#endif	
	memset(&incTime, 0, sizeof(MYTIME));

	if (TimeDiff < 0)
	{
		TimeDiff = TimeDiff * (-1);		
		TimeFlag = 1;
	}
	
	//add by zough 061121 start
	temp = (S32)(TimeDiff * 60);
	incTime.nDay = (U8)(TimeDiff /  24);
	incTime.nHour = ((S32)TimeDiff) % 24;
	//incTime.nHour = incTime.nHour / 2 ;			
	incTime.nMin = temp % 60;
	//add by zough 061121 end

	if (TimeFlag)	/* decrement */
	{
		DecrementTime(currTime, incTime, &currTime);
	}
	else
	{
		currTime = IncrementTime(currTime, incTime);
	}

#ifdef __MMI_WORLDCLOCK_DST__
   if (HomeCity != g_wc_cntx.CurrCityIndex)  /* not home city */
   {
      memset(&dstTime, 0, sizeof(MYTIME));
      dstTime.nHour = 1;
      if (g_wc_cntx.DSTForeign && g_phnset_cntx_p->currentDST) /* both date light saving is on */
      {
         /* do nothing */
      }
      else if (g_phnset_cntx_p->currentDST) /* only home city turns on DST */
      {
   		DecrementTime(currTime, dstTime, &currTime);   
      }
      else if (g_wc_cntx.DSTForeign)/* only foreign city turns on DST */
      {
         	currTime = IncrementTime(currTime, dstTime);
      }   
   }
#endif

	time_string(&currTime, (UI_string_type)Ucs2Time, DT_IDLE_SCREEN);
#if defined( __MMI_UI_STYLE_3__) || defined (__MMI_UI_STYLE_4__)  
	date_string(&currTime, (UI_string_type)Ucs2Date, DT_IDLE_SCREEN);
#else
	sprintf((S8*)(AsciiDate), "%02d/%02d", currTime.nMonth, currTime.nDay);
	AnsiiToUnicodeString((S8*)Ucs2Date, (S8*)AsciiDate);
#endif

	if ((currTime.nHour >= 6) && (currTime.nHour < 18)) 
	{
		imageday = get_image(SUN_ICON);
	}
	else
	{
		imageday = get_image(MOON_ICON);
	}
#ifndef __MMI_TOUCH_SCREEN__
#ifdef __MMI_MAINLCD_220X176__
	coolsand_UI_fill_rectangle(0,0,image_X_Begin,UI_device_height - MMI_SOFTKEY_HEIGHT ,cblue);
	coolsand_UI_fill_rectangle(UI_device_width-image_X_Begin,0,UI_device_width,UI_device_height - MMI_SOFTKEY_HEIGHT,cblue);
#else
#ifdef __MMI_MAINLCD_128X128__
    coolsand_UI_show_image(0,WC_V_LINE_Y,get_image((U16)(WORLD_CLOCK_IMAGE1+g_wc_cntx.CurrMap-1)));
#else
	coolsand_UI_fill_rectangle(0, MMI_content_height - 7, UI_device_width-1, MMI_content_height + WC_V_LINE_Y, cblue);
#endif
#endif
#endif
	c = coolsand_UI_color(0, 0, 0);
	coolsand_UI_set_text_color(c);
	coolsand_UI_set_font(&MMI_medium_font);

#if defined( __MMI_UI_STYLE_3__) || defined (__MMI_UI_STYLE_4__)  
	{
	#ifdef __MMI_TOUCH_SCREEN__
		S32 str_w, str_h, str_h1, img_w, img_h, pos_x, pos_y;
		#if defined (__MMI_UI_STYLE_4__) 
			color_t date_bg = coolsand_UI_color(106, 133, 98);
		#else
			color_t date_bg = coolsand_UI_color(80, 103, 75);
		#endif
		color_t time_bg = coolsand_UI_color(0, 0, 0);
		color_t date_time_text_color = coolsand_UI_color(255, 255, 255);

		coolsand_UI_set_text_color(date_time_text_color);
		coolsand_UI_measure_string((UI_string_type)Ucs2Date, &str_w, &str_h1);
		coolsand_UI_measure_image(imageday, &img_w, &img_h);

		/* show date */
		coolsand_UI_fill_rectangle(0, UI_device_height - MMI_button_bar_height - WC_DATE_HEIGHT - WC_TIME_HEIGHT, UI_device_width,UI_device_height-MMI_button_bar_height - WC_TIME_HEIGHT, date_bg);
		
		if (r2lMMIFlag)
		{
			coolsand_UI_move_text_cursor(((UI_device_width + str_w) >> 1), UI_device_height - MMI_button_bar_height - WC_DATE_HEIGHT - WC_TIME_HEIGHT + ((WC_DATE_HEIGHT - str_h1) >> 1));
		}
		else
		{
			coolsand_UI_move_text_cursor(((UI_device_width - str_w) >> 1), UI_device_height - MMI_button_bar_height - WC_DATE_HEIGHT - WC_TIME_HEIGHT + ((WC_DATE_HEIGHT - str_h1) >> 1));
		}
		coolsand_UI_print_text((U16*)Ucs2Date);

#ifdef __MMI_MAINLCD_220X176__

		/* show dst*/
		coolsand_UI_measure_string((UI_string_type)Ucs2TimeZone, &str_w, &str_h );
		pos_x = (((UI_device_width>>1)- str_w - 4) >> 1);
		pos_y = UI_device_height - MMI_button_bar_height - WC_TIME_HEIGHT;
		coolsand_UI_fill_rectangle(0, pos_y, UI_device_width, pos_y + WC_TIME_HEIGHT, time_bg);
		if (r2lMMIFlag)
		{
			coolsand_UI_move_text_cursor(pos_x + str_w, pos_y + ((WC_TIME_HEIGHT - str_h) >> 1));
		}
		else
		{
			coolsand_UI_move_text_cursor(pos_x, pos_y + ((WC_TIME_HEIGHT - str_h) >> 1));
		}
		coolsand_UI_print_text((U16*)Ucs2TimeZone);
		
		/* show time */
		coolsand_UI_measure_string((UI_string_type)Ucs2Time, &str_w, &str_h );
		pos_x = (UI_device_width>>1)+ (((UI_device_width>>1) - str_w - img_w - 4) >> 1);
		pos_y = UI_device_height - MMI_button_bar_height - WC_TIME_HEIGHT;
		 
		#else
		/* show time */
		coolsand_UI_measure_string((UI_string_type)Ucs2Time, &str_w, &str_h );
		pos_x = ((UI_device_width - str_w - img_w - 4) >> 1);
		pos_y = UI_device_height - MMI_button_bar_height - WC_TIME_HEIGHT;
		coolsand_UI_fill_rectangle(0, pos_y, UI_device_width, pos_y + WC_TIME_HEIGHT, time_bg);
		#endif
		
		if (r2lMMIFlag)
		{
			coolsand_UI_move_text_cursor(pos_x + str_w, pos_y + ((WC_TIME_HEIGHT - str_h) >> 1));
		}
		else
		{
			coolsand_UI_move_text_cursor(pos_x, pos_y + ((WC_TIME_HEIGHT - str_h) >> 1));
		}
		
		coolsand_UI_print_text((U16*)Ucs2Time);
		/* show day/night image */
		coolsand_UI_show_transparent_image(pos_x + str_w + 2, pos_y + ((WC_TIME_HEIGHT - img_h) >> 1), imageday, 0);
	#else
		S32 str_w, str_h, str_h1;
		#ifdef __MMI_MAINLCD_220X176__
			coolsand_UI_measure_string((UI_string_type)Ucs2Date, &str_w, &str_h1);

			/* show date */
			 
			if (r2lMMIFlag)
			{
				coolsand_UI_move_text_cursor(2 + str_w, MMI_content_height);
			}
			else
			{
				coolsand_UI_move_text_cursor(image_X_Begin + OFFSET_X_STRING, UI_DEVICE_HEIGHT - MMI_SOFTKEY_HEIGHT - str_h1);
			}
			//CSD end
			coolsand_UI_print_text((U16*)Ucs2Date);

			 
			if (r2lMMIFlag)
			{
				coolsand_UI_move_text_cursor(2 + coolsand_UI_get_string_width((UI_string_type)Ucs2Time), str_h1 + 2);
			}
			else
			{
				coolsand_UI_move_text_cursor(image_X_Begin + OFFSET_X_STRING +str_w + GAP_BETWEEN_STRING, UI_DEVICE_HEIGHT - MMI_SOFTKEY_HEIGHT - coolsand_UI_get_string_height((UI_string_type)Ucs2Time));
			}
			//CSD end
			coolsand_UI_print_text((U16*)Ucs2Time);

			coolsand_UI_measure_string((UI_string_type)Ucs2Time, &str_w, &str_h );
			coolsand_UI_show_transparent_image(image_X_Begin + OFFSET_X_STRING + coolsand_UI_get_string_width((UI_string_type)Ucs2Date) + GAP_BETWEEN_STRING + coolsand_UI_get_string_width((UI_string_type)Ucs2Time) + 13 , UI_DEVICE_HEIGHT - MMI_SOFTKEY_HEIGHT - str_h, imageday, 0);

		#else
		coolsand_UI_measure_string((UI_string_type)Ucs2Date, &str_w, &str_h1);

		/* show date */
		 
		if (r2lMMIFlag)
		{
			coolsand_UI_move_text_cursor(2 + str_w, MMI_content_height);
		}
		else
		{
			coolsand_UI_move_text_cursor(2, MMI_content_height);
		}
		//CSD end
		coolsand_UI_print_text((U16*)Ucs2Date);

		str_h1 += MMI_content_height;
		 
		if (r2lMMIFlag)
		{
			coolsand_UI_move_text_cursor(2 + coolsand_UI_get_string_width((UI_string_type)Ucs2Time), str_h1 + 2);
		}
		else
		{
			coolsand_UI_move_text_cursor(2, str_h1 + 2);
		}
		//CSD end
		coolsand_UI_print_text((U16*)Ucs2Time);

		coolsand_UI_measure_string((UI_string_type)Ucs2Time, &str_w, &str_h );
		coolsand_UI_show_transparent_image(str_w + 15, str_h1 + 2, imageday, 0);
	#endif
	#endif
	}
#else
	{
		 
		if (r2lMMIFlag)
		{
			coolsand_UI_move_text_cursor(2 + coolsand_UI_get_string_width((UI_string_type)Ucs2Date), MMI_content_height);
		}
		else
		{
			coolsand_UI_move_text_cursor(2, MMI_content_height);
		}
		//CSD end
		coolsand_UI_print_text((U16*)Ucs2Date);
		
		 
		if (r2lMMIFlag)
		{
			coolsand_UI_move_text_cursor(48 + coolsand_UI_get_string_width((UI_string_type)Ucs2Time), MMI_content_height);
		}
		else
		{
			coolsand_UI_move_text_cursor(48, MMI_content_height);
		}
		//CSD end
		coolsand_UI_print_text((U16*)Ucs2Time);
		coolsand_UI_show_transparent_image(110, MMI_content_height, imageday, 0);
	}
#endif

	#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0, MMI_content_height - 5, UI_device_width - 1,MMI_content_height + WC_V_LINE_Y);
	#endif

}
#endif
//add by liuxn 060616
/*****************************************************************************
* FUNCTION
*	WcHandleKeyRight
* DESCRIPTION
*   Right arrow key handler of browse city screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void WcHandleKeyRight(void)
{	
	S32 old_x_vertical,old_y_horizontal;
 	U8 curr_index;

	curr_index = WcGetCityPosition(g_wc_cntx.CurrCityIndex);

	old_x_vertical = TimeZoneData[gWcTimeZoneIndex[curr_index]].CityX;
	old_y_horizontal = TimeZoneData[gWcTimeZoneIndex[curr_index]].CityY + WC_V_LINE_Y;

	if (curr_index == (NUMBER_OF_CITY-2)) 
		curr_index=0;
	else
		curr_index++;

	g_wc_cntx.CurrCityIndex = gWcTimeZoneIndex[curr_index];

	WcRedrawWorldClock(old_x_vertical, old_y_horizontal);
}


/*****************************************************************************
* FUNCTION
*	WcHandleKeyLeft
* DESCRIPTION
*   Left arrow key handler of browse city screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void WcHandleKeyLeft(void)
{
	S32 old_x_vertical,old_y_horizontal;
	U8 curr_index;

	curr_index = WcGetCityPosition(g_wc_cntx.CurrCityIndex);

	old_x_vertical = TimeZoneData[gWcTimeZoneIndex[curr_index]].CityX;
	old_y_horizontal = TimeZoneData[gWcTimeZoneIndex[curr_index]].CityY + WC_V_LINE_Y;

	if (curr_index == 0) 
		curr_index=(NUMBER_OF_CITY-2); //56;
	else 
		curr_index--;

	g_wc_cntx.CurrCityIndex = gWcTimeZoneIndex[curr_index];

	WcRedrawWorldClock(old_x_vertical, old_y_horizontal);

}


/*****************************************************************************
* FUNCTION
*	WcHandleKeyEnd
* DESCRIPTION
*   End key handler of browse city screen.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void WcHandleKeyEnd(void)
{
	g_wc_cntx.CurrCityIndex = 0xff;
	DisplayIdleScreen();
}	


/*****************************************************************************
* FUNCTION
*	WcDrawTitle
* DESCRIPTION
*   Draw title bar and city name of browse city screen
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void WcDrawTitle(void)
{
	U8 i = g_wc_cntx.CurrCityIndex;
#if defined(WC_HEIGHT_160) || defined( __MMI_UI_STYLE_3__) || defined (__MMI_UI_STYLE_4__)
	color_t cblack={0,0,0,100};
	color_t cwhite={255,255,255,100};
#endif
#if defined( __MMI_UI_STYLE_3__) || defined (__MMI_UI_STYLE_4__)  
	S32 str_w=0,str_h=0;
#endif

#ifdef WC_HEIGHT_160	
	S32 width,height;
	UI_string_type CityName;

	MMI_title_string =(UI_string_type)GetString(STR_WORLD_CLOCK_TITLE);
	CityName = (UI_string_type)GetString(TimeZoneData[i].CityName);
	coolsand_UI_measure_string(CityName,&width,&height);
	coolsand_UI_fill_rectangle(0,(MMI_title_y+MMI_title_height),UI_device_width-1,WC_V_LINE_Y,cwhite);
	 
	if(r2lMMIFlag)
		coolsand_UI_move_text_cursor(((UI_device_width-width)>>1) + width,WC_V_LINE_Y-height);
	else
		coolsand_UI_move_text_cursor((UI_device_width-width)>>1,WC_V_LINE_Y-height);
	//CSD end
	coolsand_UI_set_text_color(cblack);
	coolsand_UI_print_text(CityName);
#else
#ifdef __MMI_MAINLCD_220X176__
	S32 width,height;
	UI_string_type WorldClockTitle;
	WorldClockTitle = (UI_string_type)GetString(STR_WORLD_CLOCK_TITLE);
	coolsand_UI_measure_string(WorldClockTitle,&width,&height);
	coolsand_UI_fill_rectangle(0,(MMI_title_y+MMI_title_height),UI_device_width-1,WC_V_LINE_Y,cwhite);
	 
	if(r2lMMIFlag)
		coolsand_UI_move_text_cursor(((UI_device_width-width)>>1) + width,WC_V_LINE_Y-height);
	else
		coolsand_UI_move_text_cursor((UI_device_width-width)>>1,(MMI_STATUS_BAR_HEIGHT-height)>>1);
	//CSD end
	coolsand_UI_set_text_color(cblack);
	coolsand_UI_print_text(WorldClockTitle);
#endif
	MMI_title_string =(UI_string_type)GetString(TimeZoneData[i].CityName);
#endif
	
#ifndef __MMI_MAINLCD_220X176__
	show_title_status_icon();
#endif
#if defined( __MMI_UI_STYLE_3__) || defined (__MMI_UI_STYLE_4__)  
	/* show title string */
	coolsand_UI_measure_string(MMI_title_string,&str_w,&str_h);
#ifndef __MMI_MAINLCD_220X176__
	coolsand_UI_fill_rectangle(0,MMI_status_bar_height,UI_device_width-1,MMI_status_bar_height-1+WC_TITLE_HEIGHT,cblack);
#endif

	if(r2lMMIFlag)
		coolsand_UI_move_text_cursor(((UI_device_width-str_w)>>1) + str_w,MMI_status_bar_height+((WC_TITLE_HEIGHT-str_h)>>1));
	else
	{
	#ifdef __MMI_MAINLCD_220X176__		
		coolsand_UI_move_text_cursor((UI_device_width-str_w)>>1,MMI_STATUS_BAR_HEIGHT + 1);
	#else
		coolsand_UI_move_text_cursor((UI_device_width-str_w)>>1,MMI_status_bar_height+((WC_TITLE_HEIGHT-str_h)>>1));
	#endif
	}
#ifdef __MMI_MAINLCD_220X176__	
	coolsand_UI_set_text_color(cblack);
#else
	coolsand_UI_set_text_color(cwhite);
#endif
	coolsand_UI_print_text(MMI_title_string);

#ifdef __MMI_TOUCH_SCREEN__
#if defined(__MMI_MAINLCD_220X176__)
#else
	/* show arrows */
	if ( g_wc_cntx.event_on_object == WC_TOUCH_ON_LEFT_ARROW_DOWN )
		coolsand_UI_show_transparent_image(0,MMI_status_bar_height+((WC_TITLE_HEIGHT-g_wc_cntx.arrow_height)>>1)-1,get_image(WORLD_CLOCK_LEFT_ARROW_DOWN),0);
	else
		coolsand_UI_show_transparent_image(0,MMI_status_bar_height+((WC_TITLE_HEIGHT-g_wc_cntx.arrow_height)>>1)-1,get_image(WORLD_CLOCK_LEFT_ARROW_UP),0);

	if ( g_wc_cntx.event_on_object == WC_TOUCH_ON_RIGHT_ARROW_DOWN )
		coolsand_UI_show_transparent_image(UI_device_width-g_wc_cntx.arrow_width-1,MMI_status_bar_height+((WC_TITLE_HEIGHT-g_wc_cntx.arrow_height)>>1)-1,get_image(WORLD_CLOCK_RIGHT_ARROW_DOWN),0);
	else
		coolsand_UI_show_transparent_image(UI_device_width-g_wc_cntx.arrow_width-1,MMI_status_bar_height+((WC_TITLE_HEIGHT-g_wc_cntx.arrow_height)>>1)-1,get_image(WORLD_CLOCK_RIGHT_ARROW_UP),0);
#endif
#endif
#else
	draw_title();
#endif

}

/*****************************************************************************
* FUNCTION
*	WcRedrawCategory
* DESCRIPTION
*   Redraw category function when theme update
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void WcRedrawCategory(void)
{

//	WcRedrawWorldClock(0,UI_DEVICE_HEIGHT);
	/* date and time information */
	WcShowDateTime();
	WcDrawTitle();
	show_softkey_background();
	ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	ChangeRightSoftkey(0,0);
}

#ifdef __MMI_TOUCH_SCREEN__
#include "wgui_touch_screen.h"
#define WC_PEN_GAP	3
MMI_BOOL WcPenDownHandler(mmi_pen_point_struct point)
{
	S32 number_of_city = mmi_wc_get_size();
	S32 i=0,pos_x=point.x,pos_y=point.y;
	U8 backup_idx = g_wc_cntx.CurrCityIndex;
	mmi_wgui_component_info_struct pos;
	
/* left arrow */
	pos.pos_x=0;
	pos.pos_y=MMI_status_bar_height+((WC_TITLE_HEIGHT-g_wc_cntx.arrow_height)>>1)-1;
	pos.height=g_wc_cntx.arrow_height;
	pos.width=g_wc_cntx.arrow_width;
	if ( mmi_wgui_test_object_position(point,pos) == MMI_TRUE )//LEFT ARROW
	{
		g_wc_cntx.event_on_object=WC_TOUCH_ON_LEFT_ARROW_DOWN;
		WcHandleKeyLeft();
		return MMI_TRUE;
	}

/* right arrow */
	pos.pos_x=UI_device_width-g_wc_cntx.arrow_width-1;
	pos.pos_y=MMI_status_bar_height+((WC_TITLE_HEIGHT-g_wc_cntx.arrow_height)>>1)-1;
	if ( mmi_wgui_test_object_position(point,pos) == MMI_TRUE )//RIGHT ARROW
	{
		g_wc_cntx.event_on_object=WC_TOUCH_ON_RIGHT_ARROW_DOWN;
		WcHandleKeyRight();
		return MMI_TRUE;
	}

	for ( i=0; i<number_of_city; i++ )
	{
		S32 old_x_vertical = TimeZoneData[i].CityX;
		S32 old_y_horizontal = TimeZoneData[i].CityY+ WC_V_LINE_Y ;
		if ( TimeZoneData[i].MapIndex == g_wc_cntx.CurrMap &&  pos_x >= old_x_vertical-WC_PEN_GAP && pos_x <= old_x_vertical+WC_PEN_GAP
			&& pos_y >= old_y_horizontal-WC_PEN_GAP && pos_y <= old_y_horizontal+WC_PEN_GAP )
		{
			g_wc_cntx.CurrCityIndex = i;
			WcRedrawWorldClock(TimeZoneData[backup_idx].CityX,TimeZoneData[backup_idx].CityY+ WC_V_LINE_Y);
			g_wc_cntx.event_on_object=WC_TOUCH_ON_MAP;
			return MMI_TRUE;
		}
	}
	g_wc_cntx.event_on_object=WC_TOUCH_NONE;
	return MMI_FALSE;
}

MMI_BOOL WcPenUpHandler(mmi_pen_point_struct point)
{
	mmi_wgui_component_info_struct pos;

/* left arrow */
	if ( g_wc_cntx.event_on_object == WC_TOUCH_ON_LEFT_ARROW_DOWN)
	{
		pos.pos_x=0;
		pos.pos_y=MMI_status_bar_height+((WC_TITLE_HEIGHT-g_wc_cntx.arrow_height)>>1)-1;
		pos.height=g_wc_cntx.arrow_height;
		pos.width=g_wc_cntx.arrow_width;
		if ( mmi_wgui_test_object_position(point,pos) == MMI_TRUE )//LEFT ARROW
		{
			coolsand_UI_show_transparent_image(pos.pos_x,pos.pos_y,get_image(WORLD_CLOCK_LEFT_ARROW_UP),0);
			coolsand_UI_BLT_double_buffer(pos.pos_x,pos.pos_y,pos.pos_x+pos.pos_x+pos.width-1,pos.pos_y+pos.height-1);
			g_wc_cntx.event_on_object=WC_TOUCH_NONE;
			return MMI_TRUE;
		}
	}

/* right arrow */
	if ( g_wc_cntx.event_on_object == WC_TOUCH_ON_RIGHT_ARROW_DOWN )
	{
		pos.pos_x=UI_device_width-g_wc_cntx.arrow_width-1;
		pos.pos_y=MMI_status_bar_height+((WC_TITLE_HEIGHT-g_wc_cntx.arrow_height)>>1)-1;
		pos.height=g_wc_cntx.arrow_height;
		pos.width=g_wc_cntx.arrow_width;
		if ( mmi_wgui_test_object_position(point,pos) == MMI_TRUE )//RIGHT ARROW
		{
			coolsand_UI_show_transparent_image(pos.pos_x,pos.pos_y,get_image(WORLD_CLOCK_RIGHT_ARROW_UP),0);
			coolsand_UI_BLT_double_buffer(pos.pos_x,pos.pos_y,pos.pos_x+pos.pos_x+pos.width-1,pos.pos_y+pos.height-1);
			g_wc_cntx.event_on_object=WC_TOUCH_NONE;
			return MMI_TRUE;
		}
	}

	return MMI_FALSE;
}

MMI_BOOL WcPenRepeatHandler(mmi_pen_point_struct point)
{
/* left arrow */
	if ( g_wc_cntx.event_on_object == WC_TOUCH_ON_LEFT_ARROW_DOWN)
	{
		WcHandleKeyLeft();
		return MMI_TRUE;
	}

/* right arrow */
	if ( g_wc_cntx.event_on_object == WC_TOUCH_ON_RIGHT_ARROW_DOWN)
	{
		WcHandleKeyRight();
		return MMI_TRUE;
	}

	return MMI_FALSE;
}

MMI_BOOL WcPenMoveHandler(mmi_pen_point_struct point)
{
	S32 number_of_city = mmi_wc_get_size();
	S32 i=0,pos_x=point.x,pos_y=point.y;
	U8 backup_idx = g_wc_cntx.CurrCityIndex;
	mmi_wgui_component_info_struct pos;
	
/* left arrow */
	if ( g_wc_cntx.event_on_object == WC_TOUCH_ON_LEFT_ARROW_DOWN || g_wc_cntx.event_on_object == WC_TOUCH_ON_LEFT_ARROW_UP)
	{
		pos.pos_x=0;
		pos.pos_y=MMI_status_bar_height+((WC_TITLE_HEIGHT-g_wc_cntx.arrow_height)>>1)-1;
		pos.height=g_wc_cntx.arrow_height;
		pos.width=g_wc_cntx.arrow_width;
		if ( mmi_wgui_test_object_position(point,pos) == MMI_TRUE )//LEFT ARROW
		{
			g_wc_cntx.event_on_object = WC_TOUCH_ON_LEFT_ARROW_DOWN;
			return MMI_TRUE;
		}
		else
		{
			if ( g_wc_cntx.event_on_object == WC_TOUCH_ON_LEFT_ARROW_DOWN)
			{
				g_wc_cntx.event_on_object = WC_TOUCH_ON_LEFT_ARROW_UP;
				coolsand_UI_show_transparent_image(pos.pos_x,pos.pos_y,get_image(WORLD_CLOCK_LEFT_ARROW_UP),0);
				coolsand_UI_BLT_double_buffer(pos.pos_x,pos.pos_y,pos.pos_x+pos.pos_x+pos.width-1,pos.pos_y+pos.height-1);
			}
			return MMI_FALSE;
		}
	}
/* right arrow */
	if ( g_wc_cntx.event_on_object == WC_TOUCH_ON_RIGHT_ARROW_DOWN || g_wc_cntx.event_on_object == WC_TOUCH_ON_RIGHT_ARROW_UP)
	{
		pos.pos_x=UI_device_width-g_wc_cntx.arrow_width-1;
		pos.pos_y=MMI_status_bar_height+((WC_TITLE_HEIGHT-g_wc_cntx.arrow_height)>>1)-1;
		pos.height=g_wc_cntx.arrow_height;
		pos.width=g_wc_cntx.arrow_width;
		if ( mmi_wgui_test_object_position(point,pos) == MMI_TRUE )//RIGHT ARROW
		{
			g_wc_cntx.event_on_object = WC_TOUCH_ON_RIGHT_ARROW_DOWN;
			return MMI_TRUE;
		}
		else
		{
			if ( g_wc_cntx.event_on_object == WC_TOUCH_ON_RIGHT_ARROW_DOWN )
			{
				 g_wc_cntx.event_on_object = WC_TOUCH_ON_RIGHT_ARROW_UP;
				coolsand_UI_show_transparent_image(pos.pos_x,pos.pos_y,get_image(WORLD_CLOCK_RIGHT_ARROW_UP),0);
				coolsand_UI_BLT_double_buffer(pos.pos_x,pos.pos_y,pos.pos_x+pos.pos_x+pos.width-1,pos.pos_y+pos.height-1);
			}
			return MMI_FALSE;
		}
	}

	for ( i=0; i<number_of_city; i++ )
	{
		S32 old_x_vertical = TimeZoneData[i].CityX;
		S32 old_y_horizontal = TimeZoneData[i].CityY+ WC_V_LINE_Y ;
		if ( TimeZoneData[i].MapIndex == g_wc_cntx.CurrMap &&  pos_x >= old_x_vertical-WC_PEN_GAP && pos_x <= old_x_vertical+WC_PEN_GAP
			&& pos_y >= old_y_horizontal-WC_PEN_GAP && pos_y <= old_y_horizontal+WC_PEN_GAP )
		{
			g_wc_cntx.CurrCityIndex = i;
			WcRedrawWorldClock(TimeZoneData[backup_idx].CityX,TimeZoneData[backup_idx].CityY+ WC_V_LINE_Y);
			g_wc_cntx.event_on_object=WC_TOUCH_ON_MAP;
			return MMI_TRUE;
		}
	}
	g_wc_cntx.event_on_object=WC_TOUCH_NONE;
	return MMI_FALSE;
}
#endif
#ifdef __MMI_DRAW_MANAGER__
#include "wgui_draw_manager.h"
void WcScreenDrawCallBack(dm_coordinates *coordinate )
{
	WcRedrawWorldClock(0,UI_device_height);
}
void WcShowCategory(void)
{
	dm_data_struct dm_data;

	dm_register_category_controlled_callback(WcScreenDrawCallBack);
#ifdef __MMI_TOUCH_SCREEN__
	mmi_wgui_register_category_screen_control_area_pen_handlers(WcPenDownHandler, MMI_PEN_EVENT_DOWN);
	mmi_wgui_register_category_screen_control_area_pen_handlers(WcPenUpHandler, MMI_PEN_EVENT_UP);
	mmi_wgui_register_category_screen_control_area_pen_handlers(WcPenRepeatHandler, MMI_PEN_EVENT_REPEAT);
	mmi_wgui_register_category_screen_control_area_pen_handlers(WcPenMoveHandler, MMI_PEN_EVENT_MOVE);
#endif
	dm_data.S32CatId=MMI_CATEGORY_WCLOCK;
	dm_data.S32ScrId=GetActiveScreenId();
	dm_data.S32flags=DM_CLEAR_SCREEN_BACKGROUND;
	dm_setup_data(&dm_data);
	dm_redraw_category_screen();	
}
#endif
/*****************************************************************************
* FUNCTION
*	WcRedrawWorldClock
* DESCRIPTION
*   Redraw world clock screen
* PARAMETERS
*	old_x_vertical	IN	coordinate of previous X.
*	old_y_vertical	IN	coordinate of previous Y.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void WcRedrawWorldClock(S32 old_x_vertical, S32 old_y_horizontal)
{
	U8 i;
	color_t cred={255,0,0,100};
	color_t cwhite={255,255,255,100};
	
#ifdef __MMI_MAINLCD_220X176__
	coolsand_UI_measure_image(get_image((U16)(WORLD_CLOCK_IMAGE1)),&image_Width,&image_Height);
	image_X_Begin=(UI_device_width-image_Width)>>1;
	image_Y_Begin=(UI_device_height-image_Height)>>1;
	image_X_Begin = 0;
	image_Y_Begin = 0;
#endif
	coolsand_UI_reset_clip();
	coolsand_UI_reset_text_clip();

	coolsand_UI_lock_double_buffer();

	i = g_wc_cntx.CurrCityIndex;
	
	if (TimeZoneData[i].MapIndex != g_wc_cntx.CurrMap)
	{	
	#ifdef __MMI_MAINLCD_220X176__
		coolsand_UI_fill_rectangle(0,MMI_title_y,UI_device_width-1,UI_device_height-1,cwhite);
		g_wc_cntx.CurrMap=TimeZoneData[i].MapIndex;
		coolsand_UI_show_image(image_X_Begin,image_Y_Begin,get_image((U16)(WORLD_CLOCK_IMAGE1+g_wc_cntx.CurrMap-1)));
	#else
		coolsand_UI_fill_rectangle(0,MMI_title_y,UI_device_width-1,UI_device_height-1,cwhite);
		g_wc_cntx.CurrMap=TimeZoneData[i].MapIndex;
		coolsand_UI_show_image(0,WC_V_LINE_Y,get_image((U16)(WORLD_CLOCK_IMAGE1+g_wc_cntx.CurrMap-1)));
	#endif
	}
	else
	{	
	#ifdef __MMI_MAINLCD_220X176__
		coolsand_UI_fill_rectangle(0,MMI_title_y,UI_device_width-1,UI_device_height-1,cwhite);
		coolsand_UI_show_image(image_X_Begin,image_Y_Begin,get_image((U16)(WORLD_CLOCK_IMAGE1+g_wc_cntx.CurrMap-1)));
	#endif
		_show_bitmap(old_x_vertical,WC_V_LINE_Y, g_wc_cntx.VLine);
		_show_bitmap(WC_H_LINE_X,old_y_horizontal, g_wc_cntx.HLine);
	}

	coolsand_UI_reset_clip();

	/*get the bitmap of the vertical and horizontal line*/
	_get_bitmap(TimeZoneData[i].CityX,WC_V_LINE_Y,TimeZoneData[i].CityX,MMI_content_height-8, g_wc_cntx.VLine);
	_get_bitmap(0,TimeZoneData[i].CityY+WC_V_LINE_Y,UI_device_height,TimeZoneData[i].CityY+WC_V_LINE_Y, g_wc_cntx.HLine);

	/* date and time information */
	WcShowDateTime();
    //Draw line after update DateTime
#ifdef __MMI_MAINLCD_220X176__
	coolsand_UI_draw_vertical_line(WC_V_LINE_Y,UI_device_height-35,TimeZoneData[i].CityX,cred);
	coolsand_UI_draw_horizontal_line(image_X_Begin,UI_device_width-image_X_Begin,TimeZoneData[i].CityY+WC_V_LINE_Y,cred);
#else	
	coolsand_UI_draw_vertical_line(WC_V_LINE_Y,MMI_content_height-8,TimeZoneData[i].CityX,cred);
	coolsand_UI_draw_horizontal_line(0,UI_device_height,TimeZoneData[i].CityY+WC_V_LINE_Y,cred);
#endif

	/* title */
	WcDrawTitle();
	register_default_hide_softkeys(); //to register softkeys hide default function
	show_softkey_background();
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
#ifdef __MMI_WORLDCLOCK_DST__
	ChangeLeftSoftkey(STR_GLOBAL_OPTIONS,IMG_GLOBAL_OPTIONS);
#else
	ChangeLeftSoftkey(0,0);
#endif

	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
}

#define WC_SUBLCD_DISPLAY
#if defined(__MMI_WORLD_CLOCK_SUBLCD_SETTING__)	
/*****************************************************************************
* FUNCTION
*  EntryWcApp
* DESCRIPTION
*   Entry function of world clock application.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryWcApp(void)
{	
	U16 WC_list_of_items[2];
	S32 num_of_items;
	U8 * guiBuffer;

	EntryNewScreen(SCR_ID_WC_APP, NULL, EntryWcApp, NULL);

	guiBuffer = GetCurrGuiBuffer(SCR_ID_WC_APP);

	num_of_items = GetNumOfChild(ORGANIZER_WORLDCLOCK_MENU);

	GetSequenceStringIds(ORGANIZER_WORLDCLOCK_MENU, WC_list_of_items);

	SetParentHandler(ORGANIZER_WORLDCLOCK_MENU);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	ShowCategory15Screen(STR_WORLD_CLOCK_TITLE, GetRootTitleIcon(ORGANIZER_WORLDCLOCK_MENU),
		STR_GLOBAL_OK,0,STR_GLOBAL_BACK,0,	num_of_items, WC_list_of_items,
		(U16*)gIndexIconsImageList, LIST_MENU, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*  WcGetNearbyCityNameIndex
* DESCRIPTION
*   Get the index of near by city.
* PARAMETERS
*	inp	IN	first character of city name.
* RETURNS
*	index of nearest city.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
U8 WcGetNearbyCityNameIndex(UI_character_type inp)
{
	S32 i;
	U16	n1=0;
	U8 num_city = NUMBER_OF_CITY;
	for (i=1;(i<(num_city));i++)
	{
		memcpy(&n1,subMenuDataPtrs[i],ENCODING_LENGTH);
		if(n1 >= inp )
			return (U8)(i-1);
	}

	return 255;	
}


/*****************************************************************************
* FUNCTION
*  WcMultitapInputCallBack
* DESCRIPTION
*   Call back function when input some characters in the screen.
* PARAMETERS
*	inp	IN	input character
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void WcMultitapInputCallBack(UI_character_type inp)
{
	g_wc_cntx.CurrHiliteCity =WcGetNearbyCityNameIndex(inp)+1;
	fixed_list_goto_item(g_wc_cntx.CurrHiliteCity);
}


/*****************************************************************************
* FUNCTION
*  WcMultitapCompleteCallBack
* DESCRIPTION
*   Call back function when multi-tap opeartion finished.
* PARAMETERS
*	None
* RETURNS
*	None
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void WcMultitapCompleteCallBack(void)
{
	fixed_list_goto_item(g_wc_cntx.CurrHiliteCity);
}


/*****************************************************************************
* FUNCTION
*	HighlightWcSubLcd
* DESCRIPTION
*   Highlight handler of Sub Lcd Display menu
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightWcSubLcd(void)
{
	SetLeftSoftkeyFunction(EntryWcSubLcd,KEY_EVENT_UP);
}


/*****************************************************************************
* FUNCTION
*	WcSaveSubLcdSetting
* DESCRIPTION
*   Save selected city to cache.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void WcSaveSubLcdSetting(void)
{
	S16 error;

	if(g_wc_cntx.CurrHiliteSublcd == 0)
		WriteValue(NVRAM_DOWNLOAD_CITY1, &g_wc_cntx.CurrHiliteCity, DS_BYTE ,&error);//homecity
	else
		WriteValue(NVRAM_DOWNLOAD_CITY2, &g_wc_cntx.CurrHiliteCity, DS_BYTE ,&error);
	
	SubLCDIdleScreen();
	DisplayPopup((U8*)GetString(STR_GLOBAL_DONE),IMG_GLOBAL_ACTIVATED,0,UI_POPUP_NOTIFYDURATION_TIME,SAVE_TONE);
}

/*****************************************************************************
* FUNCTION
*	HighlightWcSubLcdCity
* DESCRIPTION
*   Highlight handler of sub-lcd display city list.
* PARAMETERS
*	index	IN	index of selected item.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightWcSubLcdCity(S32 index)
{
	g_wc_cntx.CurrHiliteCity=(U8)index;	
}


/*****************************************************************************
* FUNCTION
*	EntryWcSelectCity
* DESCRIPTION
*   Display list of cities
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryWcSelectCity(void)
{	
	S32 i;
	U8 index;
	U8 num_city = NUMBER_OF_CITY;

	EntryNewScreen(SCR_ID_WC_OPTIONS, NULL, EntryWcSelectCity, NULL);	

	for (i=0; i<num_city; i++)
		pfnUnicodeStrcpy((S8*)subMenuDataPtrs[i], (S8*)GetString(TimeZoneData[i].CityName));
	
	if(g_wc_cntx.CurrHiliteSublcd == 0)
		index = g_wc_cntx.City1;
	else
		index = g_wc_cntx.City2;

	RegisterHighlightHandler(HighlightWcSubLcdCity);

	ShowCategory6Screen(STR_WORLD_SELECT_CITY,GetRootTitleIcon(ORGANIZER_WORLDCLOCK_MENU),
		STR_GLOBAL_OK,IMG_GLOBAL_OK,STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
		num_city,subMenuDataPtrs,index,NULL);

	if(!IsChineseSet())
	{
		set_numberless_multitap();
		register_multitap_no_draw_key_handlers();
		set_multitap_functions(WcMultitapInputCallBack,WcMultitapCompleteCallBack);
		change_multitap_mode(0);
	}


	SetLeftSoftkeyFunction(WcSaveSubLcdSetting,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*	WcHintCity1
* DESCRIPTION
*   Hint handler of city 1.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void WcHintCity1(U16 index)
{
	S16 error;

	ReadValue(NVRAM_DOWNLOAD_CITY1,&g_wc_cntx.City1,DS_BYTE,&error);//homecity
	if(g_wc_cntx.City1== 0xff)//not written anything in NVRAM
	{	
		g_wc_cntx.City1=PhnsetGetHomeCity()+1;
		WriteValue(NVRAM_DOWNLOAD_CITY1,&g_wc_cntx.City1,DS_BYTE,&error);//homecity
	}

	pfnUnicodeStrncpy((S8*)hintData[index], (S8*)GetString(TimeZoneData[g_wc_cntx.City1].CityName), MAX_SUBMENU_CHARACTERS);
}


/*****************************************************************************
* FUNCTION
*	WcHintCity2
* DESCRIPTION
*   Hint handler of city 2.
* PARAMETERS
*	None.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void WcHintCity2(U16 index)
{
	S16 error;

	ReadValue(NVRAM_DOWNLOAD_CITY2, &g_wc_cntx.City2, DS_BYTE ,&error);
	if (g_wc_cntx.City2==0xff)
	{
		g_wc_cntx.City2=0;
		WriteValue(NVRAM_DOWNLOAD_CITY2,&g_wc_cntx.City2,DS_BYTE,&error);//none
	}
	pfnUnicodeStrncpy((S8*)hintData[index], (S8*)GetString(TimeZoneData[g_wc_cntx.City2].CityName), MAX_SUBMENU_CHARACTERS);
}


/*****************************************************************************
* FUNCTION
*	HighlightWcSubLcdMenu
* DESCRIPTION
*   Highlight handler of Sub-Lcd menu.
* PARAMETERS
*	nIndex	IN	index of item.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void HighlightWcSubLcdMenu(S32 nIndex)
{
	g_wc_cntx.CurrHiliteSublcd = (U8)nIndex;
}


/*****************************************************************************
* FUNCTION
*	EntryWcSubLcd
* DESCRIPTION
*   Entry function of Sub-LCD display
* PARAMETERS
*	nIndex	IN	index of item.
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None
*****************************************************************************/
void EntryWcSubLcd(void)
{	
	S32 num_of_items;
	U16 cityarray[2];
	U8 * guiBuffer;
	U8*	u8WorldClockPop[2];

	EntryNewScreen(SCR_ID_WC_SUBLCD, NULL, EntryWcSubLcd, NULL);	
	
	guiBuffer = GetCurrGuiBuffer(SCR_ID_WC_SUBLCD);
	
	num_of_items = GetNumOfChild(MITEM_WORLDCLOCK_SUBLCDDISPLAY);
	GetSequenceStringIds(MITEM_WORLDCLOCK_SUBLCDDISPLAY, cityarray);
	SetParentHandler(MITEM_WORLDCLOCK_SUBLCDDISPLAY);

	RegisterHighlightHandler (HighlightWcSubLcdMenu);

	ConstructHintsList(MITEM_WORLDCLOCK_SUBLCDDISPLAY, u8WorldClockPop);

	ShowCategory52Screen(STR_WORLD_CLOCK_MENU2,GetRootTitleIcon(ORGANIZER_WORLDCLOCK_MENU),
			STR_GLOBAL_OPTIONS,0,STR_GLOBAL_BACK,0,num_of_items,cityarray,
			(U16*)gIndexIconsImageList,u8WorldClockPop,0,0,guiBuffer);

	SetLeftSoftkeyFunction(EntryWcSelectCity,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	
}

#endif //#if defined(__MMI_WORLD_CLOCK_SUBLCD_SETTING__)		
#endif // #ifdef __MMI_WORLD_CLOCK__
// CSD end	 

#endif //#ifndef _MMI_WORLDCLOCK_C


