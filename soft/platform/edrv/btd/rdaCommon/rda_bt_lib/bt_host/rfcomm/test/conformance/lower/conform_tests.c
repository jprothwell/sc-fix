#include "conformance_tests.h"
#include "rftest_types.h"
#include "rftest_pdu_gen.h"
#include "rftest_proto.h"

extern int test_num;

u_int32 prh_test_rfcomm_iut_no_mmi(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
  /* no need to do anything other side drives */
  return TEST_UNKNOWN;
}

u_int32 prh_test_rfcomm_iut_not_impl(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
  /* Not applicable */
  return TEST_ERROR;
}

u_int32 prh_test_rfcomm_iut_TP_RFC_BV_01_C(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
  t_DataBuf *pdu_buf;
  int ret, dummy;

  u_int8 expected_answer[]={ 0x03, 0x3F, 0x01, 0x1C };
  u_int8 expected_length=4;

  rftest_init();

  /* get the first PDU that arrive on RFCOMM should be SABM */
  rftest_takepdu(&pdu_buf, &dummy);
  /* the answer should be "03 3F 01 1C" */
  if (!(pMemcmp(pdu_buf->data, expected_answer, expected_length)))
    ret=TEST_PASS;
  else 
    ret=TEST_FAIL;
    
  L2_FreeWriteBuffer(pdu_buf);
  rftest_finish();
  return ret;
}

u_int32 prh_test_rfcomm_iut_TP_RFC_BV_02_C(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
  t_DataBuf *pdu_buf;
  int ret, dummy;

  u_int8 expected_answer[]={ 0x03, 0x73, 0x01, 0xD7 };
  u_int8 expected_length=4;

  rftest_init();
  rftest_l2cap_connect(remoteAddress);
  /* Need to establish L2CAP */
  rftest_sendsabm(0, 1, 1);
  rftest_takepdu(&pdu_buf, &dummy);

  if (!(pMemcmp(pdu_buf->data, expected_answer, expected_length)))
    ret=TEST_PASS;
  else 
    ret=TEST_FAIL;
    
  L2_FreeWriteBuffer(pdu_buf);
  rftest_finish();
  return ret;
}

u_int32 prh_test_rfcomm_iut_TP_RFC_BV_03_C(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
  t_DataBuf *pdu_buf;
  int ret, dummy;

  u_int8 expected_answer[]={ 0x03, 0x73, 0x01, 0xD7 };
  u_int8 expected_length=4;
  
  rftest_init();

  rftest_l2cap_connect(remoteAddress);
  rftest_sendsabm(0, 1, 1);
  rftest_dumppdu();
  
  rftest_senddisc(0, 1, 1);
  rftest_takepdu(&pdu_buf, &dummy);

  if (!(pMemcmp(pdu_buf->data, expected_answer, expected_length)))
    ret=TEST_PASS;
  else 
    ret=TEST_FAIL;

  /* verify PDU is a DISC */
  L2_FreeWriteBuffer(pdu_buf);

  rftest_finish();
  return ret;
}

u_int32 prh_test_rfcomm_iut_TP_RFC_BV_05_C(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
  t_DataBuf *pdu_buf;
  int pdu_type;
  int ret;

  u_int8 expected_answer[]={ 0x13, 0x3F, 0x01, 0x96 };
  u_int8 expected_length=4;

  rftest_init();

  /* get the first PDU that arrive on RFCOMM should be SABM */
  rftest_dumppdu();
  rftest_sendua(0, 1, 0);
  
  do {
    rftest_takepdu(&pdu_buf, &pdu_type);
    if (pdu_type==PRH_RFTEST_PDU_DLCI0)
      {
	int type;
	type=pdu_buf->data[PRH_RFCOMM_CC_TYPE_FIELD] & 0xFC;
	if (type==PRH_RFCOMM_CC_TYPE_PN)
	  rftest_sendpn(4, 0, 127);
	L2_FreeWriteBuffer(pdu_buf);
      }
  } while(pdu_type!=PRH_RFTEST_PDU_COMMAND);
  
  if (!(pMemcmp(pdu_buf->data, expected_answer, expected_length)))
    ret=TEST_PASS;
  else 
    ret=TEST_FAIL;
    
  L2_FreeWriteBuffer(pdu_buf);
  rftest_finish();
  return ret;
}


