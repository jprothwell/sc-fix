/***********************************************************************
 *
 * MODULE NAME:    hci.h
 * DESCRIPTION:    Bluetooth Host Stack HCI API header
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: hci.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 * REMARKS:
 *     This file should be incuded via bt.h only.
 *
 ***********************************************************************/


#ifndef BLUETOOTH_HCI_API_DECLARED
#define BLUETOOTH_HCI_API_DECLARED


/* No operation command, may be used for flow control */
#define HCI_cmdNOP									0x0000


/*
	HCI link control commands - Opcode Group Field = 0x01
*/


#define HCI_cmdINQUIRY								((0x1<<10) | 0x01)
#define HCI_cmdINQUIRYCANCEL						((0x1<<10) | 0x02)
#define HCI_cmdPERIODICINQUIRYMODE					((0x1<<10) | 0x03)
#define HCI_cmdEXITPERIODICINQUIRYMODE				((0x1<<10) | 0x04)
#define HCI_cmdCREATECONNECTION						((0x1<<10) | 0x05)
#define HCI_cmdDISCONNECT							((0x1<<10) | 0x06)
#define HCI_cmdADDSCOCONNECTION						((0x1<<10) | 0x07)
#define HCI_cmdACCEPTCONNECTIONREQUEST				((0x1<<10) | 0x09)
#define HCI_cmdREJECTCONNECTIONREQEUST				((0x1<<10) | 0x0A)
#define HCI_cmdLINKKEYREQUESTREPLY					((0x1<<10) | 0x0B)
#define HCI_cmdLINKKEYREQUESTNEGATIVEREPLY			((0x1<<10) | 0x0C)
#define HCI_cmdPINCODEREQUESTREPLY					((0x1<<10) | 0x0D)
#define HCI_cmdPINCODEREQUESTNEGATIVEREPLY			((0x1<<10) | 0x0E)
#define HCI_cmdCHANGECONNECTIONPACKETTYPE			((0x1<<10) | 0x0F)
#define HCI_cmdAUTHENTICATIONREQUESTED				((0x1<<10) | 0x11)
#define HCI_cmdSETCONNECTIONENCRYPTION				((0x1<<10) | 0x13)
#define HCI_cmdCHANGECONNECTIONLINKKEY				((0x1<<10) | 0x15)
#define HCI_cmdMASTERLINKKEY						((0x1<<10) | 0x17)
#define HCI_cmdREMOTENAMEREQUEST					((0x1<<10) | 0x19)
#define HCI_cmdREMOTENAMEREQUESTCANCEL			((0x1<<10 | 0x1A))
#define HCI_cmdREADREMOTESUPPORTEDFEATURES			((0x1<<10) | 0x1B)
#define HCI_cmdREADREMOTEVERSIONINFORMATION			((0x1<<10) | 0x1D)
#define HCI_cmdREADCLOCKOFFSET						((0x1<<10) | 0x1F)


/*
	HCI link policy commands - Opcode Group Field = 0x02
*/

#define HCI_cmdHOLDMODE								((0x2<<10) | 0x1)
#define HCI_cmdSNIFFMODE							((0x2<<10) | 0x3)
#define HCI_cmdEXITSNIFFMODE						((0x2<<10) | 0x4)
#define HCI_cmdPARKMODE								((0x2<<10) | 0x5)
#define HCI_cmdEXITPARKMODE							((0x2<<10) | 0x6)
#define HCI_cmdQOSSETUP								((0x2<<10) | 0x7)
#define HCI_cmdROLEDISCOVERY						((0x2<<10) | 0x9)
#define HCI_cmdSWITCHROLE							((0x2<<10) | 0xB)
#define HCI_cmdREADLINKPOLICYSETTINGS				((0x2<<10) | 0xC)
#define HCI_cmdWRITELINKPOLICYSETTINGS				((0x2<<10) | 0xD)


/*
	HCI host controller and baseband commands - Opcode Group Field = 0x03
*/

