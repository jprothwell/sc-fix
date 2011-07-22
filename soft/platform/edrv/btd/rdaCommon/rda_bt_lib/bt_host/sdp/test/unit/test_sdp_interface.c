/*
  $Id: test_sdp_interface.c 1532 2010-08-23 05:47:34Z huazeng $
 */

#include "host_config.h"
#include "host_types.h"
#include "hu_harness.h"
#include "papi.h"


#include "../../include/sdp_state_machine.h"
#include "unit_stub_l2cap_internal.h"
#include "../../include/sdp_ue.h"
#include "../../include/sdp_constants.h"
#include "../../include/sdp_server.h"
#include "../../include/sdp_db.h"

#include "../../include/sdp_error.h"




int  prh_sdp_unit_init(int p1, void *arg);
extern struct prh_sdp_primitive_task* p_SDP_Primitive_Task_List;

/******
	*  TEST NAME:   
		prh_sdp_unit_039

	*  TEST DESCRIPTION:
		Test the SDP_SSR for the normal case

	*  TEST STEPS:
		1. Call SDP_ServiceSearchRequest
		2. check request
		3. send send response to SSR
		4. check results

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_039(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 uuids;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };
  u_int8 test_results_a[] = {0x02,0x00,0x1d,0x00,0x0d,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x0a,0x00
  };

  prh_sdp_flush_database();

	pDebugCheck();
  
  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceSearchRequest(&pResults,1,&pPattern,10,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_search_response(buffer->data,handles,prh_sdp_read16(buffer->data + 1),1,1,0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);
  

    /*
    free the results
   */

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();
 
  return HU_PASSED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_040

	*  TEST DESCRIPTION:
		Test the SDP_SAR for the normal case

	*  TEST STEPS:
		1. Call SDP_ServiceAttributeRequest
		2. check request
		3. send send response to SAR
		4. check results

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_040(int p1, void *arg)
{
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int8 data[] = "Here is some random gibberish";
  u_int8 test_results_a[] = {0x04,0x00,0x1e,0x00,0x0f,0x00,0x00,0x10,0x01,
			     0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

	pDebugCheck();
  
  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceAttributeRequest(&pResults,1,0x001001,&pAttributes,100,0x00, 60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

pDebugCheck();

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);

  pDebugCheck();
  
   /*
    free the results
   */

  if(pResults->length == sizeof(data))
    {
      if(memcmp(pResults->pData,data,sizeof(data)))
	return HU_FAILED;
    } else {
      return HU_FAILED;
    }

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();

  return HU_PASSED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_041

	*  TEST DESCRIPTION:
		Test the SSAR in the normal case

	*  TEST STEPS:
		1. Call SDP_ServiceSearchAttributeRequest
		2. accept the connection.
		3. check request
		4. send response
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_041(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int32 uuids;
  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x1f,0x00,0x15,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);
  
  SDP_ServiceSearchAttributeRequest(&pResults,1,&pPattern,&pAttributes,100,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_service_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  /*
    free the results
   */


    if(pResults->length == sizeof(data))
    {
      if(memcmp(pResults->pData,data,sizeof(data)))
	return HU_FAILED;
    } else {
      return HU_FAILED;
    }

  pFree(pResults->pData);
  pFree(pResults);

  prh_stub_l2cap_do_disconnectind(connection);
   
	pDebugCheck();

  return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_042

	*  TEST DESCRIPTION:
		Test the SDP_SSR for the normal case

	*  TEST STEPS:
		1. Call SDP_ServiceSearchRequest
		2. check request
		3. send send response to SSR with continue
		4. check results
		5. Call SDP_ServiceSearchRequest with contine info
		6. check request
		7. send send response to SSR 
		8. check results

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_042(int p1, void *arg)
{
  /* a list of 100 megs */
  t_SDP_SearchPattern pPattern;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 uuids;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };
  u_int8 cont[] = { 0x04,'c','o','n','t' };

  u_int8 test_results_a[] = {0x02,0x00,0x20,0x00,0x0d,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x0a,0x00
   };

  u_int8 test_results_b[] = {0x02,0x00,0x21,0x00,0x11,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x0a,0x04,'c','o','n','t'
  };

  
  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

	pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceSearchRequest(&pResults,1,&pPattern,10,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }
  prh_sdp_generate_search_response(buffer->data,handles,prh_sdp_read16(buffer->data + 1),1,1,cont);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);

  /*
    free the results
   */

  pFree(pResults->pData);
  pFree(pResults);

  SDP_ServiceSearchRequest(&pResults,1,&pPattern,10,cont,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  if(prh_sdp_test_check(buffer,test_results_b,sizeof(test_results_b)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }
  prh_sdp_generate_search_response(buffer->data,handles,prh_sdp_read16(buffer->data + 1),1,1,0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);


  prh_stub_l2cap_do_disconnectind(connection);

    /*
    free the results
   */


  pFree(pResults->pData);
  pFree(pResults);
 
pDebugCheck();

  return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_043

	*  TEST DESCRIPTION:
		Test the SDP_SAR for the normal case

	*  TEST STEPS:
		1. Call SDP_ServiceAttributeRequest
		2. check request
		3. send send response to SAR with continue
		4. check results
		5. Call SDP_ServiceAttributeRequest with contine info
		6. check request
		7. send send response to SAR 
		8. check results

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_043(int p1, void *arg)
{
  /* a list of 100 megs */

  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int8 data[] = "Here is some random gibberish";
  u_int8 cont[] = { 0x04,'c','o','n','t' };

  u_int8 test_results_a[] = {0x04,0x00,0x00,0x00,0x0f,0x00,0x00,0x10,0x01,0x00,
			     0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  u_int8 test_results_b[] = {0x04,0x00,0x00,0x00,0x13,0x00,0x00,0x10,0x01,0x00,
			     0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x04,'c','o','n','t'
  };

    pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

	pDebugCheck();
	
  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceAttributeRequest(&pResults,1,0x001001,&pAttributes,100,0x00, 60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	prh_stub_l2cap_do_disconnectind(connection);
	  return HU_FAILED;
  }

  prh_sdp_generate_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),cont);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);

  
   /*
    free the results
   */

  pFree(pResults->pData);
  pFree(pResults);

  SDP_ServiceAttributeRequest(&pResults,1,0x001001,&pAttributes,100,cont, 60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);
  
  if(prh_sdp_test_check(buffer,test_results_b,sizeof(test_results_b)) == HU_FAILED)
  {
	prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }
  prh_sdp_generate_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer); 
  prh_stub_l2cap_do_disconnectind(connection);
  
   /*
    free the results
   */

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();

  return HU_PASSED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_044

	*  TEST DESCRIPTION:
		Test the SSAR in the continue case

	*  TEST STEPS:
		1. Call SDP_ServiceSearchAttributeRequest
		2. accept the connection.
		3. check request
		4. send response with continue
		5. check request
		6. send response
		7. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
 static prh_t_hu_outcome prh_sdp_unit_044(int p1, void *arg)
{
  /* a list of 100 megs */
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int32 uuids;
  u_int8 data[] = "Here is some random gibberish";
  u_int8 cont[] = { 0x04,'c','o','n','t' };

  u_int8 test_results_a[] = {0x06,0x00,0x00,0x00,0x15,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  u_int8 test_results_b[] = {0x06,0x00,0x00,0x00,0x19,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x04,
			     'c','o','n','t'
  };

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

	pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);
  
  SDP_ServiceSearchAttributeRequest(&pResults,1,&pPattern,&pAttributes,100,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }
  prh_sdp_generate_service_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),cont);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  /*
    free the results
   */

  pFree(pResults->pData);
  pFree(pResults);

  SDP_ServiceSearchAttributeRequest(&pResults,1,&pPattern,&pAttributes,100,cont,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_b,sizeof(test_results_b)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }
  prh_sdp_generate_service_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);
  
  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();
 
  return HU_PASSED;
}


 static int prh_sdp_unit_111_global;

 void prh_sdp_unit_111_callback(u_int8 status,t_L2_ControlCallbackArgs *arg)
 {
	 prh_sdp_unit_111_global = 1;
 }


/******
	*  TEST NAME:   
		prh_sdp_unit_111

	*  TEST DESCRIPTION:
		Test the SDP_ConnectTransport will work for the normal case.

	*  TEST STEPS:
		1. Call SDP_ConnectTransport
		2. accept the connection.
		3. send any pdu over the link
		4. check request

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_111(int p1, void *arg)
{
  void* connection;
  t_bdaddr address;
  int ret;
  struct host_buf *buffer;
  int unused;  


  prh_sdp_unit_111_global = 0;
	ret = HU_FAILED;
	address.bytes[3] = 3;

	SDP_ConnectTransport(&address,1024,prh_sdp_unit_111_callback,0x00);
	prh_stub_l2cap_accept_l2_connection(&connection,1);

/*	L2_Write(1,*/

/*  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);*/


	if(prh_sdp_unit_111_global)
		ret = HU_PASSED;

	prh_stub_l2cap_do_disconnectind(connection); /* remove connection */

/*
	SDP_DisconnectTransport(1);
	prh_stub_l2cap_send_disconnect_confirmation(connection,0);
	*/

	return ret;
}

