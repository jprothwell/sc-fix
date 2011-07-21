/******************************************************************************/
/*                                                                            */
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
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
/*      cfw_win32.c                                                                 */
/*                                                                            */
/* DESCRIPTION                                                                */
/*   Communication Framework constant                                    */
/*                                                                            */
/******************************************************************************/

//
// the necessary header and pragma defines.
//
//#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "cfw.h"
#include "cfw_multi_sim.h"
UINT32 CFW_SetFlightMode(
  UINT8 bFM, UINT8 nUTI
)
{
return 0;
}



UINT32 SRVAPI CFW_BalInit (
  VOID 
){}

UINT32 SRVAPI CFW_BalSetEventPorc (
  PVOID pUserData,
  PFN_CFW_EV_PORC pfnProc
){}

UINT32 SRVAPI CFW_IsFreeUTI (
  UINT16 nUTI,
  UINT32 nServiceId
){}

UINT32 SRVAPI CFW_GetFreeUTI (
  UINT32 nServiceId,
  UINT8* pUTI 
){}

BOOL SRVAPI CFW_BlockEvent (
  UINT32 event
){} 

BOOL SRVAPI CFW_UnBlockEvent (
  UINT32 event
){} 

UINT32 SRVAPI CFW_IsServiceAvailable (
  UINT32 nServiceId,
  UINT8* pAvailable
){}

UINT32 SRVAPI CFW_SendReqEvent (
  UINT32 nEventId,
  UINT32 nParam1,
  UINT32 nParam2,
  UINT8 nUTI
 ){}


UINT32 SRVAPI CFW_ShellControl (
  UINT8 nCommand
 ){}

//
// Configuration(CFG)  Management 
//

// General Configuration
UINT32 SRVAPI CFW_CfgSetPhoneFunctionality(
  UINT8 nLevel,
  UINT8 nRst
){}

UINT32 SRVAPI CFW_CfgGetPhoneFunctionality(
  UINT8* pLevel
){}

UINT32 SRVAPI CFW_CfgStoreUserProfile (
  VOID
){}

UINT32 SRVAPI CFW_CfgRestoreFactoryProfile (
  VOID
){}

UINT32 SRVAPI CFW_CfgGetUserProfile (VOID){}


UINT32 SRVAPI CFW_CfgGetCharacterSet (
  UINT8* chset
){}

UINT32 SRVAPI CFW_CfgSetCharacterSet (
  UINT8 chset
){}


UINT32 SRVAPI CFW_CfgSetIndicatorEvent (
  UINT16 ind
){}

UINT32 SRVAPI CFW_CfgGetIndicatorEvent (
  UINT16* ind
){}

UINT32 SRVAPI CFW_CfgSetIndicatorEventInfo (
  CFW_IND_EVENT_INFO* pIndInfo
){}

UINT32 SRVAPI CFW_CfgGetIndicatorEventInfo (
  CFW_IND_EVENT_INFO* pIndInfo
){}

UINT32 SRVAPI CFW_CfgGetEventReport(
  UINT16* pMer
){}

UINT32 SRVAPI CFW_CfgSetEventReport(
  UINT16 pMer
){}

UINT32 SRVAPI CFW_CfgSetResultCodePresentMode(
  UINT8 nMode
){}

UINT32 SRVAPI CFW_CfgSetResultCodeFormatMode(
  UINT8 nMode
){}

UINT32 SRVAPI CFW_CfgGetResultCodePresentMode(
  UINT8* pMode
){}

UINT32 SRVAPI CFW_CfgGetResultCodeFormatMode(
  UINT8* pMode
){}

UINT32 SRVAPI CFW_CfgGetProductIdentification(
  UINT8* pIdText
){}

UINT32 SRVAPI CFW_CfgGetExtendedErrorV0 (
  PSTR* pErrReport
){}

UINT32 SRVAPI CFW_CfgSetExtendedErrorV0 (
  UINT32 nErrorCode
){}

UINT32 SRVAPI CFW_CfgSetEchoMode(UINT8 nMode){}
UINT32 SRVAPI CFW_CfgGetEchoMode(UINT8 *nMode){}

UINT32 SRVAPI CFW_CfgGetExtendedErrorV1 (
  CFW_EXTENDED_ERROR* pExtendedError
){}

UINT32 SRVAPI CFW_CfgGetErrorReport(
  UINT8* pLevel
){}

UINT32 SRVAPI CFW_CfgSetErrorReport(
  UINT8 nLevel
){}
#ifdef __MMI_MULTI_SIM__
VOID CFW_SetMNCLen(UINT8 nLen,CFW_SIM_ID nSimID)
{
}
VOID CFW_GetMNCLen(UINT8 *pLen,CFW_SIM_ID nSimID)
{
}
#else
VOID CFW_SetMNCLen(UINT8 nLen )
{
}
VOID CFW_GetMNCLen(UINT8 *pLen )
{
}
#endif

INT32 VDS_FlushCacheAll(VOID)
{
return 0;
}

#ifdef __MMI_MULTI_SIM__
UINT32 CFW_SimReset(UINT16 nUTI, CFW_SIM_ID nSimID)
{
	return 0;
}
#else
UINT32 CFW_SimReset(UINT16 nUTI)
{
	return 0;
}
#endif
//SMS Configuration

#ifdef __MMI_MULTI_SIM__

UINT32 CFW_CfgSetSmsStorage (
  UINT8 nStorage, 
  CFW_SIM_ID nSimID
){}

#else

UINT32 SRVAPI CFW_CfgSetSmsStorage (
  UINT8 nStorage1,
  UINT8 nStorage2
){}

#endif

// UINT32 SRVAPI CFW_CfgGetSmsStorage (
//   UINT8* pStorage1,
//   UINT8* pStorage2
// ){}


UINT32 SRVAPI CFW_CfgGetSmsStorageInfo (
  CFW_SMS_STORAGE_INFO* pStorageInfo,
  UINT16 nStorage
){}

UINT32 SRVAPI CFW_CfgSetNewSmsOption (
  UINT8 nOption,
  UINT8 nNewSmsStorage
){}

UINT32 SRVAPI CFW_CfgGetNewSmsOption (
  UINT8* pOption, 
  UINT8* pNewSmsStorage
){}

