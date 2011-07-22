
#ifndef _RDABT_A2DP_H_
#define _RDABT_A2DP_H_

/*!
	@brief The capability to stream media. This is manditory for advance audio
	distribution profile.
*/
#define AVDTP_SERVICE_MEDIA_TRANSPORT		(1)		
/*!
	@brief The reporting capability. This is not currently supported.
*/
#define AVDTP_SERVICE_REPORTING				(2)		
/*!
	@brief The recovery capability. This is not currently supported.
*/
#define AVDTP_SERVICE_RECOVERY				(3)		
/*!
	@brief The content protection capability. This is not currently supported.
*/
#define AVDTP_SERVICE_CONTENT_PROTECTION	(4)		
/*!
	@brief The header compression capability. This is not currently supported.
*/
#define AVDTP_SERVICE_HEADER_COMPRESSION	(5)		
/*!
	@brief The multiplexing capability. This is not currently supported.
*/
#define AVDTP_SERVICE_MULTIPLEXING			(6)		
/*!
	@brief The codec capability for the Stream End Point.
*/
#define AVDTP_SERVICE_MEDIA_CODEC			(7)		

typedef struct
{
    kal_uint16 connect_id;
    t_bdaddr device_addr;
} t_a2dp_signal_connect_ind;

typedef struct
{
    u_int16 connect_id;
    u_int8 acp_seid;
} t_a2dp_capabilities_get_ind;

typedef struct
{
	u_int16 connect_id;
	u_int8 acp_seid;
	u_int8 int_seid;
	u_int8 stream_handle;
	bt_a2dp_audio_cap_struct audio_cap;
} t_a2dp_stream_config_ind;

typedef struct
{
	kal_uint16 connect_id;
	kal_uint16 result;
	kal_uint16 sep_num;
	bt_sep_info_struct sep_list[MAX_NUM_REMOTE_SEIDS];
} t_a2dp_sep_discover_cnf;


void rdabt_a2dp_signal_connect_cnf_cb(void * pArgs);
void rdabt_a2dp_signal_disconnect_cnf_cb(void * pArgs);
void av_rda_bt_a2dp_send_sep_discover_cnf_callback(void * pArgs);
void av_rda_bt_a2dp_send_sep_get_capabilities_cnf_callback(void * pArgs);
void av_rda_bt_a2dp_send_sep_set_config_cnf_callback(void * pArgs);
void av_rda_bt_a2dp_send_sep_open_cnf_callback(void * pArgs);
void av_rda_bt_a2dp_send_sep_start_cnf_callback(void * pArgs);
void av_rda_bt_a2dp_send_sep_pause_cnf_callback(void * pArgs);
void av_rda_bt_a2dp_send_sep_close_cnf_callback(void * pArgs);
void av_rda_bt_a2dp_send_sep_abort_cnf_callback(void * pArgs);

void rdabt_a2dp_activate_cb(void * pArgs);
void rdabt_a2dp_deactivate_cb(void * pArgs);
void rdabt_a2dp_signal_connect_ind_cb(void * pArgs);
void rdabt_a2dp_signal_disconnect_ind_cb(void * pArgs);
void av_rda_bt_a2dp_send_sep_discover_ind_callback(void * pArgs);
void av_rda_bt_a2dp_send_sep_get_capabilities_ind_callback(void * pArgs);
void av_rda_bt_a2dp_send_sep_set_config_ind_callback(void * pArgs);
void av_rda_bt_a2dp_send_sep_open_ind_callback(void * pArgs);
void av_rda_bt_a2dp_send_sep_start_ind_callback(void * pArgs);
void av_rda_bt_a2dp_send_sep_pause_ind_callback(void * pArgs);
void av_rda_bt_a2dp_send_sep_close_ind_callback(void * pArgs);
void av_rda_bt_a2dp_send_sep_abort_ind_callback(void * pArgs);
void av_rda_bt_a2dp_send_sep_set_reconfig_cnf_callback(void *pArgs);

void rdabt_a2dp_msg_hdlr(ilm_struct *message);
void rdabt_a2dp_int_msg_hdlr( kal_uint16 mi,void *mv );

#endif






