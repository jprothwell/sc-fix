/***********************************************************************
 *
 * MODULE NAME:   a2dp_register.c
  * DESCRIPTION:   a2dp register
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
#include "sdp_ue.h"

#include "host_ctrl.h"
#include "papi.h"
#include "a2dp.h"
#include "avdtp_callbacks.h"
#include "a2dp_core.h"
#include "avrcp_callbacks.h"
#include "sdp_constants.h"
#include "sdp_db.h"
#include "sdp_server.h"
#include "avdtp_core.h"
#include "avrcp_core.h"
#include "host_msg.h"

t_pTimer A2dp_Senddata_TimerHandle;

A2DP ga2dp; 
u_int8 gIsConfigured = 0;

/* SDP Info */
#if SDP_CLIENT
#define A2DP_MAX_ATTRIBS_NUM 3  //protocol, profile, and supported feature.

u_int32 _patternUUIDs;
t_SDP_SearchPattern a2dp_pattern; 
u_int32 _attributeIDs[A2DP_MAX_ATTRIBS_NUM];
t_SDP_AttributeIDs a2dp_attribs;
t_bdaddr _addresses;
t_SDP_Addresses a2dp_devices;
t_SDP_Results *a2dp_pResults;
t_a2dp_Supported_feature A2DP_Supported_Feature;
typedef void (* rdabt_timer_func) (void*);

extern u_int32 rdabt_put_message(u_int16 q, u_int16 mi, void *mv);
extern u_int32 rdabt_timed_event_in(u_int32 duration, rdabt_timer_func fn, /*uint16_t mi,*/ void *mv);

extern u_int8 get_list_length(u_int8* data,u_int32 *len,u_int8* offset);
extern u_int8 read_uuid(u_int8* data,u_int32* uuid,u_int8* offset);
extern u_int8 gCurMaxFrameLen;
#endif

int a2dp_sdpSearchCompleteCallback(u_int8 status);
t_A2DP_STATE Get_A2dp_State(void)
{
	return ga2dp.state;
}

void A2dp_Set_State(t_A2DP_STATE state)
{
	ga2dp.state = state;
}

/*
void headset_db_init(void)
{
	int status=0;
	status=sdp_init_headset();  //only init the audio sink.
}*/

void A2dp_Init(void)
{
	//int status=0;

	//a2dpSetState(A2dpInitialising);

	//status=sdp_init_A2DP();  //only init the audio sink.
	//if(status)
	//	pDebugPrintfEX((pLOGDEBUG,pLOGA2DP,"A2DP - SDP Record:A2dp init failure"));
	
	A2dp_Set_State(A2dpReady);
}

#ifdef COOLSAND
void rda_bt_a2dp_send_activate_req( u_int8 src_mod_id, u_int8 local_role)
{
    u_int8 result = BT_NOERROR;
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"bt_a2dp_send_activate_req local_role=%d\n",local_role));
#endif     

    if(local_role == AVDTP_SOURCE)
    {
        A2dp_Init();
        Avdtp_Register_Sep(A2DP_MPEG_AUDIO);  //register the SEP of MP3
        Avdtp_Register_Sep(A2DP_SBC);  //register the SEP of SBC.
        rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_ACTIVATE_CNF,&result);
    }
}

void rda_bt_a2dp_send_deactivate_req( u_int8 src_mod_id )
{
    u_int8 result = BT_NOERROR;
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"rda_bt_a2dp_send_deactivate_req \n"));
#endif      
    A2dp_Set_State(A2dpInitialising);
    Avrcp_Set_State(AVRCP_INITIALISING);

    rdabt_put_message(EXTERNAL_TASK_MASK,A2DP_Q|A2DP_DEACTIVATE_CNF,&result);
    
}

#if pDEBUG
extern u_int8 *prh_mgr_printBDAddress(t_bdaddr address) ;
#endif      
u_int8 rda_bt_a2dp_send_signal_connect_req( u_int8 src_mod_id, t_bdaddr device_addr, u_int8 local_role)
{
    u_int8 status=BT_NOERROR;
    
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"rda_bt_a2dp_send_signal_connect_req addr:%s\n",prh_mgr_printBDAddress(device_addr)));
#endif      
    gIsConfigured = 0;
    status=A2DP_Setup_Connect(device_addr);
   return status;
 	
}

void rda_bt_a2dp_send_signal_disconnect_req( u_int8 src_mod_id, u_int16 connect_id  )
{
    u_int8 status=BT_NOERROR;
       
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"rda_bt_a2dp_send_signal_disconnect_req connect_id=%d\n", connect_id));
#endif  

    status = L2_Disconnect(connect_id);

}


void rda_bt_a2dp_send_sep_discover_req( u_int8 src_mod_id, u_int16 connect_id  )
{
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"rda_bt_a2dp_send_sep_discover_req connect_id=%d\n", connect_id));
#endif   

    Avdtp_Start_Discover(connect_id, 0);
   
}

void rda_bt_a2dp_send_capabilities_get_req( u_int8 src_mod_id, u_int16 connect_id, u_int8 acp_seid)
{
    u_int8 status=BT_NOERROR;
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"rda_bt_a2dp_send_capabilities_get_req connect_id=%d acp_seid=%d\n", connect_id,acp_seid));
#endif    
    status =Aavdp_Send_GetCapabilities(NULL, acp_seid, connect_id); 
}

