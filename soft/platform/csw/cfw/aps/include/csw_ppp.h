/******************************************************************************/
/*                                                                            */
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* This source code is property of Coolsand. The information contained in     */
/* this file is confidential. Distribution, reproduction, as well as          */
/* exploitation,or transmisison of any content of this file is not            */
/* allowed except if expressly permitted.                                     */
/* Infringements result in damage claims!                                     */
/*                                                                            */
/* FILE NAME                                                                  */
/*      Csw_ppp.h                                                             */
/*                                                                            */
/* DESCRIPTION                                                                */
/*      in this file, parse the ppp packet and process the ppp packet         */
/* Author                                                                     */
/*      ????                                                                  */
/* modify record                                                              */
/*      2008-05-16    wuys                                                    */
/******************************************************************************/
#include <csw.h>
#ifdef CFW_TCPIP_SUPPORT


#ifndef _CSW_PPP_
#define _CSW_PPP_



#define MAX_HDLC_PACKET_SIZE 	3000	//max size of hdlc packet, I think it's enough
#define MAX_PPP_PACKET_SIZE 	1600	//max size of IP packet is 1500, we add some for ppp

// Global variances about PPP status
#define PPP_STATUS_PPP_NULL		0
#define PPP_STATUS_LCP_START	1
#define PPP_STATUS_LCP_ACKED	2
#define PPP_STATUS_NCP_START	3
#define PPP_STATUS_NCP_ACKED	4
#define PPP_STATUS_LCP_STOP     5

//the following is HDLC symbol
#define PPP_SYN_BYTE		0x7e	//HDLC byte
#define PPP_ESCAPE_BYTE		0x7d
#define PPP_ESCAPE_VALUE	0x20
#define PPP_INITFCS16		0xffff	// Initial FCS value
#define PPP_GOODFCS16		0xf0b8	// Good final FCS value

//the following is LCP code
// Control Protocols (LCP/IPCP/CCP etc.) Codes defined in RFC 1661
#define PPP_CPCODES_VEXT		0	/* Vendor-Specific (RFC2153) */
#define PPP_CPCODES_CONF_REQ	1	/* Configure-Request */
#define PPP_CPCODES_CONF_ACK	2	/* Configure-Ack */
#define PPP_CPCODES_CONF_NAK	3	/* Configure-Nak */
#define PPP_CPCODES_CONF_REJ	4	/* Configure-Reject */
#define PPP_CPCODES_TERM_REQ	5	/* Terminate-Request */
#define PPP_CPCODES_TERM_ACK	6	/* Terminate-Ack */
#define PPP_CPCODES_CODE_REJ	7	/* Code-Reject */
#define PPP_CPCODES_PROT_REJ	8	/* Protocol-Reject (LCP only) */
#define PPP_CPCODES_ECHO_REQ	9	/* Echo-Request (LCP only) */
#define PPP_CPCODES_ECHO_RPL	10	/* Echo-Reply (LCP only) */
#define PPP_CPCODES_DISC_REQ	11	/* Discard-Request (LCP only) */
#define PPP_CPCODES_ID			12	/* Identification (LCP only) RFC1570 */
#define PPP_CPCODES_TIME_REM	13	/* Time-Remaining (LCP only) RFC1570 */
#define PPP_CPCODES_RESET_REQ	14	/* Reset-Request (CCP only) RFC1962 */
#define PPP_CPCODES_RESET_REP	15	/* Reset-Reply (CCP only) */

