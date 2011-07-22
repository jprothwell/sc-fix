/*
 * MODULE NAME:    mgr_core.h
 * DESCRIPTION:    Host Manager Core Functionality
 * AUTHOR:         Martin Mellody
 * DATE:           10 March 2000
 *
 * SOURCE CONTROL: $Id: mgr_core.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */

#ifndef __MGR_CORE_H
#define __MGR_CORE_H

#include "host_config.h"
#include "host_types.h"
#include "host_buf.h"
#include "host_error.h"

#include "hci_primitives.h"
#include "l2_connect_sm.h"

#include "papi.h"
#include "hci_primitives.h"
#include "hci_extensions.h"
#include "hci_constants.h"  

#include "mgr_devdb.h"
#include "mgr_secdb.h"
#include "mgr_inqdb.h"

#define PRH_MGR_ACCEPT_CONNECTIONS_NO_SWITCH    1
#define PRH_MGR_ACCEPT_CONNECTIONS_AND_SWITCH   2
#define PRH_MGR_REJECT_CONNECTIONS              3
#define PRH_MGR_IGNORE_CONNECTIONS              4

/* macros to allow reading the value of preprocessor definitions */
#ifndef BT_STRINGIZE
#define BT_STRINGIZE(x) #x
#endif
#ifndef BT_VALUEOF
#define BT_VALUEOF(x) BT_STRINGIZE(x)
#endif

#define MAX_SCO_LINKS 3
#define MGR_HW_SCO  1 /* sco link where data comes from hardware codec transparently to the host */
#define MGR_HCI_SCO 2 /* sco link where data comes from the host */

#define PARTHUS_MANUFACTURERID 14
#define MGR_MAXACTIVEACLS 7
#define PRH_MGR_EXTRAHOLDINTERVALPERDEVICE 0x600 /* rougly 1 sec. extra per device */

#define BT_HOST_VERSION 	1 /* major software revision */
#define BT_HOST_REVISION	7 /* minor software revision */
#define BT_HOST_PATCHLEVEL	4 /* software patch level	 */

/* For handling command status event reporting errors */
#define PRH_MGR_SIZE_PENDINGCMDARRAY 5
#define PRH_MGR_INDEXPENDINGCOMMANDSTATUS_HOLDMODE		0x0
#define PRH_MGR_INDEXPENDINGCOMMANDSTATUS_SNIFFMODE		0x1
#define PRH_MGR_INDEXPENDINGCOMMANDSTATUS_EXITSNIFFMODE 0x2
#define PRH_MGR_INDEXPENDINGCOMMANDSTATUS_PARKMODE		0x3
#define PRH_MGR_INDEXPENDINGCOMMANDSTATUS_EXITPARKMODE	0x4

#define NEED_TO_RETRIEVE_NAME			0
#define NONEED_TO_RETRIEVE_NAME			1	

//class of device to be retrieved
#define ALL_KIND_OF_DEVICE				0			// all of the class 
#define	EAR_PHONE						1			
#define	MOBILE							2			


typedef struct st_t_MGR_Version {
	u_int8 HCIversion;
	u_int16 HCIrevision;
	u_int8 LMPversion;
	u_int16 LMPsubversion;
	u_int16 ManufacturerId;
} t_MGR_HCVersion;


struct st_t_pendingCmdStatus  {
	u_int8 count;
	u_int8 currentIndex;
	struct st_t_BT_ConnectionInfo *devDbEntry[MGR_MAXACTIVEACLS];
};


/* Flags for MGR_Shutdown() */
#define MGR_DETACHALL		0
#define MGR_FORCED			1

#define DEFAULT_PAGESCANREPMODE 2
#define BT_GIAC_LAP  0x9E8B33
#define BT_LIAC_LAP  0x9E8B00
#define GIAC_LAP  BT_GIAC_LAP



#define MGR_MAXSNIFFATTEMPT 0x7FFF

#define NUM_BYTES_LMP_FEATURES 8


