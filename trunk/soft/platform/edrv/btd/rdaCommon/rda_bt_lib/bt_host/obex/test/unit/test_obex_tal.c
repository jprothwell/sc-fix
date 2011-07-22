/*
  test file for obex_tal.c
 */

#include "host_config.h"

#include "host_types.h"
#include "obex_server_types.h"
#include "obex_client_types.h"
#include "obex_tal.h"
#include "obex_defines.h"
#include "mgr_utils.h"

#include "cunit.h"

extern u_int32 unit_test_state1;
extern u_int32 unit_test_state2;
extern u_int32 unit_test_state3;
extern u_int8* unit_test_pointer1;
extern u_int8* unit_test_pointer2;
extern u_int8* unit_test_pointer3;


extern struct prh_obex_transport* prh_obex_transmit_transport;
extern struct prh_obex_session_state* prh_obex_state;
extern struct prh_obex_command_callback* prh_obex_client_callbacks;

void test_prh_obex_from_transport(void);
void test_prh_obex_transport_tx(void);
void test_prh_obex_tal_rfcomm_server_data(void);
void test_prh_obex_tal_rfcomm_client_data(void);

void test_obex_tal(void)
{
  cuRegTest("prh_obex_from_transport",3,test_prh_obex_from_transport);
  cuRegTest("prh_obex_transport_tx",4,test_prh_obex_transport_tx);
  cuRegTest("prh_obex_tal_rfcomm_server_data",2,test_prh_obex_tal_rfcomm_server_data);
  cuRegTest("prh_obex_tal_rfcomm_client_data",2,test_prh_obex_tal_rfcomm_client_data);
}


void test_prh_obex_from_transport(void)
{
  struct prh_obex_session_state* p_state;
  t_DataBuf *data;
  /*
  prh_obex_from_transport(p_state,data);
  */


/*

  I need to write a load of tests for this function test all the states.
  
  */

  cuReport("a",1);
  cuReport("b",1);
  cuReport("c",1);
  
}

void test_prh_obex_transport_tx(void)
{
	struct prh_obex_transport pTransport;
	t_DataBuf* txbuffer;
	u_int8 buffer[100];
	u_int8 output[100];


	prh_obex_transmit_transport = &pTransport;

	pTransport.max_transport = 20;
	pTransport.max_obex = 50;
	pTransport.tid = 1;
	pTransport.type = PRH_OBEX_TYPE_RFCOMM;

	memcpy(buffer,test_prh_obex_transport_tx,100);
	
	txbuffer =  host_buf_alloc(120);
	host_buf_reserve_header(txbuffer,20);
	txbuffer->dataLen = 20;
	memcpy(txbuffer->buf,buffer,20);

	unit_test_state1 = 0;
	unit_test_pointer1 = output;

	prh_obex_transport_tx(1,txbuffer);
	cuReport(" data = transport mtu",!memcmp(buffer,output,20));

	memset(output,0,40);

	txbuffer =  host_buf_alloc(120);
	host_buf_reserve_header(txbuffer,20);
	txbuffer->dataLen = 40;
	memcpy(txbuffer->buf,buffer,40);

	prh_obex_transport_tx(1,txbuffer);
	cuReport(" data > transport mtu < obex mtu",!memcmp(buffer,output,40));

	memset(output,0,50);

	txbuffer =  host_buf_alloc(120);
	host_buf_reserve_header(txbuffer,20);
	txbuffer->dataLen = 50;
	memcpy(txbuffer->buf,buffer,50);

	prh_obex_transport_tx(1,txbuffer);
	cuReport(" data > transport mtu = obex mtu",!memcmp(buffer,output,50));

	memset(output,0,50);

	txbuffer =  host_buf_alloc(120);
	host_buf_reserve_header(txbuffer,20);
	txbuffer->dataLen = 51;
	memcpy(txbuffer->buf,buffer,51);

	cuReport(" data > transport mtu > obex mtu",prh_obex_transport_tx(1,txbuffer));
}


