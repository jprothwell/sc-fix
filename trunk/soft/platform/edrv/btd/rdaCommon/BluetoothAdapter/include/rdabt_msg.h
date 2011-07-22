#ifndef _RDABT_MSG_H
#define _RDABT_MSG_H

#define RDA_SEND_COSMSG(MOD_DEST,P_COS_EVENT) \
	sxr_Send(P_COS_EVENT,((TASK_HANDLE *)GetMmiTaskHandle(MOD_DEST))->nMailBoxId, SXR_SEND_EVT)

#define rda_msg_send_ext_queue(ilm_ptr) \
do \
{ \
	COS_EVENT ev; \
	ev.nEventId = ilm_ptr->msg_id; \
	ev.nParam1 = (UINT32)ilm_ptr; \
	RDA_SEND_COSMSG(ilm_ptr->dest_mod_id, &ev); \
} while (0)

#define RDA_SEND_COSMSG_HEAD(MOD_DEST,P_COS_EVENT) \
	sxr_Send(P_COS_EVENT,((TASK_HANDLE *)GetMmiTaskHandle(MOD_DEST))->nMailBoxId, SXR_SEND_EVT|SXR_QUEUE_FIRST)

#define rda_msg_send_ext_queue_to_head(ilm_ptr) \
do \
{ \
	COS_EVENT ev; \
	ev.nEventId = ilm_ptr->msg_id; \
	ev.nParam1 = (UINT32)ilm_ptr; \
	RDA_SEND_COSMSG_HEAD(ilm_ptr->dest_mod_id, &ev); \
} while (0)

#define rda_msg_send_int_queue(ilm_ptr) rda_msg_send_ext_queue((ilm_ptr))

#define EXTERNAL_TASK_MASK      0x4000
extern void rdabt_send_msg_up(kal_uint16 dest, kal_uint32 msg_id, void *local_param_ptr, void *peer_buf_ptr);
//extern void rdabt_send_msg_to_head(kal_uint16 dest, kal_uint32 msg_id, void *local_param_ptr, void *peer_buf_ptr);
#endif