#define HCI_cmdSETEVENTMASK							((0x3<<10) | 0x01)
#define HCI_cmdRESET								((0x3<<10) | 0x03)
#define HCI_cmdSETEVENTFILTER						((0x3<<10) | 0x05)
#define HCI_cmdFLUSH								((0x3<<10) | 0x08)
#define HCI_cmdREADPINTYPE							((0x3<<10) | 0x09)
#define HCI_cmdWRITEPINTYPE							((0x3<<10) | 0x0A)
#define HCI_cmdCREATENEWUNITKEY						((0x3<<10) | 0x0B)
#define HCI_cmdREADSTOREDLINKKEY					((0x3<<10) | 0x0D)
#define HCI_cmdWRITESTOREDLINKKEY					((0x3<<10) | 0x11)
#define HCI_cmdDELETESTOREDLINKKEY					((0x3<<10) | 0x12)
#define HCI_cmdCHANGELOCALNAME						((0x3<<10) | 0x13)
#define HCI_cmdREADLOCALNAME						((0x3<<10) | 0x14)
#define HCI_cmdREADCONNECTIONACCEPTTIMEOUT			((0x3<<10) | 0x15)
#define HCI_cmdWRITECONNECTIONACCEPTTIMEOUT			((0x3<<10) | 0x16)
#define HCI_cmdREADPAGETIMEOUT						((0x3<<10) | 0x17)
#define HCI_cmdWRITEPAGETIMEOUT						((0x3<<10) | 0x18)
#define HCI_cmdREADSCANENABLE						((0x3<<10) | 0x19)
#define HCI_cmdWRITESCANENABLE						((0x3<<10) | 0x1A)
#define HCI_cmdREADPAGESCANACTIVITY					((0x3<<10) | 0x1B)
#define HCI_cmdWRITEPAGESCANACTIVITY				((0x3<<10) | 0x1C)
#define HCI_cmdREADINQUIRYSCANACTIVITY				((0x3<<10) | 0x1D)
#define HCI_cmdWRITEINQUIRYSCANACTIVITY				((0x3<<10) | 0x1E)
#define HCI_cmdREADAUTHENTICATIONENABLE				((0x3<<10) | 0x1F)
#define HCI_cmdWRITEAUTHENTICATIONENABLE			((0x3<<10) | 0x20)
#define HCI_cmdREADENCRYPTIONMODE					((0x3<<10) | 0x21)
#define HCI_cmdWRITEENCRYPTIONMODE					((0x3<<10) | 0x22)
#define HCI_cmdREADCLASSOFDEVICE					((0x3<<10) | 0x23)
#define HCI_cmdWRITECLASSOFDEVICE					((0x3<<10) | 0x24)
#define HCI_cmdREADVOICESETTING						((0x3<<10) | 0x25)
#define HCI_cmdWRITEVOICESETTING					((0x3<<10) | 0x26)
#define HCI_cmdREADAUTOMATICFLUSHTIMEOUT			((0x3<<10) | 0x27)
#define HCI_cmdWRITEAUTOMATICFLUSHTIMEOUT			((0x3<<10) | 0x28)
#define HCI_cmdREADNUMBROADCASTRETRANSMISSIONS		((0x3<<10) | 0x29)
#define HCI_cmdWRITENUMBROADCASTRETRANSMISSIONS		((0x3<<10) | 0x2A)
#define HCI_cmdREADHOLDMODEACTIVITY					((0x3<<10) | 0x2B)
#define HCI_cmdWRITEHOLDMODEACTIVITY				((0x3<<10) | 0x2C)
#define HCI_cmdREADTRANSMITPOWERLEVEL				((0x3<<10) | 0x2D)
#define HCI_cmdREADSCOFLOWCONTROLENABLE				((0x3<<10) | 0x2E)
#define HCI_cmdWRITESCOFLOWCONTROLENABLE			((0x3<<10) | 0x2F)
#define HCI_cmdSETHOSTCONTROLLERTOHOSTFLOWCONTROL	((0x3<<10) | 0x31)
#define HCI_cmdHOSTBUFFERSIZE						((0x3<<10) | 0x33)
#define HCI_cmdHOSTNUMBEROFCOMPLETEDPACKETS			((0x3<<10) | 0x35)
#define HCI_cmdREADLINKSUPERVISIONTIMEOUT			((0x3<<10) | 0x36)
#define HCI_cmdWRITELINKSUPERVISIONTIMEOUT			((0x3<<10) | 0x37)
#define HCI_cmdREADNUMBEROFSUPPORTEDIAC				((0x3<<10) | 0x38)
#define HCI_cmdREADCURRENTIACLAP					((0x3<<10) | 0x39)
#define HCI_cmdWRITECURRENTIACLAP					((0x3<<10) | 0x3A)
#define HCI_cmdREADPAGESCANPERIODMODE				((0x3<<10) | 0x3B)
#define HCI_cmdWRITEPAGESCANPERIODMODE				((0x3<<10) | 0x3C)
#define HCI_cmdREADPAGESCANMODE						((0x3<<10) | 0x3D)
#define HCI_cmdWRITEPAGESCANMODE					((0x3<<10) | 0x3E)


