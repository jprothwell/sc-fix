/***********************************************************************
 *
 * MODULE NAME:    prh_rfcomm_core.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    RFCOMM Manager Functionality
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  15 March 2000
 *
 * SOURCE CONTROL: $Id: rfcomm_core.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Inc.
 *     All rights reserved.
 *
 ***********************************************************************/


#include "rfcomm_lower_layer.h"
#include "host_ctrl.h"
#include "rfcomm_ue_api.h"
#include "rfcomm_core.h"

#include "rfcomm_debug.h"

#include "rfcomm_proto.h"

t_api prh_rfcomm_l2cap_do_event(struct prh_rfcomm_l2cap_inst *l2cap_inst, PRH_RFCOMM_EVENT eent, struct prh_rfcomm_event_info *info);
void prh_rfcomm_l2cap_next_state(struct prh_rfcomm_l2cap_inst *l2cap_inst, PRH_RFCOMM_L2CAP_STATE state);
int prh_rfcomm_l2cap_init(struct prh_rfcomm_l2cap_inst **l2cap_inst_ptr, t_bdaddr bd_addr);

void prh_rfcomm_l2cap_connect_configured_cfm_cb(u_int8 eventType, t_L2_ControlCallbackArgs *args);

/* Global to hold a pointer to the RFCOMM core manager structure */
#if COMBINED_HOST==1
struct prh_rfcomm_inst _prh_rfcomm_core, *prh_rfcomm_core=&_prh_rfcomm_core;
#else
struct prh_rfcomm_inst *prh_rfcomm_core=pNULL;
#endif
/* if something frees this anything can happen */
t_pMutex prh_rfcomm_core_lock;

/* 
 * prh_rfcomm_init
 */

#if PRH_RFCOMM_RSE_SUPPORT==1
t_api prh_rse_layer_ctrl(u_int8 flags);
#endif
#if PRH_RFCOMM_PE_SUPPORT==1
t_api prh_pe_layer_ctrl(u_int8 flags);
#endif
  
/* 
 * prh_rfcomm_init
 * -- Initialise the RFCOMM stack 
 *
 * This function is called by the Port Entity to start a new instance 
 * of RFCOMM. This function initialises an RFCOMM entity in the IDLE state.
 * 
 * If an instance exists already it returns a pointer to it.
 */
int prh_rfcomm_core_init(u_int32 flags)
{
  /* Does an RFCOMM core entity exist in the system,
   *  If not create one.
   *  Return a pointer to core manager for the instance
   */
  struct l2cap_cbs callbacks;
  
#if COMBINED_HOST==0
  if (prh_rfcomm_core==pNULL)
#endif
    {
      /* Register call back handler with L2CAP for all CBs on this PSM */
      callbacks.timeoutIndication=NULL;
      callbacks.flushOccurred=NULL;
      callbacks.bufferOverflow=NULL;
      callbacks.connectionIndication=prh_rfcomm_l2cap_connect_ind;
      callbacks.qosviolationIndication=NULL;
      callbacks.dataIndication=prh_rfcomm_l2cap_data_ind_cb;
      callbacks.disconnectionIndication=prh_rfcomm_l2cap_disc_ind_cb;
      callbacks.disconnectionConfirmation=prh_rfcomm_l2cap_disc_cfm_cb;

      callbacks.connectionConfirmation=NULL;
      callbacks.configureIndication=NULL;
      callbacks.configureConfirmation=NULL;
      callbacks.connectionConfiguredConfirmation=prh_rfcomm_l2cap_connect_configured_cfm_cb;

      L2_RegisterPSM(PRH_RFCOMM_PSM, &callbacks);
    }
#if COMBINED_HOST==0

      if (prh_rfcomm_core==pNULL)
#endif
	{
#if COMBINED_HOST==0
	  prh_rfcomm_core=pMalloc(sizeof(struct prh_rfcomm_inst));
	  
	  if (prh_rfcomm_core==pNULL)
	    return BT_NORESOURCES;
#endif

	  prh_rfcomm_core_lock=pMutexCreate(0);
	  
	  pMemset(prh_rfcomm_core, 0, sizeof(struct prh_rfcomm_inst));
	  	  
	  prh_rfcomm_core->initial_credit_state=1;
	}

  return BT_NOERROR;
}

