/*
 * MODULE NAME:    tcs_cc_terminal.c
 * DESCRIPTION:    TCS Call Control for Terminal
 * AUTHOR:         Thomas Howley
 * DATE:           03 April 2001
 *
 * SOURCE CONTROL: $Id: tcs_cc_terminal.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies.
 *     All rights reserved.
 *
 */


#include "host_config.h"

#if PRH_TCS_TERMINAL


#include "tcs_cc_terminal.h"
#include "tcs_cc_const.h"
#include "tcs_cc_types.h"
#include "tcs_cc_core.h"
#include "tcs_cc_callbacks.h"
#include "tcs_core.h"
#include "tcs_l2cap.h"

#include "mgr_utils.h"

void (*registerCompleteCB)(t_api status) = NULL;
void (*connectToTLCompleteCB)(t_bdaddr address, t_api status) = NULL;
void (*connectToGWCompleteCB)(t_bdaddr address, t_api status) = NULL;

t_api prh_tcs_cc_terminal_getFreeSigChan(struct st_t_TCS_callInstance *callInstance, u_int8 pendingState, struct st_t_TCS_callParams *callParams);

APIDECL1 t_api APIDECL2 TCS_TLRegister(u_int8 flag, struct st_t_TCS_callbacks *callbacks, t_bdaddr *GWBDAddress, u_int8 connectToGW, void (*callback)(t_api status))
{
	t_api status;
	int i;


#if (PRH_TCS_TERMINAL && PRH_TCS_GATEWAY)
	tcsMode = PRH_TCS_MODE_TERMINAL;
#endif

	
	if( !(PRH_TCS_ENABLE_TCSCORDLESS(flag)) && !(PRH_TCS_ENABLE_TCSBIN(flag)) )
		return BT_INVALIDPARAM;

	/* Use flags to register for TCSBIN and TCSCORDLESS psms */
	if(BT_NOERROR != (status = prh_tcs_init(flag)))
		return status;

	if(BT_NOERROR != (status = prh_tcs_register_connectionCallbacks(prh_tcs_cc_core_TLconnectIndCallback, prh_tcs_cc_core_TLdisconnectIndCallback)))
		return status;

	if(!callbacks)
		return BT_INVALIDPARAM;
	
	/* Currently only allowing one TL instance, so return BT_NORESOURCES */
	if(TCSInstance.callbacks)
		return BT_NORESOURCES;
	
	TCSInstance.callbacks = (struct st_t_TCS_callbacks*)pMalloc(sizeof(struct st_t_TCS_callbacks));
	if(!TCSInstance.callbacks)
		return BT_NORESOURCES;
	pMemcpy(TCSInstance.callbacks, (struct st_t_TCS_callbacks*)callbacks, sizeof(struct st_t_TCS_callbacks));


	if(PRH_TCS_ENABLE_TCSCORDLESS(flag))
	{
		/* For receiving broadcast data from GW */
		status = L2_EnableConnectionlessTraffic(PRH_TCS_PSM_TCSCORDLESS);
        if(status != BT_NOERROR)
        {
            /* 4 now take no action as we still want to continue even though
                we can't get broadcast info. may want to review this when
                a definite scheme (re broadcast)for call setup is in place
                ie if broadcast all the time then no point in continuing here*/
        }

		if(GWBDAddress)
			pMemcpy(&(TCSInstance.GWBDAddress.bytes), GWBDAddress->bytes, 6);
		for(i=0; i<PRH_TCS_MAX_CALLS_PER_DEVICE; i++)
			TCSInstance.callInstance[i] = NULL;

		if(connectToGW)
		{
			if(!callback)
				return BT_INVALIDPARAM;

			registerCompleteCB = callback; 
			/* NULL is passed as callInstance because no call is being initiated */
			prh_tcs_connect_device_control(TCSInstance.GWBDAddress, PRH_TCS_PSM_TCSCORDLESS, NULL, prh_tcs_cc_callback_L2ConnComplete);
			return BT_PENDING;
		}

	}
	return BT_NOERROR;
}
APIDECL1 t_api APIDECL2 TCS_TLConnect(u_int16 callHandle, struct st_t_TCS_callParams *callParams)
{
	t_api status = BT_NOERROR;
	struct st_t_TCS_callInstance *callInstance;
	
	callInstance = prh_tcs_cc_core_getCallInstanceByHandle(callHandle);
	if(!callInstance)
		return BT_INVALIDPARAM;
	
	if(NO_SIGCHANNEL_EXISTS(callInstance))
	{   
        /*
            This may be in response to a Broadcast message and even though the bearer
                channel will have been setup, TCS will not know the cid, so we must
                find it.
        */        
        /* clear the timer that was set after the last SETUP message was rx'd */
        pTimerCancel(callInstance->timerHandle);
		callInstance->timerHandle=0;
        prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
		if(BT_NOERROR == (status = prh_tcs_cc_terminal_getFreeSigChan(callInstance, PRH_TCS_CONNECT_SIGCHANPENDING, callInstance->outCallParams)))
			return TCS_Connect(callInstance);
		else
			return status;
	}
	else
	{
		prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
		return TCS_Connect(callInstance);
	}
}
APIDECL1 t_api APIDECL2 TCS_TLReleaseComplete(u_int16 callHandle, struct st_t_TCS_callParams *callParams)
{
	t_api status = BT_NOERROR;
	struct st_t_TCS_callInstance *callInstance;
	
	callInstance = prh_tcs_cc_core_getCallInstanceByHandle(callHandle);
	if(!callInstance)
		return BT_INVALIDPARAM;
	
	if(NO_SIGCHANNEL_EXISTS(callInstance))
	{   
        /*
            This may be in response to a Broadcast message and even though the bearer
                channel will have been setup, TCS will not know the cid, so we must
                find it.
        */
        /* clear the timer that was set after the last SETUP message was rx'd */
        pTimerCancel(callInstance->timerHandle);
		callInstance->timerHandle=0;
        prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
		if(BT_NOERROR == (status = prh_tcs_cc_terminal_getFreeSigChan(callInstance, PRH_TCS_RELEASECOMPLETE_SIGCHANPENDING, callInstance->outCallParams)))
			return TCS_ReleaseComplete(callInstance);
		else
			return status;
	}
	else
	{
		prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
		return TCS_ReleaseComplete(callInstance);
	}
}

