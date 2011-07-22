/***********************************************************************
 *
 * MODULE NAME:    rfcomm_l2auto_state.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    RFCOMM L2CAP Channel Handler State Machine using Auto Configuration Scheme
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  02 Feb 2000
 *
 * SOURCE CONTROL: $Id: rfcomm_l2auto_state.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Inc.
 *     All rights reserved.
 *
 ***********************************************************************/

#include "rfcomm_lower_layer.h"
#include "rfcomm_ue_api.h"
#include "rfcomm_core.h"
#include "rfcomm_frame.h"
#include "rfcomm_proto.h"
#include "rfcomm_debug.h"
#if COMBINED_HOST==1
#include "papi.h"
#endif
#define PRH_L2CAP_MTU_MIN 48

static t_api prh_rfcomm_l2cap_state_idle(struct prh_rfcomm_l2cap_inst *l2cap_inst, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info);
static t_api prh_rfcomm_l2cap_state_w4conn(struct prh_rfcomm_l2cap_inst *l2cap_inst, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info);
static t_api prh_rfcomm_l2cap_state_conn(struct prh_rfcomm_l2cap_inst *l2cap_inst, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info);
static t_api prh_rfcomm_l2cap_state_w4rl(struct prh_rfcomm_l2cap_inst *l2cap_inst, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info);
static t_pTimer wait_dis_dlc0_timer;
void prh_rfcomm_l2cap_wait_dis_dlc0_timeout(void *param);

/*
 * l2cap_state
 * -- Strucutre to store handler functions for each state
 */
static t_api (*l2cap_state[])(struct prh_rfcomm_l2cap_inst *self, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info) = 
{
    prh_rfcomm_l2cap_state_idle,
    prh_rfcomm_l2cap_state_w4conn,
    prh_rfcomm_l2cap_state_conn,
    prh_rfcomm_l2cap_state_w4rl
};

/*
 * prh_rfcomm_l2cap_do_event
 * -- Dispatches event to the current state handler
 */
t_api prh_rfcomm_l2cap_do_event(struct prh_rfcomm_l2cap_inst *l2cap_inst, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info)
{
    return(*l2cap_state[l2cap_inst->l2cap_state])(l2cap_inst, event, info);
}

/*
 * prh_rfcomm_l2cap_next_state
 * -- Set the next state
 */
void prh_rfcomm_l2cap_next_state(struct prh_rfcomm_l2cap_inst *l2cap_inst, PRH_RFCOMM_L2CAP_STATE state)
{
    l2cap_inst->l2cap_state=state;
}
 
/*
 * prh_rfcomm_l2cap_state_idle
 * -- L2CAP Channel Handler Idle state handler
 *
 * Handles:
 *    Connection Indication from lower layer:
 *       -  set next state to W4CONFIG,
 *       -  transmit an L2CAP Connect response
 *       -  set configure state
 *       -  transmit an L2CAP Configure request
 *    RFSTART Request from upper layer:
 *       -  set next to state to wait for connect confirm
 *       -  Send L2CAP connect request
 */
