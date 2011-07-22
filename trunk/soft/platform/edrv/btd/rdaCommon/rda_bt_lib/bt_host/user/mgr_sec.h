/***********************************************************************
 *
 * MODULE NAME:    mgr_sec.h
 * DESCRIPTION:    Bluetooth Host Stack BT Manager security API header
 * AUTHOR:		   Thomas Howley, Martin Mellody
 *
 * SOURCE CONTROL: $Id: mgr_sec.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *	   This source code is copyright (c) 2000-2002 Parthus Technologies.
 *	   All rights reserved.
 *
 * REMARKS:
 *	   This file should be incuded via bt.h only.
 *
 ***********************************************************************/


#ifndef BLUETOOTH_SECURITY_API_DECLARED
#define BLUETOOTH_SECURITY_API_DECLARED


/* Remote Device PIN Code Types */
#define MGR_ALLZEROPIN		0
#define MGR_PRESETPIN		1
#define MGR_USERREQPIN		2


/* Local PIN type */
#define MGR_VARIABLEPIN 	0
#define MGR_FIXEDPIN		1


/* Device Trust Level */
#define MGR_UNKNOWN 		1
#define MGR_UNTRUSTED		2
#define MGR_TRUSTED 		3


/* Direction of Access Requests */
#define MGR_INCOMING 0
#define MGR_OUTGOING 1


/* Security Registration Attributes */
#define MGR_NOSERVICESECURITY	0x00
#define MGR_INAUTHENTICATION	0x02
#define MGR_OUTAUTHENTICATION	0x10
#define MGR_AUTHENTICATION		(MGR_INAUTHENTICATION|MGR_OUTAUTHENTICATION)
#define MGR_INAUTHORISATION 	(0x01|MGR_INAUTHENTICATION)
#define MGR_OUTAUTHORISATION	(0x08|MGR_OUTAUTHENTICATION)
#define MGR_AUTHORISATION		(MGR_INAUTHORISATION|MGR_OUTAUTHORISATION)
#define MGR_INENCRYPTION		(0x04|MGR_INAUTHENTICATION)
#define MGR_OUTENCRYPTION		(0x20|MGR_OUTAUTHENTICATION)
#define MGR_ENCRYPTION			(MGR_INENCRYPTION|MGR_OUTENCRYPTION)


/* Link Key Types (only provided by 1.1 Host Controller) */
#define MGR_COMBINATIONKEY 0
#define MGR_LOCALUNITKEY   1
#define MGR_REMOTEUNITKEY  2


/* Security Modes for MGR_SetSecurityMode(), MGR_GetSecurityMode() */
#define MGR_NOSECURITY		1
#define MGR_SERVICELEVEL	2
#define MGR_LINKLEVEL		3

/* Pairing Modes for MGR_SetPairingMode(), MGR_GetPairingMode() */
#define MGR_PAIRABLE		1
#define MGR_NONPAIRABLE 	2


/* Service Types */
#define MGR_APPSERVICE		1
#define MGR_PROTOSERVICE	2


/* Multiplexing Protocol Identifiers */

#define MGR_L2CAP	1
#define MGR_RFCOMM	2
#define MGR_SDP 	3
#define MGR_TCSBIN	4


typedef struct st_t_BT_Authorisation {
  u_int8 direction; /* INCOMING or OUTGOING */
  t_bdaddr address;
  u_int8 serviceType;
  union {
	char *serviceName;
	u_int16 protocolId;
  } service;
  u_int16 multiplexProtocolId;
  u_int32 multiplexChannelId;
} t_BT_Authorisation;


/* Bonding */
APIDECL1 int APIDECL2 MGR_InitBonding(t_bdaddr address, void (*callbackFunc)(t_bdaddr address, int status));
APIDECL1 int APIDECL2 MGR_SetBondingCallBack(t_bdaddr address,  void (*callbackfunc)(t_bdaddr address, t_api status));

