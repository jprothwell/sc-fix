#if SDP_CLIENT

#ifndef SDP_STATE_MACHINE_H
#define SDP_STATE_MACHINE_H

#include "host_config.h"

#include "sdp_stack_access.h"

#include "sdp_constants.h"
#include "sdp_ue.h"

struct prh_sdp_primitive_task;

typedef void (*prh_sdp_task_function)(struct prh_sdp_primitive_task* p_primitive, u_int32 length, u_int8* p_data, u_int8* pContinue);

typedef void (*prh_sdp_task_main)(struct prh_sdp_primitive_task* p_primitive, u_int8* pContinue);


struct prh_sdp_task
{
  u_int16 transaction_id;
  u_int8 expects;
  u_int16 state;

  prh_sdp_task_main execute;
  prh_sdp_task_function waiter;
};



struct prh_sdp_primitive_task
{
  u_int32 service_primitive_id;
  /*   u_int16 application_id; */
  t_pTimer timer_handle;

	u_int16 mtu;

  u_int32 length;
  u_int8* rx_buffer;
  u_int32 rx_size;
  u_int32 offset;
  u_int32 current_bd;
  u_int8 status;
  u_int8 keep_connection;
  u_int8 w4_l2cap_connection;
  u_int8 w4_name;
  u_int8 w4_Service_Search;
  u_int8 record_handles_left;
  u_int32* service_records;
  t_SDP_Addresses devices; 

  t_SDP_SearchPattern pattern; 
  t_SDP_AttributeIDs attributes;
  u_int8 retrieveName;
  t_classDevice cod;
  u_int32 iac;
  t_SDP_StopRule rule;  
  
  t_channelId cid;
  
  sdp_api_callback app_callback;
  t_SDP_Results* app_data;
    
  prh_sdp_task_function execute;
  
  struct prh_sdp_task sub_task_level_1;
  struct prh_sdp_task sub_task_level_2;
  
  struct prh_sdp_primitive_task* p_next;
};


void prh_sdp_add_primitive_task(struct prh_sdp_primitive_task *p_task);
void prh_sdp_free_primitive_task(struct prh_sdp_primitive_task *pTask);

#if SDP_SDAP
void prh_sdp_w4_connections(struct prh_sdp_primitive_task* pTask,u_int32 length, u_int8* pData, u_int8* pContinue);
void prh_sdp_find_services(struct prh_sdp_primitive_task* pTask,u_int8* pContinue);
#endif /* SDP_SDAP */

void prh_sdp_w4_service(struct prh_sdp_primitive_task* pTask,u_int32 length, u_int8* pData, u_int8* pContinue);

#if SDP_SDAP
void prh_sdp_search_attributes(struct prh_sdp_primitive_task* pTask,u_int8* pContinue);
#endif /* SDP_SDAP */

void prh_sdp_w4_attributes(struct prh_sdp_primitive_task* pTask,u_int32 length, u_int8* pData, u_int8* pContinue);

#if SDP_SDAP

void prh_sdp_w4_enumerate(struct prh_sdp_primitive_task *pTask,u_int32 length, u_int8* pData, u_int8* pContinue);

void prh_sdp_w4_browse_connections(struct prh_sdp_primitive_task* pTask,u_int32 length, u_int8* pData, u_int8* pContinue);

#endif /* SDP_SDAP */

void prh_sdp_find_services_attributes(struct prh_sdp_primitive_task* pTask,u_int8* pContinue);

void prh_sdp_w4_service_attributes(struct prh_sdp_primitive_task* pTask,u_int32 length, u_int8* pData, u_int8* pContinue);


#if SDP_GENERIC_BROWSE

void prh_sdp_generic_browse(struct prh_sdp_primitive_task* pTask,u_int32 length, u_int8* pData, u_int8* pContinue);

#endif /* SDP_GENERIC_BROWSE */


void prh_sdp_timer_handler(void *pTask);


void prh_sdp_finish_primitive_task(struct prh_sdp_primitive_task *pTask);


void prh_sdp_int_ss(struct prh_sdp_primitive_task* pTask,u_int32 length, u_int8* pData, u_int8* pContinue);
void prh_sdp_int_find_services(struct prh_sdp_primitive_task* pTask,u_int8* pContinue);

void prh_sdp_int_sa(struct prh_sdp_primitive_task* pTask,u_int32 length, u_int8* pData, u_int8* pContinue);
void prh_sdp_int_search_attributes(struct prh_sdp_primitive_task* pTask,u_int8* pContinue);


void prh_sdp_int_ssa(struct prh_sdp_primitive_task* pTask,u_int32 length, u_int8* pData, u_int8* pContinue);
void prh_sdp_int_find_service_attributes(struct prh_sdp_primitive_task* pTask,u_int8* pContinue);

void prh_sdp_int_store_results(struct prh_sdp_primitive_task* pTask, u_int8* pContinue, u_int16 status, u_int8 type);


#endif /* SDP_STATE_MACHINE_H */



#endif /* SDP_CLIENT */

