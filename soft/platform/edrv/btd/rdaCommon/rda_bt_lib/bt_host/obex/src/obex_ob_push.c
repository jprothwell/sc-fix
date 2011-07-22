/***********************************************************************
 *
 * MODULE NAME:    obex_ob_push.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Object Push Profile
 * MAINTAINER:     Mark Donnelly, Kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_ob_push.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/

#include "host_config.h"

#if OBEX_FTP_SERVER
#include "sdp_stack_access.h"

#include "host_types.h"
#include "papi.h"
#include "obex_ob_push.h"
#include "obex_defines.h"
#include "obex_utils.h"
#include "obex_client_interface.h"
#include "sdp_ue.h"
#include "sdp_constants.h"
#include "l2_types.h"
#include "l2_acl_link.h"


#define PRH_PUT_HEADERS 100
#define PRH_GET_HEADERS 40
#define PRH_SEQUENCE_HEADER 3
#define PRH_GET_NOHEADERS 3
 int  MGR_GetACLState(t_bdaddr addr) ;
 int  MGR_SetACLCallBack(t_bdaddr address, int index , void (*callbackfunc)(void *dbEntry, int status_t));
void prh_l2_acl_timer_expired_obex(void *arg);

void prh_obex_op_stream_callback(u_int8 status, u_int8* finger, u_int16 length);

void (*prh_user_obex_op_pull_callback)(u_int8 status, u_int8* data, u_int16 length);
void (*OBEX_FindObjectPushServer_callback)(t_bdaddr address,u_int8 channel,u_int8* formats);

//void (*FindObjectPushServer_callback)(struct st_t_BT_ConnectionInfo *dbEntry, t_api status);
u_int8 prh_op_get_list_length(u_int8* data,u_int32 *len,u_int8* offset_len);
u_int8 prh_op_read_uuid(u_int8* data,u_int32* uuid,u_int8* offset_len);

u_int32 exchange_tid;

volatile u_int8 retValue;
u_int32 prh_obex_op_tid;
#if (SINGLE_THREAD_FLAG==0)  // jiancui added it. 2008.2.20
t_pEvent prh_obex_op_miscEvent;
#endif



u_int16* prh_a2u(u_int8* string,u_int8* length)
{
	u_int16* result;
	u_int8 lcv = 0;

	while(string[lcv++]);

	*length = lcv;

	for(lcv = 0,result = (u_int16*)pMalloc((*length)<<1); lcv < *length ; lcv++) result[lcv] = string[lcv];

	(*length)--;

	return result;
}

#define OBEX_VCARD 1
#define OBEX_VCAL  2
#define OBEX_VNOTE 3
#define OBEX_VMESS 4
#define OBEX_VOTHER 5
/***********************************************************************
 *
 * FUNCTION NAME:   OBEX_ObjectPush
 * DESCRIPTION: 
	
		This function pushes an object to a peer device.
		.

 * KNOWN ISSUES:
 
 .

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_ObjectPush(u_int32 tid_s, u_int8 type, u_int16 *name, u_int16 nameLen, u_int8 *data, u_int16 dataLen, u_int32 totalLen,u_int8 final,t_obex_command_handler handler)
{
	u_int16 offset_len;
	static u_int8 pushStarted=0;
	t_DataBuf *txBuffer;
	u_int8 status;	

	/*
	Check for void/missing params
	*/	

	if(!dataLen)
		return BT_INVALIDPARAM;

	if(!data)
		return BT_INVALIDPARAM;
	
	if((final !=0) && (final !=1))
		return BT_INVALIDPARAM;

	if(!handler)
		return BT_INVALIDPARAM;
	
	/*
	set the offset_len
	*/
	offset_len=OBEX_HEADER;

	if(!pushStarted)
	{	
		/*
		Check for missing/void params that are needed only when adding the headers
		Mandatory to have a name and totalLen on first push
		*/
		if(!name)
			return BT_INVALIDPARAM;
		if(!totalLen)
			return BT_INVALIDPARAM;

		status=OBEX_GetWriteBuffer((u_int16)(dataLen+300), &txBuffer);
		if(status)
			return status;
		

		/*
		Add the type header - optional
		*/
		if(type)
		{

			switch(type){
			
				case OBEX_VCARD:
					status=OBEX_CreateSequenceHeader(OBEX_TYPE,12,(u_int8 *)"text/x-vCard",(t_DataBuf*)txBuffer,&offset_len);
					if(status)
						return status;
					break;
				case OBEX_VCAL:
					status=OBEX_CreateSequenceHeader(OBEX_TYPE,11,(u_int8 *)"text/x-vCal",(t_DataBuf*)txBuffer,&offset_len);
					if(status)
						return status;
					break;
				case OBEX_VNOTE:
					status=OBEX_CreateSequenceHeader(OBEX_TYPE,12,(u_int8 *)"text/x-vNote",(t_DataBuf*)txBuffer,&offset_len);
					if(status)
						return status;
					break;
				case OBEX_VMESS:
					status=OBEX_CreateSequenceHeader(OBEX_TYPE,12,(u_int8 *)"text/x-vMess",(t_DataBuf*)txBuffer,&offset_len);
					if(status)
						return status;
					break;
				/*
				case OTHER:	
				*/
				default:
					;
			}/*end switch(type)*/
		}/*end of if(type)*/
		
		/*
		Add name
		*/
	
		status=OBEX_CreateStringHeader(OBEX_NAME,nameLen,name,txBuffer,&offset_len);	
		if(status)
			return status;

		/*
		Add total Length
		*/
		status=OBEX_Create4ByteHeader(OBEX_LENGTH,totalLen,txBuffer,&offset_len);
		if(status)
			return status;

		
	}else/*if(pushStarted)*/ /*therefore no need to add above headers*/
	{	
		status=OBEX_GetWriteBuffer((u_int8)(dataLen+300), &txBuffer); 
		if(status)
			return status;
	}
	

	if(!final)
	{
		/*use a BODY header*/
		status=OBEX_CreateSequenceHeader(OBEX_BODY,dataLen,data,txBuffer,&offset_len);
		pushStarted=1;
	}else /*if(final)*/
	{
		/*use a ENDOFBODY header*/
		status=OBEX_CreateSequenceHeader(OBEX_END_BODY,dataLen,data,txBuffer,&offset_len);
		pushStarted=0;
	}
	if(status)
		return status;

	status=OBEX_Put(tid_s,final,offset_len,txBuffer,handler);
	
	return status;	

}


