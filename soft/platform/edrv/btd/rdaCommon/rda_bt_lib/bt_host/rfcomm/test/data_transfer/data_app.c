#include <stdio.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <tcl.h>
#include <tk.h>
#include <pthread.h>

/* set up the ports */
#define PORT_A1 "/dev/ttyp5"
#define PORT_A2 "/dev/ttyp6"

static Tcl_Interp *interp;
static Tk_Window mainWindow;

void *app_send_data(void *arg);

void sigio_handler(int);
struct port_info {
  int id;
  int fd;
  pthread_t thread_id;
  char *portname;
  int tx_count;
  int rx_count;
  time_t rx_start;
} pinfo[3];

void app_data_call_back(struct port_info *pinfo);

int main(int argc, char **argv)
{  
  int x,i;
  int mode;
  if (argc!=2)
    {
      fprintf(stderr, "usage: intel_app role\n");
      exit(-1);
    }
  
  for (i=0; i<3; i++)
    pinfo[i].id=i;
  mode=atoi(argv[1]);
  pinfo[0].fd=app_open_port(PORT_A1);

  app_sys_init();
  if (mode==0)
    {
      char source_cmd[256];
      interp=Tcl_CreateInterp();
      mainWindow=Tk_CreateWindow( interp , NULL, "Intel Stress Test Application", ":0.0");
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
      
      sprintf(source_cmd, "source /home/airlied/devel/intel_app/test_app.tcl");
      Tcl_Eval(interp,source_cmd); 
      Tk_MainLoop();
    }
  else
    {
      spawn_send_threads();
      pthread_exit(NULL);
    }
  
}

int spawn_send_threads(void)
{
  int i;
  
  for (i=0; i<1; i++)
    pthread_create(&pinfo[i].thread_id, NULL, app_send_data, (void *)&pinfo[i]);

}

int app_open_port(char *portname)
{
  int port_fd;
  struct termios oldtio, newtio;
  
  port_fd=open(portname, O_RDWR);
  if (port_fd==-1)
    {
      perror("opening port\n");
      exit(-1);
    }
  
  fcntl(port_fd, F_SETOWN, getpid());
  fcntl(port_fd, F_SETFL, O_ASYNC|O_RDWR);
  
  tcgetattr(port_fd, &oldtio);
  memset(&newtio, 0, sizeof(newtio));
  newtio.c_cflag = CLOCAL | CS8 | CREAD;
  newtio.c_iflag = IGNPAR;
  newtio.c_oflag = 0;
  newtio.c_lflag = 0;
  
  tcflush(port_fd, TCIFLUSH);
  tcsetattr(port_fd, TCSANOW, &newtio);

  return port_fd;
}

void *app_send_data(void *arg)
{
  int i,j;
  char x=0;
  char buf[128];
  int ret, tot,len;
  struct port_info *pinfo=(struct port_info *)arg;

  for (i=0; i<3000; i++)
    {
      /* Send packet */
      for (j=0; j<80; j++)
    buf[j]=x++;
      tot=0;
      len=80;
      do {
    ret=write(pinfo->fd, buf, len-tot);
    if (ret<0)
      {
        sched_yield();
        ret=0;
      }
    tot+=ret;
      } while(tot<len);
      pinfo->tx_count+=len;
      //      printf("Sent %d: %d\n",pinfo->id, pinfo->tx_count);
    }
  return NULL;
}

int app_sys_init(void)
{
  struct sigaction sigio_act;
  sigset_t my_sigset;

  sigio_act.sa_handler=sigio_handler;
  sigio_act.sa_flags=SA_RESTART;
  sigaction(SIGIO, &sigio_act, NULL);
  return 0;
}

void app_data_call_back(struct port_info *pinfo)
{
  unsigned char buf[128];
  char cmd_buf[256];
  int ret,x;
  time_t tlen;
  float rate;

  if (pinfo->rx_count==0)
    pinfo->rx_start=time(NULL);

  ret=read(pinfo->fd, buf, 80);
  
  if (ret>0)
    {
      //    printf("ret: %d ", ret);
      //  for (x=0; x<ret; x++)
      //      printf("%02X ", buf[x]);
      pinfo->rx_count+=ret;
      /* Divide rx_count by total */
      sprintf(cmd_buf,"progset %d %d %d", pinfo->id+1, pinfo->rx_count, 240000);
      Tcl_Eval(interp,cmd_buf);
      tlen=time(NULL)-pinfo->rx_start;
      if (tlen==0)
    rate=0.0;
      else
    rate=pinfo->rx_count/tlen;

      sprintf(cmd_buf,".l%d configure -text \"time: %d rate: %f K/sec\"", pinfo->id+1, tlen, rate);
      Tcl_Eval(interp,cmd_buf);
      //      printf("Recv: %d: %d\n", pinfo->id, pinfo->rx_count);
    }
  return;
  

}

void sigio_handler(int signum)
{

  int x,ret, max;
  fd_set rfds;
  struct timeval myt;
  int i;
  myt.tv_sec=0;
  myt.tv_usec=0;
  
  do {
    FD_ZERO(&rfds);
    for (i=0; i<1; i++)
      FD_SET(pinfo[i].fd, &rfds);
    
    ret=select(pinfo[0].fd+1, &rfds, NULL, NULL, &myt);
    if (ret==-1)
      {
      }
    else
      {
    if (ret!=0)
      {
        for (i=0; i<3; i++)
          if (FD_ISSET(pinfo[i].fd, &rfds))
        app_data_call_back(&pinfo[i]);
      }
      }

  } while(ret!=0);
  return;
}
