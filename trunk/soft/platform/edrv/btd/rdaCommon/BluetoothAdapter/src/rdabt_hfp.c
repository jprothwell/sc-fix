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


#include "rdabt.h"
#include "rdabt_msg.h"
#include "bt.h"


#include "rdabt_config.h"
#include "rdabt_task.h"
#include "rdabt_profile.h"
#include "rdabt_link_prim.h"
#include "rdabt_hfp.h"
#include "mmi_trace.h"
#include "bt_hfg_struct.h"
#include "papi.h"

extern void rdabt_hfg_bldn_from_hf_ind_cb(void * pArgs);
extern void HangupAllCalls(void);

//kal_int8 Terminal_Call_Flag=0;//syzhou added

typedef enum{
    HFG_ACTIVATE_CNF=0,
    HFG_DEACTIVATE_CNF,
    HFG_ACL_CONNECT_CNF,
    HFG_ACL_DISCONNECT_CNF,
    HFG_SCO_CONNECT_CNF,
    HFG_SCO_DISCONNECT_CNF,
    HFG_REJECT_INCOMING_CALL_CNF,
    HFG_DISCONNECT_FROM_HF_IND,
    HFG_BLDN_FROM_HF_IND,
    HFG_RING_CNF,
    HFG_ACL_CONNECT_IND,
    HFG_ACL_DISCONNECT_IND,
    HFG_SPEAKER_GAIN_IND,
    HFG_MIC_GAIN_IND
}HFP_Q_T;

const struct {
    kal_int16 	type;
    void 		(*handler)(void  *);
} hfp_msg_hdlr_table[] = 
{
    {HFG_ACTIVATE_CNF,  rdabt_hfg_activate_cnf_cb},
    {HFG_DEACTIVATE_CNF,  rdabt_hfg_deactivate_cnf_cb},	
    {HFG_ACL_CONNECT_CNF,  rdabt_hfg_connect_cnf_cb},
    {HFG_ACL_DISCONNECT_CNF,  rdabt_hfg_disconnect_cnf_cb},	
    {HFG_SCO_CONNECT_CNF,  rdabt_hfg_set_sco_connect_cnf_cb},
    {HFG_SCO_DISCONNECT_CNF,  rdabt_hfg_set_sco_disconnect_cnf_cb},
    {HFG_REJECT_INCOMING_CALL_CNF,  rdabt_hfg_reject_incoming_call_cnf_cb},
    {HFG_DISCONNECT_FROM_HF_IND,  rdabt_hfg_disconnect_from_HF_ind_cb},
    {HFG_BLDN_FROM_HF_IND,  rdabt_hfg_bldn_from_hf_ind_cb},
    {HFG_RING_CNF,  rdabt_hfg_ring_cnf_cb},
    {HFG_ACL_CONNECT_IND, rdabt_hfg_connect_ind_cb},
    {HFG_ACL_DISCONNECT_IND, rdabt_hfg_disconnect_ind_cb},
    {HFG_SPEAKER_GAIN_IND, rdabt_hfg_speaker_gain_ind_cb},
    {HFG_MIC_GAIN_IND, rdabt_hfg_mic_gain_ind_cb}
};


extern void rdabt_read_log(void);
void rdabt_hfg_bldn_from_hf_ind_cb(void * pArgs)
{

     mmi_trace(1, "rdabt_hfg_bldn_from_hf_ind_cb");

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    rdabt_read_log();

    bt_hfg_redial_lastnum_req_struct *cnf_param_ptr;
    cnf_param_ptr = (bt_hfg_redial_lastnum_req_struct *)construct_local_para( (kal_uint16)sizeof(bt_hfg_redial_lastnum_req_struct), TD_CTRL);
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_HFG_REDIAL_LASTNUMB_REQ, (void *)cnf_param_ptr, NULL);	
   
}


/*
extern void mmi_bth_hfg_redial_lastnum_req_hdler(void *msg);
void rdabt_hfg_bldn_from_hf_ind_cb(void * pArgs)
{

 mmi_bth_hfg_redial_lastnum_req_hdler(pArgs);
  
}
*/


