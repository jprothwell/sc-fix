
#include "settingprofile.h"
#include "internal.h"
#include "jam_msg_handler.h"


extern U8 g_mmi_java_is_vib_on;
unsigned char gcj_media_semId;
unsigned char gcj_tone_semId;

int gcj_playerid = 0;

void gcj_audio_finish_ind(mdi_result ret)
{
	gcj_TraceOut(0,"gcj_audio_finish_ind gcj_playerid=%d,gcj_IsJavaOn=%d",gcj_playerid,gcj_IsJavaOn());
	EVENT_HANDLER_T eventData;
	if (!gcj_IsJavaOn())
		return;
	//gcj_StubReleaseSemphone(gcj_media_semId);
    
    eventData.java_event_type = MM_EOM_EVENT;
    eventData.java_event_data.mmEventData.playerid = gcj_playerid;
    eventData.java_event_data.mmEventData.curmtime = 0;
    
    event_action_push(eventData);
}


void gcj_tone_finish_ind(mdi_result ret)
{
	gcj_TraceOut(0,"gcj_tone_finish_ind gcj_playerid=%d,gcj_IsJavaOn=%d",gcj_playerid,gcj_IsJavaOn());
	EVENT_HANDLER_T eventData;
	if (!gcj_IsJavaOn())
		return;
	//gcj_StubReleaseSemphone(gcj_tone_semId);
}
void gcj_StubSendEvent(COS_EVENT *ev,module_type mod)
{
	TASK_HANDLE *task = GetMmiTaskHandle(mod);
	sxr_Send(ev,task->nMailBoxId,SXR_SEND_EVT);
}


void gcj_StubWaitEvent(unsigned int eventId)
{
	TASK_HANDLE *task = GetMmiTaskHandle(MOD_MMI);

	while (GCJ_RETURN_EVENT != gcj_StubCondWait(task->nMailBoxId,0));
}

volatile unsigned char gcj_SoundEnd=1;
unsigned char gcj_isSoundEnd()
{
	return gcj_SoundEnd;
}

#define TONE 0
#define AUDIO 1

int Gcj_audio_play_string(void* audio_data, U32 len)
{
	char test[10];
	memcpy(test,audio_data,9);
	test[9]='\0';
	gcj_TraceOut(1,"current task id =%d,len=%d, Gcj_audio_play_string test %s",sxr_GetCurrentTaskId(),len,test);
	mmi_java_play_tone_req_struct *data = (mmi_java_play_tone_req_struct*)OslMalloc(sizeof(mmi_java_play_tone_req_struct));
	//gcj_SoundEnd = 0;
	data->tone_or_audio = AUDIO;
	data->audio_date = audio_data;
	data->data_len = (len+3)/4;
    	data->volume = g_mmi_java_aud_volume_level;
	data->handler = gcj_audio_finish_ind;
	COS_EVENT evt;
	memset(&evt,0,sizeof(evt));
	evt.nEventId = (unsigned int)MSG_ID_MMI_JAVA_PLAY_TONE_REQ;
	evt.nParam1 = data;
	//gcj_media_semId = gcj_StubNewSemphone();
	gcj_StubSendEvent(&evt,MOD_MMI);
	//gcj_StubTakeSemphone(gcj_media_semId);
	//gcj_SoundEnd = 1;
	return 1;
}

int Gcj_audio_stop_string()
{
/*
int nResult= 	mdi_audio_stop_string();
    if(MDI_AUDIO_SUCCESS ==  nResult)
    	{
    		return TRUE;
    	}
	else 
		{
	//	       	abort();

		return FALSE;
		}
*/
}


int Gcj_audio_set_volume( int  vol)
{
	
	//mdi_audio_set_volume(MDI_VOLUME_MEDIA, (U8 )(vol*7/100));
}

typedef enum _altertype{
    /**
     * Information Alert identifier
     */
    ALERT_INFO = 1,
    /**
     * Warning Alert identifier
     */
    ALERT_WARN = 2,
    /**
     * Error Alert identifier
     */
    ALERT_ERR  = 3,
    /**
     * Alarm Alert identifier
     */
    ALERT_ALRM = 4,
    /**
     * Confirmation Alert identifier
     */
    ALERT_CFM  = 5,
}ALERT_TYPE_T;


extern void playRequestedTone(ALL_TONE_ENUM playtone);
void gcj_StubPlayAlertSound(ALERT_TYPE_T alertType)
{
	ALL_TONE_ENUM playtone= 0;

	switch(alertType)
	{
    /**
     * Information Alert identifier
     */
	   case ALERT_INFO:
	   	playtone = MESSAGE_TONE;
         break;
		 
    /**
     * Warning Alert identifier
     */
      case  ALERT_WARN:
     	playtone = 	WARNING_TONE;
	  break;
	  
    /**
     * Error Alert identifier
     */
      case ALERT_ERR:
     	playtone = 	ERROR_TONE;
	  	break;
    /**
     * Alarm Alert identifier
     */
    case ALERT_ALRM:
     	playtone = 	ALARM_TONE;
		break;
    /**
     * Confirmation Alert identifier
     */
     case     ALERT_CFM :
     	playtone = 	SUCCESS_TONE;
		break;
	default:
		break;
	}
        unsigned int tmp = MSG_ID_MMI_JAVA_PLAY_TONE_REQ;
	gcj_TraceOut(1,"current task id =%d,gcj_StubPlayAlertSound event id =%d",sxr_GetCurrentTaskId(),tmp);
	mmi_java_play_tone_req_struct *data = (mmi_java_play_tone_req_struct*)OslMalloc(sizeof(mmi_java_play_tone_req_struct));
	data->tone_or_audio = TONE;
	data->tone_type = playtone;
	data->handler = gcj_tone_finish_ind;
	COS_EVENT evt;
	memset(&evt,0,sizeof(evt));
	evt.nEventId = (unsigned int)MSG_ID_MMI_JAVA_PLAY_TONE_REQ;
	evt.nParam1 = data;
	//gcj_tone_semId = gcj_StubNewSemphone();
	gcj_StubSendEvent(&evt,MOD_MMI);
	//gcj_StubTakeSemphone(gcj_tone_semId);
}


void Stopgcj_StubPlaynVibrate()
{
	StopTimer(FMGR_AUDIO_TIMER);
 	VibratorOff();                         
}
 void gcj_StubPlaynVibrate(int dur)
{
	if (dur == 0)
        return;

    if(g_mmi_java_is_vib_on == FALSE)
        return;
    
	PlayVibratorDur(dur) ;
//	StartTimer(TIMER_PROFILES_TONE_PLAY, dur+1000, Stopgcj_StubPlaynVibrate);

}


