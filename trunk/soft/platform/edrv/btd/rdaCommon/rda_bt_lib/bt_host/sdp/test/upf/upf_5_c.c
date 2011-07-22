/*
  This is a sample server program that is to be used with all tests.
 */

#include "host_types.h"
#include "host_buf.h"
#include "stdio.h"
#include "mgr_core.h"
#include "mgr_utils.h"
#include "sdp_ue.h"
#include "sdp_db.h"
#include "sdp_callbacks.h"

#include "sdp_le.h"
#include "host_ctrl.h"

/*#include <unistd.h>*/


#include "l2cap_primitives.h"
#define WIN_TEST 1


#if WIN_TEST

//#include "winbase.h"

void sleep(u_int32 dur)

{
	Sleep(dur);
}

#endif

int incoming_configured, outgoing_configured;

#define SERV_TCP_PORT 6542
#define SERV_HOST_ADDR "127.0.0.1"
#define BD_ADDR_SERVER  "MASTER"
#define BD_ADDR_CLIENT "SLAVE "

t_bdaddr bd_addr_serv;
t_bdaddr bd_addr_cli;
t_bdaddr bd_addr_test;
void sigio_handler(int signum);
void test_1_wrapper(u_int16 cid,struct host_buf * p_buf);
int linux_sys_close;

int linux_sys_mode;


u_int32 test_state = 0;
u_int8 handle[4];
u_int32 test_mode = 0; /* 0 = 128uuid , 1=32 , 2=16 */
u_int16 test_mtu = 90;
t_bdaddr test_address;



void (*test_procedure)(u_int16 cid,struct host_buf * p_buf);


u_int8 SearchPattern_16[5] = {
  0x35,0x03, /* list */
  0x19,0x00,0x00 /* 16 bit uuid offset 3 */
};

u_int8 SearchPattern_32[7] = {
  0x35,0x05, /* list */
  0x1a,0x00,0x00,0x00,0x00 /* 32 bit offset 3 */
};

u_int8 SearchPattern_128[19] = {
  0x35,0x11, /* list */
  0x1c,0x00,0x00,0x00,0x00, /* 128 offset 3 for 32, 5 for 16 */
  0x00,0x00,0x10,0x00,0x80,0x00,0x00,0x80,0x5F,0x9B,0x34,0xFB
  /* 0x00,0x00,0x10,0x00,0x70,0x07,0x00,0x80,0x5f,0x9b,0x34,0xfb   */
};

u_int8 Attributes_pdu[5] = {
  0x35,0x03, /* list */
  0x09,0x00,0x00 /* offst 3 */
};




 
void upf_5_test_1(void); 
void upf_5_test_2(void); 
void upf_5_test_3(void); 
void upf_5_test_4(void); 
void upf_5_test_5(void); 
void upf_5_test_6(void); 
void upf_5_test_7(void); 
void upf_5_test_8(void); 
void upf_5_test_9(void); 
void upf_5_test_10(u_int16,struct host_buf *); 
void upf_5_test_11(u_int16,struct host_buf *); 
void upf_5_test_12a(u_int16,struct host_buf *);
void upf_5_test_12b(u_int16,struct host_buf *); 
void upf_5_test_13(void); 
void upf_5_test_14(void); 

/*
	MAIN
*/

DWORD WINAPI main_loop_wrapper(LPVOID Arg)
{
	BT_Main(0);

	return 0;
}



int main(int argc, char **argv)
{


#if !WIN_TEST




  linux_sys_mode=1;


  sys_init(linux_sys_mode); /* server */
  
  fprintf(stderr,"starting the stack\n");
#else
		
	BT_Init(0);

	CreateThread(NULL,0,main_loop_wrapper,0,0,NULL);

	while(MGR_Init(0) == BT_RETRY);

#endif


	MGR_ParseHexBDAddress("0xdbdbdbdb0001",&test_address);


	pDebugCheck();
/*	upf_5_test_1(); 
	upf_5_test_2(); 
	upf_5_test_3(); 
	upf_5_test_4(); 
	upf_5_test_5(); 
	upf_5_test_6(); 
	upf_5_test_7();*/
	upf_5_test_8(); 
	upf_5_test_9(); 
	upf_5_test_9();
/*	upf_5_test_10(0,0);
	upf_5_test_11(0,0); 
	upf_5_test_12a(0,0);
	upf_5_test_12b(0,0);
	upf_5_test_13(); 
	upf_5_test_14(); */

	return 1;
}




