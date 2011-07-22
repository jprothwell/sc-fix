/***********************************************************************
 *
 * MODULE NAME:    rfcomm_l2cap.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    RFCOMM L2CAP Channel Handler non-state machine related
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  15 March 2000
 *
 * SOURCE CONTROL: $Id: rfcomm_l2cap.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Inc.
 *     All rights reserved.
 *
 ***********************************************************************/

#include "rfcomm_lower_layer.h"
#include "rfcomm_ue_api.h"
#include "rfcomm_core.h"
#include "rfcomm_frame.h"
#include "rfcomm_proto.h"
#include "rfcomm_debug.h"

#if COMBINED_HOST==1
struct prh_rfcomm_l2cap_inst rfcomm_insts;
#endif
/*
 * prh_rfcomm_l2cap_init
 *
 * Initialises L2CAP Channel Handler.
 * Checks does an instance to the remote address exist already, if so
 * it returns that instance, otherwise it creates a new instance, initialises
 * it and sends a START event to it
 */
int prh_rfcomm_l2cap_init(struct prh_rfcomm_l2cap_inst **l2cap_inst_ptr, t_bdaddr bd_addr)
{
  struct prh_rfcomm_l2cap_inst *l2cap_inst;
  int ret;

  ret=prh_rfcomm_lookup_l2cap_inst_by_bd_addr(bd_addr, &l2cap_inst);
  if (ret==BT_INVALIDPARAM)
    {
#if COMBINED_HOST==1
      l2cap_inst = &rfcomm_insts;
#else
      l2cap_inst=pMalloc(sizeof(struct prh_rfcomm_l2cap_inst));
#endif
      if (l2cap_inst==pNULL)
	return BT_NORESOURCES;
      
      pMemset(l2cap_inst, 0, sizeof(struct prh_rfcomm_l2cap_inst));

      /* need to create the l2cap instance mutex locked */
      l2cap_inst->lock=pMutexCreate(TRUE);
      l2cap_inst->dlc_head=pNULL;

      l2cap_inst->sys_params.max_frame_size=PRH_RFCOMM_MAX_FRAME_SIZE;

      prh_rfcomm_l2cap_next_state(l2cap_inst, PRH_RFCOMM_L2CAP_IDLE);
      ret=prh_rfcomm_add_l2cap_inst(l2cap_inst);
      if (ret!=BT_NOERROR)
	{
	  pMutexFree(l2cap_inst->lock);
	  pFree(l2cap_inst);
	  return ret;
	}
      
      *l2cap_inst_ptr=l2cap_inst;
      if (prh_rfcomm_core->initial_credit_state==0)
	l2cap_inst->credit_state=CREDIT_OFF;
      else
	l2cap_inst->credit_state=CREDIT_NOTSELECTED;
      return BT_INVALIDPARAM;
    }
  else
    pMutexLock(l2cap_inst->lock);
  *l2cap_inst_ptr=l2cap_inst;
  return BT_NOERROR;
}

/* 
 * prh_rfcomm_l2cap_data_ind_cb 
 * -- Callback from L2CAP to handle data on a channel.
 *
 * This callback looks up the instance of the RFCOMM L2CAP channel handler
 * depending on the cid, and if it is found passes the host_buf to this
 * instance as an event. If not instance is found the host buf is freed 
 * and the function returns.
 */
void prh_rfcomm_l2cap_data_ind_cb(u_int16 cid, struct host_buf *p_buf)
{
  struct prh_rfcomm_event_info info;
  struct prh_rfcomm_l2cap_inst *rfcomm_l2cap_ptr;
  int ret;

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"prh_rfcomm_l2cap_data_ind_cb cid=%d\n",cid));
#endif  

  /* can make this function non-PSM specific, means adding a
     lookup for CID -> to L2CAP instance - done*/
  ret=prh_rfcomm_lookup_l2cap_inst_by_cid(cid, &rfcomm_l2cap_ptr);
  if (ret==BT_INVALIDPARAM)
    {
      host_buf_free(p_buf);
    }
  else
    {
      info.buffer=p_buf;
      prh_rfcomm_l2cap_do_event(rfcomm_l2cap_ptr, PRH_RFCOMM_L2CAP_DATA_IND, &info);
    }
  return;
}

/*
 * prh_rfcomm_l2cap_disc_ind_cb
 * -- Callback from L2CAP to handle a disconnect indication on a channel
 *
 * The L2CAP cid is used to find the correct instance of the RFCOMM L2CAP
 * channel handler, and the disconnect ind is dispatched to it as an event.
 */
