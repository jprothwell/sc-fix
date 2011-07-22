/*
  $Id: test_sdp_server.c 1532 2010-08-23 05:47:34Z huazeng $
 */

#include "host_config.h"
#include "host_types.h"
#include "hu_harness.h"
#include "papi.h"
#include "host_ctrl.h"

#include "l2_types.h"
#include "unit_stub_l2cap_internal.h"

#include "../../include/sdp_ue.h"
#include "../../include/sdp_constants.h"
#include "../../include/sdp_server.h"
#include "../../include/sdp_db.h"

int  prh_sdp_unit_init(int p1, void *arg);


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
  
  SDP_EncodeAttribute(SDP_UUID,2,(u_int8 *)(&_16bit),&e1);
  
  _16bit = 0x1201;
  
  SDP_EncodeAttribute(SDP_UUID,2,(u_int8 *)(&_16bit),&e2);

  list[0] = &e1;
  list[1] = &e2;
  
  SDP_CreateList(SDP_SEQUENCE,2,list,&e3);

  SDP_AddAttribute(pRecord,0x0001,&e3);
  
  /* protocol descriptor list */

  _16bit = 0x0100;
  
  SDP_EncodeAttribute(SDP_UUID,2,(u_int8 *)(&_16bit),&e1);
  
  list[0] = &e1;
  
  SDP_CreateList(SDP_SEQUENCE,1,list,&e2);

  _16bit = 0x0003;

  SDP_EncodeAttribute(SDP_UUID,2,(u_int8 *)(&_16bit),&e1);
  
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
  
  SDP_EncodeAttribute(SDP_UUID,2,(u_int8 *)(&_16bit),&e1);
  
  list[0] = &e1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&e2);

  SDP_AddAttribute(pRecord,0x0005,&e2);

  /* language base */

  _16bit = ('e' << 8) + 'n';

  SDP_EncodeAttribute(SDP_UINT,2,(u_int8 *)(&_16bit),&e1);

  _16bit = 106;

  SDP_EncodeAttribute(SDP_UINT,2,(u_int8 *)(&_16bit),&e2);
   
  _16bit = 0x0100;
  
  SDP_EncodeAttribute(SDP_UINT,2,(u_int8 *)(&_16bit),&e3);
  
  list[0] = &e1;
  list[1] = &e2;
  list[2] = &e3;

  SDP_CreateList(SDP_SEQUENCE,3,list,&e4);

  SDP_AddAttribute(pRecord,0x0006,&e4);

  /* profile - list */

  _16bit = 0x1103;
  
  SDP_EncodeAttribute(SDP_UUID,2,(u_int8 *)(&_16bit),&e1);

  _16bit = 0x0100;

  SDP_EncodeAttribute(SDP_UINT,2,(u_int8 *)(&_16bit),&e2);

  list[0] = &e1;
  list[1] = &e2;
  
  SDP_CreateList(SDP_SEQUENCE,2,list,&e3);

  list[0] = &e3;

  SDP_CreateList(SDP_SEQUENCE,1,list,&e2);

  SDP_AddAttribute(pRecord,0x0009,&e2);


  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,sizeof(Service_name) -1,(u_int8*)Service_name,&e1);

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
  
  SDP_EncodeAttribute(SDP_UUID,2,(u_int8 *)(&_16bit),&e1);
  
  _16bit = 0x1204;
  
  SDP_EncodeAttribute(SDP_UUID,2,(u_int8 *)(&_16bit),&e2);

  list[0] = &e1;
  list[1] = &e2;
  
  SDP_CreateList(SDP_SEQUENCE,2,list,&e3);

  SDP_AddAttribute(pRecord,0x0001,&e3);
  
  /* protocol descriptor list */

  _16bit = 0x0100;
  
  SDP_EncodeAttribute(SDP_UUID,2,(u_int8 *)(&_16bit),&e1);
  
  list[0] = &e1;
  
  SDP_CreateList(SDP_SEQUENCE,1,list,&e2);

  _16bit = 0x0003;

  SDP_EncodeAttribute(SDP_UUID,2,(u_int8 *)(&_16bit),&e1);
  
  _8bit = 0x02;
  
  SDP_EncodeAttribute(SDP_UINT,1,(u_int8 *)(&_16bit),&e3);
  
  list[0] = &e1;
  list[1] = &e3;

  SDP_CreateList(SDP_SEQUENCE,2,list,&e4);
  
  list[0] = &e2;
  list[1] = &e4;

  SDP_CreateList(SDP_SEQUENCE,2,list,&e1);
  
  SDP_AddAttribute(pRecord,0x0004,&e1);

  /* browse group */

  _16bit = 0x1002;
  
  SDP_EncodeAttribute(SDP_UUID,2,(u_int8 *)(&_16bit),&e1);
  
  list[0] = &e1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&e2);

  SDP_AddAttribute(pRecord,0x0005,&e2);

  /* language base */

  _16bit = ('e' << 8) + 'n';

  SDP_EncodeAttribute(SDP_UINT,2,(u_int8 *)(&_16bit),&e1);

  _16bit = 106;

  SDP_EncodeAttribute(SDP_UINT,2,(u_int8 *)(&_16bit),&e2);
   
  _16bit = 0x0100;
  
  SDP_EncodeAttribute(SDP_UINT,2,(u_int8 *)(&_16bit),&e3);
  
  list[0] = &e1;
  list[1] = &e2;
  list[2] = &e3;

  SDP_CreateList(SDP_SEQUENCE,3,list,&e4);

  SDP_AddAttribute(pRecord,0x0006,&e4);

  /* profile - list */

  _16bit = 0x1111;
  
  SDP_EncodeAttribute(SDP_UUID,2,(u_int8 *)(&_16bit),&e1);

  _16bit = 0x0100;

  SDP_EncodeAttribute(SDP_UINT,2,(u_int8 *)(&_16bit),&e2);

  list[0] = &e1;
  list[1] = &e2;
  
  SDP_CreateList(SDP_SEQUENCE,2,list,&e3);

  list[0] = &e3;

  SDP_CreateList(SDP_SEQUENCE,1,list,&e2);

  SDP_AddAttribute(pRecord,0x0009,&e2);


  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,sizeof(Service_name) -1,(u_int8*)Service_name,&e1);

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
  
  SDP_EncodeAttribute(SDP_UUID,2,(u_int8 *)(&_16bit),&e1);
  

  list[0] = &e1;
  
  SDP_CreateList(SDP_SEQUENCE,1,list,&e3);

  SDP_AddAttribute(pRecord,0x0001,&e3);
  
  /* protocol descriptor list */

  _16bit = 0x0100;
  
  SDP_EncodeAttribute(SDP_UUID,2,(u_int8 *)(&_16bit),&e1);
  
  list[0] = &e1;
  
  SDP_CreateList(SDP_SEQUENCE,1,list,&e2);

  _16bit = 0x0003;

  SDP_EncodeAttribute(SDP_UUID,2,(u_int8 *)(&_16bit),&e1);
  
  _8bit = 0x01;
  
  SDP_EncodeAttribute(SDP_UINT,1,(u_int8 *)(&_16bit),&e3);
  
  list[0] = &e1;
  list[1] = &e3;

  SDP_CreateList(SDP_SEQUENCE,2,list,&e4);
  
  _16bit = 0x0008;

  SDP_EncodeAttribute(SDP_UUID,2,(u_int8 *)(&_16bit),&e1);

  list[0] = &e1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&e3);

  list[0] = &e2;
  list[1] = &e4;
  list[2] = &e3;

  SDP_CreateList(SDP_SEQUENCE,3,list,&e1);
  
  SDP_AddAttribute(pRecord,0x0004,&e1);

  /* browse group */

  _16bit = 0x1002;
  
  SDP_EncodeAttribute(SDP_UUID,2,(u_int8 *)(&_16bit),&e1);
  
  list[0] = &e1;

  SDP_CreateList(SDP_SEQUENCE,1,list,&e2);

  SDP_AddAttribute(pRecord,0x0005,&e2);

  /* language base */

  _16bit = ('e' << 8) + 'n';

  SDP_EncodeAttribute(SDP_UINT,2,(u_int8 *)(&_16bit),&e1);

  _16bit = 106;

  SDP_EncodeAttribute(SDP_UINT,2,(u_int8 *)(&_16bit),&e2);
   
  _16bit = 0x0100;
  
  SDP_EncodeAttribute(SDP_UINT,2,(u_int8 *)(&_16bit),&e3);
  
  list[0] = &e1;
  list[1] = &e2;
  list[2] = &e3;

  SDP_CreateList(SDP_SEQUENCE,3,list,&e4);

  SDP_AddAttribute(pRecord,0x0006,&e4);

  /* profile - list */

  _16bit = 0x1105;
  
  SDP_EncodeAttribute(SDP_UUID,2,(u_int8 *)(&_16bit),&e1);

  _16bit = 0x0100;

  SDP_EncodeAttribute(SDP_UINT,2,(u_int8 *)(&_16bit),&e2);

  list[0] = &e1;
  list[1] = &e2;
  
  SDP_CreateList(SDP_SEQUENCE,2,list,&e3);
  
  list[0] = &e3;

  SDP_CreateList(SDP_SEQUENCE,1,list,&e2);

  SDP_AddAttribute(pRecord,0x0009,&e2);

  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,sizeof(Service_name) -1,(u_int8*)Service_name,&e1);

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

  prh_sdp_encode_data(&element1,(u_int8 *)(&_16bit));  
  
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
  
  prh_sdp_encode_data(&element1,(u_int8 *)(&_16bit));
  
  prh_sdp_add(pRecord,0x0003,&element1);

  /* 0x0004 */

  _16bit = 0x0100;

  element1.type = SDP_UUID;
  element1.size = 1;
  element1.data_length = 2;
  prh_sdp_encode_data(&element1,(u_int8 *)(&_16bit));
  
  list[0] = &element1;

  prh_sdp_encode_list(&element3,1,list);

  _16bit = 0x0003;
  element1.type = SDP_UUID;
  element1.size = 1;
  element1.data_length = 2;
  prh_sdp_encode_data(&element1,(u_int8 *)(&_16bit));
  
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

  prh_sdp_encode_data(&element1,(u_int8 *)(&_16bit));
  
  list[0] = &element1;

  prh_sdp_encode_list(&element2,1,list);

  prh_sdp_add(pRecord,0x0005,&element2);

  /* 0x0006 */

  _16bit = ('e' << 8) + 'n';

  element1.type = SDP_UINT;
  element1.size = 1;
  element1.data_length = 2;
  
  prh_sdp_encode_data(&element1,(u_int8 *)(&_16bit));
  
  _16bit = 106;

  element2.type = SDP_UINT;
  element2.size = 1;
  element2.data_length = 2;
  
  prh_sdp_encode_data(&element2,(u_int8 *)(&_16bit));
  
  _16bit = 0x0100;
  
  element3.type = SDP_UINT;
  element3.size = 1;
  element3.data_length  = 2;

  prh_sdp_encode_data(&element3,(u_int8 *)(&_16bit));
  
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
  
  prh_sdp_encode_data(&element1,(u_int8 *)(&_32bit));

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
  
  prh_sdp_encode_data(&element1,(u_int8 *)(&_16bit));
  
  _16bit = 0x1104;
  
  element2.type = SDP_UUID;
  element2.size = 1;
  element2.data_length = 2;

  prh_sdp_encode_data(&element2,(u_int8 *)(&_16bit));
  
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


