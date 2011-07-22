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


/*#include <unistd.h>*/


#include "l2cap_primitives.h"
#define WIN_TEST 1


#if WIN_TEST

#include "winbase.h"



void sleep(u_int32 dur)

{

	Sleep(dur);

}



void startHost(void);



#endif






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
u_int16 test_mtu = 70;
t_bdaddr test_address;



void (*test_procedure)(u_int16 cid,struct host_buf * p_buf);



void Service_Discovery_Protocol(void);

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


int main(int argc, char **argv)
{


#if !WIN_TEST



  linux_sys_mode=1;


  sys_init(linux_sys_mode); /* server */
  
  fprintf(stderr,"starting the stack\n");
#else

//	startHost();

#endif

	while(MGR_Init(0) == BT_RETRY);


/*	test_address = prh_mgr_readHexBDAddress("0x00d0b7031728"); */
/* test_address = prh_mgr_readHexBDAddress("0x00803713bb52");*/
 test_address = prh_mgr_readHexBDAddress("0xDBDBDBDB0001");
 

/* test_1_wrapper(0,0);
*/
   Service_Discovery_Protocol(); 
 

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
  pDebugPrintf((pLOGDEBUG," <<<<<<<<< SDP Received from peer "));


pDebugDump(pLOGDEBUG,p_buf->data,p_buf->len);
//  sdp_print_buffer(p_buf->data,p_buf->len);



	test_procedure(cid,p_buf);


  host_buf_free(p_buf);
}


void conn_test()
{
  t_bdaddr address;
  l2_callbacks cbs;
 
  address = test_address;
  
  cbs.connectionIndication = prh_sdp_l2cap_connect_ind_cb;
  cbs.configureIndication = prh_sdp_l2cap_config_ind_cb;
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
      pData[3] = search_pattern >> 8; 
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
                                         
  *******************************************************************************
  *******************************************************************************
  
  SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
  SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS

 ********************************************************************************/



void TP_SERVER_SS_BV_01_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1105;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SS_BV_01_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SS_BV_01_C;
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
      test_state = 0;
      L2_Disconnect(cid);

      break;      
    default:
      /* use this like a java final */
      test_state = 0;
      break;
    }
}



void TP_SERVER_SS_BV_02_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  int total_len;
  
  search_pattern = 0x1105;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SS_BV_02_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SS_BV_02_C;
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
      
      
      /*
    second one
      */
      
    h_buff = host_buf_alloc(200+10);
    host_buf_reserve_header(h_buff,10);
    
      
    pData = h_buff->data;
      
	/*
      pData++;
  */	    
      total_len = pdu_len+5;

      *pData++ = 0x02;
      *pData++ = 0x00;
      *pData++ = test_state + 1;
      pData +=2; 

      search_pattern = 0x1103;

      i = test_pattern(pData,search_pattern);

      if(i == -1)
    return;

      pData += i;
      pdu_len = i;
      
      *pData++ = 0x00;
      *pData++ = 0x60;
      
      *pData = 0x00;

      pdu_len +=3;
      
      pData[-pdu_len -1] = pdu_len >> 8;
      pData[-pdu_len] = pdu_len & 0xff;
      

	  /*
      total_len += pdu_len +5;
*/

	  total_len = pdu_len + 5;


      h_buff->len = total_len;
      L2_Write(cid,(t_DataBuf*)h_buff);
      
      break;
    case 2: 
      test_state++;
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

void TP_SERVER_SS_BV_03_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 current_count;
  struct host_buf* h_buff;
  u_int8* pData;
  u_int8* ptemp;
  int i;
  int pdu_len;
  
  search_pattern = 0x0100;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SS_BV_03_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SS_BV_03_C;
      test_mtu = 20;
      conn_test();
      break;
    case 1:
      test_state++;

      test_mtu = 70;

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
      *pData++ = 0x08;
      
      *pData = 0x00;

      pdu_len +=3;
      
      h_buff->data[3] = pdu_len >> 8;
      h_buff->data[4] = pdu_len & 0xff;
      
      h_buff->len = 5 + pdu_len;
      L2_Write(cid,(t_DataBuf*)h_buff);
      break;
    case 2:
      test_state++;
      // check the contents;
      ptemp = p_buf->data;
      
      ptemp += 7;
      current_count = *ptemp++<<8;
      current_count += *ptemp++;
      
      ptemp += current_count*4;
      
      if(*ptemp)
    {
      
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
      *pData++ = 0x08;
      
      for(i = 0 ; i < (*ptemp) + 1 ; i++)
        {
          pData[i] = ptemp[i];
        }

      pdu_len += 2 + i;
      
      h_buff->data[3] = pdu_len >> 8;
      h_buff->data[4] = pdu_len & 0xff;
      
      h_buff->len = 5 + pdu_len;
      L2_Write(cid,(t_DataBuf*)h_buff);
      
      
    } else {
      test_state = 0;
      L2_Disconnect(cid);
    }
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


void TP_SERVER_SS_BV_04_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0xf105;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SS_BV_04_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SS_BV_04_C;
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
      test_state = 0;
      L2_Disconnect(cid);
      break;      
    default:
      /* use this like a java final */
      test_state = 0;
      break;
    }

}


void TP_SERVER_SS_BI_01_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1105;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SS_BI_01_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SS_BI_01_C;
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
      
      // set the parameter len to be invalid
      
      h_buff->data[3] = 0x34;

      
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

void TP_SERVER_SS_BI_02_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1105;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SS_BI_02_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SS_BI_02_C;
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

      // set an invalid syntax
      
      pData[0] = 0x02;
      
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
                                         
  *******************************************************************************
  *******************************************************************************
  
  SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
  AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

 ********************************************************************************/



