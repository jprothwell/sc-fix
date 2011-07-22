/***************************************************************************** 
* Include
*****************************************************************************/
/* basic system service headers */
#include "mmi_features.h"

#ifdef  __BT_A2DP_PROFILE__

#include "stdlib.h"
#include "kal_release.h"      	/* basic data type */
//#include "stack_common.h"          /* message and module ids */
//#include "stack_msgs.h"            /* enum for message ids */
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
#include "rda_bt_a2dp.h"
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
#include "rdabt_a2dp.h"

#include "mmi_trace.h"

kal_uint8 stream_handle;

typedef enum{
    A2DP_SIGNAL_CONNECT_CNF = 0,
    A2DP_SIGNAL_DISCONN_CNF,
    A2DP_SEND_SEP_DISCOVER_CNF,
    A2DP_SEND_SEP_GET_CAPABILITIES_CNF,
    A2DP_SEND_SEP_SET_CONFIG_CNF,
    A2DP_SEND_SEP_OPEN_CNF,
    A2DP_SEND_SEP_START_CNF,
    A2DP_SEND_SEP_PAUSE_CNF,
    A2DP_SEND_SEP_CLOSE_CNF,
    A2DP_SEND_SEP_ABORT_CNF,
    A2DP_ACTIVATE_CNF,
    A2DP_DEACTIVATE_CNF,
    A2DP_SIGNAL_CONNECT_IND,
    A2DP_SIGNAL_DISCONNECT_IND,
    A2DP_SEND_SEP_DISCOVER_IND,
    A2DP_SEND_SEP_GET_CAPABILITIES_IND,
    A2DP_SEND_SEP_SET_CONFIG_IND,
    A2DP_SEND_SEP_OPEN_IND,
    A2DP_SEND_SEP_START_IND,
    A2DP_SEND_SEP_PAUSE_IND,
    A2DP_SEND_SEP_CLOSE_IND,
    A2DP_SEND_SEP_ABORT_IND,
        A2DP_SEND_SEP_SET_RECONFIG_CNF
}A2DP_Q_T;

const struct {
    kal_int16 	type;
    void 		(*handler)(void  *);
} a2dp_msg_hdlr_table[] = 
{
	{A2DP_SIGNAL_CONNECT_CNF,  rdabt_a2dp_signal_connect_cnf_cb},
	{A2DP_SIGNAL_DISCONN_CNF,rdabt_a2dp_signal_disconnect_cnf_cb},
       {A2DP_SEND_SEP_DISCOVER_CNF,av_rda_bt_a2dp_send_sep_discover_cnf_callback},
	{A2DP_SEND_SEP_GET_CAPABILITIES_CNF,av_rda_bt_a2dp_send_sep_get_capabilities_cnf_callback},
	{A2DP_SEND_SEP_SET_CONFIG_CNF,av_rda_bt_a2dp_send_sep_set_config_cnf_callback},
	{A2DP_SEND_SEP_OPEN_CNF,av_rda_bt_a2dp_send_sep_open_cnf_callback},
	{A2DP_SEND_SEP_START_CNF,av_rda_bt_a2dp_send_sep_start_cnf_callback},
	{A2DP_SEND_SEP_PAUSE_CNF,av_rda_bt_a2dp_send_sep_pause_cnf_callback},
	{A2DP_SEND_SEP_CLOSE_CNF,av_rda_bt_a2dp_send_sep_close_cnf_callback},
	{A2DP_SEND_SEP_ABORT_CNF,av_rda_bt_a2dp_send_sep_abort_cnf_callback},
	{A2DP_ACTIVATE_CNF,rdabt_a2dp_activate_cb},
	{A2DP_DEACTIVATE_CNF,rdabt_a2dp_deactivate_cb},
	{A2DP_SIGNAL_CONNECT_IND,rdabt_a2dp_signal_connect_ind_cb},
	{A2DP_SIGNAL_DISCONNECT_IND,rdabt_a2dp_signal_disconnect_ind_cb},	
	{A2DP_SEND_SEP_DISCOVER_IND,av_rda_bt_a2dp_send_sep_discover_ind_callback},
	{A2DP_SEND_SEP_GET_CAPABILITIES_IND,av_rda_bt_a2dp_send_sep_get_capabilities_ind_callback},
	{A2DP_SEND_SEP_SET_CONFIG_IND,av_rda_bt_a2dp_send_sep_set_config_ind_callback},
	{A2DP_SEND_SEP_OPEN_IND,av_rda_bt_a2dp_send_sep_open_ind_callback},
	{A2DP_SEND_SEP_START_IND,av_rda_bt_a2dp_send_sep_start_ind_callback},
	{A2DP_SEND_SEP_PAUSE_IND,av_rda_bt_a2dp_send_sep_pause_ind_callback},
	{A2DP_SEND_SEP_CLOSE_IND,av_rda_bt_a2dp_send_sep_close_ind_callback},
	{A2DP_SEND_SEP_ABORT_IND,av_rda_bt_a2dp_send_sep_abort_ind_callback},
	{A2DP_SEND_SEP_SET_RECONFIG_CNF,av_rda_bt_a2dp_send_sep_set_reconfig_cnf_callback}
};

void rdabt_a2dp_int_msg_hdlr( kal_uint16 mi,void *mv )
{
    int I;
    int n = sizeof(a2dp_msg_hdlr_table)/sizeof(a2dp_msg_hdlr_table[0]);
    kal_uint16 type = (mi & 0x00ff);
    for(I=0; I<n; I++)
    {
        if( type==a2dp_msg_hdlr_table[I].type )
        {
            a2dp_msg_hdlr_table[I].handler( mv );

            break;
        }
    }

    //ASSERT( I<N ); // not supported by the avadp
}