static t_api prh_rfcomm_l2cap_state_idle(struct prh_rfcomm_l2cap_inst *l2cap_inst, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info)
{
  int ret=0;
  t_L2_ConfigReqParams req_params;

   #if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
  u_int8 temp; 
  #endif
  /* In the idle state only two transitions are available,
   * RF_START from Port Entity, initiates an L2CAP Connection,
   * L2CAP_IND from L2CAP, denoting remote side wishes to 
   * initiate a connection 
   */
  req_params.inMTUmax=l2cap_inst->sys_params.max_frame_size+5;
  req_params.outMTUmax=l2cap_inst->sys_params.max_frame_size+5;
  req_params.inMTUmin=PRH_L2CAP_MTU_MIN;
  req_params.outMTUmin=PRH_L2CAP_MTU_MIN;
  req_params.flushTO=0xffff;
  req_params.linkTO=0;
  pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"prh_rfcomm_l2cap_state_idle: event=%d\n", event));
  switch(event) 
    {
    case PRH_RFCOMM_L2CAP_CONNECT_IND:
      /* If we receive an L2CAP Indication, we take the info,
     send and L2CAP_ConnectResp and move to L2CAP Connected stage */

      /* Move to L2CAP Connected stage */
      l2cap_inst->bd_addr=info->bd_addr;
      l2cap_inst->cid=info->cid;

      prh_rfcomm_l2cap_next_state(l2cap_inst, PRH_RFCOMM_L2CAP_W4CONN);      
      
      /*
       * Send L2CAP Connect Response and Start Configuration
       */
      l2cap_inst->connector=0;
      L2_ConnectResponseandConfig(info->bd_addr, info->cid, 0, 0, &req_params);
      
      break;
    case PRH_RFCOMM_RFSTART_REQ:
      /* If we receive an RFSTART_REQ, we go into an L2CAP Connection
     and wait for the answer */
      /* Transmit an L2CAP Connection Request... this blocks */
      l2cap_inst->cb=info->cb.multi;

      l2cap_inst->bd_addr=info->bd_addr;

      if (l2cap_inst->credit_state!=CREDIT_OFF)
	l2cap_inst->credit_state=CREDIT_REQUESTED;

      prh_rfcomm_l2cap_next_state(l2cap_inst, PRH_RFCOMM_L2CAP_W4CONN);
      l2cap_inst->connector=1;
      ret=L2_ConnectandConfig(info->bd_addr, PRH_RFCOMM_PSM, &req_params);
      
      break;
    default:
#if pDEBUG
      pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"Event illegal in IDLE state\n, %d", event));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
      temp=(u_int8)event;
      SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM,LOG_RFC_RFCOMM_L2AUTO_STATE_INDEX,&temp,1,BT_CODE_BYTE);
#endif
      return BT_INVALIDPARAM;
    }
  return ret;
}

/*
 * prh_rfcomm_l2cap_state_w4conncfm
 * -- L2CAP Channel Handler Wait For Connect Confirm state handler
 *
 * Handles:
 *    Connect Confirm from L2CAP:
 *       - Check result of connection 
 *       - if valid set next state to configuration
 *       - transmit a configuration request
 *       - if invalid callback the upper layer with status
 */
static t_api prh_rfcomm_l2cap_state_w4conn(struct prh_rfcomm_l2cap_inst *l2cap_inst, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info)
{
  struct prh_rfcomm_dlc_inst *dlc_inst=0;
  struct prh_rfcomm_event_info dlc_event;  
  t_L2_ControlCallbackArgs *args;
  int ret;
  pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"prh_rfcomm_l2cap_state_w4conn: event=%d\n", event));

  switch (event)
    {

    case PRH_RFCOMM_L2CAP_CONNECT_CONFIG_CFM:
      args=info->priv;
      if (l2cap_inst->cid==0)
	l2cap_inst->cid=args->cid;
      if (args->result==0)
	{
	  /* read contents of configure results */
	  prh_rfcomm_l2cap_next_state(l2cap_inst, PRH_RFCOMM_L2CAP_CONN);      	  
	  /* we get an MTU back from peer, compare with our frame size - 5 */
	  if (args->mtu<l2cap_inst->sys_params.max_frame_size+5)
	    l2cap_inst->sys_params.max_frame_size=args->mtu-5;

	  dlc_event.priv=NULL;
	  if (l2cap_inst->connector==1)
	    {
	      ret=prh_rfcomm_dlc_init(l2cap_inst, PRH_RFCOMM_DLCI0, &dlc_inst, 1);
	      if (ret==BT_NOERROR)
		{
		  prh_rfcomm_dlc_do_event(dlc_inst, PRH_RFCOMM_DLC_ESTAB_REQ, &dlc_event);
		  pMutexUnlock(dlc_inst->lock);
		}
	    }
	}
      if (args->result>1)
        {
          /* Callback to the layer above with status */
          if (l2cap_inst->cb.rfstart_conf)
	    (*l2cap_inst->cb.rfstart_conf)(l2cap_inst->bd_addr, NULL, PRH_RFCOMM_REFUSE, l2cap_inst);
pDebugPrintfEX((pLOGDEBUG,pLOGOBEX,"prh_rfcomm_l2cap_state_w4conn 2\n"));
          prh_rfcomm_remove_l2cap_inst(l2cap_inst, 1);
        }
    default:
      break;
    }
  return 0;
}

