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
 *	 Puzzle.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	 Game Puzzle function.
 *
 * Author:
 * -------
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

#include "mmi_features.h"
#include "gamedefs.h"
#include "mmi_trace.h"
#ifdef IS_PUZZLE_BASE_GAME

#include "gameinc.h"
#include "puzzleprots.h"
#include "puzzledefs.h"

#ifdef __MMI_TOUCH_SCREEN__
#include "wgui_touch_screen.h"
#endif
#include "gui.h"
#include "gui_data_types.h"

#define PUZZLE_TRACE 1

/***************************************************************************** 
* Define
*****************************************************************************/
#define MMI_GX_PUZZLE_BACKGROUND_COLOR	(coolsand_UI_color(71, 71, 71))
#define MMI_GX_PUZZLE_LINE_COLOR			(coolsand_UI_color(235, 235, 235))
#define MMI_GX_PUZZLE_LIGHT_BORDER		(coolsand_UI_color(133, 133, 133))
#define MMI_GX_PUZZLE_DARK_BORDER		(coolsand_UI_color(16, 16, 16))
#define MMI_GX_PUZZLE_EASY					(0)
#define MMI_GX_PUZZLE_NORMAL				(1)
#define MMI_GX_PUZZLE_HARD					(2)

#ifdef __MMI_GAME_PUZZLE_128x160__
	#define MMI_GX_PUZZLE_TIME_POSITION_X	(8)
	#define MMI_GX_PUZZLE_TIME_POSITION_Y	(14)
	#define MMI_GX_PUZZLE_STEP_POSITION_X	(90)
	#define MMI_GX_PUZZLE_STEP_POSITION_Y	(14)
	#define MMI_GX_PUZZLE_LCD_HEIGHT			(160)
	#define MMI_GX_PUZZLE_LCD_WIDTH			(128)
#elif defined(__MMI_GAME_PUZZLE_160x128__)
	#define MMI_GX_PUZZLE_TIME_POSITION_X	(8)
	#define MMI_GX_PUZZLE_TIME_POSITION_Y	(14)
	#define MMI_GX_PUZZLE_STEP_POSITION_X	(90)
	#define MMI_GX_PUZZLE_STEP_POSITION_Y	(14)
	#define MMI_GX_PUZZLE_LCD_HEIGHT			(128)
	#define MMI_GX_PUZZLE_LCD_WIDTH			(160)
#elif defined(__MMI_GAME_PUZZLE_128x128__)
	#define MMI_GX_PUZZLE_TIME_POSITION_X	(24)
	#define MMI_GX_PUZZLE_TIME_POSITION_Y	(49)
	#define MMI_GX_PUZZLE_STEP_POSITION_X	(24)
	#define MMI_GX_PUZZLE_STEP_POSITION_Y	(69)
	#define MMI_GX_PUZZLE_LCD_HEIGHT			(128)
	#define MMI_GX_PUZZLE_STATUS_POS_X		(17)
	#define MMI_GX_PUZZLE_STATUS_POS_Y		(40)
	#define MMI_GX_PUZZLE_STATUS_WIDTH		(90)
	#define MMI_GX_PUZZLE_STATUS_HEIGHT		(44)
	#define MMI_GX_PUZZLE_STATUS_SHADOW		(3)
	#define MMI_GX_PUZZLE_LCD_WIDTH			(128)
#elif defined(__MMI_GAME_PUZZLE_176x220__)
	#define MMI_GX_PUZZLE_TIME_POSITION_X	(14)
	#define MMI_GX_PUZZLE_TIME_POSITION_Y	(63)
	#define MMI_GX_PUZZLE_STEP_POSITION_X	(123)
	#define MMI_GX_PUZZLE_STEP_POSITION_Y	(63)
	#define MMI_GX_PUZZLE_LCD_HEIGHT			(220)
	#define MMI_GX_PUZZLE_LCD_WIDTH			(176)
	#define MMI_GX_PUZZLE_BAR_COLOR			(MMI_GX_PUZZLE_LINE_COLOR)
#elif defined(__MMI_GAME_PUZZLE_220x176__)
	#define MMI_GX_PUZZLE_TIME_POSITION_X	(14)
	#define MMI_GX_PUZZLE_TIME_POSITION_Y	(63)
	#define MMI_GX_PUZZLE_STEP_POSITION_X	(123)
	#define MMI_GX_PUZZLE_STEP_POSITION_Y	(63)
	#define MMI_GX_PUZZLE_LCD_HEIGHT			(220)
	#define MMI_GX_PUZZLE_LCD_WIDTH			(176)
	#define MMI_GX_PUZZLE_BAR_COLOR			(MMI_GX_PUZZLE_LINE_COLOR)	
#elif defined(__MMI_GAME_PUZZLE_240x320__)
	#define MMI_GX_PUZZLE_TIME_POSITION_X	(20)
	#define MMI_GX_PUZZLE_TIME_POSITION_Y	(125) 
	#define MMI_GX_PUZZLE_TIME_TEXT_X	(35)
	#define MMI_GX_PUZZLE_TIME_TEXT_Y	(280) 
	#define MMI_GX_PUZZLE_STEP_POSITION_X	(160)
	#define MMI_GX_PUZZLE_STEP_POSITION_Y	(125) 
	#define MMI_GX_PUZZLE_STEP_TEXT_X	(175)
	#define MMI_GX_PUZZLE_STEP_TEXT_Y	(280) 
	#define MMI_GX_PUZZLE_BAR_COLOR			(MMI_GX_PUZZLE_LINE_COLOR)
	#define MMI_GX_PUZZLE_LCD_HEIGHT			(320)
	#define MMI_GX_PUZZLE_LCD_WIDTH			(240)
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#elif defined(__MMI_GAME_PUZZLE_320x240__)
	#define MMI_GX_PUZZLE_TIME_POSITION_X  (250)
	#define MMI_GX_PUZZLE_TIME_POSITION_Y  (40)
	#define MMI_GX_PUZZLE_TIME_TEXT_X	(263)
	#define MMI_GX_PUZZLE_TIME_TEXT_Y	(48) 
	#define MMI_GX_PUZZLE_STEP_POSITION_X  (250)
	#define MMI_GX_PUZZLE_STEP_POSITION_Y  (180)
	#define MMI_GX_PUZZLE_STEP_TEXT_X	(260)
	#define MMI_GX_PUZZLE_STEP_TEXT_Y	(185) 
	#define MMI_GX_PUZZLE_BAR_COLOR        (MMI_GX_PUZZLE_LINE_COLOR)
	#define MMI_GX_PUZZLE_LCD_HEIGHT       (240)
	#define MMI_GX_PUZZLE_LCD_WIDTH        (320)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#endif

#if defined(__MMI_GAME_PUZZLE_128x160__) || defined(__MMI_GAME_PUZZLE_128x128__)
#define MMI_GX_PUZZLE_ORIGIN_X			(4)
#define MMI_GX_PUZZLE_ORIGIN_Y			(4)
#define MMI_GX_PUZZLE_SIZE					(120)
	#define MMI_GX_PUZZLE_LEFT_BAR_WIDTH	(4)
	#define MMI_GX_PUZZLE_RIGHT_BAR_WIDTH	(4)
	#define MMI_GX_PUZZLE_TOP_BAR_WIDTH		(4)
	#define MMI_GX_PUZZLE_BOTTOM_BAR_WIDTH	(4)
	#define MMI_GX_PUZZLE_DIGIT_WIDTH		(4)
#elif defined(__MMI_GAME_PUZZLE_160x128__)
#define MMI_GX_PUZZLE_ORIGIN_X			(4)
#define MMI_GX_PUZZLE_ORIGIN_Y			(4)
#define MMI_GX_PUZZLE_SIZE					(120)
	#define MMI_GX_PUZZLE_LEFT_BAR_WIDTH	(4)
	#define MMI_GX_PUZZLE_RIGHT_BAR_WIDTH	(4)
	#define MMI_GX_PUZZLE_TOP_BAR_WIDTH		(4)
	#define MMI_GX_PUZZLE_BOTTOM_BAR_WIDTH	(4)
	#define MMI_GX_PUZZLE_DIGIT_WIDTH		(4)
#elif defined(__MMI_GAME_PUZZLE_176x220__)
	#define MMI_GX_PUZZLE_ORIGIN_X			(6)
	#define MMI_GX_PUZZLE_ORIGIN_Y			(6)
	#define MMI_GX_PUZZLE_SIZE					(165)
	#define MMI_GX_PUZZLE_LEFT_BAR_WIDTH	(6)
	#define MMI_GX_PUZZLE_RIGHT_BAR_WIDTH	(5)
	#define MMI_GX_PUZZLE_TOP_BAR_WIDTH		(6)
	#define MMI_GX_PUZZLE_BOTTOM_BAR_WIDTH	(6)
	#define MMI_GX_PUZZLE_DIGIT_WIDTH		(6)

	#define MMI_GX_PUZZLE_SOFTKEY_X (150)
	#define MMI_GX_PUZZLE_SOFTKEY_Y (205)
	#define MMI_GX_PUZZLE_SOFTKEY_WIDTH (20)
	#define MMI_GX_PUZZLE_SOFTKEY_HEIGHT (10)
#elif defined(__MMI_GAME_PUZZLE_220x176__)
	#define MMI_GX_PUZZLE_ORIGIN_X			(6)
	#define MMI_GX_PUZZLE_ORIGIN_Y			(6)
	#define MMI_GX_PUZZLE_SIZE					(165)
	#define MMI_GX_PUZZLE_LEFT_BAR_WIDTH	(6)
	#define MMI_GX_PUZZLE_RIGHT_BAR_WIDTH	(5)
	#define MMI_GX_PUZZLE_TOP_BAR_WIDTH		(6)
	#define MMI_GX_PUZZLE_BOTTOM_BAR_WIDTH	(6)
	#define MMI_GX_PUZZLE_DIGIT_WIDTH		(6)

	#define MMI_GX_PUZZLE_SOFTKEY_X (150)
	#define MMI_GX_PUZZLE_SOFTKEY_Y (205)
	#define MMI_GX_PUZZLE_SOFTKEY_WIDTH (20)
	#define MMI_GX_PUZZLE_SOFTKEY_HEIGHT (10)	
#elif defined(__MMI_GAME_PUZZLE_240x320__)
	#define MMI_GX_PUZZLE_ORIGIN_X			(7)
	#define MMI_GX_PUZZLE_ORIGIN_Y			(7)
	#define MMI_GX_PUZZLE_SIZE					(225)
	#define MMI_GX_PUZZLE_LEFT_BAR_WIDTH	(8)
	#define MMI_GX_PUZZLE_RIGHT_BAR_WIDTH	(7)
	#define MMI_GX_PUZZLE_TOP_BAR_WIDTH		(8)
	#define MMI_GX_PUZZLE_BOTTOM_BAR_WIDTH	(8)
	#define MMI_GX_PUZZLE_DIGIT_WIDTH		(8)

	#define MMI_GX_PUZZLE_SOFTKEY_X (190)
	#define MMI_GX_PUZZLE_SOFTKEY_Y (299)
	#define MMI_GX_PUZZLE_SOFTKEY_WIDTH (40)
	#define MMI_GX_PUZZLE_SOFTKEY_HEIGHT (20)
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#elif defined(__MMI_GAME_PUZZLE_320x240__)
	#define MMI_GX_PUZZLE_ORIGIN_X         (7)
	#define MMI_GX_PUZZLE_ORIGIN_Y         (7)
	#define MMI_GX_PUZZLE_SIZE             (225)
	#define MMI_GX_PUZZLE_LEFT_BAR_WIDTH   (8)
	#define MMI_GX_PUZZLE_RIGHT_BAR_WIDTH  (7)
	#define MMI_GX_PUZZLE_TOP_BAR_WIDTH    (8)
	#define MMI_GX_PUZZLE_BOTTOM_BAR_WIDTH (8)
	#define MMI_GX_PUZZLE_DIGIT_WIDTH      (9)

	#define MMI_GX_PUZZLE_SOFTKEY_X (280)
	#define MMI_GX_PUZZLE_SOFTKEY_Y (220)
	#define MMI_GX_PUZZLE_SOFTKEY_WIDTH (40)
	#define MMI_GX_PUZZLE_SOFTKEY_HEIGHT (20)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
