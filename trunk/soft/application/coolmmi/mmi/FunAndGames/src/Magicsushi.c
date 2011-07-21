/*****************************************************************************
* Copyright Statement:
* --------------------
* This software is protected by Copyright and the information contained
* herein is confidential. The software may not be copied and the information
* contained herein may not be used or disclosed except with the written
* permission of UNISCOPE Inc. (C) 2005
*
*****************************************************************************/
/*============================================================================
*
*
* $Id: Magicsushi.c,v 1.6 2007/09/21 09:50:20 lizongzheng Exp $
*
* $Date: 2007/09/21 09:50:20 $
*
* $Name: 1.6 $
*
* $Locker$
*
* $Revision: 1.6 $
*
* $State: Exp $
*
* HISTORY
* Below this line, this part is controlled by CVS. DO NOT MODIFY!!
*----------------------------------------------------------------------------
* $Log: Magicsushi.c,v $
* Revision 1.6  2007/09/21 09:50:20  lizongzheng
* UNI@lizongzheng_20070921 17:49:20 ����ħ����˾����Ϸ�ٶ�
*
* Revision 1.5  2007/08/16 07:36:42  liugang
* UNI@liugang_20070816 808 all 2.6�����ĸĶ� __MMI_UNI_MAINLCD_240X400__
*
* Revision 1.4  2007/08/16 06:15:47  liugang
* UNI@liugang_20070816 808 all 2.6�����ĸĶ� __MMI_UNI_MAINLCD_240X400__
*
* Revision 1.3  2007/08/16 06:08:16  liugang
* UNI@liugang_20070816 808 all 2.6�����ĸĶ� __MMI_UNI_MAINLCD_240X400__
*
* Revision 1.2  2007/05/28 05:42:02  baiyanjun
* UNI@baiyanjun_20070528h 13:41:01 ������Ϸ.
*
* Revision 1.1  2007/05/17 07:22:09  baiyanjun
* UNI@baiyanjun_20070517 15:10:01 COOLSAND U25 U26 06B V30 version
*
*
*----------------------------------------------------------------------------
* Upper this line, this part is controlled by CVS. DO NOT MODIFY!!
*============================================================================
****************************************************************************/
/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2002
*
*****************************************************************************/
/*****************************************************************************
 * Filename:
 * ---------
 * MagicSushi.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Game 
 *
 * Author:
 * -------
 * -------
 *
 ****************************************************************************/
#ifndef _MMI_GX_MAGICSUSHI_C
#define _MMI_GX_MAGICSUSHI_C

#include "mmi_features.h"
#include "gamedefs.h"

#ifdef IS_MAGICSUSHI_BASE_GAME

#include "gameinc.h"
#include "magicsushiprots.h"
#include "magicsushidefs.h"

#ifdef __MMI_TOUCH_SCREEN__
#include "wgui_touch_screen.h"
#endif 

/***************************************************************************** 
* Define
*****************************************************************************/
#if defined (__MMI_UNI_MAINLCD_240X400__)   /*UNI@liugang_20070816 808 all 2.6�����ĸĶ�*/
#define MMI_GX_MAGICSUSHI_LCD_HEIGHT			(400)
#define MMI_GX_MAGICSUSHI_LCD_WIDTH				(240)
#define MMI_GX_MAGICSUSHI_ORIGIN_X				(8)
#define MMI_GX_MAGICSUSHI_ORIGIN_Y				(80)

#define MMI_GX_MAGICSUSHI_SIZE					(28)
#define MMI_GX_MAGICSUSHI_ANIMATION_STEP		(3)

/* BACKBROUND 2 */
#define MMI_GX_MAGICSUSHI_TIME_POSITION_X	(75)
#define MMI_GX_MAGICSUSHI_TIME_POSITION_Y	(327)
#define MMI_GX_MAGICSUSHI_REMAINDER_POSITION_X	(75)
#define MMI_GX_MAGICSUSHI_REMAINDER_POSITION_Y	(342)

#define MMI_GX_MAGICSUSHI_BACKGROUND2_X1 (70)
#define MMI_GX_MAGICSUSHI_BACKGROUND2_Y1 (307)
#define MMI_GX_MAGICSUSHI_BACKGROUND2_X2 (230)
#define MMI_GX_MAGICSUSHI_BACKGROUND2_Y2 (352)

#define MMI_GX_MAGICSUSHI_SOFTKEY_X (190)
#define MMI_GX_MAGICSUSHI_SOFTKEY_Y (370)
#define MMI_GX_MAGICSUSHI_SOFTKEY_WIDTH (40)
#define MMI_GX_MAGICSUSHI_SOFTKEY_HEIGHT (20)
/* END OF BACKGROUND 2*/

/* BACKBROUND 1 */
#define MMI_GX_MAGICSUSHI_SCORE_POSITION_X (105)
#define MMI_GX_MAGICSUSHI_SCORE_POSITION_Y (51)
#define MMI_GX_MAGICSUSHI_LEVEL_POSITION_X	(105)
#define MMI_GX_MAGICSUSHI_LEVEL_POSITION_Y	(29)

#define MMI_GX_MAGICSUSHI_BACKGROUND1_X1 (55)
#define MMI_GX_MAGICSUSHI_BACKGROUND1_Y1 (25)
#define MMI_GX_MAGICSUSHI_BACKGROUND1_X2 (125)
#define MMI_GX_MAGICSUSHI_BACKGROUND1_Y2 (64)
/* END OF BACKGROUND 1 */

#define MMI_GX_MAGICSUSHI_DIGIT_WIDTH		(10)
#define MMI_GX_MAGICSUSHI_LEFT_WIDTH	(8)
#define MMI_GX_MAGICSUSHI_RIGHT_WIDTH	(8)
#define MMI_GX_MAGICSUSHI_TOP_WIDTH		(80)
#else
#define MMI_GX_MAGICSUSHI_LCD_HEIGHT			(320)
#define MMI_GX_MAGICSUSHI_LCD_WIDTH				(240)
#define MMI_GX_MAGICSUSHI_ORIGIN_X				(8)
#define MMI_GX_MAGICSUSHI_ORIGIN_Y				(43)

#define MMI_GX_MAGICSUSHI_SIZE					(28)
#define MMI_GX_MAGICSUSHI_ANIMATION_STEP		(3)

/* BACKBROUND 2 */
#define MMI_GX_MAGICSUSHI_TIME_POSITION_X	(75)
#define MMI_GX_MAGICSUSHI_TIME_POSITION_Y	(276)
#define MMI_GX_MAGICSUSHI_REMAINDER_POSITION_X	(75)
#define MMI_GX_MAGICSUSHI_REMAINDER_POSITION_Y	(291)

#define MMI_GX_MAGICSUSHI_BACKGROUND2_X1 (70)
#define MMI_GX_MAGICSUSHI_BACKGROUND2_Y1 (270)
#define MMI_GX_MAGICSUSHI_BACKGROUND2_X2 (230)
#define MMI_GX_MAGICSUSHI_BACKGROUND2_Y2 (300)

#define MMI_GX_MAGICSUSHI_SOFTKEY_X (190)
#define MMI_GX_MAGICSUSHI_SOFTKEY_Y (299)
#define MMI_GX_MAGICSUSHI_SOFTKEY_WIDTH (40)
#define MMI_GX_MAGICSUSHI_SOFTKEY_HEIGHT (20)
/* END OF BACKGROUND 2*/

/* BACKBROUND 1 */
#define MMI_GX_MAGICSUSHI_SCORE_POSITION_X (105)
#define MMI_GX_MAGICSUSHI_SCORE_POSITION_Y (26)
#define MMI_GX_MAGICSUSHI_LEVEL_POSITION_X	(105)
#define MMI_GX_MAGICSUSHI_LEVEL_POSITION_Y	(8)

#define MMI_GX_MAGICSUSHI_BACKGROUND1_X1 (50)
#define MMI_GX_MAGICSUSHI_BACKGROUND1_Y1 (4)
#define MMI_GX_MAGICSUSHI_BACKGROUND1_X2 (120)
#define MMI_GX_MAGICSUSHI_BACKGROUND1_Y2 (40)
/* END OF BACKGROUND 1 */

#define MMI_GX_MAGICSUSHI_DIGIT_WIDTH		(10)
#define MMI_GX_MAGICSUSHI_LEFT_WIDTH	(8)
#define MMI_GX_MAGICSUSHI_RIGHT_WIDTH	(8)
#define MMI_GX_MAGICSUSHI_TOP_WIDTH		(43)
#endif
#define CHECK_VERTICAL(x,start_y,end_y,length,iNum,flag)\
{\
	length=1; \
	for(++start_y; start_y <= end_y; start_y++)\
	{\
		if( g_gx_magicsushi_context.map[x][start_y] == g_gx_magicsushi_context.map[x][start_y -1])\
			length++;\
		else\
		{\
			if(length > 2)\
			{\
				g_gx_magicsushi_context.solution_start_vertical[x][iNum] = start_y - length;\
				g_gx_magicsushi_context.solution_end_vertical[x][iNum] = start_y - 1;\
				g_gx_magicsushi_context.solution_start_vertical[x][++iNum] = -1;\
				flag =1;\
			}\
			length = 1;\
		}\
	}\
	if(length > 2)\
	{\
		g_gx_magicsushi_context.solution_start_vertical[x][iNum] = start_y - length;\
		g_gx_magicsushi_context.solution_end_vertical[x][iNum] = start_y - 1;\
		g_gx_magicsushi_context.solution_start_vertical[x][++iNum] = -1;\
		flag =1;\
	}\
	g_gx_magicsushi_context.solution_start_vertical[x][iNum] = -1;\
}\

#define CHECK_HORIZONTAL(y,start_x,end_x,length,iNum,flag)\
{\
	length=1;\
	for(++start_x; start_x <= end_x; start_x++)\
	{\
		if( g_gx_magicsushi_context.map[start_x][y] == g_gx_magicsushi_context.map[start_x -1][y])\
			length++;\
		else\
		{\
			if(length > 2)\
			{\
				g_gx_magicsushi_context.solution_start_horizontal[y][iNum] = start_x - length ;\
				g_gx_magicsushi_context.solution_end_horizontal[y][iNum] = start_x - 1;\
				g_gx_magicsushi_context.solution_start_horizontal[y][++iNum] = -1;\
				flag =1;\
			}\
			length = 1;\
		}\
	}\
	if(length > 2)\
	{\
		g_gx_magicsushi_context.solution_start_horizontal[y][iNum] = start_x - length;\
		g_gx_magicsushi_context.solution_end_horizontal[y][iNum] = start_x - 1;\
		g_gx_magicsushi_context.solution_start_horizontal[y][++iNum] = -1;\
		flag =1;\
	}\
	g_gx_magicsushi_context.solution_start_horizontal[y][iNum] = -1;\
}\

#define RAND_ITEM(i,j,item)\
{\
	item =rand()%1000;\
	if (item <5 && !g_gx_magicsushi_context.special)\
	{g_gx_magicsushi_context.map[i][j] = 8;g_gx_magicsushi_context.special=TRUE;}\
	else if (item <10 && !g_gx_magicsushi_context.special)\
	{g_gx_magicsushi_context.map[i][j] = 9;g_gx_magicsushi_context.special=TRUE;}\
	else if (item <15 && !g_gx_magicsushi_context.special)\
	{g_gx_magicsushi_context.map[i][j] = 10;g_gx_magicsushi_context.special=TRUE;}\
	else\
	g_gx_magicsushi_context.map[i][j] = item%g_gx_magicsushi_context.type_num;\
	g_gx_magicsushi_context.i_map[i][j] = 8;\
}\

