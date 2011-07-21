#include<jddex_platformdefines.h>
#include<ddl.h>
#include<jcal.h>
#include <jdi_cutils_tcsstr.h>
#include <jdi_cutils.h>



#ifdef __cplusplus
    extern   "C"
    {
#endif

#include <mdi_audio.h>
#include <profilegprots.h>

#include <mmi_trace.h>

#ifdef __cplusplus
    }
#endif



JC_RETCODE jddex_PlatformGetAudioType(JC_INT8 *pcAudioMimeType, JC_UINT8 *uiAudioFormat)
{
     
	if(jc_strstr(pcAudioMimeType , "audio/mid") ||
		jc_strstr(pcAudioMimeType,"audio/midi")||
		jc_strstr(pcAudioMimeType,"audio/sp-midi") )
	{
        //*uiAudioFormat = MDI_FORMAT_IMELODY ;
        *uiAudioFormat= MDI_FORMAT_MID ;
	}
	else if(jc_strstr(pcAudioMimeType , "audio/wav")||
		jc_strstr(pcAudioMimeType,"audio/x-wav"))
	{
        *uiAudioFormat = MDI_FORMAT_WAV ;
	}
	else if(jc_strstr(pcAudioMimeType, "audio/mp3")||
             jc_strstr(pcAudioMimeType, "audio/mpeg"))
	{
    	*uiAudioFormat = MDI_FORMAT_DAF ;
	}
	else if(jc_strstr(pcAudioMimeType, "audio/amr"))
	{
    	*uiAudioFormat = MDI_FORMAT_AMR ;
	}
	else if(jc_strstr(pcAudioMimeType, "audio/aac"))
	{
    	*uiAudioFormat = MDI_FORMAT_AAC ;
	}
	else
	{
    	*uiAudioFormat= MDI_FORMAT_NONE ;
	}

	return JC_OK ;
}


/* for playing the audio. should return an ID */
JC_HANDLE  jddex_PlatformPlayAudio(JC_OBJECT_INFO	*pstObjectInfo, JC_CHAR* m_pmTempFile)
{
	mdi_result res=0;
	
	if(pstObjectInfo->bIsFileContent)
	{
		res  = mdi_audio_play_file_with_vol_path(pstObjectInfo->uBuffer.pmFileName, 
	                                   MDI_AUDIO_PLAY_ONCE, JC_NULL,JC_NULL,
	                                   GetRingVolumeLevel(),MDI_DEVICE_SPEAKER2,0);
		
	    if (MDI_AUDIO_SUCCESS == res)                               
		{
			mmi_trace(1,(PCSTR)"jddex log: jddex_PlatformPlayAudio FileCon: Success") ;
		}
		else
		{
			mmi_trace(1,(PCSTR)"jddex log: jddex_PlatformPlayAudio FileCon: Fail[%d]",res) ;
			if(res == 255 )
				DisplayPopup((PU8)GetString(STR_ID_MMS_FILE_OPEN_ERROR), IMG_GLOBAL_ERROR, 1, 2000, 1) ;

		}
	}
	else
	{
#ifdef USE_FILE_FOR_AUDIOBUFF_PLAY	
		res  = mdi_audio_play_file_with_vol_path(m_pmTempFile, MDI_AUDIO_PLAY_ONCE, 
						JC_NULL,JC_NULL,GetRingVolumeLevel(),MDI_DEVICE_SPEAKER2,0) ;
#else
		JC_UINT8 uiAudioFormat = 0;
		jddex_PlatformGetAudioType(pstObjectInfo->psMimeType, &uiAudioFormat);
		
		mdi_audio_play_string_with_vol_path(pstObjectInfo->uBuffer.pucBuffer,
		  (pstObjectInfo->uiObjectLen / 4),uiAudioFormat, 0 /*no loop*/,JC_NULL,JC_NULL,GetRingVolumeLevel(),JC_NULL);
		  
		mmi_trace(1, (PCSTR)"uiAudioFormat [%d], len[%d]", uiAudioFormat,pstObjectInfo->uiObjectLen);

#endif


		if(MDI_AUDIO_SUCCESS == res )
		{
			mmi_trace(1,(PCSTR)"jddex_PlatformPlayAudio : Success") ;
		}
		else
		{
			mmi_trace(1,(PCSTR)"jddex_PlatformPlayAudio : Fail %d", res) ;
		}
	}
	return NULL ;
}

JC_RETCODE  jddex_PlatformStopAudio(JC_OBJECT_INFO	*pstObjectInfo,JC_HANDLE pstAudioHnd,JC_CHAR* m_pmTempFile)
{
	mdi_result res ;
	if(pstObjectInfo->bIsFileContent == E_FALSE)
	{
#ifdef USE_FILE_FOR_AUDIOBUFF_PLAY		
		res = mdi_audio_stop_file();
#else
		res = mdi_audio_stop_string();
mmi_trace(1, (PCSTR)"mdi_audio_stop_string returns %d",res);
#endif

	}
	else
	{
		res = mdi_audio_stop_file();
	}
	if(res == MDI_AUDIO_SUCCESS)
	{
		mmi_trace(1,(PCSTR)"jddex_PlatformStopAudio : Success") ;
		return JC_OK ;
	}
	else
	{
		mmi_trace(1,(PCSTR)"jddex_PlatformStopAudio : Fail") ;
		return JC_ERR_GENERAL_FAILURE ;
	}
}

JC_RETCODE  jddex_PlatformPauseAudio(JC_OBJECT_INFO	*pstObjectInfo, JC_HANDLE pstAudioHnd,JC_CHAR* m_pmTempFile)
{
	mmi_trace(1,(PCSTR)"jddex_PlatformPauseAudio ") ;

	return jddex_PlatformStopAudio(pstObjectInfo,pstAudioHnd,m_pmTempFile);

	 /*if(MDI_AUDIO_SUCCESS == mdi_audio_pause(JC_NULL, JC_NULL))
     	{
     		mmi_trace(1,(PCSTR)"jddex_PlatformPauseAudio : Success") ;
     		return JC_OK ;
     	}
	 else
 	{
	 	mmi_trace(1,(PCSTR)"jddex_PlatformPauseAudio : Fails") ;
 		return JC_ERR_GENERAL_FAILURE ;
 	}*/
}

JC_RETCODE  jddex_PlatformResumeAudio(JC_OBJECT_INFO	*pstObjectInfo,JC_HANDLE pstAudioHnd, JC_CHAR* m_pmTempFile)
{
	mmi_trace(1,(PCSTR)"jddex_PlatformResumeAudio") ;
	  jddex_PlatformPlayAudio(pstObjectInfo, m_pmTempFile);
	  return JC_OK;

	 

	/* if(MDI_AUDIO_SUCCESS == mdi_audio_resume(JC_NULL, JC_NULL))
 	{
	 	mmi_trace(1,(PCSTR)"jddex_PlatformResumeAudio : Success") ;
     		return JC_OK ;
 	}
 	else
 	{
	 	mmi_trace(1,(PCSTR)"jddex_PlatformResumeAudio : Fails") ;
     		return JC_ERR_GENERAL_FAILURE ;
 	}*/
}


