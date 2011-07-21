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
/*       dual_sim.h                                                           */
/*                                                                            */
/* DESCRIPTION                                                                */
/*   MTPNP layer header file, includes definition of macros and               */
/*   data structures and API functions.                                       */
/*                                                                            */
/******************************************************************************/

#ifndef __DUAL_SIM_H__
#define __DUAL_SIM_H__

#ifdef MMI_ON_WIN32
#include "mmi_features.h"
#endif

#ifdef __MMI_MULTI_SIM__

#ifdef MMI_ON_WIN32
#include "app_ltlcom.h"
#endif

////////////////////////////////////////
// Enum and macro definitions.
#define MTPNP_NULL 0

#define DM_STATUS_MASK 0x01

typedef char				MTPNP_CHAR;
typedef signed char		MTPNP_INT8;
typedef signed short		MTPNP_INT16;
typedef signed long		MTPNP_INT32;
typedef signed int			MTPNP_INT;
typedef unsigned char		MTPNP_UINT8;
typedef unsigned short		MTPNP_UINT16;
typedef unsigned long		MTPNP_UINT32;
typedef unsigned char		MTPNP_UCHAR;
typedef unsigned int		MTPNP_UINT;

typedef enum
{
	MTPNP_FALSE = 0,
	MTPNP_TRUE = 1
} MTPNP_BOOL;

typedef enum
{
	MMI_SIM_1,
	MMI_SIM_2,
	MMI_SIM_3,
	MMI_SIM_4,
	MMI_SIM_MAX,
	MMI_SIM_ERROR = MMI_SIM_MAX,
} MMI_SIM_ID_T;

typedef enum
{
	MTPNP_AD_FLIGHT = 0x00,     /* xxxx xxxx */
	MTPNP_AD_CARD1 = 0x01,      /* xxxx xxx1 */
	MTPNP_AD_CARD2 = 0x02,      /* xxxx xx1x */
	MTPNP_AD_DUALCARD = 0x03,   /* xxxx xx11 */
	MTPNP_AD_CARD3 = 0x04,
	MTPNP_AD_CARD4 = 0x08,	

	MTPNP_AD_MAX = 0xff
} E_MTPNP_AD_STATUS;

typedef enum
{
	MTPNP_AD_SIMCARD_NOT_INIT = 0x0,
	MTPNP_AD_SIMCARD_INVALID = 0x01,    /* xxxx xxxx xxxx xxx1 */
	MTPNP_AD_SIMCARD_ABSENT = 0x02,
	MTPNP_AD_SIMCARD_CHV1_REQUESTED = 0x04,
	MTPNP_AD_SIMCARD_CHVBLOCKED = 0x08,

	MTPNP_AD_SIMCARD_VALID = 0x10,

	MTPNP_AD_SIMCARD_PIN_CANCELED = 0x20,
	MTPNP_AD_SIMCARD_PUK_CANCELED = 0x40
} E_MTPNP_AD_SIMCARD_STATUS;

typedef enum    //((MTPNP_AD_CARD1 | MTPNP_AD_SIMCARD_VALID) + (MTPNP_AD_CARD2 | (MTPNP_AD_SIMCARD_VALID << 1)))
{
	// 0000 0000
	MTPNP_AD_DUALSIM_UNUSABLE, // = MTPNP_AD_FLIGHT,
	// 0000 0001
	MTPNP_AD_SIMCARD1_NOSIM_USABLE, // = MTPNP_AD_CARD1,
	// 0000 0010
	MTPNP_AD_SIMCARD2_NOSIM_USABLE, // = MTPNP_AD_CARD2,
	// 0000 00xx
	MTPNP_AD_SIMCARD_NOSIM_USABLE, // = MTPNP_AD_CARD1 | MTPNP_AD_CARD2,
	// 0001 00xx
	MTPNP_AD_SIMCARD1_USABLE, // = MTPNP_AD_CARD1 | MTPNP_AD_SIMCARD_VALID,
	// 0010 00xx
	MTPNP_AD_SIMCARD2_USABLE, // = MTPNP_AD_CARD2 | (MTPNP_AD_SIMCARD_VALID << 1),
	// 0011 0011
	MTPNP_AD_DUALSIM_USABLE, // = (MTPNP_AD_CARD1 | MTPNP_AD_SIMCARD_VALID) + (MTPNP_AD_CARD2 | (MTPNP_AD_SIMCARD_VALID << 1))
} E_MTPNP_AD_SIMCARD_USABLE_SIDE;

