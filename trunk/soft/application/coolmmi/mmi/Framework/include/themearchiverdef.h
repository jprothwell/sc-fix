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
 *	ThemeArchiverDef.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file defines string, screen, and image ids for Theme Archiver application.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi/Framework/include/ThemeArchiverDef.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 10:57:40   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:38:10   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:24:18   zhangxb
 * Initial revision.
 * 
 *    Rev 1.2   Sep 19 2006 18:59:12   zhoumn
 * No change.
 * 
 *    Rev 1.1   Aug 28 2006 17:19:44   zhangxb
 * 更新了PSI和Simon修改的一些东西.
 * Revision 1.1.1.1  2006/06/08 12:42:59  liuruijin
 * no message
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: ThemeArchiverDef.h

  	PURPOSE		: Theme Archiver

 

 

	DATE		: April 14,05

**************************************************************/
#ifndef _MMI_THEME_ARCHIVER_DEF_H
#define _MMI_THEME_ARCHIVER_DEF_H

#include "mmi_features.h" 
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__



#define MAX_FILE_NAME_LENGTH	20
#define MAX_VERSION_LENGTH		5
#define MAX_URL_LENGTH			20
#define MAX_FILE_TYPE_LENGTH			10
#define MAX_MODEL_NAME_LENGTH			20
#define MAX_FILE_SIZE_LENGTH			20
#define MAX_DESCRIPTOR_ELEMENTS_NUM		8
#define MAX_ACCEPTABLE_THEME_FILE_TYPE	2


typedef enum
{
	CONTENT_TYPE_OMA,
	CONTENT_TYPE_NON_OMA

}DA_DESCRIPTOR_TYPE;

typedef enum
{
		DLT_TYPE,
		DLT_URI,
		DLT_SIZE,
		DLT_VERSION,
		DLT_NOTIFY_URI,
		DLT_FILE_NAME,
		DLT_LCD_HEIGHT,
		DLT_LCD_WIDTH
}DESCRIPTOR_TAG_TYPE;

const S8 *gThemeDesriptorTags[MAX_DESCRIPTOR_ELEMENTS_NUM]=
{
	{"type"},
	{"objectURI"},
	{"size"},
	{"ddversion"},
	{"installNotifyURI"},
	{"Name"},
	{"lcdht"},
	{"lcdwidth"}
};

const S8 *gAcceptableThemeFileTypes[MAX_ACCEPTABLE_THEME_FILE_TYPE]=
{
	{"med"},
	{"theme"}
};

typedef struct 
{
	U8 *file_name;
	U8 *version;
//	U8 uri[(MAX_URL_LENGTH+1)*ENCODING_LENGTH];
	U8 *uri;
	U8 *type;
	U8 *size;
	U8 *model_name;
	U16 lcd_height;
	U16 lcd_width;
}da_descriptor_details_struct;


typedef struct
{
	U8 currentHiliteStorageType;
	DA_DESCRIPTOR_TYPE content_type;
	XML_PARSER_STRUCT *parser;
	float32 da_file_size;
	S8 file_name[(FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH];
	U16 currStorageType;
	U16 discElementPresent;
	U8 numThemeType;
}DLAgentContextStruct;


#endif // #ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
#endif// #ifndef _MMI_THEME_ARCHIVER_DEF_H


