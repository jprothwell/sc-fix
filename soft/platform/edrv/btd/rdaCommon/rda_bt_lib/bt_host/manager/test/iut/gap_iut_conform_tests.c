/*
 * DESCRIPTION:    Manager Test Code
 * AUTHOR:         Thomas Howley
 *
 * SOURCE CONTROL: $Id: gap_iut_conform_tests.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */

#include "bt.h"

#include <stdio.h>
#include <string.h>

#define TESTER_BDADDR "0x00D0B7031728"

#define PRH_TEST_PSM 1013
#define PRH_TEST_MTUSIZE 70
#define L2CAP_HDR_LEN 9

/* For testing the Inquiry Procedure */
#define INQUIRYTEST_LEN        10
#define INQUIRYTEST_MAXRESULTS 1 
#define BT_GIAC_LAP  0x9E8B33
#define BT_LIAC_LAP  0x9E8B00

#define BD_ADDR_CMP(x,y) !memcmp(x.bytes, y.bytes, 6)

/* Test Utility Functions */
int prh_test_setConnectableMode(u_int8 mode);
int prh_test_setDiscoverableMode(u_int8 mode);
int prh_test_setPairingMode(u_int8 mode);
int prh_test_setSecurityMode(u_int8 mode);
void prh_test_parsePINResponse(char *hexnumber, u_int8 **pinCode, u_int8 *pinLen);
void prh_test_setupL2CAP();
void prh_test_l2cap_channelEventHandler(u_int8 eventType, t_L2_ControlCallbackArgs *pArgs);
void prh_test_recvDataHandler(u_int16 cid,t_DataBuf * p_buf);
void prh_test_SendL2CAPData(int cid, u_int8 *data, int lendata);

/* Callback Functions */
void iut_setDiscoverableMode_callback(int status);
void iut_setConnectableMode_callback(int status); 
void iut_setSecurityMode_callback(int status);
void iut_pinRequest_callback(t_bdaddr bdAddress);
void iut_inquiry_Callback(t_MGR_DeviceEntry *newDevice, u_int8 status);
void iut_bonding_Callback(t_bdaddr address, int status);


#if EBMK_TEST == 1
u_int8 setDiscoverMode = MGR_NONDISCOVERABLE;
#endif



void (*prh_test_l2capConfigConfCallback)(t_L2_ControlCallbackArgs *pArgs);
t_L2_PsmCallbacks l2cap_cbs;

int prh_test_gap_iut_TP_MOD_NDIS_BV_01_C(void)
{
  prh_test_setDiscoverableMode(MGR_NONDISCOVERABLE);
}

int prh_test_gap_iut_TP_MOD_GDIS_BV_01_C(void)
{
#if EBMK_TEST == 1
  if(BT_NOERROR == prh_test_setConnectableMode(MGR_CONNECTABLE))
    prh_test_setDiscoverableMode(MGR_GENERALDISCOVERY);
  else
    setDiscoverMode = MGR_GENERALDISCOVERY;
#else
  prh_test_setDiscoverableMode(MGR_GENERALDISCOVERY);
#endif
}

int prh_test_gap_iut_TP_MOD_NCON_BV_O1_C(void)
{
  prh_test_setConnectableMode(MGR_NONCONNECTABLE);
}


int prh_test_gap_iut_TP_MOD_CON_BV_01_C(void)
{
  prh_test_setConnectableMode(MGR_CONNECTABLE);
}


int prh_test_gap_iut_TP_MOD_NPAIR_BV_01_C(void)
{
  prh_test_setPairingMode(MGR_NONPAIRABLE);
}