void sample_conn_callback(u_int8 type,t_L2_ControlCallbackArgs *pArgs)
{
  pDebugPrintf((pLOGDEBUG," Connection is called back : "));
  pDebugPrintf((pLOGDEBUG,"     Starting to configure the link "));

  L2_Configure(pArgs->cid,test_mtu,0,0,0x00); 
}

void sample_config_callback(u_int8 type,t_L2_ControlCallbackArgs *pArgs)
{

  pDebugPrintf((pLOGDEBUG,"     the link is configured now. "));
  
  test_procedure(pArgs->cid,0x00);

}

void sample_data_callback(u_int16 cid,struct host_buf * p_buf)
{

/*  sdp_print_buffer(p_buf->data,p_buf->len);*/

  //pDebugPrintfEX((pLOGDEBUG,pLOGSDP,"<<< SDP Revieved from peer :\n"));

#if pDEBUG
  pDebugPrintfEX((pLOGDEBUG,pLOGSDP,"SDP - Received data from peer"));

#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)

  SYSDebug_PrintEx(pLOGDEBUG,pLOGSDP,(LOG_SDP_UPF_5+1),NULL,0,0);
#endif
  pDebugDumpEX(pLOGDEBUG,pLOGSDP,p_buf->data,p_buf->len);


	test_procedure(cid,p_buf);


  host_buf_free(p_buf);
}


/***********************************************************************
 *
 * FUNCTION NAME:   prh_test_config_ind_cb
 * DESCRIPTION:
 
 This function handles the config indication callback. It sends
 back a cofig response and it will also initiate a config request from 
 this side. If the requested mtu is of a smaller size then the SDP mtu will
 be changed for this session.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
void prh_test_config_ind_cb(u_int8 type,t_L2_ControlCallbackArgs *pArgs)
{
  u_int16 response;
  u_int8 error_code;

  response = 0;

  pDebugPrintf((pLOGDEBUG,"SDP - config ind"));
  
  if(pArgs->mtu < test_mtu)
    {
      test_mtu = pArgs->mtu;
    }
  incoming_configured=1;
  error_code = L2_ConfigureResponse(pArgs->cid,test_mtu,response,0x00);

  if(error_code == BT_NOERROR)
    {
   
      
    } else if (error_code == BT_RETRY) {
      /* call back another time */
    } else {
      /* this is an error code */
    }  
	if (outgoing_configured==1)
		test_procedure(pArgs->cid,0x00);			
}

void conn_test()
{
  t_bdaddr address;
  l2_callbacks cbs;
 
  address = test_address;
  
  cbs.connectionIndication = prh_sdp_l2cap_connect_ind_cb;
  cbs.configureIndication = prh_test_config_ind_cb;
  cbs.disconnectionIndication = prh_sdp_l2cap_disconnect_ind_cb;
  cbs.qosviolationIndication = 0x00; /* null */
  cbs.connectionConfirmation = sample_conn_callback;
  cbs.configureConfirmation = sample_config_callback;
  cbs.disconnectionConfirmation = prh_sdp_l2cap_disconnect_cfm_cb;
  cbs.dataIndication = sample_data_callback;
  
  L2_RegisterPSM(SDP_PSM,&cbs);

  L2_Connect(address,0x01);
}


