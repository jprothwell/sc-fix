/*
 * MODULE NAME:    tcs_cc_callbacks.c
 * DESCRIPTION:    TCS Call Control Callbacks
 * AUTHOR:         Thomas Howley
 * DATE:           13 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_cc_callbacks.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies.
 *     All rights reserved.
 *
 */

#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)

#include "host_types.h"
#include "host_error.h"


#include "tcs_cc_callbacks.h"
#include "tcs_cc_upper.h"
#include "tcs_cc_core.h"
#include "tcs_core.h"
#include "tcs_cc_enc.h"
#include "tcs_cc_dec.h"
#include "tcs_cc_peer.h"
#include "tcs_l2cap.h"

#if PRH_TCS_TERMINAL
#include "tcs_cc_terminal.h"
#endif
#if PRH_TCS_GATEWAY
#include "tcs_cc_gateway.h"
#endif



#if PRH_TCS_TERMINAL



t_api prh_tcs_cc_callback_TL_L2Data(struct prh_st_tcs_l2conn *device_info, struct host_buf *h_buff)
{
	struct st_t_TCS_callParams *callParams;
	t_api status;
	u_int8 tcsMsg = h_buff->data[0];
	struct st_t_TCS_callInstance *callInstance;
	u_int8 callType;
	u_int16 cid;
	t_bdaddr *address;

	callParams = (struct st_t_TCS_callParams*)pMalloc(sizeof(struct st_t_TCS_callParams));
	if(!callParams)
		return BT_NORESOURCES;
	prh_tcs_cc_core_initCallParams(callParams);

	if(BT_NOERROR != (status = prh_tcs_cc_decodeMsg(h_buff, callParams)))
	{
		host_buf_free(h_buff);
		return status;
	}

	host_buf_free(h_buff);

	if(PRH_TCS_MSG_SETUP == tcsMsg)
	{
		
        /* if device_info is not NULL, then it's CO. else SETUP was broadcast (connectionless l2cap) */
		if(device_info) 
		{
            pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Received SETUP, cid=%d", device_info->cid));			
			cid = device_info->cid;
			if(PRH_TCS_PSM_TCSBIN == device_info->psm)
				callType = PRH_TCS_CALLTYPE_TCSBIN;	
			else if(PRH_TCS_PSM_TCSCORDLESS == device_info->psm)
				callType = PRH_TCS_CALLTYPE_TCSCORDLESS;	
			else
				return BT_NOERROR; /* Invalid psm value */
		}
		else /* Assume that this is TCSCORDLESS-related */
		{
            pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Received Broadcast SETUP"));
			callType = PRH_TCS_CALLTYPE_TCSCORDLESS;
			cid = 0;
		}

		callInstance = prh_tcs_cc_core_TLgetNewCallInstance(cid, callType);	
		if(!callInstance)
			return BT_NORESOURCES;
        
        pTimerCancel(callInstance->timerHandle);
		callInstance->timerHandle=0;
        callInstance->timerHandle=pTimerCreate(PRH_TCS_TIMER_BCASTKEEPCALLINSTANCE ,prh_tcs_cc_callback_bcast_rx_setup_tx_no_reply_timeout, NULL, pTIMER_ONESHOT);

		if(device_info)
			device_info->un.call = callInstance;  /* so that incoming data is routed to correct call instance */
		
	}
	else
	{
		callInstance = device_info->un.call;
		if(!callInstance)
			return BT_INVALIDPARAM;
	}

	if(device_info)
		address = &(device_info->bd_addr);
	else
		address = &(TCSInstance.GWBDAddress);

	prh_tcs_cc_core_freeCallParams(callInstance->inCallParams);
	callInstance->inCallParams = callParams;
	return prh_tcs_peer_processEvent(tcsMsg, *address, callInstance);

}

