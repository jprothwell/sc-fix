/***********************************************************************
 *
 * MODULE NAME:    rfcomm_dlc_state.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    RFCOMM DLC Handler State Machine
 * MAINTAINER:     Dave Airlie
 * CREATION DATE:  02 February 2000
 *
 * SOURCE CONTROL: $Id: rfcomm_dlc_state.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies Inc.
 *     All rights reserved.
 *
 ***********************************************************************/
#include "rfcomm_lower_layer.h"
#include "rfcomm_debug.h"
#include "rfcomm_ue_api.h"
#include "rfcomm_core.h"
#include "rfcomm_frame.h"

#include "rfcomm_proto.h"

#if PRH_RFCOMM_SM2==1
#include "mgr_secdb.h"
void prh_rfcomm_dlc_incoming_sec_callback(u_int32 cbHandle, t_api status);
#endif

void prh_rfcomm_ack_timeout_cb(void *param);


static t_api prh_rfcomm_dlc_state_pn(struct prh_rfcomm_dlc_inst *self, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info);
static t_api prh_rfcomm_dlc_state_closed(struct prh_rfcomm_dlc_inst *self, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info);
static t_api prh_rfcomm_dlc_state_w4ua(struct prh_rfcomm_dlc_inst *self, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info);
static t_api prh_rfcomm_dlc_state_w4pe(struct prh_rfcomm_dlc_inst *self, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info);
static t_api prh_rfcomm_dlc_state_conn(struct prh_rfcomm_dlc_inst *self, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info);
static t_api prh_rfcomm_dlc_state_w4rl(struct prh_rfcomm_dlc_inst *self, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info);


void prh_rfcomm_dlc_next_state(struct prh_rfcomm_dlc_inst *self, PRH_RFCOMM_DLC_STATE state);


/* DLC State machine, state vector table */
static t_api (*dlc_state[])(struct prh_rfcomm_dlc_inst *self, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info) = 
{
    prh_rfcomm_dlc_state_pn,
    prh_rfcomm_dlc_state_closed,
    prh_rfcomm_dlc_state_w4ua,
    prh_rfcomm_dlc_state_w4pe,
    prh_rfcomm_dlc_state_conn,
    prh_rfcomm_dlc_state_w4rl,
};



/* 
 * prh_rfcomm_dlc_inst
 * -- Initialise DLC structure and register it
 *
 * Allocates a new DLC structure and registers it on the L2CAP Channel
 * and initialises it to idle. 
 * this creates the DLC locked.
 * 
 * prereq: MUST enter this function with the l2cap_inst lock.
 * postreq: the DLC is locked
 */
#if COMBINED_HOST==1
struct prh_rfcomm_dlc_inst self_inst[4];  //zhu jianguo changed it. 2008.11.21 
u_int8 inst_seq=0;//sisi
#endif
t_api prh_rfcomm_dlc_init(struct prh_rfcomm_l2cap_inst *l2cap_inst, prh_t_rfcomm_dlci dlci, struct prh_rfcomm_dlc_inst **instance, u_int8 locked)
{
  struct prh_rfcomm_dlc_inst *self=pNULL;
  int ret;
#if COMBINED_HOST==1
    //static u_int8 inst_seq=0;
    self = &self_inst[inst_seq];  //zhu jianguo changed it. 2008.11.21 
    if (self==pNULL)
        return BT_NORESOURCES;

     //zhu jianguo changed it. 2008.11.21 
    inst_seq++;
    if(inst_seq>4)
        return BT_NORESOURCES;
    //end of added by zhu jianguo. 2008.11.21

#else

  self=(struct prh_rfcomm_dlc_inst *)pMalloc(sizeof(struct prh_rfcomm_dlc_inst));

  if (self==pNULL)
    return BT_NORESOURCES;
#endif  
  pMemset(self, 0, sizeof(struct prh_rfcomm_dlc_inst));
  self->dlci=dlci;
  
  self->lock=pMutexCreate(locked);

  self->credit_lock=pMutexCreate(FALSE);

  /* 
   * Send a PN if we are establishing the DLC and it is not DLCI 0 
   * we are establishing if initiating & bit 0 of DLCI are same 
   */
  if (dlci==0 || ((dlci&1) == l2cap_inst->initiating))
    prh_rfcomm_dlc_next_state(self, PRH_RFCOMM_DLC_CLOSED);
  else
    prh_rfcomm_dlc_next_state(self, PRH_RFCOMM_DLC_SENDPN);  

  *instance=self;

  /* register with RFCOMM/L2CAP state machine */
  ret=prh_rfcomm_reg_dlc_instance(l2cap_inst, dlci, self);
  if (ret==BT_NORESOURCES)
    {
      pMutexFree(self->credit_lock);
      pMutexFree(self->lock);
      pFree(self);
      return BT_NORESOURCES;
    }
  return BT_NOERROR;
}

/*
 * prh_rfcomm_dlc_do_event
 * -- Dispatches incoming DLC event to function handler for current DLC state.
 */
int prh_rfcomm_dlc_do_event(struct prh_rfcomm_dlc_inst *instance, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info)
{
  return(*dlc_state[instance->dlc_state])(instance, event, info);
}

/*
 * prh_rfcomm_dlc_next_state
 * -- Sets the DLC state 
 */
void prh_rfcomm_dlc_next_state(struct prh_rfcomm_dlc_inst *self, PRH_RFCOMM_DLC_STATE state)
{
  self->dlc_state=state;
}

/*
 * prh_rfcomm_dlc_pn
 *
 * -- 
 */
