
/**************************************************************************
 * MODULE NAME:    l2_groups.c
 * PROJECT CODE:
 * DESCRIPTION:    l2cap groups handler.
 * AUTHOR:         Gary Fleming
 * DATE:           02-07-2000
 *
 *
 * LICENSE
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Plc.
 *     All rights reserved.
 *
 * DESCRIPTION
 * This module is responsible for the support of L2CAP groups and the reception
 * of connection less traffic.
 *************************************************************************/

#include "host_config.h"
#include "host_types.h"
#include "host_error.h"
#include "host_buf.h"

#include "l2_types.h"
#include "l2_connect_sm.h"
#include "l2_const.h"
#include "l2_enc_dec.h"
#include "l2_acl_link.h"
#include "l2_channel.h"
#include "l2_psm.h"
#include "l2_groups.h"
#include "l2cap_primitives.h"
#include "l2cap_lower_layer.h"
#include "papi.h"

#if (L2CAP_GROUPS_SUPPORTED==1)
/***************************************************************** 
 * An array of structures used for groups 
 *****************************************************************/ 
prh_t_cls_psm prh_l2_cls_psm_list[PRH_L2_MAX_GROUPS];
static prh_t_cls_psm* prh_l2_group_find_cls_psm(u_int16 psm);

/******************************************************************************
 * 
 * Function : L2_GroupCreate
 *
 * Parameters :
 *      u_int16 psm     The PSM identifier to be used for the group.
 *      u_int16* p_cid  A pointer to the local CID to be allocated to the group.
 *
 * Returns 
 *      t_api           An error code indicating success/failure of the primitive.
 *
 * Description 
 * The use of this primitive requests the creation of a CID to represent a 
 * logical connection to multiple devices. Input parameter is the PSM value that 
 * the outgoing connectionless traffic is labelled with, and the filter used for 
 * incoming traffic. Output parameter is the CID representing the local endpoint.
 * On creation, the group is empty but incoming traffic destined for the PSM
 * value is readable.
 *
 * Main Steps Are :
 *   1/ Allocate a Channel
 *   2/ Allocate and Initialise a PSM structure
 *   3/ Enable Connectionless Traffic for this PSM.
 *   4/ Allocate a BD_Addr list and assign to the PSM structure.
 *****************************************************************************/


APIDECL1 t_api APIDECL2 L2_GroupCreate(u_int16 psm, u_int16 *p_cid)
{
#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
     u_int8 temp[2];
#endif
    u_int16 psm_ident;
    t_api status = BT_UNKNOWNERROR; 
    prh_t_l2_channel *p_channel;
    List  *p_bda_list;
    prh_t_cls_psm *p_cls_psm;

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: L2_GroupCreate called, psm=0x%02X\n",psm));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    BT_WRITE_LE_UINT16(temp,psm);
    SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_GROUPS_INDEX),temp,2,BT_CODE_WORD);
#endif

	if (p_cid == NULL)
		return BT_INVALIDPARAM;

    p_channel = prh_l2_chn_alloc_channel();

    if(p_channel)
    {
        p_channel->type = PRH_L2_CLS_CHANNEL;

        psm_ident= prh_l2_psm_get_psm_ident(psm);
        if(psm_ident)
        {   
            p_cls_psm = &prh_l2_cls_psm_list[psm_ident];
            p_cls_psm->psm = psm;
            p_channel->psm = psm;
            L2_EnableConnectionlessTraffic(psm);
            p_bda_list = prh_l2_bda_list_init();
            if (p_bda_list != 0)
            {
                status = BT_NOERROR;
                p_channel->bda_list = p_bda_list;
                *p_cid = p_channel->local_cid;
            }
        }
        else 
            status = BT_INVALIDPARAM;
    }

    if (status != BT_NOERROR)
    {
        if(p_channel)
            prh_l2_chn_free_channel(p_channel,PRH_L2_DONT_RELEASE_ACL);
    }

    return status;  
}

