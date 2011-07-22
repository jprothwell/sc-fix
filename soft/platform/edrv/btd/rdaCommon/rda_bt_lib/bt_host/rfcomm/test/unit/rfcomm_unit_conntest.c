#include <stdio.h>
#include "host_config.h"
#include "host_types.h"
#include "host_error.h"
#include "cunit.h"
#include "assert.h"
#include "papi.h"
#include "rftest_pdu_gen.h"
#include "rftest_types.h"

#include "hu_harness.h"

#define RFCOMM_PSM 3
#define TEST_CID 64
#define TEST_REMOTE_CHAN 5

#define PRH_PORT_ENT_CONNECT 0
#define PRH_PORT_ENT_DISCONNECT 1
#define PRH_PORT_ENT_CONNECTFAILED 2

t_pEvent test_event;
int test_outcome;

void UT_RFCOMM_SYS_RESET(void);

void UT_RFCOMM_test_connect_cb(u_int8 schan, t_bdaddr bdaddr, u_int8 status)
{
  fprintf(stderr, "test connected %d\n", status);
#if 0
  if (status==PRH_PORT_ENT_CONNECT)
    test_outcome=1;
  else
    test_outcome=0;
#endif
#if 0
  pEventSet(test_event);
#endif
  return;
}

int UT_RFCOMM_test_init(int d1, void *d2)
{
  UT_RFCOMM_SYS_RESET();
  return 0;
}
  

void UT_TP_RFC_not_used(void)
{
  int ret;
  void *conn_handle;
  t_bdaddr remote_dev;
  struct host_buf *pdu_buf;
  int pdu_type;
  prh_t_hu_outcome outcome;
  pMemcpy(remote_dev.bytes, "\x00\x01\x02\x03\x04\x05", 6);

  outcome=HU_PASSED;
  test_event=pEventCreate(FALSE);

  ret=PE_CliConnectPort(remote_dev, TEST_REMOTE_CHAN, "hello", UT_RFCOMM_test_connect_cb);

  RFCOMMLT_GetLastConnectHandle(&conn_handle);
  /* feed in a connect response */
  
  RFCOMMLT_Send_ConnectConfirm(conn_handle);
  
  RFCOMMLT_Send_ConfigureIndication(conn_handle);
  RFCOMMLT_Send_ConfigureComplete(conn_handle);

  rftest_takepdu(&pdu_buf, &pdu_type);
  fprintf(stderr,"pdu_buf size is %d\n", pdu_buf->len);

  rftest_sendua(conn_handle, 0, 1, 1);

  rftest_takepdu(&pdu_buf, &pdu_type);
  fprintf(stderr,"pdu_buf size is %d\n", pdu_buf->len);

  rftest_sendpn(conn_handle, 10, 0, 127);

  rftest_takepdu(&pdu_buf, &pdu_type);
  rftest_sendua(conn_handle, 10, 1, 1);

  if (ret==BT_PENDING)
    pEventWait(test_event);
  else
    outcome=HU_FAILED;
  return;
}

prh_t_hu_outcome UT_TP_RFC_BV_01_C(int p1, void *arg)
{
  int ret;
  void *conn_handle;
  t_bdaddr remote_dev;
  struct host_buf *pdu_buf;
  int pdu_type;
  u_int8 expected_answer[]={ 0x03, 0x3F, 0x01, 0x1C };
  u_int8 expected_length=4;
  prh_t_hu_outcome outcome=HU_FAILED;
  
  pMemcpy(remote_dev.bytes, "\x00\x01\x02\x03\x04\x05", 6);

  ret=PE_CliConnectPort(remote_dev, TEST_REMOTE_CHAN, "hello", UT_RFCOMM_test_connect_cb);
  if(ret==BT_PENDING)
    {
      prh_stub_l2cap_accept_l2_connection(&conn_handle);
      
      rftest_takepdu(&pdu_buf, &pdu_type);
      fprintf(stderr,"pdu_buf size is %d\n", pdu_buf->len);
      
      rftest_sendua(conn_handle, 0, 1, 1);
      
      if (!(pMemcmp(pdu_buf->data, expected_answer, expected_length)))
        outcome=HU_PASSED;
      else
	outcome=HU_FAILED;
    }

  return outcome;
}

