#ifndef MEDIA_OTHERS_H_
#define MEDIA_OTHERS_H_

#include "l1audio.h"
//#include "kal_non_specific_general_types.h"
#include "lcddr.h"

/* Define constants for use in service parameters.  */

#define         NU_AND                          2
#define         NU_AND_CONSUME                  3
#define         NU_DISABLE_TIMER                4
#define         NU_ENABLE_TIMER                 5
#define         NU_FALSE                        0
#define         NU_FIFO                         6
#define         NU_FIXED_SIZE                   7
#define         NU_NO_PREEMPT                   8
#define         NU_NO_START                     9
#define         NU_NO_SUSPEND                   0
#define         NU_NULL                         0
#define         NU_OR                           0
#define         NU_OR_CONSUME                   1
#define         NU_PREEMPT                      10
#define         NU_PRIORITY                     11
#define         NU_START                        12
#define         NU_SUSPEND                      0xFFFFFFFFUL
#define         NU_TRUE                         1
#define         NU_VARIABLE_SIZE                13

#define  L1SP_LNA_0           0
#define  L1SP_LNA_1           1
#define  L1SP_LNA_FMRR        3


//typedef struct {
//   union {
//      Media_SMF_Param      smf;
//      Media_iMelody_Param  imy;
//      uint32               vm;
//   } param;
//   int32          max_swing;        /* 0 ~ 32767                     */
//   int8           key_shift;
//   uint8          level;            /* Digital volume control 0~100 */
//   uint8          gpt;
//   kal_bool           in_speech;
//   Media_Format   format;
//   Media_Event    end_status;
//   uint16         rb_threshold;
//   /* DSP ring buffer control variables */
//   uint16         dsp_rb_base;
//   uint16         dsp_rb_size;
//   uint16         dsp_rb_end;
//   uint16         dsp_last_sample;  /* The last sample written to DSP buffer by MCU, for last-sample padding */
//   /* Ring buffer control variables */
//   kal_bool           eof;              /* Flag indicating End-Of-File   */
//   kal_bool           ob_flag_r;        /* if ob_flag_r is set, then the odd_byte for read is stored in odd_byte_r */
//   kal_bool           ob_flag_w;        /* if ob_flag_w is set, then the odd_byte for write is stored in odd_byte_w */
//   uint8          odd_byte_r;
//   uint8          odd_byte_w;
//   uint16         *rb_base;         /* Pointer to the ring buffer    */
//   uint32         rb_size;          /* Size of the ring buffer(word) */
//   uint32         write;            /* R/W by MMI, R by WAV driver   */
//   uint32         read;             /* R by MMI, R/W by WAV driver   */
//   uint32         file_len;   /* for wav recordig */
//#ifdef   DSP_WT_SYN
//   int16          Media_FIR_Melody32K_1[45];
//   int16          Media_FIR_Melody32K_2[45];
//#endif
//   kal_bool           isDataConsumed;
//   kal_bool           is_stereo;
//   kal_bool           mono2stereo;
//} mediaControl;

#endif
