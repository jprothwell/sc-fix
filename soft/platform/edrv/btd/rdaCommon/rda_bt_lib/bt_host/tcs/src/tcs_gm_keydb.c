/***********************************************************************
 *
 * MODULE NAME:    tcs_gm_keydb.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    TCS Wireless User Group Link Key database handlers
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  06 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_gm_keydb.c 1532 2010-08-23 05:47:34Z huazeng $
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
#include "tcs_core.h"
#include "tcs_cc_core.h"
#include "tcs_gm_types.h"
#include "tcs_wug.h"
#include "tcs_gm_api.h"
#include "tcs_proto.h"



int prh_tcs_wug_keydb_generate_linkkey(u_int8 *link_key)
{
  static unsigned char x=0;
  int i;
  
  for (i=0;i<16; i++)
    link_key[i]=x;
  x++;
  return BT_NOERROR;
}

/*
 * for every x where x is a member of the WUG, y another member of the group has
 * a link key for every x.
 */

int prh_tcs_wug_keydb_trav_helper(void *data, void *input)
{
  struct prh_st_tcs_wug_member *instance=(struct prh_st_tcs_wug_member *)data;
  struct prh_st_tcs_wug_member *wug_mem=(struct prh_st_tcs_wug_member *)input;

  int ret;
  struct prh_st_tcs_wug_member_link_key *link_key1, *link_key2;

  /* generate link key */
  link_key1=(struct prh_st_tcs_wug_member_link_key *)pMalloc(sizeof(struct prh_st_tcs_wug_member_link_key));
  if (link_key1==pNULL)
    return BT_NORESOURCES;

  link_key2=(struct prh_st_tcs_wug_member_link_key *)pMalloc(sizeof(struct prh_st_tcs_wug_member_link_key));

  if (link_key2==pNULL)
    {
      pFree(link_key1);
      return BT_NORESOURCES;
    }
  
  prh_tcs_wug_keydb_generate_linkkey(link_key1->link_key);
  pMemcpy(link_key2->link_key, link_key1->link_key, 16);
  
  link_key1->peer = instance->addr;
  link_key2->peer = wug_mem->addr;

  /* add link key to both wug_members */
  ret=prh_tcs_wug_keydb_add_key(wug_mem, link_key1);
  
  if (ret!=BT_NOERROR)
    {
      pFree(link_key1);
      pFree(link_key2);
      return BT_NORESOURCES;
    }

  ret=prh_tcs_wug_keydb_add_key(instance, link_key2);
  if (ret!=BT_NOERROR)
    {
      prh_tcs_wug_keydb_remove_key_by_bd_addr(wug_mem, instance->addr, &link_key1);
      pFree(link_key1);
      pFree(link_key2);
      return BT_NORESOURCES;
    } 
      
  return BT_NOERROR;
}


t_api prh_tcs_wug_keydb_generate_link_keys(struct prh_st_tcs_wug_member *wug_mem)
{
  /* we must traverse the wug and add a link key for every member + this member */
  int ret;

  /* righto need to traverse the WUG info database */

  pMutexLock(prh_tcs_wug_lock);
  ret=prh_host_ll_traverse(prh_tcs_wug_list, prh_tcs_wug_keydb_trav_helper, (void *)wug_mem);
  pMutexUnlock(prh_tcs_wug_lock);
  return BT_NOERROR;
}

/* WUG Key database linked list manipulator functions */

int prh_tcs_wug_keydb_add_key(struct prh_st_tcs_wug_member *wug_mem, struct prh_st_tcs_wug_member_link_key *key)
{
  int ret;

  ret=prh_host_ll_add(&wug_mem->lk.member_link_keys, (void *)key);
  return ret;
}

static int prh_tcs_wug_keydb_bdaddr_cmp(void *cmpin, void *compwith)
{
  struct prh_st_tcs_wug_member_link_key *instance=(struct prh_st_tcs_wug_member_link_key *)compwith;
  t_bdaddr bd_addr=*((t_bdaddr *)cmpin);
  
  return (BD_ADDR_CMP(bd_addr, instance->peer));
}

int prh_tcs_wug_keydb_lookup_key_by_bd_addr(struct prh_st_tcs_wug_member *wug_mem, t_bdaddr addr, struct prh_st_tcs_wug_member_link_key **key)
{
  int ret;

  ret=prh_host_ll_search(&wug_mem->lk.member_link_keys, (void **)key, (void *)&addr, &prh_tcs_wug_keydb_bdaddr_cmp);
  return ret;
}


int prh_tcs_wug_keydb_remove_key_by_bd_addr(struct prh_st_tcs_wug_member *wug_mem, t_bdaddr addr, struct prh_st_tcs_wug_member_link_key **key)
{
  int ret;
 
  ret=prh_host_ll_remove(&wug_mem->lk.member_link_keys, (void **)key, (void *)&addr, &prh_tcs_wug_keydb_bdaddr_cmp);
  return ret;
}

static int prh_tcs_wug_lk_removeall_helper(void *cur_mem, void *input_data)
{
  struct prh_st_tcs_wug_member_link_key *link_key = (struct prh_st_tcs_wug_member_link_key *)cur_mem;
  pFree(link_key);
  return BT_NOERROR;
}

int prh_tcs_wug_keydb_remove_all_keys(struct prh_st_tcs_wug_member *wug_mem)
{
  int ret;
  ret=prh_host_ll_remove_all(&wug_mem->lk.member_link_keys,prh_tcs_wug_lk_removeall_helper, NULL);
  return ret;
}
#endif /* (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/