/***********************************************************************
 *
 * FUNCTION NAME:   OBEX_ObjectPull
 * DESCRIPTION: 
	
		This function pulls the default biz card from an object push server.
		the OBEX_Get() function is used. subsequent gets (if necessary) are 
		called in the callback function that is registered here.

 * KNOWN ISSUES:
 
 .

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_ObjectPull(u_int32 tid_s, t_obex_command_handler handler)
{
	u_int8 status;
	t_DataBuf *txBuffer;
	u_int16 offset_len;

	/*
	Check for void/missing params
	*/	
	if(!handler)
		return BT_INVALIDPARAM;

	/*
	Keep a record of the callback function
	*/
	prh_user_obex_op_pull_callback=handler;


	/*
	Set the offset_len
	*/
	offset_len=OBEX_HEADER;

	/*
	Get the write buffer
	*/
	status=OBEX_GetWriteBuffer((u_int16)(300), &txBuffer);
	if(status)
		return status;

	/*
	Add the type header
	*/		
	status=OBEX_CreateSequenceHeader(OBEX_TYPE,12,(u_int8 *)"text/x-vCard",(t_DataBuf*)txBuffer,&offset_len);
	if(status)
		return status;

	/*
	Get the file
	*/
	status=OBEX_Get(tid_s, 1, offset_len, txBuffer, prh_obex_op_stream_callback);
	if(status)
		return status;

	prh_obex_op_tid=tid_s;	
	
	return BT_NOERROR;
}

/***********************************************************************
 *
 * FUNCTION NAME:   prh_obex_op_stream_callback
 * DESCRIPTION: 
	
		callback that send the data to the user as it comes in.
		if the response code is CONTINUE calls get again.

 * KNOWN ISSUES:
 
 . 

 *
 *
 ***********************************************************************/
