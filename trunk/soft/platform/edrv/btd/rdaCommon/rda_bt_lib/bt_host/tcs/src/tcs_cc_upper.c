/*
 * MODULE NAME:    tcs_cc_upper.c
 * DESCRIPTION:    TCS Call Control Upper
 * AUTHOR:         Thomas Howley
 * DATE:           09 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_cc_upper.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies.
 *     All rights reserved.
 *
 */

#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)

#include "tcs_core.h"
#include "tcs_cc_upper.h"
#include "tcs_cc_types.h"
#include "tcs_cc_const.h"
#include "tcs_cc_core.h"
#include "tcs_cc_enc.h"
#include "tcs_cc_callbacks.h"
#include "tcs_l2cap.h"


t_api TCS_Setup(struct st_t_TCS_callInstance *callInstance)
{
	t_api status;

	if(!callInstance)
		return BT_INVALIDPARAM;

	if(PRH_TCS_IDLE == callInstance->state || PRH_TCS_SETUP_SIGCHANPENDING == callInstance->state)
	{
		callInstance->state = PRH_TCS_CALLINITIATED;
		status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_SETUP, callInstance->outCallParams, callInstance->cid);
		if(BT_NOERROR == status)
		{
			pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent SETUP, call handle=%d", callInstance->callHandle));
			callInstance->timerHandle=pTimerCreate(PRH_TCS_TIMER_T303, prh_tcs_cc_callback_timeout, (void *)callInstance, pTIMER_ONESHOT); 
		}
		else
			pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send SETUP, call handle=%d", callInstance->callHandle));


	}
	else
		return BT_DISALLOWED;
	
	return status;
}

APIDECL1 t_api APIDECL2 TCS_SetupAcknowledge(u_int16 callHandle, struct st_t_TCS_callParams *callParams)
{
	t_api status=BT_UNSUPPORTED;

	struct st_t_TCS_callInstance *callInstance;
	
	callInstance = prh_tcs_cc_core_getCallInstanceByHandle(callHandle);
	if(!callInstance)
		return BT_INVALIDPARAM;
	
	if(NO_SIGCHANNEL_EXISTS(callInstance))
	{
        pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send SETUPACKNOWLEDGE, call cid=%d", callInstance->cid));		
        return BT_INVALIDPARAM;    
	}
	else
	{	
    
        if(PRH_TCS_CALLPRESENT == callInstance->state)
	    {
            
		    prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
		    callInstance->state = PRH_TCS_OVERLAPRECEIVING;
		    status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_SETUP_ACK, callInstance->outCallParams, callInstance->cid);
		    if(BT_NOERROR == status)
		    {
			    pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent SETUP_ACKNOWLEDGE, call handle=%d", callInstance->callHandle));
			    /* Assuming the called party number is not complete */
			    callInstance->timerHandle=pTimerCreate(PRH_TCS_TIMER_T302, prh_tcs_cc_callback_timeout, (void *)callInstance, pTIMER_ONESHOT); 
		    }
		    else
			    pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send SETUP_ACKNOWLEDGE, call handle=%d", callInstance->callHandle));

	    }
	    else
            return BT_DISALLOWED;
    }
    return status;
}


APIDECL1 t_api APIDECL2 TCS_Information(u_int16 callHandle, struct st_t_TCS_callParams *callParams)
{
	struct st_t_TCS_callInstance *callInstance;
	t_api status;

	callInstance = prh_tcs_cc_core_getCallInstanceByHandle(callHandle);
	if(!callInstance)
		return BT_INVALIDPARAM;


	switch(callInstance->state)
	{
	case PRH_TCS_OVERLAPSENDING:
	case PRH_TCS_OUTGOINGCALLPROCEEDING:
	case PRH_TCS_CALLDELIVERED:
        prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
		status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_INFORMATION, callInstance->outCallParams, callInstance->cid);
		if(BT_NOERROR == status)
		{
			pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent INFORMATION, call handle=%d", callHandle));
			/* no state change */
			/* Assuming the called party number is not complete */
			pTimerCancel(callInstance->timerHandle);
			callInstance->timerHandle=0;
			/* restart timer T304 */
			callInstance->timerHandle=pTimerCreate(PRH_TCS_TIMER_T304, prh_tcs_cc_callback_timeout, (void *)callInstance, pTIMER_ONESHOT); 
		}
		else
			pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send INFORMATION, call handle=%d", callHandle));
		break;

	case PRH_TCS_ORIGACTIVE:
	case PRH_TCS_TERMACTIVE:
        prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
		status=prh_tcs_cc_encodeMsg(PRH_TCS_MSG_INFORMATION, callInstance->outCallParams, callInstance->cid);
		if (BT_NOERROR == status)
			pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent INFORMATION, call handle=%d\n", callHandle));
		else
			pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send INFORMATION, call handle=%d", callHandle));

	  break;

	default:
		return BT_DISALLOWED;
	}


	return BT_NOERROR;

}

