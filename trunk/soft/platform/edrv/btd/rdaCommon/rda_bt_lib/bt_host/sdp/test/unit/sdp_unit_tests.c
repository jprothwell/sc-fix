/*
  obex_unit tests

  $Id: sdp_unit_tests.c 1532 2010-08-23 05:47:34Z huazeng $
 */

#include "host_types.h"
#include "hu_harness.h"

int linux_sys_close;

int linux_sys_mode;

u_int32 unit_test_state1;
u_int32 unit_test_state2;
u_int32 unit_test_state3;
u_int8* unit_test_pointer1;
u_int8* unit_test_pointer2;
u_int8* unit_test_pointer3;

/* modules */

void test_sdp_sdap(void);
void test_sdp_interface(void);
void test_sdp_server(void);
void test_sdp_database(void);


t_api prh_sdp_srv_layer_ctrl(u_int8 flags);

void register_sdp_unit_tests(void)
{
	prh_sdp_srv_layer_ctrl(1);
	
  test_sdp_sdap();
  test_sdp_interface();
  test_sdp_server();
  test_sdp_database();
}

int  prh_sdp_unit_init(int p1, void *arg)
{
  SDP_RegisterPSM();
  return 0;
}

prh_t_hu_outcome prh_sdp_test_check(struct host_buf *buffer,u_int8* expected,u_int16 len)
{
  if(buffer->len == len)
    {
      if(buffer->data[0] == *expected)
	{
	  if(!memcmp(buffer->data+3,expected+3,len-3))
	    {
	      return HU_PASSED;
	    } else return HU_FAILED;
	} else return HU_FAILED;
    } else return HU_FAILED;    
}
