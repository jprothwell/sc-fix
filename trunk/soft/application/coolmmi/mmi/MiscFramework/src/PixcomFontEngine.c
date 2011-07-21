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
 *    pixcomfontengine.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   font engine related function calls
 *
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: PixcomFontengine.c

  	PURPOSE		: Font Engine

 
					type 1 -> size's 9 to 15.
					type's 2 to 5 -> size's 8 to 15.

 

	DATE		: May 25, 2002

**************************************************************/

#define FONTENGINE_IRAM_PLACEMENT	0
#define BORDERED_TEXT_NO_BOUNDARY 1  

#include "stdc.h"
#include <stdio.h>
//#include <signal.h>    // zrx del 20060524
#include "mmi_data_types.h"
#include "fontres.h"
#include "fontdcl.h"
#include "fontdata.h"
#include "gui_data_types.h"
#include "debuginitdef.h"
#include "mmi_features.h"
 
#include "csdpropfont.h"
#if defined(__MMI_ZI__)
#include "ziextracharacter.h"
#endif
//CSD End
#include "gui.h"

#ifdef __MMI_HINDI_ALG__
#include "pixcomfontengine.h"
#include "gui.h"
#include "hf_rules.h"
#endif


 #ifdef __MMI_LANG_VIETNAMESE__
#include "gui_lang_viet.h"
#endif

#include "gdi_include.h"
#include "sscstringhandle.h"
/*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-7 15:26 */
#include "gdi_internal.h"
#ifdef __MMI_BIDI_ALG__
#include "bididef.h"
#include "bidiprot.h"
#include "arabicgprot.h"
#endif
/*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-7 15:26 */
#include "ucs2prot.h"
#ifdef __MMI_ZI_ARABIC__
#include "zi8arshape.h"
#endif

U8 gnCurrentFontAttrib = FONTATTRIB_NORMAL;
U8	gbFontType	=	0;

U32 gThaiFontInterSpace = 0;/*The new Thai font already has the inter space*/
//U32 gThaiLineHeight = 18;/*remove this for it's without reason, it's unattached with fontres*/

void SetFontValues(U32 nFont , U8 arrCount);


#if defined(__MMI_HINDI_ALG__)
extern void get_hindi_rules_for_whole_string(PU8 String);
#endif
//extern sLanguageDetails gLanguageArray[MAX_LANGUAGES];
extern sLanguageDetails *gLanguageArray;
extern void gdi_font_begin(gdi_color fg_color,U8 font_attr);
extern void gdi_font_end(void);

extern U8  (*pfnEncodingSchemeToUnicode)( PU16 pUnicode,PU8 arrOut);
extern void gdi_show_char_bordered(S32 x, S32 y, gdi_color text_color,gdi_color border_color,U8 *font_data, U32 font_data_size,U16 char_width,U16 char_height,U8 font_attr);
extern void gdi_show_char(S32 x, S32 y, gdi_color color,U8 *font_data, U32 font_data_size,U16 char_width,U16 char_height,U8 font_attr);


extern BOOL r2lMMIFlag;
#ifdef __MMI_BIDI_ALG__
extern U8     MMI_bidi_input_type;
U16  			show_pwcWord[MAX_SHOW_STRING_TEXT_LENGTH];
U8 			show_visual_str[MAX_SHOW_STRING_TEXT_LENGTH*BIDI_ENCODING_LENGTH]; 
#endif


const RangeData gCSDProprietaryFont_RangeData[22]={
{133,134},
{711,711},
{714,715},
{729,729},
{8216,8217},
{8220,8221},
{8230,8231},
{8364,8364},
{12289,12290},
{12298,12303},
{61440,61442},
{61444,61445},
{61447,61447},
{62720,62725},
{65104,65104},
{65106,65106},
{65108,65111},
{65113,65114},
{65124,65125},
{65281,65374},
{65377,65380},
{65515,65515},
};

const RangeDetails gCSDProprietaryFont_RangeInfo={
22,
gCSDProprietaryFont_RangeData
};

sCustFontData gCSDProprietaryFont = {
14, 14, 0, 25, 500,
#ifdef __MMI_HINDI_ALG__
(U8*)NULL,
#endif
(U8*)CSDProprietaryFont_Width, (U32*)CSDProprietaryFont_Offset, (U8*)CSDProprietaryFont_Data, (U32*)CSDProprietaryFont_RangeOffset,
{      
{FONTATTRIB_NORMAL|FONTATTRIB_BOLD|FONTATTRIB_ITALIC|FONTATTRIB_OBLIQUE|FONTATTRIB_UNDERLINE|FONTATTRIB_STRIKETHROUGH,FONTATTRIB_NORMAL|FONTATTRIB_BOLD|FONTATTRIB_ITALIC|FONTATTRIB_OBLIQUE|FONTATTRIB_UNDERLINE|FONTATTRIB_STRIKETHROUGH},
{FONTATTRIB_NORMAL|FONTATTRIB_BOLD|FONTATTRIB_ITALIC|FONTATTRIB_OBLIQUE|FONTATTRIB_UNDERLINE|FONTATTRIB_STRIKETHROUGH,FONTATTRIB_NORMAL|FONTATTRIB_BOLD|FONTATTRIB_ITALIC|FONTATTRIB_OBLIQUE|FONTATTRIB_UNDERLINE|FONTATTRIB_STRIKETHROUGH},
{FONTATTRIB_NORMAL|FONTATTRIB_BOLD|FONTATTRIB_ITALIC|FONTATTRIB_OBLIQUE|FONTATTRIB_UNDERLINE|FONTATTRIB_STRIKETHROUGH,FONTATTRIB_NORMAL|FONTATTRIB_BOLD|FONTATTRIB_ITALIC|FONTATTRIB_OBLIQUE|FONTATTRIB_UNDERLINE|FONTATTRIB_STRIKETHROUGH},
{FONTATTRIB_NORMAL|FONTATTRIB_BOLD|FONTATTRIB_ITALIC|FONTATTRIB_OBLIQUE|FONTATTRIB_UNDERLINE|FONTATTRIB_STRIKETHROUGH,FONTATTRIB_NORMAL|FONTATTRIB_BOLD|FONTATTRIB_ITALIC|FONTATTRIB_OBLIQUE|FONTATTRIB_UNDERLINE|FONTATTRIB_STRIKETHROUGH},
{FONTATTRIB_NORMAL|FONTATTRIB_BOLD|FONTATTRIB_ITALIC|FONTATTRIB_OBLIQUE|FONTATTRIB_UNDERLINE|FONTATTRIB_STRIKETHROUGH,FONTATTRIB_NORMAL|FONTATTRIB_BOLD|FONTATTRIB_ITALIC|FONTATTRIB_OBLIQUE|FONTATTRIB_UNDERLINE|FONTATTRIB_STRIKETHROUGH},  },
&gCSDProprietaryFont_RangeInfo};



#if defined(__MMI_ZI__)
const RangeData gZiExtraCharacter_RangeData[5]={
{322,322},
{341,341},
{352,352},
{61440,61448},
{61456,62398},
};

const RangeDetails gZiExtraCharacter_RangeInfo={
5,
gZiExtraCharacter_RangeData
};

sCustFontData gZiExtraCharacter = {
14, 14, 0, 25, 500,
#ifdef __MMI_HINDI_ALG__
(U8*)NULL,
#endif
(U8*)ZiExtraCharacter_Width, (U32*)ZiExtraCharacter_Offset, (U8*)ZiExtraCharacter_Data, (U32*)ZiExtraCharacter_RangeOffset,
{      
{FONTATTRIB_NORMAL|FONTATTRIB_BOLD|FONTATTRIB_ITALIC|FONTATTRIB_OBLIQUE|FONTATTRIB_UNDERLINE|FONTATTRIB_STRIKETHROUGH,FONTATTRIB_NORMAL|FONTATTRIB_BOLD|FONTATTRIB_ITALIC|FONTATTRIB_OBLIQUE|FONTATTRIB_UNDERLINE|FONTATTRIB_STRIKETHROUGH},
{FONTATTRIB_NORMAL|FONTATTRIB_BOLD|FONTATTRIB_ITALIC|FONTATTRIB_OBLIQUE|FONTATTRIB_UNDERLINE|FONTATTRIB_STRIKETHROUGH,FONTATTRIB_NORMAL|FONTATTRIB_BOLD|FONTATTRIB_ITALIC|FONTATTRIB_OBLIQUE|FONTATTRIB_UNDERLINE|FONTATTRIB_STRIKETHROUGH},
{FONTATTRIB_NORMAL|FONTATTRIB_BOLD|FONTATTRIB_ITALIC|FONTATTRIB_OBLIQUE|FONTATTRIB_UNDERLINE|FONTATTRIB_STRIKETHROUGH,FONTATTRIB_NORMAL|FONTATTRIB_BOLD|FONTATTRIB_ITALIC|FONTATTRIB_OBLIQUE|FONTATTRIB_UNDERLINE|FONTATTRIB_STRIKETHROUGH},
{FONTATTRIB_NORMAL|FONTATTRIB_BOLD|FONTATTRIB_ITALIC|FONTATTRIB_OBLIQUE|FONTATTRIB_UNDERLINE|FONTATTRIB_STRIKETHROUGH,FONTATTRIB_NORMAL|FONTATTRIB_BOLD|FONTATTRIB_ITALIC|FONTATTRIB_OBLIQUE|FONTATTRIB_UNDERLINE|FONTATTRIB_STRIKETHROUGH},
{FONTATTRIB_NORMAL|FONTATTRIB_BOLD|FONTATTRIB_ITALIC|FONTATTRIB_OBLIQUE|FONTATTRIB_UNDERLINE|FONTATTRIB_STRIKETHROUGH,FONTATTRIB_NORMAL|FONTATTRIB_BOLD|FONTATTRIB_ITALIC|FONTATTRIB_OBLIQUE|FONTATTRIB_UNDERLINE|FONTATTRIB_STRIKETHROUGH},  },
&gZiExtraCharacter_RangeInfo};
#endif
/* CSD End */

U32		      gnCurrentFont;
sCustFontData*	gpCurrentFont = NULL;
extern U16		gMaxDeployedLangs;
extern U16 		gCurrLangIndex;

#ifdef __UCS2_ENCODING
U8 UCS2EncodingToUnicode(PU16 pUnicode ,PU8 arr);
#endif

/**************************************************************/
extern 	void text_drawpixel2(S32 x,S32 y);
extern color_t UI_current_text_color;
extern color_t UI_text_border_color;
//extern color UI_text_color;
extern void (*_ui_text_putpixel)(s32 x,s32 y,color_t c);
extern void (*_ui_text_putpixel_unconditional)(s32 x,s32 y,color_t c);
extern void UI_set_current_text_color(color_t c);


/**************************************************************

	FUNCTION NAME		: Get_Current_Lang_CountryCode()

  	PURPOSE				: To get current language's country code

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8 *

 

**************************************************************/
U8* Get_Current_Lang_CountryCode(void)
{
	return (U8 *)gLanguageArray[gCurrLangIndex].aLangCountryCode;
}


/**************************************************************

	FUNCTION NAME		: SetFont()

  	PURPOSE				: To set the fonts.

	INPUT PARAMETERS	: stFontAttribute

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8

 

**************************************************************/
U8 SetFont(stFontAttribute Font , U8 arrCount)
{

	gnCurrentFont = Font.size;
	gbFontType = Font.smallCaps;
	gnCurrentFontAttrib = 0;

	if(gbFontType != 0)
		gbFontType = 0;

	if(Font.bold) gnCurrentFontAttrib|=FONTATTRIB_BOLD;
	if(Font.italic) gnCurrentFontAttrib|=FONTATTRIB_ITALIC;
	if(Font.oblique) gnCurrentFontAttrib|=FONTATTRIB_OBLIQUE;
	if(Font.underline) gnCurrentFontAttrib|=FONTATTRIB_UNDERLINE;
	if(!gnCurrentFontAttrib)gnCurrentFontAttrib|=FONTATTRIB_NORMAL;

	SetFontValues(gnCurrentFont,arrCount);

	return 0;
}

void SetFontValues(U32 nFont, U8 arrCount )
{
	if((gLanguageArray[arrCount].fontfamilyList[gLanguageArray[arrCount].nCurrentFamily]->nTotalFonts) > nFont)
	{
			gpCurrentFont = gLanguageArray[arrCount].fontfamilyList[gLanguageArray[arrCount].nCurrentFamily]->fontData[nFont];
	}
	else
   {
			gpCurrentFont = gLanguageArray[arrCount].fontfamilyList[gLanguageArray[arrCount].nCurrentFamily]->fontData[0];
	}
}

