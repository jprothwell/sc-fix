/*
 * MODULE NAME:    host_callback.c
 * DESCRIPTION:    Bluestream control functions implementation
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: host_callback--mql.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 */
#include "host_config.h"

#include "host_types.h"
#include "host_config.h"
#include "papi.h"
#include "papi_impl.h"
#include "mgr_inqdb.h"
#include "host_msg.h"
#include "host_link_prim.h"



extern u_int32 rdabt_put_message(u_int16 q, u_int16 mi, void *mv);
extern APIDECL1 t_api APIDECL2 MGR_PINResponse(t_bdaddr address, u_int8 pinSupplied, u_int8 *pinCode, u_int8 pinLen);

bt_host_cb_tbl bt_host_cb_cntx;

t_pTimer pin_code_ind_timer;

	
void rdabt_write_scanenable_cb(int status)
{
    LINK_WRITE_SCANENABLE_MODE_ST mv;
    mv.type = LINK_WRITE_SCANENABLE_MODE_CNF;
    mv.result = status;
    rdabt_put_message(EXTERNAL_TASK_MASK,LINK_Q,&mv);
}


void rdabt_write_localaddr_cb(int status)
{
    LINK_WRITE_LOCALADDR_ST mv;
    mv.type = LINK_WRITE_LOCALADDR_CNF;
    mv.result = status;
    rdabt_put_message(EXTERNAL_TASK_MASK,LINK_Q,&mv);
}

void bt_write_authentication_mode_cb(int status)
{
    LINK_WRITE_LOCALADDR_ST mv;
    mv.type = LINK_WRITE_AUTHENTICATION_MODE_CNF;
    mv.result = status;
    rdabt_put_message(EXTERNAL_TASK_MASK,LINK_Q,&mv);
}

void bt_discovery_cnf_cb(u_int8 status ,u_int8 total_num)
{
    LINK_DISCOVERY_CNF_ST mv;
    mv.type = LINK_DISCOVERY_CNF;
    mv.result = status;
    mv.total_number = total_num;
    rdabt_put_message(EXTERNAL_TASK_MASK,LINK_Q,&mv);
} 

void bt_discovery_result_ind_cb(struct st_t_BT_DeviceEntry *newDevice, int status)
{
    LINK_DISCOVERY_RESULT_ST mv;
    mv.type = LINK_DISCOVERY_RESULT_IND;
    mv.result = status;
    //pMemcpy(mv.device.address.bytes, (u_int8 *)&newDevice, sizeof(t_MGR_DeviceEntry));
    pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"newDevice->deviceName = %s",newDevice->deviceName));

    mv.device = (t_MGR_DeviceEntry *)newDevice;
    pDebugPrintfEX((pLOGDEBUG,pLOGMANAGER,"mv.device->deviceName = %s",mv.device->deviceName));

    rdabt_put_message(EXTERNAL_TASK_MASK,LINK_Q,&mv);
} 

void bt_discovery_cancel_cb(u_int8 num,u_int8 result)
{
    LINK_DISCOVERY_CANCEL_ST mv;
    mv.type = LINK_DISCOVERY_CANCEL_CNF;
    mv.result = result;
    mv.number = num;
    rdabt_put_message(EXTERNAL_TASK_MASK,LINK_Q,&mv);
}



void rdabt_InputPin_MoniterTimer_cb(void  *addr )
{
	t_bdaddr tempaddr;
	pMemcpy(tempaddr.bytes,(u_int8 *)addr,sizeof(t_bdaddr));
	
	MGR_PINResponse(tempaddr, 0, NULL, 0);		
}


void bt_pin_code_ind_cb(t_bdaddr address )
{
    LINK_PIN_CODE_IND_ST mv;
    t_bdaddr  *addr = (t_bdaddr *)&address;
    mv.type = LINK_PIN_CODE_IND;
    pMemcpy(mv.bd_addr.bytes,address.bytes,sizeof(t_bdaddr));
    mv.name_len = 0;
    rdabt_put_message(EXTERNAL_TASK_MASK,LINK_Q,&mv);
    pin_code_ind_timer = pTimerCreate(60,rdabt_InputPin_MoniterTimer_cb,(void *)addr,pTIMER_ONESHOT);
}


void bt_host_reg_cb_func(HOST_LINK_ENUM type,  void *fp)
{

    switch(type)
    {
    case LINK_WRITE_SCANENABLE_MODE_CNF:
        bt_host_cb_cntx.bt_write_scanenable_mode_cb = (void (*)(int))fp;
        break;
    case LINK_WRITE_LOCALADDR_CNF:
	bt_host_cb_cntx.bt_write_local_addr_cb = (void(*)(int))fp;
    case LINK_WRITE_AUTHENTICATION_MODE_CNF:
	bt_host_cb_cntx.bt_write_authentication_mode_cb= (void(*)(int))fp;		
    case LINK_DISCOVERY_CNF:
	bt_host_cb_cntx.bt_discovery_cnf_cb= (void(*)(u_int8,u_int8))fp;		
    case LINK_DISCOVERY_RESULT_IND:
	bt_host_cb_cntx.bt_discovery_result_cb= (void(*)(struct st_t_BT_DeviceEntry *, int))fp;		
    case LINK_DISCOVERY_CANCEL_CNF:
	bt_host_cb_cntx.bt_discovery_cancel_cb= (void(*)(u_int8,u_int8))fp;		
    case LINK_PIN_CODE_IND:
	bt_host_cb_cntx.bt_pin_code_ind_cb= (void(*)(t_bdaddr))fp;			
    default:
        break;
    }  

}



void bt_host_cb_init(void)
{
    bt_host_reg_cb_func(LINK_WRITE_SCANENABLE_MODE_CNF,(void *)rdabt_write_scanenable_cb);
    bt_host_reg_cb_func(LINK_WRITE_LOCALADDR_CNF,(void *)rdabt_write_localaddr_cb);
    bt_host_reg_cb_func(LINK_WRITE_AUTHENTICATION_MODE_CNF,(void *)bt_write_authentication_mode_cb);
    bt_host_reg_cb_func(LINK_DISCOVERY_CNF,(void *)bt_discovery_cnf_cb);	
    bt_host_reg_cb_func(LINK_DISCOVERY_RESULT_IND,(void *)bt_discovery_result_ind_cb);		
    bt_host_reg_cb_func(LINK_DISCOVERY_CANCEL_CNF,(void *)bt_discovery_cancel_cb);
    bt_host_reg_cb_func(LINK_PIN_CODE_IND,(void *)bt_pin_code_ind_cb);			
	
	
}





