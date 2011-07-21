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
 *  PixcomFontEngine.h
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
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: PixcomFontengine.h

  	PURPOSE		: Fontengine header

 

 

	DATE		: May 25, 2002

**************************************************************/

#include "cswtype.h"
#include "mmi_data_types.h" //simon add
#ifndef _PIXCOMFONTENGINE_H
#define _PIXCOMFONTENGINE_H

#ifdef __cplusplus
extern "C"
{
#endif

//to check the control chars
/*add 0x0A,0x0D~0x0F for displaying control characters as space*/
/*add 0xA0 for NBSP in Vietnamese as space*/
#define FONT_TEST_CONTROL_CHAR(c)		\
(		(c=='\t'/*0x09*/)					\
	||	(c==0x08)					\
	||	(c==0x0A)					\
	||	(c==0x0D)					\
	||	(c==0x0E)					\
	||	(c==0x0F)					\
	||	(c==0xA0)					\
)

#define FONT_TEST_FORMAT_CHAR(c)	\
(	(c >= 0x200C && c <= 0x200F)\
	||(c >= 0x2028 && c <= 0x202F)\
	||(c == 0x2060)\
)

//for R2L characters
#define MAX_SHOW_STRING_TEXT_LENGTH       	700
#define MAX_NUM_OF_CHAR_FOR_EACH_LINE         120  //wangzl: 090416 change from 80 to 120 for bug 15538


#define FONT_TEST_BIDI_CONTROL_CHAR(c)	((c==0x06)||(c==0x07)||(c==0x08)||(c==0x09)||(c==0x0A)||(c==0x0D))
#define FONT_TEST_WORD_WRAPPING_BREAK_CHARACTER(c) (c == 0x20 ? 1:0)
#define FONT_TEST_ASCII_CHARACTER(c)	(((UI_character_type)(c)<=(UI_character_type)0xFF)?(1):(0))



typedef struct stFontAttribute
{
	U8 bold;
	U8 italic;
	U8 underline;
	U8 size;
	U8 color_t;
	U8 type;
	U8 oblique;
	U8 smallCaps;
}
stFontAttribute;

//for R2L characters
typedef struct stL2RLangSSC 
{
	S8 *sscString;
} L2RLangSSC;



// --------------------------- Public Functions and Variables -------------------------

void TestDiffFonts(void);

// -------------------------- Private Functions and Variables -------------------------

//static U32 GetFontdata(U32 Ch, stFontAttribute Font, U8 *CharData);
//static void SetTilt(void);

/*	Public functions													  */


void	Get_CharWidthHeight(U32 Ch,S32* pWidth, S32* pHeight);
void	Get_CharBoundingBox(U32 Ch, S32 *pCharWidth, S32 *pCharHeight, S32 *pBoxXoffset, S32 *pBoxYoffset, S32 *pBoxWidth, S32 *pBoxHeight);
U32 	Get_CharNumInWidth(U8 *String, U32 width, U8 checklinebreak);
S32	Get_CharHeight(void);
S32	Get_CharHeightOfAllLang(U8 size);
void	Get_StringWidthHeight(U8 *String,S32* pWidth, S32* pHeight);
void	Get_StringWidthHeight_n(U8 *String,S32 n,S32* pWidth, S32* pHeight);
void	Get_StringWidthHeight_w(U8 *String,S32 w,S32* pWidth, S32* pHeight);
U8* 	Get_Current_Lang_CountryCode(void);
 
void Get_StringWidthHeight_multitap(U8 *String,S32 w,S32* pWidth, S32* pHeight);

void	Get_StringWidthHeight_wn(U8 *String,S32 w,S32 n,S32* pWidth, S32* pHeight);
S32	Get_StringHeight(void);
U8	SetFont(stFontAttribute Font , U8 );
//static void	ShowChar(U32 LineHeight, U32 x, U32 y, stFontAttribute Font, U8 Background, U8 *CharData, U32 NumChar);
U32	ShowString(S32 x, S32 y, stFontAttribute Font, U8 BackGround, U8 *String, U32 LineHeight);
U32   ShowString_n(U32 x_unsigned, U32 y_unsigned, stFontAttribute Font, U8 BackGround, U8 *String, int Len, U32 LineHeight);
U32	ShowStringBordered(S32 x, S32 y, stFontAttribute Font, U8 BackGround, U8 *String, U32 LineHeight);
U32   ShowStringBordered_n(U32 x_unsigned, U32 y_unsigned, stFontAttribute Font, U8 BackGround, U8 *String, int Len, U32 LineHeight);
void GetLangIndex(U8* pCount, S32* pnFontIndex, S32* pnIndex, U32 nCh, U32* pnBase);
S32 SearchIndexinList(U8 nListIndex, U16 nFontCount,U32 nCh, U32* pnBase );
S32 SearchInPropFont(U32 nCh, U32* pnBase );
S32 Get_PropWidthHeight(U32 Ch, S32* pWidth, S32* pHeight);

S32 Get_FontHeight(stFontAttribute Font,U8 arrCount);


//for stacked Display
BOOL IsThaiToneMark(U16 ch);
BOOL IsThaiBaseLineChar(U16 ch);
BOOL IsThaiAboveLineChar(U16 ch);
BOOL IsThaiBottomLineChar(U16 ch);
BOOL IsThaiLongTailChar(U16 ch);
BOOL IsThaiDigitChar(U16 ch);
BOOL IsThaiBaseSignChar(U16 ch);
BOOL HaveThaiCharacter(U16 *str);
BOOL RequireToMoveCursor(U16 preCh, U16 curCh);
U32 ShowStackedString(S32 CurrentX, S32 CurrentY, stFontAttribute Font, U8 BackGround, U8 *String, U32 LineHeight, U32 Bordered,S32 len);
void ShowStackedChar(S32 CurrentX, S32 CurrentY, stFontAttribute Font, U8 BackGround, U16 curCh, U32 LineHeight, U32 Bordered, U16 pre2Ch, U16 preCh);


//for R2L characters
BOOL IsL2RMMIStyle(void);


#ifdef __cplusplus
}
#endif

#endif	// _PIXCOMFONTENGINE_H

/*-----------------------------------------------------------------------*/
/*--------------------------- *** End *** -------------------------------*/
/*-----------------------------------------------------------------------*/

