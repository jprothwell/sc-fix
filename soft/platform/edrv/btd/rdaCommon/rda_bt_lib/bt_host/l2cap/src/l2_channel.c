/************************************************************************
 * MODULE NAME:    l2_channel.c
 * PROJECT CODE:
 * DESCRIPTION:    l2cap routines for channel management
 * MAINTAINER:     Gary Fleming
 * DATE:           20-08-2000
 *
 * SOURCE CONTROL: 
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 *
 * Description :
 * This module is responsible for channel manipulation function in L2CAP.
 * 
 **************************************************************************/

#include "host_config.h"
#include "host_types.h"
#include "host_error.h"
#include "host_buf.h"

#include "l2_const.h"
#include "l2_channel.h"
#include "l2_acl_link.h"
#include "l2_psm.h"
#include "l2_layer_config.h"
#include "l2_config_helper.h"
#include "l2cap_lower_layer.h"

/***************************************************
 *  Array of pointers to channel structures
 ***************************************************/

#if COMBINED_HOST==0
prh_t_l2_channel*  prh_l2_channel_list[PRH_L2_MAX_NUM_CHANNEL_IDENTIFIERS];
//extern void (*FindObjectPushServer_callback)(struct st_t_BT_ConnectionInfo *dbEntry, t_api status);
extern void (*FindFTPServer_callback)(struct st_t_BT_ConnectionInfo *dbEntry, t_api status);
#else
prh_t_l2_channel  prh_l2_channel_list[PRH_L2_MAX_NUM_CHANNEL_IDENTIFIERS];
#endif
/***************************************************
 *
 * Function :- prh_l2_chn_tid2channel
 *
 * Parameters 
 *      identifier :- transaction identifier (from an L2CAP PDU)
 *
 * Returns
 *      A pointer the channel associated with the transaction identifier
 *
 * Description 
 * Uses the transaction identifier (from an L2CAP PDU) to find a
 * pointer to a channel.
 *
 ****************************************************/

prh_t_l2_channel* prh_l2_chn_tid2channel(u_int8 identifier)
{
    int i;

    for (i=0; i < PRH_L2_MAX_NUM_CHANNEL_IDENTIFIERS; i++)
    {
#if COMBINED_HOST==0
        if ((prh_l2_channel_list[i] != 0) &&
            (prh_l2_channel_list[i]->local_transaction_identifier == identifier))
            return prh_l2_channel_list[i];
#else
        if ((prh_l2_channel_list[i].flag != 0) &&
            (prh_l2_channel_list[i].local_transaction_identifier == identifier))
            return &(prh_l2_channel_list[i]);
#endif
    }
    return 0;
}

/*****************************************************************
 *
 * Function :- prh_l2_chn_handle_in_use
 *
 * Parameters 
 *      handle  :- the ACL handle to be searched for active channels
 *
 * Returns 
 *      An int indicating if channels are active on the ACL handle.
 *             0x00   - No channels are active on the ACL handle.
 *             0x01   - Channels are active on the ACL handle.
 * Description 
 * Determins if a handle has channels active on it.
 *****************************************************************/

int prh_l2_chn_handle_in_use(u_int16 handle)
{
  int i;

  for (i=0;i < PRH_L2_MAX_NUM_CHANNEL_IDENTIFIERS; i++)
  {
#if COMBINED_HOST==0
      if (prh_l2_channel_list[i] != 0)
        if (prh_l2_channel_list[i]->handle == handle)
            return 1;
#else
      if (prh_l2_channel_list[i].flag != 0)
        if (prh_l2_channel_list[i].handle == handle)
            return 1;
#endif
  }
  return 0;
}

/*******************************************************************
 *
 * Function :- prh_l2_chn_get_p_channel
 *
 * Parameters 
 *      cid :- Channel identifier
 *
 * Returns
 *      A pointer to the channel identified by cid.
 *
 * Description 
 * Given a CID returns a pointer to the channel.
 *
 *******************************************************************/

