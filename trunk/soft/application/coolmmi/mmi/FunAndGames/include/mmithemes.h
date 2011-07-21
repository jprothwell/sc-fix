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
 *	MMIThemes.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file is defines enum values, structures, and function prototypes, for the Theme application
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: MMIThemes.h

  	PURPOSE		: Prototypes for the Theme application

 


	DATE		: Restructured on May-24-2003

**************************************************************/

#ifndef _MMI_THEME_H
#define _MMI_THEME_H

#include "mmi_features.h"
#include "cswtype.h" //added by lin for theme @20070620
#ifdef __MMI_THEMES_APPLICATION__

#ifdef __MMI_THEMES_V2_SUPPORT__
#include "thememanager.h"
#endif

#include "cswtype.h"

/* 
** Define
*/
#define NO_OF_ORDERS 				4
#define MAX_THEMES					10

 
#define DefaultThmID					1
#define DefaultThmIndex				0
#define MAX_ENTER_ADDRESS_SIZE 128
#define MAX_URL_ENTRIES 5
 
#if (!defined MMI_ON_HARDWARE_P) 
#define NVRAM_THEMES_DOWNLOAD_SIZE		1292
#endif

/* 
** Typedef 
*/
typedef enum
{
	THM_UPDATE_OFF				= 0,
	THM_UPDATE_HOURLY,
	THM_UPDATE_DAILY,
	THM_UPDATE_WEEKLY,
	THM_UPDATE_MONTHLY,
	THM_UPDATE_SEASONALLY
}thm_update_period_enum;

#ifdef __MMI_THEMES_V2_SUPPORT__
typedef struct{
	
	U8  CurrThemeIndex;                  /*current index of theme*/
	U32	CurrTheme; 						/* Current id of theme */
	U32	CurrHiliteTheme;				/* Current highlighted theme */
	U8	TotalTheme;						/* Total number of themes */	
	U8	CurrHiliteItem;					/* Current selected item */		
	U8	CurrUpdatePeriod;				/* Current update period */
	U8	IsSetFromMenu;					/* If theme is updated from menu */	
	U8	IsOrderChanged;					/* Is order changed by user */ /* shall be optimize later */
	U8	IsInThmApp;					/* Is current screen in theme application */
	U8	CurrOrderIndex;					/* Position of current update order */
	PU8 ThemeName[TOTAL_THEMES_COUNT+1];		/* Name list + "None"  11 */
	S32 TmpOrderList[TOTAL_THEMES_COUNT];		/* Item of set order  4 */
	U32 SetOrderIDList[TOTAL_THEMES_COUNT];
}thm_contex_struct;
#else
typedef struct{
	U8	CurrTheme; 						/* Current index of theme */
	U8	CurrHiliteTheme;				/* Current highlighted theme */
	U8	TotalTheme;						/* Total number of themes */	
	U8	CurrHiliteItem;					/* Current selected item */		
	U8	CurrUpdatePeriod;				/* Current update period */
	U8	IsSetFromMenu;					/* If theme is updated from menu */	
	U8	IsOrderChanged;					/* Is order changed by user */ /* shall be optimize later */
	U8	IsInThmApp;					/* Is current screen in theme application */
	S16	CurrOrderIndex;					/* Position of current update order */
	U8	CurrOrderList[NO_OF_ORDERS];	/* Order list of theme 4 */	
	PU8 ThemeName[MAX_THEMES+1];		/* Name list + "None"  11 */
	S32 TmpOrderList[NO_OF_ORDERS];		/* Item of set order  4 */
}thm_contex_struct;
#endif

#ifdef __MMI_THEMES_V2_SUPPORT__
typedef struct
{
	U16  freq_type;
	U16 set_order_flag;
	U32 themes_order[23];
}thm_nvram_struct;
#else
typedef struct
{
	U8  freq_type;
	U8 current_theme_index; 
	S16 set_order_flag;
	U8 themes_order[4];
}thm_nvram_struct;
#endif

#if defined(__MMI_DOWNLOADABLE_THEMES_SUPPORT__)
typedef struct
{
	S16 total_entries;
	S8 g_thm_recent_url[MAX_URL_ENTRIES][(MAX_ENTER_ADDRESS_SIZE+1)*ENCODING_LENGTH];
} thm_nvram_download_list;
#endif
/* 
** Extern Global Variable
*/
extern thm_contex_struct	g_thm_cntx;

/* 
** Extern Global Function
*/
void InitThemes(void);
BOOL ATSetTheme(U8 index);
void UpdateThemeAlarm(void);
void ReadThemesFromNVRAM(void);
/*
** Local Used Function
*/
void EntryThmMenu(void);
void ActivateTheme(void);
 
void DeleteTheme(void);//ritesh
 
void WriteThemesToNVRAM(void);
void HighlightThmMenu(void);
void HighlightThmItems(S32 nIndex);
void ThmHandleEndKey(void);
void ThmHandleRSK(void);


 
#ifdef __MMI_UI_TAB_PANE__
void ThemeAlarmCallBackUpdateUITheme(void);
#endif
 
#if 1//defined(__MMI_THEME_AUTO_UPDATE__)
void ThemeAlarmCallBack(void);
void EntryThmOptions(void);
void EntryThmUpdatePeriod(void);
void ExitThmUpdatePeriod(void);
void EntryThmSetOrder(void);
void ExitThmSetOrder(void);
 
void EntryThmDownload(void);
void ActivateDefaultTheme(void);
 
void UpdateThemeOff(void);
void UpdateThemePeriod(void);
void SaveOrderThemes(void);
void SetStartPositionofCurIndex(void);
void FillThemesList(void);
void HighlightThmUpdatePeriod(S32 nIndex);
void HighlightThmOption(S32 nIndex);
void HighlightThmSetOrder(S32 nIndex);

#ifdef __MMI_THEMES_V2_SUPPORT__
void ConstructThemeList(void);
void ConstructThemeOrderList(void);
void UpdateOrder(U32 thm_del_ID);
void SetOrderIDList(U32 IDList[], U8 noofthemes, U8 index);
U8 GetIDList(U32** GetIDList, U8* index);
U8 CheckForCorrectOrder(void);
void SetCurrOrderIndex(U8 index);
void ThemeDeleteConfirmation(void);
#endif

#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__ 
void ThemeDeleteConfirmation(void);
void DeleteTheme(void);
void UpdateOrder(U32 thm_del_ID);
#endif

#endif	// #if defined(__MMI_THEME_AUTO_UPDATE__)

#endif	// #ifdef __MMI_THEMES_APPLICATION__


#endif 


