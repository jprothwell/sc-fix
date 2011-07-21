
#include "string.h"
#include "stdio.h"
#include "cos.h"
#include "ejTTS.h"
#include "mdi_audio.h"
#include "Fs_func.h"
#include "mmi_trace.h"
#include "OslMemory.h"
#include "nvram_user_defs.h"

#include "ejTTSPlayer.h"
#include "ejTTSMenu.h"

//#define DATA_MODE_SMALL_FILE
#define DEV_TASK_PRIORITY (COS_MMI_TASKS_PRIORITY_BASE + 1)
//extern CNPackage;
#define TTS_BUFF_SIZE 1024*384

#ifdef __cplusplus
extern "C" {
#endif

// WAV文件头格式
typedef struct _WAVE_FILE_HEADER
{
	char chRIFF[4];
	DWORD dwRIFFLen;
	char chWAVE[4];
	char chFMT[4];
	DWORD dwFMTLen;
    	WORD wFormatTag;        /* format type */
    	WORD nChannels;         /* number of channels (i.e. mono, stereo, etc.) */
    	DWORD nSamplesPerSec;    /* sample rate */
    	DWORD nAvgBytesPerSec;   /* for buffer estimation */
    	WORD nBlockAlign;       /* block size of data */
	WORD wBitsPerSample;
	char chDATA[4];
	DWORD dwDATALen;
}WAVE_FILE_HEADER;

//#pragma pack (4)
typedef struct strParameter{
	unsigned long hTTS;
	//mdi_handle* pHandle;
	FS_HANDLE fHandle;
	long offset;
}jtParameter;

typedef struct _TTS_PLAYER_PARAM{
	char pText[512];
	size_t nSize;

}TTS_PLAYER_PARAM, *P_TTS_PLAYER_PARAM;

//#pragma pack ()

//extern const unsigned char * CNPackage;

U16 AnsiiToUnicodeString(S8 *pOutBuffer, S8 *pInBuffer );

const char UTF8_HEADER[]= {-17, -69, -65};
const char UNICODE_HEADER[] = {-1, -2};
const char UNICODE_BE_HEADER[] = {-2, -1};

//const char pTestText[] = "捷通华声语音技术有限公司。";
const char pTestText[] = "请再T卡放于要合成的文本TTS.txt";
//char text[] = {' 捷','通','华','声','语','音','技','术','有','限','公','司','。','\0'};
int nReadCount = 0;

HANDLE g_hTTSPlayTask;
unsigned long hTTS;
unsigned char *pHeap;

//char gAudioBuff[1024*10];
char* gTtsData;
U32 nPosition;
U8 bPCMIdle;
U8 bBuffFull;
U8 bStopTTS;

extern int jt_Printf(const char *fmt, ...) ;

BOOL get_wav_format(unsigned long handle, kal_uint16 *sample_rate, kal_int8 *bit_rate)
{
	long wav_format = -1;
	jtErrCode nErr = jtTTS_ERR_NONE;
	
	nErr = jtTTS_GetParam(handle, jtTTS_PARAM_WAV_FORMAT, &wav_format);
	mmi_trace(1, "tts test, get wav format the code is %d", nErr);
	//kal_prompt_trace(MOD_EJTTS, "jtTTS_PARAM_WAV_FORMAT = %d %d", wav_format, nErr);

	if (jtTTS_ERR_NONE != nErr || -1 == wav_format)
	{
		mmi_trace(1, "tts test, get wav format error!");
		return FALSE;
	}

	switch(wav_format)
	{
	case jtTTS_FORMAT_PCM_NORMAL:
	//此处依音库而定，用户需要手动修改
		*bit_rate = 16;
		*sample_rate = 16000;
		break;
	case jtTTS_FORMAT_PCM_8K8B:
		*bit_rate = 8;
		*sample_rate = 8000;
		break;
	case jtTTS_FORMAT_PCM_8K16B:
		*bit_rate = 16;
		*sample_rate = 8000;
		break;
	case jtTTS_FORMAT_PCM_16K8B:
		*bit_rate = 8;
		*sample_rate = 16000;
		break;
	case jtTTS_FORMAT_PCM_16K16B:
		*bit_rate = 16;
		*sample_rate = 16000;
		break;
	case jtTTS_FORMAT_PCM_11K8B:
		*bit_rate = 8;
		*sample_rate = 11025;
		break;
	case jtTTS_FORMAT_PCM_11K16B:
		*bit_rate = 16;
		*sample_rate = 11025;
		break;
	case jtTTS_FORMAT_PCM_22K8B:
		*bit_rate = 8;
		*sample_rate = 22050;
		break;
	case jtTTS_FORMAT_PCM_22K16B:
		*bit_rate = 16;
		*sample_rate = 22050;
		break;
	case jtTTS_FORMAT_PCM_44K8B:
		*bit_rate = 8;
		*sample_rate = 44100;
		break;
	case jtTTS_FORMAT_PCM_44K16B:
		*bit_rate = 16;
		*sample_rate = 44100;
		break;
	default:
		*bit_rate = 16;
		*sample_rate = 16000;
		break;
	}

	return TRUE;
}

void playStringCallBack(mdi_result result)
{
	bPCMIdle = 1;
	mmi_trace(1, "tts test, stop the wav play and the position is: %d.", nPosition);
	mdi_audio_stop_string();
	nPosition = 0;

	//mdi_audio_play_string_with_vol_path(void * audio_data, U32 len, U8 format, U8 play_style, mdi_handle * handle_p, mdi_callback handler, U8 volume, U8 path)
}

/*
void TTS_PlayTask(PVOID pData)
{
	//add the code to play
	const char * pFileName = "d:/test.wav";
	mdi_audio_play_file(pFileName, MDI_AUDIO_PLAY_ONCE, NULL, NULL);

	COS_DeleteTask(g_hTTSPlayTask);
}
//*/

jtErrCode jt_OutputVoiceProc(void* pParameter, 
	long iOutputFormat, void* pData, long iSize)
{
	//S16 phoneDrvLetter;
	//S16 cardDrvLetter;
	//char temp[16];
	//char pFileName[16];
	//FS_HANDLE fHandle;
	INT32 write;
	//UINT len;
	//WAVE_FILE_HEADER wfh;
	jtParameter* parameter;

	if(pParameter == NULL)
	{
		mmi_trace(1, "test tts the output proc user data uis not exist.");
		return jtTTS_ERR_NONE;
	}
	
	parameter = (jtParameter*)pParameter;

	mmi_trace(1, "test tts the output proc tts handle is %x, ", parameter->hTTS);
	mmi_trace(1, "test tts the output proc file handle is %x, ", parameter->fHandle);
	mmi_trace(1, "test tts the output data size is :%d", iSize);

	//return 0;
	
	if(iSize <= 0)
	{
		jtTTS_SynthStop(parameter->hTTS);

		//mdi_audio_stop_string();
		//mdi_audio_play_file((const UINT8 *)L"D:/test/test.wav", MDI_AUDIO_PLAY_ONCE, NULL, NULL);
		/*
		g_hTTSPlayTask = COS_CreateTask(TTS_PlayTask,
			NULL, NULL, 512, 
			DEV_TASK_PRIORITY, COS_CREATE_DEFAULT,
			0, "TTS_PlayTask");
		//*/
		return jtTTS_ERR_NONE;
	}
	
	//UINT32* voices = (UINT32)pData;
	//mdi_audio_play_string(voices, iSize, MDI_FORMAT_PCM_16K, MDI_AUDIO_PLAY_ONCE, parameter->pHandle, NULL);
	//DM_Audio_StartStream(voices, iSize, DM_DICTAPHONE_AMR_RING, 0);
	//DM_Audio_StopStream();

	MMI_FS_Seek(parameter->fHandle, 0, FS_FILE_END);
	MMI_FS_Write(parameter->fHandle, pData, iSize, &write);
	//MMI_FS_Close(fHandle);

	return jtTTS_ERR_NONE;
}

jtErrCode jt_OutputVoicePCMProc(void* pParameter, 
	long iOutputFormat, void* pData, long iSize)
{
	INT32 write;
	jtParameter* parameter;

	if(pParameter == NULL)
	{
		mmi_trace(1, "test tts the output proc user data uis not exist.");
		return jtTTS_ERR_NONE;
	}
	
	parameter = (jtParameter*)pParameter;

	mmi_trace(1, "test tts the output proc tts handle is %x, ", parameter->hTTS);
	//mmi_trace(1, "test tts the output proc file handle is %x, ", parameter->fHandle);
	mmi_trace(1, "test tts the output data size is :%d", iSize);

	//return 0;
	
	if(iSize <= 0)
	{
		/*
		if(nPosition > 0){
			while(bPCMIdle == 0)
			{
				//等待播放完
			}
			mmi_trace(1, "tts test,  play buff the nPosition is:", nPosition);
			memcpy(gAudioBuff, gTtsData, nPosition);
			mdi_audio_play_string(gAudioBuff, nPosition, MDI_FORMAT_PCM_16K, MDI_AUDIO_PLAY_ONCE, NULL, playStringCallBack);
			bBuffFull = 0;
			bPCMIdle = 0;
			nPosition = 0;
		}
		//*/
		
		jtTTS_SynthStop(parameter->hTTS);

		//mdi_audio_stop_string();
		//mdi_audio_play_file((const UINT8 *)L"D:/test/test.wav", MDI_AUDIO_PLAY_ONCE, NULL, NULL);
		/*
		g_hTTSPlayTask = COS_CreateTask(TTS_PlayTask,
			NULL, NULL, 512, 
			DEV_TASK_PRIORITY, COS_CREATE_DEFAULT,
			0, "TTS_PlayTask");
		//*/
		return jtTTS_ERR_NONE;
	}
	
	//UINT32* voices = (UINT32)pData;
	//mdi_audio_play_string(voices, iSize, MDI_FORMAT_PCM_16K, MDI_AUDIO_PLAY_ONCE, parameter->pHandle, NULL);
	//DM_Audio_StartStream(voices, iSize, DM_DICTAPHONE_AMR_RING, 0);
	//DM_Audio_StopStream();
	
	if(bStopTTS == 1)
	{
		jtTTS_SynthStop(parameter->hTTS);

		return jtTTS_ERR_NONE;
	}

	if(nPosition + iSize < TTS_BUFF_SIZE)
	{
		memcpy(gTtsData+nPosition, pData, iSize);
		nPosition += iSize;
	}

	
	//MMI_FS_Seek(parameter->fHandle, 0, FS_FILE_END);
	//MMI_FS_Write(parameter->fHandle, pData, iSize, &write);
	//MMI_FS_Close(fHandle);

	return jtTTS_ERR_NONE;
}


/*
jtErrCode jt_InputTextProc(void* pParameter, void* pText, long* pSize)
{
	char* p = (char*)pText;
	int i;
	int count = (128-nReadCount)> &pSize? &pSize:(128-nReadCount);
	for(i = 0; i < count; i++)
	{
		p[i] = pTestText[nReadCount+i];
	}

	nReadCount += count;
}
//*/

/*
jtErrCode jtTTSAPI jtTTS_Init(
	const signed char* szCNLib,			// [in] chinese data library
	const signed char* szENLib,			// [in] english data library
	const signed char* szDMLib,			// [in] domain data library
	unsigned long*	pdwHandle,			// [out] handle to TTS engine
	void*			pExtBuf)			// [in] extend buffer size
{
	return 0;
}
//*/
extern const unsigned char CNPackage[];

int ejTTS_Init()
{
	jtErrCode error;
	long nSize = 0;

#ifdef TTS_LOAD_FILE_MODE

	error = jtTTS_GetExtBufSize((signed char*)L"D:/CNPackage.dat", NULL, NULL, &nSize);
	//error = jtTTS_GetExtBufSize(CNPackage, NULL, NULL, &nSize);
	mmi_trace(1, "tts test, the getbufsize error is: %d, and request heap size is: %d", error, nSize);
	jt_Printf("tts test, the buff size is %d", nSize);
	if(error != jtTTS_ERR_NONE)
	{
		mmi_trace(1, "tts test, get buf size error!");
		hTTS = 0;
		return;		
	}

	pHeap = OslMalloc(nSize);
	memset(pHeap, 0, nSize);
	mmi_trace(1, "tts test, the heap address is %x!", pHeap);

	error = jtTTS_Init((signed char*)L"D:/CNPackage.dat", NULL, NULL, &hTTS, pHeap);
#elif defined(TTS_LOAD_CONST_MODE)
	error = jtTTS_GetExtBufSize(CNPackage, NULL, NULL, &nSize);
	//error = jtTTS_GetExtBufSize(CNPackage, NULL, NULL, &nSize);
	mmi_trace(1, "tts test, the getbufsize error is: %d, and request heap size is: %d", error, nSize);
	jt_Printf("tts test, the buff size is %d", nSize);
	if(error != jtTTS_ERR_NONE)
	{
		mmi_trace(1, "tts test, get buf size error!");
		hTTS = 0;
		return;		
	}

	pHeap = OslMalloc(nSize);
	memset(pHeap, 0, nSize);
	mmi_trace(1, "tts test, the heap address is %x!", pHeap);

	error = jtTTS_Init(CNPackage, NULL, NULL, &hTTS, pHeap);


#else
  #error  "TTS_LOAD_MODE define error!!"
#endif
	//error = jtTTS_Init(CNPackage, NULL, NULL, &hTTS, pHeap);
	mmi_trace(1, "tts test, the init error is: %d", error);

	if(error != jtTTS_ERR_NONE)
	{
		mmi_trace(1, "tts test, init tts error!");
		if(pHeap != NULL)
		{
			OslMfree(pHeap);
			pHeap = NULL;
		}
		hTTS = 0;
		return;
	}

	if(gTtsData == NULL)
	{
		gTtsData = OslMalloc(TTS_BUFF_SIZE);
	}
}

void ejTTS_PlayToFile(char* pText, size_t nSize)
{	

	//long nSize = 0;
	
	//unsigned char *pHeap;

	jtErrCode error;

	//mdi_handle handle;

	jtParameter parameter;

	FS_HANDLE fp;
	char buffer[256];
	char uncode[256];
	INT32 read;
	long encode;

	FS_HANDLE fHandle;
	WAVE_FILE_HEADER wfh;
	INT32 fSize;
	UINT write;

	nvram_tts_setting_struct ttsSetting;
	long volumes[] = 
	{
		jtTTS_VOLUME_MIN,
		jtTTS_VOLUME_MIN>>1,
		jtTTS_VOLUME_NORMAL,
		jtTTS_VOLUME_MAX>>1,
		jtTTS_VOLUME_MAX

	};

	long speeds[] = 
	{
		jtTTS_SPEED_MAX,
		jtTTS_SPEED_NORMAL,
		jtTTS_SPEED_MIN
	};

	long styles[] = 
	{
		jtTTS_SPEAK_STYLE_CLEAR,
		jtTTS_SPEAK_STYLE_NORMAL,
		jtTTS_SPEAK_STYLE_PLAIN,
		jtTTS_SPEAK_STYLE_VIVID
	};

	long effects[] = 
	{
		jtTTS_SOUNDEFFECT_BASE,
		jtTTS_SOUNDEFFECT_CHORUS,
		jtTTS_SOUNDEFFECT_ECHO,
		jtTTS_SOUNDEFFECT_NEARFAR,
		jtTTS_SOUNDEFFECT_REVERB,
		jtTTS_SOUNDEFFECT_ROBOT
	};

	long tones[] = 
	{
		jtTTS_PITCH_MIN,
		jtTTS_PITCH_NORMAL,
		jtTTS_PITCH_MAX
	};

	mmi_trace(1, "tts test, the task begin!");

	//如果TTS 没有初始化，则进行初始化。
	if(hTTS == 0)
	{
		ejTTS_Init();
	}

	mmi_trace(1, "test tts the input tts handle is %x, ", parameter.hTTS);

	fp = MMI_FS_Open((const UINT8 *)L"D:/TTS.TXT", FS_READ_WRITE);
	if(fp >= 0)
	{
		MMI_FS_Read(fp, buffer, 256, &read);
		MMI_FS_Close(fp);
		AnsiiToUnicodeString( (S8*)uncode, buffer );

		if(read < 3)
		{
			encode = jtTTS_CODEPAGE_GBK;
		}else if(buffer[0] == UTF8_HEADER[0]
			&& buffer[1] == UTF8_HEADER[1]
			&& buffer[2] == UTF8_HEADER[2])
		{
			encode = jtTTS_CODEPAGE_UTF8;
		}else if(buffer[0] == UNICODE_HEADER[0]
			&& buffer[1] == UNICODE_HEADER[1])
		{
			encode = jtTTS_CODEPAGE_UNICODE;
		}else if(buffer[0] == UNICODE_BE_HEADER[0]
			&& buffer[1] == UNICODE_BE_HEADER[1])
		{
			encode = jtTTS_CODEPAGE_UNICODE_BE;
		}else
		{
			encode = jtTTS_CODEPAGE_GBK;
		}
	}else
	{
		strcpy(buffer, pTestText);
		read = strlen(pTestText);
		encode = jtTTS_CODEPAGE_GBK;
	}

	MMI_FS_Close(fp);


	mmi_trace(1, "tts test, you get txt from the txt is : %s , and the encode is %d", uncode, encode);

		//wav 头文件
	fHandle = MMI_FS_Open((const UINT8 *)L"D:/test.wav", FS_CREATE_ALWAYS);

	mmi_trace(1, "tts test, open the file the handle is:%x", fHandle);

	parameter.hTTS = hTTS;
	parameter.offset = 0;
	parameter.fHandle = fHandle;

	// WAV 文件写文件头
	memset(&wfh, 0, sizeof(wfh));
	mmi_trace(1, "tts test, the wave header size is:%d", sizeof(WAVE_FILE_HEADER));
	get_wav_format(hTTS, (kal_uint16 *)&wfh.nSamplesPerSec, (kal_int8 *)&wfh.wBitsPerSample);

	mmi_trace(1, "tts test, get the wave format nSamplesPerSec is:%d, the wBitsPerSample is: %d", wfh.nSamplesPerSec, wfh.wBitsPerSample);
	
	memcpy(wfh.chRIFF,"RIFF", 4);
	memcpy(wfh.chWAVE,"WAVE", 4);
	memcpy(wfh.chFMT, "fmt ", 4);
	memcpy(wfh.chDATA,"data", 4);
	wfh.dwFMTLen = 0x10;
	wfh.wFormatTag = 1;	// WAVE_FORMAT_PCM
	wfh.nChannels  = 1;	// mono
	wfh.nAvgBytesPerSec = wfh.wBitsPerSample * wfh.nSamplesPerSec / 8;
	wfh.nBlockAlign = (WORD)(wfh.wBitsPerSample / 8);

	MMI_FS_Seek(fHandle, 0, FS_FILE_BEGIN);
	MMI_FS_Write(fHandle, (void *)&wfh, (UINT)sizeof(WAVE_FILE_HEADER), &write);
	mmi_trace(1, "tts test, write the wave header size is:%d", write);

	// 设置输入文本编码
	error = jtTTS_SetParam(hTTS, jtTTS_PARAM_CODEPAGE, jtTTS_CODEPAGE_UNICODE);
	mmi_trace(1, "tts test, the set text code error is: %d", error);

	//设置为文本直接输入的方式
	error = jtTTS_SetParam(hTTS, jtTTS_PARAM_INPUTTXT_MODE, jtTTS_INPUT_TEXT_DIRECT);
	mmi_trace(1, "tts test, the set mode error is: %d", error);

	//error = jtTTS_SetParam(hTTS, jtTTS_PARAM_INPUT_CALLBACK, (long)jt_InputTextProc);

	error = jtTTS_SetParam(hTTS, jtTTS_PARAM_OUTPUT_CALLBACK, (long) jt_OutputVoiceProc);
	mmi_trace(1, "tts test, the set output callback error is: %d", error);

	error = jtTTS_SetParam(hTTS, jtTTS_PARAM_CALLBACK_USERDATA, (long)(&parameter));

	//error = jtTTS_Synthesize(hTTS);
	mmi_trace(1, "tts test, the set user data error is: %d", error);

	//获取tts的配置信息
	mmi_settings_tts_read_from_nvram(&ttsSetting);

	error = jtTTS_SetParam(hTTS, jtTTS_PARAM_VOLUME, volumes[ttsSetting.curTTSVolumeStatus]);

	error = jtTTS_SetParam(hTTS, jtTTS_PARAM_SPEAK_STYLE, styles[ttsSetting.curTTSStyleStatus]);

	error = jtTTS_SetParam(hTTS, jtTTS_PARAM_SPEED, speeds[ttsSetting.curTTSSpeedStatus]);

	error = jtTTS_SetParam(hTTS, jtTTS_PARAM_SOUNDEFFECT, effects[ttsSetting.curTTSEffectStatus]);

	error = jtTTS_SetParam(hTTS, jtTTS_PARAM_PITCH, tones[ttsSetting.curTTSTuneStatus]);

	//return;

	//P_TTS_PLAYER_PARAM pParam = (P_TTS_PLAYER_PARAM)pData;
	//error = jtTTS_SynthesizeText(hTTS, buffer, read);
	mmi_trace(1, "tts test, the text size is %d", nSize);
	mmi_trace_hex(1, nSize, pText);

	//初始化全局变量
	bBuffFull = 0;
	bPCMIdle = 1;
	bStopTTS = 0;
	nPosition = 0;
	
	error = jtTTS_SynthesizeText(hTTS, pText,  nSize);
	//error = jtTTS_SynthesizeText(hTTS, pTestText, strlen(pTestText));
	mmi_trace(1, "tts test, the synthe text error is: %d, and the file len is:%d", error, strlen(pTestText));

	MMI_FS_GetFileSize(fHandle, &fSize);
	wfh.dwDATALen = fSize - sizeof(WAVE_FILE_HEADER);
	wfh.dwRIFFLen = fSize - 8;
	MMI_FS_Seek(fHandle, 0, FS_FILE_BEGIN);
	MMI_FS_Write(fHandle, (void *)&wfh, (UINT)sizeof(WAVE_FILE_HEADER), &write);
	mmi_trace(1, "tts test, in the end write the wave header size is:%d", write);
	MMI_FS_Close(fHandle);

	if(bStopTTS == 0)
	{
		mdi_audio_suspend_background_play();
		mdi_audio_play_file((const UINT8 *)L"D:/test.wav", MDI_AUDIO_PLAY_ONCE, NULL, NULL);
	}
	
	//if(pHeap != NULL)
	//{
	//	OslMfree(pHeap);
	//}

	//COS_DeleteTask(g_hTTSPlayTask);

	//mdi_audio_init(void)
	//mdi_audio_init_event_hdlrs(void)
	//mdi_audio_play_file(void * file_name, U8 play_style, mdi_handle * handle_p, mdi_callback handler)
	
}

void ejTTS_PlayToPCM(char* pText, size_t nSize)
{	

	//long nSize = 0;
	
	//unsigned char *pHeap;

	jtErrCode error;

	//mdi_handle handle;

	jtParameter parameter;

	FS_HANDLE fp;
	char buffer[256];
	char uncode[256];
	INT32 read;
	long encode;

	//FS_HANDLE fHandle;
	WAVE_FILE_HEADER wfh;
	INT32 fSize;
	UINT write;

	nvram_tts_setting_struct ttsSetting;
	long volumes[] = 
	{
		jtTTS_VOLUME_MIN,
		jtTTS_VOLUME_MIN>>1,
		jtTTS_VOLUME_NORMAL,
		jtTTS_VOLUME_MAX>>1,
		jtTTS_VOLUME_MAX

	};

	long speeds[] = 
	{
		jtTTS_SPEED_MAX,
		jtTTS_SPEED_NORMAL,
		jtTTS_SPEED_MIN
	};

	long styles[] = 
	{
		jtTTS_SPEAK_STYLE_CLEAR,
		jtTTS_SPEAK_STYLE_NORMAL,
		jtTTS_SPEAK_STYLE_PLAIN,
		jtTTS_SPEAK_STYLE_VIVID
	};

	long effects[] = 
	{
		jtTTS_SOUNDEFFECT_BASE,
		jtTTS_SOUNDEFFECT_CHORUS,
		jtTTS_SOUNDEFFECT_ECHO,
		jtTTS_SOUNDEFFECT_NEARFAR,
		jtTTS_SOUNDEFFECT_REVERB,
		jtTTS_SOUNDEFFECT_ROBOT
	};

	long tones[] = 
	{
		jtTTS_PITCH_MIN,
		jtTTS_PITCH_NORMAL,
		jtTTS_PITCH_MAX
	};

	mmi_trace(1, "tts test, the task begin!");

	//如果TTS 没有初始化，则进行初始化。
	if(hTTS == 0)
	{
		ejTTS_Init();
	}

	memset(gTtsData, 0, TTS_BUFF_SIZE);

	mmi_trace(1, "test tts the input tts handle is %x, ", parameter.hTTS);

	//wav 头文件
	//fHandle = MMI_FS_Open((const UINT8 *)L"D:/test.wav", FS_CREATE_ALWAYS);

	//mmi_trace(1, "tts test, open the file the handle is:%x", fHandle);

	parameter.hTTS = hTTS;
	parameter.offset = 0;
	//parameter.fHandle = fHandle;

	// WAV 文件写文件头
	memset(&wfh, 0, sizeof(wfh));
	mmi_trace(1, "tts test, the wave header size is:%d", sizeof(WAVE_FILE_HEADER));
	get_wav_format(hTTS, (kal_uint16 *)&wfh.nSamplesPerSec, (kal_int8 *)&wfh.wBitsPerSample);

	mmi_trace(1, "tts test, get the wave format nSamplesPerSec is:%d, the wBitsPerSample is: %d", wfh.nSamplesPerSec, wfh.wBitsPerSample);
	
	memcpy(wfh.chRIFF,"RIFF", 4);
	memcpy(wfh.chWAVE,"WAVE", 4);
	memcpy(wfh.chFMT, "fmt ", 4);
	memcpy(wfh.chDATA,"data", 4);
	wfh.dwFMTLen = 0x10;
	wfh.wFormatTag = 1;	// WAVE_FORMAT_PCM
	wfh.nChannels  = 1;	// mono
	wfh.nAvgBytesPerSec = wfh.wBitsPerSample * wfh.nSamplesPerSec / 8;
	wfh.nBlockAlign = (WORD)(wfh.wBitsPerSample / 8);

	//MMI_FS_Seek(fHandle, 0, FS_FILE_BEGIN);
	//MMI_FS_Write(fHandle, (void *)&wfh, (UINT)sizeof(WAVE_FILE_HEADER), &write);
	//mmi_trace(1, "tts test, write the wave header size is:%d", write);

	// 设置输入文本编码
	error = jtTTS_SetParam(hTTS, jtTTS_PARAM_CODEPAGE, jtTTS_CODEPAGE_UNICODE);
	mmi_trace(1, "tts test, the set text code error is: %d", error);

	//设置为文本直接输入的方式
	error = jtTTS_SetParam(hTTS, jtTTS_PARAM_INPUTTXT_MODE, jtTTS_INPUT_TEXT_DIRECT);
	mmi_trace(1, "tts test, the set mode error is: %d", error);

	//error = jtTTS_SetParam(hTTS, jtTTS_PARAM_INPUT_CALLBACK, (long)jt_InputTextProc);

	error = jtTTS_SetParam(hTTS, jtTTS_PARAM_OUTPUT_CALLBACK, (long) jt_OutputVoicePCMProc);
	mmi_trace(1, "tts test, the set output callback error is: %d", error);

	error = jtTTS_SetParam(hTTS, jtTTS_PARAM_CALLBACK_USERDATA, (long)(&parameter));

	//error = jtTTS_Synthesize(hTTS);
	mmi_trace(1, "tts test, the set user data error is: %d", error);

	//获取tts的配置信息
	mmi_settings_tts_read_from_nvram(&ttsSetting);

	error = jtTTS_SetParam(hTTS, jtTTS_PARAM_VOLUME, volumes[ttsSetting.curTTSVolumeStatus]);

	error = jtTTS_SetParam(hTTS, jtTTS_PARAM_SPEAK_STYLE, styles[ttsSetting.curTTSStyleStatus]);

	error = jtTTS_SetParam(hTTS, jtTTS_PARAM_SPEED, speeds[ttsSetting.curTTSSpeedStatus]);

	error = jtTTS_SetParam(hTTS, jtTTS_PARAM_SOUNDEFFECT, effects[ttsSetting.curTTSEffectStatus]);

	error = jtTTS_SetParam(hTTS, jtTTS_PARAM_PITCH, tones[ttsSetting.curTTSTuneStatus]);

	//return;

	//P_TTS_PLAYER_PARAM pParam = (P_TTS_PLAYER_PARAM)pData;
	//error = jtTTS_SynthesizeText(hTTS, buffer, read);
	mmi_trace(1, "tts test, the text size is %d", nSize);
	mmi_trace_hex(1, nSize, pText);

	nPosition = 0;
	memcpy(gTtsData, (const void *)&wfh, sizeof(wfh));
	mdi_audio_stop_string();
	nPosition += sizeof(wfh);
	
	//初始化全局变量
	bBuffFull = 0;
	bPCMIdle = 1;
	bStopTTS = 0;
	

	error = jtTTS_SynthesizeText(hTTS, pText,  nSize);
	//error = jtTTS_SynthesizeText(hTTS, pTestText, strlen(pTestText));
	mmi_trace(1, "tts test, the synthe text error is: %d, and the file len is:%d", error, strlen(pTestText));

	//MMI_FS_GetFileSize(fHandle, &fSize);
	wfh.dwDATALen = nPosition;
	wfh.dwRIFFLen = nPosition + sizeof(wfh) - 8;
	//MMI_FS_Seek(fHandle, 0, FS_FILE_BEGIN);
	//MMI_FS_Write(fHandle, (void *)&wfh, (UINT)sizeof(WAVE_FILE_HEADER), &write);
	//mmi_trace(1, "tts test, in the end write the wave header size is:%d", write);
	//MMI_FS_Close(fHandle);
	memcpy(gTtsData, (const void *)&wfh, sizeof(wfh));

	if(bStopTTS == 0)
	{
		mmi_trace(1, "tts test, the wav buff size is: %d.", nPosition);
		mdi_audio_suspend_background_play();
		//mdi_audio_play_file((const UINT8 *)L"D:/test.wav", MDI_AUDIO_PLAY_ONCE, NULL, NULL);
		//为什么要除以4
		mdi_audio_play_string(gTtsData, nPosition/4, MDI_FORMAT_WAV, MDI_AUDIO_PLAY_ONCE, NULL, playStringCallBack);
	}
	
	//if(pHeap != NULL)
	//{
	//	OslMfree(pHeap);
	//}

	//COS_DeleteTask(g_hTTSPlayTask);

	//mdi_audio_init(void)
	//mdi_audio_init_event_hdlrs(void)
	//mdi_audio_play_file(void * file_name, U8 play_style, mdi_handle * handle_p, mdi_callback handler)
	
}


//*
int ejTTSPlayerTask(char* pText, size_t nSize)
{

	TTS_PLAYER_PARAM param;
	
	param.nSize = nSize;
	//param.pText = OslMalloc(nSize);
	memset(param.pText, 0, 512);
	memcpy(param.pText, pText, nSize);
	param.pText[nSize] = '\0';

	mmi_trace(1, "tts test, create a new task!");

	mmi_trace_hex(1, nSize, param.pText);
	/*
	mmi_trace(1, "tts test, create a new task!");
	g_hTTSPlayTask  = COS_CreateTask(ejTTS_PlayToFile,
		&param, NULL, 1024*1024, 
		DEV_TASK_PRIORITY, COS_CREATE_DEFAULT,
		0, "ejTTS_PlayToFile");
	mmi_trace(1, "tts test, create a new task end!");
	//*/

	//ejTTS_PlayToFile(&param);
	//OslMfree(param.pText);
	//param = NULL;

	return 0;

}
//*/

int ejTTSPlayerStop(void)
{
	bStopTTS = 1;
	//mdi_audio_stop_file();
	mdi_audio_stop_string();
	mdi_audio_resume_background_play();
}

#ifdef __cplusplus
}
#endif
