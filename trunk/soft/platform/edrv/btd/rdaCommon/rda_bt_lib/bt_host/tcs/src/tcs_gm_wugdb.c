/***********************************************************************
 *
 * MODULE NAME:    tcs_gm_wug.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    TCS Wireless User Group 
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  06 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_gm_wugdb.c 1532 2010-08-23 05:47:34Z huazeng $
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
#include "tcs_proto.h"

extern void prh_tcs_timer_t401_expired(void *tim);
extern void prh_tcs_timer_t403_expired(void *tim);
u_int8 prh_tcs_wug_config_accept_mode;

struct prh_host_gen_ll *prh_tcs_wug_list;
t_pMutex prh_tcs_wug_lock;

int prh_tcs_wug_add_member(struct prh_st_tcs_wug_member *new_ent)
{
  int ret;

  pMutexLock(prh_tcs_wug_lock);
  prh_tcs_wug_num_members++;
  ret=prh_host_ll_add(&prh_tcs_wug_list, (void *)new_ent);
  pMutexUnlock(prh_tcs_wug_lock);
  return ret;
}

int prh_tcs_wug_remove_member(t_bdaddr addr)
{
  struct prh_st_tcs_wug_member *instance;
  int ret;
  
    ret=prh_tcs_remove_wug_member_by_bd_addr(addr, &instance);
  if (ret==BT_NOERROR)
    {
      prh_tcs_wug_release_internal_number(instance->wug_num); 
      pFree(instance);
    }
#if PRH_TCS_GATEWAY==1
  prh_tcs_wug_distribute_config_to_wug();
#endif
  return BT_NOERROR;
} 

static int prh_tcs_wug_removeall_helper(void *cur_mem, void *input_data)
{
  struct prh_st_tcs_wug_member *wug_mem=(struct prh_st_tcs_wug_member *)cur_mem;
  if (prh_tcs_wug_role==PRH_TCS_WUG_MASTER)  
    prh_tcs_wug_keydb_remove_all_keys(wug_mem);
  pTimerCancel(wug_mem->t403_handle);
  wug_mem->t403_handle=0;
  pFree(wug_mem);
  return BT_NOERROR;
}

int prh_tcs_wug_clear_db(void)
{
  int ret;
  pMutexLock(prh_tcs_wug_lock);

  ret=prh_host_ll_remove_all(&prh_tcs_wug_list, prh_tcs_wug_removeall_helper, NULL);
  prh_tcs_wug_num_members=0;
  pMutexUnlock(prh_tcs_wug_lock);
  return ret;
}
/*
 * prh_tcs_wug_bdaddr_cmp
 * -- WUG traverse function
 *
 * This is used to compare a bd address with the entries in the list
 */
static int prh_tcs_wug_bdaddr_cmp(void *cmpin, void *compwith)
{
  struct prh_st_tcs_wug_member *instance=(struct prh_st_tcs_wug_member *)compwith;
  t_bdaddr bd_addr=*((t_bdaddr *)cmpin);
  
  return (BD_ADDR_CMP(bd_addr, instance->addr));
}

/*
 */
int prh_tcs_lookup_wug_member_by_bd_addr(t_bdaddr addr, struct prh_st_tcs_wug_member **member)
{
  int ret;
  pMutexLock(prh_tcs_wug_lock);
  ret=prh_host_ll_search(&prh_tcs_wug_list, (void **)member, (void *)&addr, &prh_tcs_wug_bdaddr_cmp);
  pMutexUnlock(prh_tcs_wug_lock);
  return ret;
}

static int prh_tcs_wug_internalnum_cmp(void *cmpin, void *compwith)
{
  struct prh_st_tcs_wug_member *instance=(struct prh_st_tcs_wug_member *)compwith;
  u_int8 *internal_num=(u_int8 *)cmpin;
  
  return (!pMemcmp(instance->wug_num, internal_num, 2));
}

/*
 */
int prh_tcs_lookup_wug_member_by_internal_num(u_int8 *internal_num, struct prh_st_tcs_wug_member **member)
{
  int ret;
  pMutexLock(prh_tcs_wug_lock);
  ret=prh_host_ll_search(&prh_tcs_wug_list, (void **)member, (void *)internal_num, &prh_tcs_wug_internalnum_cmp);
  pMutexUnlock(prh_tcs_wug_lock);
  return ret;
}

