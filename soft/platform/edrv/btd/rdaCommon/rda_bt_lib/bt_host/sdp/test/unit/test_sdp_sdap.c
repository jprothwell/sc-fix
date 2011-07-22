/*
  $Id: test_sdp_sdap.c 1532 2010-08-23 05:47:34Z huazeng $
 */

#include "host_config.h"
#include "host_types.h"
#include "hu_harness.h"
#include "papi.h"

#include "mgr_core.h"


#include "l2_types.h"
#include "unit_stub_l2cap_internal.h"

#include "../../include/sdp_ue.h"
#include "../../include/sdp_constants.h"
#include "../../include/sdp_server.h"
#include "../../include/sdp_db.h"
#include "../../include/sdp_error.h"

extern struct prh_sdp_primitive_task* p_SDP_Primitive_Task_List;

u_int32 test_state = 0;
u_int32 test_mode = 0; /* 0 = 128uuid , 1=32 , 2=16 */

u_int8 SearchPattern_16[5] = {
  0x35,0x03, /* list */
  0x19,0x00,0x00 /* 16 bit uuid offset 3 */
};

u_int8 SearchPattern_32[7] = {
  0x35,0x05, /* list */
  0x1a,0x00,0x00,0x00,0x00 /* 32 bit offset 3 */
};

u_int8 SearchPattern_128[19] = {
  0x35,0x11, /* list */
  0x1c,0x00,0x00,0x00,0x00, /* 128 offset 3 for 32, 5 for 16 */
  0x00,0x00,0x10,0x00,0x80,0x00,0x00,0x80,0x5F,0x9B,0x34,0xFB
  /* 0x00,0x00,0x10,0x00,0x70,0x07,0x00,0x80,0x5f,0x9b,0x34,0xfb   */
};

u_int8 Attributes_pdu[5] = {
  0x35,0x03, /* list */
  0x09,0x00,0x00 /* offst 3 */
};

u_int8 Attributes_range[7] = {
	0x35,0x05,
	0x0a,0x00,0x00,0x00,0x00
};

int  prh_sdp_unit_init(int p1, void *arg);

int test_pattern(u_int8* pData,u_int16 search_pattern)
{
  int retvalue = 0;
  int i;

  switch(test_mode)
    {
    case 0:
      for(i = 0 ; i< sizeof(SearchPattern_128);i++)
    {
      pData[i] = SearchPattern_128[i];
    }
      pData[5] = search_pattern >> 8; 
      pData[6] = search_pattern & 0xff;
      
      retvalue = sizeof(SearchPattern_128);
      break;
    case 1:
      for(i = 0 ; i< sizeof(SearchPattern_32);i++)
    {
      pData[i] = SearchPattern_32[i];
    }
      pData[5] = search_pattern >> 8;
      pData[6] = search_pattern & 0xff;
      
      retvalue = sizeof(SearchPattern_32);
      break;
    case 2:
      for(i = 0 ; i< sizeof(SearchPattern_16);i++)
    {
      pData[i] = SearchPattern_16[i];
    }
      pData[3] = search_pattern >> 8; 
      pData[4] = search_pattern & 0xff;
      
      retvalue = sizeof(SearchPattern_16);
      break;

    default:
      test_state = 0;
      retvalue = -1;
    }
  
  return retvalue;
}

void test_attrib(u_int8* pData,u_int16 attrib)
{
  int i;
  
  for(i = 0; i< sizeof(Attributes_pdu) ; i++)
    {
      pData[i] = Attributes_pdu[i];
    }

  pData[3] = attrib >> 8;
  pData[4] = attrib & 0xff;
}

void test_attrib_range(u_int8* pData,u_int16 start, u_int16 stop)
{
  int i;
  
  for(i = 0; i< sizeof(Attributes_pdu) ; i++)
    {
      pData[i] = Attributes_pdu[i];
    }

  pData[3] = start >> 8;
  pData[4] = start & 0xff;

  pData[5] = stop >> 8;
  pData[6] = stop & 0xff;

}







/******
	*  TEST NAME:   
		prh_sdp_unit_001
 
	*  TEST DESCRIPTION:
		This test that a SDP_ServiceSearch will perform the normal case
 
	*  TEST STEPS:
		1. Call SDP_ServiceSearch
		2. accept connection.
		3. check request.
		4. send ss response
		5. check request.
		6. send sa response
		7. disconnect
		8. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_001(int p1, void *arg)
{
  /*
    This is a normal ss and sa
   */

  t_SDP_Results* pResults;
  t_SDP_Addresses pDevices;
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_StopRule pRule;
  t_bdaddr address;
  u_int32 aid;
  u_int32 uuids;
  void * connection;
  struct host_buf *buffer;
  int unused;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };

  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = { 0x02,0x00,0x00,0x00,0x0d,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			    0x11,0x03,0x00,0x10,0x00 };

  u_int8 test_results_b[] = { 0x04,0x00,0x01,0x00,0x0f,0x00,0x01,0x00,0x00,0x01,
			      0xd8,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00};

  pDevices.numItems = 1;
  pDevices.addresses = &address; /* it doesn't really matter */

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;


  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;


  pDebugCheck();

  SDP_ServiceSearch(&pResults,&pDevices,&pPattern,&pAttributes,&pRule,0x00,0x00);

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
		SDP_FreeResults(pResults);
	  return HU_FAILED;
  }
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

  prh_sdp_generate_search_response(buffer->data,handles,prh_sdp_read16(buffer->data + 1),1,1,0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_b,sizeof(test_results_b)) == HU_FAILED)
  {
	  SDP_FreeResults(pResults);
    return HU_FAILED;
  }

  prh_sdp_generate_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);


  prh_stub_l2cap_send_disconnect_confirmation(connection,0);

  if(pResults->element.dataLen == sizeof(data))
  {
      if(!memcmp(pResults->element.pData,data,sizeof(data)))
	  {
		  	  SDP_FreeResults(pResults);

		  return HU_PASSED;
	  } else {
		  	  SDP_FreeResults(pResults);

		  return HU_FAILED;
	  }
  } else {
	  	  SDP_FreeResults(pResults);
      return HU_FAILED;
  }  
}

/******
	*  TEST NAME:   
		prh_sdp_unit_002
 
	*  TEST DESCRIPTION:
		This test that a SDP_ServiceBrowse in the normal case.
 
	*  TEST STEPS:
		1. Call normal SDP_ServiceBrowse
		2. accept connection.
		3. check request.
		4. send sar response
		5. disconnect
		6. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_002(int p1, void *arg)
{
  /*
    This is a normal browse
   */

  t_SDP_Results* pResults;
  t_SDP_Addresses pDevices;
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_StopRule pRule;
  t_bdaddr address;
  u_int32 aid;
  u_int32 uuids;
  void * connection;
  struct host_buf *buffer;
  int unused;

  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x03,0x00,0x15,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x01,0xd8,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

	
  pDevices.numItems = 1;
  pDevices.addresses = &address; /* it doesn't really matter */

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;


  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;

  SDP_ServiceBrowse(&pResults,&pDevices,&pPattern,&pAttributes,&pRule,0x00,0x00);

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  	  SDP_FreeResults(pResults);

    return HU_FAILED;
  }
  prh_sdp_generate_service_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_send_disconnect_confirmation(connection,0);

  if(pResults->element.dataLen == sizeof(data))
  {
      if(!memcmp(pResults->element.pData,data,sizeof(data)))
	  {
		  	  SDP_FreeResults(pResults);

		  return HU_PASSED;
	  } else {
		  	  SDP_FreeResults(pResults);

		  return HU_FAILED;
	  }
  } else {
	  	  SDP_FreeResults(pResults);

      return HU_FAILED;
  }  
}