u_int32 prh_test_rfcomm_iut_TP_RFC_BV_06_C(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
  t_DataBuf *pdu_buf;
  int ret, pdu_type;

  u_int8 expected_answer[]={ 0x13, 0x73, 0x01, 0x5D };
  u_int8 expected_length=4;

  rftest_init();
  rftest_l2cap_connect(remoteAddress);
  rftest_sendsabm(0, 1, 1);
  rftest_dumppdu();

  rftest_sendsabm(4, 1, 1);
  rftest_takepdu(&pdu_buf, &pdu_type);

  /* verify UA */  
  if (!(pMemcmp(pdu_buf->data, expected_answer, expected_length)))
    ret=TEST_PASS;
  else 
    ret=TEST_FAIL;

  L2_FreeWriteBuffer(pdu_buf);
  rftest_finish();
  return ret;
}

u_int32 prh_test_rfcomm_iut_TP_RFC_BV_07_C(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
 t_DataBuf *pdu_buf;
  int pdu_type;
  int ret;

  u_int8 expected_answer[]={ 0x11, 0x53, 0x01, 0x16 };
  u_int8 expected_length=4;

  rftest_init();
  rftest_l2cap_connect(remoteAddress);  

  rftest_sendsabm(0, 1, 1);
  rftest_dumppdu();

  rftest_sendsabm(4, 1, 1);
  rftest_dumppdu();

  do {
    rftest_takepdu(&pdu_buf, &pdu_type);
    if (pdu_type==PRH_RFTEST_PDU_DLCI0)
      {
	int type;
	type=pdu_buf->data[PRH_RFCOMM_CC_TYPE_FIELD] & 0xFC;
	if (type==PRH_RFCOMM_CC_TYPE_MSC)
	  rftest_sendmsc(4, 0, NULL);
	L2_FreeWriteBuffer(pdu_buf);
      }
  } while(pdu_type!=PRH_RFTEST_PDU_COMMAND);

  if (!(pMemcmp(pdu_buf->data, expected_answer, expected_length)))
    ret=TEST_PASS;
  else 
    ret=TEST_FAIL;
    
  L2_FreeWriteBuffer(pdu_buf);
  rftest_finish();
  return ret;
}
 

u_int32 prh_test_rfcomm_iut_TP_RFC_BV_08_C(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
  t_DataBuf *pdu_buf;
  int ret, pdu_type;

  u_int8 expected_answer[]={ 0x13, 0x73, 0x01, 0x5D };
  u_int8 expected_length=4;

  rftest_init();
  rftest_l2cap_connect(remoteAddress);
  rftest_sendsabm(0, 1, 1);
  rftest_dumppdu();

  rftest_sendsabm(4, 1, 1);
  rftest_dumppdu();

  rftest_senddisc(4, 1, 1);
  rftest_takepdu(&pdu_buf, &pdu_type);

  if (!(pMemcmp(pdu_buf->data, expected_answer, expected_length)))
    ret=TEST_PASS;
  else 
    ret=TEST_FAIL;

  L2_FreeWriteBuffer(pdu_buf);
  rftest_finish();
  return ret;
}

u_int32 prh_test_rfcomm_iut_TP_RFC_BV_09_C(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
  prh_t_rfcomm_control_params ctrl_params;
  int ret=TEST_UNKNOWN;

  rftest_init();
  rftest_l2cap_connect(remoteAddress);
  rftest_sendsabm(0, 1, 1);
  rftest_dumppdu();

  rftest_sendsabm(4, 1, 1);
  rftest_dumppdu();

  ctrl_params.break_signal=0;
  ctrl_params.modem_signal=PRH_RFCOMM_MSC_DTRDSR | PRH_RFCOMM_MSC_RTSCTS | PRH_RFCOMM_MSC_DCD;
  ctrl_params.flow_state=PRH_RFCOMM_FLOW_DATA_GO; 

  rftest_sendmsc(4, 1, &ctrl_params);
  rftest_dumppdu();

  pDebugSleep(5);

  ctrl_params.break_signal=0;
  ctrl_params.modem_signal=PRH_RFCOMM_MSC_DCD;
  ctrl_params.flow_state=PRH_RFCOMM_FLOW_DATA_STOP;
  rftest_sendmsc(4, 1, &ctrl_params);
  rftest_dumppdu();

  rftest_finish();
  return ret;
}