void TP_SERVER_SA_BV_01_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0001;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BV_01_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BV_01_C;
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

void TP_SERVER_SA_BV_02_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0001;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BV_02_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BV_02_C;
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
      L2_Write(cid,(t_DataBuf*)h_buff);

      // second one 

      attribute = 0x0002;

      h_buff = host_buf_alloc(200+10);
      host_buf_reserve_header(h_buff,10);
      

	  pdu_len = 0;


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
      L2_Write(cid,(t_DataBuf*)h_buff);

      break;
    case 3:      
      test_state++;  
      break;

	case 4:

		test_state = 0;

		L2_Disconnect(cid);
    default:
      /* use this like a java final */
      test_state = 0;
      break;
    }
}


void TP_SERVER_SA_BV_03_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  u_int8* p_temp;

static	u_int8 t_buff[4];


  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0100;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BV_03_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BV_03_C;
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


		t_buff[0] = p_buf->data[9];

		t_buff[1] = p_buf->data[10];

		t_buff[2] = p_buf->data[11];

		t_buff[3] = p_buf->data[12];




      *pData++ = 0x00;
      *pData++ = 0x16;



	  pdu_len += 6;

      test_attrib(pData,attribute);
      
      pData += 5;
      pdu_len += 5;
            
      *pData = 0x00;

      pdu_len +=1;
      
      h_buff->data[3] = pdu_len >> 8;
      h_buff->data[4] = pdu_len & 0xff;
      
      h_buff->len = 5 + pdu_len;
      L2_Write(cid,(t_DataBuf*)h_buff);
      break;
    case 3:
      /*test_state++;*/

      p_temp = p_buf->data;

      search_pattern = (p_temp[5] >> 8) + p_temp[6];
      
      if( (p_temp[3] >> 8) + p_temp[4] > search_pattern + 3 )
	{
	  p_temp += 7 + search_pattern;

       h_buff = host_buf_alloc(200+10);
       host_buf_reserve_header(h_buff,10);
      
      pData = h_buff->data;
      
      *pData++ = 0x04;
      *pData++ = 0x00;
      *pData++ = test_state;
      pData +=2; /* move past the length field */           

      *pData++ = t_buff[0];
      *pData++ = t_buff[1];
      *pData++ = t_buff[2];
      *pData++ = t_buff[3];

      *pData++ = 0x00;
      *pData++ = 0x16;

	

	  pdu_len += 6;

      test_attrib(pData,attribute);
      
      pData += 5;
      pdu_len += 5;
            
      for(i = 0; i < (*p_temp) + 1; i++)
	{
	  pData[i] = p_temp[i];
	}

      pdu_len += i;

      /*
      *pData = 0x00;

      pdu_len +=1;
      */
      h_buff->data[3] = pdu_len >> 8;
      h_buff->data[4] = pdu_len & 0xff;
      
      h_buff->len = 5 + pdu_len;


      L2_Write(cid,(t_DataBuf*)h_buff);


	} else {
	  test_state = 0;	  
	  L2_Disconnect(cid);
	}
      break;
    default:
      /* use this like a java final */
      test_state = 0;
      break;
    }
}


void TP_SERVER_SA_BV_04_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0003;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BV_04_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BV_04_C;
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

void TP_SERVER_SA_BV_05_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0004;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BV_05_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BV_05_C;
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

void TP_SERVER_SA_BV_06_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0002;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BV_06_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BV_06_C;
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

void TP_SERVER_SA_BV_07_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0007;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BV_07_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BV_07_C;
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

void TP_SERVER_SA_BV_08_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0005;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BV_08_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BV_08_C;
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

void TP_SERVER_SA_BV_09_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0006;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BV_09_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BV_09_C;
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

void TP_SERVER_SA_BV_10_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0008;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BV_10_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BV_10_C;
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

void TP_SERVER_SA_BV_11_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x000c;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BV_11_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BV_11_C;
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

void TP_SERVER_SA_BV_12_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0100;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BV_12_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BV_12_C;
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
      
      /* store the handle */

      handle[0] =  p_buf->data[9];
      handle[1] =  p_buf->data[10];
      handle[2] =  p_buf->data[11];
      handle[3] =  p_buf->data[12];

      *pData++ = 0x00;
      *pData++ = 0x60;

      pdu_len += 6;
      
      test_attrib(pData,0x0006);
      
      pData += 5;
      pdu_len += 5;
            
      *pData = 0x00;

      pdu_len +=1;
      
      h_buff->data[3] = pdu_len >> 8;
      h_buff->data[4] = pdu_len & 0xff;
      
      h_buff->len = 5 + pdu_len;
      L2_Write(cid,(t_DataBuf*)h_buff);
      break;
    case 3:
      test_state++;
      
      h_buff = host_buf_alloc(200+10);
      host_buf_reserve_header(h_buff,10);
      
      pData = h_buff->data;
      
      *pData++ = 0x04;
      *pData++ = 0x00;
      *pData++ = test_state;
      pData +=2; /* move past the length field */           

      /* use the same handle as before */

      *pData++ = handle[0];
      *pData++ = handle[1];
      *pData++ = handle[2];
      *pData++ = handle[3];

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
      L2_Write(cid,(t_DataBuf*)h_buff);
      break;

    case 4:      
      test_state = 0;
      L2_Disconnect(cid);

      break;      
    default:
      /* use this like a java final */
      test_state = 0;
      break;
    }
}



