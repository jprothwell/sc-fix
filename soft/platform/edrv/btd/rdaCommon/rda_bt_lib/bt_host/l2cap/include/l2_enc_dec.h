/**************************************************************************
 * MODULE NAME:    l2_end_dec.h
 * PROJECT CODE:
 * DESCRIPTION:    l2cap PDU encode & decode functions
 * MAINTAINER:         Gary Fleming
 * DATE:           02-12-1999
 *
 * SOURCE CONTROL: $Id: l2_enc_dec.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000 Parthus Technologies, Ltd.
 *     All rights reserved.
 *
 *
 * SOURCE: 
 *
 *************************************************************************/

#ifndef __PARTHUS_L2CAP_ENCODE_DECODE
#define __PARTHUS_L2CAP_ENCODE_DECODE

#include "host_config.h"
#include "host_types.h"

u_int8 prh_l2_enc_derive_identifier(void);
t_api prh_l2_encode_packet(u_int16 handle,u_int8 opcode, prh_t_l2_pdu* p_pdu_info);
u_int8* prh_l2_decode_packet(u_int16 handle,u_int8 opcode,u_int8* p_pdu);
t_api prh_l2_enc_2_pdus(u_int16 handle,u_int8 opcode1, u_int8 opcode2, prh_t_l2_pdu* p_pdu1_info,prh_t_l2_pdu* p_pdu2_info );
void prh_l2_enc_set_cur_ident(u_int8 Tid);

void L2_Set_C_Bit(void);

#endif
