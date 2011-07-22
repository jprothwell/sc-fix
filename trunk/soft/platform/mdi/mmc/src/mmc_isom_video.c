

/**************************
mmc_isom_video.c
for isomedia play (mp4 / 3gp)
author shenh 
2008.2.26



***************************/
#include "global.h"

#ifdef MP4_3GP_SUPPORT

#include "cswtype.h"
#include "cs_types.h"
#include "cpu_share.h"
#include "vpp_audiojpeg_dec.h"
#include "mmc.h"

#include "hal_aif.h"
#include "aud_m.h"
#include "di.h"
#include "mci.h"

#include "mmc_timer.h"
#include "sxs_type.h"
#include "sxr_sbx.h"
#include "sxr_tls.h"

#include "mmc_isom_video.h"

#include "event.h"
#include "hal_timers.h"
#include "lcdd_m.h"
#include "hal_overlay.h"
#include "mcip_debug.h"

#include "mmc_audiompeg.h"
#ifdef MPEG4_SUPPORT
#include "xvid_decraw.h" 
#endif

#ifdef H263_SUPPORT
#include "h263_global.h"
#endif
#include "mmi_jpeg_dec.h"

#if (CHIP_HAS_GOUDA == 1)
#include "hal_gouda.h"
#include "hal_speech.h"
#include "vpp_speech.h"
#endif

#ifdef RM_SUPPORT
#include "rm_dec.h"
#include "voc_cfg.h"
#endif

#include "global_macros.h"
#include "sys_ctrl.h"


//for mp4
typedef enum
{
	//Video player state.
	VID_PLAY_STATE_IDLE=0,
	VID_PLAY_STATE_PLAYING,
	VID_PLAY_STATE_PAUSE,
	
	//invalid after.
	VID_PLAY_STATE_INVALID 
}VID_PLAY_STATE;

#define LIMIT_FPS_15 15
#define LIMIT_WIDTH_128 128
#define LIMIT_HEIGHT_96 96
#define LIMIT_WIDTH_176 176
#define LIMIT_HEIGHT_144 144
#define LIMIT_WIDTH_240 240
#define LIMIT_HEIGHT_180 180
#define LIMIT_WIDTH_352 352
#define LIMIT_HEIGHT_288 288
#define LIMIT_WIDTH_320 320
#define LIMIT_HEIGHT_240 240

#define AAC_HEAD_SIZE 7
#define AMR_HEAD "#!AMR\n"
#define AMR_HEAD_SIZE 6

typedef enum {
A_TYPE_MPEG2_AAC=0,
A_TYPE_MPEG4_AAC,
A_TYPE_AAC,
A_TYPE_AMR,
A_TYPE_MP3,
A_TYPE_PCM,
A_TYPE_ADPCM,
A_TYPE_COOK,
A_TYPE_RA,

V_TYPE_H263,
V_TYPE_MPEG4,
V_TYPE_MJPEG,
V_TYPE_AVI_XVID,
V_TYPE_AVI_DX50,
V_TYPE_AVI_DIV3,
V_TYPE_RV,

AV_TYPE_INVALID
}AV_TYPE_ENUM;

#define MAX_SLOW_NUM 2
#define AAC_FRAME_SAMPLE 1024
#define AMR_FRAME_SAMPLE 160
#define MP3_FRAME_SAMPLE_1152 1152 
#define MP3_FRAME_SAMPLE_576 576
#define PCM_FRAME_SAMPLE 1024//user define

//NOTE:pcm pingpang buffer  must be less than 32KB
#ifdef RM_SUPPORT
#define PCM_BUF_AAC_FRAME_NUM (2*2)
#else
#define PCM_BUF_AAC_FRAME_NUM (2*2)
#endif
#define PCM_BUF_AMR_FRAME_NUM (10*2)
#define PCM_BUF_MP3_FRAME_NUM (2*2)
#define PCM_BUF_PCM_FRAME_NUM (2*2)//user define
#define PCM_BUF_COOK_FRAME_NUM (2*2)

#ifdef RM_SUPPORT
#define EXTRA_PCM_BUF_NUM 1 //use above half num as unit.
#else
#define EXTRA_PCM_BUF_NUM 1 //use above half num as unit.
#endif

#define MAX_V_SAMPLEBUF_SIZE (1024*40) //1024*20 for QCIF .1024*40 for 240x180. will change with image width and height 
#define MAX_A_SAMPLEBUF_SIZE (1024*2*3)
#define MAX_A_SAMPLEBUF_MIRROR_SIZE 1024
#ifdef RM_SUPPORT
#define MAX_A_SAMPLEBUF_PIPE_SIZE (1024*6)
#else
#define MAX_A_SAMPLEBUF_PIPE_SIZE (1024*6)
#endif
#define PIPE_SPACE 4

#define SYNC_COEFF_SCALE_BIT 8
#define V_FPS_SCALE_BIT 8

#define SYNC_TIME_OFFSET 500 // time distance for next sync. (ms)

static unsigned char aacHead[AAC_HEAD_SIZE];

static volatile uint16 play_state=VID_PLAY_STATE_INVALID;

extern HAL_AIF_STREAM_T audioStream;
extern AUD_LEVEL_T audio_cfg;
extern AUD_ITF_T    audioItf;
extern MPEG_PLAY MpegPlayer;

extern const uint8 silence_frame475[];

#if (CHIP_HAS_GOUDA != 1)
#define USE_VOC_ROTATE
#endif
#define USE_KEY_SYNC
#define USE_OVERLAY
static uint8 USE_SLEEP;

#if (CSW_EXTENDED_API_LCD == 1)
static LCDD_FBW_T lcdDraw;
#endif

#if (CHIP_HAS_GOUDA == 1)
HAL_GOUDA_VID_LAYER_DEF_T Vid_Video;
static volatile uint32 RX_BUF_SWAP_ID;//pingpang
#endif

IsomPlayGlobal *pIsomGlob;

#define VOC_WORK_NON 0
#define VOC_WORK_AUD 1
#define VOC_WORK_VID 2

static vpp_AudioJpeg_DEC_IN_T vocDecAud;
#if (CHIP_HAS_GOUDA != 1)
static vpp_AudioJpeg_DEC_IN_T vocDecVid;
#endif
static volatile int16 VOC_VID;
static vpp_AudioJpeg_DEC_OUT_T vocDecStatus;
#define VOC_WORK_CONT (0x2000000)//total time=xxx ms 
static volatile uint8 VOC_OPEN;
static volatile uint8 VOC_SETTING;
static volatile uint8 VOC_WORK;
static uint32 VOC_FREQ_STATE;
#ifdef VID_XCPU_FREQ_SET
static uint32 XCPU_FREQ_STATE;//0=normal
#endif

static volatile int16 VOC_AUD;
static volatile uint8 DISPLAY_INTERFACE;//0=not display time bar. 1=only display time bar. 2=display time bar with image area.
static volatile uint16 AUD_DEC_FRM_NUM;
static volatile uint16 AUD_DEC_MIN;//voc decode AUD_DEC_MIN audio frame then decode 1 frame video. default 2, max half of the pcm buf. will be change in playing.
static volatile uint8 PCM_ISR_FLAG;
static volatile int16 AUD_DATA_REQ;//0 =no need, 1= need, 2=reposition && need
static volatile int16 VID_DEC_REQ;//+ need more decode; - too quick, skip decode
static volatile int16 SEND_EVENT_NUM;//avoid mail box full
static volatile int16 DISPLAY_SEND_EVENT_NUM;//avoid mail box full
static uint8 DTS_FLG;//test

static uint16 MAX_KEEP_SKIP_F_NUM;
static uint16 keep_skip_frame_num;// B frame skip number continuous

typedef struct
{
	volatile uint8 done;// 0=need dec, 1=dec done
	volatile uint8 *pBufPos;
}aud_extra_pcmbuf;
//static volatile uint8 AUD_EXTRA_BUF_DONE;// 0=need dec, 1=dec done
static aud_extra_pcmbuf aud_extra_pcmbuf_state[EXTRA_PCM_BUF_NUM];//more, pingpang or only one
static volatile uint8 A_EXBF_RD_ID;// will read from
static volatile uint8 A_EXBF_WT_ID;// will write to.
static int8 DISPLAY_STATE;// 1=in 0=out
static int8 AV_DEC_STATE;// 1=in 0=out
static uint8 NEED_BLEND_IMG;//1=copy voc zoom data to lcd buffer. 0=not

#define VID_ERR_FRAME_NUM 25 // max continuous error video frame number.
static uint16 VID_ERR_FRAME_REC;// record continuous error video frame number.


static volatile int8   IMAGE_INBUF_FLAG;//for voc judge
static volatile uint16 IMAGE_W_INBUF0;
static volatile uint16 IMAGE_H_INBUF0;
static volatile uint16 IMAGE_W_INBUF1;
static volatile uint16 IMAGE_H_INBUF1;

#ifdef USE_KEY_SYNC
static uint32 KEY_SAMP_NUM;
static int32 KEY_SAMP_TIME_WAIT;
static int32 KEY_SAMP_TIME_START;
#endif

static uint32 SYNC_NUM_COEFFICIENT;
static volatile uint32 LAST_SYNC_AUD_FRM_NUM;//reuse in rmvb for record pcm sample num.
#if (defined MPEG4_SUPPORT || defined H263_SUPPORT)
static zoomPar *pZoom;
#endif
static volatile uint32 pPcmBufPos;
static uint32 TIME_LINE_CUR; // for record time start for video sync without audio play.
static uint32 TIME_LINE_LAST;

static volatile uint8 VID_SET_MODE=0;

#ifdef USE_OVERLAY
HAL_OVERLAY_ID_T VID_OVERLAY_ID;
#endif

#ifdef RM_SUPPORT
extern vpp_data_struct vpp_data_buffer[2];
extern volatile INT16  vpp_buffer_rd_id;
extern VPP_RMVB_CFG_T  vpp_rmvb_cfg;
static volatile uint8 needRA;
#endif

static volatile uint16 AUD_FADE_IN_FLAG;//zero frame num
static volatile uint16 AUD_FADE_OUT_FLAG;// 3 step. 3=pcm isr check and process fade out. 2=fade out will be play. 1=fade out over.

extern VOID mmc_MemCheck(BOOL bprintinfo);
extern char * mmc_MemMalloc(int32 nsize);
extern void mmc_MemFreeAll();
extern int mmc_MemGetFree();
static void vid_display(void) ;

#ifdef RM_SUPPORT
extern void vpp_RMVBDecScheduleOneBlk(VPP_RMVB_CFG_T *dec_in_ptr);
#endif

void (*fillAudioData)();//be call in video decoder
uint16 MBScale4AudRed=50;//MB number scale

#if (CHIP_HAS_GOUDA == 1)

#define SETUPGOUDA  {\
	uint32 tk=0;\
	while(hal_GoudaIsActive())\
	{\
		tk++;\
		if(tk>0x2000000)\
		{\
			diag_printf("[VID]gouda err!");\
			break;\
		}\
	}\
	hal_GoudaVidLayerClose();\
	Vid_Video.addrY = (UINT32*)(pIsomGlob->v_YuvInfo[0]+(uint32)pIsomGlob->v_cutY_origin*pIsomGlob->v_YuvInfo[3]+pIsomGlob->v_cutX_origin);\
	Vid_Video.addrU = (UINT32*)(pIsomGlob->v_YuvInfo[1]+(uint32)(pIsomGlob->v_cutY_origin>>1)*(pIsomGlob->v_YuvInfo[3]>>1)+(pIsomGlob->v_cutX_origin>>1));\
	Vid_Video.addrV = (UINT32*)(pIsomGlob->v_YuvInfo[2]+(uint32)(pIsomGlob->v_cutY_origin>>1)*(pIsomGlob->v_YuvInfo[3]>>1)+(pIsomGlob->v_cutX_origin>>1));\
	Vid_Video.stride = (UINT16)pIsomGlob->v_YuvInfo[3];\
	Vid_Video.height = (UINT16)pIsomGlob->v_cutH_origin;\
	Vid_Video.width = (UINT16)pIsomGlob->v_cutW_origin;\
	Vid_Video.pos.tlPX = pIsomGlob->v_lcd_start_x;\
	Vid_Video.pos.tlPY = pIsomGlob->v_lcd_start_y;\
	Vid_Video.pos.brPX =pIsomGlob->v_lcd_start_x+pIsomGlob->v_cutW-1;\
	Vid_Video.pos.brPY =pIsomGlob->v_lcd_start_y+pIsomGlob->v_cutH-1;\
	hal_GoudaVidLayerOpen(&Vid_Video);\
}
#endif                           

#if (CHIP_HAS_GOUDA != 1)
/*
将565rgb数据拷贝("嵌入")到LCD Buffer中,以便调用LCD的显示函数进行bitmap的显示.
**/
static void EmbedRgbDataIntoLcdBuffer(
	int16 LcdFrameBuffer_x, int16 LcdFrameBuffer_y, 
	uint16 bmp_width, uint16 bmp_height, 
	uint16 cut_x, uint16 cut_y, 
	uint16 cut_w, uint16 cut_h, 
	uint16 *pbmp, uint16 *pLcdShadowBuffer)
{
	uint16 *p1 = NULL;
	uint16 *p2 = NULL;
	//int16 x1,y1,x2,y2;
	uint16 i;
	uint16 rowBytes;
	uint16 rowNum;

	if(!pbmp || !pLcdShadowBuffer)
	{
		diag_printf( "EmbedRgbDataIntoLcdBuffer ERR!\n");
		return;
	}
#if 0	
	if(LcdFrameBuffer_x >= pIsomGlob->v_lcd_size_x || LcdFrameBuffer_y >= pIsomGlob->v_lcd_size_y ||
		LcdFrameBuffer_x + (int16)pIsomGlob->v_lcd_size_x <=0 || LcdFrameBuffer_y + (int16)pIsomGlob->v_lcd_size_y <=0 ||
		LcdFrameBuffer_x +(int16)bmp_width <=0 || LcdFrameBuffer_y+ (int16)bmp_height <=0)
	{
		diag_printf( "EmbedRgbDataIntoLcdBuffer parameter ERR!\n");
		return;
	}

	if(LcdFrameBuffer_x>=0)
	{
		x1=0;
		x2=LcdFrameBuffer_x;
	}
	else
	{
		x1= -LcdFrameBuffer_x;
		x2=0;
	}

	if(LcdFrameBuffer_y>=0)
	{
		y1=0;
		y2=LcdFrameBuffer_y;
	}
	else
	{
		y1= -LcdFrameBuffer_y;
		y2=0;
	}

	p1 = pbmp + y1*bmp_width + x1;
	p2 = pLcdShadowBuffer + y2 * pIsomGlob->v_lcd_size_x + x2;

	if(LcdFrameBuffer_x + bmp_width <= pIsomGlob->v_lcd_size_x)
		rowBytes=  (bmp_width -x1)<<1;
	else
		rowBytes= (pIsomGlob->v_lcd_size_x -x2)<<1;

	if(LcdFrameBuffer_y + bmp_height <= pIsomGlob->v_lcd_size_y)
		rowNum= bmp_height - y1;
	else
		rowNum= pIsomGlob->v_lcd_size_y - y2;
	if(LcdFrameBuffer_x <= 0 && rowBytes==pIsomGlob->v_lcd_size_x && rowBytes==bmp_width)
	{
		memcpy(p2, p1, rowBytes*rowNum);
	}
	else
	{
		for (i= 0; i < rowNum; i++)
		{	
			memcpy(p2, p1, rowBytes);
			p1+=bmp_width;
			p2+=pIsomGlob->v_lcd_size_x;
		}
	}
#else
	p1 = pbmp + cut_y*bmp_width + cut_x;
	p2 = pLcdShadowBuffer + LcdFrameBuffer_y * pIsomGlob->v_lcd_size_x + LcdFrameBuffer_x;
	rowBytes= cut_w*2;
	rowNum=cut_h;
	for (i= 0; i < rowNum; i++)
	{	
		memcpy(p2, p1, rowBytes);
		p1+=bmp_width;
		p2+=pIsomGlob->v_lcd_size_x;
	}
#endif
	
}
#endif

#define WRITE_AAC_HEAD(outbuf, sampLen)\
{\
	aacHead[3]&=0xfc;\
	aacHead[4]=0;\
	aacHead[5]&=0x1f;\
	aacHead[5]|=(((sampLen)&0x07)<<5);\
	aacHead[4]=(((sampLen)&0x07f8)>>3);\
	aacHead[3]|=(((sampLen)&0x1800)>>11);\
	memcpy((outbuf), aacHead, AAC_HEAD_SIZE);\
}

uint32 tick1, tick2, totalTa,totalTv, totalTx, counta, countv, countx;//test for time

/*
get remain data lenght in pipe buffer.
assume that data cnsumeLen will not bigger than mirror.
*/
static int getDataLen()
{
	int dataInBuf;
	
	//a_SampStart right, a_SampEnd left
	if(pIsomGlob->a_SampStart>pIsomGlob->a_SampEnd)
	{
		dataInBuf=pIsomGlob->a_SampBufPipeLen+(pIsomGlob->a_SampBufMirrorLen<<1)-(pIsomGlob->a_SampStart-pIsomGlob->a_SampEnd);
		
		if(pIsomGlob->a_SampEnd>=pIsomGlob->a_SampBuf+pIsomGlob->a_SampBufMirrorLen)
		{
			dataInBuf-=pIsomGlob->a_SampBufMirrorLen;
		}
		else
		{
			dataInBuf-=(pIsomGlob->a_SampEnd-pIsomGlob->a_SampBuf);
		}
	}
	else//a_SampStart left, a_SampEnd right
	{
		dataInBuf=pIsomGlob->a_SampEnd-pIsomGlob->a_SampStart;
	}

	return dataInBuf;	
}

/*
get sample start position.
assume that data cnsumeLen will not bigger than mirror.
*/
static char* getDataPos()
{
	int8* startPos;
	uint8* ritMroStart;

	// right mirror start position
	ritMroStart=pIsomGlob->a_SampBuf+pIsomGlob->a_SampBufPipeLen+pIsomGlob->a_SampBufMirrorLen;

	//sample start not in right mirror
	if(pIsomGlob->a_SampStart<ritMroStart)
	{
		startPos=pIsomGlob->a_SampStart;
	}
	else//sample start into the right mirror
	{
		uint8* lftMroPos;
		if(pIsomGlob->a_SampEnd<pIsomGlob->a_SampStart)
		{
			lftMroPos=pIsomGlob->a_SampBuf+(pIsomGlob->a_SampStart-ritMroStart);
			if(pIsomGlob->a_SampEnd>=lftMroPos)
			{
				startPos=lftMroPos;
			}
			else
				startPos=pIsomGlob->a_SampStart;
		}
		else
		{
			startPos=pIsomGlob->a_SampStart;
		}
	}

	return startPos;
}

/*
move sample start position.
assume that data cnsumeLen will not bigger than mirror.
*/
static void moveDataPos(int consumeLen)
{
	uint8* ritMroStart;
	uint8* moveStartPos;

	// right mirror start position
	ritMroStart=pIsomGlob->a_SampBuf+pIsomGlob->a_SampBufPipeLen+pIsomGlob->a_SampBufMirrorLen;
	//sample start will move position
	moveStartPos=pIsomGlob->a_SampStart+consumeLen;
	
	if(moveStartPos<ritMroStart)
	{
		if(pIsomGlob->a_SampEnd<pIsomGlob->a_SampStart)
		{
			pIsomGlob->a_SampStart=moveStartPos;
		}
		else if(pIsomGlob->a_SampEnd>=moveStartPos)
		{
			pIsomGlob->a_SampStart=moveStartPos;
		}
		else
		{
			pIsomGlob->a_SampStart=pIsomGlob->a_SampEnd;
		}
	}
	else
	{
		uint8* lftMroPos;
		if(pIsomGlob->a_SampEnd<pIsomGlob->a_SampStart)
		{
			lftMroPos=pIsomGlob->a_SampBuf+(moveStartPos-ritMroStart);
			if(pIsomGlob->a_SampEnd>=lftMroPos)
			{
				pIsomGlob->a_SampStart=lftMroPos;
			}
			else
				pIsomGlob->a_SampStart=pIsomGlob->a_SampEnd;
		}
		else if(pIsomGlob->a_SampEnd>=moveStartPos)
		{
			pIsomGlob->a_SampStart=moveStartPos;
		}
		else
		{
			pIsomGlob->a_SampStart=pIsomGlob->a_SampEnd;
		}
	}
}

/*
copy data from buffer.
char* output, 
unsigned int blkSize, //block size
unsigned int blkNum// block number
*/
static  int getData(unsigned char* output, unsigned int blkSize, unsigned int blkNum)
{
	int res;
	int getBlkNum;
	unsigned char *pTail;
	unsigned char *pOut;

	pTail=pIsomGlob->a_SampBuf+pIsomGlob->a_SampBufPipeLen+(pIsomGlob->a_SampBufMirrorLen<<1);
	pOut=output;
	getBlkNum=0;
	while(getDataLen()>=blkSize)
	{
		res=(int)(pTail-pIsomGlob->a_SampStart);
		if(res>=blkSize)
		{
			memcpy(pOut, pIsomGlob->a_SampStart, blkSize);
			moveDataPos(blkSize);
		}
		else
		{
			memcpy(pOut, pIsomGlob->a_SampStart, res);
			moveDataPos(res);
			
			memcpy(pOut+res, pIsomGlob->a_SampStart, blkSize-res);
			moveDataPos(blkSize-res);
		}

		pOut+=blkSize;
		getBlkNum++;
		if(getBlkNum==blkNum)break;
	}

	return getBlkNum;
}

/*
read data from bitstream and fill the out buffer full.
there have a temp buffer for bitstream,the size=mirror size
assume that the sample data will not bigger than mirror.
*/
static int readSamp(char *pBuf, int size)
{
	int res;
	int readSize;
	int8* pOut;
	uint8 useTmp;
	int8* pSamp;
	uint32 sampSize,gotbytes, gotcount;
	
	readSize=0;
	pOut=pBuf;
	
	if(pIsomGlob->a_SampTmpRemain>0)
	{
		if(pIsomGlob->a_SampTmpRemain>=size)
		{
			memcpy(pOut, pIsomGlob->a_SampTmpBufp, size);
			readSize=size;
		}
		else
		{
			memcpy(pOut, pIsomGlob->a_SampTmpBufp, pIsomGlob->a_SampTmpRemain);
			readSize=pIsomGlob->a_SampTmpRemain;
		}

		pOut+=readSize;
		pIsomGlob->a_SampTmpRemain-=readSize;
		pIsomGlob->a_SampTmpBufp+=readSize;
	}

	useTmp=0;
	while(readSize<size)
	{
		if(pIsomGlob->a_current_sample_num<pIsomGlob->a_total_sample_num)
		{
			if(pIsomGlob->med_type==MCI_TYPE_MP4 || pIsomGlob->med_type==MCI_TYPE_3GP)
			{
				if(pIsomGlob->a_type== A_TYPE_AAC)
				{
					if((res = stbl_GetSampleSize(pIsomGlob->a_info , pIsomGlob->a_current_sample_num+1, &sampSize))!=GF_OK)
					{
						diag_printf("[VID] AUD GetSampleSize ERR! res=%d sample_num=%d \n",res, pIsomGlob->a_current_sample_num);
						break;
					}

					if(readSize+AAC_HEAD_SIZE+sampSize<=size)
					{
						useTmp=0;
						pSamp=pOut;
					}
					else if(AAC_HEAD_SIZE+sampSize<=pIsomGlob->a_SampBufMirrorLen)
					{
						useTmp=1;
						pSamp=pIsomGlob->a_SampTmpBufp=pIsomGlob->a_SampTmpBuf;
					}
					else
					{
						diag_printf("[VID] AUD ERR! sample size=% bigger than a_SampBufMirrorLen=%d",sampSize, pIsomGlob->a_SampBufMirrorLen);
						break;
					}
					
					WRITE_AAC_HEAD(pSamp, AAC_HEAD_SIZE+sampSize);
					pSamp+=AAC_HEAD_SIZE;
					
					res=gf_isom_get_multSample((GF_MediaInformationBox *)pIsomGlob->a_info, 
												pIsomGlob->a_current_sample_num+1, 
												pSamp, 
												sampSize, 
												&gotbytes, &gotcount);
					
					if(res || (gotbytes<=0))
					{
						diag_printf("[VID] AUD get_multSample. res=%d gotbytes=%d sampSize=%d sample_num=%d",res, gotbytes, sampSize, pIsomGlob->a_current_sample_num);
						break;
					}

					if(useTmp==0)
					{
						readSize+=(AAC_HEAD_SIZE+gotbytes);
						pOut+=(AAC_HEAD_SIZE+gotbytes);
					}
					else
					{
						pIsomGlob->a_SampTmpRemain=(AAC_HEAD_SIZE+gotbytes);
						res=size-readSize;
						if(res>pIsomGlob->a_SampTmpRemain)
							res=pIsomGlob->a_SampTmpRemain;
						memcpy(pOut, pIsomGlob->a_SampTmpBufp, res);
						pIsomGlob->a_SampTmpRemain-=res;
						pIsomGlob->a_SampTmpBufp+=res;
						pOut+=res;
						readSize+=res;
					}
					pIsomGlob->a_current_sample_num+=gotcount;
					
				}
				else if(pIsomGlob->a_type== A_TYPE_AMR || pIsomGlob->a_type== A_TYPE_MP3)
				{
					if((res = stbl_GetSampleSize(pIsomGlob->a_info , pIsomGlob->a_current_sample_num+1, &sampSize))!=GF_OK)
					{
						diag_printf("[VID] AUD GetSampleSize ERR! res=%d sample_num=%d \n",res, pIsomGlob->a_current_sample_num);
						break;
					}

					if(readSize+sampSize<=size)
					{
						useTmp=0;
						pSamp=pOut;
						sampSize=size-readSize;
					}
					else if(sampSize<=pIsomGlob->a_SampBufMirrorLen)
					{
						useTmp=1;
						pSamp=pIsomGlob->a_SampTmpBufp=pIsomGlob->a_SampTmpBuf;
						sampSize=pIsomGlob->a_SampBufMirrorLen;
					}
					else
					{
						diag_printf("[VID] AUD ERR! sample size=% bigger than a_SampBufMirrorLen=%d",sampSize, pIsomGlob->a_SampBufMirrorLen);
						break;
					}
						
					res=gf_isom_get_multSample((GF_MediaInformationBox *)pIsomGlob->a_info, 
												pIsomGlob->a_current_sample_num+1, 
												pSamp, 
												sampSize, 
												&gotbytes, &gotcount);

					if(res || (gotbytes<=0))
					{
						diag_printf("[VID]AUD get_multSample res=%d gotbytes=%d sampSize=%d sample_num=%d",res, gotbytes, sampSize, pIsomGlob->a_current_sample_num);
						break;
					}

					if(useTmp==0)
					{
						readSize+=gotbytes;
						pOut+=gotbytes;
					}
					else
					{
						pIsomGlob->a_SampTmpRemain=gotbytes;
						res=size-readSize;
						if(res>pIsomGlob->a_SampTmpRemain)
							res=pIsomGlob->a_SampTmpRemain;
						memcpy(pOut, pIsomGlob->a_SampTmpBufp, res);
						pIsomGlob->a_SampTmpRemain-=res;
						pIsomGlob->a_SampTmpBufp+=res;
						pOut+=res;
						readSize+=res;
					}
					pIsomGlob->a_current_sample_num+=gotcount;

				}
			}
#ifdef RM_SUPPORT			
			else if(pIsomGlob->med_type==MCI_TYPE_RM)
			{
				//int32 tt1,tt2;
//tt1=hal_TimGetUpTime();	
			        res= rm_getAudioData(pOut, size-readSize);
				 if(res<=0)
				 {
				 	diag_printf("[VID] rm_getAudioData err res=%d\n", res);
				 	res=0;
				 }
				 readSize+=res;
//tt2=hal_TimGetUpTime();	
//diag_printf("ra read=%d",(tt2-tt1)*1000/16384);
				 
				break;
			}
#endif
			else
			{
				if((res=avi_read_audio(pIsomGlob->avi_p, pOut, size-readSize))<=0)
				{
					diag_printf(  "[VID]avi_read_audio res =%d audio_posc=%d", res, pIsomGlob->avi_p->audio_posc);  
					res=0;
				}

				readSize+=res;
				pIsomGlob->a_current_sample_num=pIsomGlob->avi_p->audio_posc+1;
				break;
			}
		}
		else
		{
			break;
		}
	}
	
	return readSize;
}

static void fillSampBuf()
{
	uint8* lftMroEnd;
	uint8* ritMroStart;
	int32 fillSize;
	int32 readSize=0;

	// left mirror end position
	lftMroEnd=pIsomGlob->a_SampBuf+pIsomGlob->a_SampBufMirrorLen;
	ritMroStart=lftMroEnd+pIsomGlob->a_SampBufPipeLen;

	while(1)
	{
		if(pIsomGlob->a_SampEnd+PIPE_SPACE<pIsomGlob->a_SampStart)
		{
			fillSize=pIsomGlob->a_SampStart-pIsomGlob->a_SampEnd-PIPE_SPACE;
			if(pIsomGlob->a_SampEnd>=lftMroEnd)
			{
				readSize=readSamp(pIsomGlob->a_SampEnd, fillSize);
				pIsomGlob->a_SampEnd+=readSize;
				
			}
			else
			{
				if(fillSize<=lftMroEnd-pIsomGlob->a_SampEnd)
				{
					readSize=fillSize;
					memcpy(pIsomGlob->a_SampEnd, ritMroStart+(pIsomGlob->a_SampEnd-pIsomGlob->a_SampBuf), readSize);
					pIsomGlob->a_SampEnd+=readSize;
				}
				else
				{
					readSize=lftMroEnd-pIsomGlob->a_SampEnd;
					memcpy(pIsomGlob->a_SampEnd, ritMroStart+(pIsomGlob->a_SampEnd-pIsomGlob->a_SampBuf), readSize);
					pIsomGlob->a_SampEnd+=readSize;

					readSize=readSamp(pIsomGlob->a_SampEnd, fillSize-readSize);
					pIsomGlob->a_SampEnd+=readSize;
				}
			}

			break;
		}
		else if(pIsomGlob->a_SampEnd>=pIsomGlob->a_SampStart)
		{
			if(pIsomGlob->a_SampEnd>=ritMroStart+pIsomGlob->a_SampBufMirrorLen)
			{
				if(pIsomGlob->a_SampStart>=pIsomGlob->a_SampBuf+PIPE_SPACE)
				{
					pIsomGlob->a_SampEnd=pIsomGlob->a_SampBuf;
					continue;
				}
				else
					break;
			}
			
			fillSize=ritMroStart+pIsomGlob->a_SampBufMirrorLen-pIsomGlob->a_SampEnd;
			readSize=readSamp(pIsomGlob->a_SampEnd, fillSize);
			pIsomGlob->a_SampEnd+=readSize;

			if(readSize<fillSize)
				break;
		}
		else
			break;
	}
}

static void get_amr_mode(uint8 *pbuf, uint32 *pmode, uint32 *pframesize )
{
	//read amr ring header mode.
	//mode 8,15 and other will replace with 475 silence data and drop original data
      switch((pbuf[0] >> 3) & 0xF)
        {
            case 0:
                *pframesize = 13;
                *pmode = HAL_AMR475_DEC;
		//diag_printf("[VID] AMR mode 0, 4.75");
                break;
            case 1:
               *pframesize = 14;
                *pmode = HAL_AMR515_DEC;
		//diag_printf("[VID] AMR mode 1, 5.15");					
                break;
            case 2:
               *pframesize = 16;
                *pmode = HAL_AMR59_DEC;
		//diag_printf("[VID] AMR mode 2, 5.9");
                break;
            case 3:
               *pframesize = 18;
                *pmode = HAL_AMR67_DEC;
		//diag_printf("[VID] AMR mode 3, 6.7");
                break;
            case 4:
               *pframesize = 20;
                *pmode = HAL_AMR74_DEC;
		//diag_printf("[VID] AMR mode 4, 7.4");
                break;
            case 5:
               *pframesize = 21;
                *pmode = HAL_AMR795_DEC;
		//diag_printf("[VID] AMR mode 5, 7.95");
                break;
            case 6:
               *pframesize = 27;
                *pmode = HAL_AMR102_DEC;
		//diag_printf("[VID] AMR mode 6, 10.2");
                break;
            case 7:
               *pframesize = 32;
                *pmode = HAL_AMR122_DEC;
		//diag_printf("[VID] AMR mode 7, 12.2");
                break;
	     case 8:
               *pframesize = 6;
                *pmode = HAL_INVALID;
		diag_printf("[VID] AMR mode 8, SID");
                break;
            case 15:
               *pframesize = 1;
                *pmode = HAL_INVALID;
		diag_printf("[VID] AMR mode 15, NO DATA");
                break;

            default:
	        //pIsomGlob->open_audio=0;
               *pframesize = 1;//move position 1byte
                *pmode = HAL_INVALID;
                diag_printf("[VID] WARNING! AMR head=%x , unsupport now!!! Audio Turn off!",pbuf[0]);
                break;
        }

}

/*
audio linear fade in
process couple of sample each time(assume 2 channel)
int16 *pPcm	//buffer in
uint32 smpNum	// total sample count
*/

static void audFadeIn(int16 *pPcm, uint32 smpNum)
{
	int32 i;
	int32 pcm1,pcm2;
	int32 scale,scale2;
//int32 tt1,tt2;
//tt1=hal_TimGetUpTime();	

	//32767=0x7FFF
	if(pPcm && smpNum>2)
	{
		smpNum>>=1;
		smpNum<<=1;
		scale=(32767+(smpNum>>1))/smpNum;
		if(scale==0)scale=1;
		for(i=0;i<smpNum;i+=2)
		{
			pcm1=(int32)*pPcm;
			pcm2=(int32)*(pPcm+1);
			scale2=i*scale;
			if(scale2>32767)break;
			*pPcm++=(int16)((pcm1*scale2)>>15);// 32767
			*pPcm++=(int16)((pcm2*scale2)>>15);// 32767
			//if(i>smpNum-64)
			//diag_printf("%d %d %d %d",pcm1,pcm2,*(pPcm-2),*(pPcm-1));
		}
	}
//tt2=hal_TimGetUpTime();	
//diag_printf("fade in time=%d",(tt2-tt1));//*1000/16384
}

/*
audio linear fade out
process couple of sample each time(assume 2 channel)
int16 *pPcm	//buffer in
uint32 smpNum	// total sample count
*/

static void audFadeOut(int16 *pPcm, uint32 smpNum)
{
	int32 i;
	int32 pcm1,pcm2;
	int32 scale,scale2;
//int32 tt1,tt2;
//tt1=hal_TimGetUpTime();	

	//32767=0x7FFF
	if(pPcm && smpNum>2)
	{
		smpNum>>=1;
		smpNum<<=1;
		scale=(32767+(smpNum>>1))/smpNum;
		if(scale==0)scale=1;
		scale2=32767;
		for(i=0;i<smpNum;i+=2)
		{
			scale2=scale2-scale;
			
			if(scale2>0)
			{
				pcm1=(int32)*pPcm;
				pcm2=(int32)*(pPcm+1);
				*pPcm++=(int16)((pcm1*scale2)>>15);// 32767
				*pPcm++=(int16)((pcm2*scale2)>>15);// 32767
			}
			else
			{
				*pPcm++=0;
				*pPcm++=0;
			}
			
			//if(i>smpNum-64)
			//diag_printf("%d %d %d %d",pcm1,pcm2,*(pPcm-2),*(pPcm-1));
		}
	}
//tt2=hal_TimGetUpTime();	
//diag_printf("fade out time=%d",(tt2-tt1));//*1000/16384
}

static void fillAudioDataFun(void)
{
	fillSampBuf();

	if(pIsomGlob->a_type== A_TYPE_PCM && AUD_DEC_FRM_NUM>0)
	{
		int res;
		while(getDataLen()>0)
		{
			if(AUD_FADE_OUT_FLAG==0)
				res=getData((unsigned char*)pPcmBufPos, pIsomGlob->a_frameBytes, 1);
			else
			{
				moveDataPos(pIsomGlob->a_frameBytes);
				res=1;
			}
			
			if(res<=0)break;
			
			AUD_DEC_FRM_NUM-=(uint16)res;

			if((res>0)&&(pIsomGlob->a_channels==2)&&(audioItf==AUD_ITF_LOUD_SPEAKER))
			{
				uint32 i;
				INT16 *pPcm=(INT16 *)pPcmBufPos;
				uint32 sNum=pIsomGlob->a_frame_sample*2;
				
				for(i=0;i<sNum;i+=2)
				{
					pPcm[i]=pPcm[i+1]=(pPcm[i]>>1)+(pPcm[i+1]>>1);					
				}
			}

			pPcmBufPos+=pIsomGlob->a_frameBytes;
			fillSampBuf();
			
			if(AUD_DEC_FRM_NUM==0)break;
		}

		if(AUD_DEC_FRM_NUM>0)
		{
			memset((void*)pPcmBufPos, 0, pIsomGlob->a_frameBytes*AUD_DEC_FRM_NUM);
			AUD_DEC_FRM_NUM=0;
			diag_printf("fillAudioDataFun pcm over");
		}
	}
}

#if 0
static void vid_timer_isr()
{
	TIMER_ISR=1;
	
	//StopMyTimer(MMC_VID_PLY_TIMER_ID);
	COS_KillTimerEX(GetMCITaskHandle(MBOX_ID_MMC), MSG_VID_TIMER_REQ);
	
	if((pIsomGlob->v_current_frame_num+1)>=pIsomGlob->v_total_frame_num 
		|| play_state!=PLAY_STATE_PLAY)
	{
		diag_printf("Timer stop!");
		return;
	}

	//StartMyTimer(MMC_VID_PLY_TIMER_ID, pIsomGlob->v_period, (oslTimerFuncPtr)vid_timer_isr);
	COS_SetTimerEX(GetMCITaskHandle(MBOX_ID_MMC), 0, COS_TIMER_MODE_SINGLE, pIsomGlob->v_period*16);

	//VID_DEC_REQ++;
	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VID_IMG_DEC_REQ);
}
#endif