/*
 */
int prh_tcs_remove_wug_member_by_bd_addr(t_bdaddr addr, struct prh_st_tcs_wug_member **member)
{
  int ret;
  pMutexLock(prh_tcs_wug_lock);
  ret=prh_host_ll_remove(&prh_tcs_wug_list, (void **)member, (void *)&addr, &prh_tcs_wug_bdaddr_cmp);
  prh_tcs_wug_num_members--;
  pMutexUnlock(prh_tcs_wug_lock);
  return ret;
}

/*
 */
int prh_tcs_wug_traverse_helper(void *data, void *input)
{
  struct prh_st_tcs_wug_member *instance=(struct prh_st_tcs_wug_member *)data;
  struct prh_st_tcs_gm_encode_info *info = (struct prh_st_tcs_gm_encode_info *)input;
  int ent_num = info->num_conf_ents;
  struct prh_st_tcs_wug_member_link_key *key;

  if (BD_ADDR_CMP(instance->addr, info->wug_mem->addr))
    return BT_NOERROR;
  info->config[ent_num].internal_num[0]=instance->wug_num[0];
  info->config[ent_num].internal_num[1]=instance->wug_num[1];
  pMemcpy(info->config[ent_num].bd_addr.bytes, instance->addr.bytes, 6);
  
  /* time to get do a linkkey lookup */
  prh_tcs_wug_keydb_lookup_key_by_bd_addr(instance, info->wug_mem->addr, &key);
  
  pMemcpy(info->config[ent_num].link_key, key->link_key, 16);
  pDebugPrintfEX((pLOGDEBUG,pLOGTCS, "Member: %d, num: %c%c, address: %s, link_key %s\n", info->num_conf_ents, info->config[ent_num].internal_num[0], info->config[ent_num].internal_num[1], prh_mgr_printBDAddress(info->config[ent_num].bd_addr), prh_mgr_printLargeNumber(key->link_key, 16)));
  info->num_conf_ents++;
  return 0;
}

#if PRH_TCS_GATEWAY==1
/*
 */
int prh_tcs_wug_info_trav_helper(void *data, void *input)
{
  struct prh_st_tcs_wug_member *instance=(struct prh_st_tcs_wug_member *)data;
  struct prh_st_tcs_gm_encode_info *info = (struct prh_st_tcs_gm_encode_info *)input;

  instance->needs_update=1;
  instance->t403_handle=pTimerCreate(PRH_TCS_TIMER_T403, prh_tcs_timer_t403_expired, (void *)instance, pTIMER_ONESHOT);
  prh_tcs_gm_encode_info_suggest(info, instance->l2_conn);
  return BT_NOERROR;
}

int prh_tcs_wug_distrib_helper(void *data, void *input)
{
  struct prh_st_tcs_wug_member *instance=(struct prh_st_tcs_wug_member *)data;
  struct prh_st_tcs_gm_encode_info info;
  int ret;

  info.config = (struct prh_st_tcs_gm_config_data *)pMalloc(sizeof(struct prh_st_tcs_gm_config_data)*prh_tcs_wug_num_members);
  info.num_conf_ents=0;
  info.compspec=0;
  info.wug_mem=instance;

  /* traverse the wug list with the info to fill out the list */
  ret=prh_host_ll_traverse(prh_tcs_wug_list, prh_tcs_wug_traverse_helper, (void *)&info);
  if (info.num_conf_ents>0)
    prh_tcs_gm_encode_info_suggest(&info, instance->l2_conn);
  pFree(info.config);
  return ret;
}

/*
 * for every member of the WUG, traverse the list using that members information 
 */
int prh_tcs_wug_distribute_config_to_wug(void)
{
  int ret;
  pMutexLock(prh_tcs_wug_lock);
  ret=prh_host_ll_traverse(prh_tcs_wug_list, prh_tcs_wug_distrib_helper, (void *)NULL);
  pMutexUnlock(prh_tcs_wug_lock);
  return ret;
}
#endif

/*
 * only called on terminal 
 */
