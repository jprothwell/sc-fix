#include "conformance_tests.h"

extern t_bdaddr bd_addr_inq;

t_pEvent test_event;

void tester_port_remote_callback(u_int8 schan, t_bdaddr bdaddr, u_int8 state)
{
  if (state==RFCOMM_PE_DISCONNECTED)
    {
      Test_Print("Remote Disconnected remote server channel: %d\n", schan); 
      //      printf("Disconnected\n");
    }
}


void tester_port_local_callback(u_int8 schan, t_bdaddr bdaddr, u_int8 state)
{
  if (state==RFCOMM_PE_CONNECTED)
    {
      Test_Print("Remote Connected local server channel: %d\n", schan); 
      pEventSet(test_event);

      //      pDebugPrintf((pLOGNOTICE,"APP: Got Connect on schan %d from %s\n", schan, prh_mgr_printBDAddress(bdaddr)));
      
      // tester_multi_inst=(((struct prh_rfcomm_pe_port *)portid)->multi_inst);
    }
  else if (state==RFCOMM_PE_DISCONNECTED)
    {
      Test_Print("Remote Disconnected from local server channel: %d\n", schan); 
      //      pDebugPrintf((pLOGNOTICE,"APP: Disconnected from schan %d from %s\n", schan, prh_mgr_printBDAddress(bdaddr)));
    }
}

u_int32 prh_test_rfcomm_iut_no_mmi(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
  return TEST_UNKNOWN;
}

u_int32 prh_test_rfcomm_iut_not_impl(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
  return TEST_UNKNOWN;

}

u_int32 prh_test_rfcomm_iut_TP_RFC_BV_01_C(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
  int ret;
  ret=PE_CliConnectPort(remoteAddress, 2, "/dev/ptyr1", tester_port_remote_callback);
  return TEST_UNKNOWN;
}

u_int32 prh_test_rfcomm_iut_TP_RFC_BV_05_C(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
  u_int8 test_schan=2;
  int ret;
  ret=PE_CliConnectPort(remoteAddress, test_schan, "/dev/ptyr1", tester_port_remote_callback);
  return TEST_UNKNOWN;
}

u_int32 prh_test_rfcomm_iut_TP_RFC_BV_07_C(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
  u_int8 test_schan=2;
  int ret;
  
  pEventWait(test_event);
  PE_SrvDisconnectPort(test_schan);
  return TEST_UNKNOWN;
}

u_int32 prh_test_rfcomm_iut_TP_RFC_BV_09_C(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{

}

u_int32 prh_test_rfcomm_iut_TP_RFC_BV_10_C(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
}

u_int32 prh_test_rfcomm_iut_TP_RFC_BV_12_C(t_bdaddr remoteAddress, u_int32 groupParam, u_int32 testParam)
{
}

int prh_rfcomm_port_regd;

int prh_tester_reg_port(void)
{
  
  u_int8 schan=2;
  int ret;
  
  if (prh_rfcomm_port_regd==0)
    {
      test_event=pEventCreate(FALSE);
      ret=PE_SrvRegisterPort("/dev/ptyr0", &schan, &tester_port_local_callback);
      prh_rfcomm_port_regd=1;
    }
  return 0;
}

int prh_tester_dereg_port(void)
{
  u_int8 schan=2;
  
  if (prh_rfcomm_port_regd==1)
    {
      PE_SrvDeregisterPort(schan);
      prh_rfcomm_port_regd=0;
    }
  return 0;
}

struct testEntry upper_rfcomm_conformance_tests[] = 
{
  { "TP/RFC/BV-01-C", prh_test_rfcomm_iut_TP_RFC_BV_01_C, 0, TEST_NOT_BATCHED|TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-02-C", prh_test_rfcomm_iut_no_mmi, 0, TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-03-C", prh_test_rfcomm_iut_no_mmi, 0, TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-04-C", prh_test_rfcomm_iut_not_impl, 0, TEST_NOT_BATCHED|TEST_RESET_SYSTEM, NULL }, 
  { "TP/RFC/BV-05-C", prh_test_rfcomm_iut_TP_RFC_BV_05_C, 0, TEST_NOT_BATCHED|TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-06-C", prh_test_rfcomm_iut_no_mmi, 0, TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-07-C", prh_test_rfcomm_iut_TP_RFC_BV_07_C, 0, TEST_NOT_BATCHED|TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-08-C", prh_test_rfcomm_iut_no_mmi, 0, TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-09-C", prh_test_rfcomm_iut_TP_RFC_BV_09_C, 0, TEST_NOT_BATCHED|TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-10-C", prh_test_rfcomm_iut_TP_RFC_BV_10_C, 0, TEST_NOT_BATCHED|TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-11-C", prh_test_rfcomm_iut_no_mmi, 0, TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-12-C", prh_test_rfcomm_iut_TP_RFC_BV_12_C, 0, TEST_NOT_BATCHED|TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-13-C", prh_test_rfcomm_iut_no_mmi, 0, TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-14-C", prh_test_rfcomm_iut_not_impl, 0, TEST_NOT_BATCHED|TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-15-C", prh_test_rfcomm_iut_no_mmi, 0, TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-17-C", prh_test_rfcomm_iut_no_mmi, 0, TEST_RESET_SYSTEM, NULL },
  { "TP/RFC/BV-19-C", prh_test_rfcomm_iut_no_mmi, 0, TEST_RESET_SYSTEM, NULL }
};

struct groupEntry upper_rfcomm_conformance_group = 
{
  "RFCOMM Upper",
  upper_rfcomm_conformance_tests,
  0, 
  GROUP_NOFLAGS, 
  ""
};
