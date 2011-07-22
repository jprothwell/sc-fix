/*
 * MODULE NAME:    hci_primitives.h
 * DESCRIPTION:    HCI Service Primitive Declarations
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: hci_primitives.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */             

#ifndef prh_HCI_PRIMITIVES_H
#    define prh_HCI_PRIMITIVES_H

#include "host_config.h"

#include "host_types.h"
#include "hci_constants.h"
#include "host_buf.h"

extern volatile u_int8 prh_hci_start_complete;
extern volatile u_int8 prh_hci_hc_responding;


/* PDU types for HCI_SendRawPDU() HCI_RegisterRxHandler() callback function */
#define HCI_pduCOMMAND          0x01
#define HCI_pduACLDATA          0x02
#define HCI_pduSCODATA          0x03
#define HCI_pduEVENT            0x04

/* handle specifiers used by HCI_GetWriteBuffer */
#define HCI_NO_HANDLE                   0xFFF1 
#define HCI_COMMAND_HANDLE              HCI_NO_HANDLE
#define HCI_ACTIVE_BROADCAST_HANDLE	    0xFFF2 
#define HCI_PICONET_BROADCAST_HANDLE    0xFFF3 
#define HCI_SCO_HANDLE 0x8000
#define HCI_ACL_HANDLE 0x0000

APIDECL1 t_api APIDECL2 HCI_RegisterEventHandler(void (*callbackFunc)(u_int8 *eventPdu));
APIDECL1 t_api APIDECL2 HCI_DeregisterEventHandler(void (*callbackFunc)(u_int8 *eventPdu));

APIDECL1 t_api APIDECL2 HCI_RegisterRxHandler(void (*callbackFunc)(u_int8 type, u_int8 *pdu, u_int32 len)); 
APIDECL1 t_api APIDECL2 HCI_DeregisterRxHandler(void (*callbackFunc)(u_int8 type, u_int8 *pdu, u_int32 len));

APIDECL1 t_api APIDECL2 HCI_GetWriteBuffer(u_int16 handle, u_int16 len, struct st_t_dataBuf **writeBuffer);
APIDECL1 t_api APIDECL2 HCI_FreeWriteBuffer(struct st_t_dataBuf *writeBuffer);
APIDECL1 t_api APIDECL2 HCI_SendRawPDU(u_int8 type, struct st_t_dataBuf *pdu);







/* flags for HCI_SendACL() and L2capACLCallback */
#define prh_hci_ACL_CONTINUE_FRAGMENT       1
#define prh_hci_ACL_ACTIVE_BROADCAST        2
#define prh_hci_ACL_PICONET_BROADCAST       4
#define prh_hci_ACL_START_FRAGMENT          8

/* Types for command primitives */

typedef struct st_t_HCI_numPackets  { /* for HCI_HostNumberOfCompletedPackets() */
    u_int16 Handle;
    u_int16 NumCompleted;
} t_HCI_NumPackets;

typedef struct st_t_HCI_WriteLinkKey  { /* for HCI_WriteStoredLinkKey() */
    t_bdaddr BDAddress;
    u_int8 linkKey[16];
} t_HCI_WriteLinkKey;

typedef struct st_t_HCI_EventFilter  { /* for HCI_SetEventFilter() */
    u_int8 FilterType;
    u_int8 ConditionType;   
    struct st_t_HCI_eventFilter_Condition  {
        t_bdaddr BDAddress; 
        u_int32 DeviceClass;
        u_int32 DeviceClassMask;
        u_int8 AutoAccept;
    } Condition;
} t_HCI_EventFilter;	 


    
/*
    HCI non-API functions 
*/
t_api prh_hci_layer_ctrl(u_int8 flag);


t_api prh_hci_register_manager_event_cb(t_api (*callbackfunc)(u_int8 *));
t_api prh_hci_register_l2cap_event_cb(t_api (*callbackfunc)(u_int8 *));
t_api prh_hci_register_l2cap_acl_cb(t_api (*callbackfunc)(u_int16 handle, struct host_buf *pdu, u_int8 flags));

t_api prh_hci_send_acl_child_list(u_int16 handle, struct host_buf *bufferParent, u_int8 flags);