void rdabt_hfg_reject_incoming_call_cnf_cb(void * pArgs)
{
mmi_trace(1, "rdabt_hfg_reject_incoming_call_cnf_cb");

 //  kal_uint8 result =*(kal_uint8*)pArgs;
    bt_hfg_incoming_reject_req_struct *cnf_param_ptr;
    cnf_param_ptr = (bt_hfg_incoming_reject_req_struct *)construct_local_para( (kal_uint16)sizeof(bt_hfg_incoming_reject_req_struct), TD_CTRL);
//    cnf_param_ptr ->result = result;
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_HFG_INCOMING_REJECT_REQ, (void *)cnf_param_ptr, NULL);	

    rda_bt_hfg_send_reject_cfm();
	
}



void rdabt_hfg_ring_cnf_cb(void * pArgs)
{
  
  //  kal_uint8 result =*(kal_uint8*)pArgs;
    bt_hfg_incoming_accept_req_struct *cnf_param_ptr;
    cnf_param_ptr = (bt_hfg_incoming_accept_req_struct *)construct_local_para( (kal_uint16)sizeof(bt_hfg_incoming_accept_req_struct), TD_CTRL);
//    cnf_param_ptr ->result = result;
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_HFG_INCOMING_ACCEPT_REQ, (void *)cnf_param_ptr, NULL);	
	
}

void rdabt_hfp_int_msg_hdlr( kal_uint16 mi,void *mv )
{
    int I;
    int n = sizeof(hfp_msg_hdlr_table)/sizeof(hfp_msg_hdlr_table[0]);
    kal_uint16 type = (mi & 0x00ff);
    for(I=0; I<n; I++)
    {
        if( type==hfp_msg_hdlr_table[I].type )
        {
            hfp_msg_hdlr_table[I].handler( mv );
            break;
        }
    }
}

void rdabt_hfg_activate_cnf_cb(void * pArgs)
{
    kal_uint8 result =*(kal_uint8*)pArgs;
    bt_hfg_activate_cnf_struct *cnf_param_ptr;
    cnf_param_ptr = (bt_hfg_activate_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_hfg_activate_cnf_struct), TD_CTRL);
    cnf_param_ptr ->result = result;
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_HFG_ACTIVATE_CNF, (void *)cnf_param_ptr, NULL);	
}

void rdabt_hfg_deactivate_cnf_cb(void * pArgs)
{
    kal_uint8 result =*(kal_uint8*)pArgs;
    bt_hfg_deactivate_cnf_struct *cnf_param_ptr;
    cnf_param_ptr = (bt_hfg_deactivate_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_hfg_deactivate_cnf_struct), TD_CTRL);
    cnf_param_ptr ->result = result;
    
    kal_prompt_trace(1, "rdabt_hfg_deactivate_cnf_cb");
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_HFG_DEACTIVATE_CNF, (void *)cnf_param_ptr, NULL);	
}
extern scheduler_identifier pin_code_ind_timer;

void rdabt_hfg_connect_cnf_cb(void * pArgs)
{
    st_t_hfp_connect_cnf *msg_p =(st_t_hfp_connect_cnf *)pArgs;
    bt_hfg_connect_cnf_struct *cnf_param_ptr;
    cnf_param_ptr = (bt_hfg_connect_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_hfg_connect_cnf_struct), TD_CTRL);
    cnf_param_ptr ->result = msg_p->result;
    cnf_param_ptr ->connection_id = msg_p->connection_id;
    cnf_param_ptr ->bt_addr.lap = BDADDR_Get_LAP(&msg_p->bt_addr);
    cnf_param_ptr ->bt_addr.uap = BDADDR_Get_UAP(&msg_p->bt_addr);
    cnf_param_ptr ->bt_addr.nap = BDADDR_Get_NAP(&msg_p->bt_addr);
    mmi_trace(g_sw_BT, "msg_p->result= %d", msg_p->result);
    kal_prompt_trace(g_sw_BT, "rdabt_hfg_connect_cnf_cb");
    if(pin_code_ind_timer)//zhou siyou add 20080727
    {
    	 pTimerCancel(pin_code_ind_timer);
    	 pin_code_ind_timer=0;
    }
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_HFG_CONNECT_CNF, (void *)cnf_param_ptr, NULL);	
}

