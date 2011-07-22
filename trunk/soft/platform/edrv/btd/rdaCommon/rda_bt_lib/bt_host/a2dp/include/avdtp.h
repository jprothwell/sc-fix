/****************************************************************************
Copyright (C) RDA Ltd. 2008

FILE NAME
    avdTp.h
    
DESCRIPTION 

    Header file for the Audio/Visual Distribution Transport Protocol
    Specfication.  This library is used by a number of Bluetooth profiles.  The
    Audio/Video Distribution Transport Protocol Specification (hereafter
    referred to as AVDTP) is used to transport Audio/Video streams over an
    L2CAP connection.

    Commands and responses for executing the stream setup procedure are defined
    as Bluetooth specific.  AVDTP defines the binary transactions between
    Bluetooth devices for stream setup and media streaming for audio and video
    using L2CAP.

    A/V streaming and stream setup signalling are transported via L2CAP
    packets.  A dedicated Protocol/Service Multiplexer (PSM) value is used to
    identify L2CAP packets that are intended for AVDTP.
    
    
	The library exposes a functional downstream API and an upstream message
	based API.
	
	  CLIENT APPLICATION
	  |	     |
	  |	A2DP Library
	  |	     |
	  |	AVDTP Library
	  |      |
	 L2CAP Library
	         |
	     BLUE CORE
	
*/
/*!
@file	    AVDTP.h

@brief     Interface to the Generic Audio Visual Distribution Profile library.

		Header file for the Generic Audio Visual Distribution Profile
		specification.
		  
		This library is used by a number of Bluetooth profiles and implements
		Audio/Visual Distribution Transport Protocol Specification. The
		Audio/Video Distribution Transport Protocol Specification (hereafter
		referred to as AVDTP) is used to transport Audio/Video streams over an
		L2CAP connection.

		Commands and responses for executing the stream setup procedure are
		defined as Bluetooth specific.  AVDTP defines the binary transactions
		between Bluetooth devices for stream setup and media streaming for
		audio and video using L2CAP.

		A/V streaming and stream setup signalling are transported via L2CAP
		packets.  A dedicated Protocol/Service Multiplexer (PSM) value is used
		to identify L2CAP packets that are intended for AVDTP.
    
    
		The library exposes a functional downstream API and an upstream message
		based API.
*/

#ifndef AVDTP_H_
#define AVDTP_H_



#if COMBINED_HOST==1
#include "sys_hal_types.h"
#include "sys_types.h"
#endif

#include "host_types.h"

struct __SINK;

/*! The Sink type. */
typedef struct __SINK *Sink;

#if COMBINED_HOST==0
#include "cs_types.h"
//typedef unsigned char boolean;
#endif

/* L2CAP PSM */
#define AVDTP_PSM						(0x19)

/*! 
	@name Service Categories.

	These are service categories to be used in service capabilities of a Stream
	End Point.
	
*/
/*! \{ */

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
/*! \} */

/*! 
	@name Service information.

	Used to fill out the fields in a media codec capabilities structure.
*/
/*! \{ */

/*!
	@brief Defines the codec type as audio.
*/
#define AVDTP_MEDIA_TYPE_AUDIO				(0)		
/*!
	@brief Defines the codec type as video.
*/
#define AVDTP_MEDIA_TYPE_VIDEO				(1)		
/*!
	@brief Defines the codec type as multimedia.
*/
#define AVDTP_MEDIA_TYPE_MULTIMEDIA			(2)		
/*!
	@brief Defines the codec as SBC. Manditory to support for A2DP.
*/
#define AVDTP_MEDIA_CODEC_SBC				(0)		
/*!
	@brief Defines the codec as MPEG1/2. Optional to support for A2DP.
*/
#define AVDTP_MEDIA_CODEC_MPEG1_2_AUDIO		(1)		
/*!
	@brief Optional to support.
*/
#define AVDTP_MEDIA_CODEC_MPEG2_4_AAC		(2)		
/*!
	@brief Optional to support.
*/
#define AVDTP_MEDIA_CODEC_ATRAC				(4)		
/*!
	@brief Not supported by a2dp.
*/
#define AVDTP_MEDIA_CODEC_NONA2DP			(0xff)	

/*! \} */

/*
	Maximum number words needed to contain a
	bit flag for all possible SEID values
*/	
#define NUM_SEID_PACKED_WORDS			(4)
#define MAX_NUM_REMOTE_SEIDS                (4)


struct __AVDTP;
/*!
	@brief The Generic Audio Visual Distribution Protocol structure. 
*/
typedef struct __AVDTP AVDTP;

/*!
	@brief The AVDTP  type.
*/
typedef enum
{
	AVDTP_SIGNAL    = (0x00),	     /*!<  signal */
	AVDTP_MEDIA			     /*!< meida */
} t_AVDTP_TYPE;