void rda_bt_a2dp_send_stream_config_req(u_int16 connect_id, u_int8 acp_seid, u_int8 int_seid, u_int8 codec_type, bt_a2dp_audio_codec_cap_struct* audio_cap )
{
    u_int8 status=BT_NOERROR;
#if pDEBUG
   //pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"rda_bt_a2dp_send_stream_config_req connect_id=%d acp_seid=%d int_seid=%d,gIsConfigured:%d\n", connect_id, acp_seid, int_seid,gIsConfigured));
#endif   

    //if(!gIsConfigured)
        status=Avdtp_Send_SetConfiguration(connect_id, acp_seid, int_seid,codec_type, audio_cap);
   // else
   //     status=Avdtp_Send_Reconfigure(connect_id, acp_seid, int_seid,codec_type, audio_cap);


}  

void rda_bt_a2dp_send_stream_reconfig_req(u_int16 connect_id, u_int8 acp_seid, u_int8 codec_type, bt_a2dp_audio_codec_cap_struct* audio_cap )
{
	u_int8 status=BT_NOERROR;
#if pDEBUG
	pDebugPrintfEX((pLOGERROR,pLOGA2DP,"rda_bt_a2dp_send_stream_config_req connect_id=%d acp_seid=%d ,gIsConfigured:%d,codec_type=%d\n", connect_id, acp_seid,gIsConfigured,codec_type));
#endif   
	status=Avdtp_Send_Reconfigure(connect_id, acp_seid, codec_type, audio_cap);
}
void rda_bt_a2dp_send_stream_open_req( u_int8 src_mod_id, u_int8 acp_seid)
{
    u_int8 status=BT_NOERROR;
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"rda_bt_a2dp_send_stream_open_req acp_seid=%d \n", acp_seid ));
#endif   

    status=Avdtp_Send_Open(acp_seid);

}  

void rda_bt_a2dp_send_stream_start_req( u_int8 src_mod_id, u_int8 acp_seid )
{
    u_int8 bfirst = Avdtp_Is_First_Conection();
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"rda_bt_a2dp_send_stream_start_req acp_seid=%d bfirst=%d\n", acp_seid,bfirst ));
#endif   
    
    if(bfirst)
        Avdtp_Start_Start();
    else
    {
        Avdtp_Set_SRC_First_Conection_Flag(TRUE);
        AVDTP_Start_Media_Channel_Connection();
    }
}

static u_int32 size_ping=0;
static u_int8 buf_ping[800];
u_int32 gbuf_len;// 659;//773;
static u_int32 seq_num =0;
u_int32 gTimeDuration;//35
static u_int32 get_len=0;
extern void * memset(void *,int,size_t);
volatile u_int8 ReTrySendFlag=0;
void A2dpDataSendTimerCallback(void *pArgs)
{
    u_int32 min, total, sample_cnt=0;
//    u_int32 TimeDuration = 0;
    int status= BT_NOERROR;
    A2DP_codec_struct *codec = (A2DP_codec_struct *)pArgs;

#if pDEBUG
        pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"A2dpDataSendTimerCallback  ReTrySendFla%d\n",ReTrySendFlag ));
#endif 


	if(ReTrySendFlag)
	{
		status=Avdtp_Send_Data(size_ping, buf_ping, 0, seq_num,0, 0);
		if(status==BT_NORESOURCES)
	   	{
	        ReTrySendFlag=TRUE;   
		  if(get_len >gbuf_len )
	            A2dp_Senddata_TimerHandle = rdabt_timed_event_in(gTimeDuration, A2dpDataSendTimerCallback, codec);
       	}
		else
		{
			ReTrySendFlag=FALSE;
			        if(seq_num==0xFFFFFFFF)
            				seq_num=0;
        
       			 seq_num++;
      		  if(get_len >gbuf_len )
        	    A2dp_Senddata_TimerHandle = rdabt_timed_event_in(gTimeDuration, A2dpDataSendTimerCallback, codec);
      		  else
       		 {
         		   if(codec->GetPayloadDone)
               		 codec->GetPayloadDone();
           		 get_len=0;
        		}
		}
	
		return;
	}
    
    codec->QueryPayloadSize(&min, &total);
    get_len = total;
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"A2dpDataSendTimerCallback min:%d, total:%d, size_ping=%d, gbuf_len=%d,get_len=%d \n", min, total, size_ping, gbuf_len, get_len));
    
    if(min<total && size_ping<total  &&  gbuf_len<get_len)
    {
        //pTimerCancel(A2dp_Senddata_TimerHandle);
        A2dp_Senddata_TimerHandle=0;
        
        memset(buf_ping, 0,sizeof(buf_ping));
        size_ping=codec->GetPayload(buf_ping, gbuf_len, &sample_cnt);
    	 pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"A2dpDataSendTimerCallback GetPayload size_ping=%d ,sample_cnt=%d , seq_num=%d\n", size_ping,sample_cnt,seq_num ));
        status=Avdtp_Send_Data(size_ping, buf_ping, 0, seq_num,0, 0);
       if(status==BT_NORESOURCES)
	   {
	   	 ReTrySendFlag=TRUE;
		  if(get_len >gbuf_len )
		     A2dp_Senddata_TimerHandle = rdabt_timed_event_in(gTimeDuration, A2dpDataSendTimerCallback, codec);
		return;
       	}
        get_len-=size_ping;
        
        if(seq_num==0xFFFFFFFF)
            seq_num=0;
        
        seq_num++;
		
        if(get_len >gbuf_len )
            A2dp_Senddata_TimerHandle = rdabt_timed_event_in(gTimeDuration, A2dpDataSendTimerCallback, codec);
        else
        {
            if(codec->GetPayloadDone)
                codec->GetPayloadDone();
            get_len=0;
        }
    }
    else
    {
        //pTimerCancel(A2dp_Senddata_TimerHandle);
        A2dp_Senddata_TimerHandle=0;
    
        if(codec->GetPayloadDone)
            codec->GetPayloadDone();
        get_len = 0;
    } 
}

