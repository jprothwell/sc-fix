/******************************************************************************/
/*                                                                            */
/*              Copyright (C) 2009, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* This source code is property of Coolsand. The information contained in     */
/* this file is confidential. Distribution, reproduction, as well as          */
/* exploitation,or transmisison of any content of this file is not            */
/* allowed except if expressly permitted.                                     */
/* Infringements result in damage claims!                                     */
/*                                                                            */
/* FILE NAME                                                                  */
/*       unit_test.h                                                          */
/*                                                                            */
/* DESCRIPTION                                                                */
/*   Unit test header file for 221 project.                                   */
/*                                                                            */
/******************************************************************************/

#ifndef __UNIT_TEST__
#define __UNIT_TEST__

#include "csw.h"


//#define ADP_CFW_DUALSIM_SIMULATOR_SUPPORT


#include "adp_events.h"

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT

////////////////////////////////////////
// Simulate card-1/2 absent.
//#define __UT_SIMU_CARD1_ABSENT__
//#define __UT_SIMU_CARD2_ABSENT__
//#define __UT_SIMU_DM_DUAL_MODE__
//#define __UT_SIMU_DM_ONLY_SIM1__
//#define __UT_SIMU_DM_ONLY_SIM2__
//#define __UT_SIMU_FLIGHT_MODE__

#define CFW_SMS_STORAGE_ME 0x01
#define CFW_SMS_STORAGE_SM 0x02
//message type
#define CFW_SMS_TYPE_PP 0X01
#define CFW_SMS_TYPE_CB 0X02
#define CFW_SMS_TYPE_SR 0X04

#if 0
typedef struct _CFW_SMS_LIST{
	UINT16 nCount;
	UINT16 nStartIndex;
	UINT8 nStorage;
	UINT8 nType;
	UINT8 nStatus;
	UINT8 nOption;
	UINT8 padding[3];
}CFW_SMS_LIST;

typedef struct _CFW_SMS_WRITE{
CFW_DIALNUMBER sNumber;
UINT8* pData;
UINT16 nDataSize;
UINT16 nIndex;
UINT8 nStorage;
UINT8 nType;
UINT8 nStatus;
UINT8 padding[1];
}CFW_SMS_WRITE;
#endif

extern U8 nCCSimId;

//////////////////////////////////////////////////////////////////////
// PUBLIC

extern void adp_SimualtorFillMsgFlag(UINT8 simID, COS_EVENT *pCosEvent);
extern void UT_SimuCfwMsg(UINT32 nEventId, UINT32 nParam1, UINT32 nParam2, UINT32 nParam3);
extern BOOL UT_AdpDoubleInd(COS_EVENT *pev);
extern UINT32 UT_CFW_SimEnterAuthentication(UINT8 *pPin, UINT8 nPinSize, UINT8 *pNewPin,
			UINT8 nNewPinSize, UINT8 nOption, UINT16 nUTI, UINT8 nSimId);

//Phonebook
UINT32 UT_CFW_SimGetPbkStorage(UINT8 nStorage, UINT16 nUTI, UINT8 nSimId);
UINT32 UT_CFW_SimGetPbkEntry(UINT8 nStorage, UINT8 nIndex, UINT16 nUTI , UINT8 nSimId);
UINT32 UT_CFW_SimAddPbkEntry(UINT8 nStorage, CFW_SIM_PBK_ENTRY_INFO* pEntryInfo, UINT16 nUTI, UINT8 nSimId);
UINT32 UT_CFW_SimDeletePbkEntry(UINT8 nStorage, UINT8 nIndex, UINT16 nUTI, UINT8 nSimId);

//FDN
UINT32 UT_CFW_SimListCountPbkEntries(UINT8 nStorage, UINT8 nIndexStart, UINT8 nCount, UINT16 nUTI, UINT8 nSimId);

extern U8 SimuGetSMSID();
extern void SimuSetSMSID(U8 simId);
extern UINT32 UT_CFW_SmsReadMessage(UINT16 nStorage, UINT16 nIndex, UINT16 nUTI, UINT8 nSimId);