/*!
	@brief The AVDTP packet type.
*/
typedef enum
{
	AVDTP_SINGLE_PACKETt      = (0x00),	/*!< A single packet.*/
	AVDTP_START_PACKET,					/*!< A start packet.*/
	AVDTP_CONTINUE_PACKET,				/*!< A continue packet.*/
	AVDTP_END_PACKET					/*!< An end packet.*/
} t_AVDTP_PACKET_TYPE;

/*!
	@brief The AVDTP message type.
*/
typedef enum
{
	AVDTP_COMMAND            = (0x00),	/*!< A command.*/
	AVDTP_RFD,							/*!< Reserved for future definition. */
	AVDTP_RESPONSE_ACCEPT,				/*!< An accept response.*/
	AVDTP_RESPONSE_REJECT				/*!< A reject response.*/
} t_AVDTP_MESSAGE_TYPE;


/*!
	@brief AVDTP Stream Endpoint type.
*/
typedef enum
{	
	AVDTP_SOURCE,				/*!< Source. */
	AVDTP_SINK					/*!< Sink.  */
} t_AVDTP_SEP_TYPE;

/*!
	@brief Status codes. 
*/
typedef enum
{
	/*! Operation was successful. */
	avdtp_success = (0),			
	/*! The operation failed. */
	avdtp_fail,						
	/*! Not enough resources. */
	avdtp_no_resource,				
	/*! Request is not supported in the current state. */
	avdtp_incorrect_state,			
	/*! Operation timed out before completion. */
	avdtp_timeout,					
	/*! Service capabilities passed to library are bad or incomplete. */
	avdtp_service_caps_error,		
	/*! Local and remote service capabilities are incompatible. */
	avdtp_service_caps_incompatible,
	/*! Remote device understood but rejected the operation. */
	avdtp_rejected_by_remote_device,
	/*! Remote device does not support the requested operation. */
	avdtp_unsupported_feature,		
	/*! SEID pass to library is unknown. */
	avdtp_unknown_seid,				
	/*! Remote device closed the stream. */
	avdtp_closed_by_remote_device,	
	/*! Local or remote abort due to signalling breakdown. */
	avdtp_aborted,				    
	/*! Device specified is not connected. */
	avdtp_device_not_connected,		
	/*! The operation is already in progress. */
	avdtp_busy,
	/*! Connection disconnected due to link loss. */
	avdtp_disconnect_link_loss
} avdtp_status_code;


/*!
	@brief  Unique Stream End Point Identifier.
*/
typedef u_int16	seid_type;

/*!
	@brief Stream EndPoint Media type. 
*/
typedef enum
{
	SEP_MEDIA_TYPE_AUDIO,		/*!< Audio.*/
	SEP_MEDIA_TYPE_VIDEO,			/*!< Video.*/
	SEP_MEDIA_TYPE_MULTIMEDIA	/*!< Multimedia.*/
} t_SEP_MEDIA_TYPE;

/*!
	@brief Stream End Point role. 
*/
typedef enum
{
	SEP_SOURCE,				/*!< Stream Endpoint is the Source.*/
	SEP_SINK				/*!< Stream Endpoint is the Sink.*/
} t_SEP_ROLE; 


/* Device states */
typedef enum 
{
	AVDTPINITIALISING,
	AVDTPREADY
} t_AVDTP_STATE;

typedef enum
{
	av_connection_state_empty,
	av_connection_state_waiting_for_discovery_and_incoming,
	av_connection_state_waiting_for_incoming,
	av_connection_state_connecting,
	av_connection_state_connected,
	av_connection_state_disconnecting,
	av_connection_state_disconnecting_with_reconnect
} av_connection_state;

/* Stream Endpoint (SEP) states based on AVDTP Figure 6.4 */
typedef enum
{
	AVDTP_SEP_STATE_EMPTY,
	AVDTP_SEP_STATE_IDLE,
	AVDTP_SEP_STATE_WAITING_FOR_SIGNAL,
	AVDTP_SEP_STATE_DISCOVERING,
	AVDTP_SEP_STATE_READING_CAPS,
	AVDTP_SEP_STATE_PROCESSING_CAPS,
	AVDTP_SEP_STATE_CONFIGURING,	
	AVDTP_SEP_STATE_CONFIGURED,
	AVDTP_SEP_STATE_LOCAL_OPENING,
	AVDTP_SEP_STATE_REMOTE_OPENING,
	AVDTP_SEP_STATE_OPEN,
	AVDTP_SEP_STATE_STREAMING,
	AVDTP_SEP_STATE_LOCAL_SUSPENDING,
	AVDTP_SEP_STATE_LOCAL_CLOSING, 
	AVDTP_SEP_STATE_REMOTE_CLOSING,
	AVDTP_SEP_STATE_RECONFIG_READING_CAPS,
	AVDTP_SEP_STATE_RECONFIG_PROCESSING_CAPS,
	AVDTP_SEP_STATE_RECONFIGURING,
	AVDTP_SEP_STATE_RECONFIGURE_FAILURE,
	AVDTP_SEP_STATE_LOCAL_ABORTING,
	AVDTP_SEP_STATE_REMOTE_ABORTING
}t_SEP_STATE;

