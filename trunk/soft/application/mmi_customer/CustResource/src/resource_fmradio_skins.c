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
*	resource_fmradio_skins.c
*
* Project:
* --------
*   MAUI
*
* Description:
* ------------
*   FM radio main screen skin settings
*
* Author:
* -------
*	Tim Chen
*
*==============================================================================
* 				HISTORY
* Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!! 
 *------------------------------------------------------------------------------
 * $Log: resource_fmradio_skins.c,v $
 * Revision 1.1.1.1  2006/10/24 08:14:38  huangjiwei
 * BASELINE VERSION
 *
 *
 * Mar 1 2006 CSD00676
 * [MAUI_00219676] FM radio-volume and "+" key is overlap
 * 
 *
 * Feb 28 2006 CSD00676
 * [MAUI_00219638] FM radio-FM radio scenia pointer location is wrong
 * 
 *
 * Feb 28 2006 CSD00676
 * [MAUI_00219636] FM Radio_the pointer location is wrong
 * 
 *
 * Jan 11 2006 CSD00676
 * [MAUI_00218584] FM radio-Channel name roll location is wrong
 * 
 *
 * Dec 31 2005 CSD00676
 * [MAUI_00218256] FM redio-Channel is cut
 * 
 *
 * Nov 18 2005 CSD00676
 * [MAUI_00152416] FM Radio-Click  "+" to  add volume,the "+" icon will display overlap
 * 
 *
 * Oct 26 2005 CSD00676
 * [MAUI_00152417] FM Radio-Skin 3 's icon ,click "+"  "-" to  change volume,it will not work
 * 
 *
 * Oct 26 2005 CSD00676
 * [MAUI_00152416] FM Radio-Click  "+" to  add volume,the "+" icon will display overlap
 * 
 *
 * Oct 3 2005 CSD00676
 * [MAUI_00126985] [FM Radio] Change skin coordination according to the new 176x220 skin
 * 
 *
 *------------------------------------------------------------------------------
* Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!! 
*==============================================================================
*******************************************************************************/

#include "mmi_features.h"

#ifdef __MMI_FM_RADIO__

#include "gdi_lcd_config.h"

// dependent headers for FMRadioDef.h"
#include "mmi_data_types.h"
#include "gui_data_types.h"

#include "fmradiodef.h"
#include "fmradiotype.h"