UINT32 SRVAPI CFW_CfgSelectSmsService(
  UINT8 nService,
  UINT8* pSupportedType
){}

UINT32 SRVAPI CFW_CfgQuerySmsService (
  UINT8* pService
){}

UINT32 SRVAPI CFW_CfgSetSmsOverflowInd (
  UINT8 nMode
){}

UINT32 SRVAPI CFW_CfgGetSmsOverflowInd (
  UINT8* pMode
){}

UINT32 SRVAPI CFW_CfgSetSmsFormat (
  UINT8 nFormat
){}

UINT32 SRVAPI CFW_CfgGetSmsFormat (
  UINT8* nFormat
){}

UINT32 SRVAPI CFW_CfgSetSmsShowTextModeParam (
  UINT8 nShow
){}

UINT32 SRVAPI CFW_CfgGetSmsShowTextModeParam (
  UINT8* pShow
){}

UINT32 SRVAPI CFW_CfgSetSmsCB (
  UINT8 mode,
  UINT8 mids[6],
  UINT8 dcss[6]
){}

UINT32 SRVAPI CFW_CfgGetSmsCB (
  UINT8* pMode,
  UINT8 mids[6],
  UINT8 dcss[6]
){}

UINT32 SRVAPI CFW_CfgSetSmsParam (
  CFW_SMS_PARAMETER* pInfo,
  UINT8 nIndex,
  BOOL bDefault
){}

UINT32 SRVAPI CFW_CfgGetSmsParam (
  CFW_SMS_PARAMETER* pInfo,
  UINT8 nIndex
){}

UINT32 SRVAPI CFW_CfgSetSmsStorageInfo (
  CFW_SMS_STORAGE_INFO* pStorageInfo,
  UINT16 nStorage
){}

//SIM Switch
UINT32 SRVAPI CFW_CfgSetSimSwitch(UINT8 n){}

UINT32 SRVAPI CFW_CfgGetSimSwitch(UINT8* n){}

VOID SimSwitch(UINT8 SimNum){}


// PBK Configuration

UINT32 SRVAPI CFW_CfgSetPbkStorage (
  UINT8 nStorage
){}

UINT32 SRVAPI CFW_CfgGetPbkStorage (
  UINT8* nStorage
){}

UINT32 SRVAPI CFW_CfgGetPbkStrorageInfo (
  CFW_PBK_STRORAGE_INFO* pStorageInfo
){}

UINT32 SRVAPI CFW_CfgSetPbkStrorageInfo (
  CFW_PBK_STRORAGE_INFO* pStorageInfo
){}

// NW Configuration
UINT32 SRVAPI CFW_CfgNwSetFrequencyBand (
  UINT8 nBand
){}

UINT32 SRVAPI CFW_CfgNwGetFrequencyBand (
  UINT8* pBand
){}
UINT32 SRVAPI CFW_CfgNwGetNetWorkMode(UINT8 *pMode){}

UINT32 SRVAPI CFW_CfgNwSetNetWorkMode(UINT8 nMode){}


UINT32 SRVAPI CFW_CfgNwGetOperatorInfo (
  UINT8** pOperatorId,
  UINT8** pOperatorName,
  UINT8 nIndex
){}

UINT32 SRVAPI CFW_CfgGetNwOperDispFormat(
  UINT8* nFormat
){}

UINT32 SRVAPI CFW_CfgNwGetOperatorId (
  UINT8** pOperatorId,
  UINT8* pOperatorName
){}

UINT32 SRVAPI CFW_CfgNwGetOperatorName (
  UINT8 pOperatorId[6],
  UINT8** pOperatorName
){}

UINT32 SRVAPI CFW_CfgSetNwOperDispFormat(
  UINT8 nFormat
){}

UINT32 SRVAPI CFW_CfgGetNwStatus (
  UINT8* pCmd
){}

UINT32 SRVAPI CFW_CfgSetNwStatus (
  UINT8 n
){}

// CM Configuration
UINT32 SRVAPI CFW_CfgSetSSN (
  UINT8 nCSSI,
  UINT8 nCSSU
){}

UINT32 SRVAPI CFW_CfgGetSSN (
  UINT8* pCSSI,
  UINT8* pCSSU
){}

UINT32 SRVAPI CFW_CfgSetClir (
  UINT8 nClir
){}

UINT32 SRVAPI CFW_CfgGetClir (
  UINT8* n
){}

UINT32 SRVAPI CFW_CfgSetToneDuration (
  UINT8  nToneDuration
){}

UINT32 SRVAPI CFW_CfgGetToneDuration (
  UINT8* pToneDuration
){}

UINT32 SRVAPI CFW_CfgSetClip (
  UINT8 nEnable
){}

UINT32 SRVAPI CFW_CfgGetClip (
  UINT8* n
){}

UINT32 SRVAPI CFW_CfgSetColp (
  UINT8 n
){}

UINT32 SRVAPI CFW_CfgGetColp (
  UINT8* n
){}

UINT32 SRVAPI CFW_CfgSetCallWaiting (
  UINT8 n
){}

UINT32 SRVAPI CFW_CfgGetCallWaiting (
  UINT8* n
){}

// Device Configuration

UINT32 SRVAPI CFW_CfgSetAudioOutputParam (
  CFW_AUD_OUT_PARAMETER* pAudOutParam
){}

UINT32 SRVAPI CFW_CfgGetAudioOutputParam (
  CFW_AUD_OUT_PARAMETER* pAudOutParam
){}

UINT32 SRVAPI CFW_CfgSetAudioAudioMode (
  UINT8 nAudioMode
){}

UINT32 SRVAPI CFW_CfgGetAudioAudioMode (
  UINT8* pAudioMode
){}

UINT32 SRVAPI CFW_CfgSetLoudspeakerVolumeLevel(
  UINT8 nVolumeLevel
){}

UINT32 SRVAPI CFW_CfgGetLoudspeakerVolumeLevel(
  UINT8* pVolumeLevel
){}

UINT32 SRVAPI CFW_CfgGetLoudspeakerVolumeLevelRange(
  UINT8* pMinVolumeLevel,
  UINT8* pMaxVolumeLevel
){}

UINT32 SRVAPI CFW_CfgDeviceSwithOff (
  VOID
){}

