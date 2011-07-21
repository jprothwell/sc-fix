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
 *   FMScheduleRecScreen.h
 *
 * Project:
 * --------
 *   Maui_sw
 *
 * Description:
 * ------------
 *  .............
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
#ifndef _MMI_FMSRSCREEN_H
#define _MMI_FMSRSCREEN_H

#include "mmi_features.h" 

#ifdef __MMI_FM_RADIO_SCHEDULE_REC__

/* if some header should be include before this one */
//#ifndef _PREINC_H
//#error "preinc.h should be included before header.h"
//#endif

/* 
** Typedef 
*/

typedef struct
{
	U16 x, y, w, h;
} fmsr_text_struct;

typedef struct
{	
	UI_image_type bg_img;

	color_t SK_text_border_color;
	color_t SK_text_color;
	UI_string_type LSK_text;
	UI_string_type RSK_text;
	
	color_t status_text_color;
	fmsr_text_struct	status;
	UI_string_type status_text;

	color_t freq_text_color;
	fmsr_text_struct	freq;
	
	color_t duration_text_color;
	fmsr_text_struct	duration;
	
	color_t date_text_color;
	fmsr_text_struct	date;
	
	color_t time_text_color;	
	fmsr_text_struct	time;

	//fmsr_text_struct err_msg;
}fmsr_rec_screen_struct;


typedef struct
{
	U16 bg_image_id;
	U8 SK_text_border_color_r, SK_text_border_color_g, SK_text_border_color_b;
	U8 SK_text_color_r, SK_text_color_g, SK_text_color_b;
	U8 status_text_color_r, status_text_color_g,status_text_color_b;
	U16 status_text_y;
	U8 freq_text_color_r, freq_text_color_g, freq_text_color_b;
	U16 freq_text_y;
	U8 duration_text_color_r, duration_text_color_g, duration_text_color_b;
	U16 duration_text_y;	
	U8 date_text_color_r, date_text_color_g, date_text_color_b;
	U16 date_text_y;
	U8 time_text_color_r, time_text_color_g, time_text_color_b;	
	U16 time_text_y;

#if 0 //def __MMI_TOUCH_SCREEN__
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

	U8 up_key_mapping, down_key_mapping, left_key_mapping, right_key_mapping;
} fmsr_skin_struct;

/* 
** Define
*/



/* 
** Extern Global Variable
*/

/* 
** Extern Global Function
*/

	
#endif /* #ifdef __MMI_FM_RADIO_SCHEDULE_REC__ */
#endif /* #ifndef _MMI_FMSRSCREEN_H */