void rda_bt_a2dp_send_stream_data_send_req( u_int8 src_mod_id, u_int8 stream_handle, A2DP_codec_struct* codec )
{
    u_int32 min, total, sample_cnt=0;
    int status= BT_NOERROR;
#if pDEBUG
        pDebugPrintfEX((pLOGERROR,pLOGA2DP,"rda_bt_a2dp_send_stream_data_send_req ReTrySendFla%d\n",ReTrySendFlag ));
#endif 

	if(ReTrySendFlag)
	{
		status=Avdtp_Send_Data(size_ping, buf_ping, 0, seq_num,0, 0);
		if(status==BT_NORESOURCES)
	   	{
	   	ReTrySendFlag=TRUE;   
		  if(get_len >gbuf_len )
	           A2dp_Senddata_TimerHandle = rdabt_timed_event_in(gTimeDuration, A2dpDataSendTimerCallback, codec);
       	}
		else
		{
			ReTrySendFlag=FALSE;
			        if(seq_num==0xFFFFFFFF)
            				seq_num=0;
        
       			 seq_num++;
      		  if(get_len >gbuf_len )
        	    A2dp_Senddata_TimerHandle = rdabt_timed_event_in(gTimeDuration, A2dpDataSendTimerCallback, codec);
      		  else
       		 {
         		   if(codec->GetPayloadDone)
               		 codec->GetPayloadDone();
           		 get_len=0;
        		}
		}
		
		return;
	}
	
    u_int8 codec_type=Avdtp_Get_Codec_Type();
    
    if(codec==NULL)
        return;

    if(codec->state &2)
    {
#if pDEBUG
        pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"rda_bt_a2dp_send_stream_data_send_req codec->state=2\n" ));
#endif 
        memset(buf_ping, 0,sizeof(buf_ping));
        pTimerCancel(A2dp_Senddata_TimerHandle);
        A2dp_Senddata_TimerHandle=0;      
         //seq_num = 0;
        return;
    }

    if(codec->QueryPayloadSize ==NULL || codec->GetPayload ==NULL)
        return;

    if(codec_type==A2DP_MPEG_AUDIO)  //mp3
    {
        gTimeDuration = 10*1000;// 20*1000;
        gbuf_len = 773;
    }
    else if(codec_type==A2DP_SBC)
    {
        gTimeDuration = 20*1000;//20*1000;
//        gbuf_len = gCurMaxFrameLen*7; //659;   // Frame size is 77
       gbuf_len = 617;//617;
		
    }

    codec->QueryPayloadSize(&min, &total); 
    get_len = total;
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"rda_bt_a2dp_send_stream_data_send_req min:%d, total:%d, size_ping=%d ,gbuf_len=%d\n", min, total, size_ping,gbuf_len ));
    
    if(min<total && size_ping<total && gbuf_len<total)
    {
        memset(buf_ping, 0,sizeof(buf_ping));
        size_ping=codec->GetPayload(buf_ping, gbuf_len, &sample_cnt);
    	pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"GetPayload size_ping=%d ,sample_cnt=%d , seq_num=%d\n", size_ping,sample_cnt,seq_num ));
        status=Avdtp_Send_Data(size_ping, buf_ping, 0, seq_num,0, 0);
	 if(status==BT_NORESOURCES)
	   {
	   	ReTrySendFlag=TRUE;
		  if(get_len >gbuf_len )
		    A2dp_Senddata_TimerHandle = rdabt_timed_event_in(gTimeDuration, A2dpDataSendTimerCallback, codec);
			return;
       	}
        get_len-= size_ping;
        if(seq_num==0xFFFFFFFF)
            seq_num=0;
        
        seq_num++;

        if(get_len >gbuf_len )
            A2dp_Senddata_TimerHandle = rdabt_timed_event_in(gTimeDuration, A2dpDataSendTimerCallback, codec);
        else
        {
            if(codec->GetPayloadDone)
                codec->GetPayloadDone();
            get_len=0;
        }
    }
    else
    {
        if(codec->GetPayloadDone)
            codec->GetPayloadDone();
    }
}

void rda_bt_a2dp_send_stream_pause_req( u_int8 src_mod_id, u_int8 stream_handle, A2DP_codec_struct* codec )
{
    u_int8 status=BT_NOERROR;
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"rda_bt_a2dp_send_stream_pause_req stream_handle=%d\n",stream_handle ));
#endif   
	if(A2dp_Senddata_TimerHandle)
	{
	    pTimerCancel(A2dp_Senddata_TimerHandle);
           A2dp_Senddata_TimerHandle=0;      
	}

  if(codec&&(codec->GetPayloadDone))
            codec->GetPayloadDone();
  
    status=Avdtp_Send_Suspend(NULL);
}

void rda_bt_a2dp_send_stream_close_req( u_int8 src_mod_id, u_int8 stream_handle)
{
//    u_int8 status=BT_NOERROR;
//    sep sep_entry;
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"rda_bt_a2dp_send_stream_close_req stream_handle=%d\n", stream_handle ));
#endif   
	if(A2dp_Senddata_TimerHandle)
	{
	    pTimerCancel(A2dp_Senddata_TimerHandle);
           A2dp_Senddata_TimerHandle=0;      
	}
    Avdtp_Send_Close(NULL);
}

void rda_bt_a2dp_send_stream_abort_req( u_int8 src_mod_id, u_int8 stream_handle )
{
//     u_int8 status=BT_NOERROR;
//    sep sep_entry;
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP,"rda_bt_a2dp_send_stream_abort_req \n" ));
#endif   

	if(A2dp_Senddata_TimerHandle)
	{
	    pTimerCancel(A2dp_Senddata_TimerHandle);
           A2dp_Senddata_TimerHandle=0;      
	}
	
    Avdtp_Send_Abort(NULL);
}