void prh_tcs_cc_core_TLconnectIndCallback(struct prh_st_tcs_l2conn * l2conn)
{
	u_int8 status;
	u_int16 ACLHandle;
	TCSInstance.cid=l2conn->cid;
	/* Inform App of new incoming channel */
	status=L2_RetrieveHandle(l2conn->cid,&ACLHandle);
    if(status != BT_NOERROR)
        return;
    
	if(TCSInstance.callbacks->sigChannelConnectIndication)
	    TCSInstance.callbacks->sigChannelConnectIndication(l2conn->bd_addr, l2conn->psm, ACLHandle);	
}

void prh_tcs_cc_core_TLdisconnectIndCallback(struct prh_st_tcs_l2conn *l2conn)
{
	struct st_t_TCS_callParams callParams;
	struct st_t_TCS_callInstance *callInstance;

	callInstance = l2conn->un.call;
	if(callInstance)
	{
		callParams.bitMask = 0;
		if(TCSInstance.callbacks->callReleaseCompleteIndication)
			TCSInstance.callbacks->callReleaseCompleteIndication(l2conn->bd_addr, callInstance->callHandle, &callParams);

		prh_tcs_cc_core_freeCallInstance(callInstance);
	}

	if(PRH_TCS_PSM_TCSCORDLESS == l2conn->psm)
	{
		if(TCSInstance.cid == l2conn->cid)
		{
			/* inform app that GW is disconnected */
			if(TCSInstance.callbacks->sigChannelDisconnectIndication)
				TCSInstance.callbacks->sigChannelDisconnectIndication(l2conn->bd_addr, l2conn->psm);
		}
	}
	else if(PRH_TCS_PSM_TCSBIN == l2conn->psm)
	{
		if(TCSInstance.callbacks->sigChannelDisconnectIndication)
			TCSInstance.callbacks->sigChannelDisconnectIndication(l2conn->bd_addr, l2conn->psm);
	}

	if(TCSInstance.cid == l2conn->cid)
		TCSInstance.cid = 0;

}

void prh_tcs_cc_callback_bcast_rx_setup_tx_no_reply_timeout(void *callbackParam)
{
    struct st_t_TCS_callInstance *callInstance;
    /* 
        we have received a broadcast SETUP message. We have not responded to
        it and we have stopped receiving these broadcast SETUP messages for 
        a length of time as defined by the pTimerCreate() that used this
        fun as it's callback.

        We must now look through our callInstances and remove the callInstance
        that has a CID ==0. ie the callInstance for the broadcast SETUP message.
    
    */
    callInstance= prh_tcs_cc_core_getCallInstanceByCID((u_int16)0);
    if(callInstance)
    {   
        /* 
            this callInstance will have NO associated tcs_l2cap structure 
            therefore, just free the callInstance members
        */
        prh_tcs_cc_core_freeUnassignedBcastCallInstance(callInstance);
    }
}

#endif /* PRH_TCS_TERMINAL */




#if PRH_TCS_GATEWAY

