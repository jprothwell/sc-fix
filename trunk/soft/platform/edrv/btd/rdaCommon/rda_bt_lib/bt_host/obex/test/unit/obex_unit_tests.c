/*
  obex_unit tests

  $Id: obex_unit_tests.c 1532 2010-08-23 05:47:34Z huazeng $
 */

#include "cunit.h"
#include "host_types.h"


int linux_sys_close;

int linux_sys_mode;

u_int32 unit_test_state1;
u_int32 unit_test_state2;
u_int32 unit_test_state3;
u_int8* unit_test_pointer1;
u_int8* unit_test_pointer2;
u_int8* unit_test_pointer3;

void test_register(void);

/* modules */

void test_obex_server_interface(void);
void test_obex_client_interface(void);
void test_obex_utils(void);
void test_obex_tal(void);
void test_obex_sec(void);
void test_obex_sec_wrap(void);
void test_obex_sync_server(void);

/*
int main(int argc, char **argv)
{
  unit_test_pointer1 = 0;
  unit_test_pointer2 = 0;
  unit_test_pointer3 = 0;
  
  unit_test_state1 = 0;
  unit_test_state2 = 0;
  unit_test_state3 = 0;

  cuStart(test_register);
	
  return 0;
}
*/

void register_obex_unit_tests(void)
{
	
/*		test_obex_sec();*/
/*		test_obex_sec_wrap();*/
/*		test_obex_tal();*/
/*		test_obex_server_interface();*/
/*		test_obex_client_interface();*/
		test_obex_utils();
/*		test_obex_sync_server();*/
}
