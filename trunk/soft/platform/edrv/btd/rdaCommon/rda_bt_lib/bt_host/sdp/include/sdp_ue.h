/***********************************************************************
 *
 * MODULE NAME:    sdp_ue.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Service Discovery Test code
 * MAINTAINER:     Mark Donnelly
 *
 * SOURCE CONTROL: $Id: sdp_ue.h 1532 2010-08-23 05:47:34Z huazeng $
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
#include "l2cap_primitives.h"

#if SDP_CLIENT


#ifndef SDP_UE_H
#define SDP_UE_H


/*
  
  Types definitions
  
*/

#define PRH_SDP_BASE_RX_BUFFER_SIZE 250
#define PRH_SDP_MAX_RESULTS_SIZE 16*1024

typedef struct {
  u_int16 numItems;
  t_bdaddr *addresses;
} t_SDP_Addresses;

#define SDP_TRUSTED				1
#define SDP_ZERO_PIN				2
#define SDP_NORMAL_PIN				4

#define SDP_CONNECTED				8
#define SDP_UNCONNECTED				16

#define SDP_REMOTE_NAME				32	

typedef struct {
  u_int8 trust;
  u_int8 connection;
} SDPRelation;

typedef struct {
  u_int16 maxItems;
  u_int16 maxDuration;
  u_int16 maxBytes;
} t_SDP_StopRule;

typedef struct {
  u_int16 numItems;
  u_int32* patternUUIDs;
} t_SDP_SearchPattern;

typedef struct {
  u_int16 numItems;
  u_int32* attributeIDs;
} t_SDP_AttributeIDs;


typedef struct st_t_SDP_Element {
  t_bdaddr deviceAddress;
  char* deviceName;
  u_int16 numMatchingRecords;
  u_int16 dataLen;
  u_int8* pData;
  struct st_t_SDP_Element* pNext;
} t_SDP_Element;

typedef struct{
  u_int8 status;
  u_int8 resultType;
  u_int16 numElements;
  t_SDP_Element element;
} t_SDP_Results;


typedef struct{
  u_int16 status;
  u_int8 type;
  u_int16 length;
  u_int8* pData;
  u_int8 Continue[17];
} t_SDP_LowLevelResults;

#define MAX_SDP_UUID_NUM 15
typedef struct 
{
    u_int32 chn_num;
    u_int32 SC_uuid;  //Service Class ID List uuid
    u_int32 PDL_uuid;//profile descriptor list uuid
}t_SDP_service_Results;

typedef struct 
{
    u_int32 total_searched;
    t_SDP_service_Results service[15];
}t_SDP_service_Parse_Results;
typedef void (*sdp_api_callback)(u_int8 status);



/* SDAP client */
APIDECL1 t_api APIDECL2 SDP_EnumerateDevices(t_SDP_Addresses** devices,u_int32 cod,u_int32 iac,t_SDP_StopRule* pRule,sdp_api_callback callback, void *reserved);
APIDECL1 t_api APIDECL2 SDP_ServiceBrowse(t_SDP_Results** pResults,t_SDP_Addresses* pDevices, t_SDP_SearchPattern* pPattern, t_SDP_AttributeIDs* pAttributes, t_SDP_StopRule* pRule, u_int16 flags, sdp_api_callback callback);
APIDECL1 t_api APIDECL2 SDP_ServiceSearch(t_SDP_Results** pResults,t_SDP_Addresses* pDevices, t_SDP_SearchPattern* pPattern, t_SDP_AttributeIDs* pAttributes, t_SDP_StopRule* pRule, u_int16 flags, sdp_api_callback callback);

#if SDP_GENERIC_BROWSE

APIDECL1 t_api APIDECL2 SDP_GenericBrowse(t_SDP_Results** pResults,t_SDP_Addresses* pDevices, t_SDP_AttributeIDs* pAttributes, t_SDP_StopRule* pRule, u_int16 flags, sdp_api_callback callback);

#endif /* SDP_GENERIC_BROWSE */

APIDECL1 t_api APIDECL2 SDP_TerminatePrimitive(t_SDP_Results* pResults);

void SDP_RegisterPSM(void); 

APIDECL1 t_api APIDECL2 SDP_FreeResults(t_SDP_Results* pResults);
APIDECL1 t_api APIDECL2 SDP_FreeAddressList(t_SDP_Addresses* devices);
APIDECL1 t_api APIDECL2 SDP_EnableCache(u_int32 maxBytes, u_int16 flags);
APIDECL1 t_api APIDECL2 SDP_DisableCache(u_int16 flags); 

/* SDP Client API */

#if SDP_INTERFACE

APIDECL1 t_api APIDECL2 SDP_ServiceSearchRequest(t_SDP_LowLevelResults** pResults,t_channelId cid, t_SDP_SearchPattern* pPattern, u_int16 maxCount, u_int8* Continue, u_int8 timeout, sdp_api_callback callback);
APIDECL1 t_api APIDECL2 SDP_ServiceAttributeRequest(t_SDP_LowLevelResults** pResults, t_channelId cid, u_int32 handle, t_SDP_AttributeIDs* pAttributes, u_int16 maxBytes, u_int8* Continue, u_int8 timeout, sdp_api_callback callback);
APIDECL1 t_api APIDECL2 SDP_ServiceSearchAttributeRequest(t_SDP_LowLevelResults** pResults, t_channelId cid, t_SDP_SearchPattern* pPattern, t_SDP_AttributeIDs* pAttributes, u_int16 maxBytes, u_int8* Continue, u_int8 timeout, sdp_api_callback callback);

APIDECL1 t_api APIDECL2 SDP_ConnectTransport(t_bdaddr* address, u_int16 mtu,void (*configured)(u_int8,t_L2_ControlCallbackArgs *),void (*disconnect)(u_int8 type,t_L2_ControlCallbackArgs *));
APIDECL1 t_api APIDECL2 SDP_DisconnectTransport(t_channelId cid);


#endif /* SDP_INTERFACE */

 APIDECL1 t_api APIDECL2  sdp_service_parse(t_SDP_Results *pResults, t_SDP_service_Parse_Results *service_finded);
#endif /* SDP_UE_H */

t_api prh_sdp_cli_layer_ctrl(u_int8 flags);

#endif /* SDP_CLIENT */