#ifdef RM_SUPPORT
//for ra c decode
int32 ra_nframe_dec(void)
{
	int32 res=0;
	if(aud_extra_pcmbuf_state[A_EXBF_WT_ID].done==0)
	{
	//int32 tt1,tt2;
		res=-1;
		AUD_DEC_FRM_NUM=0;
		
//tt1=hal_TimGetUpTime();	

		while(AUD_DEC_FRM_NUM < pIsomGlob->a_HalfPcmBufFrame)
		{
			fillSampBuf();
			
			if(AUD_FADE_IN_FLAG==0)
			{
				if(getDataLen()>0)
					res=Gecko2Decode(pIsomGlob->a_Gecko2DecHdl, getDataPos(), 0, (short*)pPcmBufPos);
				if(res!=0)
				{
					memset((void*)pPcmBufPos,0,pIsomGlob->a_frameBytes);
					diag_printf("[VID]AUD Gecko2Decode err! res=%d\n",res);
					break;
				}
			}
			else
			{
				//mute frame
				if(AUD_FADE_IN_FLAG>1)
					memset((uint8*)pPcmBufPos, 0, pIsomGlob->a_frameBytes);
				else
				{
					audFadeIn((int16*)pPcmBufPos, pIsomGlob->a_frame_sample*pIsomGlob->a_channels);
				}
				AUD_FADE_IN_FLAG--;
			}
		
			moveDataPos(pIsomGlob->a_codeframeBytes);//const code frame bytes.
			pPcmBufPos+=pIsomGlob->a_frameBytes;
			
			AUD_DEC_FRM_NUM++;
		}

		aud_extra_pcmbuf_state[A_EXBF_WT_ID].done=1;
		A_EXBF_WT_ID+=1;
		if(A_EXBF_WT_ID>=EXTRA_PCM_BUF_NUM)A_EXBF_WT_ID=0;
			
		AUD_DEC_FRM_NUM=0;

		if(needRA)
		{
			if(AUD_FADE_OUT_FLAG==0)
				memcpy(PCM_ISR_FLAG ? (pIsomGlob->a_PcmBufHalfPos):(pIsomGlob->a_PcmBuf),
					(void*)(aud_extra_pcmbuf_state[A_EXBF_RD_ID].pBufPos), pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame);
			aud_extra_pcmbuf_state[A_EXBF_RD_ID].done=0;
			A_EXBF_RD_ID+=1;
			if(A_EXBF_RD_ID>=EXTRA_PCM_BUF_NUM)A_EXBF_RD_ID=0;
			pPcmBufPos=(uint32)(aud_extra_pcmbuf_state[A_EXBF_WT_ID].pBufPos);
			needRA=0;
			//diag_printf("ra fill pcm");
		}
//tt2=hal_TimGetUpTime();	
//diag_printf("ra time=%d",(tt2-tt1)*1000/16384);
	}
	
	return	res;
}
#endif

static void vid_pcmHalf_isr () 
{
	long long syncF;
	uint8 pcmUpdate=0;

	PCM_ISR_FLAG=0;
	
	//av sync
	if(pIsomGlob->med_type!=MCI_TYPE_RM)
		pIsomGlob->a_current_frame_num+=pIsomGlob->a_HalfPcmBufFrame;
	else//reuse to record pcm sample
		pIsomGlob->a_current_frame_num+=pIsomGlob->a_frame_sample*pIsomGlob->a_HalfPcmBufFrame;

	if(pIsomGlob->a_current_frame_num-LAST_SYNC_AUD_FRM_NUM>= pIsomGlob->a_sync_distance_frame_num)
	{
		LAST_SYNC_AUD_FRM_NUM=pIsomGlob->a_current_frame_num;

		if(pIsomGlob->med_type!=MCI_TYPE_RM)
		{
			syncF=((long long)pIsomGlob->a_current_frame_num*(long long)SYNC_NUM_COEFFICIENT)>>(SYNC_COEFF_SCALE_BIT+V_FPS_SCALE_BIT);
			VID_DEC_REQ=(int16)((int32)syncF-pIsomGlob->v_current_frame_num);
			pIsomGlob->current_time=(uint32)syncF*pIsomGlob->v_period;
		}
#ifdef RM_SUPPORT
		else
		{
			syncF=((long long)pIsomGlob->a_current_frame_num*1000/(long long)pIsomGlob->a_sample_rate);
			VID_DEC_REQ=(int16)((syncF-(long long)rm_getLastFrameTimeStamp()+(long long)(pIsomGlob->v_period>>1))/(long long)pIsomGlob->v_period);
			pIsomGlob->current_time=(unsigned int)syncF;
		}
#endif
		
		//DISPLAY_INTERFACE=1;
		if(DISPLAY_SEND_EVENT_NUM==0)
		{
			DISPLAY_SEND_EVENT_NUM=1;
			mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VID_DISPLAY_INTERFACE_REQ);
		}
	}

	if(AUD_FADE_OUT_FLAG==3)
	{
		//fade out  last frame
		audFadeOut((int16*) (pIsomGlob->a_PcmBufHalfPos+ pIsomGlob->a_frameBytes*(pIsomGlob->a_HalfPcmBufFrame-1)), 
			pIsomGlob->a_frame_sample*pIsomGlob->a_channels);
	}

	//if(getDataLen()>0)
	{
		if(pIsomGlob->a_type== A_TYPE_COOK)
		{
#ifdef RM_SUPPORT		
			if(aud_extra_pcmbuf_state[A_EXBF_RD_ID].done==1)
			{
				if(AUD_FADE_OUT_FLAG==0)
					memcpy(pIsomGlob->a_PcmBuf,(VOID*)((uint32)(aud_extra_pcmbuf_state[A_EXBF_RD_ID].pBufPos)|0x20000000), pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame);
				aud_extra_pcmbuf_state[A_EXBF_RD_ID].done=0;
				A_EXBF_RD_ID+=1;
				if(A_EXBF_RD_ID>=EXTRA_PCM_BUF_NUM)A_EXBF_RD_ID=0;
				pPcmBufPos=(uint32)(aud_extra_pcmbuf_state[A_EXBF_WT_ID].pBufPos);
				pcmUpdate=1;
			}
			else
			{
				needRA=1;
				pcmUpdate=0;
			}
#endif
		}
		else if(pIsomGlob->a_type!= A_TYPE_PCM)
		{
			if(aud_extra_pcmbuf_state[A_EXBF_RD_ID].done==1)
			{
				if(AUD_FADE_OUT_FLAG==0)
					memcpy(pIsomGlob->a_PcmBuf, (VOID*)((uint32)(aud_extra_pcmbuf_state[A_EXBF_RD_ID].pBufPos)|0x20000000), pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame);
#if 1
				aud_extra_pcmbuf_state[A_EXBF_RD_ID].done=0;
				A_EXBF_RD_ID+=1;
				if(A_EXBF_RD_ID>=EXTRA_PCM_BUF_NUM)A_EXBF_RD_ID=0;

				if(VOC_WORK==VOC_WORK_NON && VOC_SETTING==0 && getDataLen()>0)
				{
				//diag_printf("[VID]pcm call 1");
					VOC_WORK=VOC_WORK_AUD;
					pPcmBufPos=(uint32)(aud_extra_pcmbuf_state[A_EXBF_WT_ID].pBufPos);
#if( CHIP_HAS_GOUDA == 1 )
					if(pIsomGlob->a_type != A_TYPE_AMR)
					{
#endif
						vocDecAud.inStreamBufAddr=(INT32 *)getDataPos();
						vocDecAud.outStreamBufAddr=(INT32 *)pPcmBufPos;
						vocDecAud.audioItf=audioItf;

						if(vpp_AudioJpegDecScheduleOneFrame(&vocDecAud)!=HAL_ERR_NO)//aac 最后有1024字节缓冲
						{
							VOC_WORK=VOC_WORK_NON;
							diag_printf("[VID]voc call err!");
						}
						//diag_printf("pcm voc stat a");
#if( CHIP_HAS_GOUDA == 1 )						
					}
					else
					{
						HAL_SPEECH_DEC_IN_T *pVocDecAmr=vpp_SpeechGetRxCodBuffer();
						
						get_amr_mode( getDataPos(), &(pIsomGlob->a_amr_mode), &(pIsomGlob->a_codeframeBytes));
						
						if(pIsomGlob->a_amr_mode != HAL_INVALID)
						{
							getData((U8*)pVocDecAmr->decInBuf, pIsomGlob->a_codeframeBytes, 1);
							pVocDecAmr->codecMode= pIsomGlob->a_amr_mode;
						}
						else
						{
							//write silence data
							moveDataPos(pIsomGlob->a_codeframeBytes);
							memcpy(pVocDecAmr->decInBuf, silence_frame475, 13);
							pVocDecAmr->codecMode= HAL_AMR475_DEC;
						}
						
						if(vpp_SpeechScheduleOneFrame(VPP_SPEECH_WAKEUP_SW_DEC) != HAL_ERR_NO)
						{
							VOC_WORK=VOC_WORK_NON;
							diag_printf("[VID]voc call err!");
						}
					}
#endif

				}
#endif
				pcmUpdate=1;
			}
			else
			{
			//diag_printf("[VID]pcm call 2");
				if(VOC_WORK==VOC_WORK_NON && VOC_SETTING==0 && getDataLen()>0)
				{
					VOC_WORK=VOC_WORK_AUD;
					VOC_AUD++;
					//pPcmBufPos=(uint32)pIsomGlob->a_PcmBuf;
					pPcmBufPos=(uint32)(aud_extra_pcmbuf_state[A_EXBF_WT_ID].pBufPos);

#if( CHIP_HAS_GOUDA == 1 )
					if(pIsomGlob->a_type != A_TYPE_AMR)
					{
#endif
						vocDecAud.inStreamBufAddr=(INT32 *)getDataPos();
						vocDecAud.outStreamBufAddr=(INT32 *)pPcmBufPos;
						vocDecAud.audioItf=audioItf;

						if(vpp_AudioJpegDecScheduleOneFrame(&vocDecAud)!=HAL_ERR_NO)//aac 最后有1024字节缓冲
						{
							VOC_WORK=VOC_WORK_NON;
							diag_printf("[VID]voc call err!");
						}
						//diag_printf("pcm voc stat a");
#if( CHIP_HAS_GOUDA == 1 )
					}
					else
					{
						HAL_SPEECH_DEC_IN_T *pVocDecAmr=vpp_SpeechGetRxCodBuffer();
						
						get_amr_mode( getDataPos(), &(pIsomGlob->a_amr_mode), &(pIsomGlob->a_codeframeBytes));
						
						if(pIsomGlob->a_amr_mode != HAL_INVALID)
						{
							getData((U8*)pVocDecAmr->decInBuf, pIsomGlob->a_codeframeBytes, 1);
							pVocDecAmr->codecMode= pIsomGlob->a_amr_mode;
						}
						else
						{
							//write silence data
							moveDataPos(pIsomGlob->a_codeframeBytes);
							memcpy(pVocDecAmr->decInBuf, silence_frame475, 13);
							pVocDecAmr->codecMode= HAL_AMR475_DEC;
						}
						
						if(vpp_SpeechScheduleOneFrame(VPP_SPEECH_WAKEUP_SW_DEC) != HAL_ERR_NO)
						{
							VOC_WORK=VOC_WORK_NON;
							diag_printf("[VID]voc call err!");
						}
					}
#endif
				}
				else if(getDataLen()>0)
				{
					if(VOC_AUD!=0)
						AUD_DEC_MIN=0;
					//else
					//	AUD_DEC_MIN=2;
						
					VOC_AUD++;
				}

				pcmUpdate=0;
			}
		}
		else
		{
		/*
			if(getDataLen()>0)
			{
				int res;
				pPcmBufPos=(uint32)pIsomGlob->a_PcmBuf;
				AUD_DEC_FRM_NUM=(uint16)pIsomGlob->a_HalfPcmBufFrame;
				if(AUD_FADE_OUT_FLAG==0)
					res=getData((unsigned char*)pPcmBufPos, pIsomGlob->a_frameBytes, (unsigned int)AUD_DEC_FRM_NUM);
				else
				{
					moveDataPos(pIsomGlob->a_frameBytes*AUD_DEC_FRM_NUM);
					res=AUD_DEC_FRM_NUM;
				}
				AUD_DEC_FRM_NUM-=(uint16)res;

				if((res>0)&&(pIsomGlob->a_channels==2)&&(audioItf==AUD_ITF_LOUD_SPEAKER))
				{
					uint32 i;
					INT16 *pPcm=(INT16 *)pPcmBufPos;
					uint32 sNum=pIsomGlob->a_frame_sample*2*res;
					
					for(i=0;i<sNum;i+=2)
					{
						pPcm[i]=pPcm[i+1]=(pPcm[i]>>1)+(pPcm[i+1]>>1);					
					}
				}
				pcmUpdate=1;
			}
			else
				pcmUpdate=0;
*/
			pPcmBufPos=(uint32)pIsomGlob->a_PcmBuf;
			AUD_DEC_FRM_NUM=(uint16)pIsomGlob->a_HalfPcmBufFrame;
			pcmUpdate=1;
			
		}

		if(AUD_FADE_OUT_FLAG>0)
		{
			memset((uint8*)pIsomGlob->a_PcmBuf, 0, pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame);
			if(AUD_FADE_OUT_FLAG>1)AUD_FADE_OUT_FLAG--;
			pcmUpdate=1;
		}
		
		 if(SEND_EVENT_NUM==0 )
		{
			SEND_EVENT_NUM=1;
			mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VID_AUD_DEC_REQ);
		}
	}
	
	if(pcmUpdate==0)
	{
		//aud_StreamStop(audioItf);
		if(AUD_DATA_REQ==1)AUD_DATA_REQ=2;
		memset(pIsomGlob->a_PcmBuf, 0, pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame);
		diag_printf("[VID]pcm isr, aud no data");
	}
	
}

static void vid_pcmEnd_isr () 
{
	long long syncF;
	uint8 pcmUpdate=0;

	PCM_ISR_FLAG=1;

	//av sync
	if(pIsomGlob->med_type!=MCI_TYPE_RM)
		pIsomGlob->a_current_frame_num+=pIsomGlob->a_HalfPcmBufFrame;
	else//reuse to record pcm sample
		pIsomGlob->a_current_frame_num+=pIsomGlob->a_frame_sample*pIsomGlob->a_HalfPcmBufFrame;
	
	if(pIsomGlob->a_current_frame_num-LAST_SYNC_AUD_FRM_NUM>= pIsomGlob->a_sync_distance_frame_num)
	{
		LAST_SYNC_AUD_FRM_NUM=pIsomGlob->a_current_frame_num;
		
		if(pIsomGlob->med_type!=MCI_TYPE_RM)
		{
			syncF=((long long)pIsomGlob->a_current_frame_num*(long long)SYNC_NUM_COEFFICIENT)>>(SYNC_COEFF_SCALE_BIT+V_FPS_SCALE_BIT);
			VID_DEC_REQ=(int16)((int32)syncF-pIsomGlob->v_current_frame_num);
			pIsomGlob->current_time=(uint32)syncF*pIsomGlob->v_period;
		}
#ifdef RM_SUPPORT		
		else
		{
			syncF=((long long)pIsomGlob->a_current_frame_num*1000/(long long)pIsomGlob->a_sample_rate);
			VID_DEC_REQ=(int16)((syncF-(long long)rm_getLastFrameTimeStamp()+(long long)(pIsomGlob->v_period>>1))/(long long)pIsomGlob->v_period);
			pIsomGlob->current_time=(unsigned int)syncF;
		}
#endif
		
		//DISPLAY_INTERFACE=1;
		if(DISPLAY_SEND_EVENT_NUM==0)
		{
			DISPLAY_SEND_EVENT_NUM=1;
			mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VID_DISPLAY_INTERFACE_REQ);
		}
	}
	
	if(AUD_FADE_OUT_FLAG==3)
	{
		//fade out  last frame
		audFadeOut((int16*) (pIsomGlob->a_PcmBuf+ pIsomGlob->a_frameBytes*(pIsomGlob->a_HalfPcmBufFrame-1)), 
			pIsomGlob->a_frame_sample*pIsomGlob->a_channels);
	}
	
	//if(getDataLen()>0)
	{
		if(pIsomGlob->a_type== A_TYPE_COOK)
		{
#ifdef RM_SUPPORT
			if(aud_extra_pcmbuf_state[A_EXBF_RD_ID].done==1)
			{
				if(AUD_FADE_OUT_FLAG==0)
					memcpy(pIsomGlob->a_PcmBufHalfPos,(VOID*)((uint32)(aud_extra_pcmbuf_state[A_EXBF_RD_ID].pBufPos)|0x20000000), pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame);
				aud_extra_pcmbuf_state[A_EXBF_RD_ID].done=0;
				A_EXBF_RD_ID+=1;
				if(A_EXBF_RD_ID>=EXTRA_PCM_BUF_NUM)A_EXBF_RD_ID=0;
				pPcmBufPos=(uint32)(aud_extra_pcmbuf_state[A_EXBF_WT_ID].pBufPos);
				pcmUpdate=1;
			}
			else
			{
				needRA=1;
				pcmUpdate=0;
			}
#endif
		}
		else if(pIsomGlob->a_type!= A_TYPE_PCM)
		{
			if(aud_extra_pcmbuf_state[A_EXBF_RD_ID].done==1)
			{
				if(AUD_FADE_OUT_FLAG==0)
					memcpy(pIsomGlob->a_PcmBufHalfPos, (VOID*)((uint32)(aud_extra_pcmbuf_state[A_EXBF_RD_ID].pBufPos)|0x20000000), pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame);
#if 1
				aud_extra_pcmbuf_state[A_EXBF_RD_ID].done=0;
				A_EXBF_RD_ID+=1;
				if(A_EXBF_RD_ID>=EXTRA_PCM_BUF_NUM)A_EXBF_RD_ID=0;

				if(VOC_WORK==VOC_WORK_NON && VOC_SETTING==0 && getDataLen()>0)
				{
					VOC_WORK=VOC_WORK_AUD;

					pPcmBufPos=(uint32)(aud_extra_pcmbuf_state[A_EXBF_WT_ID].pBufPos);

#if( CHIP_HAS_GOUDA == 1 )
					if(pIsomGlob->a_type != A_TYPE_AMR)
					{
#endif
						vocDecAud.inStreamBufAddr=(INT32 *)getDataPos();
						vocDecAud.outStreamBufAddr=(INT32 *)pPcmBufPos;
						vocDecAud.audioItf=audioItf;

						if(vpp_AudioJpegDecScheduleOneFrame(&vocDecAud)!=HAL_ERR_NO)//aac 最后有1024字节缓冲
						{
							VOC_WORK=VOC_WORK_NON;
							diag_printf("[VID]voc call err!");
						}
						//diag_printf("pcm voc stat a");
#if( CHIP_HAS_GOUDA == 1 )
					}
					else
					{
						HAL_SPEECH_DEC_IN_T *pVocDecAmr=vpp_SpeechGetRxCodBuffer();
						
						get_amr_mode( getDataPos(), &(pIsomGlob->a_amr_mode), &(pIsomGlob->a_codeframeBytes));
						
						if(pIsomGlob->a_amr_mode != HAL_INVALID)
						{
							getData((U8*)pVocDecAmr->decInBuf, pIsomGlob->a_codeframeBytes, 1);
							pVocDecAmr->codecMode= pIsomGlob->a_amr_mode;
						}
						else
						{
							//write silence data
							moveDataPos(pIsomGlob->a_codeframeBytes);
							memcpy(pVocDecAmr->decInBuf, silence_frame475, 13);
							pVocDecAmr->codecMode= HAL_AMR475_DEC;
						}
						
						if(vpp_SpeechScheduleOneFrame(VPP_SPEECH_WAKEUP_SW_DEC) != HAL_ERR_NO)
						{
							VOC_WORK=VOC_WORK_NON;
							diag_printf("[VID]voc call err!");
						}
					}
#endif
				}
#endif
				pcmUpdate=1;
			}
			else
			{
				if(VOC_WORK==VOC_WORK_NON && VOC_SETTING==0 && getDataLen()>0)
				{
					VOC_WORK=VOC_WORK_AUD;
					VOC_AUD++;

					//pPcmBufPos=(uint32)pIsomGlob->a_PcmBufHalfPos;
					pPcmBufPos=(uint32)(aud_extra_pcmbuf_state[A_EXBF_WT_ID].pBufPos);

#if( CHIP_HAS_GOUDA == 1 )
					if(pIsomGlob->a_type != A_TYPE_AMR)
					{
#endif
						vocDecAud.inStreamBufAddr=(INT32 *)getDataPos();
						vocDecAud.outStreamBufAddr=(INT32 *)pPcmBufPos;
						vocDecAud.audioItf=audioItf;

						if(vpp_AudioJpegDecScheduleOneFrame(&vocDecAud)!=HAL_ERR_NO)//aac 最后有1024字节缓冲
						{
							VOC_WORK=VOC_WORK_NON;
							diag_printf("[VID]voc call err!");
						}
						//diag_printf("pcm voc stat a");
#if( CHIP_HAS_GOUDA == 1 )
					}
					else
					{
						HAL_SPEECH_DEC_IN_T *pVocDecAmr=vpp_SpeechGetRxCodBuffer();
						
						get_amr_mode( getDataPos(), &(pIsomGlob->a_amr_mode), &(pIsomGlob->a_codeframeBytes));
						
						if(pIsomGlob->a_amr_mode != HAL_INVALID)
						{
							getData((U8*)pVocDecAmr->decInBuf, pIsomGlob->a_codeframeBytes, 1);
							pVocDecAmr->codecMode= pIsomGlob->a_amr_mode;
						}
						else
						{
							//write silence data
							moveDataPos(pIsomGlob->a_codeframeBytes);
							memcpy(pVocDecAmr->decInBuf, silence_frame475, 13);
							pVocDecAmr->codecMode= HAL_AMR475_DEC;
						}
						
						if(vpp_SpeechScheduleOneFrame(VPP_SPEECH_WAKEUP_SW_DEC) != HAL_ERR_NO)
						{
							VOC_WORK=VOC_WORK_NON;
							diag_printf("[VID]voc call err!");
						}
					}
#endif
				}
				else if(getDataLen()>0)
				{
					if(VOC_AUD!=0)
						AUD_DEC_MIN=0;
					//else
					//	AUD_DEC_MIN=2;
					
					VOC_AUD++;
				}

				pcmUpdate=0;
			}
		}
		else
		{
		/*
			if(getDataLen()>0)
			{
				int res;
				pPcmBufPos=(uint32)pIsomGlob->a_PcmBufHalfPos;
				AUD_DEC_FRM_NUM=(uint16)pIsomGlob->a_HalfPcmBufFrame;
				if(AUD_FADE_OUT_FLAG==0)
					res=getData((unsigned char*)pPcmBufPos, pIsomGlob->a_frameBytes, (unsigned int)AUD_DEC_FRM_NUM);
				else
				{
					moveDataPos(pIsomGlob->a_frameBytes*AUD_DEC_FRM_NUM);
					res=AUD_DEC_FRM_NUM;
				}
				AUD_DEC_FRM_NUM-=(uint16)res;
				
				if((res>0)&&(pIsomGlob->a_channels==2)&&(audioItf==AUD_ITF_LOUD_SPEAKER))
				{
					uint32 i;
					INT16 *pPcm=(INT16 *)pPcmBufPos;
					uint32 sNum=pIsomGlob->a_frame_sample*2*res;
					
					for(i=0;i<sNum;i+=2)
					{
						pPcm[i]=pPcm[i+1]=(pPcm[i]>>1)+(pPcm[i+1]>>1);					
					}
				}
				pcmUpdate=1;
			}
			else
				pcmUpdate=0;
			*/
			pPcmBufPos=(uint32)pIsomGlob->a_PcmBufHalfPos;
			AUD_DEC_FRM_NUM=(uint16)pIsomGlob->a_HalfPcmBufFrame;
			pcmUpdate=1;
			
		}
		
		if(AUD_FADE_OUT_FLAG>0)
		{
			memset((uint8*)pIsomGlob->a_PcmBufHalfPos, 0, pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame);
			if(AUD_FADE_OUT_FLAG>1)AUD_FADE_OUT_FLAG--;
			pcmUpdate=1;
		}
		
		 if(SEND_EVENT_NUM==0 )
		{
			SEND_EVENT_NUM=1;
			mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VID_AUD_DEC_REQ);
		}
	}
	
	if(pcmUpdate==0)
	{
		//aud_StreamStop(audioItf);
		if(AUD_DATA_REQ==1)AUD_DATA_REQ=2;
		memset(pIsomGlob->a_PcmBufHalfPos, 0, pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame);
		diag_printf("[VID]pcm isr, aud no data");
	}
}

#if (CHIP_HAS_GOUDA == 1)
static void vid_update(void )
{
    if(IMAGE_INBUF_FLAG==0)
    {
        IMAGE_W_INBUF0=pIsomGlob->v_display_width;
        IMAGE_H_INBUF0=pIsomGlob->v_display_height;
    }
    else if(IMAGE_INBUF_FLAG==1)
    {
        IMAGE_W_INBUF1=pIsomGlob->v_display_width;
        IMAGE_H_INBUF1=pIsomGlob->v_display_height;
    }
}
#endif

#if( CHIP_HAS_GOUDA != 1 )
static void vid_voc_isr(HAL_VOC_IRQ_STATUS_T * status )
{
	if(VOC_WORK==VOC_WORK_VID)
	{
		VOC_VID--;

		if(NEED_BLEND_IMG)
		{
			if(pIsomGlob->v_rotate==Rotate_with_0 )
			{
				EmbedRgbDataIntoLcdBuffer(0, 0,
											pIsomGlob->v_display_width, pIsomGlob->v_display_height,
											pIsomGlob->v_cutX, pIsomGlob->v_cutY,
											pIsomGlob->v_cutW, pIsomGlob->v_cutH,
											pIsomGlob->v_ImageBuf,
											pIsomGlob->v_ImageBuf2 );
			}
			else
			{
				EmbedRgbDataIntoLcdBuffer(0, 0,
											pIsomGlob->v_display_height, pIsomGlob->v_display_width, 
											(pIsomGlob->v_rotate==Rotate_with_90)?(pIsomGlob->v_display_height-(pIsomGlob->v_cutY+pIsomGlob->v_cutH)):(pIsomGlob->v_cutY), 
											(pIsomGlob->v_rotate==Rotate_with_90)?(pIsomGlob->v_cutX):(pIsomGlob->v_display_width-(pIsomGlob->v_cutX+pIsomGlob->v_cutW)),
											pIsomGlob->v_cutH, 
											pIsomGlob->v_cutW,
											pIsomGlob->v_ImageBuf,
											pIsomGlob->v_ImageBuf2 );
			}
		}

		if(IMAGE_INBUF_FLAG==0)
		{
			IMAGE_W_INBUF0=pIsomGlob->v_display_width;
			IMAGE_H_INBUF0=pIsomGlob->v_display_height;
		}
		else if(IMAGE_INBUF_FLAG==1)
		{
			IMAGE_W_INBUF1=pIsomGlob->v_display_width;
			IMAGE_H_INBUF1=pIsomGlob->v_display_height;
		}
		
		if(VOC_AUD>0)
		{
			VOC_WORK=VOC_WORK_AUD;
			goto start_aud;
		}
		else if(VOC_VID<=0)
		{
			if(aud_extra_pcmbuf_state[A_EXBF_WT_ID].done==1)
			{
				VOC_WORK=VOC_WORK_NON;
				VOC_VID=0;
			}
			else
			{
				VOC_VID=0;
				VOC_WORK=VOC_WORK_AUD;
				goto start_aud;
			}
			
			return;
		}

	}
	else if(VOC_WORK==VOC_WORK_AUD)
	{
		vpp_AudioJpegDecStatus(&vocDecStatus);
		
		if(vocDecStatus.ErrorStatus)
		{
			if((uint8*)pPcmBufPos +pIsomGlob->a_frameBytes<= aud_extra_pcmbuf_state[A_EXBF_WT_ID].pBufPos+pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame)
			{
				memset((uint8*)pPcmBufPos, 0, pIsomGlob->a_frameBytes);
				diag_printf("[VId]aud err. output_len=%d", vocDecStatus.output_len);
			}
			else
				diag_printf("[VId]WARNING!aud err overflow, a_frameBytes=%d a_PcmFrameTempBuf=%x end=%x", pIsomGlob->a_frameBytes, 
							(uint8*)pPcmBufPos, aud_extra_pcmbuf_state[A_EXBF_WT_ID].pBufPos+pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame);
		}

		AUD_DEC_FRM_NUM+=1;

		moveDataPos((int)vocDecStatus.consumedLen);

		if(AUD_DEC_FRM_NUM>=pIsomGlob->a_HalfPcmBufFrame)
		{
			AUD_DEC_FRM_NUM=0;
			if(VOC_AUD>0)
			{
				if(PCM_ISR_FLAG==0)
					memcpy(pIsomGlob->a_PcmBuf, (VOID*)((uint32)(aud_extra_pcmbuf_state[A_EXBF_RD_ID].pBufPos)|0x20000000), pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame);
				else
					memcpy(pIsomGlob->a_PcmBufHalfPos, (VOID*)((uint32)(aud_extra_pcmbuf_state[A_EXBF_RD_ID].pBufPos)|0x20000000), pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame);

				aud_extra_pcmbuf_state[A_EXBF_WT_ID].done=1;
				A_EXBF_WT_ID+=1;
				if(A_EXBF_WT_ID>=EXTRA_PCM_BUF_NUM)A_EXBF_WT_ID=0;
				
				aud_extra_pcmbuf_state[A_EXBF_RD_ID].done=0;
				A_EXBF_RD_ID+=1;
				if(A_EXBF_RD_ID>=EXTRA_PCM_BUF_NUM)A_EXBF_RD_ID=0;
				
				VOC_AUD--;
			}
			else
			{
				aud_extra_pcmbuf_state[A_EXBF_WT_ID].done=1;
				A_EXBF_WT_ID+=1;
				if(A_EXBF_WT_ID>=EXTRA_PCM_BUF_NUM)A_EXBF_WT_ID=0;
			}
		}
	}
	else
	{
		VOC_WORK=VOC_WORK_NON;
		VOC_VID=0;
		VOC_AUD=0;
		AUD_DEC_FRM_NUM=0;
		//AUD_EXTRA_BUF_DONE=0;
		return;
	}

start_yuv:	

	if((VOC_AUD==0 || (AUD_DEC_FRM_NUM & 0x0003)==AUD_DEC_MIN) && VOC_VID>0)
	{
		VOC_WORK=VOC_WORK_VID;

		vocDecVid.inStreamBufAddr=(INT32*)pIsomGlob->v_YuvInfo[0];
		vocDecVid.inStreamUBufAddr=(INT32*)pIsomGlob->v_YuvInfo[1];
		vocDecVid.inStreamVBufAddr=(INT32*)pIsomGlob->v_YuvInfo[2];
		vocDecVid.outStreamBufAddr=(INT32*)pIsomGlob->v_ImageBufPos;
		vocDecVid.Source_width=(short)pIsomGlob->v_YuvInfo[3];
		vocDecVid.Source_height=(short)pIsomGlob->v_YuvInfo[4];

		if(vpp_AudioJpegDecScheduleOneFrame(&vocDecVid)!=HAL_ERR_NO)
		{
			VOC_WORK=VOC_WORK_NON;
			diag_printf("[VID]voc call err!");
		}
		return;
	}

start_aud:	

	if(getDataLen()>0)
	{
		if(VOC_AUD>0 )
		{
			if(AUD_DEC_FRM_NUM!=0)
				pPcmBufPos+=pIsomGlob->a_frameBytes;
			else
			{
				//pPcmBufPos=(uint32)((PCM_ISR_FLAG)?pIsomGlob->a_PcmBufHalfPos:pIsomGlob->a_PcmBuf);
				pPcmBufPos=(uint32)(aud_extra_pcmbuf_state[A_EXBF_WT_ID].pBufPos);
			}
			
			vocDecAud.inStreamBufAddr=(INT32 *)getDataPos();//(pIsomGlob->a_SampleBuf+pIsomGlob->a_SampleBuf_BytesUsed);
			vocDecAud.outStreamBufAddr=(INT32 *)pPcmBufPos;
			vocDecAud.audioItf=audioItf;
			
			if(vpp_AudioJpegDecScheduleOneFrame(&vocDecAud)!=HAL_ERR_NO)
			{
				VOC_WORK=VOC_WORK_NON;
				diag_printf("[VID]voc call err!");
			}
			return;
		}
		else if( aud_extra_pcmbuf_state[A_EXBF_WT_ID].done==0)
		{
			if(AUD_DEC_FRM_NUM!=0)
				pPcmBufPos+=pIsomGlob->a_frameBytes;
#if 0
			else if(VOC_VID>0)
			{
				goto start_yuv;
			}
#endif
			else
			{
				pPcmBufPos=(uint32)(aud_extra_pcmbuf_state[A_EXBF_WT_ID].pBufPos);
			}
			
			vocDecAud.inStreamBufAddr=(INT32 *)getDataPos();//(pIsomGlob->a_SampleBuf+pIsomGlob->a_SampleBuf_BytesUsed);
			vocDecAud.outStreamBufAddr=(INT32 *)pPcmBufPos;
			vocDecAud.audioItf=audioItf;
			
			if(vpp_AudioJpegDecScheduleOneFrame(&vocDecAud)!=HAL_ERR_NO)
			{
				VOC_WORK=VOC_WORK_NON;
				diag_printf("[VID]voc call err!");
			}
			return;
		}
	}
	else
	{
		//aud_StreamStop(audioItf);
		if(AUD_DATA_REQ==1)AUD_DATA_REQ=2;
		diag_printf( "[VID] voc isr, aud no data");
	}
	
	AUD_DEC_FRM_NUM=0;
	VOC_AUD=0;
	
	if(VOC_VID>0)
	{
		goto start_yuv;
	}
	else
		VOC_WORK=VOC_WORK_NON;
	
}
#else


