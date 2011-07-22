/****************************************************************************
Copyright (C) RDA Ltd. 2008

FILE NAME
    avrcp.h
    
DESCRIPTION 

    Header file for the Audio/Visual Remote Control Profile library.  This
    profile library implements the AVRCP using the services of the AVCTP
    library which is hidden from the Client application by this library
	
	The library exposes a functional downstream API and an upstream message 
	based API.
	
	 CLIENT APPLICATION
	  |	     |
	  |	AVRCP Library
	  |	     |
	  |      |
	 CONNECTION Library
	         |
	     BLUESTACK
    
@file	       avrcp.h
@brief	Interface to the Audio Video Remote Control Profile library.

		This profile library implements the AVRCP.  This library permits one
		device known as the controller (CT) to send dedicated user actions to
		another device known as the target (TG).
		
		Note: This library does not handle audio streaming, this is implemented
		in the GAVDP library.
	
		The library exposes a functional downstream API and an upstream message
		based API.
	
****************************************************************************/



#ifndef AVRCP_H_
#define AVRCP_H_

#if COMBINED_HOST==1
#include "sys_hal_types.h"
#include "sys_types.h"
#endif
#include "host_types.h"

/* L2CAP PSM */
#define AVCTP_PSM						(0x17)

#define AVCTP0_TRANSACTION_SHIFT	4
#define AVCTP0_PACKET_TYPE_MASK		0x0c
#define AVCTP0_PACKET_TYPE_SINGLE	0x00
#define AVCTP0_PACKET_TYPE_START	0x04
#define AVCTP0_PACKET_TYPE_CONTINUE	0x08
#define AVCTP0_PACKET_TYPE_END		0x0C
#define AVCTP0_CR_MASK				0x02
#define AVCTP0_CR_COMMAND			0x00
#define AVCTP0_CR_RESPONSE			0x02
#define AVCTP0_IPID					0x01
#define AVCTP1_PROFILE_AVRCP_HIGH	0x11
#define AVCTP2_PROFILE_AVRCP_LOW	0x0e

#define AVRCP0_CTYPE_CONTROL			0x00
#define AVRCP0_CTYPE_STATUS				0x01
#define AVRCP1_SUBUNIT_TYPE_MASK		0xf8
#define AVRCP1_SUBUNIT_TYPE_SHIFT		3
#define AVRCP1_SUBUNIT_ID_MASK			0x07
#define AVRCP1_UNIT						0xff

#define AVRCP2_PASSTHROUGH				0x7C
#define AVRCP2_UNITINFO					0x30
#define AVRCP2_SUBUNITINFO				0x31
#define AVRCP2_VENDORDEPENDENT			0x00
#define AVRCP3_PASSTHROUGH_OP_MASK		0x7f
#define AVRCP3_PASSTHROUGH_STATE_MASK	0x80
#define AVRCP3_SUBUNITINFO_PAGE_SHIFT	4
#define AVRCP3_SUBUNITINFO_EXTEND_MASK	7
#define AVRCP4_UNITINFO_UNIT_TYPE_SHIFT	3
#define AVRCP4_UNITINFO_UNIT_MASK		7

#define AVRCP_TOTAL_HEADER_SIZE			6



struct __AVRCP;
/*!
	@brief The Audio Video Remote Control Profile structure.
*/
typedef struct __AVRCP AVRCP;


/*!
	@brief The page data length.
*/
#define PAGE_DATA_LENGTH	(4)


typedef enum
{
    AVRCP_MESSAGE_VALID,
    AVRCP_MESSAGE_INVALID
}t_AVRCP_MESSAGE_VALID;

typedef enum
{
    AVRCP_TYPE_COMMAND,
    AVRCP_TYPE_RESPONSE
}t_AVRCP_MESSAGE_TYPE;