t_api prh_tcs_cc_callback_GW_L2Data(struct prh_st_tcs_l2conn *device_info, struct host_buf *h_buff)
{
	struct st_t_TCS_callParams *callParams;
	t_api status;
	u_int8 tcsMsg = h_buff->data[0];
	struct st_t_TCS_TLPeer *TLPeer;
	struct st_t_TCS_callInstance *callInstance;

	callParams = (struct st_t_TCS_callParams*)pMalloc(sizeof(struct st_t_TCS_callParams));

	if(!callParams)
		return BT_NORESOURCES;
	prh_tcs_cc_core_initCallParams(callParams);

	if(BT_NOERROR != (status = prh_tcs_cc_decodeMsg(h_buff, callParams)))
	{
		host_buf_free(h_buff);
		return status;
	}

	host_buf_free(h_buff);

	/* Call Instance has not yet been allocated, either an incoming SETUP or a first response to a SETUP (e.g. CONNECT, ALERTING) */
	if(!(device_info->un.call)) 
	{
		/* Firstly check if TLPeer instance exists for this address */
		TLPeer = prh_tcs_cc_core_getTLPeerByBdAddress(device_info->bd_addr);
		if(!TLPeer)
			TLPeer = prh_tcs_cc_core_getNewTLPeer(device_info->bd_addr, device_info->cid);
		
		if(!TLPeer)
			return BT_NORESOURCES;

		/* If SETUP message allocate a new call Instance for this TL peer */
		if(PRH_TCS_MSG_SETUP == tcsMsg)
		{   	
			callInstance = prh_tcs_cc_core_GWgetNewCallInstance(TLPeer, device_info->cid, device_info->psm);
		}
		else
		{
            /* this is a first response to a broadcast message */
            /* if this is the first response from a device we give it the call,
            if this is a subsequent response from another device we send a RELEASE*/
		/*	callInstance = prh_tcs_cc_core_GWaddBCastCallInstance(TLPeer, device_info->cid);*/
            if(TCSInstance.pendingBroadcastSETUP ==0)
            {
                /* subsequent message => RELEASE */
                /* (might put this elsewhere?) */
                
                /* reuse the callParams variable for outgoing */
                prh_tcs_cc_core_initCallParams(callParams);
                callParams->bitMask=PRH_TCS_CC_PARAM_CAUSE;
                callParams->cause=PRH_TCS_CAUSE_NONSELECTEDUSERCLEARING;
                status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_RELEASE, callParams, device_info->cid);
                return BT_NOERROR;                 
            }
            else
            {   /* this is the first terminal to accept or reject the call */            
                pTimerCancel(TCSInstance.broadcastSETUPtimer);
				TCSInstance.broadcastSETUPtimer=0;
	            TCSInstance.pendingBroadcastSETUP = 0;
                /* kill the repeat broadcast SETUP timer */
                pTimerCancel(TCSInstance.broadcastRepeatSETUPtimer);
				TCSInstance.broadcastRepeatSETUPtimer=0
                callInstance = prh_tcs_cc_core_GWgetNewCallInstance(TLPeer, device_info->cid, PRH_TCS_CALLTYPE_TCSCORDLESS);
			    switch(tcsMsg)
			    {
			    case PRH_TCS_MSG_ALERTING:
			    case PRH_TCS_MSG_CALL_PROCEEDING:
			    case PRH_TCS_MSG_CONNECT:
                case PRH_TCS_MSG_RELEASE_COMPLETE:
				    callInstance->state = PRH_TCS_CALLINITIATED;
			    }
            }

		}

		device_info->un.call = callInstance; /* so that incoming data is routed to correct call instance */

	}
	else
	{
		callInstance = device_info->un.call;
		if(!callInstance)
			return BT_INVALIDPARAM;
	}

	prh_tcs_cc_core_freeCallParams(callInstance->inCallParams);
	callInstance->inCallParams = callParams;
	return prh_tcs_peer_processEvent(tcsMsg, device_info->bd_addr, callInstance);
	

}

void prh_tcs_cc_callback_bcast_setup_repeat_timeout(void *callbackParam)
{
    struct st_t_TCS_callParams *callParams = (struct st_t_TCS_callParams *)callbackParam;
    u_int8 status;

    status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_SETUP, callParams, TCSInstance.broadcastGroupCID);
    if(BT_NOERROR == status)
        pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent Broadcast SETUP, group CID=%d", TCSInstance.broadcastGroupCID));
    else
        pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Problem Sending Broadcast SETUP, group CID=%d", TCSInstance.broadcastGroupCID));

    TCSInstance.broadcastRepeatSETUPtimer=pTimerCreate(PRH_TCS_TIMER_BCASTSETUPREPEAT, prh_tcs_cc_callback_bcast_setup_repeat_timeout, (void *)callParams, pTIMER_ONESHOT); 

    return;
}