/******
	*  TEST NAME:   
		prh_sdp_unit_003
 
	*  TEST DESCRIPTION:
		This test that a SDP_ServiceSearch will handle 2 addresses.
 
	*  TEST STEPS:
		1. Call normal SDP_ServiceSearch
		2. accept connection.
		3. check request.
		4. send ss response
		5. check request.
		6. send sa response
		7. disconnect
		8. accept connection.
		9. check request.
		10. send ss response
		11. check request.
		12. send sa response
		13. disconnect
		14. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_003(int p1, void *arg)
{
  /*
    This is a normal ss and sa for two addresses
   */

  t_SDP_Results* pResults;
  t_SDP_Addresses pDevices;
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_StopRule pRule;
  t_bdaddr addresses[2];
  u_int32 aid;
  u_int32 uuids;
  void * connection;
  struct host_buf *buffer;
  int unused;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };

  u_int8 data[] = "Here is some random gibberish";

 
  pDevices.numItems = 2;
  pDevices.addresses = addresses; /* it doesn't really matter */

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;


  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;

  SDP_ServiceSearch(&pResults,&pDevices,&pPattern,&pAttributes,&pRule,0x00,0x00);

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  prh_sdp_generate_search_response(buffer->data,handles,prh_sdp_read16(buffer->data + 1),1,1,0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  prh_sdp_generate_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_send_disconnect_confirmation(connection,0);

  /*
    second device
   */

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  prh_sdp_generate_search_response(buffer->data,handles,prh_sdp_read16(buffer->data + 1),1,1,0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  prh_sdp_generate_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_send_disconnect_confirmation(connection,0);

  if(pResults->numElements ==2)
  {
      if(pResults->element.dataLen == sizeof(data))
	  {
	  } else {
		  	  SDP_FreeResults(pResults);

		  return HU_FAILED;
	  }
  } else {
	  	  SDP_FreeResults(pResults);

	  return HU_FAILED;
  }
	  SDP_FreeResults(pResults);

  return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_004
 
	*  TEST DESCRIPTION:
		This test that a  SDP_ServiceBrowse will handle 2 addresses.
 
	*  TEST STEPS:
		1. Call normal SDP_ServiceBrowse
		2. accept connection.
		3. check request.
		4. send sar response
		5. disconnect
		6. accept connection.
		7. check request.
		8. send sar response
		9. disconnect
		10. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_004(int p1, void *arg)
{
  /*
    This is a normal browse with multiple addresses
   */

  t_SDP_Results* pResults;
  t_SDP_Addresses pDevices;
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_StopRule pRule;
  t_bdaddr addresses[2];
  u_int32 aid;
  u_int32 uuids;
  void * connection;
  struct host_buf *buffer;
  int unused;

  u_int8 data[] = "Here is some random gibberish";

  

  pDevices.numItems = 2;
  pDevices.addresses = addresses; /* it doesn't really matter */

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;


  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;

  SDP_ServiceBrowse(&pResults,&pDevices,&pPattern,&pAttributes,&pRule,0x00,0x00);

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  prh_sdp_generate_service_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_send_disconnect_confirmation(connection,0);

  /*
    second decvice
  */

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  prh_sdp_generate_service_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_send_disconnect_confirmation(connection,0);

  if(pResults->numElements ==2)
  {
      if(pResults->element.dataLen == sizeof(data))
	  {
	  } else {
		  	  SDP_FreeResults(pResults);
return HU_FAILED;
	  }
  } else {
	  	  SDP_FreeResults(pResults);
	  return HU_FAILED;
  }

  	  SDP_FreeResults(pResults);

  return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_005
 
	*  TEST DESCRIPTION:
		This test that a service search will handle a continue correctly for SS
 
	*  TEST STEPS:
		1. Call normal service search
		2. create and send a ss resonse with a continue
		3. check that the next request from the statemachine contained the correct continue 
		information.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_005(int p1, void *arg)
{
  /*
    This is a normal ss and sa with continue for ss
   */

  t_SDP_Results* pResults;
  t_SDP_Addresses pDevices;
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_StopRule pRule;
  t_bdaddr address;
  u_int32 aid;
  u_int32 uuids;
  void * connection;
  struct host_buf *buffer;
  int unused;
  u_int8 cont[] = { 0x04,'c','o','n','t'};
  u_int32 handles[] = { 0x00010000 , 0x00010001 };

  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x02,0x00,0x00,0x00,0x11,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x10,0x04,'c','o','n','t'
  };

  pDevices.numItems = 1;
  pDevices.addresses = &address; /* it doesn't really matter */

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;


  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;

  SDP_ServiceSearch(&pResults,&pDevices,&pPattern,&pAttributes,&pRule,0x00,0x00);

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

  prh_sdp_generate_search_response(buffer->data,handles,prh_sdp_read16(buffer->data + 1),2,1,cont);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  	  SDP_FreeResults(pResults);

    return HU_FAILED;
  }

  prh_sdp_generate_search_response(buffer->data,handles,prh_sdp_read16(buffer->data + 1),2,1,0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  prh_sdp_generate_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

   prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

/*	prh_stub_l2cap_send_disconnect_confirmation(connection,0);*/
	prh_stub_l2cap_do_disconnectind(connection);

    SDP_FreeResults(pResults);

  return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_006
 
	*  TEST DESCRIPTION:
		This test that a service search will handle a continue correctly for SA
 
	*  TEST STEPS:
		1. Call normal service search
		2. create and send a sa resonse with a continue
		3. check that the next request from the statemachine contained the correct continue 
		information.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
 static prh_t_hu_outcome prh_sdp_unit_006(int p1, void *arg)
{
  /*
    This is a normal ss and sa
   */

  t_SDP_Results* pResults;
  t_SDP_Addresses pDevices;
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_StopRule pRule;
  t_bdaddr address;
  u_int32 aid;
  u_int32 uuids;
  void * connection;
  struct host_buf *buffer;
  int unused;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };
  u_int8 cont[] = { 0x04,'c','o','n','t' };

  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x04,0x00,0x00,0x00,0x10,0x00,0x01,0x00,0x00,0x01,
			     0xd8,0x35,0x03,0x09,0x00,0x03,0x04,'c','o','n','t'
  };
  

  pDevices.numItems = 1;
  pDevices.addresses = &address; /* it doesn't really matter */

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;


  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;

  SDP_ServiceSearch(&pResults,&pDevices,&pPattern,&pAttributes,&pRule,0x00,0x00);

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

  prh_sdp_generate_search_response(buffer->data,handles,prh_sdp_read16(buffer->data + 1),1,1,0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  prh_sdp_generate_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),cont);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  
  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  SDP_FreeResults(pResults);

    return HU_FAILED;
  }
  prh_sdp_generate_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_send_disconnect_confirmation(connection,0);

  SDP_FreeResults(pResults);

  return HU_PASSED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_007
 
	*  TEST DESCRIPTION:
		This test that a SDP_ServiceBrowse with continue
 
	*  TEST STEPS:
		1. Call normal SDP_ServiceBrowse
		2. accept connection.
		3. check request.
		4. send sar response with continue
		5. check request.
		6. send sar response 
		7. disconnect
		8. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_007(int p1, void *arg)
{
  /*
    This is a normal browse with continue
   */

  t_SDP_Results* pResults;
  t_SDP_Addresses pDevices;
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_StopRule pRule;
  t_bdaddr address;
  u_int32 aid;
  u_int32 uuids;
  void * connection;
  struct host_buf *buffer;
  int unused;
  u_int8 cont[] = { 0x04,'c','o','n','t' };
  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x00,0x00,0x19,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x01,0xd8,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x04,
			     'c','o','n','t'
  };

  pDevices.numItems = 1;
  pDevices.addresses = &address; /* it doesn't really matter */

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;


  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;

  SDP_ServiceBrowse(&pResults,&pDevices,&pPattern,&pAttributes,&pRule,0x00,0x00);

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);  
  prh_sdp_generate_service_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),cont);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);  

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {  
	  
		prh_stub_l2cap_do_disconnectind(connection); /* remove the connection */
	  	  SDP_FreeResults(pResults);

    return HU_FAILED;
  }
  prh_sdp_generate_service_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_send_disconnect_confirmation(connection,0);

	  SDP_FreeResults(pResults);


  return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_008
 
	*  TEST DESCRIPTION:
		This test that a service search will handle 2 addresses.
 
	*  TEST STEPS:
		1. Call normal service search
		2. accept connection.
		3. check request.
		4. call normal service search
		5. send ss response
		6. check request.
		7. send sa response
		8. disconnect
		9. check results
		10. accept connection.
		11. check request.
		12. send ss response
		13. check request.
		14. send sa response
		15. disconnect
		16. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_008(int p1, void *arg)
{
  /*
    This is a normal ss and sa
   */

  t_SDP_Results* pResults;
  t_SDP_Addresses pDevices;
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_StopRule pRule;
  t_bdaddr address;
  u_int32 aid;
  u_int32 uuids;
  void * connection;
  struct host_buf *buffer;
  int unused;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };

  u_int8 data[] = "Here is some random gibberish";

  

  pDevices.numItems = 1;
  pDevices.addresses = &address; /* it doesn't really matter */

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;


  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pRule.maxItems = 2;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;

  SDP_ServiceSearch(&pResults,&pDevices,&pPattern,&pAttributes,&pRule,0x00,0x00);

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

  prh_sdp_generate_search_response(buffer->data,handles,prh_sdp_read16(buffer->data + 1),1,1,0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  prh_sdp_generate_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_send_disconnect_confirmation(connection,0);

  SDP_FreeResults(pResults);

  return HU_PASSED;
}

static prh_t_hu_outcome prh_sdp_unit_009(int p1, void *arg)
{
  /*
    two browse connections.
   */

  t_SDP_Results* pResults,*pFirst;
  t_SDP_Addresses pDevices;
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_StopRule pRule;
  t_bdaddr address;
  u_int32 aid;
  u_int32 uuids;
  void * connection;
  struct host_buf *buffer;
  int unused;
  u_int8 data[] = "Here is some random gibberish";

  

  pDevices.numItems = 1;
  pDevices.addresses = &address; /* it doesn't really matter */

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;


  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;

  SDP_ServiceBrowse(&pResults,&pDevices,&pPattern,&pAttributes,&pRule,0x00,0x00);

  pFirst = pResults;

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);  

  SDP_ServiceBrowse(&pResults,&pDevices,&pPattern,&pAttributes,&pRule,0x00,0x00);

  prh_sdp_generate_service_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_send_disconnect_confirmation(connection,0);

  /*
    second connection
   */
  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);  

  prh_sdp_generate_service_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_send_disconnect_confirmation(connection,0);

  SDP_FreeResults(pFirst);

  SDP_FreeResults(pResults);

  return HU_PASSED;
}



static prh_t_hu_outcome prh_sdp_unit_033(int p1, void *arg)
{
    /*
    This is a normal browse
   */

  t_SDP_Results* pResults;
  t_SDP_Addresses pDevices;
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_StopRule pRule;
  t_bdaddr address;
  u_int32 aid;
  u_int32 uuids;
  void * connection;
  struct host_buf *buffer;
  int unused;

  u_int8 data[] = "Here is some random gibberish";  

  pDevices.numItems = 1;
  pDevices.addresses = &address; /* it doesn't really matter */

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;


  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;

  SDP_ServiceBrowse(&pResults,&pDevices,&pPattern,&pAttributes,&pRule,0x00,0x00);

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

  prh_sdp_generate_service_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);;
  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_send_disconnect_confirmation(connection,0);

  SDP_FreeResults(pResults);

  return HU_PASSED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_073
 
	*  TEST DESCRIPTION:
		Test the state machine for normal generic browse
 
	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back public browse group info
		4. get browse groups.
		5. get one group info.
		6 disconnect.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_073(int p1, void *arg)
{
  t_SDP_Results* pResults;
  t_SDP_Addresses pDevices;
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_StopRule pRule;
  t_bdaddr address;
  u_int32 aid;
  u_int32 uuids;
  void * connection;
  struct host_buf *buffer;
  int unused;

  u_int8 data1[] = {
    /*    0x07,0x00,0x00,0x00,0x14,0x00,0x11,*/
    0x35,0x13,0x35,0x11,0x09,0x00,0x01,0x35,0x03,0x19,0x10,0x01,0x09,0x01,0x00,0x25,0x04,'T','e','s','t',
    /*    0x00*/
  };

  u_int8 data2[] = {
    /*    0x07,0x00,0x00,0x00,0x0e,0x00,0x0b,*/
    0x35,0x08,0x35,0x06,0x09,0x02,0x00,0x19,0xe0,0x00,
    /*    0x00*/
  };

  u_int8 data3[] = {
    /*    0x07,0x00,0x00,0x00,0x14,0x00,0x11,*/
    0x35,0x13,0x35,0x11,0x09,0x00,0x01,0x35,0x03,0x19,0x10,0x01,0x09,0x01,0x00,0x25,0x04,'T','e','s','t',
    /*    0x00*/
  };


  pDevices.numItems = 1;
  pDevices.addresses = &address; /* it doesn't really matter */

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;


  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;

  SDP_GenericBrowse(&pResults,&pDevices,&pAttributes,&pRule,0x00,0x00);

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

  prh_sdp_generate_service_attrib_response(buffer->data,data1,prh_sdp_read16(buffer->data + 1),sizeof(data1),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);;
  prh_stub_l2cap_put_data(connection,buffer);


  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

  prh_sdp_generate_service_attrib_response(buffer->data,data2,prh_sdp_read16(buffer->data + 1),sizeof(data2),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);;
  prh_stub_l2cap_put_data(connection,buffer);


  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

  prh_sdp_generate_service_attrib_response(buffer->data,data3,prh_sdp_read16(buffer->data + 1),sizeof(data3),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);;
  prh_stub_l2cap_put_data(connection,buffer);


  prh_stub_l2cap_send_disconnect_confirmation(connection,0);

  SDP_FreeResults(pResults);

  return HU_PASSED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_091
 
	*  TEST DESCRIPTION:
		Test the state machine for a normal inquiry will work
 
	*  TEST STEPS:
		1. call SDP_EnumerateDevices
		2. call the inquiry callback with results
		3. call the inquiry callback to terminate the session.
		4. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_091(int p1, void *arg)
{	
  t_SDP_Addresses* pDevices;
  t_SDP_StopRule pRule;
  t_MGR_DeviceEntry entry;
  t_bdaddr address;
  int ret;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;

  SDP_EnumerateDevices( &pDevices,0x00,0x00,&pRule,0x00,0x00);

  address.bytes[0] = 2;

  entry.address = address;
  entry.classOfDevice = 3;

  prh_sdp_manager_inquiry_cb(&entry,0x00);
  
  prh_sdp_manager_inquiry_cb(0x00,0x00);


  if(pDevices->numItems == 1)
	  ret = HU_PASSED;
	else
		ret = HU_FAILED;

	/* free resources */
  SDP_FreeAddressList(pDevices);

return ret;

}

/******
	*  TEST NAME:   
		prh_sdp_unit_106
 
	*  TEST DESCRIPTION:
		Test the the address list is freed
 
	*  TEST STEPS:
		1. create an address list with 5 elements in it.
		2. call SDP_FreeAddressList		

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_106(int p1, void *arg)
{
  /*
    This is a normal browse
   */

  t_SDP_Addresses* pDevices;

	pDevices = pMalloc(sizeof(t_SDP_Addresses));

	pDevices->addresses = pMalloc(sizeof(t_bdaddr)*5);
	pDevices->numItems = 5;

	SDP_FreeAddressList(pDevices);

	return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_074
 
	*  TEST DESCRIPTION:
		Test the state machine for normal generic browse 
 
	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back continue for the service search attribute
		4. send back remainder of public browse group info
		5. get browse groups.
		6. get one group info.
		7 disconnect.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_074(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_075
 
	*  TEST DESCRIPTION:
		duplicate - same as 074
 
	*  TEST STEPS:
		1. call test 074

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_075(int p1, void *arg)
{
	return prh_sdp_unit_074(p1,arg);
}

/******
	*  TEST NAME:   
		prh_sdp_unit_076
 
	*  TEST DESCRIPTION:
		Test the state machine for normal generic browse when there are no attributes
		for a specific group.
 
	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back no information for the group
		4. disconnect.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_076(int p1, void *arg)
{
  t_SDP_Results* pResults;
  t_SDP_Addresses pDevices;
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_StopRule pRule;
  t_bdaddr address;
  u_int32 aid;
  u_int32 uuids;
  void * connection;
  struct host_buf *buffer;
  int unused;

  u_int8 data1[] = {
    /*    0x07,0x00,0x00,0x00,0x14,0x00,0x11,*/
    0x35,0x00,
    /*    0x00*/
  };

  u_int8 data2[] = {
    /*    0x07,0x00,0x00,0x00,0x0e,0x00,0x0b,*/
    0x35,0x00,
    /*    0x00*/
  };

  u_int8 data3[] = {
    /*    0x07,0x00,0x00,0x00,0x14,0x00,0x11,*/
    0x35,0x13,0x35,0x11,0x09,0x00,0x01,0x35,0x03,0x19,0x10,0x01,0x09,0x01,0x00,0x25,0x04,'T','e','s','t',
    /*    0x00*/
  };


  pDevices.numItems = 1;
  pDevices.addresses = &address; /* it doesn't really matter */

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;


  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;

  SDP_GenericBrowse(&pResults,&pDevices,&pAttributes,&pRule,0x00,0x00);

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

  prh_sdp_generate_service_attrib_response(buffer->data,data1,prh_sdp_read16(buffer->data + 1),sizeof(data1),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);;
  prh_stub_l2cap_put_data(connection,buffer);


  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

  prh_sdp_generate_service_attrib_response(buffer->data,data2,prh_sdp_read16(buffer->data + 1),sizeof(data2),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);;
  prh_stub_l2cap_put_data(connection,buffer);


  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */
/*
  prh_sdp_generate_service_attrib_response(buffer->data,data3,prh_sdp_read16(buffer->data + 1),sizeof(data3),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);;
  prh_stub_l2cap_put_data(connection,buffer);
*/

  prh_stub_l2cap_send_disconnect_confirmation(connection,0);

  SDP_FreeResults(pResults);

  return HU_FAILED; /* will require more checking */
}


