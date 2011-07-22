/***********************************************************************
 *
 * MODULE NAME:    sdp_le_request_encoder.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Service Discovery client request encoder.
 * MAINTAINER:     Mark Donnelly
 *
 * SOURCE CONTROL: $Id: sdp_le_request_encoder.c 1532 2010-08-23 05:47:34Z huazeng $
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

#if SDP_CLIENT

#include "sdp_le.h"
#include "sdp_le_request_encoder.h"
#include "sdp_constants.h"


#if SDP_SS_AND_SA_SUPPORT

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_generate_search_response   
 * DESCRIPTION:
 
 This function will create a service search pdu. It supports continue.

 header format:
 
 <P><T><T><L><L><search_pattern><M><M><C>.

 P : id
 T : transaction id
 L : length
 M : max record count 
 C : continue 

 * KNOWN ISSUES:

  none.
 
 *
 *
 ***********************************************************************/
void prh_sdp_generate_search_request(u_int8* pBuffer,struct prh_sdp_primitive_task* pTask,u_int8* pContinue)
{       
	u_int8* pAccess;
	u_int32 lcv;
	pBuffer[0] = PDU_ID_SDP_SERVICE_SEARCH_REQUEST;
	
	pTask->sub_task_level_1.expects	= PDU_ID_SDP_SERVICE_SEARCH_RESPONSE;

	prh_sdp_write16(pTask->sub_task_level_1.transaction_id,pBuffer + 1);
	
	pAccess = pBuffer + 5; /* bypass P + T + L */
	
	lcv = prh_sdp_encode_search_pattern(pAccess,&(pTask->pattern));
	
	if(pContinue != 0x00)
    { /* length = pattern size + continue len + max bytes len + continue header */
		prh_sdp_write16((u_int16)(lcv + *pContinue + 2 + 1),pBuffer + 3);
    } else {  /* length = pattern size + max bytes field + continue header */
		
		prh_sdp_write16((u_int16)(lcv +  2 + 1),pBuffer + 3);
    } 
	
	pAccess += lcv; /* bypass the patterns */
	
	prh_sdp_write16(pTask->rule.maxItems,pAccess);
	pAccess += 2;
	
	if(pContinue != 0x00)
    {
		for(lcv = 0; lcv < (u_int32)((*pContinue) + 1) ; lcv++)
		{
			pAccess[lcv] = pContinue[lcv];                  
		}      
    } else {
		*pAccess = 0x00;
    }  
}

#endif /* SDP_SS_AND_SA_SUPPORT */


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_encode_search_pattern
 * DESCRIPTION:
 
 This function will encode a serach pattern into a buffer.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
int prh_sdp_encode_search_pattern(u_int8* pBuffer,t_SDP_SearchPattern* pPattern)
{
	u_int32 lcv;
	
	*pBuffer++ = SDP_SEQUENCE_LARGE;
	prh_sdp_write32((pPattern->numItems) * 5,pBuffer);
	
	pBuffer += 4;

	for(lcv = 0; lcv < pPattern->numItems ; lcv++)
    {
		*pBuffer++ = SDP_UUID_4;
		prh_sdp_write32(pPattern->patternUUIDs[lcv],pBuffer);
		pBuffer +=4;
    }
	
	return (pPattern->numItems * 5) + 5;
}


#if SDP_SS_AND_SA_SUPPORT

/***********************************************************************
 *
 * FUNCTION NAME:   sdp_generate_attribute_request
 * DESCRIPTION:
 
 This function will generate an attribute request pdu. It supports 
continue.

 header format:

 <P><T><T><L><L><R><R><R><R><M><M><attribute list><C>
 
 P : id
 T : transaction id
 L : length
 R : record handle 
 M : max bytes count 
 C : continue 
 

 * KNOWN ISSUES:
 
 *
 *
 ***********************************************************************/
void prh_sdp_generate_attribute_request(u_int8* pBuffer, struct prh_sdp_primitive_task* pTask,u_int8* pContinue)
{
	u_int32 lcv;
	u_int8* pAccess;
    
	*pBuffer++ = PDU_ID_SDP_SERIVCE_ATTRIBUTE_REQUEST;
    
	pTask->sub_task_level_2.expects = PDU_ID_SDP_SERVICE_ATTRIBUTE_RESPONSE;

	prh_sdp_write16(pTask->sub_task_level_2.transaction_id,pBuffer);
	pBuffer += 2;
	
	lcv = prh_sdp_encode_attribute_list(pBuffer + 2 + 4 + 2 ,&(pTask->attributes)); /* bypass L + R + M */
    
	pAccess = pBuffer + 2;
    
	if(pContinue != 0x00)
    {  /* length = attrib size + continue len + record handle header + max bytes len + continue header */
		prh_sdp_write16((u_int16)(lcv + *pContinue + 4 + 2 + 1),pBuffer);
		pBuffer += 2;
		
    } else {  /* length = attrib size + record handle header + max bytes len + continue header */
		prh_sdp_write16((u_int16)(lcv +  4 + 2 + 1),pBuffer);
		/* no need to increment as I dont access with this again */
    } 
	
	pMemcpy(pAccess,&pTask->service_records[pTask->record_handles_left -1],4);
	
	pAccess +=4;  /* skip over record handle */
	
	prh_sdp_write16((u_int16)pTask->length,pAccess);
	pAccess += 2;
	
	pAccess += lcv; /* bypass attribs */
    
	if(pContinue != 0x00)
    {
		for(lcv = 0; lcv < (u_int32)((*pContinue) + 1) ; lcv++)
		{
			pAccess[lcv] = pContinue[lcv];                  
		}      
    } else {
		*pAccess = 0x00;
    }  
    
}

