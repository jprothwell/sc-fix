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
*	text_encoding.h
*
* Project:
* --------
 
*
* Description:
* ------------
*   Text encoding header
*
* Author:
* -------
 
*
*==============================================================================
* 				HISTORY
 
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/custom/app/include/text_encoding.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 10:52:58   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:33:46   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:19:28   zhangxb
 * 
 *------------------------------------------------------------------------------
 
*==============================================================================
*******************************************************************************/

#ifndef _TEXT_ENCODING_H
#define _TEXT_ENCODING_H

//#define __TEXT_ENCODING_ENABLED__	1

#define UNKOWN_UCS2_CHAR	0xFFEB
#define WCHAR_TO_UCS2(x) WCharToUCS2( (x), g_text_encoding )
#define UCS2_TO_WCHAR(x) UCS2ToWChar( (x), g_text_encoding )

typedef enum
{
	BIG_5_ENCODING,
	GB2312_ENCODING,
	NO_OF_TEXT_ENCODING
} TextEncodingEnum;

typedef struct
{
	short start;
	short end;
} key_index_t;

/*-- function prototypes --*/
char Is_Text_Encoding_Enabled( void );
unsigned short WCharToUCS2( unsigned short encode_char, TextEncodingEnum encoding );
unsigned short UCS2ToWChar( unsigned short ucs2_char, TextEncodingEnum encoding );

void TextToUCS2Str( unsigned char *dest, short dest_size, unsigned char *src, TextEncodingEnum encoding );
void MixedTextToUCS2Str( unsigned char *dest, short dest_size, unsigned char *src, TextEncodingEnum encoding );
unsigned char IsTextInStr( unsigned char *src );

#ifdef __TEXT_ENCODING_ENABLED__
/*-- exposed global data --*/
extern const key_index_t *g_ucs2_to_encode_key_LSB_index[NO_OF_TEXT_ENCODING];
extern const key_index_t *g_encode_to_ucs2_key_LSB_index[NO_OF_TEXT_ENCODING];
extern const unsigned char *g_ucs2_to_encode_key_MSB[NO_OF_TEXT_ENCODING];
extern const unsigned char *g_encode_to_ucs2_key_MSB[NO_OF_TEXT_ENCODING];
extern const unsigned short *g_ucs2_to_encode_table[NO_OF_TEXT_ENCODING];
extern const unsigned short *g_encode_to_ucs2_table[NO_OF_TEXT_ENCODING];
extern const unsigned short g_unknown_encode_char[NO_OF_TEXT_ENCODING];
extern const unsigned short *g_text_encoding_name[NO_OF_TEXT_ENCODING];
#endif // __TEXT_ENCODING_ENABLED__
extern TextEncodingEnum g_text_encoding; // this value will be updated by MMI phone setup module

#endif // _TEXT_ENCODING_H
