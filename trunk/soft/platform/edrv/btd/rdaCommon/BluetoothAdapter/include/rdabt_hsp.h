
#ifndef _RDABT_HSP_H_
#define _RDABT_HSP_H_

typedef struct
{
    kal_uint8         result;
    t_bdaddr       bt_addr;
    kal_uint16         connection_id;
} st_t_hsp_connect_cnf;

typedef struct
{
    t_bdaddr bt_addr;
    kal_uint16   connection_id;    
} st_t_hsg_connect_ind;

typedef struct
{
    t_bdaddr bt_addr;
    kal_uint16   connection_id;    
} st_t_hsg_disconnect_ind;

typedef struct
{
	kal_uint8  result;
	kal_uint8  gain;
	kal_uint16 connection_id;
} st_t_hsp_set_volume_cnf;

typedef struct
{
    kal_uint8 	        gain;
    kal_uint16          connection_id;
} st_t_hsg_gain_ind;

void rdabt_hsg_activate_cnf_cb(void * pArgs);
void rdabt_hsg_deactivate_cnf_cb(void * pArgs);
void rdabt_hsg_connect_cnf_cb(void * pArgs);
void rdabt_hsg_connect_ind_cb(void * pArgs);
void rdabt_hsg_disconnect_cnf_cb(void * pArgs);
void rdabt_hsg_disconnect_ind_cb(void * pArgs);
void rdabt_hsg_set_hs_volume_cnf_cb(void * pArgs);
void rdabt_hsg_disconnect_from_hs_ind_cb(void * pArgs);
void rdabt_hsg_bldn_from_hs_ind_cb(void * pArgs);
void rdabt_hsg_reject_incoming_call_cnf_cb(void * pArgs);
void rdabt_hsg_ring_cnf_cb(void * pArgs);
void rdabt_hsg_speaker_gain_ind_cb(void * pArgs);
void rdabt_hsg_mic_gain_ind_cb(void * pArgs);
void rdabt_hsg_msg_hdler(ilm_struct *message);
void rdabt_hsp_int_msg_hdlr( kal_uint16 mi,void *mv );




#endif