APIDECL1 t_api APIDECL2 TCS_TLConnectToTL(t_bdaddr TLbdAddress, void (*callbackFunc)(t_bdaddr address, t_api status))
{
	if(connectToTLCompleteCB)
		return BT_UNKNOWNERROR;

	connectToTLCompleteCB = callbackFunc;
	prh_tcs_connect_device_control(TLbdAddress, PRH_TCS_PSM_TCSBIN, NULL, prh_tcs_cc_callback_L2ConnComplete);
	
	return BT_PENDING;
}


APIDECL1 t_api APIDECL2	TCS_TLConnectToGW(t_bdaddr GWbdAddress, void (*callbackFunc)(t_bdaddr address, t_api status))
{
	if(connectToGWCompleteCB)
		return BT_UNKNOWNERROR;
	
	/* only allow one GW to be connected */
	if(0 != TCSInstance.cid)
		return BT_DISALLOWED;

	connectToGWCompleteCB = callbackFunc;
	pMemcpy(&(TCSInstance.GWBDAddress.bytes), &(GWbdAddress.bytes), 6);
	prh_tcs_connect_device_control(GWbdAddress, PRH_TCS_PSM_TCSCORDLESS, NULL, prh_tcs_cc_callback_L2ConnComplete);

	return BT_PENDING;
}