/* Bitmasks for MGR_SetLinkFlags */
#define MGR_KEYREQ_HANDLE           0x0001
#define MGR_KEYREQ_IGNORE           0x0002
#define MGR_KEYREQ_REJECT           0x0004
#define MGR_PINREQ_HANDLE           0x0008
#define MGR_PINREQ_IGNORE           0x0010
#define MGR_PINREQ_REJECT           0x0020
#define MGR_CONNREQ_SCO_ACCEPT      0x0040
#define MGR_CONNREQ_SCO_IGNORE      0x0080
#define MGR_CONNREQ_SCO_REJECT      0x0100
#define MGR_CONNREQ_ACL_ACCEPT      0x0200
#define MGR_CONNREQ_ACL_IGNORE      0x0400
#define MGR_CONNREQ_ACL_REJECT      0x0800
#define MGR_CONNREQ_ACL_SWITCH      0x1200
#define MGR_CONNSETUP_ALLOW_SWITCH  0x2000
#define MGR_CONNSETUP_REFUSE_SWITCH 0x4000
#define MGR_CONNREQ_IGNORE_ALL      (MGR_CONNREQ_SCO_IGNORE|MGR_CONNREQ_ACL_IGNORE)
#define MGR_CONNREQ_ACCEPT_ALL      (MGR_CONNREQ_SCO_ACCEPT|MGR_CONNREQ_ACL_ACCEPT)
#define MGR_SECREQ_IGNORE_ALL       (MGR_KEYREQ_IGNORE|MGR_PINREQ_IGNORE)
#define MGR_SECREQ_HANDLE_ALL       (MGR_KEYREQ_HANDLE|MGR_PINREQ_HANDLE)
#define MGR_LINKFLAGS_HANDLE        (MGR_CONNREQ_ACCEPT_ALL|MGR_SECREQ_HANDLE_ALL|MGR_CONNSETUP_ALLOW_SWITCH)
#define MGR_LINKFLAGS_IGNORE        (MGR_CONNREQ_IGNORE_ALL|MGR_SECREQ_IGNORE_ALL|MGR_CONNSETUP_ALLOW_SWITCH)
 
#define MGR_DISABLE_MANAGER_RXPATH 0x80000000  /* The MGR_DISABLE_MANAGER_RXPATH flag is for Parthus internal use only and is unsupported */

#define MGR_HCI_CONNREJECT_REASON 0x0D /* Reason code used for automatic rejection - 0x0d=limited resources*/

/* Packet Types */
#define MGR_DM1PACKETTYPE 0x0008
#define MGR_DHIPACKETTYPE 0x0010
#define MGR_HV1PACKETTYPE 0x0020
#define MGR_HV2PACKETTYPE 0x0040
#define MGR_HV3PACKETTYPE 0x0080
#define MGR_DM3PACKETTYPE 0x0400
#define MGR_DH3PACKETTYPE 0x0800
#define MGR_DM5PACKETTYPE 0x4000
#define MGR_DH5PACKETTYPE 0x8000
#define MGR_DEFAULTPACKETTYPE 0xff1e
//#define MGR_DEFAULTPACKETTYPE MGR_DM1PACKETTYPE|MGR_DM3PACKETTYPE|MGR_DM5PACKETTYPE

#if BTHOST_BIGENDIAN
#define PRH_GIAC			"\x00\x9E\x8B\x33"
#define PRH_GIAC_PLUS_LIAC	"\x00\x9E\x8B\x33\x00\x9E\x8B\x00"
#else
#define PRH_GIAC			"\x33\x8B\x9E\x00"
#define PRH_GIAC_PLUS_LIAC	"\x33\x8B\x9E\x00\x00\x8B\x9E\x00"
#endif

/* Discoverability Modes */
#define MGR_NONDISCOVERABLE		0x00
#define MGR_GENERALDISCOVERY	0x01
#define MGR_LIMITEDDISCOVERY	0x03
/* Connectability Modes */
#define MGR_NONCONNECTABLE		0x00
#define MGR_CONNECTABLE			0x04
		
/* Mode Defines */
#define	PRH_MGR_DISCOVERABLE_BITMASK	0x3
#define PRH_MGR_CONNECTABLE_BITMASK		0x4
#define PRH_MGR_DISCOVERABLE_BIT		0x1
#define PRH_MGR_LIMDISCOVERABLE_BIT		0x2
#define PRH_MGR_CONNECTABLE_BIT			0x4
#define PRH_MGR_SET_DISCOVERABLE(X)		(PRH_MGR_DISCOVERABLE_BIT & (X))
#define PRH_MGR_SET_GENDISCOVERABLE(X)	(PRH_MGR_DISCOVERABLE_BIT & (X)) && !((PRH_MGR_LIMDISCOVERABLE_BIT & (X))>>1)
#define PRH_MGR_SET_LIMDISCOVERABLE(X)	(PRH_MGR_DISCOVERABLE_BIT & (X)) && ((PRH_MGR_LIMDISCOVERABLE_BIT & (X))>>1)
#define PRH_MGR_SET_CONNECTABLE(X)		((PRH_MGR_CONNECTABLE_BIT & (X))>>2)

