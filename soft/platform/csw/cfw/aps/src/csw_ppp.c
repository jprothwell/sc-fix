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
/*      Csw_ppp.c                                                             */
/*                                                                            */
/* DESCRIPTION                                                                */
/*      in this file, parse the ppp packet and process the ppp packet         */
/* Author                                                                     */
/*      ????                                                                  */
/* modify record                                                              */
/*      2008-05-16    wuys                                                    */
/******************************************************************************/

#ifdef CFW_TCPIP_SUPPORT
#include "ts.h"
#include <hal_config.h>
#include "sxs_io.h"
#include <csw.h>
#include <base_prv.h>
#include <reg.h>
#include "csw_ppp.h"
#include "aps.h"

ppp_Data_t ppp_Data;
extern BOOL g_Pdp_Acting;


VOID CFW_PppTermProcess(VOID);
//UINT32 CFW_GprsAtt(UINT8 nState, UINT16 nUTI);
BOOL NCP_ACKed = FALSE;


//-----------------------------------------------------------------------------------------------------

//[[hameina[+] 2008.9.8 for clear warning
BOOL uart_PsSendData(UINT8 *pData, UINT16 uDataSize);


//]]hameina[+] 2008.9.8
//--------------------------------------------------------------------------------------------------------
//extern VOID CFW_Dump(UINT16 nModuleIdx, UINT8 *pData, UINT16 nDataSize);
#define memcpy SUL_MemCopy8
#define PPP_DEBUG_PPP_SWITCH CFW_GPRS_TS_ID    


// Macro about byte operation.
#define GET_HIGH_BYTE(val16)	((val16 & 0xff00) >> 8)
#define GET_LOW_BYTE(val16)	(val16 & 0x00ff)
#define GET_1_BYTE(val32)		((val32 & 0xff000000) >> 24)
#define GET_2_BYTE(val32)		((val32 & 0x00ffff00) >> 16)
#define GET_3_BYTE(val32)		((val32 & 0x0000ff00) >> 8)
#define GET_4_BYTE(val32)		(val32 & 0x000000ff)
#define VALUE32(p)				( *((UINT32*) p) )

//////////////////////////////////////////////////////////////////
// Macro about HDLC packet
#define ADD_HDLC_BYTE(val)	{ ppp_Data.HdlcBuffer[ppp_Data.HdlcPosition] = (val); ppp_Data.HdlcPosition++; }

//////////////////////////////////////////////////////////////////
// Macro about PPP packet
#define ADD_PPP_BYTE(val8)	{ ppp_Data.Buffer[ppp_Data.Position] = (val8); ppp_Data.Position++; }
#define ADD_PPP_4_BYTE(p)	\
	{ memcpy(&ppp_Data.Buffer[ppp_Data.Position], p, 4); ppp_Data.Position += 4; }

//////////////////////////////////////////////////////////////////
// Global variances about LCP packet
#define PPP_LCP_HEADER_LEN		4
static UINT8   g_lcpHeader[PPP_LCP_HEADER_LEN] = {0xff, 0x03, 0xc0, 0x21};

// Global variances about lcp-config packet
#define PPP_LCP_CONFIG_LEN		20
static UINT8   g_lcpConfig[PPP_LCP_CONFIG_LEN] = {
	0xff, 0xff, 0x00, 0x14,						//LCP header, the length = 0x14
	PPP_LCPOPT_ACCM, 0x06, 0x00, 0x00, 0x00, 0x00,	//fill AsynchronousControlCharacterMap//0x00, 0x0a, 0x00, 0x00
	PPP_LCPOPT_MN, 0x06, 0xff, 0xff, 0xff, 0xff,	//fill Mafic Number
	PPP_LCPOPT_PFC, 0x02, PPP_LCPOPT_ACFC, 0x02,		//fill ppp header compress
};

#define PPP_LCP_CONFREQ_LEN		24//20 , modify by wuys 2008-04-28
static UINT8   g_lcpConfReq[PPP_LCP_CONFREQ_LEN] = {
	0x01, 0xff, 0x00, 0x18,						//LCP header, the length = 0x14
	PPP_LCPOPT_ACCM, 0x06, 0x00, 0x00, 0x00, 0x00,	//fill AsynchronousControlCharacterMap//0x00, 0x0a, 0x00, 0x00
	PPP_LCPOPT_MN, 0x06, 0xff, 0xff, 0xff, 0xff,	//fill Mafic Number
	PPP_LCPOPT_PFC, 0x02, PPP_LCPOPT_ACFC, 0x02,		//fill ppp header compress
	PPP_LCPOPT_AP, 0x04, 0xc0, 0x23 //PAP, add by wuys 2008-04-28
};
// Macro about lcp-config packet
#define PPP_FILL_LCP_CONFIG_CODE(code) 	{	g_lcpConfig[0] = code;	}
#define PPP_FILL_LCP_CONFIG_ID(id) 		{	g_lcpConfig[1] = id;	}
//#define PPP_FILL_LCP_CONFIG_MN(var) 	{	VALUE32(&g_lcpConfig[12]) = VALUE32(&var);	}
#define PPP_FILL_LCP_CONFIG_MN(var) 	{	memcpy(&g_lcpConfig[12], var, 4);	}
//add by wuys 2008-04-28
#define PPP_FILL_LCP_CONFIG_MN_REQ(var) 	{	memcpy(&g_lcpConfReq[12], var, 4);	}
#define PPP_FILL_LCP_CONFIG_ID_REQ(id) 		{	g_lcpConfReq[1] = id;	}
//add end
//////////////////////////////////////////////////////////////////
// Global variances about NCP packet
#define PPP_NCP_HEADER_LEN		2
static UINT8   g_ncpHeader[PPP_NCP_HEADER_LEN] = {0x80, 0x21};

#define PPP_PAP_HEADER_LEN		2
static UINT8   g_papHeader[PPP_PAP_HEADER_LEN] = {0xc0, 0x23};

// Global variances about ncp-config packet
#define PPP_NCP_CONFIG_REQ_LEN		10
static UINT8   g_ncpConfigReq[PPP_NCP_CONFIG_REQ_LEN] = {
	0xff, 0xff, 0x00, 0x0a,						//NCP header, the length = 0x0a
	PPP_IPCP_CI_ADDR, 06, 0xff, 0xff, 0xff, 0xff,	//fill NAS's IP address
	};

//add by wuys 2008-04-21
#if 0//add by wuys 2008-08-07
static UINT8   g_ncpConfigRej[34] = {
	0xff, 0xff, 0x00, 0x22,
  PPP_IPCP_CI_COMPRESSTYPE, 06, 0x00, 0x00, 0x00, 0x00,	
  PPP_IPCP_CI_MS_DNS1, 06, 0x00, 0x00, 0x00, 0x00,	
	PPP_IPCP_CI_MS_WINS1, 06, 0x00, 0x00, 0x00, 0x00,	
	PPP_IPCP_CI_MS_DNS2, 06, 0x00, 0x00, 0x00, 0x00,	
	PPP_IPCP_CI_MS_WINS2, 06, 0x00, 0x00, 0x00, 0x00,	

};
#else
static UINT8   g_ncpConfigRej[22] = {
	0xff, 0xff, 0x00, 0x16,
  PPP_IPCP_CI_COMPRESSTYPE, 06, 0x00, 0x00, 0x00, 0x00,	
	PPP_IPCP_CI_MS_DNS2, 06, 0x00, 0x00, 0x00, 0x00,	
	PPP_IPCP_CI_MS_WINS2, 06, 0x00, 0x00, 0x00, 0x00,	

};
#endif
static UINT8  g_papAck[9] = {
	0xff, 0xff, 0x00, 0x09,
  0x04, 0x71, 0x79, 0x79, 0x68,	
};
//add end