/******
	*  TEST NAME:   
		prh_sdp_unit_077
 
	*  TEST DESCRIPTION:
		Test the state machine for normal generic browse
 
	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back public browse group info
		4. not browse group info.
		5. disconnect.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_077(int p1, void *arg)
{
  t_SDP_Results* pResults;
  t_SDP_Addresses pDevices;
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_StopRule pRule;
  t_bdaddr address;
  u_int32 aid;
  u_int32 uuids;
  void * connection;
  struct host_buf *buffer;
  int unused;

  u_int8 data1[] = {
    /*    0x07,0x00,0x00,0x00,0x14,0x00,0x11,*/
    0x35,0x13,0x35,0x11,0x09,0x00,0x01,0x35,0x03,0x19,0x10,0x01,0x09,0x01,0x00,0x25,0x04,'T','e','s','t',
    /*    0x00*/
  };

  u_int8 data2[] = {
    /*    0x07,0x00,0x00,0x00,0x0e,0x00,0x0b,*/
    0x35,0x00,
    /*    0x00*/
  };

  u_int8 data3[] = {
    /*    0x07,0x00,0x00,0x00,0x14,0x00,0x11,*/
    0x35,0x13,0x35,0x11,0x09,0x00,0x01,0x35,0x03,0x19,0x10,0x01,0x09,0x01,0x00,0x25,0x04,'T','e','s','t',
    /*    0x00*/
  };


  pDevices.numItems = 1;
  pDevices.addresses = &address; /* it doesn't really matter */

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;


  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;

  SDP_GenericBrowse(&pResults,&pDevices,&pAttributes,&pRule,0x00,0x00);

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

  prh_sdp_generate_service_attrib_response(buffer->data,data1,prh_sdp_read16(buffer->data + 1),sizeof(data1),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);;
  prh_stub_l2cap_put_data(connection,buffer);


  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

  prh_sdp_generate_service_attrib_response(buffer->data,data2,prh_sdp_read16(buffer->data + 1),sizeof(data2),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);;
  prh_stub_l2cap_put_data(connection,buffer);


  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */
/*
  prh_sdp_generate_service_attrib_response(buffer->data,data3,prh_sdp_read16(buffer->data + 1),sizeof(data3),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);;
  prh_stub_l2cap_put_data(connection,buffer);
*/

  prh_stub_l2cap_send_disconnect_confirmation(connection,0);

  SDP_FreeResults(pResults);

  return HU_FAILED; /* wil  require more checking */
}

/******
	*  TEST NAME:   
		prh_sdp_unit_078
 
	*  TEST DESCRIPTION:
		Test the state machine for normal generic browse
 
	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back public browse group info
		4. get browse groups.
		5. send back one group info with one of the attributes missing.
		6 disconnect.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_078(int p1, void *arg)
{
  t_SDP_Results* pResults;
  t_SDP_Addresses pDevices;
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_StopRule pRule;
  t_bdaddr address;
  u_int32 aid;
  u_int32 uuids;
  void * connection;
  struct host_buf *buffer;
  int unused;

  u_int8 data1[] = {
    /*    0x07,0x00,0x00,0x00,0x14,0x00,0x11,*/
    0x35,0x13,0x35,0x11,0x09,0x00,0x01,0x35,0x03,0x19,0x10,0x01,0x09,0x01,0x00,0x25,0x04,'T','e','s','t',
    /*    0x00*/
  };

  u_int8 data2[] = {
    /*    0x07,0x00,0x00,0x00,0x0e,0x00,0x0b,*/
    0x35,0x08,0x35,0x06,0x09,0x02,0x00,0x19,0xe0,0x00,
    /*    0x00*/
  };

  u_int8 data3[] = {
    /*    0x07,0x00,0x00,0x00,0x14,0x00,0x11,*/
    0x35,0x0a,0x35,0x08,0x09,0x00,0x01,0x35,0x03,0x19,0x10,0x01,
    /*    0x00*/
  };


  pDevices.numItems = 1;
  pDevices.addresses = &address; /* it doesn't really matter */

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;


  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;

  SDP_GenericBrowse(&pResults,&pDevices,&pAttributes,&pRule,0x00,0x00);

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

  prh_sdp_generate_service_attrib_response(buffer->data,data1,prh_sdp_read16(buffer->data + 1),sizeof(data1),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);;
  prh_stub_l2cap_put_data(connection,buffer);


  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

  prh_sdp_generate_service_attrib_response(buffer->data,data2,prh_sdp_read16(buffer->data + 1),sizeof(data2),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);;
  prh_stub_l2cap_put_data(connection,buffer);


  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

  prh_sdp_generate_service_attrib_response(buffer->data,data3,prh_sdp_read16(buffer->data + 1),sizeof(data3),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);;
  prh_stub_l2cap_put_data(connection,buffer);


  prh_stub_l2cap_send_disconnect_confirmation(connection,0);

  SDP_FreeResults(pResults);

  return HU_FAILED; /* more checking required */
}


