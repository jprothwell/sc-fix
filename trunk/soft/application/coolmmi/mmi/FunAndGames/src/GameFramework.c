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
 *	 GameFramework.c
 *
 * Project:
 * --------
 *
 * Description:
 * ------------
 *  Game Framework
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
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 *============================================================================
 ****************************************************************************/
#include "mmi_features.h"

#ifdef __MMI_GAME__

#include "globalconstants.h"
#include "globaldefs.h"
#include "gui.h"
#include "historygprot.h"
#include "unicodexdcl.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "nvramenum.h"
#include "miscfunctions.h"
#include "audioinc.h"
#include "settingprofile.h"					/* game sound level define */
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "mmi_trace.h"

#include "gameframework.h"
#include "gamedefs.h"							/* game id's and effect flag */
#include "profilegprots.h"						/* set audio volumn fucntion */
#include "gpioinc.h"								/* set audio volumn fucntion */
#include "gdi_include.h"
#include "wgui_categories_multimedia.h"	/* for gameover screen's empty category */
#include "commonscreens.h"						/* popup */
#include "keybrd.h"								/* for keypad tone related interface */

#include "mainmenudef.h"

#ifdef __MMI_TOUCH_SCREEN__
#include "wgui_touch_screen.h"
#endif

#ifdef __MMI_SUBLCD__
#include "sublcdhistorygprot.h"
#endif

#ifdef WIN32 
#include "sublcdhistorygprot.h"
#endif

/****************************************************************************
* Local Function                                                      
*****************************************************************************/
void mmi_gfx_volumn_up(void);
void mmi_gfx_volumn_down(void);
void mmi_gfx_exit_game(void);
void mmi_gfx_enter_game(void);
void mmi_gfx_new_game(void);
void mmi_gfx_resume_game(void);
void mmi_gfx_store_grade(void);
void mmi_gfx_exit_gameover_screen(void);
void mmi_gfx_entry_gameover_screen(void);
void mmi_gfx_set_game_level(void);
void mmi_gfx_set_level_highlight_hdlr(S32 index);
void mmi_gfx_entry_level_screen(void);
void mmi_gfx_entry_help_screen(void);
void mmi_gfx_reset_grade(void);
void mmi_gfx_entry_grade_screen(void);
void mmi_gfx_menu_hilight_hdlr(S32 index);
void mmi_gfx_entry_menu_screen(void);
#ifdef __MMI_SUBLCD__
void mmi_gfx_entry_sublcd_screen(void);
#endif
extern BOOL HandleLowBatteryProcess(void);
/****************************************************************************
* Local Variable                                                      
*****************************************************************************/
mmi_gfx_cntx_struct	g_gfx_cntx;

