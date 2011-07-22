/***********************************************************************
 *
 * MODULE NAME:    test_def.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Test Definition Structure
 * MAINTAINER:     David Airlie
 * CREATION DATE:  Oct 2000
 *
 * SOURCE CONTROL: $Id: test_def.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000 Parthus Technologies Inc.
 *     All rights reserved.
 *
 ***********************************************************************/

#ifndef TEST_DEF_H
#define TEST_DEF_H

enum prh_test_type
{
  TEST_SDP=1,
  TEST_RFCOMM=2,
  TEST_L2CAP=3,
  COMM_RFCOMM=4,
  TEST_SECURITY=5
};


struct prh_st_test_def {
  char *name;
  int (*run)(int);
  u_int32 result;
};

struct prh_st_test_group {
  enum prh_test_type category;
  struct prh_st_test_def *tests;
  u_int16 num_tests;
  char *tcl_var_name;
  char *execute_command;
};

#define PRH_MAX_TESTS 255
#endif