typedef enum
{
	MTPNP_AD_NVRAM_FLIGHT = 0x00,
	MTPNP_AD_NVRAM_CARD1 = 0x01,
	MTPNP_AD_NVRAM_CARD2 = 0x02,
	MTPNP_AD_NVRAM_DUALCARD = 0x3,
	MTPNP_AD_NVRAM_CARD3 = 0x04,
	MTPNP_AD_NVRAM_CARD4 = 0x08
} E_MTPNP_AD_NVRAM_STATUS;


typedef struct
{
	LOCAL_PARA_HDR
	kal_uint8	new_sim_config;
} mmi_smu_sim_status_update_req_struct;

typedef struct
{
	LOCAL_PARA_HDR
	kal_bool	is_sim_inserted;
	kal_uint8	dual_sim_mode_setting;
} mmi_smu_sim_status_update_ind_struct;

// the enum of dual mode channel
typedef enum
{
	MTPNP_AD_CHANNEL_MASTER,
	MTPNP_AD_CHANNEL_SLAVE,
	MTPNP_AD_CHANNEL_SIM3,
	MTPNP_AD_CHANNEL_SIM4,
	MTPNP_AD_CHANNEL_DEFAULT_MASTER,
	MTPNP_AD_CHANNEL_DEFAULT_SLAVE,
	MTPNP_AD_CHANNEL_DEFAULT_SIM3,
	MTPNP_AD_CHANNEL_DEFAULT_SIM4,

	MTPNP_AD_CHANNEL_NONE = 0xffff
} E_MTPNP_AD_CHANNELS;

typedef enum
{
	MTPNP_AD_CALL_CHANNEL, //= 0x01,
	MTPNP_AD_SMS_CHANNEL, //= 0x02,

	MTPNP_AD_DATA_SERVICE_CHANNEL, //= 0x04,
	MTPNP_AD_AUDIO_RECORD, //= 0x08,

	MTPNP_AD_CHANNEL_TOTAL
} E_MTPNP_AD_CHANNELS_OWNER;

typedef void (*MTPNP_EVENTHANDLER_FuncPtr)(void *);

typedef enum
{
	MTPNP_AD_POWERON_KEYPAD = 0,
	MTPNP_AD_POWERON_ALARM,
	MTPNP_AD_POWERON_CHARGER_IN,
	MTPNP_AD_POWERON_EXCEPTION,
	MTPNP_AD_POWERON_USB,
	MTPNP_AD_POWERON_UNINIT,
	MTPNP_AD_POWERON_PRECHARGE,

	MTPNP_AD_POWERON_MAX
} E_MTPNP_AD_POWERON_MODE;

typedef enum
{
	MTPNP_AD_DIRECT_SWITCH = 0,
	MTPNP_AD_SWITCH_SUCCEED = 1,
	MTPNP_AD_SWITCH_FAILED = 2
} E_MTPNP_AD_MODE_SWITCH_STATE;

#define MTPNP_AD_MAX_PLMN_LEN 6
#define SLAVE_NETWORK_ID_LEN 6

typedef struct
{
	MTPNP_UINT8 opcode;
	MTPNP_UINT8 oper[MTPNP_AD_MAX_PLMN_LEN + 1];
} ST_MTPNP_AD_SET_PLMN_REQ;

typedef struct
{
	MTPNP_UINT8 index;
	MTPNP_UINT16 num;
	MTPNP_UINT16 file_idx_req;
} ST_MTPNP_AD_GET_PREFERRED_NETWORK_REQ;

// plmn begin