static void vid_voc_isr(HAL_VOC_IRQ_STATUS_T * status )
{
	int16 av_check=0;
    SSHdl *my_ss_handle = MpegPlayer.MpegInput.ss_handle;
     bt_a2dp_audio_cap_struct * my_audio_config_handle=   MpegPlayer.MpegInput.audio_config_handle ;
	if(VOC_WORK==VOC_WORK_AUD)
	{
		if(pIsomGlob->a_type != A_TYPE_AMR)
		{
			vpp_AudioJpegDecStatus(&vocDecStatus);

			if( is_SSHdl_valid(my_ss_handle)&& (my_audio_config_handle->codec_type ==1)&&(pIsomGlob->a_type != A_TYPE_AMR) )      //BT_A2DP_sbc
				my_ss_handle->PutData(my_ss_handle, (UINT8*) hal_VocGetPointer(SBC_Out_put_addr),vocDecStatus.ImgWidth);	 
			
			if(vocDecStatus.ErrorStatus)
			{
				if((uint8*)pPcmBufPos +pIsomGlob->a_frameBytes<= aud_extra_pcmbuf_state[A_EXBF_WT_ID].pBufPos+pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame)
				{
					memset((uint8*)pPcmBufPos, 0, pIsomGlob->a_frameBytes);
					diag_printf("[VId]aud err. output_len=%d", vocDecStatus.output_len);
				}
				else
					diag_printf("[VId]WARNING!aud err overflow, a_frameBytes=%d a_PcmFrameTempBuf=%x end=%x", pIsomGlob->a_frameBytes, 
						(uint8*)pPcmBufPos, aud_extra_pcmbuf_state[A_EXBF_WT_ID].pBufPos+pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame);
			}

			moveDataPos((int)vocDecStatus.consumedLen);
		}
		else
		{
			// or 0x20000000 and unuse cache for get output data update.
			memcpy((VOID*)pPcmBufPos, 
				(VOID*)(((UINT32)( vpp_SpeechGetRxPcmBuffer())|0x20000000) +RX_BUF_SWAP_ID*HAL_SPEECH_FRAME_SIZE_PCM_BUF), 
				pIsomGlob->a_frameBytes);
			
			RX_BUF_SWAP_ID=1-RX_BUF_SWAP_ID;
		}

		AUD_DEC_FRM_NUM+=1;

		if(AUD_DEC_FRM_NUM>=pIsomGlob->a_HalfPcmBufFrame)
		{
			AUD_DEC_FRM_NUM=0;
			if(VOC_AUD>0)
			{
				if(AUD_FADE_OUT_FLAG==0)
				{
					if(PCM_ISR_FLAG==0)
						memcpy(pIsomGlob->a_PcmBuf, (VOID*)((uint32)(aud_extra_pcmbuf_state[A_EXBF_RD_ID].pBufPos)|0x20000000), pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame);
					else
						memcpy(pIsomGlob->a_PcmBufHalfPos, (VOID*)((uint32)(aud_extra_pcmbuf_state[A_EXBF_RD_ID].pBufPos)|0x20000000), pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame);
				}
				
				aud_extra_pcmbuf_state[A_EXBF_WT_ID].done=1;
				A_EXBF_WT_ID+=1;
				if(A_EXBF_WT_ID>=EXTRA_PCM_BUF_NUM)A_EXBF_WT_ID=0;
				
				aud_extra_pcmbuf_state[A_EXBF_RD_ID].done=0;
				A_EXBF_RD_ID+=1;
				if(A_EXBF_RD_ID>=EXTRA_PCM_BUF_NUM)A_EXBF_RD_ID=0;
				
				VOC_AUD--;
			}
			else
			{
				aud_extra_pcmbuf_state[A_EXBF_WT_ID].done=1;
				A_EXBF_WT_ID+=1;
				if(A_EXBF_WT_ID>=EXTRA_PCM_BUF_NUM)A_EXBF_WT_ID=0;
			}
			
		}

		//mute frame
		if(AUD_FADE_IN_FLAG>1)
		{
			memset((uint8*)pPcmBufPos, 0, pIsomGlob->a_frameBytes);
			AUD_FADE_IN_FLAG--;
		}
		else if(AUD_FADE_IN_FLAG==1)
		{
			audFadeIn((int16*)pPcmBufPos, pIsomGlob->a_frame_sample*pIsomGlob->a_channels);
			AUD_FADE_IN_FLAG=0;
		}
		
		
		//diag_printf("[VID]aud done VOC_VID=%d",VOC_VID);
		goto start_vid;//check start from video
	}
	else if(VOC_WORK==VOC_WORK_VID)
	{
#ifdef RM_SUPPORT	
		vpp_data_buffer[vpp_buffer_rd_id].done=0;
		vpp_buffer_rd_id=1-vpp_buffer_rd_id;
		if(VOC_VID>=PARA_BUF_NUM)
			VOC_VID-=PARA_BUF_NUM;
		else
			VOC_VID=0;
#endif		
		//VOC_WORK=VOC_WORK_NON;
		//diag_printf("[VID]vid done, VOC_VID=%d", VOC_VID);
	}

	while(1)//check voc request for audio and video
	{
		//audio check
		if(pIsomGlob->a_type != A_TYPE_COOK && pIsomGlob->a_type != A_TYPE_PCM)
		{
			if(getDataLen()>0)
			{
				if((VOC_AUD>0 )||(aud_extra_pcmbuf_state[A_EXBF_WT_ID].done==0))
				{
					VOC_WORK=VOC_WORK_AUD;
						
					if(AUD_DEC_FRM_NUM!=0)
						pPcmBufPos+=pIsomGlob->a_frameBytes;
					else
					{
						pPcmBufPos=(uint32)(aud_extra_pcmbuf_state[A_EXBF_WT_ID].pBufPos);
					}

					if(pIsomGlob->a_type != A_TYPE_AMR)
					{
						vocDecAud.inStreamBufAddr=(INT32 *)getDataPos();//(pIsomGlob->a_SampleBuf+pIsomGlob->a_SampleBuf_BytesUsed);
						vocDecAud.outStreamBufAddr=(INT32 *)pPcmBufPos;
						vocDecAud.audioItf=audioItf;

						if(vpp_AudioJpegDecScheduleOneFrame(&vocDecAud)!=HAL_ERR_NO)
						{
							VOC_WORK=VOC_WORK_NON;
							diag_printf("[VID]voc call err!");
						}
						//diag_printf("isr voc stat a");
					}
					else
					{
						HAL_SPEECH_DEC_IN_T *pVocDecAmr=vpp_SpeechGetRxCodBuffer();
						
						get_amr_mode( getDataPos(), &(pIsomGlob->a_amr_mode), &(pIsomGlob->a_codeframeBytes));
						
						if(pIsomGlob->a_amr_mode != HAL_INVALID)
						{
							getData((U8*)pVocDecAmr->decInBuf, pIsomGlob->a_codeframeBytes, 1);
							pVocDecAmr->codecMode= pIsomGlob->a_amr_mode;
						}
						else
						{
							//write silence data
							moveDataPos(pIsomGlob->a_codeframeBytes);
							memcpy(pVocDecAmr->decInBuf, silence_frame475, 13);
							pVocDecAmr->codecMode= HAL_AMR475_DEC;
						}
						
						if(vpp_SpeechScheduleOneFrame(VPP_SPEECH_WAKEUP_SW_DEC) != HAL_ERR_NO)
						{
							VOC_WORK=VOC_WORK_NON;
							diag_printf("[VID]voc call err!");
						}
						
					}

					return;
				}
			}
			else
			{
				//AUD_DATA_REQ=0;
				if(AUD_DATA_REQ==1)AUD_DATA_REQ=2;
				diag_printf( "[VID] voc isr, aud no data");
			}

			AUD_DEC_FRM_NUM=0;
			VOC_AUD=0;
		}
		
		av_check+=1;//audio check over

		//video check
start_vid:

#ifdef RM_SUPPORT

		if(VOC_VID>0)
		{
			VOC_WORK=VOC_WORK_VID;
			//to do rm
			//if(vpp_AudioJpegDecGetMode()!=MMC_RM_DECODE )
			{
				vpp_AudioJpegDecSetCurMode(-1, -1);
				vpp_AudioJpegDecSetInputMode(MMC_RM_DECODE);
				//rm_printf("voc reload!!!");
			}
			vpp_rmvb_cfg.para_buf_ptr= (INT32)(vpp_data_buffer[vpp_buffer_rd_id].buffer) & VOC_CFG_DMA_EADDR_MASK;
			vpp_rmvb_cfg.mb_num = 	(VOC_VID>PARA_BUF_NUM)?(PARA_BUF_NUM):(VOC_VID);
			vpp_RMVBDecScheduleOneBlk(&vpp_rmvb_cfg);
			//diag_printf("isr voc stat v");
			return;
		}
		
#endif
		av_check+=1;//video check over
		
		if(av_check>=2)break;//no voc request
	}

	VOC_WORK=VOC_WORK_NON;	

}


#endif

static void vid_display(void) 
{
	LCDD_ERR_T lcdRes=0;
	UINT32 lcd_wait_i=0x2000000;
	uint16 *widthInBuf=0;
	uint16 *heightInBuf=0;

	if(pIsomGlob->bSkipFrame == TRUE)
	{
		pIsomGlob->bSkipFrame = FALSE;
		return;   
	}

	//lcdd_SetDir02();
	DISPLAY_STATE=1;

	if(pIsomGlob->v_ImageBuf_HalfFlag==0)
	{
		widthInBuf=(uint16*)&IMAGE_W_INBUF1;
		heightInBuf=(uint16*)&IMAGE_H_INBUF1;
	}
	else if(pIsomGlob->v_ImageBuf_HalfFlag==1)
	{
		widthInBuf=(uint16*)&IMAGE_W_INBUF0;
		heightInBuf=(uint16*)&IMAGE_H_INBUF0;
	}
	
	if(DISPLAY_INTERFACE==0)
	{
		if(pIsomGlob->v_display_width== *widthInBuf
			&& pIsomGlob->v_display_height== *heightInBuf)
		{
#if (CHIP_HAS_GOUDA != 1)
			if(pIsomGlob->v_ImageBuf_HalfFlag==0)
			{
				lcdDraw.fb.buffer =  (UINT16 *)pIsomGlob->v_ImageBuf2;
			}
			else
			{
				if(!NEED_BLEND_IMG)
					lcdDraw.fb.buffer =  (UINT16 *)pIsomGlob->v_ImageBuf;
				else
					lcdDraw.fb.buffer =  (UINT16 *)pIsomGlob->v_ImageBuf2;
			}
#endif


#ifndef USE_VOC_ROTATE
			
			SETUPGOUDA; // set gouda

			lcdDraw.fb.buffer = (UINT16 *)get_lcd_frame_buffer_address();
			lcdDraw.fb.width = pIsomGlob->v_lcd_size_x; 
	        	lcdDraw.fb.height = pIsomGlob->v_lcd_size_y; //pIsomGlob->v_cutH;
			lcdDraw.roi.x =pIsomGlob->v_lcd_start_x;
	        	lcdDraw.roi.y =pIsomGlob->v_lcd_start_y;
	        	lcdDraw.roi.width =pIsomGlob->v_cutW;
	        	lcdDraw.roi.height =pIsomGlob->v_cutH;
#else
			if(pIsomGlob->v_rotate==Rotate_with_0)
			{
				if(!NEED_BLEND_IMG)
				{
					lcdDraw.fb.width = pIsomGlob->v_display_width; 
			        	lcdDraw.fb.height = pIsomGlob->v_display_height;
					lcdDraw.roi.x= pIsomGlob->v_cutX;
					lcdDraw.roi.y= pIsomGlob->v_cutY;
					lcdDraw.roi.width =pIsomGlob->v_cutW;
					lcdDraw.roi.height=pIsomGlob->v_cutH;
				}
				else
				{
					lcdDraw.fb.width = pIsomGlob->v_lcd_size_x; 
			        	lcdDraw.fb.height = pIsomGlob->v_cutH;
					lcdDraw.roi.x= 0;
					lcdDraw.roi.y= 0;
					lcdDraw.roi.width =pIsomGlob->v_cutW;
					lcdDraw.roi.height=pIsomGlob->v_cutH;
				}
			}
			else if(pIsomGlob->v_rotate==Rotate_with_90)
			{
				if(!NEED_BLEND_IMG)
				{
					lcdDraw.fb.width = pIsomGlob->v_display_height; 
			        	lcdDraw.fb.height = pIsomGlob->v_display_width;
					lcdDraw.roi.x= pIsomGlob->v_display_height -(pIsomGlob->v_cutY+pIsomGlob->v_cutH);
					lcdDraw.roi.y= pIsomGlob->v_cutX;
					lcdDraw.roi.width =pIsomGlob->v_cutH;
					lcdDraw.roi.height=pIsomGlob->v_cutW;
				}
				else
				{
					lcdDraw.fb.width = pIsomGlob->v_lcd_size_x; 
			        	lcdDraw.fb.height = pIsomGlob->v_cutW;
					lcdDraw.roi.x= 0;
					lcdDraw.roi.y= 0;
					lcdDraw.roi.width =pIsomGlob->v_cutH;
					lcdDraw.roi.height=pIsomGlob->v_cutW;
				}
			}
			else
			{
				if(!NEED_BLEND_IMG)
				{
					lcdDraw.fb.width = pIsomGlob->v_display_height; 
			        	lcdDraw.fb.height = pIsomGlob->v_display_width;
					lcdDraw.roi.x= pIsomGlob->v_cutY;
					lcdDraw.roi.y= pIsomGlob->v_display_width-(pIsomGlob->v_cutX+pIsomGlob->v_cutW);
					lcdDraw.roi.width =pIsomGlob->v_cutH;
					lcdDraw.roi.height=pIsomGlob->v_cutW;
				}
				else
				{
					lcdDraw.fb.width = pIsomGlob->v_lcd_size_x; 
			        	lcdDraw.fb.height = pIsomGlob->v_cutW;
					lcdDraw.roi.x= 0;
					lcdDraw.roi.y= 0;
					lcdDraw.roi.width =pIsomGlob->v_cutH;
					lcdDraw.roi.height=pIsomGlob->v_cutW;
				}
			}
#endif
			if(lcdDraw.roi.width && lcdDraw.roi.height)
			{
#ifdef RM_SUPPORT
				if(pIsomGlob->v_type==V_TYPE_RV)
				{//wait last n mb for rv voc decode .
					lcd_wait_i=VOC_WORK_CONT;
					while(VOC_VID)
					{
						
						if(lcd_wait_i==0)
						{
							diag_printf("[VID]#WARNING# RV last N MB skip!");
							break;
						}
						//sxr_Sleep(1);
						
						lcd_wait_i--;
					}
				}
#endif

				lcdRes=lcdd_Blit16(&lcdDraw, pIsomGlob->v_lcd_start_x,
									pIsomGlob->v_lcd_start_y);
				lcd_wait_i=0x2000000;
				while (lcdRes!=LCDD_ERR_NO )
			       {
			       	
			       	if(lcd_wait_i==0)
			       	{
			       		diag_printf("[VID] LCD display too long!");
			       		break;
			       	}
					//sxr_Sleep(1);
					
					lcdRes=lcdd_Blit16(&lcdDraw, pIsomGlob->v_lcd_start_x,
										pIsomGlob->v_lcd_start_y);
					lcd_wait_i--;
			       }
			}

		}
		else if(IMAGE_INBUF_FLAG!=pIsomGlob->v_ImageBuf_HalfFlag)
		{
			IMAGE_INBUF_FLAG=-1;
		}
		
		*widthInBuf=0;
		*heightInBuf=0;
		pIsomGlob->v_ImageBuf_HalfFlag=1-pIsomGlob->v_ImageBuf_HalfFlag;
	}
	else if(DISPLAY_INTERFACE==1)
	{
		DISPLAY_INTERFACE=0;
		
		/*update the panel buffer*/
		pIsomGlob->vid_draw_panel_callback(pIsomGlob->current_time);

#if (CHIP_HAS_GOUDA == 1)

		SETUPGOUDA; // set gouda
		
#endif

		/*only display time bar area*/
		lcdDraw.fb.buffer =  (UINT16 *)get_lcd_frame_buffer_address();

		lcdDraw.fb.width = pIsomGlob->v_lcd_size_x; 
        	lcdDraw.fb.height = pIsomGlob->v_lcd_size_y;
			
#if (CHIP_HAS_GOUDA == 1)
		lcdDraw.roi.x =0;
		lcdDraw.roi.y =0;
        	lcdDraw.roi.width =pIsomGlob->v_lcd_size_x;
        	lcdDraw.roi.height =pIsomGlob->v_lcd_size_y;
#else
        	/*bar under side*/
        	lcdDraw.roi.x =0;

		if(pIsomGlob->v_rotate==Rotate_with_0)
			lcdDraw.roi.y=pIsomGlob->v_lcd_start_y+pIsomGlob->v_cutH;
		else
			lcdDraw.roi.y=pIsomGlob->v_lcd_start_y+pIsomGlob->v_cutW;

        	lcdDraw.roi.width =pIsomGlob->v_lcd_size_x;
        	lcdDraw.roi.height =pIsomGlob->v_lcd_size_y-lcdDraw.roi.y;
#endif
        	if(lcdDraw.roi.width && lcdDraw.roi.height)
        	{
#ifdef RM_SUPPORT
		if(pIsomGlob->v_type==V_TYPE_RV)
		{//wait last n mb for rv voc decode .
			lcd_wait_i=VOC_WORK_CONT;
			while(VOC_VID)
			{
				
				if(lcd_wait_i==0)
				{
					diag_printf("[VID]#WARNING# RV last N MB skip!");
					break;
				}				
				//sxr_Sleep(1);
				
				lcd_wait_i--;
			}
		}
#endif
			lcdRes=lcdd_Blit16(&lcdDraw, lcdDraw.roi.x, lcdDraw.roi.y);
			lcd_wait_i=0x2000000;
			while (lcdRes!=LCDD_ERR_NO  && lcdRes != LCDD_ERR_INVALID_PARAMETER)
		       {
		       	
		       	if(lcd_wait_i==0)
		       	{
		       		diag_printf("[VID] LCD display too long!");
		       		break;
		       	}
				//sxr_Sleep(1);
				
				lcdRes=lcdd_Blit16(&lcdDraw, lcdDraw.roi.x, lcdDraw.roi.y);
				lcd_wait_i--;
		       }
	       }

#if (CHIP_HAS_GOUDA != 1)			
		/*bar right side*/
		if(pIsomGlob->v_rotate==Rotate_with_0)
			lcdDraw.roi.x=pIsomGlob->v_lcd_start_x+pIsomGlob->v_cutW;
		else
			lcdDraw.roi.x=pIsomGlob->v_lcd_start_x+pIsomGlob->v_cutH;
        		
        	if(lcdDraw.roi.x<pIsomGlob->v_lcd_size_x)
        	{
	        	lcdDraw.roi.y =0;
	        	lcdDraw.roi.width =pIsomGlob->v_lcd_size_x-lcdDraw.roi.x;
	        	lcdDraw.roi.height =pIsomGlob->v_lcd_size_y-lcdDraw.roi.height;//except under bar height.
	        	if(lcdDraw.roi.width && lcdDraw.roi.height)
	        	{
				lcdRes=lcdd_Blit16(&lcdDraw, lcdDraw.roi.x, lcdDraw.roi.y);
				lcd_wait_i=0x2000000;
				while (lcdRes!=LCDD_ERR_NO  && lcdRes != LCDD_ERR_INVALID_PARAMETER)
			       {
			       	if(lcd_wait_i==0)
			       	{
			       		diag_printf("[VID] LCD display too long!");
			       		break;
			       	}
					//sxr_Sleep(1);
					lcdRes=lcdd_Blit16(&lcdDraw, lcdDraw.roi.x, lcdDraw.roi.y);
					lcd_wait_i--;
			       }
	        	}

		}
#endif			
	}
	else//DISPLAY_INTERFACE==2 display time bar with image area.
	{
		DISPLAY_INTERFACE=0;
		
		if(pIsomGlob->v_display_width== *widthInBuf
			&& pIsomGlob->v_display_height== *heightInBuf)
		{
#ifndef USE_VOC_ROTATE

			SETUPGOUDA;// set gouda
			
			lcdDraw.fb.buffer =  (UINT16 *)get_lcd_frame_buffer_address();
#else		
			if(pIsomGlob->v_rotate==Rotate_with_0 )
			{
				lcdDraw.fb.buffer =  (UINT16 *)get_lcd_frame_buffer_address();
				if(!NEED_BLEND_IMG && pIsomGlob->v_ImageBuf_HalfFlag==1)
				{
			        	if(VID_SET_MODE)
						EmbedRgbDataIntoLcdBuffer(pIsomGlob->v_lcd_start_x, pIsomGlob->v_lcd_start_y,
													pIsomGlob->v_display_width, pIsomGlob->v_display_height,
													pIsomGlob->v_cutX, pIsomGlob->v_cutY,
													pIsomGlob->v_cutW, pIsomGlob->v_cutH,
													((pIsomGlob->v_ImageBuf_HalfFlag==1)?pIsomGlob->v_ImageBuf:pIsomGlob->v_ImageBuf2),
													lcdDraw.fb.buffer );
				}
			}
			else
			{
				if( *widthInBuf==pIsomGlob->v_lcd_size_y && *heightInBuf==pIsomGlob->v_lcd_size_x)
				{
					if(!NEED_BLEND_IMG)
						lcdDraw.fb.buffer =  (UINT16 *)((pIsomGlob->v_ImageBuf_HalfFlag==1)?pIsomGlob->v_ImageBuf:pIsomGlob->v_ImageBuf2);
					else
						lcdDraw.fb.buffer=(UINT16 *)get_lcd_frame_buffer_address();
				}
				else
				{
					lcdDraw.fb.buffer =  (UINT16 *)get_lcd_frame_buffer_address();
					if(!NEED_BLEND_IMG && pIsomGlob->v_ImageBuf_HalfFlag==1)
					{
				        	if(VID_SET_MODE)
							EmbedRgbDataIntoLcdBuffer(pIsomGlob->v_lcd_start_x, pIsomGlob->v_lcd_start_y,
														pIsomGlob->v_display_height, pIsomGlob->v_display_width, 
														(pIsomGlob->v_rotate==Rotate_with_90)?(pIsomGlob->v_display_height-(pIsomGlob->v_cutY+pIsomGlob->v_cutH)):(pIsomGlob->v_cutY), 
														(pIsomGlob->v_rotate==Rotate_with_90)?(pIsomGlob->v_cutX):(pIsomGlob->v_display_width-(pIsomGlob->v_cutX+pIsomGlob->v_cutW)),
														pIsomGlob->v_cutH, 
														pIsomGlob->v_cutW,
														((pIsomGlob->v_ImageBuf_HalfFlag==1)?pIsomGlob->v_ImageBuf:pIsomGlob->v_ImageBuf2),
														lcdDraw.fb.buffer );
					}
				}
			}
#endif
			lcdDraw.fb.width = pIsomGlob->v_lcd_size_x; 
	        	lcdDraw.fb.height = pIsomGlob->v_lcd_size_y;
	        	lcdDraw.roi.x =0;
	        	lcdDraw.roi.y =0;
	        	lcdDraw.roi.width =pIsomGlob->v_lcd_size_x;
	        	lcdDraw.roi.height =pIsomGlob->v_lcd_size_y;

#ifdef RM_SUPPORT
		if(pIsomGlob->v_type==V_TYPE_RV)
		{//wait last n mb for rv voc decode .
			lcd_wait_i=VOC_WORK_CONT;
			while(VOC_VID)
			{
				
				if(lcd_wait_i==0)
				{
					diag_printf("[VID]#WARNING# RV last N MB skip!");
					break;
				}
				//sxr_Sleep(1);
				
				lcd_wait_i--;
			}
		}
#endif
				
			lcdRes=lcdd_Blit16(&lcdDraw, 0, 0);
			lcd_wait_i=0x2000000;
			while (lcdRes!=LCDD_ERR_NO )
		       {
		       	
		       	if(lcd_wait_i==0)
		       	{
		       		diag_printf("[VID] LCD display too long!");
		       		break;
		       	}
				//sxr_Sleep(1);
				
				lcdRes=lcdd_Blit16(&lcdDraw, 0, 0);
				lcd_wait_i--;
		       }
		}
		else if(*widthInBuf==0 && *heightInBuf==0)
		{
			lcdDraw.fb.buffer =  (UINT16 *)get_lcd_frame_buffer_address();
			lcdDraw.fb.width = pIsomGlob->v_lcd_size_x; 
	        	lcdDraw.fb.height = pIsomGlob->v_lcd_size_y;
	        	lcdDraw.roi.x =0;
	        	lcdDraw.roi.y =0;
	        	lcdDraw.roi.width =pIsomGlob->v_lcd_size_x;
	        	lcdDraw.roi.height =pIsomGlob->v_lcd_size_y;
				
#if (CHIP_HAS_GOUDA == 1)
			while(hal_GoudaIsActive())
			{
				diag_printf("[VID]gouda busy!");
				sxr_Sleep(8);
			}
#endif
	        	
			lcdRes=lcdd_Blit16(&lcdDraw, 0, 0);
			while (lcdRes!=LCDD_ERR_NO )
		       {
		       	
		       	if(lcd_wait_i==0)
		       	{
		       		diag_printf("[VID] LCD display too long!");
		       		break;
		       	}
				//sxr_Sleep(1);
				
				lcdRes=lcdd_Blit16(&lcdDraw, 0, 0);
				lcd_wait_i--;
		       }
		}
		else if(IMAGE_INBUF_FLAG!=pIsomGlob->v_ImageBuf_HalfFlag)
		{
			IMAGE_INBUF_FLAG=-1;
		}

		*widthInBuf=0;
		*heightInBuf=0;
		pIsomGlob->v_ImageBuf_HalfFlag=1-pIsomGlob->v_ImageBuf_HalfFlag;
	}

	DISPLAY_STATE=0;
}

#if 1

//#define DEBUG_PRINT

static int32 vid_av_dec_frame(void)
{
    int32 res;

    if(play_state!=VID_PLAY_STATE_PLAYING)
    {
        if(play_state==VID_PLAY_STATE_PAUSE)
        {
            pIsomGlob->a_Pause_DecReq=1;
            diag_printf("[VID] vid_av_dec_frame pause state! \n");
            COS_KillTimerEX(GetMCITaskHandle(MBOX_ID_MMC), MMC_VID_PLY_TIMER_ID);
            return  ISOM_ERR_OK;
        }
        else
        {
            diag_printf("[VID] vid_av_dec_frame play_state= %d not match\n",play_state);
            COS_KillTimerEX(GetMCITaskHandle(MBOX_ID_MMC), MMC_VID_PLY_TIMER_ID);
            return  ISOM_ERR_PLAY_STATE_FAIL;
        }
    }

    //AV_DEC_STATE=1;

    /////////////////////////////////audio read//////////////////////////////
    tick1=hal_TimGetUpTime();
    if(AUD_DATA_REQ==2)
    {
		if( pIsomGlob->a_current_sample_num < pIsomGlob->a_total_sample_num-1)
		{
			pIsomGlob->a_current_sample_num = (uint32)((unsigned long long)pIsomGlob->a_total_sample_num*(unsigned long long)(pIsomGlob->v_current_frame_num+1)/
														((unsigned long long)pIsomGlob->v_total_frame_num));

			if(pIsomGlob->med_type==MCI_TYPE_MJPG)
			{
			avi_set_audio_position2(pIsomGlob->avi_p, pIsomGlob->a_current_sample_num);
			}		
			AUD_DATA_REQ=1;
		}
		else
			AUD_DATA_REQ=0;
    }
		
    if(AUD_DATA_REQ)
    {
        fillSampBuf();
	fillAudioData=fillAudioDataFun;//fillSampBuf;//read audio in video decoder.
		
	if(pIsomGlob->a_type== A_TYPE_COOK)
	{
#ifdef RM_SUPPORT
		res=ra_nframe_dec();		

#endif
	}
	else if(pIsomGlob->a_type== A_TYPE_PCM)
        {
        	fillAudioDataFun();
		/*	
            while(AUD_DEC_FRM_NUM)
            {
                res=getData(((unsigned char*)pPcmBufPos)+(pIsomGlob->a_HalfPcmBufFrame-(unsigned int)AUD_DEC_FRM_NUM)*pIsomGlob->a_frameBytes,
                            pIsomGlob->a_frameBytes, (unsigned int)AUD_DEC_FRM_NUM);
                if(res>0 )
                {
                    if((pIsomGlob->a_channels==2)&&(audioItf==AUD_ITF_LOUD_SPEAKER))
                    {
                        uint32 i;
                        INT16 *pPcm=(INT16 *)(((unsigned char*)pPcmBufPos)+(pIsomGlob->a_HalfPcmBufFrame-(unsigned int)AUD_DEC_FRM_NUM)*pIsomGlob->a_frameBytes);
                        uint32 sNum=pIsomGlob->a_frame_sample*2*res;
						
                        for(i=0;i<sNum;i+=2)
                        {
                            pPcm[i]=pPcm[i+1]=(pPcm[i]>>1)+(pPcm[i+1]>>1);					
                        }
                    }
					
                    AUD_DEC_FRM_NUM-=(uint16)res;
                    fillSampBuf();
                }
                else
                {
                    fillSampBuf();
                    res=getDataLen();
                    if(res>0)
                    {
                        getData(((unsigned char*)pPcmBufPos)+(pIsomGlob->a_HalfPcmBufFrame-(unsigned int)AUD_DEC_FRM_NUM)*pIsomGlob->a_frameBytes,
                                res, 1);
								
                        if((pIsomGlob->a_channels==2)&&(audioItf==AUD_ITF_LOUD_SPEAKER))
                        {
                            uint32 i;
                            INT16 *pPcm=(INT16 *)(((unsigned char*)pPcmBufPos)+(pIsomGlob->a_HalfPcmBufFrame-(unsigned int)AUD_DEC_FRM_NUM)*pIsomGlob->a_frameBytes);
                            uint32 sNum=res>>1;
							
                            for(i=0;i<sNum;i+=2)
                            {
                                pPcm[i]=pPcm[i+1]=(pPcm[i]>>1)+(pPcm[i+1]>>1);					
                            }
                        }

                        memset(((unsigned char*)pPcmBufPos)+(pIsomGlob->a_HalfPcmBufFrame-(unsigned int)AUD_DEC_FRM_NUM)*pIsomGlob->a_frameBytes+res, 0, pIsomGlob->a_frameBytes-res);
                        AUD_DEC_FRM_NUM-=1;
                    }

                    if(AUD_DEC_FRM_NUM>0)
                        memset(((unsigned char*)pPcmBufPos)+(pIsomGlob->a_HalfPcmBufFrame-(unsigned int)AUD_DEC_FRM_NUM)*pIsomGlob->a_frameBytes, 0, pIsomGlob->a_frameBytes*AUD_DEC_FRM_NUM);

                    AUD_DEC_FRM_NUM = 0;
                    AUD_DATA_REQ = 0;
                //  aud_StreamStop(audioItf);
                    diag_printf("[VID] vid_av_dec_frame aud no data");
                    break;
                }
            }
            */
        }
    }
	
    tick2=hal_TimGetUpTime();
    totalTa+=(tick2-tick1);
    counta++;
    ///////////////////////////////////////video decode//////////////////////////////////////////////////////////
    //video_dec:
#if 1
    tick1=hal_TimGetUpTime();


    if(VID_DEC_REQ>0 ||pIsomGlob->med_type==MCI_TYPE_RM )
    {
        VID_DEC_REQ--;

		//display last rgb frame
        vid_display();
            
        pIsomGlob->v_current_frame_num+=1;

        //update play time when no audio.
        
        if(!pIsomGlob->open_audio)
        {
            uint32 timePos;
            uint32 timeOffset;

            timePos=hal_TimGetUpTime();
            timeOffset=((timePos-TIME_LINE_LAST)*1000)>>14;
            if(timeOffset>=SYNC_TIME_OFFSET)
            {
			unsigned long long temp;
			if(pIsomGlob->med_type!=MCI_TYPE_RM)
			{
				pIsomGlob->current_time+=timeOffset;
				// >>14 == /16384
				temp=(unsigned long long)(((unsigned long long)pIsomGlob->current_time*(unsigned long long)pIsomGlob->v_fps)>>V_FPS_SCALE_BIT)/1000;
				VID_DEC_REQ=(int16)(temp-pIsomGlob->v_current_frame_num);
			}
#ifdef RM_SUPPORT
			else
			{
				pIsomGlob->current_time=rm_getLastFrameTimeStamp();
				TIME_LINE_CUR+=timeOffset;
				temp=((unsigned long long)TIME_LINE_CUR-(unsigned long long)pIsomGlob->current_time)/(unsigned long long)pIsomGlob->v_period;
				VID_DEC_REQ=(int16)temp;
			}
#endif
				
                TIME_LINE_LAST=timePos;
                if(DISPLAY_SEND_EVENT_NUM==0)
                {
                    DISPLAY_SEND_EVENT_NUM=1;
                    mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VID_DISPLAY_INTERFACE_REQ);
                }
            }		
        }
		
#ifdef RM_SUPPORT
	if(pIsomGlob->med_type!=MCI_TYPE_RM)
	{
#endif
		if((pIsomGlob->v_current_frame_num+1)>=pIsomGlob->v_total_frame_num )
		{
			COS_KillTimerEX(GetMCITaskHandle(MBOX_ID_MMC), MMC_VID_PLY_TIMER_ID);
			VID_DEC_REQ=0;

			mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VID_STOP_REQ);
			diag_printf("[VID] video frame over v_current_frame_num=%d  \n",pIsomGlob->v_current_frame_num);
			//AV_DEC_STATE=0;
			return ISOM_ERR_OK;
		}
#ifdef RM_SUPPORT
	}
	else
	{
		if(rm_getLastFrameTimeStamp() >= pIsomGlob->total_time-500)
		{
			COS_KillTimerEX(GetMCITaskHandle(MBOX_ID_MMC), MMC_VID_PLY_TIMER_ID);
			VID_DEC_REQ=0;

			mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VID_STOP_REQ);
			diag_printf("[VID] video frame over cur time=%d \n",rm_getLastFrameTimeStamp() );
			//AV_DEC_STATE=0;
			return ISOM_ERR_OK;
		}
	}
#endif

        //need more decode frame?
        if(VID_DEC_REQ>0)
        {
            if(pIsomGlob->med_type==MCI_TYPE_MP4 || pIsomGlob->med_type==MCI_TYPE_3GP || 
				(pIsomGlob->med_type==MCI_TYPE_MJPG && pIsomGlob->v_type!=V_TYPE_MJPEG))
            {
 #ifdef USE_KEY_SYNC
                if(KEY_SAMP_NUM==0 && VID_DEC_REQ>(pIsomGlob->v_fps>>V_FPS_SCALE_BIT))
                {
                	int16 frame_num_need=VID_DEC_REQ;//get VID_DEC_REQ avoid change.
                    //get next key frame for av sync
                    if(pIsomGlob->med_type==MCI_TYPE_MP4 || pIsomGlob->med_type==MCI_TYPE_3GP)
                    KEY_SAMP_NUM=gf_isom_get_nextKey(pIsomGlob->v_trak, pIsomGlob->v_current_frame_num+frame_num_need);
		      else if(pIsomGlob->med_type==MCI_TYPE_MJPG)
			      KEY_SAMP_NUM=avi_get_video_keyframe_num(pIsomGlob->avi_p, pIsomGlob->v_current_frame_num+frame_num_need);
                    KEY_SAMP_TIME_WAIT=(KEY_SAMP_NUM-(pIsomGlob->v_current_frame_num+frame_num_need)-2)*pIsomGlob->v_timerPeriod;
                    KEY_SAMP_TIME_START=hal_TimGetUpTime();

                    if(KEY_SAMP_TIME_WAIT>=0)
                    {
                        diag_printf("[VID]KEY_SAMP_NUM=%d v_current_sample_num=%d Ready!", KEY_SAMP_NUM, pIsomGlob->v_current_sample_num);
                    }
                    else
                    {
                        KEY_SAMP_NUM=0;
                        KEY_SAMP_TIME_WAIT=0;
                        KEY_SAMP_TIME_START=0;
                    }
                }

                if(KEY_SAMP_NUM>0 && (hal_TimGetUpTime()-KEY_SAMP_TIME_START>=KEY_SAMP_TIME_WAIT))
                {
                    if(KEY_SAMP_NUM>pIsomGlob->v_current_sample_num)
                    {
                        diag_printf("[VID]KEY_SAMP_NUM=%d v_current_sample_num=%d Go!", KEY_SAMP_NUM, pIsomGlob->v_current_sample_num);
                        pIsomGlob->v_current_sample_num=KEY_SAMP_NUM;
                        pIsomGlob->v_current_frame_num=KEY_SAMP_NUM-2;
                        VID_DEC_REQ=0;
                    }
                    KEY_SAMP_NUM=0;
                    KEY_SAMP_TIME_WAIT=0;
                    KEY_SAMP_TIME_START=0;
                }
#endif			
                if(SEND_EVENT_NUM==0 && VID_DEC_REQ>0)
                {
                    mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VID_IMG_DEC_REQ);
                    SEND_EVENT_NUM=1;
                }
                diag_printf("[VID]VID_DEC_REQ=%d", VID_DEC_REQ);
            }
#ifdef RM_SUPPORT			
	     else if(pIsomGlob->med_type==MCI_TYPE_RM)
	     {//rm to do ... simulate mp4/3gp
#ifdef USE_KEY_SYNC
              //int32 tk1,tk2;
              //tk1=hal_TimGetUpTime();

                if(KEY_SAMP_NUM==0 && VID_DEC_REQ>(pIsomGlob->v_fps>>V_FPS_SCALE_BIT))
                {
                	uint32 stamp1,stamp2;
			
                    //get cur real time
                    stamp1=(uint32)(((long long)pIsomGlob->a_current_frame_num+(long long)pIsomGlob->a_frame_sample*(long long)pIsomGlob->a_HalfPcmBufFrame)*1000/(long long)pIsomGlob->a_sample_rate);
		      //add sync time offset.
			//seek and get next key time stamp.
		      stamp2=rm_getNextKeyTimeStamp(stamp1);
			
                    KEY_SAMP_NUM=stamp2;//recode the key frame time stamp.
                    KEY_SAMP_TIME_WAIT=((stamp2-stamp1)<<14)/1000; // *16384==<<14
                    KEY_SAMP_TIME_START=hal_TimGetUpTime();

                    if(KEY_SAMP_TIME_WAIT>=0)
                    {
                        diag_printf("[VID]stamp1=%d stamp2=%d wait=%d ms", stamp1,stamp2,(stamp2-stamp1));
                    }
                    else
                    {
                        KEY_SAMP_NUM=0;
                        KEY_SAMP_TIME_WAIT=0;
                        KEY_SAMP_TIME_START=0;
                    }
                }

                if(KEY_SAMP_NUM>0 && (hal_TimGetUpTime()-KEY_SAMP_TIME_START>=KEY_SAMP_TIME_WAIT))
                {
                //diag_printf("[VID]last stamp=%d !",rm_getLastFrameTimeStamp());
                    if(KEY_SAMP_NUM>rm_getLastFrameTimeStamp())
                    {
				 diag_printf("[VID]KEY_SAMP_NUM=%d cur stamp=%d Go!", KEY_SAMP_NUM, rm_getLastFrameTimeStamp());
			   rm_seek_video(KEY_SAMP_NUM);
		          pIsomGlob->v_current_sample_num=KEY_SAMP_NUM/pIsomGlob->v_period;
                        pIsomGlob->v_current_frame_num=pIsomGlob->v_current_sample_num-2;
                        VID_DEC_REQ=0;
                    }
			
                    KEY_SAMP_NUM=0;
                    KEY_SAMP_TIME_WAIT=0;
                    KEY_SAMP_TIME_START=0;
                }
				
                //tk2=hal_TimGetUpTime();
                //diag_printf("rm key seek time=%d",  (tk2-tk1)*1000/16384);
#endif			
                if(SEND_EVENT_NUM==0 && VID_DEC_REQ>0)
                {
                    mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VID_IMG_DEC_REQ);
                    SEND_EVENT_NUM=1;
                }
                diag_printf("[VID]VID_DEC_REQ=%d", VID_DEC_REQ);
	     }
#endif		 
            else
            {
                pIsomGlob->v_current_sample_num+=VID_DEC_REQ;
                pIsomGlob->v_current_frame_num+=VID_DEC_REQ;
                diag_printf("[VID]VID_DEC_REQ=%d", VID_DEC_REQ);
                VID_DEC_REQ=0;
            }
        }

