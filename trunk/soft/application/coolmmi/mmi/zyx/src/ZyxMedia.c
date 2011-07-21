#ifdef __ZYX_PLATFORM_SUPPORT__

#include "zyxconfig.h"
#include "zyxsys.h"
#include "zyxmedia.h"
#include "zyxextern.h"
//#include "media.h"
#include "l1audio.h"

typedef struct ZyxMediaA{
	int volume;
	int format;
	int stat;
	int current_time;
	int start_time;
}ZyxMediaA, *ZyxMediaAPtr;

static ZyxMediaA	zma = {3};
static ZyxMediaAPtr pzma = 0;
static ZyxAudioHandler ZyxAudioHandlerCallback = NULL;

void ZyxAudioExit(){

}
void ZyxAudioInit(){

}

extern void volatile ZyxCallFunc1( U32 parg, void* pentry);


static void MediaCallback( Media_Event evt ){
	ZyxTrace("zyx audio event=%d",evt);
	switch(evt)
	{
	case MEDIA_DATA_REQUEST:{
	#if 0
		U8 *buf;
		U32 len;
		U32 dlen;
		static U32 o_len = 0;
		
		Media_GetWriteBuffer_ext((U8**)&buf,(U32*)&len);
		dlen = len > o_len ? len - o_len : o_len - len;
		if (dlen > 200){
			o_len = len;
			evt = ZYX_MEDIA_DATA_REQUEST;
		}else{
			return ;
		}
	#else
		evt = ZYX_MEDIA_DATA_REQUEST;
	#endif
	}
	break;
	case MEDIA_END:	
		evt = ZYX_MEDIA_END;			
	break;
	case MEDIA_ERROR:	
		evt = ZYX_MEDIA_ERROR;		
	break;
	case MEDIA_TERMINATED:	
		evt = ZYX_MEDIA_TERMINATED;
	break;
	default:
		evt = ZYX_MEDIA_UNKNOWN;
	break;
	}

	if (ZyxCheckCallback((void*)ZyxAudioHandlerCallback))
	{		
		//ZyxTrace("call ZyxAudioHandlerCallback %p", ZyxAudioHandlerCallback);
#if 1		
		ZyxAudioHandlerCallback(evt);
		//ZyxCallFunc1(evt, &ZyxAudioHandlerCallback);
#else
		ZyxStartTimer( 1, ZyxAudioTimerHandler);
#endif
		//ZyxTrace("call ZyxAudioHandlerCallback %p", ZyxAudioHandlerCallback);
	}
}

extern void MusicStartAudioMode();
int ZyxAudioOpen(PU8 audio_buf,int size,ZYX_MEDIA_TYPE format, ZyxAudioHandler cb)

{
	if (audio_buf == NULL)
		return -1;

	ZyxTrace("ZyxAudioOpen 1\n");
      mdi_audio_stop_all();
	ZyxTrace("ZyxAudioOpen 2\n");
	//mute bug???
      DM_SetAudioVolume(zma.volume);	 
      MusicStartAudioMode();	
	ZyxTrace("ZyxAudioOpen 3\n");
      
	ZyxTrace("zyx audio buf=%x,size=%d",audio_buf, size);
	if (pzma)
	{
	    ZyxTrace("zyx stop audio first");
	    return -1;
	}

	if (NULL == pzma)
	{
		ZyxAudioHandlerCallback = cb;
		switch (format)
		{
			case ZYX_AUDIO_FORMAT_AAC:
			pzma = &zma;
			pzma->format = ZYX_AUDIO_FORMAT_AAC;
			break;
			case ZYX_AUDIO_FORMAT_AMR:
			pzma = &zma;
			pzma->format = ZYX_AUDIO_FORMAT_AMR;			
			break;
			default:
			return -1;
			break;
		}

		ZyxTrace("zyx pzma = 0x%x", pzma);
		if (pzma)
		{
			pzma->current_time = 0;
			pzma->start_time = 0;
			pzma->stat = AAC_STATE_STOP;
			Media_SetBuffer((U16*)audio_buf, (U32)size/2);
		}
	}

	return 0;
}

