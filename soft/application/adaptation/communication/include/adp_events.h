/******************************************************************************/
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    adp_Events.h                                                         */
/*                                                                            */
/* Description:                                                               */
/*     change message  management .                                            */
/*     @sharpen 2006-05-20.                                                       */
/******************************************************************************/
#ifndef _ADP_EVENTS_H
#define _ADP_EVENTS_H
#include "mmi_features.h"
#include <csw.h>
#include "mmi.h"

#include "app_ltlcom.h"

#ifdef __ADP_MULTI_SIM__
#define ADP_DUALSIM_MODE_CLOSE_NOSIM //when dual sim mode, close no sim to flight mode
#endif

#ifdef __ADP_MULTI_SIM__
//#define ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
#endif

//#define ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS

#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
typedef struct
{
	COS_EVENT *pCosEvent;
	void * pNext;
}Simu_Msg;

typedef struct
{
	U32 msgID;
	U8 Uti;
	U8 nSimID;
	void *pNext;
}Simu_Rsp_Marker_Struct;
extern void adp_simu_add_new_msg(COS_EVENT *pCosEvent);
extern void adp_simu_add_Rsp_Marker(Simu_Rsp_Marker_Struct *pMarker);
extern U8 adp_simu_get_sim_id_by_uti_for_rsp_msg(U32 nEventId, U8 uti, U8 Old_simID);
#endif

//macro
//#define   VIBRATE_TIME_ONCE_ID    95
//#define   VIBRATE_TIME_REPEAT_ID    94
#define   VIBRATE_TIME_DURATION	500
#define   VIBRATE_COUNT		4

#define MMI_TICK1S 16384

extern BOOL MMI_InitCfw(VOID);    
extern BOOL MMI_FreeEvent(COS_EVENT* pEvent);   
extern void MMI_Free_msg_buf(ilm_struct  *ilm_ptr);  //dyj add end
#if 0
void COS_KeyEventHandler(COS_EVENT* pCosEvent);
#else
extern void COS_KeyEventHandler(ilm_struct* Message);
#endif
extern void ADP_DispatchMessage(COS_EVENT* pCosEvent);
extern BOOL ADP_DispatchCcMsg(COS_EVENT* pCosEvent);
extern BOOL ADP_DispatchSimMsg(COS_EVENT* pCosEvent);
extern BOOL ADP_DispatchSimReqMsg(COS_EVENT* pCosEvent);
extern void InitSMS(UINT32 nParam1, UINT32 nParam2, UINT8 nType);
extern BOOL ADP_DispatchSmsMsg(COS_EVENT* pCosEvent);
extern BOOL ADP_DispatchDmRspMsg(COS_EVENT* pCosEvent);
extern BOOL ADP_DispatchUemIndMsg(COS_EVENT* pCosEvent);
extern BOOL ADP_DispatchUemRspMsg(COS_EVENT* pCosEvent);
extern BOOL ADP_DispatchNwMsg(COS_EVENT* pCosEvent); // dyj add 20060613
extern BOOL ADP_DispatchSmuMsg(COS_EVENT* pCosEvent);
extern BOOL ADP_DispatchSmuReqMsg(COS_EVENT* pCosEvent);
extern BOOL ADP_DispatchAudMsg(COS_EVENT *pCosEvent);
extern void Adp_SendMessageToMMI(local_para_struct *msg_ptr, msg_type_t msg_id, module_type src_mod_id);
extern BOOL ADP_DispatchGPRSMsg(COS_EVENT *pCosEvent);
extern void ADP_USSDParseDCS(UINT8 nInputData, UINT8* pAlphabet);
extern UINT8 ADP_GetFreeUti();
extern void SetBandFirstPowerOn();

#ifdef __ADP_MULTI_SIM__
extern void AdpInitCallFwowardingStart(UINT8 nSimId);
#else  /* No Defined __ADP_MULTI_SIM__ */
extern void AdpInitCallFwowardingStart();
#endif /* __ADP_MULTI_SIM__ */