/******************************************************************************
 * 
 * Function : L2_GroupClose
 *
 * Parameter(s)
 *
 *      group_cid :-    The channel identifier for the group to be released.
 *
 * Returns
 *      t_api           BT_NOERROR
 *                      BT_INVALIDPARAM.
 * Description
 * This function closes a group and frees the Channel and PSM associated with
 * it. Each member on the group is checked to determine if there are other channels
 * open to it. If no other channels or groups exist to this group member the ACL
 * link can be released.
 * 
 *****************************************************************************/

APIDECL1 t_api APIDECL2 L2_GroupClose(u_int16 group_cid)
{
#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
u_int8 temp[2];
#endif
    t_api status = BT_NOERROR;
    t_bdaddr* p_bd_addr;
    prh_t_l2_channel* p_channel;
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: L2_GroupClose called, cid=0x%02X\n",group_cid));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    BT_WRITE_LE_UINT16(temp,group_cid);
    SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_GROUPS_INDEX+1),temp,2,BT_CODE_WORD);
#endif

    p_channel = prh_l2_chn_get_p_channel(group_cid);
    if (p_channel)
    {

        while (p_channel->bda_list->head != 0)
        {
            p_bd_addr = &p_channel->bda_list->head->data;
				L2_GroupRemoveMember(group_cid, *p_bd_addr);
        };
        prh_l2_chn_free_channel(p_channel,PRH_L2_DONT_RELEASE_ACL);
    }
    else
        status = BT_INVALIDPARAM; /* Invalid CID */

    return status;  
}

/******************************************************************************
 * 
 * Function : L2_GroupAddMember
 *
 * Parameters :- 
 *  group_cid   :-  The channel identifier for the group to be added.
 *  bdaddr      :-  The Bluetooth device address of the device to be
 *                  added to the group.
 *  addMemberCfm :- The callback function to be invoked when a new member has been successfully
 *                  added to the group. This callback is only used when an ACL does not already
 *                  exist to this member.
 * Returns
 *      t_api           BT_NOERROR  ( Success )
 *                      BT_PENDING  ( ACL link being established )
 *                      BT_INVALIDPARAM ( Invalid cid )
 *                      BT_NORESOURCES
 * Description
 * This adds a new member "bdaddr" to an existing Group. The group is identified
 * by the "group_cid". In addition a callback function "addMemberCfm" is passed as 
 * the function to be called once the new member has been successfully added.
 * 
 *
 *****************************************************************************/

APIDECL1 t_api APIDECL2 L2_GroupAddMember(u_int16 group_cid, t_bdaddr bdaddr,
                                          void (*addMemberCfm)(u_int16 cid, t_bdaddr bdaddr, u_int16 result))
{
#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
u_int8 temp[8];
u_int8 i=0;
#endif

    t_api status = BT_NOERROR;
    prh_t_cls_psm *p_cls_psm;
    prh_t_l2_acl_link *p_link;
    prh_t_l2_channel *p_channel;
    
#if pDEBUG
    //pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: L2_GroupAddMember called, cid=0x%02X, addr=%s\n",group_cid,prh_mgr_printBDAddress(bdaddr)));
    pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: L2_GroupAddMember called, addr=%s,cid=0x%02X\n",prh_mgr_printBDAddress(bdaddr),group_cid));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)

     BD_ADDR_TO_UINT8_CPY(temp,bdaddr);
    //temp=prh_mgr_printBDAddress(bdaddr)
    BT_WRITE_LE_UINT16((temp+6),group_cid);
    SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_GROUPS_INDEX+2),temp,8, (BT_CODE_BDADDR | (BT_CODE_WORD<<4)));
