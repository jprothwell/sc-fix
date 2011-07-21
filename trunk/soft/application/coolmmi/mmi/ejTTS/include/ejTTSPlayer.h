#ifndef _TTS_PLAYER_DEMO_H_
#define _TTS_PLAYER_DEMO_H_

#ifdef __cplusplus
extern "C" {
#endif

//void TTS_PlayMain(void);
extern int ejTTS_Init();
extern int ejTTSPlayerToFile(char* pText,  size_t nSize);
extern int ejTTSPlayerToPCM(char* pText,  size_t nSize);
extern int ejTTSPlayerStop();

#ifdef __cplusplus
}
#endif

#endif