#if(FONTENGINE_IRAM_PLACEMENT)
#ifdef CSD_TARGET
#pragma arm section code
#endif
#endif

#if defined(__MMI_ZI__)
/**************************************************************

	FUNCTION NAME		: SearchInZiExtraFont()

  	PURPOSE				: To search a character in the Zi Extra font

	INPUT PARAMETERS	: U32 nCh

	OUTPUT PARAMETERS	: U32* pnBase - Returns the base

	RETURNS				: S32 - The offset of the character in the Range table
 
						  array is sorted ina scending order and no ranges overlap.

**************************************************************/
S32 SearchInZiExtraFont(U32 nCh, U32* pnBase )
{
	S16	nFirst	= 	0;
	S16 nLast = gZiExtraCharacter.pRangeDetails->nNoOfRanges-1;
	S16 nMid;

	while(nLast>=nFirst)
	{
		nMid = (nFirst+nLast)/2;;
		if((nCh >=  gZiExtraCharacter_RangeData[nMid].nMin) && (nCh <=  gZiExtraCharacter_RangeData[nMid].nMax))
		{
			*pnBase = gZiExtraCharacter_RangeData[nMid].nMin;
			return gZiExtraCharacter.pRange[nMid];

		}
		if(nCh>gZiExtraCharacter_RangeData[nMid].nMin)
			nFirst = nMid +1;
		else
			nLast = nMid-1;

	}

	return -1;
}
#endif

/**************************************************************

	FUNCTION NAME		: GetFontdata()

  	PURPOSE				: To get the font information.

	INPUT PARAMETERS	: U32 Ch, stFontAttribute Font, U8 *CharData

	OUTPUT PARAMETERS	: nil

	RETURNS				: UNIT32

 

**************************************************************/
#ifdef __MMI_HINDI_ALG__
U32 GetFontdata(U32 Ch, stFontAttribute Font, U8 **ppCharData,U16* pnWidth, U16* pnDWidth,U16*	pnHeight)
#else
U32 GetFontdata(U32 Ch, stFontAttribute Font, U8 **ppCharData,U16* pnWidth, U16*	pnHeight)
#endif
{
	S32 NumChar=0;
	U8   count;
	S32   index	=	-1;
	U32	nIndexInList;
	U32	nBase;
	S32	nFontIndex	=	-1;
	#ifdef __MMI_HINDI_ALG__
	sCustFontData*  CurrentFont=gpCurrentFont;
	*pnDWidth=0;
	#endif


	GetLangIndex(&count,&nFontIndex,&index,Ch,&nBase);

	if(index > -1)
	{

		/*Comments : No error handling is being  done if some characters come that is not
		  currently supported then nothing can be displayed */

		SetFontValues(gnCurrentFont,count);
		*pnWidth = gpCurrentFont->nWidth;
		*pnHeight = gpCurrentFont->nHeight;

			if(gpCurrentFont->nEquiDistant)
			{

				NumChar = gpCurrentFont->nCharBytes;
				nIndexInList = index+(NumChar*(Ch-nBase));
				*ppCharData = (gpCurrentFont->pDataArray+nIndexInList);

			}
			else
			{
				nIndexInList = index+(Ch- nBase);
         			*pnWidth = gpCurrentFont->pWidthArray[nIndexInList];
				NumChar = (gpCurrentFont->pOffsetArray[nIndexInList+1]- gpCurrentFont->pOffsetArray[nIndexInList]);
				*ppCharData = (gpCurrentFont->pDataArray+gpCurrentFont->pOffsetArray[nIndexInList]);

			}
			#ifdef __MMI_HINDI_ALG__
			if(gpCurrentFont->pDWidthArray)
				*pnDWidth = gpCurrentFont->pDWidthArray[nIndexInList];
			gpCurrentFont=CurrentFont;
			#endif
			return NumChar;
	}
	else
	{

		
		 
		#if defined(__MMI_ZI__)
		index = SearchInZiExtraFont(Ch,&nBase);
		#else
		index = SearchInPropFont(Ch,&nBase);
		#endif
		
		if(index == -1)
		{
		#if defined(__MMI_ZI__)
		index = SearchInPropFont(Ch,&nBase);
		if(index == -1)
		{
		#endif
		
  		   Ch = 65515;//0x4E01;//0x000F;
   		   index = SearchInPropFont(Ch,&nBase);
		   if(index == -1)
			   {
			#ifdef __MMI_HINDI_ALG__
		   gpCurrentFont=CurrentFont;
			#endif
		   return NumChar;
			   }
		#if defined(__MMI_ZI__)
			}
			*pnHeight 		= gCSDProprietaryFont.nHeight;
			nIndexInList 	= index+(Ch- nBase);
	   	*pnWidth 		= gCSDProprietaryFont.pWidthArray[nIndexInList];
			NumChar 			= (gCSDProprietaryFont.pOffsetArray[nIndexInList+1]- gCSDProprietaryFont.pOffsetArray[nIndexInList]);
			*ppCharData 	= (gCSDProprietaryFont.pDataArray+gCSDProprietaryFont.pOffsetArray[nIndexInList]);
			goto label_out;
		#endif			

		}

		#if defined(__MMI_ZI__)
			*pnHeight 		= gZiExtraCharacter.nHeight;
		nIndexInList = index+(Ch- nBase);
	   	*pnWidth 		= gZiExtraCharacter.pWidthArray[nIndexInList];
			NumChar 			= (gZiExtraCharacter.pOffsetArray[nIndexInList+1]- gZiExtraCharacter.pOffsetArray[nIndexInList]);
			*ppCharData 	= (gZiExtraCharacter.pDataArray+gZiExtraCharacter.pOffsetArray[nIndexInList]);
			label_out: ;
		#else
			*pnHeight 		= gCSDProprietaryFont.nHeight;
		nIndexInList = index+(Ch- nBase);
	   	*pnWidth 		= gCSDProprietaryFont.pWidthArray[nIndexInList];
			NumChar 			= (gCSDProprietaryFont.pOffsetArray[nIndexInList+1]- gCSDProprietaryFont.pOffsetArray[nIndexInList]);
			*ppCharData 	= (gCSDProprietaryFont.pDataArray+gCSDProprietaryFont.pOffsetArray[nIndexInList]);
	   #endif
	   //CSD End

	}
	#ifdef __MMI_HINDI_ALG__
	gpCurrentFont=CurrentFont;
	#endif
	return NumChar;
}


/**************************************************************

	FUNCTION NAME		: GetLangIndex()

  	PURPOSE				: To get the language index of the character.

	INPUT PARAMETERS	: U32 nCh - Character to look for

	OUTPUT PARAMETERS	: U8* pCount - The langugae Index
						  S32* pnIndex - The offset of the character in the array
						  U32* pnBase -  Base of Character set

	RETURNS				: void
 

**************************************************************/

