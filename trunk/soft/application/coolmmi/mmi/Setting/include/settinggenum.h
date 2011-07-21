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
 *	SettingGenum.h
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
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================== 
 *******************************************************************************/

#ifndef _PIXCOM_SETTINGGENUM_H
#define _PIXCOM_SETTINGGENUM_H


typedef enum setting_akpl_time
{
	TIME_0=0,
	TIME_5,
	TIME_30,
	TIME_60,
	TIME_2
}SETTING_AKPL_TIME;

typedef enum setting_contrast
{
	CONTRAST1 =0,
	CONTRAST2,
	CONTRAST3,
	CONTRAST4,
	CONTRAST5
}SETTING_CONTRAST;


typedef enum _smu_security_type_enum
{
   MMI_SETTING_TYPE_NO_REQUIRED,
   MMI_SETTING_TYPE_PHONELOCK,
   MMI_SETTING_TYPE_CHV1,
   MMI_SETTING_TYPE_CHV2,
   MMI_SETTING_TYPE_UBCHV1,
   MMI_SETTING_TYPE_UBCHV2,
   MMI_SETTING_TYPE_NP,
   MMI_SETTING_TYPE_NSP,
   MMI_SETTING_TYPE_SP,
   MMI_SETTING_TYPE_CP,
   MMI_SETTING_TYPE_IMSI_LOCK,
   MMI_SETTING_TYPE_PERSONALIZATION_KEY,
   MMI_SETTING_TYPE_CHV1_BLOCKED,
   MMI_SETTING_TYPE_CHV2_BLOCKED,
   MMI_SETTING_TYPE_SIM_CARD_BLOCKED,
   MMI_SETTING_TYPE_UBCHV2_BLOCKED,
   MMI_SETTING_TYPE_UBCHV1_CHV1_NOT_LOCK,
   MMI_SETTING_TYPE_UNSPECIFIED
}SMUSECURITYTYPE;

#define MMIPHONEVERIFYREQ mmi_smu_verify_pin_req_struct
#define PHONERESTORERSP mmi_smu_verify_pin_rsp_struct

typedef enum setting_ctr
{
	CTR_OFF =0,
	CTR_SINGLE,
	CTR_PERIODIC
}SETTING_CTR;

 
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#else
typedef enum 
{
 
#ifdef __MMI_MAINMENU_MATRIX_SUPPORT__
   PHNSET_MAINMENU_STYLE_MATRIX, 
#endif
#ifdef __MMI_MAINMENU_PAGE_SUPPORT__ 
   PHNSET_MAINMENU_STYLE_PAGE, 
#endif
#ifdef __MMI_MAINMENU_LIST_SUPPORT__ 
   PHNSET_MAINMENU_STYLE_LIST, 
#endif
#ifdef __MMI_MAINMENU_CIRCULAR_SUPPORT__ 
   PHNSET_MAINMENU_STYLE_CIRCULE,
#endif
#ifdef __MMI_MAINMENU_ROTATE_SUPPORT__
   PHNSET_MAINMENU_STYLE_ROTATE,
#endif
#ifdef __MMI_MAINMENU_TAB_SUPPORT__
   PHNSET_MAINMENU_STYLE_TAB,		
#endif
 
   PHNSET_MAINMENU_STYLE_MAX
} PHNSET_MAINMENU_STYLE_TYPE;
#endif

#endif