#endif


/***************************************************************************** 
* Typedef 
*****************************************************************************/
/* game context */
typedef struct {
	S16		x;
	S16		y;
} gx_puzzle_position_struct;

typedef struct {
	BOOL							is_gameover;
	BOOL							is_new_game;
	U8								game_level;	
	S16							game_grade;
	U16							timer_elapse;
	U16							step;
	U16							tick;
	U8								softkey_state;
	U16							click;
	S16							destination_x;
	S16							destination_y;
	S16							puzzle[5][5];
	UI_image_ID_type			image;
	UI_image_ID_type			small_image;
	gx_puzzle_position_struct	null_grid;
	
	#ifdef __MMI_GAME_PUZZLE_128x128__
	BOOL								is_show_status;
	#endif

	#if defined (__MMI_GAME_MULTICHANNEL_SOUND__)
		S32										move_midi;
	#endif

} gx_puzzle_context_struct;


/***************************************************************************** 
* Local Variable
*****************************************************************************/
gx_puzzle_context_struct g_gx_puzzle_context =
{
	FALSE,		/* is_gameover */
	TRUE,			/* is_new_game */
	0,				/* game_level */
	0,				/* game_grade */
	100,			/* timer_elapse */
	0,				/* how many steps user went */
	0,				/* global tick */
	0,				/* softkey state */
	0				/* user clicks */
};


/***************************************************************************** 
* Local Function
*****************************************************************************/
/* Game framework related functions */
S16	mmi_gx_puzzle_calc_best_grade(S16 old_grade, S16 new_grade);	/* descide which is best grade */
void	mmi_gx_puzzle_enter_game(void);										/* entry function of the game */
void	mmi_gx_puzzle_exit_game(void);											/* exit function - usually will stop timer and release buffer */
void	mmi_gx_puzzle_draw_gameover(void);									/* draw gameover screen */
	
	
/* Game play functions */
void	mmi_gx_puzzle_framemove(void);
void	mmi_gx_puzzle_render(void);
void	mmi_gx_puzzle_gameover(void);
void	mmi_gx_puzzle_cyclic_timer(void);
void	mmi_gx_puzzle_init_game(void);											/* draw gameover screen */	
void	mmi_gx_puzzle_draw_static_background(void);
void	mmi_gx_puzzle_key_2_release(void);
void	mmi_gx_puzzle_key_4_release(void);
void	mmi_gx_puzzle_key_6_release(void);
void	mmi_gx_puzzle_key_8_release(void);
#ifdef __MMI_GAME_PUZZLE_128x128__
	void	mmi_gx_puzzle_key_5_press(void);
	void	mmi_gx_puzzle_key_5_release(void);
#endif
void	mmi_gx_puzzle_show_puzzle(void);
void	mmi_gx_puzzle_move_and_show(void);
void	mmi_gx_puzzle_my_show_image(long x, long y, S16 input_image_id);
void	mmi_gx_puzzle_draw_seperation_line(void);
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_GAME_PUZZLE_128x160__) ||defined(__MMI_GAME_PUZZLE_160x128__) || defined(__MMI_GAME_PUZZLE_176x220__) ||defined(__MMI_GAME_PUZZLE_240x320__)||defined(__MMI_GAME_PUZZLE_220x176__)||defined(__MMI_GAME_PUZZLE_320x240__)
/*- zhouqin 2011-02-16 modify for q6 320X240 */
	void	mmi_gx_puzzle_draw_step(void);
#endif

#ifdef __MMI_TOUCH_SCREEN__
/* touch screen */
void mmi_puzzle_pen_down_hdlr(mmi_pen_point_struct pos);
void mmi_puzzle_pen_up_hdlr(mmi_pen_point_struct pos);
void mmi_puzzle_pen_down_hdlr1(mmi_pen_point_struct pos);

#endif

/************************************************************************/
/* Audio [Const]																			*/
/************************************************************************/
#ifdef __MMI_GAME_MULTICHANNEL_SOUND__	
#define	PUZZLEMOVE	138
 __align(2) const U8 PuzzleMove[138] = {
 0x4D, 0x54, 0x68, 0x64, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x03, 0xC0, 0x4D, 0x54, 0x72, 
 0x6B, 0x00, 0x00, 0x00, 0x74, 0x00, 0xFF, 0x02, 0x16, 0x43, 0x6F, 0x70, 0x79, 0x72, 0x69, 0x67, 
 0x68, 0x74, 0x20, 0x63, 0x20, 0x32, 0x30, 0x30, 0x33, 0x20, 0x62, 0x79, 0x20, 0x43, 0x43, 0x00, 
 0xFF, 0x01, 0x02, 0x43, 0x43, 0x00, 0xFF, 0x58, 0x04, 0x04, 0x02, 0x18, 0x08, 0x00, 0xFF, 0x59, 
 0x02, 0x00, 0x00, 0x00, 0xFF, 0x51, 0x03, 0x05, 0x24, 0xB7, 0x00, 0xC0, 0x0A, 0x00, 0xB0, 0x07, 
 0x60, 0x00, 0x90, 0x48, 0x64, 0x00, 0xC1, 0x61, 0x00, 0xB1, 0x07, 0x7C, 0x00, 0x91, 0x48, 0x64, 
 0x81, 0x70, 0x90, 0x48, 0x00, 0x00, 0x4A, 0x64, 0x81, 0x70, 0x4A, 0x00, 0x00, 0x4C, 0x64, 0x81, 
 0x70, 0x4C, 0x00, 0x00, 0x4F, 0x64, 0x81, 0x70, 0x4F, 0x00, 0x00, 0x54, 0x64, 0x81, 0x70, 0x54, 
 0x00, 0x00, 0x91, 0x48, 0x00, 0x00, 0xFF, 0x2F, 0x00
 };


#else
#define	PUZZLEMOVE_DVI	890

__align(2) const U8 PuzzleMove_dvi[890] = {
   0x77,0xD7,0x77,0x77,0x0E,0x08,0xB7,0xB7,0x08,0x17,0x8C,0x10,0x88,0xA8,0x89,0x18,
   0x32,0x02,0xB8,0xAD,0x8A,0x42,0x33,0x92,0xCC,0xAB,0x28,0x44,0x13,0xB8,0xBD,0x9A,
   0x42,0x34,0x81,0xDA,0xBB,0x18,0x44,0x13,0xA0,0xDC,0x8A,0x30,0x53,0x82,0xC9,0xBB,
   0x09,0x53,0x24,0xA0,0xDA,0xAA,0x20,0x34,0x23,0xC9,0xAD,0x8A,0x41,0x24,0x81,0xC9,
   0xAC,0x19,0x52,0x13,0x90,0xDB,0xAA,0x18,0x54,0x12,0xA8,0xBC,0x9A,0x30,0x25,0x23,
   0xA9,0xCD,0x8B,0x30,0x35,0x02,0xB8,0xAD,0x9B,0x30,0x35,0x04,0xA8,0xCC,0xA9,0x30,
   0x53,0x13,0x99,0xCC,0xAA,0x28,0x44,0x13,0x90,0xBC,0xCB,0x19,0x72,0x22,0x90,0xBA,
   0xCB,0x89,0x52,0x34,0x80,0xC9,0xBB,0x0B,0x51,0x24,0x03,0xB9,0xAE,0x9A,0x21,0x34,
   0x33,0xA8,0xBF,0x9A,0x10,0x43,0x34,0x98,0xBD,0xAA,0x08,0x34,0x25,0x00,0xAC,0xAC,
   0x09,0x32,0x26,0x82,0xCA,0xBB,0x98,0x40,0x45,0x00,0x99,0xBB,0x8D,0x48,0x42,0x11,
   0x98,0xBC,0xAB,0x38,0x54,0x12,0x80,0xEB,0x9B,0x10,0x31,0x35,0x91,0xBC,0xBA,0x8A,
   0x44,0x24,0x82,0xCA,0xCB,0x8A,0x61,0x22,0x11,0xB8,0xBE,0x88,0x10,0x44,0x02,0xA8,
   0xDB,0xAA,0x20,0x44,0x03,0x80,0xDC,0x8B,0x00,0x42,0x15,0x98,0xA9,0xBC,0x08,0x43,
   0x24,0x00,0xD9,0xAC,0x80,0x30,0x26,0x80,0xA8,0xBC,0x0A,0x21,0x26,0x12,0xC9,0xAB,
   0xAB,0x68,0x14,0x21,0xB0,0xAD,0xA9,0x28,0x44,0x12,0x90,0xEB,0x9B,0x01,0x51,0x14,
   0x88,0xC9,0x9C,0x08,0x42,0x32,0x91,0xBB,0xDC,0x19,0x12,0x54,0x91,0x99,0xBC,0x89,
   0x41,0x33,0x13,0xBC,0xDA,0x8C,0x12,0x62,0x82,0x98,0xCB,0x9A,0x40,0x22,0x16,0x9A,
   0xC8,0x8C,0x00,0x43,0x12,0xA0,0xCA,0x9D,0x10,0x50,0x03,0x10,0xCC,0xA9,0x08,0x42,
   0x14,0x00,0xF9,0x89,0x8A,0x33,0x41,0x81,0xC9,0xAB,0x8B,0x25,0x61,0x91,0x98,0xBC,
   0x88,0x30,0x44,0x81,0xB1,0x9F,0xA8,0x31,0x22,0x14,0xA9,0x9D,0xAB,0x22,0x71,0x93,
   0x81,0xAD,0xA9,0x10,0x34,0x21,0xE3,0x8A,0x9D,0x01,0x41,0x12,0x90,0xE9,0x0A,0x1B,
   0x05,0x32,0x98,0xC9,0xBB,0x38,0x68,0x85,0x01,0xAB,0xCA,0x08,0x32,0x63,0x90,0xC0,
   0x8C,0x8A,0x14,0x32,0x82,0x9C,0xFB,0x00,0x59,0x01,0x82,0xB8,0xAA,0x1D,0x81,0x07,
   0x20,0x9A,0xAA,0x9A,0x23,0x72,0x00,0xD1,0xA8,0x8A,0x30,0x52,0x92,0xA1,0x0F,0x8B,
   0x01,0x33,0x84,0x08,0x9F,0xB8,0x02,0x52,0x01,0x88,0x9D,0xC8,0x02,0x51,0x10,0x88,
   0xAC,0xC8,0x02,0x61,0x00,0xA1,0xAA,0xD9,0x82,0x62,0x28,0x98,0xB9,0xBA,0xA3,0x36,
   0x58,0x89,0xC9,0x99,0x80,0x86,0x13,0x2A,0xAC,0xA9,0x29,0x13,0x87,0x93,0x8C,0x8C,
   0x18,0x60,0x10,0xB1,0xD1,0x99,0x08,0x23,0x62,0x2A,0xAC,0xB8,0x18,0x22,0x97,0xA4,
   0xA8,0x8B,0x19,0x52,0x50,0x1A,0xBA,0xC8,0x00,0x31,0x96,0xB4,0xA0,0x9B,0x09,0x15,
   0x42,0x3A,0x8E,0x9A,0x08,0x70,0x28,0x90,0xD0,0x98,0x08,0x02,0x97,0xB3,0xB0,0xA9,
   0x90,0x87,0x83,0x81,0xAA,0xCA,0xA2,0x15,0x32,0x1A,0xBC,0xCA,0x81,0x72,0x30,0x89,
   0xD9,0xA8,0x18,0x70,0x20,0x98,0xC8,0x99,0x39,0x78,0x10,0xA0,0xC8,0x89,0x39,0x78,
   0x10,0xA0,0xC8,0x99,0x38,0x60,0x20,0xA8,0xD9,0xA8,0x20,0x71,0x28,0x89,0xCA,0xA8,
   0x02,0x34,0x30,0x0B,0x9F,0x9A,0x02,0x06,0x02,0x89,0x8D,0x1C,0x28,0x13,0x93,0xD0,
   0xBA,0x09,0x70,0x40,0x08,0xA9,0xBB,0xA0,0x15,0x15,0x00,0x9B,0x0F,0x1A,0x40,0x21,
   0x90,0xE8,0xB8,0x00,0x32,0x43,0x08,0x9E,0x0C,0x29,0x31,0x23,0xB1,0xFA,0xBA,0x01,
   0x24,0x23,0x88,0x9E,0x0C,0x4A,0x40,0x20,0x89,0xBB,0xBA,0x12,0x07,0x84,0xB0,0xD8,
   0x98,0x11,0x23,0x02,0x9A,0x8F,0x2B,0x59,0x40,0x28,0x0C,0x0D,0x2A,0x38,0x40,0x08,
   0x9B,0x9C,0x08,0x33,0x04,0xB1,0xDA,0xA9,0x81,0x06,0x83,0xC2,0xC8,0xA8,0x03,0x06,
   0xA2,0xD8,0xB9,0x80,0x24,0x04,0xA2,0xE9,0xB9,0x00,0x34,0x15,0x91,0xEA,0xAA,0x80,
   0x23,0x25,0x91,0xDA,0xAA,0x08,0x23,0x15,0xA1,0xCA,0x99,0x11,0x24,0x83,0xC9,0xAC,
   0x89,0x25,0x24,0x91,0xEB,0xAA,0x08,0x42,0x25,0x91,0xDA,0xBB,0x38,0x35,0x14,0x90,
   0xDC,0xAA,0x18,0x34,0x24,0x90,0xCC,0xAA,0x18,0x53,0x23,0x91,0xDB,0xAC,0x08,0x52,
   0x32,0x80,0xDA,0xAA,0x0A,0x42,0x24,0x82,0xCA,0xBB,0x8A,0x42,0x44,0x12,0xA8,0xDB,
   0xAB,0x18,0x53,0x23,0x81,0xDB,0xCB,0x88,0x31,0x34,0x03,0xC9,0xDB,0x89,0x20,0x33,
   0x10,0xB9,0xBB,0xAB,0x38,0x34,0x27,0x81,0xA8,0xEB,0xAA,0x09,0x73,0x33,0x93,0xDB,
   0xAD,0x88,0x32,0x43,0x23,0xD9,0xCD,0x89,0x22,0x34,0x03,0xC8,0xDB,0x9B,0x20,0x44,
   0x23,0xA0,0xBE,0x9A,0x00,0x33,0x25,0xA2,0xCB,0xAC,0x08,0x62,0x32,0x80,0xCB,0xCB,
   0x9A,0x63,0x24,0x81,0xBA,0xBC,0x0B,0x51,0x25,0x01,0xB8,0xAE,0x8A,0x41,0x33,0x13,
   0xDA,0xCC,0x89,0x32,0x25,0x82,0xC9,0xBD,0x09,0x43,0x33,0x81,0xEA,0xAC,0x08,0x43,
   0x32,0x81,0xDC,0xAA,0x19,0x44,0x13,0x90,0xCC,0x9B,0x28,0x35,0x03,0xB8,0xBD,0x8B,
   0x42,0x34,0x82,0xCB,0xBC,0x18,0x43,0x33,0xB1,0xCD,0x9A,0x20,0x44,0x02,0xAA,0xBC,
   0x89,0x42,0x24,0x81,0xDB,0x9B,0x20,0x43,0x84,0xA9,0xBC,0x0A,0x43,0x24,0x80,0xCC,
   0x9A,0x20,0x35,0x82,0xB9,0xBD,0x19,0x33,0x34,0x90,0xBD,0xAB,0x40,0x25,0x11,0xD9,
   0x8C,0x88,0x42,0x02,0x90,0xCB,0x9A,0x41,0x43,0x81,0xD9,0xAB,0x18,0x53,0x13,0xA8,
   0xBC,0xAB,0x52,0x23,0x13,0xCC,0xBA,0x1B,0x25,0x34
};
#endif
/******************************************AUDIO************************************/