void prh_obex_op_stream_callback(u_int8 status, u_int8* finger, u_int16 length)
{
		
	u_int16 offset_len;
	t_obex_header_offsets headers;
	u_int16 headerLen;
/*	static u_int8 *data;
	static u_int16 dataOffset=0;*/
	t_DataBuf *txBuffer;
	u_int8 callback_status;

	/*
	Set the offset_len
	*/
	offset_len=OBEX_HEADER;

	OBEX_ExtractHeaders(&headers, &offset_len,finger,length);
	switch(*finger)
	{

		case OBEX_OK:

			if(headers.body)
			{
				/*
				A status of OBEX_OK should only come up when there is an
				eob present. However there may be a body and a eob in 
				the same packet. We only want to pass	up the OBEX_OK status 
				with the eob, so pass up an OBEX_CONTINUE with the body. 
				why ? because the app will want to know when all the data 
				has been recieved and the OBEX_OK is the only	way to know as
				we are abstracting the app from knowing if the data they
				recieve was in a body or an eob header.
				*/
				callback_status=status;
				if(status==0xa0)
				{
					callback_status=0x90;
				}
				
				/*
				shift 1st byte as it's the msb, add second byte to give the len. 
				-3 for the header id and length fields - same for below
				*/
					
				headerLen=((finger+headers.body)[0] << 8) + (finger+headers.body)[1] - OBEX_HEADER;
				prh_user_obex_op_pull_callback(callback_status,finger+headers.body+2,headerLen);
			}

			if(headers.eob)
			{
				headerLen=((finger+headers.eob)[0] << 8) + (finger+headers.eob)[1] - OBEX_HEADER;
				prh_user_obex_op_pull_callback(status,finger+headers.eob+2,headerLen);
			}
	
			break;
		case OBEX_CONTINUE:
			/*
			Means I have to send another Get with no headers. but there is can still be a body here
			so I must take the data.
			*/
			if(headers.body)
			{
				headerLen=((finger+headers.body)[0] << 8) + (finger+headers.body)[1] - OBEX_HEADER;
				prh_user_obex_op_pull_callback(status,finger+headers.body+2,headerLen);
			}

			/*
			Get a write Buffer
			*/
			OBEX_GetWriteBuffer((u_int16)20, &txBuffer);
			/*
			Send the Get with no headers
			*/
			OBEX_Get(prh_obex_op_tid,1,0,txBuffer,prh_obex_op_stream_callback);	
			break;

		default:
			/*
			pass data to the user
			*/
			prh_user_obex_op_pull_callback(status,finger,length);	
			break;

	}/*end switch*/
	
}


 /***********************************************************************
 *
 * FUNCTION NAME:   prh_obex_op_exchange_intermediate_callback
 * DESCRIPTION: 

				Intermediate callback that is used to perform
				the pull part of the echange when the push part has finished.
				first of all pass the result of the push to the user and then
				perform the pull part.	

 * KNOWN ISSUES:
 .
 *
 *
 ***********************************************************************/
void prh_obex_op_exchange_intermediate_callback(u_int8 status, u_int8* finger, u_int16 length)
{
	prh_user_obex_op_pull_callback( status,  finger,  length);

	if(status == OBEX_OK) 
	{
		status=OBEX_ObjectPull(exchange_tid,prh_user_obex_op_pull_callback);
	}

}
 
 
 
 /***********************************************************************
 *
 * FUNCTION NAME:   OBEX_ObjectExchange
 * DESCRIPTION: 
	
		This function exchanges biz cards with a peer device.
		Only biz cards that can be sent in one go can be used with
		this function. Push streaming of biz cards is not possible with 
		this function. If biz cards that cannot be sent in one go need
		to be exchanged with a server, then the OBEX_ObjectPull() and 
		OBEX_ObjectPush() fn must be use explicitly.
		1st callback will be for the push part of the exchange and
		the second and subsequent for the pull part of the exchange.

 * KNOWN ISSUES:
 
 .

 *
 *
 ***********************************************************************/
 APIDECL1 t_api APIDECL2 OBEX_ObjectExchange(u_int32 tid_s, u_int16 *name, u_int16 nameLen, u_int8 *data, u_int16 len, u_int32 totalLen,t_obex_command_handler handler)
{
	u_int8 status;

	status=OBEX_ObjectPush(tid_s,OBEX_VCARD,name,nameLen,data,len,totalLen,1,prh_obex_op_exchange_intermediate_callback);
	if(status)
		return status;

	prh_user_obex_op_pull_callback = handler;
	exchange_tid=tid_s;
	
	return status;
}



#if (SINGLE_THREAD_FLAG==0)  // jiancui added it. 2008.2.20
/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_ftp_client_search_callback
 * DESCRIPTION:
 

 * KNOWN ISSUES:
 
 *
 *
 ***********************************************************************/
