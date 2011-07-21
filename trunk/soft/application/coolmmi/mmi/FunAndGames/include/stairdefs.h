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
 *	GameStairDefs.h
  *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Game id define Stair.
 *
 * Author:
 * -------
 *	Bear Wang (CSD00612)
 * Vincent Hsu 
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/FunAndGames/include/StairDefs.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 10:58:12   lin
 * �ָ�CSW����ǰ�İ汾
 * 
 *    Rev 1.1   Nov 24 2006 18:38:42   lin
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
#ifndef _MMI_GX_STAIR_DEFS_H
#define _MMI_GX_STAIR_DEFS_H

/***************************************************************************** 
* Typedef 
*****************************************************************************/
/* String ID */
typedef enum {
#if defined(__MMI_GAME_STAIR_128x160__) || defined(__MMI_GAME_STAIR_128x128__) || defined(__MMI_GAME_STAIR_176x220__)
	STR_ID_GX_STAIR_GAME_NAME = GAME_STAIR_BASE + 1,
	STR_ID_GX_STAIR_HELP_DESCRIPTION,	
#elif defined(__MMI_GAME_CHICKEN_128x128__) || defined(__MMI_GAME_CHICKEN_128x160__)
	STR_ID_GX_STAIR_GAME_NAME_CHICKEN = GAME_STAIR_BASE + 1,
	STR_ID_GX_STAIR_HELP_DESCRIPTION_CHICKEN,
#elif defined(__MMI_GAME_ROBOT_176x220__) || defined(__MMI_GAME_ROBOT_128x160__)
	STR_ID_GX_STAIR_GAME_NAME_ROBOT = GAME_STAIR_BASE + 1,
	STR_ID_GX_STAIR_HELP_DESCRIPTION_ROBOT,
#endif
	STR_ID_GX_STAIR_LEVEL_EASY,
	STR_ID_GX_STAIR_LEVEL_NORMAL,
	STR_ID_GX_STAIR_LEVEL_HARD,

	STR_ID_GX_STAIR_COUNT
} gx_stair_str_id_enum;


/* Image ID */
typedef enum {
	IMG_ID_GX_STAIR_GAME_ICON = GAME_STAIR_BASE + 1,
	IMG_ID_GX_STAIR_BOX,
	IMG_ID_GX_STAIR_FLOAT_STAIR_1,
	IMG_ID_GX_STAIR_FLOAT_STAIR_2,
	IMG_ID_GX_STAIR_ROLE,
	IMG_ID_GX_STAIR_ROLE_DYING,
	IMG_ID_GX_STAIR_ROLE_FALLING_0,
	IMG_ID_GX_STAIR_ROLE_FALLING_1,
	IMG_ID_GX_STAIR_ROLE_LEFT_0,
	IMG_ID_GX_STAIR_ROLE_LEFT_1,
	IMG_ID_GX_STAIR_ROLE_RIGHT_0,
	IMG_ID_GX_STAIR_ROLE_RIGHT_1,
	IMG_ID_GX_STAIR_SCORE,
	IMG_ID_GX_STAIR_STAIR,
	IMG_ID_GX_STAIR_THORN,
	IMG_ID_GX_STAIR_THORN_STAIR,
	IMG_ID_GX_STAIR_TITLE,
	IMG_ID_GX_STAIR_WALL,
	#if defined(__MMI_GAME_STAIR_128x160__) || defined(__MMI_GAME_STAIR_176x220__) || defined(__MMI_GAME_CHICKEN_128x160__) || defined(__MMI_GAME_ROBOT_176x220__) || defined(__MMI_GAME_ROBOT_128x160__)
	IMG_ID_GX_STAIR_ENERGY_BAR,
	#elif defined(__MMI_GAME_STAIR_128x128__) || defined(__MMI_GAME_CHICKEN_128x128__)
	IMG_ID_GX_STAIR_ENERGY_BAR_HORIZONTAL,
	#endif
	IMG_ID_GX_STAIR_FLYING_MARK,
	IMG_ID_GX_STAIR_ROLE_FLYING_0,
	IMG_ID_GX_STAIR_ROLE_FLYING_1,

	IMG_ID_GX_STAIR_NUMBER_0,
	IMG_ID_GX_STAIR_NUMBER_1,
	IMG_ID_GX_STAIR_NUMBER_2,
	IMG_ID_GX_STAIR_NUMBER_3,
	IMG_ID_GX_STAIR_NUMBER_4,
	IMG_ID_GX_STAIR_NUMBER_5,
	IMG_ID_GX_STAIR_NUMBER_6,
	IMG_ID_GX_STAIR_NUMBER_7,
	IMG_ID_GX_STAIR_NUMBER_8,
	IMG_ID_GX_STAIR_NUMBER_9,

	IMG_ID_GX_STAIR_GOTEXT,
	IMG_ID_GX_STAIR_GRADESMAP,
	IMG_ID_GX_STAIR_GOPIC,

	IMG_ID_GX_STAIR_BACKGROUND,

	IMG_ID_GX_STAIR_COUNT
} gx_stair_img_id_neum;

#endif	/* _MMI_GX_STAIR_DEFS_H */


