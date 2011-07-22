/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
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
 *	 Res_Game.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Game Resource
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
 
#include "stdc.h"
#include "customcfg.h"

#include "mmi_features.h"

#ifdef __MMI_GAME__

#include "funandgamesdefs.h"
#include "gamedefs.h"

#ifdef IS_F1RACE_BASE_GAME
/* under construction !*/
#endif /* IS_F1RACE_BASE_GAME */

#ifdef IS_HANOI_BASE_GAME
/* under construction !*/
#endif /* IS_HANOI_BASE_GAME */

#ifdef IS_RICHES_BASE_GAME
/* under construction !*/
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
#include "majungdefs.h"
#endif /* IS_MAJUNG_BASE_GAME */

#ifdef IS_MEMORY_BASE_GAME
/* under construction !*/
#endif /* IS_MEMORY_BASE_GAME */

#ifdef IS_COPTER_BASE_GAME
#include "copterdefs.h"
#endif /* IS_COPTER_BASE_GAME */
		 
#ifdef IS_PUZZLE_BASE_GAME
#include "puzzledefs.h"
#endif /* IS_PUZZLE_BASE_GAME */
/// WX
#ifdef IS_DFEX_BASE_GAME
#include "dfexdefs.h"
#endif /* IS_DFEX_BASE_GAME */
#ifdef __MMI_GAME_BQMET__
#include "bqmetdefs.h"
#endif


#ifdef IS_PANDA_BASE_GAME
#include "pandadefs.h"
#endif /* IS_PANDA_BASE_GAME */

#ifdef IS_STAIR_BASE_GAME
/* under construction !*/
#endif /* IS_STAIR_BASE_GAME */

#ifdef IS_UFO_BASE_GAME 
#include "ufodefs.h"
#endif /* IS_UFO_BASE_GAME */

#ifdef IS_DANCE_BASE_GAME
#include "dancedefs.h"
#endif /* IS_DANCE_BASE_GAME */

#ifdef IS_MAGICSUSHI_BASE_GAME
#include "magicsushidefs.h"
#endif /* IS_MAGICSUSHI_BASE_GAME */

#ifdef IS_FRUIT_BASE_GAME
/* under construction !*/
#endif /* IS_FRUIT_BASE_GAME */

#ifdef IS_VSMJ_BASE_GAME
/* under construction !*/
#endif /* IS_VSMJ_BASE_GAME */

#include "populateres.h"
#include "custresdef.h"


/************************************************************************/
/* Dance                																*/		
/************************************************************************/
#ifdef IS_DANCE_BASE_GAME
void PopulateDanceRes(void)
{
   /* Image */
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_GAME_ICON,		CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_icon.pbm", 			"Dance icon" );
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_BACKGROUND,		CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_display.gif", 		"background" );
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_LIVEDISPLAY,	CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_live_display.gif", "background" );
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_LIGHTDISPLAY,	CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_light_display.gif", "background" );
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_SCOREDISPLAY,	CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_score_display.gif", "background" );
   
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_COMPUTER_STANDBY,		CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_computer_standby.gif", 		"computer standby1" );
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_COMPUTER_RIGHT_UP,	CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_computer_right_up.gif", 		"computer right up" ); 
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_COMPUTER_RIGHT_DOWN,	CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_computer_right_down.gif", 	"computer right down" ); 
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_COMPUTER_LEFT_UP,		CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_computer_left_up.gif", 		"computer left up" ); 
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_COMPUTER_LEFT_DOWN,	CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_computer_left_down.gif", 	"computer left down" ); 

   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_USER_STANDBY,		CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_user_standby.gif", 	"user standby1" );
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_USER_RIGHT_UP,		CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_user_right_up.gif", 	"user right up" ); 
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_USER_RIGHT_DOWN,	CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_user_right_down.gif", "user right down" );
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_USER_LEFT_UP,		CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_user_left_up.gif", 	"user left up" ); 
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_USER_LEFT_DOWN,	CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_user_left_down.gif", 	"user left down" );    

   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_USER_LIVE,	CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_user_live.gif", "live" );

   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_GAME_OVER,	CUST_IMG_BASE_PATH"/GameImages/Dance/gameover/gx_dance_gameover.gif", 	"gameover" );
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_GRADESMAP,	CUST_IMG_BASE_PATH"/GameImages/Dance/gameover/gx_dance_gradesmap.gif",	"gameover" );
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_PIC,			CUST_IMG_BASE_PATH"/GameImages/Dance/gameover/gx_dance_pic.gif", 				"gameover " );
   
   
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_NUM_0,	CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_num_0.gif", "num 0" );
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_NUM_1,	CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_num_1.gif", "num 1" );
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_NUM_2,	CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_num_2.gif", "num 2" );
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_NUM_3,	CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_num_3.gif", "num 3" );
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_NUM_4,	CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_num_4.gif", "num 4" );
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_NUM_5,	CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_num_5.gif", "num 5" );
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_NUM_6,	CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_num_6.gif", "num 6" );
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_NUM_7,	CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_num_7.gif", "num 7" );
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_NUM_8,	CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_num_8.gif", "num 8" );
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_NUM_9,	CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_num_9.gif", "num 9" );
   
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_LIGHT_TRUE,		CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_light_true.gif", "light true" );
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_LIGHT_FALSE,	CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_light_false.gif", "light false" );
   ADD_APPLICATION_IMAGE2(IMG_ID_GX_DANCE_LIGHT_EMPTY,	CUST_IMG_BASE_PATH"/GameImages/Dance/gx_dance_light_empty.gif", "light empty" );

   /* String */
   ADD_APPLICATION_STRING2(STR_ID_GX_DANCE_GAME_NAME,			"Dance",		"Dance Game Name");
	#if defined(__MMI_TOUCH_SCREEN__)
   ADD_APPLICATION_STRING2(STR_ID_GX_DANCE_HELP_DESCRIPTION_TP,"Old man has to mimic child's actions. Click the corresponding place around the old man's limbs.",	"Dance help desc");   
	#else
   ADD_APPLICATION_STRING2(STR_ID_GX_DANCE_HELP_DESCRIPTION,"Old man has to mimic child's actions. Press key 1,3,7,9 to do actions.",	"Dance help desc");   
	#endif
}
#endif /* IS_DANCE_BASE_GAME */


#ifdef IS_DOLL_BASE_GAME
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
#endif /* IS_DOLL_BASE_GAME */

/************************************************************************/
/* F1 Race																*/		
/************************************************************************/
#ifdef __MMI_GAME_F1RACE__
void PopulateF1RaceRes(void)
{
	/* Create Strings */ 
	ADD_APPLICATION_STRING2(STR_GX_F1RACE_GAME_NAME,"F1 Race","Caption String");
	ADD_APPLICATION_STRING2(STR_GX_F1RACE_HELP_DESCRIPTION, "(1)Avoid to hit other cars.\n(2)Use button 2,4,6,8 to control the car and 5 to fly.\n(3)Run over a car will charge the fly power." ,"F1 race help");

	/* resources for F1Race game kernal */
	ADD_APPLICATION_IMAGE2(IMG_GX_F1RACE_GAME_ICON,				CUST_IMG_BASE_PATH"/GameImages/f1race/Race.BMP","Games image");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_PLAYER_CAR,				CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_PLAYER_CAR.GIF");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_PLAYER_CAR_FLY,			CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_PLAYER_CAR_FLY.GIF");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_PLAYER_CAR_FLY_DOWN,	CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_PLAYER_CAR_FLY_DOWN.GIF");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_PLAYER_CAR_FLY_UP,		CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_PLAYER_CAR_FLY_UP.GIF");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_PLAYER_CAR_CRASH,		CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_PLAYER_CAR_CRASH.GIF");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_OPPOSITE_CAR_0,			CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_OPPOSITE_CAR_0.GIF");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_OPPOSITE_CAR_1,			CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_OPPOSITE_CAR_1.GIF");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_OPPOSITE_CAR_2,			CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_OPPOSITE_CAR_2.GIF");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_OPPOSITE_CAR_3,			CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_OPPOSITE_CAR_3.GIF");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_OPPOSITE_CAR_4,			CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_OPPOSITE_CAR_4.GIF");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_OPPOSITE_CAR_5,			CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_OPPOSITE_CAR_5.GIF");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_OPPOSITE_CAR_6,			CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_OPPOSITE_CAR_6.GIF");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_PLAYER_CAR_HEAD_LIGHT,	CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_PLAYER_CAR_HEAD_LIGHT.GIF");

	/* status misc*/
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_STATUS_LOGO,	CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_LOGO.BMP");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_STATUS_SCORE,	CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_STATUS_SCORE.GIF");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_STATUS_LEVEL,	CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_STATUS_LEVEL.GIF");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_STATUS_BOX,		CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_STATUS_BOX.GIF");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_STATUS_FLY,		CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_STATUS_FLY.GIF");

	/* number */
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_NUMBER_0, CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_NUMBER_0.gif");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_NUMBER_1, CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_NUMBER_1.gif");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_NUMBER_2, CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_NUMBER_2.gif");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_NUMBER_3, CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_NUMBER_3.gif");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_NUMBER_4, CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_NUMBER_4.gif");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_NUMBER_5, CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_NUMBER_5.gif");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_NUMBER_6, CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_NUMBER_6.gif");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_NUMBER_7, CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_NUMBER_7.gif");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_NUMBER_8, CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_NUMBER_8.gif");
	ADD_APPLICATION_IMAGE(IMG_GX_F1RACE_NUMBER_9, CUST_IMG_BASE_PATH"/GameImages/f1race/GAME_F1RACE_NUMBER_9.gif");

	ADD_APPLICATION_IMAGE2(IMG_GX_F1RACE_GOTEXT,		CUST_IMG_BASE_PATH"/GameImages/f1race/gameover/gameover.bmp",	"Riches Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_F1RACE_GRADESMAP,		CUST_IMG_BASE_PATH"/GameImages/f1race/gameover/gradesmap.bmp",	"Riches Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_F1RACE_GOPIC,			CUST_IMG_BASE_PATH"/GameImages/f1race/gameover/pic.bmp",		"Riches Image");

}
#endif /* __MMI_GAME_F1RACE__ */



/************************************************************************/
/* Smash																*/	
/************************************************************************/
#ifdef __MMI_GAME_SMASH__
void PopulateSmashRes(void)
{
	/* Smash String */
	ADD_APPLICATION_STRING2(STR_GX_SMASH_GAME_NAME, "Smash","Smash Game Name");
	ADD_APPLICATION_STRING2(STR_GX_SMASH_TIME_SEC,	"sec","#NO_TRANS# String displayed in Time Bar for seconds");
	ADD_APPLICATION_STRING2(STR_GX_SMASH_NORMAL,	"Normal","String displayed in Level Normal");
	ADD_APPLICATION_STRING2(STR_GX_SMASH_SPEEDY,	"Speedy","String displayed in Level Speedy");
	ADD_APPLICATION_STRING2(STR_GX_SMASH_ACCURACY,	"Accuracy","String displayed in Level Accuracy");
	ADD_APPLICATION_STRING2(STR_GX_SMASH_HELP_DESCRIPTION,"The Smash contains 3 mini games.\n(1)Normal game. Hit the animals, but not the skeleton.\n(2)Speedy game. Hit as fast as you can.\n(3)Accuracy game. Hit the designate ones.","actual help string for smash game");

	/* Smash Images */

	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_GAME_ICON,	CUST_IMG_BASE_PATH"/GameImages/Smash/smash.BMP", "Smash Game Bg 0" );

	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_BG_0,		CUST_IMG_BASE_PATH"/GameImages/Smash/bg/bg_0.pbm", "Smash Game Bg 0" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_BG_1,		CUST_IMG_BASE_PATH"/GameImages/Smash/bg/bg_1.pbm", "Smash Game Bg 1" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_BG_2,		CUST_IMG_BASE_PATH"/GameImages/Smash/bg/bg_2.pbm", "Smash Game Bg 2" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_BG_3,		CUST_IMG_BASE_PATH"/GameImages/Smash/bg/bg_3.pbm", "Smash Game Bg 3" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_BG_4,		CUST_IMG_BASE_PATH"/GameImages/Smash/bg/bg_4.pbm", "Smash Game Bg 4" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_BG_5,		CUST_IMG_BASE_PATH"/GameImages/Smash/bg/bg_5.pbm", "Smash Game Bg 5" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_BG_6,		CUST_IMG_BASE_PATH"/GameImages/Smash/bg/bg_6.pbm", "Smash Game Bg 6" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_BG_7,		CUST_IMG_BASE_PATH"/GameImages/Smash/bg/bg_7.pbm", "Smash Game Bg 7" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_BG_8,		CUST_IMG_BASE_PATH"/GameImages/Smash/bg/bg_8.pbm", "Smash Game Bg 8" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_HITTHIS,	CUST_IMG_BASE_PATH"/GameImages/Smash/bg/hitthis.gif", "Hitthis" );


	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_MOUSE_0,	 CUST_IMG_BASE_PATH"/GameImages/Smash/mouse/m_01.pbm", "Smash Game Mouse" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_MOUSE_1,	 CUST_IMG_BASE_PATH"/GameImages/Smash/mouse/m_02.pbm", "Smash Game Mouse" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_MOUSE_2,	 CUST_IMG_BASE_PATH"/GameImages/Smash/mouse/m_03.pbm", "Smash Game Mouse" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_MOUSE_3,	 CUST_IMG_BASE_PATH"/GameImages/Smash/mouse/m_04.pbm", "Smash Game Mouse" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_MOUSE_4,	 CUST_IMG_BASE_PATH"/GameImages/Smash/mouse/m_05.pbm", "Smash Game Mouse" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_MOUSE_5,	 CUST_IMG_BASE_PATH"/GameImages/Smash/mouse/m_06.pbm", "Smash Game Mouse" );

	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_COW_0,		 CUST_IMG_BASE_PATH"/GameImages/Smash/cow/c_01.pbm", "Smash Game Cow" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_COW_1,		 CUST_IMG_BASE_PATH"/GameImages/Smash/cow/c_02.pbm", "Smash Game Cow" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_COW_2,		 CUST_IMG_BASE_PATH"/GameImages/Smash/cow/c_03.pbm", "Smash Game Cow" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_COW_3,		 CUST_IMG_BASE_PATH"/GameImages/Smash/cow/c_04.pbm", "Smash Game Cow" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_COW_4,		 CUST_IMG_BASE_PATH"/GameImages/Smash/cow/c_05.pbm", "Smash Game Cow" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_COW_5,		 CUST_IMG_BASE_PATH"/GameImages/Smash/cow/c_06.pbm", "Smash Game Cow" );

	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_BONE_0,		 CUST_IMG_BASE_PATH"/GameImages/Smash/bone/b_01.pbm", "Smash Game Bone" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_BONE_1,		 CUST_IMG_BASE_PATH"/GameImages/Smash/bone/b_02.pbm", "Smash Game Bone" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_BONE_2,		 CUST_IMG_BASE_PATH"/GameImages/Smash/bone/b_03.pbm", "Smash Game Bone" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_BONE_3,		 CUST_IMG_BASE_PATH"/GameImages/Smash/bone/b_04.pbm", "Smash Game Bone" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_BONE_4,		 CUST_IMG_BASE_PATH"/GameImages/Smash/bone/b_05.pbm", "Smash Game Bone" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_BONE_5,		 CUST_IMG_BASE_PATH"/GameImages/Smash/bone/b_06.pbm", "Smash Game Bone" );

	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_PIG_0,		 CUST_IMG_BASE_PATH"/GameImages/Smash/pig/p_01.pbm", "Smash Game pig" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_PIG_1,		 CUST_IMG_BASE_PATH"/GameImages/Smash/pig/p_02.pbm", "Smash Game pig" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_PIG_2,		 CUST_IMG_BASE_PATH"/GameImages/Smash/pig/p_03.pbm", "Smash Game pig" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_PIG_3,		 CUST_IMG_BASE_PATH"/GameImages/Smash/pig/p_04.pbm", "Smash Game pig" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_PIG_4,		 CUST_IMG_BASE_PATH"/GameImages/Smash/pig/p_05.pbm", "Smash Game pig" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_PIG_5,		 CUST_IMG_BASE_PATH"/GameImages/Smash/pig/p_06.pbm", "Smash Game pig" );

	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_HAND_0,		 CUST_IMG_BASE_PATH"/GameImages/Smash/hand/hand01.pbm", "Smash Game hand" );
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_HAND_1,		 CUST_IMG_BASE_PATH"/GameImages/Smash/hand/hand03.pbm", "Smash Game hand" );
	
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_GOTEXT,		CUST_IMG_BASE_PATH"/GameImages/Smash/gameover/gameover.bmp",	"Smash Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_GRADESMAP,	CUST_IMG_BASE_PATH"/GameImages/Smash/gameover/gradesmap.bmp",	"Smash Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_SMASH_GOPIC,		CUST_IMG_BASE_PATH"/GameImages/Smash/gameover/pic.bmp","Smash Image");
}	
#endif /* __MMI_GAME_SMASH__ */

	