void db_sample_to_cause_continue(void)
{
  u_int32 handle;

  u_int16 _16bit;
  
  t_SDP_serviceRecord *pRecord;
  t_SDP_dataElement e1,e3;
  
  t_SDP_dataElement* list[10];
  
  handle = 0x00010002;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  _16bit = 0x0100;
  
  SDP_EncodeAttribute(SDP_UUID,2,(u_int8 *)(&_16bit),&e1);
  

  list[0] = &e1;
  
  SDP_CreateList(SDP_SEQUENCE,1,list,&e3);

  SDP_AddAttribute(pRecord,0x0001,&e3);
  
  /* protocol descriptor list */
}

void Test_Data_Base_Setup(void)
{
  u_int8 lcv;

   pDebugCheck();
  upf_3_dialup();

  pDebugCheck();
  upf_3_fax();

  pDebugCheck();
  upf_3_obex();

  pDebugCheck();

  
  Synchronisation();
  pDebugCheck();

  for(lcv = 0;lcv <6;lcv++)
  {
	db_sample_to_cause_continue();
  }
  
  pDebugCheck();

}




static prh_t_hu_outcome prh_sdp_unit_010(int p1, void *arg)
{
  /*
    database setup.
   */

	pDebugCheck();

  Test_Data_Base_Setup();

  return HU_PASSED;
}

static prh_t_hu_outcome prh_sdp_unit_011(int p1, void *arg)
{
  /*
    database shutdown and setup
   */
  prh_sdp_srv_layer_ctrl(PRH_HOST_SHUTDOWN);
  prh_sdp_srv_layer_ctrl(PRH_HOST_STARTUP);
  Test_Data_Base_Setup();
  return HU_PASSED;
}