u_int32 prh_test_rfcomm_iut_TP_RFC_BV_10_C(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
  prh_t_rfcomm_control_params ctrl_params;
  int ret=TEST_UNKNOWN;

  rftest_init();
  rftest_l2cap_connect(remoteAddress);
  rftest_sendsabm(0, 1, 1);
  rftest_dumppdu();

  rftest_sendsabm(4, 1, 1);
  rftest_dumppdu();
   
  ctrl_params.break_signal=0;
  ctrl_params.modem_signal=PRH_RFCOMM_MSC_DTRDSR | PRH_RFCOMM_MSC_RTSCTS | PRH_RFCOMM_MSC_DCD;
  ctrl_params.flow_state=PRH_RFCOMM_FLOW_DATA_GO;
  rftest_sendmsc(4, 1, &ctrl_params);
  rftest_dumppdu();
 
  /* Do data transfer */
  rftest_finish();
  return ret;
}

u_int32 prh_test_rfcomm_iut_TP_RFC_BV_11_C(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
  t_DataBuf *pdu_buf;
  int ret=TEST_FAIL, pdu_type;
  /* 21 15 41 42 43 44 45 46 47 48 49 4A AA */
  u_int8 expected_answer[]={ 0x21, 0x15, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0xAA};
  u_int8 expected_length=13;

  rftest_init();
  rftest_l2cap_connect(remoteAddress);
  rftest_sendsabm(0, 1, 1);
  rftest_dumppdu();

  rftest_sendtest(1);
  rftest_takepdu(&pdu_buf, &pdu_type);

  if (!(pMemcmp(pdu_buf->data, expected_answer, expected_length)))
    ret=TEST_PASS;
  else 
    ret=TEST_FAIL;

  L2_FreeWriteBuffer(pdu_buf);
  rftest_finish();
  return ret;


}

u_int32 prh_test_rfcomm_iut_TP_RFC_BV_12_C(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
 prh_t_rfcomm_control_params ctrl_params;
 int ret=TEST_UNKNOWN;

 rftest_init();
 rftest_l2cap_connect(remoteAddress);
 rftest_sendsabm(0, 1, 1);
 rftest_dumppdu();
 rftest_sendsabm(4, 1, 1);
 rftest_dumppdu();
 
 ctrl_params.break_signal=0;
 ctrl_params.modem_signal=PRH_RFCOMM_MSC_DTRDSR | PRH_RFCOMM_MSC_RTSCTS | PRH_RFCOMM_MSC_DCD;
 ctrl_params.flow_state=PRH_RFCOMM_FLOW_DATA_GO; 
 rftest_sendmsc(4, 1, &ctrl_params);
 rftest_dumppdu();
 
 pDebugSleep(2);
 rftest_send_fcoff(1);
 rftest_dumppdu();
 pDebugSleep(5);
 rftest_send_fcon(1);
 rftest_dumppdu();
 rftest_finish();

 return ret;
}