/************************************************************************/
/* Riches																					*/	
/************************************************************************/
#ifdef IS_RICHES_BASE_GAME
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
/* under construction !*/
#endif /* IS_RICHES_BASE_GAME */



/************************************************************************/
/* Snake																						*/		
/************************************************************************/
#ifdef IS_SNAKE_BASE_GAME 
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
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /* IS_SNAKE_BASE_GAME */




/************************************************************************/
/* Tower of Hanoi																			*/	
/************************************************************************/
#ifdef IS_HANOI_BASE_GAME
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
#endif /* IS_HANOI_BASE_GAME */




/************************************************************************/
/* ColorBalls																				*/	
/************************************************************************/
#ifdef IS_COLORBALL_BASE_GAME
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
#endif /* IS_COLORBALL_BASE_GAME */

/************************************************************************/
/* Ninja																						*/	
/************************************************************************/
#ifdef IS_NINJA_BASE_GAME
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
#endif /* IS_NINJA_BASE_GAME */


/************************************************************************/
/* Majung 128x96,176x220,240x320 Game																	*/	
/************************************************************************/
#ifdef IS_MAJUNG_BASE_GAME

#ifndef __MMI_GAME_MAJUNG_240x320__
#define __MMI_GAME_MAJUNG_240x320__
#endif

void PopulateMajungRes(void)
{
/* set the base path */
#if defined(__MMI_GAME_MAJUNG_128x96__)
	#define MAJUNG_IMG_PATH	CUST_IMG_BASE_PATH"/GameImages/majung/majung_128x96_majung/"
#elif defined(__MMI_GAME_MAJUNG_176x220__)
	#define MAJUNG_IMG_PATH	CUST_IMG_BASE_PATH"/GameImages/majung/majung_176x220_majung/"
#elif defined(__MMI_GAME_MAJUNG_240x320__)
	#define MAJUNG_IMG_PATH	CUST_IMG_BASE_PATH"/GameImages/majung/majung_240x320_majung/"
#endif

	ADD_APPLICATION_STRING2(STR_GX_MAJUNG_GAME_NAME,		"Majung",	"Majung game name");
	ADD_APPLICATION_STRING2(STR_GX_MAJUNG_LEVEL_EASY,		"Easy",		"Majung easy");
	ADD_APPLICATION_STRING2(STR_GX_MAJUNG_LEVEL_NORMAL,	"Normal",	"Majung normal");
	ADD_APPLICATION_STRING2(STR_GX_MAJUNG_LEVEL_HARD,		"Hard",		"Majung hard");

	ADD_APPLICATION_STRING2(STR_GX_MAJUNG_HELP_DESCRIPTION,	"Select two bricks with same pattern. If two bricks can be connected by making less than two turns, they will disappear and user may gain extra time. Use arrow keys (or key 2,4,6,8) to move selection.\n(2)Use key 5 to select.","Majung Help Desc");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_GAME_ICON,			MAJUNG_IMG_PATH"majung_icon.pbm",	"Majung Image");

	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_1W,			MAJUNG_IMG_PATH"brick_1w.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_2W,			MAJUNG_IMG_PATH"brick_2w.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_3W,			MAJUNG_IMG_PATH"brick_3w.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_4W,			MAJUNG_IMG_PATH"brick_4w.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_5W,			MAJUNG_IMG_PATH"brick_5w.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_6W,			MAJUNG_IMG_PATH"brick_6w.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_7W,			MAJUNG_IMG_PATH"brick_7w.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_8W,			MAJUNG_IMG_PATH"brick_8w.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_9W,			MAJUNG_IMG_PATH"brick_9w.pbm",	"Majung Image");
	
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_2S,			MAJUNG_IMG_PATH"brick_2s.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_3S,			MAJUNG_IMG_PATH"brick_3s.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_4S,			MAJUNG_IMG_PATH"brick_4s.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_5S,			MAJUNG_IMG_PATH"brick_5s.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_6S,			MAJUNG_IMG_PATH"brick_6s.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_7S,			MAJUNG_IMG_PATH"brick_7s.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_8S,			MAJUNG_IMG_PATH"brick_8s.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_9S,			MAJUNG_IMG_PATH"brick_9s.pbm",	"Majung Image");
	
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_1T,			MAJUNG_IMG_PATH"brick_1t.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_2T,			MAJUNG_IMG_PATH"brick_2t.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_3T,			MAJUNG_IMG_PATH"brick_3t.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_4T,			MAJUNG_IMG_PATH"brick_4t.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_5T,			MAJUNG_IMG_PATH"brick_5t.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_6T,			MAJUNG_IMG_PATH"brick_6t.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_7T,			MAJUNG_IMG_PATH"brick_7t.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_8T,			MAJUNG_IMG_PATH"brick_8t.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_9T,			MAJUNG_IMG_PATH"brick_9t.pbm",	"Majung Image");

	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_A,			MAJUNG_IMG_PATH"brick_a.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_B,			MAJUNG_IMG_PATH"brick_b.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_C,			MAJUNG_IMG_PATH"brick_c.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_D,			MAJUNG_IMG_PATH"brick_d.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_F,			MAJUNG_IMG_PATH"brick_f.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_G,			MAJUNG_IMG_PATH"brick_g.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_H,			MAJUNG_IMG_PATH"brick_h.pbm",	"Majung Image");

	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_1W_SEL,		MAJUNG_IMG_PATH"brick_1w_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_2W_SEL,		MAJUNG_IMG_PATH"brick_2w_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_3W_SEL,		MAJUNG_IMG_PATH"brick_3w_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_4W_SEL,		MAJUNG_IMG_PATH"brick_4w_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_5W_SEL,		MAJUNG_IMG_PATH"brick_5w_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_6W_SEL,		MAJUNG_IMG_PATH"brick_6w_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_7W_SEL,		MAJUNG_IMG_PATH"brick_7w_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_8W_SEL,		MAJUNG_IMG_PATH"brick_8w_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_9W_SEL,		MAJUNG_IMG_PATH"brick_9w_sel.pbm",	"Majung Image");
	
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_2S_SEL,		MAJUNG_IMG_PATH"brick_2s_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_3S_SEL,		MAJUNG_IMG_PATH"brick_3s_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_4S_SEL,		MAJUNG_IMG_PATH"brick_4s_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_5S_SEL,		MAJUNG_IMG_PATH"brick_5s_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_6S_SEL,		MAJUNG_IMG_PATH"brick_6s_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_7S_SEL,		MAJUNG_IMG_PATH"brick_7s_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_8S_SEL,		MAJUNG_IMG_PATH"brick_8s_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_9S_SEL,		MAJUNG_IMG_PATH"brick_9s_sel.pbm",	"Majung Image");
	
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_1T_SEL,		MAJUNG_IMG_PATH"brick_1t_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_2T_SEL,		MAJUNG_IMG_PATH"brick_2t_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_3T_SEL,		MAJUNG_IMG_PATH"brick_3t_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_4T_SEL,		MAJUNG_IMG_PATH"brick_4t_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_5T_SEL,		MAJUNG_IMG_PATH"brick_5t_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_6T_SEL,		MAJUNG_IMG_PATH"brick_6t_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_7T_SEL,		MAJUNG_IMG_PATH"brick_7t_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_8T_SEL,		MAJUNG_IMG_PATH"brick_8t_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_9T_SEL,		MAJUNG_IMG_PATH"brick_9t_sel.pbm",	"Majung Image");

	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_A_SEL,		MAJUNG_IMG_PATH"brick_a_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_B_SEL,		MAJUNG_IMG_PATH"brick_b_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_C_SEL,		MAJUNG_IMG_PATH"brick_c_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_D_SEL,		MAJUNG_IMG_PATH"brick_d_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_F_SEL,		MAJUNG_IMG_PATH"brick_f_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_G_SEL,		MAJUNG_IMG_PATH"brick_g_sel.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BRICK_H_SEL,		MAJUNG_IMG_PATH"brick_h_sel.pbm",	"Majung Image");

	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_CONNECT_HOR,		MAJUNG_IMG_PATH"connect_hor.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_CONNECT_VER,		MAJUNG_IMG_PATH"connect_ver.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_CONNECT_LU,		MAJUNG_IMG_PATH"connect_lu.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_CONNECT_RU,		MAJUNG_IMG_PATH"connect_ru.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_CONNECT_LD,		MAJUNG_IMG_PATH"connect_ld.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_CONNECT_RD,		MAJUNG_IMG_PATH"connect_rd.pbm",	"Majung Image");

	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_THICK,				MAJUNG_IMG_PATH"thick.pbm",	"Majung Image");

	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_FILL_1,		MAJUNG_IMG_PATH"bar_fill_01.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_FILL_2,		MAJUNG_IMG_PATH"bar_fill_02.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_FILL_3,		MAJUNG_IMG_PATH"bar_fill_03.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_FILL_4,		MAJUNG_IMG_PATH"bar_fill_04.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_FILL_5,		MAJUNG_IMG_PATH"bar_fill_05.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_FILL_6,		MAJUNG_IMG_PATH"bar_fill_06.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_FILL_7,		MAJUNG_IMG_PATH"bar_fill_07.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_FILL_8,		MAJUNG_IMG_PATH"bar_fill_08.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_FILL_9,		MAJUNG_IMG_PATH"bar_fill_09.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_FILL_10,		MAJUNG_IMG_PATH"bar_fill_10.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_FILL_11,		MAJUNG_IMG_PATH"bar_fill_11.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_FILL_12,		MAJUNG_IMG_PATH"bar_fill_12.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_FILL_13,		MAJUNG_IMG_PATH"bar_fill_13.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_FILL_14,		MAJUNG_IMG_PATH"bar_fill_14.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_FILL_15,		MAJUNG_IMG_PATH"bar_fill_15.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_FILL_16,		MAJUNG_IMG_PATH"bar_fill_16.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_FILL_17,		MAJUNG_IMG_PATH"bar_fill_17.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_FILL_18,		MAJUNG_IMG_PATH"bar_fill_18.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_FILL_19,		MAJUNG_IMG_PATH"bar_fill_19.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_FILL_20,		MAJUNG_IMG_PATH"bar_fill_20.pbm",	"Majung Image");

	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_EMPTY_1,		MAJUNG_IMG_PATH"bar_empty_01.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_EMPTY_2,		MAJUNG_IMG_PATH"bar_empty_02.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_EMPTY_3,		MAJUNG_IMG_PATH"bar_empty_03.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_EMPTY_4,		MAJUNG_IMG_PATH"bar_empty_04.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_EMPTY_5,		MAJUNG_IMG_PATH"bar_empty_05.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_EMPTY_6,		MAJUNG_IMG_PATH"bar_empty_06.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_EMPTY_7,		MAJUNG_IMG_PATH"bar_empty_07.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_EMPTY_8,		MAJUNG_IMG_PATH"bar_empty_08.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_EMPTY_9,		MAJUNG_IMG_PATH"bar_empty_09.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_EMPTY_10,		MAJUNG_IMG_PATH"bar_empty_10.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_EMPTY_11,		MAJUNG_IMG_PATH"bar_empty_11.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_EMPTY_12,		MAJUNG_IMG_PATH"bar_empty_12.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_EMPTY_13,		MAJUNG_IMG_PATH"bar_empty_13.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_EMPTY_14,		MAJUNG_IMG_PATH"bar_empty_14.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_EMPTY_15,		MAJUNG_IMG_PATH"bar_empty_15.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_EMPTY_16,		MAJUNG_IMG_PATH"bar_empty_16.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_EMPTY_17,		MAJUNG_IMG_PATH"bar_empty_17.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_EMPTY_18,		MAJUNG_IMG_PATH"bar_empty_18.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_EMPTY_19,		MAJUNG_IMG_PATH"bar_empty_19.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BAR_EMPTY_20,		MAJUNG_IMG_PATH"bar_empty_20.pbm",	"Majung Image");
	
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_GAMEOVER,			MAJUNG_IMG_PATH"gameover.pbm",		"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_GRADESMAP,			MAJUNG_IMG_PATH"gradesmap.pbm",		"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_COUNT,				MAJUNG_IMG_PATH"count.pbm",			"Majung Image");

	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_1,					MAJUNG_IMG_PATH"MAJUNG_NUMBER_1.gif",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_2,					MAJUNG_IMG_PATH"MAJUNG_NUMBER_2.gif",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_3,					MAJUNG_IMG_PATH"MAJUNG_NUMBER_3.gif",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_4,					MAJUNG_IMG_PATH"MAJUNG_NUMBER_4.gif",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_5,					MAJUNG_IMG_PATH"MAJUNG_NUMBER_5.gif",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_6,					MAJUNG_IMG_PATH"MAJUNG_NUMBER_6.gif",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_7,					MAJUNG_IMG_PATH"MAJUNG_NUMBER_7.gif",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_8,					MAJUNG_IMG_PATH"MAJUNG_NUMBER_8.gif",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_9,					MAJUNG_IMG_PATH"MAJUNG_NUMBER_9.gif",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_0,					MAJUNG_IMG_PATH"MAJUNG_NUMBER_0.gif",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_BOX,					MAJUNG_IMG_PATH"MAJUNG_box.gif",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_EXIT_UP,			MAJUNG_IMG_PATH"MAJUNG_exit_up.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_EXIT_DOWN,			MAJUNG_IMG_PATH"MAJUNG_exit_down.pbm",	"Majung Image");
	ADD_APPLICATION_IMAGE2(IMG_GX_MAJUNG_TIME,			    MAJUNG_IMG_PATH"majung_time.pbm",	"Majung Time");

}
#endif /* IS_MAJUNG_BASE_GAME */