int ZyxAudioPlay(void)
{
	int ret = -1;

	if (pzma)
	{
		if (ZyxGetAudioState() != AAC_STATE_STOP)
			return ret;

		switch (pzma->format)
		{
			case ZYX_AUDIO_FORMAT_AAC:
			ret = Media_Play(MDI_FORMAT_AAC, MediaCallback, NULL);
			pzma->start_time = ZyxGetSysTicks();
			break;
			case ZYX_AUDIO_FORMAT_AMR:
			ret = Media_Play(MDI_FORMAT_AMR, MediaCallback, NULL);
			pzma->start_time = ZyxGetSysTicks();
			break;
			default:
			break;
		}

		ZyxTrace("zyx play ret=%d",ret);
		if (ret == MEDIA_SUCCESS)
			ret = 0;

		pzma->stat = AAC_STATE_PLAY;
	}

	return ret;
}

extern void MCI_AudioStopBuffer();
int ZyxAudioStop(void)
{
	int ret = -1;

	if (pzma)
	{
		ZyxTrace("ZyxAudioStop");
		if (ZyxGetAudioState() == AAC_STATE_STOP)
		{
			pzma = NULL;
			return ret;
		}
		
		Media_Stop();
		//ret = mdi_audio_stop_all();
		MCI_AudioStopBuffer();
		ZyxTrace("zyx close ret=%d",ret);
		//if (ret == MEDIA_SUCCESS)	ret = 0;

		pzma->stat = AAC_STATE_STOP;
		
		pzma = NULL;
	}

	return ret;
}

int ZyxAudioPause(void)
{
	int ret = -1;

	if (pzma)
	{
		ZyxTrace("ZyxAudioPause");
		if (ZyxGetAudioState()  !=  AAC_STATE_PLAY)
			return ret;
			
		ret = Media_Pause();
		ZyxTrace("zyx pause ret=%d",ret);

		if (ret == MEDIA_SUCCESS)
			ret = 0;
			
		pzma->stat = AAC_STATE_PAUSE;
		ZyxTrace(" Zyx Audio current time=%d on pause",pzma->current_time);
		pzma->current_time += ZyxGetSysTicks() - pzma->start_time;
	}

	return ret;
}

int ZyxAudioResume(void)
{
	int ret = -1;

	if (pzma)
	{
		if (ZyxGetAudioState()  !=  AAC_STATE_PAUSE)
			return ret;
		ret = Media_Resume();
		ZyxTrace("zyx resume ret=%d",ret);
		if (ret == MEDIA_SUCCESS)
			ret = 0;
			
		pzma->stat = AAC_STATE_PLAY;
		ZyxTrace(" Zyx Audio start time=%d on resume", pzma->start_time);
		pzma->start_time = ZyxGetSysTicks() - pzma->current_time;
	}

	return ret;
}

int ZyxGetAudioState(void)
{
	if (pzma)
	{
		ZyxTrace("zyx audio state=%d",pzma->stat);
		return pzma->stat;
	}

	return AAC_STATE_STOP;
}

int ZyxAudioGetCurrentTime(void)
{
	int time;

       if (pzma)
       {
		if (ZyxGetAudioState() ==	AAC_STATE_PLAY)
		{
			time = ZyxGetSysTicks() - pzma->start_time;
		}
		else
		{
			time = pzma->current_time;
			pzma->current_time = 0;
		}
		
		ZyxTrace(" Zyx Audio current time=%d on get",time);

		return time;
       }
	 return 0;
}


void ZyxAudioGetWriteBuffer(PU8* buf_ptr,int *len)
{
	if (pzma)
	{
		Media_GetWriteBuffer_ext((U8**)buf_ptr,(U32*)len);
		ZyxTrace("zyx get write buf =%x,len=%d",*buf_ptr,*len);
	}
}

void ZyxAudioWriteDone(int len)
{
	if (pzma)
	{
		ZyxTrace("ZyxAudioWriteDone %d\n", len);
		Media_WriteDataDone(len);
	}
}

void ZyxAudioDataFinish (void)
{
	if (pzma)
	{
		ZyxTrace("ZyxAudioDataFinish\n");
		Media_DataFinished();
	}
}

void ZyxSetVolume(U8 volume)
{
	if (volume > 14)	volume = 14;

    if (pzma)
   	{
		ZyxTrace("ZyxAudioSetVolume %d\n", volume);
		DM_SetAudioVolume(volume/2);
		pzma->volume = volume;
   	}
}

int ZyxGetVolume()
{
	if (pzma)
	{
		ZyxTrace("ZyxAudioGetVolume %d\n", pzma->volume);
		return pzma->volume;
	}
	else
	{
		return zma.volume;
	}

	return 0;
}

#endif

