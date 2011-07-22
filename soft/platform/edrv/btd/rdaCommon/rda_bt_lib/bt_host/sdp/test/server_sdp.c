/*
  This is a sample server program that is to be used with all tests.
 */

#include "host_types.h"
#include "host_buf.h"
#include "stdio.h"
#include "mgr_core.h"
#include "mgr_utils.h"
#include "sdp_db.h"
#include "sdp_server.h"
#include "mgr_core.h"
#include "hci_primitives.h"

#define SERV_TCP_PORT 6542
#define SERV_HOST_ADDR "127.0.0.1"
#define BD_ADDR_SERVER  "MASTER"
#define BD_ADDR_CLIENT "SLAVE "

t_bdaddr bd_addr_serv;
t_bdaddr bd_addr_cli;
t_bdaddr bd_addr_test;
void sigio_handler(int signum);

int linux_sys_close; 

int linux_sys_mode;

 void test(t_api d)
{
	/* */
}

void Test_Data_Base_Setup(void);

void upf_3_dialup(void);

void upf_3_fax(void);

void upf_3_obex(void);

void Synchronisation(void);

void create_browse_tree(void);





#define WIN_DB 1



int main(int argc, char **argv)
{
  int a;
#if !WIN_DB



  linux_sys_mode=0;
	
  sys_init(linux_sys_mode);

	

#else

//	startHost();

#endif
  
  fprintf(stderr,"starting the stack\n");
  

  /*  prh_mgr_initStack(argc,argv); */ 
  MGR_Init(0);
  MGR_SetSecurityMode(MGR_NOSECURITY, NULL);
  HCI_WriteScanEnable(3);
  /*
    MGR_SetConnectableMode(MGR_CONNECTABLE,test);
    a = MGR_SetDiscoverableMode(MGR_GENERALDISCOVERY,test);
  */
  

  pDebugCheck();
  
  Test_Data_Base_Setup();
  

  pDebugCheck();
  SDP_OpenSearch();
  
  fprintf(stderr,"SDP server up and running.\n");

  while(1)
    {

#if WIN_DB

		Sleep(1000);

#else
		pDebugSleep(1);

#endif
    }
  
}

void Test_Data_Base_Setup(void)
{
   pDebugCheck();
  upf_3_dialup();

  pDebugCheck();
  upf_3_fax();

  pDebugCheck();
  upf_3_obex();

  pDebugCheck();

  
  Synchronisation();
  pDebugCheck();

  

  create_browse_tree(); 

    pDebugCheck();


}


void upf_3_dialup(void)
{
  u_int32 handle;

  u_int16 _16bit;
  u_int8 _8bit;
  
  t_SDP_serviceRecord *pRecord;
  t_SDP_dataElement e1,e2,e3,e4;
  
  t_SDP_dataElement* list[10];
  
  char Service_name[] = "Dial-up networking";

  handle = 0x00010000;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  _16bit = 0x1103;
  
  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e1);
  
  _16bit = 0x1201;
  
  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e2);

  list[0] = &e1;
  list[1] = &e2;
  
  SDP_CreateList(SDP_SEQUENCE,2,list,&e3);

  SDP_AddAttribute(pRecord,0x0001,&e3);
  
  /* protocol descriptor list */

  _16bit = 0x0100;
  
  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e1);
  
  list[0] = &e1;
  
  SDP_CreateList(SDP_SEQUENCE,1,list,&e2);

  _16bit = 0x0003;

  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e1);
  
  _8bit = 0x01;
  
  SDP_EncodeAttribute(SDP_UINT,1,&_8bit,&e3);
  
  list[0] = &e1;
  list[1] = &e3;

  SDP_CreateList(SDP_SEQUENCE,2,list,&e4);
  
  list[0] = &e2;
  list[1] = &e4;

  SDP_CreateList(SDP_SEQUENCE,2,list,&e1);
  
  SDP_AddAttribute(pRecord,0x0004,&e1);

  /* browse group */

  _16bit = 0x1002;
  
  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e1);
  
  list[0] = &e1;

  SDP_CreateList(SDP_SEQUENCE,2,list,&e2);

  SDP_AddAttribute(pRecord,0x0005,&e2);

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

  _16bit = 0x1103;
  
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

  SDP_EncodeAttribute(SDP_TEXT,sizeof(Service_name) -1,Service_name,&e1);

  SDP_AddAttribute(pRecord,0x0100,&e1);
}





