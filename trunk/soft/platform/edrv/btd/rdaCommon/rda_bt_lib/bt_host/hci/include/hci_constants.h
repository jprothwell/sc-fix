/*
 * MODULE NAME:    hci_constants.h
 * DESCRIPTION:    HCI Constant Definitions
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: hci_constants.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
  */


#ifndef prh_HCI_CONSTANTS_H
#    define prh_HCI_CONSTANTS_H

#include "host_config.h"


/* HCI Version */
#define PRH_HCI_VERSION_BLUETOOTHSPEC10B 0
#define PRH_HCI_VERSION_BLUETOOTHSPEC11  1


/* Link types for connection complete, etc */
#define HCI_linkACL         0x01
#define HCI_linkSCO         0x00



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
#define HCI_evFLUSHOCCURRED							0x11
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
#define	HCI_evTESTING								0xFE
#define	HCI_evPROPRIETARY							0xFF


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
#define HCI_errBADCRYPTMODE			0x25
#define HCI_errUNITKEYUSED			0x26
#define HCI_errQOSUNSUPPORTED	    0x27
#define HCI_errINSTANTPASSED		0x28
#define HCI_errNOUNITPAIRING		0x29


/*
	Maximum lengths of command pdus not completely encoded by table
*/
#define HCI_cmdSETEVENTFILTER_LEN				0x0C
#define HCI_cmdHOSTNUMCOMPLETEDPACKETS_LEN		0x102
#define HCI_cmdWRITECURRENTIACLAP_LEN			0xC4
#define HCI_cmdWRITESTOREDLINKKEY_LEN			0x102
#define HCI_cmdCHANGELOCALNAME_LEN				0xFB

#endif/* prh_HCI_CONSTANTS_H */