// Macro about ncp-config packet
#define PPP_FILL_NCP_CONFIG_REQ_CODE(code) 	{	g_ncpConfigReq[0] = code;	}
#define PPP_FILL_NCP_CONFIG_REQ_ID(id) 		{	g_ncpConfigReq[1] = id;		}
#define PPP_FILL_NCP_CONFIG_REQ_IP(var) 	{	memcpy(&g_ncpConfigReq[6], var, 4);	}

//////////////////////////////////////////////////////////////////
// Global variances about IP packet
#define PPP_IP_HEADER_LEN		1
static const UINT8   g_ipHeader[PPP_IP_HEADER_LEN] = {0x21};

// Global variances about CCP packet. add by wuys 2008-04-24
#define PPP_CCP_HEADER_LEN		2
static const UINT8   g_ccpHeader[PPP_CCP_HEADER_LEN] = {0x80,0xfd};
// add end
//////////////////////////////////////////////////////////////////
// Global variances about FCS, Copied from RFC 1662
static const UINT16 ppp_fcstab[256] = {
    0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
    0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
    0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
    0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
    0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
    0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
    0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
    0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
    0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
    0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
    0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
    0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
    0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
    0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
    0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
    0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
    0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
    0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
    0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
    0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
	  0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5, 
	  0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd, 
	  0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134, 
	  0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c, 
	  0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3, 
	  0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb, 
	  0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232, 
	  0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a, 
	  0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1, 
	  0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9, 
	  0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330, 
	  0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78 }; 

//////////////////////////////////////////////////////////////////
// Macro about send PPP packet to serial
#define SEND_PPP_PACKET		{\
	ppp_SendPppPacket (ppp_Data.HdlcBuffer, ppp_Data.HdlcPosition);	\
	ppp_Data.HdlcPosition = 0; \
	ppp_DisplayPacket ("MS->PC:", ppp_Data.HdlcBuffer, ppp_Data.HdlcPosition);	\
	ppp_DisplayStatus ("=====================================================");	\
	ppp_DisplayPacket (" MS->PC  ppp:", ppp_Data.Buffer, ppp_Data.Position);	}

void ppp_SendPPPPacket()
{
	SEND_PPP_PACKET;
}

/*
================================================================================
  Function   : ppp_ProcessPPP
--------------------------------------------------------------------------------

  Scope      : Process an PPP packet from UART
  Parameters :
  Return     :
  Others    : 
================================================================================
*/
void ppp_ProcessPPP (UINT8* packet, UINT16 size)
{
	//copy the packet to buffer
	if (size > MAX_HDLC_PACKET_SIZE)    return;
 
	memcpy (ppp_Data.HdlcBuffer, packet, size);
	ppp_Data.HdlcPosition = size;

	ppp_DisplayStatus ("=====================================================");
	ppp_DisplayPacket ("PC->MS:", ppp_Data.HdlcBuffer, ppp_Data.HdlcPosition);
	//decode HDLC
	
	if (!ppp_DecodeHDLC ())	return;
	
	ppp_DisplayPacket (" PC->MS  ppp:", ppp_Data.Buffer, ppp_Data.Position);
	ppp_DisplayStatus("decode hdlc over");

	if ( (ppp_Data.Buffer[0] == g_lcpHeader[0])
		&& (ppp_Data.Buffer[1] == g_lcpHeader[1])
		&& (ppp_Data.Buffer[2] == g_lcpHeader[2])
		&& (ppp_Data.Buffer[3] == g_lcpHeader[3])
		)	//check only first 4 bytes
	{
		ppp_DisplayStatus("LCP begin");
		ppp_ProcessLcpPacket ();	//this is LCP packet
		ppp_DisplayStatus("LCP end");
	}
	else if ((ppp_Data.Buffer[0] == g_ncpHeader[0]) && (ppp_Data.Buffer[1] == g_ncpHeader[1]))
	{
		ppp_DisplayStatus("NCP begin");
		ppp_ProcessNcpPacket ();	//this is NCP packet
		ppp_DisplayStatus("NCP end");
	}
	else if (ppp_Data.Buffer[0] == g_ipHeader[0])
	{
		ppp_DisplayStatus("IP begin");
    if(!NCP_ACKed)
      NCP_ACKed = TRUE;
		ppp_ProcessIpPacket ();	//this is IP packet
		ppp_DisplayStatus("IP end");
	}
	else if ((ppp_Data.Buffer[0] == g_ccpHeader[0]) && (ppp_Data.Buffer[1] == g_ccpHeader[1]))
	{
		ppp_DisplayStatus("received a CCP Packet");
	}else if ((ppp_Data.Buffer[0] == g_papHeader[0]) && (ppp_Data.Buffer[1] == g_papHeader[1]))
	{
		ppp_DisplayStatus("received a PAP Packet");
    ppp_ProcessPapPacket();
	}else
	{
		CSW_TRACE(PPP_DEBUG_PPP_SWITCH,"###ERROR: Unknown packet (%x %x)", ppp_Data.Buffer[0], ppp_Data.Buffer[1]);
	}

}

/*
================================================================================
  Function   : ppp_DecodeHDLC
--------------------------------------------------------------------------------

  Scope      : Decode HDLC packet as PPP packet
  Parameters :
  Return     : If is good packet, return 1, else return 0;
  Others    : 
================================================================================
*/
UINT8  ppp_DecodeHDLC ()
{
	UINT16 i, bAfterExcape = 0;
	UINT16 trialfcs;
	
	//reset ppp buffer	
	ppp_Data.Position = 0;
    ppp_DisplayStatus("in function DecodeHDLC");
	//if found PPP_ESCAPE_BYTE, decode it.
	for (i = 0; i < ppp_Data.HdlcPosition; i++)
	{
		if (ppp_Data.HdlcBuffer[i] == PPP_SYN_BYTE)
		{
			// just omit it.
		}
		else if (ppp_Data.HdlcBuffer[i] == PPP_ESCAPE_BYTE)
		{
			bAfterExcape = 1;
		}
		else
		{
			//move it
			if (bAfterExcape)
				ADD_PPP_BYTE (ppp_Data.HdlcBuffer[i] ^ PPP_ESCAPE_VALUE)
			else
				ADD_PPP_BYTE (ppp_Data.HdlcBuffer[i])
			//go next
			bAfterExcape = 0;
		}
		
		//check the packet size
		if (ppp_Data.Position >= MAX_PPP_PACKET_SIZE)	return 0;
	}

	//check fcs
	trialfcs = ppp_fcs16( PPP_INITFCS16, ppp_Data.Buffer, ppp_Data.Position);
    if ( trialfcs == PPP_GOODFCS16 )
    {
		//do nothing
	}
	else
	{		
		ppp_DisplayStatus ("###ERROR: FCS checking failed");
		return 0;
	}

	//remove the fcs
	if (ppp_Data.Position > 2)
	{
		ppp_Data.Position -= 2;
	}
	else
	{		
		ppp_DisplayStatus ("###ERROR: The PPP packet is too short");
		return 0;
	}
	
	return 1;
}