static t_api prh_rfcomm_dlc_state_pn(struct prh_rfcomm_dlc_inst *self,  PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info)
{
  prh_t_rfcomm_dlc_params *dlc_params_s=NULL;
    pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"prh_rfcomm_dlc_state_pn: event=%d\n", event));
  switch (event)
    {
    case PRH_RFCOMM_DLC_ESTAB_REQ:
      dlc_params_s=info->priv;
      prh_rfcomm_dlc_next_state(self, PRH_RFCOMM_DLC_CLOSED);
      
      if (info->cb_valid)
	self->cb=info->cb.dlc;
      
      /* check have we requested Credit Flow Control or if is on already */
      if (self->rfcomm_l2cap_ptr->credit_state==CREDIT_REQUESTED || self->rfcomm_l2cap_ptr->credit_state==CREDIT_ON)
	{
	  self->cur_local_credits=dlc_params_s->local_credits;
	  __prh_rfcomm_ue_Parneg_Credit_Req(self->dlci, dlc_params_s, dlc_params_s->local_credits, self->rfcomm_l2cap_ptr, 0);
	}
      else 
	__prh_rfcomm_ue_Parneg_Req(self->dlci, dlc_params_s, self->rfcomm_l2cap_ptr, 0);
      break;
    default:
      break;
    }
  return BT_NOERROR;
}


#if PRH_RFCOMM_SM2==1
void prh_rfcomm_incoming_servicelevel_sec_callback(u_int32 cbHandle, t_api status) 
{
    ((struct prh_rfcomm_dlc_inst *)cbHandle)->outgoing_sec_state=status;
    prh_rfcomm_dlc_do_event((struct prh_rfcomm_dlc_inst *)cbHandle,PRH_RFCOMM_DLC_ESTAB_SEC,&(((struct prh_rfcomm_dlc_inst *)cbHandle)->outgoing_sec_info));
}
#endif

/* 
 * prh_rfcomm_dlc_state_closed
 * -- DLC state machine idle state handler
 *
 * Handles events for DLCs in the idle state.
 *
 * DLC Establish Req from Port Entity
 *  - Check is it Control Channel set dbit in L2CAP channel info.
 *  - register DLC callbacks passed in from entity, 
 *  - Create an SABM, set the timer, set next state to W4UA and 
 *  - transmit the frame
 *
 * SABM from remote RFCOMM entity
 *  - Check is it Control Channel and set the dbit if it is.
 *  - set the next state to W4PE 
 *  - Send an indication to the port entity 
 *        - rfstart ind for a DLCI0
             - if no rfstart registered send a DM on DLCI 0.
 *        - dlc_estab_ind for a numbered DLC
 */
