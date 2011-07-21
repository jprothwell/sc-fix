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
 * Filename:
 * ---------
 *	Game.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Game Menu
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#include "mmi_features.h"

#ifdef __MMI_GAME__

#include "globalconstants.h"
#include "globaldefs.h"
#include "gui.h"
#include "custdatares.h"
#include "custmenures.h"
#include "custresdef.h"
#include "historygprot.h"
#include "miscfunctions.h"
#include "unicodexdcl.h"
#include "gamedefs.h"
#include "gameprots.h"
#include "funandgamesdefs.h"
#include "mainmenudef.h"
#include "gameframework.h"
#include "settingprofile.h"	/* success_tone define */
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"

#ifdef IS_F1RACE_BASE_GAME
/* under construction !*/
/* under construction !*/
#endif /* IS_F1RACE_BASE_GAME */

#ifdef IS_HANOI_BASE_GAME
#include "tohprots.h"
#include "tohdefs.h"
#endif /* IS_HANOI_BASE_GAME */

#ifdef IS_RICHES_BASE_GAME
#include "richesprots.h"
#include "richesdefs.h"
#endif /* IS_RICHES_BASE_GAME */

#ifdef IS_COLORBALL_BASE_GAME
/* under construction !*/
/* under construction !*/
#endif /* IS_COLORBALL_BASE_GAME */

#ifdef IS_SNAKE_BASE_GAME
/* under construction !*/
/* under construction !*/
#endif /* IS_SNAKE_BASE_GAME */

#ifdef IS_SMASH_BASE_GAME
/* under construction !*/
/* under construction !*/
#endif /* IS_SMASH_BASE_GAME */

#ifdef IS_DOLL_BASE_GAME
/* under construction !*/
/* under construction !*/
#endif /* IS_DOLL_BASE_GAME */

#ifdef IS_NINJA_BASE_GAME
/* under construction !*/
/* under construction !*/
#endif /* IS_NINJA_BASE_GAME */

#ifdef IS_MAJUNG_BASE_GAME
#include "majungprots.h"
#include "majungdefs.h"
#endif /* IS_MAJUNG_BASE_GAME */

#ifdef IS_MEMORY_BASE_GAME
/* under construction !*/
/* under construction !*/
#endif /* IS_MEMORY_BASE_GAME */


#ifdef IS_COPTER_BASE_GAME
/* under construction !*/
/* under construction !*/
#endif /* IS_COPTER_BASE_GAME */

#ifdef IS_PUZZLE_BASE_GAME
#include "puzzledefs.h"
#include "puzzleprots.h"
#endif /* IS_PUZZLE_BASE_GAME */

#ifdef IS_PANDA_BASE_GAME
#include "pandadefs.h"
#include "pandaprots.h"
#endif /* IS_PANDA_BASE_GAME */

#ifdef IS_STAIR_BASE_GAME 
/* under construction !*/
/* under construction !*/
#endif /* IS_STAIR_BASE_GAME */

#ifdef IS_UFO_BASE_GAME
#include "ufodefs.h"
#include "ufoprots.h"
#endif /* IS_UFO_BASE_GAME */

#ifdef IS_DANCE_BASE_GAME
/* under construction !*/
/* under construction !*/
#endif

#ifdef IS_MAGICSUSHI_BASE_GAME
#include "magicsushidefs.h"
#include "magicsushiprots.h"
#endif

#ifdef IS_FRUIT_BASE_GAME
#include "fruitresdef.h"
#include "fruitprots.h"
#endif
/****************************************************************************
* Macro                                                                
*****************************************************************************/
#define GAME_SWITCH(index, func_ptr)\
{\
	case index:\
		{\
			SetLeftSoftkeyFunction(func_ptr,KEY_EVENT_UP);\
			SetKeyHandler(func_ptr, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);\
			SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN); \
		}\
		break;\
}


/****************************************************************************
* Local Functions                                                        
*****************************************************************************/
void	mmi_game_highlight_hdlr(S32 index);
void	mmi_game_entry_app_screen(void);