int test_pattern(u_int8* pData,u_int16 search_pattern)
{
  int retvalue = 0;
  int i;

  switch(test_mode)
    {
    case 0:
      for(i = 0 ; i< sizeof(SearchPattern_128);i++)
    {
      pData[i] = SearchPattern_128[i];
    }
      pData[5] = search_pattern >> 8; 
      pData[6] = search_pattern & 0xff;
      
      retvalue = sizeof(SearchPattern_128);
      break;
    case 1:
      for(i = 0 ; i< sizeof(SearchPattern_32);i++)
    {
      pData[i] = SearchPattern_32[i];
    }
      pData[5] = search_pattern >> 8;
      pData[6] = search_pattern & 0xff;
      
      retvalue = sizeof(SearchPattern_32);
      break;
    case 2:
      for(i = 0 ; i< sizeof(SearchPattern_16);i++)
    {
      pData[i] = SearchPattern_16[i];
    }
      pData[3] = search_pattern > 8; 
      pData[4] = search_pattern & 0xff;
      
      retvalue = sizeof(SearchPattern_16);
      break;

    default:
      test_state = 0;
      retvalue = -1;
    }
  
  return retvalue;
}

void test_attrib(u_int8* pData,u_int16 attrib)
{
  int i;
  
  for(i = 0; i< sizeof(Attributes_pdu) ; i++)
    {
      pData[i] = Attributes_pdu[i];
    }

  pData[3] = attrib >> 8;
  pData[4] = attrib & 0xff;
}


void test_finish_callback(u_int8 code,void * d)
{
  test_state = 0;
}




/*

  UPF 3

*/


void upf_3_test_1(void)
{
  u_int32 uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;
  
  address= test_address;
  /* address = prh_mgr_readHexBDAddress("0x0080371221ce"); */
  uuid = 0x1103;

  /* bd address list to search for */

  devices.numItems = 1;
  devices.addresses = malloc(sizeof(t_bdaddr));  
  devices.addresses[0] = address;

  /* device relation criteria */

  criteria = SDP_TRUSTED;


  /* search pattern */

  pattern.numItems = 1;
  pattern.patternUUIDs = malloc(4);
  pattern.patternUUIDs[0] = uuid;

  /* attributes to retrieve */

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(4);
  attribs.attributeIDs[0] = 0x00000004; /* protocol decriptor list */
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 15;
  rule.maxBytes = 250;

	/* call a service search and do not retrieve the device name */

  SDP_RegisterPSM();

test_state = 1;

  SDP_ServiceSearch(&pResults,&devices,&pattern,&attribs,&rule,criteria,test_finish_callback);  
 

  while(test_state); /* wait for results */
  
  test_state = 0;
}


void upf_3_test_2(void)
{
  u_int32 uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;
  
  address= test_address;
  uuid = 0x1002;

  /* bd address list to search for */

  devices.numItems = 1;
  devices.addresses = malloc(sizeof(t_bdaddr));  
  devices.addresses[0] = address;

  /* device relation criteria */

  criteria = SDP_TRUSTED;


  /* search pattern */

  pattern.numItems = 1;
  pattern.patternUUIDs = malloc(4);
  pattern.patternUUIDs[0] = uuid;

  /* attributes to retrieve */

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(4);
  attribs.attributeIDs[0] = 0x00000100;
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;
  rule.maxBytes = 250;

  /* call a service search and do not retrieve the device name */

  SDP_RegisterPSM();

  test_state = 1;

  SDP_ServiceBrowse(&pResults,&devices,&pattern,&attribs,&rule,criteria,test_finish_callback);    

  /* examine the results */

  /* sample_results(pResults->element.attribs.pData); */
  while(test_state);

  test_state = 0;
  
}


void upf_3_test_3(void)
{
  u_int32 uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;
  
  address= test_address;
  uuid = 0x1105;

  /* bd address list to search for */

  devices.numItems = 1;
  devices.addresses = malloc(sizeof(t_bdaddr));  
  devices.addresses[0] = address;

  /* device relation criteria */

  criteria = SDP_TRUSTED;

  /* search pattern */

  pattern.numItems = 1;
  pattern.patternUUIDs = malloc(4);
  pattern.patternUUIDs[0] = uuid;

  /* attributes to retrieve */

  attribs.numItems = 2;
  attribs.attributeIDs = malloc(8);
  attribs.attributeIDs[0] = 0x00000000;
  attribs.attributeIDs[1] = 0x0001ffff;
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;
  rule.maxBytes = 250;

  /* call a service search and do not retrieve the device name */

  SDP_RegisterPSM();

  test_state = 1;

  SDP_ServiceBrowse(&pResults,&devices,&pattern,&attribs,&rule,criteria,test_finish_callback);    

  while(test_state);

  test_state = 0;

  /* examine the results */

  /* sample_results(pResults->element.attribs.pData); */
}