/*
================================================================================
  Function   : ppp_fcs16
--------------------------------------------------------------------------------

  Scope      : Calculate a new fcs given the current fcs and the new data.
  Parameters :
  Return     :
  Others    : Copied from RFC 1662 
================================================================================
*/
UINT16 ppp_fcs16(UINT16 fcs, UINT8* cp, int len)
{
    while (len--)
        fcs = (fcs >> 8) ^ ppp_fcstab[(fcs ^ *cp++) & 0xff];
	return (fcs); 
} 

/*
================================================================================
  Function   : ppp_ProcessLcpPacket
--------------------------------------------------------------------------------

  Scope      : Process an LCP packet from TAF
  Parameters :
  Return     :
  Others    : 
================================================================================
*/
void ppp_ProcessLcpPacket ()
{
	UINT8 isGoodOption;
	ppp_DisplayStatus ("   in function : ProcessLcpPacket");	
	ppp_Data.Id = ppp_Data.Buffer[PPP_LCP_HEADER_LEN + 1];

	//first byte is the code
	switch (ppp_Data.Buffer[PPP_LCP_HEADER_LEN])
	{
       	case PPP_CPCODES_CONF_REQ:
       		ppp_DisplayStatus ("   LCP:This is a LCP_CONFIG_REQ packet");
       		//////response it with LCP-Configure-Ack
       		isGoodOption = ppp_ParseLcpConfig();
            
       		if (ppp_Data.Status == PPP_STATUS_PPP_NULL)
       		{
       			ppp_DisplayStatus ("------------ENTER LCP_START STATUS---------------");
       			ppp_Data.Status = PPP_STATUS_LCP_START;
       		}
                     //add by wuys 2008-04-19
                     if(isGoodOption==2)
                     {
                           ppp_SendLcpConfig (PPP_CPCODES_CONF_REJ);
                           break;
                     }
                     //add end
               
       		if (isGoodOption == 1)
       		{
       			ppp_SendLcpConfig (PPP_CPCODES_CONF_ACK);
       			ppp_DisplayStatus ("   LCP: Send a LCP_CONFIG_ACK packet");
       		}
       		else
       		{
       			ppp_SendLcpConfig (PPP_CPCODES_CONF_NAK);
       			ppp_DisplayStatus ("   LCP: Send a LCP_CONFIG_NAK packet");
       		}
                     COS_Sleep(50);
                     if (ppp_Data.Status == PPP_STATUS_LCP_START)
       		{
       			ppp_SendLcpConfig (PPP_CPCODES_CONF_REQ);
       			ppp_DisplayStatus ("   LCP: Send a LCP_CONFIG_REQ packet");
       		}
       		break;
           
       	case PPP_CPCODES_CONF_ACK:
       		ppp_ParseLcpConfig();
       		ppp_Data.Status = PPP_STATUS_LCP_ACKED;
       		ppp_DisplayStatus ("------------ENTER LCP_ACKED STATUS---------------");
       		break;
           
       	case PPP_CPCODES_CONF_NAK:
       	case PPP_CPCODES_CONF_REJ:
       		ppp_DisplayStatus ("###ERROR: You client don't support our options");
       		ppp_SendLcpTermimate (PPP_CPCODES_TERM_REQ);
       		ppp_DisplayStatus ("   LCP: Send a LCP_TERMINATE_REQ packet");
       		ppp_Data.Status = PPP_STATUS_PPP_NULL;
                     COS_Sleep(50);
                     CFW_PppTermProcess();
       		ppp_DisplayStatus ("------------ENTER PPP_NULL STATUS---------------");
       		break;
           
       	case PPP_CPCODES_TERM_REQ:
       		ppp_DisplayStatus ("   LCP:This is a LCP-Terminate-Req packet");
       		ppp_SendLcpTermimate (PPP_CPCODES_TERM_ACK);
       		ppp_DisplayStatus ("   LCP: Send a LCP_TERMINATE_ACK packet");
                     COS_Sleep(50);
                     CFW_PppTermProcess();
                     ppp_Data.Status = PPP_STATUS_PPP_NULL;
                     NCP_ACKed = FALSE;
       		ppp_DisplayStatus ("------------ENTER PPP_NULL STATUS---------------");
       		break;
           
       	case PPP_CPCODES_TERM_ACK:
       		ppp_DisplayStatus ("   LCP:This is a LCP-Terminate-Ack packet");
                     CFW_PppTermProcess();
       		break;
           
       	default:
       		ppp_DisplayStatus ("   LCP:This is an unknown packet");
       		break;
	}  //end of switch	
}

/*
================================================================================
  Function   : ppp_ParseLcpConfig
--------------------------------------------------------------------------------

  Scope      : Parse the LCP-CONFIG-XXX packet from TAF
  Parameters :
  Return     : If all options are the same as expect, return 1; else return 0.
  Others    : 
================================================================================
*/
UINT8 ppp_ParseLcpConfig ()
{
	UINT16 i;
	UINT8 haveACCM = 0;
	UINT8 havePFC = 0;
	UINT8 haveACFC = 0;
	UINT8 haveOthers = 0;
	
	//the 7th & 8th byte is the length	
	int len = ppp_Data.Buffer[PPP_LCP_HEADER_LEN + 2] * 256 
		+ ppp_Data.Buffer[PPP_LCP_HEADER_LEN + 3];

	CSW_TRACE(CFW_GPRS_TS_ID,"   LCP Length is 0x%x", len);
	len += PPP_LCP_HEADER_LEN;	//make the length include PPP header
	
	for (i = PPP_LCP_HEADER_LEN + 4; i < len; )
	{
   #define  showLcpOption(title)	\
		   {	ppp_DisplayPacket (title, &ppp_Data.Buffer[i+2], (UINT16) (ppp_Data.Buffer[i+1] - 2));	}

		switch (ppp_Data.Buffer[i])
		{			
			case PPP_LCPOPT_ACCM:
				if ((ppp_Data.Buffer[i+2] == g_lcpConfig[6])
					&& (ppp_Data.Buffer[i+3] == g_lcpConfig[7])
					&& (ppp_Data.Buffer[i+4] == g_lcpConfig[8])
					&& (ppp_Data.Buffer[i+5] == g_lcpConfig[9]))
					haveACCM = 1;
				showLcpOption ("   AsynchronousControlCharacterMap:");
				break;
        
			case PPP_LCPOPT_MN:
				memcpy(&ppp_Data.MagicNumber, &ppp_Data.Buffer[i+2], 4);
				showLcpOption ("   MagicNumber:");
				break;			
        
			case PPP_LCPOPT_PFC:
				havePFC = 1;
				showLcpOption ("   ProtocolCompression:");
				break;
        
			case PPP_LCPOPT_ACFC:
				haveACFC = 1;
				showLcpOption ("   AddressCompression:");
				break;			
        
			default:
				ppp_DisplayStatus ("Unknown option 0x%x ", ppp_Data.Buffer[i]);
				haveOthers = 1;//remove by wuys 2008-04-19
        return 2;
				break;
		}	//end of switch

		// go next, the minimized step is 2.
		if (ppp_Data.Buffer[i+1] >= 2)		
			i +=  ppp_Data.Buffer[i+1];
		else							
			break;
	}
	
	//return result
	if (haveACCM && havePFC && haveACFC && !haveOthers)		
		return 1;
	else													
		return 0;
}