void upf_3_fax(void)
{
  u_int32 handle;

  u_int16 _16bit;
  u_int8 _8bit;
  
  t_SDP_serviceRecord *pRecord;
  t_SDP_dataElement e1,e2,e3,e4;
  
  t_SDP_dataElement* list[10];
  
  char Service_name[] = "Fax";

  handle = 0x00010001;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  _16bit = 0x1111;
  
  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e1);
  
  _16bit = 0x1204;
  
  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e2);

  list[0] = &e1;
  list[1] = &e2;
  
  SDP_CreateList(SDP_SEQUENCE,2,list,&e3);

  SDP_AddAttribute(pRecord,0x0001,&e3);
  
  /* protocol descriptor list */

  _16bit = 0x0100;
  
  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e1);
  
  list[0] = &e1;
  
  SDP_CreateList(SDP_SEQUENCE,1,list,&e2);

  _16bit = 0x0003;

  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e1);
  
  _8bit = 0x02;
  
  SDP_EncodeAttribute(SDP_UINT,1,&_16bit,&e3);
  
  list[0] = &e1;
  list[1] = &e3;

  SDP_CreateList(SDP_SEQUENCE,2,list,&e4);
  
  list[0] = &e2;
  list[1] = &e4;

  SDP_CreateList(SDP_SEQUENCE,2,list,&e1);
  
  SDP_AddAttribute(pRecord,0x0004,&e1);

  /* browse group */

  _16bit = 0x1002;
  
  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e1);
  
  list[0] = &e1;

  SDP_CreateList(SDP_SEQUENCE,2,list,&e2);

  SDP_AddAttribute(pRecord,0x0005,&e2);

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

  _16bit = 0x1111;
  
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

  SDP_EncodeAttribute(SDP_TEXT,sizeof(Service_name) -1,Service_name,&e1);

  SDP_AddAttribute(pRecord,0x0100,&e1);
}





void upf_3_obex(void)
{
  u_int32 handle;

  u_int16 _16bit;
  u_int8 _8bit;
  
  t_SDP_serviceRecord *pRecord;
  t_SDP_dataElement e1,e2,e3,e4;
  
  t_SDP_dataElement* list[10];
  
  char Service_name[] = "OBEX Object Push";

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
  
  _8bit = 0x01;
  
  SDP_EncodeAttribute(SDP_UINT,1,&_16bit,&e3);
  
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

  /* browse group */

  _16bit = 0x1002;
  
  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e1);
  
  list[0] = &e1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&e2);

  SDP_AddAttribute(pRecord,0x0005,&e2);

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

  SDP_EncodeAttribute(SDP_TEXT,sizeof(Service_name) -1,Service_name,&e1);

  SDP_AddAttribute(pRecord,0x0100,&e1);
}