void TP_SERVER_SA_BV_13_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0101;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BV_13_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BV_13_C;
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
      
      /* store the handle */

      handle[0] =  p_buf->data[9];
      handle[1] =  p_buf->data[10];
      handle[2] =  p_buf->data[11];
      handle[3] =  p_buf->data[12];

      *pData++ = 0x00;
      *pData++ = 0x60;

      pdu_len += 6;
      
      test_attrib(pData,0x0006);
      
      pData += 5;
      pdu_len += 5;
            
      *pData = 0x00;

      pdu_len +=1;
      
      h_buff->data[3] = pdu_len >> 8;
      h_buff->data[4] = pdu_len & 0xff;
      
      h_buff->len = 5 + pdu_len;
      L2_Write(cid,(t_DataBuf*)h_buff);
      break;
    case 3:
      test_state++;
      
      h_buff = host_buf_alloc(200+10);
      host_buf_reserve_header(h_buff,10);
      
      pData = h_buff->data;
      
      *pData++ = 0x04;
      *pData++ = 0x00;
      *pData++ = test_state;
      pData +=2; /* move past the length field */           

      /* use the same handle as before */

      *pData++ = handle[0];
      *pData++ = handle[1];
      *pData++ = handle[2];
      *pData++ = handle[3];

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
      L2_Write(cid,(t_DataBuf*)h_buff);
      break;

    case 4:      
      test_state = 0;
      L2_Disconnect(cid);

      break;      
    default:
      /* use this like a java final */
      test_state = 0;
      break;
    }
}

void TP_SERVER_SA_BV_14_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0102;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BV_14_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BV_12_C;
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
      
      /* store the handle */

      handle[0] =  p_buf->data[9];
      handle[1] =  p_buf->data[10];
      handle[2] =  p_buf->data[11];
      handle[3] =  p_buf->data[12];

      *pData++ = 0x00;
      *pData++ = 0x60;

      pdu_len += 6;
      
      test_attrib(pData,0x0006);
      
      pData += 5;
      pdu_len += 5;
            
      *pData = 0x00;

      pdu_len +=1;
      
      h_buff->data[3] = pdu_len >> 8;
      h_buff->data[4] = pdu_len & 0xff;
      
      h_buff->len = 5 + pdu_len;
      L2_Write(cid,(t_DataBuf*)h_buff);
      break;
    case 3:
      test_state++;
      
      h_buff = host_buf_alloc(200+10);
      host_buf_reserve_header(h_buff,10);
      
      pData = h_buff->data;
      
      *pData++ = 0x04;
      *pData++ = 0x00;
      *pData++ = test_state;
      pData +=2; /* move past the length field */           

      /* use the same handle as before */

      *pData++ = handle[0];
      *pData++ = handle[1];
      *pData++ = handle[2];
      *pData++ = handle[3];

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
      L2_Write(cid,(t_DataBuf*)h_buff);
      break;

    case 4:      
      test_state = 0;
      L2_Disconnect(cid);

      break;      
    default:
      /* use this like a java final */
      test_state = 0;
      break;
    }
}



void TP_SERVER_SA_BV_15_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1000;
  attribute = 0x0200;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BV_15_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BV_15_C;
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

void TP_SERVER_SA_BV_16_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1000;
  attribute = 0x0201;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BV_16_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BV_16_C;
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


void TP_SERVER_SA_BV_17_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0009;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BV_17_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BV_17_C;
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

void TP_SERVER_SA_BV_18_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x000a;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BV_18_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BV_18_C;
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

void TP_SERVER_SA_BV_19_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x000b;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BV_19_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BV_19_C;
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

void TP_SERVER_SA_BV_20_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0f03;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BV_20_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BV_20_C;
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







void TP_SERVER_SA_BI_01_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0003;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BI_01_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BI_01_C;
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
      *pData++ = p_buf->data[11] + 0x0f;  /* make the record handle invalid */
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


void TP_SERVER_SA_BI_02_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0003;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BI_02_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BI_02_C;
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
      
      pData[0] = 0xff; /* set the list of attributes to be invalid */

      pData += 5;
      pdu_len += 5;
            
      *pData = 0x00;

      pdu_len +=1;
      
      h_buff->data[3] = pdu_len >> 8;
      h_buff->data[4] = pdu_len & 0xff;
      
      h_buff->len = 5 + pdu_len;
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



void TP_SERVER_SA_BI_03_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0004;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SA_BI_03_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SA_BI_03_C;
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
      h_buff->data[4] = (pdu_len & 0xff);
      
      h_buff->data[3] = 23; /* set invalid length */

      h_buff->len = 5 + pdu_len;
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




/*
                                         
  *******************************************************************************
  *******************************************************************************
  
  SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
  SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
  AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

 ********************************************************************************/


void TP_SERVER_SSA_BV_01_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1f07;
  attribute = 0x0003;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_01_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_01_C;
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

void TP_SERVER_SSA_BV_02_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0f03;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_02_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_02_C;
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



void TP_SERVER_SSA_BV_03_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x110f;
  attribute = 0x0f03;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_03_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_03_C;
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

void TP_SERVER_SSA_BV_04_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0003;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_04_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_04_C;
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


void TP_SERVER_SSA_BV_05_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0003;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_05_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_05_C;
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
      L2_Write(cid,(t_DataBuf*)h_buff);

      // second one

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

