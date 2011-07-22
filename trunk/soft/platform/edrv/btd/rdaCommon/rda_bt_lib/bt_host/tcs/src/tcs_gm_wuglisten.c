/***********************************************************************
 *
 * MODULE NAME:    tcs_gm_wuglisten.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    TCS Wireless User Group Listen SM
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  06 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_gm_wuglisten.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)


#include "tcs_lower_layer.h"
#include "host_ll.h"
#include "mgr_utils.h"
#include "tcs_core.h"
#include "tcs_cc_core.h"
#include "tcs_gm_types.h"
#include "tcs_wug.h"
#include "tcs_gm_api.h"

t_pMutex prh_tcs_wug_listen_lock;
#if PRH_TCS_GATEWAY
struct prh_host_gen_ll *prh_tcs_wug_listen_list;

int prh_tcs_wug_listen_add(struct prh_st_tcs_wug_listen_info *new_ent)
{
  int ret;

  pMutexLock(prh_tcs_wug_listen_lock);
  ret=prh_host_ll_add(&prh_tcs_wug_listen_list, (void *)new_ent);
  pMutexUnlock(prh_tcs_wug_listen_lock);
  return ret;
}

/*
 * prh_tcs_wug_listen_term_bdaddr_cmp
 * -- WUG traverse function
 *
 */
static int prh_tcs_wug_listen_term_bdaddr_cmp(void *cmpin, void *compwith)
{
  struct prh_st_tcs_wug_listen_info *instance=(struct prh_st_tcs_wug_listen_info *)compwith;
  t_bdaddr bd_addr=*((t_bdaddr *)cmpin);
  
  return (BD_ADDR_CMP(bd_addr, instance->terminator->addr));
}

/*
 */
int prh_tcs_wug_listen_lookup_by_terminator_addr(t_bdaddr addr, struct prh_st_tcs_wug_listen_info **listen_info)
{
  int ret;
  pMutexLock(prh_tcs_wug_listen_lock);
  ret=prh_host_ll_search(&prh_tcs_wug_listen_list, (void **)listen_info, (void *)&addr, &prh_tcs_wug_listen_term_bdaddr_cmp);
  pMutexUnlock(prh_tcs_wug_listen_lock);
  return ret;
}


int prh_tcs_wug_listen_remove_by_terminator_addr(t_bdaddr addr, struct prh_st_tcs_wug_listen_info **listen_info)
{
  int ret;
  pMutexLock(prh_tcs_wug_listen_lock);
  ret=prh_host_ll_remove(&prh_tcs_wug_listen_list, (void **)listen_info, (void *)&addr, &prh_tcs_wug_listen_term_bdaddr_cmp);
  pMutexUnlock(prh_tcs_wug_listen_lock);
  return ret;
}

static int prh_tcs_wug_listen_removeall_helper(void *linfo, void *input_data)
{
  struct prh_st_tcs_wug_listen_info *listen_info=(struct prh_st_tcs_wug_listen_info *)linfo;
  pTimerCancel(listen_info->prh_tcs_t405_handle);
  listen_info->prh_tcs_t405_handle=0;
  pFree(listen_info);
  return BT_NOERROR;
}

int prh_tcs_wug_listen_removeall(void)
{
  int ret;
  pMutexLock(prh_tcs_wug_lock);
  ret=prh_host_ll_remove_all(&prh_tcs_wug_listen_list, prh_tcs_wug_listen_removeall_helper, NULL);
  pMutexUnlock(prh_tcs_wug_lock);
  return ret;
}
#endif

#if PRH_TCS_TERMINAL

u_int8 prh_tcs_wug_listen_in_progress;
struct prh_st_tcs_wug_member *prh_tcs_wug_listen_requested;

#endif

#endif /*(PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/