void Synchronisation()
{
  t_SDP_dataElement* list[10];

  t_SDP_dataElement element1,element2,element3,element4;

  u_int32 _32bit;
  u_int16 _16bit;
  u_int8  _8bit;
  t_SDP_serviceRecord *pRecord;
  u_int32 handle;


  char* _url= "http://www.foo.org/docs";
  char* _exe_url = "http://www.foo.org/exe/*/test";
  char* _icon_url = "http://www.foo.org/icons/*";
  char* _service_name = "synchronisation";
  char* _description = "Well do a sync I think!!";
  char* _provider = "Acme.";


  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);


  /* 0x0001 */

  _16bit = 0x1107;
  element1.type = SDP_UUID;
  element1.size = 1;  
  element1.data_length = 2;

  prh_sdp_encode_data(&element1,&_16bit);  
  
  list[0] = &element1;
  prh_sdp_encode_list(&element2,1,list);

  prh_sdp_add(pRecord,0x0001,&element2);
  
  /* 0x0002 */
  /*
    this is created automaticaly
   */

  /* 0x0003 */
  
  _16bit = 0x4321;
  
  element1.type = SDP_UUID;
  element1.size = 1;
  element1.data_length = 2;
  
  prh_sdp_encode_data(&element1,&_16bit);
  
  prh_sdp_add(pRecord,0x0003,&element1);

  /* 0x0004 */

  _16bit = 0x0100;

  element1.type = SDP_UUID;
  element1.size = 1;
  element1.data_length = 2;
  prh_sdp_encode_data(&element1,&_16bit);
  
  list[0] = &element1;

  prh_sdp_encode_list(&element3,1,list);

  _16bit = 0x0003;
  element1.type = SDP_UUID;
  element1.size = 1;
  element1.data_length = 2;
  prh_sdp_encode_data(&element1,&_16bit);
  
  _8bit = 23;
  
  element2.type = SDP_UINT;
  element2.size = 0;
  element2.data_length = 1;

  prh_sdp_encode_data(&element2,&_8bit);

  list[0] = &element1;
  list[1] = &element2;

  prh_sdp_encode_list(&element4,2,list);

  list[0] = &element3;
  list[1] = &element4;
  
  prh_sdp_encode_list(&element1,2,list);

  prh_sdp_add(pRecord,0x0004,&element1);
  
  /* 0x0005 */

  _16bit = 0x1002;
  
  element1.type = SDP_UUID;
  element1.size = 1;
  element1.data_length = 2;

  prh_sdp_encode_data(&element1,&_16bit);
  
  list[0] = &element1;

  prh_sdp_encode_list(&element2,1,list);

  prh_sdp_add(pRecord,0x0005,&element2);

  /* 0x0006 */

  _16bit = ('e' << 8) + 'n';

  element1.type = SDP_UINT;
  element1.size = 1;
  element1.data_length = 2;
  
  prh_sdp_encode_data(&element1,&_16bit);
  
  _16bit = 106;

  element2.type = SDP_UINT;
  element2.size = 1;
  element2.data_length = 2;
  
  prh_sdp_encode_data(&element2,&_16bit);
  
  _16bit = 0x0100;
  
  element3.type = SDP_UINT;
  element3.size = 1;
  element3.data_length  = 2;

  prh_sdp_encode_data(&element3,&_16bit);
  
  list[0] = &element1;
  list[1] = &element2;
  list[2] = &element3;

  prh_sdp_encode_list(&element4,3,list);

  prh_sdp_add(pRecord,0x0006,&element4);

  /* 0x0007 */

  _32bit = 0x00000060;
  
  element1.type = SDP_UINT;
  element1.size = 2;
  element1.data_length = 4;
  
  prh_sdp_encode_data(&element1,&_32bit);

  prh_sdp_add(pRecord,0x0007,&element1);
  
  /* 0x0008 */

  _8bit = 0xff;

  element1.type = SDP_UINT;
  element1.size = 0;
  element1.data_length = 1;
  
  prh_sdp_encode_data(&element1,&_8bit);

  prh_sdp_add(pRecord,0x0008,&element1);

  /* 0x0009 */

  _16bit = 0x0100;
  
  element1.type = SDP_UINT;
  element1.size = 1;
  element1.data_length = 2;
  
  prh_sdp_encode_data(&element1,&_16bit);
  
  _16bit = 0x1104;
  
  element2.type = SDP_UUID;
  element2.size = 1;
  element2.data_length = 2;

  prh_sdp_encode_data(&element2,&_16bit);
  
  list[0] = &element2;
  list[1] = &element1;
  
  prh_sdp_encode_list(&element3,2,list);
  
  list[0] = &element3;
  
  prh_sdp_encode_list(&element1,1,list);
  
  prh_sdp_add(pRecord,0x0009,&element1);
      
  /* 0x000a */

  element1.type = SDP_URL;
  element1.size = 5;
  element1.data_length = strlen(_url);

  prh_sdp_encode_data(&element1,_url);
  
  prh_sdp_add(pRecord,0x000a,&element1);

  /* 0x000b */

  element1.type = SDP_URL;
  element1.size = 5;
  element1.data_length = strlen(_exe_url);

  prh_sdp_encode_data(&element1,_exe_url);

  prh_sdp_add(pRecord,0x000b,&element1);

  /* 0x000c */
  
  element1.type = SDP_URL;
  element1.size = 5;
  element1.data_length = strlen(_icon_url);
  
  prh_sdp_encode_data(&element1,_icon_url);

  prh_sdp_add(pRecord,0x000c,&element1);

  /* 0x0100 + 0 */

  element1.type = SDP_TEXT;
  element1.size = 5;
  element1.data_length = strlen(_service_name);

  prh_sdp_encode_data(&element1,_service_name);

  prh_sdp_add(pRecord,0x0100,&element1);

  /* 0x0100 + 1 */

  element1.type = SDP_TEXT;
  element1.size = 5;
  element1.data_length = strlen(_description);
  
  prh_sdp_encode_data(&element1,_description);

  prh_sdp_add(pRecord,0x0101,&element1);
  
  /* 0x0100 + 2 */
  
  element1.type = SDP_TEXT;
  element1.size = 5;
  element1.data_length = strlen(_provider);

  prh_sdp_encode_data(&element1,_provider);

  prh_sdp_add(pRecord,0x0102,&element1);
  
  /* 0x0301 */

  _8bit = 0x05;

  element1.type = SDP_UINT;
  element1.size = 0;
  element1.data_length = 1;
  
  prh_sdp_encode_data(&element1,&_8bit);
  
  list[0] = &element1;
  
  prh_sdp_encode_list(&element2,1,list);

  prh_sdp_add(pRecord,0x0301,&element2);
    


}