/*
	HCI informational commands - Opcode Group Field = 0x04
*/

#define HCI_cmdREADLOCALVERSIONINFORMATION			((0x4<<10) | 0x01)
#define HCI_cmdREADLOCALSUPPORTEDFEATURES			((0x4<<10) | 0x03)
#define HCI_cmdREADBUFFERSIZE						((0x4<<10) | 0x05)
#define HCI_cmdREADCOUNTRYCODE						((0x4<<10) | 0x07)
#define HCI_cmdREADBDADDR							((0x4<<10) | 0x09)


/*
	HCI status commands - Opcode Group Field = 0x05
*/

#define HCI_cmdREADFAILEDCONTACTCOUNTER				((0x5<<10) | 0x01)
#define HCI_cmdRESETFAILEDCONTACTCOUNTER			((0x5<<10) | 0x02)
#define HCI_cmdGETLINKQUALITY						((0x5<<10) | 0x03)
#define HCI_cmdREADRSSI								((0x5<<10) | 0x05)


/*
	HCI test commands - Opcode Group Field = 0x06
*/

#define HCI_cmdREADLOOPBACKMODE						((0x6<<10) | 0x01)
#define HCI_cmdWRITELOOPBACKMODE					((0x6<<10) | 0x02)
#define HCI_cmdENABLEDUT							((0x6<<10) | 0x03)

// add by tianwq: for baseband test
#define HCI_cmdBBTXTEST								((0x6<<10) | 0x04)
#define HCI_cmdBBRXTEST								((0x6<<10) | 0x05)
#define HCI_cmdBBTESTEND							((0x6<<10) | 0x06)


/*
	HCI Event Codes
*/


#define HCI_evINQUIRYCOMPLETE						0x01
#define HCI_evINQUIRYRESULT							0x02
#define HCI_evCONNECTIONCOMPLETE					0x03
#define HCI_evCONNECTIONREQUEST 					0x04
#define HCI_evDISCONNECTIONCOMPLETE					0x05
#define HCI_evAUTHENTICATIONCOMPLETE				0x06
#define HCI_evREMOTENAMEREQUESTCOMPLETE				0x07
#define HCI_evENCRYPTIONCHANGE						0x08
#define HCI_evCHANGECONNECTIONLINKKEYCOMPLETE		0x09
#define HCI_evMASTERLINKKEYCOMPLETE 				0x0A
#define HCI_evREADREMOTESUPPORTEDFEATURESCOMPLETE	0x0B
#define HCI_evREADREMOTEVERSIONINFORMATIONCOMPLETE	0x0C
#define HCI_evQOSSETUPCOMPLETE						0x0D
#define HCI_evCOMMANDCOMPLETE						0x0E
#define HCI_evCOMMANDSTATUS 						0x0F
#define HCI_evHARDWAREERROR							0x10
#define HCI_evFLUSHOCCURED							0x11
#define HCI_evROLECHANGE							0x12
#define HCI_evNUMCOMPLETEDPACKETS					0x13
#define HCI_evMODECHANGE							0x14
#define HCI_evRETURNLINKKEYS						0x15
#define HCI_evPINCODEREQUEST						0x16
#define HCI_evLINKKEYREQUEST						0x17
#define HCI_evLINKKEYNOTIFICATION					0x18
#define HCI_evLOOPBACKCOMMAND						0x19
#define HCI_evDATABUFFEROVERFLOW					0x1A
#define HCI_evMAXSLOTSCHANGE						0x1B
#define HCI_evREADCLOCKOFFSETCOMPLETE				0x1C
#define HCI_evCONNECTIONPACKETTYPECHANGED			0x1D
#define HCI_evQOSVIOLATION							0x1E
#define HCI_evPAGESCANMODECHANGE					0x1F
#define HCI_evPAGESCANREPETITIONMODECHANGE			0x20
#define HCI_evFLOWSPECIFICATIONCOMPLETE             0x21
#define HCI_evINQUIRYRESULTWITHRSSI                 0x22
#define HCI_evREADREMOTEEXTENDEDFEATURESCOMPLETE    0x23
#define HCI_evFIXEDADDRESS                          0x24
#define HCI_evALIASADDRESS                          0x25
#define HCI_evGENERATEALIASREQUEST                  0x26
#define HCI_evACTIVEADDRESS                         0x27
#define HCI_evALLOWPRIVATEPAIRING                   0x28
#define HCI_evALIASADDRESSREQUEST                   0x29
#define HCI_evALIASNOTRECOGNISED                    0x2A
#define HCI_evFIXEDADDRESSATTEMPT                   0x2B
#define HCI_evSYNCHRONOUSCONNECTIONCOMPLETE         0x2C
#define HCI_evSYNCHRONOUSCONNECTIONCHANGED          0x2D
#define HCI_evSNIFFSUBRATING                                       0x2E //Zhu Jianguo added for the new Feature of BT 2.1 2008.2.25
#define HCI_evEXTENDEDINQUIRYRESULT                        0x2F  
#define HCI_evENCRYPTIONKEYPREFRESHCOMPLETE		0x30      
#define HCI_evIOCAPABILITYREQUEST                             0x31    
#define HCI_evIOCAPABILITYRESPONSE                           0x32
#define HCI_evUSERCONFIRMATIONREQUEST                   0x33
#define HCI_evUSERPASSKEYREQUEST                             0x34
#define HCI_evREMOTEOOBDATAREQUEST                        0x35
#define HCI_evSIMPLEPAIRINGCOMPLETE                         0x36
#define HCI_evLINKSUPERVISIONTIMEOUTCHANGED       0x38
#define HCI_evUSERPASSKEYNOTIFICATION                    0x3B
#define HCI_evKEYPRESSNOTFIFICATION                         0x3c    //end of added by Zhu Jianguo. 2008.2.25    
#define HCI_evREMOTEHOSTSUPPORTEDFEATURESNOTIFICATION 0x3D  //Zhu Jianguo added it for the test case. 2008.4.13
#define	HCI_evTESTING								0xFE