/************************************************************************/
/* Audio [Const]														*/
/************************************************************************/
#define MAGICSUSHIMOVE					138
#define MAGICSUSHISELECT	890
#define MAGICSUSHIBACKGROUND		2479

 __align(2) const U8 MAGICSUSHIBackground[2479] = {
 0x4D, 0x54, 0x68, 0x64, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x03, 0xC0, 0x4D, 0x54, 0x72, 
 0x6B, 0x00, 0x00, 0x09, 0x99, 0x00, 0xFF, 0x03, 0x08, 0x75, 0x6E, 0x74, 0x69, 0x74, 0x6C, 0x65, 
 0x64, 0x00, 0xFF, 0x02, 0x16, 0x43, 0x6F, 0x70, 0x79, 0x72, 0x69, 0x67, 0x68, 0x74, 0x20, 0x63, 
 0x20, 0x32, 0x30, 0x30, 0x33, 0x20, 0x62, 0x79, 0x20, 0x43, 0x43, 0x00, 0xFF, 0x01, 0x03, 0x43, 
 0x43, 0x0A, 0x00, 0xFF, 0x58, 0x04, 0x04, 0x02, 0x18, 0x08, 0x00, 0xFF, 0x59, 0x02, 0x00, 0x00, 
 0x00, 0xFF, 0x51, 0x03, 0x07, 0x44, 0x1E, 0x00, 0xC0, 0x54, 0x00, 0xB0, 0x07, 0x62, 0x00, 0x90, 
 0x48, 0x6F, 0x00, 0xC1, 0x0B, 0x00, 0xB1, 0x07, 0x64, 0x00, 0x91, 0x48, 0x67, 0x00, 0xC2, 0x01, 
 0x00, 0xB2, 0x07, 0x6A, 0x00, 0x92, 0x47, 0x7C, 0x00, 0x43, 0x7E, 0x00, 0x3C, 0x7F, 0x00, 0x40, 
 0x7C, 0x81, 0x70, 0x40, 0x00, 0x00, 0x43, 0x00, 0x00, 0x47, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x3C, 
 0x50, 0x00, 0x47, 0x51, 0x00, 0x43, 0x5A, 0x00, 0x40, 0x53, 0x50, 0x91, 0x48, 0x00, 0x00, 0x49, 
 0x67, 0x81, 0x20, 0x92, 0x3C, 0x00, 0x00, 0x43, 0x00, 0x00, 0x47, 0x00, 0x00, 0x90, 0x48, 0x00, 
 0x00, 0x92, 0x40, 0x00, 0x00, 0x90, 0x4C, 0x7C, 0x00, 0x92, 0x3C, 0x50, 0x00, 0x47, 0x50, 0x00, 
 0x43, 0x5A, 0x00, 0x40, 0x5A, 0x81, 0x20, 0x91, 0x49, 0x00, 0x00, 0x4A, 0x51, 0x50, 0x92, 0x3C, 
 0x00, 0x00, 0x43, 0x00, 0x00, 0x47, 0x00, 0x00, 0x40, 0x00, 0x00, 0x3C, 0x51, 0x00, 0x43, 0x5A, 
 0x00, 0x47, 0x51, 0x00, 0x40, 0x53, 0x81, 0x70, 0x40, 0x00, 0x00, 0x90, 0x4C, 0x00, 0x00, 0x92, 
 0x3C, 0x00, 0x00, 0x91, 0x4A, 0x00, 0x00, 0x92, 0x47, 0x00, 0x00, 0x43, 0x00, 0x00, 0x90, 0x4F, 
 0x7C, 0x00, 0x91, 0x4B, 0x7D, 0x00, 0x92, 0x3C, 0x50, 0x00, 0x43, 0x53, 0x00, 0x47, 0x49, 0x00, 
 0x40, 0x51, 0x81, 0x70, 0x40, 0x00, 0x00, 0x43, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x47, 0x00, 0x00, 
 0x3C, 0x51, 0x00, 0x47, 0x51, 0x00, 0x43, 0x53, 0x00, 0x40, 0x53, 0x50, 0x91, 0x4B, 0x00, 0x00, 
 0x4A, 0x51, 0x81, 0x20, 0x92, 0x3C, 0x00, 0x00, 0x43, 0x00, 0x00, 0x47, 0x00, 0x00, 0x90, 0x4F, 
 0x00, 0x00, 0x92, 0x40, 0x00, 0x00, 0x90, 0x48, 0x67, 0x00, 0x92, 0x47, 0x52, 0x00, 0x43, 0x5A, 
 0x00, 0x3C, 0x49, 0x00, 0x40, 0x5A, 0x81, 0x20, 0x91, 0x4A, 0x00, 0x00, 0x49, 0x53, 0x50, 0x92, 
 0x47, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x43, 0x00, 0x00, 0x90, 0x48, 0x00, 0x00, 0x92, 0x40, 0x00, 
 0x00, 0x90, 0x4C, 0x7C, 0x00, 0x92, 0x47, 0x53, 0x00, 0x3C, 0x51, 0x00, 0x43, 0x5A, 0x00, 0x40, 
 0x5A, 0x81, 0x70, 0x40, 0x00, 0x00, 0x91, 0x49, 0x00, 0x00, 0x92, 0x47, 0x00, 0x00, 0x3C, 0x00, 
 0x00, 0x43, 0x00, 0x00, 0x91, 0x48, 0x7C, 0x00, 0x92, 0x47, 0x67, 0x00, 0x43, 0x6C, 0x00, 0x40, 
 0x6E, 0x00, 0x3C, 0x6E, 0x81, 0x70, 0x3C, 0x00, 0x00, 0x43, 0x00, 0x00, 0x47, 0x00, 0x00, 0x40, 
 0x00, 0x00, 0x47, 0x51, 0x00, 0x3C, 0x50, 0x00, 0x43, 0x5A, 0x00, 0x40, 0x5A, 0x81, 0x69, 0x90, 
 0x4C, 0x00, 0x07, 0x92, 0x47, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x91, 0x48, 0x00, 0x00, 0x92, 0x40, 
 0x00, 0x00, 0x43, 0x00, 0x00, 0x90, 0x4F, 0x7C, 0x00, 0x91, 0x4C, 0x7D, 0x00, 0x92, 0x47, 0x52, 
 0x00, 0x43, 0x53, 0x00, 0x3C, 0x49, 0x00, 0x40, 0x5A, 0x81, 0x70, 0x40, 0x00, 0x00, 0x91, 0x4C, 
 0x00, 0x00, 0x92, 0x47, 0x00, 0x00, 0x43, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x91, 0x4D, 0x7C, 0x00, 
 0x92, 0x47, 0x52, 0x00, 0x3C, 0x50, 0x00, 0x43, 0x52, 0x00, 0x40, 0x5A, 0x81, 0x70, 0x40, 0x00, 
 0x00, 0x91, 0x4D, 0x00, 0x00, 0x92, 0x47, 0x00, 0x00, 0x90, 0x4F, 0x00, 0x00, 0x92, 0x43, 0x00, 
 0x00, 0x3C, 0x00, 0x00, 0x90, 0x48, 0x67, 0x00, 0x91, 0x4F, 0x7C, 0x00, 0x92, 0x3C, 0x51, 0x00, 
 0x47, 0x53, 0x00, 0x43, 0x5A, 0x00, 0x40, 0x67, 0x81, 0x70, 0x40, 0x00, 0x00, 0x91, 0x4F, 0x00, 
 0x00, 0x92, 0x3C, 0x00, 0x00, 0x47, 0x00, 0x00, 0x43, 0x00, 0x00, 0x91, 0x4D, 0x7D, 0x00, 0x92, 
 0x47, 0x52, 0x00, 0x3C, 0x51, 0x00, 0x43, 0x53, 0x00, 0x40, 0x5A, 0x81, 0x70, 0x40, 0x00, 0x00, 
 0x91, 0x4D, 0x00, 0x00, 0x92, 0x47, 0x00, 0x00, 0x90, 0x48, 0x00, 0x00, 0x92, 0x43, 0x00, 0x00, 
 0x3C, 0x00, 0x00, 0x90, 0x4C, 0x6F, 0x00, 0x91, 0x4C, 0x7D, 0x00, 0x92, 0x47, 0x52, 0x00, 0x43, 
 0x53, 0x00, 0x40, 0x53, 0x00, 0x3C, 0x48, 0x81, 0x70, 0x3C, 0x00, 0x00, 0x43, 0x00, 0x00, 0x47, 
 0x00, 0x00, 0x40, 0x00, 0x00, 0x47, 0x51, 0x00, 0x43, 0x5A, 0x00, 0x40, 0x67, 0x00, 0x3C, 0x50, 
 0x81, 0x70, 0x3C, 0x00, 0x00, 0x90, 0x4C, 0x00, 0x00, 0x92, 0x47, 0x00, 0x00, 0x91, 0x4C, 0x00, 
 0x00, 0x92, 0x40, 0x00, 0x00, 0x43, 0x00, 0x00, 0x90, 0x4D, 0x7C, 0x00, 0x91, 0x4D, 0x7D, 0x00, 
 0x92, 0x48, 0x78, 0x00, 0x45, 0x70, 0x00, 0x41, 0x72, 0x00, 0x3E, 0x7C, 0x81, 0x70, 0x3E, 0x00, 
 0x00, 0x45, 0x00, 0x00, 0x48, 0x00, 0x00, 0x41, 0x00, 0x00, 0x48, 0x50, 0x00, 0x45, 0x52, 0x00, 
 0x41, 0x53, 0x00, 0x3E, 0x51, 0x50, 0x91, 0x4D, 0x00, 0x00, 0x4E, 0x6F, 0x81, 0x20, 0x92, 0x48, 
 0x00, 0x00, 0x41, 0x00, 0x00, 0x45, 0x00, 0x00, 0x90, 0x4D, 0x00, 0x00, 0x92, 0x3E, 0x00, 0x00, 
 0x90, 0x51, 0x7C, 0x00, 0x92, 0x48, 0x42, 0x00, 0x45, 0x50, 0x00, 0x41, 0x52, 0x00, 0x3E, 0x49, 
 0x81, 0x20, 0x91, 0x4E, 0x00, 0x00, 0x4F, 0x5A, 0x50, 0x92, 0x48, 0x00, 0x00, 0x41, 0x00, 0x00, 
 0x45, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x48, 0x3F, 0x00, 0x45, 0x51, 0x00, 0x41, 0x51, 0x00, 0x3E, 
 0x42, 0x81, 0x70, 0x3E, 0x00, 0x00, 0x90, 0x51, 0x00, 0x00, 0x92, 0x48, 0x00, 0x00, 0x91, 0x4F, 
 0x00, 0x00, 0x92, 0x41, 0x00, 0x00, 0x45, 0x00, 0x00, 0x90, 0x54, 0x7C, 0x00, 0x91, 0x50, 0x7C, 
 0x00, 0x92, 0x45, 0x53, 0x00, 0x48, 0x49, 0x00, 0x41, 0x53, 0x00, 0x3E, 0x49, 0x81, 0x70, 0x3E, 
 0x00, 0x00, 0x48, 0x00, 0x00, 0x45, 0x00, 0x00, 0x41, 0x00, 0x00, 0x48, 0x49, 0x00, 0x45, 0x52, 
 0x00, 0x41, 0x52, 0x00, 0x3E, 0x51, 0x50, 0x91, 0x50, 0x00, 0x00, 0x4F, 0x53, 0x81, 0x20, 0x92, 
 0x48, 0x00, 0x00, 0x41, 0x00, 0x00, 0x45, 0x00, 0x00, 0x90, 0x54, 0x00, 0x00, 0x92, 0x3E, 0x00, 
 0x00, 0x90, 0x4D, 0x7C, 0x00, 0x92, 0x45, 0x52, 0x00, 0x3E, 0x50, 0x00, 0x48, 0x42, 0x00, 0x41, 
 0x52, 0x81, 0x20, 0x91, 0x4F, 0x00, 0x00, 0x4E, 0x7C, 0x50, 0x92, 0x45, 0x00, 0x00, 0x48, 0x00, 
 0x00, 0x3E, 0x00, 0x00, 0x90, 0x4D, 0x00, 0x00, 0x92, 0x41, 0x00, 0x00, 0x90, 0x51, 0x7C, 0x00, 
 0x92, 0x3E, 0x52, 0x00, 0x48, 0x48, 0x00, 0x45, 0x52, 0x00, 0x41, 0x53, 0x81, 0x70, 0x41, 0x00, 
 0x00, 0x91, 0x4E, 0x00, 0x00, 0x92, 0x3E, 0x00, 0x00, 0x48, 0x00, 0x00, 0x45, 0x00, 0x00, 0x91, 
 0x4D, 0x7C, 0x00, 0x92, 0x48, 0x64, 0x00, 0x45, 0x6C, 0x00, 0x3E, 0x6E, 0x00, 0x41, 0x6A, 0x81, 
 0x70, 0x41, 0x00, 0x00, 0x45, 0x00, 0x00, 0x48, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x48, 0x48, 0x00, 
 0x3E, 0x50, 0x00, 0x45, 0x53, 0x00, 0x41, 0x53, 0x81, 0x70, 0x41, 0x00, 0x00, 0x91, 0x4D, 0x00, 
 0x00, 0x92, 0x48, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x45, 0x00, 0x00, 0x90, 0x54, 0x7C, 0x00, 0x91, 
 0x51, 0x7D, 0x00, 0x92, 0x48, 0x49, 0x00, 0x45, 0x53, 0x00, 0x3E, 0x51, 0x00, 0x41, 0x53, 0x06, 
 0x90, 0x51, 0x00, 0x81, 0x6A, 0x92, 0x3E, 0x00, 0x00, 0x91, 0x51, 0x00, 0x00, 0x92, 0x48, 0x00, 
 0x00, 0x45, 0x00, 0x00, 0x41, 0x00, 0x00, 0x91, 0x53, 0x7C, 0x00, 0x92, 0x48, 0x49, 0x00, 0x45, 
 0x53, 0x00, 0x3E, 0x49, 0x00, 0x41, 0x53, 0x81, 0x70, 0x41, 0x00, 0x00, 0x91, 0x53, 0x00, 0x00, 
 0x92, 0x48, 0x00, 0x00, 0x90, 0x54, 0x00, 0x00, 0x92, 0x3E, 0x00, 0x00, 0x45, 0x00, 0x00, 0x90, 
 0x4D, 0x7D, 0x00, 0x91, 0x54, 0x6F, 0x00, 0x92, 0x48, 0x50, 0x00, 0x3E, 0x52, 0x00, 0x45, 0x53, 
 0x00, 0x41, 0x5A, 0x81, 0x70, 0x41, 0x00, 0x00, 0x91, 0x54, 0x00, 0x00, 0x92, 0x48, 0x00, 0x00, 
 0x3E, 0x00, 0x00, 0x45, 0x00, 0x00, 0x91, 0x53, 0x7C, 0x00, 0x92, 0x48, 0x52, 0x00, 0x45, 0x53, 
 0x00, 0x41, 0x67, 0x00, 0x3E, 0x50, 0x81, 0x70, 0x3E, 0x00, 0x00, 0x91, 0x53, 0x00, 0x00, 0x92, 
 0x48, 0x00, 0x00, 0x90, 0x4D, 0x00, 0x00, 0x92, 0x41, 0x00, 0x00, 0x45, 0x00, 0x00, 0x90, 0x51, 
 0x7C, 0x00, 0x91, 0x51, 0x7C, 0x00, 0x92, 0x3E, 0x52, 0x00, 0x48, 0x51, 0x00, 0x45, 0x53, 0x00, 
 0x41, 0x67, 0x81, 0x70, 0x41, 0x00, 0x00, 0x48, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x45, 0x00, 0x00, 
 0x48, 0x50, 0x00, 0x45, 0x52, 0x00, 0x41, 0x5A, 0x00, 0x3E, 0x52, 0x81, 0x70, 0x3E, 0x00, 0x00, 
 0x90, 0x51, 0x00, 0x00, 0x92, 0x48, 0x00, 0x00, 0x91, 0x51, 0x00, 0x00, 0x92, 0x41, 0x00, 0x00, 
 0x45, 0x00, 0x00, 0x90, 0x4F, 0x7C, 0x00, 0x91, 0x4F, 0x67, 0x00, 0x92, 0x4A, 0x78, 0x00, 0x47, 
 0x78, 0x00, 0x43, 0x78, 0x00, 0x40, 0x7A, 0x81, 0x70, 0x40, 0x00, 0x00, 0x47, 0x00, 0x00, 0x4A, 
 0x00, 0x00, 0x43, 0x00, 0x00, 0x4A, 0x41, 0x00, 0x47, 0x49, 0x00, 0x40, 0x48, 0x00, 0x43, 0x50, 
 0x50, 0x91, 0x4F, 0x00, 0x00, 0x50, 0x7D, 0x81, 0x20, 0x92, 0x4A, 0x00, 0x00, 0x40, 0x00, 0x00, 
 0x47, 0x00, 0x00, 0x90, 0x4F, 0x00, 0x00, 0x92, 0x43, 0x00, 0x00, 0x90, 0x53, 0x5A, 0x00, 0x92, 
 0x4A, 0x49, 0x00, 0x47, 0x49, 0x00, 0x40, 0x50, 0x00, 0x43, 0x51, 0x81, 0x20, 0x91, 0x50, 0x00, 
 0x00, 0x51, 0x5A, 0x50, 0x92, 0x4A, 0x00, 0x00, 0x40, 0x00, 0x00, 0x47, 0x00, 0x00, 0x43, 0x00, 
 0x00, 0x4A, 0x49, 0x00, 0x40, 0x49, 0x00, 0x47, 0x51, 0x00, 0x43, 0x51, 0x81, 0x70, 0x43, 0x00, 
 0x00, 0x90, 0x53, 0x00, 0x00, 0x92, 0x4A, 0x00, 0x00, 0x91, 0x51, 0x00, 0x00, 0x92, 0x47, 0x00, 
 0x00, 0x40, 0x00, 0x00, 0x90, 0x56, 0x7C, 0x00, 0x91, 0x52, 0x7C, 0x00, 0x92, 0x4A, 0x49, 0x00, 
 0x47, 0x51, 0x00, 0x43, 0x52, 0x00, 0x40, 0x48, 0x00, 0x4A, 0x42, 0x00, 0x40, 0x51, 0x00, 0x43, 
 0x52, 0x81, 0x70, 0x43, 0x00, 0x00, 0x47, 0x00, 0x00, 0x40, 0x00, 0x00, 0x43, 0x00, 0x00, 0x4A, 
 0x00, 0x00, 0x4A, 0x00, 0x00, 0x40, 0x00, 0x00, 0x40, 0x50, 0x00, 0x4A, 0x50, 0x00, 0x47, 0x53, 
 0x00, 0x43, 0x5A, 0x50, 0x91, 0x52, 0x00, 0x00, 0x51, 0x52, 0x81, 0x20, 0x92, 0x40, 0x00, 0x00, 
 0x47, 0x00, 0x00, 0x4A, 0x00, 0x00, 0x90, 0x56, 0x00, 0x00, 0x92, 0x43, 0x00, 0x00, 0x90, 0x4F, 
 0x6F, 0x00, 0x92, 0x40, 0x13, 0x00, 0x4A, 0x38, 0x00, 0x47, 0x50, 0x00, 0x43, 0x34, 0x81, 0x20, 
 0x91, 0x51, 0x00, 0x00, 0x50, 0x7C, 0x50, 0x92, 0x40, 0x00, 0x00, 0x47, 0x00, 0x00, 0x4A, 0x00, 
 0x00, 0x90, 0x4F, 0x00, 0x00, 0x92, 0x43, 0x00, 0x00, 0x90, 0x53, 0x7C, 0x00, 0x92, 0x4A, 0x7C, 
 0x00, 0x47, 0x6F, 0x00, 0x43, 0x7C, 0x00, 0x40, 0x6F, 0x81, 0x70, 0x40, 0x00, 0x00, 0x91, 0x50, 
 0x00, 0x00, 0x92, 0x4A, 0x00, 0x00, 0x47, 0x00, 0x00, 0x43, 0x00, 0x00, 0x91, 0x4F, 0x7D, 0x00, 
 0x92, 0x4A, 0x41, 0x00, 0x47, 0x51, 0x00, 0x43, 0x51, 0x00, 0x40, 0x49, 0x81, 0x70, 0x40, 0x00, 
 0x00, 0x47, 0x00, 0x00, 0x4A, 0x00, 0x00, 0x43, 0x00, 0x00, 0x4A, 0x64, 0x00, 0x47, 0x68, 0x00, 
 0x41, 0x72, 0x00, 0x43, 0x64, 0x81, 0x70, 0x43, 0x00, 0x00, 0x91, 0x4F, 0x00, 0x00, 0x92, 0x4A, 
 0x00, 0x00, 0x47, 0x00, 0x00, 0x41, 0x00, 0x00, 0x90, 0x59, 0x7C, 0x00, 0x91, 0x53, 0x7C, 0x00, 
 0x92, 0x4A, 0x41, 0x00, 0x47, 0x50, 0x00, 0x43, 0x53, 0x00, 0x41, 0x49, 0x0E, 0x90, 0x53, 0x00, 
 0x81, 0x62, 0x92, 0x43, 0x00, 0x00, 0x91, 0x53, 0x00, 0x00, 0x92, 0x4A, 0x00, 0x00, 0x47, 0x00, 
 0x00, 0x41, 0x00, 0x00, 0x91, 0x54, 0x7C, 0x00, 0x92, 0x4A, 0x51, 0x00, 0x47, 0x52, 0x00, 0x43, 
 0x52, 0x00, 0x41, 0x50, 0x81, 0x70, 0x41, 0x00, 0x00, 0x91, 0x54, 0x00, 0x00, 0x92, 0x4A, 0x00, 
 0x00, 0x90, 0x59, 0x00, 0x00, 0x92, 0x43, 0x00, 0x00, 0x47, 0x00, 0x00, 0x90, 0x4F, 0x7C, 0x00, 
 0x91, 0x56, 0x7C, 0x00, 0x92, 0x4A, 0x50, 0x00, 0x47, 0x50, 0x00, 0x41, 0x50, 0x00, 0x43, 0x53, 
 0x81, 0x70, 0x43, 0x00, 0x00, 0x91, 0x56, 0x00, 0x00, 0x92, 0x4A, 0x00, 0x00, 0x47, 0x00, 0x00, 
 0x41, 0x00, 0x00, 0x91, 0x54, 0x7C, 0x00, 0x92, 0x4A, 0x50, 0x00, 0x47, 0x52, 0x00, 0x43, 0x53, 
 0x00, 0x41, 0x52, 0x81, 0x70, 0x41, 0x00, 0x00, 0x91, 0x54, 0x00, 0x00, 0x92, 0x4A, 0x00, 0x00, 
 0x90, 0x4F, 0x00, 0x00, 0x92, 0x43, 0x00, 0x00, 0x47, 0x00, 0x00, 0x90, 0x53, 0x7C, 0x00, 0x91, 
 0x53, 0x7C, 0x00, 0x92, 0x4A, 0x48, 0x00, 0x47, 0x52, 0x00, 0x41, 0x3F, 0x00, 0x43, 0x50, 0x81, 
 0x70, 0x43, 0x00, 0x00, 0x47, 0x00, 0x00, 0x4A, 0x00, 0x00, 0x41, 0x00, 0x00, 0x4A, 0x5A, 0x00, 
 0x47, 0x58, 0x00, 0x43, 0x56, 0x00, 0x41, 0x60, 0x81, 0x70, 0x41, 0x00, 0x00, 0x90, 0x53, 0x00, 
 0x00, 0x92, 0x4A, 0x00, 0x00, 0x91, 0x53, 0x00, 0x00, 0x92, 0x43, 0x00, 0x00, 0x47, 0x00, 0x00, 
 0x90, 0x54, 0x7C, 0x00, 0x91, 0x54, 0x6F, 0x00, 0x92, 0x4C, 0x78, 0x00, 0x47, 0x78, 0x00, 0x43, 
 0x78, 0x00, 0x48, 0x78, 0x81, 0x70, 0x48, 0x00, 0x00, 0x47, 0x00, 0x00, 0x4C, 0x00, 0x00, 0x43, 
 0x00, 0x00, 0x4C, 0x42, 0x00, 0x48, 0x51, 0x00, 0x47, 0x41, 0x00, 0x43, 0x41, 0x50, 0x91, 0x54, 
 0x00, 0x00, 0x55, 0x5A, 0x81, 0x20, 0x92, 0x4C, 0x00, 0x00, 0x47, 0x00, 0x00, 0x48, 0x00, 0x00, 
 0x90, 0x54, 0x00, 0x00, 0x92, 0x43, 0x00, 0x00, 0x90, 0x58, 0x67, 0x00, 0x92, 0x4C, 0x3F, 0x00, 
 0x47, 0x42, 0x00, 0x43, 0x41, 0x00, 0x48, 0x52, 0x81, 0x20, 0x91, 0x55, 0x00, 0x00, 0x56, 0x53, 
 0x50, 0x92, 0x4C, 0x00, 0x00, 0x43, 0x00, 0x00, 0x47, 0x00, 0x00, 0x48, 0x00, 0x00, 0x47, 0x51, 
 0x00, 0x4C, 0x50, 0x00, 0x48, 0x42, 0x00, 0x43, 0x52, 0x81, 0x70, 0x43, 0x00, 0x00, 0x90, 0x58, 
 0x00, 0x00, 0x92, 0x47, 0x00, 0x00, 0x91, 0x56, 0x00, 0x00, 0x92, 0x48, 0x00, 0x00, 0x4C, 0x00, 
 0x00, 0x90, 0x5B, 0x7C, 0x00, 0x91, 0x57, 0x7C, 0x00, 0x92, 0x4C, 0x50, 0x00, 0x48, 0x42, 0x00, 
 0x47, 0x49, 0x00, 0x43, 0x49, 0x81, 0x70, 0x43, 0x00, 0x00, 0x48, 0x00, 0x00, 0x4C, 0x00, 0x00, 
 0x47, 0x00, 0x00, 0x4C, 0x50, 0x00, 0x48, 0x50, 0x00, 0x47, 0x52, 0x00, 0x43, 0x52, 0x50, 0x91, 
 0x57, 0x00, 0x00, 0x56, 0x6F, 0x81, 0x20, 0x92, 0x4C, 0x00, 0x00, 0x47, 0x00, 0x00, 0x48, 0x00, 
 0x00, 0x90, 0x5B, 0x00, 0x00, 0x92, 0x43, 0x00, 0x00, 0x90, 0x54, 0x6F, 0x00, 0x92, 0x4C, 0x51, 
 0x00, 0x48, 0x50, 0x00, 0x47, 0x52, 0x00, 0x43, 0x53, 0x81, 0x20, 0x91, 0x56, 0x00, 0x00, 0x55, 
 0x5A, 0x50, 0x92, 0x4C, 0x00, 0x00, 0x47, 0x00, 0x00, 0x48, 0x00, 0x00, 0x90, 0x54, 0x00, 0x00, 
 0x92, 0x43, 0x00, 0x00, 0x90, 0x58, 0x7C, 0x00, 0x92, 0x4C, 0x5A, 0x00, 0x48, 0x5A, 0x00, 0x47, 
 0x6F, 0x00, 0x43, 0x6F, 0x81, 0x70, 0x43, 0x00, 0x00, 0x91, 0x55, 0x00, 0x00, 0x92, 0x4C, 0x00, 
 0x00, 0x48, 0x00, 0x00, 0x47, 0x00, 0x00, 0x91, 0x54, 0x67, 0x00, 0x92, 0x4C, 0x6E, 0x00, 0x48, 
 0x65, 0x00, 0x47, 0x70, 0x00, 0x43, 0x6C, 0x81, 0x70, 0x43, 0x00, 0x00, 0x48, 0x00, 0x00, 0x4C, 
 0x00, 0x00, 0x47, 0x00, 0x00, 0x4C, 0x4A, 0x00, 0x48, 0x4A, 0x00, 0x47, 0x48, 0x00, 0x43, 0x47, 
 0x81, 0x48, 0x90, 0x58, 0x00, 0x28, 0x92, 0x4C, 0x00, 0x00, 0x48, 0x00, 0x00, 0x91, 0x54, 0x00, 
 0x00, 0x92, 0x43, 0x00, 0x00, 0x47, 0x00, 0x00, 0x90, 0x5B, 0x6F, 0x00, 0x91, 0x4F, 0x7C, 0x00, 
 0x92, 0x4C, 0x50, 0x00, 0x48, 0x50, 0x00, 0x47, 0x50, 0x00, 0x43, 0x54, 0x81, 0x70, 0x43, 0x00, 
 0x00, 0x91, 0x4F, 0x00, 0x00, 0x92, 0x4C, 0x00, 0x00, 0x48, 0x00, 0x00, 0x47, 0x00, 0x00, 0x91, 
 0x4D, 0x7C, 0x00, 0x92, 0x4C, 0x46, 0x00, 0x48, 0x48, 0x00, 0x47, 0x46, 0x00, 0x43, 0x49, 0x81, 
 0x70, 0x43, 0x00, 0x00, 0x91, 0x4D, 0x00, 0x00, 0x92, 0x4C, 0x00, 0x00, 0x90, 0x5B, 0x00, 0x00, 
 0x92, 0x47, 0x00, 0x00, 0x48, 0x00, 0x00, 0x90, 0x54, 0x6F, 0x00, 0x91, 0x4C, 0x7C, 0x00, 0x92, 
 0x4C, 0x51, 0x00, 0x48, 0x4B, 0x00, 0x47, 0x27, 0x00, 0x43, 0x52, 0x81, 0x70, 0x43, 0x00, 0x00, 
 0x91, 0x4C, 0x00, 0x00, 0x92, 0x4C, 0x00, 0x00, 0x48, 0x00, 0x00, 0x47, 0x00, 0x00, 0x91, 0x4B, 
 0x7D, 0x00, 0x92, 0x4C, 0x50, 0x00, 0x47, 0x4E, 0x00, 0x43, 0x50, 0x00, 0x48, 0x52, 0x81, 0x70, 
 0x48, 0x00, 0x00, 0x91, 0x4B, 0x00, 0x00, 0x92, 0x4C, 0x00, 0x00, 0x90, 0x54, 0x00, 0x00, 0x92, 
 0x43, 0x00, 0x00, 0x47, 0x00, 0x00, 0x90, 0x58, 0x7C, 0x00, 0x91, 0x4A, 0x7C, 0x00, 0x92, 0x4C, 
 0x51, 0x00, 0x47, 0x50, 0x00, 0x43, 0x52, 0x00, 0x48, 0x52, 0x81, 0x70, 0x48, 0x00, 0x00, 0x47, 
 0x00, 0x00, 0x4C, 0x00, 0x00, 0x43, 0x00, 0x00, 0x4C, 0x53, 0x00, 0x47, 0x52, 0x00, 0x48, 0x51, 
 0x00, 0x43, 0x52, 0x81, 0x70, 0x43, 0x00, 0x00, 0x90, 0x58, 0x00, 0x00, 0x92, 0x4C, 0x00, 0x00, 
 0x91, 0x4A, 0x00, 0x00, 0x92, 0x48, 0x00, 0x00, 0x47, 0x00, 0x00, 0xFF, 0x2F, 0x00
 };

 __align(2) const U8 MAGICSUSHIMove[138] = {
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


__align(2) const U8 MAGICSUSHISelect[890] = {
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

/***************************************************************************** 
* Typedef 
*****************************************************************************/
/* game context */
typedef struct{
	BOOL		is_gameover;
	BOOL		is_new_game;
	U8			game_level;	
	U16		game_grade;
	U8			timer_elapse;
	U8			click_mode;
	U8			selected;
	U8			level;
	U8			bonus;
	BOOL		special;
	U8			special_x;
	U8			special_y;
	U8			old_x;
	U8			old_y;
	S16		remainder;
	U8			check;           
	U8			total_time;
	U8			remain_time;
	U16		tick;
	U8			animation_step;
	U8			animation_mode;
	U8 		moving;

	U8			last_key;
	U8 		cursor_x;
	U8			cursor_y;
	
	U8 		softkey_state;
	
	gdi_handle							background;
	gdi_handle							foreground;
	
	U8   		map[8][8];
	U8			i_map[8][8];
	U8			type_num;
	S16		solution_start_vertical[8][4];
	S16		solution_end_vertical[8][4];
	S16		solution_start_horizontal[8][4];
	S16		solution_end_horizontal[8][4];

#if defined (__MMI_GAME_MULTICHANNEL_SOUND__)
	S32										background_midi;
	S32 										move_midi;
	S32 										select_midi;
#endif

} gx_magicsushi_context_struct;

/***************************************************************************** 
* Local Variable
*****************************************************************************/
gx_magicsushi_context_struct g_gx_magicsushi_context =
{
	FALSE,		/* is_gameover */
	TRUE,			/* is_new_game */
	0,				/* game_level */
	0,				/* game_grade */
	1,				/* timer_elapse */
	0,				/* click_mode */
	0,				/* selected */
	1,				/* level */
	0,				/* bonus */
	0,				/* special */
	0,				/* special x */
	0,				/* special y */
	0,				/* old x */
	0,				/* old x */
	15,			/* remainder */
	0,				/* check */
	50,			/* total time */
	25,			/* remain time */
	0,				/* tick */
	0,				/* animation_step */
	0,				/* animation_mode */
	0,				/* moving */
	0,				/* last key */
	0,				/* cursorx */
	0,				/* cursory */
	0				/* SOFTKEY STATE*/
};

/***************************************************************************** 
* Local Function
*****************************************************************************/
/* Game framework related functions */
S16	mmi_gx_magicsushi_calc_best_grade(S16 old_grade, S16 new_grade);	/* descide which is best grade */
void	mmi_gx_magicsushi_enter_game(void);										/* entry function of the game */
void	mmi_gx_magicsushi_exit_game(void);											/* exit function - usually will stop timer and release buffer */
void	mmi_gx_magicsushi_draw_gameover(void);									/* draw gameover screen */

	
/* Game play functions */
void	mmi_gx_magicsushi_framemove(void);
void	mmi_gx_magicsushi_render(void);
void	mmi_gx_magicsushi_gameover(void);
void	mmi_gx_magicsushi_cyclic_timer(void);
void	mmi_gx_magicsushi_init_game(void);											/* draw gameover screen */	

U8		mmi_gx_magicsushi_check_pair(U8 x1,U8 y1, U8 x2,U8 y2,U8 horizontal);
U8		mmi_gx_magicsushi_check_all(void);
void	mmi_gx_magicsushi_gen_map(void);
void	mmi_gx_magicsushi_update_game_data(void);
void	mmi_gx_magicsushi_update_map(void);  // slow version  
U8		mmi_gx_magicsushi_is_moremove(void);
void  mmi_gx_magicsushi_show_magicsushi(void);
void mmi_gx_magicsushi_draw_dynamic_background(void);
#ifdef __MMI_TOUCH_SCREEN__
void	mmi_gx_magicsushi_pen_down_hdlr(mmi_pen_point_struct pos);
void 	mmi_gx_magicsushi_pen_up_hdlr(mmi_pen_point_struct pos);
#endif
void 	mmi_gx_magicsushi_key_2_release(void);
void 	mmi_gx_magicsushi_key_4_release(void);
void 	mmi_gx_magicsushi_key_5_release(void);
void 	mmi_gx_magicsushi_key_6_release(void);
void 	mmi_gx_magicsushi_key_8_release(void);
void        UI_start_timer(S32 count, void (*callback) (void));
void        UI_cancel_timer(void (*callback) (void));

/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_enter_gfx()
* DESCRIPTION
*	Set Game Framework (GFX) Parameter
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
void mmi_gx_magicsushi_enter_gfx(void)
{
	/* Game menu */
	GFX.game_data.game_img_id					= IMG_ID_GX_MAGICSUSHI_GAME_ICON;	/* game icon img ID */
	GFX.game_data.game_str_id					= STR_ID_GX_MAGICSUSHI_GAME_NAME;	/* game name string ID */
	GFX.game_data.menu_resume_str_id			= STR_GAME_RESUME;						/* "Resume" string ID */	
	GFX.game_data.menu_new_str_id				= STR_GAME_NEW;							/* "New Game" string ID */
	GFX.game_data.menu_level_str_id			= 0;											/* "Game Level" string ID */
	GFX.game_data.menu_grade_str_id			= STR_GAME_GRADE;							/* "Best Grade" string ID */
	GFX.game_data.menu_help_str_id			= STR_GAME_HELP;							/* "Game Help" string ID */
	
	/* level / grade */
	GFX.game_data.level_count					= 1;											/* how many levels */
	GFX.game_data.level_str_id_list[0]		= STR_GAME_SCORE;							/* level string ID */
	
	/* add slot in NVRAMEnum.h */
	GFX.game_data.grade_nvram_id_list[0]	= NVRAM_GX_MAGICSUSHI_SCORE;			/* grade slot in NVRAM (short) */
	//GFX.game_data.grade_nvram_id_list[1]	= DUMMY_NVRAM_ID;							/* grade slot in NVRAM */
	//GFX.game_data.grade_nvram_id_list[2]	= DUMMY_NVRAM_ID;							/* grade slot in NVRAM */
	//GFX.game_data.level_nvram_id			= DUMMY_NVRAM_ID;							/* current lvl idnex  in NVRAM (byte) */

	/* help */	
	GFX.game_data.help_str_id					= STR_ID_GX_MAGICSUSHI_HELP_DESCRIPTION;	/* help desciption string id */

	/* misc */
	GFX.game_data.grade_value_ptr				= (S16*)(&g_gx_magicsushi_context.game_grade);	/* current level's grade (S16*) */
	GFX.game_data.level_index_ptr				= (U8*)(&g_gx_magicsushi_context.game_level);	/* ptr to current level index (U8*) */
	GFX.game_data.is_new_game					= (BOOL*)(&g_gx_magicsushi_context.is_new_game);/* ptr to new game flag (BOOL*) */ 
	
	/* function ptr */
	GFX.game_data.best_grade_func_ptr		= mmi_gx_magicsushi_calc_best_grade;				/* function to calculate best grade */
	GFX.game_data.enter_game_func_ptr		= mmi_gx_magicsushi_enter_game;						/* function to enter new game */
	GFX.game_data.exit_game_func_ptr			= mmi_gx_magicsushi_exit_game;						/* function to exit game */
	GFX.game_data.draw_gameover_func_ptr	= mmi_gx_magicsushi_draw_gameover;					/* function to draw gameover screen */
	
	/* some flags */
	GFX.game_data.is_keypad_audio_enable	= FALSE;					/* play keypad tone or not */

	mmi_gfx_entry_menu_screen();
}
	
/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_draw_dynamic_background
* DESCRIPTION
*	draw dynamic background
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
void mmi_gx_magicsushi_draw_dynamic_background(void)
{
	S16		tmp_digit;
	S16		tmp_grade;
	S16		position_x;
	float32 	tmp;
	float32 	tmpfloat;

	gdi_layer_push_clip();
	gdi_layer_set_clip(MMI_GX_MAGICSUSHI_BACKGROUND1_X1,MMI_GX_MAGICSUSHI_BACKGROUND1_Y1,MMI_GX_MAGICSUSHI_BACKGROUND1_X2,MMI_GX_MAGICSUSHI_BACKGROUND1_Y2);
	gdi_draw_solid_rect(MMI_GX_MAGICSUSHI_BACKGROUND1_X1,MMI_GX_MAGICSUSHI_BACKGROUND1_Y1,MMI_GX_MAGICSUSHI_BACKGROUND1_X2,MMI_GX_MAGICSUSHI_BACKGROUND1_Y2,GDI_COLOR_TRANSPARENT)	;
	
	/* draw score */
	position_x = MMI_GX_MAGICSUSHI_SCORE_POSITION_X;
	tmp_grade = g_gx_magicsushi_context.game_grade;
	do	{
		tmp_digit = tmp_grade % 10;
		tmp_grade = tmp_grade / 10;
			
		gdi_image_draw_id(position_x, MMI_GX_MAGICSUSHI_SCORE_POSITION_Y, (U16)(IMG_ID_GX_MAGICSUSHI_NUMBER_0 + tmp_digit));
		position_x = position_x - MMI_GX_MAGICSUSHI_DIGIT_WIDTH ;
	}while(tmp_grade>0);
	
	/* draw_level */
	position_x = MMI_GX_MAGICSUSHI_LEVEL_POSITION_X;
	tmp_grade = g_gx_magicsushi_context.level;

	do	{
		tmp_digit = tmp_grade % 10;
		tmp_grade = tmp_grade / 10;
			 
		gdi_image_draw_id(position_x, MMI_GX_MAGICSUSHI_LEVEL_POSITION_Y, (U16)(IMG_ID_GX_MAGICSUSHI_NUMBER_0 + tmp_digit));
		position_x = position_x - MMI_GX_MAGICSUSHI_DIGIT_WIDTH ;
	}while(tmp_grade>0);

	gdi_layer_pop_clip();
	gdi_layer_push_clip();
	gdi_layer_set_clip(MMI_GX_MAGICSUSHI_BACKGROUND2_X1,MMI_GX_MAGICSUSHI_BACKGROUND2_Y1,MMI_GX_MAGICSUSHI_BACKGROUND2_X2,MMI_GX_MAGICSUSHI_BACKGROUND2_Y2);

	/* draw left time */	
	position_x = MMI_GX_MAGICSUSHI_TIME_POSITION_X;
	if(g_gx_magicsushi_context.remain_time>g_gx_magicsushi_context.total_time)
		g_gx_magicsushi_context.remain_time=g_gx_magicsushi_context.total_time;
	
	tmp = g_gx_magicsushi_context.remain_time;
	tmpfloat =tmp/ g_gx_magicsushi_context.total_time;
	tmp = 157 * tmpfloat;
	gdi_draw_solid_rect(position_x, MMI_GX_MAGICSUSHI_TIME_POSITION_Y,position_x+157 , MMI_GX_MAGICSUSHI_TIME_POSITION_Y+4,GDI_COLOR_TRANSPARENT);
	gdi_draw_solid_rect(position_x, MMI_GX_MAGICSUSHI_TIME_POSITION_Y,(S32)(position_x+tmp) , MMI_GX_MAGICSUSHI_TIME_POSITION_Y+4,GDI_COLOR_GREEN);
	
	/* draw remainder */
	position_x = MMI_GX_MAGICSUSHI_REMAINDER_POSITION_X;
	if(g_gx_magicsushi_context.remainder <= 0)
		g_gx_magicsushi_context.remainder = 0;
	
	tmp = (float)(50 - g_gx_magicsushi_context.remainder);
	tmpfloat =tmp/ 50;
	tmp = 157 * tmpfloat;
	gdi_draw_solid_rect(position_x, MMI_GX_MAGICSUSHI_REMAINDER_POSITION_Y,position_x+157 , MMI_GX_MAGICSUSHI_REMAINDER_POSITION_Y+4,GDI_COLOR_TRANSPARENT);
	gdi_draw_solid_rect(position_x, MMI_GX_MAGICSUSHI_REMAINDER_POSITION_Y,(S32)(position_x+tmp) , MMI_GX_MAGICSUSHI_REMAINDER_POSITION_Y+4,GDI_COLOR_RED);

	gdi_layer_pop_clip();
}

/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_calc_best_grade
* DESCRIPTION
*	Calculate new best grade [Callback required by GFX]
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
S16 mmi_gx_magicsushi_calc_best_grade(S16 old_grade, S16 new_grade)
{
	if (old_grade == 0)
		return new_grade;

	if (new_grade > old_grade)
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
*	mmi_gx_magicsushi_render
* DESCRIPTION
*	Render the game images		
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
void mmi_gx_magicsushi_render(void)
{
	/* add drawing code here */
//	U8					i,j,item;

	U8					i,j,k,item;
	S16 					x,y;
	U8		count;
	float32 tmp;
	float32 tmpfloat;
	float32 tmpfloat2;

	/* initialize */
	i = 0;
	j = 0;

	gdi_layer_set_active(g_gx_magicsushi_context.foreground);
	gdi_layer_push_clip();
	gdi_layer_set_clip(MMI_GX_MAGICSUSHI_ORIGIN_X, MMI_GX_MAGICSUSHI_ORIGIN_Y, MMI_GX_MAGICSUSHI_LCD_WIDTH - 1, MMI_GX_MAGICSUSHI_LCD_HEIGHT - 1);

	/* softkey */
	if(g_gx_magicsushi_context.softkey_state == 1)
	{
		gdi_draw_solid_rect(MMI_GX_MAGICSUSHI_SOFTKEY_X,MMI_GX_MAGICSUSHI_SOFTKEY_Y,MMI_GX_MAGICSUSHI_SOFTKEY_X+MMI_GX_MAGICSUSHI_SOFTKEY_WIDTH,MMI_GX_MAGICSUSHI_SOFTKEY_Y+MMI_GX_MAGICSUSHI_SOFTKEY_HEIGHT,GDI_COLOR_TRANSPARENT);
		gdi_image_draw_id(MMI_GX_MAGICSUSHI_SOFTKEY_X,MMI_GX_MAGICSUSHI_SOFTKEY_Y,IMG_ID_GX_MAGICSUSHI_DOWN);
		g_gx_magicsushi_context.softkey_state = 0;
	}
	else if (g_gx_magicsushi_context.softkey_state == 2)
	{
		gdi_draw_solid_rect(MMI_GX_MAGICSUSHI_SOFTKEY_X,MMI_GX_MAGICSUSHI_SOFTKEY_Y,MMI_GX_MAGICSUSHI_SOFTKEY_X+MMI_GX_MAGICSUSHI_SOFTKEY_WIDTH,MMI_GX_MAGICSUSHI_SOFTKEY_Y+MMI_GX_MAGICSUSHI_SOFTKEY_HEIGHT,GDI_COLOR_TRANSPARENT);
		gdi_image_draw_id(MMI_GX_MAGICSUSHI_SOFTKEY_X,MMI_GX_MAGICSUSHI_SOFTKEY_Y,IMG_ID_GX_MAGICSUSHI_UP);
		g_gx_magicsushi_context.softkey_state = 0;
	}
       TRACE_EF(g_sw_GAME,"MMI_GAME: g_gx_magicsushi_context.last_key=%d",g_gx_magicsushi_context.last_key);
	/* draw cursor x y */
	if(g_gx_magicsushi_context.last_key!=0)
	{

		switch(g_gx_magicsushi_context.last_key)
		{
			case 1:
				i = g_gx_magicsushi_context.special_x;
				j = g_gx_magicsushi_context.special_y;
			break;
			case 2:
				i = g_gx_magicsushi_context.cursor_x ;
				j = g_gx_magicsushi_context.cursor_y - 1;
			break;
			case 4:
				i = g_gx_magicsushi_context.cursor_x + 1 ;
				j = g_gx_magicsushi_context.cursor_y;
			break;
			case 6:
				i = g_gx_magicsushi_context.cursor_x - 1;
				j = g_gx_magicsushi_context.cursor_y;
			break;
			case 8:
				i = g_gx_magicsushi_context.cursor_x ;
				j = g_gx_magicsushi_context.cursor_y + 1;
			break;			
		}

		y = (7 - j) * MMI_GX_MAGICSUSHI_SIZE+ MMI_GX_MAGICSUSHI_TOP_WIDTH;
		x = i * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
		gdi_draw_solid_rect(x,y,x+MMI_GX_MAGICSUSHI_SIZE,y+MMI_GX_MAGICSUSHI_SIZE,GDI_COLOR_TRANSPARENT);
		gdi_image_draw_id(x,y,(U16)(IMG_ID_GX_MAGICSUSHI_TYPE_0 + g_gx_magicsushi_context.map[i][j]));
		g_gx_magicsushi_context.last_key	= 0;
	}

		y = (7 - g_gx_magicsushi_context.cursor_y) * MMI_GX_MAGICSUSHI_SIZE+ MMI_GX_MAGICSUSHI_TOP_WIDTH;
		x = g_gx_magicsushi_context.cursor_x * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
		gdi_image_draw_id(x,y,IMG_ID_GX_MAGICSUSHI_CURSOR);
	
	
	/* draw selected item */
	if(g_gx_magicsushi_context.selected==1)
	{
		y = (7 - g_gx_magicsushi_context.old_y) * MMI_GX_MAGICSUSHI_SIZE+ MMI_GX_MAGICSUSHI_TOP_WIDTH;
		x = g_gx_magicsushi_context.old_x * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
		gdi_image_draw_id(x,y,IMG_ID_GX_MAGICSUSHI_SELECTED);
	}
	else if (g_gx_magicsushi_context.selected==2)
	{
		g_gx_magicsushi_context.selected=0;
		y = (7 - g_gx_magicsushi_context.old_y) * MMI_GX_MAGICSUSHI_SIZE+ MMI_GX_MAGICSUSHI_TOP_WIDTH;
		x = g_gx_magicsushi_context.old_x * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
		gdi_draw_solid_rect(x,y,x+MMI_GX_MAGICSUSHI_SIZE,y+MMI_GX_MAGICSUSHI_SIZE,GDI_COLOR_TRANSPARENT)	;
		gdi_image_draw_id(x,y,(U16)(IMG_ID_GX_MAGICSUSHI_TYPE_0+g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x ][g_gx_magicsushi_context.old_y]));

	}
       TRACE_EF(g_sw_GAME,"MMI_GAME: g_gx_magicsushi_context.animation_mode=%d",g_gx_magicsushi_context.animation_mode);
	/* handling animation */
	switch(g_gx_magicsushi_context.animation_mode)
	{
		case 0:
		/* do nothing */
			break;
		case 1:
		/* do nothnig */
			break;
		case 2:
		/* magic1: delete random selected type item */
			g_gx_magicsushi_context.animation_step--;
			count=0;
			y = (7 - g_gx_magicsushi_context.old_y) * MMI_GX_MAGICSUSHI_SIZE+ MMI_GX_MAGICSUSHI_TOP_WIDTH;
			x = g_gx_magicsushi_context.old_x * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
			gdi_draw_solid_rect(x,y,x+MMI_GX_MAGICSUSHI_SIZE,y+MMI_GX_MAGICSUSHI_SIZE,GDI_COLOR_TRANSPARENT);
			gdi_image_draw_id(x,y,(U16)(IMG_ID_GX_MAGICSUSHI_TYPE_0 + g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x][g_gx_magicsushi_context.old_y]));

			item =  g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x][g_gx_magicsushi_context.old_y];
			if(g_gx_magicsushi_context.animation_step==0)
			{ 
				for(i=0;i<8;i++)
					for(j=0;j<8;j++)
						if(g_gx_magicsushi_context.map[i][j]== item)
							{
								count++;
								y = (7 - j) * MMI_GX_MAGICSUSHI_SIZE+ MMI_GX_MAGICSUSHI_TOP_WIDTH;
								x = i * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
								gdi_image_draw_id(x,y,IMG_ID_GX_MAGICSUSHI_SELECTED);
								g_gx_magicsushi_context.map[i][j]=255;

							}
				g_gx_magicsushi_context.game_grade += count*g_gx_magicsushi_context.level;
				count /= 3;
				g_gx_magicsushi_context.remainder -= count;
				g_gx_magicsushi_context.remain_time += 3;

				g_gx_magicsushi_context.animation_step = MMI_GX_MAGICSUSHI_ANIMATION_STEP;
				g_gx_magicsushi_context.animation_mode = 8;

			}
			break;
		case 3:
		/* magic2: delete items by 3x3 matrix */
			g_gx_magicsushi_context.animation_step--;
			if(g_gx_magicsushi_context.animation_step == 0)
			{
				for(i=0;i<=2;i++)
				{
					if(g_gx_magicsushi_context.old_x+i-1 <0 || g_gx_magicsushi_context.old_x+i-1 >7)
						continue;
					else 
						x =(g_gx_magicsushi_context.old_x+i-1) * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
						
					for(j=0;j<=2;j++)
					{
						if(g_gx_magicsushi_context.old_y+j-1 <0 || g_gx_magicsushi_context.old_y+j-1 >7)
							continue;
						else 
						{
							y = (8 - g_gx_magicsushi_context.old_y-j) * MMI_GX_MAGICSUSHI_SIZE+ MMI_GX_MAGICSUSHI_TOP_WIDTH;
							gdi_image_draw_id(x,y,IMG_ID_GX_MAGICSUSHI_SELECTED);

							g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x+i-1][g_gx_magicsushi_context.old_y+j-1]=255;
						}
					}
				}
				count = 3;
				g_gx_magicsushi_context.game_grade += g_gx_magicsushi_context.level*count;
				g_gx_magicsushi_context.remainder -= count;
				g_gx_magicsushi_context.remain_time += 3;
				g_gx_magicsushi_context.animation_step = MMI_GX_MAGICSUSHI_ANIMATION_STEP;
				g_gx_magicsushi_context.animation_mode = 8;
			}

			break;
		case 4:
		/* magic3: delete items by cross deletion */
			g_gx_magicsushi_context.animation_step--;
			if(g_gx_magicsushi_context.animation_step == 0)
			{
				x =(g_gx_magicsushi_context.old_x) * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
				for(i=0;i<8;i++)
				{
					y = (7-i) * MMI_GX_MAGICSUSHI_SIZE+ MMI_GX_MAGICSUSHI_TOP_WIDTH;
					gdi_image_draw_id(x,y,IMG_ID_GX_MAGICSUSHI_SELECTED);
					g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x][i]=255;

				}
				y = (7 - g_gx_magicsushi_context.old_y)* MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_TOP_WIDTH;
				for(i=0;i<8;i++)
				{
					x = (i) * MMI_GX_MAGICSUSHI_SIZE+ MMI_GX_MAGICSUSHI_LEFT_WIDTH;
					gdi_image_draw_id(x,y,IMG_ID_GX_MAGICSUSHI_SELECTED);
					g_gx_magicsushi_context.map[i][g_gx_magicsushi_context.old_y]=255;

				}
				g_gx_magicsushi_context.game_grade += 5*g_gx_magicsushi_context.level;
				g_gx_magicsushi_context.remainder -= 5;
				g_gx_magicsushi_context.remain_time += 3;
				g_gx_magicsushi_context.animation_step = MMI_GX_MAGICSUSHI_ANIMATION_STEP;
				g_gx_magicsushi_context.animation_mode = 8;
			}
			break;
		case 5:
		/* up-down switch */
			g_gx_magicsushi_context.animation_step--;
			tmp = (float)(MMI_GX_MAGICSUSHI_ANIMATION_STEP - g_gx_magicsushi_context.animation_step % MMI_GX_MAGICSUSHI_ANIMATION_STEP);
			tmpfloat = tmp/MMI_GX_MAGICSUSHI_ANIMATION_STEP;

			x = g_gx_magicsushi_context.old_x * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
			y = (7 - g_gx_magicsushi_context.old_y )*MMI_GX_MAGICSUSHI_SIZE+ MMI_GX_MAGICSUSHI_TOP_WIDTH;
			gdi_draw_solid_rect(x,y,x+MMI_GX_MAGICSUSHI_SIZE,y+MMI_GX_MAGICSUSHI_SIZE,GDI_COLOR_TRANSPARENT)	;

			y = (7 - g_gx_magicsushi_context.special_y )*MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_TOP_WIDTH;
			gdi_draw_solid_rect(x,y,x+MMI_GX_MAGICSUSHI_SIZE,y+MMI_GX_MAGICSUSHI_SIZE,GDI_COLOR_TRANSPARENT)	;

			
			x = g_gx_magicsushi_context.old_x * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
			y = (7 - g_gx_magicsushi_context.old_y )*MMI_GX_MAGICSUSHI_SIZE 
					 + (S32)(tmpfloat * MMI_GX_MAGICSUSHI_SIZE )+ MMI_GX_MAGICSUSHI_TOP_WIDTH;
			gdi_image_draw_id(x,y,(U16)(IMG_ID_GX_MAGICSUSHI_TYPE_0 + g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x][g_gx_magicsushi_context.old_y]));

			y = (7 - g_gx_magicsushi_context.special_y )*MMI_GX_MAGICSUSHI_SIZE 
					 - (S32)(tmpfloat * MMI_GX_MAGICSUSHI_SIZE)+ MMI_GX_MAGICSUSHI_TOP_WIDTH ;
			gdi_image_draw_id(x,y,(U16)(IMG_ID_GX_MAGICSUSHI_TYPE_0 + g_gx_magicsushi_context.map[g_gx_magicsushi_context.special_x][g_gx_magicsushi_context.special_y]));

			if(g_gx_magicsushi_context.animation_step == 0)
			{			
				tmp = g_gx_magicsushi_context.map[g_gx_magicsushi_context.special_x][g_gx_magicsushi_context.special_y];
				g_gx_magicsushi_context.map [g_gx_magicsushi_context.special_x][g_gx_magicsushi_context.special_y] = g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x][g_gx_magicsushi_context.old_y];
				g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x][g_gx_magicsushi_context.old_y] = (U8)tmp;
				if(g_gx_magicsushi_context.check && !mmi_gx_magicsushi_check_pair(g_gx_magicsushi_context.old_x,g_gx_magicsushi_context.old_y,g_gx_magicsushi_context.special_x,g_gx_magicsushi_context.special_y,0))
				{
					g_gx_magicsushi_context.check  =  0;
					g_gx_magicsushi_context.animation_step =3;
					g_gx_magicsushi_context.animation_mode =5;
					g_gx_magicsushi_context.click_mode = 0;
				}
				else if (g_gx_magicsushi_context.check)
				{
					mmi_gx_magicsushi_update_game_data();  
					g_gx_magicsushi_context.click_mode = 0;
					g_gx_magicsushi_context.check  =  0;
				}
				else 
				{
					g_gx_magicsushi_context.animation_mode = 0;
					g_gx_magicsushi_context.moving = 0;
					g_gx_magicsushi_context.click_mode = 0;
					g_gx_magicsushi_context.check  =  0;
				}
			}
				break;
		case 7:
		/* left-right switch */
			g_gx_magicsushi_context.animation_step--;
			tmp = (float)(MMI_GX_MAGICSUSHI_ANIMATION_STEP - g_gx_magicsushi_context.animation_step % MMI_GX_MAGICSUSHI_ANIMATION_STEP);
			tmpfloat = tmp/MMI_GX_MAGICSUSHI_ANIMATION_STEP;

			x = g_gx_magicsushi_context.old_x * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
			y = (7 - g_gx_magicsushi_context.old_y )*MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_TOP_WIDTH;					 
			gdi_draw_solid_rect(x,y,x+MMI_GX_MAGICSUSHI_SIZE,y+MMI_GX_MAGICSUSHI_SIZE,GDI_COLOR_TRANSPARENT)	;

			x = g_gx_magicsushi_context.special_x * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH ;
			gdi_draw_solid_rect(x,y,x+MMI_GX_MAGICSUSHI_SIZE,y+MMI_GX_MAGICSUSHI_SIZE,GDI_COLOR_TRANSPARENT)	;

			x = g_gx_magicsushi_context.old_x * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH- (S32)(tmpfloat * MMI_GX_MAGICSUSHI_SIZE );
			y = (7 - g_gx_magicsushi_context.old_y )*MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_TOP_WIDTH;					 
			gdi_image_draw_id(x,y,(U16)(IMG_ID_GX_MAGICSUSHI_TYPE_0 + g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x][g_gx_magicsushi_context.old_y]));

			x = g_gx_magicsushi_context.special_x * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH + (S32)(tmpfloat * MMI_GX_MAGICSUSHI_SIZE) ;
			gdi_image_draw_id(x,y,(U16)(IMG_ID_GX_MAGICSUSHI_TYPE_0 + g_gx_magicsushi_context.map[g_gx_magicsushi_context.special_x][g_gx_magicsushi_context.special_y]));

			if(g_gx_magicsushi_context.animation_step == 0)
			{
				tmp = g_gx_magicsushi_context.map[g_gx_magicsushi_context.special_x][g_gx_magicsushi_context.special_y];
				g_gx_magicsushi_context.map [g_gx_magicsushi_context.special_x][g_gx_magicsushi_context.special_y] = g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x][g_gx_magicsushi_context.old_y];
				g_gx_magicsushi_context.map[g_gx_magicsushi_context.old_x][g_gx_magicsushi_context.old_y] = (U8)tmp;
				if(g_gx_magicsushi_context.check && !mmi_gx_magicsushi_check_pair(g_gx_magicsushi_context.old_x,g_gx_magicsushi_context.old_y,g_gx_magicsushi_context.special_x,g_gx_magicsushi_context.special_y,1))
				{
					g_gx_magicsushi_context.check  =  0;
					g_gx_magicsushi_context.animation_step =3;
					g_gx_magicsushi_context.animation_mode =7;
					g_gx_magicsushi_context.click_mode = 0;
				}
				else if (g_gx_magicsushi_context.check)
				{
					mmi_gx_magicsushi_update_game_data();
					g_gx_magicsushi_context.click_mode = 0;
					g_gx_magicsushi_context.check  =  0;
				}else 
				{
					g_gx_magicsushi_context.animation_mode = 0;
					g_gx_magicsushi_context.moving = 0;
					g_gx_magicsushi_context.click_mode = 0;
					g_gx_magicsushi_context.check  =  0;
				}
			}
			break;
		case 6:
			/* dropping */
			g_gx_magicsushi_context.animation_step--;
			g_gx_magicsushi_context.tick=0;
			tmp = (float)( g_gx_magicsushi_context.animation_step % MMI_GX_MAGICSUSHI_ANIMATION_STEP);
			tmpfloat = tmp/MMI_GX_MAGICSUSHI_ANIMATION_STEP;
			tmpfloat2 = (tmp+1)	/MMI_GX_MAGICSUSHI_ANIMATION_STEP;

			for (i=0;i<8;i++)
			{
				k=0;
				tmp = (float)(g_gx_magicsushi_context.animation_step % MMI_GX_MAGICSUSHI_ANIMATION_STEP);
				for(j=0;j<8;j++)
				{
					if(j!=g_gx_magicsushi_context.i_map[i][j])
					{
						x = i * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
						y = (8 - g_gx_magicsushi_context.i_map[i][j])*MMI_GX_MAGICSUSHI_SIZE 
							+ MMI_GX_MAGICSUSHI_TOP_WIDTH - (S32)(tmpfloat2 * MMI_GX_MAGICSUSHI_SIZE) ;
						gdi_draw_solid_rect(x,y,x+MMI_GX_MAGICSUSHI_SIZE,y+MMI_GX_MAGICSUSHI_SIZE,GDI_COLOR_TRANSPARENT);

						y = (8 - g_gx_magicsushi_context.i_map[i][j])*MMI_GX_MAGICSUSHI_SIZE 
							+ MMI_GX_MAGICSUSHI_TOP_WIDTH - (S32)(tmpfloat * MMI_GX_MAGICSUSHI_SIZE );
						gdi_image_draw_id(x,y,(U16)(IMG_ID_GX_MAGICSUSHI_TYPE_0 + g_gx_magicsushi_context.map[i][j]));

						if(k!=0 && (g_gx_magicsushi_context.i_map[i][j]-k)>1)
						{
							x = i * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
							y = (7 - k)*MMI_GX_MAGICSUSHI_SIZE 
								+ MMI_GX_MAGICSUSHI_TOP_WIDTH - (S32)(tmpfloat * MMI_GX_MAGICSUSHI_SIZE );
							gdi_draw_solid_rect(x,y,x+MMI_GX_MAGICSUSHI_SIZE,y+MMI_GX_MAGICSUSHI_SIZE,GDI_COLOR_TRANSPARENT)	;
						}
						k = g_gx_magicsushi_context.i_map[i][j];
						
						if(tmp == 0)
						{	
							g_gx_magicsushi_context.i_map[i][j]--;
							if(g_gx_magicsushi_context.i_map[i][j]==7)
							break;
						}
						
						if(g_gx_magicsushi_context.i_map[i][j]==8)
							break;						
					}
				}
			}
			if(g_gx_magicsushi_context.animation_step == 0 )
			{
				if(mmi_gx_magicsushi_check_all())
					mmi_gx_magicsushi_update_game_data();

				else if ( g_gx_magicsushi_context.remainder <= 0 )
				{
					g_gx_magicsushi_context.bonus = 1;
					g_gx_magicsushi_context.total_time -= 5;
					g_gx_magicsushi_context.remain_time=g_gx_magicsushi_context.total_time/2;
					g_gx_magicsushi_context.remainder = 50;
					g_gx_magicsushi_context.level++;
					g_gx_magicsushi_context.animation_step = 6;
					g_gx_magicsushi_context.animation_mode = 11;
					g_gx_magicsushi_context.special = 0;
					if(g_gx_magicsushi_context.level>4)
						g_gx_magicsushi_context.type_num=8;										
				}
				else if(!mmi_gx_magicsushi_is_moremove())
				{
					g_gx_magicsushi_context.bonus = 1;
					g_gx_magicsushi_context.animation_step = 6;
					g_gx_magicsushi_context.animation_mode = 10;
					g_gx_magicsushi_context.special = 0;
				}
				else
				{
					g_gx_magicsushi_context.click_mode= 0;
					g_gx_magicsushi_context.animation_mode = 0;
					g_gx_magicsushi_context.moving = 0;
					g_gx_magicsushi_context.bonus = 1;
			 	}	
			}
		break;
        
		case 8:
			/* update image for items being removed */
			for (i=0;i<8;i++)
			{
				for(j=0;j<8;j++)
				{
					if(g_gx_magicsushi_context.map[i][j]==255)
					{
						x = i * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
						y = (7-j) * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_TOP_WIDTH;
						gdi_image_draw_id(x,y,IMG_ID_GX_MAGICSUSHI_TYPE_NULL);
					}
				}
			}
        #ifdef __MMI_GAME_MULTICHANNEL_SOUND__
				GFX_STOP_SOUND_EFFECTS_MIDI(g_gx_magicsushi_context.move_midi);
				GFX_STOP_SOUND_EFFECTS_MIDI(g_gx_magicsushi_context.select_midi);

				GFX_PLAY_SOUND_EFFECTS_MIDI(MAGICSUSHIMove,MAGICSUSHIMOVE, &(g_gx_magicsushi_context.move_midi),DEVICE_AUDIO_PLAY_ONCE);
			#else //__MMI_GAME_MULTICHANNEL_SOUND__	
				GFX_PLAY_AUDIO_MIDI(MAGICSUSHIMove, MAGICSUSHIMOVE, DEVICE_AUDIO_PLAY_ONCE);	
			#endif //__MMI_GAME_MULTICHANNEL_SOUND__	



			g_gx_magicsushi_context.animation_mode = 9;
		break;
		case 9:
			/* clear image for items being removed */
			for (i=0;i<8;i++)
			{
				for(j=0;j<8;j++)
				{
					if(g_gx_magicsushi_context.map[i][j]==255)
					{
						x = i * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
						y = (7-j) * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_TOP_WIDTH;
						gdi_draw_solid_rect(x,y,x+MMI_GX_MAGICSUSHI_SIZE,y+MMI_GX_MAGICSUSHI_SIZE,GDI_COLOR_TRANSPARENT)	;

					}
				}
			}
			mmi_gx_magicsushi_update_map();	
			break;
		case 10:
			/* show no more move */
			g_gx_magicsushi_context.animation_step--;
			gdi_image_draw_id(20, 80, IMG_ID_GX_MAGICSUSHI_NOMOREMOVE);
			
			if(g_gx_magicsushi_context.animation_step == 0)
			{
				gdi_draw_solid_rect(MMI_GX_MAGICSUSHI_ORIGIN_X, MMI_GX_MAGICSUSHI_ORIGIN_Y,MMI_GX_MAGICSUSHI_ORIGIN_X+232 ,MMI_GX_MAGICSUSHI_ORIGIN_Y+232 , GDI_COLOR_TRANSPARENT );
				mmi_gx_magicsushi_gen_map();
			}
			break;
		case 11:
			/* show uplevel */
			g_gx_magicsushi_context.animation_step--;
			gdi_image_draw_id(20, 80, IMG_ID_GX_MAGICSUSHI_UPLEVEL);
			
			if(g_gx_magicsushi_context.animation_step == 0)
			{
				gdi_draw_solid_rect(MMI_GX_MAGICSUSHI_ORIGIN_X, MMI_GX_MAGICSUSHI_ORIGIN_Y,MMI_GX_MAGICSUSHI_ORIGIN_X+224 ,MMI_GX_MAGICSUSHI_ORIGIN_Y+224 , GDI_COLOR_TRANSPARENT );
				mmi_gx_magicsushi_gen_map();
			}
			break;
	}
	mmi_gx_magicsushi_draw_dynamic_background();
	
	gdi_layer_blt(	g_gx_magicsushi_context.background,	/* layer 0 */
						g_gx_magicsushi_context.foreground,	/* layer 1 */
						0,										/* layer 2 */
						0,										/* layer 3 */
						0,										/* offset x */
						0,										/* offset y */
						MMI_GX_MAGICSUSHI_LCD_WIDTH - 1,		/* width */
						MMI_GX_MAGICSUSHI_LCD_HEIGHT - 1);	/* height */

	gdi_layer_pop_clip();
	gdi_layer_set_active(g_gx_magicsushi_context.background);
	
}
#ifdef __MMI_TOUCH_SCREEN__
/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_pen_up_hdlr
* DESCRIPTION
*	pen up event handler
* PARAMETERS
*	pos IN position of pen	
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
void mmi_gx_magicsushi_pen_up_hdlr(mmi_pen_point_struct pos)
{
	S32 pos_x=pos.x,pos_y=pos.y;
	g_gx_magicsushi_context.softkey_state=2;
	if( pos_x > MMI_GX_MAGICSUSHI_SOFTKEY_X && pos_x < MMI_GX_MAGICSUSHI_SOFTKEY_X + MMI_GX_MAGICSUSHI_SOFTKEY_WIDTH 
		  && pos_y > MMI_GX_MAGICSUSHI_SOFTKEY_Y && pos_y <MMI_GX_MAGICSUSHI_SOFTKEY_Y + MMI_GX_MAGICSUSHI_SOFTKEY_HEIGHT)
	{
		GoBackHistory();
	}
}