#if ( (LCD_WIDTH == 128) && (LCD_HEIGHT == 128) )
const mmi_fmrdo_skin_struct g_fmrdo_skins[] =
{
	{ // skin 1
		0, 0, 0, // LSK text border color
		255, 0, 255, // LSK text color
		0, 0, 0, // RSK text border color
		0, 255, 255, // RSK text color
		IMG_ID_FMRDO_SKIN_1_BACKGROUND, // background image
		255, 255, 255, // channel name text border color
		100, 100, 100, // channel name text color
		2, 2, // channel name x, y
		124, 20, // channel name w, h
		0, 255, 0, // frequency text on color
		0, 150, 0, // frequency text off color
		35, 35, // frequency x, y
		58, 12, // frequency w, h
		IMG_ID_FMRDO_SKIN_1_VOLUME, // volume image
		0, // volume bar type -> 0: horizontal, 1: vertical
		46, 49, // x, y
		7, 0, // volume indicator start gap, end gap
		IMG_ID_FMRDO_SKIN_1_TUNER_SCALE, // tuner scale background image
		IMG_ID_FMRDO_SKIN_1_TUNER_SCALE_INDICATOR, // tuner scale indicator image
		0, // tuner scale type -> // 0: horizontal, 1: vertical
		20, 61, // tuner scale x, y
		0, // tuner scale ind offset
		1, 0, // tuner scale start gap, end gap
		IMG_ID_FMRDO_SKIN_1_STEP_DOWN_BUTTON_UP, // step down button up image
		IMG_ID_FMRDO_SKIN_1_STEP_DOWN_BUTTON_DOWN, // step down button down image
		28, 70, // step down button x, y
		IMG_ID_FMRDO_SKIN_1_STEP_UP_BUTTON_UP, // step up button up image
		IMG_ID_FMRDO_SKIN_1_STEP_UP_BUTTON_DOWN, // step up button down image
		89, 70, // step up button x, y
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_ON_UP, // search button on up image
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_ON_DOWN, // search button on down image
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_OFF_UP, // search button off up image
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_OFF_DOWN, // search button off down image
		42, 77, // search button x, y
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_UP, // power button on up image
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_DOWN, // power button on down image
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_OFF_UP, // power button off up image
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_OFF_DOWN, // power button off down image
		56, 91, // power button x, y
		FMRDO_BUTTON_SEARCH, FMRDO_BUTTON_POWER, FMRDO_BUTTON_STEP_DOWN, FMRDO_BUTTON_STEP_UP,FMRDO_BUTTON_POWER // up, down, left, right key mapping
	} , 
	{ // skin 2
		0, 0, 0, // LSK text border color
		255, 0, 255, // LSK text color
		0, 0, 0, // RSK text border color
		0, 255, 255, // RSK text color
		IMG_ID_FMRDO_SKIN_2_BACKGROUND, // background image
		255, 255, 255, // channel name text border color
		100, 100, 100, // channel name text color
		2, 2, // channel name x, y
		124, 20, // channel name w, h
		0, 255, 0, // frequency text on color
		0, 150, 0, // frequency text off color
		35, 35, // frequency x, y
		58, 12, // frequency w, h
		IMG_ID_FMRDO_SKIN_2_VOLUME, // volume image
		0, // volume bar type -> 0: horizontal, 1: vertical
		46, 49, // x, y
		7, 0, // volume indicator start gap, end gap
		IMG_ID_FMRDO_SKIN_2_TUNER_SCALE, // tuner scale background image
		IMG_ID_FMRDO_SKIN_2_TUNER_SCALE_INDICATOR, // tuner scale indicator image
		0, // tuner scale type -> // 0: horizontal, 1: vertical
		20, 61, // tuner scale x, y
		0, // tuner scale ind offset
		1, 0, // tuner scale start gap, end gap
		IMG_ID_FMRDO_SKIN_2_STEP_DOWN_BUTTON_UP, // step down button up image
		IMG_ID_FMRDO_SKIN_2_STEP_DOWN_BUTTON_DOWN, // step down button down image
		28, 70, // step down button x, y
		IMG_ID_FMRDO_SKIN_2_STEP_UP_BUTTON_UP, // step up button up image
		IMG_ID_FMRDO_SKIN_2_STEP_UP_BUTTON_DOWN, // step up button down image
		89, 70, // step up button x, y
		IMG_ID_FMRDO_SKIN_2_SEARCH_BUTTON_ON_UP, // search button on up image
		IMG_ID_FMRDO_SKIN_2_SEARCH_BUTTON_ON_DOWN, // search button on down image
		IMG_ID_FMRDO_SKIN_2_SEARCH_BUTTON_OFF_UP, // search button off up image
		IMG_ID_FMRDO_SKIN_2_SEARCH_BUTTON_OFF_DOWN, // search button off down image
		42, 77, // search button x, y
		IMG_ID_FMRDO_SKIN_2_POWER_BUTTON_ON_UP, // power button on up image
		IMG_ID_FMRDO_SKIN_2_POWER_BUTTON_ON_DOWN, // power button on down image
		IMG_ID_FMRDO_SKIN_2_POWER_BUTTON_OFF_UP, // power button off up image
		IMG_ID_FMRDO_SKIN_2_POWER_BUTTON_OFF_DOWN, // power button off down image
		56, 91, // power button x, y
		FMRDO_BUTTON_SEARCH, FMRDO_BUTTON_POWER, FMRDO_BUTTON_STEP_DOWN, FMRDO_BUTTON_STEP_UP,FMRDO_BUTTON_POWER // up, down, left, right key mapping
	} ,
	{ // skin 3
		0, 0, 0, // LSK text border color
		255, 0, 255, // LSK text color
		0, 0, 0, // RSK text border color
		0, 255, 255, // RSK text color
		IMG_ID_FMRDO_SKIN_3_BACKGROUND, // background image
		255, 255, 255, // channel name text border color
		100, 100, 100, // channel name text color
		2, 2, // channel name x, y
		124, 20, // channel name w, h
		0, 255, 0, // frequency text on color
		0, 150, 0, // frequency text off color
		35, 35, // frequency x, y
		58, 12, // frequency w, h
		IMG_ID_FMRDO_SKIN_3_VOLUME, // volume image
		0, // volume bar type -> 0: horizontal, 1: vertical
		46, 49, // x, y
		7, 0, // volume indicator start gap, end gap
		IMG_ID_FMRDO_SKIN_3_TUNER_SCALE, // tuner scale background image
		IMG_ID_FMRDO_SKIN_3_TUNER_SCALE_INDICATOR, // tuner scale indicator image
		0, // tuner scale type -> // 0: horizontal, 1: vertical
		20, 61, // tuner scale x, y
		0, // tuner scale ind offset
		1, 0, // tuner scale start gap, end gap
		IMG_ID_FMRDO_SKIN_3_STEP_DOWN_BUTTON_UP, // step down button up image
		IMG_ID_FMRDO_SKIN_3_STEP_DOWN_BUTTON_DOWN, // step down button down image
		28, 70, // step down button x, y
		IMG_ID_FMRDO_SKIN_3_STEP_UP_BUTTON_UP, // step up button up image
		IMG_ID_FMRDO_SKIN_3_STEP_UP_BUTTON_DOWN, // step up button down image
		89, 70, // step up button x, y
		IMG_ID_FMRDO_SKIN_3_SEARCH_BUTTON_ON_UP, // search button on up image
		IMG_ID_FMRDO_SKIN_3_SEARCH_BUTTON_ON_DOWN, // search button on down image
		IMG_ID_FMRDO_SKIN_3_SEARCH_BUTTON_OFF_UP, // search button off up image
		IMG_ID_FMRDO_SKIN_3_SEARCH_BUTTON_OFF_DOWN, // search button off down image
		42, 77, // search button x, y
		IMG_ID_FMRDO_SKIN_3_POWER_BUTTON_ON_UP, // power button on up image
		IMG_ID_FMRDO_SKIN_3_POWER_BUTTON_ON_DOWN, // power button on down image
		IMG_ID_FMRDO_SKIN_3_POWER_BUTTON_OFF_UP, // power button off up image
		IMG_ID_FMRDO_SKIN_3_POWER_BUTTON_OFF_DOWN, // power button off down image
		56, 91, // power button x, y
		FMRDO_BUTTON_SEARCH, FMRDO_BUTTON_POWER, FMRDO_BUTTON_STEP_DOWN, FMRDO_BUTTON_STEP_UP,FMRDO_BUTTON_POWER // up, down, left, right key mapping
	}
};
#elif ( (LCD_WIDTH == 128) && (LCD_HEIGHT == 160) )
const mmi_fmrdo_skin_struct g_fmrdo_skins[] =
{
#if 1 //m301wapmms
{ // skin 1
 		0, 0, 0, // LSK text border color
		255, 255, 255, // LSK text color
		0, 0, 0, // RSK text border color
		255, 255, 255, // RSK text color
		IMG_ID_FMRDO_SKIN_1_BACKGROUND, // background image
		0, 0, 0, // channel name text border color
		255, 255, 255, // channel name text color
		10, 2, // channel name x, y
		100, 20, // channel name w, h
		255, 255, 255, // frequency text on color
		255, 255, 255, // frequency text off color
		35, 65, // frequency x, y
		65, 15, // frequency w, h
		IMG_ID_FMRDO_SKIN_1_VOLUME, // volume image
		0, // volume bar type -> 0: horizontal, 1: vertical
		4, 107,  //VOLUME_BAR x,y
		7,0,//VOLUME_BAR indicator start gap, end gap
		IMG_ID_FMRDO_SKIN_1_TUNER_SCALE, // tuner scale background image
		IMG_ID_FMRDO_SKIN_1_TUNER_SCALE_INDICATOR, // tuner scale indicator image
		0, // tuner scale type -> // 0: horizontal, 1: vertical
		18, 22, //TUNER_SCALE x,y
		10,//ind offset
		1, 4,//TUNER_SCALE start gap, end gap
		IMG_ID_FMRDO_SKIN_1_STEP_DOWN_BUTTON_UP, // step down button up image
		IMG_ID_FMRDO_SKIN_1_STEP_DOWN_BUTTON_DOWN, // step down button down image
		46, 125,//STEP_DOWN_BUTTON x,y
		IMG_ID_FMRDO_SKIN_1_STEP_UP_BUTTON_UP, // step up button up image
		IMG_ID_FMRDO_SKIN_1_STEP_UP_BUTTON_DOWN, // step up button down image
		73, 125, //STEP_UP_BUTTON x,y
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_ON_UP, // search button on up image
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_ON_DOWN, // search button on down image
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_OFF_UP, // search button off up image
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_OFF_DOWN, // search button off down image
		58, 112,//SEARCH_BUTTON x,y
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_UP, // power button on up image
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_DOWN, // power button on down image
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_OFF_UP, // power button off up image
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_OFF_DOWN, // power button off down image
		59, 138, //POWER_BUTTON x,y
		FMRDO_BUTTON_SEARCH,FMRDO_BUTTON_SETTINGS, FMRDO_BUTTON_STEP_DOWN, FMRDO_BUTTON_STEP_UP, FMRDO_BUTTON_POWER // up, down, left, right key mapping		
#if defined(__MMI_FM_RADIO_RECORD__)
		,IMG_ID_FMRDO_SKIN_1_REC,	// record indication icon image
		IMG_ID_FMRDO_SKIN_1_REC_PAUSE,	//record pause indication icon image
		112,100, //RECORD_BUTTON x,y
		IMG_ID_FMRDO_SKIN_1_NUM_0, // record duration time icon. base is 0, 1 is base plus 1, 2 is base plus 2, ...
		IMG_ID_FMRDO_SKIN_1_NUM_COL, // record duration seperator icon
		86,112, //RECORD_DURATION x,y
#endif		
	},
#else
	{ // skin 1
		66, 24, 70, // LSK text border color
		255, 255, 255, // LSK text color
		66, 24, 70, // RSK text border color
		255, 255, 255, // RSK text color
		IMG_ID_FMRDO_SKIN_1_BACKGROUND, // background image
		66, 24, 70, // channel name text border color
		208, 113, 217, // channel name text color
		2, 0, // channel name x, y
		124, 20, // channel name w, h
		113, 64, 130, // frequency text on color
		255, 255, 255, // frequency text off color
		29, 62, // frequency x, y
		70, 12, // frequency w, h
		IMG_ID_FMRDO_SKIN_1_VOLUME, // volume image
		0, // volume bar type -> 0: horizontal, 1: vertical
		14, 107, // volume bar x, y
		0, 0, // volume indicator start gap, end gap
		IMG_ID_FMRDO_SKIN_1_TUNER_SCALE, // tuner scale background image
		IMG_ID_FMRDO_SKIN_1_TUNER_SCALE_INDICATOR, // tuner scale indicator image
		0, // tuner scale type -> // 0: horizontal, 1: vertical
		19, 22, // tuner scale x, y
		10, // tuner scale ind offset
		0, 0, // tuner scale start gap, end gap
		IMG_ID_FMRDO_SKIN_1_STEP_DOWN_BUTTON_UP, // step down button up image
		IMG_ID_FMRDO_SKIN_1_STEP_DOWN_BUTTON_DOWN, // step down button down image
		46, 126, // step down button x, y
		IMG_ID_FMRDO_SKIN_1_STEP_UP_BUTTON_UP, // step up button up image
		IMG_ID_FMRDO_SKIN_1_STEP_UP_BUTTON_DOWN, // step up button down image
		73, 125, // step up button x, y
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_ON_UP, // search button on up image
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_ON_DOWN, // search button on down image
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_OFF_UP, // search button off up image
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_OFF_DOWN, // search button off down image
		58, 139, // search button x, y
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_UP, // power button on up image
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_DOWN, // power button on down image
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_OFF_UP, // power button off up image
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_OFF_DOWN, // power button off down image
		57, 123, // power button x, y
		FMRDO_BUTTON_SETTINGS, FMRDO_BUTTON_SEARCH, FMRDO_BUTTON_STEP_DOWN, FMRDO_BUTTON_STEP_UP, FMRDO_BUTTON_POWER // up, down, left, right key mapping		
#if defined(__MMI_FM_RADIO_RECORD__)
		,IMG_ID_FMRDO_SKIN_1_REC,	// record indication icon image
		IMG_ID_FMRDO_SKIN_1_REC_PAUSE,	//record pause indication icon image
		96,72,	// record icon x, y
		IMG_ID_FMRDO_SKIN_1_NUM_0, // record duration time icon. base is 0, 1 is base plus 1, 2 is base plus 2, ...
		IMG_ID_FMRDO_SKIN_1_NUM_COL, // record duration seperator icon
		57,76	// recprd duration icon x, y
#endif		
	} , 
	#endif
#ifndef __MMI_FM_SLIM_SKIN__
	{ // skin 2
		255, 255, 255, // LSK text border color
		30, 70, 20, // LSK text color
		255, 255, 255, // RSK text border color
		30, 70, 20, // RSK text color
		IMG_ID_FMRDO_SKIN_2_BACKGROUND, // background image
		0, 0, 0, // channel name text border color
		95, 130, 80, // channel name text color
		2, 2, // channel name x, y
		124, 20, // channel name w, h
		255, 255, 255, // frequency text on color
		113, 64, 130, // frequency text off color
		30, 26, // frequency x, y
		70, 12, // frequency w, h
		IMG_ID_FMRDO_SKIN_2_VOLUME, // volume image
		0, // volume bar type -> 0: horizontal, 1: vertical
		18, 73, // volume bar x, y
		6, 0, // volume indicator start gap, end gap
		IMG_ID_FMRDO_SKIN_2_TUNER_SCALE, // tuner scale background image
		IMG_ID_FMRDO_SKIN_2_TUNER_SCALE_INDICATOR, // tuner scale indicator image
		0, // tuner scale type -> // 0: horizontal, 1: vertical
		18, 38, // tuner scale x, y
		12, // tuner scale ind offset
		8, 8, // tuner scale start gap, end gap
		IMG_ID_FMRDO_SKIN_2_STEP_DOWN_BUTTON_UP, // step down button up image
		IMG_ID_FMRDO_SKIN_2_STEP_DOWN_BUTTON_DOWN, // step down button down image
		11, 103, // step down button x, y
		IMG_ID_FMRDO_SKIN_2_STEP_UP_BUTTON_UP, // step up button up image
		IMG_ID_FMRDO_SKIN_2_STEP_UP_BUTTON_DOWN, // step up button down image
		86, 94, // step up button x, y
		IMG_ID_FMRDO_SKIN_2_SEARCH_BUTTON_ON_UP, // search button on up image
		IMG_ID_FMRDO_SKIN_2_SEARCH_BUTTON_ON_DOWN, // search button on down image
		IMG_ID_FMRDO_SKIN_2_SEARCH_BUTTON_OFF_UP, // search button off up image
		IMG_ID_FMRDO_SKIN_2_SEARCH_BUTTON_OFF_DOWN, // search button off down image
		48, 85, // search button x, y
		IMG_ID_FMRDO_SKIN_2_POWER_BUTTON_ON_UP, // power button on up image
		IMG_ID_FMRDO_SKIN_2_POWER_BUTTON_ON_DOWN, // power button on down image
		IMG_ID_FMRDO_SKIN_2_POWER_BUTTON_OFF_UP, // power button off up image
		IMG_ID_FMRDO_SKIN_2_POWER_BUTTON_OFF_DOWN, // power button off down image
		48, 111, // power button x, y
		FMRDO_BUTTON_SEARCH, FMRDO_BUTTON_POWER, FMRDO_BUTTON_STEP_DOWN, FMRDO_BUTTON_STEP_UP,FMRDO_BUTTON_POWER // up, down, left, right key mapping		
#if defined(__MMI_FM_RADIO_RECORD__)
		,IMG_ID_FMRDO_SKIN_2_REC,	// record indication icon image
		IMG_ID_FMRDO_SKIN_2_REC_PAUSE,	//record pause indication icon image
		99,65,	// record icon x, y
		IMG_ID_FMRDO_SKIN_2_NUM_0, // record duration time icon. base is 0, 1 is base plus 1, 2 is base plus 2, ...
		IMG_ID_FMRDO_SKIN_2_NUM_COL, // record duration seperator icon
		57,71	// recprd duration icon x, y
#endif			
	} ,
	{ // skin 3
		63, 63, 63, // LSK text border color
		255, 255, 255, // LSK text color
		63, 63, 63, // RSK text border color
		255, 255, 255, // RSK text color
		IMG_ID_FMRDO_SKIN_3_BACKGROUND, // background image
		63, 63, 63, // channel name text border color
		194, 192, 195, // channel name text color
		2, 2, // channel name x, y
		124, 20, // channel name w, h
		255, 255, 255, // frequency text on color
		113, 64, 130, // frequency text off color
		27, 26, // frequency x, y
		70, 12, // frequency w, h
		IMG_ID_FMRDO_SKIN_3_VOLUME, // volume image
		0, // volume bar type -> 0: horizontal, 1: vertical
		15, 76, // volume bar x, y
		6, 0, // volume indicator start gap, end gap
		IMG_ID_FMRDO_SKIN_3_TUNER_SCALE, // tuner scale background image
		IMG_ID_FMRDO_SKIN_3_TUNER_SCALE_INDICATOR, // tuner scale indicator image
		0, // tuner scale type -> // 0: horizontal, 1: vertical
		18, 38, // tuner scale x, y
		15, // tuner scale ind offset
		5, 6, // tuner scale start gap, end gap
		IMG_ID_FMRDO_SKIN_3_STEP_DOWN_BUTTON_UP, // step down button up image
		IMG_ID_FMRDO_SKIN_3_STEP_DOWN_BUTTON_DOWN, // step down button down image
		22, 109, // step down button x, y
		IMG_ID_FMRDO_SKIN_3_STEP_UP_BUTTON_UP, // step up button up image
		IMG_ID_FMRDO_SKIN_3_STEP_UP_BUTTON_DOWN, // step up button down image
		74, 109, // step up button x, y
		IMG_ID_FMRDO_SKIN_3_SEARCH_BUTTON_ON_UP, // search button on up image
		IMG_ID_FMRDO_SKIN_3_SEARCH_BUTTON_ON_DOWN, // search button on down image
		IMG_ID_FMRDO_SKIN_3_SEARCH_BUTTON_OFF_UP, // search button off up image
		IMG_ID_FMRDO_SKIN_3_SEARCH_BUTTON_OFF_DOWN, // search button off down image
		47, 92, // search button x, y
		IMG_ID_FMRDO_SKIN_3_POWER_BUTTON_ON_UP, // power button on up image
		IMG_ID_FMRDO_SKIN_3_POWER_BUTTON_ON_DOWN, // power button on down image
		IMG_ID_FMRDO_SKIN_3_POWER_BUTTON_OFF_UP, // power button off up image
		IMG_ID_FMRDO_SKIN_3_POWER_BUTTON_OFF_DOWN, // power button off down image
		47, 115, // power button x, y
		FMRDO_BUTTON_SEARCH, FMRDO_BUTTON_POWER, FMRDO_BUTTON_STEP_DOWN, FMRDO_BUTTON_STEP_UP,FMRDO_BUTTON_POWER // up, down, left, right key mapping

#if defined(__MMI_FM_RADIO_RECORD__)
		,IMG_ID_FMRDO_SKIN_3_REC,	// record indication icon image
		IMG_ID_FMRDO_SKIN_3_REC_PAUSE,	//record pause indication icon image
		101,67,	// record icon x, y
		IMG_ID_FMRDO_SKIN_3_NUM_0, // record duration time icon. base is 0, 1 is base plus 1, 2 is base plus 2, ...
		IMG_ID_FMRDO_SKIN_3_NUM_COL, // record duration seperator icon
		63,74	// recprd duration icon x, y
#endif		
	}
#endif
};
#elif ( (LCD_WIDTH == 176) && (LCD_HEIGHT == 220) )
#ifdef __MMI_TOUCH_SCREEN__
#if 0 //backup old style
static const fmrdo_point_struct skin1_volume_inc_points[] = {{0,0},{11,0},{11,18},{0,18}};
static const fmrdo_point_struct skin1_volume_dec_points[] = {{0,0},{11,0},{11,18},{0,18}};
static const fmrdo_point_struct skin1_step_down_button_points[] = {{0,0},{30,0},{30,30},{0,30}};
static const fmrdo_point_struct skin1_step_up_button_points[] = {{0,0},{30,0},{30,30},{0,30}};
static const fmrdo_point_struct skin1_power_button_points[] = {{0,0},{30,0},{30,30},{0,30}};
static const fmrdo_point_struct skin1_search_button_points[] = {{0,0},{30,0},{30,30},{0,30}};
static const fmrdo_point_struct skin1_setting_points[] = {{0,0},{20,0},{20,20},{0,20}};
static const fmrdo_point_struct skin1_frequency_points[] = {{0,0},{69,0},{69,11},{0,11}};
#else
static const fmrdo_point_struct skin1_volume_inc_points[] = {{110,187},{35,0},{35,20},{120,196}};
static const fmrdo_point_struct skin1_volume_dec_points[] = {{64,187},{35,0},{35,20},{74,196}};
static const fmrdo_point_struct skin1_step_down_button_points[] = {{56,168},{20,0},{20,60},{68,180}};
static const fmrdo_point_struct skin1_step_up_button_points[] = {{113,169},{20,0},{20,60},{125,180}};
static const fmrdo_point_struct skin1_power_button_points[] = {{84,166},{60,0},{60,20},{100,180}};
static const fmrdo_point_struct skin1_search_button_points[] ={{66,48},{60,0},{60,20},{109,60}};
static const fmrdo_point_struct skin1_setting_points[] = {{25,171},{31,0},{31,31},{44,188}};
static const fmrdo_point_struct skin1_frequency_points[] = {{66,63},{0,0},{0,0},{111,77}};
#endif
#ifndef __MMI_FM_SLIM_SKIN__

