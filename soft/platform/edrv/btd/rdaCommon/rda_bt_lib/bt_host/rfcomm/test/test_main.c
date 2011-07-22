#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include <tcl.h>
#include <tk.h>
#include <errno.h>
#include <sys/wait.h>
#include "host_types.h"
#include "test_def.h"

#define NUM_RFCOMM_TESTS 19

static Tcl_Interp *interp;
static Tk_Window mainWindow;

//#define BD_ADDR_SERVER "\x51\x16\x03\xb7\xd0\x00"

extern int do_rfcomm_tests(ClientData , Tcl_Interp *, int , char *[]);
extern struct prh_st_test_group test_rfcomm_conform_group;

int tester_eval_tcl(Tcl_Interp *interp, char *cmd)
{

  int ret;
  //  fprintf(stderr,"%s\n",cmd);
  ret=Tcl_Eval(interp, cmd);
  if (ret!=TCL_OK)
    fprintf(stderr,"TCL: TCL Error\n");
}

int main(int argc, char **argv)
{
  int ch;

  int ret;
  char cmd[256];

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
  
  if (Tix_Init(interp) == TCL_ERROR) {
    fprintf(stderr,"Tix_Init failed: %d\n", interp->result);
    exit(1);
  }

  Tcl_CreateCommand(interp, "do_rfcomm_conf", do_rfcomm_tests, (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);  
  Tcl_EvalFile(interp,"./test_app.tcl");
  
  sprintf(cmd, "frames_init");
  tester_eval_tcl(interp,cmd);
  //  test_build_box(interp, &test_rfcomm_conform_group);
  test_build_combo(interp, &test_rfcomm_conform_group);
  test_pack_combo(interp, &test_rfcomm_conform_group);
  //  test_pack_box(interp, &test_rfcomm_conform_group);
  pack_main_frame();
  
  sprintf(cmd, "create_buttons");
  tester_eval_tcl(interp, cmd);

  fprintf(stderr, "TESTER: System Test");
    
  Tk_MainLoop();
  
  exit(-1);
}



int do_tests(struct prh_st_test_group *test_group, int data)
{
  char *tclvar;
  char varname[16];
  char testnum[3];
  int i;
  int tests_to_run[PRH_MAX_TESTS];
  
  sprintf(varname, "%s", test_group->tcl_var_name);
  tclvar=Tcl_GetVar(interp, varname, TCL_GLOBAL_ONLY);
  if (tclvar==NULL)
    return;
  testnum[0]=tclvar[10];
  testnum[1]=tclvar[11];
  testnum[2]='\0';
  i=atoi(testnum);
  fprintf(stderr,"%d ", i);
  
  (test_group->tests[i].run)(i);
  return TCL_OK;
}

int do_rfcomm_conform_test(int test_num)
{
  char cmd1[256], cmd2[256];
  pid_t child1, child2;
  char x[5];
  int exit_code1, exit_code2;
  int ret;
  int exiting=0;

#if 0
  sprintf(cmd1,"./iut/xterm_iut");
  sprintf(cmd2,"./tester/xterm_tester");
  sprintf(x, "%d", test_num);
  child1=fork();
  if (child1==0)
    {
      fprintf(stderr, "Execing xterm_iut %s\n", x);
      execlp("/usr/X11R6/bin/xterm", "xterm", "-e", "iut/rfcomm_iut", x, (char *)0);
      /* child */
      /* exec the other program */
    }
  else
    {
      sleep(2);
      child2=fork();
      if (child2==0)
    {
      fprintf(stderr, "Execing xterm_tester %s\n", x);
      execlp("/usr/X11R6/bin/xterm", "xterm", "-e", "tester/rfcomm_tester", x, (char *)0);
    }
      
    }
  /* */
  fprintf(stderr, "Tester spawned %d + %d\n", child1, child2);
  /* We have the two child processes now */
  do {
    ret=waitpid(child2, &exit_code1, WNOHANG);
    if (ret==0)
      Tcl_DoOneEvent(TCL_DONT_WAIT);
    else
      exiting=1;
  } while(!exiting);

  ret=kill(child1, SIGKILL);
  waitpid(child1, &exit_code2, 0);

  fprintf(stderr, "Test %d exited %d,%d, killed %d", test_num, exit_code1, exit_code2 , ret);
#else
  sprintf(cmd1, "init_frames");
  tester_eval_tcl(interp, cmd1);
  sprintf(cmd1, "start_iut %d", test_num);
  tester_eval_tcl(interp, cmd1);
  sleep(1);
  sprintf(cmd1, "start_tester %d", test_num);
  tester_eval_tcl(interp, cmd1);
#endif
}

struct prh_st_test_def test_rfcomm_conform_defs[] =
{
  { "TP/RFC/BV-01-C", do_rfcomm_conform_test, 0 },
  { "TP/RFC/BV-02-C", do_rfcomm_conform_test, 0 },
  { "TP/RFC/BV-03-C", do_rfcomm_conform_test, 0 },
  { "TP/RFC/BV-04-C", do_rfcomm_conform_test, 0 },
  { "TP/RFC/BV-05-C", do_rfcomm_conform_test, 0 },
  { "TP/RFC/BV-06-C", do_rfcomm_conform_test, 0 },
  { "TP/RFC/BV-07-C", do_rfcomm_conform_test, 0 },
  { "TP/RFC/BV-08-C", do_rfcomm_conform_test, 0 },
  { "TP/RFC/BV-09-C", do_rfcomm_conform_test, 0 },
  { "TP/RFC/BV-10-C", do_rfcomm_conform_test, 0 },
  { "TP/RFC/BV-11-C", do_rfcomm_conform_test, 0 },
  { "TP/RFC/BV-12-C", do_rfcomm_conform_test, 0 },
  { "TP/RFC/BV-13-C", do_rfcomm_conform_test, 0 },
  { "TP/RFC/BV-14-C", do_rfcomm_conform_test, 0 },
  { "TP/RFC/BV-15-C", do_rfcomm_conform_test, 0 },
  { "TP/RFC/BV-16-C", do_rfcomm_conform_test, 0 },
  { "TP/RFC/BV-17-C", do_rfcomm_conform_test, 0 },
  { "TP/RFC/BV-18-C", do_rfcomm_conform_test, 0 },
  { "TP/RFC/BV-19-C", do_rfcomm_conform_test, 0 }
};

struct prh_st_test_group test_rfcomm_conform_group = { COMM_RFCOMM, test_rfcomm_conform_defs, sizeof(test_rfcomm_conform_defs)/sizeof(struct prh_st_test_def), "rfconf" , "do_rfcomm_conf" };

int test_build_combo(Tcl_Interp *interp, struct prh_st_test_group *test_group)
{
  char cmd[1024];
  int i;
  enum prh_test_type category;
  char testtype[128];
  if (test_group->num_tests==0)
    return 0;

  sprintf(testtype,"RFCOMM Comm");
  
 sprintf(cmd, "frame .f%s -borderwidth 2 -relief ridge", test_group->tcl_var_name);
  tester_eval_tcl(interp,cmd);

  sprintf(cmd, "label .f%s.name -text \"%s\" -borderwidth 2 -relief raised", test_group->tcl_var_name, testtype);
  tester_eval_tcl(interp,cmd);
  sprintf(cmd, "button .f%s.b1 -text \"Execute %s\" -command %s", test_group->tcl_var_name, testtype, test_group->execute_command);
  tester_eval_tcl(interp,cmd);

  sprintf(cmd, "tixComboBox .f%s.cb1 -label \"Tester\" -editable false -variable %s", test_group->tcl_var_name, test_group->tcl_var_name);
  tester_eval_tcl(interp,cmd);

  
  for (i=0; i<test_group->num_tests; i++)
    {
      sprintf(cmd, ".f%s.cb1 insert end \"%s\"", test_group->tcl_var_name, test_group->tests[i].name);
      tester_eval_tcl(interp,cmd);
    } 
}

int test_pack_combo(Tcl_Interp *interp, struct prh_st_test_group *test_group)
{
  char cmd[1024];
  int i;
  int ret;
 
  sprintf(cmd, "pack .f%s.name", test_group->tcl_var_name);
  //   for (i=0; i<test_group->num_tests; i++)
  //    sprintf(cmd, "%s .f%s.r%d", cmd, test_group->tcl_var_name, i+1);
  
  //  sprintf(cmd, "%s .f%s.b1 -in .f%s", cmd, test_group->tcl_var_name, test_group->tcl_var_name);
  
  sprintf(cmd,"pack .f%s.name .f%s.cb1 .f%s.b1 -in .f%s", test_group->tcl_var_name, test_group->tcl_var_name,test_group->tcl_var_name,test_group->tcl_var_name );
  tester_eval_tcl(interp,cmd);
  
  sprintf(cmd, "pack .f%s -in .f1 -side left", test_group->tcl_var_name);
  tester_eval_tcl(interp,cmd);

  return 0;
}

int test_build_box(Tcl_Interp *interp, struct prh_st_test_group *test_group)
{
  char cmd[1024];
  int i;
  enum prh_test_type category;
  char testtype[128];
  if (test_group->num_tests==0)
    return 0;
  
  category=test_group->category;
  if (category==TEST_SDP)
    sprintf(testtype,"SDP");
  else if (category==TEST_RFCOMM)
    sprintf(testtype,"RFCOMM Test");
  else if (category==TEST_L2CAP)
    sprintf(testtype,"L2CAP");
  else if (category==COMM_RFCOMM)
    sprintf(testtype,"RFCOMM Comm");
  else if (category==TEST_SECURITY)
    sprintf(testtype, "SECURITY Test");
  
  sprintf(cmd, "frame .f%s -borderwidth 2 -relief ridge", test_group->tcl_var_name);
  tester_eval_tcl(interp,cmd);
  sprintf(cmd, "label .f%s.name -text \"%s\" -borderwidth 2 -relief raised", test_group->tcl_var_name, testtype);
  tester_eval_tcl(interp,cmd);
  sprintf(cmd, "button .f%s.b1 -text \"Execute %s\" -command %s", test_group->tcl_var_name, testtype, test_group->execute_command);
  tester_eval_tcl(interp,cmd);


  for (i=0; i<test_group->num_tests; i++)
    {
      sprintf(cmd, "checkbutton .f%s.r%d -text \"%s\" -variable %s%d", test_group->tcl_var_name, i+1, test_group->tests[i].name, test_group->tcl_var_name, i+1);
      tester_eval_tcl(interp,cmd);
    }
  
  
}

int pack_main_frame(void)
{
  char cmd[256];
  sprintf(cmd, "pack .f1");
  tester_eval_tcl(interp, cmd);

  //  sprintf(cmd, "pack .f4");
  //  tester_eval_tcl(interp, cmd);
  return 0;
}

int test_pack_box(Tcl_Interp *interp, struct prh_st_test_group *test_group)
{
  char cmd[1024];
  int i;
  int ret;
 
  sprintf(cmd, "pack .f%s.name", test_group->tcl_var_name);
   for (i=0; i<test_group->num_tests; i++)
    sprintf(cmd, "%s .f%s.r%d", cmd, test_group->tcl_var_name, i+1);
  
  sprintf(cmd, "%s .f%s.b1 -in .f%s", cmd, test_group->tcl_var_name, test_group->tcl_var_name);
  
  tester_eval_tcl(interp,cmd);
  
  sprintf(cmd, "pack .f%s -in .f1 -side left", test_group->tcl_var_name);
  tester_eval_tcl(interp,cmd);

  return 0;
}

int do_rfcomm_tests(ClientData data , Tcl_Interp *interp, int argc, char *argv[])
{
  do_tests(&test_rfcomm_conform_group, 0);
  return TCL_OK;
}