static prh_t_hu_outcome prh_sdp_unit_012(int p1, void *arg)
{
  /*
    A standard service search request. Check the response.
   */

  struct host_buf *buffer;
  u_int16 i;
  void* connection;
  t_bdaddr address;
  int unused;  

  u_int8 test_results_a[] = {0x03,0x00,0x00,0x00,0x09,0x00,0x01,0x00,0x01,0x00,
			     0x01,0x00,0x00,0x00
  };

	pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,0x40,&connection);

  prh_stub_l2cap_GetWriteBuffer(500,&buffer);

  buffer->data[0] = 0x02;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

  i = test_pattern(buffer->data + 5,0x1103);

  buffer->data[5 + i] = 0x00;
  buffer->data[5 + i + 1] = 0x60;
  buffer->data[5 + i + 2] = 0x00;
  i +=3;
  prh_sdp_write16(i,buffer->data + 3);

  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);
  
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
    return HU_FAILED;

  prh_stub_l2cap_FreeWriteBuffer(buffer);

  prh_stub_l2cap_do_disconnectind(connection);

	pDebugCheck();

  return HU_PASSED;
}


static prh_t_hu_outcome prh_sdp_unit_013(int p1, void *arg)
{
  /*
    A standard attribute request, check response.
   */
  
  struct host_buf *buffer;
  u_int16 i;
  void* connection;
  t_bdaddr address;
  int unused;  

  u_int8 test_results_a[] = {0x05,0x00,0x00,0x00,0x19,0x00,0x16,0x37,0x00,0x00,
			     0x00,0x11,0x09,0x00,0x04,0x35,0x0c,0x35,0x03,0x19,
			     0x01,0x00,0x35,0x05,0x19,0x00,0x03,0x08,0x03,0x00
  };

	pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  prh_stub_l2cap_GetWriteBuffer(500,&buffer);

  buffer->data[0] = 0x04;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

  buffer->data[5] = 0x00;
  buffer->data[6] = 0x01;
  buffer->data[7] = 0x00;
  buffer->data[8] = 0x01;

  buffer->data[9] = 0x00;
  buffer->data[10] = 0x60;

  i = 6;

  test_attrib(buffer->data + 11,0x0004);

  i += 5;
  buffer->data[16] = 0x00;
  i++;

  prh_sdp_write16(i,buffer->data + 3);
  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);
  
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
    return HU_FAILED;

  prh_stub_l2cap_FreeWriteBuffer(buffer);

  prh_stub_l2cap_do_disconnectind(connection);

  pDebugCheck();

  return HU_PASSED;
}

static prh_t_hu_outcome prh_sdp_unit_014(int p1, void *arg)
{
  /*
    a standard service attribute request, check the response.
   */
  struct host_buf *buffer;
  u_int16 i;
  void* connection;
  t_bdaddr address;
  int unused;  

  u_int8 test_results_a[] = {0x07,0x00,0x00,0x00,0x1b,0x00,0x18,0x37,0x00,0x00,
			     0x00,0x13,0x37,0x00,0x00,0x00,0x0e,0x09,0x00,0x06,
			     0x35,0x09,0x09,0x65,0x6e,0x09,0x00,0x6a,0x09,0x01,
			     0x00,0x00
  };

	pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,0x40,&connection);

  prh_stub_l2cap_GetWriteBuffer(500,&buffer);

  buffer->data[0] = 0x06;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

  i = test_pattern(buffer->data + 5,0x1107);

  buffer->data[5+ i] = 0x00;
  buffer->data[6+ i] = 0x60;

  test_attrib(buffer->data + i + 7,0x0006);

  buffer->data[7 + i + 5] = 0x00;

  i += 5 + 3;

  prh_sdp_write16(i,buffer->data + 3);
  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);
  
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
    return HU_FAILED;

  prh_stub_l2cap_FreeWriteBuffer(buffer);

  prh_stub_l2cap_do_disconnectind(connection);

pDebugCheck();

  return HU_PASSED;
}

static prh_t_hu_outcome prh_sdp_unit_015(int p1, void *arg)
{
  /*
    Continue for a ss at the server side
   */
  struct host_buf *buffer;
  u_int16 i;
  void* connection;
  t_bdaddr address;
  int unused;  

  u_int8 test_results_a[] = {
 0x03,0x00,0x00,0x00,0x2b,  0x00,  0x0a,  0x00,  0x09, 
	0x00,0x01,0x00,0x00,
	0x00,0x01,0x00,0x01,
	0x00,0x01,0x00,0x02,
	0x00,0x01,0x00,0x03,
	0x00,0x01,0x00,0x04,
	0x00,0x01,0x00,0x05,
	0x00,0x01,0x00,0x06,
	0x00,0x01,0x00,0x07,
	0x00,0x01,0x00,0x08,
 0x02,  0x00,  0x09
  };

  u_int8 test_results_b[] = {
  0x03,0x00,0x00,0x00,
  0x09,
  0x00,
  0x0A,
  0x00,
  0x01,
  0x00,0x01,0x00,0x09,
  0x00 
};

pDebugCheck();

  prh_stub_l2cap_setup_l2_connection_ex(address,1,0x40,&connection,50,50);

  prh_stub_l2cap_GetWriteBuffer(500,&buffer);

  buffer->data[0] = 0x02;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

  i = test_pattern(buffer->data + 5,0x0100);

  buffer->data[5 + i] = 0x00;
  buffer->data[5 + i + 1] = 0x18;
  buffer->data[5 + i + 2] = 0x00;
  i +=3;
  prh_sdp_write16(i,buffer->data + 3);
  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);
  
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
    return HU_FAILED;

  buffer->data[0] = 0x02;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

  i = test_pattern(buffer->data + 5,0x0100);

  buffer->data[5 + i] = 0x00;
  buffer->data[5 + i + 1] = 0x18;
  buffer->data[5 + i + 2] = 0x02;
  buffer->data[5 + i + 3] = 0x00;
  buffer->data[5 + i + 4] = 0x09;
  i +=2;
  i +=3;

  prh_sdp_write16(i,buffer->data + 3);
  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);
  
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_b,sizeof(test_results_b)) == HU_FAILED)
    return HU_FAILED;  

  prh_stub_l2cap_FreeWriteBuffer(buffer);

  prh_stub_l2cap_do_disconnectind(connection);

pDebugCheck();

  return HU_PASSED;
}