/*
================================================================================
  Function   : ppp_SendLcpConfig
--------------------------------------------------------------------------------

  Scope      : Send LCP-CONFIG-XXX to TAF
  Parameters :
  Return     :
  Others    : 
================================================================================
*/
void ppp_SendLcpConfig (UINT8 code)
{
	//fill code

	UINT32 iTempMN = 0;
	PPP_FILL_LCP_CONFIG_CODE (code);
	//fill id
	if (code == PPP_CPCODES_CONF_REQ)
              PPP_FILL_LCP_CONFIG_ID_REQ(ppp_Data.Id + 100)
	else
		PPP_FILL_LCP_CONFIG_ID (ppp_Data.Id)
	//fill magic number
	if (code == PPP_CPCODES_CONF_REQ)
	{
              iTempMN = ppp_Data.MagicNumber + 1;
              PPP_FILL_LCP_CONFIG_MN_REQ(&iTempMN)
	}
	else
	{
		PPP_FILL_LCP_CONFIG_MN (&ppp_Data.MagicNumber)	
	}
       if(code == PPP_CPCODES_CONF_REJ)
       {
              UINT8 rej[]={0x04, 0x00, 0x00, 0x07, 0x0d, 0x03, 0x06};
              memcpy (&ppp_Data.Buffer[0], g_lcpHeader, PPP_LCP_HEADER_LEN);
              memcpy (&ppp_Data.Buffer[PPP_LCP_HEADER_LEN], rej, 7);
              ppp_Data.Position = PPP_LCP_HEADER_LEN + 7;
       
       }else if(code == PPP_CPCODES_CONF_REQ){
       
       //copy to ppp_Data.Buffer
       	memcpy (&ppp_Data.Buffer[0], g_lcpHeader, PPP_LCP_HEADER_LEN);
       	memcpy (&ppp_Data.Buffer[PPP_LCP_HEADER_LEN], g_lcpConfReq, PPP_LCP_CONFREQ_LEN);
       	ppp_Data.Position = PPP_LCP_HEADER_LEN + PPP_LCP_CONFREQ_LEN;
       }else
       {
       //copy to ppp_Data.Buffer
       	memcpy (&ppp_Data.Buffer[0], g_lcpHeader, PPP_LCP_HEADER_LEN);
       	memcpy (&ppp_Data.Buffer[PPP_LCP_HEADER_LEN], g_lcpConfig, PPP_LCP_CONFIG_LEN);
       	ppp_Data.Position = PPP_LCP_HEADER_LEN + PPP_LCP_CONFIG_LEN;
       
       }
	//encode 
	ppp_EncodeHDLC ();

	//and send it 
		SEND_PPP_PACKET;
//	}
}

/*
================================================================================
  Function   : ppp_EncodeHDLC
--------------------------------------------------------------------------------

  Scope      : Encode PPP packet as HDLC packet
  Parameters :
  Return     :
  Others    : 
================================================================================
*/
void ppp_EncodeHDLC ()
{	
	UINT16 i, trialfcs;
    ppp_DisplayStatus("in function ecodeHDLC");
	
	//add FCS to the tail of ppp buffer
	trialfcs = ppp_fcs16 (PPP_INITFCS16, ppp_Data.Buffer, ppp_Data.Position);
    trialfcs ^= 0xffff;        // complement 
	ADD_PPP_BYTE (GET_LOW_BYTE(trialfcs));
	ADD_PPP_BYTE (GET_HIGH_BYTE(trialfcs));

	//reset HDLC buffer
	ppp_Data.HdlcPosition = 0;

	//deal with error
	if (ppp_Data.Position >= MAX_PPP_PACKET_SIZE)	return;

	//add PPP_SYN_BYTE as the first byte
	ADD_HDLC_BYTE (PPP_SYN_BYTE);

	//add HDLC	
	for (i = 0; i < ppp_Data.Position; i++)
	{
		if ((ppp_Data.Buffer[i] <= 0x20) || (ppp_Data.Buffer[i] == PPP_ESCAPE_BYTE) || 
			(ppp_Data.Buffer[i] == PPP_SYN_BYTE))
		{
			//escape the control symbols
			ADD_HDLC_BYTE (PPP_ESCAPE_BYTE);
			ADD_HDLC_BYTE (ppp_Data.Buffer[i] ^ PPP_ESCAPE_VALUE);		
		}
		else
		{
			//copy the normal symbols
			ADD_HDLC_BYTE (ppp_Data.Buffer[i]);
		}
	}   //end of for

	//add PPP_SYN_BYTE as the last byte
	ADD_HDLC_BYTE (PPP_SYN_BYTE);
}

/*
================================================================================
  Function   : ppp_SendLcpTermimate
--------------------------------------------------------------------------------

  Scope      : Send LCP-TERM-XXX to TAF
  Parameters :
  Return     :
  Others    : 
================================================================================
*/
void  ppp_SendLcpTermimate (UINT8 code)
{
	if (code == PPP_CPCODES_TERM_REQ)
	{
		//copy to ppp_Data.Buffer
		memcpy (&ppp_Data.Buffer[0], g_lcpHeader, PPP_LCP_HEADER_LEN);	
		ppp_Data.Position = PPP_LCP_HEADER_LEN;
		
		//fill lcp
		ADD_PPP_BYTE (code);	ADD_PPP_BYTE (ppp_Data.Id + 100);
		ADD_PPP_BYTE (0);		ADD_PPP_BYTE (4);
	}
	else
	{
		//just modify from origin ppp packet
		ppp_Data.Buffer[PPP_LCP_HEADER_LEN] = code;
	}
	
	//encode 
	ppp_EncodeHDLC ();

	SEND_PPP_PACKET;
}

/*
================================================================================
  Function   : ppp_SendPppPacket
--------------------------------------------------------------------------------

  Scope      : Send PPP packet to TAF
  Parameters :
  Return     :
================================================================================
*/
extern BOOL (*PPP_SendFuc)(UINT8* buffer, UINT16 size);//add by wuys 2008-7-21

void ppp_SendPppPacket (UINT8* buffer, UINT16 size)
{
// add by wuys 2008-05-05

   if(NULL == buffer)
   {
      return ;
   }
    CSW_TRACE(CFW_GPRS_TS_ID, TSTXT("APS send data to Uart.\n"));
  

 //modify by wuys 2008-07-21 for other project except AT to compile correctly
  #if 0
   if(PPP_SendFuc)
     PPP_SendFuc(buffer, size);
  #else
  uart_PsSendData(buffer, size);
  #endif
 //modify end

}

