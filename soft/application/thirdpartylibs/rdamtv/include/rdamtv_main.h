#ifndef __RDAMTV_MAIN_H__
#define __RDAMTV_MAIN_H__
#include "kal_release.h"		  /* Basic data type */
#include "stack_timer.h"

// rdamtv tast context
typedef struct
{
	kal_uint8 rdamtv_state;
	kal_eventgrpid rdamtv_event;	/* the event group of atv module */
	stack_timer_struct rdamtv_timer;
}rdamtv_context_struct;

typedef struct
{
	LOCAL_PARA_HDR 
	kal_uint16 channel_index;
}media_rdamtv_set_channel_req_struct;

typedef struct
{
	LOCAL_PARA_HDR 
	kal_uint16 volume_level;
}media_rdamtv_set_volume_req_struct;

typedef struct
{
	LOCAL_PARA_HDR
	kal_uint16 total_channels;	//返回的数据	
	kal_uint16 scan_chan[20];	//返回的数据	
}media_rdamtv_scan_channel_req_struct;

typedef struct
{
	LOCAL_PARA_HDR
	kal_uint16 total_channels;	//返回的数据	
	kal_uint16 scan_chan[20];	//返回的数据	
}media_rdamtv_scan_channel_rsp_struct;

typedef enum
{
	RDAMTV_EVT_NOWAIT = 0,
	RDAMTV_EVT_POWER_UP = 0x0001,
	RDAMTV_EVT_INIT = 0x0002,
	RDAMTV_EVT_SCAN_CHANNEL = 0x0004,
	RDAMTV_EVT_SET_CHANNEL = 0x0008,
	RDAMTV_EVT_SET_PARAM = 0x0010,	
	RDAMTV_EVT_GET_PARAM = 0x0020,
	RDAMTV_EVT_EXIT = 0x0040,
	RDAMTV_EVT_POWER_DOWN = 0x0080
} rdamtv_wait_event_enum;

#define RDAMTV_MEM_OVERHEAD          256

#define RDAMTV_WAIT_EVENT(evt_) do{								  \
		 kal_uint32 retrieved_events;							  \
		 kal_retrieve_eg_events(rdamtv_context_p->rdamtv_event, (evt_), \
		 KAL_OR_CONSUME, &retrieved_events, KAL_SUSPEND); }while(0)

#define RDAMTV_SET_EVENT(evt_) do{ 							       \
		  kal_set_eg_events(rdamtv_context_p->rdamtv_event, (evt_),KAL_OR);\
		  } while(0)


#define RDAMTV_IN_STATE(_s_)		(rdamtv_context_p->rdamtv_state == _s_)
#define RDAMTV_ENTER_STATE(_s_)     (rdamtv_context_p->rdamtv_state = _s_)


// rdamtv state
#define RDAMTV_MED_IDLE	0
#define RDAMTV_MED_INIT	1
#define RDAMTV_MED_PLAY	2
#define RDAMTV_MED_SCAN_CHAN	3
#define RDAMTV_MED_SET_CHAN	4

extern rdamtv_context_struct *rdamtv_context_p;

#endif