static prh_t_hu_outcome prh_sdp_unit_016(int p1, void *arg)
{
  /*
    sa continue
   */

  struct host_buf *buffer;
  u_int16 i;
  void* connection;
  t_bdaddr address;
  int unused;  

  u_int8 test_results_a[] = {
	0x05,0x00,0x00,0x00, 0x16, 0x00, 0x0f, 0x37, 0x00,0x00,0x00,0x14, 0x09, 0x01, 0x00, 0x25, 0x0F, 0x73, 0x79, 0x6E, 0x63,0x68, 0x04, 0x00, 0x00, 0x00, 0x0f
};

u_int8 test_results_b[] = {
	0x05,0x00,0x00,0x00, 0x0d, 0x00, 0x0a, 0x72, 0x6F, 0x6E, 0x69, 0x73, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x00
};

	pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,0x40,&connection);

  prh_stub_l2cap_GetWriteBuffer(500,&buffer);

  buffer->data[0] = 0x04;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

  buffer->data[5] = 0x00;
  buffer->data[6] = 0x01;
  buffer->data[7] = 0x00;
  buffer->data[8] = 0x03;

  buffer->data[9] = 0x00;
  buffer->data[10] = 0x0f;

  i = 6;

  test_attrib(buffer->data + 11,0x0100);

  i += 5;
  buffer->data[16] = 0x00;
  i++;

  prh_sdp_write16(i,buffer->data + 3);
  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);
  
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
    return HU_FAILED;  

prh_stub_l2cap_FreeWriteBuffer(buffer);

  prh_stub_l2cap_GetWriteBuffer(500,&buffer);

  buffer->data[0] = 0x04;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

  buffer->data[5] = 0x00;
  buffer->data[6] = 0x01;
  buffer->data[7] = 0x00;
  buffer->data[8] = 0x03;

  buffer->data[9] = 0x00;
  buffer->data[10] = 0x0f;

  i = 6;

  test_attrib(buffer->data + 11,0x0100);
  i += 5;

  buffer->data[16] = 0x04;
  buffer->data[17] = 0x00;
  buffer->data[18] = 0x00;
  buffer->data[19] = 0x00;
  buffer->data[20] = 0x0f;
  
  i += 5;

  prh_sdp_write16(i,buffer->data + 3);
  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);
  
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_b,sizeof(test_results_b)) == HU_FAILED)
    return HU_FAILED;  

  prh_stub_l2cap_FreeWriteBuffer(buffer);


  prh_stub_l2cap_do_disconnectind(connection);

  pDebugCheck();

  return HU_PASSED;
}

static prh_t_hu_outcome prh_sdp_unit_017(int p1, void *arg)
{
  struct host_buf *buffer;
  u_int16 i;
  void* connection;
  t_bdaddr address;
  int unused;  

  u_int8 test_results_a[] = {
	0x07,0x00,0x00,0x00, 0x19, 0x00, 0x12, 0x37, 0x00,0x00,0x00, 0x19, 0x37, 0x00,0x00,0x00,0x14, 0x09, 0x01, 0x00, 0x25, 0x0F,  
	0x73, 0x79, 0x6E, 0x04, 0x00, 0x00, 0x00, 0x12
};

 u_int8 test_results_b[] = {
	0x07,0x00,0x00,0x00, 0x0f, 0x00, 0x0c, 0x63, 0x68, 0x72, 0x6F, 0x6E, 0x69, 0x73, 0x61, 0x74, 0x69, 0x6F, 0x6E, 00
};


  prh_stub_l2cap_setup_l2_connection(address,1,0x40,&connection);

  prh_stub_l2cap_GetWriteBuffer(500,&buffer);

  buffer->data[0] = 0x06;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

  i = test_pattern(buffer->data + 5,0x1107);

  buffer->data[5+ i] = 0x00;
  buffer->data[6+ i] = 0x12;

  test_attrib(buffer->data + i + 7,0x0100);

  buffer->data[7 + i + 5] = 0x00;

  i += 5 + 3;

  prh_sdp_write16(i,buffer->data + 3);
  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);
  
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
    return HU_FAILED;

  buffer->data[0] = 0x06;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

  i = test_pattern(buffer->data + 5,0x1107);

  buffer->data[5+ i] = 0x00;
  buffer->data[6+ i] = 0x12;

  test_attrib(buffer->data + i + 7,0x0100);

  buffer->data[7 + i + 5] = 0x04;
  buffer->data[7 + i + 6] = 0x00;
  buffer->data[7 + i + 7] = 0x00;
  buffer->data[7 + i + 8] = 0x00;
  buffer->data[7 + i + 9] = 0x12;

  i += 5 + 3 + 4;

  prh_sdp_write16(i,buffer->data + 3);
  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);
  
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_b,sizeof(test_results_b)) == HU_FAILED)
    return HU_FAILED;

  prh_stub_l2cap_FreeWriteBuffer(buffer);


  prh_stub_l2cap_do_disconnectind(connection);


  return HU_PASSED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_204
 
	*  TEST DESCRIPTION:
		Test attributre retrival when a range of aids is used.
 
	*  TEST STEPS:
		1. create connection
		2. construct message with attribute range
		3. send pdu
		4. test results 
		5. disconnect

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_204(int p1, void *arg)
{
  /*
    a standard service attribute request, check the response.
   */
  struct host_buf *buffer;
  u_int16 i;
  void* connection;
  t_bdaddr address;
  int unused;  

  u_int8 test_results_a[] = {0x07,0x00,0x00,0x00,0x1b,0x00,0x18,0x37,0x00,0x00,
			     0x00,0x13,0x37,0x00,0x00,0x00,0x0e,0x09,0x00,0x06,
			     0x35,0x09,0x09,0x65,0x6e,0x09,0x00,0x6a,0x09,0x01,
			     0x00,0x00
  };

	pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,0x40,&connection);

  prh_stub_l2cap_GetWriteBuffer(500,&buffer);

  buffer->data[0] = 0x06;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

  i = test_pattern(buffer->data + 5,0x1107);

  buffer->data[5+ i] = 0x00;
  buffer->data[6+ i] = 0x60;

  test_attrib_range(buffer->data + i + 7,0x0006,0x0010);

  buffer->data[7 + i + 7] = 0x00;

  i += 7 + 3;

  prh_sdp_write16(i,buffer->data + 3);
  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);
  
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
    return HU_FAILED;

  prh_stub_l2cap_FreeWriteBuffer(buffer);

  prh_stub_l2cap_do_disconnectind(connection);

