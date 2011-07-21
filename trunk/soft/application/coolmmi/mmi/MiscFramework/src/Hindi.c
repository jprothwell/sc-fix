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
 *    Hindi.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   C file of hindi font rule 
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
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#include "mmi_features.h"
#ifndef __MMI_HINDI_H__
#define __MMI_HINDI_H__

#include "gui_data_types.h"
#include  "hf_rules.h"
//#include "IMEGprot.h"
/*+ zhouqin modify for bengali 20110419*/
//#if defined (__MMI_LANG_HINDI__) || defined(__MMI_LANG_MARATHI__)
#if defined (__MMI_HINDI_ALG__)
/*- zhouqin modify for bengali 20110419*/
#ifndef NULL
#define NULL   0
#endif 

const ire_character_info_table_struct hindi_character_info_table[] = 
{
    /* INDEPENDENT VOWEL */
    {0, 0, 0, IRE_NORMAL_SIGN, 0, 0, IRE_ABOVE_BASE_POSITION, 0, IRE_CHAR_TYPE_1, 0},   /* 0x0901 */
    {0, 0, 0, IRE_NORMAL_SIGN, 0, 0, IRE_ABOVE_BASE_POSITION, 0, 0, 0},                 /* 0x0902 */
    {0, 0, 0, IRE_NORMAL_SIGN, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                   /* 0x0903 */

    {0, 0, IRE_INDEP_VOWEL, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_2, 0},     /* 0x0904 */
    {0, 0, IRE_INDEP_VOWEL, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_1, 0},     /* 0x0905 */
    {0, 0, IRE_INDEP_VOWEL, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_1, 0},     /* 0x0906 */
    {0, 0, IRE_INDEP_VOWEL, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_1, 0},     /* 0x0907 */
    {0, 0, IRE_INDEP_VOWEL, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_2, 0},     /* 0x0908 */
    {0, 0, IRE_INDEP_VOWEL, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_1, 0},     /* 0x0909 */
    {0, 0, IRE_INDEP_VOWEL, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_1, 0},     /* 0x090A */
    {0, 0, IRE_INDEP_VOWEL, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_1, 0},     /* 0x090B */
    {0, 0, IRE_INDEP_VOWEL, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_2, 0},     /* 0x090C */
    {0, 0, IRE_INDEP_VOWEL, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_2, 0},     /* 0x090D */
    {0, 0, IRE_INDEP_VOWEL, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_2, 0},     /* 0x090E */
    {0, 0, IRE_INDEP_VOWEL, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_1, 0},     /* 0x090F */
    {0, 0, IRE_INDEP_VOWEL, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_2, 0},     /* 0x0910 */
    {0, 0, IRE_INDEP_VOWEL, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_2, 0},     /* 0x0911 */
    {0, 0, IRE_INDEP_VOWEL, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_2, 0},     /* 0x0912 */
    {0, 0, IRE_INDEP_VOWEL, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_2, 0},     /* 0x0913 */
    {0, 0, IRE_INDEP_VOWEL, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_2, 0},     /* 0x0914 */

    /* CONSONANTS */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_3, 0},    /* 0x0915 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_3, 0},    /* 0x0916 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_3, 0},    /* 0x0917 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x0918 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x0919 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x091A */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x091B */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_3, 0},    /* 0x091C */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x091D */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x091E */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x091F */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x0920 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_3, 0},    /* 0x0921 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_3, 0},    /* 0x0922 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x0923 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x0924 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x0925 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x0926 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x0927 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_3, 0},    /* 0x0928 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x0929 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x092A */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_3, 0},    /* 0x092B */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x092C */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x092D */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x092E */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_3, 0},    /* 0x092F */

    {IRE_RA_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_3, 0},        /* 0x0930 */

    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x0931 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x0932 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_3, 0},    /* 0x0933 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x0934 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x0935 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x0936 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x0937 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x0938 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                  /* 0x0939 */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0x093A */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0x093B */

    /* SIGNS */
    {0, 0, 0, IRE_NORMAL_SIGN, 0, 0, IRE_BELOW_BASE_POSITION_IGNORE_CORRECTION_ATTRIBUTE, 0, IRE_CHAR_TYPE_3, 0},       /* 0x093C */
    {0, 0, 0, IRE_NORMAL_SIGN, 0, 0, IRE_ABOVE_BASE_POSITION, 0, 0, 0},                     /* 0x093D */

    /* DEPENDENT VOWELS */
    {0, IRE_POST_BASE_DEP_VOWEL, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_1, 0},         /* 0x093E */
    {0, IRE_PRE_BASE_DEP_VOWEL, 0, 0, 0, 0, IRE_PREBASE_POSITION, 0, IRE_CHAR_TYPE_2, 0},           /* 0x093F */
    {0, IRE_POST_BASE_DEP_VOWEL, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_2, 0},         /* 0x0940 */
    {0, IRE_POST_BASE_DEP_VOWEL, 0, 0, 0, 0, IRE_BELOW_BASE_POSITION_IGNORE_CORRECTION_ATTRIBUTE, 0, IRE_CHAR_TYPE_1, 0},       /* 0x0941 */
    {0, IRE_POST_BASE_DEP_VOWEL, 0, 0, 0, 0, IRE_BELOW_BASE_POSITION_IGNORE_CORRECTION_ATTRIBUTE, 0, IRE_CHAR_TYPE_1, 0},       /* 0x0942 */
    {0, IRE_POST_BASE_DEP_VOWEL, 0, 0, 0, 0, IRE_BELOW_BASE_POSITION_IGNORE_CORRECTION_ATTRIBUTE, 0, IRE_CHAR_TYPE_1, 0},       /* 0x0943 */
    {0, IRE_POST_BASE_DEP_VOWEL, 0, 0, 0, 0, IRE_BELOW_BASE_POSITION_IGNORE_CORRECTION_ATTRIBUTE, 0, IRE_CHAR_TYPE_1, 0},       /* 0x0944 */
    {0, IRE_POST_BASE_DEP_VOWEL, 0, 0, 0, 0, IRE_ABOVE_BASE_POSITION, 0, IRE_CHAR_TYPE_2, 0},       /* 0x0945 */
    {0, IRE_POST_BASE_DEP_VOWEL, 0, 0, 0, 0, IRE_ABOVE_BASE_POSITION, 0, IRE_CHAR_TYPE_2, 0},       /* 0x0946 */
    {0, IRE_POST_BASE_DEP_VOWEL, 0, 0, 0, 0, IRE_ABOVE_BASE_POSITION, 0, IRE_CHAR_TYPE_2, 0},       /* 0x0947 */
    {0, IRE_POST_BASE_DEP_VOWEL, 0, 0, 0, 0, IRE_ABOVE_BASE_POSITION, 0, IRE_CHAR_TYPE_2, 0},       /* 0x0948 */
    {0, IRE_POST_BASE_DEP_VOWEL, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_2, 0},         /* 0x0949 */
    {0, IRE_POST_BASE_DEP_VOWEL, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},                       /* 0x094A */
    {0, IRE_POST_BASE_DEP_VOWEL, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_2, 0},         /* 0x094B */
    {0, IRE_POST_BASE_DEP_VOWEL, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, IRE_CHAR_TYPE_2, 0},         /* 0x094C */

    /* SIGNS */
    {0, 0, 0, IRE_HALANT_SIGN, 0, 0, IRE_BELOW_BASE_POSITION_IGNORE_CORRECTION_ATTRIBUTE, 0, 0, 0},       /* 0x094D */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0x094E */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0x094F */
    {0, 0, 0, 0, 0, IRE_NUMERIC_DIGIT, 0, 0,  0, 0},                          /* 0x0950 */
    {0, 0, 0, IRE_NORMAL_SIGN, 0, 0, IRE_ABOVE_BASE_POSITION, 0, 0, 0},       /* 0x0951 */
    {0, 0, 0, IRE_NORMAL_SIGN, 0, 0, IRE_BELOW_BASE_POSITION_IGNORE_CORRECTION_ATTRIBUTE, 0, 0, 0},       /* 0x0952 */
    {0, 0, 0, IRE_NORMAL_SIGN, 0, 0, IRE_ABOVE_BASE_POSITION, 0, 0, 0},       /* 0x0953 */
    {0, 0, 0, IRE_NORMAL_SIGN, 0, 0, IRE_ABOVE_BASE_POSITION, 0, 0, 0},       /* 0x0954 */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0x0955 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0x0956 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0x0957 */

    /* ADDITIONAL CONSONANT */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},    /* 0x0958 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},    /* 0x0959 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},    /* 0x095A */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},    /* 0x095B */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},    /* 0x095C */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},    /* 0x095D */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},    /* 0x095E */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},    /* 0x095F */

    /* GENERIC ADDITIONS */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},    /* 0x0960 */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},    /* 0x0961 */
    {0, IRE_POST_BASE_DEP_VOWEL, 0, 0, 0, 0, IRE_ABOVE_BASE_POSITION, 0, IRE_CHAR_TYPE_1, 0},       /* 0x0962 */
    {0, IRE_POST_BASE_DEP_VOWEL, 0, 0, 0, 0, IRE_ABOVE_BASE_POSITION, 0, IRE_CHAR_TYPE_1, 0},       /* 0x0963 */
    {0, 0, 0, 0, 0, IRE_NUMERIC_DIGIT, 0, 0,  0, 0},   /* 0x0964 */
    {0, 0, 0, 0, 0, IRE_NUMERIC_DIGIT, 0, 0,  0, 0},   /* 0x0965 */
    
    /* DIGITS */
    {0, 0, 0, 0, 0, IRE_NUMERIC_DIGIT, 0, 0, 0, 0},   /* 0x0966 */
    {0, 0, 0, 0, 0, IRE_NUMERIC_DIGIT, 0, 0, 0, 0},   /* 0x0967 */
    {0, 0, 0, 0, 0, IRE_NUMERIC_DIGIT, 0, 0, 0, 0},   /* 0x0968 */
    {0, 0, 0, 0, 0, IRE_NUMERIC_DIGIT, 0, 0, 0, 0},   /* 0x0969 */
    {0, 0, 0, 0, 0, IRE_NUMERIC_DIGIT, 0, 0, 0, 0},   /* 0x096A */
    {0, 0, 0, 0, 0, IRE_NUMERIC_DIGIT, 0, 0, 0, 0},   /* 0x096B */
    {0, 0, 0, 0, 0, IRE_NUMERIC_DIGIT, 0, 0, 0, 0},   /* 0x096C */
    {0, 0, 0, 0, 0, IRE_NUMERIC_DIGIT, 0, 0, 0, 0},   /* 0x096D */
    {0, 0, 0, 0, 0, IRE_NUMERIC_DIGIT, 0, 0, 0, 0},   /* 0x096E */
    {0, 0, 0, 0, 0, IRE_NUMERIC_DIGIT, 0, 0, 0, 0},   /* 0x096F */

    /* DEVANAGARI SPECIFIC ADDITOIN */
    {0, 0, 0, 0, 0, IRE_NUMERIC_DIGIT, 0, 0,0, 0},   /* 0x0970 */
    
    /* SIGN */
    {0, 0, 0, IRE_NORMAL_SIGN, 0, 0, IRE_ABOVE_BASE_POSITION, 0, 0, 0},		/* 0x0971 */
    
    /* INDEPENDENT VOWEL */
    {0, 0, IRE_INDEP_VOWEL, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     	/* 0x0972 */
    
    /* EMPTY */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0x0973 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0x0974 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0x0975 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0x0976 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0x0977 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0x0978 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0x0979 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0x097A */
        
    /* SINDHI IMPLOSIVES */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},    /* 0x097B */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},    /* 0x097C */
    
    /* SYNBOL */
    {0, 0, 0, 0, 0, IRE_NUMERIC_DIGIT, 0, 0,  0, 0},   /* 0x097D */
    
    /* SINDHI IMPLOSIVES */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0},    /* 0x097E */
    {IRE_NORMAL_CONSONANT, 0, 0, 0, 0, 0, IRE_POSTBASE_POSITION, 0, 0, 0}     /* 0x097F */
    
};

const ire_character_info_table_struct hindi_ligature_info_table[] = 
{
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE900 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE901 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE902 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE903 */
#if defined(__MMI_LANG_MARATHI__)
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE904 */
#else 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE904 */
#endif 
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE905 */

#if defined(__MMI_LANG_MARATHI__)
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE906 */
#else 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE906 */
#endif 
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE907 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE908 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE909 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE90A */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE90B */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE90C */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE90D */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE90E */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE90F */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE910 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE911 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE912 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE913 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE914 */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE915 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE916 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE917 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE918 */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE919 */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE91A */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE91B */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE91C */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE91D */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE91E */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE91F */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE920 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE921 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE922 */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE923 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE924 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE925 */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE926 */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE927 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE928 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE929 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE92A */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE92B */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE92C */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE92D */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE92E */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE92F */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE930 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE931 */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE932 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE933 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE934 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE935 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE936 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE937 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE938 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE939 */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE93A */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE93B */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE93C */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE93D */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE93E */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE93F */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE940 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE941 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE942 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE943 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE944 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE945 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE946 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE947 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE948 */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE949 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE94A */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE94B */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE94C */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE94D */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE94E */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE94F */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE950 */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE951 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE952 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE953 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE954 */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE955 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE956 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE957 */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE958 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE959 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE95A */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE95B */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE95C */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE95D */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE95E */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE95F */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE960 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE961 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE962 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE963 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE964 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE965 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE966 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE967 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE968 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE969 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE96A */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE96B */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE96C */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE96D */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE96E */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE96F */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE970 */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE971 */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE972 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE973 */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE974 */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE975 */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE976 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE977 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE978 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE979 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE97A */

    {0, 0, 0, IRE_NORMAL_SIGN, 0, 0, IRE_ABOVE_BASE_POSITION, 0, 0, 0},       /* 0xE97B */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE97C */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE97D */

    {0, 0, 0, IRE_NORMAL_SIGN, 0, 0, IRE_ABOVE_BASE_POSITION, 0, 0, 0},       /* 0xE97E */
    {0, 0, 0, IRE_NORMAL_SIGN, 0, 0, IRE_ABOVE_BASE_POSITION, 0, 0, 0},       /* 0xE97F */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE980 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE981 */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE982 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE983 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE984 */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE985 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE986 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE987 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE988 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE989 */

    {0, 0, 0, IRE_NORMAL_SIGN, 0, 0, IRE_ABOVE_BASE_POSITION, 0, 0, 0},       /* 0xE98A */
    {0, 0, 0, IRE_NORMAL_SIGN, 0, 0, IRE_ABOVE_BASE_POSITION, 0, 0, 0},       /* 0xE98B */
    {0, 0, 0, IRE_NORMAL_SIGN, 0, 0, IRE_ABOVE_BASE_POSITION, 0, 0, 0},       /* 0xE98C */
    {0, 0, 0, IRE_NORMAL_SIGN, 0, 0, IRE_ABOVE_BASE_POSITION, 0, 0, 0},       /* 0xE98D */
    {0, 0, 0, IRE_NORMAL_SIGN, 0, 0, IRE_ABOVE_BASE_POSITION, 0, 0, 0},       /* 0xE98E */
    {0, 0, 0, IRE_NORMAL_SIGN, 0, 0, IRE_ABOVE_BASE_POSITION, 0, 0, 0},       /* 0xE98F */

    {0, 0, 0, IRE_NORMAL_SIGN, 0, 0, IRE_ABOVE_BASE_POSITION, 0, 0, 0},       /* 0xE990 */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE991 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE992 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE993 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE994 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE995 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE996 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE997 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE998 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE999 */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE99A */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE99B */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE99C */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE99D */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE99E */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE99F */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9A0 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9A1 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9A2 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9A3 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9A4 */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE9A5 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE9A6 */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9A7 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9A8 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9A9 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9AA */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9AB */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9AC */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9AD */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9AE */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9AF */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9B0 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9B1 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9B2 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9B3 */

#if defined(__MMI_LANG_MARATHI__)
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9B4 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9B5 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9B6 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9B7 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9B8 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9B9 */

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE9BA */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   /* 0xE9BB */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9BC */

    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9BD */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9BE */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9BF */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, IRE_POSTBASE_POSITION, 0, 0, 0},     /* 0xE9C0 */
    {0, 0, 0, 0, IRE_NORMAL_LIGATURE, 0, 0, 0, 0, 0}  /* 0xE9C1 */
#endif /* defined(__MMI_LANG_MARATHI__) */ 
};

 /*    CONSONANT'S TABLES   */

