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

/*******************************************************************************
 * Filename:
 * ---------
 *	Worldclock.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file is defines constant, enum value, structure, and function prototype for world clock.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**********************************************************************************
	Filename:		worldclock.h
 
	Date Created:	July-14-2003
	Contains:		WorldClock
**********************************************************************************/
#ifndef _MMI_WORLDCLOCK_H
#define _MMI_WORLDCLOCK_H

//#define __MMI_WORLDCLOCK_DST__

#include "mmi_features.h"
#include "mmi_data_types.h"
#include "gui_data_types.h"

/* 
** Define
*/

/* 
** Typedef 
*/
typedef struct 
{	
		FLOAT		CityTimeZone;		/* time difference */
	//S32		CityTimeZone;		/* time difference */
	U16			CityName;			/* city name in English. */
	U16			CityAbbr;			/* abbreviation name of city. */
	U8			CityX;				/* the X coordinate of city. */
	U8			CityY;				/* the Y coordinate of city. */
	U8          MapIndex;
}TimeZoneStruct;

typedef struct
{
	bitmap* VLine;	/* bitmap object to store the image of vertical and horizontal line */
	bitmap* HLine;
	U8	CurrMap;		/* index of current image map */
	U8	CurrCityIndex;	/* index of current selected city in world map */
#if defined(__MMI_WORLD_CLOCK_SUBLCD_SETTING__)	
	U8	CurrHiliteSublcd;
	U8	CurrHiliteCity;	/* for multitap selection of cities */
	U8	City1;
	U8	City2;
#endif
#ifdef __MMI_WORLDCLOCK_DST__
	U8       DSTForeign;
#endif
	S32	arrow_width,arrow_height;
#ifdef __MMI_TOUCH_SCREEN__
	U32	event_on_object;
#endif
}wc_context_struct;

typedef enum
{
	ORGANIZER_MENU_WORLDCLOCK_IMAGEID =  WORLD_CLOCK +1,
	WORLD_CLOCK_IMAGE1,   //16352
	WORLD_CLOCK_IMAGE2,  //16353
	WORLD_CLOCK_IMAGE3,  //16354
	SUN_ICON, //16355
	MOON_ICON,  //16356
	WORLD_CLOCK_LEFT_ARROW_UP,   //16357
	WORLD_CLOCK_LEFT_ARROW_DOWN,   //16358
	WORLD_CLOCK_RIGHT_ARROW_UP,  //16359
	WORLD_CLOCK_RIGHT_ARROW_DOWN  //16360
}WC_IMG_ID_ENUM;