void upf_3_test_4(void)
{
  u_int32 uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;
  
  address= test_address;
  uuid = 0x1234;

  /* bd address list to search for */

  devices.numItems = 1;
  devices.addresses = malloc(sizeof(t_bdaddr));  
  devices.addresses[0] = address;

  /* device relation criteria */

  criteria = SDP_TRUSTED;


  /* search pattern */

  pattern.numItems = 1;
  pattern.patternUUIDs = malloc(4);
  pattern.patternUUIDs[0] = uuid;

  /* attributes to retrieve */

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(4);
  attribs.attributeIDs[0] = 0x00000004;

  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;

  /* call a service search and do not retrieve the device name */

  SDP_RegisterPSM();

  SDP_ServiceSearch(&pResults,&devices,&pattern,&attribs,&rule,criteria,test_finish_callback);    

  while(test_state);

  test_state = 0;

  /* examine the results */

  /* sample_results(pResults->element.attribs.pData); */
}


void upf_3_test_5(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0006;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- sophi test 5"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = upf_3_test_5;
      conn_test();
      break;
    case 1:
      test_state++;
      
      h_buff = host_buf_alloc(200+10);
      host_buf_reserve_header(h_buff,10);
      
      pData = h_buff->data;
      
      *pData++ = 0x04;
      *pData++ = 0x00;
      *pData++ = test_state;
      pData +=2; /* move past the length field */           

      *pData++ = 0x12;
      *pData++ = 0x34;
      *pData++ = 0x56;
      *pData++ = 0x78;

      *pData++ = 0x00;
      *pData++ = 0x60;

      pdu_len += 6;

      test_attrib(pData,attribute);
      
      pData += 5;
      pdu_len += 5;
            
      *pData = 0x00;

      pdu_len += 1;
      
      /* invalid length */
      h_buff->data[3] = pdu_len >> 8;
      h_buff->data[4] = pdu_len & 0xff;
      
      h_buff->len = 5 + pdu_len;
      L2_Write(cid,(t_DataBuf*)h_buff);
      break;
    case 2:      
      test_state = 0;
      L2_Disconnect(cid);

      break;      
    default:
      /* use this like a java final */
      test_state = 0;
      break;
    }
}



void upf_3_test_6(void)
{
  u_int32 uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;
  
  address= test_address;
  uuid = 0x1111;

  /* bd address list to search for */

  devices.numItems = 1;
  devices.addresses = malloc(sizeof(t_bdaddr));  
  devices.addresses[0] = address;

  /* device relation criteria */

  criteria = SDP_TRUSTED;


  /* search pattern */

  pattern.numItems = 1;
  pattern.patternUUIDs = malloc(4);
  pattern.patternUUIDs[0] = uuid;

  /* attributes to retrieve */

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(4);
  attribs.attributeIDs[0] = 0x0000abcd;
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;
  rule.maxBytes = 250;

  /* call a service search and do not retrieve the device name */

  SDP_ServiceBrowse(&pResults,&devices,&pattern,&attribs,&rule,criteria,test_finish_callback);    

  while(test_state);

  test_state = 0;

  /* examine the results */

  /* sample_results(pResults->element.attribs.pData); */
}




