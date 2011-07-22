#include "host_types.h"
#include "host_buf.h"
#include "mgr_core.h"
#include "rfcomm_ue_api.h"

#include <stdio.h>
int rftest_enabled=0;
extern int test_num;
int prh_test_rfcomm_iut_no_mmi(void)
{
  /* no need to do anything other side drives */
  rftest_enabled=test_num;
}

int prh_test_rfcomm_iut_not_impl(void)
{
  /* Not applicable */
  fprintf(stderr, "This test is not applicable to this instance of RFCOMM\n");
  pExit(1);
}

int prh_test_rfcomm_iut_TP_RFC_BV_02_C(void)
{
  rftest_enabled=0;
  rftest_l2cap_connect();
  /* Need to establish L2CAP */
  rftest_enabled=test_num;
  rftest_sendsabm(0, 1, 1);

}

int prh_test_rfcomm_iut_TP_RFC_BV_03_C(void)
{
  rftest_enabled=0;
  rftest_l2cap_connect();
  rftest_sendsabm(0, 1, 1);

  pDebugSleep(2);
  rftest_enabled=test_num;
  rftest_senddisc(0, 1, 1);
}

int prh_test_rfcomm_iut_TP_RFC_BV_06_C(void)
{
  rftest_enabled=0;
  rftest_l2cap_connect();
  rftest_sendsabm(0, 1, 1);
  pDebugSleep(2);
  rftest_enabled=test_num;
  rftest_sendsabm(4, 1, 1);
}

int prh_test_rfcomm_iut_TP_RFC_BV_08_C(void)
{
  rftest_enabled=0;
  rftest_l2cap_connect();
  rftest_sendsabm(0, 1, 1);
  rftest_sendsabm(4, 1, 1);
  pDebugSleep(2);
  rftest_enabled=test_num;
  rftest_senddisc(4, 1, 1);
}

int prh_test_rfcomm_iut_TP_RFC_BV_09_C(void)
{
  prh_t_rfcomm_control_params ctrl_params;

  rftest_enabled=0;
  rftest_l2cap_connect();
  rftest_sendsabm(0, 1, 1);
  rftest_sendsabm(4, 1, 1);
  pDebugSleep(2);

  rftest_enabled=test_num;
  ctrl_params.break_signal=0;
  ctrl_params.modem_signal=PRH_RFCOMM_MSC_DTRDSR | PRH_RFCOMM_MSC_RTSCTS | PRH_RFCOMM_MSC_DCD;
  ctrl_params.flow_state=PRH_RFCOMM_FLOW_DATA_GO; 
  rftest_sendmsc(4, 1, &ctrl_params);

  pDebugSleep(5);
  
  ctrl_params.break_signal=0;
  ctrl_params.modem_signal=PRH_RFCOMM_MSC_DCD;
  ctrl_params.flow_state=PRH_RFCOMM_FLOW_DATA_STOP;
  rftest_sendmsc(4, 1, &ctrl_params);
}

int prh_test_rfcomm_iut_TP_RFC_BV_10_C(void)
{
  prh_t_rfcomm_control_params ctrl_params;
  
  rftest_l2cap_connect();
  rftest_sendsabm(0, 1, 1);
  rftest_sendsabm(4, 1, 1);
   
  ctrl_params.break_signal=0;
  ctrl_params.modem_signal=PRH_RFCOMM_MSC_DTRDSR | PRH_RFCOMM_MSC_RTSCTS | PRH_RFCOMM_MSC_DCD;
  ctrl_params.flow_state=PRH_RFCOMM_FLOW_DATA_GO;
  rftest_sendmsc(4, 1, &ctrl_params);
  
  /* Do data transfer */
}

int prh_test_rfcomm_iut_TP_RFC_BV_11_C(void)
{
  rftest_l2cap_connect();
  rftest_sendsabm(0, 1, 1);
  pDebugSleep(2);
  rftest_enabled=test_num;
  rftest_sendtest(1);
}

