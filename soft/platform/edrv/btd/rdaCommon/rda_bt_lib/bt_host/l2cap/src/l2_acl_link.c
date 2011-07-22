/**************************************************************************
 * MODULE NAME:    l2_acl_link.c
 * PROJECT CODE:
 * DESCRIPTION:    l2cap routines for ACL management
 * MAINTAINER:     Gary Fleming
 * DATE:           11-08-1999
 *
 * SOURCE CONTROL: 
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 *
 * Description :
 * This module is responsible for ACL link manipulation function in L2CAP.
 * 
 **************************************************************************/

#include "host_types.h"
#include "l2_types.h"
#include "l2_const.h"
#include "l2_acl_link.h"
#include "l2_utils.h"
#include "l2_enc_dec.h"
#include "l2_channel.h"
#include "l2cap_lower_layer.h"
#if COMBINED_HOST==1
#include "l2cap_primitives.h"
#include "host_timer.h"
#endif

/* An Array of pointers to ACL link structures */

#if COMBINED_HOST==1
prh_t_l2_acl_link prh_l2_acl_links[PRH_L2_MAX_NUM_LINKS];
#else
prh_t_l2_acl_link* prh_l2_acl_links[PRH_L2_MAX_NUM_LINKS];
#endif

void prh_l2_acl_timer_expired(void *arg);

/********************************************************************
 * Function :- prh_l2_acl_exists
 * 
 * Input Parameters :-     
 *     bd_addr :- The address of a Bluetooth device.
 *
 * Returns
 *     A pointer to the structure describing the link or NULL if
 *     unsuccessfull.
 *
 * Description 
 * Checks if an ACL link exists to a given Bluetooth device.
 *
 ********************************************************************/

prh_t_l2_acl_link* prh_l2_acl_exists(t_bdaddr* p_bd_addr)
{
    prh_t_l2_acl_link* p_link;
    int i;

    for(i=0;i<PRH_L2_MAX_NUM_LINKS;i++)
    {
#if COMBINED_HOST==1
       p_link = &(prh_l2_acl_links[i]);

       if((p_link->flag != 0) && (prh_l2_utils_addr_match(&p_link->bd_addr,p_bd_addr)))
           return p_link;
#else
       p_link = prh_l2_acl_links[i];

       if((p_link != 0) && (prh_l2_utils_addr_match(&p_link->bd_addr,p_bd_addr)))
           return p_link;
#endif
    }
    return 0;
}

/********************************************************************
 * Function :- prh_l2_acl_find_handle
 *
 * Parameters :- 
 *     handle :- The handle for the ACL link
 * 
 * Returns
 *     A pointer to the structure describing the link or NULL if
 *     unsuccessfull.
 *
 * Description 
 * This function finds a structure describing an ACL link, using the 
 * the connection handle as the key. A pointer to a structure describing
 * the ACL link is returned.
 *
 ********************************************************************/

prh_t_l2_acl_link* prh_l2_acl_find_handle(u_int16 handle)
{
    prh_t_l2_acl_link* p_link;
    int i;

    for(i=0;i<PRH_L2_MAX_NUM_LINKS;i++)
    {
#if COMBINED_HOST==1
       p_link = &(prh_l2_acl_links[i]);
       if((p_link->flag != 0) && (p_link->handle == handle))
           return p_link;
#else
       p_link = prh_l2_acl_links[i];
       if((p_link != 0) && (p_link->handle == handle))
           return p_link;
#endif
    }
    return 0;
}

/**************************************************************
 * Function : prh_l2_acl_Alloc_Link 
 *      
 * Input Parameters :
 *      p_bd_addr   :- A pointer to the Address of the peer on the 
 *                     link.
 *      pending_cid :- The local CID currently pending during ACL 
 *                     link establishment.
 *
 * Returns
 *     A pointer to the structure describing the link or NULL if
 *     unsuccessfull.
 *
 * Description 
 * Allocates a link structure for a link to a specific Bd_ADDR
 * Returns a pointer to the structue which has been allocated.
 **************************************************************/

