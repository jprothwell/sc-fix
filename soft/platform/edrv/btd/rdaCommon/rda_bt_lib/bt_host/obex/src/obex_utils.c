/***********************************************************************
 *
 * MODULE NAME:    obex_utils.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Generic Object Exchange Profile
 * MAINTAINER:     Mark Donnelly, Kevin Traynor
 *
 * SOURCE CONTROL: $Id: obex_utils.c 1532 2010-08-23 05:47:34Z huazeng $
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
#include "host_types.h"
#include "sdp_le.h"
#include "obex_utils.h"
#include "obex_defines.h"
#include "papi.h"


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_write16
 * DESCRIPTION:
 
 This function reads a u_int16 from a pdu

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
void prh_sdp_write16_(u_int16 data, u_int8* buffer)
{
	buffer[1] = data >> 8;
	buffer[0] = data & 0xff;
}

/***********************************************************************
 *
 * FUNCTION NAME:   OBEX_CreateStringHeader
 * DESCRIPTION:
 
 This function will allocate space for a header. It will then create the
header with the supplied data. It will also add the header to the data
buffer. The string input to the function should be unicode text.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_CreateStringHeader(u_int8 id, u_int16 length, u_int16* pData, t_DataBuf *txBuffer, u_int16* pOffset)
{
	u_int8 idTest;
	struct host_buf* h_buff; 
	u_int8 lcv;
	
	h_buff = (struct host_buf*) txBuffer; /* cast to host_buff*/
	
	/*check that a valid length was passed in*/
	/*	if(length && !pData)
	return BT_INVALIDPARAM;
	*/
	if(!length && pData)
		return BT_INVALIDPARAM;
	
	/*check that there is a valid offset*/
	if (!pOffset)
		return BT_INVALIDPARAM;		
	
	
	if(length && pData) /*if no data don't care what length is, only an empty header is added*/
	{
		length += length +2 +OBEX_HEADER; /* 2 bytes are required for each charcter in unicode +2 for null*/	
	} else {
		length = OBEX_HEADER;/* 3 bytes for the hi and length */
	}
	
	/*check that there is enough room in the txBuffer for the data*/
	if(txBuffer)
	{		
		if(txBuffer->bufLen > *pOffset) /* make sure the buffer is bigger than the offset*/
		{
			if((u_int16)(txBuffer->bufLen - *pOffset)< length) /* check that there is enough space for the data and headers */
				return BT_NORESOURCES;
		}
		else{
			return BT_NORESOURCES;
		}
		
	} else 
		return BT_INVALIDPARAM;
	
	
	/*check that the first 2 MSB's have the right encoding (00)*/
	idTest = id;	
	idTest |= 0x3f;		/*mask bits 1 to 6*/
	if (idTest != 0x3f)
		return BT_INVALIDPARAM;
	
	
	h_buff->data[*pOffset] = id;

	//prh_sdp_write16(length,test);
	/*h_buff->data[*pOffset + 1]= length >>8;
	h_buff->data[*pOffset + 2]= length & 0xff;*/
	prh_sdp_write16(length,&h_buff->data[*pOffset + 1]);

	
	//if(length !=3)
	//	pMemcpy((h_buff->data +*pOffset) + 3, pData, length-3);/*copy to after the length*/
	/*3 = 1 for id + 2 for length*/
	

	if(length != OBEX_HEADER)
		for(lcv = 0; lcv < length -OBEX_HEADER; lcv+=2)
		{
			
			/*((h_buff->data +*pOffset) + OBEX_HEADER)[lcv] = pData[lcv>>1] >> 8;
			((h_buff->data +*pOffset) + OBEX_HEADER)[lcv+1] = pData[lcv>>1] & 0xff;*/
			prh_sdp_write16_(pData[lcv>>1],&((h_buff->data +*pOffset) + OBEX_HEADER)[lcv]);
//			prh_sdp_write16(pData[lcv>>1],&((h_buff->data +*pOffset) + OBEX_HEADER)[lcv]);
		}
		
		*pOffset += length;	
		
		return BT_NOERROR;
		
}
 