#if 1			
        pIsomGlob->v_SampleBuf_BytesRemain-=pIsomGlob->v_SampleBuf_BytesUsed;
        if ((pIsomGlob->v_type != V_TYPE_MPEG4)&&
        	(pIsomGlob->v_type !=V_TYPE_RV)&&
		(pIsomGlob->v_SampleBuf_BytesUsed>0) && 
		(pIsomGlob->v_SampleBuf_BytesRemain > 0) && 
		pIsomGlob->v_SampleBuf_BytesRemain< pIsomGlob->v_SampleBufLen)
            memcpy(pIsomGlob->v_SampleBuf , pIsomGlob->v_SampleBuf+pIsomGlob->v_SampleBuf_BytesUsed, pIsomGlob->v_SampleBuf_BytesRemain);

        pIsomGlob->v_SampleBuf_BytesUsed=0;
        if(pIsomGlob->v_SampleBuf_BytesRemain<0)pIsomGlob->v_SampleBuf_BytesRemain=0;
		
        if(pIsomGlob->v_current_sample_num+1<pIsomGlob->v_total_sample_num)
        {
            if(pIsomGlob->med_type==MCI_TYPE_MP4 || pIsomGlob->med_type==MCI_TYPE_3GP)
            {
                uint32 readLen;
                uint32 gotbytes;
                uint32 gotcount;
                if((res = stbl_GetSampleSize(pIsomGlob->v_info , pIsomGlob->v_current_sample_num+1, &readLen))!=GF_OK)
                {
                    diag_printf("[VID] video stbl_GetSampleSize err res=%d \n",res);
                    //AV_DEC_STATE=0;
                    return ISOM_ERR_VIDEO_READ_FAIL;
                }
				
                if(pIsomGlob->v_SampleBuf_BytesRemain+readLen<pIsomGlob->v_SampleBufLen)
                {
                	if(pIsomGlob->v_type == V_TYPE_MPEG4)
                		pIsomGlob->v_SampleBuf -=readLen;//reuse from end of the Y buffer.
                	
                    res=gf_isom_get_multSample(pIsomGlob->v_info, 
                                                pIsomGlob->v_current_sample_num+1, 
                                                pIsomGlob->v_SampleBuf+pIsomGlob->v_SampleBuf_BytesRemain, 
                                                readLen, 	&gotbytes, &gotcount);

                    if(res || (gotbytes<=0))
                    {
                        diag_printf("[VID] video gf_isom_get_multSample err res=%d \n",res);
                        //AV_DEC_STATE=0;
                        return ISOM_ERR_VIDEO_READ_FAIL;
                    }

                    pIsomGlob->v_SampleBuf_BytesRemain+=gotbytes;
                    pIsomGlob->v_current_sample_num+=gotcount;
                }
                else
                {
                    diag_printf("[VID]too big readLen=%d v_SampleBuf_BytesRemain=%d \n",readLen, pIsomGlob->v_SampleBuf_BytesRemain);
                    //AV_DEC_STATE=0;
                    return ISOM_ERR_MEM_INSUFFICIENT;
                }
            }
	     else if(pIsomGlob->med_type==MCI_TYPE_RM)
	     {
	     	//to do ...
	     	pIsomGlob->v_SampleBuf_BytesRemain=1;
			
	     }
            else
            {
                int32 key_frame;
				
                res=avi_set_video_position(pIsomGlob->avi_p, pIsomGlob->v_current_sample_num);
                if(res!=0)
                {
                    diag_printf("[VID] avi_set_video_position err!");
                    //AV_DEC_STATE=0;
                    return ISOM_ERR_VIDEO_READ_FAIL;
                }

		  if(pIsomGlob->v_type == V_TYPE_MPEG4)
		  {
		  	//reuse from end of the Y buffer.
		  	pIsomGlob->v_SampleBuf -=avi_frame_size(pIsomGlob->avi_p, pIsomGlob->v_current_sample_num);
		  }
		  
                res=avi_read_frame(pIsomGlob->avi_p, pIsomGlob->v_SampleBuf+pIsomGlob->v_SampleBuf_BytesRemain, 
                                pIsomGlob->v_SampleBufLen-pIsomGlob->v_SampleBuf_BytesRemain,  &key_frame);
                if( res < 0 )
                {
                    diag_printf( "[VID]avi video frame read err res=%d \n", res ); 
                    //AV_DEC_STATE=0;
                    return ISOM_ERR_VIDEO_READ_FAIL;
                }

                pIsomGlob->v_SampleBuf_BytesRemain+=res;
                pIsomGlob->v_current_sample_num+=1;
            }
        }

        //if(USE_SLEEP)
        //	COS_Sleep(1);//release cpu for other work
        if(pIsomGlob->v_SampleBuf_BytesRemain > 0 )
        {
            switch(pIsomGlob->v_type)
            {
                case V_TYPE_H263:
                    {
#ifdef H263_SUPPORT
                        int32 used_bytes;
                        int32 gob;
                        if(pIsomGlob->v_SampleBuf_BytesRemain>0)
                        {
                            resetbits (pIsomGlob->v_SampleBuf+pIsomGlob->v_SampleBuf_BytesUsed, pIsomGlob->v_SampleBuf_BytesRemain);

                            gob= getheader ();

                            if(errorstate || !gob )
                            {
                                diag_printf("[VID] h.263 getheader err gob=%d \n",gob);
                                diag_printf("[VID] h.263 getheader err !data use=%d \n", ld->rdptr - ld->rdbfr);
                                //AV_DEC_STATE=0;
                                //return  ISOM_ERR_INVALID_FORMAT;
				    pIsomGlob->bSkipFrame=TRUE;//only skip error frame.
				    pIsomGlob->v_SampleBuf_BytesUsed+=pIsomGlob->v_SampleBuf_BytesRemain;
				    break;
                            }
						
                            getpicture ((uint8*)pIsomGlob->v_YuvInfo, pIsomGlob->v_lcd_size_x, pIsomGlob->v_lcd_start_x, pIsomGlob->v_lcd_start_y, gob, pZoom);
                            if(errorstate!=0)
                            {
                                diag_printf("[VID] h.263 getpicture err! \n");
                                //AV_DEC_STATE=0;
                                //return  ISOM_ERR_VIDEO_DEC_FAIL;
				    pIsomGlob->bSkipFrame=TRUE;//only skip error frame.
				    pIsomGlob->v_SampleBuf_BytesUsed+=pIsomGlob->v_SampleBuf_BytesRemain;
				    break;
                            }
						
                            used_bytes=ld->rdptr - ld->rdbfr;
                            /* Update buffer pointers */
						
                            if(used_bytes > 0) 
                            {
                                pIsomGlob->v_SampleBuf_BytesUsed+=used_bytes;
                            }
                        }
					
                        break;
#endif			
                    }
                case V_TYPE_MPEG4:
                    {
#ifdef MPEG4_SUPPORT
                        int32 used_bytes;
                        xvid_dec_stats_t xvid_dec_stats;

                        if(pIsomGlob->v_SampleBuf_BytesRemain>0)
                        {
				 while(pIsomGlob->v_SampleBuf_BytesRemain > MIN_USEFUL_BYTES)
                    		{
	                            used_bytes = dec_main(pIsomGlob->v_SampleBuf+pIsomGlob->v_SampleBuf_BytesUsed,   
	                                                            (uint8*)pIsomGlob->v_YuvInfo, 
	                                                            pIsomGlob->v_SampleBuf_BytesRemain, &xvid_dec_stats, pZoom,
	                                                            ((pIsomGlob->med_type ==MCI_TYPE_MJPG) ? TRUE : FALSE));

	                            if(used_bytes == -1)//profile unfit
	                            {
	                                diag_printf("[VID] video decode err v_current_sample_num=%d \n",pIsomGlob->v_current_sample_num);
	                                //AV_DEC_STATE=0;
	                                return ISOM_ERR_VIDEO_DEC_FAIL;
	                            }
	                            
	                            if(used_bytes == -2)
	                            {
	                                pIsomGlob->bSkipFrame = TRUE;
	                                pIsomGlob->v_SampleBuf_BytesUsed = pIsomGlob->v_SampleBuf_BytesRemain;
	                            }
							
			                if(used_bytes > 0) 
			                {
	            				  if(xvid_dec_stats.type == XVID_TYPE_VOL)
						  {
						  	pIsomGlob->v_SampleBuf_BytesUsed+=used_bytes;
							pIsomGlob->v_SampleBuf_BytesRemain-=used_bytes;
						  	continue;
						  }
				                 pIsomGlob->v_SampleBuf_BytesUsed+=pIsomGlob->v_SampleBuf_BytesRemain;
			                }

					break;
                        	}
				 if(pIsomGlob->v_SampleBuf_BytesRemain==MIN_USEFUL_BYTES) pIsomGlob->v_SampleBuf_BytesUsed=1;
                        }
                        else//for final 1 sample out
                        {
                            used_bytes = dec_main(NULL,  (uint8*)pIsomGlob->v_YuvInfo, -1, &xvid_dec_stats, pZoom,
                                                        ((pIsomGlob->med_type ==MCI_TYPE_MJPG) ? TRUE : FALSE));

                            if(used_bytes == -2)
                            {
                                pIsomGlob->bSkipFrame = TRUE;
                            }
                        }
						
		   		pIsomGlob->v_SampleBuf=dec_get_cur_y_end();//reuse for next frame data read.
				if(!pIsomGlob->v_SampleBuf)
				{
					diag_printf("[VID] dec_get_cur_y_end ERR!\n");
					return ISOM_ERR_VIDEO_DEC_FAIL;
				}


                        break;
#endif
                    }

                case V_TYPE_MJPEG:
                    {
                        DEC_UNIT jpgDec;
                        res= MMF_JpegDecodePicture((long)((char*)(pIsomGlob->v_SampleBuf)), 
                                                    (char*) pIsomGlob->v_YuvInfo[0],
                                                    (pIsomGlob->v_width*pIsomGlob->v_height*3)>>1,
                                                    pIsomGlob->v_width, pIsomGlob->v_height, 
                                                    0,0,
                                                    pIsomGlob->v_width-1, pIsomGlob->v_height-1, 
                                                    &jpgDec, 2,fillAudioData);
                        if(res==FUNC_OK)
                        {
                            pIsomGlob->v_YuvInfo[3]=(uint32)jpgDec.width;
                            pIsomGlob->v_YuvInfo[4]=(uint32)jpgDec.height;
                            pIsomGlob->v_SampleBuf_BytesUsed+=pIsomGlob->v_SampleBuf_BytesRemain;
                            VID_ERR_FRAME_REC=0;
                        }
                        else
                        {
                            VID_ERR_FRAME_REC+=1;
                            if(VID_ERR_FRAME_REC<VID_ERR_FRAME_NUM)
                            {
                                diag_printf("[VID]MMF_JpegDecodePicture err!res=%d VID_ERR_FRAME_REC=%d", res,VID_ERR_FRAME_REC);
                                pIsomGlob->v_SampleBuf_BytesUsed+=pIsomGlob->v_SampleBuf_BytesRemain;
                            }
                            else
                            {
                                diag_printf("[VID]MMF_JpegDecodePicture err!res=%d VID_ERR_FRAME_REC=%d", res,VID_ERR_FRAME_REC);
                                //AV_DEC_STATE=0;
                                VID_ERR_FRAME_REC=0;
                                return ISOM_ERR_VIDEO_DEC_FAIL;
                            }
                        }

                        break;
                    }
#ifdef RM_SUPPORT
		     case V_TYPE_RV:
		     {
			 	VocWorkStruct vocState;
				INT8 skip=0;
              //int32 tk1,tk2;
              //tk1=hal_TimGetUpTime();
				
				vocState.pVOC_WORK=(UINT8*)&VOC_WORK;
				vocState.pVOC_VID=(int16*)&VOC_VID;
				vocState.vid_voc_isr=vid_voc_isr;
				if(pIsomGlob->a_type== A_TYPE_COOK)
					vocState.ra_dec=(void*)ra_nframe_dec;
				else
					vocState.ra_dec=NULL;

				if(keep_skip_frame_num<MAX_KEEP_SKIP_F_NUM && VID_DEC_REQ>0)
				{
					skip=1;
				}

			 	res=rm_getOneFrameVideo((INT8*)pIsomGlob->v_YuvInfo, &vocState, skip);
				if(res==HXR_SKIP_BFRAME)
				{
					pIsomGlob->bSkipFrame = TRUE;
					keep_skip_frame_num+=1;
					diag_printf("[VID] skip B");
				}
				else if(res!=HXR_OK)
			       {
			              diag_printf("[VID] rm_getOneFrameVideo err!res=%d ", res);
					//AV_DEC_STATE=0;
			              return ISOM_ERR_VIDEO_DEC_FAIL;
			       }
				else
				{
					keep_skip_frame_num=0;
				}

				pIsomGlob->v_SampleBuf_BytesUsed=1;
                //tk2=hal_TimGetUpTime();
                //    diag_printf("rm frame time=%d",  (tk2-tk1)*1000/16384);

			 	break;
		      }
#endif
                default:
                    break;
            }	
#endif
        }
        else
        {
            pIsomGlob->bSkipFrame = TRUE;
        }


#if (CHIP_HAS_GOUDA != 1)
        VOC_SETTING=1;//protect the follow set and call, avoid pcm ISR insert and change

        if(VOC_WORK==VOC_WORK_NON)
        {
            if (pIsomGlob->bSkipFrame == FALSE)
            {
            	  if(!NEED_BLEND_IMG)
                	pIsomGlob->v_ImageBufPos=(pIsomGlob->v_ImageBuf_HalfFlag==0)?(pIsomGlob->v_ImageBuf):(pIsomGlob->v_ImageBuf2);
		  else
                	pIsomGlob->v_ImageBufPos=pIsomGlob->v_ImageBuf;
                //pIsomGlob->v_ImageBufPos+=pIsomGlob->v_lcd_start_y*pIsomGlob->v_lcd_size_x;
                IMAGE_INBUF_FLAG=(int8)pIsomGlob->v_ImageBuf_HalfFlag;
			
                //@@maybe pcm ISR insert here will make mistake@@//
                VOC_WORK=VOC_WORK_VID;
                VOC_VID=1;

                vocDecVid.inStreamBufAddr=(INT32*)pIsomGlob->v_YuvInfo[0];
                vocDecVid.inStreamUBufAddr=(INT32*)pIsomGlob->v_YuvInfo[1];
                vocDecVid.inStreamVBufAddr=(INT32*)pIsomGlob->v_YuvInfo[2];
                vocDecVid.outStreamBufAddr=(INT32*)pIsomGlob->v_ImageBufPos;
                vocDecVid.Source_width=(short)pIsomGlob->v_YuvInfo[3];
                vocDecVid.Source_height=(short)pIsomGlob->v_YuvInfo[4];

                if(vpp_AudioJpegDecScheduleOneFrame(&vocDecVid)!=HAL_ERR_NO)
                {
                    VOC_WORK=VOC_WORK_NON;
                    diag_printf("[VID]voc call err!");
                }
            }
            else
                pIsomGlob->bSkipFrame = TRUE;
        }
        else if((VOC_WORK==VOC_WORK_AUD && VOC_VID==0) )
        {
            if(!NEED_BLEND_IMG)
            		pIsomGlob->v_ImageBufPos=(pIsomGlob->v_ImageBuf_HalfFlag==0)?(pIsomGlob->v_ImageBuf):(pIsomGlob->v_ImageBuf2);
	     else
            		pIsomGlob->v_ImageBufPos=pIsomGlob->v_ImageBuf;
            //pIsomGlob->v_ImageBufPos+=pIsomGlob->v_lcd_start_y*pIsomGlob->v_lcd_size_x;
            IMAGE_INBUF_FLAG=(int8)pIsomGlob->v_ImageBuf_HalfFlag;
			
            VOC_VID++;
        }
        else
        {
            diag_printf("[VID]voc Drop 1 frame display");
        }

	VOC_SETTING=0;
#else
        if( pIsomGlob->bSkipFrame == FALSE)
        {
            IMAGE_INBUF_FLAG=(int8)pIsomGlob->v_ImageBuf_HalfFlag;
			
            //@@maybe pcm ISR insert here will make mistake@@//
            /*
            if( pIsomGlob->v_ImageBuf_HalfFlag==0 )   // copy YUV data to backup buffer
            {
                memcpy((uint8*)pIsomGlob->v_YuvInfo2[0], (uint8*)pIsomGlob->v_YuvInfo[0], (pIsomGlob->v_YuvInfo[3]*pIsomGlob->v_YuvInfo[4]));
                memcpy((uint8*)pIsomGlob->v_YuvInfo2[1], (uint8*)pIsomGlob->v_YuvInfo[1], (pIsomGlob->v_YuvInfo[3]*pIsomGlob->v_YuvInfo[4])/4);
                memcpy((uint8*)pIsomGlob->v_YuvInfo2[2], (uint8*)pIsomGlob->v_YuvInfo[2], (pIsomGlob->v_YuvInfo[3]*pIsomGlob->v_YuvInfo[4])/4);

                pIsomGlob->v_YuvInfo2[3]= pIsomGlob->v_YuvInfo[3];
                pIsomGlob->v_YuvInfo2[4]= pIsomGlob->v_YuvInfo[4];				
            }*/
            
            vid_update();	
        }
#endif
        

        tick2=hal_TimGetUpTime();
	if(pIsomGlob->bSkipFrame != TRUE)
	{
	        totalTv+=(tick2-tick1);
	        countv++;
		//diag_printf("frame time=%d",  (tick2-tick1)*1000/16384);
	}

       sxr_Sleep(48);//COS_Sleep ms , sxr_Sleep tick 16384tick=1s 16tick=1ms
	
    }
#endif

    //AV_DEC_STATE=0;
    return ISOM_ERR_OK;
}

#endif

static int32 vid_export_info_from_parse(GF_ISOFile *file )
{
	GF_DecoderConfig *temp_dcfg=NULL;
	u32 trackID, i,  m_type, m_stype;
	u32 track_count=0;
	u32 av_track=0;
	
	track_count = gf_isom_get_track_count(file);
	if(track_count<2)
	{
		diag_printf("[VID] unsupport only video or only audio! track_count=%d\n", track_count);
		return ISOM_ERR_UNSUPPORT;
	}
	
	for(i=0;i<track_count/*MAX_TRACK_NUM*/;i++)
	{
		//trackID = gf_isom_get_track_by_id(file, i+1);
		trackID=i+1;
		
		m_type = gf_isom_get_media_type(file, trackID);//vide soun
		m_stype = gf_isom_get_media_subtype(file, trackID, 1);

		if(m_type==GF_ISOM_MEDIA_VISUAL || m_type==GF_ISOM_MEDIA_AUDIO)
		{
			av_track++;
		}
		else if(i<track_count-1)
			continue;
		else
		{
			diag_printf("[VID] unsupport m_type, av_track=%d track_count=%d\n",av_track, track_count);
			return ISOM_ERR_UNSUPPORT;
		}

		pIsomGlob->aacPara.is_aac=0;
		pIsomGlob->aacPara.aac_type=0;
		temp_dcfg=NULL;
		
		if ((m_stype==GF_ISOM_BOX_TYPE_MP4V) ||(m_stype==GF_ISOM_BOX_TYPE_MP4A))
		    temp_dcfg = gf_isom_get_decoder_config(file, trackID, 1);//esds

		if(m_type==GF_ISOM_MEDIA_VISUAL)
		{
			if(m_stype==GF_ISOM_BOX_TYPE_MP4V)
				pIsomGlob->v_type=V_TYPE_MPEG4;
			else if(m_stype==GF_ISOM_BOX_TYPE_S263)
				pIsomGlob->v_type=V_TYPE_H263;
			//pIsomGlob->v_type=m_stype;
			pIsomGlob->v_trackID=trackID;
			pIsomGlob->v_trak= gf_isom_get_track_from_file(file,trackID);
			if(!pIsomGlob->v_trak)
			{
				diag_printf("[VID]  err pIsomGlob->v_trak=%x \n",pIsomGlob->v_trak);
				return ISOM_ERR_PARAM_ERROR;
			}

			if(pIsomGlob->v_trak->Media) 
			{
				if(pIsomGlob->v_trak->Media->information) 
				{
					pIsomGlob->v_info=(GF_MediaInformationBox *)pIsomGlob->v_trak->Media->information;
					pIsomGlob->v_info->lastSampleNumber=0;
					pIsomGlob->v_info->lastFirstSample=0;
					pIsomGlob->v_info->lastOffset=0;
					
					if(!pIsomGlob->v_info->dataHandler) 
					{
						if(pIsomGlob->v_trak->Media->mediaTrack) 
						{
							if(pIsomGlob->v_trak->Media->mediaTrack->moov) 
							{
								if(pIsomGlob->v_trak->Media->mediaTrack->moov->mov)
								{
									if(pIsomGlob->v_trak->Media->mediaTrack->moov->mov->movieFileMap)
										pIsomGlob->v_info->dataHandler = pIsomGlob->v_trak->Media->mediaTrack->moov->mov->movieFileMap;
									else
										return ISOM_ERR_PARAM_ERROR;
								}
								else
									return ISOM_ERR_PARAM_ERROR;
							}
							else
								return ISOM_ERR_PARAM_ERROR;
						}
						else
							return ISOM_ERR_PARAM_ERROR;
					}
				}
				else
					return ISOM_ERR_PARAM_ERROR;
			}
			else
				return ISOM_ERR_PARAM_ERROR;

 			if(pIsomGlob->v_info->sampleTable)
 			{
 				if(pIsomGlob->v_info->sampleTable->TimeToSample)
 				{
					 if(stbl_GetSampleDTScount(pIsomGlob->v_info->sampleTable->TimeToSample)>1)
					 {
					 	DTS_FLG=1;
						if(pIsomGlob->v_trak->Media->mediaHeader)
						{
							pIsomGlob->v_timeScale=pIsomGlob->v_trak->Media->mediaHeader->timeScale;
						}
						else
							return ISOM_ERR_PARAM_ERROR;
					 }
					 else
					 {
					 	DTS_FLG=0;
						pIsomGlob->v_timeScale=0;
					 }
 				}
				else
					return ISOM_ERR_PARAM_ERROR;
 			}
			else
				return ISOM_ERR_PARAM_ERROR;

			pIsomGlob->total_time=gf_isom_get_sample_totaltime(file, pIsomGlob->v_trackID);
			pIsomGlob->v_fps= gf_isom_get_sample_FPS(file, pIsomGlob->v_trackID);//fps<<V_FPS_SCALE_BIT
			pIsomGlob->v_height= gf_isom_get_sample_height(file, pIsomGlob->v_trackID);
			pIsomGlob->v_width= gf_isom_get_sample_width(file, pIsomGlob->v_trackID);
			pIsomGlob->v_total_sample_num = gf_isom_get_sample_count(file, pIsomGlob->v_trackID);//total sample(frame) Num
			pIsomGlob->v_total_frame_num=pIsomGlob->v_total_sample_num;//will change
			pIsomGlob->v_total_keyframe_num=gf_isom_get_total_KeySamp_count(file, pIsomGlob->v_trackID);

		}
		else if (m_type==GF_ISOM_MEDIA_AUDIO)
		{
			if(m_stype==GF_ISOM_BOX_TYPE_MP4A)
				pIsomGlob->a_type=A_TYPE_AAC;
			else if(m_stype==GF_ISOM_BOX_TYPE_SAMR)
				pIsomGlob->a_type=A_TYPE_AMR;
			else if(m_stype==GF_ISOM_BOX_TYPE_MP3)
				pIsomGlob->a_type=A_TYPE_MP3;
			//pIsomGlob->a_type=m_stype;
			pIsomGlob->a_trackID=trackID;
			pIsomGlob->a_trak= gf_isom_get_track_from_file(file,trackID);
			if(!pIsomGlob->a_trak)
			{
				diag_printf("[VID]  err pIsomGlob->a_trak=%x \n",pIsomGlob->a_trak);
				return ISOM_ERR_PARAM_ERROR;
			}

			if(pIsomGlob->a_trak->Media) 
			{
				if(pIsomGlob->a_trak->Media->information) 
				{
					pIsomGlob->a_info=(GF_MediaInformationBox *)pIsomGlob->a_trak->Media->information;
					pIsomGlob->a_info->lastSampleNumber=0;
					pIsomGlob->a_info->lastFirstSample=0;
					pIsomGlob->a_info->lastOffset=0;
					
					if(!pIsomGlob->a_info->dataHandler) 
					{
						if(pIsomGlob->a_trak->Media->mediaTrack) 
						{
							if(pIsomGlob->a_trak->Media->mediaTrack->moov) 
							{
								if(pIsomGlob->a_trak->Media->mediaTrack->moov->mov)
								{
									if(pIsomGlob->a_trak->Media->mediaTrack->moov->mov->movieFileMap)
										pIsomGlob->a_info->dataHandler = pIsomGlob->a_trak->Media->mediaTrack->moov->mov->movieFileMap;
									else
										return ISOM_ERR_PARAM_ERROR;
								}
								else
									return ISOM_ERR_PARAM_ERROR;
							}
							else
								return ISOM_ERR_PARAM_ERROR;
						}
						else
							return ISOM_ERR_PARAM_ERROR;
					}
				}
				else
					return ISOM_ERR_PARAM_ERROR;
			}
			else
				return ISOM_ERR_PARAM_ERROR;

			pIsomGlob->a_sample_rate=gf_isom_get_sample_samplerate(file, pIsomGlob->a_trackID);
			pIsomGlob->a_bit=gf_isom_get_sample_bitspersample(file, pIsomGlob->a_trackID);
			if (temp_dcfg) 
				pIsomGlob->a_bitrate=temp_dcfg->maxBitrate;
			pIsomGlob->a_channels=gf_isom_get_sample_channel_count(file, pIsomGlob->a_trackID);
			pIsomGlob->a_total_sample_num = gf_isom_get_sample_count(file, pIsomGlob->a_trackID);//total sample(chunk) Num
			//pIsomGlob->a_total_frame_num=pIsomGlob->a_total_sample_num;//will change
			pIsomGlob->a_total_frame_num=0;

		}
		else
		{
			diag_printf("[VID] unsupport m_type=%x \n",m_type);
			return ISOM_ERR_UNSUPPORT;
		}
		
		if (temp_dcfg) 
		{
			switch (temp_dcfg->streamType) 
			{
				case GF_STREAM_VISUAL:
					switch (temp_dcfg->objectTypeIndication) {
			        		//Extracting MPEG-4 Visual stream to m4v ".m4v"
					case 0x20:
						{
							//memcpy(pIsomGlob->v_SampleBuf,temp_dcfg->decoderSpecificInfo->data, temp_dcfg->decoderSpecificInfo->dataLength);
							//pIsomGlob->v_SampleBuf_BytesRemain+=temp_dcfg->decoderSpecificInfo->dataLength;
							pIsomGlob->v_decSpeInfo=temp_dcfg->decoderSpecificInfo;
							diag_printf("[VID] MPEG4");
						}
						break;
			        		//Extracting MPEG-4 AVC-H264 stream to h264".h264"
					case 0x21:
			    			//avccfg = gf_isom_avc_config_get(file, pIsomGlob->v_trackID, 1);
						break;
			       		//Extracting MPEG-1 Visual stream to h264".m1v"
					case 0x6A:
						break;
			        		//Extracting MPEG-2 Visual stream to h264".m2v"
					case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: 
						break;
			        		//Extracting JPEG image".jpg"
					case 0x6C:
						break;
			        		//Extracting PNG image".png"
					case 0x6D:
						break;
					default:
						return ISOM_ERR_UNSUPPORT;
					}
					break;
				case GF_STREAM_AUDIO:
					switch (temp_dcfg->objectTypeIndication) 
					{
				       		 //Extracting MPEG-2 AAC".aac"
						case 0x66:
						case 0x67:
						case 0x68:
							pIsomGlob->aacPara.is_aac = A_TYPE_MPEG2_AAC;
							pIsomGlob->aacPara.aac_type = temp_dcfg->objectTypeIndication - 0x66;
							gf_m4a_get_config(temp_dcfg->decoderSpecificInfo->data, temp_dcfg->decoderSpecificInfo->dataLength, &pIsomGlob->a_cfg);
							diag_printf("[VID] MPEG2_AAC");
							break;
				        		//Extracting MPEG-4 AAC ".aac"
						case 0x40:
							pIsomGlob->aacPara.is_aac = A_TYPE_MPEG4_AAC;
							gf_m4a_get_config(temp_dcfg->decoderSpecificInfo->data, temp_dcfg->decoderSpecificInfo->dataLength, &pIsomGlob->a_cfg);
							pIsomGlob->aacPara.aac_type = pIsomGlob->a_cfg.base_object_type - 1;
							diag_printf("[VID] MPEG4_AAC");
							break;
				        		//Extracting MPEG-1/2 Audio (MP3)".mp3"
						case 0x69:
						case 0x6B:
							break;
						default:
							diag_printf("[VID] unsupport temp_dcfg->objectTypeIndication=%x \n",temp_dcfg->objectTypeIndication);
				            		return ISOM_ERR_UNSUPPORT;
					}
				    	break;
				default:
					diag_printf("[VID] unsupport temp_dcfg->streamType=%x \n",temp_dcfg->streamType);
					return ISOM_ERR_UNSUPPORT;
			}
			
			if (temp_dcfg->streamType==GF_STREAM_AUDIO && pIsomGlob->aacPara.is_aac) 
			{
				GF_BitToInt bs;
				memset(aacHead, 0, AAC_HEAD_SIZE);
				bs.stream=aacHead;
				bs.current=0;
				bs.nbBits=0;
			
				gf_bs_bit_to_int(&bs, 0xFFF, 12);/*sync*/
				gf_bs_bit_to_int(&bs, (pIsomGlob->aacPara.is_aac==A_TYPE_MPEG2_AAC) ? 1 : 0, 1);/*mpeg2 aac*/
				gf_bs_bit_to_int(&bs, 0, 2); /*layer*/
				gf_bs_bit_to_int(&bs, 1, 1); /* protection_absent*/
				gf_bs_bit_to_int(&bs, pIsomGlob->aacPara.aac_type, 2);
				gf_bs_bit_to_int(&bs, pIsomGlob->a_cfg.base_sr_index, 4);
				gf_bs_bit_to_int(&bs, 0, 1);
				gf_bs_bit_to_int(&bs, pIsomGlob->a_cfg.nb_chan, 3);
				gf_bs_bit_to_int(&bs, 0, 4);
				//gf_bs_bit_to_int(&bs, AAC_HEAD_SIZE+dataLength, 13);
				gf_bs_bit_to_int(&bs, 0, 13);//13bit will be rewrite when every sample read
				gf_bs_bit_to_int(&bs, 0x7FF, 11);
				gf_bs_bit_to_int(&bs, 0, 2);
			}
			//gf_odf_desc_del((GF_Descriptor *) dcfg);
		} 
#ifdef H263_SUPPORT
		else if(m_stype==GF_ISOM_SUBTYPE_3GP_H263) //add shenh
		{
			//return ISOM_ERR_UNSUPPORT;//change shenh
			pIsomGlob->h263Sp=gf_isom_get_h263_spec(file, pIsomGlob->v_trackID);
			if(!pIsomGlob->h263Sp) return ISOM_ERR_UNSUPPORT;
			diag_printf("[VID] H263");
		}
#endif
		else if(m_stype==GF_ISOM_SUBTYPE_3GP_AMR)//add shenh
		{
			//return ISOM_ERR_UNSUPPORT;//change shenh
			pIsomGlob->amrSp=gf_isom_get_amr_spec(file, pIsomGlob->a_trackID);
			if(!pIsomGlob->amrSp) return ISOM_ERR_UNSUPPORT;
			diag_printf("[VID] AMR");
		}
		else if(m_stype==GF_ISOM_BOX_TYPE_MP3)
		{
			diag_printf("[VID] MP3");
		}
		else
		{
			/*
			udesc = gf_isom_get_generic_sample_description(file, trackID, 1);
			if (udesc) {
				dsi = udesc->extension_buf; udesc->extension_buf = NULL;
				dsi_size = udesc->extension_buf_size;
			}
			if (udesc) free(udesc);
			*/
			diag_printf("[VID] unsupport  m_stype=%x \n",m_stype);
			return ISOM_ERR_UNSUPPORT;//change shenh
		}

		if(av_track>=2)
			return ISOM_ERR_OK;
	}
	
	
	return ISOM_ERR_OK;
}

INT32 *G_VidSBCConstY=NULL;
extern const INT32 G_VppSBCConstX[];