__align(2) const U8 g_gfx_audio_gameover[862] = {
 0x4D, 0x54, 0x68, 0x64, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x03, 0xC0, 0x4D, 0x54, 0x72, 
 0x6B, 0x00, 0x00, 0x03, 0x48, 0x00, 0xFF, 0x02, 0x16, 0x43, 0x6F, 0x70, 0x79, 0x72, 0x69, 0x67, 
 0x68, 0x74, 0x20, 0x63, 0x20, 0x32, 0x30, 0x30, 0x33, 0x20, 0x62, 0x79, 0x20, 0x43, 0x43, 0x00, 
 0xFF, 0x01, 0x02, 0x43, 0x43, 0x00, 0xFF, 0x58, 0x04, 0x04, 0x02, 0x18, 0x08, 0x00, 0xFF, 0x59, 
 0x02, 0x00, 0x00, 0x00, 0xFF, 0x51, 0x03, 0x06, 0xA0, 0x67, 0x00, 0xC0, 0x44, 0x00, 0xB0, 0x07, 
 0x7C, 0x00, 0x90, 0x40, 0x67, 0x00, 0xC1, 0x3D, 0x00, 0xB1, 0x07, 0x58, 0x00, 0x91, 0x28, 0x7C, 
 0x00, 0x34, 0x7F, 0x00, 0xC2, 0x64, 0x00, 0xB2, 0x07, 0x3C, 0x00, 0xC3, 0x13, 0x00, 0xB3, 0x07, 
 0x68, 0x00, 0x93, 0x32, 0x7A, 0x81, 0x70, 0x32, 0x00, 0x00, 0x31, 0x6F, 0x81, 0x70, 0x31, 0x00, 
 0x00, 0x90, 0x40, 0x00, 0x00, 0x3F, 0x5A, 0x00, 0x92, 0x43, 0x7D, 0x00, 0x40, 0x7D, 0x00, 0x3B, 
 0x7C, 0x00, 0x93, 0x30, 0x6E, 0x81, 0x70, 0x30, 0x00, 0x00, 0x2F, 0x6C, 0x81, 0x70, 0x2F, 0x00, 
 0x00, 0x90, 0x3F, 0x00, 0x00, 0x40, 0x50, 0x00, 0x93, 0x32, 0x7A, 0x81, 0x70, 0x32, 0x00, 0x00, 
 0x31, 0x74, 0x81, 0x70, 0x31, 0x00, 0x00, 0x90, 0x40, 0x00, 0x00, 0x92, 0x3B, 0x00, 0x00, 0x43, 
 0x00, 0x00, 0x40, 0x00, 0x00, 0x90, 0x42, 0x7C, 0x00, 0x92, 0x43, 0x7C, 0x00, 0x40, 0x7C, 0x00, 
 0x3B, 0x6F, 0x00, 0x93, 0x30, 0x74, 0x81, 0x70, 0x30, 0x00, 0x00, 0x2F, 0x72, 0x81, 0x70, 0x2F, 
 0x00, 0x00, 0x92, 0x3B, 0x00, 0x00, 0x91, 0x28, 0x00, 0x00, 0x90, 0x42, 0x00, 0x00, 0x91, 0x34, 
 0x00, 0x00, 0x92, 0x40, 0x00, 0x00, 0x43, 0x00, 0x00, 0x90, 0x43, 0x53, 0x00, 0x91, 0x37, 0x7C, 
 0x00, 0x2B, 0x7C, 0x00, 0x93, 0x32, 0x7C, 0x81, 0x70, 0x32, 0x00, 0x00, 0x31, 0x72, 0x81, 0x70, 
 0x31, 0x00, 0x00, 0x90, 0x43, 0x00, 0x00, 0x42, 0x6F, 0x00, 0x92, 0x3F, 0x7F, 0x00, 0x42, 0x7F, 
 0x00, 0x3B, 0x7C, 0x00, 0x93, 0x30, 0x72, 0x81, 0x70, 0x30, 0x00, 0x00, 0x2F, 0x73, 0x81, 0x70, 
 0x2F, 0x00, 0x00, 0x90, 0x42, 0x00, 0x00, 0x43, 0x53, 0x00, 0x93, 0x32, 0x7C, 0x28, 0xFF, 0x51, 
 0x03, 0x06, 0xA0, 0x67, 0x28, 0xFF, 0x51, 0x03, 0x06, 0xA8, 0x4C, 0x28, 0xFF, 0x51, 0x03, 0x06, 
 0xC0, 0x6E, 0x50, 0xFF, 0x51, 0x03, 0x06, 0xD0, 0xE6, 0x28, 0xFF, 0x51, 0x03, 0x06, 0xEA, 0x34, 
 0x00, 0x93, 0x32, 0x00, 0x00, 0x31, 0x73, 0x28, 0xFF, 0x51, 0x03, 0x07, 0x04, 0x41, 0x28, 0xFF, 
 0x51, 0x03, 0x07, 0x0D, 0x1B, 0x28, 0xFF, 0x51, 0x03, 0x07, 0x16, 0x0C, 0x28, 0xFF, 0x51, 0x03, 
 0x07, 0x28, 0x33, 0x28, 0xFF, 0x51, 0x03, 0x07, 0x3A, 0xB8, 0x28, 0xFF, 0x51, 0x03, 0x07, 0x4D, 
 0x9E, 0x00, 0x93, 0x31, 0x00, 0x00, 0x90, 0x43, 0x00, 0x00, 0x92, 0x3B, 0x00, 0x00, 0x3F, 0x00, 
 0x00, 0x42, 0x00, 0x00, 0x90, 0x45, 0x53, 0x00, 0x92, 0x42, 0x7F, 0x00, 0x3F, 0x7F, 0x00, 0x3B, 
 0x6F, 0x00, 0x93, 0x30, 0x71, 0x28, 0xFF, 0x51, 0x03, 0x07, 0x60, 0xE7, 0x28, 0xFF, 0x51, 0x03, 
 0x07, 0x74, 0x97, 0x50, 0xFF, 0x51, 0x03, 0x07, 0x88, 0xB2, 0x50, 0xFF, 0x51, 0x03, 0x07, 0xA7, 
 0xA8, 0x00, 0x93, 0x30, 0x00, 0x00, 0x2F, 0x72, 0x28, 0xFF, 0x51, 0x03, 0x07, 0xB2, 0x33, 0x78, 
 0xFF, 0x51, 0x03, 0x07, 0xBC, 0xDB, 0x28, 0xFF, 0x51, 0x03, 0x07, 0xC7, 0xA1, 0x28, 0xFF, 0x51, 
 0x03, 0x07, 0xD2, 0x85, 0x00, 0x93, 0x2F, 0x00, 0x00, 0x92, 0x3B, 0x00, 0x00, 0x91, 0x37, 0x00, 
 0x00, 0x90, 0x45, 0x00, 0x00, 0x91, 0x2B, 0x00, 0x00, 0x92, 0x3F, 0x00, 0x00, 0x42, 0x00, 0x00, 
 0x90, 0x47, 0x67, 0x00, 0x91, 0x3B, 0x7D, 0x00, 0x2F, 0x7D, 0x00, 0x93, 0x32, 0x7C, 0x50, 0xFF, 
 0x51, 0x03, 0x07, 0xDD, 0x88, 0x28, 0xFF, 0x51, 0x03, 0x07, 0xE8, 0xAA, 0x78, 0x93, 0x32, 0x00, 
 0x00, 0x31, 0x73, 0x28, 0xFF, 0x51, 0x03, 0x07, 0xF3, 0xEB, 0x28, 0xFF, 0x51, 0x03, 0x07, 0xFF, 
 0x4D, 0x78, 0xFF, 0x51, 0x03, 0x08, 0x0A, 0xD0, 0x28, 0x93, 0x31, 0x00, 0x00, 0x90, 0x47, 0x00, 
 0x00, 0x45, 0x7C, 0x00, 0x92, 0x42, 0x7F, 0x00, 0x3E, 0x7D, 0x00, 0x3C, 0x7D, 0x00, 0x93, 0x30, 
 0x73, 0x28, 0xFF, 0x51, 0x03, 0x08, 0x16, 0x73, 0x78, 0xFF, 0x51, 0x03, 0x08, 0x22, 0x39, 0x50, 
 0xFF, 0x51, 0x03, 0x08, 0x2E, 0x21, 0x00, 0x93, 0x30, 0x00, 0x00, 0x2F, 0x73, 0x50, 0xFF, 0x51, 
 0x03, 0x08, 0x3A, 0x2C, 0x78, 0xFF, 0x51, 0x03, 0x08, 0x46, 0x5B, 0x28, 0xFF, 0x51, 0x03, 0x08, 
 0x52, 0xAF, 0x00, 0x93, 0x2F, 0x00, 0x00, 0x90, 0x45, 0x00, 0x00, 0x43, 0x52, 0x00, 0x93, 0x32, 
 0x7C, 0x78, 0xFF, 0x51, 0x03, 0x08, 0x5F, 0x27, 0x78, 0x93, 0x32, 0x00, 0x00, 0x31, 0x6F, 0x28, 
 0xFF, 0x51, 0x03, 0x08, 0x6B, 0xC4, 0x81, 0x48, 0x93, 0x31, 0x00, 0x00, 0x90, 0x43, 0x00, 0x00, 
 0x92, 0x3C, 0x00, 0x00, 0x42, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x90, 0x42, 0x67, 0x00, 0x92, 0x42, 
 0x7F, 0x00, 0x3E, 0x7D, 0x00, 0x3C, 0x7D, 0x00, 0x93, 0x30, 0x72, 0x50, 0xFF, 0x51, 0x03, 0x08, 
 0x78, 0x88, 0x78, 0xFF, 0x51, 0x03, 0x08, 0x85, 0x73, 0x28, 0x93, 0x30, 0x00, 0x00, 0x2F, 0x72, 
 0x81, 0x20, 0xFF, 0x51, 0x03, 0x08, 0x9F, 0xC1, 0x50, 0x93, 0x2F, 0x00, 0x00, 0x92, 0x3C, 0x00, 
 0x00, 0x91, 0x3B, 0x00, 0x00, 0x90, 0x42, 0x00, 0x00, 0x91, 0x2F, 0x00, 0x00, 0x92, 0x3E, 0x00, 
 0x00, 0x42, 0x00, 0x00, 0x90, 0x40, 0x5A, 0x00, 0x91, 0x2B, 0x7C, 0x00, 0x37, 0x7F, 0x00, 0x93, 
 0x34, 0x7C, 0x00, 0x2F, 0x7C, 0x28, 0xFF, 0x51, 0x03, 0x08, 0xAD, 0x25, 0x82, 0x40, 0xFF, 0x51, 
 0x03, 0x08, 0xBA, 0xB2, 0x82, 0x40, 0xFF, 0x51, 0x03, 0x08, 0xC8, 0x6B, 0x82, 0x18, 0x93, 0x2F, 
 0x00, 0x00, 0x91, 0x37, 0x00, 0x00, 0x93, 0x34, 0x00, 0x00, 0x90, 0x40, 0x00, 0x10, 0x91, 0x2B, 
 0x00, 0x87, 0x30, 0xFF, 0x51, 0x03, 0x06, 0x63, 0xC8, 0x00, 0xFF, 0x2F, 0x00
 };

 __align(2) const U8 g_gfx_audio_complete[310] = {
 0x4D, 0x54, 0x68, 0x64, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x03, 0xC0, 0x4D, 0x54, 0x72, 
 0x6B, 0x00, 0x00, 0x01, 0x20, 0x00, 0xFF, 0x02, 0x16, 0x43, 0x6F, 0x70, 0x79, 0x72, 0x69, 0x67, 
 0x68, 0x74, 0x20, 0x63, 0x20, 0x32, 0x30, 0x30, 0x33, 0x20, 0x62, 0x79, 0x20, 0x43, 0x43, 0x00, 
 0xFF, 0x01, 0x02, 0x43, 0x43, 0x00, 0xFF, 0x58, 0x04, 0x04, 0x02, 0x18, 0x08, 0x00, 0xFF, 0x59, 
 0x02, 0x00, 0x00, 0x00, 0xFF, 0x51, 0x03, 0x08, 0xE3, 0x7C, 0x00, 0xC0, 0x0A, 0x00, 0xB0, 0x07, 
 0x74, 0x00, 0x90, 0x4F, 0x52, 0x00, 0xC1, 0x72, 0x00, 0xB1, 0x07, 0x6C, 0x00, 0x91, 0x30, 0x7C, 
 0x81, 0x70, 0x90, 0x4F, 0x00, 0x00, 0x4D, 0x5A, 0x81, 0x70, 0x4D, 0x00, 0x00, 0x91, 0x30, 0x00, 
 0x00, 0x90, 0x4C, 0x5A, 0x00, 0x91, 0x37, 0x6F, 0x81, 0x70, 0x90, 0x4C, 0x00, 0x00, 0x4F, 0x6F, 
 0x81, 0x70, 0x4F, 0x00, 0x00, 0x91, 0x37, 0x00, 0x00, 0x90, 0x54, 0x7C, 0x00, 0x91, 0x3C, 0x6F, 
 0x83, 0x60, 0x3C, 0x00, 0x00, 0x90, 0x54, 0x00, 0x00, 0x4F, 0x7C, 0x00, 0x91, 0x3E, 0x6F, 0x83, 
 0x60, 0x3E, 0x00, 0x00, 0x90, 0x4F, 0x00, 0x00, 0x4F, 0x67, 0x00, 0x91, 0x40, 0x6F, 0x81, 0x70, 
 0x90, 0x4F, 0x00, 0x00, 0x4D, 0x67, 0x81, 0x70, 0x4D, 0x00, 0x00, 0x91, 0x40, 0x00, 0x00, 0x90, 
 0x4C, 0x5A, 0x00, 0x91, 0x3E, 0x67, 0x81, 0x70, 0x90, 0x4C, 0x00, 0x00, 0x4F, 0x52, 0x00, 0x4D, 
 0x50, 0x81, 0x70, 0x4D, 0x00, 0x00, 0x4F, 0x00, 0x00, 0x91, 0x3E, 0x00, 0x00, 0x90, 0x54, 0x5A, 
 0x00, 0x91, 0x3C, 0x6F, 0x83, 0x60, 0x3C, 0x00, 0x00, 0x90, 0x54, 0x00, 0x00, 0x4F, 0x6F, 0x00, 
 0x91, 0x37, 0x6F, 0x83, 0x60, 0x37, 0x00, 0x00, 0x90, 0x4F, 0x00, 0x00, 0x54, 0x64, 0x00, 0x48, 
 0x64, 0x00, 0x4C, 0x64, 0x00, 0x4F, 0x64, 0x00, 0x91, 0x43, 0x64, 0x00, 0x40, 0x64, 0x00, 0x48, 
 0x64, 0x00, 0x3C, 0x64, 0x87, 0x40, 0x3C, 0x00, 0x00, 0x90, 0x54, 0x00, 0x00, 0x91, 0x43, 0x00, 
 0x00, 0x90, 0x48, 0x00, 0x00, 0x4C, 0x00, 0x00, 0x4F, 0x00, 0x00, 0x91, 0x40, 0x00, 0x00, 0x48, 
 0x00, 0x00, 0xFF, 0x2F, 0x00
 };