void test_prh_obex_tal_rfcomm_server_data(void)
{
	struct prh_obex_transport pTransport;
	struct prh_obex_session_state state;
	t_DataBuf* txbuffer;
	u_int8 buffer[100];
	u_int8 output[100];
	u_int8 temp_buf[100];
	t_bdaddr address;

	MGR_ParseHexBDAddress("0xdbdbdbdb0001",&address);


	prh_obex_transmit_transport = &pTransport;

	pTransport.max_transport = 20;
	pTransport.max_obex = 50;
	pTransport.tid = 1;
	pTransport.type = PRH_OBEX_TYPE_RFCOMM;
	pTransport.use.rfcomm.dlci = 1;
	pTransport.use.rfcomm.address = address;
	pTransport.rx_buffer = temp_buf;
	pTransport.length = 0;

	state.tid = 1;

	prh_obex_state = &state;

	memcpy(buffer,test_prh_obex_transport_tx,100);

	buffer[1] = 0;
	buffer[2] = 20;

	
	txbuffer =  host_buf_alloc(120);
	host_buf_reserve_header(txbuffer,20);
	txbuffer->dataLen = 20;
	memcpy(txbuffer->buf,buffer,20);

	unit_test_state1 = 0;
	unit_test_pointer1 = output;
	
	prh_obex_tal_rfcomm_server_data(1,address,txbuffer);

	cuReport(" 1 rfcomm packet ",!memcmp(buffer,output,20));

	buffer[1] = 0;
	buffer[2] = 40;

	txbuffer =  host_buf_alloc(120);
	host_buf_reserve_header(txbuffer,20);
	txbuffer->dataLen = 20;
	memcpy(txbuffer->buf,buffer,20);
	
	prh_obex_tal_rfcomm_server_data(1,address,txbuffer);

		txbuffer =  host_buf_alloc(120);
	host_buf_reserve_header(txbuffer,20);
	txbuffer->dataLen = 20;
	memcpy(txbuffer->buf,buffer+20,20);
	
	prh_obex_tal_rfcomm_server_data(1,address,txbuffer);

	cuReport(" 2 rfcomm packets ",!memcmp(buffer,output,20));

}


void test_prh_obex_tal_rfcomm_client_data(void)
{
	struct prh_obex_transport pTransport;
	struct prh_obex_command_callback pCallback;
	t_DataBuf* txbuffer;
	u_int8 buffer[100];
	u_int8 output[100];
	u_int8 temp_buf[100];
	t_bdaddr address;

	MGR_ParseHexBDAddress("0xdbdbdbdb0001",&address);


	prh_obex_transmit_transport = &pTransport;

	pTransport.max_transport = 20;
	pTransport.max_obex = 50;
	pTransport.tid = 1;
	pTransport.type = PRH_OBEX_TYPE_RFCOMM;
	pTransport.use.rfcomm.dlci = 1;
	pTransport.use.rfcomm.address = address;
	pTransport.rx_buffer = temp_buf;
	pTransport.length = 0;

	prh_obex_client_callbacks = &pCallback;

	pCallback.tid = 1;

	memcpy(buffer,test_prh_obex_transport_tx,100);

	buffer[1] = 0;
	buffer[2] = 20;

	
	txbuffer =  host_buf_alloc(120);
	host_buf_reserve_header(txbuffer,20);
	txbuffer->dataLen = 20;
	memcpy(txbuffer->buf,buffer,20);

	unit_test_state1 = 0;
	unit_test_pointer1 = output;
	
	prh_obex_tal_rfcomm_client_data(1,address,txbuffer);

	cuReport(" 1 rfcomm packet ",!memcmp(buffer,output,20));

	buffer[1] = 0;
	buffer[2] = 40;

	txbuffer =  host_buf_alloc(120);
	host_buf_reserve_header(txbuffer,20);
	txbuffer->dataLen = 20;
	memcpy(txbuffer->buf,buffer,20);
	
	prh_obex_tal_rfcomm_client_data(1,address,txbuffer);

	txbuffer =  host_buf_alloc(120);
	host_buf_reserve_header(txbuffer,20);
	txbuffer->dataLen = 20;
	memcpy(txbuffer->buf,buffer+20,20);
	
	prh_obex_tal_rfcomm_client_data(1,address,txbuffer);

	cuReport(" 2 rfcomm packets ",!memcmp(buffer,output,20));
}