/****************************************************************
 *
 * MODULE NAME:    obex_ftp_client.c
 * PROJECT CODE:   Bluetooth host
 * DESCRIPTION:    GOEP layer
 * MAINTAINER:     Mark Donnelly, Kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_ftp_client.c 1532 2010-08-23 05:47:34Z huazeng $
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

#if OBEX_FTP_CLIENT
#include "sdp_stack_access.h"

#include "host_types.h"
#include "host_error.h"
#include "papi.h"
#include "obex_defines.h"
#include "obex_client_types.h"
#include "obex_client_interface.h"
#include "obex_tal.h"
#include "obex_utils.h"

#include "obex_ftp_client.h"

#include "sdp_ue.h"
#include "sdp_constants.h"


/* 
   Globals 
*/
 
extern struct prh_obex_transport* prh_obex_transmit_transport;
extern struct prh_obex_command_callback* prh_obex_client_callbacks;

void (*prh_user_obex_ftp_genpurpose_callback)(u_int8 status, u_int8* data, u_int16 length);

/*static u_int8 prh_ftp_client_lock;*/
t_pEvent prh_obex_ftp_miscEvent;
u_int32 localCid;	
u_int16 prh_ftp_tid;

void prh_obex_followed_by_GetFLO_callback(u_int8 status, u_int8* finger, u_int16 length);
void prh_obex_ftp_gather_callback(u_int8 status, u_int8* finger, u_int16 length);
void prh_obex_ftp_stream_callback(u_int8 status, u_int8* finger, u_int16 length);
void prh_obex_ftp_client_search_callback(u_int8 status);

u_int8 prh_get_list_length(u_int8* data,u_int32 *len,u_int8* offset_len);
u_int8 prh_read_uuid(u_int8* data,u_int32* uuid,u_int8* offset_len);

u_int8 prh_op_get_list_length(u_int8* data,u_int32 *len,u_int8* offset_len);
u_int8 prh_op_read_uuid(u_int8* data,u_int32* uuid,u_int8* offset_len) ;

void (*OBEX_FindFTPServer_callback)(t_bdaddr address,u_int8 channel,u_int8* formats);
void (*FindFTPServer_callback)(struct st_t_BT_ConnectionInfo *dbEntry, t_api status);


 #if (SINGLE_THREAD_FLAG==0)  // jiancui added it. 2008.2.20
/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_ftp_client_search_callback
 * DESCRIPTION:
 

 * KNOWN ISSUES:
 
 *
 *
 ***********************************************************************/
void prh_obex_ftp_client_search_callback(u_int8 status)
{
	pEventSet(prh_obex_ftp_miscEvent);
}
/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_FindFTPServer
 * DESCRIPTION:
 

 * KNOWN ISSUES:
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_FindFTPServer(t_bdaddr* address,u_int8* channel)
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


	if(!address)
		return BT_INVALIDPARAM;

	devs.numItems = 1;
	devs.addresses = (t_bdaddr*)pMalloc(sizeof(t_bdaddr));  
	
	devs.addresses[0] = *address;
	
	criteria = SDP_TRUSTED;
	
	/* search pattern */
	
	pattern.numItems = 1;
	pattern.patternUUIDs = (u_int32*)pMalloc(4);
	pattern.patternUUIDs[0] = SDP_SCLASS_UUID_OBEX_FTP;
	
	/* attributes to retrieve */
	
	attribs.numItems = 1;
	attribs.attributeIDs = (u_int32*)pMalloc(12);
	attribs.attributeIDs[0] = SDP_AID_PROTOCOL;
	
	
	/* stop rule */
	
	rule.maxItems = 100;
	rule.maxDuration = 60;
	rule.maxBytes = 0xf000;
	
	prh_obex_ftp_miscEvent=pEventCreate(FALSE);
	
	/* call a service search and do not retrieve the device name */
		
	SDP_ServiceBrowse(&pResults,&devs,&pattern,&attribs,&rule,criteria,prh_obex_ftp_client_search_callback);    
	
	pEventWait(prh_obex_ftp_miscEvent, NULL, NULL); // TODO: 
	pEventFree(prh_obex_ftp_miscEvent);

	pElement = &(pResults->element);

	if(!pResults->numElements)
		return BT_NOERROR;

	finger = pElement->pData;

	prh_get_list_length(finger,&length,&offset_len);
	finger+= offset_len;
	prh_get_list_length(finger,&length,&offset_len);
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
			prh_get_list_length(finger,&length,&offset_len);
			finger += offset_len;
			counter -= offset_len;
			outer = length;
			do {
				do {
					prh_get_list_length(finger,&length,&offset_len);
					finger +=offset_len;
					counter -=offset_len;
					outer -=offset_len;
					inner = length;
					prh_read_uuid(finger,&uuid,&offset_len);
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
						return BT_NOERROR;
					} else {
						finger += inner;
						counter -= inner;
						outer -= inner;
						inner = 0;
					}					
				} while(inner);
			} while (outer);
			break;
		}
	} while (counter);

	return BT_NOERROR;
}
#else

