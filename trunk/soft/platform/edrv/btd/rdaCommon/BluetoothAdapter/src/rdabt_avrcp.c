/***************************************************************************** 
* Include
*****************************************************************************/
/* basic system service headers */
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

#include "bt_a2dp_struct.h"
#include "rda_bt_hfg.h"
#include "rda_bt_avrcp.h"
#include "bt_dm_struct.h"
#include "bt_hfg_struct.h"
#include "bt_avrcp_struct.h"
//#include "rdabtmmia2dp.h"

#include "rdabt.h"
#include "rdabt_msg.h"
#include "bt.h"
//#include "manage.h"

#include "rdabt_config.h"
#include "rdabt_task.h"
#include "rdabt_profile.h"
#include "rdabt_link_prim.h"
#include "rdabt_avrcp.h"
#include "mmi_trace.h"

typedef enum{
    AVRCP_ACTIVATE_CNF = 0,
    AVRCP_DEACTIVATE_CNF,
    AVRCP_CONNECT_CNF,
    AVRCP_CONNECT_IND,
    AVRCP_DISCONNECT_CNF,
    AVRCP_DISCONNECT_IND,
    AVRCP_CMD_FRAME_CNF,
    AVRCP_CMD_FRAME_IND
}AVRCP_Q_T;

const struct {
    kal_int16 	type;
    void 		(*handler)(void  *);
} avrcp_msg_hdlr_table[] = 
{
    {AVRCP_ACTIVATE_CNF,  rdabt_avrcp_activate_cb},
    {AVRCP_DEACTIVATE_CNF,  rdabt_avrcp_deactivate_cb},
    {AVRCP_CONNECT_CNF,  rdabt_avrcp_connect_cnf_cb},
    {AVRCP_CONNECT_IND,  rdabt_avrcp_connect_ind_cb},
    {AVRCP_DISCONNECT_CNF,  rdabt_avrcp_disconnect_cnf_cb},
    {AVRCP_DISCONNECT_IND,  rdabt_avrcp_disconnect_ind_cb},
    {AVRCP_CMD_FRAME_CNF,  rdabt_avrcp_cmd_frame_cnf_cb},
    {AVRCP_CMD_FRAME_IND,  rdabt_avrcp_cmd_frame_ind_cb}
};

void rdabt_avrcp_int_msg_hdlr( kal_uint16 mi,void *mv )
{
    int I;
    int n = sizeof(avrcp_msg_hdlr_table)/sizeof(avrcp_msg_hdlr_table[0]);
    kal_uint16 type = (mi & 0x00ff);
    for(I=0; I<n; I++)
    {
        if( type==avrcp_msg_hdlr_table[I].type )
        {
            avrcp_msg_hdlr_table[I].handler( mv );
            break;
        }
    }

    //ASSERT( I<N ); // not supported by the avadp
}

void rdabt_avrcp_activate_cb(void * pArgs)
{
    kal_uint8 *msg_p = (kal_uint8 *)pArgs;
    bt_avrcp_activate_cnf_struct *cnf_param_ptr;
    cnf_param_ptr = (bt_avrcp_activate_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_avrcp_activate_cnf_struct), TD_CTRL);

    cnf_param_ptr ->result = msg_p[0];
    cnf_param_ptr->chnl_num = msg_p[1];

   if(cnf_param_ptr->result == BT_NOERROR)
        cnf_param_ptr->result = BT_AVRCP_RESULT_SUCCESS;
    else
        cnf_param_ptr->result = BT_AVRCP_RESULT_ERROR;
     
    mmi_trace(g_sw_BT, "rdabt_avrcp_activate_cb msg_p[0,1]=%d,%d",msg_p[0], msg_p[1]);
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_AVRCP_ACTIVATE_CNF, (void *)cnf_param_ptr, NULL);	
}

void rdabt_avrcp_deactivate_cb(void * pArgs)
{
    kal_uint8 *msg_p = (kal_uint8 *)pArgs;
    bt_avrcp_deactivate_cnf_struct *cnf_param_ptr;
    cnf_param_ptr = (bt_avrcp_deactivate_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_avrcp_deactivate_cnf_struct), TD_CTRL);
    cnf_param_ptr ->result = msg_p[0];
    cnf_param_ptr->chnl_num = msg_p[1];

   if(cnf_param_ptr->result == BT_NOERROR)
        cnf_param_ptr->result = BT_AVRCP_RESULT_SUCCESS;
    else
        cnf_param_ptr->result = BT_AVRCP_RESULT_ERROR;
		
    mmi_trace(g_sw_BT, "rdabt_avrcp_deactivate_cb msg_p[0,1]=%d,%d",msg_p[0], msg_p[1]);    
   rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_AVRCP_DEACTIVATE_CNF, (void *)cnf_param_ptr, NULL);	
}

