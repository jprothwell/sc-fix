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
 *   resource_ime_jtbl.c
 *
 * Project:
 * --------
 *   Maui_Software
 *
 * Description:
 * ------------
 *   This file implements indirect IME jumps table for multi-load.
 *
 * Author:
 * -------
 *   Terry Yang  (CSD00866)
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi_customer/CustResource/src/resource_lang_pack_jtbl.c-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 11:04:18   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:44:34   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:29:24   zhangxb
 * Initial revision.
 * Revision 1.1.1.1  2006/08/02 02:23:24  sushipeng
 * del .h in the include
 *
 * Revision 1.1.1.1  2006/07/22 08:00:46  sushipeng
 * second change new platform 
 * ssp 20060722
 *
 * Revision 1.1.1.1  2006/07/19 03:49:11  sushipeng
 * change coolsand platform
 * ssp 20060719
 *
 * Revision 1.2  2006/06/21 07:18:44  wangbei
 * Update Resource
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#include "mmi_features.h"

// __CUSTPACK_MULTIBIN     Terry   BEGIN

#include "imeres.h"
#include "custdatares.h"
#include "fontres.h"
#include "kal_release.h"
#include "custom_jump_tbl.h"
#include "verno.h"

#if defined(__MMI_T9_V7__)
#if defined(__MMI_T9_TR_CHINESE__)
extern T9ChineseData const T9FARDATA l0104;
#endif
#if defined(__MMI_T9_SM_CHINESE__)
extern T9ChineseData const T9FARDATA l0204;
#endif
#else
#if defined(__MMI_T9_TR_CHINESE__)
extern T9ChineseData const FARDATA l0104;
#endif
#if defined(__MMI_T9_SM_CHINESE__)
extern T9ChineseData const FARDATA l0204;
#endif
#endif


Type_Preamble_Content LangPackResPreamble = {
	VERNO_STR,
	(kal_uint32)(RESOURCE_TYPE_LANG_PACK + CURRENT_IME_RES_VERSION), 
	CONST_RES_LANG_PACK_ADDR,
	CONST_RES_LANG_PACK_TBL_SIZE,
	CONST_RES_LANG_PACK_CONTENT_SIZE, 
	(kal_uint32)MAGIC_NUMBER
};


extern const U16 CSD_gMaxDeployedLangs;
extern const sLanguageDetails CSD_gLanguageArray[];

extern const StringResList	CSD_gStringList[];

extern const sIMEModeDetails CSD_gIMEModeArray[];
extern const sIMEModeDetails CSD_gIMEQSearchModeArray[];
extern const sIMELDBDetails CSD_gIMELDBArray[];
extern const sIMEModuleDetails CSD_gIMEModule;

void* langpack2ndJumpTbl[] = {
					    (void*) CSD_gLanguageArray, (void*) &CSD_gMaxDeployedLangs,

					    (void*) CSD_gStringList,
					    					    
					    (void*) CSD_gIMEModeArray, (void*) CSD_gIMEQSearchModeArray
                                           ,(void*) CSD_gIMELDBArray, (void*) &CSD_gIMEModule
#if defined(__MMI_T9_TR_CHINESE__)
                                          ,(void*) &l0104
#else
                                          ,0
#endif
#if defined(__MMI_T9_SM_CHINESE__)
                                          , (void*) &l0204
#else
                                          ,0
#endif
                                           };
// __CUSTPACK_MULTIBIN     Terry   END
