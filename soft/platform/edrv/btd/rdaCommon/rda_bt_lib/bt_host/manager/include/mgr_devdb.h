/*
 * MODULE NAME:    mgr_devdb.h
 * DESCRIPTION:    Host Manager Device Database Functionality
 * AUTHOR:         Martin Mellody
 * DATE:           23 May 2000
 *
 * SOURCE CONTROL: $Id: mgr_devdb.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */


#ifndef __MGR_DEVDB_H
#define __MGR_DEVDB_H

#include "host_config.h"

#include "papi.h"
#include "host_types.h"
#include "host_error.h"
#include "hci_primitives.h"
#include "l2cap_primitives.h"

#define MGR_DEVDB_DISCARDABLE_THRESHOLD     16 
#define MGR_DEVDB_USER_POINTER          64
#define MGR_DEVDB_CALLBACK_PENDING      32
#define MGR_DEVDB_CONNECTION_ACTIVE     16
#define MGR_DEVDB_PINCODE_STORED        8 
#define MGR_DEVDB_LINKKEY_STORED        4   
#define MGR_DEVDB_TRUSTLEVEL_STORED     2
#define MGR_DEVDB_DEVICENAME_STORED     1

/* remote device database */
#define ROLE_SLAVE	1
#define ROLE_MASTER 2

#define MGR_MAXNUM_SCOHANDLES	3

/* The first 4 values match those defined in HCI Spec. */
#define MGR_DEVACTIVE			0
#define MGR_DEVHOLD				1
#define MGR_DEVSNIFF			2
#define MGR_DEVPARK				3
#define MGR_DEVPENDINGHOLD		14
#define MGR_DEVPENDINGSNIFF		15
#define MGR_DEVPENDINGEXITSNIFF	16
#define MGR_DEVPENDINGPARK		17
#define MGR_DEVPENDINGEXITPARK	18

#define CONN_NONE	           0 
#define CONN_ACTIVE	            1 
#define CONN_CLOSED	            2
#define CONN_PENDING	        3
#define CONN_RELEASEPENDING     4 
#define CONN_PENDINGHOLDALLACLS 5

#define CRPYT_DISABLED	1
#define CRYPT_P2PONLY	2
#define CRYPT_ALL	3

#define LINKKEY_MASTER		1
#define LINKKEY_SEMIPERM	2


#define BOND_INACTIVE				1
#define BOND_AUTH_PENDING			2	
#define BOND_CONN_PENDING			3
#define BOND_AUTH_CONN_PENDING		4
#define BOND_RELEASE_CONN_PENDING	5
#define BOND_INQUIRY_PENDING		6
#define BOND_INQUIRY_COMPLETE		7


#define LINKKEY_SIZE 16

/* Device Trust Level */
#define MGR_UNKNOWN   1
#define MGR_UNTRUSTED 2
#define MGR_TRUSTED   3

#define AUTH_IDLE    1
#define AUTH_PENDING 2

#define SET_ENCRYPT_IDLE    1
#define SET_ENCRYPT_PENDING 2

#define MAX_PIN_LEN 16
#define DEFAULT_FLUSH_TIMEOUT        0x0000  
#define MAX_FLUSH_TIMEOUT            0x07FF
#define DEFAULT_LINK_SUP_TIMEOUT     0x7D00
#define MAX_DEFAULT_LINK_SUP_TIMEOUT 0xFFFF

/* Used for Writing Automatic Flush Timeout */
#define FLUSH_TO_UNSET       0
#define FLUSH_TO_SET_PENDING 1
#define FLUSH_TO_SET         2
/* Used for Writing Link Supervision Timeout values */
#define LINK_TO_UNSET        0
#define LINK_TO_SET_PENDING  1
#define LINK_TO_SET          2


/* Remote Device PIN Code Types */
#define MGR_ALLZEROPIN	  0
#define MGR_PRESETPIN     1
#define MGR_USERREQPIN    2

#define MGR_DEVDB_LINKINFO
#define MGR_DEVDB_

struct st_t_BT_ConnectionInfo;

struct st_t_BT_ConnectionInfo {
  	t_bdaddr bdAddress;


	int role; /* 1 = master, 0 = slave */
	
	u_int8 aclState; /* CONN_OPEN | CONN_CLOSED | CONN_PENDING */
	u_int16 aclHandle;

    u_int8 scoState[3]; /* CONN_OPEN | CONN_CLOSED | CONN_PENDING */
	u_int16 scoHandle[3];	

	u_int8 deviceMode; /* DEV_SNIFF | DEV_HOLD | DEV_PARK | DEV_ACTIVE */
	u_int8 encryptionMode; /* CRPYT_DISABLED | CRYPT_P2PONLY | CRYPT_ALL */
	
	u_int8 trustLevel;

	u_int8 linkKeyType; /* MGR_COMBINATION | MGR_LOCALUNITKEY | MGR_REMOTEUNITKEY */
	u_int8 *linkKey;
	
