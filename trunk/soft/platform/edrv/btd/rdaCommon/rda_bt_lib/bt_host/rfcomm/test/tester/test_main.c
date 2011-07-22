#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include "host_types.h"
#include "papi.h"
#include "mgr_core.h"
#include "mgr_utils.h"
#include <tcl.h>
#include <tk.h>
#include <errno.h>

#define NUM_RFCOMM_TESTS 19

static Tcl_Interp *interp;
static Tk_Window mainWindow;

#define BD_ADDR_SERVER "\x51\x16\x03\xb7\xd0\x00"
#define BD_ADDR_SERV2 BD_ADDR_SERVER
#define BD_ADDR_CLIENT BD_ADDR_SERVER
int app_manager_inquiry_cb(tid_t transactionId, t_BT_DeviceEntry *deviceInfo);

int baseband_connect(ClientData , Tcl_Interp *, int , char *[]);
extern int do_l2cap_tests(ClientData , Tcl_Interp *, int , char *[]);
extern int do_rfcomm_tests(ClientData , Tcl_Interp *, int , char *[]);
extern int do_rfcomm_comms(ClientData , Tcl_Interp *, int , char *[]);
extern int do_sdp_tests(ClientData , Tcl_Interp *, int , char *[]);
int do_inquiry(ClientData clientdata, Tcl_Interp *interp, int argc, char *argv[]);
extern int tcl_sdp_query(ClientData , Tcl_Interp *, int , char *[]);
extern int tcl_rfcomm_query(ClientData, Tcl_Interp *, int, char *[]);
int test_bdaddress(ClientData clientdata, Tcl_Interp *interp, int argc, char *argv[]);
t_bdaddr bd_addr_serv, bd_addr_cli, bd_addr_inq;
t_bdaddr local_addr;
int linux_sys_close;
int linux_sys_mode;
void *pe_rfcomm_conn_handle;
int rfcomm_state;
int sys_sema;

extern int (*rfcomm_conform_test[])(int);

int host_init(int mode)
{
  char **x;

  x=(char **)malloc(3*sizeof(char *));
  x[1]=malloc(30);
  
  
  if (mode==0)
    strcpy(x[1],"/addr:112233445566");
  else
    strcpy(x[1],"/addr:112233445577");

  memcpy(bd_addr_serv.bytes, BD_ADDR_SERVER, 6); 
  memcpy(bd_addr_cli.bytes, BD_ADDR_CLIENT, 6);
  memcpy(bd_addr_inq.bytes, BD_ADDR_SERV2, 6);
  

  MGR_InitStack(2,x);

  //  PE_Init();
  return 0;
}


