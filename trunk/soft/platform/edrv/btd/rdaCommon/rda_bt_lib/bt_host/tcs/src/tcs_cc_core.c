/*
 * MODULE NAME:    tcs_cc_core.c
 * DESCRIPTION:    TCS Call Control Core
 * AUTHOR:         Thomas Howley
 * DATE:           09 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_cc_core.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies.
 *     All rights reserved.
 *
 */

#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)

#include "tcs_cc_core.h"
#include "tcs_cc_types.h"
#include "tcs_cc_const.h"
#include "tcs_cc_callbacks.h"
#include "tcs_cc_upper.h"
#include "tcs_cc_enc.h"
#include "tcs_core.h"
#include "tcs_l2cap.h"

#if (PRH_TCS_TERMINAL && PRH_TCS_GATEWAY)
u_int8 tcsMode; 
#endif /* (PRH_TCS_TERMINAL && PRH_TCS_GATEWAY) */

#if (PRH_TCS_TERMINAL && PRH_TCS_GATEWAY)
struct st_t_TCS_TLorGWInstance TCSInstance;
#elif PRH_TCS_TERMINAL
struct st_t_TCS_TLInstance TCSInstance;
#elif PRH_TCS_GATEWAY
struct st_t_TCS_GWInstance TCSInstance;
#endif 


#if PRH_TCS_GATEWAY
static u_int16 callHandleList[PRH_TCS_MAX_CALLS_PER_DEVICE+PRH_TCS_MAXTERMINALSPERGATEWAY];

u_int16 prh_tcs_cc_core_GWgetFreeCallHandle(void);

void prh_tcs_groupAddMember(u_int16 cid, t_bdaddr bdaddr, u_int16 status);
t_pEvent prh_tcs_groupAddMemberEvent;
u_int16 groupAddMemberCid;
t_bdaddr groupAddMemberBdaddr;
volatile u_int16 groupAddMemberStatus;
#endif

t_api prh_tcs_cc_core_setupDefaultOutCallParams(struct st_t_TCS_callInstance *callInstance);
void prh_tcs_cc_core_freeCallInstanceMembers(struct st_t_TCS_callInstance *callInstance);

#if PRH_TCS_TERMINAL 
struct st_t_TCS_callInstance *prh_tcs_cc_core_TLgetNewCallInstance(u_int16 cid, u_int8 callType)
{
	int i;

	for(i=0; i<PRH_TCS_MAX_CALLS_PER_DEVICE; i++)
	{
		if(!TCSInstance.callInstance[i])
		{	
			TCSInstance.callInstance[i] = pMalloc(sizeof(struct st_t_TCS_callInstance));
			if(TCSInstance.callInstance[i])
			{
				prh_tcs_cc_core_initCallInstance(TCSInstance.callInstance[i]);
				TCSInstance.callInstance[i]->callType = callType;
				TCSInstance.callInstance[i]->cid = cid;
				TCSInstance.callInstance[i]->callHandle = i+1; /* callHandle starts at '1' */
			}

			return TCSInstance.callInstance[i];
		}
		else /* the call instance is in use */
		{
			if(cid==0 && TCSInstance.callInstance[i]->cid==0)
			{
				/* the new callInstance is for a broadcast SETUP messasge */
				/* and the existing callInstance is for a broadcast SETUP message */
				/* therefore we have to assume that they are for the same call */
				/* otherwise we would have a different call everytime we received */
				/* the broadcast SETUP message */
				return TCSInstance.callInstance[i];
			}
		
		}
	}

	return NULL;
}


void prh_tcs_cc_core_TLfreeCallInstance(struct st_t_TCS_callInstance *callInstance)
{
	int i;
	struct prh_st_tcs_l2conn *l2conn;


	if(callInstance)
	{
		prh_tcs_lookup_l2cap_inst_by_cid(callInstance->cid, &l2conn);
		if(l2conn)
			l2conn->un.call = NULL;
		for(i=0; i<PRH_TCS_MAX_CALLS_PER_DEVICE; i++)
		{
			if(TCSInstance.callInstance[i] == callInstance)
			{	
				prh_tcs_cc_core_freeCallInstanceMembers(callInstance);
				TCSInstance.callInstance[i] = NULL;
				return;
			}
		}
	}
}
void prh_tcs_cc_core_freeUnassignedBcastCallInstance(struct st_t_TCS_callInstance *callInstance)
{
	int i;

	for(i=0; i<PRH_TCS_MAX_CALLS_PER_DEVICE; i++)
	{
	    if(TCSInstance.callInstance[i] == callInstance)
	    {	
		    prh_tcs_cc_core_freeCallInstanceMembers(callInstance);
		    TCSInstance.callInstance[i] = NULL;
		    return;
	    }
    }
}


