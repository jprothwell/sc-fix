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
 *	CharsetTable.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file defines function prototypes for charset realted utilities
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
#ifndef _MMI_CHARSETS_H
#define _MMI_CHARSETS_H
#include "mmi_features.h"
#include "kal_release.h"



typedef struct 
{
  unsigned short start_value; 
  unsigned short end_value; 
  unsigned short index;
} Mapping_Struct;


typedef struct
{
	short start;
	short end;
} key_index_t;

extern const Mapping_Struct g_chset_map_ucs2_common[];
extern unsigned short mmi_chset_map_ucs2_common_size(void);

#ifdef __MMI_CHSET_ARABIC_ISO__
extern const unsigned short g_chset_ucs2_arabic_iso[];
extern const unsigned short g_chset_arabic_ucs2_iso[];
extern const Mapping_Struct g_chset_map_arabic_ucs2_iso[];
extern const Mapping_Struct g_chset_map_ucs2_arabic_iso[];
extern unsigned short mmi_chset_map_ucs2_arabic_iso_size(void);
extern unsigned short mmi_chset_map_arabic_ucs2_iso_size(void);
#endif

#ifdef __MMI_CHSET_ARABIC_WIN__
extern const unsigned short g_chset_ucs2_arabic_win[];
extern const unsigned short g_chset_arabic_ucs2_win[];
extern const Mapping_Struct g_chset_map_ucs2_arabic_win[];
extern unsigned short mmi_chset_map_arabic_ucs2_win_size(void);
extern unsigned short mmi_chset_map_ucs2_arabic_win_size(void);
#endif

#ifdef __MMI_CHSET_BALTIC_ISO__
extern const unsigned short g_chset_ucs2_baltic_iso[];
extern const unsigned short g_chset_baltic_ucs2_iso[];
extern const Mapping_Struct g_chset_map_ucs2_baltic_iso[];
extern const Mapping_Struct g_chset_map_baltic_ucs2_iso[];
extern unsigned short mmi_chset_map_baltic_ucs2_iso_size(void);
extern unsigned short mmi_chset_map_ucs2_baltic_iso_size(void);
#endif

#ifdef __MMI_CHSET_BALTIC_WIN__
extern const unsigned short g_chset_ucs2_baltic_win[];
extern const unsigned short g_chset_baltic_ucs2_win[];
extern const Mapping_Struct g_chset_map_ucs2_baltic_win[];
extern const Mapping_Struct g_chset_map_baltic_ucs2_win[];
extern unsigned short mmi_chset_map_ucs2_baltic_win_size(void);
extern unsigned short mmi_chset_map_baltic_ucs2_win_size(void);
#endif

#ifdef __MMI_CHSET_CEURO_ISO__
extern const unsigned short g_chset_ucs2_ceuro_iso[];
extern const unsigned short g_chset_ceuro_ucs2_iso[];
extern const Mapping_Struct g_chset_map_ucs2_ceuro_iso[];
extern const Mapping_Struct g_chset_map_ceuro_ucs2_iso[];
extern unsigned short mmi_chset_map_ucs2_ceuro_iso_size(void);
extern unsigned short mmi_chset_map_ceuro_ucs2_iso_size(void);
#endif

#ifdef __MMI_CHSET_CEURO_WIN__
extern const unsigned short g_chset_ucs2_ceuro_win[];
extern const unsigned short g_chset_ceuro_ucs2_win[];
extern const Mapping_Struct g_chset_map_ucs2_ceuro_win[];
extern const Mapping_Struct g_chset_map_ceuro_ucs2_win[];
extern unsigned short mmi_chset_map_ceuro_ucs2_win_size(void);
extern unsigned short mmi_chset_map_ucs2_ceuro_win_size(void);
#endif

