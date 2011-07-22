/***********************************************************************
 *
 * MODULE NAME:    avdtp_callbacks.c
 * DESCRIPTION:    Audio/video Distribution Transport Profile request decode
 * MAINTAINER:      Zhu Jianguo
 *
 * 
 * LICENSE:
 *     This source code is copyright (c) 2008 RDA Ltd.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/
#include "l2cap_primitives.h"
#include "host_config.h"
#include "papi.h"
#include "host_types.h"

#include "avdtp_core.h"
#include "avdtp.h"
#include "avdtp_callbacks.h"
#include "avrcp_core.h"
#include "a2dp_core.h"
#include "host_msg.h"

extern AVDTP gAvdtp;   
extern u_int8 gIsConfigured;

u_int8 gActive_codec_type =A2DP_MPEG_AUDIO; 
u_int8 gSBC_MAX_Bit_Pool=0;
//bt_a2dp_audio_codec_cap_struct gCur_Audio_Cap;
u_int8 gCurMaxFrameLen =0;
#ifdef COOLSAND
//avdtp_sep_callbacks sep_cbs;
u_int8 gTransaction_Label=0;
#endif

#define		SEID_MASK	(0x3f)

/* SBC configuration bit fields */
#define SBC_SAMPLING_FREQ_16000        128   /* Octet 0 */
#define SBC_SAMPLING_FREQ_32000         64
#define SBC_SAMPLING_FREQ_44100         32
#define SBC_SAMPLING_FREQ_48000         16
#define SBC_CHANNEL_MODE_MONO            8
#define SBC_CHANNEL_MODE_DUAL_CHAN       4
#define SBC_CHANNEL_MODE_STEREO          2
#define SBC_CHANNEL_MODE_JOINT_STEREO    1
#define SBC_BLOCK_LENGTH_4             128   /* Octet 1 */
#define SBC_BLOCK_LENGTH_8              64
#define SBC_BLOCK_LENGTH_12             32
#define SBC_BLOCK_LENGTH_16             16
#define SBC_SUBBANDS_4                   8
#define SBC_SUBBANDS_8                   4
#define SBC_ALLOCATION_SNR               2
#define SBC_ALLOCATION_LOUDNESS          1
#define SBC_BITPOOL_MIN                  2   /* Octet 2 (min bitpool)  /  Octet 3 (max bitpool) */
#define SBC_BITPOOL_MAX                250
#define SBC_BITPOOL_MEDIUM_QUALITY      32
#define SBC_BITPOOL_HIGH_QUALITY        50


/* MP3 configuration bit fields */
#define MP3_LAYER_I                    128   /* Octet 0 */
#define MP3_LAYER_II                    64
#define MP3_LAYER_III                   32
#define MP3_CRC_PROTECTION              16
#define MP3_CHANNEL_MODE_MONO            8
#define MP3_CHANNEL_MODE_DUAL_CHAN       4
#define MP3_CHANNEL_MODE_STEREO          2
#define MP3_CHANNEL_MODE_JOINT_STEREO    1
#define MP3_SAMPLING_FREQ_16000         32   /* Octet 1 */
#define MP3_SAMPLING_FREQ_22050         16
#define MP3_SAMPLING_FREQ_24000          8
#define MP3_SAMPLING_FREQ_32000          4
#define MP3_SAMPLING_FREQ_44100          2
#define MP3_SAMPLING_FREQ_48000          1
#define MP3_VBR                        128   /* Octet 2 */
#define MP3_BITRATE_VALUE_1110          64
#define MP3_BITRATE_VALUE_1101          32
#define MP3_BITRATE_VALUE_1100          16
#define MP3_BITRATE_VALUE_1011           8
#define MP3_BITRATE_VALUE_1010           4
#define MP3_BITRATE_VALUE_1001           2
#define MP3_BITRATE_VALUE_1000           1
#define MP3_BITRATE_VALUE_0111         128   /* Octet 3 */
#define MP3_BITRATE_VALUE_0110          64
#define MP3_BITRATE_VALUE_0101          32
#define MP3_BITRATE_VALUE_0100          16
#define MP3_BITRATE_VALUE_0011           8
#define MP3_BITRATE_VALUE_0010           4
#define MP3_BITRATE_VALUE_0001           2
#define MP3_BITRATE_VALUE_FREE           1

#define AVDTP_MEDIA_HEADER_LENGTH  12L
extern u_int32 rdabt_put_message(u_int16 q, u_int16 mi, void *mv);
extern void * memset(void *,int,size_t);
extern void * memcpy(void *,const void *,size_t);

/*
	SBC sink caps.
	Support all features and full bitpool range. Note that we trust the source
	to choose a bitpool value suitable for the Bluetooth bandwidth.
*/
static const u_int8 sbc_caps_sink[] = 
{
	AVDTP_SERVICE_MEDIA_TRANSPORT,
	0,
	AVDTP_SERVICE_MEDIA_CODEC,
	6,
	AVDTP_MEDIA_TYPE_AUDIO<<2,
	AVDTP_MEDIA_CODEC_SBC,

	SBC_SAMPLING_FREQ_16000     + SBC_SAMPLING_FREQ_32000    + SBC_SAMPLING_FREQ_44100    + SBC_SAMPLING_FREQ_48000    +
	SBC_CHANNEL_MODE_MONO       + SBC_CHANNEL_MODE_DUAL_CHAN + SBC_CHANNEL_MODE_STEREO    + SBC_CHANNEL_MODE_JOINT_STEREO,

	SBC_BLOCK_LENGTH_4          + SBC_BLOCK_LENGTH_8         + SBC_BLOCK_LENGTH_12        + SBC_BLOCK_LENGTH_16        +
	SBC_SUBBANDS_4              + SBC_SUBBANDS_8             + SBC_ALLOCATION_SNR         + SBC_ALLOCATION_LOUDNESS,

	SBC_BITPOOL_MIN,
	SBC_BITPOOL_MEDIUM_QUALITY,
};


/*
	MP3 sink caps.
	Support all features except:
	- MPEG-1 Layer 3
	- MPF-1
	- free rate
*/
static const u_int8 mp3_caps_sink[] = 
{
	AVDTP_SERVICE_MEDIA_TRANSPORT,
	0,
	AVDTP_SERVICE_MEDIA_CODEC,
	6,
	AVDTP_MEDIA_TYPE_AUDIO<<2,
	AVDTP_MEDIA_CODEC_MPEG1_2_AUDIO,

	MP3_LAYER_III              + MP3_CRC_PROTECTION          +
	MP3_CHANNEL_MODE_MONO      + MP3_CHANNEL_MODE_DUAL_CHAN  + MP3_CHANNEL_MODE_STEREO     + MP3_CHANNEL_MODE_JOINT_STEREO,

	MP3_SAMPLING_FREQ_32000    + MP3_SAMPLING_FREQ_44100     + MP3_SAMPLING_FREQ_48000,

	MP3_VBR                    + MP3_BITRATE_VALUE_1110      + MP3_BITRATE_VALUE_1101      + MP3_BITRATE_VALUE_1100    +
	MP3_BITRATE_VALUE_1011     + MP3_BITRATE_VALUE_1010      + MP3_BITRATE_VALUE_1001      + MP3_BITRATE_VALUE_1000,
	MP3_BITRATE_VALUE_0111     + MP3_BITRATE_VALUE_0110      + MP3_BITRATE_VALUE_0101      + MP3_BITRATE_VALUE_0100    +
	MP3_BITRATE_VALUE_0011     + MP3_BITRATE_VALUE_0010      + MP3_BITRATE_VALUE_0001,    /* All bit rates except 'free' */
};

static boolean bitMaskOk(u_int16 bit_mask);
static boolean areSBCCodecsCompatible(const u_int8 *local_caps, const u_int8 *remote_caps, u_int8 local_losc, u_int8 remote_losc, boolean initiating);
static boolean areMPEG12AudioCodecsCompatible(const u_int8 *local_caps, const u_int8 *remote_caps, u_int8 local_losc, u_int8 remote_losc, boolean initiating);
static void Avdtp_Change_Sep_State(sep* sep_entry, t_SEP_STATE new_state);
static const u_int8* Avdtp_Find_Matching_Codec_Specific_Information(const u_int8 *local_caps, const u_int8 *remote_caps, boolean initiating);
static void Avdtp_Reset_Sep(sep* sep_entry, u_int8 status);
static void Avdtp_Abort_Sep(sep* sep_entry, u_int8 status);
static boolean Avdtp_Validate_Service_Caps(const u_int8 *caps, u_int16 caps_size, boolean reconfigure, boolean only_check_structure,
								u_int8 *error_category, u_int8 *error_code);
static boolean Avdtp_Are_Services_Categories_Compatible(const u_int8 *local_caps, u_int16 local_caps_size,
                                        const u_int8 *config_caps, u_int16 config_caps_size, u_int8 *unsupported_service);

/**********************************************************************/
extern void Headset_A2dp_Suspend_Ind(void);
extern void Headset_A2dp_Start_Ind(void);
/***********************************************************************
 *
 * FUNCTION NAME:   AVDTP_RegisterPSM
 * DESCRIPTION:
 
 This function will perform some initialisation routines for the avdtp. At
the moment this is simply the registering of the callbacks.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
void AVDTP_RegisterPSM(void)
{
	l2_callbacks cbs;
	

	cbs.disconnectionIndication = Avdtp_L2cap_Disconnect_Ind_Cb;
#if A2DP_SRC    
	cbs.qosviolationIndication = Avdtp_L2cap_Qos_Violation_Ind_Cb; 
#endif
	cbs.connectionIndication = Avdtp_L2cap_Connect_Ind_Cb;
	cbs.disconnectionConfirmation = Avdtp_L2cap_Disconnect_Cfm_Cb;
	cbs.dataIndication = Avdtp_L2cap_Data_Ind_Cb;
	cbs.flushOccurred=pNULL;
	cbs.bufferOverflow=pNULL;
	cbs.timeoutIndication=pNULL;
#if A2DP_SRC    
	cbs.connectionConfirmation = Avdtp_L2cap_Connection_Cfm_Cb;
	cbs.configureConfirmation = Avdtp_L2cap_Configure_Cfm_Cb;
#else
	cbs.connectionConfirmation = pNULL;
	cbs.configureConfirmation = pNULL;
#endif
	cbs.configureIndication = Avdtp_l2cap_config_ind_cb;
	cbs.connectionConfiguredConfirmation=pNULL;
	
	L2_RegisterPSM(AVDTP_PSM,&cbs);
	
}



/***********************************************************************/
t_api Avdtp_Layer_Ctrl(u_int8 flags) 
{
	if (flags&PRH_HOST_STARTUP) 
	{
		AVDTP_RegisterPSM();
	} 
	else if (flags&PRH_HOST_RESET) 
	{
		AVDTP_RegisterPSM();
	} 
	else if (flags&PRH_HOST_SHUTDOWN) 
	{

	}
	
	return BT_NOERROR;
}

/*****************************************************************************/
void Avdtp_Register_Sep(t_A2DP_SEP_TYPE sep_type)
{
    if(sep_type==A2DP_SBC)
    {
        gAvdtp.sep_list_head[1].seid=sep_type& SEID_MASK;
        gAvdtp.sep_list_head[1].media_type = SEP_MEDIA_TYPE_AUDIO;
#if A2DP_SRC        
        gAvdtp.sep_list_head[1].role = SEP_SOURCE;
#else
        gAvdtp.sep_list_head[1].role = SEP_SINK;
#endif
        gAvdtp.sep_list_head[1].flush_timeout = 160;
        gAvdtp.sep_list_head[1].state = AVDTP_SEP_STATE_IDLE;
        gAvdtp.sep_list_head[1].sep_count= 0;
        pMemcpy((char*)(gAvdtp.sep_list_head[1].local_service_caps), (void*)sbc_caps_sink, sizeof(sbc_caps_sink));
        gAvdtp.sep_list_head[1].local_service_caps_size = sizeof(sbc_caps_sink);

    }
    else if(sep_type==A2DP_MPEG_AUDIO)
    {
        gAvdtp.sep_list_head[0].seid=sep_type& SEID_MASK;
        gAvdtp.sep_list_head[0].media_type = SEP_MEDIA_TYPE_AUDIO;
#if A2DP_SRC        
        gAvdtp.sep_list_head[0].role = SEP_SOURCE;
#else
        gAvdtp.sep_list_head[0].role = SEP_SINK;
#endif
        gAvdtp.sep_list_head[0].flush_timeout = 160;
        gAvdtp.sep_list_head[0].state = AVDTP_SEP_STATE_IDLE;
        gAvdtp.sep_list_head[0].sep_count= 0;
        pMemcpy((char*)(gAvdtp.sep_list_head[0].local_service_caps), (void*)mp3_caps_sink, sizeof(mp3_caps_sink));
        gAvdtp.sep_list_head[0].local_service_caps_size = sizeof(mp3_caps_sink);	
    }
}

t_SEP_STATE Get_Sep_State()
{
	return gAvdtp.state;
}

/****************************************************************************
NAME	
	validateCommandPDU

DESCRIPTION
	Does a quick check to see if the length of a command packet is reasonable
	for its type.  Typically this is just the mandatory fields, so any optional
	fields should be checked during processing.

RETURNS
	bool - TRUE if packet is reasonable
*/
static boolean Validate_Command_PDU(const u_int8 *ptr, u_int16 packet_size)
{
	u_int16 min_length;

	/* check there is at least a command */
	if (packet_size < 2)
		return FALSE;

	/* check for packets that require more data */
	switch (ptr[1])
	{
		/* no payload */
		case AVDTP_NULL:
		case AVDTP_DISCOVER:
		default:
			min_length= 2;
			break;

		case AVDTP_GET_CONFIGURATION:
		case AVDTP_GET_CAPABILITIES:
		case AVDTP_OPEN:
		case AVDTP_START:
		case AVDTP_CLOSE:
		case AVDTP_SUSPEND:
		case AVDTP_ABORT:
			min_length = 3;
			break;

		case AVDTP_RECONFIGURE:
		case AVDTP_SET_CONFIGURATION:
			min_length = 5;
			break;
	}

	if (packet_size < min_length)
		return FALSE;

	return TRUE;
}


/*****************************************************************************/


static void Avdtp_Change_Sep_State(sep* sep_entry, t_SEP_STATE new_state)
{
	if (((new_state == AVDTP_SEP_STATE_LOCAL_ABORTING) || (new_state == AVDTP_SEP_STATE_REMOTE_ABORTING)) &&
		(sep_entry->state != AVDTP_SEP_STATE_LOCAL_ABORTING) &&
		(sep_entry->state != AVDTP_SEP_STATE_REMOTE_ABORTING))
		/* on first entering abort we need to store the state we came from */
		sep_entry->preabort_state = sep_entry->state;

	sep_entry->state = new_state;
}

/***********************************************************************
 *
 * FUNCTION NAME:   avdtp_discover_decode
 * DESCRIPTION:
 
 This function will discover the stream end point and response.
 * KNOWN ISSUES:
 
 None.
 
 *
 *
 ***********************************************************************/
static void Avdtp_Discover_Command(u_int8* pdu,t_channelId cid)
{
#ifdef COOLSAND
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Discover_Command lab=%x",  (pdu[0]&0xf0)>>4));
    gTransaction_Label = (pdu[0]&0xf0)>>4;
    rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SEND_SEP_DISCOVER_IND,&cid);
#else
	u_int8 i=0;
	sep *sep_entry;
	t_DataBuf *h_buff;
	u_int32 ret;
	ret = L2_GetWriteBuffer(cid, 10, &h_buff); //discover response has 6 bytes in our chip, because we only have 2 SEPs,s0 10 bytes is enough.
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return;
	}
	h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_ACCEPT;  //transaction label, single paket, command
	h_buff->buf[1] = (u_int8) AVDTP_DISCOVER;
    
	 pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Discover_Command lab=%x",  (pdu[0]&0xf0)>>4));
     
	for(i=0;i<2;i++)
	{
		sep_entry = &gAvdtp.sep_list_head[i];  /* Get the SEP from the list */
		
		/* Add details of each ACP end-point */
		if (sep_entry->state != AVDTP_SEP_STATE_EMPTY)
		{
			h_buff->buf[2+i*2] = sep_entry->seid << 2;

			if (Avdtp_Is_Sep_InUse(sep_entry))	
				h_buff->buf[2+i*2] |= 2;

			h_buff->buf[2+i*2+1] = (u_int8) (((u_int16) sep_entry->media_type) << 4) | (((u_int16) sep_entry->role) << 3);
		}

	}
	h_buff->dataLen =  6;
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Incoming_Command: Discover cid=0x%02X, packet_size=%d\n",cid,h_buff->dataLen));
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"buf[0-5] =%x,%x,%x,%x,%x,%x", h_buff->buf[0],h_buff->buf[1],h_buff->buf[2],h_buff->buf[3],h_buff->buf[4],h_buff->buf[5]));
#endif  
	
	//L2_FreeWriteBuffer(h_buff);
	L2_Write(cid, h_buff);
#endif
}


/***********************************************************************
 *
 * FUNCTION NAME:   avdtp_getcapabilities_decode
 * DESCRIPTION:
 
 This function will get the capabilities of the stream end point and response.
 * KNOWN ISSUES:
 
 None.
 
 *
 *
 ***********************************************************************/
static void Avdtp_Getcapabilities_Command(u_int8* pdu,t_channelId cid)
{
#ifdef COOLSAND
    t_a2dp_capabilities_get_ind res;
    gTransaction_Label = (pdu[0]&0xf0)>>4;
    res.connect_id = cid;
    res.acp_seid = (pdu[2]>>2) &0x3f;
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Getcapabilities_Command lab=%x, res.acp_seid=%d",  (pdu[0]&0xf0)>>4, res.acp_seid ));
    rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SEND_SEP_GET_CAPABILITIES_IND,&res);
