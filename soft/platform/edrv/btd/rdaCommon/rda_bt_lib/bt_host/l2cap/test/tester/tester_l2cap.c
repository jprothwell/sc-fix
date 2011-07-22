/*
 * DESCRIPTION:    L2CAP Test Code
 * AUTHOR:         Gary Fleming
 *
 * SOURCE CONTROL: $Id: tester_l2cap.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */
#include <stdio.h>

#include "bt.h"
#include "l2cap_lower_tester.h"

#define IUT_BDADDR "0x00D0B7030E52" 

/* #define LOCAL_BDADDR "0x00D0B7030ECB"  */
/* #define TESTER_BDADDR "0x00D0B7031651" */

int linux_sys_close;
int linux_sys_mode;
int test_num;
/* extern int (*gap_iut_conform_test[])(void); */


int main(int argc, char **argv)
{
  t_bdaddr iutBdAddr;
  
  if (argc < 2)
    {
      fprintf(stderr, "usage: tester_l2cap <test_num>");
      exit(-1);
    }

  test_num=atoi(argv[1]);
  linux_sys_mode=0;


  
  fprintf(stderr, "L2CAP TESTER: TEST No. %d", test_num);
  sys_init(linux_sys_mode); 
  
  fprintf(stderr,"L2CAP TESTER: starting the stack\n");
  MGR_Init(0);

  
  l2_init_lower_tester();
  MGR_ParseHexBDAddress(IUT_BDADDR, &iutBdAddr); 
  l2_lower_tester(test_num, iutBdAddr);

  while(1);

}