void prh_tcs_cc_callback_broadcastSETUP_timeout(void *callbackParam)
{
	struct st_t_TCS_callParams callParams;
	int i;

	pTimerCancel(TCSInstance.broadcastSETUPtimer);
	TCSInstance.broadcastSETUPtimer=0;
	TCSInstance.pendingBroadcastSETUP = 0;

    /* kill the repeat broadcast SETUP timer */
    pTimerCancel(TCSInstance.broadcastRepeatSETUPtimer);
	TCSInstance.broadcastRepeatSETUPtimer=0;
	callParams.bitMask = PRH_TCS_CC_PARAM_CAUSE;
	callParams.cause = PRH_TCS_CAUSE_RECOVERYONTIMEREXPIRY;
	for(i=0; i<PRH_TCS_MAXTERMINALSPERGATEWAY; i++)
	{
		if(TCSInstance.TLPeer[i])
		{
			if(TCSInstance.callbacks->callReleaseCompleteIndication)
				TCSInstance.callbacks->callReleaseCompleteIndication(TCSInstance.TLPeer[i]->bdAddress, PRH_TCS_CALLHANDLEUNASSIGNED, &callParams);
		}

	}
}


void prh_tcs_cc_callback_unparkAllACLs(t_api status)
{

}


void prh_tcs_cc_core_GWconnectIndCallback(struct prh_st_tcs_l2conn * l2conn)
{
	struct st_t_TCS_TLPeer *TLPeer;
	u_int16 ACLHandle;
    u_int8 status;
	/* assign TLPeer to destination Terminal, if this is a new Terminal */
	status=L2_RetrieveHandle(l2conn->cid,&ACLHandle);
    if(status != BT_NOERROR)
        return;

	TLPeer = prh_tcs_cc_core_getTLPeerByBdAddress(l2conn->bd_addr);
	if(!TLPeer)
	{
		TLPeer = prh_tcs_cc_core_getNewTLPeer(l2conn->bd_addr, l2conn->cid);
		/* Inform App of new incoming channel */
		if(TCSInstance.callbacks->sigChannelConnectIndication)
			TCSInstance.callbacks->sigChannelConnectIndication(l2conn->bd_addr, l2conn->psm, ACLHandle);
	}
}
	
void prh_tcs_cc_core_GWdisconnectIndCallback(struct prh_st_tcs_l2conn *l2conn)
{
	struct st_t_TCS_TLPeer *TLPeer;
	struct st_t_TCS_callInstance *callInstance;
	struct st_t_TCS_callParams callParams;
	int i;
	u_int8 invokeCallback;
	u_int8 noCallInstances;

	invokeCallback = (TCSInstance.callbacks->callReleaseCompleteIndication != NULL);

	/* free call associated (if one is) with signalling channel */
	callInstance = l2conn->un.call;
	if(callInstance)
	{
		callParams.bitMask = 0;
		if(invokeCallback)
			TCSInstance.callbacks->callReleaseCompleteIndication(l2conn->bd_addr, callInstance->callHandle, &callParams);
		prh_tcs_cc_core_freeCallInstance(callInstance);
	}
	
	TLPeer = prh_tcs_cc_core_getTLPeerByBdAddress(l2conn->bd_addr);
    if(TLPeer)
	{	
		/* if this is the main signalling channel to TL: remove TLPeer and all call Instances */
		if(TLPeer->cid == l2conn->cid)
		{
			for(i=0; i<PRH_TCS_MAX_CALLS_PER_DEVICE; i++)
			{
				if(TLPeer->callInstance[i])
				{
					if(invokeCallback)
						TCSInstance.callbacks->callReleaseCompleteIndication(l2conn->bd_addr, TLPeer->callInstance[i]->callHandle, &callParams);
					prh_tcs_cc_core_freeCallInstance(TLPeer->callInstance[i]);
				}
			}
			
			/* clear the info of the TLPeer if there are no more call instances to it */
			noCallInstances=1;
			/* is there any noCallInstances ?*/
			for(i=0; i<PRH_TCS_MAX_CALLS_PER_DEVICE; i++)
			{
				if(TLPeer->callInstance[i])
				{
					noCallInstances=0;
				}			
			}
			
			/* if there is no callinstances => find and delete the TLPeer */
			if(noCallInstances)
				prh_tcs_cc_core_freeTLPeerByBdAddress(l2conn->bd_addr);		

			/* inform app that the TL is disconnected */
			if(TCSInstance.callbacks->sigChannelDisconnectIndication)
				TCSInstance.callbacks->sigChannelDisconnectIndication(l2conn->bd_addr, l2conn->psm);


		}
	}
}

