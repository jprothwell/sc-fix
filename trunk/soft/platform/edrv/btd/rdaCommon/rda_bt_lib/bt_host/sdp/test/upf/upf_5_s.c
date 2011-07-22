/*
	upf5 SDP server
*/

#include "host_config.h"
#include "host_types.h"
#include "sdp_db.h"
#include "sdp_server.h"
#include "mgr_core.h"
#include "sdp_ue.h"

void upf_5_dialup(void);
void upf_5_fax(void);
void upf_5_obex(void);

#include "host_ctrl.h"

#define WIN_TEST 1

#if WIN_TEST
DWORD WINAPI main_loop_wrapper(LPVOID Arg)
{
	BT_Main(0);

	return 0;
}
#endif

void main(void)
{

#if WIN_TEST

	BT_Init(0);

	CreateThread(NULL,0,main_loop_wrapper,0,0,NULL);
#else
	sys_init(1);

#endif

	while(MGR_Init(0) == BT_RETRY);


	HCI_WriteScanEnable(3);

	pDebugCheck();

	upf_5_dialup();
	upf_5_fax();
	upf_5_obex();

	pDebugCheck();
	
	SDP_RegisterPSM();

	while(1) Sleep(500);
}

void upf_5_dialup(void)
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





void upf_5_fax(void)
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





void upf_5_obex(void)
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
