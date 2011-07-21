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
 *   custom_jump_tbl.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This is header file for indirect jump table for multi-load.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _CUSTOM_JUMP_TBL_H_
#define _CUSTOM_JUMP_TBL_H_

#if ( (defined(CSD_TARGET) || defined (CSD_KAL_MNT)) && !defined(__MAUI_BASIC__) && !defined(__L1_STANDALONE__) ) 

#include "kal_release.h"
#include "resource_verno.h"


//#define MULTIPLE_BINARY_FILES


extern const Type_Preamble_Content system_jump_table[];

//__CUSTPACK_MULTIBIN
typedef enum 
{
   JUMP_TABLE_ENTRY_START, 
   
#if defined(__OLD_MULTIPLE_BIN__)

   JUMP_TABLE_ENTRY_AUDIO = JUMP_TABLE_ENTRY_START, 
   JUMP_TABLE_ENTRY_IMAGE,
#ifdef __J2ME__
   JUMP_TABLE_ENTRY_J2ME, 
#endif
   JUMP_TABLE_ENTRY_LANG_PACK, 
   JUMP_TABLE_ENTRY_CUSTPACK,
   JUMP_TABLE_ENTRY_END = JUMP_TABLE_ENTRY_CUSTPACK
   
#else /* __OLD_MULTIPLE_BIN__ */

   JUMP_TABLE_ENTRY_LANG_PACK = JUMP_TABLE_ENTRY_START, 
   JUMP_TABLE_ENTRY_CUSTPACK,
   JUMP_TABLE_ENTRY_END = JUMP_TABLE_ENTRY_CUSTPACK
   
#endif /* __OLD_MULTIPLE_BIN__ */

} JUMP_TABLE_ENTRY_ENUM;


#define JUMP_TABLE_ENTRY_END              JUMP_TABLE_ENTRY_CUSTPACK


/* Macros for addressing each resource */
#define GET_Res_Addr(R, X)                (((kal_uint32 *) (system_jump_table[R].StartAddress)) + X)
#define GET_Res_ProjectID(X)              (system_jump_table[X].ProjectID)
#define GET_Res_Magic(X)                  (system_jump_table[X].Magic)


#ifdef MULTIPLE_BINARY_FILES


#if defined(__OLD_MULTIPLE_BIN__)

/* Macros below specific for multiple binary files or multiple load segments only */
/* Collection of images */
extern kal_uint32  Load$$IMAGE_ROM$$Base;
extern kal_uint32  Image$$IMAGE_ROM$$Length;
extern kal_uint32  Image$$IMAGE_ROM_CONTENT$$Length;

/* Collection of audio */
extern kal_uint32  Load$$AUDIO_ROM$$Base;
extern kal_uint32  Image$$AUDIO_ROM$$Length;
extern kal_uint32  Image$$AUDIO_ROM_CONTENT$$Length;

/* Collection of Lang Pack */
extern kal_uint32  Load$$LANG_PACK_ROM$$Base;
extern kal_uint32  Image$$LANG_PACK_ROM$$Length;
extern kal_uint32  Image$$LANG_PACK_ROM_CONTENT$$Length;

//__CUSTPACK_MULTIBIN__
extern kal_uint32  Load$$CUSTPACK_ROM$$Base;
extern kal_uint32  Image$$CUSTPACK_ROM$$Length;
extern kal_uint32  Image$$CUSTPACK_ROM_CONTENT$$Length;

#ifdef __J2ME__
/* Collection of JAVA games */
extern kal_uint32  Load$$J2ME_ROM$$Base;
extern kal_uint32  Image$$J2ME_ROM$$Length;
extern kal_uint32  Image$$J2ME_ROM_CONTENT$$Length;
#endif /*__J2ME__*/