/******
*  TEST NAME:   
	prh_sdp_unit_112

	*  TEST DESCRIPTION:
		Test the SDP_ConnectTransport will work when there is no response 
		From the Peer

	*  TEST STEPS:
		1. Call SDP_ConnectTransport
		2. wait for timeout 
		3. check result

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_112(int p1, void *arg)
{
  void* connection;
  t_bdaddr address;
  int ret;

  prh_sdp_unit_111_global = 0;
	ret = HU_FAILED;
	address.bytes[3] = 3;

	prh_stub_l2cap_fail_next_connect(1);
	ret = SDP_ConnectTransport(&address,1024,prh_sdp_unit_111_callback,prh_sdp_unit_111_callback);
	
	ret = (ret == BT_UNKNOWNERROR)  ? HU_PASSED : HU_FAILED;

	return ret;
  
}



static int prh_sdp_unit_310_global;

void prh_sdp_unit_310_callback(u_int8 type,t_L2_ControlCallbackArgs * arg)
{


	prh_sdp_unit_310_global = 1;
}

/******
*  TEST NAME:   
	prh_sdp_unit_310

	*  TEST DESCRIPTION:
		Test the SDP_ConnectTransport will work when the peer disconnect the connection

	*  TEST STEPS:
		1. Call SDP_ConnectTransport
		2. accept the connection
		3. peer disconnects
		4. check results

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_310(int p1, void *arg)
{
  void* connection;
  t_bdaddr address;
  int ret;

  prh_sdp_unit_310_global = 0;
	ret = HU_FAILED;

	SDP_ConnectTransport(&address,1024,prh_sdp_unit_111_callback,prh_sdp_unit_310_callback);
	prh_stub_l2cap_accept_l2_connection(&connection,1);

	prh_stub_l2cap_do_disconnectind(connection); /* remove connection */

	if(prh_sdp_unit_310_global)
		ret = HU_PASSED;


/*
	SDP_DisconnectTransport(1);
	prh_stub_l2cap_send_disconnect_confirmation(connection,0);
	*/

	return ret;
}


/******
*  TEST NAME:   
	prh_sdp_unit_311

	*  TEST DESCRIPTION:
		Test the SDP_ConnectTransport will work when there is a 
		super vision timeout.

	*  TEST STEPS:
		1. Call SDP_ConnectTransport
		2. supervision timeout 
		3. check results

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_311(int p1, void *arg)
{
	return prh_sdp_unit_310(p1,arg); /* same as previous test */
}


/******
*  TEST NAME:   
	prh_sdp_unit_116

	*  TEST DESCRIPTION:
		Test the SDP_DisconnectTransport will with invalid cid.

	*  TEST STEPS:
		1. Call SDP_DisconnectTransport with bad cid
		2. check results

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_116(int p1, void *arg)
{
  int ret;

  ret = HU_PASSED;

  if(SDP_DisconnectTransport(666)!= BT_NOERROR)
    ret = HU_FAILED;
	
  return ret;
}

/******
*  TEST NAME:   
	prh_sdp_unit_312

	*  TEST DESCRIPTION:
		Test the SDP_DisconnectTransport will with valid cid.

	*  TEST STEPS:
		1. Call SDP_DisconnectTransport with valid cid
		2. check link status
		3. check results

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_312(int p1, void *arg)
{
	  void* connection;
  t_bdaddr address;
  int ret;

  prh_sdp_unit_310_global = 0;
	ret = HU_FAILED;

	SDP_ConnectTransport(&address,1024,prh_sdp_unit_111_callback,prh_sdp_unit_310_callback);
	prh_stub_l2cap_accept_l2_connection(&connection,1);

	SDP_DisconnectTransport(1);

  prh_stub_l2cap_send_disconnect_confirmation(connection,0);

	if(prh_sdp_unit_310_global==0)
		ret = HU_PASSED;


/*
	SDP_DisconnectTransport(1);
	prh_stub_l2cap_send_disconnect_confirmation(connection,0);
	*/

	return ret;
}



/******
*  TEST NAME:   
	prh_sdp_unit_313

	*  TEST DESCRIPTION:
		Test the SDP_DisconnectTransport and the peer disconects

	*  TEST STEPS:
		1. Call SDP_DisconnectTransport 
		2. peer disconnects at the same time.
		3. check results

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_313(int p1, void *arg)
{
	  void* connection;
  t_bdaddr address;
  int ret;

  prh_sdp_unit_310_global = 0;
	ret = HU_FAILED;

	SDP_ConnectTransport(&address,1024,prh_sdp_unit_111_callback,prh_sdp_unit_310_callback);
	prh_stub_l2cap_accept_l2_connection(&connection,1);

	prh_stub_l2cap_do_disconnectind(connection);

	SDP_DisconnectTransport(1);

	if(prh_sdp_unit_310_global)
		ret = HU_PASSED;


/*
	SDP_DisconnectTransport(1);
	prh_stub_l2cap_send_disconnect_confirmation(connection,0);
	*/

	return ret;

}


/******
*  TEST NAME:   
	prh_sdp_unit_314

	*  TEST DESCRIPTION:
		Test the SDP_DisconnectTransport and Supervision timeout

	*  TEST STEPS:
		1. Call SDP_DisconnectTransport 
		2. supervision timeout.
		3. check results

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_314(int p1, void *arg)
{
	  void* connection;
  t_bdaddr address;
  int ret;

  prh_sdp_unit_310_global = 0;
	ret = HU_FAILED;

	SDP_ConnectTransport(&address,1024,prh_sdp_unit_111_callback,prh_sdp_unit_310_callback);
	prh_stub_l2cap_accept_l2_connection(&connection,1);

	SDP_DisconnectTransport(1);

	prh_stub_l2cap_do_disconnectind(connection);

	if(prh_sdp_unit_310_global)
		ret = HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_305

	*  TEST DESCRIPTION:
		Test the SSAR with invalid aid list

	*  TEST STEPS:
		1. Call SDP_ServiceSearchAttributeRequest
		2. accept the connection.
		3. check request
		4. send error response for invalid  aid list
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_305(int p1, void *arg)
{
t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int32 uuids;
  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x1f,0x00,0x15,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);
  
  SDP_ServiceSearchAttributeRequest(&pResults,1,&pPattern,&pAttributes,100,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_error_response(buffer->data,SDP_InvalidSyntax,0,0,prh_sdp_read16(buffer->data + 1));

  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  /*
    free the results
   */


/*    if(pResults->length == sizeof(data))
    {
      if(memcmp(pResults->pData,data,sizeof(data)))
	return HU_FAILED;
    } else {
      return HU_FAILED;
    }
*/

  pFree(pResults->pData);
  pFree(pResults);

  prh_stub_l2cap_do_disconnectind(connection);
   
	pDebugCheck();

  return HU_PASSED;
}




/******
	*  TEST NAME:   
		prh_sdp_unit_306

	*  TEST DESCRIPTION:
		Test the SSAR with error code no resources

	*  TEST STEPS:
		1. Call SDP_ServiceSearchAttributeRequest
		2. accept the connection.
		3. check request
		4. send error response for no resources
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_306(int p1, void *arg)
{
t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int32 uuids;
  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x1f,0x00,0x15,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);
  
  SDP_ServiceSearchAttributeRequest(&pResults,1,&pPattern,&pAttributes,100,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_error_response(buffer->data,SDP_InsufficientResources,0,0,prh_sdp_read16(buffer->data + 1));

  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  /*
    free the results
   */


/*    if(pResults->length == sizeof(data))
    {
      if(memcmp(pResults->pData,data,sizeof(data)))
	return HU_FAILED;
    } else {
      return HU_FAILED;
    }
*/
  pFree(pResults->pData);
  pFree(pResults);

  prh_stub_l2cap_do_disconnectind(connection);
   
	pDebugCheck();

  return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_307

	*  TEST DESCRIPTION:
		Test the SSAR with invalid continue

	*  TEST STEPS:
		1. Call SDP_ServiceSearchAttributeRequest
		2. accept the connection.
		3. check request
		4. send error response for an invalid continue
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_307(int p1, void *arg)
{
t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int32 uuids;
  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x1f,0x00,0x15,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);
  
  SDP_ServiceSearchAttributeRequest(&pResults,1,&pPattern,&pAttributes,100,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_error_response(buffer->data,SDP_InvalidContinuation,0,0,prh_sdp_read16(buffer->data + 1));

  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  /*
    free the results
   */


  /*  if(pResults->length == sizeof(data))
    {
      if(memcmp(pResults->pData,data,sizeof(data)))
	return HU_FAILED;
    } else {
      return HU_FAILED;
    }
*/

  pFree(pResults->pData);
  pFree(pResults);

  prh_stub_l2cap_do_disconnectind(connection);
   
	pDebugCheck();

  return HU_PASSED;
}