/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_pen_down_hdlr
* DESCRIPTION
*	pen down event handler
* PARAMETERS
*	pos IN position of pen	
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
void mmi_gx_magicsushi_pen_down_hdlr(mmi_pen_point_struct pos)
{
	S32 pos_x=pos.x,pos_y=pos.y;
	S32 coord_x=0,coord_y=0;

	if( pos_x > MMI_GX_MAGICSUSHI_SOFTKEY_X && pos_x < MMI_GX_MAGICSUSHI_SOFTKEY_X + MMI_GX_MAGICSUSHI_SOFTKEY_WIDTH 
		  && pos_y > MMI_GX_MAGICSUSHI_SOFTKEY_Y && pos_y <MMI_GX_MAGICSUSHI_SOFTKEY_Y + MMI_GX_MAGICSUSHI_SOFTKEY_HEIGHT)
	{
		g_gx_magicsushi_context.softkey_state=1;
	}
       TRACE_EF(g_sw_GAME,"MMI_GAME: g_gx_magicsushi_context.moving=%d",g_gx_magicsushi_context.moving);
       TRACE_EF(g_sw_GAME,"MMI_GAME: pos_x=%d,pos_y=%d",pos_x,pos_y);

	if(!g_gx_magicsushi_context.moving)
	{
		coord_x=(pos_x-MMI_GX_MAGICSUSHI_ORIGIN_X)/MMI_GX_MAGICSUSHI_SIZE;
		coord_y= 7 - (pos_y-MMI_GX_MAGICSUSHI_ORIGIN_Y)/MMI_GX_MAGICSUSHI_SIZE;

		if(coord_x<8 && coord_x>=0 &&coord_y<8 && coord_y>=0 )
		{
			
			if (g_gx_magicsushi_context.click_mode==0) 
			{
            #ifdef __MMI_GAME_MULTICHANNEL_SOUND__
					GFX_STOP_SOUND_EFFECTS_MIDI(g_gx_magicsushi_context.move_midi);
					GFX_STOP_SOUND_EFFECTS_MIDI(g_gx_magicsushi_context.select_midi);

					GFX_PLAY_SOUND_EFFECTS_MIDI(MAGICSUSHISelect, MAGICSUSHISELECT,&(g_gx_magicsushi_context.select_midi),DEVICE_AUDIO_PLAY_ONCE);
				#else //__MMI_GAME_MULTICHANNEL_SOUND__	
					GFX_PLAY_AUDIO_MIDI(MAGICSUSHISelect, MAGICSUSHISELECT, DEVICE_AUDIO_PLAY_ONCE);	
				#endif //__MMI_GAME_MULTICHANNEL_SOUND__	
			


				g_gx_magicsushi_context.old_x = coord_x;
				g_gx_magicsushi_context.old_y = coord_y;

				g_gx_magicsushi_context.last_key	= 1;

				g_gx_magicsushi_context.special_x = g_gx_magicsushi_context.cursor_x; 
				g_gx_magicsushi_context.special_y = g_gx_magicsushi_context.cursor_y;
				
				g_gx_magicsushi_context.cursor_x = coord_x;
				g_gx_magicsushi_context.cursor_y = coord_y;
				g_gx_magicsushi_context.click_mode = 1;
				
				switch ( g_gx_magicsushi_context.map[coord_x][coord_y] )
				{
					case 8: 
					g_gx_magicsushi_context.map[coord_x][coord_y] = rand()%g_gx_magicsushi_context.type_num;
					g_gx_magicsushi_context.animation_mode = 2;
					g_gx_magicsushi_context.animation_step = MMI_GX_MAGICSUSHI_ANIMATION_STEP;
					g_gx_magicsushi_context.moving = 1;
					g_gx_magicsushi_context.special = 0;
					break;
					case 9:
					g_gx_magicsushi_context.moving = 1;
					g_gx_magicsushi_context.animation_mode = 3;
					g_gx_magicsushi_context.animation_step = MMI_GX_MAGICSUSHI_ANIMATION_STEP;
					g_gx_magicsushi_context.special = 0;
					break;
					case 10:
					g_gx_magicsushi_context.moving = 1;
					g_gx_magicsushi_context.animation_mode = 4;
					g_gx_magicsushi_context.animation_step = MMI_GX_MAGICSUSHI_ANIMATION_STEP;
					g_gx_magicsushi_context.special = 0;
					break;
					default:
					g_gx_magicsushi_context.selected = 1;
					break;
				}
				
			}
			else if(g_gx_magicsushi_context.click_mode==1)
			{
				g_gx_magicsushi_context.click_mode=1;
				if (ABS(g_gx_magicsushi_context.old_x-coord_x)+ABS(g_gx_magicsushi_context.old_y-coord_y)== 1)
				{

					g_gx_magicsushi_context.moving = 1;
					g_gx_magicsushi_context.check = 1;
					g_gx_magicsushi_context.animation_step = MMI_GX_MAGICSUSHI_ANIMATION_STEP;
					g_gx_magicsushi_context.selected = 0;

					g_gx_magicsushi_context.cursor_x = coord_x;
					g_gx_magicsushi_context.cursor_y = coord_y;

					/* up-down switch */
					if (ABS(g_gx_magicsushi_context.old_x-coord_x)==0) 
					{
						g_gx_magicsushi_context.animation_mode = 5;
						if(g_gx_magicsushi_context.old_y > coord_y)
						{
							g_gx_magicsushi_context.special_y = coord_y;
							g_gx_magicsushi_context.special_x = coord_x;
						}
						else
						{
							g_gx_magicsushi_context.special_x = g_gx_magicsushi_context.old_x;
							g_gx_magicsushi_context.special_y = g_gx_magicsushi_context.old_y;
							g_gx_magicsushi_context.old_x= coord_x;
							g_gx_magicsushi_context.old_y= coord_y;
						}
						
					}
					/* left-right switch */
					else  
					{
						g_gx_magicsushi_context.animation_mode = 7;
						if(g_gx_magicsushi_context.old_x > coord_x)
						{
							g_gx_magicsushi_context.special_y = coord_y;
							g_gx_magicsushi_context.special_x = coord_x;
						}
						else
						{
							g_gx_magicsushi_context.special_x = g_gx_magicsushi_context.old_x;
							g_gx_magicsushi_context.special_y = g_gx_magicsushi_context.old_y;
							g_gx_magicsushi_context.old_x= coord_x;
							g_gx_magicsushi_context.old_y= coord_y;
						}
					}
				}
				else if (ABS(g_gx_magicsushi_context.old_x-coord_x)+ABS(g_gx_magicsushi_context.old_y-coord_y)== 0)
				{
						g_gx_magicsushi_context.selected = 2;
						g_gx_magicsushi_context.click_mode= 0;
						g_gx_magicsushi_context.moving = 0;
				}else {	
				/* click others */
                #ifdef __MMI_GAME_MULTICHANNEL_SOUND__
					GFX_STOP_SOUND_EFFECTS_MIDI(g_gx_magicsushi_context.move_midi);
					GFX_STOP_SOUND_EFFECTS_MIDI(g_gx_magicsushi_context.select_midi);

					GFX_PLAY_SOUND_EFFECTS_MIDI(MAGICSUSHISelect, MAGICSUSHISELECT, &(g_gx_magicsushi_context.select_midi),DEVICE_AUDIO_PLAY_ONCE);
				#else //__MMI_GAME_MULTICHANNEL_SOUND__	
					GFX_PLAY_AUDIO_MIDI(MAGICSUSHISelect, MAGICSUSHISELECT, DEVICE_AUDIO_PLAY_ONCE);	
				#endif //__MMI_GAME_MULTICHANNEL_SOUND__	
	
					g_gx_magicsushi_context.old_x = coord_x;
					g_gx_magicsushi_context.old_y = coord_y;
					g_gx_magicsushi_context.last_key	= 1;

					g_gx_magicsushi_context.special_x = g_gx_magicsushi_context.cursor_x; 
					g_gx_magicsushi_context.special_y = g_gx_magicsushi_context.cursor_y;
								
					g_gx_magicsushi_context.cursor_x = coord_x;
					g_gx_magicsushi_context.cursor_y = coord_y;
					g_gx_magicsushi_context.click_mode = 1;
					
					switch ( g_gx_magicsushi_context.map[coord_x][coord_y] )
					{
						case 8: 
						g_gx_magicsushi_context.map[coord_x][coord_y] = rand()%g_gx_magicsushi_context.type_num;
						g_gx_magicsushi_context.animation_mode = 2;
						g_gx_magicsushi_context.animation_step = MMI_GX_MAGICSUSHI_ANIMATION_STEP;
						g_gx_magicsushi_context.moving = 1;
						g_gx_magicsushi_context.special = 0;
						break;
						case 9:
						g_gx_magicsushi_context.moving = 1;
						g_gx_magicsushi_context.animation_mode = 3;
						g_gx_magicsushi_context.animation_step = MMI_GX_MAGICSUSHI_ANIMATION_STEP;
						g_gx_magicsushi_context.special = 0;
						break;
						case 10:
						g_gx_magicsushi_context.moving = 1;
						g_gx_magicsushi_context.animation_mode = 4;
						g_gx_magicsushi_context.animation_step = MMI_GX_MAGICSUSHI_ANIMATION_STEP;
						g_gx_magicsushi_context.special = 0;
						break;
						default:
						g_gx_magicsushi_context.selected = 1;
						break;
					}
				}				
			}	
		}
	}
}
#endif
/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_draw_gameover
* DESCRIPTION
*	Draw Gameover Screen [Callback required by GFX]	
* PARAMETERS
*	void 
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
void mmi_gx_magicsushi_draw_gameover(void)
{
	S16	tmp_digit;
	S16	tmp_grade;
	S16	position_x;

//	GFX_PLAY_AUDIO_COMPLETE();     //fixed bug as bug 13979
	
	/* call this function to draw standard gameover screen */
	gdi_layer_set_active(g_gx_magicsushi_context.background);
	gdi_layer_push_clip();
	gdi_layer_set_clip(0, 0, MMI_GX_MAGICSUSHI_LCD_WIDTH - 1, MMI_GX_MAGICSUSHI_LCD_HEIGHT - 1);
	gdi_image_draw_id( 0,0,IMG_ID_GX_MAGICSUSHI_GAMEOVER);
	
	/* draw score */
	position_x = 130;
	
	tmp_grade = g_gx_magicsushi_context.game_grade;
	do	{
		tmp_digit = tmp_grade % 10;
		tmp_grade = tmp_grade / 10;
			
		gdi_image_draw_id(position_x, 130, (U16)(IMG_ID_GX_MAGICSUSHI_NUMBER_0 + tmp_digit));
		position_x = position_x - MMI_GX_MAGICSUSHI_DIGIT_WIDTH ;
	}while(tmp_grade>0);
	
	gdi_layer_blt(	g_gx_magicsushi_context.background,	/* layer 0 */
						0,	/* layer 1 */
						0,										/* layer 2 */
						0,										/* layer 3 */
						0,										/* offset x */
						0,										/* offset y */
						MMI_GX_MAGICSUSHI_LCD_WIDTH - 1,		/* width */
						MMI_GX_MAGICSUSHI_LCD_HEIGHT - 1);	/* height */
	gdi_layer_pop_clip();
}