void rdabt_a2dp_activate_cb(void * pArgs)
{
    kal_uint8 result =*(kal_uint8*)pArgs;
    bt_a2dp_activate_cnf_struct *cnf_param_ptr;
    cnf_param_ptr = (bt_a2dp_activate_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_activate_cnf_struct), TD_CTRL);
	if (result ==BT_NOERROR)
	      cnf_param_ptr ->result =  BT_A2DP_RESULT_OK;
	else
	      cnf_param_ptr ->result =   BT_A2DP_RESULT_FATAL_ERROR;
	
     mmi_trace(1, "rdabt_a2dp_activate_cb result=%d",result);
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_ACTIVATE_CNF, (void *)cnf_param_ptr, NULL);	
}

void rdabt_a2dp_deactivate_cb(void * pArgs)
{
    kal_uint8 result =*(kal_uint8*)pArgs;
    bt_a2dp_deactivate_cnf_struct *cnf_param_ptr;
    cnf_param_ptr = (bt_a2dp_deactivate_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_deactivate_cnf_struct), TD_CTRL);

	if (result ==BT_NOERROR)
	      cnf_param_ptr ->result =  BT_A2DP_RESULT_OK;
	else 
	      cnf_param_ptr ->result =   BT_A2DP_RESULT_FATAL_ERROR;
   
    mmi_trace(1, "rdabt_a2dp_deactivate_cb result=%d",result);
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_DEACTIVATE_CNF, (void *)cnf_param_ptr, NULL);	
}

void rdabt_a2dp_signal_connect_ind_cb(void * pArgs)
{
    t_a2dp_signal_connect_ind *msg =(t_a2dp_signal_connect_ind*)pArgs;
    bt_a2dp_signal_connect_ind_struct *cnf_param_ptr;
    cnf_param_ptr = (bt_a2dp_signal_connect_ind_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_signal_connect_ind_struct), TD_CTRL);
    cnf_param_ptr->connect_id = msg->connect_id;
    cnf_param_ptr->device_addr.lap = BDADDR_Get_LAP(&msg->device_addr);
    cnf_param_ptr->device_addr.nap = BDADDR_Get_NAP(&msg->device_addr);
    cnf_param_ptr->device_addr.uap = BDADDR_Get_UAP(&msg->device_addr);
    
    mmi_trace(1, "rdabt_a2dp_signal_connect_ind_cb msg->connect_id=%x",msg->connect_id);
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_SIGNAL_CONNECT_IND, (void *)cnf_param_ptr, NULL);	
}

void rdabt_a2dp_signal_disconnect_ind_cb(void * pArgs)
{
    kal_uint16 cid =*(kal_uint16*)pArgs;
    bt_a2dp_signal_disconnect_ind_struct *cnf_param_ptr;
    cnf_param_ptr = (bt_a2dp_signal_disconnect_ind_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_signal_disconnect_ind_struct), TD_CTRL);
    cnf_param_ptr->connect_id = cid;
    
    mmi_trace(1, "rdabt_a2dp_signal_disconnect_ind_cb cid=%x",cid);
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_SIGNAL_DISCONNECT_IND, (void *)cnf_param_ptr, NULL);	
}


void rdabt_a2dp_signal_connect_cnf_cb(void * pArgs)
{
    t_L2_ControlCallbackArgs *msg_p = (t_L2_ControlCallbackArgs *)pArgs;
    bt_a2dp_signal_connect_cnf_struct *cnf_param_ptr;
    cnf_param_ptr = (bt_a2dp_signal_connect_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_signal_connect_cnf_struct), TD_CTRL);
    mmi_trace(1, "rdabt_a2dp_signal_connect_cnf_cb msg_p->result=%d", msg_p->result);
    if(msg_p->result== BT_NOERROR)
        cnf_param_ptr ->result = BT_A2DP_RESULT_OK;
    else
        cnf_param_ptr ->result = BT_A2DP_RESULT_FATAL_ERROR;

    cnf_param_ptr->device_addr.lap = BDADDR_Get_LAP(&msg_p->bdaddr);
    cnf_param_ptr->device_addr.uap = BDADDR_Get_UAP(&msg_p->bdaddr);
    cnf_param_ptr->device_addr.nap = BDADDR_Get_NAP(&msg_p->bdaddr);

    cnf_param_ptr->connect_id = msg_p->cid;
    
    mmi_trace(1, "rdabt_a2dp_signal_connect_cnf_cb cid=%x,lap:%x,uap:%x,nap:%x", msg_p->cid,cnf_param_ptr->device_addr.lap,cnf_param_ptr->device_addr.uap,cnf_param_ptr->device_addr.nap);
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_SIGNAL_CONNECT_CNF, (void *)cnf_param_ptr, NULL);	
}

void rdabt_a2dp_signal_disconnect_cnf_cb(void * pArgs)
{
    t_L2_ControlCallbackArgs *msg_p = (t_L2_ControlCallbackArgs *)pArgs;
    bt_a2dp_signal_disconnect_cnf_struct *cnf_param_ptr;

    cnf_param_ptr = (bt_a2dp_signal_disconnect_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_signal_disconnect_cnf_struct), TD_CTRL);

    if(msg_p->status== BT_NOERROR)
        cnf_param_ptr ->result = BT_A2DP_RESULT_OK;
    else
        cnf_param_ptr ->result = BT_A2DP_RESULT_FATAL_ERROR;

    cnf_param_ptr->connect_id = msg_p->cid;
     
    mmi_trace(1, "rdabt_a2dp_signal_disconnect_cnf_cb");
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_SIGNAL_DISCONNECT_CNF, (void *)cnf_param_ptr, NULL);	
}