void rdabt_hfg_connect_ind_cb(void * pArgs)
{
    st_t_hfg_connect_ind *msg_p =(st_t_hfg_connect_ind *)pArgs;
    bt_hfg_connect_ind_req_struct *cnf_param_ptr;
    kal_prompt_trace(1, "rdabt_hfg_connect_ind_cb");
    
    cnf_param_ptr = (bt_hfg_connect_ind_req_struct *)construct_local_para( (kal_uint16)sizeof(bt_hfg_connect_ind_req_struct), TD_CTRL);
    
    cnf_param_ptr ->connection_id = msg_p->connection_id;
    cnf_param_ptr ->bt_addr.lap = BDADDR_Get_LAP(&msg_p->bt_addr);
    cnf_param_ptr ->bt_addr.uap = BDADDR_Get_UAP(&msg_p->bt_addr);
    cnf_param_ptr ->bt_addr.nap = BDADDR_Get_NAP(&msg_p->bt_addr);
    
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_HFG_CONNECT_IND, (void *)cnf_param_ptr, NULL);	
}

void rdabt_hfg_disconnect_cnf_cb(void * pArgs)
{
    st_t_hfp_connect_cnf *msg_p =(st_t_hfp_connect_cnf *)pArgs;
    bt_hfg_disconnect_cnf_struct *cnf_param_ptr;
    
    cnf_param_ptr = (bt_hfg_disconnect_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_hfg_disconnect_cnf_struct), TD_CTRL);
    cnf_param_ptr ->result = msg_p->result;
    cnf_param_ptr ->connection_id = msg_p->connection_id;
    cnf_param_ptr ->bt_addr.lap = BDADDR_Get_LAP(&msg_p->bt_addr);
    cnf_param_ptr ->bt_addr.uap = BDADDR_Get_UAP(&msg_p->bt_addr);
    cnf_param_ptr ->bt_addr.nap = BDADDR_Get_NAP(&msg_p->bt_addr);
    
     kal_prompt_trace(1, "rdabt_hfg_disconnect_cnf_cb msg_p->connection_id:%x",msg_p->connection_id);
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_HFG_DISCONNECT_CNF, (void *)cnf_param_ptr, NULL);	
}

void rdabt_hfg_disconnect_ind_cb(void * pArgs)
{
    st_t_hfg_connect_ind *msg_p =(st_t_hfg_connect_ind *)pArgs;
    bt_hfg_disconnect_ind_struct *cnf_param_ptr;

    kal_prompt_trace(1, "rdabt_hfg_disconnect_ind_cb");
    
    cnf_param_ptr = (bt_hfg_disconnect_ind_struct *)construct_local_para( (kal_uint16)sizeof(bt_hfg_disconnect_ind_struct), TD_CTRL);
    
    cnf_param_ptr ->connection_id = msg_p->connection_id;
    cnf_param_ptr ->bt_addr.lap = BDADDR_Get_LAP(&msg_p->bt_addr);
    cnf_param_ptr ->bt_addr.uap = BDADDR_Get_UAP(&msg_p->bt_addr);
    cnf_param_ptr ->bt_addr.nap = BDADDR_Get_NAP(&msg_p->bt_addr);
    
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_HFG_DISCONNECT_IND, (void *)cnf_param_ptr, NULL);	
}
void rdabt_hfg_set_sco_connect_cnf_cb(void * pArgs)
{
    st_t_hfg_sco_connect_cnf *msg_p =(st_t_hfg_sco_connect_cnf *)pArgs;
    bt_hfg_sco_connect_cnf_struct *cnf_param_ptr;
    cnf_param_ptr = (bt_hfg_sco_connect_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_hfg_sco_connect_cnf_struct), TD_CTRL);
    cnf_param_ptr ->result = msg_p->result;
    cnf_param_ptr->connection_id = msg_p->connection_id;
    
    rdabt_send_msg_up(MOD_MED, MSG_ID_BT_HFG_SCO_CONNECT_CNF, (void *)cnf_param_ptr, NULL);	
}