/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_enter_gfx
* DESCRIPTION
*	Set Game Framework (GFX) Parameter
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	GFX.game_data. - set Game Framwork data in GameFramework.c
*****************************************************************************/

/* keep this game old naming style */
void mmi_gx_puzzle_enter_gfx(void)
{
	/* Game menu */
	GFX.game_data.game_img_id					= IMG_ID_GX_PUZZLE_GAME_ICON;	/* game icon img ID */
	GFX.game_data.game_str_id					= STR_ID_GX_PUZZLE_GAME_NAME;	/* game name string ID */
	GFX.game_data.menu_resume_str_id			= STR_GAME_RESUME;				/* "Resume" string ID */	
	GFX.game_data.menu_new_str_id				= STR_GAME_NEW;					/* "New Game" string ID */
	GFX.game_data.menu_level_str_id			= STR_GAME_LEVEL;					/* "Game Level" string ID */
	GFX.game_data.menu_grade_str_id			= STR_GAME_GRADE;					/* "Best Grade" string ID */
	GFX.game_data.menu_help_str_id			= STR_GAME_HELP;					/* "Game Help" string ID */
	
	/* level / grade */
	GFX.game_data.level_count					= 3;											/* how many levels */
	GFX.game_data.level_str_id_list[0]		= STR_ID_GX_PUZZLE_LEVEL_EASY;		/* level string ID */
	GFX.game_data.level_str_id_list[1]		= STR_ID_GX_PUZZLE_LEVEL_NORMAL;		/* level string ID */
	GFX.game_data.level_str_id_list[2]		= STR_ID_GX_PUZZLE_LEVEL_HARD;		/* level string ID */

	/* add slot in NVRAMEnum.h */
	GFX.game_data.grade_nvram_id_list[0]	= NVRAM_GX_PUZZLE_EASY_SCORE;			/* grade slot in NVRAM (short) */
	GFX.game_data.grade_nvram_id_list[1]	= NVRAM_GX_PUZZLE_NORMAL_SCORE;		/* grade slot in NVRAM */
	GFX.game_data.grade_nvram_id_list[2]	= NVRAM_GX_PUZZLE_HARD_SCORE;			/* grade slot in NVRAM */
	GFX.game_data.level_nvram_id				= NVRAM_GX_PUZZLE_LEVEL;				/* current lvl idnex  in NVRAM (byte) */

	/* help */	
	#if defined(__MMI_TOUCH_SCREEN__)
	GFX.game_data.help_str_id					= STR_ID_GX_PUZZLE_HELP_DESCRIPTION_TP;			/* help desciption string id */
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#elif defined(__MMI_GAME_PUZZLE_128x160__) || defined(__MMI_GAME_PUZZLE_160x128__) ||defined(__MMI_GAME_PUZZLE_176x220__) || defined(__MMI_GAME_PUZZLE_240x320__) ||defined(__MMI_GAME_PUZZLE_220x176__)||defined(__MMI_GAME_PUZZLE_320x240__)
	/*- zhouqin 2011-02-16 modify for q6 320X240 */
	GFX.game_data.help_str_id					= STR_ID_GX_PUZZLE_HELP_DESCRIPTION;			/* help desciption string id */
	#elif defined(__MMI_GAME_PUZZLE_128x128__)
	GFX.game_data.help_str_id					= STR_ID_GX_PUZZLE_HELP_DESCRIPTION_128;		/* help desciption string id */
	#endif

	/* misc */
	GFX.game_data.grade_value_ptr				= (S16*)(&g_gx_puzzle_context.game_grade);	/* current level's grade (S16*) */
	GFX.game_data.level_index_ptr				= (U8*)(&g_gx_puzzle_context.game_level);		/* ptr to current level index (U8*) */
	GFX.game_data.is_new_game					= (BOOL*)(&g_gx_puzzle_context.is_new_game);	/* ptr to new game flag (BOOL*) */ 
	
	/* function ptr */
	GFX.game_data.best_grade_func_ptr		= mmi_gx_puzzle_calc_best_grade;	/* function to calculate best grade */
	GFX.game_data.enter_game_func_ptr		= mmi_gx_puzzle_enter_game;		/* function to enter new game */
	GFX.game_data.exit_game_func_ptr			= mmi_gx_puzzle_exit_game;			/* function to exit game */
	GFX.game_data.draw_gameover_func_ptr	= mmi_gx_puzzle_draw_gameover;	/* function to draw gameover screen */

	/* some flags */
	GFX.game_data.is_keypad_audio_enable = FALSE;					/* play keypad tone or not */

	mmi_gfx_entry_menu_screen();
}


/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_calc_best_grade
* DESCRIPTION
*	Calculate new best grade [Callback required by GFX]	
* PARAMETERS
*	old_grade	IN
*	new_grade	IN
* RETURNS
*	S16
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
S16 mmi_gx_puzzle_calc_best_grade(S16 old_grade, S16 new_grade)
{
	/* compare the best grade and return it */
	if (old_grade == 0)
		return new_grade;

	if (new_grade < old_grade)
	{
		return new_grade;
	}
	else
	{
		return old_grade;
	}
}


/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_draw_gameover
* DESCRIPTION
*	Draw Gameover Screen [Callback required by GFX]	
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mmi_gx_puzzle_draw_gameover(void)
{
  	//GFX_PLAY_AUDIO_COMPLETE();//chenhe ,this api is not managed by mdi
	
	/* call this function to draw standard gameover screen */
	mmi_gfx_draw_gameover_screen(IMG_ID_GX_PUZZLE_GOTEXT, IMG_ID_GX_PUZZLE_GRADESMAP, IMG_ID_GX_PUZZLE_GOPIC, g_gx_puzzle_context.game_grade);
}