//BEGIN: add by panxu for the distinguish different message types of pin1, pin2, puk1, puk2, 2006-11-26
typedef enum
{
	PHB_MSG,
	PHB_DAIL_MODE,
	PHB_CHECK_PIN2,
	PIN1_LOCK,
	PIN2_MSG,
	PUK1_LOCK,
	PUK2_LOCK,
	PHONE_MSG,
	CHANGE_PIN1_PWD,
	CHANGE_PIN2_PWD,
	CHECK_PIN1,
	VERIFY_PIN1, //used for verifying the pin1 in the poweron process
	VERIFY_PUK1 //used for verifying the puk1 in the poweron process
} message_type;
//END

typedef enum  
{

	ADP_NVRAM_READ_FAIL,
	ADP_NVRAM_READ_SUCCESS,
	ADP_NVRAM_ERROR_ACTUAL_READ_GREATER_THAN_REQUESTED_READ,
	ADP_NVRAM_ERROR_ACTUAL_READ_LESS_THAN_REQUESTED_READ,
	ADP_NVRAM_WRITE_SUCCESS,
	ADP_NVRAM_WRITE_FAIL

}ADPNVRAMERRORSENUM;

typedef  enum _ADP_MSG_ALPHABET
{ 
    ADP_MSG_GSM_7 = 0,
    ADP_MSG_8_BIT_DATA,
    ADP_MSG_UCS2,
    ADP_MSG_RESERVED,
}ADP_MSG_ALPHABET;


//void KeyOneHandler(void);
//void KeyTwoHandler(void);
extern U8 GetHandsetInPhone();
extern void SetHandsetInPhone(U8 h_status);
void dm_dealedKey(void);

typedef BOOL( *PFN_DATA_CONN_CALLBACK )( COS_EVENT *pEV );
typedef BOOL ( *PFN_TCPIP_CALLBACK )( COS_EVENT *pEV );
extern UINT32 CFW_SetDataConnFun(PFN_DATA_CONN_CALLBACK pfnEv);
extern UINT32 CFW_SetTCPIPCallBack ( PFN_TCPIP_CALLBACK pfnEv ) ;
#ifdef __ADP_MULTI_SIM__
extern UINT32 CFW_SetDataConnFunEx ( PFN_DATA_CONN_CALLBACK pfnEv, UINT8 SimId,UINT8 Cid );
#else
extern UINT32 CFW_SetDataConnFunEx ( PFN_DATA_CONN_CALLBACK pfnEv, UINT8 Cid );
#endif
extern UINT32 CFW_SetTCPIPCallBackEx ( PFN_TCPIP_CALLBACK pfnEv, UINT8 SocketId );


#define ADP_MAX_EVENT 500									

typedef BOOL (*AdpEvHandlerPtr)(COS_EVENT *);

typedef struct _ADP_EVENT_INFO_T
{
	UINT32 EventID;
	AdpEvHandlerPtr EntryFuncPtr;
} ADP_EVENT_INFO_T;

extern BOOL ADP_EventHandler(COS_EVENT *MsgStruct);

#define adp_GetCswEvType(pev) (HIUINT8((pev)->nParam3))
typedef enum
{
	ADP_SIM_1,
	ADP_SIM_2,
	ADP_SIM_3,
	ADP_SIM_4,
	ADP_SIM_MAX,
	ADP_SIM_ERROR = ADP_SIM_MAX,
} ADP_SIM_ID_T;

#define FREE_UTI  (ADP_GetFreeUti())

#ifdef __ADP_MULTI_SIM__

typedef enum
{
	ADP_MODULE_COMMON,
	ADP_MODULE_SMS,
	ADP_MODULE_CC,
	ADP_MODULE_PHB,
	ADP_MODULE_NW,
	ADP_MODULE_SIM,
	ADP_MODULE_SS,
	ADP_MODULE_SAT,
	ADP_MODULE_CALL_LOG,
	ADP_MODULE_WAP,
	ADP_MODULE_MAX,
} ADP_MODULE;


