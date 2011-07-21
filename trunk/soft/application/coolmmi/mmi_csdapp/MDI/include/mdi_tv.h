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
 *	 mdi_tv.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  TV related interface header file
 *
 * Author:
 * -------
 *                      
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi_CSDapp/MDI/include/mdi_tv.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 11:05:56   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:45:50   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:31:16   zhangxb
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _MDI_TV_H_
#define _MDI_TV_H_

#ifdef __MMI_TVOUT__

/***************************************************************************** 
* Include
*****************************************************************************/
#include "tv_out.h"


/***************************************************************************** 
* Define
*****************************************************************************/
/* return result */
#define MDI_RES_TV_DISABLE				5002
#define MDI_RES_TV_ENABLE				5001
#define MDI_RES_TV_SUCCEED				0
#define MDI_RES_TV_NOT_READY			-5001
#define MDI_RES_TV_FAILED				-5002
#define MDI_RES_TV_NOT_OWNER			-5003


/* tv out mode */
#ifndef MMI_ON_WIN32
	/* Hardware */
	#define MDI_TV_OUTPUT_MODE_1				TV_OUTPUT_MODE1
	#define MDI_TV_OUTPUT_MODE_2				TV_OUTPUT_MODE2
	#define MDI_TV_OUTPUT_MODE_3				TV_OUTPUT_MODE3
	#define MDI_TV_OUTPUT_MODE_4				TV_OUTPUT_MODE4
#else
	/* PC Sim */
	#define MDI_TV_OUTPUT_MODE_1				0
	#define MDI_TV_OUTPUT_MODE_2				1
	#define MDI_TV_OUTPUT_MODE_3				2
	#define MDI_TV_OUTPUT_MODE_4				3
#endif /* MMI_ON_WIN32 */

/* tv out mode */
#define MDI_TV_MODE_NONE						0
#define MDI_TV_MODE_LCD_SCREEN				1
#define MDI_TV_MODE_OWNER_DRAW				2
#define MDI_TV_MODE_CAMERA_PREVIEW			3
#define MDI_TV_MODE_VIDEO_PREVIEW			4
#define MDI_TV_MODE_VIDEO_PLAYBACK			5
#define MDI_TV_MODE_HOR_CAMERA_PREVIEW		6
#define MDI_TV_MODE_HOR_VIDEO_PREVIEW		7
#define MDI_TV_MODE_HOR_VIDEO_PLAYBACK		8


/* tv out format */
#ifndef MMI_ON_WIN32
	/* Hardware */
	#define MDI_TV_OUTPUT_FORMAT_PAL			TV_FORMAT_PAL
	#define MDI_TV_OUTPUT_FORMAT_NTSC		TV_FORMAT_NTSC
	#define MDI_TV_OUTPUT_FORMAT_PALM		TV_FORMAT_PALM
	#define MDI_TV_OUTPUT_FORMAT_PALC		TV_FORMAT_PALC
#else
	/* PC Sim */
	#define MDI_TV_OUTPUT_FORMAT_PAL			0
	#define MDI_TV_OUTPUT_FORMAT_NTSC		1
	#define MDI_TV_OUTPUT_FORMAT_PALM		2
	#define MDI_TV_OUTPUT_FORMAT_PALC		3
#endif /* MMI_ON_WIN32 */

/* owner */
#define MDI_TV_OWNER_GDI				0
#define MDI_TV_OWNER_CAMERA			1
#define MDI_TV_OWNER_VIDEO				2
#define MDI_TV_OWNER_MMI				3

typedef void (*mdi_tv_event_callback)(MDI_RESULT);

/***************************************************************************** 
* Typedef 
*****************************************************************************/
typedef struct {
	U16							format;
	U16							mode;	
	U16							owner;

	/* double buf */
	U8								act_buf_id;		/* buf id that can be used by MMI */
	PU8							buf_1_ptr;
	PU8							buf_2_ptr;
	S32							buf_width;
	S32							buf_height;
	S32							buf_size;

	BOOL							is_enable;	
	
	gdi_handle					layer_1;
	gdi_handle					layer_2;
	
	mdi_tv_event_callback	event_callback;		

} mdi_tv_cntx_struct;

/*****************************************************************************              
* Extern Global Variable                                                                    
*****************************************************************************/             

/*****************************************************************************              
* Extern Global Function                                                                    
*****************************************************************************/     
extern MDI_RESULT	mdi_tvout_init(mdi_tv_event_callback tv_event_callback);
extern MDI_RESULT mdi_tvout_set_mode(U16 mode, S32 owner_draw_width, S32 owner_draw_height);
extern MDI_RESULT	mdi_tvout_start(void);
extern MDI_RESULT	mdi_tvout_stop(void);
extern MDI_RESULT	mdi_tvout_get_para(	U16 owner, 
													U16 *mode_ptr,
													U16 *buf_width_ptr, 
													U16 *buf_height_ptr, 
													U32 *buf_size_ptr, 
													U8  **buf_1_pp, 
													U8  **buf_2_pp);
extern MDI_RESULT	mdi_tvout_blt(void);
extern MDI_RESULT	mdi_tvout_set_format(U16 format);
extern MDI_RESULT mdi_tvout_set_owner(U16 owner);

extern U16			mdi_tvout_get_owner(void);
extern BOOL			mdi_tvout_is_enable(void);
extern GDI_HANDLE mdi_tvout_get_active_layer(void);
extern MDI_RESULT mdi_tvout_resize_buffer(S32 buffer_width, S32 buffer_height);

#endif /* __MMI_TVOUT__ */
#endif /* _MDI_TV_H_ */