void prh_rfcomm_l2cap_disc_ind_cb(u_int8 eventType, t_L2_ControlCallbackArgs *args)
{
  struct prh_rfcomm_event_info info;
  struct prh_rfcomm_l2cap_inst *l2cap_inst;
  int ret;

  ret=prh_rfcomm_lookup_l2cap_inst_by_cid(args->cid, &l2cap_inst);
  if (ret==BT_NOERROR)
    {
      info.cid=args->cid;
      prh_rfcomm_l2cap_do_event(l2cap_inst, PRH_RFCOMM_L2CAP_DISC_IND, &info);
    }
  return;
}

/*
 * prh_rfcomm_l2cap_disc_cfm_cb
 * -- Callback from L2CAP to handle a disconnect confirm on a channel
 *
 * The L2CAP cid is used to find the correct instance of the RFCOMM L2CAP
 * channel handler, and the disconnect cfm is dispatched to it as an event.
 */
void prh_rfcomm_l2cap_disc_cfm_cb(u_int8 eventType, t_L2_ControlCallbackArgs *args)
{
  struct prh_rfcomm_event_info info;
  struct prh_rfcomm_l2cap_inst *l2cap_inst;
  int ret;

  ret=prh_rfcomm_lookup_l2cap_inst_by_cid(args->cid, &l2cap_inst);
  if (ret==BT_NOERROR)
    {
      info.priv=args;
      info.cid=args->cid;
      prh_rfcomm_l2cap_do_event(l2cap_inst, PRH_RFCOMM_L2CAP_DISC_CFM, &info);
    }
  return;
}

/*
 * prh_rfcomm_l2cap_connect_configured_cfm_cb
 * -- Callback from L2CAP to handle a connect confirmation on a channel
 *
 * The L2CAP cid is used to find the correct instance of the RFCOMM L2CAP
 * channel handler, and the connect confirm is dispatched to it as an event.
 */

void prh_rfcomm_l2cap_connect_configured_cfm_cb(u_int8 eventType, t_L2_ControlCallbackArgs *args)
{
  struct prh_rfcomm_event_info info;
  struct prh_rfcomm_l2cap_inst *rfcomm_l2cap_ptr;
  int ret;

  ret=prh_rfcomm_lookup_l2cap_inst_by_bd_addr(args->BDAddress, &rfcomm_l2cap_ptr);

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"prh_rfcomm_l2cap_connect_configured_cfm_cb ret=%d\n",ret));
#endif  
  
  if (ret==BT_NOERROR)
    {
      info.cid=args->cid;
      info.priv=args;
      
      /* if this is only a more info event ignore it */
      if (args->result==0 || args->result>1)
	prh_rfcomm_l2cap_do_event(rfcomm_l2cap_ptr, PRH_RFCOMM_L2CAP_CONNECT_CONFIG_CFM, &info);
    }
  else
    {
#if pDEBUG
      pDebugPrintfEX((pLOGNOTICE,pLOGRFCOMM,"RFCOMM: L2CAP Connect Cfm recevied unexpectedly\n"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
      SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM,LOG_RFC_RFCOMM_L2CAP_INDEX,NULL,0,0);
#endif      
    }
  return;
}

/*
 * The RFCOMM L2CAP Channel Handler, stores a linked list of DLCs currently
 * established on it. These functions deal with this linked list
 */

/*
 * prh_rfcomm_reg_dlc_instance
 * -- Register an instance of a DLC on an L2CAP Channel. specified whether we own lock or not
 * we must own the lock.. DO NOT CALL THIS FUNCTION WITHOUT THE LOCK
 */
int prh_rfcomm_reg_dlc_instance(struct prh_rfcomm_l2cap_inst *self, prh_t_rfcomm_dlci dlci, struct prh_rfcomm_dlc_inst *instance)
{
  int ret;
  instance->rfcomm_l2cap_ptr=self;
  ret=prh_host_ll_add(&(self->dlc_head), (void *)instance);
  if (ret==BT_NOERROR)
    self->dlc_count++;
  return ret;
}

/*
 * prh_rfcomm_dlc_dlci_cmp
 * -- DLC Linked List Traverse helper function
 *
 * This function is used to compare a DLCI with DLCIs in the list
 */
int prh_rfcomm_dlc_dlci_cmp_helper(void *cmpin, void *compwith)
{
  struct prh_rfcomm_dlc_inst *dlc_inst=compwith;
  prh_t_rfcomm_dlci dlci = (prh_t_rfcomm_dlci)(u_int32)cmpin;
  
  return(dlc_inst->dlci==dlci);
}

/*
 * prh_rfcomm_lookup_dlci_instance
 * -- Lookup a DLC by DLCI
 */