/***********************************************************************
 *
 * FUNCTION NAME:   OBEX_CreateSequenceHeader
 * DESCRIPTION:
 
 This function will allocate space for a header. It will then create the
header with the supplied data.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_CreateSequenceHeader(u_int8 id, u_int16 length, u_int8* pData, t_DataBuf *txBuffer, u_int16* pOffset)
{
	u_int8 idTest;
	struct host_buf* h_buff; 
	h_buff = (struct host_buf*) txBuffer; /* cast to host_buff*/

	/*check that a valid length was passed in*/
/*	if(length == 0)
		return BT_INVALIDPARAM;
*/

	/*check that if there is no length, that there is no data also*/
/*	if (length && !pData)
		return BT_INVALIDPARAM;		
*/
	if (!length && pData)
		return BT_INVALIDPARAM;		

	/*check that there is a valid offset*/
	if (!pOffset)
		return BT_INVALIDPARAM;		

	
	if(pData)
	{
		length +=OBEX_HEADER;
	}else{
		length =OBEX_HEADER;
	}

	/*check that there is enough room in the txBuffer for the data*/
	if(txBuffer)
	{		
		if(txBuffer->bufLen > *pOffset) /* make sure the buffer is bigger than the offset*/
		{
			if((u_int16)(txBuffer->bufLen - *pOffset) < length) /* check that there is enough space for the data and headers */
				return BT_NORESOURCES;
		}
		else{
			return BT_NORESOURCES;
		}

	} else 
		return BT_INVALIDPARAM;

	/*check that the first 2 MSB's have the right encoding (01)*/
	idTest = id;
	idTest |= 0x3f;
	if (idTest != 0x7f)
		return BT_INVALIDPARAM;		


	h_buff->data[*pOffset] = id;

	/*h_buff->data[*pOffset + 1]= length >>8;
	h_buff->data[*pOffset + 2]= length & 0xff; */

	prh_sdp_write16(length,&h_buff->data[*pOffset + 1]);

	if (length !=OBEX_HEADER) /*this means there is no data also*/
		pMemcpy((h_buff->data +*pOffset) + OBEX_HEADER, pData, length -OBEX_HEADER);/*copy to after the length*/
															 /*3 = 1 for id + 2 for length*/
	*pOffset += length;
	
	return BT_NOERROR;

}

/***********************************************************************
 *
 * FUNCTION NAME:   OBEX_CreateByteHeader
 * DESCRIPTION:
 
 This function will allocate space for a header. It will then create the
header with the supplied data.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_CreateByteHeader(u_int8 id, u_int8 data, t_DataBuf* txBuffer, u_int16* pOffset)
{

	u_int8 idTest;
	struct host_buf* h_buff; 
	
	h_buff = (struct host_buf*) txBuffer; /* cast to host_buff*/	

	/*check that there is a valid offset*/
	if (!pOffset)
		return BT_INVALIDPARAM;	

	/*check that there is enough room in the txBuffer for the data*/
	if(txBuffer)
	{		
		if(txBuffer->bufLen > *pOffset) /* make sure the buffer is bigger than the offset*/
		{
			if((u_int16)(txBuffer->bufLen - *pOffset) < 2) /* check that there is enough space for the data and headers */
				return BT_NORESOURCES;
		}
		else{
			return BT_NORESOURCES;
		}

	} else 
		return BT_INVALIDPARAM;

	/*check that the first 2 MSB's have the right encoding (10)*/
	idTest = id;	
	idTest |= 0x3f; 
	if (idTest != 0xbf)
		return BT_INVALIDPARAM;		

	
	h_buff->data[*pOffset] = id;
	h_buff->data[*pOffset + 1] = data;

	*pOffset += 2;
					
	return BT_NOERROR;

}
/***********************************************************************
 *
 * FUNCTION NAME:   OBEX_Create4ByteHeader
 * DESCRIPTION:
 
 This function will allocate space for a header. It will then create the
header with the supplied data.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_Create4ByteHeader(u_int8 id, u_int32 data, t_DataBuf *txBuffer, u_int16* pOffset)
{	
	u_int8 idTest;
	struct host_buf* h_buff; 

	h_buff = (struct host_buf*) txBuffer; /* cast to host_buff*/

	/*check that there is a valid offset*/
	if (!pOffset)
		return BT_INVALIDPARAM;		

	/*check that there is enough room in the txBuffer for the data*/
	if(txBuffer)
	{		
		if(txBuffer->bufLen > *pOffset) /* make sure the buffer is bigger than the offset*/
		{
			if((u_int16)(txBuffer->bufLen - *pOffset) < 5) /* check that there is enough space for the data and headers */
				return BT_NORESOURCES;
		}
		else{
			return BT_NORESOURCES;
		}

	} else 
		return BT_INVALIDPARAM;

	/*check that the first 2 MSB's have the right encoding (11)*/
	idTest = id;	

	idTest |= 0x3f;
	if (idTest != 0xff)
		return BT_INVALIDPARAM;		

	h_buff->data[*pOffset] = id;
	
	/*
	h_buff->data[*pOffset + 1] = (data >> 24);
	h_buff->data[*pOffset + 2] = (data >> 16);
	h_buff->data[*pOffset + 3] = (data >> 8);
	h_buff->data[*pOffset + 4] = data;
*/
	prh_sdp_write32(data,&h_buff->data[*pOffset + 1]);
	*pOffset += 5;
	
	return BT_NOERROR;

	
}

