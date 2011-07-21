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
 * Conversions.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 * This file implements utilities for encoding/decoding between two charsets
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

 
#include <csw.h>
#include "mmi.h"
#include "uim.h"
#include "adp_events.h"
#include "adp_eventsredef.h"
#include "cfw.h"   

#include "conversions.h"
#include "charsettable.h"
#include "stdc.h"
#include "mmi_trace.h"
#include "debuginitdef.h"

/* Handling UCS2 string not aligned with 2 bytes. (Obigo adopts UTF8 by default) */
#define MMI_CHSET_DBG_ASSERT(_expr)
// #define MMI_CHSET_UTF8_FAULT_TOLERANT
#define MMI_CHSET_UTF8_ALLOW_BOM
#define CAST_PU8(RAW)   ((kal_uint8*)(RAW))
#define CAST_CPU8(RAW)  ((const kal_uint8*)(RAW))
#define GET_U8_BYTE_TO_U16(RAW, i)  ((kal_uint16)(CAST_CPU8(RAW)[i]))
#define STR_SIZE(len) ((len)<<1)
#define STR_AT(RAW, n) ((void*)(CAST_PU8(RAW)+STR_SIZE(n)))
#define CHR_AT(RAW, n) ((kal_uint16)(GET_U8_BYTE_TO_U16(RAW, STR_SIZE(n))+(GET_U8_BYTE_TO_U16(RAW, STR_SIZE(n)+1)<<8)))
#define SET_CHR_AT(RAW, n, value)   do {                          \
                              kal_uint8 *_p = STR_AT(RAW, n);     \
                              kal_uint16 v= (kal_uint16) (value); \
                              _p[0] = (kal_uint8) (v & 0xff);     \
                              _p[1] = (kal_uint8) (v >> 8);       \
                           } while (0)
#define CHR(x)      ((kal_uint16)(x))

#define MERGE_TO_WCHAR(high, low)  (high << 8) + ((kal_uint8)low)

#define UTF16_HIGH_START      0xD800
#define UTF16_HIGH_END        0xDBFF
#define UTF16_LOW_START       0xDC00
#define UTF16_LOW_END         0xDFFF

static BOOL g_chset_tbl_is_init = FALSE;

const kal_uint16 g_chset_unknown_encode_char_no_space[] = 
{
    0xFFFF,
    0xFFFF
};

const kal_uint16 g_chset_unknown_encode_char_space[] = 
{
    0x20,
    0x20
};

static const kal_uint8 g_cheset_utf8_bytes_per_char[16] = 
{
    1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 2, 2, 3, 4  /* we don't support UCS4 */
};


const kal_uint8 g_chset_state[] = 
{
    1,

#ifdef __MMI_CHSET_ARABIC_ISO__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_ARABIC_WIN__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_BALTIC_ISO__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_BALTIC_WIN__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_CEURO_ISO__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_CEURO_WIN__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_GREEK_ISO__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_GREEK_WIN__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_HEBREW_ISO__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_HEBREW_WIN__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_LATIN_ISO__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_NORDIC_ISO__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_SEURO_ISO__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_TURKISH_ISO__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_TURKISH_WIN__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_WESTERN_ISO__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_WESTERN_WIN__
    1,
#else 
    0,
#endif 

    1,  /* MMI_CHSET_8_BITS_END *//* Add new (8-bit) encodings above this line */

#ifdef __MMI_CHSET_BIG5__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_GB2312__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_GB18030__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_UTF7__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_EUCKR_WIN__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_THAI_WINDOWS__
    1,
#else 
    0,
#endif 

#ifdef __MMI_CHSET_VIETNAMESE_WINDOWS__
    1,
#else 
    0,
#endif 

    /* PLace all encodings using algo to above this one */
    1,  /* MMI_CHSET_UTF16LE, */
    1,  /* MMI_CHSET_UTF16BE, */
    1,  /* MMI_CHSET_UTF8, */
    1,  /* MMI_CHSET_UCS2, */
    1,  /* MMI_CHSET_ASCII */
    1   /* MMI_CHSET_TOTAL */
};

mmi_chset_info_struct g_chset_tbl[CHSET_PAIR_TOTAL];

const kal_uint16 *g_chset_unknown_encode_char=g_chset_unknown_encode_char_no_space;
extern S32 (*pfnUnicodeStrlen)(const S8 *arrOut);

#if defined(__MMI_CHSET_BIG5__)
mmi_chset_enum g_chset_text_encoding = MMI_CHSET_BIG5;
#elif defined(__MMI_CHSET_GB2312__)
mmi_chset_enum g_chset_text_encoding = MMI_CHSET_GB2312;
#else 
mmi_chset_enum g_chset_text_encoding = MMI_CHSET_UTF8;
#endif 
/*****************************************************************************
 * FUNCTION
 *  mmi_chset_init
 * DESCRIPTION
 *  Routine for initializing the related data structures of the various encoding types
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_chset_init(void)
{
#ifdef __MMI_CHSET_BIG5__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_chset_tbl[BIG5_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[BIG5_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[BIG5_TO_UCS2].UndefChar = 0xFF1F;

    g_chset_tbl[UCS2_TO_BIG5].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_BIG5].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_BIG5].UndefChar = 0xA148;
#endif /* __MMI_CHSET_BIG5__ */ 

#ifdef __MMI_CHSET_GB2312__
    g_chset_tbl[GB2312_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[GB2312_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[GB2312_TO_UCS2].UndefChar = 0xFF1F;

    g_chset_tbl[UCS2_TO_GB2312].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_GB2312].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_GB2312].UndefChar = 0x233F;
#endif /* __MMI_CHSET_GB2312__ */ 