	u_int8 pinType; /* PIN_ALL_ZERO | FIXED | USER_ENTRY */
	u_int8 pinLength; /* 1-16 Bytes */
	u_int8 *pinCode;
	u_int32 cod;
	/* User API level members end here ... (devicename may be visible on the API in the future)*/
    char *deviceName;
	
	/* inquiry results */
	u_int32 entryAge;
	u_int8 entryUsage;
    u_int8 outstandingCallbacks;
    u_int8 recordState; /* one of PRH_MGR_DEVDB_USED_xxxx */

	tid_t aclCallbackTid[MAX_OUTSTANDING_CALLBACKS];
	void (*aclCallbackFunc[MAX_OUTSTANDING_CALLBACKS])(tid_t transactionId, t_api status);
	u_int16 *aclCallbackReturn[MAX_OUTSTANDING_CALLBACKS]; /* pointers to the handle variables to be returned */
	void (*aclReleaseCallbackFunc[MAX_OUTSTANDING_CALLBACKS])(struct st_t_BT_ConnectionInfo *dbEntry, t_api status);

	void (*scoCallbackFunc[MAX_OUTSTANDING_CALLBACKS])(u_int16 *scoHandle, t_api status);
	u_int16 *scoCallbackReturn[MAX_OUTSTANDING_CALLBACKS]; /* pointers to the handle variables to be returned */
	void (*scoReleaseCallbackFunc[3])(u_int16 scoHandle, t_api status);
	
	/* callback data for MGR_GetRemoteName primitve */
	void (*nameCallbackFunc[MAX_OUTSTANDING_CALLBACKS])(t_bdaddr address, t_api status);
	u_int8 nameCallbackBufLen[MAX_OUTSTANDING_CALLBACKS];
	u_int8 *nameCallbackNameLen[MAX_OUTSTANDING_CALLBACKS];
	u_int8 *nameCallbackOutBuffer[MAX_OUTSTANDING_CALLBACKS];
	
	
	/* For setting the Flush Timeout on an ACL */
	u_int8 aclFlushTimeoutFlag;
	u_int16 aclFlushTimeout;
	tid_t flushTOCallbackTid;
	void (*flushTOCallbackFunc)(tid_t transactionId, t_api status);
	
	/* For setting the Link Supervision Timeout on an ACL */
	u_int8 aclLinkSupTimeoutFlag;
	u_int16 aclLinkSupTimeout;
	u_int16 aclLinkSupTimeoutNew;
	tid_t aclLinkSupTOCallbackTid;
	void (*aclLinkSupTOCallbackFunc)(tid_t, t_api, u_int16);
	
	/* For setting the QoS parameters for an ACL */
	t_L2_ChannelFlowSpec *qosParams;
	t_L2_ChannelFlowSpec *qosParamsNew;
	tid_t qosSetupCallbackTid;
	void (*qosSetupCallbackFunc)(tid_t, t_L2_ChannelFlowSpec*, t_api);
	
	u_int32 accessReqCallbackHandle[MAX_OUTSTANDING_CALLBACKS];
	callbackFuncPtr_t accessReqCallbackFunc[MAX_OUTSTANDING_CALLBACKS];
	u_int8 accessReqCallbackEncryptOn[MAX_OUTSTANDING_CALLBACKS];         
	u_int8 accessReqAuthState;
	u_int8 accessReqEncryptState;
	
	struct st_t_BT_Authorisation  *authorisationInfo[MAX_OUTSTANDING_CALLBACKS];
	
	void (*bondingCallbackFunc[MAX_OUTSTANDING_CALLBACKS])(t_bdaddr address, t_api status);
	u_int8 bondingState;  /* BOND_INACTIVE | BOND_PENDING */
	
	u_int8 updated; /* set to 0 when this record is referenced or modified, incremented periodically */
	u_int8 inquiryResponded;


	void (*holdACLCallbackFunc)(t_api status, u_int16 aclHandle, u_int16 interval);
	void (*exitHoldACLCallbackFunc)(t_api status, u_int16 aclHandle);
	void (*sniffACLCallbackFunc)(t_api status, u_int16 aclHandle, u_int16 interval);
	void (*exitSniffACLCallbackFunc)(t_api status, u_int16 aclHandle);
	void (*parkACLCallbackFunc)(t_api status, u_int16 aclHandle, u_int16 interval);
	void (*exitParkACLCallbackFunc)(t_api status, u_int16 aclHandle);

#if 0 /* code unused until WUG implementation complete */
#if PRH_TCS_GATEWAY
	u_int8 *tcsNum; /* number for TCS Terminal device */
	struct st_t_TCS_TLPeer *TLPeer;
#endif
#endif

	struct st_t_BT_ConnectionInfo *nextDevice; /* linked list */
};