int prh_test_gap_iut_TP_SEC_AUT_BV_01_C(void)
{
  t_bdaddr testerBdAddr;

  MGR_ParseHexBDAddress(TESTER_BDADDR, &testerBdAddr); 

  /* Preamble for Generic Authentication */
  prh_test_setConnectableMode(MGR_CONNECTABLE);
  prh_test_setSecurityMode(MGR_LINKLEVEL);

  /* Register PIN Handler to handle PIN requests for remote TESTER to demonstrate App entry of PIN */
  MGR_SetRemoteDevicePIN(testerBdAddr, MGR_USERREQPIN, 0, 0);
  MGR_RegisterPINHandler(iut_pinRequest_callback);
  
}



int prh_test_gap_iut_TP_SEC_AUT_BV_02_C(void)
{
 t_bdaddr testerBdAddr;

  MGR_ParseHexBDAddress(TESTER_BDADDR, &testerBdAddr); 

  prh_test_setConnectableMode(MGR_CONNECTABLE);
  prh_test_setPairingMode(MGR_PAIRABLE);

  /* Register PIN Handler to handle PIN requests for remote TESTER to demonstrate App entry of PIN */
  MGR_SetRemoteDevicePIN(testerBdAddr, MGR_USERREQPIN, 0, 0);
  MGR_RegisterPINHandler(iut_pinRequest_callback);
  

}



int prh_test_gap_iut_TP_SEC_SEM_BV_01_C(void)
{
  prh_test_setConnectableMode(MGR_CONNECTABLE);
  prh_test_setSecurityMode(MGR_NOSECURITY);

  /* setup L2CAP server */
  prh_test_setupL2CAP();

}


int prh_test_gap_iut_TP_SEC_SEM_BV_02_C(void)
{
  prh_test_setConnectableMode(MGR_CONNECTABLE);
  prh_test_setSecurityMode(MGR_SERVICELEVEL);

  /* setup L2CAP server */
  prh_test_setupL2CAP();
  /* register security required */
  MGR_RegisterAppSecurityLevel("GAP_IUT_APPA", MGR_L2CAP, PRH_TEST_PSM, MGR_INAUTHENTICATION);
}


int prh_test_gap_iut_TP_SEC_SEM_BV_03_C(void)
{
  prh_test_setConnectableMode(MGR_CONNECTABLE);
  prh_test_setSecurityMode(MGR_LINKLEVEL);

  /* setup L2CAP server */
  prh_test_setupL2CAP();
}



int prh_test_gap_iut_TP_IDLE_GIN_BV_01_C(void)
{
  MGR_Inquiry(INQUIRYTEST_LEN, BT_GIAC_LAP, INQUIRYTEST_MAXRESULTS, iut_inquiry_Callback); 
}

int prh_test_gap_iut_TP_IDLE_LIN_BV_01_C(void)
{
  MGR_Inquiry(INQUIRYTEST_LEN, BT_LIAC_LAP, INQUIRYTEST_MAXRESULTS, iut_inquiry_Callback); 
}


int prh_test_gap_iut_TP_IDLE_BON_BV_01_C(void)
{
  t_bdaddr testerBdAddr;

  MGR_ParseHexBDAddress(TESTER_BDADDR, &testerBdAddr);

  /* Register PIN Handler to handle PIN requests for remote TESTER to demonstrate App entry of PIN */
  MGR_SetRemoteDevicePIN(testerBdAddr, MGR_USERREQPIN, 0, 0);
  MGR_RegisterPINHandler(iut_pinRequest_callback);

  /* Initiate the Bonding procedure */
  MGR_InitBonding(testerBdAddr, iut_bonding_Callback);


}

int prh_test_gap_iut_TP_EST_LIE_BV_02_C(void)
{
  t_bdaddr testerBdAddr;

  MGR_ParseHexBDAddress(TESTER_BDADDR, &testerBdAddr); 
  
  prh_test_setupL2CAP();
  L2_Connect(testerBdAddr, PRH_TEST_PSM);
  
}


/* Test Utility Functions */

