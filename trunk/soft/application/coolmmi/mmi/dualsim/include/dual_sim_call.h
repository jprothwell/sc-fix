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
/*       dual_sim_call.h                                                           */
/*                                                                            */
/* DESCRIPTION                                                                */
/*   MTPNP layer header file, includes definition of macros and               */
/*   data structures and API functions.                                       */
/*                                                                            */
/******************************************************************************/

#ifndef __DUAL_SIM_CALL_H__
#define __DUAL_SIM_CALL_H__

#ifdef __MMI_MULTI_SIM__

#include "dual_sim.h"

//////////////////////////////////////////////////////////////////////////
/////CC

typedef enum
{
    MMI_UCM_FROM_IDLE = 0,
    MMI_UCM_FROM_L4C,
    MMI_UCM_FROM_L4C_SIM2,
    MMI_UCM_FROM_SAT,
    MMI_UCM_FROM_WAP,
    MMI_UCM_FROM_ABM,
    MMI_UCM_FROM_URI_AGENT,
    MMI_UCM_MODULE_ORIGIN_TOTAL
} mmi_ucm_module_origin_enum;
typedef enum
{
    MMI_UCM_ENTRY_MENU = 0,
    MMI_UCM_ENTRY_SEND_KEY,
    MMI_UCM_ENTRY_CENTER_KEY,
    MMI_UCM_ENTRY_DIAL_CALL_TYPE_TOTAL
} mmi_ucm_entry_dial_call_type_enum;

typedef enum
{
    MMI_UCM_VOICE_CALL_TYPE = 0x0001,
    MMI_UCM_VOICE_CALL_TYPE_SIM2 = 0x0002,
    MMI_UCM_VOICE_CALL_TYPE_SIM3 = 0x0004,
    MMI_UCM_VOICE_CALL_TYPE_SIM4 = 0x0008,
    MMI_UCM_DATA_CALL_TYPE  = 0x0010, /* data call with in-call screen */
    MMI_UCM_DATA_CALL_TYPE_SIM2 = 0x0020,
    MMI_UCM_CSD_CALL_TYPE   = 0x0040, /* data call without in-call screen */
    MMI_UCM_CSD_CALL_TYPE_SIM2 = 0x0080,
    MMI_UCM_VOIP_CALL_TYPE  = 0x0100,
    MMI_UCM_VIDEO_CALL_TYPE = 0x0200,
    MMI_UCM_CALL_TYPE_END_OF_ENUM
} mmi_ucm_call_type_enum;

#define MMI_UCM_SIM1_CALL_TYPE_ALL     (MMI_UCM_VOICE_CALL_TYPE | MMI_UCM_DATA_CALL_TYPE | MMI_UCM_CSD_CALL_TYPE)
#define MMI_UCM_SIM2_CALL_TYPE_ALL     (MMI_UCM_VOICE_CALL_TYPE_SIM2 | MMI_UCM_DATA_CALL_TYPE_SIM2 | MMI_UCM_CSD_CALL_TYPE_SIM2)
#define MMI_UCM_VOICE_CALL_TYPE_ALL    (MMI_UCM_VOICE_CALL_TYPE | MMI_UCM_VOICE_CALL_TYPE_SIM2|MMI_UCM_VOICE_CALL_TYPE_SIM3|MMI_UCM_VOICE_CALL_TYPE_SIM4)

typedef void (*mmi_ucm_callback_func_ptr) (void *);

#define MMI_UCM_MAX_NUM_URI_LEN                 (65)


typedef struct
{
    U16 log_name[MAX_PB_NAME_LENGTH + 1];
#if 0
    U16 disp_name[MMI_UCM_MAX_DISP_NAME_LEN];
#endif /* Guoj delete. It's not used now */
    U8 num_uri[MMI_UCM_MAX_NUM_URI_LEN];
    U16 active_icon;
} mmi_ucm_remote_info_struct;


typedef struct
{
#if 0
    U8 dial_pad_buffer[MMI_UCM_MAX_NUM_URI_LEN * ENCODING_LENGTH];
#endif /* Guoj delete. It's not used now */
    mmi_ucm_module_origin_enum module_id;

    mmi_ucm_call_type_enum dial_type;
    mmi_ucm_remote_info_struct dial_num;
    MMI_BOOL is_wait_dial;
    mmi_ucm_callback_func_ptr callback;
    void* callback_para;
} mmi_ucm_dial_info_struct;