struct st_t_TCS_callInstance *prh_tcs_cc_core_TLgetCallInstanceByBearerHandle(u_int16 bearerHandle)
{
	int i;

	for(i=0; i<PRH_TCS_MAX_CALLS_PER_DEVICE; i++)
	{
		if(TCSInstance.callInstance[i])
		{
			if(TCSInstance.callInstance[i]->bearerHandle == bearerHandle)
				return TCSInstance.callInstance[i];
		}
	}

	return NULL;
}



struct st_t_TCS_callInstance *prh_tcs_cc_core_TLgetCallInstanceByBearerRef(u_int16 *bearerRef)
{
	int i;

	for(i=0; i<PRH_TCS_MAX_CALLS_PER_DEVICE; i++)
	{
		if(TCSInstance.callInstance[i])
		{
			if(&(TCSInstance.callInstance[i]->bearerHandle) == bearerRef)
				return TCSInstance.callInstance[i];
		}
	}

	return NULL;
}



/* Search for call Instance based on call handle*/
struct st_t_TCS_callInstance *prh_tcs_cc_core_TLgetCallInstanceByHandle(u_int16 callHandle)
{
	int i;

	for(i=0; i<PRH_TCS_MAX_CALLS_PER_DEVICE; i++)
	{
		if(TCSInstance.callInstance[i])
		{
			if(TCSInstance.callInstance[i]->callHandle == callHandle)
				return TCSInstance.callInstance[i];
		}
	}

	return NULL;
}



/* Search for call Instance based on cid used for call signalling */
struct st_t_TCS_callInstance *prh_tcs_cc_core_TLgetCallInstanceByCID(u_int16 cid)
{
	int i;

	for(i=0; i<PRH_TCS_MAX_CALLS_PER_DEVICE; i++)
	{
		if(TCSInstance.callInstance[i])
		{
			if(TCSInstance.callInstance[i]->cid == cid)
				return TCSInstance.callInstance[i];
		}
	}

	return NULL;
}


t_bdaddr *prh_tcs_cc_core_TLgetBdAddressByCallInstance(struct st_t_TCS_callInstance *callInstance)
{
	struct prh_st_tcs_l2conn *l2conn;

	if(PRH_TCS_CALLTYPE_TCSCORDLESS == callInstance->callType)
		return &(TCSInstance.GWBDAddress);
	else if(PRH_TCS_CALLTYPE_TCSBIN == callInstance->callType)
	{
		prh_tcs_lookup_l2cap_inst_by_cid(callInstance->cid, &l2conn);
		if(l2conn)
			return &(l2conn->bd_addr);
		else 
			return NULL;
	}
	else
		return NULL;

}

u_int8 prh_tcs_cc_core_TL_L2ReleaseRequired(struct st_t_TCS_callInstance *callInstance)
{
	if(PRH_TCS_CALLTYPE_TCSBIN == callInstance->callType)
		return 1;
	
	/* Otherwise, it is a TCSCORDLESS call */
	/* If cid = original l2cap inst to GW, don't release it */
	if(callInstance->cid == TCSInstance.cid)
		return 0;
	else
		return 1;

}

#endif /* PRH_TCS_TERMINAL */





#if PRH_TCS_GATEWAY 


struct st_t_TCS_callInstance *prh_tcs_cc_core_GWaddBCastCallInstance(struct st_t_TCS_TLPeer *TLPeer, u_int16 cid)
{
	if(TLPeer)
	{
		TLPeer->bcastCallInstance = pMalloc(sizeof(struct st_t_TCS_callInstance));
		if(TLPeer->bcastCallInstance)
		{
			prh_tcs_cc_core_initCallInstance(TLPeer->bcastCallInstance);
			TLPeer->bcastCallInstance->cid = cid;
			TLPeer->bcastCallInstance->callType = PRH_TCS_CALLTYPE_PENDINGBCAST;
			TLPeer->bcastCallInstance->callHandle= prh_tcs_cc_core_GWgetFreeCallHandle();
		}
		return TLPeer->bcastCallInstance;
	}
	