/*
================================================================================
  Function   : ppp_ProcessNcpPacket
--------------------------------------------------------------------------------

  Scope      : Process NCP packet from AT
  Parameters :
  Return     :
  Others    : 
================================================================================
*/
void ppp_ProcessNcpPacket ()
{
	UINT8 isGoodOption;
	//
	ppp_DisplayStatus ("   in function : ProcessNcpPacket %d, %d",ppp_Gprs_state.PPP_Att,ppp_Gprs_state.PPP_Act);	
	ppp_Data.Id = ppp_Data.Buffer[PPP_NCP_HEADER_LEN + 1];
  
  if(g_Pdp_Acting == TRUE)
     return;
//	ppp_Data.Status = PPP_STATUS_NCP_START;
  
  
  // check the PDP context state, if no active,  ppp_Gprs_state.PPP_Cid == 0
  if(ppp_Gprs_state.PPP_Cid == 0)
  {
    //CFW_NW_STATUS_INFO sStatusInfo;
    UINT8 actState=0;
    UINT8 attState=0;
      // check the GPRS attach, if no attach, attState == 0

    //CFW_GetGprsAttState(&attState);
    
    if(attState == 0)
    {
      ppp_Data.Status = PPP_STATUS_LCP_ACKED;
      g_Pdp_Acting = TRUE;
      //Cfw_SetApsApp(APS_PPP);
      ppp_DisplayStatus ("   no attach, attaching");	
      ppp_Gprs_state.PPP_Att = TRUE;
      
      
      #if 1
         //CFW_GprsAtt(CFW_GPRS_ATTACHED, 1);

      #else
      UINT8 uTi;
      if(ERR_SUCCESS == CFW_GetFreeUTI(CFW_NW_SRV_ID, &uTi))
      {
         ppp_DisplayStatus ("   attaching , uti  %d",uTi);	

        CFW_GprsAtt(CFW_GPRS_ATTACHED, uTi);
      }
      else
         ppp_DisplayStatus ("   no free uTi ");	
      #endif
      
      return;
    }

   // if attached, search the actived PDP , if no actived PDP, cur_Cid >7, and need active a PDP 
    UINT8 cur_Cid;
    for(cur_Cid = 1; cur_Cid <= 7; cur_Cid ++)
    {
        //CFW_GetGprsActState(cur_Cid,&actState);
        
        if(actState == 1)
         break;
    }
    
    if(cur_Cid > 7)
    {
        ppp_Gprs_state.PPP_Cid = 1;
        ppp_Data.Status = PPP_STATUS_LCP_ACKED;
        g_Pdp_Acting = TRUE;
        //Cfw_SetApsApp(APS_PPP);
        ppp_DisplayStatus ("   no active activing  pdp context");	
        ppp_Gprs_state.PPP_Act = TRUE;
        
        #if 1
          //CFW_GprsAct(CFW_GPRS_ACTIVED, ppp_Gprs_state.PPP_Cid, 2);

        #else
        UINT8 uTi;
        if(ERR_SUCCESS == CFW_GetFreeUTI(CFW_GPRS_SRV_ID, &uTi))
          CFW_GprsAct(CFW_GPRS_ACTIVED, ppp_Gprs_state.PPP_Cid, uTi);
        else
          ppp_DisplayStatus ("   no free uTi ");	
        
    #endif
        return;
    }else
    {
      ppp_Gprs_state.PPP_Cid = cur_Cid;
    }
  }else
  {
            ppp_DisplayStatus ("   pdp context active");	

    
  }
    //if ppp_Gprs_state.PPP_Cid !=0, start handle IPCP packet
	//first byte is the code
	switch (ppp_Data.Buffer[PPP_NCP_HEADER_LEN])
	{
	case PPP_CPCODES_CONF_REQ:
		ppp_DisplayStatus ("   NCP:This is a NCP_CONFIG_REQ packet");
		//////response it with NCP-Configure-Ack
		isGoodOption = ppp_ParseNcpConfig();

    if(isGoodOption == 2)
    {
      ppp_SendNcpConfig (PPP_CPCODES_CONF_REJ);
			ppp_DisplayStatus ("   NCP: Send a NCP_CONFIG_REJ packet");
      return;
    }
		if(ppp_Data.Status == PPP_STATUS_LCP_ACKED)
		{

      ppp_Data.Status = PPP_STATUS_NCP_START;
			ppp_FetchSMContext ();	//fetch SM context
		
      CSW_TRACE(PPP_DEBUG_PPP_SWITCH,"In Function: ProcessNcpPacket\n");
			ppp_DisplayStatus ("------------ENTER NCP_START STATUS---------------");			
		}
		
		if (isGoodOption)
		{
      if(NCP_ACKed)
  		{
  			ppp_SendNcpConfig (PPP_CPCODES_CONF_ACK);
  			ppp_DisplayStatus ("   NCP: Send a NCP_CONFIG_ACK packet");
  		}
		}
		else
		{
			ppp_SendNcpConfig (PPP_CPCODES_CONF_NAK);
			ppp_DisplayStatus ("   NCP: Send a NCP_CONFIG_NAK packet");
		}
     
    if ((ppp_Data.Status == PPP_STATUS_NCP_START)&&(!NCP_ACKed))
		{
      COS_Sleep(50);
			ppp_SendNcpConfig (PPP_CPCODES_CONF_REQ);
			ppp_DisplayStatus ("   NCP: Send a NCP_CONFIG_REQ packet");
		}
       
		break;
    
	case PPP_CPCODES_CONF_ACK:
		ppp_ParseNcpConfig();
    NCP_ACKed = TRUE;
		ppp_Data.Status = PPP_STATUS_NCP_ACKED;
		ppp_DisplayStatus ("------------ENTER NCP_ACKED STATUS---------------");
		break;
    
	case PPP_CPCODES_CONF_NAK:	
	case PPP_CPCODES_CONF_REJ:
		ppp_ParseNcpConfig ();
		ppp_DisplayStatus ("###ERROR: You client don't support our options");
		ppp_SendNcpTermimate (PPP_CPCODES_TERM_REQ);
		ppp_DisplayStatus ("   LCP: Send a NCP_TERMINATE_REQ packet");
		ppp_Data.Status = PPP_STATUS_LCP_STOP;
		ppp_DisplayStatus ("------------ENTER LCP_STOP STATUS---------------");
		break;
    
	case PPP_CPCODES_TERM_REQ:
		ppp_DisplayStatus ("   NCP:This is a NCP-Terminate-Req packet");
		ppp_SendNcpTermimate (PPP_CPCODES_TERM_ACK);
		ppp_DisplayStatus ("   LCP: Send a NCP_TERMINATE_ACK packet");
		ppp_Data.Status = PPP_STATUS_LCP_STOP;
		ppp_DisplayStatus ("------------ENTER LCP_STOP STATUS---------------");
		break;
    
	case PPP_CPCODES_TERM_ACK:
		ppp_DisplayStatus ("   NCP:This is a NCP-Terminate-Ack packet");
		break;
    
	default:
		ppp_DisplayStatus ("   NCP:This is an unknown packet");
		break;
	}  //end of switch	
}


