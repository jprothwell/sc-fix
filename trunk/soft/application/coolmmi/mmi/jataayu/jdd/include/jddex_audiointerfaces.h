#ifndef _JDDEX_AUDIOINTERFACES_
#define _JDDEX_AUDIOINTERFACES_


JC_HANDLE  jddex_PlatformPlayAudio(JC_OBJECT_INFO	*pstObjectInfo, JC_CHAR* m_pmTempFile);

JC_RETCODE  jddex_PlatformStopAudio(JC_OBJECT_INFO	*pstObjectInfo,JC_HANDLE pstAudioHnd,JC_CHAR* m_pmTempFile);

JC_RETCODE  jddex_PlatformPauseAudio(JC_OBJECT_INFO	*pstObjectInfo,JC_HANDLE pstAudioHnd,JC_CHAR* m_pmTempFile) ;

JC_RETCODE  jddex_PlatformResumeAudio(JC_OBJECT_INFO	*pstObjectInfo, JC_HANDLE pstAudioHnd,JC_CHAR* m_pmTempFile);



#endif //_JDDEX_AUDIOINTERFACES_
