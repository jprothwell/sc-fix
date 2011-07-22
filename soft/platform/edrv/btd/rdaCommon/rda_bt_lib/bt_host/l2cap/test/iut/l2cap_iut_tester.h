/*
 * DESCRIPTION:    L2CAP Test Code
 * AUTHOR:         Gary Fleming
 *
 * SOURCE CONTROL: $Id: l2cap_iut_tester.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */
#ifndef __L2_TESTER
#define __L2_TESTER

#define L2_COS_CED_BV_01   1
#define L2_COS_CED_BV_02   2
#define L2_COS_CED_BV_03   3
#define L2_COS_CED_BV_04   4
#define L2_COS_CED_BV_05   5
#define L2_COS_CED_BV_06   6
#define L2_COS_CED_BV_07   7
#define L2_COS_CED_BV_08   8
#define L2_COS_CED_BV_09   9

#define L2_COS_CFD_BV_01   11
#define L2_COS_CFD_BV_02   12
#define L2_COS_CFD_BV_03   13
#define L2_COS_CFD_BV_04   14

#define L2_COS_ECH_BV_01   21
#define L2_COS_ECH_BV_02   22

#define L2_COS_RCO_BV_01   31
#define L2_COS_RCO_BI_01   32

#define L2_COS_TMH_BV_01   41
#define L2_COS_TMH_BV_02   42





void l2_init_iut_tester();
int l2_iut_tester(u_int8 test_case, t_bdaddr target_addr);

#endif
