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
 *  ComposeRingToneEngineDefs.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/FunAndGames/include/ComposeRingToneEngineDefs.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 10:57:56   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:38:24   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:24:32   zhangxb
 * Initial revision.
 * Revision 1.1.1.1  2006/08/02 02:23:56  sushipeng
 * del .h in the include
 *
 * Revision 1.1.1.1  2006/07/22 08:01:18  sushipeng
 * second change new platform 
 
 *
 * Revision 1.1.1.1  2006/07/19 03:49:31  sushipeng
 * change coolsand platform
 
 *
 * Revision 1.1.1.1  2006/06/08 12:42:59  liuruijin
 * no message
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
 
/**************************************************************

	FILENAME	: ComposeRingToneEngineDefs.h

  	PURPOSE		: Resource IDs for the RingToneComposer Application

 


	DATE		: Restructured on May-24-2003

**************************************************************/
#ifndef RING_COMPOSER_DEFS_H
#define RING_COMPOSER_DEFS_H

#include "mmi_features.h"
#include "gui_data_types.h"
#if defined(__MMI_RING_COMPOSER__) && defined(__MMI_IMELODY_SUPPORT__)
#if 0
#define	MAX_NOTES				150
#define	NUM_NOTES_IN_STAVE		5			/* maximal number of notes per stave */

#define	INSTR_PIANO				2
#define	INSTR_MUSICBOX			11
#define	INSTR_XYLOPHONE			13
#define	INSTR_HARMONICA			23
#define	INSTR_GUITAR			26
#define	INSTR_VIOLIN			45
#define	INSTR_SPHONE			66
#define	INSTR_FLUTE				74
#define	INSTR_TRUMPET			57
#define	INSTR_SDRUMS			115

#define	SLOW_SPEED				60
#define	NORMAL_SPEED			120	
#define	FAST_SPEED				180

typedef struct
{
	UI_image_type ImageList[NUM_NOTES_IN_STAVE];
	S16	TotalNotes;					/* total number of notes*/
	U16	CurrNoteIndex;				/* current position of cursor */
	U16	CurrUIPos;					/* current UI position */
	U16	NotesList[MAX_NOTES];	
	U8 YPos[NUM_NOTES_IN_STAVE];
	U8 IsRskLP;						/* if RSK is long-pressed */
}rngc_engine_context_struct;

enum ComposerStr
{
	STR_RING_COMPOSE_TITLE	=COMPOSE_RING_TONE_ENGINE_BASE+1
};

enum ComposeImages
{
 	IMG_3BY2_OCT_L_M_N = COMPOSE_RING_TONE_ENGINE_BASE+1,
 	IMG_3BY2_OCT_L_M_F,
 	IMG_3BY2_OCT_L_M_S,

 	IMG_3BY2_OCT_U_N,
 	IMG_3BY2_OCT_U_F,
 	IMG_3BY2_OCT_U_S,

 	IMG_1_OCT_L_M_N,
 	IMG_1_OCT_L_M_F,
 	IMG_1_OCT_L_M_S,

 	IMG_1_OCT_U_N,
 	IMG_1_OCT_U_F,
 	IMG_1_OCT_U_S,

 	IMG_3BY4_OCT_L_M_N,
 	IMG_3BY4_OCT_L_M_F,
 	IMG_3BY4_OCT_L_M_S,

 	IMG_3BY4_OCT_U_N,
 	IMG_3BY4_OCT_U_F,
 	IMG_3BY4_OCT_U_S,

 	IMG_1BY2_OCT_L_M_N,
 	IMG_1BY2_OCT_L_M_F,
 	IMG_1BY2_OCT_L_M_S,

 	IMG_1BY2_OCT_U_N,
 	IMG_1BY2_OCT_U_F,
 	IMG_1BY2_OCT_U_S,

 	IMG_3BY8_OCT_L_M_N,
 	IMG_3BY8_OCT_L_M_F,
 	IMG_3BY8_OCT_L_M_S,

 	IMG_3BY8_OCT_U_N,
 	IMG_3BY8_OCT_U_F,
 	IMG_3BY8_OCT_U_S,

 	IMG_1BY4_OCT_L_M_N,
 	IMG_1BY4_OCT_L_M_F,
 	IMG_1BY4_OCT_L_M_S,

 	IMG_1BY4_OCT_U_N,
 	IMG_1BY4_OCT_U_F,
 	IMG_1BY4_OCT_U_S,

