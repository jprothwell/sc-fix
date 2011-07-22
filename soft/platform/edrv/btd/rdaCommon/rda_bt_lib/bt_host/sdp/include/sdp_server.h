/***********************************************************************
 *
 * MODULE NAME:    sdp_server.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Service Discovery Server interface
 * MAINTAINER:     Mark Donnelly
 *
 * SOURCE CONTROL: $Id: sdp_server.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001-2001 Parthus Technologies
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

#include "sdp_db.h"

#define SDP_RECORD_UNAVAILABLE 1
#define SDP_RECORD_AVAILABLE 2


APIDECL1 t_api APIDECL2 SDP_CreateRecord(u_int32* recordHandle,t_SDP_serviceRecord** pRecord);
APIDECL1 t_api APIDECL2 SDP_RemoveRecord(u_int32 recordHandle);
APIDECL1 t_api APIDECL2 SDP_GetRecord(u_int32 recordHandle, t_SDP_serviceRecord** pRecord);
APIDECL1 t_api APIDECL2 SDP_GetNextRecord(u_int32 currentRecordHandle,u_int32 *pNextHandle);

APIDECL1 t_api APIDECL2 SDP_EncodeAttribute(u_int8 type,u_int32 size, u_int8* pData, t_SDP_dataElement* attribute);
APIDECL1 t_api APIDECL2 SDP_AddAttribute(t_SDP_serviceRecord* pRecord,u_int16 attribute_id,t_SDP_dataElement* data);
APIDECL1 t_api APIDECL2 SDP_ReplaceAttribute(t_SDP_serviceRecord* pRecord,u_int16 attribute_id,t_SDP_dataElement* data);


APIDECL1 t_api APIDECL2 SDP_CreateList(u_int8 type,u_int32 number,t_SDP_dataElement** attributes,t_SDP_dataElement* list_attributes);
APIDECL1 t_api APIDECL2 SDP_SetRecordState(u_int32 recordHandle,u_int8 state);


APIDECL1 t_api APIDECL2 SDP_StartServer(u_int16 flags, void *reserved);
APIDECL1 t_api APIDECL2 SDP_StopServer(u_int16 flags, void *reserved);

t_api prh_sdp_srv_layer_ctrl(u_int8 flags);

APIDECL1 t_api APIDECL2 SDP_DBSize(u_int32* length);
APIDECL1 t_api APIDECL2 SDP_DBtoStatic(u_int8** raw,u_int32 length);
APIDECL1 t_api APIDECL2 SDP_BuildROMForTransfer(void);


#endif /* SDP_SERVER */



















