/*
 * MODULE NAME:    tcs_cc_peer.c
 * DESCRIPTION:    TCS Call Control Peer
 * AUTHOR:         Thomas Howley
 * DATE:           13 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_cc_peer.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies.
 *     All rights reserved.
 *
 */

#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)

#include "host_types.h"

#include "tcs_cc_peer.h"
#include "tcs_cc_types.h"
#include "tcs_cc_core.h"
#include "tcs_cc_enc.h"
#include "tcs_cc_callbacks.h"
#include "tcs_cc_upper.h"
#include "mgr_core.h"



t_api prh_tcs_peer_SETUP(t_bdaddr address, struct st_t_TCS_callInstance *callInstance);
t_api prh_tcs_peer_SETUPACKNOWLEDGE(t_bdaddr address, struct st_t_TCS_callInstance *callInstance);
t_api prh_tcs_peer_ALERTING(t_bdaddr address, struct st_t_TCS_callInstance *callInstance);
t_api prh_tcs_peer_CALLPROCEEDING(t_bdaddr address, struct st_t_TCS_callInstance *callInstance);
t_api prh_tcs_peer_CONNECT(t_bdaddr address, struct st_t_TCS_callInstance *callInstance);
t_api prh_tcs_peer_CONNECTACKNOWLEDGE(t_bdaddr address, struct st_t_TCS_callInstance *callInstance);
t_api prh_tcs_peer_PROGRESS(t_bdaddr address, struct st_t_TCS_callInstance *callInstance);
t_api prh_tcs_peer_DISCONNECT(t_bdaddr address, struct st_t_TCS_callInstance *callInstance);
t_api prh_tcs_peer_RELEASE(t_bdaddr address, struct st_t_TCS_callInstance *callInstance);
t_api prh_tcs_peer_RELEASECOMPLETE(t_bdaddr address, struct st_t_TCS_callInstance *callInstance);
t_api prh_tcs_peer_INFORMATION(t_bdaddr address, struct st_t_TCS_callInstance *callInstance);
t_api prh_tcs_peer_STARTDTMF(t_bdaddr address, struct st_t_TCS_callInstance *callInstance);
t_api prh_tcs_peer_STARTDTMFACKNOWLEDGE(t_bdaddr address, struct st_t_TCS_callInstance *callInstance);
t_api prh_tcs_peer_STARTDTMFREJECT(t_bdaddr address, struct st_t_TCS_callInstance *callInstance);
t_api prh_tcs_peer_STOPDTMF(t_bdaddr address, struct st_t_TCS_callInstance *callInstance);
t_api prh_tcs_peer_STOPDTMFACKNOWLEDGE(t_bdaddr address, struct st_t_TCS_callInstance *callInstance);
t_api prh_tcs_peer_prepareDefaultOutCallParams(struct st_t_TCS_callInstance *callInstance);

t_api (*peerTCSmsgToFunction[])(t_bdaddr address, struct st_t_TCS_callInstance *callInstance) = {
	prh_tcs_peer_ALERTING,
	prh_tcs_peer_CALLPROCEEDING,
	prh_tcs_peer_CONNECT,
	prh_tcs_peer_CONNECTACKNOWLEDGE,
	prh_tcs_peer_PROGRESS,
	prh_tcs_peer_SETUP,
	prh_tcs_peer_SETUPACKNOWLEDGE,
	prh_tcs_peer_DISCONNECT,
	prh_tcs_peer_RELEASE,
	prh_tcs_peer_RELEASECOMPLETE,
	prh_tcs_peer_INFORMATION,
	prh_tcs_peer_STARTDTMF,
	prh_tcs_peer_STARTDTMFACKNOWLEDGE,
	prh_tcs_peer_STARTDTMFREJECT,
	prh_tcs_peer_STOPDTMF,
	prh_tcs_peer_STOPDTMFACKNOWLEDGE
};


