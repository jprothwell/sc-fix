/***********************************************************************
 *
 * MODULE NAME:    sdp_le_request_decoder.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Service Discovery server
 * MAINTAINER:     Mark Donnelly
 *
 * SOURCE CONTROL: $Id: sdp_le_request_decoder.h 1532 2010-08-23 05:47:34Z huazeng $
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

#if SDP_SERVER

#ifndef SDP_LE_REQUEST_DECODER
#define SDP_LE_REQUEST_DECODER

#include "host_config.h"

#include "sdp_db.h"

typedef struct
{
  u_int16 start_id;
  u_int16 finish_id;
} sdp_attribute_id_list;



void prh_sdp_request_dispatcher(u_int8* pdu,t_channelId cid);
void prh_sdp_service_search_decode(u_int8* pdu,t_channelId cid);
void prh_sdp_attribute_search_decode(u_int8* pdu,t_channelId cid);
void prh_sdp_service_attribute_search_decode(u_int8* pdu,t_channelId cid);

u_int8 prh_sdp_attribute_check(u_int32* offset,u_int8* attributes, u_int8* len, sdp_attribute_id_list** p_list);

u_int8 prh_sdp_uuid_check(u_int32* offset,u_int8* uuids,u_int32* pList,u_int32* pNumber);

u_int32 prh_sdp_attribute_retrieve(t_SDP_serviceRecord* pRecord,u_int8 len, sdp_attribute_id_list* pList,u_int16 buffsize,u_int32 previous,u_int32* current,u_int8* buffer);

#define PRH_SDP_READ_SIZE( data ) ( (data & 7) )
#define PRH_SDP_READ_TYPE( data ) ( (data >> 3) )

#endif /* SDP_LE_REQUEST_DECODER */

#endif /* SDP_SERVER */