pDebugCheck();

  return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_208
 
	*  TEST DESCRIPTION:
		Test that a service search with a malformed UUID is rejected by the server.
 
	*  TEST STEPS:
		1. create connection
		2. constructe service search request with a bad uuid.
		3. send the pdu
		4. check for the correct error code.
		5. disconnect

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_208(int p1, void *arg)
{
  /*
    A standard service search request. Check the response.
   */

  struct host_buf *buffer;
  u_int16 i;
  void* connection;
  t_bdaddr address;
  int unused;  

  u_int8 test_results_a[] = {0x01,0x00,0x00,0x00,0x02,0x00,0x03};

	pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,0x40,&connection);

  prh_stub_l2cap_GetWriteBuffer(500,&buffer);

  buffer->data[0] = 0x02;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

  i = test_pattern(buffer->data + 5,0x1103);

 buffer->data[5] = 0xff;  /* bad uuid */

  buffer->data[5 + i] = 0x00;
  buffer->data[5 + i + 1] = 0x60;
  buffer->data[5 + i + 2] = 0x00;
  i +=3;
  prh_sdp_write16(i,buffer->data + 3);

  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);
  
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
    return HU_FAILED;

  prh_stub_l2cap_FreeWriteBuffer(buffer);

  prh_stub_l2cap_do_disconnectind(connection);

	pDebugCheck();

  return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_209
 
	*  TEST DESCRIPTION:
		Test that a service search attribute with a malformed UUID is rejected by the server.
 
	*  TEST STEPS:
		1. create connection
		2. constructe service search attribute request with a bad uuid.
		3. send the pdu
		4. check for the correct error code.
		5. disconnect

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_209(int p1, void *arg)
{
  /*
    a standard service attribute request, check the response.
   */
  struct host_buf *buffer;
  u_int16 i;
  void* connection;
  t_bdaddr address;
  int unused;  

  u_int8 test_results_a[] = {0x01,0x00,0x00,0x00,0x02,0x00,0x03 };

	pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,0x40,&connection);

  prh_stub_l2cap_GetWriteBuffer(500,&buffer);

  buffer->data[0] = 0x06;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

  i = test_pattern(buffer->data + 5,0x1107);

	buffer->data[5] = 0xff; /* bad uuid */

  buffer->data[5+ i] = 0x00;
  buffer->data[6+ i] = 0x60;

  test_attrib(buffer->data + i + 7,0x0006);

  buffer->data[7 + i + 5] = 0x00;

  i += 5 + 3;

  prh_sdp_write16(i,buffer->data + 3);
  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);
  
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
    return HU_FAILED;

  prh_stub_l2cap_FreeWriteBuffer(buffer);

  prh_stub_l2cap_do_disconnectind(connection);

  return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_210
 
	*  TEST DESCRIPTION:
		Test that a service search with a bad max bytes is rejected by the server
 
	*  TEST STEPS:
		1. create connection
		2. constructe service search request with a bad max bytes
		3. send the pdu
		4. check for the correct error code.
		5. disconnect

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_210(int p1, void *arg)
{

  struct host_buf *buffer;
  u_int16 i;
  void* connection;
  t_bdaddr address;
  int unused;  

  u_int8 test_results_a[] = {0x01,0x00,0x00,0x00,0x02,0x00,0x03 };

  pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,0x40,&connection);

  prh_stub_l2cap_GetWriteBuffer(500,&buffer);

  
  buffer->data[0] = 0x04;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

  buffer->data[5] = 0x00;
  buffer->data[6] = 0x01;
  buffer->data[7] = 0x00;
  buffer->data[8] = 0x01;

  buffer->data[9] = 0x00;
  buffer->data[10] = 0x06; /* bad max bytes */

  i = 6;

  test_attrib(buffer->data + 11,0x0004);

  i += 5;
  buffer->data[16] = 0x00;
  i++;

  prh_sdp_write16(i,buffer->data + 3);
  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);

  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
    return HU_FAILED;

  prh_stub_l2cap_FreeWriteBuffer(buffer);

  prh_stub_l2cap_do_disconnectind(connection);

  return HU_PASSED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_211
 
	*  TEST DESCRIPTION:
		Test that a service search attribute with a bad max bytes is rejecte by the server.
 
	*  TEST STEPS:
		1. create connection
		2. constructe service search attribute request with a bad max bytes.
		3. send the pdu
		4. check for the correct error code.
		5. disconnect

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_211(int p1, void *arg)
{

  struct host_buf *buffer;
  u_int16 i;
  void* connection;
  t_bdaddr address;
  int unused;  

  u_int8 test_results_a[] = {0x01,0x00,0x00,0x00,0x02,0x00,0x03 };

	pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,0x40,&connection);

  prh_stub_l2cap_GetWriteBuffer(500,&buffer);

  buffer->data[0] = 0x06;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

  i = test_pattern(buffer->data + 5,0x1107);

  buffer->data[5+ i] = 0x00;
  buffer->data[6+ i] = 0x06; /* bad max bytes */

  test_attrib(buffer->data + i + 7,0x0006);

  buffer->data[7 + i + 5] = 0x00;

  i += 5 + 3;

  prh_sdp_write16(i,buffer->data + 3);
  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);
  
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
    return HU_FAILED;

  prh_stub_l2cap_FreeWriteBuffer(buffer);

  prh_stub_l2cap_do_disconnectind(connection);

  return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_212
 
	*  TEST DESCRIPTION:
		Test that a service search attribute with a malformed aid is rejected by the server.
 
	*  TEST STEPS:
		1. create connection
		2. constructe service search attribute request with a bad aid.
		3. send the pdu
		4. check for the correct error code.
		5. disconnect

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_212(int p1, void *arg)
{
  /*
    a standard service attribute request, check the response.
   */
  struct host_buf *buffer;
  u_int16 i;
  void* connection;
  t_bdaddr address;
  int unused;  

  u_int8 test_results_a[] = {0x01,0x00,0x00,0x00,0x02,0x00,0x03 };

	pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,0x40,&connection);

  prh_stub_l2cap_GetWriteBuffer(500,&buffer);

  buffer->data[0] = 0x06;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

  i = test_pattern(buffer->data + 5,0x1107);

  buffer->data[5+ i] = 0x00;
  buffer->data[6+ i] = 0x60;

  test_attrib(buffer->data + i + 7,0x0006);

	buffer->data[i + 7] = 0xff; /* bad aid */

  buffer->data[7 + i + 5] = 0x00;

  i += 5 + 3;

  prh_sdp_write16(i,buffer->data + 3);
  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);
  
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
    return HU_FAILED;

  prh_stub_l2cap_FreeWriteBuffer(buffer);

  prh_stub_l2cap_do_disconnectind(connection);