int prh_test_setDiscoverableMode(u_int8 mode)
{
  int status;

  status = MGR_SetDiscoverableMode(mode, iut_setDiscoverableMode_callback);
  if(BT_NOERROR == status)
    fprintf(stderr, "GAP IUT TESTS: Discoverable Mode set\n");
  else if(BT_PENDING != status && BT_NOERROR != status)
    fprintf(stderr, "GAP IUT TESTS: ERROR setting Discoverable Mode\n");

  return status;

}


int prh_test_setConnectableMode(u_int8 mode)
{
  int status;

  status = MGR_SetConnectableMode(mode, iut_setConnectableMode_callback);
  if(BT_NOERROR == status)
    fprintf(stderr, "GAP IUT TESTS: Connectable Mode set\n");
  else if(BT_PENDING != status && BT_NOERROR != status)
    fprintf(stderr, "GAP IUT TESTS: ERROR setting Connectable Mode\n");

  return status;
}


int prh_test_setPairingMode(u_int8 mode)
{
  int status;
  
  if(BT_NOERROR == MGR_SetPairingMode(mode))
    fprintf(stderr, "GAP IUT TESTS: Pairable Mode set\n");
  else
    fprintf(stderr, "GAP IUT TESTS: ERROR setting Pairable Mode\n");

  return status;
}

int prh_test_setSecurityMode(u_int8 mode)
{
  int status;

  status = MGR_SetSecurityMode(mode, iut_setSecurityMode_callback);
  if(BT_NOERROR == status)
    fprintf(stderr, "GAP IUT TESTS: Security Mode set\n");
  else if(BT_PENDING != status && BT_NOERROR != status)
    fprintf(stderr, "GAP IUT TESTS: ERROR setting Security Mode\n");

  return status;
}

void prh_test_parsePINResponse(char *hexnumber, u_int8 **pinCode, u_int8 *pinLen)
{
  signed int i;
  u_int8 nibble;
  u_int8 seperator;
  int pinLength;
  
  /* First three characters used for PIN length */
  sscanf(hexnumber, "%d", &pinLength);
  *pinLen = (u_int8)pinLength;
  while(*hexnumber != ' ')
    hexnumber++;
  hexnumber++;

  if ((hexnumber[1]&0x5F)=='X') /* string had "0x" or "0X" at the start */
    hexnumber+=2;
  
  if (hexnumber[2]==':') /* the bytes are colon-seperated */
    seperator=1;
  else
    seperator=0;
  
  *pinCode = (u_int8*)malloc(pinLength);
  for (i=pinLength-1; i>=0; i--) {      
    nibble=hexnumber[(pinLength-1-i)*2+(pinLength-1-i)*seperator];
    nibble-=48;
    if (nibble>9)
      nibble-=7;
    if (nibble>15)
      nibble-=32;
    (*pinCode)[i]=nibble<<4;
    nibble=hexnumber[(pinLength-1-i)*2+1+(pinLength-1-i)*seperator];
    nibble-=48;
    if (nibble>9)
      nibble-=7;
    if (nibble>15)
      nibble-=32;
    (*pinCode)[i]|=nibble;
  }
  
}


void prh_test_setupL2CAP()
{ 

  l2cap_cbs.connectionIndication = prh_test_l2cap_channelEventHandler;
  l2cap_cbs.configureIndication = prh_test_l2cap_channelEventHandler;
  l2cap_cbs.disconnectionIndication =prh_test_l2cap_channelEventHandler;
  l2cap_cbs.qosviolationIndication = NULL;
  l2cap_cbs.connectionConfirmation = prh_test_l2cap_channelEventHandler;
  l2cap_cbs.configureConfirmation = prh_test_l2cap_channelEventHandler;
  l2cap_cbs.disconnectionConfirmation = prh_test_l2cap_channelEventHandler;
  l2cap_cbs.dataIndication = prh_test_recvDataHandler;
  
  L2_RegisterPSM(PRH_TEST_PSM, &l2cap_cbs);

} 