#endif /* SDP_SS_AND_SA_SUPPORT */


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_encode_attribute_list
 * DESCRIPTION:
 
 This function will convert a list of attributes and encode them for the
 wire. It will handle a range of attributes by setting the start id to be
 of the format 0xFxxx in the SDPAttributeIDs structure.

 * KNOWN ISSUES:
 
 None.

 *
 *
 ***********************************************************************/
int prh_sdp_encode_attribute_list(u_int8* pBuffer,t_SDP_AttributeIDs* pAttribs)
{
	u_int32 lcv;
	u_int8* pSize;
	u_int32 length;
	u_int32 limit;
/*	u_int32 counter;*/
	*pBuffer++ = SDP_SEQUENCE_LARGE;
    
	pSize = pBuffer;
    
	pBuffer +=4;
    
	for (lcv = 0,length = 0,limit = 1<<8; lcv < pAttribs->numItems ; lcv++)
    {
		if( pAttribs->attributeIDs[lcv] >> 16 ) /* attribute range */
		{
			*pBuffer++ = SDP_UINT_4;
			prh_sdp_write32(pAttribs->attributeIDs[lcv],pBuffer);
			pBuffer +=4;
			
			length += 5; /* u_int32 */
		} else { /* normal aid */
			*pBuffer++ = SDP_UINT_2;
			prh_sdp_write16((u_int16)pAttribs->attributeIDs[lcv],pBuffer);
			pBuffer += 2;
			
			length += 3; /* u_int16 */
		}
		/*
		if(length > limit)
		{
			counter = length;
			if(limit == 1<<8)
			{
				limit = 1 << 16;
				do {
					pSize[counter+2] = pSize[counter+1];
				} while(counter--);
				pBuffer++;
			} else {
				limit = 0xffffffff;
				do {
					pSize[counter+4] = pSize[counter+2];
				} while(counter--);
				pBuffer +=2;
			}
		}
		*/
    }    
    

	prh_sdp_write32(length,pSize);

	length += SDP_LARGE_HEADER;
	/*
	if(length <1<<8)
    {
		*pSize = length;
        
		length +=SDP_SMALL_HEADER;
    } else if (length <1<<16) {
		pSize[-1] = SDP_TEXT_MEDIUM;
		
		prh_sdp_write16((u_int16)length,pSize);
		
		length +=SDP_MEDIUM_HEADER;
    } else if (length <=0xffffffff) {
		pSize[-1] = SDP_TEXT_LARGE;
		
		prh_sdp_write32(length,pSize);
		
		length +=SDP_LARGE_HEADER;
    }
	*/

    
	return length;
}

#if SDP_SSA_SUPPORT

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_genetate_service_attribute_request
 * DESCRIPTION:
 
 This function will generate a service attribute request pdu. It supports
 continue also.

 header format:

 <P><T><T><L><L><search pattern><M><M><attribute list><C>
 
 P : id
 T : transaction id
 L : length
 M : max bytes count 
 C : continue 

 * KNOWN ISSUES:

none.
 
 *
 *
 ***********************************************************************/
void prh_sdp_genetate_service_attribute_request(u_int8* pBuffer, struct prh_sdp_primitive_task* pTask,u_int8* pContinue)
{
	u_int8* pAccess;
	u_int32 len;
	u_int32 lcv;
    
	*pBuffer++ = PDU_ID_SDP_SERVICE_SEARCH_ATTRIBUTE_REQUEST;
 
	pTask->sub_task_level_1.expects = PDU_ID_SDP_SERVICE_SEARCH_ATTRIBUTE_RESPONSE;

	*pBuffer++ = pTask->sub_task_level_1.transaction_id >> 8;
	*pBuffer++ = pTask->sub_task_level_1.transaction_id & 0xff;
    
	pAccess = pBuffer + 2;
    
	lcv = prh_sdp_encode_search_pattern(pAccess,&(pTask->pattern));
    
	pAccess += lcv;
    
	*pAccess++ = (pTask->length) >> 8;  
	*pAccess++ = (pTask->length) & 0xff;
    
	len = prh_sdp_encode_attribute_list(pAccess,&(pTask->attributes));
    
	if(pContinue != 0x00)
    { /* length = attribute + continue lenth + search pattern + max bytes header + continue header */
		*pBuffer++ = (len + *pContinue + lcv + 2 + 1) >> 8;
		*pBuffer++ = (len + *pContinue + lcv + 2 + 1) & 0xff;
    } else {   /* length = attribute + search pattern + max bytes header + continue header */
		*pBuffer++ = (len +  lcv + 2 + 1) >> 8;
		*pBuffer++ = (len +  lcv + 2 + 1) & 0xff;
    } 
    
	pAccess += len;
    
	if(pContinue != 0x00)
    {
		for(lcv = 0; lcv < (u_int32)((*pContinue) + 1) ; lcv++)
		{
			pAccess[lcv] = pContinue[lcv];                  
		}      
    } else {
		*pAccess = 0x00;
    }  
}

#endif /* SDP_SSA_SUPPORT */

#endif /*  SDP_CLIENT */
