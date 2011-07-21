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
 *    bididef.h
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
 * removed!
 * removed!
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

	FILENAME	: BIDIDef.h

  	PURPOSE		: BIDI Defines

 

 

	DATE		: Aug-23-2004

**************************************************************/

#ifndef _BIDIDEF_H
#define _BIDIDEF_H

#ifdef __BIDI_CONSOLE__    //To include file common.h in projects BidiConsole and GenBidiCodeMap
	#include "common.h"
#else
	#include "mmi_data_types.h"
#endif

typedef enum
{			
    ON = 0,  // Other Neutral 
    BIDI_L,       // Left Letter
    BIDI_R,       // Right Letter
    AN,      // Arabic Number
    EN,      // European Number
    AL,      // Arabic Letter (Right-to-left)
    NSM,      
    CS,      // Common Separator
    ES,      // European Separator
    ET,      // European Terminator 
    BN,      // Boundary neutral    
    S,       // Segment Separator (TAB)		// used only in L1
    WS,      // White space					// used only in L1
    BIDI_B,       
    RLO,     // these are used only in X1-X9
    RLE,
    LRO,
    LRE,
    PDF,
    N = ON  // alias, where ON, WS and S are treated the same
}PMT_BIDI_TYPES;

typedef enum
{
    P_LTR = 0,
    P_RTL,
    P_ON
}BIDI_PARAGRAPH_TYPES;  /* mbj06060 */

typedef struct _bidichar_type_map
{
	U8 type; //Type of Unicode Characters
	U8 nCount;//Total number of unicode characters of same type starting from sIndex
	U16 sIndex;//Starting Index of Unicode Character
}BIDICHAR_TYPE_MAP;

typedef struct _bidichar_mirror_map
{
	U16 bidiChar;
	U16 bidiMirror;
}BIDICHAR_MIRROR_MAP;

//#define DEFAULT_BIDI_TYPE BIDI_L
#define MAX_LEVEL_EMBEDDING			  61
#define MAX_TEXT_LENGTH              1600//160 
#define LS 0x0d
#define NL	0x0a//bakshi
#define ODD(x) ((x) & 1)
#define EVEN(x) (((x) & 1) ? 0:1)
#define GETMAX(x,y) (( x > y) ? x:y)
 
#define CURSOR_MARK (0x0006) //(0xFFA1) 
#define LAST_MARK (0xFFA2)  //(0x0007)

 
#define FIRST_HIGHLT		0x0008 //0xFFA3 
#define SECOND_HIGHLT	0x0009 //0xFFA4 
 
 

#ifdef __UCS2_ENCODING
#define BIDI_ENCODING_LENGTH 2
#else
#define BIDI_ENCODING_LENGTH 1
#endif

extern void ArabicStringShape(U16*, U16*);
#endif//_BIDIDEF_H