typedef struct
{
	MTPNP_UINT8 index;
	MTPNP_UINT8 opcode;
	MTPNP_UINT8 oper[MTPNP_AD_MAX_PLMN_LEN + 1];
	MTPNP_UINT8 rat;
} ST_MTPNP_AD_SET_PREFERRED_NETWORK_REQ;

typedef enum
{
	MTPNP_STORAGE_TYPE_NONE,
	MTPNP_STORAGE_TYPE_SIM,
	MTPNP_STORAGE_TYPE_NATIVE,
	MTPNP_STORAGE_TYPE_ANY
} E_MTPNP_STORAGE_TYPE;

typedef struct
{
	E_MTPNP_STORAGE_TYPE storage;	/* storage type */
	MTPNP_UINT8 max_name_len;		 
	MTPNP_UINT8 max_number_len;		 
	MTPNP_UINT16 capacity;			/* capacity of phonebook */
} ST_MTPNP_DATA_PB_CAPACITY;

// Define coding scheme
typedef enum
{
	MTPNP_CODING_SCHEME_DEFAULT,
	MTPNP_CODING_SCHEME_ASCII,
	MTPNP_CODING_SCHEME_UCS2,
	MTPNP_CODING_SCHEME_GB2312,
	MTPNP_CODING_SCHEME_BIG5,
	MTPNP_CODING_SCHEME_STREAM,
	MTPNP_CODING_SCHEME_UNKNOWN
} E_MTPNP_CODING_SCHEME;

#define MTPNP_AD_MAX_CC_ADDR_LEN 	41

// message setup begin

typedef struct
{
	MTPNP_UINT8 name_length;
	MTPNP_UINT8 name_dcs;
	MTPNP_UINT8 name[32];
} ST_MTPNP_AD_NAME;

typedef struct
{
	MTPNP_UINT8 type;
	MTPNP_UINT8 length;
	MTPNP_UINT8 number[MTPNP_AD_MAX_CC_ADDR_LEN];
} ST_MTPNP_AD_PHONE_NUMBER;

typedef struct
{
	MTPNP_UINT16 para_ind;
	MTPNP_UINT8 profile_no;
	MTPNP_UINT8 pid;
	MTPNP_UINT8 dcs;
	MTPNP_UINT8 vp;
	ST_MTPNP_AD_PHONE_NUMBER sc_addr;
	ST_MTPNP_AD_NAME profile_name;
} ST_MTPNP_AD_SMS_SET_PROFILE_PARAMS_REQ;

typedef struct
{
	MTPNP_UINT16 para_ind;
	MTPNP_UINT8 fo;
	MTPNP_UINT8 bearer_service;
	MTPNP_UINT8 status_report;
	MTPNP_UINT8 reply_path;
} ST_MTPNP_AD_SMS_SET_COMMON_PARAMS_REQ;

typedef struct
{
	MTPNP_UINT8 mem1;
	MTPNP_UINT8 mem2;
	MTPNP_UINT8 mem3;
} ST_MTPNP_AD_SMS_SET_PREFER_STORAGE_REQ;

typedef enum
{
	MTPNP_CM_PHS_MODULE,
	MTPNP_CM_GSM_MODULE,
	MTPNP_PC_SIMULATOR,
	MTPNP_CALIBRATION_TOOLS,
	MTPNP_UNKNOWN_MODULE
} E_MTPNP_SLAVE_MODULE_TYPE;

/* sms show begin */
typedef enum
{
	MTPNP_AD_FILTER_MASTER,
	MTPNP_AD_FILTER_SLAVE,
	MTPNP_AD_FILTER_SIM3,
	MTPNP_AD_FILTER_SIM4,
	MTPNP_AD_FILTER_ALL,
} E_MTPNP_AD_FILTER_RULE;

typedef enum
{
	MTPNP_AD_SHOW_MIXED,
	MTPNP_AD_SHOW_DIVIDED
} E_MTPNP_AD_SHOW_STYLE;
/* sms show end */