int32 mmc_vid_isom_open_file( HANDLE file_handle ,  uint8 file_mode, uint32 data_Len, int32 audio_path, uint8 open_audio, mci_type_enum type, void(*vid_play_finish_callback)(int32), void (*vid_draw_panel_callback)(uint32))
{
	int32 res=0,i;
      SSHdl *my_ss_handle = MpegPlayer.MpegInput.ss_handle;
      bt_a2dp_audio_cap_struct * my_audio_config_handle=   MpegPlayer.MpegInput.audio_config_handle ;
#ifdef VID_XCPU_FREQ_SET
	  XCPU_FREQ_STATE=0;
#endif
	
	if(play_state!=VID_PLAY_STATE_INVALID)
	{
		diag_printf("[VID] mmc_vid_isom_open_file err play_state= %d \n",play_state);
		res= ISOM_ERR_PLAY_STATE_FAIL;
		goto open_error;
	}

	pIsomGlob=NULL;

	//malloc global parameters
	pIsomGlob=(IsomPlayGlobal*)mmc_MemMalloc(sizeof(IsomPlayGlobal));
	if(pIsomGlob)
		memset(pIsomGlob, 0, sizeof(IsomPlayGlobal));
	else
	{
		res=ISOM_ERR_MALLOC_FAIL;
		goto open_error;
	}
	
#if (defined MPEG4_SUPPORT || defined H263_SUPPORT)
	pZoom=(zoomPar*)mmc_MemMalloc(sizeof(zoomPar));
	if(pZoom)
		memset(pZoom, 0, sizeof(zoomPar));
	else
	{
		res=ISOM_ERR_MALLOC_FAIL;
		goto open_error;
	}
#endif

	pIsomGlob->med_type=type;
	
	if(vid_play_finish_callback)
		pIsomGlob->vid_play_finish_callback=vid_play_finish_callback;
	else
	{
		res=ISOM_ERR_PARAM_ERROR;
		goto open_error;
	}

	if(vid_draw_panel_callback)
		pIsomGlob->vid_draw_panel_callback=vid_draw_panel_callback;
	else
	{
		res=ISOM_ERR_PARAM_ERROR;
		goto open_error;
	}

	if(file_mode && (!file_handle || data_Len<=0))
	{
		res=ISOM_ERR_PARAM_ERROR;
		goto open_error;
	}


   /******************************************************************
                     parse  file
    ******************************************************************/
	switch(pIsomGlob->med_type)
	{
		case MCI_TYPE_MP4:
		case MCI_TYPE_3GP:
			res= gf_isom_open_file( file_handle, file_mode, data_Len, &pIsomGlob->file);
			if (res) 
			{
		    		diag_printf( "[VID] Error opening file \n");
				res=ISOM_ERR_OPEN_PARSE_FILE_FAIL;
				goto open_error;		
			}	

			  /******************************************************************
			                     export  file
			   ******************************************************************/
		  
			if( (res=vid_export_info_from_parse(pIsomGlob->file))!=ISOM_ERR_OK)
			{	
				diag_printf("[VID] gf_media_export_info err res=%d \n",res);
				goto open_error;		
			}
			
			break;
		case MCI_TYPE_MJPG:
			pIsomGlob->avi_p=NULL;
			pIsomGlob->avi_p=avi_open_input_file(file_handle, file_mode, data_Len, 1);
			if(pIsomGlob->avi_p==NULL)
			{
				diag_printf( "[VID]##ERROR## avi_open_input_file error %d \n",avi_get_error()); 
				res=ISOM_ERR_OPEN_FILE_FAIL;
				goto open_error;		
			}

			if((uint32)*((uint32*)pIsomGlob->avi_p->compressor2)==AVI_FORMAT_MJPG)
			{
				diag_printf("[VID] MJPEG");
				pIsomGlob->v_type=V_TYPE_MJPEG;
			}
			else if(((uint32)*((uint32*)pIsomGlob->avi_p->compressor2)==AVI_FORMAT_XVID)
				|| ((uint32)*((uint32*)pIsomGlob->avi_p->compressor2)==AVI_FORMAT_XVID2))
			{
				diag_printf("[VID] XVID");
				pIsomGlob->v_type=V_TYPE_MPEG4;
			}
			else if((uint32)*((uint32*)pIsomGlob->avi_p->compressor2)==AVI_FORMAT_DX50)
			{
				diag_printf("[VID] DX50");
				pIsomGlob->v_type=V_TYPE_MPEG4;
			}
			else if((uint32)*((uint32*)pIsomGlob->avi_p->compressor2)==AVI_FORMAT_DIV3)
			{
				diag_printf("[VID] DIV3");
				pIsomGlob->v_type=V_TYPE_MPEG4;
			}
			else if((uint32)*((uint32*)pIsomGlob->avi_p->compressor2)==AVI_FORMAT_DIVX)
			{
				diag_printf("[VID] DIVX");
				pIsomGlob->v_type=V_TYPE_MPEG4;
			}
			else if((uint32)*((uint32*)pIsomGlob->avi_p->compressor2)==AVI_FORMAT_PM4V)
			{
				diag_printf("[VID] PM4V DIV4");
				pIsomGlob->v_type=V_TYPE_MPEG4;
			}
			else
			{
				res=ISOM_ERR_UNSUPPORT;
				diag_printf("[VID] unsupport video type=%x \n",*(uint32*)pIsomGlob->avi_p->compressor2);
				goto open_error;		
			}
			
			if(pIsomGlob->avi_p->a_fmt==AVI_FORMAT_AMR)
			{
				pIsomGlob->a_type=A_TYPE_AMR;
				diag_printf("[VID] AMR");
			}
			else if(pIsomGlob->avi_p->a_fmt==AVI_FORMAT_AMR_2)
			{
				//same as AMR, only FCC different.
				pIsomGlob->a_type=A_TYPE_AMR;
				diag_printf("[VID] AMR 2");
			}
			else if(pIsomGlob->avi_p->a_fmt==AVI_FORMAT_MP3)
			{
				pIsomGlob->a_type=A_TYPE_MP3;
				diag_printf("[VID] MP3");
			}
			else if(pIsomGlob->avi_p->a_fmt==AVI_FORMAT_PCM)
			{
				pIsomGlob->a_type=A_TYPE_PCM;
				diag_printf("[VID] PCM");
			}
			else if(pIsomGlob->avi_p->a_fmt==AVI_FORMAT_ADPCM)
			{
				pIsomGlob->a_type=A_TYPE_ADPCM;
				//open_audio=0;// not support ADPCM now.
				//diag_printf("[VID] ADPCM");
				res=ISOM_ERR_UNSUPPORT;
				diag_printf("[VID] unsupport audio ADPCM now \n");
				goto open_error;		
			}
			else
			{
				res=ISOM_ERR_UNSUPPORT;
				diag_printf("[VID] unsupport audio type=%x \n",pIsomGlob->avi_p->a_fmt);
				goto open_error;		
			}
			pIsomGlob->a_channels=(uint16)pIsomGlob->avi_p->a_chans;
			pIsomGlob->a_sample_rate=pIsomGlob->avi_p->a_rate;
			pIsomGlob->a_bit=pIsomGlob->avi_p->a_bits;
			pIsomGlob->a_bitrate=pIsomGlob->avi_p->mp3rate;
			pIsomGlob->a_total_sample_num=pIsomGlob->avi_p->audio_chunks;
			pIsomGlob->a_total_frame_num=0;
			
			pIsomGlob->v_total_sample_num=pIsomGlob->avi_p->video_frames;
			pIsomGlob->v_total_frame_num=pIsomGlob->v_total_sample_num;
			pIsomGlob->v_fps=pIsomGlob->avi_p->fps;
			if(pIsomGlob->v_fps)
				pIsomGlob->total_time=pIsomGlob->v_total_sample_num*1000/pIsomGlob->v_fps;
			pIsomGlob->v_fps<<=V_FPS_SCALE_BIT;
			pIsomGlob->v_width=pIsomGlob->avi_p->width;
			pIsomGlob->v_height=pIsomGlob->avi_p->height;
			
			break;
#ifdef RM_SUPPORT
		case MCI_TYPE_RM:
			{
				rmInfo rm_Info;
				
#ifdef USE_OVERLAY
				VID_OVERLAY_ID=HAL_OVERLAY_INT_SRAM_ID_4;
				res=hal_OverlayLoad(VID_OVERLAY_ID);
				if(res!=HAL_ERR_NO)
					diag_printf( "[VID]RV HAL_OVERLAY_INT_SRAM_ID_4 hal_OverlayLoad  res=%d",res);
#endif

				res=rm_init(file_handle, &rm_Info);
				if(res != HXR_OK)
				{
					diag_printf( "[VID]##ERROR## rm_init error %d \n",res); 
					res=ISOM_ERR_OPEN_FILE_FAIL;
					goto open_error;		
				}

				pIsomGlob->total_time=rm_Info.rm_Duration;
				
				if(rm_Info.ra_Format==RA_CODEC_RAAC|| rm_Info.ra_Format==RA_CODEC_RACP)
				{
					//raac
					pIsomGlob->a_type=A_TYPE_AAC;
					
					//estimate the total frame number.
					if(rm_Info.ra_SampleRate)
						pIsomGlob->a_total_sample_num=(uint32)((long long)rm_Info.rm_Duration*(long long)rm_Info.ra_SampleRate/(AAC_FRAME_SAMPLE*1000));
					else
					{
						res=ISOM_ERR_PARAM_ERROR;
						diag_printf("[VID] rm audio ra_SampleRate=0 ERR!\n");
						goto open_error;		
					}
					pIsomGlob->a_total_frame_num=pIsomGlob->a_total_sample_num;
					diag_printf("[VID] RM AAC\n");
				}
				else if(rm_Info.ra_Format==RA_CODEC_SIPR)
				{
					//real audio voice
					pIsomGlob->a_type=A_TYPE_RA;
					pIsomGlob->a_total_frame_num=pIsomGlob->a_total_sample_num=1;
					//res=ISOM_ERR_UNSUPPORT;
					diag_printf("[VID] unsupport rm audio type RA_INTERLEAVER_SIPR\n");
					//goto open_error;		
				}
				else if(rm_Info.ra_Format==RA_CODEC_COOK )
				{
					uint32 codingDelay=0;
					
					//real audio 8 low bitrate (cook)
					pIsomGlob->a_type=A_TYPE_COOK;
					
					pIsomGlob->a_codeframeBytes=rm_Info.ra_BitsPerFrame>>3;
					if(rm_Info.ra_NumChannels)
						pIsomGlob->a_frame_sample=rm_Info.ra_SamplePerFrame/rm_Info.ra_NumChannels;//256,512,1024
	
					//estimate the total frame number.
					if(rm_Info.ra_SampleRate && rm_Info.ra_SamplePerFrame)
						pIsomGlob->a_total_sample_num=(uint32)((long long)rm_Info.rm_Duration*(long long)rm_Info.ra_SampleRate/((long long)rm_Info.ra_SamplePerFrame*1000));
					else
					{
						res=ISOM_ERR_PARAM_ERROR;
						diag_printf("[VID] rm audio ra_SampleRate=0 ERR!\n");
						goto open_error;		
					}
					pIsomGlob->a_total_frame_num=pIsomGlob->a_total_sample_num;

					pIsomGlob->a_Gecko2DecHdl=Gecko2InitDecoder(pIsomGlob->a_frame_sample,
																	rm_Info.ra_NumChannels,
																	rm_Info.ra_Regions,
																	rm_Info.ra_BitsPerFrame, 
																	rm_Info.ra_SampleRate,
																	rm_Info.ra_cplStart, 
																	rm_Info.ra_cplQBits,	
																	&codingDelay);
					diag_printf("[VID] RM low bitrate cook\n");
					diag_printf("[VID] cook. regions=%d bitPerFrame=%d cplStart=%d cplQBits=%d codingDelay=%d\n",
						rm_Info.ra_Regions,rm_Info.ra_BitsPerFrame, rm_Info.ra_cplStart, rm_Info.ra_cplQBits,codingDelay);

					if(pIsomGlob->a_Gecko2DecHdl==0)
					{
						res=ISOM_ERR_COOK_INIT_FAIL;
						diag_printf("[VID] Gecko2InitDecoder fail!\n");
						goto open_error;		
					}
				}
				else if(rm_Info.ra_Format==RA_CODEC_ATRC)
				{
					//real audio 8 high bitrate(atrc)
					pIsomGlob->a_type=A_TYPE_RA;
					pIsomGlob->a_total_frame_num=pIsomGlob->a_total_sample_num=1;
					//res=ISOM_ERR_UNSUPPORT;
					diag_printf("[VID] unsupport rm audio type RA_INTERLEAVER_GENR \n");
					//goto open_error;		
				}
				else
				{
					pIsomGlob->a_type=A_TYPE_RA;
					pIsomGlob->a_total_frame_num=pIsomGlob->a_total_sample_num=1;
					//res=ISOM_ERR_UNSUPPORT;
					diag_printf("[VID] unsupport rm audio type=%x \n", rm_Info.ra_Format);
					//goto open_error;		
				}
				//pIsomGlob->a_type=A_TYPE_PCM;//for debug
				pIsomGlob->a_channels=(uint16) rm_Info.ra_NumChannels;
				pIsomGlob->a_sample_rate=rm_Info.ra_SampleRate;
				pIsomGlob->a_bit=rm_Info.ra_BitsPerSample;
				pIsomGlob->a_bitrate=0;

				if(rm_Info.rv_Format==HX_RV30VIDEO_ID || rm_Info.rv_Format==HX_RV40VIDEO_ID
					|| rm_Info.rv_Format==HX_RV89COMBO_ID)
				{
					pIsomGlob->v_type=V_TYPE_RV;
				}
				else
				{
					res=ISOM_ERR_UNSUPPORT;
					diag_printf("[VID] unsupport rm video type=%x \n", rm_Info.rv_Format);
					goto open_error;		
				}
				//estimate the total frame number.
				pIsomGlob->v_total_sample_num=rm_Info.rm_Duration*rm_Info.rv_Fps/1000;
				pIsomGlob->v_total_frame_num=pIsomGlob->v_total_sample_num;
				pIsomGlob->v_fps=rm_Info.rv_Fps;
				pIsomGlob->v_fps<<=V_FPS_SCALE_BIT;
				pIsomGlob->v_width=rm_Info.rv_Width;
				pIsomGlob->v_height=rm_Info.rv_Height;
//tick1=hal_TimGetUpTime();

				res=rm_seek(pIsomGlob->total_time);//for init the seek table.
				if(res != HXR_OK)
				{
					diag_printf( "[VID]##ERROR## rm seek table init error %d \n",res); 
					res=ISOM_ERR_OPEN_FILE_FAIL;
					goto open_error;		
				}

//tick2=hal_TimGetUpTime();
//diag_printf("[VID]rm seek table init time=%d", (tick2-tick1)*1000/16384);
			}
			break;
#endif
		default:
			res=ISOM_ERR_UNSUPPORT;
			diag_printf("[VID] unsupport media type=%d \n",type);
			goto open_error;		
	}
	
	switch(pIsomGlob->a_type)
	{
		case A_TYPE_AAC:
			pIsomGlob->a_vocDecType=MMC_AAC_DECODE;
			if(pIsomGlob->med_type==MCI_TYPE_MP4 || pIsomGlob->med_type==MCI_TYPE_3GP)
			{
				if(pIsomGlob->a_channels!=pIsomGlob->a_cfg.nb_chan)
				{
					if(pIsomGlob->a_cfg.nb_chan!=0)
						pIsomGlob->a_channels=pIsomGlob->a_cfg.nb_chan;
					diag_printf("[VID] real channels pIsomGlob->a_cfg.nb_chan=%d !!!\n",pIsomGlob->a_cfg.nb_chan);
				}

				if(pIsomGlob->a_sample_rate!=pIsomGlob->a_cfg.base_sr)
				{
					if(pIsomGlob->a_cfg.base_sr!=0)
						pIsomGlob->a_sample_rate=pIsomGlob->a_cfg.base_sr;
					diag_printf("[VID] real a_sample_rate pIsomGlob->a_cfg.sbr_sr=%d !!!\n",pIsomGlob->a_cfg.base_sr);
				}
			}

			pIsomGlob->a_frame_sample=AAC_FRAME_SAMPLE;
			pIsomGlob->a_frameBytes=(AAC_FRAME_SAMPLE*pIsomGlob->a_channels<<1);
			pIsomGlob->a_HalfPcmBufFrame=(PCM_BUF_AAC_FRAME_NUM>>1);
			pIsomGlob->a_SampBufPipeLen=MAX_A_SAMPLEBUF_PIPE_SIZE;
			pIsomGlob->a_SampBufMirrorLen=MAX_A_SAMPLEBUF_MIRROR_SIZE;
			
			if(!pIsomGlob->a_sample_rate)
			{
				diag_printf("[VID] err pIsomGlob->a_sample_rate=%d !!!\n",pIsomGlob->a_sample_rate);
				res=ISOM_ERR_PARAM_ERROR;
				goto open_error;		
			}
			pIsomGlob->a_frame_Period=((AAC_FRAME_SAMPLE*1000)+(pIsomGlob->a_sample_rate>>1))/pIsomGlob->a_sample_rate;
			
			break;
		case A_TYPE_AMR:
			pIsomGlob->a_vocDecType=MMC_AMR_DECODE;
			
			if(pIsomGlob->a_channels!=1 || pIsomGlob->a_sample_rate!=8000)
			{
				if(pIsomGlob->med_type==MCI_TYPE_MP4 || pIsomGlob->med_type==MCI_TYPE_3GP)
				{
					if(pIsomGlob->amrSp->mode_set & 0x0080)
					{
						pIsomGlob->a_channels=1;
						pIsomGlob->a_sample_rate=8000;
						diag_printf("[VID] pIsomGlob->amrSp->mode_set=%x \n", pIsomGlob->amrSp->mode_set);
					}
					else
					{
						pIsomGlob->a_channels=1;
						pIsomGlob->a_sample_rate=8000;
						diag_printf("[VID] WARNING! pIsomGlob->amrSp->mode_set=%x \n", pIsomGlob->amrSp->mode_set);
						//res=ISOM_ERR_INVALID_FORMAT;
						//goto open_error;
					}
				}	
				else
				{
					diag_printf("[VID] warning!amr a_channels=%d a_sample_rate=%d\n", pIsomGlob->a_channels, pIsomGlob->a_sample_rate);
					pIsomGlob->a_channels=1;
					pIsomGlob->a_sample_rate=8000;
				}
			}

			pIsomGlob->a_frame_sample=AMR_FRAME_SAMPLE;
			pIsomGlob->a_frameBytes=(AMR_FRAME_SAMPLE*pIsomGlob->a_channels<<1);
			pIsomGlob->a_HalfPcmBufFrame=(PCM_BUF_AMR_FRAME_NUM>>1);
			pIsomGlob->a_SampBufPipeLen=MAX_A_SAMPLEBUF_PIPE_SIZE;
			pIsomGlob->a_SampBufMirrorLen=MAX_A_SAMPLEBUF_MIRROR_SIZE;
			
			if(!pIsomGlob->a_sample_rate)
			{
				diag_printf("[VID] err pIsomGlob->a_sample_rate=%d !!!\n",pIsomGlob->a_sample_rate);
				res=ISOM_ERR_PARAM_ERROR;
				goto open_error;		
			}
			pIsomGlob->a_frame_Period=((AMR_FRAME_SAMPLE*1000)+(pIsomGlob->a_sample_rate>>1))/pIsomGlob->a_sample_rate;
			
			break;
		case A_TYPE_MP3:
			pIsomGlob->a_vocDecType=MMC_MP3_DECODE;

			if(pIsomGlob->a_sample_rate>=32000)
				pIsomGlob->a_frame_sample=MP3_FRAME_SAMPLE_1152;
			else 
				pIsomGlob->a_frame_sample=MP3_FRAME_SAMPLE_576;
			pIsomGlob->a_frameBytes=(pIsomGlob->a_frame_sample*pIsomGlob->a_channels<<1);
			pIsomGlob->a_HalfPcmBufFrame=(PCM_BUF_MP3_FRAME_NUM>>1);
			pIsomGlob->a_SampBufPipeLen=MAX_A_SAMPLEBUF_PIPE_SIZE;
			pIsomGlob->a_SampBufMirrorLen=MAX_A_SAMPLEBUF_MIRROR_SIZE;
			
			if(!pIsomGlob->a_sample_rate)
			{
				diag_printf("[VID] err pIsomGlob->a_sample_rate=%d !!!\n",pIsomGlob->a_sample_rate);
				res=ISOM_ERR_PARAM_ERROR;
				goto open_error;		
			}
			pIsomGlob->a_frame_Period=((pIsomGlob->a_frame_sample*1000)+(pIsomGlob->a_sample_rate>>1))/pIsomGlob->a_sample_rate;
			
			break;
		case A_TYPE_PCM:
		case A_TYPE_ADPCM:
			//no voc decoder, input pcm directly.
			pIsomGlob->a_frame_sample=PCM_FRAME_SAMPLE;
			pIsomGlob->a_frameBytes=(pIsomGlob->a_frame_sample*pIsomGlob->a_channels<<1);
			pIsomGlob->a_HalfPcmBufFrame=(PCM_BUF_PCM_FRAME_NUM>>1);
			pIsomGlob->a_SampBufPipeLen=MAX_A_SAMPLEBUF_PIPE_SIZE;
			pIsomGlob->a_SampBufMirrorLen=MAX_A_SAMPLEBUF_MIRROR_SIZE;
			
			if(!pIsomGlob->a_sample_rate)
			{
				diag_printf("[VID] err pIsomGlob->a_sample_rate=%d !!!\n",pIsomGlob->a_sample_rate);
				res=ISOM_ERR_PARAM_ERROR;
				goto open_error;		
			}
			pIsomGlob->a_frame_Period=((pIsomGlob->a_frame_sample*1000)+(pIsomGlob->a_sample_rate>>1))/pIsomGlob->a_sample_rate;
			
			break;
		case A_TYPE_COOK:
			//c decode. use like pcm
			//pIsomGlob->a_frame_sample get from rmvb.
			pIsomGlob->a_frameBytes=(pIsomGlob->a_frame_sample*pIsomGlob->a_channels<<1);
			pIsomGlob->a_HalfPcmBufFrame=(PCM_BUF_COOK_FRAME_NUM>>1);
			pIsomGlob->a_SampBufPipeLen=MAX_A_SAMPLEBUF_PIPE_SIZE;
			pIsomGlob->a_SampBufMirrorLen=MAX_A_SAMPLEBUF_MIRROR_SIZE;
			if(!pIsomGlob->a_sample_rate)
			{
				diag_printf("[VID] err pIsomGlob->a_sample_rate=%d !!!\n",pIsomGlob->a_sample_rate);
				res=ISOM_ERR_PARAM_ERROR;
				goto open_error;		
			}
			pIsomGlob->a_frame_Period=((pIsomGlob->a_frame_sample*1000)+(pIsomGlob->a_sample_rate>>1))/pIsomGlob->a_sample_rate;
			
			break;
		case A_TYPE_RA:
			
			open_audio=0;//not support ra.
			pIsomGlob->a_frame_sample=1;
			pIsomGlob->a_frameBytes=1;
			pIsomGlob->a_HalfPcmBufFrame=1;
			pIsomGlob->a_SampBufPipeLen=MAX_A_SAMPLEBUF_PIPE_SIZE;
			pIsomGlob->a_SampBufMirrorLen=MAX_A_SAMPLEBUF_MIRROR_SIZE;
			
			break;
		default:
			diag_printf("[VID] err! pIsomGlob->a_type=%x \n", pIsomGlob->a_type);
			res=ISOM_ERR_INVALID_FORMAT;
			goto open_error;
	}
	
	if(!pIsomGlob->v_fps)
	{
		diag_printf("[VID] err pIsomGlob->v_fps=%d !!!\n",pIsomGlob->v_fps);
		res=ISOM_ERR_PARAM_ERROR;
		goto open_error;		
	}

	pIsomGlob->v_period=(uint32)((1000<<V_FPS_SCALE_BIT)+(pIsomGlob->v_fps>>1))/pIsomGlob->v_fps;
	if(!pIsomGlob->v_period)
	{
		diag_printf("[VID] err pIsomGlob->v_period=%d !!!\n",pIsomGlob->v_period);
		res=ISOM_ERR_PARAM_ERROR;
		goto open_error;		
	}
	pIsomGlob->v_timerPeriod=(16384<<V_FPS_SCALE_BIT)/pIsomGlob->v_fps;

	if(pIsomGlob->med_type!=MCI_TYPE_RM)
		pIsomGlob->a_sync_distance_frame_num=SYNC_TIME_OFFSET*pIsomGlob->a_sample_rate/(1000*pIsomGlob->a_frame_sample);//one sync / 500ms
	else//reuse a_sync_distance_frame_num to record pcm sample distance for rmvb
		pIsomGlob->a_sync_distance_frame_num=SYNC_TIME_OFFSET*pIsomGlob->a_sample_rate/1000;

	/******************
	pcm
	time=pIsomGlob->a_current_frame_num*(1000*pIsomGlob->a_frame_sample/pIsomGlob->a_sample_rate);
	video fram num=time/(1000/(pIsomGlob->v_fps>>V_FPS_SCALE_BIT));
				  =pIsomGlob->a_current_frame_num*(1000*pIsomGlob->a_frame_sample/pIsomGlob->a_sample_rate)/(1000/(pIsomGlob->v_fps>>V_FPS_SCALE_BIT))
				  =pIsomGlob->a_current_frame_num*pIsomGlob->a_frame_sample*(pIsomGlob->v_fps>>V_FPS_SCALE_BIT)/pIsomGlob->a_sample_rate;
				  =pIsomGlob->a_current_frame_num*SYNC_NUM_COEFFICIENT>>(V_FPS_SCALE_BIT+ SYNC_COEFF_SCALE_BIT)
	*******************/
	SYNC_NUM_COEFFICIENT=((pIsomGlob->a_frame_sample*pIsomGlob->v_fps<<SYNC_COEFF_SCALE_BIT)+(pIsomGlob->a_sample_rate>>1))/pIsomGlob->a_sample_rate;

  	//Lily_Lcd_GetSize(&pIsomGlob->v_lcd_size_x, &pIsomGlob->v_lcd_size_y);
	pIsomGlob->v_lcd_size_x=MMC_LcdWidth;
	pIsomGlob->v_lcd_size_y=MMC_LcdHeight;

	pIsomGlob->v_display_width=pIsomGlob->v_width;//be resize
	pIsomGlob->v_display_height=pIsomGlob->v_height;
#if (defined MPEG4_SUPPORT || defined H263_SUPPORT)
	pZoom->zoom_width=pIsomGlob->v_width;//be resize
	pZoom->zoom_height=pIsomGlob->v_height;
	pZoom->zoom_mode=0;
	pZoom->rotate_mode=0;
#endif
	
	//limit condition
	if(pIsomGlob->v_width<=LIMIT_WIDTH_320 && pIsomGlob->v_height<=LIMIT_HEIGHT_240)
	{
		USE_SLEEP=0;
		if(pIsomGlob->v_width>LIMIT_WIDTH_320 || pIsomGlob->v_height>LIMIT_HEIGHT_240)
			USE_SLEEP=1;
		
		/*default display size*/
		pIsomGlob->v_display_width=pIsomGlob->v_lcd_size_x;//be resize
		pIsomGlob->v_display_height=(pIsomGlob->v_lcd_size_x*3)>>2;//width : height= 4 : 3
#if (defined MPEG4_SUPPORT || defined H263_SUPPORT)
		pZoom->zoom_width=pIsomGlob->v_display_width;
		pZoom->zoom_height=pIsomGlob->v_display_height;
		pZoom->zoom_mode=0;
		pZoom->rotate_mode=0;
#endif
		pIsomGlob->v_cutX=0;
		pIsomGlob->v_cutY=0;
		pIsomGlob->v_cutW=pIsomGlob->v_display_width;
		pIsomGlob->v_cutH=pIsomGlob->v_display_height;
		
#ifdef USE_VOC_ROTATE
		pIsomGlob->v_rotate=Rotate_with_0;
#endif

#if (CSW_EXTENDED_API_LCD == 1)
		lcdDraw.fb.width = pIsomGlob->v_display_width;
        	lcdDraw.fb.height = pIsomGlob->v_display_height;
        	lcdDraw.fb.colorFormat = LCDD_COLOR_FORMAT_RGB_565;
        	lcdDraw.roi.x =0;
        	lcdDraw.roi.y =0;
        	lcdDraw.roi.width =pIsomGlob->v_display_width;
        	lcdDraw.roi.height =pIsomGlob->v_display_height;

#if(CHIP_HAS_GOUDA == 1)
            Vid_Video.alpha = 255;
            Vid_Video.cKeyEn = FALSE;
            Vid_Video.cKeyColor = 0;
            Vid_Video.cKeyMask = HAL_GOUDA_CKEY_MASK_OFF;
            Vid_Video.depth = HAL_GOUDA_VID_LAYER_OVER_ALL;
            Vid_Video.fmt = HAL_GOUDA_IMG_FORMAT_IYUV;

            Vid_Video.width = (short)pIsomGlob->v_width;
            Vid_Video.height = (short)pIsomGlob->v_height;
#endif
#endif
		
	}
	else
	{
		diag_printf("[VID] ##WARNING## image size unsupport! \n");
		res=ISOM_ERR_VIDEO_UNSUPPORT_IMAGE_SIZE;
		goto open_error;
	}

	if(pIsomGlob->v_width*pIsomGlob->v_height> LIMIT_WIDTH_176*LIMIT_HEIGHT_144 )
	{
		if((pIsomGlob->v_fps>>V_FPS_SCALE_BIT)>20)
			MAX_KEEP_SKIP_F_NUM=3;
		else if((pIsomGlob->v_fps>>V_FPS_SCALE_BIT)>15)
			MAX_KEEP_SKIP_F_NUM=2;
		else
			MAX_KEEP_SKIP_F_NUM=1;
	}
	else
	{
		MAX_KEEP_SKIP_F_NUM=1;
	}


	if(pIsomGlob->v_type==V_TYPE_MPEG4)
		pIsomGlob->v_SampleBufLen=0;// reuse Y buffer in decoder and v_ImageBuf(once).
	else
		pIsomGlob->v_SampleBufLen=MAX_V_SAMPLEBUF_SIZE;
#if 0
	if((pIsomGlob->v_fps>>V_FPS_SCALE_BIT)>LIMIT_FPS_15)
	{
		diag_printf("[VID] ##WARNING## fps=%d unsupport! \n", pIsomGlob->v_fps>>V_FPS_SCALE_BIT);
		res=ISOM_ERR_VIDEO_UNSUPPORT_FPS;
		goto open_error;
	}
#endif
	
	diag_printf("[VID] total_time=%d \n",pIsomGlob->total_time);
	diag_printf("[VID] v_type=%x \n",pIsomGlob->v_type);
	diag_printf("[VID] v_trackID=%d \n",pIsomGlob->v_trackID);
	diag_printf("[VID] v_total_sample_num=%d \n",pIsomGlob->v_total_sample_num);
	diag_printf("[VID] v_total_frame_num=%d \n",pIsomGlob->v_total_frame_num);
	diag_printf("[VID] v_total_keyframe_num=%d \n",pIsomGlob->v_total_keyframe_num);
	diag_printf("[VID] v_fps=%d \n",pIsomGlob->v_fps>>V_FPS_SCALE_BIT);
	diag_printf("[VID] v_period=%d \n",pIsomGlob->v_period);
	diag_printf("[VID] v_timerPeriod=%d \n",pIsomGlob->v_timerPeriod);
	diag_printf("[VID] DTS_FLG=%d \n", DTS_FLG);
	diag_printf("[VID] v_timeScale=%d \n",pIsomGlob->v_timeScale);
	diag_printf("[VID] v_width=%d \n",pIsomGlob->v_width);
	diag_printf("[VID] v_height=%d \n",pIsomGlob->v_height);
	diag_printf("[VID] v_display_width=%d \n",pIsomGlob->v_display_width);
	diag_printf("[VID] v_display_height=%d \n",pIsomGlob->v_display_height);
	diag_printf("[VID] v_lcd_size_x=%d \n",pIsomGlob->v_lcd_size_x);
	diag_printf("[VID] v_lcd_size_y=%d \n",pIsomGlob->v_lcd_size_y);

	diag_printf("[VID] a_type=%x \n",pIsomGlob->a_type);
	diag_printf("[VID] a_trackID=%d \n",pIsomGlob->a_trackID);
	diag_printf("[VID] a_total_sample_num=%d \n",pIsomGlob->a_total_sample_num);
	diag_printf("[VID] a_total_frame_num=%d \n",pIsomGlob->a_total_frame_num);
	diag_printf("[VID] a_bitrate=%d \n",pIsomGlob->a_bitrate);
	diag_printf("[VID] a_sample_rate=%d \n",pIsomGlob->a_sample_rate);
	diag_printf("[VID] a_frame_Period=%d \n",pIsomGlob->a_frame_Period);
	diag_printf("[VID] a_bit=%d \n",pIsomGlob->a_bit);
	diag_printf("[VID] a_channels=%d \n",pIsomGlob->a_channels);
	diag_printf("[VID] a_frame_sample=%d \n",pIsomGlob->a_frame_sample);
	
	//malloc
	
	//initResample(pIsomGlob->v_lcd_size_x);//for resample, some row memory.
	//InitColorDither();//for yuv2rgb lookup table

	if(pIsomGlob->v_type == V_TYPE_MPEG4 || pIsomGlob->v_type == V_TYPE_RV)//only mpeg4 for qvga.(h263 have not)
	{
		pIsomGlob->v_SampleBuf=0;// reuse Y buffer in decoder and v_ImageBuf(once).
	}
	else
	{
		if(pIsomGlob->v_SampleBufLen >mmc_MemGetFree()-32)
		{
			res=ISOM_ERR_MEM_INSUFFICIENT;
			goto open_error;
		}
		pIsomGlob->v_SampleBuf=(unsigned char*)mmc_MemMalloc(pIsomGlob->v_SampleBufLen);
		if(pIsomGlob->v_SampleBuf)
		{
			memset(pIsomGlob->v_SampleBuf, 0, pIsomGlob->v_SampleBufLen);
		}
		else
		{
			res=ISOM_ERR_MALLOC_FAIL;
			goto open_error;
		}
	}
	

	if(pIsomGlob->a_SampBufPipeLen+(pIsomGlob->a_SampBufMirrorLen<<1)>mmc_MemGetFree()-32)
	{
		res=ISOM_ERR_MEM_INSUFFICIENT;
		goto open_error;
	}
	pIsomGlob->a_SampBuf=(uint8*)mmc_MemMalloc(pIsomGlob->a_SampBufPipeLen+(pIsomGlob->a_SampBufMirrorLen<<1));
	if(pIsomGlob->a_SampBuf)
	{
		memset(pIsomGlob->a_SampBuf, 0, pIsomGlob->a_SampBufPipeLen+(pIsomGlob->a_SampBufMirrorLen<<1));
		pIsomGlob->a_SampStart=pIsomGlob->a_SampEnd=pIsomGlob->a_SampBuf+pIsomGlob->a_SampBufMirrorLen;
	}
	else
	{
		res=ISOM_ERR_MALLOC_FAIL;
		goto open_error;
	}

	if(pIsomGlob->a_SampBufMirrorLen>mmc_MemGetFree()-32)
	{
		res=ISOM_ERR_MEM_INSUFFICIENT;
		goto open_error;
	}
	pIsomGlob->a_SampTmpBuf=(uint8*)mmc_MemMalloc(pIsomGlob->a_SampBufMirrorLen);
	if(pIsomGlob->a_SampTmpBuf)
	{
		memset(pIsomGlob->a_SampTmpBuf, 0, pIsomGlob->a_SampBufMirrorLen);
		
		pIsomGlob->a_SampTmpBufp=pIsomGlob->a_SampTmpBuf;
	}
	else
	{
		res=ISOM_ERR_MALLOC_FAIL;
		goto open_error;
	}

	//one more pIsomGlob->a_frameBytes for avoid overflow
	if(pIsomGlob->a_frameBytes*(pIsomGlob->a_HalfPcmBufFrame<<1)+pIsomGlob->a_frameBytes>mmc_MemGetFree()-32)
	{
		res=ISOM_ERR_MEM_INSUFFICIENT;
		goto open_error;
	}
	//malloc enough memory avoid the fake parameters
	if(pIsomGlob->a_channels==1)
	{
		if(pIsomGlob->a_type==A_TYPE_MP3 && pIsomGlob->a_frame_sample==MP3_FRAME_SAMPLE_576)
		{
			res=(pIsomGlob->a_frameBytes<<2)*pIsomGlob->a_HalfPcmBufFrame;
			pIsomGlob->a_PcmBuf=(uint8 *)mmc_MemMalloc(res<<1);
			pIsomGlob->a_ExtraPcmBuf=(uint8 *)mmc_MemMalloc(res*EXTRA_PCM_BUF_NUM);
		}
		else
		{
			res=(pIsomGlob->a_frameBytes<<1)*(pIsomGlob->a_HalfPcmBufFrame);
			pIsomGlob->a_PcmBuf=(uint8 *)mmc_MemMalloc(res<<1);
			pIsomGlob->a_ExtraPcmBuf=(uint8 *)mmc_MemMalloc(res*EXTRA_PCM_BUF_NUM);
		}
	}
	else if(pIsomGlob->a_channels==2)
	{
		if(pIsomGlob->a_type==A_TYPE_MP3 && pIsomGlob->a_frame_sample==MP3_FRAME_SAMPLE_576)
		{
			res=(pIsomGlob->a_frameBytes<<1)*(pIsomGlob->a_HalfPcmBufFrame);
			pIsomGlob->a_PcmBuf=(uint8 *)mmc_MemMalloc(res<<1);
			pIsomGlob->a_ExtraPcmBuf=(uint8 *)mmc_MemMalloc(res*EXTRA_PCM_BUF_NUM);
		}
		else
		{
			res=pIsomGlob->a_frameBytes*(pIsomGlob->a_HalfPcmBufFrame);
			pIsomGlob->a_PcmBuf=(uint8 *)mmc_MemMalloc(res<<1);
			pIsomGlob->a_ExtraPcmBuf=(uint8 *)mmc_MemMalloc(res*EXTRA_PCM_BUF_NUM);
		}
	}

	//diag_printf("pIsomGlob->a_PcmBuf=%x %x res=%d",pIsomGlob->a_PcmBuf,pIsomGlob->a_ExtraPcmBuf, res);

	if(pIsomGlob->a_PcmBuf && pIsomGlob->a_ExtraPcmBuf)
	{
		memset(pIsomGlob->a_PcmBuf, 0, pIsomGlob->a_frameBytes*(pIsomGlob->a_HalfPcmBufFrame<<1));
		pIsomGlob->a_PcmBufHalfPos=pIsomGlob->a_PcmBuf+pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame;
	}
	else
	{
		res=ISOM_ERR_MALLOC_FAIL;
		goto open_error;
	}

	//init extra buf
	for(i=0;i<EXTRA_PCM_BUF_NUM;i++)
	{
		aud_extra_pcmbuf_state[i].done=0;
		aud_extra_pcmbuf_state[i].pBufPos=pIsomGlob->a_ExtraPcmBuf+res*i;
		diag_printf("pBufPos=%x ",aud_extra_pcmbuf_state[i].pBufPos);
	}
	A_EXBF_RD_ID=0;
	A_EXBF_WT_ID=0;

#if(CHIP_HAS_GOUDA != 1)
	if(pIsomGlob->v_lcd_size_x*pIsomGlob->v_lcd_size_y*2>mmc_MemGetFree()-32)
	{
		res=ISOM_ERR_MEM_INSUFFICIENT;
		goto open_error;
	}
	pIsomGlob->v_ImageBuf=(uint16*)mmc_MemMalloc(pIsomGlob->v_lcd_size_x*pIsomGlob->v_lcd_size_y*2);
	if(pIsomGlob->v_ImageBuf)
	{
		memset(pIsomGlob->v_ImageBuf, 0, pIsomGlob->v_lcd_size_x*pIsomGlob->v_lcd_size_y*2);
		pIsomGlob->v_ImageBuf_HalfFlag=0;
	}
	else
	{
		res=ISOM_ERR_MALLOC_FAIL;
		goto open_error;
	}

       //reuse lcd buffer for v_ImageBuf2.
       if(pIsomGlob->v_display_width==pIsomGlob->v_lcd_size_x)//voc directly output to lcd buffer
		NEED_BLEND_IMG=0;
	else//voc ouput to v_ImageBuf then copy to lcd buffer.
		NEED_BLEND_IMG=1;
	pIsomGlob->v_ImageBuf2=(unsigned short*)get_lcd_frame_buffer_address();
	/*
	if(pIsomGlob->v_lcd_size_x*pIsomGlob->v_lcd_size_y*2>mmc_MemGetFree()-32)
	{
		res=ISOM_ERR_MEM_INSUFFICIENT;
		goto open_error;
	}
	pIsomGlob->v_ImageBuf2=(uint16*)mmc_MemMalloc(pIsomGlob->v_lcd_size_x*pIsomGlob->v_lcd_size_y*2);
	if(pIsomGlob->v_ImageBuf2)
	{
		memset(pIsomGlob->v_ImageBuf2, 0, pIsomGlob->v_lcd_size_x*pIsomGlob->v_lcd_size_y*2);
		pIsomGlob->v_ImageBuf_HalfFlag=0;
	}
	else
	{
		res=ISOM_ERR_MALLOC_FAIL;
		goto open_error;
	}
	*/
#else
/*
        pIsomGlob->v_YuvInfo2[0]=0;
        pIsomGlob->v_YuvInfo2[1]=0;
        pIsomGlob->v_YuvInfo2[2]=0;
        pIsomGlob->v_YuvInfo2[3]=0;
        pIsomGlob->v_YuvInfo2[4]=0;
        */
#endif

	if(pIsomGlob->v_type==V_TYPE_MJPEG)
	{
		pIsomGlob->v_YuvInfo[0]=0;
		pIsomGlob->v_YuvInfo[1]=0;
		pIsomGlob->v_YuvInfo[2]=0;
		pIsomGlob->v_YuvInfo[3]=0;
		pIsomGlob->v_YuvInfo[4]=0;
		if(((pIsomGlob->v_width*pIsomGlob->v_height*3)>>1)>mmc_MemGetFree()-32)//for 2 yuv buffer
		{
			res=ISOM_ERR_MEM_INSUFFICIENT;
			goto open_error;
		}
		
		pIsomGlob->v_YuvInfo[0]=(uint32)mmc_MemMalloc((pIsomGlob->v_width*pIsomGlob->v_height*3)>>1);
		if(pIsomGlob->v_YuvInfo[0])
		{
			memset((uint8*)pIsomGlob->v_YuvInfo[0], 0, (pIsomGlob->v_width*pIsomGlob->v_height*3)>>1);
			pIsomGlob->v_YuvInfo[1]=pIsomGlob->v_YuvInfo[0]+pIsomGlob->v_width*pIsomGlob->v_height;
			pIsomGlob->v_YuvInfo[2]=pIsomGlob->v_YuvInfo[1]+((pIsomGlob->v_width*pIsomGlob->v_height)>>2);
		}
		else
		{
			res=ISOM_ERR_MALLOC_FAIL;
			goto open_error;
		}
	}
	
	/*open voc*/
	VOC_OPEN=0;
	VOC_VID=0;
	VOC_AUD=0;
	VOC_WORK=VOC_WORK_NON;
	VOC_SETTING=0;

	//get current voc frequency and set to 78M.
	VOC_FREQ_STATE=hwp_sysCtrl->Cfg_Clk_Voc;
	if(pIsomGlob->v_type == V_TYPE_RV)
	{
		hwp_sysCtrl->Cfg_Clk_Voc = SYS_CTRL_VOC_SEL_PLL_PLL | (4<<0);//SYS_CTRL_VOC_FREQ_78M=(4<<0)
		diag_printf("[VID] set voc 78M!%x %x", &(hwp_sysCtrl->Cfg_Clk_Voc), hwp_sysCtrl->Cfg_Clk_Voc);
#ifdef VID_XCPU_FREQ_SET		
		hal_SysRequestFreq(HAL_SYS_FREQ_CAMS, HAL_SYS_FREQ_156M, NULL);//reuse cam id
		XCPU_FREQ_STATE=HAL_SYS_FREQ_156M;
		diag_printf("[VID] set XCPU 156M!");
#endif
	}
	
#if(CHIP_HAS_GOUDA == 1)
	if(pIsomGlob->a_type != A_TYPE_AMR)
	{
#endif
		if((res=vpp_AudioJpegDecOpen(vid_voc_isr))!=HAL_ERR_NO)// will make a ISR.
		{
			diag_printf("[VID]##WARNING## vpp_AudioJpegDecOpen err res=%d \n" ,res);
			res=ISOM_ERR_VOC_OPEN_FAIL;
			goto open_error;
		}
		
		vocDecAud.reset=1;
		vocDecAud.mode=pIsomGlob->a_vocDecType;	
		vocDecAud.EQ_Type=-1;
		vocDecAud.BsbcEnable = 0;

#if(CHIP_HAS_GOUDA == 1)	
		G_VidSBCConstY = (INT32*)mmc_MemMalloc(468*4);
		if( G_VidSBCConstY==NULL)
		{
				mmc_MemFreeAll();
				return ISOM_ERR_MALLOC_FAIL;		
		}
#endif
	 	if( is_SSHdl_valid(my_ss_handle)&& (my_audio_config_handle->codec_type == 1))      //BT_A2DP_sbc
		 {
#if(CHIP_HAS_GOUDA == 1)	
	            vocDecAud.BsbcEnable = 1;
#endif
	 	}
		
#if(CHIP_HAS_GOUDA == 1)
#ifdef RM_SUPPORT
		if(pIsomGlob->v_type==V_TYPE_RV)
			vpp_AudioJpegSetReuseCode(1);//rmvb
#endif
#endif
		
#if(CHIP_HAS_GOUDA == 1)
	}
	else
	{
		VPP_SPEECH_AUDIO_CFG_T vppCfg;
		HAL_SPEECH_DEC_IN_T *pVocDecAmr=NULL;

		if ((res = vpp_SpeechOpen(vid_voc_isr, VPP_SPEECH_WAKEUP_SW_DEC)) == HAL_ERR_RESOURCE_BUSY)
		{
			diag_printf("[VID]##WARNING## vpp_SpeechOpen err res=%d \n" ,res);
			res=ISOM_ERR_VOC_OPEN_FAIL;
			goto open_error;
		}
		
		// VPP audio config
		vppCfg.echoEsOn = 0;
		vppCfg.echoEsVad = 0;
		vppCfg.echoEsDtd = 0;
		vppCfg.echoExpRel = 0;
		vppCfg.echoExpMu = 0;
		vppCfg.echoExpMin = 0;
		vppCfg.encMute = VPP_SPEECH_MUTE;
		vppCfg.decMute = VPP_SPEECH_UNMUTE;
		vppCfg.sdf = NULL;
		vppCfg.mdf = NULL;
		vppCfg.if1 = 1;// amr ring full mode must set 1.

		vpp_SpeechAudioCfg(&vppCfg);
        	vpp_SpeechSetReceiverDigitalGain(0);	
		pVocDecAmr=vpp_SpeechGetRxCodBuffer();
		if(pVocDecAmr==NULL)
		{
			diag_printf("[VID]##ERROR## vpp_SpeechGetRxCodBuffer NULL!\n");
			res=ISOM_ERR_VOC_OPEN_FAIL;
			goto open_error;
		}

		pVocDecAmr->dtxOn = 0;
		pVocDecAmr->codecMode= HAL_INVALID;
		pVocDecAmr->decFrameType = 0;
		pVocDecAmr->bfi = 0;
		pVocDecAmr->sid = 0;
		pVocDecAmr->taf = 0;
		pVocDecAmr->ufi = 0;
		pVocDecAmr->reserved= 0;
		memset(pVocDecAmr->decInBuf, 0, HAL_SPEECH_FRAME_SIZE_COD_BUF);
	}
#endif
	
#if(CHIP_HAS_GOUDA != 1)	
	vocDecVid.reset=1;
	vocDecVid.mode=MMC_H263_ZOOM;
	vocDecVid.BsbcEnable = 0;
	vocDecVid.Cut_width=(short)pIsomGlob->v_width; 
	vocDecVid.Cut_height=(short)pIsomGlob->v_height;
	vocDecVid.Zoomed_width=pIsomGlob->v_display_width;
	vocDecVid.Zoomed_height=pIsomGlob->v_display_height;
#ifdef USE_VOC_ROTATE
	vocDecVid.zoom_mode=pIsomGlob->v_rotate;
#endif
#endif
	VOC_OPEN=1;

	fillAudioData=NULL;//not read audio in video decoder.
	
	switch(pIsomGlob->v_type)
	{
#ifdef MPEG4_SUPPORT
		case V_TYPE_MPEG4:
#ifdef USE_OVERLAY
			VID_OVERLAY_ID=HAL_OVERLAY_INT_SRAM_ID_1;
			res=hal_OverlayLoad(VID_OVERLAY_ID);
			if(res!=HAL_ERR_NO)
				diag_printf( "[VID]MPEG4 HAL_OVERLAY_INT_SRAM_ID_1 hal_OverlayLoad  res=%d",res);
#endif

			res = dec_init(0, 0);
			
			if (res) {
				res=ISOM_ERR_VIDEO_INIT_FAIL;
				goto open_error;
			}

			if(pIsomGlob->v_decSpeInfo || pIsomGlob->med_type ==MCI_TYPE_MJPG)
			{
#if (CHIP_HAS_GOUDA != 1)
				pIsomGlob->v_SampleBuf=(unsigned char*)pIsomGlob->v_ImageBuf;//reuse v_ImageBuf for file read only this place.
				pIsomGlob->v_SampleBufLen=pIsomGlob->v_lcd_size_x*pIsomGlob->v_lcd_size_y*2;
#else
				pIsomGlob->v_SampleBuf=(unsigned char*)pIsomGlob->a_SampBuf;//reuse a_SampBuf for file read only this place.
				pIsomGlob->v_SampleBufLen=(pIsomGlob->a_SampBufPipeLen+(pIsomGlob->a_SampBufMirrorLen<<1));
#endif
				
				if(pIsomGlob->med_type !=MCI_TYPE_MJPG)
				{
					if(pIsomGlob->v_decSpeInfo->dataLength<=pIsomGlob->v_SampleBufLen)
					{
						memcpy(pIsomGlob->v_SampleBuf, pIsomGlob->v_decSpeInfo->data, pIsomGlob->v_decSpeInfo->dataLength);
						pIsomGlob->v_SampleBuf_BytesRemain+=pIsomGlob->v_decSpeInfo->dataLength;
					}
					else
					{
						diag_printf( "[VID]MPEG4 header=%d > buflen=%d ERR!",pIsomGlob->v_decSpeInfo->dataLength,
							pIsomGlob->v_SampleBufLen);	
						res=ISOM_ERR_MEM_INSUFFICIENT;
						goto open_error;
					}
				}
				else//mpeg4 in AVI
				{
					int32 key_frame;
					pIsomGlob->v_current_sample_num=0;
					pIsomGlob->v_SampleBuf_BytesRemain=0;
					pIsomGlob->v_SampleBuf_BytesUsed=0;
					do
					{
						res=avi_set_video_position(pIsomGlob->avi_p, pIsomGlob->v_current_sample_num);
						if(res!=0)
						{
							diag_printf("[VID] avi_set_video_position err!");
							res= ISOM_ERR_VIDEO_READ_FAIL;
							goto open_error;
						}
						res=avi_read_frame(pIsomGlob->avi_p, pIsomGlob->v_SampleBuf, pIsomGlob->v_SampleBufLen,  &key_frame);
						if( res < 0 )
						{
							diag_printf( "[VID]MJPG avi video frame read err res=%d \n", res ); 
							res= ISOM_ERR_VIDEO_READ_FAIL;
							goto open_error;
						}
						if(res==0)
							pIsomGlob->v_current_sample_num+=1;
						pIsomGlob->v_SampleBuf_BytesRemain+=res;
					}while(res==0 && pIsomGlob->v_current_sample_num < pIsomGlob->v_total_sample_num);
				}

				if(pIsomGlob->v_SampleBuf_BytesRemain>0)
				{
					int32 used_bytes;
					xvid_dec_stats_t xvid_dec_stats;
					used_bytes = dec_main(pIsomGlob->v_SampleBuf+pIsomGlob->v_SampleBuf_BytesUsed,   
										(uint8*)pIsomGlob->v_YuvInfo, 
										pIsomGlob->v_SampleBuf_BytesRemain, &xvid_dec_stats, pZoom,
										((pIsomGlob->med_type ==MCI_TYPE_MJPG) ? TRUE : FALSE));
					diag_printf("[VID] dec_main head used_bytes=%d xvid_dec_stats.type=%d\n",used_bytes,xvid_dec_stats.type);
					if(used_bytes == -1)//profile unfit
					{
						res= ISOM_ERR_VIDEO_DEC_FAIL;
						goto open_error;
					}
					/* Resize image buffer if needed */
					if(xvid_dec_stats.type == XVID_TYPE_VOL) {
						/* Check if old buffer is smaller */
						if(pIsomGlob->v_width*pIsomGlob->v_height>= xvid_dec_stats.data.vol.width*xvid_dec_stats.data.vol.height)
						{
							if(pIsomGlob->v_width!=xvid_dec_stats.data.vol.width)
							{
								pIsomGlob->v_width=xvid_dec_stats.data.vol.width;
								pIsomGlob->v_height=xvid_dec_stats.data.vol.height;
#if(CHIP_HAS_GOUDA != 1)								
								vocDecVid.Cut_width=(short)pIsomGlob->v_width; 
								vocDecVid.Cut_height=(short)pIsomGlob->v_height;
#endif								
								diag_printf("[VID] ##WARNING## real decode width=%d height=%d \n",xvid_dec_stats.data.vol.width, xvid_dec_stats.data.vol.height);
							}
						}
						else
						{
							diag_printf("[VID] ##WARNING## real decode width=%d height=%d \n",xvid_dec_stats.data.vol.width, xvid_dec_stats.data.vol.height);
							res= ISOM_ERR_PARAM_ERROR;
							goto open_error;
						}
					}

					/* Update buffer pointers */
					if(used_bytes > 0) 
					{
						pIsomGlob->v_SampleBuf_BytesUsed=pIsomGlob->v_SampleBuf_BytesRemain;
					}

				}

				pIsomGlob->v_SampleBuf=dec_get_cur_y_end();//reuse for next frame data read.
				if(!pIsomGlob->v_SampleBuf)
				{
					diag_printf("[VID] dec_get_cur_y_end ERR!\n");
					res= ISOM_ERR_VIDEO_DEC_FAIL;
					goto open_error;
				}
				pIsomGlob->v_SampleBufLen=pIsomGlob->v_width*pIsomGlob->v_height;
			}
			
			break;
#endif

#ifdef H263_SUPPORT
		case V_TYPE_H263:
		{
			uint32 readLen, gotbytes, gotcount;
			int32 gob;

#ifdef USE_OVERLAY
			VID_OVERLAY_ID=HAL_OVERLAY_INT_SRAM_ID_0;
			res=hal_OverlayLoad(VID_OVERLAY_ID);
			if(res!=HAL_ERR_NO)
				diag_printf( "[VID]H263 HAL_OVERLAY_INT_SRAM_ID_1 hal_OverlayLoad  res=%d",res);
#endif

			//init h.263
			quiet = 0;
			errorstate=0;
			initbits(pIsomGlob->v_SampleBuf);
			if(errorstate)
			{
				diag_printf("[VID] h.263 initbit err! \n");
				res=ISOM_ERR_MALLOC_FAIL;
				goto open_error;
			}

			initoptions ();
			temp_ref = 0;
			prev_non_disposable_temp_ref =0;
			next_non_disposable_temp_ref = 0;

			coded_picture_width=pIsomGlob->v_width;
			coded_picture_height=pIsomGlob->v_height;
			
			pIsomGlob->v_SampleBuf_BytesUsed=0;
			
			if((res = stbl_GetSampleSize(pIsomGlob->v_info , 1, &readLen))!=GF_OK)
			{
				diag_printf("[VID] video stbl_GetSampleSize err res=%d v_current_sample_num=%d \n",res, pIsomGlob->v_current_sample_num);
				res=ISOM_ERR_VIDEO_READ_FAIL;
				goto open_error;
			}
			
			if(pIsomGlob->v_SampleBuf_BytesRemain+readLen<pIsomGlob->v_SampleBufLen)
			{
				res=gf_isom_get_multSample(pIsomGlob->v_info, 
											pIsomGlob->v_current_sample_num+1, 
											pIsomGlob->v_SampleBuf+pIsomGlob->v_SampleBuf_BytesRemain, 
											readLen, 	&gotbytes, &gotcount);

				if(res || (gotbytes<=0))
				{
					diag_printf("[VID] video gf_isom_get_sample err res=%d \n",res);
					res=ISOM_ERR_VIDEO_READ_FAIL;
					goto open_error;
				}
			}
			else
			{
				res=ISOM_ERR_MEM_INSUFFICIENT;
				goto open_error;
			}
			
		
			resetbits (pIsomGlob->v_SampleBuf+pIsomGlob->v_SampleBuf_BytesUsed, readLen);
			
			 gob=getheader ();
			if(errorstate)
			{
				diag_printf("[VID] h.263 getheader err gob=%d \n",gob);
				res=ISOM_ERR_INVALID_FORMAT;
				goto open_error;
			}
			
			if (!gob )
			{
				diag_printf("[VID] h.263 getheader err ! \n");
				res=ISOM_ERR_INVALID_FORMAT;
				goto open_error;
			}
			
			base_source_format = source_format;
			initdecoder ();
			if(errorstate)
			{
				diag_printf("[VID] h.263 initdecoder err ! \n");
				res=ISOM_ERR_MALLOC_FAIL;
				goto open_error;
			}
				
			pIsomGlob->v_SampleBuf_BytesUsed=0;
			pIsomGlob->v_SampleBuf_BytesRemain=0;

			break;
		}
#endif
		case V_TYPE_MJPEG:
		{
		
			break;
		}
		case V_TYPE_RV:
		{
			break;
		}
		default:
			diag_printf("[VID] err! pIsomGlob->v_type=%x \n", pIsomGlob->v_type);
			res=ISOM_ERR_INVALID_FORMAT;
			goto open_error;
	}

	mmc_MemCheck(TRUE);
	
	pIsomGlob->a_audio_path=audio_path;
	pIsomGlob->open_audio=open_audio;
	pIsomGlob->bSkipFrame = FALSE;
	
	pIsomGlob->v_current_frame_num= -1;//0=the first frame
	AV_DEC_STATE=0;
	DISPLAY_STATE=0;
	AUD_DEC_MIN=2;//default
	VID_ERR_FRAME_REC=0;
	VID_SET_MODE=0;
	AUD_DATA_REQ=0;
	
	play_state=VID_PLAY_STATE_IDLE;
	diag_printf("[VID] mmc_vid_isom_open_file ok!\n",res);
	return ISOM_ERR_OK;

open_error:
	diag_printf("[VID] ##WARNING## mmc_vid_isom_open_file err res=%d\n",res);
	if(VOC_OPEN)
	{
#if(CHIP_HAS_GOUDA == 1)
		if(pIsomGlob->a_type != A_TYPE_AMR)
		{
#endif
			vpp_AudioJpegDecClose();
#if(CHIP_HAS_GOUDA == 1)
		}
		else
		{
			vpp_SpeechClose();
		}
#endif
		VOC_OPEN=0;
	}

	if(pIsomGlob)
	{
		if(pIsomGlob->med_type==MCI_TYPE_MP4 || pIsomGlob->med_type==MCI_TYPE_3GP)
		{
#ifdef MPEG4_SUPPORT
			if(pIsomGlob->v_type==V_TYPE_MPEG4)
				dec_stop();
#endif
			gf_isom_close_file(pIsomGlob->file);
		}
#ifdef RM_SUPPORT		
		else if(pIsomGlob->med_type==MCI_TYPE_RM)
		{
			rm_destroy(0);
			if(pIsomGlob->a_type==A_TYPE_COOK && pIsomGlob->a_Gecko2DecHdl)
			{
				Gecko2FreeDecoder(pIsomGlob->a_Gecko2DecHdl);
			}
				
		}
#endif		
		else
		{
		#ifdef MPEG4_SUPPORT	
			if( pIsomGlob->v_type == V_TYPE_MPEG4 )
			{
				dec_stop();
			}
		#endif	

			if(pIsomGlob->avi_p)
				avi_close_input_file(pIsomGlob->avi_p);
		}
	}
	mmc_MemFreeAll();
	return res;
}

