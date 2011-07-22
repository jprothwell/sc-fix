/*
 * DESCRIPTION:    Manager Test Code
 * AUTHOR:         Thomas Howley
 *
 * SOURCE CONTROL: $Id: tester_gap.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */
#include <stdio.h>

#include "tester_gap.h"
#include "bt.h"


/* #define LOCAL_BDADDR "0x00D0B7031651" */

int linux_sys_close;
int linux_sys_mode;
int test_num;
extern int (*gap_tester_conform_test[])(void);

int main(int argc, char **argv)
{
  
  if (argc < 2)
    {
      fprintf(stderr, "usage: iut_gap <test_num>");
      exit(-1);
    }

  test_num=atoi(argv[1]);
  linux_sys_mode=0;


  
  fprintf(stderr, "TESTER: GAP TEST No. %d\n", test_num);
  sys_init(linux_sys_mode); 
  
  fprintf(stderr,"starting the stack\n");

  MGR_Init(0);
  (*gap_tester_conform_test[test_num-1])();

  while(1);

}
