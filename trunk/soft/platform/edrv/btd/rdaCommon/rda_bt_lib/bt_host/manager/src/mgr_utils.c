/*
 * MODULE NAME:    mgr_utils.c
 * DESCRIPTION:    Host Manager Address/String Handling Utility Functions
 * AUTHOR:         Martin Mellody
 * DATE:           10 March 2000
 *
 * SOURCE CONTROL: $Id: mgr_utils.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */

#include "host_config.h"

#include "host_types.h"
#include "mgr_core.h"


/************ API FUNCTIONS *************/


/*************************************************************************************************
 * FUNCTION :- MGR_PrintBDAddress
 *
 * DESCRIPTION :- Format a 6-byte bluetooth t_addr as a hex string
 *                Output format: "AA:BB:CC:DD:EE:FF" where 0xAA is the LSB and 0xFF is the MSB  
 ************************************************************************************************/
APIDECL1 t_api APIDECL2 MGR_PrintBDAddress(t_bdaddr address, char *outBuffer) {
    int i;

    for (i=5; i>=0; i--) {
        /* upper nybble */
        outBuffer[15-i*3]=(((address.bytes[i])&0xF0)>>4)+48;
        outBuffer[15-i*3]+=outBuffer[15-i*3]>57?7:0;
        /* lower nybble */
        outBuffer[15+1-i*3]=((address.bytes[i])&0x0F)+48;
        outBuffer[15+1-i*3]+=outBuffer[15+1-i*3]>57?7:0;
        /* delimiter */
        outBuffer[15+2-i*3]=':';
    }
    outBuffer[17]='\0';

    return BT_NOERROR;
}


/*************************************************************************************************
 * FUNCTION :- MGR_ParseHexBDAddress
 *
 * DESCRIPTION :- Read an ASCII device address and convert to a t_bdaddr.
 *                Input charaster case, leading whitespece and leading "0x" are ignored
 *                Delimiters between input bytes (e.g. AA:BB-CC:DD EE:FF) are handled (ignored)
 ************************************************************************************************/
APIDECL1 t_api APIDECL2 MGR_ParseHexBDAddress(char *inString, t_bdaddr *outAddress) {

    char c;
    int inOffset=0, outOffset=6, lowerNibble=1, seperator=0;

    if (inString==NULL || inString[0]=='\0')
        return BT_INVALIDPARAM;     

    while (inString[inOffset] && outOffset) {

		if (inString[inOffset]=='0' && (inString[inOffset+1]&0x5F)=='X')
			inOffset+=2;                 
	
        c=inString[inOffset++];

        if (c==':' || c=='-' || c==' ' || c==' ' || c==',') {
			seperator=1;
			if (!lowerNibble) {
				outAddress->bytes[outOffset-1]>>=4;
				outOffset--;
				lowerNibble^=1;          
			}
            continue;                   
		}

        if (c>0x5F)
            c&=0x5F;                    

        if (c<0x30 || (c>0x39 && c<0x41) || c>0x46)
            return BT_INVALIDPARAM;     

        c-=0x30;
        if (c>9)
            c-=7;

        if (lowerNibble) {
            outAddress->bytes[outOffset-1]=c<<4;
        } else {
            outAddress->bytes[outOffset-1]|=c;            
            outOffset--;
        }
        lowerNibble^=1;          
    };

	if (!lowerNibble && seperator) {
		outAddress->bytes[outOffset-1]>>=4;
		outOffset--;
        lowerNibble=1;          
	};

	
    if (outOffset!=0 || lowerNibble!=1)
        return BT_INVALIDPARAM;  

	if ((inString[inOffset]>0x2F && inString[inOffset]<0x40) || ((inString[inOffset]&0x5F)>0x40 && (inString[inOffset]&0x5F)<0x47)) 
        return BT_INVALIDPARAM;  



    return BT_NOERROR;
}




/************ NON-API FUNCTIONS *************/


/*************************************************************************************************
 * FUNCTION :- prh_mgr_getLocalBDAddress
 *
 * DESCRIPTION :- Return the local bluetooth device address as read by the manager
 ************************************************************************************************/
t_bdaddr prh_mgr_getLocalBDAddress(void) {
    return prh_mgr_LocalBDADDR;
}