void prh_obex_op_client_search_callback(u_int8 status)
{
	pEventSet(prh_obex_op_miscEvent);
}
/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_FindObjectPushServer
 * DESCRIPTION:
 

 * KNOWN ISSUES:
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_FindObjectPushServer(t_bdaddr* address,u_int8* channel, u_int8* formats)
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
	pattern.patternUUIDs[0] = SDP_SCLASS_UUID_OBEX_PUSH;
	
	/* attributes to retrieve */
	
	attribs.numItems = 1;
	attribs.attributeIDs = (u_int32*)pMalloc(12);
	attribs.attributeIDs[0] = SDP_AID_PROTOCOL;
	
	
	/* stop rule */
	
	rule.maxItems = 100;
	rule.maxDuration = 60;
	rule.maxBytes = 0xf000;
	
	prh_obex_op_miscEvent=pEventCreate(FALSE);
	
	/* call a service search and do not retrieve the device name */
		
	SDP_ServiceBrowse(&pResults,&devs,&pattern,&attribs,&rule,criteria,prh_obex_op_client_search_callback);    
	
	pEventWait(prh_obex_op_miscEvent, NULL, NULL); // TODO: 
	pEventFree(prh_obex_op_miscEvent);

	pElement = &(pResults->element);

	if(!pResults->numElements)
		return 0;

	finger = pElement->pData;

	prh_op_get_list_length(finger,&length,&offset_len);
	finger+= offset_len;
	prh_op_get_list_length(finger,&length,&offset_len);
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
			prh_op_get_list_length(finger,&length,&offset_len);
			finger += offset_len;
			counter -= offset_len;
			outer = length;
			do {
				do {
					prh_op_get_list_length(finger,&length,&offset_len);
					finger +=offset_len;
					counter -=offset_len;
					outer -=offset_len;
					inner = length;
					prh_op_read_uuid(finger,&uuid,&offset_len);
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
			case SDP_AID_FORMATS_LIST:
				prh_op_get_list_length(finger,&length,&offset_len);
				finger += offset_len;
				counter -= offset_len;
				if(length)
				{
					do {
						if(finger[1] == 0xff)
						{
							*formats += 1 << 6;
						} else {
							*formats += 1 << (finger[1]-1);
						}
						finger +=2;
						counter -=2;
						length -=2;
					} while(length);
				}
				break;
		} 
	} while (counter);

	return BT_NOERROR;
}
#else
static t_SDP_Results *pResults= NULL;;

