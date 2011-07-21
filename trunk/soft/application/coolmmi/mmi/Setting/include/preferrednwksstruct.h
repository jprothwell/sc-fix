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
 * PreferredNwksStruct.h
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
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#ifndef _PIXCOM_PREFERREDNWKS_STRUCT_H
#define _PIXCOM_PREFERREDNWKS_STRUCT_H


#define	MAX_PREFFERED_PLMN	50//man 2005-6-28
#define	MAX_PRIORITY_DIGITS	3
//#define	CS_NOTIFYDURATION		UI_POPUP_NOTIFYDURATION_TIME



typedef struct _pref_network_list_struct {
	U16 PrefListSize;
	U8 PrefPLMN[MAX_PREFFERED_PLMN][MAX_PLMN_LEN+1];
} PrefNetworkListContext;

typedef struct _pref_network_list_display {
	S32	PriorityVal;
	S8	PrefNetworkListDislpay[MAX_PREFFERED_PLMN][MAX_LENGTH_DISPLAY_NAME*ENCODING_LENGTH];	
	U8	PlmnNew[(MAX_PLMN_LEN+1) * ENCODING_LENGTH];
	U8	NewNwPriorityDisplay[10]; 
	PU8	nStrItemList[MAX_PREFFERED_PLMN];
	U8	IsNewPrefNetwork;
	U8	CurrSelPrefNetwork;
	U8	CurrSelAddPLMN;
	U8	PrefNwGotResponse;
} PrefNetworkListDisplay;

typedef struct
{
	U8 op;
	U8 index;
	U8 plmn[MAX_PLMN_LEN + 1];
} NETQUEUE;

void HighlightNETSETAddPrefNwk(void);
void HighlightNETSETEditPrefNwk(void);
void HighlightNETSETDelPrefNwk(void);
void HighlightNETSETNewPrefNwk(void);
void HiliteHandlerPrefList(S32 hiliteIdx);
void HiliteHandlerAddNetworkPLMNList(S32 hiliteIdx);


void EntryNETSETEditNetwork(void);
void EntryNETSETAddNetworkPLMNList(void);
pBOOL NETSETGetPrefNetworkListItem( S32 , UI_string_type , UI_image_type* , U8 );
S32 NETSETGetPrefNetworkListHint ( S32 , UI_string_type * );
void EntryNETSETShowPrefNwkOptions(void);
void EntryNETSETShowPreferredList(void);
void ExitNETSETShowPreferredListReGet(void);
void EntryNETSETNewNetwork(void);
void ExitNETSETNewNetwork(void);
void EntryNETSETAddNetworkPriority(void);
void EntryNETSETConfirmDeleteNetwork(void);
void ExitNETSETConfirmDeleteNetwork(void);


void NETSETEditNetworkAddPhaseReq(void);
void NETSETEditNetworkAddPhaseRsp(void* info);
void NETSETEditNetworkDeletePhaseReq(void);
void NETSETEditNetworkDeletePhaseRsp(void* info);
void NETSETGetPreferredNetworkReq(void);
void NETSETGetPreferredNetworkRsp(void* info);
void NETSETAddPrefNetworkReq(void);
void NETSETAddPrefNetworkRsp(void* info);
void NETSETDeletePrefNetworkReq(void);
void NETSETDeletePrefNetworkRsp(void* info);


U8 IsNetworkDuplicated(void);
void ConstructPreferredListHint(U8 **hintArray);
PS8 GetCurrentPLMNName(S8* PlmnNo);
U16 GetPrefNwkNameFromPLMN(void);//PU8 *Dispnames);
void GoBackNetworkSetup(void);
void ChcekThenAddNetwork(void);
void GoBackPrefList(void);
void SendNewPrr(void);
void FillNewPrefNetworkInlineEditStruct(void);
void HiliteHandlerPrefNwkHideAddIfFull(S32 index);
U8* InLinePriorityDefault(void);
U8* InLinePriorityMinusOne(void);
U8* InLinePriorityAddOne(void);


extern void playRequestedTone(ALL_TONE_ENUM);
extern void stopRequestedTone(ALL_TONE_ENUM);
extern wgui_inline_item wgui_inline_items[];


#endif//_PIXCOM_PREFERREDNWKS_STRUCT_H