/*
 * prh_rfcomm_shutdown
 * -- Reset the RFCOMM stack
 *
 * This function resets shutsdown all RFCOMM layers
 */
int prh_rfcomm_core_shutdown(u_int32 flags)
{
  prh_rfcomm_remove_all_l2cap_inst();
  prh_rfcomm_remove_all_servers();

  pFree(prh_rfcomm_core);
  pMutexFree(prh_rfcomm_core_lock);
  prh_rfcomm_core=pNULL;
  
  return 0;
}

/*
 * prh_rfcomm_layer_ctrl
 * -- Start/stop the RFCOMM Layer
 *
 */
t_api prh_rfcomm_layer_ctrl(u_int8 flags) {
    if (flags&PRH_HOST_STARTUP) {
#if PRH_RFCOMM_PE_SUPPORT==1
        prh_pe_layer_ctrl(flags);
#endif
#if PRH_RFCOMM_RSE_SUPPORT==1
	prh_rse_layer_ctrl(flags);
#endif
    } else if (flags&PRH_HOST_RESET) {
#if PRH_RFCOMM_RSE_SUPPORT==1
        prh_rse_layer_ctrl(PRH_HOST_SHUTDOWN);
#endif
#if PRH_RFCOMM_PE_SUPPORT==1
        prh_pe_layer_ctrl(PRH_HOST_SHUTDOWN);
#endif
        prh_rfcomm_entity_shutdown(flags);
        prh_rfcomm_core_shutdown(flags);
#if PRH_RFCOMM_PE_SUPPORT==1
        prh_pe_layer_ctrl(PRH_HOST_STARTUP);
#endif
#if PRH_RFCOMM_RSE_SUPPORT==1
        prh_rse_layer_ctrl(PRH_HOST_STARTUP);
#endif
    } else if (flags&PRH_HOST_SHUTDOWN) {
#if PRH_RFCOMM_RSE_SUPPORT==1
	prh_rse_layer_ctrl(flags);
#endif
#if PRH_RFCOMM_PE_SUPPORT==1
        prh_pe_layer_ctrl(flags);
#endif
        prh_rfcomm_entity_shutdown(flags);
        prh_rfcomm_core_shutdown(flags);
    }
    return BT_NOERROR;
}
/*
 * prh_rfcomm_check_channel_state
 *
 */
int prh_rfcomm_check_channel_state(prh_t_rfcomm_schan schan)
{
  int ret=0;
  
  pMutexLock(prh_rfcomm_core_lock);
  if (prh_rfcomm_core)
    {
      ret=((prh_rfcomm_core->channel_registered_state & (1U<<schan)) == (1U<<schan));
    }
  pMutexUnlock(prh_rfcomm_core_lock);
  return ret;
}

/*
 * prh_rfcomm_l2cap_connect_ind
 * -- L2CAP Connect Ind Callback
 *
 * Receive L2CAP Connection Indication from lower layers.
 * Initialise a new RFCOMM L2CAP state machine and passes the connect ind to it
 */
void prh_rfcomm_l2cap_connect_ind(u_int8 eventType, t_L2_ControlCallbackArgs *args)
{
  struct prh_rfcomm_event_info info;
  struct prh_rfcomm_l2cap_inst *l2cap_inst;
  int ret;

  /* call prh_rfcomm_l2cap_init to create a new RFCOMM L2CAP entity */
  /* let it deal with the L2CAP CONNECT IND */
  /* this locks the L2CAP instance */

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"prh_rfcomm_l2cap_connect_ind eventType=%d\n",eventType));
#endif  
  
  ret=prh_rfcomm_l2cap_init(&l2cap_inst, args->BDAddress);

  if (ret!=BT_NORESOURCES)
    {
      info.cid=args->cid;
      info.bd_addr=args->BDAddress;
      prh_rfcomm_l2cap_do_event(l2cap_inst, PRH_RFCOMM_L2CAP_CONNECT_IND, &info);
    }
  pMutexUnlock(l2cap_inst->lock);
  return;
}

