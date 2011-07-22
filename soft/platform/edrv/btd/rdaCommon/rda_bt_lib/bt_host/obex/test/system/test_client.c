    
/*
  test file for obex_server_interface.c
 */

#include "host_config.h"

#include "host_types.h"
#include "host_error.h"
#include "obex_client_types.h"
#include "obex_client_interface.h"
#include "obex_utils.h"
#include "obex_defines.h"
#include "obex_sync_client.h"

#include "mgr_utils.h"

#include "rfcomm_types.h"
#include "stack_ent_api.h"
#include "obex_tal.h"
#include "papi.h"
#include "mgr_core.h"

#include "sdp_ue.h"
#include "sdp_constants.h"

#include "vcc.h" 

int linux_sys_close;

int linux_sys_mode;


int waiter;

void tester(void);
void obex_search(t_bdaddr address,u_int8* channel,u_int8* types);

void main(void)
{
  linux_sys_mode = 1;
/*
  sys_init(linux_sys_mode);
*/
   startHost();

  while(MGR_Init(0) == BT_RETRY);

  RFCOMM_SE_Init();

  tester();

  while(1) Sleep(500);
}

void service(u_int8 status, t_DataBuf *data)
{
  u_int8 test;

  test = 1; 

  printf("Response to request");

	waiter = 0;
}


void tester(void)
{
	t_bdaddr address;
	u_int8 server_channel;
	u_int16 offset;
	u_int32 tid_s;
	void* cb;
	int ret;
	u_int8* data;
	u_int8 target[4];
	u_int8 types;
	
	t_DataBuf* buff;
	
	buff =  host_buf_alloc(60);
	host_buf_reserve_header(buff,20);
	
	server_channel = 5;
	cb = 0x00;
	
	buff->buf[0] = 'a';
	buff->buf[1] = 'b';
	buff->buf[2] = 'c';
	buff->buf[3] = 'd';
	buff->buf[4] = 'e';
	
	waiter = 1;
	
	MGR_ParseHexBDAddress("0xdbdbdbdb0001",&address);
	pDebugCheck();



//	obex_search(address,&server_channel,&types);

	OBEX_FindSyncServer(&address,&server_channel,&types);

	pDebugCheck();

	OBEX_TransportConnect_RF(address,server_channel, &tid_s,0x00);
	
	pDebugCheck();

	waiter = 1;


	OBEX_Connect(tid_s,23,01,100,0,0x00,0,buff,service);
	pDebugCheck();

	while(waiter) Sleep(500);
	
	buff =  host_buf_alloc(60);
	host_buf_reserve_header(buff,20);

	waiter = 1;
	
	OBEX_Disconnect(tid_s,0,buff,service);
	pDebugCheck();

	while(waiter) Sleep(500);
	
	buff =  host_buf_alloc(300);
	host_buf_reserve_header(buff,20);
	waiter = 1;

	offset = 3;

	data = malloc(10);

	data[0] = 0;
	data[1] = 'M';
	data[2] = 0;
	data[3] = 'a';
	data[4] = 0;
	data[5] = 'r';
	data[6] = 0;
	data[7] = 'k';
	data[8] = 0;
	data[9] = 0;

	OBEX_CreateStringHeader(OBEX_NAME,4,data,buff,&offset);

	free(data);

	data = malloc(14);

	strcpy(data,"text/x-vCard");

	OBEX_CreateSequenceHeader(OBEX_TYPE,12,data,buff,&offset);

	free(data);

	data = malloc(500);

	pDebugCheck();

	{
		VObject* pCard;
		int len;

	char sample[300];

	strcpy(sample,"BEGIN:VCARD\r\nFN:Mr Mark G Donnelly Esq\r\nN:Donnelly;Mark;G;Mr;Esq\r\nBDAY:1973-05-27\r\nTEL;WORK:508125\r\nTITLE:Technologies\r\nORG:Parthus\r\nEND:VCARD\r\n");

	len = strlen(sample);

	pCard = Parse_MIME(sample,len);

	len = 500;
	

/*	pCard = newVObject("mark");*/
		

	writeMemVObject(data,&len,pCard);
	
	pDebugCheck();

	OBEX_CreateSequenceHeader(OBEX_BODY,len,data,buff,&offset);

	}

	pDebugCheck();

	free(data);

	OBEX_Put(tid_s,0,offset,buff,service);
	pDebugCheck();

	while(waiter) Sleep(500);
	buff =  host_buf_alloc(60);
	host_buf_reserve_header(buff,20);
	waiter = 1;
	
	OBEX_Put(tid_s,1,0,buff,service);
	pDebugCheck();
	
	while(waiter) Sleep(500);
	
	buff =  host_buf_alloc(250);
	host_buf_reserve_header(buff,20);
	waiter = 1;
	
	offset = 3;
	data = malloc(12);
	data[0] = 0;
	data[1] = 'T';
	data[2] = 0;
	data[3] = 'e';
	data[4] = 0;
	data[5] = 's';
	data[6] = 0;
	data[7] = 't';
	data[8] = 0;
	data[9] = '!';
	data[10] = 0;
	data[11] = 0;

	pDebugCheck();

	OBEX_CreateStringHeader(OBEX_NAME,5,data,buff,&offset);

	free(data);

	pDebugCheck();

	OBEX_Get(tid_s,0,offset,buff,service);
	
	while(waiter) Sleep(500);
	
	buff =  host_buf_alloc(60);
	host_buf_reserve_header(buff,20);
	waiter = 1;
	
	OBEX_Get(tid_s,1,0,buff,service);
	
	while(waiter) Sleep(500);

	OBEX_TransportDisconnect(tid_s);
}