static const fmrdo_point_struct skin2_volume_inc_points[] = {{0,0},{25,0},{25,13},{0,13}};
static const fmrdo_point_struct skin2_volume_dec_points[] = {{0,0},{25,0},{25,13},{0,13}};
static const fmrdo_point_struct skin2_step_down_button_points[] = {{0,0},{25,0},{25,28},{0,28}};
static const fmrdo_point_struct skin2_step_up_button_points[] = {{0,0},{25,0},{25,28},{0,28}};
static const fmrdo_point_struct skin2_power_button_points[] = {{0,0},{25,0},{25,28},{0,28}};
static const fmrdo_point_struct skin2_search_button_points[] = {{0,0},{25,0},{25,28},{0,28}};
static const fmrdo_point_struct skin2_setting_points[] = {{0,0},{25,0},{25,28},{0,28}};
static const fmrdo_point_struct skin2_frequency_points[] = {{0,0},{69,0},{69,11},{0,11}};

static const fmrdo_point_struct skin3_volume_inc_points[] = {{1,0},{24,0},{24,11},{1,11}};
static const fmrdo_point_struct skin3_volume_dec_points[] = {{1,0},{24,0},{24,11},{1,11}};
static const fmrdo_point_struct skin3_step_down_button_points[] = {{0,0},{34,0},{34,34},{0,34}};
static const fmrdo_point_struct skin3_step_up_button_points[] = {{0,0},{34,0},{34,34},{0,34}};
static const fmrdo_point_struct skin3_power_button_points[] = {{0,0},{34,0},{34,34},{0,34}};
static const fmrdo_point_struct skin3_search_button_points[] = {{0,0},{34,0},{34,34},{0,34}};
static const fmrdo_point_struct skin3_setting_points[] = {{0,0},{34,0},{34,34},{0,34}};
static const fmrdo_point_struct skin3_frequency_points[] = {{0,0},{69,0},{69,11},{0,11}};
#endif
#endif
const mmi_fmrdo_skin_struct g_fmrdo_skins[] =
{
#if 0  //backup old style:176X220
	{ // skin 1
		0, 0, 0, // LSK text border color
		255, 0, 255, // LSK text color
		0, 0, 0, // RSK text border color
		255, 0, 255, // RSK text color
		IMG_ID_FMRDO_SKIN_1_BACKGROUND, // background image
		0, 0, 0, // channel name text border color
		70, 250, 205, // channel name text color
		2, 2, // channel name x, y
		172, 20, // channel name w, h
		20, 81, 37, // frequency text on color
		150, 150, 150, // frequency text off color
		56, 56, // frequency x, y
		70, 12, // frequency w, h
		IMG_ID_FMRDO_SKIN_1_VOLUME, // volume image
		0, // volume bar type -> 0: horizontal, 1: vertical
		31, 150, // volume bar x, y
		9, 0, // volume indicator start gap, end gap
		IMG_ID_FMRDO_SKIN_1_TUNER_SCALE, // tuner scale background image
		IMG_ID_FMRDO_SKIN_1_TUNER_SCALE_INDICATOR, // tuner scale indicator image
		0, // tuner scale type -> // 0: horizontal, 1: vertical
		0, 70, // tuner scale x, y
		7, // tuner scale ind offset
		0, 0, // tuner scale start gap, end gap
		IMG_ID_FMRDO_SKIN_1_STEP_DOWN_BUTTON_UP, // step down button up image
		IMG_ID_FMRDO_SKIN_1_STEP_DOWN_BUTTON_DOWN, // step down button down image
		2, 176, // step down button x, y
		IMG_ID_FMRDO_SKIN_1_STEP_UP_BUTTON_UP, // step up button up image
		IMG_ID_FMRDO_SKIN_1_STEP_UP_BUTTON_DOWN, // step up button down image
		71, 176, // step up button x, y
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_ON_UP, // search button on up image
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_ON_DOWN, // search button on down image
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_OFF_UP, // search button off up image
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_OFF_DOWN, // search button off down image
		140, 176, // search button x, y
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_UP, // power button on up image
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_DOWN, // power button on down image
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_OFF_UP, // power button off up image
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_OFF_DOWN, // power button off down image
		37, 176, // power button x, y
		FMRDO_BUTTON_SEARCH, FMRDO_BUTTON_POWER, FMRDO_BUTTON_STEP_DOWN, FMRDO_BUTTON_STEP_UP, // up, down, left, right key mapping

#ifdef __MMI_TOUCH_SCREEN__
		59,150,// volume_inc_x, volume_inc_y,
		IMG_ID_FMRDO_SKIN_1_VOLUME_INC_UP,// volume_inc_up_image_id
		IMG_ID_FMRDO_SKIN_1_VOLUME_INC_DOWN,// volume_inc_down_image_id
		2,150,// volume_dec_x, volume_dec_y,
		IMG_ID_FMRDO_SKIN_1_VOLUME_DEC_UP,// volume_dec_up_image_id
		IMG_ID_FMRDO_SKIN_1_VOLUME_DEC_DOWN,// volume_dec_down_image_id
		105,176,// setting_x, setting_y
		IMG_ID_FMRDO_SKIN_1_SETTING_UP, // setting_up_image_id
		IMG_ID_FMRDO_SKIN_1_SETTING_DOWN, // setting_down_image_id
		{(const fmrdo_point_struct *)skin1_volume_inc_points, 4}, // volume_inc_area
		{(const fmrdo_point_struct *)skin1_volume_dec_points, 4}, // volume_dec_area
		{(const fmrdo_point_struct *)skin1_step_down_button_points, 4}, // step_down_button_area
		{(const fmrdo_point_struct *)skin1_step_up_button_points, 4}, // step_up_button_area
		{(const fmrdo_point_struct *)skin1_power_button_points, 4}, // power_button_area
		{(const fmrdo_point_struct *)skin1_search_button_points, 4}, // search_button_area
		{(const fmrdo_point_struct *)skin1_setting_points, 4}, // setting_area
		{(const fmrdo_point_struct *)skin1_frequency_points, 4}, // frequency_area
#endif
#if defined(__MMI_FM_RADIO_RECORD__)
		IMG_ID_FMRDO_SKIN_1_REC,	// record indication icon image
		IMG_ID_FMRDO_SKIN_1_REC_PAUSE,	//record pause indication icon image
		99,131,	// record icon x, y
		IMG_ID_FMRDO_SKIN_1_NUM_0, // record duration time icon. base is 0, 1 is base plus 1, 2 is base plus 2, ...
		IMG_ID_FMRDO_SKIN_1_NUM_COL, // record duration seperator icon
		96,113	// recprd duration icon x, y
#endif	
	} , 
#endif
	{ // skin 1
		255, 255, 255, // LSK text border color
		255, 0, 255, // LSK text color
		255, 255, 255, // RSK text border color
		255, 0, 255, // RSK text color
		IMG_ID_FMRDO_SKIN_1_BACKGROUND, // background image
		0, 0, 0, // channel name text border color
		70, 250, 205, // channel name text color
		41,25, //CHANNEL_NAME x, y
		96,18, //CHANNEL_NAME w, h
		20, 81, 37, // frequency text on color
		150, 150, 150, // frequency text off color
		66,63, //FREQUENCY x, y
		50,18, //FREQUENCY w, h
		IMG_ID_FMRDO_SKIN_1_VOLUME, // volume image
		0, // volume bar type -> 0: horizontal, 1: vertical
		79,188, //VOLUME_BAR x,y
		7,0,//VOLUME_BAR indicator start gap, end gap
		IMG_ID_FMRDO_SKIN_1_TUNER_SCALE, // tuner scale background image
		IMG_ID_FMRDO_SKIN_1_TUNER_SCALE_INDICATOR, // tuner scale indicator image
		0, // tuner scale type -> // 0: horizontal, 1: vertical
		29,89, //TUNER_SCALE x,y
		18,//ind offset
		19,29,//TUNER_SCALE start gap, end gap
		IMG_ID_FMRDO_SKIN_1_STEP_DOWN_BUTTON_UP, // step down button up image
		IMG_ID_FMRDO_SKIN_1_STEP_DOWN_BUTTON_DOWN, // step down button down image
		56,168, //STEP_DOWN_BUTTON x,y
		IMG_ID_FMRDO_SKIN_1_STEP_UP_BUTTON_UP, // step up button up image
		IMG_ID_FMRDO_SKIN_1_STEP_UP_BUTTON_DOWN, // step up button down image
		113,169, //STEP_UP_BUTTON x,y
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_ON_UP, // search button on up image
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_ON_DOWN, // search button on down image
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_OFF_UP, // search button off up image
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_OFF_DOWN, // search button off down image
		66,48, //SEARCH_BUTTON x,y
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_UP, // power button on up image
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_DOWN, // power button on down image
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_OFF_UP, // power button off up image
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_OFF_DOWN, // power button off down image
		84,166, //POWER_BUTTON x,y
		FMRDO_BUTTON_SEARCH, FMRDO_BUTTON_POWER, FMRDO_BUTTON_STEP_DOWN, FMRDO_BUTTON_STEP_UP, FMRDO_BUTTON_POWER, // up, down, left, right key mapping

#ifdef __MMI_TOUCH_SCREEN__
		109,187, //VOLUME_DECREASE x,y
		IMG_ID_FMRDO_SKIN_1_VOLUME_DEC_UP,// volume_dec_up_image_id //image of  volume_dec and volume_inc is confused
		IMG_ID_FMRDO_SKIN_1_VOLUME_DEC_DOWN,// volume_dec_down_image_id
		64,187, //VOLUME_INCREASE x,y
		IMG_ID_FMRDO_SKIN_1_VOLUME_INC_UP,// volume_inc_up_image_id
		IMG_ID_FMRDO_SKIN_1_VOLUME_INC_DOWN,// volume_inc_down_image_id		
		22,165, //SETTINGS_BUTTON x,y
		IMG_ID_FMRDO_SKIN_1_SETTING_UP, // setting_up_image_id
		IMG_ID_FMRDO_SKIN_1_SETTING_DOWN, // setting_down_image_id
		{(const fmrdo_point_struct *)skin1_volume_inc_points, 4}, // volume_inc_area
		{(const fmrdo_point_struct *)skin1_volume_dec_points, 4}, // volume_dec_area
		{(const fmrdo_point_struct *)skin1_step_down_button_points, 4}, // step_down_button_area
		{(const fmrdo_point_struct *)skin1_step_up_button_points, 4}, // step_up_button_area
		{(const fmrdo_point_struct *)skin1_power_button_points, 4}, // power_button_area
		{(const fmrdo_point_struct *)skin1_search_button_points, 4}, // search_button_area
		{(const fmrdo_point_struct *)skin1_setting_points, 4}, // setting_area
		{(const fmrdo_point_struct *)skin1_frequency_points, 4} // frequency_area
#endif
#if defined(__MMI_FM_RADIO_RECORD__)
		,IMG_ID_FMRDO_SKIN_1_REC,	// record indication icon image
		IMG_ID_FMRDO_SKIN_1_REC_PAUSE,	//record pause indication icon image
		137,175, //RECORD_BUTTON x,y
		IMG_ID_FMRDO_SKIN_1_NUM_0, // record duration time icon. base is 0, 1 is base plus 1, 2 is base plus 2, ...
		IMG_ID_FMRDO_SKIN_1_NUM_COL, // record duration seperator icon
		139,49, //RECORD_DURATION x,y
#endif	
	},
#ifndef __MMI_FM_SLIM_SKIN__

	{ // skin 2
		0, 0, 0, // LSK text border color
		255, 0, 255, // LSK text color
		0, 0, 0, // RSK text border color
		255, 0, 255, // RSK text color
		IMG_ID_FMRDO_SKIN_2_BACKGROUND, // background image
		0, 0, 0, // channel name text border color
		70, 250, 205, // channel name text color
		2, 2, // channel name x, y
		172, 20, // channel name w, h
		20, 81, 37, // frequency text on color
		150, 150, 150, // frequency text off color
		56, 56, // frequency x, y
		70, 12, // frequency w, h
		IMG_ID_FMRDO_SKIN_2_VOLUME, // volume image
		0, // volume bar type -> 0: horizontal, 1: vertical
		37, 114, // volume bar x, y
		9, 0, // volume indicator start gap, end gap
		IMG_ID_FMRDO_SKIN_2_TUNER_SCALE, // tuner scale background image
		IMG_ID_FMRDO_SKIN_2_TUNER_SCALE_INDICATOR, // tuner scale indicator image
		0, // tuner scale type -> // 0: horizontal, 1: vertical
		29, 78, // tuner scale x, y
		7, // tuner scale ind offset
		0, 0, // tuner scale start gap, end gap
		IMG_ID_FMRDO_SKIN_2_STEP_DOWN_BUTTON_UP, // step down button up image
		IMG_ID_FMRDO_SKIN_2_STEP_DOWN_BUTTON_DOWN, // step down button down image
		46, 159, // step down button x, y
		IMG_ID_FMRDO_SKIN_2_STEP_UP_BUTTON_UP, // step up button up image
		IMG_ID_FMRDO_SKIN_2_STEP_UP_BUTTON_DOWN, // step up button down image
		72, 159, // step up button x, y
		IMG_ID_FMRDO_SKIN_2_SEARCH_BUTTON_ON_UP, // search button on up image
		IMG_ID_FMRDO_SKIN_2_SEARCH_BUTTON_ON_DOWN, // search button on down image
		IMG_ID_FMRDO_SKIN_2_SEARCH_BUTTON_OFF_UP, // search button off up image
		IMG_ID_FMRDO_SKIN_2_SEARCH_BUTTON_OFF_DOWN, // search button off down image
		100, 159, // search button x, y
		IMG_ID_FMRDO_SKIN_2_POWER_BUTTON_ON_UP, // power button on up image
		IMG_ID_FMRDO_SKIN_2_POWER_BUTTON_ON_DOWN, // power button on down image
		IMG_ID_FMRDO_SKIN_2_POWER_BUTTON_OFF_UP, // power button off up image
		IMG_ID_FMRDO_SKIN_2_POWER_BUTTON_OFF_DOWN, // power button off down image
		129, 159, // power button x, y
		FMRDO_BUTTON_SEARCH, FMRDO_BUTTON_POWER, FMRDO_BUTTON_STEP_DOWN, FMRDO_BUTTON_STEP_UP,FMRDO_BUTTON_POWER // up, down, left, right key mapping

#ifdef __MMI_TOUCH_SCREEN__
		,58,131,// volume_inc_x, volume_inc_y,
		IMG_ID_FMRDO_SKIN_2_VOLUME_INC_UP,// volume_inc_up_image_id
		IMG_ID_FMRDO_SKIN_2_VOLUME_INC_DOWN,// volume_inc_down_image_id
		29,131,// volume_dec_x, volume_dec_y,
		IMG_ID_FMRDO_SKIN_2_VOLUME_DEC_UP,// volume_dec_up_image_id
		IMG_ID_FMRDO_SKIN_2_VOLUME_DEC_DOWN,// volume_dec_down_image_id
		24,159,// setting_x, setting_y
		IMG_ID_FMRDO_SKIN_2_SETTING_UP, // setting_up_image_id
		IMG_ID_FMRDO_SKIN_2_SETTING_DOWN, // setting_down_image_id
		{(const fmrdo_point_struct *)skin2_volume_inc_points, 4}, // volume_inc_area
		{(const fmrdo_point_struct *)skin2_volume_dec_points, 4}, // volume_dec_area
		{(const fmrdo_point_struct *)skin2_step_down_button_points, 4}, // step_down_button_area
		{(const fmrdo_point_struct *)skin2_step_up_button_points, 4}, // step_up_button_area
		{(const fmrdo_point_struct *)skin2_power_button_points, 4}, // power_button_area
		{(const fmrdo_point_struct *)skin2_search_button_points, 4}, // search_button_area
		{(const fmrdo_point_struct *)skin2_setting_points, 4}, // setting_area
		{(const fmrdo_point_struct *)skin2_frequency_points, 4} // frequency_area
#endif
#if defined(__MMI_FM_RADIO_RECORD__)
		,IMG_ID_FMRDO_SKIN_2_REC,	// record indication icon image
		IMG_ID_FMRDO_SKIN_2_REC_PAUSE,	//record pause indication icon image
		99,131,	// record icon x, y
		IMG_ID_FMRDO_SKIN_2_NUM_0, // record duration time icon. base is 0, 1 is base plus 1, 2 is base plus 2, ...
		IMG_ID_FMRDO_SKIN_2_NUM_COL, // record duration seperator icon
		96,113	// recprd duration icon x, y
#endif	
	} ,

	{ // skin 3
		255, 255, 255, // LSK text border color
		213, 116, 146, // LSK text color
		255, 255, 255, // RSK text border color
		213, 116, 146, // RSK text color
		IMG_ID_FMRDO_SKIN_3_BACKGROUND, // background image
		90, 90, 90, // channel name text border color
		255, 255, 255, // channel name text color
		41, 10, // channel name x, y
		93, 20, // channel name w, h
		12, 52, 18, // frequency text on color
		150, 150, 150, // frequency text off color
		58, 51, // frequency x, y
		70, 12, // frequency w, h
		IMG_ID_FMRDO_SKIN_3_VOLUME, // volume image
		0, // volume bar type -> 0: horizontal, 1: vertical
		129, 109, // x, y
		9, 0, // volume indicator start gap, end gap
		IMG_ID_FMRDO_SKIN_3_TUNER_SCALE, // tuner scale background image
		IMG_ID_FMRDO_SKIN_3_TUNER_SCALE_INDICATOR, // tuner scale indicator image
		0, // tuner scale type -> // 0: horizontal, 1: vertical
		26, 66, // tuner scale x, y
		5, // tuner scale ind offset
		0, 0, // tuner scale start gap, end gap
		IMG_ID_FMRDO_SKIN_3_STEP_DOWN_BUTTON_UP, // step down button up image
		IMG_ID_FMRDO_SKIN_3_STEP_DOWN_BUTTON_DOWN, // step down button down image
		34, 142, // step down button x, y
		IMG_ID_FMRDO_SKIN_3_STEP_UP_BUTTON_UP, // step up button up image
		IMG_ID_FMRDO_SKIN_3_STEP_UP_BUTTON_DOWN, // step up button down image
		105, 142, // step up button x, y
		IMG_ID_FMRDO_SKIN_3_SEARCH_BUTTON_ON_UP, // search button on up image
		IMG_ID_FMRDO_SKIN_3_SEARCH_BUTTON_ON_DOWN, // search button on down image
		IMG_ID_FMRDO_SKIN_3_SEARCH_BUTTON_OFF_UP, // search button off up image
		IMG_ID_FMRDO_SKIN_3_SEARCH_BUTTON_OFF_DOWN, // search button off down image
		70, 130, // search button x, y
		IMG_ID_FMRDO_SKIN_3_POWER_BUTTON_ON_UP, // power button on up image
		IMG_ID_FMRDO_SKIN_3_POWER_BUTTON_ON_DOWN, // power button on down image
		IMG_ID_FMRDO_SKIN_3_POWER_BUTTON_OFF_UP, // power button off up image
		IMG_ID_FMRDO_SKIN_3_POWER_BUTTON_OFF_DOWN, // power button off down image
		70, 163, // power button x, y
		FMRDO_BUTTON_SEARCH, FMRDO_BUTTON_POWER, FMRDO_BUTTON_STEP_DOWN, FMRDO_BUTTON_STEP_UP,FMRDO_BUTTON_POWER // up, down, left, right key mapping

#ifdef __MMI_TOUCH_SCREEN__
		,96,100,// volume_inc_x, volume_inc_y,
		IMG_ID_FMRDO_SKIN_3_VOLUME_INC_UP,// volume_inc_up_image_id
		IMG_ID_FMRDO_SKIN_3_VOLUME_INC_DOWN,// volume_inc_down_image_id
		95,114,// volume_dec_x, volume_dec_y,
		IMG_ID_FMRDO_SKIN_3_VOLUME_DEC_UP,// volume_dec_up_image_id
		IMG_ID_FMRDO_SKIN_3_VOLUME_DEC_DOWN,// volume_dec_down_image_id
		6,163,// setting_x, setting_y
		IMG_ID_FMRDO_SKIN_3_SETTING_UP, // setting_up_image_id
		IMG_ID_FMRDO_SKIN_3_SETTING_DOWN, // setting_down_image_id
		{(const fmrdo_point_struct *)skin3_volume_inc_points, 4}, // volume_inc_area
		{(const fmrdo_point_struct *)skin3_volume_dec_points, 4}, // volume_dec_area
		{(const fmrdo_point_struct *)skin3_step_down_button_points, 4}, // step_down_button_area
		{(const fmrdo_point_struct *)skin3_step_up_button_points, 4}, // step_up_button_area
		{(const fmrdo_point_struct *)skin3_power_button_points, 4}, // power_button_area
		{(const fmrdo_point_struct *)skin3_search_button_points, 4}, // search_button_area
		{(const fmrdo_point_struct *)skin3_setting_points, 4}, // setting_area
		{(const fmrdo_point_struct *)skin3_frequency_points, 4} // frequency_area
#endif
#if defined(__MMI_FM_RADIO_RECORD__)
		,IMG_ID_FMRDO_SKIN_3_REC,	// record indication icon image
		IMG_ID_FMRDO_SKIN_3_REC_PAUSE,	//record pause indication icon image
		6,100,	// record icon x, y
		IMG_ID_FMRDO_SKIN_3_NUM_0, // record duration time icon. base is 0, 1 is base plus 1, 2 is base plus 2, ...
		IMG_ID_FMRDO_SKIN_3_NUM_COL, // record duration seperator icon
		45,108	// recprd duration icon x, y
#endif	
	}
#endif
};
#elif ( (LCD_WIDTH == 220) && (LCD_HEIGHT == 176) )
#ifdef __MMI_TOUCH_SCREEN__
static const fmrdo_point_struct skin1_volume_inc_points[] = {{0,0},{35,0},{35,20},{0,20}};
static const fmrdo_point_struct skin1_volume_dec_points[] = {{0,0},{35,0},{35,20},{0,20}};
static const fmrdo_point_struct skin1_step_down_button_points[] = {{0,0},{20,0},{20,60},{0,60}};
static const fmrdo_point_struct skin1_step_up_button_points[] = {{0,0},{20,0},{20,60},{0,60}};
static const fmrdo_point_struct skin1_power_button_points[] = {{0,0},{60,0},{60,20},{0,20}};
static const fmrdo_point_struct skin1_search_button_points[] ={{0,0},{60,0},{60,20},{0,20}};
static const fmrdo_point_struct skin1_setting_points[] = {{0,0},{31,0},{31,31},{0,31}};
static const fmrdo_point_struct skin1_frequency_points[] = {{0,0},{0,0},{0,0},{0,0}};