	return NULL;
}

struct st_t_TCS_callInstance *prh_tcs_cc_core_GWgetNewCallInstance(struct st_t_TCS_TLPeer *TLPeer, u_int16 cid, u_int16 callType)
{
	int i;
	/* Must also check global number of calls to TLPeers */
	
	for(i=0; i<PRH_TCS_MAX_CALLS_PER_DEVICE; i++)
	{
		if(!(TLPeer->callInstance[i]))
		{
			TLPeer->callInstance[i] = pMalloc(sizeof(struct st_t_TCS_callInstance));
			if(TLPeer->callInstance[i])
			{
				prh_tcs_cc_core_initCallInstance(TLPeer->callInstance[i]);
				TLPeer->callInstance[i]->cid = cid;
				TLPeer->callInstance[i]->callType = callType;
				TLPeer->callInstance[i]->callHandle = prh_tcs_cc_core_GWgetFreeCallHandle();
			}
			return TLPeer->callInstance[i];
		}
	}

	return NULL;
}


/* Search for call Instance based on call handle */
struct st_t_TCS_callInstance *prh_tcs_cc_core_GWgetCallInstanceByHandle(u_int16 callHandle)
{
	int i, j;

	for(i=0; i<PRH_TCS_MAXTERMINALSPERGATEWAY; i++)
	{
		if(TCSInstance.TLPeer[i])
		{
			for(j=0; j<PRH_TCS_MAX_CALLS_PER_DEVICE; j++)
			{		
				if(TCSInstance.TLPeer[i]->callInstance[j])
				{
					if(callHandle == TCSInstance.TLPeer[i]->callInstance[j]->callHandle)
						return TCSInstance.TLPeer[i]->callInstance[j];
				}
			}
		}
	}

	return NULL;
}



/* Search for call Instance based on cid used for call signalling */
struct st_t_TCS_callInstance *prh_tcs_cc_core_GWgetCallInstanceByCID(u_int16 cid)
{
	int i, j;

	for(i=0; i<PRH_TCS_MAXTERMINALSPERGATEWAY; i++)
	{
		if(TCSInstance.TLPeer[i])
		{
			for(j=0; j<PRH_TCS_MAX_CALLS_PER_DEVICE; j++)
			{		
				if(TCSInstance.TLPeer[i]->callInstance[j])
				{
					if(cid == TCSInstance.TLPeer[i]->callInstance[j]->cid)
						return TCSInstance.TLPeer[i]->callInstance[j];
				}
			}
		}
	}

	return NULL;
}




void prh_tcs_cc_core_GWfreeCallInstance(struct st_t_TCS_callInstance *callInstance)
{
	int i, j;
	struct prh_st_tcs_l2conn *l2conn;
	
	if(callInstance)
	{
		prh_tcs_lookup_l2cap_inst_by_cid(callInstance->cid, &l2conn);
		if(l2conn)
			l2conn->un.call = NULL;
		
		for(i=0; i<PRH_TCS_MAXTERMINALSPERGATEWAY; i++)
		{
			if(TCSInstance.TLPeer[i])
			{
				/* Check pending broadcast call instance */
				if(TCSInstance.TLPeer[i]->bcastCallInstance == callInstance)
				{
					/* TBD: Should the callHandle be freed in this case ?? */
					prh_tcs_cc_core_GWfreeCallHandle(callInstance->callHandle);
					prh_tcs_cc_core_freeCallInstanceMembers(callInstance);
					TCSInstance.TLPeer[i]->bcastCallInstance = NULL;
					return;
				}
				
				/* check each active call instance */
				for(j=0; j<PRH_TCS_MAX_CALLS_PER_DEVICE; j++)
				{		
					
					if(TCSInstance.TLPeer[i]->callInstance[j] == callInstance)
					{
						prh_tcs_cc_core_GWfreeCallHandle(callInstance->callHandle);
						prh_tcs_cc_core_freeCallInstanceMembers(callInstance);
						TCSInstance.TLPeer[i]->callInstance[j] = NULL;
						return;
						
					}
				}
			}
		}
	}
}