//the following is LCP config option
#define PPP_LCPOPT_VEXT		0
#define PPP_LCPOPT_MRU		1
#define PPP_LCPOPT_ACCM		2
#define PPP_LCPOPT_AP		3
#define PPP_LCPOPT_QP		4
#define PPP_LCPOPT_MN		5
#define PPP_LCPOPT_DEP6		6
#define PPP_LCPOPT_PFC		7
#define PPP_LCPOPT_ACFC		8
#define PPP_LCPOPT_FCSALT	9
#define PPP_LCPOPT_SDP		10
#define PPP_LCPOPT_NUMMODE	11
#define PPP_LCPOPT_DEP12	12
#define PPP_LCPOPT_CBACK	13
#define PPP_LCPOPT_DEP14	14
#define PPP_LCPOPT_DEP15	15
#define PPP_LCPOPT_DEP16	16
#define PPP_LCPOPT_MLMRRU	17
#define PPP_LCPOPT_MLSSNHF	18
#define PPP_LCPOPT_MLED		19
#define PPP_LCPOPT_PROP		20
#define PPP_LCPOPT_DCEID	21
#define PPP_LCPOPT_MPP		22
#define PPP_LCPOPT_LD		23
#define PPP_LCPOPT_LCPAOPT	24
#define PPP_LCPOPT_COBS		25
#define PPP_LCPOPT_PE		26
#define PPP_LCPOPT_MLHF		27
#define PPP_LCPOPT_I18N		28
#define PPP_LCPOPT_SDLOS	29
#define PPP_LCPOPT_PPPMUX	30

//the following is NCP config option
#define PPP_IPCP_CI_ADDRS			1	/* IP Addresses */
#define PPP_IPCP_CI_COMPRESSTYPE	2	/* Compression Type */
#define PPP_IPCP_CI_ADDR			3
#define PPP_IPCP_CI_MS_DNS1		129	/* Primary DNS value */
#define PPP_IPCP_CI_MS_WINS1		130	/* Primary WINS value */
#define PPP_IPCP_CI_MS_DNS2		131	/* Secondary DNS value */
#define PPP_IPCP_CI_MS_WINS2		132	/* Secondary WINS value */

typedef struct
{
	UINT8   HdlcBuffer[MAX_HDLC_PACKET_SIZE];
	UINT16  HdlcPosition;
	UINT8   Buffer[MAX_PPP_PACKET_SIZE];
	UINT16  Position;
	UINT8   Status;
	UINT8   Id;   // Global variances about the id of PPP packet
	UINT32	  MagicNumber;
	// Global variances about NCP packet
	UINT32	  NcpIP;
	UINT32	  NcpDNS1;
	UINT32	  NcpDNS2;
	UINT32	  NcpWINS1;
	UINT32	  NcpWINS2;
	// Global variances about SM context
	UINT32	  SmNas;
	UINT32	  SmIP;
	UINT32	  SmDNS1;
	UINT32	  SmDNS2;
	UINT32	  SmWINS1;
	UINT32	  SmWINS2;
} ppp_Data_t;


UINT8  ppp_DecodeHDLC ();
void ppp_ProcessPPP (UINT8* packet, UINT16 size);
UINT16 ppp_fcs16(UINT16 fcs, UINT8* cp, int len);
void ppp_ProcessLcpPacket ();
UINT8 ppp_ParseLcpConfig ();
void ppp_SendLcpConfig (UINT8 code);
void ppp_EncodeHDLC ();
void ppp_SendPPPPacket();
void  ppp_SendLcpTermimate (UINT8 code);
void ppp_SendPppPacket (UINT8* buffer, UINT16 size);
void ppp_ProcessNcpPacket ();
UINT8 ppp_ParseNcpConfig ();
void ppp_FetchSMContext();
void  ppp_SendNcpConfig (UINT8 code);
//void  ppp_SendCcpConfig (UINT8 code);
void  ppp_SendNcpTermimate (UINT8 code);
void ppp_ProcessIpPacket ();
void ppp_SendIpPacket (UINT8* buffer, UINT16 size);
void ppp_DisplayPacket (char* title, UINT8* buffer, UINT16 size);
void ppp_DisplayStatus (char* format, ...);
void ppp_InitVars();
void ppp_Init (VOID);
void ppp_OnReceiveIpPacket(UINT8* packet, UINT16 size);
void ppp_ProcessPapPacket (void);
void ppp_SendPapConfig (UINT8 code);

#endif
#endif