prh_t_l2_channel* prh_l2_chn_get_p_channel(u_int16 cid)
{
#if COMBINED_HOST==0
    return ((cid < (PRH_L2_MAX_NUM_CHANNEL_IDENTIFIERS + PRH_L2_MIN_USER_CID))
         && (cid >= PRH_L2_MIN_USER_CID)) ? prh_l2_channel_list[cid-PRH_L2_MIN_USER_CID] : 0;
#else
    return ((cid < (PRH_L2_MAX_NUM_CHANNEL_IDENTIFIERS + PRH_L2_MIN_USER_CID))
         && (cid >= PRH_L2_MIN_USER_CID)) ? &(prh_l2_channel_list[cid-PRH_L2_MIN_USER_CID]) : 0;
#endif
}

/*******************************************************************
 *
 * Function :- prh_l2_chn_free_channels_on_handle
 *
 * Parameters 
 *      handle  :- the ACL handle to be searched for active channels
 *
 * Returns 
 *      void
 *
 * Description :-
 * Frees any L2CAP channels which have been established on a given
 * connection handle. For each channel which is freed a Disconnect_ind 
 * event is raised to the corresponding higher layer PSM.
 ********************************************************************/

void  prh_l2_chn_free_channels_on_handle(u_int16 handle)
{
    int i;
    prh_t_l2_channel *p_channel;
    u_int8 psm_index;
    t_L2_ControlCallbackArgs  eventinfo;
    prh_t_l2_channel_states state;
    if (handle == 0x00)
        return;
    
    for (i=0;i < PRH_L2_MAX_NUM_CHANNEL_IDENTIFIERS; i++)
    {
#if COMBINED_HOST==0
        p_channel = prh_l2_channel_list[i];
#else
        p_channel = &(prh_l2_channel_list[i]);
#endif
        if (p_channel != 0)
        {
            if (p_channel->handle == handle)
            {
				pMemset(&eventinfo,0,sizeof(eventinfo));
                psm_index = prh_l2_psm_get_psm_ident(p_channel->psm);
                eventinfo.cid=p_channel->local_cid;
                eventinfo.psm=p_channel->psm;
				state=p_channel->state;
                prh_l2_chn_free_channel(p_channel,PRH_L2_DONT_RELEASE_ACL);
				if ((state==PRH_L2_W4_L2CAP_CONNECT_RSP) || (state == PRH_L2_W4_LP_CONNECT_CFM))
				{
					eventinfo.result=L2CAP_CONNECTION_TIMEOUT;
					eventinfo.status=0x00;
					eventinfo.BDAddress = *prh_l2_acl_get_bd_addr(handle);
					prh_l2_psm_event(psm_index, L2CAP_evCONNECTCONF, &eventinfo);
				   
				}
				else if (state!=PRH_L2_CLOSED)
					prh_l2_psm_event(psm_index,L2CAP_evDISCONNECTIND,&eventinfo);
            }
        }
    }
}


/*******************************************************************
 *
 * Function :- prh_l2_chn_data_error_for_channels_on_handle
 *
 * Parameters 
 *      handle  :- the ACL handle to be searched for active channels
 *      flush   :- Indicates if the data error event was due to flush
 *                 or buffer overflow.
 *
 * Returns 
 *      void
 *
 * Description :-
 * Invokes a callback for any L2CAP channels which are on the ACL handle 
 * associated with the data error event.
 ********************************************************************/

void  prh_l2_chn_data_error_for_channels_on_handle(u_int16 handle,u_int8 data_error)
{
    int i;
    prh_t_l2_channel *p_channel;
    u_int8 psm_index;
    t_L2_ControlCallbackArgs  eventinfo;
    if (handle == 0x00)
        return;
    
    for (i=0;i < PRH_L2_MAX_NUM_CHANNEL_IDENTIFIERS; i++)
    {
#if COMBINED_HOST==0
        p_channel = prh_l2_channel_list[i];
#else
        p_channel = &(prh_l2_channel_list[i]);
#endif
        if (p_channel != 0)
        {
            if (p_channel->handle == handle)
            {
				pMemset(&eventinfo,0,sizeof(eventinfo));
                psm_index = prh_l2_psm_get_psm_ident(p_channel->psm);
                eventinfo.cid=p_channel->local_cid;
                eventinfo.psm=p_channel->psm;
				if (p_channel->state != PRH_L2_CLOSED);
				{
					switch(data_error)
					{
					case L2CAP_evFLUSHOCCURRED :
                    case L2CAP_evBUFFEROVERFLOW :
					case L2CAP_evQOSVIOLATIONIND :
						prh_l2_psm_event(psm_index, data_error, &eventinfo);
						break;

					default :
						break;
					}

				}
            }
        }
    }
}