#else
	u_int8 seid = 0;
	u_int8 i=0;

	//u_int8 resp[48]; 	/* Build response - assume it will always be less than 48 bytes and hence fit in a single L2CAP packet. */
	//struct st_t_dataBuf h_buff;
	t_DataBuf *h_buff;
	u_int32 ret;
	ret = L2_GetWriteBuffer(cid, 48, &h_buff); //discover response has 6 bytes in our chip, because we only have 2 SEPs,s0 10 bytes is enough.
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return;
	}
	//h_buff.buf = resp;
	
	h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_ACCEPT;  //transaction label, single paket, command
	h_buff->buf[1] = (u_int8) AVDTP_GET_CAPABILITIES;

	seid = (pdu[2]>>2) &0x3f;
    
         pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Getcapabilities_Command seid=%d,lab=%x", seid, (pdu[0]&0xf0)>>4));
         
	if(seid == 1 || seid==2)//stream endpoint 1 sbc, or endpoint 2 mp3 
	{
		i = seid-1;
		/* copy in the caps supplied by the application */
		pMemcpy(&h_buff->buf[2], (void*)gAvdtp.sep_list_head[i].local_service_caps, gAvdtp.sep_list_head[i].local_service_caps_size);
		h_buff->dataLen = 2+gAvdtp.sep_list_head[i].local_service_caps_size;
	}
	else  /* Invalid SEP - reject request */
	{
		h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;  //transaction label, single paket, command
		h_buff->buf[2] = (u_int8) avdtp_bad_acp_seid;
		h_buff->dataLen =3;
	}
    
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Incoming_Command: Getcapabilities cid=0x%02X,seid=%d, packet_size=%d, h_buff->buf=%s\n",cid,seid,h_buff->dataLen));
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"buf[0-11] =%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x", h_buff->buf[0],h_buff->buf[1],h_buff->buf[2],h_buff->buf[3],h_buff->buf[4],h_buff->buf[5],
                                                                                                    h_buff->buf[6],h_buff->buf[7],h_buff->buf[8],h_buff->buf[9],h_buff->buf[10],h_buff->buf[11]));
#endif  

	//L2_FreeWriteBuffer(h_buff);
	L2_Write(cid, h_buff);
#endif
}


/***********************************************************************
 *
 * FUNCTION NAME:   avdtp_setconfigration_decode
 * DESCRIPTION:
 
 This function will check the validity of the set config and then response.
 * KNOWN ISSUES:
 
 None.
 
 *
 *
 ***********************************************************************/
static void Avdtp_Setconfigration_Command(u_int8* pdu, t_channelId cid, u_int16 packet_size)
{
#ifdef COOLSAND
    t_a2dp_stream_config_ind res;
    u_int8 codec_type;
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Setconfigration:size:%d, pdu[0-13]:%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x",packet_size, pdu[0],pdu[1], pdu[2], pdu[3],pdu[4],pdu[5],pdu[6], pdu[7], pdu[8],pdu[9],pdu[10], pdu[11], pdu[12],pdu[13] ));

    codec_type = pdu[9];
    gTransaction_Label = (pdu[0]&0xf0)>>4;
    res.connect_id = cid;
    res.acp_seid = pdu[2]>>2;
    res.int_seid = pdu[3]>>2;
    res.stream_handle =  pdu[3]>>2;//we use it to store the int seid.
    res.audio_cap.codec_type = codec_type+1; 
    gActive_codec_type = codec_type+1;
    gAvdtp.active_seid = res.int_seid; //store the HS int seid.

    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Setconfigration_Command lab=%x, gAvdtp.active_seid:%d,  res.acp_seid:%d",  (pdu[0]&0xf0)>>4,gAvdtp.active_seid, res.acp_seid));
    
    if(codec_type==AVDTP_MEDIA_CODEC_SBC)
    {
         res.audio_cap.codec_cap.sbc.sample_rate = (pdu[10]>>4)&0x0F;
         res.audio_cap.codec_cap.sbc.channel_mode = pdu[10]&0x0F;
         res.audio_cap.codec_cap.sbc.block_len= (pdu[11]>>4)&0x0F;
         res.audio_cap.codec_cap.sbc.subband_num= (pdu[11]>>2)&0x03;
         res.audio_cap.codec_cap.sbc.alloc_method= (pdu[11])&0x03;
         res.audio_cap.codec_cap.sbc.min_bit_pool= pdu[12];
         res.audio_cap.codec_cap.sbc.max_bit_pool= pdu[13];
    }
    else if (codec_type==AVDTP_MEDIA_CODEC_MPEG1_2_AUDIO)
    {
        res.audio_cap.codec_cap.mp3.layer = (pdu[10]>>5)&0x07;
        res.audio_cap.codec_cap.mp3.CRC= (pdu[10]>>4)&0x01;
        res.audio_cap.codec_cap.mp3.channel_mode= pdu[10]&0x0F;
        res.audio_cap.codec_cap.mp3.MPF = (pdu[11]>>6)&0x01;
        res.audio_cap.codec_cap.mp3.sample_rate = pdu[11]&0x3f;
        res.audio_cap.codec_cap.mp3.VBR = (pdu[12]>>7)&0x01;
        res.audio_cap.codec_cap.mp3.bit_rate =  (u_int16)((pdu[13])|(pdu[12]&0x7f)<<8); 
    }
    rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SEND_SEP_SET_CONFIG_IND,&res);
      
#else
	/* Build response - assume it will always be less than
	   48 bytes and hence fit in a single L2CAP packet. */
	u_int8 unsupported_service;
	u_int8 error_cat, error_code;
	//u_int8 resp[48];
	u_int8 seid = 0;
	u_int8 i=0;
	t_DataBuf *h_buff;
	sep *sep_entry;
	
	//h_buff.buf = resp;
	u_int32 ret;
	ret = L2_GetWriteBuffer(cid, 48, &h_buff);
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return;
	}
	seid = (pdu[2]>>2) &0x3f;
	i = seid-1;
	 pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Setconfigration_Command seid=%d,lab=%x", seid, (pdu[0]&0xf0)>>4));
	if(seid ==1 || seid==2)
	{
		sep_entry = &gAvdtp.sep_list_head[i];
		gAvdtp.active_seid = seid;
		
		if (Avdtp_Is_Sep_InUse(sep_entry))	/* SEP is already in use - reject */
		{
			h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
			h_buff->buf[1] = (u_int8) AVDTP_SET_CONFIGURATION;
			h_buff->buf[2] = 0;   /* Service Capabilities were not the problem */
			h_buff->buf[3] = (u_int8) avdtp_sep_in_use;
			h_buff->dataLen = 4;
		}
		else if(!Avdtp_Validate_Service_Caps(&pdu[4],packet_size-4, FALSE, FALSE, &error_cat, &error_code))
		{
			h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
			h_buff->buf[1] = (u_int8) AVDTP_SET_CONFIGURATION;
			h_buff->buf[2] = error_cat;   /* Service Capabilities were not the problem */
			h_buff->buf[3] = error_code;
			h_buff->dataLen = 4;
		}
		else if(!Avdtp_Are_Services_Categories_Compatible(sep_entry->local_service_caps, sep_entry->local_service_caps_size, &pdu[4], packet_size-4, &unsupported_service))
		{
			h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
			h_buff->buf[1] = (u_int8) AVDTP_SET_CONFIGURATION;
			h_buff->buf[2] = unsupported_service;   /* Service Capabilities were not the problem */
			h_buff->buf[3] = (u_int8) avdtp_unsupported_configuration;
			h_buff->dataLen = 4;			
		}
		else if(Avdtp_Find_Matching_Codec_Specific_Information(sep_entry->local_service_caps, &pdu[4], 0) == NULL)
		{
			/* Check the codec specific attributes are compatible, the set config does not match our caps - reject */
			h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
			h_buff->buf[1] = (u_int8) AVDTP_SET_CONFIGURATION;
			h_buff->buf[2] = AVDTP_SERVICE_MEDIA_CODEC;   /* Service Capabilities were not the problem */
			h_buff->buf[3] = (u_int8) avdtp_unsupported_configuration;
			h_buff->dataLen = 4;	
		}
		else  //valid config and response.
		{
			h_buff->buf[0] = (pdu[0] & 0xf0) |  AVDTP_MESSAGE_TYPE_ACCEPT;
			h_buff->buf[1] = (u_int8) AVDTP_SET_CONFIGURATION;

			/* SEP is now configured */
			//sep_entry->state = AVDTP_SEP_STATE_CONFIGURED;
			Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_CONFIGURED);

			/* Store remote SEID */
			sep_entry->remote_seid = (pdu[3] >> 2) & 0x3f;

			/* Store configuration */
			sep_entry->configured_service_caps_size = packet_size - 4;

			pMemcpy(sep_entry->configured_service_caps, &pdu[4], sep_entry->configured_service_caps_size);

			/* Associate with new Signalling Channel */
			//sep_entry->base_connection = conn;

			h_buff->dataLen = 2;

			//here maybe we should set the codec settings according the setconfiguraton.!!!!!!!!!!!!
			// Codec_Setting();   TO DO LATER....
		}
		
	}
	else  /* Invalid SEP - reject request */
	{
		h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
		h_buff->buf[1] = (u_int8) AVDTP_SET_CONFIGURATION;
		h_buff->buf[2] = 0;      /* Service Capabilities were not the problem */
		h_buff->buf[3] = (u_int8) avdtp_bad_acp_seid;
		h_buff->dataLen = 4;
	}

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Incoming_Command: Setconfigration cid=0x%02X, packet_size=%d\n",cid,h_buff->dataLen));
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"buf[0-1] =%x,%x", h_buff->buf[0],h_buff->buf[1]));

#endif  

	//L2_FreeWriteBuffer(h_buff);
	L2_Write(cid, h_buff);
#endif
}


/***********************************************************************
 *
 * FUNCTION NAME:   avdtp_getconfiguration_decode
 * DESCRIPTION:
 
 This function will get the config and response.
 * KNOWN ISSUES:
 
 None.
 
 *
 *
 ***********************************************************************/
static void Avdtp_Getconfiguration_Command(u_int8* pdu,t_channelId cid)
{
	/* Build response - assume it will always be less than
	48 bytes and hence fit in a single L2CAP packet. */
	//u_int8 resp[48];
	u_int8 seid = 0;
	u_int8 i=0;
	t_DataBuf *h_buff;
	sep *sep_entry;
    
	//h_buff.buf = resp;
	u_int32 ret;
	ret = L2_GetWriteBuffer(cid, 48, &h_buff);
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ;
	}
	seid = (pdu[2]>>2) &0x3f;
	i = seid-1;

	if(seid ==1 || seid==2)
	{
		sep_entry = &gAvdtp.sep_list_head[i];
		
		if ( (sep_entry->state != AVDTP_SEP_STATE_CONFIGURED) &&  
			(sep_entry->state != AVDTP_SEP_STATE_OPEN) &&  
			(sep_entry->state != AVDTP_SEP_STATE_STREAMING) )
		{
			h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
			h_buff->buf[1] = (u_int8) AVDTP_GET_CONFIGURATION;
			h_buff->buf[2] = avdtp_bad_state;      
			h_buff->dataLen = 3;	
		}
		else
		{
			h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_ACCEPT;
			h_buff->buf[1] = (u_int8) AVDTP_GET_CONFIGURATION;		

			/* copy in the agreed configuration for this SEP.
			Note that this only returns the last SET_CONFIG or RECONFIGURE
			parameters and not the global configuration - is this ok? */
			pMemcpy(&h_buff->buf[2],sep_entry->configured_service_caps, sep_entry->configured_service_caps_size);
			h_buff->dataLen = 2 + sep_entry->configured_service_caps_size;;	
		}
		
	}
	else/* Invalid SEP - reject request */
	{
		h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
		h_buff->buf[1] = (u_int8) AVDTP_GET_CONFIGURATION;
		h_buff->buf[2] = avdtp_bad_acp_seid;      
		h_buff->dataLen = 3;
	}

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Incoming_Command: Getconfiguration cid=0x%02X, packet_size=%d\n",cid,h_buff->dataLen));
    pDebugDump(pLOGA2DP,h_buff->buf,h_buff->dataLen);
#endif  

	//L2_FreeWriteBuffer(h_buff);
	L2_Write(cid, h_buff);
}


/***********************************************************************
 *
 * FUNCTION NAME:   avdtp_reconfigure_decode
 * DESCRIPTION:
 
 This function will reconfig service capabilities and response.
 * KNOWN ISSUES:
 
 None.
 
 *
 *
 ***********************************************************************/
static void Avdtp_Reconfigure_Command(u_int8* pdu,t_channelId cid, u_int16 packet_size)
{
	/* Build response - assume it will always be less than
	   48 bytes and hence fit in a single L2CAP packet. */
	u_int8 unsupported_service;
	u_int8 error_cat, error_code;
	//u_int8 resp[48];
	u_int8 seid = 0;
	u_int8 i=0;
	t_DataBuf *h_buff;
	sep *sep_entry;

	//h_buff.buf = resp;
	u_int32 ret;
	ret = L2_GetWriteBuffer(cid, 48, &h_buff);
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ;
	}
	seid = (pdu[2]>>2) &0x3f;
	i = seid-1;

	if(seid ==1 || seid==2)
	{
		sep_entry = &gAvdtp.sep_list_head[i];

		if (!Avdtp_Validate_Service_Caps(&pdu[3], packet_size-3, FALSE, TRUE, &error_cat, &error_code))
		{
			/*Check that caps from remote device parse and look reasonable bad caps - reject  */
			h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
			h_buff->buf[1] = (u_int8) AVDTP_RECONFIGURE;
			h_buff->buf[2] = error_cat;
			h_buff->buf[3] = error_code;
			h_buff->dataLen = 4;
		}
		/* check that the service caps are valid for reconfigure */
		else if (!Avdtp_Validate_Service_Caps(&pdu[3], packet_size-3, TRUE, FALSE, &error_cat, &error_code))
		{
			/* Check that caps from remote device parse and look reasonable bad caps - reject */
		}
		else if(sep_entry->state != AVDTP_SEP_STATE_OPEN)
		{
			h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
			h_buff->buf[1] = (u_int8) AVDTP_RECONFIGURE;
			h_buff->buf[2] = 0;      /* Service Capabilities were not the problem */
			h_buff->buf[3] = (u_int8) avdtp_sep_not_in_use;
			h_buff->dataLen = 4;
		}
		else if(!Avdtp_Are_Services_Categories_Compatible(sep_entry->local_service_caps, sep_entry->local_service_caps_size, &pdu[3], packet_size-3, &unsupported_service))
		{
			h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
			h_buff->buf[1] = (u_int8) AVDTP_RECONFIGURE;
			h_buff->buf[2] = unsupported_service;     
			h_buff->buf[3] = (u_int8) avdtp_unsupported_configuration;
			h_buff->dataLen = 4;
		}
		else if(Avdtp_Find_Matching_Codec_Specific_Information(sep_entry->local_service_caps, &pdu[3], 0) == NULL)
		{
			h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
			h_buff->buf[1] = (u_int8) AVDTP_RECONFIGURE;
			h_buff->buf[2] = AVDTP_SERVICE_MEDIA_CODEC;     
			h_buff->buf[3] = (u_int8) avdtp_unsupported_configuration;
			h_buff->dataLen = 4;			
		}
		else //valid, reconfig and response
		{
			h_buff->buf[0] = (pdu[0] & 0xf0) |  AVDTP_MESSAGE_TYPE_ACCEPT;
			h_buff->buf[1] = (u_int8) AVDTP_RECONFIGURE;
			h_buff->dataLen = 2;
			
			/* SEP is now back to open */
			//sep_entry->state = AVDTP_SEP_STATE_OPEN;
			Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_OPEN);

			/* Store remote SEID */
			sep_entry->remote_seid = (pdu[3] >> 2) & 0x3f;

			/* Free any old configuration */
			pMemset((char*)sep_entry->configured_service_caps,0, sizeof(sep_entry->configured_service_caps));

			/* Store configuration */
			sep_entry->configured_service_caps_size = packet_size - 3;

			pMemcpy(sep_entry->configured_service_caps, &pdu[3], sep_entry->configured_service_caps_size);

			//here maybe we should set the codec settings according the setconfiguraton.!!!!!!!!!!!!
			// Codec_Setting();
		}
		
	}
	else/* Invalid SEP - reject request */
	{
		h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
		h_buff->buf[1] = (u_int8) AVDTP_RECONFIGURE;
		h_buff->buf[2] = 0;      /* Service Capabilities were not the problem */
		h_buff->buf[3] = (u_int8) avdtp_sep_not_in_use;
		h_buff->dataLen = 4;
	}

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Incoming_Command: Reconfigure cid=0x%02X, packet_size=%d, h_buff->buf=%s\n",cid,h_buff->dataLen));
    pDebugDump(pLOGA2DP,h_buff->buf,h_buff->dataLen);
#endif  

	//L2_FreeWriteBuffer(h_buff);
	L2_Write(cid, h_buff);
}

/***********************************************************************
 *
 * FUNCTION NAME:   avdtp_open_decode
 * DESCRIPTION:
 
 This function will open the stream and response.
 * KNOWN ISSUES:
 
 None.
 
 *
 *
 ***********************************************************************/
static void Avdtp_Open_Command(u_int8* pdu,t_channelId cid)
{
#ifdef COOLSAND
    u_int8  res;
    gTransaction_Label = (pdu[0]&0xf0)>>4;
    res = (pdu[2]>>2)&0x3F;  //acp endpoint
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Open_Command lab=%x,res=%d",  (pdu[0]&0xf0)>>4, res));
    rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SEND_SEP_OPEN_IND,&res);