t_SDP_Results *pResults_ftp;
void prh_obex_ftp_client_search_callback2(struct st_t_BT_ConnectionInfo *dbEntry, t_api status)
{
	u_int8* finger;
	int8 counter;
	int32 outer;
	t_SDP_Element *pElement;
	u_int8 offset_len;
	int32 inner;
	u_int32 length;
	u_int16 aid;
	u_int32 uuid;
	u_int8 channel = 0;
	u_int8* formats=NULL;
	pDebugPrintfEX((pLOGERROR,pLOGOBEX,"prh_obex_ftp_client_search_callback2 will be called,status = %d,pResults_ftp=%x,pResults_ftp->numElements=%d,&pResults_ftp->element=%x\n",status,pResults_ftp,pResults_ftp->numElements,&pResults_ftp->element));
	pElement = &(pResults_ftp->element);
	if(pResults_ftp)
	if(!pResults_ftp->numElements)
		{
			if(pResults_ftp)
			{
				SDP_FreeResults(pResults_ftp);
				pResults_ftp = NULL;
			}
			pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"prh_obex_ftp_client_search_callback2  OBEX_FindFTPServer_callback = %x\n",OBEX_FindFTPServer_callback));
//			OBEX_FindFTPServer_callback( pElement->deviceAddress,0,0);
			return ;
		}
	finger = pElement->pData;
	prh_op_get_list_length(finger,&length,&offset_len);
	finger+= offset_len;
	prh_op_get_list_length(finger,&length,&offset_len);
	finger+= offset_len;
	counter = length;
	#if 0
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
						return BT_NOERROR;
					} else {
						finger += inner;
						counter -= inner;
						outer -= inner;
						inner = 0;
					}
				pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"prh_obex_ftp_client_search_callback2 inner =%d\n",inner));
				} while(inner>0);
			pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"prh_obex_ftp_client_search_callback2 outer =%d\n",outer));
			} while (outer>0);
			break;
		} 
	pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"prh_obex_ftp_client_search_callback2 counter =%d,channel=%d,\n",counter,channel));
	} while (counter>0);
	#endif
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
				pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"prh_obex_op_client_search_callback2 inner =%d\n",inner));
				} while(inner);
			pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"prh_obex_op_client_search_callback2 outer =%d\n",outer));
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
					pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"prh_obex_op_client_search_callback2 length =%d\n",length));
					} while(length);
				}
				break;
		} 
	pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"prh_obex_ftp_client_search_callback2 counter =%d,channel=%d,\n",counter,channel));
	} while (counter);

	OBEX_FindFTPServer_callback( pElement->deviceAddress,channel,formats);
	//asm("break 1");
	if(pResults_ftp)
	{
		SDP_FreeResults(pResults_ftp);
		pResults_ftp = NULL;
	}
	return ;
}
void prh_obex_ftp_client_search_callback(u_int8 status)
{

	if(((pResults_ftp)&&(!pResults_ftp->numElements))||(status != BT_NOERROR))
		{
                    pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"prh_obex_ftp_client_search_callback will be called,status = %d,pResults_ftp=%x,pResults_ftp->numElements=%d\n",status,pResults_ftp,pResults_ftp->numElements));

			t_bdaddr deviceAddress;
			deviceAddress.bytes[0]=0;
			deviceAddress.bytes[1]=1;
			deviceAddress.bytes[2]=0;
			deviceAddress.bytes[3]=1;
			deviceAddress.bytes[4]=0;
			deviceAddress.bytes[5]=1;			
			OBEX_FindFTPServer_callback( deviceAddress,0,NULL);
		}
	return ;
}
APIDECL1 t_api APIDECL2 OBEX_FindFTPServer(t_bdaddr* address,u_int8* channel,u_int8* formats, void (*callbackFunc)(t_bdaddr address,u_int8 channel,u_int8* formats))
{
	t_SDP_Addresses devs;
	u_int16 criteria;
	t_SDP_SearchPattern pattern;
	t_SDP_AttributeIDs attribs;
	t_SDP_StopRule rule;
//	u_int8* finger;
//	u_int8 counter;
//	u_int16 aid;
//	u_int8 offset_len;
//	u_int32 length;
//	u_int32 outer;
//	u_int32 inner;
//	t_SDP_Element *pElement;
	int ret = BT_NOERROR;
	devs.numItems = 1;
	devs.addresses = (t_bdaddr*)pMalloc(sizeof(t_bdaddr));  
	devs.addresses[0] = *address;
	criteria = SDP_TRUSTED;
	OBEX_FindFTPServer_callback = callbackFunc;
	FindFTPServer_callback = 	prh_obex_ftp_client_search_callback2;
	
	pattern.numItems = 1;
	pattern.patternUUIDs = (u_int32*)pMalloc(4);
	pattern.patternUUIDs[0] = SDP_SCLASS_UUID_OBEX_FTP;
	attribs.numItems = 1;
	
	attribs.attributeIDs = (u_int32*)pMalloc(12);
	attribs.attributeIDs[0] = SDP_AID_PROTOCOL;
	rule.maxItems = 100;
	rule.maxDuration = 60;
	rule.maxBytes = 0xf000;
	ret = SDP_ServiceBrowse(&pResults_ftp,&devs,&pattern,&attribs,&rule,criteria,prh_obex_ftp_client_search_callback);    

	pFree(devs.addresses);
	pFree(pattern.patternUUIDs);
	pFree(attribs.attributeIDs);

	return ret;
}

