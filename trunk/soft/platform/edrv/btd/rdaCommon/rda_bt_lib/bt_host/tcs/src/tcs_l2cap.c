/***********************************************************************
 *
 * MODULE NAME:    tcs_l2cap.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    TCS L2CAP Channel Handler non-state machine related
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  15 March 2000
 *
 * SOURCE CONTROL: $Id: tcs_l2cap.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:

 *
 ***********************************************************************/
#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)

#include "tcs_lower_layer.h"
#include "host_ll.h"
#include "mgr_core.h"
#include "mgr_utils.h"

#include "tcs_core.h"
#include "tcs_l2cap.h"
#include "tcs_cc_callbacks.h"
#include "tcs_cc_core.h"
#include "tcs_wug.h"
#include "tcs_gm_types.h"
#include "tcs_proto.h"

int prh_tcs_remove_l2cap_inst_by_cid(u_int16 cid, struct prh_st_tcs_l2conn **l2cap_inst_ptr);
int prh_tcs_lookup_pending_l2cap_inst_by_bd_addr(t_bdaddr bd_addr, struct prh_st_tcs_l2conn **l2cap_inst_ptr);
int prh_tcs_pending_l2cap_bdaddr_cmp(void *cmpin, void *compwith);
int prh_tcs_free_tcsbin_l2cap_bdaddr_cmp(void *cmpin, void *compwith);
int prh_tcs_free_tcscordless_l2cap_bdaddr_cmp(void *cmpin, void *compwith);
int prh_tcs_l2cap_state_cmp(void *cmpin, void *compwith);

int prh_tcs_device_supports_park;
t_pMutex prh_tcs_l2cap_mutex;
extern t_pMutex prh_tcs_wug_lock;

void (*prh_tcs_connectIndCallback)(struct prh_st_tcs_l2conn * l2conn) = NULL;
void (*prh_tcs_disconnectIndCallback)(struct prh_st_tcs_l2conn * l2conn) = NULL;

/* 
 * prh_tcs_l2cap_data_ind_cb 
 * -- Callback from L2CAP to handle data on a channel.
 *
 */

void prh_tcs_l2cap_data_ind_cb(u_int16 cid, struct host_buf *p_buf)
{
  int ret = BT_NOERROR;
  struct prh_st_tcs_l2conn *l2_conn = NULL;

  if(L2CAP_CONNECTIONLESS_DATA != cid)
	  ret=prh_tcs_lookup_l2cap_inst_by_cid(cid, &l2_conn);
  
  if(l2_conn)
  {
	/* If data is from a connection-oriented channel */
	if(PRH_TCS_L2CAP_OPEN != l2_conn->l2chan_state)
	{
		ret = BT_DISALLOWED;
		host_buf_free(p_buf);
	}

	if(PRH_TCS_L2CAP_CHAN_CALL != l2_conn->type)
		return;
  }

  if (ret==BT_NOERROR)
  {
	  switch(p_buf->data[0] & 0xE0)
	  {
	  case PRH_TCS_PROTO_DISC_CC:
		  prh_tcs_cc_callback_L2Data(l2_conn, p_buf);
		  break;
	  case PRH_TCS_PROTO_DISC_GM:
		  prh_tcs_gm_process_event(l2_conn, p_buf);
		  break;
	  case PRH_TCS_PROTO_DISC_CL:
		  break;
	  }
  }

}

/*
 * prh_tcs_l2cap_connect_configured_cfm_cb
 * -- Callback from L2CAP to handle a connect confirmation on a channel
 *
 * The L2CAP cid is used to find the correct instance of the TCS L2CAP
 * channel handler, and the connect confirm is dispatched to it as an event.
 */

void prh_tcs_l2cap_connect_configured_cfm_cb(u_int8 eventType, t_L2_ControlCallbackArgs *args)
{
  int ret;
  struct prh_st_tcs_l2conn *l2_conn;

  ret = prh_tcs_lookup_pending_l2cap_inst_by_bd_addr(args->BDAddress, &l2_conn);
  if (ret==BT_NOERROR)
    {
      if (args->result==0)
	{
	  
		l2_conn->cid=args->cid;
	  l2_conn->l2chan_state=PRH_TCS_L2CAP_OPEN;
	  if (l2_conn->conn_callback)
	    (*l2_conn->conn_callback)(BT_NOERROR, l2_conn);
	  if (prh_tcs_connectIndCallback)
	    prh_tcs_connectIndCallback(l2_conn); 
	}
      else
	if (args->result>1)
	  {
	    /* Tell someone we didn't get a connection */
	    /*l2_conn->call->callback */
			if (l2_conn->conn_callback)
				(*l2_conn->conn_callback)(args->result, l2_conn);
	    prh_tcs_remove_l2cap_inst_by_cid(args->cid, &l2_conn);
	    pFree(l2_conn);
	  }
      
    }
  else
    pDebugPrintf((pLOGWARNING, "TCS: Got L2CAP Connect Cfm in in-correct state\n"));

  return;
}