/*
	HCI Status/Error Codes
*/


#define HCI_errNOERROR				0x00
#define HCI_errUNKNOWNCOMMAND		0x01
#define HCI_errNOCONNECTION			0x02
#define HCI_errHWFAILURE			0x03
#define HCI_errPAGETIMEOUT			0x04
#define HCI_errAUTHFAILURE			0x05
#define HCI_errKEYMISSING			0x06
#define HCI_errMEMORYFULL			0x07
#define HCI_errCONNTIMEOUT			0x08
#define HCI_errMAXCONNECTIONS		0x09
#define HCI_errMAXSCOS				0x0A
#define HCI_errACLEXISTS			0x0B
#define HCI_errCMDDISALLOWED		0x0C
#define HCI_errREJRESOURCES			0x0D
#define HCI_errREJSECURITY			0x0E
#define HCI_errREJPERSONALDEV		0x0F
#define HCI_errHOSTTIMEOUT			0x10
#define HCI_errUNSUPPORTED			0x11
#define HCI_errINVALIDPARAM			0x12
#define HCI_errTERMUSER				0x13
#define HCI_errTERMRESOURCES		0x14
#define HCI_errTERMPOWEROFF			0x15
#define HCI_errLOCALTERM			0x16
#define HCI_errREPEATED				0x17
#define HCI_errNOPARING				0x18
#define HCI_errUNKNOWNLMPPDU		0x19
#define HCI_errUNSUPPORTEDREMOTE	0x1A
#define HCI_errSCOOFFSETREJ			0x1B
#define HCI_errSCOINTERVALREJ		0x1C
#define HCI_errSCOMODEREJ			0x1D
#define HCI_errLMPINVALIDPARAM		0x1E
#define HCI_errUNSPECERROR			0x1F
#define HCI_errLMPUNSUPPPARAM		0x20
#define HCI_errNOROLECHANGE			0x21
#define HCI_errLMPTIMEOUT			0x22
#define HCI_errLMPTRANSCOLLISION	0x23
#define HCI_errLMPPDUDISALLOWED		0x24



/*
	HCI Function Declarations
*/




	
/*
	HCI event callback registration  
*/
APIDECL1 int APIDECL2 HCI_RegisterEventHandler(void (*callbackFunc)(u_int8 *eventPdu));
APIDECL1 int APIDECL2 HCI_DeregisterEventHandler(void (*callbackFunc)(u_int8 *eventPdu));



/*
	HCI low-level data primitives 
*/