const ire_glyph_info_table_struct KA_Tbl[] = 
{

    /* ka + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_KA, 0x0}, {HINDI_C_KA, 0x0}},
    /* Ka + nukta -> QA */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_KA, HINDI_S_NUKTA, 0x0}, {HINDI_AC_QA, 0x0}},
    /* Ka + Halant/Virama -> Half form of Ka */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_KA, HINDI_S_HALANT, 0x0}, {HINDI_H_KA, 0x0}},
    /* Ka + Nukta +Halant -> half form of Ka with Nukta */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_KA, HINDI_S_NUKTA, HINDI_S_HALANT, 0x0}, {HINDI_L_KA_N, 0x0}},
    /* Ka + Halant + KA -> ligature form of KKA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_KA, HINDI_S_HALANT, HINDI_C_KA, 0x0}, {HINDI_L_KKA, 0x0}},
    /* Ka + Halant + Ta -> Kta */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_KA, HINDI_S_HALANT, HINDI_C_TA, 0x0}, {HINDI_L_KTA, 0x0}},
    /* Ka + Halant + Ra -> Kra */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_KA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_KRA, 0x0}},
    /* Ka + Halant + Sha -> Ksha */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_KA, HINDI_S_HALANT, HINDI_C_SSA, 0x0}, {HINDI_L_KSHA, 0x0}},
    /* Ka + Halant + Sha + Halant -> Half Ksha */
    {0, 4, 1, MMI_ALL_LANG, {HINDI_C_KA, HINDI_S_HALANT, HINDI_C_SSA, HINDI_S_HALANT, 0x0}, {HINDI_H_KSHA, 0x0}},   
    /* Ka + Halant + Ra +Halant -> Kra +Halant */
    {0, 4, 1, MMI_ALL_LANG, {HINDI_C_KA, HINDI_S_NUKTA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_KRA_N, 0x0}},
    /* Ka + Halant + Ra + Halant -> Half Form of Ka + Ra + Halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_C_KA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_H_KA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
	{1, 6, 4, MMI_ALL_LANG, {HINDI_C_KA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_KA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}     	
};

const ire_glyph_info_table_struct KHA_Tbl[] = 
{
    /* kha + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_KHA, 0x0}, {HINDI_C_KHA, 0x0}},
    /* kha + nukta -> khha */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_KHA, HINDI_S_NUKTA, 0x0}, {HINDI_AC_KHHA, 0x0}},
    /* Kha + halant  -> half form of kha */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_KHA, HINDI_S_HALANT, 0x0}, {HINDI_H_KHA, 0x0}},
    /* kha + nukta +halant -> half form of kha with nukta */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_KHA, HINDI_S_NUKTA, HINDI_S_HALANT, 0x0}, {HINDI_L_KHA_N, 0x0}},
    /* kha + halant + ra -> Kha + subscript form of RA OR KHRA as a whole */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_KHA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_KHRA, 0x0}},       
    /* kha + Nukta + halant + ra -> kha + RA subscript +halant */
    {0, 4, 1, MMI_ALL_LANG, {HINDI_C_KHA, HINDI_S_NUKTA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_KHRA_N, 0x0}},      
    /* kha + halant + ra + halant -> half kha + RA subscript +halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_C_KHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_H_KHA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_C_KHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_KHA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}} 
}; 

const ire_glyph_info_table_struct GA_Tbl[] = 
{
    /* ga + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_GA, 0x0}, {HINDI_C_GA, 0x0}},
    /* ga + nukta -> nukta form of ga */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_GA, HINDI_S_NUKTA, 0x0}, {HINDI_AC_GHHA, 0x0}},
    /* ga + halant -> half form of ga */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_GA, HINDI_S_HALANT, 0x0}, {HINDI_H_GA, 0x0}},
    /* ga + nukta + halant -> half ga with nukta */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_GA, HINDI_S_NUKTA, HINDI_S_HALANT, 0x0}, {HINDI_L_GA_N, 0x0}},
    /* ga + halant + ra -> ga + subscript form of RA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_GA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_GRA, 0x0}}, /* Q */
    #if defined(__MMI_LANG_MARATHI__)
    {0, 3, 1, MMI_MARATHI_LANG, {HINDI_C_GA, HINDI_S_HALANT, HINDI_C_NA, 0x0}, {HINDI_L_GA_NA, 0x0}},
		 #endif 
    /* ga + halant + ra + halant -> ga + subscript form of RA + halant */
    {0, 4, 1, MMI_ALL_LANG, {HINDI_C_GA, HINDI_S_NUKTA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_GRA_N, 0x0}},        
    /* ga + halant + ra + halant -> halfga + RA + halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_C_GA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_H_GA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_C_GA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_GA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}} 
};

const ire_glyph_info_table_struct GHA_Tbl[] = 
{
    /* gha + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_GHA, 0x0}, {HINDI_C_GHA, 0x0}},
    /* gha + halant -> half form of gha */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_GHA, HINDI_S_HALANT, 0x0}, {HINDI_H_GHA, 0x0}},
    /* gha + halant + RA  -> gha + subscript form of RA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_GHA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_GHRA, 0x0}},       
    /* gha + halant + RA + halant -> gha + RA subscript + halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_C_GHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0}, 
     {HINDI_H_GHA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_C_GHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_GHA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}} 
};

const ire_glyph_info_table_struct NGA_Tbl[] = 
{
    /* nga + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_NGA, 0x0}, {HINDI_C_NGA, 0x0}},
    /* nga + halant + RA -> nga + subscript RA(special representation) */
    {0, 3, 2, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_C_NGA, HINDI_SUB_RA, 0x0}},
    /* nga + halant + ka -> ligature HINDI_L_DAKA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KA, 0x0}, {HINDI_L_DAKA, 0x0}},
    /* nga + halant + ka -> ligature HINDI_L_DAKHA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KHA, 0x0}, {HINDI_L_DAKHA, 0x0}},
    /* nga + halant + ka -> ligature L_DAGA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GA, 0x0}, {HINDI_L_DAGA, 0x0}},
    /* nga + halant + ka -> ligature HINDI_L_DAGHA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GHA, 0x0}, {HINDI_L_DAGHA, 0x0}},
    /* nga + halant + ka ->  */
    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KA, HINDI_S_HALANT, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KA, HINDI_S_HALANT, 0x0}},
    /* nga + halant + ka ->  */
    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KHA, HINDI_S_HALANT, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KHA, HINDI_S_HALANT, 0x0}},
    /* nga + halant + ka ->  */
    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GA, HINDI_S_HALANT, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GA, HINDI_S_HALANT, 0x0}},
    /* nga + halant + ka -> ligature HINDI_L_DAGHA */
    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GHA, HINDI_S_HALANT, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GHA, HINDI_S_HALANT, 0x0}},

    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KA, HINDI_DV_U, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KA, HINDI_DV_U, 0x0}},
    /* nga + halant + ka ->  */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KHA, HINDI_DV_U, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KHA, HINDI_DV_U, 0x0}},
    /* nga + halant + ka ->  */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GA, HINDI_DV_U, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GA, HINDI_DV_U, 0x0}},
    /* nga + halant + ka -> ligature HINDI_L_DAGHA */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GHA, HINDI_DV_U, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GHA, HINDI_DV_U, 0x0}},

    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KA, HINDI_DV_UU, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KA, HINDI_DV_UU, 0x0}},
    /* nga + halant + ka ->  */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KHA, HINDI_DV_UU, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KHA, HINDI_DV_UU, 0x0}},
    /* nga + halant + ka ->  */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GA, HINDI_DV_UU, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GA, HINDI_DV_UU, 0x0}},
    /* nga + halant + ka -> ligature HINDI_L_DAGHA */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GHA, HINDI_DV_UU, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GHA, HINDI_DV_UU, 0x0}},

    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KA, HINDI_DV_R, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KA, HINDI_DV_R, 0x0}},
    /* nga + halant + ka ->  */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KHA, HINDI_DV_R, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KHA, HINDI_DV_R, 0x0}},
    /* nga + halant + ka ->  */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GA, HINDI_DV_R, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GA, HINDI_DV_R, 0x0}},
    /* nga + halant + ka -> ligature HINDI_L_DAGHA */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GHA, HINDI_DV_R, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GHA, HINDI_DV_R, 0x0}},

    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {0, 4, 3, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_U, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_L_RU, 0x0}},
    {0, 4, 3, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_UU, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_L_RUU, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_R, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_R, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KHA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_KHA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GHA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_GHA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_NGA, HINDI_S_HALANT, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct CHA_Tbl[] = 
{
    /* cha + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_CHA, 0x0}, {HINDI_C_CHA, 0x0}},
    /* cha  + halant -> half form of cha   */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_CHA, HINDI_S_HALANT, 0x0}, {HINDI_H_CHA, 0x0}},
    /* cha + halant ra -> chra  OR cha + ra subscript */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_CHA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_CHRA, 0x0}},       
    /* cha  + halant + ra + halant  -> chra + halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_C_CHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_H_CHA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_C_CHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_CHA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}} 
};

const ire_glyph_info_table_struct CHHA_Tbl[] = 
{
    /* chha + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_CHHA, 0x0}, {HINDI_C_CHHA, 0x0}},
    /* chha + halant -> chha + halant */
    {0, 2, 2, MMI_ALL_LANG, {HINDI_C_CHHA, HINDI_S_HALANT, 0x0}, {HINDI_C_CHHA, HINDI_S_HALANT, 0x0}},
    /* chha + halant + ra -> chha + RA subscriprt(special representation) */
    {0, 3, 2, MMI_ALL_LANG, {HINDI_C_CHHA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_C_CHHA, HINDI_SUB_RA, 0x0}},
    #if defined(__MMI_LANG_MARATHI__)
    {0, 3, 1, MMI_MARATHI_LANG, {HINDI_C_CHHA, HINDI_S_HALANT, HINDI_C_YA, 0x0}, {HINDI_L_CHHA_YA, 0x0}},
		#endif 
    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_CHHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_C_CHHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0}},

    {0, 4, 3, MMI_ALL_LANG, {HINDI_C_CHHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_U, 0x0},
     {HINDI_C_CHHA, HINDI_S_HALANT, HINDI_L_RU, 0x0}},
    {0, 4, 3, MMI_ALL_LANG, {HINDI_C_CHHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_UU, 0x0},
     {HINDI_C_CHHA, HINDI_S_HALANT, HINDI_L_RUU, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_CHHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_R, 0x0},
     {HINDI_C_CHHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_R, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_CHHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_CHHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct JA_Tbl[] = 
{
    /* ja + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_JA, 0x0}, {HINDI_C_JA, 0x0}},
    /* ja + halant -> half form of ja  */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_JA, HINDI_S_HALANT, 0x0}, {HINDI_H_JA, 0x0}},
    /* ja + nukta -> za */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_JA, HINDI_S_NUKTA, 0x0}, {HINDI_AC_ZA, 0x0}},
    /* ja + nukta + halant -> half za */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_JA, HINDI_S_NUKTA, HINDI_S_HALANT, 0x0}, {HINDI_H_JA_N, 0x0}},
    /* ja + halant + ra -> jra  */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_JA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_JRA, 0x0}},
    /* ja + halant + nya -> GYA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_JA, HINDI_S_HALANT, HINDI_C_NYA, 0x0}, {HINDI_L_GYA, 0x0}},
    /* ja + halant + ra + halant -> jra + halant or ja + subscript RA + halant */
    {0, 4, 1, MMI_ALL_LANG, {HINDI_C_JA, HINDI_S_NUKTA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_JRA_N, 0x0}},

    /* ja + halant + nya + halant -> half form of GYA */
    /*{1, 4, 2, MMI_ALL_LANG, {HINDI_C_JA, HINDI_S_HALANT, HINDI_C_NYA, HINDI_S_HALANT, 0x0},
     {HINDI_L_GYA, HINDI_S_HALANT, 0x0}},*/
    /* ja + halant + ra + halant -> half ja + RA + halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_C_JA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_H_JA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_C_JA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_JA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct JHA_Tbl[] = 
{
    /* jha + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_JHA, 0x0}, {HINDI_C_JHA, 0x0}},
    /* jha + halant -> half jha  */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_JHA, HINDI_S_HALANT, 0x0}, {HINDI_H_JHA, 0x0}},
    /* jha + halant + ra-> jhra OR jha + subscript RA form */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_JHA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_JHRA, 0x0}},
    /*  jha + halant+ ra + halant  -> half jhra */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_C_JHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_H_JHA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_C_JHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_JHA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct NYA_Tbl[] = 
{
    /* nya + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_NYA, 0x0}, {HINDI_C_NYA, 0x0}},
    /* nya + halant  -> half form of nya */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_NYA, HINDI_S_HALANT, 0x0}, {HINDI_H_NYA, 0x0}},
};

const ire_glyph_info_table_struct TTA_Tbl[] = 
{
    /* TTa + space -> TTa */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_TTA, 0x0}, {HINDI_C_TTA, 0x0}},
    /* TTA + Halant -> TTA + Halant */
    {0, 2, 2, MMI_ALL_LANG, {HINDI_C_TTA, HINDI_S_HALANT, 0x0}, {HINDI_C_TTA, HINDI_S_HALANT, 0x0}},
    /* TTA + Halant + Ra -> TA + subscript RA */
    {0, 3, 2, MMI_ALL_LANG, {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_C_TTA, HINDI_SUB_RA, 0x0}},
    /* TTa + halant + TTa -> Ligature form of TTA below TTA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTA, 0x0}, {HINDI_L_TTTTA, 0x0}},
    /* TTa + halant + TTHA -> Ligature form of TTHA below TTA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTHA, 0x0}, {HINDI_L_TTHA, 0x0}},
    #if defined(__MMI_LANG_MARATHI__)
    {0, 3, 1, MMI_MARATHI_LANG, {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_YA, 0x0}, {HINDI_L_TTA_YA, 0x0}},
		#endif 
    /* TTa + halant + TTa -> Ligature form of TTA below TTA */
    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTA, HINDI_S_HALANT, 0x0},
     {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTA, HINDI_S_HALANT, 0x0}},

    /* TTa + halant + TTa -> Ligature form of TTA below TTA */
    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTHA, HINDI_S_HALANT, 0x0},
     {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTHA, HINDI_S_HALANT, 0x0}},

    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTA, HINDI_DV_U, 0x0},
     {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTA, HINDI_DV_U, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTHA, HINDI_DV_U, 0x0},
     {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTHA, HINDI_DV_U, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTA, HINDI_DV_UU, 0x0},
     {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTA, HINDI_DV_UU, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTHA, HINDI_DV_UU, 0x0},
     {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTHA, HINDI_DV_UU, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTA, HINDI_DV_R, 0x0},
     {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTA, HINDI_DV_R, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTHA, HINDI_DV_R, 0x0},
     {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTHA, HINDI_DV_R, 0x0}},
    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {0, 4, 3, MMI_ALL_LANG, {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_U, 0x0},
     {HINDI_C_TTA, HINDI_S_HALANT, HINDI_L_RU, 0x0}},
    {0, 4, 3, MMI_ALL_LANG, {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_UU, 0x0},
     {HINDI_C_TTA, HINDI_S_HALANT, HINDI_L_RUU, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_R, 0x0},
     {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_R, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTHA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_TTHA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct TTHA_Tbl[] = 
{
    /* Tha -> Tha + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_TTHA, 0x0}, {HINDI_C_TTHA, 0x0}},
    /* TTHA + halant -> TTHA + Halant */
    {0, 2, 2, MMI_ALL_LANG, {HINDI_C_TTHA, HINDI_S_HALANT, 0x0}, {HINDI_C_TTHA, HINDI_S_HALANT, 0x0}},

    /* TTha + halant + Ra -> TTHA + subscript form of RA(special type) */
    {0, 3, 2, MMI_ALL_LANG, {HINDI_C_TTHA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_C_TTHA, HINDI_SUB_RA, 0x0}},
    /* TTha + halant + TTha -> TTHA + subscript form of RA(special type) */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_TTHA, HINDI_S_HALANT, HINDI_C_TTHA, 0x0}, {HINDI_L_TTTHA, 0x0}},
    #if defined(__MMI_LANG_MARATHI__)
    {0, 3, 1, MMI_MARATHI_LANG, {HINDI_C_TTHA, HINDI_S_HALANT, HINDI_C_YA, 0x0}, {HINDI_L_TTHA_YA, 0x0}},
 		#endif 
    /* TTha + halant + TTha -> TTHA + subscript form of RA(special type) */
    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_TTHA, HINDI_S_HALANT, HINDI_C_TTHA, HINDI_S_HALANT, 0x0},
     {HINDI_C_TTHA, HINDI_S_HALANT, HINDI_C_TTHA, HINDI_S_HALANT, 0x0}},

    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_TTHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_C_TTHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {0, 4, 3, MMI_ALL_LANG, {HINDI_C_TTHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_U, 0x0},
     {HINDI_C_TTHA, HINDI_S_HALANT, HINDI_L_RU, 0x0}},
    {0, 4, 3, MMI_ALL_LANG, {HINDI_C_TTHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_UU, 0x0},
     {HINDI_C_TTHA, HINDI_S_HALANT, HINDI_L_RUU, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_TTHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_R, 0x0},
     {HINDI_C_TTHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_R, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_TTHA, HINDI_S_HALANT, HINDI_C_TTHA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_TTHA, HINDI_S_HALANT, HINDI_C_TTHA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_TTHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_TTHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct DDA_Tbl[] = 
{
    /* dda + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_DDA, 0x0}, {HINDI_C_DDA, 0x0}},
    /* dda + halant -> dda+ halant */
    {0, 2, 2, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, 0x0}, {HINDI_C_DDA, HINDI_S_HALANT, 0x0}},
    /* Dda + nukta -> dda with nukta */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_NUKTA, 0x0}, {HINDI_AC_DDDHA, 0x0}},
    /* Dda + nukta+ halant  -> dda with nukta + halant */
    {0, 3, 2, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_NUKTA, HINDI_S_HALANT, 0x0}, {HINDI_AC_DDDHA, HINDI_S_HALANT, 0x0}},
    /* dda + halant + ra -> dda + subscript ra(special represenatio */
    {0, 3, 2, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_C_DDA, HINDI_SUB_RA, 0x0}},
    /* dda + halant + ga -> ligature L_DDAGA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_GA, 0x0}, {HINDI_L_DDAGA, 0x0}},
    /* dda + halant + ra -> ligature L_DADA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDA, 0x0}, {HINDI_L_DADA, 0x0}},
    /* dda + halant + ga -> ligature L_DDAGA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDHA, 0x0}, {HINDI_L_DADDHA, 0x0}},
    #if defined(__MMI_LANG_MARATHI__)
    {0, 3, 1, MMI_MARATHI_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_YA, 0x0}, {HINDI_L_DDA_YA, 0x0}},
 		#endif 

    /* dda + halant + ga -> ligature L_DDAGA */
    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDHA, HINDI_S_HALANT, 0x0},
     {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDHA, HINDI_S_HALANT, 0x0}},
    /* dda + halant + ga -> ligature L_DDAGA */
    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_GA, HINDI_S_HALANT, 0x0},
     {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_GA, HINDI_S_HALANT, 0x0}},
    /* dda + halant + ga -> ligature L_DDAGA */
    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDA, HINDI_S_HALANT, 0x0},
     {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDA, HINDI_S_HALANT, 0x0}},

    /* dda + halant + ga -> ligature L_DDAGA */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDHA, HINDI_DV_U, 0x0},
     {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDHA, HINDI_DV_U, 0x0}},
    /* dda + halant + ga -> ligature L_DDAGA */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_GA, HINDI_DV_U, 0x0},
     {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_GA, HINDI_DV_U, 0x0}},
    /* dda + halant + ga -> ligature L_DDAGA */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDA, HINDI_DV_U, 0x0},
     {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDA, HINDI_DV_U, 0x0}},

    /* dda + halant + ga -> ligature L_DDAGA */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDHA, HINDI_DV_UU, 0x0},
     {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDHA, HINDI_DV_UU, 0x0}},
    /* dda + halant + ga -> ligature L_DDAGA */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_GA, HINDI_DV_UU, 0x0},
     {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_GA, HINDI_DV_UU, 0x0}},
    /* dda + halant + ga -> ligature L_DDAGA */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDA, HINDI_DV_UU, 0x0},
     {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDA, HINDI_DV_UU, 0x0}},

    /* dda + halant + ga -> ligature L_DDAGA */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDHA, HINDI_DV_R, 0x0},
     {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDHA, HINDI_DV_R, 0x0}},
    /* dda + halant + ga -> ligature L_DDAGA */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_GA, HINDI_DV_R, 0x0},
     {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_GA, HINDI_DV_R, 0x0}},
    /* dda + halant + ga -> ligature L_DDAGA */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDA, HINDI_DV_R, 0x0},
     {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDA, HINDI_DV_R, 0x0}},

    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {0, 4, 3, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_U, 0x0},
     {HINDI_C_DDA, HINDI_S_HALANT, HINDI_L_RU, 0x0}},
    {0, 4, 3, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_UU, 0x0},
     {HINDI_C_DDA, HINDI_S_HALANT, HINDI_L_RUU, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_R, 0x0},
     {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_R, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDHA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDHA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_GA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_GA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_DDA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_DDA, HINDI_S_HALANT, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct DDHA_Tbl[] = 
{
    /* ddha + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_DDHA, 0x0}, {HINDI_C_DDHA, 0x0}},
    /* ddha + halant -> ddha + halant */
    {0, 2, 2, MMI_ALL_LANG, {HINDI_C_DDHA, HINDI_S_HALANT, 0x0}, {HINDI_C_DDHA, HINDI_S_HALANT, 0x0}},
    /* Ddha + nukta -> ddha with nukta form */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_DDHA, HINDI_S_NUKTA, 0x0}, {HINDI_AC_RHA, 0x0}},
    /* ddha + nukta + halant -> ddha with nukta form + halant */
    {0, 3, 2, MMI_ALL_LANG, {HINDI_C_DDHA, HINDI_S_NUKTA, HINDI_S_HALANT, 0x0}, {HINDI_AC_RHA, HINDI_S_HALANT, 0x0}},
    /* ddha + halant + ra -> ddha + subscript ra(special form) */
    {0, 3, 2, MMI_ALL_LANG, {HINDI_C_DDHA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_C_DDHA, HINDI_SUB_RA, 0x0}},
    #if defined(__MMI_LANG_MARATHI__)
    {0, 3, 1, MMI_MARATHI_LANG, {HINDI_C_DDHA, HINDI_S_HALANT, HINDI_C_YA, 0x0}, {HINDI_L_DDHA_YA, 0x0}},
 		#endif
    /* ddha + nukta + halant + ra -> ddha with nukta form + subscript ra(special form) */
    {0, 4, 2, MMI_ALL_LANG, {HINDI_C_DDHA, HINDI_S_NUKTA, HINDI_S_HALANT, HINDI_C_RA, 0x0},
     {HINDI_AC_RHA, HINDI_SUB_RA, 0x0}},

    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_DDHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_C_DDHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {0, 4, 3, MMI_ALL_LANG, {HINDI_C_DDHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_U, 0x0},
     {HINDI_C_DDHA, HINDI_S_HALANT, HINDI_L_RU, 0x0}},
    {0, 4, 3, MMI_ALL_LANG, {HINDI_C_DDHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_UU, 0x0},
     {HINDI_C_DDHA, HINDI_S_HALANT, HINDI_L_RUU, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DDHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_R, 0x0},
     {HINDI_C_DDHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_R, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_DDHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_DDHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct NNA_Tbl[] = 
{
    /* Nna+ space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_NNA, 0x0}, {HINDI_C_NNA, 0x0}},
    /* Nna + halant -> half Nna */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_NNA, HINDI_S_HALANT, 0x0}, {HINDI_H_ANNA, 0x0}},
    /* Nna + halant + ra -> half Nna + ra(nominal form) */
    {0, 3, 2, MMI_ALL_LANG, {HINDI_C_NNA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_H_ANNA, HINDI_C_RA, 0x0}}
};

const ire_glyph_info_table_struct TA_Tbl[] = 
{
    /* ta + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_TA, 0x0}, {HINDI_C_TA, 0x0}},
    /* ta + halant -> half form of ta */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_TA, HINDI_S_HALANT, 0x0}, {HINDI_H_TA, 0x0}},
    /* ta + halant + ra ->  tra(ligature) */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_TA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_TRA, 0x0}},
    /* ta + halant + ra + halant -> half tra  */
    /* {0,4,1,MMI_ALL_LANG,           {HINDI_C_TA,HINDI_S_HALANT,HINDI_C_RA,HINDI_S_HALANT,0x0},  {H_TRA,0x0} },//vj 01/12/04 */
    /* ta + halant + ta -> TTA(ligature form) */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_TA, HINDI_S_HALANT, HINDI_C_TA, 0x0}, {HINDI_L_TTA, 0x0}},
    /* ta + halant + ta + halant -> TTA(half form) */
    {0, 4, 1, MMI_ALL_LANG, {HINDI_C_TA, HINDI_S_HALANT, HINDI_C_TA, HINDI_S_HALANT,0x0}, {HINDI_H_TTA, 0x0}},
    /* ta + halant + ra + halant -> half tra  */
    {0, 4, 1, MMI_ALL_LANG, {HINDI_C_TA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT,0x0},  {HINDI_H_TRA,0x0} }
};