#endif
/***********************************************************************
 *
 * FUNCTION NAME:    prh_get_list_length
 * DESCRIPTION:
 

 * KNOWN ISSUES:
 
 *
 *
 ***********************************************************************/
u_int8 prh_get_list_length(u_int8* data,u_int32 *len,u_int8* offset_len) {
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
 * FUNCTION NAME:    prh_read_uuid
 * DESCRIPTION:
 

 * KNOWN ISSUES:
 
 *
 *
 ***********************************************************************/
u_int8 prh_read_uuid(u_int8* data,u_int32* uuid,u_int8* offset_len) {

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
 * FUNCTION NAME:    OBEX_FTPConnect
 * DESCRIPTION:
 
 This function will Connect to the FTPServer .

 * KNOWN ISSUES:

 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_FTPConnect(u_int32 tid_s, u_int16 mtu, t_DataBuf *txBuffer,u_int16 headerLength, t_obex_command_handler handler)
{
	u_int8 status;

	u_int8 FTP_UUID	[16] = {
	0xf9,0xec,0x7b,0xc4,0x95,0x3c,0x11,0xd2,0x98,0x4e,0x52,0x54,0x00,0xdc,0x9e,0x09};

	/*
	Check for missing/void params
	*/
	if(!handler)
		return BT_INVALIDPARAM;

	prh_user_obex_ftp_genpurpose_callback=handler;
	/*
	Connect
	*/
	status=OBEX_Connect(tid_s,0x10,0,mtu,16,FTP_UUID,headerLength,txBuffer,handler);

	return status;
}


/***********************************************************************
 *
 * FUNCTION NAME:    prh_obex_ftp_gather_callback
 * DESCRIPTION:
		Gathers all of the data b4 sending it to the user callback.
		used for the folder listing object.

 * KNOWN ISSUES:
 
 *
 *
 ***********************************************************************/

void prh_obex_ftp_gather_callback(u_int8 status, u_int8* finger, u_int16 length)
{	
	static u_int8* result_data = 0x00;
	static u_int16 result_length;
	static u_int16 buffer_length;
	
	
	u_int16 offset_len;
	t_obex_header_offsets headers;
	u_int8 retValue;

	t_DataBuf *buffer;

	if(result_data == 0x00)
	{
		result_data = pMalloc(500); 
		result_length = 0;
		buffer_length = 500;
	}

	if(length + result_length > buffer_length)
	{
		u_int8* temp;
		temp = pMalloc(length + result_length);
		buffer_length = result_length + length;
		pMemcpy(temp,result_data,result_length);
		pFree(result_data);
		result_data = temp;
	} 

	offset_len = OBEX_HEADER;
	retValue=OBEX_ExtractHeaders(&headers, &offset_len,finger,length);

	/* check headers were extracted ok*/
	if(retValue)
	{
		prh_user_obex_ftp_genpurpose_callback(BT_UNKNOWNERROR,0x00,0x00);
		return;
	}

	switch(status)
	{
	/*
	Success 
	*/
	case OBEX_OK:

		if(headers.body)
		{
			u_int16 temp;
			/*
			shift 1st byte as it's the msb, add second byte to give the len. 
			-OBEX_HEADER for the header id and length fields
			*/
			temp = ((finger + headers.body)[0] << 8) + (finger + headers.body)[1] -OBEX_HEADER;
			pMemcpy(result_data + result_length, finger + headers.body + 2,temp);
			result_length += temp;
		}

		if(headers.eob)
		{
			u_int16 temp;
			/*
			shift 1st byte as it's the msb, add second byte to give the len. 
			-OBEX_HEADER for the header id and length fields
			*/
			temp = ((finger + headers.eob)[0] << 8) + (finger + headers.eob)[1] -OBEX_HEADER;
			pMemcpy(result_data + result_length, finger + headers.eob + 2,temp);
			result_length += temp;
		}
		/*
		Call the user registered callback function - OBEX_OK indicates it's finished
		so no need to put the below code in the "if(headers.eob)" path.
		*/	
		prh_user_obex_ftp_genpurpose_callback(OBEX_OK,result_data,result_length);
		result_data = 0x00;
		break;
	case OBEX_CONTINUE:
		/*
		Means I have to send another Get with no headers. but there is can still be a body here
		so I must take the data if it exists.
		*/
		if(headers.body)
		{
			/*
			shift 1st byte as it's the msb, add second byte to give the len. 
			-3 for the header id and length fields
			*/
			u_int16 temp;
			temp = ((finger + headers.body)[0] << 8) + (finger + headers.body)[1] -OBEX_HEADER;
			pMemcpy(result_data + result_length, finger + headers.body + 2,temp);
			result_length += temp;
		}

		OBEX_GetWriteBuffer(0,&buffer);
		/*
		Send the Get with no headers
		*/
		OBEX_Get(prh_ftp_tid,1,0,buffer,prh_obex_ftp_gather_callback);
		break;
	default:
		/* error case */
		pFree(result_data);
		result_data = 0x00;
		
		prh_user_obex_ftp_genpurpose_callback(status,finger,length);

		break;
	}

	return;	
}





#if 0
/***********************************************************************
 *
 * FUNCTION NAME:   prh_obex_followed_by_GetFLO_callback
 * DESCRIPTION: 

	callback. when it gets an OK, it sends calls the user callback
	function and then does a OBEX_Get() on the current folder listing
	object. Used in OBEX_FTPConnect and OBEX_ChooseCurrentFolder().

 * KNOWN ISSUES:
 
 .old version of this function - still has code for when this was
 used as the callback to the FTPConnect.

 *
 *
 ***********************************************************************/
void prh_obex_followed_by_GetFLO_callback(u_int8 status, u_int8* finger, u_int16 length)
{	
	u_int8 retValue;
	u_int16 offset_len;
	t_obex_header_offsets headers;
	t_DataBuf *txBuffer;
	
	
	if(connectActive)
		offset_len=OBEX_CONNECT_HEADER;
	else
		offset_len=OBEX_HEADER;

	retValue=OBEX_ExtractHeaders(&headers, &offset_len,finger,length);

	/* check headers were extracted ok*/
	if(retValue)
	{
		prh_user_obex_ftp_genpurpose_callback(BT_UNKNOWNERROR,0x00,0x00);
		return;
	}

	/* I will take this out later as it increases code size */

	switch(*finger)
	{
		/*
		Success - callback with the cid to the user
		*/
		case OBEX_OK:

			/*
			Could be a callback due to a Connect or a SetPath
			*/
			if(connectActive)
			{	if(headers.cid)/*does the cid exist*/
				{
					/*pass to the user*/
					prh_user_obex_ftp_genpurpose_callback(status,finger + headers.cid,4);
					/* revert from the network byte order*/
					localCid=((finger+headers.cid)[0] << 24)+((finger+headers.cid)[1] << 16)+((finger+headers.cid)[2] << 8)+(finger+headers.cid)[3];
					connectActive=0;
				}
				else
				{
					prh_user_obex_ftp_genpurpose_callback(status,finger,length);
					return;
				}
			}
			else /*Must be from a setpath call*/
			{
				prh_user_obex_ftp_genpurpose_callback(status,finger,length);
			}

			/*
			Now that the connection has been successful, we can use OBEX_GET() fn 
			to get the folder listing ob for the root folder
			*/

			/*
			Get the folder-listing object
			*/

			/* Set the offset_len */
			offset_len=OBEX_HEADER;

			/*
			Get the write buffer
			*/
			OBEX_GetWriteBuffer(300,&txBuffer); 
			/*
			Create the headers
			*/
			OBEX_Create4ByteHeader(OBEX_CID,connectCid,txBuffer,&offset_len);
			OBEX_CreateStringHeader(OBEX_NAME,0,0,txBuffer,&offset_len);	
		  OBEX_CreateSequenceHeader(OBEX_TYPE,21,(u_int8 *)"x-obex/folder-listing",txBuffer,&offset_len);
			/*
			use the OBEX_Get() fn
			*/
			OBEX_Get(prh_ftp_tid,1,offset_len,txBuffer,prh_obex_ftp_gather_callback);
	
			return;
	
		
			break;
			/*
			not successful - return status and data to user
			*/
		default:

			prh_user_obex_ftp_genpurpose_callback(status,finger,length);

			break;
	}

	return;
}
#endif

/***********************************************************************
 *
 * FUNCTION NAME:   prh_obex_followed_by_GetFLO_callback
 * DESCRIPTION: 

	callback. when it gets an OK, it sends calls the user callback
	function and then does a OBEX_Get() on the current folder listing
	object. Used in OBEX_FTPConnect and OBEX_ChooseCurrentFolder().

 * KNOWN ISSUES:
 
 .old version of this function - still has code for when this was
 used as the callback to the FTPConnect.

 *
 *
 ***********************************************************************/
void prh_obex_followed_by_GetFLO_callback(u_int8 status, u_int8* finger, u_int16 length)
{	
	u_int8 retValue;
	u_int16 offset_len;
	t_obex_header_offsets headers;
	t_DataBuf *txBuffer;
	
	

	offset_len=OBEX_HEADER;

	retValue=OBEX_ExtractHeaders(&headers, &offset_len,finger,length);

	/* check headers were extracted ok*/
	if(retValue)
	{
		prh_user_obex_ftp_genpurpose_callback(BT_UNKNOWNERROR,0x00,0x00);
		return;
	}

	/* I will take this out later as it increases code size */

	switch(*finger)
	{
		/*
		Success - callback with the cid to the user
		*/
		case OBEX_OK:

			/*
			Must be from a setpath call
			*/
			prh_user_obex_ftp_genpurpose_callback(status,finger,length);
			

			/*********************************************************************
			Now that the everything has been successful, we can use OBEX_GET() fn 
			to get the folder listing ob for the root folder
			**********************************************************************/

			/*
			Get the folder-listing object
			*/

			/* Set the offset_len */
			offset_len=OBEX_HEADER;

			/*
			Get the write buffer
			*/
			OBEX_GetWriteBuffer(300,&txBuffer); 
			/*
			Create the headers
			*/
			OBEX_Create4ByteHeader(OBEX_CID,localCid,txBuffer,&offset_len);
			OBEX_CreateStringHeader(OBEX_NAME,0,0,txBuffer,&offset_len);	
		  OBEX_CreateSequenceHeader(OBEX_TYPE,21,(u_int8 *)"x-obex/folder-listing",txBuffer,&offset_len);
			/*
			use the OBEX_Get() fn
			*/
			OBEX_Get(prh_ftp_tid,1,offset_len,txBuffer,prh_obex_ftp_gather_callback);
			break;

			/*
			not successful - return status and data to user
			*/
		default:

			prh_user_obex_ftp_genpurpose_callback(status,finger,length);

			break;
	}

	return;
}


/***********************************************************************
 *
 * FUNCTION NAME:   OBEX_GetFolderListingObject
 * DESCRIPTION: 
		Gets the folder listing object of the current folder as stored 
		in the ftp server.
	

 * KNOWN ISSUES:
 
 .

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_GetFolderListingObject(u_int32 tid_s, u_int32 cid, t_obex_command_handler handler)
{

	u_int8 status;
	u_int16 offset_len;
	t_DataBuf *txBuffer;

	
	if(!handler)
		return BT_INVALIDPARAM;

	/*
	store the tid
	*/
	prh_ftp_tid=tid_s;

	/*
	store the callback
	*/
	prh_user_obex_ftp_genpurpose_callback=handler;
	
	/*
	Set the offset_len 
	*/
	offset_len=OBEX_HEADER;

	/*
	Get the write buffer
	*/
	status=OBEX_GetWriteBuffer(300,&txBuffer); 

	/*
	Create the headers
	*/

	/*cid*/
	status=OBEX_Create4ByteHeader(OBEX_CID,cid,txBuffer,&offset_len);
	if(status)
		return status;

	/*blank name*/
	
	status=OBEX_CreateStringHeader(OBEX_NAME,0,0,txBuffer,&offset_len);	
	if(status)
		return status;

	/*type*/
	status=OBEX_CreateSequenceHeader(OBEX_TYPE,21,(u_int8 *)"x-obex/folder-listing",txBuffer,&offset_len);
	if(status)
		return status;

	/*
	use the OBEX_Get() fn
	*/
	status=OBEX_Get(prh_ftp_tid,1,offset_len,txBuffer,prh_obex_ftp_gather_callback);

	return status;	
}


/***********************************************************************
 *
 * FUNCTION NAME:   OBEX_FTPPushFile
 * DESCRIPTION: 
		Pushes a file to the ftp server.
	

 * KNOWN ISSUES:
 
 UNTESTED.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_FTPPushFile(u_int32 tid_s, u_int32 cid, u_int16 *name, u_int16 nameLen, u_int8 *data, u_int16 dataLen, u_int32 totalLen,u_int8 final,t_obex_command_handler handler)
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
	Keep a record of the callback function
	*/
	prh_user_obex_ftp_genpurpose_callback=handler;
	
	/*
	set the offset_len
	*/
	offset_len=OBEX_HEADER;

	if(!pushStarted)
	{	
		/*
		Check for missing/void params that are needed only when adding the headers
		*/

		if(!name) /*name header is mandatory*/
			return BT_INVALIDPARAM;

		status=OBEX_GetWriteBuffer((u_int16)(dataLen+300), &txBuffer);
		if(status)
			return status;
		
		/*
		Add CID
		*/
		status=OBEX_Create4ByteHeader(OBEX_CID,cid,txBuffer,&offset_len);
		if(status)
			return status;

		/*
		Add name
		*/
		status=OBEX_CreateStringHeader(OBEX_NAME,nameLen,name,txBuffer,&offset_len);	
		if(status)
			return status;

		/*
		Add total Length
		*/
		if(totalLen)
		{
			status=OBEX_Create4ByteHeader(OBEX_LENGTH,totalLen,txBuffer,&offset_len);
			if(status)
				return status;
		}
		
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

	
	/*up to the user to call this fn several times as necessary => register user callback */
	status=OBEX_Put(tid_s,final,offset_len,txBuffer,handler);
	
	
	return status;	

}

/***********************************************************************
 *
 * FUNCTION NAME:   OBEX_FTPPullFile
 * DESCRIPTION: 
		pulls a file from the server.


 * KNOWN ISSUES:
 
 .

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_FTPPullFile(u_int32 tid_s, u_int32 cid, u_int16 *name, u_int16 nameLen, t_obex_command_handler handler)
{
	u_int8 status;
	t_DataBuf *txBuffer;
	u_int16 offset_len;
	/*
	Check for void/missing params
	*/	
	if(!handler)
		return BT_INVALIDPARAM;
	
	if((!name) || (!nameLen))
		return BT_INVALIDPARAM;

	/*
	Keep a record of the callback function
	*/
	prh_user_obex_ftp_genpurpose_callback=handler;


	/*
	keep a record of the tid
	*/
	prh_ftp_tid=tid_s;

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
	Add CID
	*/
	status=OBEX_Create4ByteHeader(OBEX_CID,cid,txBuffer,&offset_len);
	if(status)
		return status;

	/*
	Add name
	*/
	status=OBEX_CreateStringHeader(OBEX_NAME,nameLen,name,txBuffer,&offset_len);	
	if(status)
		return status;

	/*
	Get the file
	*/
	status=OBEX_Get(tid_s, 1, offset_len, txBuffer, prh_obex_ftp_stream_callback);
	if(status)
		return status;
	
	
	return BT_NOERROR;
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_obex_ftp_stream_callback
 * DESCRIPTION: 
	
		callback that send the data to the user as it comes in.
		if the response code is CONTINUE calls get again.

 * KNOWN ISSUES:
 
 . 

 *
 *
 ***********************************************************************/
void prh_obex_ftp_stream_callback(u_int8 status, u_int8* finger, u_int16 length)
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
				prh_user_obex_ftp_genpurpose_callback(callback_status,finger+headers.body+2,headerLen);
			}

			if(headers.eob)
			{
				headerLen=((finger+headers.eob)[0] << 8) + (finger+headers.eob)[1] - OBEX_HEADER;
				prh_user_obex_ftp_genpurpose_callback(status,finger+headers.eob+2,headerLen);
			}else
			{
				prh_user_obex_ftp_genpurpose_callback(OBEX_METHOD_NOT_ALLOWED,finger,length);
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
				prh_user_obex_ftp_genpurpose_callback(status,finger+headers.body+2,headerLen);
			}

			/*
			Get a write Buffer
			*/
			OBEX_GetWriteBuffer((u_int16)20, &txBuffer);
			/*
			Send the Get with no headers
			*/
			OBEX_Get(prh_ftp_tid,1,0,txBuffer,prh_obex_ftp_stream_callback);	
			break;

		default:
			/*
			pass data to the user
			*/
			prh_user_obex_ftp_genpurpose_callback(status,finger,length);	
			break;

	}/*end switch*/
	
}


