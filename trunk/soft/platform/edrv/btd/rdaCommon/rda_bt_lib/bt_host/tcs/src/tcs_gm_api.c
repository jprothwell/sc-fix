/***********************************************************************
 *
 * MODULE NAME:    tcs_wug.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    TCS L2CAP Channel Handler non-state machine related
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  06 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_gm_api.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
#include "tcs_lower_layer.h"
#include "tcs_core.h"
#include "tcs_gm_types.h"
#include "tcs_wug.h"
#include "tcs_cc_core.h"
#include "tcs_gm_api.h"
#include "tcs_proto.h"

#if PRH_TCS_TERMINAL
void prh_tcs_timer_t401_expired(void *tim);
void prh_tcs_timer_t404_expired(void *tim);
extern int prh_tcs_t401_handle;
extern int prh_tcs_t404_handle;
#endif

#if PRH_TCS_TERMINAL
APIDECL1 t_api APIDECL2 TCS_ObtainWUGAccessRights(t_bdaddr gw_addr)
{
  int ret;
  struct prh_st_tcs_gm_encode_info info;

  struct prh_st_tcs_l2conn *l2_conn;

  ret=prh_tcs_lookup_l2cap_inst_by_bd_addr(gw_addr, &l2_conn);
  if (ret==BT_NOERROR)
    {
      info.compspec=NULL;
      
      prh_tcs_wug_role=PRH_TCS_WUG_MEMBER;
      /* Start timer T401 8 seconds */
      prh_tcs_t401_handle=pTimerCreate(PRH_TCS_TIMER_T401, prh_tcs_timer_t401_expired, NULL, pTIMER_ONESHOT);
	  ret=prh_tcs_gm_encode_access_rights_req(&info, l2_conn);
    }
  return ret;
}
#endif

#if PRH_TCS_GATEWAY
APIDECL1 t_api APIDECL2 TCS_AccessRightsResponse(t_TCS_wugMember *wugMember, u_int8 accept)
{
  struct prh_st_tcs_gm_encode_info info;
  struct prh_st_tcs_wug_member *wug_mem=(struct prh_st_tcs_wug_member *)wugMember;

  info.compspec=NULL;
  if (accept)
    {
      
      prh_tcs_wug_add_member(wug_mem);
      prh_tcs_gm_encode_access_rights_accept(&info, wug_mem->l2_conn);
      prh_tcs_wug_distribute_config_to_wug();
    }
  else
    {
      prh_tcs_gm_encode_access_rights_reject(&info, wug_mem->l2_conn);
      pFree(wug_mem);
    }
  return BT_NOERROR;
}
#endif

#if PRH_TCS_GATEWAY
APIDECL1 t_api APIDECL2 TCS_DistriuteWUGConfigInfo(void)
{
  return prh_tcs_wug_distribute_config_to_wug();
}
#endif

#if PRH_TCS_TERMINAL
extern struct prh_st_tcs_wug_member *prh_tcs_wug_listen_requested;
extern u_int8 prh_tcs_wug_listen_in_progress;

APIDECL1 t_api APIDECL2 TCS_ListenRequest(char *int_num)
{
  struct prh_st_tcs_l2conn *l2_conn;
  struct prh_st_tcs_gm_encode_info info;
  struct prh_st_tcs_gm_called_party_number cpn;
  int ret;
  ret=prh_tcs_lookup_l2cap_inst_by_bd_addr(TCSInstance.GWBDAddress, &l2_conn);
  if (ret==BT_INVALIDPARAM)
    return BT_INVALIDPARAM;

  /* TCS can only handle a single listen request at a time */
  /* check have we requested fast access to someone already */
  pMutexLock(prh_tcs_wug_listen_lock);  
  /* have we a listen request in progress */
  if (prh_tcs_wug_listen_in_progress)
    {
      pMutexUnlock(prh_tcs_wug_listen_lock);
      return BT_INVALIDPARAM;
    }
  prh_tcs_wug_listen_in_progress=1;
  /*  prh_tcs_wug_listen_requested=*/
  pMutexUnlock(prh_tcs_wug_listen_lock);
  
  info.compspec=NULL;
  cpn.type=0;
  cpn.plan_id=0;
  cpn.num_len=2;
  cpn.num=(u_int8 *)pMalloc(2);
  cpn.num[0]=int_num[0];
  cpn.num[1]=int_num[1];
  info.calledpn=&cpn;
  
  prh_tcs_t404_handle=pTimerCreate((unsigned int)PRH_TCS_TIMER_T404, prh_tcs_timer_t404_expired, NULL, pTIMER_ONESHOT);
  prh_tcs_gm_encode_listen_request(&info, l2_conn);
  return BT_NOERROR;
}

void prh_tcs_wug_retreiveClockOffset(u_int16 aclHandle, u_int16 clockOffset, t_api status);

APIDECL1 t_api APIDECL2 TCS_ListenResponse(u_int8 accept, u_int8 cause)
{
    struct prh_st_tcs_gm_encode_info info;
    struct prh_st_tcs_l2conn *l2_conn;
    int ret;
    u_int16 handle;
    
    ret=prh_tcs_lookup_l2cap_inst_by_bd_addr(TCSInstance.GWBDAddress, &l2_conn);
    if (ret==BT_INVALIDPARAM)
        return BT_INVALIDPARAM;
    
    info.compspec=NULL;
    if (accept)
    {
        L2_RetrieveHandle(l2_conn->cid, &handle);
        


        /* the manager will call prh_tcs_wug_retreiveClockOffset on every ReadClockOffsetComplete event */
        l2_conn->clk_offset_pending=1;
        ret=HCI_ReadClockOffset(handle); 
        if (ret)
            return ret;

       

    }
    else
    {
        if (cause)
            info.cause=&cause;
        prh_tcs_gm_encode_listen_reject(&info, l2_conn);
    }
    return BT_NOERROR;
}
    

#endif


#if PRH_TCS_TERMINAL
APIDECL1 t_api APIDECL2 TCS_TLConfigAcceptMode(int mode)
{
  if (mode>PRH_TCS_WUG_ACCEPT_CONFIG || mode<0)
    return BT_INVALIDPARAM;
  
  prh_tcs_wug_config_accept_mode=mode;
  return BT_NOERROR;
}
#endif

