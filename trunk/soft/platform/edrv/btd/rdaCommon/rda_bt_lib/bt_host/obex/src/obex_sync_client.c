/****************************************************************
 *
 * MODULE NAME:    obex_client_interface.c
 * PROJECT CODE:   Bluetooth host
 * DESCRIPTION:    GOEP layer
 * MAINTAINER:     Mark Donnelly, kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_sync_client.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/
 
#include "host_config.h"

#if OBEX_SYNC_CLIENT

#include "host_types.h"
#include "host_error.h"
#include "papi.h"
#include "obex_defines.h"
#include "obex_client_types.h"
#include "obex_client_interface.h"
#include "obex_tal.h"
#include "obex_utils.h"

#include "obex_sync_client.h"
#include "sdp_constants.h"
#include "sdp_server.h"

#include "sdp_ue.h"
#include "sdp_constants.h"


/* 
   Globals 
*/
 
extern struct prh_obex_transport* prh_obex_transmit_transport;
extern struct prh_obex_command_callback* prh_obex_client_callbacks;


t_pEvent prh_sync_client_lock;

/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_RegisterSyncCommandServer
 * DESCRIPTION:
 

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_RegisterSyncCommandServer(u_int8 channel)
{
	u_int32 handle;
	
	u_int16 _16bit;
	u_int8 _8bit;
	t_SDP_serviceRecord *pRecord;
	t_SDP_dataElement e1,e2,e3,e4;
		
	t_SDP_dataElement* list[4];
	
	/* Create a Service Record */ 
	
	SDP_CreateRecord(&handle,&pRecord);
	
	/* service class list */
	
	_16bit = SDP_SCLASS_UUID_IRMC_COMMAND;
	
	SDP_EncodeAttribute(SDP_UUID,2,(u_int8*)&_16bit,&e1);
	
	
	list[0] = &e1;
	
	SDP_CreateList(SDP_SEQUENCE,1,list,&e3);
	
	SDP_AddAttribute(pRecord,SDP_AID_SERVICE_CLASS,&e3);
	
	/* protocol descriptor list */
	
	_16bit = SDP_PROTOCOL_UUID_L2CAP;
	
	SDP_EncodeAttribute(SDP_UUID,2,(u_int8*)&_16bit,&e1);
	
	list[0] = &e1;
	
	SDP_CreateList(SDP_SEQUENCE,1,list,&e2);
	
	_16bit = SDP_PROTOCOL_UUID_RFCOMM;
	
	SDP_EncodeAttribute(SDP_UUID,2,(u_int8*)&_16bit,&e1);
	
	_8bit = channel;
	
	SDP_EncodeAttribute(SDP_UINT,1,&_8bit,&e3);
	
	list[0] = &e1;
	list[1] = &e3;
	
	SDP_CreateList(SDP_SEQUENCE,2,list,&e4);
	
	_16bit = SDP_PROTOCOL_UUID_OBEX;
	
	SDP_EncodeAttribute(SDP_UUID,2,(u_int8*)&_16bit,&e1);
	
	list[0] = &e1;
	
	SDP_CreateList(SDP_SEQUENCE,1,list,&e3);
	
	list[0] = &e2;
	list[1] = &e4;
	list[2] = &e3;
	
	SDP_CreateList(SDP_SEQUENCE,3,list,&e1);
	
	SDP_AddAttribute(pRecord,SDP_AID_PROTOCOL,&e1);
		
	/* profile - list */
	
	_16bit = SDP_SCLASS_UUID_IRMC_SYNC;
	
	SDP_EncodeAttribute(SDP_UUID,2,(u_int8*)&_16bit,&e1);
	
	_16bit = 0x0100;
	
	SDP_EncodeAttribute(SDP_UINT,2,(u_int8*)&_16bit,&e2);
	
	list[0] = &e1;
	list[1] = &e2;
	
	SDP_CreateList(SDP_SEQUENCE,2,list,&e3);
	
	list[0] = &e3;
	
	SDP_CreateList(SDP_SEQUENCE,1,list,&e2);
	
	SDP_AddAttribute(pRecord,SDP_AID_PROFILE_LIST,&e2);
	
	/* service name */
	
	SDP_EncodeAttribute(SDP_TEXT,20,(u_int8 *)"Sync Command Service",&e1);
	
	SDP_AddAttribute(pRecord,0x0100,&e1);
		
	return BT_NOERROR;

}