void upf_3_test_7(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0009;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- sophi test 7"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = upf_3_test_7;
      conn_test();
      break;
    case 1:
      test_state++;
      
      h_buff = host_buf_alloc(200+10);
      host_buf_reserve_header(h_buff,10);
      
      pData = h_buff->data;
      
      *pData++ = 0x04;
      *pData++ = 0x00;
      *pData++ = test_state;
      pData +=2; /* move past the length field */           

      *pData++ = 0x00;
      *pData++ = 0x01;
      *pData++ = 0x00;
      *pData++ = 0x02;

      *pData++ = 0x00;
      *pData++ = 0x60;

      pdu_len += 6;

      test_attrib(pData,attribute);
      
      pData += 5;
      pdu_len += 5;
            
      *pData = 0x00;

      pdu_len += 1;
      
      /* invalid length */
      h_buff->data[3] = 0x50 >> 8;
      h_buff->data[4] = 0x50 & 0xff;
      
      h_buff->len = 5 + pdu_len;
      L2_Write(cid,(t_DataBuf*)h_buff);
      break;
    case 2:      
      test_state = 0;
      L2_Disconnect(cid);

      break;      
    default:
      /* use this like a java final */
      test_state = 0;
      break;
    }
}




/*
  I will have to hand code the pdu as my encoder will not create an incorrect pdu.
 */
void upf_3_test_8(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1111;
  attribute = 0x0009;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- sophi test 8"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = upf_3_test_8;
      conn_test();
      break;
    case 1:
      test_state++;
      /* here I will need to start the first part of the tests */       
      h_buff = host_buf_alloc(200+10);
      host_buf_reserve_header(h_buff,10);
      
      pData = h_buff->data;
      
      *pData++ = 0x06;
      *pData++ = 0x00;
      *pData++ = test_state;
      pData +=2; /* move past the length field */           

      i = test_pattern(pData,search_pattern);

      if(i == -1)
    return;

      /* invalidate the syntax*/

      pData[2] = 0x09;

      pData += i;
      pdu_len += i;
      
      *pData++ = 0x00;
      *pData++ = 0x60;
      
      pdu_len += 2;

      test_attrib(pData,attribute);

      pData += 5;
      pdu_len += 5;

      *pData = 0x00;

      pdu_len++;
            
      h_buff->data[3] = pdu_len >> 8;
      h_buff->data[4] = pdu_len & 0xff;
      
      h_buff->len = 5 + pdu_len;
      L2_Write(cid,(t_DataBuf*)h_buff);
      break;
    case 2:
      test_state = 0;
      L2_Disconnect(cid);

      break;      
    default:
      /* use this like a java final */
      test_state = 0;
      break;
    }
}


void upf_3_test_9(void)
{
  u_int32 uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;
  
  address= test_address;
  uuid = 0x1105;

  /* bd address list to search for */

  devices.numItems = 1;
  devices.addresses = malloc(sizeof(t_bdaddr));  
  devices.addresses[0] = address;

  /* device relation criteria */

  criteria = SDP_TRUSTED;


  /* search pattern */

  pattern.numItems = 3;
  pattern.patternUUIDs = malloc(12);
  pattern.patternUUIDs[0] = uuid;
  pattern.patternUUIDs[1] = 0x0003;
  pattern.patternUUIDs[2] = 0x0100;

  /* attributes to retrieve */

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(4);
  attribs.attributeIDs[0] = 0x00000004;

  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;
  rule.maxBytes = 250;

  /* call a service search and do not retrieve the device name */

  SDP_RegisterPSM();

  SDP_ServiceBrowse(&pResults,&devices,&pattern,&attribs,&rule,criteria,test_finish_callback);    

  while(test_state);

  test_state = 0;
  
  /* examine the results */

  /* sample_results(pResults->element.attribs.pData); */
}




/*
	UPF 4
*/


void upf_4_test_1(void)
{
  test_state = 1;

  upf_3_test_1();
}

void upf_4_test_2(void)
{
  test_state = 1;

  upf_3_test_2();
}

