#include "stdlib.h"
#include "kal_release.h"      	/* basic data type */
#include "stack_common.h"          /* message and module ids */
#include "stack_msgs.h"            /* enum for message ids */
#include "app_ltlcom.h"       	/* task message communiction */
//#include "syscomp_config.h"         /* type of system module components */
//#include "task_config.h"      	/* task creation */
#include "stacklib.h"        	   /* basic type for dll, evshed, stacktimer */
#include "event_shed.h"       	/* event scheduler */
#include "stack_timer.h"      	/* stack timer */
//#include "stdio.h"                  /* basic c i/o functions */
#include "gpio_sw.h"               /* for gpio */

#include "nvram_struct.h"           /* for nvram read/write */
#include "nvram_user_defs.h"
#include "bluetooth_bm_struct.h"

#include "rdabt.h"
#include "rdabt_msg.h"
#include "bt.h"


#include "rdabt_config.h"
#include "rdabt_task.h"
#include "rdabt_profile.h"
#include "rdabt_link_prim.h"



void rdabt_link_inquiry_cancel_cnf(ilm_struct *msg, void *mv)
{
        bt_bm_discovery_cancel_cnf_struct *param_ptr;
        LINK_CANCEL_DISCOVERY_ST *mv_struct = (LINK_CANCEL_DISCOVERY_ST *)mv;
        param_ptr = (bt_bm_discovery_cancel_cnf_struct *)construct_local_para(
        (kal_uint16)sizeof(bt_bm_discovery_cancel_cnf_struct), TD_CTRL);	
        param_ptr->result = mv_struct->result;
        param_ptr->total_number = mv_struct->total_number;
        msg->msg_id = MSG_ID_BT_BM_DISCOVERY_CANCEL_CNF;
        msg->local_para_ptr = (local_para_struct*)param_ptr;
        msg->peer_buff_ptr = NULL;
}



