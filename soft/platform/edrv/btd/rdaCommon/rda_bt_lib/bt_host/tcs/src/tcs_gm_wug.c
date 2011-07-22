/***********************************************************************
 *
 * MODULE NAME:    tcs_gm_wug.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    TCS Wireless User Group 
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  06 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_gm_wug.c 1532 2010-08-23 05:47:34Z huazeng $
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
#include "mgr_utils.h"
#include "tcs_core.h"
#include "tcs_cc_core.h"
#include "tcs_gm_types.h"
#include "tcs_wug.h"
#include "tcs_gm_api.h"
#include "tcs_proto.h"

t_pTimer prh_tcs_t401_handle;
t_pTimer prh_tcs_t404_handle;
extern t_pTimer prh_tcs_t406_handle;
int prh_tcs_wug_new_members_welcome = 1;
u_int8 prh_tcs_wug_role;
unsigned int prh_tcs_wug_num_members;


#if PRH_TCS_TERMINAL
extern u_int8 prh_tcs_wug_listen_in_progress;
#endif

#if PRH_TCS_TERMINAL
void prh_tcs_timer_t401_expired(void *tim)
{
  if (TCSInstance.callbacks->accessRightsResponse)
    TCSInstance.callbacks->accessRightsResponse(0);
}

void prh_tcs_timer_t404_expired(void *tim)
{
  pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"Timer T404 fired\n"));

  pMutexLock(prh_tcs_wug_listen_lock);  
  /* have we a listen request in progress */
  prh_tcs_wug_listen_in_progress=0;
  /*  prh_tcs_wug_listen_requested=*/
  pMutexUnlock(prh_tcs_wug_listen_lock);  

}

#endif
#if PRH_TCS_GATEWAY==1
void prh_tcs_timer_t403_expired(void *tim)
{

}

void prh_tcs_timer_t405_expired(void *tim)
{
  int ret;
  struct prh_st_tcs_wug_listen_info *listen_info=(struct prh_st_tcs_wug_listen_info *)tim;
  struct prh_st_tcs_wug_listen_info *listen_returned;
  struct prh_st_tcs_gm_encode_info info;
  u_int8 cause=PRH_TCS_CAUSE_SUBSCRIBERABSENT;

  ret=prh_tcs_wug_listen_remove_by_terminator_addr(listen_info->terminator->addr, &listen_returned);
  if (ret==BT_NOERROR)
    {
      info.compspec=NULL;
      info.cause=&cause;
      prh_tcs_gm_encode_listen_reject(&info, listen_info->requester->l2_conn);
      pFree(listen_returned);
    }
}
#endif

#if PRH_TCS_GATEWAY==1
int prh_tcs_wug_handle_access_rights_request(struct prh_st_tcs_l2conn *device_info)
{
  struct prh_st_tcs_gm_encode_info info;
  struct prh_st_tcs_wug_member *wug_mem;
  int ret;

  info.compspec=NULL;

  /* check has application registered callback */
  if (TCSInstance.callbacks->accessRightsRequest)  
    {
      /* fill out WUG member structure then ask the application */
      wug_mem=(struct prh_st_tcs_wug_member *)pMalloc(sizeof(struct prh_st_tcs_wug_member));
      if (wug_mem==NULL)
	return BT_NORESOURCES;
      prh_tcs_wug_role=PRH_TCS_WUG_MASTER;

      pMemset(wug_mem, 0, sizeof(struct prh_st_tcs_wug_member));
      
      wug_mem->addr = device_info->bd_addr;
      wug_mem->needs_update=1;
      
      /* need to assign internal number to device */
      ret=prh_tcs_wug_assign_internal_number(wug_mem->wug_num);
      if (ret==BT_NORESOURCES)
	{
	  pFree(wug_mem);
	  return BT_NORESOURCES;
	}
      /* need to get link key for device */
      /* need to generate link keys for the device + all other members of WUG -- big stuff..*/
      prh_tcs_wug_keydb_generate_link_keys(wug_mem);
  
      wug_mem->l2_conn=device_info;
     
      TCSInstance.callbacks->accessRightsRequest((t_TCS_wugMember *)wug_mem);
    }
  else
    prh_tcs_gm_encode_access_rights_reject(&info, device_info);

  return BT_NOERROR;
}
#endif

int prh_tcs_wug_handle_access_rights_accept(struct prh_st_tcs_l2conn *device_info)
{
  pTimerCancel(prh_tcs_t401_handle);
prh_tcs_t401_handle=0;
  if (TCSInstance.callbacks->accessRightsResponse)
    TCSInstance.callbacks->accessRightsResponse(1);
  return 0;
}