/* Data primitives for L2CAP to use */
t_api prh_hci_GetCommandBuffer(u_int16 length, u_int32 flags, struct host_buf **p_buf);
t_api prh_hci_GetWriteBuffer(u_int16 handle, u_int16 length, u_int32 flags, struct host_buf **p_buf);
t_api prh_hci_GetSCOBuffer(u_int16 handle, u_int16 length, u_int32 flags, struct host_buf **p_buf);
t_api prh_hci_FreeWriteBuffer(struct host_buf *p_buf);
t_api prh_hci_ReleaseCommandBuffer(struct host_buf *p_buf);
t_api prh_hci_TransportFreeBuffer(struct host_buf *p_buf, u_int8 type);

/*
    HCI data primitives 
*/
APIDECL1 t_api APIDECL2 HCI_SendSCO(u_int16 handle, struct host_buf *data);
APIDECL1 t_api APIDECL2 HCI_SendACL(u_int16 handle, struct host_buf *data, u_int8 flags);


/*
    HCI link control commands - Opcode Group Field = 0x01
*/
APIDECL1 t_api APIDECL2 HCI_Inquiry(u_int32 lap, u_int8 inquiryLength, u_int8 numResponses);
APIDECL1 t_api APIDECL2 HCI_InquiryCancel(void);
APIDECL1 t_api APIDECL2 HCI_PeriodicInquiryMode(u_int16 maxPeriodLen, u_int16 minPeriodLen, u_int32 lap, u_int8 inquiryLen, u_int8 numResponses);
APIDECL1 t_api APIDECL2 HCI_ExitPeriodicInquiryMode(void);
APIDECL1 t_api APIDECL2 HCI_CreateConnection(t_bdaddr bdaddr, u_int16 packetType, u_int8 pageScanRepet, u_int8 pageScanMode, u_int16 clockOffset, u_int8 role);
APIDECL1 t_api APIDECL2 HCI_Disconnect(u_int16 handle, u_int8 reason);
APIDECL1 t_api APIDECL2 HCI_AddSCOConnection(u_int16 handle, u_int16 packetType);
APIDECL1 t_api APIDECL2 HCI_AcceptConnectionRequest(t_bdaddr bdaddr, u_int8 role);
APIDECL1 t_api APIDECL2 HCI_RejectConnectionRequest(t_bdaddr bdaddr, u_int8 reason);
APIDECL1 t_api APIDECL2 HCI_LinkKeyRequestReply(t_bdaddr bdaddr, u_int8 *linkKey);
APIDECL1 t_api APIDECL2 HCI_LinkKeyRequestNegativeReply(t_bdaddr bdaddr);
APIDECL1 t_api APIDECL2 HCI_PINCodeRequestReply(t_bdaddr bdaddr, u_int8 pinCodeLen, u_int8 *pincode);
APIDECL1 t_api APIDECL2 HCI_PINCodeRequestNegativeReply(t_bdaddr bdaddr);
APIDECL1 t_api APIDECL2 HCI_ChangeConnectionPacketType(u_int16 handle, u_int16 packetType);
APIDECL1 t_api APIDECL2 HCI_AuthenticationRequested(u_int16 handle);
APIDECL1 t_api APIDECL2 HCI_SetConnectionEncryption(u_int16 handle, u_int8 encrypt);
APIDECL1 t_api APIDECL2 HCI_ChangeConnectionLinkKey(u_int16 handle);
APIDECL1 t_api APIDECL2 HCI_MasterLinkKey(u_int8 keyFlag);
APIDECL1 t_api APIDECL2 HCI_RemoteNameRequest(t_bdaddr bdaddr, u_int8 pageScanRepet, u_int8 pageScanMode, u_int16 clockOffset);
APIDECL1 t_api APIDECL2 HCI_RemoteNameRequestCancel(t_bdaddr bdaddr);
APIDECL1 t_api APIDECL2 HCI_ReadRemoteSupportedFeatures(u_int16 handle);
APIDECL1 t_api APIDECL2 HCI_ReadRemoteVersionInformation(u_int16 handle);
APIDECL1 t_api APIDECL2 HCI_ReadClockOffset(u_int16 handle);


