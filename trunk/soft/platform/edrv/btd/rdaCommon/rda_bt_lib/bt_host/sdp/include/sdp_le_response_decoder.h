/***********************************************************************
 *
 * MODULE NAME:    sdp_le_response_decoder.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Service Discovery Test code
 * MAINTAINER:     Mark Donnelly
 *
 * SOURCE CONTROL: $Id: sdp_le_response_decoder.h 1532 2010-08-23 05:47:34Z huazeng $
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

#include "host_config.h"

#if SDP_CLIENT

#ifndef SDP_LE_RESPONSE_DECODER
#define SDP_LE_RESPONSE_DECODER

void prh_sdp_response_dispatcher(u_int8* pdu,t_channelId cid);
void prh_sdp_service_response_decode(u_int8* pdu,t_channelId cid);
void prh_sdp_attribute_response_decode(u_int8* pdu,t_channelId cid);
void prh_sdp_service_attribute_response_decode(u_int8* pdu,t_channelId cid);
void prh_sdp_error_decode(u_int8* pdu); /* on need for cid as I won't be responding */

#endif /* SDP_LE_RESPONSE_DECODER */

#endif /* SDP_CLIENT */

