#include "a2dp_core.h"
#include "a2dp.h"
#include "avrcp_core.h"
#include "avdtp_core.h"
#include "headset_private.h"
#include "headset_sched.h"


#if COMBINED_HOST==0
#include "papi.h"
#endif

#if HFP_HF
#if (BUILD_TYPE!=UNIT_TEST_BUILD)
#include "hs_codec.h"
#endif
#endif

t_headsetData gheadset;

#define HFP_CALL_CLOSED                02
#define HFP_CALL_ESTABLISHMENT         04
#define HFP_CALL_W4_ADT_OK             10
#define HFP_CALL_W4_CIEV_CALL_SETUP_2  11
#define HFP_CALL_W4_CIEV_CALL_1        12
#define HFP_CALL_ACTIVE                13
#define HFP_CALL_W4_USER_ACCEPT        14
#define HFP_W4_PEER_CALL_CONNECT       15
#define HFP_LOCAL_USER_INIT_CALL_REJECT 16
#define HFP_LOCAL_USER_INIT_CALL_RELEASE 17
#define HFP_CALL_ALERTING              20
#define HFP_CALL_ANSWERED              21

extern int Get_Current_HFP_Call_Status(void);
extern int Hfp_Get_BdAddr(t_bdaddr *addr);
extern int Is_AG_Support_In_Band_Ring(void);

void Headset_Init(void)
{
#ifdef HEADSET_PRESENT
#if A2DP_SNK
	//headset_db_init();
	gheadset.sent_suspend = 0;
	gheadset.active_profile = none_active;
	gheadset.speaker_volume.av_volume = 8;
	gheadset.speaker_volume.hfp_volume = 8;
#endif    

	A2dp_Init();
	Avrcp_Init();
	Avdtp_Register_Sep(A2DP_MPEG_AUDIO);  //register the SEP of MP3
	Avdtp_Register_Sep(A2DP_SBC);  //register the SEP of SBC.
#endif
}

#ifdef HFP_PRESENT
#if HFP_HF
u_int8 Headset_Hfp_Sched(void)
{
	int hfp_status= 0;
	t_A2DP_STATE a2dp_status = A2dpReady;
	
	a2dp_status = Get_A2dp_State();
	hfp_status=Get_Current_HFP_Call_Status();

	switch(hfp_status)
	{
		case HFP_CALL_CLOSED:
			if(gheadset.sent_suspend)
				Headset_AvStream_Start();
			break;
			
		case HFP_CALL_ESTABLISHMENT:
			gheadset.active_profile = hfp_active;
			break;

		case HFP_CALL_W4_ADT_OK:
			break;

		case HFP_CALL_W4_CIEV_CALL_1:
			break;

		case HFP_CALL_ACTIVE:
			break;

		case HFP_CALL_W4_USER_ACCEPT: //incoming call and ring.
		{
			if(!Is_AG_Support_In_Band_Ring()) //HFG not support the in-band ring
			{
				//play ring tone.
#if (BUILD_TYPE!=UNIT_TEST_BUILD)
				HSCodec_Play_Hint(HINT_RING);
#endif
			}
			else //play in band ring.
			{
				if(a2dp_status==A2dpStreaming)
				{
					/* If the AV audio has not already been stopped then stop it now.
					Only if AG and A2DP source aren't same device. */
					if (!Headset_Is_A2dp_SourceAnAg())
						Headset_AvStream_Stop(TRUE);
				}
			}
		}
			break;

		case HFP_W4_PEER_CALL_CONNECT:   //outgoing call
			break;

		case HFP_LOCAL_USER_INIT_CALL_REJECT:
			break;

		case HFP_LOCAL_USER_INIT_CALL_RELEASE:
			break;
	}

	return 0;
}
#endif
#endif

#if A2DP_PRESENT
u_int8 Headset_A2dp_Sched(void)
{
	return 0;
}
#endif