/***********************************************************************
 *
 * FUNCTION NAME:    get_list_length
 * DESCRIPTION:
 

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/

u_int8 get_list_length(u_int8* data,u_int32 *len,u_int8* offset_len)
{
	u_int8* internal;
	
	internal = data;
	
	switch(*internal)
	{
	case SDP_SEQUENCE_SMALL:
	case SDP_UNION_SMALL:
	case SDP_URL_SMALL:
	case SDP_TEXT_SMALL:
		*len = internal[1];
		*offset_len = SDP_SMALL_HEADER;
		break;
	case SDP_SEQUENCE_MEDIUM:
	case SDP_UNION_MEDIUM:
	case SDP_URL_MEDIUM:
	case SDP_TEXT_MEDIUM:
		*len = (internal[1] << 8) + internal[2];
		*offset_len = SDP_MEDIUM_HEADER;
		break;
	case SDP_SEQUENCE_LARGE:
	case SDP_UNION_LARGE:
	case SDP_URL_LARGE:
	case SDP_TEXT_LARGE:
		*len = (internal[1] << 24) + (internal[2] << 16) + (internal[3] << 8) + internal[4];
		*offset_len = SDP_LARGE_HEADER;
		break;
	default:
		return BT_INVALIDPARAM;
	}
	return BT_NOERROR;
}

/***********************************************************************
 *
 * FUNCTION NAME:    read_uuid
 * DESCRIPTION:
 

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
u_int8 read_uuid(u_int8* data,u_int32* uuid,u_int8* offset_len)
{
	switch(*data)
	{
	case SDP_UUID_2:
		*uuid = (data[1] << 8) + data[2];
		*offset_len = 3;
		break;
	case SDP_UUID_4:
		*uuid = (data[1] << 24) + (data[2] << 16) + (data[3] << 8) + data[4];
		*offset_len = 5;
		break;
	case SDP_UUID_16:
		*uuid = (data[1] << 24) + (data[2] << 16) + (data[3] << 8) + data[4];
		*offset_len = 17;
		break;
	default:
		return BT_INVALIDPARAM;
	}
	return BT_NOERROR;
}

/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_sync_client_search_callback
 * DESCRIPTION:
 

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
void prh_obex_sync_client_search_callback(u_int8 status)
{
	pEventSet(prh_sync_client_lock);
}
/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_FindSyncServer
 * DESCRIPTION:
 

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_FindSyncServer(t_bdaddr* address,u_int8* channel,u_int8* formats)
{
	u_int32 uuid;
	t_SDP_Addresses devs;
	u_int16 criteria;
	t_SDP_SearchPattern pattern;
	t_SDP_AttributeIDs attribs;
	
	t_SDP_StopRule rule;
	
	t_SDP_Results *pResults;
	u_int8* finger;
	u_int8 counter;
	u_int16 aid;
	u_int8 offset_len;
	u_int32 length;
	u_int32 outer;
	u_int32 inner;
	t_SDP_Element *pElement;


	devs.numItems = 1;
	devs.addresses = (t_bdaddr*)pMalloc(sizeof(t_bdaddr));  
	
	devs.addresses[0] = *address;
	
	criteria = SDP_TRUSTED;
	
	/* search pattern */
	
	pattern.numItems = 1;
	pattern.patternUUIDs = (u_int32*)pMalloc(4);
	pattern.patternUUIDs[0] = SDP_SCLASS_UUID_IRMC_SYNC;
	
	/* attributes to retrieve */
	
	attribs.numItems = 2;
	attribs.attributeIDs = (u_int32*)pMalloc(12);
	attribs.attributeIDs[0] = SDP_AID_PROTOCOL;
	attribs.attributeIDs[1] = SDP_AID_DATA_STORES;
	
	
	/* stop rule */
	
	rule.maxItems = 100;
	rule.maxDuration = 60;
	rule.maxBytes = 0xf000;
	
	prh_sync_client_lock = pEventCreate(FALSE);
	
	
	/* call a service search and do not retrieve the device name */
		
	SDP_ServiceBrowse(&pResults,&devs,&pattern,&attribs,&rule,criteria,prh_obex_sync_client_search_callback);
	
	pEventWait(prh_sync_client_lock);

	if(!pResults->numElements)
	{
		*channel = 0xff;
		*formats = 0xff;
		return BT_NOERROR;
	}

	pElement = &(pResults->element);

	finger = pElement->pData;

	get_list_length(finger,&length,&offset_len);
	finger+= offset_len;
	get_list_length(finger,&length,&offset_len);
	finger+= offset_len;
	counter = length;
	do {
		counter--;
		finger++; /* bypass type code */
		aid = (finger[0] << 8) + finger[1];
		finger+=2;
		counter-=2;
		switch(aid)
		{
		case SDP_AID_PROTOCOL:
			get_list_length(finger,&length,&offset_len);
			finger += offset_len;
			counter -= offset_len;
			outer = length;
			do {
				do {
					get_list_length(finger,&length,&offset_len);
					finger +=offset_len;
					counter -=offset_len;
					outer -=offset_len;
					inner = length;
					read_uuid(finger,&uuid,&offset_len);
					finger += offset_len;
					counter -= offset_len;
					outer -= offset_len;
					inner -= offset_len;
					if(uuid == SDP_PROTOCOL_UUID_RFCOMM && inner)
					{
						*channel = finger[1];
						finger+=2;
						counter-=2;
						inner-=2;
						outer-=2;
					} else {
						finger += inner;
						counter -= inner;
						outer -= inner;
						inner = 0;
					}					
				} while(inner);
			} while (outer);
			break;
		case SDP_AID_DATA_STORES:
			get_list_length(finger,&length,&offset_len);
			/* I will need to check the type code here for completeness */
			finger += offset_len;
			counter -= offset_len;
			if(length)
			{
				do {
					if(finger[0] != SDP_UINT_1)
						return BT_UNKNOWNERROR;

					if(finger[1] == 0xff)
					{
//						pList->devices[index].types += 1 << 6;
					} else {
//						pList->devices[index].types += 1 << (finger[1]-1);
					}
					finger +=2;
					counter -=2;
					length -=2;
				} while(length);
			}
			break;
		default:
			/* I will do this another time */
			break;
		}
	} while (counter);

	return BT_NOERROR;
}




#endif /* OBEX SYNC CLIENT */
