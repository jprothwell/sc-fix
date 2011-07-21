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
 *    bidiprot.h
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
 *******************************************************************************/

/**************************************************************

	FILENAME	: BIDIProt.h

  	PURPOSE		: BIDI function prototypes.

 

 

	DATE		: Aug-23-2004

**************************************************************/
#ifndef _BIDIPROTS_H
#define _BIDIPROTS_H

#include "bididef.h"

extern U16 nMaxIndexBidiTypeMap;
extern U16 nMaxIndexBidiMirror;
extern BIDICHAR_TYPE_MAP bidiCharTypeMap[];
extern BIDICHAR_MIRROR_MAP bidiCharMirror[];

//BIDI.c
 
void bidi_main(U8* logicalStr,U16 logicalStrLen,U16 logicalCurPos,U8* visualStr,U16* visualStrLen,
			   U16* visualCurPos,pBOOL fSwap,U8 inputMode,
			   U16 logicalHilightStart,U16 logicalHilightEnd,U16 *visualHilightStart,
			   U16 *visualHilightEnd);
 
pBOOL bidi_get_char_type(U8* input_char,U8* charType);
U8 bidi_get_para_direction(U8* input_str);
pBOOL set_bidi_cursor_pos(U8* input_str,U16 cursor_pos,U16 cursor_mark);
U16 get_bidi_cursor_pos(U8*  input_str,U16 cursor_mark);
 
 
pBOOL find_mark(U8* input_str,U16 input_strLen,U16* cur_mark,U16* cur_pos);
 


//BIDIProcessDataTypes.c
 
U16 delete_explicit_direction_codes(U8* inputTypes,U16 inputLen,U8* all_embeddings_level,U8* dest_levels,U8* dest_types);
void  get_run_level(U16 inputLen,U8 paragraphEmbeddingLevel,U8* dest_levels,U8* dest_types);
 

void processWeakTokens(U16 begin,U16 range, U16 currentLevel,U8 sor,U8 eor,U8* dest_types);
U16 get_run_rate(U8* dest_types,U16 base, U16 rate, U8* correctType,U16 length);//return the base if match is not found
void setTokens(U8* dest_types,U16 begin, U16 range,U8 newType);//check
void processNeutralTokens(U16 begin, U16 range, U16 currentLevel, U8 sor, U8 eor,U8* dest_types);
void processImplicitLevels(U16 begin, U16 range, U16 currentLevel, U8 sor, U8 eor,U8* dest_levels,U8* dest_types);
U16 insert_explicit_direction_codes(U8* inputTypes,U16 inputLen,U8 paragraphEmbeddingLevel,U8* all_embeddings_level,U8* dest_levels,U8* dest_types);


//BIDIParaRules.c
 
//U16 get_direction_codes(U8* inputTypes,U8* inputString);
U16 get_direction_codes(U8* inputTypes,U8* inputString,S16 sLen);
//U16 identify_paragraphs(U16 inputLen);
U16 identify_paragraphs(U8* inputTypes,U16 inputLen,pBOOL* bFlag);
 
U8 get_para_embedding_level(U8* inputTypes,U16 inputLen);
U8 getLeastOddEmbeddingLevel(U8 pEmbeddinglevel);
U8 getLeastEvenEmbeddingLevel(U8 pEmbeddinglevel);
 
//void resolve_explicit_embedding_levels(U16 inputLen,U8 paraEmbeddingLevel/*,U8* all_embeddings_level*/);
void resolve_explicit_embedding_levels(U8* inputTypes,U16 inputLen,U8 paraEmbeddingLevel);//,U8* all_embeddings_level);
 
 
 
void set_dest_levels(U16 startCount,U16 endCount,U8 paraEmbedLevel,U8* dest_levels);
 
void copy_direction_codes(U8* dTypes,U8* sTypes,U16 nCount);

//BIDI_L_Rules.c
 
//void resolve_lines(U8 paragraphEmbeddingLevel,U8 *inputString,U8* direction_codes,U8* dest_levels,U16 startPara,pBOOL mirrorFlag);
void resolve_lines(U8 paragraphEmbeddingLevel,U8 *inputString,U16 paraCount,pBOOL mirrorFlag,U8* pInput_Types,U8* pInput_Levels);
 
void bidi_resolve_white_space(U8 base_level, const U8 *input_types, U8 *input_level,U16 lineCount);
void bidi_set_level_run(U8* pinput_level,U8 cur_level, U16 iStart, U8 base_level);
U16 bidi_resolve_lines(U8* pinput,U16 cCount, U16 *pBreak);
void get_mirror(U8 *inputString,U8* input_levels,U16 lineCount);
pBOOL get_mirror_char(U8* pinput);
U16 bidi_reorder(U8 base_level, U8 *pinput,U8 *input_level, U16 cCount);
U16 bidi_reorder_level(U8 level, U8 *pinput,U8 *input_level,U16 cCount,pBOOL reverse_flag);
void bidi_reverse(U8* pinput, U16 cCount);

#endif //_BIDIGPROTS_H