int prh_test_rfcomm_iut_TP_RFC_BV_12_C(void)
{
 prh_t_rfcomm_control_params ctrl_params;


 rftest_l2cap_connect();
 rftest_sendsabm(0, 1, 1);
 rftest_sendsabm(4, 1, 1);
 
 ctrl_params.break_signal=0;
 ctrl_params.modem_signal=PRH_RFCOMM_MSC_DTRDSR | PRH_RFCOMM_MSC_RTSCTS | PRH_RFCOMM_MSC_DCD;
 ctrl_params.flow_state=PRH_RFCOMM_FLOW_DATA_GO; 
 rftest_sendmsc(4, 1, &ctrl_params);

 pDebugSleep(2);
 rftest_send_fcoff(1);
 pDebugSleep(5);
 rftest_send_fcon(1);
}

int prh_test_rfcomm_iut_TP_RFC_BV_13_C(void)
{
  rftest_l2cap_connect();
  rftest_sendsabm(0, 1, 1);
  rftest_sendsabm(4, 1, 1);

  pDebugSleep(2);
  rftest_enabled=test_num;
  rftest_sendrls(4, 1, PRH_RFCOMM_LSTAT_PARITY_ERROR|PRH_RFCOMM_LSTAT_ERROR);
}

int prh_test_rfcomm_iut_TP_RFC_BV_15_C(void)
{
  rftest_l2cap_connect();
  rftest_sendsabm(0, 1, 1); 
  pDebugSleep(2);
  rftest_sendpn(4, 1, 500);
  pDebugSleep(2);
  rftest_sendpn(4, 1, 64);
}

int prh_test_rfcomm_iut_TP_RFC_BV_17_C(void)
{
  prh_t_rfcomm_port_params port_params;
  rftest_enabled=0;
  port_params.port_speed=RF9600;
  port_params.data_bits=RFDS8;
  port_params.stop_bits=RFSTOP1;
  port_params.parity_enable=RFNONE;
  port_params.paritytype=RFODD;
  rftest_l2cap_connect();
  rftest_sendsabm(0, 1, 1); 
  rftest_sendsabm(4, 1, 1);
  pDebugSleep(2);
  rftest_enabled=test_num;
  rftest_sendrpnc(4, 1, &port_params);
}

int prh_test_rfcomm_iut_TP_RFC_BV_19_C(void)
{
  rftest_enabled=0;
  rftest_l2cap_connect();
  rftest_sendsabm(0, 1, 1);
  rftest_sendsabm(4, 1, 1); 
  pDebugSleep(2);
  rftest_enabled=19;
  rftest_sendrpnr(4, 1);
}

int (*rfcomm_conform_test[])(void) = 
{
  prh_test_rfcomm_iut_no_mmi,         /* TP/RFC/BV-01-C */ 
  prh_test_rfcomm_iut_TP_RFC_BV_02_C, /* TP/RFC/BV-02-C */
  prh_test_rfcomm_iut_TP_RFC_BV_03_C, /* TP/RFC/BV-03-C */
  prh_test_rfcomm_iut_not_impl,       /* TP/RFC/BV-04-C */ 
  prh_test_rfcomm_iut_no_mmi,         /* TP/RFC/BV-05-C */
  prh_test_rfcomm_iut_TP_RFC_BV_06_C, /* TP/RFC/BV-06-C */
  prh_test_rfcomm_iut_no_mmi,         /* TP/RFC/BV-07-C */
  prh_test_rfcomm_iut_TP_RFC_BV_08_C, /* TP/RFC/BV-08-C */
  prh_test_rfcomm_iut_TP_RFC_BV_09_C, /* TP/RFC/BV-09-C */
  prh_test_rfcomm_iut_TP_RFC_BV_10_C, /* TP/RFC/BV-10-C */
  prh_test_rfcomm_iut_TP_RFC_BV_11_C, /* TP/RFC/BV-11-C */
  prh_test_rfcomm_iut_TP_RFC_BV_12_C, /* TP/RFC/BV-12-C */
  prh_test_rfcomm_iut_TP_RFC_BV_13_C, /* TP/RFC/BV-13-C */
  prh_test_rfcomm_iut_not_impl,       /* TP/RFC/BV-14-C */
  prh_test_rfcomm_iut_TP_RFC_BV_15_C, /* TP/RFC/BV-15-C */
  prh_test_rfcomm_iut_not_impl,       /* TP/RFC/BV-16-C */
  prh_test_rfcomm_iut_TP_RFC_BV_17_C, /* TP/RFC/BV-17-C */
  prh_test_rfcomm_iut_not_impl,       /* TP/RFC/BV-18-C */
  prh_test_rfcomm_iut_TP_RFC_BV_19_C, /* TP/RFC/BV-19-C */

};
