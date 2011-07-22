#include "host_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <pthread.h>
#include <termios.h>
#include "host_types.h"
#include "host_error.h"
#include "papi.h"

extern int linux_sys_close;

static struct test_ll_fd_list {
  struct test_ll_fd_list *next;
  int fd;
  int enabled;
} *test_system_ll_head;


void *test_sys_upper_io_thread_handler(void *args)
{
 fd_set rfds;
  struct timeval myt;
  struct test_ll_fd_list *trav;
  int ret,max;
  while(linux_sys_close!=1)
    {
      FD_ZERO(&rfds);
      /* Add the list of currently opened ports */
      if (test_system_ll_head!=pNULL)
    {
      trav=test_system_ll_head;
      do {
        if (trav->enabled)
          {
        FD_SET(trav->fd, &rfds);
        if (trav->fd>max)
          max=trav->fd+1;
          }
        trav=trav->next;
      } while (trav!=pNULL);
    }
      /* set a timeout */
      myt.tv_sec=10;
      ret=select(max, &rfds, NULL, NULL, &myt);
      if (ret==-1)
    {
      //    perror("Selecting\n");
    }
      else
    {
      if (ret!=0) {
        /* check the pty fds ... linked list */
        if (test_system_ll_head!=pNULL)
          {
        trav=test_system_ll_head;
        do {
          if (FD_ISSET(trav->fd, &rfds))
            test_port_ent_data_req(trav->fd);
          trav=trav->next;
        } while (trav!=pNULL);
          }
      }
    }
      
      
    }
  


}


int test_system_add_fd(int fd)
{
  struct test_ll_fd_list *trav;
  struct test_ll_fd_list *new_ent;

  trav=test_system_ll_head;

  new_ent=(struct test_ll_fd_list *)pMalloc(sizeof(struct test_ll_fd_list));

  new_ent->fd=fd;
  new_ent->next=pNULL;
  new_ent->enabled=1;
  if (trav==pNULL)
    {
      test_system_ll_head=new_ent;
    }
  else
    {
      while(trav->next!=pNULL)
    trav=trav->next;
      trav->next=new_ent;
    }
  return BT_NOERROR;
}

int test_system_rem_fd(int fd)
{
  struct test_ll_fd_list *trav, *prev;
  
  if (test_system_ll_head==pNULL)
    return BT_NOERROR;

  prev=trav=test_system_ll_head;
  while(trav->next!=NULL && trav->fd!=fd)
    {
      prev=trav;
      trav=trav->next;
    }

  if (trav->fd==fd)
    {
      if (trav==test_system_ll_head)
    {
      test_system_ll_head=trav->next;
    }
      else
    {
      prev->next=trav->next;
    }
      pFree(trav);
      return BT_NOERROR;
    }
  return BT_NOERROR;

  
}

int test_system_set_enb_flag(int fd, int enabled)
{
  struct test_ll_fd_list *trav;
  
  trav=test_system_ll_head;
  if (trav==NULL)
    return 0;

  while(trav->next!=NULL && trav->fd!=fd)
    {
      trav=trav->next;
    }

  if (trav->fd==fd)
    {
      trav->enabled=enabled;
    } 
}