/***********************************************************************
 *
 * FUNCTION NAME:   OBEX_GetWriteBuffer
 * DESCRIPTION:
 
Allocates a data buffer suitable for passing to Obex primitives

 * KNOWN ISSUES:

 gets the amount of space for l2cap headers and the minimum needed for
 RFCOMM and Obex. #define tbd for this.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_GetWriteBuffer(u_int16 len, t_DataBuf **writeBuffer) {

    struct host_buf *buf;

	/*
	check parameters 
	*/

	if(!writeBuffer)
		return BT_INVALIDPARAM;

	if(len < OBEX_HEADER)
	{
		len = 9 + 6;
	} else {
		len = 9 + 3 + len; /* l2 + rf + ob + len */
	}	
	  u_int32  nCallerAdd = 0x00;
	  COS_GET_RA(&nCallerAdd);
	buf=host_buf_alloc(len + 9);

	if (!buf)
		return BT_NORESOURCES;
  COS_UpdateMemCallAdd(buf,nCallerAdd);
    host_buf_reserve_header(buf,9);
	
    *writeBuffer=(struct st_t_dataBuf *)buf;
	
    return BT_NOERROR;
	
}

/***********************************************************************
 *
 * FUNCTION NAME:   OBEX_FreeWriteBuffer
 * DESCRIPTION:
 
Releases a data buffer as allocated by OBEX_GetWriteBuffer

 * KNOWN ISSUES:
none.

 *
 *
 ***********************************************************************/

APIDECL1 t_api APIDECL2 OBEX_FreeWriteBuffer(t_DataBuf *writeBuffer) {

	host_buf_free((struct host_buf *)writeBuffer); 
  return BT_NOERROR;

}