void av_rda_bt_a2dp_send_sep_discover_cnf_callback(void *pArgs)
{

    bt_a2dp_sep_discover_cnf_struct *cnf_param_ptr;
    kal_uint8 *msg_p =(kal_uint8*)pArgs;
    kal_uint8 data_length = msg_p[0];
    kal_uint8 result = (msg_p[1] & 3);
    kal_uint8 *ptr= &msg_p[3];
    int i,j;

    //mmi_trace(1, "av_rda_bt_a2dp_send_sep_discover_cnf_callback data_length=%d, result=%d",data_length,result);
    
    cnf_param_ptr = (bt_a2dp_sep_discover_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_sep_discover_cnf_struct), TD_CTRL);

    for (i=j = 0;  i<(data_length-2); i+=2)
    {
        cnf_param_ptr->sep_list[j].seid= (ptr[i])>>2;
        cnf_param_ptr->sep_list[j].in_use =  (ptr[i]>>1) & 1;
        cnf_param_ptr->sep_list[j].media_type = ptr[i+1] >> 4;
        cnf_param_ptr->sep_list[j].sep_type = (ptr[i+1] >> 3) & 1;
        j++;
    }
    
    cnf_param_ptr ->sep_num = j;

    if(result == BT_A2DP_MESSAGE_TYPE_ACCEPT)
        cnf_param_ptr ->result = BT_A2DP_RESULT_OK;
    else
        cnf_param_ptr ->result = BT_A2DP_RESULT_FATAL_ERROR;
    
    cnf_param_ptr ->connect_id = 0;
    mmi_trace(1, "discover_cnf_callback sep_num=%d, seid[0]=%x, seid[0].in_use:%d,  seid[1]=%x, seid[1].in_use:%d ",cnf_param_ptr->sep_num, cnf_param_ptr->sep_list[0].seid, cnf_param_ptr->sep_list[0].in_use,
                                                                                                                                                                    cnf_param_ptr->sep_list[1].seid, cnf_param_ptr->sep_list[1].in_use);
  rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_SEP_DISCOVER_CNF, (void *)cnf_param_ptr, NULL);	

  
}

void av_rda_bt_a2dp_send_sep_get_capabilities_cnf_callback(void *pArgs)
{

  
     kal_uint8 *msg_p =(kal_uint8*)pArgs;
    bt_a2dp_capabilities_get_cnf_struct *cnf_param_ptr;
    kal_uint8 result = (msg_p[0] & 3);
    kal_uint8 code_type;
    kal_uint8 media_codec[6];
    kal_uint8 losc = 0;
    kal_uint8 num =0;

    msg_p+=2; //skip the packet type, message type, and singnaling ID
    while(*msg_p !=AVDTP_SERVICE_MEDIA_CODEC &&num<7)
    {
        losc = *(msg_p +1);
        if(losc !=0)
        {
            msg_p +=(losc+2);
        }
        else
            msg_p+=2;
        num++;
    }
    
    msg_p ++;
    losc = *msg_p;
    msg_p++;
    
    memcpy(media_codec, msg_p, losc);

    code_type = media_codec[1];
    
    mmi_trace(1, "av_rda_bt_a2dp_send_sep_get_capabilities_cnf_callback code_type=%d, result=%d",code_type,result);
    
    cnf_param_ptr = (bt_a2dp_capabilities_get_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_capabilities_get_cnf_struct), TD_CTRL);

    if(result == BT_A2DP_MESSAGE_TYPE_ACCEPT)
        cnf_param_ptr ->result = BT_A2DP_RESULT_OK;
    else
        cnf_param_ptr ->result = BT_A2DP_RESULT_FATAL_ERROR;

     cnf_param_ptr ->connect_id = 0;

    cnf_param_ptr->audio_cap_num=1;   //only 1 cap.
    if(code_type == 0)
    {
        cnf_param_ptr->audio_cap_list[0].codec_type =  BT_A2DP_SBC;
        cnf_param_ptr->audio_cap_list[0].codec_cap.sbc.sample_rate = (media_codec[2]&0xF0)>>4;
        cnf_param_ptr->audio_cap_list[0].codec_cap.sbc.channel_mode = (media_codec[2]&0x0F);
        cnf_param_ptr->audio_cap_list[0].codec_cap.sbc.block_len = (media_codec[3]&0xF0)>>4;
        cnf_param_ptr->audio_cap_list[0].codec_cap.sbc.subband_num = (media_codec[3]&0x0C)>>2;
        cnf_param_ptr->audio_cap_list[0].codec_cap.sbc.alloc_method = (media_codec[3]&0x03);
        cnf_param_ptr->audio_cap_list[0].codec_cap.sbc.min_bit_pool = media_codec[4];
        cnf_param_ptr->audio_cap_list[0].codec_cap.sbc.max_bit_pool = media_codec[5];
        mmi_trace(1, "SBC cap sample_rate:%d, ch_mode:%x, block_len:%d, subband_num:%d, method:%d, min_pool:%d,max_pool:%d",
                                           cnf_param_ptr->audio_cap_list[0].codec_cap.sbc.sample_rate ,
                                            cnf_param_ptr->audio_cap_list[0].codec_cap.sbc.channel_mode ,
                                            cnf_param_ptr->audio_cap_list[0].codec_cap.sbc.block_len ,
                                            cnf_param_ptr->audio_cap_list[0].codec_cap.sbc.subband_num,
                                            cnf_param_ptr->audio_cap_list[0].codec_cap.sbc.alloc_method,
                                            cnf_param_ptr->audio_cap_list[0].codec_cap.sbc.min_bit_pool,
                                            cnf_param_ptr->audio_cap_list[0].codec_cap.sbc.max_bit_pool);
    }
    else if(code_type == 1)
    {
        cnf_param_ptr->audio_cap_list[0].codec_type =  BT_A2DP_MP3;
        cnf_param_ptr->audio_cap_list[0].codec_cap.mp3.layer = (media_codec[2]&0x20)>>5;
        cnf_param_ptr->audio_cap_list[0].codec_cap.mp3.CRC = (media_codec[2]&0x10)>>4;
        cnf_param_ptr->audio_cap_list[0].codec_cap.mp3.channel_mode = (media_codec[2]&0x0F);
        cnf_param_ptr->audio_cap_list[0].codec_cap.mp3.MPF = (media_codec[3]&0x40)>>6;
        cnf_param_ptr->audio_cap_list[0].codec_cap.mp3.sample_rate = (media_codec[3]&0x3F);
        cnf_param_ptr->audio_cap_list[0].codec_cap.mp3.VBR = (media_codec[4]&0x80)>>7;
        cnf_param_ptr->audio_cap_list[0].codec_cap.mp3.bit_rate = (kal_uint16)((media_codec[5])|(media_codec[5]&0x7f)<<8);
        mmi_trace(1, "MP3 cap layer:%d, CRC:%x, channel_mode:%d, MPF:%d, sample_rate:%d, VBR:%d,bit_rate:%d",
                                            cnf_param_ptr->audio_cap_list[0].codec_cap.mp3.layer ,
                                            cnf_param_ptr->audio_cap_list[0].codec_cap.mp3.CRC ,
                                            cnf_param_ptr->audio_cap_list[0].codec_cap.mp3.channel_mode,
                                            cnf_param_ptr->audio_cap_list[0].codec_cap.mp3.MPF,
                                            cnf_param_ptr->audio_cap_list[0].codec_cap.mp3.sample_rate,
                                            cnf_param_ptr->audio_cap_list[0].codec_cap.mp3.VBR,
                                            cnf_param_ptr->audio_cap_list[0].codec_cap.mp3.bit_rate);
    }   

       
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_CAPABILITIES_GET_CNF, (void *)cnf_param_ptr, NULL);	
}