t_api prh_tcs_peer_processEvent(u_int8 tcsMsg, t_bdaddr address, struct st_t_TCS_callInstance *callInstance)
{
	u_int8 index;
	index = (tcsMsg <= PRH_TCS_MSG_INFORMATION)?tcsMsg:tcsMsg-5;

	if(NULL != peerTCSmsgToFunction[index])
	{		
		return peerTCSmsgToFunction[index](address, callInstance);
	}
	else
		return BT_INVALIDPARAM;
}



t_api prh_tcs_peer_SETUP(t_bdaddr address, struct st_t_TCS_callInstance *callInstance)
{

	if(PRH_TCS_IDLE == callInstance->state || PRH_TCS_CALLPRESENT == callInstance->state/* added to allow for multiple SETUP messages during broadcast */)
	{
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: The SETUP message is for call handle=%d", callInstance->callHandle));
		if(NULL != TCSInstance.callbacks->callSetupIndication)
		{		
			callInstance->state = PRH_TCS_CALLPRESENT;
			if(TCSInstance.callbacks->callSetupIndication)	
				TCSInstance.callbacks->callSetupIndication(address, callInstance->callHandle, callInstance->callType, callInstance->inCallParams);
		}
	}
	else
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Received SETUP in invalid state!, call handle=%d", callInstance->callHandle));

	return BT_NOERROR;
}


t_api prh_tcs_peer_SETUPACKNOWLEDGE(t_bdaddr address, struct st_t_TCS_callInstance *callInstance)
{

	if(PRH_TCS_CALLINITIATED == callInstance->state)
	{
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Received SETUPACKNOWLEDGE, call handle=%d", callInstance->callHandle));
		if(NULL != TCSInstance.callbacks->setupAckIndication)
		{			
			callInstance->state = PRH_TCS_OVERLAPSENDING;
			pTimerCancel(callInstance->timerHandle);
			callInstance->timerHandle=0;
			callInstance->timerHandle=pTimerCreate(PRH_TCS_TIMER_T304, prh_tcs_cc_callback_timeout, (void *)callInstance, pTIMER_ONESHOT); 
			if(TCSInstance.callbacks->setupAckIndication)	
				TCSInstance.callbacks->setupAckIndication(address, callInstance->callHandle, callInstance->inCallParams);
		}

	}
	else
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Received SETUPACKNOWLEDGE in invalid state!, call handle=%d", callInstance->callHandle));

	return BT_NOERROR;
}



t_api prh_tcs_peer_ALERTING(t_bdaddr address, struct st_t_TCS_callInstance *callInstance)
{

	switch(callInstance->state)
	{
	case PRH_TCS_CALLINITIATED: /* stop timer T303 */
	case PRH_TCS_OVERLAPSENDING: /* stop timer T304 */
	case PRH_TCS_OUTGOINGCALLPROCEEDING: /* stop timer T310 */
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Received ALERTING, call handle=%d", callInstance->callHandle));
		pTimerCancel(callInstance->timerHandle); /* stop timer - depends on current state */
		callInstance->timerHandle=0;
		callInstance->state = PRH_TCS_CALLDELIVERED;
		callInstance->timerHandle=pTimerCreate(PRH_TCS_TIMER_T301, prh_tcs_cc_callback_timeout, (void *)callInstance, pTIMER_ONESHOT); 
		if(TCSInstance.callbacks->alertingIndication)
			TCSInstance.callbacks->alertingIndication(address, callInstance->callHandle, callInstance->inCallParams);
		break;

	default:
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Received ALERTING in invalid state!, call handle=%d", callInstance->callHandle));
		break;
	}

	return BT_NOERROR;
}