#else
	/* Build response - assume it will always be less than
	48 bytes and hence fit in a single L2CAP packet. */
	//u_int8 resp[48];
	u_int8 seid = 0;
	u_int8 i=0;
	//struct st_t_dataBuf h_buff;
	t_DataBuf *h_buff;
	sep *sep_entry;

	//h_buff.buf = resp;
	u_int32 ret;
	ret = L2_GetWriteBuffer(cid, 48, &h_buff);
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ;
	}
	seid = (pdu[2]>>2) &0x3f;
	i = seid-1;

        pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Open_Command seid=%d,lab=%x", seid, (pdu[0]&0xf0)>>4));

	if(seid ==1 || seid==2)
	{
		sep_entry = &gAvdtp.sep_list_head[i];
		if (sep_entry->state != AVDTP_SEP_STATE_CONFIGURED)
		{
			h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
			h_buff->buf[1] = (u_int8) AVDTP_OPEN;
			h_buff->buf[2] = (u_int8) avdtp_bad_state;
			h_buff->dataLen = 3;		
		}
		else
		{
			h_buff->buf[0] = (pdu[0] & 0xf0) |  AVDTP_MESSAGE_TYPE_ACCEPT;
			h_buff->buf[1] = (u_int8) AVDTP_OPEN;
			h_buff->dataLen = 2;

			//sep_entry->state =AVDTP_SEP_STATE_OPEN;
			Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_OPEN);
		}
	}
	else
	{
		h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
		h_buff->buf[1] = (u_int8) AVDTP_OPEN;
		h_buff->buf[2] = (u_int8) avdtp_bad_acp_seid;
		h_buff->dataLen = 3;
	}

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Incoming_Command: Open cid=0x%02X, packet_size=%d, h_buff->buf=%s\n",cid,h_buff->dataLen));
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"buf[0-1] =%x,%x", h_buff->buf[0],h_buff->buf[1]));
#endif 

	//L2_FreeWriteBuffer(h_buff);
	L2_Write(cid, h_buff);	
#endif
}

/***********************************************************************
 *
 * FUNCTION NAME:   avdtp_start_decode
 * DESCRIPTION:
 
 This function will start the stream and response.
 * KNOWN ISSUES:
 
 None.
 
 *
 *
 ***********************************************************************/
static void Avdtp_Start_Command(u_int8* pdu,t_channelId cid, u_int16 packet_size)
{
#ifdef COOLSAND
    u_int8  res;
    gTransaction_Label = (pdu[0]&0xf0)>>4;
    res = (pdu[2]>>2)&0x3F;
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Start_Command lab=0x%x,res=%d",  (pdu[0]&0xf0)>>4, res));
    rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SEND_SEP_START_IND,&res);
#else
	/* Build response - assume it will always be less than
	48 bytes and hence fit in a single L2CAP packet. */
	//u_int8 resp[48];
	u_int8 seids = 0;
	u_int8 i=0;
	//struct st_t_dataBuf h_buff;
	t_DataBuf *h_buff;
	sep *sep_entry;

	//h_buff.buf = resp;
	u_int32 ret;
	ret = L2_GetWriteBuffer(cid, 48, &h_buff);
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ;
	}
	seids = packet_size-2;

    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Start_Command seids=%d,lab=%x", seids, (pdu[0]&0xf0)>>4));

	for(;seids!=0;seids--)
	{
		u_int8 seid =  (pdu[packet_size-seids] >> 2) & 0x3f;
		i = seid-1;
            pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Start_Command seid=%d", seid));
		if(seid ==1 || seid==2)
		{
			sep_entry = &gAvdtp.sep_list_head[i];
			/*
				There is a race condition due to Streams which causes us to see data before L2CAP signalling.
				This means that we may see a START request before being informed of the media channel(s) opening.
				To avoid this, we ignore the command now and come back later.
			*/
			//if (sep_entry->state == av_sep_state_remote_opening)   //?????????????????????????????????
			//{
			//	return ;
			//}
			
			if (sep_entry->state != AVDTP_SEP_STATE_OPEN)
			{
				h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
				h_buff->buf[1] = (u_int8) AVDTP_START;
				h_buff->buf[2] = seid << 2;
				h_buff->buf[3] = (u_int8) avdtp_bad_state;
				h_buff->dataLen = 4;
				//L2_FreeWriteBuffer(h_buff);
				L2_Write(cid, h_buff);	
				return;
			}

			//sep_entry->state =AVDTP_SEP_STATE_STREAMING;
			Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_STREAMING);

		}
		else
		{
			h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
			h_buff->buf[1] = (u_int8) AVDTP_START;
			h_buff->buf[2] = seid << 2;
			h_buff->buf[3] = (u_int8) avdtp_bad_acp_seid;
			h_buff->dataLen = 4;
			L2_Write(cid, h_buff);	
			//L2_FreeWriteBuffer(h_buff);
			return;
		}
	}
	Headset_A2dp_Start_Ind();
	
	/* All SEIDs started, accept */
	h_buff->buf[0] = (pdu[0] & 0xf0) |  AVDTP_MESSAGE_TYPE_ACCEPT;
	h_buff->buf[1] = (u_int8) AVDTP_START;
	h_buff->dataLen = 2;

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Incoming_Command: Start cid=0x%02X, packet_size=%d, h_buff->buf=%s\n",cid,h_buff->dataLen));
	pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"buf[0-1] =%x,%x", h_buff->buf[0],h_buff->buf[1]));
#endif 
    
	L2_Write(cid, h_buff);	
#endif
}

/***********************************************************************
 *
 * FUNCTION NAME:   avdtp_close_decode
 * DESCRIPTION:
 
 This function will close the stream and response.
 * KNOWN ISSUES:
 
 None.
 
 *
 *
 ***********************************************************************/
static void Avdtp_Close_Command(u_int8* pdu,t_channelId cid)
{
#ifdef COOLSAND
    u_int8  res;
    gTransaction_Label = (pdu[0]&0xf0)>>4;
    res = (pdu[2]>>2)&0x3F;
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Close_Command lab=%x,res=%x",  (pdu[0]&0xf0)>>4,res));
    rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SEND_SEP_CLOSE_IND,&res);
#else
	/* Build response - assume it will always be less than
	48 bytes and hence fit in a single L2CAP packet. */
	//u_int8 resp[48];
	u_int8 seid = 0;
	u_int8 i=0;
	//struct st_t_dataBuf h_buff;
	t_DataBuf *h_buff;
	sep *sep_entry;

	
	//h_buff.buf = resp;
	u_int32 ret;
	ret = L2_GetWriteBuffer(cid, 48, &h_buff);
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ;
	}
	seid = (pdu[2]>>2) &0x3f;
	i = seid-1;

	if(seid ==1 || seid==2)
	{
		sep_entry = &gAvdtp.sep_list_head[i];
		if ((sep_entry->state != AVDTP_SEP_STATE_OPEN) && (sep_entry->state != AVDTP_SEP_STATE_STREAMING))
		{
			h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
			h_buff->buf[1] = (u_int8) AVDTP_CLOSE;
			h_buff->buf[2] = (u_int8) avdtp_bad_state;
			h_buff->dataLen = 3;		
		}
		else
		{
			h_buff->buf[0] = (pdu[0] & 0xf0) |  AVDTP_MESSAGE_TYPE_ACCEPT;
			h_buff->buf[1] = (u_int8) AVDTP_CLOSE;
			h_buff->dataLen = 2;

			//sep_entry->state =AVDTP_SEP_STATE_REMOTE_ABORTING;
			Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_REMOTE_ABORTING);
		}
	}
	else
	{
		h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
		h_buff->buf[1] = (u_int8) AVDTP_CLOSE;
		h_buff->buf[2] = (u_int8) avdtp_bad_acp_seid;
		h_buff->dataLen = 3;
	}

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Incoming_Command: Close cid=0x%02X, packet_size=%d, h_buff->buf=%s\n",cid,h_buff->dataLen));
	//pDebugDump(pLOGA2DP,h_buff->buf,h_buff->dataLen);
#endif 
	
	L2_Write(cid, h_buff);	
#endif
}

/***********************************************************************
 *
 * FUNCTION NAME:   avdtp_suspend_decode
 * DESCRIPTION:
 
 This function will suspend the stream and response.
 * KNOWN ISSUES:
 
 None.
 
 *
 *
 ***********************************************************************/
static void Avdtp_Suspend_Command(u_int8* pdu,t_channelId cid, u_int16 packet_size)
{
#ifdef COOLSAND
    u_int8  res;
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Suspend_Command lab=%x",  (pdu[0]&0xf0)>>4));
    gTransaction_Label = (pdu[0]&0xf0)>>4;
    res = (pdu[2]>>2)&0x3F;
    rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SEND_SEP_PAUSE_IND,&res);
#else
	/* Build response - assume it will always be less than
	48 bytes and hence fit in a single L2CAP packet. */
	//u_int8 resp[48];
	u_int8 seids = 0;
	u_int8 i=0;
	//struct st_t_dataBuf h_buff;
	t_DataBuf *h_buff;
	sep *sep_entry;


	
	//h_buff.buf = resp;
	u_int32 ret;
	ret = L2_GetWriteBuffer(cid, 48, &h_buff);
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ;
	}

	
	seids = packet_size-2;

	for(;seids!=0;seids--)
	{
		u_int8 seid = (pdu[packet_size-seids] >> 2) & 0x3f;
		i = seid-1;
		sep_entry = &gAvdtp.sep_list_head[i];

		if(seid ==1 || seid==2)
		{
			if (sep_entry->state != AVDTP_SEP_STATE_STREAMING)
			{
				h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
				h_buff->buf[1] = (u_int8) AVDTP_SUSPEND;
				h_buff->buf[2] = seid << 2;
				h_buff->buf[3] = (u_int8) avdtp_bad_state;
				h_buff->dataLen = 4;		
				L2_Write(cid, h_buff);
				//L2_FreeWriteBuffer(h_buff);
				return;
			}
			//sep_entry->state =AVDTP_SEP_STATE_OPEN;
			Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_OPEN);

		}
		else
		{
			h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
			h_buff->buf[1] = (u_int8) AVDTP_SUSPEND;
			h_buff->buf[2] = seid << 2;
			h_buff->buf[3] = (u_int8) avdtp_bad_acp_seid;
			h_buff->dataLen = 4;
			L2_Write(cid, h_buff);
			//L2_FreeWriteBuffer(h_buff);
			return;
		}
	}

	Headset_A2dp_Suspend_Ind();
	
	/* all SEIDs started, accept */
	h_buff->buf[0] = (pdu[0] & 0xf0) |  AVDTP_MESSAGE_TYPE_ACCEPT;
	h_buff->buf[1] = (u_int8) AVDTP_SUSPEND;
	h_buff->dataLen = 2;

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Incoming_Command: Suspend cid=0x%02X, packet_size=%d, h_buff->buf=%s\n",cid,h_buff->dataLen));
	pDebugDump(pLOGA2DP,h_buff->buf,h_buff->dataLen);
#endif 
    
	L2_Write(cid, h_buff);	
#endif
}

/***********************************************************************
 *
 * FUNCTION NAME:   avdtp_abort_decode
 * DESCRIPTION:
 
 This function will abort the stream and response.
 * KNOWN ISSUES:
 
 None.
 
 *
 *
 ***********************************************************************/
 #if 0
static void Avdtp_Abort_Command(u_int8* pdu,t_channelId cid)
{
#ifdef COOLSAND
    u_int8  res;
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Abort_Command lab=%x",  (pdu[0]&0xf0)>>4));
    gTransaction_Label = (pdu[0]&0xf0)>>4;
    res = (pdu[2]>>2)&0x3F;
    rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SEND_SEP_ABORT_IND,&res);
#else
	/* Build response - assume it will always be less than
	48 bytes and hence fit in a single L2CAP packet. */
	//u_int8 resp[48];
	u_int8 seid = 0;
	u_int8 i=0;
	//struct st_t_dataBuf h_buff;
	t_DataBuf *h_buff;
	sep *sep_entry;
	
	//h_buff.buf = resp;
	u_int32 ret;
	ret = L2_GetWriteBuffer(cid, 48, &h_buff);
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ;
	}
	
	seid = (pdu[2]>>2) &0x3f;
	i = seid-1;

	if(seid ==1 || seid==2)
	{
		sep_entry = &gAvdtp.sep_list_head[i];
		//sep_entry->state =AVDTP_SEP_STATE_REMOTE_ABORTING;
		Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_REMOTE_ABORTING);
		
		/* must close signalling channel if an ABORT is received */
		//need to set the sep to idle later!!!!!!!!!!!!!!!!!!
	}
	
	/* We always have to acknowledge ABORT */
	h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_ACCEPT;
	h_buff->buf[1] = (u_int8) AVDTP_ABORT;
	h_buff->dataLen = 2;

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Incoming_Command: Abort cid=0x%02X, packet_size=%d, h_buff->buf=%s\n",cid,h_buff->dataLen));
	pDebugDump(pLOGA2DP,h_buff->buf,h_buff->dataLen);
#endif 
	
	L2_Write(cid, h_buff);	
#endif
}
#endif
/***********************************************************************
 *
 * FUNCTION NAME:   avdtp_security_control_decode
 * DESCRIPTION:
 
 This function will security control the stream and response.
 * KNOWN ISSUES:
 
 None.
 
 *
 *
 ***********************************************************************/
static void Avdtp_Security_Control_Command(u_int8* pdu,t_channelId cid)
{

}


/****************************************************************************
NAME	
	avdtpValidateServiceCaps

DESCRIPTION
	Attempts to validate that a support Service Capabilities list can
	be parsed and contains reasonable values.

	This function should allow all valid values, even if the local hardware/software
	does not support them.

	It is also used to validate the caps returned by the remote device, so should not
	be dependent on local settings.

	The 'reconfigure' flag is used to adjust the validation rules depending on if
	the Capabilities supplied are complete, or part of a reconfiguration.

	When 'only_check_structure' is TRUE this function only tests that the structure
	is correct, it does not verify if mandatory entries are present.

	When the function returns FALSE, it will write the service category with the error
	and the AVDTP error code to error_category and error_code parameters.

RETURNS
	void
****************************************************************************/
static boolean Avdtp_Validate_Service_Caps(const u_int8 *caps, u_int16 caps_size, boolean reconfigure, boolean only_check_structure,
								u_int8 *error_category, u_int8 *error_code)
{
    boolean has_media_transport = FALSE;
    boolean has_codec = FALSE;
    u_int8 media_codec[12];
    u_int8 codec_type =0;
	/* set error to a reasonable default so we don't have to fill it out  everywhere.
	*/
	*error_code = avdtp_invalid_capabilities;

	do
	{
		u_int8 service;
		u_int16 losc;

		/* each entry must contain at least two bytes; Service Category and LOSC. */
		if (caps_size < 2)
			return FALSE;

		/* read header */
		service = *(caps++);
		losc = *(caps++);
		caps_size-=2;

		/* keep the error current */
		*error_category = service;

		/* is there enough space to contain the declared LOSC */
		if (losc > caps_size)
			return FALSE;

		/*
			Perform some checks on the entries			
		*/
		switch (service)
		{
			case AVDTP_SERVICE_MEDIA_TRANSPORT:
				has_media_transport = TRUE;
				if (losc != 0)
				{
					*error_code = avdtp_bad_media_transport_format;
					return FALSE;
				}
				break;

			case AVDTP_SERVICE_REPORTING:
				/* Reporting losc is always 0. (AVDTP 8.19.3) */
				if (losc != 0)
				{
					*error_code = avdtp_bad_payload_format;
					return FALSE;
				}
				break;

			case AVDTP_SERVICE_RECOVERY:
				/* Check caps match those defined in spec (AVDTP 8.19.4) */
				if ((losc != 3) || (caps_size < 3))
				{
					*error_code = avdtp_bad_recovery_format;
					return FALSE;
				}
				/* Recovery Type: 1 is only valid value */
				if (caps[0] != 1)
				{
					*error_code = avdtp_bad_recovery_type;
					return FALSE;
				}
				/* MRWS: check range */
				if ((caps[1] < 0x01) || (caps[1] > 0x18))
				{
					*error_code = avdtp_bad_recovery_format;
					return FALSE;
				}
				/* MNMP: check range */
				if ((caps[2] < 0x01) || (caps[2] > 0x18))
				{
					*error_code = avdtp_bad_recovery_format;
					return FALSE;
				}
				break;

			case AVDTP_SERVICE_CONTENT_PROTECTION:
				/* content protection must at least containt 16bit Type */
				if (losc < 2)
				{
					*error_code = avdtp_bad_cp_format;
					return FALSE;
				}
				break;

			case AVDTP_SERVICE_HEADER_COMPRESSION:
				if (losc != 1)
				{
					*error_code = avdtp_bad_rohc_format;
					return FALSE;
				}
				break;

			case AVDTP_SERVICE_MULTIPLEXING:
				if (losc != 9)
				{
					*error_code = avdtp_bad_multiplexing_format;
					return FALSE;
				}
				break;

			case AVDTP_SERVICE_MEDIA_CODEC:
                            memset(media_codec,0,sizeof(media_codec));
                            memcpy(media_codec,caps, losc);
                            codec_type = media_codec[1];
                            if(codec_type ==AVDTP_MEDIA_CODEC_SBC)
                            {
                                gSBC_MAX_Bit_Pool = media_codec[5];
                                pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Validate_Service_Caps gSBC_MAX_Bit_Pool=%d",  gSBC_MAX_Bit_Pool));
                            }
				/* Actual codec parameters are validated during configuration
				   in findMatchingCodecSpecificInformation(). We just check
				   the structure here. */
				if (losc < 2)
				{
					/* must contain a media type and codec type. */
					*error_code = avdtp_bad_payload_format;
					return FALSE;
				}

				if (has_codec)
					return FALSE;
				else
					has_codec = TRUE;

				break;

			default:
				*error_code = avdtp_bad_serv_category;
				return FALSE;
		}

		/* move to next entry */
		caps += losc;
		caps_size -= losc;
	}	while (caps_size != 0);

	if (!only_check_structure)
	{
		/* check that the media transport is present. */
		if (!has_media_transport && !reconfigure)
		{
			*error_category = AVDTP_SERVICE_MEDIA_TRANSPORT;
			return FALSE;
		}

		/* check that there is a codec present. */
		if (!has_codec)
		{
			*error_category = AVDTP_SERVICE_MEDIA_CODEC;
			return FALSE;
		}
	}

	/* go to the end without any errors. */
	return TRUE;
}