typedef enum
{
	DM_MASTER_BOX = 0x01,
	DM_SLAVE_BOX = 0x02,
	DM_SIM3_BOX = 0x04,
	DM_SIM4_BOX = 0x08,
	DM_MIX_BOX = 0x0f,

	DM_INVALID_BOX = 0xffff
} E_DUALMODE_MSGS_BOX;

////////////////////////////////////////
// Public APIs

extern void MTPNP_AD_Init(void);
extern void MTPNP_PFAL_Init(void);
extern E_MTPNP_AD_STATUS MTPNP_AD_Get_Status(void);
extern BOOL MTPNP_AD_Is_Flight_Mode(void);
extern E_MTPNP_AD_NVRAM_STATUS MTPNP_AD_Get_Startup_Mode(void);
extern void MTPNP_AD_Init_BootupDM(E_MTPNP_AD_NVRAM_STATUS dm);
extern E_MTPNP_AD_CHANNELS MTPNP_AD_Set_Channel(E_MTPNP_AD_CHANNELS_OWNER owner, E_MTPNP_AD_CHANNELS channel);
extern E_MTPNP_AD_CHANNELS MTPNP_AD_Get_Channel(E_MTPNP_AD_CHANNELS_OWNER owner);
extern void MTPNP_AD_InitChannel(void);
extern BOOL MTPNP_AD_IsMasterChannel(E_MTPNP_AD_CHANNELS_OWNER owner);
extern void MTPNP_AD_SetProtocolEventHandler(PsFuncPtr funcPtr, U32 eventID, E_MTPNP_AD_CHANNELS_OWNER owner);
extern void MTPNP_PFAL_Rearrange_StatusBar(void);
extern void MTPNP_AD_Free_Channel(E_MTPNP_AD_CHANNELS_OWNER owner);
extern void MTPNP_PFAL_Register_Event_Handler(MTPNP_UINT32 eventID, MTPNP_EVENTHANDLER_FuncPtr funcptr);
extern void MTPNP_PFAL_Remove_Event_Handler(MTPNP_UINT32 eventID);
extern void *MTPNP_OSAL_malloc(const MTPNP_UINT size);
extern void MTPNP_OSAL_memset(void *dest, MTPNP_UINT8 value, MTPNP_UINT len);
extern void MTPNP_OSAL_memcpy(void *dest, const void *src, const MTPNP_UINT len);
extern void MTPNP_OSAL_memfree(void *ptrBlock);
extern MTPNP_INT MTPNP_PFAL_Unicode_String_Length(const MTPNP_CHAR * str);
extern MTPNP_CHAR *MTPNP_PFAL_Unicode_String_Copy(MTPNP_CHAR * dest, const MTPNP_CHAR * src);
extern MTPNP_CHAR *MTPNP_PFAL_Unicode_Strcat(MTPNP_CHAR * dest, const MTPNP_CHAR * src);
extern MTPNP_UINT16 MTPNP_PFAL_Ansii_To_Unicode_String(MTPNP_CHAR * dest, MTPNP_CHAR * src);
extern MTPNP_UINT16 MTPNP_PFAL_Unicode_To_Ansii_String(MTPNP_CHAR * dest, MTPNP_CHAR * src);
extern void MTPNP_PFAL_Hide_StatusICON(MTPNP_INT16 icon_id);
extern void MTPNP_PFAL_Show_StatusICON(MTPNP_INT16 icon_id);
extern void MTPNP_PFAL_Replace_StatusICON(MTPNP_INT16 icon_id, MTPNP_UINT16 image_id);
extern void MTPNP_PFAL_Blink_StatusICON(MTPNP_UINT16 icon_id);
extern void MTPNP_PFAL_Refresh_StatusBar(void);
extern void mmi_mtpnp_entry_menu(U16 scr_id, FuncPtr exit_func, FuncPtr entry_func, U16 menu_id, U16 str_id, U16 img_id);
extern void MTPNP_PFAL_SMS_Set_Current_Index(S32 nIndex);
extern MTPNP_BOOL MTPNP_PFAL_Is_All_SIM_Init(void);
////////////////////////////////////////
// Bootup APIs