/* Service Registration functions */
APIDECL1 int APIDECL2 MGR_RegisterAppSecurityLevel(char *serviceName, u_int16 multiplexProtocolId, u_int32 multiplexChannelId, u_int8 securityLevel);
APIDECL1 int APIDECL2 MGR_DeregisterAppSecurityLevel(char *serviceName);
APIDECL1 int APIDECL2 MGR_RegisterProtocolSecurityLevel(u_int16 protocolId, u_int16 multiplexProtocolId, u_int32 multiplexChannelId, u_int8 securityLevel);
APIDECL1 int APIDECL2 MGR_DeregisterProtocolSecurityLevel(u_int16 protocolId);

/* Access Request functions */
APIDECL1 int APIDECL2 MGR_ApplicationOutgoingAccessRequest(char *serviceName, t_bdaddr address, u_int32 cbHandle, void (*callbackFunc)(u_int32 cbHandle, int status));
APIDECL1 int APIDECL2 MGR_ProtocolOutgoingAccessRequest(u_int16 protocolId, t_bdaddr address, u_int32 cbHandle, void (*callbackFunc)(u_int32 cbHandle, int status));
APIDECL1 int APIDECL2 MGR_MultiplexOutgoingAccessRequest(u_int16 multiplexProtocolId, u_int32 multiplexChannelId, t_bdaddr address, u_int32 cbHandle, void (*callbackFunc)(u_int32 cbHandle, int status));
APIDECL1 int APIDECL2 MGR_IncomingAccessRequest(u_int16 multiplexProtocolId, u_int32 multiplexChannelId, t_bdaddr address, u_int32 cbHandle, void (*callbackFunc)(u_int32 cbHandle, int status));

/* Authorisation/PIN Handling functions */
APIDECL1 int APIDECL2 MGR_RegisterAuthorisationHandler(void (*authorisationCallback)(t_BT_Authorisation *authorisationInfo));
APIDECL1 int APIDECL2 MGR_RegisterPINHandler(void (*pinCallback)(t_bdaddr address));
APIDECL1 int APIDECL2 MGR_DeregisterAuthorisationHandler(void (*authorisationCallback)(t_BT_Authorisation *authorisationInfo));
APIDECL1 int APIDECL2 MGR_DeregisterPINHandler(void (*pinCallback)(t_bdaddr address));
APIDECL1 int APIDECL2 MGR_AuthorisationResponse(t_BT_Authorisation *authorisationInfo, u_int8 allowConnection, u_int8 makeTrusted);
APIDECL1 int APIDECL2 MGR_PINResponse(t_bdaddr address, u_int8 pinSupplied, u_int8 *pinCode, u_int8 pinLen);

/* Local Device Settings */
APIDECL1 u_int8 APIDECL2 MGR_GetSecurityMode(void);
APIDECL1 u_int8 APIDECL2 MGR_GetPairingMode(void);
APIDECL1 int APIDECL2 MGR_SetSecurityMode(u_int8 mode, void (*callbackFunc)(int status));
APIDECL1 int APIDECL2 MGR_SetPairingMode(u_int8 mode);


APIDECL1 int APIDECL2 MGR_SetLocalPINType(u_int8 pinType, void (*callbackFunc)(int status));
APIDECL1 int APIDECL2 MGR_GetLocalPINType(void (*callbackFunc)(int status, u_int8 pinType));

/* Service Database Access */
APIDECL1 int APIDECL2 MGR_FlushSecurityDatabase(u_int32 flags);

/* Remote Device Security Settings */
APIDECL1 int APIDECL2 MGR_SetDeviceTrustLevel(t_bdaddr address, u_int8 trustLevel);
APIDECL1 u_int8 APIDECL2 MGR_GetDeviceTrustLevel(t_bdaddr address);
APIDECL1 int APIDECL2 MGR_SetRemoteDevicePIN(t_bdaddr address, u_int8 pinType, u_int8 pinLength, u_int8 *pinCode);
APIDECL1 int APIDECL2 MGR_GetRemoteDevicePIN(t_bdaddr address, u_int8 *pinType, u_int8 *buffer, u_int8 bufLen, u_int8 *pinLen);
APIDECL1 int APIDECL2 MGR_ReadLinkKeyType(t_bdaddr address, u_int8 *linkKeyType);


#endif /* BLUETOOTH_SECURITY_API_DECLARED */
