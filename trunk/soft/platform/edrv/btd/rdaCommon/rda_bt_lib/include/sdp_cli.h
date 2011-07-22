/***********************************************************************
 *
 * MODULE NAME:    sdp_cli.h
 * DESCRIPTION:    Bluetooth Host Stack SDP API client header
 * AUTHOR:         Mark Donnelly, Martin Mellody
 *
 * SOURCE CONTROL: $Id: sdp_cli.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 * REMARKS:
 *     This file should be incuded via bt.h only.
 *
 ***********************************************************************/


#ifndef BLUETOOTH_SDPCLIENT_API_DECLARED
#define BLUETOOTH_SDPCLIENT_API_DECLARED

#include "bt_sys.h"
#include "bt.h"

typedef struct st_t_SDP_SearchPattern {
  u_int16 numItems;
  u_int32* patternUUIDs;
} t_SDP_SearchPattern;

typedef struct st_t_SDP_Addresses {
  u_int16 numItems;
  t_bdaddr *addresses;
} t_SDP_Addresses;


typedef struct st_t_SDP_AttributeIDs {
  u_int16 numItems;
  u_int32 *attributeIDs;
} t_SDP_AttributeIDs;


typedef struct st_t_SDP_StopRule {
  u_int16 maxItems;
  u_int16 maxDuration;
  u_int16 maxBytes;
} t_SDP_StopRule;


typedef struct st_t_SDP_Element {
  t_bdaddr deviceAddress;
  char *deviceName;
  u_int16 numMatchingRecords;
  u_int16 dataLen;
  u_int8 *pData;
  struct st_t_SDP_Element *pNext;
} t_SDP_Element;



/* SDP protocol constants for status member */
#define SDP_NOERROR					0x00
#define SDP_UNSUPPORTED				0x01
#define SDP_INVALIDHANDLE			0x02
#define SDP_INVALIDSYNTAX			0x03
#define SDP_INVALIDSIZE				0x04
#define SDP_INVALIDCONTINUATION		0x05
#define SDP_NORESOURCES				0x06

/* Constants for resultType member */
#define SDP_SEARCH_RESULT		1
#define SDP_BROWSE_RESULT		2
#define SDP_GENBROWSE_RESULT	3

typedef struct st_t_SDP_Results {
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

/* Flag parameter values for search/browse functions */

#define SDP_TRUSTED					1
#define SDP_ZERO_PIN				2
#define SDP_NORMAL_PIN				4

#define SDP_CONNECTED				8
#define SDP_UNCONNECTED				16

#define SDP_REMOTE_NAME				32	



typedef void (*sdp_api_callback)(u_int8 status);


/* Wildcard Class Of Device used for EnumerateDevices */
#define SDP_ANY_COD 0xFFFFFFFF

APIDECL1 int APIDECL2 SDP_EnumerateDevices(t_SDP_Addresses **devices, u_int32 cod, u_int32 iac, t_SDP_StopRule *rule, sdp_api_callback callback, void (*discoverCallback)(t_MGR_DeviceEntry *deviceInfo));

APIDECL1 int APIDECL2 SDP_ServiceSearch(t_SDP_Results **results, t_SDP_Addresses *devices, t_SDP_SearchPattern *pattern, 
						t_SDP_AttributeIDs *attributes, t_SDP_StopRule *rule, u_int16 flags, sdp_api_callback callback);

APIDECL1 int APIDECL2 SDP_ServiceBrowse(t_SDP_Results **results, t_SDP_Addresses *devices, t_SDP_SearchPattern *pattern, 
						t_SDP_AttributeIDs *attributes, t_SDP_StopRule *rule, u_int16 flags, sdp_api_callback callback);

APIDECL1 int APIDECL2 SDP_GenericBrowse(t_SDP_Results **results, t_SDP_Addresses *devices,
						t_SDP_AttributeIDs *attributes, t_SDP_StopRule *rule, u_int16 flags, sdp_api_callback callback);


APIDECL1 int APIDECL2 SDP_FreeResults(t_SDP_Results *results);
APIDECL1 int APIDECL2 SDP_FreeAddressList(t_SDP_Addresses *devices);

APIDECL1 int APIDECL2 SDP_TerminatePrimitive(t_SDP_Results *results);

APIDECL1 int APIDECL2 SDP_ServiceSearchRequest(t_SDP_LowLevelResults** pResults,t_channelId cid, t_SDP_SearchPattern* pPattern, u_int16 maxCount, u_int8* Continue, u_int8 timeout, sdp_api_callback callback);
APIDECL1 int APIDECL2 SDP_ServiceAttributeRequest(t_SDP_LowLevelResults** pResults, t_channelId cid, u_int32 handle, t_SDP_AttributeIDs* pAttributes, u_int16 maxBytes, u_int8* Continue, u_int8 timeout, sdp_api_callback callback);
APIDECL1 int APIDECL2 SDP_ServiceSearchAttributeRequest(t_SDP_LowLevelResults** pResults, t_channelId cid, t_SDP_SearchPattern* pPattern, t_SDP_AttributeIDs* pAttributes, u_int16 maxBytes, u_int8* Continue, u_int8 timeout, sdp_api_callback callback);

//APIDECL1 int APIDECL2 SDP_ConnectTransport(t_bdaddr* address, u_int16 mtu,void (*configured)(u_int8,t_L2_ControlCallbackArgs *),void (*disconnect)(u_int8 type,t_L2_ControlCallbackArgs *));
APIDECL1 int APIDECL2 SDP_DisconnectTransport(t_channelId cid);


/* Support for Client-side caching, where present */
APIDECL1 int APIDECL2 SDP_EnableCache(u_int32 maxBytes, u_int16 flags);
APIDECL1 int APIDECL2 SDP_DisableCache(u_int16 flags); 

APIDECL1 int APIDECL2 sdp_service_parse(t_SDP_Results *pResults, t_SDP_service_Parse_Results *service_finded);


#endif /* BLUETOOTH_SDPCLIENT_API_DECLARED */