/*************************************************************************************************
 * FUNCTION :- prh_mgr_copyBDAddress
 *
 * DESCRIPTION :- Copy a t_bdaddr structure
 ************************************************************************************************/
void prh_mgr_copyBDAddress(t_bdaddr *dest, t_bdaddr *src) {
    pMemcpy(dest->bytes,src->bytes,sizeof(src->bytes));
}


/*************************************************************************************************
 * FUNCTION :- prh_mgr_hci2bt_error
 *
 * DESCRIPTION :- Map a HC event status code to a host api error code (from host_error.h).
 *                Not all possible HC errors have mappings - low-level applications must
 *                monitor the event stream themselves, and high-level applications are not
 *                generally concerned with the particular error code once any error has occurred.
 ************************************************************************************************/
t_api prh_mgr_hci2bt_error(u_int8 eventStatus) {
    static t_api prh_mgr_hci2bt_error_table[0x2A] = 
        {
         /* Host Error Code    --    Host Controller Error Code  */
	        BT_NOERROR,				/* HCI_errNOERROR			0x00 */
	        BT_UNSUPPORTED,			/* HCI_errUNKNOWNCOMMAND	0x01 */
	        BT_DISALLOWED,			/* HCI_errNOCONNECTION		0x02 */
	        BT_HWERROR,			 	/* HCI_errHWFAILURE			0x03 */
	        BT_TIMEOUT,				/* HCI_errPAGETIMEOUT		0x04 */
	        BT_AUTHENTICATIONFAIL,	/* HCI_errAUTHFAILURE		0x05 */
	        BT_AUTHENTICATIONFAIL,	/* HCI_errKEYMISSING		0x06 */
	        BT_NORESOURCES,			/* HCI_errMEMORYFULL		0x07 */
	        BT_TIMEOUT,				/* HCI_errCONNTIMEOUT		0x08 */
	        BT_NORESOURCES,			/* HCI_errMAXCONNECTIONS	0x09 */
	        BT_NORESOURCES,			/* HCI_errMAXSCOS			0x0A */
	        BT_NORESOURCES,			/* HCI_errACLEXISTS			0x0B */
	        BT_DISALLOWED,			/* HCI_errCMDDISALLOWED		0x0C */
	        BT_NORESOURCES,			/* HCI_errREJRESOURCES		0x0D */
	        BT_SECFAIL,				/* HCI_errREJSECURITY		0x0E */
	        BT_UNSUPPORTED,			/* HCI_errREJPERSONALDEV	0x0F */
	        BT_TIMEOUT,				/* HCI_errHOSTTIMEOUT		0x10 */
	        BT_UNSUPPORTED,			/* HCI_errUNSUPPORTED		0x11 */
	        BT_INVALIDPARAM,		/* HCI_errINVALIDPARAM		0x12 */
	        BT_DISCONNECTED,		/* HCI_errTERMUSER			0x13 */
	        BT_DISCONNECTED,		/* HCI_errTERMRESOURCES		0x14 */
	        BT_DISCONNECTED,		/* HCI_errTERMPOWEROFF		0x15 */
	        BT_DISCONNECTED,		/* HCI_errLOCALTERM			0x16 */
	        BT_DISALLOWED,			/* HCI_errREPEATED			0x17 */
	        BT_DISALLOWED,			/* HCI_errNOPARING			0x18 */
	        BT_UNKNOWNERROR,		/* HCI_errUNKNOWNLMPPDU		0x19 */
	        BT_UNKNOWNERROR,		/* HCI_errUNSUPPORTEDREMOTE 0x1A */
	        BT_NORESOURCES,			/* HCI_errSCOOFFSETREJ		0x1B */
	        BT_NORESOURCES,			/* HCI_errSCOINTERVALREJ	0x1C */
	        BT_NORESOURCES,			/* HCI_errSCOMODEREJ		0x1D */
	        BT_UNKNOWNERROR,		/* HCI_errLMPINVALIDPARAM	0x1E */
	        BT_UNKNOWNERROR,		/* HCI_errUNSPECERROR		0x1F */
	        BT_INVALIDPARAM,		/* HCI_errLMPUNSUPPPARAM	0x20 */
	        BT_UNSUPPORTED,			/* HCI_errNOROLECHANGE		0x21 */
	        BT_TIMEOUT,				/* HCI_errLMPTIMEOUT		0x22 */
	        BT_UNKNOWNERROR,		/* HCI_errLMPTRANSCOLLISION 0x23 */
	        BT_DISALLOWED,			/* HCI_errLMPPDUDISALLOWED	0x24 */
	        BT_ENCRYPTFAIL,			/* HCI_errBADCRYPTMODE		0x25 */
	        BT_DISALLOWED,			/* HCI_errUNITKEYUSED		0x26 */
	        BT_UNSUPPORTED,			/* HCI_errQOSUNSUPPORTED	0x27 */
	        BT_SECFAIL,				/* HCI_errINSTANTPASSED	    0x28 */
	        BT_DISALLOWED			/* HCI_errNOUNITPAIRING	    0x29 */
        };

    if (eventStatus<0x2A)
        return prh_mgr_hci2bt_error_table[eventStatus];    
    else 
        return BT_UNKNOWNERROR;
}


