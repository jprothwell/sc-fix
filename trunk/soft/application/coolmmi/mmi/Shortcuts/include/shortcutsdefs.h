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
 *	ShortcutsDefs.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file defines constant and enum value used in Shortcuts application.
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

	FILENAME	: ShortcutsDefs.h

  	PURPOSE		: Shortcuts application 

 

 

	DATE		: May 12,03

**************************************************************/
#ifndef _MMI_SHORTCUTSDEFS_H
#define _MMI_SHORTCUTSDEFS_H
#include "stdc.h"
#include "mmi_data_types.h"
#include "frameworkstruct.h"
/* 
** Define
*/
#define	SHCT_MAX_ALIAS_TABLE		1
#define	SHCT_COUNT_BYTE				0
#define	SHCT_INTEGRITY_BYTE			1
#define	SHCT_DATA_BYTE				2

#define	SHCT_MAX_SEL_LIST 			10
#define	SHCT_MAX_CANDI_LIST 		50
#define	SHCT_DEFAULT_MENU_ID		0
#define	SHCT_NVRAM_SIZE				250

/* 
** Typedef 
*/

/* Screen Ids */
typedef enum
{
	SCR_ID_SHCT_IDLE_SHORTCUT = SHORTCUTS_BASE+1,
	SCR_ID_SHCT_MAIN_MENU,
	SCR_ID_SHCT_EDIT,
	SCR_ID_SHCT_OPTION, //add by panxu
#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__)
	SCR_ID_SHCT_DEDICATED_KEYS,
	SCR_ID_SHCT_EDIT_DEDICATED_KEYS,			
#endif
	SCR_ID_SHTC_TOTAL
}SCR_ID_SHCT_ENUM;

/* String Ids */
typedef enum
{
SHORTCUTS_OPTIONS_EDIT_STRING_ID=SHORTCUTS_BASE+1,
SHORTCUTS_TITLE_STRING_ID,
SHORTCUTS_HINT_STRING_ID,
STR_ID_SHORTCUTS_LIST_FULL
}STR_ID_SHCT_ENUM;

/* Image Ids */
typedef enum
{
SHORTCUTS_OPTIONS_EDIT_IMAGE_ID=SHORTCUTS_BASE+1,
SHORTCUTS_TITLE_IMAGE_ID
}IMG_ID_SHCT_ENUM;

#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__)
typedef enum
{
	DEDICATED_KEY_UP		= 	0,
	DEDICATED_KEY_DOWN,
	DEDICATED_KEY_LEFT,
	DEDICATED_KEY_RIGHT,
	DEDICATED_KEY_OK,
	TOTAL_DEDICATED_KEYS
}SHCT_DEDICATED_KEY_ENUM;
#endif

typedef struct
{

	FuncPtr	LSKFunc;
#ifdef __MMI_TOUCH_SCREEN__	
	/* Always append one entry for Pen Calibration on display */
	U16	SelShctList[SHCT_MAX_SEL_LIST+1];
#else
	U16	SelShctList[SHCT_MAX_SEL_LIST];
#endif /*__MMI_TOUCH_SCREEN__*/
	U16	CandShctList[SHCT_MAX_CANDI_LIST];
#if defined(__MMI_DEDICATED_KEY_SHORTCUTS__)
	U16	DediShctList[TOTAL_DEDICATED_KEYS];
	U8	CurrSelArrow;
	U8	IsKeyRetrieving;
#endif
	U8	NumCandShct;	/* number of candidate shortcut */
	U8	NumSelShct;		/* number of selected shortcut */
	U8	CurrSelShct;
	U8	CurrEditShct;
	U8    CurrAddShct;

	U8    IsLeftKeyPressed;
	U8    IsRightKeyPressed;
	U8    IsUpKeyPressed;
	U8    IsDownKeyPressed;
	U8    IsOkKeyPressed;
}shct_context_struct;

/* 
** Extern Global Variable
*/
extern shct_context_struct g_shct_cntx;
/* 
** Extern Global Function
*/




#endif /* _MMI_SHORTCUTSDEFS_H */