/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_enter_game
* DESCRIPTION
*	Enter Game [Callback required by GFX]	
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mmi_gx_puzzle_enter_game(void)
{
	if(g_gx_puzzle_context.is_new_game == TRUE )
		mmi_gx_puzzle_init_game(); /* is new game, otherwise resume game */

	g_gx_puzzle_context.is_new_game = FALSE;
	g_gx_puzzle_context.is_gameover = FALSE;

	#ifdef __MMI_GAME_PUZZLE_128x128__
	g_gx_puzzle_context.is_show_status = FALSE;
	SetKeyHandler(mmi_gx_puzzle_key_5_press, KEY_5, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_gx_puzzle_key_5_release, KEY_5, KEY_EVENT_UP);
	#endif

	SetKeyHandler(mmi_gx_puzzle_key_2_release, KEY_2, KEY_EVENT_UP);
	SetKeyHandler(mmi_gx_puzzle_key_2_release, KEY_UP_ARROW, KEY_EVENT_UP);
	SetKeyHandler(mmi_gx_puzzle_key_4_release, KEY_4, KEY_EVENT_UP);
	SetKeyHandler(mmi_gx_puzzle_key_4_release, KEY_LEFT_ARROW, KEY_EVENT_UP);
	SetKeyHandler(mmi_gx_puzzle_key_6_release, KEY_6, KEY_EVENT_UP);
	SetKeyHandler(mmi_gx_puzzle_key_6_release, KEY_RIGHT_ARROW, KEY_EVENT_UP);
	SetKeyHandler(mmi_gx_puzzle_key_8_release, KEY_8, KEY_EVENT_UP);
	SetKeyHandler(mmi_gx_puzzle_key_8_release, KEY_DOWN_ARROW, KEY_EVENT_UP);
	#ifdef __MMI_GAME_MULTICHANNEL_SOUND__	
			GFX_OPEN_SOUND_EFFECTS_MIDI(PuzzleMove,PUZZLEMOVE,1,g_gx_puzzle_context.move_midi);
	#else
			
			TRACE_EF(g_sw_GAME, "MMI_PUZZLE: GFX.is_aud_on=%d",GFX.is_aud_on);
			if(TRUE == GFX.is_aud_on)
			{
				mdi_audio_stop_all();				//added by jinzh:20070724
				
			}	
	#endif
	
	mmi_gx_puzzle_draw_static_background();
	mmi_gx_puzzle_show_puzzle();
	mmi_gx_puzzle_draw_seperation_line();

	/* start game loop */
	mmi_gx_puzzle_cyclic_timer();

#ifdef __MMI_TOUCH_SCREEN__
		SetKeyHandler(DisplayIdleScreen/*GoBackHistory*/,KEY_END,KEY_EVENT_DOWN);//dongwg modify
		mmi_wgui_register_pen_down_handler(mmi_puzzle_pen_down_hdlr1);
		mmi_wgui_register_pen_up_handler(mmi_puzzle_pen_up_hdlr);		
#endif

}


/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_draw_static_background
* DESCRIPTION
*	Draw the elements that only need to be drawn once
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void	mmi_gx_puzzle_draw_static_background(void)
{
	coolsand_UI_fill_rectangle(	0,
										0,
										MMI_GX_PUZZLE_LCD_WIDTH - 1,
										MMI_GX_PUZZLE_LCD_HEIGHT - 1,
										MMI_GX_PUZZLE_BACKGROUND_COLOR);
	
	#if defined(__MMI_GAME_PUZZLE_128x160__)
	coolsand_UI_show_image(	49,
									129,
									(UI_image_type)GetImage(g_gx_puzzle_context.small_image));
	#elif defined(__MMI_GAME_PUZZLE_160x128__)
	coolsand_UI_show_image(	129,
									49,
									(UI_image_type)GetImage(g_gx_puzzle_context.small_image));
	#elif defined(__MMI_GAME_PUZZLE_176x220__)
	coolsand_UI_show_image(	68,
									178,
									(UI_image_type)GetImage(g_gx_puzzle_context.small_image));
	if (g_gx_puzzle_context.game_level == MMI_GX_PUZZLE_NORMAL)
	{
		coolsand_UI_draw_vertical_line(0, MMI_GX_PUZZLE_ORIGIN_Y + MMI_GX_PUZZLE_SIZE, MMI_GX_PUZZLE_ORIGIN_X + MMI_GX_PUZZLE_SIZE - 1, MMI_GX_PUZZLE_BAR_COLOR);
		coolsand_UI_draw_horizontal_line(0, MMI_GX_PUZZLE_LCD_WIDTH - 1, MMI_GX_PUZZLE_ORIGIN_Y + MMI_GX_PUZZLE_SIZE - 1, MMI_GX_PUZZLE_BAR_COLOR);
	}
	#elif defined(__MMI_GAME_PUZZLE_220x176__)
	coolsand_UI_show_image(	178,
									68,
									(UI_image_type)GetImage(g_gx_puzzle_context.small_image));
	if (g_gx_puzzle_context.game_level == MMI_GX_PUZZLE_NORMAL)
	{
		coolsand_UI_draw_vertical_line(0, MMI_GX_PUZZLE_ORIGIN_X + MMI_GX_PUZZLE_SIZE - 1, MMI_GX_PUZZLE_ORIGIN_Y + MMI_GX_PUZZLE_SIZE, MMI_GX_PUZZLE_BAR_COLOR);
		coolsand_UI_draw_horizontal_line(0, MMI_GX_PUZZLE_ORIGIN_Y + MMI_GX_PUZZLE_SIZE - 1, MMI_GX_PUZZLE_LCD_WIDTH - 1, MMI_GX_PUZZLE_BAR_COLOR);
	}	
	#elif defined(__MMI_GAME_PUZZLE_240x320__)
	coolsand_UI_show_image(	95,
									245,
									(UI_image_type)GetImage(g_gx_puzzle_context.small_image));
	if (g_gx_puzzle_context.game_level == MMI_GX_PUZZLE_NORMAL)
	{
		coolsand_UI_draw_vertical_line(0, MMI_GX_PUZZLE_ORIGIN_Y + MMI_GX_PUZZLE_SIZE, MMI_GX_PUZZLE_ORIGIN_X + MMI_GX_PUZZLE_SIZE - 1, MMI_GX_PUZZLE_BAR_COLOR);
		coolsand_UI_draw_horizontal_line(0, MMI_GX_PUZZLE_LCD_WIDTH - 1, MMI_GX_PUZZLE_ORIGIN_Y + MMI_GX_PUZZLE_SIZE - 1, MMI_GX_PUZZLE_BAR_COLOR);
	}
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#elif defined(__MMI_GAME_PUZZLE_320x240__)
	coolsand_UI_show_image(	255,
									95,
									(UI_image_type)GetImage(g_gx_puzzle_context.small_image));
	if (g_gx_puzzle_context.game_level == MMI_GX_PUZZLE_NORMAL)
	{
		coolsand_UI_draw_vertical_line(0, MMI_GX_PUZZLE_ORIGIN_Y + MMI_GX_PUZZLE_SIZE, MMI_GX_PUZZLE_ORIGIN_X + MMI_GX_PUZZLE_SIZE - 1, MMI_GX_PUZZLE_BAR_COLOR);
		coolsand_UI_draw_horizontal_line(0, MMI_GX_PUZZLE_LCD_WIDTH - 1, MMI_GX_PUZZLE_ORIGIN_Y + MMI_GX_PUZZLE_SIZE - 1, MMI_GX_PUZZLE_BAR_COLOR);
	}
/*- zhouqin 2011-02-16 modify for q6 320X240 */
	#endif

	#if defined(__MMI_TOUCH_SCREEN__) && defined(__MMI_GAME_PUZZLE_240x320__) //|| defined(__MMI_GAME_PUZZLE_176x220__))||defined(__MMI_GAME_PUZZLE_220x176__)
	/*	draw softkay image */
	gdi_image_draw_id(MMI_GX_PUZZLE_SOFTKEY_X,MMI_GX_PUZZLE_SOFTKEY_Y,IMG_ID_GX_PUZZLE_UP);

	#endif
	coolsand_UI_show_image(	MMI_GX_PUZZLE_ORIGIN_X - MMI_GX_PUZZLE_LEFT_BAR_WIDTH,
									MMI_GX_PUZZLE_ORIGIN_Y - MMI_GX_PUZZLE_TOP_BAR_WIDTH,
									(UI_image_type)GetImage(IMG_ID_GX_PUZZLE_TOP_BAR));

	coolsand_UI_show_image(	MMI_GX_PUZZLE_ORIGIN_X - MMI_GX_PUZZLE_LEFT_BAR_WIDTH,
									MMI_GX_PUZZLE_ORIGIN_Y - MMI_GX_PUZZLE_TOP_BAR_WIDTH,
									(UI_image_type)GetImage(IMG_ID_GX_PUZZLE_LEFT_BAR));

	coolsand_UI_show_image(	MMI_GX_PUZZLE_ORIGIN_X - MMI_GX_PUZZLE_LEFT_BAR_WIDTH,
									MMI_GX_PUZZLE_ORIGIN_Y + MMI_GX_PUZZLE_SIZE,
									(UI_image_type)GetImage(IMG_ID_GX_PUZZLE_BOTTOM_BAR));

	coolsand_UI_show_image(	MMI_GX_PUZZLE_ORIGIN_X + MMI_GX_PUZZLE_SIZE,
									MMI_GX_PUZZLE_ORIGIN_Y - MMI_GX_PUZZLE_TOP_BAR_WIDTH,
									(UI_image_type)GetImage(IMG_ID_GX_PUZZLE_RIGHT_BAR));

	coolsand_UI_show_image(	MMI_GX_PUZZLE_ORIGIN_X - MMI_GX_PUZZLE_LEFT_BAR_WIDTH,
									MMI_GX_PUZZLE_ORIGIN_Y - MMI_GX_PUZZLE_TOP_BAR_WIDTH,
									(UI_image_type)GetImage(IMG_ID_GX_PUZZLE_TOP_BAR));
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#if defined(__MMI_GAME_PUZZLE_128x160__) ||defined(__MMI_GAME_PUZZLE_160x128__) || defined(__MMI_GAME_PUZZLE_176x220__) || defined(__MMI_GAME_PUZZLE_240x320__)||defined(__MMI_GAME_PUZZLE_220x176__)||defined(__MMI_GAME_PUZZLE_320x240__)
	/*- zhouqin 2011-02-16 modify for q6 320X240 */
	/* four lines */
	coolsand_UI_draw_horizontal_line(	2,
												MMI_GX_PUZZLE_LCD_WIDTH - 1 - 2,
												MMI_GX_PUZZLE_LCD_WIDTH + 1,
												MMI_GX_PUZZLE_LIGHT_BORDER);
	coolsand_UI_draw_horizontal_line(	2,
												MMI_GX_PUZZLE_LCD_WIDTH - 1 - 2,
												MMI_GX_PUZZLE_LCD_HEIGHT - 1 - 1,
												MMI_GX_PUZZLE_DARK_BORDER);
	coolsand_UI_draw_vertical_line(	MMI_GX_PUZZLE_LCD_WIDTH + 2,
											MMI_GX_PUZZLE_LCD_HEIGHT - 1 - 1,
											1,
											MMI_GX_PUZZLE_LIGHT_BORDER);
	coolsand_UI_draw_vertical_line(	MMI_GX_PUZZLE_LCD_WIDTH + 2,
											MMI_GX_PUZZLE_LCD_HEIGHT - 1 - 1,
											MMI_GX_PUZZLE_LCD_WIDTH - 1 - 1,
											MMI_GX_PUZZLE_DARK_BORDER);
	#ifdef __MMI_MAINLCD_220X176__	
	/*Draw the background of the time or step.*/
	coolsand_UI_show_image(	MMI_GX_PUZZLE_TIME_POSITION_Y + 128-12,
									MMI_GX_PUZZLE_TIME_POSITION_X,
									(UI_image_type)GetImage(IMG_ID_GX_PUZZLE_BOX));
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#elif defined (__MMI_MAINLCD_320X240__)	
	/*Draw the background of the time or step.*/
	coolsand_UI_show_image(	MMI_GX_PUZZLE_TIME_POSITION_X,
									MMI_GX_PUZZLE_TIME_POSITION_Y,
									(UI_image_type)GetImage(IMG_ID_GX_PUZZLE_BOX));
/*- zhouqin 2011-02-16 modify for q6 320X240 */
	#else	
	coolsand_UI_show_image(	MMI_GX_PUZZLE_TIME_POSITION_X,
									MMI_GX_PUZZLE_TIME_POSITION_Y + 128,
									(UI_image_type)GetImage(IMG_ID_GX_PUZZLE_BOX));
	#endif
	
	#ifndef 	__MMI_GAME_PUZZLE_240x320__
	#ifdef __MMI_MAINLCD_220X176__	
	coolsand_UI_show_image(	MMI_GX_PUZZLE_TIME_POSITION_Y - 12 + 128+2,
									MMI_GX_PUZZLE_TIME_POSITION_X+2,									
									(UI_image_type)GetImage(IMG_ID_GX_PUZZLE_TIME));
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#elif defined (__MMI_GAME_PUZZLE_320x240__)	
	coolsand_UI_show_image(	MMI_GX_PUZZLE_TIME_POSITION_X,
									MMI_GX_PUZZLE_TIME_POSITION_Y,											
									(UI_image_type)GetImage(IMG_ID_GX_PUZZLE_TIME));
	/*- zhouqin 2011-02-16 modify for q6 320X240 */
	#else
	coolsand_UI_show_image(	MMI_GX_PUZZLE_TIME_POSITION_X + 2,
									#if defined(__MMI_GAME_PUZZLE_128x160__)
									MMI_GX_PUZZLE_TIME_POSITION_Y - 8 + 128,
									#elif defined(__MMI_GAME_PUZZLE_160x128__)
									MMI_GX_PUZZLE_TIME_POSITION_Y - 8 + 128,
									#elif defined(__MMI_GAME_PUZZLE_176x220__)
									MMI_GX_PUZZLE_TIME_POSITION_Y - 12 + 128,
									#elif defined(__MMI_GAME_PUZZLE_220x176__)	
									MMI_GX_PUZZLE_TIME_POSITION_Y - 12 + 128,									
									#endif
									(UI_image_type)GetImage(IMG_ID_GX_PUZZLE_TIME));
	#endif
	#endif
	
	#ifdef __MMI_MAINLCD_220X176__	
	/*Draw the background of the time or step.*/
	coolsand_UI_show_image(	MMI_GX_PUZZLE_STEP_POSITION_Y + 128-12,
									MMI_GX_PUZZLE_STEP_POSITION_X,
									(UI_image_type)GetImage(IMG_ID_GX_PUZZLE_BOX));
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#elif defined (__MMI_MAINLCD_320X240__)	
	/*Draw the background of the time or step.*/
	coolsand_UI_show_image(	MMI_GX_PUZZLE_STEP_POSITION_X,
									MMI_GX_PUZZLE_STEP_POSITION_Y,
									(UI_image_type)GetImage(IMG_ID_GX_PUZZLE_BOX));
/*- zhouqin 2011-02-16 modify for q6 320X240 */
	#else
	coolsand_UI_show_image(	MMI_GX_PUZZLE_STEP_POSITION_X,
									MMI_GX_PUZZLE_STEP_POSITION_Y + 128,
									(UI_image_type)GetImage(IMG_ID_GX_PUZZLE_BOX));
	#endif

	#ifndef 	__MMI_GAME_PUZZLE_240x320__
	#ifdef __MMI_MAINLCD_220X176__	
	coolsand_UI_show_image(	MMI_GX_PUZZLE_STEP_POSITION_Y - 12 + 128+2,	
									MMI_GX_PUZZLE_STEP_POSITION_X+2,									
									(UI_image_type)GetImage(IMG_ID_GX_PUZZLE_STEP));

	/*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#elif defined ( __MMI_MAINLCD_320X240__)	
	coolsand_UI_show_image(	MMI_GX_PUZZLE_STEP_POSITION_X,	
									MMI_GX_PUZZLE_STEP_POSITION_Y,									
									(UI_image_type)GetImage(IMG_ID_GX_PUZZLE_STEP));
	/*- zhouqin 2011-02-16 modify for q6 320X240 */
	#else
	coolsand_UI_show_image(	MMI_GX_PUZZLE_STEP_POSITION_X + 2,
									#if defined(__MMI_GAME_PUZZLE_128x160__)
									MMI_GX_PUZZLE_STEP_POSITION_Y - 8 + 128,
									#elif defined(__MMI_GAME_PUZZLE_160x128__)
									MMI_GX_PUZZLE_STEP_POSITION_Y - 8 + 128,
									#elif defined(__MMI_GAME_PUZZLE_176x220__)
									MMI_GX_PUZZLE_STEP_POSITION_Y - 12 + 128,
									#elif defined(__MMI_GAME_PUZZLE_220x176__)		
									MMI_GX_PUZZLE_STEP_POSITION_Y - 12 + 128,									
									#endif
									(UI_image_type)GetImage(IMG_ID_GX_PUZZLE_STEP));
	#endif
	#endif
	
	mmi_gx_puzzle_draw_step();
	#endif
}


