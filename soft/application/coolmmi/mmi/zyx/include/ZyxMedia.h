#ifndef __ZYX_MEDIA_H__
#define __ZYX_MEDIA_H__

#include "zyxtypes.h"

typedef enum
{
    ZYX_AUDIO_FORMAT_AAC,
    ZYX_AUDIO_FORMAT_AMR
}ZYX_MEDIA_TYPE;

typedef enum
{
    ZYX_MEDIA_DATA_REQUEST, //播放数据请求
    ZYX_MEDIA_END,            //播放结束
    ZYX_MEDIA_ERROR,         //播放错误
    ZYX_MEDIA_TERMINATED,    //播放被终止
    ZYX_MEDIA_UNKNOWN
}ZYX_MEDIA_EVENT;

#define AAC_STATE_PLAY 1
#define AAC_STATE_PAUSE 2
#define AAC_STATE_STOP 3

typedef int (*ZyxAudioHandler)(ZYX_MEDIA_EVENT param);

extern int ZyxAudioOpen(PU8 audio_buf,int size,ZYX_MEDIA_TYPE format, ZyxAudioHandler cb);
extern int ZyxAudioPlay(void);
extern int ZyxAudioStop(void);
extern int ZyxAudioPause(void);
extern int ZyxAudioResume(void);
extern int ZyxGetAudioState(void);

extern void ZyxSetVolume(U8 volume);
extern int ZyxAudioGetCurrentTime(void);

extern void ZyxAudioGetWriteBuffer(PU8* buf_ptr,int *len);
extern void ZyxAudioWriteDone(int len);
extern void ZyxAudioDataFinish (void);

#endif