/* Search for call Instance based on bearer handle */
struct st_t_TCS_callInstance *prh_tcs_cc_core_GWgetCallInstanceByBearerHandle(u_int16 bearerHandle)
{
	int i, j;

	for(i=0; i<PRH_TCS_MAXTERMINALSPERGATEWAY; i++)
	{
		if(TCSInstance.TLPeer[i])
		{
			for(j=0; j<PRH_TCS_MAX_CALLS_PER_DEVICE; j++)
			{
				if(TCSInstance.TLPeer[i]->callInstance[j])
				{
					if(TCSInstance.TLPeer[i]->callInstance[j]->bearerHandle == bearerHandle)
						return TCSInstance.TLPeer[i]->callInstance[j];
				}
			}
		}
	}

	return NULL;
}

/* Search for call Instance based on pointer to bearer handle */
struct st_t_TCS_callInstance *prh_tcs_cc_core_GWgetCallInstanceByBearerRef(u_int16 *bearerRef)
{
	int i, j;

	for(i=0; i<PRH_TCS_MAXTERMINALSPERGATEWAY; i++)
	{
		if(TCSInstance.TLPeer[i])
		{
			for(j=0; j<PRH_TCS_MAX_CALLS_PER_DEVICE; j++)
			{
				if(TCSInstance.TLPeer[i]->callInstance[j])
				{
					if(&(TCSInstance.TLPeer[i]->callInstance[j]->bearerHandle) == bearerRef)
						return TCSInstance.TLPeer[i]->callInstance[j];
				}
			}
		}
	}

	return NULL;
}




struct st_t_TCS_TLPeer *prh_tcs_cc_core_getNewTLPeer(t_bdaddr address, /*u_int8 initState,*/ u_int16 cid)
{
	int i, j;

	for(i=0; i<PRH_TCS_MAXTERMINALSPERGATEWAY; i++)
	{
		if(!(TCSInstance.TLPeer[i]))
		{
			TCSInstance.TLPeer[i] = (struct st_t_TCS_TLPeer *)pMalloc(sizeof(struct st_t_TCS_TLPeer));
			if(!(TCSInstance.TLPeer[i]))
				return NULL;
			
			pMemcpy(&(TCSInstance.TLPeer[i]->bdAddress.bytes), &(address.bytes), 6);
			TCSInstance.TLPeer[i]->cid = cid;
			for(j=0; j<PRH_TCS_MAX_CALLS_PER_DEVICE; j++)
				TCSInstance.TLPeer[i]->callInstance[j] = NULL;
			TCSInstance.TLPeer[i]->bcastCallInstance = NULL;
			return TCSInstance.TLPeer[i];
		}
	}

	return NULL;
}


struct st_t_TCS_TLPeer *prh_tcs_cc_core_getTLPeerByCallInstance(struct st_t_TCS_callInstance *callInstance)
{

	int i, j;

	for(i=0; i<PRH_TCS_MAXTERMINALSPERGATEWAY; i++)
	{
		if(TCSInstance.TLPeer[i])
		{
			/* check pending broadcast call instance */
			if(TCSInstance.TLPeer[i]->bcastCallInstance == callInstance)
				return TCSInstance.TLPeer[i];

			for(j=0; j<PRH_TCS_MAX_CALLS_PER_DEVICE; j++)
			{
				if(TCSInstance.TLPeer[i]->callInstance[j])
				{
					if(TCSInstance.TLPeer[i]->callInstance[j] == callInstance)
						return TCSInstance.TLPeer[i];
				}
			}
		}

	}

	return NULL;
}


struct st_t_TCS_TLPeer *prh_tcs_cc_core_getTLPeerByBdAddress(t_bdaddr address)
{
	int i;

	for(i=0; i<PRH_TCS_MAXTERMINALSPERGATEWAY; i++)
	{
		if(TCSInstance.TLPeer[i])
		{
			if(BD_ADDR_CMP(TCSInstance.TLPeer[i]->bdAddress, address))
				return TCSInstance.TLPeer[i];
		}
	}

