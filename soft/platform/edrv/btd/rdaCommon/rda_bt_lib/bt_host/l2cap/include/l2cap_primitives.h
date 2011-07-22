 /**************************************************************************
 * MODULE NAME:    l2_primitives.h
 * PROJECT CODE:
 * DESCRIPTION:    l2cap API
 * MAINTAINER:     Martin Mellody
 * DATE:           02-12-1999
 *
 * SOURCE CONTROL: $Id: l2cap_primitives.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies, Ltd.
 *     All rights reserved.
 *
 * SOURCE: 
 *
 *************************************************************************/

/*
    WARNING::

    Some of these structure types are visible on the API and 
    therefore MUST match the declarations in user/l2cap.h. 
*/

#ifndef __L2CAP_PRIMITIVES_H
#define __L2CAP_PRIMITIVES_H

#include "host_config.h"
#include "host_types.h"
#include "host_error.h"


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
#define L2CAP_evTIMEOUTIND  11	
#if PRH_L2CAP_AUTO_CONFIG==1
#define L2CAP_evCONNCONFIGUREDCONF 12
#endif

#if PRH_L2CAP_AUTO_CONFIG==1
typedef struct {
  u_int16 inMTUmin;
  u_int16 inMTUmax;
  u_int16 outMTUmin;
  u_int16 outMTUmax;
  u_int16 flushTO;
  u_int16 linkTO;
} t_L2_ConfigReqParams;
#endif
/* Quality of service definitions */

typedef struct  {
	u_int8 flags;
	u_int8 serviceType;
	u_int32 tokenRate;
	u_int32 bucketSize;
	u_int32 peakBandwidth;
	u_int32 latency;
	u_int32 delayVariation;
} t_L2_ChannelFlowSpec ;


/* The arguments of control events delivered to the upper layer */

typedef struct st_t_L2_controlCallbackArgs {
	t_bdaddr BDAddress;
	u_int16 psm;
	u_int16 cid;
        u_int16 mtu; 
	u_int16 flushTimeout;
	u_int16 status;
	u_int16 result;
	u_int8 timerType;    /* Identifies RTX or ERTX timer					*/
    u_int8 identifier;
	t_L2_ChannelFlowSpec *flow;
    struct host_buf* p_buf;
#if (PRH_L2CAP_AUTO_CONFIG==1)
  u_int16 mtu_in; /* only used by AUTOCONFIG */
#endif
} t_L2_ControlCallbackArgs ;

/* A structure of callback functions used to deliver events to the higher layers */

struct l2cap_cbs {
	void (*connectionIndication)(u_int8 eventType, t_L2_ControlCallbackArgs *args);
	void (*configureIndication)(u_int8 eventType, t_L2_ControlCallbackArgs *args);
	void (*disconnectionIndication)(u_int8 eventType, t_L2_ControlCallbackArgs *args);
	void (*qosviolationIndication)(u_int8 eventType,t_L2_ControlCallbackArgs *args);
	void (*connectionConfirmation)(u_int8 eventType, t_L2_ControlCallbackArgs *args);
	void (*configureConfirmation)(u_int8 eventType, t_L2_ControlCallbackArgs *args);
	void (*disconnectionConfirmation)(u_int8 eventType, t_L2_ControlCallbackArgs *args);  /* Unused ?? */
	void (*bufferOverflow)(u_int8 eventType,t_L2_ControlCallbackArgs *args); 
	void (*flushOccurred)(u_int8 eventType, t_L2_ControlCallbackArgs *args);
	void (*timeoutIndication)(u_int8 eventType,t_L2_ControlCallbackArgs *args);
	void (*dataIndication)(u_int16 cid, struct host_buf* p_buf);
/*#if (PRH_L2CAP_AUTO_CONFIG==1)*/
        void (*connectionConfiguredConfirmation)(u_int8 eventType,t_L2_ControlCallbackArgs *args);
/*#endif*/
      
};

typedef struct l2cap_cbs l2_callbacks;


