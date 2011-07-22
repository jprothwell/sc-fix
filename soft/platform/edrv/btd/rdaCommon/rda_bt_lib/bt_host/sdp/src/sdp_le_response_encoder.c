/***********************************************************************
 *
 * MODULE NAME:    sdp_le_response_encoder.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Service Discovery server response encoder.
 * MAINTAINER:     Mark Donnelly
 *
 * SOURCE CONTROL: $Id: sdp_le_response_encoder.c 1532 2010-08-23 05:47:34Z huazeng $
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

#if SDP_SERVER

#include "sdp_le_response_encoder.h"
#include "sdp_constants.h"
#include "sdp_le.h"

extern t_SDP_serviceRecord* prh_sdp_database;


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_generate_search_response   
 * DESCRIPTION:
 
 This function will take a list of uuids and place them in search response
 pdu. It supports continue.

 * KNOWN ISSUES:

  none.

 *
 *
 ***********************************************************************/
void prh_sdp_generate_search_response(u_int8* pBuffer, u_int32* pData, u_int16 transaction_id, u_int16 total, u_int16 current, u_int8* pContinue)
{
	u_int8* pAccess;
	register u_int32 lcv;
	
	pBuffer[0] = PDU_ID_SDP_SERVICE_SEARCH_RESPONSE;
	
	
	prh_sdp_write16(transaction_id,pBuffer+1);
	
	
	pAccess = pBuffer + 9;
    
	if(pContinue != 0x00)
    {
		prh_sdp_write16((u_int16)(current*4 + *pContinue + 2 + 2 + 1),pBuffer + 3);
    } else {
		prh_sdp_write16((u_int16)(current*4 + 2 + 2 + 1), pBuffer + 3);
    }
    
	prh_sdp_write16(total,pBuffer + 5);
	
	prh_sdp_write16(current,pBuffer + 7);
	
	for(lcv = 0; lcv < current; lcv++)
    {
		prh_sdp_write32(pData[lcv],pAccess);
		pAccess += 4;
    }
    
	if(pContinue != 0x00)
    {
		pMemcpy(pAccess,pContinue,(*pContinue) +1);
    } else {
		*pAccess = 0x00;
    }  
    
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_generate_attrib_response
 * DESCRIPTION:
 
 This function will take the attributes in pdu format and create an 
 attribute response pdu around it. It will support continue.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
void prh_sdp_generate_attrib_response(u_int8* pBuffer, u_int8* pData,u_int16 transaction_id,u_int16 byte_count,u_int8* pContinue)
{
	u_int8* pAccess;
	
	pBuffer[0] = PDU_ID_SDP_SERVICE_ATTRIBUTE_RESPONSE; /* attrib search response */
	
	prh_sdp_write16(transaction_id,pBuffer+1);  
	
	pAccess = pBuffer + 7;
	
	if(pContinue != 0x00)
    {
		prh_sdp_write16((u_int16)(byte_count + *pContinue + 2+ 1), pBuffer + 3);
    } else {
		prh_sdp_write16((u_int16)(byte_count + 2+ 1), pBuffer + 3);
    }
	
    prh_sdp_write16((u_int16)(byte_count), pBuffer + 5);
	
	
	pMemcpy(pAccess,pData,byte_count);
	
	pAccess += byte_count;
	
	if(pContinue != 0x00)
    {
		pMemcpy(pAccess,pContinue,(*pContinue) +1);
		
    } else {
		*pAccess = 0x00;  
    }
	
}

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_generate_service_attrib_response
 * DESCRIPTION: 

  This function will use the attribute response to generate the service
  attribute response pdu. It supports continue.
  
 * KNOWN ISSUES:
    
 none.
      
 *
 *
 ***********************************************************************/
void prh_sdp_generate_service_attrib_response(u_int8* pBuffer, u_int8* pData,u_int16 transaction_id,u_int16 byte_count,u_int8* pContinue)
{
    
	prh_sdp_generate_attrib_response(pBuffer,pData,transaction_id,byte_count,pContinue);
    
	pBuffer[0] = PDU_ID_SDP_SERVICE_SEARCH_ATTRIBUTE_RESPONSE;
	
}



/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_generate_error_response
 * DESCRIPTION:

  This function will give an error code generate a pdu that contains a 
  error response. Some optional error information can be added.
  
 * KNOWN ISSUES:
    
  none.
      
 *
 *
 ***********************************************************************/
void prh_sdp_generate_error_response(u_int8* pBuffer,u_int16 error_code,u_int16 length,u_int8* errorInfo, u_int16 transaction_id)
{
    u_int16 lcv;
	
    pBuffer[0] = 0x01; /* service error response */
	
    prh_sdp_write16(transaction_id,pBuffer+1); 
	
	prh_sdp_write16((u_int16)(2 + length), pBuffer + 3);
    
	prh_sdp_write16(error_code,pBuffer + 5);
	
    for(lcv = 0; lcv < length ; lcv++)
    {
		*pBuffer++ = *errorInfo++;
    }
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_data_element_converter
 * DESCRIPTION:
 
 This function will convert an element to its format in a pdu. This will
use recursion to handle list types. It will return the number of bytes 
that were read.

 * KNOWN ISSUES:

none.
 
 *
 *
 ***********************************************************************/
u_int32 prh_sdp_data_element_converter(u_int16 buff_size,u_int8* pData, u_int32 previous, u_int32 *current,t_SDP_dataElement* pElement)
{
	u_int32 ret_value;
	u_int32 counter;
	u_int32 lcv;
	u_int32 offset;
	u_int32 size;
	counter = 0;
	ret_value = 0;
	lcv = 0;
	size = 0;
    
	if(previous <= *current)
    {
		if(*current - previous + 1 > buff_size)
		{
			/* keep going */
		} else {
			pData[*current - previous] = pElement->size + (pElement->type << 3);
			(*current)++;
		}
    } else {
		(*current)++;
    }
	ret_value++;
	
    
	switch(pElement->size)
    {
    case 0:
    case 1:
    case 2:
		size = 1 << pElement->size;
		
		if(previous <= *current)
		{
			if(*current - previous + size > buff_size)
			{ 
				pMemcpy(pData + *current - previous,(u_int8*)&pElement->data.local,buff_size - (*current - previous));
				*current += buff_size - (*current - previous);
			} else {
				pMemcpy(pData + *current - previous,(u_int8*)&pElement->data.local,size);
				*current += size;
			}
		} else if (previous <= *current + size) {
			
			if(*current + size - previous > buff_size)
			{
				pMemcpy(pData,(u_int8*)&pElement->data.local + (previous - *current), buff_size);
				*current = previous + buff_size;
			} else {
				pMemcpy(pData,(u_int8*)&pElement->data.local + (previous - *current), size - ( previous - *current));
				*current += size;
			}
		} else {
			*current += size;
		}
		
		ret_value += size;
		
		break;
    case 3:
    case 4:
		
		size = 1 << pElement->size;
		
		if(previous <= *current)
		{
			if(*current - previous + size > buff_size)
			{ 
#if SDP_ROM_SUPPORT
				pMemcpy(pData + *current - previous,(u_int8*)prh_sdp_database + (u_int32)pElement->data.pointer,buff_size - (*current - previous));
#else
				pMemcpy(pData + *current - previous,pElement->data.pointer,buff_size - (*current - previous));
#endif /* SDP_ROM_SUPPORT */
				*current += buff_size - (*current - previous);
			} else {
#if SDP_ROM_SUPPORT
				pMemcpy(pData + *current - previous,(u_int8*)prh_sdp_database + (u_int32)pElement->data.pointer,size);
#else
				pMemcpy(pData + *current - previous,pElement->data.pointer,size);
#endif /* SDP_ROM_SUPPORT */
				*current += size;
			}
		} else if (previous <= *current + size) {
			
			if(*current + size - previous > buff_size)
			{
#if SDP_ROM_SUPPORT
				pMemcpy(pData,(u_int8*)prh_sdp_database + (u_int32)pElement->data.pointer + (previous - *current), buff_size);
#else
				pMemcpy(pData,pElement->data.pointer + (previous - *current), buff_size);
#endif /* SDP_ROM_SUPPORT */
				*current = previous + buff_size;
			} else {
#if SDP_ROM_SUPPORT
				pMemcpy(pData,(u_int8*)prh_sdp_database + (u_int32)pElement->data.pointer + (previous - *current), size - ( previous - *current));
#else
				pMemcpy(pData,pElement->data.pointer + (previous - *current), size - ( previous - *current));
#endif /* SDP_ROM_SUPPORT */
				*current += size;
			}
		} else {
			*current += size;
		}
		
		ret_value += size;
		
		
		break;
    case 5:
    case 6:
    case 7:
		size = pElement->data_length;
		
		switch(pElement->size)
		{
		case 5:
			
			if(previous <= *current)
			{
				if(*current - previous + 1 > buff_size)
				{
					/* keep going */
				} else {
					pData[*current - previous] = size;	
					(*current)++;
				}
			} else {
				(*current)++;
			}
			ret_value ++;
			break;
		case 6:
			if(previous <= *current)
			{
				if(*current - previous + 1 > buff_size)
				{ 
					/* keep going */
				} else {
					prh_sdp_write16((u_int16)size,pData + *current - previous);
					*current += 2;
				}				
			} else {
				*current += 2;
			}
			ret_value += 2;
			break;
		case 7:
			if(previous <= *current)
			{
				if(*current - previous + 1 > buff_size)
				{ 
					/* keep going */
				} else {
					prh_sdp_write32(size,pData + *current - previous);
					*current += 4;			
				}
			} else {
				*current += 4;
			}
			ret_value += 4;
		}
		
		switch(pElement->type)
		{
		case SDP_SEQUENCE:
		case SDP_UNION:
			for(lcv=0, counter = 0; lcv < size ; counter++)
			{	
#if SDP_ROM_SUPPORT
				offset = prh_sdp_data_element_converter(buff_size, pData,previous,current,((t_SDP_dataElement*) ((u_int8*)prh_sdp_database + (u_int32)pElement->data.pointer) ) + counter);
#else
				offset = prh_sdp_data_element_converter(buff_size, pData,previous,current,((t_SDP_dataElement*)pElement->data.pointer) + counter);
#endif /* SDP_ROM_SUPPORT */
				lcv += offset;
			}
			ret_value += lcv;	  
			break;
		case SDP_TEXT: 
		case SDP_URL:			
			if(previous <= *current)
			{
				if(*current - previous + size > buff_size)
				{ 
#if SDP_ROM_SUPPORT
					pMemcpy(pData + *current - previous,(u_int8*)prh_sdp_database + (u_int32)pElement->data.pointer,buff_size - (*current - previous));
#else
					pMemcpy(pData + *current - previous,pElement->data.pointer,buff_size - (*current - previous));
#endif /* SDP_ROM_SUPPORT */
					*current += buff_size - (*current - previous);
				} else {
#if SDP_ROM_SUPPORT
					pMemcpy(pData + *current - previous,(u_int8*)prh_sdp_database + (u_int32)pElement->data.pointer,size);
#else 
					pMemcpy(pData + *current - previous,pElement->data.pointer,size);
#endif /* SDP_ROM_SUPPORT */
					*current += size;
				}
				
			} else if(previous <= *current + size) {
				
				if(*current + size - previous > buff_size)
				{
#if SDP_ROM_SUPPORT
					pMemcpy(pData,(u_int8*)prh_sdp_database + (u_int32)pElement->data.pointer + (previous - *current), buff_size);
#else
					pMemcpy(pData,pElement->data.pointer + (previous - *current), buff_size);
#endif /* SDP_ROM_SUPPORT */
					*current = previous + buff_size;
				} else {
#if SDP_ROM_SUPPORT
					pMemcpy(pData,(u_int8*)prh_sdp_database + (u_int32)pElement->data.pointer + (previous - *current), size - ( previous - *current));
#else
					pMemcpy(pData,pElement->data.pointer + (previous - *current), size - ( previous - *current));
#endif /* SDP_ROM_SUPPORT */
					*current += size;
				}
			} else {
				*current += size;
			}
			
			ret_value += size;
			break; 
		}      
		break;
    }
	return ret_value;
}


#endif /* SDP_SERVER */
