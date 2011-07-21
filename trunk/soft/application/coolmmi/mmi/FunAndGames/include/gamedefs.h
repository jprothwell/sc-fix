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
 *	 GameDefs.h
  *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Game Defines
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/


#ifndef GAME_DEF_H
#define GAME_DEF_H

#include "mmi_data_types.h"	
#include "settingdefs.h"
#include "mmi_features.h"

/* skip audio align in PC environment */
#ifdef WIN32
	#define __align(X)
#endif

/* define game base ID */
/* (GAME_BASE) - (GAME_BASE + 99) is reserved for game.c */

#define GAME_FRAMEWORK								GAME_BASE + 100

#define GAME_SNAKE_BASE								GAME_BASE + 150  /* GAME_BASE defined in mmi_data_types.h*/
#define GAME_F1RACE_BASE							GAME_BASE + 200
#define GAME_TOH_BASE								GAME_BASE + 250
#define GAME_RICHES_BASE							GAME_BASE + 300
#define GAME_COLORBALLS_BASE						GAME_BASE + 350
#define GAME_SMASH_BASE								GAME_BASE + 400
#define GAME_DOLL_BASE								GAME_BASE + 450
#define GAME_NINJA_BASE								GAME_BASE + 500
#define GAME_MAJUNG_BASE							GAME_BASE + 550	/* !! Majung needs 150 id for images */
#define GAME_MEMORY_BASE							GAME_BASE + 700
#define GAME_COPTER_BASE							GAME_BASE + 750
#define GAME_PUZZLE_BASE							GAME_BASE + 800   /* !! Puzzle needs 100 id for images */
#define GAME_PANDA_BASE								GAME_BASE + 900
#define GAME_STAIR_BASE								GAME_BASE + 950
#define GAME_UFO_BASE								GAME_BASE + 1000	/* !! Ufo need 100 id for images */
#define GAME_DANCE_BASE								GAME_BASE + 1100
#define GAME_MAGICSUSHI_BASE						GAME_BASE + 1150
#define GAME_FRUIT_BASE                                                 GAME_BASE + 1700   /* !! Fruit need 100 id for images */
/* Game effect */
/* comment this if you dont want game effect */
#define GAME_AUDIO_ENABLE		
#define GAME_VIBRATOR_ENABLE		


/* Game define Macro */
#if defined(__MMI_GAME_F1RACE__)
	#define IS_F1RACE_BASE_GAME
#endif 

#if defined(__MMI_GAME_HANOI__)
	#define IS_HANOI_BASE_GAME
#endif 

#if defined(__MMI_GAME_RICHES__)
	#define IS_RICHES_BASE_GAME
#endif 

#if defined(__MMI_GAME_COLORBALLS__)
	#define IS_COLORBALL_BASE_GAME
#endif 

#if defined(__MMI_GAME_SNAKE__)
	#define IS_SNAKE_BASE_GAME
#endif 

#if defined(__MMI_GAME_SMASH__)
	#define IS_SMASH_BASE_GAME
#endif 

#if defined(__MMI_GAME_DOLL__)
	#define IS_DOLL_BASE_GAME
#endif 

#if defined(__MMI_GAME_NINJA_128x96__)
	#define IS_NINJA_BASE_GAME
#endif 

#if defined(__MMI_GAME_MAJUNG_128x96__)||defined(__MMI_GAME_MAJUNG_240x320__)||defined(__MMI_GAME_FRUIT_176x220__)
	#define IS_MAJUNG_BASE_GAME
#endif 

#if defined(__MMI_GAME_FRUIT_176x220__)
    #define IS_FRUIT_BASE_GAME
#endif 

#if defined(__MMI_GAME_MEMORY_128x96__)
	#define IS_MEMORY_BASE_GAME
#endif 

#if defined(__MMI_GAME_DANCE_176x220__)
	#define IS_DANCE_BASE_GAME
#endif 

#if defined(__MMI_GAME_COPTER_128x128__) || defined(__MMI_GAME_COPTER_128x160__) || defined(__MMI_GAME_COPTER_176x220__) || defined(__MMI_GAME_SUBMARINE_128x128__) || defined(__MMI_GAME_SUBMARINE_128x160__) || defined(__MMI_GAME_SUBMARINE_176x220__) || defined(__MMI_GAME_JET_128x128__) || defined(__MMI_GAME_JET_128x160__) || defined(__MMI_GAME_JET_176x220__)
	#define IS_COPTER_BASE_GAME