APIDECL1 t_api APIDECL2 TCS_CallProceeding(u_int16 callHandle, struct st_t_TCS_callParams *callParams)
{

	struct st_t_TCS_callInstance *callInstance;
	t_api status;

	callInstance = prh_tcs_cc_core_getCallInstanceByHandle(callHandle);
	if(!callInstance)
		return BT_INVALIDPARAM;

	if(NO_SIGCHANNEL_EXISTS(callInstance))
	{
        pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send CALLPROCEEDING, call cid=%d", callInstance->cid));		
        return BT_INVALIDPARAM;    
	}
	else
	{
	    switch(callInstance->state)
	    {
	    
	    case PRH_TCS_OVERLAPRECEIVING:		
		    pTimerCancel(callInstance->timerHandle); /* stop timer T302 */
			callInstance->timerHandle=0;
	    case PRH_TCS_CALLPRESENT:
            prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
		    callInstance->state = PRH_TCS_INCOMINGCALLPROCEEDING;
		    status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_CALL_PROCEEDING, callInstance->outCallParams, callInstance->cid);
		    if(BT_NOERROR == status)
		    {
			    pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent CALL PROCEEDING, call handle=%d", callHandle));
		    }
		    else
			    pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send CALL PROCEEDING, call handle=%d", callHandle));
		    break;

	    default:
		    return BT_DISALLOWED;
	    }
        return status;
    }
	
}

APIDECL1 t_api APIDECL2 TCS_Alerting(u_int16 callHandle, struct st_t_TCS_callParams *callParams)
{
	t_api status=BT_UNSUPPORTED;

	struct st_t_TCS_callInstance *callInstance;
	
	callInstance = prh_tcs_cc_core_getCallInstanceByHandle(callHandle);
	if(!callInstance)
		return BT_INVALIDPARAM;
	
	if(NO_SIGCHANNEL_EXISTS(callInstance))
	{
        pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send ALERTING, call cid=%d", callInstance->cid));		
        return BT_INVALIDPARAM;    
	}
	else
	{	
	    switch(callInstance->state)
	    {
	    case PRH_TCS_OVERLAPRECEIVING:
		    pTimerCancel(callInstance->timerHandle); /* stop timer T302 */
			callInstance->timerHandle=0;
	    case PRH_TCS_CALLPRESENT:
	    case PRH_TCS_INCOMINGCALLPROCEEDING:
            prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
		    callInstance->state = PRH_TCS_CALLRECEIVED;
		    status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_ALERTING, callInstance->outCallParams, callInstance->cid);
		    if(BT_NOERROR == status)
		    {
			    pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent ALERTING, call handle=%d", callInstance->callHandle));
		    }
		    else
			    pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send ALERTING, call handle=%d", callInstance->callHandle));		
		    break;
		    
	    default:
		    return BT_DISALLOWED;
	    }
        return status;
    }

	
	
}

APIDECL1 t_api APIDECL2 TCS_Connect(struct st_t_TCS_callInstance *callInstance)
{
    t_api status=BT_UNSUPPORTED;
    
    if(!callInstance)
        return BT_INVALIDPARAM;
    
    
    switch(callInstance->state)
    {
    case PRH_TCS_OVERLAPRECEIVING:
        pTimerCancel(callInstance->timerHandle); /* stop timer T302 */
		callInstance->timerHandle=0;
    case PRH_TCS_CALLPRESENT:
    case PRH_TCS_INCOMINGCALLPROCEEDING:
    case PRH_TCS_CALLRECEIVED:
        callInstance->state = PRH_TCS_CONNECTREQUEST;
        status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_CONNECT, callInstance->outCallParams, callInstance->cid);
        if(BT_NOERROR == status)
        {
            pDebugPrintfEX((pLOGNOTICE,pLOGTCS,"TCS: Sent CONNECT, call handle=%d", callInstance->cid));
            callInstance->timerHandle=pTimerCreate(PRH_TCS_TIMER_T313, prh_tcs_cc_callback_timeout, (void *)callInstance, pTIMER_ONESHOT); 
        }
        else
            pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send CONNECT, call handle=%d", callInstance->callHandle));		
        break;
        
    default:
        return BT_DISALLOWED;
    }
    return status;
}

