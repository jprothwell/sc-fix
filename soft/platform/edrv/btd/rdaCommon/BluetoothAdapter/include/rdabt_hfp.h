
#ifndef _RDABT_HFP_H_
#define _RDABT_HFP_H_

typedef struct
{
    kal_uint8         result;
    t_bdaddr       bt_addr;
    kal_uint16         connection_id;
} st_t_hfp_connect_cnf;

typedef struct
{
    t_bdaddr bt_addr;
    kal_uint16   connection_id;    
} st_t_hfg_connect_ind;

typedef struct
{
    kal_uint8   result;
    kal_uint16 connection_id;
} st_t_hfg_sco_connect_cnf;

typedef struct
{
	kal_uint8  result;
	kal_uint8  gain;
	kal_uint16 connection_id;
} st_t_hfp_set_volume_cnf;

typedef struct
{
    u_int8    gain;
    u_int16  connection_id;
} st_t_hfg_gain_ind_struct;

void rdabt_hfg_activate_cnf_cb(void * pArgs);
void rdabt_hfg_deactivate_cnf_cb(void * pArgs);
void rdabt_hfg_connect_cnf_cb(void * pArgs);
void rdabt_hfg_connect_ind_cb(void * pArgs);
void rdabt_hfg_disconnect_cnf_cb(void * pArgs);
void rdabt_hfg_disconnect_ind_cb(void * pArgs);

void rdabt_hfg_set_sco_connect_cnf_cb(void * pArgs);
void rdabt_hfg_set_sco_disconnect_cnf_cb(void * pArgs);
void rdabt_hfg_set_HF_volume_cnf_cb(void * pArgs);
void rdabt_hfg_disconnect_from_HF_ind_cb(void * pArgs);
void rdabt_hfg_bldn_from_hf_ind_cb(void * pArgs);
void rdabt_hfg_reject_incoming_call_cnf_cb(void * pArgs);
void rdabt_hfg_ring_cnf_cb(void * pArgs);
void rdabt_hfg_speaker_gain_ind_cb(void * pArgs);
void rdabt_hfg_mic_gain_ind_cb(void * pArgs);
void rdabt_hfg_msg_hdler(ilm_struct *message);
void rdabt_hfp_int_msg_hdlr( kal_uint16 mi,void *mv );

#endif
