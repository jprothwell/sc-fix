/***********************************************************************
 *
 * MODULE NAME:    l2cap.h
 * DESCRIPTION:    Bluetooth Host Stack L2CAP API header
 * AUTHOR:         Gary Fleming, Martin Mellody
 *
 * SOURCE CONTROL: $Id: l2cap.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 * REMARKS:
 *     This file should be incuded via bt.h only.
 *
 ***********************************************************************/


#ifndef BLUETOOTH_L2CAP_API_DECLARED
#define BLUETOOTH_L2CAP_API_DECLARED

#include "bt_sys.h"
#include "bt.h"

/* QoS constants */
#define L2_TBR_WILDCARD			0x00000000
#define L2_TBR_DONTCARE			0xFFFFFFFF
#define L2_TBS_WILDCARD			0x00000000
#define L2_TBS_DONTCARE			0xFFFFFFFF
#define L2_PEAKBW_UNKNOWN		0x00000000
#define L2_LATENCY_DONTCARE		0xFFFFFFFF
#define L2_DELAY_DONTCARE		0xFFFFFFFF



/* Structure for QoS specification */
typedef struct st_t_L2_channelFlowSpec {
	u_int8 flags;
	u_int8 serviceType;
	u_int32 tokenRate;
	u_int32 bucketSize;
	u_int32 peakBandwidth;
	u_int32 latency;
	u_int32 delayVariation;
} t_L2_ChannelFlowSpec;


/* Structure for all control callback arguments */
typedef struct st_t_L2_controlCallbackArgs {
	t_bdaddr bdaddr;
	u_int16 psm;
	u_int16 cid;
	u_int16 mtu;
	u_int16 flushTimeout;
	u_int16 status;
	u_int16 result;
	u_int8 timerType;    
    u_int8 identifier;
	struct st_t_L2_channelFlowSpec *flow;
} t_L2_ControlCallbackArgs;



/* Constants for eventType calback argument */
#define L2CAP_evCONNECTIND		1
#define L2CAP_evCONNECTCONF		2
#define L2CAP_evCONFIGUREIND	3
#define L2CAP_evCONFIGURECONF	4
#define L2CAP_evDISCONNECTIND	5
#define L2CAP_evDISCONNECTCONF	6
#define L2CAP_evQOSVIOLATIONIND	7
#define L2CAP_evDATAIND			8

#define L2CAP_evBUFFEROVERFLOW	9
#define L2CAP_evFLUSHOCCURRED	10
#define L2CAP_evTIMEOUT			11

#define L2_BROADCASTCID 0x02

/* Strucutre for per-PSM callback registration */
typedef struct st_t_L2_psmCallbacks {

	void (*connectionIndication)(u_int8 eventType, t_L2_ControlCallbackArgs *args);
	void (*configureIndication)(u_int8 eventType, t_L2_ControlCallbackArgs *args);
	void (*disconnectionIndication)(u_int8 eventType, t_L2_ControlCallbackArgs *args);
	void (*qosviolationIndication)(u_int8 eventType,t_L2_ControlCallbackArgs *args);
	void (*connectionConfirmation)(u_int8 eventType, t_L2_ControlCallbackArgs *args);
	void (*configureConfirmation)(u_int8 eventType, t_L2_ControlCallbackArgs *args);
	void (*disconnectionConfirmation)(u_int8 eventType, t_L2_ControlCallbackArgs *args);  
    void (*bufferOverflow)(u_int8 eventType, t_L2_ControlCallbackArgs *args);  	
	void (*flushOccurred)(u_int8 eventType, t_L2_ControlCallbackArgs *args); 
	void (*timeoutIndication)(u_int8 eventType, t_L2_ControlCallbackArgs *args);  
	void (*dataIndication)(u_int16 cid, t_DataBuf *buffer);
        void (*connectionConfigured)(u_int8 eventType, t_L2_ControlCallbackArgs *args);  
} t_L2_PsmCallbacks;



/*
	L2CAP Function Declarations
*/

APIDECL1 int APIDECL2 L2_RegisterPSM(u_int16 psm,  t_L2_PsmCallbacks *callbacks);
APIDECL1 int APIDECL2 L2_DeregisterPSM(u_int16 psm);

APIDECL1 int APIDECL2 L2_Connect(t_bdaddr bdaddr, u_int16 psm);
APIDECL1 int APIDECL2 L2_ConnectResponse(t_bdaddr bdaddr, u_int16 cid, u_int16 response, u_int16 status);

/* Link Supervision Timeout/Flush Timeout, MTU constants */
#define L2_FTO_INFINITE		0xFFFF	/* no flush */
#define L2_FTO_DEFAULT		0xFFFF		
#define L2_FTO_USEEXISTING	0	
	
#define L2_LST_DEFAULT		0xFFFF	/* 40 secs */
#define L2_LST_USEEXISTING	0	

#define L2_MTU_DEFAULT		672	
#define L2_MTU_MIN              48

/* Successful status/result value */
#define L2CAP_NOERROR		0

APIDECL1 int APIDECL2 L2_Configure(u_int16 cid, u_int16 inMTU, u_int16 outFlushTimeout, u_int16 linkTimeout, t_L2_ChannelFlowSpec *outfFlow);
APIDECL1 int APIDECL2 L2_ConfigureResponse(u_int16 cid, u_int16 outMTU, u_int16 response, t_L2_ChannelFlowSpec *inFlow);

APIDECL1 int APIDECL2 L2_Disconnect(u_int16 cid);

APIDECL1 int APIDECL2 L2_GetWriteBuffer(u_int16 cid, u_int16 len, t_DataBuf **writeBuffer);
APIDECL1 int APIDECL2 L2_Write(u_int16 cid, t_DataBuf *writeBuffer);
APIDECL1 int APIDECL2 L2_FreeWriteBuffer(t_DataBuf *writeBuffer);

APIDECL1 int APIDECL2 L2_RetrieveHandle(u_int16 cid, u_int16 *handle);

APIDECL1 int APIDECL2 L2_GroupCreate(u_int16 psm, u_int16 *cid);
APIDECL1 int APIDECL2 L2_GroupClose(u_int16 cid);

APIDECL1 int APIDECL2 L2_GroupAddMember(u_int16 cid, t_bdaddr bdaddr, void (*callbackFunc)(u_int16 cid, t_bdaddr bdaddr, u_int16 result));
APIDECL1 int APIDECL2 L2_GroupRemoveMember(u_int16 cid, t_bdaddr bdaddr);
APIDECL1 int APIDECL2 L2_GroupGetMembers(u_int16 cid, t_bdaddr *addrList, u_int16 *addrListLen);

APIDECL1 int APIDECL2 L2_EnableConnectionlessTraffic(u_int16 psm);
APIDECL1 int APIDECL2 L2_DisableConnectionlessTraffic(u_int16 psm);

APIDECL1 int APIDECL2 L2_Ping(t_bdaddr bdaddr, u_int8 *echoData, u_int16 length, void (*callbackFunc)(t_bdaddr bdaddr, u_int16 result, u_int8 *echoData, u_int16 length));
APIDECL1 int APIDECL2 L2_GetInfo(t_bdaddr bdaddr, u_int16 infoType, void (*callbackFunc)(t_bdaddr bdaddr, u_int16 result, u_int8* infoData, u_int16 infoLen));
#endif /* BLUETOOTH_L2CAP_API_DECLARED */