/*
================================================================================
  Function   : ppp_ParseNcpConfig
--------------------------------------------------------------------------------

  Scope      : Parse NCP-CONFIG-XXX from TAF
  Parameters :
  Return     : If all options are the same as expect, return 1; else return 0.
  Others    : 
================================================================================
*/
UINT8 ppp_ParseNcpConfig ()
{
	UINT16 i, len;
	UINT8 isZeroOption = 0;
	UINT8 haveOthers = 0;
	
	//the 3th & 4th byte is the length	
	len = ppp_Data.Buffer[PPP_NCP_HEADER_LEN + 2] * 256 
		+ ppp_Data.Buffer[PPP_NCP_HEADER_LEN + 3];
	len += PPP_NCP_HEADER_LEN;
	CSW_TRACE(PPP_DEBUG_PPP_SWITCH,"  NCP Length is %d \n", len);
	
	for (i = PPP_NCP_HEADER_LEN + 4; i < len; )
	{
#define  showNcpOption(title)	\
		{	ppp_DisplayPacket (title, &ppp_Data.Buffer[i+2], (UINT16) (ppp_Data.Buffer[i+1] - 2));	}
		
		switch (ppp_Data.Buffer[i])
		{
			case PPP_IPCP_CI_ADDRS:
			case PPP_IPCP_CI_ADDR:
				memcpy(&ppp_Data.NcpIP, &ppp_Data.Buffer[i + 2], 4);
				if (ppp_Data.NcpIP  == 0)	
          isZeroOption = 1;
				showNcpOption ("   IP Address:");
				break;
        //modify by wuys 2008-04-23
			case PPP_IPCP_CI_COMPRESSTYPE:
				showNcpOption ("   CompressMethod:");
        memcpy(&g_ncpConfigRej[6], &ppp_Data.Buffer[i + 2], 4);
        haveOthers = 1;
				break;
       #if 0//add by wuys 2008-08-07
      case PPP_IPCP_CI_MS_DNS1:
				showNcpOption ("   Primary DNS:");
        memcpy(&g_ncpConfigRej[12], &ppp_Data.Buffer[i + 2], 4);
        haveOthers = 1;
				break;
     
      case PPP_IPCP_CI_MS_WINS1:
				showNcpOption ("   Primary WINS:");
        memcpy(&g_ncpConfigRej[18], &ppp_Data.Buffer[i + 2], 4);
        haveOthers = 1;
				break;
      case PPP_IPCP_CI_MS_DNS2:
				showNcpOption ("   Secondary DNS:");
        memcpy(&g_ncpConfigRej[24], &ppp_Data.Buffer[i + 2], 4);
        haveOthers = 1;
				break;
      case PPP_IPCP_CI_MS_WINS2:
				showNcpOption ("   econdary WINS:");
        memcpy(&g_ncpConfigRej[30], &ppp_Data.Buffer[i + 2], 4);
        haveOthers = 1;
				break;
      #else
      case PPP_IPCP_CI_MS_DNS1:
				showNcpOption ("   Primary DNS:");
				break;
     
      case PPP_IPCP_CI_MS_WINS1:
				showNcpOption ("   Primary WINS:");
				break;
     
     
      case PPP_IPCP_CI_MS_DNS2:
				showNcpOption ("   Secondary DNS:");
        memcpy(&g_ncpConfigRej[12], &ppp_Data.Buffer[i + 2], 4);
        haveOthers = 1;
				break;
      case PPP_IPCP_CI_MS_WINS2:
				showNcpOption ("   econdary WINS:");
        memcpy(&g_ncpConfigRej[18], &ppp_Data.Buffer[i + 2], 4);
        haveOthers = 1;
				break;
      #endif
			default:
				haveOthers = 1;
				CSW_TRACE(PPP_DEBUG_PPP_SWITCH,"option = %d", ppp_Data.Buffer[i]);
				break;
       //modify end
		}	//end of switch

		// go next, the minimized step is 2.
		if (ppp_Data.Buffer[ i + 1 ] >= 2)		
      i +=  ppp_Data.Buffer[ i + 1];
		else							
      break;
	}
	if(haveOthers == 1)
    return 2;
	if (isZeroOption)	
   return 0;
	else				
   return 1;
}

/*
================================================================================
  Function   : ppp_FetchSMContext
--------------------------------------------------------------------------------

  Scope      : fetch SM context from SM module.
  Parameters :
  Return     :
  Others    : 
================================================================================
*/
void ppp_FetchSMContext()
{
  UINT8 nPdpAddr[ 4 ] = {0,};
//  UINT8 nPdpLen = 4;

	CSW_TRACE(PPP_DEBUG_PPP_SWITCH,"in function ppp_FetchSMContext");

  //CFW_GprsGetPdpAddr(ppp_Gprs_state.PPP_Cid, &nPdpLen, nPdpAddr);

  if (ppp_Data.SmNas   == 0)	
    ppp_Data.SmNas   = 0x0d0c0b0a;
	ppp_Data.SmIP    = *((UINT32*)nPdpAddr);//0x0d0c0b0b;
	
  CSW_TRACE(PPP_DEBUG_PPP_SWITCH,"In Function: ppp_FetchSMContext ppp_Data.SmIP = 0x%x\n",ppp_Data.SmIP);
	if (ppp_Data.SmDNS1  == 0)	ppp_Data.SmDNS1  = 0x0d0c0b0c;
	if (ppp_Data.SmDNS2  == 0)	ppp_Data.SmDNS2  = 0x0d0c0b0d;
	if (ppp_Data.SmWINS1 == 0)	ppp_Data.SmWINS1 = 0x0d0c0b0e;
	if (ppp_Data.SmWINS2 == 0)	ppp_Data.SmWINS2 = 0x0d0c0b0f;
}
extern UINT32 CFW_GetDnsAddr( struct ip_addr *PriDnsSer, struct ip_addr *SecDnsSer );
/*
================================================================================
  Function   : ppp_SendNcpConfig
--------------------------------------------------------------------------------

  Scope      : Send NCP-CONFIG-XXX to TAF
  Parameters :
  Return     :
  Others    : 
================================================================================
*/
void  ppp_SendNcpConfig (UINT8 code)
{
	//fill all options
	if (code == PPP_CPCODES_CONF_REQ)
	{
		//fill all options
		PPP_FILL_NCP_CONFIG_REQ_CODE (code);
		PPP_FILL_NCP_CONFIG_REQ_ID (ppp_Data.Id + 100);
		PPP_FILL_NCP_CONFIG_REQ_IP (&ppp_Data.SmNas);//ppp_Data.SmIP
	//	PPP_FILL_NCP_CONFIG_REQ_IP (&ppp_Data.SmIP);//

		//copy to ppp_Data.Buffer
		memcpy (&ppp_Data.Buffer[0], g_ncpHeader, PPP_NCP_HEADER_LEN);
		memcpy (&ppp_Data.Buffer[PPP_NCP_HEADER_LEN], g_ncpConfigReq, PPP_NCP_CONFIG_REQ_LEN);
		ppp_Data.Position = PPP_NCP_HEADER_LEN + PPP_NCP_CONFIG_REQ_LEN;
	}
	else if (code == PPP_CPCODES_CONF_ACK)
	{
		//just modify the code
		ppp_Data.Buffer[PPP_NCP_HEADER_LEN] = PPP_CPCODES_CONF_ACK;
    
	}else if (code == PPP_CPCODES_CONF_REJ)
	{
    g_ncpConfigRej[0]=PPP_CPCODES_CONF_REJ;
    g_ncpConfigRej[1]=ppp_Data.Id;
   	memcpy (&ppp_Data.Buffer[0], g_ncpHeader, PPP_NCP_HEADER_LEN);
    //modify by wuys 2008-08-07
	//	memcpy (&ppp_Data.Buffer[PPP_NCP_HEADER_LEN], g_ncpConfigRej, 34);
	//ppp_Data.Position = PPP_NCP_HEADER_LEN + 34;
    	memcpy (&ppp_Data.Buffer[PPP_NCP_HEADER_LEN], g_ncpConfigRej, 22);
   
		ppp_Data.Position = PPP_NCP_HEADER_LEN + 22;
    // modify end
    
	}else if (code == PPP_CPCODES_CONF_NAK)
	{
		//add ppp header
		memcpy (&ppp_Data.Buffer[0], g_ncpHeader, PPP_NCP_HEADER_LEN);
		ppp_Data.Position = PPP_NCP_HEADER_LEN;
		
		//add ncp header
		ADD_PPP_BYTE (PPP_CPCODES_CONF_NAK);
		ADD_PPP_BYTE (ppp_Data.Id);
		ADD_PPP_BYTE (0);
		ADD_PPP_BYTE (0);

		//fill all options
		if (ppp_Data.NcpIP == 0)
		{
			ADD_PPP_BYTE (PPP_IPCP_CI_ADDR);
			ADD_PPP_BYTE (6);
			ADD_PPP_4_BYTE (&ppp_Data.SmIP);
      //add by wuys 2008-08-07, for provide DNS to ppp user
      ADD_PPP_BYTE (PPP_IPCP_CI_MS_DNS1);
			ADD_PPP_BYTE (6);
      
      struct ip_addr *PriDnsSer = (struct ip_addr *)CSW_TCPIP_MALLOC(sizeof(struct ip_addr));
      struct ip_addr *SecDnsSer = (struct ip_addr *)CSW_TCPIP_MALLOC(sizeof(struct ip_addr));
      PriDnsSer->addr = 0x00;
      SecDnsSer->addr = 0x00;
      //CFW_GetDnsAddr( PriDnsSer, SecDnsSer );
     
      UINT32 dns_ip = PriDnsSer->addr;
      CSW_TRACE(PPP_DEBUG_PPP_SWITCH,"In Function: ppp_FetchSMContext PriDnsSer->addr = 0x%x\n",PriDnsSer->addr);
      //UINT8 dns_ip[4] = {0xca,0x6a,0xc4,0x73};
			ADD_PPP_4_BYTE (&dns_ip);
      
      UINT8 wins_ip[4] = {0x00,0x00,0x00,0x00};
      ADD_PPP_BYTE (PPP_IPCP_CI_MS_WINS1);
			ADD_PPP_BYTE (6);
      ADD_PPP_4_BYTE (wins_ip);
      //add end
		}

		//at last, fill the length
		ppp_Data.Buffer[PPP_NCP_HEADER_LEN + 2] = GET_HIGH_BYTE((ppp_Data.Position - PPP_NCP_HEADER_LEN));
		ppp_Data.Buffer[PPP_NCP_HEADER_LEN + 3] = GET_LOW_BYTE((ppp_Data.Position - PPP_NCP_HEADER_LEN));	
	}
		
	//encode 
	ppp_EncodeHDLC ();

	SEND_PPP_PACKET;
	
}