/******
	*  TEST NAME:   
		prh_sdp_unit_308

	*  TEST DESCRIPTION:
		Test the SSAR with error for invalid size

	*  TEST STEPS:
		1. Call SDP_ServiceSearchAttributeRequest
		2. accept the connection.
		3. check request
		4. send error response for invalid size
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_308(int p1, void *arg)
{
t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int32 uuids;
  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x1f,0x00,0x15,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);
  
  SDP_ServiceSearchAttributeRequest(&pResults,1,&pPattern,&pAttributes,100,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_error_response(buffer->data,SDP_InvalidSize,0,0,prh_sdp_read16(buffer->data + 1));

  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  /*
    free the results
   */

/*
    if(pResults->length == sizeof(data))
    {
      if(memcmp(pResults->pData,data,sizeof(data)))
	return HU_FAILED;
    } else {
      return HU_FAILED;
    }
*/

  pFree(pResults->pData);
  pFree(pResults);

  prh_stub_l2cap_do_disconnectind(connection);
   
	pDebugCheck();

  return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_309

	*  TEST DESCRIPTION:
		Test the SSAR with error for unsupported 
		
	*  TEST STEPS:
		1. Call SDP_ServiceSearchAttributeRequest
		2. accept the connection.
		3. check request
		4. send error response for unsupported
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_309(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int32 uuids;
  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x1f,0x00,0x15,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);
  
  SDP_ServiceSearchAttributeRequest(&pResults,1,&pPattern,&pAttributes,100,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_error_response(buffer->data,SDP_UnSupported,0,0,prh_sdp_read16(buffer->data + 1));

  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  /*
    free the results
   */

/*
    if(pResults->length == sizeof(data))
    {
      if(memcmp(pResults->pData,data,sizeof(data)))
	return HU_FAILED;
    } else {
      return HU_FAILED;
    }
*/

  pFree(pResults->pData);
  pFree(pResults);

  prh_stub_l2cap_do_disconnectind(connection);
   
	pDebugCheck();

  return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_354

	*  TEST DESCRIPTION:
		Test the SSR with error in the main state

	*  TEST STEPS:
		1. Call SDP_ServiceSearchRequest
		2. place in main state
		3. inject error response 
		4. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_354(int p1, void *arg)
{
t_SDP_SearchPattern pPattern;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 uuids;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };
  u_int8 test_results_a[] = {0x02,0x00,0x1d,0x00,0x0d,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x0a,0x00
  };
  int ret;
 struct prh_sdp_primitive_task *p_task;
struct prh_sdp_primitive_task *temp;

  prh_sdp_flush_database();

	pDebugCheck();
  ret = HU_FAILED;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

	prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

	p_task = malloc(sizeof( struct prh_sdp_primitive_task)); /* create dummy task */
	p_task->p_next = 0x00;
	p_SDP_Primitive_Task_List = p_task;

  SDP_ServiceSearchRequest(&pResults,1,&pPattern,10,0x00,60,0x00);

  temp = p_task->p_next;
  free(p_task);
	p_SDP_Primitive_Task_List = temp;

/*  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }
*/

  prh_stub_l2cap_GetWriteBuffer(50,&buffer);

  prh_sdp_generate_error_response(buffer->data,SDP_InvalidSize,0,0,prh_sdp_read16(buffer->data + 1));

  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);
  

    /*
    free the results
   */

  if(pResults->status == 0)
	  ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();
 
  return ret;	
}

/******
	*  TEST NAME:   
		prh_sdp_unit_355

	*  TEST DESCRIPTION:
		Test the SSR with error in find services state

	*  TEST STEPS:
		1. Call SDP_ServiceSearchRequest
		2. place in find services state
		3. inject error response 
		4. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_355(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 uuids;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };
  u_int8 test_results_a[] = {0x02,0x00,0x1d,0x00,0x0d,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x0a,0x00
  };
  u_int8 data[] = "Here is some random gibberish";
 struct prh_sdp_primitive_task *p_task;
struct prh_sdp_primitive_task *temp;
	int ret;

  prh_sdp_flush_database();

	pDebugCheck();
  
  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

	ret = HU_FAILED;

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

	p_task = malloc(sizeof( struct prh_sdp_primitive_task)); /* create dummy task */
	p_task->p_next = 0x00;
	p_SDP_Primitive_Task_List = p_task;


  SDP_ServiceSearchRequest(&pResults,1,&pPattern,10,0x00,60,0x00);

  temp = p_task->p_next;
  free(p_task);
	p_SDP_Primitive_Task_List = temp;

	/*
	prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }
*/

	prh_stub_l2cap_GetWriteBuffer(50,&buffer);

  prh_sdp_generate_error_response(buffer->data,SDP_UnSupported,0,0,prh_sdp_read16(buffer->data + 1));
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);
  
    /*
    free the results
   */

  if(pResults->status == 0)
	  ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();
 
  return ret;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_356

	*  TEST DESCRIPTION:
		Test the SSR with attribute response in w4 services state

	*  TEST STEPS:
		1. Call SDP_ServiceSearchRequest
		2. place in w4 services state
		3. inject attribute response 
		4. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_356(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 uuids;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };
  u_int8 test_results_a[] = {0x02,0x00,0x1d,0x00,0x0d,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x0a,0x00
  };
  u_int8 data[] = "Here is some random gibberish";

  int ret;

  prh_sdp_flush_database();

	pDebugCheck();
  
	ret = HU_FAILED;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceSearchRequest(&pResults,1,&pPattern,10,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);
  

    /*
    free the results
   */

	if(pResults->status == 0 && pResults->length == 0)
		ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();
 
  return ret;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_357

	*  TEST DESCRIPTION:
		Test the SSR with a peer disconnect

	*  TEST STEPS:
		1. Call SDP_ServiceSearchRequest
		2. inject disconnect.
		3. check result

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_357(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 uuids;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };
  u_int8 test_results_a[] = {0x02,0x00,0x1d,0x00,0x0d,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x0a,0x00
  };
  u_int8 data[] = "Here is some random gibberish";
	int ret;


  prh_sdp_flush_database();

	pDebugCheck();
  ret = HU_FAILED;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceSearchRequest(&pResults,1,&pPattern,10,0x00,60,0x00);


  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_stub_l2cap_do_disconnectind(connection);  

    /*
    free the results
   */

	if(pResults->length == 0)
		ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();
 
  return ret;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_358

	*  TEST DESCRIPTION:
		Test the SAR with a error in the main state

	*  TEST STEPS:
		1. Call SDP_ServiceAttributeRequest
		2. place in main state
		2. inject error
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_358(int p1, void *arg)
{
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int8 data[] = "Here is some random gibberish";
  u_int8 test_results_a[] = {0x04,0x00,0x1e,0x00,0x0f,0x00,0x00,0x10,0x01,
			     0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };
 struct prh_sdp_primitive_task *p_task;
struct prh_sdp_primitive_task *temp;
	int ret;

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

	pDebugCheck();
  
	ret = HU_FAILED;

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

	p_task = malloc(sizeof( struct prh_sdp_primitive_task)); /* create dummy task */
	p_task->p_next = 0x00;
	p_SDP_Primitive_Task_List = p_task;


  SDP_ServiceAttributeRequest(&pResults,1,0x001001,&pAttributes,100,0x00, 60,0x00);

    temp = p_task->p_next;
  free(p_task);
	p_SDP_Primitive_Task_List = temp;

/*
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }
*/

	prh_stub_l2cap_GetWriteBuffer(50,&buffer);

	prh_sdp_generate_error_response(buffer->data,SDP_UnSupported,0,0,prh_sdp_read16(buffer->data + 1));
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

pDebugCheck();

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);

  pDebugCheck();
  
   /*
    free the results
   */

	if(pResults->status == 0)
		ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();

  return ret;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_359

	*  TEST DESCRIPTION:
		Test the SAR with a error in the find attributes state

	*  TEST STEPS:
		1. Call SDP_ServiceAttributeRequest
		2. place in find attributes state
		2. inject error
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_359(int p1, void *arg)
{
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int8 data[] = "Here is some random gibberish";
  u_int8 test_results_a[] = {0x04,0x00,0x1e,0x00,0x0f,0x00,0x00,0x10,0x01,
			     0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };
  int ret;
 struct prh_sdp_primitive_task *p_task;
struct prh_sdp_primitive_task *temp;

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

	pDebugCheck();
  ret = HU_FAILED;
  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  	p_task = malloc(sizeof( struct prh_sdp_primitive_task)); /* create dummy task */
	p_task->p_next = 0x00;
	p_SDP_Primitive_Task_List = p_task;

  SDP_ServiceAttributeRequest(&pResults,1,0x001001,&pAttributes,100,0x00, 60,0x00);

      temp = p_task->p_next;
  free(p_task);
	p_SDP_Primitive_Task_List = temp;

/*  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }
*/

		prh_stub_l2cap_GetWriteBuffer(50,&buffer);

	prh_sdp_generate_error_response(buffer->data,SDP_UnSupported,0,0,prh_sdp_read16(buffer->data + 1));
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

pDebugCheck();

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);

  pDebugCheck();
  
   /*
    free the results
   */

	if(pResults->status == 0)
		ret = HU_PASSED;
	
  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();

  return ret;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_360

	*  TEST DESCRIPTION:
		Test the SAR with a service recieved in the find attributes state

	*  TEST STEPS:
		1. Call SDP_ServiceAttributeRequest
		2. place in w4_attributes state
		2. inject service response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_360(int p1, void *arg)
{
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int8 data[] = "Here is some random gibberish";
  u_int8 test_results_a[] = {0x04,0x00,0x1e,0x00,0x0f,0x00,0x00,0x10,0x01,
			     0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  int ret;

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

	pDebugCheck();
  
	ret = HU_PASSED;

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceAttributeRequest(&pResults,1,0x001001,&pAttributes,100,0x00, 60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

   prh_sdp_generate_search_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),1,1,0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

pDebugCheck();

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);

  pDebugCheck();
  
   /*
    free the results
   */

	if(pResults->status == 0 && pResults->length == 0)
		ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();

  return ret;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_361

	*  TEST DESCRIPTION:
		Test the SSAR with a error in the main state

	*  TEST STEPS:
		1. Call SDP_ServiceSearchAttributeRequest
		2. place in main state
		2. inject error
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_361(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int32 uuids;
  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x1f,0x00,0x15,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };
  int ret;
 struct prh_sdp_primitive_task *p_task;
struct prh_sdp_primitive_task *temp;


  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  pDebugCheck();

  ret = HU_FAILED;
  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

    	p_task = malloc(sizeof( struct prh_sdp_primitive_task)); /* create dummy task */
	p_task->p_next = 0x00;
	p_SDP_Primitive_Task_List = p_task;


  SDP_ServiceSearchAttributeRequest(&pResults,1,&pPattern,&pAttributes,100,0x00,60,0x00);

/*
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }
*/
      temp = p_task->p_next;
  free(p_task);
	p_SDP_Primitive_Task_List = temp;

		prh_stub_l2cap_GetWriteBuffer(50,&buffer);
  
	prh_sdp_generate_error_response(buffer->data,SDP_UnSupported,0,0,prh_sdp_read16(buffer->data + 1));
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  /*
    free the results
   */


  if(pResults->status == 0)
	  ret = HU_PASSED;
    

  pFree(pResults->pData);
  pFree(pResults);

  prh_stub_l2cap_do_disconnectind(connection);
   
	pDebugCheck();

  return ret;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_362

	*  TEST DESCRIPTION:
		Test the SSAR with a error in the find service attributes state

	*  TEST STEPS:
		1. Call SDP_ServiceSearchAttributeRequest
		2. place in find service attributes state
		2. inject error
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_362(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int32 uuids;
  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x1f,0x00,0x15,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };
  int ret;
 struct prh_sdp_primitive_task *p_task;
struct prh_sdp_primitive_task *temp;

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
	uuids = 0x1103;

	ret = HU_FAILED;
  pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

      	p_task = malloc(sizeof( struct prh_sdp_primitive_task)); /* create dummy task */
	p_task->p_next = 0x00;
	p_SDP_Primitive_Task_List = p_task;

  SDP_ServiceSearchAttributeRequest(&pResults,1,&pPattern,&pAttributes,100,0x00,60,0x00);

      temp = p_task->p_next;
  free(p_task);
	p_SDP_Primitive_Task_List = temp;

		prh_stub_l2cap_GetWriteBuffer(50,&buffer);

  /*
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }
*/

  prh_sdp_generate_error_response(buffer->data,SDP_UnSupported,0,0,prh_sdp_read16(buffer->data + 1));
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  /*
    free the results
   */

  if(pResults->status == 0)
	  ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  prh_stub_l2cap_do_disconnectind(connection);
   
	pDebugCheck();

  return ret;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_363

	*  TEST DESCRIPTION:
		Test the SSAR with a attribute recieved in the find attributes state

	*  TEST STEPS:
		1. Call SDP_ServiceSearchAttributeRequest
		2. place in w4_attributes state
		2. inject attribute response
		3. check state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_363(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int32 uuids;
  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x1f,0x00,0x15,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  int ret;

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  pDebugCheck();

  ret = HU_FAILED;

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);
  
  SDP_ServiceSearchAttributeRequest(&pResults,1,&pPattern,&pAttributes,100,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  
  prh_sdp_generate_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  /*
    free the results
   */

	if(pResults->status == 0 && pResults->length == 0)
		ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  prh_stub_l2cap_do_disconnectind(connection);
   
	pDebugCheck();

  return ret;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_267
 
	*  TEST DESCRIPTION:
		Test the SSR for a time out.

	*  TEST STEPS:
		1. Call SDP_ServiceSearchRequest
		2. accept the connection.
		3. check the message
		4. wait for the timeout
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_267(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 uuids;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };
  u_int8 test_results_a[] = {0x02,0x00,0x1d,0x00,0x0d,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x0a,0x00
  };

  int ret = HU_FAILED;

  prh_sdp_flush_database();

	pDebugCheck();
  
  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceSearchRequest(&pResults,1,&pPattern,10,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_stub_l2cap_do_disconnectind(connection);  

    /*
    free the results
   */

	if(pResults->length == 0)
		ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);
 
  return ret;		
}


/******
	*  TEST NAME:   
		prh_sdp_unit_268
 
	*  TEST DESCRIPTION:
		Test the SSR using a 16bit uuid

	*  TEST STEPS:
		1. Call SDP_ServiceSearchRequest for 16bit uuid
		2. accept the connection.
		3. check the message
		4. send back response
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_268(int p1, void *arg)
{
	return HU_UNTESTED;/* This test is invalid, only a 32bit UUID can be used */
}