APIDECL1 t_api APIDECL2 TCS_ConnectAcknowledge(u_int16 callHandle, struct st_t_TCS_callParams *callParams)
{
	struct st_t_TCS_callInstance *callInstance;
	t_api status = BT_NOERROR;
	t_bdaddr *address;
	

	callInstance = prh_tcs_cc_core_getCallInstanceByHandle(callHandle);
	if(!callInstance)
		return BT_INVALIDPARAM;

	if(PRH_TCS_CONNECTACKPENDING == callInstance->state)
	{
        prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
		callInstance->state = PRH_TCS_TERMACTIVE;
		status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_CONNECT_ACK, callInstance->outCallParams, callInstance->cid);
		if(BT_NOERROR == status)
		{
			pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent CONNECT ACK, call handle=%d", callInstance->callHandle));		
		}
		else
			pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send CONNECT ACK, call handle=%d", callInstance->callHandle));

			
		address = prh_tcs_cc_core_getBdAddressByCallInstance(callInstance);
		if(!address)
			return BT_UNKNOWNERROR;

		if(TCSInstance.callbacks->callCompleteIndication)
			TCSInstance.callbacks->callCompleteIndication(*address, callInstance->callHandle);
	}
	else
		return BT_DISALLOWED;

	return status;
}

APIDECL1 t_api APIDECL2 TCS_Disconnect(u_int16 callHandle, struct st_t_TCS_callParams *callParams)
{
	struct st_t_TCS_callInstance *callInstance;
	t_api status = BT_NOERROR;

	callInstance = prh_tcs_cc_core_getCallInstanceByHandle(callHandle);
	if(!callInstance)
		return BT_INVALIDPARAM;

	if(NO_SIGCHANNEL_EXISTS(callInstance))
	{
        pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send DISCONNECT, call cid=%d", callInstance->cid));		
        return BT_INVALIDPARAM;    
	}
	else
	{		
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
		    /* disconnect from bearer */	
            prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
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

	    default:
		    status=BT_UNSUPPORTED;
		    break;
	    }
	
	    return status;
    }
}


APIDECL1 t_api APIDECL2 TCS_Release(u_int16 callHandle, struct st_t_TCS_callParams *callParams)
{

	struct st_t_TCS_callInstance *callInstance;
	t_api status = BT_NOERROR;

	callInstance = prh_tcs_cc_core_getCallInstanceByHandle(callHandle);
	if(!callInstance)
		return BT_INVALIDPARAM;

	if(PRH_TCS_DISCONNECTINDICATION == callInstance->state)
	{
        prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
		callInstance->state = PRH_TCS_RELEASEREQUEST;
		status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_RELEASE, callInstance->outCallParams, callInstance->cid);
		if(BT_NOERROR == status)
		{
			pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent RELEASE, call handle=%d", callInstance->callHandle));
			callInstance->timerHandle=pTimerCreate(PRH_TCS_TIMER_T308, prh_tcs_cc_callback_timeout, (void *)callInstance, pTIMER_ONESHOT); 
		}
		else
			pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send RELEASE call handle=%d", callInstance->callHandle));	

	}

	return status;
}