int prh_rfcomm_lookup_dlci_instance(struct prh_rfcomm_l2cap_inst *self, prh_t_rfcomm_dlci dlci, struct prh_rfcomm_dlc_inst **instance)
{
  int ret;
  pMutexLock(self->lock);
  ret=prh_host_ll_search(&(self->dlc_head), (void **)instance, (void *)(u_int32)dlci, &prh_rfcomm_dlc_dlci_cmp_helper);
  pMutexUnlock(self->lock);
  return ret;
}

/* this is entered with L2CAP locked already .. it locks DLCI on way out */
/* prereq: Must have L2CAP lock
 *  postreq: DLC is locked
 */
int prh_rfcomm_lookup_dlci_instance_and_lock(struct prh_rfcomm_l2cap_inst *self, prh_t_rfcomm_dlci dlci, struct prh_rfcomm_dlc_inst **instance)
{
  int ret;
  ret=prh_host_ll_search(&(self->dlc_head), (void **)instance, (void *)(u_int32)dlci, &prh_rfcomm_dlc_dlci_cmp_helper);
  if (ret==BT_NOERROR)
    pMutexLock((*instance)->lock);
  return ret;
}

/*
 * prh_rfcomm_remove_dlc_instance
 * -- Remove a DLC instance from an L2CAP Channel list, by DLCI.
 */
int prh_rfcomm_remove_dlc_instance(struct prh_rfcomm_l2cap_inst *self, prh_t_rfcomm_dlci dlci, struct prh_rfcomm_dlc_inst **ret_inst)
{
  struct prh_rfcomm_dlc_inst *returned;
  int ret;
  
  pMutexLock(self->lock);
  ret=prh_host_ll_remove(&(self->dlc_head), (void**)&returned, (void *)(u_int32)dlci, &prh_rfcomm_dlc_dlci_cmp_helper);
  if (ret==BT_NOERROR)
    {
      self->dlc_count--;
      /* if there is a timer on the DLC cancel it */
      if (returned->pfbit_timer)
      	{
		    pTimerCancel(returned->pfbit_timer);
			returned->pfbit_timer = 0;
      	}
      /* let the callee do the actual freeing */
      if (ret_inst!=NULL)
	*ret_inst=returned;
    }
  pMutexUnlock(self->lock);
  return ret;
}

/*
 * prh_rfcomm_dlc_remove_all_helper
 * -- Linked List Traverse Function called per single DLC instance.
 * 
 * func_info is used to determine whether the upper layer should
 * be called back or not.
 * 
 */
int prh_rfcomm_dlc_remove_all_helper(void *data, void *func_info)
{
  struct prh_rfcomm_dlc_inst *inst=(struct prh_rfcomm_dlc_inst *)data;
  u_int32 do_release=(u_int32)func_info;
  struct prh_rfcomm_event_info info;

  pMutexLock(inst->lock);
  
  if (inst->pfbit_timer)
  	{
    pTimerCancel(inst->pfbit_timer);
	inst->pfbit_timer = 0;
  	}
  if (inst->dlci!=0)
    if (do_release)
      {
	pMutexUnlock(inst->lock);
	pMutexUnlock(inst->rfcomm_l2cap_ptr->lock);
	/* hold no locks calling this one */
	prh_rfcomm_dlc_do_event(inst, PRH_RFCOMM_DLC_CLOSE_NOW, &info);
	pMutexLock(inst->rfcomm_l2cap_ptr->lock);
	pMutexLock(inst->lock);
      }
  
  prh_rfcomm_free_dlc_instance(inst);
  return BT_NOERROR;
}

/*
 * prh_rfcomm_remove_all_dlc_instance
 * -- Remove all DLC instance from an L2CAP list.
 *
 * Uses the helper function prh_rfcomm_dlc_remove_all_helper, to remove
 * all DLC instances and callback the upper layers depending on do_release.
 * this must be entered with the L2CAP Instance LOCKED 
 */
int prh_rfcomm_remove_all_dlc_instance(struct prh_rfcomm_l2cap_inst *self, int do_release)
{
  int ret;

  ret=prh_host_ll_remove_all(&(self->dlc_head), &prh_rfcomm_dlc_remove_all_helper, (void *)do_release);
  self->dlc_head=pNULL;
  self->dlc_count=0;
  return ret;

}

int prh_rfcomm_free_dlc_instance(struct prh_rfcomm_dlc_inst *self)
{

  pMutexLock(self->credit_lock);
  pMutexUnlock(self->credit_lock);
  pMutexFree(self->credit_lock);
  pMutexUnlock(self->lock);
  pMutexFree(self->lock);
  pFree(self);
  return BT_NOERROR;
}