void GetLangIndex(U8* pCount, S32* pnFontIndex, S32* pnIndex, U32 nCh, U32* pnBase)
{
	// first check the index in the current language. 90% of the cases strings will be from current language
	U16		nFontCount;
	sFontFamily*	pFontFamily;
	U16		nFontAttrib = gnCurrentFontAttrib;
	*pCount = 0;

	while(*pCount < gMaxDeployedLangs)
	{
		pFontFamily = gLanguageArray[*pCount].fontfamilyList[gLanguageArray[*pCount].nCurrentFamily];
		nFontCount = 0;
		if(gnCurrentFontAttrib&FONTATTRIB_BOLD && (U32)(pFontFamily->fontData[nFontCount]->pFontType[gnCurrentFont][gbFontType]&FONTATTRIB_BOLD))	// this will test for bold font.. which ever font is bold will betaken
									                     // bold is super set. If font is displayed for bold and italic then the bold font will be made as italic
			nFontAttrib = FONTATTRIB_BOLD;
		else
		{
			nFontAttrib &= ~FONTATTRIB_BOLD;
			if(!nFontAttrib)nFontAttrib |= FONTATTRIB_NORMAL;
		}

		while(nFontCount < pFontFamily->nTotalFonts)
		{
			if((U32)(pFontFamily->fontData[nFontCount]->pFontType[gnCurrentFont][gbFontType]&nFontAttrib))
			{
				if(nCh>=pFontFamily->fontData[nFontCount]->pRangeDetails->pRangeData[0].nMin &&
					nCh<=pFontFamily->fontData[nFontCount]->pRangeDetails->pRangeData[pFontFamily->fontData[nFontCount]->pRangeDetails->nNoOfRanges-1].nMax)
				{

					*pnIndex = SearchIndexinList(*pCount,nFontCount,nCh,pnBase);
					if(*pnIndex == -1)
					{
						++nFontCount;
					}
					else
					{
						gpCurrentFont = pFontFamily->fontData[nFontCount];
						*pnFontIndex = nFontCount;
						return;
					}

				}//if(nCh>=pFontFamily->fontData[nFontCount].pRangeData[0].nMin &&
				else
					++nFontCount;
			}
			else
				++nFontCount;
		}//while(nFontCount < pFontFamily->nTotalFonts)
		++(*pCount);

	}//while(*pCount < gMaxDeployedLangs)



	if(*pnIndex == -1 && nCh != 0)
	{
		mmi_trace(0,"WARNING!! ch(0x%x) not found in font res", nCh);
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: SearchIndexinList()

  	PURPOSE				: To search index and base of the character in a language.

	INPUT PARAMETERS	: U8 nListIndex, U32 nCh

	OUTPUT PARAMETERS	: U32* pnBase - Returns the base

	RETURNS				: S32 - The offset of the character in the Range table
 
						  array is sorted ina scending order and no ranges overlap.

**************************************************************/



S32 SearchIndexinList(U8 nListIndex, U16 nFontCount,U32 nCh, U32* pnBase )
{
	S16	nFirst	= 	0;
	S16 nLast = gLanguageArray[nListIndex].fontfamilyList[gLanguageArray[nListIndex].nCurrentFamily]->fontData[nFontCount]->pRangeDetails->nNoOfRanges-1;
	const RangeData*	pRangeData	=	gLanguageArray[nListIndex].fontfamilyList[gLanguageArray[nListIndex].nCurrentFamily]->fontData[nFontCount]->pRangeDetails->pRangeData;
	U32*		pRange	=	gLanguageArray[nListIndex].fontfamilyList[gLanguageArray[nListIndex].nCurrentFamily]->fontData[nFontCount]->pRange;
	S16 nMid;

	while(nLast>=nFirst)
	{
		nMid = (nFirst+nLast)/2;;
		if((nCh >=  pRangeData[nMid].nMin) && (nCh <=  pRangeData[nMid].nMax))
		{
			*pnBase = pRangeData[nMid].nMin;
			return pRange[nMid];

		}
		if(nCh>pRangeData[nMid].nMin)
			nFirst = nMid +1;
		else
			nLast = nMid-1;

	}

	return -1;
}
/**************************************************************

	FUNCTION NAME		: SearchInPropFont()

  	PURPOSE				: To search a character in the properitory font

	INPUT PARAMETERS	: U32 nCh

	OUTPUT PARAMETERS	: U32* pnBase - Returns the base

	RETURNS				: S32 - The offset of the character in the Range table
 
						  array is sorted ina scending order and no ranges overlap.

**************************************************************/



S32 SearchInPropFont(U32 nCh, U32* pnBase )
{
	S16	nFirst	= 	0;
	S16 nLast = gCSDProprietaryFont.pRangeDetails->nNoOfRanges-1;
	S16 nMid;

	while(nLast>=nFirst)
	{
		nMid = (nFirst+nLast)/2;;
		if((nCh >=  gCSDProprietaryFont_RangeData[nMid].nMin) && (nCh <=  gCSDProprietaryFont_RangeData[nMid].nMax))
		{
			*pnBase = gCSDProprietaryFont_RangeData[nMid].nMin;
			return gCSDProprietaryFont.pRange[nMid];

		}
		if(nCh>gCSDProprietaryFont_RangeData[nMid].nMin)
			nFirst = nMid +1;
		else
			nLast = nMid-1;

	}

	return -1;
}

/************************************************************************************************

	FUNCTION NAME		: ShowString()

  	PURPOSE				: To display a string in the screen

	INPUT PARAMETERS	: U32 LineHeight, U32 x, U32 y, stFontAttribute Font, U8 Background, U8 *CharData, U32 NumChar

	OUTPUT PARAMETERS	: nil

	RETURNS				: UNIT32

 

***********************************************************************************************/    
 
BOOL cancelWidth=0;
#ifndef __MMI_LANG_THAI__/*Remove compile warnings*/
 
extern color_t	UI_text_color;
#ifdef __MMI_HINDI_ALG__
 
static U32 ShowString_internal(S32 x, S32 y, stFontAttribute Font, U8 BackGround, U8 *String, S32 len,U32 LineHeight, U32 Bordered)
#else
static U32 ShowString_internal(S32 x, S32 y, stFontAttribute Font, U8 *String, S32 len,U32 LineHeight, U32 Bordered)
#endif
{
	GDI_ENTER_CRITICAL_SECTION(ShowString_internal)
	U8*	CharData;

	U32	NumChar;
	U32	Counter=0;
	S32	CurrentX;
	U16	nHgt;
	U16	nWidth;
	#ifdef __MMI_HINDI_ALG__
	U16	nDWidth;
	#endif
	S32	nXOffset;
	S32	nYOffset;
	S32 yy;
	U16 unicode = 0;
#if defined(__MMI_LANG_VIETNAMESE__)
	U16 next_unicode = 0;
	viet_tone_mark tone_mark = VIET_TONE_NONE;
	viet_vowel_letter viet_vowel = VIET_VOWEL_NONE;
#endif	
 
#ifdef __MMI_HINDI_ALG__
	static S32 single_time_bidi = 0; 
 
	static S32 hindi_rule_string = 1;  
 
#endif
 
	U8   font_attr = 0;
	gdi_color border_color = 0,text_color;
	S32 clip_x1,clip_y1,clip_x2,clip_y2;

	#ifdef __MMI_BIDI_ALG__
	PMT_BIDI_TYPES    default_direction= BIDI_L;
	U16 					logical_cur_pos; 
	U8 * 					temp_text_P; 
	U16					visual_str_len; 
	U16 					visual_cur_pos;
	U16 					nLen;
	U16 					visual_hilight_start;
	U16 					visual_hilight_end;
	#endif
 
	gdi_layer_get_clip(&clip_x1,&clip_y1,&clip_x2,&clip_y2); 
	if (len == 0)
		GDI_RETURN(1); 

 
#ifdef __MMI_HINDI_ALG__
	if(!single_time_bidi && !cancelWidth) 
#endif 
	{ 
 
	#ifdef __MMI_BIDI_ALG__
	nLen = UCS2Strlen((const char *)String);
   MMI_ASSERT(!(nLen >  MAX_SHOW_STRING_TEXT_LENGTH-ENCODING_LENGTH));
	if (len > 0)
		UCS2Strncpy((S8 *)show_pwcWord, (S8 *)String, len);
	else
		UCS2Strcpy((S8 *)show_pwcWord, (S8 *)String);
	nLen = UCS2Strlen((const char *)show_pwcWord);
	if(nLen)
		//ArabicStringShape((U16 *)&nLen, show_pwcWord);		//output will replace the input string after returning
		ArabicShapeEngine_ext((U16 *)&nLen, show_pwcWord);
	String = (U8 *)show_pwcWord;
	bidi_get_char_type((U8 *)String, (U8 *)&default_direction);
	logical_cur_pos = 0;
	
	if(default_direction == BIDI_R || default_direction == AL)
	{
		bidi_main((U8 *)String, (U16) UCS2Strlen((const S8 *)String), logical_cur_pos, 
					  show_visual_str, &visual_str_len, &visual_cur_pos, 
					  MMI_TRUE ,MMI_bidi_input_type,
					  0,0,
					  &visual_hilight_start, &visual_hilight_end);		
		default_direction = BIDI_R;
	}
	else //if(default_direction == BIDI_L)
	{
		bidi_main((U8 *)String, (U16) UCS2Strlen((const S8 *)String), 
					  logical_cur_pos, show_visual_str, &visual_str_len, &visual_cur_pos, 
					  MMI_FALSE ,MMI_bidi_input_type,
					  0,0,
					  &visual_hilight_start, &visual_hilight_end);		
		default_direction = BIDI_L;
	}
         	 
		#ifdef __MMI_HINDI_ALG__
		if(r2lMMIFlag)
		{
			get_hindi_rules_for_whole_string(show_visual_str);
			hindi_rule_string=0;
		}
        #endif // __MMI_HINDI_ALG__
		 

	if((r2lMMIFlag && default_direction == BIDI_L) ||(!r2lMMIFlag && default_direction != BIDI_L))
	 	bidi_reverse(show_visual_str, (U16) UCS2Strlen((const S8 *)show_visual_str));
	
	temp_text_P = String;
	String=show_visual_str;
	#endif
        } 

    
	//UI_current_text_color = UI_text_color;	
    
	if(gnCurrentFontAttrib&FONTATTRIB_ITALIC)
		font_attr |= FONTATTRIB_ITALIC;
	else if(gnCurrentFontAttrib&FONTATTRIB_OBLIQUE) 
		font_attr |= FONTATTRIB_OBLIQUE;
	
	if(Font.underline) 	font_attr |=FONTATTRIB_UNDERLINE;

	{
		if(Bordered)
		border_color = gdi_act_color_from_rgb(0xff,UI_text_border_color.r, UI_text_border_color.g, UI_text_border_color.b);
	
	text_color = gdi_act_color_from_rgb(0xff,UI_current_text_color.r, UI_current_text_color.g, UI_current_text_color.b);
		
	}
		
	gdi_font_begin(text_color,font_attr);
	
	CurrentX=x;

#ifdef __MMI_HINDI_ALG__
	if(hf_is_reset_hindi_params())
 	reset_hindi_params();
	if(Font.color_t == 0 )
		Font.color_t = 15 ;
	 
	 
	//UI_current_text_color = UI_text_color;	
	 
	if(hf_is_hindi_rules_parsing()&& hindi_rule_string) 
 
	{
		U16 cluster_fill[G_MAX];
		S32 cluster_length;
		U16 glyph_output[G_MAX];
		S32 Len;
 
		S32 total_len=0;
		S32 original_len=0;
 
		S32 width, height;
		init_cluster_start_p(String);
 
		if (len > 0)
		{
		init_cluster_end_p(String+(len*2));
		}
	    else
		{
		  init_cluster_end_p(String+(2*UCS2Strlen((const char *)String)+1));
		}
		original_len=len;
 
		hf_disable_hindi_rules_parsing();
		hf_disable_hindi_reset_hindi_params();
 
		single_time_bidi = 1; 
 
		
		do{
			cluster_length = hf_get_cluster(cluster_fill);		
 
			total_len+=cluster_length;
			if(original_len>0)
			{
				if(total_len>original_len)
					break;
			}
 
			if(cluster_length)
			{
				Len=hf_hindi_rules(glyph_output,cluster_fill,cluster_length);
				ShowString_internal(x,y, Font, BackGround, (U8*)glyph_output,len,LineHeight,Bordered);
				Get_StringWidthHeight((U8*)glyph_output, &width, &height);
 
				if (r2lMMIFlag)
				x-=width;
				else
				x+=width;
 
			}
		}while(cluster_length);
		hf_enable_hindi_rules_parsing();
		hf_enable_hindi_reset_hindi_params();
 
		single_time_bidi = 0; 
 
      	gdi_font_end();
      
      	GDI_RETURN(0);
	}
	 
	hindi_rule_string = 1;
	 
#endif

	while(len!=0)
	{
		 
		U16 prev_unicode = 0;
		 
		if( (String[0] == '\0')  &&  (String[1] == '\0'))
				break;
		len--;

		 
		prev_unicode = unicode;
		 
		unicode = String[0];
		unicode|=(String[1]<<8);

		if(FONT_TEST_CONTROL_CHAR(unicode) )
			unicode = 0x20;

		if(FONT_TEST_FORMAT_CHAR(unicode))
		{
			String += 2;
			continue;
		}

#if defined(__MMI_LANG_VIETNAMESE__)
		if ((unicode > 0x0040) && (unicode < 0x01B1))
		{
			next_unicode = String[2];
			next_unicode |= (String[3]<<8);
			tone_mark = mmi_viet_tone_mark(next_unicode);
			if (VIET_TONE_NONE != tone_mark)
			{
				viet_vowel = mmi_viet_vowel_letter(unicode);
				if (VIET_VOWEL_NONE != viet_vowel)
				{
					unicode = mmi_viet_combine_vowel_tone(viet_vowel,tone_mark);
					String += 2;
				}
			}
		}
#endif

		#ifdef __MMI_HINDI_ALG__
		NumChar = GetFontdata(unicode, Font, &CharData,&nWidth,&nDWidth,&nHgt);
		#else
		NumChar = GetFontdata(unicode, Font, &CharData,&nWidth,&nHgt);
		#endif
		Counter++;

		if(LineHeight>0)
			yy=y+LineHeight-nHgt;
		else
			yy=y;
 
	   if(r2lMMIFlag 
#ifdef __MMI_HINDI_ALG__
		    &&(!cancelWidth)
#endif
         )
	   {
#ifdef __MMI_HINDI_ALG__
		    
		   if(UI_HINDI_CHARACTER_UCS2_RANGE(unicode))
		   {
			   if(UI_HINDI_SIGN_RANGE(prev_unicode))
                   CurrentX -= 0;
			   else if(UI_HINDI_SIGN_RANGE(unicode))
                   CurrentX -= nWidth;
			   else
			   CurrentX -= nDWidth;
		   }
		   else
#endif
                   
	 	   CurrentX -= nWidth;
	   }
 
		nXOffset = CurrentX+nWidth;
		nYOffset = yy+nHgt;
 
      #if defined(__MMI_HINDI_ALG__)
		if (cancelWidth || !r2lMMIFlag)
		get_text_x(&CurrentX, unicode,yy,nDWidth,nWidth);
      #endif
 

		if(Bordered)
			gdi_show_char_bordered(CurrentX, yy, text_color,border_color,CharData, NumChar,nWidth,nHgt, font_attr);
		else if(!(gnCurrentFontAttrib&FONTATTRIB_BOLD && gpCurrentFont->pFontType[gnCurrentFont][gbFontType]&FONTATTRIB_BOLD) 
			|| !(gpCurrentFont->pFontType[gnCurrentFont][gbFontType]&FONTATTRIB_NORMAL)) 
		{
			gdi_show_char(CurrentX, yy, text_color,CharData, NumChar,nWidth,nHgt, font_attr);
		} 
		else 
		{
			gdi_show_char(CurrentX, yy-1, text_color,CharData, NumChar,nWidth,nHgt, font_attr);
			gdi_show_char(CurrentX, yy, text_color,CharData, NumChar,nWidth,nHgt, font_attr);
		}

    	if(!r2lMMIFlag)
	      CurrentX += nWidth;

		nHgt = (U8)LineHeight;
		String += 2;
		if ( (CurrentX > clip_x2) && !r2lMMIFlag )
			break;
	}
	gdi_font_end();

	GDI_RETURN (CurrentX);
	
	GDI_EXIT_CRITICAL_SECTION(ShowString_internal)
	return 1;
		
}
#endif /*__MMI_LANG_THAI__*/
U32 ShowString_n(U32 x_unsigned, U32 y_unsigned, stFontAttribute Font, U8 BackGround, U8 *String, int Len, U32 LineHeight)
{
#if defined(__MMI_LANG_THAI__)
   return ShowStackedString(x_unsigned,y_unsigned,Font,BackGround,String,LineHeight, 0,Len);
#elif defined(__MMI_HINDI_ALG__)
 
	return ShowString_internal(x_unsigned,y_unsigned,Font,BackGround,String,Len,LineHeight,FALSE);
 
#else
	return ShowString_internal(x_unsigned,y_unsigned,Font,String,Len,LineHeight,FALSE);
#endif
}


U32 ShowString(S32 x, S32 y, stFontAttribute Font, U8 BackGround, U8 *String, U32 LineHeight)
{
   #if defined(__MMI_LANG_THAI__)
   return ShowStackedString(x,y,Font,BackGround,String,LineHeight, 0,-1);
#elif defined(__MMI_HINDI_ALG__)
 
      return ShowString_internal(x,y,Font,BackGround,String,-1,LineHeight, 0);
 
   #else
      return ShowString_internal(x,y,Font,String,-1,LineHeight, 0);
   #endif
}

U32 ShowStringBordered(S32 x, S32 y, stFontAttribute Font, U8 BackGround, U8 *String, U32 LineHeight)
{
   #if defined(__MMI_LANG_THAI__)
   return ShowStackedString(x,y,Font,BackGround,String,LineHeight, 1,-1);
   #elif defined(__MMI_HINDI_ALG__)
 
      return ShowString_internal(x,y,Font,BackGround,String,-1,LineHeight, 1);
 
#else
      return ShowString_internal(x,y,Font,String,-1,LineHeight, 1);
   #endif
}
U32 ShowStringBordered_n(U32 x_unsigned, U32 y_unsigned, stFontAttribute Font, U8 BackGround, U8 *String, int Len, U32 LineHeight)
{
#if defined(__MMI_LANG_THAI__)
   return ShowStackedString(x_unsigned,y_unsigned,Font,BackGround,String,LineHeight, 1,Len);
#elif defined(__MMI_HINDI_ALG__)
 
   return ShowString_internal(x_unsigned,y_unsigned,Font,BackGround,String,Len,LineHeight,1);
 
#else
   return ShowString_internal(x_unsigned,y_unsigned,Font,String,Len,LineHeight,1);
#endif
}

 

/**************************************************************

	FUNCTION NAME		: Get_CharWidth()

  	PURPOSE				: To get char widths

	INPUT PARAMETERS	: U32 Ch

	OUTPUT PARAMETERS	: nil

	RETURNS				: S32

 

**************************************************************/

void Get_CharWidth(U32 Ch, S32* pdWidth)
{
	U8  count   =   0;
    S32 index   =   -1;
    U32 nBase;
	S32 Height=0;
    S32	nFontIndex	=	-1;
    sCustFontData*  CurrentFont=gpCurrentFont;
	GetLangIndex(&count,&nFontIndex,&index,Ch,&nBase);
	if(index > -1)
    {
		if(count>=gMaxDeployedLangs)
		{
			*pdWidth=0;
			gpCurrentFont=CurrentFont;
			return;
		}
		SetFontValues(gnCurrentFont,count);
		if(gpCurrentFont->nEquiDistant)
			*pdWidth = gpCurrentFont->nWidth;
		else
			*pdWidth = gpCurrentFont->pWidthArray[index + (Ch- nBase)];
	}
	else
	{
		Get_PropWidthHeight(Ch, pdWidth, &Height);
	}
    gpCurrentFont = CurrentFont;
    return ;
}


/**************************************************************

	FUNCTION NAME		: Get_CharNumInWidth()

  	PURPOSE				: To get char number in specified width

	INPUT PARAMETERS	: U8 *String, U32 width

	OUTPUT PARAMETERS	: number of characters

	RETURNS				: void

 

**************************************************************/

U32 Get_CharNumInWidth(U8 *String, U32 width, U8 checklinebreak)
{
	U32 nwidth = 0, chwidth, chheight;
	U32 nchar = 0, line_break_char_counter=0;
	U16 ch;
	U8   linebreakflag=0;

	pfnEncodingSchemeToUnicode(&ch, String);
	if(ch == 0)
		return 0;

	do
	{
		pfnEncodingSchemeToUnicode(&ch, String);

		if(checklinebreak)
               linebreakflag = FONT_TEST_WORD_WRAPPING_BREAK_CHARACTER(ch);

		if(FONT_TEST_BIDI_CONTROL_CHAR(ch) )
			chwidth = 0;
		else
		{
			Get_CharWidthHeight(ch, (S32 *) &chwidth, (S32 *)&chheight);
			chwidth += 1;
		}
		
		nwidth += chwidth;
		String +=2;
		nchar ++;
		if(checklinebreak)
		{
        if(UI_STRING_END_OF_STRING_CHARACTER(ch))
        {
           line_break_char_counter=nchar;
        }
  
		  if(linebreakflag)
        {
           line_break_char_counter=nchar;
        }
         /*    if (!FONT_TEST_ASCII_CHARACTER(ch))
              {
                    line_break_char_counter = 0;
              }
		  */
		}
					
	}while(nwidth <= width && ch != 0 /*&& ch != 0x0A && ch != 0x0D*/);

	if(checklinebreak)
	{
	  if(line_break_char_counter == 0 )
		  nchar--;
	  else		
		  nchar = line_break_char_counter;
	}
	else
	nchar--;
	
	return nchar;


}




/**************************************************************

	FUNCTION NAME		: Get_CharWidthHeight()

  	PURPOSE				: To get char widths

	INPUT PARAMETERS	: U32 Ch

	OUTPUT PARAMETERS	: nil

	RETURNS				: S32

 

**************************************************************/
#ifdef __MMI_HINDI_ALG__
void Get_CharWidthHeight(U32 Ch, S32* pdWidth, S32* pHeight)
#else
void Get_CharWidthHeight(U32 Ch, S32* pWidth, S32* pHeight)
#endif
{
    U8  count   =   0;
    S32 index   =   -1;
    U32 nBase;
    S32	nFontIndex	=	-1;
    sCustFontData*  CurrentFont=gpCurrentFont;


    GetLangIndex(&count,&nFontIndex,&index,Ch,&nBase);
    
    if(index > -1)
    {	
		#ifdef __MMI_HINDI_ALG__
		if(count>=gMaxDeployedLangs)
		{
			*pdWidth=0;
			*pHeight=0;
			gpCurrentFont=CurrentFont;
			return;
		}
		#endif
      SetFontValues(gnCurrentFont,count);
      *pHeight = gpCurrentFont->nHeight;
		#ifdef __MMI_HINDI_ALG__
		if(UI_HINDI_CHARACTER_UCS2_RANGE(Ch))
		{
			*pdWidth = gpCurrentFont->pDWidthArray[index + (Ch- nBase)/*+103*/];		
		}
		else
		{
			if(gpCurrentFont->nEquiDistant)
				*pdWidth = gpCurrentFont->nWidth;
			else
				*pdWidth = gpCurrentFont->pWidthArray[index + (Ch- nBase)];
		}
		#else
            if(gpCurrentFont->nEquiDistant)
                 *pWidth = gpCurrentFont->nWidth;
            else
                 *pWidth = gpCurrentFont->pWidthArray[index + (Ch- nBase)];
		#endif
    }
    else
    {
		#ifdef __MMI_HINDI_ALG__
			Get_PropWidthHeight(Ch, pdWidth, pHeight);
		#else	
         Get_PropWidthHeight(Ch, pWidth, pHeight);
		#endif
    }
    gpCurrentFont = CurrentFont;
    return ;


}


/**************************************************************

	FUNCTION NAME		: Get_CharBoundingBox()

  	PURPOSE				: To get the bounding box of "drawn area" of a character

	INPUT PARAMETERS	: U32 Ch

	OUTPUT PARAMETERS	: pCharWidth - width of character
							pCharHeight 	- height of character
							pBoxXoffset 	- X offset of the bounding box of drawn area relative to the top-left corner
							pBoxYoffset 	- Y offset of the bounding box of drawn area  relative to the top-left corner
							pBoxWidth 		- width of the bounding box of drawn area 
							pBoxHeight		- height of the bounding box of drawn area 

	RETURNS				: void

 
							1. It does not handle Hindi properly.
							2. The function is slow because it process internal font data.
							3. This function can be used to align a character in a better way because the "drawn area" is aligned
							  differently for different font database.
**************************************************************/
void Get_CharBoundingBox(U32 Ch, S32 *pCharWidth, S32 *pCharHeight, S32 *pBoxXoffset, S32 *pBoxYoffset, S32 *pBoxWidth, S32 *pBoxHeight)
{
	U8*	font_data;
	U32	font_data_size;
	U16	char_height;
	U16	char_width;
#ifdef __MMI_HINDI_ALG__
	U16	char_d_width=0;
#endif
	S32	fx1, fy1, fx2, fy2, x, y;

#ifdef __MMI_HINDI_ALG__
	font_data_size = GetFontdata(Ch, *UI_font /* dummy parameter */, &font_data,&char_width,&char_d_width,&char_height);
#else
	font_data_size = GetFontdata(Ch, *UI_font /* dummy parameter */, &font_data,&char_width,&char_height);
#endif

	*pCharWidth = char_width;
	*pCharHeight = char_height;

	fx1 = char_width - 1; /* min X of drawn area */
	fx2 = 0; /* max X of drawn area */
	fy1 = char_height - 1; /* min Y of drawn area */
	fy2 = 0; /* max Y of drawn area */

	x = 0;
	y = 0;

	while (font_data_size--)
	{
		U8	pattern = *font_data++;

		if (!pattern)
		{
			U32	nTemp;
			
			x += 8;
			nTemp = x / char_width;
			if (nTemp)
			{
				y += nTemp;
				char_height -= (U16) nTemp;
				if(!char_height) 
					break;
			}

			x %= char_width;
		}
		else
		{
			S32 i;
			for (i = 0; i < 8; i++)
			{
				if (pattern & 1)
				{
					if (x < fx1)
						fx1 = x;
					if (y < fy1)
						fy1 = y;
					if (x > fx2)
						fx2 = x;
					if (y > fy2)
						fy2 = y;
				}
				
				x++;
				if (x == (S32) char_width)
				{
					y++;
					char_height --;
					x = 0;
					if (!char_height)
						break;
				}
				pattern >>= 1;
			}
		}
	}

	MMI_DBG_ASSERT(fx2 >= fx1 && fy2 >= fy1);

	*pBoxXoffset = fx1;
	*pBoxYoffset = fy1;
	*pBoxWidth = fx2 - fx1 + 1;
	*pBoxHeight = fy2 - fy1 + 1;
}

/**************************************************************

	FUNCTION NAME		: Get_CharWidthHeight()

  	PURPOSE				: To get char widths

	INPUT PARAMETERS	: U32 Ch

	OUTPUT PARAMETERS	: nil

	RETURNS				: S32

 

**************************************************************/
S32 Get_PropWidthHeight(U32 Ch, S32* pWidth, S32* pHeight)
{
	U32		nBase	=	0;
	S32		index	=	-1;
 
	#if defined(__MMI_ZI__)
	index = SearchInZiExtraFont(Ch,&nBase);
	#else
	index = SearchInPropFont(Ch,&nBase);
	#endif

	if(index == -1)
	{
	#if defined(__MMI_ZI__)
	index = SearchInPropFont(Ch,&nBase);
	if(index == -1)
	{
	#endif
		
  	    Ch = 65515;
     		 index = SearchInPropFont(Ch,&nBase);
	    if(index == -1)
		 return index;

	#if defined(__MMI_ZI__)
	}
		*pHeight = gCSDProprietaryFont.nHeight;
	   *pWidth = gCSDProprietaryFont.pWidthArray[index];
	   goto label_out2;
	#endif	

	}

#if defined(__MMI_ZI__)
	*pHeight = gZiExtraCharacter.nHeight;
   *pWidth = gZiExtraCharacter.pWidthArray[index];
   label_out2: ;
#else
   *pHeight = gCSDProprietaryFont.nHeight;
   *pWidth = gCSDProprietaryFont.pWidthArray[index];
#endif
//CSD End
	return index;
}

/**************************************************************

	FUNCTION NAME		: Get_StringWidthHeight()

  	PURPOSE				: To get string width and height

	INPUT PARAMETERS	: U8 *String

	OUTPUT PARAMETERS	: nil

	RETURNS				: S32

 

**************************************************************/

void Get_StringWidthHeight(U8 *String, S32* pWidth, S32* pHeight)
{
	S32 StrWidth = 0;
	S32 nHeight = 0;
	S32 Counter = 0;
	U16 unicode = 0;
	U16 curCh=0, preCh=0, pre2Ch=0;
#if defined(__MMI_LANG_VIETNAMESE__)
	U16 next_unicode = 0;
	viet_tone_mark tone_mark = VIET_TONE_NONE;
	viet_vowel_letter viet_vowel = VIET_VOWEL_NONE;
#endif	
   #ifdef __MMI_BIDI_ALG__
	U16 	nLen;
   #endif

	*pHeight = 0;
	*pWidth = 0;
#ifdef __MMI_HINDI_ALG__
	if(hf_is_reset_hindi_params())
 	reset_hindi_params();
	if(hf_is_hindi_rules_parsing()) 
	{
		U16 cluster_fill[G_MAX];
		S32 cluster_length;
		U16 glyph_output[G_MAX];
		S32 Len;
		S32 len;

		S32 total_len=0;
		S32 original_len=0;

		S32 width, height;
		pre2Ch = 0;
		preCh = 0;
		if(String==NULL)
			return;
		init_cluster_start_p(String);
		len = coolsand_UI_strlen((UI_string_type)String);

	  if (len > 0)
	  {
		init_cluster_end_p(String+(len*2));
	  }
	  else
	  {
		  init_cluster_end_p(String+(2*UCS2Strlen((const char *)String)+1));
	  }
		original_len=len;

	   hf_disable_hindi_rules_parsing();
	   hf_disable_hindi_reset_hindi_params();
		
		do{
			cluster_length = hf_get_cluster(cluster_fill);		

			total_len+=cluster_length;
			if(original_len>0)
			{
				if(total_len>original_len)
					break;
			}

			if(cluster_length)
			{
				Len=hf_hindi_rules(glyph_output,cluster_fill,cluster_length);
				Get_StringWidthHeight((U8*)glyph_output, &width, &height);
				if(height>*pHeight) *pHeight = height;
				*pWidth+=width;
			}
		}while(cluster_length);
		hf_enable_hindi_rules_parsing();
		hf_enable_hindi_reset_hindi_params();
      	return;
}
#endif
	if(String==NULL) return;

   #ifdef __MMI_BIDI_ALG__
   nLen = UCS2Strlen((const char *)String);
   MMI_ASSERT(!(nLen >  MAX_SHOW_STRING_TEXT_LENGTH-ENCODING_LENGTH));
	UCS2Strcpy((S8 *)show_pwcWord, (S8 *)String);
	nLen = UCS2Strlen((const char *)show_pwcWord);
	if(nLen)
		//ArabicStringShape(&nLen, show_pwcWord);		//output will replace the input string after returning
		ArabicShapeEngine_ext(&nLen, show_pwcWord);
	String = (U8 *)show_pwcWord;
   #endif

	while(1)
	{
		if( (String[0] == '\0')  &&  (String[1] == '\0') )
		{
				break;

		}

		memcpy(&unicode,String,2);
#if defined(__MMI_LANG_VIETNAMESE__)
		if ((unicode > 0x0040) && (unicode < 0x01B1))
		{
			next_unicode = String[2];
			next_unicode |= (String[3]<<8);
			tone_mark = mmi_viet_tone_mark(next_unicode);
			if (VIET_TONE_NONE != tone_mark)
			{
				viet_vowel = mmi_viet_vowel_letter(unicode);
				if (VIET_VOWEL_NONE != viet_vowel)
				{
					unicode = mmi_viet_combine_vowel_tone(viet_vowel,tone_mark);
					String += 2;
				}
			}
		}
#endif

		Get_CharWidthHeight(unicode,&StrWidth,&nHeight);
      //Consider the Thai String length is different from others
		curCh = unicode;
		if (curCh >= 0x0E01 && curCh <= 0x0E7F)
		{
			 
			if (IsThaiBaseLineChar(curCh))
			{
            //if the current character is 0x0E33 and previous or previous 2 character is base line character, 0x0E33 shall be
            //moved ahead to let the circyle sign on the top of previous or previous2 character
				if(curCh == 0x0E33 && (IsThaiBaseLineChar(preCh) ||IsThaiBaseLineChar(pre2Ch)))
				{
					*pWidth= *pWidth - 4 + (S32)StrWidth + gThaiFontInterSpace;
				}
				else
				{
					*pWidth= *pWidth + (S32)StrWidth + gThaiFontInterSpace;
				}
			}
			//For Thai digits and base line sign, the only thing we have to do is moving cursor
			else if(IsThaiDigitChar(curCh) || IsThaiBaseSignChar(curCh))
			{
				*pWidth= *pWidth + (S32)StrWidth + gThaiFontInterSpace;
			}
			else if(IsThaiToneMark(curCh))
			{
				if(IsThaiBaseLineChar(preCh)||IsThaiAboveLineChar(preCh)||IsThaiBottomLineChar(preCh))
				{
				}
				else if(preCh == 0x0E47 || preCh == 0x0E4C || preCh == 0x0E4D)/*Add by panxu for 0x0E4d*/
				{
			 		 
			 		*pWidth= *pWidth + (S32)StrWidth + gThaiFontInterSpace;
				}
				else
				{
					*pWidth= *pWidth + (S32)StrWidth + gThaiFontInterSpace;
				}

			}

			//*pHeight = gThaiLineHeight;//If  the string has Thai characters,the height will become gThaiLineHeight(now the value is 17)
			pre2Ch = preCh;
			preCh = curCh;
		}
#ifdef __MMI_HINDI_ALG__
		else if (UI_HINDI_CHARACTER_UCS2_RANGE(curCh))
		{
			U16 nextCharUCS2;
			memcpy(&nextCharUCS2,String+2,2);
			if(UI_HINDI_SIGN_RANGE(curCh))
			{
				(*pWidth)+=0;
			}
			else
			{
				if (UI_HINDI_CHARACTER_UCS2_RANGE(nextCharUCS2) || nextCharUCS2==0)
				 {
					(*pWidth)+=StrWidth;
				 }
				 else
				 {
					 Get_CharWidth(curCh,&StrWidth);
					 (*pWidth)+=StrWidth;
				 }
			}
		}
#endif
		else
		{
			(*pWidth)+=StrWidth;
		}
		
			if(*pHeight < nHeight)
		{
				*pHeight = nHeight;
		}
			Counter++;

		String += 2;
	}
	return;
}

#if(FONTENGINE_IRAM_PLACEMENT)
#ifdef CSD_TARGET
#pragma arm section code
#endif
#endif

/**************************************************************

	FUNCTION NAME		: Get_StringWidthHeight_n()

  	PURPOSE				: To get string widths and height upto a given number of characters

	INPUT PARAMETERS	: U8 *String,S32 n,S32* pWidth, S32* pHeight

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void Get_StringWidthHeight_n(U8 *String,S32 n,S32* pWidth, S32* pHeight)
{

	S32 StrWidth = 0;
	S32 Counter = 0;
	S32	nHeight	=	0;

	U16 unicode = 0;
	U16 curCh=0, preCh=0, pre2Ch=0;
#if defined(__MMI_LANG_VIETNAMESE__)
	U16 next_unicode = 0;
	viet_tone_mark tone_mark = VIET_TONE_NONE;
	viet_vowel_letter viet_vowel = VIET_VOWEL_NONE;
#endif	

	*pHeight = 0;
	*pWidth = 0;

	if(String==NULL) return;


	while(Counter<n)
	{
		if( (String[0] == '\0')  &&  (String[1] == '\0') )
		{
				break;
		}

		memcpy(&unicode,String,2);
#if defined(__MMI_LANG_VIETNAMESE__)
		if ((unicode > 0x0040) && (unicode < 0x01B1))
		{
			next_unicode = String[2];
			next_unicode |= (String[3]<<8);
			tone_mark = mmi_viet_tone_mark(next_unicode);
			if (VIET_TONE_NONE != tone_mark)
			{
				viet_vowel = mmi_viet_vowel_letter(unicode);
				if (VIET_VOWEL_NONE != viet_vowel)
				{
					unicode = mmi_viet_combine_vowel_tone(viet_vowel,tone_mark);
					String += 2;
					n +=1;
				}
			}
		}
#endif

			Get_CharWidthHeight(unicode,&StrWidth,&nHeight);
      //Consider the Thai String length is different from others
		curCh = unicode;
		if (curCh >= 0x0E01 && curCh <= 0x0E7F)
		{
			 
			if (IsThaiBaseLineChar(curCh))
			{
            //if the current character is 0x0E33 and previous or previous 2 character is base line character, 0x0E33 shall be
            //moved ahead to let the circyle sign on the top of previous or previous2 character
				if(curCh == 0x0E33 && (IsThaiBaseLineChar(preCh) ||IsThaiBaseLineChar(pre2Ch)))
				{
					*pWidth= *pWidth - 4 + (S32)StrWidth + gThaiFontInterSpace;
				}
				else
				{
					*pWidth= *pWidth + (S32)StrWidth + gThaiFontInterSpace;
				}
			}
			//For Thai digits and base line sign, the only thing we have to do is moving cursor
			else if(IsThaiDigitChar(curCh) || IsThaiBaseSignChar(curCh))
			{
				*pWidth= *pWidth + (S32)StrWidth + gThaiFontInterSpace;
			}
			else if(IsThaiToneMark(curCh))
			{
				if(IsThaiBaseLineChar(preCh)||IsThaiAboveLineChar(preCh)||IsThaiBottomLineChar(preCh))
				{
				}
				else if(preCh == 0x0E47 || preCh == 0x0E4C)
				{
			 		 
			 		*pWidth= *pWidth + (S32)StrWidth + gThaiFontInterSpace;
				}
				else
				{
					*pWidth= *pWidth + (S32)StrWidth + gThaiFontInterSpace;
				}

			}

			//*pHeight = gThaiLineHeight;//If  the string has Thai characters,the height will become gThaiLineHeight(now the value is 17)
			pre2Ch = preCh;
			preCh = curCh;
		}
		else
		{
			(*pWidth)+=StrWidth;
		}

			if(*pHeight < nHeight)
		{
				*pHeight = nHeight;
		}
			Counter++;

		String += 2;
	}
	return;
}
 
void Get_StringWidthHeight_multitap(U8 *String,S32 w,S32* pWidth, S32* pHeight)
{
	S32 StrWidth = 0;
	S32 Counter = 0;
	S32	nHeight	=	0;
	U16 unicode = 0;
	

	*pHeight = 0;
	*pWidth = 0;

	if(String==NULL) return;


	while(1)
	{
		if( (String[0] == '\0')  &&  (String[1] == '\0') )
		{
				break;
		}
		memcpy(&unicode,String,2);

		Get_CharWidthHeight(unicode,&StrWidth,&nHeight);
      //Consider the Thai String length is different from others
		
		(*pWidth)+=(StrWidth+w);
		if(*pHeight < nHeight)
			*pHeight = nHeight;
		Counter++;

		String += 2;
	}
	return;
}

void Get_StringWidthHeight_w(U8 *String,S32 w,S32* pWidth, S32* pHeight)
{
	S32 StrWidth = 0;
	S32 Counter = 0;
	S32	nHeight	=	0;
	U16 unicode = 0;
	U16 curCh=0, preCh=0, pre2Ch=0;
#if defined(__MMI_LANG_VIETNAMESE__)
	U16 next_unicode = 0;
	viet_tone_mark tone_mark = VIET_TONE_NONE;
	viet_vowel_letter viet_vowel = VIET_VOWEL_NONE;
#endif	

	*pHeight = 0;
	*pWidth = 0;

	if(String==NULL) return;


	while(1)
	{
		if( (String[0] == '\0')  &&  (String[1] == '\0') )
		{
				break;
		}
		memcpy(&unicode,String,2);
#if defined(__MMI_LANG_VIETNAMESE__)
		if ((unicode > 0x0040) && (unicode < 0x01B1))
		{
			next_unicode = String[2];
			next_unicode |= (String[3]<<8);
			tone_mark = mmi_viet_tone_mark(next_unicode);
			if (VIET_TONE_NONE != tone_mark)
			{
				viet_vowel = mmi_viet_vowel_letter(unicode);
				if (VIET_VOWEL_NONE != viet_vowel)
				{
					unicode = mmi_viet_combine_vowel_tone(viet_vowel,tone_mark);
					String += 2;
				}
			}
		}
#endif

			Get_CharWidthHeight(unicode,&StrWidth,&nHeight);
      //Consider the Thai String length is different from others
		curCh = unicode;
		if (curCh >= 0x0E01 && curCh <= 0x0E7F)
		{
			 
			if (IsThaiBaseLineChar(curCh))
			{
				  //if the current character is 0x0E33 and previous or previous 2 character is base line character, 0x0E33 shall be
				//moved ahead to let the circyle sign on the top of previous or previous2 character
				if(curCh == 0x0E33 && (IsThaiBaseLineChar(preCh) ||IsThaiBaseLineChar(pre2Ch)))
				{
					*pWidth= *pWidth - 4 + (S32)StrWidth + gThaiFontInterSpace + w;
				}
				else
				{
					*pWidth= *pWidth + (S32)StrWidth + gThaiFontInterSpace + w;
				}
			}
			//For Thai digits and base line sign, the only thing we have to do is moving cursor
			else if(IsThaiDigitChar(curCh) || IsThaiBaseSignChar(curCh))
			{
				*pWidth= *pWidth + (S32)StrWidth + gThaiFontInterSpace;
			}
			else if(IsThaiToneMark(curCh))
			{
				if(IsThaiBaseLineChar(preCh)||IsThaiAboveLineChar(preCh)||IsThaiBottomLineChar(preCh))
				{
				}
				else if(preCh == 0x0E47 || preCh == 0x0E4C)
				{
			 		 
			 		*pWidth= *pWidth + (S32)StrWidth + gThaiFontInterSpace + w;
				}
				else
				{
					*pWidth= *pWidth + (S32)StrWidth + gThaiFontInterSpace + w;
				}

			}

			//*pHeight = gThaiLineHeight;//If  the string has Thai characters,the height will become gThaiLineHeight(now the value is 17)
			pre2Ch = preCh;
			preCh = curCh;
		}
		else
		{
			(*pWidth)+=(StrWidth+w);
		}
			if(*pHeight < nHeight)
				*pHeight = nHeight;
			Counter++;

		String += 2;
	}
	return;
}

void Get_StringWidthHeight_wn(U8 *String,S32 w,S32 n,S32* pWidth, S32* pHeight)
{
	S32 StrWidth = 0;
	S32 Counter = 0;
	S32	nHeight	=	0;
	U16 unicode = 0;
	U16 curCh=0, preCh=0, pre2Ch=0;
#if defined(__MMI_LANG_VIETNAMESE__)
	U16 next_unicode = 0;
	viet_tone_mark tone_mark = VIET_TONE_NONE;
	viet_vowel_letter viet_vowel = VIET_VOWEL_NONE;
#endif	

	if(String==NULL) return;
	*pHeight = 0;
	*pWidth = 0;


	while(Counter<n)
	{
		if( (String[0] == '\0')  &&  (String[1] == '\0') )
		{
				break;
		}

		memcpy(&unicode,String,2);
#if defined(__MMI_LANG_VIETNAMESE__)
		if ((unicode > 0x0040) && (unicode < 0x01B1))
		{
			next_unicode = String[2];
			next_unicode |= (String[3]<<8);
			tone_mark = mmi_viet_tone_mark(next_unicode);
			if (VIET_TONE_NONE != tone_mark)
			{
				viet_vowel = mmi_viet_vowel_letter(unicode);
				if (VIET_VOWEL_NONE != viet_vowel)
				{
					unicode = mmi_viet_combine_vowel_tone(viet_vowel,tone_mark);
					String += 2;
					n += 1;
				}
			}
		}
#endif
		Get_CharWidthHeight(unicode,&StrWidth,&nHeight);
      //Consider the Thai String length is different from others
		curCh = unicode;
		if (curCh >= 0x0E01 && curCh <= 0x0E7F)
		{
			//curCh = unicode;
			 
			if (IsThaiBaseLineChar(curCh))
			{
				  //if the current character is 0x0E33 and previous or previous 2 character is base line character, 0x0E33 shall be
				//moved ahead to let the circyle sign on the top of previous or previous2 character
				if(curCh == 0x0E33 && (IsThaiBaseLineChar(preCh) ||IsThaiBaseLineChar(pre2Ch)))
				{
					*pWidth= *pWidth - 4 + (S32)StrWidth + gThaiFontInterSpace + w;
				}
				else
				{
					*pWidth= *pWidth + (S32)StrWidth + gThaiFontInterSpace + w;
				}
			}
			//For Thai digits and base line sign, the only thing we have to do is moving cursor
			else if(IsThaiDigitChar(curCh) || IsThaiBaseSignChar(curCh))
			{
				*pWidth= *pWidth + (S32)StrWidth + gThaiFontInterSpace;
			}
			else if(IsThaiToneMark(curCh))
			{
				if(IsThaiBaseLineChar(preCh)||IsThaiAboveLineChar(preCh)||IsThaiBottomLineChar(preCh))
				{
				}
				else if(preCh == 0x0E47 || preCh == 0x0E4C)
				{
			 		 
			 		*pWidth= *pWidth + (S32)StrWidth + gThaiFontInterSpace + w;
				}
				else
				{
					*pWidth= *pWidth + (S32)StrWidth + gThaiFontInterSpace + w;
				}
		}

			//*pHeight = gThaiLineHeight;//If  the string has Thai characters,the height will become gThaiLineHeight(now the value is 17)
			pre2Ch = preCh;
			preCh = curCh;
		}
		else
		{
			(*pWidth)+=(StrWidth+w);
		}

			Counter++;

		String += 2;
	}

	return;
}


/**************************************************************

	FUNCTION NAME		: Get_CharHeight()

  	PURPOSE				: To get char height

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: S32

 

**************************************************************/

S32 Get_CharHeight()
{
	return (S32)gpCurrentFont->nHeight;
}


/**************************************************************

	FUNCTION NAME		: Get_CharHeightOfAllLang()

  	PURPOSE				: To get maximum char height regardless of language

	INPUT PARAMETERS	: U8 size

	OUTPUT PARAMETERS	: nil

	RETURNS				: S32

 

**************************************************************/
S32 Get_CharHeightOfAllLang(U8 size)
{
	int 			lang_index;
   	sFontFamily*	pFontFamily;
   	S32				height = 0, h;

	for (lang_index = 0; lang_index < gMaxDeployedLangs; lang_index++)
	{
		pFontFamily = gLanguageArray[lang_index].fontfamilyList[gLanguageArray[lang_index].nCurrentFamily];
		if ((h = (S32) pFontFamily->fontData[size]->nHeight ) > height)
			height = h;
	}

	if (height < (S32) gCSDProprietaryFont.nHeight)
	{
		height = (S32) gCSDProprietaryFont.nHeight;
	}
	
 
#if defined(__MMI_ZI__)
	if (height < (S32) gZiExtraCharacter.nHeight)
	{
		height = (S32) gZiExtraCharacter.nHeight;
	}
#endif
//CSD End
	
	return height;
}


/**************************************************************

	FUNCTION NAME		: Get_StringHeight()

  	PURPOSE				: To get string height

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: S32

 

**************************************************************/

S32 Get_StringHeight()
{
   	sFontFamily*	pFontFamily;
       pFontFamily = gLanguageArray[gCurrLangIndex].fontfamilyList[gLanguageArray[gCurrLangIndex].nCurrentFamily];
       return (S32) (pFontFamily->fontData[gnCurrentFont]->nHeight);

#if 0
/* under construction !*/
#endif
}


/***************************************************************************

	FUNCTION NAME		: TestDiffFonts()

  	PURPOSE				: To test the different fonts with diferent sizes.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: BOOL

 

****************************************************************************/

void TestDiffFonts()
{
	S16 count = 0, y = 20;
	stFontAttribute fontInfo;
	fontInfo.bold = 0;
	fontInfo.italic = 0;
	fontInfo.underline = 0;

	fontInfo.size = 9;		// (size 8..15 FOR type 2..5 ) & (size 9...15 FOR type 1 )
	fontInfo.type = 1;		// 1...5

	for(count=0; count<2; count++)
	{
		ShowString(0, y, fontInfo, 0 , (PU8)"abcdefghij", 30);
		y += 20;

		ShowString(0, y, fontInfo, 0 , (PU8)"klmnopqrst", 30);
		y += 20;

		ShowString(0, y, fontInfo, 0 , (PU8)"uvwxyz", 30);
		fontInfo.size += 1;
		y += 20;

	}
}

S32 Get_FontHeight(stFontAttribute f,U8 arrCount)
{
   	sFontFamily*	pFontFamily;
       pFontFamily = gLanguageArray[arrCount].fontfamilyList[gLanguageArray[arrCount].nCurrentFamily];
       return (S32) (pFontFamily->fontData[f.size]->nHeight);
}

BOOL IsThaiToneMark(U16 ch)
{
	if(ch == 0x0E48 || ch == 0x0E49 ||
		ch == 0x0E4A || ch == 0x0E4B)
		return TRUE;
	else
		return FALSE;
}


BOOL IsThaiBaseLineChar(U16 ch)
{
	if (ch >= 0x0E01 && ch <= 0x0E7F)
	{
		    //above vowels
		if(ch != 0x0E31 && ch != 0x0E34 && ch != 0x0E35 && ch != 0x0E36 && ch != 0x0E37 &&
		     
		    ch != 0x0E48 && ch != 0x0E49 && ch != 0x0E4A && ch != 0x0E4B &&
		    //Below vowels
		    ch != 0x0E38 && ch != 0x0E39 && ch != 0X0E3A &&
		    //Signs
		    ch != 0x0E2F && ch != 0x0E3F && ch != 0x0E46 &&
		    ch != 0x0E47 && ch != 0x0E4C && 
			ch != 0x0E4d && /*Add by panxu for the 0x0E4D*/
		    //Exclude Thai digits
		    ch<0x0E4F)
		    return TRUE;
		else
  		    return FALSE;

	}
	else
		return FALSE;


}


BOOL IsThaiAboveLineChar(U16 ch)
{
 	if(ch==0x0E31 || ch==0x0E34 ||ch==0x0E35 ||
	    ch==0x0E36 || ch==0x0E37 || ch==0x0E47 || ch==0x0E4C 
		|| ch==0x0E4D /*Add by panxu for the 0x0E4D*/
		)
		return TRUE;
	else
		return FALSE;

}


BOOL IsThaiBottomLineChar(U16 ch)
{
	if (ch == 0x0E38 || ch ==0x0E39 || ch == 0x0E3A)
		return TRUE;
	else
		return FALSE;
}

BOOL IsThaiLongTailChar(U16 ch)
{
	if(ch == 0x0E1B || ch ==0x0E1D || ch == 0x0E1F)
		return TRUE;
	else
		return FALSE;
}

BOOL IsThaiDigitChar(U16 ch)
{
	if (ch >= 0x0E50 && ch<= 0x0E59)
		return TRUE;
	else
		return FALSE;

}


BOOL IsThaiBaseSignChar(U16 ch)
{
	 if (ch==0x0E2F || ch==0x0E3F|| ch==0x0E46 ||
	 	ch==0x0E4F || ch==0x0E5A || ch==0x0E5B)
	 	return TRUE;
	else
		return FALSE;
}


BOOL HaveThaiCharacter(U16 * str)
{
	U16 ch=0;

	while(1)
	{
		ch = str[0];
		if(ch == 0)
			return FALSE;
		if(ch >= 0x0E01&& ch <= 0x0E7F)
			return TRUE;
		str++;
	}
	return FALSE;

}

BOOL RequireToMoveCursor(U16 preCh, U16 curCh)
{

	pBOOL isPreBaseLineChar = IsThaiBaseLineChar(preCh);

	if (curCh >= 0x0E01 && curCh <= 0x0E7F)
	{

		 if (IsThaiBaseLineChar(curCh) || IsThaiDigitChar(curCh) ||
		 	IsThaiBaseSignChar(curCh))
		{ 	// For base level character
			return TRUE;
		}
		else if (IsThaiAboveLineChar(curCh))
		{ //For above level character
			if (isPreBaseLineChar)
				return FALSE;
			else if(curCh == 0x0E4C && preCh != 0x0E4C && preCh != 0x0E47)
				return FALSE;
			else
				return TRUE;

		}
		else if (IsThaiBottomLineChar(curCh))
		{	// For below level character
			if(isPreBaseLineChar)
				return FALSE;
			else
				return TRUE;
		}
		else if (IsThaiToneMark(curCh))
		{	 

			if (isPreBaseLineChar || IsThaiAboveLineChar(preCh) ||
				IsThaiBottomLineChar(preCh))
				return FALSE;
			else
				return TRUE;

		}
		else
			return TRUE;
	}
	else
		return TRUE;

}


__inline static void ShowCharFuncDispatch(U32 LineHeight, U16 nWidth, U16 nHgt, S32 CurrentX,
S32 CurrentY, stFontAttribute Font, U8 BackGround, U8 *CharData, U32 NumChar, U32 Bordered)
{
	S32 nXOffset, nYOffset;
	U8   font_attr = 0;
	gdi_color border_color = 0, text_color = 0;

	if(gnCurrentFontAttrib&FONTATTRIB_ITALIC)
		font_attr |= FONTATTRIB_ITALIC;
	else if(gnCurrentFontAttrib&FONTATTRIB_OBLIQUE) 
		font_attr |= FONTATTRIB_OBLIQUE;
	
	if(Font.underline) 	font_attr |=FONTATTRIB_UNDERLINE;

	{
		if(Bordered)
		border_color = gdi_act_color_from_rgb(0xff,UI_text_border_color.r, UI_text_border_color.g, UI_text_border_color.b);
	
	text_color = gdi_act_color_from_rgb(0xff,UI_current_text_color.r, UI_current_text_color.g, UI_current_text_color.b);
			
	}

      /* CSD consider Thai display in ShowStackedString
	if(LineHeight>0)
	{

		if(LineHeight==gThaiLineHeight ) //If the line height is equal to 17 points
			CurrentY+=(LineHeight-(S32)nHgt-1); // Lower one pixel to centralize ThaiFont
		else
			CurrentY+=(LineHeight-(S32)nHgt);

	}
      */
	nXOffset = CurrentX + nWidth;
	nYOffset = CurrentY + nHgt;
	if(Bordered)
		gdi_show_char_bordered(CurrentX, CurrentY, text_color,border_color,CharData, NumChar,nWidth,nHgt, font_attr);		
	else if(!(gnCurrentFontAttrib&FONTATTRIB_BOLD && gpCurrentFont->pFontType[gnCurrentFont][gbFontType]&FONTATTRIB_BOLD) 
			|| !(gpCurrentFont->pFontType[gnCurrentFont][gbFontType]&FONTATTRIB_NORMAL)) 
	{
		gdi_show_char(CurrentX, CurrentY, text_color,CharData, NumChar,nWidth,nHgt, font_attr);
	} 
	else 
	{
		gdi_show_char(CurrentX, CurrentY-1, text_color,CharData, NumChar,nWidth,nHgt, font_attr);
		gdi_show_char(CurrentX, CurrentY, text_color,CharData, NumChar,nWidth,nHgt, font_attr);
	}	
}


U32 ShowStackedString(S32 CurrentX, S32 CurrentY, stFontAttribute Font, U8 BackGround, U8 *String, U32 LineHeight, U32 Bordered,S32 len)
{
   GDI_ENTER_CRITICAL_SECTION(ShowStackedString)
   U8* CharData;
   U32 NumChar;
   U16 nHgt;
   U16 nWidth;
	BOOL has_thai_char = MMI_FALSE;
   S32 iX, iY;
   static U16 curCh=0, preCh=0, pre2Ch=0, nextCharUCS2=0;
   S32 interLeaving= gThaiFontInterSpace;
   gdi_color border_color,text_color;
   U8   font_attr = 0;
   S32 str_width, str_height;
   S32 clip_x1,clip_y1,clip_x2,clip_y2;
   
#if defined(__MMI_LANG_VIETNAMESE__)
   viet_tone_mark tone_mark = VIET_TONE_NONE;
   viet_vowel_letter viet_vowel = VIET_VOWEL_NONE;
#endif	
#if defined(__MMI_HINDI_ALG__)&&defined(__MMI_BIDI_ALG__)
static S32 hindi_rule_string = 1; 
#endif
#ifdef __MMI_HINDI_ALG__
   U16 nDWidth=0;
   static S32 single_time_bidi = 0; 
#endif
   
#ifdef __MMI_BIDI_ALG__
   PMT_BIDI_TYPES    default_direction= BIDI_L;
   U16 					logical_cur_pos; 
   U8 * 					temp_text_P; 
   U16					visual_str_len; 
   U16 					visual_cur_pos;
   U16 					nLen;
   U16 					visual_hilight_start;
   U16 					visual_hilight_end;
   S32 					temp_len = 0;
#endif
   
	gdi_layer_get_clip(&clip_x1,&clip_y1,&clip_x2,&clip_y2); 
   Get_StringWidthHeight(String, &str_width, &str_height);
   
#ifdef __MMI_HINDI_ALG__
   if(!single_time_bidi && !cancelWidth)
#endif 
   {
#ifdef __MMI_BIDI_ALG__
      nLen = UCS2Strlen((const char *)String);
      MMI_ASSERT(!(nLen >  MAX_SHOW_STRING_TEXT_LENGTH-ENCODING_LENGTH));
      if (len > 0)
      {
         UCS2Strncpy((S8 *)show_pwcWord, (S8 *)String, len);
      }
      else
      {
         UCS2Strcpy((S8 *)show_pwcWord, (S8 *)String);
      }
      nLen = UCS2Strlen((const char *)show_pwcWord);
      if(nLen)
      {
         //ArabicStringShape(&nLen, show_pwcWord);		//output will replace the input string after returning
         ArabicShapeEngine_ext(&nLen, show_pwcWord);
      }
      String = (U8 *)show_pwcWord;
      bidi_get_char_type((U8 *)String, (U8 *)&default_direction);
      logical_cur_pos = 0;
      
      if(default_direction == BIDI_R || default_direction == AL)
      {
         bidi_main((U8 *)String, (U16) UCS2Strlen((const S8 *)String), logical_cur_pos, 
                        show_visual_str, &visual_str_len, &visual_cur_pos, 
                        MMI_TRUE ,MMI_bidi_input_type,
                        0,0,
                        &visual_hilight_start, &visual_hilight_end);		
         default_direction = BIDI_R;
      }
      else //if(default_direction == BIDI_L)
      {
         bidi_main((U8 *)String, (U16) UCS2Strlen((const S8 *)String), 
                        logical_cur_pos, show_visual_str, &visual_str_len, &visual_cur_pos, 
                        MMI_FALSE ,MMI_bidi_input_type,
                        0,0,
                        &visual_hilight_start, &visual_hilight_end);		
         default_direction = BIDI_L;
      }
   
    
#ifdef __MMI_HINDI_ALG__
      if(r2lMMIFlag)
      {
         get_hindi_rules_for_whole_string(show_visual_str);
         hindi_rule_string=0;
      }
#endif // __MMI_HINDI_ALG__
    
   
      if((r2lMMIFlag && default_direction == BIDI_L) ||(!r2lMMIFlag && default_direction != BIDI_L))
      {
         bidi_reverse(show_visual_str, (U16) UCS2Strlen((const S8 *)show_visual_str));
      }
      
      temp_text_P = String;
      String=show_visual_str;
#endif
   } 
   
   if(gnCurrentFontAttrib&FONTATTRIB_ITALIC)
   {
      font_attr |= FONTATTRIB_ITALIC;
   }
   else if(gnCurrentFontAttrib&FONTATTRIB_OBLIQUE) 
   {
      font_attr |= FONTATTRIB_OBLIQUE;
   }
   
   if(Font.underline)
   {
      font_attr |=FONTATTRIB_UNDERLINE;
   }
   
   if(Bordered)
   {
      border_color = gdi_act_color_from_rgb(0xff,UI_text_border_color.r, UI_text_border_color.g, UI_text_border_color.b);
   }
   text_color = gdi_act_color_from_rgb(0xff,UI_current_text_color.r, UI_current_text_color.g, UI_current_text_color.b);
   
   gdi_font_begin(text_color,font_attr);
   //UI_current_text_color = UI_text_color;
   //Process string
#ifdef __MMI_HINDI_ALG__
   if(hf_is_reset_hindi_params())
   {
      reset_hindi_params();
   }
   if(Font.color_t == 0 )
   {
      Font.color_t = 15 ;
   }
   //For hindi character sudden disappear 
   //UI_current_text_color = UI_text_color;
   if(hf_is_hindi_rules_parsing()) 
   {
      U16 cluster_fill[G_MAX];
      S32 cluster_length;
      U16 glyph_output[G_MAX];
      S32 Len;
       
      S32 total_len=0;
      S32 original_len=0;
       
      S32 width, height;
      pre2Ch = 0;
      preCh = 0;
      init_cluster_start_p(String);
       
      if (len > 0)
      {
         init_cluster_end_p(String+(len*2));
      }
      else
      {
         init_cluster_end_p(String+(2*UCS2Strlen((const char *)String)+1));
      }
      original_len=len;
       
      hf_disable_hindi_rules_parsing();
      hf_disable_hindi_reset_hindi_params();
       
      single_time_bidi = 1;
      do{
         cluster_length = hf_get_cluster(cluster_fill);		
          
         total_len+=cluster_length;
         if(original_len>0)
         {
            if(total_len>original_len)
            {
               break;
            }
         }
          
         if(cluster_length)
         {
            Len=hf_hindi_rules(glyph_output,cluster_fill,cluster_length);
            ShowStackedString(CurrentX,CurrentY, Font, BackGround, (U8*)glyph_output,LineHeight,Bordered,len);
            Get_StringWidthHeight((U8*)glyph_output, &width, &height);
            CurrentX+=width;
         }
      }while(cluster_length);
      hf_enable_hindi_rules_parsing();
      hf_enable_hindi_reset_hindi_params();
       
      single_time_bidi = 0;
      gdi_font_end();
      
      GDI_RETURN(0);
   }
#endif
//tangjie add begin 20060512
   #ifdef __MMI_BIDI_ALG__
	if(r2lMMIFlag){
		
		while(1){
			curCh = (String[temp_len]|(String[temp_len+1]<<8));
			if (curCh >= 0x0E01 && curCh <= 0x0E7F){
				has_thai_char = MMI_TRUE;
				break;
			}
			temp_len +=2;
			if(curCh == 0x0000) break;
		}
		
		if (has_thai_char == MMI_TRUE){
			bidi_reverse(String, (U16) UCS2Strlen((const S8 *)String));
			Get_StringWidthHeight(String, &str_width, &str_height);
			CurrentX -= str_width;
		}
		
	}
	
#endif
//tangjie add end 20060512
   while(len!=0)
   {
      iX =CurrentX;
      iY = CurrentY;
      //prepare the unicode of current and next characters
      curCh = (String[0]|(String[1]<<8));
      nextCharUCS2 = (String[2]|(String[3]<<8));
      
      if (curCh == 0x0000)
      {
         break;
      }
      
      len--;          
      
      if(FONT_TEST_CONTROL_CHAR(curCh) )
      {
         curCh = 0x20;
      }
	if(FONT_TEST_FORMAT_CHAR(curCh))
	{
		String += 2;
		continue;
	}
#if defined(__MMI_LANG_VIETNAMESE__)
      if ((curCh > 0x0040) && (curCh < 0x01B1))
      {
         tone_mark = mmi_viet_tone_mark(nextCharUCS2);
         if (VIET_TONE_NONE != tone_mark)
         {
            viet_vowel = mmi_viet_vowel_letter(curCh);
            if (VIET_VOWEL_NONE != viet_vowel)
            {
               curCh = mmi_viet_combine_vowel_tone(viet_vowel,tone_mark);
               String += 2;
            }
         }
      }
#endif

#if defined(__MMI_HINDI_ALG__)
      NumChar = GetFontdata(curCh, Font, &CharData, &nWidth, &nDWidth,&nHgt);
      get_text_x(&iX, curCh,iY,nDWidth,nWidth);		
#else
      NumChar = GetFontdata(curCh, Font, &CharData, &nWidth, &nHgt);
#endif
      
      //check if current character is in Thai character ranges
      if (curCh >= 0x0E01 && curCh <= 0x0E7F)
      {
          
         if (IsThaiBaseLineChar(curCh))
         {
            //if the current character is 0x0E33 and previous or previous 2 character is base line character, 0x0E33 shall be
            //moved ahead to let the circyle sign on the top of previous or previous2 character
            if(curCh == 0x0E33 && (IsThaiBaseLineChar(preCh) ||IsThaiBaseLineChar(pre2Ch)))
            {
               iX -= 4;
            }
            CurrentX=(iX+(S32)nWidth+interLeaving);
         }
         //For Thai digits and base line sign, the only thing we have to do is moving cursor
         else if(IsThaiDigitChar(curCh) || IsThaiBaseSignChar(curCh))
         {
            CurrentX=(iX+(S32)nWidth+interLeaving);
         }
         else if(IsThaiAboveLineChar(curCh))
         {
            //Above line characters
            iY-=1;// Reserver 1 vertical interleaving
            if(IsThaiBaseLineChar(preCh))
            {
               // Process the X coordinate when combining with other base line characters
               iX-=((S32)nWidth+interLeaving);
               if(IsThaiLongTailChar(preCh))//move ahead 3 pixels,
                  iX -= 3;
               else if(preCh == 0x0E0A || preCh == 0x0E0B)
                  iX -= 2;
               else if(preCh == 0x0E2C) //move ahead 6 pixels
                  iX -= 6;
            }
         }
         else if(IsThaiBottomLineChar(curCh))
         {
            //Bottom line characters
            iY += interLeaving;
            if(IsThaiBaseLineChar(preCh))
            {
               iX -= ((S32)nWidth+interLeaving);
               if(preCh == 0x0E2C)
                  iX -= 2; //move ahead for 2 pixels
            }
         }
         else if(IsThaiToneMark(curCh))
         {
             
            iY -= 1;
            if(IsThaiBaseLineChar(preCh))
            {
               iX -= ((S32)nWidth + interLeaving);
               if(preCh == 0x0E33)
               {
                  iY -= 3; 
                  if(IsThaiBaseLineChar(pre2Ch))
                     iX -= 7;  
               }
               else if(nextCharUCS2 == 0x0E33)
                  iY -= 3; 
               else if(IsThaiLongTailChar(preCh))
                  iX -= 3;  
               else if(preCh == 0x0E0A || preCh == 0x0E0B)
                  iX -= 2;
               else if(preCh == 0x0E2C)
                  iX -= 6;
            }
            else if(IsThaiAboveLineChar(preCh))
            {
               iY -= 3;
               iX -=((S32)nWidth+interLeaving);
               if(IsThaiLongTailChar(pre2Ch))
                  iX -= 3;
               else if(pre2Ch == 0x0E0A || pre2Ch == 0x0E0B)
                  iX -=2;
               else if(pre2Ch == 0x0E2C)
                  iX -= 6;
            }
            else if(IsThaiBottomLineChar(preCh))
            {
               iX -= ((S32)nWidth+interLeaving);
               if(IsThaiLongTailChar(pre2Ch))
                  iX-=3;
            }
            else if(preCh == 0x0E47 || preCh == 0x0E4C 
				|| preCh == 0x0E4D /*Add by panxu*/
				)
            {
                
               iX +=((S32)nWidth+interLeaving);
               CurrentX += ((S32)nWidth+interLeaving);
            }
            else
            {
               CurrentX += ((S32)nWidth+interLeaving);
            }
         }
         iY += (str_height - nHgt); 
         ShowCharFuncDispatch(LineHeight, nWidth, nHgt, iX, iY, Font, BackGround, CharData, NumChar, Bordered);
      }
      else//non-Thai char
      {
         if(r2lMMIFlag 
#ifdef __MMI_HINDI_ALG__
            &&(!cancelWidth)
#endif
         )
         {
#ifdef __MMI_HINDI_ALG__
            if(UI_HINDI_CHARACTER_UCS2_RANGE(curCh))
            {
               if(UI_HINDI_SIGN_RANGE(preCh))
               {
                  CurrentX -= 0;
               }
               else if(UI_HINDI_SIGN_RANGE(curCh))
               {
                  CurrentX -= nWidth;
               }
               else
               {
                  CurrentX -= nDWidth;
               }
            }
            else
#endif
		    		if(!has_thai_char)
               {
                  CurrentX -= nWidth;
               }
         }
          
#ifndef __MMI_HINDI_ALG__
         iX = CurrentX;
#endif
          
         iY += (str_height - nHgt);
         ShowCharFuncDispatch(LineHeight, nWidth, nHgt, iX, iY, Font, BackGround, CharData, NumChar, Bordered);
    		 
			 if(!r2lMMIFlag || has_thai_char)
          {
             CurrentX += nWidth;
          }
      }
      nHgt = (U8) LineHeight;
      pre2Ch = preCh;
      preCh = curCh;
      String += 2;
      if ( (CurrentX > clip_x2) && !r2lMMIFlag )
		break;
   }
   gdi_font_end();
   
   GDI_RETURN(CurrentX);
   GDI_EXIT_CRITICAL_SECTION(ShowStackedString)
}

void ShowStackedChar(S32 CurrentX, S32 CurrentY, stFontAttribute Font, U8 BackGround, U16 curCh, U32 LineHeight, U32 Bordered, U16 pre2Ch, U16 preCh)
{
	GDI_ENTER_CRITICAL_SECTION(ShowStackedChar)
	U8 *CharData;
	U32 NumChar;
	U16 nHgt;
	U16 nWidth;
	#ifdef __MMI_HINDI_ALG__
	U16 nDWidth;
	#endif
	S32 interLeaving = gThaiFontInterSpace;
	S32 iX = CurrentX;
	S32 iY = CurrentY;
	gdi_color border_color=0,text_color;	
	U8   font_attr = 0;

	if(gnCurrentFontAttrib&FONTATTRIB_ITALIC)
		font_attr |= FONTATTRIB_ITALIC;
	else if(gnCurrentFontAttrib&FONTATTRIB_OBLIQUE) 
		font_attr |= FONTATTRIB_OBLIQUE;
	
	if(Font.underline) 	font_attr |=FONTATTRIB_UNDERLINE;


	{
		if(Bordered)
		border_color = gdi_act_color_from_rgb(0xff,UI_text_border_color.r, UI_text_border_color.g, UI_text_border_color.b);
	
	text_color = gdi_act_color_from_rgb(0xff,UI_current_text_color.r, UI_current_text_color.g, UI_current_text_color.b);
			
	}


   if(FONT_TEST_CONTROL_CHAR(curCh) )
		curCh = 0x20;

	if(FONT_TEST_FORMAT_CHAR(curCh))
	{
		return;
	}
	#ifdef __MMI_HINDI_ALG__
		NumChar = GetFontdata(curCh, Font, &CharData, &nWidth, &nDWidth,&nHgt);
	#else
		NumChar = GetFontdata(curCh, Font, &CharData, &nWidth, &nHgt);
	#endif

	gdi_font_begin(text_color,font_attr);

	//adjust iX and iY for Thai characters
	if(curCh >= 0x0E01 && curCh <= 0x0E7F)
	{
			if (IsThaiBaseLineChar(curCh))
			{
				if(IsThaiLongTailChar(curCh))
					iX += 1;
				  //if the current character is 0x0E33 and previous or previous 2 character is base line character, 0x0E33 shall be
				  //moved ahead to let the circyle sign on the top of previous or previous2 character
				if(curCh == 0x0E33 && (IsThaiBaseLineChar(preCh) ||IsThaiBaseLineChar(pre2Ch)))
					iX -= 1;

			}
			else if(IsThaiAboveLineChar(curCh))
			{
				//Above line characters
				iY-=1;// Reserver 1 vertical interleaving
				if(IsThaiBaseLineChar(preCh))
				{
				   // Process the X coordinate when combining with other base line characters
				   iX-=((S32)nWidth+interLeaving);
				   if(IsThaiLongTailChar(preCh))//move ahead 3 pixels,
				    	iX -= 3;
				   else if(preCh == 0x0E0A || preCh == 0x0E0B)
				   	iX -= 2;
				   else if(preCh == 0x0E2C) //move ahead 6 pixels
				   	iX -= 6;
				}

			}
			else if(IsThaiBottomLineChar(curCh))
			{
				//Bottom line characters
				iY += interLeaving;
				if(IsThaiBaseLineChar(preCh))
				{
					iX -= ((S32)nWidth+interLeaving);
					if(preCh == 0x0E2C)
						iX -= 2; //move ahead for 2 pixels
				}
			}
			else if(IsThaiToneMark(curCh))
			{
				 
				iY -= 1;
				if(IsThaiBaseLineChar(preCh))
				{
				 	iX -= ((S32)nWidth + interLeaving);
				 	if(preCh == 0x0E33)
				 	{
				 	 	iY -= 3; 
				 	 	if(IsThaiBaseLineChar(pre2Ch))
				 	 		iX -= 7;  

				 	}
				 	//else if(nextCharUCS2 == 0x0E33)
				 	 
				 	else if(IsThaiLongTailChar(preCh))
				 		iX -= 3;  
				 	else if(preCh == 0x0E0A || preCh == 0x0E0B)
				 		iX -= 2;
				 	else if(preCh == 0x0E2C)
				 		iX -= 6;
				}
				else if(IsThaiAboveLineChar(preCh))
				{
					iY -= 3;
					iX -=((S32)nWidth+interLeaving);
					if(IsThaiLongTailChar(pre2Ch))
						iX -= 3;
					else if(pre2Ch == 0x0E0A || pre2Ch == 0x0E0B)
						iX -=2;
					else if(pre2Ch == 0x0E2C)
						iX -= 6;
				}
				else if(IsThaiBottomLineChar(preCh))
				{
					iX -= ((S32)nWidth+interLeaving);
					if(IsThaiLongTailChar(pre2Ch))
						iX-=3;
				}
				else if(preCh == 0x0E47 || preCh == 0x0E4C)
				{
				 	 
				 	iX +=((S32)nWidth+interLeaving);
				}

			}
			//(1)Other cases: For Thai digits and base line sign, the only thing we have to do is moving cursor
			//(2)Show Characters
			ShowCharFuncDispatch(LineHeight, nWidth, nHgt, iX, iY, Font, BackGround, CharData, NumChar, Bordered);

		}
		else//non-Thai char
		{
			ShowCharFuncDispatch(LineHeight, nWidth, nHgt, iX, iY, Font, BackGround, CharData, NumChar, Bordered);
		}
	gdi_font_end();
	GDI_EXIT_CRITICAL_SECTION(ShowStackedChar)		
}


const L2RLangSSC l2RLangSSC[]=
{
	{SSC_ARABIC},
	{SSC_PERSIAN},
};

BOOL IsL2RMMIStyle(void)
{
	U16 index=0;
	U8    temp_str1[30];
	U8    temp_str2[30];


   AnsiiToUnicodeString((S8*)temp_str1, (S8*) gLanguageArray[gCurrLangIndex].aLangSSC);

   for (index=0; index< (sizeof(l2RLangSSC)/sizeof(l2RLangSSC[0])); index++)
   {
      AnsiiToUnicodeString((S8*)temp_str2, (S8*)l2RLangSSC[index].sscString);
      //pTemp = (U8 *) temp_str;

      if (UCS2Strcmp((const S8*) temp_str1, (const S8*) temp_str2)==0)
	  {
		  return MMI_TRUE;
	  }

   }

   return MMI_FALSE;

}

U32 ShowDebugString(S32 x, S32 y, stFontAttribute Font, U8 *String, S32 len,U32 LineHeight, U32 Bordered)
{
	GDI_ENTER_CRITICAL_SECTION(ShowDebugString)
	U8*	CharData;

	U32	NumChar;
	U32	Counter=0;
	S32	CurrentX;
	U16	nHgt;
	U16	nWidth;
	S32	nXOffset;
	S32	nYOffset;
	S32 yy;
	U16 unicode = 0;
#ifdef __MMI_HINDI_ALG__
	U16 nDWidth=0;
#endif
	
	U8   font_attr = 0;
	gdi_color border_color = 0,text_color = 0;

	if(gnCurrentFontAttrib&FONTATTRIB_ITALIC)
		font_attr |= FONTATTRIB_ITALIC;
	else if(gnCurrentFontAttrib&FONTATTRIB_OBLIQUE) 
		font_attr |= FONTATTRIB_OBLIQUE;
	
	if(Font.underline) 	font_attr |=FONTATTRIB_UNDERLINE;

	{
		if(Bordered)
		border_color = gdi_act_color_from_rgb(0xff,UI_text_border_color.r, UI_text_border_color.g, UI_text_border_color.b);
	
	text_color = gdi_act_color_from_rgb(0xff,UI_current_text_color.r, UI_current_text_color.g, UI_current_text_color.b);
		
	}
		
	gdi_font_begin(text_color,font_attr);
	
	CurrentX=x;
	while(len!=0)
	{
		if( (String[0] == '\0')  &&  (String[1] == '\0'))
				break;
		len--;

		unicode = String[0];
		unicode|=(String[1]<<8);

		if(FONT_TEST_CONTROL_CHAR(unicode) )
			unicode = 0x20;
		if(FONT_TEST_FORMAT_CHAR(unicode))
		{
			String += 2;
			continue;
		}
		#ifdef __MMI_HINDI_ALG__
			NumChar = GetFontdata(unicode, Font, &CharData,&nWidth,&nDWidth,&nHgt);
		#else
		NumChar = GetFontdata(unicode, Font, &CharData,&nWidth,&nHgt);
		#endif

		
		Counter++;

		if(LineHeight>0)
			yy=y+LineHeight-nHgt;
		else
			yy=y;

		nXOffset = CurrentX+nWidth;
		nYOffset = yy+nHgt;

		if(Bordered)
			gdi_show_char_bordered(CurrentX, yy, text_color,border_color,CharData, NumChar,nWidth,nHgt, font_attr);
		else if(!(gnCurrentFontAttrib&FONTATTRIB_BOLD && gpCurrentFont->pFontType[gnCurrentFont][gbFontType]&FONTATTRIB_BOLD) 
			|| !(gpCurrentFont->pFontType[gnCurrentFont][gbFontType]&FONTATTRIB_NORMAL)) 
		{
			gdi_show_char(CurrentX, yy, text_color,CharData, NumChar,nWidth,nHgt, font_attr);
		} 
		else 
		{
			gdi_show_char(CurrentX, yy-1, text_color,CharData, NumChar,nWidth,nHgt, font_attr);
			gdi_show_char(CurrentX, yy, text_color,CharData, NumChar,nWidth,nHgt, font_attr);
		}

	      CurrentX += nWidth;

		nHgt = (U8)LineHeight;
		String += 2;
	}
	gdi_font_end();

	GDI_RETURN (CurrentX);
	
	GDI_EXIT_CRITICAL_SECTION(ShowDebugString)
	
}

/*-----------------------------------------------------------------------*/
/*--------------------------- *** End *** -------------------------------*/
/*-----------------------------------------------------------------------*/