/* SCAN ENABLE Defines */
#define PRH_MGR_NOSCANENABLE		0x00
#define PRH_MGR_INQUIRYSCANENABLE	0x01 
#define PRH_MGR_PAGESCANENABLE		0x02
#define PRH_MGR_SCANENABLE_OFF(X)		((X == PRH_MGR_NOSCANENABLE))
#define PRH_MGR_INQUIRYSCANENABLE_ON(X) ((X) & PRH_MGR_INQUIRYSCANENABLE )
#define PRH_MGR_PAGESCANENABLE_ON(X)	(((X) &  PRH_MGR_PAGESCANENABLE ) >>1)

/* mode flag Defines */
#define PRH_MGR_SETMODEPENDING_BIT		0x1
#define PRH_MGR_SCANENABLEPENDING_BIT	0x2
#define PRH_MGR_IACLAPPENDING_BIT		0x4
#define PRH_MGR_SETMODEPENDING(X)		(PRH_MGR_SETMODEPENDING_BIT & (X))
#define PRH_MGR_SCANENABLEPENDING(X)	((PRH_MGR_SCANENABLEPENDING_BIT & (X))>>1)
#define PRH_MGR_IACLAPPENDING(X)		((PRH_MGR_IACLAPPENDING_BIT & (X))>>2)
#define PRH_MGR_MODEPENDING(X)			(PRH_MGR_IACLAPPENDING(X) | PRH_MGR_SCANENABLEPENDING(X))

#define MAX_5_SLOT_TOKEN_RATE 723
#define MAX_3_SLOT_TOKEN_RATE 585
#define MAX_1_SLOT_TOKEN_RATE 172

typedef void (*t_inquiryCallbackFunc)(struct st_t_BT_ConnectionInfo *newDevice, u_int8 status);

/* GLOBAL VARIABLES */
extern u_int8 prh_mgr_HCIversion;
extern u_int16 prh_mgr_HCIrevision;
extern u_int8 prh_mgr_LMPversion;
extern u_int16 prh_mgr_LMPsubversion;
extern u_int16 prh_mgr_ManufacturerId;
extern u_int8 *prh_mgr_LocalName;
/*  local device configuration information */
extern t_bdaddr prh_mgr_LocalBDADDR;
extern u_int8 prh_mgr_localLMPFeatures[];
extern u_int8 prh_mgr_currentMode;
extern u_int8 prh_mgr_currentRole;
extern u_int8 prh_mgr_periodicInquiryModeActive;
extern u_int32 prh_mgr_periodicInquiryModeLap;
extern u_int8 prh_mgr_inquiryInProgress;

extern u_int8 prh_mgr_autoAcceptAll;
extern u_int8 prh_mgr_authenticationMode;
extern u_int16 prh_mgr_connectionAcceptTimeout;
extern u_int16 prh_mgr_pageTimeout;
extern u_int8 prh_mgr_mode_flag;
extern u_int8 prh_mgr_scan_enable;
extern u_int8 prh_mgr_scan_enable_pending;
extern u_int8 prh_mgr_liac_present;
extern u_int8 prh_mgr_liac_present_pending;

extern u_int16 prh_mgr_numDeviceDatabaseEntries;
extern u_int8 prh_hci_security_mode_pending;



/* API FUNCTIONS */

/* Startup/shutdown functions */ 
APIDECL1 t_api APIDECL2 MGR_Init(u_int32 flags); 
APIDECL1 t_api APIDECL2 MGR_Shutdown(u_int32 flags);
APIDECL1 t_api APIDECL2 MGR_InitSpecific();

/* Link Control functions */
APIDECL1 t_api APIDECL2 MGR_AddSCO(u_int16 aclHandle, u_int16 *scoHandle, u_int16 packetType, void (*callbackFunc)(u_int16 *scoHandle, t_api status));
APIDECL1 t_api APIDECL2 MGR_ReleaseSCO(u_int16 handle, u_int8 reason, void (*callbackFunc)(u_int16 scoHandle, t_api status));
/* Support for Hardware Codec SCOs where present */
t_api MGR_AddHWSCO(u_int16 aclHandle, u_int16 *scoHandle, u_int16 packetType, u_int32 hwConfig, void (*callbackFunc)(u_int16 *scoHandle, t_api status));
APIDECL1 t_api APIDECL2 MGR_SetLinkFlags(u_int32 flags);
APIDECL1 u_int32 APIDECL2 MGR_GetLinkFlags(void);