/* macro of resource address and size for const arrary initialization */
#define CONST_RES_IMAGE_ADDR              (&Load$$IMAGE_ROM$$Base)
#define CONST_RES_IMAGE_TBL_SIZE          ((kal_uint32)&Image$$IMAGE_ROM$$Length)
#define CONST_RES_IMAGE_CONTENT_SIZE      ((kal_uint32)&Image$$IMAGE_ROM_CONTENT$$Length)
#define CONST_RES_AUDIO_ADDR              (&Load$$AUDIO_ROM$$Base)
#define CONST_RES_AUDIO_TBL_SIZE          ((kal_uint32)&Image$$AUDIO_ROM$$Length)
#define CONST_RES_AUDIO_CONTENT_SIZE      ((kal_uint32)&Image$$AUDIO_ROM_CONTENT$$Length)
#define CONST_RES_LANG_PACK_ADDR          (&Load$$LANG_PACK_ROM$$Base)
#define CONST_RES_LANG_PACK_TBL_SIZE      ((kal_uint32)&Image$$LANG_PACK_ROM$$Length)
#define CONST_RES_LANG_PACK_CONTENT_SIZE  ((kal_uint32)&Image$$LANG_PACK_ROM_CONTENT$$Length)
#define CONST_RES_CUSTPACK_ADDR           (&Load$$CUSTPACK_ROM$$Base)
#define CONST_RES_CUSTPACK_TBL_SIZE       ((kal_uint32)&Image$$CUSTPACK_ROM$$Length)
#define CONST_RES_CUSTPACK_CONTENT_SIZE   ((kal_uint32)&Image$$CUSTPACK_ROM_CONTENT$$Length)

#ifdef __J2ME__
#define CONST_RES_J2ME_ADDR               (&Load$$J2ME_ROM$$Base)
#define CONST_RES_J2ME_TBL_SIZE           ((kal_uint32)&Image$$J2ME_ROM$$Length)
#define CONST_RES_J2ME_CONTENT_SIZE       ((kal_uint32)&Image$$J2ME_ROM_CONTENT$$Length)
#endif /*__J2ME__*/

#define GET_ResImage_Addr(X)              GET_Res_Addr(JUMP_TABLE_ENTRY_IMAGE, X)
#define GET_ResAudio_Addr(X)              GET_Res_Addr(JUMP_TABLE_ENTRY_AUDIO, X)
#define GET_ResLangPack_Addr(X)           GET_Res_Addr(JUMP_TABLE_ENTRY_LANG_PACK, X)
#define GET_ResCUSTPACK_Addr(X)				GET_Res_Addr(JUMP_TABLE_ENTRY_CUSTPACK, X)

#ifdef __J2ME__
#define GET_ResJ2ME_Addr(X)               GET_Res_Addr(JUMP_TABLE_ENTRY_J2ME, X)
#endif /*__J2ME__*/


#else /* __OLD_MULTIPLE_BIN__ */

extern Type_Preamble_Content AudioResPreamble;
extern void* audio2ndJumpTbl[];

extern Type_Preamble_Content ImageResPreamble;
extern void* image2ndJumpTbl[];

#ifdef __J2ME__
extern Type_Preamble_Content J2MEResPreamble;
extern void* J2ME2ndJumpTbl[];
#endif /*__J2ME__*/

#define CONST_RES_IMAGE_ADDR              ((kal_uint32 *)&ImageResPreamble)
#define CONST_RES_IMAGE_TBL_SIZE          ((kal_uint32)0)
#define CONST_RES_IMAGE_CONTENT_SIZE      ((kal_uint32)0)
#define CONST_RES_AUDIO_ADDR              ((kal_uint32 *)&AudioResPreamble)
#define CONST_RES_AUDIO_TBL_SIZE          ((kal_uint32)0)
#define CONST_RES_AUDIO_CONTENT_SIZE      ((kal_uint32)0)

#ifdef __J2ME__
#define CONST_RES_J2ME_ADDR               ((kal_uint32 *)&J2MEResPreamble)
#define CONST_RES_J2ME_TBL_SIZE           ((kal_uint32)0)
#define CONST_RES_J2ME_CONTENT_SIZE       ((kal_uint32)0)
#endif /*__J2ME__*/  


/* Macros below specific for multiple binary files or multiple load segments only */
/* Collection of Lang Pack */
extern kal_uint32  Load$$LANG_PACK_ROM$$Base;
extern kal_uint32  Image$$LANG_PACK_ROM$$Length;
extern kal_uint32  Image$$LANG_PACK_ROM_CONTENT$$Length;

//__CUSTPACK_MULTIBIN__
extern kal_uint32  Load$$CUSTPACK_ROM$$Base;
extern kal_uint32  Image$$CUSTPACK_ROM$$Length;
extern kal_uint32  Image$$CUSTPACK_ROM_CONTENT$$Length;