int prh_tcs_wug_handle_access_rights_reject(struct prh_st_tcs_l2conn *device_info)
{
  pTimerCancel(prh_tcs_t401_handle);
  prh_tcs_t401_handle=0;
  if (TCSInstance.callbacks->accessRightsResponse)
    TCSInstance.callbacks->accessRightsResponse(0);
  return 0;
}

int prh_tcs_wug_handle_info_accept(struct prh_st_tcs_l2conn *device_info)
{
  struct prh_st_tcs_wug_member *wug_mem;
  int ret;

  ret=prh_tcs_lookup_wug_member_by_bd_addr(device_info->bd_addr, &wug_mem);
  if (ret==BT_NOERROR)
    {
      wug_mem->needs_update=0;
      pTimerCancel(wug_mem->t403_handle);
	  wug_mem->t403_handle=0;
    }
  return BT_NOERROR;
}

#if PRH_TCS_GATEWAY==1
int prh_tcs_wug_handle_listen_request(struct prh_st_tcs_l2conn *device_info, struct prh_st_tcs_gm_called_party_number *cpn_num)
{
  struct prh_st_tcs_wug_member *requester, *terminator;
  struct prh_st_tcs_gm_encode_info info;
  struct prh_st_tcs_wug_listen_info *listen_info;
  int ret;
  u_int8 cause;
  if (cpn_num->num_len!=2)
    return BT_INVALIDPARAM;
  
  ret=prh_tcs_lookup_wug_member_by_bd_addr(device_info->bd_addr, &requester);
  if (ret==BT_NOERROR)
    {
      ret=prh_tcs_lookup_wug_member_by_internal_num(cpn_num->num, &terminator);
	
      if (ret==BT_NOERROR)
	{
	  
	  /* check if the device is in a call already */
	  
	  /* send a listen suggest to the device */
	  info.compspec=NULL;
	  /* add to local listen data base */
	  listen_info=(struct prh_st_tcs_wug_listen_info *)pMalloc(sizeof(struct prh_st_tcs_wug_listen_info));
	  if (listen_info==pNULL)
	    return BT_NORESOURCES;
	  
	  listen_info->requester=requester;
	  listen_info->terminator=terminator;
	  ret=prh_tcs_wug_listen_add(listen_info);
	  if (ret==BT_NORESOURCES)
	    {
	      pFree(listen_info);
	      return BT_NORESOURCES;
	    }
	  
	  /* set Timer T405 on master */
	  listen_info->prh_tcs_t405_handle=pTimerCreate(PRH_TCS_TIMER_T405, prh_tcs_timer_t405_expired, (void *)listen_info, pTIMER_ONESHOT);	  
	  if (ret==BT_NORESOURCES)
	    return ret;
	  prh_tcs_gm_encode_listen_suggest(&info, terminator->l2_conn);
	}
      else
	{
	  pMemset(&info, 0, sizeof(struct prh_st_tcs_gm_encode_info));
	  cause=PRH_TCS_CAUSE_UNASSIGNEDNUMBER;
	  info.cause=&cause;
	  prh_tcs_gm_encode_listen_reject(&info, device_info);
	  return BT_INVALIDPARAM;
	}
    }
  return BT_NOERROR;
}
#endif

int prh_tcs_wug_handle_listen_suggest(struct prh_st_tcs_l2conn *device_info)
{
  if (TCSInstance.callbacks->listenSuggest)
    {
	  TCSInstance.callbacks->listenSuggest();
    }
  return BT_NOERROR;
}