void prh_test_l2cap_channelEventHandler(u_int8 eventType, t_L2_ControlCallbackArgs *pArgs)
{  
  u_int16 linkSupTimeout = 0; 
  u_int16 flushTimeout = 0;

  switch(eventType)
    {
    case L2CAP_evCONNECTIND:
      fprintf(stderr, "GAP IUT TESTS - Connection Indication Received\n");
      L2_ConnectResponse(pArgs->bdaddr, pArgs->cid, 0x00 , pArgs->status);
      L2_Configure(pArgs->cid, PRH_TEST_MTUSIZE, flushTimeout, linkSupTimeout, 0x00); 
      break;

    case L2CAP_evCONFIGUREIND:
      fprintf(stderr, "GAP IUT TESTS - Config Indication Received\n");
      L2_ConfigureResponse(pArgs->cid, PRH_TEST_MTUSIZE, 0x00, 0x00);
      break;
      
    case L2CAP_evDISCONNECTIND:
      fprintf(stderr, "GAP IUT TESTS - Disconnection Indication Received\n");
      break;

    case L2CAP_evCONNECTCONF:
      fprintf(stderr, "GAP IUT TESTS - Connection Confirmation Received\n");
      L2_Configure(pArgs->cid, PRH_TEST_MTUSIZE, flushTimeout, linkSupTimeout, 0x00); 
      break;

    case L2CAP_evCONFIGURECONF:
      fprintf(stderr, "GAP IUT TESTS - Configure Confirm Received\n");
      prh_test_SendL2CAPData(pArgs->cid, "THIS IS SAMPLE DATA", 19);
        
      break;

    case L2CAP_evDISCONNECTCONF:
      fprintf(stderr, "GAP IUT TESTS - Disconnect Confirm Received\n");
      break;

    default:
      break;
    }

}


void prh_test_recvDataHandler(u_int16 cid, t_DataBuf * p_buf)
{
  int i;

  fprintf(stderr, "GAP TESTER - Received Data:");
  for(i=0; i<p_buf->dataLen; i++)
    fprintf(stderr, "%c", p_buf->data[i]);
  fprintf(stderr, "\n");

}

void prh_test_SendL2CAPData(int cid, u_int8 *data, int lendata)
{
  t_DataBuf *h_buff;
  int i;

  h_buff = (t_DataBuf *)host_buf_alloc(lendata + L2CAP_HDR_LEN);
  host_buf_reserve_header(h_buff, L2CAP_HDR_LEN);

  for(i=0; i<lendata; i++)
    h_buff->data[i] = data[i];      

  h_buff->dataLen = lendata;
  L2_Write(cid, h_buff);

}




/* Callback Functions */

void iut_setDiscoverableMode_callback(int status)
{
  if(BT_NOERROR == status)
    fprintf(stderr, "GAP IUT TESTS: Discoverable Mode set\n");
  else
    fprintf(stderr, "GAP IUT TESTS: ERROR setting Discoverable Mode\n");
}

void iut_setConnectableMode_callback(int status)
{
  if(BT_NOERROR == status)
    fprintf(stderr, "GAP IUT TESTS: Connectable Mode set\n");
  else
    fprintf(stderr, "GAP IUT TESTS: ERROR setting Connectable Mode\n");



#if EBMK_TEST == 1
  if(MGR_NONDISCOVERABLE != setDiscoverMode)
    {
      prh_test_setDiscoverableMode(setDiscoverMode);
      setDiscoverMode = MGR_NONDISCOVERABLE;      
    }
#endif

}

void iut_setSecurityMode_callback(int status)
{
  if(BT_NOERROR == status)
    fprintf(stderr, "GAP IUT TESTS: Security Mode set\n");
  else
    fprintf(stderr, "GAP IUT TESTS: ERROR setting Security Mode\n");
}