	return NULL;
}

u_int8 prh_tcs_cc_core_freeTLPeerByBdAddress(t_bdaddr address)
{
	int i;

	for(i=0; i<PRH_TCS_MAXTERMINALSPERGATEWAY; i++)
		{
			if(TCSInstance.TLPeer[i])
			{
				if(BD_ADDR_CMP(TCSInstance.TLPeer[i]->bdAddress, address))
				{
					pFree(TCSInstance.TLPeer[i]);
					TCSInstance.TLPeer[i] = NULL;
					return 0;
				}
			}
		}
	return 1;
}




t_api prh_tcs_cc_core_broadcastIncomingSETUP(struct st_t_TCS_callParams *callParams)
{

    u_int16 groupCID;
	t_bdaddr *bdAddress;
	t_api status = BT_NOERROR;
    static struct st_t_TCS_callParams *broadcastRepeatcallParams;
	
    prh_tcs_cc_core_freeCallParamMembers(broadcastRepeatcallParams);

	if(TCSInstance.pendingBroadcastSETUP)
		{
		pTimerCancel(TCSInstance.broadcastSETUPtimer);
		TCSInstance.broadcastSETUPtimer=0;
		}
	
	else
		TCSInstance.pendingBroadcastSETUP = 1;
	
	bdAddress = prh_tcs_l2cap_findConnectedTL();
	if(bdAddress)
	{
		status = L2_GroupCreate(PRH_TCS_PSM_TCSCORDLESS, &groupCID);
		if(BT_NOERROR == status)
        {
            prh_tcs_groupAddMemberEvent=pEventCreate(FALSE);
            status = L2_GroupAddMember(groupCID, *bdAddress, prh_tcs_groupAddMember);
        }
        if(BT_PENDING == status)
        {   
            pEventWait(prh_tcs_groupAddMemberEvent);
            if(groupAddMemberStatus == HCI_errNOERROR)
            {
                if (groupAddMemberCid != groupCID || pMemcmp(&groupAddMemberBdaddr,bdAddress,sizeof(t_bdaddr)) != 0x00)
                {
                    return BT_UNKNOWNERROR;
                }                
            }   
            else
            {
                return BT_UNKNOWNERROR;	        
            }

            status = BT_NOERROR;
        }
		if(BT_NOERROR == status)
        {    
            status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_SETUP, callParams, groupCID);
            TCSInstance.broadcastGroupCID = groupCID;
            /* set up the timer that will timeout if there is no reply */
            TCSInstance.broadcastSETUPtimer=pTimerCreate(PRH_TCS_TIMER_T303, prh_tcs_cc_callback_broadcastSETUP_timeout, NULL, pTIMER_ONESHOT); 
            prh_tcs_cc_core_storeCallParams(&broadcastRepeatcallParams, callParams);
            /* setup the timer that will fire and re-transmit the broadcast SETUP message at a period of PRH_TCS_TIMER_BCASTSETUPREPEAT */
            TCSInstance.broadcastRepeatSETUPtimer=pTimerCreate(PRH_TCS_TIMER_BCASTSETUPREPEAT, prh_tcs_cc_callback_bcast_setup_repeat_timeout, (void *)broadcastRepeatcallParams, pTIMER_ONESHOT); 
        }
        
		if(BT_NOERROR == status)
			pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent Broadcast SETUP, group CID=%d", groupCID));

	}
	else
		status = BT_UNKNOWNERROR;
	
	return status;
}


void prh_tcs_groupAddMember(u_int16 cid, t_bdaddr bdaddr, u_int16 status)
{
    
    groupAddMemberStatus=status;
    groupAddMemberCid=cid;
    pMemcpy(&groupAddMemberBdaddr,&bdaddr,sizeof(t_bdaddr));
        
    pEventSet(prh_tcs_groupAddMemberEvent);
    pEventFree(prh_tcs_groupAddMemberEvent);
}

t_bdaddr *prh_tcs_cc_core_GWgetBdAddressByCallInstance(struct st_t_TCS_callInstance *callInstance)
{
	struct prh_st_tcs_l2conn *l2conn;

	prh_tcs_lookup_l2cap_inst_by_cid(callInstance->cid, &l2conn);
	if(l2conn)
		return &(l2conn->bd_addr);
	else
		return NULL;
}