extern void MTPNP_AD_Set_PowerOn_Mode(E_MTPNP_AD_POWERON_MODE mode);
extern void MTPNP_PFAL_SIM2_ECCInfoSave(void *inMsg);


////////////////////////////////////////
// Mode Switch APIs

extern E_MTPNP_AD_MODE_SWITCH_STATE MTPNP_PFAL_Switch_Mode(E_MTPNP_AD_NVRAM_STATUS curr_status);
//extern E_MTPNP_AD_SIMCARD_STATUS MTPNP_AD_Get_Card1Status(void);
//extern E_MTPNP_AD_SIMCARD_STATUS MTPNP_AD_Get_Card2Status(void);
extern E_MTPNP_AD_SIMCARD_USABLE_SIDE MTPNP_AD_Get_UsableSide(void);
extern void MTPNP_AD_Set_CardStatus(E_MTPNP_AD_SIMCARD_STATUS card_status);
extern MTPNP_BOOL MTPNP_AD_Is_During_Mode_Switch(void);
extern MTPNP_BOOL MTPNP_AD_Is_Enable_Mode_Switch(void);
//extern MTPNP_BOOL MTPNP_PFAL_Is_Card1Absent(void);
//extern MTPNP_BOOL MTPNP_PFAL_Is_Card2Absent(void);
//extern MTPNP_BOOL MTPNP_PFAL_Is_Card1Valid(void);
//extern MTPNP_BOOL MTPNP_PFAL_Is_Card2Valid(void);
//extern MTPNP_BOOL MTPNP_PFAL_Is_Card1CHVBlocked(void);
//extern MTPNP_BOOL MTPNP_PFAL_Is_Card2CHVBlocked(void);
extern MTPNP_UINT8 MTPNP_AD_Update_Switch_Table(void);
extern MTPNP_BOOL MTPNP_AD_Get_Mode_Item(E_MTPNP_AD_NVRAM_STATUS option, MTPNP_INT8 * index);

extern MTPNP_BOOL MTPNP_AD_MMS_Is_Avail(void);

extern MTPNP_BOOL MTPNP_PFAL_Has_CardValid(void);
extern MTPNP_BOOL MTPNP_PFAL_Is_CardAbsent(U8 nSimID);
extern MTPNP_BOOL MTPNP_PFAL_Is_CardCHVBlocked(U8 nSimID);
extern BOOL mmi_bootup_is_network_searching_complete(U8 nSimID);


extern MTPNP_BOOL MTPNP_PFAL_Is_CardValid(U8 nSimID);
extern U8 MTPNP_PFAL_Get_Channel_SIMID(E_MTPNP_AD_CHANNELS_OWNER owner);
extern MTPNP_BOOL MTPNP_PFAL_Is_Card_Usable(U8 nSimID);
extern void MTPNP_AD_SetMenuHideUnhide(U32 menuID);
extern U8 MTPNP_AD_Get_UsableSide_Number(void);
extern S8 MTPNP_AD_GET_UsableSide_Index(void);
////////////////////////////////////////
// Setting-CHV APIs

extern void MTPNP_AD_Get_SIM_Status_For_Lock(void);
extern MTPNP_UINT8 MTPNP_AD_Get_CHV1_Count(void);
extern MTPNP_UINT8 MTPNP_AD_Get_UBCHV1_Count(void);
extern void MTPNP_AD_SetSIMLockReq(void);
extern MTPNP_BOOL MTPNP_AD_SIM_CHV1_Is_Enable(void);
extern MTPNP_BOOL MTPNP_AD_SIM_CHV1_Is_Disable(void);
extern void MTPNP_AD_Get_SIM_Status_For_CHV2(void);
extern MTPNP_UINT8 MTPNP_AD_Get_CHV2_Count(void);
extern MTPNP_UINT8 MTPNP_AD_Get_UBCHV2_Count(void);
extern void MTPNP_AD_Validate_CHV(void);
extern MTPNP_BOOL MTPNP_AD_CHV_Operation_Rsp(MTPNP_BOOL result);
extern void MTPNP_AD_ADN_SetLoadFinish(MTPNP_BOOL value);