void upf_4_test_3(void)
{

  u_int32 uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;

  test_state = 1;

  
  address= test_address;
  uuid = 0x1105;

  /* bd address list to search for */

  devices.numItems = 1;
  devices.addresses = malloc(sizeof(t_bdaddr));  
  devices.addresses[0] = address;

  /* device relation criteria */

  criteria = SDP_TRUSTED;


  /* search pattern */

  pattern.numItems = 1;
  pattern.patternUUIDs = malloc(4);
  pattern.patternUUIDs[0] = uuid;

  /* attributes to retrieve */

  attribs.numItems = 2;
  attribs.attributeIDs = malloc(8);
  attribs.attributeIDs[0] = 0x00000000;
  attribs.attributeIDs[1] = 0x0001ffff;
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;
  rule.maxBytes = 0x45 + sizeof(t_SDP_Results);

  /* call a service search and do not retrieve the device name */

  SDP_ServiceBrowse(&pResults,&devices,&pattern,&attribs,&rule,criteria,test_finish_callback);


  while(test_state);

  test_state = 0;

  /* examine the results */

  /* sample_results(pResults->element.attribs.pData); */
}

void upf_4_test_4(void)
{
  test_state = 1;

  upf_3_test_4();
}

void upf_4_test_5(void)
{
  test_state = 0;
  
  upf_3_test_5(0,0);

  while(test_state);

  test_state = 0;
}

void upf_4_test_6(void)
{
  test_state = 1;

  upf_3_test_6();  
}

void upf_4_test_7(void)
{
  test_state = 0;

  upf_3_test_7(0,0);

  while(test_state);
  
  test_state = 0;
}

void upf_4_test_8(void)
{
  test_state = 0;

  upf_3_test_8(0,0);

  while(test_state);

  test_state = 0;
}

void upf_4_test_9(void)
{
  test_state = 1;

  upf_3_test_9();
}

void upf_4_test_10(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x0003;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- upf_4_test_10"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = upf_4_test_10;
      conn_test();
      break;
    case 1:
      test_state++;
      /* here I will need to start the first part of the tests */       
      h_buff = host_buf_alloc(200+10);
      host_buf_reserve_header(h_buff,10);
      
      pData = h_buff->data;
      
      *pData++ = 0x02;
      *pData++ = 0x00;
      *pData++ = test_state;
      pData +=2; /* move past the length field */           

      i = test_pattern(pData,search_pattern);

      if(i == -1)
    return;

      pData += i;
      pdu_len += i;
      
      *pData++ = 0x00;
      *pData++ = 0x02;
      
      *pData = 0x00;

      pdu_len +=3;
      
      h_buff->data[3] = pdu_len >> 8;
      h_buff->data[4] = pdu_len & 0xff;
      
      h_buff->len = 5 + pdu_len;
      L2_Write(cid,(t_DataBuf*)h_buff);
      break;
    case 2:
      test_state = 0;
      L2_Disconnect(cid);

      break;      
    default:
      /* use this like a java final */
      test_state = 0;
      break;
    }  

}

void upf_4_test_11(u_int16 cid,struct host_buf * p_buf)
{
    u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;

  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0001;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- upf_4_test_11"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = upf_4_test_11;
      conn_test();
      break;
    case 1:
      test_state++;
      
      h_buff = host_buf_alloc(200+10);
      host_buf_reserve_header(h_buff,10);
      
      pData = h_buff->data;
      
      *pData++ = 0x04;
      *pData++ = 0x00;
      *pData++ = test_state;
      pData +=2; /* move past the length field */           


      /* DAVE use the first record handle from the last test */



      *pData++ = 0x00;
      *pData++ = 0x01;
      *pData++ = 0x00;
      *pData++ = 0x00;
    /* or 

     *pData++ = 0x00;
     *pData++ = 0x01;
     *pData++ = 0x00;
     *pData++ = 0x00;

     */

    

      *pData++ = 0x00;
      *pData++ = 0x60;

      pdu_len += 6;

      /* attribute id list */

      *pData++ = 0x35;
      *pData++ = 0x09;

      *pData++ = 0x09;
      *pData++ = 0x00;
      *pData++ = 0x06;

      *pData++ = 0x09;
      *pData++ = 0x00;
      *pData++ = 0x11;

      *pData++ = 0x09;
      *pData++ = 0x01;
      *pData++ = 0x00;
      
      
      pdu_len += 11;
            
      *pData = 0x00;

      pdu_len +=1;
      
      h_buff->data[3] = pdu_len >> 8;
      h_buff->data[4] = pdu_len & 0xff;
      
      h_buff->len = 5 + pdu_len;
      L2_Write(cid,(t_DataBuf*)h_buff);
      break;
    case 2:      
      test_state = 0;
      L2_Disconnect(cid);

      break;      
    default:
      /* use this like a java final */
      test_state = 0;
      break;
    }
}