static t_api prh_rfcomm_dlc_state_closed(struct prh_rfcomm_dlc_inst *self, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info)
{
    struct host_buf *tx_buf;
    prh_t_rfcomm_dlc_params *dlc_params=NULL;
    struct prh_rfcomm_dlc_inst *dlc0_inst;
    struct prh_rfcomm_l2cap_inst *l2cap_ptr;
    int ret;

#if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))

u_int8 temp;

#endif

    t_api (*dlc_estab_conf)(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci_b, prh_t_rfcomm_dlc_params *dlc_params_b, prh_t_rfcomm_accept accept, void *multi_inst);
    prh_t_rfcomm_dlci dlci;
    struct prh_rfcomm_event_info ev_info;  
#if PRH_RFCOMM_SM2==1
    t_api security_status;
#endif
    /* Just inited */
    /* Can either be asked to establish the DLC by sending an SABM or
    can receive and SABM from peer entity. */
    pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"prh_rfcomm_dlc_state_closed: event=%d\n", event));
    switch (event) 
    {
        case PRH_RFCOMM_DLC_PN:
        case PRH_RFCOMM_DLC_ESTAB_REQ:

#if PRH_RFCOMM_SM2==1
        self->outgoing_sec_state=MGR_MultiplexOutgoingAccessRequest(RFCOMM_PROTOCOLID, self->dlci>>1, self->rfcomm_l2cap_ptr->bd_addr, (u_int32)self, prh_rfcomm_incoming_servicelevel_sec_callback); 
        if (self->outgoing_sec_state==BT_PENDING) 
        {
            /* we must deep-copy info.priv so that it is available in the security callback */
            pMemcpy(&self->outgoing_sec_info,info,sizeof(info));
            pMemcpy(&self->outgoing_sec_params,(prh_t_rfcomm_dlc_params*)info->priv,sizeof(prh_t_rfcomm_dlc_params));
            self->outgoing_sec_info.priv=(void *)&self->outgoing_sec_params;
            /* remain in the same state and wait for the security callback */
            break;
        }
        /* NB: FALLTHROUGH */

        case PRH_RFCOMM_DLC_ESTAB_SEC:
        if (self->outgoing_sec_state!=BT_NOERROR) 
        {
            // security failure
            if (self->dlci >  PRH_RFCOMM_DLCI0)
            {
                l2cap_ptr=self->rfcomm_l2cap_ptr;
                dlc_estab_conf=self->cb.dlc_estab_conf;
                dlci=self->dlci;
                pMutexLock(self->lock);
                ret=prh_rfcomm_remove_dlc_instance(self->rfcomm_l2cap_ptr, self->dlci, NULL);
                if (ret==BT_NOERROR)
                {
                    prh_rfcomm_free_dlc_instance(self);
                    if (l2cap_ptr->dlc_count==1)
                    {
                        /* kill the L2CAP instance */
                        prh_rfcomm_l2cap_do_event(l2cap_ptr, PRH_RFCOMM_RFCLOSE_REQ, &ev_info);
                    }
                }
                /* report an error to the user */
                if (dlc_estab_conf)
                    (*dlc_estab_conf)(l2cap_ptr->bd_addr, dlci, NULL, PRH_RFCOMM_REFUSE, l2cap_ptr);
            }
            return self->outgoing_sec_state;
        } 
#endif

        /* Need to send SABM */
        /* 
        * If we are establishing DLCI 0 we are the initiating side 
        * so set the flag in the multiplexer container 
        */
        if (self->dlci==PRH_RFCOMM_DLCI0)
            self->rfcomm_l2cap_ptr->initiating=1;

        if (info->cb_valid)
        {
            self->cb=info->cb.dlc;
        }

        dlc_params=(prh_t_rfcomm_dlc_params *)info->priv;
        if (dlc_params)
        {
            self->dlc_params.max_frame_size=dlc_params->max_frame_size;
            self->dlc_params.peer_credits=dlc_params->peer_credits;
            self->cur_peer_credits=dlc_params->peer_credits;
        }

        ret=PRH_RFCOMM_GET_L2_FRAME(self->rfcomm_l2cap_ptr->cid, PRH_RFCOMM_FR_BUFSIZE, &tx_buf);
        if (ret!=BT_NOERROR)
            return ret;

        /* SABM - frame, dlci, pbit, cresp bit */
        /* SABM is a command frame so CR bit is same as direction */
        prh_rfcomm_create_sabm(tx_buf, self->dlci, PRH_RFCOMM_PF_SET, self->rfcomm_l2cap_ptr->initiating);
        /* Start timer for SABM transmit - pfbit timer */
	if (self->pfbit_timer !=0)
	{
      		pDebugPrintfEX((pLOGERROR,pLOGRFCOMM,"prh_rfcomm_dlc_state_closed: self->pfbit_timer=%d\n", self->pfbit_timer));
		pTimerCancel(self->pfbit_timer);
		self->pfbit_timer=0;
	}
      self->pfbit_timer=pTimerCreate(TIMER_ACK_T1, prh_rfcomm_ack_timeout_cb, (void *)self, pTIMER_ONESHOT);
        /* Need to do this */
        prh_rfcomm_dlc_next_state(self, PRH_RFCOMM_DLC_W4UA);
        L2_Write(self->rfcomm_l2cap_ptr->cid, (t_DataBuf *)tx_buf);

        // if (event==PRH_RFCOMM_DLC_ESTAB_SEC) {
        //if (self->cb.dlc_paramneg)
        //   (*self->cb.dlc_paramneg)(dlci, (prh_t_rfcomm_dlc_params)info->priv), self, cresp>>1);
        //  }
        break;

        case PRH_RFCOMM_DLC_SABM:
        /* Got an SABM */
        /* The DLCI is locked upon entering here */
        /* 
        * If this is DLCI 0, Set Multiplexer to responding device 
        */
        if (self->dlci==PRH_RFCOMM_DLCI0)
            self->rfcomm_l2cap_ptr->initiating=0;
        
        prh_rfcomm_dlc_next_state(self, PRH_RFCOMM_DLC_W4PE);
        /* Send a RFCOMM RFSTART.ind of DLC_ESTAB.ind upwards */
#if pDEBUG
      pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"prh_rfcomm_dlc_state_closed: self->dlci=%d\n", self->dlci));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
      temp=self->dlci;
      SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM, LOG_RFC_RFCOMM_DLC_STATE_INDEX, &temp,1,BT_CODE_BYTE); 
      #endif
        if (self->dlci==PRH_RFCOMM_DLCI0)
        {
            if (self->rfcomm_l2cap_ptr->rfcomm_ptr->rfstart_ind_cb)
                self->rfcomm_l2cap_ptr->rfcomm_ptr->rfstart_ind_cb(self->rfcomm_l2cap_ptr->bd_addr, NULL, PRH_RFCOMM_ACCEPT, self->rfcomm_l2cap_ptr);
            else
                goto send_dm;
        }
        else
        {
            if (prh_rfcomm_check_channel_state((u_int8)(self->dlci>>1)))
            {
                struct prh_rfcomm_server_inst *serv_inst;

                ret=prh_rfcomm_lookup_server_inst_by_schan((u_int8)(self->dlci>>1), &serv_inst);
                if (ret==BT_NOERROR)
                {
#if PRH_RFCOMM_SM2==1
                    /* do security here? */
                    security_status = MGR_IncomingAccessRequest(RFCOMM_PROTOCOLID, self->dlci>>1, self->rfcomm_l2cap_ptr->bd_addr, (u_int32)self, prh_rfcomm_dlc_incoming_sec_callback);
                    if (BT_NOERROR == security_status)
                    {
                        /* args to callback are 
                        a) bd_addr, b) dlci, c) mode, d)sys params, e) accept, f)void ptr
                        */
                        if (serv_inst->dlc_cbs.dlc_estab_ind)
                            serv_inst->dlc_cbs.dlc_estab_ind(self->rfcomm_l2cap_ptr->bd_addr, self->dlci, &self->dlc_params, self->rfcomm_l2cap_ptr);
                        //   pMutexUnlock(self->lock);
                    }
                    else if (BT_PENDING!=security_status)
                        goto send_dm;
#else
                    if (serv_inst->dlc_cbs.dlc_estab_ind)
                        serv_inst->dlc_cbs.dlc_estab_ind(self->rfcomm_l2cap_ptr->bd_addr, self->dlci, &self->dlc_params, self->rfcomm_l2cap_ptr);
#endif
                }
                else
                    goto send_dm;
            }
            else
                goto send_dm;

        }
        break;
        
        case PRH_RFCOMM_DLC_UIH:
            host_buf_free(info->buffer);
        break;
        
        /* these are the same if we get a DLC from the remote or the 
        L2CAP connection goes down hard... need to return CONNECTFAIL */
        /* For a DM, we need to do all the usual stuff for a connection down,
        For a CLOSE_NOW we just need to indicate the upper layer and clean
        up the DLC no L2CAP cleanups  or DLCI 0 checks. */
        case PRH_RFCOMM_DLC_DM:
        /* it is theoretically impossible to get a DM for DLCI 0 at this stage
        due to the fact that we can't do a PN exchange for DLCI 0 so there
        is no way to enter here for DLCI 0, DM for DLCI 0 is handled at W4UA
        stage */
        if (event==PRH_RFCOMM_DLC_DM)
	{
#if pDEBUG
	  pDebugPrintfEX((pLOGNOTICE,pLOGRFCOMM, "RFCOMM: DM Response received\n"));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
         SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM, (LOG_RFC_RFCOMM_DLC_STATE_INDEX+1), NULL,0,0); 