APIDECL1 t_api APIDECL2 TCS_TLIntercomSetup(u_int16 *callHandle, t_bdaddr TLbdAddress, struct st_t_TCS_callParams *callParams)
{
	struct st_t_TCS_callInstance *callInstance;
	struct prh_st_tcs_l2conn *l2conn = NULL;

	callInstance = prh_tcs_cc_core_TLgetNewCallInstance(0, PRH_TCS_CALLTYPE_TCSBIN);
	if(!callInstance)
		return BT_NORESOURCES;

	*callHandle = callInstance->callHandle;

	prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);

	/* Search for "free" L2CAP channel, use PRH_TCS_PSM_TCSCORDLESS psm in search, if one exists call setup */
	prh_tcs_lookup_free_l2cap_inst(TLbdAddress, PRH_TCS_PSM_TCSBIN, &l2conn);
	if(l2conn)
	{
		callInstance->cid = l2conn->cid;
		l2conn->un.call = callInstance;  /* so that incoming data is redirected to correct call instance */
		return TCS_Setup(callInstance);  /* begin call setup */
	}
    else
        return BT_UNSUPPORTED;     
}


APIDECL1 t_api APIDECL2 TCS_TLCordlessSetup(u_int16 *callHandle, struct st_t_TCS_callParams *callParams)
{
	t_api status = BT_NOERROR;
	struct st_t_TCS_callInstance *callInstance;
	struct prh_st_tcs_l2conn *l2conn = NULL;
	t_api retStatus;

	callInstance = prh_tcs_cc_core_TLgetNewCallInstance(0, PRH_TCS_CALLTYPE_TCSCORDLESS);
	if(!callInstance)
		return BT_NORESOURCES;

	*callHandle = callInstance->callHandle;
	prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);

	/* Search for "free" L2CAP channel, use PRH_TCS_PSM_TCSCORDLESS psm in search, if one exists call setup */
	retStatus = prh_tcs_lookup_free_l2cap_inst(TCSInstance.GWBDAddress, PRH_TCS_PSM_TCSCORDLESS, &l2conn);
	if(!retStatus && l2conn)
	{	
		callInstance->cid = l2conn->cid;
		l2conn->un.call = callInstance; /* so that incoming data is redirected to correct call instance */
		status = TCS_Setup(callInstance);
        return status;
	}
    else
    {
        return BT_UNSUPPORTED;
    }



}

APIDECL1 t_api APIDECL2 TCS_TLRegisterRecall(u_int16 callHandle)
{
	struct st_t_TCS_callParams callParams;
	
	callParams.bitMask = PRH_TCS_CC_PARAM_KEYPADFACILITY;
	callParams.keypadFacility = PRH_TCS_CHARSET_REGISTER_RECALL;
	return TCS_Information(callHandle, &callParams);
}


t_api prh_tcs_cc_terminal_getFreeSigChan(struct st_t_TCS_callInstance *callInstance, u_int8 pendingState, struct st_t_TCS_callParams *callParams)
{
	struct prh_st_tcs_l2conn *l2conn = NULL;
	t_api retStatus;

	/* Search for "free" L2CAP channel, use PRH_TCS_PSM_TCSCORDLESS psm in search, if one exists ack the setup */
	retStatus = prh_tcs_lookup_free_l2cap_inst(TCSInstance.GWBDAddress, PRH_TCS_PSM_TCSCORDLESS, &l2conn);
	if(!retStatus && l2conn)
	{	
		callInstance->cid = l2conn->cid;
		l2conn->un.call = callInstance; /* so that incoming data is redirected to correct call instance */
		return BT_NOERROR;
	}
    else
        return BT_UNSUPPORTED;

}

APIDECL1 t_api APIDECL2 TCS_TLDisconnectFromGWorTL(void)
{
	int retVal;
	struct st_t_TCS_callInstance *currentCallInstance;
	
	if(TCSInstance.cid) 
	{
		currentCallInstance=prh_tcs_cc_core_getCallInstanceByCID(TCSInstance.cid);
		if(!currentCallInstance)
		{
			retVal=L2_Disconnect(TCSInstance.cid);
			return retVal;
		}
		else
		{
			return BT_DISALLOWED;
		}
	}
	else
	{
		return BT_DISALLOWED;
	}

}

#endif /* PRH_TCS_TERMINAL */