pDebugCheck();

  return HU_PASSED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_213
 
	*  TEST DESCRIPTION:
		Test that a service search with a malformed aid is rejected by the server.
 
	*  TEST STEPS:
		1. create connection
		2. constructe service search request with a bad aid.
		3. send the pdu
		4. check for the correct error code.
		5. disconnect

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_213(int p1, void *arg)
{
  /*
    A standard attribute request, check response.
   */
  
  struct host_buf *buffer;
  u_int16 i;
  void* connection;
  t_bdaddr address;
  int unused;  
  int ret;

  u_int8 test_results_a[] = {0x01,0x00,0x00,0x00,0x02,0x00,0x03};
  ret = HU_PASSED;

	pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  prh_stub_l2cap_GetWriteBuffer(500,&buffer);

  buffer->data[0] = 0x04;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

  buffer->data[5] = 0x00;
  buffer->data[6] = 0x01;
  buffer->data[7] = 0x00;
  buffer->data[8] = 0x01;

  buffer->data[9] = 0x00;
  buffer->data[10] = 0x60;

  i = 6;

  test_attrib(buffer->data + 11,0x0004);

	buffer->data[11] = 0xff; /* bad aid */

  i += 5;
  buffer->data[16] = 0x00;
  i++;

  prh_sdp_write16(i,buffer->data + 3);
  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);
  
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
    ret =  HU_FAILED;

  prh_stub_l2cap_FreeWriteBuffer(buffer);

  prh_stub_l2cap_do_disconnectind(connection);

  pDebugCheck();

  return ret;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_214
 
	*  TEST DESCRIPTION:
		Test that a service attribute will handle a medium sized aid list.
 
	*  TEST STEPS:
		1. create connection
		2. constructe service attribute request with medium aid list
		3. send the pdu
		4. check for the correct response.
		5. disconnect

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_214(int p1, void *arg)
{
  /*
    A standard attribute request, check response.
   */
  
  struct host_buf *buffer;
  u_int16 i;
  void* connection;
  t_bdaddr address;
  int unused;  

  u_int8 med_attrib[] = {
	0x36,0x00,0x03,
	0x09,0x00,0x04
  };

  u_int8 test_results_a[] = {0x05,0x00,0x00,0x00,0x19,0x00,0x16,0x37,0x00,0x00,
			     0x00,0x11,0x09,0x00,0x04,0x35,0x0c,0x35,0x03,0x19,
			     0x01,0x00,0x35,0x05,0x19,0x00,0x03,0x08,0x03,0x00
  };

	pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  prh_stub_l2cap_GetWriteBuffer(500,&buffer);

  buffer->data[0] = 0x04;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

  buffer->data[5] = 0x00;
  buffer->data[6] = 0x01;
  buffer->data[7] = 0x00;
  buffer->data[8] = 0x01;

  buffer->data[9] = 0x00;
  buffer->data[10] = 0x60;

  i = 6;

  test_attrib(buffer->data + 11,0x0004);

  memcpy(buffer->data + 11,med_attrib,6);

  i += 6;
  buffer->data[17] = 0x00;
  i++;

  prh_sdp_write16(i,buffer->data + 3);
  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);
  
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
    return HU_FAILED;

  prh_stub_l2cap_FreeWriteBuffer(buffer);

  prh_stub_l2cap_do_disconnectind(connection);

  pDebugCheck();

  return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_215
 
	*  TEST DESCRIPTION:
		Test that a service attribute with a large aid list is handled by the server.
 
	*  TEST STEPS:
		1. create connection
		2. constructe service attribute request with a large aid list.
		3. send the pdu
		4. check for the correct response.
		5. disconnect

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_215(int p1, void *arg)
{
  /*
    A standard attribute request, check response.
   */
  
  struct host_buf *buffer;
  u_int16 i;
  void* connection;
  t_bdaddr address;
  int unused;  

  u_int8 large_attrib[] = {
	0x37,0x00,0x00,0x00,0x03,
	0x09,0x00,0x04
  };

  u_int8 test_results_a[] = {0x05,0x00,0x00,0x00,0x19,0x00,0x16,0x37,0x00,0x00,
			     0x00,0x11,0x09,0x00,0x04,0x35,0x0c,0x35,0x03,0x19,
			     0x01,0x00,0x35,0x05,0x19,0x00,0x03,0x08,0x03,0x00
  };

	pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  prh_stub_l2cap_GetWriteBuffer(500,&buffer);

  buffer->data[0] = 0x04;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

  buffer->data[5] = 0x00;
  buffer->data[6] = 0x01;
  buffer->data[7] = 0x00;
  buffer->data[8] = 0x01;

  buffer->data[9] = 0x00;
  buffer->data[10] = 0x60;

  i = 6;

  test_attrib(buffer->data + 11,0x0004);

  memcpy(buffer->data + 11,large_attrib,8);

  i += 8;
  buffer->data[19] = 0x00;
  i++;

  prh_sdp_write16(i,buffer->data + 3);
  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);
  
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
    return HU_FAILED;

  prh_stub_l2cap_FreeWriteBuffer(buffer);

  prh_stub_l2cap_do_disconnectind(connection);

  pDebugCheck();

  return HU_PASSED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_216
 
	*  TEST DESCRIPTION:
		Test that a service search with a medium uuid list is handled correctly by the server.
 
	*  TEST STEPS:
		1. create connection
		2. constructe service search request with a medium uuid list.
		3. send the pdu
		4. check for the correct response.
		5. disconnect

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_216(int p1, void *arg)
{
  /*
    A standard service search request. Check the response.
   */

  struct host_buf *buffer;
  u_int16 i;
  void* connection;
  t_bdaddr address;
  int unused;  

  u_int8 med_list[8] = {
  0x36,0x00,0x05, /* list */
  0x1a,0x00,0x00,0x11,0x03 /* 32 bit offset 3 */
};

  u_int8 test_results_a[] = {0x03,0x00,0x00,0x00,0x09,0x00,0x01,0x00,0x01,0x00,
			     0x01,0x00,0x00,0x00};

	pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,0x40,&connection);

  prh_stub_l2cap_GetWriteBuffer(500,&buffer);

  buffer->data[0] = 0x02;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

	memcpy(buffer->data + 5,med_list,8);