t_api prh_tcs_peer_CALLPROCEEDING(t_bdaddr address, struct st_t_TCS_callInstance *callInstance)
{
	switch(callInstance->state)
	{
	case PRH_TCS_OVERLAPSENDING:
		pTimerCancel(callInstance->timerHandle); /* stop timer T304 */
		callInstance->timerHandle=0;
	case PRH_TCS_CALLINITIATED:
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Received CALLPROCEEDING, call handle=%d", callInstance->callHandle));
		callInstance->state = PRH_TCS_OUTGOINGCALLPROCEEDING;
		callInstance->timerHandle=pTimerCreate(PRH_TCS_TIMER_T310, prh_tcs_cc_callback_timeout, (void *)callInstance, pTIMER_ONESHOT); 
		if(TCSInstance.callbacks->callProceedingIndication)
			TCSInstance.callbacks->callProceedingIndication(address, callInstance->callHandle, callInstance->inCallParams);
		break;

	default:
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Received CALLPROCEEDING in invalid state!, call handle=%d", callInstance->callHandle));
		break;
	}

	return BT_NOERROR;
}


t_api prh_tcs_peer_CONNECT(t_bdaddr address, struct st_t_TCS_callInstance *callInstance)
{

	switch(callInstance->state)
	{
	case PRH_TCS_CALLINITIATED: /* stop timer T303 */
	case PRH_TCS_OVERLAPSENDING: /* stop timer T304 */
	case PRH_TCS_OUTGOINGCALLPROCEEDING: /* stop timer T310 */
	case PRH_TCS_CALLDELIVERED: /* stop timer T301 */
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Received CONNECT, call handle=%d", callInstance->callHandle));
		pTimerCancel(callInstance->timerHandle); /* stop timer - depends on current state */
		callInstance->timerHandle=0;
		callInstance->state = PRH_TCS_CONNECTACKPENDING;
		/* store SCO handle */
		if(PRH_TCS_CC_PARAM_SCOHANDLE_EXISTS(callInstance->inCallParams->bitMask))
			callInstance->bearerHandle = (callInstance->inCallParams->scoHandle);

		if(TCSInstance.callbacks->connectIndication)
            TCSInstance.callbacks->connectIndication(address, callInstance->callHandle, callInstance->inCallParams);

	break;
		
	default:
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Received CONNECT in invalid state!, call handle=%d", callInstance->callHandle));
		break;
	}

	return BT_NOERROR;
}




t_api prh_tcs_peer_CONNECTACKNOWLEDGE(t_bdaddr address, struct st_t_TCS_callInstance *callInstance)
{
	if(PRH_TCS_CONNECTREQUEST == callInstance->state)
	{
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Received CONNECTACKNOWLEDGE, call handle=%d", callInstance->callHandle));
		pTimerCancel(callInstance->timerHandle); /* cancel T313 timer */
		callInstance->timerHandle=0;
		callInstance->state = PRH_TCS_TERMACTIVE;
		if(TCSInstance.callbacks->callCompleteIndication)
			TCSInstance.callbacks->callCompleteIndication(address, callInstance->callHandle);
			
	}
	else
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Received CONNECTACKNOWLEDGE in invalid state!, call handle=%d", callInstance->callHandle));

	return BT_NOERROR;
}





t_api prh_tcs_peer_INFORMATION(t_bdaddr address, struct st_t_TCS_callInstance *callInstance)
{
	switch(callInstance->state)
	{
	case PRH_TCS_OVERLAPRECEIVING:
		/* reset T302 timer, only need to reset timer if number is not complete */
		pTimerCancel(callInstance->timerHandle);
		callInstance->timerHandle0;
		callInstance->timerHandle=pTimerCreate(PRH_TCS_TIMER_T302, prh_tcs_cc_callback_timeout, (void *)callInstance, pTIMER_ONESHOT); 
	case PRH_TCS_INCOMINGCALLPROCEEDING:
	case PRH_TCS_CALLRECEIVED:
	case PRH_TCS_ORIGACTIVE:
	case PRH_TCS_TERMACTIVE:
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Received INFORMATION, call handle=%d", callInstance->callHandle));
		if(NULL != TCSInstance.callbacks->informationIndication)
		{

			if(TCSInstance.callbacks->informationIndication)
				TCSInstance.callbacks->informationIndication(callInstance->callHandle, callInstance->inCallParams);
		}
		break;


	default:
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Received INFORMATION in invalid state!, call handle=%d", callInstance->callHandle));
		break;
	}

	return BT_NOERROR;
}


