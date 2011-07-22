/*
 * MODULE NAME:    tcs_cc_gateway.c
 * DESCRIPTION:    TCS Call Control for Gateway
 * AUTHOR:         Thomas Howley
 * DATE:           03 April 2001
 *
 * SOURCE CONTROL: $Id: tcs_cc_gateway.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies.
 *     All rights reserved.
 *
 */

#include "host_config.h"

#if PRH_TCS_GATEWAY 

#include "tcs_cc_gateway.h"
#include "tcs_cc_const.h"
#include "tcs_cc_types.h"
#include "tcs_cc_core.h"
#include "tcs_cc_callbacks.h"
#include "tcs_cc_upper.h"
#include "tcs_core.h"
#include "tcs_l2cap.h"

#include "mgr_devdb.h"
#include "mgr_utils.h"

APIDECL1 t_api APIDECL2 TCS_GWRegister(u_int8 flag, struct st_t_TCS_callbacks *callbacks)
{
	t_api status;
	int i;

#if (PRH_TCS_TERMINAL && PRH_TCS_GATEWAY)
	tcsMode = PRH_TCS_MODE_GATEWAY;
#endif

	if(BT_NOERROR != (status = prh_tcs_init(flag)))
		return status;
	
	if(BT_NOERROR != (status = prh_tcs_register_connectionCallbacks(prh_tcs_cc_core_GWconnectIndCallback, prh_tcs_cc_core_GWdisconnectIndCallback)))
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


	for(i=0; i<PRH_TCS_MAXTERMINALSPERGATEWAY; i++)
		TCSInstance.TLPeer[i] = NULL;

	TCSInstance.pendingBroadcastSETUP = 0;
	
	return BT_NOERROR;
}



#if PRH_TCS_ENABLEGATEWAYINTERCOMCALLS
APIDECL1 t_api APIDECL2 TCS_GWIntercomSetup(u_int16 *callHandle, t_bdaddr TLbdAddress, struct st_t_TCS_callParams *callParams)
{
	struct st_t_TCS_callInstance *callInstance;
	struct st_t_TCS_TLPeer *TLPeer;
	struct prh_st_tcs_l2conn *l2conn = NULL;
	t_api status = BT_NOERROR;
	
	/* assign TLPeer to destination Terminal */
	TLPeer = prh_tcs_cc_core_getTLPeerByBdAddress(TLbdAddress);
	if(!TLPeer)
	{
		TLPeer = prh_tcs_cc_core_getNewTLPeer(TLbdAddress, 0);
		if(!TLPeer)
			return BT_NORESOURCES;
	}
	
	/* Setup callInstance for intercom call */
	callInstance = prh_tcs_cc_core_GWgetNewCallInstance(TLPeer, 0, PRH_TCS_CALLTYPE_TCSBIN);
	if(!callInstance)
		return BT_NORESOURCES;
	*callHandle = callInstance->callHandle;

	prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
	
	/* Search for "free" L2CAP channel, use PRH_TCS_PSM_TCSBIN psm in search, if one exists call setup */
	prh_tcs_lookup_free_l2cap_inst(TLbdAddress, PRH_TCS_PSM_TCSBIN, &l2conn);
	if(l2conn)
	{
		callInstance->cid = l2conn->cid;
		l2conn->un.call = callInstance;  /* so that incoming data is redirected to correct call instance */
		return TCS_Setup(callInstance);  /* begin call setup */
	}
	
	/* Need to setup new L2CAP Channel for this intercom call */
	callInstance->state = PRH_TCS_SETUP_SIGCHANPENDING;
	status = prh_tcs_connect_device_control(TLbdAddress, PRH_TCS_PSM_TCSBIN, callInstance, prh_tcs_cc_callback_L2ConnComplete);
	
	return status;
}

#endif /* PRH_TCS_ENABLEGATEWAYINTERCOMCALLS */