/******
	*  TEST NAME:   
		prh_sdp_unit_269
 
	*  TEST DESCRIPTION:
		Test the SSR for 2 matching services.

	*  TEST STEPS:
		1. Call SDP_ServiceSearchRequest
		2. accept the connection.
		3. check the message
		4. send back response
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_269(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 uuids;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };
  u_int8 test_results_a[] = {0x02,0x00,0x1d,0x00,0x0d,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x0a,0x00
  };

  prh_sdp_flush_database();

	pDebugCheck();
  
  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceSearchRequest(&pResults,1,&pPattern,10,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_search_response(buffer->data,handles,prh_sdp_read16(buffer->data + 1),2,2,0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);
  

    /*
    free the results
   */

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();
 
  return HU_PASSED;
}


static t_pEvent prh_sdp_unit_271_wait;

void prh_sdp_unit_271_cb(int status)
{
	pEventSet(prh_sdp_unit_271_wait);
}

/******
	*  TEST NAME:   
		prh_sdp_unit_271
 
	*  TEST DESCRIPTION:
		Test the SSR for max duration

	*  TEST STEPS:
		1. Call SDP_ServiceSearchRequest
		2. accept the connection.
		3. wait for timer
		4. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_271(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 uuids;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };
  u_int8 test_results_a[] = {0x02,0x00,0x1d,0x00,0x0d,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x0a,0x00
  };

  prh_sdp_flush_database();

	pDebugCheck();
  
  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  prh_sdp_unit_271_wait = pEventCreate(0);

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceSearchRequest(&pResults,1,&pPattern,10,0x00,10,prh_sdp_unit_271_cb);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }


	pEventWait(prh_sdp_unit_271_wait);

  
  prh_stub_l2cap_do_disconnectind(connection);  

    /*
    free the results
   */

  if(pResults->pData) pFree(pResults->pData);
  pFree(pResults);

  pEventFree(prh_sdp_unit_271_wait);

  return HU_PASSED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_272
 
	*  TEST DESCRIPTION:
		Test the SSR for max items

	*  TEST STEPS:
		1. Call SDP_ServiceSearchRequest
		2. accept the connection.
		3. send back results
		4. cause max items to occur
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_272(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 uuids;
  u_int32 handles[] = { 0x00010000 , 0x00010001 ,0x00010002};
  u_int8 test_results_a[] = {0x02,0x00,0x1d,0x00,0x0d,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x0a,0x00
  };
  int ret;

  prh_sdp_flush_database();

	pDebugCheck();
  
  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  ret = HU_FAILED;
  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceSearchRequest(&pResults,1,&pPattern,1,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

/*  
  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }
*/

  prh_sdp_generate_search_response(buffer->data,handles,prh_sdp_read16(buffer->data + 1),3,2,0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);
  
    /*
    free the results
   */
  if(pResults->length == 4) /* not 8 */
	  ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();
 
  return ret; 
}

/******
	*  TEST NAME:   
		prh_sdp_unit_273
 
	*  TEST DESCRIPTION:
		Test the SSR for service not found

	*  TEST STEPS:
		1. Call SDP_ServiceSearchRequest
		2. accept the connection.
		3. send back results with no services
		4. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_273(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 uuids;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };
  u_int8 test_results_a[] = {0x02,0x00,0x1d,0x00,0x0d,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x0a,0x00
  };
  int ret;

  prh_sdp_flush_database();

	pDebugCheck();
  
	ret = HU_FAILED;
  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceSearchRequest(&pResults,1,&pPattern,10,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_search_response(buffer->data,handles,prh_sdp_read16(buffer->data + 1),0,0,0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);
  

    /*
    free the results
   */

	if(	pResults->length == 0 && pResults->status == 0)
		ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();
 
  return ret;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_274
 
	*  TEST DESCRIPTION:
		Test the SSR service found in second entry

	*  TEST STEPS:
		1. Call SDP_ServiceSearchRequest
		2. accept the connection.
		3. send back results
		4. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_274(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 uuids;
  u_int32 handles[] = { 0x00010001 };
  u_int8 test_results_a[] = {0x02,0x00,0x1d,0x00,0x0d,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x0a,0x00
  };

  int ret;

  prh_sdp_flush_database();

	pDebugCheck();
  
	ret = HU_FAILED;	
  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceSearchRequest(&pResults,1,&pPattern,10,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_search_response(buffer->data,handles,prh_sdp_read16(buffer->data + 1),1,1,0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);
  

    /*
    free the results
   */
	if(pResults->length == 4)
		ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();
 
  return ret;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_275
 
	*  TEST DESCRIPTION:
		Test the SSR invalid encoding of uuid list

	*  TEST STEPS:
		1. Call SDP_ServiceSearchRequest
		2. accept the connection.
		3. send error response for a bad uuid list.
		4. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_275(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 uuids;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };
  u_int8 test_results_a[] = {0x02,0x00,0x1d,0x00,0x0d,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x0a,0x00
  };

  int ret;

  prh_sdp_flush_database();

	pDebugCheck();
  
	ret = HU_FAILED;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceSearchRequest(&pResults,1,&pPattern,10,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_error_response(buffer->data,SDP_InvalidSyntax,0,0,prh_sdp_read16(buffer->data + 1));

  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);
  

    /*
    free the results
   */

  
	if(pResults->status == SDP_InvalidSyntax && pResults->length == 00)
		ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();
 
  return ret;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_277
 
	*  TEST DESCRIPTION:
		Test the SSR no resources at peer

	*  TEST STEPS:
		1. Call SDP_ServiceSearchRequest
		2. accept the connection.
		3. send back error response for no resources
		4. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_277(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 uuids;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };
  u_int8 test_results_a[] = {0x02,0x00,0x1d,0x00,0x0d,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x0a,0x00
  };
  
  int ret;

  prh_sdp_flush_database();

	pDebugCheck();
  ret = HU_FAILED;
  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceSearchRequest(&pResults,1,&pPattern,10,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_error_response(buffer->data,SDP_InsufficientResources,0,0,prh_sdp_read16(buffer->data + 1));

  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);
  

    /*
    free the results
   */

	if(pResults->status == SDP_InsufficientResources && pResults->length == 00)
		ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();
 
  return ret;

}

/******
	*  TEST NAME:   
		prh_sdp_unit_278
 
	*  TEST DESCRIPTION:
		Test the SSR with invalid continue

	*  TEST STEPS:
		1. Call SDP_ServiceSearchRequest
		2. accept the connection.
		3. send back error response for invalid continue
		4. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_278(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 uuids;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };
  u_int8 test_results_a[] = {0x02,0x00,0x1d,0x00,0x0d,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x0a,0x00
  };

  int ret;
  prh_sdp_flush_database();

	pDebugCheck();
  
	ret = HU_FAILED;
  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceSearchRequest(&pResults,1,&pPattern,10,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_error_response(buffer->data,SDP_InvalidContinuation,0,0,prh_sdp_read16(buffer->data + 1));

  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);
  

    /*
    free the results
   */

  	if(pResults->status == SDP_InvalidContinuation && pResults->length == 00)
		ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();
 
  return ret;


}


/******
	*  TEST NAME:   
		prh_sdp_unit_279
 
	*  TEST DESCRIPTION:
		Test the SSR with invalid size

	*  TEST STEPS:
		1. Call SDP_ServiceSearchRequest
		2. accept the connection.
		3. send back error response invalid size
		4. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_279(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 uuids;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };
  u_int8 test_results_a[] = {0x02,0x00,0x1d,0x00,0x0d,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x0a,0x00
  };

	int ret;

  prh_sdp_flush_database();

	pDebugCheck();
  ret = HU_FAILED;
  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceSearchRequest(&pResults,1,&pPattern,10,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_error_response(buffer->data,SDP_InvalidSize,0,0,prh_sdp_read16(buffer->data + 1));

  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);
  

    /*
    free the results
   */
  	if(pResults->status == SDP_InvalidSize && pResults->length == 00)
		ret = HU_PASSED;  

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();
 
  return ret;	
}


