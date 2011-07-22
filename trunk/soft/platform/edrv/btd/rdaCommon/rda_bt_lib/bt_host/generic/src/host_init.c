/*
 * MODULE NAME:    host_sched.c
 * DESCRIPTION:    Host system scheduler implementation
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: host_init.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies.
 *     All rights reserved.
 *
 */

#include "host_config.h"

#include "host_types.h"
#include "host_ctrl.h"
#include "host_error.h"
#include "headset_sched.h"
#include "hfp_core.h"

static t_api host_layer_ctrl(u_int8 layerFlags);

void  BThost_Initialise(void)
{
	host_layer_ctrl(PRH_HOST_STARTUP);

#if HEADSET_PRESENT
#if A2DP_PRESENT
	Headset_Init();
#endif
#if (BUILD_TYPE!=UNIT_TEST_BUILD)
	HSMain_Initialise();
#endif
#endif	
}


/*************************************************************************************************
 * host_layer_ctrl
 *
 * Passes startup/shutdown commands to the stack layers. 
 * NB: starting/stopping individual layers in isolation is not supported.
 ************************************************************************************************/

t_api host_layer_ctrl(u_int8 layerFlags) {
    t_api status=0;

#if HCI_PRESENT 
   status|=prh_hci_layer_ctrl(layerFlags);
#endif

#if MANAGER_PRESENT
    status|=prh_manager_layer_ctrl(layerFlags);
#endif

#if L2CAP_PRESENT
    status|=prh_l2cap_layer_ctrl(layerFlags);
#endif

#if SDP_CLIENT
    status|=prh_sdp_cli_layer_ctrl(layerFlags);
#endif

#if SDP_SERVER
    status|=prh_sdp_srv_layer_ctrl(layerFlags);
#endif

#if RFCOMM_PRESENT
    status|=prh_rfcomm_layer_ctrl(layerFlags);
#endif

#if A2DP_PRESENT
	status|=Avdtp_Layer_Ctrl(layerFlags);
#endif

#if AVRCP_PRESENT
	status|=Avctp_Layer_Ctrl(layerFlags);
#endif

#if OBEX_PRESENT
    status|=prh_obex_layer_ctrl(layerFlags);
#endif
    
    if (status)
        return BT_UNKNOWNERROR;
    else
        return BT_NOERROR;
}