/*
================================================================================
  Function   : ppp_SendNcpTermimate
--------------------------------------------------------------------------------

  Scope      : Send NCP-TERN-REQ to TAF
  Parameters :
  Return     :
  Others    : 
================================================================================
*/
void  ppp_SendNcpTermimate (UINT8 code)
{
	if (code == PPP_CPCODES_TERM_REQ)
	{
		//copy to ppp_Data.Buffer
		memcpy (&ppp_Data.Buffer[0], g_ncpHeader, PPP_NCP_HEADER_LEN);	
		ppp_Data.Position = PPP_NCP_HEADER_LEN;
		
		//fill ncp
		ADD_PPP_BYTE (code);	ADD_PPP_BYTE (ppp_Data.Id + 100);
		ADD_PPP_BYTE (0);		ADD_PPP_BYTE (4);
	}
	else
	{
		//just modify from origin ppp packet
		ppp_Data.Buffer[PPP_NCP_HEADER_LEN] = code;
	}
	
	//encode 
	ppp_EncodeHDLC ();

	SEND_PPP_PACKET;
}


/*
================================================================================
  Function   : ppp_ProcessIpPacket
--------------------------------------------------------------------------------

  Scope      : Process an IP packet from TAF
  Parameters :
  Return     :
  Others    : 
================================================================================
*/
void ppp_ProcessIpPacket ()
{
	UINT16 len;
	
	ppp_DisplayStatus ("   IP: This is a IP packet");
	
	//the 3rd & 4th byte is the length
	len = ppp_Data.Buffer[PPP_IP_HEADER_LEN + 2] * 256 + ppp_Data.Buffer[PPP_IP_HEADER_LEN + 3];

	CSW_TRACE(PPP_DEBUG_PPP_SWITCH,"IP: Length is %d", len);
	
	//send it to SNDCP
	ppp_SendIpPacket (&ppp_Data.Buffer[PPP_IP_HEADER_LEN], len);
	ppp_DisplayStatus ("=====================================================");
	ppp_DisplayPacket ("   IP:", &ppp_Data.Buffer[PPP_IP_HEADER_LEN], len);
	ppp_DisplayStatus ("   IP: Send IP packet to SNDCP");
}

/*
================================================================================
  Function   : ppp_SendIpPacket
--------------------------------------------------------------------------------

  Scope      : Send IP packet to SNDCP
  Parameters :
  Return     :
================================================================================
*/

void ppp_SendIpPacket (UINT8* buffer, UINT16 size)
{
    CFW_GPRS_DATA * pGprsData = NULL;
    //UINT16 i = 0;

    pGprsData = CSW_TCPIP_MALLOC(4 + size);\
    if(pGprsData == NULL)
    {
       CSW_TRACE(CFW_GPRS_TS_ID,"ppp_SendIpPacket CSW_TCPIP_MALLOC error \n");
      return;
    }
    SUL_MemCopy8(pGprsData->pData, buffer, size);
    pGprsData->nDataLength = size;
    
    CSW_TRACE(CFW_GPRS_TS_ID,"IPdata begin to SNDCP \n");
#ifdef CFW_MULTI_SIM
#else
    //CFW_GprsSendData(ppp_Gprs_state.PPP_Cid, pGprsData);
 #endif
    if(pGprsData != NULL)
      CSW_TCPIP_FREE(pGprsData);
	
	  CSW_TRACE(PPP_DEBUG_PPP_SWITCH,"PPP send a packet to API module\n");
}

/*
================================================================================
  Function   : ppp_DisplayPacket
--------------------------------------------------------------------------------

  Scope      : Display the packet for logging
  Parameters :
  Return     :
================================================================================
*/
extern UINT32 trace_id;

void ppp_DisplayPacket (char* title, UINT8* buffer, UINT16 size)
{   
//	UINT16 i;
	CSW_TRACE(CFW_GPRS_TS_ID,title);
	SXS_DUMP(trace_id, 0, buffer, size);
}

/*
================================================================================
  Function   : ppp_DisplayStatus
--------------------------------------------------------------------------------

  Scope      : Display the status for logging
  Parameters :
  Return     :
================================================================================
*/
void ppp_DisplayStatus (char* format, ...)
{

	CSW_TRACE(CFW_GPRS_TS_ID,format);
	CSW_TRACE(CFW_GPRS_TS_ID,"\n");
}