int main(int argc, char **argv)
{
  struct sembuf sop;
  int ch;

  int ret;
  char source_cmd[256];
  if (argc!=2)
    {
      fprintf(stderr, "usage: ppp_test mode\n");
      exit(-1);
    }
  sys_sema=semget(IPC_PRIVATE, 10, 511|IPC_CREAT);
  sop.sem_num=0;
  sop.sem_op=0;
  sop.sem_flg=0;
  ret=semop(sys_sema, &sop, 1);
  sop.sem_num=1;
  sop.sem_op=0;
  sop.sem_flg=0;
  ret=semop(sys_sema, &sop, 1);
  if (ret!=0)
    exit(-1);
  linux_sys_mode=atoi(argv[1]);

  sys_init(linux_sys_mode);
  host_init(linux_sys_mode);
  rftest_init();
  //  rfcomm_test_init(linux_sys_mode);
  if (1)
    {
      interp=Tcl_CreateInterp();
      mainWindow=Tk_CreateWindow( interp , NULL, "Tests", ":0.0");
      if (mainWindow == NULL) {
    fprintf(stderr,"%d\n", interp->result);
    exit(1);
      }
      
      if (Tcl_Init(interp) == TCL_ERROR) {
    fprintf(stderr,"Tcl_Init failed: %d\n", interp->result);
    exit(1);
      }
      
      if (Tk_Init(interp) == TCL_ERROR) {
    fprintf(stderr,"Tk_Init failed: %d\n ",interp->result);
    exit(1);
      }

      Tcl_CreateCommand(interp, "baseband_connect", baseband_connect, (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
      Tcl_CreateCommand(interp, "ccode_inquiry", do_inquiry, (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
      Tcl_CreateCommand(interp, "ccode_bdaddr", test_bdaddress, (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
      Tcl_CreateCommand(interp, "do_rfcomm_tests", do_rfcomm_tests, (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
      sprintf(source_cmd, "source ./test_app.tcl");
      Tcl_Eval(interp,source_cmd);
      local_addr=MGR_GetLocalAddress();
      sprintf(source_cmd, ".f7.f1.l2 configure -text %s", 
          MGR_PrintLargeNumber(local_addr.bytes, 6));
      Tcl_Eval(interp,source_cmd);
    }
  
  fprintf(stderr, "TESTER: System Test");
  if (linux_sys_mode==0)
    fprintf(stderr,": Dev A\n");
  else
    fprintf(stderr,": Dev B\n");
  
  if (1)
    Tk_MainLoop();
  else
    {
      while(1)
    {
      waitfor_rfcomm_connected();
      waitfor_rfcomm_disconn();
    }
    }

  exit(-1);
}

pSem_t syssem;

int do_inquiry(ClientData clientdata, Tcl_Interp *interp, int argc, char *argv[])
{
  
 syssem=pSemCreate(0);
 
 if (linux_sys_mode==1)
   {
     MGR_Inquiry(5, BT_GIAC_LAP, 1, app_manager_inquiry_cb); 
     
     pSemLock(syssem,0);
   }
 return TCL_OK;

}

int test_bdaddress(ClientData clientdata, Tcl_Interp *interp, int argc, char *argv[])
{
  char cmd[256];

  char *tclvar;

  tclvar=Tcl_GetVar(interp, "rem_bdaddr", TCL_GLOBAL_ONLY);

  bd_addr_serv=MGR_ReadHexBDAddress(tclvar);
  sprintf(cmd, "destroy .gbd");
  Tcl_Eval(interp, cmd);
  return TCL_OK;
}


int do_rfcomm_tests(ClientData clientdata, Tcl_Interp *interp, int argc, char *argv[])
{
  int tests[NUM_RFCOMM_TESTS];
  int i;
  char *tclvar;
  char varname[30];
  char cmd[256];

  for (i=0; i<NUM_RFCOMM_TESTS; i++)
    {
      sprintf(varname, "rf_test%d", i);
      tclvar=Tcl_GetVar(interp,varname, TCL_GLOBAL_ONLY);
      if (tclvar!=NULL)
    tests[i]=atoi(tclvar);
    }

  fprintf(stderr,"Executing RFCOMM:");
  for (i=0; i<NUM_RFCOMM_TESTS; i++)
    if (tests[i]==1)
      fprintf(stderr,"%d ", i);
  fprintf(stderr, "\n");
  
  for (i=0; i<NUM_RFCOMM_TESTS; i++)
    if (tests[i])
      (*(rfcomm_conform_test)[i])(0);
  
  return TCL_OK;
}

void connect_Callback(tid_t transid, t_api result)
{
  fprintf(stderr,"ACL established\n");
}

int baseband_connect(ClientData clientdata, Tcl_Interp *interp, int argc, char *argv[])
{
  int ret;
  u_int16 handle;
  tid_t transid=10;
  ret=MGR_CreateACL(bd_addr_inq, 0, &handle, connect_Callback, transid);
  return TCL_OK;
}

int waitfor_system_sem(void)
{
  struct sembuf sop;
  int ret;
  
  sop.sem_num=1;
  sop.sem_op=-1;
  sop.sem_flg=0;
  
  do {
    ret=semop(sys_sema, &sop, 1);
  } while (ret==-1 && errno==EINTR);
  return 0;
}

int waitfor_rfcomm_connected(void)
{
  struct sembuf sop;
  int ret;
  sop.sem_num=1;
  sop.sem_op=-1;
  sop.sem_flg=0;
  
  do {
    ret=semop(sys_sema, &sop, 1);
  } while (ret==-1 && errno==EINTR);
  
  return 0;
}

int sys_setw4sem(void)
{
  int ret;
  struct sembuf sop;
  sop.sem_num=1;
  sop.sem_op=1;
  sop.sem_flg=0;
  
  ret=semop(sys_sema, &sop, 1);
};

int waitfor_rfcomm_disconn(void)
{
  struct sembuf sop;
  int ret;
  sop.sem_num=1;
  sop.sem_op=-1;
  sop.sem_flg=0;
  
  do {
    ret=semop(sys_sema, &sop, 1);
  } while (ret==-1 && errno==EINTR);
  
  return 0;
}

int app_manager_inquiry_cb(tid_t transactionId, t_BT_DeviceEntry *deviceInfo)
{
  char cmd[256];
  if (deviceInfo)
    {
      sprintf(cmd, ".inq.f1.l1 insert end %s", MGR_PrintLargeNumber(deviceInfo->bdAddress.bytes, 6));
      Tcl_Eval(interp, cmd);
      memcpy(bd_addr_inq.bytes, deviceInfo->bdAddress.bytes, 6);
      
    }
  else
      pSemUnlock(syssem);
    
}

