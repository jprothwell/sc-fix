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

t_bdaddr bd_addr_inq;
t_pEvent sys_event;
extern struct groupEntry lower_rfcomm_conformance_group; 
int test_mode;

int host_init(int mode)
{
  int ret;
  do {
    ret=MGR_Init(0);
    if (ret==BT_RETRY)
      sleep(1);
  } while(ret==BT_RETRY);
  HCI_WriteScanEnable(3);
  HCI_WritePageTimeout(0xffff);
  return 0;
}


void app_manager_inquiry_cb(t_MGR_DeviceEntry *deviceInfo, u_int8 status)
{
  if (deviceInfo)
    {
      memcpy(bd_addr_inq.bytes, deviceInfo->address.bytes, 6);
    
    }
  else
      pEventSet(sys_event);
    
}

int test_num;

int main(int argc, char **argv)
{
  int ret;
  int i;
 if (argc<2)
    {
      fprintf(stderr, "usage: rfcomm_test <test_num>\n");
      exit(-1);
    }

 test_num=atoi(argv[1])+1;
 test_mode=1;

 fprintf(stderr, "IUT: RFCOMM Test\n");
 sys_init(test_mode);
 host_init(test_mode);
 rftest_init();
 // TEST_PE_Init();
#if 0
 memcpy(bd_addr_inq.bytes, "\xFF\xEE\xDD\xCC\xBB\xAA", 6);
#else
 sys_event=pEventCreate(FALSE);
 
 if (test_mode==1)
   {
     MGR_Inquiry(5, BT_GIAC, 0, app_manager_inquiry_cb); 
     
     pEventWait(sys_event);
     fprintf(stderr,"Inquiry Complete%s\n", prh_mgr_printLargeNumber(bd_addr_inq.bytes, 6));
     
   }
#endif
 /* Wait for tester to connect */
 // while(MGR_IsDeviceConnected(bd_addr_inq)==FALSE);

#define RUN_ONE 1

#if RUN_ONE

 // baseband_connect();
 fprintf(stderr,"Running Test %s\n", lower_rfcomm_conformance_group.testList[test_num-1].testName);
 ret=(lower_rfcomm_conformance_group.testList[test_num-1].testFunc)(bd_addr_inq, 0, 0);
 switch(ret)
   {
   case TEST_PASS:
     fprintf(stderr,"Test %d:%s passed\n", test_num-1, lower_rfcomm_conformance_group.testList[test_num-1].testName);
     
     break;
   case TEST_FAIL:
     fprintf(stderr,"Test %d:%s failed\n", test_num-1, lower_rfcomm_conformance_group.testList[i].testName);
	     
     break;
   case TEST_UNKNOWN:
     fprintf(stderr,"Test %d:%s unknown\n", test_num-1, lower_rfcomm_conformance_group.testList[i].testName);
     
     break;
   }
#else
 for (i=0; i<17; i++)
   {
     if (!(lower_rfcomm_conformance_group.testList[i].flags & TEST_NOT_BATCHED))
       {
	 baseband_connect();
	 fprintf(stderr,"Running Test %s\n", lower_rfcomm_conformance_group.testList[i].testName);	 
	 ret=(lower_rfcomm_conformance_group.testList[i].testFunc)(bd_addr_inq, 0, 0);
	 switch(ret)
	   {
	   case TEST_PASS:
	     fprintf(stderr,"Test %d:%s passed\n", test_num-1, lower_rfcomm_conformance_group.testList[i].testName);
	     
	     break;
	   case TEST_FAIL:
	     fprintf(stderr,"Test %d:%s failed\n", test_num-1, lower_rfcomm_conformance_group.testList[i].testName);
	     
	     break;
	   case TEST_UNKNOWN:
	     fprintf(stderr,"Test %d:%s unknown\n", test_num-1, lower_rfcomm_conformance_group.testList[i].testName);
	     
	     break;
	   }
	 pDebugSleep(2);
       }
   }
 /* run the test */ 
#endif
 while(1)
   sleep(1000);
}

t_pEvent acl_link_event;
void connect_Callback(int transid, int result)
{
  fprintf(stderr,"ACL established\n");
  pEventSet(acl_link_event);
}

int baseband_connect(void)
{
  int ret;
  u_int16 handle;
  int transid=1;

  acl_link_event=pEventCreate(0);
  ret=prh_mgr_createACL(bd_addr_inq, 0, &handle, connect_Callback, transid);
  pEventWait(acl_link_event);
  pEventFree(acl_link_event);
  return 0;
}