#endif
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_GAME_PUZZLE_128x128__) || defined(__MMI_GAME_PUZZLE_128x160__) || defined(__MMI_GAME_PUZZLE_160x128__) || defined(__MMI_GAME_PUZZLE_176x220__) || defined(__MMI_GAME_PUZZLE_240x320__)||defined(__MMI_GAME_PUZZLE_220x176__)||defined(__MMI_GAME_PUZZLE_320x240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
	#define IS_PUZZLE_BASE_GAME
#endif

#if defined(__MMI_GAME_PANDA_128x128__) || defined(__MMI_GAME_PANDA_128x160__) || defined(__MMI_GAME_MONKEY_128x128__) || defined(__MMI_GAME_MONKEY_128x160__) //|| defined(__MMI_GAME_MONKEY_176x220__)// defined(__MMI_GAME_PANDA_176x220__) ||
	#define IS_PANDA_BASE_GAME
#endif

#if defined(__MMI_GAME_CHICKEN_128x160__) || defined(__MMI_GAME_CHICKEN_128x128__) || defined(__MMI_GAME_ROBOT_176x220__) || defined(__MMI_GAME_ROBOT_128x160__)
	#define IS_STAIR_BASE_GAME
#endif

#if defined(__MMI_GAME_UFO_128x128__) || defined(__MMI_GAME_UFO_128x160__) || defined(__MMI_GAME_UFO_176x220__)|| defined(__MMI_GAME_UFO_220x176__)
	#define IS_UFO_BASE_GAME
#endif

#if defined(__MMI_GAME_MAGICSUSHI__)
	#define IS_MAGICSUSHI_BASE_GAME
#endif


typedef enum {

#ifdef IS_F1RACE_BASE_GAME
/* under construction !*/
#endif /* IS_F1RACE_BASE_GAME */

#ifdef IS_HANOI_BASE_GAME
	game_hanoi,
#endif /* IS_HANOI_BASE_GAME */

#ifdef IS_RICHES_BASE_GAME
	game_riches,
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
	game_majung,
#endif /* IS_MAJUNG_BASE_GAME */

#ifdef IS_MEMORY_BASE_GAME
/* under construction !*/
#endif /* IS_MEMORY_BASE_GAME */

#ifdef IS_COPTER_BASE_GAME
/* under construction !*/
#endif /* IS_COPTER_BASE_GAME */

#ifdef IS_PUZZLE_BASE_GAME
	game_puzzle,
#endif /* IS_PUZZLE_BASE_GAME */

#ifdef IS_PANDA_BASE_GAME
	game_panda,
#endif /* IS_PANDA_BASE_GAME */

#ifdef IS_STAIR_BASE_GAME 
/* under construction !*/
#endif /* IS_STAIR_BASE_GAME */

#if defined(IS_UFO_BASE_GAME)  
	game_ufo,
#endif /* IS_UFO_BASE_GAME */

#ifdef IS_DANCE_BASE_GAME
/* under construction !*/
#endif /* IS_DANCE_BASE_GAME */

#ifdef IS_MAGICSUSHI_BASE_GAME
	game_magicsushi,
#endif /* IS_MAGICSUSHI_BASE_GAME */
#ifdef IS_FRUIT_BASE_GAME
    game_fruit,
#endif 

	game_list_count
} GAME_LIST_NEUM;

/* Screen ID */
typedef enum {
	SCR_GAME = GAME_BASE+1,
#if defined(__MMI_NOKIA_STYLE_N800__)
	SCR_JAVA_GAME,
#endif
	SCR_ID_GFX_SETTING_SCREEN
} SCREENID_LIST_GAME;


/* String ID */
typedef enum {
	STR_GAME_CAPTION = GAME_BASE+1,
	STR_GAME_LSK,
	STR_GAME_NEW,
	STR_GAME_LEVEL,
	STR_GAME_GRADE,
	STR_GAME_HELP,
#ifdef __T800_MAIN_MENU__
      STR_GAME_SOUND_SWITCH,
#endif
	STR_GAME_RESUME,
	STR_GAME_RESET,
	STR_GAME_SCORE,	
	STR_MENU3105_GAMES
} GAME_STR_IDS;


typedef enum {
	IMG_GAME = GAME_BASE+1,
	IMG_MENU3105_GAMES,
	IMG_GAME_SUBLCD_ICON
} GAME_IMG_IDS;


#define GAME_NOTIFYDURATION						ST_NOTIFYDURATION


#endif /* GAME_DEF_H */