/* PDU types for HCI_SendRawPDU() HCI_RegisterRxHandler() callback function */
#define HCI_pduCOMMAND		0x01
#define HCI_pduACLDATA		0x02
#define HCI_pduSCODATA		0x03
#define HCI_pduEVENT		0x04

/* handle specifiers used by HCI_GetWriteBuffer */
#define HCI_NO_HANDLE                   0xFFF1 
#define HCI_COMMAND_HANDLE              HCI_NO_HANDLE
#define HCI_ACTIVE_BROADCAST_HANDLE	    0xFFF2 
#define HCI_PICONET_BROADCAST_HANDLE    0xFFF3 
#define HCI_SCO_HANDLE 0x8000
#define HCI_ACL_HANDLE 0x0000

APIDECL1 int APIDECL2 HCI_RegisterRxHandler(void (*callbackFunc)(u_int8 type, u_int8 *pdu, u_int32 len)); 
APIDECL1 int APIDECL2 HCI_DeregisterRxHandler(void (*callbackFunc)(u_int8 type, u_int8 *pdu, u_int32 len));

APIDECL1 int APIDECL2 HCI_GetWriteBuffer(u_int16 handle, u_int16 len, t_DataBuf **writeBuffer);
APIDECL1 int APIDECL2 HCI_FreeWriteBuffer(t_DataBuf *writeBuffer);
APIDECL1 int APIDECL2 HCI_SendRawPDU(u_int8 type, t_DataBuf *writeBuffer);



/*
	HCI link control commands - Opcode Group Field = 0x01
*/
APIDECL1 int APIDECL2 HCI_Inquiry(u_int32 lap, u_int8 inquiryLength, u_int8 numResponses);
APIDECL1 int APIDECL2 HCI_InquiryCancel(void);
APIDECL1 int APIDECL2 HCI_PeriodicInquiryMode(u_int16 maxPeriodLen, u_int16 minPeriodLen, u_int32 lap, u_int8 inquiryLen, u_int8 numResponses);
APIDECL1 int APIDECL2 HCI_ExitPeriodicInquiryMode(void);
APIDECL1 int APIDECL2 HCI_CreateConnection(t_bdaddr address, u_int16 packetType, u_int8 pageScanRepet, u_int8 pageScanMode, u_int16 clockOffset, u_int8 role);
APIDECL1 int APIDECL2 HCI_Disconnect(u_int16 handle, u_int8 reason);
APIDECL1 int APIDECL2 HCI_AddSCOConnection(u_int16 handle, u_int16 packetType);
APIDECL1 int APIDECL2 HCI_AcceptConnectionRequest(t_bdaddr address, u_int8 role);
APIDECL1 int APIDECL2 HCI_RejectConnectionRequest(t_bdaddr address, u_int8 reason);
APIDECL1 int APIDECL2 HCI_LinkKeyRequestReply(t_bdaddr address, u_int8 *linkKey);
APIDECL1 int APIDECL2 HCI_LinkKeyRequestNegativeReply(t_bdaddr address);
APIDECL1 int APIDECL2 HCI_PINCodeRequestReply(t_bdaddr address, u_int8 pinCodeLen, u_int8 *pinCode);
APIDECL1 int APIDECL2 HCI_PINCodeRequestNegativeReply(t_bdaddr address);
APIDECL1 int APIDECL2 HCI_ChangeConnectionPacketType(u_int16 handle, u_int16 packetType);
APIDECL1 int APIDECL2 HCI_AuthenticationRequested(u_int16 handle);
APIDECL1 int APIDECL2 HCI_SetConnectionEncryption(u_int16 handle, u_int8 encrypt);
APIDECL1 int APIDECL2 HCI_ChangeConnectionLinkKey(u_int16 handle);
APIDECL1 int APIDECL2 HCI_MasterLinkKey(u_int8 keyFlag);
APIDECL1 int APIDECL2 HCI_RemoteNameRequest(t_bdaddr address, u_int8 pageScanRepet, u_int8 pageScanMode, u_int16 clockOffset);
APIDECL1 int APIDECL2 HCI_ReadRemoteSupportedFeatures(u_int16 handle);
APIDECL1 int APIDECL2 HCI_ReadRemoteVersionInformation(u_int16 handle);
APIDECL1 int APIDECL2 HCI_ReadClockOffset(u_int16 handle);


