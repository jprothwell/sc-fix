/***********************************************************************
 *
 * MODULE NAME:    manager.h
 * DESCRIPTION:    Bluetooth Host Stack BT Manager API header
 * AUTHOR:         Thomas Howley, Martin Mellody
 *
 * SOURCE CONTROL: $Id: manager.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 * REMARKS:
 *     This file should be incuded via bt.h only.
 *
 ***********************************************************************/


#ifndef BLUETOOTH_MANAGER_API_DECLARED
#define BLUETOOTH_MANAGER_API_DECLARED


#ifndef BLUETOOTH_NO_SECURITY
#	include "mgr_sec.h"
#endif



#define MGR_Slots_To_mSec(slots) ((slots)*5/8)
#define MGR_mSec_To_Slots(msec)  ((u_int32)((msec)*8/5))



/* 
	Startup/Shutdown functions
*/


APIDECL1 int APIDECL2 MGR_Init(u_int32 flags);


/* Flags for MGR_Shutdown() */
#define MGR_DETACHALL	0
#define MGR_FORCED		1

APIDECL1 int APIDECL2 MGR_Shutdown(u_int32 flags);




/* 
	Utility functions
*/
#define MGR_ADDRSTRLEN 18 /* The length of the string returned by MGR_PrintBDAddress() */

APIDECL1 int APIDECL2 MGR_PrintBDAddress(t_bdaddr address, char *outBuffer);
APIDECL1 int APIDECL2 MGR_ParseHexBDAddress(char *inString, t_bdaddr *outAddress);



/* 
	Informational functions
*/

typedef struct st_t_MGR_HCVersion {
	u_int8 HCIversion;
	u_int16 HCIrevision;
	u_int8 LMPversion;
	u_int16 LMPsubversion;
	u_int16 ManufacturerId;
} t_MGR_HCVersion;

APIDECL1 int APIDECL2 MGR_GetLocalBDAddress(t_bdaddr *address);
APIDECL1 int APIDECL2 MGR_GetLocalFeatures(u_int8 *outBuffer);
APIDECL1 int APIDECL2 MGR_GetLocalName(char *outBuffer, u_int8 bufLen, u_int8 *nameLen);
APIDECL1 int APIDECL2 MGR_GetDeviceVersion(t_MGR_HCVersion *versionInfo);
APIDECL1 char * APIDECL2 MGR_GetHostVersion(void);

APIDECL1 int APIDECL2 MGR_SetLinkFlags(u_int32 flags);
APIDECL1 u_int32 APIDECL2 MGR_GetLinkFlags(void);

APIDECL1 int APIDECL2 MGR_GetRemoteName(t_bdaddr address, char *outBuffer, u_int8 bufLen, u_int8 *nameLen, void (*callbackFunc)(t_bdaddr address, int status));
APIDECL1 int APIDECL2 MGR_RetrieveDeviceList(t_bdaddr *addressBuffer, u_int16 buflen, u_int16 *numReturned, u_int16 *totalNum);
APIDECL1 int APIDECL2 MGR_GetNextConnectionRecord(struct st_t_BT_ConnectionEntry *current, struct st_t_BT_ConnectionEntry **next);
APIDECL1 int APIDECL2 MGR_RemoveConnectionRecord(struct st_t_BT_ConnectionEntry *entry, u_int32 flags);
APIDECL1 int APIDECL2 MGR_DeleteLinkKey(t_bdaddr address);
APIDECL1 u_int16 APIDECL2 MGR_HintMTU(u_int16 psm);


/* Macros to determine the Local Supported Features of the Host Controller */
#define MGR_3_SLOT_PKT_SUPPORTED(features)                       (features[0] & 0x01)
#define MGR_5_SLOT_PKT_SUPPORTED(features)                       (features[0] & 0x02)
#define MGR_ENCRYPTION_SUPPORTED(features)                       (features[0] & 0x04)
#define MGR_CLOCK_OFFSET_SUPPORTED(features)                     (features[0] & 0x08)
#define MGR_TIMING_ACCURACY_SUPPORTED(features)                  (features[0] & 0x10)
#define MGR_SWITCH_ROLE_SUPPORTED(features)                      (features[0] & 0x20)
#define MGR_HOLD_MODE_SUPPORTED(features)                        (features[0] & 0x40)
#define MGR_SNIFF_MODE_SUPPORTED(features)                       (features[0] & 0x80)
#define MGR_PARK_MODE_SUPPORTED(features)                        (features[1] & 0x01)
#define MGR_RSSI_SUPPORTED(features)                             (features[1] & 0x02)
#define MGR_CHANNEL_QUALITY_DRIVEN_DATA_RATE_SUPPORTED(features) (features[1] & 0x04)
#define MGR_SCO_LINK_SUPPORTED(features)                         (features[1] & 0x08)
#define MGR_HV2_PKTS_SUPPORTED(features)                         (features[1] & 0x10)
#define MGR_HV3_PKTS_SUPPORTED(features)                         (features[1] & 0x20)
#define MGR_U_LAW_SUPPORTED(features)                            (features[1] & 0x40)
#define MGR_A_LAW_SUPPORTED(features)                            (features[1] & 0x80)
#define MGR_CVSD_SUPPORTED(features)                             (features[2] & 0x01)
#define MGR_PAGING_SCHEME_SUPPORTED(features)                    (features[2] & 0x02)
#define MGR_POWER_CONTROL_SUPPORTED(features)                    (features[2] & 0x04)