u_int8 prh_tcs_cc_core_GW_L2ReleaseRequired(struct st_t_TCS_callInstance *callInstance)
{
	struct st_t_TCS_TLPeer *TLPeer;

	if(PRH_TCS_CALLTYPE_TCSBIN == callInstance->callType)
		return 1;

	/* Otherwise, it is a TCSCORDLESS call */
	TLPeer = prh_tcs_cc_core_getTLPeerByCallInstance(callInstance);
	/* If cid = original l2cap inst to TL, don't release it */
	if(callInstance->cid == TLPeer->cid)
		return 0;
	else
		return 1;

}



u_int16 prh_tcs_cc_core_GWgetFreeCallHandle(void)
{
	u_int8 i;

	for(i=0; i<PRH_TCS_MAX_CALLS_PER_DEVICE+PRH_TCS_MAXTERMINALSPERGATEWAY; i++)
	{
		if(0 == callHandleList[i])
		{
			callHandleList[i] = 1;
			return i + 1;
		}
	}
	return 0; /* no available callHandles */
}

void prh_tcs_cc_core_GWfreeCallHandle(u_int16 callHandle)
{
	if(callHandle <= PRH_TCS_MAX_CALLS_PER_DEVICE+PRH_TCS_MAXTERMINALSPERGATEWAY)
		callHandleList[callHandle-1] = 0;
}


t_api prh_tcs_cc_core_GW_releaseNonSelectedBCastCalls(void)
{
	int i;
	struct st_t_TCS_callParams callParams;
	struct st_t_TCS_callInstance *callInstance;
	t_api status;

	callParams.bitMask = PRH_TCS_CC_PARAM_CAUSE;
	callParams.cause = PRH_TCS_CAUSE_NONSELECTEDUSERCLEARING;

	for(i=0; i<PRH_TCS_MAXTERMINALSPERGATEWAY; i++)
	{
		if(TCSInstance.TLPeer[i])
		{
			callInstance = TCSInstance.TLPeer[i]->bcastCallInstance;
			if(callInstance)
			{
				callInstance->state = PRH_TCS_DISCONNECTREQUEST;
				status = prh_tcs_cc_encodeMsg(PRH_TCS_MSG_DISCONNECT, &callParams, callInstance->cid);
				if(BT_NOERROR == status)
				{
					pDebugPrintfEX((pLOGDEBUG,pLOGTCS,"TCS: Sent RELEASE, call handle=%d", callInstance->callHandle));
				    callInstance->timerHandle=pTimerCreate(PRH_TCS_TIMER_T308, prh_tcs_cc_callback_timeout, (void *)callInstance, pTIMER_ONESHOT); 
				}
			}
			
		}
	}

	return BT_NOERROR;
}


struct st_t_TCS_TLPeer *prh_tcs_cc_core_GWgetNumConnectedTLPeers(u_int8 *numTLPeers)
{
	u_int8 count = 0;
	struct st_t_TCS_TLPeer *TLPeer = NULL;
	int i;

	for(i=0; i<PRH_TCS_MAXTERMINALSPERGATEWAY; i++)
	{
		if(TCSInstance.TLPeer[i] /* also check if connected */)
		{
			TLPeer = TCSInstance.TLPeer[i];
			count++;
		}
	}

	*numTLPeers = count;
	return TLPeer;
}

#endif /* PRH_TCS_GATEWAY */