void test_finish_callback(int status,void* test)
{
	waiter = 0;
}



/*

u_int8 get_list_length(u_int8* data,u_int32 *len,u_int8* offset)
{
	u_int8* internal;
	
	internal = data;
	
	switch(*internal)
	{
	case SDP_SEQUENCE_SMALL:
	case SDP_UNION_SMALL:
	case SDP_URL_SMALL:
	case SDP_TEXT_SMALL:
		*len = internal[1];
		*offset = SDP_SMALL_HEADER;
		break;
	case SDP_SEQUENCE_MEDIUM:
	case SDP_UNION_MEDIUM:
	case SDP_URL_MEDIUM:
	case SDP_TEXT_MEDIUM:
		*len = (internal[1] << 8) + internal[2];
		*offset = SDP_MEDIUM_HEADER;
		break;
	case SDP_SEQUENCE_LARGE:
	case SDP_UNION_LARGE:
	case SDP_URL_LARGE:
	case SDP_TEXT_LARGE:
		*len = (internal[1] << 24) + (internal[2] << 16) + (internal[3] << 8) + internal[4];
		*offset = SDP_LARGE_HEADER;
		break;
	default:
		return BT_INVALIDPARAM;
	}
	return BT_NOERROR;
}

*/
/*
u_int8 read_uuid(u_int8* data,u_int32* uuid,u_int8* offset)
{
	switch(*data)
	{
	case SDP_UUID_2:
		*uuid = (data[1] << 8) + data[2];
		*offset = 3;
		break;
	case SDP_UUID_4:
		*uuid = (data[1] << 24) + (data[2] << 16) + (data[3] << 8) + data[4];
		*offset = 5;
		break;
	case SDP_UUID_16:
		*uuid = (data[1] << 24) + (data[2] << 16) + (data[3] << 8) + data[4];
		*offset = 17;
		break;
	default:
		return BT_INVALIDPARAM;
	}
	return BT_NOERROR;
}


*/

void obex_search(t_bdaddr address,u_int8* channel,u_int8* types)
{
  u_int32 uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;

  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;
  u_int8* finger;
  u_int8 counter;
  u_int16 aid;
  u_int8 offset;
  u_int32 length;
  u_int32 outer;
  u_int32 inner;
  uuid = 0x1105; /* obex */

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

  attribs.numItems = 3;
  attribs.attributeIDs = malloc(12);
  attribs.attributeIDs[0] = 0x00000004;
  attribs.attributeIDs[1] = 0x00000100;
  attribs.attributeIDs[2] = 0x00000303;

  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 60;
  rule.maxBytes = 250;

  waiter = 1;

  /* call a service search and do not retrieve the device name */

  SDP_ServiceBrowse(&pResults,&devices,&pattern,&attribs,&rule,criteria,test_finish_callback);    

  while(waiter) Sleep(500);
 
	waiter = 0;

  /* examine the results */

  /* sample_results(pResults->element.attribs.pData); */

	*types = 0;
	
	pResults->element.dataLen;

	finger = pResults->element.pData;

	get_list_length(finger,&length,&offset);
	finger+= offset;
	get_list_length(finger,&length,&offset);
	finger+= offset;
	counter = length;
	do {
		counter--;
		finger++; /* bypass type code */
		aid = (finger[0] << 8) + finger[1];
		finger+=2;
		counter-=2;
		switch(aid)
		{
		case SDP_AID_PROTOCOL:
			get_list_length(finger,&length,&offset);
			finger += offset;
			counter -= offset;
			outer = length;
			do {
				do {
					get_list_length(finger,&length,&offset);
					finger +=offset;
					counter -=offset;
					outer -=offset;
					inner = length;
					read_uuid(finger,&uuid,&offset);
					finger += offset;
					counter -= offset;
					outer -= offset;
					inner -= offset;
					if(uuid == SDP_PROTOCOL_UUID_RFCOMM && inner)
					{
						*channel = finger[1];
						finger+=2;
						counter-=2;
						outer-=2;
					} else {
						finger += inner;
						counter -= inner;
						outer -= inner;
						inner = 0;
					}					
				} while(inner);
			} while (outer);
			break;
		case SDP_AID_DEFAULT_LANGUAGE + SDP_AID_SERVICE_NAME_OFFSET:
			get_list_length(finger,&length,&offset);
			finger += offset;
			counter -= offset;
			if(length)
			{
				finger += length;
				counter -= length;
			}
			break;
		case SDP_AID_FORMATS_LIST:
			get_list_length(finger,&length,&offset);
			finger += offset;
			counter -= offset;
			if(length)
			{
				do {
					if(finger[1] == 0xff)
					{
						*types += 1 << 6;
					} else {
						*types += 1 << (finger[1]-1);
					}
					finger +=2;
					counter -=2;
					length -=2;
				} while(length);
			}
			break;
		default:
			/* I will do this another time */
			break;
		}
	} while (counter);
}