typedef struct
{
#if 0
    U8 disp_dtmf[(MMI_UCM_DTMF_LEN + 1) * ENCODING_LENGTH];   /* 80 unicode + 2 bytes null terminator. input and display dtmf for category screen */
    U8 input_dtmf[(MMI_UCM_DTMF_LEN + 1)];                    /* 40 ascii + 1 byte null terminator. actual inputted dtmf for VoIP RTP */
#endif /* Guoj delete. It's not used now */
    S32 hilite_tab;
    S32 hilite_index;
    S32 hilite_multi_held_index;
    S32 no_rcode_count; /* do not send result code for how many times */
#if 0
    mmi_ucm_index_info_struct index_list[MMI_UCM_MAX_NUM_TAB * MMI_UCM_MAX_NUM_INDEX];
    mmi_ucm_act_opcode_enum curr_action;
    mmi_ucm_module_origin_enum curr_module;
    mmi_ucm_l4c_info_struct l4c_info; /* information that needs UCM to bypass to L4C */
    mmi_ucm_incoming_tone_enum play_tone;
    mmi_ucm_nw_status_enum nw_service_status;
    mmi_ucm_dtmf_struct dtmf;
#endif /* Guoj delete. It's not used now */
    mmi_ucm_entry_dial_call_type_enum entry_type;
#ifdef __MMI_URI_AGENT__
    mmi_ucm_result_enum call_end_cause;
#endif /* __MMI_URI_AGENT__ */
    FuncPtr  sendkey_hdlr;
    FuncPtr  centerkey_hdlr;
    MMI_BOOL is_ring_after_vibra;
    MMI_BOOL is_speech;
    MMI_BOOL is_loud;
    MMI_BOOL is_mute;
    MMI_BOOL is_dtmf;
    MMI_BOOL is_delay_ciev;
    MMI_BOOL is_bgs_selected;
    MMI_BOOL is_ringtone_suppress;
    MMI_BOOL is_dtmf_start;
} mmi_ucm_call_misc_struct;
typedef struct
{
	mmi_ucm_call_misc_struct call_misc;
	mmi_ucm_dial_info_struct mo_info;
	U8 nSimID;
} mmi_ucm_cntx_struct;


//////////////////////////////////////////////////////////////
// Call APIs
extern void UCMResourceData(void);
extern void mmi_ucm_highlight_call_type_voice(void);
extern void mmi_ucm_dial_option_make_call(void);
extern void mmi_ucm_set_call_channel(U8 nSimID);
extern void mmi_ucm_dial_option_make_call_exec_callback(void);
extern void MTPNP_PFAL_CC_SetCenterFunc(FuncPtr cenfunc);
extern void mmi_ucm_set_centerkey_hdlr(FuncPtr centerkey_hdlr);
extern void MTPNP_PFAL_CC_CSKHandler(void);
extern void mmi_ucm_entry_dial_option_by_centerkey(void);
extern void mmi_ucm_entry_call_type(void);
extern void mmi_ucm_dial_option_unhide_menu_item(void);
extern FuncPtr MTPNP_PFAL_CC_GetCenterFunc(void);
extern FuncPtr mmi_ucm_get_centerkey_hdlr(void);
extern void MTPNP_PFAL_CC_ExeCenterFunc(void);
extern void mmi_ucm_exec_centerkey_hdlr(void);
extern void MTPNP_PFAL_CC_HandleSendKeys(FuncPtr newCallFunc, mmi_key_types_enum keyevent);
extern void MTPNP_PFAL_CC_SetMOCallFunc(FuncPtr funcname);
extern void mmi_ucm_set_sendkey_hdlr(FuncPtr sendkey_hdlr);
extern void MTPNP_PFAL_CC_SendKeyHandler(void);
extern FuncPtr MTPNP_PFAL_CC_GetMOCallFunc(void);
extern FuncPtr mmi_ucm_get_sendkey_hdlr(void);
extern void MTPNP_PFAL_CC_ExeMOCallFunc(void);
extern void mmi_ucm_exec_sendkey_hdlr(void);
extern void mmi_ucm_entry_dial_option_by_sendkey(void);
extern void MTPNP_AD_PsCBackCallIncoming (void* info);
extern void mmi_ucm_app_make_call(mmi_ucm_call_type_enum call_type, U8 *num_uri);
extern void mmi_ucm_app_entry_dial_option(U8 *num_uri, mmi_ucm_callback_func_ptr callback_after_dial, void* callback_para);
//////////////////////////////////////////////////////////////
//Call Setting
extern void MTPNP_PFAL_CallSetting_Init(void);
extern void MTPNP_CallSetSetProtocolEventHandler(PsFuncPtr funcPtr, U32 eventID);
extern void SetCallSetContext(U8 nSimId);
extern U16 GetCallSetLine(U8 nSimId);
extern void mmi_idle_set_dual_sim_dialing_keypad_call_handler(FuncPtr sim1_func, FuncPtr sim2_func,FuncPtr sim3_func,FuncPtr sim4_func);
extern MTPNP_BOOL MTPNP_PFAL_Retrieve_CallDial_State(MTPNP_UINT8 * m_dial_enable, MTPNP_UINT8 * s_dial_enable,MTPNP_UINT8 * t_dial_enable,MTPNP_UINT8 * f_dial_enable);
extern void mmi_sim_error_set_emergency_call_path(void);
#endif

#endif //__DUAL_SIM_CALL_H__