int32 mmc_vid_isom_play (uint16 lcd_start_x,  uint16 lcd_start_y)
{
    int i,j;
    int32 res;
    unsigned long long temp;

#if(CHIP_HAS_GOUDA == 1)	
     SSHdl *my_ss_handle = MpegPlayer.MpegInput.ss_handle;
     bt_a2dp_audio_cap_struct * my_audio_config_handle=   MpegPlayer.MpegInput.audio_config_handle ;
     bt_a2dp_sbc_codec_cap_struct *sbc_config_handle =  &my_audio_config_handle->codec_cap.sbc;
#endif
    if(play_state!=VID_PLAY_STATE_IDLE)
    {
        diag_printf("[VID] mmc_vid_isom_play err play_state= %d \n",play_state);
        res= ISOM_ERR_PLAY_STATE_FAIL;
        goto play_error;
    }

    diag_printf("[VID] v_lcd_start_x=%d \n",pIsomGlob->v_lcd_start_x);
    diag_printf("[VID] v_lcd_start_y=%d \n",pIsomGlob->v_lcd_start_y);

    //for audio
    if(!pIsomGlob->v_fps)
    {
        diag_printf("[VID] err pIsomGlob->v_fps=%d !!!\n",pIsomGlob->v_fps);
        res=ISOM_ERR_PARAM_ERROR;
        goto play_error;		
    }
	
    if(pIsomGlob->a_type== A_TYPE_AAC)
        temp=((((unsigned long long)pIsomGlob->v_current_frame_num+1)*(unsigned long long)pIsomGlob->a_sample_rate)<<V_FPS_SCALE_BIT)/
                    ((unsigned long long)pIsomGlob->v_fps*AAC_FRAME_SAMPLE);
    else if(pIsomGlob->a_type== A_TYPE_AMR)
        temp=((((unsigned long long)pIsomGlob->v_current_frame_num+1)*(unsigned long long)pIsomGlob->a_sample_rate)<<V_FPS_SCALE_BIT)/
                    ((unsigned long long)pIsomGlob->v_fps*AMR_FRAME_SAMPLE);
    else if(pIsomGlob->a_type== A_TYPE_MP3)
    {
        if(pIsomGlob->a_sample_rate>=32000)
            temp=((((unsigned long long)pIsomGlob->v_current_frame_num+1)*(unsigned long long)pIsomGlob->a_sample_rate)<<V_FPS_SCALE_BIT)/
                        ((unsigned long long)pIsomGlob->v_fps*MP3_FRAME_SAMPLE_1152);
        else 
            temp=((((unsigned long long)pIsomGlob->v_current_frame_num+1)*(unsigned long long)pIsomGlob->a_sample_rate)<<V_FPS_SCALE_BIT)/
                        ((unsigned long long)pIsomGlob->v_fps*MP3_FRAME_SAMPLE_576);
    }
    else if(pIsomGlob->a_type== A_TYPE_PCM)
        temp=((((unsigned long long)pIsomGlob->v_current_frame_num+1)*(unsigned long long)pIsomGlob->a_sample_rate)<<V_FPS_SCALE_BIT)/
                    ((unsigned long long)pIsomGlob->v_fps*PCM_FRAME_SAMPLE);
    else if(pIsomGlob->a_type== A_TYPE_ADPCM)
	 temp=0;
    else if(pIsomGlob->a_type== A_TYPE_COOK)
        temp=((((unsigned long long)pIsomGlob->v_current_frame_num+1)*(unsigned long long)pIsomGlob->a_sample_rate)<<V_FPS_SCALE_BIT)/
                    ((unsigned long long)pIsomGlob->v_fps*pIsomGlob->a_frame_sample);
    else if(pIsomGlob->a_type== A_TYPE_RA)
	 temp=0;
    else
    {
        diag_printf("[VID] err pIsomGlob->a_type=%d !!!\n",pIsomGlob->a_type);
        res=ISOM_ERR_UNSUPPORT;
        goto play_error;		
    }
    pIsomGlob->a_current_frame_num=(uint32)temp;
	
    if(!pIsomGlob->v_total_frame_num)
    {
        diag_printf("[VID] err pIsomGlob->v_total_frame_num=%d !!!\n",pIsomGlob->v_total_frame_num);
        res=ISOM_ERR_PARAM_ERROR;
        goto play_error;		
    }

    pIsomGlob->a_current_sample_num = (uint32)((unsigned long long)pIsomGlob->a_total_sample_num*(unsigned long long)(pIsomGlob->v_current_frame_num+1)/
                                                                    ((unsigned long long)pIsomGlob->v_total_frame_num));

    pIsomGlob->a_SampStart=pIsomGlob->a_SampEnd=pIsomGlob->a_SampBuf+pIsomGlob->a_SampBufMirrorLen;

	if(pIsomGlob->med_type==MCI_TYPE_MP4 || pIsomGlob->med_type==MCI_TYPE_3GP)
	{
		if(pIsomGlob->a_type== A_TYPE_AMR)
		{
#if(CHIP_HAS_GOUDA != 1)
			memcpy(pIsomGlob->a_SampTmpBuf, AMR_HEAD, AMR_HEAD_SIZE);
			pIsomGlob->a_SampTmpBufp=pIsomGlob->a_SampTmpBuf;
			pIsomGlob->a_SampTmpRemain=AMR_HEAD_SIZE;
#else
			pIsomGlob->a_SampTmpBufp=pIsomGlob->a_SampTmpBuf;
			pIsomGlob->a_SampTmpRemain=0;
#endif
		}
		else
		{
			pIsomGlob->a_SampTmpBufp=pIsomGlob->a_SampTmpBuf;
			pIsomGlob->a_SampTmpRemain=0;
		}

	}
#ifdef RM_SUPPORT
	else if(pIsomGlob->med_type==MCI_TYPE_RM)
	{
		pIsomGlob->a_SampTmpBufp=pIsomGlob->a_SampTmpBuf;
		pIsomGlob->a_SampTmpRemain=0;
		
		fillSampBuf();
		temp=(unsigned long long)rm_getAudioTimeStampStart();
		temp=temp*(unsigned long long)(pIsomGlob->a_sample_rate)/1000;
		pIsomGlob->a_current_frame_num=(unsigned int)temp;//reuse in rmvb for record pcm sample num.
		diag_printf("[VID]audio time line pcm sample=%d", pIsomGlob->a_current_frame_num);
	}
#endif
	else
	{
		if(pIsomGlob->a_type== A_TYPE_AMR && pIsomGlob->a_current_sample_num!=0)
		{
#if(CHIP_HAS_GOUDA != 1)
			memcpy(pIsomGlob->a_SampTmpBuf, AMR_HEAD, AMR_HEAD_SIZE);
			pIsomGlob->a_SampTmpBufp=pIsomGlob->a_SampTmpBuf;
			pIsomGlob->a_SampTmpRemain=AMR_HEAD_SIZE;
#else
			pIsomGlob->a_SampTmpBufp=pIsomGlob->a_SampTmpBuf;
			pIsomGlob->a_SampTmpRemain=0;
#endif
		}
		else
		{
			pIsomGlob->a_SampTmpBufp=pIsomGlob->a_SampTmpBuf;
			pIsomGlob->a_SampTmpRemain=0;
		}

		res=avi_set_audio_position2(pIsomGlob->avi_p, pIsomGlob->a_current_sample_num);
		if(res!=0)
		{
			diag_printf("[VID] avi_set_audio_position2 err! res=%d ", res);
			res=ISOM_ERR_AUDIO_READ_FAIL;
			goto play_error;		
		}

#if(CHIP_HAS_GOUDA == 1)
		if(pIsomGlob->a_type== A_TYPE_AMR && pIsomGlob->a_current_sample_num ==0)
		{
			fillSampBuf();
			//move amr header
			moveDataPos(AMR_HEAD_SIZE);
		}
#endif

	}

	vocDecAud.reset=1;
    //AUD_EXTRA_BUF_DONE=0;
    	//diag_printf("%x",aud_extra_pcmbuf_state[0].pBufPos);
	//diag_printf("%x",aud_extra_pcmbuf_state[1].pBufPos);

	A_EXBF_RD_ID=0;
	A_EXBF_WT_ID=0;
	for(i=0;i<EXTRA_PCM_BUF_NUM;i++)
	{
		aud_extra_pcmbuf_state[i].done=0;
	}

    VOC_AUD=0;
#ifdef RM_SUPPORT
	needRA=0;
#endif
	#if 1
	if(pIsomGlob->open_audio)
	{
		AUD_FADE_IN_FLAG=(pIsomGlob->a_HalfPcmBufFrame+2);//zero pcm num
		
		for(i=0;i<(2+EXTRA_PCM_BUF_NUM);i++)
	    {
	        fillSampBuf();

	        /*check AMR mode*/
	        if(pIsomGlob->a_type== A_TYPE_AMR && i==0)
	        {
	            uint8 *pH;	
	            pH=getDataPos();

#if(CHIP_HAS_GOUDA != 1)    
	            if(pH[6]!=0x3C && pH[36]!=0x3C)//AMR 122
	            {
	                pIsomGlob->open_audio=0;
	                diag_printf("[VID] WARNING! AMR head=(6)%x (36)%x (not 3C AMR122), unsupport now!!! Audio Turn off!",pH[6],pH[36]);
	                break;
	            }
#else
		   /*
	           get_amr_mode( pH, &(pIsomGlob->a_amr_mode), &(pIsomGlob->a_codeframeBytes));
		    if(pIsomGlob->a_amr_mode == HAL_INVALID)
			 	pIsomGlob->open_audio=0;
		   */
#endif
	        }
			
	        if(getDataLen()>0)
	        {
			//pPcmBufPos=(uint32)pIsomGlob->a_ExtraPcmBuf;
			if(i==0)
				pPcmBufPos=(uint32)pIsomGlob->a_PcmBuf;
			else if(i==1)
				pPcmBufPos=(uint32)pIsomGlob->a_PcmBufHalfPos;
			else
			{
				//aud_extra_pcmbuf_state[i-2].done=0;
				pPcmBufPos=(uint32)(aud_extra_pcmbuf_state[i-2].pBufPos);
			}

		
			if(pIsomGlob->a_type== A_TYPE_COOK)
			{
#ifdef RM_SUPPORT
				//int32 lostflag;
				//if(i==0)lostflag=1;
				//else lostflag=0;
				tick1=hal_TimGetUpTime();

				AUD_DEC_FRM_NUM=0;
				
				while(AUD_DEC_FRM_NUM < pIsomGlob->a_HalfPcmBufFrame)
				{
					if(AUD_FADE_IN_FLAG==0)
					{
						res=Gecko2Decode(pIsomGlob->a_Gecko2DecHdl, getDataPos(), 0, (short*)pPcmBufPos);
						if(res!=0)
						{
							diag_printf("[VID]AUD Gecko2Decode err! res=%d\n",res);
							res=ISOM_ERR_AUDIO_DEC_FAIL;
							goto play_error;
						}
					}
					else
					{
						//mute frame
						if(AUD_FADE_IN_FLAG>1)
							memset((uint8*)pPcmBufPos, 0, pIsomGlob->a_frameBytes);
						else
						{
							audFadeIn((int16*)pPcmBufPos, pIsomGlob->a_frame_sample*pIsomGlob->a_channels);
						}
						AUD_FADE_IN_FLAG--;
					}
		
					moveDataPos(pIsomGlob->a_codeframeBytes);//const code frame bytes.
					pPcmBufPos+=pIsomGlob->a_frameBytes;
					
					AUD_DEC_FRM_NUM++;
					//lostflag=0;
					
				}

				if(i>1)
					aud_extra_pcmbuf_state[i-2].done=1;
					
				AUD_DEC_FRM_NUM=0;
				
				tick2=hal_TimGetUpTime();
		              diag_printf( "[VID] aud cook dec time=%d frame num=%d", (tick2-tick1)*1000/16384, pIsomGlob->a_HalfPcmBufFrame);
#endif
			}
	            else if(pIsomGlob->a_type!= A_TYPE_PCM && pIsomGlob->a_type!= A_TYPE_ADPCM)
	            {
	            	  tick1=hal_TimGetUpTime();
	                AUD_DEC_FRM_NUM=0;
	                if(VOC_WORK==VOC_WORK_NON )
	                {
	                	
					
	                    VOC_WORK=VOC_WORK_AUD;
#if(CHIP_HAS_GOUDA == 1)
				if(pIsomGlob->a_type!= A_TYPE_AMR)
				{
#endif

		                    vocDecAud.inStreamBufAddr=(INT32 *)getDataPos();
		                    vocDecAud.outStreamBufAddr=(INT32 *)pPcmBufPos;
		                    vocDecAud.audioItf=audioItf;
							
#if(CHIP_HAS_GOUDA == 1)
					if( is_SSHdl_valid(my_ss_handle)&& (my_audio_config_handle->codec_type == 1))      //BT_A2DP_sbc
				 	{
				
				 		int mode =SBC_MODE_JOINT_STEREO;
			                 	vocDecAud.BsbcEnable = 1;
						
				      if( sbc_config_handle->channel_mode == 0x01 )
				         mode = SBC_MODE_JOINT_STEREO; // joint stereo
				      else if( sbc_config_handle->channel_mode == 0x02 )
				        mode = SBC_MODE_STEREO; // stereo
				      else if( sbc_config_handle->channel_mode == 0x04 )
				        mode = SBC_MODE_DUAL_CHANNEL; // dual
				      else if( sbc_config_handle->channel_mode == 0x08 )
				        mode= SBC_MODE_MONO; // mono
				      else
				          diag_printf("[VID] WARNING! ##WARNING##ErrorSBC channel mode!\n");
					  
					   vocDecAud.Source_width = mode;

				      if( sbc_config_handle->alloc_method  == 0x01 )
				           vocDecAud.Source_height= SBC_AM_LOUDNESS;// 
				      else if( sbc_config_handle->alloc_method  == 0x02 )
				           vocDecAud.Source_height= SBC_AM_SNR;//SBC_AM_LOUDNESS;//
				      else
				          diag_printf("[VID] WARNING! ##WARNING##ErrorSBC alloc_method!\n");
					   vocDecAud.Cut_width =    SBC_BIT_POOL;
					   vocDecAud.inStreamUBufAddr = (INT32*)G_VppSBCConstX;
					   vocDecAud.inStreamVBufAddr = G_VidSBCConstY;
					   
				 	}
#endif	

		       		vpp_AudioJpegDecScheduleOneFrame(&vocDecAud);
		           		vocDecAud.reset=0;
#if(CHIP_HAS_GOUDA != 1)					
			    vocDecVid.reset=0;
#endif					
#if(CHIP_HAS_GOUDA == 1)

				}
				else
				{
					HAL_SPEECH_DEC_IN_T *pVocDecAmr = vpp_SpeechGetRxCodBuffer();

					get_amr_mode( getDataPos(), &(pIsomGlob->a_amr_mode), &(pIsomGlob->a_codeframeBytes));

					if(i==0)
					{
						int32 dropNum=pIsomGlob->a_HalfPcmBufFrame;
						//make start with silence
						while(dropNum)
						{
							moveDataPos(pIsomGlob->a_codeframeBytes);//drop one frame
							get_amr_mode( getDataPos(), &(pIsomGlob->a_amr_mode), &(pIsomGlob->a_codeframeBytes));
							dropNum--;
						}
						memset((void*)pPcmBufPos, 0, pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame);
						aud_extra_pcmbuf_state[A_EXBF_WT_ID].done=1;
						A_EXBF_WT_ID+=1;
						if(A_EXBF_WT_ID>=EXTRA_PCM_BUF_NUM)A_EXBF_WT_ID=0;
						VOC_WORK=VOC_WORK_NON;

					}
					else
					{
						if(pIsomGlob->a_amr_mode != HAL_INVALID)
						{
							getData((U8*)pVocDecAmr->decInBuf, pIsomGlob->a_codeframeBytes, 1);
							pVocDecAmr->codecMode= pIsomGlob->a_amr_mode;
						}
						else
						{
							//write silence data
							moveDataPos(pIsomGlob->a_codeframeBytes);
							memcpy(pVocDecAmr->decInBuf, silence_frame475, 13);
							pVocDecAmr->codecMode= HAL_AMR475_DEC;
						}

						pVocDecAmr->dtxOn = 0;
						//pVocDecAmr->codecMode= pIsomGlob->a_amr_mode;
						pVocDecAmr->decFrameType = 0;
						pVocDecAmr->bfi = 0;
						pVocDecAmr->sid = 0;
						pVocDecAmr->taf = 0;
						pVocDecAmr->ufi = 0;
						pVocDecAmr->reserved= 0;
						RX_BUF_SWAP_ID=0;
						
						vpp_SpeechScheduleOneFrame(VPP_SPEECH_WAKEUP_SW_DEC);
					}
				}
#endif
	                }
					
	                j = 0;
	                while(VOC_WORK )
	                {
	                    j++;
	                    if(j>=VOC_WORK_CONT)
	                    {	
	                        diag_printf("[VID]AUD VOC_WORK_CONT end! \n");
	                        res=ISOM_ERR_AUDIO_DEC_FAIL;
	                        goto play_error;
	                    }
				//sxr_Sleep(1);
	                }	

			//if(i>1)
			//	aud_extra_pcmbuf_state[i-2].done=1;
			if(i<2)
			{
				aud_extra_pcmbuf_state[A_EXBF_RD_ID].done=0;
				A_EXBF_RD_ID+=1;
				if(A_EXBF_RD_ID>=EXTRA_PCM_BUF_NUM)A_EXBF_RD_ID=0;
			}

	                tick2=hal_TimGetUpTime();
	                diag_printf( "[VID]voc aud dec time=%d frame num=%d", (tick2-tick1)*1000/16384, pIsomGlob->a_HalfPcmBufFrame);
			if(pIsomGlob->a_type!= A_TYPE_AMR)
			{
				diag_printf( "[VID] AUD real channel=%d SampleRate=%d BitRate=%d output_len=%d streamStatus=%d",
					vocDecStatus.nbChannel, 
					vocDecStatus.SampleRate, 
					vocDecStatus.BitRate, 
					vocDecStatus.output_len, 
					vocDecStatus.streamStatus);

				if(vocDecStatus.nbChannel !=pIsomGlob->a_channels)
				{
					diag_printf( "[VID] warning! AUD channel not match");
					if(vocDecStatus.nbChannel==1)
					{
						pIsomGlob->a_channels=1;
						pIsomGlob->a_frameBytes>>=1;
						pIsomGlob->a_PcmBufHalfPos=pIsomGlob->a_PcmBuf+pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame;
					}
					else if(vocDecStatus.nbChannel==2)
					{
						pIsomGlob->a_channels=2;
						pIsomGlob->a_frameBytes<<=1;
						pIsomGlob->a_PcmBufHalfPos=pIsomGlob->a_PcmBuf+pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame;
					}
					else
					{
						res=ISOM_ERR_AUDIO_DEC_FAIL;
						goto play_error;
					}
				}
				else if(vocDecStatus.SampleRate !=pIsomGlob->a_sample_rate)
				{
					diag_printf( "[VID] warning! AUD SampleRate not match");
					if(vocDecStatus.SampleRate>=8000 && vocDecStatus.SampleRate<=48000)
						pIsomGlob->a_sample_rate=vocDecStatus.SampleRate;
					else
					{
						res=ISOM_ERR_AUDIO_DEC_FAIL;
						goto play_error;
					}

					if(pIsomGlob->a_type==A_TYPE_MP3 )
					{
						if( pIsomGlob->a_frame_sample==MP3_FRAME_SAMPLE_576 && vocDecStatus.SampleRate>=32000)
						{
							pIsomGlob->a_frame_sample=MP3_FRAME_SAMPLE_1152;
							pIsomGlob->a_frameBytes<<=1;
							pIsomGlob->a_PcmBufHalfPos=pIsomGlob->a_PcmBuf+pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame;
						}
						else if( pIsomGlob->a_frame_sample==MP3_FRAME_SAMPLE_1152 && vocDecStatus.SampleRate<=24000)
						{
							pIsomGlob->a_frame_sample=MP3_FRAME_SAMPLE_576;
							pIsomGlob->a_frameBytes>>=1;
							pIsomGlob->a_PcmBufHalfPos=pIsomGlob->a_PcmBuf+pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame;
						}
					}
				}
				else if(vocDecStatus.output_len !=pIsomGlob->a_frameBytes)
				{
					diag_printf( "[VID] warning! AUD output_len not match");
					res=ISOM_ERR_AUDIO_DEC_FAIL;
					goto play_error;
				}
			}
	            }
	            else if(pIsomGlob->a_type == A_TYPE_PCM)
	            {
	                if(i==0)
	                    pPcmBufPos=(uint32)pIsomGlob->a_PcmBuf;
	                else if(i==1)
	                {
	                    pPcmBufPos=(uint32)pIsomGlob->a_PcmBufHalfPos;
	                    i=2;
	                }
	                AUD_DEC_FRM_NUM=pIsomGlob->a_HalfPcmBufFrame;
	                while(AUD_DEC_FRM_NUM)
	                {
	                    res=getData(((unsigned char*)pPcmBufPos)+(pIsomGlob->a_HalfPcmBufFrame-(unsigned int)AUD_DEC_FRM_NUM)*pIsomGlob->a_frameBytes,
	                                    pIsomGlob->a_frameBytes, (unsigned int)AUD_DEC_FRM_NUM);
	                    if(res>0)
	                    {
	                        if((pIsomGlob->a_channels==2)&&(audioItf==AUD_ITF_LOUD_SPEAKER))
	                        {
	                            INT16 *pPcm=(INT16 *)(((unsigned char*)pPcmBufPos)+(pIsomGlob->a_HalfPcmBufFrame-(unsigned int)AUD_DEC_FRM_NUM)*pIsomGlob->a_frameBytes);
	                            uint32 sNum=pIsomGlob->a_frame_sample*2*res;

	                            for(j=0;j<sNum;j+=2)
	                            {
	                                pPcm[j]=pPcm[j+1]=(pPcm[j]>>1)+(pPcm[j+1]>>1);					
	                            }
	                        }
							
	                        AUD_DEC_FRM_NUM-=(uint16)res;
	                        fillSampBuf();
	                    }
	                    else
	                    {
	                        diag_printf( "[VID] warning! AUD data end!");
	                        AUD_DEC_FRM_NUM=0;
	                        break;
	                    }
	                }
	            }									
	        }
	    }
	}

#endif
    LAST_SYNC_AUD_FRM_NUM=pIsomGlob->a_current_frame_num;
    AUD_DEC_FRM_NUM=0;
    PCM_ISR_FLAG=0;
    VOC_WORK=VOC_WORK_NON;
    VOC_AUD=0;
    //AUD_EXTRA_BUF_DONE=1;
	A_EXBF_RD_ID=0;
	A_EXBF_WT_ID=0;
	//diag_printf("%x",aud_extra_pcmbuf_state[0].pBufPos);
	//diag_printf("%x",aud_extra_pcmbuf_state[1].pBufPos);

	fillAudioData=NULL;//not read audio in video decoder.

    //for video
#if (CHIP_HAS_GOUDA != 1)
    //NEED_BLEND_IMG==0 will decode 2 frame pingpang. or decode 1 frame.
    for(i=0;i<((NEED_BLEND_IMG==0) ? 2:1);i++)
#else
    for(i=0;i<1;i++)
#endif
    {
        if(pIsomGlob->v_current_sample_num<pIsomGlob->v_total_sample_num)
        {
            pIsomGlob->v_SampleBuf_BytesRemain-=pIsomGlob->v_SampleBuf_BytesUsed;
            if ((pIsomGlob->v_type != V_TYPE_MPEG4)&&
        	(pIsomGlob->v_type !=V_TYPE_RV)&&
        	(pIsomGlob->v_SampleBuf_BytesUsed>0) &&
        	(pIsomGlob->v_SampleBuf_BytesRemain > 0) && 
        	pIsomGlob->v_SampleBuf_BytesRemain<pIsomGlob->v_SampleBufLen)
                memcpy(pIsomGlob->v_SampleBuf , pIsomGlob->v_SampleBuf+pIsomGlob->v_SampleBuf_BytesUsed, pIsomGlob->v_SampleBuf_BytesRemain);
			
            pIsomGlob->v_SampleBuf_BytesUsed=0;
            if(pIsomGlob->v_SampleBuf_BytesRemain<0)pIsomGlob->v_SampleBuf_BytesRemain=0;

            if(pIsomGlob->med_type==MCI_TYPE_MP4 || pIsomGlob->med_type==MCI_TYPE_3GP)
            {
                uint32 readLen;
				
                if((res = stbl_GetSampleSize(pIsomGlob->v_info , pIsomGlob->v_current_sample_num+1, &readLen))!=GF_OK)
                {
                    diag_printf("[VID] video stbl_GetSampleSize err res=%d v_current_sample_num=%d \n",res, pIsomGlob->v_current_sample_num);
                    res=ISOM_ERR_VIDEO_READ_FAIL;
                    goto play_error;
                }
				
                if(pIsomGlob->v_SampleBuf_BytesRemain+readLen<pIsomGlob->v_SampleBufLen)
                {
                    uint32 gotbytes, gotcount;
					
			if(pIsomGlob->v_type == V_TYPE_MPEG4)		
				pIsomGlob->v_SampleBuf -=readLen;//reuse from end of the Y buffer.
				
                    res=gf_isom_get_multSample(pIsomGlob->v_info, 
                                            pIsomGlob->v_current_sample_num+1, 
                                            pIsomGlob->v_SampleBuf+pIsomGlob->v_SampleBuf_BytesRemain, 
                                            readLen, 	&gotbytes, &gotcount);
												
                    if(res || (gotbytes<=0))
                    {
                        diag_printf("[VID] video gf_isom_get_sample err res=%d \n",res);
                        res=ISOM_ERR_VIDEO_READ_FAIL;
                        goto play_error;
                    }
                    pIsomGlob->v_current_sample_num+=gotcount;
                    pIsomGlob->v_SampleBuf_BytesRemain+=gotbytes;
                }
                else
                {
                    res=ISOM_ERR_MEM_INSUFFICIENT;
                    goto play_error;
                }
            }
            else if(pIsomGlob->med_type==MCI_TYPE_MJPG)
            {
                int32 key_frame;

                res=avi_set_video_position(pIsomGlob->avi_p, pIsomGlob->v_current_sample_num);
                if(res!=0)
                {
                    diag_printf("[VID] avi_set_video_position err!res=%d", res);
                    res=ISOM_ERR_VIDEO_READ_FAIL;
                    goto play_error;
                }

		  if(pIsomGlob->v_type == V_TYPE_MPEG4)
		  {
		  	//reuse from end of the Y buffer.
		  	pIsomGlob->v_SampleBuf -=avi_frame_size(pIsomGlob->avi_p, pIsomGlob->v_current_sample_num);
		  }

                res=avi_read_frame(pIsomGlob->avi_p, pIsomGlob->v_SampleBuf+pIsomGlob->v_SampleBuf_BytesRemain, 
                                    pIsomGlob->v_SampleBufLen-pIsomGlob->v_SampleBuf_BytesRemain,  &key_frame);
                if( res < 0 )
                {
                    diag_printf( "[VID]avi video frame read err res=%d \n", res ); 
                    res=ISOM_ERR_VIDEO_READ_FAIL;
                    goto play_error;
                }

                pIsomGlob->v_current_sample_num+=1;				
                pIsomGlob->v_SampleBuf_BytesRemain+=res;			
            }
	     else
	     {
	     		//rm to do...
	     		pIsomGlob->v_SampleBuf_BytesRemain=1;
	     }
        }

        if(pIsomGlob->v_SampleBuf_BytesRemain>0)
        {
            //int32 used_bytes;
            if(pIsomGlob->v_type== V_TYPE_MPEG4)
            {
#ifdef MPEG4_SUPPORT
			int32 used_bytes;
                xvid_dec_stats_t xvid_dec_stats;
		  while(pIsomGlob->v_SampleBuf_BytesRemain > MIN_USEFUL_BYTES)
		  {
	                used_bytes = dec_main(pIsomGlob->v_SampleBuf+pIsomGlob->v_SampleBuf_BytesUsed,   
	                                        (uint8*)pIsomGlob->v_YuvInfo, 
	                                        pIsomGlob->v_SampleBuf_BytesRemain, &xvid_dec_stats, pZoom,
	                                        ((pIsomGlob->med_type ==MCI_TYPE_MJPG) ? TRUE : FALSE));
					
	                if(used_bytes == -1)//profile unfit
	                {
	                    diag_printf("[VID] video decode err v_current_sample_num=%d \n",pIsomGlob->v_current_sample_num);
	                    res=ISOM_ERR_VIDEO_DEC_FAIL;
	                    goto play_error;
	                }
	                
	                if(used_bytes == -2)
	                {
	                    // to skip a B-frame
	                    pIsomGlob->bSkipFrame = TRUE;
	                    pIsomGlob->v_SampleBuf_BytesUsed=pIsomGlob->v_SampleBuf_BytesRemain;
	                }
					
	                if(used_bytes > 0) 
	                {
    				  if(xvid_dec_stats.type == XVID_TYPE_VOL)
				  {
				  	pIsomGlob->v_SampleBuf_BytesUsed+=used_bytes;
					pIsomGlob->v_SampleBuf_BytesRemain-=used_bytes;
				  	continue;
				  }

		                 pIsomGlob->v_SampleBuf_BytesUsed+=pIsomGlob->v_SampleBuf_BytesRemain;
	                }

			 break;
		  }
		  if(pIsomGlob->v_SampleBuf_BytesRemain==MIN_USEFUL_BYTES) pIsomGlob->v_SampleBuf_BytesUsed=1;

		pIsomGlob->v_SampleBuf=dec_get_cur_y_end();//reuse for next frame data read.
		if(!pIsomGlob->v_SampleBuf)
		{
			diag_printf("[VID] dec_get_cur_y_end ERR!\n");
			res= ISOM_ERR_VIDEO_DEC_FAIL;
			goto play_error;
		}

#endif
            }
#ifdef H263_SUPPORT
            else if(pIsomGlob->v_type== V_TYPE_H263)
            {
			int32 used_bytes;
                int32 gob;
                resetbits (pIsomGlob->v_SampleBuf+pIsomGlob->v_SampleBuf_BytesUsed, pIsomGlob->v_SampleBuf_BytesRemain);

                gob= getheader ();
                if(errorstate)
                {
                    diag_printf("[VID] h.263 getheader err gob=%d \n",gob);
                    res= ISOM_ERR_INVALID_FORMAT;
                    goto play_error;
                }
				
                if (!gob )
                {
                    diag_printf("[VID] h.263 getheader err !data use=%d \n", ld->rdptr - ld->rdbfr);
                    res= ISOM_ERR_INVALID_FORMAT;
                    goto play_error;
                }
			
                getpicture ((uint8*)pIsomGlob->v_YuvInfo, pIsomGlob->v_lcd_size_x, pIsomGlob->v_lcd_start_x, pIsomGlob->v_lcd_start_y, gob, pZoom);
                if(errorstate!=0)
                {
                    diag_printf("[VID] h.263 getpicture err! \n");
                    res= ISOM_ERR_VIDEO_DEC_FAIL;
                    goto play_error;
                }

                used_bytes=ld->rdptr - ld->rdbfr;
                /* Update buffer pointers */
				
                if(used_bytes > 0) 
                {
                    pIsomGlob->v_SampleBuf_BytesUsed+=used_bytes;
                }
            }
#endif
            else if(pIsomGlob->v_type== V_TYPE_MJPEG)
            {
                DEC_UNIT jpgDec;
                res= MMF_JpegDecodePicture((long)((char*)pIsomGlob->v_SampleBuf), (char*) pIsomGlob->v_YuvInfo[0],
                                            (pIsomGlob->v_width*pIsomGlob->v_height*3)>>1,
                                            pIsomGlob->v_width, pIsomGlob->v_height, 
                                            0,0,
                                            pIsomGlob->v_width-1, pIsomGlob->v_height-1, 
                                            &jpgDec, 2,0);
                if(res!=FUNC_OK)
                {
                    diag_printf("[VID] Mjpeg MMF_JpegDecodePicture err!res=%d", res);
                    res= ISOM_ERR_VIDEO_DEC_FAIL;
                    goto play_error;
                }
                pIsomGlob->v_YuvInfo[3]=(uint32)jpgDec.width;
                pIsomGlob->v_YuvInfo[4]=(uint32)jpgDec.height;
                pIsomGlob->v_SampleBuf_BytesUsed+=pIsomGlob->v_SampleBuf_BytesRemain;
            }
#ifdef RM_SUPPORT			
	     else if( pIsomGlob->v_type==V_TYPE_RV)
	     {
	     		VocWorkStruct vocState;
			vocState.pVOC_WORK=(UINT8*)&VOC_WORK;
			vocState.pVOC_VID=(int16*)&VOC_VID;
			vocState.vid_voc_isr=vid_voc_isr;
			vocState.ra_dec=NULL;
			
			res=rm_getOneFrameVideo((INT8*)pIsomGlob->v_YuvInfo, &vocState, 0);
			if(res==HXR_SKIP_BFRAME)
			{
				pIsomGlob->bSkipFrame = TRUE;
			}
			else if(res!=HXR_OK)
		       {
		              diag_printf("[VID] rm_getOneFrameVideo err!res=%d ", res);
	                    res= ISOM_ERR_VIDEO_DEC_FAIL;
	                    goto play_error;
		       }

			pIsomGlob->v_SampleBuf_BytesUsed=1;
	      }
#endif
            else
            {
                diag_printf("[VID] err pIsomGlob->v_type=%d !!!\n",pIsomGlob->v_type);
                res=ISOM_ERR_UNSUPPORT;
                goto play_error;		
            }
        }
        else
        {
#if 0
            if(pIsomGlob->v_current_sample_num>=pIsomGlob->v_total_sample_num-1)
            {
                diag_printf("[VID] video no data, play finish!\n");
                play_state=VID_PLAY_STATE_PLAYING;
                res=ISOM_ERR_VIDEO_READ_FAIL;
                goto play_error;
            }
            else
            {
                res=ISOM_ERR_DATA_NONE;
                goto play_error;
            }
#else
            pIsomGlob->bSkipFrame = TRUE;
#endif
        }

        if( pIsomGlob->bSkipFrame == FALSE )
        {
            pIsomGlob->v_ImageBuf_HalfFlag=i;
        }

#if (CHIP_HAS_GOUDA != 1)
        if(VOC_WORK==VOC_WORK_NON)
        {
            if( pIsomGlob->bSkipFrame== FALSE )
            {
            	  if(!NEED_BLEND_IMG)
                	pIsomGlob->v_ImageBufPos=(pIsomGlob->v_ImageBuf_HalfFlag==0)?(pIsomGlob->v_ImageBuf):(pIsomGlob->v_ImageBuf2);
		  else
                	pIsomGlob->v_ImageBufPos=pIsomGlob->v_ImageBuf;

                //pIsomGlob->v_ImageBufPos+=pIsomGlob->v_lcd_start_y*pIsomGlob->v_lcd_size_x;
                IMAGE_INBUF_FLAG=(int8)pIsomGlob->v_ImageBuf_HalfFlag;
                VOC_WORK=VOC_WORK_VID;
                VOC_VID=1;

                vocDecVid.inStreamBufAddr=(INT32*)pIsomGlob->v_YuvInfo[0];
                vocDecVid.inStreamUBufAddr=(INT32*)pIsomGlob->v_YuvInfo[1];
                vocDecVid.inStreamVBufAddr=(INT32*)pIsomGlob->v_YuvInfo[2];
                vocDecVid.outStreamBufAddr=(INT32*)pIsomGlob->v_ImageBufPos;
                vocDecVid.Source_width=(short)pIsomGlob->v_YuvInfo[3];
                vocDecVid.Source_height=(short)pIsomGlob->v_YuvInfo[4];

                vpp_AudioJpegDecScheduleOneFrame(&vocDecVid);
            }
            else
            {
                pIsomGlob->bSkipFrame = FALSE;
            }
        }
        else if((VOC_WORK==VOC_WORK_AUD && VOC_VID==0))
        {
            if(!NEED_BLEND_IMG)
            		pIsomGlob->v_ImageBufPos=(pIsomGlob->v_ImageBuf_HalfFlag==0)?(pIsomGlob->v_ImageBuf):(pIsomGlob->v_ImageBuf2);
	     else
            		pIsomGlob->v_ImageBufPos=pIsomGlob->v_ImageBuf;
            //pIsomGlob->v_ImageBufPos+=pIsomGlob->v_lcd_start_y*pIsomGlob->v_lcd_size_x;
            IMAGE_INBUF_FLAG=(int8)pIsomGlob->v_ImageBuf_HalfFlag;
            VOC_VID++;
        }
        else
        {
            diag_printf("[VID]voc Drop 1 frame display");
        }

        j=0;
        while(VOC_WORK )
        {
            j++;
            if(j>=VOC_WORK_CONT)
            {	
                diag_printf("YUV VOC_WORK_CONT end! \n");
                res=ISOM_ERR_VIDEO_DEC_FAIL;
                goto play_error;
            }
		//sxr_Sleep(1);
        }
#else
        if( pIsomGlob->bSkipFrame == FALSE )
        {
            IMAGE_INBUF_FLAG=(int8)pIsomGlob->v_ImageBuf_HalfFlag;
/*
            if( pIsomGlob->v_ImageBuf_HalfFlag==0 )   // copy YUV data to backup buffer
            {
                memcpy((uint8*)pIsomGlob->v_YuvInfo2[0], (uint8*)pIsomGlob->v_YuvInfo[0], (pIsomGlob->v_YuvInfo[3]*pIsomGlob->v_YuvInfo[4]));
                memcpy((uint8*)pIsomGlob->v_YuvInfo2[1], (uint8*)pIsomGlob->v_YuvInfo[1], (pIsomGlob->v_YuvInfo[3]*pIsomGlob->v_YuvInfo[4])/4);
                memcpy((uint8*)pIsomGlob->v_YuvInfo2[2], (uint8*)pIsomGlob->v_YuvInfo[2], (pIsomGlob->v_YuvInfo[3]*pIsomGlob->v_YuvInfo[4])/4);

                pIsomGlob->v_YuvInfo2[3]= pIsomGlob->v_YuvInfo[3];
                pIsomGlob->v_YuvInfo2[4]= pIsomGlob->v_YuvInfo[4];
            }
*/
            vid_update();	
        }
#endif
    }
	
    diag_printf("[VID] v_current_frame_num= %d v_current_sample_num=%d \n",pIsomGlob->v_current_frame_num, pIsomGlob->v_current_sample_num);

#if 1
    if(audioItf!=AUD_ITF_EAR_PIECE&&audioItf!=AUD_ITF_LOUD_SPEAKER)
    {
        // Initial cfg
        audioItf = AUD_ITF_EAR_PIECE;
    }
    
    audioStream.startAddress = (UINT32*)pIsomGlob->a_PcmBuf;
    audioStream.length=pIsomGlob->a_frameBytes*(pIsomGlob->a_HalfPcmBufFrame<<1);
    audioStream.channelNb = pIsomGlob->a_channels;
    audioStream.voiceQuality = FALSE; 
    audioStream.halfHandler = vid_pcmHalf_isr;
    audioStream.endHandler = vid_pcmEnd_isr;

    switch (pIsomGlob->a_sample_rate)
    {
        case 48000:
            audioStream.sampleRate = HAL_AIF_FREQ_48000HZ;
            break;
        case 44100:
            audioStream.sampleRate = HAL_AIF_FREQ_44100HZ;
            break;
        case 32000:
            audioStream.sampleRate = HAL_AIF_FREQ_32000HZ;
            break;
        case 24000:
            audioStream.sampleRate = HAL_AIF_FREQ_24000HZ;
            break;
        case 22050:
            audioStream.sampleRate = HAL_AIF_FREQ_22050HZ;
            break;
        case 16000:
            audioStream.sampleRate = HAL_AIF_FREQ_16000HZ;
            break;
        case 12000:
            audioStream.sampleRate = HAL_AIF_FREQ_12000HZ;
            break;
        case 11025:
            audioStream.sampleRate = HAL_AIF_FREQ_11025HZ;
            break;
        case 8000:
            audioStream.sampleRate = HAL_AIF_FREQ_8000HZ;
            break;			
        default:
            diag_printf( "##WARNING## invalid a_sample_rate=%d\n",pIsomGlob->a_sample_rate);
            res= ISOM_ERR_INVALID_FORMAT;
            goto play_error;
    }

#if 1
    if(pIsomGlob->open_audio)
    {
    diag_printf( "[VID]open audio \n");
        if((res=aud_StreamStart(audioItf, &audioStream, &audio_cfg))!=AUD_ERR_NO)
        {
            diag_printf( "[VID]##WARNING## first aud_StreamStart fail res= %d \n",res);
	     aud_StreamStop(audioItf);
	     if((res=aud_StreamStart(audioItf, &audioStream, &audio_cfg))!=AUD_ERR_NO)
	     {
	            diag_printf( "[VID]##ERROR## aud_StreamStart res= %d \n",res);
	            res=ISOM_ERR_PCM_OPEN_FAIL;
	            goto play_error;
	     }
        }
        AUD_DATA_REQ=1;
    }
    else
    {
		// record the start time for time bar and video sync
		if(pIsomGlob->med_type!=MCI_TYPE_RM)
			TIME_LINE_LAST=hal_TimGetUpTime();
		else
		{
			TIME_LINE_CUR=pIsomGlob->current_time;
			TIME_LINE_LAST=hal_TimGetUpTime();
		}
    }
#endif
	
#endif
	
#if 1 //display video or not 
	
    res=COS_SetTimerEX(GetMCITaskHandle(MBOX_ID_MMC), MMC_VID_PLY_TIMER_ID, COS_TIMER_MODE_PERIODIC, pIsomGlob->v_timerPeriod);
    diag_printf( "[VID]timer set result= %d \n",res);
	
    VID_DEC_REQ=0;
    SEND_EVENT_NUM=0;
    DISPLAY_SEND_EVENT_NUM=0;
	keep_skip_frame_num=0;

#ifdef USE_KEY_SYNC
    KEY_SAMP_NUM=0;
    KEY_SAMP_TIME_WAIT=0;
    KEY_SAMP_TIME_START=0;
#endif

#endif
	
    play_state=VID_PLAY_STATE_PLAYING;
    diag_printf("[VID] mmc_vid_isom_play OK!");
    return ISOM_ERR_OK;

play_error:
    diag_printf("[VID] ##WARNING## mmc_vid_isom_play err res= %d \n",res);
    mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VID_STOP_REQ);
    return res;
}