#endif
	  /* Find DLC0 and cancel timer */
            ret=prh_rfcomm_lookup_dlci_instance(self->rfcomm_l2cap_ptr, PRH_RFCOMM_DLCI0, &dlc0_inst);
            if (ret==BT_NOERROR)
            {
                prh_rfcomm_cc_cancel_timer(dlc0_inst);
            }
        }
        
        case PRH_RFCOMM_DLC_CLOSE_NOW:

        if (self->dlci > PRH_RFCOMM_DLCI0)
        {
            l2cap_ptr=self->rfcomm_l2cap_ptr;
            dlc_estab_conf=self->cb.dlc_estab_conf;
            dlci=self->dlci;

            /* only do this for a DM reply not a CLOSE NOW */
            if (event == PRH_RFCOMM_DLC_DM)
            {
                pMutexLock(self->lock);
                ret=prh_rfcomm_remove_dlc_instance(self->rfcomm_l2cap_ptr, self->dlci, NULL);
                if (ret==BT_NOERROR)
                {
                    prh_rfcomm_free_dlc_instance(self);
                    /* if there is a single DLC left shut down the L2CAP */
                    if (l2cap_ptr->dlc_count==1)
                    {
                        /* kill the L2CAP instance */
                        prh_rfcomm_l2cap_do_event(l2cap_ptr, PRH_RFCOMM_RFCLOSE_REQ, &ev_info);
                    }
                }
            }

            /* report an error to the user */
            if (dlc_estab_conf)
                (*dlc_estab_conf)(l2cap_ptr->bd_addr, dlci, NULL, PRH_RFCOMM_REFUSE, l2cap_ptr);
        }

        default:
            break;
    }
    return BT_NOERROR;

 send_dm:
#if pDEBUG
  pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM, "RFCOMM: Sending DM frame channel closed %d\n", self->dlci));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
  SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM, (LOG_RFC_RFCOMM_DLC_STATE_INDEX+2), &temp,1,BT_CODE_BYTE); 
#endif

    ret=PRH_RFCOMM_GET_L2_FRAME(self->rfcomm_l2cap_ptr->cid, PRH_RFCOMM_FR_BUFSIZE, &tx_buf);
    if (ret!=BT_NOERROR)
        return ret;

    /* DM is a response frame so CR bit is opposite to direction */
    prh_rfcomm_create_dm(tx_buf, self->dlci, PRH_RFCOMM_PF_SET, !(self->rfcomm_l2cap_ptr->initiating));
    L2_Write(self->rfcomm_l2cap_ptr->cid, (t_DataBuf *)tx_buf);
    pMutexLock(self->lock);
    prh_rfcomm_remove_dlc_instance(self->rfcomm_l2cap_ptr, self->dlci, NULL);
    prh_rfcomm_free_dlc_instance(self);
    return BT_NOERROR;
}

/*
 * prh_rfcomm_dlc_state_w4ua
 * -- DLC state machine wait for UA state handler
 * 
 * Handles:
 *    UA frame:
 *       - If a UA arrives, cancel any pending timers, 
 *       - set the DLC state to connected and callback the upper layer.
 *       - if the DLCI is 0 callback with rfstart rather than DLC establish.
 *    DM frame:
 *       - Cancel the timer and act like a timeout.
 *    Timeout:
 *       - If a DM frames arrives or nothing is gotten within T1
 *       - shutdown the multiplexer and callback the upper layer.
 */
static t_api prh_rfcomm_dlc_state_w4ua(struct prh_rfcomm_dlc_inst *self, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info)
{
  int ret;
  struct prh_rfcomm_event_info ev_info;
  struct prh_rfcomm_l2cap_inst *l2cap_ptr;
  #if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
  u_int8 temp;
  #endif
  prh_t_rfcomm_dlci dlci=self->dlci;
  t_api (*dlc_estab_conf)(t_bdaddr bd_addr, prh_t_rfcomm_dlci dlci_b, prh_t_rfcomm_dlc_params *dlc_params_b, prh_t_rfcomm_accept accept, void *multi_inst);
    pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"prh_rfcomm_dlc_state_w4ua: event=%d\n", event));
  switch (event) 
    {
    case PRH_RFCOMM_DLC_UA:
      /* Send PRH_RFCOMM_DLC_ESTAB_CONF upwards */
      /* DLCI 0 also uses the same callback */
      /* Got back a response */
    if(self->pfbit_timer)
      pTimerCancel(self->pfbit_timer);
      self->pfbit_timer=0;
#if pDEBUG
      pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"RFCOMM: rx: UA: dlci:%d\n",self->dlci));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
      temp=self->dlci;   
      SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM,(LOG_RFC_RFCOMM_DLC_STATE_INDEX+3), &temp,1,BT_CODE_BYTE);
#endif
      prh_rfcomm_dlc_next_state(self, PRH_RFCOMM_DLC_CONN);
      if (self->dlci==PRH_RFCOMM_DLCI0)
	{
	  if(self->rfcomm_l2cap_ptr->cb.rfstart_conf)
	    (*self->rfcomm_l2cap_ptr->cb.rfstart_conf)(self->rfcomm_l2cap_ptr->bd_addr, NULL, PRH_RFCOMM_ACCEPT, self->rfcomm_l2cap_ptr);
	}
      else
	{
	  /* Is there a callback for this DLC Establish */
	  if (self->cb.dlc_estab_conf)
	    {
        #if pDEBUG
	      pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"RFCOMM: Calling DLC_ESTAB_CONF %d\n", self->dlci));
        #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
             temp=self->resp_count;      
             SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM, (LOG_RFC_RFCOMM_DLC_STATE_INDEX+4), &temp,1,BT_CODE_BYTE); 
        #endif
	      (*self->cb.dlc_estab_conf)(self->rfcomm_l2cap_ptr->bd_addr, self->dlci, &self->dlc_params, PRH_RFCOMM_ACCEPT, self->rfcomm_l2cap_ptr);
	    }
	}
      
      break;
    case PRH_RFCOMM_DLC_DM:
	if(self->pfbit_timer)
      pTimerCancel(self->pfbit_timer);
      self->pfbit_timer=0;
#if pDEBUG
      pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"RFCOMM: rx: DM: dlci:%d\n", self->dlci));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
      temp=self->dlci;   
      SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM,(LOG_RFC_RFCOMM_DLC_STATE_INDEX+5), &temp,1,BT_CODE_BYTE);