extern U8 GetReqSrcModIndex(void);
extern U8 GetReqSrcModeIndexByIndex(U8 index);
extern void SetReqSrcModeIndex(U8 index, U8 mode_index);
extern void MultiSimMenuHighLightHandler(S32 hiliteid);
////////////////////////////////////////
// Network Setup APIs

typedef enum
{
	MTPNP_AD_NO_SERVICE = 0,
	MTPNP_AD_LIMITED_SERVICE,
	MTPNP_AD_FULL_SERVICE
} E_MTPNP_AD_SERVICE_TYPE;

extern void MTPNP_PFAL_Update_SIM2_GPRS_Status(MTPNP_UINT8 gprs_state);
extern MTPNP_BOOL MTPNP_AD_Set_PLMN_Req(ST_MTPNP_AD_SET_PLMN_REQ * set_req);
extern MTPNP_BOOL MTPNP_AD_Get_PLMN_List_Req(void);
extern MTPNP_BOOL MTPNP_AD_Set_PLMN_Req(ST_MTPNP_AD_SET_PLMN_REQ * set_req);
extern MTPNP_BOOL MTPNP_AD_Set_Select_Mode_Req(void);
extern MTPNP_BOOL MTPNP_AD_Set_PLMN_Req(ST_MTPNP_AD_SET_PLMN_REQ * set_req);
extern MTPNP_BOOL MTPNP_AD_Get_Preferred_Network_Req(ST_MTPNP_AD_GET_PREFERRED_NETWORK_REQ * get_req);
extern MTPNP_BOOL MTPNP_AD_Set_Preferred_Network_Req(ST_MTPNP_AD_SET_PREFERRED_NETWORK_REQ * set_req);
extern void MTPNP_AD_Set_CardServiceType(E_MTPNP_AD_SERVICE_TYPE svr_type, U8 nSimID);
extern E_MTPNP_AD_SERVICE_TYPE MTPNP_AD_Get_Card1ServiceType(void);
extern E_MTPNP_AD_SERVICE_TYPE MTPNP_AD_Get_Card2ServiceType(void);


////////////////////////////////////////
// Phonebook APIs

extern void MTPNP_PFAL_Slave_PHB_Init(void);
extern void HintSlaveAddEntry(MTPNP_UINT16 index);
extern void HighlightSlaveCopylEntrySIM2Phone(void);
extern void HighlightSlaveMovelEntrySIM2Phone(void);
extern void HighlightSlaveCopylEntrySIM2SIM(void);
extern void HighlightSlaveMovelEntrySIM2SIM(void);
extern void HighlightSlaveCopyAllSIM2toPhone(void);
extern void HighlightSlaveCopyAllPhonetoSIM2(void);
extern void HighlightSlaveCopyAllSIM1toSIM2(void);
extern void HighlightSlaveCopyAllSIM2toSIM1(void);