const ire_glyph_info_table_struct THA_Tbl[] = 
{
    /* tha + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_THA, 0x0}, {HINDI_C_THA, 0x0}},
    /* tha + halant -> half form of tha */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_THA, HINDI_S_HALANT, 0x0}, {HINDI_H_THA, 0x0}},
    /* tha + halant + ra ->  tha + subscript form of RA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_THA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_THRA, 0x0}},       
    /* tha + halant + ra + halant -> half tha + RA + halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_C_THA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_H_THA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_C_THA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_THA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct DA_Tbl[] = 
{
    /* da + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_DA, 0x0}, {HINDI_C_DA, 0x0}},
    /* da + halant -> da + halant */
    {0, 2, 2, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, 0x0}, {HINDI_C_DA, HINDI_S_HALANT, 0x0}},
    /* da + halant + ra -> da + subscript ra  */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_DRA, 0x0}}, 
    /* da + halant + ya  ->  ligature dya */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DA, 0x0}, {HINDI_L_DYA, 0x0}},
    /* da +halant + da ->   ligature ddya */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_YA, 0x0}, {HINDI_L_DDYA, 0x0}},
    /* da + halant + va   ->  ligature DVA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_VA, 0x0}, {HINDI_L_DVA, 0x0}},
    /* da + halant + dha   ->  ligature HINDI_L_DADHA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DHA, 0x0}, {HINDI_L_DADHA, 0x0}},
    /* da + halant + bha   ->  ligature HINDI_L_DABHA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_BHA, 0x0}, {HINDI_L_DABHA, 0x0}},
	{0, 3, 1, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_MA, 0x0}, {HINDI_L_DAMA, 0x0}},
    #if defined(__MMI_LANG_MARATHI__)
    {0, 3, 1, MMI_MARATHI_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_MA, 0x0}, {HINDI_L_DA_MA, 0x0}},
 	#endif 
    /* da + halant + ya  ->  ligature dya */
    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DA, HINDI_S_HALANT, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DA, HINDI_S_HALANT, 0x0}},
    /* da + halant + ya  ->  ligature dya */
    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_YA, HINDI_S_HALANT, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_YA, HINDI_S_HALANT, 0x0}},
    /* da + halant + va +halant  ->   */
    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_VA, HINDI_S_HALANT, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_VA, HINDI_S_HALANT, 0x0}},
    /* da + halant + va +halant  ->   */
    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DHA, HINDI_S_HALANT, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DHA, HINDI_S_HALANT, 0x0}},
    /* da + halant + va +halant  ->   */
    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_BHA, HINDI_S_HALANT, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_BHA, HINDI_S_HALANT, 0x0}},
    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_MA, HINDI_S_HALANT, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_MA, HINDI_S_HALANT, 0x0}},
	{0, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DA, HINDI_DV_U, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DA, HINDI_DV_U, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_YA, HINDI_DV_U, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_YA, HINDI_DV_U, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_VA, HINDI_DV_U, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_VA, HINDI_DV_U, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DHA, HINDI_DV_U, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DHA, HINDI_DV_U, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_BHA, HINDI_DV_U, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_BHA, HINDI_DV_U, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_MA, HINDI_DV_U, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_MA, HINDI_DV_U, 0x0}},
	{0, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DA, HINDI_DV_UU, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DA, HINDI_DV_UU, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_YA, HINDI_DV_UU, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_YA, HINDI_DV_UU, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_VA, HINDI_DV_UU, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_VA, HINDI_DV_UU, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DHA, HINDI_DV_UU, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DHA, HINDI_DV_UU, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_BHA, HINDI_DV_UU, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_BHA, HINDI_DV_UU, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_MA, HINDI_DV_UU, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_MA, HINDI_DV_UU, 0x0}},
	{0, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DA, HINDI_DV_R, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DA, HINDI_DV_R, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_YA, HINDI_DV_R, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_YA, HINDI_DV_R, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_VA, HINDI_DV_R, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_VA, HINDI_DV_R, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DHA, HINDI_DV_R, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DHA, HINDI_DV_R, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_BHA, HINDI_DV_R, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_BHA, HINDI_DV_R, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_MA, HINDI_DV_R, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_MA, HINDI_DV_R, 0x0}},
	{1, 4, 4, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_YA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_YA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_VA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_VA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DHA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_DHA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_BHA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_BHA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_MA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_MA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_DA, HINDI_S_HALANT, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct DHA_Tbl[] = 
{
    /* Dha +space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_DHA, 0x0}, {HINDI_C_DHA, 0x0}},
    /* Dha + halant - > half form of Dha */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_DHA, HINDI_S_HALANT, 0x0}, {HINDI_H_DHA, 0x0}},
    /* Dha + halant + ra ->  Dhra OR dha + subscript ra */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_DHA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_DHRA, 0x0}},
    /* Dha + halant + ra + halant ->  half dha + Ra + halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_C_DHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_H_DHA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_C_DHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_DHA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct NA_Tbl[] = 
{
    /* na + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_NA, 0x0}, {HINDI_C_NA, 0x0}},
    /* na + Nukta  ->  naa */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_NA, HINDI_S_NUKTA, 0x0}, {HINDI_C_NNNA, 0x0}},
    /* na + halant  ->  half form of na */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_NA, HINDI_S_HALANT, 0x0}, {HINDI_H_NA, 0x0}},
    /* NA + Nukta +Halant -> half form of Na with Nukta */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_NA, HINDI_S_NUKTA, HINDI_S_HALANT, 0x0}, {HINDI_H_NNNA, 0x0}},
    /* na + halant + ra  -> na + subscript RA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_NA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_NRA, 0x0}}, 
    /* na + halant + na -> (ligature form) NNA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_NA, HINDI_S_HALANT, HINDI_C_NA, 0x0}, {HINDI_L_NNNA, 0x0}},
    /* na + halant + ra + halant -> half na + RA + halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_C_NA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_H_NA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},      
    /* na + halant + na + halant -> (ligature form) NNA + halant */
    /*{1, 4, 2, MMI_ALL_LANG, {HINDI_C_NA, HINDI_S_HALANT, HINDI_C_NA, HINDI_S_HALANT, 0x0},
     {HINDI_L_NNNA, HINDI_S_HALANT, 0x0}},*/
    {1, 6, 4, MMI_ALL_LANG, {HINDI_C_NA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_NA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct NNNA_Tbl[] = 
{
    /* nna + space */
    {0, 1, 2, MMI_ALL_LANG, {HINDI_C_NNNA, 0x0}, {HINDI_C_NA, HINDI_S_NUKTA, 0x0}},
    /* nna + halant  ->  half form of na */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_NNNA, HINDI_S_HALANT, 0x0}, {HINDI_H_NNNA, 0x0}},
    /* nna + halant + ra  -> na + subscript RA*/
    /* {0,3,2,MMI_ALL_LANG,                     {HINDI_C_NNNA,HINDI_S_HALANT,HINDI_C_RA,0x0},         {HINDI_C_NNNA,HINDI_DV_R,0x0} },*/
    /* nna + halant + na -> (ligature form) NNA */
    //{0, 3, 1, MMI_ALL_LANG, {HINDI_C_NNA, HINDI_S_HALANT, HINDI_C_NNA, 0x0}, {HINDI_L_NNNA, 0x0}},
    /* nna + halant + ra + halant -> half nna + RA + halant */
    /*{1, 4, 3, MMI_ALL_LANG, {HINDI_C_NNNA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_H_NNNA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},*/
    /* nna + halant + na + halant -> (ligature form) NNA + halant */
    /*{1, 4, 2, MMI_ALL_LANG, {HINDI_C_NNNA, HINDI_S_HALANT, HINDI_C_NNNA, HINDI_S_HALANT, 0x0},
     {HINDI_L_NNNA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_C_NNNA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_NNNA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}*/
};

const ire_glyph_info_table_struct PA_Tbl[] = 
{
    /* pa + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_PA, 0x0}, {HINDI_C_PA, 0x0}},
    /* pa + halant -> half of pa */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_PA, HINDI_S_HALANT, 0x0}, {HINDI_H_PA, 0x0}},
    /* pa + halant + ra -> pra OR pa + subscript RA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_PA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_PRA, 0x0}},
    /* pa + halant + ra + halant -> half pa + RA + halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_C_PA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_H_PA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_C_PA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_PA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct PHA_Tbl[] = 
{
    /* pha +space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_PHA, 0x0}, {HINDI_C_PHA, 0x0}},
    /* pha + halant -> half form of pha */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_PHA, HINDI_S_HALANT, 0x0}, {HINDI_H_PHA, 0x0}},
    /* pha + nukta -> pha with nukta form  */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_PHA, HINDI_S_NUKTA, 0x0}, {HINDI_AC_FA, 0x0}},
    /* pha + nukta + halant-> half pha with nukta */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_PHA, HINDI_S_NUKTA, HINDI_S_HALANT, 0x0}, {HINDI_H_PHA_N, 0x0}},
    /* pha + halant + ra -> fra OR pha + subscript RA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_PHA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_FRA, 0x0}},
    /* pha + nukta + halant + ra -> pha with nukta + subscript RA */
    {0, 4, 1, MMI_ALL_LANG, {HINDI_C_PHA, HINDI_S_NUKTA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_FRA_N, 0x0}},
    /* pha + halant + ra + halant -> half pha + RA + halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_C_PHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_H_PHA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_C_PHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_PHA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct BA_Tbl[] = 
{
    /* ba + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_BA, 0x0}, {HINDI_C_BA, 0x0}},
    /* ba + halant -> half form of ba */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_BA, HINDI_S_HALANT, 0x0}, {HINDI_H_BA, 0x0}},
    /* ba + halant + ra -> ba + subscript RA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_BA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_BRA, 0x0}}, 
    /* ba + halant + ra + halant -> ba + subscript RA + halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_C_BA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0}, 
     {HINDI_H_BA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_C_BA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_BA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct BHA_Tbl[] = 
{
    /* bha  + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_BHA, 0x0}, {HINDI_C_BHA, 0x0}},
    /* bha + halant -> half form of bha   */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_BHA, HINDI_S_HALANT, 0x0}, {HINDI_H_BHA, 0x0}},
    /* bha + halant + ra ->  bha + subscript RA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_BHA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_BHRA, 0x0}},       
    /* bha + halant + ra + halant -> half bha + RA + halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_C_BHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_H_BHA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_C_BHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_BHA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct MA_Tbl[] = 
{
    /* ma  + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_MA, 0x0}, {HINDI_C_MA, 0x0}},
    /* ma + halant -> half form of ma */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_MA, HINDI_S_HALANT, 0x0}, {HINDI_H_MA, 0x0}},
    /* ma + halant + ra -> ma + subscript RA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_MA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_MRA, 0x0}}, 
    /* ma + halant + ra + halant ->half ma + RA + halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_C_MA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0}, 
     {HINDI_H_MA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_C_MA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_MA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct YA_Tbl[] = 
{
    /* ya + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_YA, 0x0}, {HINDI_C_YA, 0x0}},
    /* ya + halant -> half form of ya */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_YA, HINDI_S_HALANT, 0x0}, {HINDI_H_YA, 0x0}},
    /* ya + nukta   -> ya with nukta form */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_YA, HINDI_S_NUKTA, 0x0}, {HINDI_AC_YYA, 0x0}},
    /* ya + nukta + halant -> half form of ya with nukta */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_YA, HINDI_S_NUKTA, HINDI_S_HALANT, 0x0}, {HINDI_L_YA_N, 0x0}},
    /* ya + halant + ra -> ya + subscript RA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_YA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_YRA, 0x0}}, 
    /* ya + halant + ra + halant -> ya+ subscript RA + halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_C_YA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0}, 
     {HINDI_H_YA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},      
    /* ya + nukta + halant + ra -> ya with nukta + subscript form of RA */
    // {0,4,2,MMI_ALL_LANG,                  {HINDI_C_YA,HINDI_S_HALANT,HINDI_C_RA,HINDI_S_HALANT,0x0},  {HINDI_AC_YYA,HINDI_DV_R,0x0} },/*Q*/
    /* ya + nukta +halant + ra + halant -> ya with nukta + subscript RA + halant */
    {0, 4, 1, MMI_ALL_LANG, {HINDI_C_YA, HINDI_S_NUKTA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_YRA_N, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_C_YA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_YA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct RA_Tbl[] = 
{
    /* ra + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_RA, 0x0}, {HINDI_C_RA, 0x0}},
    /* ra + Nukta  ->  rra */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_RA, HINDI_S_NUKTA, 0x0}, {HINDI_C_RRA, 0x0}},
    /* ra + halant -> superscript form of RA */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_RA, HINDI_S_HALANT, 0x0}, {HINDI_SUP_RA, 0x0}},
    /* ra + U -> Ru  */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_RA, HINDI_DV_U, 0x0}, {HINDI_L_RU, 0x0}},
    /* ra + UU -> RU  */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_RA, HINDI_DV_UU, 0x0}, {HINDI_L_RUU, 0x0}},
    /* rA + Nukta +Halant -> half form of ra with Nukta */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_RA, HINDI_S_NUKTA, HINDI_S_HALANT, 0x0}, {HINDI_H_EYLASH, 0x0}},
    
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_RA, HINDI_S_HALANT, HINDI_S_BINDU, 0x0}, {HINDI_L_RABIN, 0x0}},

    /* RA + HAL + HINDI_DV_E -> L_DVERA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_RA, HINDI_S_HALANT, HINDI_DV_E, 0x0}, {HINDI_L_DVERA, 0x0}},
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_RA, HINDI_S_HALANT, HINDI_DV_AI, 0x0}, {HINDI_L_DVAIRA, 0x0}},
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_RA, HINDI_S_HALANT, HINDI_DV_O, 0x0}, {HINDI_L_DVORA, 0x0}},
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_RA, HINDI_S_HALANT, HINDI_DV_AU, 0x0}, {HINDI_L_DVAURA, 0x0}},
    {0, 4, 1, MMI_ALL_LANG, {HINDI_C_RA, HINDI_S_HALANT, HINDI_DV_E, HINDI_S_BINDU, 0x0}, {HINDI_L_DVEBINRA, 0x0}},

    {0, 4, 1, MMI_ALL_LANG, {HINDI_C_RA, HINDI_S_HALANT, HINDI_DV_AI, HINDI_S_BINDU, 0x0}, {HINDI_L_DVAIBINRA, 0x0}},

    {0, 4, 1, MMI_ALL_LANG, {HINDI_C_RA, HINDI_S_HALANT, HINDI_DV_O, HINDI_S_BINDU, 0x0}, {HINDI_L_DVOBINRA, 0x0}},
};

const ire_glyph_info_table_struct RRA_Tbl[] = 
{
    /* ra + space */
    {0, 1, 2, MMI_ALL_LANG, {HINDI_C_RRA, 0x0}, {HINDI_C_RA, HINDI_S_NUKTA, 0x0}},
    /* ra + halant -> eyelash RA */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_RRA, HINDI_S_HALANT, 0x0}, {HINDI_H_EYLASH, 0x0}}
};

const ire_glyph_info_table_struct LA_Tbl[] = 
{
    /* la + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_LA, 0x0}, {HINDI_C_LA, 0x0}},
    /* la + halant -> half form of la */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_LA, HINDI_S_HALANT, 0x0}, {HINDI_H_LA, 0x0}},
    /* la + halant + ra -> half forFm of la + ra(nominal form) */
    {0, 3, 2, MMI_ALL_LANG, {HINDI_C_LA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_H_LA, HINDI_C_RA, 0x0}},

#if defined(__MMI_LANG_MARATHI__)
    {0, 3, 1, MMI_MARATHI_LANG, {HINDI_C_LA, HINDI_S_HALANT, HINDI_C_LA, 0x0}, {HINDI_L_LA_LA, 0x0}},
#endif 
};

const ire_glyph_info_table_struct LLA_Tbl[] = 
{
    /* Lla */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_LLA, 0x0}, {HINDI_C_LLA, 0x0}},
    /* LLa + Nukta  ->  LLaa */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_LLA, HINDI_S_NUKTA, 0x0}, {HINDI_C_LLLA, 0x0}},
    /* LLA + Nukta +Halant -> half form of LLa with Nukta */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_LLA, HINDI_S_NUKTA, HINDI_S_HALANT, 0x0}, {HINDI_H_LLLA, 0x0}},
    /* Lla + halant -> Half form of LLA */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_LLA, HINDI_S_HALANT, 0x0}, {HINDI_H_LLA, 0x0}},
    /* Lla + halant + ra-> Lla + halant + ra */
    //{0, 3, 2, MMI_ALL_LANG, {HINDI_C_LLA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_H_LLA, HINDI_C_RA, 0x0}},
    /* Lla + halant + ra + Halant-> Lla + halant + ra + Halant */
    /*{1, 4, 4, MMI_ALL_LANG, {HINDI_C_LLA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_H_LLA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},*/
    /*{1, 6, 4, MMI_ALL_LANG, {HINDI_C_LLA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_LLA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}*/
};

const ire_glyph_info_table_struct LLLA_Tbl[] = 
{
    /* Llla */
    {0, 1, 2, MMI_ALL_LANG, {HINDI_C_LLLA, 0x0}, {HINDI_C_LLA, HINDI_S_NUKTA, 0x0}},
    /* Llla + halant -> half form of LLLA */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_LLLA, HINDI_S_HALANT, 0x0}, {HINDI_H_LLLA, 0x0}},
    /* Llla + halant + ra-> Llla + halant + ra */
    //{0, 3, 2, MMI_ALL_LANG, {HINDI_C_LLLA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_H_LLLA, HINDI_C_RA, 0x0}},
    /* Llla + halant + ra + Halant-> Llla + halant + ra + Halant */
    /*{1, 4, 3, MMI_ALL_LANG, {HINDI_C_LLLA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_H_LLLA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_C_LLLA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_LLLA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}*/
};

const ire_glyph_info_table_struct VA_Tbl[] = 
{
    /* va */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_VA, 0x0}, {HINDI_C_VA, 0x0}},
    /* va + halant -> half form of va */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_VA, HINDI_S_HALANT, 0x0}, {HINDI_H_VA, 0x0}},
    /* va + halant + ra -> vra OR va + subscript RA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_VA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_VRA, 0x0}}, 
    /* va + halant + ra + halant -> vra OR va + subscript RA +halant  */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_C_VA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_H_VA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_C_VA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_VA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct SHA_Tbl[] = 
{
    /* sha */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_SHA, 0x0}, {HINDI_C_SHA, 0x0}},
    /* sha + halant -> half form of sha */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_SHA, HINDI_S_HALANT, 0x0}, {HINDI_H_SHA, 0x0}},
    /* sha + halant + ra -> shra ligature */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_SHA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_SHRA, 0x0}},
	/* sha + halant + ra + halant -> half shra */
	{0, 4, 1, MMI_ALL_LANG, {HINDI_C_SHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0}, {HINDI_H_SHRA, 0x0}},
	#if defined(__MMI_LANG_MARATHI__)
    {0, 5, 1, MMI_MARATHI_LANG, {HINDI_C_SHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, HINDI_C_CHA, 0x0}, {HINDI_L_SHRA_CHA, 0x0}},
    {0, 5, 1, MMI_MARATHI_LANG, {HINDI_C_SHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, HINDI_C_VA, 0x0}, {HINDI_L_SHRA_VA, 0x0}},
    {0, 5, 1, MMI_MARATHI_LANG, {HINDI_C_SHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, HINDI_C_LA, 0x0}, {HINDI_L_SHRA_LA, 0x0}},
    {0, 5, 1, MMI_MARATHI_LANG, {HINDI_C_SHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, HINDI_C_NA, 0x0}, {HINDI_L_SHRA_NA, 0x0}}
	#endif /* defined(__MMI_LANG_MARATHI__) */ 	
};