#define ADP_GET_MMI_DEST_MOD(nParam3) (UINT8)nParam3
#define ADP_GET_CSW_FLAG(nParam3) (UINT8)nParam3

// This macro to get flag information from parameter nParam3 of COS_EVENT structure.
// Low 8 bits is.
#define adp_GetCswEvFlag(pev) (LOUINT8((pev)->nParam3))
#define adp_GetCswEvFlag2(p3) (LOUINT8(p3))
// This macro to get type information from parameter nParam3 of COS_EVENT structure.
// High 8 bits is.
#define adp_GetCswEvType(pev) (HIUINT8((pev)->nParam3))
#define adp_GetCswEvType2(p3) (HIUINT8(p3))


extern UINT8 adp_GetCSWSimID(UINT8 cswFlag);
extern UINT8 adp_GetMMISimID(UINT8 destMod);
extern void adp_ConfigureContext(ADP_MODULE module, UINT8 simID);
#ifdef ADP_DUALSIM_MODE_CLOSE_NOSIM 
extern void ADP_CloseNoSIM(void);
extern U8 ADP_ReopNoSIM(void);
extern void ADP_InitCloseNoSIM(void);
#endif
extern UINT32 adp_PrepareGPRS( UINT32 arg_simid, UINT8 apnsize, char *apnName);
extern UINT32  soc_StopGPRS();

extern U8 ADP_GetCfwDmSetting(void);

////////////////////////////////////////
//  ADP common.

#define adp_SimId2IlmMod(id) (MOD_L4C + id)

////////////////////////////////////////////
//CC

typedef struct _ADP_CC_CONTEXT_T
{
	ADP_SIM_ID_T sim_id;
	// TODO: add dual-sim related global variables here...
} ADP_CC_CONTEXT_T;

////////////////////////////////////////////

//[221] Added by ZXB 2009/02/06								[BEGIN]
// Add ADP common context data structure and global var for dual-sim.

typedef struct _ADP_COMMON_CONTEXT_T
{
	ADP_SIM_ID_T sim_id;
	// TODO: add dual-sim related global variables here...
} ADP_COMMON_CONTEXT_T;

//[221] Added by ZXB 2009/02/06								[END]

typedef struct _adp_sim_context_t
{
	ADP_SIM_ID_T sim_id;
	// TODO: add dual-sim related global variables here...
} adp_sim_context_t;

#endif  //// __ADP_MULTI_SIM__

// This macro to get type information from parameter nParam3 of COS_EVENT structure.
// High 8 bits is.

typedef struct _adp_phb_context_t
{
	UINT16 	PB_SIM_RECORDID[260];
	UINT16 	nSIMusedSlot;
	UINT16 	nSIMTotalSlot;
	UINT16 	nSimIndex;
	BOOL   	flag_list_FDN;
#ifdef __ADP_MULTI_SIM__
	ADP_SIM_ID_T 	nSimId;
#endif /* __ADP_MULTI_SIM__ */
	UINT16 nFDNusedSlot;
	UINT16 nFDNtotalSlot;
}adp_phb_context_t;



typedef struct _adp_sat_context_t
{
#ifdef __ADP_MULTI_SIM__
	ADP_SIM_ID_T sim_id;
#endif /* __ADP_MULTI_SIM__ */
	UINT32 gCurrSatReqId;
	// TODO: add dual-sim related global variables here...
} adp_sat_context_t;

// structure of sms context
typedef struct _adp_sms_context_t
{
	UINT8 nWriteMessageStatus;
	BOOL bSIMFull;
	BOOL bMEFull;
	UINT8 nCurrentStorage;
	UINT8 adpDelBitmap[480];
	UINT16 nIndexCount;
	UINT16 nSMSCountMe;
	UINT16 nSMSCountSIM;
	UINT16 nSMSInME;
	UINT16 nSMSInSM;
	UINT16 SMSListState;
	UINT16 nMsgCount;
	UINT16 nListIndex;
	UINT16 nUsedMessageME;
	UINT16 nUsedMessageSM;
#ifdef __ADP_MULTI_SIM__
	ADP_SIM_ID_T nSimId;
#endif /* __ADP_MULTI_SIM__ */
	UINT32 nStartupBeginMsgId;
	UINT32 nStartupReadMsgId;
	UINT32 nStartupFinishMsgId;
	UINT32 nStartupReadyMsgId;
	UINT16 nStartIndexInMap;
}adp_sms_context_t;

