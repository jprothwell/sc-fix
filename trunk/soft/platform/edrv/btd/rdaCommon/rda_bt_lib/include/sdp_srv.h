/***********************************************************************
 *
 * MODULE NAME:    sdp_srv.h
 * DESCRIPTION:    Bluetooth Host Stack SDP API server header
 * AUTHOR:         Mark Donnelly, Martin Mellody
 *
 * SOURCE CONTROL: $Id: sdp_srv.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 * REMARKS:
 *     This file should be incuded via bt.h only.
 *
 ***********************************************************************/


#ifndef BLUETOOTH_SDPSERVER_API_DECLARED
#define BLUETOOTH_SDPSERVER_API_DECLARED



enum sdp_data_types{
  SDP_UINT = 1,
  SDP_INT = 2,
  SDP_UUID = 3,
  SDP_TEXT = 4,
  SDP_BOOLEAN = 5,
  SDP_SEQUENCE = 6,
  SDP_UNION = 7,
  SDP_URL = 8
};


typedef struct st_t_SDP_dataElement {
  u_int8 size;
  u_int8 type;
  u_int32 data_length;
  u_int8 *data;
} t_SDP_dataElement;


typedef struct st_t_SDP_attribute {
  u_int16 id;
  t_SDP_dataElement value;
  struct st_t_SDP_attribute *nextAttribute;
} t_SDP_attribute;


typedef struct st_t_SDP_serviceRecord {
  t_SDP_attribute attribute;
  struct st_t_SDP_serviceRecord *nextRecord;
} t_SDP_serviceRecord;



APIDECL1 int APIDECL2 SDP_CreateRecord(u_int32 *recordHandle, t_SDP_serviceRecord **record);
APIDECL1 int APIDECL2 SDP_RemoveRecord(u_int32 recordHandle);

APIDECL1 int APIDECL2 SDP_EncodeAttribute(u_int8 attributeType, u_int32 attributeSize, u_int8 *dataBuf, t_SDP_dataElement *encodedAttrib);
APIDECL1 int APIDECL2 SDP_AddAttribute(t_SDP_serviceRecord *destRecord, u_int16 attributeID, t_SDP_dataElement *srcData);
APIDECL1 int APIDECL2 SDP_ReplaceAttribute(t_SDP_serviceRecord *destRecord, u_int16 attributeID , t_SDP_dataElement *srcData);

APIDECL1 int APIDECL2 SDP_CreateList(u_int8 listType, u_int32 numElements, t_SDP_dataElement **listElements, t_SDP_dataElement *createdList);

APIDECL1 int APIDECL2 SDP_GetNextRecord(u_int32 currentRecordHandle, u_int32 *nextRecordHandle);
APIDECL1 int APIDECL2 SDP_GetRecord(u_int32 recordHandle, t_SDP_serviceRecord **record);


/* states for SDP_SetRecordState() */
#define SDP_RECORD_UNAVAILABLE  1
#define SDP_RECORD_AVAILABLE    2

APIDECL1 int APIDECL2 SDP_SetRecordState(u_int32 recordHandle, u_int8 state);


APIDECL1 int APIDECL2 SDP_StartServer(u_int16 flags, void *reserved);
APIDECL1 int APIDECL2 SDP_StopServer(u_int16 flags, void *reserved); 


#endif /* BLUETOOTH_SDPSERVER_API_DECLARED */