const ire_glyph_info_table_struct SSA_Tbl[] = 
{
    /* Ssa */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_SSA, 0x0}, {HINDI_C_SSA, 0x0}},
    /* SSA + halant -> half form of SSA  */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_SSA, HINDI_S_HALANT, 0x0}, {HINDI_H_SSA, 0x0}},
    /* ssa + halant + ra -> half ssa + ra(nominal form) */
    //{0, 3, 2, MMI_ALL_LANG, {HINDI_C_SSA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_H_SSA, HINDI_C_RA, 0x0}},

	#if defined(__MMI_LANG_MARATHI__)
    {0, 3, 1, MMI_MARATHI_LANG, {HINDI_C_SSA, HINDI_S_HALANT, HINDI_C_TTA, 0x0}, {HINDI_L_SSA_TTA, 0x0}},
	#endif 
    /* ssa + halant + ra + halant -> half ssa + ra + Halant */
    /*{1, 4, 3, MMI_ALL_LANG, {HINDI_C_SSA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_H_SSA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},*/

 	#if defined(__MMI_LANG_MARATHI__)
    {0, 5, 1, MMI_MARATHI_LANG, {HINDI_C_SSA, HINDI_S_HALANT, HINDI_C_TTA, HINDI_S_HALANT, HINDI_C_RA, 0x0},
     {HINDI_L_SSA_TTA_RA, 0x0}},
 	#endif /* defined(__MMI_LANG_MARATHI__) */ 
 	/*{1, 6, 4, MMI_ALL_LANG, {HINDI_C_SSA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_SSA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}*/
};

const ire_glyph_info_table_struct SA_Tbl[] = 
{
    /* sa + space */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_SA, 0x0}, {HINDI_C_SA, 0x0}},
    /* sa + halant -> half form of sa */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_SA, HINDI_S_HALANT, 0x0}, {HINDI_H_SA, 0x0}},
    /* sa + halant + ra -> sa + subscript RA OR sra */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_SA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_SRA, 0x0}},
    /* sa + halan + ra + halant -> half sa + RA + halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_C_SA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_H_SA, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_C_SA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_SA, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct HA_Tbl[] = 
{
    /* ha + space  */
    {0, 1, 1, MMI_ALL_LANG, {HINDI_C_HA, 0x0}, {HINDI_C_HA, 0x0}},
    /* ha + halant  -> half ha */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_C_HA, HINDI_S_HALANT, 0x0}, {HINDI_H_HA, 0x0}},
    /* ha + halant + ra -> ligature form OR ha + subscript RA(special form) */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_HRA, 0x0}},
    /* ha + halant + ya -> ligature form */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_YA, 0x0}, {HINDI_L_HYA, 0x0}},
    /* ha + halant + ma -> ligature form */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_MA, 0x0}, {HINDI_L_HMMA, 0x0}},
    /* ha + halant + ya -> ligature form */
    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_YA, HINDI_S_HALANT, 0x0},
     {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_YA, HINDI_S_HALANT, 0x0}},
    /* ha + halant + ya -> ligature form */
    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_MA, HINDI_S_HALANT, 0x0},
     {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_MA, HINDI_S_HALANT, 0x0}},

    {1, 4, 4, MMI_ALL_LANG, {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_U, 0x0},
     {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_U, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_UU, 0x0},
     {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_UU, 0x0}},
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_R, 0x0},
     {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_RA, HINDI_DV_R, 0x0}},

    /* ha + halant + ya -> ligature form */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_YA, HINDI_DV_U, 0x0},
     {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_YA, HINDI_DV_U, 0x0}},
    /* ha + halant + ya -> ligature form */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_MA, HINDI_DV_U, 0x0},
     {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_MA, HINDI_DV_U, 0x0}},

    /* ha + halant + ya -> ligature form */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_YA, HINDI_DV_UU, 0x0},
     {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_YA, HINDI_DV_UU, 0x0}},
    /* ha + halant + ya -> ligature form */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_MA, HINDI_DV_UU, 0x0},
     {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_MA, HINDI_DV_UU, 0x0}},

    /* ha + halant + ya -> ligature form */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_YA, HINDI_DV_R, 0x0},
     {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_YA, HINDI_DV_R, 0x0}},
    /* ha + halant + ya -> ligature form */
    {0, 4, 4, MMI_ALL_LANG, {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_MA, HINDI_DV_R, 0x0},
     {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_MA, HINDI_DV_R, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_YA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_YA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_MA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_MA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 5, MMI_ALL_LANG, {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_C_HA, HINDI_S_HALANT, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

                           /* ADDITIONAL CONSONANT'S TABLE */

const ire_glyph_info_table_struct QA_Tbl[] = 
{
    /* qa + space */
    {0, 1, 2, MMI_ALL_LANG, {HINDI_AC_QA, 0x0}, {HINDI_C_KA, HINDI_S_NUKTA, 0x0}},
    /* qa + Halant/Virama -> Half form of qa */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_AC_QA, HINDI_S_HALANT, 0x0}, {HINDI_L_KA_N, 0x0}},
    //  /* qa + Halant + Ra -> qra */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_AC_QA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_KRA_N, 0x0}},
    /* Qa + Halant + Ra + Halant -> Half Form of Ka + Ra + Halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_AC_QA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_L_KA_N, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_AC_QA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_L_KA_N, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct KHHA_Tbl[] = 
{
    /* khha + space */
    {0, 1, 2, MMI_ALL_LANG, {HINDI_AC_KHHA, 0x0}, {HINDI_C_KHA, HINDI_S_NUKTA, 0x0}},
    /* Khha + halant  -> half form of khha */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_AC_KHHA, HINDI_S_HALANT, 0x0}, {HINDI_L_KHA_N, 0x0}},
    /* khha + halant + ra -> Khha + subscript form of RA OR KHHRA as a whole */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_AC_KHHA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_KHRA_N, 0x0}},   
    /* khha + halant + ra + halant -> half khha + RA  + halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_AC_KHHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_L_KHA_N, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_AC_KHHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_L_KHA_N, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct GHHA_Tbl[] = 
{
    /* ghhaa + space */
    {0, 1, 2, MMI_ALL_LANG, {HINDI_AC_GHHA, 0x0}, {HINDI_C_GA, HINDI_S_NUKTA, 0x0}},
    /* ghha + halant -> half form of ghha */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_AC_GHHA, HINDI_S_HALANT, 0x0}, {HINDI_L_GA_N, 0x0}},
    /* ghha + halant + ra -> ghha + subscript form of RA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_AC_GHHA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_GRA_N, 0x0}},    
    /* ghha + halant + ra + halant -> half ghha + RA + halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_AC_GHHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_L_GA_N, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_AC_GHHA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_L_GA_N, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct ZA_Tbl[] = 
{
    /* za + space */
    {0, 1, 2, MMI_ALL_LANG, {HINDI_AC_ZA, 0x0}, {HINDI_C_JA, HINDI_S_NUKTA, 0x0}},
    /* za + halant -> half form of za  */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_AC_ZA, HINDI_S_HALANT, 0x0}, {HINDI_H_JA_N, 0x0}},
    /* za + halant + ra -> zra  */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_AC_ZA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_JRA_N, 0x0}},      
    /* za + halant + ra + halant -> zra + halant or za + subscript RA + halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_AC_ZA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_H_JA_N, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_AC_ZA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_JA_N, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct DDDHA_Tbl[] = 
{
    /* dddha + space */
    {0, 1, 2, MMI_ALL_LANG, {HINDI_AC_DDDHA, 0x0}, {HINDI_C_DDA, HINDI_S_NUKTA, 0x0}},
    /* dddha + halant -> dddha+ halant */
    //{0, 2, 2, MMI_ALL_LANG, {HINDI_AC_DDDHA, HINDI_S_HALANT, 0x0}, {HINDI_AC_DDDHA, HINDI_S_HALANT, 0x0}},
};

const ire_glyph_info_table_struct RHA_Tbl[] = 
{
    /* rha + space */
    {0, 1, 2, MMI_ALL_LANG, {HINDI_AC_RHA, 0x0}, {HINDI_C_DDHA, HINDI_S_NUKTA, 0x0}},
    /* rha + halant -> rha + halant */
   //{0, 2, 2, MMI_ALL_LANG, {HINDI_AC_RHA, HINDI_S_HALANT, 0x0}, {HINDI_AC_RHA, HINDI_S_HALANT, 0x0}},
};

const ire_glyph_info_table_struct FA_Tbl[] = 
{
    /* fa +space */
    {0, 1, 2, MMI_ALL_LANG, {HINDI_AC_FA, 0x0}, {HINDI_C_PHA, HINDI_S_NUKTA, 0x0}},
    /* fa + halant -> half form of fa */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_AC_FA, HINDI_S_HALANT, 0x0}, {HINDI_H_PHA_N, 0x0}},
    /* fa + halant + ra -> fra OR fa + subscript RA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_AC_FA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_FRA_N, 0x0}},      
    /* fa + halant + ra + halant -> half fa + ra + halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_AC_FA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_H_PHA_N, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_AC_FA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_H_PHA_N, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct YYA_Tbl[] = 
{
    /* yya + space */
    {0, 1, 2, MMI_ALL_LANG, {HINDI_AC_YYA, 0x0}, {HINDI_C_YA, HINDI_S_NUKTA, 0x0}},
    /* yya + halant -> half form of yya */
    {0, 2, 1, MMI_ALL_LANG, {HINDI_AC_YYA, HINDI_S_HALANT, 0x0}, {HINDI_L_YA_N, 0x0}},
    /* yya + halant + ra -> yya + subscript RA */
    {0, 3, 1, MMI_ALL_LANG, {HINDI_AC_YYA, HINDI_S_HALANT, HINDI_C_RA, 0x0}, {HINDI_L_YRA_N, 0x0}},     
    /* yya + halant + ra + halant -> half yya+ RA + halant */
    {1, 4, 3, MMI_ALL_LANG, {HINDI_AC_YYA, HINDI_S_HALANT, HINDI_C_RA, HINDI_S_HALANT, 0x0},
     {HINDI_L_YA_N, HINDI_C_RA, HINDI_S_HALANT, 0x0}},
    {1, 6, 4, MMI_ALL_LANG, {HINDI_AC_YYA, HINDI_S_HALANT, HINDI_C_RA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_HALANT, 0x0},
     {HINDI_L_YA_N, HINDI_C_RA, HINDI_SUP_RA, HINDI_S_HALANT, 0x0}}
};

              /* VARIOUS SIGNS TABLES */
const ire_glyph_info_table_struct CHNDR_BINDU_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_S_CHANDRA_BINDU, 0x0}, {HINDI_S_CHANDRA_BINDU, 0x0}}
};

const ire_glyph_info_table_struct BINDU_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_S_BINDU, 0x0}, {HINDI_S_BINDU, 0x0}}
};

const ire_glyph_info_table_struct VISARGA_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_S_VISARGA, 0x0}, {HINDI_S_VISARGA, 0x0}}
};

const ire_glyph_info_table_struct NUKTA_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_S_NUKTA, 0x0}, {HINDI_S_NUKTA, 0x0}}
};

const ire_glyph_info_table_struct OM_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_S_OM, 0x0}, {HINDI_S_OM, 0x0}}
};

                        /* INDEPENDENT VOWEL'S TABLE */

const ire_glyph_info_table_struct NSA_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_NIV_A, 0x0}, {HINDI_NIV_A, 0x0}}
};

const ire_glyph_info_table_struct A_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_IV_A, 0x0}, {HINDI_IV_A, 0x0}}
};

const ire_glyph_info_table_struct AA_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_IV_AA, 0x0}, {HINDI_IV_AA, 0x0}}
};

const ire_glyph_info_table_struct I_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_IV_I, 0x0}, {HINDI_IV_I, 0x0}}
};

const ire_glyph_info_table_struct II_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_IV_II, 0x0}, {HINDI_IV_II, 0x0}}
};

const ire_glyph_info_table_struct U_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_IV_U, 0x0}, {HINDI_IV_U, 0x0}}
};

const ire_glyph_info_table_struct UU_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_IV_UU, 0x0}, {HINDI_IV_UU, 0x0}}
};

const ire_glyph_info_table_struct R_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_IV_R, 0x0}, {HINDI_IV_R, 0x0}}
};

const ire_glyph_info_table_struct NVL_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_NIV_L, 0x0}, {HINDI_NIV_L, 0x0}}
};

const ire_glyph_info_table_struct NCE_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_NIV_CE, 0x0}, {HINDI_NIV_CE, 0x0}},
    {0, 2, 2, MMI_ALL_LANG, {HINDI_NIV_CE, HINDI_S_BINDU, 0x0}, {HINDI_IV_E, HINDI_S_CHANDRA_BINDU, 0x0}}
};

const ire_glyph_info_table_struct NSE_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_NIV_SE, 0x0}, {HINDI_NIV_SE, 0x0}}
};

const ire_glyph_info_table_struct E_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_IV_E, 0x0}, {HINDI_IV_E, 0x0}}
};

const ire_glyph_info_table_struct AI_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_IV_AI, 0x0}, {HINDI_IV_AI, 0x0}}
};

const ire_glyph_info_table_struct NCO_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_NIV_CO, 0x0}, {HINDI_NIV_CO, 0x0}},
    {0, 2, 2, MMI_ALL_LANG, {HINDI_NIV_CO, HINDI_S_BINDU, 0x0}, {HINDI_IV_AA, HINDI_L_CHANDRA_BINDU, 0x0}}
};

const ire_glyph_info_table_struct NCSO_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_NIV_SO, 0x0}, {HINDI_NIV_SO, 0x0}}
};

const ire_glyph_info_table_struct O_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_IV_O, 0x0}, {HINDI_IV_O, 0x0}},
    {0, 2, 2, MMI_ALL_LANG, {HINDI_IV_O, HINDI_S_BINDU, 0x0}, {HINDI_IV_A, HINDI_L_MO, 0x0}}
};

const ire_glyph_info_table_struct AU_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_IV_AU, 0x0}, {HINDI_IV_AU, 0x0}},
    {0, 2, 2, MMI_ALL_LANG, {HINDI_IV_AU, HINDI_S_BINDU, 0x0}, {HINDI_IV_A, HINDI_L_MAU, 0x0}}
};

                      /* DEPENDENT VOWEL SIGN/MATRAS TABLES */

const ire_glyph_info_table_struct MAA_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_DV_AA, 0x0}, {HINDI_DV_AA, 0x0}},
    {0, 2, 1, MMI_ALL_LANG, {HINDI_DV_AA, HINDI_S_CHANDRA_BINDU, 0x0}, {HINDI_L_NCAND, 0x0}},
    {0, 2, 1, MMI_ALL_LANG, {HINDI_DV_AA, HINDI_S_BINDU, 0x0}, {HINDI_L_NBINDU, 0x0}},
    {0, 4, 2, MMI_ALL_LANG, {HINDI_DV_AA, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_CHANDRA_BINDU, 0x0}, {HINDI_L_NCAND, HINDI_SUP_RA, 0x0}}
};

const ire_glyph_info_table_struct MI_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_DV_I, 0x0}, {HINDI_DV_I, 0x0}},
    /* {0, 3, 1, MMI_ALL_LANG, {HINDI_DV_I, HINDI_C_RA, HINDI_S_HALANT, 0x0}, {HINDI_L_IRAHAL, 0x0}}, */
    {0, 4, 1, MMI_ALL_LANG, {HINDI_DV_I, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_BINDU, 0x0}, {HINDI_L_IRAHALB, 0x0}}
};

const ire_glyph_info_table_struct MII_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_DV_II, 0x0}, {HINDI_DV_II, 0x0}},
    {0, 2, 1, MMI_ALL_LANG, {HINDI_DV_II, HINDI_S_BINDU, 0x0}, {HINDI_L_MII, 0x0}},
    {0, 3, 1, MMI_ALL_LANG, {HINDI_DV_II, HINDI_C_RA, HINDI_S_HALANT, 0x0}, {HINDI_L_MIIRA, 0x0}},
    {0, 4, 1, MMI_ALL_LANG, {HINDI_DV_II, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_BINDU, 0x0}, {HINDI_L_IIRAHALB, 0x0}}
};

const ire_glyph_info_table_struct MU_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_DV_U, 0x0}, {HINDI_DV_U, 0x0}}
};

const ire_glyph_info_table_struct MUU_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_DV_UU, 0x0}, {HINDI_DV_UU, 0x0}}
};

const ire_glyph_info_table_struct MR_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_DV_R, 0x0}, {HINDI_DV_R, 0x0}}
};

const ire_glyph_info_table_struct MC_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_DV_CE, 0x0}, {HINDI_DV_CE, 0x0}},
    {0, 2, 1, MMI_ALL_LANG, {HINDI_DV_CE, HINDI_S_BINDU, 0x0}, {HINDI_S_CHANDRA_BINDU, 0x0}},
    {0, 2, 1, MMI_ALL_LANG, {HINDI_DV_CE, HINDI_S_CHANDRA_BINDU, 0x0}, {HINDI_S_CHANDRA_BINDU, 0x0}}
};

const ire_glyph_info_table_struct ME_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_DV_E, 0x0}, {HINDI_DV_E, 0x0}},
    {0, 2, 1, MMI_ALL_LANG, {HINDI_DV_E, HINDI_S_BINDU, 0x0}, {HINDI_L_ME, 0x0}},
    {0, 3, 1, MMI_ALL_LANG, {HINDI_DV_E, HINDI_C_RA, HINDI_S_HALANT, 0x0}, {HINDI_L_DVERA, 0x0}},
    {0, 4, 1, MMI_ALL_LANG, {HINDI_DV_E, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_BINDU, 0x0}, {HINDI_L_DVEBINRA, 0x0}}
};

const ire_glyph_info_table_struct MAI_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_DV_AI, 0x0}, {HINDI_DV_AI, 0x0}},
    {0, 2, 1, MMI_ALL_LANG, {HINDI_DV_AI, HINDI_S_BINDU, 0x0}, {HINDI_L_MAI, 0x0}},
    {0, 3, 1, MMI_ALL_LANG, {HINDI_DV_AI, HINDI_C_RA, HINDI_S_HALANT, 0x0}, {HINDI_L_DVAIRA, 0x0}},
    {0, 4, 1, MMI_ALL_LANG, {HINDI_DV_AI, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_BINDU, 0x0}, {HINDI_L_DVAIBINRA, 0x0}}
};

const ire_glyph_info_table_struct MCO_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_NDV_CO, 0x0}, {HINDI_NDV_CO, 0x0}},
    {0, 2, 1, MMI_ALL_LANG, {HINDI_NDV_CO, HINDI_S_BINDU, 0x0}, {HINDI_L_NCAND, 0x0}}
};

const ire_glyph_info_table_struct MO_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_DV_O, 0x0}, {HINDI_DV_O, 0x0}},
    {0, 2, 1, MMI_ALL_LANG, {HINDI_DV_O, HINDI_S_BINDU, 0x0}, {HINDI_L_MO, 0x0}},
    {0, 3, 1, MMI_ALL_LANG, {HINDI_DV_O, HINDI_C_RA, HINDI_S_HALANT, 0x0}, {HINDI_L_DVORA, 0x0}},
    {0, 4, 1, MMI_ALL_LANG, {HINDI_DV_O, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_BINDU, 0x0}, {HINDI_L_DVOBINRA, 0x0}}
};

const ire_glyph_info_table_struct MAU_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_DV_AU, 0x0}, {HINDI_DV_AU, 0x0}},
    {0, 2, 1, MMI_ALL_LANG, {HINDI_DV_AU, HINDI_S_BINDU, 0x0}, {HINDI_L_MAU, 0x0}},
    {0, 3, 1, MMI_ALL_LANG, {HINDI_DV_AU, HINDI_C_RA, HINDI_S_HALANT, 0x0}, {HINDI_L_DVAURA, 0x0}},
    {0, 4, 1, MMI_ALL_LANG, {HINDI_DV_AU, HINDI_C_RA, HINDI_S_HALANT, HINDI_S_BINDU, 0x0}, {HINDI_L_DVAUBINRA, 0x0}}
};

const ire_glyph_info_table_struct HAL_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_S_HALANT, 0x0}, {HINDI_S_HALANT, 0x0}}
};

const ire_glyph_info_table_struct NGD_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_NG_D, 0x0}, {HINDI_NG_D, 0x0}}
};

const ire_glyph_info_table_struct NGDD_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_NG_DD, 0x0}, {HINDI_NG_DD, 0x0}}
};

const ire_glyph_info_table_struct NABBR_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_S_ABBR, 0x0}, {HINDI_S_ABBR, 0x0}}
};

                       /* NUMERALS TABLE */
const ire_glyph_info_table_struct ZERO_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_D_ZERO, 0x0}, {HINDI_D_ZERO, 0x0}}
};

const ire_glyph_info_table_struct ONE_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_D_ONE, 0x0}, {HINDI_D_ONE, 0x0}}
};