#ifdef __MMI_GAME_MULTICHANNEL_SOUND__

__align(2) const U8 g_gfx_Dummy_background_midi[94]={
   0x4D,0x54,0x68,0x64,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x01,0x03,0xC0,0x4D,0x54,
   0x72,0x6B,0x00,0x00,0x00,0x48,0x00,0xFF,0x02,0x16,0x43,0x6F,0x70,0x79,0x72,0x69,
   0x67,0x68,0x74,0x20,0x63,0x20,0x32,0x30,0x30,0x35,0x20,0x62,0x79,0x20,0x43,0x43,
   0x00,0xFF,0x01,0x02,0x43,0x43,0x00,0xFF,0x58,0x04,0x04,0x02,0x18,0x08,0x00,0xFF,
   0x59,0x02,0x00,0x00,0x00,0xFF,0x51,0x03,0x09,0x27,0xC0,0x00,0xC0,0x00,0x00,0xB0,
   0x07,0x00,0x00,0x90,0x32,0x01,0x9E,0x00,0x32,0x00,0x00,0xFF,0x2F,0x00
/*  0x4D,0x54,0x68,0x64,0x00,0x00,0x00,0x06,
    0x00,0x00,0x00,0x01,0x03,0xC0,0x4D,0x54,
    0x72,0x6B,0x00,0x00,0x01,0x13,0x00,0xFF,
    0x02,0x16,0x43,0x6F,0x70,0x79,0x72,0x69,
    0x67,0x68,0x74,0x20,0x63,0x20,0x32,0x30,
    0x30,0x33,0x20,0x62,0x79,0x20,0x43,0x43,
    0x00,0xFF,0x01,0x02,0x43,0x43,0x00,0xFF,
    0x58,0x04,0x04,0x02,0x18,0x08,0x00,0xFF,
    0x59,0x02,0x00,0x00,0x00,0xFF,0x51,0x03,
    0x09,0x27,0xC0,0x00,0xC0,0x47,0x00,0x90,
    0x4A,0x5A,0x00,0x45,0x6F,0x81,0x70,0x45,
    0x00,0x00,0x4A,0x00,0x00,0x4A,0x67,0x00,
    0x45,0x6F,0x81,0x70,0x45,0x00,0x00,0x4A,
    0x00,0x00,0x4A,0x53,0x00,0x45,0x67,0x81,
    0x70,0x45,0x00,0x00,0x4A,0x00,0x00,0x4A,
    0x67,0x00,0x45,0x7C,0x81,0x70,0x45,0x00,
    0x00,0x4A,0x00,0x00,0x48,0x5A,0x00,0x43,
    0x6F,0x81,0x70,0x43,0x00,0x00,0x48,0x00,
    0x81,0x70,0x48,0x67,0x00,0x43,0x7C,0x81,
    0x70,0x43,0x00,0x00,0x48,0x00,0x81,0x70,
    0x4A,0x6F,0x00,0x45,0x7C,0x81,0x70,0x45,
    0x00,0x00,0x4A,0x00,0x81,0x70,0x4A,0x5A,
    0x00,0x45,0x7C,0x81,0x70,0x45,0x00,0x00,
    0x4A,0x00,0x81,0x70,0x4D,0x53,0x00,0x48,
    0x5A,0x87,0x40,0x48,0x00,0x00,0x4D,0x00,
    0x00,0x4F,0x52,0x00,0x4A,0x52,0x81,0x70,
    0x4A,0x00,0x00,0x4F,0x00,0x00,0x51,0x50,
    0x00,0x4C,0x5A,0x81,0x70,0x4C,0x00,0x00,
    0x51,0x00,0x00,0x4F,0x5A,0x00,0x4A,0x5A,
    0x81,0x70,0x4A,0x00,0x00,0x4F,0x00,0x00,
    0x4D,0x5A,0x00,0x48,0x5A,0x81,0x70,0x48,
    0x00,0x00,0x4D,0x00,0x00,0x4A,0x67,0x00,
    0x45,0x67,0x81,0x70,0x45,0x00,0x00,0x4A,
    0x00,0x81,0x70,0x48,0x5A,0x00,0x43,0x67,
    0x81,0x70,0x43,0x00,0x00,0x48,0x00,0x81,
    0x70,0x4A,0x67,0x00,0x45,0x7C,0x87,0x40,
    0x45,0x00,0x00,0x4A,0x00,0x00,0xFF,0x2F,
    0x00,
*/
};	