/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_exit_game
* DESCRIPTION
*	Exit Game [Callback required by GFX]	
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mmi_gx_puzzle_exit_game(void)
{
	#ifdef __MMI_GAME_MULTICHANNEL_SOUND__	
		GFX_STOP_SOUND_EFFECTS_MIDI(g_gx_puzzle_context.move_midi);
		GFX_CLOSE_SOUND_EFFECTS_MIDI(g_gx_puzzle_context.move_midi);
	#else
		TRACE_EF(g_sw_GAME, "MMI_PUZZLE: GFX.is_aud_on=%d",GFX.is_aud_on);
		if(TRUE == GFX.is_aud_on)
		{
				mdi_audio_stop_all();				//added by jinzh:20070724
		}
	#endif

	coolsand_UI_cancel_timer(mmi_gx_puzzle_cyclic_timer);	/* cancle the looping timer */
}


/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_draw_seperation_line
* DESCRIPTION
	Draw seperation lines in the puzzle
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mmi_gx_puzzle_draw_seperation_line(void)
{
	U8		i;
	U8		grid_number;

	grid_number = g_gx_puzzle_context.game_level + 3;
	/* draw the seperation line */
	for (i = 1; i < grid_number; i++)
	{
		coolsand_UI_draw_vertical_line(MMI_GX_PUZZLE_ORIGIN_Y, MMI_GX_PUZZLE_ORIGIN_Y + MMI_GX_PUZZLE_SIZE - 1, MMI_GX_PUZZLE_ORIGIN_X + i * MMI_GX_PUZZLE_SIZE / (g_gx_puzzle_context.game_level + 3) - 1, MMI_GX_PUZZLE_LINE_COLOR);
		coolsand_UI_draw_horizontal_line(MMI_GX_PUZZLE_ORIGIN_X, MMI_GX_PUZZLE_ORIGIN_X + MMI_GX_PUZZLE_SIZE - 1, MMI_GX_PUZZLE_ORIGIN_Y + i * MMI_GX_PUZZLE_SIZE / (g_gx_puzzle_context.game_level + 3) - 1, MMI_GX_PUZZLE_LINE_COLOR);
	}
	coolsand_UI_BLT_double_buffer(0,0, UI_device_width - 1, UI_device_height -1);
}


/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_show_puzzle
* DESCRIPTION
	Draw all grids in the puzzle
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mmi_gx_puzzle_show_puzzle(void)
{
	U8						grid_number;
	U8						i;
	U8						j;

	grid_number = g_gx_puzzle_context.game_level + 3;
	
	for (i = 0; i < grid_number; i++)
	{
		for (j = 0; j < grid_number; j++)
		{
			mmi_gx_puzzle_my_show_image(	MMI_GX_PUZZLE_ORIGIN_X + MMI_GX_PUZZLE_SIZE / grid_number * j,
													MMI_GX_PUZZLE_ORIGIN_Y + MMI_GX_PUZZLE_SIZE / grid_number * i,
													g_gx_puzzle_context.puzzle[i][j]);
		}
	}

	coolsand_UI_BLT_double_buffer(0,0, UI_device_width - 1, UI_device_height - 1);
}


/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_init_game
* DESCRIPTION
	Game initilization
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mmi_gx_puzzle_init_game(void)
{
	/* init game */

	U16	i = 0;
	U16	j = 0;
	U8		bound_size = 0;			/* height & width of the puzzle */
	U8		index = 0;
	U16	shuffle_times = 0;		/* shuffle for 'shuffle_times' times */
	U8		move_type = 0;
	S16	destination_x = 0;
	S16	destination_y = 0;
	S16	tmp = 0;

	g_gx_puzzle_context.tick = 0;
	g_gx_puzzle_context.step = 0;
	g_gx_puzzle_context.click = 0;

	#ifdef __MMI_GAME_PUZZLE_128x128__
	g_gx_puzzle_context.is_show_status = FALSE;
	#endif

	/* select the image for corresponding game level */
	tmp = g_gx_puzzle_context.game_level;
	if (g_gx_puzzle_context.game_level == MMI_GX_PUZZLE_EASY)
	{
		g_gx_puzzle_context.image = IMG_ID_GX_PUZZLE_3_1;
	}
	else
	{
		g_gx_puzzle_context.image = IMG_ID_GX_PUZZLE_3_1 + tmp * (tmp + 2) * (tmp + 2) / tmp;
	}
	g_gx_puzzle_context.small_image = IMG_ID_GX_PUZZLE_SMALL_3 + tmp;

	/* init puzzle grids */
	if (g_gx_puzzle_context.game_level == MMI_GX_PUZZLE_EASY)
	{
		bound_size = 3;
	}
	else if (g_gx_puzzle_context.game_level == MMI_GX_PUZZLE_NORMAL)
	{
		bound_size = 4;
	}
	else if (g_gx_puzzle_context.game_level == MMI_GX_PUZZLE_HARD)
	{
		bound_size = 5;
	}

	shuffle_times = bound_size * bound_size * 100;

	index = 1;

	for (i = 0; i < bound_size; i++)
	{
		for (j = 0; j < bound_size; j++)
		{
			g_gx_puzzle_context.puzzle[i][j] = index;
			index = index + 1;
		}
	}

	g_gx_puzzle_context.null_grid.y = i - 1;
	g_gx_puzzle_context.null_grid.x = j - 1;

	/* shuffle */
	for (i = 0; i < shuffle_times; i++)
	{
		move_type = rand() % 4;

		switch (move_type)
		{
		case 0:					/* up */
			destination_x = 0;
			destination_y = -1;
			break;
		case 1:					/* left */
			destination_x = -1;
			destination_y = 0;
			break;
		case 2:					/* bottom */
			destination_x = 0;
			destination_y = 1;
			break;
		case 3:					/* right */
			destination_x = 1;
			destination_y = 0;
			break;
		}

		/* check if this shuffle is valid */
		if (	g_gx_puzzle_context.null_grid.x + destination_x < 0 || 
				g_gx_puzzle_context.null_grid.x + destination_x >= bound_size ||
				g_gx_puzzle_context.null_grid.y + destination_y < 0 ||
				g_gx_puzzle_context.null_grid.y + destination_y >= bound_size)
		{
			i--;
			continue;
		}

		/* switch the null grid and its neighboring grid */
		tmp = g_gx_puzzle_context.puzzle[g_gx_puzzle_context.null_grid.y][g_gx_puzzle_context.null_grid.x];
		g_gx_puzzle_context.puzzle[g_gx_puzzle_context.null_grid.y][g_gx_puzzle_context.null_grid.x] = g_gx_puzzle_context.puzzle[g_gx_puzzle_context.null_grid.y + destination_y][g_gx_puzzle_context.null_grid.x + destination_x];
		g_gx_puzzle_context.puzzle[g_gx_puzzle_context.null_grid.y + destination_y][g_gx_puzzle_context.null_grid.x + destination_x] = tmp;

		g_gx_puzzle_context.null_grid.x = g_gx_puzzle_context.null_grid.x + destination_x;
		g_gx_puzzle_context.null_grid.y = g_gx_puzzle_context.null_grid.y + destination_y;
	}
}


/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_framemove
* DESCRIPTION
*	Framemove - process the game logic	
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mmi_gx_puzzle_framemove(void)
{
	/* add logic code here */
}