/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_enter_game
* DESCRIPTION
*	Enter Game [Callback required by GFX]	
* PARAMETERS
*	void 
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
void mmi_gx_magicsushi_enter_game(void)
{
	gdi_layer_multi_layer_enable();
	
	gdi_layer_get_base_handle(&g_gx_magicsushi_context.background);
	
	gdi_layer_create(0, 0, MMI_GX_MAGICSUSHI_LCD_WIDTH, MMI_GX_MAGICSUSHI_LCD_HEIGHT, &g_gx_magicsushi_context.foreground);
	gdi_layer_set_active(g_gx_magicsushi_context.foreground);
	gdi_layer_set_source_key(TRUE, GDI_COLOR_BLUE);		/* set pure-blue as the transparent color in foreground */
	gdi_layer_clear_background(GDI_COLOR_BLUE);			/* clean foreground layer */

	TRACE_EF(g_sw_GAME,"MMI_GAME: g_gx_magicsushi_context.is_new_game=%d",g_gx_magicsushi_context.is_new_game);
	if(g_gx_magicsushi_context.is_new_game == TRUE )
		mmi_gx_magicsushi_init_game(); /* is new game, otherwise resume game */
	else 
		mmi_gx_magicsushi_show_magicsushi();

	g_gx_magicsushi_context.is_new_game = FALSE;
	g_gx_magicsushi_context.is_gameover = FALSE;

	gdi_layer_set_active(g_gx_magicsushi_context.background);

	gdi_image_draw_id(0,0,IMG_ID_GX_MAGICSUSHI_GAME_BACKGROUND);
	TRACE_EF(g_sw_GAME,"MMI_GAME: IMG_ID_GX_MAGICSUSHI_GAME_BACKGROUND=%d",IMG_ID_GX_MAGICSUSHI_GAME_BACKGROUND);

	/*	draw softkay image */
	gdi_image_draw_id(MMI_GX_MAGICSUSHI_SOFTKEY_X,MMI_GX_MAGICSUSHI_SOFTKEY_Y,IMG_ID_GX_MAGICSUSHI_UP);

#ifdef __MMI_TOUCH_SCREEN__
			wgui_register_pen_down_handler(mmi_gx_magicsushi_pen_down_hdlr);			/* function to pen down event */
			wgui_register_pen_up_handler(mmi_gx_magicsushi_pen_up_hdlr);				/* function to pen up event */
#endif /* __MMI_TOUCH_SCREEN__ */ 
//	SetKeyHandler(mmi_gx_magicsushi_key_5_release, KEY_ENTER, KEY_EVENT_DOWN);
        	SetKeyHandler(mmi_gx_magicsushi_key_5_release, KEY_5, KEY_EVENT_DOWN);
        	SetKeyHandler(mmi_gx_magicsushi_key_2_release, KEY_2, KEY_EVENT_UP);
        	SetKeyHandler(mmi_gx_magicsushi_key_2_release, KEY_UP_ARROW, KEY_EVENT_UP);
        	SetKeyHandler(mmi_gx_magicsushi_key_4_release, KEY_4, KEY_EVENT_UP);
        	SetKeyHandler(mmi_gx_magicsushi_key_4_release, KEY_LEFT_ARROW, KEY_EVENT_UP);
        	SetKeyHandler(mmi_gx_magicsushi_key_6_release, KEY_6, KEY_EVENT_UP);
        	SetKeyHandler(mmi_gx_magicsushi_key_6_release, KEY_RIGHT_ARROW, KEY_EVENT_UP);
        	SetKeyHandler(mmi_gx_magicsushi_key_8_release, KEY_8, KEY_EVENT_UP);
        	SetKeyHandler(mmi_gx_magicsushi_key_8_release, KEY_DOWN_ARROW, KEY_EVENT_UP);
#ifdef __MMI_GAME_MULTICHANNEL_SOUND__
		GFX_OPEN_BACKGROUND_SOUND(MAGICSUSHIBackground,MAGICSUSHIBACKGROUND,g_gx_magicsushi_context.background_midi);
		GFX_PLAY_BACKGROUND_SOUND(MAGICSUSHIBackground,MAGICSUSHIBACKGROUND, &(g_gx_magicsushi_context.background_midi),DEVICE_AUDIO_PLAY_INFINITE);

		GFX_OPEN_DUMMY_BACKGROUND_SOUND();
		GFX_PLAY_DUMMY_BACKGROUND_SOUND();


		GFX_OPEN_SOUND_EFFECTS_MIDI(MAGICSUSHIMove,MAGICSUSHIMOVE,1,g_gx_magicsushi_context.move_midi);
		GFX_OPEN_SOUND_EFFECTS_MIDI(MAGICSUSHISelect,MAGICSUSHISELECT,1,g_gx_magicsushi_context.select_midi);
	#endif


	/* start game loop */
	mmi_gx_magicsushi_cyclic_timer();
}