/************************************************************************/
/* Mmemory Game																			*/	
/************************************************************************/
#ifdef IS_MEMORY_BASE_GAME
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
#endif /* IS_MEMORY_BASE_GAME */



/************************************************************************/
/* Copter	128x160																		*/	
/************************************************************************/
#ifdef IS_COPTER_BASE_GAME
void PopulateCopterRes(void)
{
	/* set the base path */
#if defined(__MMI_GAME_COPTER_128x128__)
	#define COPTER_IMG_PATH	CUST_IMG_BASE_PATH"/GameImages/copter/copter_128x128_copter/"
#elif defined(__MMI_GAME_COPTER_128x160__)
	#define COPTER_IMG_PATH	CUST_IMG_BASE_PATH"/GameImages/copter/copter_128x160_copter/"
#elif defined(__MMI_GAME_COPTER_176x220__)
	#define COPTER_IMG_PATH	CUST_IMG_BASE_PATH"/GameImages/copter/copter_176x220_copter/"

#elif defined(__MMI_GAME_SUBMARINE_128x128__)
	#define COPTER_IMG_PATH	CUST_IMG_BASE_PATH"/GameImages/copter/copter_128x128_submarine/"
#elif defined(__MMI_GAME_SUBMARINE_128x160__)
	#define COPTER_IMG_PATH	CUST_IMG_BASE_PATH"/GameImages/copter/copter_128x160_submarine/"
#elif defined(__MMI_GAME_SUBMARINE_176x220__)
	#define COPTER_IMG_PATH	CUST_IMG_BASE_PATH"/GameImages/copter/copter_176x220_submarine/"

#elif defined(__MMI_GAME_JET_128x128__)
	#define COPTER_IMG_PATH	CUST_IMG_BASE_PATH"/GameImages/copter/copter_128x128_jet/"
#elif defined(__MMI_GAME_JET_128x160__)
	#define COPTER_IMG_PATH	CUST_IMG_BASE_PATH"/GameImages/copter/copter_128x160_jet/"
#elif defined(__MMI_GAME_JET_176x220__)
	#define COPTER_IMG_PATH	CUST_IMG_BASE_PATH"/GameImages/copter/copter_176x220_jet/"
#endif

	/* set game strings */
#if defined(__MMI_GAME_COPTER_128x128__) || defined(__MMI_GAME_COPTER_128x160__) || defined(__MMI_GAME_COPTER_176x220__)
	ADD_APPLICATION_STRING2(STR_ID_GX_COPTER_GAME_NAME,			"Copter",		"Copter Game Name");
	#if defined(__MMI_TOUCH_SCREEN__)
	ADD_APPLICATION_STRING2(STR_ID_GX_COPTER_HELP_DESCRIPTION_TP,	"Drive the copter and prevent it from crashing into the obstacle.\n(1)Use stylus to click the screen to make it go upward.",	"Copter Help Description");
	#else
	ADD_APPLICATION_STRING2(STR_ID_GX_COPTER_HELP_DESCRIPTION,	"Drive the copter and prevent it from crashing into the obstacle.\n(1)Use key 5 to make it go upward.",	"Copter Help Description");
	#endif
#elif defined(__MMI_GAME_SUBMARINE_128x128__) || defined(__MMI_GAME_SUBMARINE_128x160__) || defined(__MMI_GAME_SUBMARINE_176x220__)
	ADD_APPLICATION_STRING2(STR_ID_GX_COPTER_GAME_NAME_SUBMARINE,			"Submarine",		"Submarine Game Name");
	#if defined(__MMI_TOUCH_SCREEN__)
	ADD_APPLICATION_STRING2(STR_ID_GX_COPTER_HELP_DESCRIPTION_SUBMARINE_TP,	"Drive the submarine and prevent it from crashing into the obstacle.\n(1)Use stylus to click the screen to make it go upward.",	"Submarine Help Description");
	#else
	ADD_APPLICATION_STRING2(STR_ID_GX_COPTER_HELP_DESCRIPTION_SUBMARINE,	"Drive the submarine and prevent it from crashing into the obstacle.\n(1)Use key 5 to make it go upward.",	"Submarine Help Description");
	#endif
#elif defined(__MMI_GAME_JET_128x128__) || defined(__MMI_GAME_JET_128x160__) || defined(__MMI_GAME_JET_176x220__)
	ADD_APPLICATION_STRING2(STR_ID_GX_COPTER_GAME_NAME_JET,			"Jet",		"Jet Game Name");
	#if defined(__MMI_TOUCH_SCREEN__)
	ADD_APPLICATION_STRING2(STR_ID_GX_COPTER_HELP_DESCRIPTION_JET_TP,	"Drive the jet and prevent it from crashing into the obstacle.\n(1)Use stylus to click the screen to make it go upward.",	"Jet Help Description");
	#else
	ADD_APPLICATION_STRING2(STR_ID_GX_COPTER_HELP_DESCRIPTION_JET,	"Drive the jet and prevent it from crashing into the obstacle.\n(1)Use key 5 to make it go upward.",	"Jet Help Description");
	#endif
#endif

	/* set game images */
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_GAME_ICON,		COPTER_IMG_PATH"gx_copter_copter_icon.pbm",				"Copter Image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_OBSTACLE,		COPTER_IMG_PATH"gx_copter_obstacle.pbm",					"Obstacle Image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_NUMBER_0,		COPTER_IMG_PATH"gx_copter_number_0.gif",					"Image for 0");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_NUMBER_1,		COPTER_IMG_PATH"gx_copter_number_1.gif",					"Image for 1");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_NUMBER_2,		COPTER_IMG_PATH"gx_copter_number_2.gif",					"Image for 2");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_NUMBER_3,		COPTER_IMG_PATH"gx_copter_number_3.gif",					"Image for 3");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_NUMBER_4,		COPTER_IMG_PATH"gx_copter_number_4.gif",					"Image for 4");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_NUMBER_5,		COPTER_IMG_PATH"gx_copter_number_5.gif",					"Image for 5");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_NUMBER_6,		COPTER_IMG_PATH"gx_copter_number_6.gif",					"Image for 6");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_NUMBER_7,		COPTER_IMG_PATH"gx_copter_number_7.gif",					"Image for 7");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_NUMBER_8,		COPTER_IMG_PATH"gx_copter_number_8.gif",					"Image for 8");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_NUMBER_9,		COPTER_IMG_PATH"gx_copter_number_9.gif",					"Image for 9");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_BOX,				COPTER_IMG_PATH"gx_copter_box.gif",							"Image for box");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_SCORE,			COPTER_IMG_PATH"gx_copter_number_score.gif",				"Image for alphabetscore");

	#if defined(__MMI_GAME_COPTER_128x160__) || defined(__MMI_GAME_COPTER_176x220__) || defined(__MMI_GAME_SUBMARINE_128x160__) || defined(__MMI_GAME_SUBMARINE_176x220__) || defined(__MMI_GAME_JET_128x160__) || defined(__MMI_GAME_JET_176x220__)
		ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_TITLE,			COPTER_IMG_PATH"gx_copter_title.bmp",					"title image");
	#endif

	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_BACKGROUND,	COPTER_IMG_PATH"gx_copter_bg.pbm",							"copter background image");

	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_0,				COPTER_IMG_PATH"gx_copter_frame_0.pbm",					"copter image 0");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_1,				COPTER_IMG_PATH"gx_copter_frame_1.pbm",					"copter image 1");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_2,				COPTER_IMG_PATH"gx_copter_frame_2.pbm",					"copter image 2");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_CRASHED_0,		COPTER_IMG_PATH"gx_copter_crashed_0.pbm",					"crashed copter image 0");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_CRASHED_1,		COPTER_IMG_PATH"gx_copter_crashed_1.pbm",					"crashed copter image 1");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_CRASHED_2,		COPTER_IMG_PATH"gx_copter_crashed_2.pbm",					"crashed copter image 2");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_GOTEXT,			COPTER_IMG_PATH"gameover/gx_copter_gameover.gif",	"game over text");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_GRADESMAP,		COPTER_IMG_PATH"gameover/gx_copter_gradesmap.gif",	"grade background");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_COPTER_GOPIC,			COPTER_IMG_PATH"gameover/gx_copter_pic.gif",			"game over image");
}
#endif /* IS_COPTER_BASE_GAME */


/************************************************************************/
/* Puzzle	128x160  																	*/	
/************************************************************************/
#ifdef IS_PUZZLE_BASE_GAME
void PopulatePuzzleRes(void)
{
	/* set the base path */
#if defined(__MMI_GAME_PUZZLE_128x128__)
	#define PUZZLE_IMG_PATH	CUST_IMG_BASE_PATH"/GameImages/puzzle/puzzle_128x128_puzzle/"
#elif defined(__MMI_GAME_PUZZLE_128x160__)
	#define PUZZLE_IMG_PATH	CUST_IMG_BASE_PATH"/GameImages/puzzle/puzzle_128x160_puzzle/"
#elif defined(__MMI_GAME_PUZZLE_176x220__)
	#define PUZZLE_IMG_PATH	CUST_IMG_BASE_PATH"/GameImages/puzzle/puzzle_176x220_puzzle/"
#elif defined(__MMI_GAME_PUZZLE_220x176__)
	#define PUZZLE_IMG_PATH	CUST_IMG_BASE_PATH"/GameImages/puzzle/puzzle_220x176_puzzle/"	
#elif  defined(__MMI_GAME_PUZZLE_240x320__)	
	#define PUZZLE_IMG_PATH	CUST_IMG_BASE_PATH"/GameImages/puzzle/puzzle_240x320_puzzle/"
#endif

	/* set game strings */
	ADD_APPLICATION_STRING2(STR_ID_GX_PUZZLE_GAME_NAME,			         "Puzzle",		"Puzzle Game Name");
	
	#if defined(__MMI_TOUCH_SCREEN__)
		ADD_APPLICATION_STRING2(STR_ID_GX_PUZZLE_HELP_DESCRIPTION_TP,			"Move the small pictures to make it become the original image.\n(1)touch the small picture around the blank block to make it move toward to the blank block.",	"Puzzle Help Description");
	#elif defined(__MMI_GAME_PUZZLE_128x128__)
		ADD_APPLICATION_STRING2(STR_ID_GX_PUZZLE_HELP_DESCRIPTION_128,			"Move the small pictures to make it become the original image.\n(1)Use key 2(or up-arrow key) to make a picture go upward, key 4(or left-arrow key) to make a picture go leftward, key 6(or right-arrow key) to make a picture go rightward, and key 8(or down-arrow key) to make a picture go downward.\n(2)Press key 5 to see the original image and the gameplay information.",	"Puzzle Help Description");
	#else
		ADD_APPLICATION_STRING2(STR_ID_GX_PUZZLE_HELP_DESCRIPTION,				"Move the small pictures to make it become the original image.\n(1)Use key 2(or up-arrow key) to make a picture go upward, key 4(or left-arrow key) to make a picture go leftward, key 6(or right-arrow key) to make a picture go rightward, and key 8(or down-arrow key) to make a picture go downward.",	"Puzzle Help Description");
	#endif 

	ADD_APPLICATION_STRING2(STR_ID_GX_PUZZLE_LEVEL_EASY,		"Easy", "Menu string");
	ADD_APPLICATION_STRING2(STR_ID_GX_PUZZLE_LEVEL_NORMAL,	"Normal", "Menu string");
	ADD_APPLICATION_STRING2(STR_ID_GX_PUZZLE_LEVEL_HARD,		"Hard", "Menu string");

	/* set game images */
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_GAME_ICON,		PUZZLE_IMG_PATH"gx_puzzle_icon.bmp",					"Puzzle icon image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_NUMBER_0,		PUZZLE_IMG_PATH"gx_puzzle_number_0.gif",				"Image for 0");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_NUMBER_1,		PUZZLE_IMG_PATH"gx_puzzle_number_1.gif",				"Image for 1");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_NUMBER_2,		PUZZLE_IMG_PATH"gx_puzzle_number_2.gif",				"Image for 2");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_NUMBER_3,		PUZZLE_IMG_PATH"gx_puzzle_number_3.gif",				"Image for 3");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_NUMBER_4,		PUZZLE_IMG_PATH"gx_puzzle_number_4.gif",				"Image for 4");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_NUMBER_5,		PUZZLE_IMG_PATH"gx_puzzle_number_5.gif",				"Image for 5");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_NUMBER_6,		PUZZLE_IMG_PATH"gx_puzzle_number_6.gif",				"Image for 6");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_NUMBER_7,		PUZZLE_IMG_PATH"gx_puzzle_number_7.gif",				"Image for 7");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_NUMBER_8,		PUZZLE_IMG_PATH"gx_puzzle_number_8.gif",				"Image for 8");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_NUMBER_9,		PUZZLE_IMG_PATH"gx_puzzle_number_9.gif",				"Image for 9");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_BOX,				PUZZLE_IMG_PATH"gx_puzzle_box.gif",						"Image for box");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_STEP,			PUZZLE_IMG_PATH"gx_puzzle_step.gif",					"Image for alphabet: STEP");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_TIME,			PUZZLE_IMG_PATH"gx_puzzle_time.gif",					"Image for alphabet: TIME");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_LEFT_BAR,		PUZZLE_IMG_PATH"gx_puzzle_left_bar.gif",				"Image for left border");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_RIGHT_BAR,		PUZZLE_IMG_PATH"gx_puzzle_right_bar.gif",				"Image for right border");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_TOP_BAR,		PUZZLE_IMG_PATH"gx_puzzle_top_bar.gif",				"Image for top border");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_BOTTOM_BAR,	PUZZLE_IMG_PATH"gx_puzzle_bottom_bar.gif",			"Image for bottom border");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_3_SPACE,		PUZZLE_IMG_PATH"gx_puzzle_3_space.gif",				"Space Image of 3x3 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_4_SPACE,		PUZZLE_IMG_PATH"gx_puzzle_4_space.gif",				"Space Image of 4x4 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_SPACE,		PUZZLE_IMG_PATH"gx_puzzle_5_space.gif",				"Space Image of 5x5 puzzle");

	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_SMALL_3,		PUZZLE_IMG_PATH"gx_puzzle_small_3.gif",				"Small image for original image 3x3");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_SMALL_4,		PUZZLE_IMG_PATH"gx_puzzle_small_4.gif",				"Small image for original image 4x4");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_SMALL_5,		PUZZLE_IMG_PATH"gx_puzzle_small_5.gif",				"Small image for original image 5x5");

	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_3_1,				PUZZLE_IMG_PATH"gx_puzzle_3_1.gif",						"Image 1 of 3x3 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_3_2,				PUZZLE_IMG_PATH"gx_puzzle_3_2.gif",						"Image 2 of 3x3 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_3_3,				PUZZLE_IMG_PATH"gx_puzzle_3_3.gif",						"Image 3 of 3x3 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_3_4,				PUZZLE_IMG_PATH"gx_puzzle_3_4.gif",						"Image 4 of 3x3 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_3_5,				PUZZLE_IMG_PATH"gx_puzzle_3_5.gif",						"Image 5 of 3x3 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_3_6,				PUZZLE_IMG_PATH"gx_puzzle_3_6.gif",						"Image 6 of 3x3 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_3_7,				PUZZLE_IMG_PATH"gx_puzzle_3_7.gif",						"Image 7 of 3x3 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_3_8,				PUZZLE_IMG_PATH"gx_puzzle_3_8.gif",						"Image 8 of 3x3 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_4_1,				PUZZLE_IMG_PATH"gx_puzzle_4_1.gif",						"Image 1 of 4x4 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_4_2,				PUZZLE_IMG_PATH"gx_puzzle_4_2.gif",						"Image 2 of 4x4 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_4_3,				PUZZLE_IMG_PATH"gx_puzzle_4_3.gif",						"Image 3 of 4x4 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_4_4,				PUZZLE_IMG_PATH"gx_puzzle_4_4.gif",						"Image 4 of 4x4 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_4_5,				PUZZLE_IMG_PATH"gx_puzzle_4_5.gif",						"Image 5 of 4x4 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_4_6,				PUZZLE_IMG_PATH"gx_puzzle_4_6.gif",						"Image 6 of 4x4 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_4_7,				PUZZLE_IMG_PATH"gx_puzzle_4_7.gif",						"Image 7 of 4x4 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_4_8,				PUZZLE_IMG_PATH"gx_puzzle_4_8.gif",						"Image 8 of 4x4 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_4_9,				PUZZLE_IMG_PATH"gx_puzzle_4_9.gif",						"Image 9 of 4x4 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_4_10,			PUZZLE_IMG_PATH"gx_puzzle_4_10.gif",					"Image 10 of 4x4 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_4_11,			PUZZLE_IMG_PATH"gx_puzzle_4_11.gif",					"Image 11 of 4x4 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_4_12,			PUZZLE_IMG_PATH"gx_puzzle_4_12.gif",					"Image 12 of 4x4 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_4_13,			PUZZLE_IMG_PATH"gx_puzzle_4_13.gif",					"Image 13 of 4x4 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_4_14,			PUZZLE_IMG_PATH"gx_puzzle_4_14.gif",					"Image 14 of 4x4 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_4_15,			PUZZLE_IMG_PATH"gx_puzzle_4_15.gif",					"Image 15 of 4x4 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_1,				PUZZLE_IMG_PATH"gx_puzzle_5_1.gif",						"Image 1 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_2,				PUZZLE_IMG_PATH"gx_puzzle_5_2.gif",						"Image 2 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_3,				PUZZLE_IMG_PATH"gx_puzzle_5_3.gif",						"Image 3 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_4,				PUZZLE_IMG_PATH"gx_puzzle_5_4.gif",						"Image 4 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_5,				PUZZLE_IMG_PATH"gx_puzzle_5_5.gif",						"Image 5 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_6,				PUZZLE_IMG_PATH"gx_puzzle_5_6.gif",						"Image 6 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_7,				PUZZLE_IMG_PATH"gx_puzzle_5_7.gif",						"Image 7 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_8,				PUZZLE_IMG_PATH"gx_puzzle_5_8.gif",						"Image 8 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_9,				PUZZLE_IMG_PATH"gx_puzzle_5_9.gif",						"Image 9 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_10,			PUZZLE_IMG_PATH"gx_puzzle_5_10.gif",					"Image 10 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_11,			PUZZLE_IMG_PATH"gx_puzzle_5_11.gif",					"Image 11 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_12,			PUZZLE_IMG_PATH"gx_puzzle_5_12.gif",					"Image 12 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_13,			PUZZLE_IMG_PATH"gx_puzzle_5_13.gif",					"Image 13 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_14,			PUZZLE_IMG_PATH"gx_puzzle_5_14.gif",					"Image 14 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_15,			PUZZLE_IMG_PATH"gx_puzzle_5_15.gif",					"Image 15 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_16,			PUZZLE_IMG_PATH"gx_puzzle_5_16.gif",					"Image 16 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_17,			PUZZLE_IMG_PATH"gx_puzzle_5_17.gif",					"Image 17 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_18,			PUZZLE_IMG_PATH"gx_puzzle_5_18.gif",					"Image 18 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_19,			PUZZLE_IMG_PATH"gx_puzzle_5_19.gif",					"Image 19 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_20,			PUZZLE_IMG_PATH"gx_puzzle_5_20.gif",					"Image 20 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_21,			PUZZLE_IMG_PATH"gx_puzzle_5_21.gif",					"Image 21 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_22,			PUZZLE_IMG_PATH"gx_puzzle_5_22.gif",					"Image 22 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_23,			PUZZLE_IMG_PATH"gx_puzzle_5_23.gif",					"Image 23 of 5x5 puzzle");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_5_24,			PUZZLE_IMG_PATH"gx_puzzle_5_24.gif",					"Image 24 of 5x5 puzzle");
	
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_GOTEXT,			PUZZLE_IMG_PATH"gameover/gx_puzzle_gameover.gif","game over text");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_GRADESMAP,		PUZZLE_IMG_PATH"gameover/gx_puzzle_gradesmap.gif","grade background");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_GOPIC,			PUZZLE_IMG_PATH"gameover/gx_puzzle_pic.gif",		"game over image");
	#if  defined(__MMI_TOUCH_SCREEN__)&& defined(__MMI_GAME_PUZZLE_240x320__)	
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_UP,			PUZZLE_IMG_PATH"gx_puzzle_up.pbm","Tower Bar Image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PUZZLE_DOWN,		PUZZLE_IMG_PATH"gx_puzzle_down.pbm","Tower Bar Image");
	#endif
}
#endif /* IS_PUZZLE_BASE_GAME */