/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_render
* DESCRIPTION
*	Render the game images		
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mmi_gx_puzzle_render(void)
{
	/* add drawing code here */

	U16					tmp_tick;
	#ifdef __MMI_GAME_PUZZLE_128x128__
	U16					tmp_click;
	#endif
	S16					position_shift;		/* the position offset of digit displayed */
	UI_image_ID_type	image_id;				/* store the last digit of tick */
	#if defined(__MMI_TOUCH_SCREEN__) && defined(__MMI_GAME_PUZZLE_240x320__)      //modified by kecx for project cs2324c on 20090601
		/* softkey */
		if(g_gx_puzzle_context.softkey_state == 1)
		{
			#if defined(__MMI_GAME_PUZZLE_240x320__)
			//gdi_draw_solid_rect(MMI_GX_PUZZLE_SOFTKEY_X,MMI_GX_PUZZLE_SOFTKEY_Y,MMI_GX_PUZZLE_SOFTKEY_X+MMI_GX_PUZZLE_SOFTKEY_WIDTH,MMI_GX_PUZZLE_SOFTKEY_Y+MMI_GX_PUZZLE_SOFTKEY_HEIGHT,GDI_COLOR_TRANSPARENT);
			#elif defined(__MMI_GAME_PUZZLE_176x220__)
			coolsand_UI_fill_rectangle(MMI_GX_PUZZLE_SOFTKEY_X,MMI_GX_PUZZLE_SOFTKEY_Y,MMI_GX_PUZZLE_SOFTKEY_X+MMI_GX_PUZZLE_SOFTKEY_WIDTH,MMI_GX_PUZZLE_SOFTKEY_Y+MMI_GX_PUZZLE_SOFTKEY_HEIGHT,MMI_GX_PUZZLE_BACKGROUND_COLOR);
			#elif defined(__MMI_GAME_PUZZLE_220x176__)
			coolsand_UI_fill_rectangle(MMI_GX_PUZZLE_SOFTKEY_X,MMI_GX_PUZZLE_SOFTKEY_Y,MMI_GX_PUZZLE_SOFTKEY_X+MMI_GX_PUZZLE_SOFTKEY_WIDTH,MMI_GX_PUZZLE_SOFTKEY_Y+MMI_GX_PUZZLE_SOFTKEY_HEIGHT,MMI_GX_PUZZLE_BACKGROUND_COLOR);		
			#endif

			gdi_image_draw_id(MMI_GX_PUZZLE_SOFTKEY_X,MMI_GX_PUZZLE_SOFTKEY_Y,IMG_ID_GX_PUZZLE_DOWN);
			g_gx_puzzle_context.softkey_state = 0;
		}
		else if (g_gx_puzzle_context.softkey_state == 2)
		{
			#if defined(__MMI_GAME_PUZZLE_240x320__)
			//gdi_draw_solid_rect(MMI_GX_PUZZLE_SOFTKEY_X,MMI_GX_PUZZLE_SOFTKEY_Y,MMI_GX_PUZZLE_SOFTKEY_X+MMI_GX_PUZZLE_SOFTKEY_WIDTH,MMI_GX_PUZZLE_SOFTKEY_Y+MMI_GX_PUZZLE_SOFTKEY_HEIGHT,GDI_COLOR_TRANSPARENT);
			#elif defined(__MMI_GAME_PUZZLE_176x220__)
			coolsand_UI_fill_rectangle(MMI_GX_PUZZLE_SOFTKEY_X,MMI_GX_PUZZLE_SOFTKEY_Y,MMI_GX_PUZZLE_SOFTKEY_X+MMI_GX_PUZZLE_SOFTKEY_WIDTH,MMI_GX_PUZZLE_SOFTKEY_Y+MMI_GX_PUZZLE_SOFTKEY_HEIGHT,MMI_GX_PUZZLE_BACKGROUND_COLOR);
			#elif defined(__MMI_GAME_PUZZLE_220x176__)
			coolsand_UI_fill_rectangle(MMI_GX_PUZZLE_SOFTKEY_X,MMI_GX_PUZZLE_SOFTKEY_Y,MMI_GX_PUZZLE_SOFTKEY_X+MMI_GX_PUZZLE_SOFTKEY_WIDTH,MMI_GX_PUZZLE_SOFTKEY_Y+MMI_GX_PUZZLE_SOFTKEY_HEIGHT,MMI_GX_PUZZLE_BACKGROUND_COLOR);
			#endif
			gdi_image_draw_id(MMI_GX_PUZZLE_SOFTKEY_X,MMI_GX_PUZZLE_SOFTKEY_Y,IMG_ID_GX_PUZZLE_UP);
			g_gx_puzzle_context.softkey_state = 0;
		}
	#endif
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#if defined(__MMI_GAME_PUZZLE_128x160__) ||defined(__MMI_GAME_PUZZLE_160x128__) || defined(__MMI_GAME_PUZZLE_176x220__) || defined(__MMI_GAME_PUZZLE_240x320__)||defined(__MMI_GAME_PUZZLE_220x176__)||defined(__MMI_GAME_PUZZLE_320x240__)
      
    /* update TIME every second */
#if defined (__MMI_GAME_PUZZLE_240x320__) || defined (__MMI_GAME_PUZZLE_320x240__)
     /*- zhouqin 2011-02-16 modify for q6 320X240 */
        gui_move_text_cursor(MMI_GX_PUZZLE_TIME_TEXT_X,MMI_GX_PUZZLE_TIME_TEXT_Y);
        gui_set_font(&MMI_medium_font);
        gui_set_text_color(UI_COLOR_WHITE);
        gui_print_text((UI_string_type)L"Time");
#endif
	if (g_gx_puzzle_context.tick % 10 == 0)
	{
		tmp_tick = g_gx_puzzle_context.tick / 10;
		#if defined(__MMI_GAME_PUZZLE_128x160__) || defined(__MMI_GAME_PUZZLE_160x128__)
		position_shift = MMI_GX_PUZZLE_TIME_POSITION_X + 15;
		/*+ zhouqin 2011-02-16 modify for q6 320X240 */
		#elif defined(__MMI_GAME_PUZZLE_176x220__) || defined(__MMI_GAME_PUZZLE_240x320__)||defined(__MMI_GAME_PUZZLE_220x176__)||defined(__MMI_GAME_PUZZLE_320x240__)
                /*- zhouqin 2011-02-16 modify for q6 320X240 */
                position_shift = MMI_GX_PUZZLE_TIME_POSITION_X + 17;
		#endif
	
		do
		{
			image_id = IMG_ID_GX_PUZZLE_NUMBER_0 + (tmp_tick % 10);
			tmp_tick = tmp_tick / 10;
			
			#ifdef __MMI_MAINLCD_220X176__	
			coolsand_UI_show_image(	MMI_GX_PUZZLE_TIME_POSITION_Y + 2 + 128+position_shift -MMI_GX_PUZZLE_TIME_POSITION_X-17,
											MMI_GX_PUZZLE_TIME_POSITION_X+MMI_GX_PUZZLE_TIME_POSITION_X + 17, 
											(UI_image_type)GetImage(image_id));
                        /*+ zhouqin 2011-02-16 modify for q6 320X240 */
                        #elif defined (__MMI_MAINLCD_320X240__)	
                        			coolsand_UI_show_image(	position_shift +2,
                        											MMI_GX_PUZZLE_TIME_POSITION_Y + 30, 
                        											(UI_image_type)GetImage(image_id));
                        /*- zhouqin 2011-02-16 modify for q6 320X240 */				
			#else
			coolsand_UI_show_image(	MMI_GX_PUZZLE_TIME_POSITION_X + position_shift,
							#if defined(__MMI_GAME_PUZZLE_240x320__)
											MMI_GX_PUZZLE_TIME_POSITION_Y + 5 + 128, 
							#else
											MMI_GX_PUZZLE_TIME_POSITION_Y + 2 + 128, 
							#endif
		
											(UI_image_type)GetImage(image_id));
			#endif
			/*+ zhouqin 2011-02-16 modify for q6 320X240 */
			#if defined(__MMI_GAME_PUZZLE_176x220__)|| defined(__MMI_GAME_PUZZLE_240x320__)||defined(__MMI_GAME_PUZZLE_220x176__)||defined(__MMI_GAME_PUZZLE_320x240__)
			/*- zhouqin 2011-02-16 modify for q6 320X240 */
			position_shift = position_shift - MMI_GX_PUZZLE_DIGIT_WIDTH - 1;
			#else
			position_shift = position_shift - MMI_GX_PUZZLE_DIGIT_WIDTH;
			#endif
			
		} while (tmp_tick != 0);
	}
	#elif defined(__MMI_GAME_PUZZLE_128x128__)
	/* draw status ? */
	if (g_gx_puzzle_context.is_show_status != FALSE)
	{
		coolsand_UI_fill_rectangle(	MMI_GX_PUZZLE_STATUS_POS_X + MMI_GX_PUZZLE_STATUS_SHADOW,
											MMI_GX_PUZZLE_STATUS_POS_Y + MMI_GX_PUZZLE_STATUS_SHADOW,
											MMI_GX_PUZZLE_STATUS_POS_X + MMI_GX_PUZZLE_STATUS_WIDTH - 1 + MMI_GX_PUZZLE_STATUS_SHADOW,
											MMI_GX_PUZZLE_STATUS_POS_Y + MMI_GX_PUZZLE_STATUS_HEIGHT - 1 + MMI_GX_PUZZLE_STATUS_SHADOW,
											coolsand_UI_color(155, 155, 155));

		coolsand_UI_fill_rectangle(	MMI_GX_PUZZLE_STATUS_POS_X,
											MMI_GX_PUZZLE_STATUS_POS_Y,
											MMI_GX_PUZZLE_STATUS_POS_X + MMI_GX_PUZZLE_STATUS_WIDTH - 1,
											MMI_GX_PUZZLE_STATUS_POS_Y + MMI_GX_PUZZLE_STATUS_HEIGHT - 1,
											MMI_GX_PUZZLE_BACKGROUND_COLOR);

		coolsand_UI_show_image(	MMI_GX_PUZZLE_TIME_POSITION_X,
										MMI_GX_PUZZLE_TIME_POSITION_Y,
										(UI_image_type)GetImage(IMG_ID_GX_PUZZLE_BOX));
	
		coolsand_UI_show_image(	MMI_GX_PUZZLE_TIME_POSITION_X + 2,
										MMI_GX_PUZZLE_TIME_POSITION_Y - 8,
										(UI_image_type)GetImage(IMG_ID_GX_PUZZLE_TIME));

		coolsand_UI_show_image(	MMI_GX_PUZZLE_STEP_POSITION_X,
										MMI_GX_PUZZLE_STEP_POSITION_Y,
										(UI_image_type)GetImage(IMG_ID_GX_PUZZLE_BOX));

		coolsand_UI_show_image(	MMI_GX_PUZZLE_STEP_POSITION_X + 2,
										MMI_GX_PUZZLE_STEP_POSITION_Y - 8,
										(UI_image_type)GetImage(IMG_ID_GX_PUZZLE_STEP));

		coolsand_UI_show_image(	66,
										47,
										(UI_image_type)GetImage(g_gx_puzzle_context.small_image));

		/* update TIME */
		tmp_tick = g_gx_puzzle_context.tick / 10;
		position_shift = MMI_GX_PUZZLE_TIME_POSITION_X + 0;

		do
		{
			image_id = IMG_ID_GX_PUZZLE_NUMBER_0 + (tmp_tick % 10);
			tmp_tick = tmp_tick / 10;
			coolsand_UI_show_image(	MMI_GX_PUZZLE_TIME_POSITION_X + position_shift,
											MMI_GX_PUZZLE_TIME_POSITION_Y + 2, 
											(UI_image_type)GetImage(image_id));
			position_shift = position_shift - 4;
		} while (tmp_tick != 0);

		/* draw how many step user went */
		tmp_click = g_gx_puzzle_context.click;
		position_shift = MMI_GX_PUZZLE_TIME_POSITION_X + 0;
	
		do
		{
			image_id = IMG_ID_GX_PUZZLE_NUMBER_0 + (tmp_click % 10);
			tmp_click = tmp_click / 10;
			coolsand_UI_show_image(	MMI_GX_PUZZLE_TIME_POSITION_X + position_shift,
											MMI_GX_PUZZLE_TIME_POSITION_Y + 2 + 20, 
											(UI_image_type)GetImage(image_id));
			position_shift = position_shift - 4;
		} while (tmp_click != 0);
	}
	#endif

	coolsand_UI_BLT_double_buffer(0,0, UI_device_width - 1, UI_device_height -1);
}