int32 mmc_vid_isom_seek ( uint32	time, int16 time_mode, int16 lcd_start_x, int16	lcd_start_y)
{
    int32 res;
    int32 used_bytes;
    unsigned long long temp;

    totalTa=0;
    totalTv=0;
    totalTx=0;
    counta=0;
    countv=0;
    countx=0;
	
    if((play_state!=VID_PLAY_STATE_IDLE) && (play_state!=VID_PLAY_STATE_PAUSE) )
    {
        diag_printf("[VID] mmc_vid_isom_seek err play_state= %d \n",play_state);
        return  ISOM_ERR_PLAY_STATE_FAIL;
    }
	
    //pIsomGlob->v_lcd_start_x=lcd_start_x;//set by mmc_vid_isom_set_mode
    //pIsomGlob->v_lcd_start_y=lcd_start_y;
    pIsomGlob->v_lcd_end_x=pIsomGlob->v_lcd_start_x+pIsomGlob->v_display_width-1;
    pIsomGlob->v_lcd_end_y=pIsomGlob->v_lcd_start_y+pIsomGlob->v_display_height-1;

    if(time_mode==SEEK_TIME_MODE_OFFSET)
    {
        pIsomGlob->current_time+=time;
    }
    else if(time_mode==SEEK_TIME_MODE_ABSOLUTE)//default
    {
        pIsomGlob->current_time=time;
    }
    diag_printf("[VID] v_lcd_start_x=%d v_lcd_end_x=%d",pIsomGlob->v_lcd_start_x, pIsomGlob->v_lcd_end_x);
    diag_printf("[VID] v_lcd_start_y=%d v_lcd_end_y=%d",pIsomGlob->v_lcd_start_y, pIsomGlob->v_lcd_end_y);
    diag_printf("[VID] seek time=%d \n",pIsomGlob->current_time);

    if(pIsomGlob->current_time<0)
    {
        pIsomGlob->current_time=0;
    }
    else if(pIsomGlob->current_time>pIsomGlob->total_time)
    {
        pIsomGlob->current_time=pIsomGlob->total_time;
    }

    temp=(unsigned long long)pIsomGlob->current_time*(unsigned long long)pIsomGlob->v_fps/(1000<<V_FPS_SCALE_BIT);
    pIsomGlob->v_current_frame_num=(uint32)temp;
    pIsomGlob->v_current_sample_num=pIsomGlob->v_current_frame_num;//wiil change (frame per sample ?)

    //get key frame number
    if(pIsomGlob->v_current_sample_num<pIsomGlob->v_total_sample_num)
    {
        if(pIsomGlob->med_type==MCI_TYPE_MP4 || pIsomGlob->med_type==MCI_TYPE_3GP)
	        pIsomGlob->v_current_sample_num=gf_isom_get_keySampleNumber(pIsomGlob->v_trak, pIsomGlob->v_current_sample_num );
	 else if(pIsomGlob->med_type==MCI_TYPE_MJPG && pIsomGlob->v_type!=V_TYPE_MJPEG)
	 	pIsomGlob->v_current_sample_num=avi_get_video_keyframe_num(pIsomGlob->avi_p, pIsomGlob->v_current_sample_num);
    }
    else
    {
		pIsomGlob->v_current_sample_num=(pIsomGlob->v_total_sample_num==0)?0:(pIsomGlob->v_total_sample_num-1);
        if(pIsomGlob->med_type==MCI_TYPE_MP4 || pIsomGlob->med_type==MCI_TYPE_3GP)
		 pIsomGlob->v_current_sample_num=gf_isom_get_keySampleNumber(pIsomGlob->v_trak, pIsomGlob->v_current_sample_num );
	 else if(pIsomGlob->med_type==MCI_TYPE_MJPG && pIsomGlob->v_type!=V_TYPE_MJPEG)
	 	pIsomGlob->v_current_sample_num=avi_get_video_keyframe_num(pIsomGlob->avi_p, pIsomGlob->v_current_sample_num);
    }
	
    //update seek frame number and time
    pIsomGlob->v_current_frame_num=pIsomGlob->v_current_sample_num;//wiil change (frame per sample ?)
    if(pIsomGlob->med_type==MCI_TYPE_MP4 || pIsomGlob->med_type==MCI_TYPE_3GP
		|| (pIsomGlob->med_type==MCI_TYPE_MJPG && pIsomGlob->v_type!=V_TYPE_MJPEG))
    {
        if(!pIsomGlob->v_fps)
        {
            diag_printf("[VID] err pIsomGlob->v_fps=%d !!!\n",pIsomGlob->v_fps);
            return ISOM_ERR_PARAM_ERROR;
        }
        temp=(((unsigned long long)pIsomGlob->v_current_frame_num*1000)<<V_FPS_SCALE_BIT)/(unsigned long long)pIsomGlob->v_fps;
        pIsomGlob->current_time=(uint32)temp;
    }
    diag_printf("[VID] new position key framenum=%d \n", pIsomGlob->v_current_frame_num);
    diag_printf("[VID] new position current_time=%d \n", pIsomGlob->current_time);
	
    pIsomGlob->v_SampleBuf_BytesRemain-=pIsomGlob->v_SampleBuf_BytesUsed;	
    if ((pIsomGlob->v_SampleBuf_BytesUsed>0) &&(pIsomGlob->v_SampleBuf_BytesRemain > 0) && pIsomGlob->v_SampleBuf_BytesRemain<pIsomGlob->v_SampleBufLen)
        memcpy(pIsomGlob->v_SampleBuf , pIsomGlob->v_SampleBuf+pIsomGlob->v_SampleBuf_BytesUsed, pIsomGlob->v_SampleBuf_BytesRemain);

    pIsomGlob->v_SampleBuf_BytesUsed=0;
    if(pIsomGlob->v_SampleBuf_BytesRemain<0)pIsomGlob->v_SampleBuf_BytesRemain=0;
	
    tick1=hal_TimGetUpTime();			
    switch(pIsomGlob->med_type)
    {
        case MCI_TYPE_3GP:
        case MCI_TYPE_MP4:
        {
            uint32 readLen, gotbytes, gotcount;
            if((res = stbl_GetSampleSize(pIsomGlob->v_info , pIsomGlob->v_current_sample_num+1, &readLen))!=GF_OK)
            {
                diag_printf("[VID] video stbl_GetSampleSize err res=%d v_current_sample_num=%d \n",res, pIsomGlob->v_current_sample_num);
                return ISOM_ERR_VIDEO_READ_FAIL;
            }

            if(pIsomGlob->v_SampleBuf_BytesRemain+readLen<pIsomGlob->v_SampleBufLen)
            {
                if(pIsomGlob->v_type == V_TYPE_MPEG4)
            	  	pIsomGlob->v_SampleBuf -=readLen;//reuse from end of the Y buffer.
            	  
                res=gf_isom_get_multSample(pIsomGlob->v_info, 
                                pIsomGlob->v_current_sample_num+1, 
                                pIsomGlob->v_SampleBuf+pIsomGlob->v_SampleBuf_BytesRemain, 
                                readLen, 	&gotbytes, &gotcount);
												
                if(res || (gotbytes<=0))
                {
                    diag_printf("[VID] video gf_isom_get_sample err res=%d \n",res);
                    return ISOM_ERR_VIDEO_READ_FAIL;
                }
                pIsomGlob->v_current_sample_num+=gotcount;
                pIsomGlob->v_SampleBuf_BytesRemain+=gotbytes;
            }
            else
            {
                diag_printf("[VID] err MEM_INSUFFICIENT readLen=%d \n", readLen);
                return ISOM_ERR_MEM_INSUFFICIENT;
            }
        }
        break;

        case MCI_TYPE_MJPG:
        {
			int32 key_frame;
			do
			{
				res=avi_set_video_position(pIsomGlob->avi_p, pIsomGlob->v_current_sample_num);
				if(res!=0)
				{
					diag_printf("[VID] avi_set_video_position err!");
					//return ISOM_ERR_VIDEO_READ_FAIL;
					break;
				}

				  if(pIsomGlob->v_type == V_TYPE_MPEG4)
				  {
				  	//reuse from end of the Y buffer.
				  	pIsomGlob->v_SampleBuf -=avi_frame_size(pIsomGlob->avi_p, pIsomGlob->v_current_sample_num);
				  }
		  
				res=avi_read_frame(pIsomGlob->avi_p, pIsomGlob->v_SampleBuf, pIsomGlob->v_SampleBufLen,  &key_frame);
				if( res < 0 )
				{
					diag_printf( "[VID]MJPG avi video frame read err res=%d \n", res ); 
					//return ISOM_ERR_VIDEO_READ_FAIL;
					break;
				}
				pIsomGlob->v_current_sample_num+=1;
				pIsomGlob->v_SampleBuf_BytesRemain+=res;
			}while(res==0 && pIsomGlob->v_current_sample_num < pIsomGlob->v_total_sample_num);
			
            break;
        }
#ifdef RM_SUPPORT		
	case MCI_TYPE_RM:
	{
		res=rm_seek( time);
		
		if(res!=HXR_OK)
		{
		 	diag_printf("[VID]rm_seek err res=%d\n", res);
		 	return ISOM_ERR_VIDEO_READ_FAIL;
		}
		//get real time offset.
		pIsomGlob->current_time=rm_getKeyTimeStamp();
		while(pIsomGlob->current_time<time)
		{
			time=pIsomGlob->current_time;
			res=rm_seek( pIsomGlob->current_time);
			if(res!=HXR_OK)
			{
			 	diag_printf("[VID]rm_seek err res=%d\n", res);
			 	return ISOM_ERR_VIDEO_READ_FAIL;
			}
			
			pIsomGlob->current_time=rm_getKeyTimeStamp();
			
			if(pIsomGlob->current_time<=0)break;
		}
		temp=(unsigned long long)pIsomGlob->current_time*(unsigned long long)pIsomGlob->v_fps/(1000<<V_FPS_SCALE_BIT);
		pIsomGlob->v_current_frame_num=(uint32)temp;
		pIsomGlob->v_current_sample_num=pIsomGlob->v_current_frame_num;
		
		pIsomGlob->v_SampleBuf_BytesRemain=1;
	}
	break;
#endif
        default:
            diag_printf("[VID]  err! pIsomGlob->v_type=%d \n",pIsomGlob->v_type);
            return ISOM_ERR_INVALID_FORMAT;
    }
	
    tick2=hal_TimGetUpTime();
    diag_printf("[VID]video read time=%d", (tick2-tick1)*1000/16384);
    tick1=hal_TimGetUpTime();

    pIsomGlob->bSkipFrame = FALSE;
	fillAudioData=NULL;//not read audio in video decoder.
	
    if(pIsomGlob->v_SampleBuf_BytesRemain > 0)
    {
        switch(pIsomGlob->v_type)
        {
#ifdef MPEG4_SUPPORT
            case V_TYPE_MPEG4:
            //decode mpeg4
            {
                xvid_dec_stats_t xvid_dec_stats;
                // read new data 
					
                //if(pIsomGlob->v_SampleBuf_BytesRemain<=MIN_USEFUL_BYTES)
                //    return ISOM_ERR_DATA_NONE;
		
			while(pIsomGlob->v_SampleBuf_BytesRemain > MIN_USEFUL_BYTES)
			{
		                used_bytes = dec_main(pIsomGlob->v_SampleBuf+pIsomGlob->v_SampleBuf_BytesUsed, 
		                                                (uint8*)pIsomGlob->v_YuvInfo, 
		                                                pIsomGlob->v_SampleBuf_BytesRemain, &xvid_dec_stats, pZoom,
		                                                ((pIsomGlob->med_type ==MCI_TYPE_MJPG) ? TRUE : FALSE));

				  diag_printf("[VID] used_bytes=%d xvid_dec_stats.type=%d \n", used_bytes, xvid_dec_stats.type);
				  
		                if(used_bytes == -1)//profile unfit
		                {
		                    return ISOM_ERR_VIDEO_DEC_FAIL;
		                }
		                if(used_bytes == -2)
		                {
		                    pIsomGlob->bSkipFrame = TRUE;
		                    pIsomGlob->v_SampleBuf_BytesUsed=pIsomGlob->v_SampleBuf_BytesRemain;
		                }
						
		                /* Update buffer pointers */
		                if(used_bytes > 0) 
		                {
            				  if(xvid_dec_stats.type == XVID_TYPE_VOL)
					  {
					  	pIsomGlob->v_SampleBuf_BytesUsed+=used_bytes;
						pIsomGlob->v_SampleBuf_BytesRemain-=used_bytes;
					  	continue;
					  }

			                 pIsomGlob->v_SampleBuf_BytesUsed+=pIsomGlob->v_SampleBuf_BytesRemain;
		                }
						
				break;
			}
			if(pIsomGlob->v_SampleBuf_BytesRemain==MIN_USEFUL_BYTES) pIsomGlob->v_SampleBuf_BytesUsed=1;

			pIsomGlob->v_SampleBuf=dec_get_cur_y_end();//reuse for next frame data read.
			if(!pIsomGlob->v_SampleBuf)
			{
				diag_printf("[VID] dec_get_cur_y_end ERR!\n");
				return  ISOM_ERR_VIDEO_DEC_FAIL;
			}

            }
            break;
#endif
		
#ifdef H263_SUPPORT
            case V_TYPE_H263:
            {
                //decode h.263
                int32 gob;

                resetbits (pIsomGlob->v_SampleBuf+pIsomGlob->v_SampleBuf_BytesUsed, pIsomGlob->v_SampleBuf_BytesRemain);

                gob= getheader ();

                if(errorstate)
                {
                    diag_printf("[VID] h.263 getheader err gob=%d \n",gob);
                    return ISOM_ERR_INVALID_FORMAT;
                }
				
                if (!gob )
                {
                    diag_printf("[VID] h.263 getheader err !data use=%d \n", ld->rdptr - ld->rdbfr);
                    return ISOM_ERR_INVALID_FORMAT;
                    //continue;
                }

                getpicture ((uint8*)pIsomGlob->v_YuvInfo, pIsomGlob->v_lcd_size_x, pIsomGlob->v_lcd_start_x, pIsomGlob->v_lcd_start_y, gob, pZoom);
				
                if(errorstate!=0)
                {
                    diag_printf("[VID] h.263 getpicture err! \n");
                    return ISOM_ERR_VIDEO_DEC_FAIL;
                }

                used_bytes=ld->rdptr - ld->rdbfr;
                /* Update buffer pointers */
				
                if(used_bytes > 0) 
                {
                    pIsomGlob->v_SampleBuf_BytesUsed+=used_bytes;
                }
				
                break;
            }
#endif
            case V_TYPE_MJPEG:
            {
                DEC_UNIT jpgDec;

                res= MMF_JpegDecodePicture((long)((char*)pIsomGlob->v_SampleBuf), (char*) pIsomGlob->v_YuvInfo[0],
                                                (pIsomGlob->v_width*pIsomGlob->v_height*3)>>1,
                                                pIsomGlob->v_width, pIsomGlob->v_height,
                                                0,0,
                                                pIsomGlob->v_width-1, pIsomGlob->v_height-1, 
                                                &jpgDec, 2,0);
                if(res!=FUNC_OK)
                {
                    diag_printf("[VID] Mjpeg MMF_JpegDecodePicture err!res=%d %x", res, *(int*)pIsomGlob->v_SampleBuf);
                    return ISOM_ERR_VIDEO_DEC_FAIL;
                }

                pIsomGlob->v_YuvInfo[3]=(uint32)jpgDec.width;
                pIsomGlob->v_YuvInfo[4]=(uint32)jpgDec.height;
                pIsomGlob->v_SampleBuf_BytesUsed+=pIsomGlob->v_SampleBuf_BytesRemain;
				
                break;
            }
#ifdef RM_SUPPORT
	     case V_TYPE_RV:
	     {
		 	int32 i;
			VocWorkStruct vocState;
			vocState.pVOC_WORK=(UINT8*)&VOC_WORK;
			vocState.pVOC_VID=(int16*)&VOC_VID;
			vocState.vid_voc_isr=vid_voc_isr;
			vocState.ra_dec=NULL;

			//first output may be last frame. do twice to output key frame
			i=2;
			while(i>0)
			{
			 	res=rm_getOneFrameVideo((INT8*)pIsomGlob->v_YuvInfo, &vocState, 0);

				if(res!=HXR_OK)
			       {
			              diag_printf("[VID] rm_getOneFrameVideo err!res=%d ", res);
			              return ISOM_ERR_VIDEO_DEC_FAIL;
			       }

				if(rm_getLastFrameTimeStamp() == pIsomGlob->current_time)break;
				
				i--;
			}
			pIsomGlob->v_SampleBuf_BytesUsed=1;
				
		 	break;
	      }
#endif
            default:
                diag_printf("[VID]  err! pIsomGlob->v_type=%d \n",pIsomGlob->v_type);
                return ISOM_ERR_INVALID_FORMAT;
        }
    }
    else
    {
        pIsomGlob->bSkipFrame = TRUE;
    }

#if (CHIP_HAS_GOUDA != 1)
    tick2=hal_TimGetUpTime();
    diag_printf("[VID]video decode time=%d", (tick2-tick1)*1000/16384);

	IMAGE_W_INBUF0=0;
	IMAGE_H_INBUF0=0;
	IMAGE_W_INBUF1=0;
	IMAGE_H_INBUF1=0;

    if(VOC_WORK==VOC_WORK_NON)
    {
        if( pIsomGlob->bSkipFrame == FALSE)
        {
            pIsomGlob->v_ImageBufPos=pIsomGlob->v_ImageBuf;//+pIsomGlob->v_lcd_start_y*pIsomGlob->v_lcd_size_x;
            IMAGE_INBUF_FLAG=0;
            VOC_WORK=VOC_WORK_VID;
            VOC_VID=1;
     	     //AUD_EXTRA_BUF_DONE=1;

            vocDecVid.inStreamBufAddr=(INT32*)pIsomGlob->v_YuvInfo[0];
            vocDecVid.inStreamUBufAddr=(INT32*)pIsomGlob->v_YuvInfo[1];
            vocDecVid.inStreamVBufAddr=(INT32*)pIsomGlob->v_YuvInfo[2];
            vocDecVid.outStreamBufAddr=(INT32*)pIsomGlob->v_ImageBufPos;//(pIsomGlob->v_ImageBuf+pIsomGlob->v_lcd_start_y*pIsomGlob->v_lcd_size_x);
            vocDecVid.Source_width=(short)pIsomGlob->v_YuvInfo[3];
            vocDecVid.Source_height=(short)pIsomGlob->v_YuvInfo[4];
			
            tick1=hal_TimGetUpTime();	
		
            vpp_AudioJpegDecScheduleOneFrame(&vocDecVid);
			
            vocDecVid.reset=0;
            vocDecAud.reset=0;
        }
		else
			pIsomGlob->bSkipFrame = FALSE;	
    }
    else if((VOC_WORK==VOC_WORK_AUD && VOC_VID==0))
    {
        pIsomGlob->v_ImageBufPos=pIsomGlob->v_ImageBuf;//+pIsomGlob->v_lcd_start_y*pIsomGlob->v_lcd_size_x;
        IMAGE_INBUF_FLAG=0;
        VOC_VID++;
        diag_printf( "[VID]voc working!");
    }
    else
    {
        diag_printf("[VID]voc Drop 1 frame display");
    }
	
    res=0;
    while(VOC_WORK )
    {
        res++;
        if(res>=5)
        {	
            diag_printf("[VID]VOC_WORK_CONT end! \n");
            return ISOM_ERR_VOC_OPEN_FAIL;
        }
	sxr_Sleep(1600);
    }

    tick2=hal_TimGetUpTime();	
    diag_printf("[VID]voc yuv time=%d", (tick2-tick1)*1000/16384);
#else
    tick2=hal_TimGetUpTime();
    diag_printf("[VID]video decode time=%d", (tick2-tick1)*1000/16384);
/*
    if( pIsomGlob->v_YuvInfo2[0]==0  )
    {
        if(((pIsomGlob->v_width*pIsomGlob->v_height*3)>>1)>mmc_MemGetFree()-32)//for  yuv backup buffer
        {
            res=ISOM_ERR_MEM_INSUFFICIENT;
            goto open_error2;
        }
 
        pIsomGlob->v_YuvInfo2[0]=(uint32)mmc_MemMalloc((pIsomGlob->v_YuvInfo[3]*pIsomGlob->v_YuvInfo[4]*3)>>1);
	
        if(pIsomGlob->v_YuvInfo2[0])
        {
            memset((uint8*)pIsomGlob->v_YuvInfo2[0], 0, (pIsomGlob->v_YuvInfo[3]*pIsomGlob->v_YuvInfo[4]*3)>>1);
            pIsomGlob->v_YuvInfo2[1]=pIsomGlob->v_YuvInfo2[0]+pIsomGlob->v_YuvInfo[3]*pIsomGlob->v_YuvInfo[4];
            pIsomGlob->v_YuvInfo2[2]=pIsomGlob->v_YuvInfo2[1]+((pIsomGlob->v_YuvInfo[3]*pIsomGlob->v_YuvInfo[4])>>2);
            pIsomGlob->v_ImageBuf_HalfFlag=0;
        }
        else
        {
            res=ISOM_ERR_MALLOC_FAIL;
            goto open_error2;
        }
    }*/
		
    //tick2=hal_TimGetUpTime();
    //diag_printf("[VID]video decode time=%d", (tick2-tick1)*1000/16384);

    tick1=hal_TimGetUpTime();
    if( pIsomGlob->bSkipFrame == FALSE)
    {
        IMAGE_INBUF_FLAG=0;		
    	/*
        memcpy((uint8*)pIsomGlob->v_YuvInfo2[0], (uint8*)pIsomGlob->v_YuvInfo[0], (pIsomGlob->v_YuvInfo[3]*pIsomGlob->v_YuvInfo[4]));
        memcpy((uint8*)pIsomGlob->v_YuvInfo2[1], (uint8*)pIsomGlob->v_YuvInfo[1], (pIsomGlob->v_YuvInfo[3]*pIsomGlob->v_YuvInfo[4])/4);
        memcpy((uint8*)pIsomGlob->v_YuvInfo2[2], (uint8*)pIsomGlob->v_YuvInfo[2], (pIsomGlob->v_YuvInfo[3]*pIsomGlob->v_YuvInfo[4])/4);
					
        pIsomGlob->v_YuvInfo2[3]= pIsomGlob->v_YuvInfo[3];
        pIsomGlob->v_YuvInfo2[4]= pIsomGlob->v_YuvInfo[4];
	*/				
        vid_update();
    }
		
    //tick2=hal_TimGetUpTime();	
    //diag_printf("[VID]copy time=%d",  (tick2-tick1)*1000/16384);
    
#endif

    tick1=hal_TimGetUpTime();
    
    pIsomGlob->v_ImageBuf_HalfFlag=1;

    DISPLAY_INTERFACE=2;
    vid_display();

    tick2=hal_TimGetUpTime();	
    diag_printf("[VID]display time=%d",  (tick2-tick1)*1000/16384);


    play_state=VID_PLAY_STATE_IDLE;
    diag_printf("[VID] mmc_vid_isom_seek OK!");
    //asm("break 1");
    return ISOM_ERR_OK;

#if(CHIP_HAS_GOUDA == 1)	
/*
open_error2:
    diag_printf("[VID] ##WARNING## mmc_vid_isom_open_file err res=%d\n",res);
    if(VOC_OPEN)
    {
#if(CHIP_HAS_GOUDA == 1)
		if(pIsomGlob->a_type != A_TYPE_AMR)
		{
#endif
			vpp_AudioJpegDecClose();
#if(CHIP_HAS_GOUDA == 1)
		}
		else
		{
			vpp_SpeechClose();
		}
#endif
        VOC_OPEN=0;
    }

    if(pIsomGlob)
    {
        if(pIsomGlob->med_type==MCI_TYPE_MP4 || pIsomGlob->med_type==MCI_TYPE_3GP)
        {
#ifdef MPEG4_SUPPORT
            if(pIsomGlob->v_type==V_TYPE_MPEG4)
                dec_stop();
#endif
                gf_isom_close_file(pIsomGlob->file);
        }
        else
        {
#ifdef MPEG4_SUPPORT	
            if( pIsomGlob->v_type == V_TYPE_MPEG4 )
            {
                dec_stop();
            }
#endif	

            if(pIsomGlob->avi_p)
                avi_close_input_file(pIsomGlob->avi_p);
        }
    }
    mmc_MemFreeAll();
    return res;
*/
#endif
	
}

