/*
 * MODULE NAME:    tcs_cc_types.h
 * DESCRIPTION:    TCS Call Control Types Header File
 * AUTHOR:         Thomas Howley
 * DATE:           05 March 2001
 *
 * SOURCE CONTROL: $Id: tcs_cc_types.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001 Parthus Technologies.
 *     All rights reserved.
 *
 */

#ifndef __TCS_CC_TYPES_H
#define __TCS_CC_TYPES_H

#include "host_config.h"

#if (PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)

#include "tcs_lower_layer.h"
#include "tcs_const.h"
#include "tcs_cc_const.h"

typedef struct st_t_TCS_aclBearer{
	t_L2_ChannelFlowSpec qos;
	u_int8 layerInfo; /* Bits 7-4: Layer 3 | Bits 3-0: Layer 2 */
}t_TCS_aclBearer;



typedef struct st_t_TCS_callParams {
	u_int16 bitMask;
	u_int8 callClass;
	/* Bearer Capability */
	u_int8 linkType; 
	/* union { */
	u_int8 scoPacketType;
	t_TCS_aclBearer *aclBearer;
	/* }; */
	u_int8 cause;
	u_int8 progressIndicator;
	u_int8 signal;
	u_int8 keypadFacility;
	/* Calling Party Number */
	u_int8 callingPartyNumberType;
	u_int8 callingPartyPresScreenIndicator;
	u_int8 callingPartyNumberLength;
	u_int8 *callingPartyNumber;
	/* Called Party Number */
	u_int8 calledPartyNumberType;
	u_int8 calledPartyNumberLength;
	u_int8 *calledPartyNumber;
	/* Audio Control */
	u_int8 audioControlInfoLength;
	u_int8 *audioControlInfo;
	/* Bearer Handle */
	/* union { */
	u_int16 scoHandle;
	u_int16 cid;
	/* }; */
	/* Company Specific Info */
	u_int16 companyID;
	u_int8 companySpecificLength;
	u_int8 *companySpecific;
}t_TCS_callParams;


typedef struct st_t_TCS_callInstance {
	u_int16 callHandle;
	u_int16 cid; 
	u_int8 state;
	u_int16 callType; /* PRH_TCS_CALLTYPE_TCSBIN | PRH_TCS_CALLTYPE_TCSCORDLESS */
	t_pTimer timerHandle;
	u_int16 bearerHandle; /* either SCO handle, or L2CAP cid */
	struct st_t_TCS_callParams *outCallParams;
	struct st_t_TCS_callParams *inCallParams;
}t_TCS_callInstance;

/* Give the application this much information about the wugMember
   he needs no more -- *NOTE* these must be in the same order as in 
   tcs_wug.h. */
typedef struct st_t_TCS_wugMember {
    t_bdaddr BDAddress;
    u_int8 internal_num[2];
} t_TCS_wugMember;

