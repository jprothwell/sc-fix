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
 *	NinjaDefs_128x96.h
  *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Game id define Ninja.
 *
 * Author:
 * -------
 *	Bear Wang (CSD00612)
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/FunAndGames/include/NinjaDefs.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 10:58:06   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:38:36   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:24:38   zhangxb
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
 
 *============================================================================
 ****************************************************************************/
#ifndef GAME_NINJA_DEFS_H
#define GAME_NINJA_DEFS_H

typedef enum {
	STR_GX_NINJA_GAME_NAME = GAME_NINJA_BASE+1,
	STR_GX_NINJA_HELP_DESCRIPTION,
	STR_GX_NINJA_RANK_TITLE_0,
	STR_GX_NINJA_RANK_TITLE_1,
	STR_GX_NINJA_RANK_TITLE_2,
	STR_GX_NINJA_RANK_TITLE_3,
	STR_GX_NINJA_RANK_TITLE_4,
	STR_GX_NINJA_RANK_TITLE_5,
	STR_GX_NINJA_RANK_TITLE_6,
	STR_GX_NINJA_RANK_TITLE_7
} NINJA_STR_ID;


/* Image ID */
typedef enum {
	// Image
	IMG_GX_NINJA_GAME_ICON = GAME_NINJA_BASE+1,
	IMG_GX_NINJA_WOOD,
	IMG_GX_NINJA_WOOD_0,
	IMG_GX_NINJA_WOOD_1,
	IMG_GX_NINJA_WOOD_2,
	IMG_GX_NINJA_WOOD_3,
	IMG_GX_NINJA_WOOD_4,
	IMG_GX_NINJA_WOOD_5,
	IMG_GX_NINJA_BG,
	IMG_GX_NINJA_MAN_0,
	IMG_GX_NINJA_MAN_1,
	IMG_GX_NINJA_MAN_2,
	IMG_GX_NINJA_MAN_WATER_0,
	IMG_GX_NINJA_MAN_WATER_1,
	IMG_GX_NINJA_MAN_WATER_2,
	IMG_GX_NINJA_SCROLL,
	IMG_GX_NINJA_SCROLL_BODY,
	IMG_GX_NINJA_BG_TREE_0,
	IMG_GX_NINJA_BG_TREE_1,
	IMG_GX_NINJA_BG_TREE_2,
	IMG_GX_NINJA_BG_TREE_3,
	IMG_GX_NINJA_BG_TREE_4,
	IMG_GX_NINJA_BG_TREE_5,
	IMG_GX_NINJA_BG_TREE_6,
	IMG_GX_NINJA_BG_TREE_7,
	IMG_GX_NINJA_BG_WATER,
	IMG_GX_NINJA_BG_SKY,
	IMG_GX_NINJA_SCORE_WOOD,
	IMG_GX_NINJA_GAMEOVER_SCROLL
} NINJA_IMG_ID;


#endif	/* GAME_NINJA_DEFS_H */