u_int32 prh_test_rfcomm_iut_TP_RFC_BV_13_C(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
   t_DataBuf *pdu_buf;
  int ret=TEST_FAIL, pdu_type;
  /* 51 05 13 05 AA */ 
  u_int8 expected_answer[]={ 0x51, 0x05, 0x13, 0x05, 0xAA };
  u_int8 expected_length=5;

  rftest_init();
  rftest_l2cap_connect(remoteAddress);
  rftest_sendsabm(0, 1, 1);
  rftest_dumppdu();

  rftest_sendsabm(4, 1, 1);
  rftest_dumppdu();

  rftest_sendrls(4, 1, PRH_RFCOMM_LSTAT_PARITY_ERROR|PRH_RFCOMM_LSTAT_ERROR);
  rftest_takepdu(&pdu_buf, &pdu_type);

  if (!(pMemcmp(pdu_buf->data, expected_answer, expected_length)))
    ret=TEST_PASS;
  else 
    ret=TEST_FAIL;

  L2_FreeWriteBuffer(pdu_buf);
  rftest_finish();
  return ret;
}

u_int32 prh_test_rfcomm_iut_TP_RFC_BV_15_C(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
  t_DataBuf *pdu_buf;
  int ret=TEST_FAIL, pdu_type;
  u_int16 sent_frame_size=500, ret_frame_size;
  /* 51 05 13 05 AA */ 
  /* 81 11 04 00 00 00 F4 01 00 00 AA */
  u_int8 expected_answer[]={ 0x81, 0x11, 0x04, 0x00, 0x00, 0x00 };
  u_int8 expected_length=5;

  rftest_init();
  rftest_l2cap_connect(remoteAddress);
  rftest_sendsabm(0, 1, 1);
  rftest_dumppdu();

  rftest_sendpn(4, 1, sent_frame_size);
  rftest_takepdu(&pdu_buf, &pdu_type);
  
  /* check we got a PN on DLCI 4 and all zeros in right places*/
  if (!(pMemcmp(pdu_buf->data, expected_answer, 6)))
    ret=TEST_PASS;
  else 
    {
      ret=TEST_FAIL;    
      goto free_out;
    }
     
  ret_frame_size=pdu_buf->data[6]+(pdu_buf->data[7]<<8);
  if (ret_frame_size>sent_frame_size)
    {
      ret=TEST_FAIL;
      goto free_out;
    }
  
  
  L2_FreeWriteBuffer(pdu_buf);
  sent_frame_size=64;
  rftest_sendpn(4, 1, sent_frame_size);
  rftest_takepdu(&pdu_buf, &pdu_type);
  
  if (!(pMemcmp(pdu_buf->data, expected_answer, 6)))
    ret=TEST_PASS;
  else 
    {
      ret=TEST_FAIL;    
      goto free_out;
    }
     
  ret_frame_size=pdu_buf->data[6]+(pdu_buf->data[7]<<8);
  if (ret_frame_size>sent_frame_size)
    {
      ret=TEST_FAIL;
      goto free_out;
    }
  
free_out:
  L2_FreeWriteBuffer(pdu_buf);

  rftest_finish();
  return ret;
}

u_int32 prh_test_rfcomm_iut_TP_RFC_BV_17_C(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
  t_DataBuf *pdu_buf;
  prh_t_rfcomm_port_params port_params;
  int ret=TEST_FAIL, pdu_type;
  /* 91 11 13 03 03 0C 11 13 6F 0C AA */
  u_int8 expected_answer[]={ 0x91, 0x11, 0x13, 0x03, 0x03, 0x0C, 0x11, 0x13, 0x6F, 0x0C, 0xAA };
  u_int8 expected_length=11;

  rftest_init();

  port_params.port_speed=RF9600;
  port_params.data_bits=RFDS8;
  port_params.stop_bits=RFSTOP1;
  port_params.parity_enable=RFNONE;
  port_params.paritytype=RFODD;
  port_params.flow_mask=PRH_RFCOMM_CC_RPN_FLC_IRTR | PRH_RFCOMM_CC_RPN_FLC_ORTR;
  port_params.xon=17;
  port_params.xoff=19;
  port_params.param_mask=PRH_RFCOMM_CC_RPN_PM_BITRATE|PRH_RFCOMM_CC_RPN_PM_DATABITS | PRH_RFCOMM_CC_RPN_PM_STOPBITS | PRH_RFCOMM_CC_RPN_PM_PARITY | PRH_RFCOMM_CC_RPN_PM_XON_CH | PRH_RFCOMM_CC_RPN_PM_XOFF_CH | PRH_RFCOMM_CC_RPN_PM_RTR_INPUT | PRH_RFCOMM_CC_RPN_PM_RTR_OUTPUT;

  /* set up DLCI 0 */
  rftest_l2cap_connect(remoteAddress);
  rftest_sendsabm(0, 1, 1);
  rftest_dumppdu();

  /* actual test */
  rftest_sendrpnc(4, 1, &port_params);
  rftest_takepdu(&pdu_buf, &pdu_type);


  if (!(pMemcmp(pdu_buf->data, expected_answer, expected_length)))
    ret=TEST_PASS;
  else 
    ret=TEST_FAIL;
  L2_FreeWriteBuffer(pdu_buf);
  rftest_finish();
  return ret;
}