void av_rda_bt_a2dp_send_sep_set_config_cnf_callback(void *pArgs)
{
    bt_a2dp_stream_config_cnf_struct *cnf_param_ptr;
    kal_uint8 *msg_p =(kal_uint8*)pArgs;
    kal_uint8 result = (msg_p[0] & 3);

        
    cnf_param_ptr = (bt_a2dp_stream_config_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_stream_config_cnf_struct), TD_CTRL);

    cnf_param_ptr->connect_id = 0;
    
    if(result == BT_A2DP_MESSAGE_TYPE_ACCEPT)
        cnf_param_ptr ->result = BT_A2DP_RESULT_OK;
    else if(result == BT_A2DP_MESSAGE_TYPE_REJECT)
    {
        kal_uint8 error_code = msg_p[3];
        switch(error_code)
        {
            case 0x12:
               cnf_param_ptr ->result = BT_A2DP_RESULT_BAD_ACP_SEID; 
               break;
               
            case 0x13:
                cnf_param_ptr ->result = BT_A2DP_RESULT_SEP_IND_USE; 
                break;
                
            case  0x11:
            case 0x17:
            case 0x18:
            case 0x19:
            case 0x22:
            case 0x23:
            case 0x25:
            case 0x26:
            case 0x27:
            case 0x29:
                cnf_param_ptr ->result = BT_A2DP_RESULT_FATAL_ERROR; 
            break;

            case 0x31:
                cnf_param_ptr ->result = BT_A2DP_RESULT_BAD_STATE; 
            break;
            default:
                break;
                                                    
        }
    }

    cnf_param_ptr->stream_handle = stream_handle;

    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_STREAM_CONFIG_CNF, (void *)cnf_param_ptr, NULL);	

	
}
void av_rda_bt_a2dp_send_sep_set_reconfig_cnf_callback(void *pArgs)
{
    bt_a2dp_stream_reconfig_cnf_struct *cnf_param_ptr;
    kal_uint8 *msg_p =(kal_uint8*)pArgs;
    kal_uint8 result = (msg_p[0] & 3);

        mmi_trace(1,"reconfig_cnf_callback msg_p[0-3]=%d,%d,%d,%d",msg_p[0],msg_p[1],msg_p[2],msg_p[3]);
    cnf_param_ptr = (bt_a2dp_stream_reconfig_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_stream_reconfig_cnf_struct), TD_CTRL);

    
    if(result == BT_A2DP_MESSAGE_TYPE_ACCEPT)
        cnf_param_ptr ->result = BT_A2DP_RESULT_OK;
    else if(result == BT_A2DP_MESSAGE_TYPE_REJECT)
    {
        kal_uint8 error_code = msg_p[3];
        switch(error_code)
        {
            case 0x12:
               cnf_param_ptr ->result = BT_A2DP_RESULT_BAD_ACP_SEID; 
               break;
               
            case 0x13:
                cnf_param_ptr ->result = BT_A2DP_RESULT_SEP_IND_USE; 
                break;
                
            case  0x11:
            case 0x17:
            case 0x18:
            case 0x19:
            case 0x22:
            case 0x23:
            case 0x25:
            case 0x26:
            case 0x27:
            case 0x29:
                cnf_param_ptr ->result = BT_A2DP_RESULT_FATAL_ERROR; 
            break;

            case 0x31:
                cnf_param_ptr ->result = BT_A2DP_RESULT_BAD_STATE; 
            break;
            default:
                break;
                                                    
        }
    }

    cnf_param_ptr->stream_handle = stream_handle;

    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_STREAM_RECONFIG_CNF, (void *)cnf_param_ptr, NULL);	

	
}

void av_rda_bt_a2dp_send_sep_open_cnf_callback(void *pArgs)
{
    bt_a2dp_stream_open_cnf_struct *cnf_param_ptr;
    kal_uint8 *msg_p =(kal_uint8*)pArgs;
    kal_uint8 result = (msg_p[0] & 3);
    cnf_param_ptr = (bt_a2dp_stream_open_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_stream_open_cnf_struct), TD_CTRL);
    
    cnf_param_ptr->stream_handle = stream_handle;
    
    if(result == BT_A2DP_MESSAGE_TYPE_ACCEPT)
        cnf_param_ptr ->result = BT_A2DP_RESULT_OK;
    else
        cnf_param_ptr ->result = BT_A2DP_RESULT_FATAL_ERROR;
    
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_STREAM_OPEN_CNF, (void *)cnf_param_ptr, NULL);	
	
}


