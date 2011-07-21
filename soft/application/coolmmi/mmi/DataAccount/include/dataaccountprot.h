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
 * DataAccount.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for ¡K.
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _DATA_ACCOUNT_PROT_H
#define _DATA_ACCOUNT_PROT_H

extern wgui_inline_item wgui_inline_items[];
extern DataAccountNode g_DataAccount[MAX_DATA_ACCOUNT_GSM_LIMIT];

#ifdef __MMI_GPRS_FEATURES__
extern GPRSDataAccountNode g_GPRS_DataAccount[MAX_DATA_ACCOUNT_GPRS_LIMIT];
#endif 

extern void EntryDTCNTList(void);
extern void EntryDTCNTEdit(void);
extern void ExitDTCNTEdit(void);
extern void EntryDTCNTSave(void);
extern void EntryDTCNTProgress(void);

extern void DTCNTSetAccountReq(U8 ItemNum);
extern void DTCNTSetAccountRsp(void *info);
extern void DTCNTGetAccountReq(U8 ItemNum);
extern void DTCNTGetAccountRsp(void *info);

extern void HighlightDTCNTList(S32 nIndex);
extern void DataAccountEditNVRAM(void);
extern void InitInLineDataAccount(void);
extern void DataAccountFillInlineStruct(void);
extern U8 VerifyDataAcountChanges(void);
extern void DataAccountSaveChanges(void);
extern void GoToMainDataAccountScreen(void);
extern void ShowDataAccountOKMessage(void);
extern void DataAccountSavingScreenInProgress(U16 msdId);
extern void PreparedIPAddressString(U8 *string_buffer, U8 *IP1, U8 *IP2, U8 *IP3, U8 *IP4);
extern void ReturnThreeDigitString(PU8 StringVal, U8 DigitVal);
extern void DataAccountFullLineEditScreen(void);
extern void HighlightAppDTCNTList(S32 nIndex);
extern void EntryAppDTCNTList(void);
extern void AppDTCNTSelectOK(void);
extern void SetDynaDataAccountName(S8 *UCS2Name, U8 index);

#ifdef __MMI_GPRS_FEATURES__
extern void EntryDTCNTGPRSList(void);
extern void EntryDTCNTGPRSEdit(void);
extern void ExitDTCNTGPRSEdit(void);
extern void EntryDTCNTGPRSSave(void);

extern void DTCNTGPRSGetAccountReq(U8);
extern void DTCNTGPRSGetAccountRsp(void *info);
extern void DTCNTGPRSSetAccountReq(U8);
extern void DTCNTGPRSSetAccountRsp(void *info);

extern void HighlightDTCNTGPRSList(S32);
extern void InitInLineGPRSDataAccount(void);
extern void GPRSDataAccountFillInlineStruct(void);
extern U8 VerifyGPRSDataAcountChanges(void);
extern void GPRSDataAccountSaveChanges(void);

extern void HighlightAppDTCNTMain(S32 nIndex);
extern void EntryDTCNTAppMain(void);
extern void AppDTCNTMainSelectOK(void);
extern void HighlightGPRSAppDTCNTList(S32 nIndex);
extern void EntryGPRSAppDTCNTList(void);
extern void GPRSAppDTCNTSelectOK(void);

#endif /* __MMI_GPRS_FEATURES__ */ 

#endif /* _DATA_ACCOUNT_PROT_H */ 