S32 dummy_background_midi=0;

#endif

 
/****************************************************************************
* Global Function                                                      
*****************************************************************************/
extern void UI_enable_alignment_timers(void);
extern void UI_disable_alignment_timers(void);
extern void AudioStopReq(U16 soundId);
extern U16 	GetCurKeypadTone(void);

/****************************************************************************
* Golbal Variable                                                       
*****************************************************************************/
extern BOOL	r2lMMIFlag;


/****************************************************************************
* Enter Game(New/Resume), Exit Game                                                  
*****************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_gfx_volumn_up()
* DESCRIPTION
*   Set game volumn up
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_gfx_volumn_up(void)
{
	if(GFX.aud_volumn < LEVEL7)
		GFX.aud_volumn++;

	mdi_audio_set_volume(MDI_VOLUME_MEDIA, GFX.aud_volumn);
}


/*****************************************************************************
* FUNCTION
*	 mmi_gfx_volumn_down()
* DESCRIPTION
*   Set game volumn down
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_gfx_volumn_down(void)
{
	if(GFX.aud_volumn > LEVEL1)
		GFX.aud_volumn--;

	mdi_audio_set_volume(MDI_VOLUME_MEDIA, GFX.aud_volumn);
}


/*****************************************************************************
* FUNCTION
*	 mmi_gfx_exit_game()
* DESCRIPTION
*   Called by game when exit the game. usually called by the MMI framework
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_gfx_exit_game(void)
{
	S16 error;

	UI_enable_alignment_timers();
	GFX.game_data.exit_game_func_ptr();	/* call this funtion to stop and exit game */
#if 0 //wufasong delete 20061106
	#ifndef WIN32
	mmi_frm_set_key_handle_in_high_frequency(MMI_FALSE);
	#endif
#endif
	/* re-enable the keypad tone */
	if(GFX.game_data.is_keypad_audio_enable == FALSE)
	{
		mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED);
	}

	/* stop all playing audio and vibration */
	mdi_audio_stop_string(); //Fix Bug#11358 for cs2324c by cong.li on 2009.06.08.
	
	VibratorOff();

	/* resume background play */
	mdi_audio_resume_background_play();

#ifdef __MMI_SUBLCD__
	/* resume sublcd */
	GoBackSubLCDHistory();
#endif
	/* write volumn setting */
	WriteValue(NVRAM_GFX_AUDIO_VOLUMN, &GFX.aud_volumn, DS_BYTE , &error);

	/* let MMI can sleep */
	TurnOffBacklight();
}


/*****************************************************************************
* FUNCTION
*	 mmi_gfx_enter_game()
* DESCRIPTION
*   Enter the game
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_gfx_enter_game(void)
{
	S16 error;

	EntryNewScreen(GFX_GAME_SCREEN, mmi_gfx_exit_game, NULL, NULL);

	/* reset clip */
	gdi_layer_reset_clip();
	gdi_layer_reset_text_clip();
	
	/* entry full screen app */
	entry_full_screen();

	/* disable the keypad tone */
	if(GFX.game_data.is_keypad_audio_enable == FALSE)
	{
		/* force all playing keypad tone off */
		AudioStopReq(GetCurKeypadTone());

		mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);
	}

	/* read volumn setting */
	ReadValue(NVRAM_GFX_AUDIO_VOLUMN, &GFX.aud_volumn, DS_BYTE , &error);
	if(GFX.aud_volumn == 0xff)
	{
		GFX.aud_volumn  = LEVEL4;
	}


	GFX.is_first_time_enter_gameover = TRUE;	/* first time enter */

	/* suspend background play */
    //    if(TRUE == GFX.is_aud_on)
        {
        	mdi_audio_suspend_background_play();
        }   

	#ifdef __MMI_GAME_MULTICHANNEL_SOUND__
			
	mdi_audio_set_volume(MDI_VOLUME_MEDIA, GFX.aud_volumn);

	#endif
	ClearAllKeyHandler();
	clear_category_screen_key_handlers();

	SetKeyHandler(GoBackHistory,	KEY_LSK, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,	KEY_RSK, KEY_EVENT_UP);
	SetKeyHandler(mmi_gfx_volumn_up,	KEY_VOL_UP, KEY_EVENT_DOWN);
	SetKeyHandler(mmi_gfx_volumn_down,	KEY_VOL_DOWN, KEY_EVENT_DOWN);

	/* reset clip */
	coolsand_UI_reset_text_clip();
	coolsand_UI_reset_clip();

	/* stop MMI sleep */
	TurnOnBacklight(1);

#ifdef __MMI_SUBLCD__	
	/* draw game icon on sublcd */
	ForceSubLCDScreen(mmi_gfx_entry_sublcd_screen);	
#endif
	/*
	* fill background with white color - 
	* for display smaller game on bigger LCM that will have clear background 
	*/
#ifdef __MMI_MAINLCD_220X176__
	coolsand_UI_fill_rectangle(0,0, UI_device_width-1, UI_device_height-1,*(current_MMI_theme->UI_black_color));
#else
	coolsand_UI_fill_rectangle(0,0, UI_device_width-1, UI_device_height-1,coolsand_UI_color(255,255,255));
#endif
	/* lock and blt, this may force full screen region will be blt when enter game */
	gdi_layer_lock_frame_buffer();
	gdi_layer_blt_previous(0, 0, UI_device_width-1, UI_device_height-1);
	gdi_layer_unlock_frame_buffer();

	/* disalbe align timer and enter game */
	UI_disable_alignment_timers();