void TP_SERVER_SSA_BV_06_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  u_int16 temp_val;
  struct host_buf* h_buff;
  u_int8* pData;

  u_int8* p_temp;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0100;
  pdu_len = 0;
  
  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_06_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_06_C;
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
      *pData++ = 0x12;
      
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

		p_temp = p_buf->data;



		temp_val = (p_temp[5] >> 8) + p_temp[6];



		if((p_temp[3] >> 8) + p_temp[4] > temp_val + 3)

		{



			p_temp += 7 + temp_val;

			

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

			*pData++ = 0x12;

			

			pdu_len += 2;

			

			test_attrib(pData,attribute);

			

			pData += 5;

			pdu_len += 5;

			

			for(i =0; i< (*p_temp) + 1; i++)
			  {
			    pData[i] = p_temp[i];
			  }
			/*
			*pData = 0x00;

			

		       
			pdu_len++;

			*/
			pdu_len += i;

			

			h_buff->data[3] = pdu_len >> 8;

			h_buff->data[4] = pdu_len & 0xff;

			

			h_buff->len = 5 + pdu_len;

			L2_Write(cid,(t_DataBuf*)h_buff);

		} else {

						
				test_state = 0;
			L2_Disconnect(cid);
		}
		break;      
    default:
      /* use this like a java final */
      test_state = 0;
      break;
    }
}

void TP_SERVER_SSA_BV_07_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0002;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_07_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_07_C;
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

void TP_SERVER_SSA_BV_08_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1000;
  attribute = 0x0201;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_08_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_08_C;
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

void TP_SERVER_SSA_BV_09_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0007;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_09_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_09_C;
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

void TP_SERVER_SSA_BV_10_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0003;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_10_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_10_C;
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


void TP_SERVER_SSA_BV_11_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0004;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_11_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_11_C;
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


void TP_SERVER_SSA_BV_12_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0005;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_12_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_12_C;
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


void TP_SERVER_SSA_BV_13_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0006;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_13_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_13_C;
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


void TP_SERVER_SSA_BV_14_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0008;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_14_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_14_C;
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


void TP_SERVER_SSA_BV_15_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x000c;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_15_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_15_C;
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


void TP_SERVER_SSA_BV_16_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0100;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_16_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_16_C;
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

      test_attrib(pData,0x0006);

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

void TP_SERVER_SSA_BV_17_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0101;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_17_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_17_C;
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

      test_attrib(pData,0x0006);

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


void TP_SERVER_SSA_BV_18_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0102;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_18_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_18_C;
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

      test_attrib(pData,0x0006);

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

void TP_SERVER_SSA_BV_19_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1000;
  attribute = 0x0200;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_19_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_19_C;
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

void TP_SERVER_SSA_BV_20_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0009;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_20_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_20_C;
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


void TP_SERVER_SSA_BV_21_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x000a;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_21_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_21_C;
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

void TP_SERVER_SSA_BV_22_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x000b;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BV_22_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BV_22_C;
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





void TP_SERVER_SSA_BI_01_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0003;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BI_01_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BI_01_C;
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

      pData[0] = 0xff; /* invalidate the syntax */

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


void TP_SERVER_SSA_BI_02_C(u_int16 cid,struct host_buf * p_buf)
{
  u_int16 search_pattern;
  u_int16 attribute;
  struct host_buf* h_buff;
  u_int8* pData;
  int i;
  int pdu_len;
  
  search_pattern = 0x1107;
  attribute = 0x0003;
  pdu_len = 0;

  switch(test_state )
    {
    case 0:
      /* initial state */
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      pDebugPrintf((pLOGDEBUG,"-- TP_SERVER_SSA_BI_02_C"));
      pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
      test_state++;
      test_procedure = TP_SERVER_SSA_BI_02_C;
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
      
      h_buff->data[3] = 23; /* invalidate the length field */
      
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
                                         
  *******************************************************************************
  *******************************************************************************
  
  BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
  RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
  WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW

 ********************************************************************************/


void TP_SERVER_BRW_BV_02_C(void)
{
  u_int32 application_id;
  t_SDP_Results* pResults;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_AttributeIDs attribs;
  t_SDP_StopRule rule;
  t_bdaddr address;

  criteria = SDP_TRUSTED;

  SDP_OpenSearch(); /* set up callbacks */

  application_id = 5;

  rule.maxItems = 10;
  rule.maxDuration = 0xff;
  rule.maxBytes = 2000;
 
  address = test_address;
  /*  address = prh_mgr_readHexBDAddress("0x0080371221ce"); */
  devices.numItems = 1;
  devices.addresses = malloc(sizeof(t_bdaddr) * 3);  
  devices.addresses[0] = address;
  devices.addresses[1] = address;
  devices.addresses[2] = address;

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(4);
  attribs.attributeIDs[0] = 0x0100;
  
  test_state = 1;

  SDP_GenericBrowse(&pResults,&devices,&attribs,&rule,criteria,test_finish_callback);

  while(test_state);

  test_state = 0;
}


/*
  *******************************************************************************
  *******************************************************************************

  IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
  NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
  QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ
  
  
 ********************************************************************************/

void TP_INQ_BV_01_I(void)
{
  t_SDP_StopRule rule;
  t_classDevice cod;
  t_SDP_Addresses* pResults;
  u_int32 iac;

  iac = 0;

  SDP_OpenSearch();
  
  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  pDebugPrintf((pLOGDEBUG,"-- TP_INQ_BV_01_I")); 
  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));

  rule.maxItems = 10;
  rule.maxDuration = 60;
  rule.maxBytes = 200;



  cod = 0x00;
  
  SDP_EnumerateDevices(&pResults,cod,iac,&rule,0x00,0x00);    
}


void TP_INQ_BV_02_I(void)
{
  t_SDP_StopRule rule;
  t_classDevice cod;
  t_SDP_Addresses* pResults;
  u_int32 iac;

  iac = 0;
  SDP_OpenSearch();
  
  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  pDebugPrintf((pLOGDEBUG,"-- TP_INQ_BV_02_I")); 
  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));

  rule.maxItems = 10;
  rule.maxDuration = 60;



  cod = 0x00;
  
  SDP_EnumerateDevices(&pResults,cod,iac,&rule,0x00,0x00);    
}

