#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>
#include "host_types.h"
#include "host_buf.h"
#include "mgr_core.h"
#include "rfcomm_ue_api.h"

int linux_sys_close;
int linux_sys_mode;

t_bdaddr bd_addr_inq;
pSem_t syssem;
extern int (*rfcomm_conform_test[])(void);
int host_init(int mode)
{
  char **x;

  x=(char **)malloc(3*sizeof(char *));
  x[1]=malloc(30);
  
  
  if (mode==0)
    strcpy(x[1],"/addr:112233445566");
  else
    strcpy(x[1],"/addr:112233445577");

  prh_mgr_initStack(2,x);

  return 0;
}


void app_manager_inquiry_cb(t_BT_DeviceEntry *deviceInfo, t_api status)
{
  if (deviceInfo)
    {
      memcpy(bd_addr_inq.bytes, deviceInfo->bdAddress.bytes, 6);
    
    }
  else
      pSemUnlock(syssem);
    
}

int test_num;

int main(int argc, char **argv)
{
 if (argc<2)
    {
      fprintf(stderr, "usage: rfcomm_test <test_num>\n");
      exit(-1);
    }

 test_num=atoi(argv[1])+1;
 linux_sys_mode=1;

 fprintf(stderr, "IUT: RFCOMM Test\n");
 pInit();
 sys_init(linux_sys_mode);
 host_init(linux_sys_mode);
 rftest_init();
 // TEST_PE_Init();
#if 0
 memcpy(bd_addr_inq.bytes, "\xFF\xEE\xDD\xCC\xBB\xAA", 6);
#else
 syssem=pSemCreate(0);
 
 if (linux_sys_mode==1)
   {
     MGR_Inquiry(5, GIAC_LAP, 0, app_manager_inquiry_cb); 
     
     pSemLock(syssem,0);
     fprintf(stderr,"Inquiry Complete%s\n", prh_mgr_printLargeNumber(bd_addr_inq.bytes, 6));
     
   }
#endif
 // reg_port();
 /* Wait for tester to connect */
 // while(MGR_IsDeviceConnected(bd_addr_inq)==FALSE);
 baseband_connect();

 (*rfcomm_conform_test[test_num-1])();
 /* run the test */ 
 while(1);
}

t_api tester_port_local_callback(prh_t_rfcomm_schan schan, t_bdaddr bdaddr, u_int8 state)
{
  if (state==PRH_PORT_ENT_CONNECT)
    {
      pDebugPrintf((pLOGNOTICE,"APP: Got Connect on for schan %d\n", schan));
     }
  else if (state==PRH_PORT_ENT_DISCONNECT)
    {
      pDebugPrintf((pLOGNOTICE,"APP: Disconnected from port %s for schan %d\n", schan));
    }
  
  return 0;
}

int reg_port(void)
{
  prh_t_rfcomm_schan schan=2;
  int ret;

  ret=PE_SrvRegisterPort("/dev/ptyr4", &schan, tester_port_local_callback);
}

pSem_t acl_link_sema;
void connect_Callback(tid_t transid, t_api result)
{
  fprintf(stderr,"ACL established\n");
  pSemUnlock(acl_link_sema);
}

int baseband_connect(void)
{
  int ret;
  u_int16 handle;
  tid_t transid=1;

  acl_link_sema=pSemCreate(0);
  ret=prh_mgr_createACL(bd_addr_inq, 0, &handle, connect_Callback, transid);
  pSemLock(acl_link_sema, 0);
  pSemFree(acl_link_sema);
  return 0;
}
