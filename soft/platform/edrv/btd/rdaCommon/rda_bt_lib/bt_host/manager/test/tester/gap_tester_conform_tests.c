/*
 * DESCRIPTION:    Manager Test Code
 * AUTHOR:         Thomas Howley
 *
 * SOURCE CONTROL: $Id: gap_tester_conform_tests.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */
#include "tester_gap.h"
#include "mgr_core.h"
#include "mgr_utils.h"
#include "host_types.h"

#include <stdio.h>


#define PRH_TEST_PSM 1013
#define PRH_TEST_MTUSIZE 70
#define L2CAP_HDR_LEN 9

pSem_t acl_link_sema;
static u_int16 handle;

#if EBMK_TEST == 1
u_int8 setDiscoverMode = MGR_NONDISCOVERABLE;
#endif


/* Test Utility Functions */
int prh_test_setDiscoverableMode(u_int8 mode);
int prh_test_setConnectableMode(u_int8 mode);
int prh_test_baseband_connect(void (*connectCallback)(tid_t, t_api));
void prh_test_parsePINResponse(char *hexnumber, u_int8 **pinCode, u_int8 *pinLen);
int prh_test_setSecurityMode(u_int8 mode);
void prh_test_setupL2CAP();
void prh_test_l2cap_channelEventHandler(u_int8 eventType, t_L2_ControlCallbackArgs *pArgs);
void prh_test_recvDataHandler(u_int16 cid, struct host_buf *p_buf);
void prh_test_SendL2CAPData(int cid, u_int8 *data, int lendata);

/* CallbackFunctions */
void setDiscoverableMode_callback(int status);
void setConnectableMode_callback(int status);
void prh_TP_MOD_NDIS_BV_01_C_inquiry_Callback(t_BT_DeviceEntry *newDevice, t_api status); 
void prh_TP_MOD_GDIS_BV_01_C_inquiry_Callback(t_BT_DeviceEntry *newDevice, t_api status); 
void prh_TP_MOD_NCON_BV_01_C_connect_Callback(tid_t transid, t_api result);
void prh_TP_MOD_CON_BV_01_C_connect_Callback(tid_t transid, t_api result);
void prh_TP_MOD_NPAIR_BV_01_C_bonding_Callback(t_bdaddr address, t_api status);
void prh_TP_SEC_AUT_BV_01_C_pinRequest_callback(t_bdaddr bdAddress);
void prh_TP_SEC_AUT_BV_01_C_bonding_Callback(t_bdaddr address, t_api status);
void prh_TP_SEC_AUT_BV_01_C_connect_Callback(tid_t transid, t_api status);
void prh_TP_SEC_AUT_BV_01_C_connect_Callback2(tid_t transid, t_api status);
void prh_TP_SEC_AUT_BV_02_C_connect_Callback(tid_t transid, t_api status);
void prh_TP_SEC_AUT_BV_02_C_setSecurityMode_Callback(int status);


void (*prh_test_l2capConfigConfCallback)(t_L2_ControlCallbackArgs *pArgs);
l2_callbacks l2cap_cbs;


int prh_test_gap_tester_TP_MOD_NDIS_BV_01_C(void)
{
  MGR_Inquiry(INQUIRYTEST_LEN, BT_GIAC_LAP, INQUIRYTEST_MAXRESULTS, prh_TP_MOD_NDIS_BV_01_C_inquiry_Callback);
}

int prh_test_gap_tester_TP_MOD_GDIS_BV_01_C(void)
{
  MGR_Inquiry(INQUIRYTEST_LEN, BT_GIAC_LAP, INQUIRYTEST_MAXRESULTS, prh_TP_MOD_GDIS_BV_01_C_inquiry_Callback);
}

int prh_test_gap_tester_TP_MOD_NCON_BV_01_C(void)
{
  prh_test_baseband_connect(prh_TP_MOD_NCON_BV_01_C_connect_Callback);
}

int prh_test_gap_tester_TP_MOD_CON_BV_01_C(void)
{
  prh_test_baseband_connect(prh_TP_MOD_CON_BV_01_C_connect_Callback);
}