APIDECL1 t_api APIDECL2 TCS_GWCordlessSetup(u_int16 *callHandle, t_bdaddr *TLbdAddress, struct st_t_TCS_callParams *callParams)
{
	struct prh_st_tcs_l2conn *l2conn;
	struct st_t_TCS_callInstance *callInstance;
#if !PRH_TCS_SINGLEPOINTCONFIGURATION
	struct st_t_TCS_TLPeer *TLPeer;
#endif

#if !(PRH_TCS_SINGLEPOINTCONFIGURATION)
	u_int8 numTLPeers;
#endif
				

#if PRH_TCS_SINGLEPOINTCONFIGURATION
	/* assign TLPeer to destination Terminal */
	/* Only one TLPeer can exist at a time */
	if(TCSInstance.TLPeer[0])
	{
		/*
		as this is single point configuration. the channel is already
		setup and the TLPeers address is already set.
		*/
		callInstance = prh_tcs_cc_core_GWgetNewCallInstance(TCSInstance.TLPeer[0], 0, PRH_TCS_CALLTYPE_TCSCORDLESS); /* don't know cid value yet */
		if(!callInstance)
			return BT_NORESOURCES;
		*callHandle = callInstance->callHandle;	
		prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
				
		/* Search for "free" L2CAP channel, use PRH_TCS_PSM_TCSCORDLESS psm in search, if one exists call setup */
		prh_tcs_lookup_free_l2cap_inst(TCSInstance.TLPeer[0]->bdAddress, PRH_TCS_PSM_TCSCORDLESS, &l2conn);
		if(l2conn)
		{
			callInstance->cid = l2conn->cid;
			l2conn->un.call = callInstance;  /* so that incoming data is redirected to correct call instance */
			return TCS_Setup(callInstance);  /* begin call setup */
		}
        else
		    return BT_UNKNOWNERROR;
	}
	else
		return prh_tcs_cc_core_broadcastIncomingSETUP(callParams);
	
#else /* Multipoint configuration */
    
	if(!TLbdAddress)
	{
		/* will get the unique TLPeer if only one exists */
		TLPeer = prh_tcs_cc_core_GWgetNumConnectedTLPeers(&numTLPeers);
		if(0 == numTLPeers) /* need at least one connected TLPeer */
			return BT_DISALLOWED; 
	}
	else
		TLPeer = prh_tcs_cc_core_getTLPeerByBdAddress(*TLbdAddress);
    
	if(TLbdAddress || 1 == numTLPeers)
	{
		if(TLPeer)
		{
			/* Set up new call Instance */
			callInstance = prh_tcs_cc_core_GWgetNewCallInstance(TLPeer, 0, PRH_TCS_CALLTYPE_TCSCORDLESS); /* don't know cid value yet */
			if(!callInstance)
				return BT_NORESOURCES;
			*callHandle = callInstance->callHandle;		
			prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
			
			/* Search for "free" L2CAP channel, use PRH_TCS_PSM_TCSCORDLESS psm in search, if one exists call setup */
			prh_tcs_lookup_free_l2cap_inst(*TLbdAddress, PRH_TCS_PSM_TCSCORDLESS, &l2conn);
			if(l2conn)
			{
				callInstance->cid = l2conn->cid;
				l2conn->un.call = callInstance;  /* so that incoming data is redirected to correct call instance */
				return TCS_Setup(callInstance);  /* begin call setup */
			}

		}
		return BT_UNKNOWNERROR;
	}

	/* there are more than one connected TLPeers */
	*callHandle = PRH_TCS_CALLHANDLEUNASSIGNED;
	return prh_tcs_cc_core_broadcastIncomingSETUP(callParams);


#endif /* PRH_TCS_SINGLEPOINTCONFIGURATION */

	return BT_NOERROR;

}

APIDECL1 t_api APIDECL2 TCS_GWDisconnectFromTL(t_bdaddr TLAddress)
{

	struct st_t_TCS_callInstance *currentCallInstance;
	struct st_t_TCS_TLPeer *TLPeer;
	int retVal;

	TLPeer = prh_tcs_cc_core_getTLPeerByBdAddress(TLAddress);
	if(TLPeer)
	{
		currentCallInstance=prh_tcs_cc_core_getCallInstanceByCID(TLPeer->cid);
		if(!currentCallInstance)
		{
			retVal=L2_Disconnect(TLPeer->cid);
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

APIDECL1 t_api APIDECL2 TCS_GWConnect(u_int16 callHandle, struct st_t_TCS_callParams *callParams)
{
	struct st_t_TCS_callInstance *callInstance;

	callInstance = prh_tcs_cc_core_getCallInstanceByHandle(callHandle);
	if(!callInstance)
		return BT_INVALIDPARAM;

	prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
	return TCS_Connect(callInstance);
}
APIDECL1 t_api APIDECL2 TCS_GWReleaseComplete(u_int16 callHandle, struct st_t_TCS_callParams *callParams)
{
	struct st_t_TCS_callInstance *callInstance;

	callInstance = prh_tcs_cc_core_getCallInstanceByHandle(callHandle);
	if(!callInstance)
		return BT_INVALIDPARAM;

	prh_tcs_cc_core_storeCallParams(&(callInstance->outCallParams), callParams);
	return TCS_ReleaseComplete(callInstance);
}
#endif /* PRH_TCS_GATEWAY */