void prh_obex_op_client_search_callback2(struct st_t_BT_ConnectionInfo *dbEntry, t_api status)
{
	u_int8* finger;
	u_int8 counter;
	u_int32 outer;
	t_SDP_Element *pElement;
	u_int8 offset_len;
	u_int32 inner;
	u_int32 length;
	u_int16 aid;
	u_int32 uuid;
	u_int8 channel = 0;
	u_int8* formats=NULL;
	
       if(!pResults)
       {
		pDebugPrintfEX((pLOGERROR,pLOGOBEX,"prh_obex_op_client_search_callback3 pResults=0\n"));
		OBEX_FindObjectPushServer_callback( dbEntry->bdAddress,channel,formats);
		return ;
       }
       
	if(pResults->numElements)
	{
		pElement = &(pResults->element);
		finger = pElement->pData;
		prh_op_get_list_length(finger,&length,&offset_len);
		finger+= offset_len;
		prh_op_get_list_length(finger,&length,&offset_len);
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
				prh_op_get_list_length(finger,&length,&offset_len);
				finger += offset_len;
				counter -= offset_len;
				outer = length;
				do {
					do {
						prh_op_get_list_length(finger,&length,&offset_len);
						finger +=offset_len;
						counter -=offset_len;
						outer -=offset_len;
						inner = length;
						prh_op_read_uuid(finger,&uuid,&offset_len);
						finger += offset_len;
						counter -= offset_len;
						outer -= offset_len;
						inner -= offset_len;
						if(uuid == SDP_PROTOCOL_UUID_RFCOMM && inner)
						{
							channel = finger[1];
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
				case SDP_AID_FORMATS_LIST:
					prh_op_get_list_length(finger,&length,&offset_len);
					finger += offset_len;
					counter -= offset_len;
					if(length)
					{
						do {
							if(finger[1] == 0xff)
							{
								*formats += 1 << 6;
							} else {
								*formats += 1 << (finger[1]-1);
							}
							finger +=2;
							counter -=2;
							length -=2;
						} while(length);
					}
					break;
			} 
		} while (counter);
	}
	pDebugPrintfEX((pLOGERROR,pLOGOBEX,"prh_obex_op_client_search_callback2 will be called,status = %d,channel=%x\n",status,channel));	
	OBEX_FindObjectPushServer_callback( dbEntry->bdAddress,channel,formats);
	if(pResults)
	{
		SDP_FreeResults(pResults);
		pResults = NULL;
	}
	return ;
}
void prh_obex_op_client_search_callback(u_int8 status)
{
 	pDebugPrintfEX((pLOGERROR,pLOGOBEX,"prh_obex_op_client_search_callback will be called,status = %d\n",status));

	if(MGR_GetACLState(pResults->element.deviceAddress)!=2)//CONN_ACTIVE 
	{
		if(!MGR_SetACLCallBack(pResults->element.deviceAddress,0,prh_obex_op_client_search_callback2))
			OBEX_FindObjectPushServer_callback( pResults->element.deviceAddress,0,pResults);
		else
		{
 			pDebugPrintfEX((pLOGERROR,pLOGOBEX,"prh_obex_op_client_search_callback MGR_SetACLCallBack failllllllll\n"));
			OBEX_FindObjectPushServer_callback( pResults->element.deviceAddress,0,NULL);
			if(pResults)
			{
				SDP_FreeResults(pResults);
				pResults = NULL;
			}
		}
	}
	else
	{
		OBEX_FindObjectPushServer_callback( pResults->element.deviceAddress,0,NULL);
		if(pResults)
		{
		SDP_FreeResults(pResults);
		pResults = NULL;
		}
	}
	return ;
}

APIDECL1 t_api APIDECL2 OBEX_FindObjectPushServer(t_bdaddr* address,u_int8* channel,u_int8* formats, void (*callbackFunc)(t_bdaddr address,u_int8 channel,u_int8* formats))
{
	t_SDP_Addresses OBEX_devs;
	u_int16 criteria;
	t_SDP_SearchPattern pattern;
	t_SDP_AttributeIDs attribs;
	t_SDP_StopRule rule;
	int ret = BT_NOERROR;
	if(pResults)
	{
		SDP_FreeResults(pResults);
		pResults = NULL;
	}
	OBEX_devs.numItems = 1;
	OBEX_devs.addresses = (t_bdaddr*)pMalloc(sizeof(t_bdaddr));  
	OBEX_devs.addresses[0] = *address;
	criteria = SDP_TRUSTED;
	OBEX_FindObjectPushServer_callback = callbackFunc;

	pattern.numItems = 1;
	pattern.patternUUIDs = (u_int32*)pMalloc(4);
	pattern.patternUUIDs[0] = SDP_SCLASS_UUID_OBEX_PUSH;
	attribs.numItems = 1;
	
	attribs.attributeIDs = (u_int32*)pMalloc(12);
	attribs.attributeIDs[0] = SDP_AID_PROTOCOL;
	rule.maxItems = 100;
	rule.maxDuration = 60;
	rule.maxBytes = 0xf000;
	ret = SDP_ServiceBrowse(&pResults,&OBEX_devs,&pattern,&attribs,&rule,criteria,prh_obex_op_client_search_callback);    

	if(ret != BT_PENDING)
	{
		if(pResults)
		{
			SDP_FreeResults(pResults);
			pResults = NULL;
		}
	}
	pFree(OBEX_devs.addresses);
	pFree(pattern.patternUUIDs);
	pFree(attribs.attributeIDs);
	return ret;
}
#endif
/***********************************************************************
 *
 * FUNCTION NAME:    prh_op_get_list_length
 * DESCRIPTION:
 

 * KNOWN ISSUES:
 
 *
 *
 ***********************************************************************/
u_int8 prh_op_get_list_length(u_int8* data,u_int32 *len,u_int8* offset_len) {
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
 * FUNCTION NAME:    prh_op_read_uuid
 * DESCRIPTION:
 

 * KNOWN ISSUES:
 
 *
 *
 ***********************************************************************/
u_int8 prh_op_read_uuid(u_int8* data,u_int32* uuid,u_int8* offset_len) {

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
 * FUNCTION NAME:    prh_op_read_uuid
 * DESCRIPTION:
 

 * KNOWN ISSUES:
 
 *
 *
 ***********************************************************************/
u_int8 OBEX_L2_ACL_Expired(t_bdaddr* p_bd_addr) {

    prh_t_l2_acl_link* p_link = prh_l2_acl_exists(p_bd_addr);
    if(!p_link)
	return BT_INVALIDPARAM;
    prh_l2_acl_timer_expired_obex(p_link);
	return BT_NOERROR;
}


#endif /*OBEX_FTP_SERVER*/