t_api prh_tcs_peer_DISCONNECT(t_bdaddr address, struct st_t_TCS_callInstance *callInstance)
{

	if(!callInstance)
		return BT_INVALIDPARAM;
	
	switch(callInstance->state)
	{
	case PRH_TCS_CALLINITIATED: /* stop timer T303 (originating side) */
	case PRH_TCS_OVERLAPSENDING: /* stop timer T304 (originating side) */
	case PRH_TCS_OUTGOINGCALLPROCEEDING: /* stop timer T310 (originating side) */
	case PRH_TCS_CALLDELIVERED: /* stop timer T301 (originating side) */
	case PRH_TCS_OVERLAPRECEIVING: /* stop timer T302 (terminating side)*/
	case PRH_TCS_CONNECTREQUEST: /* stop timer T313 (terminating side)*/
		pTimerCancel(callInstance->timerHandle); /* stop timer - depends on current state */
		callInstance->timerHandle=0;
	case PRH_TCS_INCOMINGCALLPROCEEDING:
	case PRH_TCS_CALLRECEIVED:
	case PRH_TCS_ORIGACTIVE:
	case PRH_TCS_TERMACTIVE:
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Received DISCONNECT, call handle=%d", callInstance->callHandle));
		/* disconnect from bearer */
		callInstance->state = PRH_TCS_DISCONNECTINDICATION;
		/*		callInstance->timerHandle=pTimerCreate(PRH_TCS_TIMER_T308, prh_tcs_cc_callback_timeout, (void *)callInstance, pTIMER_ONESHOT);  */
		if(TCSInstance.callbacks->disconnectIndication)
			TCSInstance.callbacks->disconnectIndication(callInstance->callHandle, callInstance->inCallParams);

		break;
		
	default:
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Received DISCONNECT in invalid state!, call handle=%d", callInstance->callHandle));
		break;
		
	}

	return BT_NOERROR;
}


t_api prh_tcs_peer_RELEASE(t_bdaddr address, struct st_t_TCS_callInstance *callInstance)
{

	if(!callInstance)
		return BT_INVALIDPARAM;

	switch(callInstance->state)
	{
	case PRH_TCS_ORIGACTIVE:
	case PRH_TCS_TERMACTIVE:
	case PRH_TCS_DISCONNECTINDICATION:
	case PRH_TCS_DISCONNECTREQUEST:
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Received RELEASE, call handle=%d", callInstance->callHandle));
		pTimerCancel(callInstance->timerHandle); /* stop timer T305 */
		callInstance->timerHandle=0;
		if(TCSInstance.callbacks->releaseIndication)
			TCSInstance.callbacks->releaseIndication(callInstance->callHandle, callInstance->inCallParams);

			callInstance->state = PRH_TCS_RELEASECOMPLETEPENDING;
		break;

	case PRH_TCS_OVERLAPRECEIVING:
	case PRH_TCS_INCOMINGCALLPROCEEDING:
	case PRH_TCS_CALLRECEIVED:
    /* 
        added PRH_TCS_CONNECTREQUEST for the situation where TL applies to take call, 
        but the call has been allocated to another TL
    */
    case PRH_TCS_CONNECTREQUEST:
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Received RELEASE, call handle=%d", callInstance->callHandle));
        /*callInstance->state = PRH_TCS_RELEASECOMPLETEPENDING;*/
		if(TCSInstance.callbacks->releaseIndication)
			TCSInstance.callbacks->releaseIndication(callInstance->callHandle, callInstance->inCallParams);

		break;

	default:
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Received RELEASE in invalid state!, call handle=%d, call state=%d", callInstance->callHandle,callInstance->state));
		break;
	}
	
	
	return BT_NOERROR;
}