/*
 * prh_tcs_l2cap_disc_ind_cb
 * -- Callback from L2CAP to handle a disconnect indication on a channel
 *
 * The L2CAP cid is used to find the correct instance of the TCS L2CAP
 * channel handler, and the disconnect ind is dispatched to it as an event.
 */
void prh_tcs_l2cap_disc_ind_cb(u_int8 eventType, t_L2_ControlCallbackArgs *args)
{
	int ret;
	struct prh_st_tcs_l2conn *l2_conn;
#if PRH_TCS_GATEWAY
	struct st_t_TCS_TLPeer *TLPeer; 
#endif
	ret=prh_tcs_lookup_l2cap_inst_by_cid(args->cid, &l2_conn);
	if (ret==BT_NOERROR)
    {
		/* must do this before calling prh_tcs_disconnectIndCallback */
#if PRH_TCS_GATEWAY

		TLPeer = prh_tcs_cc_core_getTLPeerByBdAddress(l2_conn->bd_addr);
		if (TLPeer)
			if(TLPeer->cid == args->cid)
				prh_tcs_wug_remove_member(l2_conn->bd_addr);
#endif

		if(prh_tcs_disconnectIndCallback)
			prh_tcs_disconnectIndCallback(l2_conn);

		prh_tcs_remove_l2cap_inst_by_cid(args->cid, &l2_conn);
		pFree((void*)l2_conn);
    }
	return;
}

/*
 * prh_tcs_l2cap_disc_cfm_cb
 * -- Callback from L2CAP to handle a disconnect confirm on a channel
 *
 * The L2CAP cid is used to find the correct instance of the TCS L2CAP
 * channel handler, and the disconnect cfm is dispatched to it as an event.
 */
void prh_tcs_l2cap_disc_cfm_cb(u_int8 eventType, t_L2_ControlCallbackArgs *args)
{
  int ret;
  struct prh_st_tcs_l2conn *l2_conn;
  
  ret=prh_tcs_lookup_l2cap_inst_by_cid(args->cid, &l2_conn);
  if (ret==BT_NOERROR)
    {
#if PRH_TCS_GATEWAY

			struct st_t_TCS_TLPeer *TLPeer; 
      
      TLPeer = prh_tcs_cc_core_getTLPeerByBdAddress(l2_conn->bd_addr);
      if (TLPeer)
	if(TLPeer->cid == args->cid)
	  prh_tcs_wug_remove_member(l2_conn->bd_addr);
      
#endif
  	  if(prh_tcs_disconnectIndCallback)
		  prh_tcs_disconnectIndCallback(l2_conn);
	  prh_tcs_remove_l2cap_inst_by_cid(args->cid, &l2_conn);
	  pFree((void*)l2_conn);
    }
  return;
}

/*
 * prh_tcs_l2cap_connect_ind_cb
 * -- Callback from L2CAP to handle a connect indication on a channel
 *
 * The L2CAP cid is used to find the correct instance of the TCS L2CAP
 * channel handler, and the config ind is dispatched to it as an event.
 */
void prh_tcs_l2cap_connect_ind_cb(u_int8 eventType, t_L2_ControlCallbackArgs *args)
{
   struct prh_st_tcs_l2conn *new_device;
   int ret;
  t_L2_ConfigReqParams req_params;
  u_int16 mtu=672;
  
  new_device=(struct prh_st_tcs_l2conn *)pMalloc(sizeof(struct prh_st_tcs_l2conn));
  if(new_device==pNULL)
      return;
  pMemset(new_device, 0, sizeof(struct prh_st_tcs_l2conn));
  new_device->bd_addr=args->BDAddress;

  req_params.inMTUmax=mtu+4;
  req_params.outMTUmax=mtu+4;
  req_params.inMTUmin=48;
  req_params.outMTUmin=48;
  req_params.flushTO=0xffff;
  req_params.linkTO=0;
  
  new_device->l2chan_state=PRH_TCS_L2CAP_W4CONN;
  
  new_device->cid=args->cid;
  new_device->type=PRH_TCS_L2CAP_CHAN_CALL;
  new_device->conn_callback=NULL;
  new_device->psm = args->psm;

  ret=prh_tcs_add_l2cap_inst(new_device);
  
  L2_ConnectResponseandConfig(args->BDAddress, args->cid, 0, 0, &req_params);
  
  return;

}