/*
  This function will create a database entry for the serial port profile

  It contains a:
  Service Class list,
  Protocol Descriptor list,
  Language base list,
  and Service name.

  for a description of these attributes see the service attribute definitions
  section of the service discovery protocol specification.
 */
void sample_setup(void)
{
  u_int8 channel;
  u_int32 handle;
  u_int16 serial_uuid,l2cap_uuid,rfcomm_uuid,language_id,language_encoding,language_base;
  t_SDP_serviceRecord *pRecord;
  t_SDP_dataElement SerialPortUUID,ServiceClassList,L2CAP_UUID,Protocol0,RFCOMM_UUID,RFCOMM_CHANNEL,Protocol1;
  t_SDP_dataElement ProtocolDescriptorList,Serial_Name,Lang_id,Lang_Enc,Lang_base,Lang_list_1,Lang_base_list;
  t_SDP_dataElement* list[10];
  
  char* Service_name = "Serial_user_a";

  handle = 16;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  serial_uuid = 0x1101; /* serial port uuid */
  SDP_EncodeAttribute(SDP_UUID,2,&serial_uuid,&SerialPortUUID);

  /* place the uuid in a list */
  
  list[0] = &SerialPortUUID;
  SDP_CreateList(SDP_SEQUENCE,1,list,&ServiceClassList);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0001,&ServiceClassList);

  /* protocol descriptor list */


  /* construct the l2cap protocol entry */
  
  l2cap_uuid = 0x0100;
  SDP_EncodeAttribute(SDP_UUID,2,&l2cap_uuid,&L2CAP_UUID);

  /* place l2cap uuid in a list */

  list[0] = &L2CAP_UUID;
  SDP_CreateList(SDP_SEQUENCE,1,list,&Protocol0);
  
  
  /* construct the rfcomm entry */

  rfcomm_uuid = 0x0003;
  SDP_EncodeAttribute(SDP_UUID,2,&rfcomm_uuid,&RFCOMM_UUID);

  channel = 0x02; /* setting of the RFCOMM channel number */
  SDP_EncodeAttribute(SDP_UINT,1,&channel,&RFCOMM_CHANNEL);

  /* place rfcomm uuid and chanel number in a list */

  list[0] = &RFCOMM_UUID;
  list[1] = &RFCOMM_CHANNEL;
  SDP_CreateList(SDP_SEQUENCE,2,list,&Protocol1);
  
  /* place protocol 0 and protocol 1 in a list */

  list[0] = &Protocol0;
  list[1] = &Protocol1;
  
  SDP_CreateList(SDP_SEQUENCE,2,list,&ProtocolDescriptorList);
  
  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0004,&ProtocolDescriptorList);
  

  /* language base list */
  
  /* language identifier english */

  language_id = ('e' << 8) + 'n';

  SDP_EncodeAttribute(SDP_UINT,2,&language_id,&Lang_id);

  /* language encoding  utf-8 */

  language_encoding = 106; 
  
  SDP_EncodeAttribute(SDP_UINT,2,&language_encoding,&Lang_Enc);

  /* language base */

  language_base = 0x0100;
  
  SDP_EncodeAttribute(SDP_UINT,2,&language_base,&Lang_base);
  
  /* place the entries in a list */

  list[0] = &Lang_id;
  list[1] = &Lang_Enc;
  list[2] = &Lang_base;

  SDP_CreateList(SDP_SEQUENCE,3,list,&Lang_list_1);

  /* add this to the main list */

  list[0] = &Lang_list_1;
  
  SDP_CreateList(SDP_SEQUENCE,1,list,&Lang_base_list);

  /* add this as an attribute to the service record */
  
  SDP_AddAttribute(pRecord,0x0006,&Lang_base_list);

  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,sizeof(Service_name),Service_name,&Serial_Name);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,language_base + 0,&Serial_Name);
      
}