/*
  *******************************************************************************
  *******************************************************************************

  NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
  AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
  DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
  
 ********************************************************************************/




void TP_NAD_BV_01_I(void)
{
  u_int32 serial_uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;
  
  address= test_address;
  serial_uuid = 0x1101;

  /* bd address list to search for */

  devices.numItems = 1;
  devices.addresses = malloc(sizeof(t_bdaddr));  
  devices.addresses[0] = address;

  /* device relation criteria */

  criteria = SDP_TRUSTED;

 /* search pattern */

  pattern.numItems = 1;
  pattern.patternUUIDs = malloc(4);
  pattern.patternUUIDs[0] = serial_uuid;

  /* attributes to retrieve */

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(2);
  attribs.attributeIDs[0] = 0x0004; /* protocol decriptor list */
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;

  /* call a service search and do not retrieve the device name */

  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  pDebugPrintf((pLOGDEBUG,"-- TP_NAD_BV_01_I")); 
  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  

  SDP_ServiceSearch(&pResults,&devices,&pattern,&attribs,&rule,criteria,0x00);
  
  pResults = 0x00;
}



/*
  *******************************************************************************
  *******************************************************************************

  LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
  SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
  EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
  
 ********************************************************************************/


void TP_LSE_BV_01_I(void)
{
  u_int32 serial_uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;
  
  address= test_address;
  serial_uuid = 0x1101;

  /* bd address list to search for */

  devices.numItems = 1;
  devices.addresses = malloc(sizeof(t_bdaddr));  
  devices.addresses[0] = address;

  /* device relation criteria */

  criteria = SDP_TRUSTED;

 /* search pattern */

  pattern.numItems = 1;
  pattern.patternUUIDs = malloc(4);
  pattern.patternUUIDs[0] = serial_uuid;

  /* attributes to retrieve */

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(2);
  attribs.attributeIDs[0] = 0x0004; /* protocol decriptor list */
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;

  /* call a service search and do not retrieve the device name */

  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  pDebugPrintf((pLOGDEBUG,"-- TP_LSE_BV_01_I")); 
  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  

  SDP_ServiceSearch(&pResults,&devices,&pattern,&attribs,&rule,criteria,0x00);
  
  pResults = 0x00;
}

void TP_LSE_BV_02_I(void)
{
  u_int32 serial_uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;
  
  address= test_address;
  serial_uuid = 0x1101;

  /* bd address list to search for */

  devices.numItems = 2;
  devices.addresses = malloc(sizeof(t_bdaddr) * 2);  
  devices.addresses[0] = address;
  devices.addresses[1] = address;

  /* device relation criteria */

  criteria = SDP_TRUSTED;


 /* search pattern */

  pattern.numItems = 1;
  pattern.patternUUIDs = malloc(4);
  pattern.patternUUIDs[0] = serial_uuid;

  /* attributes to retrieve */

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(2);
  attribs.attributeIDs[0] = 0x0004; /* protocol decriptor list */
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;

  /* call a service search and do not retrieve the device name */

  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  pDebugPrintf((pLOGDEBUG,"-- TP_LSE_BV_02_I")); 
  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  

  SDP_ServiceSearch(&pResults,&devices,&pattern,&attribs,&rule,criteria,0x00);
  
  pResults = 0x00;
}


void TP_LSE_BV_03_I(void)
{
  u_int32 serial_uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;
  
  address= test_address;
  serial_uuid = 0x1101;

  /* bd address list to search for */

  devices.numItems = 1;
  devices.addresses = malloc(sizeof(t_bdaddr));  
  devices.addresses[0] = address;

  /* device relation criteria */

  criteria = SDP_ZERO_PIN;

 /* search pattern */

  pattern.numItems = 1;
  pattern.patternUUIDs = malloc(4);
  pattern.patternUUIDs[0] = serial_uuid;

  /* attributes to retrieve */

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(2);
  attribs.attributeIDs[0] = 0x0004; /* protocol decriptor list */
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;

  /* call a service search and do not retrieve the device name */

  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  pDebugPrintf((pLOGDEBUG,"-- TP_LSE_BV_03_I")); 
  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  

  SDP_ServiceSearch(&pResults,&devices,&pattern,&attribs,&rule,criteria,0x00);

  pResults = 0x00;
}


void TP_LSE_BV_04_I(void)
{
  u_int32 serial_uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;
  
  address= test_address;
  serial_uuid = 0x1101;

  /* bd address list to search for */

  devices.numItems = 2;
  devices.addresses = malloc(sizeof(t_bdaddr) * 2);  
  devices.addresses[0] = address;
  devices.addresses[1] = address;

  /* device relation criteria */

  criteria = SDP_ZERO_PIN;

 /* search pattern */

  pattern.numItems = 1;
  pattern.patternUUIDs = malloc(4);
  pattern.patternUUIDs[0] = serial_uuid;

  /* attributes to retrieve */

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(2);
  attribs.attributeIDs[0] = 0x0004; /* protocol decriptor list */
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;

  /* call a service search and do not retrieve the device name */

  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  pDebugPrintf((pLOGDEBUG,"-- TP_LSE_BV_04_I")); 
  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  

  SDP_ServiceSearch(&pResults,&devices,&pattern,&attribs,&rule,criteria,0x00);
  
  pResults = 0x00;
}


