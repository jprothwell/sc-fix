/*
 * MODULE NAME:    hci_debug.c
 * DESCRIPTION:    HCI Debugging-only Functions
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: hci_debug.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *              
 */


/*
    This whole file is required only if pDEBUG is enabled.
    These functions generate HCI command/event logging output.
*/

#include "host_config.h"
#include "platform_config.h"


#if pDEBUG

#include "host_types.h"
#include "host_error.h"
#include "host_buf.h"
#include "hci_primitives.h"
#include "host_error.h"
#include "hci_constants.h"
#include "hci_debug.h"
#include "mgr_utils.h"
#include "mgr_core.h"
#include "papi.h"

#define MAX_DEBUG_EVENTCODE_VALUE 32
const char * const prh_hci_event_code_to_name[]= {
    "<UNKNOWN EVENT CODE>",
    "InquiryComplete",
    "InquiryResult",
    "ConnectionComplete",
    "ConnectionRequest",
    "Disconnection",
    "Authentication",
    "RemoteName",
    "EncryptionChange",
    "ChangeLinkKey",
    "ChangeMasterLinkKey",
    "RemoteFeatures",
    "RemoteVersion",
    "QoSSetup",
    "CommandComplete",
    "CommandStatus",
    "HardwareError",
    "Flush",
    "RoleChange",
    "NumCompletedPackets",
    "ModeChange",
    "ReturnLinkKeys",
    "PINCodeRequest",
    "LinkKeyRequest",
    "LinkKeyNotification",
    "Loopback",
    "DataBuferOverflow",
    "MaxSlotsChange",
    "ReadClockOffset",
    "PacketTypeChanged",
    "QoSViolation",
    "PageScanModeChange",
    "PageScanRepetitionModeChange"
};


#define MAX_DEBUG_STATUS_VALUE 41
const char * const prh_hci_hci_status_to_name[]= {
    "Success",
    "Unknown Command",
    "No Connection",
    "Hardware Failure",
    "Page Timeout",
    "Authentication Failure",
    "Key Missing",
    "Memory Full",
    "Connection Timeout",
    "Max. Num. Connections",
    "Max. Num. SCO Connections",
    "ACL Already Exists",
    "Command Disallowed",
    "Host Rejected Connection- Limited Resources",
    "Host Rejected Connection - Security Reasons",
    "Host Rejected Connection - Personal Device Only",
    "Host Timeout",
    "Unsupported Feature Or Parameter",
    "Invalid Command Parameters",
    "OETC - User Terminated",
    "OETC - Low Resources",
    "OETC - Powering Off",
    "Local Host Terminated Connection",
    "Repeated Attempts",
    "Paring Not Allowed",
    "Unknown LMP PDU",
    "Unsupported Remote Feature",
    "SCO Offset Rejected",
    "SCO Interval Rejected",
    "SCO Air Mode Rejected",
    "Invalid LMP Parameters",
    "Unspecified Error",
    "Unsupported LMP Parameter Value",
    "Role Change Not Allowed",
    "LMP Response Timeout",
    "LMP Error Transaction Collision",
    "LMP PDU Not Allowed",
    "Encryption Mode Unacceptable",
    "Unit Key Used",
    "QoS Not Supported",
    "Instant Passed",
    "Pairing With Unit Key Not Supported",
};