void create_browse_tree(void)
{
  /*
    see page 339 of spec.
   */

  u_int32 handle;
  u_int16 uuid;
  t_SDP_serviceRecord *pRecord;
  t_SDP_dataElement SerialPortUUID,BrowseGroupList,ServiceClassList,BGL1;
  t_SDP_dataElement Serial_Name;
  t_SDP_dataElement* list[10];
  
  char* Service_name[20];

  strcpy(Service_name,"Entertainment");

  handle = 200;

  handle++;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  uuid = 0x1001; /* serial port uuid */
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&SerialPortUUID);

  /* place the uuid in a list */
  
  list[0] = &SerialPortUUID;
  SDP_CreateList(SDP_SEQUENCE,1,list,&ServiceClassList);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0001,&ServiceClassList);

  /* browse group list */
  
  uuid = 0x1002;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  /*
  uuid = 0x1001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  */

  list[0] = &BGL1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&BrowseGroupList);

  SDP_AddAttribute(pRecord,0x0005,&BrowseGroupList);
  
  /* browse group descriptor */

  uuid = 0xe001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);

  SDP_AddAttribute(pRecord,0x0200,&BGL1);

  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,strlen(Service_name),Service_name,&Serial_Name);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0100,&Serial_Name);


  strcpy(Service_name,"News");

  handle++;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  uuid = 0x1001; /* serial port uuid */
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&SerialPortUUID);

  /* place the uuid in a list */
  
  list[0] = &SerialPortUUID;
  SDP_CreateList(SDP_SEQUENCE,1,list,&ServiceClassList);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0001,&ServiceClassList);

  /* browse group list */
  
  uuid = 0x1002;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  /*
  uuid = 0x1001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  */

  list[0] = &BGL1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&BrowseGroupList);

  SDP_AddAttribute(pRecord,0x0005,&BrowseGroupList);
  
  /* browse group descriptor */

  uuid = 0xe002;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);

  SDP_AddAttribute(pRecord,0x0200,&BGL1);

  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,strlen(Service_name),Service_name,&Serial_Name);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0100,&Serial_Name);

  strcpy(Service_name,"Reference");

  handle++;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  uuid = 0x1001; /* serial port uuid */
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&SerialPortUUID);

  /* place the uuid in a list */
  
  list[0] = &SerialPortUUID;
  SDP_CreateList(SDP_SEQUENCE,1,list,&ServiceClassList);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0001,&ServiceClassList);

  /* browse group list */
  
  uuid = 0x1002;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  /*
  uuid = 0x1001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  */

  list[0] = &BGL1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&BrowseGroupList);

  SDP_AddAttribute(pRecord,0x0005,&BrowseGroupList);
  
  /* browse group descriptor */

  uuid = 0xe003;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);

  SDP_AddAttribute(pRecord,0x0200,&BGL1);

  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,strlen(Service_name),Service_name,&Serial_Name);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0100,&Serial_Name);

  strcpy(Service_name,"Games");

  handle++;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  uuid = 0x1001; /* serial port uuid */
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&SerialPortUUID);

  /* place the uuid in a list */
  
  list[0] = &SerialPortUUID;
  SDP_CreateList(SDP_SEQUENCE,1,list,&ServiceClassList);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0001,&ServiceClassList);

  /* browse group list */
  
  uuid = 0xe001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  /*
  uuid = 0x1001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  */

  list[0] = &BGL1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&BrowseGroupList);

  SDP_AddAttribute(pRecord,0x0005,&BrowseGroupList);
  
  /* browse group descriptor */

  uuid = 0xe004;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);

  SDP_AddAttribute(pRecord,0x0200,&BGL1);

  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,strlen(Service_name),Service_name,&Serial_Name);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0100,&Serial_Name);


  strcpy(Service_name,"Movies");

  handle++;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  uuid = 0x1001; /* serial port uuid */
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&SerialPortUUID);

  /* place the uuid in a list */
  
  list[0] = &SerialPortUUID;
  SDP_CreateList(SDP_SEQUENCE,1,list,&ServiceClassList);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0001,&ServiceClassList);

  /* browse group list */
  
  uuid = 0xe001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  /*
  uuid = 0x1001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  */

  list[0] = &BGL1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&BrowseGroupList);

  SDP_AddAttribute(pRecord,0x0005,&BrowseGroupList);
  
  /* browse group descriptor */

  uuid = 0xe005;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);

  SDP_AddAttribute(pRecord,0x0200,&BGL1);

  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,strlen(Service_name),Service_name,&Serial_Name);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0100,&Serial_Name);


  strcpy(Service_name,"Starcraft");

  handle++;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  uuid = 0x1101; /* serial port uuid */
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&SerialPortUUID);

  /* place the uuid in a list */
  
  list[0] = &SerialPortUUID;
  SDP_CreateList(SDP_SEQUENCE,1,list,&ServiceClassList);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0001,&ServiceClassList);

  /* browse group list */
  
  uuid = 0xe004;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  /*
  uuid = 0x1001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  */

  list[0] = &BGL1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&BrowseGroupList);

  SDP_AddAttribute(pRecord,0x0005,&BrowseGroupList);
  
  /* browse group descriptor */
  /*
  uuid = 0xe001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);

  SDP_AddAttribute(pRecord,0x0200,&BGL1);
  */

  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,strlen(Service_name),Service_name,&Serial_Name);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0100,&Serial_Name);

  strcpy(Service_name,"A Bug's Life");

  handle++;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  uuid = 0x1101; /* serial port uuid */
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&SerialPortUUID);

  /* place the uuid in a list */
  
  list[0] = &SerialPortUUID;
  SDP_CreateList(SDP_SEQUENCE,1,list,&ServiceClassList);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0001,&ServiceClassList);

  /* browse group list */
  
  uuid = 0xe005;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  /*
  uuid = 0x1001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  */

  list[0] = &BGL1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&BrowseGroupList);

  SDP_AddAttribute(pRecord,0x0005,&BrowseGroupList);
  
  /* browse group descriptor */
  /*
  uuid = 0xe001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);

  SDP_AddAttribute(pRecord,0x0200,&BGL1);
  */
  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,strlen(Service_name),Service_name,&Serial_Name);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0100,&Serial_Name);

  strcpy(Service_name,"Dictionary");

  handle++;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  uuid = 0x1101; /* serial port uuid */
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&SerialPortUUID);

  /* place the uuid in a list */
  
  list[0] = &SerialPortUUID;
  SDP_CreateList(SDP_SEQUENCE,1,list,&ServiceClassList);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0001,&ServiceClassList);

  /* browse group list */
  
  uuid = 0xe003;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  /*
  uuid = 0x1001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  */

  list[0] = &BGL1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&BrowseGroupList);

  SDP_AddAttribute(pRecord,0x0005,&BrowseGroupList);
  
  /* browse group descriptor */
  /*
  uuid = 0xe001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);

  SDP_AddAttribute(pRecord,0x0200,&BGL1);
  */
  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,strlen(Service_name),Service_name,&Serial_Name);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0100,&Serial_Name);

  strcpy(Service_name,"Encyclopedia");

  handle++;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  uuid = 0x1101; /* serial port uuid */
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&SerialPortUUID);

  /* place the uuid in a list */
  
  list[0] = &SerialPortUUID;
  SDP_CreateList(SDP_SEQUENCE,1,list,&ServiceClassList);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0001,&ServiceClassList);

  /* browse group list */
  
  uuid = 0xe003;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  /*
  uuid = 0x1001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  */

  list[0] = &BGL1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&BrowseGroupList);

  SDP_AddAttribute(pRecord,0x0005,&BrowseGroupList);
  
  /* browse group descriptor */
  /*
  uuid = 0xe001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);

  SDP_AddAttribute(pRecord,0x0200,&BGL1);
  */
  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,strlen(Service_name),Service_name,&Serial_Name);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0100,&Serial_Name);

  strcpy(Service_name,"New York Times");

  handle++;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  uuid = 0x1101; /* serial port uuid */
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&SerialPortUUID);

  /* place the uuid in a list */
  
  list[0] = &SerialPortUUID;
  SDP_CreateList(SDP_SEQUENCE,1,list,&ServiceClassList);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0001,&ServiceClassList);

  /* browse group list */
  
  uuid = 0xe002;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  /*
  uuid = 0x1001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  */

  list[0] = &BGL1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&BrowseGroupList);

  SDP_AddAttribute(pRecord,0x0005,&BrowseGroupList);
  
  /* browse group descriptor */
  /*
  uuid = 0xe001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);

  SDP_AddAttribute(pRecord,0x0200,&BGL1);
  */
  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,strlen(Service_name),Service_name,&Serial_Name);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0100,&Serial_Name);

  strcpy(Service_name,"Limerick Post");

  handle++;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  uuid = 0x1101; /* serial port uuid */
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&SerialPortUUID);

  /* place the uuid in a list */
  
  list[0] = &SerialPortUUID;
  SDP_CreateList(SDP_SEQUENCE,1,list,&ServiceClassList);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0001,&ServiceClassList);

  /* browse group list */
  
  uuid = 0xe002;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  /*
  uuid = 0x1001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  */

  list[0] = &BGL1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&BrowseGroupList);

  SDP_AddAttribute(pRecord,0x0005,&BrowseGroupList);
  
  /* browse group descriptor */
  /*
  uuid = 0xe001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);

  SDP_AddAttribute(pRecord,0x0200,&BGL1);
  */
  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,strlen(Service_name),Service_name,&Serial_Name);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0100,&Serial_Name);

  strcpy(Service_name,"London Times");

  handle++;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  uuid = 0x1101; /* serial port uuid */
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&SerialPortUUID);

  /* place the uuid in a list */
  
  list[0] = &SerialPortUUID;
  SDP_CreateList(SDP_SEQUENCE,1,list,&ServiceClassList);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0001,&ServiceClassList);

  /* browse group list */
  
  uuid = 0xe002;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  /*
  uuid = 0x1001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);
  */

  list[0] = &BGL1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&BrowseGroupList);

  SDP_AddAttribute(pRecord,0x0005,&BrowseGroupList);
  
  /* browse group descriptor */
  /*
  uuid = 0xe001;
  SDP_EncodeAttribute(SDP_UUID,2,&uuid,&BGL1);

  SDP_AddAttribute(pRecord,0x0200,&BGL1);
  */
  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,strlen(Service_name),Service_name,&Serial_Name);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0100,&Serial_Name);
      
}