/*******************************************************************
 *
 * Function :- prh_l2_chn_free_channels_on_psm
 *
 * Parameters 
 *      handle  :- the ACL handle to be searched for active channels
 *
 * Returns 
 *      void
 *
 * Description :-
 * Frees any L2CAP connection orientedchannels which have been established 
 * on a given psm. For each channel which is freed a Disconnect_ind 
 * event is raised to the corresponding higher layer PSM.
 ********************************************************************/

void  prh_l2_chn_free_channels_on_psm(t_psm psm)
{
  int i;
  prh_t_l2_channel* p_channel;
  u_int8 psm_index;
  t_L2_ControlCallbackArgs  eventinfo;

  for (i=0;i < PRH_L2_MAX_NUM_CHANNEL_IDENTIFIERS; i++)
  {
#if COMBINED_HOST==0
        p_channel = prh_l2_channel_list[i];
#else
        p_channel = &(prh_l2_channel_list[i]);
#endif
      if (p_channel != 0)
      {
          if (p_channel->psm == psm && p_channel->state!=PRH_L2_CLOSED)
          {
               psm_index = prh_l2_psm_get_psm_ident(p_channel->psm);
               eventinfo.cid=p_channel->local_cid;
               eventinfo.psm=p_channel->psm;
               prh_l2_chn_free_channel(p_channel,PRH_L2_DONT_RELEASE_ACL);
               prh_l2_psm_event(psm_index,L2CAP_evDISCONNECTIND,&eventinfo);
           }
      }
   }
}


/**************************************************
 *
 * Function : prh_l2_chn_timer_expired
 *
 * Parameter
 *       p_channel - A pointer to the channel for which the timer expired.
 * 
 * Returns 
 *       void
 *
 * ACTIONS
 * 1/ Send an L2CA TimeOutInd to upper layer
 * 2/ Return the CID to the free pool
 * 3/ Modify State to closed.
 **************************************************/

void prh_l2_chn_timer_expired(void *arg)
{
    prh_t_l2_channel* p_channel=(prh_t_l2_channel*)arg;
    prh_t_l2_channel_states chn_state;
    u_int8 psm_index;
    u_int8 timer_type;
    t_L2_ControlCallbackArgs  eventinfo;
    pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"prh_l2_chn_timer_expired arg =%x\n", arg));
	pMemset(&eventinfo,0,sizeof(t_L2_ControlCallbackArgs));
    eventinfo.cid = p_channel->local_cid;
    eventinfo.timerType = p_channel->timer_active;
    
    /* Save the timer type */
    timer_type = p_channel->timer_active;
    p_channel->timer_active = 0x0;
    
    psm_index = prh_l2_psm_get_psm_ident(p_channel->psm);
    if (psm_index != 0)
    {
        chn_state = p_channel->state;
        prh_l2_psm_event(psm_index,L2CAP_evTIMEOUTIND,&eventinfo);

		/* If waiting for a connect response and this is the RTX timer expiry */
        if ((chn_state == PRH_L2_W4_L2CAP_CONNECT_RSP) && (timer_type == 1))
        {
            eventinfo.psm = p_channel->psm;
            eventinfo.cid = 0x00;
            eventinfo.status = 0x00;
            eventinfo.result = 0XEEEE;
			prh_l2_chn_free_channel(p_channel, PRH_L2_RELEASE_ACL);
            prh_l2_psm_event(psm_index,L2CAP_evCONNECTCONF,&eventinfo);
        } 
		else if (chn_state == PRH_L2_W4_L2CAP_DISCON_RSP)
        {
            eventinfo.result = 0XEEEE;
           
			prh_l2_chn_free_channel(p_channel, PRH_L2_RELEASE_ACL);
            prh_l2_psm_event(psm_index,L2CAP_evDISCONNECTCONF,&eventinfo);
        }
        else /* If a channel is beyond the connnected state and not awaiting a disconnect resp */
        {
            eventinfo.result = 0XEEEE;
			L2_Disconnect(p_channel->local_cid);
			prh_l2_chn_free_channel(p_channel, PRH_L2_RELEASE_ACL);
            prh_l2_psm_event(psm_index,L2CAP_evDISCONNECTIND,&eventinfo);
        }
    }
}