#if 0
APIDECL1 t_api APIDECL2 TCS_ReleaseComplete(u_int16 callHandle, struct st_t_TCS_callParams *callParams)
{
	struct st_t_TCS_callInstance *callInstance;
	t_bdaddr *address;
    u_int8 status;

	callInstance = prh_tcs_cc_core_getCallInstanceByHandle(callHandle);
	if(!callInstance)
		return BT_INVALIDPARAM;

	address = prh_tcs_cc_core_getBdAddressByCallInstance(callInstance);
	if(!address)
		return BT_UNKNOWNERROR;

	switch(callInstance->state)
	{
	case PRH_TCS_CALLINITIATED:
	case PRH_TCS_CALLPRESENT:
	case PRH_TCS_RELEASECOMPLETEPENDING:
		prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams),  callParams);
        status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_RELEASE_COMPLETE, callInstance->outCallParams, callInstance->cid);
        if(BT_NOERROR == status)
        {
	        pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent RELEASE COMPLETE, call handle=%d", callInstance->callHandle));
	        if(TCSInstance.callbacks->callReleaseCompleteIndication)
		        TCSInstance.callbacks->callReleaseCompleteIndication(*address, callInstance->callHandle, callInstance->inCallParams);
#if 0	        
	        /* May have to release the L2CAP signalling channel used for the call */
            /* if disconnecting the disconnectInd will in turn free the callInstacnce */
	        if(prh_tcs_cc_core_L2ReleaseRequired(callInstance))
		        L2_Disconnect(callInstance->cid);
#endif     
	            prh_tcs_cc_core_freeCallInstance(callInstance);

        }
        else
	        pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send RELEASE COMPLETE call handle=%d", callInstance->callHandle));	
        
        return status;

		break;

	case PRH_TCS_RELEASEREQUEST:
		break;
	}

	return BT_NOERROR;
}
#endif
APIDECL1 t_api APIDECL2 TCS_ReleaseComplete(struct st_t_TCS_callInstance *callInstance)
{
	t_bdaddr *address;

    t_api status=BT_UNSUPPORTED;
    
    if(!callInstance)
        return BT_INVALIDPARAM;
       
	address = prh_tcs_cc_core_getBdAddressByCallInstance(callInstance);
	if(!address)
		return BT_UNKNOWNERROR;

	switch(callInstance->state)
	{
	case PRH_TCS_CALLINITIATED:
	case PRH_TCS_CALLPRESENT:
	case PRH_TCS_RELEASECOMPLETEPENDING:
        status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_RELEASE_COMPLETE, callInstance->outCallParams, callInstance->cid);
        if(BT_NOERROR == status)
        {
	        pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent RELEASE COMPLETE, call handle=%d", callInstance->callHandle));
	        if(TCSInstance.callbacks->callReleaseCompleteIndication)
		        TCSInstance.callbacks->callReleaseCompleteIndication(*address, callInstance->callHandle, callInstance->inCallParams);
#if 0	        
	        /* May have to release the L2CAP signalling channel used for the call */
            /* if disconnecting the disconnectInd will in turn free the callInstacnce */
	        if(prh_tcs_cc_core_L2ReleaseRequired(callInstance))
		        L2_Disconnect(callInstance->cid);
#endif     
	            prh_tcs_cc_core_freeCallInstance(callInstance);

        }
        else
	        pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send RELEASE COMPLETE call handle=%d", callInstance->callHandle));	
        
        return status;

		break;

	case PRH_TCS_RELEASEREQUEST:
		break;
	}

	return BT_NOERROR;
}
APIDECL1 t_api APIDECL2 TCS_StartDTMF(u_int16 callHandle, struct st_t_TCS_callParams *callParams)
{
	struct st_t_TCS_callInstance *callInstance;
	t_api status;

	callInstance = prh_tcs_cc_core_getCallInstanceByHandle(callHandle);
	if(!callInstance)
		return BT_INVALIDPARAM;

	/* keypad facility element is mandatory */
	if(!PRH_TCS_CC_PARAM_KEYPADFACILITY_EXISTS(callParams->bitMask))
		return BT_INVALIDPARAM;

	if(PRH_TCS_ORIGACTIVE == callInstance->state || PRH_TCS_TERMACTIVE == callInstance->state)
	{
        prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
		/* no change of state for transmission/reception of DTMF messages */
		status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_START_DTMF, callInstance->outCallParams, callInstance->cid);
		if(BT_NOERROR == status)
			pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent START DTMF, call handle=%d", callInstance->callHandle));
		else
			pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send START DTMF call handle=%d", callInstance->callHandle));
	}
	else
		return BT_DISALLOWED;

	return BT_NOERROR;

}


APIDECL1 t_api APIDECL2 TCS_StartDTMFAcknowledge(u_int16 callHandle, struct st_t_TCS_callParams *callParams)
{
	struct st_t_TCS_callInstance *callInstance;
	t_api status;

	callInstance = prh_tcs_cc_core_getCallInstanceByHandle(callHandle);
	if(!callInstance)
		return BT_INVALIDPARAM;

	/* keypad facility element is mandatory */
	if(!PRH_TCS_CC_PARAM_KEYPADFACILITY_EXISTS(callParams->bitMask))
		return BT_INVALIDPARAM;

	if(PRH_TCS_ORIGACTIVE == callInstance->state || PRH_TCS_TERMACTIVE == callInstance->state)
	{   
        prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
		/* no change of state for transmission/reception of DTMF messages */
		status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_START_DTMF_ACK, callInstance->outCallParams, callInstance->cid);
		if(BT_NOERROR == status)
			pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent START DTMF ACKNOWLEDGE, call handle=%d", callInstance->callHandle));
		else
			pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send START DTMF ACKNOWLEDGE call handle=%d", callInstance->callHandle));
	}
	else
		return BT_DISALLOWED;

	return BT_NOERROR;
}


