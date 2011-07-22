/**************************************************************************
 * MODULE NAME:    l2_utils.c
 * PROJECT CODE:
 * DESCRIPTION:    helper functions for the L2CAP layer
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
 * This module provides some common helper functions for the L2CAP layer.
 * 
 **************************************************************************/

#include "host_types.h"

#include "l2_types.h"
#include "l2_const.h"
#include "l2_channel.h"
#include "l2_groups.h"
#include "l2_acl_link.h"
#include "l2_psm.h"
#include "l2_sar.h"
#include "l2_connect_sm.h"
#include "l2_enc_dec.h"
#include "l2_config_helper.h"
#include "l2cap_lower_layer.h"
#include "host_ctrl.h"

#include "papi.h"

/**************************************************************
 * Function : prh_l2_utils_addr_match
 *      
 * Parameters :
 *      p_bd_addr1 :- Pointer to the destination Address.
 *      p_bd_addr2 :- Pointer to the source Address
 *
 * Returns
 *      u_int8      The outcome of the function.
 *                      0x00 -- Address Mis-Match
 *                      0x01 -- Address Match
 * Description 
 * Local function which compares two bluetooth device addresses
 *
 **************************************************************/

u_int8 prh_l2_utils_addr_match(t_bdaddr* p_bd_addr1,t_bdaddr* p_bd_addr2)
{
    if ((p_bd_addr1->bytes[0] == p_bd_addr2->bytes[0]) &&
        (p_bd_addr1->bytes[1] == p_bd_addr2->bytes[1]) &&
        (p_bd_addr1->bytes[2] == p_bd_addr2->bytes[2]) &&
        (p_bd_addr1->bytes[3] == p_bd_addr2->bytes[3]) &&
        (p_bd_addr1->bytes[4] == p_bd_addr2->bytes[4]) &&
        (p_bd_addr1->bytes[5] == p_bd_addr2->bytes[5]))
        return 1;

    else
        return 0;
}

/**************************************************************
 * Function : prh_l2_utils_set_bd_addr
 *      
 * Parameters :
 *      p_bd_addr1 :- Pointer to the destination Address.
 *      p_bd_addr2 :- Pointer to the source Address
 *
 * Returns
 *      none
 *
 * Description 
 * Assigns the contents of one Device Address to another.
 *
 **************************************************************/

void prh_l2_utils_set_bd_addr(t_bdaddr* p_dest_bd_addr, t_bdaddr* p_src_bd_addr)
{
    pMemcpy(p_dest_bd_addr->bytes,p_src_bd_addr->bytes,6);
}




#if (PRH_L2CAP_INTERNAL_HOST_TESTER==1)
extern u_int16 pending_cid; 
extern u_int8 L2_Send_Disconnect_Response;
extern u_int8 L2_Send_Echo_Response;
extern u_int8 tester_config;
#endif

static void l2cap_layer_shutdown(void) {
    int i;
    i=0;
#if COMBINED_HOST==0
    while (prh_l2_channel_list[i] && i<PRH_L2_MAX_NUM_CHANNEL_IDENTIFIERS) {
        if (prh_l2_channel_list[i])
            prh_l2_chn_free_channel(prh_l2_channel_list[i], 0);
        i++;
    };    
    i=0;
    while (prh_l2_acl_links[i] && i<PRH_L2_MAX_NUM_LINKS) {
        if (prh_l2_acl_links[i])
            prh_l2_acl_free_link(prh_l2_acl_links[i]);
        i++;
    };
#else
    while (prh_l2_channel_list[i].flag && i<PRH_L2_MAX_NUM_CHANNEL_IDENTIFIERS) {
        if (prh_l2_channel_list[i].flag)
            prh_l2_chn_free_channel(&(prh_l2_channel_list[i]), 0);
        i++;
    };    
    i=0;
    while (prh_l2_acl_links[i].flag && i<PRH_L2_MAX_NUM_LINKS) {
        if (prh_l2_acl_links[i].flag)
            prh_l2_acl_free_link(&(prh_l2_acl_links[i]));
        i++;
    };
#endif
#if PRH_L2CAP_AUTO_CONFIG==1
    prh_l2_config_helper_shutdown(0); 
#endif
}

static void l2cap_layer_startup(void) {
#if (PRH_L2CAP_INTERNAL_HOST_TESTER==1)
        L2_Send_Disconnect_Response = 0x1;
        L2_Send_Echo_Response = 0x1;
        tester_config=0x00;
#endif        
		prh_l2_enc_set_cur_ident(0x44);
#if (L2CAP_GROUPS_SUPPORTED==1)
        pMemset(prh_l2_cls_psm_list,0,sizeof(prh_t_cls_psm)*PRH_L2_MAX_GROUPS);
#endif
        pMemset(prh_l2_psm_cb_list,0,sizeof(struct l2cap_psm)*PRH_L2_MAX_NUM_PSM);
        pMemset(prh_l2_psm_id_array,0,sizeof(u_int16)*PRH_L2_MAX_NUM_PSM);
#if COMBINED_HOST==0
        pMemset(prh_l2_acl_links,0,sizeof(prh_t_l2_acl_link*)*PRH_L2_MAX_NUM_LINKS);
        pMemset(prh_l2_channel_list,0,sizeof(prh_t_l2_channel*)*PRH_L2_MAX_NUM_CHANNEL_IDENTIFIERS);
        prh_hci_register_l2cap_event_cb(prh_l2_connect_event_handler);
        prh_hci_register_l2cap_acl_cb(prh_l2_sar_data_ind);
#else
        pMemset(&prh_l2_acl_links,0,sizeof(prh_t_l2_acl_link*)*PRH_L2_MAX_NUM_LINKS);
        pMemset(&prh_l2_channel_list,0,sizeof(prh_t_l2_channel*)*PRH_L2_MAX_NUM_CHANNEL_IDENTIFIERS);
#endif

#if PRH_L2CAP_AUTO_CONFIG==1
        prh_l2_config_helper_startup(0);
#endif    

}

int prh_l2cap_layer_ctrl(u_int8 flags) {
    if (flags&PRH_HOST_STARTUP) {
        l2cap_layer_startup();
    } else if (flags&PRH_HOST_RESET) {
        l2cap_layer_shutdown();
        l2cap_layer_startup();
    } else if (flags&PRH_HOST_SHUTDOWN) {
        l2cap_layer_shutdown();
    }
    return BT_NOERROR;
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_l2_utils_out_mtu
 * DESCRIPTION:
 
 This function will return the mtu that is assigned for a channel

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
t_api prh_l2_utils_out_mtu(u_int16 cid,u_int16* mtu)
{
  prh_t_l2_channel* p_channel;

  p_channel = prh_l2_chn_get_p_channel(cid);
#if COMBINED_HOST==0
  if(p_channel) *mtu = p_channel->outMTU; else return BT_INVALIDPARAM; 
#else
	if(p_channel) 
		*mtu = p_channel->outMTU; 
	else 
		return BT_INVALIDPARAM; 
#endif
  return BT_NOERROR;
}