void rda_bt_a2dp_send_sep_discover_res( u_int8 src_mod_id, u_int16 connect_id, u_int16 result, u_int8 sep_num, bt_sep_info_struct* sep_list)
{
    Avdtp_Send_Sep_Discover_Res(connect_id, sep_list);
}

void rda_bt_a2dp_send_capabilities_get_res( u_int8 src_mod_id, u_int16 connect_id, u_int16 result, u_int8 codec_type, bt_a2dp_audio_codec_cap_struct* audio_cap )
{
    Avdtp_Send_Sep_Capabilities_Get_Res(connect_id, codec_type, audio_cap);
}

void rda_bt_a2dp_send_stream_config_res( u_int8 src_mod_id, u_int16 result, u_int8 stream_handle)
{
    Avdtp_Send_Sep_Stream_Config_Res(result, stream_handle);
}

void rda_bt_a2dp_send_stream_reconfig_res( u_int8 src_mod_id, u_int16 result, u_int8 stream_handle)
{
    
}

void rda_bt_a2dp_send_stream_open_res( u_int8 src_mod_id, u_int16 result, u_int8 stream_handle )
{
    Avdtp_Send_Sep_Stream_Open_Res(result, stream_handle);
}

void rda_bt_a2dp_send_stream_start_res( u_int8 src_mod_id, u_int16 result, u_int8 stream_handle )
{
    Avdtp_Send_Sep_Stream_Start_Res(result, stream_handle);
}

void rda_bt_a2dp_send_stream_pause_res( u_int8 src_mod_id, u_int16 result, u_int8 stream_handle )
{
    Avdtp_Send_Sep_Stream_Suspend_Res(result,stream_handle);
}

void rda_bt_a2dp_send_stream_close_res( u_int8 src_mod_id, u_int16 result, u_int8 stream_handle)
{
    Avdtp_Send_Sep_Stream_Close_Res(result,stream_handle);
}

void rda_bt_a2dp_send_stream_abort_res( u_int8 src_mod_id, u_int8 stream_handle )
{
    Avdtp_Send_Sep_Stream_Abort_Res(stream_handle);
}

void bt_a2dp_alg_set_bit_rate_from_qos_init(u_int32 bit_rate_cur)
{

}
u_int32 bt_a2dp_alg_set_bit_rate_from_qos(u_int8 qos, u_int32 bit_rate_prev, u_int32 (*SetCodecBitRate)(u_int32))
{
    return 0;
}

#endif