/*
 * L2CAP Channels Linked List handler functions 
 *
 * These functions use the RFCOMM Generic linked list 
 * and will be moved to a generic host list when finalised
 */

/* 
 * prh_rfcomm_add_l2cap_inst
 * --  Add an instance of the L2CAP Channel Handler to the RFCOMM entity 
 */
int prh_rfcomm_add_l2cap_inst(struct prh_rfcomm_l2cap_inst *instance)
{
  int ret;
  pMutexLock(prh_rfcomm_core_lock);
  instance->rfcomm_ptr=prh_rfcomm_core;
  ret=prh_host_ll_add(&(prh_rfcomm_core->head), (void *)instance);
  pMutexUnlock(prh_rfcomm_core_lock);
  return ret;
}

/*
 * prh_rfcomm_l2cap_inst_cmp
 * -- L2CAP Channel Handler Linked List helper traverse function
 *
 * This is used to compare an L2CAP instance pointer
 */
int prh_rfcomm_l2cap_inst_cmp(void *cmpin, void *compwith)
{
  struct prh_rfcomm_l2cap_inst *instance=(struct prh_rfcomm_l2cap_inst *)compwith;
  struct prh_rfcomm_l2cap_inst *inst_check=(struct prh_rfcomm_l2cap_inst *)cmpin;
  
  return (instance==inst_check);
}

/*
 * prh_rfcomm_validate_l2cap_inst_by_bd_addr
 * -- Finds an instance of L2CAP Channel Handler using a BD Address
 *
 * Searches linked list using prh_rfcomm_l2cap_inst_cmp
 */
int prh_rfcomm_validate_and_lock_l2cap_inst_by_ptr(struct prh_rfcomm_l2cap_inst *l2cap_inst)
{
  int ret;
  struct prh_rfcomm_l2cap_inst *temp;
  
  /* passes in the address of the list head... pretty messy. */
  pMutexLock(prh_rfcomm_core_lock);  
  ret=prh_host_ll_search(&(prh_rfcomm_core->head), (void *)&temp, (void *)l2cap_inst, &prh_rfcomm_l2cap_inst_cmp);
  /* looking up the instance locks it */
  if (ret==BT_NOERROR)
    pMutexLock(l2cap_inst->lock);
  pMutexUnlock(prh_rfcomm_core_lock);
  return ret;
}

/*
 * prh_rfcomm_l2cap_cid_cmp
 * -- L2CAP Channel Handler Linked List helper traverse function
 * 
 * This is used to compare a cid with cids of the entries in the list
 */
int prh_rfcomm_l2cap_cid_cmp(void *cmpin, void *compwith)
{
  struct prh_rfcomm_l2cap_inst *instance=(struct prh_rfcomm_l2cap_inst *)compwith;
  unsigned int cid=(int)cmpin;
  
  return (instance->cid==cid);
}

/*
 * prh_rfcomm_lookup_l2cap_inst_by_cid
 * -- Finds an instance of L2CAP Channel Handler using a CID
 *
 * Searches linked list using prh_rfcomm_l2cap_cid_cmp
 */
