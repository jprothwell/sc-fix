/*
  test file for obex_server_interface.c
 */

#include "host_config.h"

#include "host_types.h"
#include "host_error.h"
#include "obex_server_types.h"
#include "obex_server_interface.h"
#include "obex_client_types.h"

#include "cunit.h"

#include "obex_tal.h"
#include "papi.h"

extern u_int32 unit_test_state1;
extern u_int32 unit_test_state2;
extern u_int32 unit_test_state3;
extern u_int8* unit_test_pointer1;
extern u_int8* unit_test_pointer2;
extern u_int8* unit_test_pointer3;

extern struct prh_obex_transport* prh_obex_transmit_transport;
extern struct prh_obex_session_state obex_state;

int prh_obex_allocate_cid(struct prh_obex_service_registration* registration);

void test_prh_obex_allocate_cid(void);
void test_OBEX_RegisterInbox(void);
void test_OBEX_RegisterService(void);
void test_OBEX_TransportListen_RF(void);
void test_OBEX_Response(void);
void test_OBEX_GetCID(void);

void test_obex_server_interface(void)
{
	cuRegTest("prh_obex_allocate_cid",3,test_prh_obex_allocate_cid);
	cuRegTest("OBEX_TransportListen_RF",1,test_OBEX_TransportListen_RF);
	cuRegTest("OBEX_RegisterInbox",3,test_OBEX_RegisterInbox);
	cuRegTest("OBEX_RegisterService",3,test_OBEX_RegisterService);
	cuRegTest("OBEX_Response",3,test_OBEX_RegisterService);
	cuRegTest("OBEX_GetCID",3,test_OBEX_GetCID);
}






void test_prh_obex_allocate_cid(void)
{
  struct prh_obex_service_registration reg;
  
  reg.cid = 0x00;
  reg.cid_length = 0x00;

  prh_obex_allocate_cid(&reg);
  cuReport("increment",*(u_int32*)reg.cid);
  
  prh_obex_allocate_cid(&reg);
  cuReport("length",reg.cid_length == 4);

  *((u_int32*)reg.cid) = -1;
  prh_obex_allocate_cid(&reg);  
  cuReport("wrapping",*(u_int32*)reg.cid == 0x00);
}


void test_OBEX_TransportListen_RF(void)
{
  u_int8 server_channel;
  u_int32 tid;
  void* cb;
  int ret;

  server_channel = 5;
  cb = 0x00;

  ret = OBEX_TransportListen_RF(server_channel,&tid,cb);
  cuReport("normal call",ret != BT_NOERROR);
}


void test_OBEX_RegisterInbox(void)
{
	u_int32 tid;
	t_obex_service_handler handler;
	int ret;
	struct prh_obex_transport* pTransport;
	
	prh_obex_transmit_transport = 0x00;
	tid = 0;
	handler = 1;
	ret = OBEX_RegisterInbox(tid,handler,0x00);
	cuReport("no transport regisered",ret == BT_INVALIDPARAM);
	
	tid = 0;
	handler = 1;
	pTransport = pMalloc(sizeof(struct prh_obex_transport));
	pTransport->tid = 1;
	pTransport->p_next = 0x00;
	prh_obex_transmit_transport = pTransport;

	ret = OBEX_RegisterInbox(tid,handler,0x00);
	cuReport("invalid tid",ret == BT_INVALIDPARAM);
	free(pTransport);

	tid = 1;
	handler = 1;
	pTransport = pMalloc(sizeof(struct prh_obex_transport));
	pTransport->tid = 1;
	pTransport->p_next = 0x00;
	prh_obex_transmit_transport = pTransport;

	ret = OBEX_RegisterInbox(tid,handler,0x00);
	cuReport("Normal",ret == BT_NOERROR);
	free(pTransport);
}


void test_OBEX_RegisterService(void)
{

}


void test_OBEX_Response(void)
{

}


void test_OBEX_GetCID(void)
{
	/*
	int OBEX_GetCID(u_int32 tid,u_int8* target, u_int8 target_length,u_int8* cid,u_int8* cid_length)
	*/
}