/*******************************************************************
 *
 * Function :- prh_l2_cfg_timer_expired
 *
 * Parameters 
 *      void*  pointer to the channel
 *
 *
 * Description 
 * Handles a configuration timer expiry
 *
 *******************************************************************/

void prh_l2_chn_cfg_timer_expired(void *arg)
{
    prh_t_l2_channel* p_channel=(prh_t_l2_channel*)arg;

    if (p_channel != 0)
    {
        L2_Disconnect(p_channel->local_cid);
    }
}


/********************************************************************
 * Function :- prh_l2_chn_alloc_channel
 * 
 * Parameters
 *       NONE
 * 
 * Returns
 *       A pointer to an l2cap channel structure. A null pointer is returned
 *       if a channel could not be allocated.
 *
 * Description
 * Allocates an L2CAP Channel. Get the next free entry in the L2CAP channel
 * list and allocate a channel to it.
 ********************************************************************/

prh_t_l2_channel* prh_l2_chn_alloc_channel()
{
    t_channelId cid = 0;
    prh_t_l2_channel* p_channel = 0;

#if COMBINED_HOST==0
    while((prh_l2_channel_list[cid]) && (cid < PRH_L2_MAX_NUM_CHANNEL_IDENTIFIERS))
        cid++;
#else
    while((prh_l2_channel_list[cid].flag) && (cid < PRH_L2_MAX_NUM_CHANNEL_IDENTIFIERS))
        cid++;
#endif

    if(PRH_L2_MAX_NUM_CHANNEL_IDENTIFIERS != cid)
    {
#if COMBINED_HOST==0
        p_channel = prh_l2_channel_list[cid] = pMalloc(sizeof(prh_t_l2_channel));
#else
        p_channel = &(prh_l2_channel_list[cid]);// = pMalloc(sizeof(prh_t_l2_channel));
#endif
        if(p_channel)
        {
            pMemset(p_channel,0,sizeof(prh_t_l2_channel));
            p_channel->type = PRH_L2_CO_CHANNEL;
            p_channel->local_cid = cid+PRH_L2_MIN_USER_CID;
#if COMBINED_HOST==1
	     p_channel->flag = 1;
#endif
        }
    }
    return p_channel;
}

/********************************************************************
 *
 * Function :- prh_l2_chn_free_channel
 *
 * Parameters 
 *      p_channel - A pointer to the channel to be freed.
 *
 *      release_acl - A flag indicating if the ACL link should be 
 *              released if this is the last channel on the ACL link.
 *
 *                  0x00 PRH_L2_DONT_RELEASE_ACL
 *                  0x01 PRH_L2_RELEASE_ACL
 *
 * Description
 * Frees a previously allocated L2CAP Channel.
 * If there are no other channels open (and release_acl = RELEASE_ACL) then can
 * call the Bluetooth manager to release the ACL.
 ********************************************************************/