i = 8;

  buffer->data[5 + i] = 0x00;
  buffer->data[5 + i + 1] = 0x60;
  buffer->data[5 + i + 2] = 0x00;
  i +=3;
  prh_sdp_write16(i,buffer->data + 3);

  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);
  
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
    return HU_FAILED;

  prh_stub_l2cap_FreeWriteBuffer(buffer);

  prh_stub_l2cap_do_disconnectind(connection);

	pDebugCheck();

  return HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_217
 
	*  TEST DESCRIPTION:
		Test that a service search with a large uuid list is handled correctly by the server.
 
	*  TEST STEPS:
		1. create connection
		2. constructe service search request with a large uuid list.
		3. send the pdu
		4. check for the correct error code.
		5. disconnect

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_217(int p1, void *arg)
{
  /*
    A standard service search request. Check the response.
   */

  struct host_buf *buffer;
  u_int16 i;
  void* connection;
  t_bdaddr address;
  int unused;  

  u_int8 med_list[10] = {
  0x37,0x00,0x00,0x00,0x05, /* list */
  0x1a,0x00,0x00,0x11,0x03 /* 32 bit offset 3 */
};

  u_int8 test_results_a[] = {0x03,0x00,0x00,0x00,0x09,0x00,0x01,0x00,0x01,0x00,
			     0x01,0x00,0x00,0x00};

	pDebugCheck();

  prh_stub_l2cap_setup_l2_connection(address,1,0x40,&connection);

  prh_stub_l2cap_GetWriteBuffer(500,&buffer);

  buffer->data[0] = 0x02;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

	memcpy(buffer->data + 5,med_list,10);

i = 10;

  buffer->data[5 + i] = 0x00;
  buffer->data[5 + i + 1] = 0x60;
  buffer->data[5 + i + 2] = 0x00;
  i +=3;
  prh_sdp_write16(i,buffer->data + 3);

  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);
  
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
    return HU_FAILED;

  prh_stub_l2cap_FreeWriteBuffer(buffer);

  prh_stub_l2cap_do_disconnectind(connection);

	pDebugCheck();

  return HU_PASSED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_218
 
	*  TEST DESCRIPTION:
		Test that a server can correcly encode a list containing a medium list
 
	*  TEST STEPS:
		1. create attribute with a list containing medium list.
		2. create connection
		3. constructe service attribute request
		4. send the pdu
		5. check for the response.
		6. disconnect

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_218(int p1, void *arg)
{
	  u_int32 handle;
	  t_SDP_serviceRecord* pRecord;
	  t_SDP_dataElement pE1,pE2,pE3,pE4,pE5,pE6,pElement;
	  t_SDP_dataElement* pList[10];
	  t_SDP_dataElement main_list;
	  u_int8* data;
  struct host_buf *buffer;
  u_int16 i;
  void* connection;
  t_bdaddr address;
  int unused;  
  int ret;

  u_int8 test_results_a[] = {0x00,0x67,0x00,0x60,0x37,0x00,0x00,
			     0x28,0x0c,0x09,0x00,0x04,0x36,0x28,0x06,0x36,0x28,0x03,0x26,
			     0x28,0x00,0x2e,0x2e,0x2e
  };

	  data = pMalloc(1024*10); /* 10K */
  
	ret = HU_PASSED;

	  prh_sdp_flush_database();

	  SDP_CreateRecord(&handle,&pRecord);

	  pMemset(data,'.',1024*10);
	  SDP_EncodeAttribute(SDP_TEXT,1024*10,data,&pE1);
	  pList[0] = &pE1;
	  
	  SDP_CreateList(SDP_SEQUENCE,1,pList,&pElement);

	pList[0]  = &pElement;

	SDP_CreateList(SDP_SEQUENCE,1,pList,&main_list);

	SDP_AddAttribute(pRecord,4,&main_list);

	/*
		test cleanup remove the record.
	*/

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  prh_stub_l2cap_GetWriteBuffer(500,&buffer);

  buffer->data[0] = 0x04;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

  buffer->data[5] = handle >> 24;	 /* pass in the record handle */
  buffer->data[6] = (handle >> 16) & 0xff;
  buffer->data[7] = (handle >> 8) & 0xff;
  buffer->data[8] = handle & 0xff;

  buffer->data[9] = 0x00;
  buffer->data[10] = 0x60;

  i = 6;

  test_attrib(buffer->data + 11,0x0004);

  i += 5;
  buffer->data[16] = 0x00;
  i++;

  prh_sdp_write16(i,buffer->data + 3);
  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);
  
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);


  if(buffer->len >=sizeof(test_results_a))
  {
	  if(memcmp(test_results_a,buffer->data+3,sizeof(test_results_a)))
		ret = HU_FAILED;
  } else ret = HU_FAILED;
  /*
  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
    ret =HU_FAILED; 
*/
	prh_stub_l2cap_do_disconnectind(connection);
	pFree(data);
	prh_stub_l2cap_FreeWriteBuffer(buffer);
	SDP_RemoveRecord(handle);

	return ret;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_219
 
	*  TEST DESCRIPTION:
		Test that a server can correctly encode a list containing a large list.
 
	*  TEST STEPS:
		1. create attribute with a list containing a large list.
		2. create connection
		3. constructe service attribute request
		4. send the pdu
		5. check for the correct error code.
		6. disconnect

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_219(int p1, void *arg)
{
	  u_int32 handle;
	  t_SDP_serviceRecord* pRecord;
	  t_SDP_dataElement pE1,pE2,pE3,pE4,pE5,pE6,pElement;
	  t_SDP_dataElement* pList[10];
	  t_SDP_dataElement main_list;
	  u_int8* data;
  struct host_buf *buffer;
  u_int16 i;
  void* connection;
  t_bdaddr address;
  int unused;  
  int ret;

  u_int8 test_results_a[] = {0x00,0x67,0x00,0x60,0x37,0x00,0x01,
			     0x18,0x12,0x09,0x00,0x04,0x37,0x00,0x01,0x18,0x0a,0x37,0x00,
			     0x01,0x18,0x05,0x27,0x00,0x01,0x18,0x00,0x2e,0x2e,0x2e,0x2e
  };

	  data = pMalloc(1024*70); /* 70K */
  
	ret = HU_PASSED;

	  prh_sdp_flush_database();

	  SDP_CreateRecord(&handle,&pRecord);

	  pMemset(data,'.',1024*70);
	  SDP_EncodeAttribute(SDP_TEXT,1024*70,data,&pE1); /* large text */
	  pList[0] = &pE1;
	  
	  SDP_CreateList(SDP_SEQUENCE,1,pList,&pElement);

	pList[0]  = &pElement;

	SDP_CreateList(SDP_SEQUENCE,1,pList,&main_list);

	SDP_AddAttribute(pRecord,4,&main_list);

	/*
		test cleanup remove the record.
	*/

  prh_stub_l2cap_setup_l2_connection(address,1,1,&connection);

  prh_stub_l2cap_GetWriteBuffer(500,&buffer);

  buffer->data[0] = 0x04;
  buffer->data[1] = 0x00;
  buffer->data[2] = 0x02;

  buffer->data[5] = handle >> 24;	 /* pass in the record handle */
  buffer->data[6] = (handle >> 16) & 0xff;
  buffer->data[7] = (handle >> 8) & 0xff;
  buffer->data[8] = handle & 0xff;

  buffer->data[9] = 0x00;
  buffer->data[10] = 0x60;

  i = 6;

  test_attrib(buffer->data + 11,0x0004);

  i += 5;
  buffer->data[16] = 0x00;
  i++;

  prh_sdp_write16(i,buffer->data + 3);
  buffer->len = i +5;

  prh_stub_l2cap_put_data(connection,buffer);
  
  prh_stub_l2cap_get_next_pdu(connection,&buffer,&unused);

  if(buffer->len >=sizeof(test_results_a))
  {
	  if(memcmp(test_results_a,buffer->data+3,sizeof(test_results_a)))
		ret = HU_FAILED;
  } else ret = HU_FAILED;
  
  /*
  if(prh_sdp_test_check(buffer,test_results_a,sizeof(test_results_a)) == HU_FAILED)
    ret =HU_FAILED; 
*/
	prh_stub_l2cap_do_disconnectind(connection);
	pFree(data);
	prh_stub_l2cap_FreeWriteBuffer(buffer);
	SDP_RemoveRecord(handle);

	return ret;
}

