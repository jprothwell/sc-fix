/***********************************************************************
 *
 * MODULE NAME:    host_ll.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    HOST Generic Linked List Header
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  21 Aug 2000
 *
 * SOURCE CONTROL: $Id: host_ll.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    15.Mar.2000 -   DA - first import
 *
 *
 * ISSUES:
 *
 ***********************************************************************/

#ifndef PRH_HOST_LL_H
#define PRH_HOST_LL_H

typedef int (*prh_host_t_cmp_funcptr)(void *cmpin, void *compwith);
typedef int (*prh_host_t_exe_funcptr)(void *exe_data, void *func_info);
struct prh_host_gen_ll 
{
  struct prh_host_gen_ll *next;
  void *data;
  u_int8 flag;
};


int prh_host_ll_add(struct prh_host_gen_ll **list, void *data);
int prh_host_ll_remove(struct prh_host_gen_ll **list, void **data, void *cmp_input, prh_host_t_cmp_funcptr cmp_funcptr);
int prh_host_ll_search(struct prh_host_gen_ll **list, void **data,  void *cmp_input, prh_host_t_cmp_funcptr cmp_funcptr);
int prh_host_ll_remove_all(struct prh_host_gen_ll **list,prh_host_t_exe_funcptr execute_funcptr, void *func_info);
int prh_host_ll_traverse(struct prh_host_gen_ll *list, prh_host_t_exe_funcptr execute_funcptr, void *func_data);
#endif