typedef struct st_t_TCS_callbacks{
	void (*callSetupIndication)(t_bdaddr bdAddress, u_int16 callHandle, u_int16 callType, struct st_t_TCS_callParams *callParams);
	void (*setupAckIndication)(t_bdaddr bdAddress, u_int16 callHandle, struct st_t_TCS_callParams *callParams);
	void (*informationIndication)(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
	void (*callProceedingIndication)(t_bdaddr bdAddress, u_int16 callHandle, struct st_t_TCS_callParams *callParams);
	void (*alertingIndication)(t_bdaddr bdAddress, u_int16 callHandle, struct st_t_TCS_callParams *callParams);
	void (*connectIndication)(t_bdaddr bdAddress, u_int16 callHandle, struct st_t_TCS_callParams *callParams);
	void (*progressIndication)(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
	void (*callCompleteIndication)(t_bdaddr address, u_int16 callHandle);
	void (*disconnectIndication)(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
	void (*releaseIndication)(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
	void (*callReleaseCompleteIndication)(t_bdaddr address, u_int16 callHandle, struct st_t_TCS_callParams *callParams);
	void (*sigChannelConnectIndication)(t_bdaddr bdAddress, u_int16 psm, u_int16 ACLHandle);
	void (*sigChannelDisconnectIndication)(t_bdaddr bdAddress, u_int16 psm);
	void (*CLInfoIndication)(struct st_t_TCS_callParams *callParams);

	void (*startDTMFIndication)(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
	void (*startDTMFAckIndication)(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
	void (*startDTMFRejectIndication)(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
	void (*stopDTMFIndication)(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
	void (*stopDTMFAckIndication)(u_int16 callHandle, struct st_t_TCS_callParams *callParams);
	void (*accessRightsResponse)(u_int8 accept);
	int (*accessRightsRequest)(t_TCS_wugMember *wugMember);
	void (*listenSuggest)(void);
	int (*newWugInformationIndication)(t_TCS_wugMember **list, int num_entries);  
	int (*listenReject)(u_int8 cause);
	int (*listenAccept)(void);
	
}t_TCS_callbacks;


typedef struct st_t_TCS_TLPeer {
	t_bdaddr bdAddress;
	u_int16 cid; /* signalling channel */
	struct st_t_TCS_callInstance *callInstance[PRH_TCS_MAX_CALLS_PER_DEVICE];
	struct st_t_TCS_callInstance *bcastCallInstance;
}t_TCS_TLPeer;



#if (PRH_TCS_TERMINAL && PRH_TCS_GATEWAY)


typedef struct st_t_TCS_TLorGWInstance{
	struct st_t_TCS_callbacks *callbacks;
/* Terminal */
	struct st_t_TCS_callInstance *callInstance[PRH_TCS_MAX_CALLS_PER_DEVICE];
	/*u_int8 state; */
	u_int16 cid; /* signalling channel */
	t_bdaddr GWBDAddress;
/* Gateway */
	struct st_t_TCS_TLPeer *TLPeer[PRH_TCS_MAXTERMINALSPERGATEWAY];
	u_int8 pendingBroadcastSETUP;
	t_pTimer broadcastSETUPtimer;
    t_pTimer broadcastRepeatSETUPtimer;
    u_int16 broadcastGroupCID;
}t_TCS_TLorGWInstance;

#elif PRH_TCS_TERMINAL

typedef struct st_t_TCS_TLInstance {
	struct st_t_TCS_callbacks *callbacks;
	struct st_t_TCS_callInstance *callInstance[PRH_TCS_MAX_CALLS_PER_DEVICE];
	/* u_int8 state; */
	u_int16 cid; /* signalling channel */
	t_bdaddr GWBDAddress; /* Assigned by TCS_TLRegister */
}t_TCS_TLInstance;

#elif PRH_TCS_GATEWAY

typedef struct st_t_TCS_GWInstance {
	struct st_t_TCS_callbacks *callbacks;
	struct st_t_TCS_TLPeer *TLPeer[PRH_TCS_MAXTERMINALSPERGATEWAY];
	u_int8 pendingBroadcastSETUP;
	t_pTimer broadcastSETUPtimer;
    t_pTimer broadcastRepeatSETUPtimer;
    u_int16 broadcastGroupCID;
}t_TCS_GWInstance;


#endif










/* TCS Call Control Parameter Bit Masks */

#define PRH_TCS_CC_PARAM_CALLCLASS          0x0001
#define PRH_TCS_CC_PARAM_SENDINGCOMPLETE    0x0002
#define PRH_TCS_CC_PARAM_BEARERCAPABILITY   0x0004
#define PRH_TCS_CC_PARAM_CAUSE              0x0008
#define PRH_TCS_CC_PARAM_PROGRESSINDICATOR  0x0010
#define PRH_TCS_CC_PARAM_SIGNAL             0x0020
#define PRH_TCS_CC_PARAM_KEYPADFACILITY     0x0040
#define PRH_TCS_CC_PARAM_CALLINGPARTYNUMBER 0x0080
#define PRH_TCS_CC_PARAM_CALLEDPARTYNUMBER  0x0100
#define PRH_TCS_CC_PARAM_AUDIOCONTROL       0x0200
/*#define PRH_TCS_CC_PARAM_BEARERHANDLE       0x0800 */ /* SCOHANDLE or L2CAPCID */
#define PRH_TCS_CC_PARAM_SCOHANDLE          0x0400
#define PRH_TCS_CC_PARAM_DESTCID            0x0800
#define PRH_TCS_CC_PARAM_COMPANYSPECIFIC    0x1000

#define PRH_TCS_CC_BEARERHANDLETYPE_MASK    0x0400 

#define PRH_TCS_CC_PARAM_CALLCLASS_EXISTS(X)          ((X) & PRH_TCS_CC_PARAM_CALLCLASS)
#define PRH_TCS_CC_PARAM_SENDINGCOMPLETE_EXISTS(X)    (((X) & PRH_TCS_CC_PARAM_SENDINGCOMPLETE) >> 1)
#define PRH_TCS_CC_PARAM_BEARERCAPABILITY_EXISTS(X)   (((X) & PRH_TCS_CC_PARAM_BEARERCAPABILITY) >> 2)
#define PRH_TCS_CC_PARAM_CAUSE_EXISTS(X)              (((X) & PRH_TCS_CC_PARAM_CAUSE) >> 3)
#define PRH_TCS_CC_PARAM_PROGRESSINDICATOR_EXISTS(X) (((X) & PRH_TCS_CC_PARAM_PROGRESSINDICATOR) >> 4)
#define PRH_TCS_CC_PARAM_SIGNAL_EXISTS(X)             (((X) & PRH_TCS_CC_PARAM_SIGNAL) >> 5)
#define PRH_TCS_CC_PARAM_KEYPADFACILITY_EXISTS(X)            (((X) & PRH_TCS_CC_PARAM_KEYPADFACILITY) >> 6)
#define PRH_TCS_CC_PARAM_CALLINGPARTYNUMBER_EXISTS(X) (((X) & PRH_TCS_CC_PARAM_CALLINGPARTYNUMBER) >> 7)
#define PRH_TCS_CC_PARAM_CALLEDPARTYNUMBER_EXISTS(X)  (((X) & PRH_TCS_CC_PARAM_CALLEDPARTYNUMBER) >> 8)
#define PRH_TCS_CC_PARAM_AUDIOCONTROL_EXISTS(X)       (((X) & PRH_TCS_CC_PARAM_AUDIOCONTROL) >> 9)
#define PRH_TCS_CC_PARAM_SCOHANDLE_EXISTS(X)       (((X) & PRH_TCS_CC_PARAM_SCOHANDLE) >> 10)
#define PRH_TCS_CC_PARAM_DESTCID_EXISTS(X)       (((X) & PRH_TCS_CC_PARAM_DESTCID) >> 11)

#define PRH_TCS_CC_ILLEGAL_BITMASK(X)  (X > 0x1FFF)
														
/*
#define PRH_TCS_CC_PARAM_BEARERHANDLE_EXISTS(X)       (((X) & PRH_TCS_CC_PARAM_BEARERHANDLE) >> 11)
#define PRH_TCS_CC_PARAM_BEARER_SCOHANDLE_EXISTS(X)   ((PRH_TCS_CC_PARAM_BEARERHANDLE_EXISTS(X)) & !(((X) & PRH_TCS_CC_BEARERHANDLETYPE_MASK) >> 10) )
#define PRH_TCS_CC_PARAM_BEARER_L2CAPCID_EXISTS(X)    ((PRH_TCS_CC_PARAM_BEARERHANDLE_EXISTS(X)) & (((X) & PRH_TCS_CC_BEARERHANDLETYPE_MASK) >> 10) )
*/
#define PRH_TCS_CC_PARAM_COMPANYSPECIFIC_EXISTS(X)    (((X) & PRH_TCS_CC_PARAM_COMPANYSPECIFIC) >> 12)


#define PRH_TCS_CC_MSG_ALERTING_BITMASK (PRH_TCS_CC_PARAM_BEARERCAPABILITY|PRH_TCS_CC_PARAM_PROGRESSINDICATOR|PRH_TCS_CC_PARAM_SCOHANDLE|PRH_TCS_CC_PARAM_DESTCID|PRH_TCS_CC_PARAM_COMPANYSPECIFIC)
#define PRH_TCS_CC_MSG_CALLPROCEEDING_BITMASK (PRH_TCS_CC_MSG_ALERTING_BITMASK)
#define PRH_TCS_CC_MSG_CONNECT_BITMASK (PRH_TCS_CC_PARAM_BEARERCAPABILITY|PRH_TCS_CC_PARAM_SCOHANDLE|PRH_TCS_CC_PARAM_COMPANYSPECIFIC)
#define PRH_TCS_CC_MSG_CONNECTACKNOWLEDGE_BITMASK (PRH_TCS_CC_PARAM_SCOHANDLE|PRH_TCS_CC_PARAM_DESTCID|PRH_TCS_CC_PARAM_COMPANYSPECIFIC)
#define PRH_TCS_CC_MSG_DISCONNECT_BITMASK (PRH_TCS_CC_PARAM_CAUSE|PRH_TCS_CC_PARAM_PROGRESSINDICATOR|PRH_TCS_CC_PARAM_SCOHANDLE|PRH_TCS_CC_PARAM_DESTCID|PRH_TCS_CC_PARAM_COMPANYSPECIFIC)
#define PRH_TCS_CC_MSG_INFORMATION_BITMASK (PRH_TCS_CC_PARAM_SENDINGCOMPLETE|PRH_TCS_CC_PARAM_KEYPADFACILITY|PRH_TCS_CC_PARAM_CALLINGPARTYNUMBER|PRH_TCS_CC_PARAM_CALLEDPARTYNUMBER|PRH_TCS_CC_PARAM_AUDIOCONTROL|PRH_TCS_CC_PARAM_COMPANYSPECIFIC)
#define PRH_TCS_CC_MSG_PROGRESS_BITMASK (PRH_TCS_CC_PARAM_PROGRESSINDICATOR|PRH_TCS_CC_PARAM_SCOHANDLE|PRH_TCS_CC_PARAM_DESTCID|PRH_TCS_CC_PARAM_COMPANYSPECIFIC)
#define PRH_TCS_CC_MSG_RELEASE_BITMASK (PRH_TCS_CC_PARAM_CAUSE|PRH_TCS_CC_PARAM_SCOHANDLE|PRH_TCS_CC_PARAM_COMPANYSPECIFIC)
#define PRH_TCS_CC_MSG_RELEASECOMPLETE_BITMASK (PRH_TCS_CC_PARAM_CAUSE|PRH_TCS_CC_PARAM_SCOHANDLE|PRH_TCS_CC_PARAM_COMPANYSPECIFIC)
#define PRH_TCS_CC_MSG_SETUP_BITMASK (PRH_TCS_CC_PARAM_CALLCLASS|PRH_TCS_CC_PARAM_SENDINGCOMPLETE|PRH_TCS_CC_PARAM_BEARERCAPABILITY|PRH_TCS_CC_PARAM_SIGNAL|PRH_TCS_CC_PARAM_CALLINGPARTYNUMBER|PRH_TCS_CC_PARAM_CALLEDPARTYNUMBER|PRH_TCS_CC_PARAM_COMPANYSPECIFIC)
#define PRH_TCS_CC_MSG_SETUPACKNOWLEDGE_BITMASK (PRH_TCS_CC_PARAM_BEARERCAPABILITY|PRH_TCS_CC_PARAM_PROGRESSINDICATOR|PRH_TCS_CC_PARAM_SCOHANDLE|PRH_TCS_CC_PARAM_DESTCID|PRH_TCS_CC_PARAM_COMPANYSPECIFIC)
#define PRH_TCS_CC_MSG_STARTDTMF_BITMASK (PRH_TCS_CC_PARAM_KEYPADFACILITY)
#define PRH_TCS_CC_MSG_STARTDTMFACKNOWLEDGE_BITMASK (PRH_TCS_CC_PARAM_KEYPADFACILITY)
#define PRH_TCS_CC_MSG_STARTDTMFREJECT_BITMASK (PRH_TCS_CC_PARAM_CAUSE)
#define PRH_TCS_CC_MSG_STOPDTMF_BITMASK (PRH_TCS_CC_PARAM_KEYPADFACILITY)
#define PRH_TCS_CC_MSG_STOPDTMFACKNOWLEDGE_BITMASK (PRH_TCS_CC_PARAM_KEYPADFACILITY)


#endif /*(PRH_TCS_TERMINAL || PRH_TCS_GATEWAY)*/


#endif /* __TCS_CC_TYPES_H */
