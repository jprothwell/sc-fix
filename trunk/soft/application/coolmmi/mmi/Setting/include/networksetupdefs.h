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
 * NetworkSetupDefs.h
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

#ifndef _NETWORKSETUPDEFS_H
#define _NETWORKSETUPDEFS_H


#define	PLMN_NAME_LENGTH		30
#ifndef CS_NOTIFYDURATION
#define	CS_NOTIFYDURATION		UI_POPUP_NOTIFYDURATION_TIME
#endif

//micha0715
#define	NETSETMSGQUEUESIZE	5

//micha1109
#define	SET_GPRS_MODE_DELAY	2000  

enum NETSET_OPERATION
{
NETSET_OP_NONE,
NETWORK_SELECTION,
NETWORK_RESTORE,
GET_PLMN_LIST,
BAND_SELECTION
};


typedef struct {
	U8				ItemType;
	FuncPtr		CallBackFunc;
	U32			MsgID;
	PsFuncPtr		CallBackRspHandler;
	U8			srcMode;
} NetworkSetupMsgQueueEntity;


typedef struct {
	U8			Index;
	U8			Top;
	NetworkSetupMsgQueueEntity Queue[NETSETMSGQUEUESIZE];
} NetworkSetupMsgQueue;



typedef struct {
	U8 PLMNRequested[MAX_PLMN_LEN+1];				// last requested PLMN
	U8 NetworkSelectAbort;						// ignore response if user pressed abort
	U8 BandType;
#ifdef __MMI_GPRS_FEATURES__
	U8 GPRSConnType;
#endif
	mmi_nw_get_plmn_list_rsp_struct		*PlmnList;   
} NetworkSetupContext;


typedef struct {
	U16 *PLMNListIcons;
	U8	CurrSelectNetwork;
//micha_Man_sel
	U8	TempCurrSelectNetwork;	
	U8	*PLMNName[MAX_PLMN_LIST_LEN];
	U8	CurrSelectBand;
} NetworkSetupDisplay;


void HighlightNETSETMain(void);
void HighlightNETSETSelectionMode(void);
void HighlightNETSETSelectionManual(void);
void HighlightNETSETSelectionAuto(void);
void HighlightNETSETPreferredNetworks(void);
void HighlightNETSETBandSelection(void);
void HintNETSETBandSelection(U16);
void HintNETSETSelectionMode(U16);
void HiliteHandlerShowPLMNList(S32);
void HiliteHandlerBandSelection(S32);

void EntryNETSETMain(void);
void EntryNETSETSelectionMode(void);
void EntryNETSETShowPlmnList(void);
void ExitNETSETShowPLMNList(void);
void EntryNETSETBandSelection(void);


void NETSETGetPLMNListReq(void);
void NETSETGetPLMNListRsp(void*);
void NETSETAutoNetworkSearchReq(void);
void NETSETAutoNetworkSearchRsp(void*);
void NETSETManualSelectNetworkReq(void);
void NETSETManuSelectNetworkRsp(void*);
void NETSETAbortPLMNListReq(void);
void NETSETAbortPLMNListRsp(void*);
void NETSETSetBandSelectionReq(void);
void NETSETSetBandSelectionRsp(void*);
void NETSETGetBandSelectionReq(void);
void NETSETGetBandSelectionRsp(void*);

void EntryNETSETAutoSearchProgress(void);
void ExitNETSETAutoSearchProgress(void);
void EntryNETSETManualSearchProgress(void);
void ExitNETSETManualSearchProgress(void);
void EntryNETSETBandSelectionProgress(void);
void ExitNETSETBandSelectionProgress(void);
void EntryNETSETManualRegProgress(void);
void ExitNETSETManualRegProgress(void);

void RestorePreviousNetwork(void);
void GoBackNetSelection(void);
void AbortManualRegGoBack(void);
void AbortManualRegToIdle(void);
void AbortAutoSearchGoBack(void);
void AbortAutoSearchToIdle(void);
void AbortGetPLMNListToIdle(void);


void NETSETGetPLMNListCheck(void);
void NETSETManualSelectNetworkCheck(void) ;
void NETSETManualNetworkRestoreReq(void);
void NETSETSetBandSelectionCheck(void);
void NETSETAutoSelectNetworkCheck(void) ;
void NETSETMsgQueueIndexAddOne(void) ;
void NETSETMsgQueueTopAddOne(void);
void NETSETMsgQueueTopMinusOne(void) ;
U8 NETSETMsgQueueGetPreviousTop(void) ;
U8 NETSETMsgQueueGetPreviousIndex(void) ;
void NETSETMsgQueueProtocolHandler(void *info) ;
void NETSETMsgQueuePush(U8 ItemType, FuncPtr CallBackFunc, UINT32 MsgID, PsFuncPtr CallBackRspHandler, U8 src_mod);
U8 NETSETMsgQueueCheckCurrOP(void);
U8 NETSETMsgQueueCheckLastOP(void);
void ExitNETSETSelectionMode(void);
U8 NETSETAutoBandIndex(void);

#ifdef __MMI_GPRS_FEATURES__
void HighlightNETSETGPRSConnectionMode(void);
void HintNETSETGPRSConnectionMode(U16 index);
void NETSETSetGPRSConnModeReq(void);
//micha1109
void NETSETSetGPRSConnModeTimeOut(void);
void NETSETSetGPRSConnModeRsp(void *info);
void NETSETGetGPRSModeReq(void);
void NETSETGetGPRSModeRsp(void *info);
#endif

//micha_Man_sel
#ifdef __MANUAL_MODE_NW_SEL__
void HighlightNETSETSelectionModeChoice(void);
void EntryNETSETSelectionModeReq(void);
void EntryNETSETSelectionModeRsp(void *info) ;
#endif


#endif