#if 0 //wufasong delete 20061106
	#ifndef WIN32
	mmi_frm_set_key_handle_in_high_frequency(MMI_TRUE);
	#endif
#endif
	GFX.is_resume_game = TRUE;
	GFX.game_data.enter_game_func_ptr();
}


/*****************************************************************************
* FUNCTION
*	 mmi_gfx_new_game()
* DESCRIPTION
*   Enter new game
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_gfx_new_game(void)
{	
	/* reset new game flag */	
	*(GFX.game_data.is_new_game) = TRUE;

	mmi_gfx_enter_game();
}


/*****************************************************************************
* FUNCTION
*	 mmi_gfx_resume_game()
* DESCRIPTION
*   Enter resume game
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_gfx_resume_game(void)
{
	*(GFX.game_data.is_new_game) = FALSE;

	mmi_gfx_enter_game();
}



/***************************************************************************
*  Gameover Screen												        
***************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_gfx_store_grade()
* DESCRIPTION
*   Calculate best grade, write into NVRAM
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_gfx_store_grade(void)
{
	S16 error		= 0;
	U16 grade		= 0;
	U16 best_grade	= 0;
	U16 i;
	
	/* if only have one level */
	if(GFX.game_data.level_count == 1)
	{
		ReadValue((U8)(GFX.game_data.grade_nvram_id_list[0]), (void*)&grade, DS_SHORT , &error);
		/* first time write in grade */
		if(grade == 0xffff)
		{
			WriteValue((U8)(GFX.game_data.grade_nvram_id_list[0]), GFX.game_data.grade_value_ptr, DS_SHORT , &error);
			return;
		}
		
		/* compare to get best grade */
		best_grade = GFX.game_data.best_grade_func_ptr(grade, *(GFX.game_data.grade_value_ptr));

		/* write back best grade to NVRAM */
		WriteValue((U8)(GFX.game_data.grade_nvram_id_list[0]), &best_grade, DS_SHORT , &error);
		return;
	}
	
	
	for(i = 0 ; i < GFX.game_data.level_count; i++)
	{
		/* if current grade equal to index */
		if(i == (*(GFX.game_data.level_index_ptr)))
		{
			ReadValue((U8)(GFX.game_data.grade_nvram_id_list[i]), (void*)&grade, DS_SHORT , &error);
		
			/* first time write in grade */
			if(grade == 0xffff)
			{
				WriteValue((U8)(GFX.game_data.grade_nvram_id_list[i]), GFX.game_data.grade_value_ptr, DS_SHORT , &error);
				return;
			}
			
			/* compare to get best grade */
			best_grade = GFX.game_data.best_grade_func_ptr(grade, *(GFX.game_data.grade_value_ptr));

			/* write back best grade to NVRAM */
			WriteValue((U8)(GFX.game_data.grade_nvram_id_list[i]), &best_grade, DS_SHORT , &error);
			return;
		}
	}
}


/*****************************************************************************
* FUNCTION
*	 mmi_gfx_exit_gameover_screen()
* DESCRIPTION
*   Exit gameover screen
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_gfx_exit_gameover_screen(void)
{
	/* stop all playing audio and vibration */
	VibratorOff();
	mdi_audio_stop_string();// wufasong delete 20061106
//if(GFX.is_aud_on)
{
	/* resume if there is background playing */
	mdi_audio_resume_background_play();
}

}

/*****************************************************************************
* FUNCTION
*	 mmi_gfx_entry_gameover_screen()
* DESCRIPTION
*   Entry of gameover screen
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_gfx_entry_gameover_screen(void)
{
	U8  *guiBuffer;       /* Buffer holding history data */

	/* entry full screen app */
	entry_full_screen();
	
	EntryNewScreen(GFX_GAMEOVER_SCREEN, mmi_gfx_exit_gameover_screen, mmi_gfx_entry_gameover_screen, NULL);

	/* check if is frist time enter, or re-enter because interrupt */
	guiBuffer = GetCurrGuiBuffer(GFX_GAMEOVER_SCREEN);

	/* not first time enter */
	if (guiBuffer != NULL)
		GFX.is_first_time_enter_gameover = FALSE;

//if(GFX.is_aud_on)
{
	/* suspend background play */
	mdi_audio_suspend_background_play();
}

	/* store best grade */
	mmi_gfx_store_grade();
#if 0 
	ShowCategory221Screen(	0, 0,														/* caption */
									STR_GLOBAL_OK,	IMG_GLOBAL_OK,						/* LSK */
									0,	0,														/* RSK */
									GDI_COLOR_WHITE,
									GFX.game_data.draw_gameover_func_ptr);			/* redraw callback */
#else
#ifdef __MMI_MAINLCD_220X176__
	/*No multi-input for QPAD.*/
	ShowCategory1Screen(0, 0, STR_GLOBAL_OK, IMG_GLOBAL_OK, 0, 0, 0, NULL, 0, NULL);
#else
	ShowCategory2Screen(0, 0, STR_GLOBAL_OK, IMG_GLOBAL_OK, 0, 0, 0, NULL);
#endif
	GFX.game_data.draw_gameover_func_ptr();
#endif
	/* go back to game menu */
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	/* gameover will go back to first menuitem */
	GFX.is_gameover = TRUE;

	SetLeftSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


/*****************************************************************************
* FUNCTION
*	 mmi_gfx_draw_gameover_screen()
* DESCRIPTION
*   draw gameover screen
* PARAMETERS
*	 text_img_id		IN		text image id
*	 box_img_id			IN		box image id
*	 pic_img_id			IN		picture image id
*	 score				IN 	score
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_gfx_draw_gameover_screen(U16 text_img_id, U16 box_img_id, U16 pic_img_id, S32 score)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8 	buffer[64];
	U8 	buffer_UCS2[64];
	S32	text_image_width;
	S32	text_image_height;
	S32	box_image_width;
	S32	box_image_height;
	S32	pic_image_width;
	S32	pic_image_height;
	S32	text_image_offset_y;
	S32   box_image_offset_y;
	S32	pic_image_offset_y;
	S32	score_str_offset_y;
	S32	score_str_offset_x;
	S32	str_width;
	S32	str_height;
	S32	spacing;

	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	TRACE_AUD_FUNCTION();
  	coolsand_UI_reset_clip();
	coolsand_UI_reset_text_clip();
	coolsand_UI_set_font(&MMI_medium_font);

	/* get image size */
	coolsand_UI_measure_image(get_image(text_img_id), &text_image_width, &text_image_height);
	coolsand_UI_measure_image(get_image(box_img_id), &box_image_width, &box_image_height);
	coolsand_UI_measure_image(get_image(pic_img_id), &pic_image_width, &pic_image_height);	

#if defined (__MMI_MAINLCD_176X220__) //luanyy modify for cs621
	spacing = (UI_device_height-MMI_button_bar_height+MMI_status_bar_height-(text_image_height+box_image_height+pic_image_height))>>2;	