/*! 
	@brief AVRCP responses 
*/
typedef enum
{
	/*! The request is not implemented */
	AVCTP_RESPONSE_NOT_IMPLEMENTED=0X08,
	/*! The request has been accepted */
	AVCTP_RESPONSE_ACCEPTED = 0x09,			
	/*! The request has been rejected */
	AVCTP_RESPONSE_REJECTED = 0x0a,			
	/*! The target is in a state of transition */
	AVCTP_RESPONSE_IN_TRANSTION = 0x0b,	
	/*! A stable response */
	AVCTP_RESPONSE_STABLE = 0x0c,			
	/*! The target devices state has changed */
	AVCTP_RESPONSE_CHANGED = 0x0d,			
	/*! The response is an interim response */
	AVCTP_RESPONSE_INTERIM = 0x0f,			
	/*! The specified profile is not acceptable */
	AVCTP_RESPONSE_BAD_PROFILE				
} t_AVRCP_RESPONSE_TYPE;


/*!
	@brief AVRCP status codes 
*/
typedef enum
{
	/*! Operation was successful. */
	AVRCP_SUCCESS = (0),			
	/*! Operation failed. */
	AVRCP_FAI,						
	/*! Not enough resources. */
	AVRCP_NO_RESOURCE,				
	/*! Request is not supported in the current state. */
	AVRCP_BAD_STATE,				
	/*! Operation timed out before completion. */
	AVRCP_TIIMEOUT,					
	/*! Device specified is not connected. */
	AVRCP_DEVICE_NOT_CONNECTED,		
	/*! Operation is already in progress. */
	AVRCP_BUSY,						
	/*! Requested operation is not supported. */
	AVRCP_UNSUPPORTED,				
	/*! Sink supplied was invalid. */
	AVRCP_INVALID_SINK,
	/*! Link loss occurred. */
	AVRCP_LINK_LOSS
} t_AVRCP_STATUS_CODE;


/*! 
	@brief Operation ID, used to identify operation. See table 9.21 AV/C Panel
	Subunit spec. 1.1 #
*/
typedef enum
{
	OPID_SELECT				= (0x0),
	OPID_UP,
	OPID_DOWN,
	OPID_LEFT,
	OPID_RIGHT,
	OPID_RIGHT_UP,
	IPID_RIGHT_DOWN,
	OPID_LEFT_UP,
	OPID_LEFT_DOWN,
	OPID_ROOT_MENU,
	OPID_SETUP_MENU,
	OPID_CONTENTS_MENU,
	OPID_FAVOURITE_MENU,
	OPID_EXIT,
	/* 0x0e to 0x1f Reserved */
	OPID_0					= (0x20),
	OPID_1,
	OPID_2,
	OPID_3,
	OPID_4,
	OPID_5,
	OPID_6,
	OPID_7,
	OPID_8,
	OPID_9,
	OPID_DOT,
	OPID_ENTER,
	OPID_CLEAR,
	/* 0x2d - 0x2f Reserved */
	OPID_CHANNEL_UP			= (0x30),
	OPID_CHANNEL_DOWN,
	OPID_SOUND_SELECT,
	OPID_INPUT_SELECT,
	OPID_DISPLAY_INFORMATION,
	OPID_HELP,
	OPID_PAGE_UP,
	OPID_PAGE_DOWN,
	/* 0x39 - 0x3f Reserved */
	OPID_POWER				= (0x40),
	OPID__VOLUME_UP,
	OPID_VOLUME_DOWN,
	OPID__MUTE,
	OPID_PLAY,
	OPID_STOP,
	OPID_PAUSE,
	OPID_RECORD,
	OPID_REWIND,
	OPID_FAST_FORWARD,
	OPID__EJECT,
	OPID_FORWARD,
	OPID_BACKWARD,
	/* 0x4d - 0x4f Reserved */
	OPID_ANGLE				= (0x50),
	OPID_SUBPICTURE,
	/* 0x52 - 0x70 Reserved */
	OPID_F1					= (0x71),
	OPID_F2,
	OPID_F3,
	OPID_F4,
	OPID_F5,
	OPID_VENDOR_UNIQUE		= (0x7e)
	/* Ox7f Reserved */
} t_AVC_OPERATION_ID; 


