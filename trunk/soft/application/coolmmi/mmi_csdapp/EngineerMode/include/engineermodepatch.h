/****************************************************************/
/* Copyright (C) 2003, CII Technologies, Inc. */
/* All Rights Reserved */
/****************************************************************/
/* File Name : EngineerModePatch.h */
/* Description : */
/* this file is patch for EM/FM */
/* Revision : */
 
/* add new file */
/****************************************************************/
#ifndef _ENGINEERMODEINIT_H_
#define _ENGINEERMODEINIT_H_


//---------------------------------------------------------
//include
#include "mmi_features.h"
#include "kal_non_specific_general_types.h"
#include "cswtype.h"
#include "stack_config.h"
#include "l1audio.h"
#include "custom_equipment.h"
#include "uart_sw.h"
#include "stack_config.h"
#include "custom_hw_default.h"
#include "mmiapi.h"
#include "app_ltlcom.h"
#include "kal_non_specific_general_types.h"
#include "stdio.h"
#include "stack_msgs.h"
#define GPIO_INVAILD   0x0
#define GPIO_VAILD     0x01
#define GPO_VAILD        0x02
#define MAX_NETNAME_TEXT 16

#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_ENGINEER_MODE__) ) 	

//---------------------------------------------------------
//macro

//---------------------------------------------------------
//struct
typedef struct
{
   const kal_char * bb_chip;
   const kal_char * dsp_fw;
   const kal_char * dsp_ptch;
   const kal_char * mcu_sw;
   const kal_char * mcu_sw_branch;
   const kal_char * bb_board;
} version_struct;

typedef struct {
   kal_uint16    pdn_con0;
   kal_uint16    pdn_con1;
   kal_uint16    pdn_con2;
   kal_uint16    pdn_con3;
} CONFIG_PDN_TYPE;

//---------------------------------------------------------
//declare
void INT_VersionNumbers(version_struct * ptr_version);
void INT_QueryPowerDownStatus(CONFIG_PDN_TYPE *PDN_ptr);
void INT_SetPowerDownStatus(CONFIG_PDN_TYPE *PDN_ptr);

//---------------------------------------------------------
//extern
extern kal_uint32 kal_milli_secs_to_ticks(U8 tt);
extern HANDLE GetMmiTaskHandle(module_type mod);
extern void EntryEngineerModeMenu(void);
extern void EntryFMMenu(void);

//---------------------------------------------------------
//stub
kal_uint16 UART_PutBytes(UART_PORT port, 
                                             kal_uint8 *Buffaddr, 
                                             kal_uint16 Length, 
                                             module_type ownerid);

kal_uint16 UART_GetBytes(UART_PORT port, 
                                              kal_uint8 *Buffaddr, 
                                              kal_uint16 Length, 
                                              kal_uint8 *status, 
                                              module_type ownerid);
module_type UART_GetOwnerID(UART_PORT port);
void UART_SetOwner (UART_PORT port, kal_uint8 owner);
void Media_Stop( void );
//WCHAR *kal_wstrcpy(WCHAR *to, const WCHAR *from);

const kal_char* release_verno(void) ;
const kal_char* release_hw_ver(void) ;
const kal_char* build_date_time(void) ;
const kal_char* release_build(void) ;
const kal_char* release_branch(void) ;
void L1sim_Enable_Enhanced_Speed(kal_bool enable);

#define kal_mem_cpy(a,b,c) memcpy(a,b,c)

void* get_pdu_ptr(peer_buff_struct *peer_buff_ptr, kal_uint16 *length_ptr);
void free_int_peer_buff(peer_buff_struct *pdu_ptr, kal_char* file,kal_uint32 line);
kal_int32 media_aud_stop_id(module_type src_mod_id, kal_uint8 audio_id);
//---------------------------------------------------------
#endif //defined(MMI_ON_HARDWARE_P) || defined(__MMI_ENGINEER_MODE__)

#endif /* _ENGINEERMODEINIT_H_ */