/// WX add
/***********************************************************************/
/* BQMET game 
//
/***********************************************************************/
#ifdef __MMI_GAME_BQMET__
void PopulateBqmetRes(void)
{
   	/*mj game*/
	#ifdef __GAME_BQMET_MJ__
	ADD_APPLICATION_STRING2(STR_GX_MJ_GAME_NAME,"MJ","Title displayed in Games Menu");
	ADD_APPLICATION_STRING2(STR_GX_MJ_HELP_DESCRIPTION,"2/up key:shift cursor up\n4/left key:shift cursor left\n6/right key:shift cursor right\n8/down key:shift cursor down\nleft soft key/ok key: select\nright soft key:back\n1:a hint\n2:reset the cards\n5:pause", "String for Help In Hanio Game");
	#define BQMET_MJ_IMG_PATH	CUST_IMG_BASE_PATH"/GameImages/bqgame/mj/"
	
	ADD_APPLICATION_IMAGE2(IMG_GX_MJ_GAME_ICON,	CUST_IMG_BASE_PATH"/GameImages/bqgame/mj.bmp","Games image");
	
	ADD_APPLICATION_IMAGE2(IMG_GX_MJ_GAME_1,CUST_IMG_BASE_PATH"/GameImages/bqgame/mj/logo.jpg","");
	ADD_APPLICATION_IMAGE2(IMG_GX_MJ_GAME_6,CUST_IMG_BASE_PATH"/GameImages/bqgame/mj/mj_dice.gif","");
	ADD_APPLICATION_IMAGE2(IMG_GX_MJ_GAME_28,CUST_IMG_BASE_PATH"/GameImages/bqgame/mj/Bg1.gif","");
	ADD_APPLICATION_IMAGE2(IMG_GX_MJ_GAME_30,CUST_IMG_BASE_PATH"/GameImages/bqgame/mj/main.gif","");
	ADD_APPLICATION_IMAGE2(IMG_GX_MJ_GAME_31,CUST_IMG_BASE_PATH"/GameImages/bqgame/mj/menu.gif","");
	ADD_APPLICATION_IMAGE2(IMG_GX_MJ_GAME_33,CUST_IMG_BASE_PATH"/GameImages/bqgame/mj/item0.gif","");
	ADD_APPLICATION_IMAGE2(IMG_GX_MJ_GAME_34,CUST_IMG_BASE_PATH"/GameImages/bqgame/mj/item1.gif","");
	ADD_APPLICATION_IMAGE2(IMG_GX_MJ_GAME_35,CUST_IMG_BASE_PATH"/GameImages/bqgame/mj/item2.gif","");
	ADD_APPLICATION_IMAGE2(IMG_GX_MJ_GAME_36,CUST_IMG_BASE_PATH"/GameImages/bqgame/mj/item3.gif","");
	ADD_APPLICATION_IMAGE2(IMG_GX_MJ_GAME_37,CUST_IMG_BASE_PATH"/GameImages/bqgame/mj/item4.gif","");
	ADD_APPLICATION_IMAGE2(IMG_GX_MJ_GAME_38,CUST_IMG_BASE_PATH"/GameImages/bqgame/mj/index.gif","");
	#endif
}
#endif	
//WX add
#ifdef IS_DFEX_BASE_GAME
void PopulateDFexRes(void)
{
	#define DFEX_BASE_PATH	CUST_IMG_BASE_PATH"/GameImages/DFex/"
#if defined(__MMI_GAME_DFEX_240x320__)	
	#define DFEX_IMG_PATH	CUST_IMG_BASE_PATH"/GameImages/DFex/dfex240x320/"
#endif

	/* set game strings */
	ADD_APPLICATION_STRING2(STR_ID_GX_DFEX_GAME_NAME,			         "Dragon Force ex",		"DFEX Game Name");
	
	#if defined(__MMI_TOUCH_SCREEN__)
		ADD_APPLICATION_STRING2(STR_ID_GX_DFEX_HELP_DESCRIPTION_TP,			"You can contral the plan by arrow key. Press the comfirm key to use the item that collected",	"DFEX Help Description");
	#else
		ADD_APPLICATION_STRING2(STR_ID_GX_DFEX_HELP_DESCRIPTION,				"You can contral the plan by arrow key. Press the comfirm key to use the item that collected",	"DFEX Help Description");
	#endif 

	/* set game images */
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_ICON,		DFEX_IMG_PATH"ico.bmp",					"icon image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EXPLODE1,	       DFEX_IMG_PATH"explode1.gif", ""); 
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EXPLODE2,	       DFEX_IMG_PATH"explode2.gif", ""); 
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EXPLODE3,	       DFEX_IMG_PATH"explode3.gif", ""); 
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EXPLODE4,	       DFEX_IMG_PATH"explode4.gif", ""); 
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EXPLODE5,	       DFEX_IMG_PATH"explode5.gif", ""); 
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EXPLODE6,	       DFEX_IMG_PATH"explode6.gif", ""); 
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EXPLODE7,	       DFEX_IMG_PATH"explode7.gif", ""); 
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EXPLODE8,	       DFEX_IMG_PATH"explode8.gif", ""); 
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EXPLODE9,	       DFEX_IMG_PATH"explode9.gif", ""); 
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_FAILD,              DFEX_IMG_PATH"faild.gif", ""); 
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_LEI,                DFEX_IMG_PATH"lei.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_PLANE3,             DFEX_IMG_PATH"plane3.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_PLANE41,             DFEX_IMG_PATH"plane41.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_PLANE42,             DFEX_IMG_PATH"plane42.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_PLANE43,             DFEX_IMG_PATH"plane43.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_PLANE5,             DFEX_IMG_PATH"plane5.gif", ""); 
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_BOSS11,              DFEX_IMG_PATH"boss11.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_BOSS12,              DFEX_IMG_PATH"boss12.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_BOSS21,              DFEX_IMG_PATH"boss21.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_BOSS22,              DFEX_IMG_PATH"boss22.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_BOSS31,              DFEX_IMG_PATH"boss31.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_BOSS32,              DFEX_IMG_PATH"boss32.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_BRICK,              DFEX_IMG_PATH"brick.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_BULLETS1,            DFEX_IMG_PATH"bullets1.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_BULLETS2,            DFEX_IMG_PATH"bullets2.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_BULLETS3,            DFEX_IMG_PATH"bullets3.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_BULLETS4,            DFEX_IMG_PATH"bullets4.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_BULLETS5,            DFEX_IMG_PATH"bullets5.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_BULLETS6,            DFEX_IMG_PATH"bullets6.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_SPPOWER,            DFEX_IMG_PATH"spPower.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_SPSPARK1,            DFEX_IMG_PATH"spSpark1.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_SPSPARK2,            DFEX_IMG_PATH"spSpark2.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_SPSPARK3,            DFEX_IMG_PATH"spSpark3.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_SPTABLE,            DFEX_IMG_PATH"spTable.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_THRUSTER1,          DFEX_IMG_PATH"thruster1.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_THRUSTER2,          DFEX_IMG_PATH"thruster2.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_LOGOMET,            DFEX_IMG_PATH"title.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_MAIN,               DFEX_IMG_PATH"main.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_MAINBG,             DFEX_IMG_PATH"mainbg.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_MENU,               DFEX_IMG_PATH"menu.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_MP1,                 DFEX_IMG_PATH"mp1.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_MP2,                 DFEX_IMG_PATH"mp2.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_MP3,                 DFEX_IMG_PATH"mp3.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_MYB,                DFEX_IMG_PATH"myb.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_NUM0,                DFEX_IMG_PATH"num0.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_NUM1,                DFEX_IMG_PATH"num1.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_NUM2,                DFEX_IMG_PATH"num2.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_NUM3,                DFEX_IMG_PATH"num3.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_NUM4,                DFEX_IMG_PATH"num4.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_NUM5,                DFEX_IMG_PATH"num5.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_NUM6,                DFEX_IMG_PATH"num6.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_NUM7,                DFEX_IMG_PATH"num7.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_NUM8,                DFEX_IMG_PATH"num8.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_NUM9,                DFEX_IMG_PATH"num9.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_P1,                  DFEX_IMG_PATH"p1.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_P2,                  DFEX_IMG_PATH"p2.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_PAD,                DFEX_IMG_PATH"pad.gif", "");             
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_PAD1,                DFEX_IMG_PATH"pad1.gif", "");             
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_PLANE1,             DFEX_IMG_PATH"plane1.gif", ""); 
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_SPCARD1,             DFEX_IMG_PATH"spCard1.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_SPCARD2,             DFEX_IMG_PATH"spCard2.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_SPCARD3,             DFEX_IMG_PATH"spCard3.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_SPCARD4,             DFEX_IMG_PATH"spCard4.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_SPCARD5,             DFEX_IMG_PATH"spCard5.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_STONE1,              DFEX_IMG_PATH"stone1.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_STONE2,              DFEX_IMG_PATH"stone2.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_STONES,              DFEX_IMG_PATH"stones.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_TITLE,              DFEX_IMG_PATH"title.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_ARROW,              DFEX_IMG_PATH"arrow.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_CLOUD,                DFEX_IMG_PATH"cloud.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_BLOODTABLE1,         DFEX_IMG_PATH"bloodTable1.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_BLOODTABLE2,         DFEX_IMG_PATH"bloodTable2.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EMPLACEMENT_LAND1,   DFEX_IMG_PATH"emplacement_land1.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EMPLACEMENT_LAND2,   DFEX_IMG_PATH"emplacement_land2.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EMPLACEMENT_LAND3,   DFEX_IMG_PATH"emplacement_land3.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EMPLACEMENT_LAND4,   DFEX_IMG_PATH"emplacement_land4.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EMPLACEMENT_SEA1,    DFEX_IMG_PATH"emplacement_sea1.gif", ""); 
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EMPLACEMENT_SEA2,    DFEX_IMG_PATH"emplacement_sea2.gif", ""); 
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EMPLACEMENT_SEA3,    DFEX_IMG_PATH"emplacement_sea3.gif", ""); 
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EMPLACEMENT_SEA4,    DFEX_IMG_PATH"emplacement_sea4.gif", ""); 
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EMPLACEMENT_SEA5,    DFEX_IMG_PATH"emplacement_sea5.gif", ""); 
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EMPLACEMENT_SEA6,    DFEX_IMG_PATH"emplacement_sea6.gif", ""); 
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EMPLACEMENT_SEA7,    DFEX_IMG_PATH"emplacement_sea7.gif", ""); 
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EMPLACEMENT_SEA21,   DFEX_IMG_PATH"emplacement_sea21.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EMPLACEMENT_SEA22,   DFEX_IMG_PATH"emplacement_sea22.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EMPLACEMENT_SEA23,   DFEX_IMG_PATH"emplacement_sea23.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EMPLACEMENT_SEA24,   DFEX_IMG_PATH"emplacement_sea24.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EMPLACEMENT_SEA25,   DFEX_IMG_PATH"emplacement_sea25.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EMPLACEMENT_SEA26,   DFEX_IMG_PATH"emplacement_sea26.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_EMPLACEMENT_SEA27,   DFEX_IMG_PATH"emplacement_sea27.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_ENERGYBALL1,         DFEX_IMG_PATH"energyBall1.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_ENERGYBALL2,         DFEX_IMG_PATH"energyBall2.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_ENERGYBALL3,         DFEX_IMG_PATH"energyBall3.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_TREE,         DFEX_IMG_PATH"tree.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_XRQ,         DFEX_IMG_PATH"xrq.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_HILL1,         DFEX_IMG_PATH"hill1.gif", "");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_DFEX_GAME_HILL2,         DFEX_IMG_PATH"hill2.gif", "");
	
	}