int prh_tcs_register_connectionCallbacks( void (*connectIndCallback)(struct prh_st_tcs_l2conn * l2conn), void (*disconnectIndCallback)(struct prh_st_tcs_l2conn * l2conn))
{
	/* Register callbacks for new tcs channels and tcs channel disconnections */
	prh_tcs_connectIndCallback = connectIndCallback;	
	prh_tcs_disconnectIndCallback = disconnectIndCallback;

	return BT_NOERROR;
}


/*
 * prh_tcs_init
 * -- Initialise the TCS Protocol
 *
 */
int prh_tcs_initialised=0;

int prh_tcs_init(int flags)
{
    u_int8 status;
  struct l2cap_cbs callbacks;
  u_int8 features[NUM_BYTES_LMP_FEATURES];
  
  
  if (!prh_tcs_initialised)
    {
      prh_tcs_l2cap_mutex=pMutexCreate(FALSE);
      
      /* Register call back handler with L2CAP for all CBs on this PSM */
      callbacks.timeoutIndication=NULL;/* prh_tcs_l2cap_disc_ind_cb or prh_tcs_l2cap_disc_cfm_cb ?*/
      callbacks.flushOccurred=NULL;
      callbacks.bufferOverflow=NULL;
      callbacks.connectionIndication=prh_tcs_l2cap_connect_ind_cb;
      callbacks.qosviolationIndication=NULL;
      callbacks.dataIndication=prh_tcs_l2cap_data_ind_cb;
      callbacks.disconnectionIndication=prh_tcs_l2cap_disc_ind_cb;
      callbacks.disconnectionConfirmation=prh_tcs_l2cap_disc_cfm_cb;

      callbacks.connectionConfirmation=NULL;      
      callbacks.configureIndication=NULL;
      callbacks.configureConfirmation=NULL;
      callbacks.connectionConfiguredConfirmation=prh_tcs_l2cap_connect_configured_cfm_cb;

      if(PRH_TCS_ENABLE_TCSBIN(flags))
      {
        status=L2_RegisterPSM(PRH_TCS_PSM_TCSBIN, &callbacks);
        if(status != BT_NOERROR)
          return status;
      }


      if(PRH_TCS_ENABLE_TCSCORDLESS(flags))
      {
        status=L2_RegisterPSM(PRH_TCS_PSM_TCSCORDLESS, &callbacks);
        if(status != BT_NOERROR)
              return status;
      }
      
      prh_tcs_wug_lock=pMutexCreate(FALSE);
      prh_tcs_wug_listen_lock=pMutexCreate(FALSE);
      
      MGR_GetLocalFeatures(features);
      if (PRH_PARK_MODE_SUPPORTED(features))
	prh_tcs_device_supports_park=1;
      else
	prh_tcs_device_supports_park=0;
      
      prh_tcs_initialised=1;
      
    }
  
  return BT_NOERROR;
}

int prh_tcs_connect_device_control(t_bdaddr bd_addr, u_int16 psm, struct st_t_TCS_callInstance *call, t_api(*callback_func_ptr)(t_api status, struct prh_st_tcs_l2conn *l2_conn))
{
  /* */
  struct prh_st_tcs_l2conn *new_device;
  int ret;
  t_L2_ConfigReqParams req_params;
  u_int16 mtu=672;

  new_device=(struct prh_st_tcs_l2conn *)pMalloc(sizeof(struct prh_st_tcs_l2conn));
  if (new_device==pNULL)
	return BT_NORESOURCES;

  req_params.inMTUmax=mtu+4;
  req_params.outMTUmax=mtu+4;
  req_params.inMTUmin=48;
  req_params.outMTUmin=48;
  req_params.flushTO=0xffff;
  req_params.linkTO=0;

  pMemset(new_device, 0, sizeof(struct prh_st_tcs_l2conn));
  new_device->bd_addr=bd_addr;
  new_device->l2chan_state=PRH_TCS_L2CAP_W4CONN;
  new_device->un.call = call;
  new_device->type=PRH_TCS_L2CAP_CHAN_CALL;
  new_device->conn_callback=callback_func_ptr;
  new_device->psm = psm;
  ret=prh_tcs_add_l2cap_inst(new_device);

  if (ret==BT_NOERROR)
    {
      ret=L2_ConnectandConfig(new_device->bd_addr, new_device->psm, &req_params);
      if (ret!=BT_PENDING && ret!=BT_NOERROR)
	{
	  /*	  prh_tcs_remove_l2cap_inst_by_(new_device);*/
	  pFree(new_device);
	  return BT_NORESOURCES;
	}
    }
  return BT_NOERROR;
}