void av_rda_bt_a2dp_send_sep_start_cnf_callback(void *pArgs)
{
    bt_a2dp_stream_start_cnf_struct *cnf_param_ptr;
    kal_uint8 *msg_p = (kal_uint8*)pArgs;
    kal_uint8 result = (msg_p[0] & 3);
    cnf_param_ptr = (bt_a2dp_stream_start_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_stream_start_cnf_struct), TD_CTRL);
    
    cnf_param_ptr->stream_handle =stream_handle;
    
    if(result == BT_A2DP_MESSAGE_TYPE_ACCEPT)
        cnf_param_ptr ->result = BT_A2DP_RESULT_OK;
    else
        cnf_param_ptr ->result = BT_A2DP_RESULT_FATAL_ERROR;
    
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_STREAM_START_CNF, (void *)cnf_param_ptr, NULL);	
	
}


void av_rda_bt_a2dp_send_sep_pause_cnf_callback(void *pArgs)
{
    bt_a2dp_stream_pause_cnf_struct *cnf_param_ptr;
    kal_uint8 *msg_p = (kal_uint8*)pArgs;
    kal_uint8 result = (msg_p[0] & 3);
    cnf_param_ptr = (bt_a2dp_stream_pause_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_stream_pause_cnf_struct), TD_CTRL);

    cnf_param_ptr->stream_handle = stream_handle;

    if(result == BT_A2DP_MESSAGE_TYPE_ACCEPT)
        cnf_param_ptr ->result = BT_A2DP_RESULT_OK;
    else
        cnf_param_ptr ->result = BT_A2DP_RESULT_FATAL_ERROR;

    mmi_trace(1, "av_rda_bt_a2dp_send_sep_pause_cnf_callback result= %d",result);
    
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_STREAM_PAUSE_CNF, (void *)cnf_param_ptr, NULL);	

	
}

void av_rda_bt_a2dp_send_sep_close_cnf_callback(void *pArgs)
{
    bt_a2dp_stream_close_cnf_struct *cnf_param_ptr;
    kal_uint8 *msg_p = (kal_uint8*)pArgs;
    kal_uint8 result = (msg_p[0] & 3);
    cnf_param_ptr = (bt_a2dp_stream_close_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_stream_close_cnf_struct), TD_CTRL);

    cnf_param_ptr->stream_handle = stream_handle;

    if(result == BT_A2DP_MESSAGE_TYPE_ACCEPT)
        cnf_param_ptr ->result = BT_A2DP_RESULT_OK;
    else
        cnf_param_ptr ->result = BT_A2DP_RESULT_FATAL_ERROR;

    //mmi_trace(1, "av_rda_bt_a2dp_send_sep_close_cnf_callback result= %d",result);
    
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_STREAM_CLOSE_CNF, (void *)cnf_param_ptr, NULL);	

	
}

void av_rda_bt_a2dp_send_sep_abort_cnf_callback(void *pArgs)
{
    bt_a2dp_stream_abort_cnf_struct *cnf_param_ptr;
 //   kal_uint8 *msg_p = (kal_uint8*)pArgs;
    cnf_param_ptr = (bt_a2dp_stream_abort_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_stream_abort_cnf_struct), TD_CTRL);

    cnf_param_ptr->stream_handle = stream_handle;

    mmi_trace(1, "av_rda_bt_a2dp_send_sep_abort_cnf_callback");

    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_STREAM_ABORT_CNF, (void *)cnf_param_ptr, NULL);	
}

void av_rda_bt_a2dp_send_sep_discover_ind_callback(void * pArgs)
{
    kal_uint16 cid = *(kal_uint16*)pArgs;
    bt_a2dp_sep_discover_ind_struct *cnf_param_ptr;
    mmi_trace(1, "av_rda_bt_a2dp_send_sep_discover_ind_callback");

    cnf_param_ptr = (bt_a2dp_sep_discover_ind_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_sep_discover_ind_struct), TD_CTRL);
    cnf_param_ptr->connect_id = cid;

    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_SEP_DISCOVER_IND, (void *)cnf_param_ptr, NULL);	
}

void av_rda_bt_a2dp_send_sep_get_capabilities_ind_callback(void * pArgs)
{
    t_a2dp_capabilities_get_ind *res = (t_a2dp_capabilities_get_ind*)pArgs;
    bt_a2dp_capabilities_get_ind_struct *cnf_param_ptr;
    mmi_trace(1, "av_rda_bt_a2dp_send_sep_get_capabilities_ind_callback");

    cnf_param_ptr = (bt_a2dp_capabilities_get_ind_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_capabilities_get_ind_struct), TD_CTRL);
    cnf_param_ptr->acp_seid = res->acp_seid;    
    cnf_param_ptr->connect_id = res->connect_id;

    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_CAPABILITIES_GET_IND, (void *)cnf_param_ptr, NULL);	
}