#endif
      
      /* Fall through deliberate... we do the same thing on a timeout as if we received a DM response from the peer */
    case PRH_RFCOMM_TIMEOUT:
      /* TBC: if timeout fail the whole bloody lot.. not sure if spec compliant  */
      if (dlci==PRH_RFCOMM_DLCI0)
	{
	  /* If we have multiplexer failure reset the L2CAP connection */
	  l2cap_ptr=self->rfcomm_l2cap_ptr;
	  pMutexLock(self->lock);
	  ret=prh_rfcomm_remove_dlc_instance(self->rfcomm_l2cap_ptr, self->dlci, NULL);
	  prh_rfcomm_free_dlc_instance(self);
	  prh_rfcomm_l2cap_do_event(l2cap_ptr, PRH_RFCOMM_RFCLOSE_REQ, NULL);
	  /* Always do the callback last to avoid any problems */
	  if(l2cap_ptr->cb.rfstart_conf)
	    (*l2cap_ptr->cb.rfstart_conf)(l2cap_ptr->bd_addr, NULL, PRH_RFCOMM_REFUSE, l2cap_ptr); 
	  
	}
      /* deliberate fall through */
    case PRH_RFCOMM_DLC_CLOSE_NOW: 
      if (dlci >  PRH_RFCOMM_DLCI0)
	{
	  l2cap_ptr=self->rfcomm_l2cap_ptr;
	  dlc_estab_conf=self->cb.dlc_estab_conf;
	  dlci=self->dlci;
	  if (event != PRH_RFCOMM_DLC_CLOSE_NOW)
	    {
	      pMutexLock(self->lock);
	      ret=prh_rfcomm_remove_dlc_instance(self->rfcomm_l2cap_ptr, self->dlci, NULL);
	      if (ret==BT_NOERROR)
		{
		  prh_rfcomm_free_dlc_instance(self);
		  if (l2cap_ptr->dlc_count==1)
		    {
		      /* kill the L2CAP instance */
		      prh_rfcomm_l2cap_do_event(l2cap_ptr, PRH_RFCOMM_RFCLOSE_REQ, &ev_info);
		    }
		}
	    }
	  /* report an error to the user */
	  if (dlc_estab_conf)
	    (*dlc_estab_conf)(l2cap_ptr->bd_addr, dlci, NULL, PRH_RFCOMM_REFUSE, l2cap_ptr);
	}
      break;
    case PRH_RFCOMM_DLC_UIH:
      host_buf_free(info->buffer);
      break;
    case PRH_RFCOMM_DLC_RELEASE_REQ:
      {
	     struct host_buf *tx_buf;

            /* Send a DISC request to remote side, shutdown DLC */
            ret=PRH_RFCOMM_GET_L2_FRAME(self->rfcomm_l2cap_ptr->cid, PRH_RFCOMM_FR_BUFSIZE, &tx_buf);
            if (ret!=BT_NOERROR)
                return ret;

            /* DISC is a command frame so CR bit is set to direction */
            prh_rfcomm_create_disc(tx_buf, self->dlci, PRH_RFCOMM_PF_SET, self->rfcomm_l2cap_ptr->initiating);
            /* Start timer for DISC */
            /* Timer T1 needs to be set to 60 seconds */
	    	if (self->pfbit_timer !=0)
		{
      			pDebugPrintfEX((pLOGERROR,pLOGRFCOMM,"prh_rfcomm_dlc_state_w4ua: self->pfbit_timer=%d\n", self->pfbit_timer));
			pTimerCancel(self->pfbit_timer);
			self->pfbit_timer=0;
		}
		
	    self->pfbit_timer=pTimerCreate(TIMER_ACK_T1, prh_rfcomm_ack_timeout_cb, (void *)self, pTIMER_ONESHOT);
            prh_rfcomm_dlc_next_state(self, PRH_RFCOMM_DLC_W4RL);
            L2_Write(self->rfcomm_l2cap_ptr->cid, (t_DataBuf *)tx_buf);
        break;
    	}
//      return BT_INVALIDPARAM;
    default:
      break;
    }
  return BT_NOERROR;
}

/*
 * prh_rfcomm_dlc_state_w4pe
 * -- DLC state machine wait for port entity state handler
 *
 * Handles:
 *    Multiplexer Start Response (DLCI 0 Estab Resp):
 *    DLC Establish Response
 *       - Check is the DLC accepted,
 *       - if so, set the DLC to connected, 
 *       -        if non control DLCI, set up DLC callbacks,
 *       -        if DLCI0 set up control channel callbacks
 *       -        Create a UA, and transmit it.
 *       - if not accepted, send a DM frame and cleanup
 */   
static t_api prh_rfcomm_dlc_state_w4pe(struct prh_rfcomm_dlc_inst *self, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info)
{
  struct host_buf *tx_buf;
  prh_t_rfcomm_dlc_params *dlc_params;
  int ret;
  int accept;
  
  accept=info->accept;
    pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"prh_rfcomm_dlc_state_w4pe: event=%d\n", event));
  switch(event) 
    {
      /* if we get a DM from the peer here, lock and remove */
    case PRH_RFCOMM_DLC_DM:
      accept=0;
      pMutexLock(self->lock);
    case PRH_RFCOMM_RFSTART_RESP:
    case PRH_RFCOMM_DLC_ESTAB_RESP:
      /* Transmit a UA with f=1 */
      if (accept==1)
    {
      prh_rfcomm_dlc_next_state(self, PRH_RFCOMM_DLC_CONN);

      ret=PRH_RFCOMM_GET_L2_FRAME(self->rfcomm_l2cap_ptr->cid, PRH_RFCOMM_FR_BUFSIZE, &tx_buf);
      if (ret!=BT_NOERROR)
	return ret;

      /* Setup Callbacks if control channel */
      if (self->dlci==0)
        {
          self->rfcomm_l2cap_ptr->cb=info->cb.multi;
        }
      else
      	{
      	  self->cb=info->cb.dlc;
      	}

      dlc_params=(prh_t_rfcomm_dlc_params *)info->priv;
      if (dlc_params)
	{
	  self->dlc_params.local_credits=dlc_params->local_credits;
	  self->cur_local_credits=dlc_params->local_credits;
	  
	  self->dlc_params.peer_credits=dlc_params->peer_credits;
	  self->cur_peer_credits=dlc_params->peer_credits;
	}
      prh_rfcomm_create_ua(tx_buf, self->dlci, PRH_RFCOMM_PF_SET, !(self->rfcomm_l2cap_ptr->initiating));
      L2_Write(self->rfcomm_l2cap_ptr->cid, (t_DataBuf *)tx_buf);
      pMutexUnlock(self->lock);
    }
      else
    {
      if (event!=PRH_RFCOMM_DLC_DM)
	{
	  /* Send back a DM */
	  ret=PRH_RFCOMM_GET_L2_FRAME(self->rfcomm_l2cap_ptr->cid, PRH_RFCOMM_FR_BUFSIZE, &tx_buf);
	  if (ret!=BT_NOERROR)
	    return ret;
	  
	  /* DM is a response frame so CR bit is opposite to direction */
	  prh_rfcomm_create_dm(tx_buf, self->dlci, PRH_RFCOMM_PF_SET, !(self->rfcomm_l2cap_ptr->initiating));
	  L2_Write(self->rfcomm_l2cap_ptr->cid, (t_DataBuf *)tx_buf);
	}
      prh_rfcomm_remove_dlc_instance(self->rfcomm_l2cap_ptr, self->dlci, NULL);
      prh_rfcomm_free_dlc_instance(self);
    }
      break;
    case PRH_RFCOMM_DLC_UIH:
      host_buf_free(info->buffer);
      break;
    default:
      break;
    }
  return BT_NOERROR;
}