int prh_rfcomm_lookup_l2cap_inst_by_cid(prh_t_rfcomm_cid cid, struct prh_rfcomm_l2cap_inst **l2cap_inst_ptr)
{
  int ret;
  pMutexLock(prh_rfcomm_core_lock);
  /* passes in the address of the list head... pretty messy. */
  ret=prh_host_ll_search(&(prh_rfcomm_core->head), (void **)(l2cap_inst_ptr), (void *)(u_int32)cid, &prh_rfcomm_l2cap_cid_cmp);
  /* looking up the instance locks it */
  pMutexUnlock(prh_rfcomm_core_lock);
  return ret;
}

/*
 * prh_rfcomm_l2cap_bdaddr_cmp
 * -- L2CAP Channel Handler Linked List helper traverse function
 *
 * This is used to compare a bd address with the entries in the list
 */
int prh_rfcomm_l2cap_bdaddr_cmp(void *cmpin, void *compwith)
{
  struct prh_rfcomm_l2cap_inst *instance=(struct prh_rfcomm_l2cap_inst *)compwith;
  t_bdaddr bd_addr=*((t_bdaddr *)cmpin);
  
  return (BD_ADDR_CMP(bd_addr, instance->bd_addr));
}

/*
 * prh_rfcomm_lookup_l2cap_inst_by_bd_addr
 * -- Finds an instance of L2CAP Channel Handler using a BD Address
 *
 * Searches linked list using prh_rfcomm_l2cap_bdaddr_cmp
 */
int prh_rfcomm_lookup_l2cap_inst_by_bd_addr(t_bdaddr bd_addr, struct prh_rfcomm_l2cap_inst **l2cap_inst_ptr)
{
  int ret;
  /* passes in the address of the list head... pretty messy. */
  pMutexLock(prh_rfcomm_core_lock);  
  ret=prh_host_ll_search(&(prh_rfcomm_core->head), (void *)l2cap_inst_ptr, (void *)&bd_addr, &prh_rfcomm_l2cap_bdaddr_cmp);
  /* looking up the instance locks it */
  pMutexUnlock(prh_rfcomm_core_lock);
  return ret;
}

/*
 * prh_rfcomm_remove_l2cap_inst
 * -- Removes an L2CAP Channel instance from the linked list
 */
int prh_rfcomm_remove_l2cap_inst(struct prh_rfcomm_l2cap_inst *l2cap_inst, int do_callback)
{
  struct prh_rfcomm_l2cap_inst *returned;
  int ret;
  
  pMutexLock(prh_rfcomm_core_lock);
  pMutexLock(l2cap_inst->lock);
  ret=prh_host_ll_remove(&(prh_rfcomm_core->head), (void **)&returned, (void *)(u_int32)l2cap_inst->cid,&prh_rfcomm_l2cap_cid_cmp);
  if (ret==BT_NOERROR)
    {
      prh_rfcomm_remove_all_dlc_instance(returned,do_callback);
      pMutexUnlock(returned->lock);
      pMutexFree(returned->lock);
      pFree(returned);
    }
  pMutexUnlock(prh_rfcomm_core_lock);
  return ret;
}

/*
 * prh_rfcomm_l2cap_remove_all_inst_helper
 * -- Traverse function used to clean up DLCs on an L2CAP channel
 * 
 */ 
int prh_rfcomm_l2cap_remove_all_inst_helper(void *data, void *func_info)
{
  struct prh_rfcomm_l2cap_inst *inst=(struct prh_rfcomm_l2cap_inst *)data;

  pMutexLock(inst->lock);
  prh_rfcomm_remove_all_dlc_instance(inst,0);
  pMutexUnlock(inst->lock);
  pMutexFree(inst->lock);
  pFree(inst);
  return BT_NOERROR;
}

/* 
 * prh_rfcomm_remove_all_l2cap_inst
 * -- Removes all L2CAP Channel Instances from the linked list
 *
 */
int prh_rfcomm_remove_all_l2cap_inst(void)
{
  int ret;
  pMutexLock(prh_rfcomm_core_lock);
  ret=prh_host_ll_remove_all(&(prh_rfcomm_core->head), &prh_rfcomm_l2cap_remove_all_inst_helper, NULL);
  pMutexUnlock(prh_rfcomm_core_lock);
  return ret;
}