const ire_glyph_info_table_struct TWO_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_D_TWO, 0x0}, {HINDI_D_TWO, 0x0}}
};

const ire_glyph_info_table_struct THREE_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_D_THREE, 0x0}, {HINDI_D_THREE, 0x0}}
};

const ire_glyph_info_table_struct FOUR_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_D_FOUR, 0x0}, {HINDI_D_FOUR, 0x0}}
};

const ire_glyph_info_table_struct FIVE_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_D_FIVE, 0x0}, {HINDI_D_FIVE, 0x0}}
};

const ire_glyph_info_table_struct SIX_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_D_SIX, 0x0}, {HINDI_D_SIX, 0x0}}
};

const ire_glyph_info_table_struct SEVEN_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_D_SEVEN, 0x0}, {HINDI_D_SEVEN, 0x0}}
};

const ire_glyph_info_table_struct EIGHT_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_D_EIGHT, 0x0}, {HINDI_D_EIGHT, 0x0}}
};

const ire_glyph_info_table_struct NINE_Tbl[] = 
{
    {0, 1, 1, MMI_ALL_LANG, {HINDI_D_NINE, 0x0}, {HINDI_D_NINE, 0x0}}
};

const ire_main_glyph_table_struct hfDvngTbl[] = 
{

    /* NULL for devanagari sign Short A */
    {CHNDR_BINDU_Tbl, sizeof(CHNDR_BINDU_Tbl) / sizeof(CHNDR_BINDU_Tbl[0]), 1}
    ,
    {BINDU_Tbl, sizeof(BINDU_Tbl) / sizeof(BINDU_Tbl[0]), 1}
    ,
    {VISARGA_Tbl, sizeof(VISARGA_Tbl) / sizeof(VISARGA_Tbl[0]), 1}
    ,
    /* INDEPENDENT VOWELS */
    /* NULL For Independent Vowel L,Chandra E,Short E,Chandra O,Short O */
    {NSA_Tbl, sizeof(NSA_Tbl) / sizeof(NSA_Tbl[0]), 1}
    ,
    {A_Tbl, sizeof(A_Tbl) / sizeof(A_Tbl[0]), 1}
    ,
    {AA_Tbl, sizeof(AA_Tbl) / sizeof(AA_Tbl[0]), 1}
    ,
    {I_Tbl, sizeof(I_Tbl) / sizeof(I_Tbl[0]), 1}
    ,
    {II_Tbl, sizeof(II_Tbl) / sizeof(II_Tbl[0]), 1}
    ,
    {U_Tbl, sizeof(U_Tbl) / sizeof(U_Tbl[0]), 1}
    ,
    {UU_Tbl, sizeof(UU_Tbl) / sizeof(UU_Tbl[0]), 1}
    ,
    {R_Tbl, sizeof(R_Tbl) / sizeof(R_Tbl[0]), 1}
    ,
    {NVL_Tbl, sizeof(NVL_Tbl) / sizeof(NVL_Tbl[0]), 1}
    ,
    {NCE_Tbl, sizeof(NCE_Tbl) / sizeof(NCE_Tbl[0]), 2}
    ,
    {NSE_Tbl, sizeof(NSE_Tbl) / sizeof(NSE_Tbl[0]), 1}
    ,
    {E_Tbl, sizeof(E_Tbl) / sizeof(E_Tbl[0]), 1}
    ,
    {AI_Tbl, sizeof(AI_Tbl) / sizeof(AI_Tbl[0]), 1}
    ,
    {NCO_Tbl, sizeof(NCO_Tbl) / sizeof(NCO_Tbl[0]), 2}
    ,
    {NCSO_Tbl, sizeof(NCSO_Tbl) / sizeof(NCSO_Tbl[0]), 1}
    ,
    {O_Tbl, sizeof(O_Tbl) / sizeof(O_Tbl[0]), 2}
    ,
    {AU_Tbl, sizeof(AU_Tbl) / sizeof(AU_Tbl[0]), 2}
    ,

    /* Consonants */
    {KA_Tbl, sizeof(KA_Tbl) / sizeof(KA_Tbl[0]), 6}
    ,
    {KHA_Tbl, sizeof(KHA_Tbl) / sizeof(KHA_Tbl[0]), 6}
    ,
    {GA_Tbl, sizeof(GA_Tbl) / sizeof(GA_Tbl[0]), 6}
    ,
    {GHA_Tbl, sizeof(GHA_Tbl) / sizeof(GHA_Tbl[0]), 6}
    ,
    {NGA_Tbl, sizeof(NGA_Tbl) / sizeof(NGA_Tbl[0]), 6}
    ,
    {CHA_Tbl, sizeof(CHA_Tbl) / sizeof(CHA_Tbl[0]), 6}
    ,
    {CHHA_Tbl, sizeof(CHHA_Tbl) / sizeof(CHHA_Tbl[0]), 6}
    ,
    {JA_Tbl, sizeof(JA_Tbl) / sizeof(JA_Tbl[0]), 6}
    ,
    {JHA_Tbl, sizeof(JHA_Tbl) / sizeof(JHA_Tbl[0]), 6}
    ,
    {NYA_Tbl, sizeof(NYA_Tbl) / sizeof(NYA_Tbl[0]), 1}
    ,
    {TTA_Tbl, sizeof(TTA_Tbl) / sizeof(TTA_Tbl[0]), 6}
    ,
    {TTHA_Tbl, sizeof(TTHA_Tbl) / sizeof(TTHA_Tbl[0]), 6}
    ,
    {DDA_Tbl, sizeof(DDA_Tbl) / sizeof(DDA_Tbl[0]), 6}
    ,
    {DDHA_Tbl, sizeof(DDHA_Tbl) / sizeof(DDHA_Tbl[0]), 6}
    ,
    {NNA_Tbl, sizeof(NNA_Tbl) / sizeof(NNA_Tbl[0]), 3}
    ,
    {TA_Tbl, sizeof(TA_Tbl) / sizeof(TA_Tbl[0]), 4}
    ,
    {THA_Tbl, sizeof(THA_Tbl) / sizeof(THA_Tbl[0]), 6}
    ,
    {DA_Tbl, sizeof(DA_Tbl) / sizeof(DA_Tbl[0]), 6}
    ,
    {DHA_Tbl, sizeof(DHA_Tbl) / sizeof(DHA_Tbl[0]), 6}
    ,
    {NA_Tbl, sizeof(NA_Tbl) / sizeof(NA_Tbl[0]), 6}
    ,
    {NNNA_Tbl, sizeof(NNNA_Tbl) / sizeof(NNNA_Tbl[0]), 2}
    ,
    {PA_Tbl, sizeof(PA_Tbl) / sizeof(PA_Tbl[0]), 6}
    ,
    {PHA_Tbl, sizeof(PHA_Tbl) / sizeof(PHA_Tbl[0]), 6}
    ,
    {BA_Tbl, sizeof(BA_Tbl) / sizeof(BA_Tbl[0]), 6}
    ,
    {BHA_Tbl, sizeof(BHA_Tbl) / sizeof(BHA_Tbl[0]), 6}
    ,
    {MA_Tbl, sizeof(MA_Tbl) / sizeof(MA_Tbl[0]), 6}
    ,
    {YA_Tbl, sizeof(YA_Tbl) / sizeof(YA_Tbl[0]), 6}
    ,
    {RA_Tbl, sizeof(RA_Tbl) / sizeof(RA_Tbl[0]), 2}
    ,
    {RRA_Tbl, sizeof(RRA_Tbl) / sizeof(RRA_Tbl[0]), 2}
    ,
    {LA_Tbl, sizeof(LA_Tbl) / sizeof(LA_Tbl[0]), 3}
    ,
    {LLA_Tbl, sizeof(LLA_Tbl) / sizeof(LLA_Tbl[0]), 2}
    ,
    {LLLA_Tbl, sizeof(LLLA_Tbl) / sizeof(LLLA_Tbl[0]), 2}
    ,
    {VA_Tbl, sizeof(VA_Tbl) / sizeof(VA_Tbl[0]), 6}
    ,
    {SHA_Tbl, sizeof(SHA_Tbl) / sizeof(SHA_Tbl[0]), 5}
    ,
    {SSA_Tbl, sizeof(SSA_Tbl) / sizeof(SSA_Tbl[0]), 5}
    ,
    {SA_Tbl, sizeof(SA_Tbl) / sizeof(SA_Tbl[0]), 6}
    ,
    {HA_Tbl, sizeof(HA_Tbl) / sizeof(HA_Tbl[0]), 6}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,

    /* Various Signs */
    /* NULL for Avagraha */
    {NUKTA_Tbl, sizeof(NUKTA_Tbl) / sizeof(NUKTA_Tbl[0]), 1}
    ,
    {NULL, 0, 0}
    ,

    /* Dependent Vowel Signs */
    /* Null for RR,Short E,Chandra O,Short O */
    {MAA_Tbl, sizeof(MAA_Tbl) / sizeof(MAA_Tbl[0]), 4}
    ,
    {MI_Tbl, sizeof(MI_Tbl) / sizeof(MI_Tbl[0]), 4}
    ,
    {MII_Tbl, sizeof(MII_Tbl) / sizeof(MII_Tbl[0]), 4}
    ,
    {MU_Tbl, sizeof(MU_Tbl) / sizeof(MU_Tbl[0]), 1}
    ,
    {MUU_Tbl, sizeof(MUU_Tbl) / sizeof(MUU_Tbl[0]), 1}
    ,
    {MR_Tbl, sizeof(MR_Tbl) / sizeof(MR_Tbl[0]), 1}
    ,
    {NULL, 0, 0}
    ,
    {MC_Tbl, sizeof(MC_Tbl) / sizeof(MC_Tbl[0]), 2}
    ,
    {NULL, 0, 0}
    ,
    {ME_Tbl, sizeof(ME_Tbl) / sizeof(ME_Tbl[0]), 4}
    ,
    {MAI_Tbl, sizeof(MAI_Tbl) / sizeof(MAI_Tbl[0]), 4}
    ,
    {MCO_Tbl, sizeof(MCO_Tbl) / sizeof(MCO_Tbl[0]), 2}
    ,
    {NULL, 0, 0}
    ,
    {MO_Tbl, sizeof(MO_Tbl) / sizeof(MO_Tbl[0]), 4}
    ,
    {MAU_Tbl, sizeof(MAU_Tbl) / sizeof(MAU_Tbl[0]), 4}
    ,

    /* Various Signs */
    {HAL_Tbl, sizeof(HAL_Tbl) / sizeof(HAL_Tbl[0]), 1}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {OM_Tbl, sizeof(OM_Tbl) / sizeof(OM_Tbl[0]), 1}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    /* Additional Consonant */
    {QA_Tbl, sizeof(QA_Tbl) / sizeof(QA_Tbl[0]), 6}
    ,
    {KHHA_Tbl, sizeof(KHHA_Tbl) / sizeof(KHHA_Tbl[0]), 6}
    ,
    {GHHA_Tbl, sizeof(GHHA_Tbl) / sizeof(GHHA_Tbl[0]), 6}
    ,
    {ZA_Tbl, sizeof(ZA_Tbl) / sizeof(ZA_Tbl[0]), 6}
    ,
    {DDDHA_Tbl, sizeof(DDDHA_Tbl) / sizeof(DDDHA_Tbl[0]), 2}
    ,
    {RHA_Tbl, sizeof(RHA_Tbl) / sizeof(RHA_Tbl[0]), 2}
    ,
    {FA_Tbl, sizeof(FA_Tbl) / sizeof(FA_Tbl[0]), 6}
    ,
    {YYA_Tbl, sizeof(YYA_Tbl) / sizeof(YYA_Tbl[0]), 6}
    ,
    /* Generic Addition,used in Sanskrit only */
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {NGD_Tbl, sizeof(NGD_Tbl) / sizeof(NGD_Tbl[0]), 1}
    ,
    {NGDD_Tbl, sizeof(NGDD_Tbl) / sizeof(NGDD_Tbl[0]), 1}
    ,
    {ZERO_Tbl, sizeof(ZERO_Tbl) / sizeof(ZERO_Tbl[0]), 1}
    ,
    {ONE_Tbl, sizeof(ONE_Tbl) / sizeof(ONE_Tbl[0]), 1}
    ,
    {TWO_Tbl, sizeof(TWO_Tbl) / sizeof(TWO_Tbl[0]), 1}
    ,
    {THREE_Tbl, sizeof(THREE_Tbl) / sizeof(THREE_Tbl[0]), 1}
    ,
    {FOUR_Tbl, sizeof(FOUR_Tbl) / sizeof(FOUR_Tbl[0]), 1}
    ,
    {FIVE_Tbl, sizeof(FIVE_Tbl) / sizeof(FIVE_Tbl[0]), 1}
    ,
    {SIX_Tbl, sizeof(SIX_Tbl) / sizeof(SIX_Tbl[0]), 1}
    ,
    {SEVEN_Tbl, sizeof(SEVEN_Tbl) / sizeof(SEVEN_Tbl[0]), 1}
    ,
    {EIGHT_Tbl, sizeof(EIGHT_Tbl) / sizeof(EIGHT_Tbl[0]), 1}
    ,
    {NINE_Tbl, sizeof(NINE_Tbl) / sizeof(NINE_Tbl[0]), 1}
    ,
    {NABBR_Tbl, sizeof(NABBR_Tbl) / sizeof(NABBR_Tbl[0]), 1}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
    ,
    {NULL, 0, 0}
};

/* Hindi string array */

#if defined (__MMI_MULTITAP_HINDI__)
#if defined(__MMI_ZI_HINDI__)

#if defined (__MMI_ZI_V7__)
const UI_character_type default_hindi_multitap_strings[MAX_MULTITAPS][45] = 
{

    {' ', HINDI_C_SSA,HINDI_C_SA,HINDI_C_HA, '0', '\0'}, /* key 0 */
    { HINDI_NG_D, '1', '\0'},           /* KEY 1 */
    {HINDI_IV_A, HINDI_IV_AA,HINDI_IV_I, HINDI_IV_II, HINDI_IV_U, HINDI_IV_UU, HINDI_IV_R, '2', '\0'}, /* Key 2 */
    {HINDI_IV_E, HINDI_IV_AI, HINDI_NIV_CE, HINDI_IV_O, HINDI_IV_AU, HINDI_NIV_CO, '3', '\0'}, /* Key 3 */
    {HINDI_C_KA, HINDI_C_KHA, HINDI_C_GA, HINDI_C_GHA, HINDI_C_NGA, HINDI_AC_QA, HINDI_AC_KHHA, HINDI_AC_GHHA, '4', '\0'},  /* key 4 */
    {HINDI_C_CHA, HINDI_C_CHHA, HINDI_C_JA,  HINDI_C_JHA, HINDI_C_NYA, HINDI_AC_ZA, '5', '\0'}, /* key 5 */
    {HINDI_C_TTA, HINDI_C_TTHA, HINDI_C_DDA,  HINDI_C_DDHA,  HINDI_C_NNA,HINDI_AC_DDDHA, HINDI_AC_RHA, '6','\0'},       /* key 6 */
    {HINDI_C_TA,HINDI_C_THA,HINDI_C_DA,HINDI_C_DHA,HINDI_C_NA,HINDI_C_NNNA,'7','\0'},      /* key 7 */
    {HINDI_C_PA,HINDI_C_PHA,  HINDI_C_BA,HINDI_C_BHA,HINDI_C_MA,HINDI_AC_FA,'8','\0'},    /* key 8 */
    {HINDI_C_YA, HINDI_C_RA,  HINDI_C_LA, HINDI_C_VA, HINDI_C_SHA, HINDI_AC_YYA, HINDI_C_RRA,'9', '\0'},      /* key 9 */
 #if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
 #else /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
    {'\0'},
    {'\0'},
 #endif /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
};

const UI_character_type default_hindimatra_multitap_strings[MAX_MULTITAPS][45] = 
{
    {' ', HINDI_C_SSA,HINDI_C_SA,HINDI_C_HA,HINDI_S_CHANDRA_BINDU,HINDI_S_VISARGA,HINDI_S_HALANT, '0', '\0'}, /* key 0 */
    {HINDI_S_BINDU, HINDI_S_NUKTA, HINDI_NG_D, '1', '\0' },    /* KEY 1 */    
    {HINDI_DV_AA, HINDI_DV_I, HINDI_DV_II, HINDI_DV_U, HINDI_DV_UU,
     HINDI_DV_R,HINDI_IV_A, HINDI_IV_AA, HINDI_IV_I,HINDI_IV_II, HINDI_IV_U, HINDI_IV_UU, HINDI_IV_R, '2', '\0'}, /* Key 2 */
    {HINDI_DV_E, HINDI_DV_AI, HINDI_DV_CE, HINDI_DV_O, HINDI_DV_AU, 
	 HINDI_NDV_CO, HINDI_IV_E, HINDI_IV_AI, HINDI_NIV_CE, HINDI_IV_O, HINDI_IV_AU, HINDI_NIV_CO, '3', '\0'}, /* Key 3 */
    {HINDI_C_KA, HINDI_C_KHA, HINDI_C_GA, HINDI_C_GHA, HINDI_C_NGA, HINDI_AC_QA, HINDI_AC_KHHA, HINDI_AC_GHHA, '4', '\0'},  /* key 4 */
    {HINDI_C_CHA, HINDI_C_CHHA, HINDI_C_JA,  HINDI_C_JHA, HINDI_C_NYA, HINDI_AC_ZA, '5', '\0'}, /* key 5 */
    {HINDI_C_TTA, HINDI_C_TTHA, HINDI_C_DDA,  HINDI_C_DDHA,  HINDI_C_NNA,HINDI_AC_DDDHA, HINDI_AC_RHA, '6','\0'},       /* key 6 */
    {HINDI_C_TA,HINDI_C_THA,HINDI_C_DA,HINDI_C_DHA,HINDI_C_NA,HINDI_C_NNNA,'7','\0'},      /* key 7 */
    {HINDI_C_PA,HINDI_C_PHA,  HINDI_C_BA,HINDI_C_BHA,HINDI_C_MA,HINDI_AC_FA,'8','\0'},    /* key 8 */
    {HINDI_C_YA, HINDI_C_RA,  HINDI_C_LA, HINDI_C_VA, HINDI_C_SHA, HINDI_AC_YYA, HINDI_C_RRA,'9', '\0'},      /* key 9 */
 #if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
 #else /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
    {'\0'},
    {'\0'},
 #endif /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
};

/* Without Nukta */
const UI_character_type default_hindimatra3_multitap_strings[MAX_MULTITAPS][45] = 
{
    {' ', HINDI_C_SSA,HINDI_C_SA,HINDI_C_HA,HINDI_S_CHANDRA_BINDU,HINDI_S_VISARGA,HINDI_S_HALANT, '0', '\0'}, /* key 0 */
    {HINDI_S_BINDU, HINDI_NG_D, '1', '\0' }, /* key 1 */																									/* KEY 1 */    
    {HINDI_DV_AA, HINDI_DV_I, HINDI_DV_II, HINDI_DV_U, HINDI_DV_UU,
     HINDI_DV_R,HINDI_IV_A, HINDI_IV_AA, HINDI_IV_I,HINDI_IV_II, HINDI_IV_U, HINDI_IV_UU, HINDI_IV_R, '2', '\0'}, /* Key 2 */
    {HINDI_DV_E, HINDI_DV_AI, HINDI_DV_CE, HINDI_DV_O, HINDI_DV_AU, 
	 HINDI_NDV_CO, HINDI_IV_E, HINDI_IV_AI, HINDI_NIV_CE, HINDI_IV_O, HINDI_IV_AU, HINDI_NIV_CO, '3', '\0'}, /* Key 3 */
    {HINDI_C_KA, HINDI_C_KHA, HINDI_C_GA, HINDI_C_GHA, HINDI_C_NGA, HINDI_AC_QA, HINDI_AC_KHHA, HINDI_AC_GHHA, '4', '\0'},  /* key 4 */
    {HINDI_C_CHA, HINDI_C_CHHA, HINDI_C_JA,  HINDI_C_JHA, HINDI_C_NYA, HINDI_AC_ZA, '5', '\0'}, /* key 5 */
    {HINDI_C_TTA, HINDI_C_TTHA, HINDI_C_DDA,  HINDI_C_DDHA,  HINDI_C_NNA,HINDI_AC_DDDHA, HINDI_AC_RHA, '6','\0'},       /* key 6 */
    {HINDI_C_TA,HINDI_C_THA,HINDI_C_DA,HINDI_C_DHA,HINDI_C_NA,HINDI_C_NNNA,'7','\0'},      /* key 7 */
    {HINDI_C_PA,HINDI_C_PHA,  HINDI_C_BA,HINDI_C_BHA,HINDI_C_MA,HINDI_AC_FA,'8','\0'},    /* key 8 */
    {HINDI_C_YA, HINDI_C_RA,  HINDI_C_LA, HINDI_C_VA, HINDI_C_SHA, HINDI_AC_YYA, HINDI_C_RRA,'9', '\0'},      /* key 9 */
 #if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
 #else /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
    {'\0'},
    {'\0'},
 #endif /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
};

