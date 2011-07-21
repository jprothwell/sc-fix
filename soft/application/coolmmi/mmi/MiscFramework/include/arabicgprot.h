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
 * ArabicGProt.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is header file for ArabicGProt
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------


 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/


#ifndef __MMI_ARABIC_GPROT_H__
#define __MMI_ARABIC_GPROT_H__

typedef enum
{
ARABIC_LIGATURE_NONE,
ARABIC_LIGATURE_ARABIC,
ARABIC_LIGATURE_PERSIAN,
ARABIC_LIGATURE_ARABIC_PERSIAN,
ARABIC_LIGATURE_URDU,
ARABIC_LIGATURE_ARABIC_URDU,
ARABIC_LIGATURE_PERSIAN_URDU,
ARABIC_LIGATURE_ARABIC_PERSIAN_URDU
} ARABIC_LANGUAGE_ENUM;

#define IS_ARABIC_CHARACTER(c)                                                                                          \
   (( ( ((UI_character_type)(c)>=(UI_character_type)0x0620) && ((UI_character_type)(c)<=(UI_character_type)0x06FF) ) || \
   ( ((UI_character_type)(c)>=(UI_character_type)0xFB50) && ((UI_character_type)(c)<=(UI_character_type)0xFDFF) ) ||    \
   ( ((UI_character_type)(c)>=(UI_character_type)0xFE70) && ((UI_character_type)(c)<=(UI_character_type)0xFEFF) ) )?(1):(0))


extern void ArabicShapeEngine_ext(PU16 , PU16 );
extern void ArabicShapeEngine2_ext(PU16, PU16, PU16, PU16);

#endif /* __MMI_ARABIC_GPROT_H__ */ 