/******
	*  TEST NAME:   
		prh_sdp_unit_079
 
	*  TEST DESCRIPTION:
		Test the state machine for normal generic browse
 
	*  TEST STEPS:
		1. call SDP_GenericBrowse with two 16bit uuids
		2. fake connection.
		3. send back public browse group info
		4. get browse groups.
		5. get one group info.
		6 disconnect.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_079(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_080
 
	*  TEST DESCRIPTION:
		Test the state machine for normal generic browse
 
	*  TEST STEPS:
		1. call SDP_GenericBrowse with 32bit uuid
		2. fake connection.
		3. send back public browse group info
		4. get browse groups.
		5. get one group info.
		6 disconnect.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_080(int p1, void *arg)
{
  t_SDP_Results* pResults;
  t_SDP_Addresses pDevices;
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_StopRule pRule;
  t_bdaddr address;
  u_int32 aid;
  u_int32 uuids;
  void * connection;
  struct host_buf *buffer;
  int unused;

  u_int8 data1[] = {
    /*    0x07,0x00,0x00,0x00,0x14,0x00,0x11,*/
    0x35,0x13,0x35,0x11,0x09,0x00,0x01,0x35,0x03,0x19,0x10,0x01,0x09,0x01,0x00,0x25,0x04,'T','e','s','t',
    /*    0x00*/
  };

  u_int8 data2[] = {
    /*    0x07,0x00,0x00,0x00,0x0e,0x00,0x0b,*/
    0x35,0x08,0x35,0x06,0x09,0x02,0x00,0x19,0xe0,0x00,
    /*    0x00*/
  };

  u_int8 data3[] = {
    /*    0x07,0x00,0x00,0x00,0x14,0x00,0x11,*/
    0x35,0x13,0x35,0x11,0x09,0x00,0x01,0x35,0x03,0x19,0x10,0x01,0x09,0x01,0x00,0x25,0x04,'T','e','s','t',
    /*    0x00*/
  };


  pDevices.numItems = 1;
  pDevices.addresses = &address; /* it doesn't really matter */

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;


  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;

  SDP_GenericBrowse(&pResults,&pDevices,&pAttributes,&pRule,0x00,0x00);

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

  prh_sdp_generate_service_attrib_response(buffer->data,data1,prh_sdp_read16(buffer->data + 1),sizeof(data1),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);;
  prh_stub_l2cap_put_data(connection,buffer);


  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

  prh_sdp_generate_service_attrib_response(buffer->data,data2,prh_sdp_read16(buffer->data + 1),sizeof(data2),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);;
  prh_stub_l2cap_put_data(connection,buffer);


  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

  prh_sdp_generate_service_attrib_response(buffer->data,data3,prh_sdp_read16(buffer->data + 1),sizeof(data3),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);;
  prh_stub_l2cap_put_data(connection,buffer);


  prh_stub_l2cap_send_disconnect_confirmation(connection,0);

  SDP_FreeResults(pResults);

  return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_092
 
	*  TEST DESCRIPTION:
		Test the state machine for a limited inquiry will work
 
	*  TEST STEPS:
		1. call SDP_EnumerateDevices with limited inquiry access code
		2. call the inquiry callback with results
		3. call the inquiry callback to terminat the session
		4. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_092(int p1, void *arg)
{
  t_SDP_Addresses* pDevices;
  t_SDP_StopRule pRule;
  t_MGR_DeviceEntry entry;
  t_bdaddr address;
  int ret;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;

  SDP_EnumerateDevices( &pDevices,0x00,BT_LIAC_LAP,&pRule,0x00,0x00);

  address.bytes[0] = 2;

  entry.address = address;
  entry.classOfDevice = 3;

  prh_sdp_manager_inquiry_cb(&entry,0x00);
  
  prh_sdp_manager_inquiry_cb(0x00,0x00);


  if(pDevices->numItems == 1)
	  ret = HU_PASSED;
	else
		ret = HU_FAILED;

	/* free resources */
  SDP_FreeAddressList(pDevices);

  
  return ret;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_093
 
	*  TEST DESCRIPTION:
		Test the state machine for a normal inquiry will work when multiple
		results are returned.
 
	*  TEST STEPS:
		1. call SDP_EnumerateDevices
		2. call the inquiry callback with one set of results
		3. call the inquiry with the second results.
		4. call the final inquiry callback to terminate the session.
		3. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_093(int p1, void *arg)
{
  t_SDP_Addresses* pDevices;
  t_SDP_StopRule pRule;
  t_MGR_DeviceEntry entry;
  t_bdaddr address;
  int ret;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;

  SDP_EnumerateDevices( &pDevices,0x00,0x00,&pRule,0x00,0x00);

  address.bytes[4] = 4;

  entry.address = address;
  entry.classOfDevice = 3;

  prh_sdp_manager_inquiry_cb(&entry,0x00);
  entry.address.bytes[3] = 2;
  prh_sdp_manager_inquiry_cb(&entry,0x00);
  
  prh_sdp_manager_inquiry_cb(0x00,0x00); /* inquiry complete */


  if(pDevices->numItems == 2)
	  ret = HU_PASSED;
	else
		ret = HU_FAILED;

	/* free resources */
  SDP_FreeAddressList(pDevices);

  return ret;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_094
 
	*  TEST DESCRIPTION:
		Test the state machine for a normal inquiry will work when filtering
		on the class of device.
 
	*  TEST STEPS:
		1. call SDP_EnumerateDevices
		2. call the inquiry callback with results
		3. call the inquiry callback with results that don't match the cod
		4. call the inquiry callback to terminate the session.
		5. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_094(int p1, void *arg)
{
t_SDP_Addresses* pDevices;
  t_SDP_StopRule pRule;
  t_MGR_DeviceEntry entry;
  t_bdaddr address;
  int ret;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;

  SDP_EnumerateDevices( &pDevices,5,0x00,&pRule,0x00,0x00);

  address.bytes[4] = 6;

  entry.address = address;
  entry.classOfDevice = 5;

  prh_sdp_manager_inquiry_cb(&entry,0x00);

  entry.classOfDevice = 4;
  entry.address.bytes[4] = 5;

  prh_sdp_manager_inquiry_cb(&entry,0x00);

  prh_sdp_manager_inquiry_cb(0x00,0x00);


  if(pDevices->numItems == 1)
	  ret = HU_PASSED;
	else
		ret = HU_FAILED;

	/* free resources */
  SDP_FreeAddressList(pDevices);

  return ret;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_095
 
	*  TEST DESCRIPTION:
		Test the state machine for a normal inquiry will work with no 
		results.
 
	*  TEST STEPS:
		1. call SDP_EnumerateDevices
		2. call the inquiry callback to terminate the session
		3. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_095(int p1, void *arg)
{
	
  t_SDP_Addresses* pDevices;
  t_SDP_StopRule pRule;
  int ret;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;

  SDP_EnumerateDevices( &pDevices,0x00,0x00,&pRule,0x00,0x00);

  
  prh_sdp_manager_inquiry_cb(0x00,0x00);


  if(pDevices->numItems == 0)
	  ret = HU_PASSED;
	else
		ret = HU_FAILED;

	/* free resources */
  SDP_FreeAddressList(pDevices);

	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_099
 
	*  TEST DESCRIPTION:
		Test the state machine for terminating a primitive
 
	*  TEST STEPS:
		1. call SDP_ServiceSearch
		2. fake connection
		3. call SDP_TerminatePrimitive
		4. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_099(int p1, void *arg)
{
  t_SDP_Results* pResults;
  t_SDP_Addresses pDevices;
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_StopRule pRule;
  t_bdaddr address;
  u_int32 aid;
  u_int32 uuids;
  void * connection;
  struct host_buf *buffer;
  int unused;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };
  int ret = HU_FAILED;

  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = { 0x02,0x00,0x00,0x00,0x0d,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			    0x11,0x03,0x00,0x10,0x00 };

  u_int8 test_results_b[] = { 0x04,0x00,0x01,0x00,0x0c,0x00,0x01,0x00,0x00,0x01,
			      0xd8,0x35,0x03,0x09,0x00,0x03,0x00};

  pDevices.numItems = 1;
  pDevices.addresses = &address; /* it doesn't really matter */

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;


  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;


  pDebugCheck();

  SDP_ServiceSearch(&pResults,&pDevices,&pPattern,&pAttributes,&pRule,0x00,0x00);

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
		SDP_FreeResults(pResults);
	  return HU_FAILED;
  }

	SDP_TerminatePrimitive(pResults);  

	if(p_SDP_Primitive_Task_List==0)
	{
		ret = HU_PASSED;
	}

	SDP_FreeResults(pResults);

	 prh_stub_l2cap_do_disconnectind(connection); /* cleanup */

	return ret;

}

/******
	*  TEST NAME:   
		prh_sdp_unit_103
 
	*  TEST DESCRIPTION:
		Test the SDP_FreeResults
 
	*  TEST STEPS:
		1. create a results struct with 5 elements deep
		2. call SDP_FreeResults.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_103(int p1, void *arg)
{
	t_SDP_Results* pResults;
	t_SDP_Element* ele;

	pResults = pMalloc(sizeof(t_SDP_Results));
			
	ele = &(pResults->element);

	ele->pData = pMalloc(10);
	ele->deviceName = 0x00;
	ele->pNext = pMalloc(sizeof(t_SDP_Element));
	ele = ele->pNext;

	ele->pData = pMalloc(10);
	ele->deviceName = 0x00;
	ele->pNext = pMalloc(sizeof(t_SDP_Element));
	ele = ele->pNext;	
	
	ele->pData = pMalloc(10);
	ele->deviceName = 0x00;
	ele->pNext = pMalloc(sizeof(t_SDP_Element));
	ele = ele->pNext;

	ele->pData = pMalloc(10);
	ele->deviceName = 0x00;
	ele->pNext = pMalloc(sizeof(t_SDP_Element));
	ele = ele->pNext;

	ele->pData = pMalloc(10);
	ele->deviceName = 0x00;
	ele->pNext = pMalloc(sizeof(t_SDP_Element));
	ele = ele->pNext;

	ele->pData = pMalloc(10);
	ele->deviceName = 0x00;
	ele->pNext = 0;


	SDP_FreeResults(pResults);

	return HU_PASSED; /* manualy checked and fine. */
}

/******
	*  TEST NAME:   
		prh_sdp_unit_145
 
	*  TEST DESCRIPTION:
		Test the generation of a c file for the embedded database
 
	*  TEST STEPS:
		1. create buffer to send to file.
		2. call sdp_db_to_file

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_145(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_220
 
	*  TEST DESCRIPTION:
		Reproduce bug 1545, i.e. 2 matching records handles found in the service search, thus 
		it will have to cater for the next handle case.
 
	*  TEST STEPS:
		1. Call SDP_ServiceSearch
		2. accept the connection
		3. check request
		4. send results for ss
		5. check pdu.
		6. check request.
		7. send results for sa
		8.  check pdu.
		9. send results for sa
		10. check pdu.
		11. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_220(int p1, void *arg)
{
  /*
    This is a normal ss and sa
   */

  t_SDP_Results* pResults;
  t_SDP_Addresses pDevices;
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_StopRule pRule;
  t_bdaddr address;
  u_int32 aid;
  u_int32 uuids;
  void * connection;
  struct host_buf *buffer;
  int unused;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };

  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = { 0x02,0x00,0x00,0x00,0x0d,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			    0x11,0x03,0x00,0x10,0x00 };

  u_int8 test_results_b[] = { 0x04,0x00,0x01,0x00,0x0c,0x00,0x01,0x00,0x00,0x01,
			      0xd8,0x35,0x03,0x09,0x00,0x03,0x00};

  pDevices.numItems = 1;
  pDevices.addresses = &address; /* it doesn't really matter */

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;


  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;


  pDebugCheck();

  SDP_ServiceSearch(&pResults,&pDevices,&pPattern,&pAttributes,&pRule,0x00,0x00);

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
		SDP_FreeResults(pResults);
	  return HU_FAILED;
  }
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

  prh_sdp_generate_search_response(buffer->data,handles,prh_sdp_read16(buffer->data + 1),2,2,0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

/*  if(prh_sdp_test_check(buffer,test_results_b,sizeof(test_results_b)) == HU_FAILED)
  {
	  SDP_FreeResults(pResults);
    return HU_FAILED;
  }*/

  prh_sdp_generate_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

	/* second one */

 prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

/*  if(prh_sdp_test_check(buffer,test_results_b,sizeof(test_results_b)) == HU_FAILED)
  {
	  SDP_FreeResults(pResults);
    return HU_FAILED;
  }*/

  prh_sdp_generate_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

	/* bring down the connection as normal */

  prh_stub_l2cap_send_disconnect_confirmation(connection,0);
  
  if(pResults->element.dataLen == sizeof(data)*2)
  {
      if(!memcmp(pResults->element.pData,data,sizeof(data)))
	  {
		  /* OK */
	  } else {
		  SDP_FreeResults(pResults);
		  
		  return HU_FAILED;
	  }
	  if(!memcmp(pResults->element.pData+sizeof(data),data,sizeof(data))) /* second half */
	  {
		/* OK */
	  } else {
		  SDP_FreeResults(pResults);
		  
		  return HU_FAILED;
	  }
	  
	  SDP_FreeResults(pResults);
	  
	  return HU_PASSED;
  } else {
	  	  SDP_FreeResults(pResults);
		  return HU_FAILED;
  }  
}


