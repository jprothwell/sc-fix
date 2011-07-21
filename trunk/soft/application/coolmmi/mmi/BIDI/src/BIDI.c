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
 *    bidi.c
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
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: BIDI.c

  	PURPOSE		: Main functions of BIDI Algorithm

 

 

	DATE		: Aug-23-2004

**************************************************************/
#ifndef __BIDI_CONSOLE__
#include "mmi_features.h"
#endif

#include "mmi_trace.h"
#define __MMI_BIDI__   //To be placed in MMI_features.h later
#ifdef __MMI_BIDI__

#include "stdc.h"
#include "bidiprot.h"
#include "bididef.h"

#ifdef __BIDI_CONSOLE__
#include "commonprots.h"
#else
#include "unicodexdcl.h"
#include "debuginitdef.h"
#endif


#define BIDI_CONSIDER_DIFF_INPUT_METHOD_BEHAVIOR 0

//void ArabicShapeEngine2(U16* len, U16* str, U16* Orgstr, U16* p);

U8 input_types[MAX_TEXT_LENGTH*BIDI_ENCODING_LENGTH];//source array for storing the direction codes of the input string
U8 input_levels[MAX_LEVEL_EMBEDDING];//This global contains all the embedding levels
U8 dest_levels[MAX_TEXT_LENGTH*BIDI_ENCODING_LENGTH];//destitnation array for processed levels
U8 dest_types[MAX_TEXT_LENGTH*BIDI_ENCODING_LENGTH];//Destination array for storing the direction codes of the input string
U8 localLogicalBuffer[MAX_TEXT_LENGTH * BIDI_ENCODING_LENGTH];

static U8 find_last_marker, find_cursor_marker, find_second_highlight_marker, find_first_highlight_marker;

const S8* bidiOverrideLMap[] =
{
    (S8 *) L":)",
    (S8 *) L":(",
    (S8 *) L":@",
    (S8 *) L"^ol",
    (S8 *) L":$",
    (S8 *) L";)",
    (S8 *) L"(L)",
    (S8 *) L"|-)",
    (S8 *) L"+o(",
    (S8 *) L":|",
    (S8 *) L":-s"
};
U16 nMaxIndexBidiOverride = sizeof(bidiOverrideLMap)/ sizeof(S8*);

 
#if defined __MMI_LANG_HINDI__  && defined __MMI_LANG_ARABIC__
extern S32 ARABIC_PRESENT;
extern BOOL cancelWidth;
#endif
 
#if 0//defined( __MMI_LANG_ARABIC__)||defined(__MMI_LANG_PERSIAN__)
#include "gbar_tran.h"
U16 arabicShapeTemp[640];
//#define TEST_SHAPE_DATA
extern GBT_SECTION gbTranSection;
 void ArabicShapeEngine2(U16* len, U16* str, U16* Orgstr, U16* p)
 {
#if defined(__MMI_GB__)
	*len = GBTransform(gbTranSection, str, arabicShapeTemp, sizeof(arabicShapeTemp) / sizeof(arabicShapeTemp[0]));
#endif
	pfnUnicodeStrcpy((S8*)str, (S8*)arabicShapeTemp);
	*len = pfnUnicodeStrlen((S8*)str);
 }

void ArabicShapeEngine(U16* len, U16* str)
{
	#ifdef TEST_SHAPE_DATA
	S32 str_len = pfnUnicodeStrlen((S8*)str);
	S32 i;
	for(i = 0; i < *len-1; i++)
		mmi_trace(g_sw_IM,"MMI_IME: before transform, str is: 0x%x", *(str+i));
	#endif
#if defined(__MMI_GB__)    
	*len = GBTransform(gbTranSection, str, arabicShapeTemp, sizeof(arabicShapeTemp) / sizeof(arabicShapeTemp[0]));
#endif
	pfnUnicodeStrcpy((S8*)str, (S8*)arabicShapeTemp);
	#ifdef TEST_SHAPE_DATA
	{
		str_len = pfnUnicodeStrlen((S8*)str);
		for(i = 0; i < *len-1; i++)
			mmi_trace(g_sw_IM,"MMI_IME: after transform, str is: 0x%x", *(str+i));
	}
	#endif
	*len = pfnUnicodeStrlen((S8*)str);
}

