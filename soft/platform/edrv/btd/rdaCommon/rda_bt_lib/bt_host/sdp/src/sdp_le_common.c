/***********************************************************************
 *
 * MODULE NAME:    sdp_le_common.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Service Discovery Test code
 * MAINTAINER:     Mark Donnelly
 *
 * SOURCE CONTROL: $Id: sdp_le_common.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/


#include "host_config.h"

#if SDP_CLIENT || SDP_SERVER

#include "sdp_stack_access.h"
#include "sdp_constants.h"
#include "sdp_callbacks.h"


#if SDP_CLIENT 
#include "sdp_state_machine.h"
extern struct prh_sdp_primitive_task* p_SDP_Primitive_Task_List;
#endif 

/***********************************************************************
 *
 * FUNCTION NAME:   SDP_RegisterPSM
 * DESCRIPTION:
 
 This function will perform some initialisation routines for the sdp. At
the moment this is simply the registering of the callbacks.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
void SDP_RegisterPSM(void)
{
	l2_callbacks cbs;
	
	/* initialise the mtu */
#if SDP_CLIENT
	p_SDP_Primitive_Task_List = 0x00;
#endif 
    
	cbs.disconnectionIndication = prh_sdp_l2cap_disconnect_ind_cb;
	cbs.qosviolationIndication = 0x00; /* null */
	cbs.connectionIndication = prh_sdp_l2cap_connect_ind_cb;
	cbs.disconnectionConfirmation = prh_sdp_l2cap_disconnect_cfm_cb;
	cbs.dataIndication = prh_sdp_l2cap_data_ind_cb;
	cbs.flushOccurred=pNULL;
	cbs.bufferOverflow=pNULL;
	cbs.timeoutIndication=pNULL;
	
#if PRH_L2CAP_AUTO_CONFIG==0
	cbs.connectionConfirmation = prh_sdp_l2cap_connect_cfm_cb;
	cbs.configureIndication = prh_sdp_l2cap_config_ind_cb;
	cbs.configureConfirmation = prh_sdp_l2cap_config_cfm_cb;
#else
	cbs.connectionConfirmation = pNULL;
	cbs.configureIndication = pNULL;
	cbs.configureConfirmation = pNULL;
	cbs.connectionConfiguredConfirmation=prh_sdp_l2cap_connect_configured_cfm_cb;
#endif
	
	L2_RegisterPSM(SDP_PSM,&cbs);
	
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_allocate_tx_buffer
 * DESCRIPTION:
 
 This function allocates a host buffer for transmission. The length is set
to the sdp mtu length

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
struct host_buf* prh_sdp_allocate_tx_buffer(u_int32 size, u_int16 cid)
{
	
	struct host_buf* h_buff;
	
	h_buff = NULL;

	prh_l2_GetWriteBuffer(cid, (u_int16)size, PRH_HOST_BUF_USE_RESV_ACL_MEM, &h_buff);
	
	return h_buff;
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_transfer_data
 * DESCRIPTION:
 
 This function will calculate the length and add it to the host buffer
the host buffer is then sent using the l2cap

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
void prh_sdp_transfer_data(t_channelId cid,struct host_buf* h_buff)
{
	h_buff->len =  SDP_PDU_HEADER_LENGTH + (h_buff->data[3] << 8) + h_buff->data[4];
	
	L2_Write(cid,(struct st_t_dataBuf *)h_buff);
}



/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_read16
 * DESCRIPTION:
 
 This function reads a u_int16 from a pdu

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
u_int16 prh_sdp_read16(u_int8* p_pdu)
{
	return ( ((u_int16)p_pdu[0]) << 8) + (u_int16)p_pdu[1];
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_read32
 * DESCRIPTION:
 
 This function reads a u_int32 from a pdu

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
u_int32 prh_sdp_read32(u_int8* pData)
{
	return (((u_int32)pData[0]) << 24) + (((u_int32)pData[1]) <<16) + ( ((u_int32)pData[2]) << 8) + (u_int32)pData[3];
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_write16
 * DESCRIPTION:
 
 This function reads a u_int16 from a pdu

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
void prh_sdp_write16(u_int16 data, u_int8* buffer)
{
	buffer[0] = data >> 8;
	buffer[1] = data & 0xff;
}

/***********************************************************************
 *
 * FUNCTION NAME:   prh_sdp_write32
 * DESCRIPTION:
 
 This function reads a u_int16 from a pdu

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
void prh_sdp_write32(u_int32 data, u_int8* buffer)
{
	buffer[0] = data >> 24;
	buffer[1] = (data>> 16) & 0xff;
	buffer[2] = (data >> 8) & 0xff;
	buffer[3] = data & 0xff;
}



#endif /* SDP_CLIENT || SDP_SERVER */