void TP_LSE_BV_05_I(void)
{
  u_int32 serial_uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;
  
  address= test_address;
  serial_uuid = 0x1101;

  /* bd address list to search for */

  devices.numItems = 1;
  devices.addresses = malloc(sizeof(t_bdaddr));  
  devices.addresses[0] = address;

  /* device relation criteria */

  criteria = SDP_NORMAL_PIN;


 /* search pattern */

  pattern.numItems = 1;
  pattern.patternUUIDs = malloc(4);
  pattern.patternUUIDs[0] = serial_uuid;

  /* attributes to retrieve */

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(2);
  attribs.attributeIDs[0] = 0x0004; /* protocol decriptor list */
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;

  /* call a service search and do not retrieve the device name */

  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  pDebugPrintf((pLOGDEBUG,"-- TP_LSE_BV_05_I")); 
  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  

  SDP_ServiceSearch(&pResults,&devices,&pattern,&attribs,&rule,criteria,0x00);
  
  pResults = 0x00;
}

void TP_LSE_BV_06_I(void)
{
  u_int32 serial_uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;
  
  address= test_address;
  serial_uuid = 0x1101;

  /* bd address list to search for */

  devices.numItems = 2;
  devices.addresses = malloc(sizeof(t_bdaddr) * 2);  
  devices.addresses[0] = address;
  devices.addresses[1] = address;

  /* device relation criteria */

  criteria = SDP_NORMAL_PIN;


 /* search pattern */

  pattern.numItems = 1;
  pattern.patternUUIDs = malloc(4);
  pattern.patternUUIDs[0] = serial_uuid;

  /* attributes to retrieve */

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(2);
  attribs.attributeIDs[0] = 0x0004; /* protocol decriptor list */
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;

  /* call a service search and do not retrieve the device name */

  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  pDebugPrintf((pLOGDEBUG,"-- TP_LSE_BV_06_I")); 
  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  

  SDP_ServiceSearch(&pResults,&devices,&pattern,&attribs,&rule,criteria,0x00);
  
  pResults = 0x00;
}

/*
  *******************************************************************************
  *******************************************************************************

  CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
  OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
  NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
  
 ********************************************************************************/



/* will need to put in some manager code here to sort this out. */



















/*
  *******************************************************************************
  *******************************************************************************

  SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
  SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
  
 ********************************************************************************/
  

void TP_SS_BV_01_I(void)
{
  u_int32 serial_uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;
  
  address= test_address;
  serial_uuid = 0x1101;

  /* bd address list to search for */

  devices.numItems = 1;
  devices.addresses = malloc(sizeof(t_bdaddr));  
  devices.addresses[0] = address;

  /* device relation criteria */

  criteria = SDP_NORMAL_PIN;

 /* search pattern */

  pattern.numItems = 1;
  pattern.patternUUIDs = malloc(4);
  pattern.patternUUIDs[0] = serial_uuid;

  /* attributes to retrieve */

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(2);
  attribs.attributeIDs[0] = 0x0004; /* protocol decriptor list */
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;

  /* call a service search and do not retrieve the device name */

  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  pDebugPrintf((pLOGDEBUG,"-- TP_SS_BV_01_I")); 
  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  
  
  SDP_ServiceSearch(&pResults,&devices,&pattern,&attribs,&rule,criteria,0x00);
  
  pResults = 0x00;
}

/*
  *******************************************************************************
  *******************************************************************************

  SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
  AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
  
 ********************************************************************************/


void TP_SA_BV_01_I(void)
{
  u_int32 serial_uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;
  
  address= test_address;
  serial_uuid = 0x1101;

  /* bd address list to search for */

  devices.numItems = 1;
  devices.addresses = malloc(sizeof(t_bdaddr));  
  devices.addresses[0] = address;

  /* device relation criteria */

  criteria = SDP_NORMAL_PIN;

 /* search pattern */

  pattern.numItems = 1;
  pattern.patternUUIDs = malloc(4);
  pattern.patternUUIDs[0] = serial_uuid;

  /* attributes to retrieve */

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(2);
  attribs.attributeIDs[0] = 0x0004; /* protocol decriptor list */
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;

  /* call a service search and do not retrieve the device name */

  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  pDebugPrintf((pLOGDEBUG,"-- TP_SA_BV_01_I")); 
  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  

  SDP_ServiceSearch(&pResults,&devices,&pattern,&attribs,&rule,criteria,0x00);
  
  pResults = 0x00;
}

/*
  *******************************************************************************
  *******************************************************************************

  SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
  SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
  AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
  
 ********************************************************************************/


void TP_SSA_BV_01_I(void)
{
  u_int32 serial_uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern group;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;
  
  address= test_address;
  serial_uuid = 0x1101;

  /* bd address list to search for */

  devices.numItems = 1;
  devices.addresses = malloc(sizeof(t_bdaddr));  
  devices.addresses[0] = address;

  /* device relation criteria */

  criteria = SDP_NORMAL_PIN;


 /* search pattern */

  group.numItems = 1;
  group.patternUUIDs = malloc(4);
  group.patternUUIDs[0] = serial_uuid;

  /* attributes to retrieve */

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(2);
  attribs.attributeIDs[0] = 0x0004; /* protocol decriptor list */
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;

  /* call a service search and do not retrieve the device name */

  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  pDebugPrintf((pLOGDEBUG,"-- TP_SSA_BV_01_I")); 
  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  

  SDP_ServiceBrowse(&pResults,&devices,&group, &attribs, &rule,criteria,0x00);
  pResults = 0x00;
}



/*
  *******************************************************************************
  *******************************************************************************

  BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
  RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
  WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
  
 ********************************************************************************/