/**************************************************************************/
void Headset_AvStream_Stop(boolean send_suspend)
{
	/*
	if (app->channel_mode == a2dp_mono)
	{
		// plug port 0 into both DACs 
		StreamDisconnect(StreamKalimbaSource(0),StreamPcmSink(0));
	}
	else
	{
		// disconnect port 0 from Left DAC 
		StreamDisconnect(StreamKalimbaSource(0),StreamPcmSink(0));

		// Disconnect port 1 from Right DAC 
		StreamDisconnect(StreamKalimbaSource(1),StreamPcmSink(1));
	}

	StreamDisconnect(StreamPcmSource(0),StreamKalimbaSink(0));
	StreamDisconnect(StreamPcmSource(1),StreamKalimbaSink(1));
*/
	// Stop decode 
	A2dp_Codec_Disable();
	
	
	if (send_suspend)
	{
		A2dp_Suspend();
		if (!Headset_Is_A2dp_SourceAnAg())
		{
			Headset_Send_Pause();
		}
		gheadset.sent_suspend = 1;
	}
	else
		gheadset.sent_suspend = 0;

	//headsetPowerAmp (app , FALSE ) ;
}	

void Headset_AvStream_Start(void)
{
	int hfp_status= 0;
	t_A2DP_STATE a2dp_status = A2dpReady;
	
	a2dp_status = Get_A2dp_State();
	hfp_status=Get_Current_HFP_Call_Status();
	
	if (a2dp_status==A2dpStreaming || hfp_status != HFP_CALL_CLOSED)
		return;

	// Set the local volume to mute 
	Headset_Update_Volume(0);

	// Make sure amp is powered on
	//headsetPowerAmp(app, TRUE);

	//Configure the DACs for the codec configuration 
	A2dp_Codec_configure();

	if (gheadset.sent_suspend)
	{
		A2dp_Start();
		if (!Headset_Is_A2dp_SourceAnAg())
		{
			Headset_Send_Play();
		}
	}
	else
	{
		A2dp_Codec_Enable();
		gheadset.active_profile = av_active;
	}

	//Set the local volume back to what it was 
	Headset_Update_Volume(gheadset.speaker_volume.av_volume);
	
}


/********************************************************
	if AG and A2DP source are same device.
*********************************************************/
boolean Is_Same_BdAddr(t_bdaddr* p_bd_addr1,t_bdaddr* p_bd_addr2)
{
	if ((p_bd_addr1->bytes[0] == p_bd_addr2->bytes[0]) &&
		(p_bd_addr1->bytes[1] == p_bd_addr2->bytes[1]) &&
		(p_bd_addr1->bytes[2] == p_bd_addr2->bytes[2]) &&
		(p_bd_addr1->bytes[3] == p_bd_addr2->bytes[3]) &&
		(p_bd_addr1->bytes[4] == p_bd_addr2->bytes[4]) &&
		(p_bd_addr1->bytes[5] == p_bd_addr2->bytes[5]))
		return TRUE;

	else
		return FALSE;
}

// if AG and A2DP source are same device.
boolean Headset_Is_A2dp_SourceAnAg(void)
{
	t_bdaddr bdaddr_a2dp, bdaddr_hfp;
    
	if (!A2dp_Get_BdAddr(&bdaddr_a2dp))
		return FALSE;
	
	if (!Hfp_Get_BdAddr( &bdaddr_hfp))
		return FALSE;
	
	if (Is_Same_BdAddr(&bdaddr_a2dp, &bdaddr_hfp))
	        return TRUE;
    
	return FALSE;
}

void Headset_A2dp_Suspend_Ind(void)
{
	if(Get_A2dp_State() != A2dpStreaming)
		return;
	
	/* Change to connected state */
	A2dp_Set_State(A2dpConnected);
	Headset_AvStream_Stop(FALSE);
}

void Headset_A2dp_Start_Ind(void)
{
	if(Get_A2dp_State() == A2dpStreaming)
	        return;

	/* HFP is active so don't start AV */
	if(Get_Current_HFP_Call_Status()!=HFP_CALL_CLOSED)
	{
		A2dp_Suspend();
		Headset_Send_Pause();
	}
	else
	{
		/* Change to streaming state */
		A2dp_Set_State(A2dpStreaming);
		Headset_AvStream_Start();
	}

}

void Headset_Update_Volume(u_int8 gain)
{

}