 	IMG_3BY16_OCT_L_M_N,
 	IMG_3BY16_OCT_L_M_F,
 	IMG_3BY16_OCT_L_M_S,

 	IMG_3BY16_OCT_U_N,
	IMG_3BY16_OCT_U_F,
	IMG_3BY16_OCT_U_S,

	IMG_1BY8_OCT_L_M_N,
	IMG_1BY8_OCT_L_M_F,
	IMG_1BY8_OCT_L_M_S,

 	IMG_1BY8_OCT_U_N,
 	IMG_1BY8_OCT_U_F,
 	IMG_1BY8_OCT_U_S,

 	IMG_3BY32_OCT_L_M_N,
 	IMG_3BY32_OCT_L_M_F,
 	IMG_3BY32_OCT_L_M_S,

 	IMG_3BY32_OCT_U_N,
 	IMG_3BY32_OCT_U_F,
 	IMG_3BY32_OCT_U_S,

 	IMG_1BY16_OCT_L_M_N,
 	IMG_1BY16_OCT_L_M_F,
 	IMG_1BY16_OCT_L_M_S,

 	IMG_1BY16_OCT_U_N,
 	IMG_1BY16_OCT_U_F,
 	IMG_1BY16_OCT_U_S,
	IMG_3BY8_OCT_L_M,
	IMG_1BY4_OCT_L_M,
	IMG_3BY16_OCT_L_M,
	IMG_1BY8_OCT_L_M,
	IMG_3BY32_OCT_L_M,
	IMG_1BY16_OCT_L_M,

/*add note images on top of this***********/
	IMG_RING_EVENTBL_ON,
	IMG_RING_EVENTBL_OFF,
	IMG_RING_EVENTLED_ON,
	IMG_RING_EVENTLED_OFF,
	IMG_RING_EVENTVIB_ON,
	IMG_RING_EVENTVIB_OFF,

	IMG_RING_EVENT1,
	IMG_RING_EVENT2,
	IMG_RING_EVENT3,
	IMG_RING_EVENT4,
	IMG_RING_EVENT5,
	IMG_RING_INSTRU1,
	IMG_RING_INSTRU2,
	IMG_RING_INSTRU3,
	IMG_RING_INSTRU4,
	IMG_RING_INSTRU5,
	IMG_RING_INSTRU6,
	IMG_RING_INSTRU7,
	IMG_RING_INSTRU8,
	IMG_RING_INSTRU9,
	IMG_RING_INSTRU10,
	IMG_RING_COMPOSER_BACKGROUND
};

enum NotesList{
	NOTE_C = 0x0001,
	NOTE_D,
	NOTE_E,
	NOTE_F,
	NOTE_G,
	NOTE_A,
	NOTE_B,
	NOTE_REST,
	NOTE_VIBRATION_ON,
	NOTE_VIBRATION_OFF,
	NOTE_LED_ON,
	NOTE_LED_OFF,
	NOTE_BACKLIGHT_ON,
	NOTE_BACKLIGHT_OFF

};			// First Four bits of the Note

enum UINote
{
	FIRST_POS=0,
	SEC_POS,
	THIRD_POS,
	FOURTH_POS,
	FIFTH_POS,
	MAX_DISPLAY_POS
};
enum NoteType
{
	NORMAL = 0x0010,	
	FLAT = 0x0020,		
	SHARP = 0x0030		
};		// Bit 5 , 6 


enum Octave
{
	OCTAVE_3 = 0x0040,	////000000000100000
	OCTAVE_4 = 0x0080,	////000000001000000
	OCTAVE_5 = 0x00C0	////000000001100000
};			//bits 7,8,9 for octave

enum Duration
{
	DURATION_3BY2		= 0x0200,	//00001000000000
	DURATION_1			= 0x0400,
	DURATION_3BY4		= 0x0600,
	DURATION_1BY2		= 0x0800,
	DURATION_3BY8		= 0x0A00,
	DURATION_1BY4		= 0x0C00,
	DURATION_3BY16		= 0x0E00,
	DURATION_1BY8		= 0x1000,
	DURATION_3BY32		= 0x1200,
	DURATION_1BY16		= 0x1400

};			// bits 10,11,12,13

extern rngc_engine_context_struct *g_rngc_eng_cntx;
#endif
#endif //#if defined(__MMI_RING_COMPOSER__)
#endif


