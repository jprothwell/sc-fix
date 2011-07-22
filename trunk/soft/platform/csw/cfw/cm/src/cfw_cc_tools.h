
#define CFW_CM_NORMAL_CLEARING 16
#define CFW_CM_USER_BUSY    17 
#define CFW_CM_CALL_REJECTD    21
#define CFW_CM_HOLDMPTY_UTI 1


#define CC_STATE_NULL       0x00
#define CC_STATE_ACTIVE     0x01
#define CC_STATE_HOLD       0x02
#define CC_STATE_WAITING    0x04
#define CC_STATE_INCOMING   0x08
#define CC_STATE_DIALING    0x10
#define CC_STATE_ALERTLING  0x20
#define CC_STATE_RELEASE    0x40

VOID CallingPartyBCD2String(UINT8* pcIn, UINT8* pcOut, UINT8 iLength);
UINT8 CallingPartyrRaw2BCD(UINT8 *pcIn, UINT8 *pcOut, UINT8 iLength);
VOID String2BCD(UINT8 *pcIn, UINT8* pcOut, UINT8 iLength);
VOID CC_ReleaseCmplt( HANDLE hAo, UINT8    nCause);
VOID CC_ProcNwDisconnectMsg(HAO hAo, CM_SM_INFO* proc, VOID* nEvParam);
VOID CC_ProcNwReleaseMsg(HAO hAo, CM_SM_INFO* proc, UINT8 iCauseValue);
VOID CC_ProcNwReleaseCmplMsg(HAO hAo, CM_SM_INFO* proc, UINT8 iCauseValue);
VOID CC_ProcHeldStateCHLD2X(HAO hSm);
BOOL IsDTMFChar(INT8 iTone);
#ifndef CFW_MULTI_SIM
VOID CC_Release( CM_SM_INFO* proc, UINT8    nCause);
VOID CC_Disc( CM_SM_INFO* proc, UINT8    nCause);
VOID CC_CallConfirm(UINT8 nTI, UINT8 nCause);
VOID CC_Alert(UINT8 nTI);
UINT32 CC_RejectCall ( VOID);
VOID CC_Facility(CM_SM_INFO* proc, UINT32 nOperationCode);
VOID CC_TerminateOne(UINT32 nState, UINT32 nSubState, UINT8 nHoldFunction);
VOID CC_TerminateMulti(UINT32 nState,UINT32 nSubState, UINT8 nHoldFunction);
VOID CC_RetrieveCall(HAO hOldSm);
VOID CC_HoldCall(VOID);
VOID CC_HoldMPTY(VOID);
VOID CC_Hold_AcceptCall(VOID);
VOID CC_HoldMPTY_Acceptcall(VOID);
VOID CC_RetrieveMPTY(VOID);
VOID CC_StartDTMF( CM_SM_INFO* proc, UINT8 iDtmf);
VOID CC_StopDTMF(CM_SM_INFO* proc);
UINT8 CC_GetIndex();
VOID CC_ProcMeetingStateCHLD2X(HAO hMeetingSm, UINT8 nIndex);
VOID CC_ProcMeetingHeldStateCHLD2X(HAO hMeetingHeldSm, UINT8 nIndex);
VOID CC_ModifyOtherAoState(UINT8 nCurState, UINT8 nLastStae, BOOL bOneTimes);
VOID CC_ModifyToMeetingHldState(HAO hAo, UINT8 nCurState, UINT8 nLastStae);
VOID CC_SendCrssInfoNotify(api_CrssFacilityIndIE_t *pFacilityInd);
#else
VOID CC_Release( CM_SM_INFO* proc, UINT8    nCause, CFW_SIM_ID nSimId);
VOID CC_Disc( CM_SM_INFO* proc, UINT8    nCause, CFW_SIM_ID nSimId);
VOID CC_CallConfirm(UINT8 nTI, UINT8 nCause, CFW_SIM_ID nSimId);
VOID CC_Alert(UINT8 nTI, CFW_SIM_ID nSimId);
UINT32 CC_RejectCall (CFW_SIM_ID nSimId);
VOID CC_Facility(CM_SM_INFO* proc, UINT32 nOperationCode, CFW_SIM_ID nSimId);
VOID CC_TerminateOne(UINT32 nState, UINT32 nSubState, UINT8 nHoldFunction, CFW_SIM_ID nSimId);
VOID CC_TerminateMulti(UINT32 nState,UINT32 nSubState, UINT8 nHoldFunction, CFW_SIM_ID nSimId);
VOID CC_RetrieveCall(HAO hOldSm, CFW_SIM_ID nSimId);
VOID CC_HoldCall(CFW_SIM_ID nSimId);
VOID CC_HoldMPTY(CFW_SIM_ID nSimId);
VOID CC_Hold_AcceptCall(CFW_SIM_ID nSimId);
VOID CC_HoldMPTY_Acceptcall( CFW_SIM_ID nSimId);
VOID CC_RetrieveMPTY(CFW_SIM_ID nSimId);
VOID CC_StartDTMF( CM_SM_INFO* proc, UINT8 iDtmf, CFW_SIM_ID nSimId);
VOID CC_StopDTMF(CM_SM_INFO* proc, CFW_SIM_ID nSimId);
UINT8 CC_GetIndex(CFW_SIM_ID nSimId);
VOID CC_ProcMeetingStateCHLD2X(HAO hMeetingSm, UINT8 nIndex, CFW_SIM_ID nSimId);
VOID CC_ProcMeetingHeldStateCHLD2X(HAO hMeetingHeldSm, UINT8 nIndex, CFW_SIM_ID nSimId);
VOID CC_ModifyOtherAoState(UINT8 nCurState, UINT8 nLastStae, BOOL bOneTimes, CFW_SIM_ID nSimId);
VOID CC_ModifyToMeetingHldState(HAO hAo, UINT8 nCurState, UINT8 nLastStae, CFW_SIM_ID nSimId);
VOID CC_SendCrssInfoNotify(api_CrssFacilityIndIE_t *pFacilityInd, CFW_SIM_ID nSimId);
#endif