#if 0
int Sdp_Init_Headset(void)
{
	int status=0;
#if SDP_RECORD_GEN
	t_SDP_serviceRecord *pRecord;
	u_int32 recordHandle;
	char *serviceName = "Headset";
	u_int16 serviceNameLen = 7;//strlen(serviceName);
	u_int8 rf_serverChannel = 2;

	status = SDP_CreateRecord(&recordHandle, &pRecord);
	if (status!=BT_NOERROR)
		return 1;

	{
		/********************************/
		/*** add the Service Class List */
		/********************************/

		u_int16 uuid;
		t_SDP_dataElement elem_headsetUUID;
		t_SDP_dataElement elem_genAudioUUID;
		t_SDP_dataElement *elem_serviceUUIDList[10];
		t_SDP_dataElement elem_ServiceClassIDList;

		/* Encode the headset uuid to a data element */
		uuid = SDP_SCLASS_UUID_HEADSET; 
		status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_headsetUUID);
		if (status!=BT_NOERROR)
			return 1;

		/* Encode the generic audio uuid to an attribute */
		uuid = SDP_SCLASS_UUID_GEN_AUDIO; 
		status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_genAudioUUID);
		if (status!=BT_NOERROR)
			return 1;

		/* Add both encoded uuids to a list */
		elem_serviceUUIDList[0] = &elem_headsetUUID;
		elem_serviceUUIDList[1] = &elem_genAudioUUID;
		status = SDP_CreateList(SDP_SEQUENCE, 2, elem_serviceUUIDList, &elem_ServiceClassIDList);
		if (status!=BT_NOERROR)
			return 1;
		
		status = SDP_AddAttribute(pRecord, SDP_AID_SERVICE_CLASS, &elem_ServiceClassIDList);
		if (status!=BT_NOERROR)
			return 1;
	}



	{
		/******* add the Protocol Descriptor List (PDL) 
		The PDL is a list containing two lists, one for each lower protocol 
		<PDL:  <L2CAP: UUID> <RFCOMM: UUID, serverChannel> >
		*/
		t_SDP_dataElement elem_l2capProtocolInfoList;
		t_SDP_dataElement elem_rfcommProtocolInfoList;
		t_SDP_dataElement elem_protocolDescriptorList;
		t_SDP_dataElement *elem_protocolDescriptorElements[10];

		{
			/**** create the L2CAP protocol information list */
			u_int16 uuid;
			t_SDP_dataElement elem_l2capUUID;
			t_SDP_dataElement *elem_l2capProtocolInfoElements[10];

			/* encode the l2cap protocol uuid */
			uuid = SDP_PROTOCOL_UUID_L2CAP;
			status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_l2capUUID);
			if (status!=BT_NOERROR)
				return 1;

			/* Add the l2cap protocol uuid to a list */
			elem_l2capProtocolInfoElements[0] = &elem_l2capUUID;
			status = SDP_CreateList(SDP_SEQUENCE, 1, elem_l2capProtocolInfoElements, &elem_l2capProtocolInfoList);
			if (status!=BT_NOERROR)
				return 1;
		}

		{
			/**** create the RFCOMM protocol information list */
			u_int16 uuid;
			t_SDP_dataElement elem_rfcommUUID;
			t_SDP_dataElement elem_rfcommServerChannel;
			t_SDP_dataElement *elem_rfcommProtocolInfoElements[10];

			/* encode the rfcomm protocol uuid */
			uuid = SDP_PROTOCOL_UUID_RFCOMM;
			status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_rfcommUUID);
			if (status!=BT_NOERROR)
				return 1;

			/* encode the rfcomm server channel number */
			status = SDP_EncodeAttribute(SDP_UINT, 1, (u_int8 *)&rf_serverChannel, &elem_rfcommServerChannel);
			if (status!=BT_NOERROR)
				return 1;

			/*  Add the both of the attributes to an RFCOMM protocol information list */
			elem_rfcommProtocolInfoElements[0] = &elem_rfcommUUID;
			elem_rfcommProtocolInfoElements[1] = &elem_rfcommServerChannel;
			status = SDP_CreateList(SDP_SEQUENCE, 2, elem_rfcommProtocolInfoElements, &elem_rfcommProtocolInfoList);
			if (status!=BT_NOERROR)
				return 1;
		}


		/* Enclose both L2CAP and RFCOMM protocol information lists in an outer protocol descriptor list */
		elem_protocolDescriptorElements[0] = &elem_l2capProtocolInfoList;
		elem_protocolDescriptorElements[1] = &elem_rfcommProtocolInfoList;

		status = SDP_CreateList(SDP_SEQUENCE, 2, elem_protocolDescriptorElements, &elem_protocolDescriptorList);
		if (status!=BT_NOERROR)
			return 1;

		/* Add the protocol descriptor list as a record attribute */
		status = SDP_AddAttribute(pRecord, SDP_AID_PROTOCOL, &elem_protocolDescriptorList);
		if (status!=BT_NOERROR)
			return 1;
	}


	{
		/**** add the Profile Descriptor List */
		t_SDP_dataElement elem_profile0DescriptorList;
		t_SDP_dataElement elem_ProfileDescriptorList;
		t_SDP_dataElement *elem_ProfileDescriptorElements[10];

		{
			/**** create the Profile0 Descriptor List */

			u_int16 uuid;
			t_SDP_dataElement elem_profile0UUID;
			t_SDP_dataElement elem_profile0Version;
			t_SDP_dataElement *elem_profile0DescriptorElements[10];
			u_int16 profile0Version=0x0100;

			/* encode the profile0 protocol uuid */
			uuid = SDP_SCLASS_UUID_HEADSET;
			status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_profile0UUID);
			if (status!=BT_NOERROR)
				return 1;

			/* encode the profile0 server channel number */
			status = SDP_EncodeAttribute(SDP_UINT, 2, (u_int8 *)&profile0Version, &elem_profile0Version);
			if (status!=BT_NOERROR)
				return 1;

			/*  Add the both of the attributes to an profile0 protocol information list */
			elem_profile0DescriptorElements[0] = &elem_profile0UUID;
			elem_profile0DescriptorElements[1] = &elem_profile0Version;
			status = SDP_CreateList(SDP_SEQUENCE, 2, elem_profile0DescriptorElements, &elem_profile0DescriptorList);
			if (status!=BT_NOERROR)
					return 1;
		}  

		/* Enclose the Profile0 Descriptor List in an outer Profile Descriptor List */
		elem_ProfileDescriptorElements[0] = &elem_profile0DescriptorList;

		status = SDP_CreateList(SDP_SEQUENCE, 1, elem_ProfileDescriptorElements, &elem_ProfileDescriptorList);
		if (status!=BT_NOERROR)
			return 1;

		/* Add the the Profile Descriptor List to the record  */
		status = SDP_AddAttribute(pRecord, SDP_AID_PROFILE_LIST, &elem_ProfileDescriptorList);
		if (status!=BT_NOERROR)
			return 1;
	}


	{
		/**** add the Service Name */
		t_SDP_dataElement elem_serviceName;
		u_int16 nameAttributeId = SDP_AID_DEFAULT_LANGUAGE+SDP_AID_SERVICE_NAME_OFFSET; 

		/*  Encode the name as a string attrbiute */
		status = SDP_EncodeAttribute(SDP_TEXT, serviceNameLen, (u_int8*)serviceName, &elem_serviceName);
		if (status!=BT_NOERROR)
			return 1;

		/* Add the the Profile0 Descriptor List to the record  */
		status = SDP_AddAttribute(pRecord, nameAttributeId, &elem_serviceName);
		if (status!=BT_NOERROR)
			return 1;
	}


	{
		/***** Add the Volume Control Support `***********/
		t_SDP_dataElement elem_volCtrlSupported;
		u_int8 volCtrlStatus = 1;

		status = SDP_EncodeAttribute(SDP_BOOLEAN, 1, &volCtrlStatus, &elem_volCtrlSupported);
		if (status!=BT_NOERROR)
			return 1;

		status = SDP_AddAttribute(pRecord, SDP_AID_AUDIO_VOLUME, &elem_volCtrlSupported);
		if (status!=BT_NOERROR)
			return 1;
	}
#endif
	return 0; /* success */

}