#elif defined(__MMI_MAINLCD_220X176__)/*cong.li added for product QPAD[220*176] on 2009.08.11. */
	spacing = (UI_device_height-MMI_button_bar_height-MMI_status_bar_height-(text_image_height+box_image_height+pic_image_height))>>2;		
#elif defined(__MMI_MAINLCD_128X160__)/*cong.li added for product QPAD[220*176] on 2009.08.11. */
	spacing = (UI_device_height-MMI_button_bar_height+MMI_title_height-(text_image_height+box_image_height+pic_image_height))>>2;	
#elif defined(__MMI_MAINLCD_160X128__)||defined (__MMI_MAINLCD_128X128__)
	spacing = (UI_device_height-MMI_button_bar_height+MMI_title_height-(text_image_height+box_image_height+pic_image_height))>>2;	
#else
	spacing = (UI_device_height-MMI_button_bar_height-(text_image_height+box_image_height+pic_image_height))>>2;
#endif
#if defined(__MMI_MAINLCD_220X176__)
	text_image_offset_y 	= MMI_status_bar_height+spacing;
#else

	text_image_offset_y 	= spacing;
#endif

	box_image_offset_y 	= text_image_offset_y+text_image_height+spacing;
	pic_image_offset_y 	= box_image_offset_y+box_image_height+spacing;
	
	coolsand_UI_show_transparent_image((UI_device_width-text_image_width)>>1, 
												text_image_offset_y, 
												get_image(text_img_id), 
												0);	

	coolsand_UI_show_transparent_image((UI_device_width-box_image_width)>>1, 
												box_image_offset_y, 
												get_image(box_img_id), 
												0);	

	coolsand_UI_show_transparent_image((UI_device_width-pic_image_width)>>1, 
												pic_image_offset_y, 
												get_image(pic_img_id), 
												0);	

	sprintf((PS8)buffer, "%d", (S16)score );
	AnsiiToUnicodeString((PS8) buffer_UCS2, (PS8)buffer);
 
	coolsand_UI_measure_string((UI_string_type)buffer_UCS2, &str_width, &str_height);

	score_str_offset_x = (UI_device_width-str_width)>>1;
	score_str_offset_y = box_image_offset_y+((box_image_height-str_height)>>1);
#ifdef __MMI_MAINLCD_220X176__
	coolsand_UI_set_text_color(*current_MMI_theme->title_text_color);
#else
	coolsand_UI_set_text_color(coolsand_UI_color(0,0,0));
#endif	
	if(r2lMMIFlag)
		coolsand_UI_move_text_cursor( score_str_offset_x+str_width, score_str_offset_y );
	else
		coolsand_UI_move_text_cursor( score_str_offset_x, score_str_offset_y );

   coolsand_UI_print_text((UI_string_type)buffer_UCS2);
	coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1, UI_device_height-1);

}




/***************************************************************************
*  Game Level Screen										        
***************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_gfx_set_game_level()
* DESCRIPTION
*   Set game level in the NVRAM
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_gfx_set_game_level(void)
{
	S16 error;

	(*(GFX.game_data.level_index_ptr)) = (U8)GFX.hilight_index;
	WriteValue(GFX.game_data.level_nvram_id, GFX.game_data.level_index_ptr,DS_BYTE,&error);
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, GAME_NOTIFYDURATION, SUCCESS_TONE);

	DeleteUptoScrID(GFX_MENU_SCREEN);
	*(GFX.game_data.is_new_game)		= TRUE;
	GFX.is_level_changed			= TRUE;
}


/*****************************************************************************
* FUNCTION
*	 mmi_gfx_set_level_highlight_hdlr()
* DESCRIPTION
*   Get current hilight index
* PARAMETERS
*	 index 	IN		hilight index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_gfx_set_level_highlight_hdlr(S32 index)
{
	GFX.hilight_index = index;
}


/*****************************************************************************
* FUNCTION
*	 mmi_gfx_entry_level_screen()
* DESCRIPTION
*   Entry level screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_gfx_entry_level_screen(void)
{
	U8  *guiBuffer;       /* Buffer holding history data */
	U16 i;
	U16 level_str_list[GFX_MAX_LEVEL_COUNT];

	EntryNewScreen(GFX_LEVEL_SCREEN, NULL, mmi_gfx_entry_level_screen, NULL);



	for(i = 0 ; i < GFX.game_data.level_count; i++)
	{
		level_str_list[i] = GFX.game_data.level_str_id_list[i];
	}

	guiBuffer	= GetCurrGuiBuffer(GFX_LEVEL_SCREEN);
	
	RegisterHighlightHandler(mmi_gfx_set_level_highlight_hdlr);
	
	ShowCategory11Screen(GFX.game_data.game_str_id, 
								MAIN_MENU_TITLE_FUNANDGAMES_ICON, 
								STR_GLOBAL_OK, IMG_GLOBAL_OK,
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
								GFX.game_data.level_count, 
								level_str_list,
								*(GFX.game_data.level_index_ptr),
								guiBuffer);

	/* go back to game menu */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetLeftSoftkeyFunction(mmi_gfx_set_game_level, KEY_EVENT_UP);
}

#ifdef __T800_MAIN_MENU__
void mmi_gfx_set_game_soundswitch(void)
{
	S16 error;
#ifdef __MMI_GAME_MULTICHANNEL_SOUND__
	GFX.is_background_music_on = ((U8)GFX.hilight_index == 1)?0:1;
      GFX.is_sound_effect_on = GFX.is_background_music_on;   
      error = GFX.is_background_music_on;
	WriteValue(NVRAM_GFX_BACKGROUND_MUSIC_SETTING, &GFX.is_background_music_on, DS_BYTE , &error);
	WriteValue(NVRAM_GFX_SOUND_EFFECT_SETTING,	&GFX.is_sound_effect_on, DS_BYTE , &error);    
#else
        GFX.is_aud_on =  (U8)GFX.hilight_index ;
	WriteValue(NVRAM_GFX_AUDIO_SETTING,&GFX.is_aud_on, DS_BYTE , &error);
#endif    
	DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, 1, GAME_NOTIFYDURATION, SUCCESS_TONE);

	DeleteUptoScrID(GFX_MENU_SCREEN);
}

void mmi_gfx_entry_soundswitch_screen(void)
{
	U8  *guiBuffer;       /* Buffer holding history data */
	U16 soundswitch_str[2];

	EntryNewScreen(GFX_LEVEL_SCREEN, NULL, mmi_gfx_entry_soundswitch_screen, NULL);

      soundswitch_str[0] = STR_GLOBAL_DETACTIVATE ;
      soundswitch_str[1] = STR_GLOBAL_ACTIVATE;
      
	guiBuffer	= GetCurrGuiBuffer(GFX_LEVEL_SCREEN);	
	RegisterHighlightHandler(mmi_gfx_set_level_highlight_hdlr);	
	ShowCategory11Screen(GFX.game_data.game_str_id, 
								MAIN_MENU_TITLE_FUNANDGAMES_ICON, 
								STR_GLOBAL_OK, IMG_GLOBAL_OK,
								STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
								2, 
								soundswitch_str,
								GFX.is_aud_on,
								guiBuffer);

	/* go back to game menu */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);

	SetLeftSoftkeyFunction(mmi_gfx_set_game_soundswitch, KEY_EVENT_UP);
}
#endif

