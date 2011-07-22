/****************************************************************
 *
 * MODULE NAME:    obex_ftp_server.c
 * PROJECT CODE:   Bluetooth host
 * DESCRIPTION:    GOEP layer
 * MAINTAINER:     Mark Donnelly, Kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_ftp_server.c 1532 2010-08-23 05:47:34Z huazeng $
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

#if OBEX_FTP_SERVER

#include "host_types.h"
#include "host_error.h"
#include "papi.h"
#include "obex_defines.h"
#include "obex_client_types.h"
#include "obex_server_interface.h"
#include "obex_tal.h"
#include "obex_utils.h"

#include "obex_ftp_server.h"
#include "sdp_constants.h"
#include "sdp_server.h"
#include "sdp_ue.h"

/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_RegisterFTPServer
 * DESCRIPTION:
 
 This function will send a PUSH command to the IrMC Client

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_RegisterFTPServer(u_int8 channel)
{
	u_int32 handle;
	
	u_int16 _16bit;
	u_int8 _8bit;
	
	t_SDP_serviceRecord *pRecord;
	t_SDP_dataElement e1,e2,e3,e4;
		
	t_SDP_dataElement* list[3];
	
	if(!channel)
		return BT_INVALIDPARAM;


	/* Create a Service Record */ 
	
	SDP_CreateRecord(&handle,&pRecord);
	
	/* service class list */
	
	_16bit = SDP_SCLASS_UUID_OBEX_FTP;
	
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
	
	_16bit = SDP_SCLASS_UUID_OBEX_FTP;
	
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
	
	SDP_EncodeAttribute(SDP_TEXT,18,(u_int8 *)"OBEX File Transfer",&e1);
	
	SDP_AddAttribute(pRecord,0x0100,&e1);
		
	return BT_NOERROR;
}


/***********************************************************************
 *
 * FUNCTION NAME:    OBEX_ChooseCurrentFolderResponse
 * DESCRIPTION:


 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_ChooseCurrentFolderResponse(u_int32 tid_s, u_int8 responseCode, u_int8 *folderListingOb, u_int8 floLen, u_int16 totalLen)
{
	
	u_int8 status;
	t_DataBuf *txBuffer;
	u_int16 offset_len;
	static u_int8 responseStarted=0;
	
	
	if(!folderListingOb)
		return BT_INVALIDPARAM;

	if(folderListingOb && !floLen)
		return BT_INVALIDPARAM;

	/*
	Get the write buffer
	*/
	status=OBEX_GetWriteBuffer(400,&txBuffer);
	if(status)
		return status;

	/*
	Set the offset_len
	*/
	offset_len=OBEX_HEADER;

	if(!responseStarted)
	{
		/*
		Add total Length
		*/
		if(totalLen)
		{
			status=OBEX_Create4ByteHeader(OBEX_LENGTH,totalLen,txBuffer,&offset_len);
			if(status)
				return status;
		}
	}


	/*
	add the Data in a BODY/EOB header
	*/
	if(folderListingOb)
	{
		if(responseCode==0x09)/*CONTINUE : there will be more => use a BODY*/
		{
			status=OBEX_CreateSequenceHeader(OBEX_BODY,floLen,folderListingOb,txBuffer,&offset_len);
			responseStarted=1;
		}else{
		/*if(responseCode!=0x09)- could be SUCCESS or could be failure with a description - use an EOB*/
			status=OBEX_CreateSequenceHeader(OBEX_END_BODY,floLen,folderListingOb,txBuffer,&offset_len);
			responseStarted=0;
		}
		if(status) 
			return status;
	}
		
	/*
	use OBEX_Response
	*/
	status=OBEX_Response(tid_s,responseCode,offset_len,txBuffer);

	return BT_NOERROR;
}

/***********************************************************************
 *
 * FUNCTION NAME:   OBEX_FTPPullResponse
 * DESCRIPTION: 
	
		

 * KNOWN ISSUES:
			UNTESTED.
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_FTPPullResponse(u_int32 tid_s, u_int8 responseCode, u_int16 *name, u_int8 nameLen, u_int16 totalLen, u_int8* data, u_int16 dataLen)
{
	t_DataBuf *txBuffer;
	u_int8 status;
	u_int16 offset_len;
	static u_int8 responseStarted=0;
	
	if(!data)
		return BT_INVALIDPARAM;

	/*
	if data and no data length - return
	*/
	if((data) && (!dataLen))
		return BT_INVALIDPARAM;

	/*
	if there is no data ensure dataLen=0, so the proper size write buffer can be allocated
	*/
	if(!data)
		dataLen=0;
	
	/*
	Set the initial offset_len
	*/
	offset_len=OBEX_HEADER;

	/*
	Get a write buffer
	*/
	status=OBEX_GetWriteBuffer((u_int16)(dataLen+nameLen+3), &txBuffer);
	if(status)
		return status;
	
	/* Only add these headers for the first response*/
	if(!responseStarted)
	{
		/*
		Add total Length
		*/
		if(totalLen)
		{
			status=OBEX_Create4ByteHeader(OBEX_LENGTH,totalLen,txBuffer,&offset_len);
			if(status)
				return status;
		}
		
		/*
		If there is a name header and a valid length add them.
		*/
		if(name && nameLen)
		{
			status=OBEX_CreateStringHeader(OBEX_NAME,nameLen,name,txBuffer,&offset_len);
			if(status)
				return status;
		}
	}
		
	/*
	add the Data in a BODY/EOB header
	*/
	if(data)
	{
		if(responseCode==0x09)/*CONTINUE : there will be more => use a BODY*/
		{
			status=OBEX_CreateSequenceHeader(OBEX_BODY,dataLen,data,txBuffer,&offset_len);
			responseStarted=1;
		}else{
		/*if(responseCode!=0x09)- could be SUCCESS or could be failure with a description - use an EOB*/
			status=OBEX_CreateSequenceHeader(OBEX_END_BODY,dataLen,data,txBuffer,&offset_len);
			responseStarted=0;
		}
		if(status) 
			return status;
	}
	/*
	use the OBEX_Response() function to send to the client
	*/
	status=OBEX_Response(tid_s, responseCode, offset_len, txBuffer);
	
	return status;
}


#endif /* OBEX FTP SERVER */