#endif /* IS_DFEX_BASE_GAME */

/************************************************************************/
/* Panda	  128x128, 128x160		176x220													*/	
/************************************************************************/
#ifdef IS_PANDA_BASE_GAME
void PopulatePandaRes(void)
{
	/* set image path */
#if defined(__MMI_GAME_PANDA_128x128__)
	#define PANDA_IMG_PATH CUST_IMG_BASE_PATH"/GameImages/panda/panda_128x128_panda/"
#elif defined(__MMI_GAME_PANDA_128x160__)
	#define PANDA_IMG_PATH CUST_IMG_BASE_PATH"/GameImages/panda/panda_128x160_panda/"
#elif defined(__MMI_GAME_PANDA_176x220__)
	#define PANDA_IMG_PATH CUST_IMG_BASE_PATH"/GameImages/panda/panda_176x220_panda/"

#elif defined(__MMI_GAME_MONKEY_128x128__)
	#define PANDA_IMG_PATH CUST_IMG_BASE_PATH"/GameImages/panda/panda_128x128_monkey/"
#elif defined(__MMI_GAME_MONKEY_128x160__)
	#define PANDA_IMG_PATH CUST_IMG_BASE_PATH"/GameImages/panda/panda_128x160_monkey/"
#elif defined(__MMI_GAME_MONKEY_176x220__)
	#define PANDA_IMG_PATH CUST_IMG_BASE_PATH"/GameImages/panda/panda_176x220_monkey/"
#endif
	/* set game strings */
#if defined(__MMI_GAME_PANDA_128x128__) || defined(__MMI_GAME_PANDA_128x160__) || defined(__MMI_GAME_PANDA_176x220__)
	ADD_APPLICATION_STRING2(STR_ID_GX_PANDA_GAME_NAME,				"Panda",		"Panda Game Name");
	ADD_APPLICATION_STRING2(STR_ID_GX_PANDA_HELP_DESCRIPTION,	"Control the panda to attack enemies or prevent tounching them. To get the CLOCK can extend the playing time.\n(1)Use key 2(or up-arrow key) to move upward, key 4(or left-arrow key) to move leftward, key 8(or down-arrow key) to move downward, key 6(or right-arrow key) to move rightward, and key 5 to attack enemies.",	"Panda Help Description");
#elif defined(__MMI_GAME_MONKEY_128x128__) || defined(__MMI_GAME_MONKEY_128x160__) || defined(__MMI_GAME_MONKEY_176x220__)
	ADD_APPLICATION_STRING2(STR_ID_GX_PANDA_GAME_NAME_MONKEY,				"Monkey",		"Monkey Game Name");
	ADD_APPLICATION_STRING2(STR_ID_GX_PANDA_HELP_DESCRIPTION_MONKEY,	"Control the monkey to attack enemies or prevent tounching them. To get the CLOCK can extend the playing time.\n(1)Use key 2(or up-arrow key) to move upward, key 4(or left-arrow key) to move leftward, key 8(or down-arrow key) to move downward, key 6(or right-arrow key) to move rightward, and key 5 to attack enemies.",	"Monkey Help Description");
#endif

	/* set game images */
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_GAME_ICON,					PANDA_IMG_PATH"gx_panda_icon.bmp",						"Panda icon image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_BACKGROUND,					PANDA_IMG_PATH"gx_panda_background.gif",				"Background Image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_CLOCK,						PANDA_IMG_PATH"gx_panda_clock.pbm",						"Clock Image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_SCORE,						PANDA_IMG_PATH"gx_panda_score.bmp",						"Image for string SCORE");

	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ROLE_JUMP,					PANDA_IMG_PATH"gx_panda_role_jump.bmp",				"Jump image for role");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ROLE_FALL_FRAME_0,		PANDA_IMG_PATH"gx_panda_role_fall_frame_0.bmp",		"Falling image 0 for role");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ROLE_FALL_FRAME_1,		PANDA_IMG_PATH"gx_panda_role_fall_frame_1.bmp",		"Falling image 1 for role");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ROLE_LEFT_FRAME_0,		PANDA_IMG_PATH"gx_panda_role_left_frame_0.bmp",		"Left image 0 for role");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ROLE_LEFT_FRAME_1,		PANDA_IMG_PATH"gx_panda_role_left_frame_1.bmp",		"Left image 1 for role");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ROLE_RIGHT_FRAME_0,		PANDA_IMG_PATH"gx_panda_role_right_frame_0.bmp",	"Right image 0 for role");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ROLE_RIGHT_FRAME_1,		PANDA_IMG_PATH"gx_panda_role_right_frame_1.bmp",	"Right image 1 for role");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ROLE_ATTACK_L_FRAME_0,	PANDA_IMG_PATH"gx_panda_role_attack_l_frame_0.bmp","Left image 0 for role attacking");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ROLE_ATTACK_L_FRAME_1,	PANDA_IMG_PATH"gx_panda_role_attack_l_frame_1.bmp","Left image 1 for role attacking");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ROLE_ATTACK_R_FRAME_0,	PANDA_IMG_PATH"gx_panda_role_attack_r_frame_0.bmp","Right image 0 for role attacking");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ROLE_ATTACK_R_FRAME_1,	PANDA_IMG_PATH"gx_panda_role_attack_r_frame_1.bmp","Right image 1 for role attacking");
	
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ENEMY_1_LEFT,				PANDA_IMG_PATH"gx_panda_enemy_1_left.bmp",			"Left image for enemy 1");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ENEMY_2_LEFT,				PANDA_IMG_PATH"gx_panda_enemy_2_left.bmp",			"Left image for enemy 2");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ENEMY_3_LEFT,				PANDA_IMG_PATH"gx_panda_enemy_3_left.bmp",			"Left image for enemy 3");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ENEMY_1_LEFT_FRAME_2,	PANDA_IMG_PATH"gx_panda_enemy_1_left_frame_2.bmp",	"Left image for enemy 1");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ENEMY_2_LEFT_FRAME_2,	PANDA_IMG_PATH"gx_panda_enemy_2_left_frame_2.bmp",	"Left image for enemy 2");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ENEMY_3_LEFT_FRAME_2,	PANDA_IMG_PATH"gx_panda_enemy_3_left_frame_2.bmp",	"Left image for enemy 3");
	
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ENEMY_1_RIGHT,				PANDA_IMG_PATH"gx_panda_enemy_1_right.bmp",			"Right image for enemy 1");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ENEMY_2_RIGHT,				PANDA_IMG_PATH"gx_panda_enemy_2_right.bmp",			"Right image for enemy 2");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ENEMY_3_RIGHT,				PANDA_IMG_PATH"gx_panda_enemy_3_right.bmp",			"Right image for enemy 3");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ENEMY_1_RIGHT_FRAME_2,	PANDA_IMG_PATH"gx_panda_enemy_1_right_frame_2.bmp","Right image for enemy 1");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ENEMY_2_RIGHT_FRAME_2,	PANDA_IMG_PATH"gx_panda_enemy_2_right_frame_2.bmp","Right image for enemy 2");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_ENEMY_3_RIGHT_FRAME_2,	PANDA_IMG_PATH"gx_panda_enemy_3_right_frame_2.bmp","Right image for enemy 3");

	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_GOTEXT,						PANDA_IMG_PATH"gameover/gx_panda_gameover.gif",	"game over text");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_GRADESMAP,					PANDA_IMG_PATH"gameover/gx_panda_gradesmap.gif","grade background");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_PANDA_GOPIC,						PANDA_IMG_PATH"gameover/gx_panda_pic.gif",		"game over image");
}
#endif /* IS_PANDA_BASE_GAME */


/************************************************************************/
/* Stair	128x160  																	   */	
/************************************************************************/
#ifdef IS_STAIR_BASE_GAME
/* under construction !*/
/* under construction !*/
/* under construction !*/
#if defined(__MMI_GAME_STAIR_128x128__)
/* under construction !*/
#elif defined(__MMI_GAME_STAIR_128x160__)
/* under construction !*/
#elif defined(__MMI_GAME_STAIR_176x220__)
/* under construction !*/
/* under construction !*/
#elif defined(__MMI_GAME_CHICKEN_128x128__)
/* under construction !*/
#elif defined(__MMI_GAME_CHICKEN_128x160__)
/* under construction !*/
/* under construction !*/
#elif defined(__MMI_GAME_ROBOT_176x220__)
/* under construction !*/
#elif defined(__MMI_GAME_ROBOT_128x160__)
/* under construction !*/
#endif
/* under construction !*/
/* under construction !*/
#if defined(__MMI_GAME_STAIR_128x160__) || defined(__MMI_GAME_STAIR_128x128__) || defined(__MMI_GAME_STAIR_176x220__)
/* under construction !*/
/* under construction !*/
#elif defined(__MMI_GAME_CHICKEN_128x128__) || defined(__MMI_GAME_CHICKEN_128x160__)
/* under construction !*/
/* under construction !*/
#elif defined(__MMI_GAME_ROBOT_176x220__) || defined(__MMI_GAME_ROBOT_128x160__)
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
/* under construction !*/
	#if defined(__MMI_GAME_STAIR_128x160__) || defined(__MMI_GAME_STAIR_176x220__) || defined(__MMI_GAME_CHICKEN_128x160__) || defined(__MMI_GAME_ROBOT_176x220__) || defined(__MMI_GAME_ROBOT_128x160__)
/* under construction !*/
	#elif defined(__MMI_GAME_STAIR_128x128__) || defined(__MMI_GAME_CHICKEN_128x128__)
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
#endif /* IS_STAIR_BASE_GAME */


/************************************************************************/
/* Ufo	128x 128 128x160 176*220 																	   */	
/************************************************************************/
#ifdef IS_UFO_BASE_GAME 
void PopulateUfoRes(void)
{
	/* set image path */
#if defined(__MMI_GAME_UFO_128x128__)
	#define UFO_IMG_PATH CUST_IMG_BASE_PATH"/GameImages/ufo/ufo_128x128_ufo/"
#elif defined(__MMI_GAME_UFO_128x160__)
	#define UFO_IMG_PATH CUST_IMG_BASE_PATH"/GameImages/ufo/ufo_128x160_ufo/"
#elif defined(__MMI_GAME_UFO_176x220__)
	#define UFO_IMG_PATH CUST_IMG_BASE_PATH"/GameImages/ufo/ufo_176x220_ufo/"
#elif defined(__MMI_GAME_UFO_220x176__)
	#define UFO_IMG_PATH CUST_IMG_BASE_PATH"/GameImages/ufo/ufo_220x176_ufo/"	
#endif

	/* set game strings */
	ADD_APPLICATION_STRING2(STR_ID_GX_UFO_GAME_NAME,			"UFO",		"Ufo Game Name");
	ADD_APPLICATION_STRING2(STR_ID_GX_UFO_HELP_DESCRIPTION,	"Control the fighter to shoot UFOs and prevent being attacked by them.\n(1)Use key 2(or up-arrow key) to move upward, key 4(or left-arrow key) to move leftward, key 8(or down-arrow key) to move downward, key 6(or right-arrow key) to move rightward.",	"Ufo Help Description");

	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_GAME_ICON,			UFO_IMG_PATH"gx_ufo_icon.pbm",						"UFO icon image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_DIGIT_0,			UFO_IMG_PATH"gx_ufo_digit_0.pbm",					"Image for 0");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_DIGIT_1,			UFO_IMG_PATH"gx_ufo_digit_1.pbm",					"Image for 1");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_DIGIT_2,			UFO_IMG_PATH"gx_ufo_digit_2.pbm",					"Image for 2");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_DIGIT_3,			UFO_IMG_PATH"gx_ufo_digit_3.pbm",					"Image for 3");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_DIGIT_4,			UFO_IMG_PATH"gx_ufo_digit_4.pbm",					"Image for 4");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_DIGIT_5,			UFO_IMG_PATH"gx_ufo_digit_5.pbm",					"Image for 5");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_DIGIT_6,			UFO_IMG_PATH"gx_ufo_digit_6.pbm",					"Image for 6");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_DIGIT_7,			UFO_IMG_PATH"gx_ufo_digit_7.pbm",					"Image for 7");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_DIGIT_8,			UFO_IMG_PATH"gx_ufo_digit_8.pbm",					"Image for 8");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_DIGIT_9,			UFO_IMG_PATH"gx_ufo_digit_9.pbm",					"Image for 9");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_BACKGROUND_1,		UFO_IMG_PATH"gx_ufo_background_1.pbm",			"Background image 1");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_BACKGROUND_2,		UFO_IMG_PATH"gx_ufo_background_2.pbm",			"Background image 2");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_BACKGROUND_3,		UFO_IMG_PATH"gx_ufo_background_3.pbm",			"Background image 3");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_BACKGROUND_4,		UFO_IMG_PATH"gx_ufo_background_4.pbm",			"Background image 4");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_BACKGROUND_5,		UFO_IMG_PATH"gx_ufo_background_5.pbm",			"Background image 5");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_BACKGROUND_6,		UFO_IMG_PATH"gx_ufo_background_6.pbm",			"Background image 6");

	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_BOSS,				UFO_IMG_PATH"gx_ufo_boss.pbm",						"Boss image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_BOSS_HIT,			UFO_IMG_PATH"gx_ufo_boss_hit.pbm",				"Boss hit image");
	
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_ENEMY_1,			UFO_IMG_PATH"gx_ufo_enemy_1.pbm",					"Enemy 1 image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_ENEMY_2,			UFO_IMG_PATH"gx_ufo_enemy_2.pbm",					"Enemy 2 image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_ENEMY_3,			UFO_IMG_PATH"gx_ufo_enemy_3.pbm",					"Enemy 3 image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_ENEMY_4,			UFO_IMG_PATH"gx_ufo_enemy_4.pbm",					"Enemy 4 image");
	
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_ENEMY_1_HIT,		UFO_IMG_PATH"gx_ufo_enemy_1_hit.pbm",			"Enemy 1 hit image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_ENEMY_2_HIT,		UFO_IMG_PATH"gx_ufo_enemy_2_hit.pbm",			"Enemy 2 hit image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_ENEMY_3_HIT,		UFO_IMG_PATH"gx_ufo_enemy_3_hit.pbm",			"Enemy 3 hit image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_ENEMY_4_HIT,		UFO_IMG_PATH"gx_ufo_enemy_4_hit.pbm",			"Enemy 4 hit image");
	
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_ENEMY_BULLET_1,	UFO_IMG_PATH"gx_ufo_enemy_bullet_1.pbm",		"Enemy bullet 1 image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_ENEMY_BULLET_2,	UFO_IMG_PATH"gx_ufo_enemy_bullet_2.pbm",		"Enemy bullet 2 image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_ENEMY_BULLET_3,	UFO_IMG_PATH"gx_ufo_enemy_bullet_3.pbm",		"Enemy bullet 3 image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_ENEMY_BULLET_4,	UFO_IMG_PATH"gx_ufo_enemy_bullet_4.pbm",		"Enemy bullet 4 image");
	
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_ENERGY,				UFO_IMG_PATH"gx_ufo_energy.pbm",					"Image for string ENERGY");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_SCORE,				UFO_IMG_PATH"gx_ufo_score.pbm",					"Image for string SCORE");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_ENERGY_UNIT,		UFO_IMG_PATH"gx_ufo_energy_unit.pbm",			"Image for energy unit");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_WEAPON_1,			UFO_IMG_PATH"gx_ufo_weapon_1.pbm",				"Image for weapon");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_WEAPON_2,			UFO_IMG_PATH"gx_ufo_weapon_2.pbm",				"Image for weapon");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_ENERGY_FILL_1,	UFO_IMG_PATH"gx_ufo_energy_fill_1.pbm",			"Image for energy filling");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_ENERGY_FILL_2,	UFO_IMG_PATH"gx_ufo_energy_fill_2.pbm",			"Image for energy filling");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_BOMB_1,				UFO_IMG_PATH"gx_ufo_bomb_1.pbm",					"Image for bomb");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_BOMB_2,				UFO_IMG_PATH"gx_ufo_bomb_2.pbm",					"Image for bomb");

	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_FLAME_1,			UFO_IMG_PATH"gx_ufo_flame_1.pbm",					"Flame 1 image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_FLAME_2,			UFO_IMG_PATH"gx_ufo_flame_2.pbm",					"Flame 2 image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_FLAME_3,			UFO_IMG_PATH"gx_ufo_flame_3.pbm",					"Flame 3 image");

	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_ROLE,				UFO_IMG_PATH"gx_ufo_role.pbm",						"Role image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_ROLE_HIT,			UFO_IMG_PATH"gx_ufo_role_hit.pbm",				"Role hit image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_ROLE_BULLET,		UFO_IMG_PATH"gx_ufo_role_bullet.pbm",			"Image of role's bullet");

	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_INVINCIBLE_1,		UFO_IMG_PATH"gx_ufo_invincible_1.pbm",			"Image of invincible treasure");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_INVINCIBLE_2,		UFO_IMG_PATH"gx_ufo_invincible_2.pbm",			"Image of invincible treasure");

	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_LEVEL_CLEAR,		UFO_IMG_PATH"gx_ufo_level_clear.pbm",			"String for LEVEL CLRAR");

	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_GOTEXT,				UFO_IMG_PATH"gameover/gx_ufo_gameover.gif",	"game over text");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_GRADESMAP,			UFO_IMG_PATH"gameover/gx_ufo_gradesmap.gif",	"grade background");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_UFO_GOPIC,				UFO_IMG_PATH"gameover/gx_ufo_pic.gif",			"game over image");
}
#endif /* IS_UFO_BASE_GAME */