/***************************************************************************
*  Game Help Menu									        
***************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_gfx_entry_help_screen()
* DESCRIPTION
*   Entry help screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_gfx_entry_help_screen(void)
{
	PS8 buffer;
	S32 bufferSize;

	EntryNewScreen(GFX_HELP_SCREEN, NULL, mmi_gfx_entry_help_screen, NULL);

	buffer		= GetString(GFX.game_data.help_str_id);
	bufferSize	= pfnUnicodeStrlen(buffer);

	ShowCategory74Screen( GFX.game_data.menu_help_str_id, 
								 MAIN_MENU_TITLE_FUNANDGAMES_ICON,
								 0, 0,
								 STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
								 (PU8)buffer, bufferSize, NULL);

	/* go back to game menu */
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}






/***************************************************************************
*  Game Grade Menu   							        
***************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_gfx_reset_grade()
* DESCRIPTION
*   Reset all grade in NVRAM
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_gfx_reset_grade(void)
{
	U16 rest_score = 0;
	S16 error;
	U8  *display_str_ptr;
	U16 i;

	for( i = 0 ; i < GFX.game_data.level_count; i++)
	{
		WriteValue((U8)(GFX.game_data.grade_nvram_id_list[i]), &rest_score, DS_SHORT , &error);
	}
	
	display_str_ptr = (PU8)GetString(STR_GLOBAL_DONE);
	DisplayPopup(display_str_ptr, IMG_GLOBAL_ACTIVATED, 1, GAME_NOTIFYDURATION, SUCCESS_TONE);
	DeleteUptoScrID(GFX_MENU_SCREEN);

}


/*****************************************************************************
* FUNCTION
*	 mmi_gfx_entry_grade_screen()
* DESCRIPTION
*   Entry point of grade screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_gfx_entry_grade_screen(void)
{
	U8		i;
	U16		data;
	S16		error;

	U8 *guiBuffer;
	S8 *gameLevelList[MAX_SUB_MENUS];
	U8 buffer[MAX_SUB_MENU_SIZE];
	U8 buffer_UCS2[MAX_SUB_MENU_SIZE];
	
	EntryNewScreen(GFX_GRADE_SCREEN, NULL, mmi_gfx_entry_grade_screen, NULL);
	
	guiBuffer = GetCurrGuiBuffer(GFX_GRADE_SCREEN); 
	

	for(i = 0 ;i < GFX.game_data.level_count; i++)
	{
		gameLevelList[i] = (PS8)GetString(GFX.game_data.level_str_id_list[i]);

		/* score string */
		ReadValue((U8)GFX.game_data.grade_nvram_id_list[i], &data, DS_SHORT , &error);
		if(data == 0xffff)
		{
			data = 0;
			WriteValue((U8)GFX.game_data.grade_nvram_id_list[i], &data, DS_SHORT , &error);
		}

		sprintf((PS8)buffer, "%d", data);
		AnsiiToUnicodeString((PS8)buffer_UCS2, (PS8)buffer);
		pfnUnicodeStrcpy( (PS8)subMenuDataPtrs[i], (PS8)buffer_UCS2);
	}

	ShowCategory153Screen(	GFX.game_data.game_str_id, 
									MAIN_MENU_TITLE_FUNANDGAMES_ICON, 
									STR_GAME_RESET, 0, 
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
									GFX.game_data.level_count, (U8 **)gameLevelList, subMenuDataPtrs, 0, guiBuffer );
	#ifdef __MMI_TOUCH_SCREEN__
	/* disable the click resopnse which returns ok! */
	mmi_wgui_register_list_item_selected_callback_all(UI_dummy_function);
	#endif 

	SetLeftSoftkeyFunction(mmi_gfx_reset_grade, KEY_EVENT_UP);

	/* go back to game menu */
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

