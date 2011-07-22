/***********************************************************************
 *
 * MODULE NAME:    tcs_gm_clock.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    TCS Wireless User Group Clock Offset Linked List
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  06 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_gm_clock.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)

#include "tcs_lower_layer.h"
#include "tcs_core.h"
#include "tcs_cc_core.h"
#include "tcs_gm_types.h"
#include "tcs_wug.h"
#include "tcs_gm_api.h"
#include "tcs_proto.h"

t_pTimer prh_tcs_t406_handle;

void prh_tcs_timer_t406_expired(void *dummy)
{
  /* reconnect back to the master again if no connection received in the time */
  
}

#if PRH_TCS_TERMINAL==1
/* This is calledby the manager when a ReadClockOffsetComplete event is recieved and TCSBIN_PRESENT is set */
void prh_tcs_wug_retreiveClockOffset(u_int16 aclHandle, u_int16 clockOffset, t_api status)
{
    struct prh_st_tcs_gm_encode_info info;
    struct prh_st_tcs_l2conn *l2_conn;
    int ret;
    if (status==BT_NOERROR)
    {
        /* do some sort of lookup on handle */
        ret=prh_tcs_lookup_l2cap_inst_by_bd_addr(TCSInstance.GWBDAddress, &l2_conn);

        if (ret==BT_NOERROR)
        {
            if (l2_conn->clk_offset_pending)
            {

                info.compspec=NULL;
                info.clock_offset=&clockOffset;
            
                prh_tcs_gm_encode_listen_accept(&info, l2_conn);
            
                prh_tcs_t406_handle=pTimerCreate(PRH_TCS_TIMER_T406, prh_tcs_timer_t406_expired, NULL, pTIMER_ONESHOT);
                /* need to close the L2CAP connection to the master around here - how do I know he got the accept though.. */
#if (PRH_TCS_MULTIPICONETCAPABLE==0)
                L2_Disconnect(l2_conn->cid);
#endif
            }
        }
        
    }
}
#endif
#endif /*(PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/