/* Inquiry */
APIDECL1 t_api APIDECL2 MGR_Inquiry(u_int8 inquiryLength, u_int32 accessCode, u_int8 maxResults, void (*callbackFunc)(struct st_t_BT_DeviceEntry *newDevice, t_api status));

/* Device Discovery */
APIDECL1 t_api APIDECL2 MGR_InitDeviceDiscovery(u_int8 IsNameNeeded, u_int32 classOfdevice, u_int8 discoveryLength, u_int32 accessCode, u_int8 maxResults, void (*callbackFunc)(struct st_t_BT_DeviceEntry *newDevice, t_api status));
APIDECL1 t_api APIDECL2 MGR_InquiryCancel(void (*InquiryCancelCallBackfun)(int num, int  status));
APIDECL1 u_int32 	APIDECL2 MGR_GetSpecificDeviceClass(t_bdaddr addr);

/* Informational functions */
APIDECL1 t_api APIDECL2 MGR_GetRemoteName(t_bdaddr address, char *outBuffer, u_int8 bufLen, u_int8 *nameLen, void (*callbackFunc)(t_bdaddr address, t_api status));
APIDECL1 t_api APIDECL2 MGR_GetLocalFeatures(u_int8 *outBuffer);
APIDECL1 t_api APIDECL2 MGR_GetLocalBDAddress(t_bdaddr *address);
APIDECL1 t_api APIDECL2 MGR_GetLocalName(char *outBuffer, u_int8 bufLen, u_int8 *nameLen);
APIDECL1 t_api APIDECL2 MGR_GetDeviceVersion(t_MGR_HCVersion *ver);
APIDECL1 char * APIDECL2 MGR_GetHostVersion(void);
APIDECL1 t_api APIDECL2 MGR_ChangeLocalName(u_int8 *name);
APIDECL1 t_api APIDECL2 MGR_SetDeviceInfo(t_bdaddr address,  u_int8 *deviceName, u_int8 *linkKey,  u_int8 keyType);

/* TCI extensions functions*/
APIDECL1 t_api APIDECL2 MGR_WriteLocalDeviceAddress(u_int8 *address, void (*callbakFunc)(t_api status));	/* 00:00:00:00:00 */
APIDECL1 t_api APIDECL2 MGR_ReadPHYRegister(u_int8 flag, u_int32 address, 
							void(*callbakFunc)(t_api status, int result));

APIDECL1 t_api APIDECL2 MGR_WritePHYRegister(u_int8 flag, u_int32 address, u_int8 count, 
							u_int32 *value, void (*callbakFunc)(t_api status));

APIDECL1 t_api APIDECL2 MGR_BaseBand_TXTest( u_int8 hop_frq, u_int8 msg_sco, u_int8 msg_burst,
							u_int8 msg_type, u_int8 power_level,u_int16 PRbs_init, u_int16 msg_data,
							u_int8 tx_packet_type, u_int8 EDR_mode, u_int32 tx_len,
							u_int8 am_addr, u_int32 syncword_low, u_int32 syncword_hight, 
							void (*callbakFunc)(t_api status));

APIDECL1 t_api APIDECL2 MGR_BaseBand_RXTest(void(*callbackFunc)(t_api status));

APIDECL1 t_api APIDECL2 MGR_BaseBand_TestEnd(void(*callbackFunc)(t_api status));

/* L2CAP MTU Selection based on PSM */
APIDECL1 u_int16 APIDECL2 MGR_HintMTU(u_int16 psm);


/* General Mode Setting */
APIDECL1 t_api APIDECL2 MGR_SetMode(u_int8 mode, void (*callbackFunc)(t_api status));
APIDECL1 t_api APIDECL2 MGR_GetMode(u_int8 *mode);
/* Connectability Mode */
APIDECL1 t_api APIDECL2 MGR_GetConnectableMode(u_int8 *mode);
APIDECL1 t_api APIDECL2 MGR_SetConnectableMode(u_int8 mode, void (*callbackFunc)(t_api status));

/* Discoverability Mode */
APIDECL1 t_api APIDECL2 MGR_GetDiscoverableMode(u_int8 *mode);
APIDECL1 t_api APIDECL2 MGR_SetDiscoverableMode(u_int8 mode, void (*callbackFunc)(t_api status));  