t_api prh_tcs_cc_core_storeCallParams(struct st_t_TCS_callParams **destRef, struct st_t_TCS_callParams *src)
{
	struct st_t_TCS_callParams *dest;

	if(!destRef)
		return BT_INVALIDPARAM;

	if(*destRef)
		prh_tcs_cc_core_freeCallParams(*destRef);

	if(!src)
	{
		*destRef = NULL;
		return BT_NOERROR;
	}

	dest = (struct st_t_TCS_callParams *)pMalloc(sizeof(struct st_t_TCS_callParams));
	if(dest)
		pMemcpy(dest, src, sizeof(struct st_t_TCS_callParams));
	else
		return BT_NORESOURCES;

		
	if(dest)
	{
		pMemcpy(dest, src, sizeof(struct st_t_TCS_callParams));
		if(PRH_TCS_CC_PARAM_BEARERCAPABILITY_EXISTS(src->bitMask) && PRH_TCS_BEARER_CAP_LINK_TYPE_ACL == src->linkType && src->aclBearer)
		{
			dest->aclBearer = (struct st_t_TCS_aclBearer *)pMalloc(sizeof(struct st_t_TCS_aclBearer));
			if(dest->aclBearer)
				pMemcpy(dest->aclBearer, src->aclBearer, sizeof(struct st_t_TCS_aclBearer));
			else
				return BT_NORESOURCES;
		}
		
		if(PRH_TCS_CC_PARAM_AUDIOCONTROL_EXISTS(src->bitMask) && src->audioControlInfo)
		{
			dest->audioControlInfo = (u_int8 *)pMalloc(src->audioControlInfoLength);
			if(dest->audioControlInfo)
				pMemcpy(dest->audioControlInfo, src->audioControlInfo, src->audioControlInfoLength);
			else
				return BT_NORESOURCES;
		}

		if(PRH_TCS_CC_PARAM_COMPANYSPECIFIC_EXISTS(src->bitMask) && src->companySpecific)
		{
			dest->companySpecific = (u_int8 *)pMalloc(src->companySpecificLength);
			if(dest->companySpecific)
				pMemcpy(dest->companySpecific, src->companySpecific, src->companySpecificLength);
			else
				return BT_NORESOURCES;
		}

		if(PRH_TCS_CC_PARAM_CALLEDPARTYNUMBER_EXISTS(src->bitMask) && src->calledPartyNumber)
		{
			dest->calledPartyNumber = (u_int8*)pMalloc(src->calledPartyNumberLength); 
			if(dest->calledPartyNumber)
				pMemcpy(dest->calledPartyNumber, src->calledPartyNumber, src->calledPartyNumberLength); 
			else
				return BT_NORESOURCES;
		}

		if(PRH_TCS_CC_PARAM_CALLINGPARTYNUMBER_EXISTS(src->bitMask) && src->callingPartyNumber)
		{
			dest->callingPartyNumber = (u_int8*)pMalloc(src->callingPartyNumberLength);
			if(dest->callingPartyNumber)
				pMemcpy(dest->callingPartyNumber, src->callingPartyNumber, src->callingPartyNumberLength); 
			else
				return BT_NORESOURCES;
		}

		*destRef = dest;
	}

	return BT_NOERROR;
}


void prh_tcs_cc_core_initCallInstance(struct st_t_TCS_callInstance *callInstance)
{
	if(callInstance)
	{
		callInstance->cid = 0;
		callInstance->state = PRH_TCS_IDLE;
		callInstance->bearerHandle = 0x0000;
		callInstance->inCallParams = NULL;
		callInstance->outCallParams = NULL;
	}

}

void prh_tcs_cc_core_freeCallInstanceMembers(struct st_t_TCS_callInstance *callInstance)
{
	prh_tcs_cc_core_freeCallParams(callInstance->inCallParams);
	callInstance->inCallParams = NULL;
	prh_tcs_cc_core_freeCallParams(callInstance->outCallParams);
	callInstance->outCallParams = NULL;
	pFree(callInstance);	
}

void prh_tcs_cc_core_initCallParams(struct st_t_TCS_callParams *callParams)
{

	if(callParams)
	{
		callParams->aclBearer = NULL;
		callParams->audioControlInfo = NULL;
		callParams->audioControlInfoLength = 0;
		callParams->bitMask = 0;
		callParams->calledPartyNumber = NULL;
		callParams->callingPartyNumber = NULL;
		callParams->companySpecific = NULL;
		callParams->companySpecificLength = 0;
	}


}


void prh_tcs_cc_core_freeCallParams(struct st_t_TCS_callParams *callParams)
{
	if(callParams)
	{
		prh_tcs_cc_core_freeCallParamMembers(callParams);
		pFree(callParams);
	}

}