void rdabt_avrcp_connect_ind_cb(void * pArgs)
{
    t_avrcp_connect_ind *msg_p = (t_avrcp_connect_ind*)pArgs;
    bt_avrcp_connect_ind_struct *cnf_param_ptr;
    
    cnf_param_ptr = (bt_avrcp_connect_ind_struct *)construct_local_para( (kal_uint16)sizeof(bt_avrcp_connect_ind_struct), TD_CTRL);
    cnf_param_ptr->chnl_num = msg_p->chnl_num;
    cnf_param_ptr->connect_id = msg_p->connect_id;
    cnf_param_ptr->device_addr.lap = BDADDR_Get_LAP(&msg_p->device_addr);
    cnf_param_ptr->device_addr.nap = BDADDR_Get_NAP(&msg_p->device_addr);
    cnf_param_ptr->device_addr.uap = BDADDR_Get_UAP(&msg_p->device_addr);

    mmi_trace(g_sw_BT, "rdabt_avrcp_connect_ind_cb lap=%x, nap=%x, uap=%x", cnf_param_ptr->device_addr.lap,
                                                                                                                            cnf_param_ptr->device_addr.nap,
                                                                                                                            cnf_param_ptr->device_addr.uap );
        
   rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_AVRCP_CONNECT_IND, (void *)cnf_param_ptr, NULL);	
   
}

void rdabt_avrcp_connect_cnf_cb(void * pArgs)
{
    t_avrcp_connect_cnf *msg_p = (t_avrcp_connect_cnf *)pArgs;
    bt_avrcp_connect_cnf_struct *cnf_param_ptr;
    cnf_param_ptr = (bt_avrcp_connect_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_avrcp_connect_cnf_struct), TD_CTRL);
    cnf_param_ptr->chnl_num = msg_p->chnl_num;
    cnf_param_ptr->connect_id = msg_p->connect_id;
    
    if(msg_p->result == BT_NOERROR)
        cnf_param_ptr->result = BT_AVRCP_RESULT_SUCCESS;
    else
        cnf_param_ptr->result = BT_AVRCP_RESULT_ERROR;

    cnf_param_ptr->device_addr.lap = BDADDR_Get_LAP(&msg_p->device_addr);
    cnf_param_ptr->device_addr.nap = BDADDR_Get_NAP(&msg_p->device_addr);
    cnf_param_ptr->device_addr.uap = BDADDR_Get_UAP(&msg_p->device_addr);
    
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_AVRCP_CONNECT_CNF, (void *)cnf_param_ptr, NULL);	

	
}

void rdabt_avrcp_disconnect_ind_cb(void * pArgs)
{
    t_avrcp_disconnect_ind *msg_p = (t_avrcp_disconnect_ind*)pArgs;
    bt_avrcp_disconnect_ind_struct *cnf_param_ptr;
    cnf_param_ptr = (bt_avrcp_disconnect_ind_struct *)construct_local_para( (kal_uint16)sizeof(bt_avrcp_disconnect_ind_struct), TD_CTRL);
    cnf_param_ptr->chnl_num = msg_p->chnl_num;
    cnf_param_ptr->connect_id = msg_p->connect_id;

    mmi_trace(g_sw_BT, "rdabt_avrcp_disconnect_ind_cb chnl_num:%d, connect_id:%x",msg_p->chnl_num, msg_p->connect_id);

   rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_AVRCP_DISCONNECT_IND, (void *)cnf_param_ptr, NULL);	
  
}

void rdabt_avrcp_disconnect_cnf_cb(void * pArgs)
{
    t_avrcp_disconnect_cnf *msg = (t_avrcp_disconnect_cnf *)pArgs;
    bt_avrcp_disconnect_cnf_struct *cnf_param_ptr;
    cnf_param_ptr = (bt_avrcp_disconnect_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_avrcp_disconnect_cnf_struct), TD_CTRL);

    cnf_param_ptr->chnl_num = msg->chnl_num;
    cnf_param_ptr->connect_id = msg->connect_id;
    if(msg->result == BT_NOERROR)
        cnf_param_ptr->result = BT_AVRCP_RESULT_SUCCESS;
    else
        cnf_param_ptr->result = BT_AVRCP_RESULT_ERROR;

   rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_AVRCP_DISCONNECT_CNF, (void *)cnf_param_ptr, NULL);	
   
}

