/****************************************************************
 *
 * MODULE NAME:    obex_client_interface.c
 * PROJECT CODE:   Bluetooth host
 * DESCRIPTION:    GOEP layer
 * MAINTAINER:     Mark Donnelly, kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_sync_server.c 1532 2010-08-23 05:47:34Z huazeng $
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

#if OBEX_SYNC_SERVER

#include "host_types.h"
#include "host_error.h"
#include "papi.h"
#include "obex_defines.h"
#include "obex_client_types.h"
#include "obex_client_interface.h"
#include "obex_tal.h"
#include "obex_utils.h"
#include "obex_server_interface.h"

#include "obex_sync_server.h"
#include "sdp_constants.h"
#include "sdp_server.h"
#include "sdp_ue.h"


static t_pEvent prh_sync_server_lock;


/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_SuggestSync
 * DESCRIPTION:
 
 This function will send a PUSH command to the IrMC Client

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_SuggestSync(u_int32 tid_s, u_int8 type, char* pAny, u_int8 length,t_obex_command_handler handler)
{
		t_DataBuf* buff;

	// object name is " t e l e c o m / p u s h . t x t ";
	// \r\n
	
	// "telecom/pb.vcf"
	// "telecom/cal.vcs"
	// "telecom/note.vnt"
	// "telecom/inmsg.vmg"
	// "telecom/outmsg.vmg"
	// "telecom/sentmsg.vmg"f
	// "telecom/rtc.txt"
	// "default"

	u_int8 *buffer;
	u_int8 offset_len;
	u_int16 t_offset;
	u_int8 lcv;
	u_int8 obj_name[34];
	pMemcpy(obj_name," t e l e c o m / p u s h . t x t ",34);

	buffer = pMalloc(165 + length);

	offset_len = 0;

	if(type & 1)
	{
		pMemcpy(buffer + offset_len ,"SYNC:",5);
		offset_len += 5;
		pMemcpy(buffer + offset_len , "telecom/pb.vcf\r\n",16);
		offset_len += 16;
	}

	if(type & 2)
	{
		pMemcpy(buffer + offset_len ,"SYNC:",5);
		offset_len += 5;
		pMemcpy(buffer + offset_len , "telecom/cal.vcf\r\n",17);
		offset_len += 17;
	}

	if(type & 4)
	{
		pMemcpy(buffer + offset_len ,"SYNC:",5);
		offset_len += 5;
		pMemcpy(buffer + offset_len , "telecom/note.vcf\r\n",18);
		offset_len += 18;
	}

	if(type & 8)
	{
		pMemcpy(buffer + offset_len ,"SYNC:",5);
		offset_len += 5;
		pMemcpy(buffer + offset_len , "telecom/inmsg.vcf\r\n",19);
		offset_len += 19;
	}

	if(type & 16)
	{
		pMemcpy(buffer + offset_len ,"SYNC:",5);
		offset_len += 5;
		pMemcpy(buffer + offset_len , "telecom/outmgs.vcf\r\n",20);
		offset_len += 20;
	}

	if(type & 32)
	{
		pMemcpy(buffer + offset_len ,"SYNC:",5);
		offset_len += 5;
		pMemcpy(buffer + offset_len , "telecom/sentmgs.vcf\r\n",21);
		offset_len += 21;
	}

	if(type & 64)
	{
		pMemcpy(buffer + offset_len ,"SYNC:",5);
		offset_len += 5;
		pMemcpy(buffer + offset_len , "telecom/rtc.vcf\r\n",17);
		offset_len += 17;
	}

	if(!type)
	{
		pMemcpy(buffer + offset_len ,"SYNC:",5);
		offset_len += 5;
		pMemcpy(buffer + offset_len , "default\r\n",9);
		offset_len += 9;
	}

	if(pAny)
	{
		pMemcpy(buffer + offset_len ,"SYNC:",5);
		offset_len += 5;
		pMemcpy(buffer + offset_len , pAny ,length);

		offset_len += length;
		buffer[offset_len++] = '\r';
		buffer[offset_len++] = '\n';
	}


	OBEX_GetWriteBuffer(300,&buff);
	/*
	buff =  (t_DataBuf*)host_buf_alloc(300);
	host_buf_reserve_header((struct host_buf*)buff,20);
*/
	t_offset = 3;
	
	for(lcv = 0; lcv < 33; lcv+=2)
	{
		obj_name[lcv] = 0;
	}

	OBEX_CreateStringHeader(OBEX_NAME,(u_int16)16,(u_int16*)obj_name,buff,&t_offset);
	OBEX_Create4ByteHeader(OBEX_LENGTH,offset_len,buff, &t_offset);
	OBEX_CreateSequenceHeader(OBEX_BODY,offset_len,buffer,buff,&t_offset);
	OBEX_CreateSequenceHeader(OBEX_END_BODY,0,0,buff,&t_offset);

	OBEX_Put(tid_s,0x01,t_offset,buff,handler);

	return BT_NOERROR;
}