void ArabicStringShape(U16 *len, U16*str)
{
#if defined(__MMI_GB__)
	*len = GBTransform(gbTranSection, str, arabicShapeTemp, sizeof(arabicShapeTemp) / sizeof(arabicShapeTemp[0]));
#endif
	pfnUnicodeStrcpy((S8*)str, (S8*)arabicShapeTemp);
	*len = pfnUnicodeStrlen((S8*)str);
}

/*jinzh Add Start For ARABIC Ver: TBM780  on 2007-12-26 14:5 */
void ArabicStringShapeReverse(U16 *len, U16*str)
{
#if defined(__MMI_GB__)
	*len = GBTransformReverse(gbTranSection, str, arabicShapeTemp, sizeof(arabicShapeTemp) / sizeof(arabicShapeTemp[0]));
#endif
	pfnUnicodeStrcpy((S8*)str, (S8*)arabicShapeTemp);	
}
/*jinzh Add End  For ARABIC Ver: TBM780  on 2007-12-26 14:5 */
#endif
/*****************************************************************************
 * FUNCTION
 *  initial_bidi_map_array
 * DESCRIPTION
 *  Initialize the array with the continue index started from base.
 * PARAMETERS
 *  array             [OUT]       the source array
 *  count             [IN]        array length
 *  base              [IN]        the base index
 * RETURNS
 *  void
 *****************************************************************************/
static void initial_bidi_map_array(U16* array, U16 count, U16 base)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (array == NULL)
        return;

    for (i=0; i<count; i++)
        array[i] = i+base;
    
    return;
}


/*****************************************************************************
 * FUNCTION
 *  set_direction_using_override_map
 * DESCRIPTION
 *  replace the direction type by the override map
 * PARAMETERS
 *  inputTypes      [?]         
 *  inputString     [?]         
 *  sLen            [IN] 
 * RETURNS
 *  void
 *****************************************************************************/
static void set_direction_using_override_map(U8 *inputTypes, U8 *inputString, S16 sLen)
{
    U32 i, j, len;

    for(i=0; i<sLen;)
    {
        for(j=0; j<nMaxIndexBidiOverride; j++)
        {
            len = pfnUnicodeStrlen(bidiOverrideLMap[j]);
            
            if (i + len > sLen)
                return;

            if (pfnUnicodeStrncmp((const S8*)&inputString[i*ENCODING_LENGTH], bidiOverrideLMap[j], len) == 0)
            {
                /* find one override string */
                while(len > 0)
                {
                    inputTypes[i] = BIDI_L;
                    i++;
                    len--;
                }

                break;
            }
        }

        if (j >= nMaxIndexBidiOverride)
            i++;
    }
}

/*****************************************************************************
 * FUNCTION
 *  remove_one_member
 * DESCRIPTION
 *  remove one member from the source array
 * PARAMETERS
 *  array             [IN/OUT]        the source array
 *  pos               [IN]        the position of the member to be removed
 *  total_count       [IN]        the array length
 * RETURNS
 *  void
 *****************************************************************************/