prh_t_hu_outcome UT_TP_RFC_BV_02_C(int p1, void *arg)
{
  int ret, dummy;
  void *conn_handle;
  t_bdaddr remote_dev;
  struct host_buf *pdu_buf;
  int pdu_type;
  u_int8 expected_answer[]={ 0x03, 0x73, 0x01, 0xD7 };
  u_int8 expected_length=4;
  prh_t_hu_outcome outcome=HU_FAILED;

  pMemcpy(remote_dev.bytes, "\x00\x01\x02\x03\x04\x05", 6);
  
  ret=prh_stub_l2cap_setup_l2_connection(remote_dev, RFCOMM_PSM, TEST_CID, &conn_handle);
  if (ret==BT_NOERROR)
    {
      rftest_sendsabm(conn_handle, 0, 1, 1);
      rftest_takepdu(&pdu_buf, &dummy);
      
      if (!(pMemcmp(pdu_buf->data, expected_answer, expected_length)))
	outcome=HU_PASSED;
      
    }
  else
    outcome=HU_FAILED;
  return outcome;
}

prh_t_hu_outcome UT_TP_RFC_BV_03_C(int p1, void *arg)
{
  int ret, dummy;
  void *conn_handle;
  t_bdaddr remote_dev;
  struct host_buf *pdu_buf;
  int pdu_type;
  prh_t_hu_outcome outcome=HU_FAILED;
  u_int8 expected_answer[]={ 0x03, 0x73, 0x01, 0xD7 };
  u_int8 expected_length=4;


  pMemcpy(remote_dev.bytes, "\x00\x01\x02\x03\x04\x05", 6);

  ret=prh_stub_l2cap_setup_l2_connection(remote_dev, RFCOMM_PSM, TEST_CID, &conn_handle);
  if (ret==BT_NOERROR)
    {
      rftest_sendsabm(conn_handle, 0, 1, 1);
      rftest_dumppdu();
      rftest_senddisc(conn_handle, 0, 1, 1);
      
      rftest_takepdu(&pdu_buf, &dummy);
      
      if (!(pMemcmp(pdu_buf->data, expected_answer, expected_length)))
	outcome=HU_PASSED;
      
    }
  else
    outcome=HU_FAILED;
  return outcome;
}

prh_t_hu_outcome UT_TP_RFC_BV_05_C(int p1, void *arg)
{
  int ret, dummy;
  void *conn_handle;
  t_bdaddr remote_dev;
  struct host_buf *pdu_buf;
  int pdu_type;
  int ptype;
  prh_t_hu_outcome outcome=HU_FAILED;
  u_int8 expected_answer[]={ 0x2B, 0x3F, 0x01, 0x8c };
  u_int8 expected_length=4;

  pMemcpy(remote_dev.bytes, "\x00\x01\x02\x03\x04\x05", 6);

  ret=PE_CliConnectPort(remote_dev, TEST_REMOTE_CHAN, "hello", UT_RFCOMM_test_connect_cb);

  if (ret==BT_PENDING)
    {
      prh_stub_l2cap_accept_l2_connection(&conn_handle);
      
      rftest_dumppdu();
      rftest_sendua(conn_handle, 0, 1, 1);
      
      do {
	rftest_takepdu(&pdu_buf, &pdu_type);
	
	ptype=pdu_buf->data[1];
	if (ptype==0xef) /* check is this a UIH frame */
	  {
	    int type;
	    
	    if ((pdu_buf->data[3] & 0xFC) == PRH_RFCOMM_CC_TYPE_PN)
	      rftest_sendpn(conn_handle, pdu_buf->data[5], 0, 127);
	    L2_FreeWriteBuffer(pdu_buf);
	  }
      } while(ptype!=0x3f);
      
      if (!(pMemcmp(pdu_buf->data, expected_answer, expected_length)))
	outcome=HU_PASSED;
      else
	outcome=HU_FAILED;
    }
  else
    outcome=HU_FAILED;
  return outcome;
}

prh_t_hu_outcome UT_TP_RFC_BV_06_C(int p1, void *arg)
{
  int ret, dummy;
  void *conn_handle;
  t_bdaddr remote_dev;
  struct host_buf *pdu_buf;
  int pdu_type;
  int chan=TEST_REMOTE_CHAN;
  prh_t_hu_outcome outcome=HU_FAILED;  
  u_int8 expected_answer[]={ 0x2B, 0x73, 0x01, 0x47 };
  u_int8 expected_length=4;

  ret=PE_SrvRegisterPort("/dev/ptyr0", &chan, UT_RFCOMM_test_connect_cb);
  if (ret==BT_NOERROR)
    {
      pMemcpy(remote_dev.bytes, "\x00\x01\x02\x03\x04\x05", 6);

      ret=prh_stub_l2cap_setup_l2_connection(remote_dev, RFCOMM_PSM, TEST_CID, &conn_handle);      
      if (ret==BT_NOERROR)
	{
	  rftest_sendsabm(conn_handle, 0, 1, 1);
	  rftest_dumppdu();
	  
	  rftest_sendsabm(conn_handle, 10, 1, 1);
	  rftest_takepdu(&pdu_buf, &pdu_type);
	  
	  if (!(pMemcmp(pdu_buf->data, expected_answer, expected_length)))
	    outcome=HU_PASSED;
	  
	}
      else
	outcome=HU_FAILED;
    }
  else
    outcome=HU_FAILED;
  //cuReport("Connect Test Failed\n", test_outcome==1);
  return outcome;
}