/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_gameover
* DESCRIPTION
*	Gameover function		
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mmi_gx_puzzle_gameover(void)
{
	g_gx_puzzle_context.is_gameover = TRUE;
	g_gx_puzzle_context.is_new_game = TRUE;
	
	g_gx_puzzle_context.game_grade = g_gx_puzzle_context.tick / 10;

	/* call this function to draw gameover screen */
	mmi_gfx_entry_gameover_screen();
}


/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_cyclic_timer
* DESCRIPTION
*	Timer trigger function - looping				
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
void mmi_gx_puzzle_cyclic_timer(void)
{
	g_gx_puzzle_context.tick++;
	if (g_gx_puzzle_context.tick > 65530)
		g_gx_puzzle_context.tick = 0;

	coolsand_UI_start_timer(g_gx_puzzle_context.timer_elapse, mmi_gx_puzzle_cyclic_timer);
	
	if(g_gx_puzzle_context.is_gameover == FALSE)
	{
		mmi_gx_puzzle_framemove();
		mmi_gx_puzzle_render();
	}
}


/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_key_2_release
* DESCRIPTION
*	Key handling
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
****************************************************************************/
void mmi_gx_puzzle_key_2_release(void)
{
	g_gx_puzzle_context.destination_x = 0;
	g_gx_puzzle_context.destination_y = 1;
	mmi_trace(PUZZLE_TRACE,"mmi_gx_puzzle_key_2_release ");

	mmi_gx_puzzle_move_and_show();
}


/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_key_4_release
* DESCRIPTION
*	Key handling
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
****************************************************************************/
void mmi_gx_puzzle_key_4_release(void)
{
	g_gx_puzzle_context.destination_x = 1;
	g_gx_puzzle_context.destination_y = 0;
	mmi_trace(PUZZLE_TRACE,"mmi_gx_puzzle_key_4_release ");

	mmi_gx_puzzle_move_and_show();
}


/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_key_6_release
* DESCRIPTION
*	Key handling
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
****************************************************************************/
void mmi_gx_puzzle_key_6_release(void)
{
	g_gx_puzzle_context.destination_x = -1;
	g_gx_puzzle_context.destination_y = 0;
	mmi_trace(PUZZLE_TRACE,"mmi_gx_puzzle_key_6_release ");

	mmi_gx_puzzle_move_and_show();
}


/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_key_8_release
* DESCRIPTION
*	Key handling
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
****************************************************************************/
void mmi_gx_puzzle_key_8_release(void)
{
	g_gx_puzzle_context.destination_x = 0;
	g_gx_puzzle_context.destination_y = -1;
	mmi_trace(PUZZLE_TRACE,"mmi_gx_puzzle_key_8_release ");

	mmi_gx_puzzle_move_and_show();
}


#ifdef __MMI_GAME_PUZZLE_128x128__
/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_key_5_press
* DESCRIPTION
*	Key handling
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
****************************************************************************/
void mmi_gx_puzzle_key_5_press(void)
{
	g_gx_puzzle_context.is_show_status = TRUE;
}


/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_key_5_release
* DESCRIPTION
*	Key handling
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
****************************************************************************/
void mmi_gx_puzzle_key_5_release(void)
{
	coolsand_UI_push_clip();
	coolsand_UI_set_clip(	MMI_GX_PUZZLE_STATUS_POS_X,
								MMI_GX_PUZZLE_STATUS_POS_Y,
								MMI_GX_PUZZLE_STATUS_POS_X + MMI_GX_PUZZLE_STATUS_WIDTH - 1 + MMI_GX_PUZZLE_STATUS_SHADOW,
								MMI_GX_PUZZLE_STATUS_POS_Y + MMI_GX_PUZZLE_STATUS_HEIGHT - 1 + MMI_GX_PUZZLE_STATUS_SHADOW);
	
	g_gx_puzzle_context.is_show_status = FALSE;
	mmi_gx_puzzle_show_puzzle();
	mmi_gx_puzzle_draw_seperation_line();

	coolsand_UI_pop_clip();

	coolsand_UI_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);
}
#endif


/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_move_and_show
* DESCRIPTION
*	Move the grid (in memory) and show the movement
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
****************************************************************************/
void	mmi_gx_puzzle_move_and_show(void)
{
	S16					tmp;
	U8						bound_size;
	#ifdef __MMI_GAME_PUZZLE_128x128__
	UI_image_ID_type	image_id;				/* store the last digit of tick */
	U8						position_shift;		/* the position offset of digit displayed */
	#endif
	U8						i;
	U8						j;
	BOOL					is_gameover;

	#ifdef __MMI_GAME_PUZZLE_128x128__
	/* when the status shows, no action taken */
	if (g_gx_puzzle_context.is_show_status != FALSE)
		return;
	#endif

	/* play moving sound */
	#ifdef __MMI_GAME_MULTICHANNEL_SOUND__	
		GFX_STOP_SOUND_EFFECTS_MIDI(g_gx_puzzle_context.move_midi);

		//GFX_PLAY_SOUND_EFFECTS_MIDI(g_gx_puzzle_context.move_midi);
	#else
		//GFX_PLAY_AUDIO_DVI(PuzzleMove_dvi, PUZZLEMOVE_DVI, DEVICE_AUDIO_PLAY_ONCE);
		
			TRACE_EF(g_sw_GAME, "MMI_PUZZLE: GFX.is_aud_on=%d",GFX.is_aud_on);
			if(TRUE == GFX.is_aud_on)
			{
				AudioPlayReq(FNG_PUZZLE_SOUND,(U8)DEVICE_AUDIO_PLAY_ONCE);	//added by jinzh:20070724
			}	
	#endif

	
	bound_size = g_gx_puzzle_context.game_level + 3;

	/* check if the movement is valid */
	if (	g_gx_puzzle_context.null_grid.x + g_gx_puzzle_context.destination_x < 0 || 
			g_gx_puzzle_context.null_grid.x + g_gx_puzzle_context.destination_x >= bound_size ||
			g_gx_puzzle_context.null_grid.y + g_gx_puzzle_context.destination_y < 0 ||
			g_gx_puzzle_context.null_grid.y + g_gx_puzzle_context.destination_y >= bound_size)
	{
		return;
	}

	/* update the numbers user clicks */
	g_gx_puzzle_context.click = g_gx_puzzle_context.click + 1;
	if (g_gx_puzzle_context.click > 65534)
		g_gx_puzzle_context.click = 0;
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#if defined(__MMI_GAME_PUZZLE_128x160__) || defined(__MMI_GAME_PUZZLE_160x128__) || defined(__MMI_GAME_PUZZLE_176x220__)|| defined(__MMI_GAME_PUZZLE_240x320__)||defined(__MMI_GAME_PUZZLE_220x176__)||defined(__MMI_GAME_PUZZLE_320x240__)
	/*- zhouqin 2011-02-16 modify for q6 320X240 */
        mmi_gx_puzzle_draw_step();
	#endif

	/* switch the null grid and its neighboring grid and draw them*/
	tmp = g_gx_puzzle_context.puzzle[g_gx_puzzle_context.null_grid.y][g_gx_puzzle_context.null_grid.x];

	g_gx_puzzle_context.puzzle[g_gx_puzzle_context.null_grid.y][g_gx_puzzle_context.null_grid.x] = g_gx_puzzle_context.puzzle[g_gx_puzzle_context.null_grid.y + g_gx_puzzle_context.destination_y][g_gx_puzzle_context.null_grid.x + g_gx_puzzle_context.destination_x];
	mmi_gx_puzzle_my_show_image(	MMI_GX_PUZZLE_ORIGIN_X + MMI_GX_PUZZLE_SIZE / bound_size * g_gx_puzzle_context.null_grid.x,
											MMI_GX_PUZZLE_ORIGIN_Y + MMI_GX_PUZZLE_SIZE / bound_size * g_gx_puzzle_context.null_grid.y,
											g_gx_puzzle_context.puzzle[g_gx_puzzle_context.null_grid.y + g_gx_puzzle_context.destination_y][g_gx_puzzle_context.null_grid.x + g_gx_puzzle_context.destination_x]);

	g_gx_puzzle_context.puzzle[g_gx_puzzle_context.null_grid.y + g_gx_puzzle_context.destination_y][g_gx_puzzle_context.null_grid.x + g_gx_puzzle_context.destination_x] = tmp;
	mmi_gx_puzzle_my_show_image(	MMI_GX_PUZZLE_ORIGIN_X + MMI_GX_PUZZLE_SIZE / bound_size * (g_gx_puzzle_context.null_grid.x + g_gx_puzzle_context.destination_x),
											MMI_GX_PUZZLE_ORIGIN_Y + MMI_GX_PUZZLE_SIZE / bound_size * (g_gx_puzzle_context.null_grid.y + g_gx_puzzle_context.destination_y),
											tmp);

	g_gx_puzzle_context.null_grid.x = g_gx_puzzle_context.null_grid.x + g_gx_puzzle_context.destination_x;
	g_gx_puzzle_context.null_grid.y = g_gx_puzzle_context.null_grid.y + g_gx_puzzle_context.destination_y;
	
	mmi_gx_puzzle_draw_seperation_line();

	coolsand_UI_BLT_double_buffer(0,0, UI_device_width - 1, UI_device_height - 1);

	/* check if gameover */
	tmp = 1;
	is_gameover = TRUE;
	for (i = 0; i < bound_size; i++)
	{
		for (j = 0; j < bound_size; j++)
		{
			if (g_gx_puzzle_context.puzzle[i][j] != tmp)
			{
				is_gameover = FALSE;
				break;
			}
			tmp = tmp + 1;
		}
		if (is_gameover != TRUE)
		{
			break;
		}
	}
	if (is_gameover != FALSE)
	{
		mmi_gx_puzzle_gameover();
	}
}