void iut_pinRequest_callback(t_bdaddr bdAddress)
{
  int i;
  int x;
  char strPINInfo[] = "3 0x112233";
  char buffer[20];
  u_int8 pinSupplied = 0;
  u_int8 pinLen;
  u_int8 *pinCode;

  MGR_PrintBDAddress(bdAddress, buffer);
  fprintf(stderr, "GAP IUT TESTS: PIN REQUEST for BD_ADDR: %s \n", buffer);
  fprintf(stderr, "Enter PIN Info: ");


  /* gets(strPINInfo);  */
  prh_test_parsePINResponse(strPINInfo, &pinCode, &pinLen); 
  pinSupplied = 1;  
  if( BT_NOERROR != MGR_PINResponse(bdAddress, pinSupplied, pinCode, pinLen) )
    fprintf(stderr, "GAP IUT TESTS -  ERROR: Responding to PIN Request\n");
  
}

void iut_inquiry_Callback(t_MGR_DeviceEntry *newDevice, u_int8 status)
{
  static u_int8 testerFound = 0;
  t_bdaddr testerBdAddr;

  MGR_ParseHexBDAddress(TESTER_BDADDR, &testerBdAddr);
  if(newDevice)
    {
      if(BD_ADDR_CMP(newDevice->address, testerBdAddr))
        testerFound = 1;
    }
  else
    {
      if(testerFound)
        fprintf(stderr, "GAP TEST PASSED: - TESTER Device found\n");
      else
        fprintf(stderr, "GAP TEST FAILED: - TESTER Device not found\n");
    }
}

void iut_bonding_Callback(t_bdaddr address, int status)
{
 t_bdaddr testerBdAddr;

 MGR_ParseHexBDAddress(TESTER_BDADDR, &testerBdAddr); 
 if(BD_ADDR_CMP(address, testerBdAddr) && BT_NOERROR == status)
   fprintf(stderr,"TEST PASSED - bonding completed\n");
 else
   fprintf(stderr,"TEST FAILED - bonding failed with status = %d\n", status);
}



int (*gap_iut_conform_test[])(void) = 
{
  prh_test_gap_iut_TP_MOD_NDIS_BV_01_C, /* TP/MOD/NDIS/BV-01-C */
  prh_test_gap_iut_TP_MOD_GDIS_BV_01_C, /* TP/MOD/GDIS/BV-01-C */
  prh_test_gap_iut_TP_MOD_NCON_BV_O1_C, /* TP/MOD/NCON/BV-O1-C */
  prh_test_gap_iut_TP_MOD_CON_BV_01_C, /* TP/MOD/CON/BV-01-C */
  prh_test_gap_iut_TP_MOD_NPAIR_BV_01_C,   /* TP/MOD/NPAIR/BV-01-C */
  prh_test_gap_iut_TP_SEC_AUT_BV_01_C,   /* TP/SEC/AUT/BV-01-C */
  prh_test_gap_iut_TP_SEC_AUT_BV_02_C,    /* TP/SEC/AUT/BV-02-C */
  prh_test_gap_iut_TP_SEC_SEM_BV_01_C,     /* TP/SEC/SEM/BV-01-C */
  prh_test_gap_iut_TP_SEC_SEM_BV_02_C,     /* TP/SEC/SEM/BV-02-C */
  prh_test_gap_iut_TP_SEC_SEM_BV_03_C,     /* TP/SEC/SEM/BV-02-C */
  prh_test_gap_iut_TP_IDLE_GIN_BV_01_C,     /* TP/IDLE/GIN/BV-01-C */
  prh_test_gap_iut_TP_IDLE_LIN_BV_01_C,     /* TP/IDLE/LIN/BV-01-C */
  prh_test_gap_iut_TP_IDLE_BON_BV_01_C,    /* TP/IDLE/BON/BV-01-C */
  prh_test_gap_iut_TP_EST_LIE_BV_02_C   /* TP/EST/LIE/BV-02-C */
};