/* Signalling header message type */
enum
{
	AVDTP_MESSAGE_TYPE_COMMAND = (0x0),
	AVDTP_MESSAGE_TYPE_ACCEPT = (0x02),
	AVDTP_MESSAGE_TYPE_REJECT = (0x03)
};

/* Signalling header packet type */
enum
{		
	AVDTP_PACKET_TYPE_SINGLE = (0x0),
	AVDTP_PACKET_TYPE_START = (0x01),
	AVDTP_PACKET_TYPE_CONTINUE = (0x02),
	AVDTP_PACKET_TYPE_END = (0x03)
};

/* AVDTP signals */
typedef enum
{	
	AVDTP_NULL = (0x00),
	AVDTP_DISCOVER,
	AVDTP_GET_CAPABILITIES,
	AVDTP_SET_CONFIGURATION,
	AVDTP_GET_CONFIGURATION,
	AVDTP_RECONFIGURE,
	AVDTP_OPEN,
	AVDTP_START,
	AVDTP_CLOSE,
	AVDTP_SUSPEND,
	AVDTP_ABORT,
	ABDTP_SECURITY_CONTROL
} t_AVDTP_SIGNAL_ID;



/* State for each Signal connection and hence each connected remote device */
typedef struct av_connection
{
	av_connection_state		state;
	t_bdaddr					bd_addr;
	u_int16					flush_timeout;
	//Sink					signalling_sink;
	u_int16                  sink_mtu;	
	//u_int8                   *reassembled_packet;
	//u_int16                  reassembled_packet_length;
	struct av_connection	*next;
} av_connection;


/* Stream endpoint state */
typedef struct sep
{
    t_SEP_STATE		state;

    u_int8                   sep_count;
    u_int8                   in_use;
    u_int8			seid;
    u_int8			remote_seid;
    u_int16			discovered_seids[NUM_SEID_PACKED_WORDS];	

    t_SEP_STATE			preabort_state;
    u_int16			flush_timeout;
    u_int8			resource_id;
    t_SEP_MEDIA_TYPE	media_type;
    t_SEP_ROLE 		role;
    u_int8			pending_transaction_label;

    u_int16			media_sink_mtu;

    const u_int8		local_service_caps[10];  //caps size is 10 bytes.
    u_int16			local_service_caps_size;
    u_int8			configured_service_caps[10];
    u_int16			configured_service_caps_size;	

	//struct sep		*next;
}sep;


struct __AVDTP
{
    //TaskData				task;
    //Task					clientTask; /* Client task id, so we know where to route responses */
    t_AVDTP_STATE	    state;
    u_int8			    transaction_label;
    u_int16		    priority;
    u_int8                   active_seid;
    //u_int8                   sep_count;

    sep   			    sep_list_head[2];
    //av_connection		*av_connection_list_head;
    u_int16                 signal_cid;
    u_int16                 media_cid;
    t_bdaddr               bd_addr;

    boolean                signalling_indications_enabled;
};

/*!
	@brief Error codes.
*/
typedef enum
{
	avdtp_bad_header_format             = (0x01),	/*!< The request packet header format error that is not specified above ERROR_CODE. */
	avdtp_bad_length                    = (0x11),	/*!< The request packet length is not match the assumed length. */
	avdtp_bad_acp_seid                  = (0x12),	/*!< The requested command indicates an invalid ACP SEID (not addressable). */
	avdtp_sep_in_use                    = (0x13),	/*!< The SEP is in use. */
	avdtp_sep_not_in_use                = (0x14),	/*!< The SEP is not in use. */
	avdtp_bad_serv_category             = (0x17),	/*!< The value of Service Category in the request packet is not defined in AVDTP. */
	avdtp_bad_payload_format            = (0x18),	/*!< The requested command has an incorrect payload format (Format errors not specified in this ERROR_CODE). */
	avdtp_not_supported_command         = (0x19),	/*!< The requested command is not supported by the device. */
	avdtp_invalid_capabilities          = (0x1a),	/*!< The reconfigure command is an attempt to reconfigure a transport service capabilities of the SEP. Reconfigure is only permitted for application service capabilities. */
	avdtp_bad_recovery_type             = (0x22),	/*!< The requested Recovery Type is not defined in AVDTP. */
	avdtp_bad_media_transport_format    = (0x23),	/*!< The format of Media Transport Capability is not correct. */
	avdtp_bad_recovery_format           = (0x25),	/*!< The format of Recovery Service Capability is not correct. */
	avdtp_bad_rohc_format               = (0x26),	/*!< The format of Header Compression Service. */
	avdtp_bad_cp_format                 = (0x27),	/*!< The format of Content Protection Service Capability is not correct. */
	avdtp_bad_multiplexing_format       = (0x28),	/*!< The format of Multiplexing Service Capability is not correct. */
	avdtp_unsupported_configuration     = (0x29),	/*!< Configuration not supported. */
	avdtp_bad_state                     = (0x31)	/*!< Indicates that the ACP state machine is in an invalid state in order to process the signal.*/
} t_AVDTP_ERROR_CODE;