void av_rda_bt_a2dp_send_sep_set_config_ind_callback(void * pArgs)
{
    t_a2dp_stream_config_ind *res = (t_a2dp_stream_config_ind *)pArgs;
    bt_a2dp_stream_config_ind_struct *cnf_param_ptr;
    kal_uint8 code_type = res->audio_cap.codec_type;
    mmi_trace(1, "av_rda_bt_a2dp_send_sep_set_config_ind_callback ");
    
    cnf_param_ptr = (bt_a2dp_stream_config_ind_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_stream_config_ind_struct), TD_CTRL);
    cnf_param_ptr->connect_id = res->connect_id;
    cnf_param_ptr->acp_seid= res->acp_seid;
    cnf_param_ptr->int_seid= res->int_seid;
    cnf_param_ptr->stream_handle= res->stream_handle;
    cnf_param_ptr->audio_cap.codec_type = code_type;
    if(code_type == BT_A2DP_SBC) 
    {
        cnf_param_ptr->audio_cap.codec_cap.sbc.alloc_method = res->audio_cap.codec_cap.sbc.alloc_method;
        cnf_param_ptr->audio_cap.codec_cap.sbc.block_len  = res->audio_cap.codec_cap.sbc.block_len;
        cnf_param_ptr->audio_cap.codec_cap.sbc.channel_mode = res->audio_cap.codec_cap.sbc.channel_mode;
        cnf_param_ptr->audio_cap.codec_cap.sbc.max_bit_pool = res->audio_cap.codec_cap.sbc.max_bit_pool;
        cnf_param_ptr->audio_cap.codec_cap.sbc.min_bit_pool = res->audio_cap.codec_cap.sbc.min_bit_pool;
        cnf_param_ptr->audio_cap.codec_cap.sbc.sample_rate = res->audio_cap.codec_cap.sbc.sample_rate;
        cnf_param_ptr->audio_cap.codec_cap.sbc.subband_num = res->audio_cap.codec_cap.sbc.subband_num;
    }
    else if(code_type == BT_A2DP_MP3) 
    {
        cnf_param_ptr->audio_cap.codec_cap.mp3.bit_rate = res->audio_cap.codec_cap.mp3.bit_rate;
        cnf_param_ptr->audio_cap.codec_cap.mp3.channel_mode = res->audio_cap.codec_cap.mp3.channel_mode;
        cnf_param_ptr->audio_cap.codec_cap.mp3.CRC = res->audio_cap.codec_cap.mp3.CRC;
        cnf_param_ptr->audio_cap.codec_cap.mp3.layer = res->audio_cap.codec_cap.mp3.layer;
        cnf_param_ptr->audio_cap.codec_cap.mp3.MPF = res->audio_cap.codec_cap.mp3.MPF;
        cnf_param_ptr->audio_cap.codec_cap.mp3.sample_rate = res->audio_cap.codec_cap.mp3.sample_rate;
        cnf_param_ptr->audio_cap.codec_cap.mp3.VBR = res->audio_cap.codec_cap.mp3.VBR;
    }
    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_STREAM_CONFIG_IND, (void *)cnf_param_ptr, NULL);	
}

void av_rda_bt_a2dp_send_sep_open_ind_callback(void * pArgs)
{
    kal_uint8 handle = *(kal_uint8*)pArgs;
    bt_a2dp_stream_open_ind_struct *cnf_param_ptr;

    mmi_trace(1, "av_rda_bt_a2dp_send_sep_open_ind_callback");
    cnf_param_ptr = (bt_a2dp_stream_open_ind_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_stream_open_ind_struct), TD_CTRL);
    cnf_param_ptr->stream_handle = handle; //store the acp endpoint.

    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_STREAM_OPEN_IND, (void *)cnf_param_ptr, NULL);	
}

void av_rda_bt_a2dp_send_sep_start_ind_callback(void * pArgs)
{
    kal_uint8 handle = *(kal_uint8*)pArgs;
    bt_a2dp_stream_start_ind_struct *cnf_param_ptr;
    mmi_trace(1, "av_rda_bt_a2dp_send_sep_start_ind_callback");

    cnf_param_ptr = (bt_a2dp_stream_start_ind_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_stream_start_ind_struct), TD_CTRL);
    cnf_param_ptr->stream_handle = handle; //store the acp endpoint.

    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_STREAM_START_IND, (void *)cnf_param_ptr, NULL);	
}

void av_rda_bt_a2dp_send_sep_pause_ind_callback(void * pArgs)
{
    kal_uint8 handle = *(kal_uint8*)pArgs;
    bt_a2dp_stream_pause_ind_struct *cnf_param_ptr;
    mmi_trace(1, "av_rda_bt_a2dp_send_sep_abort_ind_callback");
    
    cnf_param_ptr = (bt_a2dp_stream_pause_ind_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_stream_pause_ind_struct), TD_CTRL);
    cnf_param_ptr->stream_handle = handle; //store the acp endpoint.

    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_STREAM_PAUSE_IND, (void *)cnf_param_ptr, NULL);	
}

void av_rda_bt_a2dp_send_sep_close_ind_callback(void * pArgs)
{
    kal_uint8 handle = *(kal_uint8*)pArgs;
    bt_a2dp_stream_close_ind_struct *cnf_param_ptr;
    mmi_trace(1, "av_rda_bt_a2dp_send_sep_close_ind_callback");
    
    cnf_param_ptr = (bt_a2dp_stream_close_ind_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_stream_close_ind_struct), TD_CTRL);
    cnf_param_ptr->stream_handle = handle; //store the acp endpoint.

    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_STREAM_CLOSE_IND, (void *)cnf_param_ptr, NULL);	
}

void av_rda_bt_a2dp_send_sep_abort_ind_callback(void * pArgs)
{
    kal_uint8 handle = *(kal_uint8*)pArgs;
    bt_a2dp_stream_abort_ind_struct *cnf_param_ptr;
    mmi_trace(1, "av_rda_bt_a2dp_send_sep_abort_ind_callback");
    
    cnf_param_ptr = (bt_a2dp_stream_abort_ind_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_stream_abort_ind_struct), TD_CTRL);
    cnf_param_ptr->stream_handle = handle; //store the acp endpoint.

    rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_STREAM_ABORT_IND, (void *)cnf_param_ptr, NULL);	
}

extern void checkMsg(void *message);

