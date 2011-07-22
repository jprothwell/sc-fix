/*
 * MODULE NAME:    mgr_secdb.h
 * DESCRIPTION:    Host Manager Service Security Database Functionality
 * AUTHOR:         Tom Howley
 * DATE:           30 June 2000
 *
 * SOURCE CONTROL: $Id:
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */

#ifndef __MGR_SECDB_H
#define __MGR_SECDB_H

#include "host_config.h"

#include "host_error.h"
#include "papi.h"
#include "hci_primitives.h"
#include "host_types.h"
#include "mgr_devdb.h"
#include "mgr_core.h"
#include "platform_config.h"
#include "mgr_inqdb.h"

/* Local PIN type */
#define MGR_VARIABLEPIN	 0
#define MGR_FIXEDPIN	 1

/* SECURITY MODES */
#define MGR_NOSECURITY   1
#define MGR_SERVICELEVEL 2
#define MGR_LINKLEVEL    3

/* PAIRING MODES */
#define MGR_PAIRABLE	  1
#define MGR_NONPAIRABLE	  2

/* Direction of Access Requests */
#define MGR_INCOMING 0
#define MGR_OUTGOING 1

/* Service Types */
#define MGR_APPSERVICE   1
#define MGR_PROTOSERVICE 2



/* Security Level Attributes */
#define IN_AUTHOR_ON_MASK   0x01
#define IN_AUTHEN_ON_MASK   0x02
#define IN_ENCRYPT_ON_MASK  0x04
#define OUT_AUTHOR_ON_MASK  0x08
#define OUT_AUTHEN_ON_MASK  0x10
#define OUT_ENCRYPT_ON_MASK 0x20

/* These should be defined somewhere else - HCI constants? */
#define ENCRYPT_ENABLE_OFF 0
#define ENCRYPT_ENABLE_ON  1
 
/* Link Key Types (only provided by 1.1 Host Controller) */
#define MGR_COMBINATIONKEY 0
#define MGR_LOCALUNITKEY   1
#define MGR_REMOTEUNITKEY  2



#define IN_AUTHOR_ON(X) ((X) & IN_AUTHOR_ON_MASK)
#define IN_AUTHEN_ON(X) (((X) & IN_AUTHEN_ON_MASK)>> 1)
#define IN_ENCRYPT_ON(X) (((X) & IN_ENCRYPT_ON_MASK) >> 2)
#define OUT_AUTHOR_ON(X) (((X) & OUT_AUTHOR_ON_MASK) >> 3)
#define OUT_AUTHEN_ON(X) (((X) & OUT_AUTHEN_ON_MASK) >> 4)
#define OUT_ENCRYPT_ON(X) (((X) & OUT_ENCRYPT_ON_MASK) >> 5) 

#define AUTHOR_ON(X,Y) (((Y) == MGR_INCOMING) ? IN_AUTHOR_ON((X)) : OUT_AUTHOR_ON((X)))
#define AUTHEN_ON(X,Y) (((Y) == MGR_INCOMING) ? IN_AUTHEN_ON((X)) : OUT_AUTHEN_ON((X)))
#define ENCRYPT_ON(X,Y) (((Y) == MGR_INCOMING) ? IN_ENCRYPT_ON((X)) : OUT_ENCRYPT_ON((X)))




/* Service Database Entry */
struct st_t_mgr_secdbentry {
  struct st_t_mgr_secdbentry *nextService;
  u_int8 serviceType;
  union {
    char *name;
    u_int16 protocolId;
  }service;
  u_int16 multiplexProtocolId;
  u_int32 multiplexChannelId;
  u_int8 securityLevel;
};


/* GLOBAL VARIABLES */
extern struct st_t_mgr_secdbentry *prh_service_list_head;
extern u_int8 prh_local_pin_type;
extern u_int8 prh_security_mode;
extern u_int8 prh_security_mode_pending;
extern void (*prh_callback_set_pinType)(t_api status);
extern void (*prh_callback_get_pinType)(t_api status, u_int8 pinType); 
extern void (*setSecurityModeCallback)(t_api status);

/* API FUNCTIONS */
/* Bonding */
APIDECL1 t_api APIDECL2 MGR_InitBonding(t_bdaddr address, void (*callbackFunc)(t_bdaddr address, t_api status));