static const fmrdo_point_struct skin2_volume_inc_points[] = {{0,0},{25,0},{25,13},{0,13}};
static const fmrdo_point_struct skin2_volume_dec_points[] = {{0,0},{25,0},{25,13},{0,13}};
static const fmrdo_point_struct skin2_step_down_button_points[] = {{0,0},{25,0},{25,28},{0,28}};
static const fmrdo_point_struct skin2_step_up_button_points[] = {{0,0},{25,0},{25,28},{0,28}};
static const fmrdo_point_struct skin2_power_button_points[] = {{0,0},{25,0},{25,28},{0,28}};
static const fmrdo_point_struct skin2_search_button_points[] = {{0,0},{25,0},{25,28},{0,28}};
static const fmrdo_point_struct skin2_setting_points[] = {{0,0},{25,0},{25,28},{0,28}};
static const fmrdo_point_struct skin2_frequency_points[] = {{0,0},{69,0},{69,11},{0,11}};

static const fmrdo_point_struct skin3_volume_inc_points[] = {{1,0},{24,0},{24,11},{1,11}};
static const fmrdo_point_struct skin3_volume_dec_points[] = {{1,0},{24,0},{24,11},{1,11}};
static const fmrdo_point_struct skin3_step_down_button_points[] = {{0,0},{34,0},{34,34},{0,34}};
static const fmrdo_point_struct skin3_step_up_button_points[] = {{0,0},{34,0},{34,34},{0,34}};
static const fmrdo_point_struct skin3_power_button_points[] = {{0,0},{34,0},{34,34},{0,34}};
static const fmrdo_point_struct skin3_search_button_points[] = {{0,0},{34,0},{34,34},{0,34}};
static const fmrdo_point_struct skin3_setting_points[] = {{0,0},{34,0},{34,34},{0,34}};
static const fmrdo_point_struct skin3_frequency_points[] = {{0,0},{69,0},{69,11},{0,11}};
#endif
const mmi_fmrdo_skin_struct g_fmrdo_skins[] =
{
	{ // skin 1
		255, 255, 255, // LSK text border color
		255, 255, 255, // LSK text color
		255, 255, 255, // RSK text border color
		255, 255, 255, // RSK text color
		IMG_ID_FMRDO_SKIN_1_BACKGROUND, // background image
		0, 0, 0, // channel name text border color
		70, 250, 205, // channel name text color
		150,70, //CHANNEL_NAME x, y
		96,18, //CHANNEL_NAME w, h
		20, 81, 37, // frequency text on color
		150, 150, 150, // frequency text off color
		38,70, //FREQUENCY x, y
		50,18, //FREQUENCY w, h
		IMG_ID_FMRDO_SKIN_1_VOLUME_L_0, // volume image
		0, // volume bar type -> 0: horizontal, 1: vertical
		11,52, //VOLUME_BAR x,y12,35/34
		9,0,//VOLUME_BAR indicator start gap, end gap
		IMG_ID_FMRDO_SKIN_1_TUNER_SCALE, // tuner scale background image
		IMG_ID_FMRDO_SKIN_1_TUNER_SCALE_INDICATOR, // tuner scale indicator image
		0, // tuner scale type -> // 0: horizontal, 1: vertical
		37,123, //TUNER_SCALE x,y
		18,//ind offset
		19,29,//TUNER_SCALE start gap, end gap
		IMG_ID_FMRDO_SKIN_1_FM, // step down button up image
		IMG_ID_FMRDO_SKIN_1_FM, // step down button down image
		56,186, //STEP_DOWN_BUTTON x,y
		IMG_ID_FMRDO_SKIN_1_FM, // step up button up image
		IMG_ID_FMRDO_SKIN_1_FM, // step up button down image
		113,187, //STEP_UP_BUTTON x,y
		IMG_ID_FMRDO_SKIN_1_FM, // search button on up image
		IMG_ID_FMRDO_SKIN_1_FM, // search button on down image
		IMG_ID_FMRDO_SKIN_1_FM, // search button off up image
		IMG_ID_FMRDO_SKIN_1_FM, // search button off down image
		59,20, //SEARCH_BUTTON x,y//56,4
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_UP, // power button on up image
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_DOWN, // power button on down image
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_OFF_UP, // power button off up image
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_OFF_DOWN, // power button off down image
		87,63, //POWER_BUTTON x,y
		FMRDO_BUTTON_SEARCH, FMRDO_BUTTON_POWER, FMRDO_BUTTON_STEP_DOWN, FMRDO_BUTTON_STEP_UP, FMRDO_BUTTON_POWER, // up, down, left, right key mapping

	}

};