/*
 * prh_rfcomm_dlc_state_conn
 * -- DLC state machine connected state handler
 *
 * Handles:
 *    UIH frame from peer
 *       - transmit the data to the upper layers after checking length
 *    DISC frame from peer
 *       - send a UA frame and callback upper layers with disconnection ind
 *    Disconnect Request from Upper Layer
 *       - Send a DISC frame, and start timer, set state to wait for ua release
 *    Data request from upper layer
 *       - Create a UIH frame and send it.
 *    Timeout
 *       - Kill multiplexer (??)
 */
static t_api prh_rfcomm_dlc_state_conn(struct prh_rfcomm_dlc_inst *self, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info)
{
  struct host_buf *tx_buf;
  int ret;
  unsigned int len;

  #if ((pDEBUG==0) && (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED))
  u_int8 temp[4];
#endif

  
    struct prh_rfcomm_l2cap_inst *l2cap_inst;
    int send_go=0;
    u_int8 pfbit=0;
    /* When connected we can get
    *  a) A UIH packet which is data for the upper layers 
    *  b) a DISC packet which is a disconnect request from the peer
    *  c) a DLC_RELEASE_REQ from upper layers
    *  d) a DATA_IND from upper layers 
    */
    pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"prh_rfcomm_dlc_state_conn: event=%d\n", event));
    switch (event) 
    {
        case PRH_RFCOMM_DLC_UIH:
            /* need to decode length field in UIH packet */
            /* Check the EA bit of the first byte, is it set if so
            single byte else, double byte length */

            pfbit=info->buffer->data[PRH_RFCOMM_FR_CTRL_FIELD] & (unsigned char)PRH_RFCOMM_FR_CTRL_PF_BIT;
            if (info->buffer->data[PRH_RFCOMM_FR_LENGTH_FIELD1] & PRH_RFCOMM_EA_BIT)
            {
                /* length field is single byte */
                len = info->buffer->data[PRH_RFCOMM_FR_LENGTH_FIELD1] >> 1;
                host_buf_reserve_header(info->buffer, PRH_RFCOMM_BUF_UIH_SHORT);
            }
            else
            {
                /* length field is two bytes */
                len = (info->buffer->data[PRH_RFCOMM_FR_LENGTH_FIELD1] >> 1) + (info->buffer->data[PRH_RFCOMM_FR_LENGTH_FIELD2] << 7);
                host_buf_reserve_header(info->buffer, PRH_RFCOMM_BUF_UIH_LONG);
            }

            /* For Credit flow control if PF is set, there is credit info in the
            frame */
            if (pfbit)
            {
                pMutexLock(self->credit_lock);
                if (self->cur_peer_credits==0)
                    send_go=1;

                self->cur_peer_credits+=info->buffer->data[0];

                if (self->cb.credit_flow_ind)
                    (*self->cb.credit_flow_ind)(self->dlci, PRH_RFCOMM_FLOW_DATA_GO, self->rfcomm_l2cap_ptr);
                
                pMutexUnlock(self->credit_lock);
                /* This octet contains credit information */
                host_buf_reserve_header(info->buffer, 1);
            }

    #if pDEBUG
    if (info->buffer->len != len)
      {
	pDebugPrintfEX((pLOGERROR, pLOGRFCOMM,"RFCOMM: ERROR got differing length fields, %d\n", info->buffer->len));
      }
    pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"RFCOMM: rx data len: %d\n", len));

    #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    if (info->buffer->len != len)
      {
      BT_WRITE_LE_UINT32(temp, info->buffer->len);
      SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM,(LOG_RFC_RFCOMM_DLC_STATE_INDEX+6), temp,4,BT_CODE_DWORD);
      }
       BT_WRITE_LE_UINT32(temp, len);
       SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM,(LOG_RFC_RFCOMM_DLC_STATE_INDEX+7),temp,4,BT_CODE_DWORD);
     
    #endif

    
    
    if (len!=0)
      {
	/* send data upwards */
	if (self->dlci!=PRH_RFCOMM_DLCI0)
	  {
	    /* If credit based flow is enabled */
	    if (self->rfcomm_l2cap_ptr->credit_state==CREDIT_ON)
	      {
		--self->cur_local_credits;
		if (self->cur_local_credits==PRH_RFCOMM_CREDIT_MIN)
		  {
#if pDEBUG
		    pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"Local credits at minimum\n"));

#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
                  SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM,(LOG_RFC_RFCOMM_DLC_STATE_INDEX+8),NULL,0,0);
#endif
		    /* Need to tell someone peer is out of credits?? */
		    prh_rfcomm_credit_peer_nocredit(self);
		  }
	      }
	    if (self->cb.dlc_data_ind)
	      {
		(*self->cb.dlc_data_ind)(self->dlci, info->buffer, self->rfcomm_l2cap_ptr);
	      }
	    else
	      {
		/* this in theory cannot happen - but just in case free the buffer so as to avoid memory leak */
#if pDEBUG
		pDebugPrintfEX((pLOGERROR,pLOGRFCOMM,"Rx'ed data on DLCI with no handler %d\n", self->dlci));

#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
              temp[0]=self->dlci;
              SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM,(LOG_RFC_RFCOMM_DLC_STATE_INDEX+9),temp,1,BT_CODE_BYTE);