/*
    HCI link policy commands - Opcode Group Field = 0x02
*/
APIDECL1 t_api APIDECL2 HCI_HoldMode(u_int16 handle, u_int16 maxInterval, u_int16 minInterval);
APIDECL1 t_api APIDECL2 HCI_SniffMode(u_int16 handle, u_int16 maxInterval, u_int16 minInterval, u_int16 sniffAttempt, u_int16 sniffTimeout);
APIDECL1 t_api APIDECL2 HCI_ExitSniffMode(u_int16 handle);
APIDECL1 t_api APIDECL2 HCI_ParkMode(u_int16 handle, u_int16 beaconMaxInterval, u_int16 beaconMinInterval);
APIDECL1 t_api APIDECL2 HCI_ExitParkMode(u_int16 handle);
APIDECL1 t_api APIDECL2 HCI_QoSSetup(u_int16 handle, u_int8 flags, u_int8 serviceType, u_int32 tokenRate, u_int32 peakBandwidth, u_int32 latency, u_int32 delayVar);
APIDECL1 t_api APIDECL2 HCI_RoleDiscovery(u_int16 handle);
APIDECL1 t_api APIDECL2 HCI_SwitchRole(t_bdaddr bdaddr, u_int8 role);
APIDECL1 t_api APIDECL2 HCI_ReadLinkPolicySettings(u_int16 handle);
APIDECL1 t_api APIDECL2 HCI_WriteLinkPolicySettings(u_int16 handle, u_int16 settings);