void prh_tcs_cc_core_freeCallParamMembers(struct st_t_TCS_callParams *callParams)
{
	if(callParams)
	{
		if(PRH_TCS_CC_PARAM_CALLEDPARTYNUMBER_EXISTS(callParams->bitMask) && callParams->calledPartyNumber)
			pFree(callParams->calledPartyNumber);
		if(PRH_TCS_CC_PARAM_CALLINGPARTYNUMBER_EXISTS(callParams->bitMask) && callParams->callingPartyNumber)
			pFree(callParams->callingPartyNumber);
		if(PRH_TCS_CC_PARAM_BEARERCAPABILITY_EXISTS(callParams->bitMask) && (PRH_TCS_BEARER_CAP_LINK_TYPE_ACL == callParams->linkType) && callParams->aclBearer)
			pFree(callParams->aclBearer);
		if(PRH_TCS_CC_PARAM_AUDIOCONTROL_EXISTS(callParams->bitMask) && callParams->audioControlInfo)
			pFree(callParams->audioControlInfo);
		if(PRH_TCS_CC_PARAM_COMPANYSPECIFIC_EXISTS(callParams->bitMask) && callParams->companySpecific)
			pFree(callParams->companySpecific);
	}

}


#if (PRH_TCS_TERMINAL && PRH_TCS_GATEWAY)

struct st_t_TCS_callInstance *prh_tcs_cc_core_TLorGWgetCallInstanceByHandle(u_int16 callHandle)
{
	if(PRH_TCS_MODE_TERMINAL == tcsMode)
		return prh_tcs_cc_core_TLgetCallInstanceByHandle(callHandle);
	else
		return prh_tcs_cc_core_GWgetCallInstanceByHandle(callHandle);
}


struct st_t_TCS_callInstance *prh_tcs_cc_core_TLorGWgetCallInstanceByCID(u_int16 cid)
{
	if(PRH_TCS_MODE_TERMINAL == tcsMode)
		return prh_tcs_cc_core_TLgetCallInstanceByCID(cid);
	else
		return prh_tcs_cc_core_GWgetCallInstanceByCID(cid);
}


struct st_t_TCS_callInstance *prh_tcs_cc_core_TLorGWgetCallInstanceByBearerHandle(u_int16 bearerHandle)
{
	if(PRH_TCS_MODE_TERMINAL == tcsMode)
		return prh_tcs_cc_core_TLgetCallInstanceByBearerHandle(bearerHandle);
	else
		return prh_tcs_cc_core_GWgetCallInstanceByBearerHandle(bearerHandle);
}

struct st_t_TCS_callInstance *prh_tcs_cc_core_TLorGWgetCallInstanceByBearerRef(u_int16 *bearerRef)
{
	if(PRH_TCS_MODE_TERMINAL == tcsMode)
		return prh_tcs_cc_core_TLgetCallInstanceByBearerRef(bearerRef);
	else
		return prh_tcs_cc_core_GWgetCallInstanceByBearerRef(bearerRef);
}

void prh_tcs_cc_core_TLorGWfreeCallInstance(struct st_t_TCS_callInstance *callInstance)
{
	if(PRH_TCS_MODE_TERMINAL == tcsMode)
		prh_tcs_cc_core_TLfreeCallInstance(callInstance);
	else
		prh_tcs_cc_core_GWfreeCallInstance(callInstance);
}

t_bdaddr *prh_tcs_cc_core_TLorGWgetBdAddressByCallInstance(struct st_t_TCS_callInstance *callInstance)
{
	if(PRH_TCS_MODE_TERMINAL == tcsMode)
		return prh_tcs_cc_core_TLgetBdAddressByCallInstance(callInstance);
	else
		return prh_tcs_cc_core_GWgetBdAddressByCallInstance(callInstance);
}

u_int8 prh_tcs_cc_core_TLorGW_L2ReleaseRequired(struct st_t_TCS_callInstance *callInstance)
{
	if(PRH_TCS_MODE_TERMINAL == tcsMode)
		return prh_tcs_cc_core_TL_L2ReleaseRequired(callInstance);
	else
		return prh_tcs_cc_core_GW_L2ReleaseRequired(callInstance);
}

#endif /* (PRH_TCS_TERMINAL && PRH_TCS_GATEWAY) */

#endif /*(PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/
