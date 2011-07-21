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
 *	SmashDefs.h
  *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Smash game id defines
 *
 * Author:
 * -------
 *	Bear Wang (CSD00612)
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/FunAndGames/include/SmashDefs.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 10:58:10   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:38:40   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:24:42   zhangxb
 * Initial revision.
 * Revision 1.1.1.1  2006/08/02 02:23:57  sushipeng
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

#ifndef GAME_SMASH_DEFS_H
#define GAME_SMASH_DEFS_H

typedef enum {
	STR_GX_SMASH_GAME_NAME = GAME_SMASH_BASE + 1,
	STR_GX_SMASH_TIME_SEC,
	STR_GX_SMASH_NORMAL,
	STR_GX_SMASH_SPEEDY,
	STR_GX_SMASH_ACCURACY,
	STR_GX_SMASH_HELP_DESCRIPTION
} SMASH_STR_IDS;


/* Image ID */
typedef enum {
	// Image
	IMG_GX_SMASH_GAME_ICON = GAME_SMASH_BASE+1,
	IMG_GX_SMASH_BG_0, 
	IMG_GX_SMASH_BG_1,
	IMG_GX_SMASH_BG_2,
	IMG_GX_SMASH_BG_3,
	IMG_GX_SMASH_BG_4,
	IMG_GX_SMASH_BG_5,
	IMG_GX_SMASH_BG_6,
	IMG_GX_SMASH_BG_7,
	IMG_GX_SMASH_BG_8,
	IMG_GX_SMASH_MOUSE_0,
	IMG_GX_SMASH_MOUSE_1,
	IMG_GX_SMASH_MOUSE_2,
	IMG_GX_SMASH_MOUSE_3,
	IMG_GX_SMASH_MOUSE_4,
	IMG_GX_SMASH_MOUSE_5,
	IMG_GX_SMASH_COW_0,
	IMG_GX_SMASH_COW_1,
	IMG_GX_SMASH_COW_2,
	IMG_GX_SMASH_COW_3,
	IMG_GX_SMASH_COW_4,
	IMG_GX_SMASH_COW_5,
	IMG_GX_SMASH_BONE_0,
	IMG_GX_SMASH_BONE_1,
	IMG_GX_SMASH_BONE_2,
	IMG_GX_SMASH_BONE_3,
	IMG_GX_SMASH_BONE_4,
	IMG_GX_SMASH_BONE_5,
	IMG_GX_SMASH_PIG_0,
	IMG_GX_SMASH_PIG_1,
	IMG_GX_SMASH_PIG_2,
	IMG_GX_SMASH_PIG_3,
	IMG_GX_SMASH_PIG_4,
	IMG_GX_SMASH_PIG_5,
	IMG_GX_SMASH_HAND_0,
	IMG_GX_SMASH_HAND_1,
	IMG_GX_SMASH_HAND_2,
	IMG_GX_SMASH_HITTHIS,
	IMG_GX_SMASH_GOTEXT,
	IMG_GX_SMASH_GRADESMAP,
	IMG_GX_SMASH_GOPIC
} SMASH_IMG_IDS;

#endif /* GAME_SMASH_DEFS_H */