prh_t_hu_outcome UT_TP_RFC_BV_07_C(int p1, void *arg)
{
  int ret, dummy;
  void *conn_handle;
  t_bdaddr remote_dev;
  struct host_buf *pdu_buf;
  int pdu_type;
  int chan=TEST_REMOTE_CHAN;
  int ptype;
  prh_t_rfcomm_control_params ctrl_params;
  prh_t_hu_outcome outcome=HU_FAILED;

  u_int8 expected_answer[]={ 0x29, 0x53, 0x01, 0x0C };  
  u_int8 expected_length=4;

  ret=PE_SrvRegisterPort("/dev/ptyr5", &chan, UT_RFCOMM_test_connect_cb);

  if (ret==BT_NOERROR)
    {
      pMemcpy(remote_dev.bytes, "\x00\x01\x02\x03\x04\x05", 6);

      ret=prh_stub_l2cap_setup_l2_connection(remote_dev, RFCOMM_PSM, TEST_CID, &conn_handle);            
      if (ret==BT_NOERROR)
	{
	  rftest_sendsabm(conn_handle, 0, 1, 1);
	  rftest_dumppdu();
	  
	  rftest_sendsabm(conn_handle, 10, 1, 1);
	  rftest_takepdu(&pdu_buf, &pdu_type);
	  
	  do {
	    rftest_takepdu(&pdu_buf, &pdu_type);
	    
	    ptype=pdu_buf->data[1];
	    if (ptype==0xef) /* check is this a UIH frame */
	      {
		int type;
				
		if ((pdu_buf->data[3] & 0xFC) == PRH_RFCOMM_CC_TYPE_MSC)
		  {
		    pMemset(&ctrl_params, 0, sizeof(ctrl_params));
		    rftest_sendmsc(conn_handle, 10, 0, &ctrl_params);
		    PE_SrvDisconnectPort(5);
		  }
		L2_FreeWriteBuffer(pdu_buf);
	      }
	  } while(ptype!=0x53);
	  
	  
	  if (!(pMemcmp(pdu_buf->data, expected_answer, expected_length)))
	    test_outcome=1;
	  L2_FreeWriteBuffer(pdu_buf);
	}
      else
	test_outcome=0;
    }
  else
    test_outcome=0;
  cuReport("TP_RFC_BV_07_C failed\n", test_outcome==1);
  return;
}


void UT_RFCOMM_SYS_RESET(void)
{
  //  prh_tcs_reset();
  UNIT_STUB_RESET_L2();
  prh_rfcomm_layer_ctrl(PRH_HOST_SHUTDOWN);
  prh_rfcomm_layer_ctrl(PRH_HOST_STARTUP);

}

prh_t_hu_testlist prh_hu_rfcomm_testlist [] =  
{
  { "TP_RFC_BV_01_C", UT_RFCOMM_test_init, UT_TP_RFC_BV_01_C, NULL, 0 },
  { "TP_RFC_BV_02_C", UT_RFCOMM_test_init, UT_TP_RFC_BV_02_C, NULL, 0 },
  { "TP_RFC_BV_03_C", UT_RFCOMM_test_init, UT_TP_RFC_BV_03_C, NULL, 0 },
  { "TP_RFC_BV_05 C", UT_RFCOMM_test_init, UT_TP_RFC_BV_05_C, NULL, 0 },
  { "TP_RFC_BV_06_C", UT_RFCOMM_test_init, UT_TP_RFC_BV_06_C, NULL, 0 },
  { "TP_RFC_BV_07_C", UT_RFCOMM_test_init, UT_TP_RFC_BV_07_C, NULL, 0 },
};

prh_t_hu_harness prh_hu_rfcomm_testgroup =
{ NULL, HU_RFCOMM, "RFCOMM Connection Tests", 0, 6, prh_hu_rfcomm_testlist, 0, 0, 0, 0, 0};


void register_rfcomm_tests(void)
{
  prh_hu_add_testgroup(&prh_hu_rfcomm_testgroup);
}