int prh_tcs_wug_put_info_into_db(struct prh_st_tcs_gm_config_data *config_array, int num_entries)
{
  struct prh_st_tcs_wug_member *wug_mem;
  t_TCS_wugMember **app_list;

  int i, ret;

  prh_tcs_wug_clear_db();
  
  app_list = (t_TCS_wugMember **)pMalloc(sizeof(t_TCS_wugMember *)*num_entries);
  
  for (i=0; i<num_entries; i++)
    {
      wug_mem=(struct prh_st_tcs_wug_member *)pMalloc(sizeof(struct prh_st_tcs_wug_member));
      if (wug_mem==pNULL)
	return BT_NORESOURCES;

      pDebugPrintfEX((pLOGDEBUG,pLOGTCS, "member %d: addr: %s, num %c%c lk: %s", i, prh_mgr_printBDAddress(config_array[i].bd_addr), config_array[i].internal_num[0], config_array[i].internal_num[1], prh_mgr_printLargeNumber(config_array[i].link_key, 16)));
      

      wug_mem->addr=config_array[i].bd_addr;
      wug_mem->wug_num[0]=config_array[i].internal_num[0];
      wug_mem->wug_num[1]=config_array[i].internal_num[1];
      pMemcpy(wug_mem->lk.link_key, config_array[i].link_key, 16);
      if (prh_tcs_wug_config_accept_mode)
	prh_mgr_updateLinkKeyEntry(wug_mem->addr, wug_mem->lk.link_key, LINKKEY_MASTER);

      app_list[i]=(t_TCS_wugMember *)wug_mem;
      
      ret=prh_tcs_wug_add_member(wug_mem);
      if (ret==BT_NORESOURCES)
	return BT_NORESOURCES;
    }

  if (TCSInstance.callbacks->newWugInformationIndication)
    TCSInstance.callbacks->newWugInformationIndication(app_list, num_entries);

  return BT_NOERROR;
}


/* max num of members in WUG is 99 (max internal numbers) */
/* write a unit test for this one */
unsigned long prh_wug_member_mask[4];

/* reset mask to zero - used for test and system reset */
t_api prh_tcs_wug_reset_internal_numbers(void)
{
  prh_wug_member_mask[0]=prh_wug_member_mask[1]=prh_wug_member_mask[2]=prh_wug_member_mask[3]=0;
  return BT_NOERROR;
}

t_api prh_tcs_wug_assign_internal_number(u_int8 *internal_num)
{
  unsigned int num=0;
  int found=0;
  
  while (num<99 && !found)
    {
      if (num<32)
	{
	  if ((~prh_wug_member_mask[0]) & 1<<num)
	    {
	      found=num;
	      prh_wug_member_mask[0] |= 1<<num;
	    }
	}
      else if (num<64)
	{
	  if ((~prh_wug_member_mask[1]) & 1<<(num-32))
	    {
	      found=num;
	      prh_wug_member_mask[1] |= 1<<(num-32);
	    }
	}  
      else if (num<96)
	{
	  if ((~prh_wug_member_mask[2]) & 1<<(num-64))
	    {
	      found=num;
	      prh_wug_member_mask[2] |= 1<<(num-64);
	    }
	}
      else 
	{
	  if ((~prh_wug_member_mask[3]) & 1<<(num-96))
	    {
	      found=num;
	      prh_wug_member_mask[3] |= 1<<(num-96);
	    }
	}
      num++;
    }
  
  if (found)
    {
      internal_num[0]=(found/10)+'0';
      internal_num[1]=(found%10)+'0';
      return BT_NOERROR;
    }
  return BT_NORESOURCES;
}


t_api prh_tcs_wug_release_internal_number(u_int8 *internal_num)
{
  int lookfor = (internal_num[0]-'0')*10 + (internal_num[1]-'0');

  if (lookfor<32)
    prh_wug_member_mask[0] &= ~(1<<lookfor);
  else if (lookfor<64)
    prh_wug_member_mask[1] &= ~(1<<(lookfor-32));  
  else if (lookfor<96)
    prh_wug_member_mask[2] &= ~(1<<(lookfor-64));  
  else if (lookfor<99)
    prh_wug_member_mask[3] &= ~(1<<(lookfor-96));  
  else
    return BT_INVALIDPARAM;

  return BT_NOERROR;
}
#endif /*(PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/
