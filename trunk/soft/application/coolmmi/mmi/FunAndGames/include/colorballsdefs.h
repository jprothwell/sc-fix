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
 *	ColorBallsDefs.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  ColorBalls id define
 *
 * Author:
 * -------
 *	Bear Wang (CSD00612)
 *                      
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/FunAndGames/include/ColorBallsDefs.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 10:57:54   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:38:24   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:24:30   zhangxb
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

#ifndef COLORBALLDEFS_H
#define COLORBALLDEFS_H


typedef enum
{
	STR_GX_COLORBALLS_GAME_NAME = GAME_COLORBALLS_BASE+1, 
	STR_GX_COLORBALLS_EASY,
	STR_GX_COLORBALLS_NORMAL,
	STR_GX_COLORBALLS_HARD,
	STR_GX_COLORBALLS_DESCRIPTION
}COLORBALLS_STR_IDS;

typedef enum
{
	IMG_GX_COLORBALLS_GAME_ICON = GAME_COLORBALLS_BASE+1,
	IMG_GX_COLORBALLS_BIG_BLUE,
	IMG_GX_COLORBALLS_BIG_GREEN,
	IMG_GX_COLORBALLS_BIG_LIGHT,
	IMG_GX_COLORBALLS_BIG_PINK,
	IMG_GX_COLORBALLS_SMALL_BLUE,
	IMG_GX_COLORBALLS_SMALL_GREEN,
	IMG_GX_COLORBALLS_SMALL_LIGHT,
	IMG_GX_COLORBALLS_SMALL_PINK,
	IMG_GX_COLORBALLS_BG,
	IMG_GX_COLORBALLS_SCORE,
	IMG_GX_COLORBALLS_LAUNCH,
	IMG_GX_COLORBALLS_ENERGYBG,
	IMG_GX_COLORBALLS_GRADEBG,
	IMG_GX_COLORBALLS_LEVELBG,
	IMG_GX_COLORBALLS_ENERGYBAR,
	IMG_GX_COLORBALLS_GOTEXT,
	IMG_GX_COLORBALLS_GRADESMAP,
	IMG_GX_COLORBALLS_GOPIC
}COLORBALLS_IMG_IDS;


#endif /* COLORBALLDEFS_H */


