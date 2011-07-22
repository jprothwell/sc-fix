/*
 * MODULE NAME:    host_sched.c
 * DESCRIPTION:    Host system scheduler implementation
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: host_sched.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies.
 *     All rights reserved.
 *
 */
#include "sys_config.h"
#include "host_config.h"

#include "host_timer.h"

#include "host_types.h"
#include "l2_sar.h"

#include "lmp_acl_container.h"
#include "tra_queue.h"
#include "headset_sched.h"

#if (BUILD_TYPE!=UNIT_TEST_BUILD)
#include "hs_main.h"
#endif

/* host schedule function */
u_int8  BThost_Sched(u_int32 num_interations)
{
	// dispatch l2cap data
    t_deviceIndex device_index;    
    t_lmp_link *p_link ;
    t_q_descr *qd;
    struct host_buf*buf;
    u_int8 status = 0;

    /*
     * This array stores the header of the PDU to be transmitted to the host.
     * The maximum header is 5, 1 for the type byte and 4 for an ACL header.
     */
    u_int32 inter;

    for(inter = 0; inter < num_interations; num_interations--)
    {

        /* 
         * Start with device index 1 - 0 isn't used for received data
         */
    
        for(device_index=1;device_index<MAX_ACTIVE_DEVICE_LINKS;device_index++)
        {
            qd = BTq_Dequeue_Next(L2CAP_IN_Q, device_index);

			if(qd)
			{
				//p_link = LMaclctr_Find_Device_Index(qd->device_index);
				buf = host_buf_alloc(qd->length);
				buf->data = qd->data;
                          buf->link_handle = LMaclctr_Find_Device_Index(qd->device_index)->handle;
				prh_l2_sar_data_ind(buf->link_handle, buf, (qd->message_type) | (qd->broadcast_flags<<2));
				BTq_Ack_Last_Dequeued(L2CAP_IN_Q, device_index, qd->length);
				status |= 0x10;
			}
		}
#if HEADSET_PRESENT
#if (BUILD_TYPE!=UNIT_TEST_BUILD)
		status |= HSMain_Sched();
#endif
#endif		

#if HFP_PRESENT
		//status |= Headset_Hfp_Sched();
#endif

	    host_handle_expired_timers();
    }
    return status;
}