typedef enum
{
	STR_WCLOCK_CITY_NONE=WORLD_CLOCK +1,
	STR_WCLOCK_CITY1,
	STR_WCLOCK_CITY2,
	STR_WCLOCK_CITY3,
	STR_WCLOCK_CITY4,
	STR_WCLOCK_CITY5,
	STR_WCLOCK_CITY6,
	STR_WCLOCK_CITY7,
	STR_WCLOCK_CITY8,
	STR_WCLOCK_CITY9,
	STR_WCLOCK_CITY10,
	STR_WCLOCK_CITY11,
	STR_WCLOCK_CITY12,
	STR_WCLOCK_CITY13,
	STR_WCLOCK_CITY14,
	STR_WCLOCK_CITY15,
	STR_WCLOCK_CITY16,
	STR_WCLOCK_CITY17,
	STR_WCLOCK_CITY18,
	STR_WCLOCK_CITY19,
	STR_WCLOCK_CITY20,
	STR_WCLOCK_CITY21,
	STR_WCLOCK_CITY22,
	STR_WCLOCK_CITY23,
	STR_WCLOCK_CITY24,
	STR_WCLOCK_CITY25,
	STR_WCLOCK_CITY26,
	STR_WCLOCK_CITY27,
	STR_WCLOCK_CITY28,
	STR_WCLOCK_CITY29,
	STR_WCLOCK_CITY30,
	STR_WCLOCK_CITY31,
	STR_WCLOCK_CITY32,
	STR_WCLOCK_CITY33,
	STR_WCLOCK_CITY34,
	STR_WCLOCK_CITY35,
	STR_WCLOCK_CITY36,
	STR_WCLOCK_CITY37,
	STR_WCLOCK_CITY38,
	STR_WCLOCK_CITY39,
	STR_WCLOCK_CITY40,
	STR_WCLOCK_CITY41,
	STR_WCLOCK_CITY42,
	STR_WCLOCK_CITY43,
	STR_WCLOCK_CITY44,
	STR_WCLOCK_CITY45,
	STR_WCLOCK_CITY46,
	STR_WCLOCK_CITY47,
	STR_WCLOCK_CITY48,
	STR_WCLOCK_CITY49,
	STR_WCLOCK_CITY50,
	STR_WCLOCK_CITY51,
	STR_WCLOCK_CITY52,
	STR_WCLOCK_CITY53,
	STR_WCLOCK_CITY54,
	STR_WCLOCK_CITY55,
	STR_WCLOCK_CITY56,
	STR_WCLOCK_CITY57,
	STR_WCLOCK_CITY58,
	STR_WCLOCK_CITY59,
	STR_WCLOCK_CITY60,
	STR_TOTAL_NUM_CITY,
	STR_WCLOCK_CITY_ABBRE_NONE,
	STR_WCLOCK_CITY_ABBRE1,
	STR_WCLOCK_CITY_ABBRE2,
	STR_WCLOCK_CITY_ABBRE3,
	STR_WCLOCK_CITY_ABBRE4,
	STR_WCLOCK_CITY_ABBRE5,
	STR_WCLOCK_CITY_ABBRE6,
	STR_WCLOCK_CITY_ABBRE7,
	STR_WCLOCK_CITY_ABBRE8,
	STR_WCLOCK_CITY_ABBRE9,
	STR_WCLOCK_CITY_ABBRE10,
	STR_WCLOCK_CITY_ABBRE11,
	STR_WCLOCK_CITY_ABBRE12,
	STR_WCLOCK_CITY_ABBRE13,
	STR_WCLOCK_CITY_ABBRE14,
	STR_WCLOCK_CITY_ABBRE15,
	STR_WCLOCK_CITY_ABBRE16,
	STR_WCLOCK_CITY_ABBRE17,
	STR_WCLOCK_CITY_ABBRE18,
	STR_WCLOCK_CITY_ABBRE19,
	STR_WCLOCK_CITY_ABBRE20,
	STR_WCLOCK_CITY_ABBRE21,
	STR_WCLOCK_CITY_ABBRE22,
	STR_WCLOCK_CITY_ABBRE23,
	STR_WCLOCK_CITY_ABBRE24,
	STR_WCLOCK_CITY_ABBRE25,
	STR_WCLOCK_CITY_ABBRE26,
	STR_WCLOCK_CITY_ABBRE27,
	STR_WCLOCK_CITY_ABBRE28,
	STR_WCLOCK_CITY_ABBRE29,
	STR_WCLOCK_CITY_ABBRE30,
	STR_WCLOCK_CITY_ABBRE31,
	STR_WCLOCK_CITY_ABBRE32,
	STR_WCLOCK_CITY_ABBRE33,
	STR_WCLOCK_CITY_ABBRE34,
	STR_WCLOCK_CITY_ABBRE35,
	STR_WCLOCK_CITY_ABBRE36,
	STR_WCLOCK_CITY_ABBRE37,
	STR_WCLOCK_CITY_ABBRE38,
	STR_WCLOCK_CITY_ABBRE39,
	STR_WCLOCK_CITY_ABBRE40,
	STR_WCLOCK_CITY_ABBRE41,
	STR_WCLOCK_CITY_ABBRE42,
	STR_WCLOCK_CITY_ABBRE43,
	STR_WCLOCK_CITY_ABBRE44,
	STR_WCLOCK_CITY_ABBRE45,
	STR_WCLOCK_CITY_ABBRE46,
	STR_WCLOCK_CITY_ABBRE47,
	STR_WCLOCK_CITY_ABBRE48,
	STR_WCLOCK_CITY_ABBRE49,
	STR_WCLOCK_CITY_ABBRE50,
	STR_WCLOCK_CITY_ABBRE51,
	STR_WCLOCK_CITY_ABBRE52,
	STR_WCLOCK_CITY_ABBRE53,
	STR_WCLOCK_CITY_ABBRE54,
	STR_WCLOCK_CITY_ABBRE55,
	STR_WCLOCK_CITY_ABBRE56,
	STR_WCLOCK_CITY_ABBRE57,
	STR_WCLOCK_CITY_ABBRE58,
	STR_WCLOCK_CITY_ABBRE59,
	STR_WCLOCK_CITY_ABBRE60,
	STR_WORLD_CLOCK_TITLE,
	STR_WORLD_CLOCK_MENU1,
#if defined(__MMI_WORLD_CLOCK_SUBLCD_SETTING__)			
	STR_WORLD_CLOCK_MENU2,
	STR_WORLD_CLOCK_CITY1,
	STR_WORLD_CLOCK_CITY2,
	STR_WORLD_SELECT_CITY,
#endif
#ifdef __MMI_WORLDCLOCK_DST__
   STR_WORLD_CLOCK_DST_FOREIGN,
   STR_WORLD_CLOCL_DST_TITLE,
#endif
	ORGANIZER_MENU_WORLDCLOCK_STRINGID
}WC_STR_ID_ENUM;