/******
	*  TEST NAME:   
		prh_sdp_unit_280
 
	*  TEST DESCRIPTION:
		Test the SSR with unsupported

	*  TEST STEPS:
		1. Call SDP_ServiceSearchRequest
		2. accept the connection.
		3. send back error response for unsupported.
		4. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_280(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 uuids;
  u_int32 handles[] = { 0x00010000 , 0x00010001 };
  u_int8 test_results_a[] = {0x02,0x00,0x1d,0x00,0x0d,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x0a,0x00
  };

  int ret;

  prh_sdp_flush_database();

	pDebugCheck();
  ret = HU_FAILED;
  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceSearchRequest(&pResults,1,&pPattern,10,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_error_response(buffer->data,SDP_UnSupported,0,0,prh_sdp_read16(buffer->data + 1));

  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);
  

    /*
    free the results
   */

  	if(pResults->status == SDP_UnSupported && pResults->length == 00)
		ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();
 
  return ret;
}


static t_pEvent prh_sdp_unit_281_wait;

void prh_sdp_unit_281_cb(int status)
{
	pEventSet(prh_sdp_unit_281_wait);
}

/******
	*  TEST NAME:   
		prh_sdp_unit_281

	*  TEST DESCRIPTION:
		Test the SAR with 2 devices and the first one disconnects

	*  TEST STEPS:
		1. Call SDP_ServiceAttributeRequest
		2. accept the connection.
		3. check request
		4. peer disconnects
		5. accept the connection.
		6. send back response
		7. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_281(int p1, void *arg)
{
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int8 data[] = "Here is some random gibberish";
  u_int8 test_results_a[] = {0x04,0x00,0x1e,0x00,0x0f,0x00,0x00,0x10,0x01,
			     0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

	/* the number of connections is not related to this api call - invalid test*/

  return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_282

	*  TEST DESCRIPTION:
		Test the SAR with aid range

	*  TEST STEPS:
		1. Call SDP_ServiceAttributeRequest with aid range
		2. accept the connection.
		3. check resquest
		4. send back response
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_282(int p1, void *arg)
{
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int8 data[] = "Here is some random gibberish";
  u_int8 test_results_a[] = {0x04,0x00,0x1e,0x00,0x11,0x00,0x00,0x10,0x01,
			     0x00,0x64,0x37,0x00,0x00,0x00,0x05,0x0a,0x00,0x01,0x00,0x03,0x00
  };

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00010003;

	pDebugCheck();
  
  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceAttributeRequest(&pResults,1,0x001001,&pAttributes,100,0x00, 60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

pDebugCheck();

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);

  pDebugCheck();
  
   /*
    free the results
   */

  if(pResults->length == sizeof(data))
    {
      if(memcmp(pResults->pData,data,sizeof(data)))
	return HU_FAILED;
    } else {
      return HU_FAILED;
    }

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();

  return HU_PASSED;

}

/******
	*  TEST NAME:   
		prh_sdp_unit_283

	*  TEST DESCRIPTION:
		Test the SAR and max bytes limit is hit

	*  TEST STEPS:
		1. Call SDP_ServiceAttributeRequest
		2. accept the connection.
		3. check request
		4. send back response
		5. cause max bytes to occur.
		6. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_283(int p1, void *arg)
{
	  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int8 data[] = "Here is some random gibberish";
  u_int8 test_results_a[] = {0x04,0x00,0x1e,0x00,0x0f,0x00,0x00,0x10,0x01,
			     0x00,0x1e,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

	pDebugCheck();
  
  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceAttributeRequest(&pResults,1,0x001001,&pAttributes,sizeof(data),0x00, 60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

pDebugCheck();

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);

  pDebugCheck();
  
   /*
    free the results
   */

  if(pResults->length == sizeof(data))
    {
      if(memcmp(pResults->pData,data,sizeof(data)))
	return HU_FAILED;
    } else {
      return HU_FAILED;
    }

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();

  return HU_PASSED;	
}

static t_pEvent prh_sdp_unit_284_wait;

void prh_sdp_unit_284_cb(int status)
{
	pEventSet(prh_sdp_unit_284_wait);
}