/****************************************************************************
NAME	
	avdtpFindCodecSpecificInformation

DESCRIPTION
	Finds the next codec block in a list of caps.
	Passed pointer and size are updated to point to the search result.
	IMPORTANT: It assumes the basic structure of the caps is valid. Call
	avdtpValidateServiceCaps() first to make sure.

RETURNS
	void
*/
static boolean Avdtp_Find_Codec_Specific_Information(const u_int8 **caps, u_int16 *caps_size)
{
	if (caps_size != NULL)
	{
		while (*caps_size != 0)
		{
			u_int8 service = (*caps)[0];
			u_int8 losc = (*caps)[1];

			if (service == AVDTP_SERVICE_MEDIA_CODEC)
				return TRUE;

			/* move to next entry */
			*caps += 2 + losc;
			*caps_size -= 2 + losc;
		}
		return FALSE;
	}
	else
	{
		while ((*caps)[0] != AVDTP_SERVICE_MEDIA_CODEC)
			*caps += 2 + (*caps)[1];

		return TRUE;
	}
}


/****************************************************************************
NAME	
	avdtpAreServicesCategoriesCompatible

DESCRIPTION
    Checks the Services requested in a SET_CONFIG or RECONFIGURE command
    are supported by the local SEP.  It only checks for the Service entry
    and DOES NOT validate the actual service capabilities - that should
    be done by other functions e.g. gavdpFindMatchingCodecSpecificInformation

	IMPORTANT: It assumes the basic structure of the caps is valid. Call
	avdtpValidateServiceCaps() first to make sure.

RETURNS
	TRUE if OK, FALSE is Configuration contains entry not in local caps.
*/
static boolean Avdtp_Are_Services_Categories_Compatible(const u_int8 *local_caps, u_int16 local_caps_size,
                                        const u_int8 *config_caps, u_int16 config_caps_size, u_int8 *unsupported_service)
{
	u_int16 i,j;

	/* loop through configuration */
	for(i=0;i<config_caps_size;i+=config_caps[i+1]+2)
	{
		u_int8 service = config_caps[i];
		boolean match = FALSE;

		/* check entry is in local caps */
		for(j=0;!match && j<local_caps_size;j+=local_caps[j+1]+2)
		{
			/* compare local service to caps */
			if (local_caps[j] == service)
				match = TRUE;
		}

		/* didn't find Service in local caps, fail */
		if (!match)
		{
			/* report unsupported cap */
			*unsupported_service = service;
				return FALSE;
		}
	}

	return TRUE;
}


/****************************************************************************
NAME	
	avdtpFindMatchingCodecSpecificInformation

DESCRIPTION
	Returns pointer to start of codec specific information if
	the local and remote codecs are compatible.

	IMPORTANT: It assumes the basic structure of the caps is valid. Call
	avdtpValidateServiceCaps() first to make sure.

RETURNS
	void
*****************************************************************************/
static const u_int8* Avdtp_Find_Matching_Codec_Specific_Information(const u_int8 *local_caps, const u_int8 *remote_caps, boolean initiating)
{
	const u_int8 *local_codec = local_caps;
	const u_int8 *remote_codec = remote_caps;

	/* find the codec specific info in both caps */
	if (!Avdtp_Find_Codec_Specific_Information(&local_codec,NULL))
		return NULL;

	if (!Avdtp_Find_Codec_Specific_Information(&remote_codec,NULL))
		return NULL;

	/* check they are the same type */
	if ( (local_codec[2] == remote_codec[2]) && /* media type */
		 (local_codec[3] == remote_codec[3])) /* media codec */
	{
		/* we have a matching codec, now check the fields */
		if (local_codec[2] == (AVDTP_MEDIA_TYPE_AUDIO<<2))
		{
			switch (local_codec[3])
			{
				case AVDTP_MEDIA_CODEC_SBC:
					/* check SBC codecs are compatible */
					if (areSBCCodecsCompatible(local_codec+4, remote_codec+4, local_codec[1], remote_codec[1], initiating))
						return remote_codec;
					break;

				case AVDTP_MEDIA_CODEC_MPEG1_2_AUDIO:
					/* check MPEG-1,2 Audio codecs are compatible. */
					if (areMPEG12AudioCodecsCompatible(local_codec+4,remote_codec+4,local_codec[1],remote_codec[1], initiating))
						return remote_codec;
					break;

				case AVDTP_MEDIA_CODEC_MPEG2_4_AAC:
				case AVDTP_MEDIA_CODEC_ATRAC:
				case AVDTP_MEDIA_CODEC_NONA2DP:
				default:
					/* unknown - default to accepting new codecs */
					return remote_codec;
			}
		}
		else
		{
			/* unknown - default to accepting new codecs */
			return remote_codec;
		}
	}

	return NULL;
}


/******************************************************************
*
*        Returns true if the bit mask passed in only has a single bit set 
*
******************************************************************/
static boolean bitMaskOk(u_int16 bit_mask)
{
	u_int16 test_bit = 0;	
	
	for (test_bit = bit_mask; test_bit != 0; test_bit>>=1)
	{
		if (test_bit & 1)
		{
			if (test_bit == 1)
				return 1;
			else
				return 0;
		}
	}
	return 0;
}

/******************************************************************
	Determines if SBC Codecs from both devices are compatible.

	SBC Codec Specific Information
		Octet 0	Bits 4-7	Sampling Frequency
				Bits 0-4	Channel Mode
		Octet 1	Bits 4-7	Block Length
				Bits 2-3	Subbands
				Bite 0-1	Allocation Method
		Octet 2				Minimum Bitpool Value
		Octet 3				Maximum Bitpool Value 
******************************************************************/
static boolean areSBCCodecsCompatible(const u_int8 *local_caps, const u_int8 *remote_caps, u_int8 local_losc, u_int8 remote_losc, boolean initiating)
{
	/* check length so we don't read off end of buffer */
	if (local_losc < 6)
		return FALSE;

	if (remote_losc < 6)
		return FALSE;
	
	/* do sampling frequency bits overlap? */
	if ( !((local_caps[0] >> 4) & (remote_caps[0] >> 4)))
		return FALSE;

	/* make sure only a single bit is set for the sampling frequency */
	if (!bitMaskOk((remote_caps[0] >> 4)) && !initiating)
		return FALSE;
	
	/* do channel mode bits overlap? */
	if ( !((local_caps[0] & 15) & (remote_caps[0] & 15)))
		return FALSE;

	/* make sure only a single bit is set for the channel mode */
	if (!bitMaskOk(remote_caps[0] & 0xf) && !initiating)
		return FALSE;
	
	/* do Block Length bits overlap? */
	if ( !((local_caps[1] >> 4) & (remote_caps[1] >> 4)))
		return FALSE;

	/* make sure only a single bit is set in the block length mask */
	if (!bitMaskOk((remote_caps[1] >> 4)) && !initiating)
		return FALSE;
	
	/* do Subbands bits overlap? */
	if ( !(((local_caps[1] >> 2) & 3) & ((remote_caps[1] >> 2) & 3)))
		return FALSE;

	/* make sure only a single bit is set in the subbands mask */
	if (!bitMaskOk(((remote_caps[1] >> 2) & 0x3)) && !initiating)
		return FALSE;

	/* do Allocation Method bits overlap? */
	if ( !((local_caps[1] & 3) & (remote_caps[1] & 3)))
		return FALSE;

	/* make sure only a single bit is set in the allocation method mask */
	if (!bitMaskOk((remote_caps[1] & 0x3)) && !initiating)
		return FALSE;

	/* check Min/Max Bitpool Values are in range */
	if (initiating)	
	{
		/* local min is greater than remote max */
		if (local_caps[2] > remote_caps[3])
			return FALSE;
	
		/* local max is less than remote min */
		if (remote_caps[2] > local_caps[3])
			return FALSE;
	}
	else
	{
		/* remote max greater than local max */
		if (remote_caps[3] > local_caps[3])
			return FALSE;

		/* remote min less than local mit */
		if (remote_caps[2] < local_caps[2])
			return FALSE;
	}

	/* match */
	return TRUE;
}

/*****************************************************************
	Determines if MPEG-1,2 Audio codecs from both devices are compatible.

	Codec Specific Information
		Octet0	Bits 5-7	Layer
				Bits 4		CRC
				Bits 0-3	Channel Mode
		Octet1	Bits 7		RFA
				Bits 6		MPF
				Bits 0-5	Sampling Frequency
		Octet2	Bits 7		VBR
				Bits 0-6	Bit Rate
		Octet3				Bit Rate
*******************************************************************/
static boolean areMPEG12AudioCodecsCompatible(const u_int8 *local_caps, const u_int8 *remote_caps, u_int8 local_losc, u_int8 remote_losc, boolean initiating)
{
	u_int16 local_bit_rate;
	u_int16 remote_bit_rate;

	/* check length so we don't read off end of buffer */
	if (local_losc < 6)
		return FALSE;

	if (remote_losc < 6)
		return FALSE;

	/* layer bits overlap? */
	if ( !((local_caps[0] >> 5) & (remote_caps[0] >> 5)))
		return FALSE;

	/* make sure only a single bit is set in the layer mask */
	if (!bitMaskOk((remote_caps[0] >> 5)) && !initiating)
		return FALSE;

	/* CRC protection is optional, so ignore. */

	/* Channel Mode bits overlap? */
	if ( !((local_caps[0] & 0x0f) & (remote_caps[0] & 0x0f)))
		return FALSE;

	/* make sure only a single bit is set in the channel mode mask */
	if (!bitMaskOk((remote_caps[0] & 0x0f)) && !initiating)
		return FALSE;

	/* If MPF is set, codec must still support it being unset, so ignore. */

	/* Sampling Frequency bits overlap? */
	if ( !((local_caps[1] & 0x1f) & (remote_caps[1] & 0x1f)))
		return FALSE;

	/* make sure only a single bit is set in the sampling frequency mask */
	if (!bitMaskOk((remote_caps[1] & 0x1f)) && !initiating)
		return FALSE;

	/* VBR is optional, so ignore */

	/* Bit Rate bits overlap? */
	local_bit_rate = ((local_caps[2] & 0x7f) << 8) | (local_caps[3] & 0xff);
	remote_bit_rate = ((remote_caps[2] & 0x7f) << 8) | (remote_caps[3] & 0xff);

	if (!(local_bit_rate & remote_bit_rate))
		return FALSE;

	/* match */
	return TRUE;
}


/****************************************************************************
NAME	
	Avdtp_Process_Command

DESCRIPTION
	process incoming command from remote device, and make response.
	
RETURNS
	void
****************************************************************************/
static void Avdtp_Process_Command(u_int8* pdu,t_channelId cid, u_int16 packet_size, t_bdaddr bdaddr)
{
	t_DataBuf *h_buff=NULL;
	u_int8 SignalId=(pdu[1] & 0x3f);
	
	//h_buff.buf = resp;
	
	pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Process_Command SignalId=%d", SignalId));
	if (!Validate_Command_PDU(pdu, packet_size))
	{
		u_int32 ret;
		ret =L2_GetWriteBuffer(cid, 48, &h_buff);	/* Build response - assume it will always be less than 48 bytes and hence fit in a single L2CAP packet. */
		if (ret != BT_NOERROR)
		{
		 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
			return ;
		}

		h_buff->buf[0] = (pdu[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
		h_buff->buf[1] = (packet_size > 1?pdu[1]:0);
		h_buff->buf[2] = (u_int8) avdtp_bad_length;
		h_buff->dataLen  = 3;
		
		L2_Write(cid, h_buff);
		//L2_FreeWriteBuffer(h_buff);
		return;
	}
	
	switch(SignalId)
	{
		case AVDTP_DISCOVER:
			Avdtp_Discover_Command(pdu,cid);
			break;
			
		case AVDTP_GET_CAPABILITIES:
			Avdtp_Getcapabilities_Command(pdu,cid);
			break;
			
		case AVDTP_SET_CONFIGURATION:
			Avdtp_Setconfigration_Command(pdu,cid,packet_size);
			break;

		case AVDTP_GET_CONFIGURATION:
			Avdtp_Getconfiguration_Command(pdu,cid);
			break;

		case AVDTP_RECONFIGURE:
			Avdtp_Reconfigure_Command(pdu,cid,packet_size);
			break;

		case AVDTP_OPEN:
			Avdtp_Open_Command(pdu,cid);
			
			/*Maybe here we shall start the avrcp request later*/
			//get the addr form the ps key, because we should store the pairing addr into ps key. 
			//Avrcp_Connect_Req(bdaddr);
			break;

		case AVDTP_START:
			Avdtp_Start_Command(pdu,cid,packet_size);
			
			break;

		case AVDTP_CLOSE:
			Avdtp_Close_Command(pdu,cid);
			break;

		case AVDTP_SUSPEND:
			Avdtp_Suspend_Command(pdu,cid,packet_size);
			break;

		case AVDTP_ABORT:
		//	Avdtp_Abort_Command(pdu,cid);
			break;		

		case ABDTP_SECURITY_CONTROL:
			Avdtp_Security_Control_Command(pdu,cid);
			break;
			
		default:
			{
				u_int32 ret;
				ret = L2_GetWriteBuffer(cid, 48, &h_buff);	/* Build response - assume it will always be less than 48 bytes and hence fit in a single L2CAP packet. */
				if (ret != BT_NOERROR)
				{
				 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
					return ;
				}

				h_buff->buf[0] = (pdu[0] & 0xf0);
				h_buff->buf[1] = (u_int8) AVDTP_NULL;
				h_buff->dataLen  = 2;
				
				L2_Write(cid, h_buff);
				//L2_FreeWriteBuffer(h_buff);
			}
			break;
	}
}

/****************************************************************************
NAME	
	validateResponsePDU

DESCRIPTION
	This function is called to validate a response PDU
	
RETURNS
	void
*****************************************************************************/
static boolean Validate_Response_PDU(const u_int8* ptr,u_int16 packet_size)
{
	if (packet_size < 2)
		/* must contain at least a header and signal ID */
		return FALSE;

	return TRUE;
}
#ifndef COOLSAND

static boolean Avdtp_Is_Sep_InUse(const sep *sep_entry)
{
	u_int8 i = 0;
	sep *e ;
	
	if (sep_entry->state != AVDTP_SEP_STATE_IDLE)
		return 1;
	
	for(i=0;i<2;i++)
	{
		e=& gAvdtp.sep_list_head[i];
		/* Check if SEP is in use */
		if((e != sep_entry) && 
			(e->state != AVDTP_SEP_STATE_EMPTY) && 
			 e->resource_id && (e->resource_id == sep_entry->resource_id) &&
			(e->state != AVDTP_SEP_STATE_IDLE)
		    )
			return 1;
	}

	return 0;	
}


/****************************************************************************
	Helper functions for storing SEIDs discovered present in remote
	device.  Assumes each ID is unique and hence corresponds
	to a bitfield.
****************************************************************************/
#ifndef  COOLSAND
static void Reset_Discovered_Seids(sep* sep_entry)
{
	pMemset(sep_entry->discovered_seids,0,sizeof(u_int16)*NUM_SEID_PACKED_WORDS);
}

static void Add_Discovered_Seid(sep* sep_entry,u_int8 id)
{
	/* set a bit in the appropriate word */
	sep_entry->discovered_seids[id >> 4] |= 1 << (id & 0xF);
}

static u_int8 Pop_Discovered_Seid(sep* sep_entry)
{
	u_int8 b,w;
	/* search for bit */
	for(w=0;w<NUM_SEID_PACKED_WORDS;w++)
	{
		u_int16 words = sep_entry->discovered_seids[w];
		for(b=0;b<16;b++)
		{
			if (words & 1)
			{
				/* found - clear the bit and return SEID */
				sep_entry->discovered_seids[w] &= ~((u_int16)1 << b);
				return b + w*16;
			}
			words>>=1;
		}
	}
	return 0;
}
#endif
#endif

/****************************************************************************
NAME	
	handleDiscoverResponse

DESCRIPTION
	Handle Discover response from remote device.
	
RETURNS
	void
****************************************************************************/
static void Avdtp_Discover_Response(sep* sep_entry, const u_int8* ptr, u_int16 packet_size, u_int16 cid)
{
#ifdef COOLSAND
    u_int8 resp[48];
    pDebugPrintfEX((pLOGDEBUG, pLOGA2DP,"Avdtp_Discover_Response packet_size=%d, ptr[0-5]=0x%x,0x%x,0x%x,0x%x,0x%x,0x%x",packet_size, ptr[0], ptr[1], ptr[2], ptr[3],ptr[4], ptr[5]));
    pMemset(resp, 0, sizeof(resp));
    resp[0] = packet_size;
    pMemcpy(&resp[1], (void*)ptr,packet_size);
    rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SEND_SEP_DISCOVER_CNF,resp);

#else
        u_int16 i;
        /* we only expect this if we are the initiator in the discovering state */
        if (sep_entry->state != AVDTP_SEP_STATE_DISCOVERING)
            return;
    
        /*
        A valid response contains ACCEPT and at least one entry.
        */
        if (((ptr[0] & 3) == AVDTP_MESSAGE_TYPE_ACCEPT) && (packet_size > 2))
        {
            /*
            We must have been the initiator 
            Loop through response looking for potential matches
            and for those that match, read back the capabilities.
            */
            /* clear record of valid SEP */
            Reset_Discovered_Seids(sep_entry);
            ptr+=2;

            for(i=0;i<(packet_size-2);i+=2)
            {
                u_int8 media_type = ptr[1] >> 4;
                u_int8 tsep = (ptr[1] >> 3) & 1;

                /* check that the SEP is not in use, the media type matches and also that the SEP
                type is the opposite. */
                if (!(ptr[0] & 2) &&
                        (media_type == (u_int16)sep_entry->media_type) &&
                        (tsep == ((u_int16)sep_entry->role^1)))
                {
                    /* store */
                    Add_Discovered_Seid(sep_entry,ptr[0]>>2);
                }

                /* move the next result */
                ptr+=2;
            }
            sep_entry->sep_count = i+1;
            /* read back first seid */
            sep_entry->remote_seid = Pop_Discovered_Seid(sep_entry);
            if (sep_entry->remote_seid != 0)	
            {
                /* start reading caps from the remote SEPs */
                Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_READING_CAPS);
                (void)Aavdp_Send_GetCapabilities(sep_entry, sep_entry->remote_seid, cid);
                return;
            }
        }

	/* device does not have matching endpoint so stop */
	//Avdp_Reset_Sep(sep_entry, gavdp_fail);
#endif

}