/* Class of Device */

/* Major Service Classes: bits 23-13 */
#define PRH_LIMITEDDISCOVERYMODE_BIT				0x002000
#define PRH_CODSERVICEBIT_NETWORKING				0x020000
#define PRH_CODSERVICEBIT_RENDERING					0x040000
#define PRH_CODSERVICEBIT_CAPTURING					0x080000
#define PRH_CODSERVICEBIT_OBJECTTRANSFER			0x100000
#define PRH_CODSERVICEBIT_AUDIO						0x200000
#define PRH_CODSERVICEBIT_TELEPHONY					0x400000
#define PRH_CODSERVICEBIT_INFORMATION				0x800000
/* Major Device Classes: bits 12-8 */
#define PRH_CODMAJORDEVICE_MISCELLANEOUS			0x000000
#define PRH_CODMAJORDEVICE_COMPUTER					0x000100
#define PRH_CODMAJORDEVICE_PHONE					0x000200
#define PRH_CODMAJORDEVICE_LANACCESPOINT			0x000300
#define PRH_CODMAJORDEVICE_AUDIO					0x000400
#define PRH_CODMAJORDEVICE_PERIPHERAL				0x000500
#define PRH_CODMAJORDEVICE_UNCLASSIFIED				0x001F00
/* Minor Device Classes: bits 7-2 */
/* Minor Device Class Field - Computer Major Class */
#define PRH_CODMINORDEVICE_COMPUTER_UNCLASSIFIED	0x000000
#define PRH_CODMINORDEVICE_COMPUTER_DESKTOP			0x000004
#define PRH_CODMINORDEVICE_COMPUTER_SERVER			0x000008
#define PRH_CODMINORDEVICE_COMPUTER_LAPTOP			0x00000C
#define PRH_CODMINORDEVICE_COMPUTER_HANDHELDPCPDA	0x000010
#define PRH_CODMINORDEVICE_COMPUTER_PALMSIZEDPCPDA	0x000014
/* Minor Device Class Field - Phone Major Class */
#define PRH_CODMINORDEVICE_PHONE_UNCLASSIFIED		0x000000
#define PRH_CODMINORDEVICE_PHONE_CELLULAR			0x000004
#define PRH_CODMINORDEVICE_PHONE_CORDLESS			0x000008
#define PRH_CODMINORDEVICE_PHONE_SMARTPHONE			0x00000C
#define PRH_CODMINORDEVICE_PHONE_WIREDMODEMORGW		0x000010
/* Minor Device Class Field - LAN AP Major Class: bits 7 - 5*/
#define PRH_CODMINORDEVICE_LANAP_FULLYAVAILABLE		0x000000
#define PRH_CODMINORDEVICE_LANAP_1_17_UTILISED		0x000004
#define PRH_CODMINORDEVICE_LANAP_17_33_UTILISED		0x000008
#define PRH_CODMINORDEVICE_LANAP_33_50_UTILISED		0x00000C
#define PRH_CODMINORDEVICE_LANAP_50_67_UTILISED		0x000010
#define PRH_CODMINORDEVICE_LANAP_67_83_UTILISED		0x000014
#define PRH_CODMINORDEVICE_LANAP_83_99_UTILISED		0x000018
#define PRH_CODMINORDEVICE_LANAP_NOSERVICEAVAILABLE	0x00001C
/* Minor Device Class Field - LAN AP Major Class: bits 4 - 2*/
#define PRH_CODMINORDEVICE_LANAP_UNCLASSIFIED		0x000000
/* Minor Device Class Field - Audio Major Class */
#define PRH_CODMINORDEVICE_AUDIO_UNCLASSIFIED		0x000000
#define PRH_CODMINORDEVICE_AUDIO_HEADSETPROFILE		0x000004


/* Service Class Macros */
#define PRH_LIMITEDDISCOVERYMODE_SET(classOfDevice)  ((classOfDevice) &  PRH_LIMITEDDISCOVERYMODE_BIT)
#define PRH_IS_NETWORKINGSERVICE(classOfDevice) ((classOfDevice) & PRH_CODSERVICEBIT_NETWORKING)
#define PRH_IS_RENDERINGSERVICE(classOfDevice) ((classOfDevice) & PRH_CODSERVICEBIT_RENDERING)
#define PRH_IS_CAPTURINGSERVICE(classOfDevice) ((classOfDevice) & PRH_CODSERVICEBIT_CAPTURING)
#define PRH_IS_OBJECTTRANSFERSERVICE(classOfDevice) ((classOfDevice) & PRH_CODSERVICEBIT_OBJECTTRANSFER)
#define PRH_IS_AUDIOSERVICE(classOfDevice) ((classOfDevice) & PRH_CODSERVICEBIT_AUDIO)
#define PRH_IS_TELEPHONYSERVICE(classOfDevice) ((classOfDevice) & PRH_CODSERVICEBIT_TELEPHONY)
#define PRH_IS_INFORMATIONSERVICE(classOfDevice) ((classOfDevice) & PRH_CODSERVICEBIT_INFORMATION)