/*
 * prh_rfcomm_l2cap_state_conn
 * -- L2CAP Channel Handler Connected state handler
 *
 * Handles:
 *    DLC Establish Request from upper layer
 *       - instantiates a new DLC and passes the request to it.
 *    L2CAP Data Indication
 *       - Pass the data to process data function
 *    L2CAP Disconect Indication
 *       - Shutdown all DLCs on the channel
 *       - Remove the channel handler.
 *    DLC Data Request
 *       - Pass the data to be written to L2CAP with the CID.
 *    Multiplexer Close Request
 *       - Set next state to wait for release
 *       - Kills multiplexer using L2CAP Disconnect 
 *    L2CAP Configuration Ind
 *       - Send a Configuration response ( not totally correct idea ).
 */
static t_api prh_rfcomm_l2cap_state_conn(struct prh_rfcomm_l2cap_inst *l2cap_inst, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info)
{
    prh_t_rfcomm_cid cid;
    struct prh_rfcomm_dlc_inst *dlc_inst;
    int ret;
  pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"prh_rfcomm_l2cap_state_conn: event=%d\n", event));

    switch(event) {
    case PRH_RFCOMM_DLC_ESTAB_REQ:
      /* we have the L2CAP lock at this stage */
      ret=prh_rfcomm_dlc_init(l2cap_inst, info->dlci, &dlc_inst, 1);
      pMutexUnlock(l2cap_inst->lock);
      if (ret==BT_NOERROR)
	{
	  /* we now have the DLC lock */
	  prh_rfcomm_dlc_do_event(dlc_inst, PRH_RFCOMM_DLC_ESTAB_REQ, info);
	  pMutexUnlock(dlc_inst->lock);
	}
      break;
    case PRH_RFCOMM_L2CAP_DATA_IND:
      /* allowable to have multiple RFCOMM msgs in one L2CAP ?? */
      /* and the answer is no, SPEC ERRATA 1051 */
      prh_rfcomm_l2cap_process_data(l2cap_inst, info);
      break;
    case PRH_RFCOMM_L2CAP_DISC_IND:
      /* shutdown RFCOMM */
      /* lookup DLCI 0 */
      
      ret=prh_rfcomm_lookup_dlci_instance(l2cap_inst, PRH_RFCOMM_DLCI0, &dlc_inst);
      if (ret==BT_NOERROR)
	{
	  if (dlc_inst->dlc_state==PRH_RFCOMM_DLC_W4UA)
	    {
	      /* call rfstart.conf with negative */
	      if(l2cap_inst->cb.rfstart_conf)
		(*l2cap_inst->cb.rfstart_conf)(l2cap_inst->bd_addr, NULL, PRH_RFCOMM_REFUSE, l2cap_inst);
	    }
	}
      /* Need to callback all DLC callbacks */
      prh_rfcomm_remove_l2cap_inst(l2cap_inst, 1);
      break;
    case PRH_RFCOMM_RFCLOSE_REQ:
      /* Can send DISC on 0 or can just kill L2CAP connection */
      cid=l2cap_inst->cid;
      prh_rfcomm_l2cap_next_state(l2cap_inst, PRH_RFCOMM_L2CAP_W4RL);
      /* According to Specificiation we can either shutdown DLCI 0 and
     wait for a response or we can disconnect the L2CAP channel direct */
 //     L2_Disconnect(l2cap_inst->cid);
	  if(wait_dis_dlc0_timer)
	  	{
			pTimerCancel(wait_dis_dlc0_timer);
			wait_dis_dlc0_timer=0;
		}
      wait_dis_dlc0_timer=pTimerCreate(TIMER_wait_dis_dlc0_T2, prh_rfcomm_l2cap_wait_dis_dlc0_timeout, (void *)l2cap_inst, pTIMER_ONESHOT);

      break;
    default:
    break;
    }
    return BT_NOERROR;
}

/* 
 * prh_rfcomm_l2cap_state_w4rl
 * -- L2CAP Channel Handler wait for release state handler
 * 
 * Handles:
 *    L2CAP Disconnect Confirm
 *       - Remove all DLCs and remove L2CAP instance
 */ 
