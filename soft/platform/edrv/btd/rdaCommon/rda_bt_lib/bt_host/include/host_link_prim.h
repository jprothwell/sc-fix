
#ifndef _BT_HSOT_LINK_PRIM_H_

#define _BT_HSOT_LINK_PRIM_H_




typedef struct
{
    u_int16  type;
    u_int8	result;
    u_int8	total_number;    
}LINK_DISCOVERY_CNF_ST;

typedef struct
{
    u_int16  type;
    u_int8	result;
    u_int8	reserved;
    t_MGR_DeviceEntry  *device;
}LINK_DISCOVERY_RESULT_ST;


typedef struct
{
    u_int16  type;
    u_int8	result;
    u_int8	number;    
}LINK_DISCOVERY_CANCEL_ST;

typedef struct
{
    u_int16   type;
    u_int8	result;
}LINK_WRITE_SCANENABLE_MODE_ST;

typedef struct
{
    u_int16   type;
    u_int8	result;
}LINK_WRITE_LOCALADDR_ST;

typedef struct
{
    u_int16   type;
    u_int8	result;
}LINK_WRITE_AUTHENTICATION_MODE_ST;

typedef struct
{
    u_int16  type;
    t_bdaddr     bd_addr;
    u_int8	        name_len;
    u_int8	        name[MGR_MAX_LOCAL_NAME_LEN];
}LINK_PIN_CODE_IND_ST;

typedef struct
{
    void (*bt_write_scanenable_mode_cb) (int result);
    void (*bt_write_local_addr_cb)(int result);
    void (*bt_write_authentication_mode_cb)(int result);
    void (*bt_discovery_cnf_cb)(u_int8 result,u_int8 total_number);
    void (*bt_discovery_result_cb)(struct st_t_BT_DeviceEntry *newDevice, int status);
    void (*bt_discovery_cancel_cb)(u_int8 num,u_int8 result);
    void (*bt_pin_code_ind_cb)(t_bdaddr address);
	

}bt_host_cb_tbl;


#endif