/*****************************************************************************
* FUNCTION
*  rdabt_link_discovery_result_ind
* DESCRIPTION
*   This function is to send msg to MMI task
*
* PARAMETERS
*  a  IN/OUT      first variable, used as returns
*  b  IN       second variable
* RETURNS
*  NONE.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void rdabt_link_discovery_result_ind(ilm_struct *msg, void *mv)
{
    bt_bm_discovery_result_ind_struct *cm_discovery_result_ind_p;
    LINK_DISCOVERY_RESULT_ST *param_temp = (LINK_DISCOVERY_RESULT_ST *)mv;
    btbm_bd_addr_t dev_addr = {0};
    u_int32 name_len = BTBM_ADP_MAX_NAME_LEN;
    cm_discovery_result_ind_p = (bt_bm_discovery_result_ind_struct *) construct_local_para(sizeof(bt_bm_discovery_result_ind_struct), TD_CTRL);

    dev_addr.lap = BDADDR_Get_LAP(&param_temp->device->address);
    dev_addr.uap = BDADDR_Get_UAP(&param_temp ->device->address);
    dev_addr.nap = BDADDR_Get_NAP(&param_temp ->device->address);
    cm_discovery_result_ind_p ->bd_addr = dev_addr;
    cm_discovery_result_ind_p ->cod = param_temp->device->classOfDevice;
    memset(cm_discovery_result_ind_p->name,0,name_len);
    if (name_len > strlen(param_temp ->device->deviceName))
		name_len = strlen(param_temp ->device->deviceName);
    memcpy(cm_discovery_result_ind_p ->name,param_temp->device->deviceName,name_len);
    cm_discovery_result_ind_p ->name_len = name_len;//strlen(param_temp ->device->deviceName);
    
    msg->msg_id = MSG_ID_BT_BM_DISCOVERY_RESULT_IND;
    msg->local_para_ptr = (local_para_struct*)cm_discovery_result_ind_p;
    msg->peer_buff_ptr = NULL;

}

void rdabt_link_discovery_cnf(ilm_struct *msg, void *mv)
{
    bt_bm_discovery_cnf_struct *param_ptr;
    LINK_DISCOVERY_CNF_ST *mv_struct = (LINK_DISCOVERY_CNF_ST *)mv;
    
    param_ptr = (bt_bm_discovery_cnf_struct *) construct_local_para(sizeof(bt_bm_discovery_cnf_struct), TD_CTRL);

    param_ptr->result = mv_struct->result;
    param_ptr->total_number = mv_struct->total_number;
    
    msg->msg_id = MSG_ID_BT_BM_DISCOVERY_CNF;
    msg->local_para_ptr = (local_para_struct*)param_ptr;
    msg->peer_buff_ptr = NULL;

}




void rdabt_link_write_scanenable_mode_cnf(ilm_struct *msg, void *mv)
{
        bt_bm_write_scanenable_mode_cnf_struct *param_ptr;
        LINK_WRITE_SCANENABLE_MODE_ST *mv_struct = (LINK_WRITE_SCANENABLE_MODE_ST *)mv;
        param_ptr = (bt_bm_write_scanenable_mode_cnf_struct *)construct_local_para(
        (kal_uint16)sizeof(bt_bm_write_scanenable_mode_cnf_struct), TD_CTRL);	
        param_ptr->result = mv_struct->result;
        msg->msg_id = MSG_ID_BT_BM_WRITE_SCANENABLE_MODE_CNF;
        msg->local_para_ptr = (local_para_struct*)param_ptr;
        msg->peer_buff_ptr = NULL;       
}

void rdabt_link_write_localaddr_cnf(ilm_struct *msg, void *mv)
{
    bt_bm_read_local_addr_cnf_struct *param_ptr;
    LINK_WRITE_LOCALADDR_ST *mv_struct = (LINK_WRITE_LOCALADDR_ST *)mv;
        param_ptr = (bt_bm_read_local_addr_cnf_struct *)construct_local_para(
        (kal_uint16)sizeof(bt_bm_read_local_addr_cnf_struct), TD_CTRL);	
        param_ptr->result = mv_struct->result;
        param_ptr ->bd_addr = mv_struct ->bd_addr;
        msg->msg_id = MSG_ID_BT_BM_READ_LOCAL_ADDR_CNF;
        msg->local_para_ptr = (local_para_struct*)param_ptr;
        msg->peer_buff_ptr = NULL;       
}

void rdabt_link_write_authentication_cnf(ilm_struct *msg, void *mv)
{
    bt_bm_write_authentication_mode_cnf_struct *param_ptr;
    LINK_WRITE_AUTHENTICATION_MODE_ST *mv_struct = (LINK_WRITE_AUTHENTICATION_MODE_ST *)mv;
        param_ptr = (bt_bm_write_authentication_mode_cnf_struct *)construct_local_para(
        (kal_uint16)sizeof(bt_bm_write_authentication_mode_cnf_struct), TD_CTRL);	
        param_ptr->result = mv_struct->result;
        msg->msg_id = MSG_ID_BT_BM_WRITE_AUTHENTICATION_MODE_CNF;
        msg->local_para_ptr = (local_para_struct*)param_ptr;
        msg->peer_buff_ptr = NULL;       
}


void rdabt_link_pin_code_ind(ilm_struct *msg, void *mv)
{
	bt_bm_pin_code_ind_struct *param_ptr;
	LINK_PIN_CODE_IND_ST   *mv_struct = (LINK_PIN_CODE_IND_ST *)mv;
	
       param_ptr = (bt_bm_pin_code_ind_struct*)construct_local_para((kal_uint16)sizeof(bt_bm_pin_code_ind_struct), TD_CTRL);
	param_ptr->bd_addr.lap= BDADDR_Get_LAP(&mv_struct->bd_addr);
	param_ptr->bd_addr.uap = BDADDR_Get_UAP(&mv_struct->bd_addr);
	param_ptr->bd_addr.nap = BDADDR_Get_NAP(&mv_struct->bd_addr);
	param_ptr->name_len = 0;
       msg->msg_id = MSG_ID_BT_BM_PIN_CODE_IND;
       msg->local_para_ptr = (local_para_struct*)param_ptr;
       msg->peer_buff_ptr = NULL;   		
			
}

void rdabt_link_bonding_result_ind(ilm_struct *msg, void *mv)
{
	bt_bm_bonding_result_ind_struct *param_ptr;
	LINK_PIN_CODE_RESULT_IND_ST   *mv_struct = (LINK_PIN_CODE_RESULT_IND_ST *)mv;
	
       param_ptr = (bt_bm_bonding_result_ind_struct*)construct_local_para((kal_uint16)sizeof(bt_bm_bonding_result_ind_struct), TD_CTRL);
	param_ptr->bd_addr= mv_struct ->bd_addr;
       param_ptr->result= mv_struct ->result;
	param_ptr->cod= mv_struct ->cod;

	memcpy(param_ptr->linkKey, mv_struct->linkKey, 16);
	param_ptr->KeyType = mv_struct->KeyType;

       msg->msg_id = MSG_ID_BT_BM_BONDING_RESULT_IND;
       msg->local_para_ptr = (local_para_struct*)param_ptr;
       msg->peer_buff_ptr = NULL;   		   
			
}

void rdabt_link_bonding_cnf(ilm_struct *msg, void *mv)
{
	bt_bm_bonding_cnf_struct *param_ptr;
	LINK_PIN_CODE_CNF_ST   *mv_struct = (LINK_PIN_CODE_CNF_ST *)mv;
	
       param_ptr = (bt_bm_bonding_cnf_struct*)construct_local_para((kal_uint16)sizeof(bt_bm_bonding_cnf_struct), TD_CTRL);

	param_ptr->result = mv_struct->result;
	param_ptr ->bd_addr = mv_struct->bd_addr;
       msg->msg_id = MSG_ID_BT_BM_BONDING_CNF;
       msg->local_para_ptr = (local_para_struct*)param_ptr;
       msg->peer_buff_ptr = NULL;   		
		
}