extern void MTPNP_AD_ADN_Startup_Read(ST_MTPNP_DATA_PB_CAPACITY *info);
extern MTPNP_BOOL MTPNP_AD_ADN_READALL_FINISH(MTPNP_UINT8 max_name_len, MTPNP_UINT16 capacity, MTPNP_UINT8 max_number_len);
extern MTPNP_BOOL MTPNP_AD_ADN_SIM2_Is_LoadFinished(void);
extern MTPNP_UINT8 MTPNP_AD_ADN_SIM2GetTagLen(void);
extern void MTPNP_AD_ADN_SIM2SetTagSupport(MTPNP_UINT8 len);
extern MTPNP_UINT8 MTPNP_AD_ADN_SIM2GetNumLen(void);
extern void MTPNP_AD_ADN_SIM2SetNumberSupport(MTPNP_UINT16 len);
extern MTPNP_UINT16 MTPNP_AD_ADN_SIM2GetUsedNumber(void);
extern MTPNP_UINT16 MTPNP_AD_ADN_SIM2GetCapacity(void);
extern MTPNP_UINT16 MTPNP_AD_FDN_GetUsedNumber(void);
extern MTPNP_UINT8 MTPNP_AD_FDN_GetReady(void);
extern MTPNP_BOOL MTPNP_AD_Add_ADN_Record(E_MTPNP_CODING_SCHEME coding_scheme, MTPNP_CHAR * name, MTPNP_CHAR * number, MTPNP_UINT8 TON_NPI);
extern MTPNP_BOOL MTPNP_AD_Edit_ADN_Record(E_MTPNP_CODING_SCHEME coding_scheme, MTPNP_CHAR * name, MTPNP_CHAR * number, MTPNP_UINT8 TON_NPI, MTPNP_UINT16 index);
extern MTPNP_BOOL MTPNP_AD_ADN_Record_ADD_Succeed(MTPNP_UINT16 index, MTPNP_UINT16 name_length, E_MTPNP_STORAGE_TYPE storage);
extern MTPNP_BOOL MTPNP_AD_ADN_Record_ADD_Failed(MTPNP_UINT16 fail_cause, E_MTPNP_STORAGE_TYPE storage);
extern MTPNP_BOOL MTPNP_AD_ADN_Record_EDIT_Succeed(MTPNP_UINT16 index, MTPNP_UINT16 name_length, E_MTPNP_STORAGE_TYPE storage);
extern MTPNP_BOOL MTPNP_AD_ADN_Record_EDIT_Failed(MTPNP_UINT16 fail_cause, E_MTPNP_STORAGE_TYPE storage);
extern MTPNP_UINT8 MTPNP_AD_ADN_SIM2GetTagSupport(void);
extern MTPNP_UINT8 MTPNP_AD_ADN_SIM2GetNumberSupport(void);
extern MTPNP_BOOL MTPNP_AD_DELETE_ADN_Record(MTPNP_UINT16 index);
extern MTPNP_BOOL MTPNP_AD_ADN_Record_DEL_Succeed(MTPNP_UINT16 index, E_MTPNP_STORAGE_TYPE storage);
extern MTPNP_BOOL MTPNP_AD_ADN_Record_DEL_Failed(MTPNP_UINT16 index, E_MTPNP_STORAGE_TYPE storage);
extern void MTPNP_AD_ADN_DelAllOneAfterOneReq(void *pData);
extern MTPNP_UINT16 MTPNP_AD_ADN_GetNextValidIndex(MTPNP_UINT16 Begin);
extern void MTPNP_AD_ADN_DelAllFinish(void);

////////////////////////////////////////
// fdn APIs

////////////////////////////////////////
// SS APIs


////////////////////////////////////////
// SAT APIs


////////////////////////////////////////
// SMS APIs
extern void MTPNP_PFAL_Refresh_StatusBar_SMS_Display(MTPNP_BOOL blink, MTPNP_BOOL unblink, U8 nSimID);
extern MTPNP_BOOL MTPNP_AD_Msgbox_Is_Init(void);
extern void MTPNP_AD_Msgbox_HasInit(void);
extern void MTPNP_PFAL_SMS_Unload(U8 nSimID);
extern E_MTPNP_SLAVE_MODULE_TYPE MTPNP_AD_Get_Slave_Type(void);
extern U16 MTPNP_PFAL_SMS_Get_SMS_List_Size(U8 type);
extern void MTPNP_PFAL_SMS_Modify_Index(U8 type);
extern E_MTPNP_AD_FILTER_RULE MTPNP_AD_SMS_Get_Filter_Rule(void);
extern S32 MTPNP_PFAL_SMS_Get_Current_Index(void);
extern MTPNP_BOOL MTPNP_PFAL_Msg_Inbox_List_Get_Item(MTPNP_INT32 item_index, UI_string_type str_buff, MTPNP_UINT8 ** img_buff_p, MTPNP_UINT8 str_img_mask);
extern S32 MTPNP_PFAL_Msg_Inbox_List_Get_Hint(S32 item_index, UI_string_type * hint_array);
extern MTPNP_BOOL MTPNP_PFAL_Msg_Outbox_List_Get_Item(MTPNP_INT32 item_index, UI_string_type str_buff, MTPNP_UINT8 ** img_buff_p, MTPNP_UINT8 str_img_mask);
#ifdef __MMI_MESSAGES_DRAFT_BOX__
extern MTPNP_BOOL MTPNP_PFAL_Msg_Draftbox_List_Get_Item(MTPNP_INT32 item_index, UI_string_type str_buff, MTPNP_UINT8 ** img_buff_p, MTPNP_UINT8 str_img_mask);
extern void mmi_msg_pre_entry_master_draftbox_list(void);
extern void mmi_msg_pre_entry_slave_draftbox_list(void);
#endif