#endif
	/* Check that the callback was set */

		if (addMemberCfm==NULL)
			return BT_INVALIDPARAM;

    p_link = prh_l2_acl_exists(&bdaddr);
    p_channel = prh_l2_chn_get_p_channel(group_cid);
    if (p_channel)
        p_cls_psm = prh_l2_group_find_cls_psm(p_channel->psm);
    else 
        return BT_INVALIDPARAM;

    if (p_link)
    {
        if (p_cls_psm)
        {
            p_cls_psm->addMemberCfm = addMemberCfm;
            prh_l2_bda_list_insert(p_channel->bda_list,&bdaddr);
        }
        else
            status = BT_INVALIDPARAM;
    }
    else /* No ACL Link Exists  */
    {
         if (p_cls_psm)
         {
             p_cls_psm->addMemberCfm = addMemberCfm;
             p_link = prh_l2_acl_alloc_link(&bdaddr,group_cid);
             if (p_link)
             {
                 p_link->op_pending = L2_GROUP_ADD;
                 status = prh_mgr_createACL(bdaddr, p_cls_psm->psm,
                     &p_link->handle, prh_l2_connect_lp_connect_cfm, (tid_t)p_link);
             }
             else
                 status = BT_NORESOURCES; 
         }
         else
              status = BT_INVALIDPARAM;
    }
    return status;
}

/******************************************************************************
 * 
 * Function : prh_l2_group_lp_add_complete
 *
 * Parameters
 *      p_link      A pointer to the ACL link being established.
 *      add_status  The outcome of the attempt to establish an ACL link.
 *
 * Returns 
 *      t_api   
 *
 * Description
 * This function is called by the lower layer when the establishment of an ACL
 * for a new group member is complete.
 * 
 *****************************************************************************/

t_api prh_l2_group_lp_add_complete(prh_t_l2_acl_link* p_link,t_api add_status)
{
    t_api status = BT_NOERROR;
    prh_t_cls_psm *p_cls_psm;
    prh_t_l2_channel* p_channel;

    if (p_link)
    {
        p_channel = prh_l2_chn_get_p_channel(p_link->pending_cid);
        if (p_channel)
        {
            p_cls_psm = prh_l2_group_find_cls_psm(p_channel->psm);
            if (p_cls_psm) 
            {
				if (add_status == BT_NOERROR)
					prh_l2_bda_list_insert(p_channel->bda_list,&p_link->bd_addr);
                p_cls_psm->addMemberCfm(p_link->pending_cid,p_link->bd_addr,(u_int16)add_status);
				if (add_status != BT_NOERROR)
					prh_l2_acl_free_link(p_link);
            }
        }
        else
            status = BT_UNKNOWNERROR;
    }
    return status;  
}


/******************************************************************************
 * 
 * Function : L2_GroupRemoveMember
 *
 * Parameters
 *      u_int16 cid         Local Identifier for the group.
 *      u_int8* bdaddr  The device to remove from the group.
 *
 * Returns
 *      t_api           BT_NOERROR
 *                      BT_INVALIDPARAM
 *
 * Description
 * Removes a member from the L2CAP Group identified by "cid". If any other channels
 * are active to this bdaddr OR there are other groups with this bdaddr as 
 * a member then the ACL link should not be closed.
 *****************************************************************************/

APIDECL1 t_api APIDECL2 L2_GroupRemoveMember(u_int16 cid, t_bdaddr bdaddr) 
{
#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
    u_int8 temp[8];
    u_int8 i;
#endif

    t_api status = BT_NOERROR;
    prh_t_l2_acl_link* p_link;
    prh_t_l2_channel* p_channel;
#if pDEBUG
    //pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: L2_GroupRemoveMember called, cid=0x%02X, addr=%s\n",cid,prh_mgr_printBDAddress(bdaddr)));
       pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: L2_GroupRemoveMember called, addr=%s, cid=0x%02X\n",prh_mgr_printBDAddress(bdaddr),cid));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)

    BD_ADDR_TO_UINT8_CPY(temp,bdaddr);

    //temp=prh_mgr_printBDAddress(bdaddr)
    BT_WRITE_LE_UINT16((temp+6),cid);
    SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_GROUPS_INDEX+3),temp,8,(BT_CODE_BDADDR | (BT_CODE_WORD<<4)));