void rdabt_a2dp_msg_hdlr(ilm_struct *message)
{

        switch(message->msg_id)
	{
         case MSG_ID_BT_A2DP_STREAM_DATA_SEND_REQ:
            {
	//	  checkMsg(message);
	      
                bt_a2dp_stream_data_send_req_struct  *msg_p = (bt_a2dp_stream_data_send_req_struct*)message->local_para_ptr;
	//	  checkMsg(msg_p);		
                rda_bt_a2dp_send_stream_data_send_req(MOD_MMI, msg_p->stream_handle, msg_p->codec);
                break;
            }
	
		case MSG_ID_BT_A2DP_ACTIVATE_REQ:
		{
			bt_a2dp_activate_req_struct * msg_p = (bt_a2dp_activate_req_struct*)message->local_para_ptr;
			rda_bt_a2dp_send_activate_req(MOD_MMI,msg_p->local_role);
			break;
		}

		case MSG_ID_BT_A2DP_DEACTIVATE_REQ:
		{
			  rda_bt_a2dp_send_deactivate_req(MOD_MMI);
			break;
		}
		
            //A2DP
            case MSG_ID_BT_A2DP_SIGNAL_CONNECT_REQ:
            {
                t_bdaddr bd_addr;
		    kal_uint8 status ;	
		
                bt_a2dp_signal_connect_req_struct * msg_p = (bt_a2dp_signal_connect_req_struct*)message->local_para_ptr;
                BDADDR_Set_LAP_UAP_NAP(&bd_addr, msg_p->device_addr.lap, msg_p->device_addr.uap, msg_p->device_addr.nap);                   
               status= rda_bt_a2dp_send_signal_connect_req(MOD_MMI, bd_addr, msg_p->local_role);

#ifndef  MGR_MULTI_CONNECTION 

	               if (BT_NORESOURCES ==status)
	 	        {
			     mmi_trace(1,"rda_bt_a2dp_send_signal_connect_req fail!!!!");	

			    bt_a2dp_signal_connect_cnf_struct *cnf_param_ptr;
			    cnf_param_ptr = (bt_a2dp_signal_connect_cnf_struct *)construct_local_para( (kal_uint16)sizeof(bt_a2dp_signal_connect_cnf_struct), TD_CTRL);
			  
			    cnf_param_ptr ->result = BT_A2DP_RESULT_FATAL_ERROR;

			    cnf_param_ptr->device_addr.lap = BDADDR_Get_LAP(&bd_addr);
			    cnf_param_ptr->device_addr.uap = BDADDR_Get_UAP(&bd_addr);
			    cnf_param_ptr->device_addr.nap = BDADDR_Get_NAP(&bd_addr);

			    cnf_param_ptr->connect_id =(kal_uint16) NULL;
			    
		           rdabt_send_msg_up(MOD_MMI, MSG_ID_BT_A2DP_SIGNAL_CONNECT_CNF, (void *)cnf_param_ptr, NULL);	

			}
 #endif			
                break;
            }
            
            case MSG_ID_BT_A2DP_SIGNAL_DISCONNECT_REQ:
            {
                bt_a2dp_signal_disconnect_req_struct * msg_p = (bt_a2dp_signal_disconnect_req_struct*)message->local_para_ptr;
                rda_bt_a2dp_send_signal_disconnect_req(MOD_MMI, msg_p->connect_id);
                break;
            }

              case MSG_ID_BT_A2DP_SEP_DISCOVER_REQ:
            {
                bt_a2dp_sep_discover_req_struct * msg_p = (bt_a2dp_sep_discover_req_struct*)message->local_para_ptr;
                rda_bt_a2dp_send_sep_discover_req(MOD_MMI, msg_p->connect_id);
                break;
            }
            
            case MSG_ID_BT_A2DP_CAPABILITIES_GET_REQ:
            {
                bt_a2dp_capabilities_get_req_struct * msg_p = (bt_a2dp_capabilities_get_req_struct*)message->local_para_ptr;
                rda_bt_a2dp_send_capabilities_get_req(MOD_MMI, msg_p->connect_id, msg_p->acp_seid);
                break;
            }


            case MSG_ID_BT_A2DP_STREAM_CONFIG_REQ:
            {
                bt_a2dp_stream_config_req_struct * msg_p = (bt_a2dp_stream_config_req_struct*)message->local_para_ptr;
                stream_handle = msg_p->acp_seid;
                rda_bt_a2dp_send_stream_config_req(msg_p->connect_id, 
                                                                        msg_p->acp_seid, 
                                                                        msg_p->int_seid, 
                                                                        msg_p->audio_cap.codec_type,
                                                                        &msg_p->audio_cap.codec_cap
                                                                      );
                break;
            }            

            case MSG_ID_BT_A2DP_STREAM_OPEN_REQ:
            {
                bt_a2dp_stream_open_req_struct * msg_p = (bt_a2dp_stream_open_req_struct*)message->local_para_ptr;
                rda_bt_a2dp_send_stream_open_req(MOD_MMI, msg_p->stream_handle);
                break;
            }

      		case MSG_ID_BT_A2DP_STREAM_RECONFIG_REQ:
            {
                bt_a2dp_stream_reconfig_req_struct * msg_p = (bt_a2dp_stream_reconfig_req_struct*)message->local_para_ptr;
                stream_handle = msg_p->acp_seid;
		   mmi_trace(1,"rdatmain reconfig_req msg_p->audio_cap.codec_type=%d",\
		   	msg_p->audio_cap.codec_type);	
		   
		/*mmi_trace(g_sw_BT,"rdatmain reconfig_req MP3 av_bt_ctx.cfg,bit_rate=%d,channel_mode=%d, CRC=%d, layer=%d, MPF=%d,sample_rate=%d, VBR=%d,",\
		msg_p->audio_cap.codec_cap.mp3.bit_rate,
		msg_p->audio_cap.codec_cap.mp3.channel_mode,
		msg_p->audio_cap.codec_cap.mp3.CRC,
		msg_p->audio_cap.codec_cap.mp3.layer,
		msg_p->audio_cap.codec_cap.mp3.MPF,
		msg_p->audio_cap.codec_cap.mp3.sample_rate,
		msg_p->audio_cap.codec_cap.mp3.VBR);*/
		
                rda_bt_a2dp_send_stream_reconfig_req(msg_p->connect_id, 
                                                                        msg_p->acp_seid,                                                                         
                                                                        msg_p->audio_cap.codec_type,
                                                                        &msg_p->audio_cap.codec_cap
                                                                      );

			
		break;
		}
            case MSG_ID_BT_A2DP_STREAM_START_REQ:
            {
                bt_a2dp_stream_start_req_struct * msg_p = (bt_a2dp_stream_start_req_struct*)message->local_para_ptr;
                rda_bt_a2dp_send_stream_start_req(MOD_MMI, msg_p->stream_handle);
                break;
            }

       
            case MSG_ID_BT_A2DP_STREAM_PAUSE_REQ:
            {
                bt_a2dp_stream_pause_req_struct * msg_p = (bt_a2dp_stream_pause_req_struct*)message->local_para_ptr;
                rda_bt_a2dp_send_stream_pause_req(MOD_MMI,msg_p->stream_handle,msg_p->codec);
                break;
            }

            case MSG_ID_BT_A2DP_STREAM_CLOSE_REQ:
            {
                bt_a2dp_stream_close_req_struct * msg_p = (bt_a2dp_stream_close_req_struct*)message->local_para_ptr;
               rda_bt_a2dp_send_stream_close_req(MOD_MMI, msg_p->stream_handle);
                break;
            }

                case MSG_ID_BT_A2DP_STREAM_ABORT_REQ:
            {
                bt_a2dp_stream_abort_req_struct * msg_p = (bt_a2dp_stream_abort_req_struct*)message->local_para_ptr;
                rda_bt_a2dp_send_stream_abort_req(MOD_MMI, msg_p->stream_handle);
                break;
            }

   
           //end of A2DP msg
		default:
			ASSERT(0);
			break;
	
	}



}