void prh_l2_chn_free_channel(prh_t_l2_channel* p_channel, u_int8 flags)
{
    prh_t_l2_acl_link* p_link;
    u_int16 handle = p_channel->handle;
    
#if (PRH_L2CAP_AUTO_CONFIG==1)
    if (!(flags & PRH_L2_DONT_RELEASE_CONFIG))
      prh_l2_config_helper_remove_cid(p_channel->local_cid);
#endif
    //prh_l2_channel_list[p_channel->local_cid-PRH_L2_MIN_USER_CID] = 0;
#if COMBINED_HOST==1
    p_channel->flag = 0;
#else    
    if (p_channel->outQOS != 0)
        pFree(p_channel->outQOS);

	if (p_channel->inQOS != 0)
		pFree(p_channel->inQOS);
#endif
    if (p_channel->timer_active)
    	{
        	pTimerCancel(p_channel->timer_handle);
			p_channel->timer_active = 0;
			p_channel->timer_handle=0;
    	}

	if (p_channel->config_timer_handle)
	{ 
		pTimerCancel(p_channel->config_timer_handle);
		p_channel->config_timer_handle = 0;
	}
#if (L2CAP_GROUPS_SUPPORTED==1)
    if (p_channel->bda_list)
        prh_l2_bda_list_free(p_channel->bda_list);
#endif
#if COMBINED_HOST==0
	prh_l2_channel_list[p_channel->local_cid-PRH_L2_MIN_USER_CID] = 0;
    if(p_channel!=NULL)
    		pFree(p_channel);
     p_channel=NULL;
#else //add by xzc
	prh_l2_channel_list[p_channel->local_cid-PRH_L2_MIN_USER_CID].flag= 0;
	p_channel = 0;
#endif
    
    if (handle != 0x00)
    {
        if((!prh_l2_chn_handle_in_use(handle)) && (flags & PRH_L2_RELEASE_ACL))
        {
            
#if COMBINED_HOST==1
	     LMconnection_LM_Disconnect_Req(handle,PRH_USER_ENDED_CONNECTION);
#else
/*
	if(FindObjectPushServer_callback)
	{
            prh_mgr_releaseACL(handle,PRH_USER_ENDED_CONNECTION, FindObjectPushServer_callback);
	     FindObjectPushServer_callback =NULL;
#if pDEBUG
	pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"prh_l2_chn_free_channel opp  _releaseACL\n"));
#endif
	}
*/	
	if(FindFTPServer_callback)
	{
            prh_mgr_releaseACL(handle,PRH_USER_ENDED_CONNECTION, FindFTPServer_callback);
	     FindFTPServer_callback =NULL;

		pDebugPrintfEX((pLOGDEBUG,pLOGL2CAP,"prh_l2_chn_free_channel ftp  _releaseACL\n"));

	}
	else
            prh_mgr_releaseACL(handle,PRH_USER_ENDED_CONNECTION, NULL);
#endif
            p_link = prh_l2_acl_find_handle(handle);
            prh_l2_acl_free_link(p_link);
        }
    }


	

}

/********************************************************************
 *
 * Function :- prh_l2_chn_set_qos
 *
 * Parameters
 *      p_channel - A pointer to the channel to which the QoS is to be assigned.
 *      p_outflow - A pointer to a structure describing the QoS.
 *
 * Returns
 *      u_int8 - Indicating the success/failure of the operation
 *                  0x00  - Failure
 *                  0x01  - Success
 *
 * Description
 * Sets the Quality of service parameters for a given channel.
 *
 ********************************************************************/

u_int8 prh_l2_chn_set_qos(prh_t_l2_channel* p_channel,t_L2_ChannelFlowSpec *p_outflow)
{
    if ((p_outflow == 0) || (p_channel == 0))
        return 0;

	/* ensure that a new QoS structure is not Malloced if one already exists */
#if COMBINED_HOST==0
	if (p_channel->outQOS == 0)
		p_channel->outQOS = pMalloc(sizeof(t_L2_ChannelFlowSpec));

    if (p_channel->outQOS)
    {
        p_channel->outQOS->flags = p_outflow->flags;
        p_channel->outQOS->bucketSize = p_outflow->bucketSize;
        p_channel->outQOS->delayVariation = p_outflow->delayVariation;
        p_channel->outQOS->latency = p_outflow->latency;
        p_channel->outQOS->peakBandwidth = p_outflow->peakBandwidth;
        p_channel->outQOS->serviceType = p_outflow->serviceType;
        p_channel->outQOS->tokenRate = p_outflow->tokenRate;
    }
    else
    {
        return 0;
    }
#else
    {
        p_channel->outQOS.flags = p_outflow->flags;
        p_channel->outQOS.bucketSize = p_outflow->bucketSize;
        p_channel->outQOS.delayVariation = p_outflow->delayVariation;
        p_channel->outQOS.latency = p_outflow->latency;
        p_channel->outQOS.peakBandwidth = p_outflow->peakBandwidth;
        p_channel->outQOS.serviceType = p_outflow->serviceType;
        p_channel->outQOS.tokenRate = p_outflow->tokenRate;
    }
#endif
    return 1;
}


/*******************************************************************
 *
 * Function :- prh_l2_chn_start_RTX_timer
 *
 * Parameters 
 *      p_channel :- A pointer to a channel structure
 *
 * Returns
 *      None.
 *
 * Description 
 * Enables the RTX timer on a given L2CAP channel.
	 *
 *******************************************************************/