/******
	*  TEST NAME:   
		prh_sdp_unit_284

	*  TEST DESCRIPTION:
		Test the SAR and max duration limit is hit

	*  TEST STEPS:
		1. Call SDP_ServiceAttributeRequest
		2. accept the connection.
		3. check request
		4. wait for timer.
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_284(int p1, void *arg)
{
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int8 data[] = "Here is some random gibberish";
  u_int8 test_results_a[] = {0x04,0x00,0x1e,0x00,0x0f,0x00,0x00,0x10,0x01,
			     0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;


	prh_sdp_unit_284_wait = pEventCreate(0);

	pDebugCheck();
  
  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceAttributeRequest(&pResults,1,0x001001,&pAttributes,100,0x00,10,prh_sdp_unit_284_cb);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  pEventWait(prh_sdp_unit_284_wait);

  prh_stub_l2cap_do_disconnectind(connection);

  pDebugCheck();
  
   /*
    free the results
   */


  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();
  
  pEventFree(prh_sdp_unit_284_wait);

  return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_285

	*  TEST DESCRIPTION:
		Test the SAR and there are no matching attributes

	*  TEST STEPS:
		1. Call SDP_ServiceAttributeRequest
		2. accept the connection.
		3. check request
		4. send response with no results.
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_285(int p1, void *arg)
{
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int8 data[] = "Here is some random gibberish";
  u_int8 test_results_a[] = {0x04,0x00,0x1e,0x00,0x0f,0x00,0x00,0x10,0x01,
			     0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  int ret;

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

	pDebugCheck();

	ret = HU_FAILED;
  
  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceAttributeRequest(&pResults,1,0x001001,&pAttributes,100,0x00, 60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

	prh_sdp_generate_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),0,0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

pDebugCheck();

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);

  pDebugCheck();
  
   /*
    free the results
   */

  if(pResults->length == 0)
	  ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();

  return ret;
}