void rdabt_hfg_set_sco_disconnect_cnf_cb(void * pArgs)
{
    st_t_hfg_sco_connect_cnf *msg_p =(st_t_hfg_sco_connect_cnf *)pArgs;
    bt_hfg_sco_disconnect_cnf_struct *cnf_param_ptr;
    cnf_param_ptr = (bt_hfg_sco_disconnect_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_hfg_sco_disconnect_cnf_struct), TD_CTRL);
    cnf_param_ptr ->result = msg_p->result;
    cnf_param_ptr->connection_id = msg_p->connection_id;
    
    rdabt_send_msg_up(MOD_MED, MSG_ID_BT_HFG_SCO_DISCONNECT_CNF, (void *)cnf_param_ptr, NULL);	
//    Terminal_Call_Flag = 0;
}

void rdabt_hfg_set_HF_volume_cnf_cb(void * pArgs)
{
    st_t_hfp_set_volume_cnf *msg_p = (st_t_hfp_set_volume_cnf *)pArgs;
    bt_hfg_speaker_gain_cnf_struct  *cnf_param_ptr;
    cnf_param_ptr = (bt_hfg_speaker_gain_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_hfg_speaker_gain_cnf_struct), TD_CTRL);
    cnf_param_ptr ->result = msg_p->result;
    cnf_param_ptr->connection_id = msg_p->connection_id;

    rdabt_send_msg_up(MOD_MED, MSG_ID_BT_HFG_SPEAKER_GAIN_CNF, (void *)cnf_param_ptr, NULL);	
}

void rdabt_hfg_disconnect_from_HF_ind_cb(void * pArgs)
{
    kal_uint8 connection_id = *(kal_uint8*)pArgs;



  kal_prompt_trace(1, "rdabt_hfg_disconnect_from_HF_ind_cb connection_id=%d", connection_id);
    	
    bt_hfg_hangup_req_struct *cnf_param_ptr;
    cnf_param_ptr = (bt_hfg_hangup_req_struct *)construct_local_para( (kal_uint16)sizeof(bt_hfg_hangup_req_struct), TD_CTRL);
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_HFG_HANGUP_REQ, (void *)cnf_param_ptr, NULL);	
 
            //rda_bt_hfg_send_sco_disconnect_req(connection_id);

}

void rdabt_hfg_speaker_gain_ind_cb(void * pArgs)
{
    st_t_hfg_gain_ind_struct *msg_p =(st_t_hfg_gain_ind_struct *)pArgs;
    bt_hfg_speaker_gain_ind_struct *cnf_param_ptr;

    kal_prompt_trace(1, "rdabt_hfg_speaker_gain_ind_cb");
    
    cnf_param_ptr = (bt_hfg_speaker_gain_ind_struct *)construct_local_para( (kal_uint16)sizeof(bt_hfg_speaker_gain_ind_struct), TD_CTRL);
    
    cnf_param_ptr ->connection_id = msg_p->connection_id;
    cnf_param_ptr ->gain = msg_p->gain;
    
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_HFG_SPEAKER_GAIN_IND, (void *)cnf_param_ptr, NULL);	
}

void rdabt_hfg_mic_gain_ind_cb(void * pArgs)
{
    st_t_hfg_gain_ind_struct *msg_p =(st_t_hfg_gain_ind_struct *)pArgs;
    bt_hfg_mic_gain_ind_struct *cnf_param_ptr;

    kal_prompt_trace(1, "rdabt_hfg_mic_gain_ind_cb");
    
    cnf_param_ptr = (bt_hfg_mic_gain_ind_struct *)construct_local_para( (kal_uint16)sizeof(bt_hfg_mic_gain_ind_struct), TD_CTRL);
    
    cnf_param_ptr ->connection_id = msg_p->connection_id;
    cnf_param_ptr ->gain = msg_p->gain;
    
   rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_HFG_MIC_GAIN_IND, (void *)cnf_param_ptr, NULL);	
}