/*
	HCI link policy commands - Opcode Group Field = 0x02
*/
APIDECL1 int APIDECL2 HCI_HoldMode(u_int16 handle, u_int16 maxInterval, u_int16 minInterval);
APIDECL1 int APIDECL2 HCI_SniffMode(u_int16 handle, u_int16 maxInterval, u_int16 minInterval, u_int16 sniffAttempt, u_int16 sniffTimeout);
APIDECL1 int APIDECL2 HCI_ExitSniffMode(u_int16 handle);
APIDECL1 int APIDECL2 HCI_ParkMode(u_int16 handle, u_int16 beaconMaxInterval, u_int16 beaconMinInterval);
APIDECL1 int APIDECL2 HCI_ExitParkMode(u_int16 handle);
APIDECL1 int APIDECL2 HCI_QoSSetup(u_int16 handle, u_int8 flags, u_int8 serviceType, u_int32 tokenRate, u_int32 peakBandwidth, u_int32 latency, u_int32 delayVar);
APIDECL1 int APIDECL2 HCI_RoleDiscovery(u_int16 handle);
APIDECL1 int APIDECL2 HCI_SwitchRole(t_bdaddr address, u_int8 role);
APIDECL1 int APIDECL2 HCI_ReadLinkPolicySettings(u_int16 handle);
APIDECL1 int APIDECL2 HCI_WriteLinkPolicySettings(u_int16 handle, u_int16 settings);


/*
	HCI host controller and baseband commands - Opcode Group Field = 0x03
*/
APIDECL1 int APIDECL2 HCI_SetEventMask(u_int8 *eventMask);
APIDECL1 int APIDECL2 HCI_Reset(void);

/* for HCI_SetEventFilter() */
typedef struct st_t_HCI_EventFilter { 
	u_int8 filterType;
	u_int8 conditionType;	
	struct st_t_HCI_eventFilter_Condition {
		t_bdaddr address; 
		u_int32 deviceClass;
		u_int32 deviceClassMask;
		u_int8 autoAccept;
	} condition;
} t_HCI_EventFilter;	

APIDECL1 int APIDECL2 HCI_SetEventFilter(t_HCI_EventFilter *filterSpec);

APIDECL1 int APIDECL2 HCI_Flush(u_int16 handle);
APIDECL1 int APIDECL2 HCI_ReadPINType(void);
APIDECL1 int APIDECL2 HCI_WritePINType(u_int8 pinType);
APIDECL1 int APIDECL2 HCI_CreateNewUnitKey(void);
APIDECL1 int APIDECL2 HCI_ReadStoredLinkKey(t_bdaddr address, u_int8 readFlag);


/* for HCI_WriteStoredLinkKey() */
typedef struct st_t_HCI_WriteLinkKey { 
	t_bdaddr address;
	u_int8 linkKey[16];
} t_HCI_WriteLinkKey;

APIDECL1 int APIDECL2 HCI_WriteStoredLinkKey(u_int8 numKeys, t_HCI_WriteLinkKey *keys);