void TP_BRW_BV_01_I(void)
{
  u_int32 serial_uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern group;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;
  
  address= test_address;
  serial_uuid = 0x1101;

  /* bd address list to search for */

  devices.numItems = 1;
  devices.addresses = malloc(sizeof(t_bdaddr));  
  devices.addresses[0] = address;

  /* device relation criteria */

  criteria = SDP_NORMAL_PIN;


 /* search pattern */

  group.numItems = 1;
  group.patternUUIDs = malloc(4);
  group.patternUUIDs[0] = serial_uuid;

  /* attributes to retrieve */

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(2);
  attribs.attributeIDs[0] = 0x0004; /* protocol decriptor list */
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;

  /* call a service search and do not retrieve the device name */

  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  pDebugPrintf((pLOGDEBUG,"-- TP_BRW_BV_01_I")); 
  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  

  SDP_ServiceBrowse(&pResults,&devices, &group, &attribs, &rule,criteria,0x00);
  pResults = 0x00;
}


/*
  *******************************************************************************
  *******************************************************************************

  TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
  MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
  NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
  
 ********************************************************************************/


void TP_TMN_BV_01_I(void)
{
  u_int32 serial_uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;
  
  address= test_address;
  serial_uuid = 0x1101;

  /* bd address list to search for */

  devices.numItems = 1;
  devices.addresses = malloc(sizeof(t_bdaddr));  
  devices.addresses[0] = address;

  /* device relation criteria */

  criteria = SDP_NORMAL_PIN;

 /* search pattern */

  pattern.numItems = 1;
  pattern.patternUUIDs = malloc(4);
  pattern.patternUUIDs[0] = serial_uuid;

  /* attributes to retrieve */

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(2);
  attribs.attributeIDs[0] = 0x0004; /* protocol decriptor list */
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;

  /* call a service search and do not retrieve the device name */

  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  pDebugPrintf((pLOGDEBUG,"-- TP_TMN_BV_01_I")); 
  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  

  SDP_ServiceSearch(&pResults,&devices,&pattern,&attribs,&rule,criteria,0x00);
  
  pResults = 0x00;
}


void TP_TMN_BV_02_I(void)
{
  u_int32 serial_uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;
  
  address= test_address;
  serial_uuid = 0x1101;

  /* bd address list to search for */

  devices.numItems = 2;
  devices.addresses = malloc(sizeof(t_bdaddr) * 2);  
  devices.addresses[0] = address;
  devices.addresses[1] = address;

  /* device relation criteria */

  criteria = SDP_NORMAL_PIN;

 /* search pattern */

  pattern.numItems = 1;
  pattern.patternUUIDs = malloc(4);
  pattern.patternUUIDs[0] = serial_uuid;

  /* attributes to retrieve */

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(2);
  attribs.attributeIDs[0] = 0x0004; /* protocol decriptor list */
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;

  /* call a service search and do not retrieve the device name */

  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  pDebugPrintf((pLOGDEBUG,"-- TP_TMN_BV_02_I")); 
  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));
  

  SDP_ServiceSearch(&pResults,&devices,&pattern,&attribs,&rule,criteria,0x00);
  
  pResults = 0x00;
}









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
  attribs.attributeIDs = malloc(2);
  attribs.attributeIDs[0] = 0x0004; /* protocol decriptor list */
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 15;
  rule.maxBytes = 250;

  /* call a service search and do not retrieve the device name */

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
  attribs.attributeIDs = malloc(2);
  attribs.attributeIDs[0] = 0x0100;
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;
  rule.maxBytes = 250;

  /* call a service search and do not retrieve the device name */

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
  attribs.attributeIDs[0] = 0x0004;

  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;

  /* call a service search and do not retrieve the device name */

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
  attribute = 0x0009;
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
  attribs.attributeIDs = malloc(1);
  attribs.attributeIDs[0] = 0x0004;

  
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


void test_of_browse(void)
{
  u_int32 application_id;
  t_SDP_Results* pResults;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_AttributeIDs attribs;
  t_SDP_StopRule rule;
  t_bdaddr address;

  criteria = SDP_TRUSTED;


  application_id = 5;

  rule.maxItems = 10;
  rule.maxDuration = 60;
  rule.maxBytes = 250;
 
  address =  test_address;
  /*  address = prh_mgr_readHexBDAddress("0x0080371221ce"); */
  devices.numItems = 1;
  devices.addresses = malloc(sizeof(t_bdaddr) * 3);  
  devices.addresses[0] = address;
  devices.addresses[1] = address;
  devices.addresses[2] = address;

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(4);
  attribs.attributeIDs[0] = 0x0004;
  
  test_state = 1;

  SDP_GenericBrowse(&pResults,&devices,&attribs,&rule,criteria,test_finish_callback);


  while(test_state);

  test_state = 0;
}


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
  
  search_pattern = 0x1002;
  attribute = 0x0100;
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


void data_base_tester(void)
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
  uuid = 0x2323;

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

  test_state = 1;

  /* call a service search and do not retrieve the device name */

  SDP_ServiceBrowse(&pResults,&devices,&pattern,&attribs,&rule,criteria,test_finish_callback);    

  while(test_state);

  test_state = 0;

  /* examine the results */

  /* sample_results(pResults->element.attribs.pData); */
}






/*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */







void Service_Discovery_Protocol(void)

