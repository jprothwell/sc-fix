/***********************************************************************
 *
 * MODULE NAME:    rftest_l2cap.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Handle L2CAP for tester
 * MAINTAINER:     Dave Airlie
 * CREATION DATE: 
 *
 * SOURCE CONTROL: $Id: rftest_l2cap.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    02.Feb.2000 -   DA - first import
 *
 *
 * ISSUES:
 *
 ***********************************************************************/
#include "host_config.h"

#include "host_types.h"
#include "host_error.h"
#include "papi.h"
#include "host_buf.h"

#include "l2cap_primitives.h"
#include "rfcomm_ue_api.h"
#include "rfcomm_core.h"
#include "rfcomm_frame.h"
#include "rfcomm_proto.h"

#define RFTEST_L2_INIT_NONE 0
#define RFTEST_L2_INIT_ME 1
#define RFTEST_L2_INIT_THEM 2

struct st_l2cap_inst
{
  t_bdaddr bd_addr;
  prh_t_rfcomm_cid cid;  
  int initiator;
} test_l2cap_inst;

extern t_bdaddr bd_addr_inq;
struct st_l2cap_inst *test_l2_inst=&test_l2cap_inst;

int rftest_get_crbit(void)
{
  if (test_l2cap_inst.initiator==RFTEST_L2_INIT_ME)
    return 1;

  if (test_l2cap_inst.initiator==RFTEST_L2_INIT_THEM)
    return 0;
  return -1;
}

void rftest_l2cap_data_ind_cb(u_int16 cid, struct host_buf *p_buf);
void rftest_l2cap_connect_ind(u_int8 eventType, t_L2_ControlCallbackArgs *args)
{
  test_l2_inst->bd_addr=args->BDAddress;
  test_l2_inst->cid=args->cid;
  test_l2_inst->initiator=RFTEST_L2_INIT_THEM;
  L2_ConnectResponse(test_l2_inst->bd_addr, test_l2_inst->cid, 0, 0);
      
  //      l2cap_inst->w4_configure=CONN_NONINIT;
      
  L2_Configure(test_l2_inst->cid,PRH_RFCOMM_DEFAULT_MTU,0x0,0x0,0x00);

}

int rftest_to_configure;
int rftest_for_configure;
int l2cap_cid;


int tester_setting_up_l2cap, tester_removing_l2cap;
t_pEvent test_l2cap_event;

void rftest_l2cap_config_ind_cb(u_int8 eventType, t_L2_ControlCallbackArgs *args)
{
  u_int16 response=0;
  L2_ConfigureResponse(test_l2_inst->cid,PRH_RFCOMM_DEFAULT_MTU,response,0x00);
  
  if (rftest_for_configure)
    {
      rftest_for_configure=0x0;
      if (!rftest_to_configure)
	{
	  if (tester_setting_up_l2cap)
	    pEventSet(test_l2cap_event);
	  /* callback */
	}
    }

}

void rftest_l2cap_config_cfm_cb(u_int8 eventType, t_L2_ControlCallbackArgs *args)
{ 
  pDebugPrintf((pLOGDEBUG,"     the link is configured now. "));
  
  if (rftest_to_configure)
    {
      rftest_to_configure=0x0;
      if (!rftest_for_configure)
	{
	  if (tester_setting_up_l2cap)
	    pEventSet(test_l2cap_event);
	  /* start test */
	}
    }

}

void rftest_l2cap_disc_ind_cb(u_int8 eventType, t_L2_ControlCallbackArgs *args)
{
  pMemset(test_l2_inst, 0, sizeof(struct st_l2cap_inst));
}

void rftest_l2cap_disc_cfm_cb(u_int8 eventType, t_L2_ControlCallbackArgs *args)
{
  pMemset(test_l2_inst, 0, sizeof(struct st_l2cap_inst));
  if (tester_removing_l2cap)
    pEventSet(test_l2cap_event);
}

void rftest_l2cap_connect_cfm_cb(u_int8 eventType, t_L2_ControlCallbackArgs *args)
{
  
  pDebugPrintf((pLOGDEBUG," Connection is called back : "));
  pDebugPrintf((pLOGDEBUG,"     Starting to configure the link "));
  test_l2_inst->cid=args->cid;
  L2_Configure(args->cid,PRH_RFCOMM_DEFAULT_MTU,0x0,0x0,0x00);
}

void rftest_l2cap_connect(t_bdaddr remoteaddress)
{

  test_l2cap_event=pEventCreate(FALSE);

  tester_setting_up_l2cap=1;  
  rftest_to_configure=1;
  rftest_for_configure=1;
  L2_Connect(remoteaddress, PRH_RFCOMM_PSM);
  test_l2_inst->initiator=RFTEST_L2_INIT_ME;
  pEventWait(test_l2cap_event);

  pEventFree(test_l2cap_event);
  tester_setting_up_l2cap=0;
}

int rftest_init(void)
{
  
  struct l2cap_cbs callbacks;

  /* Register call back handler with L2CAP for all CBs on this PSM */
  
  callbacks.connectionIndication=rftest_l2cap_connect_ind;
  callbacks.configureIndication=rftest_l2cap_config_ind_cb;
  callbacks.configureConfirmation=rftest_l2cap_config_cfm_cb;
  callbacks.qosviolationIndication=NULL;
  callbacks.dataIndication=rftest_l2cap_data_ind_cb;
  callbacks.connectionConfirmation=rftest_l2cap_connect_cfm_cb;
  callbacks.disconnectionIndication=rftest_l2cap_disc_ind_cb;
  callbacks.disconnectionConfirmation=rftest_l2cap_disc_cfm_cb;
  
  L2_RegisterPSM(PRH_RFCOMM_PSM, &callbacks);
  test_l2_inst->initiator=RFTEST_L2_INIT_NONE;
  rftest_init_events();

  return 0;
}

int rftest_finish(void)
{
  rftest_free_events();
  if (test_l2_inst->cid)
    {
      test_l2cap_event=pEventCreate(FALSE);
      tester_removing_l2cap=1;  
      
      L2_Disconnect(test_l2_inst->cid);
      
      pEventWait(test_l2cap_event);
      
      pEventFree(test_l2cap_event);
    }
  tester_removing_l2cap=0;
  L2_DeregisterPSM(PRH_RFCOMM_PSM);

}