/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_key_2_release
* DESCRIPTION
*	key 2 handling
* PARAMETERS
*	void 
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
void mmi_gx_magicsushi_key_2_release(void)
{
// Beginning--modified by kecx for fixing bug 12531 on 20090820
    if(g_gx_magicsushi_context.animation_mode != 6)
    {
	if(g_gx_magicsushi_context.cursor_y<7)
	{
		g_gx_magicsushi_context.cursor_y++;
		g_gx_magicsushi_context.last_key = 2;
	}
    }
    else
    {
	if(g_gx_magicsushi_context.cursor_y<7)
	{
		g_gx_magicsushi_context.last_key = 2;
	}
    }
// End--modified by kecx for fixing bug 12531 on 20090820    
}

/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_key_4_release
* DESCRIPTION
*	key 2 handling
* PARAMETERS
*	void 
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
void mmi_gx_magicsushi_key_4_release(void)
{
// Beginning--modified by kecx for fixing bug 12531 on 20090820
    if(g_gx_magicsushi_context.animation_mode != 6)
    {
	if(g_gx_magicsushi_context.cursor_x>0)
	{
		g_gx_magicsushi_context.cursor_x--;
		g_gx_magicsushi_context.last_key = 4;
	}
    }
    else
    {
        if(g_gx_magicsushi_context.cursor_x>0)
	{
		g_gx_magicsushi_context.last_key = 4;
	}
    }
// End--modified by kecx for fixing bug 12531 on 20090820
}