/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_RegisterSyncServer
 * DESCRIPTION:
 
 This function will send a PUSH command to the IrMC Client

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_RegisterSyncServer(u_int8 channel, u_int8 supported_types)
{
	u_int32 handle;
	
	u_int16 _16bit;
	u_int8 _8bit;
	
	t_SDP_serviceRecord *pRecord;
	t_SDP_dataElement e1,e2,e3,e4,e5;
		
	t_SDP_dataElement* list[4];
	
	/* Create a Service Record */ 
	
	SDP_CreateRecord(&handle,&pRecord);
	
	/* service class list */
	
	_16bit = SDP_SCLASS_UUID_IRMC_SYNC;
	
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
	
	SDP_EncodeAttribute(SDP_TEXT,20,(u_int8 *)"IrMC Synchronisation",&e1);
	
	SDP_AddAttribute(pRecord,0x0100,&e1);
	
	/* supported formats */
	_16bit = 0;

	if(supported_types & 0x01)
	{
		_8bit = 1;	
		SDP_EncodeAttribute(SDP_UINT,1,&_8bit,&e1);
		list[_16bit++] = &e1;
	}

	if(supported_types & 0x02)
	{
		_8bit = 3;	
		SDP_EncodeAttribute(SDP_UINT,1,&_8bit,&e2);
		list[_16bit++] = &e2;
	}

	if(supported_types & 0x04)
	{
		_8bit = 5;	
		SDP_EncodeAttribute(SDP_UINT,1,&_8bit,&e3);
		list[_16bit++] = &e3;
	}

	if(supported_types & 0x08)
	{
		_8bit = 6;	
		SDP_EncodeAttribute(SDP_UINT,1,&_8bit,&e4);
		list[_16bit++] = &e4;
	}

	SDP_CreateList(SDP_SEQUENCE,_16bit,list,&e5);
	
	SDP_AddAttribute(pRecord,SDP_AID_DATA_STORES,&e5);


	_16bit = SDP_SCLASS_UUID_PUBLIC;
	
	SDP_EncodeAttribute(SDP_UUID,2,(u_int8*)&_16bit,&e1);
	
	
	list[0] = &e1;
	
	SDP_CreateList(SDP_SEQUENCE,1,list,&e3);
	
	SDP_AddAttribute(pRecord,SDP_AID_BROWSE_LIST,&e3);



	
	return BT_NOERROR;
}
/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_sync_server_search_callback
 * DESCRIPTION:


 * KNOWN ISSUES:

 .
 
 *
 *
 ***********************************************************************/
void prh_obex_sync_server_search_callback(u_int8 status)
{
	pEventSet(prh_sync_server_lock);
}

/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_SyncConnectResponse
 * DESCRIPTION:


 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_SyncConnectResponse(u_int32 tid, u_int16 maxLength,u_int8 responseCode)
{

	u_int8 status;
	u_int32 cid;
	t_DataBuf *txBuffer;
	u_int16 offset_len;
	

	u_int8 SYNC_UUID [9] = "IRMC-SYNC";


	if(!responseCode)
		return BT_INVALIDPARAM;

	/*
	Get the cid
	*/
	status=OBEX_GetCID(tid,16,SYNC_UUID,&cid);
	if(status)
		return status;

	/*
	Get the write buffer
	*/
	status=OBEX_GetWriteBuffer(200,&txBuffer);
	if(status)
		return status;

	
	/*
	Set the offset_len
	*/
	offset_len=OBEX_CONNECT_HEADER;
	
	/*
	add the cid header
	*/
	status=OBEX_Create4ByteHeader(OBEX_CID,cid,txBuffer,&offset_len);
	if(status)
		return status;

	/*
	Add the WHO header
	*/
	status=OBEX_CreateSequenceHeader(OBEX_WHO,9,SYNC_UUID,txBuffer,&offset_len);
	if(status)
		return status;
	
	/*
	Add the MTU size to the txBuffer
	*/
	txBuffer->buf[5]=maxLength >> 8;
	txBuffer->buf[6]=maxLength & 0xff;

	/*
	use OBEX_Response
	*/
	status=OBEX_Response(tid,responseCode,offset_len,txBuffer);

	return status;
}