typedef struct {
    u_int16 opcode;
    char *name;
}prh_hci_opcode2name;
const prh_hci_opcode2name prh_hci_opcode2name_array[] = {
    { 0x000, "NOP" },
	{ 0x401, "Inquiry" },
	{ 0x402, "InquiryCancel" },
	{ 0x403, "PeriodicInquiryMode" },
	{ 0x404, "ExitPeriodicInquiryMode" },
	{ 0x405, "CreateConnection" },
	{ 0x406, "Disconnect" },
	{ 0x407, "AddSCOConnection" },
	{ 0x409, "AcceptConnectionRequest" },
	{ 0x40A, "RejectConnectionReqeust" },
	{ 0x40B, "LinkKeyRequestReply" },
	{ 0x40C, "LinkKeyRequestNegativeReply" },
	{ 0x40D, "PINCodeRequestReply" },
	{ 0x40E, "PINCodeRequestNegativeReply" },
	{ 0x40F, "ChangeConnectionPacketType" },
	{ 0x411, "AuthenticationRequested" },
	{ 0x413, "SetConnectionEncryption" },
	{ 0x415, "ChangeConnectionLinkKey" },
	{ 0x417, "MasterLinkKey" },
	{ 0x419, "RemoteNameRequest" },
	{ 0x41B, "ReadRemoteSupportedFeatures" },
	{ 0x41D, "ReadRemoteVersionInformation" },
	{ 0x41F, "ReadClockOffset" },
	{ 0x801, "HoldMode" },
	{ 0x803, "SniffMode" },
	{ 0x804, "ExitSniffMode" },
	{ 0x805, "ParkMode" },
	{ 0x806, "ExitParkMode" },
	{ 0x807, "QoSSetup" },
	{ 0x809, "RoleDiscovery" },
	{ 0x80B, "SwitchRole" },
	{ 0x80C, "ReadLinkPolicySettings" },
	{ 0x80D, "WriteLinkPolicySettings" },
	{ 0xC01, "SetEventMask" },
	{ 0xC03, "Reset" },
	{ 0xC05, "SetEventFilter" },
	{ 0xC08, "Flush" },
	{ 0xC09, "ReadPINType" },
	{ 0xC0A, "WritePINType" },
	{ 0xC0B, "CreateNewUnitKey" },
	{ 0xC0D, "ReadStoredLinkKey" },
	{ 0xC11, "WriteStoredLinkKey" },
	{ 0xC12, "DeleteStoredLinkKey" },
	{ 0xC13, "ChangeLocalName" },
	{ 0xC14, "ReadLocalName" },
	{ 0xC15, "ReadConnectionAcceptTimeout" },
	{ 0xC16, "WriteConnectionAcceptTimeout" },
	{ 0xC17, "ReadPageTimeout" },
	{ 0xC18, "WritePageTimeout" },
	{ 0xC19, "ReadScanEnable" },
	{ 0xC1A, "WriteScanEnable" },
	{ 0xC1B, "ReadPageScanActivity" },
	{ 0xC1C, "WritePageScanActivity" },
	{ 0xC1D, "ReadInquiryScanActivity" },
	{ 0xC1E, "WriteInquiryScanActivity" },
	{ 0xC1F, "ReadAuthenticationEnable" },
	{ 0xC20, "WriteAuthenticationEnable" },
	{ 0xC21, "ReadEncryptionMode" },
	{ 0xC22, "WriteEncryptionMode" },
	{ 0xC23, "ReadClassofDevice" },
	{ 0xC24, "WriteClassofDevice" },
	{ 0xC25, "ReadVoiceSetting" },
	{ 0xC26, "WriteVoiceSetting" },
	{ 0xC27, "ReadAutomaticFlushTimeout" },
	{ 0xC28, "WriteAutomaticFlushTimeout" },
	{ 0xC29, "ReadNumBroadcastRetransmissions" },
	{ 0xC2A, "WriteNumBroadcastRetransmissions" },
	{ 0xC2B, "ReadHoldModeActivity" },
	{ 0xC2C, "WriteHoldModeActivity" },
	{ 0xC2D, "ReadTransmitPowerLevel" },
	{ 0xC2E, "ReadSCOFlowControlEnable" },
	{ 0xC2F, "WriteSCOFlowControlEnable" },
	{ 0xC31, "SetHostControllerToHostFlowControl" },
	{ 0xC33, "HostBufferSize" },
	{ 0xC35, "HostNumberOfCompletedPackets" },
	{ 0xC36, "ReadLinkSupervisionTimeout" },
	{ 0xC37, "WriteLinkSupervisionTimeout" },
	{ 0xC38, "ReadNumberOfSupportedIAC" },
	{ 0xC39, "ReadCurrentIACLAP" },
	{ 0xC3A, "WriteCurrentIACLAP" },
	{ 0xC3B, "ReadPageScanPeriodMode" },
	{ 0xC3C, "WritePageScanPeriodMode" },
	{ 0xC3D, "ReadPageScanMode" },
	{ 0xC3E, "WritePageScanMode" },
	{ 0x1001, "ReadLocalVersionInformation" },
	{ 0x1003, "ReadLocalSupportedFeatures" },
	{ 0x1005, "ReadBufferSize" },
	{ 0x1007, "ReadCountryCode" },
	{ 0x1009, "ReadBDAddress" },
	{ 0x1401, "ReadFailedContactCounter" },
	{ 0x1402, "ResetFailedContactCounter" },
	{ 0x1403, "GetLinkQuality" },
	{ 0x1405, "ReadRSSI" },
	{ 0x1801, "ReadLoopbackMode" },
	{ 0x1802, "WriteLoopbackMode" },
	{ 0x1803, "EnableDUT" },
    { (0x3F<<10)|0x06, "ReadHardwareVersion (TCI)" },
    { (0x3F<<10)|0x1D, "ReadExtendedFeatures (TCI)" },
    { (0x3F<<10)|0x1A, "WriteLocalDeviceAddress (TCI)" },
    { 0x0, NULL}
};

#define MAX_DEBUG_LMPOPCODE_VALUE 57
const char * const prh_hci_lmp_opcode_to_string[]= {
    "<UNKNOWN LMP OPCODE>",
    "LMP_name_req",
    "LMP_name_res",
    "LMP_accepted",
    "LMP_not_accepted",
    "LMP_clkoffset_req",
    "LMP_clkoffset_res",
    "LMP_detach",
    "LMP_in_rand",
    "LMP_comb_key",
    "LMP_unit_key",
    "LMP_au_rand",
    "LMP_sres",
    "LMP_temp_rand",
    "LMP_temp_key",
    "LMP_encryption_mode_req",
    "LMP_encryption_key_size_req",
    "LMP_start_encryption_req",
    "LMP_stop_encryption_req",
    "LMP_switch_req",
    "LMP_hold",
    "LMP_hold_req",
    "LMP_sniff_req",
    "<UNKNOWN LMP OPCODE>",
    "LMP_unsniff_req",
    "LMP_park_req",
    "<UNKNOWN LMP OPCODE>",
    "LMP_set_broadcast_scan_window",
    "LMP_modify_beacon",
    "LMP_unpark_bd_addr_req",
    "LMP_unpark_pm_addr_req",
    "LMP_incr_power_req",
    "LMP_decr_power_req",
    "LMP_max_power",
    "LMP_min_power",
    "LMP_auto_rate",
    "LMP_preferred_rate",
    "LMP_version_req",
    "LMP_version_res",
    "LMP_features_req",
    "LMP_features_res",
    "LMP_quality_of_service",
    "LMP_quality_of_service_req",
    "LMP_sco_link_req",
    "LMP_remove_sco_link",
    "LMP_max_slot",
    "LMP_max_slot_req",
    "LMP_timing_accuracy_req",
    "LMP_timing_accuracy_res",
    "LMP_setup_complete",
    "LMP_use_semi_permanent_key",
    "LMP_host_connection_req",
    "LMP_slot_offset",
    "LMP_page_mode_req",
    "LMP_page_scan_mode_req",
    "LMP_supervision_timeout",
    "LMP_test_activate",
    "LMP_test_control"
};

char *prh_hci_hciOpcode2string(u_int16 opcode) {
    u_int16 i=0;
    while (prh_hci_opcode2name_array[i].name) 
        if (prh_hci_opcode2name_array[i++].opcode==opcode)
            return prh_hci_opcode2name_array[i-1].name;

    //pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: received unknown opcode 0x%04X (ogf=0x%02X, ocf=0x%02X)\n",opcode,opcode>>10, opcode&0x3f));
    return "<UNKNOWN HCI OPCODE>";
}

char *prh_hci_eventCode2string(u_int16 eventCode) {
    if (eventCode>MAX_DEBUG_EVENTCODE_VALUE || eventCode<1) {
        //pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: received unknown event 0x%02X\n",eventCode));
        return "<UNKNOWN EVENT CODE>";
    } else
        return prh_hci_event_code_to_name[eventCode];
}