/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_Delete
 * DESCRIPTION:
 
 This function will try to delete a file from the FTP server.
 The OBEX_Put() fn is used with no body/eob to achieve to
 indicate a delete operation.

 * KNOWN ISSUES:

 .
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_Delete(u_int32 tid_s, u_int32 cid, u_int16* name, u_int8 nameLen, t_obex_command_handler handler)
{	
	u_int8 status;
	u_int16 t_offset;
	t_DataBuf* buff;
	
	/*
	check for missing/void params
	*/
	if(!name)
		return BT_INVALIDPARAM;
	if(!nameLen)
		return BT_INVALIDPARAM;
	if(!handler)
		return BT_INVALIDPARAM;


	t_offset = OBEX_HEADER;

	status=OBEX_GetWriteBuffer(300,&buff); /* I will need to trim this down a bit */
	if(status)
		return status;

	status=OBEX_Create4ByteHeader(OBEX_CID,cid,buff,&t_offset);
	if(status)
		return status;
	status=OBEX_CreateStringHeader(OBEX_NAME,nameLen,name,buff,&t_offset);
	if(status)
		return status;
	status=OBEX_Put(tid_s,1,t_offset,buff,handler);
	
	return status;

}


/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_CreateFolder
 * DESCRIPTION:
 
 This function tries to create a new folder on the ftp server.
 This is done by using the OBEX_SetPath() fn with a name that
 is not already present on the FTP server.

 * KNOWN ISSUES:

 UNTESTED.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_CreateFolder(u_int32 tid_s,u_int32 cid, u_int16* name,u_int8 length, t_obex_command_handler handler)	
{
	u_int8 status;
	u_int16 t_offset;
	t_DataBuf* buff;

	/*
	check for missing/void params
	*/
	if(!name)
		return BT_INVALIDPARAM;
	if(!length)
		return BT_INVALIDPARAM;
	if(!cid)
		return BT_INVALIDPARAM;
	if(!handler)
		return BT_INVALIDPARAM;

	/*
	Set the offset_len
	*/
	t_offset = OBEX_SETPATH_HEADER;
	
	/*
	store the callback
	*/
	prh_user_obex_ftp_genpurpose_callback=handler;


	status=OBEX_GetWriteBuffer(300,&buff); 
	if(status)
		return status;

	status=OBEX_Create4ByteHeader(OBEX_CID,cid,buff,&t_offset);
	if(status)
		return status;
	status=OBEX_CreateStringHeader(OBEX_NAME,length,name,buff,&t_offset);
	if(status)
		return status;

	status=OBEX_SetPath(tid_s,0,0,t_offset,buff,handler);

	return status;
}


