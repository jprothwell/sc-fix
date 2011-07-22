/***********************************************************************
 *
 * MODULE NAME:    rfcomm_entity_core.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Generic Port Entity code
 * MAINTAINER:     David Airlie
 * CREATION DATE:  1 May 2000
 *
 * SOURCE CONTROL: $Id: rfcomm_entity_core.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Inc.
 *     All rights reserved.
 *
 ***********************************************************************/
#include "rfcomm_lower_layer.h"
#include "rfcomm_ue_api.h"
#include "rfcomm_core.h"
#include "rfcomm_debug.h"
#include "rfcomm_proto.h"


/* handle for the RFCOMM system wide instance */

struct prh_host_gen_ll *prh_rfcomm_pending_list;

t_pMutex prh_rfcomm_pending_lock;

struct prh_rfcomm_entity_pending_conns {
  t_bdaddr bd_addr;
  void *multi_inst;
  struct prh_host_gen_ll *pending_dlc_head;

#if COMBINED_HOST==1
  u_int8 info_used; 	
#endif

};

struct prh_rfcomm_entity_pending_dlc {
  struct prh_rfcomm_entity_pending_conns *parent;
  prh_t_rfcomm_schan remote_schan;
  struct prh_rfcomm_dlc_cb cbs;

  prh_t_rfcomm_dlc_params dlc_params;
  prh_t_rfcomm_dlci dlci;

#if COMBINED_HOST==1
  u_int8 info_used; 
#endif

};

int prh_rfcomm_entity_rfstart_ind_cb(t_bdaddr bd_addr, prh_t_rfcomm_start_sys_params *system_parameters, prh_t_rfcomm_accept accept, void *multi_inst);
t_api prh_rfcomm_entity_rfstart_conf_cb(t_bdaddr bd_addr, prh_t_rfcomm_start_sys_params *system_parameters, prh_t_rfcomm_accept accept, void *multi_inst);
static int prh_rfcomm_entity_add_pending_device(struct prh_rfcomm_entity_pending_conns *pe_conn);
static int prh_rfcomm_entity_pending_device_process(t_bdaddr bd_addr, u_int32 multi_open);
static int prh_rfcomm_entity_add_pending_channel(t_bdaddr bd_addr, struct prh_rfcomm_entity_pending_dlc *pending_info);
static int prh_rfcomm_entity_remove_all_pending_channels(struct prh_rfcomm_entity_pending_conns *pe_conn);
static int prh_rfcomm_entity_remove_pending_device(t_bdaddr bd_addr);
/* 
 * prh_rfcomm_entity_init
 */
int prh_rfcomm_entity_started=0;
#if COMBINED_HOST==1
struct prh_rfcomm_entity_pending_dlc pending_info2[PRH_RFCOMM_MAX_Connect_Port];
struct prh_rfcomm_entity_pending_conns pending_conns2[PRH_RFCOMM_MAX_Connect_Port];
#endif

t_api prh_rfcomm_entity_init(u_int32 flags)
{
  int ret;
  //if (prh_rfcomm_entity_started==0)
    {
      ret=prh_rfcomm_core_init(flags);
      if (ret==BT_NORESOURCES)
	return ret;
      prh_rfcomm_pending_lock=pMutexCreate(0);
      PRH_RFCOMM_UE_register_rfstart_ind_cb(prh_rfcomm_entity_rfstart_ind_cb);
      prh_rfcomm_entity_started=1;
    }
  return 0;
}

t_api prh_rfcomm_entity_shutdown(u_int32 flags)
{
  pMutexLock(prh_rfcomm_pending_lock);
  prh_rfcomm_entity_remove_all_pending_conns();
  pMutexFree(prh_rfcomm_pending_lock);
  prh_rfcomm_entity_started=0;
 
  return 0;
}

/*
 * prh_rfcomm_entity_rfstart_ind_cb
 * -- RF Start Indication callback
 * 
 * Callback to handle a multiplexer connection from a remote device.
 * Responds to remove device accepting multiplexer connection
 */
int prh_rfcomm_entity_rfstart_ind_cb(t_bdaddr bd_addr, prh_t_rfcomm_start_sys_params *system_parameters, prh_t_rfcomm_accept accept, void *multi_inst)
{
  struct prh_rfcomm_multi_cb multi_cbs;

  multi_cbs.rfstart_conf=prh_rfcomm_entity_rfstart_conf_cb;

  PRH_RFCOMM_UE_Start_Resp(bd_addr, NULL, PRH_RFCOMM_ACCEPT, &multi_cbs, multi_inst);
  return 0;
}

