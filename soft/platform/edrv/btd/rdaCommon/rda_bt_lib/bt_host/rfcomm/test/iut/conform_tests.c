#include "host_types.h"
#include "host_buf.h"
#include "mgr_core.h"
#include "rfcomm_ue_api.h"

#include <stdio.h>
extern t_bdaddr bd_addr_inq;
void *conform_multi_inst;
struct prh_rfcomm_pe_port *test_pe_info;

int prh_test_rfcomm_iut_no_mmi(void)
{
}

int prh_test_rfcomm_iut_not_impl(void)
{
  fprintf(stderr, "This test is not applicable to this instance of RFCOMM\n");
  pExit(1);
}

int prh_test_rfcomm_iut_TP_RFC_BV_01_C(void)
{
  
  prh_test_rfcomm_open_multiplexer(bd_addr_inq, &conform_multi_inst);

}

int prh_test_rfcomm_iut_TP_RFC_BV_04_C(void)
{
  PRH_RFCOMM_UE_Close_Req(conform_multi_inst);
}

int prh_test_rfcomm_iut_TP_RFC_BV_05_C(void)
{
  pSem_t sema;
  prh_t_rfcomm_schan test_schan=2;
  prh_t_rfcomm_dlci dlci;
  struct prh_rfcomm_pe_port *pe_info;

  prh_test_rfcomm_pe_connect_port(bd_addr_inq, test_schan, "COMTEST", &dlci, &pe_info, 127);
  test_pe_info=pe_info;
  conform_multi_inst=test_pe_info->multi_inst;
}

int prh_test_rfcomm_iut_TP_RFC_BV_07_C(void)
{
   pSem_t sema;
  prh_t_rfcomm_schan test_schan=2;
  prh_t_rfcomm_dlci dlci;
  struct prh_rfcomm_pe_port *pe_info;

  prh_test_rfcomm_pe_connect_port(bd_addr_inq, test_schan, "COMTEST", &dlci, &pe_info, 127);
  test_pe_info=pe_info;
  conform_multi_inst=test_pe_info->multi_inst;

  pDebugSleep(2);
  PRH_RFCOMM_UE_DLC_Rel_Req(test_pe_info->dlci, test_pe_info->multi_inst);
}

int prh_test_rfcomm_iut_TP_RFC_BV_09_C(void)
{

}

int prh_test_rfcomm_iut_TP_RFC_BV_10_C(void)
{
}

int prh_test_rfcomm_iut_TP_RFC_BV_12_C(void)
{
}

int (*rfcomm_conform_test[])(void) = 
{
  prh_test_rfcomm_iut_TP_RFC_BV_01_C, /* TP/RFC/BV-01-C */
  prh_test_rfcomm_iut_no_mmi,         /* TP/RFC/BV-02-C */ 
  prh_test_rfcomm_iut_no_mmi,         /* TP/RFC/BV-03-C */
  prh_test_rfcomm_iut_not_impl,       /* TP/RFC/BV-04-C */
  prh_test_rfcomm_iut_TP_RFC_BV_05_C, /* TP/RFC/BV-05-C */
  prh_test_rfcomm_iut_no_mmi,         /* TP/RFC/BV-06-C */
  prh_test_rfcomm_iut_TP_RFC_BV_07_C, /* TP/RFC/BV-07-C */
  prh_test_rfcomm_iut_no_mmi,         /* TP/RFC/BV-08-C */
  prh_test_rfcomm_iut_TP_RFC_BV_09_C, /* TP/RFC/BV-09-C */
  prh_test_rfcomm_iut_TP_RFC_BV_10_C, /* TP/RFC/BV-10-C */
  prh_test_rfcomm_iut_no_mmi,         /* TP/RFC/BV-11-C */
  prh_test_rfcomm_iut_TP_RFC_BV_12_C, /* TP/RFC/BV-12-C */
  prh_test_rfcomm_iut_no_mmi,         /* TP/RFC/BV-13-C */
  prh_test_rfcomm_iut_not_impl,       /* TP/RFC/BV-14-C */
  prh_test_rfcomm_iut_no_mmi,         /* TP/RFC/BV-15-C */
  prh_test_rfcomm_iut_not_impl,       /* TP/RFC/BV-16-C */
  prh_test_rfcomm_iut_no_mmi,         /* TP/RFC/BV-17-C */
  prh_test_rfcomm_iut_not_impl,       /* TP/RFC/BV-18-C */
  prh_test_rfcomm_iut_no_mmi,         /* TP/RFC/BV-19-C */
};
