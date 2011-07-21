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

/*******************************************************************************
 * Filename:
 * ---------
 *  ImageID.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/MiscFramework/include/ImageID.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 11:02:10   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:42:34   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:27:44   zhangxb
 * Initial revision.
 * 
 *    Rev 1.2   Sep 19 2006 19:06:20   zhoumn
 * 更新了部分代码、文档
 * 
 *    Rev 1.1   Aug 28 2006 17:19:52   zhangxb
 * 更新了PSI和Simon修改的一些东西.
 * Revision 1.1.1.1  2006/06/08 12:43:17  liuruijin
 * no message
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/*****************************************************************************

	Filename:	ImageID.h

	Purpose:	IDs of images to be displayed.

 

	Date:		Aug 6, 2002

	
*******************************************************************************/


#define MAX_IMAGE_SIZE	131072
#define MAX_FRAMES_IN_IMAGE 32
#define MAX_PALETTE_IN_IMAGE 256

#define IMAGE_ID_17		(U8*)"..\\Images\\DemoIcons\\Image1.bmp"
#define IMAGE_ID_18		(U8*)"..\\Images\\DemoIcons\\Image2.bmp"
#define IMAGE_ID_19		(U8*)"..\\Images\\DemoIcons\\Image3.bmp"
#define IMAGE_ID_20		(U8*)"..\\Images\\DemoIcons\\Image4.bmp"
#define IMAGE_ID_21		(U8*)"..\\Images\\DemoIcons\\Image5.bmp"
#define IMAGE_ID_22		(U8*)"..\\Images\\DemoIcons\\Image6.bmp"
#define IMAGE_ID_23		(U8*)"..\\Images\\DemoIcons\\Image7.bmp"
#define IMAGE_ID_24		(U8*)"..\\Images\\DemoIcons\\Image8.bmp"
#define IMAGE_ID_25		(U8*)"..\\Images\\DemoIcons\\Image9.bmp"
#define IMAGE_ID_26		(U8*)"..\\Images\\DemoIcons\\Image10.bmp"

#define IMAGE_ID_27		(U8*)"..\\Images\\SplashScreens\\logo.gif"

#define IMAGE_ID_SCROLLBAR_5X_LEFT		(U8*)"..\\Images\\scrollbar\\LeftArrow.bmp"
#define IMAGE_ID_SCROLLBAR_5X_RIGHT		(U8*)"..\\Images\\scrollbar\\RightArrow.bmp"
#define IMAGE_ID_SCROLLBAR_5X_UP		(U8*)"..\\Images\\scrollbar\\UpArrow.bmp"
#define IMAGE_ID_SCROLLBAR_5X_DOWN		(U8*)"..\\Images\\scrollbar\\DownArrow.bmp"

#define IMAGE_ID_WALLPAPER1				(U8*)"..\\Images\\Wallpapers\\w1.bmp"
#define IMAGE_ID_WALLPAPER2				(U8*)"..\\Images\\Wallpapers\\w2.bmp"
#define IMAGE_ID_WALLPAPER3				(U8*)"..\\Images\\Wallpapers\\w3.bmp"
#define IMAGE_ID_WALLPAPER4				(U8*)"..\\Images\\Wallpapers\\w4.bmp"

#define IMAGE_RADIO_ON		(U8*)	"..\\Images\\TwoStateIcons\\RadioOn.bmp"
#define IMAGE_RADIO_OFF		(U8*)	"..\\Images\\TwoStateIcons\\RadioOff.bmp"
#define IMAGE_CHECKBOX_ON	(U8*)	"..\\Images\\TwoStateIcons\\CheckBoxOn.bmp"
#define IMAGE_CHECKBOX_OFF	(U8*)	"..\\Images\\TwoStateIcons\\CheckBoxOff.bmp"

#define IMAGE_ID_CUSTOM_LOGO		"..\\Images\\SplashScreens\\customlogo.bmp"

#define IMAGE_ID_ANIMATION_DEMO			(U8*)"..\\Images\\DemoImages\\Animation1\\FunAndGames"

