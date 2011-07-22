#ifndef _RDA_BT_AVRCP_H_
#define _RDA_BT_AVRCP_H_

//#include "..\rdabt\user\bt.h"
#include "bt.h"
typedef struct
{
    kal_uint8   chnl_num;	
    kal_uint8   seq_id;
    kal_uint8   c_type;
    kal_uint8   subunit_type;
    kal_uint8   subunit_id;
    kal_uint16  data_len;
    kal_uint16  profile_id;
    kal_uint8   frame_data[20];
} t_avrcp_cmd_frame_ind;

typedef struct
{
    kal_uint8 chnl_num;
    kal_uint8 connect_id;
    t_bdaddr device_addr;
} t_avrcp_connect_ind;

typedef struct
{
    kal_uint8 chnl_num;
    kal_uint8 connect_id;
    kal_uint16 result;
    t_bdaddr device_addr;
}t_avrcp_connect_cnf;

typedef struct
{
    kal_uint8 chnl_num;
    kal_uint8 connect_id;
}t_avrcp_disconnect_ind;

typedef struct
{
    kal_uint8 chnl_num;
    kal_uint8 connect_id;    
    kal_uint16 result;
} t_avrcp_disconnect_cnf;

// activate
void rda_bt_avrcp_send_activate_req(kal_uint8 chnl_num, kal_uint8 local_role);

// deactivate
void rda_bt_avrcp_send_deactivate_req( kal_uint8 chnl_num);

// connect
void rda_bt_avrcp_send_connect_req( kal_uint8 chnl_num, t_bdaddr device_addr, kal_uint8 local_role );

// disconnect
void rda_bt_avrcp_send_disconnect_req( kal_uint8 chnl_num );

//command frame
void rda_bt_avrcp_send_cmd_frame_req( kal_uint8 chnl_num, kal_uint8 seq_id, kal_uint8 c_type, kal_uint8 subunit_type, kal_uint8 subunit_id, kal_uint16 data_len, kal_uint8* frame_data);
void rda_bt_avrcp_send_cmd_frame_ind_res(kal_uint8 chnl_num, kal_uint8 seq_id, kal_uint16 profile_id,kal_uint8 result, kal_uint8 subunit_type, kal_uint8 subunit_id, kal_uint16 data_len, kal_uint8* frame_data );


#endif /* #ifndef _RDA_BT_AVRCP_H_ */

