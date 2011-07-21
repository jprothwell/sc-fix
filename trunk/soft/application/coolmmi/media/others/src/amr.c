#include "media.h" /* wufasong deleted 2007.06.17 */

#if 1
//#if  IS_CHIP_MT6218_AND_LATTER_VERSION || IS_EDGE_CHIP_MT6229_AND_LATTER_VERSION

#include "amr_table.h"
#include "am.h"

#if 1
//#if IS_CHIP_MT6228_AND_LATTER_VERSION  || IS_EDGE_CHIP_MT6229_AND_LATTER_VERSION
#define DSP_IDLE                     0
#define DSP_INITIALIZED              1
#define DSP_PLAYING                  2
#define DSP_RECORDING                2
#define DSP_STOP                     3
#endif

#define DEBUG_AMR_RAM_FILE 0
#define DEBUG_AMR_ROM_FILE 0
#define DEBUG_AMR_DSP 0

/* Turn on these only when determining the most appropriate size of the ring buffer */
#define AMR_CHECK_BUFFER_UNDERFLOW 0
#define AMR_CHECK_BUFFER_OVERFLOW 0

#define AMR_WRITE_FILE_HEADER 0
#define AMR_READ_FILE_HEADER 0

#define AMR_STATE_RECORD 1
#define AMR_STATE_RECORD_PAUSE 2
#define AMR_STATE_PLAY 3
#define AMR_STATE_PLAY_PAUSE 4
#define AMR_STATE_STOP 5


//delect by licheng for code clean
//static kal_uint16 Tx_ordering_buffer[30];
//static kal_uint16 Tx_data_buffer[30];

/* Note: can't record with odd frame length, unless modify ring buffer mechanism in media.c */

static struct{
   mediaControl   *ctrl;
   kal_uint8      state;
   kal_uint16     wType;
   kal_uint8      bCodecMode; /* Codec Mode for DSP Sherif Setting */
   kal_bool       bWideBand;
   kal_uint32     uRecordOverflowCount; /* number of frames that is waiting to be put into ring buffer */
   kal_bool       bOddFlag;
   kal_uint8      bOddData; /* buffers/store one byte when read/write from/to ring buffer */
   kal_bool       dedicated_mode;
#if DEBUG_AMR_RAM_FILE
   kal_uint8      bRamFileBuffer[80000];
   kal_uint32     uRamFileOffset;
#endif
} amr;

int amrGetDataCount( void )
{
   int32 count = 0;


   return count;
}
//delect by licheng for code clean
/*
static void media_playback_error() {

}
*/
void media_buffer_put_byte(kal_uint8 bData) {

}

/* to trigger the update of write pointer, so that
the last byte can be recorded */
void media_buffer_flush()
{

}

void media_buffer_put_2bytes(kal_uint16 wData)
{

}

void media_buffer_put_nbytes(kal_uint8 *pBuf, int n) {

}
//delect by licheng for code clean
/*
static void amr_bit_ordering_sherif_to_file(kal_uint8 bTxType, kal_uint8 bFrameType, kal_uint16* pInData, kal_uint16* pOutData, kal_uint16 wLenBits)
{

}
*/
void amrRecordHisr( void )
{

}

kal_uint8 media_buffer_get_byte() {

   return 0;
}

kal_uint8 media_buffer_peek_byte() {

   return 0;
}

kal_uint16 media_buffer_get_2bytes() {

   return 0;
}

void media_buffer_get_nbytes(kal_uint8 *buf, kal_uint32 uLen) {

}

kal_uint8 media_buffer_peek_nbytes(kal_uint8 *buf, kal_uint32 uLen) {

   return 0;
}
//delect by licheng for code clean
/*
static void amr_write_speech_mode_to_dsp(kal_uint8 bTxType)
{

}

static void amr_write_speech_data_to_dsp(kal_uint16 *pwData, kal_uint16 bLen)
{

}

static void amr_bit_ordering_file_to_sherif(kal_uint8 bTxType, kal_uint8 bFrameType, kal_uint16* pInData, kal_uint16* pOutData, kal_uint16 wLenBits)
{

}
*/
void amrPlaybackHisr( void )
{

}

void amrRecord( mediaControl *ctrl, kal_uint16 type )
{

}

kal_uint16 amrSetAndGetInitialCodecMode()
{
   kal_uint16 wInitialCodecMode;
#if IS_CHIP_MT6219_AND_LATTER_VERSION || IS_EDGE_CHIP_MT6229_AND_LATTER_VERSION
   if (KAL_TRUE==amr.bWideBand) {
      amr.bCodecMode = 0x20;
      wInitialCodecMode = (0x20<<2);
   } else
#endif
   {
      amr.bCodecMode = 0x03;
      wInitialCodecMode = (0x03<<2) | (0x03<<6);
   }
   return wInitialCodecMode;
}

Media_Status amrPlay( mediaControl *ctrl )
{


   return MEDIA_SUCCESS;
}


void amrStop( void )
{

}

void amrPause(mediaControl *ctrl)
{

}

void amrResume(mediaControl *ctrl)
{

}

void amrUpdateLevel()
{

}

#endif   // IS_CHIP_MT6218_AND_LATTER_VERSION || IS_EDGE_CHIP_MT6229_AND_LATTER_VERSION