void prh_l2_chn_start_RTX_timer(prh_t_l2_channel* p_channel)
{
    p_channel->timer_active = 01;
    if (p_channel->timer_handle)
		pTimerCancel(p_channel->timer_handle);
    p_channel->timer_handle=pTimerCreate((unsigned int)PRH_L2_RTX_TIMEOUT_VALUE,prh_l2_chn_timer_expired,(void*)p_channel,pTIMER_ONESHOT);
}


/*******************************************************************
 *
 * Function :- prh_l2_chn_start_cfg_timer
 *
 * Parameters 
 *      p_channel :- A pointer to a channel structure
 *
 * Returns
 *      None.
 *
 * Description 
 * Starts the 120 sec config timer used during L2CAP channel configuration.
 *
 *******************************************************************/

void prh_l2_chn_start_cfg_timer(prh_t_l2_channel* p_channel)
{
	if (p_channel->config_timer_handle==0)
		p_channel->config_timer_handle=pTimerCreate((unsigned int)PRH_L2_CFG_TIMEOUT_VALUE,
			prh_l2_chn_cfg_timer_expired,(void*)p_channel,pTIMER_ONESHOT);
}

/*******************************************************************
 *
 * Function :- prh_l2_chn_start_ERTX_timer
 *
 * Parameters 
 *      p_channel :- A pointer to a channel structure
 *
 * Returns
 *      None.
 *
 * Description 
 * Enables the ERTX timer on a given L2CAP channel.
 *
 *******************************************************************/

void prh_l2_chn_start_ERTX_timer(prh_t_l2_channel* p_channel)
{ 
    p_channel->timer_active = 02;   
    if (p_channel->timer_handle)
		pTimerCancel(p_channel->timer_handle);	
    p_channel->timer_handle=pTimerCreate((unsigned int)PRH_L2_ERTX_TIMEOUT_VALUE,
			prh_l2_chn_timer_expired,(void*)p_channel,pTIMER_ONESHOT);
}

/*******************************************************************
 *
 * Function :- prh_l2_chn_disable_RTX_timer
 *
 * Parameters 
 *      p_channel :- A pointer to a channel structure
 *
 * Returns
 *      None.
 *
 * Description 
 * Disables the RTX timer on a given L2CAP channel.
 *
 *******************************************************************/
//#define tmp_SXS_GET_RA(store) asm volatile("sw $31, 0(%0)"::"r"((store)))

void prh_l2_chn_disable_RTX_timer(prh_t_l2_channel* p_channel)
{
//    u_int32 CallerAdd;
//    tmp_SXS_GET_RA(&CallerAdd);
//    pDebugPrintfEX((pLOGERROR,pLOGL2CAP,"prh_l2_chn_disable_RTX_timer p_channel =%x CallerAdd=%x\n",p_channel,CallerAdd));

if(p_channel->timer_active)
	{
   	 pTimerCancel(p_channel->timer_handle);
    	p_channel->timer_active = 0;
		p_channel->timer_handle=0;
	}
}

/*******************************************************************
 *
 * Function :- prh_l2_chn_disable_ERTX_timer
 *
 * Parameters 
 *      p_channel :- A pointer to a channel structure
 *
 * Returns
 *      None.
 *
 * Description 
 * Disables the ERTX timer on a given L2CAP channel.
 *
 *******************************************************************/

void prh_l2_chn_disable_ERTX_timer(prh_t_l2_channel* p_channel)
{
if(p_channel->timer_active)
    {
        pTimerCancel(p_channel->timer_handle);
        p_channel->timer_active = 0;
        p_channel->timer_handle=0;
    }
}



/*******************************************************************
 *
 * Function :- prh_l2_chn_stop_cfg_timer
 *
 * Parameters 
 *      p_channel :- A pointer to a channel structure
 *
 * Returns
 *      None.
 *
 * Description 
 * Disables the config timer on a given L2CAP channel.
 *
 *******************************************************************/

void prh_l2_chn_stop_cfg_timer(prh_t_l2_channel* p_channel)
{ 
    if(p_channel->config_timer_handle)
    {
        pTimerCancel(p_channel->config_timer_handle);
        p_channel->config_timer_handle = 0;
    }
}