/* Service Registration functions */
APIDECL1 t_api APIDECL2 MGR_RegisterAppSecurityLevel(char *serviceName, u_int16 multiplexProtocolId, u_int32 multiplexChannelId, u_int8 securityLevel);
APIDECL1 t_api APIDECL2 MGR_RegisterProtocolSecurityLevel(u_int16 protocolId, u_int16 multiplexProtocolId, u_int32 multiplexChannelId, u_int8 securityLevel);
APIDECL1 t_api APIDECL2 MGR_DeregisterAppSecurityLevel(char *serviceName);
APIDECL1 t_api APIDECL2 MGR_DeregisterProtocolSecurityLevel(u_int16 protocolId);

/* Access Request functions */
APIDECL1 t_api APIDECL2 MGR_ApplicationOutgoingAccessRequest(char *serviceName, t_bdaddr address, u_int32 cbHandle, void (*callbackFunc)(u_int32 cbHandle, t_api status));
APIDECL1 t_api APIDECL2 MGR_ProtocolOutgoingAccessRequest(u_int16 protocolId, t_bdaddr address, u_int32 cbHandle, void (*callbackFunc)(u_int32 cbHandle, t_api status));
APIDECL1 t_api APIDECL2 MGR_MultiplexOutgoingAccessRequest(u_int16 multiplexProtocolId, u_int32 multiplexChannelId, t_bdaddr address, u_int32 cbHandle, void (*callbackFunc)(u_int32 cbHandle, t_api status));
APIDECL1 t_api APIDECL2 MGR_IncomingAccessRequest(u_int16 multiplexProtocolId, u_int32 multiplexChannelId, t_bdaddr address, u_int32 cbHandle, void (*callbackFunc)(u_int32 cbHandle, t_api status));

/* Authorisation/PIN Handling functions */
APIDECL1 t_api APIDECL2 MGR_RegisterAuthorisationHandler(void (*authorisationCallback)(t_BT_Authorisation *authorisationInfo));
APIDECL1 t_api APIDECL2 MGR_RegisterPINHandler(void (*pinCallback)(t_bdaddr address));
APIDECL1 t_api APIDECL2 MGR_DeregisterAuthorisationHandler(void (*authorisationCallback)(t_BT_Authorisation *authorisationInfo));
APIDECL1 t_api APIDECL2 MGR_DeregisterPINHandler(void (*pinCallback)(t_bdaddr address));
APIDECL1 t_api APIDECL2 MGR_AuthorisationResponse(t_BT_Authorisation *authorisationInfo, u_int8 allowConnection, u_int8 makeTrusted);
APIDECL1 t_api APIDECL2 MGR_PINResponse(t_bdaddr address, u_int8 pinSupplied, u_int8 *pinCode, u_int8 pinLen);

APIDECL1 int  APIDECL2 MGR_RegisterAcceptCnnHandle(void (*AcceptCnnCallBack)(t_bdaddr address));	//add by tan, 07-12-2009

APIDECL1 int  APIDECL2 MGR_AcceptConnRequest(t_bdaddr address, int result);



/* Local Device Security Settings */
APIDECL1 t_api APIDECL2 MGR_SetSecurityMode(u_int8 mode, void (*callbackFunc)(t_api status));
APIDECL1 u_int8 APIDECL2 MGR_GetSecurityMode(void);
APIDECL1 t_api APIDECL2 MGR_SetPairingMode(u_int8 mode);
APIDECL1 u_int8 APIDECL2 MGR_GetPairingMode(void);
APIDECL1 t_api APIDECL2 MGR_SetLocalPINType(u_int8 pinType, void (*callbackFunc)(t_api status));
APIDECL1 t_api APIDECL2 MGR_GetLocalPINType(void (*callbackFunc)(t_api status, u_int8 pinType));

/* Service Database Access */
APIDECL1 t_api APIDECL2 MGR_FlushSecurityDatabase(u_int32 flags);



/* NON-API FUNCTIONS */
struct st_t_mgr_secdbentry *prh_mgr_get_next_serviceEntry(struct st_t_mgr_secdbentry *secDbEntry);
void prh_mgr_handle_security_hciEvent(u_int8 *pdu);
void prh_mgr_callback_bondingInquiry(struct st_t_BT_DeviceEntry *deviceInfo, t_api status);
void prh_mgr_callback_bondingACL(tid_t transactionId, t_api status);
void prh_mgr_callback_bondingReleaseACL(struct st_t_BT_ConnectionInfo *dbEntry, t_api status);

#endif /*__MGR_SECDB_H */
