
#ifndef _RDABT_AVRCP_H_
#define  _RDABT_AVRCP_H_

void rdabt_avrcp_activate_cb(void * pArgs);
void rdabt_avrcp_deactivate_cb(void * pArgs);
void rdabt_avrcp_connect_ind_cb(void * pArgs);
void rdabt_avrcp_connect_cnf_cb(void * pArgs);
void rdabt_avrcp_disconnect_ind_cb(void * pArgs);
void rdabt_avrcp_disconnect_cnf_cb(void * pArgs);
void rdabt_avrcp_cmd_frame_cnf_cb(void * pArgs);
void rdabt_avrcp_cmd_frame_ind_cb(void * pArgs);
void rdabt_avrcp_msg_hdlr(ilm_struct *message);
void rdabt_avrcp_int_msg_hdlr( kal_uint16 mi,void *mv );


#endif