#ifdef __MMI_CHSET_TURKISH_ISO__
    g_chset_tbl[TURKISH_ISO_TO_UCS2].pConversnTable = &g_chset_turkish_ucs2_iso[0];
    g_chset_tbl[TURKISH_ISO_TO_UCS2].pMappingTable = &g_chset_map_turkish_ucs2_iso[0];
    g_chset_tbl[TURKISH_ISO_TO_UCS2].TotalMapTbIndex = mmi_chset_map_turkish_ucs2_iso_size();
    g_chset_tbl[TURKISH_ISO_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[TURKISH_ISO_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[TURKISH_ISO_TO_UCS2].UndefChar = 0x003F;

    g_chset_tbl[UCS2_TO_TURKISH_ISO].pConversnTable = &g_chset_ucs2_turkish_iso[0];
    g_chset_tbl[UCS2_TO_TURKISH_ISO].pMappingTable = &g_chset_map_ucs2_turkish_iso[0];
    g_chset_tbl[UCS2_TO_TURKISH_ISO].TotalMapTbIndex = mmi_chset_map_ucs2_turkish_iso_size();
    g_chset_tbl[UCS2_TO_TURKISH_ISO].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_TURKISH_ISO].Output_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[UCS2_TO_TURKISH_ISO].UndefChar = 0x3F;
#endif /* __MMI_CHSET_TURKISH_ISO__ */ 

#ifdef __MMI_CHSET_TURKISH_WIN__
    g_chset_tbl[TURKISH_WINDOWS_TO_UCS2].pConversnTable = &g_chset_turkish_ucs2_win[0];
    g_chset_tbl[TURKISH_WINDOWS_TO_UCS2].pMappingTable = &g_chset_map_turkish_ucs2_win[0];
    g_chset_tbl[TURKISH_WINDOWS_TO_UCS2].TotalMapTbIndex = mmi_chset_map_turkish_ucs2_win_size();
    g_chset_tbl[TURKISH_WINDOWS_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[TURKISH_WINDOWS_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[TURKISH_WINDOWS_TO_UCS2].UndefChar = 0x003F;

    g_chset_tbl[UCS2_TO_TURKISH_WINDOWS].pConversnTable = &g_chset_ucs2_turkish_win[0];
    g_chset_tbl[UCS2_TO_TURKISH_WINDOWS].pMappingTable = &g_chset_map_ucs2_turkish_win[0];
    g_chset_tbl[UCS2_TO_TURKISH_WINDOWS].TotalMapTbIndex = mmi_chset_map_ucs2_turkish_win_size();
    g_chset_tbl[UCS2_TO_TURKISH_WINDOWS].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_TURKISH_WINDOWS].Output_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[UCS2_TO_TURKISH_WINDOWS].UndefChar = 0x3F;
#endif /* __MMI_CHSET_TURKISH_WIN__ */ 

#ifdef __MMI_CHSET_BALTIC_ISO__
    g_chset_tbl[BALTIC_ISO_TO_UCS2].pConversnTable = &g_chset_baltic_ucs2_iso[0];
    g_chset_tbl[BALTIC_ISO_TO_UCS2].pMappingTable = &g_chset_map_ucs2_common[0];
    g_chset_tbl[BALTIC_ISO_TO_UCS2].TotalMapTbIndex = mmi_chset_map_ucs2_common_size();
    g_chset_tbl[BALTIC_ISO_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[BALTIC_ISO_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[BALTIC_ISO_TO_UCS2].UndefChar = 0x003F;

    g_chset_tbl[UCS2_TO_BALTIC_ISO].pConversnTable = &g_chset_ucs2_baltic_iso[0];
    g_chset_tbl[UCS2_TO_BALTIC_ISO].pMappingTable = &g_chset_map_ucs2_baltic_iso[0];
    g_chset_tbl[UCS2_TO_BALTIC_ISO].TotalMapTbIndex = mmi_chset_map_ucs2_baltic_iso_size();
    g_chset_tbl[UCS2_TO_BALTIC_ISO].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_BALTIC_ISO].Output_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[UCS2_TO_BALTIC_ISO].UndefChar = 0x3F;
#endif /* __MMI_CHSET_BALTIC_ISO__ */ 

#ifdef __MMI_CHSET_BALTIC_WIN__
    g_chset_tbl[BALTIC_WINDOWS_TO_UCS2].pConversnTable = &g_chset_baltic_ucs2_win[0];
    g_chset_tbl[BALTIC_WINDOWS_TO_UCS2].pMappingTable = &g_chset_map_ucs2_common[0];
    g_chset_tbl[BALTIC_WINDOWS_TO_UCS2].TotalMapTbIndex = mmi_chset_map_ucs2_common_size();
    g_chset_tbl[BALTIC_WINDOWS_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[BALTIC_WINDOWS_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[BALTIC_WINDOWS_TO_UCS2].UndefChar = 0x003F;

    g_chset_tbl[UCS2_TO_BALTIC_WINDOWS].pConversnTable = &g_chset_ucs2_baltic_win[0];
    g_chset_tbl[UCS2_TO_BALTIC_WINDOWS].pMappingTable = &g_chset_map_ucs2_baltic_win[0];
    g_chset_tbl[UCS2_TO_BALTIC_WINDOWS].TotalMapTbIndex = mmi_chset_map_baltic_ucs2_win_size();
    g_chset_tbl[UCS2_TO_BALTIC_WINDOWS].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_BALTIC_WINDOWS].Output_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[UCS2_TO_BALTIC_WINDOWS].UndefChar = 0x3F;
#endif /* __MMI_CHSET_BALTIC_WIN__ */ 

#ifdef __MMI_CHSET_CEURO_ISO__
    g_chset_tbl[CENTRAL_EUROPEAN_ISO_TO_UCS2].pConversnTable = &g_chset_ceuro_ucs2_iso[0];
    g_chset_tbl[CENTRAL_EUROPEAN_ISO_TO_UCS2].pMappingTable = &g_chset_map_ucs2_common[0];
    g_chset_tbl[CENTRAL_EUROPEAN_ISO_TO_UCS2].TotalMapTbIndex = mmi_chset_map_ucs2_common_size();
    g_chset_tbl[CENTRAL_EUROPEAN_ISO_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[CENTRAL_EUROPEAN_ISO_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[CENTRAL_EUROPEAN_ISO_TO_UCS2].UndefChar = 0x003F;

    g_chset_tbl[UCS2_TO_CENTRAL_EUROPEAN_ISO].pConversnTable = &g_chset_ucs2_ceuro_iso[0];
    g_chset_tbl[UCS2_TO_CENTRAL_EUROPEAN_ISO].pMappingTable = &g_chset_map_ucs2_ceuro_iso[0];
    g_chset_tbl[UCS2_TO_CENTRAL_EUROPEAN_ISO].TotalMapTbIndex = mmi_chset_map_ucs2_ceuro_iso_size();
    g_chset_tbl[UCS2_TO_CENTRAL_EUROPEAN_ISO].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_CENTRAL_EUROPEAN_ISO].Output_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[UCS2_TO_CENTRAL_EUROPEAN_ISO].UndefChar = 0x3F;
#endif /* __MMI_CHSET_CEURO_ISO__ */ 

#ifdef __MMI_CHSET_CEURO_WIN__
    g_chset_tbl[CENTRAL_EUROPEAN_WINDOWS_TO_UCS2].pConversnTable = &g_chset_ceuro_ucs2_win[0];
    g_chset_tbl[CENTRAL_EUROPEAN_WINDOWS_TO_UCS2].pMappingTable = &g_chset_map_ucs2_common[0];
    g_chset_tbl[CENTRAL_EUROPEAN_WINDOWS_TO_UCS2].TotalMapTbIndex = mmi_chset_map_ucs2_common_size();
    g_chset_tbl[CENTRAL_EUROPEAN_WINDOWS_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[CENTRAL_EUROPEAN_WINDOWS_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[CENTRAL_EUROPEAN_WINDOWS_TO_UCS2].UndefChar = 0x003F;

    g_chset_tbl[UCS2_TO_CENTRAL_EUROPEAN_WINDOWS].pConversnTable = &g_chset_ucs2_ceuro_win[0];
    g_chset_tbl[UCS2_TO_CENTRAL_EUROPEAN_WINDOWS].pMappingTable = &g_chset_map_ucs2_ceuro_win[0];
    g_chset_tbl[UCS2_TO_CENTRAL_EUROPEAN_WINDOWS].TotalMapTbIndex = mmi_chset_map_ucs2_ceuro_win_size();
    g_chset_tbl[UCS2_TO_CENTRAL_EUROPEAN_WINDOWS].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_CENTRAL_EUROPEAN_WINDOWS].Output_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[UCS2_TO_CENTRAL_EUROPEAN_WINDOWS].UndefChar = 0x3F;
#endif /* __MMI_CHSET_CEURO_WIN__ */ 

#ifdef __MMI_CHSET_GREEK_ISO__
    g_chset_tbl[GREEK_ISO_TO_UCS2].pConversnTable = &g_chset_greek_ucs2_iso[0];
    g_chset_tbl[GREEK_ISO_TO_UCS2].pMappingTable = &g_chset_map_ucs2_common[0];
    g_chset_tbl[GREEK_ISO_TO_UCS2].TotalMapTbIndex = mmi_chset_map_ucs2_common_size();
    g_chset_tbl[GREEK_ISO_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[GREEK_ISO_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[GREEK_ISO_TO_UCS2].UndefChar = 0x003F;

    g_chset_tbl[UCS2_TO_GREEK_ISO].pConversnTable = &g_chset_ucs2_greek_iso[0];
    g_chset_tbl[UCS2_TO_GREEK_ISO].pMappingTable = &g_chset_map_ucs2_greek_iso[0];
    g_chset_tbl[UCS2_TO_GREEK_ISO].TotalMapTbIndex = mmi_chset_map_ucs2_greek_iso_size();
    g_chset_tbl[UCS2_TO_GREEK_ISO].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_GREEK_ISO].Output_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[UCS2_TO_GREEK_ISO].UndefChar = 0x3F;
#endif /* __MMI_CHSET_GREEK_ISO__ */ 

#ifdef __MMI_CHSET_GREEK_WIN__
    g_chset_tbl[GREEK_WINDOWS_TO_UCS2].pConversnTable = &g_chset_greek_ucs2_win[0];
    g_chset_tbl[GREEK_WINDOWS_TO_UCS2].pMappingTable = &g_chset_map_ucs2_common[0];
    g_chset_tbl[GREEK_WINDOWS_TO_UCS2].TotalMapTbIndex = mmi_chset_map_ucs2_common_size();
    g_chset_tbl[GREEK_WINDOWS_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[GREEK_WINDOWS_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[GREEK_WINDOWS_TO_UCS2].UndefChar = 0x003F;

    g_chset_tbl[UCS2_TO_GREEK_WINDOWS].pConversnTable = &g_chset_ucs2_greek_win[0];
    g_chset_tbl[UCS2_TO_GREEK_WINDOWS].pMappingTable = &g_chset_map_ucs2_greek_win[0];
    g_chset_tbl[UCS2_TO_GREEK_WINDOWS].TotalMapTbIndex = mmi_chset_map_ucs2_greek_win_size();
    g_chset_tbl[UCS2_TO_GREEK_WINDOWS].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_GREEK_WINDOWS].Output_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[UCS2_TO_GREEK_WINDOWS].UndefChar = 0x3F;
#endif /* __MMI_CHSET_GREEK_WIN__ */ 

#ifdef __MMI_CHSET_HEBREW_ISO__
    g_chset_tbl[HEBREW_ISO_TO_UCS2].pConversnTable = &g_chset_hebrew_ucs2_iso[0];
    g_chset_tbl[HEBREW_ISO_TO_UCS2].pMappingTable = &g_chset_map_ucs2_common[0];
    g_chset_tbl[HEBREW_ISO_TO_UCS2].TotalMapTbIndex = mmi_chset_map_ucs2_common_size();
    g_chset_tbl[HEBREW_ISO_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[HEBREW_ISO_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[HEBREW_ISO_TO_UCS2].UndefChar = 0x003F;

    g_chset_tbl[UCS2_TO_HEBREW_ISO].pConversnTable = &g_chset_ucs2_hebrew_iso[0];
    g_chset_tbl[UCS2_TO_HEBREW_ISO].pMappingTable = &g_chset_map_ucs2_hebrew_iso[0];
    g_chset_tbl[UCS2_TO_HEBREW_ISO].TotalMapTbIndex = mmi_chset_map_ucs2_hebrew_iso_size();
    g_chset_tbl[UCS2_TO_HEBREW_ISO].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_HEBREW_ISO].Output_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[UCS2_TO_HEBREW_ISO].UndefChar = 0x3F;
#endif /* __MMI_CHSET_HEBREW_ISO__ */ 

#ifdef __MMI_CHSET_LATIN_ISO__
    g_chset_tbl[LATIN_ISO_TO_UCS2].pConversnTable = &g_chset_latin_ucs2_iso[0];
    g_chset_tbl[LATIN_ISO_TO_UCS2].pMappingTable = &g_chset_map_ucs2_common[0];
    g_chset_tbl[LATIN_ISO_TO_UCS2].TotalMapTbIndex = mmi_chset_map_ucs2_common_size();
    g_chset_tbl[LATIN_ISO_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[LATIN_ISO_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[LATIN_ISO_TO_UCS2].UndefChar = 0x003F;

    g_chset_tbl[UCS2_TO_LATIN_ISO].pConversnTable = &g_chset_ucs2_latin_iso[0];
    g_chset_tbl[UCS2_TO_LATIN_ISO].pMappingTable = &g_chset_map_ucs2_latin_iso[0];
    g_chset_tbl[UCS2_TO_LATIN_ISO].TotalMapTbIndex = mmi_chset_map_ucs2_latin_iso_size();
    g_chset_tbl[UCS2_TO_LATIN_ISO].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_LATIN_ISO].Output_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[UCS2_TO_LATIN_ISO].UndefChar = 0x3F;
#endif /* __MMI_CHSET_LATIN_ISO__ */ 

#ifdef __MMI_CHSET_NORDIC_ISO__
    g_chset_tbl[NORDIC_ISO_TO_UCS2].pConversnTable = &g_chset_nordic_ucs2_iso[0];
    g_chset_tbl[NORDIC_ISO_TO_UCS2].pMappingTable = &g_chset_map_ucs2_common[0];
    g_chset_tbl[NORDIC_ISO_TO_UCS2].TotalMapTbIndex = mmi_chset_map_ucs2_common_size();
    g_chset_tbl[NORDIC_ISO_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[NORDIC_ISO_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[NORDIC_ISO_TO_UCS2].UndefChar = 0x003F;

    g_chset_tbl[UCS2_TO_NORDIC_ISO].pConversnTable = &g_chset_ucs2_nordic_iso[0];
    g_chset_tbl[UCS2_TO_NORDIC_ISO].pMappingTable = &g_chset_map_ucs2_nordic_iso[0];
    g_chset_tbl[UCS2_TO_NORDIC_ISO].TotalMapTbIndex = mmi_chset_map_ucs2_nordic_iso_size();
    g_chset_tbl[UCS2_TO_NORDIC_ISO].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_NORDIC_ISO].Output_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[UCS2_TO_NORDIC_ISO].UndefChar = 0x3F;
#endif /* __MMI_CHSET_NORDIC_ISO__ */ 

#ifdef __MMI_CHSET_SEURO_ISO__
    g_chset_tbl[SOUTH_EUROPEAN_ISO_TO_UCS2].pConversnTable = &g_chset_seuro_ucs2_iso[0];
    g_chset_tbl[SOUTH_EUROPEAN_ISO_TO_UCS2].pMappingTable = &g_chset_map_ucs2_common[0];
    g_chset_tbl[SOUTH_EUROPEAN_ISO_TO_UCS2].TotalMapTbIndex = mmi_chset_map_ucs2_common_size();
    g_chset_tbl[SOUTH_EUROPEAN_ISO_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[SOUTH_EUROPEAN_ISO_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[SOUTH_EUROPEAN_ISO_TO_UCS2].UndefChar = 0x003F;

    g_chset_tbl[UCS2_TO_SOUTH_EUROPEAN_ISO].pConversnTable = &g_chset_ucs2_seuro_iso[0];
    g_chset_tbl[UCS2_TO_SOUTH_EUROPEAN_ISO].pMappingTable = &g_chset_map_ucs2_seuro_iso[0];
    g_chset_tbl[UCS2_TO_SOUTH_EUROPEAN_ISO].TotalMapTbIndex = mmi_chset_map_ucs2_seuro_iso_size();
    g_chset_tbl[UCS2_TO_SOUTH_EUROPEAN_ISO].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_SOUTH_EUROPEAN_ISO].Output_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[UCS2_TO_SOUTH_EUROPEAN_ISO].UndefChar = 0x3F;
#endif /* __MMI_CHSET_SEURO_ISO__ */ 

#ifdef __MMI_CHSET_WESTERN_ISO__
    g_chset_tbl[WESTERN_ISO_TO_UCS2].pConversnTable = &g_chset_western_ucs2_iso[0];
    g_chset_tbl[WESTERN_ISO_TO_UCS2].pMappingTable = &g_chset_map_ucs2_common[0];
    g_chset_tbl[WESTERN_ISO_TO_UCS2].TotalMapTbIndex = mmi_chset_map_ucs2_common_size();
    g_chset_tbl[WESTERN_ISO_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[WESTERN_ISO_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[WESTERN_ISO_TO_UCS2].UndefChar = 0x003F;

    g_chset_tbl[UCS2_TO_WESTERN_ISO].pConversnTable = &g_chset_ucs2_western_iso[0];
    g_chset_tbl[UCS2_TO_WESTERN_ISO].pMappingTable = &g_chset_map_ucs2_western_iso[0];
    g_chset_tbl[UCS2_TO_WESTERN_ISO].TotalMapTbIndex = mmi_chset_map_ucs2_western_iso_size();
    g_chset_tbl[UCS2_TO_WESTERN_ISO].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_WESTERN_ISO].Output_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[UCS2_TO_WESTERN_ISO].UndefChar = 0x3F;
#endif /* __MMI_CHSET_WESTERN_ISO__ */ 

#ifdef __MMI_CHSET_WESTERN_WIN__
   // g_chset_tbl[WESTERN_WINDOWS_TO_UCS2].pConversnTable = 0;//&g_chset_western_ucs2_win[0];
  //  g_chset_tbl[WESTERN_WINDOWS_TO_UCS2].pMappingTable = 0;//&g_chset_map_ucs2_common[0];
  //  g_chset_tbl[WESTERN_WINDOWS_TO_UCS2].TotalMapTbIndex = 0;//mmi_chset_map_ucs2_common_size();
    g_chset_tbl[WESTERN_WINDOWS_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[WESTERN_WINDOWS_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[WESTERN_WINDOWS_TO_UCS2].UndefChar = 0x003F;

   // g_chset_tbl[UCS2_TO_WESTERN_WINDOWS].pConversnTable =0;// &g_chset_ucs2_western_win[0];
  //  g_chset_tbl[UCS2_TO_WESTERN_WINDOWS].pMappingTable = 0;//&g_chset_map_ucs2_western_win[0];
  //  g_chset_tbl[UCS2_TO_WESTERN_WINDOWS].TotalMapTbIndex =0;// mmi_chset_map_western_ucs2_win_size();
    g_chset_tbl[UCS2_TO_WESTERN_WINDOWS].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_WESTERN_WINDOWS].Output_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[UCS2_TO_WESTERN_WINDOWS].UndefChar = 0x3F;
#endif /* __MMI_CHSET_WESTERN_WIN__ */ 

#ifdef __MMI_CHSET_ARABIC_ISO__
    g_chset_tbl[ARABIC_ISO_TO_UCS2].pConversnTable = &g_chset_arabic_ucs2_iso[0];
    g_chset_tbl[ARABIC_ISO_TO_UCS2].pMappingTable = &g_chset_map_arabic_ucs2_iso[0];
    g_chset_tbl[ARABIC_ISO_TO_UCS2].TotalMapTbIndex = mmi_chset_map_arabic_ucs2_iso_size();
    g_chset_tbl[ARABIC_ISO_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[ARABIC_ISO_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[ARABIC_ISO_TO_UCS2].UndefChar = 0x003F;

    g_chset_tbl[UCS2_TO_ARABIC_ISO].pConversnTable = &g_chset_ucs2_arabic_iso[0];
    g_chset_tbl[UCS2_TO_ARABIC_ISO].pMappingTable = &g_chset_map_ucs2_arabic_iso[0];
    g_chset_tbl[UCS2_TO_ARABIC_ISO].TotalMapTbIndex = mmi_chset_map_ucs2_arabic_iso_size();
    g_chset_tbl[UCS2_TO_ARABIC_ISO].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_ARABIC_ISO].Output_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[UCS2_TO_ARABIC_ISO].UndefChar = 0x3F;
#endif /* __MMI_CHSET_ARABIC_ISO__ */ 

#ifdef __MMI_CHSET_ARABIC_WIN__
    g_chset_tbl[ARABIC_WINDOWS_TO_UCS2].pConversnTable = &g_chset_arabic_ucs2_win[0];
    g_chset_tbl[ARABIC_WINDOWS_TO_UCS2].pMappingTable = &g_chset_map_ucs2_common[0];
    g_chset_tbl[ARABIC_WINDOWS_TO_UCS2].TotalMapTbIndex = mmi_chset_map_ucs2_common_size();
    g_chset_tbl[ARABIC_WINDOWS_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[ARABIC_WINDOWS_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[ARABIC_WINDOWS_TO_UCS2].UndefChar = 0x003F;

    g_chset_tbl[UCS2_TO_ARABIC_WINDOWS].pConversnTable = &g_chset_ucs2_arabic_win[0];
    g_chset_tbl[UCS2_TO_ARABIC_WINDOWS].pMappingTable = &g_chset_map_ucs2_arabic_win[0];
    g_chset_tbl[UCS2_TO_ARABIC_WINDOWS].TotalMapTbIndex = mmi_chset_map_ucs2_arabic_win_size();
    g_chset_tbl[UCS2_TO_ARABIC_WINDOWS].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_ARABIC_WINDOWS].Output_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[UCS2_TO_ARABIC_WINDOWS].UndefChar = 0x3F;
#endif /* __MMI_CHSET_ARABIC_WIN__ */ 

#ifdef __MMI_CHSET_HEBREW_WIN__
    g_chset_tbl[HEBREW_WINDOWS_TO_UCS2].pConversnTable = &g_chset_hebrew_ucs2_win[0];
    g_chset_tbl[HEBREW_WINDOWS_TO_UCS2].pMappingTable = &g_chset_map_ucs2_common[0];
    g_chset_tbl[HEBREW_WINDOWS_TO_UCS2].TotalMapTbIndex = mmi_chset_map_ucs2_common_size();
    g_chset_tbl[HEBREW_WINDOWS_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[HEBREW_WINDOWS_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[HEBREW_WINDOWS_TO_UCS2].UndefChar = 0x003F;

    g_chset_tbl[UCS2_TO_HEBREW_WINDOWS].pConversnTable = &g_chset_ucs2_hebrew_win[0];
    g_chset_tbl[UCS2_TO_HEBREW_WINDOWS].pMappingTable = &g_chset_map_ucs2_hebrew_win[0];
    g_chset_tbl[UCS2_TO_HEBREW_WINDOWS].TotalMapTbIndex = mmi_chset_map_ucs2_hebrew_win_size();
    g_chset_tbl[UCS2_TO_HEBREW_WINDOWS].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_HEBREW_WINDOWS].Output_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[UCS2_TO_HEBREW_WINDOWS].UndefChar = 0x3F;
#endif /* __MMI_CHSET_HEBREW_WIN__ */ 

#ifdef __MMI_CHSET_EUCKR_WIN__
    g_chset_tbl[EUCKR_WINDOWS_TO_UCS2].pConversnTable = &g_chset_euckr_ucs2_win[0];
    g_chset_tbl[EUCKR_WINDOWS_TO_UCS2].pMappingTable = &g_chset_map_euckr_ucs2_win[0];
    g_chset_tbl[EUCKR_WINDOWS_TO_UCS2].TotalMapTbIndex = g_chset_map_euckr_ucs2_win_size();
    g_chset_tbl[EUCKR_WINDOWS_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[EUCKR_WINDOWS_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[EUCKR_WINDOWS_TO_UCS2].UndefChar = 0x003F;
    g_chset_tbl[UCS2_TO_EUCKR_WINDOWS].pConversnTable = &g_chset_ucs2_euckr_win[0];
    g_chset_tbl[UCS2_TO_EUCKR_WINDOWS].pMappingTable = &g_chset_map_ucs2_euckr_win[0];
    g_chset_tbl[UCS2_TO_EUCKR_WINDOWS].TotalMapTbIndex = mmi_chset_map_ucs2_euckr_win_size();
    g_chset_tbl[UCS2_TO_EUCKR_WINDOWS].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_EUCKR_WINDOWS].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_EUCKR_WINDOWS].UndefChar = 0x003F;
#endif /* __MMI_CHSET_EUCKR_WIN__ */ 
#ifdef __MMI_CHSET_THAI_WINDOWS__
    g_chset_tbl[THAI_WINDOWS_TO_UCS2].pConversnTable = &g_chset_thai_ucs2_win[0];
    g_chset_tbl[THAI_WINDOWS_TO_UCS2].pMappingTable = &g_chset_map_thai_ucs2_win[0];
    g_chset_tbl[THAI_WINDOWS_TO_UCS2].TotalMapTbIndex = g_chset_map_thai_ucs2_win_size();
    g_chset_tbl[THAI_WINDOWS_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[THAI_WINDOWS_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[THAI_WINDOWS_TO_UCS2].UndefChar = 0x003F;
    g_chset_tbl[UCS2_TO_THAI_WINDOWS].pConversnTable = &g_chset_ucs2_thai_win[0];
    g_chset_tbl[UCS2_TO_THAI_WINDOWS].pMappingTable = &g_chset_map_ucs2_thai_win[0];
    g_chset_tbl[UCS2_TO_THAI_WINDOWS].TotalMapTbIndex = mmi_chset_map_ucs2_thai_win_size();
    g_chset_tbl[UCS2_TO_THAI_WINDOWS].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_THAI_WINDOWS].Output_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[UCS2_TO_THAI_WINDOWS].UndefChar = 0x3F;
#endif /* __MMI_CHSET_THAI_WINDOWS__ */ 
#ifdef __MMI_CHSET_VIETNAMESE_WINDOWS__
    g_chset_tbl[VIETNAMESE_WINDOWS_TO_UCS2].pConversnTable = &g_chset_vietnamese_ucs2_win[0];
    g_chset_tbl[VIETNAMESE_WINDOWS_TO_UCS2].pMappingTable = &g_chset_map_vietnamese_ucs2_win[0];
    g_chset_tbl[VIETNAMESE_WINDOWS_TO_UCS2].TotalMapTbIndex = g_chset_map_vietnamese_ucs2_win_size();
    g_chset_tbl[VIETNAMESE_WINDOWS_TO_UCS2].Input_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[VIETNAMESE_WINDOWS_TO_UCS2].Output_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[VIETNAMESE_WINDOWS_TO_UCS2].UndefChar = 0x003F;
    g_chset_tbl[UCS2_TO_VIETNAMESE_WINDOWS].pConversnTable = &g_chset_ucs2_vietnamese_win[0];
    g_chset_tbl[UCS2_TO_VIETNAMESE_WINDOWS].pMappingTable = &g_chset_map_ucs2_vietnamese_win[0];
    g_chset_tbl[UCS2_TO_VIETNAMESE_WINDOWS].TotalMapTbIndex = mmi_chset_map_ucs2_vietnamese_win_size();
    g_chset_tbl[UCS2_TO_VIETNAMESE_WINDOWS].Input_word_bits = CHSET_BIT_WORD_16;
    g_chset_tbl[UCS2_TO_VIETNAMESE_WINDOWS].Output_word_bits = CHSET_BIT_WORD_8;
    g_chset_tbl[UCS2_TO_VIETNAMESE_WINDOWS].UndefChar = 0x3F;
#endif /* __MMI_CHSET_VIETNAMESE_WINDOWS__ */ 
    g_chset_tbl_is_init = TRUE;
}
/*****************************************************************************
 * FUNCTION
 *  mmi_chset_utf8_to_ucs2
 * DESCRIPTION
 *  
 * PARAMETERS
 *  dest        [OUT]     
 *  utf8        [IN]     
 * RETURNS
 *  
 *****************************************************************************/
kal_int32 mmi_chset_utf8_to_ucs2(kal_uint8 *dest, kal_uint8 *utf8)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_uint8 c = utf8[0];
    int count = g_cheset_utf8_bytes_per_char[c >> 4];
    kal_uint16 ucs2 = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (count)
    {
        case 1:
            ucs2 = c;
            break;
        case 2:
            ucs2 = ((kal_uint16) (c & 0x1f) << 6) | (kal_uint16) (utf8[1] ^ 0x80);
            break;
        case 3:
            ucs2 = ((kal_uint16) (c & 0x0f) << 12)
                | ((kal_uint16) (utf8[1] ^ 0x80) << 6) | (kal_uint16) (utf8[2] ^ 0x80);
            break;
        case 4:
            return count;
        default:
            return 1;   /* the character cannot be converted, return 1 means always consume 1 byte */
    }

    SET_CHR_AT(dest, 0, ucs2);

    return count;
}



/*****************************************************************************
 * FUNCTION
 *  mmi_chset_ucs2_to_utf8
 * DESCRIPTION
 *  
 * PARAMETERS
 *  utf8        [OUT]         
 *  ucs2        [IN]        
 * RETURNS
 *  
 *****************************************************************************/
kal_int32 mmi_chset_ucs2_to_utf8(kal_uint8 *utf8, kal_uint16 ucs2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int count;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (ucs2 < (kal_uint16) 0x80)
    {
        count = 1;
    }
    else if (ucs2 < (kal_uint16) 0x800)
    {
        count = 2;
    }
    else
    {
        count = 3;
    }
    switch (count)
    {
        case 3:
            utf8[2] = 0x80 | (ucs2 & 0x3f);
            ucs2 = ucs2 >> 6;
            ucs2 |= 0x800;
        case 2:
            utf8[1] = 0x80 | (ucs2 & 0x3f);
            ucs2 = ucs2 >> 6;
            ucs2 |= 0xc0;
        case 1:
            utf8[0] = (kal_uint8) ucs2;
    }
    return count;
}

#ifdef MMI_CHSET_UTF8_ALLOW_BOM

/*****************************************************************************
 * FUNCTION
 *  mmi_chset_is_utf8_BOM
 * DESCRIPTION
 *  
 * PARAMETERS
 *  str     [IN]        
 * RETURNS
 *  
 *****************************************************************************/
static __inline kal_bool mmi_chset_is_utf8_BOM(const kal_uint8 *str)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (str[0] == 0xEF && str[1] == 0xBB && str[2] == 0xBF)
    {
        return KAL_TRUE;
    }
    else
    {
        return KAL_FALSE;
    }
}
#endif

/*****************************************************************************
 * FUNCTION
 *  mmi_chset_utf8_to_ucs2_string
 * DESCRIPTION
 *  
 * PARAMETERS
 *  dest            [OUT]         
 *  dest_size       [IN]        
 *  src             [IN]         
 * RETURNS
 *  
 *****************************************************************************/
kal_int32 mmi_chset_utf8_to_ucs2_string(kal_uint8 *dest, kal_int32 dest_size, kal_uint8 *src)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int pos = 0;
    int cnt;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef MMI_CHSET_UTF8_ALLOW_BOM
    if (strlen((const char*)src) >= 3 && mmi_chset_is_utf8_BOM(src))
    {
        src += 3;
    }
#endif /* MMI_CHSET_UTF8_ALLOW_BOM */ 

    MMI_CHSET_DBG_ASSERT((dest_size % 2) == 0);
    dest_size -= (dest_size % 2);

    if (dest_size < 2)
    {
        MMI_CHSET_DBG_ASSERT(0);    /* dest wont be NULL-terminated */
        return 0;
    }

    while (*src && pos < dest_size - 2)
    {
        cnt = mmi_chset_utf8_to_ucs2(dest + pos, src);
        if (cnt == 0)   /* abnormal case */
        {
        #ifdef MMI_CHSET_UTF8_FAULT_TOLERANT
            src++;
        #else 
            break;
        #endif 
        }
        else if (cnt == 4)
        {
            src += cnt;
        }
        else    /* normal case */
        {
            src += cnt;
            pos += 2;
        }
    }
    dest[pos] = 0;
    dest[pos + 1] = 0;
    return pos + 2;
}
/*****************************************************************************
 * FUNCTION
 *  mmi_chset_ucs2_to_utf8_len
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ucs2        [IN]        
 * RETURNS
 *  
 *****************************************************************************/
static __inline int mmi_chset_ucs2_to_utf8_len(kal_uint16 ucs2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (ucs2 < (kal_uint16) 0x80)
    {
        return 1;
    }
    else if (ucs2 < (kal_uint16) 0x800)
    {
        return 2;
    }
    else
    {
        return 3;
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_chset_ucs2_to_utf8_string
 * DESCRIPTION
 *  
 * PARAMETERS
 *  dest            [OUT]         
 *  dest_size       [IN]        
 *  src             [IN]         
 * RETURNS
 *  
 *****************************************************************************/
kal_int32 mmi_chset_ucs2_to_utf8_string(kal_uint8 *dest, kal_int32 dest_size, kal_uint8 *src)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int pos = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (src[0] || src[1])
    {
        if (pos + mmi_chset_ucs2_to_utf8_len(CHR_AT(src, 0)) >= dest_size)      /* leave space of '\0' */
        {
            MMI_CHSET_DBG_ASSERT(0);
            break;
        }

        pos += mmi_chset_ucs2_to_utf8(dest + pos, CHR_AT(src, 0));
        src += 2;
        if (pos >= dest_size - 1)
        {
            break;
        }
    }
    if (pos >= dest_size)
    {
        MMI_CHSET_DBG_ASSERT(0);
        dest[dest_size - 1] = 0;
    }
    else
    {
        dest[pos] = 0;
    }
    return pos + 1;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_chset_utf8_strlen
 * DESCRIPTION
 *  
 * PARAMETERS
 *  s       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
kal_int32 mmi_chset_utf8_strlen(const kal_uint8 *s)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int count = 0;
    int rawlen;
    int d;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (s == NULL)
    {
        MMI_CHSET_DBG_ASSERT(0);
        return 0;
    }

    rawlen = strlen((const char*)s);

#ifdef MMI_CHSET_UTF8_ALLOW_BOM
    if (rawlen >= 3 && mmi_chset_is_utf8_BOM(s))
    {
        s += 3;
        rawlen -= 3;
    }
#endif /* MMI_CHSET_UTF8_ALLOW_BOM */ 

    while (*s != '\0' && rawlen > 0)
    {
        d = g_cheset_utf8_bytes_per_char[*s >> 4];
        if (d == 0)
        {
        #ifdef MMI_CHSET_UTF8_FAULT_TOLERANT
            /* skip this character */
            rawlen--;
            s++;
            continue;
        #else /* MMI_CHSET_UTF8_FAULT_TOLERANT */ 
            MMI_CHSET_DBG_ASSERT(0);
            return count;
        #endif /* MMI_CHSET_UTF8_FAULT_TOLERANT */ 
        }
        if (d > rawlen)
        {
            return count;
        }

        rawlen -= d;
        s += d;

        count++;
    }

    MMI_CHSET_DBG_ASSERT(rawlen == 0);

    return count;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_chset_text_to_ucs2
 * DESCRIPTION
 *  
 * PARAMETERS
 *  dest            [OUT]         
 *  dest_size       [IN]        
 *  src             [IN]         
 * RETURNS
 *  
 *****************************************************************************/
kal_int32 mmi_chset_text_to_ucs2(kal_uint8 *dest, kal_int32 dest_size, kal_uint8 *src)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return mmi_chset_convert(g_chset_text_encoding, MMI_CHSET_UCS2, (char*)src, (char*)dest, dest_size);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_chset_dont_use_space_for_unknown_character
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
void mmi_chset_do_not_use_space_for_unknown_character(void)
{
#ifdef __MMI_CHSET_IDEOGRAM_SUPPORT__
    g_chset_unknown_encode_char=g_chset_unknown_encode_char_no_space;
#endif
}

void mmi_chset_use_space_for_unknown_character(void)
{
#ifdef __MMI_CHSET_IDEOGRAM_SUPPORT__
    g_chset_unknown_encode_char=g_chset_unknown_encode_char_space;
#endif
}
/*****************************************************************************
 * FUNCTION
 *  mmi_chset_utf8_to_ucs2_string_ex
 * DESCRIPTION
 *  
 * PARAMETERS
 *  dest            [OUT]       
 *  dest_size       [IN]        
 *  src             [IN]        
 *  src_end_pos     [OUT]       
 * RETURNS
 *  
 *****************************************************************************/
kal_int32 mmi_chset_utf8_to_ucs2_string_ex(
            kal_uint8 *dest,
            kal_int32 dest_size,
            kal_uint8 *src,
            kal_uint32 *src_end_pos)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int pos = 0;
    int cnt;
    int src_len = strlen((const char*)src);

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef MMI_CHSET_UTF8_ALLOW_BOM
    if (src_len >= 3 && mmi_chset_is_utf8_BOM(src))
    {
        src += 3;
        src_len -= 3;
    }
#endif /* MMI_CHSET_UTF8_ALLOW_BOM */ 

    MMI_CHSET_DBG_ASSERT((dest_size % 2) == 0);
    dest_size -= (dest_size % 2);
    *src_end_pos = (kal_uint32) src; /* set src_end_pos first */

    if (dest_size < 2)
    {
        MMI_CHSET_DBG_ASSERT(0);    /* dest wont be NULL-terminated */
        return 0;
    }

    while (*src && pos < dest_size - 2)
    {
        cnt = mmi_chset_utf8_to_ucs2(dest + pos, src);
         if (((kal_uint32)src - (*src_end_pos)) >= (src_len - cnt) &&
             (*(dest + pos) == 0xFF && *(dest + pos + 1) == 0xFF) &&
            !(*src == 0xEF && *(src+1) == 0xBF && *(src+2) == 0xBF))
        {
            /* 
             * If src isn't 0xEF, 0xBF, 0xBF and its remain length is not enough but dest is 0xFFFF, we will abort the process.
             * dest data is invalid character because src data is not enough to convert 
             */
            break;
        }
        if (cnt == 0)   /* abnormal case */
        {
        #ifdef MMI_CHSET_UTF8_FAULT_TOLERANT
            src++;
        #else 
            break;
        #endif 
        }        
        else    /* normal case */
        {
            src += cnt;
            pos += 2;
        }
    }
    *src_end_pos = (kal_uint32) src;
    dest[pos] = 0;
    dest[pos + 1] = 0;
    return pos + 2;
}
/*****************************************************************************
 * FUNCTION
 *  mmi_chset_ucs2_to_utf8_string_ex
 * DESCRIPTION
 *  
 * PARAMETERS
 *  dest            [OUT]       
 *  dest_size       [IN]        
 *  src             [IN]        
 *  src_end_pos     [OUT]       
 * RETURNS
 *  
 *****************************************************************************/
kal_int32 mmi_chset_ucs2_to_utf8_string_ex(
            kal_uint8 *dest,
            kal_int32 dest_size,
            kal_uint8 *src,
            kal_uint32 *src_end_pos)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int pos = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (src[0] || src[1])
    {
        if (pos + mmi_chset_ucs2_to_utf8_len(CHR_AT(src, 0)) >= dest_size)      /* leave space of '\0' */
        {
            MMI_CHSET_DBG_ASSERT(0);
            break;
        }

        pos += mmi_chset_ucs2_to_utf8(dest + pos, CHR_AT(src, 0));
        src += 2;
        if (pos >= dest_size - 1)
        {
            break;
        }
    }
    if (pos >= dest_size)
    {
        MMI_CHSET_DBG_ASSERT(0);
        dest[dest_size - 1] = 0;
    }
    else
    {
        dest[pos] = 0;
    }
    *src_end_pos = (kal_uint32) src;
    return pos + 1;
}
/*****************************************************************************
 * FUNCTION
 *  mmi_chset_text_to_ucs2_str_ex
 * DESCRIPTION
 *  
 * PARAMETERS
 *  dest            [OUT]       
 *  dest_size       [IN]        
 *  src             [IN]        
 *  encoding        [IN]        
 *  src_end_pos     [OUT]       
 * RETURNS
 *  
 *****************************************************************************/
kal_uint16 mmi_chset_text_to_ucs2_str_ex(
            kal_uint8 *dest,
            kal_int32 dest_size,
            kal_uint8 *src,
            TextEncodingEnum encoding,
            kal_uint32 *src_end_pos)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    kal_int16 pos = 0;
    kal_uint8 *uOut=NULL;
    kal_uint32 uOutLen = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    
    while ((*(src) != 0)&&(*(src+1) != 0)) /* I do not know how we assume that this is end of data.. Currently we are taking only chinese encodings for
                           the optimized algo so we can take it as 2 bytes */
    {
        if (*(src) <= 0x7f)
        {
            *(dest + pos) = *src;
            pos++;
            *(dest + pos) = 0;
            pos++;
            src += 1;
        }
        else
        {
            ML_LocalLanguage2Unicode((const UINT8*)src,  2, &uOut, (UINT32*)&uOutLen,  ML_CP936);
            
            if(uOut&&uOutLen)
            {

                if(uOutLen==2)
                {
			if(uOut[0] == 0 && uOut [1] == 0)
			{
				mmi_trace(0, "ERROR:mmi_chset_text_to_ucs2_str_ex(%d) src[0] is 0x%x, src[1] is 0x%x",__LINE__, *src, *(src+1));
				uOut[0] = 0x20;
			}
                    dest[pos] = uOut[1];
                    dest[pos + 1] = uOut[0];
                    src += 2;
                    pos += 2;
		    
                }
                if(uOutLen == 4&&pos <= dest_size - 6)
                {
			if(uOut[0] == 0 && uOut [1] == 0)
			{
				mmi_trace(0, "ERROR:mmi_chset_text_to_ucs2_str_ex(%d) src[0] is 0x%x, src[1] is 0x%x",__LINE__, *src, *(src+1));
				uOut[0] = 0x20;
			}
			if(uOut[2] == 0 && uOut [3] == 0)
			{
				mmi_trace(0, "ERROR:mmi_chset_text_to_ucs2_str_ex(%d) src[2] is 0x%x, src[3] is 0x%x",__LINE__, *(src+2), *(src+3));
				uOut[2] = 0x20;
			}

                    dest[pos] = uOut[1];
                    dest[pos + 1] = uOut[0];
                    dest[pos+2] = uOut[3];
                    dest[pos + 3] = uOut[2];
                    src += 2;
                    pos += 4;
                }
                else if( uOutLen == 4&&pos >= dest_size - 4 )
                {
			if(uOut[0] == 0 && uOut [1] == 0)
			{
				mmi_trace(0, "ERROR:mmi_chset_text_to_ucs2_str_ex(%d) src[0] is 0x%x, src[1] is 0x%x",__LINE__, *src, *(src+1));
				uOut[0] = 0x20;
			}
                    dest[pos] = uOut[1];
                    dest[pos + 1] = uOut[0];
                    src += 2;
                    pos += 2;
                }

                
                COS_Free( uOut);
                uOut = NULL;
                uOutLen = 0;
            }
            else
            {
            //skip this charactor
                src += 2;
            }
        }
        if (pos >= dest_size - 2)
        {
            break;
        }
    }
    if(src_end_pos)
    *src_end_pos = (kal_uint32) src;
    dest[pos] = 0;
    dest[pos + 1] = 0;
    return pos + 2;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_chset_convert
 * DESCRIPTION
 *  Convert string between 2 character sets.
 * PARAMETERS
 *  src_type        [IN]        Charset type of source
 *  dest_type       [IN]        Charset type of destination
 *  src_buff        [IN]        Buffer stores source string
 *  dest_buff       [OUT]       Buffer stores destination string
 *  dest_size       [IN]        Size of destination buffer (bytes)
 * RETURNS
 *  Length of destination string, including null terminator. (bytes)
 *****************************************************************************/
kal_int32 mmi_chset_convert(
            mmi_chset_enum src_type,
            mmi_chset_enum dest_type,
            char *src_buff,
            char *dest_buff,
            kal_int32 dest_size)
{
   //translate to UNICODE form src type
    UINT8* uOut = NULL,*dOut = NULL;
    UINT32 uOutLen=0,dOutLen=0;
   
    UINT32 inLen = 0;

    if(src_type==dest_type )
    {
        memcpy(dest_buff,src_buff,dest_size);
        return dest_size;
    } 

    if ((src_type == MMI_CHSET_UTF8)&&(dest_type ==MMI_CHSET_UCS2 ))
    {
        return (kal_uint16) mmi_chset_utf8_to_ucs2_string(
                                (kal_uint8*) dest_buff,
                                dest_size,
                                (kal_uint8*) src_buff);
    }
    else if ((src_type == MMI_CHSET_UCS2)&&(dest_type == MMI_CHSET_UTF8))
    {
        return (kal_uint16) mmi_chset_ucs2_to_utf8_string(
                                (kal_uint8*) dest_buff,
                                dest_size,
                                (kal_uint8*) src_buff);
    }
    else
    {
        inLen = pfnUnicodeStrlen(src_buff )*ENCODING_LENGTH;
        //just covert to ucs2 str
        switch(src_type )
        {
            case MMI_CHSET_BIG5:
            case MMI_CHSET_GB2312:
            case MMI_CHSET_GB18030:
                  return  mmi_chset_text_to_ucs2_str_ex( dest_buff, 
                    dest_size, src_buff, 
                    0, 
                     NULL);
               // ML_LocalLanguage2Unicode((const UINT8*)src_buff,  inLen, &uOut, &uOutLen, ML_CP936);
                break;
            case MMI_CHSET_ARABIC_ISO:
            case  MMI_CHSET_ARABIC_WIN:
                ML_LocalLanguage2Unicode((const UINT8*)src_buff,  inLen, &uOut, &uOutLen, ML_ISO8859_6);
                break;
            case MMI_CHSET_THAI_WIN:
                ML_LocalLanguage2Unicode((const UINT8*)src_buff,  inLen, &uOut, &uOutLen, ML_CP874);
                break;
            case MMI_CHSET_VIETNAMESE_WIN:
                ML_LocalLanguage2Unicode((const UINT8*)src_buff,  inLen, &uOut, &uOutLen, ML_CP1258);
                break;
            default:
                ML_LocalLanguage2Unicode((const UINT8*)src_buff,  inLen, &uOut, &uOutLen, NULL);
                break;
        }

    //make sure the range not break

        if( (dest_type >= CHSET_UTF16LE)&&(dest_type <=MMI_CHSET_UCS2   ) )
         {
               if(uOutLen>dest_size )
                    memcpy(dest_buff,uOut,dest_size);
               else
                    memcpy(dest_buff,uOut,uOutLen);

                OslMfree( uOut);

                return (uOutLen>dest_size)?dest_size:uOutLen;
        }
           //translate to dest type
        switch(dest_type )
        {
            case MMI_CHSET_BIG5:
            case MMI_CHSET_GB2312:
            case MMI_CHSET_GB18030:
            ML_Unicode2LocalLanguage((const UINT8 * )uOut,  uOutLen, &dOut, &dOutLen , ML_CP936);
                break;
            case MMI_CHSET_ARABIC_ISO:
            case  MMI_CHSET_ARABIC_WIN:
            ML_Unicode2LocalLanguage((const UINT8 * )uOut,  uOutLen, &dOut, &dOutLen , ML_ISO8859_6);
                break;
            case MMI_CHSET_THAI_WIN:
            ML_Unicode2LocalLanguage((const UINT8 * )uOut,  uOutLen, &dOut, &dOutLen , ML_CP874);
                break;
            case MMI_CHSET_VIETNAMESE_WIN:
            ML_Unicode2LocalLanguage((const UINT8 * )uOut,  uOutLen, &dOut, &dOutLen , ML_CP1258);
                break;
            default:
            ML_Unicode2LocalLanguage((const UINT8 * )uOut,  uOutLen, &dOut, &dOutLen , NULL);
                break;
        }
       //output

       if(dOutLen>dest_size )
            memcpy(dest_buff,dOut,dest_size);
       else
            memcpy(dest_buff,dOut,dOutLen);

        OslMfree( uOut);
        OslMfree( dOut);

        return (dOutLen>dest_size)?dest_size:dOutLen;
    
    }

//return 0;
}
/*****************************************************************************
 * FUNCTION
 *  mmi_chset_copy_to_dest
 * DESCRIPTION
 *  When the source charset type equals to dest charset type, copy source to dest directly
 * PARAMETERS
 *  src_type        [IN]        Charset type of source
 *  src_buff        [IN]        Buffer stores source string
 *  dest_buff       [OUT]       Buffer stores destination string
 *  dest_size       [IN]        Size of destination buffer (bytes)
 *  src_end_pos     [OUT]       
 * RETURNS
 *  Bytes copied
 *****************************************************************************/
kal_int32 mmi_chset_copy_to_dest(
            mmi_chset_enum src_type,
            char *src_buff,
            char *dest_buff,
            kal_int32 dest_size,
            kal_uint32 *src_end_pos)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_uint8 WordBytes = 0;
    mmi_chset_pair_enum Scheme;
    kal_int32 count = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (src_type == MMI_CHSET_UCS2)
    {
        WordBytes = CHSET_BIT_WORD_16;
    }
    else
    {
        Scheme = (mmi_chset_pair_enum) (BASE_ENCODING_SCHEME + (src_type - MMI_CHSET_BASE) * 2 - 1);
        WordBytes = g_chset_tbl[Scheme].Input_word_bits;
    }

    if (WordBytes == CHSET_BIT_WORD_16)
    {
        for (count = 0; src_buff[count] != 0 || src_buff[count + 1] != 0; count = count + 2)
        {
            if (count >= dest_size - 2)
            {
                count = (kal_uint16) dest_size - 2;
                break;
            }
            dest_buff[count] = src_buff[count];
            dest_buff[count + 1] = src_buff[count + 1];
        }
        *src_end_pos = (kal_uint32) (src_buff + count);
        dest_buff[count] = 0;
        dest_buff[++count] = 0;
        return count + 1;
    }
    else if(WordBytes == CHSET_BIT_WORD_8)
    {
        for (count = 0; src_buff[count] != 0; count++)
        {
            if (count >= dest_size - 1)
            {
                count = (kal_uint16) dest_size - 1;
                break;
            }
            dest_buff[count] = src_buff[count];
        }
        *src_end_pos = (kal_uint32) (src_buff + count);
        dest_buff[count] = 0;
        return count + 1;
    }
    else
    {
        return 0;
    }
}
static void mmi_chset_simple_convert(
			 kal_int8 *dest,
			 kal_int32 dest_size,
			 kal_int8 *src,
                         kal_uint32 *src_end_pos)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    while ((*src != 0) && (dest_size > 2))
    {
        if (*src < 0x7F)
        {
	    *dest++ = *src++;
	    *dest++ = 0x00;
	}
	else
	{
	    *dest++ = 0x3F;
	    *dest++ = 0x00;
	}
        
        dest_size -= 2;
    }

    *src_end_pos = (kal_uint32) src;
    
    *dest++ = 0x00;
    *dest = 0x00;

}

/*****************************************************************************
 * FUNCTION
 *  mmi_chset_get_utf_byte_order
 * DESCRIPTION
 
 * PARAMETERS
 
 * RETURNS
 *  returns MMI_CHSET_UTF16LE if the stream is little-endian
 *  returns MMI_CHSET_UTF16BE if the stream is big-endian
 *  returns MMI_CHSET_UTF8 if the input BOM is a UTF-8's BOM
 *  Otherwise returns -1
 *****************************************************************************/
kal_int8 mmi_chset_get_utf_byte_order(const kal_int8 *str_bom)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_wchar wc;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wc = MERGE_TO_WCHAR(str_bom[0], str_bom[1]);

    switch (wc)
    {
        case 0xFFFE:
            return MMI_CHSET_UTF16LE;

        case 0xFEFF:
            return MMI_CHSET_UTF16BE;

        case 0xEFBB:
            if ((signed char)0xBF == str_bom[2])
                return MMI_CHSET_UTF8;
            else
                return -1;

        default:
            return -1;
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_chset_utf16_to_ucs2_string
 * DESCRIPTION
 *  Converts text encoded using the Unicode transformation format UTF-16
 *  into the Unicode UCS-2 character set.
 * PARAMETERS
 *  dest_ucs2       [OUT]       Destination buffer. On return, contains the Unicode encoded output string.
 *  dest_size       [IN]        The size of Destination buffer.
 *  src_utf16       [IN]        The UTF-16 encoded input string.
 *  utf16_type      [IN]        The type of input UTF-16 encoded string. MMI_CHSET_UTF16LE or MMI_CHSET_UTF16BE
 *  src_end_pos     [OUT]       The point to the end of converted sub-string at the input string.
 * RETURNS
 *  The number of converted bytes, including the final null wide-characters.
 *****************************************************************************/
static kal_int32 mmi_chset_utf16_to_ucs2_string(
                    kal_int8 *dest_ucs2,
                    kal_int32 dest_size,
                    kal_int8 *src_utf16,
                    mmi_chset_enum utf16_type,
                    kal_uint32 *src_end_pos)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_int8 utf16_bo;
    kal_int32 count = 0;
    kal_wchar wc1;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    utf16_bo = mmi_chset_get_utf_byte_order(src_utf16);

    if (-1 != utf16_bo)
    {
        src_utf16 += 2;
        /* needn't convert the beginning BOM */
    }
    else
    {
        utf16_bo = utf16_type;
    }

    while ((src_utf16[0] || src_utf16[1]) && (count < (dest_size - 2)))
    {
        wc1 = ((MMI_CHSET_UTF16BE == utf16_bo) ?
               MERGE_TO_WCHAR(src_utf16[0], src_utf16[1]) : MERGE_TO_WCHAR(src_utf16[1], src_utf16[0]));

        /* we ignore surrogate pair */
        if (((wc1 >= UTF16_HIGH_START) && (wc1 <= UTF16_HIGH_END)) ||
            ((wc1 >= UTF16_LOW_START) && (wc1 <= UTF16_LOW_END)))
        {
            src_utf16 += 2;
        }
        else
        {
            /* normal case */
            dest_ucs2[0] = wc1 & 0xFF;
            dest_ucs2[1] = wc1 >> 8;
            src_utf16 += 2;
            dest_ucs2 += 2;
            count += 2;
        }
    }

    dest_ucs2[0] = '\0';
    dest_ucs2[1] = '\0';

    *src_end_pos = (kal_uint32) src_utf16;
    return (count + 2);

}

/*****************************************************************************
 * FUNCTION
 *  mmi_chset_ucs2_to_utf16_string
 * DESCRIPTION
 *  Converts Unicode text into UTF-16 encoding.
 * PARAMETERS
 *  dest_utf16      [OUT]       Destination buffer. On return, contains the Unicode encoded output string.
 *  dest_size       [IN]        The size of Destination buffer.
 *  utf16_type      [IN]        The type of input UTF-16 encoded string. MMI_CHSET_UTF16LE or MMI_CHSET_UTF16BE
 
 *  src_ucs2        [IN]        The UTF-16 encoded input string.
 *  src_end_pos     [OUT]       The point to the end of converted sub-string at the input string.
 * RETURNS
 *  The number of converted bytes, including the final null wide-characters.
 *****************************************************************************/
static kal_int32 mmi_chset_ucs2_to_utf16_string(
                    kal_int8 *dest_utf16,
                    kal_int32 dest_size,
                    mmi_chset_enum utf16_type,
                    kal_bool include_bom,
                    kal_int8 *src_ucs2,
                    kal_uint32 *src_end_pos)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_int32 count = 0;
    kal_wchar wc;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    if (include_bom)
    {
        if (MMI_CHSET_UTF16BE == utf16_type)
        {
            dest_utf16[0] = (kal_int8) 0xFF;
            dest_utf16[1] = (kal_int8) 0xFE;
        }
        else
        {
            dest_utf16[0] = (kal_int8) 0xFE;
            dest_utf16[1] = (kal_int8) 0xFF;
        }
        dest_utf16 += 2;
        dest_size -= 2;
        count += 2;
    }

    while ((src_ucs2[0] || src_ucs2[1]) && (count < (dest_size - 2)))
    {
        wc = MERGE_TO_WCHAR(src_ucs2[1], src_ucs2[0]);

        /* UTF-16 surrogate values are illegal in UCS2; 
           0xffff or 0xfffe are both reserved values */
        if (!(wc >= UTF16_HIGH_START && wc <= UTF16_LOW_END))
        {
            if (MMI_CHSET_UTF16LE == utf16_type)
            {
                dest_utf16[0] = src_ucs2[0];
                dest_utf16[1] = src_ucs2[1];
            }
            else
            {
                dest_utf16[0] = src_ucs2[1];
                dest_utf16[1] = src_ucs2[0];
            }
            dest_utf16 += 2;
            count += 2;
        }
        src_ucs2 += 2;
    }

    dest_utf16[0] = '\0';
    dest_utf16[1] = '\0';

    *src_end_pos = (kal_uint32) src_ucs2;
    return (count + 2);

}

/*****************************************************************************
 * FUNCTION
 *  EncodingDecodingEx
 * DESCRIPTION
 *  This function encodes the input string to desired encoding type.
 * PARAMETERS
 *  Scheme          [IN]        
 *  pOutBuffer      [OUT]       
 *  pInBuffer       [IN]        
 *  dest_size       [IN]        
 *  src_end_pos     [OUT]       
 * RETURNS
 *  number of bytes in output string
 *****************************************************************************/
kal_uint16 EncodingDecodingEx(
            mmi_chset_pair_enum Scheme,
            char *pOutBuffer,
            char *pInBuffer,
            kal_int32 dest_size,
            kal_uint32 *src_end_pos)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (Scheme == UTF8_TO_UCS2)
    {
        return (kal_uint16) mmi_chset_utf8_to_ucs2_string_ex(
                                (kal_uint8*) pOutBuffer,
                                dest_size,
                                (kal_uint8*) pInBuffer,
                                src_end_pos);
    }
    else if (Scheme == UCS2_TO_UTF8)
    {
        return (kal_uint16) mmi_chset_ucs2_to_utf8_string_ex(
                                (kal_uint8*) pOutBuffer,
                                dest_size,
                                (kal_uint8*) pInBuffer,
                                src_end_pos);
    }
    else
    {
        return   mmi_chset_text_to_ucs2_str_ex(pOutBuffer, dest_size,  pInBuffer, 0,  src_end_pos);

    }
//chinese charset use csw translation
#if 0
#ifdef __MMI_CHSET_BIG5__
    else if (BIG5_TO_UCS2 <= Scheme && Scheme <= UCS2_TO_BIG5)  /* Patch added to use different algorithm for chinese encodings */
    {
        return mmi_chset_encode_decode_algo2_ex(Scheme, pOutBuffer, pInBuffer, dest_size, src_end_pos);    /* Length is hardcode for now to maximum */
    }
#endif /* __MMI_CHSET_BIG5__ */ 
#ifdef __MMI_CHSET_GB2312__
    else if (GB2312_TO_UCS2 <= Scheme && Scheme <= UCS2_TO_GB2312)      /* Patch added to use different algorithm for chinese encodings */
    {
        return mmi_chset_encode_decode_algo2_ex(Scheme, pOutBuffer, pInBuffer, dest_size, src_end_pos);    /* Length is hardcode for now to maximum */
    }
#endif /* __MMI_CHSET_GB2312__ */ 

#ifdef __MMI_CHSET_EUCKR_WIN__
    else if (Scheme == EUCKR_WINDOWS_TO_UCS2)
    {
        return mmi_chset_encode_decode_algo3(Scheme, pOutBuffer, pInBuffer, dest_size, src_end_pos);
    }
#endif /* __MMI_CHSET_EUCKR_WIN__ */ 
#ifdef __MMI_CHSET_EUCKR_WIN__
    else if (Scheme == UCS2_TO_EUCKR_WINDOWS)
    {
        return mmi_chset_encode_decode_algo3(Scheme, pOutBuffer, pInBuffer, dest_size, src_end_pos);
    }
#endif /* __MMI_CHSET_EUCKR_WIN__ */ 

#ifdef __MMI_CHSET_ALPHABET_SUPPORT__
    else    /* For all of the rest encodings. */
    {
        return mmi_chset_encode_decode_ex(Scheme, pOutBuffer, pInBuffer, dest_size, src_end_pos);
    }
#else /* __MMI_CHSET_ALPHABET_SUPPORT__ */ 
    else
    {
        return 0;
    }
#endif /* __MMI_CHSET_ALPHABET_SUPPORT__ */ 
#endif
}
/*****************************************************************************
 * FUNCTION
 *  mmi_chset_convert_ex
 * DESCRIPTION
 *  Convert string between 2 character sets. (will add the terminate character)
 * PARAMETERS
 *  src_type        [IN]        Charset type of source
 *  dest_type       [IN]        Charset type of destination
 *  src_buff        [IN]        Buffer stores source string
 *  dest_buff       [OUT]       Buffer stores destination string
 *  dest_size       [IN]        Size of destination buffer (bytes)
 *  src_end_pos     [OUT]       
 * RETURNS
 *  Length of destination string, including null terminator. (bytes)
 *****************************************************************************/
kal_int32 mmi_chset_convert_ex(
            mmi_chset_enum src_type,
            mmi_chset_enum dest_type,
            char *src_buff,
            char *dest_buff,
            kal_int32 dest_size,
            kal_uint32 *src_end_pos)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* unsupported charset */
    *src_end_pos = (kal_uint32) src_buff;
    
    if (g_chset_tbl_is_init == FALSE)
    {
        mmi_chset_init();
    }

    if (src_type >= MMI_CHSET_TOTAL || dest_type >= MMI_CHSET_TOTAL)
    {
        return 0;
    }

    /* If src_type == dest_type */
    if (src_type == dest_type)
    {
        return mmi_chset_copy_to_dest(src_type, src_buff, dest_buff, dest_size, src_end_pos);
    }

    if (g_chset_state[src_type] == 0)
    {
        if (src_type <= MMI_CHSET_WESTERN_WIN)
	{
	    mmi_chset_simple_convert((kal_int8 *)dest_buff, dest_size, (kal_int8 *)src_buff, src_end_pos);			
	}

        return 0;
    }

    if (g_chset_state[dest_type] == 0)
    {
        return 0;
    }
    if (src_type == MMI_CHSET_GB2312 && dest_type == MMI_CHSET_UCS2)
    {
                return EncodingDecodingEx(
                    (mmi_chset_pair_enum) (BASE_ENCODING_SCHEME + (dest_type - MMI_CHSET_BASE) * 2),
                    dest_buff,
                    src_buff,
                    dest_size,
                    src_end_pos);
    }
    else    if (src_type == MMI_CHSET_UCS2 && dest_type == MMI_CHSET_UTF8)
    {
        return EncodingDecodingEx(UCS2_TO_UTF8, dest_buff, src_buff, dest_size, src_end_pos);
    }
    else if (src_type == MMI_CHSET_UTF8 && dest_type == MMI_CHSET_UCS2)
    {
        return EncodingDecodingEx(UTF8_TO_UCS2, dest_buff, src_buff, dest_size, src_end_pos);
    }
    /* UTF-16 */
    else if (src_type == MMI_CHSET_UTF16LE && dest_type == MMI_CHSET_UCS2)
    {
        return mmi_chset_utf16_to_ucs2_string(
                (kal_int8*) dest_buff,
                dest_size,
                (kal_int8*) src_buff,
                MMI_CHSET_UTF16LE,
                src_end_pos);
    }
    else if (src_type == MMI_CHSET_UCS2 && dest_type == MMI_CHSET_UTF16LE)
    {
        return mmi_chset_ucs2_to_utf16_string(
                (kal_int8*) dest_buff,
                dest_size,
                MMI_CHSET_UTF16LE,
                KAL_FALSE,
                (kal_int8*) src_buff,
                src_end_pos);
    }
    else if (src_type == MMI_CHSET_UTF16BE && dest_type == MMI_CHSET_UCS2)
    {
        return mmi_chset_utf16_to_ucs2_string(
                (kal_int8*) dest_buff,
                dest_size,
                (kal_int8*) src_buff,
                MMI_CHSET_UTF16BE,
                src_end_pos);
    }
    else if (src_type == MMI_CHSET_UCS2 && dest_type == MMI_CHSET_UTF16BE)
    {
        return mmi_chset_ucs2_to_utf16_string(
                (kal_int8*) dest_buff,
                dest_size,
                MMI_CHSET_UTF16BE,
                KAL_FALSE,
                (kal_int8*) src_buff,
                src_end_pos);
    }
#ifdef __MMI_CHSET_UTF7__
    else if (src_type == MMI_CHSET_UCS2 && dest_type == MMI_CHSET_UTF7)
    {
        return mmi_chset_ucs2_to_utf7_str(dest_buff, src_buff, dest_size, src_end_pos);
    }

    else if (src_type == MMI_CHSET_UTF7 && dest_type == MMI_CHSET_UCS2)
    {
        return mmi_chset_utf7_to_ucs2_str(dest_buff, src_buff, dest_size, src_end_pos);
    }
#endif /* __MMI_CHSET_UTF7__ */ 
#ifdef __MMI_CHSET_EUCKR_WIN__
    else if (src_type == MMI_CHSET_UCS2 && dest_type == MMI_CHSET_EUCKR)
    {
        return EncodingDecoding(UCS2_TO_EUCKR_WINDOWS, dest_buff, src_buff, dest_size, src_end_pos);
    }

    else if (src_type == MMI_CHSET_EUCKR && dest_type == MMI_CHSET_UCS2)
    {
        return EncodingDecoding(EUCKR_WINDOWS_TO_UCS2, dest_buff, src_buff, dest_size, src_end_pos);
    }
#endif /* __MMI_CHSET_EUCKR_WIN__ */ 
#ifdef __MMI_CHSET_THAI_WINDOWS__
    else if (src_type == MMI_CHSET_UCS2 && dest_type == MMI_CHSET_THAI_WIN)
    {
        return EncodingDecoding(UCS2_TO_THAI_WINDOWS, dest_buff, src_buff, dest_size, src_end_pos);
    }

    else if (src_type == MMI_CHSET_THAI_WIN && dest_type == MMI_CHSET_UCS2)
    {
        return EncodingDecoding(THAI_WINDOWS_TO_UCS2, dest_buff, src_buff, dest_size, src_end_pos);
    }
#endif /* __MMI_CHSET_THAI_WINDOWS__ */ 
#ifdef __MMI_CHSET_VIETNAMESE_WINDOWS__
    else if (src_type == MMI_CHSET_UCS2 && dest_type == MMI_CHSET_VIETNAMESE_WIN)
    {
        return EncodingDecoding(UCS2_TO_VIETNAMESE_WINDOWS, dest_buff, src_buff, dest_size, src_end_pos);
    }

    else if (src_type == MMI_CHSET_VIETNAMESE_WIN && dest_type == MMI_CHSET_UCS2)
    {
        return EncodingDecoding(VIETNAMESE_WINDOWS_TO_UCS2, dest_buff, src_buff, dest_size, src_end_pos);
    }
#endif /* __MMI_CHSET_VIETNAMESE_WINDOWS__ */ 
#ifdef __MMI_CHSET_KOI8_R__
    else if (src_type == MMI_CHSET_UCS2 && dest_type == MMI_CHSET_KOI8_R)
    {
        return EncodingDecoding(UCS2_TO_KOI8_R, dest_buff, src_buff, dest_size, src_end_pos);
    }
	
    else if (src_type == MMI_CHSET_KOI8_R && dest_type == MMI_CHSET_UCS2)
    {
        return EncodingDecoding(KOI8_R_TO_UCS2, dest_buff, src_buff, dest_size, src_end_pos);
    }
#endif /* __MMI_CHSET_KOI8_R__ */
#ifdef __MMI_CHSET_TIS_620__
    else if (src_type == MMI_CHSET_UCS2 && dest_type == MMI_CHSET_TIS_620)
    {
        return EncodingDecoding(UCS2_TO_TIS_620, dest_buff, src_buff, dest_size, src_end_pos);
    }
	
    else if (src_type == MMI_CHSET_TIS_620 && dest_type == MMI_CHSET_UCS2)
    {
        return EncodingDecoding(TIS_620_TO_UCS2, dest_buff, src_buff, dest_size, src_end_pos);
    }
#endif /* __MMI_CHSET_TIS_620__ */

#ifdef __MMI_CHSET_GB18030__
    else if (src_type == MMI_CHSET_GB18030 && dest_type == MMI_CHSET_UCS2)
    {
        return mmi_chset_gb18030_to_ucs2_str(dest_buff, src_buff, dest_size, src_end_pos);
    }
    else if (src_type == MMI_CHSET_UCS2 && dest_type == MMI_CHSET_GB18030)
    {
        return mmi_chset_ucs2_to_gb18030_str(dest_buff, src_buff, dest_size, src_end_pos);
    }
#endif /* __MMI_CHSET_GB18030__ */ 
#if defined(__MMI_CHSET_ALPHABET_SUPPORT__) || defined(__MMI_CHSET_IDEOGRAM_SUPPORT__)
    else    /* intermedia conversion required */
    {
        if (src_type == MMI_CHSET_UCS2)
        {
            return EncodingDecodingEx(
                    (mmi_chset_pair_enum) (BASE_ENCODING_SCHEME + (dest_type - MMI_CHSET_BASE) * 2),
                    dest_buff,
                    src_buff,
                    dest_size,
                    src_end_pos);
        }
        else if (dest_type == MMI_CHSET_UCS2)
        {
            return EncodingDecodingEx(
                    (mmi_chset_pair_enum) (BASE_ENCODING_SCHEME + (src_type - MMI_CHSET_BASE) * 2 - 1),
                    dest_buff,
                    src_buff,
                    dest_size,
                    src_end_pos);
        }
        else
        {
            kal_uint16 result_len;

            char *tmp_buff = OslMalloc(CHSET_TMP_BUFF_SIZE);

            EncodingDecodingEx(
                (mmi_chset_pair_enum) (BASE_ENCODING_SCHEME + (src_type - MMI_CHSET_BASE) * 2 - 1),
                tmp_buff,
                src_buff,
                CHSET_TMP_BUFF_SIZE,
                src_end_pos);

            result_len = EncodingDecodingEx(
                            (mmi_chset_pair_enum) (BASE_ENCODING_SCHEME + (dest_type - MMI_CHSET_BASE) * 2),
                            dest_buff,
                            tmp_buff,
                            dest_size,
                            src_end_pos);

            OslMfree(tmp_buff);
            return result_len;
        }
    }
#else /* defined(__MMI_CHSET_ALPHABET_SUPPORT__) || defined(__MMI_CHSET_IDEOGRAM_SUPPORT__) */ 
    return 0;
#endif /* defined(__MMI_CHSET_ALPHABET_SUPPORT__) || defined(__MMI_CHSET_IDEOGRAM_SUPPORT__) */ 

}
/*****************************************************************************
 * FUNCTION
 *  mmi_chset_mixed_text_to_ucs2_str
 * DESCRIPTION
 *  
 * PARAMETERS
 *  dest            [OUT]         
 *  dest_size       [IN]        
 *  src             [IN]         
 *  encoding        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/

void mmi_chset_mixed_text_to_ucs2_str(kal_uint8 *dest, kal_int16 dest_size, kal_uint8 *src, mmi_chset_enum encoding)
{
    UINT8* out;
    UINT32 outLen;
    UINT32 inLen;

    inLen = pfnUnicodeStrlen(src )*ENCODING_LENGTH;

//just covert to ucs2 str
    ML_LocalLanguage2Unicode((const UINT8*)src,  inLen, &out, &outLen, ML_CP936);

    if(out )
    {
	    if(outLen >dest_size )
	        memcpy(dest,out,dest_size);
	    else
	        memcpy(dest,out,outLen);

	    OslMfree( out);
    }

}