char *prh_hci_eventStatus2string(u_int16 status) {
    if (status>MAX_DEBUG_STATUS_VALUE) {
        //pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: received unknown status code 0x%02X\n",status));
        return "<UNKNOWN STATUS CODE>";
    } else
        return prh_hci_hci_status_to_name[status];
}

char *prh_hci_lmpOpcode2string(u_int16 opcode) {
    if (opcode>MAX_DEBUG_LMPOPCODE_VALUE)
        return "<UNKNOWN LMP OPCODE>";
    else
        return prh_hci_lmp_opcode_to_string[opcode];
}

char *prh_hci_linkPolicyMode2string(u_int16 mode) {
    static char *modeStringArray[] = {"Active","Hold","Sniff","Park"};
    if (mode>3) {
        //pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: received unknown link policy mode 0x%02X\n",mode));
        return "<UNKNOWN LINK POLICY MODE>";
    } else
        return modeStringArray[mode];
}



/*************************************************************************************************
 * prh_hci_debug_print_large_number
 *
 * Used for printing link keys & pin codes in hex during debug logging.
 * Uses a static buffer for output string, care must be taken when calling multiple times..
 * Output format: "0x00112233AABBCCDDEEFF", input array must be stored LSB first.
 * This function should be updated to use the platform-specific BT_READ_LE_UINTxx macros, 
 * but this gains only speed & size which is not critical in the debug build.
 ************************************************************************************************/
u_int8 *prh_hci_debug_print_large_number(u_int8 *number, u_int8 length) {

#define prh_hci_debug_print_large_number_maxlen  32

    static u_int8 buffer[prh_hci_debug_print_large_number_maxlen*2+3]="0x";
    int i;

    if (length>prh_hci_debug_print_large_number_maxlen)
        return (u_int8 *)"0x<TooBig>";

    for (i=length-1; i>=0; i--) {
        /* upper nibble */
        buffer[length*2-i*2]=(((number[i])&0xF0)>>4)+48;
        buffer[length*2-i*2]+=buffer[length*2-i*2]>57?7:0;
        /* lower nibble */
        buffer[length*2+1-i*2]=((number[i])&0x0F)+48;
        buffer[length*2+1-i*2]+=buffer[length*2+1-i*2]>57?7:0;
    }
    buffer[length*2+2]='\0';
    return buffer;
}




/*************************************************************************************************
 * prh_hci_debug_print_event
 *
 * Prints the type and parameters of received events (uses prh_hci_debug_print_command_complete_event()).
 ************************************************************************************************/