int prh_tcs_connect_device_acl_bearer(void)
{

	return BT_UNSUPPORTED;
}

struct prh_host_gen_ll *prh_tcs_l2cap_head;

int prh_tcs_add_l2cap_inst(struct prh_st_tcs_l2conn *new_ent)
{
  int ret;
  
  pMutexLock(prh_tcs_l2cap_mutex);
  ret=prh_host_ll_add(&prh_tcs_l2cap_head, (void *)new_ent);
  pMutexUnlock(prh_tcs_l2cap_mutex);
  return ret;
}

int prh_tcs_l2cap_cid_cmp(void *cmpin, void *compwith)
{
  struct prh_st_tcs_l2conn *device=(struct prh_st_tcs_l2conn *)compwith;
  u_int16 cid=(u_int16)(u_int32)cmpin;
  
  return (device->cid==cid);
}

/*
 * prh_tcs_lookup_l2cap_inst_by_cid
 * -- Finds an instance of TCS stuff by CID
 *
 * Searches linked list using prh_tcs_l2cap_cid_cmp
 */
int prh_tcs_lookup_l2cap_inst_by_cid(u_int16 cid, struct prh_st_tcs_l2conn **l2cap_inst_ptr)
{
  int ret;
  pMutexLock(prh_tcs_l2cap_mutex);
  /* passes in the address of the list head... pretty messy. */
  ret=prh_host_ll_search(&prh_tcs_l2cap_head, (void **)(l2cap_inst_ptr), (void *)(u_int32)cid, &prh_tcs_l2cap_cid_cmp);
  pMutexUnlock(prh_tcs_l2cap_mutex);
  return ret;
}

int prh_tcs_remove_l2cap_inst_by_cid(u_int16 cid, struct prh_st_tcs_l2conn **l2cap_inst_ptr)
{
  int ret;
  pMutexLock(prh_tcs_l2cap_mutex);
  /* passes in the address of the list head... pretty messy. */
  ret=prh_host_ll_remove(&prh_tcs_l2cap_head, (void **)(l2cap_inst_ptr), (void *)(u_int32)cid, &prh_tcs_l2cap_cid_cmp);
  pMutexUnlock(prh_tcs_l2cap_mutex);
  return ret;
} 
 
/*
 * prh_tcs_l2cap_bdaddr_cmp
 * -- L2CAP Channel Handler Linked List helper traverse function
 *
 * This is used to compare a bd address with the entries in the list
 */
int prh_tcs_l2cap_bdaddr_cmp(void *cmpin, void *compwith)
{
  struct prh_st_tcs_l2conn *instance=(struct prh_st_tcs_l2conn *)compwith;
  t_bdaddr bd_addr=*((t_bdaddr *)cmpin);
  
  return (BD_ADDR_CMP(bd_addr, instance->bd_addr));
}

/*
 * prh_tcs_lookup_l2cap_inst_by_bd_addr
 * -- Finds an instance of L2CAP Channel Handler using a BD Address
 *
 * Searches linked list using prh_tcs_l2cap_bdaddr_cmp
 */
int prh_tcs_lookup_l2cap_inst_by_bd_addr(t_bdaddr bd_addr, struct prh_st_tcs_l2conn **l2cap_inst_ptr)
{
  int ret;
  /* passes in the address of the list head...  */
  pMutexLock(prh_tcs_l2cap_mutex);  
  ret=prh_host_ll_search(&prh_tcs_l2cap_head, (void *)l2cap_inst_ptr, (void *)&bd_addr, &prh_tcs_l2cap_bdaddr_cmp);
  pMutexUnlock(prh_tcs_l2cap_mutex);
  return ret;
}