static void remove_one_member(U16* array, U16 pos, U16 total_count)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 i, tmp;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (array == NULL || pos >= total_count)
        return;

    tmp = array[pos];

    for (i=0; i<pos; i++)
    {
        if (array[i] > tmp)
        {
            array[i]--;
        }
    }

    for (i=pos; i<total_count-1; i++)
    {
        array[i] = array[i+1];
        if (array[i] > tmp)
        {
            array[i]--;
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  bidi_main_ext
 * DESCRIPTION
 *  fill the string buffer to bidirectional algorithm
 * PARAMETERS
 *  logicalStr              [IN]        Input string for bidi
 *  logicalStrLen           [IN]        String length of input string
 *  logicalCurPos           [IN]        Current cursor position of input string
 *  visualStr               [OUT]       Final output string from bidi algorithm
 *  visualStrLen            [OUT]       String length of final output string
 *  visualCurPos            [OUT]       Cursor position of final output string
 *  pLToV                   [OUT]       Map table from Logic to Visual
 *  pVToL                   [OUT]       Map table from Visual to Logic
 *  inputMode               [IN]        the current input method direction mode
 *  paratype                [IN]        BIDI_PARAGRAPH_TYPES enum value pointed to the paragraph direction
 * RETURNS
 *  void
 *****************************************************************************/
void bidi_main_ext(
        U8 *logicalStr,
        U16 logicalStrLen,
        U16 logicalCurPos,
        U8 *visualStr,
        U16 *visualStrLen,
        U16 *visualCurPos,
        U16 *pLToV,
        U16 *pVToL,
        pBOOL fSwap,
        U8 inputMode,
        U8 fUseOverride,
        BIDI_PARAGRAPH_TYPES paratype)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    U8 paragraphEmbeddingLevel = 0;

#if BIDI_CONSIDER_DIFF_INPUT_METHOD_BEHAVIOR
    U8 lastType;
    pBOOL flag = FALSE;
#endif /* BIDI_CONSIDER_DIFF_INPUT_METHOD_BEHAVIOR */ 
    U16 bCheckFlag;
    S16 iCount, nParaCount = 0;
    U8 *pInputType = input_types;
    /* U8 *pInputLevels = input_levels; */
    U8 *pDestLevels = dest_levels;
    U8 *pDestTypes = dest_types;
    /* U8 *pLogicalBuffer = localLogicalBuffer; */
    U8 *pVisualBuffer = visualStr;
    U16 *p_logic_to_visual = pLToV;
    U16 *p_visual_to_logic = pVToL;
    U16 i;
    U16 mark;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /*
    memset(input_types, 0,sizeof(input_types));
    memset(input_levels, 0,sizeof(input_levels));
    memset(dest_levels, 0,sizeof(dest_levels));
    memset(dest_types, 0,sizeof(dest_types));
    memset(localLogicalBuffer, 0,sizeof(localLogicalBuffer));
    */

    if (logicalStrLen == 0)
    {
        /* *visualCurPos = 0; */
        *visualCurPos = 0xFFFF;
        *visualStr = 0;
        *(visualStr + 1) = 0;
        *visualStrLen = 0;
        return;
    }

    find_last_marker = 0; 
    find_cursor_marker = 0;
    find_second_highlight_marker = 0;
    find_first_highlight_marker = 0;

 
/*code removed*/
 
    pfnUnicodeStrncmp((S8*) localLogicalBuffer, (const S8*)logicalStr, (U32) logicalStrLen);
    localLogicalBuffer[logicalStrLen * ENCODING_LENGTH] = 0;
    localLogicalBuffer[logicalStrLen * ENCODING_LENGTH + 1] = 0;

#if BIDI_CONSIDER_DIFF_INPUT_METHOD_BEHAVIOR
    if (inputMode == BIDI_R)
    {
        bidi_get_char_type((U8*) & localLogicalBuffer[(logicalStrLen - 1) * BIDI_ENCODING_LENGTH], (U8*) & lastType);
        if (lastType == WS)
        {
            localLogicalBuffer[(logicalStrLen - 1) * BIDI_ENCODING_LENGTH] = 0;
            localLogicalBuffer[(logicalStrLen - 1) * BIDI_ENCODING_LENGTH + 1] = 0;
            if (logicalCurPos == logicalStrLen)
            {
                flag = TRUE;
                logicalCurPos--;
            }
            logicalStrLen--;
        }

        if (logicalCurPos != 0xFFFF)
        {
            set_bidi_cursor_pos((U8*) localLogicalBuffer, logicalCurPos, (U16) CURSOR_MARK);
            find_cursor_marker = 1;
        }

        logicalStrLen = (U16) pfnUnicodeStrlen((const S8*)localLogicalBuffer);

        set_bidi_cursor_pos((U8*) localLogicalBuffer, logicalStrLen, (U16) LAST_MARK);
        
        find_last_marker = 1;
    }
    else
#endif /* BIDI_CONSIDER_DIFF_INPUT_METHOD_BEHAVIOR */ 
    {
        if (logicalCurPos != 0xFFFF)
        {
            set_bidi_cursor_pos((U8*) localLogicalBuffer, logicalCurPos, (U16) CURSOR_MARK);
            find_cursor_marker = 1;
        }
        find_last_marker = 0;
    }

    logicalStrLen = get_direction_codes((U8*) input_types, (U8*) localLogicalBuffer, 0);

    if (fUseOverride)
        set_direction_using_override_map((U8*) input_types, (U8*) localLogicalBuffer, logicalStrLen);
    
    iCount = 0;
    /* modified by mbj06060, we only use one line to apply bidi*/
    /* while (iCount < logicalStrLen) */
    {
        bCheckFlag = 0;
        nParaCount = identify_paragraphs(pInputType, (U16) (logicalStrLen - iCount), &bCheckFlag);

        if (paratype == P_ON)
            paragraphEmbeddingLevel = get_para_embedding_level(pInputType, nParaCount);
        else
            paragraphEmbeddingLevel = paratype;
        
        if (bCheckFlag == 2)
        {
            /* This line is ended by 0x0d0a, we handle these two letters specially*/
            nParaCount--;
            
            if (paragraphEmbeddingLevel == 1) /* R to L*/
            {
                pfnUnicodeStrncmp((S8*)visualStr, (const S8*)&localLogicalBuffer[(logicalStrLen - 2)*ENCODING_LENGTH], 2);
                pVisualBuffer += 2 * ENCODING_LENGTH;
                pfnUnicodeStrncmp((S8*) pVisualBuffer, (const S8*)localLogicalBuffer, logicalStrLen - 2);
                pVisualBuffer[(logicalStrLen - 2) * ENCODING_LENGTH] = 0;
                pVisualBuffer[(logicalStrLen - 2) * ENCODING_LENGTH + 1] = 0;
                if (p_visual_to_logic)
                {
                    p_visual_to_logic[0] = nParaCount;
                    p_visual_to_logic[1] = nParaCount + 1;
                    p_visual_to_logic += 2;
                }
            }
            else /* L to R*/
            {
                /* we don't bidi 0x0d0a */
                if (p_visual_to_logic)
                {
                    p_visual_to_logic[nParaCount] = nParaCount;
                    p_visual_to_logic[nParaCount + 1] = nParaCount + 1;
                }
                pfnUnicodeStrcpy((S8*) visualStr, (const S8*)localLogicalBuffer);
            }
        }
        else
            pfnUnicodeStrcpy((S8*) visualStr, (const S8*)localLogicalBuffer);
        
        if (bCheckFlag  == 1)
        {
            pDestTypes[nParaCount] = 0;
            pInputType[nParaCount] = 0;
            nParaCount++;
        }
        
        copy_direction_codes(pDestTypes, pInputType, (U16) nParaCount);

        if (p_visual_to_logic !=NULL)
            initial_bidi_map_array(p_visual_to_logic, nParaCount, 0);

        set_dest_levels(0, nParaCount, paragraphEmbeddingLevel, pDestLevels);

        get_run_level(nParaCount, paragraphEmbeddingLevel, pDestLevels, pDestTypes);

        resolve_lines(paragraphEmbeddingLevel, pVisualBuffer, nParaCount, (pBOOL)!fUseOverride, pDestTypes, pDestLevels);

        /* If we only take account one line bidi, needn't to do the following work */
/*     
        pInputType += nParaCount;
        pInputLevels += nParaCount;
        pDestLevels += nParaCount;
        pDestTypes += nParaCount;
        pLogicalBuffer += nParaCount * 2;
        pVisualBuffer += nParaCount * 2;
        iCount += nParaCount;

        if (p_visual_to_logic != NULL)
            p_visual_to_logic += nParaCount;
*/
    }

    *visualStrLen = (U16) pfnUnicodeStrlen((const S8*)visualStr);
    p_visual_to_logic = pVToL;

    if(fSwap)
    {
        bidi_reverse((U8*) visualStr, *visualStrLen);
    }

    *visualCurPos = 0xFFFF;
     
    while (find_mark(visualStr, *visualStrLen, &mark, (U16*) & iCount))
         
    {
        switch (mark)
        {
            case LAST_MARK:
            case CURSOR_MARK:
        #if BIDI_CONSIDER_DIFF_INPUT_METHOD_BEHAVIOR
                if (inputMode == BIDI_R)
                {
                    *visualCurPos = get_bidi_cursor_pos(visualStr, (U16) LAST_MARK);

                    if (lastType == WS)
                    {
                        set_bidi_cursor_pos(visualStr, *visualCurPos, (U16) (0x0020));
                    }
                    else
                    {
                        /* get_bidi_cursor_pos remove one visual character, so remove one from p_visual_to_logic*/
                        remove_one_member(p_visual_to_logic, *visualCurPos, *visualStrLen);
                    }

                    *visualCurPos = get_bidi_cursor_pos(visualStr, (U16) CURSOR_MARK);
                    remove_one_member(p_visual_to_logic, *visualCurPos, *visualStrLen);
                }
                else
        #endif /* BIDI_CONSIDER_DIFF_INPUT_METHOD_BEHAVIOR */ 
                {
                    *visualCurPos = get_bidi_cursor_pos(visualStr, (U16) CURSOR_MARK);
                    remove_one_member(p_visual_to_logic, *visualCurPos, *visualStrLen);
                }
                break;
        }
    }

    *visualStrLen = (U16) pfnUnicodeStrlen((const S8*)visualStr);

    /* fill p_logic_to_visual */
    if (p_logic_to_visual != NULL && p_visual_to_logic != NULL)
    {
        for (i = 0; i<*visualStrLen; i++)
            p_logic_to_visual[p_visual_to_logic[i]] = i;
    }

}
/*----------------------------------------------------------------------------
Function:			bidi_main
Description:		fill the string buffer to bidirectional algorithm
Input Parameters:	logicalStr				input string for bidi
						logicalStrLen			string length of input string
						logicalCurPos			current cursor position of input string
						fSwap					   if true, then reserse the string after apply bidi algorithm
						inputMode
						logicalHilightStart	word hilight start position of input string
						logicalHilightEnd		word hilight end position of input string
Output Parameters:
						visualStr				final output string from bidi algorithm
						visualStrLen			string length of final output string
						visualCurPos			cursor position of final output string
						visualHilightStart	word hilight start position of output string
						visualHilightEnd		word hilight end position of output string

Returns:			void
----------------------------------------------------------------------------*/

void bidi_main(U8* logicalStr,U16 logicalStrLen,U16 logicalCurPos,U8* visualStr,
			   U16* visualStrLen,U16* visualCurPos,pBOOL fSwap,U8 inputMode,
			   U16 logicalHilightStart,U16 logicalHilightEnd,
			   U16 *visualHilightStart, U16 *visualHilightEnd)
{

   U8 paragraphEmbeddingLevel=0;
   #if BIDI_CONSIDER_DIFF_INPUT_METHOD_BEHAVIOR
	U8 lastType;
	pBOOL flag=FALSE;
	#endif
	pBOOL bCheckFlag;
	S16 iCount,nParaCount=0;
	U8* pInputType=input_types;
	U8* pInputLevels=input_levels;
	U8* pDestLevels=dest_levels;
	U8* pDestTypes=dest_types;
	U8* pLogicalBuffer=localLogicalBuffer;
	U8* pVisualBuffer=visualStr;
	U16 mark;
	 
	U8 newLineflag=0;
	 
	U8 testHilighOneCharFlag = 0;

   memset(input_types,sizeof(input_types),0);
   memset(input_levels,sizeof(input_levels),0);
   memset(dest_levels,sizeof(dest_levels),0);
   memset(dest_types,sizeof(dest_types),0);
   memset(localLogicalBuffer,sizeof(localLogicalBuffer),0);

   if (logicalStrLen == 0 )
   {
	   //*visualCurPos = 0;
	   *visualCurPos = 0xFFFF;
	   *visualStr = 0;
	   *(visualStr+1) = 0;
	   *visualStrLen = 0;
	   *visualHilightStart = 0;
	   *visualHilightEnd = 0;
	   return;
   }
    find_last_marker = 0; 
    find_cursor_marker = 0;
    find_second_highlight_marker = 0;
    find_first_highlight_marker = 0;
    
	if(logicalCurPos != 0xFFFF)
	{
		if (logicalCurPos < logicalHilightEnd) logicalCurPos=logicalHilightEnd;
		if (logicalHilightStart > logicalHilightEnd) logicalHilightStart=logicalHilightEnd;
	}

  #if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
			#if defined __MMI_LANG_HINDI__  && defined __MMI_LANG_ARABIC__
/* under construction !*/
/* under construction !*/
         #endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

	pfnUnicodeStrncpy((S8 *)localLogicalBuffer,(const S8 *)logicalStr, (U32)logicalStrLen );

#if BIDI_CONSIDER_DIFF_INPUT_METHOD_BEHAVIOR
	if (inputMode==BIDI_R)
	{
		bidi_get_char_type((U8 *)&localLogicalBuffer[(logicalStrLen-1)*BIDI_ENCODING_LENGTH],(U8 *)&lastType);
		if(lastType==WS)
		{
			localLogicalBuffer[(logicalStrLen-1)*BIDI_ENCODING_LENGTH]=0;
			localLogicalBuffer[(logicalStrLen-1)*BIDI_ENCODING_LENGTH+1]=0;
			if (logicalCurPos==logicalStrLen)
			{
				flag=TRUE;
				logicalCurPos--;
			}
			logicalStrLen--;
		}

		set_bidi_cursor_pos((U8 *)localLogicalBuffer,logicalCurPos,(U16)CURSOR_MARK);

		logicalStrLen=(U16)pfnUnicodeStrlen((const S8*)localLogicalBuffer);
		if (logicalCurPos != 0xFFFF)
		{
			set_bidi_cursor_pos((U8 *)localLogicalBuffer,logicalStrLen,(U16)LAST_MARK);
			find_cursor_marker = 1;
		}
	}
	else
#endif
	{
		if(logicalCurPos != 0xFFFF )
		{
			set_bidi_cursor_pos((U8 *)localLogicalBuffer,logicalCurPos,(U16)CURSOR_MARK);
			find_cursor_marker = 1;
		}
	}

	set_bidi_cursor_pos((U8 *)localLogicalBuffer,logicalHilightEnd,(U16)SECOND_HIGHLT);
	set_bidi_cursor_pos((U8 *)localLogicalBuffer,(U16)(logicalHilightStart),(U16)FIRST_HIGHLT);
	find_second_highlight_marker = 1;
	find_first_highlight_marker = 1;
    
	pfnUnicodeStrcpy((S8 *)visualStr,(const S8 *)localLogicalBuffer);
	logicalStrLen=get_direction_codes((U8 *)input_types,(U8 *)localLogicalBuffer,0);


	iCount=0;

	while (iCount<logicalStrLen)
	{
		bCheckFlag=FALSE;
		nParaCount=identify_paragraphs(pInputType,(U16)(logicalStrLen-iCount),&bCheckFlag);
		if (logicalCurPos < nParaCount)
			newLineflag = 1;
		copy_direction_codes(pDestTypes,pInputType,(U16)nParaCount);
		if (bCheckFlag)
		{
			pDestTypes[nParaCount] = 0;
			pInputType[nParaCount] = 0;
			if (pInputType[nParaCount-1] == BIDI_B)
			{
				pDestTypes[nParaCount-1] = 0;
				pInputType[nParaCount-1] = 0;
			}
		}
		paragraphEmbeddingLevel=get_para_embedding_level(pInputType,nParaCount);
		set_dest_levels(0,nParaCount,paragraphEmbeddingLevel,pDestLevels);


		get_run_level(nParaCount,paragraphEmbeddingLevel,pDestLevels,pDestTypes);
		resolve_lines(paragraphEmbeddingLevel,pVisualBuffer,nParaCount,TRUE,pDestTypes,pDestLevels);

		if (bCheckFlag) nParaCount++;
		pInputType+=nParaCount;
		pInputLevels+=nParaCount;
		pDestLevels+=nParaCount;
		pDestTypes+=nParaCount;
		pLogicalBuffer+=nParaCount*2;
		pVisualBuffer+=nParaCount*2;
		iCount+=nParaCount;
	}

   *visualStrLen=(U16)pfnUnicodeStrlen((const S8 *)visualStr);

   if(fSwap)
   {
	   bidi_reverse((U8*)visualStr,*visualStrLen);
   }

   *visualCurPos = 0xFFFF;
  
  while (find_mark(visualStr,*visualStrLen,&mark,(U16*)&iCount))
  
   {
		switch (mark)
		{
		case LAST_MARK:
		case CURSOR_MARK:
			#if BIDI_CONSIDER_DIFF_INPUT_METHOD_BEHAVIOR
			if (inputMode==BIDI_R)
			{
				*visualCurPos=get_bidi_cursor_pos(visualStr,(U16)LAST_MARK);

				if(lastType==WS)
				{
					set_bidi_cursor_pos(visualStr,*visualCurPos,(U16)(0x0020));
				}

				*visualCurPos=get_bidi_cursor_pos(visualStr,(U16)CURSOR_MARK);
			}
			else
			#endif
			{
				*visualCurPos=get_bidi_cursor_pos(visualStr,(U16)CURSOR_MARK);
			}
			testHilighOneCharFlag = 1;
			break;
		case SECOND_HIGHLT:
			*visualHilightEnd = get_bidi_cursor_pos(visualStr,(U16)SECOND_HIGHLT);
			testHilighOneCharFlag = 0;
			break;
		case FIRST_HIGHLT:
			*visualHilightStart = get_bidi_cursor_pos(visualStr,(U16)FIRST_HIGHLT);
			testHilighOneCharFlag = 0;

			break;
		}
   }

	if(testHilighOneCharFlag == 0 && *visualHilightEnd == *visualHilightStart)
	{
		*visualHilightStart = *visualCurPos;
		*visualHilightEnd = *visualHilightStart + 1; //changed by panxu 2007-12-24
	}

   if ( (logicalHilightStart==0) && (logicalHilightEnd==0) )
   {
	   *visualHilightEnd = 0;
	   *visualHilightStart = 0;
   }


	if ((*visualHilightEnd </*=*/ *visualHilightStart) /*&& (!(*visualHilightStart == 0 && *visualHilightEnd == 0))*/)
	{
		(*visualHilightEnd)++;
		(*visualHilightStart)++;
	}



	*visualStrLen=(U16)pfnUnicodeStrlen((const S8 *)visualStr);
}

pBOOL set_bidi_cursor_pos(U8* input_str,U16 cursor_pos,U16 cursor_mark)
{
	U16 sLen,iCount;
	U8 cTemp[2];

	pfnUnicodeToEncodingScheme(cursor_mark,(U8*)&sLen,cTemp);

	sLen=(U16)pfnUnicodeStrlen((const S8 *)input_str);

	if (sLen<cursor_pos) return FALSE;

	for(iCount=cursor_pos;iCount<=sLen;iCount++)
	{
		U8 cTempSwap;

		cTempSwap=input_str[iCount*BIDI_ENCODING_LENGTH];
		input_str[iCount*BIDI_ENCODING_LENGTH] = cTemp[0];
		cTemp[0] = cTempSwap;
		cTempSwap=input_str[iCount*BIDI_ENCODING_LENGTH + 1];
		input_str[iCount*BIDI_ENCODING_LENGTH + 1] = cTemp[1];
		cTemp[1] = cTempSwap;
	}

	input_str[iCount*BIDI_ENCODING_LENGTH]=cTemp[0];
	input_str[iCount*BIDI_ENCODING_LENGTH+1]=cTemp[1];

	return TRUE;
}
 
pBOOL find_mark(U8* input_str,U16 input_strLen,U16* cur_mark,U16* cur_pos)
{
	U16 sLen,iCount=0;
	U8 cTemp[2];
	while(!((input_str[iCount*2]==0)&&(input_str[iCount*2+1]==0)) && iCount<=input_strLen)
 
	{
		if(find_last_marker)
		{   
			pfnUnicodeToEncodingScheme(LAST_MARK,(U8*)&sLen,cTemp);
			if((input_str[iCount*BIDI_ENCODING_LENGTH]==cTemp[0])&&(input_str[iCount*BIDI_ENCODING_LENGTH+1]==cTemp[1]))
			{
				*cur_mark=LAST_MARK;
				*cur_pos=iCount;
				return TRUE;
			}
		}
        
		if(find_cursor_marker)
		{ 
			pfnUnicodeToEncodingScheme(CURSOR_MARK,(U8*)&sLen,cTemp);
			if((input_str[iCount*BIDI_ENCODING_LENGTH]==cTemp[0])&&(input_str[iCount*BIDI_ENCODING_LENGTH+1]==cTemp[1]))
			{
				*cur_mark=CURSOR_MARK;
				*cur_pos=iCount;
				return TRUE;
			}
		}
        
		if(find_second_highlight_marker)
		{         
			pfnUnicodeToEncodingScheme(SECOND_HIGHLT,(U8*)&sLen,cTemp);
			if((input_str[iCount*BIDI_ENCODING_LENGTH]==cTemp[0])&&(input_str[iCount*BIDI_ENCODING_LENGTH+1]==cTemp[1]))
			{
				*cur_mark=SECOND_HIGHLT;
				*cur_pos=iCount;
				return TRUE;
			}
		}
        
		if(find_first_highlight_marker)
		{        
			pfnUnicodeToEncodingScheme(FIRST_HIGHLT,(U8*)&sLen,cTemp);
			if((input_str[iCount*BIDI_ENCODING_LENGTH]==cTemp[0])&&(input_str[iCount*BIDI_ENCODING_LENGTH+1]==cTemp[1]))
			{
				*cur_mark=FIRST_HIGHLT;
				*cur_pos=iCount;
				return TRUE;
			}
		}
        
		iCount++;
	}
	return FALSE;
}

U16 get_bidi_cursor_pos(U8*  input_str,U16 cursor_mark)
{
	U16 sLen,iCount=0;
	U8 cTemp[2];
	S16 cur_pos=-1;

	pfnUnicodeToEncodingScheme(cursor_mark,(U8*)&sLen,cTemp);

//	while((input_str[iCount*BIDI_ENCODING_LENGTH]!=0)&&(input_str[iCount*BIDI_ENCODING_LENGTH+1]!=0))
	while(!((input_str[iCount*2]==0)&&(input_str[iCount*2+1]==0)))
	{
		if((input_str[iCount*BIDI_ENCODING_LENGTH]==cTemp[0])&&(input_str[iCount*BIDI_ENCODING_LENGTH+1]==cTemp[1]))
			cur_pos=iCount;
		if(cur_pos!=-1)
		{
			input_str[iCount*BIDI_ENCODING_LENGTH] = input_str[(iCount+1)*BIDI_ENCODING_LENGTH];
			input_str[iCount*BIDI_ENCODING_LENGTH+1] = input_str[(iCount+1)*BIDI_ENCODING_LENGTH+1];
		}
		iCount++;
	}
	return (U16)((cur_pos==-1)? 0:cur_pos);
}

pBOOL bidi_get_char_type(U8* input_char,U8* charType)
{
	S32	nFirst=	0;
	S32 nLast = nMaxIndexBidiTypeMap;
	S32 nMid;

	U16 charInput=0;

#ifdef __ASCII
	charInput=(U16)(*input_char);
#endif
#ifdef __UCS2_ENCODING
	pfnEncodingSchemeToUnicode((U16*)&charInput,(U8*)(input_char));
#endif

	while(nLast>=nFirst)
	{
		nMid = (nFirst+nLast)/2;
		 
//		if((bidiCharTypeMap[nMid].sIndex<= charInput)&&((bidiCharTypeMap[nMid].sIndex+ (U16)bidiCharTypeMap[nMid].nCount)>= charInput))
		if((bidiCharTypeMap[nMid].sIndex<= charInput)&&((bidiCharTypeMap[nMid].sIndex+ (U16)bidiCharTypeMap[nMid].nCount)> charInput))
		 
		{
			*charType=bidiCharTypeMap[nMid].type;
			return TRUE;
		}
		 
//		if (charInput>(bidiCharTypeMap[nMid].sIndex+ (U16)bidiCharTypeMap[nMid].nCount))
		if (charInput>=(bidiCharTypeMap[nMid].sIndex+ (U16)bidiCharTypeMap[nMid].nCount))
		{
		    
			nFirst = nMid+1;
		}
		else
		{
			if (charInput<bidiCharTypeMap[nMid].sIndex)
				nLast = nMid-1;
			else
				nLast=nMid;
		}
	}


	*charType=BIDI_L;//Default Type
	return FALSE;
}

U8 bidi_get_para_direction(U8* input_str)
{
	//U8 input_types[MAX_TEXT_LENGTH*BIDI_ENCODING_LENGTH];//source array for storing the direction codes of the input string
	U16 sLen;

 
//   sLen=get_direction_codes(input_types,input_str);
   sLen=get_direction_codes(input_types,input_str,0);
 

   return (get_para_embedding_level(input_types,sLen))? BIDI_R:BIDI_L;
}
#endif