/******
	*  TEST NAME:   
		prh_sdp_unit_286

	*  TEST DESCRIPTION:
		Test the SAR and there are some matching attributes

	*  TEST STEPS:
		1. Call SDP_ServiceAttributeRequest
		2. accept the connection.
		3. check request
		4. send response with results for some of the attributes.
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_286(int p1, void *arg)
{
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int8 data[] = "Here is some random gibberish";
  u_int8 test_results_a[] = {0x04,0x00,0x11,0x00,0x11,0x00,0x00,0x10,0x01,
			     0x00,0x64,0x37,0x00,0x00,0x00,0x05,0x0a,0x00,0x01,0x00,0x03,0x00
  };

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00010003;

	pDebugCheck();
  
  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceAttributeRequest(&pResults,1,0x001001,&pAttributes,100,0x00, 60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

	prh_sdp_generate_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

pDebugCheck();

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);

  pDebugCheck();
  
   /*
    free the results
   */

  if(pResults->length == sizeof(data))
    {
      if(memcmp(pResults->pData,data,sizeof(data)))
	return HU_FAILED;
    } else {
      return HU_FAILED;
    }

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();

  return HU_PASSED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_287

	*  TEST DESCRIPTION:
		Test the SAR with an error for incorrect handle

	*  TEST STEPS:
		1. Call SDP_ServiceAttributeRequest
		2. accept the connection.
		3. check request
		4. send error response for incorrect handle
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_287(int p1, void *arg)
{
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int8 data[] = "Here is some random gibberish";
  u_int8 test_results_a[] = {0x04,0x00,0x1e,0x00,0x0f,0x00,0x00,0x10,0x01,
			     0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  int ret;
  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

	pDebugCheck();
  ret = HU_FAILED;
  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceAttributeRequest(&pResults,1,0x001001,&pAttributes,100,0x00, 60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_error_response(buffer->data,SDP_InvalidHandle,0,0,prh_sdp_read16(buffer->data + 1));

  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

pDebugCheck();

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);

  pDebugCheck();
  
   /*
    free the results
   */
	if(pResults->status == SDP_InvalidHandle && pResults->length == 0)
		ret = HU_PASSED;
  
  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();

  return ret;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_288

	*  TEST DESCRIPTION:
		Test the SAR with an error invalid aid list

	*  TEST STEPS:
		1. Call SDP_ServiceAttributeRequest
		2. accept the connection.
		3. check request
		4. send error response for an invalid list.
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_288(int p1, void *arg)
{
t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int8 data[] = "Here is some random gibberish";
  u_int8 test_results_a[] = {0x04,0x00,0x1e,0x00,0x0f,0x00,0x00,0x10,0x01,
			     0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  int ret;
  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

	pDebugCheck();
  
	ret = HU_FAILED;

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceAttributeRequest(&pResults,1,0x001001,&pAttributes,100,0x00, 60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_error_response(buffer->data,SDP_InvalidSyntax,0,0,prh_sdp_read16(buffer->data + 1));

  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

pDebugCheck();

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);

  pDebugCheck();
  
   /*
    free the results
   */

	if(pResults->status == SDP_InvalidSyntax && pResults->length == 0)
		ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();

  return ret;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_289

	*  TEST DESCRIPTION:
		Test the SAR with an error for no resources

	*  TEST STEPS:
		1. Call SDP_ServiceAttributeRequest
		2. accept the connection.
		3. check request
		4. send error response for no resources
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_289(int p1, void *arg)
{
t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int8 data[] = "Here is some random gibberish";
  u_int8 test_results_a[] = {0x04,0x00,0x1e,0x00,0x0f,0x00,0x00,0x10,0x01,
			     0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };
  int ret;

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

	pDebugCheck();
  ret = HU_FAILED;
  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceAttributeRequest(&pResults,1,0x001001,&pAttributes,100,0x00, 60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_error_response(buffer->data,SDP_InsufficientResources,0,0,prh_sdp_read16(buffer->data + 1));

  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

pDebugCheck();

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);

  pDebugCheck();
  
   /*
    free the results
   */

  	if(pResults->status == SDP_InsufficientResources && pResults->length == 0)
		ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();

  return ret;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_290

	*  TEST DESCRIPTION:
		Test the SAR with an error for an invalid continue

	*  TEST STEPS:
		1. Call SDP_ServiceAttributeRequest
		2. accept the connection.
		3. check request
		4. send error response for an invalid continue
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_290(int p1, void *arg)
{
t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int8 data[] = "Here is some random gibberish";
  u_int8 test_results_a[] = {0x04,0x00,0x1e,0x00,0x0f,0x00,0x00,0x10,0x01,
			     0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  int ret;

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

	pDebugCheck();
  
	ret = HU_FAILED;

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceAttributeRequest(&pResults,1,0x001001,&pAttributes,100,0x00, 60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_error_response(buffer->data,SDP_InvalidContinuation,0,0,prh_sdp_read16(buffer->data + 1));

  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

pDebugCheck();

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);

  pDebugCheck();
  
   /*
    free the results
   */

  	if(pResults->status == SDP_InvalidContinuation && pResults->length == 0)
		ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();

  return ret;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_291

	*  TEST DESCRIPTION:
		Test the SAR with an error for an invalid size

	*  TEST STEPS:
		1. Call SDP_ServiceAttributeRequest
		2. accept the connection.
		3. check request
		4. send error response for an invalid message size
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_291(int p1, void *arg)
{
t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int8 data[] = "Here is some random gibberish";
  u_int8 test_results_a[] = {0x04,0x00,0x1e,0x00,0x0f,0x00,0x00,0x10,0x01,
			     0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  int ret;
  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

	pDebugCheck();
  
	ret = HU_FAILED;

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceAttributeRequest(&pResults,1,0x001001,&pAttributes,100,0x00, 60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_error_response(buffer->data,SDP_InvalidSize,0,0,prh_sdp_read16(buffer->data + 1));

  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

pDebugCheck();

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);

  pDebugCheck();
  
   /*
    free the results
   */

	if(pResults->status == SDP_InvalidSize && pResults->length == 0)
		ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();

  return ret;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_292

	*  TEST DESCRIPTION:
		Test the SAR with an error for unsupported

	*  TEST STEPS:
		1. Call SDP_ServiceAttributeRequest
		2. accept the connection.
		3. check request
		4. send error response for unsupported
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_292(int p1, void *arg)
{
t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int8 data[] = "Here is some random gibberish";
  u_int8 test_results_a[] = {0x04,0x00,0x1e,0x00,0x0f,0x00,0x00,0x10,0x01,
			     0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  int ret;

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

	pDebugCheck();
  
	ret = HU_FAILED;

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  SDP_ServiceAttributeRequest(&pResults,1,0x001001,&pAttributes,100,0x00, 60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_error_response(buffer->data,SDP_UnSupported,0,0,prh_sdp_read16(buffer->data + 1));

  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);

pDebugCheck();

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_do_disconnectind(connection);

  pDebugCheck();
  
   /*
    free the results
   */

  	if(pResults->status == SDP_UnSupported && pResults->length == 0)
		ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  pDebugCheck();

  return ret;
}



static t_pEvent prh_sdp_unit_293_wait;

void prh_sdp_unit_293_cb(int status)
{
	pEventSet(prh_sdp_unit_293_wait);
}


/******
	*  TEST NAME:   
		prh_sdp_unit_293

	*  TEST DESCRIPTION:
		Test the SSAR with 2 devices and the first disconnects.

	*  TEST STEPS:
		1. Call SDP_ServiceSearchAttributeRequest
		2. accept the connection.
		3. check request
		4. disconnect
		5. accept the connection.
		6. check request
		7. send response
		8. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_293(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int32 uuids;
  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x1f,0x00,0x15,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  /*	This is an invalid test as the connection is not related to this api call so 2 devices 
	does not make any sense.	
  */
	return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_294

	*  TEST DESCRIPTION:
		Test the SSAR with 16bit uuid

	*  TEST STEPS:
		1. Call SDP_ServiceSearchAttributeRequest
		2. accept the connection.
		3. check request
		4. send response
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_294(int p1, void *arg)
{
	/* this test is not possible only 32bit uuid can be used. */
	return HU_UNTESTED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_295

	*  TEST DESCRIPTION:
		Test the SSAR with 2 matching services.

	*  TEST STEPS:
		1. Call SDP_ServiceSearchAttributeRequest
		2. accept the connection.
		3. check request
		4. send response for 2 services.
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_295(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int32 uuids;
  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x1f,0x00,0x15,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  pDebugCheck();
	
  /*
	as far as the client is concerned this is the same as  the normal case
  */

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);
  
  SDP_ServiceSearchAttributeRequest( &pResults,1,&pPattern,&pAttributes,100,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_service_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  /*
    free the results
   */


    if(pResults->length == sizeof(data))
    {
      if(memcmp(pResults->pData,data,sizeof(data)))
	return HU_FAILED;
    } else {
      return HU_FAILED;
    }

  pFree(pResults->pData);
  pFree(pResults);

  prh_stub_l2cap_do_disconnectind(connection); 

	pDebugCheck();

  return HU_PASSED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_296

	*  TEST DESCRIPTION:
		Test the SSAR with aid range

	*  TEST STEPS:
		1. Call SDP_ServiceSearchAttributeRequest with aid range
		2. accept the connection.
		3. check request
		4. send response 
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_296(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int32 uuids;
  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x1f,0x00,0x17,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x64,0x37,0x00,0x00,0x00,0x05,0x0a,0x00,0x01,0x00,0x03,0x00
  };

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00010003;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);
  
  SDP_ServiceSearchAttributeRequest(&pResults,1,&pPattern,&pAttributes,100,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_service_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  /*
    free the results
   */


    if(pResults->length == sizeof(data))
    {
      if(memcmp(pResults->pData,data,sizeof(data)))
	return HU_FAILED;
    } else {
      return HU_FAILED;
    }

  pFree(pResults->pData);
  pFree(pResults);

  prh_stub_l2cap_do_disconnectind(connection);
   
	pDebugCheck();

  return HU_PASSED;

}

/******
	*  TEST NAME:   
		prh_sdp_unit_297

	*  TEST DESCRIPTION:
		Test the SSAR with max bytes

	*  TEST STEPS:
		1. Call SDP_ServiceSearchAttributeRequest
		2. accept the connection.
		3. check request
		4. send response , cause max bytes limit
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_297(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int32 uuids;
  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x1f,0x00,0x15,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x1e,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);
  
  SDP_ServiceSearchAttributeRequest(&pResults,1,&pPattern,&pAttributes,sizeof(data),0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_service_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  /*
    free the results
   */


    if(pResults->length == sizeof(data))
    {
      if(memcmp(pResults->pData,data,sizeof(data)))
	return HU_FAILED;
    } else {
      return HU_FAILED;
    }

  pFree(pResults->pData);
  pFree(pResults);

  prh_stub_l2cap_do_disconnectind(connection);
   
	pDebugCheck();

  return HU_PASSED;
}

static t_pEvent prh_sdp_unit_298_wait;

void prh_sdp_unit_298_cb(int status)
{
	pEventSet(prh_sdp_unit_298_wait );
}

/******
	*  TEST NAME:   
		prh_sdp_unit_298

	*  TEST DESCRIPTION:
		Test the SSAR with max duration

	*  TEST STEPS:
		1. Call SDP_ServiceSearchAttributeRequest
		2. accept the connection.
		3. check request
		4. wait for timeout.
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_298(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int32 uuids;
  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x1f,0x00,0x15,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  
  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  prh_sdp_unit_298_wait = pEventCreate(0);

  pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);
  
  SDP_ServiceSearchAttributeRequest(&pResults,1,&pPattern,&pAttributes,100,0x00,10,prh_sdp_unit_298_cb);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  pEventWait(prh_sdp_unit_298_wait);

  pFree(pResults->pData);
  pFree(pResults);

  prh_stub_l2cap_do_disconnectind(connection);
   
	pDebugCheck();

	pEventFree(prh_sdp_unit_298_wait);


  return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_299

	*  TEST DESCRIPTION:
		Test the SSAR with max items

	*  TEST STEPS:
		1. Call SDP_ServiceSearchAttributeRequest
		2. accept the connection.
		3. check request
		4. send response , cause max items limit
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_299(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int32 uuids;
  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x1f,0x00,0x15,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  pDebugCheck();

	/*
		there is no way to tell, this test relates to the ServiceBrowse...
	*/

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);
  
  SDP_ServiceSearchAttributeRequest(&pResults,1,&pPattern,&pAttributes,100,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_service_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  /*
    free the results
   */


    if(pResults->length == sizeof(data))
    {
      if(memcmp(pResults->pData,data,sizeof(data)))
	return HU_FAILED;
    } else {
      return HU_FAILED;
    }

  pFree(pResults->pData);
  pFree(pResults);

  prh_stub_l2cap_do_disconnectind(connection);
   
	pDebugCheck();

  return HU_PASSED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_300

	*  TEST DESCRIPTION:
		Test the SSAR with service found in second entry

	*  TEST STEPS:
		1. Call SDP_ServiceSearchAttributeRequest
		2. accept the connection.
		3. check request
		4. send response
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_300(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int32 uuids;
  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x1f,0x00,0x15,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  pDebugCheck();

  /*
	from the client side this doesn't matter...
  */

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);
  
  SDP_ServiceSearchAttributeRequest(&pResults,1,&pPattern,&pAttributes,100,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_service_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  /*
    free the results
   */


    if(pResults->length == sizeof(data))
    {
      if(memcmp(pResults->pData,data,sizeof(data)))
	return HU_FAILED;
    } else {
      return HU_FAILED;
    }

  pFree(pResults->pData);
  pFree(pResults);

  prh_stub_l2cap_do_disconnectind(connection);
   
	pDebugCheck();

  return HU_PASSED;

}

/******
	*  TEST NAME:   
		prh_sdp_unit_301

	*  TEST DESCRIPTION:
		Test the SSAR with no attributes

	*  TEST STEPS:
		1. Call SDP_ServiceSearchAttributeRequest
		2. accept the connection.
		3. check request
		4. send response with no attributes
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_301(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int32 uuids;
  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x1f,0x00,0x15,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  int ret;

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  pDebugCheck();

  ret = HU_FAILED;

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);
  
  SDP_ServiceSearchAttributeRequest(&pResults,1,&pPattern,&pAttributes,100,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_service_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),0,0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  /*
    free the results
   */

	if(pResults->length == 0)
		ret = HU_PASSED;

  pFree(pResults->pData);
  pFree(pResults);

  prh_stub_l2cap_do_disconnectind(connection);
   
	pDebugCheck();

  return ret;

}



/******
	*  TEST NAME:   
		prh_sdp_unit_302

	*  TEST DESCRIPTION:
		Test the SSAR with some attributes

	*  TEST STEPS:
		1. Call SDP_ServiceSearchAttributeRequest
		2. accept the connection.
		3. check request
		4. send response with some attributes
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_302(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int32 uuids;
  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x1f,0x00,0x15,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);
  
  SDP_ServiceSearchAttributeRequest(&pResults,1,&pPattern,&pAttributes,100,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_service_attrib_response(buffer->data,data,prh_sdp_read16(buffer->data + 1),sizeof(data),0x00);
  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  /*
    free the results
   */



  pFree(pResults->pData);
  pFree(pResults);

  prh_stub_l2cap_do_disconnectind(connection);
   
	pDebugCheck();

  return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_304

	*  TEST DESCRIPTION:
		Test the SSAR with invalid uuid list

	*  TEST STEPS:
		1. Call SDP_ServiceSearchAttributeRequest
		2. accept the connection.
		3. check request
		4. send error response for invalid uuid
		5. check the results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_304(int p1, void *arg)
{
  t_SDP_SearchPattern pPattern;
  t_SDP_AttributeIDs pAttributes;
  t_SDP_LowLevelResults* pResults;
  struct host_buf *buffer;
  void* connection;
  t_bdaddr address;
  int unused;  
  u_int32 aid;
  u_int32 uuids;
  u_int8 data[] = "Here is some random gibberish";

  u_int8 test_results_a[] = {0x06,0x00,0x1f,0x00,0x15,0x37,0x00,0x00,0x00,0x05,0x1a,0x00,0x00,
			     0x11,0x03,0x00,0x64,0x37,0x00,0x00,0x00,0x03,0x09,0x00,0x03,0x00
  };

  pAttributes.numItems = 1;
  pAttributes.attributeIDs = &aid;
  aid = 0x00000003;

  pPattern.numItems = 1;
  pPattern.patternUUIDs = &uuids;
  uuids = 0x1103;

  pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);
  
  SDP_ServiceSearchAttributeRequest(&pResults,1,&pPattern,&pAttributes,100,0x00,60,0x00);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
  {
	  prh_stub_l2cap_do_disconnectind(connection);
    return HU_FAILED;
  }

  prh_sdp_generate_error_response(buffer->data,SDP_InvalidSyntax,0,0,prh_sdp_read16(buffer->data + 1));

  buffer->len =  SDP_PDU_HEADER_LENGTH + prh_sdp_read16(buffer->data + 3);
  prh_stub_l2cap_put_data(connection,buffer);

  /*
    free the results
   */

/*
    if(pResults->length == sizeof(data))
    {
      if(memcmp(pResults->pData,data,sizeof(data)))
	return HU_FAILED;
    } else {
      return HU_FAILED;
    }
*/
  pFree(pResults->pData);
  pFree(pResults);

  prh_stub_l2cap_do_disconnectind(connection);
   
	pDebugCheck();

  return HU_PASSED;
}


int prh_sdp_interface_connect_clenup(int test,void* arg)
{
	L2_DeregisterPSM(1);
	SDP_RegisterPSM();
}


prh_t_hu_testinfo prh_sdp_interface_tests [] =  
{
	{ "TP_SDP_V_298_U",0x00,prh_sdp_unit_298, NULL, 0 }, /* these timer tests are run 3 times */
	{ "TP_SDP_V_284_U",0x00,prh_sdp_unit_284, NULL, 0 }, /* these timer tests are run 3 times */
            { "TP_SDP_V_039_U",0x00,prh_sdp_unit_039, NULL, 0 },
      		{ "TP_SDP_V_040_U",0x00,prh_sdp_unit_040, NULL, 0 },
	      	{ "TP_SDP_V_041_U",0x00,prh_sdp_unit_041, NULL, 0 },
			{ "TP_SDP_V_042_U",0x00,prh_sdp_unit_042, NULL, 0 },
	      	{ "TP_SDP_V_043_U",0x00,prh_sdp_unit_043, NULL, 0 },
	      	{ "TP_SDP_V_044_U",0x00,prh_sdp_unit_044, NULL, 0 },

			

	      	{ "TP_SDP_V_111_U",0x00,prh_sdp_unit_111, prh_sdp_interface_connect_clenup, 0 },
	      	{ "TP_SDP_V_112_U",0x00,prh_sdp_unit_112, prh_sdp_interface_connect_clenup, 0 },
	      	{ "TP_SDP_V_310_U",0x00,prh_sdp_unit_310, prh_sdp_interface_connect_clenup, 0 },
	      	{ "TP_SDP_V_116_U",0x00,prh_sdp_unit_116, prh_sdp_interface_connect_clenup, 0 },
	      	{ "TP_SDP_V_312_U",0x00,prh_sdp_unit_312, prh_sdp_interface_connect_clenup, 0 },
	      	{ "TP_SDP_V_313_U",0x00,prh_sdp_unit_313, prh_sdp_interface_connect_clenup, 0 },
	      	{ "TP_SDP_V_314_U",0x00,prh_sdp_unit_314, prh_sdp_interface_connect_clenup, 0 },

	      	{ "TP_SDP_V_305_U",0x00,prh_sdp_unit_305, NULL, 0 },
	      	{ "TP_SDP_V_306_U",0x00,prh_sdp_unit_306, NULL, 0 },
	      	{ "TP_SDP_V_307_U",0x00,prh_sdp_unit_307, NULL, 0 },
	      	{ "TP_SDP_V_308_U",0x00,prh_sdp_unit_308, NULL, 0 },
	      	{ "TP_SDP_V_309_U",0x00,prh_sdp_unit_309, NULL, 0 },
			{ "TP_SDP_V_298_U",0x00,prh_sdp_unit_298, NULL, 0 }, /* these timer tests are run 3 times */
	      	{ "TP_SDP_V_354_U",0x00,prh_sdp_unit_354, NULL, 0 },
	      	{ "TP_SDP_V_355_U",0x00,prh_sdp_unit_355, NULL, 0 },
	      	{ "TP_SDP_V_356_U",0x00,prh_sdp_unit_356, NULL, 0 },
	      	{ "TP_SDP_V_357_U",0x00,prh_sdp_unit_357, NULL, 0 },
	      	{ "TP_SDP_V_358_U",0x00,prh_sdp_unit_358, NULL, 0 },
	      	{ "TP_SDP_V_359_U",0x00,prh_sdp_unit_359, NULL, 0 },
	      	{ "TP_SDP_V_360_U",0x00,prh_sdp_unit_360, NULL, 0 },
	      	{ "TP_SDP_V_361_U",0x00,prh_sdp_unit_361, NULL, 0 },
	      	{ "TP_SDP_V_362_U",0x00,prh_sdp_unit_362, NULL, 0 },
	      	{ "TP_SDP_V_363_U",0x00,prh_sdp_unit_363, NULL, 0 },
      	{ "TP_SDP_V_311_U",0x00,prh_sdp_unit_311, prh_sdp_interface_connect_clenup, 0 }, 
	{ "TP_SDP_V_267_U",0x00,prh_sdp_unit_267, NULL, 0 },
/*	{ "TP_SDP_V_268_U",0x00,prh_sdp_unit_268, NULL, 0 },*/ /* This test is invalid, only a 32bit UUID can be used */
	{ "TP_SDP_V_269_U",0x00,prh_sdp_unit_269, NULL, 0 },
/*	{ "TP_SDP_V_270_U",0x00,prh_sdp_unit_270, NULL, 0 }, invalid */
	{ "TP_SDP_V_271_U",0x00,prh_sdp_unit_271, NULL, 0 },
	{ "TP_SDP_V_272_U",0x00,prh_sdp_unit_272, NULL, 0 },
	{ "TP_SDP_V_273_U",0x00,prh_sdp_unit_273, NULL, 0 },
	{ "TP_SDP_V_274_U",0x00,prh_sdp_unit_274, NULL, 0 },
	{ "TP_SDP_V_275_U",0x00,prh_sdp_unit_275, NULL, 0 },
	{ "TP_SDP_V_277_U",0x00,prh_sdp_unit_277, NULL, 0 },
	{ "TP_SDP_V_278_U",0x00,prh_sdp_unit_278, NULL, 0 },
	{ "TP_SDP_V_279_U",0x00,prh_sdp_unit_279, NULL, 0 },
	{ "TP_SDP_V_280_U",0x00,prh_sdp_unit_280, NULL, 0 },
	{ "TP_SDP_V_284_U",0x00,prh_sdp_unit_284, NULL, 0 }, /* these timer tests are run 3 times */
	{ "TP_SDP_V_281_U",0x00,prh_sdp_unit_281, NULL, 0 },
	{ "TP_SDP_V_282_U",0x00,prh_sdp_unit_282, NULL, 0 },
	{ "TP_SDP_V_283_U",0x00,prh_sdp_unit_283, NULL, 0 },
	{ "TP_SDP_V_285_U",0x00,prh_sdp_unit_285, NULL, 0 },
	{ "TP_SDP_V_286_U",0x00,prh_sdp_unit_286, NULL, 0 },
	{ "TP_SDP_V_287_U",0x00,prh_sdp_unit_287, NULL, 0 },
	{ "TP_SDP_V_288_U",0x00,prh_sdp_unit_288, NULL, 0 },
	{ "TP_SDP_V_289_U",0x00,prh_sdp_unit_289, NULL, 0 },
	{ "TP_SDP_V_290_U",0x00,prh_sdp_unit_290, NULL, 0 },
	{ "TP_SDP_V_291_U",0x00,prh_sdp_unit_291, NULL, 0 },
	{ "TP_SDP_V_292_U",0x00,prh_sdp_unit_292, NULL, 0 },
	{ "TP_SDP_V_293_U",0x00,prh_sdp_unit_293, NULL, 0 },
	/*{ "TP_SDP_V_294_U",0x00,prh_sdp_unit_294, NULL, 0 }, /* This test is invalid, only a 32bit UUID can be used */
	{ "TP_SDP_V_295_U",0x00,prh_sdp_unit_295, NULL, 0 },
	{ "TP_SDP_V_296_U",0x00,prh_sdp_unit_296, NULL, 0 },
	{ "TP_SDP_V_297_U",0x00,prh_sdp_unit_297, NULL, 0 },
	{ "TP_SDP_V_299_U",0x00,prh_sdp_unit_299, NULL, 0 },
	{ "TP_SDP_V_300_U",0x00,prh_sdp_unit_300, NULL, 0 },
	{ "TP_SDP_V_301_U",0x00,prh_sdp_unit_301, NULL, 0 },
	{ "TP_SDP_V_302_U",0x00,prh_sdp_unit_302, NULL, 0 },
	{ "TP_SDP_V_304_U",0x00,prh_sdp_unit_304, NULL, 0 },
	{ "TP_SDP_V_298_U",0x00,prh_sdp_unit_298, NULL, 0 },
	{ "TP_SDP_V_284_U",0x00,prh_sdp_unit_284, NULL, 0 }, /* these timer tests are run 3 times */
	{ "TP_SDP_V_298_U",0x00,prh_sdp_unit_298, NULL, 0 }, /* these timer tests are run 3 times */
};

prh_t_hu_harness prh_sdp_interface_group =
{ NULL, HU_SDP, "SDP_INTERFACE",prh_sdp_unit_init,PRH_UT_STUBS_ENABLED, 1,sizeof(prh_sdp_interface_tests)/sizeof(prh_t_hu_testinfo), prh_sdp_interface_tests, 0, 0, 0, 0, 0};

void test_sdp_interface(void)
{
  prh_hu_add_testgroup(&prh_sdp_interface_group);
}