/******
	*  TEST NAME:   
		prh_sdp_unit_221
 
	*  TEST DESCRIPTION:
		This will test the SDP state machine for service search to see how it handles
		searching for 2 devices and the first device disconnects.
 
	*  TEST STEPS:
		1. call SDP_ServiceSearch for 2 addresses
		2. accept the connection
		3. send disconnect from the peer.		
		4. accept the connection for the second
		5. check request
		6. send results for ss
		7. check pdu.
		8. check request.
		9. send results for sa
		10. check pdu.
		11. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_221(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_222
 
	*  TEST DESCRIPTION:
		This test will send a servic attribute request using an aid range
 
	*  TEST STEPS:
		1. call SDP_ServiceSearch with aid range
		2. accept the connection
		3. check request
		4. send results for ss
		5. check pdu.
		6. check request.
		7. send results for sa
		8.  check pdu.
		9. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_222(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_223
 
	*  TEST DESCRIPTION:
		This test will veryify that the state machine can handle a max bytes occuring.
 
	*  TEST STEPS:
		1. call SDP_ServiceSearch 
		2. accept the connection
		3. check request
		4. send results for ss
		5. check pdu.
		6. check request.
		7. send results for sa
		8.  check pdu. Cause max bytes to occur.
		9. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_223(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_224
 
	*  TEST DESCRIPTION:
		This test will veryify that the max duration timer will cause the
		state machine to recover.
 
	*  TEST STEPS:
		1. call SDP_ServiceSearch with timeout of 20sec
		2. accept the connection
		3. check request
		4. send results for ss
		5. check pdu.
		6. check request.
		7. wait for timeout.
		8. check final results.


	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_224(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_225
 
	*  TEST DESCRIPTION:
		Verify that the state machine can handle a max items event.
 
	*  TEST STEPS:
		1. call SDP_ServiceSearch with items limit
		2. accept the connection
		3. check request
		4. send results for ss
		5. hit the limit for items
		6. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_225(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_226
 
	*  TEST DESCRIPTION:
		Test statemachine for a service that is not found.
 
	*  TEST STEPS:
		1. call SDP_ServiceSearch
		2. accept the connection
		3. check request
		4. send results for ss with no results
		5. check final results.


	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_226(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_227
 
	*  TEST DESCRIPTION:
		Test needed ------------------------------------------------------------------------------------
 
	*  TEST STEPS:
		1. call SDP_ServiceSearch 
		2. accept the connection
		3. check request
		4. send results for ss
		5. check pdu.
		6. check request.
		7. wait for timeout.
		8. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_227(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_228
 
	*  TEST DESCRIPTION:
		This test will veryify that the state machine can handle the server not finding 
		an attribute.
 
	*  TEST STEPS:
		1. call SDP_ServiceSearch 
		2. accept the connection
		3. check request
		4. send results for ss
		5. check pdu.
		6. check request.
		7.  send results for sa with no results
		8. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_228(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_229
 
	*  TEST DESCRIPTION:
		This test will veryify that the state machine can handle the server finding 
		some attributes.
 
	*  TEST STEPS:
		1. call SDP_ServiceSearch 
		2. accept the connection
		3. check request
		4. send results for ss
		5. check pdu.
		6. check request.
		7.  send results for sa with some of the results.
		8. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_229(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_230
 
	*  TEST DESCRIPTION:
		This test will veryify that the state machine can handle the server sending back an invalid handle.
 
	*  TEST STEPS:
		1. call SDP_ServiceSearch 
		2. accept the connection
		3. check request
		4. send results error response for invalid handle
		5. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_230(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_231
 
	*  TEST DESCRIPTION:
		This test will veryify that the state machine can handle the server sending back an invalid uuid
 
	*  TEST STEPS:
		1. call SDP_ServiceSearch 
		2. accept the connection
		3. check request
		4. send results error response for invalid uuid
		5. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_231(int p1, void *arg)
{
  /*
    This is a normal ss and sa
   */

  t_SDP_Results* pResults;
  t_SDP_Addresses pDevices;
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_StopRule pRule;
  t_bdaddr address;
  u_int32 aid;
  u_int32 uuids;
  void * connection;
  struct host_buf *buffer;
  int unused;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };

  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = { 0x02,0x00,0x00,0x00,0x0d,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			    0x11,0x03,0x00,0x10,0x00 };

  u_int8 test_results_b[] = { 0x04,0x00,0x01,0x00,0x0c,0x00,0x01,0x00,0x00,0x01,
			      0xd8,0x35,0x03,0x09,0x00,0x03,0x00};

  int ret;

  pDevices.numItems = 1;
  pDevices.addresses = &address; /* it doesn't really matter */

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  ret = HU_FAILED;

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;


  pDebugCheck();

  SDP_ServiceSearch(&pResults,&pDevices,&pPattern,&pAttributes,&pRule,0x00,0x00);

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
		SDP_FreeResults(pResults);
	  return HU_FAILED;
  }
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

	prh_sdp_generate_error_response(buffer->data,SDP_InvalidSyntax,0,0,prh_sdp_read16(buffer->data + 1));

  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);


  prh_stub_l2cap_send_disconnect_confirmation(connection,0);

  if(pResults->status == SDP_InvalidSyntax)
	  ret = HU_PASSED;

  SDP_FreeResults(pResults);

	return ret;	
}