/**************************************************************

	FUNCTION NAME		: mmi_game_highlight_hdlr

  	PURPOSE				: game selection menu highlight hdlr

	INPUT PARAMETERS	: S32 index
							 						  
	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void mmi_game_highlight_hdlr(S32 index)
{
    TRACE_EF(g_sw_GAME,"MMI_GAME: mmi_game_highlight_hdlr");
    switch(index)
	{

#ifdef IS_F1RACE_BASE_GAME
/* under construction !*/
#endif /* IS_F1RACE_BASE_GAME */

#ifdef IS_HANOI_BASE_GAME
	GAME_SWITCH(game_hanoi,TOH_Enter_GFX); 
#endif /* IS_HANOI_BASE_GAME */

#ifdef IS_RICHES_BASE_GAME
	GAME_SWITCH(game_riches,Riches_Enter_GFX); 
#endif /* IS_RICHES_BASE_GAME */

#ifdef IS_COLORBALL_BASE_GAME
/* under construction !*/
#endif /* IS_COLORBALL_BASE_GAME */

#ifdef IS_SNAKE_BASE_GAME
/* under construction !*/
#endif /* IS_SNAKE_BASE_GAME */

#ifdef IS_SMASH_BASE_GAME
/* under construction !*/
#endif /* IS_SMASH_BASE_GAME */

#ifdef IS_DOLL_BASE_GAME
/* under construction !*/
#endif /* IS_DOLL_BASE_GAME */

#ifdef IS_NINJA_BASE_GAME
/* under construction !*/
#endif /* IS_NINJA_BASE_GAME */

#ifdef IS_MAJUNG_BASE_GAME
	GAME_SWITCH(game_majung, Majung_Enter_GFX);
#endif /* IS_MAJUNG_BASE_GAME */

#ifdef IS_MEMORY_BASE_GAME
/* under construction !*/
#endif /* IS_MEMORY_BASE_GAME */

#ifdef IS_COPTER_BASE_GAME
/* under construction !*/
#endif /* IS_COPTER_BASE_GAME */

#ifdef IS_PUZZLE_BASE_GAME
	GAME_SWITCH(game_puzzle,mmi_gx_puzzle_enter_gfx); 
#endif /* IS_PUZZLE_BASE_GAME */

#ifdef IS_PANDA_BASE_GAME
	GAME_SWITCH(game_panda,mmi_gx_panda_enter_gfx); 
#endif /* IS_PANDA_BASE_GAME */

#ifdef IS_STAIR_BASE_GAME
/* under construction !*/
#endif /* IS_STAIR_BASE_GAME */

#if defined(IS_UFO_BASE_GAME) 
	GAME_SWITCH(game_ufo,mmi_gx_ufo_enter_gfx); 
#endif /* IS_UFO_BASE_GAME */

#ifdef IS_DANCE_BASE_GAME
/* under construction !*/
#endif /* IS_DANCE_BASE_GAME */

    #ifdef IS_FRUIT_BASE_GAME
            GAME_SWITCH(game_fruit, mmi_gx_fruit_enter_gfx);
    #endif 
#ifdef IS_MAGICSUSHI_BASE_GAME
	GAME_SWITCH(game_magicsushi,mmi_gx_magicsushi_enter_gfx); 
#endif /* IS_MAGICSUSHI_BASE_GAME */

	}
}