/* Major Device Class Macros */
#define MAJORDEVICECLASS_MASK		0x00001F00 /* bits 12 - 8 */
#define PRH_IS_PHONEMAJORDEVICE(classOfDevice)  ( ((classOfDevice) & MAJORDEVICECLASS_MASK) == PRH_CODMAJORDEVICE_PHONE)

/* Minor Device Class Macros */
#define MINORDEVICECLASS_MASK		0x000000FC  /* bits 7 - 2 */ 
#define PRH_IS_CORDLESSMINORDEVICE(classOfDevice) ( ((classOfDevice) & MINORDEVICECLASS_MASK) == PRH_CODMINORDEVICE_PHONE_CORDLESS)
#define PRH_IS_WIREDMODEDORGWMINORDEVICE(classOfDevice) ( ((classOfDevice) & MINORDEVICECLASS_MASK) == PRH_CODMINORDEVICE_PHONE_WIREDMODEMORGW)



/* 
	Link Control functions
*/
APIDECL1 int APIDECL2 MGR_AddSCO(u_int16 aclHandle, u_int16 *scoHandle, u_int16 packetType, void (*callbackFunc)(u_int16 *scoHandle, int status));
APIDECL1 int APIDECL2 MGR_ReleaseSCO(u_int16 handle, u_int8 reason, void (*callbackFunc)(u_int16 scoHandle, int status));

/* Support for Hardware Codec SCOs where present */
/*APIDECL1 int APIDECL2 MGR_AddHWSCO(u_int16 aclHandle, u_int16 *scoHandle, u_int16 packetType, u_int32 hwConfig, void (*callbackFunc)(u_int16 *scoHandle, int status));*/


/* 
	Device Discovery functions
*/

/* Discoverability Modes */
#define MGR_NONDISCOVERABLE		0x00
#define MGR_GENERALDISCOVERY	0x01
#define MGR_LIMITEDDISCOVERY	0x03
/* Connectability Modes */
#define MGR_NONCONNECTABLE		0x00
#define MGR_CONNECTABLE			0x04
/* 
	Device Discovery functions
*/
#define MGR_DISCOVERABLE_BIT		0x1
#define MGR_LIMDISCOVERABLE_BIT		0x2
#define MGR_CONNECTABLE_BIT			0x4

/* General Mode Setting */
APIDECL1 int APIDECL2 MGR_SetMode(u_int8 mode, void (*callbackFunc)(int status));
APIDECL1 int APIDECL2 MGR_GetMode(u_int8 *mode);
APIDECL1 int APIDECL2 MGR_GetDiscoverableMode(u_int8 *mode);
APIDECL1 int APIDECL2 MGR_SetDiscoverableMode(u_int8 mode, void (*callbackFunc)(int status));
APIDECL1 int APIDECL2 MGR_GetConnectableMode(u_int8 *mode);
APIDECL1 int APIDECL2 MGR_SetConnectableMode(u_int8 mode, void (*callbackFunc)(int status));



typedef struct st_t_MGR_DeviceEntry {
  t_bdaddr address;
  u_int8 pageScanRepMode;
  u_int8 pageScanPeriodMode;
  u_int8 pageScanMode;
  u_int32 classOfDevice;
  u_int16 clockOffset;
  char *deviceName;
  u_int32 entryAge;
} t_MGR_DeviceEntry;


/* Structure for inquiry & device discovery callback */


APIDECL1 int APIDECL2 MGR_GetNextDeviceRecord(t_MGR_DeviceEntry *current, t_MGR_DeviceEntry **next);
APIDECL1 int APIDECL2 MGR_Inquiry(u_int8 inquiryLength, u_int32 accessCode, u_int8 maxResults, void (*callbackFunc)(t_MGR_DeviceEntry *newDevice, int status));

/* Device Discovery */
APIDECL1 int APIDECL2 MGR_InitDeviceDiscovery(u_int8 discoveryLength, u_int32 accessCode, u_int8 maxResults, void (*callbackFunc)(t_MGR_DeviceEntry *newDevice, int status));


/* SCO types*/
#define PRH_MGR_HV1PACKETTYPE 0x0020
#define PRH_MGR_HV2PACKETTYPE 0x0040
#define PRH_MGR_HV3PACKETTYPE 0x0080

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
#define MGR_LINKFLAGS_IGNORE        (MGR_CONNREQ_IGNORE_ALL|MGR_SECREQ_IGNORE_ALL|MGR_CONNSETUP_ALLOW_SWITCH)



#endif /* BLUETOOTH_MANAGER_API_DECLARED */