int prh_test_gap_tester_TP_MOD_NPAIR_BV_01_C(void)
{
  t_bdaddr iutBdAddr;

  MGR_ParseHexBDAddress(IUT_BDADDR, &iutBdAddr);
  MGR_InitBonding(iutBdAddr, prh_TP_MOD_NPAIR_BV_01_C_bonding_Callback);
}


int prh_test_gap_tester_TP_SEC_AUT_BV_01_C(void)
{
  t_bdaddr iutBdAddr;

  MGR_ParseHexBDAddress(IUT_BDADDR, &iutBdAddr); 

  /* Register PIN Handler to handle PIN requests for remote IUT to demonstrate App entry of PIN */
  MGR_SetRemoteDevicePIN(iutBdAddr, MGR_USERREQPIN, 0, 0);
  MGR_RegisterPINHandler(prh_TP_SEC_AUT_BV_01_C_pinRequest_callback);
  MGR_InitBonding(iutBdAddr, prh_TP_SEC_AUT_BV_01_C_bonding_Callback); 

  /* prh_test_baseband_connect(prh_TP_SEC_AUT_BV_01_C_connect_Callback); */
  
}



int prh_test_gap_tester_TP_SEC_AUT_BV_02_C(void) 
{
  t_bdaddr iutBdAddr;

  MGR_ParseHexBDAddress(IUT_BDADDR, &iutBdAddr); 
  prh_test_setSecurityMode(MGR_LINKLEVEL);
  /* Register PIN Handler to handle PIN requests for remote IUT to demonstrate App entry of PIN */
  MGR_SetRemoteDevicePIN(iutBdAddr, MGR_USERREQPIN, 0, 0);
  MGR_RegisterPINHandler(prh_TP_SEC_AUT_BV_01_C_pinRequest_callback);
  prh_test_baseband_connect(prh_TP_SEC_AUT_BV_02_C_connect_Callback);
   

}

int prh_test_gap_tester_TP_SEC_SEM_BV_01_C(void)
{

  t_bdaddr iutBdAddr;

  MGR_ParseHexBDAddress(IUT_BDADDR, &iutBdAddr);   
  prh_test_setupL2CAP();
  L2_Connect(iutBdAddr, PRH_TEST_PSM);
  

}


int prh_test_gap_tester_TP_SEC_SEM_BV_02_C(void)
{
  prh_test_gap_tester_TP_SEC_SEM_BV_01_C();
}



int prh_test_gap_tester_TP_SEC_SEM_BV_03_C(void)
{
  prh_test_gap_tester_TP_SEC_SEM_BV_01_C();
}



int prh_test_gap_tester_TP_IDLE_GIN_BV_01_C(void)
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


int prh_test_gap_tester_TP_IDLE_LIN_BV_01_C(void)
{
#if EBMK_TEST == 1
  if(BT_NOERROR == prh_test_setConnectableMode(MGR_CONNECTABLE))
    prh_test_setDiscoverableMode(MGR_LIMITEDDISCOVERY);
  else
    setDiscoverMode = MGR_LIMITEDDISCOVERY;
#else
  prh_test_setDiscoverableMode(MGR_LIMITEDDISCOVERY);
#endif

}


int prh_test_gap_tester_TP_IDLE_BON_BV_01_C(void)
{
  t_bdaddr iutBdAddr;

  MGR_ParseHexBDAddress(IUT_BDADDR, &iutBdAddr); 
  /* Register PIN Handler to handle PIN requests for remote IUT to demonstrate App entry of PIN */
  MGR_SetRemoteDevicePIN(iutBdAddr, MGR_USERREQPIN, 0, 0);
  MGR_RegisterPINHandler(prh_TP_SEC_AUT_BV_01_C_pinRequest_callback);
}

int prh_test_gap_tester_TP_EST_LIE_BV_02_C(void)
{

  prh_test_setConnectableMode(MGR_CONNECTABLE);
  prh_test_setSecurityMode(MGR_NOSECURITY);

  /* setup L2CAP server */
  prh_test_setupL2CAP();

}



/* Test Utility Functions */
int prh_test_setDiscoverableMode(u_int8 mode)
{
  int status;

  status = MGR_SetDiscoverableMode(mode, setDiscoverableMode_callback);
  if(BT_NOERROR == status)
    fprintf(stderr, "GAP TESTER: Discoverable Mode set\n");
  else if(BT_PENDING != status && BT_NOERROR != status)
    fprintf(stderr, "GAP TESTER: ERROR setting Discoverable Mode\n");

  return status;

}


