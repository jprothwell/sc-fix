/***********************************************************************
 *
 * MODULE NAME:    sdp_le_request_encoder.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Service Discovery client request encoder
 * MAINTAINER:     Mark Donnelly
 *
 * SOURCE CONTROL: $Id: sdp_le_request_encoder.h 1532 2010-08-23 05:47:34Z huazeng $
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

#ifndef SDP_LE_REQUEST_ENC
#define SDP_LE_REQUEST_ENC

#include "sdp_state_machine.h"

void prh_sdp_generate_search_request(u_int8* pBuffer,struct prh_sdp_primitive_task* pTask,u_int8* pContinue);
int prh_sdp_encode_search_pattern(u_int8* pBuffer,t_SDP_SearchPattern* pPattern);

void prh_sdp_generate_attribute_request(u_int8* pBuffer,struct prh_sdp_primitive_task* pTask,u_int8* pContinue);

int prh_sdp_encode_attribute_list(u_int8* pBuffer,t_SDP_AttributeIDs* pAttribs);

void prh_sdp_genetate_service_attribute_request(u_int8* pBuffer,struct prh_sdp_primitive_task* pTask,u_int8* pContinue);


#endif /* SDP_LE_REQUEST_ENC */

#endif /* SDP_CLIENT */