void test_db_api_sample_1(void)
{
  u_int32 handle;

  u_int16 _16bit;
  u_int8 _8bit;
  
  t_SDP_serviceRecord *pRecord;
  t_SDP_dataElement e1,e2,e3,e4;
  
  t_SDP_dataElement* list[10];
  
  char Service_name[] = "A sample database";

  handle = 0x00010002;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  _16bit = 0x2323;
  
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
  
  _8bit = 0x01;
  
  SDP_EncodeAttribute(SDP_UINT,1,&_16bit,&e3);
  
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

  /* browse group */

  _16bit = 0x1002;
  
  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e1);
  
  list[0] = &e1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&e2);

  SDP_AddAttribute(pRecord,0x0005,&e2);

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

  SDP_AddAttribute(pRecord,0x0009,&e3);

  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,sizeof(Service_name) -1,Service_name,&e1);

  SDP_AddAttribute(pRecord,0x0100,&e1);
}


void test_db_api_sample_2(void)
{
  u_int32 handle;

  u_int16 _16bit;
  u_int8 _8bit;
  
  t_SDP_serviceRecord *pRecord;
  t_SDP_dataElement e1,e2,e3,e4;
  
  t_SDP_dataElement* list[10];
  
  char Service_name[] = "A sample database";

  handle = 0x00010002;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  _16bit = 0x2323;
  
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
  
  _8bit = 0x01;
  
  SDP_EncodeAttribute(SDP_UINT,1,&_16bit,&e3);
  
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

  /* browse group */

  _16bit = 0x1002;
  
  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e1);
  
  list[0] = &e1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&e2);

  SDP_AddAttribute(pRecord,0x0005,&e2);

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

  SDP_AddAttribute(pRecord,0x0009,&e3);

  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,sizeof(Service_name) -1,Service_name,&e1);

  SDP_AddAttribute(pRecord,0x0100,&e1);

  /* now modify the protocol descriptor list */

  _16bit = 0x0100;
  
  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e1);
  
  list[0] = &e1;
  
  SDP_CreateList(SDP_SEQUENCE,1,list,&e2);

  _16bit = 0x0003;

  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e1);
  
  _8bit = 0x01;
  
  SDP_EncodeAttribute(SDP_UINT,1,&_16bit,&e3);
  
  list[0] = &e1;
  list[1] = &e3;

  SDP_CreateList(SDP_SEQUENCE,2,list,&e4);
  
  _16bit = 0x0009;  /* changed from 0x0008 */

  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e1);

  list[0] = &e1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&e3);

  list[0] = &e2;
  list[1] = &e4;
  list[2] = &e3;

  SDP_CreateList(SDP_SEQUENCE,3,list,&e1);
  
  /*  SDP_AddAttribute(pRecord,0x0004,&e1); */

  SDP_ReplaceAttribute(pRecord,0x0004,&e1);


}