int prh_test_setConnectableMode(u_int8 mode)
{
  int status;

  status = MGR_SetConnectableMode(mode, setConnectableMode_callback);
  if(BT_NOERROR == status)
    fprintf(stderr, "GAP TESTER: Connectable Mode set\n");
  else if(BT_PENDING != status && BT_NOERROR != status)
    fprintf(stderr, "GAP TESTER: ERROR setting Connectable Mode\n");

  return status;
}









int prh_test_baseband_connect(void (*connectCallback)(tid_t, t_api))
{
  t_api status;
  /* u_int16 handle; */
  tid_t tranid = (tid_t)1;
  t_bdaddr iutBdAddr;

  acl_link_sema=pSemCreate(0);
  MGR_ParseHexBDAddress(IUT_BDADDR, &iutBdAddr);
  status=prh_mgr_createACL(iutBdAddr, 0, &handle, connectCallback, tranid);
  pSemLock(acl_link_sema, 0);
  pSemFree(acl_link_sema);

  return (int)status;
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

int prh_test_setSecurityMode(u_int8 mode)
{
  int status;

  status = MGR_SetSecurityMode(mode, prh_TP_SEC_AUT_BV_02_C_setSecurityMode_Callback);
  if(BT_NOERROR == status)
    fprintf(stderr, "GAP TESTER: Security Mode set\n");
  else if(BT_PENDING != status && BT_NOERROR != status)
    fprintf(stderr, "GAP TESTER: ERROR setting Security Mode\n");

  return status;
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
      fprintf(stderr, "GAP TESTER - Connection Indication Received\n");
      L2_ConnectResponse(pArgs->BDAddress, pArgs->cid, 0x00 , pArgs->status);
      L2_Configure(pArgs->cid, PRH_TEST_MTUSIZE, flushTimeout, linkSupTimeout, 0x00); 
      break;

    case L2CAP_evCONFIGUREIND:
      fprintf(stderr, "GAP TESTER - Config Indication Received\n");
      L2_ConfigureResponse(pArgs->cid, PRH_TEST_MTUSIZE, 0x00, 0x00);
      break;
      
    case L2CAP_evDISCONNECTIND:
      fprintf(stderr, "GAP TESTER - Disconnection Indication Received\n");
      break;

    case L2CAP_evCONNECTCONF:
      fprintf(stderr, "GAP TESTER - Connection Confirmation Received\n");
      L2_Configure(pArgs->cid, PRH_TEST_MTUSIZE, flushTimeout, linkSupTimeout, 0x00); 
      break;

    case L2CAP_evCONFIGURECONF:
      fprintf(stderr, "GAP TESTER - Configure Confirm Received\n");
      /* prh_test_l2capConfigConfCallback(pArgs); */
      prh_test_SendL2CAPData(pArgs->cid, "THIS IS SAMPLE DATA", 19);  
      break;

    case L2CAP_evDISCONNECTCONF:
      fprintf(stderr, "GAP TESTER - Disconnect Confirm Received\n");
      break;

    default:
      break;
    }

}


void prh_test_recvDataHandler(u_int16 cid, struct host_buf *p_buf)
{
  int i;

  fprintf(stderr, "GAP TESTER - Received Data:");
  for(i=0; i<p_buf->len; i++)
    fprintf(stderr, "%c", p_buf->data[i]);
  fprintf(stderr, "\n");

}



void prh_test_SendL2CAPData(int cid, u_int8 *data, int lendata)
{
  struct host_buf *h_buff;
  int i;

  h_buff = (struct host_buf*)host_buf_alloc(lendata + L2CAP_HDR_LEN);
  host_buf_reserve_header(h_buff, L2CAP_HDR_LEN);

  for(i=0; i<lendata; i++)
    h_buff->data[i] = data[i];      

  h_buff->len = lendata;
  prh_l2_sar_data_req(0,cid, h_buff);
  /* L2_Write(cid, h_buff); */

}





/* Callback Functions */