/*!
	@brief Subunit types 
*/
typedef enum
{
	SUBUNIT_MONITOR			= (0x0),
	SUBUNIT_AUDIO,
	SUBUNIT_PRINTER,
	SUBUNIT_DISC,
	SUBUNIT_TAPE_RECORDER_PLAYER,
	SUBUNIT_TUNER,
	SUBUNIT_CA,
	SUBUNIT_CAMERA,
	SUBUNIT_RESERVED,
	SUBUNIT_PANEL,
	SUBUNIT_BULLETIN_BOARD,
	SUBUNIT_CAMERA_STORAGE,
	/* 0x0c - 0x1b Reserved */
	SUBUNIT_VENDOR_UNIQUE	= (0x1c),
	SUBUNIT_RESERVED_FOR_ALL,
	SUBUNIT_EXTENDED,
	SUBUNIT_UNIT
} t_AVC_SUBUNIT_TYPE;


/*!
	@brief AVRCP device type
       The avrcp library can be configured to be either a target or a controller  device.
*/
typedef enum
{
	AVRCP_DEVICE_NONE,
	AVRCP_TARGET,
	AVRCP_CONTROLLER,
	AVRCP_TARGET_AND_CONTROLLER
} t_AVRCP_DEVICE_TYPE;


typedef enum 
{
	AVRCP_INITIALISING,
	AVRCP_READY,
	AVRCP_CONNECTING,
	AVRCP_CONNECTED
} t_AVRCP_STATE;


typedef enum
{
	AVRCP_NONE,
	AVRCP_PASSTHROUGH,
	AVRCP_UNIT_INFO,
	AVRCP_SUBUNIT_INFO,
	AVRCP_VENDOR
} t_AVRCP_PENDING;


typedef enum
{
	AVRCP_FRAG_NONE,
	AVRCP_FRAG_START,
	AVRCP_FRAG_CONTINUE,
	AVRCP_FRAG_END
} t_AVRCP_FRAGMENT;

typedef struct TaskData *Task;
typedef u_int16 MessageId;
typedef const void *Message;

typedef struct TaskData 
{ 
	void (*handler)(Task, MessageId, Message); 
} TaskData;

struct __AVRCP
{
    t_AVRCP_STATE	      state;
    t_AVRCP_PENDING	      pending;
    u_int8                         block_received_data;
    u_int8			     watchdog;
    t_channelId		     cid;
    u_int8			     transaction_label;
    t_AVRCP_FRAGMENT   fragmented;
    u_int16 			     l2cap_mtu;
    u_int16			      priority;
    t_AVRCP_DEVICE_TYPE   device_type;
    t_bdaddr device_addr;
    u_int8			        *identifier;   
#ifdef COOLSAND
    u_int8 chnl_num;
    u_int8 local_role;
#endif
};

#ifdef COOLSAND
#if 0
struct avrcp_signal_cbs 
{
    void (*avrcpconnectcfm)( void *pArgs);
    void (*avrcpconnectind)( void *pArgs);
    void (*avrcpdisconnectcfm)( void *pArgs);
    void (*avrcpdisconnectind)( void *pArgs);
};

typedef struct avrcp_signal_cbs avrcp_signal_callbacks;

struct avrcp_cmd_cbs 
{
    void (*avrcpcmdframecnf)( void *pArgs);
    void (*avrcpcmdframeind)( void *pArgs);
};

typedef struct avrcp_cmd_cbs avrcp_cmd_callbacks;
#endif

typedef struct
{
    u_int8   chnl_num;	
    u_int8   seq_id;
    u_int8   c_type;
    u_int8   subunit_type;
    u_int8   subunit_id;
    u_int16  data_len;
    u_int16  profile_id;
    u_int8   frame_data[20];
} t_avrcp_cmd_frame_ind;

typedef struct
{
    u_int8 chnl_num;
    u_int8 connect_id;
    t_bdaddr device_addr;
} t_avrcp_connect_ind;

typedef struct
{
    u_int8 chnl_num;
    u_int8 connect_id;
    u_int16 result;
    t_bdaddr device_addr;
}t_avrcp_connect_cnf;

typedef struct
{
    u_int8 chnl_num;
    u_int8 connect_id;
}t_avrcp_disconnect_ind;

typedef struct
{
    u_int8 chnl_num;
    u_int8 connect_id;    
    u_int16 result;
} t_avrcp_disconnect_cnf;

#endif

#endif //AVRCP_H_