/* macro of resource address and size for const arrary initialization */
#define GET_ResLangPack_Addr(X)           GET_Res_Addr(JUMP_TABLE_ENTRY_LANG_PACK, X)
#define CONST_RES_LANG_PACK_ADDR          (&Load$$LANG_PACK_ROM$$Base)
#define CONST_RES_LANG_PACK_TBL_SIZE      ((kal_uint32)&Image$$LANG_PACK_ROM$$Length)
#define CONST_RES_LANG_PACK_CONTENT_SIZE  ((kal_uint32)&Image$$LANG_PACK_ROM_CONTENT$$Length)


//__CUSTPACK_MULTIBIN__
#define GET_ResCUSTPACK_Addr(X)				GET_Res_Addr(JUMP_TABLE_ENTRY_CUSTPACK, X)
#define CONST_RES_CUSTPACK_ADDR           (&Load$$CUSTPACK_ROM$$Base)
#define CONST_RES_CUSTPACK_TBL_SIZE       ((kal_uint32)&Image$$CUSTPACK_ROM$$Length)
#define CONST_RES_CUSTPACK_CONTENT_SIZE   ((kal_uint32)&Image$$CUSTPACK_ROM_CONTENT$$Length)

#endif //defined(__OLD_MULTIPLE_BIN__)


#else /* MULTIPLE_BINARY_FILES */

extern Type_Preamble_Content AudioResPreamble;
extern void* audio2ndJumpTbl[];
extern Type_Preamble_Content ImageResPreamble;
extern void* image2ndJumpTbl[];
extern Type_Preamble_Content LangPackResPreamble ;
extern void* langpack2ndJumpTbl[];

#ifdef __J2ME__
extern Type_Preamble_Content J2MEResPreamble;
extern void* J2ME2ndJumpTbl[];
#endif /*__J2ME__*/

extern Type_Preamble_Content CustPackResPreamble ;
extern void* custpack2ndJumpTbl[];


#define CONST_RES_IMAGE_ADDR              ((kal_uint32 *)&ImageResPreamble)
#define CONST_RES_IMAGE_TBL_SIZE          ((kal_uint32)0)
#define CONST_RES_IMAGE_CONTENT_SIZE      ((kal_uint32)0)
#define CONST_RES_AUDIO_ADDR              ((kal_uint32 *)&AudioResPreamble)
#define CONST_RES_AUDIO_TBL_SIZE          ((kal_uint32)0)
#define CONST_RES_AUDIO_CONTENT_SIZE      ((kal_uint32)0)
#define CONST_RES_LANG_PACK_ADDR          ((kal_uint32 *)&LangPackResPreamble)
#define CONST_RES_LANG_PACK_TBL_SIZE      ((kal_uint32)0)
#define CONST_RES_LANG_PACK_CONTENT_SIZE  ((kal_uint32)0)
#define CONST_RES_CUSTPACK_ADDR           ((kal_uint32 *)&CustPackResPreamble)
#define CONST_RES_CUSTPACK_TBL_SIZE       ((kal_uint32)0)
#define CONST_RES_CUSTPACK_CONTENT_SIZE   ((kal_uint32)0)

#ifdef __J2ME__
#define CONST_RES_J2ME_ADDR               ((kal_uint32 *)&J2MEResPreamble)
#define CONST_RES_J2ME_TBL_SIZE           ((kal_uint32)0)
#define CONST_RES_J2ME_CONTENT_SIZE       ((kal_uint32)0)
#endif /*__J2ME__*/


#define GET_ResImage_Addr(X)              ((kal_uint32 *)&image2ndJumpTbl)
#define GET_ResAudio_Addr(X)              ((kal_uint32 *)&audio2ndJumpTbl)
#define GET_ResFont_Addr(X)               ((kal_uint32 *)&font2ndJumpTbl)
#define GET_ResStr_Addr(X)                ((kal_uint32 *)&str2ndJumpTbl)
#define GET_ResIME_Addr(X)                ((kal_uint32 *)&IME2ndJumpTbl)
#define GET_ResLangPack_Addr(X)           ((kal_uint32 *)(void *)&langpack2ndJumpTbl)
#define GET_ResCUSTPACK_Addr(X)           ((kal_uint32 *)(void *)&custpack2ndJumpTbl)

#ifdef __J2ME__
#define GET_ResJ2ME_Addr(X)               ((kal_uint32 *)&J2ME2ndJumpTbl)
#endif /*__J2ME__*/


#endif /* MULTIPLE_BINARY_FILES */

#endif /* CSD_TARGET */

#endif /* _CUSTOM_JUMP_TBL_H_ */