static t_api prh_rfcomm_l2cap_state_w4rl(struct prh_rfcomm_l2cap_inst *l2cap_inst, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info)
{
  pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"prh_rfcomm_l2cap_state_w4rl: event=%d\n", event));
  switch(event)
    {
    case PRH_RFCOMM_L2CAP_DISC_CFM:
    case PRH_RFCOMM_L2CAP_DISC_IND:
      prh_rfcomm_l2cap_next_state(l2cap_inst, PRH_RFCOMM_L2CAP_IDLE);
      prh_rfcomm_remove_l2cap_inst(l2cap_inst, 0);
      break;
    case PRH_RFCOMM_L2CAP_DATA_IND:
      /* allowable to have multiple RFCOMM msgs in one L2CAP ?? */
      /* and the answer is no, SPEC ERRATA 1051 */
      prh_rfcomm_l2cap_process_data(l2cap_inst, info);
      break;
    default:
      break;
    }

  return BT_NOERROR;
}



/*
 * prh_rfcomm_ack_timeout_cb
 * -- Timeout Callback for PFBIT timer
 */
void prh_rfcomm_l2cap_wait_dis_dlc0_timeout(void *param)
{
  struct prh_rfcomm_l2cap_inst *l2cap_inst=(struct prh_rfcomm_l2cap_inst *)param;
  L2_Disconnect(l2cap_inst->cid);
  wait_dis_dlc0_timer=0;
}


/*
 * prh_rfcomm_l2cap_process_data
 * -- Process data received on an L2CAP channel
 *
 * Verify the packet is correct using the FCS,
 * extract the DLCI from the packet, and check it exists,
 *  if the DLC exists send the data to it,
 * if the DLC doesn't exist,
 *  check is the packet an SABM packet, if so instantiate a new DLC,
 *  and dispatch the packet to it.
 * otherwise return an error for now
 */
int prh_rfcomm_l2cap_process_data(struct prh_rfcomm_l2cap_inst *l2cap_inst, struct prh_rfcomm_event_info *info)
{
    /* decide which DLCI state machine the packet is for */
    int ret=0;
    prh_t_rfcomm_dlci dlci=0;
    struct prh_rfcomm_dlc_inst *dlc_inst;
    struct host_buf *tx_buf;

    /* verify data packet using FCS */
    if ((info->buffer->data[PRH_RFCOMM_FR_CTRL_FIELD] & ~PRH_RFCOMM_FR_CTRL_PF_BIT)==PRH_RFCOMM_FR_CTRL_UIH)
        ret=prh_rfcomm_rx_fcs(info->buffer, 2);
    else
        ret=prh_rfcomm_rx_fcs(info->buffer, 3);
     pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"prh_rfcomm_l2cap_process_data\n"));

    if (ret==0)
    {
        /* Extract DLCI from packet */
        dlci=(info->buffer->data[PRH_RFCOMM_FR_ADDR_FIELD]&0xFC) >> 2;

        ret=prh_rfcomm_lookup_dlci_instance(l2cap_inst, dlci, &dlc_inst);
        if (ret==BT_NOERROR)
        {
            /* Got an instance to send data packet to */
            prh_rfcomm_l2cap_dispatch_data(dlc_inst, info);
        }
        else
        {
            /* Check is the packet an SABM packet with p bit = 1 */
            if (info->buffer->data[PRH_RFCOMM_FR_CTRL_FIELD]==(PRH_RFCOMM_FR_CTRL_SABM | PRH_RFCOMM_FR_CTRL_PF_BIT))
            {
                /* If it is, instantiate a new DLCI instance */
                ret=prh_rfcomm_dlc_init(l2cap_inst, dlci, &dlc_inst, 0);
                if (ret==BT_NOERROR)
                {
                    /* start it and then send packet to it */
                    prh_rfcomm_l2cap_dispatch_data(dlc_inst, info);
                }
                else
                    return 0;
            }
            else if (info->buffer->data[PRH_RFCOMM_FR_CTRL_FIELD]==(PRH_RFCOMM_FR_CTRL_DISC | PRH_RFCOMM_FR_CTRL_PF_BIT))
            {
                host_buf_free(info->buffer);
                /* send back a DM */
                ret=PRH_RFCOMM_GET_L2_FRAME(l2cap_inst->cid, PRH_RFCOMM_FR_BUFSIZE, &tx_buf);
                if (ret!=BT_NOERROR)
                    return ret;
                
                prh_rfcomm_create_dm(tx_buf, dlci, PRH_RFCOMM_PF_SET, !(l2cap_inst->initiating));
                L2_Write(l2cap_inst->cid, (t_DataBuf *)tx_buf);
            }
             /* If we got a DM FRAME then maybe a control channel sent a PN/RPN  and we are getting a DM response */
            else if (info->buffer->data[PRH_RFCOMM_FR_CTRL_FIELD]==(PRH_RFCOMM_FR_CTRL_DM | PRH_RFCOMM_FR_CTRL_PF_BIT))
            {
        #if pDEBUG
		  pDebugPrintfEX((pLOGNOTICE,pLOGRFCOMM, "RFCOMM: DM Response received\n"));
        #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
                 SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM,(LOG_RFC_RFCOMM_L2AUTO_STATE_INDEX+1),NULL,0,0);
        #endif
                ret=prh_rfcomm_lookup_dlci_instance(l2cap_inst, PRH_RFCOMM_DLCI0, &dlc_inst);
                if (ret==BT_NOERROR)
                {
                    prh_rfcomm_cc_cancel_timer(dlc_inst);
                }
                host_buf_free(info->buffer);
            }
        }
    }
    else
    {
        host_buf_free(info->buffer);
        return -1;
    }

    return 0;
}