/************************************************************************/
/* Magicsushi																				*/	
/************************************************************************/
#ifdef IS_MAGICSUSHI_BASE_GAME
void PopulateMagicsushiRes(void)
{
	/* strings */
	ADD_APPLICATION_STRING2(STR_ID_GX_MAGICSUSHI_GAME_NAME,"Magicsushi","Title displayed in Games Menu");
	//ADD_APPLICATION_STRING2(STR_ID_GX_MAGICSUSHI_TITLE,"Hanoi","String displayed in Hanio Title Bar");
	
	ADD_APPLICATION_STRING2(STR_ID_GX_MAGICSUSHI_HELP_DESCRIPTION,"Use the following methods to eliminate susui:\n(1)\n(2)star:will randomly eliminate one kind of susui.\n(3)cross: will eliminate all susuis on the cross' column and row.\n(4)little ball: will eliminate susuis by 3x3 matrix which is centered by the little ball.", "String for Help In Hanio Game");
	
	/* images */
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_GAME_ICON,	CUST_IMG_BASE_PATH"/GameImages/Magicsushi/Magicsushi.BMP","Games image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_GAME_BACKGROUND,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_background.Pbm","Game's background image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_NUMBER_0,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_num_0.pbm","Number 0 Image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_NUMBER_1,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_num_1.pbm","Number 1 Image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_NUMBER_2,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_num_2.pbm","Number 2 Image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_NUMBER_3,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_num_3.pbm","Number 3 Image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_NUMBER_4,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_num_4.pbm","Number 4 Image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_NUMBER_5,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_num_5.pbm","Number 5 Image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_NUMBER_6,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_num_6.pbm","Number 6 Image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_NUMBER_7,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_num_7.pbm","Number 7 Image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_NUMBER_8,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_num_8.pbm","Number 8 Image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_NUMBER_9,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_num_9.pbm","Number 9 Image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_TYPE_NULL,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_item_null.pbm","null item's Image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_SELECTED,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_select.pbm","Image for the selected item");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_PROGRESS,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_progress.pbm","progress bar Image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_TYPE_0,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_item_1.pbm","susui 1");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_TYPE_1,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_item_2.pbm","susui 2");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_TYPE_2,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_item_3.pbm","susui 3");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_TYPE_3,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_item_4.pbm","susui 4");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_TYPE_4,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_item_5.pbm","susui 5");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_TYPE_5,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_item_6.pbm","susui 6");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_TYPE_6,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_item_7.pbm","susui 7");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_TYPE_7,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_item_8.pbm","susui 8");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_TYPE_MAGIC1,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_magic_1.pbm","magic item 1");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_TYPE_MAGIC2,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_magic_2.pbm","magic item 2");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_TYPE_MAGIC3,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_magic_3.pbm","magic item 3");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_GAMEOVER,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_gameover.gif","gameover image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_UPLEVEL,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_uplevel.gif","uplevel image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_NOMOREMOVE,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_nomoremove.gif","no more move Image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_CURSOR,CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_select1.pbm","cursor Image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_UP,			CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_up.pbm","Tower Bar Image");
	ADD_APPLICATION_IMAGE2(IMG_ID_GX_MAGICSUSHI_DOWN,		CUST_IMG_BASE_PATH"/GameImages/Magicsushi/gx_magicsushi_down.pbm","Tower Bar Image");

}
#endif /* IS_MAGICSUSHI_BASE_GAME */

/************************************************************************/
/* Fruit	  176x220																		*/	
/************************************************************************/

#ifdef IS_FRUIT_BASE_GAME
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
#if defined(__MMI_GAME_FRUIT_176x220__)
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
/* under construction !*/
/* under construction !*/
#endif /* IS_FRUIT_BASE_GAME */	

/************************************************************************/
/* Vsmj	  240x320																		*/	
/************************************************************************/

#ifdef IS_VSMJ_BASE_GAME
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
#endif /* IS_VSMJ_BASE_GAME */	

/************************************************************************/
/* Games																						*/		
/************************************************************************/
void PopulateGamesRes(void)
{
	/* create string */
	ADD_APPLICATION_STRING2(STR_GAME_CAPTION,		"Games","Caption string");
	ADD_APPLICATION_STRING2(STR_GAME_LSK,			"Select","LSK String");

	ADD_APPLICATION_STRING2(STR_GAME_NEW,			"Start Game","Start Menu String");	
	ADD_APPLICATION_STRING2(STR_GAME_LEVEL,		"Game Level","Resume Menu String");	
	ADD_APPLICATION_STRING2(STR_GAME_GRADE,		"Best Grades","Grades Menu-Caption String");	
	ADD_APPLICATION_STRING2(STR_GAME_HELP,			"Game Help","Help Menu-Caption String");	
	ADD_APPLICATION_STRING2(STR_GAME_RESUME,		"Resume","Resume Menu String");	
	ADD_APPLICATION_STRING2(STR_GAME_RESET,		"Reset","Game Score Reset LSK String");	
	ADD_APPLICATION_STRING2(STR_GAME_SCORE,		"Score","Score string");	
	ADD_APPLICATION_STRING2(STR_MENU3105_GAMES,	"Games","Games menu");

	/* game seting */
	ADD_APPLICATION_STRING2(STR_ID_FNG_SETTING_GAME, "Game Setting", "Game Settings");	
	ADD_APPLICATION_IMAGE2(IMG_ID_FNG_SETTING_GAME, CUST_IMG_PATH"/MainLCD/SubMenu/Funandgame/Gamesetting/SB_FGST.gif",	"effect setting");

	ADD_APPLICATION_STRING2(STR_ID_FNG_SETTING_GAME_AUD, "Game Audio",	"Game Audio setting");

#ifdef __MMI_GAME_MULTICHANNEL_SOUND__
//	ADD_APPLICATION_STRING2(STR_ID_GAME_SETTING_MULTICHANNEL_BACKGROUND_MUSIC, "BGM",	"Game Audio setting");
//	ADD_APPLICATION_STRING2(STR_ID_GAME_SETTING_MULTICHANNEL_SOUND_EFFECT, "Sound Effect",	"Game Audio setting");
#endif  /*__MMI_GAME_MULTICHANNEL_SOUND__*/
	ADD_APPLICATION_STRING2(STR_ID_FNG_SETTING_GAME_VIB, "Game Vibration",	"Game Vibration setting");	
	ADD_APPLICATION_IMAGE2(IMG_ID_FNG_SETTING_GAME_AUD, CUST_IMG_PATH"/MainLCD/SubMenu/Funandgame/Gamesetting/SB_GAMEAUD.gif",	"Game Audio Icon");	
#ifdef __MMI_GAME_MULTICHANNEL_SOUND__
//		ADD_APPLICATION_IMAGE2(IMG_ID_GAME_SETTING_BACK_AUD, CUST_IMG_PATH"/MainLCD/SubMenu/Funandgame/Gamesetting/SB_GAMEBACKAUD.pbm",	"Game BACK Audio Icon");	
#endif  /*__MMI_GAME_MULTICHANNEL_SOUND__*/
	ADD_APPLICATION_IMAGE2(IMG_ID_FNG_SETTING_GAME_VIB, CUST_IMG_PATH"/MainLCD/SubMenu/Funandgame/Gamesetting/SB_GAMEVIB.gif",	"Game Vib Icon");	


	/* create image */
	ADD_APPLICATION_IMAGE2(IMG_MENU3105_GAMES,CUST_IMG_PATH"/MainLCD/SubMenu/FunANDGame/SB_FGGAME.gif","Games image");
	//ADD_APPLICATION_IMAGE2(IMG_ID_GAME_SUBLCD_ICON,CUST_IMG_PATH"/SubLCD/Active/sb_game.bmp","Sublcd games icon");

#ifdef IS_F1RACE_BASE_GAME
/* under construction !*/
#endif /* IS_F1RACE_BASE_GAME */

#ifdef IS_HANOI_BASE_GAME
/* under construction !*/
#endif /* IS_HANOI_BASE_GAME */

#ifdef IS_RICHES_BASE_GAME
/* under construction !*/
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
#endif  /* IS_NINJA_BASE_GAME */

#ifdef IS_MAJUNG_BASE_GAME
	PopulateMajungRes();
#endif  /* IS_MAJUNG_BASE_GAME */

#ifdef IS_MEMORY_BASE_GAME
/* under construction !*/
#endif /* IS_MEMORY_BASE_GAME */

#ifdef IS_COPTER_BASE_GAME
	PopulateCopterRes();
#endif /* IS_COPTER_BASE_GAME */

#ifdef IS_PUZZLE_BASE_GAME
	PopulatePuzzleRes();
#endif /* IS_PUZZLE_BASE_GAME */
///WX
#ifdef IS_DFEX_BASE_GAME
	PopulateDFexRes();
#endif /* IS_DFEX_BASE_GAME */
/// WX add
#ifdef __MMI_GAME_BQMET__
	PopulateBqmetRes();
#endif /* __MMI_GAME_BQMET__ */

#ifdef IS_PANDA_BASE_GAME
	PopulatePandaRes();
#endif /* IS_PANDA_BASE_GAME */

#ifdef IS_STAIR_BASE_GAME
/* under construction !*/
#endif /* IS_STAIR_BASE_GAME */

#ifdef IS_UFO_BASE_GAME 
	PopulateUfoRes();
#endif /* IS_UFO_BASE_GAME */

#ifdef IS_DANCE_BASE_GAME
	PopulateDanceRes();
#endif /* IS_DANCE_BASE_GAME */

#ifdef IS_MAGICSUSHI_BASE_GAME
	PopulateMagicsushiRes();
#endif /* IS_MAGICBALLOON_BASE_GAME */

#ifdef IS_FRUIT_BASE_GAME
/* under construction !*/
#endif /* IS_FRUIT_BASE_GAME */

#ifdef IS_VSMJ_BASE_GAME
/* under construction !*/
#endif /* IS_VSMJ_BASE_GAME */

}


#endif /* __MMI_GAME__ */


/*****************************************************************************
*
*	Motion Sensor Applications
*
*****************************************************************************/
#ifdef __MMI_MOTION_APP__

#include "FunAndGamesDefs.h"
#include "GameDefs.h"

#ifdef IS_DICE_BASE_MOTIONGAME
#include "MotionDiceDefs.h"
#endif /* IS_DICE_BASE_MOTIONGAME */

#ifdef IS_DJ_BASE_MOTIONGAME
#include "MotionDJDefs.h"
#endif /* IS_DJ_BASE_MOTIONGAME */