/***********************************************************************
 *
 * FUNCTION NAME:    prh_sync_get_list_length
 * DESCRIPTION:
 

 * KNOWN ISSUES:
 
 *
 *
 ***********************************************************************/
u_int8 prh_sync_get_list_length(u_int8* data,u_int32 *len,u_int8* offset_len) {
    u_int8* internal;
	
	if(!data)
	{
		return BT_INVALIDPARAM;
	}

	internal = data;
	
    switch(*internal) {

        case SDP_SEQUENCE_SMALL:
        case SDP_UNION_SMALL:
        case SDP_URL_SMALL:
        case SDP_TEXT_SMALL:
            *len = internal[1];
            *offset_len = 2;
            break;

        case SDP_SEQUENCE_MEDIUM:
        case SDP_UNION_MEDIUM:
        case SDP_URL_MEDIUM:
        case SDP_TEXT_MEDIUM:
            *len = (internal[1] << 8) + internal[2];
            *offset_len = 3;
            break;

        case SDP_SEQUENCE_LARGE:
        case SDP_UNION_LARGE:
        case SDP_URL_LARGE:
        case SDP_TEXT_LARGE:
            *len = (internal[1] << 24) + (internal[2] << 16) + (internal[3] << 8) + internal[4];
            *offset_len = 5;
            break;

        default:
            return BT_INVALIDPARAM;
    }

    return BT_NOERROR;
}

/***********************************************************************
 *
 * FUNCTION NAME:    prh_sync_read_uuid
 * DESCRIPTION:
 

 * KNOWN ISSUES:
 
 *
 *
 ***********************************************************************/
u_int8 prh_sync_read_uuid(u_int8* data,u_int32* uuid,u_int8* offset_len) {

	switch(*data) {
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
 * FUNCTION NAME:    OBEX_FindSyncCommandServer
 * DESCRIPTION:


 * KNOWN ISSUES:

 .
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_FindSyncCommandServer(t_bdaddr* address,u_int8* channel)
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
	pattern.patternUUIDs[0] = SDP_SCLASS_UUID_IRMC_COMMAND;
	
	/* attributes to retrieve */
	
	attribs.numItems = 1;
	attribs.attributeIDs = (u_int32*)pMalloc(12);
	attribs.attributeIDs[0] = SDP_AID_PROTOCOL;
	//attribs.attributeIDs[2] = SDP_AID_DATA_STORES;
	
	
	/* stop rule */
	
	rule.maxItems = 100;
	rule.maxDuration = 60;
	rule.maxBytes = 0xf000;
	
	prh_sync_server_lock = pEventCreate(FALSE);
	
	/* call a service search and do not retrieve the device name */
		
	SDP_ServiceBrowse(&pResults,&devs,&pattern,&attribs,&rule,criteria,prh_obex_sync_server_search_callback);    

	
	pEventWait(prh_sync_server_lock);

	if(!pResults->numElements)
	{
		*channel = 0xff;
		return BT_NOERROR;
	}

	pElement = &(pResults->element);


	if(pElement->dataLen == 2)
	{
		*channel = 0xff;
		return BT_NOERROR;
	}

	finger = pElement->pData;

	prh_sync_get_list_length(finger,&length,&offset_len);
	finger+= offset_len;
	prh_sync_get_list_length(finger,&length,&offset_len);
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
			prh_sync_get_list_length(finger,&length,&offset_len);
			finger += offset_len;
			counter -= offset_len;
			outer = length;
			do {
				do {
					prh_sync_get_list_length(finger,&length,&offset_len);
					finger +=offset_len;
					counter -=offset_len;
					outer -=offset_len;
					inner = length;
					prh_sync_read_uuid(finger,&uuid,&offset_len);
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
		default:
			/* I will do this another time */
			break;
		}
	} while (counter);

	return BT_NOERROR;
}




#endif /* OBEX SYNC CLIENT */
