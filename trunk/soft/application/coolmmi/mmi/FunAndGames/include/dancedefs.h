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
 *	GameDanceDefs.h
  *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Game id define Dance.
 *
 * Author:
 * -------
 *	Bear Wang (CSD00612)
 * Lin
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/FunAndGames/include/DanceDefs.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 10:57:58   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:38:26   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:24:32   zhangxb
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
#ifndef GAME_DANCE_DEFS_H
#define GAME_DANCE_DEFS_H

typedef enum {
	STR_ID_GX_DANCE_GAME_NAME = GAME_DANCE_BASE+1,
	#if defined(__MMI_TOUCH_SCREEN__)
	STR_ID_GX_DANCE_HELP_DESCRIPTION_TP,
	#else
	STR_ID_GX_DANCE_HELP_DESCRIPTION,	
	#endif
	STR_ID_GX_DANCE_COUNT
} DANCE_STR_ID;


/* Image ID */
typedef enum {
	// Image
	IMG_ID_GX_DANCE_GAME_ICON = GAME_DANCE_BASE+1, 
	IMG_ID_GX_DANCE_COUNT,
	IMG_ID_GX_DANCE_BACKGROUND,
	IMG_ID_GX_DANCE_LIVEDISPLAY,
	IMG_ID_GX_DANCE_SCOREDISPLAY,
	IMG_ID_GX_DANCE_LIGHTDISPLAY,

	IMG_ID_GX_DANCE_COMPUTER_STANDBY,
	IMG_ID_GX_DANCE_COMPUTER_RIGHT_UP,	
	IMG_ID_GX_DANCE_COMPUTER_RIGHT_DOWN,
	IMG_ID_GX_DANCE_COMPUTER_LEFT_UP,
	IMG_ID_GX_DANCE_COMPUTER_LEFT_DOWN,

	IMG_ID_GX_DANCE_USER_STANDBY,
	IMG_ID_GX_DANCE_USER_RIGHT_UP,
	IMG_ID_GX_DANCE_USER_RIGHT_DOWN,
	IMG_ID_GX_DANCE_USER_LEFT_UP,
	IMG_ID_GX_DANCE_USER_LEFT_DOWN,

	IMG_ID_GX_DANCE_USER_LIVE,

	IMG_ID_GX_DANCE_GAME_OVER,
	IMG_ID_GX_DANCE_GRADESMAP,
	IMG_ID_GX_DANCE_PIC,

	IMG_ID_GX_DANCE_NUM_0,
	IMG_ID_GX_DANCE_NUM_1,
	IMG_ID_GX_DANCE_NUM_2,
	IMG_ID_GX_DANCE_NUM_3,
	IMG_ID_GX_DANCE_NUM_4,
	IMG_ID_GX_DANCE_NUM_5,
	IMG_ID_GX_DANCE_NUM_6,
	IMG_ID_GX_DANCE_NUM_7,
	IMG_ID_GX_DANCE_NUM_8,
	IMG_ID_GX_DANCE_NUM_9,
#if defined(__MMI_TOUCH_SCREEN__)
	IMG_ID_GX_DANCE_UP,
	IMG_ID_GX_DANCE_DOWN,
#endif
	IMG_ID_GX_DANCE_LIGHT_TRUE,
	IMG_ID_GX_DANCE_LIGHT_FALSE,
	IMG_ID_GX_DANCE_LIGHT_EMPTY
} DANCE_IMG_ID;


#endif	/* GAME_DANCE_DEFS_H */