/*** API Level Structure (for backward compatability with API declarations):: */
typedef struct st_t_BT_ConnectionEntry {

	/* WARNING - this structure MUST match the layout of the first part of the st_t_BT_ConnectionInfo structure above */
  	t_bdaddr bdAddress;


	int role; /* 1 = master, 0 = slave */
	
	u_int8 aclState; /* CONN_OPEN | CONN_CLOSED | CONN_PENDING */
	u_int16 aclHandle;

    u_int8 scoState[3]; /* CONN_OPEN | CONN_CLOSED | CONN_PENDING */
	u_int16 scoHandle[3];	

	u_int8 deviceMode; /* DEV_SNIFF | DEV_HOLD | DEV_PARK | DEV_ACTIVE */
	u_int8 encryptionMode; /* CRPYT_DISABLED | CRYPT_P2PONLY | CRYPT_ALL */
	
	u_int8 trustLevel;

	u_int8 linkKeyType; /* MGR_COMBINATION | MGR_LOCALUNITKEY | MGR_REMOTEUNITKEY */
	u_int8 *linkKey;
	
	u_int8 pinType; /* PIN_ALL_ZERO | FIXED | USER_ENTRY */
	u_int8 pinLength; /* 1-16 Bytes */
	u_int8 *pinCode;

  	u_int32 cod;

}t_BT_ConnectionEntry;


/* GLOBAL VARIABLES */
extern struct st_t_BT_ConnectionInfo *prh_device_list_head;


/* API FUNCTIONS */

/* Remote Device Security Settings */
APIDECL1 t_api APIDECL2 MGR_SetDeviceTrustLevel(t_bdaddr address, u_int8 trustLevel);
APIDECL1 u_int8 APIDECL2 MGR_GetDeviceTrustLevel(t_bdaddr address);
APIDECL1 t_api APIDECL2 MGR_SetRemoteDevicePIN(t_bdaddr address, u_int8 pinType, u_int8 pinLength, u_int8 *pinCode);
APIDECL1 t_api APIDECL2 MGR_GetRemoteDevicePIN(t_bdaddr address, u_int8 *pinType, u_int8 *buffer, u_int8 bufLen, u_int8 *pinLen);
APIDECL1 t_api APIDECL2 MGR_ReadLinkKeyType(t_bdaddr address, u_int8 *linkKeyType);

/* Device Database Access */
APIDECL1 t_api APIDECL2 MGR_RetrieveDeviceList(t_bdaddr *addressBuffer, u_int16 buflen, u_int16 *numReturned, u_int16 *totalNum);
APIDECL1 t_api APIDECL2 MGR_GetNextConnectionRecord(struct st_t_BT_ConnectionEntry *current, struct st_t_BT_ConnectionEntry **next);
APIDECL1 t_api APIDECL2 MGR_RemoveConnectionRecord(struct st_t_BT_ConnectionEntry *entry, u_int32 flags);
APIDECL1 t_api APIDECL2 MGR_DeleteLinkKey(t_bdaddr address);
APIDECL1 t_api APIDECL2 MGR_SetDeviceInfo(t_bdaddr address,  u_int8 *linkKey,  u_int8 keyType);

/* NON-API FUNCTIONS */

/* Device Database Access */
struct st_t_BT_ConnectionInfo *prh_mgr_get_unused_entry(u_int8 severity);
t_api prh_mgr_set_callback(u_int8 *offset, void **array, void *callbackFunc, struct st_t_BT_ConnectionInfo *dbEntry);
void prh_mgr_get_callback(u_int8 *offset, void **array, void **callbackFunc, struct st_t_BT_ConnectionInfo *dbEntry);
t_api prh_mgr_setupPendingCommandStatus(u_int8 hciCmdIndex, struct st_t_BT_ConnectionInfo *devDbEntry);
t_api prh_mgr_release_deviceDatabase(u_int32 flags);
struct st_t_BT_ConnectionInfo *prh_mgr_find_deviceEntryByAddress(t_bdaddr addr);
struct st_t_BT_ConnectionInfo *prh_mgr_return_deviceEntryByAddress(t_bdaddr addr);
struct st_t_BT_ConnectionInfo *prh_mgr_get_deviceEntryByHandle(u_int16 handle, u_int8 *linkType);
struct st_t_BT_ConnectionInfo *prh_mgr_get_deviceEntryByName(char *name);
struct st_t_BT_ConnectionInfo *prh_mgr_addDeviceEntry(t_bdaddr addr);
u_int8 prh_mgr_isDeviceConnected(t_bdaddr addr);
void prh_mgr_ageDeviceDB(void);
void prh_mgr_cullDeviceDB(u_int8 threshold);
void prh_mgr_updateLinkKeyEntry(t_bdaddr addr, u_int8 *linkKey, u_int8 linkKeyType);
u_int8 *prh_mgr_getLinkKeyEntry(t_bdaddr addr);
#if PRH_TCS_GATEWAY 
struct st_t_BT_ConnectionInfo *prh_mgr_get_deviceEntryByTCSNum(u_int8 *tcsNum, u_int8 *uniqueEntry);
#endif
struct st_t_BT_ConnectionInfo *prh_mgr_getActiveACLDeviceEntry(void);

#include "mgr_secdb.h"

#endif /*__MGR_DEVDB_H*/