int32 mmc_vid_isom_pause (void)
{
	int res;
	int i;
	
	if(play_state!=VID_PLAY_STATE_PLAYING) 
	{
		mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VID_STOP_REQ);
		diag_printf("[VID]##WARNING##mmc_vid_isom_pause play_state= %d \n",play_state);
		return  ISOM_ERR_PLAY_STATE_FAIL;
	}
	
	do
	{
		res=COS_KillTimerEX(GetMCITaskHandle(MBOX_ID_MMC), MMC_VID_PLY_TIMER_ID);
		diag_printf("[VID]kill timer res=%d", res);
	}while(res!=1);
	
	
	if(pIsomGlob->open_audio)
	{
		AUD_FADE_OUT_FLAG=3;
		i=0;
		//fade out
		while(AUD_FADE_OUT_FLAG !=1)
		{
			sxr_Sleep(320);//about 20ms
			if(i>20)
				break;
			i++;
		}

		if((res=aud_StreamPause(audioItf, TRUE))!=AUD_ERR_NO)
		{
			diag_printf("[VID] ##WARNING##aud_StreamPause res=%d",res);
		}
		AUD_DATA_REQ=0;
		AUD_FADE_OUT_FLAG=0;
	}
	diag_printf("[VID] a_Pause_Pos= %x \n",pIsomGlob->a_Pause_Pos);
	
	play_state=VID_PLAY_STATE_PAUSE;

	//wait voc work over
	i=0;
	while(AV_DEC_STATE ||DISPLAY_STATE || VOC_WORK )
	{
		i++;
		if(i>=VOC_WORK_CONT)
		{	
			diag_printf("[VID]PAUSE WARNING! AUD VOC_WORK_CONT end! \n");
			break;
		}
		sxr_Sleep(1600);
	}
			
	return ISOM_ERR_OK;
}

int32 mmc_vid_isom_resume (void)
{
	int32 res=0;

	if(play_state!=VID_PLAY_STATE_PAUSE) 
	{
		diag_printf("[VID]##WARNING##mmc_vid_isom_resume play_state= %d \n",play_state);
		res=ISOM_ERR_PLAY_STATE_FAIL;
		goto resume_error;
	}

	if(pIsomGlob->a_Pause_DecReq==1)
	{

		pIsomGlob->a_Pause_DecReq=0;
	}
	if(pIsomGlob->open_audio)
	{
		int32 i,j;
		uint8 rd_id=A_EXBF_RD_ID;
		
		AUD_FADE_IN_FLAG=(pIsomGlob->a_HalfPcmBufFrame+2);//zero pcm num
		//mute current pcm play buffer
		if(PCM_ISR_FLAG==0)
		{
			memset((uint8*)pIsomGlob->a_PcmBufHalfPos, 0, pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame);
		}
		else
		{
			memset((uint8*)pIsomGlob->a_PcmBuf, 0, pIsomGlob->a_frameBytes*pIsomGlob->a_HalfPcmBufFrame);
		}
		//mute or fade in extern buffer
		for(i=0;i<EXTRA_PCM_BUF_NUM;i++)
		{
			if(aud_extra_pcmbuf_state[rd_id].done==1)
			{
				j=0;
				while(j<pIsomGlob->a_HalfPcmBufFrame)
				{
					if(AUD_FADE_IN_FLAG>1)
					{
						memset((uint8*)(aud_extra_pcmbuf_state[rd_id].pBufPos+ pIsomGlob->a_frameBytes*j), 0, pIsomGlob->a_frameBytes);
						AUD_FADE_IN_FLAG--;
					}
					else if(AUD_FADE_IN_FLAG==1)
					{
						audFadeIn((int16*)(aud_extra_pcmbuf_state[rd_id].pBufPos+ pIsomGlob->a_frameBytes*j), 
							pIsomGlob->a_frame_sample*pIsomGlob->a_channels);
						AUD_FADE_IN_FLAG=0;
					}
					
					j++;
				}
			}
			else
				break;
			
			rd_id+=1;
			if(rd_id>=EXTRA_PCM_BUF_NUM)rd_id=0;
		}

	
		if((res=aud_StreamPause(audioItf, FALSE))!=AUD_ERR_NO)
		{
			diag_printf( "##WARNING## aud_StreamPause err result= %d \n",res);
			res=ISOM_ERR_PCM_OPEN_FAIL;
			goto resume_error;
		}
		AUD_DATA_REQ=1;
	}

	COS_SetTimerEX(GetMCITaskHandle(MBOX_ID_MMC), MMC_VID_PLY_TIMER_ID, COS_TIMER_MODE_PERIODIC, pIsomGlob->v_timerPeriod);

	SEND_EVENT_NUM=0;
	DISPLAY_SEND_EVENT_NUM=0;
	
	play_state=VID_PLAY_STATE_PLAYING;
	diag_printf("[VID] mmc_vid_isom_resume");
	
	return ISOM_ERR_OK;
resume_error:
	
	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VID_STOP_REQ);
	diag_printf("[VID] mmc_vid_isom_resume err res=%d\n", res);
	return res;
	
}
int32 mmc_vid_isom_stop(void)
{
	int res;
	int i;


	if(play_state>=VID_PLAY_STATE_INVALID)
	{
		diag_printf("[VID]mmc_vid_isom_stop play_state=%d ERR!", play_state);
		return ISOM_ERR_PLAY_STATE_FAIL;
	}
	else if( play_state==VID_PLAY_STATE_IDLE)
	{
		diag_printf("[VID] mmc_vid_isom_stop play_state= %d \n",play_state);
		return ISOM_ERR_OK;
	}
		
	do
	{
		res=COS_KillTimerEX(GetMCITaskHandle(MBOX_ID_MMC), MMC_VID_PLY_TIMER_ID);
		diag_printf("[VID]kill timer res=%d", res);
	}while(res!=1);

	VID_ERR_FRAME_REC=0;
	VID_DEC_REQ=0;
#ifdef USE_KEY_SYNC
	KEY_SAMP_NUM=0;
	KEY_SAMP_TIME_WAIT=0;
	KEY_SAMP_TIME_START=0;
#endif
	if(pIsomGlob)
	{
		if(pIsomGlob->open_audio)
		{
			AUD_FADE_OUT_FLAG=3;
			i=0;
			//fade out
			while(AUD_FADE_OUT_FLAG !=1)
			{
				sxr_Sleep(320);//about 20ms
				if(i>20)
					break;
				i++;
			}
			
			if((res=aud_StreamStop(audioItf))!=AUD_ERR_NO)
			{
				diag_printf("[VID] ##WARNING##aud_StreamStop res=%d",res);
			}
			AUD_DATA_REQ=0;
			AUD_FADE_OUT_FLAG=0;
		}

		PCM_ISR_FLAG=0;
		pIsomGlob->a_Pause_DecReq=0;
	}
	diag_printf("[VID] mmc_vid_isom_stop");
	
	if(countv)
		diag_printf("[VID] avg  a_read,dec =%d ms ", (totalTa*1000+countv*(16384/2))/(countv*16384));
	if(countv)
		diag_printf("[VID] avg v_dec=%d ms ", (totalTv*1000+countv*(16384/2))/(countv*16384));
	
	
	play_state=VID_PLAY_STATE_IDLE;

	//wait voc work over
	i=0;
	while(AV_DEC_STATE ||DISPLAY_STATE || VOC_WORK )
	{
		i++;
		if(i>=VOC_WORK_CONT)
		{	
			diag_printf("[VID]STOP WARNING! AUD VOC_WORK_CONT end! \n");
			break;
		}
		sxr_Sleep(1600);
	}
	return ISOM_ERR_OK;
}
int32 mmc_vid_isom_close_file (void)
{
	int32 res;
	
	if(play_state!=VID_PLAY_STATE_IDLE) 
	{
		diag_printf("[VID] mmc_vid_isom_close_file err play_state= %d \n",play_state);
		mmc_vid_isom_stop();
	}


#if (defined MPEG4_SUPPORT || defined H263_SUPPORT)
	if(pZoom)
	{
		pZoom->rotate_mode=0;
		pZoom->zoom_mode=0;
	}
#endif

	if(VOC_OPEN)
	{
#if(CHIP_HAS_GOUDA == 1)
		if(pIsomGlob->a_type != A_TYPE_AMR)
		{
#endif
			vpp_AudioJpegDecClose();
#if(CHIP_HAS_GOUDA == 1)
		}
		else
		{
			vpp_SpeechClose();
		}
#endif
		VOC_OPEN=0;

		//resume voc frequency
		hwp_sysCtrl->Cfg_Clk_Voc = VOC_FREQ_STATE;

	}

#ifdef VID_XCPU_FREQ_SET
	if(XCPU_FREQ_STATE)
	{//free xcpu resource
		hal_SysRequestFreq(HAL_SYS_FREQ_CAMS, HAL_SYS_FREQ_32K, NULL);//  reuse cam id
		XCPU_FREQ_STATE=0;
	}
#endif

	if(pIsomGlob)
	{
		if(pIsomGlob->med_type==MCI_TYPE_MP4 || pIsomGlob->med_type==MCI_TYPE_3GP)
		{
#ifdef MPEG4_SUPPORT
			if(pIsomGlob->v_type==V_TYPE_MPEG4)
				dec_stop();
#endif
			gf_isom_close_file(pIsomGlob->file);
		}
#ifdef RM_SUPPORT
		else if(pIsomGlob->med_type==MCI_TYPE_RM)
		{
			rm_destroy(0);
			if(pIsomGlob->a_type==A_TYPE_COOK && pIsomGlob->a_Gecko2DecHdl)
			{
				Gecko2FreeDecoder(pIsomGlob->a_Gecko2DecHdl);
			}
		}
#endif
		else
		{
		#ifdef MPEG4_SUPPORT	
			if( pIsomGlob->v_type == V_TYPE_MPEG4 )
			{
				dec_stop();
			}
		#endif	

			if(pIsomGlob->avi_p)
				avi_close_input_file(pIsomGlob->avi_p);
		}
	}

#if(CHIP_HAS_GOUDA == 1)	
	while(hal_GoudaIsActive())
	{
		diag_printf("[VID]close gouda busy!");
		sxr_Sleep(160);
	}

    hal_GoudaVidLayerClose();
#endif

	play_state=VID_PLAY_STATE_INVALID;

	//wait video work over
	while(AV_DEC_STATE )
	{
		sxr_Sleep(1600);
		diag_printf("[VID]AV_DEC_STATE busy!");
	}
	
	mmc_MemFreeAll();
	pIsomGlob=NULL;
	VID_SET_MODE=0;
	
#ifndef USE_VOC_ROTATE
	
	lcdd_SetDirDefault();
#endif
	
	diag_printf("[VID]  mmc_vid_isom_close_file  \n");
	//play_state=VID_PLAY_STATE_INVALID;
	
#ifdef USE_OVERLAY
	res=hal_OverlayUnload(VID_OVERLAY_ID);
	if(res!=HAL_ERR_NO)
		diag_printf( "[VID]H263 VID_OVERLAY_ID=%d hal_OverlayUnload  res=%d", VID_OVERLAY_ID, res);
#endif

	 

	return ISOM_ERR_OK;
}

int32 mmc_vid_isom_get_info( IsomPlayInfo* pInfo)
{
	if(play_state>=VID_PLAY_STATE_INVALID)
	{
		diag_printf("[VID]mmc_vid_isom_set_mode play_state=%d ERR!", play_state);
		return ISOM_ERR_PLAY_STATE_FAIL;
	}

	/*get info*/
	pInfo->total_time=(pIsomGlob->total_time<500)?0:(pIsomGlob->total_time-500);
	pInfo->current_time= pIsomGlob->current_time;
	pInfo->total_frame_num=pIsomGlob->v_total_frame_num;
	pInfo->image_height=pIsomGlob->v_height; 
	pInfo->image_width=pIsomGlob->v_width; 
       pInfo->aud_channel_no = pIsomGlob->a_channels;
       pInfo->aud_sample_rate = pIsomGlob->a_sample_rate;

     switch(pIsomGlob->a_type)
	{
       case A_TYPE_AAC:
       case A_TYPE_AMR:
       case A_TYPE_MP3:
	case A_TYPE_PCM:
	case A_TYPE_COOK:
      		pInfo->track = MDI_VIDEO_TRACK_A_ONLY;
      		break;
	default:
		pInfo->track = MDI_VIDEO_TRACK_NONE;
		break;
    	}

     switch(pIsomGlob->v_type)
	{

       case V_TYPE_H263:
       case V_TYPE_MPEG4:
       case V_TYPE_MJPEG:
       //case V_TYPE_AVI_XVID:
       //case V_TYPE_AVI_DX50:
	//case V_TYPE_AVI_DIV3:   	
		
      		 if(pInfo->track == MDI_VIDEO_TRACK_A_ONLY)  
			pInfo->track =  MDI_VIDEO_TRACK_AV;
               else
		     pInfo->track =  MDI_VIDEO_TRACK_V_ONLY;   	
      		break;
	default:
		break;
    	}

	return ISOM_ERR_OK;
}

int32 mmc_vid_isom_set_mode( IsomPlayMode* pMode)
{
	int32 cleanBufFlag=0;
	
#if 1
	if(play_state>=VID_PLAY_STATE_INVALID)
	{
		diag_printf("[VID]mmc_vid_isom_set_mode play_state=%d ERR!", play_state);
		return ISOM_ERR_PLAY_STATE_FAIL;
	}

	if(pMode->rotate !=pIsomGlob->v_rotate || pMode->zoom_width != pIsomGlob->v_display_width ||
		pMode->zoom_height != pIsomGlob->v_display_height )
	{
		cleanBufFlag=1;
	}

	/*set mode*/
	if(pMode->rotate==Rotate_with_0)
	{
#ifndef USE_VOC_ROTATE
		lcdd_SetDirDefault();
		pIsomGlob->v_rotate=Rotate_with_0;
#else
        	pIsomGlob->v_rotate=Rotate_with_0;
#endif
		pIsomGlob->v_lcd_start_x=pMode->lcd_start_x;
		pIsomGlob->v_lcd_start_y=pMode->lcd_start_y;
		pIsomGlob->v_lcd_size_x=MMC_LcdWidth;
		pIsomGlob->v_lcd_size_y=MMC_LcdHeight;
	}
	else
	{
		if(pMode->zoom_width==pIsomGlob->v_lcd_size_y && pMode->zoom_height==pIsomGlob->v_lcd_size_x)
		{
			DISPLAY_INTERFACE=0;
		}

#ifndef USE_VOC_ROTATE
		lcdd_SetDirRotation();
		pIsomGlob->v_rotate=Rotate_with_0;
		pIsomGlob->v_lcd_start_x=pMode->lcd_start_y;
		pIsomGlob->v_lcd_start_y=MMC_LcdWidth-(pMode->lcd_start_x+pMode->zoom_height );
		pIsomGlob->v_lcd_size_x=MMC_LcdHeight;
		pIsomGlob->v_lcd_size_y=MMC_LcdWidth;
#else
       	pIsomGlob->v_rotate=pMode->rotate;
		pIsomGlob->v_lcd_start_x=pMode->lcd_start_x;
		pIsomGlob->v_lcd_start_y=pMode->lcd_start_y;
		pIsomGlob->v_lcd_size_x=MMC_LcdWidth;
		pIsomGlob->v_lcd_size_y=MMC_LcdHeight;
#endif
	}
	
	//pIsomGlob->v_lcd_start_x=pMode->lcd_start_x;
	//pIsomGlob->v_lcd_start_y=pMode->lcd_start_y;
	pIsomGlob->v_display_width=pMode->zoom_width;
	pIsomGlob->v_display_height=pMode->zoom_height; 
	pIsomGlob->v_cutX=pMode->cutX; 
	pIsomGlob->v_cutY=pMode->cutY; 
	pIsomGlob->v_cutW=pMode->cutW; 
	pIsomGlob->v_cutH=pMode->cutH; 

	if(pIsomGlob->v_lcd_start_x<0)pIsomGlob->v_lcd_start_x=0;
	if(pIsomGlob->v_lcd_start_x>=pIsomGlob->v_lcd_size_x)pIsomGlob->v_lcd_start_x=pIsomGlob->v_lcd_size_x;
	if(pIsomGlob->v_lcd_start_y<0)pIsomGlob->v_lcd_start_y=0;
	if(pIsomGlob->v_lcd_start_y>=pIsomGlob->v_lcd_size_y)pIsomGlob->v_lcd_start_y=pIsomGlob->v_lcd_size_y;

	if(pIsomGlob->v_rotate==Rotate_with_0)
	{
		if(pIsomGlob->v_cutX<0)pIsomGlob->v_cutX=0;
		if(pIsomGlob->v_cutX>=pIsomGlob->v_display_width)pIsomGlob->v_cutX=pIsomGlob->v_display_width;
		if(pIsomGlob->v_cutY<0)pIsomGlob->v_cutY=0;
		if(pIsomGlob->v_cutY>=pIsomGlob->v_display_height)pIsomGlob->v_cutY=pIsomGlob->v_display_height;
		if(pIsomGlob->v_cutW<0)pIsomGlob->v_cutW=0;
		if(pIsomGlob->v_cutW>=pIsomGlob->v_display_width)pIsomGlob->v_cutW=pIsomGlob->v_display_width;
		if(pIsomGlob->v_cutH<0)pIsomGlob->v_cutH=0;
		if(pIsomGlob->v_cutH>=pIsomGlob->v_display_height)pIsomGlob->v_cutH=pIsomGlob->v_display_height;

		if(pIsomGlob->v_lcd_start_x+pIsomGlob->v_cutW >= pIsomGlob->v_lcd_size_x)
			pIsomGlob->v_cutW= pIsomGlob->v_lcd_size_x-pIsomGlob->v_lcd_start_x;
		if(pIsomGlob->v_lcd_start_y+pIsomGlob->v_cutH >= pIsomGlob->v_lcd_size_y)
			pIsomGlob->v_cutH= pIsomGlob->v_lcd_size_y-pIsomGlob->v_lcd_start_y;

	       //reuse lcd buffer for v_ImageBuf2.
	       if(pIsomGlob->v_display_width==pIsomGlob->v_lcd_size_x && 
		   	pIsomGlob->v_cutW==pIsomGlob->v_lcd_size_x &&
		   	pIsomGlob->v_lcd_start_x==0)//voc directly output to lcd buffer
	       {
			NEED_BLEND_IMG=0;
			pIsomGlob->v_ImageBuf2=((unsigned short*)get_lcd_frame_buffer_address())+
				pIsomGlob->v_lcd_size_x*pIsomGlob->v_lcd_start_y;
	       }
		else//voc ouput to v_ImageBuf then copy to lcd buffer.
		{
			NEED_BLEND_IMG=1;
			pIsomGlob->v_ImageBuf2=((unsigned short*)get_lcd_frame_buffer_address())+
				pIsomGlob->v_lcd_size_x*pIsomGlob->v_lcd_start_y+pIsomGlob->v_lcd_start_x;
		}
	}
	else
	{
		if(pIsomGlob->v_cutX<0)pIsomGlob->v_cutX=0;
		if(pIsomGlob->v_cutX>=pIsomGlob->v_display_width)pIsomGlob->v_cutX=pIsomGlob->v_display_width;
		if(pIsomGlob->v_cutY<0)pIsomGlob->v_cutY=0;
		if(pIsomGlob->v_cutY>=pIsomGlob->v_display_height)pIsomGlob->v_cutY=pIsomGlob->v_display_height;
		if(pIsomGlob->v_cutW<0)pIsomGlob->v_cutW=0;
		if(pIsomGlob->v_cutW>=pIsomGlob->v_display_width)pIsomGlob->v_cutW=pIsomGlob->v_display_width;
		if(pIsomGlob->v_cutH<0)pIsomGlob->v_cutH=0;
		if(pIsomGlob->v_cutH>=pIsomGlob->v_display_height)pIsomGlob->v_cutH=pIsomGlob->v_display_height;

		if(pIsomGlob->v_lcd_start_x+pIsomGlob->v_cutH >= pIsomGlob->v_lcd_size_x)
			pIsomGlob->v_cutH= pIsomGlob->v_lcd_size_x-pIsomGlob->v_lcd_start_x;
		if(pIsomGlob->v_lcd_start_y+pIsomGlob->v_cutW >= pIsomGlob->v_lcd_size_y)
			pIsomGlob->v_cutW= pIsomGlob->v_lcd_size_y-pIsomGlob->v_lcd_start_y;

	       //reuse lcd buffer for v_ImageBuf2.
	       if(pIsomGlob->v_display_height==pIsomGlob->v_lcd_size_x && 
		   	pIsomGlob->v_cutH==pIsomGlob->v_lcd_size_x &&
		   	pIsomGlob->v_lcd_start_x==0)//voc directly output to lcd buffer
	       {
			NEED_BLEND_IMG=0;
			pIsomGlob->v_ImageBuf2=((unsigned short*)get_lcd_frame_buffer_address())+
				pIsomGlob->v_lcd_size_x*pIsomGlob->v_lcd_start_y;
	       }
		else//voc ouput to v_ImageBuf then copy to lcd buffer.
		{
			NEED_BLEND_IMG=1;
			pIsomGlob->v_ImageBuf2=((unsigned short*)get_lcd_frame_buffer_address())+
				pIsomGlob->v_lcd_size_x*pIsomGlob->v_lcd_start_y+pIsomGlob->v_lcd_start_x;
		}
	}

	if(pIsomGlob->v_display_width>0 && pIsomGlob->v_display_height>0)
	{
		pIsomGlob->v_cutX_origin=(int16)((uint32)pIsomGlob->v_width*(uint32)pIsomGlob->v_cutX/(uint32)pIsomGlob->v_display_width);
		pIsomGlob->v_cutX_origin=(pIsomGlob->v_cutX_origin+0x7)&0xFFF0;//multiple of 16 for YUV
		pIsomGlob->v_cutY_origin=(int16)((uint32)pIsomGlob->v_height*(uint32)pIsomGlob->v_cutY/(uint32)pIsomGlob->v_display_height);
		pIsomGlob->v_cutY_origin=(pIsomGlob->v_cutY_origin+0x7)&0xFFF0;//multiple of 16 for YUV
		pIsomGlob->v_cutW_origin=(int16)((uint32)(pIsomGlob->v_width)*(uint32)(pIsomGlob->v_cutW)/(uint32)(pIsomGlob->v_display_width));
		if(pIsomGlob->v_cutX_origin+pIsomGlob->v_cutW_origin > pIsomGlob->v_width)
			pIsomGlob->v_cutW_origin=(pIsomGlob->v_width&0xFFF0)-pIsomGlob->v_cutX_origin;
		pIsomGlob->v_cutW_origin=(pIsomGlob->v_cutW_origin+0x7)&0xFFF0;//multiple of 16 for YUV
		pIsomGlob->v_cutH_origin=(int16)((uint32)pIsomGlob->v_height*(uint32)pIsomGlob->v_cutH/(uint32)pIsomGlob->v_display_height);
		if(pIsomGlob->v_cutY_origin+pIsomGlob->v_cutH_origin > pIsomGlob->v_height)
			pIsomGlob->v_cutH_origin=(pIsomGlob->v_height&0xFFF0)-pIsomGlob->v_cutY_origin;
		pIsomGlob->v_cutH_origin=(pIsomGlob->v_cutH_origin+0x7)&0xFFF0;//multiple of 16 for YUV
	}

	IMAGE_W_INBUF0=0;
	IMAGE_H_INBUF0=0;
	IMAGE_W_INBUF1=0;
	IMAGE_H_INBUF1=0;

	if(cleanBufFlag)
	{
#if (CHIP_HAS_GOUDA != 1)		
		memset(pIsomGlob->v_ImageBuf, 0, (pIsomGlob->v_lcd_size_x*pIsomGlob->v_lcd_size_y*2));
		//memset(pIsomGlob->v_ImageBuf2, 0, (pIsomGlob->v_lcd_size_x*pIsomGlob->v_lcd_size_y*2));
#endif		
	}

#if (CHIP_HAS_GOUDA != 1)		
	vocDecVid.Zoomed_width=pIsomGlob->v_display_width;
	vocDecVid.Zoomed_height=pIsomGlob->v_display_height;
#ifdef USE_VOC_ROTATE
	vocDecVid.zoom_mode=pIsomGlob->v_rotate;
#endif
#endif
	VID_SET_MODE=1;

#endif
	diag_printf("[VID]mmc_vid_isom_set_mode lcd_start_x=%d  lcd_start_y=%d \n zoom_width=%d  zoom_height=%d rotate=%d \n",
	pMode->lcd_start_x, pMode->lcd_start_y, pMode->zoom_width,pMode->zoom_height, pMode->rotate);
	diag_printf("[VID]cutX=%d cutY=%d cutW=%d cutH=%d\n",	
			pMode->cutX,pMode->cutY,pMode->cutW,pMode->cutH);
	
	diag_printf("[VID]real lcd_start_x=%d  lcd_start_y=%d \n zoom_width=%d  zoom_height=%d rotate=%d \n",
	pIsomGlob->v_lcd_start_x, pIsomGlob->v_lcd_start_y, pIsomGlob->v_display_width,pIsomGlob->v_display_height, pIsomGlob->v_rotate);
	diag_printf("[VID]cutX=%d cutY=%d cutW=%d cutH=%d\n",	
			pIsomGlob->v_cutX,pIsomGlob->v_cutY,pIsomGlob->v_cutW,pIsomGlob->v_cutH);
	diag_printf("[VID]origin cutX=%d cutY=%d cutW=%d cutH=%d\n",	
			pIsomGlob->v_cutX_origin,pIsomGlob->v_cutY_origin,pIsomGlob->v_cutW_origin,pIsomGlob->v_cutH_origin);

	return ISOM_ERR_OK;
}


int32 Video_IsomUserMsg(COS_EVENT *pVidEv)
{
    int32 res;

	AV_DEC_STATE=1;
	
    switch(pVidEv->nEventId)
    {
        case MSG_VID_AUD_DEC_REQ:
            SEND_EVENT_NUM=0;

            res=vid_av_dec_frame();
			
            if(res!=ISOM_ERR_OK && res!=ISOM_ERR_PLAY_STATE_FAIL)
            {
                mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VID_STOP_REQ);
                diag_printf("[VID] UserMsg vid_audio_dec_frame err=%d \n", res);
            }
			
            break;

        case MSG_VID_IMG_DEC_REQ:
            SEND_EVENT_NUM=0;

            res=vid_av_dec_frame();
			
            if(res!=ISOM_ERR_OK && res!=ISOM_ERR_PLAY_STATE_FAIL)
            {
                mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VID_STOP_REQ);
                diag_printf("[VID] UserMsg vid_video_dec_frame err=%d \n", res);
            }

            break;

        case EV_TIMER:
            switch(pVidEv->nParam1)
            {
                case MMC_VID_PLY_TIMER_ID:
                    VID_DEC_REQ++;

                    if(SEND_EVENT_NUM==0)
                    {

                        res = vid_av_dec_frame();
						
                        if(res!=ISOM_ERR_OK && res!=ISOM_ERR_PLAY_STATE_FAIL)
                        {
                            mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VID_STOP_REQ);
                            diag_printf("[VID] UserMsg vid_video_dec_frame err=%d \n", res);
                        }
                    }
                    //else
                    //	diag_printf( "[VID]Skip Timer");
                    break;

                default:

                    break;
            }
            break;

        case MSG_VID_DISPLAY_REQ:
            //vid_display();
            break;

        case MSG_VID_DISPLAY_INTERFACE_REQ:   

            if(DISPLAY_SEND_EVENT_NUM)
            {
                DISPLAY_SEND_EVENT_NUM=0;
                DISPLAY_INTERFACE=1;
                vid_display();
            }
            else
            {
                if(play_state==VID_PLAY_STATE_PLAYING)
                {
                    DISPLAY_INTERFACE=1;
                    vid_display();
                }
		  else if(play_state==VID_PLAY_STATE_PAUSE)
		  {
			if(!IMAGE_W_INBUF1 && !IMAGE_H_INBUF1 && !IMAGE_W_INBUF0 && !IMAGE_H_INBUF0)
			{//display last image again.
			
				pIsomGlob->v_ImageBuf_HalfFlag=1-pIsomGlob->v_ImageBuf_HalfFlag;
				if(pIsomGlob->v_ImageBuf_HalfFlag==0)
				{
					IMAGE_W_INBUF1=pIsomGlob->v_display_width;
					IMAGE_H_INBUF1=pIsomGlob->v_display_height;
				}
				else if(pIsomGlob->v_ImageBuf_HalfFlag==1)
				{
					IMAGE_W_INBUF0=pIsomGlob->v_display_width;
					IMAGE_H_INBUF0=pIsomGlob->v_display_height;
				}
			}

			DISPLAY_INTERFACE=2;
			vid_display();
		  }
                else if(play_state<VID_PLAY_STATE_INVALID)
                {
                    DISPLAY_INTERFACE=2;
                    pIsomGlob->v_ImageBuf_HalfFlag=1;
	
                    vid_display();
                }
            }
            break;

        case MSG_VID_STOP_REQ:
        //  if(play_state!=VID_PLAY_STATE_IDLE)
            {
                if(pIsomGlob->open_audio)
                    aud_StreamStop(audioItf);
					
                COS_KillTimerEX(GetMCITaskHandle(MBOX_ID_MMC), MMC_VID_PLY_TIMER_ID);
	
                pIsomGlob->current_time=0;
                pIsomGlob->vid_play_finish_callback(0);
            }
            break;

        default:
            break;
    }

	AV_DEC_STATE=0;

    return ISOM_ERR_OK;
    
}


#endif