{



  test_mode = 2;

    

  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));

  pDebugPrintf((pLOGDEBUG,"-- Service Discovery Protocol Tests"));

  pDebugPrintf((pLOGDEBUG,"--------------------------------------------------------------------------------"));

  



  TP_SERVER_SS_BV_01_C(0,0);

  

  while(test_state);



  TP_SERVER_SS_BV_02_C(0,0);

  

  while(test_state);

  
  TP_SERVER_SS_BV_03_C(0,0);



  while(test_state);


  TP_SERVER_SS_BV_04_C(0,0);



  while(test_state);

  



  TP_SERVER_SS_BI_01_C(0,0);



  while(test_state);



  TP_SERVER_SS_BI_02_C(0,0);

  

  while(test_state);



  

  sleep(5);



  TP_SERVER_SA_BV_01_C(0,0);

  while(test_state);

  

  TP_SERVER_SA_BV_02_C(0,0);

  while(test_state);



  TP_SERVER_SA_BV_03_C(0,0);

  while(test_state);



  TP_SERVER_SA_BV_04_C(0,0);

  while(test_state);

 

  TP_SERVER_SA_BV_05_C(0,0);

  while(test_state);

 

  TP_SERVER_SA_BV_06_C(0,0);

  while(test_state);

  

  TP_SERVER_SA_BV_07_C(0,0);

  while(test_state);

  

  TP_SERVER_SA_BV_08_C(0,0);

  while(test_state);

  

  TP_SERVER_SA_BV_09_C(0,0);

  while(test_state);

  

  TP_SERVER_SA_BV_10_C(0,0);

  while(test_state);

  

  TP_SERVER_SA_BV_11_C(0,0);

  while(test_state);

  

  TP_SERVER_SA_BV_12_C(0,0);

  while(test_state);



  TP_SERVER_SA_BV_13_C(0,0);

  while(test_state);

  

  TP_SERVER_SA_BV_14_C(0,0);

  while(test_state);

  

  TP_SERVER_SA_BV_15_C(0,0);

  while(test_state);

  

  TP_SERVER_SA_BV_16_C(0,0);

  while(test_state);

  

  TP_SERVER_SA_BV_17_C(0,0);

  while(test_state);

  

  TP_SERVER_SA_BV_18_C(0,0);

  while(test_state);

  

  TP_SERVER_SA_BV_19_C(0,0);

  while(test_state);

  

  TP_SERVER_SA_BV_20_C(0,0);

  while(test_state); 



  



  TP_SERVER_SA_BI_01_C(0,0);

  while(test_state);



  TP_SERVER_SA_BI_02_C(0,0);

  while(test_state);



  TP_SERVER_SA_BI_03_C(0,0);

  while(test_state);


  sleep(5);

/*
  TP_SERVER_SSA_BV_01_C(0,0);

  while(test_state);

  

  TP_SERVER_SSA_BV_02_C(0,0);

  while(test_state);

  

  TP_SERVER_SSA_BV_03_C(0,0);

  while(test_state);

  

  TP_SERVER_SSA_BV_04_C(0,0);

  while(test_state);

  

  TP_SERVER_SSA_BV_05_C(0,0);

  while(test_state);

 
  

  TP_SERVER_SSA_BV_06_C(0,0);

  while(test_state);


  TP_SERVER_SSA_BV_07_C(0,0);

  while(test_state);
  
  

  TP_SERVER_SSA_BV_08_C(0,0);

  while(test_state);


  TP_SERVER_SSA_BV_09_C(0,0);

  while(test_state);

  

  TP_SERVER_SSA_BV_10_C(0,0);

  while(test_state);

  

  TP_SERVER_SSA_BV_11_C(0,0);

  while(test_state);

  

  TP_SERVER_SSA_BV_12_C(0,0);

  while(test_state);

  

  TP_SERVER_SSA_BV_13_C(0,0);

  while(test_state);

  

  TP_SERVER_SSA_BV_14_C(0,0);

  while(test_state);

  

  TP_SERVER_SSA_BV_15_C(0,0);

  while(test_state);

  
  

  TP_SERVER_SSA_BV_16_C(0,0);

  while(test_state);
  

  TP_SERVER_SSA_BV_17_C(0,0);

  while(test_state);

  
  TP_SERVER_SSA_BV_18_C(0,0);

  while(test_state);

  
  TP_SERVER_SSA_BV_19_C(0,0);

  while(test_state);

  

  TP_SERVER_SSA_BV_20_C(0,0);
 
  while(test_state);

  

  TP_SERVER_SSA_BV_21_C(0,0);

  while(test_state);

  

  TP_SERVER_SSA_BV_22_C(0,0);

  while(test_state);

  





  TP_SERVER_SSA_BI_01_C(0,0);

  while(test_state);





  TP_SERVER_SSA_BI_02_C(0,0);

  while(test_state);



  sleep(5);
*/


/*  

  TP_SERVER_BRW_BV_02_C();

*/ 
  pDebugPrintf((pLOGDEBUG," Finished "));

}





void test_1(u_int16 cid)
{
	t_SDP_LowLevelResults* pResults;
	t_SDP_SearchPattern pPattern;
	u_int16 maxCount;
	u_int8* Continue;
	u_int8 timeout;
	sdp_api_callback callback;

	maxCount = 250;
	Continue = 0x00;
	pPattern.numItems = 1;
	pPattern.patternUUIDs = malloc(4);
	pPattern.patternUUIDs[0] = 0x00001103;

	timeout = 0xff;


	callback = 0x00;

	SDP_ServiceSearchRequest(&pResults,cid,&pPattern,maxCount,Continue,timeout,callback);


}

void test_1_wrapper(u_int16 cid,struct host_buf * p_buf)
{  
  switch(test_state )
    {
    case 0:
      test_state++;
      test_procedure = test_1_wrapper;
      conn_test();
      break;
    case 1:
      SDP_OpenSearch();
      
      test_1(cid);
    }
}