/******
	*  TEST NAME:   
		prh_sdp_unit_232
 
	*  TEST DESCRIPTION:
		This test will veryify that the state machine can handle the server sending back an invalid aid
 
	*  TEST STEPS:
		1. call SDP_ServiceSearch 
		2. accept the connection
		3. check request
		4. send results for ss
		5. check pdu.
		6. check request.
		7.  send error for invalid aid list.
		8. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_232(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_233
 
	*  TEST DESCRIPTION:
		This test will veryify that the state machine can handle the server sending back no resources
 
	*  TEST STEPS:
		1. call SDP_ServiceSearch 
		2. accept the connection
		3. check request
		4. send results for ss
		5. check pdu.
		6. check request.
		7.  send error for no resources
		8. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_233(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_234
 
	*  TEST DESCRIPTION:
		This test will veryify that the state machine can handle the server sending back an error
		for an invalid continue
 
	*  TEST STEPS:
		1. call SDP_ServiceSearch 
		2. accept the connection
		3. check request
		4. send results for ss
		5. check pdu.
		6. check request.
		7.  send results for sa with continue
		8. check pdu.
		9. check request.
		10.  send error for continue
		11. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_234(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_235
 
	*  TEST DESCRIPTION:
		This test will veryify that the state machine can handle the server sending back an error
		for an invalid size to the command
 
	*  TEST STEPS:
		1. call SDP_ServiceSearch 
		2. accept the connection
		3. check request
		4. send error for invalid size
		5. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_235(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_059
 
	*  TEST DESCRIPTION:
		This test will veryify that the state machine can handle the service browse not finding any 
		services.
 
	*  TEST STEPS:
		1. call SDP_ServiceBrowse
		2. accept the connection
		3. check request
		4. send ssa response with no results.
		5. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_059(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_060
 
	*  TEST DESCRIPTION:
		This test will veryify that the state machine can handle the service browse not finding any 
		attributes.
 
	*  TEST STEPS:
		1. call SDP_ServiceBrowse
		2. accept the connection
		3. check request
		4. send ssa response with no results.
		5. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_060(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_064
 
	*  TEST DESCRIPTION:
		This test will send a service search attribute request using an aid range
 
	*  TEST STEPS:
		1. call SDP_ServiceBrowse with aid range
		2. accept the connection
		3. check request
		4. send results for ssa
		5. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_064(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_065
 
	*  TEST DESCRIPTION:
		This test will verify that the state machine can handle a max duration being hit.
 
	*  TEST STEPS:
		1. call SDP_ServiceBrowse
		2. accept the connection
		3. check request
		4. wait for timeout
		5. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_065(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_066
 
	*  TEST DESCRIPTION:
		This test will verify that the state machine can handle a max items.
 
	*  TEST STEPS:
		1. call SDP_ServiceBrowse
		2. accept the connection
		3. check request
		4. send results for ssa, cause max items.
		5. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_066(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_067
 
	*  TEST DESCRIPTION:
		This test will verify that the state machine can handle a max bytes.
 
	*  TEST STEPS:
		1. call SDP_ServiceBrowse
		2. accept the connection
		3. check request
		4. send results for ssa, cause max bytes in the stores results.
		5. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_067(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_236
 
	*  TEST DESCRIPTION:
		This test will verify that the state machine can handle 2 devices and the first disconnects.
 
	*  TEST STEPS:
		1. call SDP_ServiceBrowse.
		2. accept the connection.
		3. check request.
		4. disconnect the first connection.
		5. accept the connection.
		6. check request.
		7. send SSA response.
		8. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_236(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_237
 
	*  TEST DESCRIPTION:
		This test will verify that the state machine can handle 
		the record being found in the second device entry.
 
	*  TEST STEPS:
		1. call SDP_ServiceBrowse.
		2. accept the connection.
		3. check request.
		4. send SSA response. 
		5. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_237(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_238
 
	*  TEST DESCRIPTION:
		This test will verify that the state machine can handle only some of the attributes being
		found.
 
	*  TEST STEPS:
		1. call SDP_ServiceBrowse.
		2. accept the connection.
		3. check request.
		4. send SSA response.
		5. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_238(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_239
 
	*  TEST DESCRIPTION:
		This test will verify that the state machine can handle the responce for an error of
		an incorrect uuid list
 
	*  TEST STEPS:
		1. call SDP_ServiceBrowse.
		2. accept the connection.
		3. check request.
		4. send error response for invalid encoding
		5. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_239(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_240
 
	*  TEST DESCRIPTION:
		This test will verify that the state machine can handle the responce for an error of
		an incorrect aid list
 
	*  TEST STEPS:
		1. call SDP_ServiceBrowse.
		2. accept the connection.
		3. check request.
		4. send error response for invalid encoding
		5. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_240(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_241
 
	*  TEST DESCRIPTION:
		This test will verify that the state machine can handle the responce for an error of
		no resources
 
	*  TEST STEPS:
		1. call SDP_ServiceBrowse.
		2. accept the connection.
		3. check request.
		4. send error response for no resources
		5. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_241(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_242
 
	*  TEST DESCRIPTION:
		This test will verify that the state machine can handle the responce for an error of
		an invalid continue
 
	*  TEST STEPS:
		1. call SDP_ServiceBrowse.
		2. accept the connection.
		3. check request.
		4. send error response for invalid continue
		5. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_242(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_243
 
	*  TEST DESCRIPTION:
		This test will verify that the state machine can handle the responce for an error of
		an invalid size
 
	*  TEST STEPS:
		1. call SDP_ServiceBrowse.
		2. accept the connection.
		3. check request.
		4. send error response for invalid size
		5. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_243(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_244
 
	*  TEST DESCRIPTION:
		This test will verify that the state machine can handle the responce for an error of
		an unsupported
 
	*  TEST STEPS:
		1. call SDP_ServiceBrowse.
		2. accept the connection.
		3. check request.
		4. send error response for  unsupported
		5. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_244(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_245
 
	*  TEST DESCRIPTION:
		This test will verify that the state machine can handle two matching services
 
	*  TEST STEPS:
		1. call SDP_ServiceBrowse.
		2. accept the connection.
		3. check request.
		4. send SSA response for two services.
		5. check final results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_245(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_082
 
	*  TEST DESCRIPTION:
		Test the state machine for attribute range
 
	*  TEST STEPS:
		1. call SDP_GenericBrowse with aid range
		2. fake connection.
		3. send back public browse group info
		4. get browse groups.
		5. get one group info.
		6 disconnect.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_082(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_083
 
	*  TEST DESCRIPTION:
		Test the state machine for max duration
 
	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back public browse group info
		4. wait for timer.
		5. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_083(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_084
 
	*  TEST DESCRIPTION:
		Test the state machine for items case.
 
	*  TEST STEPS:
		1. call SDP_GenericBrowses
		2. fake connection.
		3. send back public browse group info
		4. cause max items to happen
		5. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_084(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_085
 
	*  TEST DESCRIPTION:
		Test the state machine for max bytes
 
	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back public browse group info
		4. cause max bytes to happen
		5. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_085(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_246
 
	*  TEST DESCRIPTION:
		Test the state machine two devices
 
	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back public browse group info
		4. get browse groups.
		5. get one group info.
		6 disconnect.
		7. fake connection.
		8. send back public browse group info
		9. get browse groups.
		10. get one group info.
		11. disconnect.
		12. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_246(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_247
 
	*  TEST DESCRIPTION:
		Test the state machine two devices and the first device disconnects.
 
	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back public browse group info
		4 disconnect from peer.
		5. fake connection.
		6. send back public browse group info
		7. get browse groups.
		8. get one group info.
		9. disconnect.
		10. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_247(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_248
 
	*  TEST DESCRIPTION:
		Test the state machine for a service not being found.

	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back public browse group info with no service info
		4. get browse groups.
		5. get one group info.
		6. disconnect.
		7. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_248(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_249
 
	*  TEST DESCRIPTION:
		Test the state machine for a service found in second entry

	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back public browse service found in second entry
		4. get browse groups.
		5. get one group info.
		6. disconnect.
		7. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_249(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_250
 
	*  TEST DESCRIPTION:
		Test the state machine for no matching attributes.

	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back public browse service found in second entry
		4. get browse groups. 
		5. get one group info. response with no matching attributes.
		6. disconnect.
		7. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_250(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_251
 
	*  TEST DESCRIPTION:
		Test the state machine for some matching attributes.

	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back public browse service found in second entry
		4. get browse groups. 
		5. get one group info. response with some matching attributes.
		6. disconnect.
		7. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_251(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_252
 
	*  TEST DESCRIPTION:
		Test the state machine for a response for an invalid uuid list

	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back error response for invalid uuid list.
		4. disconnect.
		5. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_252(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_253
 
	*  TEST DESCRIPTION:
		Test the state machine for a response for an invalid aid list

	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back error response for invalid aid list.
		4. disconnect.
		5. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_253(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_254
 
	*  TEST DESCRIPTION:
		Test the state machine for a response of no resources

	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back error response for no resources
		4. disconnect.
		5. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_254(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_255
 
	*  TEST DESCRIPTION:
		Test the state machine for a response of invalid continue

	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back error response for invalid continue
		4. disconnect.
		5. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_255(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_256
 
	*  TEST DESCRIPTION:
		Test the state machine for a response of invalid size

	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back error response for invalid size.
		4. disconnect.
		5. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_256(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_257
 
	*  TEST DESCRIPTION:
		Test the state machine for a response of unsupported

	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back error response for unsupported
		4. disconnect.
		5. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_257(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_258
 
	*  TEST DESCRIPTION:
		Test the state machine for a 2 matching services

	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back public browse for 2 matching services.
		4. get browse groups.
		5. get one group info.
		6. disconnect.
		7. check results


	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_258(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_259
 
	*  TEST DESCRIPTION:
		Test the state machine for no public browse info

	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back no public browse info
		4. get browse groups.
		5. get one group info.
		6. disconnect.
		7. check results


	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_259(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_260
 
	*  TEST DESCRIPTION:
		Test the state machine for no attribs public browse info

	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back no public browse info attribs
		4. get browse groups.
		5. get one group info.
		6. disconnect.
		7. check results


	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_260(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_261
 
	*  TEST DESCRIPTION:
		Test the state machine for 2 public browse groups.

	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back 2 public browse info
		4. get browse groups.
		5. get one group info.
		6. disconnect.
		7. check results


	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_261(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_262
 
	*  TEST DESCRIPTION:
		Test the state machine for 2  browse groups.

	*  TEST STEPS:
		1. call SDP_GenericBrowse
		2. fake connection.
		3. send back  public browse info
		4. get browse groups. find 2 groups.
		5. get one group info for first
		6. get one group info for second.
		7. disconnect.
		8. check results


	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_262(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_263
 
	*  TEST DESCRIPTION:
		Test the state machine for inquiry when it hits the max items

	*  TEST STEPS:
		1. call SDP_EnumerateDevices
		2. send back inquiry results till the max 
		3. check results


	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_263(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_264
 
	*  TEST DESCRIPTION:
		Test the Termninate primitive when none exists.

	*  TEST STEPS:
		1. call SDP_TerminatePrimitive with bad reference

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_264(int p1, void *arg)
{
	
  t_SDP_Results* pResults;
  t_SDP_Addresses pDevices;
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_StopRule pRule;
  t_bdaddr address;
  u_int32 aid;
  u_int32 uuids;
  void * connection;
  struct host_buf *buffer;
  int unused;

	SDP_TerminatePrimitive(pResults);  


	return HU_PASSED;


}

/******
	*  TEST NAME:   
		prh_sdp_unit_265
 
	*  TEST DESCRIPTION:
		Test the Termninate primitive for a primitve that is connected.

	*  TEST STEPS:
		1. set up a primitve that had a connection up
		2. call SDP_TerminatePrimitive
		3. examine results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_265(int p1, void *arg)
{
  t_SDP_Results* pResults;
  t_SDP_Addresses pDevices;
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_StopRule pRule;
  t_bdaddr address;
  u_int32 aid;
  u_int32 uuids;
  void * connection;
  struct host_buf *buffer;
  int unused;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };

  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = { 0x02,0x00,0x00,0x00,0x0a,0x35,0x05,0x1a,0x00,0x00,
			    0x11,0x03,0x00,0x10,0x00 };

  u_int8 test_results_b[] = { 0x04,0x00,0x01,0x00,0x0c,0x00,0x01,0x00,0x00,0x01,
			      0xd8,0x35,0x03,0x09,0x00,0x03,0x00};

  pDevices.numItems = 1;
  pDevices.addresses = &address; /* it doesn't really matter */

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;


  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;


  pDebugCheck();

  SDP_ServiceSearch(&pResults,&pDevices,&pPattern,&pAttributes,&pRule,0x00,0x00);

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
 	
	prh_stub_l2cap_do_disconnectind(connection); /* remove this when the test is working */

	SDP_TerminatePrimitive(pResults);  

	SDP_FreeResults(pResults);

	return HU_PASSED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_266
 
	*  TEST DESCRIPTION:
		Test the Termninate primitive when a primitive is disconnecting.

	*  TEST STEPS:
		1. setup a primitive that is in the process of disconnecting.
		2. call SDP_TerminatePrimitive with bad reference
		3. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_266(int p1, void *arg)
{
  /*
    This is a normal browse
   */

  t_SDP_Results* pResults;
  t_SDP_Addresses pDevices;
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_StopRule pRule;
  t_bdaddr address;
  u_int32 aid;
  u_int32 uuids;
  void * connection;
  struct host_buf *buffer;
  int unused;

  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x03,0x00,0x15,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x01,0xd8,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

	
  pDevices.numItems = 1;
  pDevices.addresses = &address; /* it doesn't really matter */

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;


  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pRule.maxItems = 16;
  pRule.maxDuration = 10;
  pRule.maxBytes = 500;

  SDP_ServiceBrowse(&pResults,&pDevices,&pPattern,&pAttributes,&pRule,0x00,0x00);

  prh_stub_l2cap_accept_l2_connection(&connection,1);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  /*
    read the response
   */
  
  /*
    send back a response, I will use the same buffer
  */

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  	  SDP_FreeResults(pResults);

    return HU_FAILED;
  }
  prh_sdp_generate_service_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  SDP_TerminatePrimitive(pResults);  

  prh_stub_l2cap_send_disconnect_confirmation(connection,0); /* remove this when the test is correct */


  if(pResults->element.dataLen == sizeof(data))
  {
      if(!memcmp(pResults->element.pData,data,sizeof(data)))
	  {
		  	  SDP_FreeResults(pResults);

		  return HU_PASSED;
	  } else {
		  	  SDP_FreeResults(pResults);

		  return HU_FAILED;
	  }
  } else {
	  	  SDP_FreeResults(pResults);

      return HU_FAILED;
  }  
}