const UI_character_type default_hindimatra1_multitap_strings[MAX_MULTITAPS][45] = 
{
    {' ', HINDI_C_SSA,HINDI_C_SA,HINDI_C_HA,HINDI_S_CHANDRA_BINDU,HINDI_S_VISARGA,'0','\0'}, /* key 0 */
    {HINDI_S_BINDU, HINDI_NG_D, '1','\0'}, 	/* key 1 */																				/* KEY 1 */
    {HINDI_IV_A, HINDI_IV_AA, HINDI_IV_I,HINDI_IV_II, HINDI_IV_U, HINDI_IV_UU, HINDI_IV_R, '2', '\0'}, /* Key 2 */
    {HINDI_IV_E, HINDI_IV_AI, HINDI_NIV_CE, HINDI_IV_O, HINDI_IV_AU, HINDI_NIV_CO, '3', '\0'}, /* Key 3 */
    {HINDI_C_KA, HINDI_C_KHA, HINDI_C_GA, HINDI_C_GHA, HINDI_C_NGA, HINDI_AC_QA, HINDI_AC_KHHA, HINDI_AC_GHHA, '4', '\0'},  /* key 4 */
    {HINDI_C_CHA, HINDI_C_CHHA, HINDI_C_JA,  HINDI_C_JHA, HINDI_C_NYA, HINDI_AC_ZA, '5', '\0'}, /* key 5 */
    {HINDI_C_TTA, HINDI_C_TTHA, HINDI_C_DDA,  HINDI_C_DDHA,  HINDI_C_NNA,HINDI_AC_DDDHA, HINDI_AC_RHA, '6','\0'},       /* key 6 */
    {HINDI_C_TA,HINDI_C_THA,HINDI_C_DA,HINDI_C_DHA,HINDI_C_NA,HINDI_C_NNNA,'7','\0'},      /* key 7 */
    {HINDI_C_PA,HINDI_C_PHA,  HINDI_C_BA,HINDI_C_BHA,HINDI_C_MA,HINDI_AC_FA,'8','\0'},    /* key 8 */
    {HINDI_C_YA, HINDI_C_RA,  HINDI_C_LA, HINDI_C_VA, HINDI_C_SHA, HINDI_AC_YYA, HINDI_C_RRA,'9', '\0'},      /* key 9 */
 #if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
 #else /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
    {'\0'},
    {'\0'},
 #endif /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
};
const UI_character_type default_hindimatra2_multitap_strings[MAX_MULTITAPS][45] = 
{
    {' ', HINDI_C_SSA,HINDI_C_SA,HINDI_C_HA,HINDI_S_VISARGA, '0', '\0'}, /* key 0 */
    {HINDI_S_BINDU, HINDI_NG_D, '1','\0'}, 	/* KEY 1 */
    {HINDI_IV_A, HINDI_IV_AA, HINDI_IV_I, HINDI_IV_II, HINDI_IV_U, HINDI_IV_UU, HINDI_IV_R, '2', '\0'}, /* Key 2 */
    {HINDI_IV_E, HINDI_IV_AI, HINDI_NIV_CE, HINDI_IV_O, HINDI_IV_AU, HINDI_NIV_CO, '3', '\0'}, /* Key 3 */
    {HINDI_C_KA, HINDI_C_KHA, HINDI_C_GA, HINDI_C_GHA, HINDI_C_NGA, HINDI_AC_QA, HINDI_AC_KHHA, HINDI_AC_GHHA, '4', '\0'},  /* key 4 */
    {HINDI_C_CHA, HINDI_C_CHHA, HINDI_C_JA,  HINDI_C_JHA, HINDI_C_NYA, HINDI_AC_ZA, '5', '\0'}, /* key 5 */
    {HINDI_C_TTA, HINDI_C_TTHA, HINDI_C_DDA,  HINDI_C_DDHA,  HINDI_C_NNA,HINDI_AC_DDDHA, HINDI_AC_RHA, '6','\0'},       /* key 6 */
    {HINDI_C_TA,HINDI_C_THA,HINDI_C_DA,HINDI_C_DHA,HINDI_C_NA,HINDI_C_NNNA,'7','\0'},      /* key 7 */
    {HINDI_C_PA,HINDI_C_PHA,  HINDI_C_BA,HINDI_C_BHA,HINDI_C_MA,HINDI_AC_FA,'8','\0'},    /* key 8 */
    {HINDI_C_YA, HINDI_C_RA,  HINDI_C_LA, HINDI_C_VA, HINDI_C_SHA, HINDI_AC_YYA, HINDI_C_RRA,'9', '\0'},      /* key 9 */
 #if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
 #else /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
    {'\0'},
    {'\0'},
 #endif /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
};
#else /* defined (__MMI_ZI_V7__) */
const UI_character_type default_hindi_multitap_strings[MAX_MULTITAPS][32] = 
{

    {' ', HINDI_D_ZERO, '0', '\0'}, /* key 0 */
    {HINDI_S_ABBR, HINDI_NG_D, HINDI_D_ONE, '1', '\0'}, /* key 1 */
    {HINDI_IV_A, HINDI_IV_AA, HINDI_IV_I, HINDI_IV_II, HINDI_IV_U, HINDI_IV_UU, HINDI_IV_R, HINDI_IV_E, HINDI_IV_AI, HINDI_IV_O, HINDI_IV_AU, HINDI_NIV_CO, HINDI_NIV_CE, HINDI_D_TWO, '2', '\0'},      /* key 2 */
    {HINDI_D_THREE, '3', '\0'},     /* key 3 */
    {HINDI_C_KA, HINDI_C_KHA, HINDI_C_GA, HINDI_C_GHA, HINDI_C_NGA, HINDI_C_CHA, HINDI_C_CHHA, HINDI_C_JA, HINDI_C_JHA, HINDI_C_NYA, HINDI_D_FOUR, HINDI_AC_QA, HINDI_AC_KHHA, HINDI_AC_GHHA, HINDI_AC_ZA, '4', '\0'},  /* key 4 */
    {HINDI_C_TTA, HINDI_C_TTHA, HINDI_C_DDA, HINDI_C_DDHA, HINDI_C_NNA, HINDI_D_FIVE, HINDI_AC_DDDHA, HINDI_AC_RHA, '5', '\0'}, /* key 5 */
    {HINDI_C_TA, HINDI_C_THA, HINDI_C_DA, HINDI_C_DHA, HINDI_C_NA, HINDI_D_SIX, HINDI_C_NNNA, '6', '\0'},       /* key 6 */
    {HINDI_C_PA, HINDI_C_PHA, HINDI_C_BA, HINDI_C_BHA, HINDI_C_MA, HINDI_D_SEVEN, HINDI_AC_FA, '7', '\0'},      /* key 7 */
    {HINDI_C_YA, HINDI_C_RA, HINDI_C_LA, HINDI_C_VA, HINDI_C_SHA, HINDI_C_SSA, HINDI_C_SA, HINDI_C_HA, HINDI_D_EIGHT, HINDI_AC_YYA, HINDI_C_RRA, '8', '\0'},    /* key 8 */
    {HINDI_D_NINE, '9', '\0'},      /* key 9 */
 #if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
 #else /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
    {'\0'},
    {'\0'},
 #endif /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
};

const UI_character_type default_hindimatra_multitap_strings[MAX_MULTITAPS][32] = 
{
    {' ', HINDI_D_ZERO, '0', '\0'}, /* key 0 */
    {HINDI_S_ABBR, HINDI_NG_D, HINDI_D_ONE, '1', '\0'}, /* key 1 */
    {HINDI_IV_A, HINDI_IV_AA, HINDI_IV_I, HINDI_IV_II, HINDI_IV_U, HINDI_IV_UU, HINDI_IV_R, HINDI_IV_E, HINDI_IV_AI,
     HINDI_IV_O, HINDI_IV_AU, HINDI_NIV_CO, HINDI_NIV_CE, HINDI_D_TWO, '2', '\0'},  /* key 2 */
    {HINDI_DV_AA, HINDI_DV_I, HINDI_DV_II, HINDI_DV_U, HINDI_DV_UU, HINDI_DV_R, HINDI_DV_E, HINDI_DV_AI, HINDI_DV_O, HINDI_DV_AU, HINDI_DV_CE, HINDI_NDV_CO, HINDI_D_THREE, '3', '\0'}, /* key 3 */
    {HINDI_C_KA, HINDI_C_KHA, HINDI_C_GA, HINDI_C_GHA, HINDI_C_NGA, HINDI_C_CHA, HINDI_C_CHHA, HINDI_C_JA, HINDI_C_JHA, HINDI_C_NYA, HINDI_D_FOUR, HINDI_AC_QA, HINDI_AC_KHHA, HINDI_AC_GHHA, HINDI_AC_ZA, '4', '\0'},  /* key 4 */
    {HINDI_C_TTA, HINDI_C_TTHA, HINDI_C_DDA, HINDI_C_DDHA, HINDI_C_NNA, HINDI_D_FIVE, HINDI_AC_DDDHA, HINDI_AC_RHA, '5', '\0'}, /* key 5 */
    {HINDI_C_TA, HINDI_C_THA, HINDI_C_DA, HINDI_C_DHA, HINDI_C_NA, HINDI_D_SIX, HINDI_C_NNNA, '6', '\0'},       /* key 6 */
    {HINDI_C_PA, HINDI_C_PHA, HINDI_C_BA, HINDI_C_BHA, HINDI_C_MA, HINDI_D_SEVEN, HINDI_AC_FA, '7', '\0'},      /* key 7 */
    {HINDI_C_YA, HINDI_C_RA, HINDI_C_LA, HINDI_C_VA, HINDI_C_SHA, HINDI_C_SSA, HINDI_C_SA, HINDI_C_HA, HINDI_D_EIGHT, HINDI_AC_YYA, HINDI_C_RRA, '8', '\0'},    /* key 8 */
    {HINDI_S_CHANDRA_BINDU, HINDI_S_BINDU, HINDI_S_HALANT, HINDI_S_VISARGA, HINDI_D_NINE, '9', '\0'},   /* key 9 */
 #if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
 #else /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
    {'\0'},
    {'\0'},
 #endif /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
};

const UI_character_type default_hindimatra1_multitap_strings[MAX_MULTITAPS][32] = 
{
    {' ', HINDI_D_ZERO, '0', '\0'}, /* key 0 */
    {HINDI_S_ABBR, HINDI_NG_D, HINDI_D_ONE, '1', '\0'}, /* key 1 */
    {HINDI_IV_A, HINDI_IV_AA, HINDI_IV_I, HINDI_IV_II, HINDI_IV_U, HINDI_IV_UU, HINDI_IV_R, HINDI_IV_E, HINDI_IV_AI,
     HINDI_IV_O, HINDI_IV_AU, HINDI_NIV_CO, HINDI_NIV_CE, HINDI_D_TWO, '2', '\0'},  /* key 2 */
    {HINDI_D_THREE, '3', '\0'}, /* key 3 */
    {HINDI_C_KA, HINDI_C_KHA, HINDI_C_GA, HINDI_C_GHA, HINDI_C_NGA, HINDI_C_CHA, HINDI_C_CHHA, HINDI_C_JA, HINDI_C_JHA, HINDI_C_NYA, HINDI_D_FOUR, HINDI_AC_QA, HINDI_AC_KHHA, HINDI_AC_GHHA, HINDI_AC_ZA, '4', '\0'},  /* key 4 */
    {HINDI_C_TTA, HINDI_C_TTHA, HINDI_C_DDA, HINDI_C_DDHA, HINDI_C_NNA, HINDI_D_FIVE, HINDI_AC_DDDHA, HINDI_AC_RHA, '5', '\0'}, /* key 5 */
    {HINDI_C_TA, HINDI_C_THA, HINDI_C_DA, HINDI_C_DHA, HINDI_C_NA, HINDI_D_SIX, HINDI_C_NNNA, '6', '\0'},       /* key 6 */
    {HINDI_C_PA, HINDI_C_PHA, HINDI_C_BA, HINDI_C_BHA, HINDI_C_MA, HINDI_D_SEVEN, HINDI_AC_FA, '7', '\0'},      /* key 7 */
    {HINDI_C_YA, HINDI_C_RA, HINDI_C_LA, HINDI_C_VA, HINDI_C_SHA, HINDI_C_SSA, HINDI_C_SA, HINDI_C_HA, HINDI_D_EIGHT, HINDI_AC_YYA, HINDI_C_RRA, '8', '\0'},    /* key 8 */
    {HINDI_S_CHANDRA_BINDU, HINDI_S_BINDU, HINDI_S_VISARGA, HINDI_D_NINE, '9', '\0'},   /* key 9 */
 #if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
 #else /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
    {'\0'},
    {'\0'},
 #endif /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
};
const UI_character_type default_hindimatra2_multitap_strings[MAX_MULTITAPS][32] = 
{
    {' ', HINDI_D_ZERO, '0', '\0'}, /* key 0 */
    {HINDI_S_ABBR, HINDI_NG_D, HINDI_D_ONE, '1', '\0'}, /* key 1 */
    {HINDI_IV_A, HINDI_IV_AA, HINDI_IV_I, HINDI_IV_II, HINDI_IV_U, HINDI_IV_UU, HINDI_IV_R, HINDI_IV_E, HINDI_IV_AI,
     HINDI_IV_O, HINDI_IV_AU, HINDI_NIV_CO, HINDI_NIV_CE, HINDI_D_TWO, '2', '\0'},  /* key 2 */
    {HINDI_D_THREE, '3', '\0'}, /* key 3 */
    {HINDI_C_KA, HINDI_C_KHA, HINDI_C_GA, HINDI_C_GHA, HINDI_C_NGA, HINDI_C_CHA, HINDI_C_CHHA, HINDI_C_JA, HINDI_C_JHA, HINDI_C_NYA, HINDI_D_FOUR, HINDI_AC_QA, HINDI_AC_KHHA, HINDI_AC_GHHA, HINDI_AC_ZA, '4', '\0'},  /* key 4 */
    {HINDI_C_TTA, HINDI_C_TTHA, HINDI_C_DDA, HINDI_C_DDHA, HINDI_C_NNA, HINDI_D_FIVE, HINDI_AC_DDDHA, HINDI_AC_RHA, '5', '\0'}, /* key 5 */
    {HINDI_C_TA, HINDI_C_THA, HINDI_C_DA, HINDI_C_DHA, HINDI_C_NA, HINDI_D_SIX, HINDI_C_NNNA, '6', '\0'},       /* key 6 */
    {HINDI_C_PA, HINDI_C_PHA, HINDI_C_BA, HINDI_C_BHA, HINDI_C_MA, HINDI_D_SEVEN, HINDI_AC_FA, '7', '\0'},      /* key 7 */
    {HINDI_C_YA, HINDI_C_RA, HINDI_C_LA, HINDI_C_VA, HINDI_C_SHA, HINDI_C_SSA, HINDI_C_SA, HINDI_C_HA, HINDI_D_EIGHT, HINDI_AC_YYA, HINDI_C_RRA, '8', '\0'},    /* key 8 */
    {HINDI_S_BINDU, HINDI_S_VISARGA, HINDI_D_NINE, '9', '\0'},  /* key 9 */
 #if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
 #else /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
    {'\0'},
    {'\0'},
 #endif /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
};

#endif /* defined (__MMI_ZI_V7__) */ 
/* t9 portion */
#elif defined(__MMI_T9_HINDI__)

const UI_character_type default_hindi_multitap_strings[MAX_MULTITAPS][32] = 
{

    {' ', '0', '\0'},   /* key 0 */
    {'1', '\0'},        /* key 1 */
    {HINDI_IV_A, HINDI_IV_AA, HINDI_IV_I, HINDI_IV_II, HINDI_IV_U, HINDI_IV_UU, HINDI_IV_R, '2', '\0'}, /* key 2 */
    {HINDI_IV_E, HINDI_IV_AI, HINDI_NIV_CE, HINDI_IV_O, HINDI_IV_AU, HINDI_NIV_CO, '3', '\0'},  /* key 3 */
    {HINDI_C_KA, HINDI_C_KHA, HINDI_C_GA, HINDI_C_GHA, HINDI_C_NGA, HINDI_AC_QA, HINDI_AC_KHHA, HINDI_AC_GHHA, '4', '\0'},      /* key 4 */
    {HINDI_C_CHA, HINDI_C_CHHA, HINDI_C_JA, HINDI_C_JHA, HINDI_C_NYA, HINDI_AC_ZA, '5', '\0'},  /* key 5 */
    {HINDI_C_TTA, HINDI_C_TTHA, HINDI_C_DDA, HINDI_C_DDHA, HINDI_C_NNA, HINDI_AC_DDDHA, HINDI_AC_RHA, '6', '\0'},       /* key 6 */
    {HINDI_C_TA, HINDI_C_THA, HINDI_C_DA, HINDI_C_DHA, HINDI_C_NA, HINDI_C_NNNA, '7', '\0'},    /* key 7 */
    {HINDI_C_PA, HINDI_C_PHA, HINDI_C_BA, HINDI_C_BHA, HINDI_C_MA, HINDI_AC_FA, '8', '\0'},     /* key 8 */
    {HINDI_C_YA, HINDI_C_RA, HINDI_C_LA, HINDI_C_VA, HINDI_C_SHA, HINDI_C_SSA, HINDI_C_SA, HINDI_C_HA, HINDI_C_LLA, HINDI_C_RRA, HINDI_C_LLLA, HINDI_AC_YYA, '9', '\0'},        /* key 9 */
#if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
#else /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
    {'\0'},
    {'\0'},
#endif /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
};
const UI_character_type default_hindimatra_multitap_strings[MAX_MULTITAPS][32] = 
{
    {' ', '0', '\0'},   /* key 0 */
    {HINDI_S_CHANDRA_BINDU, HINDI_S_BINDU, HINDI_S_VISARGA, HINDI_S_HALANT, '1', '\0'}, /* key 1 */
    {HINDI_DV_AA, HINDI_DV_I, HINDI_DV_II, HINDI_DV_U, HINDI_DV_UU, HINDI_DV_R, HINDI_IV_A, HINDI_IV_AA, HINDI_IV_I, HINDI_IV_II, HINDI_IV_U, HINDI_IV_UU, HINDI_IV_R, '2', '\0'},      /* key 2 */
    {HINDI_DV_E, HINDI_DV_AI, HINDI_DV_CE, HINDI_DV_O, HINDI_DV_AU, HINDI_NDV_CO, HINDI_IV_E, HINDI_IV_AI, HINDI_NIV_CE, HINDI_IV_O, HINDI_IV_AU, HINDI_NIV_CO, '3', '\0'},     /* key 3 */
    {HINDI_C_KA, HINDI_C_KHA, HINDI_C_GA, HINDI_C_GHA, HINDI_C_NGA, HINDI_AC_QA, HINDI_AC_KHHA, HINDI_AC_GHHA, '4', '\0'},      /* key 4 */
    {HINDI_C_CHA, HINDI_C_CHHA, HINDI_C_JA, HINDI_C_JHA, HINDI_C_NYA, HINDI_AC_ZA, '5', '\0'},  /* key 5 */
    {HINDI_C_TTA, HINDI_C_TTHA, HINDI_C_DDA, HINDI_C_DDHA, HINDI_C_NNA, HINDI_AC_DDDHA, HINDI_AC_RHA, '6', '\0'},       /* key 6 */
    {HINDI_C_TA, HINDI_C_THA, HINDI_C_DA, HINDI_C_DHA, HINDI_C_NA, HINDI_C_NNNA, '7', '\0'},    /* key 7 */
    {HINDI_C_PA, HINDI_C_PHA, HINDI_C_BA, HINDI_C_BHA, HINDI_C_MA, HINDI_AC_FA, '8', '\0'},     /* key 8 */
    {HINDI_C_YA, HINDI_C_RA, HINDI_C_LA, HINDI_C_VA, HINDI_C_SHA, HINDI_C_SSA, HINDI_C_SA, HINDI_C_HA, HINDI_C_LLA, HINDI_C_RRA, HINDI_C_LLLA, HINDI_AC_YYA, '9', '\0'},        /* key 9 */
#if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
#else /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
    {'\0'},
    {'\0'},
#endif /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
};
const UI_character_type default_hindimatra1_multitap_strings[MAX_MULTITAPS][32] = 
{
    {' ', '0', '\0'},   /* key 0 */
    {HINDI_S_CHANDRA_BINDU, HINDI_S_BINDU, HINDI_S_VISARGA, '1', '\0'}, /* key 1 */
    {HINDI_IV_A, HINDI_IV_AA, HINDI_IV_I, HINDI_IV_II, HINDI_IV_U, HINDI_IV_UU, HINDI_IV_R, '2', '\0'}, /* key 2 */
    {HINDI_IV_E, HINDI_IV_AI, HINDI_NIV_CE, HINDI_IV_O, HINDI_IV_AU, HINDI_NIV_CO, '3', '\0'},  /* key 3 */
    {HINDI_C_KA, HINDI_C_KHA, HINDI_C_GA, HINDI_C_GHA, HINDI_C_NGA, HINDI_AC_QA, HINDI_AC_KHHA, HINDI_AC_GHHA, '4', '\0'},      /* key 4 */
    {HINDI_C_CHA, HINDI_C_CHHA, HINDI_C_JA, HINDI_C_JHA, HINDI_C_NYA, HINDI_AC_ZA, '5', '\0'},  /* key 5 */
    {HINDI_C_TTA, HINDI_C_TTHA, HINDI_C_DDA, HINDI_C_DDHA, HINDI_C_NNA, HINDI_AC_DDDHA, HINDI_AC_RHA, '6', '\0'},       /* key 6 */
    {HINDI_C_TA, HINDI_C_THA, HINDI_C_DA, HINDI_C_DHA, HINDI_C_NA, HINDI_C_NNNA, '7', '\0'},    /* key 7 */
    {HINDI_C_PA, HINDI_C_PHA, HINDI_C_BA, HINDI_C_BHA, HINDI_C_MA, HINDI_AC_FA, '8', '\0'},     /* key 8 */
    {HINDI_C_YA, HINDI_C_RA, HINDI_C_LA, HINDI_C_VA, HINDI_C_SHA, HINDI_C_SSA, HINDI_C_SA, HINDI_C_HA, HINDI_C_LLA,
     HINDI_C_RRA, HINDI_C_LLLA, HINDI_AC_YYA, '9', '\0'},
 #if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
 #else /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
    {'\0'},
    {'\0'},
 #endif /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
};