/**************************************************************************/
void Avdtp_Select_Optimal_Sbc_Caps_Source(u_int8 codec, const u_int8 *local_caps,u_int8 *remote_caps)
{
	if(codec==AVDTP_MEDIA_CODEC_SBC)
	{
		if(remote_caps[6]&0xf0)  //remote support all the sample
			remote_caps[6]&= 0x2f;  //default choose 44.1KHZ

		if(remote_caps[6]&0x0f)  //remote support all channel mode
			remote_caps[6]&= 0xf1;  //default joint stereo

		if(remote_caps[7]&0xf0)  //remote support all block length
			remote_caps[7]&= 0x1f;  //default 16

		if(remote_caps[7]&0x0c)  
			remote_caps[7]&= 0xf7;  //default 8 subband

		if(remote_caps[7]&0x03)  
			remote_caps[7]&= 0xfe;  //default SNR
	}
	else if(codec==AVDTP_MEDIA_CODEC_MPEG1_2_AUDIO)
	{
		//layers supported: mp3, CRC protection supported
        
		if(remote_caps[6]&0x0f)  //remote support all channel mode
			remote_caps[6]&= 0xf1;  //default joint stereo
		
		if(remote_caps[7]&0x07)  //remote support sample frequency
			remote_caps[7]&= 0x02;  //default 44100
		
		remote_caps[8] &= 0x82;  //bit rate: 1001 
		remote_caps[9] &= 0x00;
	
	}
}

void Avdtp_Select_Optimal_Codec(sep *sep_entry)
{
	u_int8 codec =0;
    if(sep_entry->role==SEP_SOURCE)
    {
		codec = sep_entry->local_service_caps[5];
		Avdtp_Select_Optimal_Sbc_Caps_Source(codec, sep_entry->local_service_caps, sep_entry->configured_service_caps);
    }
}

/****************************************************************************************
NAME	
	Start_Building_Configuration

DESCRIPTION
	Starts to build the SET_CONFIGURATION/RECONFIGURE request based on the local and remote caps
	
RETURNS
	void
******************************************************************************************/
#ifndef COOLSAND
static boolean Start_Building_Configuration(sep *sep_entry, const u_int8 *remote_caps, boolean reconfig)
{
    u_int8 tmp[48];
    const u_int8 *remote_codec=tmp;
    u_int8 config[48];
    u_int16 config_write_offset = 0;
    u_int16 config_size = 0;
    u_int8 seid_types=0;

#if 0    
    sep *tmp_sep;
    u_int8 matched=0;
#endif
	if (!reconfig)
	{
		config_size = 2;
		/* fill in media transport as it is always present */
		config[config_write_offset++] = AVDTP_SERVICE_MEDIA_TRANSPORT;
		config[config_write_offset++] = 0; /* LOSC is always zero */

		/*
			TODO - Recovery Configuration.
		*/

		/*
			TODO - Reporting Configuration.
		*/
	}
	else
		/* reconfig doesn't have media transport */
		config_size = 0;

	/*
		Codec Configuration.
	*/
	remote_codec = Avdtp_Find_Matching_Codec_Specific_Information(sep_entry->local_service_caps, remote_caps, 1);
	if (!remote_codec)
	{
		/* could find matching codec */
		return FALSE;
	}
	pMemcpy(&config[2], (void*)remote_codec, 2 + remote_codec[1] );

	if(config_size + 2 + remote_codec[1]>10)
		return FALSE;

	pMemcpy(sep_entry->configured_service_caps, config, config_size + 2 + remote_codec[1]);
	//sep_entry->configured_service_caps = config;
	sep_entry->configured_service_caps_size = config_size + 2 + remote_codec[1];
    seid_types = sep_entry->local_service_caps[3];

    /*select the optimal codec*/
    Avdtp_Select_Optimal_Codec(sep_entry);

	return TRUE;
}
#endif

/****************************************************************************
NAME	
	Avdtp_GetCapabilities_Response

DESCRIPTION
	Handle Get Capabilities response from remote device.
	
RETURNS
	void
****************************************************************************/
static void Avdtp_GetCapabilities_Response(sep* sep_entry, const u_int8* ptr, u_int16 packet_size, u_int16 cid)
{
#ifdef COOLSAND
    u_int8 resp[48];
    u_int8 error_cat, error_code;
    
    /* check the service capabilities are valid */
    if (!Avdtp_Validate_Service_Caps(&ptr[2], packet_size-2, FALSE, FALSE, &error_cat, &error_code))
        return;

    pMemset(resp, 0, sizeof(resp));
    pMemcpy(resp, (void*)ptr, packet_size);
//    sep_cbs.SepGetCapabilitiesConf(resp);
    rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SEND_SEP_GET_CAPABILITIES_CNF,resp);

 #else
	u_int8 error_cat, error_code;

	/* we only expect this if we are the initiator in the discovering state */
	if ((sep_entry->state != AVDTP_SEP_STATE_READING_CAPS) &&
		(sep_entry->state != AVDTP_SEP_STATE_RECONFIG_READING_CAPS))
		return;

	if (((ptr[0] & 3) == AVDTP_MESSAGE_TYPE_ACCEPT && (packet_size > 2)))
	{
		/* check the service capabilities are valid */
		if (!Avdtp_Validate_Service_Caps(&ptr[2], packet_size-2, FALSE, FALSE, &error_cat, &error_code))
		{
			Avdtp_Reset_Sep(sep_entry, avdtp_fail);
			return;
		}

		if (sep_entry->state == AVDTP_SEP_STATE_READING_CAPS)
		{
			/* change state (stop watchdog) */
			Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_PROCESSING_CAPS);

			/*
			Try to build the SET_CONFIGURATION command.
			*/
			if (!Start_Building_Configuration( sep_entry, &ptr[2], FALSE))
			{
				/* Caps not compatible.
				Is there another SEID to try? */
				sep_entry->remote_seid = Pop_Discovered_Seid(sep_entry);
				if (sep_entry->remote_seid != 0)
				{
					/* re-enter the state to reset watchdog */
					Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_READING_CAPS);
					(void)Aavdp_Send_GetCapabilities(sep_entry, sep_entry->remote_seid, cid);
					return;
				}

				/* oh dear, no match and we've run out of SEPs to query */
				//Avdtp_Reset_Sep(sep_entry, avdtp_fail);
				//didn't find matched codec, try another sep.
				sep_entry->state = AVDTP_SEP_STATE_IDLE;
				Avdtp_Start_Discover(gAvdtp.signal_cid,1);
				return;
			}
		}
		else
		{
			/* change state (stop watchdog) */
			Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_RECONFIG_PROCESSING_CAPS);

			/* try building RECONFIGURE */
			if (!Start_Building_Configuration(sep_entry, &ptr[2], TRUE))
			{
				/* caps aren't compatible */
				//Send_Configure_Cfm(gavdp, sep_entry, avdtp_service_caps_incompatible);

				/* back to open */
				Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_OPEN);
			}
			else
			{
				Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_RECONFIGURING);
			}
		}

	}
	else
	{
		if (sep_entry->state == AVDTP_SEP_STATE_READING_CAPS)
		{
			/* request was rejected - abort */
			Avdtp_Reset_Sep(sep_entry, avdtp_fail);
		}
		else
		{
			/* reconfiguration was rejected */
			//Send_Configure_Cfm(sep_entry, avdtp_rejected_by_remote_device);

			/* back to open */
			Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_OPEN);
		}
	}

	//Found the matching codec information, and set the configuration
	Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_CONFIGURING);
	Avdtp_Send_SetConfiguration(sep_entry);
#endif    
}


/****************************************************************************
NAME	
	Avdtp_SetConfiguration_Response

DESCRIPTION
	Handle Set configure response from remote device.
	
RETURNS
	void
****************************************************************************/
static void Avdtp_SetConfiguration_Response(sep* sep_entry, const u_int8* ptr, u_int16 packet_size)
{
#ifdef COOLSAND
    u_int8 resp[4];
    pMemset(resp, 0, sizeof(resp));
    pMemcpy(resp, (void*)ptr, packet_size);
    gIsConfigured = 1;
    rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SEND_SEP_SET_CONFIG_CNF,resp);
#else
	/* we only expect this if we are the initiator in the discovering state */
	if (sep_entry->state != AVDTP_SEP_STATE_CONFIGURING)
		return;

	if (((ptr[0] & 3) == AVDTP_MESSAGE_TYPE_ACCEPT && (packet_size >= 2)))
	{
		/*
			We are now configured, so try and open the link.
		*/
		Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_LOCAL_OPENING);
		Avdtp_Send_Open(sep_entry);
		/* process return value */
	}
	else
	{
		/* request was rejected - abort */
		Avdtp_Reset_Sep(sep_entry, avdtp_fail);

		//packet_size = packet_size;
	}
#endif
}


/****************************************************************************
NAME	
	Avdtp_Reconfigure_Response

DESCRIPTION
	Handle Reconfigure response from remote device.
	
RETURNS
	void
****************************************************************************/

static void Avdtp_Reconfigure_Response(sep* sep_entry, const u_int8* ptr, u_int16 packet_size)
{
#ifdef COOLSAND
    u_int8 resp[4];
    gIsConfigured = 1;
    pMemset(resp, 0, sizeof(resp));
    pMemcpy(resp, (void*)ptr, packet_size);
	pDebugPrintfEX((pLOGERROR, pLOGA2DP,"reconfig response resp[0-3]=0x%x,0x%x,0x%x,0x%x",resp[0],resp[1],resp[2],resp[3]));
    rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SEND_SEP_SET_RECONFIG_CNF,resp);
#else
	/* we only expect this if we are the initiator in the discovering state */
	if (sep_entry->state != AVDTP_SEP_STATE_RECONFIGURING)
		return;

	if (((ptr[0] & 3) == AVDTP_MESSAGE_TYPE_ACCEPT && (packet_size > 2)))
	{
		/* tell application the good news! */
		//sendConfigureCfm(gavdp, sep_entry, gavdp_success);What shall we do??

		/* We now return to the open state. */
		Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_OPEN);
	}
	else
	{
		/* failed */
		//sendConfigureCfm(gavdp, sep_entry, gavdp_rejected_by_remote_device);What shall we do??

		/* move to the reconfig failure state as the application
		   must try again. */
		Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_RECONFIGURE_FAILURE);
	}

	packet_size = packet_size;
#endif
}


/****************************************************************************
NAME	
	Avdtp_Open_Response

DESCRIPTION
	Handle open response from remote device.
	
RETURNS
	void
****************************************************************************/
static void Avdtp_Open_Response(sep* sep_entry, const u_int8* ptr,t_bdaddr bdaddr, u_int16 packet_size)
{
#ifdef COOLSAND
    u_int8 resp[4];
    pMemset(resp, 0, sizeof(resp));
    pMemcpy(resp, (void*)ptr, packet_size);
//    sep_cbs.SepOpenConf(resp);
    rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SEND_SEP_OPEN_CNF,resp);

#else
	/* we only expect this if we are the initiator in the discovering state */
	if (sep_entry->state != AVDTP_SEP_STATE_LOCAL_OPENING)		
		return;

	if ((ptr[0] & 3) == AVDTP_MESSAGE_TYPE_ACCEPT)
	{
		/* 
			On acceptance of AVDTP_OPEN, the initiator shall open the transport 
			channels. We know there must always be a media channel, so start 
			by opening it! 
		*/
		 L2_Connect(bdaddr, AVDTP_PSM);
	}
	else
	{
		/* request was rejected - abort */
		Avdtp_Abort_Sep(sep_entry, avdtp_fail);
	}
#endif
}


/****************************************************************************
NAME	
	Avdtp_Start_Response

DESCRIPTION
	Handle start response from remote device.
	
RETURNS
	void
****************************************************************************/
static void Avdtp_Start_Response(sep* sep_entry, const u_int8* ptr, u_int16 packet_size)
{
#ifdef COOLSAND
    u_int8 resp[4];
    pMemset(resp, 0, sizeof(resp));
    pMemcpy(resp, (void*)ptr, packet_size);
    //sep_cbs.SepStartConf(resp);
        rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SEND_SEP_START_CNF,resp);

#else
    if ((sep_entry->state != AVDTP_SEP_STATE_OPEN) && (sep_entry->state != AVDTP_SEP_STATE_STREAMING))
    {
        return;
    }

    if ((ptr[0] & 3) == AVDTP_MESSAGE_TYPE_ACCEPT)
    {
        /* tell application the good news! */
        //sendStartCfm(gavdp, sep_entry->media_sink, avdtp_success);

        /* end point is now streaming */
        Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_STREAMING);
    }
    else
    {
        /* pass on failure */
        //sendStartCfm(gavdp, sep_entry->media_sink, avdtp_rejected_by_remote_device);
    }
#endif
}


/****************************************************************************
NAME	
	Avdtp_Close_Response

DESCRIPTION
	Handle close response from remote device.
	
RETURNS
	void
****************************************************************************/
extern void Avdtp_Set_First_Conection(u_int8 flag);
extern u_int8 Close_resp[4];
static void Avdtp_Close_Response(sep* sep_entry, const u_int8* ptr, u_int16 packet_size)
{
#ifdef COOLSAND
 pDebugPrintfEX((pLOGERROR, pLOGA2DP,"Avdtp_Close_Response is called"));
#if 0
    u_int8 resp[4];
    u_int8 status=BT_NOERROR;
    pMemset(resp, 0, sizeof(resp));
    pMemcpy(resp, (void*)ptr, packet_size);*/
#else
    u_int8 status=BT_NOERROR;
    pMemset(Close_resp, 0, sizeof(Close_resp));
    pMemcpy(Close_resp, (void*)ptr, packet_size);
#endif
    status=L2_Disconnect(gAvdtp.media_cid);
    //rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SEND_SEP_CLOSE_CNF,resp);
    //Avdtp_Set_First_Conection(0);
#else

    if ((ptr[0] & 3) == AVDTP_MESSAGE_TYPE_ACCEPT)
    {
        /* tell application the good news! */
        //sendStartCfm(gavdp, sep_entry->media_sink, avdtp_success);

        /* end point is now streaming */
        Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_LOCAL_CLOSING);
    }
    else
    {
        /* pass on failure */
        //sendStartCfm(gavdp, sep_entry->media_sink, avdtp_rejected_by_remote_device);
    }
#endif
}


/****************************************************************************
NAME	
	Avdtp_Suspend_Response

DESCRIPTION
	Handle suspend response from remote device.
	
RETURNS
	void
****************************************************************************/
static void Avdtp_Suspend_Response(sep* sep_entry, const u_int8* ptr, u_int16 packet_size)
{
#ifdef COOLSAND
    u_int8 resp[2];
    pMemset(resp, 0, sizeof(resp));
    pMemcpy(resp, (void*)ptr, packet_size);
   // sep_cbs.SepSuspendConf(resp);
     pDebugPrintfEX((pLOGERROR, pLOGA2DP,"Avdtp_Suspend_Response is call will send A2DP_SEND_SEP_PAUSE_CNF"));
    rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SEND_SEP_PAUSE_CNF,resp);

#else

    if ((ptr[0] & 3) == AVDTP_MESSAGE_TYPE_ACCEPT)
    {
        /* tell application the good news! */
        //sendStartCfm(gavdp, sep_entry->media_sink, avdtp_success);

        /* end point is now streaming */
        Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_LOCAL_SUSPENDING);
    }
    else
    {
        /* pass on failure */
        //sendStartCfm(gavdp, sep_entry->media_sink, avdtp_rejected_by_remote_device);
    }
#endif
}


/****************************************************************************
NAME	
	Avdtp_Abort_Response

DESCRIPTION
	Handle abort response from remote device.
	
RETURNS
	void
****************************************************************************/
static void Avdtp_Abort_Response(sep* sep_entry, const u_int8* ptr, u_int16 packet_size)
{
#ifdef COOLSAND
    u_int8 resp[2];
    u_int8 status=BT_NOERROR;
    pMemset(resp, 0, sizeof(resp));
    pMemcpy(resp, (void*)ptr, packet_size);
    //Release the L2cap channel for media cid.
    status=L2_Disconnect(gAvdtp.media_cid);
    //sep_cbs.SepAbortConf(resp);
    rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_SEND_SEP_ABORT_CNF,resp);
#else

    if ((ptr[0] & 3) == AVDTP_MESSAGE_TYPE_ACCEPT)
    {
        /* tell application the good news! */
        //sendStartCfm(gavdp, sep_entry->media_sink, avdtp_success);

        /* end point is now streaming */
        Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_LOCAL_SUSPENDING);
    }
    else
    {
        /* pass on failure */
        //sendStartCfm(gavdp, sep_entry->media_sink, avdtp_rejected_by_remote_device);
    }
#endif
}

/****************************************************************************/


sep *Avdp_Get_Sep_From_TransactionLabel(u_int8 transaction_label)
{
    sep *this_sep = gAvdtp.sep_list_head;
    u_int8 count=0;
	
    while (this_sep && count<2)  //only support 2 stream endpoint.
    {
        if((this_sep->state != AVDTP_SEP_STATE_EMPTY) && (this_sep->pending_transaction_label == transaction_label))
            return this_sep;

        /* Get thie next SEP */
        this_sep ++;
        count++;
    }
	
    return NULL;
}

