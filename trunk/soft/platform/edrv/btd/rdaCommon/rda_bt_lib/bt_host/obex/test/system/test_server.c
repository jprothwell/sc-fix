    
/*
  test file for obex_server_interface.c
 */

#include "host_config.h"

#include "host_types.h"
#include "host_error.h"
#include "obex_defines.h"
#include "obex_server_types.h"
#include "obex_server_interface.h"
#include "obex_client_types.h"
#include "obex_sync_server.h"


#include "rfcomm_types.h"
#include "stack_ent_api.h"
#include "obex_tal.h"
#include "papi.h"
#include "mgr_core.h"
#include "host_buf.h"

#include "sdp_server.h"


int linux_sys_close;

int linux_sys_mode;

extern struct prh_obex_transport* prh_obex_transmit_transport;
extern struct prh_obex_session_state obex_state;
int prh_obex_allocate_cid(struct prh_obex_service_registration* registration);

void setup_obex_db(u_int8 channel,char* ServiceName,u_int8 formats);

void tester(void);

void main(void)
{
  linux_sys_mode = 0;
	
  startHost();

  while(MGR_Init(0) == BT_RETRY);

  /*sys_init(linux_sys_mode);*/

  RFCOMM_SE_Init();

  tester();
	
	
  while(1) Sleep(500);

}

void service(u_int32 tid, u_int8 messageType, t_obex_headers headers, t_DataBuf *data,void* cbref)
{
  t_DataBuf* buf;
  u_int8 a;
  a = 100;

	switch(messageType)
	{
	case 0x80:
			printf("Received a Connect message\n");
	break;
	case 0x81:
			printf("Received a Disconnect message\n");
		break;
	case 0x02:
			printf("Received a Put message\n");
		break;
	case 0x82:
			printf("Received a Final Put message\n");
		break;
	case 0x03:
			printf("Received a Get message\n");
		break;
	case 0x83:
			printf("Received a Final Get message\n");
		break;
	case 0x85:
			printf("Received a SetPath message\n");
		break;
	case 0xff:
			printf("Received a Abort message\n");
		break;
	default:
		printf("Received a message of type 0x%x from the Client\n",messageType);
		break;
	}



	buf = host_buf_alloc(60);
	host_buf_reserve_header(buf,20);
	OBEX_Response(tid,OBEX_OK,0x0,buf);
}


void tester(void)
{
  u_int8 server_channel;
  u_int32 tid;
  void* cb;
  int ret;
  u_int8 target[4];
  struct prh_obex_session_state obex_state;
  t_DataBuf buff;

  server_channel = 5;
  cb = 0x00;
  

	//setup_obex_db(server_channel,"Test Obex Server",1+2+4+16);

	OBEX_RegisterSyncServer(server_channel, 1+2);

  ret = OBEX_TransportListen_RF(server_channel,&tid,cb);

  pDebugCheck();

/*  OBEX_RegisterService(tid,service,4,target,0x00);
*/

	OBEX_RegisterInbox(tid,service,0x00);

  pDebugCheck();

}


void setup_obex_db(u_int8 channel,char* ServiceName,u_int8 formats)
{
  u_int32 handle;
  u_int8 lcv,count;

  u_int16 _16bit;
  u_int8 _8bit;
  
  t_SDP_serviceRecord *pRecord;
  t_SDP_dataElement e1,e2,e3,e4;

  t_SDP_dataElement fmt[8];
  
  t_SDP_dataElement* list[10];
  
  handle = 0x00010002;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  _16bit = 0x1105;
  
  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e1);
  

  list[0] = &e1;
  
  SDP_CreateList(SDP_SEQUENCE,1,list,&e3);

  SDP_AddAttribute(pRecord,0x0001,&e3);
  
  /* protocol descriptor list */

  _16bit = 0x0100;
  
  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e1);
  
  list[0] = &e1;
  
  SDP_CreateList(SDP_SEQUENCE,1,list,&e2);

  _16bit = 0x0003;

  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e1);
  
  _8bit = channel;
  
  SDP_EncodeAttribute(SDP_UINT,1,&_8bit,&e3);
  
  list[0] = &e1;
  list[1] = &e3;

  SDP_CreateList(SDP_SEQUENCE,2,list,&e4);
  
  _16bit = 0x0008;

  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e1);

  list[0] = &e1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&e3);

  list[0] = &e2;
  list[1] = &e4;
  list[2] = &e3;

  SDP_CreateList(SDP_SEQUENCE,3,list,&e1);
  
  SDP_AddAttribute(pRecord,0x0004,&e1);

  /* language base */

  _16bit = ('e' << 8) + 'n';

  SDP_EncodeAttribute(SDP_UINT,2,&_16bit,&e1);

  _16bit = 106;

  SDP_EncodeAttribute(SDP_UINT,2,&_16bit,&e2);
   
  _16bit = 0x0100;
  
  SDP_EncodeAttribute(SDP_UINT,2,&_16bit,&e3);
  
  list[0] = &e1;
  list[1] = &e2;
  list[2] = &e3;

  SDP_CreateList(SDP_SEQUENCE,3,list,&e4);

  SDP_AddAttribute(pRecord,0x0006,&e4);

  /* profile - list */

  _16bit = 0x1105;
  
  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e1);

  _16bit = 0x0100;

  SDP_EncodeAttribute(SDP_UINT,2,&_16bit,&e2);

  list[0] = &e1;
  list[1] = &e2;
  
  SDP_CreateList(SDP_SEQUENCE,2,list,&e3);
  
  list[0] = &e3;

  SDP_CreateList(SDP_SEQUENCE,1,list,&e2);

  SDP_AddAttribute(pRecord,0x0009,&e2);

  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,strlen(ServiceName),ServiceName,&e1);

  SDP_AddAttribute(pRecord,0x0100,&e1);

  /* supported formats */
  if(formats == 0xff)
  {
	_8bit = 0xff;
	SDP_EncodeAttribute(SDP_UINT,1,&_8bit,fmt);
	list[0] = fmt;
	count = 1;
  } else {
	  
	  for(count = 0,lcv = 0;lcv < 7; lcv++,formats >>=1)
	  {
		  if(formats & 1)
		  {
			  _8bit = lcv + 1;		
			  SDP_EncodeAttribute(SDP_UINT,1,&_8bit,fmt + count);
			  list[count] = fmt + count;
			  count++;
		  }
	  }
  }
	SDP_CreateList(SDP_SEQUENCE,count,list,&e1);

	SDP_AddAttribute(pRecord,0x0303,&e1);
}

