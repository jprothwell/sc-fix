
#ifndef _RDABT_CALLBACK_H_
#define _RDABT_CALLBACK_H_

void rdabt_write_scanenable_cb(int status);

void rdabt_write_localaddr_cb(int status);

void bt_write_authentication_mode_cb(int status);

void bt_discovery_cnf_cb(u_int8 status ,u_int8 total_num); 

void bt_discovery_result_ind_cb(t_MGR_DeviceEntry *newDevice, int status);

void bt_discovery_cancel_cb(u_int8 num,u_int8 result);

void rdabt_InputPin_MoniterTimer_cb(void  *addr );

void bt_pin_code_ind_cb(t_bdaddr address );
void rdabt_being_bonding_cb(t_bdaddr address, int status);
void rdabt_GetRemoteName_cb(t_bdaddr address, int status);

void rdabt_reply_rsp_cb(t_bdaddr address, int status);

#endif