typedef enum
{
	SCR_ID_WC_BROWSE_CITY = WORLD_CLOCK +1,//SCREEN IDS
#if defined(__MMI_WORLD_CLOCK_SUBLCD_SETTING__)	
	SCR_ID_WC_APP,
	SCR_ID_WC_SUBLCD,
	SCR_ID_WC_OPTIONS,
#endif
#ifdef __MMI_WORLDCLOCK_DST__
   SCR_ID_WC_DST,
#endif
	SCR_ID_WC_TOTAL

}WC_SCR_ID_ENUM;

/* 
** Extern Global Variable
*/
extern const TimeZoneStruct TimeZoneData[];
extern U8 mmi_wc_get_size(void);
#define NUMBER_OF_CITY mmi_wc_get_size()

#ifdef __MMI_WORLD_CLOCK__
extern wc_context_struct g_wc_cntx;
#endif

/* 
** Extern Global Function
*/
extern void WcInit(void);
extern void WcGenerateTimeZoneIndex(void);
//add by zough 061121 start
#ifndef __MOD_ORGANIZER__
extern FLOAT GetTimeZone(U8 cityIndex);
#else
extern S32 GetTimeZone(U8 cityIndex);
#endif
//add by zough 061121 end
extern S8* mmi_wc_get_home_city(void);
extern void WcSetCityIndex(U16 index);

#ifdef __MMI_WORLD_CLOCK__
extern void HighlightWCMenu(void);
extern void EntryWcBrowseCity(void);
extern void EntryWcBrowseCityEx(void (*entry_func)(void), void (*exit_func)(void));
extern void ExitWcBrowseCity(void);
extern void HighlightWcBrowseCity(void);

//add by zough 061121 start
#ifndef __MOD_ORGANIZER__	
extern FLOAT WcTimeZoneDiff(U8 City1, U8 City2);
#else
extern FLOAT WcTimeZoneDiff(U8 City1, U8 City2);
#endif
//add by zough 061121 end

extern void WcShowDateTime(void);
extern void WcHandleKeyLeft(void);
extern void WcHandleKeyRight(void);
extern void WcHandleKeyEnd(void);
extern void WcEntryDST(void);
extern void WcHandleRSK(void);
extern void WcRedrawWorldClock(S32 old_x_vertical, S32 old_y_horizontal);

extern void WcRedrawCategory(void);

#if defined(__MMI_WORLD_CLOCK_SUBLCD_SETTING__)
extern void HighlightWcSubLcd(void);
extern void EntryWcSubLcd(void);
extern void EntryWcSelectCity(void); // sub-LCD
extern void EntryWcApp(void);
extern void WcHintCity1(U16 index);
extern void WcHintCity2(U16 index);
#endif
#endif
#endif // _MMI_WORLDCLOCK_H


