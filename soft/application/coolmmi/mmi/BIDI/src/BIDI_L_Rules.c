/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
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
 *    bidi_l_rules.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   bidirectional algorithm engine 
 *
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * removed!
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 *****************************************************************************/

/**************************************************************

	FILENAME	: BIDI_L_Rules.c

 

	 

 

	DATE		: Aug-23-2004

**************************************************************/
#include "bididef.h"
#include "bidiprot.h"
#include "cswtype.h"

#ifdef __BIDI_CONSOLE__  
#include "commonprots.h"
#else
#include "unicodexdcl.h"
#endif

void resolve_lines(U8 paragraphEmbeddingLevel,U8 *inputString,U16 paraCount,pBOOL mirrorFlag,U8* pInput_Types,U8* pInput_Levels)
{
	U16 lineCount = 0;
	U16 *pBreak=0;

	do
	{
		// break lines at LS
		lineCount = bidi_resolve_lines(inputString,paraCount,pBreak);

		// resolve whitespace
		bidi_resolve_white_space(paragraphEmbeddingLevel, pInput_Types, pInput_Levels, lineCount);
    
		if (mirrorFlag)
		{
			get_mirror(inputString, pInput_Levels, lineCount);
		}

		// reorder each line in place
		bidi_reorder(paragraphEmbeddingLevel, inputString, pInput_Levels, lineCount);

 
//		inputString += lineCount;
		inputString += lineCount*BIDI_ENCODING_LENGTH;
		pInput_Levels += lineCount;
		pBreak += pBreak ? lineCount : 0;
		pInput_Types += lineCount;
		paraCount -= lineCount;

	} while (paraCount);
}

void bidi_resolve_white_space(U8 base_level, const U8 *pInput_Types, U8 *pInput_Level,U16 lineCount)
{
    U8 cur_level = 0,old_level = base_level;
	S16 iCount;
    
	for (iCount = 0; iCount < lineCount; iCount++)
    {
        switch(pInput_Types[iCount])
        {
			case WS:
				 
				//cur_level++;
				//break;
				 
			case RLE:
			case LRE:
			case LRO:
			case RLO:
			case PDF:
			case BN:
				pInput_Level[iCount] = old_level;
				cur_level++;
				break;
			case S:
			case BIDI_B:
				bidi_set_level_run(pInput_Level, cur_level, iCount, base_level);
				cur_level = 0;
				pInput_Level[iCount] = base_level;
				break;
			default:
				cur_level = 0;
				break;
        }
		old_level = pInput_Level[iCount];
    }
    bidi_set_level_run(pInput_Level, cur_level, iCount, base_level);
}

void bidi_set_level_run(U8 *pinput_level, U8 cur_level, U16 iStart, U8 base_level)
{
	S16 iCount;
    for (iCount = iStart - 1; iCount >= iStart - cur_level; iCount--)
    {
        pinput_level[iCount] = base_level;
    }
}

U16 bidi_resolve_lines(U8* pinput,U16 cCount, U16 *pBreak)
{
	S16 iCount;

	// skip characters not of type LS
	for(iCount = 0; iCount < (cCount); iCount++)
	{
		 
 
//		if ((pinput[iCount] == LS)||(pBreak && pBreak[iCount])||(pinput[iCount] == NL) )
		if (((pinput[iCount*BIDI_ENCODING_LENGTH] == LS)&&(pinput[(iCount*BIDI_ENCODING_LENGTH)+1] == 0))||(pBreak && pBreak[iCount])||((pinput[iCount*BIDI_ENCODING_LENGTH] == NL)&&(pinput[(iCount*BIDI_ENCODING_LENGTH)+1] == 0)))
 
		 
		{
			iCount++;
			break;
		}
	}

	return iCount;
}

void get_mirror(U8 *inputString,U8* input_levels,U16 lineCount)
{
	S16 iCount;
    for (iCount= 0; iCount < lineCount; iCount ++)
    {
        if (!ODD(input_levels[iCount])) continue;
		get_mirror_char(&inputString[iCount*BIDI_ENCODING_LENGTH]);
	}
}