extern t_SDP_service_Parse_Results uuid_finded; 
void rdabt_hfg_msg_hdler(ilm_struct *message)
{

      switch(message->msg_id)
        {
        	case MSG_ID_BT_HFG_ACTIVATE_REQ:
		{
			rda_bt_hfg_send_activate_req();
			break;
        	}

       	case MSG_ID_BT_HFG_DEACTIVATE_REQ:
		{
			rda_bt_hfg_send_deactivate_req();
			break;
        	}
		
           //HFP message
            case MSG_ID_MEDIA_BT_AUDIO_OPEN_REQ:
            {
                bt_hfg_sco_connect_req_struct  *msg_req= (bt_hfg_sco_connect_req_struct*)message->local_para_ptr;
	//			Terminal_Call_Flag=0;//syzhou
                rda_bt_hfg_send_sco_connect_req(msg_req->connection_id);
                break;
            }

            case MSG_ID_MEDIA_BT_AUDIO_CLOSE_REQ:
            {
                bt_hfg_sco_disconnect_req_struct  *msg_req= (bt_hfg_sco_disconnect_req_struct*)message->local_para_ptr;
               rda_bt_hfg_send_sco_disconnect_req(msg_req->connection_id);
                break;                
            }

            case MSG_ID_BT_HFG_CONNECT_REQ:
            {
                t_bdaddr bd_addr;
                kal_uint8 chn_num=0;
//                kal_uint8 i=0;
                bt_hfg_connect_req_struct  *msg_req= (bt_hfg_connect_req_struct*)message->local_para_ptr;

               /* for(i=0;i<uuid_finded.total_searched;i++)
                {
                   if(uuid_finded.service[i].PDL_uuid==SDP_SCLASS_UUID_HANDSFREE)
                        chn_num = uuid_finded.service[i].chn_num;
                    
                    kal_prompt_trace(1, "mmi_bth_hfg_connect_req_hdler PDL_uuid=%x, chn_num=%d", uuid_finded.service[i].PDL_uuid,uuid_finded.service[i].chn_num);
                }*/
                
                BDADDR_Set_LAP_UAP_NAP(&bd_addr, msg_req->bt_addr.lap, msg_req->bt_addr.uap, msg_req->bt_addr.nap);
                rda_bt_hfg_send_connect_req(chn_num, bd_addr);
                break;
            }

            case MSG_ID_BT_HFG_DISCONNECT_REQ:
            {
                t_bdaddr bd_addr;
                bt_hfg_disconnect_req_struct  *msg_req= (bt_hfg_disconnect_req_struct*)message->local_para_ptr;
                BDADDR_Set_LAP_UAP_NAP(&bd_addr, msg_req->bt_addr.lap, msg_req->bt_addr.uap, msg_req->bt_addr.nap);
                rda_bt_hfg_send_disconnect_req(msg_req->connection_id, bd_addr);
                break;
            }
                        
            case MSG_ID_BT_HFG_SPEAKER_GAIN_REQ:
            {
                bt_hfg_speaker_gain_req_struct  *msg_req= (bt_hfg_speaker_gain_req_struct*)message->local_para_ptr;
                kal_prompt_trace(1,"rda_bt_hfg_send_set_volume_req is called!");
                rda_bt_hfg_send_set_volume_req(msg_req->connection_id,msg_req->gain);
                break;
            }

          case MSG_ID_BT_HFG_INCOMING_DISCONNECT_REQ:
            {
            
                kal_prompt_trace(1,"MSG_ID_BT_HFG_INCOMING_DISCONNECT_REQ is called!");
		  rda_bt_hfg_send_reject_cfm();//syzhou added 2009.6.28

                break;
            }

			
            default:
                ASSERT(0);
                break;				
            //end of hfp msg
        }
		
}
