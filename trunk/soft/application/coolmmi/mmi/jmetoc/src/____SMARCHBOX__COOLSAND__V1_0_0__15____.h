/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__15____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// ������� /////////////////////////////////////////////////////////////
 
enum{
   MDI_FORMAT_NONE=-1,
   MDI_FORMAT_GSM_FR,                /* 0 */
   MDI_FORMAT_GSM_HR,               /* 1 */
   MDI_FORMAT_GSM_EFR,               /* 2 */
   MDI_FORMAT_AMR,                      /* 3 */
   MDI_FORMAT_AMR_WB,                  /* 4 */
   MDI_FORMAT_DAF,                     /* 5 */
   MDI_FORMAT_AAC,                     /* 6 */
   MDI_FORMAT_PCM_8K,                  /* 7 */
   MDI_FORMAT_PCM_16K,                 /* 8 */
   MDI_FORMAT_G711_ALAW,            /* 9 */
   MDI_FORMAT_G711_ULAW,            /* 10 */
   MDI_FORMAT_DVI_ADPCM,            /* 11 */
   MDI_FORMAT_VR,                   /* 12 */
   MDI_FORMAT_WAV,                  /* 13 */
   MDI_FORMAT_WAV_ALAW,             /* 14 */
   MDI_FORMAT_WAV_ULAW,              /* 15 */
   MDI_FORMAT_WAV_DVI_ADPCM,       /* 16 */
   MDI_FORMAT_SMF,                   /* 17 */
   MDI_FORMAT_IMELODY,            /* 18 */
   MDI_FORMAT_SMF_SND,              /* 19 */
   MDI_FORMAT_MMF,                 /* 20 */
   MDI_FORMAT_AU,                 /*  21 */
   MDI_FORMAT_AIFF,                 /*  22 */
   MDI_FORMAT_M4A,                   /*  23 */
   MDI_FORMAT_3GP,                     /* 24 */ 
   MDI_FORMAT_MP4,	                  /* 25 */
   MDI_FORMAT_JPG,	                     /* 26 */
   MDI_FORMAT_GIF,	                     /* 27 */
   MDI_FORMAT_MJPG,	                  /* 28 */
   MDI_FORMAT_WMA,                  /* 29 */
   MDI_FORMAT_MID,                  /* 30 */
   MDI_NO_OF_FORMAT
};

/* Define the audio play style. */
typedef enum {
    DEVICE_AUDIO_PLAY_CRESCENDO  = 0 ,  /* Play sound for crescendo. */
    DEVICE_AUDIO_PLAY_INFINITE       ,  /* Play sound for infinite. */
    DEVICE_AUDIO_PLAY_ONCE           ,  /* Play sound for once. */
    DEVICE_AUDIO_PLAY_DESCENDO         /* Play sound for descendo. */
}audio_play_style_enum;

#define MDI_DEVICE_SPEAKER2            4        /* earphone, carkit */
#define MDI_DEVICE_LOUDSPEAKER         5        /* loudspeaker for free sound */

typedef S32 mdi_result;
extern mdi_result mdi_audio_stop_all(void);
typedef S32 mdi_handle;
typedef void (*mdi_callback) (mdi_result result);
extern mdi_result mdi_audio_play_string_with_vol_path_non_block(
                    void *audio_data,
                    U32 len,
                    U8 format,
                    U8 play_style,
                    mdi_handle *handle_p,
                    mdi_callback handler,
                    U8 volume,
                    U8 path);

#define JMETOC_PLAY_AUDIO_WAV(raw_data, size, play_style)                     \
      mdi_audio_play_string_with_vol_path_non_block(  (void*)raw_data,     \
                                                      (U32)size,           \
                                                      MDI_FORMAT_WAV,      \
                                                      play_style,          \
                                                      NULL,                \
                                                      NULL,                \
                                                      3,                   \
                                                      MDI_DEVICE_LOUDSPEAKER)

#define JMETOC_PLAY_AUDIO_MIDI(raw_data, size, play_style)                    \
      mdi_audio_play_string_with_vol_path_non_block(  (void*)raw_data,     \
                                                      (U32)size,           \
                                                      MDI_FORMAT_MID,      \
                                                      play_style,          \
                                                      NULL,                \
                                                      NULL,                \
                                                      3,                   \
                                                      MDI_DEVICE_LOUDSPEAKER)

extern com_smarch_mmedia_BasicPlayer* jmetoc_player;       // �������һ�εĲ�������

/**
 * ����������
 * <p>
 * ���������ݵ� com_smarch_mmedia_BasicPlayer �����У�
 * ���Ա com_smarch_mmedia_BasicPlayer_loopCount �����˲��ŵ�ѭ��������-1Ϊ����ѭ����
 * ���Ա com_smarch_mmedia_BasicPlayer_state �����˲������ĵ�ǰ״̬��
 * ���Ա com_smarch_mmedia_BasicPlayer_data Ϊһ���������ݽṹ���ǲ��ŵ�ʵ�����ݡ�
 * </p>
 * @param player ����������
 * @param midi �Ƿ�Ϊ MIDI��true Ϊ MIDI��false Ϊ wav��ĿǰֻҪ֧�����߼ȿ�
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
void com_smarch_mmedia_play(com_smarch_mmedia_BasicPlayer* player, JMETOC_BOOL midi) {
  java_lang_Array* a = player->com_smarch_mmedia_BasicPlayer_data;
  s32 style = player->com_smarch_mmedia_BasicPlayer_loopCount == -1 ? DEVICE_AUDIO_PLAY_INFINITE : DEVICE_AUDIO_PLAY_ONCE;
  s32 midresult;
  if (midi) {
    midresult = JMETOC_PLAY_AUDIO_MIDI(a->array, a->len, style);
    jmetoc_player = player;
  } else {
    midresult = JMETOC_PLAY_AUDIO_WAV(a->array, a->len, style);
    jmetoc_player = player;
  }
  player->com_smarch_mmedia_BasicPlayer_state = javax_microedition_media_Player_STARTED;
}

/**
 * ֹͣ�������š�
 * @param player ����������
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
void com_smarch_mmedia_stop(com_smarch_mmedia_BasicPlayer* player) {
  mdi_audio_stop_all();
  player->com_smarch_mmedia_BasicPlayer_state = javax_microedition_media_Player_CLOSED;
  jmetoc_player = null;
}

/**
 * �ر��������š�
 * <p>
 * ͬʱ��Ҫ�ͷ��������ݵĴ洢��
 * </p>
 * @param player ����������
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
void com_smarch_mmedia_close(com_smarch_mmedia_BasicPlayer* player) {
  java_lang_Array* a = player->com_smarch_mmedia_BasicPlayer_data;
  if (a) {
    com_smarch_mmedia_stop(player);
    java_lang_Runtime_mfree((void*)a->array);
    java_lang_Runtime_mfree(a);
    a->array = null;
    player->com_smarch_mmedia_BasicPlayer_data = null;
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