/*
 * prh_rfcomm_l2cap_dispatch_data
 * -- Dispatch the packet as an event to the DLC state machine.
 * 
 * Extract the packet type from the packet and send an event of that 
 * type to the DLC state machine.
 */
int prh_rfcomm_l2cap_dispatch_data(struct prh_rfcomm_dlc_inst *instance, struct prh_rfcomm_event_info *info)
{
  int type;

   #if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
  u_int8 temp[2];
  #endif
  
  type=info->buffer->data[PRH_RFCOMM_FR_CTRL_FIELD] & ~PRH_RFCOMM_FR_CTRL_PF_BIT;

  if (type!=PRH_RFCOMM_FR_CTRL_UIH)
    host_buf_free(info->buffer);

  pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"prh_rfcomm_l2cap_dispatch_data %d\n",type));
  /* check whether data is an SABM,DISC, UA, DM etc... */
  switch(type)
    {
    case PRH_RFCOMM_FR_CTRL_SABM:
      prh_rfcomm_dlc_do_event(instance, PRH_RFCOMM_DLC_SABM, info);
      break;
    case PRH_RFCOMM_FR_CTRL_UA:
      /* Unnumbered acknowledgement */
      prh_rfcomm_dlc_do_event(instance, PRH_RFCOMM_DLC_UA, info);
      break;
    case PRH_RFCOMM_FR_CTRL_DM:
      prh_rfcomm_dlc_do_event(instance, PRH_RFCOMM_DLC_DM, info);
      break;
    case PRH_RFCOMM_FR_CTRL_DISC: 
      prh_rfcomm_dlc_do_event(instance, PRH_RFCOMM_DLC_DISC, info);
      break;
    case PRH_RFCOMM_FR_CTRL_UIH:
      prh_rfcomm_dlc_do_event(instance, PRH_RFCOMM_DLC_UIH, info);
      break;
    default:
#if pDEBUG
      pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"RFCOMM - Rfcomm_L2Cap Dispatch data type: %X", type));
      pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"RFCOMM - Rfcomm_L2Cap Dispatch data other_info: %02X %02X", info->buffer->data[0], info->buffer->data[1]));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
      temp[0]=type;
      SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM,(LOG_RFC_RFCOMM_L2AUTO_STATE_INDEX+2),temp,1,BT_CODE_BYTE);
      temp[0]= info->buffer->data[0];
      temp[1]= info->buffer->data[1];
      SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM,(LOG_RFC_RFCOMM_L2AUTO_STATE_INDEX+3),temp,2,(BT_CODE_BYTE | (BT_CODE_BYTE<<4)));
#endif
      /* Unknown data type .. uggh */
      break;
    }
  return 0;
}

