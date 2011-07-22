/****************************************************************************
Copyright (C) RDA Ltd. 2008

FILE NAME
    a2dp.h
    
DESCRIPTION
	Header file for the A2DP profile library
	
	The library exposes a functional downstream API and an upstream message 
	based API.
	
	 CLIENT APPLICATION
	 	     |
	 	A2DP Library
	  	     |
	  	AVDTP Library
	            |
	       L2CAP Library
	            |
	      Core
	
*/
/*!
@file	       a2dp.h
@brief	Interface to the Advanced Audio Distribution Profile library.

		Header file for the A2DP profile library.  This profile library
		implements the A2DP using the services of the GAVDP library which is
		hidden from the Client application by this library
		
		When a device wishes to start streaming audio content, the device must
		first set up a streaming connection.  During the stream setup, the
		devices select the most suitable audio streaming parameters.
		Application service capability and transport service capability are
		configured during this stream setup procedure.
		
		Once a streaming connection is established and the start streaming
		procedure as defined in the GAVDP specification is executed, audio can
		be streamed from the Source (SRC) to the Sink (SNK).
		
		This library provides the low level services to permit an audio stream
		to be configured, started, stopped and suspended.  This library
		provides the stream configuration and control.  The actual streaming of
		data is performed by the underlying firmware.  The audio stream is
		routed to the Digital Signal Processor (DSP) present on CSR BlueCore
		Multimedia devices.  The CPU intensive operation of encoding/decoding a
		media stream is performed by the DSP.
	
		The library exposes a functional downstream API and an upstream message
		based API.
*/

#ifndef A2DP_H_
#define A2DP_H_

#include "avdtp.h"


struct __A2DP;
/*!
	@brief The Advanced Audio Distribution Profile structure.
*/
typedef struct __A2DP A2DP;


/*!
	@brief Kalimba Resource.
*/
//#define RESOURCE_KALIMBA	(1)


/*! 
	@brief A2DP role.
	
	The A2DP specification defines two roles, Source (SRC) and Sink (SNK).  
	
	A device is the SRC when it acts as a source of a digital audio stream that
	is delivered to a SNK on the same Piconet.
	
	A device is the SNK when it acts as a sink of a digital audio stream
	delivered from the SRC on the same Piconet.
*/
typedef enum
{
	/*!  This SEP is a sink. Register the config block dependent on the SEP
	  type (a2dp_sep_type). */
	a2dp_sink,				
	/*! The SEP is an AV source with an USB input for the data. */
	a2dp_source_usb,		
	/*! The SEP is an AV source with an analogue input for the data, using the
	  CSR internal codec. */
	a2dp_source_analogue		
} a2dp_role;


/*! 
	@brief The type of the SEP. This identifies the codec type being registered
	or used.
*/
typedef enum
{
	/*! Invalid SEP value. */
	A2DP_SEP_INVALID = 0,	
	/*! The SEP supports the SBC codec (support for this codec is
	  mandatory). */
	A2DP_SBC,				
	/*! The SEP supports the MPEG-1,2 AUdio codec. */
	A2DP_MPEG_AUDIO,		
	/*! The SEP supports the MPEG-2,4 AAC codec. */
	A2DP_MPEG_AAC,			
	/*! The SEP supports a codec from the ATRAC family. */
	A2DP_ATRAC,				
	/*! The SEP supports a vendor defined codec. */
	A2DP_VENDOR				
} t_A2DP_SEP_TYPE;


/*! 
	@brief Codec cppabilities specification block. This is used if the client
	wants to supply their own specific SEP capability parameters.
*/ 
typedef struct 
{
	/*! The size of the configuration block. */
	u_int16	size_service_caps;	
	/*! Capabilities block for the SEP being added. */
	u_int8	*service_caps;		
} sep_config_params;


/*!
	@brief A2DP SEP (Stream End Point) configuration parameters.

	Before a SEP can be used for streaming it must be registered with the A2DP
	library. These parameters are used to determine the SEP type to register
	and configure it correctly.

	If the params pointer is set to null then the default parameters stored in
	the library are used to configure the SEP. If however the client wishes to
	supply their own, specific, set of parameters then they must provide a
	valid sep_config_params configuration. Note that the SEP config block
	provided must either be a pointer to constant data or the client must
	allocate and manage the memory for it as the a2dp library will copy the
	pointer but will not take ownership of that memory. It is recommended that
	the config parameters are passed in as a constant block of data.
*/
typedef struct
{
	t_A2DP_SEP_TYPE		sep_type;
	sep_config_params	*params;
} a2dp_sep_config;