pBOOL get_mirror_char(U8* pinput)
{
	U16 charInput=0; 
	S32	nFirst=	0;
	S32 nLast = nMaxIndexBidiMirror;
	S32 nMid;
#ifdef __UCS2_ENCODING
	U8 charLen;
#endif

#ifdef __ASCII
	charInput=(U16)(*pinput);
#endif
#ifdef __UCS2_ENCODING
	pfnEncodingSchemeToUnicode((U16*)&charInput,pinput);
#endif


	while(nLast>=nFirst)
	{
		nMid = (nFirst+nLast)/2;

		if(bidiCharMirror[nMid].bidiChar==charInput)
		{
			
#ifdef __ASCII
			*pinput=(U8)(bidiCharMirror[nMid].bidiMirror);
#endif
#ifdef __UCS2_ENCODING
			pfnUnicodeToEncodingScheme(bidiCharMirror[nMid].bidiMirror,&charLen,pinput);
#endif
			return TRUE;
		}
		else
		if(bidiCharMirror[nMid].bidiMirror==charInput)
		{
			
#ifdef __ASCII
			*pinput=(U8)(bidiCharMirror[nMid].bidiChar);
#endif
#ifdef __UCS2_ENCODING
			pfnUnicodeToEncodingScheme(bidiCharMirror[nMid].bidiChar,&charLen,pinput);
#endif
			return TRUE;
		}
		else
		{
			 
			//if(bidiCharMirror[nMid].bidiChar>charInput)
			if(bidiCharMirror[nMid].bidiChar<charInput)
			 
				nFirst = nMid+1;
			else
				nLast = nMid-1;
		}
	}
	return FALSE;
}

U16 bidi_reorder(U8 base_level, U8 *pinput,U8 *input_level, U16 cCount)
{
    U16 iCount = 0;

    while (iCount < cCount)
    {
 
        iCount += bidi_reorder_level(base_level, pinput + iCount*BIDI_ENCODING_LENGTH, input_level + iCount,(U16)(cCount - iCount),FALSE);
    }
    return iCount;
}


U16 bidi_reorder_level(U8 level, U8 *pinput,U8 *input_level,U16 cCount,pBOOL reverse_flag)
{
	U16 iCount;
	
    reverse_flag = reverse_flag || ODD(level);

    for (iCount = 0; iCount < cCount; iCount++)
    {
        if (input_level[iCount] < level)
        {
            break;
        }
        else if (input_level[iCount] > level)
        {
 
            iCount += bidi_reorder_level((U8)(level + 1), pinput + iCount*BIDI_ENCODING_LENGTH, input_level + iCount,(U16)(cCount - iCount), reverse_flag) - 1;
        }
    }
 
//    if (reverse_flag)
    if (reverse_flag && iCount)
 
    {
        bidi_reverse(pinput, iCount);
    }
 
	if (!iCount) iCount=1;
 
    return iCount;
}

void bidi_reverse(U8* pinput, U16 cCount)
{
    U8 cTemp;
 
	S16 iCount=0,jCount=0,kCount=0;
	pBOOL bFlag;

	while (iCount<cCount)
	{
		bFlag=FALSE;
		for (jCount=iCount;jCount<cCount;jCount++)
		{
			 if (pinput[jCount*BIDI_ENCODING_LENGTH]==0x0a && pinput[jCount*BIDI_ENCODING_LENGTH+1]==0x00)
			 {
//				 jCount--;
				 bFlag=TRUE;
				 break;
			 }
		}

		for (kCount = iCount; kCount < (iCount+(jCount-iCount)/2); kCount++)
    {
#ifdef __ASCII
			cTemp = pinput[kCount];
			pinput[kCount] = pinput[jCount-1-kCount];
			pinput[jCount-1-kCount] = cTemp;
#endif
#ifdef __UCS2_ENCODING
			cTemp = pinput[kCount*BIDI_ENCODING_LENGTH];
			pinput[kCount*BIDI_ENCODING_LENGTH] = pinput[(jCount+iCount-1-kCount)*BIDI_ENCODING_LENGTH];
			pinput[(jCount+iCount-1-kCount)*BIDI_ENCODING_LENGTH]= cTemp;
			cTemp = pinput[kCount*BIDI_ENCODING_LENGTH+1];
			pinput[kCount*BIDI_ENCODING_LENGTH+1] = pinput[(jCount+iCount-1-kCount)*BIDI_ENCODING_LENGTH+1];
			pinput[(jCount+iCount-1-kCount)*BIDI_ENCODING_LENGTH+1]= cTemp;
#endif
    }
		if (bFlag) jCount++;
		iCount=jCount;
    }
 
}


