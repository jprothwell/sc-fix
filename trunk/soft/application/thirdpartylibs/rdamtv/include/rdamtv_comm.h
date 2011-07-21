// ---------------------------------------------------------------------------
// RDA Microelectronics, Inc. Company Confidential Strictly Private
//
// $RCSfile: rdamtv_comm.h,v $
// $Revision: 0 $
// $Author: wuhp $
// $Date: 2009-05-25 $
//
// ---------------------------------------------------------------------------
// >>>>>>>>>>>>>>>>>>>>>>>>> COPYRIGHT NOTICE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ---------------------------------------------------------------------------
// Copyright 2008-2009 (c) RDA Microelectronics, Inc.
//
// RDA owns the sole copyright to this software. Under international
// copyright laws you (1) may not make a copy of this software except for
// the purposes of maintaining a single archive copy, (2) may not derive
// works herefrom, (3) may not distribute this work to others. These rights
// are provided for information clarification, other restrictions of rights
// may apply as well.
//
// This is an unpublished work.
// ---------------------------------------------------------------------------

#ifndef __RDAMTV_COMM_H__
#define __RDAMTV_COMM_H__
#include "kal_non_specific_general_types.h"
#include "stack_config.h"

#ifndef LOCAL_PARA_HDR
#define LOCAL_PARA_HDR \
kal_uint8 ref_count; \
kal_uint16 msg_len;
#endif

typedef struct 
{
    LOCAL_PARA_HDR
    kal_uint8 rdamtv_mode;
	kal_int16 *pwRet;
} rdamtv_init_req_struct;

typedef struct 
{
    LOCAL_PARA_HDR
    kal_uint32 param_id;
    kal_int16 wValue;
	kal_int16 *pwRet;
} rdamtv_set_param_req_struct;

typedef struct 
{
    LOCAL_PARA_HDR
    kal_uint32 param_id;
    kal_int16 *pwValue;
	kal_int16 *pwRet;
} rdamtv_get_param_req_struct;

typedef struct 
{
	LOCAL_PARA_HDR
	kal_uint32 dwFreq;
	kal_int16 *pwRet;
} rdamtv_set_channel_req_struct;

typedef struct 
{
	LOCAL_PARA_HDR
	kal_uint32 dwFreq;
	kal_int16 *pwRet;
} atv_auto_scan_req_struct;

typedef struct 
{
	LOCAL_PARA_HDR
	kal_int16 wResult;
	kal_int16 *pwRet;
} rdamtv_ind_result_struct;

// supported frequency bandwidth settings.
typedef enum {
	RDAMTV_BW_5 = 5,				// 5 MHz bandwidth	
	RDAMTV_BW_6 = 6,				// 6 MHz bandwidth	
	RDAMTV_BW_7 = 7,				// 7 MHz bandwidth	
	RDAMTV_BW_8 = 8,				// 8 MHz bandwidth	
	RDAMTV_BW_NONE
} rdamtv_bw_t;

/* rdamtv param enum */
typedef enum {
	RDAMTV_PARAM_BRIGHTNESS,
	RDAMTV_PARAM_CHROMA,
	RDAMTV_PARAM_VOLUME,
	RDAMTV_PARAM_RSSI,
	RDAMTV_PARAM_MAX
} rdamtv_para_t;

kal_int16 rdamtv_comm_init(module_type src_mod_id, kal_uint8 rdamtv_mode);
kal_int16 rdamtv_comm_scan_channel(module_type src_mod_id, kal_uint32 dwFreq);
kal_int16 rdamtv_comm_set_channel(module_type src_mod_id, kal_uint32 dwFreq);
void rdamtv_comm_exit(module_type src_mod_id);

#endif
