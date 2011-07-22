/*
 * MODULE NAME:    mgr_utils.h
 * DESCRIPTION:    Host Manager Utility Functionality
 * AUTHOR:         Martin Mellody
 * DATE:           10 March 2000
 *
 * SOURCE CONTROL: $Id: mgr_utils.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */

#include "host_config.h"


#include "host_types.h"


#define MGR_Slots_To_mSec(slots) (((slots)*5)>>3)
#define MGR_mSec_To_Slots(msec)  (((msec)*8)/5)


/* Used for determining Local Supported Features of the Host Controller */
#define PRH_SLOT_3_PACKETS_MASK  0x01
#define PRH_SLOT_5_PACKETS_MASK  0x02
#define PRH_ENCRYPTION_MASK      0x04
#define PRH_SLOT_OFFSET_MASK     0x08
#define PRH_TIMING_ACCURACY_MASK 0x10
#define PRH_SWITCH_ROLE_MASK     0x20
#define PRH_HOLD_MODE_MASK       0x40
#define PRH_SNIFF_MODE_MASK      0x80
#define PRH_PARK_MODE_MASK       0x01
#define PRH_RSSI_MASK            0x02
#define PRH_RATE_CHANGE_MASK     0x04
#define PRH_SCO_LINK_MASK        0x08
#define PRH_HV2_PACKETS_MASK     0x10
#define PRH_HV3_PACKETS_MASK     0x20
#define PRH_U_LAW_MASK           0x40
#define PRH_A_LAW_MASK           0x80
#define PRH_CVSD_MASK            0x01
#define PRH_PAGING_SCHEME_MASK   0x02
#define PRH_POWER_CONTROL_MASK   0x04


#define PRH_3_SLOT_PKT_SUPPORTED(features)                       ( features[0] & PRH_SLOT_3_PACKETS_MASK )
#define PRH_5_SLOT_PKT_SUPPORTED(features)                       ( features[0] & PRH_SLOT_5_PACKETS_MASK )
#define PRH_ENCRYPTION_SUPPORTED(features)                       ( features[0] & PRH_ENCRYPTION_MASK     )
#define PRH_CLOCK_OFFSET_SUPPORTED(features)                     ( features[0] & PRH_SLOT_OFFSET_MASK    )
#define PRH_TIMING_ACCURACY_SUPPORTED(features)                  ( features[0] & PRH_TIMING_ACCURACY_MASK)
#define PRH_SWITCH_ROLE_SUPPORTED(features)                      ( features[0] & PRH_SWITCH_ROLE_MASK    )
#define PRH_HOLD_MODE_SUPPORTED(features)                        ( features[0] & PRH_HOLD_MODE_MASK      )
#define PRH_SNIFF_MODE_SUPPORTED(features)                       ( features[0] & PRH_SNIFF_MODE_MASK     )
#define PRH_PARK_MODE_SUPPORTED(features)                        ( features[1] & PRH_PARK_MODE_MASK      )
#define PRH_RSSI_SUPPORTED(features)                             ( features[1] & PRH_RSSI_MASK           )
#define PRH_CHANNEL_QUALITY_DRIVEN_DATA_RATE_SUPPORTED(features) ( features[1] & PRH_RATE_CHANGE_MASK    )
#define PRH_SCO_LINK_SUPPORTED(features)                         ( features[1] & PRH_SCO_LINK_MASK       )
#define PRH_HV2_PKTS_SUPPORTED(features)                         ( features[1] & PRH_HV2_PACKETS_MASK    )
#define PRH_HV3_PKTS_SUPPORTED(features)                         ( features[1] & PRH_HV3_PACKETS_MASK    )
#define PRH_U_LAW_SUPPORTED(features)                            ( features[1] & PRH_U_LAW_MASK          )
#define PRH_A_LAW_SUPPORTED(features)                            ( features[1] & PRH_A_LAW_MASK          )
#define PRH_CVSD_SUPPORTED(features)                             ( features[2] & PRH_CVSD_MASK           )
#define PRH_PAGING_SCHEME_SUPPORTED(features)                    ( features[2] & PRH_PAGING_SCHEME_MASK  )
#define PRH_POWER_CONTROL_SUPPORTED(features)                    ( features[2] & PRH_POWER_CONTROL_MASK  )




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





#define PRH_LIMITEDDISCOVERYMODE_SET(classOfDevice)  ((classOfDevice) &  PRH_LIMITEDDISCOVERYMODE_BIT)
#define PRH_IS_NETWORKINGSERVICE(classOfDevice) ((classOfDevice) & PRH_CODSERVICEBIT_NETWORKING)
#define PRH_IS_RENDERINGSERVICE(classOfDevice) ((classOfDevice) & PRH_CODSERVICEBIT_RENDERING)
#define PRH_IS_CAPTURINGSERVICE(classOfDevice) ((classOfDevice) & PRH_CODSERVICEBIT_CAPTURING)
#define PRH_IS_OBJECTTRANSFERSERVICE(classOfDevice) ((classOfDevice) & PRH_CODSERVICEBIT_OBJECTTRANSFER)
#define PRH_IS_AUDIOSERVICE(classOfDevice) ((classOfDevice) & PRH_CODSERVICEBIT_AUDIO)
#define PRH_IS_TELEPHONYSERVICE(classOfDevice) ((classOfDevice) & PRH_CODSERVICEBIT_TELEPHONY)
#define PRH_IS_INFORMATIONSERVICE(classOfDevice) ((classOfDevice) & PRH_CODSERVICEBIT_INFORMATION)


#define MAJORDEVICECLASS_MASK		0x00001F00 /* bits 12 - 8 */
#define PRH_IS_PHONEMAJORDEVICE(classOfDevice)  ( ((classOfDevice) & MAJORDEVICECLASS_MASK) == PRH_CODMAJORDEVICE_PHONE)

#define MINORDEVICECLASS_MASK		0x000000FC  /* bits 7 - 2 */ 
#define PRH_IS_CORDLESSMINORDEVICE(classOfDevice) ( ((classOfDevice) & MINORDEVICECLASS_MASK) == PRH_CODMINORDEVICE_PHONE_CORDLESS)
#define PRH_IS_WIREDMODEDORGWMINORDEVICE(classofDevice) ( ((classOfDevice) & MINORDEVICECLASS_MASK) == PRH_CODMINORDEVICE_PHONE_WIREDMODEMORGW)


#define PRH_CODVALUEPENDING_BIT      0x1000000

#define PRH_CODVALUEPENDING(classOfDevice) (classOfDevice & PRH_CODVALUEPENDING_BIT)


/* API FUNCTIONS */


APIDECL1 t_api APIDECL2 MGR_PrintBDAddress(t_bdaddr address, char *outBuffer);
APIDECL1 t_api APIDECL2 MGR_ParseHexBDAddress(char *inString, t_bdaddr *outAddress);


/* NON-API FUNCTIONS */
u_int8 *prh_mgr_printBDAddress(t_bdaddr address);
t_bdaddr prh_mgr_readHexBDAddress(char *hexnumber);
t_bdaddr prh_mgr_getLocalBDAddress(void);
u_int8 * prh_mgr_printLargeNumber(u_int8 *number, u_int8 length);
void prh_mgr_copyBDAddress(t_bdaddr *dest, t_bdaddr *src);
t_api prh_mgr_hci2bt_error(u_int8 eventStatus);
