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
 *	DollDefs.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Catch Doll Game ID Declare
 *
 * Author:
 * -------
 *	Bear Wang (CSD00612)
 *                      
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/FunAndGames/include/DollDefs.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 10:57:58   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:38:28   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:24:34   zhangxb
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

#ifndef GAME_DOLL_DEFS_H
#define GAME_DOLL_DEFS_H

typedef enum {
	STR_GX_DOLL_GAME_NAME = GAME_DOLL_BASE+1,
	STR_GX_DOLL_NEW,
	STR_GX_DOLL_RESUME,
	STR_GX_DOLL_GRADE,
	STR_GX_DOLL_HELP,
	STR_GX_DOLL_HELP_DESCRIPTION,	
	STR_GX_DOLL_COUNT
} DOLL_STR_ID;


/* Image ID */
typedef enum {
	// Image
	IMG_GX_DOLL_GAME_ICON = GAME_DOLL_BASE+1, 
	IMG_GX_DOLL_SEAL_0,
	IMG_GX_DOLL_SEAL_1,
	IMG_GX_DOLL_SEAL_2,
	IMG_GX_DOLL_SEAL_3,
	IMG_GX_DOLL_ARM,
	IMG_GX_DOLL_ARM_BASE,
	IMG_GX_DOLL_CLAW_0,
	IMG_GX_DOLL_CLAW_1,
	IMG_GX_DOLL_CLAW_2,
	IMG_GX_DOLL_CLAW_3,
	IMG_GX_DOLL_CHAT_BOX,
	IMG_GX_DOLL_CATCHED,
	IMG_GX_DOLL_CAN_0,
	IMG_GX_DOLL_CAN_1,
	IMG_GX_DOLL_CAN_2,
	IMG_GX_DOLL_CAN_BOX,
	IMG_GX_DOLL_COIN,
	IMG_GX_DOLL_SMALLDOLL,
	IMG_GX_DOLL_GOTEXT,
	IMG_GX_DOLL_GRADESMAP,
	IMG_GX_DOLL_NUM_0,
	IMG_GX_DOLL_NUM_1,
	IMG_GX_DOLL_NUM_2,
	IMG_GX_DOLL_NUM_3,
	IMG_GX_DOLL_NUM_4,
	IMG_GX_DOLL_NUM_5,
	IMG_GX_DOLL_NUM_6,
	IMG_GX_DOLL_NUM_7,
	IMG_GX_DOLL_NUM_8,
	IMG_GX_DOLL_NUM_9,
	IMG_GX_DOLL_NUM_10,
	IMG_GX_DOLL_COUNT
} DOLL_IMG_ID;

#endif	/* GAME_DOLL_DEFS_H */