/*
    HCI host controller and baseband commands - Opcode Group Field = 0x03
*/
APIDECL1 t_api APIDECL2 HCI_SetEventMask(u_int8 *eventMask);
APIDECL1 t_api APIDECL2 HCI_Reset(void);
APIDECL1 t_api APIDECL2 HCI_SetEventFilter(t_HCI_EventFilter *filter);
APIDECL1 t_api APIDECL2 HCI_Flush(u_int16 handle);
APIDECL1 t_api APIDECL2 HCI_ReadPINType(void);
APIDECL1 t_api APIDECL2 HCI_WritePINType(u_int8 pinType);
APIDECL1 t_api APIDECL2 HCI_CreateNewUnitKey(void);
APIDECL1 t_api APIDECL2 HCI_ReadStoredLinkKey(t_bdaddr bdaddr, u_int8 readFlag);
APIDECL1 t_api APIDECL2 HCI_WriteStoredLinkKey(u_int8 numKeys, t_HCI_WriteLinkKey *links);
APIDECL1 t_api APIDECL2 HCI_DeleteStoredLinkKey(t_bdaddr bdaddr, u_int8 deleteFlag);
APIDECL1 t_api APIDECL2 HCI_ChangeLocalName(u_int8 *name); 
APIDECL1 t_api APIDECL2 HCI_ReadLocalName(void);
APIDECL1 t_api APIDECL2 HCI_ReadConnectionAcceptTimeout(void);
APIDECL1 t_api APIDECL2 HCI_WriteConnectionAcceptTimeout(u_int16 timeout);
APIDECL1 t_api APIDECL2 HCI_ReadPageTimeout(void);
APIDECL1 t_api APIDECL2 HCI_WritePageTimeout(u_int16 timeout);
APIDECL1 t_api APIDECL2 HCI_ReadScanEnable(void);
APIDECL1 t_api APIDECL2 HCI_WriteScanEnable(u_int8 scanEnable);
APIDECL1 t_api APIDECL2 HCI_ReadPageScanActivity(void);
APIDECL1 t_api APIDECL2 HCI_WritePageScanActivity(u_int16 interval, u_int16 window);
APIDECL1 t_api APIDECL2 HCI_ReadInquiryScanActivity(void);
APIDECL1 t_api APIDECL2 HCI_WriteInquiryScanActivity(u_int16 interval, u_int16 window);
APIDECL1 t_api APIDECL2 HCI_ReadAuthenticationEnable(void);
APIDECL1 t_api APIDECL2 HCI_WriteAuthenticationEnable(u_int8 enable);
APIDECL1 t_api APIDECL2 HCI_ReadEncryptionMode(void);
APIDECL1 t_api APIDECL2 HCI_WriteEncryptionMode(u_int8 mode);
APIDECL1 t_api APIDECL2 HCI_ReadClassOfDevice(void);
APIDECL1 t_api APIDECL2 HCI_WriteClassOfDevice(u_int32 cod);
APIDECL1 t_api APIDECL2 HCI_ReadVoiceSetting(void);
APIDECL1 t_api APIDECL2 HCI_WriteVoiceSetting(u_int16 voiceSetting);
APIDECL1 t_api APIDECL2 HCI_ReadAutomaticFlushTimeout(u_int16 handle);
APIDECL1 t_api APIDECL2 HCI_WriteAutomaticFlushTimeout(u_int16 handle, u_int16 timeout);
APIDECL1 t_api APIDECL2 HCI_ReadNumBroadcastRetransmissions(void);
APIDECL1 t_api APIDECL2 HCI_WriteNumBroadcastRetransmissions(u_int8 number);
APIDECL1 t_api APIDECL2 HCI_ReadHoldModeActivity(void);
APIDECL1 t_api APIDECL2 HCI_WriteHoldModeActivity(u_int8 activity);
APIDECL1 t_api APIDECL2 HCI_ReadTransmitPowerLevel(u_int16 handle, u_int8 type);
APIDECL1 t_api APIDECL2 HCI_ReadSCOFlowControlEnable(void);
APIDECL1 t_api APIDECL2 HCI_WriteSCOFlowControlEnable(u_int8 enable);
APIDECL1 t_api APIDECL2 HCI_SetHostControllerToHostFlowControl(u_int8 enable);
APIDECL1 t_api APIDECL2 HCI_HostBufferSize(u_int16 ACLlength, u_int8 SCOlength, u_int16 totalACLpackets, u_int16 totalSCOpackets);
APIDECL1 t_api APIDECL2 HCI_HostNumberOfCompletedPackets(u_int8 numHandles, t_HCI_NumPackets *numPackets);
APIDECL1 t_api APIDECL2 HCI_ReadLinkSupervisionTimeout(u_int16 handle);
APIDECL1 t_api APIDECL2 HCI_WriteLinkSupervisionTimeout(u_int16 handle, u_int16 timeout);
APIDECL1 t_api APIDECL2 HCI_ReadNumberOfSupportedIAC(void);
APIDECL1 t_api APIDECL2 HCI_ReadCurrentIACLAP(void);
APIDECL1 t_api APIDECL2 HCI_WriteCurrentIACLAP(u_int8 numLap, u_int32 *lap);
APIDECL1 t_api APIDECL2 HCI_ReadPageScanPeriodMode(void);
APIDECL1 t_api APIDECL2 HCI_WritePageScanPeriodMode(u_int8 mode);
APIDECL1 t_api APIDECL2 HCI_ReadPageScanMode(void);
APIDECL1 t_api APIDECL2 HCI_WritePageScanMode(u_int8 mode);


/*
    HCI informational commands - Opcode Group Field = 0x04
*/
APIDECL1 t_api APIDECL2 HCI_ReadFailedContactCounter(u_int16 handle);
APIDECL1 t_api APIDECL2 HCI_ResetFailedContactCounter(u_int16 handle);
APIDECL1 t_api APIDECL2 HCI_GetLinkQuality(u_int16 handle);
APIDECL1 t_api APIDECL2 HCI_ReadRSSI(u_int16 handle);


/*
    HCI status commands - Opcode Group Field = 0x05
*/
APIDECL1 t_api APIDECL2 HCI_ReadLocalVersionInformation(void);
APIDECL1 t_api APIDECL2 HCI_ReadLocalSupportedFeatures(void);
APIDECL1 t_api APIDECL2 HCI_ReadBufferSize(void);
APIDECL1 t_api APIDECL2 HCI_ReadCountryCode(void);
APIDECL1 t_api APIDECL2 HCI_ReadBDAddress(void);


/*
    HCI test commands - Opcode Group Field = 0x06
*/
APIDECL1 t_api APIDECL2 HCI_ReadLoopbackMode(void);
APIDECL1 t_api APIDECL2 HCI_WriteLoopbackMode(u_int8 mode);
APIDECL1 t_api APIDECL2 HCI_EnableDUT(void);




#endif