#elif ( (LCD_WIDTH == 240) && (LCD_HEIGHT == 320) )
#ifdef __MMI_TOUCH_SCREEN__
static const fmrdo_point_struct skin1_volume_inc_points[] = {{85,168},{105,168},{85,190},{105,190}};
static const fmrdo_point_struct skin1_volume_dec_points[] = {{29,168},{49,168},{29,190},{49,190}};
static const fmrdo_point_struct skin1_step_down_button_points[] = {{120,232},{146,232},{120,260},{146,260}};
static const fmrdo_point_struct skin1_step_up_button_points[] = {{153,232},{179,232},{153,260},{179,260}};
static const fmrdo_point_struct skin1_power_button_points[] = {{182,232},{210,232},{182,260},{210,260}};
static const fmrdo_point_struct skin1_search_button_points[] = {{25,238},{42,238},{25,256},{42,256}};
static const fmrdo_point_struct skin1_setting_points[] = {{85,232},{115,232},{85,260},{115,260}};
static const fmrdo_point_struct skin1_frequency_points[] = {{0,0},{69,0},{69,11},{0,11}};
static const fmrdo_point_struct skin1_record_button_points[] = {{0,0},{35,0},{35,35},{0,35}};

static const fmrdo_point_struct skin2_volume_inc_points[] = {{1,0},{12,0},{12,18},{1,18}};
static const fmrdo_point_struct skin2_volume_dec_points[] = {{0,0},{11,0},{11,18},{0,18}};
static const fmrdo_point_struct skin2_step_down_button_points[] = {{5,4},{32,4},{32,60},{5,50}};
static const fmrdo_point_struct skin2_step_up_button_points[] = {{5,4},{32,4},{32,60},{5,50}};
static const fmrdo_point_struct skin2_power_button_points[] = {{5,4},{32,4},{32,60},{5,50}};
static const fmrdo_point_struct skin2_search_button_points[] = {{5,4},{32,4},{32,60},{5,50}};
static const fmrdo_point_struct skin2_setting_points[] = {{1,1},{25,1},{25,27},{1,27}};
static const fmrdo_point_struct skin2_frequency_points[] = {{0,0},{69,0},{69,11},{0,11}};