/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined(__MMI_GAME_PUZZLE_128x160__) || defined(__MMI_GAME_PUZZLE_160x128__) || defined(__MMI_GAME_PUZZLE_176x220__)|| defined(__MMI_GAME_PUZZLE_240x320__)||defined(__MMI_GAME_PUZZLE_220x176__)||defined(__MMI_GAME_PUZZLE_320x240__)
/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_draw_step
* DESCRIPTION
*	Draw steps
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
****************************************************************************/
void mmi_gx_puzzle_draw_step(void)
{
	U16					tmp_click;
	U8						position_shift;		/* the position offset of digit displayed */
	UI_image_ID_type	image_id;				/* store the last digit of tick */
	
	tmp_click = g_gx_puzzle_context.click;
	#if defined(__MMI_GAME_PUZZLE_128x160__) || defined(__MMI_GAME_PUZZLE_160x128__)
	position_shift = MMI_GX_PUZZLE_STEP_POSITION_X + 15;
	#elif defined(__MMI_GAME_PUZZLE_176x220__)|| defined(__MMI_GAME_PUZZLE_240x320__)||defined(__MMI_GAME_PUZZLE_220x176__)
	position_shift = MMI_GX_PUZZLE_STEP_POSITION_X + 17;
	/*+ zhouqin 2011-02-16 modify for q6 320X240 */
	#elif defined(__MMI_GAME_PUZZLE_320x240__)
	position_shift = MMI_GX_PUZZLE_STEP_POSITION_X;
	/*- zhouqin 2011-02-16 modify for q6 320X240 */
	#endif
/*+ zhouqin 2011-02-16 modify for q6 320X240 */
#if defined (__MMI_GAME_PUZZLE_240x320__) || defined (__MMI_GAME_PUZZLE_320x240__)
        gui_move_text_cursor(MMI_GX_PUZZLE_STEP_TEXT_X,MMI_GX_PUZZLE_STEP_TEXT_Y);
        gui_set_font(&MMI_medium_font);
        gui_set_text_color(UI_COLOR_WHITE);
        gui_print_text((UI_string_type)L"Step");
#endif
	do
	{
		image_id = IMG_ID_GX_PUZZLE_NUMBER_0 + (tmp_click % 10);
		tmp_click = tmp_click / 10;
		
		#ifdef __MMI_MAINLCD_220X176__	
		coolsand_UI_show_image(	MMI_GX_PUZZLE_TIME_POSITION_Y + 2 + 128+position_shift-MMI_GX_PUZZLE_STEP_POSITION_X - 17,
										MMI_GX_PUZZLE_TIME_POSITION_X+ MMI_GX_PUZZLE_STEP_POSITION_X + 17 , 
										(UI_image_type)GetImage(image_id));
	        /*+ zhouqin 2011-02-16 modify for q6 320X240 */
	        #elif defined (__MMI_MAINLCD_320X240__)	
	        coolsand_UI_show_image(	MMI_GX_PUZZLE_STEP_TEXT_X,
										MMI_GX_PUZZLE_STEP_TEXT_Y +25 , 
										(UI_image_type)GetImage(image_id));
	        /*- zhouqin 2011-02-16 modify for q6 320X240 */
		#else
		coolsand_UI_show_image(	MMI_GX_PUZZLE_TIME_POSITION_X + position_shift,
		#if defined(__MMI_GAME_PUZZLE_240x320__)
										MMI_GX_PUZZLE_TIME_POSITION_Y + 5 + 128, 
		#else
										MMI_GX_PUZZLE_TIME_POSITION_Y + 2 + 128, 
		#endif
										(UI_image_type)GetImage(image_id));
		#endif
		/*+ zhouqin 2011-02-16 modify for q6 320X240 */
		#if defined(__MMI_GAME_PUZZLE_176x220__)|| defined(__MMI_GAME_PUZZLE_240x320__)||defined(__MMI_GAME_PUZZLE_220x176__)||defined(__MMI_GAME_PUZZLE_320x240__)
		position_shift = position_shift - MMI_GX_PUZZLE_DIGIT_WIDTH - 1;
		#else
		position_shift = position_shift - MMI_GX_PUZZLE_DIGIT_WIDTH;
		#endif
		
	} while (tmp_click != 0);
}
#endif


/*****************************************************************************
* FUNCTION
*	mmi_gx_puzzle_my_show_image
* DESCRIPTION
*	Calculate which part of the image should be shown
* PARAMETERS
*	x	X position of the shown image
*	y	Y position of the shown image
*	i	UI_image_type for the shown image
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
****************************************************************************/
void	mmi_gx_puzzle_my_show_image(long x, long y, S16 input_image_id)
{
/*	U8						grid_size;
	S16					output_x;
	S16					output_y;
*/	UI_image_ID_type	null_image = 0;
	UI_image_ID_type	image_id = 0;

	if (input_image_id == (g_gx_puzzle_context.game_level + 3) * (g_gx_puzzle_context.game_level + 3))
	{
		switch (g_gx_puzzle_context.game_level)
		{
		case MMI_GX_PUZZLE_EASY:
			null_image = IMG_ID_GX_PUZZLE_3_SPACE;
			break;
		case MMI_GX_PUZZLE_NORMAL:
			null_image = IMG_ID_GX_PUZZLE_4_SPACE;
			break;
		case MMI_GX_PUZZLE_HARD:
			null_image = IMG_ID_GX_PUZZLE_5_SPACE;
			break;
		}

		coolsand_UI_show_image(x, y, (UI_image_type)GetImage(null_image));
	}
	else {
/* Old flash memory saving image drawing method
		grid_size = MMI_GX_PUZZLE_SIZE / (g_gx_puzzle_context.game_level + 3);
		output_x = x - grid_size * ((input_image_id - 1) % (g_gx_puzzle_context.game_level + 3));
		output_y = y - grid_size * ((input_image_id - 1) / (g_gx_puzzle_context.game_level + 3));
	
		coolsand_UI_push_clip();
		coolsand_UI_set_clip(x, y, x + grid_size - 1, y + grid_size - 1);
		coolsand_UI_show_image(output_x, output_y, (UI_image_type)GetImage(g_gx_puzzle_context.image));
		coolsand_UI_pop_clip();
*/
		switch (g_gx_puzzle_context.game_level)
		{
		case MMI_GX_PUZZLE_EASY:
			image_id = IMG_ID_GX_PUZZLE_3_1;
			break;
		case MMI_GX_PUZZLE_NORMAL:
			image_id = IMG_ID_GX_PUZZLE_4_1;
			break;
		case MMI_GX_PUZZLE_HARD:
			image_id = IMG_ID_GX_PUZZLE_5_1;
			break;
		}
		coolsand_UI_show_image(x, y, (UI_image_type)GetImage((U16)(image_id - 1 + input_image_id)));
	}
}

 
#ifdef __MMI_TOUCH_SCREEN__
typedef enum
{
	PUZZLE_MOVE_NONE=0,
	PUZZLE_MOVE_RIGHT=5,
	PUZZLE_MOVE_DOWN=4,
	PUZZLE_MOVE_LEFT=7,
	PUZZLE_MOVE_UP=8
}PUZZLE_MOVEMENT;

PUZZLE_MOVEMENT mmi_puzzle_is_puzzle_moveable(S32 coord_x, S32 coord_y)
{
	#define ABS_T(n) ((n)<0)?-(n):(n) //qiff added
	
	U8 grid_number;
	S16 diff_x=0;
	S16 diff_y = 0;
	S16 diff = 0 ;

	mmi_trace(PUZZLE_TRACE,"mmi_puzzle_is_puzzle_moveable coord_x = %d ,coord_y = %d",coord_x,coord_y);
	diff_x= ABS_T(coord_x-g_gx_puzzle_context.null_grid.x);
	diff_y= ABS_T(coord_y-g_gx_puzzle_context.null_grid.y);
	diff = diff_x +diff_y ;
	grid_number = g_gx_puzzle_context.game_level + 3;
	mmi_trace(PUZZLE_TRACE,"mmi_puzzle_is_puzzle_moveable diff_x = %d ,diff_y = %d",diff_x,diff_y);
	mmi_trace(PUZZLE_TRACE,"mmi_puzzle_is_puzzle_moveable diff = %d ,g_gx_puzzle_context.null_grid.x = %d ,g_gx_puzzle_context.null_grid.y = %d",diff,g_gx_puzzle_context.null_grid.x,g_gx_puzzle_context.null_grid.y);
	if(coord_x <0 || coord_x >= grid_number || coord_y <0 || coord_y >= grid_number)
		return PUZZLE_MOVE_NONE;
	else if ( diff == 1 )
		return ( (coord_x-g_gx_puzzle_context.null_grid.x+2) + 2*(coord_y-g_gx_puzzle_context.null_grid.y+2) );
	else
		return PUZZLE_MOVE_NONE;
}
#endif /* __MMI_TOUCH_SCREEN__ */

/*****************************************************************************
* FUNCTION
*	mmi_puzzle_pen_up_hdlr
* DESCRIPTION
*	pen up event handler
* PARAMETERS
*	pos IN position of pen	
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
#ifdef __MMI_TOUCH_SCREEN__
static mmi_pen_point_struct  puzzle_tmp_pos;
void mmi_puzzle_pen_up_hdlr(mmi_pen_point_struct pos)
{

	mmi_puzzle_pen_down_hdlr(puzzle_tmp_pos);
#ifdef __MMI_GAME_PUZZLE_240x320__     //modified by kecx for project cs2324c on 20090601
  {
	  S32 pos_x=pos.x,pos_y=pos.y;
	  g_gx_puzzle_context.softkey_state=2;

		if( pos_x > MMI_GX_PUZZLE_SOFTKEY_X && pos_x < MMI_GX_PUZZLE_SOFTKEY_X + MMI_GX_PUZZLE_SOFTKEY_WIDTH 
			  && pos_y > MMI_GX_PUZZLE_SOFTKEY_Y && pos_y <MMI_GX_PUZZLE_SOFTKEY_Y + MMI_GX_PUZZLE_SOFTKEY_HEIGHT)
		{
			GoBackHistory();
		}
  }
#endif
}
#endif


#ifdef __MMI_TOUCH_SCREEN__

void mmi_puzzle_pen_down_hdlr1(mmi_pen_point_struct pos)
{
	memcpy(&puzzle_tmp_pos,&pos,sizeof(mmi_pen_point_struct));
}
void mmi_puzzle_pen_down_hdlr(mmi_pen_point_struct pos)
{
	U8 grid_number = g_gx_puzzle_context.game_level + 3;
	U16 puzzle_size = MMI_GX_PUZZLE_SIZE/grid_number;
//	U8	i=0,j=0;
	S32 pos_x=pos.x,pos_y=pos.y;//pen location
	S32 coord_x=0,coord_y=0;//puzzle coordinate index
    #ifdef __MMI_GAME_PUZZLE_240x320__     //modified by kecx for project cs2324c on 20090601
		if( pos_x > MMI_GX_PUZZLE_SOFTKEY_X && pos_x < MMI_GX_PUZZLE_SOFTKEY_X + MMI_GX_PUZZLE_SOFTKEY_WIDTH 
			  && pos_y > MMI_GX_PUZZLE_SOFTKEY_Y && pos_y <MMI_GX_PUZZLE_SOFTKEY_Y + MMI_GX_PUZZLE_SOFTKEY_HEIGHT)
		{
			g_gx_puzzle_context.softkey_state=1;
		}
    #endif
	coord_x=(pos_x-MMI_GX_PUZZLE_ORIGIN_X)/puzzle_size;
	coord_y=(pos_y-MMI_GX_PUZZLE_ORIGIN_Y)/puzzle_size;
	mmi_trace(PUZZLE_TRACE,"mmi_puzzle_pen_down_hdlr  coord_x = %d, coord_y = %d ",coord_x,coord_y);

	switch ( mmi_puzzle_is_puzzle_moveable(coord_x,coord_y) )
	{
		case PUZZLE_MOVE_UP:
			mmi_gx_puzzle_key_2_release();
			break;
		case PUZZLE_MOVE_LEFT:
			mmi_gx_puzzle_key_4_release();
			break;
		case PUZZLE_MOVE_RIGHT:
			mmi_gx_puzzle_key_6_release();
			break;
		case PUZZLE_MOVE_DOWN:
			mmi_gx_puzzle_key_8_release();
			break;
        default:
            break;
	}
}
#endif /* __MMI_TOUCH_SCREEN__ */



#endif /* IS_PUZZLE_BASE_GAME */