/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_key_6_release
* DESCRIPTION
*	key 2 handling
* PARAMETERS
*	void 
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
void mmi_gx_magicsushi_key_6_release(void)
{
// Beginning--modified by kecx for fixing bug 12531 on 20090820
    if(g_gx_magicsushi_context.animation_mode != 6)
    {
	if(g_gx_magicsushi_context.cursor_x<7)
	{
		g_gx_magicsushi_context.cursor_x++;	
		g_gx_magicsushi_context.last_key = 6;
	}
    }
    else
    {
	if(g_gx_magicsushi_context.cursor_x<7)
	{
		g_gx_magicsushi_context.last_key = 6;
	}
    }
// End--modified by kecx for fixing bug 12531 on 20090820
}

/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_key_8_release
* DESCRIPTION
*	key 2 handling
* PARAMETERS
*	void 
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
void mmi_gx_magicsushi_key_8_release(void)
{
// Beginning--modified by kecx for fixing bug 12531 on 20090820
    if(g_gx_magicsushi_context.animation_mode != 6)
    {
	if(g_gx_magicsushi_context.cursor_y>0)
	{
		g_gx_magicsushi_context.cursor_y--;
		g_gx_magicsushi_context.last_key = 8;
	}
    }
    else
    {
	if(g_gx_magicsushi_context.cursor_y>0)
	{
		g_gx_magicsushi_context.last_key = 8;
	}
    }
// End--modified by kecx for fixing bug 12531 on 20090820
}