/****************************************************************************
NAME	
	Avdtp_Process_Response

DESCRIPTION
	process the response from the remote device according to the signal type.
	
RETURNS
	void
****************************************************************************/
static void Avdtp_Process_Response(u_int8* pdu,t_channelId cid, u_int16 packet_size, t_bdaddr bdaddr)
{
#ifdef COOLSAND

#if pDEBUG
    u_int8 tmp[48]={0,};
    pMemcpy(tmp, pdu, packet_size);
#endif	

    /* check the PDU is reasonable */
    if (!Validate_Response_PDU(pdu, packet_size))
        return;

#if pDEBUG
    	pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Process_Response: signal ID=%d, cid=0x%02X, packet_size=%d\n",(pdu[1]&0x3f),cid,packet_size));
    	//pDebugDump(pLOGA2DP,tmp,packet_size);
#endif

    switch (pdu[1]&0x3f)
    {
        case AVDTP_DISCOVER:
            Avdtp_Discover_Response(NULL, pdu, packet_size, cid);
        break;

        case AVDTP_GET_CAPABILITIES:
            Avdtp_GetCapabilities_Response( NULL, pdu, packet_size, cid);
        break;

        case AVDTP_SET_CONFIGURATION:
            Avdtp_SetConfiguration_Response(NULL, pdu, packet_size);
        break;

        case AVDTP_GET_CONFIGURATION:
            /* We never send the request, so don't expect a reply! */
        break;

        case AVDTP_RECONFIGURE:
            Avdtp_Reconfigure_Response(NULL, pdu, packet_size);
        break;

        case AVDTP_OPEN:
            Avdtp_Open_Response(NULL, pdu, bdaddr,packet_size);
        break;

        case AVDTP_START:
            Avdtp_Start_Response(NULL, pdu, packet_size);
        break;

        case AVDTP_CLOSE:
            Avdtp_Close_Response(NULL, pdu, packet_size);
        break;

        case AVDTP_SUSPEND:
            Avdtp_Suspend_Response(NULL, pdu, packet_size);
        break;

        case AVDTP_ABORT:
            Avdtp_Abort_Response(NULL, pdu, packet_size);
        break;

        default:
            break;
    }
    
#else

    sep *sep_entry=Avdp_Get_Sep_From_TransactionLabel(pdu[0] >> 4);
#if pDEBUG
    u_int8 tmp[48]={0,};
    pMemcpy(tmp, pdu, packet_size);
#endif	

	if (sep_entry == NULL)
		return;

	/* clear pending transaction as we have a response */
	sep_entry->pending_transaction_label = 0;

	/* check the PDU is reasonable */
	if (!Validate_Response_PDU(pdu, packet_size))
		return;

#if pDEBUG
	pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Process_Response: signal ID=%d, cid=0x%02X, packet_size=%d\n",(pdu[1]&0x3f),cid,packet_size));
	pDebugDump(pLOGA2DP,tmp,packet_size);
#endif    

	switch (pdu[1]&0x3f)
	{
		case AVDTP_DISCOVER:
			Avdtp_Discover_Response(sep_entry, pdu, packet_size, cid);
			break;

		case AVDTP_GET_CAPABILITIES:
			Avdtp_GetCapabilities_Response( sep_entry, pdu, packet_size, cid);
			break;

		case AVDTP_SET_CONFIGURATION:
			Avdtp_SetConfiguration_Response( sep_entry, pdu, packet_size);
			break;

		case AVDTP_GET_CONFIGURATION:
			/* We never send the request, so don't expect a reply! */
			break;

		case AVDTP_RECONFIGURE:
			Avdtp_Reconfigure_Response(sep_entry, pdu, packet_size);
			break;

		case AVDTP_OPEN:
			Avdtp_Open_Response(sep_entry, pdu, bdaddr, packet_size);
			break;

		case AVDTP_START:
			Avdtp_Start_Response(sep_entry, pdu, packet_size);
			break;

		case AVDTP_CLOSE:
			Avdtp_Close_Response(sep_entry, pdu, packet_size);
			break;

		case AVDTP_SUSPEND:
			Avdtp_Suspend_Response(sep_entry, pdu, packet_size);
			break;

		case AVDTP_ABORT:
			Avdtp_Abort_Response(sep_entry, pdu, packet_size);
			break;

		default:
			break;
	}
#endif    
}


/****************************************************************************
NAME	
	Avdtp_Process_BadHeader

DESCRIPTION
	get the bad header pud, so send response to note.
	
RETURNS
	void
****************************************************************************/
static void Avdtp_Process_BadHeader(u_int8* pdu,t_channelId cid, u_int16 packet_size)
{
	t_DataBuf *h_buff;
	u_int32 ret;
	ret = L2_GetWriteBuffer(cid, 10, &h_buff);
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return;
	}

	/* reject header */
	h_buff->buf[0] = (h_buff->buf[0] & 0xf0) | AVDTP_MESSAGE_TYPE_REJECT;
	h_buff->buf[1] = (packet_size > 1 ? h_buff->buf[1]:0);
	h_buff->buf[2] = (u_int8) avdtp_bad_header_format;
	h_buff->dataLen = 3;
	L2_Write(cid, h_buff);
	//L2_FreeWriteBuffer(h_buff);
}


/***********************************************************************
 *
 * FUNCTION NAME:   avdtp_request_dispatcher
 * DESCRIPTION:
 
 This function will send the pdu to the correct decoder dependant on the
 message type.

 * KNOWN ISSUES:
 
 An invalid command is just ignored.

 *
 *
 ***********************************************************************/
void Avdtp_Request_Dispatcher(u_int8* pdu,t_channelId cid, u_int16 packet_size, t_AVDTP_TYPE type, t_bdaddr bdaddr)
{
	//t_bdaddr bdaddr;
	//u_int8 resp[48]; 	/* Build response - assume it will always be less than 48 bytes and hence fit in a single L2CAP packet. */
	//struct st_t_dataBuf h_buff;

	if(type==AVDTP_SIGNAL)
	{
		switch(pdu[0] & 3)
		{
			case AVDTP_MESSAGE_TYPE_COMMAND:
				Avdtp_Process_Command(pdu, cid, packet_size,bdaddr);
				break;

			case AVDTP_MESSAGE_TYPE_ACCEPT:
			case AVDTP_MESSAGE_TYPE_REJECT:
				Avdtp_Process_Response(pdu, cid, packet_size,bdaddr);
				break;

			default:
				Avdtp_Process_BadHeader(pdu, cid, packet_size);
			break;
			
		}
	}
	else  //avdtp_media
	{
		//TO DO LATER.........
		//get the media data, and send it to the hardware decode.	
	}


}


/****************************************************************************
NAME	
	Avdtp_Handle_Suspend_Req

DESCRIPTION
	Request to suspend streaming .
	
RETURNS
	void
****************************************************************************/
void Avdtp_Handle_Suspend_Req(void)
{
	sep *this_sep;
	u_int8 active_seid = gAvdtp.active_seid;
	
	if(active_seid == 0)
		return;

	active_seid-=1;
	this_sep = &gAvdtp.sep_list_head[active_seid];

	if(this_sep->state != AVDTP_SEP_STATE_STREAMING)
		return;

	Avdtp_Change_Sep_State(this_sep, AVDTP_SEP_STATE_LOCAL_SUSPENDING);

	if(!Avdtp_Send_Suspend(this_sep))//Can't send close  - abort 
		Avdtp_Abort_Sep(this_sep,avdtp_fail);
}


/****************************************************************************
NAME	
	Avdtp_Handle_Start_Req

DESCRIPTION
	Request to begin streaming on a previously established connection.
	
RETURNS
	void
****************************************************************************/
void Avdtp_Handle_Start_Req(void)
{

}

/****************************************************************************
NAME	
	Avdtp_Handle_Close_Req

DESCRIPTION
	Request the connection be released.
	
RETURNS
	void
****************************************************************************/
void Avdtp_Handle_Close_Req(void)
{

}


/****************************************************************************
NAME	
	Avdtp_Get_BdAddr

DESCRIPTION
	get the current avdtp address.
	
RETURNS
	void
****************************************************************************/
boolean Avdtp_Get_BdAddr(t_bdaddr *addr)
{
	if(addr)
	{
		pMemcpy(addr->bytes, gAvdtp.bd_addr.bytes,sizeof(addr->bytes));
		return TRUE;
	}
	
	return FALSE;
}

/****************************************************************************
NAME	
    Avdtp_Get_Next_TransactionLabel

DESCRIPTION
    This function returns the next transaction label.  This is used to route
	responses from the remote device to the correct local initiator.  Note
	that zero is a special case, indicating no command is pending

RETURNS
	Transaction label
*****************************************************************************/
static u_int8 Avdtp_Get_Next_TransactionLabel(void)
{
#ifdef COOLSAND
	gTransaction_Label = (gTransaction_Label + 1) & 0xf;
	
	if(gTransaction_Label== 0)
		gTransaction_Label = 1;

#if pDEBUG
    //pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Get_Next_TransactionLabel gTransaction_Label:%d\n",gTransaction_Label));
#endif    
	
	return gTransaction_Label;
#else
	gAvdtp.transaction_label = (gAvdtp.transaction_label + 1) & 0xf;
	
	if(gAvdtp.transaction_label == 0)
		gAvdtp.transaction_label = 1;
	
	return gAvdtp.transaction_label;
#endif    
}

/*****************************************************************************/
#if 0
static void Avdtp_Set_SepTo_Idle(sep* sep_entry)
{
	pMemset(sep_entry->configured_service_caps, 0, sizeof(sep_entry->configured_service_caps ));
	sep_entry->configured_service_caps_size = 0;

	sep_entry->pending_transaction_label = 0;

	Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_IDLE);
}
#endif
static void Avdtp_Reset_Sep(sep* sep_entry, u_int8 status)
{
	
}


/****************************************************************************
NAME	
	Avdtp_Abort_Sep

DESCRIPTION
	abort from the sep

RETURNS
	void
*****************************************************************************/
static void Avdtp_Abort_Sep(sep* sep_entry, u_int8 status)
{
	if ((sep_entry->state == AVDTP_SEP_STATE_LOCAL_ABORTING) ||
		(sep_entry->state == AVDTP_SEP_STATE_REMOTE_ABORTING))
		// already aborting 
		return;

	// change to aborting 
	Avdtp_Change_Sep_State(sep_entry, AVDTP_SEP_STATE_LOCAL_ABORTING);
		
	// send the abort command 
	if (!Avdtp_Send_Abort(sep_entry))
		Avdtp_Reset_Sep(sep_entry, status);
}


/****************************************************************************
NAME	
	Avdtp_Send_Discover

DESCRIPTION
	Send discover request.

RETURNS
	void
*****************************************************************************/
boolean Avdtp_Send_Discover(sep* sep_entry, u_int16 cid)
{
	t_DataBuf *h_buff;
	u_int8 status=0;
	u_int32 ret;
	ret = L2_GetWriteBuffer(cid, 10, &h_buff);
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ret;
	}
    
#ifdef COOLSAND	
        h_buff->buf[0] = (Avdtp_Get_Next_TransactionLabel() << 4) | AVDTP_MESSAGE_TYPE_COMMAND;
#else
	if (sep_entry->pending_transaction_label)
	/* command is already pending. */
		return FALSE;

	/* get a transaction ID and store it to match with the response. */
	sep_entry->pending_transaction_label = Avdtp_Get_Next_TransactionLabel();
     
	h_buff->buf[0] = (sep_entry->pending_transaction_label << 4) | AVDTP_MESSAGE_TYPE_COMMAND;
#endif

	h_buff->buf[1] = (u_int8) AVDTP_DISCOVER;
	h_buff->dataLen = 2;
    
#if pDEBUG
    //pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Send_Command: discover seid=0x%02X, buf[0-1]=0x%x,0x%x\n",cid,h_buff->buf[0],h_buff->buf[1]));
#endif  

	status=L2_Write(cid, h_buff);
	if(status==BT_NOERROR)
		return TRUE;
	
	return FALSE;
}

/****************************************************************************
NAME	
	Aavdp_Send_GetCapabilities

DESCRIPTION
	Send get capabilities request.

RETURNS
	void
*****************************************************************************/
boolean Aavdp_Send_GetCapabilities(sep* sep_entry, u_int8 seid, u_int16 cid)
{
    t_DataBuf *h_buff;
    u_int8 status=0;
	u_int32 ret;
    ret = L2_GetWriteBuffer(cid, 10, &h_buff);
	if (ret != BT_NOERROR)
	{
		pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
	return ret;
	}

#ifdef COOLSAND 
    h_buff->buf[0] = (Avdtp_Get_Next_TransactionLabel() << 4) | AVDTP_MESSAGE_TYPE_COMMAND;
#else
    if (sep_entry->pending_transaction_label)
    /* command is already pending. */
        return FALSE;

    /* get a transaction ID and store it to match with the response. */
    sep_entry->pending_transaction_label = Avdtp_Get_Next_TransactionLabel();

    h_buff->buf[0] = (sep_entry->pending_transaction_label << 4) | AVDTP_MESSAGE_TYPE_COMMAND;
#endif    

    h_buff->buf[1] = (u_int8) AVDTP_GET_CAPABILITIES;
    h_buff->buf[2] = seid << 2;
    h_buff->dataLen = 3;

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Send_Command: GetCapabilities seid=%d, buf[0-2]=0x%x,0x%x,0x%x\n",seid,h_buff->buf[0],h_buff->buf[1],h_buff->buf[2]));
#endif  

    status=L2_Write(cid, h_buff);
    if(status==BT_NOERROR)
        return TRUE;

    return FALSE;
}


/****************************************************************************
NAME	
	Avdtp_Send_SetConfiguration

DESCRIPTION
	Combines the pass codec configuration with the store SEP configuration and
	sends a SET_CONFIGURATION command to the SEP.

RETURNS
	void
*****************************************************************************/
#ifdef COOLSAND
boolean Avdtp_Send_SetConfiguration(u_int16 connect_id, u_int8 acp_seid, u_int8 int_seid, u_int8 codec_type,bt_a2dp_audio_codec_cap_struct* audio_cap)
{
    t_DataBuf *h_buff;
    u_int8 status=0;
    u_int16 bit_rate =0;
    u_int8 bitpool=32;


   status= L2_GetWriteBuffer(connect_id, 48, &h_buff);

    if (status!=BT_NOERROR || h_buff == NULL)
    {
#if pDEBUG
       pDebugPrintf((pLOGERROR,"Avdtp_Send_SetConfiguration : Error - Memory could not be allocated for L2CAP Packet. status =%d\n",status));
#endif  
        return BT_NORESOURCES; //insufficent resource
    }

    
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Send_SetConfiguration connect_id=0x%x,acp_seid:0x%x,int_seid:%x,codec_type:%d",connect_id,acp_seid,int_seid,codec_type));
#endif

    gAvdtp.active_seid = acp_seid; //store the HS acp seid.
    h_buff->buf[0] = (Avdtp_Get_Next_TransactionLabel() << 4) | AVDTP_MESSAGE_TYPE_COMMAND;
    h_buff->buf[1] = (u_int8) AVDTP_SET_CONFIGURATION;
    h_buff->buf[2] = acp_seid << 2;
    h_buff->buf[3] = int_seid<< 2;
    h_buff->buf[4] =  AVDTP_SERVICE_MEDIA_TRANSPORT;  //service category: media transport
    h_buff->buf[5] = 0; //length of service capability(LOSC:0)
    h_buff->buf[6] =  AVDTP_SERVICE_MEDIA_CODEC;   //service category: media codec
    h_buff->buf[7] =  6;  //length of service capabilty(LOSC:6)
    h_buff->buf[8] =  AVDTP_MEDIA_TYPE_AUDIO; //media type: audio

    if(codec_type == A2DP_SBC)
    {   
        pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"SetConfiguration:sample_rate:%x,max_bit_pool:%d,gSBC_MAX_Bit_Pool:%d",audio_cap->sbc.sample_rate,audio_cap->sbc.max_bit_pool,gSBC_MAX_Bit_Pool));
        gActive_codec_type = A2DP_SBC;
        //memcpy(&gCur_Audio_Cap.sbc, audio_cap->sbc, sizeof(bt_a2dp_sbc_codec_cap_struct) );
        h_buff->buf[9] =  AVDTP_MEDIA_CODEC_SBC;
        h_buff->buf[10] = ((audio_cap->sbc.sample_rate&0x0F)<<4)   //sample frequency
                                    |audio_cap->sbc.channel_mode;               //channel mode
        h_buff->buf[11] = ((audio_cap->sbc.block_len & 0x0F)<<4)   //block length
                                    | ((audio_cap->sbc.subband_num & 0x03)<<2)  //subband
                                    | (audio_cap->sbc.alloc_method &0x03);            //alloc method
        h_buff->buf[12] = audio_cap->sbc.min_bit_pool;
        
        if(audio_cap->sbc.max_bit_pool >gSBC_MAX_Bit_Pool)
        {
            h_buff->buf[13] = gSBC_MAX_Bit_Pool;
            bitpool = gSBC_MAX_Bit_Pool;
        }
        else
        {
            h_buff->buf[13] =  audio_cap->sbc.max_bit_pool;
            bitpool = audio_cap->sbc.max_bit_pool;
        }

        {  //calculate the frame length.
            u_int8 subbands=8;
            u_int8 channels = 2;
            u_int8 blocks = 16;
            u_int8 bIsJoint = 0;
            
            switch(audio_cap->sbc.subband_num)
            {
                case 1:
                    subbands = 8;
                    break;

                case 2:
                    subbands = 4;
                break;

                default:
                    subbands = 8;
            }

            switch(audio_cap->sbc.block_len)
            {
                case 1:
                    blocks = 16;
                    break;

                case 2:
                    blocks = 12;
                    break;

                case 4:
                    blocks = 8;
                break;

                case 8:
                    blocks = 4;
                break;

                default:
                    blocks = 16;
            }

            switch(audio_cap->sbc.channel_mode)
            {
                case 1: //joint stereo
                    bIsJoint = 1;
                    channels = 2;
                    break;
                    
                case 2:  //stereo
                case 4:  //dual channel
                    channels = 2;
                break;
                    
                 case 8:  //mono
                    channels = 1;
                break;
            }


            if(audio_cap->sbc.channel_mode & 0xC)  //mono, dual channel
            {
                gCurMaxFrameLen = 4+(4*subbands*channels)/8 +(blocks*channels*bitpool/8);
            }
            else //stereo, joint stereo
            {
                 gCurMaxFrameLen = 4+(4*subbands*channels)/8 +(bIsJoint*subbands+blocks*bitpool)/8;
            }
            pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Send_SetConfiguration gCurMaxFrameLen:%d",gCurMaxFrameLen));
        }
    }
    else if(codec_type == A2DP_MPEG_AUDIO)
    {
        gActive_codec_type = A2DP_MPEG_AUDIO;
        //memcpy(&gCur_Audio_Cap.mp3, audio_cap->mp3, sizeof(bt_a2dp_mp3_codec_cap_struct) );
        bit_rate = audio_cap->mp3.bit_rate;
        
#if 0//pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"layer:0x%x, CRC:0x%x, Ch_mode:0x%x, MPF:0x%x, sampel_rate:0x%x, VBR:0x%x, bit_rate:0x%x\n",
                                                                    audio_cap->mp3.layer, audio_cap->mp3.CRC, 
                                                                    audio_cap->mp3.channel_mode,audio_cap->mp3.MPF,
                                                                    audio_cap->mp3.sample_rate, audio_cap->mp3.VBR, 
                                                                    audio_cap->mp3.bit_rate));