#ifdef __MMI_CHSET_GREEK_ISO__
extern const unsigned short g_chset_ucs2_greek_iso[];
extern const unsigned short g_chset_greek_ucs2_iso[];
extern const Mapping_Struct g_chset_map_ucs2_greek_iso[];
extern const Mapping_Struct g_chset_map_greek_ucs2_iso[];
extern unsigned short mmi_chset_map_ucs2_greek_iso_size(void);
extern unsigned short mmi_chset_map_greek_ucs2_iso_size(void);
#endif

#ifdef __MMI_CHSET_GREEK_WIN__
extern const unsigned short g_chset_ucs2_greek_win[];
extern const unsigned short g_chset_greek_ucs2_win[];
extern const Mapping_Struct g_chset_map_ucs2_greek_win[];
extern const Mapping_Struct g_chset_map_greek_ucs2_win[];
extern unsigned short mmi_chset_map_greek_ucs2_win_size(void);
extern unsigned short mmi_chset_map_ucs2_greek_win_size(void);
#endif

#ifdef __MMI_CHSET_HEBREW_ISO__
extern const unsigned short g_chset_ucs2_hebrew_iso[];
extern const unsigned short g_chset_hebrew_ucs2_iso[];
extern const Mapping_Struct g_chset_map_ucs2_hebrew_iso[];
extern const Mapping_Struct g_chset_map_hebrew_ucs2_iso[];
extern unsigned short mmi_chset_map_hebrew_ucs2_iso_size(void);
extern unsigned short mmi_chset_map_ucs2_hebrew_iso_size(void);
#endif

#ifdef __MMI_CHSET_HEBREW_WIN__
extern const unsigned short g_chset_ucs2_hebrew_win[];
extern const unsigned short g_chset_hebrew_ucs2_win[];
extern const Mapping_Struct g_chset_map_ucs2_hebrew_win[];
extern const Mapping_Struct g_chset_map_hebrew_ucs2_win[];
extern unsigned short mmi_chset_map_ucs2_hebrew_win_size(void);
extern unsigned short mmi_chset_map_hebrew_ucs2_win_size(void);
#endif

#ifdef __MMI_CHSET_LATIN_ISO__
extern const unsigned short g_chset_ucs2_latin_iso[];
extern const unsigned short g_chset_latin_ucs2_iso[];
extern const Mapping_Struct g_chset_map_ucs2_latin_iso[];
extern const Mapping_Struct g_chset_map_latin_ucs2_iso[];
extern unsigned short mmi_chset_map_latin_ucs2_iso_size(void);
extern unsigned short mmi_chset_map_ucs2_latin_iso_size(void);
#endif

#ifdef __MMI_CHSET_NORDIC_ISO__
extern const unsigned short g_chset_ucs2_nordic_iso[];
extern const unsigned short g_chset_nordic_ucs2_iso[];
extern const Mapping_Struct g_chset_map_ucs2_nordic_iso[];
extern const Mapping_Struct g_chset_map_nordic_ucs2_iso[];
extern unsigned short mmi_chset_map_ucs2_nordic_iso_size(void);
extern unsigned short mmi_chset_map_nordic_ucs2_iso_size(void);
#endif

#ifdef __MMI_CHSET_SEURO_ISO__
extern const unsigned short g_chset_ucs2_seuro_iso[];
extern const unsigned short g_chset_seuro_ucs2_iso[];
extern const Mapping_Struct g_chset_map_ucs2_seuro_iso[];
extern const Mapping_Struct g_chset_map_seuro_ucs2_iso[];
extern unsigned short mmi_chset_map_ucs2_seuro_iso_size(void);
extern unsigned short mmi_chset_map_seuro_ucs2_iso_size(void);
#endif

#ifdef __MMI_CHSET_TURKISH_ISO__
extern const unsigned short g_chset_turkish_ucs2_iso[];
extern const unsigned short g_chset_ucs2_turkish_iso[];
extern const Mapping_Struct g_chset_map_turkish_ucs2_iso[];
extern const Mapping_Struct g_chset_map_ucs2_turkish_iso[];
extern unsigned short mmi_chset_map_turkish_ucs2_iso_size(void);
extern unsigned short mmi_chset_map_ucs2_turkish_iso_size(void);
#endif

