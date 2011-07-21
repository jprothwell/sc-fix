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
 *	 PhotoEditor.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  PhotoEditor Header
 *
 * Author:
 * -------
 * -------
 *                      
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _PHOTOEDITOR_H_
#define _PHOTOEDITOR_H_

#ifdef __MMI_PHOTOEDITOR__

/***************************************************************************** 
* Define
*****************************************************************************/
/* shall move to feature define files */
#define __PHOEDT_FEATURE_PHOART__
#define __PHOEDT_FEATURE_SCENMAST__
//#define __PHOEDT_FEATURE_IMGTILE__
//#define __PHOEDT_FEATURE_SLIDSHOW__


#define PHOEDT_NOTIFY_DURATION			2000
#define PHOEDT_NOTIFY_SHORT_DURATION	ST_NOTIFYDURATION

/***************************************************************************** 
* Typedef
*****************************************************************************/
typedef enum{
#ifdef __PHOEDT_FEATURE_PHOART__
	PHOEDT_PHOART,
#endif 

#ifdef __PHOEDT_FEATURE_SCENMAST__
	PHOEDT_SCENMAST,
#endif 

#ifdef __PHOEDT_FEATURE_IMGTILE__
	PHOEDT_IMGTILE,
#endif 	
	
#ifdef __PHOEDT_FEATURE_SLIDSHOW__
	PHOEDT_SLIDSHOW,
#endif 

	PHOEDT_APP_COUNT	/* how many app in photo editor */
} phoedt_app_enum;



/* 200 byte */
typedef struct{
	/* Photo Artist - 100 byte */
	S8		pa_add_text_string[80];					/* 80 byte */	/* text string */ /* reserve more than needed */
	S32	pa_text_color;
	S32	pa_border_color;
	S32	pa_shadow_color;	
	S32	pa_text_size;								/* text size */
	S32	pa_text_italic;

	/* reserve 100 byte for future app */
	U8		reserve[100];
	
} phoedt_nvram_data_struct;


typedef struct
{
	phoedt_nvram_data_struct	setting_data;
	S8									filename_buf[1024];
} phoedt_cntx_struct;


#endif /* __MMI_PHOTOEDITOR__ */
#endif /* _PHOTOEDITOR_H_ */