/*****************************************************************************
* FUNCTION
*	 PopulateMotionDiceRes()
* DESCRIPTION
*   populate motion game - dice
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef IS_DICE_BASE_MOTIONGAME
void PopulateMotionDiceRes(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	

  	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_APP,						"Dice", "STR_ID_MOTION_DICE_APP");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_START,					"Start","STR_ID_MOTION_DICE_START");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_GAME_SETTING,			"Game Setting","STR_ID_MOTION_DICE_GAME_SETTING");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_EVENT_SETTING,		"Event Setting","STR_ID_MOTION_DICE_EVENT_SETTING");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_DICE_COUNT,			"Dice Number","STR_ID_MOTION_DICE_DICE_COUNT");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_SHOW_EVENT,			"Show Event","STR_ID_MOTION_DICE_SHOW_EVENT");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_OPTION,					"Option","STR_ID_MOTION_DICE_OPTION");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_OPTION_ADD,			"Add","STR_ID_MOTION_DICE_OPTION_ADD");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_OPTION_REMOVE,		"Remove","STR_ID_MOTION_DICE_OPTION_REMOVE");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_OPTION_EDIT,			"Edit","STR_ID_MOTION_DICE_OPTION_EDIT");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_EVENT_EDIT,			"Edit Event","STR_ID_MOTION_DICE_EVENT_EDIT");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_EVENT_MATCH_NUM,		"Match Number","STR_ID_MOTION_DICE_EVENT_MATCH_NUM");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_EVENT_TEXT,			"Text","STR_ID_MOTION_DICE_EVENT_TEXT");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_EVENT_DEFAULT_TEXT,	"Event","STR_ID_MOTION_DICE_EVENT_DEFAULT_TEXT");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_EVENT_EFFECT_FILE,	"Effect File","STR_ID_MOTION_DICE_EVENT_EFFECT_FILE");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_EVENT_FILE_NONE,		"None","STR_ID_MOTION_DICE_EVENT_FILE_NONE");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_EVENT_FILE_SELECT,	"Select From File","STR_ID_MOTION_DICE_EVENT_FILE_SELECT");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_EVENT_EDITOR_TITLE,	"Text Editor","STR_ID_MOTION_DICE_EVENT_EDITOR_TITLE");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_EVENT_PLAY_VIDEO,	"Play Video","STR_ID_MOTION_DICE_EVENT_PLAY_VIDEO");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_EVENT_PLAY_AUDIO,	"Play Audio","STR_ID_MOTION_DICE_EVENT_PLAY_AUDIO");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_EVENT_VIEW_IMAGE,	"View Image","STR_ID_MOTION_DICE_EVENT_VIEW_IMAGE");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_REMOVE_CONFIRM,		"Remove?","STR_ID_MOTION_DICE_REMOVE_CONFIRM");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_EVENT_REMOVED,		"Removed","STR_ID_MOTION_DICE_EVENT_REMOVED");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_SAVE_CONFIRM,			"Save Change?","STR_ID_MOTION_DICE_SAVE_CONFIRM");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_NUMBER_REGION,		"Valid Number","STR_ID_MOTION_DICE_NUMBER_REGION");		
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_DUP_NUMBER,			"Number Duplicated","STR_ID_MOTION_DICE_DUP_NUMBER");		
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_EVENT_FULL,			"Events Full","STR_ID_MOTION_DICE_EVENT_FULL");			
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_1,						"1","STR_ID_MOTION_DICE_1");		
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_2,						"2","STR_ID_MOTION_DICE_1");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_3,						"3","STR_ID_MOTION_DICE_1");		
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_4,						"4","STR_ID_MOTION_DICE_1");		
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DICE_5,						"5","STR_ID_MOTION_DICE_1");			

	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_APP, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_app.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_1, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_1.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_2, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_2.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_3, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_3.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_4, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_4.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_5, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_5.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_6, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_6.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_7, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_7.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_8, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_8.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_9, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_9.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_10, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_10.pbm","Dice");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_11, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_11.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_12, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_12.pbm","Dice");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_13, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_13.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_14, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_14.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_15, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_15.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_16, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_16.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_17, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_17.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_18, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_18.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_19, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_19.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_20, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_20.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_21, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_21.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_22, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_22.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_23, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_23.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_24, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_24.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_25, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_25.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_26, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_26.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_27, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_27.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_28, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_28.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_29, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_29.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ICON_30, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_30.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_1, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_1.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_2, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_2.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_3, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_3.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_4, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_4.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_5, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_5.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_6, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_6.pbm","Dice");	

	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ROLL_1, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_roll_01.pbm","Dice");		
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ROLL_2, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_roll_02.pbm","Dice");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ROLL_3, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_roll_03.pbm","Dice");		
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ROLL_4, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_roll_04.pbm","Dice");			
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ROLL_5, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_roll_05.pbm","Dice");	

	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ROLL_6, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_roll_06.pbm","Dice");		
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ROLL_7, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_roll_07.pbm","Dice");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ROLL_8, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_roll_08.pbm","Dice");		
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ROLL_9, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_roll_09.pbm","Dice");			
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ROLL_10, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_roll_10.pbm","Dice");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ROLL_11, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_roll_11.pbm","Dice");			
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_ROLL_12, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_roll_12.pbm","Dice");	

	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_EVENT_1, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_event_1.pbm","Dice");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_EVENT_2, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_event_2.pbm","Dice");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_EVENT_3, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_event_3.pbm","Dice");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_EVENT_4, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_event_4.pbm","Dice");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_EVENT_5, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_event_5.pbm","Dice");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_EVENT_6, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_event_6.pbm","Dice");		
	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_PLAY_BG, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_bg.gif","Dice");			
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_EVENT_BG, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_event_bg.gif","Dice");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_EVENT_DEFAULT_IMG, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_event_def.gif","Dice");		

	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_FX_ICON_DUMMY, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_dummy.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_FX_ICON_AUD, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_aud.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_FX_ICON_VDO, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_vdo.pbm","Dice");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DICE_FX_ICON_IMG, CUST_IMG_BASE_PATH"/GameImages/Dice/dice_icon_img.pbm","Dice");	

	/* App Menu */
	ADD_APPLICATION_MENUITEM((	MENU_ID_MOTION_DICE_APP, 
										MENU_ID_MOTION_APP,
										3,
										MENU_ID_MOTION_DICE_START,
										MENU_ID_MOTION_DICE_GAME_SETTING,
										MENU_ID_MOTION_DICE_EVENT_SETTING,
										SHOW,
										NONMOVEABLE,
										DISP_LIST,
										STR_ID_MOTION_DICE_APP,
										IMG_ID_MOTION_DICE_APP));

	ADD_APPLICATION_MENUITEM((MENU_ID_MOTION_DICE_START, MENU_ID_MOTION_DICE_APP,0,
					SHOW,NONMOVEABLE,1,STR_ID_MOTION_DICE_START,0));

	ADD_APPLICATION_MENUITEM((MENU_ID_MOTION_DICE_GAME_SETTING, MENU_ID_MOTION_DICE_APP,0,
					SHOW,NONMOVEABLE,1,STR_ID_MOTION_DICE_GAME_SETTING,0));
	
	ADD_APPLICATION_MENUITEM((MENU_ID_MOTION_DICE_EVENT_SETTING, MENU_ID_MOTION_DICE_APP,0,
					SHOW,NONMOVEABLE,1,STR_ID_MOTION_DICE_EVENT_SETTING,0));	

	/* Event Option Menu */
	ADD_APPLICATION_MENUITEM((	MENU_ID_MOTION_DICE_OPTION, 
										0,
										3,
										MENU_ID_MOTION_DICE_OPTION_ADD,
										MENU_ID_MOTION_DICE_OPTION_REMOVE,
										MENU_ID_MOTION_DICE_OPTION_EDIT,
										SHOW,
										NONMOVEABLE,
										DISP_LIST,
										STR_ID_MOTION_DICE_OPTION,
										0));

	ADD_APPLICATION_MENUITEM((MENU_ID_MOTION_DICE_OPTION_ADD, MENU_ID_MOTION_DICE_OPTION,0,
					SHOW,NONMOVEABLE,1,STR_ID_MOTION_DICE_OPTION_ADD,0));

	ADD_APPLICATION_MENUITEM((MENU_ID_MOTION_DICE_OPTION_REMOVE, MENU_ID_MOTION_DICE_OPTION,0,
					SHOW,NONMOVEABLE,1,STR_ID_MOTION_DICE_OPTION_REMOVE,0));
	
	ADD_APPLICATION_MENUITEM((MENU_ID_MOTION_DICE_OPTION_EDIT, MENU_ID_MOTION_DICE_OPTION,0,
					SHOW,NONMOVEABLE,1,STR_ID_MOTION_DICE_OPTION_EDIT,0));	

	/* Editor Option */
	ADD_APPLICATION_MENUITEM((	MENU_ID_MOTION_DICE_EDT_OPT_ROOT, 
										0,
										3,
										MENU_ID_MOTION_DICE_EDT_OPT_DONE,
										MENU_ID_MOTION_DICE_EDT_OPT_CANCEL,
										MENU_ID_MOTION_DICE_EDT_OPT_INPUT,
										SHOW,
										NONMOVEABLE,
										DISP_LIST,
										STR_GLOBAL_OPTIONS,
										0));

	ADD_APPLICATION_MENUITEM((MENU_ID_MOTION_DICE_EDT_OPT_DONE, MENU_ID_MOTION_DICE_EDT_OPT_ROOT,0,
					SHOW,NONMOVEABLE,1,STR_GLOBAL_DONE,0));

	ADD_APPLICATION_MENUITEM((MENU_ID_MOTION_DICE_EDT_OPT_CANCEL, MENU_ID_MOTION_DICE_EDT_OPT_ROOT,0,
					SHOW,NONMOVEABLE,1,STR_GLOBAL_CANCEL,0));
	
	ADD_APPLICATION_MENUITEM((MENU_ID_MOTION_DICE_EDT_OPT_INPUT, MENU_ID_MOTION_DICE_EDT_OPT_ROOT,0,
					SHOW,NONMOVEABLE,1,STR_GLOBAL_INPUT_METHOD,0));	
	
	/* Select File */
	ADD_APPLICATION_MENUITEM((	MENU_ID_MOTION_DICE_EVENT_FILE_SELECT_ROOT, 
										0,
										1,
										MENU_ID_MOTION_DICE_EVENT_FILE_SELECT,
										SHOW,
										NONMOVEABLE,
										DISP_LIST,
										STR_ID_MOTION_DICE_EVENT_EFFECT_FILE,
										0));

	ADD_APPLICATION_MENUITEM((MENU_ID_MOTION_DICE_EVENT_FILE_SELECT, MENU_ID_MOTION_DICE_EVENT_FILE_SELECT_ROOT,0,
					SHOW,NONMOVEABLE,1,STR_ID_MOTION_DICE_EVENT_FILE_SELECT,0));

	/* Select File + View Image */
	ADD_APPLICATION_MENUITEM((	MENU_ID_MOTION_DICE_EVENT_VIEW_IMAGE_ROOT, 
										0,
										2,
										MENU_ID_MOTION_DICE_EVENT_VIEW_IMAGE,
										MENU_ID_MOTION_DICE_EVENT_FILE_SELECT,
										SHOW,
										NONMOVEABLE,
										DISP_LIST,
										STR_ID_MOTION_DICE_EVENT_EFFECT_FILE,
										0));

	ADD_APPLICATION_MENUITEM((MENU_ID_MOTION_DICE_EVENT_VIEW_IMAGE, MENU_ID_MOTION_DICE_EVENT_VIEW_IMAGE_ROOT,0,
					SHOW,NONMOVEABLE,1,STR_ID_MOTION_DICE_EVENT_VIEW_IMAGE,0));


	/* Select File + Play Video */
	ADD_APPLICATION_MENUITEM((	MENU_ID_MOTION_DICE_EVENT_PLAY_VIDEO_ROOT, 
										0,
										2,
										MENU_ID_MOTION_DICE_EVENT_PLAY_VIDEO,
										MENU_ID_MOTION_DICE_EVENT_FILE_SELECT,
										SHOW,
										NONMOVEABLE,
										DISP_LIST,
										STR_ID_MOTION_DICE_EVENT_EFFECT_FILE,
										0));

	ADD_APPLICATION_MENUITEM((MENU_ID_MOTION_DICE_EVENT_PLAY_VIDEO, MENU_ID_MOTION_DICE_EVENT_PLAY_VIDEO_ROOT,0,
					SHOW,NONMOVEABLE,1,STR_ID_MOTION_DICE_EVENT_PLAY_VIDEO,0));

	/* Select File + Play Audio */
	ADD_APPLICATION_MENUITEM((	MENU_ID_MOTION_DICE_EVENT_PLAY_AUDIO_ROOT, 
										0,
										2,
										MENU_ID_MOTION_DICE_EVENT_PLAY_AUDIO,
										MENU_ID_MOTION_DICE_EVENT_FILE_SELECT,
										SHOW,
										NONMOVEABLE,
										DISP_LIST,
										STR_ID_MOTION_DICE_EVENT_EFFECT_FILE,
										0));

	ADD_APPLICATION_MENUITEM((MENU_ID_MOTION_DICE_EVENT_PLAY_AUDIO, MENU_ID_MOTION_DICE_EVENT_PLAY_AUDIO_ROOT,0,
					SHOW,NONMOVEABLE,1,STR_ID_MOTION_DICE_EVENT_PLAY_AUDIO,0));
	
}
#endif /* IS_DICE_BASE_MOTIONGAME */