//SAT
extern UINT32 UT_CFW_SatActivation(UINT8 nMode, UINT16 nUTI , UINT8 nSimId);
extern UINT32 UT_CFW_SatResponse(UINT8 nCmdType, UINT8 nStatus, UINT8 nItemId, VOID* pInputStr, UINT8 InputStrLen, UINT16 nUTI, UINT8 nSimId);
extern UINT32 UT_CFW_SatGetInformation(UINT8 nCmdType, VOID** CmdData , UINT8 nSimId);
//sms
extern UINT32 UT_CFW_SmsListMessages(CFW_SMS_LIST * pListInfo,  UINT16 nUTI, UINT8 nSimId);
extern UINT32  UT_CFW_SmsGetMessageNode(PVOID pListResult, UINT16 nIndex, CFW_SMS_NODE **pNode, UINT8 nSimId);
extern UINT32 UT_CFW_CfgGetSmsStorageInfo(CFW_SMS_STORAGE_INFO* pStorageInfo, UINT16 nStorage, UINT8 nSimId);
extern UINT32 UT_CFW_SmsSendMessage(CFW_DIALNUMBER* pNumber, UINT8* pData, UINT16 nDataSize, UINT16 nUTI, UINT8 nSimId);
extern UINT32 UT_CFW_SmsWriteMessage(CFW_SMS_WRITE*	pSMSWrite, UINT16 nUTI, UINT8 nSimId);
extern UINT32 UT_CFW_SmsDeleteMessage(UINT16 nIndex, UINT8 nStatus, UINT16 nStorage, UINT8 nType, UINT16 nUTI, UINT8 nSimId);
//////////////////////////////////////////////////////////////////////
//CC
extern UINT32 UT_CFW_CcAcceptSpeechCall(UINT8 nSimId);
extern UINT32 UT_CFW_CcPlayTone(INT8 iTone, UINT8 nSimId);
extern UINT32 UT_CFW_CcGetCurrentCall(
		CFW_CC_CURRENT_CALL_INFO CallInfo[7],
		UINT8* pCnt,
		UINT8 nSimId
	);
extern UINT32 UT_CFW_CcReleaseCall(UINT8 nSimId);
extern UINT32 UT_CFW_CcCallHoldMultiparty(
		UINT8 nCmd,
		UINT8 nIndex,
		UINT8 nSimId
	);
extern UINT32 UT_CFW_CcRejectCall(UINT8 nSimId);
extern UINT32 UT_CFW_CcEmcDialEx(
		UINT8* pDialNumber,
		UINT8 nDialNumberSize,
		UINT8 *pIndex,
		UINT8 nSimId
	);
extern UINT32 UT_CFW_CcInitiateSpeechCallEx(
		CFW_DIALNUMBER* pDialNumber,
		UINT8 *pIndex,
		UINT16 nUTI,
		UINT8 nSimId
	);

extern UINT32 UT_CFW_CcInitiateSpeechCall (
  CFW_DIALNUMBER* pDialNumber,
  UINT16 nUTI,
  UINT8 nSimId
);
///////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
//SS
extern UINT32 UT_CFW_SsSetCallWaiting(UINT8 nMode, UINT8 nClass, UINT16 nUTI, UINT8 nSimId);
extern UINT32 UT_CFW_SsQueryCallWaiting(UINT8 nClass, UINT16 nUTI, UINT8 nSimId);
extern UINT32 UT_CFW_SsSetFacilityLock(UINT16 nFac, UINT8* pBufPwd, UINT8 nPwdSize, UINT8 nClass, UINT8 nMode, UINT16 nUTI, UINT8 nSimId);
extern UINT32 UT_CFW_SsQueryFacilityLock(UINT16 nFac, UINT8 nClass, UINT16 nUTI, UINT8 nSimId);
extern UINT32 UT_CFW_SsSetCallForwarding(CFW_SS_SET_CALLFORWARDING_INFO* pCallForwarding, UINT16 nUTI, UINT8 nSimId);
extern UINT32 UT_CFW_SsQueryCallForwarding(UINT8 nReason,  UINT8 nClass,  UINT16 nUTI, UINT8 nSimId);
extern UINT32 UT_CFW_SsSendUSSD(UINT8* pUsdString, UINT8 nUsdStringSize, UINT8 nOption, UINT8 nDcs, UINT16 nUTI, UINT8 nSimId);
extern UINT32 UT_CFW_SsQueryClip(UINT16 nUTI, UINT8 nSimId);
extern UINT32 UT_CFW_SsQueryClir(UINT16 nUTI, UINT8 nSimId);
extern UINT32 UT_CFW_SsQueryColp(UINT16 nUTI, UINT8 nSimId);
extern UINT32 UT_CFW_SsQueryColr(UINT16 nUTI, UINT8 nSimId);
extern UINT32 UT_CFW_SsChangePassword(UINT16 nFac, UINT8* pBufOldPwd, UINT8 nOldPwdSize, UINT8* pBufNewPwd, UINT8 nNewPwdSize, UINT16 nUTI, UINT8 nSimId);
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//
#define  UT_CFW_CFG_RECEIVE_STORAGE(sms, bm, sr) ((sms)|(bm)|(sr))
#define EV_CFW_SET_COMM_RSP                                  (EV_CFW_NW_RSP_BASE + 8)

