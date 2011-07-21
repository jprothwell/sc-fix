/*****************************************************************************
* Copyright Statement:
* --------------------
* This software is protected by Copyright and the information contained
* herein is confidential. The software may not be copied and the information
* contained herein may not be used or disclosed except with the written
* permission of UNISCOPE Inc. (C) 2005
*
*****************************************************************************/
/*============================================================================
*
*
* $Id: MajungDefs.h,v 1.1 2007/05/17 07:21:58 baiyanjun Exp $
*
* $Date: 2007/05/17 07:21:58 $
*
* $Name: 1.1 $
*
* $Locker$
*
* $Revision: 1.1 $
*
* $State: Exp $
*
* HISTORY
* Below this line, this part is controlled by CVS. DO NOT MODIFY!!
*----------------------------------------------------------------------------
* $Log: MajungDefs.h,v $
* Revision 1.1  2007/05/17 07:21:58  baiyanjun
* UNI@baiyanjun_20070517 15:10:01 COOLSAND U25 U26 06B V30 version
*
*
*----------------------------------------------------------------------------
* Upper this line, this part is controlled by CVS. DO NOT MODIFY!!
*============================================================================
****************************************************************************/
/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
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
 * GameMajungDefs.h
  *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Game id define Majung.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef GAME_MAJUNG_DEFS_H
#define GAME_MAJUNG_DEFS_H

typedef enum {
    STR_GX_MAJUNG_GAME_NAME = GAME_MAJUNG_BASE + 1,
    STR_GX_MAJUNG_NEW,
    STR_GX_MAJUNG_RESUME,
    STR_GX_MAJUNG_LEVEL,
    STR_GX_MAJUNG_SCORE,
    STR_GX_MAJUNG_HELP,
    STR_GX_MAJUNG_LEVEL_EASY,
    STR_GX_MAJUNG_LEVEL_NORMAL,
    STR_GX_MAJUNG_LEVEL_HARD,
    STR_GX_MAJUNG_HELP_DESCRIPTION,
    STR_GX_MAJUNG_COUNT
} MAJUNG_STR_ID;