/*****************************************************************************
* FUNCTION
*	 PopulateMotionDJRes()
* DESCRIPTION
*   populate motion game - dj
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef IS_DJ_BASE_MOTIONGAME
void PopulateMotionDJRes(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/	

  	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_APP,		"Music DJ", "STR_ID_MOTION_DJ_APP");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_START,		"Start", "STR_ID_MOTION_DJ_START");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_SETTING,	"Music Setting", "STR_ID_MOTION_DJ_SETTING");
	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_INSTRUMENT_TITLE_1,	"Instrument 1", "STR_ID_MOTION_DJ_INSTRUMENT_TITLE_1");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_INSTRUMENT_TITLE_2,	"Instrument 2", "STR_ID_MOTION_DJ_INSTRUMENT_TITLE_2");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_INSTRUMENT_TITLE_3,	"Instrument 3", "STR_ID_MOTION_DJ_INSTRUMENT_TITLE_3");	
	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_VOCAL_TITLE_1,	"Vocal 1", "STR_ID_MOTION_DJ_VOCAL_TITLE_1");		
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_VOCAL_TITLE_2,	"Vocal 2", "STR_ID_MOTION_DJ_VOCAL_TITLE_2");		
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_VOCAL_TITLE_3,	"Vocal 3", "STR_ID_MOTION_DJ_VOCAL_TITLE_3");	

	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_INSTRUMENT_0,	"Ins 0", "STR_ID_MOTION_DJ_INSTRUMENT_0");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_INSTRUMENT_1,	"Ins 1", "STR_ID_MOTION_DJ_INSTRUMENT_1");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_INSTRUMENT_2,	"Ins 2", "STR_ID_MOTION_DJ_INSTRUMENT_2");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_INSTRUMENT_3,	"Ins 3", "STR_ID_MOTION_DJ_INSTRUMENT_3");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_INSTRUMENT_4,	"Ins 4", "STR_ID_MOTION_DJ_INSTRUMENT_4");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_INSTRUMENT_5,	"Ins 5", "STR_ID_MOTION_DJ_INSTRUMENT_5");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_INSTRUMENT_6,	"Ins 6", "STR_ID_MOTION_DJ_INSTRUMENT_6");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_INSTRUMENT_7,	"Ins 7", "STR_ID_MOTION_DJ_INSTRUMENT_7");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_INSTRUMENT_8,	"Ins 8", "STR_ID_MOTION_DJ_INSTRUMENT_8");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_INSTRUMENT_9,	"Ins 9", "STR_ID_MOTION_DJ_INSTRUMENT_9");	

	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_VOCAL_0,	"Vox 0", "STR_ID_MOTION_DJ_VOCAL_0");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_VOCAL_1,	"Vox 1", "STR_ID_MOTION_DJ_VOCAL_1");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_VOCAL_2,	"Vox 2", "STR_ID_MOTION_DJ_VOCAL_2");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_VOCAL_3,	"Vox 3", "STR_ID_MOTION_DJ_VOCAL_3");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_VOCAL_4,	"Vox 4", "STR_ID_MOTION_DJ_VOCAL_4");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_VOCAL_5,	"Vox 5", "STR_ID_MOTION_DJ_VOCAL_5");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_VOCAL_6,	"Vox 6", "STR_ID_MOTION_DJ_VOCAL_6");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_VOCAL_7,	"Vox 7", "STR_ID_MOTION_DJ_VOCAL_7");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_VOCAL_8,	"Vox 8", "STR_ID_MOTION_DJ_VOCAL_8");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_VOCAL_9,	"Vox 9", "STR_ID_MOTION_DJ_VOCAL_9");	

	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_KEY_0,			"Key 0", "STR_ID_MOTION_DJ_KEY_0");
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_KEY_1,			"Key 1", "STR_ID_MOTION_DJ_KEY_1");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_KEY_2,			"Key 2", "STR_ID_MOTION_DJ_KEY_2");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_KEY_3,			"Key 3", "STR_ID_MOTION_DJ_KEY_3");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_KEY_4,			"Key 4", "STR_ID_MOTION_DJ_KEY_4");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_KEY_5,			"Key 5", "STR_ID_MOTION_DJ_KEY_5");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_KEY_6,			"Key 6", "STR_ID_MOTION_DJ_KEY_6");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_KEY_7,			"Key 7", "STR_ID_MOTION_DJ_KEY_7");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_KEY_8,			"Key 8", "STR_ID_MOTION_DJ_KEY_8");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_KEY_9,			"Key 9", "STR_ID_MOTION_DJ_KEY_9");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_KEY_STAR,		"Key *", "STR_ID_MOTION_DJ_KEY_STAR");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_KEY_POUND,	"Key #", "STR_ID_MOTION_DJ_KEY_POUND");	

	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_EDIT_AUDIO,		"Audio", "STR_ID_MOTION_DJ_EDIT_AUDIO");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_EDIT_VOLUME,		"Volume", "STR_ID_MOTION_DJ_EDIT_VOLUME");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_EDIT_TRIGGER,	"Trigger", "STR_ID_MOTION_DJ_EDIT_TRIGGER");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_EDIT_KEY,			"Key", "STR_ID_MOTION_DJ_EDIT_KEY");		

	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_EDIT_TRIGGER_AUTO,			"Auto", "STR_ID_MOTION_DJ_EDIT_TRIGGER_AUTO");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_EDIT_TRIGGER_SHAKE,		"Shake", "STR_ID_MOTION_DJ_EDIT_TRIGGER_SHAKE");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_EDIT_TRIGGER_KEY_PRESS,	"Key Press", "STR_ID_MOTION_DJ_EDIT_TRIGGER_KEY_PRESS");		

	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_NOTIFY_HAS_AUTO,				"Already has [Auto] trigger", "STR_ID_MOTION_DJ_NOTIFY_HAS_AUTO");	
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_NOTIFY_HAS_SHAKE,				"Already has [Shake]", "STR_ID_MOTION_DJ_NOTIFY_HAS_SHAKE_SAME_KEY");		
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_NOTIFY_HAS_SHAKE_SAME_KEY,	"Already has [Shake] trigger with same key", "STR_ID_MOTION_DJ_NOTIFY_HAS_SAME_KEY");		
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_NOTIFY_HAS_PRESS_SAME_KEY,	"Already has [Key Press] trigger with same key", "STR_ID_MOTION_DJ_NOTIFY_HAS_SAME_KEY");			
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_NOTIFY_EMPTY_TRIGGER,			"Please set trigger first", "STR_ID_MOTION_DJ_NOTIFY_EMPTY_TRIGGER");				
	ADD_APPLICATION_STRING2(STR_ID_MOTION_DJ_NOTIFY_EMPTY_KEY,				"Key is not set", "STR_ID_MOTION_DJ_NOTIFY_EMPTY_KEY");				



	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_APP, CUST_IMG_BASE_PATH"/GameImages/DJ/dj_app.pbm","DJ");
	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_ICON_TRIGGER_NONE, CUST_IMG_BASE_PATH"/GameImages/DJ/dj_icon_none.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_ICON_TRIGGER_AUTO, CUST_IMG_BASE_PATH"/GameImages/DJ/dj_icon_auto.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_ICON_TRIGGER_SHAKE, CUST_IMG_BASE_PATH"/GameImages/DJ/dj_icon_shake.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_ICON_TRIGGER_PRESS, CUST_IMG_BASE_PATH"/GameImages/DJ/dj_icon_press.pbm","DJ");
	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_ICON_KEY_DUMMY , CUST_IMG_BASE_PATH"/GameImages/DJ/dj_key_dummy.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_ICON_KEY_0, CUST_IMG_BASE_PATH"/GameImages/DJ/dj_key_0.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_ICON_KEY_1, CUST_IMG_BASE_PATH"/GameImages/DJ/dj_key_1.pbm","DJ");			
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_ICON_KEY_2, CUST_IMG_BASE_PATH"/GameImages/DJ/dj_key_2.pbm","DJ");			
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_ICON_KEY_3, CUST_IMG_BASE_PATH"/GameImages/DJ/dj_key_3.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_ICON_KEY_4, CUST_IMG_BASE_PATH"/GameImages/DJ/dj_key_4.pbm","DJ");			
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_ICON_KEY_5, CUST_IMG_BASE_PATH"/GameImages/DJ/dj_key_5.pbm","DJ");			
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_ICON_KEY_6, CUST_IMG_BASE_PATH"/GameImages/DJ/dj_key_6.pbm","DJ");			
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_ICON_KEY_7, CUST_IMG_BASE_PATH"/GameImages/DJ/dj_key_7.pbm","DJ");		
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_ICON_KEY_8, CUST_IMG_BASE_PATH"/GameImages/DJ/dj_key_8.pbm","DJ");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_ICON_KEY_9 , CUST_IMG_BASE_PATH"/GameImages/DJ/dj_key_9.pbm","DJ");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_ICON_KEY_STAR , CUST_IMG_BASE_PATH"/GameImages/DJ/dj_key_star.pbm","DJ");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_ICON_KEY_POUND , CUST_IMG_BASE_PATH"/GameImages/DJ/dj_key_pound.pbm","DJ");	

	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_BG,			CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_bg.gif","DJ");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_NONE, 		CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_none.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_AUTO, 		CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_auto.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_AUTO_SEL, 	CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_auto_sel.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_SHAKE, 		CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_shake.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_SHAKE_SEL, 	CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_shake_sel.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_PRESS, 		CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_press.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_PRESS_SEL, 	CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_press_sel.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_0, 		CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_0.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_0_SEL, 	CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_0_sel.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_1, 		CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_1.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_1_SEL,	CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_1_sel.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_2,	 	CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_2.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_2_SEL, 	CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_2_sel.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_3, 		CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_3.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_3_SEL, 	CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_3_sel.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_4, 		CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_4.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_4_SEL, 	CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_4_sel.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_5, 		CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_5.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_5_SEL, 	CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_5_sel.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_6, 		CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_6.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_6_SEL, 	CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_6_sel.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_7, 		CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_7.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_7_SEL, 	CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_7_sel.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_8, 		CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_8.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_8_SEL, 	CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_8_sel.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_9, 		CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_9.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_9_SEL, 	CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_9_sel.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_STAR, 		CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_star.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_STAR_SEL, 	CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_star_sel.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_POUND, 		CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_pound.pbm","DJ");	
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_DJ_PLAY_KEY_POUND_SEL, CUST_IMG_BASE_PATH"/GameImages/DJ/dj_play_key_pound_sel.pbm","DJ");	

	/* App Menu */
	ADD_APPLICATION_MENUITEM((	MENU_ID_MOTION_DJ_APP, 
										MENU_ID_MOTION_APP,
										2,
										MENU_ID_MOTION_DJ_START,
										MENU_ID_MOTION_DJ_SETTING,
										SHOW,
										NONMOVEABLE,
										DISP_LIST,
										STR_ID_MOTION_DICE_APP,
										IMG_ID_MOTION_DICE_APP));

	ADD_APPLICATION_MENUITEM((MENU_ID_MOTION_DJ_START, MENU_ID_MOTION_DJ_APP,0,
					SHOW,NONMOVEABLE,1,STR_ID_MOTION_DJ_START,0));

	ADD_APPLICATION_MENUITEM((MENU_ID_MOTION_DJ_SETTING, MENU_ID_MOTION_DJ_APP,0,
					SHOW,NONMOVEABLE,1,STR_ID_MOTION_DJ_SETTING,0));

}
#endif /* IS_DJ_BASE_MOTIONGAME */



/*****************************************************************************
* FUNCTION
*	 PopulateMotionAppRes()
* DESCRIPTION
*   populate motion app's resource
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void PopulateMotionAppRes(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	ADD_APPLICATION_STRING2(STR_ID_MOTION_APP, "Motion Games","STR_ID_MOTION_APP");
	ADD_APPLICATION_IMAGE2(IMG_ID_MOTION_APP,CUST_IMG_PATH"/MainLCD/SubMenu/FunANDGame/SB_MOTION.PBM","IMG_ID_MOTION_APP");


	#ifdef IS_DICE_BASE_MOTIONGAME
		PopulateMotionDiceRes();
	#endif /* IS_DICE_BASE_MOTIONGAME */

	#ifdef IS_DJ_BASE_MOTIONGAME
		PopulateMotionDJRes();
	#endif /* IS_DJ_BASE_MOTIONGAMEs */
	
}

#endif /* __MMI_MOTION_APP__ */


#ifdef __MMI_3D_GAME__

#include "FunAndGamesDefs.h"
#include "GameDefs.h"
#include "PopulateRes.h"
#include "CustResDef.h"

/*****************************************************************************
* FUNCTION
*	 Populate3DGameRes()
* DESCRIPTION
*   populate 3d game's resource
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void Populate3DGameRes(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/		
	ADD_APPLICATION_STRING2(STR_ID_3D_GAME, "3D Game","STR_ID_3D_GAME");
	ADD_APPLICATION_STRING2(STR_ID_3D_GAME_TERMINATE_JAVA, 
	                        "Insufficient memory. Terminate Java first.",
	                        "STR_ID_3D_GAME_TERMINATE_JAVA");

	
	ADD_APPLICATION_IMAGE2(
		IMG_ID_3D_GAME,
		CUST_IMG_PATH"/MainLCD/SubMenu/FunANDGame/SB_3DGAME.PBM",
		"IMG_ID_3D_GAME");

	 
#if defined(__MMI_3D_GAME_BROGENT_GGR2_240x320__) || defined(__MMI_3D_GAME_BROGENT_GGR2_176x220__)
	ADD_APPLICATION_STRING2(
		STR_ID_3D_GAME_BROGENT_GOGOROBOT, 
		"GoGoRobot",
		"STR_ID_3D_GAME_BROGENT_GOGOROBOT");
	
	ADD_APPLICATION_IMAGE2(
		IMG_ID_3D_GAME_BROGENT_GOGOROBOT,
		CUST_IMG_PATH"/MainLCD/Submenu/Funandgame/3DGame/GoGoRobot.pbm",		
		"IMG_ID_3D_GAME_BROGENT_GOGOROBOT");
#endif /* defined(__MMI_3D_GAME_BROGENT_GGR2_240x320__) || defined(__MMI_3D_GAME_BROGENT_GGR2_176x220__) */


	 
#if defined(__MMI_3D_GAME_BROGENT_HB2_240x320__) || defined(__MMI_3D_GAME_BROGENT_HB2_176x220__) 
	ADD_APPLICATION_STRING2(
		STR_ID_3D_GAME_BROGENT_HEATBREAKER2, 
		"HeatBreaker",
		"STR_ID_3D_GAME_BROGENT_HEATBREAKER2");
	
	ADD_APPLICATION_IMAGE2(
		IMG_ID_3D_GAME_BROGENT_HEATBREAKER2,
		CUST_IMG_PATH"/MainLCD/Submenu/Funandgame/3DGame/HeatBreaker2.pbm",
		"IMG_ID_3D_GAME_BROGENT_HEATBREAKER2");
#endif /* defined(__MMI_3D_GAME_BROGENT_HB2_240x320__) || defined(__MMI_3D_GAME_BROGENT_HB2_176x220__)  */


#if defined(__MMI_3D_GAME_INTERGRAFX_DEMOGAME_240x320__) || defined(__MMI_3D_GAME_INTERGRAFX_DEMOGAME_176x220__) 
	ADD_APPLICATION_STRING2(
		STR_ID_3D_GAME_INTERGRAFX_DEMOGAME, 
		"DemoGame",
		"STR_ID_3D_GAME_INTERGRAFX_DEMOGAME");
	
	ADD_APPLICATION_IMAGE2(
		IMG_ID_3D_GAME_INTERGRAFX_DEMOGAME,
		CUST_IMG_PATH"/MainLCD/Submenu/Funandgame/3DGame/DemoGame.pbm",
		"IMG_ID_3D_GAME_INTERGRAFX_DEMOGAME");
#endif /* defined(__MMI_3D_GAME_BROGENT_HB2_240x320__) || defined(__MMI_3D_GAME_BROGENT_HB2_176x220__)  */

#if defined(__MMI_3D_GAME_INTERGRAFX_PETFISH_240x320__) || defined(__MMI_3D_GAME_INTERGRAFX_PETFISH_176x220__) 
	ADD_APPLICATION_STRING2(
		STR_ID_3D_GAME_INTERGRAFX_PETFISH, 
		"PetFish",
		"STR_ID_3D_GAME_INTERGRAFX_PETFISH");
	
	ADD_APPLICATION_IMAGE2(
		IMG_ID_3D_GAME_INTERGRAFX_PETFISH,
		CUST_IMG_PATH"/MainLCD/Submenu/Funandgame/3DGame/PETFISH.pbm",
		"IMG_ID_3D_GAME_INTERGRAFX_PETFISH");
#endif /* defined(__MMI_3D_GAME_INTERGRAFX_PETFISH_240x320__) || defined(__MMI_3D_GAME_INTERGRAFX_PETFISH_176x220__)  */

#if defined(__MMI_3D_GAME_INTERGRAFX_ROLLINGBALL_240x320__) || defined(__MMI_3D_GAME_INTERGRAFX_ROLLINGBALL_176x220__)
	ADD_APPLICATION_STRING2(
		STR_ID_3D_GAME_INTERGRAFX_ROLLINGBALL, 
		"Rolling Ball",
		"STR_ID_3D_GAME_INTERGRAFX_ROLLINGBALL");
	
	ADD_APPLICATION_IMAGE2(
		IMG_ID_3D_GAME_INTERGRAFX_ROLLINGBALL,
		CUST_IMG_PATH"/MainLCD/Submenu/Funandgame/3DGame/ROLLINGBALL.pbm",
		"IMG_ID_3D_GAME_INTERGRAFX_ROLLINGBALL");
#endif /* defined(__MMI_3D_GAME_INTERGRAFX_ROLLINGBALL_240x320__) || defined(__MMI_3D_GAME_INTERGRAFX_ROLLINGBALL_176x220__)*/

	
}

#endif /* __MMI_3D_GAME__ */