static const fmrdo_point_struct skin3_volume_inc_points[] = {{1,0},{12,0},{12,18},{1,18}};
static const fmrdo_point_struct skin3_volume_dec_points[] = {{0,0},{11,0},{11,18},{0,18}};
static const fmrdo_point_struct skin3_step_down_button_points[] = {{12,7},{38,7},{43,12},{43,36},{38,41},{12,41},{7,36},{7,12}};
static const fmrdo_point_struct skin3_step_up_button_points[] = {{12,7},{38,7},{43,12},{43,36},{38,41},{12,41},{7,36},{7,12}};
static const fmrdo_point_struct skin3_power_button_points[] = {{12,7},{38,7},{43,12},{43,36},{38,41},{12,41},{7,36},{7,12}};
static const fmrdo_point_struct skin3_search_button_points[] = {{12,7},{38,7},{43,12},{43,36},{38,41},{12,41},{7,36},{7,12}};
static const fmrdo_point_struct skin3_setting_points[] = {{1,1},{25,1},{25,27},{1,27}};
static const fmrdo_point_struct skin3_frequency_points[] = {{0,0},{70,0},{70,12},{0,12}};
#endif

const mmi_fmrdo_skin_struct g_fmrdo_skins[] =
{
	{ // skin 1
		0, 0, 0, // LSK text border color
		255, 255, 0, // LSK text color
		0, 0, 0, // RSK text border color
		255, 255, 0, // RSK text color
		IMG_ID_FMRDO_SKIN_1_BACKGROUND, // background image
		0, 0, 0, // channel name text border color
		70, 250, 205, // channel name text color
		54, 12, // channel name x, y
		100, 20, // channel name w, h
		255, 255, 255, // frequency text on color
		150, 150, 150, // frequency text off color
		140, 174, // frequency x, y
		70, 14, // frequency w, h
		IMG_ID_FMRDO_SKIN_1_VOLUME, // volume image
		0, // volume bar type -> 0: horizontal, 1: vertical
		53, 173, // volume bar x, y
		9, 0, // volume indicator start gap, end gap
		IMG_ID_FMRDO_SKIN_1_TUNER_SCALE, // tuner scale background image
		IMG_ID_FMRDO_SKIN_1_TUNER_SCALE_INDICATOR, // tuner scale indicator image
		0, // tuner scale type -> // 0: horizontal, 1: vertical
		46, 48, // tuner scale x, y
		32, // tuner scale ind offset
		4, 4, // tuner scale start gap, end gap
		IMG_ID_FMRDO_SKIN_1_STEP_DOWN_BUTTON_UP, // step down button up image
		IMG_ID_FMRDO_SKIN_1_STEP_DOWN_BUTTON_DOWN, // step down button down image
		118, 232, // step down button x, y
		IMG_ID_FMRDO_SKIN_1_STEP_UP_BUTTON_UP, // step up button up image
		IMG_ID_FMRDO_SKIN_1_STEP_UP_BUTTON_DOWN, // step up button down image
		150, 233, // step up button x, y
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_ON_UP, // search button on up image
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_ON_DOWN, // search button on down image
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_OFF_UP, // search button off up image
		IMG_ID_FMRDO_SKIN_1_SEARCH_BUTTON_OFF_DOWN, // search button off down image
		23, 236, // search button x, y
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_UP, // power button on up image
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_ON_DOWN, // power button on down image
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_OFF_UP, // power button off up image
		IMG_ID_FMRDO_SKIN_1_POWER_BUTTON_OFF_DOWN, // power button off down image
		182, 232, // power button x, y
			FMRDO_BUTTON_SEARCH, FMRDO_BUTTON_SETTINGS, FMRDO_BUTTON_STEP_DOWN, FMRDO_BUTTON_STEP_UP,FMRDO_BUTTON_POWER // up, down, left, right key mapping 	
#ifdef __MMI_FM_RADIO_ANIMATION__
			,42,103,// animation backgroup x, y
			IMG_ID_FMRDO_SKIN_1_ANIMATION// animation backgroup image
#endif
#ifdef __MMI_TOUCH_SCREEN__
			,85,168,// volume_inc_x, volume_inc_y,modify gdm 080515
		IMG_ID_FMRDO_SKIN_1_VOLUME_INC_UP,// volume_inc_up_image_id
		IMG_ID_FMRDO_SKIN_1_VOLUME_INC_DOWN,// volume_inc_down_image_id
		29,168,// volume_dec_x, volume_dec_y,
		IMG_ID_FMRDO_SKIN_1_VOLUME_DEC_UP,// volume_dec_up_image_id
		IMG_ID_FMRDO_SKIN_1_VOLUME_DEC_DOWN,// volume_dec_down_image_id
		85,232,// setting_x, setting_y
		IMG_ID_FMRDO_SKIN_1_SETTING_UP, // setting_up_image_id
		IMG_ID_FMRDO_SKIN_1_SETTING_DOWN, // setting_down_image_id
		{(const fmrdo_point_struct *)skin1_volume_inc_points, 4}, // volume_inc_area
		{(const fmrdo_point_struct *)skin1_volume_dec_points, 4}, // volume_dec_area
		{(const fmrdo_point_struct *)skin1_step_down_button_points, 4}, // step_down_button_area
		{(const fmrdo_point_struct *)skin1_step_up_button_points, 4}, // step_up_button_area
			{(const fmrdo_point_struct *)skin1_power_button_points, 4}, // power_button_area
#if defined(__MMI_FM_RADIO_RECORD__)			
			{(const fmrdo_point_struct *)skin1_record_button_points, 4}, // record_area
#endif
			{(const fmrdo_point_struct *)skin1_search_button_points, 4}, // search_button_area
			{(const fmrdo_point_struct *)skin1_setting_points, 4}, // setting_area
			{(const fmrdo_point_struct *)skin1_frequency_points, 4} // frequency_area
//add gdm 080515

#endif
#if defined(__MMI_FM_RADIO_RECORD__)
			,IMG_ID_FMRDO_SKIN_1_REC,	// record indication icon image
			IMG_ID_FMRDO_SKIN_1_REC_PAUSE,	//record pause indication icon image
		10,206,	// record icon x, y
			IMG_ID_FMRDO_SKIN_1_NUM_0, // record duration time icon. base is 0, 1 is base plus 1, 2 is base plus 2, ...
			IMG_ID_FMRDO_SKIN_1_NUM_COL, // record duration seperator icon
		126,210	// recprd duration icon x, y
#endif	
	},
	{ // skin 2
		0, 0, 0, // LSK text border color
		255, 0, 255, // LSK text color
		0, 0, 0, // RSK text border color
		255, 0, 255, // RSK text color
		IMG_ID_FMRDO_SKIN_2_BACKGROUND, // background image
		0, 0, 0, // channel name text border color
		70, 250, 205, // channel name text color
		20, 2, // channel name x, y
		200, 26, // channel name w, h
		255, 255, 255, // frequency text on color
		150, 150, 150, // frequency text off color
		85, 120, // frequency x, y
		70, 14, // frequency w, h
		IMG_ID_FMRDO_SKIN_2_VOLUME, // volume image
		0, // volume bar type -> 0: horizontal, 1: vertical
		67, 196, // volume bar x, y
		5, 0, // volume indicator start gap, end gap
		IMG_ID_FMRDO_SKIN_2_TUNER_SCALE, // tuner scale background image
		IMG_ID_FMRDO_SKIN_2_TUNER_SCALE_INDICATOR, // tuner scale indicator image
		0, // tuner scale type -> // 0: horizontal, 1: vertical
		40, 47, // tuner scale x, y
		35, // tuner scale ind offset
		11, 14, // tuner scale start gap, end gap
		IMG_ID_FMRDO_SKIN_2_STEP_DOWN_BUTTON_UP, // step down button up image
		IMG_ID_FMRDO_SKIN_2_STEP_DOWN_BUTTON_DOWN, // step down button down image
		134, 230, // step down button x, y
		IMG_ID_FMRDO_SKIN_2_STEP_UP_BUTTON_UP, // step up button up image
		IMG_ID_FMRDO_SKIN_2_STEP_UP_BUTTON_DOWN, // step up button down image
		169, 230, // step up button x, y
		IMG_ID_FMRDO_SKIN_2_SEARCH_BUTTON_ON_UP, // search button on up image
		IMG_ID_FMRDO_SKIN_2_SEARCH_BUTTON_ON_DOWN, // search button on down image
		IMG_ID_FMRDO_SKIN_2_SEARCH_BUTTON_OFF_UP, // search button off up image
		IMG_ID_FMRDO_SKIN_2_SEARCH_BUTTON_OFF_DOWN, // search button off down image
		97, 230, // search button x, y
		IMG_ID_FMRDO_SKIN_2_POWER_BUTTON_ON_UP, // power button on up image
		IMG_ID_FMRDO_SKIN_2_POWER_BUTTON_ON_DOWN, // power button on down image
		IMG_ID_FMRDO_SKIN_2_POWER_BUTTON_OFF_UP, // power button off up image
		IMG_ID_FMRDO_SKIN_2_POWER_BUTTON_OFF_DOWN, // power button off down image
		61, 230, // power button x, y
		FMRDO_BUTTON_SEARCH, FMRDO_BUTTON_POWER, FMRDO_BUTTON_STEP_DOWN, FMRDO_BUTTON_STEP_UP, FMRDO_BUTTON_POWER // up, down, left, right key mapping		
#ifdef __MMI_FM_RADIO_ANIMATION__
		,40,112,// animation backgroup x, y
		IMG_ID_FMRDO_SKIN_2_ANIMATION// animation backgroup image
#endif
#ifdef __MMI_TOUCH_SCREEN__
		,100,194,// volume_inc_x, volume_inc_y,
		IMG_ID_FMRDO_SKIN_2_VOLUME_INC_UP,// volume_inc_up_image_id
		IMG_ID_FMRDO_SKIN_2_VOLUME_INC_DOWN,// volume_inc_down_image_id
		48,194,// volume_dec_x, volume_dec_y,
		IMG_ID_FMRDO_SKIN_2_VOLUME_DEC_UP,// volume_dec_up_image_id
		IMG_ID_FMRDO_SKIN_2_VOLUME_DEC_DOWN,// volume_dec_down_image_id
		35,234,// setting_x, setting_y
		IMG_ID_FMRDO_SKIN_2_SETTING_UP, // setting_up_image_id
		IMG_ID_FMRDO_SKIN_2_SETTING_DOWN, // setting_down_image_id
		{(const fmrdo_point_struct *)skin2_volume_inc_points, 4}, // volume_inc_area
		{(const fmrdo_point_struct *)skin2_volume_dec_points, 4}, // volume_dec_area
		{(const fmrdo_point_struct *)skin2_step_down_button_points, 4}, // step_down_button_area
		{(const fmrdo_point_struct *)skin2_step_up_button_points, 4}, // step_up_button_area
		{(const fmrdo_point_struct *)skin2_power_button_points, 4}, // power_button_area
		#if defined(__MMI_FM_RADIO_RECORD__)			
			{(const fmrdo_point_struct *)skin1_record_button_points, 4}, // record_area
#endif
		{(const fmrdo_point_struct *)skin2_search_button_points, 4}, // search_button_area
		{(const fmrdo_point_struct *)skin2_setting_points, 4}, // setting_area
		{(const fmrdo_point_struct *)skin2_frequency_points, 4} // frequency_area
#endif
#if defined(__MMI_FM_RADIO_RECORD__)
		,IMG_ID_FMRDO_SKIN_2_REC,	// record indication icon image
		IMG_ID_FMRDO_SKIN_2_REC_PAUSE,	//record pause indication icon image
		177,193,	// record icon x, y
		IMG_ID_FMRDO_SKIN_2_NUM_0, // record duration time icon. base is 0, 1 is base plus 1, 2 is base plus 2, ...
		IMG_ID_FMRDO_SKIN_2_NUM_COL, // record duration seperator icon
		127,198	// recprd duration icon x, y
#endif	
	} ,
	{ // skin 3
		255, 255, 255, // LSK text border color
		213, 116, 146, // LSK text color
		255, 255, 255, // RSK text border color
		213, 116, 146, // RSK text color
		IMG_ID_FMRDO_SKIN_3_BACKGROUND, // background image
		90, 90, 90, // channel name text border color
		255, 255, 255, // channel name text color
		20, 26, // channel name x, y
		200, 20, // channel name w, h
		255, 255, 255, // frequency text on color
		150, 150, 150, // frequency text off color
		85, 62, // frequency x, y
		70, 14, // frequency w, h
		IMG_ID_FMRDO_SKIN_3_VOLUME, // volume image
		0, // volume bar type -> 0: horizontal, 1: vertical
		172, 205, // x, y
		5, 0, // volume indicator start gap, end gap
		IMG_ID_FMRDO_SKIN_3_TUNER_SCALE, // tuner scale background image
		IMG_ID_FMRDO_SKIN_3_TUNER_SCALE_INDICATOR, // tuner scale indicator image
		0, // tuner scale type -> // 0: horizontal, 1: vertical
		51, 123, // tuner scale x, y
		32, // tuner scale ind offset
		6, 6, // tuner scale start gap, end gap
		IMG_ID_FMRDO_SKIN_3_STEP_DOWN_BUTTON_UP, // step down button up image
		IMG_ID_FMRDO_SKIN_3_STEP_DOWN_BUTTON_DOWN, // step down button down image
		45, 234, // step down button x, y
		IMG_ID_FMRDO_SKIN_3_STEP_UP_BUTTON_UP, // step up button up image
		IMG_ID_FMRDO_SKIN_3_STEP_UP_BUTTON_DOWN, // step up button down image
		92, 234, // step up button x, y
		IMG_ID_FMRDO_SKIN_3_SEARCH_BUTTON_ON_UP, // search button on up image
		IMG_ID_FMRDO_SKIN_3_SEARCH_BUTTON_ON_DOWN, // search button on down image
		IMG_ID_FMRDO_SKIN_3_SEARCH_BUTTON_OFF_UP, // search button off up image
		IMG_ID_FMRDO_SKIN_3_SEARCH_BUTTON_OFF_DOWN, // search button off down image
		92, 191, // search button x, y
		IMG_ID_FMRDO_SKIN_3_POWER_BUTTON_ON_UP, // power button on up image
		IMG_ID_FMRDO_SKIN_3_POWER_BUTTON_ON_DOWN, // power button on down image
		IMG_ID_FMRDO_SKIN_3_POWER_BUTTON_OFF_UP, // power button off up image
		IMG_ID_FMRDO_SKIN_3_POWER_BUTTON_OFF_DOWN, // power button off down image
		45, 191, // power button x, y
		FMRDO_BUTTON_SEARCH, FMRDO_BUTTON_POWER, FMRDO_BUTTON_STEP_DOWN, FMRDO_BUTTON_STEP_UP,FMRDO_BUTTON_POWER  // up, down, left, right key mapping		
#ifdef __MMI_FM_RADIO_ANIMATION__
		,0,61,// animation backgroup x, y
		IMG_ID_FMRDO_SKIN_3_ANIMATION// animation backgroup image
#endif		
#ifdef __MMI_TOUCH_SCREEN__
		,213,201,// volume_inc_x, volume_inc_y,
		IMG_ID_FMRDO_SKIN_3_VOLUME_INC_UP,// volume_inc_up_image_id
		IMG_ID_FMRDO_SKIN_3_VOLUME_INC_DOWN,// volume_inc_down_image_id
		146,201,// volume_dec_x, volume_dec_y,
		IMG_ID_FMRDO_SKIN_3_VOLUME_DEC_UP,// volume_dec_up_image_id
		IMG_ID_FMRDO_SKIN_3_VOLUME_DEC_DOWN,// volume_dec_down_image_id
		9,191,// setting_x, setting_y
		IMG_ID_FMRDO_SKIN_3_SETTING_UP, // setting_up_image_id
		IMG_ID_FMRDO_SKIN_3_SETTING_DOWN, // setting_down_image_id
		{(const fmrdo_point_struct *)skin3_volume_inc_points, 4}, // volume_inc_area
		{(const fmrdo_point_struct *)skin3_volume_dec_points, 4}, // volume_dec_area
		{(const fmrdo_point_struct *)skin3_step_down_button_points, 8}, // step_down_button_area
		{(const fmrdo_point_struct *)skin3_step_up_button_points, 8}, // step_up_button_area
		{(const fmrdo_point_struct *)skin3_power_button_points, 8}, // power_button_area
		#if defined(__MMI_FM_RADIO_RECORD__)			
			{(const fmrdo_point_struct *)skin1_record_button_points, 4}, // record_area
#endif
		{(const fmrdo_point_struct *)skin3_search_button_points, 8}, // search_button_area
		{(const fmrdo_point_struct *)skin3_setting_points, 4}, // setting_area
		{(const fmrdo_point_struct *)skin3_frequency_points, 4} // frequency_area
#endif
#if defined(__MMI_FM_RADIO_RECORD__)
		,IMG_ID_FMRDO_SKIN_3_REC,	// record indication icon image
		IMG_ID_FMRDO_SKIN_3_REC_PAUSE,	//record pause indication icon image
		205,242,	// record icon x, y
		IMG_ID_FMRDO_SKIN_3_NUM_0, // record duration time icon. base is 0, 1 is base plus 1, 2 is base plus 2, ...
		IMG_ID_FMRDO_SKIN_3_NUM_COL, // record duration seperator icon
		155,253	// recprd duration icon x, y
#endif	
	}
};
#endif
#endif // __MMI_FM_RADIO__