void test_db_api_sample_3(void)
{
  u_int32 handle;

  u_int16 _16bit;
  u_int8 _8bit;
  
  t_SDP_serviceRecord *pRecord;
  t_SDP_dataElement e1,e2,e3,e4;
  
  t_SDP_dataElement* list[10];
  
  char Service_name[] = "A sample database";

  handle = 0x00010002;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  _16bit = 0x2323;
  
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
  
  _8bit = 0x01;
  
  SDP_EncodeAttribute(SDP_UINT,1,&_16bit,&e3);
  
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

  /* browse group */

  _16bit = 0x1002;
  
  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e1);
  
  list[0] = &e1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&e2);

  SDP_AddAttribute(pRecord,0x0005,&e2);

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

  SDP_AddAttribute(pRecord,0x0009,&e3);

  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,sizeof(Service_name) -1,Service_name,&e1);

  SDP_AddAttribute(pRecord,0x0100,&e1);


  /*  Now to remove the record */

  SDP_RemoveRecord(0x00010003);
}



void test_db_api_sample_4(void)
{
  u_int32 handle;

  u_int16 _16bit;
  u_int8 _8bit;
  
  t_SDP_serviceRecord *pRecord;
  t_SDP_dataElement e1,e2,e3,e4;
  
  t_SDP_dataElement* list[10];
  
  char Service_name[] = "A sample database";

  handle = 0x00010002;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  _16bit = 0x2323;
  
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
  
  _8bit = 0x01;
  
  SDP_EncodeAttribute(SDP_UINT,1,&_16bit,&e3);
  
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

  /* browse group */

  _16bit = 0x1002;
  
  SDP_EncodeAttribute(SDP_UUID,2,&_16bit,&e1);
  
  list[0] = &e1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&e2);

  SDP_AddAttribute(pRecord,0x0005,&e2);

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

  SDP_AddAttribute(pRecord,0x0009,&e3);

  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,sizeof(Service_name) -1,Service_name,&e1);

  SDP_AddAttribute(pRecord,0x0100,&e1);


  handle = 0;
  /* now to get the next one from the list  */
  SDP_GetNextRecord(0x00010002,&handle);

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

