/* Image ID */
typedef enum {
    /* Image */
    IMG_GX_MAJUNG_GAME_ICON = GAME_MAJUNG_BASE + 1,
    IMG_GX_MAJUNG_BRICK_1W,
    IMG_GX_MAJUNG_BRICK_2W,
    IMG_GX_MAJUNG_BRICK_3W,
    IMG_GX_MAJUNG_BRICK_4W,
    IMG_GX_MAJUNG_BRICK_5W,
    IMG_GX_MAJUNG_BRICK_6W,
    IMG_GX_MAJUNG_BRICK_7W,
    IMG_GX_MAJUNG_BRICK_8W,
    IMG_GX_MAJUNG_BRICK_9W,
    IMG_GX_MAJUNG_BRICK_1T,
    IMG_GX_MAJUNG_BRICK_2T,
    IMG_GX_MAJUNG_BRICK_3T,
    IMG_GX_MAJUNG_BRICK_4T,
    IMG_GX_MAJUNG_BRICK_5T,
    IMG_GX_MAJUNG_BRICK_6T,
    IMG_GX_MAJUNG_BRICK_7T,
    IMG_GX_MAJUNG_BRICK_8T,
    IMG_GX_MAJUNG_BRICK_9T,
    IMG_GX_MAJUNG_BRICK_2S,
    IMG_GX_MAJUNG_BRICK_3S,
    IMG_GX_MAJUNG_BRICK_4S,
    IMG_GX_MAJUNG_BRICK_5S,
    IMG_GX_MAJUNG_BRICK_6S,
    IMG_GX_MAJUNG_BRICK_7S,
    IMG_GX_MAJUNG_BRICK_8S,
    IMG_GX_MAJUNG_BRICK_9S,
    IMG_GX_MAJUNG_BRICK_A,
    IMG_GX_MAJUNG_BRICK_B,
    IMG_GX_MAJUNG_BRICK_C,
    IMG_GX_MAJUNG_BRICK_D,
    IMG_GX_MAJUNG_BRICK_F,
    IMG_GX_MAJUNG_BRICK_G,
    IMG_GX_MAJUNG_BRICK_H,
    IMG_GX_MAJUNG_BRICK_1W_SEL,
    IMG_GX_MAJUNG_BRICK_2W_SEL,
    IMG_GX_MAJUNG_BRICK_3W_SEL,
    IMG_GX_MAJUNG_BRICK_4W_SEL,
    IMG_GX_MAJUNG_BRICK_5W_SEL,
    IMG_GX_MAJUNG_BRICK_6W_SEL,
    IMG_GX_MAJUNG_BRICK_7W_SEL,
    IMG_GX_MAJUNG_BRICK_8W_SEL,
    IMG_GX_MAJUNG_BRICK_9W_SEL,
    IMG_GX_MAJUNG_BRICK_1T_SEL,
    IMG_GX_MAJUNG_BRICK_2T_SEL,
    IMG_GX_MAJUNG_BRICK_3T_SEL,
    IMG_GX_MAJUNG_BRICK_4T_SEL,
    IMG_GX_MAJUNG_BRICK_5T_SEL,
    IMG_GX_MAJUNG_BRICK_6T_SEL,
    IMG_GX_MAJUNG_BRICK_7T_SEL,
    IMG_GX_MAJUNG_BRICK_8T_SEL,
    IMG_GX_MAJUNG_BRICK_9T_SEL,
    IMG_GX_MAJUNG_BRICK_2S_SEL,
    IMG_GX_MAJUNG_BRICK_3S_SEL,
    IMG_GX_MAJUNG_BRICK_4S_SEL,
    IMG_GX_MAJUNG_BRICK_5S_SEL,
    IMG_GX_MAJUNG_BRICK_6S_SEL,
    IMG_GX_MAJUNG_BRICK_7S_SEL,
    IMG_GX_MAJUNG_BRICK_8S_SEL,
    IMG_GX_MAJUNG_BRICK_9S_SEL,
    IMG_GX_MAJUNG_BRICK_A_SEL,
    IMG_GX_MAJUNG_BRICK_B_SEL,
    IMG_GX_MAJUNG_BRICK_C_SEL,
    IMG_GX_MAJUNG_BRICK_D_SEL,
    IMG_GX_MAJUNG_BRICK_F_SEL,
    IMG_GX_MAJUNG_BRICK_G_SEL,
    IMG_GX_MAJUNG_BRICK_H_SEL,
    IMG_GX_MAJUNG_CONNECT_HOR,
    IMG_GX_MAJUNG_CONNECT_VER,
    IMG_GX_MAJUNG_CONNECT_LU,
    IMG_GX_MAJUNG_CONNECT_RU,
    IMG_GX_MAJUNG_CONNECT_LD,
    IMG_GX_MAJUNG_CONNECT_RD,
    IMG_GX_MAJUNG_THICK,
    IMG_GX_MAJUNG_BAR_FILL_1,
    IMG_GX_MAJUNG_BAR_FILL_2,
    IMG_GX_MAJUNG_BAR_FILL_3,
    IMG_GX_MAJUNG_BAR_FILL_4,
    IMG_GX_MAJUNG_BAR_FILL_5,
    IMG_GX_MAJUNG_BAR_FILL_6,
    IMG_GX_MAJUNG_BAR_FILL_7,
    IMG_GX_MAJUNG_BAR_FILL_8,
    IMG_GX_MAJUNG_BAR_FILL_9,
    IMG_GX_MAJUNG_BAR_FILL_10,
    IMG_GX_MAJUNG_BAR_FILL_11,
    IMG_GX_MAJUNG_BAR_FILL_12,
    IMG_GX_MAJUNG_BAR_FILL_13,
    IMG_GX_MAJUNG_BAR_FILL_14,
    IMG_GX_MAJUNG_BAR_FILL_15,
    IMG_GX_MAJUNG_BAR_FILL_16,
    IMG_GX_MAJUNG_BAR_FILL_17,
    IMG_GX_MAJUNG_BAR_FILL_18,
    IMG_GX_MAJUNG_BAR_FILL_19,
    IMG_GX_MAJUNG_BAR_FILL_20,
    IMG_GX_MAJUNG_BAR_EMPTY_1,
    IMG_GX_MAJUNG_BAR_EMPTY_2,
    IMG_GX_MAJUNG_BAR_EMPTY_3,
    IMG_GX_MAJUNG_BAR_EMPTY_4,
    IMG_GX_MAJUNG_BAR_EMPTY_5,
    IMG_GX_MAJUNG_BAR_EMPTY_6,
    IMG_GX_MAJUNG_BAR_EMPTY_7,
    IMG_GX_MAJUNG_BAR_EMPTY_8,
    IMG_GX_MAJUNG_BAR_EMPTY_9,
    IMG_GX_MAJUNG_BAR_EMPTY_10,
    IMG_GX_MAJUNG_BAR_EMPTY_11,
    IMG_GX_MAJUNG_BAR_EMPTY_12,
    IMG_GX_MAJUNG_BAR_EMPTY_13,
    IMG_GX_MAJUNG_BAR_EMPTY_14,
    IMG_GX_MAJUNG_BAR_EMPTY_15,
    IMG_GX_MAJUNG_BAR_EMPTY_16,
    IMG_GX_MAJUNG_BAR_EMPTY_17,
    IMG_GX_MAJUNG_BAR_EMPTY_18,
    IMG_GX_MAJUNG_BAR_EMPTY_19,
    IMG_GX_MAJUNG_BAR_EMPTY_20,
    IMG_GX_MAJUNG_GAMEOVER,
    IMG_GX_MAJUNG_GRADESMAP,
    IMG_GX_MAJUNG_0,
    IMG_GX_MAJUNG_1,
    IMG_GX_MAJUNG_2,
    IMG_GX_MAJUNG_3,
    IMG_GX_MAJUNG_4,
    IMG_GX_MAJUNG_5,
    IMG_GX_MAJUNG_6,
    IMG_GX_MAJUNG_7,
    IMG_GX_MAJUNG_8,
    IMG_GX_MAJUNG_9,
    IMG_GX_MAJUNG_BOX,
    IMG_GX_MAJUNG_EXIT_UP,
    IMG_GX_MAJUNG_EXIT_DOWN,
    IMG_GX_MAJUNG_COUNT,
    IMG_GX_MAJUNG_TIME
} MAJUNG_IMG_ID;

#endif /* GAME_MAJUNG_DEFS_H */ 