#ifdef COOLSAND
typedef struct
{
    u_int32 (*GetPayload)(u_int8 *, u_int32, u_int32 *);
    void (*GetPayloadDone)(void);
    void (*QueryPayloadSize)(u_int32 *, u_int32 *);
    u_int32 (*AdjustBitRateFromQos)(u_int8); // return adjusted bit rate
    u_int32 (*SetBitRate)(u_int32); // return adjusted bit rate
    u_int8 state;
} A2DP_codec_struct;

typedef struct
{
	u_int8 min_bit_pool;
	u_int8 max_bit_pool;
	u_int8 block_len; // b0: 16, b1: 12, b2: 8, b3: 4
	u_int8 subband_num; // b0: 8, b1: 4
	u_int8 alloc_method; // b0: loudness, b1: SNR
	u_int8 sample_rate; // b0: 48000, b1: 44100, b2: 32000, b3: 16000
	u_int8 channel_mode; // b0: joint stereo, b1: stereo, b2: dual channel, b3: mono
} bt_a2dp_sbc_codec_cap_struct;

typedef struct
{
	u_int8 layer; // b0: layerIII, b1: layerII, b0: layerI
	u_int8 CRC;
	u_int8 channel_mode; // b0: joint stereo, b1: stereo, b2: dual channel, b3: mono
	u_int8 MPF; // is support MPF-2
	u_int8 sample_rate; // b0: 48000, b1: 44100, b2: 32000, b3: 24000, b4: 22050, b5: 16000
	u_int8 VBR; // is support VBR
	u_int16 bit_rate; // bit-rate index in ISO 11172-3 , b0:0000 ~ b14: 1110
} bt_a2dp_mp3_codec_cap_struct; /* all MPEG-1,2 Audio */

#if 0
typedef struct
{
	u_int8 object_type; // b4: M4-scalable, b5: M4-LTP, b6: M4-LC, b7: M2-LC
	u_int16 sample_rate; // b0~b11: 96000,88200,64000,48000,44100,32000,24000,22050,16000,12000,11025,8000
	u_int8 channels; // b0: 2, b1: 1
	u_int8 VBR; // is supported VBR
	u_int32 bit_rate; // constant/peak bits per second in 23 bit UiMsbf, 0:unknown
} bt_a2dp_aac_codec_cap_struct; /* all MPEG-2,4 AAC */

typedef struct
{
    u_int8 version; // 1:ATRAC, 2:ATRAC2, 3:ATRAC3
    u_int8 channel_mode; // b0: joint stereo, b1: dual, b2: single
    u_int8 sample_rate; // b0: 48000, b1: 44100
    u_int8 VBR; // is supported VBR
    u_int32 bit_rate; // bit-rate index in ATRAC, b0: 0x0012 ~ b18: 0x0000
    u_int16 max_sul; // sound unit length in 16 bits UiMsbf
} bt_a2dp_atrac_codec_cap_struct; /* all ATRAC family */
#endif

typedef union
{
	bt_a2dp_sbc_codec_cap_struct sbc;
	bt_a2dp_mp3_codec_cap_struct mp3;
	//bt_a2dp_aac_codec_cap_struct aac;
	//bt_a2dp_atrac_codec_cap_struct atrac;
} bt_a2dp_audio_codec_cap_struct;

typedef struct
{
	u_int8 codec_type; // SBC, MP3
	bt_a2dp_audio_codec_cap_struct codec_cap;
} bt_a2dp_audio_cap_struct;

typedef struct
{
	u_int8 in_use;
	u_int8 seid;
	u_int8 media_type;
	u_int8 sep_type;
} bt_sep_info_struct;

typedef struct
{
    u_int16 connect_id;
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
	u_int16 connect_id;
	u_int16 result;
	u_int16 sep_num;
	bt_sep_info_struct sep_list[MAX_NUM_REMOTE_SEIDS];
} t_a2dp_sep_discover_cnf;

#endif

#endif //AVDTP_H_