#endif
    p_channel = prh_l2_chn_get_p_channel(cid);
    if (p_channel)
    {
        p_link = prh_l2_acl_exists(&bdaddr);
        if (!prh_l2_bda_list_remove(p_channel->bda_list,&bdaddr))
			return BT_INVALIDPARAM;
    
        if(p_link)
        {
            if(!prh_l2_chn_handle_in_use(p_link->handle))
            {
                prh_mgr_releaseACL(p_link->handle,PRH_USER_ENDED_CONNECTION, NULL);
                prh_l2_acl_free_link(p_link);
            }
        }
    }
    else
        status = BT_INVALIDPARAM;

    return status;      
}

/******************************************************************************
 * 
 * Function : L2_GroupGetMembers
 *
 * Description
 * Retrieves the Bluetooth Device Addresses of the members of a group. 
 *
 *        u_int16 cid   
 *        t_bdaddr addrList
 *        u_int8* addrList  (a pointer to a block of memory containing a number
 *                                     of Bluetooth Addresses ).
 *
 *****************************************************************************/


APIDECL1 t_api APIDECL2 L2_GroupGetMembers(u_int16 cid, t_bdaddr *addrList, u_int16 *addrListLen) 
{

#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
    u_int8 temp[4];
#endif
    t_api status;
    struct node* p_current_node;
    u_int16 current_offset=0;

    prh_t_l2_channel* p_channel;

	if ((addrList == NULL) || (addrListLen == NULL))
		return BT_INVALIDPARAM;
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: L2_GroupGetMembers called, cid=0x%02X, *addrListLen=0x%02X\n",cid,*addrListLen));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    BT_WRITE_LE_UINT16(temp,cid);
    BT_WRITE_LE_UINT16((temp+2),*addrListLen);
    SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, (LOG_L2CAP_L2_GROUPS_INDEX+4),temp,4,(BT_CODE_WORD|(BT_CODE_WORD<<4)));
#endif

    p_channel = prh_l2_chn_get_p_channel(cid);

    if (p_channel)
    {

        if(*addrListLen == 0)
        {
            *addrListLen = p_channel->bda_list->length;
            status = BT_NOERROR;
        }
        else if(p_channel->bda_list->length > *addrListLen)
        {
            *addrListLen = p_channel->bda_list->length;
            status = BT_RETRY;
        }
        else
        {
            p_current_node = p_channel->bda_list->head;
            while ((p_current_node != 0) && (current_offset != *addrListLen))
            {
                pMemcpy(addrList[current_offset].bytes,p_current_node->data.bytes,6);
                current_offset++;
                p_current_node = p_current_node->next;
            };
            *addrListLen = p_channel->bda_list->length;
            status = BT_NOERROR;
        }
    }
    else
    {
        status = BT_INVALIDPARAM;
    }
    return status;  
}


/********************************************************************
 * Function :- prh_l2_group_find_cls_psm
 *
 * Parameters
 *      psm             The PSM identifier for the group.
 *
 * Returns
 *      prh_t_cls_psm*  A pointer to the connectionless PSM structure used to describe the group.
 *
 * Description
 * Scans the Connectionless PSM list to find a match to the "group_cid" parameter
 * If a match is found it returns a pointer to the structure describing the
 * Group for the connectionless PSM.
 *********************************************************************/

prh_t_cls_psm* prh_l2_group_find_cls_psm(u_int16 psm)
{
    int i;

    for (i=0; i<PRH_L2_MAX_NUM_PSM; i++)
    {
        if ((prh_l2_cls_psm_list[i].psm == psm))
        {
            return &prh_l2_cls_psm_list[i];
        }
    }
    return 0;
}

#endif   /* End of #if (L2CAP_GROUPS_SUPPORTED==1) */