u_int32 prh_test_rfcomm_iut_TP_RFC_BV_19_C(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
   t_DataBuf *pdu_buf;
  int ret=TEST_FAIL, pdu_type;
  /* this test makes sure we get an RPN response for our DLC as the pass */
  /* 91 11 13 */ 
  u_int8 expected_answer[]={ 0x91, 0x11, 0x13 };
  u_int8 expected_length=3;

  rftest_init();

  rftest_l2cap_connect(remoteAddress);
  rftest_sendsabm(0, 1, 1);
  rftest_dumppdu();
  
  rftest_sendrpnr(4, 1);
  rftest_takepdu(&pdu_buf, &pdu_type);

  if (!(pMemcmp(pdu_buf->data, expected_answer, expected_length)))
    ret=TEST_PASS;
  else 
    ret=TEST_FAIL;

  L2_FreeWriteBuffer(pdu_buf);
  rftest_finish();
  return ret;
}

struct testEntry lower_rfcomm_conformance_tests[] = 
{
  { "TP/RFC/BV-01-C", prh_test_rfcomm_iut_TP_RFC_BV_01_C, 0, TEST_NOT_BATCHED|TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-02-C", prh_test_rfcomm_iut_TP_RFC_BV_02_C, 0, TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-03-C", prh_test_rfcomm_iut_TP_RFC_BV_03_C, 0, TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-04-C", prh_test_rfcomm_iut_not_impl, 0, TEST_NOT_BATCHED|TEST_RESET_SYSTEM, NULL }, 
  { "TP/RFC/BV-05-C", prh_test_rfcomm_iut_TP_RFC_BV_05_C, 0, TEST_NOT_BATCHED|TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-06-C", prh_test_rfcomm_iut_TP_RFC_BV_06_C, 0, TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-07-C", prh_test_rfcomm_iut_TP_RFC_BV_07_C, 0, TEST_NOT_BATCHED|TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-08-C", prh_test_rfcomm_iut_TP_RFC_BV_08_C, 0, TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-09-C", prh_test_rfcomm_iut_TP_RFC_BV_09_C, 0, TEST_NOT_BATCHED|TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-10-C", prh_test_rfcomm_iut_TP_RFC_BV_10_C, 0, TEST_NOT_BATCHED|TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-11-C", prh_test_rfcomm_iut_TP_RFC_BV_11_C, 0, TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-12-C", prh_test_rfcomm_iut_TP_RFC_BV_12_C, 0, TEST_NOT_BATCHED|TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-13-C", prh_test_rfcomm_iut_TP_RFC_BV_13_C, 0, TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-14-C", prh_test_rfcomm_iut_not_impl, 0, TEST_NOT_BATCHED|TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-15-C", prh_test_rfcomm_iut_TP_RFC_BV_15_C, 0, TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-17-C", prh_test_rfcomm_iut_TP_RFC_BV_17_C, 0, TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-19-C", prh_test_rfcomm_iut_TP_RFC_BV_19_C, 0, TEST_RESET_SYSTEM, NULL }
};

struct groupEntry lower_rfcomm_conformance_group = 
{
  "RFCOMM Lower",
  lower_rfcomm_conformance_tests,
  0, 
  GROUP_NOFLAGS, 
  ""
};