#ifdef __MMI_CHSET_TURKISH_WIN__
extern const unsigned short g_chset_turkish_ucs2_win[];
extern const unsigned short g_chset_ucs2_turkish_win[];
extern const Mapping_Struct g_chset_map_turkish_ucs2_win[];
extern const Mapping_Struct g_chset_map_ucs2_turkish_win[];
extern unsigned short mmi_chset_map_ucs2_turkish_win_size(void);
extern unsigned short mmi_chset_map_turkish_ucs2_win_size(void);
#endif

#ifdef __MMI_CHSET_WESTERN_ISO__
extern const unsigned short g_chset_ucs2_western_iso[];
extern const unsigned short g_chset_western_ucs2_iso[];
extern const Mapping_Struct g_chset_map_ucs2_western_iso[];
extern unsigned short mmi_chset_map_ucs2_western_iso_size(void);
#endif

#ifdef __MMI_CHSET_WESTERN_WIN__
extern const unsigned short g_chset_ucs2_western_win[];
extern const unsigned short g_chset_western_ucs2_win[];
extern const Mapping_Struct g_chset_map_ucs2_western_win[];
extern const Mapping_Struct g_chset_map_western_ucs2_win[];
extern unsigned short mmi_chset_map_ucs2_western_win_size(void);
extern unsigned short mmi_chset_map_western_ucs2_win_size(void);
#endif


#ifdef __MMI_CHSET_BIG5__
extern const key_index_t g_chset_ucs2_to_big5_key_LSB_index[];
extern const key_index_t g_chset_big5_to_ucs2_key_LSB_index[];
extern const unsigned char g_chset_ucs2_to_big5_key_MSB[];
extern const unsigned char g_chset_big5_to_ucs2_key_MSB[];
extern const unsigned short g_chset_ucs2_to_big5_table[];
extern const unsigned short g_chset_big5_to_ucs2_table[];
#endif


#ifdef __MMI_CHSET_GB2312__
extern const key_index_t g_chset_ucs2_to_gb2312_key_LSB_index[];
extern const key_index_t g_chset_gb2312_to_ucs2_key_LSB_index[];
extern const unsigned char g_chset_ucs2_to_gb2312_key_MSB[];
extern const unsigned char g_chset_gb2312_to_ucs2_key_MSB[];
extern const unsigned short g_chset_ucs2_to_gb2312_table[];
extern const unsigned short g_chset_gb2312_to_ucs2_table[];
#endif

#ifdef __MMI_CHSET_GB18030__
typedef struct
{
	kal_uint16 gb_code;
	kal_uint16 ucs2_code;
}mmi_chset_gb18030_ucs2_2_byte_struct;

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

typedef struct
{
	kal_uint16 gb_code_high;
	kal_uint16 gb_code_low;
	kal_uint16 ucs2_code;
}mmi_chset_gb18030_ucs2_4_byte_struct;

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

extern const mmi_chset_gb18030_ucs2_2_byte_struct g_chset_gb18030_to_ucs2_2_byte_tbl[];
extern const mmi_chset_gb18030_ucs2_4_byte_struct g_chset_gb18030_to_ucs2_4_byte_tbl[];
extern const mmi_chset_gb18030_ucs2_2_byte_struct g_chset_ucs2_to_gb18030_2_byte_tbl[];
extern const mmi_chset_gb18030_ucs2_4_byte_struct g_chset_ucs2_to_gb18030_4_byte_tbl[];

extern kal_uint32 mmi_chset_gb18030_ucs2_2byte_size(void);
extern kal_uint32 mmi_chset_gb18030_ucs2_4byte_size(void);
extern kal_uint32 mmi_chset_ucs2_gb18030_2byte_size(void);
extern kal_uint32 mmi_chset_ucs2_gb18030_4byte_size(void);

#endif

#endif // #ifndef _MMI_CHARSETS_H