t_api prh_tcs_peer_RELEASECOMPLETE(t_bdaddr address, struct st_t_TCS_callInstance *callInstance)
{

	if(!callInstance)
		return BT_INVALIDPARAM;

	switch(callInstance->state)
	{
	case PRH_TCS_RELEASEREQUEST:
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Received RELEASE COMPLETE, call handle=%d", callInstance->callHandle));
		pTimerCancel(callInstance->timerHandle); /* stop timer T308 */
		callInstance->state = PRH_TCS_TERMRELEASEBEARERPENDING;
		callInstance->timerHandle=0;
		if(TCSInstance.callbacks->callReleaseCompleteIndication)
		    TCSInstance.callbacks->callReleaseCompleteIndication(address, callInstance->callHandle, callInstance->inCallParams);

		prh_tcs_cc_core_freeCallInstance(callInstance);		
		break;

	case PRH_TCS_CALLINITIATED:
        pTimerCancel(callInstance->timerHandle); /* stop timer T303 */
		callInstance->timerHandle=0;
	case PRH_TCS_CALLPRESENT:
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Received RELEASE COMPLETE, call handle=%d", callInstance->callHandle));
		if(TCSInstance.callbacks->callReleaseCompleteIndication)
			TCSInstance.callbacks->callReleaseCompleteIndication(address, callInstance->callHandle, callInstance->inCallParams);
		prh_tcs_cc_core_freeCallInstance(callInstance);
		break;

	default:
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Received RELEASE COMPLETE in invalid state!, call handle=%d", callInstance->callHandle));
		return BT_DISALLOWED;

	}

	return BT_NOERROR;
}


t_api prh_tcs_peer_STARTDTMF(t_bdaddr address, struct st_t_TCS_callInstance *callInstance)
{

	if(!callInstance)
		return BT_INVALIDPARAM;

	/* keypad facility element is mandatory */
	if(!PRH_TCS_CC_PARAM_KEYPADFACILITY_EXISTS(callInstance->inCallParams->bitMask))
	{
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Received START DTMF with invalid Call Parameters, call handle=%d", callInstance->callHandle));
		return BT_NOERROR;
	}

	if(PRH_TCS_ORIGACTIVE == callInstance->state || PRH_TCS_TERMACTIVE == callInstance->state)
	{
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Received START DTMF, call handle=%d", callInstance->callHandle));
		if(TCSInstance.callbacks->startDTMFIndication)
			TCSInstance.callbacks->startDTMFIndication(callInstance->callHandle, callInstance->inCallParams);
	}
	else
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Received START DTMF in invalid state!, call handle=%d", callInstance->callHandle));

	return BT_NOERROR;
}




t_api prh_tcs_peer_STARTDTMFACKNOWLEDGE(t_bdaddr address, struct st_t_TCS_callInstance *callInstance)
{

	if(!callInstance)
		return BT_INVALIDPARAM;

	/* keypad facility element is mandatory */
	if(!PRH_TCS_CC_PARAM_KEYPADFACILITY_EXISTS(callInstance->inCallParams->bitMask))
	{
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Received START DTMF ACKNOWLEDGE with invalid Call Parameters, call handle=%d", callInstance->callHandle));
		return BT_NOERROR;
	}

	if(PRH_TCS_ORIGACTIVE == callInstance->state || PRH_TCS_TERMACTIVE == callInstance->state)
	{
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Received START DTMF ACKNOWLEDGE, call handle=%d", callInstance->callHandle));
		if(TCSInstance.callbacks->startDTMFAckIndication)
			TCSInstance.callbacks->startDTMFAckIndication(callInstance->callHandle, callInstance->inCallParams);
	}
	else
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Received START DTMF ACKNOWLEDGE in invalid state!, call handle=%d", callInstance->callHandle));

	return BT_NOERROR;
}


