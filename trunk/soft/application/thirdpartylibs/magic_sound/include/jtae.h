/* Copyright (C) 2010-2050, SinoVoice Ltd.
*
* �ó����ļ���audio effect��ͷ�ļ���
*/

#ifndef __SINO_VOICE__EJAE__H__
#define __SINO_VOICE__EJAE__H__

#define jtAEAPI 

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    jtAE_ERR_NONE,
    jtAE_ERR_NOTINIT,
    jtAE_ERR_PARAM,
    jtAE_ERR_MEMORY,
}jtAEErr;

typedef enum
{
    jtAE_PARAM_SAMPLE,
    jtAE_PARAM_AUDIOEFFECT,
    jtAE_PARAM_TIMBRE,
    jtAE_PARAM_VOLUME,
}jtAEParam;

#define jtAE_SAMPLE_8K             0       // 8K
#define jtAE_SAMPLE_16K            1       // 16K 

#define jtAE_AUDIOEFFECT_BASE      0       // ����Ч
#define jtAE_AUDIOEFFECT_REVERB    1       // ����
#define jtAE_AUDIOEFFECT_ECHO      2       // ����
#define jtAE_AUDIOEFFECT_CHORUS    3       // �ϳ�
#define jtAE_AUDIOEFFECT_NEARFAR   4       // ��Զ����
#define jtAE_AUDIOEFFECT_ROBOT     5       // ������

#define jtAE_TIMBRE_BASE           0       // ԭ��ɫ
#define jtAE_TIMBRE_MALE           1       // ����
#define jtAE_TIMBRE_FEMALE         2       // Ů��
#define jtAE_TIMBRE_CHILD          3       // ��ͯ

#define jtAE_MIN_BUF_LEN           25600   // ��ʼ������ѿռ��С(Byte)

/** 
* @brief   jtAE_Init
* 
* init audio effect engine
* 
* @param   unsigned long *dwHandle [out] handle to audio effect engine
* @return  jtAEParam 
*/
jtAEErr jtAEAPI jtAE_Init(unsigned long  *dwHandle, void *pvBuf);

/** 
* @brief   jtAE_SetParam
* 
* set param
* 
* @param   unsigned long dwHandle  - [in] handle to audio effect engine
* @param   jtAEParam     nParam    - [in] parameter index
* @param   long          iValue    - [in] parameter value
* @return  jtAEParam  
*/
jtAEErr jtAEAPI jtAE_SetParam(unsigned long    dwHandle,
                              jtAEParam        nParam,
                              long             iValue);
                              
/** 
* @brief   jtAE_GetParam
* 
* get param
* 
* @param   unsigned long dwHandle  - [in] handle to audio effect engine
* @param   jtAEParam     nParam    - [in] parameter index
* @param   long*        piValue    - [out] buffer to receive the parameter value
* @return  jtAEParam  
*/
jtAEErr jtAEAPI jtAE_GetParam(unsigned long    dwHandle,
                              jtAEParam        nParam,
                              long             *piValue);
                              
/** 
* @brief   jtAE_Progress
* 
* audio effect
* 
* @param   unsigned long dwHandle - [in] handle to audio effect engine
* @param   short *panData         - [in/out] audio data
* @param   int nDataLen           - [in]     audio data length
* @return  jtAEParam
*/
jtAEErr jtAEAPI jtAE_Progress(unsigned long   dwHandle,
                              short *panData, 
                              int   nDataLen);
                              
#ifdef __cplusplus
}; // extern "C"
#endif

#endif // __SINO_VOICE__EJAE__H__

