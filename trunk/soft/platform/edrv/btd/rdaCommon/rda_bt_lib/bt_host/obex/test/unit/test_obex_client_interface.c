/*
  test file for obex_client_interface.c
 */

#include "host_config.h"

#include "host_types.h"
#include "host_error.h"
#include "obex_client_types.h"
#include "obex_client_interface.h"

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

void test_OBEX_Connect_header_gen(void);
void connectCallbackFn(u_int8 status, t_DataBuf* data);

void test_obex_client_interface(void)
{
  	cuRegTest("OBEX_Connect_header_gen",2,test_OBEX_Connect_header_gen);	
}

void test_OBEX_Connect_header_gen(void)
{
	u_int32 tid;
	u_int8 version;
	u_int8 flags;
	u_int16 max_length;
	u_int16 target_length;
	u_int8* target;
	u_int16 header_length;
	t_DataBuf *databuf;
	int retValue;

  struct host_buf* h_buff;
	void (*callbackfunc)(u_int8 status, t_DataBuf* data)= connectCallbackFn;

  h_buff = host_buf_alloc(100);
	host_buf_reserve_header(h_buff,30);

  databuf= (t_DataBuf*)h_buff;

	tid = 0x000000bb;
	version = 0x10;
	flags = 0x00;
	max_length = 0x2000;

	target = pMalloc(10);

	target[0] = 0x01;
	target[1] = 0x02;
	target[2] = 0x03;
	target[3] = 0x04;
	target[4] = 0x05;

	target_length = 0x0005;

	header_length = 0x0000;

	/*databuf = pMalloc(240);*/

	retValue = OBEX_Connect(tid,version,flags,max_length,target_length,target,header_length,databuf,callbackfunc);
  cuReport("Connect not working with target- basic",(retValue == BT_NOERROR));

	target = 0x00;
	target_length = 0;

	retValue = OBEX_Connect(tid,version,flags,max_length,target_length,target,header_length,databuf,callbackfunc);
  cuReport("Connect not working without target - basic",(retValue == BT_NOERROR));

}


void connectCallbackFn(u_int8 status, t_DataBuf* data)
{
	;
}