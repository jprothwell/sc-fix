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
 
*
* Description:
* ------------
*   FM radio main screen skin settings
*
* Author:
* -------
 
*
*==============================================================================
* 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
*==============================================================================
*******************************************************************************/

#include "mmi_features.h"

#ifdef __MMI_FM_RADIO_SCHEDULE_REC__

#include "gdi_lcd_config.h"

// dependent headers for FMRadioDef.h"
#include "mmi_data_types.h"
#include "gui_data_types.h"

#include "fmschedulerecscreen.h"
#include "fmschedulerecdef.h"

#if ( (LCD_WIDTH == 128) && (LCD_HEIGHT == 128) )
const fmsr_skin_struct g_fmsr_skins[] =
{
	{//skin1
		IMG_ID_FMSR_BACKGROUND,	//Background Image ID
		16, 112, 168,	// Soft key RGB color
		96, 192, 248,	// Soft key border RGB color
		255,0,0,			// Status ind RGB color
		10,	// Status text y 
		255, 255, 255,	// Freq text RGB color
		41,	// Freq text y
		0, 0, 0,	// Duration text RGB color
		64,	// Dudation text y		
		56, 168, 240,	// Date text RGB color
		86,	// Date text y
		176, 176, 176,	// Time text RGB color
		100	// Time text y
	}
};
#elif ( (LCD_WIDTH == 128) && (LCD_HEIGHT == 160) )
const fmsr_skin_struct g_fmsr_skins[] =
{
	{//skin1
		IMG_ID_FMSR_BACKGROUND,	//Background Image ID
		16, 112, 168,	// Soft key RGB color
		96, 192, 248,	// Soft key border RGB color
		255,0,0,			// Status ind RGB color
		10,	// Status text y 
		255, 255, 255,	// Freq text RGB color
		41,	// Freq text y
		0, 0, 0,	// Duration text RGB color
		64,	// Dudation text y		
		56, 168, 240,	// Date text RGB color
		86,	// Date text y
		176, 176, 176,	// Time text RGB color
		100	// Time text y
	}
};
#elif ( (LCD_WIDTH == 176) && (LCD_HEIGHT == 220) )
const fmsr_skin_struct g_fmsr_skins[] =
{
	{//skin1
		IMG_ID_FMSR_BACKGROUND,	//Background Image ID
		16, 112, 168,	// Soft key RGB color
		96, 192, 248,	// Soft key border RGB color
		255,0,0,			// Status ind RGB color
		68,	// Status text y 
		255, 255, 255,	// Freq text RGB color
		100,	// Freq text y
		0, 0, 0,	// Duration text RGB color
		125,	// Dudation text y		
		56, 168, 240,	// Date text RGB color
		146,	// Date text y
		176, 176, 176,	// Time text RGB color
		161	// Time text y
	}
};
#elif ( (LCD_WIDTH == 220) && (LCD_HEIGHT == 176) )	
const fmsr_skin_struct g_fmsr_skins[] =
{
	{//skin1
		IMG_ID_FMSR_BACKGROUND,	//Background Image ID
		16, 112, 168,	// Soft key RGB color
		96, 192, 248,	// Soft key border RGB color
		255,0,0,			// Status ind RGB color
		68,	// Status text y 
		255, 255, 255,	// Freq text RGB color
		100,	// Freq text y
		0, 0, 0,	// Duration text RGB color
		125,	// Dudation text y		
		56, 168, 240,	// Date text RGB color
		146,	// Date text y
		176, 176, 176,	// Time text RGB color
		161	// Time text y
	}
};
#elif ( (LCD_WIDTH == 240) && (LCD_HEIGHT == 320) )
const fmsr_skin_struct g_fmsr_skins[] =
{
	{//skin1
		IMG_ID_FMSR_BACKGROUND,	//Background Image ID
		16, 112, 168,	// Soft key RGB color
		96, 192, 248,	// Soft key border RGB color
		255,0,0,			// Status ind RGB color
		40,	// Status text y 
		255, 255, 255,	// Freq text RGB color
		114,	// Freq text y
		0, 0, 0,	// Duration text RGB color
		145,	// Dudation text y		
		56, 168, 240,	// Date text RGB color
		171,	// Date text y
		176, 176, 176,	// Time text RGB color
		196	// Time text y
	}
};
#endif
#endif // __MMI_FM_RADIO_SCHEDULE_REC__