APIDECL1 t_api APIDECL2 TCS_StartDTMFReject(u_int16 callHandle, struct st_t_TCS_callParams *callParams)
{
	struct st_t_TCS_callInstance *callInstance;
	t_api status;

	callInstance = prh_tcs_cc_core_getCallInstanceByHandle(callHandle);
	if(!callInstance)
		return BT_INVALIDPARAM;

	if(PRH_TCS_ORIGACTIVE == callInstance->state || PRH_TCS_TERMACTIVE == callInstance->state)
	{
        prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
		/* no change of state for transmission/reception of DTMF messages */
		status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_START_DTMF_REJ, callInstance->outCallParams, callInstance->cid);
		if(BT_NOERROR == status)
			pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent START DTMF REJECT, call handle=%d", callInstance->callHandle));
		else
			pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send START DTMF REJECT call handle=%d", callInstance->callHandle));
	}
	else
		return BT_DISALLOWED;

	return BT_NOERROR;
}



APIDECL1 t_api APIDECL2 TCS_StopDTMF(u_int16 callHandle, struct st_t_TCS_callParams *callParams)
{
	struct st_t_TCS_callInstance *callInstance;
	t_api status;

	callInstance = prh_tcs_cc_core_getCallInstanceByHandle(callHandle);
	if(!callInstance)
		return BT_INVALIDPARAM;

	/* keypad facility element is mandatory */
	if(!PRH_TCS_CC_PARAM_KEYPADFACILITY_EXISTS(callParams->bitMask))
		return BT_INVALIDPARAM;

	if(PRH_TCS_ORIGACTIVE == callInstance->state || PRH_TCS_TERMACTIVE == callInstance->state)
	{
        prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
		/* no change of state for transmission/reception of DTMF messages */
		status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_STOP_DTMF, callInstance->outCallParams, callInstance->cid);
		if(BT_NOERROR == status)
			pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent STOP DTMF, call handle=%d", callInstance->callHandle));
		else
			pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send STOP DTMF call handle=%d", callInstance->callHandle));
	}
	else
		return BT_DISALLOWED;

	return BT_NOERROR;
}


APIDECL1 t_api APIDECL2 TCS_StopDTMFAcknowledge(u_int16 callHandle, struct st_t_TCS_callParams *callParams)
{
	struct st_t_TCS_callInstance *callInstance;
	t_api status;

	callInstance = prh_tcs_cc_core_getCallInstanceByHandle(callHandle);
	if(!callInstance)
		return BT_INVALIDPARAM;

	/* keypad facility element is mandatory */
	if(!PRH_TCS_CC_PARAM_KEYPADFACILITY_EXISTS(callParams->bitMask))
		return BT_INVALIDPARAM;

	if(PRH_TCS_ORIGACTIVE == callInstance->state || PRH_TCS_TERMACTIVE == callInstance->state)
	{
        prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
		/* no change of state for transmission/reception of DTMF messages */
		status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_STOP_DTMF_ACK, callInstance->outCallParams, callInstance->cid);
		if(BT_NOERROR == status)
			pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent STOP DTMF ACKNOWLEDGE, call handle=%d", callInstance->callHandle));
		else
			pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send STOP DTMF ACKNOWLEDGE call handle=%d", callInstance->callHandle));
	}
	else
		return BT_DISALLOWED;

	return BT_NOERROR;
}


APIDECL1 t_api APIDECL2 TCS_Progress(u_int16 callHandle, struct st_t_TCS_callParams *callParams)
{
	struct st_t_TCS_callInstance *callInstance;
	t_api status;

	callInstance = prh_tcs_cc_core_getCallInstanceByHandle(callHandle);
	if(!callInstance)
		return BT_INVALIDPARAM;

	/* progress indicator element is mandatory */
	if(!PRH_TCS_CC_PARAM_PROGRESSINDICATOR_EXISTS(callParams->bitMask))
		return BT_INVALIDPARAM;
    prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
	status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_PROGRESS, callInstance->outCallParams, callInstance->cid);
	if(BT_NOERROR == status)
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent PROGRESS, call handle=%d", callInstance->callHandle));
	else
		pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS Error: Attempted to Send PROGRESS call handle=%d", callInstance->callHandle));


	return BT_NOERROR;
}
#endif /*(PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/