void upf_4_test_12a(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1103;
  attribute = 0x0004;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- upf_4_test_12a"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = upf_4_test_12a;
      conn_test();
      break;
    case 1:
      test_state++;
      /* here I will need to start the first part of the tests */       
      h_buff = host_buf_alloc(200+10);
      host_buf_reserve_header(h_buff,10);
      
      pData = h_buff->data;
      
      *pData++ = 0x02;
      *pData++ = 0x00;
      *pData++ = test_state;
      pData +=2; /* move past the length field */           

      i = test_pattern(pData,search_pattern);

      if(i == -1)
    return;

      pData += i;
      pdu_len += i;
      
      *pData++ = 0x00;
      *pData++ = 0x60;
      
      *pData = 0x00;

      pdu_len +=3;
      
      h_buff->data[3] = pdu_len >> 8;
      h_buff->data[4] = pdu_len & 0xff;
      
      h_buff->len = 5 + pdu_len;
      L2_Write(cid,(t_DataBuf*)h_buff);
      break;
    case 2:
      test_state++;
      
      h_buff = host_buf_alloc(200+10);
      host_buf_reserve_header(h_buff,10);
      
      pData = h_buff->data;
      
      *pData++ = 0x04;
      *pData++ = 0x00;
      *pData++ = test_state;
      pData +=2; /* move past the length field */           

      *pData++ = p_buf->data[9];
      *pData++ = p_buf->data[10];
      *pData++ = p_buf->data[11];
      *pData++ = p_buf->data[12];

      *pData++ = 0x00;
      *pData++ = 0x60;

      pdu_len += 6;
      
      test_attrib(pData,attribute);
      
      pData += 5;
      pdu_len += 5;
            
      *pData = 0x00;

      pdu_len +=1;
      
      h_buff->data[3] = pdu_len >> 8;
      h_buff->data[4] = pdu_len & 0xff;
      
      h_buff->len = 5 + pdu_len;

      sleep(20);

      L2_Write(cid,(t_DataBuf*)h_buff);
      break;
    case 3:      
      test_state = 0;
      L2_Disconnect(cid);

      break;      
    default:
      /* use this like a java final */
      test_state = 0;
      break;
    }  
}

void upf_4_test_12b(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1002;
  attribute = 0x0100;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- upf_4_test_12b"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = upf_4_test_12b;

      conn_test();
      break;
    case 1:
      test_state++;
      /* here I will need to start the first part of the tests */       
      h_buff = host_buf_alloc(200+10);
      host_buf_reserve_header(h_buff,10);
      
      pData = h_buff->data;
      
      *pData++ = 0x06;
      *pData++ = 0x00;
      *pData++ = test_state;
      pData +=2; /* move past the length field */           

      i = test_pattern(pData,search_pattern);

      if(i == -1)
    return;

      pData += i;
      pdu_len += i;
      
      *pData++ = 0x00;
      *pData++ = 0x60;
      
      pdu_len += 2;

      test_attrib(pData,attribute);

      pData += 5;
      pdu_len += 5;

      *pData = 0x00;

      pdu_len++;
            
      h_buff->data[3] = pdu_len >> 8;
      h_buff->data[4] = pdu_len & 0xff;
      
      h_buff->len = 5 + pdu_len;

      sleep(5);

      L2_Write(cid,(t_DataBuf*)h_buff);
      break;
    case 2:
      
      sleep(5);

      test_state = 0;
      L2_Disconnect(cid);

      break;      
    default:
      /* use this like a java final */
      test_state = 0;
      break;
    }  
}