/*
 * prh_rfcomm_entity_rfstart_conf_cb
 * -- Start confirmation callback
 * 
 * unlocks semaphore from multiplexer establish function
 */
t_api prh_rfcomm_entity_rfstart_conf_cb(t_bdaddr bd_addr, prh_t_rfcomm_start_sys_params *system_parameters, prh_t_rfcomm_accept accept, void *rfcomm_inst)
{
  pMutexLock(prh_rfcomm_pending_lock);
  prh_rfcomm_entity_pending_device_process(bd_addr, accept);
 
  pMutexUnlock(prh_rfcomm_pending_lock);
  return BT_NOERROR;
}

/*
 */
int prh_rfcomm_open_multiplexer(t_bdaddr remote_bd_addr, void **multi_inst)
{
  struct prh_rfcomm_multi_cb multi_cbs;
  int ret;
  prh_t_rfcomm_start_sys_params sys_params;

  multi_cbs.rfstart_conf=prh_rfcomm_entity_rfstart_conf_cb;

  sys_params.enable_credit_flow_control=1;
  sys_params.max_frame_size=PRH_RFCOMM_MAX_FRAME_SIZE;

  ret=PRH_RFCOMM_UE_Start_Req(remote_bd_addr, &sys_params, &multi_cbs, multi_inst);
  
  return ret;
;
}

/*
 * prh_rfcomm_entity_connect_port
 * -- Port Entity DLC Establish helper function
 *
 * Allocate a Port entity structure, sets it up, 
 * finds a multiplexer, (either gets current one or starts one)
 * Sets up callbacks, sends Parameter negotiation, and then does 
 * the DLC establish.
 *
 * Returns: BT_NOERROR - a multiplexer is already started
 *          BT_PENDING - a multiplexer start is in progress or required
 *          BT_NORESOURCES - no resources were available
 */