#endif /* PRH_TCS_GATEWAY */

t_api prh_tcs_cc_callback_L2ConnComplete(t_api status, struct prh_st_tcs_l2conn *l2_conn)
{
	struct st_t_TCS_callInstance *callInstance;

	if(l2_conn)
		callInstance = l2_conn->un.call;
	else
		return BT_UNKNOWNERROR;

	/* if callInstance is NULL, this L2CAP channel is being set up as part of registration 
		and is not yet being used for a call */

#if(PRH_TCS_TERMINAL == 1)
		TCSInstance.cid = l2_conn->cid;
		if(registerCompleteCB)
		{
			registerCompleteCB(status);
			registerCompleteCB = NULL;
		}

		if(connectToTLCompleteCB)
		{
			connectToTLCompleteCB(l2_conn->bd_addr, status);
			connectToTLCompleteCB = NULL;
		}
		if(connectToGWCompleteCB)
		{
			connectToGWCompleteCB(l2_conn->bd_addr, status);
			connectToGWCompleteCB = NULL;
		}
#endif
#if (PRH_TCS_GATEWAY == 1)
	

#endif
		/* Must also Check first Call Instance */
		return BT_NOERROR;

}

void prh_tcs_cc_callback_timeout(void *callbackParam)
{
	struct st_t_TCS_callInstance *callInstance = (struct st_t_TCS_callInstance *)callbackParam;
	struct st_t_TCS_callParams callParams;
	t_bdaddr *address;
	t_api status;

	callParams.bitMask = PRH_TCS_CC_PARAM_CAUSE;
	callParams.cause = PRH_TCS_CAUSE_RECOVERYONTIMEREXPIRY;
	if(callInstance)
	{
		switch(callInstance->state)
		{

		case PRH_TCS_CALLINITIATED: /* T303 *//*Outgoing*/
			/*
			if on cls channel, return to null state.
			if in co channel, send a RELEASE COMPLETE msg, with cause #102
			*/
			prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), &callParams);
			status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_RELEASE_COMPLETE, callInstance->outCallParams, callInstance->cid);
			if(BT_NOERROR == status)
			{
				pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent RELEASE COMPLETE, call handle=%d", callInstance->callHandle));				
#if 0
                /* May have to release the L2CAP signalling channel used for the call */
				if(prh_tcs_cc_core_L2ReleaseRequired(callInstance))
					L2_Disconnect(callInstance->cid);
#endif
			}
			else
				pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send RELEASE COMPLETE call handle=%d", callInstance->callHandle));	
			
			address = prh_tcs_cc_core_getBdAddressByCallInstance(callInstance);
			if(TCSInstance.callbacks->callReleaseCompleteIndication)
				TCSInstance.callbacks->callReleaseCompleteIndication(*address, callInstance->callHandle, &callParams);
			
			/* free call instance */
			prh_tcs_cc_core_freeCallInstance(callInstance);
			break;
		

		case PRH_TCS_OVERLAPRECEIVING: /* T302 *//*Incoming*/
			/*
			if info is incomplete, clear call with cause #28 invalid or incomplete number 
			*/
			if(!PRH_TCS_CC_PARAM_SENDINGCOMPLETE_EXISTS(callInstance->inCallParams->bitMask))
			{
				callParams.bitMask = PRH_TCS_CC_PARAM_CAUSE;
				callParams.cause = PRH_TCS_CAUSE_INVALIDORINCOMPLETENUMBER;
				callInstance->state = PRH_TCS_DISCONNECTREQUEST;
				status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_DISCONNECT, callInstance->outCallParams, callInstance->cid);
				if(BT_NOERROR == status)
				{
					pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent DISCONNECT, call handle=%d", callInstance->callHandle));			
					callInstance->timerHandle=pTimerCreate(PRH_TCS_TIMER_T305, prh_tcs_cc_callback_timeout, (void *)callInstance, pTIMER_ONESHOT); 
				}
				else
					pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send DISCONNECT call handle=%d", callInstance->callHandle));						
			}
			else 
			{
				/* according to the spec I reply with a CALL PROCEEDING, ALERTING or CONNECT Message */
				/*  I choose to reply with a CALL_PROCEEDING msg */
				
				callInstance->state = PRH_TCS_INCOMINGCALLPROCEEDING;
				status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_CALL_PROCEEDING, callInstance->outCallParams, callInstance->cid);
				if(BT_NOERROR == status)
				{
					pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent CALL PROCEEDING, call handle=%d", callInstance->callHandle));
				}
				else
					pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send CALL PROCEEDING, call handle=%d", callInstance->callHandle));
			}
			break;
			
			
		
		case PRH_TCS_OVERLAPSENDING: /* T304 *//*Outgoing*/
		case PRH_TCS_OUTGOINGCALLPROCEEDING: /* T310 *//*Outgoing*/
		case PRH_TCS_CALLDELIVERED: /* T301 *//*Outgoing*/
		case PRH_TCS_CONNECTREQUEST: /* T313 *//*Incoming*/
			/*
			The most common action - initiate call clearing (send DISCONNECT msg) 
			with cause #102 recovery on timer expiry
			*/	
			callInstance->state = PRH_TCS_DISCONNECTREQUEST;
			status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_DISCONNECT, callInstance->outCallParams, callInstance->cid);
			if(BT_NOERROR == status)
			{
				pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent DISCONNECT, call handle=%d", callInstance->callHandle));			
				callInstance->timerHandle=pTimerCreate(PRH_TCS_TIMER_T305, prh_tcs_cc_callback_timeout, (void *)callInstance, pTIMER_ONESHOT); 
			}
			else
				pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send DISCONNECT call handle=%d", callInstance->callHandle));	
			break;


		/****************************
		Call Clearing Timer callbacks
		****************************/


		case PRH_TCS_DISCONNECTREQUEST: /* T305 *//*Clearing-Outgoing*/
			/*
			have already sent a disconnect, but got a timeout from it.
			send a RELEASE with the cause from the DISCONNECT and start 
			timer T308.
			change state to release request state.
			*/
			callInstance->state = PRH_TCS_RELEASEREQUEST;
			status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_RELEASE, callInstance->outCallParams, callInstance->cid);
			if(BT_NOERROR == status)
			{
				pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent RELEASE, call handle=%d", callInstance->callHandle));			
				callInstance->timerHandle=pTimerCreate(PRH_TCS_TIMER_T308, prh_tcs_cc_callback_timeout, (void *)callInstance, pTIMER_ONESHOT); 
			}
			else
				pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send RELEASE call handle=%d", callInstance->callHandle));	
			break;


			break;


		case PRH_TCS_RELEASEREQUEST: /* T308 *//*Clearing-Incoming*/
			/*
			return to the NULL state
			*/
			callInstance->state = PRH_TCS_IDLE;
			break;


		default:
			break;
		}
	}

}






#if (PRH_TCS_TERMINAL && PRH_TCS_GATEWAY)
t_api prh_tcs_cc_callback_TLorGW_L2Data(struct prh_st_tcs_l2conn *device_info, struct host_buf *p_buf)
{
	if(PRH_TCS_MODE_TERMINAL == tcsMode)
		return prh_tcs_cc_callback_TL_L2Data(device_info, p_buf);
	else
		return prh_tcs_cc_callback_GW_L2Data(device_info, p_buf);
}


#endif /* (PRH_TCS_TERMINAL && PRH_TCS_GATEWAY) */

#endif /*(PRH_TCS_TERMINAL | PRH_TCS_GATEWAY)*/