int sdp_init_A2DP(void)
{
	int status=0;
	
#if SDP_RECORD_GEN
	t_SDP_serviceRecord *pRecord;
	u_int32 recordHandle;

	status = SDP_CreateRecord(&recordHandle, &pRecord);
	if (status!=BT_NOERROR)
		return 1;

	{
		/********************************/
		/*** add the Service Class List */
		/********************************/

		u_int16 uuid;
		t_SDP_dataElement elem_audiosinktUUID;
		t_SDP_dataElement *elem_serviceUUIDList[10];
		t_SDP_dataElement elem_ServiceClassIDList;

		/* Encode the audiosink uuid to a data element */
		uuid = SDP_SCLASS_UUID_AUDIOSINK; 
		status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_audiosinktUUID);
		if (status!=BT_NOERROR)
			return 1;

		/* Add the encoded uuids to a list */
		elem_serviceUUIDList[0] = &elem_audiosinktUUID;
		status = SDP_CreateList(SDP_SEQUENCE, 1, elem_serviceUUIDList, &elem_ServiceClassIDList);
		if (status!=BT_NOERROR)
			return 1;
		
		status = SDP_AddAttribute(pRecord, SDP_AID_SERVICE_CLASS, &elem_ServiceClassIDList);
		if (status!=BT_NOERROR)
			return 1;
	}

	{
		/******* add the Protocol Descriptor List (PDL) 
		The PDL is a list containing two lists, one for each lower protocol 
		<PDL:  <L2CAP: UUID, PSM> <AVDTP: UUID, version> >
		*/
		t_SDP_dataElement elem_l2capProtocolInfoList;
		t_SDP_dataElement elem_a2dpProtocolInfoList;
		t_SDP_dataElement elem_protocolDescriptorList;
		t_SDP_dataElement *elem_protocolDescriptorElements[10];

		{
			/**** create the L2CAP protocol information list */
			u_int16 uuid;
			t_SDP_dataElement elem_l2capUUID;
			t_SDP_dataElement elem_psm;
			t_SDP_dataElement *elem_l2capProtocolInfoElements[10];
			u_int16 psm=0;

			/* encode the l2cap protocol uuid */
			uuid = SDP_PROTOCOL_UUID_L2CAP;
			status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_l2capUUID);
			if (status!=BT_NOERROR)
				return 1;

			/* encode the psm */
			psm = 0x0019;  //AVDTP 
			status = SDP_EncodeAttribute(SDP_UINT, 2, (u_int8 *)&psm, &elem_psm);
			if (status!=BT_NOERROR)
				return 1;

			/* Add the l2cap protocol uuid to a list */
			elem_l2capProtocolInfoElements[0] = &elem_l2capUUID;
			elem_l2capProtocolInfoElements[1] = &elem_psm;
			status = SDP_CreateList(SDP_SEQUENCE, 2, elem_l2capProtocolInfoElements, &elem_l2capProtocolInfoList);
			if (status!=BT_NOERROR)
				return 1;
		}

		{
			/**** create the AVDTP protocol information list */
			u_int16 uuid;
			t_SDP_dataElement elem_avdtpUUID;
			t_SDP_dataElement elem_avdtpVersion;
			t_SDP_dataElement *elem_avdtpProtocolInfoElements[10];
			u_int16 protocolVersion=0x0100;

			/* encode the avdtp protocol uuid */
			uuid = SDP_PROTOCOL_UUID_AVDTP;
			status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_avdtpUUID);
			if (status!=BT_NOERROR)
				return 1;

			/* encode the avdtp version */
			status = SDP_EncodeAttribute(SDP_UINT, 2, (u_int8 *)&protocolVersion, &elem_avdtpVersion);
			if (status!=BT_NOERROR)
				return 1;

			/*  Add the both of the attributes to an avdtp protocol information list */
			elem_avdtpProtocolInfoElements[0] = &elem_avdtpUUID;
			elem_avdtpProtocolInfoElements[1] = &elem_avdtpVersion;
			status = SDP_CreateList(SDP_SEQUENCE, 2, elem_avdtpProtocolInfoElements, &elem_a2dpProtocolInfoList);
			if (status!=BT_NOERROR)
				return 1;
		}


		/* Enclose both L2CAP and AVDTP protocol information lists in an outer protocol descriptor list */
		elem_protocolDescriptorElements[0] = &elem_l2capProtocolInfoList;
		elem_protocolDescriptorElements[1] = &elem_a2dpProtocolInfoList;

		status = SDP_CreateList(SDP_SEQUENCE, 2, elem_protocolDescriptorElements, &elem_protocolDescriptorList);
		if (status!=BT_NOERROR)
			return 1;

		/* Add the protocol descriptor list as a record attribute */
		status = SDP_AddAttribute(pRecord, SDP_AID_PROTOCOL, &elem_protocolDescriptorList);
		if (status!=BT_NOERROR)
			return 1;
	}


	{
		/**** add the Profile Descriptor List */
		t_SDP_dataElement elem_profile0DescriptorList;
		t_SDP_dataElement elem_ProfileDescriptorList;
		t_SDP_dataElement *elem_ProfileDescriptorElements[10];

		{
			/**** create the Profile0 Descriptor List */

			u_int16 uuid;
			t_SDP_dataElement elem_profile0UUID;
			t_SDP_dataElement elem_profile0Version;
			t_SDP_dataElement *elem_profile0DescriptorElements[10];
			u_int16 profile0Version=0x0100;

			/* encode the profile0 protocol uuid */
			uuid = SDP_SCLASS_UUID_A2DP;
			status = SDP_EncodeAttribute(SDP_UUID, 2, (u_int8 *)&uuid, &elem_profile0UUID);
			if (status!=BT_NOERROR)
				return 1;

			/* encode the profile0 version */
			status = SDP_EncodeAttribute(SDP_UINT, 2, (u_int8 *)&profile0Version, &elem_profile0Version);
			if (status!=BT_NOERROR)
				return 1;

			/*  Add the both of the attributes to an profile0 protocol information list */
			elem_profile0DescriptorElements[0] = &elem_profile0UUID;
			elem_profile0DescriptorElements[1] = &elem_profile0Version;
			status = SDP_CreateList(SDP_SEQUENCE, 2, elem_profile0DescriptorElements, &elem_profile0DescriptorList);
			if (status!=BT_NOERROR)
					return 1;
		}  

		/* Enclose the Profile0 Descriptor List in an outer Profile Descriptor List */
		elem_ProfileDescriptorElements[0] = &elem_profile0DescriptorList;

		status = SDP_CreateList(SDP_SEQUENCE, 1, elem_ProfileDescriptorElements, &elem_ProfileDescriptorList);
		if (status!=BT_NOERROR)
			return 1;

		/* Add the the Profile Descriptor List to the record  */
		status = SDP_AddAttribute(pRecord, SDP_AID_PROFILE_LIST, &elem_ProfileDescriptorList);
		if (status!=BT_NOERROR)
			return 1;
	}


	{
		/**** add the supported features */
		t_SDP_dataElement elem_SupportedFeatures;
		u_int16 supportedFeatures= 0x0001;//category 1 supported

		/*  Encode the supported features */
		status = SDP_EncodeAttribute(SDP_UINT, 2, (u_int8 *)&supportedFeatures, &elem_SupportedFeatures);
		if (status!=BT_NOERROR)
			return 1;

		/* Add the the Profile0 Descriptor List to the record  */
		status = SDP_AddAttribute(pRecord, SDP_AID_SUPPORTED_FEATURES, &elem_SupportedFeatures);
		if (status!=BT_NOERROR)
			return 1;
	}