const UI_character_type default_hindimatra2_multitap_strings[MAX_MULTITAPS][32] = 
{
    {' ', '0', '\0'},                               /* key 0 */
    {HINDI_S_BINDU, HINDI_S_VISARGA, '1', '\0'},    /* key 1 */
    {HINDI_IV_A, HINDI_IV_AA, HINDI_IV_I, HINDI_IV_II, HINDI_IV_U, HINDI_IV_UU, HINDI_IV_R, '2', '\0'}, /* key 2 */
    {HINDI_IV_E, HINDI_IV_AI, HINDI_NIV_CE, HINDI_IV_O, HINDI_IV_AU, HINDI_NIV_CO, '3', '\0'},  /* key 3 */
    {HINDI_C_KA, HINDI_C_KHA, HINDI_C_GA, HINDI_C_GHA, HINDI_C_NGA, HINDI_AC_QA, HINDI_AC_KHHA, HINDI_AC_GHHA, '4', '\0'},      /* key 4 */
    {HINDI_C_CHA, HINDI_C_CHHA, HINDI_C_JA, HINDI_C_JHA, HINDI_C_NYA, HINDI_AC_ZA, '5', '\0'},  /* key 5 */
    {HINDI_C_TTA, HINDI_C_TTHA, HINDI_C_DDA, HINDI_C_DDHA, HINDI_C_NNA, HINDI_AC_DDDHA, HINDI_AC_RHA, '6', '\0'},       /* key 6 */
    {HINDI_C_TA, HINDI_C_THA, HINDI_C_DA, HINDI_C_DHA, HINDI_C_NA, HINDI_C_NNNA, '7', '\0'},    /* key 7 */
    {HINDI_C_PA, HINDI_C_PHA, HINDI_C_BA, HINDI_C_BHA, HINDI_C_MA, HINDI_AC_FA, '8', '\0'},     /* key 8 */
    {HINDI_C_YA, HINDI_C_RA, HINDI_C_LA, HINDI_C_VA, HINDI_C_SHA, HINDI_C_SSA, HINDI_C_SA, HINDI_C_HA, HINDI_C_LLA, HINDI_C_RRA, HINDI_C_LLLA, HINDI_AC_YYA, '9', '\0'},        /* key 9 */
#if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
#else /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
    {'\0'},
    {'\0'},
#endif /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
};

#else 

const UI_character_type default_hindi_multitap_strings[MAX_MULTITAPS][32] = 
{
    {' ', '.', HINDI_NG_D, HINDI_S_OM, HINDI_NG_DD, HINDI_D_ZERO, '0', '\0'}, /* key 0 */
    {HINDI_C_KA, HINDI_C_KHA, HINDI_C_GA, HINDI_C_GHA, HINDI_C_NGA, HINDI_AC_QA, HINDI_AC_KHHA, HINDI_AC_GHHA, HINDI_D_ONE, '1', '\0'},      /* key 1 */
    {HINDI_C_CHA, HINDI_C_CHHA, HINDI_C_JA, HINDI_C_JHA, HINDI_C_NYA, HINDI_AC_ZA, HINDI_D_TWO, '2', '\0'},  /* key 2 */
    {HINDI_C_TTA, HINDI_C_TTHA, HINDI_C_DDA, HINDI_C_DDHA, HINDI_C_NNA, HINDI_AC_DDDHA, HINDI_AC_RHA, HINDI_D_THREE, '3', '\0'},       /* key 3 */
    {HINDI_C_TA, HINDI_C_THA, HINDI_C_DA, HINDI_C_DHA, HINDI_C_NA, HINDI_C_NNNA, HINDI_D_FOUR, '4', '\0'},    /* key 4 */
    {HINDI_C_PA, HINDI_C_PHA, HINDI_C_BA, HINDI_C_BHA, HINDI_C_MA, HINDI_AC_FA, HINDI_D_FIVE, '5', '\0'},     /* key 5 */
    {HINDI_C_YA, HINDI_C_RA, HINDI_C_LA, HINDI_C_LLA, HINDI_C_VA, HINDI_AC_YYA, HINDI_C_RRA, HINDI_C_LLLA, HINDI_D_SIX, '6', '\0'},  /* key 6 */
    {HINDI_C_SHA, HINDI_C_SSA, HINDI_C_SA, HINDI_C_HA, HINDI_D_SEVEN, '7', '\0'},      /* key 7 */
    {HINDI_IV_A, HINDI_IV_AA, HINDI_IV_I, HINDI_IV_II, HINDI_IV_U, HINDI_IV_UU, HINDI_IV_R, HINDI_IV_E, HINDI_IV_AI, HINDI_NIV_CE, HINDI_IV_O, HINDI_IV_AU, HINDI_NIV_CO, HINDI_D_EIGHT, '8', '\0'},   /* key 8 */
    {HINDI_D_NINE, '9', '\0'},    /* key 9 */
#if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
#else /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
    {'\0'},
    {'\0'},
#endif /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
};

const UI_character_type default_hindimatra_multitap_strings[MAX_MULTITAPS][32] = 
{
    {' ', HINDI_S_HALANT, HINDI_S_CHANDRA_BINDU, HINDI_S_BINDU, HINDI_S_VISARGA, HINDI_S_OM, HINDI_NG_D, HINDI_S_ABBR, HINDI_NG_DD, '.', HINDI_D_ZERO, '0', '\0'},    /* key 0 */
    {HINDI_C_KA, HINDI_C_KHA, HINDI_C_GA, HINDI_C_GHA, HINDI_C_NGA, HINDI_AC_QA, HINDI_AC_KHHA, HINDI_AC_GHHA, HINDI_D_ONE, '1', '\0'},      /* key 1 */
    {HINDI_C_CHA, HINDI_C_CHHA, HINDI_C_JA, HINDI_C_JHA, HINDI_C_NYA, HINDI_AC_ZA, HINDI_D_TWO, '2', '\0'},  /* key 2 */
    {HINDI_C_TTA, HINDI_C_TTHA, HINDI_C_DDA, HINDI_C_DDHA, HINDI_C_NNA, HINDI_AC_DDDHA, HINDI_AC_RHA, HINDI_D_THREE, '3', '\0'},       /* key 3 */
    {HINDI_C_TA, HINDI_C_THA, HINDI_C_DA, HINDI_C_DHA, HINDI_C_NA, HINDI_C_NNNA, HINDI_D_FOUR, '4', '\0'},    /* key 4 */
    {HINDI_C_PA, HINDI_C_PHA, HINDI_C_BA, HINDI_C_BHA, HINDI_C_MA, HINDI_AC_FA, HINDI_D_FIVE, '5', '\0'},     /* key 5 */
    {HINDI_C_YA, HINDI_C_RA, HINDI_C_LA, HINDI_C_LLA, HINDI_C_VA, HINDI_AC_YYA, HINDI_C_RRA, HINDI_C_LLLA, HINDI_D_SIX, '6', '\0'},  /* key 6 */
    {HINDI_C_SHA, HINDI_C_SSA, HINDI_C_SA, HINDI_C_HA, HINDI_D_SEVEN, '7', '\0'},      /* key 7 */
    {HINDI_IV_A, HINDI_IV_AA, HINDI_IV_I, HINDI_IV_II, HINDI_IV_U, HINDI_IV_UU, HINDI_IV_R, HINDI_IV_E, HINDI_IV_AI, HINDI_NIV_CE, HINDI_IV_O, HINDI_IV_AU, HINDI_NIV_CO, HINDI_D_EIGHT, '8', '\0'},   /* key 8 */
    {HINDI_DV_AA, HINDI_DV_I, HINDI_DV_II, HINDI_DV_U, HINDI_DV_UU, HINDI_DV_R, HINDI_DV_E, HINDI_DV_AI, HINDI_DV_CE, HINDI_DV_O, HINDI_DV_AU, HINDI_NDV_CO, HINDI_D_NINE, '9', '\0'},        /* key 9 */
#if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
#else /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
    {'\0'},
    {'\0'},
#endif /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
};

const UI_character_type default_hindimatra1_multitap_strings[MAX_MULTITAPS][32] = 
{
    {' ', HINDI_S_CHANDRA_BINDU, HINDI_S_BINDU, HINDI_S_VISARGA, HINDI_S_OM, HINDI_NG_D, HINDI_S_ABBR, HINDI_NG_DD, '.', HINDI_D_ZERO, '0', '\0'},    /* key 0 */
    {HINDI_C_KA, HINDI_C_KHA, HINDI_C_GA, HINDI_C_GHA, HINDI_C_NGA, HINDI_AC_QA, HINDI_AC_KHHA, HINDI_AC_GHHA, HINDI_D_ONE, '1', '\0'},      /* key 1 */
    {HINDI_C_CHA, HINDI_C_CHHA, HINDI_C_JA, HINDI_C_JHA, HINDI_C_NYA, HINDI_AC_ZA, HINDI_D_TWO, '2', '\0'},  /* key 2 */
    {HINDI_C_TTA, HINDI_C_TTHA, HINDI_C_DDA, HINDI_C_DDHA, HINDI_C_NNA, HINDI_AC_DDDHA, HINDI_AC_RHA, HINDI_D_THREE, '3', '\0'},       /* key 3 */
    {HINDI_C_TA, HINDI_C_THA, HINDI_C_DA, HINDI_C_DHA, HINDI_C_NA, HINDI_C_NNNA, HINDI_D_FOUR, '4', '\0'},    /* key 4 */
    {HINDI_C_PA, HINDI_C_PHA, HINDI_C_BA, HINDI_C_BHA, HINDI_C_MA, HINDI_AC_FA, HINDI_D_FIVE, '5', '\0'},     /* key 5 */
    {HINDI_C_YA, HINDI_C_RA, HINDI_C_LA, HINDI_C_LLA, HINDI_C_VA, HINDI_AC_YYA, HINDI_C_RRA, HINDI_C_LLLA, HINDI_D_SIX, '6', '\0'},  /* key 6 */
    {HINDI_C_SHA, HINDI_C_SSA, HINDI_C_SA, HINDI_C_HA, HINDI_D_SEVEN, '7', '\0'},      /* key 7 */
    {HINDI_IV_A, HINDI_IV_AA, HINDI_IV_I, HINDI_IV_II, HINDI_IV_U, HINDI_IV_UU, HINDI_IV_R, HINDI_IV_E, HINDI_IV_AI, HINDI_NIV_CE, HINDI_IV_O, HINDI_IV_AU, HINDI_NIV_CO, HINDI_D_EIGHT, '8', '\0'},   /* key 8 */
    {HINDI_D_NINE, '9', '\0'},    /* key 9 */
#if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
#else /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
    {'\0'},
    {'\0'},
#endif /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
};

const UI_character_type default_hindimatra2_multitap_strings[MAX_MULTITAPS][32] = 
{
    {' ', '.', HINDI_S_BINDU, HINDI_S_VISARGA, HINDI_S_OM, HINDI_NG_D, HINDI_S_ABBR, HINDI_NG_DD, HINDI_D_ZERO, '0', '\0'},   /* key 0 */
    {HINDI_C_KA, HINDI_C_KHA, HINDI_C_GA, HINDI_C_GHA, HINDI_C_NGA, HINDI_AC_QA, HINDI_AC_KHHA, HINDI_AC_GHHA, HINDI_D_ONE, '1', '\0'},      /* key 1 */
    {HINDI_C_CHA, HINDI_C_CHHA, HINDI_C_JA, HINDI_C_JHA, HINDI_C_NYA, HINDI_AC_ZA, HINDI_D_TWO, '2', '\0'},  /* key 2 */
    {HINDI_C_TTA, HINDI_C_TTHA, HINDI_C_DDA, HINDI_C_DDHA, HINDI_C_NNA, HINDI_AC_DDDHA, HINDI_AC_RHA, HINDI_D_THREE, '3', '\0'},       /* key 3 */
    {HINDI_C_TA, HINDI_C_THA, HINDI_C_DA, HINDI_C_DHA, HINDI_C_NA, HINDI_C_NNNA, HINDI_D_FOUR, '4', '\0'},    /* key 4 */
    {HINDI_C_PA, HINDI_C_PHA, HINDI_C_BA, HINDI_C_BHA, HINDI_C_MA, HINDI_AC_FA, HINDI_D_FIVE, '5', '\0'},     /* key 5 */
    {HINDI_C_YA, HINDI_C_RA, HINDI_C_LA, HINDI_C_LLA, HINDI_C_VA, HINDI_AC_YYA, HINDI_C_RRA, HINDI_C_LLLA, HINDI_D_SIX, '6', '\0'},  /* key 6 */
    {HINDI_C_SHA, HINDI_C_SSA, HINDI_C_SA, HINDI_C_HA, HINDI_D_SEVEN, '7', '\0'},      /* key 7 */
    {HINDI_IV_A, HINDI_IV_AA, HINDI_IV_I, HINDI_IV_II, HINDI_IV_U, HINDI_IV_UU, HINDI_IV_R, HINDI_IV_E, HINDI_IV_AI, HINDI_NIV_CE, HINDI_IV_O, HINDI_IV_AU, HINDI_NIV_CO, HINDI_D_EIGHT, '8', '\0'},   /* key 8 */
    {HINDI_D_NINE, '9', '\0'},    /* key 9 */
#if !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__)
#else /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
    {'\0'},
    {'\0'},
#endif /* !defined(__MMI_MULTITAP_FOR_STAR_AND_POUND__) */ 
};

#endif 

#if 0
/*****************************************************************************
 * FUNCTION
 *  get_hindi_multitap_string
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  UI_string_type
 *****************************************************************************/
UI_string_type get_hindi_multitap_string(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_character_type previous_char = 0;
   mmi_imc_message_struct imc_msg;
    UI_character_type string_buf[10] = {0}; ///TODO: define an value for the string length
	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

	/* get previous character but skip highlight and multitap. */
    imc_msg.message_id = MMI_IMC_MESSAGE_GET_STRING_BEFORE_CANDIDATE;
    imc_msg.param_0 = 1;
    imc_msg.param_1 = (U32)string_buf;
	
    mmi_imc_send_message(&imc_msg);
	
    previous_char = string_buf[0];

    if (previous_char)
    {
        if ((previous_char >= HINDI_C_KA && previous_char <= HINDI_C_HA) || (previous_char >= HINDI_AC_QA &&
            previous_char <= HINDI_AC_YYA))
        {
        #if defined (__MMI_ZI_V7__) && defined (__MMI_ZI_HINDI__)
            switch(previous_char)
            {
                case HINDI_C_KA:
                case HINDI_C_KHA:
                case HINDI_C_GA:
                case HINDI_C_JA:
                case HINDI_C_DDA:
                case HINDI_C_DDHA:
                case HINDI_C_NA:
                case HINDI_C_PHA:
                case HINDI_C_YA:
                case HINDI_C_RA:
                case HINDI_C_LLA:
                    return (UI_character_type*)default_hindimatra_multitap_strings[index];
                    break;
                default:
                    return (UI_character_type*)default_hindimatra3_multitap_strings[index];
            
            }
        #else /* defined (__MMI_ZI_V7__) && defined (__MMI_ZI_HINDI__) */ 
                return (UI_character_type*)default_hindimatra_multitap_strings[index];
        #endif /* defined (__MMI_ZI_V7__) && defined (__MMI_ZI_HINDI__) */ 
            
        }
        else
        {
            switch (previous_char)
            {
                case (HINDI_DV_AA):
                case (HINDI_DV_U):
                case (HINDI_DV_UU):
                case (HINDI_DV_R):
                case (HINDI_IV_A):
                case (HINDI_IV_AA):
                case (HINDI_IV_I):
                case (HINDI_IV_U):
                case (HINDI_IV_UU):
                case (HINDI_IV_R):
                case (HINDI_IV_E):
                {
                    return (UI_character_type*)default_hindimatra1_multitap_strings[index];
                    
                    break;
                }
                case (HINDI_DV_I):
                case (HINDI_DV_II):
                case (HINDI_DV_CE):
                case (HINDI_DV_E):
                case (HINDI_DV_AI):
                case (HINDI_NDV_CO):
                case (HINDI_DV_O):
                case (HINDI_DV_AU):
                case (HINDI_IV_II):
                case (HINDI_NIV_CE):
                case (HINDI_IV_AI):
                case (HINDI_NIV_CO):
                case (HINDI_IV_O):
                case (HINDI_IV_AU):
                {
                       return (UI_character_type*)default_hindimatra2_multitap_strings[index];
                    break;
                }
                default:
                    return (UI_character_type*)default_hindi_multitap_strings[index];
            }
        }

    }
    else
    {
        return (UI_character_type*)default_hindi_multitap_strings[index];
    }

}


/*****************************************************************************
 * FUNCTION
 *  get_hindi_single_line_multitap_string
 * DESCRIPTION
 *  
 * PARAMETERS
 *  i       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
UI_character_type *get_hindi_single_line_multitap_string(S32 i)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UI_character_type previous_char = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
		previous_char = gui_single_line_current_character(&MMI_singleline_inputbox);
    /* For performance improval */
    if (previous_char)
    {
        if ((previous_char >= HINDI_C_KA && previous_char <= HINDI_C_HA) || (previous_char >= HINDI_AC_QA &&
            previous_char <= HINDI_AC_YYA))
        {
        #if defined (__MMI_ZI_V7__) && defined (__MMI_ZI_HINDI__)
            switch(previous_char)
            {
                case HINDI_C_KA:
                case HINDI_C_KHA:
                case HINDI_C_GA:
                case HINDI_C_JA:
                case HINDI_C_DDA:
                case HINDI_C_DDHA:
                case HINDI_C_NA:
                case HINDI_C_PHA:
                case HINDI_C_YA:
                case HINDI_C_RA:
                case HINDI_C_LLA:
                    return ((UI_character_type*) default_hindimatra_multitap_strings[i]);
                    break;
                
                default:
                    return ((UI_character_type*) default_hindimatra3_multitap_strings[i]);
            
            }
        #else /* defined (__MMI_ZI_V7__) && defined (__MMI_ZI_HINDI__) */ 
            return ((UI_character_type*) default_hindimatra_multitap_strings[i]);
        #endif /* defined (__MMI_ZI_V7__) && defined (__MMI_ZI_HINDI__) */ 
        }
        else
        {
            switch (previous_char)
            {
                case (HINDI_DV_AA):
                case (HINDI_DV_U):
                case (HINDI_DV_UU):
                case (HINDI_DV_R):
                case (HINDI_IV_A):
                case (HINDI_IV_AA):
                case (HINDI_IV_I):
                case (HINDI_IV_U):
                case (HINDI_IV_UU):
                case (HINDI_IV_R):
                case (HINDI_IV_E):
                {
                    return ((UI_character_type*) default_hindimatra1_multitap_strings[i]);
                    break;
                }
                case (HINDI_DV_I):
                case (HINDI_DV_II):
                case (HINDI_DV_CE):
                case (HINDI_DV_E):
                case (HINDI_DV_AI):
                case (HINDI_NDV_CO):
                case (HINDI_DV_O):
                case (HINDI_DV_AU):
                case (HINDI_IV_II):
                case (HINDI_NIV_CE):
                case (HINDI_IV_AI):
                case (HINDI_NIV_CO):
                case (HINDI_IV_O):
                case (HINDI_IV_AU):
                {
                    return ((UI_character_type*) default_hindimatra2_multitap_strings[i]);
                    break;
                }
                default:
                {
                    return ((UI_character_type*) default_hindi_multitap_strings[i]);
                }

            }
        }

    }
    else
    {
        return ((UI_character_type*) default_hindi_multitap_strings[i]);
    }

}
#endif
#endif /* defined (__MMI_MULTITAP_HINDI__) */ /* #if defined (__MMI_MULTITAP_HINDI__)   */