void setDiscoverableMode_callback(int status)
{
  if(BT_NOERROR == status)
    fprintf(stderr, "GAP TESTER: Discoverable Mode set\n");
  else
    fprintf(stderr, "GAP TESTER: ERROR setting Discoverable Mode\n");
}

void setConnectableMode_callback(int status)
{
  if(BT_NOERROR == status)
    fprintf(stderr, "GAP TESTER: Connectable Mode set\n");
  else
    fprintf(stderr, "GAP TESTER: ERROR setting Connectable Mode\n");



#if EBMK_TEST == 1
  if(MGR_NONDISCOVERABLE != setDiscoverMode)
    {
      prh_test_setDiscoverableMode(setDiscoverMode);
      setDiscoverMode = MGR_NONDISCOVERABLE;      
    }
#endif
}


void prh_TP_MOD_NDIS_BV_01_C_inquiry_Callback(t_BT_DeviceEntry *newDevice, t_api status)
{
  static u_int8 iutFound = 0;
  t_bdaddr iutBdAddr;

  MGR_ParseHexBDAddress(IUT_BDADDR, &iutBdAddr);
  if(newDevice)
    {
      if(BD_ADDR_CMP(newDevice->bdAddress, iutBdAddr))
        iutFound = 1;
    }
  else
    {
      if(iutFound)
        fprintf(stderr, "GAP TEST FAILED: - IUT Device found\n");
      else
        fprintf(stderr, "GAP TEST PASSED: - IUT Device not found\n");
    }
}


void prh_TP_MOD_GDIS_BV_01_C_inquiry_Callback(t_BT_DeviceEntry *newDevice, t_api status)
{
  static u_int8 iutFound = 0;
  t_bdaddr iutBdAddr;

  MGR_ParseHexBDAddress(IUT_BDADDR, &iutBdAddr);
  if(newDevice)
    {
      if(BD_ADDR_CMP(newDevice->bdAddress, iutBdAddr))
        iutFound = 1;
    }
  else
    {
      if(iutFound)
        fprintf(stderr, "GAP TEST PASSED: - IUT Device found\n");
      else
        fprintf(stderr, "GAP TEST FAILED: - IUT Device not found\n");
    }
}

void prh_TP_MOD_NCON_BV_01_C_connect_Callback(tid_t transid, t_api status)
{
  if(HCI_errPAGETIMEOUT == status)
    fprintf(stderr,"TEST PASSED - Page time out occured\n");
  else
    fprintf(stderr,"TEST FAILED\n");
  pSemUnlock(acl_link_sema);
}

/*
void prh_TP_MOD_CON_BV_01_C_connect_Callback(tid_t transid, t_api status)
{
  pSemUnlock(acl_link_sema);
 
}
*/

void prh_TP_MOD_CON_BV_01_C_connect_Callback(tid_t transid, t_api status)
{
  if(BT_NOERROR == status)
    fprintf(stderr,"TEST PASSED - ACL established\n");
  else
    fprintf(stderr,"TEST FAILED - ACL not established\n");
  pSemUnlock(acl_link_sema);
}


void prh_TP_MOD_NPAIR_BV_01_C_bonding_Callback(t_bdaddr address, t_api status)
{
 t_bdaddr iutBdAddr;

 MGR_ParseHexBDAddress(IUT_BDADDR, &iutBdAddr); 
 if(BD_ADDR_CMP(address, iutBdAddr) && BT_NOERROR != status)
   fprintf(stderr,"TEST PASSED - Pairing failed with status = %d\n", status);
 else
   fprintf(stderr,"TEST FAILED - status = %d\n", status);
}


void prh_TP_SEC_AUT_BV_01_C_pinRequest_callback(t_bdaddr bdAddress)
{
  int i;
  int x;
  char strPINInfo[] = "3 0x112233";
  char buffer[20];
  u_int8 pinSupplied = 0;
  u_int8 pinLen;
  u_int8 *pinCode;

  MGR_PrintBDAddress(bdAddress, buffer);
  fprintf(stderr, "GAP TESTER: PIN REQUEST for BD_ADDR: %s \n", buffer);
  fprintf(stderr, "Enter PIN Info: ");


  /* gets(strPINInfo);  */
  prh_test_parsePINResponse(strPINInfo, &pinCode, &pinLen); 
  pinSupplied = 1;  
  if( BT_NOERROR != MGR_PINResponse(bdAddress, pinSupplied, pinCode, pinLen) )
    fprintf(stderr, "GAP TESTER -  ERROR: Responding to PIN Request\n");
  
}


