#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>
#include "conformance_tests.h"

struct groupEntry upper_rfcomm_conformance_group;

int test_mode;
t_bdaddr bd_addr_inq;
t_pEvent sys_event;

int host_init(int mode)
{
  int ret;
  do {
    ret=MGR_Init(0);
    if (ret==BT_RETRY)
      sleep(1);
  } while(ret==BT_RETRY);
  
  PE_Init();
  return 0;
}


void app_manager_inquiry_cb(t_MGR_DeviceEntry *deviceInfo, int status)
{
  if (deviceInfo)
    {
      memcpy(bd_addr_inq.bytes, deviceInfo->address.bytes, 6);
    
    }
  else
      pEventSet(sys_event);
    
}

int test_num;
void *test_sys_upper_io_thread_handler(void *args);

int main(int argc, char **argv)
{
  int ret;
  int i;
  //  pthread_t my_upper_handler;
  if (argc<2)
    {
      fprintf(stderr, "usage: iut_test <test_num>\n");
      exit(-1);
    }
  
  test_num=atoi(argv[1])+1;
  test_mode=0;
  
  fprintf(stderr, "IUT: RFCOMM Test\n");
  sys_init(test_mode);
  //  pthread_create(&my_upper_handler, NULL, test_sys_upper_io_thread_handler, NULL);
         
  host_init(test_mode);
 
#if 1 
  memcpy(bd_addr_inq.bytes, "\x02\x00\xDB\xDB\xDB\xDB", 6);
#else
 sys_event=pEventCreate(FALSE);
 
 if (test_mode==1)
   {
     MGR_Inquiry(5, GIAC_LAP, 0, app_manager_inquiry_cb); 
     
     pEventWait(sys_event,0);
     fprintf(stderr,"Inquiry Complete%s\n", prh_mgr_printLargeNumber(bd_addr_inq.bytes, 6));
     
   }
#endif
 /* reg_port();*/
 prh_tester_reg_port();
 /* Wait for tester to connect */

 
#if 0
 for (i=0; i<17; i++)
   {
     if (!(upper_rfcomm_conformance_group.testList[i].flags & TEST_NOT_BATCHED))
       {
	 while(prh_mgr_isDeviceConnected(bd_addr_inq)==FALSE);
	 fprintf(stderr,"Running Test %s\n", upper_rfcomm_conformance_group.testList[i].testName);
	 /* run the test */ 
	 ret=(upper_rfcomm_conformance_group.testList[i].testFunc)(bd_addr_inq, 0, 0);
	 switch(ret)
	   {
	   case TEST_PASS:
	     fprintf(stderr,"Test %d:%s passed\n", i, upper_rfcomm_conformance_group.testList[i].testName);
	     break;
	   case TEST_FAIL:
	     fprintf(stderr,"Test %d failed\n", i);
	     break;
	   case TEST_UNKNOWN:
	     fprintf(stderr,"Test %d unknown\n", i);
	     break;
	   }

       }
   }
#endif
 while(1);
}

int Test_Print(char *formatString, ...) 
{
  char printBuffer[4096];
  int i;
  va_list args;

  va_start (args, formatString);
  i=sprintf(printBuffer, "%s", "TEST_OUT:");
  
  vsnprintf(printBuffer+i, sizeof(printBuffer)-strlen(printBuffer)-3, formatString, args);
  printBuffer[sizeof(printBuffer)-3]='\0';
  va_end(args);

  fputs(printBuffer, stderr);
  return BT_NOERROR;
}