int prh_tcs_lookup_pending_l2cap_inst_by_bd_addr(t_bdaddr bd_addr, struct prh_st_tcs_l2conn **l2cap_inst_ptr)
{
  int ret;
  /* passes in the address of the list head...  */
  pMutexLock(prh_tcs_l2cap_mutex);  
  ret=prh_host_ll_search(&prh_tcs_l2cap_head, (void *)l2cap_inst_ptr, (void *)&bd_addr, &prh_tcs_pending_l2cap_bdaddr_cmp);
  pMutexUnlock(prh_tcs_l2cap_mutex);
  return ret;
}


int prh_tcs_pending_l2cap_bdaddr_cmp(void *cmpin, void *compwith)
{
  struct prh_st_tcs_l2conn *instance=(struct prh_st_tcs_l2conn *)compwith;
  t_bdaddr bd_addr=*((t_bdaddr *)cmpin);
  return (BD_ADDR_CMP(bd_addr, instance->bd_addr) && (PRH_TCS_L2CAP_W4CONN == instance->l2chan_state) );
}

/*
 * prh_tcs_free_tcsbin_l2cap_bdaddr_cmp
 * -- L2CAP Channel Handler Linked List helper traverse function
 *
 * This is used to a find a free TCSBIN call-related l2cap instance, based on a match of bdaddress 
 */
int prh_tcs_free_tcsbin_l2cap_bdaddr_cmp(void *cmpin, void *compwith)
{
  struct prh_st_tcs_l2conn *instance=(struct prh_st_tcs_l2conn *)compwith;
  t_bdaddr bd_addr=*((t_bdaddr *)cmpin);
  
  return (BD_ADDR_CMP(bd_addr, instance->bd_addr) && NULL == instance->un.call && PRH_TCS_PSM_TCSBIN == instance->psm);
}


/*
 * prh_tcs_free_tcsbin_l2cap_bdaddr_cmp
 * -- L2CAP Channel Handler Linked List helper traverse function
 *
 * This is used to a find a free TCSCORDLESS call-related l2cap instance, based on a match of bdaddress 
 */

int prh_tcs_free_tcscordless_l2cap_bdaddr_cmp(void *cmpin, void *compwith)
{
  struct prh_st_tcs_l2conn *instance=(struct prh_st_tcs_l2conn *)compwith;
  t_bdaddr bd_addr=*((t_bdaddr *)cmpin);
  
  return (BD_ADDR_CMP(bd_addr, instance->bd_addr) && NULL == instance->un.call && PRH_TCS_PSM_TCSCORDLESS == instance->psm);
}


int prh_tcs_lookup_free_l2cap_inst(t_bdaddr bd_addr, u_int16 psm, struct prh_st_tcs_l2conn **l2cap_inst_ptr)
{
	int ret;
	
	pMutexLock(prh_tcs_l2cap_mutex);  
	if(PRH_TCS_PSM_TCSBIN == psm)
		ret=prh_host_ll_search(&prh_tcs_l2cap_head, (void *)l2cap_inst_ptr, (void *)&bd_addr, &prh_tcs_free_tcsbin_l2cap_bdaddr_cmp);
	else if(PRH_TCS_PSM_TCSCORDLESS == psm)
		ret=prh_host_ll_search(&prh_tcs_l2cap_head, (void *)l2cap_inst_ptr, (void *)&bd_addr, &prh_tcs_free_tcscordless_l2cap_bdaddr_cmp);
	else
		ret = BT_INVALIDPARAM;
	pMutexUnlock(prh_tcs_l2cap_mutex);
	return ret;
}


#if PRH_TCS_GATEWAY
t_bdaddr *prh_tcs_l2cap_findConnectedTL(void)
{
	struct prh_st_tcs_l2conn *l2conn;
	t_api status;

	pMutexLock(prh_tcs_l2cap_mutex);
	status = prh_host_ll_search(&prh_tcs_l2cap_head, (void **)(&l2conn), (void *)PRH_TCS_L2CAP_OPEN, prh_tcs_l2cap_state_cmp);
	pMutexUnlock(prh_tcs_l2cap_mutex);

	if(!status && l2conn)
		return &(l2conn->bd_addr);
	else
		return NULL;
}


int prh_tcs_l2cap_state_cmp(void *cmpin, void *compwith)
{
  struct prh_st_tcs_l2conn *device=(struct prh_st_tcs_l2conn *)compwith;
  PRH_TCS_L2CAP_STATE state =  (PRH_TCS_L2CAP_STATE)(u_int32)cmpin;
  
  return (device->l2chan_state == state);
}


#endif /* PRH_TCS_GATEWAY */
#endif/* (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/