/***********************************************************************
 *
 * FUNCTION NAME:   OBEX_ExtractHeaders
 * DESCRIPTION:
 Extracts the headers.

 * KNOWN ISSUES:
none.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 OBEX_ExtractHeaders(t_obex_header_offsets* headers,u_int16* offset_len,u_int8* finger, u_int16 len)
{
	u_int16 temp_len;
	char* name;
	char* type;
	/*static u_int8 incoming=0;*/

	pMemset(headers,0,sizeof(t_obex_header_offsets));

	name = type = 0x00;
	
	len	-= *offset_len;
	temp_len=0;
	if(len)
	{
		do
		{	
			len--;	
			if (finger == NULL)
				asm("break 1");
			switch(finger[(*offset_len)++])
			{
			case OBEX_COUNT :
				headers->count = *offset_len;
				*offset_len +=4;
				len -=4;
				break;
			case OBEX_NAME :
				headers->name = *offset_len;
				/*
				temp_len = (finger[*offset_len] << 8) + finger[*offset_len+1];
				*/
				temp_len = prh_sdp_read16(finger + *offset_len);

				*offset_len += temp_len -1;
				len -= temp_len -1;
				break;
			case OBEX_TYPE :
				headers->type = *offset_len;
//				temp_len = (finger[*offset_len] << 8) + finger[*offset_len+1];
				temp_len = prh_sdp_read16(finger + *offset_len);

				*offset_len += temp_len -1;
				len -= temp_len -1;
				break;
			case OBEX_LENGTH :
				headers->len = *offset_len;
				*offset_len += 4;
				len -=4;
				break;
			case OBEX_TIME_ISO :
				headers->time_iso = *offset_len;
//				temp_len = (finger[*offset_len] << 8) + finger[*offset_len+1];
				temp_len = prh_sdp_read16(finger + *offset_len);
				*offset_len += temp_len -1;
				len -= temp_len -1;
				break;			
			case OBEX_TIME_32 :
				headers->time_32 = *offset_len;
				*offset_len +=4;
				len -=4;
				break;
			case OBEX_DESCRIPTION :
				headers->des = *offset_len;
//				temp_len = (finger[*offset_len] << 8) + finger[*offset_len+1];
				temp_len = prh_sdp_read16(finger + *offset_len);
				*offset_len += temp_len -1;
				len -= temp_len -1;
				break;
			case OBEX_TARGET :
				headers->tar = *offset_len;
//				temp_len = (finger[*offset_len] << 8) + finger[*offset_len+1];
				temp_len = prh_sdp_read16(finger + *offset_len);
				*offset_len += temp_len -1;
				len -= temp_len -1;
				break;
			case OBEX_HTTP :
				headers->http = *offset_len;
//				temp_len = (finger[*offset_len] << 8) + finger[*offset_len+1];
				temp_len = prh_sdp_read16(finger + *offset_len);
				*offset_len += temp_len -1;
				len -= temp_len -1;
				break;			
			case OBEX_BODY :
				headers->body = *offset_len;
//				temp_len = (finger[*offset_len] << 8) + finger[*offset_len+1];
				temp_len = prh_sdp_read16(finger + *offset_len);
				*offset_len += temp_len -1;
				len -= temp_len -1;
				break;
			case OBEX_END_BODY :
				headers->eob = *offset_len;
//				temp_len = (finger[*offset_len] << 8) + finger[*offset_len+1];
				temp_len = prh_sdp_read16(finger + *offset_len);
				*offset_len += temp_len -1;
				len -= temp_len -1;
				break;
			case OBEX_WHO :
				headers->who = *offset_len;
//				temp_len = (finger[*offset_len] << 8) + finger[*offset_len+1];
				temp_len = prh_sdp_read16(finger + *offset_len);
				*offset_len += temp_len -1;
				len -= temp_len -1;
				break;
			case OBEX_CID :
				headers->cid = *offset_len;
				*offset_len += 4;
				len -=4;
				break;
			case OBEX_APP_PARAM:
				headers->app = *offset_len;
//				temp_len = (finger[*offset_len] << 8) + finger[*offset_len+1];
				temp_len = prh_sdp_read16(finger + *offset_len);
				*offset_len += temp_len -1;
				len -= temp_len -1;
				break;			
			case OBEX_AUTH_CHALLANGE :
				headers->au_cal = *offset_len;
//				temp_len = (finger[*offset_len] << 8) + finger[*offset_len+1];
				temp_len = prh_sdp_read16(finger + *offset_len);
				*offset_len += temp_len -1;
				len -= temp_len -1;
				break;			
			case OBEX_AUTH_RESPONSE :
				headers->au_rsp = *offset_len;
//				temp_len = (finger[*offset_len] << 8) + finger[*offset_len+1];
				temp_len = prh_sdp_read16(finger + *offset_len);
				*offset_len += temp_len -1;
				len -= temp_len -1;
				break;
			case OBEX_CLASS :
				headers->cla = *offset_len;
//				temp_len = (finger[*offset_len] << 8) + finger[*offset_len+1];
				temp_len = prh_sdp_read16(finger + *offset_len);
				*offset_len += temp_len -1;
				len -= temp_len -1;
				break;
			}
		} while(len);
	}
	len = 1;
	return BT_NOERROR;
}