#endif
	return 0; /* success */
}
#endif

boolean A2dp_Get_BdAddr(t_bdaddr *addr)
{
	return Avdtp_Get_BdAddr(addr);
}

void A2dp_Suspend(void)
{
	if(ga2dp.state != A2dpStreaming)
		return;
	
	Avdtp_Handle_Suspend_Req();

	/* Change to connected state */
	A2dp_Set_State(A2dpConnected);
}

void A2dp_Start(void)
{
	if(ga2dp.state == A2dpStreaming)
		return;
	
	Avdtp_Handle_Start_Req();

	/* Change to connected state */
	A2dp_Set_State(A2dpStreaming);	
}

void A2dp_Codec_Enable(void)
{

}

void A2dp_Codec_Disable(void)
{

}

void A2dp_Codec_configure(void)
{

}

#if SDP_CLIENT	
int A2dp_ServiceDiscover(t_bdaddr address) 
/**********************************************************************/
{
    int status=BT_NOERROR;

    u_int16 criteria;
    t_SDP_StopRule rule;
    

    /******************************/
    /* address list to search for */
	/******************************/
    a2dp_devices.numItems = 1;
    //a2dp_pattern.addresses = malloc(sizeof(t_bdaddr));
    a2dp_devices.addresses =&_addresses;

    if(!a2dp_devices.addresses)
    {
        return BT_NORESOURCES; 
    }
    a2dp_devices.addresses[0] = address;
    
    /* device relation criteria */
    criteria = 0;
    
    /******************************/
    /* search pattern             */
    /******************************/

    a2dp_pattern.numItems = 1;
    //a2dp_pattern.patternUUIDs = malloc(a2dp_pattern.numItems*sizeof(u_int32));
     a2dp_pattern.patternUUIDs = &_patternUUIDs;

    /*
    if(!a2dp_pattern.patternUUIDs)
    {
        if(a2dp_devices.addresses)
        {
            free(a2dp_devices.addresses);
        }
        return BT_NORESOURCES;   
    }*/
    
    // PROFILE DEPENDENT
    a2dp_pattern.patternUUIDs[0] = SDP_SCLASS_UUID_AUDIOSINK;

    /* attributes to retrieve */
    a2dp_attribs.numItems = A2DP_MAX_ATTRIBS_NUM;
    //a2dp_attribs.attributeIDs = malloc(a2dp_attribs.numItems*sizeof(u_int32));
    a2dp_attribs.attributeIDs = _attributeIDs;;

    /*if(!a2dp_attribs.attributeIDs)
    {
        if(a2dp_devices.addresses)
            free(a2dp_devices.addresses);		
        if(a2dp_pattern.patternUUIDs)
            free(a2dp_pattern.patternUUIDs);	

        return BT_NORESOURCES;
    }*/

    a2dp_attribs.attributeIDs[0] = SDP_AID_PROTOCOL;
    a2dp_attribs.attributeIDs[1] = SDP_AID_PROFILE_LIST;
    a2dp_attribs.attributeIDs[2] = SDP_AID_SUPPORTED_FEATURES;

    /* stop rule */
    rule.maxItems = 10;
    rule.maxDuration = 40;
    rule.maxBytes = 250;
    
    /* call a service search  */
    status=SDP_ServiceBrowse(&a2dp_pResults,&a2dp_devices,&a2dp_pattern,&a2dp_attribs,&rule,criteria,(sdp_api_callback)a2dp_sdpSearchCompleteCallback);    
    /*if (status!=BT_PENDING)
    {
        if(a2dp_devices.addresses)
            free(a2dp_devices.addresses);		
        if(a2dp_pattern.patternUUIDs)
            free(a2dp_pattern.patternUUIDs);	
        if(a2dp_attribs.attributeIDs)
            free(a2dp_attribs.attributeIDs);	

        return status;
    }*/ 
	
	return status;
}