APIDECL1 int APIDECL2 HCI_DeleteStoredLinkKey(t_bdaddr address, u_int8 deleteFlag);
APIDECL1 int APIDECL2 HCI_ChangeLocalName(u_int8 *name); 
APIDECL1 int APIDECL2 HCI_ReadLocalName(void);
APIDECL1 int APIDECL2 HCI_ReadConnectionAcceptTimeout(void);
APIDECL1 int APIDECL2 HCI_WriteConnectionAcceptTimeout(u_int16 timeout);
APIDECL1 int APIDECL2 HCI_ReadPageTimeout(void);
APIDECL1 int APIDECL2 HCI_WritePageTimeout(u_int16 timeout);
APIDECL1 int APIDECL2 HCI_ReadScanEnable(void);
APIDECL1 int APIDECL2 HCI_WriteScanEnable(u_int8 scanEnable);
APIDECL1 int APIDECL2 HCI_ReadPageScanActivity(void);
APIDECL1 int APIDECL2 HCI_WritePageScanActivity(u_int16 interval, u_int16 window);
APIDECL1 int APIDECL2 HCI_ReadInquiryScanActivity(void);
APIDECL1 int APIDECL2 HCI_WriteInquiryScanActivity(u_int16 interval, u_int16 window);
APIDECL1 int APIDECL2 HCI_ReadAuthenticationEnable(void);
APIDECL1 int APIDECL2 HCI_WriteAuthenticationEnable(u_int8 enable);
APIDECL1 int APIDECL2 HCI_ReadEncryptionMode(void);
APIDECL1 int APIDECL2 HCI_WriteEncryptionMode(u_int8 mode);
APIDECL1 int APIDECL2 HCI_ReadClassOfDevice(void);
APIDECL1 int APIDECL2 HCI_WriteClassOfDevice(u_int32 cod);
APIDECL1 int APIDECL2 HCI_ReadVoiceSetting(void);
APIDECL1 int APIDECL2 HCI_WriteVoiceSetting(u_int16 voiceSetting);
APIDECL1 int APIDECL2 HCI_ReadAutomaticFlushTimeout(u_int16 handle);
APIDECL1 int APIDECL2 HCI_WriteAutomaticFlushTimeout(u_int16 handle, u_int16 timeout);
APIDECL1 int APIDECL2 HCI_ReadNumBroadcastRetransmissions(void);
APIDECL1 int APIDECL2 HCI_WriteNumBroadcastRetransmissions(u_int8 number);
APIDECL1 int APIDECL2 HCI_ReadHoldModeActivity(void);
APIDECL1 int APIDECL2 HCI_WriteHoldModeActivity(u_int8 activity);
APIDECL1 int APIDECL2 HCI_ReadTransmitPowerLevel(u_int16 handle, u_int8 type);
APIDECL1 int APIDECL2 HCI_ReadSCOFlowControlEnable(void);
APIDECL1 int APIDECL2 HCI_WriteSCOFlowControlEnable(u_int8 enable);
APIDECL1 int APIDECL2 HCI_SetHostControllerToHostFlowControl(u_int8 enable);
APIDECL1 int APIDECL2 HCI_HostBufferSize(u_int16 ACLlength, u_int8 SCOlength, u_int16 totalACLpackets, u_int16 totalSCOpackets);


/* for HCI_HostNumberOfCompletedPackets() */
typedef struct st_t_HCI_numPackets { 
	u_int16 handle;
	u_int16 numCompleted;
} t_HCI_NumPackets;

APIDECL1 int APIDECL2 HCI_HostNumberOfCompletedPackets(u_int8 numHandles, t_HCI_NumPackets *numPackets);

APIDECL1 int APIDECL2 HCI_ReadLinkSupervisionTimeout(u_int16 handle);
APIDECL1 int APIDECL2 HCI_WriteLinkSupervisionTimeout(u_int16 handle, u_int16 timeout);
APIDECL1 int APIDECL2 HCI_ReadNumberOfSupportedIAC(void);
APIDECL1 int APIDECL2 HCI_ReadCurrentIACLAP(void);
APIDECL1 int APIDECL2 HCI_WriteCurrentIACLAP(u_int8 numLap, u_int32 *lap);
APIDECL1 int APIDECL2 HCI_ReadPageScanPeriodMode(void);
APIDECL1 int APIDECL2 HCI_WritePageScanPeriodMode(u_int8 mode);
APIDECL1 int APIDECL2 HCI_ReadPageScanMode(void);
APIDECL1 int APIDECL2 HCI_WritePageScanMode(u_int8 mode);


/*
	HCI informational commands - Opcode Group Field = 0x04
*/
APIDECL1 int APIDECL2 HCI_ReadLocalVersionInformation(void);
APIDECL1 int APIDECL2 HCI_ReadLocalSupportedFeatures(void);
APIDECL1 int APIDECL2 HCI_ReadBufferSize(void);
APIDECL1 int APIDECL2 HCI_ReadCountryCode(void);
APIDECL1 int APIDECL2 HCI_ReadBDAddress(void);


/*
	HCI status commands - Opcode Group Field = 0x05
*/
APIDECL1 int APIDECL2 HCI_ReadFailedContactCounter(u_int16 handle);
APIDECL1 int APIDECL2 HCI_ResetFailedContactCounter(u_int16 handle);
APIDECL1 int APIDECL2 HCI_GetLinkQuality(u_int16 handle);
APIDECL1 int APIDECL2 HCI_ReadRSSI(u_int16 handle);


/*
	HCI test commands - Opcode Group Field = 0x06
*/
APIDECL1 int APIDECL2 HCI_ReadLoopbackMode(void);
APIDECL1 int APIDECL2 HCI_WriteLoopbackMode(u_int8 mode);
APIDECL1 int APIDECL2 HCI_EnableDUT(void);



#endif /* BLUETOOTH_HCI_API_DECLARED */