/*
================================================================================
  Function   : ppp_InitVars
--------------------------------------------------------------------------------

  Scope     : Init all variances
  Parameters:
  Return    :
  Others    :
================================================================================
*/
void ppp_InitVars()
{
  ppp_Data.HdlcPosition = 0;
	ppp_Data.Position = 0;
	ppp_Data.Status = PPP_STATUS_PPP_NULL;
	ppp_Data.Id = 0;
	ppp_Data.MagicNumber = 0;
	ppp_Data.NcpIP    = 0x00000000;
	ppp_Data.NcpDNS1  = 0x00000000;
	ppp_Data.NcpDNS2  = 0x00000000;
	ppp_Data.NcpWINS1 = 0xffffffff;
	ppp_Data.NcpWINS2 = 0xffffffff;
	ppp_Data.SmNas    = 0x00000000;
	ppp_Data.SmIP     = 0x00000000;
	ppp_Data.SmDNS1   = 0x00000000;
	ppp_Data.SmDNS2   = 0x00000000;
	ppp_Data.SmWINS1  = 0xffffffff;
	ppp_Data.SmWINS2  = 0xffffffff;
}

/*
================================================================================
  Function   : ppp_init
--------------------------------------------------------------------------------

  Scope      : Initialization function for PPP
  Parameters : None
  Return     : None
================================================================================
*/
void ppp_Init (void)
{
    ppp_InitVars ();
    ppp_Gprs_state.PPP_Act = FALSE;
    ppp_Gprs_state.PPP_Att = FALSE;
    ppp_Gprs_state.PPP_Cid = 0;
  	CSW_TRACE(PPP_DEBUG_PPP_SWITCH,"PPP  started. \n");
}

/*
================================================================================
  Function   : ppp_OnReceiveIpPacket
--------------------------------------------------------------------------------

  Scope      : Receive an IP packet from SNDCP
  Parameters :
  Return     :
  Others    : 
================================================================================
*/
void ppp_OnReceiveIpPacket(UINT8* packet, UINT16 size)
{
	//ppp received an IP packet from SNDCP
	ppp_DisplayStatus ("=====================================================");
	ppp_DisplayPacket ("SNDCP:", packet, size);
	ppp_DisplayStatus ("IP: This is an IP packet from SNDCP");
	
	//check its size
	if (size > MAX_PPP_PACKET_SIZE)
	{
		ppp_DisplayStatus ("   IP: This IP packet is too large, discard it.");
		return;
	}

	//make a ppp packet
	ppp_Data.Position = PPP_IP_HEADER_LEN + size;
	memcpy (ppp_Data.Buffer, g_ipHeader, PPP_IP_HEADER_LEN);
	memcpy (&ppp_Data.Buffer[PPP_IP_HEADER_LEN], packet, size);
  
	
	//encode 
	ppp_EncodeHDLC();

	SEND_PPP_PACKET;

	ppp_DisplayStatus ("   ppp: this is an IP packet to serial");
}

//END_C


//add by wuys 2008-04-24 

/*
================================================================================
  Function   : ppp_SendCcpConfig
--------------------------------------------------------------------------------

  Scope      : send 
  Parameters :
  Return     :
  Others    : 
================================================================================
*/
/*
void  ppp_SendCcpConfig (UINT8 code)
{
  if (code == PPP_CPCODES_CONF_REJ)
	{
		ppp_Data.Buffer[PPP_CCP_HEADER_LEN] = PPP_CPCODES_CONF_REJ;
	}
	ppp_EncodeHDLC ();
  
	SEND_PPP_PACKET;
}
*/
//add by wuys 2008-04-28
/*
================================================================================
  Function   : ppp_ProcessPapPacket
--------------------------------------------------------------------------------

  Scope      : process PAP authentication packet
  Parameters :
  Return     :
  Others    : 
================================================================================
*/
void ppp_ProcessPapPacket (void)
{
	//UINT8 isGoodOption;
	ppp_DisplayStatus ("   in function : ProcessPAPPacket");	
	ppp_Data.Id = ppp_Data.Buffer[PPP_PAP_HEADER_LEN + 1];

  UINT8 user_id[20] = {0,};
  UINT8 pwd[20] = {0,};
  UINT8 uid_len, pwd_len;
 // INT8 len;
	//first byte is the code
	switch (ppp_Data.Buffer[PPP_PAP_HEADER_LEN])
	{
	case PPP_CPCODES_CONF_REQ:
   
    SXS_DUMP(trace_id,0,ppp_Data.Buffer,10);
    if(ppp_Data.Buffer[PPP_PAP_HEADER_LEN + 3] > 4)
		{
  		uid_len =  ppp_Data.Buffer[PPP_PAP_HEADER_LEN + 4] ;
      ppp_DisplayStatus ("   PAP:This is a PAP REQ packet: uid_len: 0x%x",uid_len);
      // check and save the UID
      if((uid_len < 19)&&(uid_len > 0))
      {
  		   memcpy(user_id, ppp_Data.Buffer + PPP_PAP_HEADER_LEN + 5, uid_len);
         SXS_DUMP(trace_id,0,user_id,uid_len+2);
         SXS_TRACE(trace_id,"   PAP:This is a PAP REQ packet: UID: 0x%x",uid_len);
      }
      if(ppp_Data.Buffer[PPP_PAP_HEADER_LEN + 3] > (4 + 1 + uid_len))
      {
        pwd_len = ppp_Data.Buffer[PPP_PAP_HEADER_LEN + 4 + uid_len + 1] ;
        ppp_DisplayStatus ("   PAP:This is a PAP REQ packet: pwd len: 0x%x",pwd_len);
       // check and save the Password
        if((pwd_len < 19)&&(pwd_len > 0))
        {
          memcpy(pwd, ppp_Data.Buffer + PPP_PAP_HEADER_LEN + 5 + uid_len + 1, pwd_len );
          SXS_DUMP(trace_id,0,user_id,uid_len+2);
        }
      }
		}
    
    ppp_SendPapConfig (PPP_CPCODES_CONF_ACK);
    break;
	case PPP_CPCODES_CONF_ACK:
	case PPP_CPCODES_CONF_NAK:
    //TODO
		break;
	default:
		ppp_DisplayStatus ("   PAP:This is an unknown packet");
		break;
	}  //end of switch	
}
/*
================================================================================
  Function   : ppp_ProcessPapPacket
--------------------------------------------------------------------------------

  Scope      : process PAP authentication packet
  Parameters :
  Return     :
  Others    : 
================================================================================
*/
void  ppp_SendPapConfig (UINT8 code)
{
  if (code == PPP_CPCODES_CONF_ACK)
	{
  	g_papAck[0]=PPP_CPCODES_CONF_ACK;
    g_papAck[1]=ppp_Data.Id;
    memcpy (&ppp_Data.Buffer[0], g_papHeader, PPP_PAP_HEADER_LEN);
  	memcpy (&ppp_Data.Buffer[PPP_PAP_HEADER_LEN], g_papAck, 9);
  	ppp_Data.Position = PPP_PAP_HEADER_LEN + 9;
  }else{
    //TODO
  }
  ppp_EncodeHDLC ();

	
	SEND_PPP_PACKET;
	
}

BOOL uart_PsSendData(UINT8 *pData, UINT16 uDataSize)
{
    return TRUE;
}

//add end
#endif