void prh_TP_SEC_AUT_BV_01_C_bonding_Callback(t_bdaddr address, t_api status)
{
  struct st_t_BT_DeviceEntry *devDbEntry;

  MGR_DeleteLinkKey(address);
  
  prh_mgr_releaseACL(handle, 0x13, NULL);
  sleep(3);
  prh_test_baseband_connect(prh_TP_SEC_AUT_BV_01_C_connect_Callback2);

}


void prh_TP_SEC_AUT_BV_01_C_connect_Callback(tid_t transid, t_api status)
{
  /*
    if(BT_NOERROR == status)
    fprintf(stderr,"TESTER: - ACL established\n");
    else
    fprintf(stderr,"TESTER: - ACL not established\n");
  */
  pSemUnlock(acl_link_sema);
  prh_mgr_releaseACL(handle, 0x13, NULL);
  sleep(3);
  prh_test_baseband_connect(prh_TP_SEC_AUT_BV_01_C_connect_Callback2);

}

void prh_TP_SEC_AUT_BV_01_C_connect_Callback2(tid_t transid, t_api status)
{
  
  if(BT_NOERROR == status)
    fprintf(stderr,"GAP TESTER: - ACL established\n");
  else
    fprintf(stderr,"GAP TESTER: - ACL not established\n");
  
  pSemUnlock(acl_link_sema);
}



void prh_TP_SEC_AUT_BV_02_C_connect_Callback(tid_t transid, t_api status)
{ 
  if(BT_NOERROR == status)  
    fprintf(stderr,"GAP TESTER: - ACL established\n");
  else  
    fprintf(stderr,"GAP TESTER: - ACL not established\n");
  pSemUnlock(acl_link_sema);  
  
}

void prh_TP_SEC_AUT_BV_02_C_setSecurityMode_Callback(int status)
{
  if(BT_NOERROR == status)
    fprintf(stderr, "GAP TESTER: Security Mode set\n");
  else
    fprintf(stderr, "GAP TESTER: ERROR setting Security Mode\n");
}



int (*gap_tester_conform_test[])(void) = 
{
  prh_test_gap_tester_TP_MOD_NDIS_BV_01_C,  /* TP/MOD/NDIS/BV-01-C */
  prh_test_gap_tester_TP_MOD_GDIS_BV_01_C,  /* TP/MOD/GDIS/BV-01-C */
  prh_test_gap_tester_TP_MOD_NCON_BV_01_C,  /* TP/MOD/NCON/BV-01-C */
  prh_test_gap_tester_TP_MOD_CON_BV_01_C,   /* TP/MOD/CON/BV-01-C */
  prh_test_gap_tester_TP_MOD_NPAIR_BV_01_C, /* TP/MOD/NPAIR/BV-01-C */
  prh_test_gap_tester_TP_SEC_AUT_BV_01_C,   /* TP/SEC/AUT/BV-01-C */
  prh_test_gap_tester_TP_SEC_AUT_BV_02_C,   /* TP/SEC/AUT/BV-02-C */
  prh_test_gap_tester_TP_SEC_SEM_BV_01_C,   /* TP/SEC/SEM/BV-01-C */
  prh_test_gap_tester_TP_SEC_SEM_BV_02_C,   /* TP/SEC/SEM/BV-01-C */
  prh_test_gap_tester_TP_SEC_SEM_BV_03_C,   /* TP/SEC/SEM/BV-01-C */
  prh_test_gap_tester_TP_IDLE_GIN_BV_01_C,  /* TP/IDLE/GIN/BV-01-C */
  prh_test_gap_tester_TP_IDLE_LIN_BV_01_C,  /* TP/IDLE/LIN/BV-01-C */
  prh_test_gap_tester_TP_IDLE_BON_BV_01_C,  /* TP/IDLE/BON/BV-01-C */
  prh_test_gap_tester_TP_EST_LIE_BV_02_C    /* TP/EST/LIE/BV-02-C */
};
