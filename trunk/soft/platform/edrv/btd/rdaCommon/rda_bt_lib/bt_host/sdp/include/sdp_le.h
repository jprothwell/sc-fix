#ifndef SDP_LE_H
#define SDP_LE_H

#include "host_config.h"


/***********************************************************************
 *
 * MODULE NAME:    sdp_le.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Service Discovery Test code
 * MAINTAINER:     Mark Donnelly
 * CREATION DATE:  08 March 2000
 *
 * SOURCE CONTROL: $Id: sdp_le.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000 Parthus Technologies
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/

#include "sdp_stack_access.h"

#include "sdp_db.h"


void sdp_print_buffer(u_int8* pBuffer, int length);

void prh_sdp_transfer_data(t_channelId cid,struct host_buf* h_buff);

u_int16 prh_sdp_read16(u_int8* p_pdu);
u_int32 prh_sdp_read32(u_int8* pData);

void prh_sdp_write16(u_int16 data, u_int8* buffer);
void prh_sdp_write32(u_int32 data, u_int8* buffer);

struct host_buf* prh_sdp_allocate_tx_buffer(u_int32 size, u_int16 cid);

#endif /* SDP_LE_H */