/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_key_5_release
* DESCRIPTION
*	key 2 handling
* PARAMETERS
*	void 
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
void mmi_gx_magicsushi_key_5_release(void)
{
	S32 coord_x=0,coord_y=0;

	if(!g_gx_magicsushi_context.moving)
	{
			coord_x = g_gx_magicsushi_context.cursor_x;
			coord_y=g_gx_magicsushi_context.cursor_y;
			if (g_gx_magicsushi_context.click_mode==0) 
			{
        #ifdef __MMI_GAME_MULTICHANNEL_SOUND__
					GFX_STOP_SOUND_EFFECTS_MIDI(g_gx_magicsushi_context.move_midi);
					GFX_STOP_SOUND_EFFECTS_MIDI(g_gx_magicsushi_context.select_midi);

					GFX_PLAY_SOUND_EFFECTS_MIDI(MAGICSUSHISelect, MAGICSUSHISELECT, &(g_gx_magicsushi_context.select_midi),DEVICE_AUDIO_PLAY_ONCE);
				#else
					GFX_PLAY_AUDIO_MIDI(MAGICSUSHISelect, MAGICSUSHISELECT, DEVICE_AUDIO_PLAY_ONCE);	
				#endif

				g_gx_magicsushi_context.old_x = coord_x;
				g_gx_magicsushi_context.old_y = coord_y;
				g_gx_magicsushi_context.click_mode = 1;
				
				switch ( g_gx_magicsushi_context.map[coord_x][coord_y] )
				{
					case 8: 
					g_gx_magicsushi_context.map[coord_x][coord_y] = rand()%g_gx_magicsushi_context.type_num;
					g_gx_magicsushi_context.animation_mode = 2;
					g_gx_magicsushi_context.animation_step = MMI_GX_MAGICSUSHI_ANIMATION_STEP;
					g_gx_magicsushi_context.moving = 1;
					g_gx_magicsushi_context.special = 0;
					break;
					case 9:
					g_gx_magicsushi_context.moving = 1;
					g_gx_magicsushi_context.animation_mode = 3;
					g_gx_magicsushi_context.animation_step = MMI_GX_MAGICSUSHI_ANIMATION_STEP;
					g_gx_magicsushi_context.special = 0;
					break;
					case 10:
					g_gx_magicsushi_context.moving = 1;
					g_gx_magicsushi_context.animation_mode = 4;
					g_gx_magicsushi_context.animation_step = MMI_GX_MAGICSUSHI_ANIMATION_STEP;
					g_gx_magicsushi_context.special = 0;
					break;
					default:
					g_gx_magicsushi_context.selected = 1;
					break;
				}
				
			}
			else if(g_gx_magicsushi_context.click_mode==1)
			{
				g_gx_magicsushi_context.click_mode=1;
				if (ABS(g_gx_magicsushi_context.old_x-coord_x)+ABS(g_gx_magicsushi_context.old_y-coord_y)== 1)
				{

					g_gx_magicsushi_context.moving = 1;
					g_gx_magicsushi_context.check = 1;
					g_gx_magicsushi_context.animation_step = MMI_GX_MAGICSUSHI_ANIMATION_STEP;
					g_gx_magicsushi_context.selected = 0;

					/* up-down switch */
					if (ABS(g_gx_magicsushi_context.old_x-coord_x)==0) 
					{
						g_gx_magicsushi_context.animation_mode = 5;
						if(g_gx_magicsushi_context.old_y > coord_y)
						{
							g_gx_magicsushi_context.special_y = coord_y;
							g_gx_magicsushi_context.special_x = coord_x;
						}
						else
						{
							g_gx_magicsushi_context.special_x = g_gx_magicsushi_context.old_x;
							g_gx_magicsushi_context.special_y = g_gx_magicsushi_context.old_y;
							g_gx_magicsushi_context.old_x= coord_x;
							g_gx_magicsushi_context.old_y= coord_y;
						}
					}
					/* left-right switch */
					else  
					{
						g_gx_magicsushi_context.animation_mode = 7;
						if(g_gx_magicsushi_context.old_x > coord_x)
						{
							g_gx_magicsushi_context.special_y = coord_y;
							g_gx_magicsushi_context.special_x = coord_x;
						}
						else
						{
							g_gx_magicsushi_context.special_x = g_gx_magicsushi_context.old_x;
							g_gx_magicsushi_context.special_y = g_gx_magicsushi_context.old_y;
							g_gx_magicsushi_context.old_x= coord_x;
							g_gx_magicsushi_context.old_y= coord_y;
						}
					}
				}
				else if (ABS(g_gx_magicsushi_context.old_x-coord_x)+ABS(g_gx_magicsushi_context.old_y-coord_y)== 0)
				{
						g_gx_magicsushi_context.selected = 2;
						g_gx_magicsushi_context.click_mode= 0;
						g_gx_magicsushi_context.moving = 0;
				}
			}	
	}
}