#endif
		host_buf_free(info->buffer);
	      }
	  }
	else
	  {
	    prh_rfcomm_cc_data_ind(self->dlci, info->buffer, self->rfcomm_l2cap_ptr);
                }
            }
            else
                host_buf_free(info->buffer);

        break;
        
        case PRH_RFCOMM_DLC_DISC:
            /* Send a UA and DLC_RELEASE_IND to upper layer */

            ret=PRH_RFCOMM_GET_L2_FRAME(self->rfcomm_l2cap_ptr->cid, PRH_RFCOMM_FR_BUFSIZE, &tx_buf);
            if (ret!=BT_NOERROR)
                return ret;

            /* UA is a response frame so CR bit is opposite to direction */
            prh_rfcomm_create_ua(tx_buf, self->dlci, PRH_RFCOMM_PF_SET, !(self->rfcomm_l2cap_ptr->initiating));
            L2_Write(self->rfcomm_l2cap_ptr->cid, (t_DataBuf *)tx_buf);
            /* call back to the upper layer with the release indication */
            /* fall through on purpose */
            /* handle a DM - shut down like we got a DISC but with no UA response */
        case PRH_RFCOMM_DLC_DM: 
        case PRH_RFCOMM_DLC_CLOSE_NOW:
            if(self->dlci)
            {
            	if (self->cb.dlc_rel_ind)
            	{
            	   pDebugPrintfEX((pLOGERROR,pLOGRFCOMM,"PRH_RFCOMM_DLC_CLOSE_NOW self->dlci =%d, self->cb.dlc_rel_ind=0x%x\n", self->dlci,self->cb.dlc_rel_ind));
             	   (*self->cb.dlc_rel_ind)(self->dlci, self->rfcomm_l2cap_ptr);
            	}
            }
            /* destroy the DLC */
            if (event==PRH_RFCOMM_DLC_DISC || event==PRH_RFCOMM_DLC_DM)
            {
                pMutexLock(self->lock);
                ret=prh_rfcomm_remove_dlc_instance(self->rfcomm_l2cap_ptr, self->dlci, NULL);
                prh_rfcomm_free_dlc_instance(self);
            }

            /* If remote side is disconnecting.. leave it go..
            perhaps start a timer here */    
        break; 
            
        case PRH_RFCOMM_DLC_RELEASE_REQ:
            /* Send a DISC request to remote side, shutdown DLC */
            ret=PRH_RFCOMM_GET_L2_FRAME(self->rfcomm_l2cap_ptr->cid, PRH_RFCOMM_FR_BUFSIZE, &tx_buf);
            if (ret!=BT_NOERROR)
                return ret;

            /* DISC is a command frame so CR bit is set to direction */
            prh_rfcomm_create_disc(tx_buf, self->dlci, PRH_RFCOMM_PF_SET, self->rfcomm_l2cap_ptr->initiating);
            /* Start timer for DISC */
            /* Timer T1 needs to be set to 60 seconds */
	    	if (self->pfbit_timer !=0)
		{
      			pDebugPrintfEX((pLOGERROR,pLOGRFCOMM,"prh_rfcomm_dlc_state_conn: self->pfbit_timer=%d\n", self->pfbit_timer));
			pTimerCancel(self->pfbit_timer);
			self->pfbit_timer=0;
		}
    		self->pfbit_timer=pTimerCreate(TIMER_ACK_T1, prh_rfcomm_ack_timeout_cb, (void *)self, pTIMER_ONESHOT);
            prh_rfcomm_dlc_next_state(self, PRH_RFCOMM_DLC_W4RL);
            L2_Write(self->rfcomm_l2cap_ptr->cid, (t_DataBuf *)tx_buf);
        break;
        
        case PRH_RFCOMM_DATA_REQ:
            /* Send a UIH to remote peer, cresp bit is set as per 5.2.1.2 */
            /* UIH is a data frame so CR bit is same to direction */
            if (self->dlci>0 && self->rfcomm_l2cap_ptr->credit_state==CREDIT_ON)
            {
                pMutexLock(self->credit_lock);
                if (self->cur_peer_credits==0)
                {
                    pMutexUnlock(self->credit_lock);
                    return BT_FLOWCTRL;
                }
                pMutexUnlock(self->credit_lock);
                pfbit = 1;  //zhu jianguo added it. 2008.12.5
            }

            prh_rfcomm_create_uih(info->buffer, self->dlci, self->rfcomm_l2cap_ptr->initiating, pfbit);

            pMutexLock(self->rfcomm_l2cap_ptr->lock);

            ret=L2_Write(self->rfcomm_l2cap_ptr->cid, (t_DataBuf *)info->buffer);
            if (ret==BT_FLOWCTRL)
            {
                pMutexUnlock(self->rfcomm_l2cap_ptr->lock);
                prh_rfcomm_destroy_uih(info->buffer);
                return BT_FLOWCTRL;
            }
            
            pMutexUnlock(self->rfcomm_l2cap_ptr->lock);
            if (self->dlci>0 && self->rfcomm_l2cap_ptr->credit_state==CREDIT_ON)
            {
                pMutexLock(self->credit_lock);

                --self->cur_peer_credits;
                if (self->cur_peer_credits==0)
                {
                    /* Need to notify someone we are out of credits - stop sending */
                    if (self->cb.credit_flow_ind)
                        (*self->cb.credit_flow_ind)(self->dlci, PRH_RFCOMM_FLOW_DATA_STOP, self->rfcomm_l2cap_ptr);
                }

                pMutexUnlock(self->credit_lock);
            }
        break;
        
        case PRH_RFCOMM_DLC_PN:
        /* got a PN store the info */

        break;
        
        case PRH_RFCOMM_TIMEOUT:
            /* Must be a timeout on the Control Channel at this stage */
            /* spec says shut the whole lot down if this happens */
            /* so do it ... */
            l2cap_inst=self->rfcomm_l2cap_ptr;
            pMutexLock(prh_rfcomm_core_lock);
            pMutexLock(l2cap_inst->lock);
            prh_rfcomm_remove_all_dlc_instance(l2cap_inst,1);
            pMutexUnlock(l2cap_inst->lock);
            pMutexUnlock(prh_rfcomm_core_lock);
            prh_rfcomm_l2cap_do_event(l2cap_inst, PRH_RFCOMM_RFCLOSE_REQ, info);
            /* we need to do more here.. clean up all the DLCs for the application and notify it of everything.*/
            
        default:
        break;
    }
    return BT_NOERROR;
}