#if 1 /* the remaining functions are for formatting debug output only */

/*************************************************************************************************
 * FUNCTION :- prh_mgr_printBDAddress
 *
 * DESCRIPTION :- Format a 6-byte bluetooth t_addr as a hex string
 *                Output format: "AA:BB:CC:DD:EE:FF" where 0xAA is the LSB and 0xFF is the MSB  
 *                The return buffer is static, so a subsequent or threaded call will overwrite
 ************************************************************************************************/
u_int8 *prh_mgr_printBDAddress(t_bdaddr address) {

    static u_int8 buffer[18];

    MGR_PrintBDAddress(address,(char *)buffer);
    return buffer;
}




/*************************************************************************************************
 * FUNCTION :- prh_mgr_readHexBDAddress
 *
 * DESCRIPTION :- Read an ASCII device address and convert to a t_bdaddr.
 *                Input charaster case, leading whitespece and leading "0x" are ignored
 *                Delimiters between input bytes (e.g. AA:BB-CC:DD EE:FF) are handled (ignored)
 *                The return buffer is static, so a subsequent or threaded call will overwrite
 ************************************************************************************************/
t_bdaddr prh_mgr_readHexBDAddress(char *hexnumber) {
    static t_bdaddr address={{0}};

    MGR_ParseHexBDAddress(hexnumber,&address);    
    return address;
}



/*************************************************************************************************
 * FUNCTION :- prh_mgr_printLargeNumber
 *
 * DESCRIPTION :- Format an array of bytes to a hex string for printing (e.g. link keys, pin codes)
 *                Output format:"0x00112233AABBCCDDEEFF" where 0xAA is the LSB and 0xFF is the MSB
 *                The array must be stored LSB first in memory.
 *                The return buffer is static, so a subsequent or threaded call will overwrite.
 ************************************************************************************************/
u_int8 * prh_mgr_printLargeNumber(u_int8 *number, u_int8 length) {

#define MGR_PrintLargeNumber_MAXLEN  64

    static u_int8 buffer[MGR_PrintLargeNumber_MAXLEN*2+3]="0x";
    int i;

    if (length>MGR_PrintLargeNumber_MAXLEN)
		return (u_int8 *)"0x<TooBig>";

#ifdef WIN32
    for (i=0; i<length; i++) 
    {
        /* upper nibble */
        buffer[i*2]=(((number[i])&0xF0)>>4)+0x30;
        buffer[i*2]+=(buffer[i*2]>57)?7:0;
        
        /* lower nibble */
        buffer[i*2+1]=((number[i])&0x0F)+48;
        buffer[i*2+1]+=(buffer[i*2+1]>57)?7:0;
    }
    
    buffer[length*2+2]='\0';
    
#else

    for (i=length-1; i>=0; i--) 
    {
        /* upper nybble */
        buffer[length*2-i*2]=(((number[i])&0xF0)>>4)+48;
        buffer[length*2-i*2]+=buffer[length*2-i*2]>57?7:0;
        /* lower nybble */
        buffer[length*2+1-i*2]=((number[i])&0x0F)+48;
        buffer[length*2+1-i*2]+=buffer[length*2+1-i*2]>57?7:0;
    }
    buffer[length*2+2]='\0';
#endif

    return buffer;
}


#endif /* pDEBUG */