/* add server is superseded by code in rfcomm_ue_api.c with locking */

/*
 * prh_rfcomm_schan_cmp
 * -- L2CAP Channel Handler Linked List helper traverse function
 * 
 * This is used to compare a cid with cids of the entries in the list
 */
int prh_rfcomm_schan_cmp(void *cmpin, void *compwith)
{
  struct prh_rfcomm_server_inst *instance=(struct prh_rfcomm_server_inst *)compwith;
  u_int8 server_chan=(u_int8)(((u_int32)cmpin) & 0xFF);
  
  return (instance->server_chan==server_chan);
}

/*
 * prh_rfcomm_lookup_server_inst_by_schan
 * -- Finds an instance of an RFCOMM Server by channel number
 *
 * Searches linked list using prh_rfcomm_schan_cmp
 */
int prh_rfcomm_lookup_server_inst_by_schan(prh_t_rfcomm_schan schan, struct prh_rfcomm_server_inst **server_inst_ptr)
{
  int ret;
  pMutexLock(prh_rfcomm_core_lock);
  /* passes in the address of the list head... pretty messy. */
  ret=prh_host_ll_search(&(prh_rfcomm_core->server_list), (void **)(server_inst_ptr), (void *)(u_int32)schan, &prh_rfcomm_schan_cmp);
  pMutexUnlock(prh_rfcomm_core_lock);
  return ret;
}


/*
 * prh_rfcomm_remove_server_inst_by_schan
 * -- Finds an instance of an RFCOMM Server by channel number
 *
 * Searches linked list using prh_rfcomm_schan_cmp
 */
int prh_rfcomm_remove_server_inst_by_schan(prh_t_rfcomm_schan schan, struct prh_rfcomm_server_inst **server_inst_ptr)
{
  int ret;
  pMutexLock(prh_rfcomm_core_lock);
  /* passes in the address of the list head... pretty messy. */
  ret=prh_host_ll_remove(&(prh_rfcomm_core->server_list), (void **)(server_inst_ptr), (void *)(u_int32)schan, &prh_rfcomm_schan_cmp);
  pMutexUnlock(prh_rfcomm_core_lock);
  return ret;
}

#if 0
/*
 * prh_rfcomm_remove_server_inst
 * -- Removes an L2CAP Channel instance from the linked list
 */
int prh_rfcomm_remove_server_inst(struct prh_rfcomm_server_inst *server_inst)
{
  struct prh_rfcomm_server_inst *returned;
  int ret;
  
  pMutexLock(prh_rfcomm_core_lock);
  ret=prh_host_ll_remove(&(prh_rfcomm_core->server_list), (void **)&returned, (void *)(u_int32)server_inst->server_chan,&prh_rfcomm_schan_cmp);
  
  if (ret==BT_NOERROR)
    {
      pMutexLock(returned->lock);
      pMutexUnlock(returned->lock);
      pMutexFree(returned->lock);
      pFree(returned);
    }
  pMutexUnlock(prh_rfcomm_core_lock);
  return ret;
}
#endif

int prh_rfcomm_remove_all_server_helper(void *data, void *func_info)
{
#if COMBINED_HOST ==0
  struct prh_rfcomm_server_inst *inst=(struct prh_rfcomm_server_inst *)data;
  
  pMutexLock(inst->lock);
  pMutexUnlock(inst->lock);
  pMutexFree(inst->lock);
  pFree(inst);
#endif
  return 0;
}

int prh_rfcomm_remove_all_servers(void)
{
  int ret;
  
  pMutexLock(prh_rfcomm_core_lock);
  ret=prh_host_ll_remove_all(&prh_rfcomm_core->server_list, &prh_rfcomm_remove_all_server_helper, NULL);
  pMutexUnlock(prh_rfcomm_core_lock);
  return ret;
}