/***************************************************************************
*  Game Main Menu   							        
***************************************************************************/
/*****************************************************************************
* FUNCTION
*	 mmi_gfx_menu_hilight_hdlr()
* DESCRIPTION
*   Game menu screen hilight handler
* PARAMETERS
*	 index	IN		hilight index
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_gfx_menu_hilight_hdlr(S32 index)
{
	S32 i = 0;
	

	if((*(GFX.game_data.is_new_game)) == FALSE)
	{
		/* Resume game */
		if(GFX.game_data.menu_resume_str_id != 0) /* has level menu */
		{
			if(index == i)	 
			{
				SetLeftSoftkeyFunction(	mmi_gfx_resume_game, KEY_EVENT_UP);
				ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
			}
			i++;
		}
	}
		
	/* new game menu item */
	if(index == i)	 /* new game [REQUIRED] */
	{
		SetLeftSoftkeyFunction(	mmi_gfx_new_game, KEY_EVENT_UP);
		ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	}

	/* has level menu item */
	if(GFX.game_data.menu_level_str_id != 0) 
	{
		i++;	
		if(index == i)
		{
			SetLeftSoftkeyFunction(	mmi_gfx_entry_level_screen, KEY_EVENT_UP);
			SetKeyHandler(mmi_gfx_entry_level_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
		}
	}

	/* has grade menu item */
	if(GFX.game_data.menu_grade_str_id != 0) 
	{
		i++;	
		if(index == i)
		{
			SetLeftSoftkeyFunction(	mmi_gfx_entry_grade_screen, KEY_EVENT_UP);
			SetKeyHandler(mmi_gfx_entry_grade_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
		}
	}

	/* has help menu item */
	if(GFX.game_data.menu_help_str_id != 0) 
	{
		i++;	
		if(index == i)
		{
			SetLeftSoftkeyFunction(	mmi_gfx_entry_help_screen, KEY_EVENT_UP);
			SetKeyHandler(mmi_gfx_entry_help_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
		}
	}
#if defined(__MMI_NOKIA_STYLE_N800__)
	/* has soundswitch menu item */
	if(GFX.game_data.menu_soundswitch_str_id != 0) 
	{
		i++;	
		if(index == i)
		{
			#ifdef __T800_MAIN_MENU__
			SetLeftSoftkeyFunction(	mmi_gfx_entry_soundswitch_screen, KEY_EVENT_UP);
			SetKeyHandler(mmi_gfx_entry_soundswitch_screen, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
			#endif
		}
	}
#endif
}


/*****************************************************************************
* FUNCTION
*	 mmi_gfx_entry_menu_screen()
* DESCRIPTION
*   Game menu screen entry point
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_gfx_entry_menu_screen(void)
{
	
	U8* guiBuffer;       /* Buffer holding history data */
	U16 menu_str_list[GFX_MAX_MENU_ITEM_COUNT]; /* Stores the strings id of submenus returned */
	U16 menu_img_list[GFX_MAX_MENU_ITEM_COUNT] =  {	IMG_GLOBAL_L1, IMG_GLOBAL_L2, IMG_GLOBAL_L3,
																	IMG_GLOBAL_L4, IMG_GLOBAL_L5, IMG_GLOBAL_L6, 
																	IMG_GLOBAL_L7, IMG_GLOBAL_L8, IMG_GLOBAL_L9};

	U16 i = 0;
	S16 error;
	U8 data;

    if(HandleLowBatteryProcess())
    {
        mmi_trace(1,"low battery in game app");
        return ;
    }

	EntryNewScreen(GFX_MENU_SCREEN, NULL, mmi_gfx_entry_menu_screen, NULL);
	guiBuffer = GetCurrGuiBuffer(GFX_MENU_SCREEN);	

	/* load level */
	if(GFX.game_data.menu_level_str_id != 0)
	{
		ReadValue(GFX.game_data.level_nvram_id, &data, DS_BYTE,&error);
		if(data == 0xFF)
		{
			/* init */
			*GFX.game_data.level_index_ptr = 0;
		}
		else
		{
			*GFX.game_data.level_index_ptr = data;
		}
	}
	

	/* if change level, set selection back to first */
	if(GFX.is_level_changed)
	{
		if(guiBuffer != NULL)
			guiBuffer[0] = 0;

		GFX.is_level_changed = FALSE;
	}

	/* if gameover, back to first selection */
	if(GFX.is_gameover)
	{
		if(guiBuffer != NULL)
			guiBuffer[0] = 0;

		GFX.is_gameover = FALSE;
	}
	
	if(GFX.is_resume_game == TRUE)
	{
		if(guiBuffer != NULL)
			guiBuffer[0] = 0;

		GFX.is_resume_game = FALSE;		
	}
	
	if((*(GFX.game_data.is_new_game)) == FALSE)
		menu_str_list[i++] = GFX.game_data.menu_resume_str_id;
	
	menu_str_list[i++] = GFX.game_data.menu_new_str_id;

	if(GFX.game_data.menu_level_str_id != 0) 
		menu_str_list[i++] = GFX.game_data.menu_level_str_id;

	menu_str_list[i++] = GFX.game_data.menu_grade_str_id;
	menu_str_list[i++] = GFX.game_data.menu_help_str_id;
    if(GFX.game_data.menu_soundswitch_str_id)
	menu_str_list[i++] = GFX.game_data.menu_soundswitch_str_id;

	RegisterHighlightHandler(mmi_gfx_menu_hilight_hdlr);

	ShowCategory15Screen(	GFX.game_data.game_str_id, 
									MAIN_MENU_TITLE_FUNANDGAMES_ICON, 
									STR_GLOBAL_OK, IMG_GLOBAL_OK,
									STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 
									i, 
									menu_str_list,
									menu_img_list,
									LIST_MENU,
									0,
									guiBuffer);
	
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}



/*****************************************************************************
* FUNCTION
*	 mmi_gx_store_setting()
* DESCRIPTION
*   store game setting back to NVRAM
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_gx_store_setting(void)
{
	S16 error;
#ifdef __MMI_GAME_MULTICHANNEL_SOUND__
	WriteValue(NVRAM_GFX_BACKGROUND_MUSIC_SETTING,		&GFX.is_background_music_on, DS_BYTE , &error);
	WriteValue(NVRAM_GFX_SOUND_EFFECT_SETTING,		&GFX.is_sound_effect_on, DS_BYTE , &error);
#else//__MMI_GAME_MULTICHANNEL_SOUND__
	WriteValue(NVRAM_GFX_AUDIO_SETTING,		&GFX.is_aud_on, DS_BYTE , &error);
#endif//__MMI_GAME_MULTICHANNEL_SOUND__
	WriteValue(NVRAM_GFX_VIBRATION_SETTING, &GFX.is_vib_on, DS_BYTE , &error);
}


/*****************************************************************************
* FUNCTION
*	 mmi_gx_load_setting()
* DESCRIPTION
*   load game setting from NVRAM
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_gx_load_setting(void)
{
	S16 error;
#ifdef __MMI_GAME_MULTICHANNEL_SOUND__

	ReadValue(NVRAM_GFX_BACKGROUND_MUSIC_SETTING, &GFX.is_background_music_on, DS_BYTE , &error);

	//GFX.is_background_music_on = 0xff;
	/* if not init, init as ON */
	if(GFX.is_background_music_on == 0xff)
	{
		GFX.is_background_music_on = TRUE;
		WriteValue(NVRAM_GFX_BACKGROUND_MUSIC_SETTING,&GFX.is_background_music_on, DS_BYTE , &error);
	}

	ReadValue(NVRAM_GFX_SOUND_EFFECT_SETTING, &GFX.is_sound_effect_on, DS_BYTE , &error);

	//GFX.is_sound_effect_on = 0xff;
	/* if not init, init as ON */
	if(GFX.is_sound_effect_on == 0xff)
	{
		GFX.is_sound_effect_on = TRUE;
		WriteValue(NVRAM_GFX_SOUND_EFFECT_SETTING,&GFX.is_sound_effect_on, DS_BYTE , &error);
	}
#else//__MMI_GAME_MULTICHANNEL_SOUND__
	ReadValue(NVRAM_GFX_AUDIO_SETTING, &GFX.is_aud_on, DS_BYTE , &error);

	/* if not init, init as ON */
	if(GFX.is_aud_on == 0xff)
	{
		GFX.is_aud_on = TRUE;
		WriteValue(NVRAM_GFX_AUDIO_SETTING,	&GFX.is_aud_on, DS_BYTE , &error);
	}
#endif//__MMI_GAME_MULTICHANNEL_SOUND__
	
	ReadValue(NVRAM_GFX_VIBRATION_SETTING, &GFX.is_vib_on, DS_BYTE , &error);

	/* if not init, init as ON */
	if(GFX.is_vib_on == 0xff)
	{
		GFX.is_vib_on = TRUE;
		WriteValue(NVRAM_GFX_VIBRATION_SETTING,	&GFX.is_vib_on, DS_BYTE , &error);
	}
}

#ifdef __MMI_SUBLCD__
/*****************************************************************************
* FUNCTION
*	 mmi_gfx_entry_sublcd_screen()
* DESCRIPTION
*   entry sublcd
* PARAMETERS
*	 void
* RETURNS
*   BOOL
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_gfx_entry_sublcd_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	ExecSubLCDCurrExitHandler();
	ShowCategory332Screen(IMG_GAME_SUBLCD_ICON);
}
#endif /* __MMI_SUBLCD__ */

void mmi_gameapp_clean_app(void)
{
    //   DeleteScreenIfPresent(SCR_ID_ANALOGTV_APP);
    if(IsScreenPresent( GFX_MENU_SCREEN))
    {
        DeleteUptoScrID(IDLE_SCREEN_ID);
    }
}

#endif /* __MMI_GAME__ */