//
// NW Service
//
UINT32 SRVAPI CFW_NwGetAvailableOperators (
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_NwGetCurrentOperator (
  UINT8 OperatorId[6],
  UINT8* pMode
){}

UINT32 SRVAPI CFW_NwGetImei (
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_NwGetSignalQuality (
  UINT8* pSignalLevel,
  UINT8* pBitError
){}


UINT32 SRVAPI CFW_NwGetStatus (
  CFW_NW_STATUS_INFO* pStatusInfo
){}

UINT32 SRVAPI CFW_NwSetRegistration (
  UINT8 nOperatorId[6],
  UINT8 nMode,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_NwDeRegister (
  UINT16 nUTI
){}


UINT32 SRVAPI CFW_NwSetFrequencyBand (
  UINT8 nBand,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_NwAbortListOperators(UINT16 nUTI){}
    

//
// CM Service
//

UINT32 SRVAPI CFW_CcAcceptSpeechCall (
  VOID
){}

UINT32 SRVAPI CFW_CcGetCallStatus(
  VOID
){}

UINT32 SRVAPI CFW_CcGetCurrentCall (
  CFW_CC_CURRENT_CALL_INFO CallInfo[7],
  UINT8* pCnt
){}

UINT32 SRVAPI CFW_CcCallHoldMultiparty (
  UINT8 nCmd,
  UINT8 nIndex
){}

UINT32 SRVAPI CFW_CcInitiateSpeechCall (
  CFW_DIALNUMBER* pDialNumber,
  UINT16 nUTI
){}

//jiashuo add start
UINT32 CFW_CcInitiateSpeechCallEx (
  CFW_DIALNUMBER* pDialNumber,
  UINT8 *pIndex,
  UINT16 nUTI
){}
//jiashuo add end

UINT32 SRVAPI CFW_CcEmcDial (
                      UINT8* pNumber,
                      UINT8 nSize
                      ){}

//jiashuo add start
UINT32 CFW_CcEmcDialEx (
  UINT8 *pNumber,
  UINT8  nSize,
  UINT8 *pIndex
){}
//jiashuo add end

UINT32 SRVAPI CFW_CcReleaseCall (
  VOID
){}

UINT32 SRVAPI CFW_CcRejectCall (
  VOID
){}

UINT32 SRVAPI CFW_CcPlayTone (
  INT8 iTone
){}

UINT32 SRVAPI CFW_SsQueryCallWaiting (
  UINT8 nClass, 
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SsSetCallWaiting (
  UINT8 nMode, 
  UINT8 nClass,
  UINT16 nUTI //Hameina 2006.10.27 [add]:the original one has no nUTI
){}

#ifdef __MMI_MULTI_SIM__
UINT32 SRVAPI CFW_SsQueryCallForwarding (
  UINT8 nReason,
  UINT8 nClass,
  UINT16 nUTI,
  CFW_SIM_ID nSimID
);
#else
UINT32 SRVAPI CFW_SsQueryCallForwarding (
  UINT8 nReason,
  UINT8 nClass,
  UINT16 nUTI
){}
#endif /*__MMI_MULTI_SIM__*/

UINT32 SRVAPI CFW_SsSetCallForwarding (
  CFW_SS_SET_CALLFORWARDING_INFO* pCallForwarding,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SsSetFacilityLock (
  UINT16 nFac,
  UINT8* pBufPwd,
  UINT8 nPwdSize,
  UINT8 nClass,
  UINT8 nMode,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SsQueryFacilityLock (
  UINT16 nFac,
  UINT8 nClass,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SsChangePassword (
  UINT16 nFac,
  UINT8* pBufOldPwd,
  UINT8 nOldPwdSize,
  UINT8* pBufNewPwd,
  UINT8 nNewPwdSize, 
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SsQueryClip (
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SsQueryClir (
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SsQueryColp (
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SsQueryColr (
  UINT16 nUTI
){}


UINT32 SRVAPI  CFW_SsSendUSSD (
  UINT8* pUsdString,
  UINT8  nUsdStringSize,
  UINT8  nOption,
  UINT8  nDcs,
  UINT16 nUTI
){}

//
// SIM Service
//

UINT32 SRVAPI CFW_SimAddPbkEntry (
  UINT8 nStorage,
  CFW_SIM_PBK_ENTRY_INFO* pEntryInfo,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SimDeletePbkEntry (
  UINT8 nStorage,
  UINT8 nIndex,
  UINT16 nUTI 
){}

UINT32 SRVAPI CFW_SimGetPbkEntry (
  UINT8 nStorage,
  UINT8 nIndex,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SimListPbkEntries (
  UINT8 nStorage,
  UINT8 nIndexStart,
  UINT8 nIndexEnd,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SimListCountPbkEntries (
    UINT8 nStorage,
    UINT8 nIndexStart,
    UINT8 nCount,
    UINT16 nUTI
){}

UINT32 SRVAPI CFW_SimGetPbkMode (
  UINT16 nUTI 
){}

UINT32 SRVAPI CFW_SimSetPbkMode (
  UINT8 nPbkMode,
  UINT16 nUTI 
){}

UINT32 SRVAPI CFW_SimGetPbkStrorageInfo (
  UINT8 nStorage,
  UINT16 nUTI 
){}

UINT32 SRVAPI CFW_SimGetPbkStorage(
  UINT8 nStorage,
  UINT16 nUTI
  ){}

UINT32 SRVAPI CFW_SimGetProviderId (
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SimChangePassword (
  UINT16 nFac,
  UINT8* pBufOldPwd,
  UINT8 nOldPwdSize,
  UINT8* pBufNewPwd,
  UINT8 nNewPwdSize,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SimGetAuthenticationStatus (
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SimEnterAuthentication (
  UINT8* pPin,
  UINT8 nPinSize,
  UINT8* pNewPin,
  UINT8 nNewPinSize,
  UINT8 nOption,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SimSetFacilityLock (
  UINT16 nFac,
  UINT8* pBufPwd,
  UINT8 nPwdSize,
  UINT8 nMode,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SimGetFacilityLock (
  UINT16 nFac,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SimGetPrefOperatorListMaxNum(
    UINT16 nUTI
){}

UINT32 SRVAPI CFW_SimGetPrefOperatorList(
    UINT16 nUTI
){}

UINT32 SRVAPI CFW_SimSetPrefOperatorList( 
    UINT8* pOperatorList,
    UINT8 nSize, 
    UINT16 nUTI
){} 

// SAT

#if 0
typedef struct _CFW_SAT_MENU_RSP_LIST 
{
	UINT8 nComID;
	UINT8 nComQualifier;
	UINT16 nAlphaLen;
	UINT8* pAlphaStr;
	CFW_SAT_ITEM_LIST * pItemList; //Link list
}CFW_SAT_MENU_RSP_LIST;
#endif

UINT32 SRVAPI CFW_SatActivation (
  UINT8 nMode,
  /*UINT8 nCharacter, */ 
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SatGetStatus (
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SatResponse (
  UINT8 nCmdType,
  UINT8 nStatus,
  UINT8 nItemId,
  PVOID pInputString,
  UINT8 InputStrLen,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SatGetInformation (
  UINT8 nCmdType,
  PVOID* pCmdData
){}

//
// SMS Service
//


#ifdef __MMI_MULTI_SIM__
PUBLIC UINT32 CFW_SmsDeleteMessage( 
  UINT16 nIndex,
  UINT8 nStatus,
  UINT8 nStorage,
  UINT8 nType,
  UINT16 nUTI,
  CFW_SIM_ID nSimId
){}
#else
UINT32 SRVAPI CFW_SmsDeleteMessage (
  UINT16 nIndex,
  UINT8 nStatus,
  UINT16 nStorage,
  UINT16 nUTI
){}
#endif

UINT32 SRVAPI CFW_SmsGetStorage (
  UINT16 nStorage,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SmsSetUnRead2Read ( 
  UINT16 nStorage,
  UINT16 nIndex,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SmsSetStorage (
  UINT16 nStorage1,
  UINT16 nStorage2,
  UINT16 nStorage3,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SmsListMessages ( 
  UINT16 nStorage,
  UINT8 nStatus,
  UINT16 nCount,
  UINT16 nStartIndex,
  UINT8 nOption,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SmsCopyMessages ( 
  UINT8 nStatus,
  UINT16 nCount,
  UINT16 nStartIndex,
  UINT8 nOption,
  UINT16 nUTI
){}

#ifdef __MMI_MULTI_SIM__
UINT32 CFW_SmsReadMessage(
	UINT8 nStorage, 
	UINT8 nType, 
	UINT16 nIndex, 
	UINT16 nUTI, 
	CFW_SIM_ID nSimId
){}
#else
UINT32 SRVAPI CFW_SmsReadMessage ( 
  UINT16 nStorage,
  UINT16 nIndex,
  UINT16 nUTI
){}
#endif

UINT32 SRVAPI CFW_SmsSetRead2UnRead ( 
      UINT16 nStorage,
      UINT16 nIndex,
      UINT16 nUTI
    ){}
UINT32 SRVAPI CFW_SmsSendMessage ( 
  CFW_DIALNUMBER* pNumber,
  UINT8* pData,
  UINT16 nDataSize,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SmsWriteMessage ( 
  CFW_DIALNUMBER* pNumber,
  UINT8* pData,
  UINT16 nDataSize,
  UINT16 nStorage,
  UINT16 nIndex,
  UINT8 nStatus,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_SmsGetMessageNode ( 
  PVOID pListResult,
  UINT16 nIndex,
  CFW_SMS_NODE **pNode
){}

UINT32 SRVAPI CFW_SmsListMessagesHeader ( 
  UINT16 nStorage,
  UINT8 nStatus,
  UINT16 nCount,
  UINT16 nStartIndex,
  UINT8 nOption,
  UINT16 nUTI
){}

#ifdef __MMI_MULTI_SIM__
PUBLIC UINT32 CFW_SmsSetUnSent2Sent(
  UINT8 nStorage,
  UINT8 nType,
  UINT16 nIndex,
  UINT8 nSendStatus,
  UINT16 nUTI,
  CFW_SIM_ID nSimId
){}
#else
UINT32 SRVAPI CFW_SmsSetUnSent2Sent(
    UINT16 nStorage,
    UINT16 nIndex,
    UINT8 nSendStatus,
    UINT16 nUTI
){}
#endif

UINT32 SRVAPI CFW_SmsQueryOverflowInd (
  UINT8* pMode,
  UINT8* pStatus
){}

UINT32 SRVAPI CFW_SmsNewSmsAck (
  UINT8 nCmd,
  UINT8* pData,
  UINT16 nSize
){}

UINT32 SRVAPI CFW_SmsComposePdu(
                         UINT8* pTpUd, // "ABC"
                         UINT16 nTpUdl, // 3
                         CFW_DIALNUMBER* pNumber, // telephone number to send, 135XXX, 
                         CFW_SMS_PDU_PARAM* pSmsParam,  // NULL
                         UINT32* pSmsData,              // ouput need to maloc.
                         UINT16* pSmsDataSize          // size
                         ){}

#ifdef CFW_PBK_SYNC_VER

UINT32 SRVAPI CFW_PbkAddEntry (
  CFW_PBK_ENTRY_HRD* pEntryHrd,
  CFW_PBK_OUT_PARAM* pOutParam
){}

UINT32 SRVAPI CFW_PbkUpdateEntry (
  CFW_PBK_ENTRY_HRD* pEntryHrd,
  CFW_PBK_OUT_PARAM* pOutParam
){} 

UINT32 SRVAPI CFW_PbkDeleteEntry (
  UINT8 nStorageId,
  UINT16 nPosition,
  CFW_PBK_OUT_PARAM* pOutParam
){}

UINT32 SRVAPI CFW_PbkDeleteBatchEntries (
  UINT16 nStartPos,
  UINT16 nCount,
  UINT8 nStorageId,
  CFW_PBK_OUT_PARAM* pOutParam
){}

UINT32 SRVAPI CFW_PbkFindEntry (
  UINT8* pString,
  UINT8 nStringSize,
  UINT16 nStartPos,
  UINT16 nCount,
  UINT8 nStorageId,
  UINT16 nGroup,
  UINT16 nOption,
  CFW_PBK_OUT_PARAM* pOutParam
){}

UINT32 SRVAPI CFW_PbkListEntries (
  UINT16 nStartPos,
  UINT16 nPageCount,
  UINT16 nSkipPage,
  UINT8 nStorageId,
  UINT16 nGroup,
  UINT8 nOption,
  CFW_PBK_OUT_PARAM* pOutParam
){}

UINT32 SRVAPI CFW_PbkGetEntry (
  UINT16 nPosition,
  UINT8 nStorageId,
  UINT8 nOption,
  CFW_PBK_OUT_PARAM* pOutParam
){}

UINT32 SRVAPI CFW_PbkCopyEntries (
  UINT16 nStartPos,
  UINT16 nCount,
  UINT8 nOption,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_PbkAddEntryItem (
  CFW_PBK_ENTRY_ITEM* pEntryItem,
  UINT8 nCount,
  CFW_PBK_OUT_PARAM* pOutParam
){}

UINT32 SRVAPI CFW_PbkGetEntryItems (
  UINT16 nPosition,
  UINT32 nItemGroupType,
  CFW_PBK_OUT_PARAM* pOutParam
){}

UINT32 SRVAPI CFW_PbkDeleteEntryItem (
  UINT16 nItemId,
  /*UINT32 nItemGroupType,*/
  CFW_PBK_OUT_PARAM* pOutParam
){}


UINT32 SRVAPI CFW_PbkGetCapacityInfo (
  UINT8 nStorageId,
  CFW_PBK_CAPACITY_INFO* pCapInfo 
){}

UINT32 SRVAPI CFW_PbkGetCalllogCapacityInfo (
  UINT8 nStorageId,
  CFW_PBK_CAPACITY_INFO* pCapInfo 
){}

UINT32 SRVAPI CFW_PbkAddCallLogEntry (
  CFW_PBK_CALLLOG_ENTRY* pEntryInfo,
  CFW_PBK_OUT_PARAM* pOutParam
){}

UINT32 SRVAPI CFW_PbkGetCallLogEntry (
  UINT16 nPosition,
  UINT8 nStorageId,
  CFW_PBK_OUT_PARAM* pOutParam
){}

UINT32 SRVAPI CFW_PbkListCallLogEntries (
  UINT16 nStartPosition,
  UINT16 nCount,
  UINT8 nStorage,
  CFW_PBK_OUT_PARAM* pOutParam
){}

UINT32 SRVAPI CFW_PbkDeleteBatchCallLogEntries (
  UINT16 nStartPos,
  UINT16 nCount,
  UINT8 nStorageId,
  CFW_PBK_OUT_PARAM* pOutParam
){}

UINT32 SRVAPI CFW_PbkGetCalllogCapacityInfo (
  UINT8 nStorageId,
  CFW_PBK_CAPACITY_INFO* pCapInfo 
){}
#else

UINT32 SRVAPI CFW_PbkAddEntry (
  CFW_PBK_ENTRY_HRD* pEntryHrd,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_PbkUpdateEntry (
  CFW_PBK_ENTRY_HRD* pEntryHrd,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_PbkDeleteEntry (
  UINT8 nStorageId,
  UINT16 nPosition,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_PbkDeleteBatchEntries (
  UINT16 nStartPos,
  UINT16 nCount,
  UINT8 nStorageId,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_PbkFindEntry (
  UINT8* pString,
  UINT8 nStringSize,
  UINT16 nStartPos,
  UINT16 nCount,
  UINT8 nStorageId,
  UINT16 nGroup,
  UINT16 nOption,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_PbkListEntries (
  UINT16 nStartPos,
  UINT16 nPageCount,
  UINT16 nSkipPage,
  UINT8 nStorageId,
  UINT16 nGroup,
  UINT8 nOption,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_PbkGetEntry (
  UINT16 nPosition,
  UINT8 nStorageId,
  UINT8 nOption,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_PbkCopyEntries (
  UINT16 nStartPos,
  UINT16 nCount,
  UINT8 nOption,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_PbkAddEntryItem (
  CFW_PBK_ENTRY_ITEM* pEntryItem,
  UINT8 nCount,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_PbkGetEntryItems (
  UINT16 nPosition,
  UINT32 nItemGroupType,
  UINT16 nUTI
){}
//
//modify by lixp
//
#if 0
UINT32 SRVAPI CFW_PbkDeleteEntryItem (
  UINT16 nItemId,
  UINT32 nItemGroupType,
  UINT16 nUTI
){}
#endif
UINT32 SRVAPI CFW_PbkDeleteEntryItem (
  UINT16 nItemId,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_PbkGetCapacityInfo (
  UINT8 nStorageId,
  CFW_PBK_CAPACITY_INFO* pCapInfo 
){}

UINT32 SRVAPI CFW_PbkGetCalllogCapacityInfo (
  UINT8 nStorageId,
  CFW_PBK_CAPACITY_INFO* pCapInfo 
){}

UINT32 SRVAPI CFW_PbkAddCallLogEntry (
  CFW_PBK_CALLLOG_ENTRY* pEntryInfo,
  UINT16 nUTI
){}

//
//modify by lixp
//
#if 0
UINT32 SRVAPI CFW_PbkGetCallLogEntry (
  UINT16 nPosition,
  UINT8 nStorageId,
  UINT16 nUTI
){}
#endif
UINT32 SRVAPI CFW_PbkGetCallLogEntry (
  UINT16 nPosition,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_PbkListCallLogEntries (
  UINT16 nStartPosition,
  UINT16 nCount,
  UINT8 nStorage,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_PbkDeleteBatchCallLogEntries (
  UINT16 nStartPos,
  UINT16 nCount,
  UINT8 nStorageId,
  UINT16 nUTI
){}

//jiashuo delete start
#if 0
UINT32 SRVAPI CFW_PbkGetCalllogCapacityInfo (
  UINT8 nStorageId,
  CFW_PBK_CAPACITY_INFO* pCapInfo 
){}
#endif
//jiashuo delete end

UINT32 SRVAPI CFW_PbkCancel (
  UINT16 nOption,
  UINT16 nUTI
){}

#endif 

//
// GPRS
//
#ifdef __GPRS_MODE__
//synchronization function
// AT+CGDCONT
UINT32 SRVAPI CFW_GprsSetPdpCxt(
    UINT8 nCid, 
    CFW_GPRS_PDPCONT_INFO *pPdpCont
){} // [in]nCid, [in]pPdpCont

UINT32 SRVAPI CFW_GprsGetPdpCxt(
    UINT8 nCid, 
    CFW_GPRS_PDPCONT_INFO *pPdpCont
){} // [in]nCid, [out]pPdpCont

// AT+CGQREQ
// This command allows the TE to specify a Quality of Service Profile 
// that is used when the MT sends an Activate PDP Context Request message 
// to the network.
UINT32 SRVAPI CFW_GprsSetReqQos(
    UINT8 nCid, 
    CFW_GPRS_QOS *pQos
    ){} // [in]nCid, [in]pQos
    
UINT32 SRVAPI CFW_GprsGetReqQos(
    UINT8 nCid, 
    CFW_GPRS_QOS *pQos){} // [in]nCid, [out]pQos

//AT+CGPADDR
//get the address of the selected Pdp context of the gprs network
//[in]nCid: [1-7]
// Specify the PDP Context Identifier (a numeric) which specifies a particular PDP context definition. 
//[in/out]nLength: [in] nLength should be the length that malloc the pdpAdd.
//                 [out] nLength should be the length of the Pdp address.
//[out]pPdpAdd: Point to the buffer to hold the PDP address. 
// return: If the function succeeds, the return value is ERR_SUCCESS, 
// if the function fails, the following error code may be returned.
// ERR_CFW_INVALID_PARAMETER, ERR_CME_UNKNOWN, 

UINT32 SRVAPI CFW_GprsGetPdpAddr( 
    UINT8 nCid, UINT8 *nLength,
    UINT8 *pPdpAdd
    ){}

// using net parameter
//AT+CGREG
// get the status of the gprs network.
// [out]pStatus: Pointer to CFW_NW_STATUS_INFO structure to contain the network status information
// return: If the function succeeds, the return value is ERR_SUCCESS, 
// if the function fails, the following error code may be returned.
// ERR_CFW_INVALID_PARAMETER, ERR_CME_UNKNOWN, 
UINT32 SRVAPI CFW_GprsGetstatus(
    CFW_NW_STATUS_INFO *pStatus
){}  // 

// AT+CGATT?
// retrieves the state of the GPRS service.
// [out]pState: Point to UINT8 to hold the state of GPRS attachment. 
// This parameter can be one of the following value:
// CFW_GPRS_DETACHED 0 detached 
// CFW_GPRS_ATTACHED 1 attached 
// return: If the function succeeds, the return value is ERR_SUCCESS, 
// if the function fails, the following error code may be returned.
// ERR_CFW_INVALID_PARAMETER, ERR_CME_UNKNOWN, 

UINT32 SRVAPI CFW_GetGprsAttState (
    UINT8* pState
){}

//
// AT+CGACT? 
// retrieves the state of the GPRS Service according to the selected cid
// [in]nCid:[1-7]
// Specify the PDP Context Identifier (a numeric) which specifies a particular PDP context definition. 
// [out]pState:Point to UINT8 to hold the state of GPRS activation.
// CFW_GPRS_DEACTIVED 0 deactivated 
// CFW_GPRS_ACTIVED 1 activated 
// return: If the function succeeds, the return value is ERR_SUCCESS, 
// if the function fails, the following error code may be returned.
// ERR_CFW_INVALID_PARAMETER, ERR_CME_UNKNOWN, 

UINT32 SRVAPI CFW_GetGprsActState (
    UINT8 nCid, 
    UINT8* pState
){}
#endif
// asynchronism
////------> move to Net

// AT+CGATT  
// ps attach and detach
// nState: 
// CFW_GPRS_DETACHED 0 detached 
// CFW_GPRS_ATTACHED 1 attached 
// event:EV_CFW_GPRS_ATT_NOTIFY
// parameter1: 0
// parameter2: 0
// type: 0 or 1 to distinguish the attach or detach
#ifdef __MMI_MULTI_SIM__

UINT32 CFW_GprsAtt(UINT8 nState, UINT16 nUTI, CFW_SIM_ID nSimID)
{

}

#else

// notify: success or fail*/
UINT32 CFW_GprsAtt(UINT8 nState, UINT16 nUTI)
{

}

#endif

#ifdef __GPRS_MODE__
// AT: AT+CGACT
// PDP context activate or deactivate
// nState:
// CFW_GPRS_DEACTIVED 0 deactivated 
// CFW_GPRS_ACTIVED 1 activated 
// event:EV_CFW_GPRS_ACT_NOTIFY
// parameter1:0
// parameter2:0
// type: 0 or 1 to distingush the active or deactive
UINT32 SRVAPI CFW_GprsAct(
    UINT8 nState, 
    UINT8 nCid, 
    UINT16 nUTI
){}     // [in] nCid  // notify: success or fail

UINT32 CFW_GprsActEx(UINT8 nState, UINT8 nCid, UINT16 nUTI, CFW_SIM_ID nSimID,BOOL SavedInApp)
{
}
//
// AT: ATA
// Manual acceptance of a network request for PDP context activation 'A'
// [in]nUTI:[1-7]
// Specify the UTI (Unique Transaction Identifier) operation,
// which support multi-application in parallel. This parameter will be 
// return in notification event (see below). You also can get a free UTI by 
// CFW_GetFreeUTI function if you application is a dynamical module. 
// This parameter should be less than 255 for all upper layer application.
// [in]nCid: Specify the PDP Context Identifier (a numeric) which specifies a particular PDP context definition.  
// response event: EV_CFW_GPRS_CXT_ACTIVE_IND_NOTIFY
// parameter1: 0
// parameter2: 0
// return: If the function succeeds, the return value is ERR_SUCCESS, 
// if the function fails, the following error code may be returned.
// ERR_CFW_INVALID_PARAMETER, ERR_CME_UNKNOWN, 
UINT32 SRVAPI CFW_GprsManualAcc(
    UINT16 nUTI, 
    UINT8 nCid
){}


//
// ATH
// Manual rejection of a network request for PDP context activation 'H'
// The function is used to reject the request of a network for GPRS PDP 
// context activation which has been signaled to the TE by the RING or CRING 
// unsolicited result code.
// [in]UTI:
// [in]nCid:
// return: If the function succeeds, the return value is ERR_SUCCESS, 
// if the function fails, the following error code may be returned.
// ERR_CFW_INVALID_PARAMETER, ERR_CME_UNKNOWN, 
// event: None
// parameter1: None 
// parameter2: None
UINT32 SRVAPI CFW_GprsManualRej(
    UINT16 nUTI,
    UINT8 nCid
){}


// modify needed
// The function is used to modify the specified PDP context with QoS parameter. 
// After this function has completed, the MT returns to V.25ter online data state. 
// If the requested modification for any specified context cannot be achieved, 
// an event with ERROR information is returned. 
//nCid: 
//[in]a numeric parameter which specifies a particular PDP context definition;
//
//nUTI: 
//[in]
//
//QOS:
//[in] Point to CFW_GPRS_QOS structure to set the Quality of Service.
// return: If the function succeeds, the return value is ERR_SUCCESS, 
// if the function fails, the following error code may be returned.
// ERR_CFW_INVALID_PARAMETER, ERR_CME_UNKNOWN, 

UINT32 SRVAPI CFW_GprsCtxModify(
    UINT16 nUTI, 
    UINT8 nCid, 
    CFW_GPRS_QOS *Qos
){}

// no response
// This message is sent by MMI to accept the PDP context modification requested by the
// network in an api_PdpCtxModifyInd message.
// return: If the function succeeds, the return value is ERR_SUCCESS, 
// if the function fails, the following error code may be returned.
// ERR_CFW_INVALID_PARAMETER, ERR_CME_UNKNOWN, 
// event: NO
UINT32 SRVAPI CFW_GprsCtxModifyAcc(
    UINT16 nUTI, 
    UINT8 nCid
){}

// return: If the function succeeds, the return value is ERR_SUCCESS, 
// if the function fails, the following error code may be returned.
// ERR_CFW_INVALID_PARAMETER, ERR_CME_UNKNOWN, 
// event: NO
UINT32 SRVAPI CFW_GprsCtxModifyRej(
    UINT16 nUTI, 
    UINT8 nCid
){}

//The function is used to send PDP data to the network. The implementation of
// this function is always called by upper protocol 
// send data   //UINT8 nCid, UINT16 nDataLength, UINT8 *pData
// nCid: 
//[in]a numeric parameter which specifies a particular PDP context definition;
//pGprsData: point to the data that need to send
// return: If the function succeeds, the return value is ERR_SUCCESS, 
// if the function fails, the following error code may be returned.
// ERR_CFW_INVALID_PARAMETER,
// event: None
UINT32 SRVAPI CFW_GprsSendData(
    UINT8 nCid, 
    CFW_GPRS_DATA * pGprsData
){}
#endif
//
//Tcpip
//
#ifdef __TCPIP__
UINT32 CFW_TcpipInetAddr(const INT8 *cp){}
UINT16 CFW_TcpipAvailableBuffer(UINT8 nSocket){}
UINT32 CFW_TcpipGetLastError(VOID){}
UINT32 CFW_TcpipSocket(UINT8 nDomain, UINT8 nType, UINT8 nProtocol){}
UINT32 CFW_TcpipSocketConnect(UINT8 nSocket,  CFW_TCPIP_SOCKET_ADDR *pName, UINT8 nNameLen){}
UINT32 CFW_TcpipSocketSend(UINT8 nSocket, UINT8 *pData, UINT16 nDataSize, UINT32 nFlags){}
UINT32 CFW_TcpipSocketRecv(UINT8 nSocket, UINT8* pData, UINT16 nDataSize, UINT32 nFlags){}
UINT32 CFW_TcpipSocketClose(UINT8 nSocket){}
#endif
// Shell 
//
UINT32 SRVAPI CFW_MeChangePassword (
  UINT16 nFac,
  UINT8* pBufPwd,
  UINT8  nPwdSize,
  UINT8  nMode,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_MeGetFacilityLock (
  UINT16 nFac,
  UINT16 nUTI
){}

UINT32 SRVAPI CFW_MeSetFacilityLock (
  UINT16 nFac,
  UINT8* pBufPwd,
  UINT8  nPwdSize,
  UINT8  nMode,
  UINT16 nUTI
){}


BOOL CFW_EmodGetBatteryInfo(CFW_EMOD_BATTERY_INFO* pBatInfo){}

void CFW_EmodAudioTestStart(UINT8 type){}			//type value: 0 is normal; 1 is mic AUX and speak AUX; 2 is mic loud and speak loud.	

void CFW_EmodAudioTestEnd(void){}

BOOL CFW_EmodClearUserInfo(void){}					//return TRUE is success,and FALSE is failure.

void CFW_EmodGetIMEI(UINT8* pImei,UINT8* pImeiLen){}				//pImei is the buffer that 

//
//bind 
//

void CFW_EmodReadBindSIM(CFW_EMOD_READ_BIND_SIM* pReadBindSim){}

void CFW_EmodReadBindNW(CFW_EMOD_READ_BIND_NW* pReadBindNw){}

void CFW_EmodGetBindInfo(BOOL* bSim,BOOL* bNetWork){}	//ture: had been bond.false:not bind	

UINT32 CFW_EmodSetBindInfo(BOOL bSim,BOOL bNetWork){}


CFW_EMOD_UPDATE_RETURN CFW_EmodUpdateSimBind(UINT8 nIndex,UINT8* pBindSimNum,UINT8 nLen){}		

CFW_EMOD_UPDATE_RETURN CFW_EmodUpdateNwBind(UINT8 nIndex,UINT8* pBindNwNum,UINT8 nLen){}	

//SN number

void CFW_EmodGetProductInfo(CFW_EMOD_LOCT_PRODUCT_INFO* pProduct){}

void CFW_EmodGetRFCalibrationInfo(CFW_EMOD_RF_CALIB_INFO *pRfCalib){}

/***************************************API FUNCTION OF TSM***********************************************/
#ifdef __MMI_MULTI_SIM__
UINT32 CFW_EmodOutfieldTestStart(CFW_TSM_FUNCTION_SELECT* pSelecFUN,UINT16 nUTI,CFW_SIM_ID nSimID){}
#else
UINT32 SRVAPI CFW_EmodOutfieldTestStart(CFW_TSM_FUNCTION_SELECT* pSelecFUN,UINT16 nUTI){}	
#endif

UINT32 SRVAPI CFW_EmodOutfieldTestEnd(UINT16 nUTI){}

/**********************************************************************************************************/

UINT32 ML_Init(){return 1;}
UINT32 ML_SetCodePage(UINT8 nCharset[12]){return 1;}

//Modify by zhuoxz at 20090602 for Mult task invoking conflict
UINT32 ML_LocalLanguage2Unicode(const UINT8* in, UINT32 in_len, UINT8** out, 
UINT32* out_len, UINT8 nCharset[12])
{
  UINT32 i=0;

  *out = COS_MALLOC(in_len*2);
  for (i=0;i<in_len;i++)
  {
    (*out)[i*2] = in[i];
    (*out)[i*2+1] = '\0';
  }
  return in_len*2;
}
//UINT32 ML_Unicode2LocalLanguage(const UINT8* in, UINT32 in_len, UINT8** out, 
//UINT32* out_len, UINT8 nCharset[12]){return 1;}
UINT32 ML_LocalLanguage2UnicodeBigEnding(const UINT8* in, UINT32 in_len, UINT8
** out, UINT32* out_len, UINT8 nCharset[12]){return 1;}


/* Add by zhuoxz,2009-4-29 */
/* start */
#ifdef __TCPIP__
UINT32 CFW_TcpipSocketBind(SOCKET nSocket,  CFW_TCPIP_SOCKET_ADDR *pName, UINT8 nNameLen)
{
  return 0;
}

UINT32 CFW_TcpipSocketAccept(SOCKET nSocket,  CFW_TCPIP_SOCKET_ADDR *addr, UINT32 * addrlen)
{
  return 0;
}

UINT32 CFW_TcpipSocketListen(SOCKET nSocket, UINT32 backlog)
{
  return 0;
}

UINT32 CFW_TcpipSocketSendto(SOCKET nSocket, VOID *pData, UINT16 nDataSize, UINT32 nFlags, CFW_TCPIP_SOCKET_ADDR *to, INT32 tolen)
{
  return 0;
}

UINT32 CFW_TcpipSocketRecvfrom(SOCKET nSocket, VOID *pMem, INT32 nLen, UINT32 nFlags, CFW_TCPIP_SOCKET_ADDR *from, INT32 * fromlen)
{
  return 0;
}

UINT32 CFW_Gethostbyname(char *hostName, struct ip_addr *addr)
{
  return 0;
}

UINT32 CFW_SetDnsServer(INT8 *PriDnsServer, INT8 *SecDnsServer)
{
  return 0;
}
#endif
#ifdef __MMI_MULTI_SIM__

UINT32 CFW_SetComm (
  CFW_COMM_MODE 	nMode, 
  UINT16  		nUTI, 
  CFW_SIM_ID 		nSimID
)
{

}

UINT32 CFW_GetComm (
  CFW_COMM_MODE* pMode, 
  CFW_SIM_ID 	nSimID
)
{

}

UINT32 SRVAPI CFW_CfgGetSmsStorage (
  UINT8* pStorage,
  CFW_SIM_ID nSimID
)
{

}

UINT32 SRVAPI CFW_SsQueryCallForwarding (
  UINT8 nReason,
  UINT8 nClass,
  UINT16 nUTI,
  CFW_SIM_ID nSimID
)
{

}

UINT32 CFW_GetCellInfo(CFW_TSM_CURR_CELL_INFO *pCurrCellInfo, CFW_TSM_ALL_NEBCELL_INFO *pNeighborCellInfo,CFW_SIM_ID nSimID)
{
  return 0;
}

#else

UINT32 CFW_GetCellInfo(CFW_TSM_CURR_CELL_INFO *pCurrCellInfo, CFW_TSM_ALL_NEBCELL_INFO *pNeighborCellInfo)
{

}
UINT32 SRVAPI CFW_CfgGetSmsStorage (UINT8* pStorage)
{

}
#endif

#ifdef __MMI_MULTI_SIM__
UINT32 CFW_GetFreeCID(UINT8* pCID, CFW_SIM_ID nSimID)
{
  return 0;
}
#else
UINT32 CFW_GetFreeCID(UINT8* pCID)
{
  return 0;
}
#endif
INT32 SUL_Decode7Bit(
                     PCSTR   pSrc,	            // The given SMS string.
                     PSTR    pDest,            // Output Decode SMS string.
                     UINT16  nSrcSize          // the given SMS sting size in bytes.
                     )
{
    return 0;
}

#ifdef __MMI_MULTI_SIM__	
UINT32 CFW_SmsInitComplete(UINT16 nUTI, CFW_SIM_ID nSimID)
{
    return 0;
}
UINT32 CFW_SetSMSConcat (BOOL bConcat, CFW_SIM_ID nSimID)
{
	return 0;
};
#else
UINT32 CFW_SmsInitComplete(UINT16 nUTI)
{
    return 0;
}
UINT32 CFW_SetSMSConcat (BOOL bConcat)
{
	return 0;
};
#endif

#ifdef __MMI_MULTI_SIM__
UINT32 CFW_SetQualReport(BOOL bStart,CFW_SIM_ID nSimID)
{
    return 0;
}
#else
UINT32 CFW_SetQualReport(BOOL bStart)
{
    return 0;
}
#endif
VOID bal_fstraceStart(void)
{
	return;
}
VOID bal_fstraceStop(void)
{
	return;
}

BOOL vid_is_audio_channel_available(void)
{
	return TRUE;
}

UINT32 CFW_CfgSetFirstPowerOnFlag(BOOL bFPO)
{
	return 0;
}
UINT32 CFW_CfgCheckFirstPowerOnFlag(BOOL *pbFPO)
{
	return 0;
}

UINT32 CFW_SetJmetocPwd(INT8 * pwd)
{
	return 0;
}

UINT32 CFW_SetJmetocUser(const INT8 *user)
{
	return 0;
}
UINT32 CFW_SetJmetocApn(const INT8 *apn)
{
	return 0;
}
UINT32 CFW_GetSimSPN(CFW_SIM_ID nSimID, CFW_SPN_INFO *pSPNInfo)
{
    return 0;
}
void med_free_ext_mem(void** pointer)
{
	free(*pointer);
	*pointer=NULL;
}
#ifdef CFW_MULTI_SIM
BOOL CFW_isPdpActiveAvailable(CFW_SIM_ID nSimID)
{
  return TRUE;
}

#else
BOOL CFW_isPdpActiveAvailable()
{
  return FALSE;
}
#endif
/* end */