#if 0//defined(__MMI_NOKIA_STYLE_N800__)
void mmi_java_game_entry_app_screen(void)
{
#ifdef IS_UFO_BASE_GAME

	U8* guiBuffer;
	//U16 index = 0;
	U16 menu_str[1] = {STR_ID_GX_UFO_GAME_NAME};
	U16 menu_img[1] = {IMG_ID_GX_UFO_GAME_ICON};
	
	EntryNewScreen(SCR_JAVA_GAME,NULL,mmi_java_game_entry_app_screen,NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_JAVA_GAME);
	ShowCategory15Screen(STR_GAME_CAPTION,
						MAIN_MENU_TITLE_FUNANDGAMES_ICON,
						STR_GLOBAL_OK,
						IMG_GLOBAL_OK,
						STR_GLOBAL_BACK,
						IMG_GLOBAL_BACK,
						1,
						menu_str,
						menu_img,
						1,
						0,
						guiBuffer);
	SetLeftSoftkeyFunction(mmi_gx_ufo_enter_gfx,KEY_EVENT_UP);
	SetKeyHandler(mmi_gx_ufo_enter_gfx,KEY_RIGHT_ARROW,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	mmi_gx_load_setting();
    #endif
}
void mmi_java_game_highlight_app(void)
{
	SetLeftSoftkeyFunction(mmi_java_game_entry_app_screen,KEY_EVENT_UP);
	SetKeyHandler(mmi_java_game_entry_app_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
#endif
/**************************************************************

	FUNCTION NAME		: mmi_game_highlight_app

  	PURPOSE				: highlight game

	INPUT PARAMETERS	: nil
							 						  
	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
#if defined(__MMI_GAME__)
void mmi_game_highlight_app(void)
{
    TRACE_EF(g_sw_GAME,"MMI_GAME: mmi_game_highlight_app");
    SetLeftSoftkeyFunction(mmi_game_entry_app_screen,KEY_EVENT_UP);
    SetKeyHandler(mmi_game_entry_app_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
}
#endif /* __MMI_GAME__ */


/**************************************************************

	FUNCTION NAME		: mmi_game_entry_app_screen

  	PURPOSE				: game entry screen

	INPUT PARAMETERS	: nil
							 						  
	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void mmi_game_entry_app_screen(void)
{
	U8* guiBuffer;
	UI_time t;
	#ifdef WIN32
	U16 menu_str[5];
	U16 menu_img[5]; 
	#else
	U16 menu_str[game_list_count];
	U16 menu_img[game_list_count]; 
	#endif
	U16 index = 0;
	

#ifdef IS_F1RACE_BASE_GAME
/* under construction !*/
/* under construction !*/
#endif /* IS_F1RACE_BASE_GAME */

#ifdef IS_HANOI_BASE_GAME
	menu_str[index]	= STR_GX_TOH_GAME_NAME; //not used now. wufasong 20061106
	menu_img[index++]	= IMG_GX_TOH_GAME_ICON; 
#endif /* IS_HANOI_BASE_GAME */

#ifdef IS_RICHES_BASE_GAME
	menu_str[index]	= STR_GX_RICHES_GAME_NAME; //not used now. wufasong 20061106
	menu_img[index++]	= IMG_GX_RICHES_GAME_ICON; 
#endif /* IS_RICHES_BASE_GAME */

#ifdef IS_COLORBALL_BASE_GAME
/* under construction !*/
/* under construction !*/
#endif /* IS_COLORBALL_BASE_GAME */

#ifdef IS_SNAKE_BASE_GAME
/* under construction !*/
/* under construction !*/
#endif /* IS_SNAKE_BASE_GAME */

#ifdef IS_SMASH_BASE_GAME
/* under construction !*/
/* under construction !*/
#endif /* IS_SMASH_BASE_GAME */

#ifdef IS_DOLL_BASE_GAME
/* under construction !*/
/* under construction !*/
#endif /* IS_DOLL_BASE_GAME */

#ifdef IS_NINJA_BASE_GAME
/* under construction !*/
/* under construction !*/
#endif /* IS_NINJA_BASE_GAME */

#ifdef IS_MAJUNG_BASE_GAME
	menu_str[index]	= STR_GX_MAJUNG_GAME_NAME; //not used now. wufasong 20061106
	menu_img[index++]	=  IMG_GX_MAJUNG_GAME_ICON; 
#endif /* IS_MAJUNG_BASE_GAME */

#ifdef IS_MEMORY_BASE_GAME
/* under construction !*/
/* under construction !*/
#endif /* IS_MEMORY_BASE_GAME */

#ifdef IS_COPTER_BASE_GAME
/* under construction !*/
	#if defined(__MMI_GAME_COPTER_128x128__) || defined(__MMI_GAME_COPTER_128x160__) || defined(__MMI_GAME_COPTER_176x220__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
	#elif defined(__MMI_GAME_SUBMARINE_128x128__) || defined(__MMI_GAME_SUBMARINE_128x160__) || defined(__MMI_GAME_SUBMARINE_176x220__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
	#elif defined(__MMI_GAME_JET_128x128__) || defined(__MMI_GAME_JET_128x160__) || defined(__MMI_GAME_JET_176x220__)
/* under construction !*/
	#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /* IS_COPTER_BASE_GAME */

#ifdef IS_PUZZLE_BASE_GAME
	menu_str[index]		= STR_ID_GX_PUZZLE_GAME_NAME;  //ÖÇÄÜÆ´Í¼ wufasong 20061106
	menu_img[index++]		= IMG_ID_GX_PUZZLE_GAME_ICON; 
#endif /* IS_PUZZLE_BASE_GAME */

#ifdef IS_PANDA_BASE_GAME
	/* panda */
	#if defined(__MMI_GAME_PANDA_128x128__) || defined(__MMI_GAME_PANDA_128x160__) || defined(__MMI_GAME_PANDA_176x220__)
		menu_str[index]	= STR_ID_GX_PANDA_GAME_NAME;
	
	/* monkey */
	#elif defined(__MMI_GAME_MONKEY_128x128__) || defined(__MMI_GAME_MONKEY_128x160__) || defined(__MMI_GAME_MONKEY_176x220__)
		menu_str[index]	= STR_ID_GX_PANDA_GAME_NAME_MONKEY;//Ã¨ÐÜ wufasong 20061106
	#endif
	
	menu_img[index++]		=  IMG_ID_GX_PANDA_GAME_ICON; 
	
#endif /* IS_PANDA_BASE_GAME */

#ifdef IS_STAIR_BASE_GAME
/* under construction !*/
/* under construction !*/
	#if defined(__MMI_GAME_CHICKEN_128x160__) || defined(__MMI_GAME_CHICKEN_128x128__)
/* under construction !*/
/* under construction !*/
/* under construction !*/
	#elif defined(__MMI_GAME_ROBOT_176x220__) || defined(__MMI_GAME_ROBOT_128x160__)
/* under construction !*/
/* under construction !*/
	#endif
/* under construction !*/
/* under construction !*/
#endif /* IS_STAIR_BASE_GAME */
#ifdef IS_FRUIT_BASE_GAME
    menu_str[index] = STR_ID_GX_FRUIT_GAME_NAME;
    menu_img[index++] = IMG_ID_GX_FRUIT_GAME_ICON;
#endif /* IS_FRUIT_BASE_GAME */ 

#if defined(IS_UFO_BASE_GAME) 

	/* ufo */
	#if defined(__MMI_GAME_UFO_128x160__) || defined(__MMI_GAME_UFO_128x128__) || defined(__MMI_GAME_UFO_176x220__)|| defined(__MMI_GAME_UFO_220x176__)
		menu_str[index]		= STR_ID_GX_UFO_GAME_NAME;//ÓÄ¸¡ÈëÇÖ wufasong 20061106
		menu_img[index++]		= IMG_ID_GX_UFO_GAME_ICON; 

	/* air force */
	#elif defined(__MMI_GAME_AIRFORCE_128x160__) || defined(__MMI_GAME_AIRFORCE_128x128__)
		menu_str[index]		= STR_ID_GX_UFO_GAME_NAME_AIRFORCE;
		menu_img[index++]		= IMG_ID_GX_UFO_GAME_ICON; 
	#endif
	
#endif /* IS_UFO_BASE_GAME */

#ifdef IS_DANCE_BASE_GAME
/* under construction !*/
/* under construction !*/
#endif /* IS_DANCE_BASE_GAME */

#ifdef IS_MAGICSUSHI_BASE_GAME
	menu_str[index]		= STR_ID_GX_MAGICSUSHI_GAME_NAME; //not used now. wufasong 20061106
	menu_img[index++]		= IMG_ID_GX_MAGICSUSHI_GAME_ICON; 
#endif /* IS_MAGICSUSHI_BASE_GAME */

	/* Set random seed */
	GetDateTime(&t);
	srand(t.nMin);	

	EntryNewScreen(SCR_GAME, NULL, mmi_game_entry_app_screen, NULL);
	
	guiBuffer = GetCurrGuiBuffer (SCR_GAME);		 

	RegisterHighlightHandler(mmi_game_highlight_hdlr);

	ShowCategory15Screen(	STR_GAME_CAPTION, MAIN_MENU_TITLE_FUNANDGAMES_ICON, STR_GLOBAL_OK, IMG_GLOBAL_OK,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK, game_list_count,
									menu_str, menu_img, 1, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	/* load game settings */
	mmi_gx_load_setting();
}


#endif /* __MMI_GAME__ */