/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_show_magicsushi
* DESCRIPTION
*	Draw the magicsushi
* PARAMETERS
*	void 
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
void mmi_gx_magicsushi_show_magicsushi()
{
	int i,j,x,y;
	for (i=0;i<8;i++)
	{
		y = (7 - i) * MMI_GX_MAGICSUSHI_SIZE+ MMI_GX_MAGICSUSHI_TOP_WIDTH;
		for(j=0;j<8;j++)
		{
			if(i == g_gx_magicsushi_context.i_map[j][i] )
			{
				x = j * MMI_GX_MAGICSUSHI_SIZE + MMI_GX_MAGICSUSHI_LEFT_WIDTH;
				gdi_image_draw_id(x,y,(U16)(IMG_ID_GX_MAGICSUSHI_TYPE_0 + g_gx_magicsushi_context.map[j][i]));
			}
		}
	}	
}
/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_exit_game
* DESCRIPTION
*	Exit Game [Callback required by GFX]
* PARAMETERS
*	void 
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
void mmi_gx_magicsushi_exit_game(void)
{
	UI_cancel_timer(mmi_gx_magicsushi_cyclic_timer);	/* cancle the looping timer */
#ifdef __MMI_GAME_MULTICHANNEL_SOUND__
		GFX_STOP_SOUND_EFFECTS_MIDI(g_gx_magicsushi_context.move_midi);
		GFX_STOP_SOUND_EFFECTS_MIDI(g_gx_magicsushi_context.select_midi);

		GFX_CLOSE_SOUND_EFFECTS_MIDI(g_gx_magicsushi_context.move_midi);
		GFX_CLOSE_SOUND_EFFECTS_MIDI(g_gx_magicsushi_context.select_midi);

		GFX_STOP_DUMMY_BACKGROUND_SOUND();
		GFX_CLOSE_DUMMY_BACKGROUND_SOUND();

		GFX_STOP_BACKGROUND_SOUND(g_gx_magicsushi_context.background_midi);
	  	GFX_CLOSE_BACKGROUND_SOUND(g_gx_magicsushi_context.background_midi);	  
	#endif

	gdi_layer_multi_layer_disable();
	gdi_layer_free(g_gx_magicsushi_context.foreground);

}

/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_init_game
* DESCRIPTION
*	Game initilization
* PARAMETERS
*	void 
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
void mmi_gx_magicsushi_init_game(void)
{
	/* init game here */
	U8 i;
	g_gx_magicsushi_context.click_mode = 0;
	g_gx_magicsushi_context.level = 1;
	g_gx_magicsushi_context.bonus = 1;
	g_gx_magicsushi_context.remainder = 50;
	g_gx_magicsushi_context.total_time = 50;
	g_gx_magicsushi_context.remain_time = 25;
	g_gx_magicsushi_context.tick = 0;
	g_gx_magicsushi_context.type_num = 7;
	g_gx_magicsushi_context.animation_step = 0;
	g_gx_magicsushi_context.selected = 0;
	g_gx_magicsushi_context.moving = 1;	
	g_gx_magicsushi_context.special = 0;
	g_gx_magicsushi_context.game_grade= 0;
	g_gx_magicsushi_context.check = 0;           
	g_gx_magicsushi_context.cursor_x = 0;           
	g_gx_magicsushi_context.cursor_y = 0;           
	g_gx_magicsushi_context.last_key = 0;
	g_gx_magicsushi_context.special_x =0;
	g_gx_magicsushi_context.special_y =0;
	for(i=0;i<8;i++)
	{
		g_gx_magicsushi_context.solution_start_horizontal[i][0]=-1;
		g_gx_magicsushi_context.solution_start_vertical[i][0]=-1;
	}
	mmi_gx_magicsushi_gen_map();
}

/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_framemove
* DESCRIPTION
*	Framemove - process the game logic	
* PARAMETERS
*	void 
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
void mmi_gx_magicsushi_framemove(void)
{
	/* add logic code here */
}

/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_gameover
* DESCRIPTION
*	Gameover function			
* PARAMETERS
*	void 
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
void mmi_gx_magicsushi_gameover(void)
{
	g_gx_magicsushi_context.is_gameover = TRUE;
	g_gx_magicsushi_context.is_new_game = TRUE;
	
	/* call this function to draw gameover screen */		
	mmi_gfx_entry_gameover_screen();
}

/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_cyclic_timer
* DESCRIPTION
*	Timer trigger function - looping		
* PARAMETERS
*	void 
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
void mmi_gx_magicsushi_cyclic_timer(void) /* done */
{
    TRACE_EF(g_sw_GAME, "MMI_GAME: mmi_gx_magicsushi_cyclic_timer");
	g_gx_magicsushi_context.tick++;
	if (g_gx_magicsushi_context.tick > 65520)
		g_gx_magicsushi_context.tick = 0;
	UI_start_timer(g_gx_magicsushi_context.timer_elapse, mmi_gx_magicsushi_cyclic_timer);

	/* update TIME every 1 second */
/*UNI@lizongzheng_20070921 17:49:20 ����ħ����˾����Ϸ�ٶ�   */
	#if (defined(MT6225))
	if (g_gx_magicsushi_context.tick % 20 == 0)
	#else
	if (g_gx_magicsushi_context.tick % 10 == 0)
	#endif
	{
		--g_gx_magicsushi_context.remain_time;		

		if(g_gx_magicsushi_context.remain_time == 0)
			mmi_gx_magicsushi_gameover();
	}
	if(g_gx_magicsushi_context.is_gameover == FALSE)
	{
		mmi_gx_magicsushi_framemove();
mmi_gx_magicsushi_render();
	}
}

/*****************************************************************************
* FUNCTION
*	mmi_gx_mgacisushi_check_pair
* DESCRIPTION
*	check if any match around the pair (x1,y1) (x2,y2)
* PARAMETERS
*	x1 : x_cord array index of point1
*	y1 : y_cord array index of point1
*	x2 : x_cord array index of point2
*	y2 : y_cord array index of point2
*	horizontal : horizontal or not
* RETURNS
*	S16
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
U8	mmi_gx_magicsushi_check_pair(U8 x1,U8 y1,U8 x2,U8 y2,U8 horizontal) /* done */
{

	U8 flag=0, length=1, iNum=0;
	S16 dx1,dx2,dy1,dy2;
	if (horizontal)
	{
		dy1 = y1-2;
		dy2 = y1+2;
		dx1 = x2-2;
		dx2 = x1+2;
		if(dy1<0)
			dy1 = 0;
		if(dy2 >7)
			dy2 = 7;
		if(dx1<0)
			dx1 = 0;
		if(dx2>7)
			dx2 = 7;
		CHECK_HORIZONTAL(y1,dx1,dx2,length,iNum,flag);
		
		iNum=0;
		length = 1;
		CHECK_VERTICAL(x1,dy1,dy2,length,iNum,flag);
		
		dy1 = y1-2;
		if(dy1<0)
			dy1 = 0;
		iNum=0;
		length = 1;
		CHECK_VERTICAL(x2,dy1,dy2,length,iNum,flag);
		
	}
	else
	{
		dy1 = y2-2;
		dy2 = y1+2;
		dx1 = x1-2;
		dx2 = x1+2;
		if(dy1<0)
			dy1 = 0;
		if(dy2 >7)
			dy2 = 7;
		if(dx1<0)
			dx1 = 0;
		if(dx2>7)
			dx2 = 7;
		CHECK_VERTICAL(x1,dy1,dy2,length,iNum,flag);
		iNum=0;
		length = 1;
		CHECK_HORIZONTAL(y1,dx1,dx2,length,iNum,flag);
		
		dx1 = x1-2;
		if(dx1<0)
			dx1 = 0;
		iNum=0;
		length = 1;
		CHECK_HORIZONTAL(y2,dx1,dx2,length,iNum,flag);
	}
	return flag;	
}


/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_check_all
* DESCRIPTION
*	check if any match on the map
* PARAMETERS
*	void
* RETURNS
*	void
* GLOBALS AFFECTED
*	NIL
*****************************************************************************/
U8 mmi_gx_magicsushi_check_all() /* done */
{
	S16 i;
	U8 flag=0, length, iNum;
	U8 x,y;
	x = 0;
	y = 7;
	for(i=0;i<8;i++)
	{
		iNum=0;
		length = 1;
		x=0;
		CHECK_VERTICAL(i,x,y,length,iNum,flag);

		iNum=0;
		length = 1;
		x=0;
		CHECK_HORIZONTAL(i,x,y,length,iNum,flag);
	}
	return flag;
}

/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_gen_map
* DESCRIPTION
*	Generate the whole map
* PARAMETERS
*	void 
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
void	mmi_gx_magicsushi_gen_map() /* done */
{
	U16 i,j,item;	
	for (j=0;j<8;j++)
		for (i=0;i<8;i++)
		{
			RAND_ITEM(i,j,item);
		}

	g_gx_magicsushi_context.animation_step = 8 * MMI_GX_MAGICSUSHI_ANIMATION_STEP;
	g_gx_magicsushi_context.animation_mode = 6;
	
}

/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_update_game_data
* DESCRIPTION
*	Update game data (grade, map)
* PARAMETERS
*	void 
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
void	mmi_gx_magicsushi_update_game_data() /* doen */
{
	U8 iNum=0;
	U8 count,i;
	U8 time = g_gx_magicsushi_context.total_time/10;
	S16 j;

	for(i=0;i<8;i++)
	{
		while(g_gx_magicsushi_context.solution_start_vertical[i][iNum]!=-1)
		{
			count = g_gx_magicsushi_context.solution_end_vertical[i][iNum]-g_gx_magicsushi_context.solution_start_vertical[i][iNum]+1;
			g_gx_magicsushi_context.game_grade += g_gx_magicsushi_context.level*g_gx_magicsushi_context.bonus;
			g_gx_magicsushi_context.remainder -= 1;
			g_gx_magicsushi_context.remain_time += time;

			j = g_gx_magicsushi_context.solution_start_vertical[i][iNum];
			while(j <= g_gx_magicsushi_context.solution_end_vertical[i][iNum])
			{
				g_gx_magicsushi_context.map[i][j]=255;
				j++;
			}
			iNum++;		
		}

		g_gx_magicsushi_context.solution_start_vertical[i][0]=-1;
	
		iNum = 0;
		while(g_gx_magicsushi_context.solution_start_horizontal[i][iNum]!=-1)
		{
			count = g_gx_magicsushi_context.solution_end_horizontal[i][iNum]-g_gx_magicsushi_context.solution_start_horizontal[i][iNum]+1;
			g_gx_magicsushi_context.game_grade += g_gx_magicsushi_context.level*g_gx_magicsushi_context.bonus;
			g_gx_magicsushi_context.remainder -= 1;
			g_gx_magicsushi_context.remain_time += time;

			j = g_gx_magicsushi_context.solution_start_horizontal[i][iNum];
			while(j <= g_gx_magicsushi_context.solution_end_horizontal[i][iNum])
			{
				g_gx_magicsushi_context.map[j][i]=255;
				j++;
			}
			iNum++;		
		}
		iNum = 0;
		g_gx_magicsushi_context.solution_start_horizontal[i][0]=-1;
	}
	g_gx_magicsushi_context.animation_step = MMI_GX_MAGICSUSHI_ANIMATION_STEP;
	g_gx_magicsushi_context.animation_mode = 8;
	g_gx_magicsushi_context.bonus++;
}

/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_update_map
* DESCRIPTION
*	Update map
* PARAMETERS
*	void 
* RETURNS
*	void
* GLOBALS AFFECTED
*	
*****************************************************************************/
void	mmi_gx_magicsushi_update_map()  /* done */
{
	U8 item; /* for RAND_ITEM() */
	U8 i,j,movesteps,max_move=0;
	for (i=0;i<8;i++)
	{
		movesteps = 0;
		for (j=0;j<8;j++)
		{
			if(g_gx_magicsushi_context.map[i][j]==255)
				movesteps++;
			else if(movesteps>0)
			{
				g_gx_magicsushi_context.map[i][j-movesteps]=g_gx_magicsushi_context.map[i][j];
				g_gx_magicsushi_context.i_map[i][j-movesteps]=g_gx_magicsushi_context.i_map[i][j];
			}
		}

		for (j = 8-movesteps;j<8;j++)
		{
			RAND_ITEM(i,j,item);
		}	
		if(movesteps > max_move)
			max_move = movesteps;
	}
	g_gx_magicsushi_context.animation_step = max_move * MMI_GX_MAGICSUSHI_ANIMATION_STEP;
	g_gx_magicsushi_context.animation_mode = 6;
}

/*****************************************************************************
* FUNCTION
*	mmi_gx_magicsushi_is_moremove
* DESCRIPTION
*	check if the map has solution
* PARAMETERS
*	void 
* RETURNS
*	S16 Flag
* GLOBALS AFFECTED
*	
*****************************************************************************/
U8	mmi_gx_magicsushi_is_moremove() /* done */
{
	U8 item,i,j,scase,index;

	if(g_gx_magicsushi_context.special == 1)
	return 1;
	
	for(i=1;i<7;i++)
	{
		for(j=1;j<7;j++)
		{
			scase = 0;
			index = 1;
			item = g_gx_magicsushi_context.map[i][j];
	
			if(g_gx_magicsushi_context.map[i-1][j+1]==item)
				scase += index;
	
			index = index<<1;
			
			if(g_gx_magicsushi_context.map[i+1][j+1]==item)
				scase += index;

			index = index<<1;

			if(g_gx_magicsushi_context.map[i-1][j-1]==item)
				scase += index;
				
			index = index<<1;
			
			if(g_gx_magicsushi_context.map[i+1][j-1]==item)
				scase += index;

			switch(scase)
			{
				case 0:
				break;
				case 1:
				if (g_gx_magicsushi_context.map[i+1][j]==item||g_gx_magicsushi_context.map[i][j-1]==item)
					return 1;
				break;
				case 2:
				if (g_gx_magicsushi_context.map[i-1][j]==item||g_gx_magicsushi_context.map[i][j-1]==item)
					return 1;
				break;
				case 4:
				if (g_gx_magicsushi_context.map[i][j+1]==item||g_gx_magicsushi_context.map[i+1][j]==item)
					return 1;
				break;
				case 8:
				if (g_gx_magicsushi_context.map[i][j+1]==item||g_gx_magicsushi_context.map[i-1][j]==item)
					return 1;
				break;
				case 6:	
				case 9:
				if (g_gx_magicsushi_context.map[i+1][j]==item||g_gx_magicsushi_context.map[i][j-1]==item||g_gx_magicsushi_context.map[i-1][j]==item||g_gx_magicsushi_context.map[i][j+1]==item)
					return 1;
				break;
				default:
				return 1;
				break;
			}

/*
			if(i>1 && j>1 && i <6 && j<6)
			{
				if(g_gx_magicsushi_context.map[i][j+1]==item)
					if(g_gx_magicsushi_context.map[i][j-2]==item)
						return 1;
			
				if(g_gx_magicsushi_context.map[i+1][j]==item)
					if(g_gx_magicsushi_context.map[i-2][j]==item)
						return 1;

				if(g_gx_magicsushi_context.map[i][j-1]==item)
					if(g_gx_magicsushi_context.map[i][j+2]==item)
						return 1;
				if(g_gx_magicsushi_context.map[i-1][j]==item)
					if(g_gx_magicsushi_context.map[i+2][j]==item)
						return 1;
			}
*/
		}
	}

	for (i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			/* horizontal check xX x| x Xx */
			if(j>1)
				if(g_gx_magicsushi_context.map[i][j+1]==item)
					if(g_gx_magicsushi_context.map[i][j-2]==item)
						return 1;
			if(j<6)
				if(g_gx_magicsushi_context.map[i][j-1]==item)
					if(g_gx_magicsushi_context.map[i][j+2]==item)
						return 1;
			/* vertical check */
			if(i>1)
				if(g_gx_magicsushi_context.map[i+1][j]==item)
					if(g_gx_magicsushi_context.map[i-2][j]==item)
						return 1;
			if(i<6)
				if(g_gx_magicsushi_context.map[i-1][j]==item)
					if(g_gx_magicsushi_context.map[i+2][j]==item)
						return 1;
		}
	}	

	return 0;
}

#endif /* __MMI_GAME_MAGICSUSHI__ */
#endif /* _MMI_GX_MAGICSUSHI_C */ 