////////////////////////////////////////////
//SS
typedef struct _ADP_SS_CONTEXT_T
{
#ifdef __ADP_MULTI_SIM__
	ADP_SIM_ID_T sim_id;
#endif /* __ADP_MULTI_SIM__ */
	UINT8 forwarding_state;
	// TODO: add dual-sim related global variables here...
} ADP_SS_CONTEXT_T;



////////////////////////////////////////////
//CHIS
typedef struct _ADP_CHIS_CONTEXT_T
{
	ADP_SIM_ID_T sim_id;
	U8	nRecordId;
	// TODO: add dual-sim related global variables here...
} ADP_CHIS_CONTEXT_T;

////////////////////////////////////////////
//WAP

typedef struct _adp_wap_context_t
{
	ADP_SIM_ID_T sim_id;
	// TODO: add dual-sim related global variables here...
} adp_wap_context_t;
////////////////////////////////////////////

#ifdef __ADP_MULTI_SIM__
#define ADP_PHB_MSG_SRC_MOD adp_SimId2IlmMod(adp_pPHBContext->nSimId)
#else
#define ADP_PHB_MSG_SRC_MOD MOD_L4C
#endif

#ifdef __ADP_MULTI_SIM__
extern void DummyInitSMSForNoSim(UINT32 nParam1, UINT32 nParam2, UINT8 nType);
extern void ADP_PhbInitFinishOneCardStartAnother(ADP_SIM_ID_T SimId);
extern void Adp_Get_FDN_State(ADP_SIM_ID_T SimId);
#endif

#ifdef __ADP_MULTI_SIM__
#define ADP_PHB_MSG_SRC_MOD adp_SimId2IlmMod(adp_pPHBContext->nSimId)
#define ADP_SAT_MSG_SRC_MOD adp_SimId2IlmMod(g_pSATContext->sim_id)
#define ADP_SMS_MSG_SRC_MOD adp_SimId2IlmMod(adp_gSMSContext->nSimId)
#define ADP_SS_MSG_SRC_MOD adp_SimId2IlmMod(g_pSSContext->sim_id)
#define ADP_COMM_MSG_SRC_MOD adp_SimId2IlmMod(g_pCommonContext->sim_id)
#define ADP_NW_MSG_SRC_MOD adp_SimId2IlmMod(g_pNwContext->sim_id)
#define ADP_SIM_MSG_SRC_MOD adp_SimId2IlmMod(g_pSIMContext->sim_id)
#define ADP_CSET_MSG_SRC_MOD adp_SimId2IlmMod(g_callset_cntx_p->nSimId)
#define ADP_CHIS_MSG_SRC_MOD adp_SimId2IlmMod(g_pCHISContext->sim_id)
#ifdef __GPRS_MODE__
#define ADP_WAP_MSG_SRC_MOD adp_SimId2IlmMod(adp_pWAPContext->sim_id)
#endif
// TODO: add more modules'...
#else
#define ADP_PHB_MSG_SRC_MOD MOD_L4C
#define ADP_SAT_MSG_SRC_MOD MOD_L4C
#define ADP_SMS_MSG_SRC_MOD MOD_L4C
#define ADP_SS_MSG_SRC_MOD MOD_L4C
#define ADP_COMM_MSG_SRC_MOD MOD_L4C
#define ADP_NW_MSG_SRC_MOD MOD_L4C
#define ADP_SIM_MSG_SRC_MOD MOD_L4C
#define ADP_CSET_MSG_SRC_MOD MOD_L4C
#define ADP_CHIS_MSG_SRC_MOD MOD_L4C
#define ADP_WAP_MSG_SRC_MOD MOD_L4C
// TODO: add more modules'...
#endif
#endif

