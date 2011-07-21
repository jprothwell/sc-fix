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
 *	 PhotoEditor.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  PhotoEditor Header
 *
 * Author:
 * -------
 * -------
 *                      
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _PHOTOEDITOR_SCR_H_
#define _PHOTOEDITOR_SCR_H_


/***************************************************************************** 
* Screen ID
*****************************************************************************/
typedef enum {

	/************* Photo Editor **************/
	SCR_ID_PHOEDT_APP = PHOEDT_BASE+1,


	
	/************* Photo Art **************/
	SCR_ID_PHOART_SELECT_FILE,
	SCR_ID_PHOART_CLIP_OPTION,
	SCR_ID_PHOART_CLIP,
	SCR_ID_PHOART_MAIN,
	SCR_ID_PHOART_OPTION,
	SCR_ID_PHOART_ADJUST_OPTION,
	SCR_ID_PHOART_EDITOR_FILENAME,
	SCR_ID_PHOART_EDITOR_OPTION,
	SCR_ID_PHOART_EFFECT_SELECT,
	SCR_ID_PHOART_EFFECT_EDIT,
	SCR_ID_PHOART_FRAME_SELECT,
	SCR_ID_PHOART_FRAME_EDIT,
	SCR_ID_PHOART_ICON_SELECT,
	SCR_ID_PHOART_ICON_EDIT,
	SCR_ID_PHOART_TEXT_STR_EDITOR,
	SCR_ID_PHOART_TEXT_STR_EDITOR_OPTION,
	SCR_ID_PHOART_TEXT_SELECT,
	SCR_ID_PHOART_TEXT_EDIT,
	SCR_ID_PHOART_ADJUST_EDIT,
	SCR_ID_PHOART_FLIP_EDIT,
	SCR_ID_PHOART_HELP
	/************* Scene Master **************/
	
	/************* Image Tile **************/	
	
	/************* Slide Show **************/	

	
} phoedt_scr_id_enum;


#endif /* _PHOTOEDITOR_SCR_H_ */


