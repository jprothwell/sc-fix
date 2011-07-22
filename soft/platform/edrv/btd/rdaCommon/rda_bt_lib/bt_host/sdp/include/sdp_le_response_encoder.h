/***********************************************************************
 *
 * MODULE NAME:    sdp_le.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Service Discovery Test code
 * MAINTAINER:     Mark Donnelly
 * CREATION DATE:  08 March 2000
 *
 * SOURCE CONTROL: $Id: sdp_le_response_encoder.h 1532 2010-08-23 05:47:34Z huazeng $
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

#if SDP_SERVER

#ifndef SDP_LE_RESPONSE_ENC
#define SDP_LE_RESPONSE_ENC


#include "sdp_db.h"


void prh_sdp_generate_attrib_response(u_int8* pBuffer, u_int8* pData,u_int16 transaction_id,u_int16 byte_count,u_int8* pContinue);
void prh_sdp_generate_service_attrib_response(u_int8* pBuffer, u_int8* pData,u_int16 transaction_id,u_int16 byte_count,u_int8* pContinue);
void prh_sdp_generate_search_response(u_int8* pBuffer, u_int32* pData, u_int16 transaction_id, u_int16 total, u_int16 current, u_int8* pContinue);
void prh_sdp_generate_error_response(u_int8* pBuffer,u_int16 error_code,u_int16 length, u_int8* pError_info, u_int16 transaction_id);

u_int32 prh_sdp_data_element_converter(u_int16 buffer_size,u_int8* pData,u_int32 previous,u_int32 *current,t_SDP_dataElement* pElement);
void prh_sdp_gen_continue(u_int16 place_in_list, u_int16 next_id, u_int8** pBuffer);

#endif /* SDP_LE_RESPONSE_ENC */

#endif /* SDP_SERVER */