/******
*  TEST NAME:   
	prh_sdp_unit_108

	*  TEST DESCRIPTION:
		Test the SDP_EnableCache for the normal

	*  TEST STEPS:
		1. Call SDP_DisconnectTransport 
		2. does it return unsupported.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_314(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
*  TEST NAME:   
	prh_sdp_unit_108

	*  TEST DESCRIPTION:
		Test the SDP_EnableCache for the normal

	*  TEST STEPS:
		1. Call SDP_EnableCache
		2. does it return unsupported.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_108(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
*  TEST NAME:   
	prh_sdp_unit_110

	*  TEST DESCRIPTION:
		Test the SDP_DisableCache for the normal

	*  TEST STEPS:
		1. Call SDP_DisableCache 
		2. does it return unsupported.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_110(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
*  TEST NAME:   
	prh_sdp_unit_374

	*  TEST DESCRIPTION:
		Test the SDP_Enumerate for an error response

	*  TEST STEPS:
		1. Call SDP_Enumerate
		2. inject inquiry result with error

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_374(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_339

	*  TEST DESCRIPTION:
		Test the ServiceBrowse with max bytes in the w4_service_attributes state

	*  TEST STEPS:
		1. Call SDP_ServiceBrowse
		2. place in w4_service_attributes state
		2. cause max bytes
		3. check result

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_339(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_348

	*  TEST DESCRIPTION:
		Test the ServiceBrowse with error in the create connection state

	*  TEST STEPS:
		1. Call SDP_ServiceBrowse
		2. place in create connection state
		2. inject error response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_348(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_349

	*  TEST DESCRIPTION:
		Test the ServiceBrowse with error in the w4_browse connection state

	*  TEST STEPS:
		1. Call SDP_ServiceBrowse
		2. place in w4_browse_connection state
		2. inject error response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_349(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_350

	*  TEST DESCRIPTION:
		Test the ServiceBrowse with error in the find services attributes state

	*  TEST STEPS:
		1. Call SDP_ServiceBrowse
		2. place in find services attributes state
		2. inject error response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_350(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_351

	*  TEST DESCRIPTION:
		Test the ServiceBrowse with service response in the w4_service_attributes  state

	*  TEST STEPS:
		1. Call SDP_ServiceBrowse
		2. place in w4_service_attributes state
		2. inject service response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_351(int p1, void *arg)
{
	return HU_UNTESTED;
}




/******
	*  TEST NAME:   
		prh_sdp_unit_352

	*  TEST DESCRIPTION:
		Test the ServiceBrowse with error in the find store results state

	*  TEST STEPS:
		1. Call SDP_ServiceBrowse
		2. place in find store results state
		2. inject error response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_352(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_353

	*  TEST DESCRIPTION:
		Test the ServiceBrowse with error in the find w4_disconnect state

	*  TEST STEPS:
		1. Call SDP_ServiceBrowse
		2. place in find w4_disconnect state
		2. inject error response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_353(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_337

	*  TEST DESCRIPTION:
		Test the ServiceSearch with max bytes in the w4_service state

	*  TEST STEPS:
		1. Call SDP_ServiceSearch
		2. place in w4_service state
		2. cause max bytes
		3. check result

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_337(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_338

	*  TEST DESCRIPTION:
		Test the ServiceSearch with max bytes in the store results state

	*  TEST STEPS:
		1. Call SDP_ServiceSearch
		2. place in store results state
		2. cause max bytes
		3. check result

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_338(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_378

	*  TEST DESCRIPTION:
		Test the ServiceSearch with error in main state state

	*  TEST STEPS:
		1. Call SDP_ServiceSearch
		2. place in main state
		2. inject error response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_378(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_340

	*  TEST DESCRIPTION:
		Test the ServiceSearch with error in connecting state

	*  TEST STEPS:
		1. Call SDP_ServiceSearch
		2. place in connecting state
		2. inject error response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_340(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_341

	*  TEST DESCRIPTION:
		Test the ServiceSearch with error in w4_connection state

	*  TEST STEPS:
		1. Call SDP_ServiceSearch
		2. place in w4_connection state
		2. inject error response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_341(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_342

	*  TEST DESCRIPTION:
		Test the ServiceSearch with error in find services state

	*  TEST STEPS:
		1. Call SDP_ServiceSearch
		2. place in find services state
		2. inject error response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_342(int p1, void *arg)
{
	return HU_UNTESTED;
}




/******
	*  TEST NAME:   
		prh_sdp_unit_343

	*  TEST DESCRIPTION:
		Test the ServiceSearch with attribute in w4_services state

	*  TEST STEPS:
		1. Call SDP_ServiceSearch
		2. place in w4_services state
		2. inject attribute response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_343(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_344

	*  TEST DESCRIPTION:
		Test the ServiceSearch with error in found services state

	*  TEST STEPS:
		1. Call SDP_ServiceSearch
		2. place in found services state
		2. inject error response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_344(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_345

	*  TEST DESCRIPTION:
		Test the ServiceSearch with error in find attributes state

	*  TEST STEPS:
		1. Call SDP_ServiceSearch
		2. place in find attributes state
		2. inject error response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_345(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_346

	*  TEST DESCRIPTION:
		Test the ServiceSearch with service in w4_attributes state

	*  TEST STEPS:
		1. Call SDP_ServiceSearch
		2. place in w4_attributes state
		2. inject service response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_346(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_347

	*  TEST DESCRIPTION:
		Test the ServiceSearch with service in store results state

	*  TEST STEPS:
		1. Call SDP_ServiceSearch
		2. place in store results state
		2. inject error response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_347(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_375

	*  TEST DESCRIPTION:
		Test the ServiceSearch with 2 devices and the first times out

	*  TEST STEPS:
		1. Call SDP_ServiceSearch
		2. accept connection
		3. check request
		4. wait for time out
		5. check results

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_375(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_376

	*  TEST DESCRIPTION:
		Test the ServiceBrowse with 2 devices and the first times out

	*  TEST STEPS:
		1. Call SDP_ServiceBrowse
		2. accept connection
		3. check request
		4. wait for time out
		5. check results

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_376(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_377

	*  TEST DESCRIPTION:
		Test the GenericBrowse with 2 devices and the first times out

	*  TEST STEPS:
		1. Call SDP_GenericBrowse
		2. accept connection
		3. check request
		4. wait for time out
		5. check results

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_377(int p1, void *arg)
{
	return HU_UNTESTED;
}




/******
	*  TEST NAME:   
		prh_sdp_unit_379

	*  TEST DESCRIPTION:
		Test the GenericBrowse with error in the create connection

	*  TEST STEPS:
		1. Call SDP_GenericBrowse
		2. create connection returns and error
		3. check results

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_379(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_380

	*  TEST DESCRIPTION:
		Test the GenericBrowse with connection failure

	*  TEST STEPS:
		1. Call SDP_GenericBrowse
		2. connection failure
		3. check results

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_380(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_381

	*  TEST DESCRIPTION:
		Test the GenericBrowse with error sending public

	*  TEST STEPS:
		1. Call SDP_GenericBrowse
		2. error in sending public browse request
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_381(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_382

	*  TEST DESCRIPTION:
		Test the GenericBrowse with error in w4_public_browse_group

	*  TEST STEPS:
		1. Call SDP_GenericBrowse
		2. error response for public browse.
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_382(int p1, void *arg)
{
	return HU_UNTESTED;
}




/******
	*  TEST NAME:   
		prh_sdp_unit_383

	*  TEST DESCRIPTION:
		Test the GenericBrowse with error group descriptor

	*  TEST STEPS:
		1. Call SDP_GenericBrowse
		2. error in sending group descriptor request
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_383(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_384

	*  TEST DESCRIPTION:
		Test the GenericBrowse with error in w4_browse_group_descriptor

	*  TEST STEPS:
		1. Call SDP_GenericBrowse
		2. error response for browse group descriptor.
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_384(int p1, void *arg)
{
	return HU_UNTESTED;
}




/******
	*  TEST NAME:   
		prh_sdp_unit_385

	*  TEST DESCRIPTION:
		Test the GenericBrowse with specific group

	*  TEST STEPS:
		1. Call SDP_GenericBrowse
		2. error in sending specific group request
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_385(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_386

	*  TEST DESCRIPTION:
		Test the GenericBrowse with error in w4_group_attributes

	*  TEST STEPS:
		1. Call SDP_GenericBrowse
		2. error response for specific group
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_386(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_364

	*  TEST DESCRIPTION:
		Test the GenericBrowse with error in main  state

	*  TEST STEPS:
		1. Call SDP_GenericBrowse
		2. place in main state
		2. inject error response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_364(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_365

	*  TEST DESCRIPTION:
		Test the GenericBrowse with error in create connection  state

	*  TEST STEPS:
		1. Call SDP_GenericBrowse
		2. place in create connection state
		2. inject error response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_365(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_366

	*  TEST DESCRIPTION:
		Test the GenericBrowse with error in  w4_connection  state

	*  TEST STEPS:
		1. Call SDP_GenericBrowse
		2. place in w4_connection state
		2. inject error response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_366(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_367

	*  TEST DESCRIPTION:
		Test the GenericBrowse with error in public browse  state

	*  TEST STEPS:
		1. Call SDP_GenericBrowse
		2. place in public browse state
		2. inject error response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_367(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_368

	*  TEST DESCRIPTION:
		Test the GenericBrowse with service in w4_public_browse_group state

	*  TEST STEPS:
		1. Call SDP_GenericBrowse
		2. place in w4_public_browse_group state
		2. inject service response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_368(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_369

	*  TEST DESCRIPTION:
		Test the GenericBrowse with error in browse descriptor state

	*  TEST STEPS:
		1. Call SDP_GenericBrowse
		2. place in browse descriptor state
		2. inject error response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_369(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_370

	*  TEST DESCRIPTION:
		Test the GenericBrowse with service in w4_browse_group_descriptor state

	*  TEST STEPS:
		1. Call SDP_GenericBrowse
		2. place in w4_browse_group_descriptor state
		2. inject service response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_370(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_371

	*  TEST DESCRIPTION:
		Test the GenericBrowse with error in group attributes state

	*  TEST STEPS:
		1. Call SDP_GenericBrowse
		2. place in group attributes state
		2. inject error response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_371(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_372

	*  TEST DESCRIPTION:
		Test the GenericBrowse with service in w4_group_attributes state

	*  TEST STEPS:
		1. Call SDP_GenericBrowse
		2. place in w4_group_attributes state
		2. inject service response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_372(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_373

	*  TEST DESCRIPTION:
		Test the GenericBrowse with service in store results state

	*  TEST STEPS:
		1. Call SDP_GenericBrowse
		2. place in store results state
		2. inject error response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_373(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_278
 
	*  TEST DESCRIPTION:
		Test the state machine for a response of unsupported

	*  TEST STEPS:
		1. call SDP_ServiceSearch
		2. fake connection.
		3. send back error response for unsupported
		4. disconnect.
		5. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_278(int p1, void *arg)
{
	return HU_UNTESTED;
}

/*
	------------------------------------------------------------------
*/