void prh_hci_debug_print_event(struct host_buf *rxbuf) {    
    int i; 
    u_int8 *pdu;
    u_int8 eventType;
    t_bdaddr BDAddress;
    u_int16 aclHandle;
    pdu=rxbuf->data;
    eventType=pdu[0];
    
    
    switch (eventType) {
    
        case HCI_evPROPRIETARY: /* parthus TCI */

            switch (pdu[2]) {
                case 0x00:
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: Parthus TCI command complete event"));
                    break;
                case 0x01:
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: Parthus TCI LC monitors event"));
                    break;
                case 0x02:
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: Parthus TCI MSS clocks event"));
                    break;
                case 0x22:
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: Parthus TCI LMP trace event, opcode=0x%02X (%s: %s)",pdu[4],pdu[4]&0x01?"Master":"Slave",prh_hci_lmpOpcode2string((u_int16)(pdu[4]>>1))));
                    break;
                default:
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: Parthus TCI event, unknown subtype 0x%02X",pdu[2]));
            }
            break;

        case HCI_evINQUIRYRESULT:
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: InquiryResult event, numResults=0x%02X", pdu[2]));
            for (i=0; i<pdu[2]; i++) {
                pMemcpy(&BDAddress.bytes,pdu+3+i*14,6);
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:   inquiry_result[%d]: addr=%s, pageScanMode=0x%02X, pageScanRepetitionMode=0x%02X, pageScanPeriodMode=0x%02X, classOfDevice=0x%04X, clockOffset=0x%04X\n",
                 i, prh_mgr_printBDAddress(BDAddress),
                 pdu[3+i*14+8], pdu[3+i*14+6], pdu[3+i*14+7],
                 pdu[3+i*14+9]|(pdu[3+i*14+10]<<8)|(pdu[3+i*14+11]<<16),
                 pdu[3+i*14+12]|(pdu[3+i*14+13]<<8)));
            }
            break;

        case HCI_evINQUIRYCOMPLETE:
		    if(prh_mgr_HCIversion >= PRH_HCI_VERSION_BLUETOOTHSPEC11) /* Version 1.1 or greater */
			    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: InquiryComplete event, status=0x%02X (%s)\n", pdu[2], prh_hci_eventStatus2string(pdu[2])));
		    else /* Version 1.0B */
			    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: InquiryComplete event, status=0x%02X (%s), numResponses=0x%02X\n", pdu[2], prh_hci_eventStatus2string(pdu[2]), pdu[3]));
            break;

        case HCI_evCONNECTIONCOMPLETE:
            pMemcpy(&BDAddress.bytes, pdu+5,6);          
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: ConnectionComplete event, status=0x%02X (%s), handle=0x%03X, bdaddr=%s, linkType=0x%01X, encryptionMode=0x%01X\n", pdu[2], prh_hci_eventStatus2string(pdu[2]), BT_READ_LE_UINT16(pdu+3), prh_mgr_printBDAddress(BDAddress), pdu[11], pdu[12]));
            break;
        
        case HCI_evCONNECTIONREQUEST:          
            pMemcpy(&BDAddress.bytes, pdu+2,6);
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: ConnectionRequest event, bdaddr=%s, cod=0x%03X, linkType=0x%01X\n", prh_mgr_printBDAddress(BDAddress), BT_READ_LE_UINT24(pdu+8), pdu[11]));
            break;
        
        case HCI_evDISCONNECTIONCOMPLETE:
            aclHandle = BT_READ_LE_UINT16(pdu+3);
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: DisconnectionComplete event, status=0x%02X (%s), handle=0x%03X, reason=0x%03X (%s)\n", pdu[2], prh_hci_eventStatus2string(pdu[2]), aclHandle, pdu[5], prh_hci_eventStatus2string(pdu[5])));
            break;
        
        case HCI_evAUTHENTICATIONCOMPLETE:
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: AuthenticationComplete event, status=0x%02X (%s), handle=0x%03X\n", pdu[2], prh_hci_eventStatus2string(pdu[2]), BT_READ_LE_UINT16(pdu+3) ));
            break;
        
        case HCI_evREMOTENAMEREQUESTCOMPLETE:
            pMemcpy(&BDAddress.bytes, pdu+3,6);
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: RemoteNameComplete event, status=0x%02X (%s), bdaddr=%s, RemoteName=\"%s\"\n", pdu[2], prh_hci_eventStatus2string(pdu[2]), prh_mgr_printBDAddress(BDAddress), pdu+9));
            break;
        
        case HCI_evENCRYPTIONCHANGE:
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: EncryptionChange event, status=0x%02X (%s), handle=0x%03X, encryption enable 0x%01X\n", pdu[2], prh_hci_eventStatus2string(pdu[2]), BT_READ_LE_UINT16(pdu+3), pdu[5]));
            break;
        
        case HCI_evCHANGECONNECTIONLINKKEYCOMPLETE:           
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: ChangeLinkKeyComplete event, status=0x%02X (%s), handle=0x%03X\n", pdu[2], prh_hci_eventStatus2string(pdu[2]), BT_READ_LE_UINT16(pdu+3) ));
            break;
        
        case HCI_evMASTERLINKKEYCOMPLETE:
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: ChangeMasterLinkKeyComplete event, status=0x%02X (%s), handle=0x%03X, key flag 0x%01X\n", pdu[2], prh_hci_eventStatus2string(pdu[2]), BT_READ_LE_UINT16(pdu+3), pdu[5]));
            break;
        
        case HCI_evREADREMOTESUPPORTEDFEATURESCOMPLETE:
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: ReadRemoteFeaturesComplete event, status=0x%02X (%s), handle=0x%03X, LMP features %s\n", pdu[2], prh_hci_eventStatus2string(pdu[2]), BT_READ_LE_UINT16(pdu+3), prh_hci_debug_print_large_number(pdu+5,8)));            
            if (pdu[5]&0x01)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Remote HC features: 3-slot Packets Supported\n"));
            if (pdu[5]&0x02)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Remote HC features: 5-slot Packets Supported\n"));
            if (pdu[5]&0x04)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Remote HC features: Enctyption Supported\n"));
            if (pdu[5]&0x08)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Remote HC features: Slot Offset Supported\n"));
            if (pdu[5]&0x10)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Remote HC features: Timing Accuracy Supported\n"));
            if (pdu[5]&0x20)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Remote HC features: Role Switch Supported\n"));
            if (pdu[5]&0x40)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Remote HC features: Hold Mode Supported\n"));
            if (pdu[5]&0x80)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Remote HC features: Sniff Mode Supported\n"));
            if (pdu[6]&0x01)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Remote HC features: Park Mode Supported\n"));
            if (pdu[6]&0x02)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Remote HC features: RSSI Supported\n"));
            if (pdu[6]&0x04)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Remote HC features: Channel-Driven Quality Supported\n"));
            if (pdu[6]&0x08)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Remote HC features: SCO Supported\n"));
            if (pdu[6]&0x10)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Remote HC features: HV2 Packets Supported\n"));
            if (pdu[6]&0x20)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Remote HC features: HV3 Packets Supported\n"));
            if (pdu[6]&0x40)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Remote HC features: u-Law Supported\n"));
            if (pdu[6]&0x80)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Remote HC features: A-Law Supported\n"));
            if (pdu[7]&0x01)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Remote HC features: CVSD Supported\n"));
            if (pdu[7]&0x02)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Remote HC features: Paging Scheme Supported\n"));
            if (pdu[7]&0x04)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Remote HC features: Power Control Supported\n"));
            if (pdu[7]&0x08)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Remote HC features: Transparent SCO Data Supported\n"));
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Remote HC features: Flow Control Lag=0x%02X\n",pdu[7]&0x70));
            break;
        
        case HCI_evREADREMOTEVERSIONINFORMATIONCOMPLETE:
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: ReadRemoteVersionComplete event, status=0x%02X (%s), handle=0x%03X, LMP version=0x%01X, manufacturer name=0x%X, LMP subversion=0x%X\n", pdu[2], prh_hci_eventStatus2string(pdu[2]), BT_READ_LE_UINT16(pdu+3), pdu[5], BT_READ_LE_UINT16(pdu+6),BT_READ_LE_UINT16(pdu+8) ));        
            break;
        
        case HCI_evQOSSETUPCOMPLETE:
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: QoSSetupComplete event, status=0x%02X (%s), handle=0x%03X, Flags=0x%01X, service type=0x%01X, token rate=0x%X, peak bandwidth=0x%X, latency=0x%X, delay variation=0x%X\n", pdu[2], prh_hci_eventStatus2string(pdu[2]), BT_READ_LE_UINT16(pdu+3), pdu[5], pdu[6], BT_READ_LE_UINT32(pdu+7), BT_READ_LE_UINT32(pdu+11), BT_READ_LE_UINT32(pdu+15), BT_READ_LE_UINT32(pdu+19)));
            break;

        case HCI_evCOMMANDCOMPLETE:     
            if (pdu[5] == 0) {  /* status */
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: CommandComplete event for %s, numCommandPackets=0x%01X, status=0x%02X (%s)\n",prh_hci_hciOpcode2string(BT_READ_LE_UINT16(pdu+3)),pdu[2],pdu[5],prh_hci_eventStatus2string(pdu[5])));
            } else {
               pDebugPrintfEX((pLOGWARNING,pLOGHCI,"HCI STATUS ERROR: Received CommandComplete event for %s, numCommandPackets=0x%01X, status=0x%02X (%s)\n",prh_hci_hciOpcode2string(BT_READ_LE_UINT16(pdu+3)), pdu[2],pdu[5],prh_hci_eventStatus2string(pdu[5])));
            }
            prh_hci_debug_print_command_complete_event(pdu);
            break;      
        
        case HCI_evCOMMANDSTATUS:
            if (pdu[2] == 0) {  /* status */
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: CommandStatus event for %s, numCommandPackets=0x%01X, status=0x%02X (%s)\n",prh_hci_hciOpcode2string(BT_READ_LE_UINT16(pdu+4)),pdu[3],pdu[2],prh_hci_eventStatus2string(pdu[2])));
            } else {
                pDebugPrintfEX((pLOGWARNING,pLOGHCI,"HCI STATUS ERROR: Received CommandStatus event for %s, numCommandPackets=0x%01X, status=0x%02X (%s)\n",prh_hci_hciOpcode2string(BT_READ_LE_UINT16(pdu+4)),pdu[3],pdu[2],prh_hci_eventStatus2string(pdu[2])));
            }
            break;      
        
        case HCI_evHARDWAREERROR:
            pDebugPrintfEX((pLOGFATAL,pLOGHCI,"HCI Rx: HardwareError event, code=0x%01X\n", pdu[2],prh_hci_eventStatus2string(pdu[2])));
            break;
        
        case HCI_evFLUSHOCCURRED:
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: FlushOccurred event, handle=0x%03X\n",BT_READ_LE_UINT16(pdu+2)));
            break;
        
        case HCI_evROLECHANGE:
            pMemcpy(&BDAddress.bytes, pdu+3,6);
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: RoleChange event, status=0x%02X (%s), bdaddr=%s, newRole=0x%01X\n", pdu[2], prh_hci_eventStatus2string(pdu[2]), prh_mgr_printBDAddress(BDAddress), pdu[9]));
            break;
        
        case HCI_evNUMCOMPLETEDPACKETS:
            //pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: NumCompletedPackets event, numHandles=0x%01X\n",pdu[2]));
            for (i=0; i<pdu[2]; i++) {
                pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"HCI: NumCompletedPackets event, handle[%d]: handle=0x%03X, numCompletedPackets=0x%04X\n", i, BT_READ_LE_UINT16(&pdu[3+(i<<2)]), BT_READ_LE_UINT16(&pdu[5+(i<<2)]) ));      
            }
            break;
        
        case HCI_evMODECHANGE:
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: ModeChange event, status=0x%02X (%s), handle=0x%03X, current mode=0x%01X (%s), interval=0x%X\n", pdu[2], prh_hci_eventStatus2string(pdu[2]),BT_READ_LE_UINT16(pdu+3),pdu[5],prh_hci_linkPolicyMode2string(pdu[5]), BT_READ_LE_UINT16(pdu+6) ));                 
            break;
        
        case HCI_evRETURNLINKKEYS:
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: ReturnLinkKeys event, NumKeys=0x%01X\n",pdu[2]));
            for (i=0; i<pdu[2]; i++) {              
                pMemcpy(&BDAddress.bytes,pdu+3+(i*22),6);
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: ReturnLinkKeys event, key[%d]: bdaddr=%s, link key=%s\n", i, prh_mgr_printBDAddress(BDAddress), prh_hci_debug_print_large_number(pdu+9+(i*22),16)));                 
            }
            break;
        
        case HCI_evPINCODEREQUEST:
            pMemcpy(&BDAddress.bytes,pdu+2,6);              
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: PINCodeRequest event, bdaddr=%s\n", prh_mgr_printBDAddress(BDAddress)));
            break;
        
        case HCI_evLINKKEYREQUEST:
            pMemcpy(&BDAddress.bytes,pdu+2,6);              
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: LinkKeyRequest event, bdaddr=%s\n", prh_mgr_printBDAddress(BDAddress)));  
            break;
        
        case HCI_evLINKKEYNOTIFICATION:
            pMemcpy(&BDAddress.bytes,pdu+2,6);      
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: LinkKeyNotification event, bdaddr=%s, linkkey=%s\n", prh_mgr_printBDAddress(BDAddress), prh_hci_debug_print_large_number(pdu+8,16)));                                 
            break;
        
        case HCI_evLOOPBACKCOMMAND: 
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: LoopbackCommand event, length=0x%02X, loopback opcode=0x%04X (%s)\n", pdu[1], BT_READ_LE_UINT16(pdu+2),prh_hci_hciOpcode2string(BT_READ_LE_UINT16(pdu+2))));
            break;

        case HCI_evDATABUFFEROVERFLOW: 
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: DataBufferOverflow event, linkType=0x%01X\n", pdu[2] ));
            break;
        
        case HCI_evMAXSLOTSCHANGE:
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: MaxSlotsChange event, handle=0x%03X, LMPMaxSlots=0x%01X\n", BT_READ_LE_UINT16(pdu+2), pdu[4]));
            break;
        
        case HCI_evREADCLOCKOFFSETCOMPLETE:
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: ReadClockOffsetComplete event, status=0x%02X (%s), handle=0x%03X, clockOffset=0x%04X\n",pdu[2], prh_hci_eventStatus2string(pdu[2]), BT_READ_LE_UINT16(pdu+3), BT_READ_LE_UINT16(pdu+5) ));
            break;
        
        case HCI_evCONNECTIONPACKETTYPECHANGED:
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: ConnectionPacketTypeChanged event, status=0x%02X (%s), handle=0x%03X, packetType=0x%04X\n", pdu[2], prh_hci_eventStatus2string(pdu[2]), BT_READ_LE_UINT16(pdu+3), BT_READ_LE_UINT16(pdu+5) ));
            break;
        
        case HCI_evQOSVIOLATION:            
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: QoSViolation event, handle=0x%03X\n", BT_READ_LE_UINT16(pdu+2) ));
            break;
        
        case HCI_evPAGESCANMODECHANGE:          
            pMemcpy(&BDAddress.bytes,pdu+2,6);
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: PageScanModeChange event, pageScanMode=0x%01X, bdaddr=%s\n", pdu[8], prh_mgr_printBDAddress(BDAddress)));
            break;
        
        case HCI_evPAGESCANREPETITIONMODECHANGE:
            pMemcpy(&BDAddress.bytes, pdu+2, 6);
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx: PageScanRepetitionModeChange event, pageScanRepetitionMode=0x%01X\n", pdu[8], prh_mgr_printBDAddress(BDAddress)));
            break;
        
        default:
            pDebugPrintfEX((pLOGWARNING,pLOGHCI,"HCI: received unknown event type 0x%02X\n", eventType));
        
    }
}