int prh_rfcomm_entity_connect_port(t_bdaddr remote_bd_addr, prh_t_rfcomm_schan rem_schan, prh_t_rfcomm_dlci *dlci, prh_t_rfcomm_dlc_params *dlc_params, struct prh_rfcomm_dlc_cb *cbs, void **multi_inst_ptr)
{
  int dbit;
  struct prh_rfcomm_l2cap_inst *l2cap_inst;
  struct prh_rfcomm_entity_pending_dlc *pending_info;
  struct prh_rfcomm_entity_pending_conns *pending_conns;
  t_api ret;
  
#if COMBINED_HOST==1  
  int i =0; 
#endif

  pMutexLock(prh_rfcomm_pending_lock);
  
   /* Check does a multiplexer to the remote BD Address exist already or is one pending */
  ret=PRH_RFCOMM_UE_Get_Multiplexer_State(remote_bd_addr, (void **)&l2cap_inst);
  if (ret==BT_NOERROR)
    {
      /* A Multiplexer exists */
      dbit=l2cap_inst->initiating;

      /* If it exists establish the DLC */
      *dlci=((!dbit) | (rem_schan<<1));
      *multi_inst_ptr=(void *)l2cap_inst;
      
      PRH_RFCOMM_UE_DLC_Estab_Req(remote_bd_addr, *dlci, dlc_params, cbs, *multi_inst_ptr);
      ret=BT_NOERROR;
      goto out;
    }

#if COMBINED_HOST==1
  //cui jian changed to static malloc pending information. 2009.02.11
	for( i =0; i< PRH_RFCOMM_MAX_Connect_Port ; i++)
	{
		if( pending_info2[i].info_used == 0 )
		{
			pending_info = &pending_info2[i ];
			pending_info2[i].info_used  = 1;
			break;
		}
	}
	if( i == PRH_RFCOMM_MAX_Connect_Port )
	  {
	      ret=BT_NORESOURCES;
	      goto out;
	  }
#else

  /* we'll be pending - malloc pending information */
  pending_info=(struct prh_rfcomm_entity_pending_dlc *)pMalloc(sizeof(struct prh_rfcomm_entity_pending_dlc));
  if (pending_info==NULL)
    {
      ret=BT_NORESOURCES;
      goto out;
    }
#endif
  
  pMemset(pending_info, 0, sizeof(struct prh_rfcomm_entity_pending_dlc));

  pending_info->remote_schan=rem_schan;
  pending_info->cbs=*cbs;
  pending_info->dlc_params=*dlc_params;

  dbit=1;
  pending_info->dlci=*dlci=((!dbit) | (rem_schan<<1));

  if (ret==BT_INVALIDPARAM)
    {
      /* No Multiplexer exists or is being started */
      /* Need to start a pending list */

#if COMBINED_HOST==1
  //cui jian changed to static malloc pending information. 2009.02.11

	for( i =0; i< PRH_RFCOMM_MAX_Connect_Port ; i++)
	{
		if( pending_conns2[i].info_used == 0 )
		{
			pending_conns = &pending_conns2[i ];
			pending_conns2[i].info_used  = 1;
			break;
		}
	}
	if( i == PRH_RFCOMM_MAX_Connect_Port )
	  {
	      ret=BT_NORESOURCES;
	      goto out;
	  }
#else
      pending_conns=(struct prh_rfcomm_entity_pending_conns *)pMalloc(sizeof(struct prh_rfcomm_entity_pending_conns));
      if (pending_conns==NULL)
	{
	  ret=BT_NORESOURCES;
	  goto out_free_info;
	}
#endif

      pMemset(pending_conns, 0, sizeof(struct prh_rfcomm_entity_pending_conns));
      pending_conns->bd_addr=remote_bd_addr;
      pending_conns->pending_dlc_head=NULL;
      pending_conns->multi_inst=NULL;

      /* Add to the list */
      ret=prh_rfcomm_entity_add_pending_device(pending_conns);
      if (ret==BT_NORESOURCES)
	{
	  goto out_free_conns;
	}

      ret=prh_rfcomm_entity_add_pending_channel(remote_bd_addr, pending_info);
      if (ret==BT_NORESOURCES)
	{
	  prh_rfcomm_entity_remove_pending_device(remote_bd_addr);
	  goto out_free_info;
	}

      ret=prh_rfcomm_open_multiplexer(remote_bd_addr, &pending_conns->multi_inst);
      if (ret!=BT_PENDING && ret!=BT_NOERROR)
	{
	  prh_rfcomm_entity_remove_all_pending_channels(pending_conns);
	  prh_rfcomm_entity_remove_pending_device(remote_bd_addr);
	}
      else
	*multi_inst_ptr=pending_conns->multi_inst;	    

    }
  else if (ret==BT_PENDING)
    {
      /* A multiplexer start is pending */

      *multi_inst_ptr=(void *)l2cap_inst;
      /* add to the list */
      ret=prh_rfcomm_entity_add_pending_channel(remote_bd_addr, pending_info);
      /* if we are pending return pending not no-error */
      if (ret!=BT_NOERROR)
	{
	  ret=BT_NORESOURCES;
	  goto out_free_info;
	}
      else
	ret=BT_PENDING;
    }
  
  goto out;

 out_free_conns:
  pFree(pending_conns);
 out_free_info:
  pFree(pending_info);
 out:
  pMutexUnlock(prh_rfcomm_pending_lock);
  return ret;
}

/* Add a new device to the pending list */
static int prh_rfcomm_entity_add_pending_device(struct prh_rfcomm_entity_pending_conns *pe_conn)
{
  return prh_host_ll_add(&prh_rfcomm_pending_list, pe_conn);
}

/* Compare function for bd address */
static int prh_rfcomm_entity_bdaddr_cmp(void *comp_input, void *comp_with)
{
  t_bdaddr *bd_addr_ptr=(t_bdaddr *)comp_input;
  struct prh_rfcomm_entity_pending_conns *pe_conn=(struct prh_rfcomm_entity_pending_conns *)comp_with;

  return BD_ADDR_CMP(pe_conn->bd_addr, (*bd_addr_ptr));
}

static int prh_rfcomm_entity_remove_pending_device(t_bdaddr bd_addr)
{
  int ret;
  struct prh_rfcomm_entity_pending_conns *pe_conn;
  ret=prh_host_ll_remove(&prh_rfcomm_pending_list, (void **)&pe_conn, (void *)&bd_addr, prh_rfcomm_entity_bdaddr_cmp);
  if (ret==BT_NOERROR)
    {
#if COMBINED_HOST==1
      pe_conn->info_used  = 0;
#else
      pFree(pe_conn);
#endif
    }
  return BT_NOERROR;
}

