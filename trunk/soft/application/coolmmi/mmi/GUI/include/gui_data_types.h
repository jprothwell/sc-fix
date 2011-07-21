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
 *   gui_data_types.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *   Basic data types
 * Author:
 * -------
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**********************************************************************************
	Filename:		gui_data_types.h
	Date Created:	December-24-2002
	Contains:		UI data types
					Includes standard and external data types
**********************************************************************************/

#ifndef __GUI_DATA_TYPES_H__
#define __GUI_DATA_TYPES_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*	External includes					*/

#include "mmi_data_types.h"
#include "pixcomfontengine.h"
#include "fontres.h"
#include "gui_config.h"

/*	External datatypes used by the UI	*/

#define MAX_ANIMATED_IMAGES			32
#define MAX_ANIMATION_FRAMES		32
#define ANIMATED_IMAGE_FRAME_RATE	200			/*	milli-seconds	*/

#define ANIMATED_IMAGE_TRANSPARENT				0x00000001
#define ANIMATED_IMAGE_CYCLE_SELECTED_FRAMES	0x00000002

typedef struct _bitmap
{	S32		xsize;
	S32		ysize;
    S8		color_depth;
    S32		row_bytes;
    byte	*palette;
    byte	*data;
} bitmap;

typedef byte*				UI_image_type;
typedef byte*				UI_audio_type; 
typedef byte				UI_transparent_color_type;
typedef stFontAttribute*	UI_font_type;

#ifdef __UCS2_ENCODING
typedef	U16					UI_character_type;
#else
typedef U8					UI_character_type;
#endif

typedef UI_character_type*	UI_string_type;
typedef U8*					UI_buffer_type;

typedef struct _animated_image
{	S16				n_frames;
    S16				frame_rate;
    S16				frame_counter;
	S32				x;
	S32				y;
	UI_image_type	images[MAX_ANIMATION_FRAMES];
    void			(*hide_function)(S32 x1,S32 y1,S32 x2,S32 y2);
    dword			flags;
	S32				clip_x1;
	S32				clip_y1;
	S32				clip_x2;
	S32				clip_y2;
	S16				start_frame;
	S16				end_frame;
	bitmap*			graphics_context;
	void			(*last_frame_function)(void);
} animated_image;

typedef animated_image*					UI_animated_image_handle;

extern	UI_font_type					UI_font;

#define UI_NULL_IMAGE					NULL
#define UI_DEFAULT_TRANSPARENT_COLOR	0
#define UI_NULL_TRANSPARENT_COLOR		255

/*	Retained for compatibility sake. Will be removed later	*/
#define UI_IMAGE_ID_NULL				NULL

#define UI_UNUSED_PARAMETER(x)			((void)(x))

#if(UI_INDEXED_COLOR_SYSTEM)

#if(UI_COLOR_DEPTH<=8)

typedef byte	color_t;

#else

typedef word	color_t;

#endif

#else

typedef struct _color
{	byte r;
	byte g;
	byte b;
	byte alpha;
} color_t;

#endif

typedef struct _polygon_vertex
{	S32		x;
	S32		y;
} polygon_vertex;

#ifdef __cplusplus
}
#endif

#endif