int prh_tcs_wug_handle_listen_reject(struct prh_st_tcs_l2conn *device_info, u_int8 cause)
{

#if PRH_TCS_GATEWAY==1
  struct prh_st_tcs_wug_listen_info *listen_info;
  struct prh_st_tcs_gm_encode_info info;
  int ret;

  if (prh_tcs_wug_role==PRH_TCS_WUG_MASTER)
    {
      /* if I'm the WUG MASTER I need to send a reject back to the person
	 who requested the SUGGEST */
      ret=prh_tcs_wug_listen_remove_by_terminator_addr(device_info->bd_addr, &listen_info);
      if (ret==BT_NOERROR)
	{

	  info.compspec=NULL;
	  info.cause=&cause;
	  pTimerCancel(listen_info->prh_tcs_t405_handle);
	  listen_info->prh_tcs_t405_handle=0;
	  prh_tcs_gm_encode_listen_reject(&info, listen_info->requester->l2_conn);	  
	  pFree(listen_info);
	}
    }
#endif
#if PRH_TCS_TERMINAL==1
	if (prh_tcs_wug_role!=PRH_TCS_WUG_MASTER)
     {
       /* if I'm a WUG MEMBER I need to cancel the timer and send something to the application */
       pTimerCancel(prh_tcs_t404_handle); 
	   prh_tcs_t404_handle=0;
       pMutexLock(prh_tcs_wug_listen_lock);  
       /* have we a listen request in progress */

       /* if we don't .. a timer probably fired and cleaned it up */
       if(prh_tcs_wug_listen_in_progress==0)
	 {
	   pMutexUnlock(prh_tcs_wug_listen_lock);
	   return BT_INVALIDPARAM;
	 }

       prh_tcs_wug_listen_in_progress=0;
       /*  prh_tcs_wug_listen_requested=*/
       pMutexUnlock(prh_tcs_wug_listen_lock);

  
       if (TCSInstance.callbacks->listenReject)
	 {
	   TCSInstance.callbacks->listenReject(cause);
	 }
     }
#endif
  return BT_NOERROR;
}

int prh_tcs_wug_handle_listen_accept(struct prh_st_tcs_l2conn *device_info)
{
#if PRH_TCS_GATEWAY==1
  struct prh_st_tcs_wug_listen_info *listen_info;
  struct prh_st_tcs_gm_encode_info info;
  int ret;

	if (prh_tcs_wug_role==PRH_TCS_WUG_MASTER)
    {
      ret=prh_tcs_wug_listen_remove_by_terminator_addr(device_info->bd_addr, &listen_info);
      if (ret==BT_NOERROR)
	{
	  info.compspec=NULL;
	  pTimerCancel(listen_info->prh_tcs_t405_handle);
	  listen_info->prh_tcs_t405_handle=0;
	  prh_tcs_gm_encode_listen_accept(&info, listen_info->requester->l2_conn);
	  pFree(listen_info);
	}
    }
#endif
#if PRH_TCS_TERMINAL==1
	if (prh_tcs_wug_role!=PRH_TCS_WUG_MASTER)
    {
       pTimerCancel(prh_tcs_t404_handle);
	   prh_tcs_t404_handle=0;
       pMutexLock(prh_tcs_wug_listen_lock);  
       /* have we a listen request in progress */

       /* if we don't .. a timer probably fired and cleaned it up */
       if(prh_tcs_wug_listen_in_progress==0)
	 {
	   pMutexUnlock(prh_tcs_wug_listen_lock);
	   return BT_INVALIDPARAM;
	 }

       prh_tcs_wug_listen_in_progress=0;
       /*  prh_tcs_wug_listen_requested=*/
       pMutexUnlock(prh_tcs_wug_listen_lock);
       /* if we wre not a multi-piconet capable device ... disconnect from Master */
#if (PRH_TCS_MULTIPICONETCAPABLE==0)
       L2_Disconnect(device_info->cid);
#endif
       if (TCSInstance.callbacks->listenAccept)
	 {
	   TCSInstance.callbacks->listenAccept();
	 }

    }
#endif
  return BT_NOERROR;
}

int prh_tcs_gm_reset(int flags)
{
#if (PRH_TCS_TERMINAL==1)
  pTimerCancel(prh_tcs_t401_handle);
  pTimerCancel(prh_tcs_t404_handle);
  pTimerCancel(prh_tcs_t406_handle);
  prh_tcs_t401_handle=0;
  prh_tcs_t404_handle=0;
  prh_tcs_t406_handle=0;
#endif

  /*clean up WUG database*/
  prh_tcs_wug_clear_db();

  /* nuke the mutex for the DB */
  pMutexLock(prh_tcs_wug_lock);
  pMutexFree(prh_tcs_wug_lock);

  /* is their a listen database or variable used */
#if (PRH_TCS_GATEWAY==1)
  if (prh_tcs_wug_role==PRH_TCS_WUG_MASTER)
    {
      prh_tcs_wug_listen_removeall();
      pMutexLock(prh_tcs_wug_listen_lock);
    }
#endif
#if (PRH_TCS_TERMINAL==1)
    if (prh_tcs_wug_role!=PRH_TCS_WUG_MASTER)
    {
      pMutexLock(prh_tcs_wug_listen_lock);
      prh_tcs_wug_listen_in_progress=0;
    }
#endif
  pMutexFree(prh_tcs_wug_listen_lock);
  prh_tcs_wug_role=0;
  /* clean up timers */

  return BT_NOERROR;
}
#endif /*(PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/