int a2dp_sdpSearchCompleteCallback(u_int8 status)
{
    u_int32 uuid;
    u_int8 exit_condiction=0;
    u_int8 featuresFound = 0;
    u_int8 a2dp_versionFound = 0;
    u_int8 avdtp_versionFound = 0;
    u_int8* finger;
    u_int8* end_byte;
    u_int8 counter;
    u_int16 aid;
    u_int8 offset;
    u_int32 length;
    u_int32 outer;
    u_int32 inner;
    u_int8 search_fail=0;
    //u_int8 incomplete_results=0;

    u_int16 a2dp_versionInfo=0;
    u_int16 avdtp_versionInfo=0;

#if 0
    if (a2dp_pResults->status!=0x00)
    {
        if(a2dp_devices.addresses)
            pFree(a2dp_devices.addresses);		
        if(a2dp_pattern.patternUUIDs)
            pFree(a2dp_pattern.patternUUIDs);	
        if(a2dp_attribs.attributeIDs)
            pFree(a2dp_attribs.attributeIDs);	

        status = BT_UNKNOWNERROR;
    }
    else if(!a2dp_pResults->element.pData)
    {
        if(a2dp_devices.addresses)
            pFree(a2dp_devices.addresses);		
        if(a2dp_pattern.patternUUIDs)
            pFree(a2dp_pattern.patternUUIDs);	
        if(a2dp_attribs.attributeIDs)
            pFree(a2dp_attribs.attributeIDs);	

        status = BT_UNKNOWNERROR;
    }
#endif

    if (status == BT_NOERROR)
    {
        end_byte = a2dp_pResults->element.pData + a2dp_pResults->element.dataLen -1;

        finger = a2dp_pResults->element.pData;

        get_list_length(finger,&length,&offset);
        finger+= offset;
        get_list_length(finger,&length,&offset);
        finger+= offset;
        counter = length;
        do 
        {
            counter--;
            finger++; /* bypass type code */
            aid = (finger[0] << 8) + finger[1];
            finger+=2;
            counter-=2;

            switch(aid) 
            {
                case SDP_AID_PROTOCOL:
                    get_list_length(finger,&length,&offset);
                    finger += offset;
                    counter -= offset;
                    outer = length;
                    do 
                    {
                        do 
                        {
                            get_list_length(finger,&length,&offset);
                            finger +=offset;
                            counter -=offset;
                            outer -=offset;
                            inner = length;
                            read_uuid(finger,&uuid,&offset);
                            finger += offset;
                            counter -= offset;
                            outer -= offset;
                            inner -= offset;
                            if(uuid == SDP_PROTOCOL_UUID_AVDTP && inner) 
                            {
                                avdtp_versionInfo = (finger[1]<<8)+finger[2];
                                avdtp_versionFound = 1;
                                finger+=3;
                                counter-=3;
                                inner -= 3;
                                outer-=3;
                            }
                            else
                            {
                                finger += inner;
                                counter -= inner;
                                outer -= inner;
                                inner = 0;
                            }					
                        }while(inner);
                    } while (outer);
                break;

                case SDP_AID_PROFILE_LIST :
                    get_list_length(finger,&length,&offset);
                    finger += offset;
                    counter -= offset;
                    outer = length;
                    do 
                    {
                        do 
                        {
                            get_list_length(finger,&length,&offset);
                            finger +=offset;
                            counter -=offset;
                            outer -=offset;
                            inner = length;
                            read_uuid(finger,&uuid,&offset);
                            finger += offset;
                            counter -= offset;
                            outer -= offset;
                            inner -= offset;
                            if(uuid == SDP_SCLASS_UUID_A2DP && inner) 
                            {
                                a2dp_versionInfo = (finger[1] << 8) + finger[2];
                                a2dp_versionFound = 1;
                                finger+=3;
                                counter-=3;
                                inner -= 3;
                                outer-=3;
                            }
                            else
                            {
                                finger += inner;
                                counter -= inner;
                                outer -= inner;
                                inner = 0;
                            }					
                        }while(inner);
                    }while (outer);
                break;

                case SDP_AID_SUPPORTED_FEATURES :
                    counter--;
                    finger++; // Skip over Type code and len = 0x09
                    A2DP_Supported_Feature.supported_feature= (finger[0] << 8) + finger[1];
                    featuresFound = 1;
                    finger+=2;
                    counter-=2;
                break;

                default :
                    exit_condiction=1;
                break;

            }
        }while (counter && !exit_condiction); 
    }
    else
    {
        // Search Complete returned an Error.
        search_fail = 1;
    }

	/*****************************************************
	 *
	 * Interop fix for Motorola V525.
	 *
	 * As some devices/phone dont respond with features 
	 * or version information it is important that the SDP
	 * request is not failed due to lack of either of these 
	 * parameters. The critical parameter is the Server Channel 
	 * Number, once we have this we can proceed to SLC setup.
	 ********************************************************/

	if (!featuresFound)
	{
		A2DP_Supported_Feature.supported_feature= A2DP_DEFAULT_SUPPORTED_FEATURES;
	}
	if (!a2dp_versionFound)
	{
		a2dp_versionInfo = A2DP_DEFAULT_VERSION_INFORMATION;
    }
	if (!avdtp_versionFound)
	{
		avdtp_versionInfo = AVDTP_DEFAULT_VERSION_INFORMATION;
    }

	if(a2dp_versionFound && avdtp_versionFound)
		A2DP_Supported_Feature.a2dp_supported= 1;

	return 0;
}

t_a2dp_Supported_feature Get_A2DP_Feature(void)
{
    return A2DP_Supported_Feature;
}

int A2DP_Setup_Connect(t_bdaddr address)
{
    int status;
    status=L2_Connect(address, AVDTP_PSM);
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGA2DP, "A2DP_Setup_Connect status=%d", status));
#endif
    return status;
}

int A2DP_Send_Data(u_int16 length, u_int8* p_data, u_int32 timestamp,u_int16 seq_num_s, u_int8 payload_type, u_int8 marker )
{
    return Avdtp_Send_Data(length,  p_data,  timestamp, seq_num_s,  payload_type, marker);
}


#endif