#endif

        h_buff->buf[9] =  AVDTP_MEDIA_CODEC_MPEG1_2_AUDIO;
        h_buff->buf[10] = ((audio_cap->mp3.layer &0x07)<<5)            //layer
                                    |((audio_cap->mp3.CRC &0x01 )<<4)          //CRC protection
                                    |(audio_cap->mp3.channel_mode &0x0F);  //channel mode
                                    
        h_buff->buf[11] = (audio_cap->mp3.MPF &0x01)<<7    //RFA:0
                                    |(audio_cap->mp3.sample_rate&0x3F);  //sample frequency
                                    
        h_buff->buf[12] = ((audio_cap->mp3.VBR &0x01)<<7)  //VBR supported
                                    | ((bit_rate>>8)& 0x7F);                                           //bit rate
        
        h_buff->buf[13] = (u_int8) (bit_rate & 0xFF);                           //bit rate
    }
#if pDEBUG      
        pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"SetConfiguration buf[0-13]=0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x, 0x%x,0x%x,0x%x,0x%x \n", 
                                                                        h_buff->buf[0],h_buff->buf[1],  h_buff->buf[2], h_buff->buf[3],h_buff->buf[4],  h_buff->buf[5],h_buff->buf[6],  h_buff->buf[7], h_buff->buf[8],h_buff->buf[9],  h_buff->buf[10],h_buff->buf[11],  h_buff->buf[12], h_buff->buf[13]));
#endif 
    h_buff->dataLen = 14;

    status=L2_Write(connect_id,h_buff);
    if(status==BT_NOERROR)
        return TRUE;
	
    return FALSE;
}
#else
gfhdf
boolean Avdtp_Send_SetConfiguration(sep* sep_entry)
{
	t_DataBuf *h_buff;
	u_int8 status=0;
	u_int32 ret;
	ret =L2_GetWriteBuffer(gAvdtp.signal_cid, 48, &h_buff);
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ret;
	}

	if (sep_entry->pending_transaction_label)
		/* command is already pending. */
		return FALSE;

	/* get a transaction ID and store it to match with the response. */
	sep_entry->pending_transaction_label = Avdtp_Get_Next_TransactionLabel();

	h_buff->buf[0] = (sep_entry->pending_transaction_label << 4) | AVDTP_MESSAGE_TYPE_COMMAND;
	h_buff->buf[1] = (u_int8) AVDTP_SET_CONFIGURATION;
	h_buff->buf[2] = sep_entry->remote_seid << 2;
	h_buff->buf[3] = sep_entry->seid << 2;
	pMemcpy(&h_buff->buf[4], sep_entry->configured_service_caps, sep_entry->configured_service_caps_size);
	h_buff->dataLen = 4+sep_entry->configured_service_caps_size;

        if(sep_entry->configured_service_caps[5] == A2DP_MPEG_AUDIO)
            gActive_codec_type = A2DP_MPEG_AUDIO;
        else if(sep_entry->configured_service_caps[5] == A2DP_SBC)
            gActive_codec_type = A2DP_SBC;

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Send_Command: SetConfiguration h_buff->dataLen=%d,sep_entry->seid=%d\n",h_buff->dataLen,sep_entry->seid));
    pDebugDump(pLOGA2DP,h_buff->buf,h_buff->dataLen);
#endif  
    
	status=L2_Write(gAvdtp.signal_cid,h_buff);
	if(status==BT_NOERROR)
		return TRUE;
	
	return FALSE;
}
#endif


/****************************************************************************
NAME	
	Avdtp_Send_Reconfigure

DESCRIPTION
	Combines the pass codec configuration with the store SEP configuration and
	sends a RECONFIGURE command to the SEP.

RETURNS
	void
*/
#ifdef COOLSAND
boolean Avdtp_Send_Reconfigure(u_int16 connect_id, u_int8 acp_seid, u_int8 codec_type,bt_a2dp_audio_codec_cap_struct* audio_cap)
{
    t_DataBuf *h_buff;
    u_int8 status=0;
    u_int16 bit_rate =0;
    u_int32 ret;
    ret =L2_GetWriteBuffer(connect_id, 48, &h_buff);
    	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ret;
	}
	
#if pDEBUG
    pDebugPrintfEX((pLOGERROR,pLOGA2DP,"Avdtp_Send_Reconfigure gSBC_MAX_Bit_Pool=0x%x,connect_id=0x%x,acp_seid:0x%x,codec_type:%d, gIsConfigured:%d",gSBC_MAX_Bit_Pool,connect_id,acp_seid,codec_type,gIsConfigured));
#endif

    gAvdtp.active_seid = acp_seid;
    h_buff->buf[0] = (Avdtp_Get_Next_TransactionLabel() << 4) | AVDTP_MESSAGE_TYPE_COMMAND;
    h_buff->buf[1] = (u_int8) AVDTP_RECONFIGURE;
    h_buff->buf[2] = acp_seid << 2;
    h_buff->buf[3] =  AVDTP_SERVICE_MEDIA_CODEC;  //service category: media transport
   // h_buff->buf[4] = 0; //length of service capability(LOSC:0)
   // h_buff->buf[5] =  AVDTP_SERVICE_MEDIA_CODEC;   //service category: media codec
    h_buff->buf[4] =  6;  //length of service capabilty(LOSC:6)
    h_buff->buf[5] =  AVDTP_MEDIA_TYPE_AUDIO; //media type: audio

    if(codec_type == A2DP_SBC)
    {
	#if pDEBUG
    pDebugPrintfEX((pLOGERROR,pLOGA2DP,"sbc sample_rate%d, channel_mode%d, subband_num%d, alloc_method%d, min_bit_pool %d,max_bit_pool%d,",\
    audio_cap->sbc.sample_rate,\
    audio_cap->sbc.channel_mode,\
    audio_cap->sbc.subband_num,\
    audio_cap->sbc.alloc_method,\
    audio_cap->sbc.min_bit_pool,\
    audio_cap->sbc.max_bit_pool
    ));
#endif
	gActive_codec_type = A2DP_SBC;
        h_buff->buf[6] =  AVDTP_MEDIA_CODEC_SBC;
        h_buff->buf[7] = ((audio_cap->sbc.sample_rate&0x0F)<<4)   //sample frequency
                                    |audio_cap->sbc.channel_mode;               //channel mode
        h_buff->buf[8] = ((audio_cap->sbc.block_len & 0x0F)<<4)   //block length
                                    | ((audio_cap->sbc.subband_num & 0x03)<<2)  //subband
                                    | (audio_cap->sbc.alloc_method &0x03);            //alloc method
        h_buff->buf[9] = audio_cap->sbc.min_bit_pool;
       // h_buff->buf[10] =  audio_cap->sbc.max_bit_pool;
		
         if(audio_cap->sbc.max_bit_pool >32)
        {
            h_buff->buf[10] = gSBC_MAX_Bit_Pool;//32;
        }
        else
        {
            h_buff->buf[10] =  audio_cap->sbc.max_bit_pool;
        }

		
    }
    else if(codec_type == A2DP_MPEG_AUDIO)
    {

	#if pDEBUG
    pDebugPrintfEX((pLOGERROR,pLOGA2DP,"mp3 layer=%d,CRC=%d,channel_mode=%d,MPF=%d,sample_rate=%d,VBR=%d",\
    audio_cap->mp3.layer,\
    audio_cap->mp3.CRC,\
    audio_cap->mp3.channel_mode,\
    audio_cap->mp3.MPF ,\
    audio_cap->mp3.sample_rate,\
    audio_cap->mp3.VBR
    ));
#endif
	gActive_codec_type = A2DP_MPEG_AUDIO;
        bit_rate = audio_cap->mp3.bit_rate;
        
        h_buff->buf[6] =  AVDTP_MEDIA_CODEC_MPEG1_2_AUDIO;
        h_buff->buf[7] = ((audio_cap->mp3.layer &0x07)<<5)            //layer
                                    |((audio_cap->mp3.CRC &0x01 )<<4)          //CRC protection
                                    |(audio_cap->mp3.channel_mode &0x0F);  //channel mode
                                    
        h_buff->buf[8] = (audio_cap->mp3.MPF &0x01)<<7    //RFA:0
                                    |(audio_cap->mp3.sample_rate&0x3F);  //sample frequency
                                    
        h_buff->buf[9] = ((audio_cap->mp3.VBR &0x01)<<7)  //VBR supported
                                    | ((bit_rate>>8)& 0x7F);                                           //bit rate
        
        h_buff->buf[10] = (u_int8) (bit_rate & 0xFF);                           //bit rate

    
    }
    h_buff->dataLen = 11;
#if pDEBUG      
        pDebugPrintfEX((pLOGERROR,pLOGA2DP,"A2DP_MPEG_AUDIO h_buff->buf[6-10]=0x%x,0x%x,0x%x,0x%x,0x%x \n", 
                                                                        h_buff->buf[6],  h_buff->buf[7],h_buff->buf[8],  h_buff->buf[9], h_buff->buf[10]));
#endif     
    status=L2_Write(connect_id,h_buff);
    if(status==BT_NOERROR)
        return TRUE;
	
    return FALSE;
}
#else
boolean Avdtp_Send_Reconfigure(sep* sep_entry, u_int8* config, u_int16 config_size)
{
	t_DataBuf *h_buff;
	u_int8 status=0;
	u_int32 ret;
	ret = L2_GetWriteBuffer(gAvdtp.signal_cid, 48, &h_buff);
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ret;
	}

	/* write the codec configuration to the end of our configuration */
	pMemcpy(sep_entry->configured_service_caps+sep_entry->configured_service_caps_size-config_size, config, config_size);


	if (sep_entry->pending_transaction_label)
		/* command is already pending. */
		return FALSE;

	/* get a transaction ID and store it to match with the response. */
	sep_entry->pending_transaction_label = Avdtp_Get_Next_TransactionLabel();

	h_buff->buf[0] = (sep_entry->pending_transaction_label << 4) | AVDTP_MESSAGE_TYPE_COMMAND;
	h_buff->buf[1] = (u_int8) AVDTP_RECONFIGURE;
	h_buff->buf[2] = sep_entry->remote_seid << 2;
	pMemcpy(&h_buff->buf[3],sep_entry->configured_service_caps,sep_entry->configured_service_caps_size);
	h_buff->dataLen = 3+sep_entry->configured_service_caps_size;

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Send_Command: Reconfigure config_size=0x%d, h_buff->dataLen=%d\n",config_size,h_buff->dataLen));
    pDebugDump(pLOGA2DP,h_buff->buf,h_buff->dataLen);
#endif  

      status=L2_Write(gAvdtp.signal_cid,h_buff);
	if(status==BT_NOERROR)
		return TRUE;
	
	return FALSE;
}
#endif


/****************************************************************************
NAME	
	Avdtp_Send_Open

DESCRIPTION
	Send request to open streaming.

RETURNS
	void
****************************************************************************/
#ifdef COOLSAND
boolean Avdtp_Send_Open(u_int8 acp_seid)
{
    t_DataBuf *h_buff;
    u_int8 status=0;
    u_int32 ret;
    ret = L2_GetWriteBuffer(gAvdtp.signal_cid, 48, &h_buff);
    //gAvdtp.active_seid = acp_seid;
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ret;
	}

    h_buff->buf[0] = (Avdtp_Get_Next_TransactionLabel() << 4) | AVDTP_MESSAGE_TYPE_COMMAND;
    h_buff->buf[1] = (u_int8) AVDTP_OPEN;
    h_buff->buf[2] = acp_seid << 2;
    h_buff->dataLen = 3;

#if pDEBUG
    //pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Send_Command: Open signal_cid=0x%02X, acp_seid=%d\n",gAvdtp.signal_cid,acp_seid));
#endif  

    status=L2_Write(gAvdtp.signal_cid,h_buff);
    if(status==BT_NOERROR)
        return TRUE;

    return FALSE;
}
#else
boolean Avdtp_Send_Open(sep* sep_entry)
{
	t_DataBuf *h_buff;
	u_int8 status=0;
	u_int32 ret;
	ret = L2_GetWriteBuffer(gAvdtp.signal_cid, 48, &h_buff);
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ret;
	}
    
	if (sep_entry->pending_transaction_label)
		/* command is already pending. */
		return FALSE;

	/* get a transaction ID and store it to match with the response. */
	sep_entry->pending_transaction_label = Avdtp_Get_Next_TransactionLabel();

	h_buff->buf[0] = (sep_entry->pending_transaction_label << 4) | AVDTP_MESSAGE_TYPE_COMMAND;
	h_buff->buf[1] = (u_int8) AVDTP_OPEN;
	h_buff->buf[2] = sep_entry->remote_seid << 2;
	h_buff->dataLen = 3;

#if pDEBUG
        //pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Send_Command: Open signal_cid=0x%02X, h_buff->dataLen=%d\n",gAvdtp.signal_cid,h_buff->dataLen));
	//pDebugDump(pLOGA2DP,h_buff->buf,h_buff->dataLen);
#endif  

	status=L2_Write(gAvdtp.signal_cid,h_buff);
	if(status==BT_NOERROR)
		return TRUE;
	
	return FALSE;
}
#endif


/****************************************************************************
NAME	
	Avdtp_Send_Start

DESCRIPTION
	Send request to start streaming.

RETURNS
	void
****************************************************************************/
boolean Avdtp_Send_Start(sep* sep_entry)
{
    t_DataBuf *h_buff;
    u_int8 status=0;
     status= L2_GetWriteBuffer(gAvdtp.signal_cid, 48, &h_buff);

     if (status!=BT_NOERROR || h_buff == NULL)
    {
#if pDEBUG
       pDebugPrintf((pLOGERROR,"Avdtp_Send_Start : Error - Memory could not be allocated for L2CAP Packet. status =%d\n",status));
#endif  
        return BT_NORESOURCES; //insufficent resource
    }
	
 #ifdef COOLSAND
    h_buff->buf[0] = (Avdtp_Get_Next_TransactionLabel() << 4) | AVDTP_MESSAGE_TYPE_COMMAND;
    h_buff->buf[1] = (u_int8) AVDTP_START;
    h_buff->buf[2] = gAvdtp.active_seid << 2;
 #else
    if (sep_entry->pending_transaction_label)
    /* command is already pending. */
        return FALSE;

    /* get a transaction ID and store it to match with the response. */
    sep_entry->pending_transaction_label = Avdtp_Get_Next_TransactionLabel();
    h_buff->buf[0] = (sep_entry->pending_transaction_label << 4) | AVDTP_MESSAGE_TYPE_COMMAND;
    h_buff->buf[1] = (u_int8) AVDTP_START;
    h_buff->buf[2] = sep_entry->remote_seid << 2;        
#endif    

	h_buff->dataLen = 3;

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Send_Command: Start signal_cid=0x%x, buf[0-2]=0x%x,0x%x,0x%x\n",gAvdtp.signal_cid,h_buff->buf[0],h_buff->buf[1], h_buff->buf[2]));
#endif  
    
    status=L2_Write(gAvdtp.signal_cid,h_buff);
    if(status==BT_NOERROR)
        return TRUE;
	
    return FALSE;
}


/****************************************************************************
NAME	
	Avdtp_Send_Close

DESCRIPTION
	Send request to close streaming.

RETURNS
	void
****************************************************************************/
boolean Avdtp_Send_Close(sep* sep_entry)
{
    t_DataBuf *h_buff;
    u_int8 status=0;
	u_int32 ret;
    ret = L2_GetWriteBuffer(gAvdtp.signal_cid, 16, &h_buff);
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ret;
	}
    
#ifdef COOLSAND
    h_buff->buf[0] = (Avdtp_Get_Next_TransactionLabel()<< 4) | AVDTP_MESSAGE_TYPE_COMMAND;
    h_buff->buf[1] = (u_int8) AVDTP_CLOSE;
    h_buff->buf[2] =  gAvdtp.active_seid<< 2;
#else
    if (sep_entry->pending_transaction_label)
    /* command is already pending. */
        return FALSE;

    /* get a transaction ID and store it to match with the response. */
    sep_entry->pending_transaction_label = Avdtp_Get_Next_TransactionLabel();

    h_buff->buf[0] = (sep_entry->pending_transaction_label << 4) | AVDTP_MESSAGE_TYPE_COMMAND;
    h_buff->buf[1] = (u_int8) AVDTP_CLOSE;
    h_buff->buf[2] = sep_entry->remote_seid << 2;