prh_t_hu_testinfo prh_sdp_sdap_tests [] =  
{
    { "TP_SDP_V_001_U",0x00,prh_sdp_unit_001, NULL, 0 },
	{ "TP_SDP_V_002_U",0x00,prh_sdp_unit_002, NULL, 0 },
   	{ "TP_SDP_V_003_U",0x00,prh_sdp_unit_003, NULL, 0 },
   	{ "TP_SDP_V_004_U",0x00,prh_sdp_unit_004, NULL, 0 },
	{ "TP_SDP_V_005_U",0x00,prh_sdp_unit_005, NULL, 0 },
/*	{ "TP_SDP_V_006_U",0x00,prh_sdp_unit_006, NULL, 0 },*/
	{ "TP_SDP_V_007_U",0x00,prh_sdp_unit_007, NULL, 0 },
    { "TP_SDP_V_008_U",0x00,prh_sdp_unit_008, NULL, 0 },
    { "TP_SDP_V_009_U",0x00,prh_sdp_unit_009, NULL, 0 },
    { "TP_SDP_V_033_U",0x00,prh_sdp_unit_033, NULL, 0 },
	{ "TP_SDP_V_073_U",0x00,prh_sdp_unit_073, NULL, 0 },
#ifndef P7D_PT_RELEASE_TESTS
	{ "TP_SDP_V_074_U",0x00,prh_sdp_unit_074, NULL, 0 },
	{ "TP_SDP_V_075_U",0x00,prh_sdp_unit_075, NULL, 0 },
/*	{ "TP_SDP_V_076_U",0x00,prh_sdp_unit_076, NULL, 0 },
	{ "TP_SDP_V_077_U",0x00,prh_sdp_unit_077, NULL, 0 },
	{ "TP_SDP_V_078_U",0x00,prh_sdp_unit_078, NULL, 0 },*/
	{ "TP_SDP_V_079_U",0x00,prh_sdp_unit_079, NULL, 0 },
#endif
/*	{ "TP_SDP_V_080_U",0x00,prh_sdp_unit_080, NULL, 0 },*/
	{ "TP_SDP_V_091_U",0x00,prh_sdp_unit_091, NULL, 0 },
/*	{ "TP_SDP_V_092_U",0x00,prh_sdp_unit_092, NULL, 0 },
	{ "TP_SDP_V_093_U",0x00,prh_sdp_unit_093, NULL, 0 },
	{ "TP_SDP_V_094_U",0x00,prh_sdp_unit_094, NULL, 0 },*/
#ifndef P7D_PT_RELEASE_TESTS
	{ "TP_SDP_V_095_U",0x00,prh_sdp_unit_095, NULL, 0 },
#endif
	{ "TP_SDP_V_099_U",0x00,prh_sdp_unit_099, NULL, 0 },
	{ "TP_SDP_V_103_U",0x00,prh_sdp_unit_103, NULL, 0 },
	{ "TP_SDP_V_106_U",0x00,prh_sdp_unit_106, NULL, 0 },
#ifndef P7D_PT_RELEASE_TESTS
	{ "TP_SDP_V_145_U",0x00,prh_sdp_unit_145, NULL, 0 },
#endif
	{ "TP_SDP_V_220_U",0x00,prh_sdp_unit_220, NULL, 0 },
#ifndef P7D_PT_RELEASE_TESTS
	{ "TP_SDP_V_221_U",0x00,prh_sdp_unit_221, NULL, 0 },
	{ "TP_SDP_V_222_U",0x00,prh_sdp_unit_222, NULL, 0 },
	{ "TP_SDP_V_223_U",0x00,prh_sdp_unit_223, NULL, 0 },
	{ "TP_SDP_V_224_U",0x00,prh_sdp_unit_224, NULL, 0 },
	{ "TP_SDP_V_225_U",0x00,prh_sdp_unit_225, NULL, 0 },
	{ "TP_SDP_V_226_U",0x00,prh_sdp_unit_226, NULL, 0 },
	{ "TP_SDP_V_227_U",0x00,prh_sdp_unit_227, NULL, 0 },
	{ "TP_SDP_V_228_U",0x00,prh_sdp_unit_228, NULL, 0 },
	{ "TP_SDP_V_229_U",0x00,prh_sdp_unit_229, NULL, 0 },
	{ "TP_SDP_V_230_U",0x00,prh_sdp_unit_230, NULL, 0 },
#endif
	{ "TP_SDP_V_231_U",0x00,prh_sdp_unit_231, NULL, 0 },
#ifndef P7D_PT_RELEASE_TESTS
	{ "TP_SDP_V_232_U",0x00,prh_sdp_unit_232, NULL, 0 },
	{ "TP_SDP_V_233_U",0x00,prh_sdp_unit_233, NULL, 0 },
	{ "TP_SDP_V_234_U",0x00,prh_sdp_unit_234, NULL, 0 },
	{ "TP_SDP_V_059_U",0x00,prh_sdp_unit_059, NULL, 0 },
	{ "TP_SDP_V_060_U",0x00,prh_sdp_unit_060, NULL, 0 },
	{ "TP_SDP_V_064_U",0x00,prh_sdp_unit_064, NULL, 0 },
	{ "TP_SDP_V_065_U",0x00,prh_sdp_unit_065, NULL, 0 },
	{ "TP_SDP_V_066_U",0x00,prh_sdp_unit_066, NULL, 0 },
	{ "TP_SDP_V_067_U",0x00,prh_sdp_unit_067, NULL, 0 },
	{ "TP_SDP_V_236_U",0x00,prh_sdp_unit_236, NULL, 0 },
	{ "TP_SDP_V_237_U",0x00,prh_sdp_unit_237, NULL, 0 },
	{ "TP_SDP_V_238_U",0x00,prh_sdp_unit_238, NULL, 0 },
	{ "TP_SDP_V_239_U",0x00,prh_sdp_unit_239, NULL, 0 },
	{ "TP_SDP_V_240_U",0x00,prh_sdp_unit_240, NULL, 0 },
	{ "TP_SDP_V_241_U",0x00,prh_sdp_unit_241, NULL, 0 },
	{ "TP_SDP_V_242_U",0x00,prh_sdp_unit_242, NULL, 0 },
	{ "TP_SDP_V_243_U",0x00,prh_sdp_unit_243, NULL, 0 },
	{ "TP_SDP_V_244_U",0x00,prh_sdp_unit_244, NULL, 0 },
	{ "TP_SDP_V_245_U",0x00,prh_sdp_unit_245, NULL, 0 },
	{ "TP_SDP_V_082_U",0x00,prh_sdp_unit_082, NULL, 0 },
	{ "TP_SDP_V_083_U",0x00,prh_sdp_unit_083, NULL, 0 },
	{ "TP_SDP_V_084_U",0x00,prh_sdp_unit_084, NULL, 0 },
	{ "TP_SDP_V_085_U",0x00,prh_sdp_unit_085, NULL, 0 },
	{ "TP_SDP_V_246_U",0x00,prh_sdp_unit_246, NULL, 0 },
	{ "TP_SDP_V_247_U",0x00,prh_sdp_unit_247, NULL, 0 },
	{ "TP_SDP_V_248_U",0x00,prh_sdp_unit_248, NULL, 0 },
	{ "TP_SDP_V_249_U",0x00,prh_sdp_unit_249, NULL, 0 },
	{ "TP_SDP_V_250_U",0x00,prh_sdp_unit_250, NULL, 0 },
	{ "TP_SDP_V_251_U",0x00,prh_sdp_unit_251, NULL, 0 },
	{ "TP_SDP_V_252_U",0x00,prh_sdp_unit_252, NULL, 0 },
	{ "TP_SDP_V_253_U",0x00,prh_sdp_unit_253, NULL, 0 },
	{ "TP_SDP_V_254_U",0x00,prh_sdp_unit_254, NULL, 0 },
	{ "TP_SDP_V_255_U",0x00,prh_sdp_unit_255, NULL, 0 },
	{ "TP_SDP_V_256_U",0x00,prh_sdp_unit_256, NULL, 0 },
	{ "TP_SDP_V_257_U",0x00,prh_sdp_unit_257, NULL, 0 },
	{ "TP_SDP_V_258_U",0x00,prh_sdp_unit_258, NULL, 0 },
	{ "TP_SDP_V_259_U",0x00,prh_sdp_unit_259, NULL, 0 },
	{ "TP_SDP_V_260_U",0x00,prh_sdp_unit_260, NULL, 0 },
	{ "TP_SDP_V_261_U",0x00,prh_sdp_unit_261, NULL, 0 },
	{ "TP_SDP_V_262_U",0x00,prh_sdp_unit_262, NULL, 0 },
	{ "TP_SDP_V_263_U",0x00,prh_sdp_unit_263, NULL, 0 },
#endif
   	{ "TP_SDP_V_264_U",0x00,prh_sdp_unit_264, NULL, 0 },
	{ "TP_SDP_V_265_U",0x00,prh_sdp_unit_265, NULL, 0 },
	{ "TP_SDP_V_266_U",0x00,prh_sdp_unit_266, NULL, 0 },
#ifndef P7D_PT_RELEASE_TESTS
	{ "TP_SDP_V_314_U",0x00,prh_sdp_unit_314, NULL, 0 },
	{ "TP_SDP_V_339_U",0x00,prh_sdp_unit_339, NULL, 0 },
	{ "TP_SDP_V_340_U",0x00,prh_sdp_unit_340, NULL, 0 },
	{ "TP_SDP_V_341_U",0x00,prh_sdp_unit_341, NULL, 0 },
	{ "TP_SDP_V_342_U",0x00,prh_sdp_unit_342, NULL, 0 },
	{ "TP_SDP_V_343_U",0x00,prh_sdp_unit_343, NULL, 0 },
	{ "TP_SDP_V_344_U",0x00,prh_sdp_unit_344, NULL, 0 },
	{ "TP_SDP_V_345_U",0x00,prh_sdp_unit_345, NULL, 0 },
	{ "TP_SDP_V_346_U",0x00,prh_sdp_unit_346, NULL, 0 },
	{ "TP_SDP_V_347_U",0x00,prh_sdp_unit_347, NULL, 0 },
	{ "TP_SDP_V_348_U",0x00,prh_sdp_unit_348, NULL, 0 },
	{ "TP_SDP_V_349_U",0x00,prh_sdp_unit_349, NULL, 0 },
	{ "TP_SDP_V_364_U",0x00,prh_sdp_unit_364, NULL, 0 },
	{ "TP_SDP_V_365_U",0x00,prh_sdp_unit_365, NULL, 0 },
	{ "TP_SDP_V_366_U",0x00,prh_sdp_unit_366, NULL, 0 },
	{ "TP_SDP_V_367_U",0x00,prh_sdp_unit_367, NULL, 0 },
	{ "TP_SDP_V_368_U",0x00,prh_sdp_unit_368, NULL, 0 },
	{ "TP_SDP_V_369_U",0x00,prh_sdp_unit_369, NULL, 0 },
	{ "TP_SDP_V_370_U",0x00,prh_sdp_unit_370, NULL, 0 },
	{ "TP_SDP_V_371_U",0x00,prh_sdp_unit_371, NULL, 0 },
	{ "TP_SDP_V_372_U",0x00,prh_sdp_unit_372, NULL, 0 },
	{ "TP_SDP_V_373_U",0x00,prh_sdp_unit_373, NULL, 0 },
	{ "TP_SDP_V_374_U",0x00,prh_sdp_unit_374, NULL, 0 },
	{ "TP_SDP_V_375_U",0x00,prh_sdp_unit_375, NULL, 0 },
	{ "TP_SDP_V_376_U",0x00,prh_sdp_unit_376, NULL, 0 },
	{ "TP_SDP_V_377_U",0x00,prh_sdp_unit_377, NULL, 0 },
	{ "TP_SDP_V_378_U",0x00,prh_sdp_unit_378, NULL, 0 },
	{ "TP_SDP_V_379_U",0x00,prh_sdp_unit_379, NULL, 0 },
	{ "TP_SDP_V_380_U",0x00,prh_sdp_unit_380, NULL, 0 },
	{ "TP_SDP_V_381_U",0x00,prh_sdp_unit_381, NULL, 0 },
	{ "TP_SDP_V_382_U",0x00,prh_sdp_unit_382, NULL, 0 },
	{ "TP_SDP_V_383_U",0x00,prh_sdp_unit_383, NULL, 0 },
	{ "TP_SDP_V_384_U",0x00,prh_sdp_unit_384, NULL, 0 },
	{ "TP_SDP_V_385_U",0x00,prh_sdp_unit_385, NULL, 0 },
	{ "TP_SDP_V_386_U",0x00,prh_sdp_unit_386, NULL, 0 },
	{ "TP_SDP_V_108_U",0x00,prh_sdp_unit_108, NULL, 0 },
	{ "TP_SDP_V_110_U",0x00,prh_sdp_unit_110, NULL, 0 },
	{ "TP_SDP_V_350_U",0x00,prh_sdp_unit_350, NULL, 0 },
	{ "TP_SDP_V_351_U",0x00,prh_sdp_unit_351, NULL, 0 },
	{ "TP_SDP_V_352_U",0x00,prh_sdp_unit_352, NULL, 0 },
	{ "TP_SDP_V_353_U",0x00,prh_sdp_unit_353, NULL, 0 },
	{ "TP_SDP_V_337_U",0x00,prh_sdp_unit_337, NULL, 0 },
	{ "TP_SDP_V_338_U",0x00,prh_sdp_unit_338, NULL, 0 },
	{ "TP_SDP_V_235_U",0x00,prh_sdp_unit_235, NULL, 0 },
#endif
};



prh_t_hu_harness prh_sdp_sdap_group =
{ NULL, HU_SDP, "SDP_SDAP",prh_sdp_unit_init,PRH_UT_STUBS_ENABLED, 1,sizeof(prh_sdp_sdap_tests)/sizeof(prh_t_hu_testinfo), prh_sdp_sdap_tests, 0, 0, 0, 0, 0};



void test_sdp_sdap(void)
{
	prh_hu_add_testgroup(&prh_sdp_sdap_group);
}

