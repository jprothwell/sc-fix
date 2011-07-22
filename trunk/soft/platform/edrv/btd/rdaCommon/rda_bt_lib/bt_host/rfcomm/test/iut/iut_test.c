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

t_api tester_port_remote_callback(prh_t_rfcomm_schan schan, t_bdaddr bdaddr, u_int8 state)
{
  if (state==PRH_PORT_ENT_DISCONNECT)
    {
      pDebugPrintf((pLOGNOTICE,"APP: Remote Device %s disconnected us from remote schan %d\n", prh_mgr_printBDAddress(bdaddr), schan));
    }
  
  return BT_NOERROR;
}

t_api tester_port_local_callback(prh_t_rfcomm_schan schan, t_bdaddr bdaddr, u_int8 state)
{
  if (state==PRH_PORT_ENT_CONNECT)
    {
      pDebugPrintf((pLOGNOTICE,"APP: Got Connect on schan %d from %s\n", schan, prh_mgr_printBDAddress(bdaddr)));
      
      // tester_multi_inst=(((struct prh_rfcomm_pe_port *)portid)->multi_inst);
    }
  else if (state==PRH_PORT_ENT_DISCONNECT)
    {
      pDebugPrintf((pLOGNOTICE,"APP: Disconnected from schan %d from %s\n", schan, prh_mgr_printBDAddress(bdaddr)));
    }

  return 0;
}


extern int (*rfcomm_conform_test[])(void);

t_bdaddr bd_addr_inq;
pSem_t syssem;

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
void *test_sys_upper_io_thread_handler(void *args);

int main(int argc, char **argv)
{
  pthread_t my_upper_handler;
  if (argc<2)
    {
      fprintf(stderr, "usage: iut_test <test_num>\n");
      exit(-1);
    }
  
  test_num=atoi(argv[1])+1;
  linux_sys_mode=0;
  
  fprintf(stderr, "IUT: RFCOMM Test\n");
  sys_init(linux_sys_mode);
  pthread_create(&my_upper_handler, NULL, test_sys_upper_io_thread_handler, NULL);
         
  host_init(linux_sys_mode);
 
 TEST_PE_Init();
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
 reg_port();
 /* Wait for tester to connect */
 while(prh_mgr_isDeviceConnected(bd_addr_inq)==FALSE);

 /* run the test */ 
 (*rfcomm_conform_test[test_num-1])();
 while(1);
}

int reg_port(void)
{
  prh_t_rfcomm_schan schan=2;
  int ret;

  ret=TEST_PE_SrvRegisterPort("/dev/ptyp5", &schan, &tester_port_local_callback);
}