extern void MTPNP_PFAL_SMS_Init_Show_Style(void);
extern void MTPNP_AD_SMS_Set_Filter_Rule(E_MTPNP_AD_FILTER_RULE rule);
extern E_MTPNP_AD_SHOW_STYLE MTPNP_AD_SMS_Get_Show_Style(void);
extern void MTPNP_PFAL_InitMessageSetup(void);
extern MTPNP_BOOL MTPNP_AD_Dual_Card_Reply_SMS(void);
extern BOOL mmi_frm_sms_is_sms_valid(void);
extern BOOL mmi_frm_sms_is_hide_send_sms_menu_item(void);
extern void mmi_msg_dial_use_number_ext(void);

extern void MTPNP_PFAL_Entry_Slave_Mem_Exceed_Ind(void);
#ifdef JATAAYU_SUPPORT
typedef enum _mtpnp_sim_select_items
{
	E_MTPNP_SIM_SELECT_ALWAYS_ASK,
	E_MTPNP_SIM_SELECT_SIM1,
	E_MTPNP_SIM_SELECT_SIM2,
	E_MTPNP_SIM_SELECT_SIM3,
	E_MTPNP_SIM_SELECT_SIM4,
	E_MTPNP_SIM_SELECT_MENU_COUNT = E_MTPNP_SIM_SELECT_SIM1 + MMI_SIM_NUMBER
}MTPNP_SIM_SELECT_ITEMS ;

#define WAP_MMS_UICONNID_SIM1_FLAG	0x01
#define WAP_MMS_UICONNID_SIM2_FLAG	0x02
#define WAP_MMS_UNICONID_SIM3_FLAG 	0x04
#define WAP_MMS_UNICONID_SIM4_FLAG	0x08
#define WAP_MMS_UICONNID_WAP_FLAG	0x10000
#define WAP_MMS_UICONNID_MMS_FLAG	0x100000
#define WAP_MMS_UICONNID_PROV_FLAG	0x1000000


extern void 	MTPNP_jwap_always_ask_select_sim_entry_handler(void);
extern void 	MTPNP_jwap_edit_ph_select_sim_entry_handler(void);
extern void 	MTPNP_jWap_mainMenu_entry_handler (void) ;
extern void 	MTPNP_jWap_select_sim_entry_handler (void);
extern void 	MTPNP_jWap_select_sim_entry_highlight_handler(int iSelectIndex);
extern void 	MTPNP_mmi_ph_init(void);

extern void 	MTPNP_mms_setting_select_sim_entry_handler(void);
extern void 	MTPNP_mms_send_msg_select_sim_entry_handler(void);
extern void 	MTPNP_mms_resend_select_sim_entry_handler(void);
extern void 	mmi_entry_gsm_change_pin1_select_sim(void);
extern void 	mmi_entry_gsm_change_pin2_select_sim(void);
void jwap_DualsimSelectionEndKayHandler();
void jwap_ProfileEditEndKayHandler();
#endif
////////////////////////////////////////
// Setting-Profile APIs
#ifdef __SENDKEY2_SUPPORT__
extern void MTPNP_PFAL_CC_Sim1Invaild(void);
extern void MTPNP_PFAL_CC_Sim2Invaild(void);
extern void MTPNP_PFAL_CC_Sim3Invaild(void);
#endif

void MTPNP_CPHSDisplayCFU();

#endif

#endif

