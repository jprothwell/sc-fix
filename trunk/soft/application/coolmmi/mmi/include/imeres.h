/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2001
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
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   IMERes.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This is header file for IME structure for Multiple Bin.
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
#ifndef _IME_Res_H_
#define _IME_Res_H_

#include "mmi_features.h" 

 
#define MAX_LDB_NUMBER 20
#include "commonscreensenum.h"
#include "settingdefs.h"
#include "gui_data_types.h"
#include "wgui_categories_inputsenum.h"
#if defined(__MMI_T9__)
#include "t9api.h"

typedef struct T9LdbMemData_s {
	T9U16 wLdbNum; 			/* Language ID */
	T9U32 nNumFullBlk; 		/* Number of the blocks whose */
								/* data size is BLOCKSIZE. */
								/* BLOCKSIZE is the size of each */
								/* block with assumption that all*/
								/* data blocks but the last one */
								/* are in the same size. */
	T9U32 dwLastBlkSize; 	/* Size of the last data block if*/
								/* its size is smaller than */
								/* BLOCKSIZE. Set to 0 if the */
								/* size equals BLOCKSIZE. And */
								/* this block should be added */
								/* into nNumFullBlk. */
#if defined(__MMI_T9_V7__)
 
//Increased the size to 7 as russian has 7 language database arrays
	const T9U8 T9FARDATA *dwLdbDataBlk[7];
//END 
#else
 
	const T9U8 FARDATA *dwLdbDataBlk[7];
//END 
#endif
								/* Pointers to each data block. */
}T9LdbMemData;

#elif defined(__MMI_ZI__)
#include "zi8api.h"
#define ZI_TR_CHINESE 0
#define ZI_SIM_CHINESE 1
#endif

typedef struct _IMEModeDetails
{
	SUPPORT_INPUT_MODES IME_Mode_ID;
	SUPPORT_INPUT_TYPES IME_Type_ID;
	U16 Common_Screen_StringID;
	U16 Prefered_IMEStringID;
	MMI_BOOL English_Only_Flag;
} sIMEModeDetails;

#if defined(__MMI_T9__)
typedef T9LdbMemData sIMELDBDetails;
#elif defined(__MMI_ZI__)
#if defined(__MMI_ZI_DYNAMIC_LAN_LIB_INIT__)
typedef ZI8_LANGUAGE_ENTRY sIMELDBDetails[MAX_LDB_NUMBER];
#else
typedef ZI8_LANGUAGE_ENTRY sIMELDBDetails;
#endif
#else
typedef char sIMELDBDetails;
#endif

typedef enum
{
	ZiV5,
	ZiV6,
	T9V6,
	T9V7,
	IME_OTHERS
} sIMEModuleDetails;

 
#endif/*IME_RES_H*/

