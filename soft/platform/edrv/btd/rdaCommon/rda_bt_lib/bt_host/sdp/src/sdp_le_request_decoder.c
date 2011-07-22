/***********************************************************************
 *
 * MODULE NAME:    sdp_le_request_decoder.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Service Discovery Test code
 * MAINTAINER:     Mark Donnelly
 *
 * SOURCE CONTROL: $Id: sdp_le_request_decoder.c 1532 2010-08-23 05:47:34Z huazeng $
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

#include "sdp_stack_access.h"

#include "sdp_error.h"
#include "sdp_le.h"
#include "sdp_le_request_decoder.h"
#include "sdp_le_response_encoder.h"
#include "sdp_constants.h"

extern t_SDP_serviceRecord head;

static const u_int8 SDP_BASE_UUID[12] = {
  0x00,0x00,0x10,0x00,0x80,0x00,0x00,0x80,0x5F,0x9B,0x34,0xFB
};

extern t_pMutex prh_sdp_server_access;
extern u_int8 prh_sdp_server_active;

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_request_dispatcher
 * DESCRIPTION:
 
 This function will send the pdu to the correct decoder dependant on the
 message type.

 * KNOWN ISSUES:
 
 An invalid command is just ignored.

 *
 *
 ***********************************************************************/
void prh_sdp_request_dispatcher(u_int8* pdu,t_channelId cid)
{
	pMutexLock(prh_sdp_server_access); /* <<<< C-Section */
	if(prh_sdp_server_active)
		switch(*pdu++)
    {
    case PDU_ID_SDP_SERVICE_SEARCH_REQUEST:
		prh_sdp_service_search_decode(pdu,cid);
		break;
    case PDU_ID_SDP_SERIVCE_ATTRIBUTE_REQUEST:
		prh_sdp_attribute_search_decode(pdu,cid); 
		break;
    case PDU_ID_SDP_SERVICE_SEARCH_ATTRIBUTE_REQUEST:
		prh_sdp_service_attribute_search_decode(pdu,cid);
		break;
    default:
		break;
    }
	
	pMutexUnlock(prh_sdp_server_access); /* >>>> C-Section */
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_service_search_decode
 * DESCRIPTION:
 
 This function will decode a service search pdu an query the database and
 end the results.

 * KNOWN ISSUES:
 
 None.
 
 *
 *
 ***********************************************************************/
void prh_sdp_service_search_decode(u_int8* pdu,t_channelId cid)
{
	u_int16 transaction_id;
	u_int32 length;
	
	u_int8 pContinue[3];
   #if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
  u_int8* temp;
#endif
	
	u_int32* pRecordHandles = 0x00;
	u_int32 uuids[12]; /* 12 max number see spec p.348 */
	u_int32 number_uuids;
	u_int16 number_handles;
	u_int32 max_count;
	
	u_int32 offset = 0;   
	struct host_buf* h_buff;
	u_int16 mtu;
	
	pContinue[0] = 0x00;
	
	length = prh_l2_utils_out_mtu(cid,&mtu);
	
	if(length!= BT_NOERROR)
		return;
	
	transaction_id = prh_sdp_read16(pdu);
	pdu += 2;
	
	length = prh_sdp_read16(pdu);
	pdu += 2;
#if pDEBUG	
	pDebugPrintfEX((pLOGDEBUG,pLOGSDP,"<<    SDP_ServiceSearchRequest(%d ,%d)",transaction_id,length));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
       temp=pdu-4;
       SYSDebug_PrintEx(pLOGDEBUG,pLOGSDP,LOG_SDP_SDP_LE_REQUEST_DECODER,temp,4,(BT_CODE_WORD|(BT_CODE_WORD<<4)));
#endif
	
	h_buff = prh_sdp_allocate_tx_buffer(mtu, cid);
	
	pDebugCheck();
	
	if(h_buff)
    {		
		if(0x00 != uuids)
		{   
			if(prh_sdp_uuid_check(&offset,pdu,uuids,&number_uuids))
			{
			/*
			extract the continue information.
				*/
				pdu += offset;            				
				max_count = prh_sdp_read16(pdu);
				pdu +=2;
				
				offset = 0;
				if(*pdu)
				{
					pdu++;
					
					offset = prh_sdp_read16(pdu);
					pdu +=2;
				}
				
				prh_sdp_service_search(uuids,number_uuids,&number_handles,&pRecordHandles);
				
				if(number_handles > max_count)
					number_handles = max_count;
				
				if(offset < number_handles || ((offset == number_handles) && !number_handles) )
				{
					
					mtu = mtu - (5 /*header*/+ 5/* continue */ + L2CAP_HEADER_OVERHEAD );
					
					if( ((number_handles - offset) << 2 ) <= mtu)
					{ /* no need to generate a continue */
						mtu = number_handles - offset;
						/* mtu here refers to the current count of records */
					} else {
					/*
					generate continue with the offset set to offset+mtu
						*/
						mtu >>= 2;
						pContinue[0] = 2;
						prh_sdp_write16((u_int16)(offset + mtu),pContinue +1);
					}
					
					
					prh_sdp_generate_search_response(h_buff->data,pRecordHandles + offset, transaction_id,number_handles,mtu,pContinue);
					
				} else {
					prh_sdp_generate_error_response(h_buff->data,SDP_InvalidContinuation,0,0x00,transaction_id);				
				}
				
			} else { /* incorrect syntax */
				prh_sdp_generate_error_response(h_buff->data,SDP_InvalidSyntax,0,0x00,transaction_id);
			}       
		} else { /* resources */
			prh_sdp_generate_error_response(h_buff->data,SDP_InsufficientResources,0,0x00,transaction_id);
		}
    } else {
		h_buff = prh_sdp_allocate_tx_buffer(7, cid);
		
		if(h_buff)
		{
			prh_sdp_generate_error_response(h_buff->data,SDP_InsufficientResources,0,0x00,transaction_id);
			
		} else { /* nothing I can do */
			return ;
		}
    }
	
	prh_sdp_transfer_data(cid,h_buff);
	
#if COMBINED_HOST==0
	if(pRecordHandles) pFree(pRecordHandles);	
#endif
}



/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_attribute_search_decode
 * DESCRIPTION:
 
 This function will decode the attribute search pdu and query the database
 and then send the results back to the caller.

 * KNOWN ISSUES:
 
 None.

 *
 *
 ***********************************************************************/
 void prh_sdp_attribute_search_decode(u_int8* pdu,t_channelId cid)
 {
	 u_int8 len;
	 
	 u_int16 transaction_id;
	 u_int16 length;
	 u_int32 service_record_handle;
	 u_int16 max_bytes;
	 u_int8 p_temp_buffer[128];//[48];
	 u_int8 pContinue[5];
	 u_int16 mtu;
	 
	 u_int32 data_len;
	 u_int32 offset;

        #if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
     u_int8* temp;
     #endif
	 u_int32 previous = 0;
	 u_int32 current = 0;
	 
	 struct host_buf* h_buff;
	 
	 sdp_attribute_id_list* pList;
	 t_SDP_serviceRecord* pRecord;
	
	 pList = 0;

	 
	 if(prh_l2_utils_out_mtu(cid,&mtu)!= BT_NOERROR)
		 return;
	 
	 transaction_id = prh_sdp_read16(pdu);
	 pdu += 2;
	 
	 length = prh_sdp_read16(pdu);
	 pdu += 2;
	 
	 service_record_handle = prh_sdp_read32(pdu);
	 pdu += 4;
	 
	 max_bytes = prh_sdp_read16(pdu);
	 pdu += 2;
	 
	 pContinue[0] = 0;
	 offset = 0;
#if pDEBUG	 
	 pDebugPrintfEX((pLOGDEBUG,pLOGSDP,"<<    SDP_ServiceAttributeRequest(%d ,%d, %d, %d)",transaction_id,length,service_record_handle,max_bytes));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
        temp=pdu-10;
            
        SYSDebug_PrintEx(pLOGDEBUG,pLOGSDP,(LOG_SDP_SDP_LE_REQUEST_DECODER+1),temp,10,(BT_CODE_WORD |(BT_CODE_WORD<<4)|(BT_CODE_DWORD<<8) |(BT_CODE_WORD<<12)));
#endif
	 
	 mtu = mtu -( 5 + 2 + 5 + L2CAP_HEADER_OVERHEAD);
	 
	 if(max_bytes < mtu)
	 {
		 mtu = max_bytes;
	 }
	 
	 h_buff = prh_sdp_allocate_tx_buffer(mtu + 5 + 2 + 5, cid);  /* will need to check this */
	 
	 pDebugCheck();
	 
	 if(h_buff)
	 {
		 if(max_bytes > 6) /* allowed range is from 7 to 0xffff */
		 {
			 if(prh_sdp_valid_record(service_record_handle,&pRecord))
			 {    
				 
				 
				 if(prh_sdp_attribute_check(&offset,pdu,&len,&pList))
				 {
					 
					 
					 pdu += offset;
					 offset = 0;
					 if(*pdu)
					 {
						 pdu++;
						 previous = prh_sdp_read32(pdu);
					 }					
					 //p_temp_buffer = pMalloc(mtu);
					 
					 if(p_temp_buffer)
					 {
						 
						 data_len = prh_sdp_attribute_retrieve(pRecord,len,pList,mtu,previous,&current,p_temp_buffer);
						 
						 pDebugCheck();
						 
						 
						 
						 if(current != data_len)
						 {
							 pContinue[0] = 4;
							 prh_sdp_write32(current,pContinue +1);
						 }
						 						 
						 pDebugCheck();
						 
						 if(previous < data_len)
						 {
							 prh_sdp_generate_attrib_response(h_buff->data,p_temp_buffer,transaction_id,(u_int16)(current - previous),pContinue);
							 
						 } else {
							 prh_sdp_generate_error_response(h_buff->data,SDP_InvalidContinuation,0,0x00,transaction_id);								
						 }
						 
						 pDebugCheck();
						 
					 } else {
						 prh_sdp_generate_error_response(h_buff->data,SDP_InsufficientResources,0,0x00,transaction_id);
					 }
				 } else {
					 prh_sdp_generate_error_response(h_buff->data,SDP_InvalidSyntax,0,0x00,transaction_id);
				 }
			 } else {
				 prh_sdp_generate_error_response(h_buff->data,SDP_InvalidHandle,0,0x00,transaction_id);
			 }
		 } else {
			 prh_sdp_generate_error_response(h_buff->data,SDP_InvalidSyntax,0,0x00,transaction_id);      
		 }
	 } else {
		 
		 h_buff = prh_sdp_allocate_tx_buffer(7, cid); 
		 
		 if(h_buff)
		 {
			 prh_sdp_generate_error_response(h_buff->data,SDP_InsufficientResources,0,0x00,transaction_id);
			 
		 } else { /* nothing I can do */
			 return ;
		 }
	 }
	 
	 
	 
	 prh_sdp_transfer_data(cid,h_buff);
	 
#if COMBINED_HOST==0
	 if(pList) pFree(pList);
#endif
}



/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_service_attribute_search_decode
 * DESCRIPTION:
 
 This function will decode a service attribute search pdu. And perform
 the database searches and send back the results. It is able to support
 the continueation and all list sizes.

 * KNOWN ISSUES:
 
 it will need to check that the max length is not exceeded....
 
 *
 *
 ***********************************************************************/
 void prh_sdp_service_attribute_search_decode(u_int8* pdu,t_channelId cid)
 {

 #if pDEBUG==0

 u_int8* temp;
 #endif
	 u_int16 transaction_id;
	 u_int16 length;
	 u_int8 len;
	 u_int16 max_bytes;
	 sdp_attribute_id_list* pList;
	 u_int32 uuids[12];
	 u_int32 number_uuids;
	 u_int32 lcv;
	 u_int32 data_len;
	 register u_int32 total_len;
	 u_int16 number_handles;
	 u_int32 temp_len;
	 u_int8 continue_set_on_entry = 0;
	 u_int8 pContinue[5];
	 
	 u_int8* p_temp_buffer = 0;
	 u_int32* pRecordHandles;
	 t_SDP_serviceRecord* pRecord;
	 u_int8* p_main_attrib_list_len=0;
	 u_int16 mtu;
	 
	 u_int32 current = 0;
	 u_int32 previous = 0;
	 
	 struct host_buf* h_buff;
	 
	 pContinue[0] = 0;
	 pList = 0;
	 pRecordHandles = 0;
	 
	 if(prh_l2_utils_out_mtu(cid,&mtu) != BT_NOERROR)
		 return;
	 
	 
	 if(mtu > 5 + 5 + 2 + L2CAP_HEADER_OVERHEAD)
		 mtu -= 5 + 5 + 2 + L2CAP_HEADER_OVERHEAD;
	 
	 transaction_id = prh_sdp_read16(pdu);
	 
	 pdu += 2;
	 
	 length = prh_sdp_read16(pdu);
	 pdu += 2;
	 
#if pDEBUG	 
	 pDebugPrintfEX((pLOGDEBUG,pLOGSDP,"<<    SDP_ServiceSearchAttributeRequest(%d ,%d)",transaction_id,length));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
        temp=pdu-4;
        SYSDebug_PrintEx(pLOGDEBUG,pLOGSDP,(LOG_SDP_SDP_LE_REQUEST_DECODER+2),temp,4,(BT_CODE_WORD|(BT_CODE_WORD<<4)));
#endif
	 h_buff = prh_sdp_allocate_tx_buffer(mtu + 5 + 5 + 2, cid);
	 
	 if(h_buff)
	 { /* h_buff ok */
		 
		 if(prh_sdp_uuid_check(&lcv,pdu,uuids,&number_uuids))
		 { /* uuid check passed */       
			 prh_sdp_service_search(uuids,number_uuids,&number_handles,&pRecordHandles);
			 
			 pdu += lcv;
			 
			 max_bytes = prh_sdp_read16(pdu);
			 pdu += 2;
			 
			 if(max_bytes < mtu)
			 {
				 mtu = max_bytes;
			 }
			 
			 
			 if(max_bytes > 8) /* max allowed range is from 9 to 0x0ffff */
			 {
				 
				 if(prh_sdp_attribute_check(&temp_len,pdu,&len,&pList))           
				 { /* attribute check passed */          
					 pdu += temp_len;
					 
					 
#if COMBINED_HOST==0
					 p_temp_buffer = (u_int8*)pMalloc(mtu); /* I will need to re-evaluate this for zero copy */
#endif					 
					 /* note I will need to put in an expanding buffer here */
					 /* ****************************************  */
					 
					 if(p_temp_buffer)
					 { /* data buffer allocated */
						 
						 
						 if(*pdu)
						 {
							 pdu +=3;
							 
							 previous = prh_sdp_read16(pdu);
							 pdu += 2;
							 
							 continue_set_on_entry = 0x01;
							 
						 }
						 
						 if(previous <= current)
						 {
							 *p_temp_buffer = SDP_SEQUENCE_LARGE; /* list */
							 p_main_attrib_list_len = p_temp_buffer+1;
							 
						 }
						 
						 current += 5;
						 
						 
						 for(total_len = 0,lcv=0; lcv < number_handles;lcv++)
						 {
							 
							 if(prh_sdp_valid_record(pRecordHandles[lcv],&pRecord))
							 {
								 /* total_len may be greater than u_int16 earlier in the function, but should be less than u_int16 here - MD */
								 data_len = prh_sdp_attribute_retrieve(pRecord,len,pList,mtu,previous,&current,p_temp_buffer);
								 
								 
								 if(data_len==5)
								 {
									 data_len = 0;
									 current -= 5;
								 } 
								 
							 } else continue;
							 
							 total_len += data_len;
							 
							 
						 } /* end of for */
						 
						 
						 if(!continue_set_on_entry)
						 {
							 prh_sdp_write32(total_len,p_main_attrib_list_len);
						 }													
						 
						 if(current -5 != total_len && number_handles)
						 {				
							 pContinue[0] = 4;
							 prh_sdp_write32(current,pContinue +1);
						 }
						 
						 if(previous < total_len  || ((previous == total_len)&&!total_len))
						 {
							 
							 prh_sdp_generate_service_attrib_response(h_buff->data,
								 p_temp_buffer,
								 transaction_id,
								 (u_int16)(current - previous),
								 pContinue);
							 
							 
						 } else {
							 prh_sdp_generate_error_response(h_buff->data,SDP_InvalidContinuation,0,0x00,transaction_id);
						 }
						 
						 
					 } else {
						 prh_sdp_generate_error_response(h_buff->data,SDP_InsufficientResources,0,0x00,transaction_id);
					 }						   
				 } else {
					 prh_sdp_generate_error_response(h_buff->data,SDP_InvalidSyntax,0,0x00,transaction_id);
				 }
			 } else {
				 prh_sdp_generate_error_response(h_buff->data,SDP_InvalidSyntax,0,0x00,transaction_id);        
			 }		
			} else {
				prh_sdp_generate_error_response(h_buff->data,SDP_InvalidSyntax,0,0x00,transaction_id);
			}
    } else {
		h_buff = prh_sdp_allocate_tx_buffer(7, cid); 
		
		if(h_buff)
		{
			
			prh_sdp_generate_error_response(h_buff->data,SDP_InsufficientResources,0,0x00,transaction_id);
			
		} else { /* nothing I can do */
			return ;
		}
    }
	
	
	prh_sdp_transfer_data(cid,h_buff);
    	
#if COMBINED_HOST==0
	if(p_temp_buffer) pFree(p_temp_buffer);		
	if(pRecordHandles) pFree(pRecordHandles);	
	if(pList) pFree(pList);
#endif
}

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_attribute_check
 * DESCRIPTION:

 This function will extract the attributes from the pdu and place them in 
an attribute list.

 * KNOWN ISSUES:

 none.

 *
 *
 ***********************************************************************/
 u_int8 prh_sdp_attribute_check(u_int32* offset,u_int8* attributes, u_int8* len,sdp_attribute_id_list** p_list)
 {
	 u_int8 ret_value;
	 u_int32 numberofitems=0;
	 u_int32 counter;
	 u_int32 t_lcv=0;
	 u_int8 header;
	 u_int8* data; 
	 
	 
#if COMBINED_HOST==0
	 sdp_attribute_id_list* list=NULL;
#else	 
	 static sdp_attribute_id_list list[8];
#endif
	 
	 ret_value = 1;
	 *offset = 0;  
	 header = *attributes;
	 data = attributes+1; /* move past the header */
	 
	 if( PRH_SDP_READ_TYPE( header) == SDP_SEQUENCE)
	 {
	 /*
	 now to check the size and see home much to step in the data.
		 */
		 switch( PRH_SDP_READ_SIZE( header) )
		 {
		 case 5:
			 numberofitems = *data++;
			 
			 *offset = numberofitems + SDP_SMALL_HEADER;
			 break;
		 case 6:
			 numberofitems = prh_sdp_read16(data);
			 data += 2;
			 
			 *offset = numberofitems + SDP_MEDIUM_HEADER;
			 break;
		 case 7:
			 numberofitems = prh_sdp_read32(data);
			 data +=4;
			 
			 *offset = numberofitems + SDP_LARGE_HEADER;
			 break;
		 default:
		 /*
		 invalid size
			 */            
			 ret_value = 0;
			 break;            
		 }
		 
		 /*
		 now to get the entries from the seqence
		 */
		 
#if COMBINED_HOST==0
		 list = pMalloc(sizeof(sdp_attribute_id_list) * (numberofitems>>1));
#endif
		 if(!list)
			 return 0;
		 
		 for(counter = 0, t_lcv = 0; counter<numberofitems; counter += (1 << PRH_SDP_READ_SIZE( header) )  + 1, t_lcv++)
		 {
			 header = *data;
			 
			 data++; /* move past the header */
			 
			 if( PRH_SDP_READ_TYPE( header) == SDP_UINT)
			 {
			 /*
			 next check the size
				 */
				 switch( PRH_SDP_READ_SIZE( header) )
				 {
				 case 1:
					 list[t_lcv].start_id = prh_sdp_read16(data);
					 data += 2;
					 list[t_lcv].finish_id = list[t_lcv].start_id;
					 break;
				 case 2:
					 list[t_lcv].start_id = prh_sdp_read16(data);
					 data += 2;
					 list[t_lcv].finish_id = prh_sdp_read16(data);
					 data += 2;
					 break;
				 default:
					 /* incorrect size */
					 ret_value = 0;
					 
					 goto finish;
					 
				 }
			 } else {
				 /* it was not a uuid */
				 ret_value = 0;
				 break;
			 }           
		 }                  
	 } else {
		 ret_value = 0;
	 }
	 
finish:
	 if(ret_value == 0)
	 {
#if COMBINED_HOST==0
		 if(list)
			 pFree(list);
#endif
		 p_list = 0x00;
	 } else {
		 *p_list = list;
		 *len = t_lcv;
	 }
	 
	 return ret_value;
}



/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_uuid_check
 * DESCRIPTION:
 
 This function will take in the list of uuids from the pdu and convert 
 if to a list of u_int32 containing the uuids. It will support 16 and 32
 bit uuids both are stored in as a 32bit number. The 128 bit uuids are
 converted to 32bit ones, and the remainder of the 128 bit uuids is checked
 against the base uuid.
 
 * KNOWN ISSUES:

  none.

 *
 *
 ***********************************************************************/
 u_int8 prh_sdp_uuid_check(u_int32* offset,u_int8* uuids, u_int32* pList,u_int32* pNumber)
 {
	 u_int8 ret_value;
     
	 u_int32 lcv;
	 u_int32 numberofitems;
	 u_int32 counter;
	 u_int8* data; 
	 u_int8 header;
	 
     
	 /*
	 Check to see that each entry in the list is a valid uuid....z
	 */
	 *offset = 0;
	 ret_value = 1;
     
	 *pNumber = 0;
	 header = *uuids;
	 data = uuids+1; /* move past the header */
     
	 
	 if( PRH_SDP_READ_TYPE(header) == SDP_SEQUENCE)
	 {
	 /*
	 now to check the size and see home much to step in the data.
		 */
		 switch( PRH_SDP_READ_SIZE(header) )
		 {
		 case 5:
			 numberofitems = *data++;    
			 *offset = numberofitems + SDP_SMALL_HEADER;
			 break;
		 case 6:
			 numberofitems = prh_sdp_read16(data);
			 data += 2;
			 *offset = numberofitems + SDP_MEDIUM_HEADER;
			 break;
		 case 7:
			 numberofitems = prh_sdp_read32(data);
			 
			 data +=4;
			 *offset = numberofitems + SDP_LARGE_HEADER;
			 break;
		 default:
		 /*
		 invalid size
			 */
             
			 ret_value = 0;
			 goto finish;
             
		 }
         
         
		 if(numberofitems/17 > 12)
		 {
			 ret_value = 0;
			 goto finish;
		 }
		 
		 /*
		 data will now be pointing in the right place.
		 */
		 for(counter = 0; counter<numberofitems; counter += (1 << (PRH_SDP_READ_SIZE(header) + 1)))
		 {
			 header = *data;
             
			 data++; /* move past the header */
             
			 if( PRH_SDP_READ_TYPE(header) == SDP_UUID)
			 {
			 /*
			 next check the size
				 */
                 
				 (*pNumber)++;
                 
				 pList[*pNumber -1] = 0x00; /* initialise */
                 
				 switch( PRH_SDP_READ_SIZE(header) )
				 {
				 case 1:
					 pList[*pNumber-1] = prh_sdp_read16(data);
					 data += 2;
					 break;
				 case 2:
					 pList[*pNumber-1] = prh_sdp_read32(data);
					 data +=4;
					 break;
				 case 4:
					 
					 pList[*pNumber-1] = prh_sdp_read32(data);
					 data +=4;
					 /* now to check the rest of the uuid to see if it matches the base */
					 for(lcv = 0;lcv<12;lcv++)
					 {
						 if(SDP_BASE_UUID[lcv] != *data++)
						 {
							 ret_value = 0;
							 goto finish;
						 }
					 }                    
					 break;
				 default:
					 /* incorrect size */
					 ret_value = 0;
                     
					 goto finish;                   
				 }                
			 } else {
				 /* it was not a uuid */
				 ret_value = 0;
				 break;
			 }            
		 }
	 } else {
		 /* it was not a list */
		 ret_value = 0;
	 }
     
finish:
	 
	 return ret_value;
}



/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_attribute_retrieve 
 * DESCRIPTION:
 
 This function will take a list of attribute ids and retrieve them from
 the database and encode them in a supplied buffer.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
 u_int32 prh_sdp_attribute_retrieve(t_SDP_serviceRecord* pRecord,u_int8 len, sdp_attribute_id_list* pList,u_int16 buffsize,u_int32 previous,u_int32* current,u_int8* buffer)
 {
	 register u_int32 ret_value;
	 sdp_attribute_id_list attribs;
	 t_SDP_dataElement* pData;  
	 
	 u_int32 offset;
	 u_int32 counter;
	 u_int32 lcv;
	 u_int32 temp_count;
	 u_int32 starting;
	 u_int8 space_for_header;
	 
	 offset = 0;
	 ret_value = 0;
	 space_for_header = 0;
	 starting = *current;
	 
	 if(previous <= *current)
	 {
		 if(*current - previous + 5 > buffsize)
		 {
			 /* keep going */
		 } else {
			 buffer[*current - previous] = SDP_SEQUENCE_LARGE;
			 *current +=5;
			 space_for_header = 1;
		 }
	 } else {
		 *current +=5;
	 }
	 
	 counter = 0;
	 
	 for(; counter < len ; counter++)
	 {
		 attribs = pList[counter];
		 if(attribs.start_id == attribs.finish_id)
		 {     
			 prh_sdp_query(pRecord,attribs.start_id,&pData);
			 
			 if(pData)
			 {
			 /*
			 insert the id
				 */
				 
				 if(previous <= *current)
				 {
					 
					 if(*current - previous + 3 > buffsize)
					 {
						 /* just keep going */
					 } else {
						 buffer[*current - previous] = SDP_UINT_2;
						 (*current)++;
						 prh_sdp_write16(attribs.start_id,buffer + *current - previous);							 
						 *current += 2;
					 }
				 } else {
					 *current += 3;
				 }				 
				 
				 /*
				 insert the record value
				 */
				 offset = prh_sdp_data_element_converter(buffsize,buffer,previous,current,pData);
				 
				 if(!offset)
					 break;
				 
				 temp_count = ret_value + offset + 3;
				 
				 ret_value += offset + 3; /* and the id */									
			 }
		 } else {
			 /* attribute list */
			 
			 lcv = attribs.start_id;           
			 
			 while ( lcv <= attribs.finish_id)
			 {          
				 prh_sdp_query(pRecord,(u_int16)lcv,&pData);
				 if(pData)
				 {
					 /* write in the ID*/
					 if(previous <= *current)
					 {
						 if(*current - previous + 3 > buffsize)
						 {
							 /* just keep going */
						 } else {
							 buffer[*current - previous] = SDP_UINT_2;
							 (*current)++;							 
							 prh_sdp_write16((u_int16)lcv,buffer + *current - previous);							 
							 *current += 2;							 							 
						 }
					 } else {
						 *current += 3;
					 }
					 
					 offset = prh_sdp_data_element_converter((u_int16)(buffsize),buffer,previous,current,pData);
					 
					 if(!offset)
						 goto exit_point;
					 
					 temp_count = ret_value + offset + 3;
					 
					 ret_value += offset + 3;
				 }         
				 
				 if(lcv == 0xffff)
					 break;
				 
				 lcv++;
			 }     
		 }
	 }
	 
exit_point: 
	 
	 if(space_for_header)
	 {
		 prh_sdp_write32(ret_value,buffer + (starting - previous) +1);
	 }
	 ret_value += 5; /* needs to be after */
	 
	 
	 return ret_value;
}


	      
#endif /* SDP_SERVER */



