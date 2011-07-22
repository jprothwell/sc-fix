/*
  obex_unit tests

  $Id: rfcomm_unit_tests.c 1532 2010-08-23 05:47:34Z huazeng $
 */

#include "host_types.h"


u_int32 unit_test_state1;
u_int32 unit_test_state2;
u_int32 unit_test_state3;
u_int8* unit_test_pointer1;
u_int8* unit_test_pointer2;
u_int8* unit_test_pointer3;

void test_register(void);

/* modules */

void test_rfcomm_frame_gen(void);

#if 0
int main(int argc, char **argv)
{
  unit_test_pointer1 = 0;
  unit_test_pointer2 = 0;
  unit_test_pointer3 = 0;
  
  unit_test_state1 = 0;
  unit_test_state2 = 0;
  unit_test_state3 = 0;

  pInit();
  rftest_init_events();
  cuStart(test_register);
}

void test_register(void)
{
  test_rfcomm_conn_test();
}
#endif