/* Add a pending pe_info to a BD Address list */
static int prh_rfcomm_entity_add_pending_channel(t_bdaddr bd_addr, struct prh_rfcomm_entity_pending_dlc *pending_info)
{
  int ret;
  struct prh_rfcomm_entity_pending_conns *pe_conn;

  ret=prh_host_ll_search(&prh_rfcomm_pending_list, (void *)&pe_conn, (void *)&bd_addr, &prh_rfcomm_entity_bdaddr_cmp);
  if (ret==BT_NOERROR)
    {
      pending_info->parent=pe_conn;
      ret=prh_host_ll_add(&pe_conn->pending_dlc_head, (void *)pending_info);
    }
  return ret;
}

static int prh_rfcomm_entity_remove_pending_channel_helper(void *data, void *func_info)
{
  u_int32 multi_open=(u_int32)func_info;
  struct prh_rfcomm_entity_pending_dlc *pending_info=(struct prh_rfcomm_entity_pending_dlc *)data;

  /* Multiplexer established? */
  if (multi_open==PRH_RFCOMM_ACCEPT)
    {
      PRH_RFCOMM_UE_DLC_Estab_Req(pending_info->parent->bd_addr, pending_info->dlci, &pending_info->dlc_params, &pending_info->cbs, pending_info->parent->multi_inst);
    }
  else
    {
      /* Call back DLCI ind with failure */
      if (pending_info->cbs.dlc_estab_ind)
	pending_info->cbs.dlc_estab_conf(pending_info->parent->bd_addr, pending_info->dlci, &pending_info->dlc_params, PRH_RFCOMM_REFUSE, pending_info->parent->multi_inst);
    }
#if COMBINED_HOST==1
  pending_info->info_used  = 0;
#else
  pFree(pending_info);
#endif

  return BT_NOERROR;
}

static int prh_rfcomm_entity_pending_device_process(t_bdaddr bd_addr, u_int32 multi_open)
{
  int ret;
  struct prh_rfcomm_entity_pending_conns *pe_conn;
  ret=prh_host_ll_search(&prh_rfcomm_pending_list, (void **)&pe_conn, (void *)&bd_addr, &prh_rfcomm_entity_bdaddr_cmp);
  if (ret==BT_NOERROR)
    {
      ret=prh_host_ll_remove_all(&pe_conn->pending_dlc_head, &prh_rfcomm_entity_remove_pending_channel_helper, (void *)multi_open);
      prh_rfcomm_entity_remove_pending_device(bd_addr);
    }
  return ret;
}

/* remove all pending dlcs */
/* clean up for reset remove everything */
int prh_rfcomm_entity_remove_all_pending_channels_helper(void *data, void *func_info)
{
  struct prh_rfcomm_entity_pending_dlc *pending_info=(struct prh_rfcomm_entity_pending_dlc *)data;  
  pFree(pending_info);
  return BT_NOERROR;
}


int prh_rfcomm_entity_remove_all_pending_channels(struct prh_rfcomm_entity_pending_conns *pe_conn)
{
  int ret;
  ret=prh_host_ll_remove_all(&(pe_conn->pending_dlc_head), &prh_rfcomm_entity_remove_all_pending_channels_helper, NULL);
  return ret;
}
/* clean up for reset remove everything */
int prh_rfcomm_entity_remove_all_pending_conns_helper(void *data, void *func_info)
{
  struct prh_rfcomm_entity_pending_conns *pe_conn=(struct prh_rfcomm_entity_pending_conns *)data;
  
  /* if any pending dlcs free em all */
  if (pe_conn->pending_dlc_head)
    {
      prh_rfcomm_entity_remove_all_pending_channels(pe_conn);
    }
  
  pFree(pe_conn);
  return BT_NOERROR;
}

int prh_rfcomm_entity_remove_all_pending_conns(void)
{
  int ret;
  ret=prh_host_ll_remove_all(&prh_rfcomm_pending_list, &prh_rfcomm_entity_remove_all_pending_conns_helper, NULL);
  return ret;
}