prh_t_l2_acl_link* prh_l2_acl_alloc_link(t_bdaddr* p_bd_addr, u_int16 pending_cid)
{
    prh_t_l2_acl_link* p_link;
    int i;

    /* First Check does a link already exist to this Address */
    
    for(i=0;i<PRH_L2_MAX_NUM_LINKS;i++)
    {
#if COMBINED_HOST==1
       p_link = &(prh_l2_acl_links[i]);
       if(p_link->flag == 0)
       {
           pMemset(p_link,0,sizeof(prh_t_l2_acl_link)); 
		   p_link->flag = 1;
#else
       p_link = prh_l2_acl_links[i];
       if(p_link == 0)
       {
           p_link = pMalloc(sizeof(prh_t_l2_acl_link));
		   if (!p_link)
			   return 0;
           pMemset(p_link,0,sizeof(prh_t_l2_acl_link)); 
           prh_l2_acl_links[i] = p_link;
		   p_link->cur_pos=0;
#endif
           prh_l2_utils_set_bd_addr(&p_link->bd_addr, p_bd_addr);
           p_link->pending_cid = pending_cid;
		   p_link->mtu_complete = 1;
           return p_link;
       }
    }
    return 0;
}

/**************************************************************
 * Function : prh_l2_acl_free_Link 
 *      
 * Parameters :
 *      p_link :- A pointer to the link structure to be freed.
 *
 * Returns
 *     --
 *
 * Description 
 * Frees a link structure for a link to a specific Bd_ADDR.
 * Also cancels any timers associated with the link.
 **************************************************************/

void  prh_l2_acl_free_link(prh_t_l2_acl_link* p_link)
{
#if COMBINED_HOST==0
    u_int16 i;
#endif
#if (L2CAP_GROUPS_SUPPORTED==1)
	prh_t_l2_channel* p_channel;
    #endif
    if (p_link != 0)
    {
#if (L2CAP_GROUPS_SUPPORTED==1)
        for (i = PRH_L2_MIN_USER_CID; i < (PRH_L2_MAX_NUM_CHANNEL_IDENTIFIERS + PRH_L2_MIN_USER_CID);i++)
		{
			
			p_channel = prh_l2_chn_get_p_channel(i);
			if ((p_channel) && (p_channel->bda_list))
			{
				prh_l2_bda_list_remove(p_channel->bda_list,&p_link->bd_addr);
			}
		}
#endif
#if COMBINED_HOST==1
        p_link->flag = 0;
        if (p_link->timer_active)
            pTimerCancel(p_link->timer_handle);
#else
        for(i=0;i<PRH_L2_MAX_NUM_LINKS;i++)
        {
            if(prh_l2_acl_links[i] == p_link)
            {
                if (p_link->timer_active)
                	{
        	            pTimerCancel(p_link->timer_handle);
       			p_link->timer_active = 0;
       			p_link->timer_handle=0;
                	}
                pFree(p_link);
                prh_l2_acl_links[i] = 0;
                return;
            }
        }
#endif
    }
}

/**************************************************************
 * Function : prh_l2_acl_get_bd_addr
 *      
 * Parameters :
 *       handle :- An ACL connection handle.
 *
 * Returns
 *       A pointer the Bluetooth device address of the peer on the link
 *
 * Description 
 * Returns a pointer to the Device Address of the peer on an ACL
 * link identified by a connection handle.
 *
 **************************************************************/

t_bdaddr* prh_l2_acl_get_bd_addr(u_int16 handle)
{
    prh_t_l2_acl_link* p_link;
    int i;

    for(i=0;i<PRH_L2_MAX_NUM_LINKS;i++)
    {
#if COMBINED_HOST==1
       p_link = &(prh_l2_acl_links[i]);
       if (p_link->flag != 0)
#else
       p_link = prh_l2_acl_links[i];
       if (p_link != 0)
#endif
       {
           if(p_link->handle == handle)
               return &p_link->bd_addr;
       }
    }
    return 0;
}


/**************************************************
 * Function :- prh_l2_acl_start_RTX_timer
 *
 * Parameter
 *      handle :-   Handle for the ACL link on which 
 *                  the signalling command is sent.
 * Returns
 *      none
 *
 * Description
 * This function initiates an RTX timer for a signalling
 * command sent on an ACL link. This is distinct from normal
 * local signalling scenarios where the timer is started for
 * signalling specific to a user CID. 
 * In this case no user CID may exist and the commands are
 * only tied to the current ACL.
 * This is specifically implemented to support RTX for :
 *          L2CAP_ECHO_REQUEST
 *          L2CAP_INFORMATION_REQUEST
 **************************************************/

void prh_l2_acl_start_RTX_timer(u_int16 handle)
{
    prh_t_l2_acl_link* p_link = prh_l2_acl_find_handle(handle);
  
    p_link->timer_handle=pTimerCreate((unsigned int)PRH_L2_RTX_TIMEOUT_VALUE,
        prh_l2_acl_timer_expired,p_link,pTIMER_ONESHOT);
    p_link->timer_active = 0x01;
}

/**************************************************
 * Function :- prh_l2_acl_disable_RTX_timer
 *
 * Parameter
 *      handle :-   Handle for the ACL link on which 
 *                  the signalling command is sent.
 * Returns
 *      none
 *
 * Description
 * This function is used to disable the RTX timer for
 * an ACL link.
 **************************************************/

void prh_l2_acl_disable_RTX_timer(prh_t_l2_acl_link* p_link)
{ 
    p_link->timer_active = 0x00;
	if(p_link->timer_handle)
		{
		    pTimerCancel(p_link->timer_handle);
			p_link->timer_handle = 0;
		}
}

/**************************************************
 * Function :- prh_l2_acl_timer_expired
 *
 * Parameter
 *      p_link :-   A pointer to an ACL link structure.
 * Returns
 *      none
 *
 * Description
 * This function called back by the papi timers when the
 * timer for an ACL link has expired.
 **************************************************/

void prh_l2_acl_timer_expired(void *arg)
{

    prh_t_l2_acl_link* p_link=(prh_t_l2_acl_link*)arg;
    t_bdaddr* p_bd_addr;
	
    if (p_link != 0)
    {
        p_bd_addr = prh_l2_acl_get_bd_addr(p_link->handle);
        if (p_link->op_pending == L2CAP_ECHO_REQUEST)
        {
            p_link->ping_callback(*p_bd_addr,L2CAP_PING_TIMEOUT_OCCURED,0, 0);
			
			/* 
			* From the new test specs it appears that if a ping fails then all
			* channels on that ACL should be released.
			*
			*/
			
			prh_l2_chn_free_channels_on_handle(p_link->handle);
			/* Release the ACL */
#if COMBINED_HOST==1
		    LMconnection_LM_Disconnect_Req(p_link->handle,PRH_USER_ENDED_CONNECTION);
#else
		    prh_mgr_releaseACL(p_link->handle,PRH_USER_ENDED_CONNECTION, NULL);
#endif
            prh_l2_acl_free_link(p_link);
        }
        else if (p_link->op_pending == L2CAP_INFORMATION_REQUEST)
        {
            p_link->info_callback(*p_bd_addr,L2CAP_INFO_TIMEOUT_OCCURED ,0,0);
			/* As the peer device is non responsive release all Channels */
			prh_l2_chn_free_channels_on_handle(p_link->handle);
			/* Release the ACL */
#if COMBINED_HOST==1
		    LMconnection_LM_Disconnect_Req(p_link->handle,PRH_USER_ENDED_CONNECTION);
#else
		    prh_mgr_releaseACL(p_link->handle,PRH_USER_ENDED_CONNECTION, NULL);
#endif
            prh_l2_acl_free_link(p_link);
        }
    }
}


/**************************************************
 * Function :- prh_l2_acl_timer_expired
 *
 * Parameter
 *      p_link :-   A pointer to an ACL link structure.
 * Returns
 *      none
 *
 * Description
 * This function called back by the papi timers when the
 * timer for an ACL link has expired.
 **************************************************/

void prh_l2_acl_timer_expired_obex(void *arg)
{

    prh_t_l2_acl_link* p_link=(prh_t_l2_acl_link*)arg;
    t_bdaddr* p_bd_addr;
	
    if (p_link != 0)
    {
        p_bd_addr = prh_l2_acl_get_bd_addr(p_link->handle);
        if (p_link->op_pending == L2CAP_ECHO_REQUEST)
        {
		pDebugPrintfEX((pLOGERROR,pLOGOBEX,"prh_l2_acl_timer_expired_obex L2CAP_ECHO_REQUEST\n"));
            p_link->ping_callback(*p_bd_addr,L2CAP_PING_TIMEOUT_OCCURED,0, 0);
			
			/* 
			* From the new test specs it appears that if a ping fails then all
			* channels on that ACL should be released.
			*
			*/
			
			prh_l2_chn_free_channels_on_handle(p_link->handle);
			/* Release the ACL */
#if COMBINED_HOST==1
		    LMconnection_LM_Disconnect_Req(p_link->handle,PRH_USER_ENDED_CONNECTION);
#else
		    prh_mgr_releaseACL(p_link->handle,PRH_USER_ENDED_CONNECTION, NULL);
#endif
            prh_l2_acl_free_link(p_link);
        }
        else if (p_link->op_pending == L2CAP_INFORMATION_REQUEST)
        {
		pDebugPrintfEX((pLOGERROR,pLOGOBEX,"prh_l2_acl_timer_expired_obex L2CAP_INFORMATION_REQUEST\n"));
            p_link->info_callback(*p_bd_addr,L2CAP_INFO_TIMEOUT_OCCURED ,0,0);
			/* As the peer device is non responsive release all Channels */
			prh_l2_chn_free_channels_on_handle(p_link->handle);
			/* Release the ACL */
#if COMBINED_HOST==1
		    LMconnection_LM_Disconnect_Req(p_link->handle,PRH_USER_ENDED_CONNECTION);
#else
		    prh_mgr_releaseACL(p_link->handle,PRH_USER_ENDED_CONNECTION, NULL);
#endif
            prh_l2_acl_free_link(p_link);
        }
	else
	{
		pDebugPrintfEX((pLOGERROR,pLOGOBEX,"prh_l2_acl_timer_expired_obex\n"));
		prh_l2_chn_free_channels_on_handle(p_link->handle);
		prh_mgr_releaseACL(p_link->handle,PRH_USER_ENDED_CONNECTION, NULL);
   		prh_l2_acl_free_link(p_link);
	}	
    }
}

/**************************************************************
 * Function : prh_l2_acl_shutdown_all
 *      
 * Parameters :
 *      none
 *
 * Returns
 *      --
 *
 * Description 
 * Frees an all ACL links current active and releases all channels 
 * active on the link.
 *
 **************************************************************/

void  prh_l2_acl_shutdown_all(u_int32 flags)
{
    prh_t_l2_acl_link* p_link;
    u_int16 i;

    for(i=0;i<PRH_L2_MAX_NUM_LINKS;i++)
    {
#if COMBINED_HOST==1
        p_link = &(prh_l2_acl_links[i]);

        if(p_link->flag != 0)
#else
        p_link = prh_l2_acl_links[i];

        if(p_link != 0)
#endif
        {
            if(p_link->handle != 0)
                prh_l2_chn_free_channels_on_handle(p_link->handle);
#if COMBINED_HOST==1
	     LMconnection_LM_Disconnect_Req(p_link->handle,PRH_USER_ENDED_CONNECTION);
#else
            prh_mgr_releaseACL(p_link->handle,PRH_USER_ENDED_CONNECTION, NULL);
#endif
            prh_l2_acl_free_link(p_link);
        }
    }
}