/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_ChooseCurrentFolder
 * DESCRIPTION:
 
 This function will perform one of 3 options, move to the root dir, move to 
 a sub dir, or it will move to the parent directory.

 * KNOWN ISSUES:

	Possiblilty that the user wants an up to date folder-listing object
	of the current directory. create another option to get the same 
	directory as last time.Alternative is let the user go up and then
	back down. Best option for the moment. 

		.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_ChooseCurrentFolder(u_int32 tid_s, u_int32 cid, u_int16* name, u_int8 length,t_obex_command_handler handler)
{
	u_int8 status;
	u_int8 alreadyAssigned;
	u_int16 t_offset;
	t_DataBuf* buff;
	u_int16 reservedString[3];
	u_int8 setPathFlags=0;
	
	/*
	check for missing/void params
	*/
	if(!name)
		return BT_INVALIDPARAM;
	if(!length)
		return BT_INVALIDPARAM;
	if(!cid)
		return BT_INVALIDPARAM;
	if(!handler)
		return BT_INVALIDPARAM;
		
	/*
	Store the cid
	*/
	localCid=cid;

	/*
	set the offset
	*/
	t_offset = OBEX_SETPATH_HEADER;

	alreadyAssigned=0;
	status=OBEX_GetWriteBuffer(300,&buff); /* I will need to trim this down a bit*/
	if(status)
		return status;

	prh_user_obex_ftp_genpurpose_callback=handler;

	reservedString[0]='.';
	reservedString[1]='.';
	reservedString[2]=0x00;
	
	/* check if name indicates to move back a directory */	
	if(!pMemcmp(name,reservedString,6))
	{
		status=OBEX_Create4ByteHeader(OBEX_CID,cid,buff,&t_offset);
		if(status)
			return status;
		/*use flags parameter to indicate that it is a move to root dir*/
		setPathFlags=0x01;
		alreadyAssigned=1;
	}

	reservedString[0]='/';
	reservedString[1]=0x00;

	/* check if name indicates to move to a root dir */
	if(!pMemcmp(name,reservedString,4))
	{
		status=OBEX_Create4ByteHeader(OBEX_CID,cid,buff,&t_offset);
		if(status)
			return status;
		/*empty name*/
		status=OBEX_CreateStringHeader(OBEX_NAME,0,0,buff,&t_offset);
		if(status)
			return status;
		setPathFlags=0x00;
		alreadyAssigned=1;
	}

	/* otherwise the name must be to move into a sub-directory move to the sub dir*/
	if(!alreadyAssigned)
	{ 
		status=OBEX_Create4ByteHeader(OBEX_CID,cid,buff,&t_offset);
		if(status)
			return status;
		status=OBEX_CreateStringHeader(OBEX_NAME,length,name,buff,&t_offset);
		if(status)
			return status;
		/*use flags to indicate- don't create if doesn't exist*/
		setPathFlags=0x02;
	}

	/*
	Call OBEX_SetPath();
	*/
	status=OBEX_SetPath(tid_s,setPathFlags,0,t_offset,buff,prh_obex_followed_by_GetFLO_callback);

	return status;
}


#endif /* OBEX FTP CLIENT */