/*************************************************************************************************
 * prh_hci_debug_print_command_complete_event
 *
 * Prints the type and parameters of received events (called by prh_hci_debug_print_event()).
 * The status and opcode fields have already been printed.
 * This function should be updated to use the platform-specific BT_READ_LE_UINTxx macros, 
 * but this gains only speed & size which is not critical in the debug build.
 ************************************************************************************************/
 void prh_hci_debug_print_command_complete_event(u_int8 *pdu) {
     int i;
     u_int16 cmdOpcode = BT_READ_LE_UINT16(pdu+3);
     t_bdaddr BDAddress;

     switch (cmdOpcode) {

         case HCI_cmdNOP:
             /* no parameters */
             break;
         
        case HCI_cmdREADBUFFERSIZE:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: maxACLLength=0x%02X, maxSCOLength=0x%01X, maxTotalACLs=0x%02X, maxTotalSCOs=0x%02X\n", BT_READ_LE_UINT16(pdu+6), pdu[8], BT_READ_LE_UINT16(pdu+9), BT_READ_LE_UINT16(pdu+11) ));
             break;
         
         case HCI_cmdWRITEAUTOMATICFLUSHTIMEOUT:                        
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: handle=0x%03X\n", BT_READ_LE_UINT16(pdu+6)));    
             break;

         case HCI_cmdWRITELINKSUPERVISIONTIMEOUT:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: handle=0x%03X\n", BT_READ_LE_UINT16(pdu+6)));    
             break;
         
         case HCI_cmdINQUIRYCANCEL:
             /* no parameters */
             break;
         
         case HCI_cmdPERIODICINQUIRYMODE:
             /* no parameters */
             break;
         
         case HCI_cmdEXITPERIODICINQUIRYMODE:
             /* no parameters */
             break;
         
         case HCI_cmdLINKKEYREQUESTREPLY:
             pMemcpy(BDAddress.bytes,pdu+6,6);
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: bdaddr=%s\n",
                 prh_mgr_printBDAddress(BDAddress)));                       
             break;
         
         case HCI_cmdLINKKEYREQUESTNEGATIVEREPLY:
             pMemcpy(BDAddress.bytes,pdu+6,6);
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: bdaddr=%s\n",
                 prh_mgr_printBDAddress(BDAddress)));                       
             break;
         
         case HCI_cmdPINCODEREQUESTREPLY:
             pMemcpy(BDAddress.bytes,pdu+6,6);
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: bdaddr=%s\n",
                 prh_mgr_printBDAddress(BDAddress)));                       
             break;
         
         case HCI_cmdPINCODEREQUESTNEGATIVEREPLY:
             pMemcpy(BDAddress.bytes,pdu+6,6);
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: bdaddr=%s\n",
                 prh_mgr_printBDAddress(BDAddress)));                       
             break;
         
         case HCI_cmdROLEDISCOVERY:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: handle=0x%03X, role=0x%01X\n",
                 pdu[6]|(pdu[7]<<8), 
                 pdu[8]));          
             break;
         
         case HCI_cmdREADLINKPOLICYSETTINGS:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: handle=0x%03X, policy=0x%X\n",
                 pdu[6]|(pdu[7]<<8), 
                 pdu[8]|(pdu[9]<<8)));                                  
             break;
         
         case HCI_cmdWRITELINKPOLICYSETTINGS:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: handle=0x%03X\n",
                 pdu[6]|(pdu[7]<<8)));                      
             break;
         
         case HCI_cmdSETEVENTMASK:
             /* no parameters */
             break;
         
         case HCI_cmdRESET:
             /* no parameters */
             break;
         
         case HCI_cmdSETEVENTFILTER:
             /* no parameters */
             break;
         
         case HCI_cmdFLUSH:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: handle=0x%03X\n",
                 pdu[6]|(pdu[7]<<8)));                      
             break;
         
         case HCI_cmdREADPINTYPE:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: pin type=0x%01X\n",                      
                 pdu[6]));          
             break;
         
         case HCI_cmdWRITEPINTYPE:
             /* no parameters */
             break;
         
         case HCI_cmdCREATENEWUNITKEY:
             /* no parameters */
             break;
         
         case HCI_cmdREADSTOREDLINKKEY:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: max=0x%X, number=0x%X\n",
                 pdu[6]|(pdu[7]<<8),
                 pdu[8]|(pdu[9]<<8)));                                  
             break;
         
         case HCI_cmdWRITESTOREDLINKKEY:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: number written=0x%01X\n",                        
                 pdu[6]));                                          
             break;
         
         case HCI_cmdDELETESTOREDLINKKEY:           
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: number=0x%X\n",                      
                 pdu[6]|(pdu[7]<<8)));                                              
             break;
         
         case HCI_cmdCHANGELOCALNAME:
             /* no parameters */
             break;
         
         case HCI_cmdREADLOCALNAME:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: Name=\"%.*s\"\n",    
                 pdu+5, pdu+6));                        
             break;
         
         case HCI_cmdREADCONNECTIONACCEPTTIMEOUT:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: connection accept timeout=0x%X\n",
                 pdu[6]|(pdu[7]<<8)));      
             break;
         
         case HCI_cmdWRITECONNECTIONACCEPTTIMEOUT:
             /* no parameters */
             break;
         
         case HCI_cmdREADPAGETIMEOUT:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: page timeout=0x%X\n",
                 pdu[6]|(pdu[7]<<8)));                  
             break;
         
         case HCI_cmdWRITEPAGETIMEOUT:
             /* no parameters */
             break;
         
         case HCI_cmdREADSCANENABLE:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: scan enable=0x%01X\n",                       
                 pdu[6]));                                                  
             break;
         
         case HCI_cmdWRITESCANENABLE:
             /* no parameters */
             break;
         
         case HCI_cmdREADPAGESCANACTIVITY:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: page scan interval=0x%X, page scan window=0x%X\n",
                 pdu[6]|(pdu[7]<<8),
                 pdu[8]|(pdu[9]<<8)));                                              
             break;
         
         case HCI_cmdWRITEPAGESCANACTIVITY:
             /* no parameters */     
             break;
         
         case HCI_cmdREADINQUIRYSCANACTIVITY:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: inquiry scan activity interval=0x%X, inquiry scan activity window=0x%X\n",
                 pdu[6]|(pdu[7]<<8),
                 pdu[8]|(pdu[9]<<8)));                                                  
             break;
         
         case HCI_cmdWRITEINQUIRYSCANACTIVITY:
             /* no parameters */     
             break;
         
         case HCI_cmdREADAUTHENTICATIONENABLE:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: authentication enable=0x%01X\n",                     
                 pdu[6]));                                                          
             break;
         
         case HCI_cmdWRITEAUTHENTICATIONENABLE:
             /* no parameters */     
             break;
         
         case HCI_cmdREADENCRYPTIONMODE:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: encryption mode=0x%01X\n",                       
                 pdu[6]));                                                          
             break;
         
         case HCI_cmdWRITEENCRYPTIONMODE:
             /* no parameters */     
             break;
         
         case HCI_cmdREADCLASSOFDEVICE:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: class=0x%03X\n",                     
                 pdu[6]|pdu[7]<<8|pdu[8]<<16));         
             break;
         
         case HCI_cmdWRITECLASSOFDEVICE:
             /* no parameters */                 
             break;
         
         case HCI_cmdREADVOICESETTING:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: VoiceSettings=0x%X\n",
                 pdu[6]|(pdu[7]<<8)));                                                  
             break;
         
         case HCI_cmdWRITEVOICESETTING:
             /* no parameters */                             
             break;
         
         case HCI_cmdREADAUTOMATICFLUSHTIMEOUT:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: handle=0x%03X, flush timeout=0x%X\n",
                 pdu[6]|(pdu[7]<<8),
                 pdu[8]|(pdu[9]<<8)));      
             break;
         
         
         case HCI_cmdREADNUMBROADCASTRETRANSMISSIONS:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: number broadcast retransmissions=0x%01X\n",
                 pdu[6]));                  
             break;
         
         case HCI_cmdWRITENUMBROADCASTRETRANSMISSIONS:
             /* no parameters */             
             break;
         
         case HCI_cmdREADHOLDMODEACTIVITY:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: hold mode activity=0x%01X\n",
                 pdu[6]));                                      
             break;
         
         case HCI_cmdWRITEHOLDMODEACTIVITY:
             /* no parameters */             
             break;
         
         case HCI_cmdREADTRANSMITPOWERLEVEL:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: handle=0x%03X, transmit power level=0x%01X\n",
                 pdu[6]|(pdu[7]<<8),
                 pdu[8]));                      
             break;
         
         case HCI_cmdREADSCOFLOWCONTROLENABLE:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: SCO flow control enable=0x%01X\n",
                 pdu[6]));                                                  
             break;
         
         case HCI_cmdWRITESCOFLOWCONTROLENABLE:
             /* no parameters */                         
             break;
         
         case HCI_cmdSETHOSTCONTROLLERTOHOSTFLOWCONTROL:
             /* no parameters */             
             break;
         
         case HCI_cmdHOSTBUFFERSIZE:
             /* no parameters */             
             break;
         
         case HCI_cmdHOSTNUMBEROFCOMPLETEDPACKETS:
             /* no parameters */ 
             break;
         
         case HCI_cmdREADLINKSUPERVISIONTIMEOUT:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: handle=0x%03X, link supervision timeout=0x%X\n",
                 pdu[6]|(pdu[7]<<8),
                 pdu[8]|(pdu[9]<<8)));                          
             break;
         
         case HCI_cmdREADNUMBEROFSUPPORTEDIAC:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: number supported IAC=0x%01X\n",
                 pdu[6]));                                                              
             break;
         
         case HCI_cmdREADCURRENTIACLAP: 
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: ReadCurrentIACLAP: numIACs=0x%01X\n", pdu[6]));                                                              
             for (i=0; i<pdu[6]; i++) { 
                 pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: ReadCurrentIACLAP[%d]: IAC LAP=0x%6X\n",i,BT_READ_LE_UINT24(pdu+6+(i*3))));                                                        
             }                                                      
             break;
         
         case HCI_cmdWRITECURRENTIACLAP:
             /* no parameters */                                 
             break;
         
         case HCI_cmdREADPAGESCANPERIODMODE:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: page scan period mode=0x%01X\n",
                 pdu[6]));                                                                          
             break;
         
         case HCI_cmdWRITEPAGESCANPERIODMODE:
             /* no parameters */                                 
             break;
         
         case HCI_cmdREADPAGESCANMODE:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: page scan mode=0x%01X\n",
                 pdu[6]));                                                                          
             break;
         
         case HCI_cmdWRITEPAGESCANMODE:
             /* no parameters */                                 
             break;
         
         case HCI_cmdREADLOCALVERSIONINFORMATION:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: HCIversion=0x%01X, HCIrevision=0x%X, LMP version=0x%01X, ManufacturerId=0x%X, LMP subversion=0x%X\n",
                 pdu[6],
                 pdu[7]|(pdu[8]<<8),
                 pdu[9],pdu[10]|(pdu[11]<<8),
                 pdu[12]|(pdu[13]<<8)));                                                                            
             break;
         
         case HCI_cmdREADLOCALSUPPORTEDFEATURES:
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: LMP features %s\n",              
                prh_hci_debug_print_large_number(pdu+6,8)));
            if (pdu[6]&0x01)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Local HC features: 3-slot Packets Supported\n"));
            if (pdu[6]&0x02)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Local HC features: 5-slot Packets Supported\n"));
            if (pdu[6]&0x04)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Local HC features: Enctyption Supported\n"));
            if (pdu[6]&0x08)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Local HC features: Slot Offset Supported\n"));
            if (pdu[6]&0x10)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Local HC features: Timing Accuracy Supported\n"));
            if (pdu[6]&0x20)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Local HC features: Role Switch Supported\n"));
            if (pdu[6]&0x40)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Local HC features: Hold Mode Supported\n"));
            if (pdu[6]&0x80)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Local HC features: Sniff Mode Supported\n"));
            if (pdu[7]&0x01)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Local HC features: Park Mode Supported\n"));
            if (pdu[7]&0x02)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Local HC features: RSSI Supported\n"));
            if (pdu[7]&0x04)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Local HC features: Channel-Driven Quality Supported\n"));
            if (pdu[7]&0x08)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Local HC features: SCO Supported\n"));
            if (pdu[7]&0x10)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Local HC features: HV2 Packets Supported\n"));
            if (pdu[7]&0x20)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Local HC features: HV3 Packets Supported\n"));
            if (pdu[7]&0x40)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Local HC features: u-Law Supported\n"));
            if (pdu[7]&0x80)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Local HC features: A-Law Supported\n"));
            if (pdu[8]&0x01)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Local HC features: CVSD Supported\n"));
            if (pdu[8]&0x02)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Local HC features: Paging Scheme Supported\n"));
            if (pdu[8]&0x04)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Local HC features: Power Control Supported\n"));
            if (pdu[8]&0x08)
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Local HC features: Transparent SCO Data Supported\n"));
            pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Local HC features: Flow Control Lag=0x%02X\n",pdu[7]&0x70));
            break; 
         
         
         case HCI_cmdREADCOUNTRYCODE:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: country code=0x%01X\n",
                 pdu[6]));          
             break;
         
         case HCI_cmdREADBDADDR:
             pMemcpy(BDAddress.bytes,pdu+6,6);
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: bdaddr=%s\n",
                 prh_mgr_printBDAddress(BDAddress)));                                       
             break;
         
         case HCI_cmdREADFAILEDCONTACTCOUNTER:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: handle=0x%03X, counter=0x%X\n",
                 pdu[6]|(pdu[7]<<8),
                 pdu[8]|(pdu[9]<<8)));                                      
             break;
         
         case HCI_cmdRESETFAILEDCONTACTCOUNTER:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: handle=0x%03X\n",
                 pdu[6]|(pdu[7]<<8)));          
             break;
         
         case HCI_cmdGETLINKQUALITY:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: handle=0x%03X, quality=0x%0X\n",
                 pdu[6]|(pdu[7]<<8),pdu[8]));           
             break;
         
         case HCI_cmdREADRSSI:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: handle=0x%03X, RSSI=0x%0X\n",
                 pdu[6]|(pdu[7]<<8),pdu[8]));                       
             break;
         
         case HCI_cmdREADLOOPBACKMODE:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI Rx:     CommandComplete Parameters: loopback mode =0x%01X\n",
                 pdu[6]));                      
             break;
         
         case HCI_cmdWRITELOOPBACKMODE:
             /* no parameters */                     
             break;
         
         case HCI_cmdENABLEDUT:
             /* no parameters */                         
             break;

        case ((0x3F<<10)|0x06): /* proprietary Read_Hardware_Version command */
            if (!pdu[5])
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Proprietary HC version (via TCI): major=0x%02X, minor=0x%02X\n",pdu[6],pdu[7]));
            break;

        case ((0x3F<<10)|0x1D): /* proprietary Read_Extended_Features command */
            if (!pdu[5]) {
                pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI: Proprietary HC extended features (via TCI): features=%s\n",prh_mgr_printLargeNumber(pdu+6,8)));
                if (pdu[6]&0x01)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Filters Supported\n"));
                if (pdu[6]&0x02)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Host Controller To Host Flow Control Supported\n"));
                if (pdu[6]&0x04)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Link Key Database Supported\n"));
                if (pdu[6]&0x08)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Remote Name Request Supported\n"));
                if (pdu[6]&0x10)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Loopback via HCI Supported\n"));
                if (pdu[6]&0x20)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Flush Supported\n"));
                if (pdu[6]&0x40)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Piconet Operation Supported\n"));
                if (pdu[6]&0x80)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Scatternet Operation Supported\n"));
                if (pdu[7]&0x01)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Modifiable Timers Supported\n"));
                if (pdu[7]&0x02)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Test Mode Supported\n"));
                if (pdu[7]&0x04)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Change Local Name Supported\n"));
                if (pdu[7]&0x08)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Modifiable Local Features Supported\n"));
                if (pdu[7]&0x10)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Test Mode Tester Supported\n"));
                if (pdu[7]&0x20)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Negotiated Hold Supported\n"));
                if (pdu[7]&0x40)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Data Pump Supported\n"));
                if (pdu[7]&0x80)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: 79 Hop Only Supported\n"));
                if (pdu[8]&0x01)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Write Scan Activity Supported\n"));
                if (pdu[8]&0x02)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Paging Supported\n"));
                if (pdu[8]&0x04)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Page Scan Supported\n"));
                if (pdu[8]&0x08)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Inquiry Supported\n"));
                if (pdu[8]&0x10)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Inquiry Scan Supported\n"));
                if (pdu[8]&0x20)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Read/Write IAC Supported\n"));
                if (pdu[8]&0x40)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Unique Inquiry Results Supported\n"));
                if (pdu[8]&0x80)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Periodic Inquiry Supported\n"));
                if (pdu[9]&0x01)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Quality Of Service Supported\n"));
                if (pdu[9]&0x02)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Forced Quality Of Service Supported\n"));
                if (pdu[9]&0x04)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Change ACL Packet Type Supported\n"));
                if (pdu[9]&0x08)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Change SCO Packet Type Supported\n"));
                if (pdu[9]&0x10)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Auto-Rate Supported\n"));
                if (pdu[9]&0x20)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Preferred-Rate Supported\n"));
                if (pdu[9]&0x40)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: SCO via HCI Supported\n"));
                if (pdu[9]&0x80)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Force Max Slots Supported\n"));
                if (pdu[10]&0x01)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: LMP Tracing via HCI Supported\n"));
                if (pdu[10]&0x02)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: LC Tracing via HCI Supported\n"));
                if (pdu[10]&0x04)
                    pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI:     Proprietary HC extended features: Baseband Tracing via HCI Supported\n"));
            }
            break;
	 case ((0x3F<<10)|0x1A):
             break;                                    
         default:
             pDebugPrintfEX((pLOGERROR,pLOGHCI,"HCI_ExtractCommandCompleteParams: received unexpected command opcode 0x%02X\n", cmdOpcode)); 
    }
}

#endif /* pDEBUG */