/*
	UPF 5
*/

void upf_5_test_1(void)
{
  test_state = 1;

  upf_3_test_1();
}

void upf_5_test_2(void)
{
  test_state = 1;

  upf_3_test_2();
}


void upf_5_test_3(void)
{
	test_state = 1;

	upf_4_test_3();

}

void upf_5_test_4(void)
{
  test_state = 1;

  upf_3_test_4();
}

void upf_5_test_5(void)
{
  test_state = 0;
  
  upf_3_test_5(0,0);

  while(test_state);

  test_state = 0;
}

void upf_5_test_6(void)
{
  test_state = 1;

  upf_3_test_6();  
}

void upf_5_test_7(void)
{
  test_state = 0;

  upf_3_test_7(0,0);

  while(test_state);
  
  test_state = 0;
}

void upf_5_test_8(void)
{
  test_state = 0;

  upf_3_test_8(0,0);

  while(test_state);

  test_state = 0;
}

void upf_5_test_9(void)
{
  test_state = 1;

  upf_3_test_9();
}

void upf_5_test_10(u_int16 cid,struct host_buf * p_buf)
{
	test_state = 0;
	
	upf_4_test_10(0,0);
	
	while(test_state);
	
	test_state = 0;	
}

void upf_5_test_11(u_int16 cid,struct host_buf * p_buf)
{
	test_state = 0;
	
	upf_4_test_11(0,0);
	
	while(test_state);
	
	test_state = 0;	
}


void upf_5_test_12a(u_int16 cid,struct host_buf * p_buf)
{
  	test_state = 0;
	
	upf_4_test_12a(0,0);
	
	while(test_state);
	
	test_state = 0;	
}

void upf_5_test_12b(u_int16 cid,struct host_buf * p_buf)
{
  	test_state = 0;
	
	upf_4_test_12b(0,0);
	
	while(test_state);
	
	test_state = 0;	

}


void upf_5_test_13(void)
{

  u_int32 uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;

  test_state = 1;

  
  address= test_address;
  uuid = 0xabcd;

  /* bd address list to search for */

  devices.numItems = 1;
  devices.addresses = malloc(sizeof(t_bdaddr));  
  devices.addresses[0] = address;

  /* device relation criteria */

  criteria = SDP_TRUSTED;


  /* search pattern */

  pattern.numItems = 1;
  pattern.patternUUIDs = malloc(4);
  pattern.patternUUIDs[0] = uuid;

  /* attributes to retrieve */

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(8);
  attribs.attributeIDs[0] = 0x00000001;
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;
  rule.maxBytes = 0x45 + sizeof(t_SDP_Results);

  /* call a service search and do not retrieve the device name */

  SDP_RegisterPSM();

  SDP_ServiceBrowse(&pResults,&devices,&pattern,&attribs,&rule,criteria,test_finish_callback);


  while(test_state);

  test_state = 0;

  /* examine the results */

  /* sample_results(pResults->element.attribs.pData); */
}


void upf_5_test_14(void)
{

  u_int32 uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;

  test_state = 1;

  
  address= test_address;
  uuid = 0xabcd;

  /* bd address list to search for */

  devices.numItems = 1;
  devices.addresses = malloc(sizeof(t_bdaddr));  
  devices.addresses[0] = address;

  /* device relation criteria */

  criteria = SDP_TRUSTED;


  /* search pattern */

  pattern.numItems = 1;
  pattern.patternUUIDs = malloc(4);
  pattern.patternUUIDs[0] = uuid;

  /* attributes to retrieve */

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(8);
  attribs.attributeIDs[0] = 0x0000abcd;
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;
  rule.maxBytes = 0x45 + sizeof(t_SDP_Results);

  /* call a service search and do not retrieve the device name */

  SDP_RegisterPSM();

  SDP_ServiceBrowse(&pResults,&devices,&pattern,&attribs,&rule,criteria,test_finish_callback);


  while(test_state);

  test_state = 0;

  /* examine the results */

  /* sample_results(pResults->element.attribs.pData); */
}