typedef enum  {
    	CFW_DISABLE_COMM = 0x00,   //don't start
    	CFW_ENABLE_COMM,  		// start stack
    	CFW_CHECK_COMM 
}CFW_COMM_MODE;
typedef enum {
	CFW_SIM_0 = 0x00,
	CFW_SIM_1 = 0x01,
	CFW_SIM_COUNT = 0x02,
	CFW_SIM_END = 0xFF
}CFW_SIM_ID;

extern UINT32 UT_CFW_GetComm(
		CFW_COMM_MODE* pMode,
		CFW_SIM_ID 	nSimID
	);
extern UINT32 UT_CFW_SetComm(
		CFW_COMM_MODE 	nMode,
		UINT16  		nUTI,
		CFW_SIM_ID		nSimID
	);
extern UINT32 UT_CFW_NwGetCurrentOperator (
  UINT8 OperatorId[6],
  UINT8* pMode,
  CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_CfgGetIndicatorEvent (
  UINT16* ind, CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_CfgSetIndicatorEvent (
  UINT16 ind, CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_SimGetProviderId (
  UINT16 nUTI,
  CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_CfgSetSmsFormat (
  UINT8 nFormat,
  CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_CfgSetSmsShowTextModeParam (
  UINT8 nShow,
  CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_CfgSetClip (
  UINT8 nEnable,
  CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_CfgSetNwStatus (
  UINT8 n,
  CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_NwAbortListOperators(
  UINT16 nUTI,
  CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_NwSetRegistration (
  UINT8 nOperatorId[6],
  UINT8 nMode,
  UINT16 nUTI,
  CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_CfgNwGetFrequencyBand (
  UINT8* pBand,
  CFW_SIM_ID nSimID
);
extern UINT32 SRVAPI UT_CFW_NwGetAvailableOperators (
  UINT16 nUTI,
  CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_NwSetFrequencyBand (
  UINT8 nBand,
  UINT16 nUTI,
  CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_NwDeRegister (
  UINT16 nUTI,
  CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_SimGetPrefOperatorList(
  UINT16 nUTI,
  CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_SimSetPrefOperatorList( 
  UINT8* pOperatorList,
  UINT8 nSize, 
  UINT16 nUTI,
  CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_SimGetFacilityLock (
  UINT16 nFac,
  UINT16 nUTI,
  CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_SimSetFacilityLock (
  UINT16 nFac,
  UINT8* pBufPwd,
  UINT8 nPwdSize,
  UINT8 nMode,
  UINT16 nUTI,
  CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_SimGetAuthenticationStatus (
  UINT16 nUTI,
  CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_SimChangePassword (
  UINT16 nFac,
  UINT8* pBufOldPwd,
  UINT8 nOldPwdSize,
  UINT8* pBufNewPwd,
  UINT8 nNewPwdSize,
  UINT16 nUTI,
  CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_CfgGetSmsStorage (
  UINT8* pStorage,
  CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_CfgSetNewSmsOption (
  UINT8 nOption,
  UINT8 nNewSmsStorage,
  CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_CfgGetSmsParam (CFW_SMS_PARAMETER* pInfo,UINT8 nIndex,CFW_SIM_ID nSimID);
extern UINT32 UT_CFW_CfgSetSmsOverflowInd (
  UINT8 nMode,
   CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_CfgSetSmsParam (
  CFW_SMS_PARAMETER * pInfo, 
  UINT16 nUTI,
  CFW_SIM_ID nSimID
);
extern UINT32 UT_CFW_SimGetPrefOperatorListMaxNum(
  UINT16 nUTI,
  CFW_SIM_ID nSimID
);
////////////////////////////////////////////////////////////////////////

#endif

#endif