/*! 
	@brief Audio stream channel mode.
	
	The specification defines the following channel modes. The SNK must support
	all modes. It is mandatory for the SRC to support mono and at least one of
	the remaining three modes.
*/
typedef enum
{
	a2dp_mono,
	a2dp_dual_channel,
	a2dp_stereo,
	a2dp_joint_stereo
} a2dp_channel_mode;


/*! 
	@brief The Status codes are returned to indicate to the Client application
	the status of a requested operation.
*/
typedef enum
{
	/*! Operation was successful. */
	a2dp_success = (0),				
	/*! Operation failed. */
 	a2dp_fail,						
	/*! Not enough resources. */
	a2dp_no_resource,				
	/*! Request is not supported in the current state. */
	a2dp_bad_state,					
	/*! Operation timed out before completion. */
	a2dp_timeout,					
	/*! Service capabilities passed to library are bad or incomplete. */
	a2dp_service_caps_error,		
	/*! Local and remote service capabilities are incompatible. */
	a2dp_service_caps_incompatible,	
	/*! Remote device understood but rejected the operation. */
	a2dp_rejected_by_remote_device,	
	/*! Remote device does not support the requested operation. */
	a2dp_unsupported_feature,		
	/*! SEID passed to library is unknown. */
	a2dp_unknown_seid,				
	/*! Remote device closed the stream. */
	a2dp_closed_by_remote_device,	
	/*! Local or remote abort due to signalling breakdown. */
	a2dp_aborted,				    
	/*! Device specified is not connected. */
	a2dp_device_not_connected,		
	/*! The operation is already in progress. */
	a2dp_busy,						
    /*! Connection disconnected due to link loss. */
    a2dp_disconnect_link_loss,
	/*! The A2DP library has received an unrecognised error from the GAVDP
	  library. */
	a2dp_unhandled_error,			
	/*! The requested codec does not have a decode DSP file in the file
	  system. */
	a2dp_unsupported_codec,			
	/*! The requested sep role is unknown/ unsupported. */
	a2dp_unsupported_sep_role		
} a2dp_status_code;


/*! 
	@brief A2DP configuration parameters. 

    The initialisation parameters allow the client to specify whether it wants
    to configure the a2dp library as a source or a sink. The client may also
    specify a service record to register instead of the default one supplied
    with this library. If the service_record pointer is set to null then the
    default service record supplied with the a2dp library is used.
*/
typedef struct
{
	a2dp_role	role;
	u_int16      priority;
	u_int16      size_service_record;
	u_int8       *service_record;
} a2dp_init_params;


/*! 
	@brief The upstream A2DP library messages. 
*/
#define A2DP_MESSAGE_BASE	0x6d00

/*
	Do not document this enum.
*/
#ifndef DO_NOT_DOCUMENT
typedef enum
{
	/* Library initialisation */
	A2DP_INIT_CFM = A2DP_MESSAGE_BASE,
	
	/* Stream endpoint registration */
	A2DP_ADD_SEP_CFM,
	
	/* Stream endpoint connection */
	A2DP_OPEN_CFM,
	A2DP_OPEN_IND,
	A2DP_CLOSE_CFM,
	A2DP_CLOSE_IND,
	
	/* Stream endpoint control */
	A2DP_START_CFM,
	A2DP_START_IND,
	A2DP_SUSPEND_CFM,
	A2DP_SUSPEND_IND,
	
	/* Stream endpoint configuration */
	A2DP_CONFIGURE_CFM, /* reserved for future use */
	A2DP_CODEC_SETTINGS_IND,
	A2DP_SIGNALLING_OPEN_IND,
	A2DP_SIGNALLING_CLOSE_IND,
	
	A2DP_MESSAGE_TOP
} A2dpMessageId;
#endif


typedef enum 
{
	A2dpInitialising,
	A2dpReady,
	A2dpInitiating,
	A2dpSignallingActive,
	A2dpConnected,
	A2dpStreaming
} t_A2DP_STATE;


struct __A2DP
{
	//TaskData		task;
	//Task			clientTask;
	t_A2DP_STATE		state;	
	a2dp_role		role;
	t_A2DP_SEP_TYPE	active_sep;
	u_int16			av_stream_sink_mtu;
	u_int16			priority;
	u_int8			sbc_bitpool_min;
	u_int8			sbc_bitpool_max;
	u_int8			sbc_format;
	AVDTP			*avdtp;
	u_int16          size_service_record;
	u_int8           *service_record;
};

typedef struct
{
    u_int8   a2dp_supported;
    u_int16 supported_feature;
} t_a2dp_Supported_feature;

#endif /* A2DP_H_ */