t_api prh_tcs_peer_STARTDTMFREJECT(t_bdaddr address, struct st_t_TCS_callInstance *callInstance)
{

	if(!callInstance)
		return BT_INVALIDPARAM;

	if(PRH_TCS_ORIGACTIVE == callInstance->state || PRH_TCS_TERMACTIVE == callInstance->state)
	{
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Received START DTMF REJECT, call handle=%d", callInstance->callHandle));
		if(TCSInstance.callbacks->startDTMFRejectIndication)
			TCSInstance.callbacks->startDTMFRejectIndication(callInstance->callHandle, callInstance->inCallParams);
	}
	else
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Received START DTMF REJECT in invalid state!, call handle=%d", callInstance->callHandle));

	return BT_NOERROR;
}


t_api prh_tcs_peer_STOPDTMF(t_bdaddr address, struct st_t_TCS_callInstance *callInstance)
{

	if(!callInstance)
		return BT_INVALIDPARAM;

	/* keypad facility element is mandatory */
	if(!PRH_TCS_CC_PARAM_KEYPADFACILITY_EXISTS(callInstance->inCallParams->bitMask))
	{
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Received STOP DTMF with invalid Call Parameters, call handle=%d", callInstance->callHandle));
		return BT_NOERROR;
	}

	if(PRH_TCS_ORIGACTIVE == callInstance->state || PRH_TCS_TERMACTIVE == callInstance->state)
	{
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Received STOP DTMF, call handle=%d", callInstance->callHandle));
		if(TCSInstance.callbacks->stopDTMFIndication)
		    TCSInstance.callbacks->stopDTMFIndication(callInstance->callHandle, callInstance->inCallParams);
	}
	else
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Received STOP DTMF in invalid state!, call handle=%d", callInstance->callHandle));

	return BT_NOERROR;
}


t_api prh_tcs_peer_STOPDTMFACKNOWLEDGE(t_bdaddr address, struct st_t_TCS_callInstance *callInstance)
{

	if(!callInstance)
		return BT_INVALIDPARAM;

	/* keypad facility element is mandatory */
	if(!PRH_TCS_CC_PARAM_KEYPADFACILITY_EXISTS(callInstance->inCallParams->bitMask))
	{
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Received STOP DTMF ACKNOWLEDGE with invalid Call Parameters, call handle=%d", callInstance->callHandle));
		return BT_NOERROR;
	}

	if(PRH_TCS_ORIGACTIVE == callInstance->state || PRH_TCS_TERMACTIVE == callInstance->state)
	{
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Received STOP DTMF ACKNOWLEDGE, call handle=%d", callInstance->callHandle));
		if(TCSInstance.callbacks->stopDTMFAckIndication)
			TCSInstance.callbacks->stopDTMFAckIndication(callInstance->callHandle, callInstance->inCallParams);
	}
	else
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Received STOP DTMF ACKNOWLEDGE in invalid state!, call handle=%d", callInstance->callHandle));

	return BT_NOERROR;
}

t_api prh_tcs_peer_PROGRESS(t_bdaddr address, struct st_t_TCS_callInstance *callInstance)
{

	if(!callInstance)
		return BT_INVALIDPARAM;

	/* progress indicator element is mandatory */
	if(!PRH_TCS_CC_PARAM_PROGRESSINDICATOR_EXISTS(callInstance->inCallParams->bitMask))
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Received PROGRESS with invalid Call Parameters, call handle=%d", callInstance->callHandle));
	else
	{
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Received PROGRESS, call handle=%d", callInstance->callHandle));
		if(TCSInstance.callbacks->progressIndication)
			TCSInstance.callbacks->progressIndication(callInstance->callHandle, callInstance->inCallParams);
	}

	return BT_NOERROR;
}

#endif /*(PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/