/*
 * prh_rfcomm_dlc_state_w4rl
 * -- DLC State Machine Wait for release state handler
 *
 * Handles:
 *    UA from peer:
 *       - remove DLC instance from L2CAP channel handler
 *       - close multiplexer if last open DLC.
 *    Timeout:
 *       - shutdown multiplexer (??)
 */
static t_api prh_rfcomm_dlc_state_w4rl(struct prh_rfcomm_dlc_inst *self, PRH_RFCOMM_EVENT event, struct prh_rfcomm_event_info *info)
{
  int ret;
  struct prh_rfcomm_event_info ev_info;
  struct prh_rfcomm_l2cap_inst *l2cap_ptr;
    pDebugPrintfEX((pLOGDEBUG,pLOGRFCOMM,"prh_rfcomm_dlc_state_w4rl: event=%d\n", event));
  switch(event)
    {
    case PRH_RFCOMM_DLC_DM:
    case PRH_RFCOMM_DLC_UA:
	  if(self->pfbit_timer)
	  {
	      pTimerCancel(self->pfbit_timer);
    	  self->pfbit_timer = 0;
	  }
    case PRH_RFCOMM_TIMEOUT:
      l2cap_ptr=self->rfcomm_l2cap_ptr;
      pMutexLock(self->lock);
      ret=prh_rfcomm_remove_dlc_instance(self->rfcomm_l2cap_ptr, self->dlci, NULL);
      if (ret==BT_INVALIDPARAM)
	return BT_NOERROR;
      prh_rfcomm_free_dlc_instance(self);
      if (ret==BT_NOERROR)
    {
      if (l2cap_ptr->dlc_count==1)
        {
          /* kill the L2CAP instance */
          prh_rfcomm_l2cap_do_event(l2cap_ptr, PRH_RFCOMM_RFCLOSE_REQ, &ev_info);
        }
    }
      break;
    case PRH_RFCOMM_DLC_CLOSE_NOW: 
      /* do nothing if we were waiting to shutdown and got shutdown
	 well cool */
      break;
    case PRH_RFCOMM_DLC_UIH:
      host_buf_free(info->buffer);
    default:
      break;
    }
  return BT_NOERROR;
}


/*
 * prh_rfcomm_ack_timeout_cb
 * -- Timeout Callback for PFBIT timer
 */
void prh_rfcomm_ack_timeout_cb(void *param)
{
  struct prh_rfcomm_dlc_inst *dlc_inst=(struct prh_rfcomm_dlc_inst *)param;
  
  /* TBC : Timer needs more thought */
  #if pDEBUG
  pDebugPrintfEX((pLOGFATAL,pLOGRFCOMM,"RFCOMM: DLC Timed out occured"));
  #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
   SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM,(LOG_RFC_RFCOMM_DLC_STATE_INDEX+10),NULL,0,0);
#endif
  dlc_inst->pfbit_timer=0;
  prh_rfcomm_dlc_do_event(dlc_inst, PRH_RFCOMM_TIMEOUT, pNULL);

}

#if PRH_RFCOMM_SM2==1
void prh_rfcomm_dlc_incoming_sec_callback(u_int32 cbHandle, t_api status)
{
  struct prh_rfcomm_dlc_inst *self=(struct prh_rfcomm_dlc_inst *)(void *)cbHandle;
  struct prh_rfcomm_server_inst *serv_inst;
  int ret;
  struct host_buf *tx_buf;

  if (status==BT_NOERROR)
    {
      ret=prh_rfcomm_lookup_server_inst_by_schan((u_int8)(self->dlci>>1), &serv_inst);
      if (ret==BT_NOERROR)
      {
	
	if (serv_inst->dlc_cbs.dlc_estab_ind)
	  serv_inst->dlc_cbs.dlc_estab_ind(self->rfcomm_l2cap_ptr->bd_addr, self->dlci, &self->dlc_params, self->rfcomm_l2cap_ptr);
      }
    }
  else
    {
    #if pDEBUG
      pDebugPrintf((pLOGDEBUG,"RFCOMM: Sending DM frame channel closed %d\n", self->dlci));
    #elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
      SYSDebug_PrintEx(pLOGDEBUG,pLOGRFCOMM,(LOG_RFC_RFCOMM_DLC_STATE_INDEX+2),&(self->dlci),1,BT_CODE_BYTE);
    #endif

      ret=PRH_RFCOMM_GET_L2_FRAME(self->rfcomm_l2cap_ptr->cid, PRH_RFCOMM_FR_BUFSIZE, &tx_buf);
      if (ret!=BT_NOERROR)
	    return;
      
      /* DM is a response frame so CR bit is opposite to direction */
      prh_rfcomm_create_dm(tx_buf, self->dlci, PRH_RFCOMM_PF_SET, !(self->rfcomm_l2cap_ptr->initiating));
      L2_Write(self->rfcomm_l2cap_ptr->cid, (t_DataBuf *)tx_buf);
      pMutexLock(self->lock);
      prh_rfcomm_remove_dlc_instance(self->rfcomm_l2cap_ptr, self->dlci, NULL);
      prh_rfcomm_free_dlc_instance(self);
    }

  return;
}
#endif