kal_uint8 Avdtp_Check_SBC_Codecs_Compatible(bt_a2dp_sbc_codec_cap_struct local_cap, bt_a2dp_sbc_codec_cap_struct req_cfg)
{
    mmi_trace(1,"SBC Local_Cap max_pool:%d, min_pool:%d, block_len:%d, subband_num:%d,alloc_method:%d, sample_rate:%d, channel_mode:%d",
                local_cap.max_bit_pool, local_cap.min_bit_pool,local_cap.block_len,local_cap.subband_num, local_cap.alloc_method,local_cap.sample_rate, local_cap.channel_mode);

    mmi_trace(1,"SBC Req_Cfg max_pool:%d, min_pool:%d, block_len:%d, subband_num:%d,alloc_method:%d, sample_rate:%d, channel_mode:%d",
                req_cfg.max_bit_pool, req_cfg.min_bit_pool,req_cfg.block_len,req_cfg.subband_num, req_cfg.alloc_method,req_cfg.sample_rate, req_cfg.channel_mode);

    if(local_cap.min_bit_pool> req_cfg.min_bit_pool)
        return KAL_FALSE;

    //kal_prompt_trace(1,"Avdtp_Check_SBC_Codecs_Compatible local_cap.max_pool:%d, req_cfg.max_pool:%d",local_cap.max_bit_pool, req_cfg.max_bit_pool);
    //if(local_cap.max_bit_pool < req_cfg.max_bit_pool)
    //    req_cfg.max_bit_pool = local_cap.max_bit_pool;
        //return KAL_FALSE;    

    if(!(local_cap.block_len & req_cfg.block_len))
        return KAL_FALSE;   

    if(!(local_cap.subband_num & req_cfg.subband_num))
        return KAL_FALSE;   

    if(!(local_cap.alloc_method & req_cfg.alloc_method))
        return KAL_FALSE;   

    if(!(local_cap.sample_rate & req_cfg.sample_rate))
        return KAL_FALSE; 

    if(!(local_cap.channel_mode & req_cfg.channel_mode))
        return KAL_FALSE; 

    return KAL_TRUE;
}

kal_uint8 Avdtp_Check_MP3_Codecs_Compatible(bt_a2dp_mp3_codec_cap_struct local_cap, bt_a2dp_mp3_codec_cap_struct req_cfg)
{
    mmi_trace(1,"MP3 Local_Cap layer:%d, channel_mode:%d, sample_rate:%d, bit_rate:%d",local_cap.layer, local_cap.channel_mode,local_cap.sample_rate,local_cap.bit_rate);

    mmi_trace(1,"MP3 Req_Cfg layer:%d, channel_mode:%d, sample_rate:%d, bit_rate:%d",req_cfg.layer, req_cfg.channel_mode,req_cfg.sample_rate,req_cfg.bit_rate);


    if(!(local_cap.layer & req_cfg.layer))
        return KAL_FALSE;

    if(!(local_cap.channel_mode & req_cfg.channel_mode))
        return KAL_FALSE;   

    if(!(local_cap.sample_rate & req_cfg.sample_rate))
        return KAL_FALSE;   

    if(!(local_cap.bit_rate & req_cfg.bit_rate))
        return KAL_FALSE; 

    return KAL_TRUE;    
}

kal_uint8 rda_bt_a2dp_match_audio_capabilities( bt_a2dp_audio_cap_struct *audio_cap, bt_a2dp_audio_cap_struct *audio_cfg, kal_uint16 *result )
{
    kal_uint8 status = KAL_TRUE;
    
    mmi_trace(g_sw_BT, "rda_bt_a2dp_match_audio_capabilities audio_cap->codec_type=%x,audio_cfg->codec_type=%x,",audio_cap->codec_type,audio_cfg->codec_type);
    if(audio_cap->codec_type != audio_cfg->codec_type)
    {
        *result = KAL_FALSE;
        return KAL_FALSE;
    }

    if(audio_cap->codec_type == BT_A2DP_SBC)
    {
        status = Avdtp_Check_SBC_Codecs_Compatible(audio_cap->codec_cap.sbc, audio_cfg->codec_cap.sbc);
    }
    else if(audio_cap->codec_type == BT_A2DP_MP3)
    {
        status = Avdtp_Check_MP3_Codecs_Compatible(audio_cap->codec_cap.mp3, audio_cfg->codec_cap.mp3);
    }
    *result = status; 

   return status;
}
extern kal_uint8 mmi_bt_is_a2dp_connect(void);
U8 mmi_bt_is_avrcp_connect(void);



unsigned char is_a2dp_connect(void)
{
	if( (mmi_bt_is_a2dp_connect()) || (mmi_bt_is_avrcp_connect()) )
		return 1;
	return 0;
}

#else // !__BT_A2DP_PROFILE__

unsigned char is_a2dp_connect(void)
{
	return 0;
}

#endif // !__BT_A2DP_PROFILE__