/* NON-API FUNCTIONS */
t_api prh_manager_layer_ctrl(u_int8 flags);

t_api prh_mgr_add_sco(u_int16 aclHandle, u_int16 *scoHandle, u_int8 scoType, u_int16 packetType, u_int32 hwConfig, void (*callbackFunc)(u_int16 *scoHandle, t_api status));

/* ACL functions */
t_api prh_mgr_createACL(t_bdaddr target, u_int16 psm, u_int16 *handle, void (*callbackFunc)(tid_t transactionId, t_api status), tid_t transactionId);
t_api prh_mgr_releaseACL(u_int16 handle, u_int8 reason, void (*callbackFunc)(struct st_t_BT_ConnectionInfo *dbEntry, t_api status));
/* Link Policy Related */
t_api prh_mgr_holdAllOpenACLs(u_int16 minHoldInterval, u_int16 maxHoldInterval, void (*callbackFuncStart)(t_api status), void (*callbackFuncEnd)(t_api status));
t_api prh_mgr_holdACL(u_int16 aclHandle, u_int16 maxHoldInterval, u_int16 minHoldInterval, void (*callbackFuncStart)(t_api status, u_int16 aclHandle, u_int16 interval), void (*callbackFuncEnd)(t_api status, u_int16 aclHandle));
t_api prh_mgr_parkACL(u_int16 aclHandle, u_int16 beaconMaxInterval, u_int16 beaconMinInterval, void (*callbackFunc)(t_api status, u_int16 aclHandle, u_int16 interval));
t_api prh_mgr_exitParkACL(u_int16 aclHandle, void (*callbackFunc)(t_api status, u_int16 aclHandle));
t_api prh_mgr_sniffACL(u_int16 aclHandle, u_int16 maxSniffInterval, u_int16 minSniffInterval, u_int16 sniffAttempt, u_int16 sniffTimeout, void (*callbackFunc)(t_api status, u_int16 aclHandle, u_int16 interval));
t_api prh_mgr_exitSniffACL(u_int16 aclHandle, void (*callbackFunc)(t_api status, u_int16 aclHandle));

void prh_mgr_handle_HCI_evMODECHANGE(u_int16 aclHandle, u_int8 eventStatus, u_int8 currentMode, u_int16 interval);

t_api prh_mgr_setupPendingCommandStatus(u_int8 hciCmdIndex, struct st_t_BT_ConnectionInfo *devDbEntry);

/* Callback functions */
t_api prh_mgr_callbackHCI_Ctrl(u_int8 *pdu);
t_api prh_mgr_callbackHCI_SCO(u_int16 handle, struct host_buf *data);
void prh_mgr_callback_DeviceDiscovery(struct st_t_BT_DeviceEntry *newDevice, t_api status);
void prh_mgr_createACL_callback_StartholdAllOpenACLs(t_api status);
void prh_mgr_createACL_callback_EndholdAllOpenACLs(t_api status);
void prh_mgr_inquiry_callback_holdAllOpenACLs(t_api status);
t_api prh_mgr_registerACLConnectionIndCallback(void (*callbackFunc)(t_bdaddr address, u_int16 handle));
t_api prh_mgr_registerACLQoSViolationCallback(void (*callbackFunc)(u_int16 handle));
t_api prh_mgr_registerACLDataLostCallback(void (*callbackFunc)(u_int16 handle));


void prh_mgr_discoveryTimerCallBack(void * handle);


/* QoS functions */
t_api prh_mgr_updateFlushTimeout(u_int16 handle, u_int16* outFlushTimeout, void (*flushTOCallback)(tid_t transactionId, t_api status_t), tid_t transactionId);
t_api prh_mgr_updateLinkSupervisionTimeout(u_int16 handle, u_int16* linkTimeout, void (*linkTOCallback)(tid_t, t_api, u_int16), tid_t transactionId);
t_api prh_updateQoS(u_int16 handle,t_L2_ChannelFlowSpec *p_flowSpec , void (*updateQoSCallback)(tid_t, t_L2_ChannelFlowSpec *, t_api), tid_t transactionId);


void prh_manager_shutdown(u_int32 flags);
t_api prh_mgr_init(void);



extern t_pMutex prh_mgr_hold_acl_lock;

extern u_int8 *MGR_LocalName;

#endif /*__MGR_CORE_H*/
