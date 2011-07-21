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
 *	GameCopterDefs.h
  *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Game id define Copter.
 *
 * Author:
 * -------
 *	
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/FunAndGames/include/CopterDefs.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 10:57:56   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:38:26   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:24:32   zhangxb
 * 
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _MMI_GX_COPTER_DEFS_H
#define _MMI_GX_COPTER_DEFS_H

/***************************************************************************** 
* Typedef 
*****************************************************************************/
/* String ID */
typedef enum {
	STR_ID_GX_COPTER_GAME_NAME = GAME_COPTER_BASE + 1,

	#if defined(__MMI_TOUCH_SCREEN__)
	STR_ID_GX_COPTER_HELP_DESCRIPTION_TP,
	#else
	STR_ID_GX_COPTER_HELP_DESCRIPTION,	
	#endif
	
	STR_ID_GX_COPTER_GAME_NAME_SUBMARINE,

	#if defined(__MMI_TOUCH_SCREEN__)
	STR_ID_GX_COPTER_HELP_DESCRIPTION_SUBMARINE_TP,
	#else
	STR_ID_GX_COPTER_HELP_DESCRIPTION_SUBMARINE,
	#endif
	STR_ID_GX_COPTER_GAME_NAME_JET,

	#if defined(__MMI_TOUCH_SCREEN__)
	STR_ID_GX_COPTER_HELP_DESCRIPTION_JET_TP,
	#else
	STR_ID_GX_COPTER_HELP_DESCRIPTION_JET,
	#endif

	STR_ID_GX_COPTER_COUNT
} gx_copter_str_id_enum;


/* Image ID */
typedef enum {
	IMG_ID_GX_COPTER_GAME_ICON = GAME_COPTER_BASE + 1,
	IMG_ID_GX_COPTER_OBSTACLE,
	IMG_ID_GX_COPTER_NUMBER_0,
	IMG_ID_GX_COPTER_NUMBER_1,
	IMG_ID_GX_COPTER_NUMBER_2,
	IMG_ID_GX_COPTER_NUMBER_3,
	IMG_ID_GX_COPTER_NUMBER_4,
	IMG_ID_GX_COPTER_NUMBER_5,
	IMG_ID_GX_COPTER_NUMBER_6,
	IMG_ID_GX_COPTER_NUMBER_7,
	IMG_ID_GX_COPTER_NUMBER_8,
	IMG_ID_GX_COPTER_NUMBER_9,
	IMG_ID_GX_COPTER_BOX,
	IMG_ID_GX_COPTER_SCORE,
	IMG_ID_GX_COPTER_TITLE,
	IMG_ID_GX_COPTER_0,
	IMG_ID_GX_COPTER_1,
	IMG_ID_GX_COPTER_2,
	IMG_ID_GX_COPTER_CRASHED_0,
	IMG_ID_GX_COPTER_CRASHED_1,
	IMG_ID_GX_COPTER_CRASHED_2,
	IMG_ID_GX_COPTER_GOTEXT,
	IMG_ID_GX_COPTER_GRADESMAP,
	IMG_ID_GX_COPTER_GOPIC,
#if defined(__MMI_TOUCH_SCREEN__)
	IMG_ID_GX_COPTER_UP,
	IMG_ID_GX_COPTER_DOWN,
#endif
	IMG_ID_GX_COPTER_BACKGROUND,
	IMG_ID_GX_COPTER_COUNT
} gx_copter_img_id_neum;


#endif	/* IS_COPTER_BASE_GAME */