void rdabt_avrcp_cmd_frame_cnf_cb(void * pArgs)
{
    bt_avrcp_cmd_frame_cnf_struct *cnf_param_ptr;
    cnf_param_ptr = (bt_avrcp_cmd_frame_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_avrcp_cmd_frame_cnf_struct), TD_CTRL);

    //TO DO LATER.....   
 rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_AVRCP_CMD_FRAME_CNF, (void *)cnf_param_ptr, NULL);	
	
}

void rdabt_avrcp_cmd_frame_ind_cb(void * pArgs)
{
    t_avrcp_cmd_frame_ind *msg_p = (t_avrcp_cmd_frame_ind*)pArgs;
    bt_avrcp_cmd_frame_ind_struct *cnf_param_ptr;
    cnf_param_ptr = (bt_avrcp_cmd_frame_ind_struct *)construct_local_para( (kal_uint16)sizeof(bt_avrcp_cmd_frame_ind_struct), TD_CTRL);

    mmi_trace(g_sw_BT, "rdabt_avrcp_cmd_frame_ind_cb data_len=%d, subunit_type=%x, subunit_id=%d, frame_data[0]=%x", msg_p->data_len, msg_p->subunit_type, msg_p->subunit_id, msg_p->frame_data[0]);

    cnf_param_ptr->chnl_num = msg_p->chnl_num;
    cnf_param_ptr->c_type = msg_p->c_type;
    cnf_param_ptr->data_len  = msg_p->data_len;
    cnf_param_ptr->seq_id  = msg_p->seq_id;
    cnf_param_ptr->subunit_type  = msg_p->subunit_type;
    cnf_param_ptr->subunit_id  = msg_p->subunit_id;
    cnf_param_ptr->profile_id  = msg_p->profile_id;
    memcpy(cnf_param_ptr->frame_data, msg_p->frame_data, msg_p->data_len);

    //mmi_trace(0, "rdabt_avrcp_cmd_frame_ind_cb chnl_num=%d,chnl_num=%d, profile_id=%x",msg_p->chnl_num, cnf_param_ptr->chnl_num, cnf_param_ptr->profile_id);
    
  rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_AVRCP_CMD_FRAME_IND, (void *)cnf_param_ptr, NULL);	
	
}

void rdabt_avrcp_msg_hdlr(ilm_struct *message)
{

        switch(message->msg_id)
        {
        	case MSG_ID_BT_AVRCP_ACTIVATE_REQ:
		{
			bt_avrcp_activate_req_struct *msg_p = (bt_avrcp_activate_req_struct*)message->local_para_ptr;
			rda_bt_avrcp_send_activate_req(msg_p->local_role,msg_p->local_role);
			break;
		}

        	case MSG_ID_BT_AVRCP_DEACTIVATE_REQ:
		{
			bt_avrcp_deactivate_cnf_struct* msg_p = (bt_avrcp_deactivate_cnf_struct*)message->local_para_ptr;
			 rda_bt_avrcp_send_deactivate_req(msg_p->chnl_num);
			break;
		}
			
            //AVRCP msg
            case MSG_ID_BT_AVRCP_CONNECT_REQ:
            {
                t_bdaddr bt_addr;
                bt_avrcp_connect_req_struct * msg_p = (bt_avrcp_connect_req_struct*)message->local_para_ptr;
                BDADDR_Set_LAP_UAP_NAP(&bt_addr, msg_p->device_addr.lap, msg_p->device_addr.uap, msg_p->device_addr.nap); 
                rda_bt_avrcp_send_connect_req(msg_p->chnl_num, bt_addr, msg_p->local_role);
                break;
            }
            case MSG_ID_BT_AVRCP_DISCONNECT_REQ:
            {
                bt_avrcp_disconnect_req_struct * msg_p = (bt_avrcp_disconnect_req_struct*)message->local_para_ptr;
                rda_bt_avrcp_send_disconnect_req( msg_p->chnl_num);
                break;                
            }
            default:
                ASSERT(0);
                break;			
            //end of AVRCP msg
        }

	
}
