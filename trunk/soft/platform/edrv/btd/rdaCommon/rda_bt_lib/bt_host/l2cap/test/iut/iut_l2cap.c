/*
 * DESCRIPTION:    L2CAP Test Code
 * AUTHOR:         Gary Fleming
 *
 * SOURCE CONTROL: $Id: iut_l2cap.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */
#include <stdio.h>

#include "bt.h" 
#include "l2cap_iut_tester.h"
/* #include "mgr_core.h" */

#define TESTER_BDADDR "0x00D0B7031728" 

/* #define LOCAL_BDADDR "0x00D0B7030ECB"  */
/* #define TESTER_BDADDR "0x00D0B7031651" */

int linux_sys_close;
int linux_sys_mode;
int test_num;
/* extern int (*gap_iut_conform_test[])(void); */


int main(int argc, char **argv)
{
  t_bdaddr testerBdAddr;
  
  if (argc < 2)
    {
      fprintf(stderr, "usage: iut_l2cap <test_num>");
      exit(-1);
    }

  test_num=atoi(argv[1]);
  linux_sys_mode=0;


  
  fprintf(stderr, "L2CAP IUT: TEST No. %d", test_num);
  sys_init(linux_sys_mode); 
  
  fprintf(stderr,"L2CAP IUT: starting the stack\n");
  MGR_Init(0);

  
  l2_init_iut_tester();
  MGR_ParseHexBDAddress(TESTER_BDADDR, &testerBdAddr); 
  l2_iut_tester(test_num, testerBdAddr);

  while(1);

}