APIDECL1 t_api APIDECL2 L2_RegisterPSM(u_int16 psm,  l2_callbacks *callbacks);
APIDECL1 t_api APIDECL2 L2_DeregisterPSM(u_int16 psm);


APIDECL1 t_api APIDECL2 L2_Connect(t_bdaddr BDAddress, u_int16 psm);
APIDECL1 t_api APIDECL2 L2_ConnectResponse(t_bdaddr BDAddress, u_int16 cid, u_int16 response, u_int16 status);


APIDECL1 t_api APIDECL2 L2_Configure(u_int16 cid, u_int16 inMTU, u_int16 outFlushTimeout, u_int16 linkTimeout, t_L2_ChannelFlowSpec *outflow);
APIDECL1 t_api APIDECL2 L2_ConfigureResponse(u_int16 cid, u_int16 outMTU, u_int16 response, t_L2_ChannelFlowSpec *inflow);

#if PRH_L2CAP_AUTO_CONFIG==1
APIDECL1 t_api APIDECL2 L2_ConnectandConfig(t_bdaddr bdaddr, u_int16 psm, t_L2_ConfigReqParams *req_params);
APIDECL1 t_api APIDECL2 L2_ConnectResponseandConfig(t_bdaddr bdaddr, u_int16 cid, u_int16 response, u_int16 status, t_L2_ConfigReqParams *req_params);
#endif

APIDECL1 t_api APIDECL2 L2_Disconnect(u_int16 cid);

APIDECL1 t_api APIDECL2 L2_RetrieveHandle(u_int16 cid, u_int16* handle);

APIDECL1 t_api APIDECL2 L2_DisableConnectionlessTraffic(u_int16 psm);
APIDECL1 t_api APIDECL2 L2_EnableConnectionlessTraffic(u_int16 psm);

APIDECL1 t_api APIDECL2 L2_GetWriteBuffer(u_int16 cid, u_int16 len, t_DataBuf **writeBuffer);
APIDECL1 t_api APIDECL2 L2_Write(u_int16 cid, t_DataBuf *writeBuffer); 
APIDECL1 t_api APIDECL2 L2_FreeWriteBuffer(t_DataBuf *writeBuffer); 

#if (L2CAP_GROUPS_SUPPORTED==1)
APIDECL1 t_api APIDECL2 L2_GroupCreate(u_int16 psm, u_int16 *cid);
APIDECL1 t_api APIDECL2 L2_GroupClose(u_int16 cid);

APIDECL1 t_api APIDECL2 L2_GroupAddMember(u_int16 group_cid, t_bdaddr bdaddr, void (*addMemberCfm)(u_int16 cid, t_bdaddr bdaddr, u_int16 result));

APIDECL1 t_api APIDECL2 L2_GroupRemoveMember(u_int16 cid, t_bdaddr bdaddr);
APIDECL1 t_api APIDECL2 L2_GroupGetMembers(u_int16 group_cid,t_bdaddr *addrList, u_int16* addrListLen);
#endif

#if (L2CAP_PING_SUPPORTED==1)
APIDECL1 t_api APIDECL2 L2_Ping(t_bdaddr BDAddress,u_int8 *echoData, u_int16 length, void (*callbackFunc)(t_bdaddr BDAddress, u_int16 result, u_int8 *echoData, u_int16 length));
#endif

#if (L2CAP_INFO_SUPPORTED==1)
APIDECL1 t_api APIDECL2 L2_GetInfo(t_bdaddr BDAddress, u_int16 infoType, void (*callbackFunc)(t_bdaddr BDAddress,u_int16 result, u_int8* infoData,u_int16 infoLen)); 
#endif

void L2_Dont_Send_Disconnect_Rsp(void);
void L2_Dont_Send_Echo_Response(void);

/* Internal stack buffer allocation functions */
t_api prh_l2_GetWriteBuffer(u_int16 cid, u_int16 len, u_int32 flags, struct host_buf **writeBuffer);
t_api prh_l2_FreeWriteBuffer(struct host_buf *writeBuffer);


#endif /* __L2CAP_API_H */
 