#endif    
    h_buff->dataLen= 3;


#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Send_Command: Close signal_cid=0x%02X,  gAvdtp.active_seid=%d\n",gAvdtp.signal_cid, gAvdtp.active_seid));
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"h_buff[0-2]=0x%x, 0x%x, 0x%0x\n", h_buff->buf[0], h_buff->buf[1],h_buff->buf[2]));
#endif  
    //Avdtp_Set_First_Conection(0);
    
    status=L2_Write(gAvdtp.signal_cid,h_buff);
    if(status==BT_NOERROR)
        return TRUE;
	
    return FALSE;
}


/****************************************************************************
NAME	
	Avdtp_Send_Suspend

DESCRIPTION
	Send request to suspend streaming.

RETURNS
	void
****************************************************************************/
boolean Avdtp_Send_Suspend(sep* sep_entry)
{
    t_DataBuf *h_buff=NULL;
    u_int8 status=0;

   status= L2_GetWriteBuffer(gAvdtp.signal_cid, 10, &h_buff);

    if (status!=BT_NOERROR || h_buff == NULL)
    {
#if pDEBUG
        pDebugPrintf((pLOGERROR,"Avdtp_Send_Suspend : Error - Memory could not be allocated for L2CAP Packet. status =%d\n",status));
#endif  
        return BT_NORESOURCES; //insufficent resource
    }
    


#ifdef COOLSAND
    h_buff->buf[0] = (Avdtp_Get_Next_TransactionLabel() << 4) | AVDTP_MESSAGE_TYPE_COMMAND;
    h_buff->buf[1] = (u_int8) AVDTP_SUSPEND;
    h_buff->buf[2] =  gAvdtp.active_seid<< 2;
#else
    if (sep_entry->pending_transaction_label)
    /* command is already pending. */
        return FALSE;

    /* get a transaction ID and store it to match with the response. */
    sep_entry->pending_transaction_label = Avdtp_Get_Next_TransactionLabel();

    h_buff->buf[0] = (sep_entry->pending_transaction_label << 4) | AVDTP_MESSAGE_TYPE_COMMAND;
    h_buff->buf[1] = (u_int8) AVDTP_SUSPEND;
    h_buff->buf[2] = sep_entry->remote_seid << 2;
#endif

    h_buff->dataLen= 3;

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Send_Command: Suspend signal_cid=0x%02X,  gAvdtp.active_seid=%d\n",gAvdtp.signal_cid, gAvdtp.active_seid));
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"buf[0-2]=0x%x,0x%x,0x%x\n",h_buff->buf[0], h_buff->buf[1],h_buff->buf[2]));
#endif  

    status=L2_Write(gAvdtp.signal_cid,h_buff);
    if(status ==BT_NOERROR)
        return TRUE;

    return FALSE;
	
}

/****************************************************************************
NAME	
	Avdtp_Send_Abort

DESCRIPTION
	Send request to Abort an SEP.

RETURNS
	void
****************************************************************************/
boolean Avdtp_Send_Abort(sep* sep_entry)
{
    t_DataBuf *h_buff;
    u_int8 status=0;
	u_int32 ret;
    ret = L2_GetWriteBuffer(gAvdtp.signal_cid, 10, &h_buff);
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ret;
	}

#ifdef COOLSAND
    h_buff->buf[0] = (Avdtp_Get_Next_TransactionLabel()<< 4) | AVDTP_MESSAGE_TYPE_COMMAND;
    h_buff->buf[1] = (u_int8) AVDTP_ABORT;
    h_buff->buf[2] =  gAvdtp.active_seid  << 2;
#else
    /* NOTE: Always send the abort, even if there is another message outstanding. */

    /* get a transaction ID and store it to match with the response. */
    sep_entry->pending_transaction_label = Avdtp_Get_Next_TransactionLabel();

    h_buff->buf[0] = (sep_entry->pending_transaction_label << 4) | AVDTP_MESSAGE_TYPE_COMMAND;
    h_buff->buf[1] = (u_int8) AVDTP_ABORT;
    h_buff->buf[2] = sep_entry->remote_seid << 2;
#endif    
    h_buff->dataLen= 3;

#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"AVDTP-Send_Command: Abort signal_cid=0x%02X, gAvdtp.active_seid =%d\n",gAvdtp.signal_cid,gAvdtp.active_seid));
#endif  

    status=L2_Write(gAvdtp.signal_cid,h_buff);
    if(status==BT_NOERROR)
        return TRUE;

    return FALSE;
}


/****************************************************************************
NAME	
	Avdtp_Start_Discover

DESCRIPTION
	Send request to discover SEP.

RETURNS
	void
****************************************************************************/
void Avdtp_Start_Discover(u_int16 cid,u_int8 seid)
{
#ifdef COOLSAND
    Avdtp_Send_Discover(NULL, cid);
#else
    sep *this_sep= &gAvdtp.sep_list_head[seid];

	/* Update the sep state */
	Avdtp_Change_Sep_State(this_sep, AVDTP_SEP_STATE_DISCOVERING);

	/* Signalling channel open - start discovery */
	if(!Avdtp_Send_Discover(this_sep,cid))
	/* Cannot perform discovery - abort */
		Avdtp_Abort_Sep(this_sep, avdtp_fail);	
#endif    
}

/****************************************************************************
NAME	
	Avdtp_Start_Start

DESCRIPTION
	Send request to start the stram.

RETURNS
	void
****************************************************************************/
void Avdtp_Start_Start(void)
{
#ifdef COOLSAND
    Avdtp_Send_Start(NULL);
#else
	sep *this_sep = &gAvdtp.sep_list_head[0];

	/* Update the sep state */
	Avdtp_Change_Sep_State(this_sep, AVDTP_SEP_STATE_STREAMING);

	if(!Avdtp_Send_Start(this_sep))
	/* Cannot perform discovery - abort */
		Avdtp_Abort_Sep(this_sep, avdtp_fail);	
#endif
}

u_int8 Avdtp_Get_Codec_Type(void)
{
    return gActive_codec_type;
}
extern u_int8 get_a2dp_havebuffer(u_int16 cid,u_int16 len);//zhou siyou091228

int Avdtp_Send_Data(u_int32 length, u_int8* p_data, u_int32 timestamp,u_int16 seq_num, u_int8 payload_type, u_int8 marker )
{
    t_DataBuf* p_writeBuffer=NULL;
    int status =0;
    u_int32 len=length+AVDTP_MEDIA_HEADER_LENGTH; //12 is the length of the avdtp media header
#if pDEBUG
			//pDebugPrintf((pLOGERROR,"Avdtp_Send_Data"));
#endif  
    if(!get_a2dp_havebuffer(gAvdtp.media_cid,len))
     {
#if pDEBUG
        pDebugPrintf((pLOGNOTICE," Error - Memory no Buffer"));
#endif 
        return BT_NORESOURCES;
     }
    status=L2_GetWriteBuffer(gAvdtp.media_cid, len, &p_writeBuffer);  

    if (status!=BT_NOERROR || p_writeBuffer == NULL)
    {
#if pDEBUG
        pDebugPrintf((pLOGERROR,"Avdtp_Send_Data : Error - Memory could not be allocated for L2CAP Packet.  length=%d,status =%d\n",length,status));
#endif  
        return BT_HOSTERROR; //insufficent resource
    }
    
    if(gActive_codec_type==A2DP_MPEG_AUDIO)
        payload_type = 0x61;
    else if(gActive_codec_type==A2DP_SBC)
        payload_type = 0x1;

    p_writeBuffer->buf[0] = 0x80; //RTP Version:2, Padding:No, Header Extension:No, CSRC Count:0;
    p_writeBuffer->buf[1] = (marker&0x01)<<7 |(payload_type&0x7f);
    p_writeBuffer->buf[2] = (seq_num &0xff00)>>8;
    p_writeBuffer->buf[3] = (seq_num &0x00ff);
    p_writeBuffer->buf[4] = (timestamp &0xff000000)>>24;
    p_writeBuffer->buf[5] = (timestamp &0xff0000)>>16;
    p_writeBuffer->buf[6] = (timestamp &0xff00)>>8;
    p_writeBuffer->buf[7] = (timestamp &0xff);
    
    p_writeBuffer->buf[8] = 0x00;  //SSRC always 1;
    p_writeBuffer->buf[9] = 0x00; 
    p_writeBuffer->buf[10] = 0x00;
    p_writeBuffer->buf[11] = 0x01; 

    pMemcpy(&p_writeBuffer->buf[12], p_data, length);
    p_writeBuffer->dataLen = len;//length+AVDTP_MEDIA_HEADER_LENGTH;
    
  //  pDebugPrintf((pLOGNOTICE,"Avdtp_Send_Data gAvdtp.media_cid=%x, length=%d, dataLen=%d\n",gAvdtp.media_cid, length, p_writeBuffer->dataLen));
 
    status = L2_Write(gAvdtp.media_cid, p_writeBuffer);

    return status; 
}


/**********************************************************************************/
/* added for the coolsand platform callbacks*/
/**********************************************************************************/
#ifdef COOLSAND
void AVDTP_Start_Media_Channel_Connection(void)
{
    L2_Connect(gAvdtp.bd_addr, AVDTP_PSM);
}

void Avdtp_Send_Sep_Discover_Res(u_int16 cid, bt_sep_info_struct* sep_list)
{
 //   u_int8 i=0;
    t_DataBuf *h_buff;
	u_int32 ret;
    ret = L2_GetWriteBuffer(cid, 10, &h_buff); //discover response has 6 bytes in our chip, because we only have 2 SEPs,s0 10 bytes is enough.
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ;
	}

    h_buff->buf[0] = (gTransaction_Label <<4)| AVDTP_MESSAGE_TYPE_ACCEPT;  //transaction label, single paket
    h_buff->buf[1] = (u_int8) AVDTP_DISCOVER;

    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Send_Sep_Discover_Res lab=%x",  gTransaction_Label));

    h_buff->buf[2] = sep_list->seid<<2 | sep_list->in_use<<1;
    h_buff->buf[3] = sep_list->media_type<<4| sep_list->sep_type<<3;
    h_buff->dataLen =  4;
    
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"buf[0-4] =0x%x,0x%x,0x%x,0x%x", h_buff->buf[0],h_buff->buf[1],h_buff->buf[2],h_buff->buf[3]));
#endif  
	
    L2_Write(cid, h_buff);
}

void Avdtp_Send_Sep_Capabilities_Get_Res(u_int16 cid, u_int8 codec_type, bt_a2dp_audio_codec_cap_struct* audio_cap)
{
    t_DataBuf *h_buff;
	u_int32 ret;
    ret = L2_GetWriteBuffer(cid, 48, &h_buff); 
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ;
	}

    h_buff->buf[0] = (gTransaction_Label <<4) | AVDTP_MESSAGE_TYPE_ACCEPT;  //transaction label, single paket
    h_buff->buf[1] = (u_int8) AVDTP_GET_CAPABILITIES;

    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Send_Sep_Capabilities_Get_Res lab=%x codec_type=%d", gTransaction_Label, codec_type ));

    h_buff->buf[2] = AVDTP_SERVICE_MEDIA_TRANSPORT;
    h_buff->buf[3] = 0; //LOSC:length of service capability
    h_buff->buf[4] = AVDTP_SERVICE_MEDIA_CODEC;
    h_buff->buf[5] = 6;
    h_buff->buf[6] = AVDTP_MEDIA_TYPE_AUDIO;

    if((codec_type-1) ==AVDTP_MEDIA_CODEC_SBC )  
    {
        pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"sample_rate:0x%x, channel_mode:0x%x, block_len:0x%x,subband_num:0x%x,method:0x%x,min:0x%x,max:0x%x ", audio_cap->sbc.sample_rate, audio_cap->sbc.channel_mode, 
                                audio_cap->sbc.block_len, audio_cap->sbc.subband_num, audio_cap->sbc.alloc_method,audio_cap->sbc.min_bit_pool, audio_cap->sbc.max_bit_pool ));
        h_buff->buf[7] = AVDTP_MEDIA_CODEC_SBC;
        h_buff->buf[8] = audio_cap->sbc.sample_rate<<4 |audio_cap->sbc.channel_mode;
        h_buff->buf[9] = audio_cap->sbc.block_len<<4 |audio_cap->sbc.subband_num<<2|audio_cap->sbc.alloc_method;
        h_buff->buf[10] = audio_cap->sbc.min_bit_pool;
        h_buff->buf[11] = audio_cap->sbc.max_bit_pool;
    }
    else if((codec_type-1) ==AVDTP_MEDIA_CODEC_MPEG1_2_AUDIO )
    {
        h_buff->buf[7] = AVDTP_MEDIA_CODEC_MPEG1_2_AUDIO;
        h_buff->buf[8] = audio_cap->mp3.layer<<5 |audio_cap->mp3.CRC<<4|audio_cap->mp3.channel_mode;
        h_buff->buf[9] = audio_cap->mp3.sample_rate;
        h_buff->buf[10] = audio_cap->mp3.VBR<<7 |((audio_cap->mp3.bit_rate>>8)&0x7f);
        h_buff->buf[11] = audio_cap->mp3.bit_rate & 0xff;
    }
#if pDEBUG      
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Capabilities Res buf[0-11]=0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x, 0x%x,0x%x\n", 
                                                                        h_buff->buf[0],h_buff->buf[1],  h_buff->buf[2], h_buff->buf[3],h_buff->buf[4],  h_buff->buf[5],h_buff->buf[6],  h_buff->buf[7], h_buff->buf[8],h_buff->buf[9],  h_buff->buf[10],h_buff->buf[11]));
#endif 
    h_buff->dataLen =  12;

    L2_Write(cid, h_buff);
}

void Avdtp_Send_Sep_Stream_Config_Res(u_int16 result, u_int8 stream_handle)
{
    t_DataBuf *h_buff;
	u_int32 ret;
    ret = L2_GetWriteBuffer(gAvdtp.signal_cid, 48, &h_buff); 
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ;
	}

    h_buff->buf[0] = (gTransaction_Label <<4) | result;  //transaction label, single paket
    h_buff->buf[1] = (u_int8) AVDTP_SET_CONFIGURATION;

    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Send_Sep_Stream_Config_Res lab=%x,result=%d", gTransaction_Label,result));
   
    h_buff->dataLen =  2;

    L2_Write(gAvdtp.signal_cid, h_buff);
}

void Avdtp_Send_Sep_Stream_Open_Res(u_int16 result, u_int8 stream_handle)
{
    t_DataBuf *h_buff;
    u_int32 ret;
    ret = L2_GetWriteBuffer(gAvdtp.signal_cid, 48, &h_buff); 
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ;
	}

    h_buff->buf[0] = (gTransaction_Label <<4) | result;  //transaction label, single paket
    h_buff->buf[1] = (u_int8) AVDTP_OPEN;

    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Send_Sep_Stream_Open_Res lab=%x,result=%d", gTransaction_Label,result));
   
    h_buff->dataLen =  2;

    L2_Write(gAvdtp.signal_cid, h_buff);
}

void Avdtp_Send_Sep_Stream_Start_Res(u_int16 result, u_int8 stream_handle)
{
    t_DataBuf *h_buff;
	u_int32 ret;
    //gAvdtp.active_seid = stream_handle;
    ret = L2_GetWriteBuffer(gAvdtp.signal_cid, 48, &h_buff); 
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ;
	}
    
    h_buff->buf[0] = (gTransaction_Label <<4) | result;  //transaction label, single paket
    h_buff->buf[1] = (u_int8) AVDTP_START;

    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Send_Sep_Stream_Start_Res lab0x=%x, stream_handle=%d,result=%d", gTransaction_Label, stream_handle,result));
   
    h_buff->dataLen =  2;

    L2_Write(gAvdtp.signal_cid, h_buff);
}

void Avdtp_Send_Sep_Stream_Close_Res(u_int16 result, u_int8 stream_handle)
{
    t_DataBuf *h_buff;
    u_int32 ret;
    ret = L2_GetWriteBuffer(gAvdtp.signal_cid, 48, &h_buff); 
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ;
	}

    h_buff->buf[0] = (gTransaction_Label <<4) | result;  //transaction label, single paket
    h_buff->buf[1] = (u_int8) AVDTP_CLOSE;

    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Send_Sep_Stream_Close_Res lab=%x", gTransaction_Label));
   
    h_buff->dataLen =  2;

    L2_Write(gAvdtp.signal_cid, h_buff);
}

void Avdtp_Send_Sep_Stream_Abort_Res( u_int8 stream_handle)
{
    t_DataBuf *h_buff;
	u_int32 ret;
    ret = L2_GetWriteBuffer(gAvdtp.signal_cid, 48, &h_buff); 
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ;
	}

    h_buff->buf[0] = (gTransaction_Label <<4) | AVDTP_MESSAGE_TYPE_ACCEPT;  //transaction label, single paket
    h_buff->buf[1] = (u_int8) AVDTP_ABORT;

    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Send_Sep_Stream_Abort_Res lab=%x", gTransaction_Label));
   
    h_buff->dataLen =  2;

    L2_Write(gAvdtp.signal_cid, h_buff);
}

void Avdtp_Send_Sep_Stream_Suspend_Res(u_int16 result, u_int8 stream_handle)
{
    t_DataBuf *h_buff;
	u_int32 ret;
    ret = L2_GetWriteBuffer(gAvdtp.signal_cid, 48, &h_buff); 
	if (ret != BT_NOERROR)
	{
	 	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"L2_GetWriteBuffer FAILE IN : %s %d, ret=%d",  __FUNCTION__,__LINE__,ret));
		return ;
	}

    h_buff->buf[0] = (gTransaction_Label <<4) | result;  //transaction label, single paket
    h_buff->buf[1] = (u_int8) AVDTP_SUSPEND;

    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"Avdtp_Send_Sep_Stream_Suspend_Res lab=%x", gTransaction_Label));
   
    h_buff->dataLen =  2;

    L2_Write(gAvdtp.signal_cid, h_buff);
}

#endif