const U8 g_hindi_character_type_array[] = 
{
    HF_INVALID,
    SIGN,       /* S_CHANDRA_BINDU=0x0901, */
    SIGN,       /* S_BINDU=0x902, */
    SIGN,       /* S_VISARGA=0x903, */
    INDVOW,     /* NIV_A=0x904, */
    INDVOW,     /* IV_A=0x905, */
    INDVOW,     /* IV_AA=0x906, */
    INDVOW,     /* IV_I=0x907, */
    INDVOW,     /* IV_II=0x908, */
    INDVOW,     /* IV_U=0x909, */
    INDVOW,     /* IV_UU=0x90a, */
    INDVOW,     /* IV_R=0x90b, */
    INDVOW,     /* NIV_L=0x90c, */
    INDVOW,     /* NIV_CE=0x90d, */
    INDVOW,     /* NIV_SE=0x90e, */
    INDVOW,     /* IV_E=0x90f, */
    INDVOW,     /* IV_AI=0x910, */
    INDVOW,     /* NIV_CO=0x911, */
    INDVOW,     /* NIV_SO=0x912, */
    INDVOW,     /* IV_O=0x913, */
    INDVOW,     /* IV_AU=0x914, */
    CON,        /* C_KA=0x915, */
    CON,        /* C_KHA=0x916, */
    CON,        /* C_GA=0x917, */
    CON,        /* C_GHA=0x918, */
    CON,        /* C_NGA=0x919, */
    CON,        /* C_CHA=0x91a, */
    CON,        /* C_CHHA=0x91b, */
    CON,        /* C_JA=0x91c, */
    CON,        /* C_JHA=0x91d, */
    CON,        /* C_NYA=0x91e, */
    CON,        /* C_TTA=0x91f, */
    CON,        /* C_TTHA=0x920, */
    CON,        /* C_DDA=0x921, */
    CON,        /* C_DDHA=0x922, */
    CON,        /* C_NNA=0x923, */
    CON,        /* C_TA=0x924, */
    CON,        /* C_THA=0x925, */
    CON,        /* C_DA=0x926, */
    CON,        /* C_DHA=0x927, */
    CON,        /* C_NA=0x928, */
    CON,        /* C_NNNA=0x929, */
    CON,        /* C_PA=0x92a, */
    CON,        /* C_PHA=0x92b, */
    CON,        /* C_BA=0x92c, */
    CON,        /* C_BHA=0x92d, */
    CON,        /* C_MA=0x92e, */
    CON,        /* C_YA=0x92f, */
    CON,        /* C_RA=0x930, */
    CON,        /* C_RRA=0x931, */
    CON,        /* C_LA=0x932, */
    CON,        /* C_LLA=0x933, */
    CON,        /* C_LLLA=0x934, */
    CON,        /* C_VA=0x935, */
    CON,        /* C_SHA=0x936, */
    CON,        /* C_SSA=0x937, */
    CON,        /* C_SA=0x938, */
    CON,        /* C_HA=0x939, */
    HF_INVALID, /* N_BLANK1=0x93a, */
    HF_INVALID, /* N_BLANK2=0x93b, */
    SIGN,       /* S_NUKTA=0x93c, */
    SIGN,       /* NS_AVAGRAHA=0x93d, */
    DEPVOW,     /* DV_AA=0x93e, */
    DEPVOW,     /* DV_I=0x93F, */
    DEPVOW,     /* DV_II=0x940, */
    DEPVOW,     /* DV_U=0x941, */
    DEPVOW,     /* DV_UU=0x942, */
    DEPVOW,     /* DV_R=0x943, */
    DEPVOW,     /* NDV_RR=0x944, */
    DEPVOW,     /* DV_CE=0x945, */
    DEPVOW,     /* NDV_E=0x946, */
    DEPVOW,     /* DV_E=0x947, */
    DEPVOW,     /* DV_AI=0x948, */
    DEPVOW,     /* NDV_CO=0x949, */
    DEPVOW,     /* NDV_O=0x94a, */
    DEPVOW,     /* DV_O=0x94b, */
    DEPVOW,     /* DV_AU=0x94c, */
    HALANT,     /* S_HALANT=0x94d, */
    HF_INVALID, /* NS_RESERVED1=0x94e, */
    HF_INVALID, /* NS_RESERVED2=0x94f, */
 
    INDVOW,     /* S_OM=0x950, */
 
    SIGN,       /* NS_UDATTA=0x951, */
    SIGN,       /* NS_ANUDATTA=0x952, */
    SIGN,       /* NS_GACCENT=0x953, */
    SIGN,       /* NS_AACCENT=0x954, */
    HF_INVALID, /* N_BLANK5=0x955, */
    HF_INVALID, /* N_BLANK6=0x956, */
    HF_INVALID, /* N_BLANK7=0x957, */
    CON,        /* AC_QA=0x958, */
    CON,        /* AC_KHHA=0x959, */
    CON,        /* AC_GHHA=0x95a, */
    CON,        /* AC_ZA=0x95b, */
    CON,        /* AC_DDDHA=0x95c, */
    CON,        /* AC_RHA=0x95d, */
    CON,        /* AC_FA=0x95e, */
    CON,        /* AC_YYA=0x95f, */
    HF_INVALID, /* NG_RR=0x960, */
    HF_INVALID, /* NG_LL=0x961, */
    HF_INVALID, /* NG_L=0x962, */
    HF_INVALID, /* NG_VLL=0x963, */
 
    INDVOW,     /* NG_D=0x964, */
    INDVOW,     /* NG_DD=0x965, */
 
    NUM,        /* D_ZERO=0x966, */
    NUM,        /* D_ONE=0x967, */
    NUM,        /* D_TWO=0x968, */
    NUM,        /* D_THREE=0x969, */
    NUM,        /* D_FOUR=0x96a, */
    NUM,        /* D_FIVE=0x96b, */
    NUM,        /* D_SIX=0x96c, */
    NUM,        /* D_SEVEN=0x96d, */
    NUM,        /* D_EIGHT=0x96e, */
    NUM,        /* D_NINE=0x96f, */
 
    INDVOW      /* S_ABBR=0x970 */
 
};
const U8 g_hindi_character_sign_array[] = 
{
    FALSE,
    TRUE,   /* S_CHANDRA_BINDU=0x0901, */
    TRUE,   /* S_BINDU=0x902, */
 
    TRUE,   /* S_VISARGA=0x903, */
 
    FALSE,  /* NIV_A=0x904, */
    FALSE,  /* IV_A=0x905, */
    FALSE,  /* IV_AA=0x906, */
    FALSE,  /* IV_I=0x907, */
    FALSE,  /* IV_II=0x908, */
    FALSE,  /* IV_U=0x909, */
    FALSE,  /* IV_UU=0x90a, */
    FALSE,  /* IV_R=0x90b, */
    FALSE,  /* NIV_L=0x90c, */
    FALSE,  /* NIV_CE=0x90d, */
    FALSE,  /* NIV_SE=0x90e, */
    FALSE,  /* IV_E=0x90f, */
    FALSE,  /* IV_AI=0x910, */
    FALSE,  /* NIV_CO=0x911, */
    FALSE,  /* NIV_SO=0x912, */
    FALSE,  /* IV_O=0x913, */
    FALSE,  /* IV_AU=0x914, */
    FALSE,  /* C_KA=0x915, */
    FALSE,  /* C_KHA=0x916, */
    FALSE,  /* C_GA=0x917, */
    FALSE,  /* C_GHA=0x918, */
    FALSE,  /* C_NGA=0x919, */
    FALSE,  /* C_CHA=0x91a, */
    FALSE,  /* C_CHHA=0x91b, */
    FALSE,  /* C_JA=0x91c, */
    FALSE,  /* C_JHA=0x91d, */
    FALSE,  /* C_NYA=0x91e, */
    FALSE,  /* C_TTA=0x91f, */
    FALSE,  /* C_TTHA=0x920, */
    FALSE,  /* C_DDA=0x921, */
    FALSE,  /* C_DDHA=0x922, */
    FALSE,  /* C_NNA=0x923, */
    FALSE,  /* C_TA=0x924, */
    FALSE,  /* C_THA=0x925, */
    FALSE,  /* C_DA=0x926, */
    FALSE,  /* C_DHA=0x927, */
    FALSE,  /* C_NA=0x928, */
    FALSE,  /* C_NNNA=0x929, */
    FALSE,  /* C_PA=0x92a, */
    FALSE,  /* C_PHA=0x92b, */
    FALSE,  /* C_BA=0x92c, */
    FALSE,  /* C_BHA=0x92d, */
    FALSE,  /* C_MA=0x92e, */
    FALSE,  /* C_YA=0x92f, */
    FALSE,  /* C_RA=0x930, */
    FALSE,  /* C_RRA=0x931, */
    FALSE,  /* C_LA=0x932, */
    FALSE,  /* C_LLA=0x933, */
    FALSE,  /* C_LLLA=0x934, */
    FALSE,  /* C_VA=0x935, */
    FALSE,  /* C_SHA=0x936, */
    FALSE,  /* C_SSA=0x937, */
    FALSE,  /* C_SA=0x938, */
    FALSE,  /* C_HA=0x939, */
    FALSE,  /* N_BLANK1=0x93a, */
    FALSE,  /* N_BLANK2=0x93b, */
    TRUE,   /* S_NUKTA=0x93c, */
    FALSE,  /* NS_AVAGRAHA=0x93d, */
    FALSE,  /* DV_AA=0x93e, */
    FALSE,  /* DV_I=0x93F, */
    FALSE,  /* DV_II=0x940, */
    TRUE,   /* DV_U=0x941, */
    TRUE,   /* DV_UU=0x942, */
    TRUE,   /* DV_R=0x943, */
    FALSE,  /* NDV_RR=0x944, */
    TRUE,   /* DV_CE=0x945, */
    FALSE,  /* NDV_E=0x946, */
    TRUE,   /* DV_E=0x947, */
    TRUE,   /* DV_AI=0x948, */
    FALSE,  /* NDV_CO=0x949, */
    FALSE,  /* NDV_O=0x94a, */
    FALSE,  /* DV_O=0x94b, */
    FALSE,  /* DV_AU=0x94c, */
    TRUE,   /* S_HALANT=0x94d, */
    FALSE,  /* NS_RESERVED1=0x94e, */
    FALSE,  /* NS_RESERVED2=0x94f, */
    FALSE,  /* S_OM=0x950, */
    FALSE,  /* NS_UDATTA=0x951, */
    FALSE,  /* NS_ANUDATTA=0x952, */
    FALSE,  /* NS_GACCENT=0x953, */
    FALSE,  /* NS_AACCENT=0x954, */
    FALSE,  /* N_BLANK5=0x955, */
    FALSE,  /* N_BLANK6=0x956, */
    FALSE,  /* N_BLANK7=0x957, */
    FALSE,  /* AC_QA=0x958, */
    FALSE,  /* AC_KHHA=0x959, */
    FALSE,  /* AC_GHHA=0x95a, */
    FALSE,  /* AC_ZA=0x95b, */
    FALSE,  /* AC_DDDHA=0x95c, */
    FALSE,  /* AC_RHA=0x95d, */
    FALSE,  /* AC_FA=0x95e, */
    FALSE,  /* AC_YYA=0x95f, */
    FALSE,  /* NG_RR=0x960, */
    FALSE,  /* NG_LL=0x961, */
    FALSE,  /* NG_L=0x962, */
    FALSE,  /* NG_VLL=0x963, */
    FALSE,  /* NG_D=0x964, */
    FALSE,  /* NG_DD=0x965, */
    FALSE,  /* D_ZERO=0x966, */
    FALSE,  /* D_ONE=0x967, */
    FALSE,  /* D_TWO=0x968, */
    FALSE,  /* D_THREE=0x969, */
    FALSE,  /* D_FOUR=0x96a, */
    FALSE,  /* D_FIVE=0x96b, */
    FALSE,  /* D_SIX=0x96c, */
    FALSE,  /* D_SEVEN=0x96d, */
    FALSE,  /* D_EIGHT=0x96e, */
    FALSE,  /* D_NINE=0x96f, */
    FALSE   /* S_ABBR=0x970 */
};
const U8 g_hindi_character_extended_sign_array[] = 
{
    FALSE,  /* 59648 */
    FALSE,  /* 59649 */
    FALSE,  /* 59650 */
    FALSE,  /* 59651 */
    FALSE,  /* 59652 */
    FALSE,  /* 59653 */
    FALSE,  /* 59654 */
    FALSE,  /* 59655 */
    FALSE,  /* 59656 */
    FALSE,  /* 59657 */
    FALSE,  /* 59658 */
    FALSE,  /* 59659 */
    FALSE,  /* 59660 */
    FALSE,  /* 59661 */
    FALSE,  /* 59662 */
    FALSE,  /* 59663 */
    FALSE,  /* 59664 */
    FALSE,  /* 59665 */
    FALSE,  /* 59666 */
    FALSE,  /* 59667 */
    FALSE,  /* 59668 */
    FALSE,  /* 59669 */
    FALSE,  /* 59670 */
    FALSE,  /* 59671 */
    FALSE,  /* 59672 */
    FALSE,  /* 59673 */
    FALSE,  /* 59674 */
    FALSE,  /* 59675 */
    FALSE,  /* 59676 */
    FALSE,  /* 59677 */
    FALSE,  /* 59678 */
    FALSE,  /* 59679 */
    FALSE,  /* 59680 */
    FALSE,  /* 59681 */
    FALSE,  /* 59682 */
    FALSE,  /* 59683 */
    FALSE,  /* 59684 */
    FALSE,  /* 59685 */
    FALSE,  /* 59686 */
    FALSE,  /* 59687 */
    FALSE,  /* 59688 */
    FALSE,  /* 59689 */
    FALSE,  /* 59690 */
    FALSE,  /* 59691 */
    FALSE,  /* 59692 */
    FALSE,  /* 59693 */
    FALSE,  /* 59694 */
    FALSE,  /* 59695 */
    FALSE,  /* 59696 */
    FALSE,  /* 59697 */
    FALSE,  /* 59698 */
    FALSE,  /* 59699 */
    FALSE,  /* 59700 */
    FALSE,  /* 59701 */
    FALSE,  /* 59702 */
    FALSE,  /* 59703 */
    FALSE,  /* 59704 */
    FALSE,  /* 59705 */
    FALSE,  /* 59706 */
    FALSE,  /* 59707 */
    FALSE,  /* 59708 */
    FALSE,  /* 59709 */
    FALSE,  /* 59710 */
    FALSE,  /* 59711 */
    FALSE,  /* 59712 */
    FALSE,  /* 59713 */
    FALSE,  /* 59714 */
    FALSE,  /* 59715 */
    FALSE,  /* 59716 */
    FALSE,  /* 59717 */
    FALSE,  /* 59718 */
    FALSE,  /* 59719 */
    FALSE,  /* 59720 */
    FALSE,  /* 59721 */
    FALSE,  /* 59722 */
    FALSE,  /* 59723 */
    FALSE,  /* 59724 */
    FALSE,  /* 59725 */
    FALSE,  /* 59726 */
    FALSE,  /* 59727 */
    FALSE,  /* 59728 */
    FALSE,  /* 59729 */
    FALSE,  /* 59730 */
    FALSE,  /* 59731 */
    FALSE,  /* 59732 */
    FALSE,  /* 59733 */
    FALSE,  /* 59734 */
    FALSE,  /* 59735 */
    FALSE,  /* 59736 */
    FALSE,  /* 59737 */
    FALSE,  /* 59738 */
    FALSE,  /* 59739 */
    FALSE,  /* 59740 */
    FALSE,  /* 59741 */
    FALSE,  /* 59742 */
    FALSE,  /* 59743 */
    FALSE,  /* 59744 */
    FALSE,  /* 59745 */
    FALSE,  /* 59746 */
    FALSE,  /* 59747 */
    FALSE,  /* 59748 */
    FALSE,  /* 59749 */
    FALSE,  /* 59750 */
    FALSE,  /* 59751 */
    FALSE,  /* 59752 */
    FALSE,  /* 59753 */
    FALSE,  /* 59754 */
    FALSE,  /* 59755 */
    FALSE,  /* 59756 */
    FALSE,  /* 59757 */
    FALSE,  /* 59758 */
    FALSE,  /* 59759 */
    FALSE,  /* 59760 */
    FALSE,  /* 59761 */
    FALSE,  /* 59762 */
    FALSE,  /* 59763 */
    FALSE,  /* 59764 */
    FALSE,  /* 59765 */
    FALSE,  /* 59766 */
    FALSE,  /* 59767 */
    FALSE,  /* 59768 */
    FALSE,  /* 59769 */
    FALSE,  /* 59770 */
    TRUE,   /* 59771 */
    FALSE,  /* 59772 */
    FALSE,  /* 59773 */
    TRUE,   /* 59774 */
    TRUE,   /* 59775 */
    FALSE,  /* 59776 */
    FALSE,  /* 59777 */
    FALSE,  /* 59778 */
    FALSE,  /* 59779 */
    FALSE,  /* 59780 */
    FALSE,  /* 59781 */
    FALSE,  /* 59782 */
    FALSE,  /* 59783 */
    FALSE,  /* 59784 */
    FALSE,  /* 59785 */
    TRUE,   /* 59786 */
    TRUE,   /* 59787 */
    TRUE,   /* 59788 */
    TRUE,   /* 59789 */
    TRUE,   /* 59790 */
    TRUE,   /* 59791 */
    TRUE,   /* 59792 */
    FALSE,  /* 59793 */
    FALSE,  /* 59794 */
    FALSE,  /* 59795 */
    FALSE,  /* 59796 */
    FALSE,  /* 59797 */
    FALSE,  /* 59798 */
    FALSE,  /* 59799 */
    FALSE,  /* 59800 */
    FALSE,  /* 59801 */
    FALSE,  /* 59802 */
    FALSE,  /* 59803 */
    FALSE,  /* 59804 */
    FALSE,  /* 59805 */
    FALSE,  /* 59806 */
    FALSE,  /* 59807 */
    FALSE,  /* 59808 */
    FALSE,  /* 59809 */
    FALSE,  /* 59810 */
    FALSE,  /* 59811 */
    FALSE,  /* 59812 */
    FALSE,  /* 59813 */
    FALSE,  /* 59814 */
    FALSE,  /* 59815 */
    FALSE,  /* 59816 */
    FALSE,  /* 59817 */
    FALSE,  /* 59818 */
    FALSE,  /* 59819 */
    FALSE,  /* 59820 */
    FALSE,  /* 59821 */
    FALSE,  /* 59822 */
    FALSE,  /* 59823 */
    FALSE,  /* 59824 */
    FALSE,  /* 59825 */
    FALSE,  /* 59826 */
    FALSE,  /* 59827 */

};
#else /* defined (__MMI_LANG_HINDI__) || defined(__MMI_LANG_MARATHI__) */


/*****************************************************************************
 * FUNCTION
 *  mmi_hindi_warning_remove
 * DESCRIPTION
 *  for removing warnings
 * PARAMETERS
 *  void        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_hindi_warning_remove(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* dummy function */
}
#endif /* defined (__MMI_LANG_HINDI__) || defined(__MMI_LANG_MARATHI__) */ /* #if defined (__MMI_LANG_HINDI__) */

#endif /* __MMI_HINDI_H__ */ 