/******
*  TEST NAME:   
	prh_sdp_unit_201

	*  TEST DESCRIPTION:
		Test the operation of the server for reception of a list of aids

	*  TEST STEPS:
		1. clear database
		2. create UPF database
		3. inject SA with a list containing multiple ids.
		4. check response

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_201(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
*  TEST NAME:   
	prh_sdp_unit_202

	*  TEST DESCRIPTION:
		Test the operation of the server for reception of a range aids

	*  TEST STEPS:
		1. clear database
		2. create UPF database
		3. inject SA with a list containing an aid range
		4. check response

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_202(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
*  TEST NAME:   
	prh_sdp_unit_203

	*  TEST DESCRIPTION:
		Test the operation of the server for reception of a list of aids for SSA

	*  TEST STEPS:
		1. clear database
		2. create UPF database
		3. inject SSA with a list containing multiple ids.
		4. check response

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_203(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
*  TEST NAME:   
	prh_sdp_unit_205

	*  TEST DESCRIPTION:
		Test the operation of the server for reception of a SS with 16 uuid

	*  TEST STEPS:
		1. clear database
		2. create UPF database
		3. inject SS with a 16 uuid
		4. check response

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_205(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
*  TEST NAME:   
	prh_sdp_unit_206

	*  TEST DESCRIPTION:
		Test the operation of the server for reception of a SS with 32 uuid

	*  TEST STEPS:
		1. clear database
		2. create UPF database
		3. inject SS with a 32 uuid
		4. check response

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_206(int p1, void *arg)
{
	return HU_UNTESTED;
}




/******
*  TEST NAME:   
	prh_sdp_unit_207

	*  TEST DESCRIPTION:
		Test the operation of the server for reception of a SS with 128 uuid

	*  TEST STEPS:
		1. clear database
		2. create UPF database
		3. inject SS with a 128 uuid
		4. check response

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_207(int p1, void *arg)
{
	return HU_UNTESTED;
}


prh_t_hu_testinfo prh_sdp_server_tests [] =		
{
      	{ "TP_SDP_V_010_U",0x00,prh_sdp_unit_010, NULL, 0 },
      	{ "TP_SDP_V_011_U",0x00,prh_sdp_unit_011, NULL, 0 },
      	{ "TP_SDP_V_012_U",0x00,prh_sdp_unit_012, NULL, 0 },
	//      	{ "TP_SDP_V_013_U",0x00,prh_sdp_unit_013, NULL, 0 }, obsolete
      	{ "TP_SDP_V_014_U",0x00,prh_sdp_unit_014, NULL, 0 },
	{ "TP_SDP_V_015_U",0x00,prh_sdp_unit_015, NULL, 0 },
       	{ "TP_SDP_V_016_U",0x00,prh_sdp_unit_016, NULL, 0 },
      	{ "TP_SDP_V_017_U",0x00,prh_sdp_unit_017, NULL, 0 },
	{ "TP_SDP_V_204_U",0x00,prh_sdp_unit_204, NULL, 0 },
	{ "TP_SDP_V_208_U",0x00,prh_sdp_unit_208, NULL, 0 },
	{ "TP_SDP_V_209_U",0x00,prh_sdp_unit_209, NULL, 0 },
	{ "TP_SDP_V_210_U",0x00,prh_sdp_unit_210, NULL, 0 },
	{ "TP_SDP_V_211_U",0x00,prh_sdp_unit_211, NULL, 0 },
		{ "TP_SDP_V_212_U",0x00,prh_sdp_unit_212, NULL, 0 },
	{ "TP_SDP_V_213_U",0x00,prh_sdp_unit_213, NULL, 0 },
	{ "TP_SDP_V_214_U",0x00,prh_sdp_unit_214, NULL, 0 },
	{ "TP_SDP_V_215_U",0x00,prh_sdp_unit_215, NULL, 0 },
	{ "TP_SDP_V_216_U",0x00,prh_sdp_unit_216, NULL, 0 },
	{ "TP_SDP_V_217_U",0x00,prh_sdp_unit_217, NULL, 0 },
	{ "TP_SDP_V_218_U",0x00,prh_sdp_unit_218, NULL, 0 },
	{ "TP_SDP_V_219_U",0x00,prh_sdp_unit_219, NULL, 0 },
#ifndef P7D_PT_RELEASE_TESTS
      	{ "TP_SDP_V_201_U",0x00,prh_sdp_unit_201, NULL, 0 }, 
      	{ "TP_SDP_V_202_U",0x00,prh_sdp_unit_202, NULL, 0 }, 
      	{ "TP_SDP_V_203_U",0x00,prh_sdp_unit_203, NULL, 0 }, 
      	{ "TP_SDP_V_205_U",0x00,prh_sdp_unit_205, NULL, 0 }, 
      	{ "TP_SDP_V_206_U",0x00,prh_sdp_unit_206, NULL, 0 }, 
      	{ "TP_SDP_V_207_U",0x00,prh_sdp_unit_207, NULL, 0 }, 
#endif
};

prh_t_hu_harness prh_sdp_server_group =
{ NULL, HU_SDP, "SDP_SERVER",prh_sdp_unit_init,PRH_UT_STUBS_ENABLED, 1,sizeof(prh_sdp_server_tests)/sizeof(prh_t_hu_testinfo), prh_sdp_server_tests, 0, 0, 0, 0, 0};

void test_sdp_server(void)
{
  prh_hu_add_testgroup(&prh_sdp_server_group);
}
