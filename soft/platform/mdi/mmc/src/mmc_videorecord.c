// //////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2002-2007, Coolsand Technologies, Inc.
// All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmisison of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
// 
// FILENAME: mmc_videorecord.c
// 
// DESCRIPTION:
// TODO... 
// 
// REVISION HISTORY:
// NAME            DATE            REMAKS
// TODO... 
// Zhaoy                           create framework
// Baoyh           20071008        implement
// 
// //////////////////////////////////////////////////////////////////////////////

// #include "switch.h"
// #if defined __VIDEO_RECORD__

// //////////////////////////////////////////////////////////////////////////////
// DESIGN THOUGHT DESCRIPTOIN 
// //////////////////////////////////////////////////////////////////////////////
// TODO.. 

// //////////////////////////////////////////////////////////////////////////////
// DECLARE HEAD FILES
// //////////////////////////////////////////////////////////////////////////////
// TODO... 

#include "mmc_videorecord.h"
#include "hal_error.h"
#include "fs.h"
#include "event.h"
#include "mmc_timer.h"
#include "vpp_audiojpeg_enc.h"
#include "mmf_jpegamrring_avi_enc.h"
#include "mmc_camera.h" // chenhe add for get a new filename
#include "mci.h"
#include "dm.h"

#include "ars_m.h"
#include "camera_m.h"
#include "hal_timers.h"
#include "lcdd_m.h"
#include "mcip_debug.h"
#include "string.h"
#include "fs_asyn.h"
//#include "hal_overlay.h"

#include "global.h"
#include "calib_m.h"


#if (CHIP_HAS_GOUDA == 1)

#include "hal_gouda.h"
#include "sxr_tls.h"

#define VSRECBUF_LEN 900 //mutiple of 5 ,2 or 3.
#define AVI_SIZE_MIN 1000
#define YUV2RGB_ASM
#define PCM_REC_BUF_SIZE  (160*2*10)
#define AMR_TRANS_COUNT 5

extern void set_mmc_camera_preview();

VIDREC_CONTEXT g_VidRecCntx;
FS_INFO g_FsVidRecInfo;

uint8* vidrec_pAvi_Buff;
uint8* vidrec_avi_out;
uint8* RecPcmBuffer;
uint8* AMR_buffer;
uint8* vidrec_JpegBuff0;
uint8* vidrec_JpegBuff1;
uint8* linebuff_asm;

//int8* gp_VideoRecordAFrameJPEGBuf;
int8* gp_VidRec_Prebuf;

int32 vidrec_VocFirstSchedule;
int32 vidrec_AviNuminBuff;

volatile mmcModeStatus videoRecordState = VidrecStopStatus;

uint8 vidrec_amr_first_frame[32* 30 + 6];
uint8 vidrec_avi_head[AVI_HEAD_SIZE];
uint8* vidrec_avi_end;  

volatile uint8 *AMR_buffer_tail;
volatile uint8 *AMR_buffer_heard;
volatile uint8 *vidrec_pJpegAviIn;
volatile uint8 *vidrec_pJpegVocOut;

volatile uint32* vidrec_pJpeg_sizeVoc=NULL;
volatile uint32* vidrec_pJpeg_sizeAvi=NULL;

volatile int32 Msg_Wait_Num = 0;
volatile int32 AMR_Data_Store = 0;
volatile int32 Vidrec_Sensor_Seq = 0;
volatile int32 Cam_Data_Valid = -1;
volatile int32 Voc_JpegEnc_Busy = 0;
volatile int32 Voc_AmrEnc_Busy = 0;
volatile int32 Voc_Interrupt_From = -1;
volatile int32 Voc_Amr_Count = 0;
volatile int32 Pcm_StartPosition=0;
volatile int32 Voc_Jpg_Need = 0;
volatile int32 Voc_Amr_Need = 0;
volatile int32 Voc_JpegOut_Seq = 0;
volatile int32 VR_Init=0;
volatile int32 VR_Amr_Init = 0;

volatile uint32 Avi_Out_Index=0;
volatile uint32 g_AMR_DataInCount=0;

int32 vidrec_Jpeg0_size;
int32 vidrec_Jpeg1_size;

extern UINT16* sensorbuf;
extern UINT16* blendBuffer;
extern CAM_SIZE_T MY_NPIX;
extern uint16 startPos;
extern uint16 srcwidth;
extern uint16 srcheight;
extern uint16 cutwidth;
extern uint16 cutheight;
extern uint16  bset;
extern int video_i;
extern uint32 cam_wbValue;
extern AUD_ITF_T audioItf;
extern AVI_video_index* avi_viidx;  // bao debug: design the index table according 500 frame 
extern volatile CAM_INFO_STRUCT  CameraInfo;
extern volatile int32 Effect_Set_Valid;

#if (CSW_EXTENDED_API_LCD == 1)
extern LCDD_FBW_T displayFbw;
#endif

static int32 vidrec_AmrEncFirstSchedule = 0;
extern void VidRec_bilinear_zoom(char* srcbuf,int src_w,int src_h,int cut_w,int cut_h,char* decbuf,int dec_w,int dec_h,char* linebuff);

void VidRec_EncodeFrameIsr(HAL_VOC_IRQ_STATUS_T* irqStatus)
{
    MCI_TRACE (MCI_VIDREC_TRC,0,"------------Voc_Interrupt_From=%d,Voc_Jpg_Need=%d,Voc_Amr_Need =%d,Voc_Amr_Count=%d",Voc_Interrupt_From,Voc_Jpg_Need,Voc_Amr_Need,Voc_Amr_Count);

    if ((vidrec_VocFirstSchedule != 0)&&(videoRecordState == VidrecRecordStatus))
    {
        if(Voc_Interrupt_From == 0)
        {
            Voc_Interrupt_From = -1;
            Avi_Out_Index += 1;
            vpp_AudioJpeg_ENC_OUT_T VocOutputStatus; 
            vpp_AudioJpegEncStatus(&VocOutputStatus);
            *vidrec_pJpeg_sizeVoc = VocOutputStatus.output_len;// + 607;

            if(Voc_JpegOut_Seq == 0)
            {
                Voc_JpegOut_Seq = 1;
                vidrec_pJpegVocOut = vidrec_JpegBuff0;
                vidrec_pJpegAviIn = vidrec_JpegBuff1;
                vidrec_pJpeg_sizeVoc = &vidrec_Jpeg0_size;
                vidrec_pJpeg_sizeAvi = &vidrec_Jpeg1_size;				
            }
            else
            {
                Voc_JpegOut_Seq = 0;
                vidrec_pJpegVocOut = vidrec_JpegBuff1;
                vidrec_pJpegAviIn = vidrec_JpegBuff0;
                vidrec_pJpeg_sizeVoc = &vidrec_Jpeg1_size;		
                vidrec_pJpeg_sizeAvi = &vidrec_Jpeg0_size;				
            }

            Voc_JpegEnc_Busy = 0;

            ///////////voc processing is over and try to check if there's waiting one!//////////////

            if (Voc_Amr_Need > 0)
                VidRec_Pcm2Amr(); 
        }
        else if (Voc_Interrupt_From == 1)
        {
            Voc_Interrupt_From = -1;    
            Voc_AmrEnc_Busy = 0;
            AMR_Data_Store++;

            if(Voc_Amr_Count >= AMR_TRANS_COUNT)
            {
                Voc_Amr_Count = 0;
                Voc_Amr_Need = 0;
            }

            if (Voc_Jpg_Need == 1 && AMR_Data_Store >= 5)
            {
                Voc_JpegEnc_Busy = 1;
                Voc_Interrupt_From = 0;
                Voc_Jpg_Need = 0;

                VidRec_JPEGEncode((int32 *)(sensorbuf+srcwidth*((srcheight-cutheight)>>1)+((srcwidth-cutwidth)>>1)),
                            g_VidRecCntx.settingInfo.imageWidth,g_VidRecCntx.settingInfo.imageHeight);
            }
            else if (Voc_Amr_Need > 0)
            {
                VidRec_Pcm2Amr();
            }
            else
            {
                MCI_TRACE (MCI_VIDREC_TRC,0,"------------VOC IDLE!,Voc_Amr_Need=%d",Voc_Amr_Need);
            }
        }
    }
    else
        vidrec_VocFirstSchedule = 1;

    //MCI_TRACE (MCI_VIDREC_TRC,0,"------------Voc_Interrupt over!");	
}

void VidRec_Pcm2Amr(void)
{     
    if ((Voc_JpegEnc_Busy == 0)&&Voc_AmrEnc_Busy == 0)//&&(!((PCM_BUF_INT=1)&&(VOC_AMRENC_BUSY))))
    {
        vpp_AudioJpeg_ENC_IN_T   audio_amrenc;

        Voc_AmrEnc_Busy = 1;
        Voc_Interrupt_From = 1;
        Voc_Amr_Count++;    
        //Voc_Amr_Need=1;

        audio_amrenc.inStreamBufAddr = (INT32*)(RecPcmBuffer+(Pcm_StartPosition*PCM_REC_BUF_SIZE/2)+(Voc_Amr_Count-1)*320);
        audio_amrenc.outStreamBufAddr = (INT32*)AMR_buffer_tail;
        AMR_buffer_tail += 32;

        if((AMR_buffer_tail-AMR_buffer) >= 32 * VSRECBUF_LEN)
            AMR_buffer_tail = AMR_buffer;

        audio_amrenc.mode = MMC_AMR_ENCODE;
        audio_amrenc.channel_num = 1;   
        audio_amrenc.SampleRate = HAL_AIF_FREQ_8000HZ;
        audio_amrenc.reset = 0;

        if (VR_Amr_Init == 0)
        {
            VR_Amr_Init = 1;
            audio_amrenc.reset = 1;
        }

        if(HAL_ERR_NO != vpp_AudioJpegEncScheduleOneFrame(&audio_amrenc)) 
        {
            Voc_AmrEnc_Busy = 0;
            diag_printf ("Failed to call VOC in VidRec_Pcm2Amr :%d th", Voc_Amr_Count);
        }
    }
    else
    {
        //Voc_Amr_Need=1;
        MCI_TRACE (MCI_VIDREC_TRC,0,
                "------%d pcm2amr need waiting,Voc_JpegEnc_Busy=%d,Voc_AmrEnc_Busy=%d",
                Voc_Amr_Need, Voc_JpegEnc_Busy, Voc_AmrEnc_Busy);
    }
}

error_type vidrec_EncodeRun(VIDREC_CONTEXT* amr_in)
{
    int32 count = 0;

    // ////////////////// JPEG encode a frame finish ////////////////////////////////////

    amr_in->JPEGEncodeFrameCnt++;

    //  check if finish the whole video recored 
    if (((g_VidRecCntx.curFileSize + VIDREC_CHECK_AVI_FILE_SIZE) >= g_VidRecCntx.settingInfo.fileSizePermit * 1024)
            &&(g_VidRecCntx.settingInfo.fileSizePermit > 0))
    {
        diag_printf("[Vidrec Encode Run] file size is reach, file size = %d \n", g_VidRecCntx.curFileSize);
        VidRec_RecordStop();
        g_VidRecCntx.vidrec_callback(MDI_RES_VDOREC_REACH_SIZE_LIMIT);
        diag_printf("Video Record Finished, REACH_SIZE_LIMIT!\n");    
        return MDI_RES_VDOREC_REACH_SIZE_LIMIT; 
    }

    {
        int32 avi_frame_length = 0;
        JpegAmr_Avi_Enc avi_enc_info;
        static uint8 amrHead[6] = {0x23, 0x21, 0x41, 0x4d, 0x52, 0x0a};

        /*************avi frame integrate***************************/
        if ((videoRecordState == VidrecRecordStatus) &&
                    (g_VidRecCntx.JPEGEncodeFrameCnt > 0) &&
                    (g_VidRecCntx.settingInfo.IfRecordAudio//&& g_AMR_DataInCount--)
                    || ((!g_VidRecCntx.settingInfo.IfRecordAudio) 
                    && (g_VidRecCntx.RGBBufStatus[0] == RGB_BUF_NULL))))
        {
            if (!g_VidRecCntx.settingInfo.IfRecordAudio)
            {
                //diag_printf( "[No sound]: AVI frame package now. \n" );        
                g_VidRecCntx.pingInDataBlockNum = VIDREC_AMR_FRAME_NUM_IN_A_PACK;
                //memset(vidrec_outbuf,0x00,VIDREC_AMRENC_OUTBUF_SIZE);
            }

            /**********************VIDREC_AMR_FRAME_MAX_CNT_IN_A_PACK is just for test of zhangl**************************/   

            g_VidRecCntx.pingInDataBlockNum = VIDREC_AMR_FRAME_NUM_IN_A_PACK;
	
            if(AMR_Data_Store<5)
            {
                diag_printf("######discard one jpeg AMR_Data_Store=%d",AMR_Data_Store);
                return NO_ERRO;
            }
			
            //if(vidrec_AviNuminBuff < 6)
            if(vidrec_AviNuminBuff < 3)
            {
                avi_enc_info.Avi_outBuf = vidrec_avi_out;
                vidrec_avi_out += 20*1024; 
                //if(vidrec_avi_out > (vidrec_pAvi_Buff+100*1024)) vidrec_avi_out = vidrec_pAvi_Buff;
                if(vidrec_avi_out > (vidrec_pAvi_Buff+40*1024)) vidrec_avi_out = vidrec_pAvi_Buff;
                vidrec_AviNuminBuff++;
                // MCI_TRACE (MCI_VIDREC_TRC,0," ++vidrec_AviNuminBuff =%d \n",vidrec_AviNuminBuff);
            }
            else
            {
                diag_printf("######discard avi,buffer overflow!vidrec_AviNuminBuff =%d,AMR_Data_Store=%d",vidrec_AviNuminBuff,AMR_Data_Store);
                AMR_Data_Store -= g_VidRecCntx.pingInDataBlockNum;
                AMR_buffer_heard +=  g_VidRecCntx.pingInDataBlockNum * 32;
                if (AMR_buffer_heard >= (AMR_buffer+VSRECBUF_LEN*32))
                {
                    AMR_buffer_heard = AMR_buffer;
                }		  
                return NO_ERRO;
            }
			
            //avi_enc_info.Avi_outBuf = vidrec_avi_out;
            avi_enc_info.Jpeg_Frame_Size = * vidrec_pJpeg_sizeAvi;
            avi_enc_info.Jpeg_inBuf = (uint8*) vidrec_pJpegAviIn + 1;
			
            if (g_VidRecCntx.AVIFrameCnt == 0)
            {
                memcpy((void*)vidrec_amr_first_frame, (const void*)amrHead, 6);
                memcpy((void*)(vidrec_amr_first_frame + 6), (const void*)AMR_buffer_heard, g_VidRecCntx.pingInDataBlockNum* 32);

                avi_enc_info.Amr_Data_Size = g_VidRecCntx.pingInDataBlockNum * 32 + 6;
                avi_enc_info.Amr_inBuf = vidrec_amr_first_frame;
            }
            else
            {
                avi_enc_info.Amr_Data_Size = g_VidRecCntx.pingInDataBlockNum * 32;
                avi_enc_info.Amr_inBuf =(uint8*) AMR_buffer_heard;
            }

            /****************have no much enough amr data to integrate this frame, just discard this jpeg*********************/

            avi_frame_length = MMF_Avi_Enc_Frame(&avi_enc_info);
            g_VidRecCntx.AVIFrameCnt++;
            g_VidRecCntx.curFileSize += avi_frame_length + VIDREC_AVI_SINGLE_INDEX_LEN;

            AMR_Data_Store -= g_VidRecCntx.pingInDataBlockNum;
            AMR_buffer_heard += g_VidRecCntx.pingInDataBlockNum * 32;
            if (AMR_buffer_heard >= (AMR_buffer+VSRECBUF_LEN*32))
            {
                AMR_buffer_heard = AMR_buffer;
            }
	  
            //MCI_TRACE (MCI_VIDREC_TRC,0,"------begin write %d byte from:%x,at:%d\n",avi_frame_length,avi_enc_info.Avi_outBuf,hal_TimGetUpTime());
            //count = FS_Write(g_VidRecCntx.fileHandle, vidrec_avi_out, (int32)avi_frame_length);
            count = avi_frame_length;
            if(0 != FS_AsynWriteReq(g_VidRecCntx.fileHandle, avi_enc_info.Avi_outBuf, (int32)avi_frame_length,(FS_ASNY_FUNCPTR)VidRec_FS_AsynWrite_CB))
            {
                diag_printf("------FS write fail!");
                vidrec_AviNuminBuff--;
                VidRec_RecordStop();
                g_VidRecCntx.vidrec_callback(MDI_RES_VDOREC_ERR_FAILED);
                return MDI_RES_VDOREC_ERR_FAILED;
            }

            g_FsVidRecInfo.iUsedSize +=avi_frame_length;
            if ((g_FsVidRecInfo.iUsedSize + AVI_TF_MIN*1024) >= g_FsVidRecInfo.iTotalSize)
            {
                VidRec_RecordStop();
                g_VidRecCntx.vidrec_callback(MDI_RES_VDOREC_DISK_FULL);
                diag_printf("Video Record Finished, DISK_FULL!\n");
                return MDI_RES_VDOREC_DISK_FULL;
            }
            if(video_i >=VIDREC_AVI_FRAME_MAX_COUNT)
            {
                VidRec_RecordStop();
                g_VidRecCntx.vidrec_callback(MDI_RES_VDOREC_REACH_SIZE_LIMIT);
                diag_printf("Video Record File Reach Maximum!\n");
                return MDI_RES_VDOREC_REACH_SIZE_LIMIT;
            }

            if (count != avi_frame_length)
            {
                diag_printf("Video Record Finished, Write file error!\n");
                VidRec_RecordStop();
                g_VidRecCntx.vidrec_callback(MDI_RES_VDOREC_ERR_FAILED);
                return MDI_RES_VDOREC_ERR_FAILED;
            }
        }
    }

    amr_in->VocState = VIDREC_VOC_IDLE;

    //MCI_TRACE (MCI_VIDREC_TRC,0,"******* end to write avi: %d--*********",hal_TimGetUpTime());
    return NO_ERRO;
}

error_type VidRec_Init(uint8* pFileName, VIDREC_CONTEXT* amr_in)
{
    JpegAmr_Avi_Init Avi_Init_Info;
    HAL_ERR_T voc_ret = 0;
    CALIB_CALIBRATION_T* calibPtr = calib_GetPointers();

    /**************init avi file info*********************************/
    Avi_Init_Info.Amr_BitRate      = 12800;
    Avi_Init_Info.Audio_Bits         = 16;
    Avi_Init_Info.Audio_Channel   = 1;
    Avi_Init_Info.Audio_SampleRate = 8000;
    Avi_Init_Info.Image_Height    = g_VidRecCntx.settingInfo.imageHeight;
    Avi_Init_Info.Image_Width     = g_VidRecCntx.settingInfo.imageWidth;
    Avi_Init_Info.Video_fps          = VIDREC_AVI_VIDEO_FRAME_RATE;
    if (MMF_Avi_Enc_Init(&Avi_Init_Info) == FUNC_OK)
        MCI_TRACE (MCI_VIDREC_TRC,0,"Avi Encoder init ok\n");

    // init video record context
    //amr_in->VppCodecMode       = AMR_ENCODE;
    amr_in->AMREncodeUnitCnt  = 0;
    amr_in->AVIFrameCnt = 0;
    amr_in->curFileSize     = AVI_HEAD_SIZE + VIDREC_AVI_INDEX_TABLE_HEAD_LEN;
    amr_in->lastRecordPeriod = 0;
    amr_in->JPEGEncodeFrameCnt = 0; // set init value as 0!
    amr_in->RGBBufStatus[0] = RGB_BUF_NULL;
    amr_in->RGBBufStatus[1] = RGB_BUF_NULL;
    amr_in->VocState        = VIDREC_VOC_IDLE;

    vidrec_VocFirstSchedule = 0;
    vidrec_AmrEncFirstSchedule = 0; // first schedule Voc, reset = 1.
    voc_ret = vpp_AudioJpegEncOpen(VidRec_EncodeFrameIsr);

    switch (voc_ret)
    {
        case HAL_ERR_NO:
            break;

        case HAL_ERR_RESOURCE_RESET:
        case HAL_ERR_RESOURCE_BUSY:
        case HAL_ERR_RESOURCE_TIMEOUT:
        case HAL_ERR_RESOURCE_NOT_ENABLED:
        case HAL_ERR_BAD_PARAMETER:
        case HAL_ERR_UART_RX_OVERFLOW:
        case HAL_ERR_UART_TX_OVERFLOW:
        case HAL_ERR_UART_PARITY:
        case HAL_ERR_UART_FRAMING:
        case HAL_ERR_UART_BREAK_INT:
        case HAL_ERR_TIM_RTC_NOT_VALID:
        case HAL_ERR_TIM_RTC_ALARM_NOT_ENABLED:
        case HAL_ERR_TIM_RTC_ALARM_NOT_DISABLED:
        case HAL_ERR_COMMUNICATION_FAILED:
        case HAL_ERR_QTY:
            diag_printf("fail to call vpp_AMREncOpen(), result: %d \n", voc_ret);
            return ERROR_AMROPENFAIL;

        default:
            break;
    }

    vpp_AudioJpegEncSetMicDigitalGain(calibPtr->bb->audio[audioItf].audioParams.MicDigitalGainInVoC);

    return NO_ERRO;
}

void Mic_RecMsgHandle_Half(VOID)
{
    MCI_TRACE (MCI_VIDREC_TRC,0,"Mic_Interrupt_half start!Msg_Wait_Num=%d",Msg_Wait_Num);	

    if ((videoRecordState == VidrecRecordStatus) && (vidrec_VocFirstSchedule > 0) )
    {
        Pcm_StartPosition = 0;
        Voc_Amr_Need = 1;
        Voc_Amr_Count = 0;
        if(Msg_Wait_Num <= 0)
            VidRec_Pcm2Amr();
    }

    MCI_TRACE (MCI_VIDREC_TRC,0,"Mic_Interrupt_half over!");		
}

void Mic_RecMsgHandle_End(VOID)
{
    MCI_TRACE (MCI_VIDREC_TRC,0,"Mic_Interrupt_end start!Msg_Wait_Num=%d",Msg_Wait_Num);

    if ((videoRecordState == VidrecRecordStatus) && (vidrec_VocFirstSchedule > 0) )
    {
        Pcm_StartPosition = 1;
        Voc_Amr_Need = 1;
        Voc_Amr_Count = 0;
        if(Msg_Wait_Num <= 0)
            VidRec_Pcm2Amr();
    }

    MCI_TRACE (MCI_VIDREC_TRC,0,"Mic_Interrupt_end over!");		
}

int32 VidRec_PreviewStart(MMC_VDOREC_SETTING_STRUCT* previewPara, void(*vid_rec_finish_ind)(uint16 msg_result) )
{
    VOID* InvalideAddress = (VOID*)convertaddr(NULL);

    MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MMC_VIDREC]VidRec_PreviewStart!");

    if ((videoRecordState == VidrecStopStatus) || (videoRecordState == NoneStatus)) 
    {
        videoRecordState = VidrecPreviewStatus;

        memset(&g_VidRecCntx, 0x00, sizeof(VIDREC_CONTEXT));
        *(&(g_VidRecCntx.settingInfo)) = *previewPara;
        g_VidRecCntx.vidrec_callback = vid_rec_finish_ind;

        //vidrec_pAvi_Buff = (uint8*)convertaddr(mmc_MemMalloc(120* 1024));
        vidrec_pAvi_Buff = (uint8*)convertaddr(mmc_MemMalloc(60* 1024));
        if(vidrec_pAvi_Buff == InvalideAddress)
        {
            MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MMC_VIDREC_ERROR]NO ENOUGH MEMORY!");
            mmc_MemFreeAll();
            return MDI_RES_VDOREC_ERR_MEMORY_NOT_ENOUGH;
        }
        vidrec_avi_out = vidrec_pAvi_Buff;

        blendBuffer = 
            (UINT16*)convertaddr((mmc_MemMalloc(previewPara->previewWidth*previewPara->previewHeight*2)));
        if (blendBuffer == InvalideAddress)
        {
            mmc_MemFreeAll();
            return MDI_RES_VDOREC_ERR_MEMORY_NOT_ENOUGH;
        }

        avi_viidx = (AVI_video_index *)mmc_MemMalloc((int32)VIDREC_AVI_FRAME_MAX_COUNT * sizeof(AVI_video_index)); 
        RecPcmBuffer = (uint8 *)convertaddr(mmc_MemMalloc((int32)PCM_REC_BUF_SIZE * sizeof(uint8)));  
        AMR_buffer = (uint8 *)convertaddr((mmc_MemMalloc(32*VSRECBUF_LEN)));
        if(avi_viidx == NULL || RecPcmBuffer == InvalideAddress || AMR_buffer == InvalideAddress)
        {
            MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MMC_VIDREC_ERROR]NO ENOUGH MEMORY!");
            mmc_MemFreeAll();
            return MDI_RES_VDOREC_ERR_MEMORY_NOT_ENOUGH;
        }
        AMR_buffer_heard = AMR_buffer_tail = AMR_buffer;

        Msg_Wait_Num = 0; 
        Effect_Set_Valid = 0;
        AMR_Data_Store = 0;
        Vidrec_Sensor_Seq = 0;

        if(MCI_CamPowerUp(1, NULL) != 0)
        {
            diag_printf(" MCI_CamPowerUp is failed\n");
            return MDI_RES_VDOREC_ERR_FAILED;   
        }

        vidrec_VideoSampleStart(previewPara);
/*
        gp_VideoRecordAFrameJPEGBuf = (int8* )convertaddr((mmc_MemMalloc(40* 1024)));
        vidrec_JpegBuff0 = gp_VideoRecordAFrameJPEGBuf;
        vidrec_JpegBuff1 = gp_VideoRecordAFrameJPEGBuf+20*1024;
*/
        vidrec_JpegBuff0 = (int8* )convertaddr((mmc_MemMalloc(40* 1024)));
        if(vidrec_JpegBuff0 == InvalideAddress)
        {
            MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MMC_VIDREC_ERROR]NO ENOUGH MEMORY!");
            VidRec_PreviewStop();
            return MDI_RES_VDOREC_ERR_MEMORY_NOT_ENOUGH;
        }
        vidrec_JpegBuff1 = vidrec_JpegBuff0 + 20*1024;
        
        linebuff_asm = (int8* )mmc_MemMalloc(320*4);
        if(linebuff_asm==NULL)
        {
            MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MMC_VIDREC_ERROR]NO ENOUGH MEMORY!");
            VidRec_PreviewStop();
            return MDI_RES_VDOREC_ERR_MEMORY_NOT_ENOUGH;
        }
        
/*
        linebuff_asm = (int8* )mmc_MemMalloc(320*4);
        gp_VidRec_Prebuf=(int8* )convertaddr(mmc_MemMalloc(previewPara->previewWidth* previewPara->previewHeight*2));
        if(linebuff_asm==NULL||gp_VidRec_Prebuf==InvalideAddress)
        {
            MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MMC_VIDREC_ERROR]NO ENOUGH MEMORY!");
            VidRec_PreviewStop();
            return MDI_RES_VDOREC_ERR_MEMORY_NOT_ENOUGH;
        }
*/
        vidrec_avi_end = (uint8*)mmc_MemMalloc(30*1024);// (uint8*)mmc_MemMalloc( mmc_MemGetFree()- 1024);//VIDREC_AVI_FRAME_MAX_COUNT* 32 + 8  );  // 225*3k
        if(vidrec_avi_end == NULL)
        {
            MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MMC_VIDREC_ERROR]NO ENOUGH MEMORY!");
            VidRec_PreviewStop();
            return MDI_RES_VDOREC_ERR_MEMORY_NOT_ENOUGH;
        }
        
        MCI_TRACE (MCI_VIDREC_TRC,0,"vidrec_avi_out = %p \n",vidrec_avi_out);
        MCI_TRACE (MCI_VIDREC_TRC,0,"avi_viidx = %p \n",avi_viidx); 
        MCI_TRACE (MCI_VIDREC_TRC,0,"RecPcmBuffer = %p \n",RecPcmBuffer);      
        MCI_TRACE (MCI_VIDREC_TRC,0,"AMR_buffer_heard = %p, AMR_buffer_tail = %p, AMR_buffer = %p \n", AMR_buffer_heard, AMR_buffer_tail, AMR_buffer);
        MCI_TRACE (MCI_VIDREC_TRC,0,"[MMC_VIDREC]vidrec_JpegBuff0 = 0x%x",vidrec_JpegBuff0);

        return MDI_RES_VDOREC_SUCCEED;
    }

    return MDI_RES_VDOREC_ERR_FAILED;
}

int32 VidRec_PreviewStop(void)
{
    if (videoRecordState == VidrecPreviewStatus)
    {
        videoRecordState = VidrecStopStatus;
        MCI_CamPowerDown();
    }

    return NO_ERRO;
}

// Open mic for video recoder zhangl 2008-11-03
AUD_ERR_T VidRec_MicStart(VOID)
{
    AUD_LEVEL_T audioCfg;
    AUD_ERR_T audErr;
    HAL_AIF_STREAM_T pcmStream;

    // Set audio interface 
    audioCfg.spkLevel = AUD_SPK_MUTE;       
    audioCfg.micLevel = AUD_MIC_ENABLE;
    audioCfg.sideLevel = AUD_SIDE_VOL_3;//AUD_SIDE_MUTE;
    audioCfg.toneLevel = 0;
            
    // PCM Audio stream, input of VPP
    pcmStream.startAddress = (UINT32*)RecPcmBuffer;
    pcmStream.length = PCM_REC_BUF_SIZE;//sizeof(HAL_SPEECH_PCM_BUF_T);
    pcmStream.sampleRate = HAL_AIF_FREQ_8000HZ;
    pcmStream.channelNb = HAL_AIF_MONO;
    pcmStream.voiceQuality = TRUE;
    pcmStream.halfHandler = Mic_RecMsgHandle_Half; // Mechanical interaction with VPP's VOC
    pcmStream.endHandler = Mic_RecMsgHandle_End;//NULL; // Mechanical interaction with VPP's VOC

 	if(audioItf!=AUD_ITF_EAR_PIECE&&audioItf!=AUD_ITF_RECEIVER)
   	{
   	  	  // Initial cfg
    		audioItf = AUD_ITF_RECEIVER;
   	}

   
    audErr = aud_StreamRecord(audioItf, &pcmStream, &audioCfg);     

    if(audErr == AUD_ERR_NO)
        MCI_TRACE (MCI_VIDREC_TRC,0,"----audio recoder of the vr is started: ok-----");
    else
        MCI_TRACE (MCI_VIDREC_TRC|TSTDOUT,0,"[MMC_VIDREC_ERROR]audio recoder of the video recorder is started: failure with %d-----",audErr);
            
    return audErr;
}
/*+ zhouqin modify for video record in secord card20110530*/
#if defined(DUAL_TFLASH_SUPPORT)
extern BOOL mmi_vdorec_check_storage(void);
#endif
/*- zhouqin modify for video record in secord card20110530*/
int32 VidRec_RecordStart(HANDLE filehandle)
{
    g_VidRecCntx.fileHandle = filehandle;
    
    if (videoRecordState == VidrecPreviewStatus)
    {
        MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0, "[MMC_VIDREC]VidRec_RecordStart!");

        /*******initialization of  the file process ********/
        
        /*+ zhouqin modify for video record in secord card20110530*/
        #if defined(DUAL_TFLASH_SUPPORT)
        if(mmi_vdorec_check_storage()== TRUE)
        FS_GetFSInfo("mmc1", &g_FsVidRecInfo);
        else
        #endif
        /*- zhouqin modify for video record in secord card20110530*/
	 FS_GetFSInfo("mmc0", &g_FsVidRecInfo);

        if ((g_FsVidRecInfo.iUsedSize + AVI_TF_MIN*1024) >= g_FsVidRecInfo.iTotalSize)
        {
            videoRecordState = VidrecStopStatus;

            MCI_CamPowerDown(); 

            diag_printf("Video Record Finished, DISK_FULL!\n");
            return MDI_RES_VDOREC_REACH_DISK_LIMIT;
        }  

        memset(vidrec_avi_head, 0x00, AVI_HEAD_SIZE);

        FS_Write(g_VidRecCntx.fileHandle, vidrec_avi_head, AVI_HEAD_SIZE);

        Voc_AmrEnc_Busy = 0;
        Voc_JpegEnc_Busy = 0;

        // ////////////////// AVI info init, video record context init and voc init  //////////////////////
        if (VidRec_Init("", &g_VidRecCntx) != NO_ERRO)
            return MDI_RES_VDOREC_ERR_FAILED;

        g_AMR_DataInCount = 0;

        if (g_VidRecCntx.settingInfo.IfRecordAudio)
        {
            diag_printf( "----sound recorder is started----\n" );
            VR_Init = 0;
            VR_Amr_Init = 0;
            Voc_Amr_Count = 0;
            memset(RecPcmBuffer, 0x0, PCM_REC_BUF_SIZE*sizeof(uint8));
            memset(AMR_buffer, 0, 32*VSRECBUF_LEN*sizeof(uint8));
            VidRec_MicStart();
        }
        else
        {
            // fill the AMR output buffer with 0
            memset(AMR_buffer, 0, 32*VSRECBUF_LEN*sizeof(uint8));
        }

        videoRecordState = VidrecRecordStatus;
        Avi_Out_Index = 0;
        vidrec_AviNuminBuff = 0;
        Voc_JpegOut_Seq = 0;
        vidrec_pJpegVocOut = vidrec_JpegBuff1;
        vidrec_pJpegAviIn = vidrec_JpegBuff0;
        vidrec_pJpeg_sizeVoc = &vidrec_Jpeg1_size;
        vidrec_pJpeg_sizeAvi = &vidrec_Jpeg0_size;

        g_VidRecCntx.recordStartTime = hal_TimGetUpTime();//cyg_current_time(); changde by zhangl
        //   test_video=0;
        return MDI_RES_VDOREC_SUCCEED;
    }
    return MDI_RES_VDOREC_ERR_FAILED;
}

int32 VidRec_RecordPause(void)
{
    if (videoRecordState == VidrecRecordStatus)
    {
	videoRecordState = VidrecPauseStatus;

        if (g_VidRecCntx.settingInfo.IfRecordAudio)
        {
            aud_StreamPause(audioItf, TRUE);
        }

        g_VidRecCntx.VocState = VIDREC_VOC_IDLE;

        g_VidRecCntx.RGBBufStatus[0] = RGB_BUF_NULL;
        g_VidRecCntx.RGBBufStatus[1] = RGB_BUF_NULL;

        g_VidRecCntx.lastRecordPeriod +=
            (hal_TimGetUpTime()/*cyg_current_time()*/ - g_VidRecCntx.recordStartTime) / VIDREC_DEFAULT_FREQ_TICK_COUNT_ONE_SECOND;
    }

    return MDI_RES_VDOREC_SUCCEED;
}

int32 VidRec_RecordResume(void)
{
    if (videoRecordState == VidrecPauseStatus)
    {
        if (g_VidRecCntx.settingInfo.IfRecordAudio)
        {
            // begint to sound sample
            AMR_buffer_heard = AMR_buffer_tail = AMR_buffer;
            Avi_Out_Index = 0;
            AMR_Data_Store = 0;
            Voc_AmrEnc_Busy = 0;
            Voc_JpegEnc_Busy = 0;
            aud_StreamPause(audioItf, FALSE);
        }

        // begin to count time
        g_VidRecCntx.recordStartTime = hal_TimGetUpTime();//cyg_current_time(); changed by zhangl
        videoRecordState = VidrecRecordStatus;
    }

    return MDI_RES_VDOREC_SUCCEED;
}

int32 VidRec_RecordStop(void)
{
    if ((videoRecordState == VidrecRecordStatus) || (videoRecordState == VidrecPauseStatus))
    {
        videoRecordState = VidrecStopStatus;

	MCI_TRACE (MCI_VIDREC_TRC,0, "[COMM]: GET A MESSAGE - REC_STOP_SAMPLE_REQ%d", g_VidRecCntx.settingInfo.IfRecordAudio);

        if (g_VidRecCntx.settingInfo.IfRecordAudio)
        {
            MCI_TRACE (MCI_VIDREC_TRC,0, "[COMM]: GET A MESSAGE - REC_STOP_SAMPLE_REQ");

            if(aud_StreamStop( audioItf ) != 0)
                MCI_TRACE (MCI_VIDREC_TRC,0,"aud_StreamStop close failed");

        }

        bset = 2;
        mmc_SetCurrMode(MMC_MODE_IDLE);
        Lily_Camera_PreviewOff();
        camera_PowerOff();

        g_AMR_DataInCount = 0;
        vpp_AudioJpegEncClose();
        VidRec_AVIFileClose(&g_VidRecCntx);

        mmc_MemFreeAll();
    }

    return MDI_RES_VDOREC_SUCCEED;
}

void VidRec_JPEGEncode(int32* buf_in,short width,short height)
{
    vpp_AudioJpeg_ENC_IN_T VocMP4EncIn; 

    VocMP4EncIn.SampleRate=(cutheight<<16)|cutwidth;
    VocMP4EncIn.mode = MMC_MJPEG_ENCODE;  // update ram according to global  mode
    VocMP4EncIn.imag_width  = width;//VIDEO_RECORD_DISP_WIDTH;
    VocMP4EncIn.imag_height = height;//VIDEO_RECORD_DISP_HEIGHT;

    g_VidRecCntx.settingInfo.encodeQuality = 1;
    if (g_VidRecCntx.settingInfo.encodeQuality == VID_REC_QTY_LOW)
    {
        VocMP4EncIn.imag_quality = 0;
    }
    else
    {
        VocMP4EncIn.imag_quality = 1; 
    }
    g_VidRecCntx.VocState = VIDREC_JPEG_ENC_BUSY;

    VPP_WRITE_JPEGHeadr(VocMP4EncIn.imag_quality, width, height, (uint8*)vidrec_pJpegVocOut);

    VocMP4EncIn.inStreamBufAddr = (INT32*)buf_in;   //gp_VideoRecordAFrameRGBBuf;
    VocMP4EncIn.outStreamBufAddr = (INT32*)(vidrec_pJpegVocOut+ 608);
    VocMP4EncIn.channel_num = 1;
    if (VR_Init==0)
    {
        VR_Init = 1;
        VocMP4EncIn.reset = 1;
    }

    if(HAL_ERR_NO != vpp_AudioJpegEncScheduleOneFrame(&VocMP4EncIn))
    {
        Voc_JpegEnc_Busy = 0;
        diag_printf ("[MMC_VIDREC_ERROR]Failed to call VOC in VidRec_JPEGEncode");
    }
}


 void VidRec_FS_AsynWrite_CB(void* pParam)
 {
    FS_ASYN_READ_RESULT *sRsp ;  

    sRsp = (FS_ASYN_READ_RESULT*)pParam;
    if(sRsp->iResult >= 0)
    {
 	vidrec_AviNuminBuff--;
    }
    else //fs write file
    {
        vidrec_AviNuminBuff = 0;

        diag_printf("[MMC_VIDREC_ERROR]FS write fail!");
        VidRec_RecordStop();
	g_VidRecCntx.vidrec_callback(MDI_RES_VDOREC_ERR_FAILED);
    }
    return;
 }

error_type VidRec_AVIFileClose(VIDREC_CONTEXT* amr_in)
{
    JpegAmr_Avi_End Avi_End_Input_Info;
    Avi_End_Return Avi_End_Output_Info;
    int32 count;

    /************write avi end data****************************/
    Avi_End_Input_Info.Buf_size = 512;  // VIDREC_AVI_FRAME_MAX_COUNT * 32 + 8;
    Avi_End_Input_Info.pAvi_End_Buf = vidrec_avi_end;
    Avi_End_Output_Info.OutByte_size = 0;
    Avi_End_Output_Info.RemainByte_size = 0;

    while(vidrec_AviNuminBuff>0)
    {
        diag_printf(" vidrec_AviNuminBuff =%d\n",vidrec_AviNuminBuff);
        sxr_Sleep(100);
    }

    do
    {
        if (MMF_Avi_Enc_End(&Avi_End_Input_Info, &Avi_End_Output_Info) == FUNC_OK)
            FS_Write((int32)amr_in->fileHandle, vidrec_avi_end, (int32)Avi_End_Output_Info.OutByte_size);
    } while (Avi_End_Output_Info.RemainByte_size > 0);


    /*************seek and write avi head********************************/
    count = FS_Seek((int32)amr_in->fileHandle, 0, FS_SEEK_SET);
    MMF_Avi_Enc_Head(vidrec_avi_head);
    count = FS_Write((int32)amr_in->fileHandle, vidrec_avi_head, AVI_HEAD_SIZE);

    // here should call the callback function to delete it
    if(amr_in->curFileSize < AVI_SIZE_MIN)
    {
        diag_printf(" amr_in->curFileSize =%d\n", amr_in->curFileSize);

        g_VidRecCntx.vidrec_callback((uint16)MDI_RES_VDOREC_ERR_FILE_SIZE);
    }

    return NO_ERRO;
}

void VidRec_CallbackFun(UINT8 id,  UINT8 ActiveBufNum)
{
    Cam_Data_Valid = id;
    MCI_TRACE (MCI_VIDREC_TRC, 0,
                "[MMC_VIDREC]VidRec_CallbackFun TIME:%d,NUM:%d,ID:%d", hal_TimGetUpTime(), Msg_Wait_Num, id);

    //send message for preview and capture the next picture
    if(Msg_Wait_Num <= 0)
    {
        mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VIDREC_SENSOR_IMAGE_READY);
        Msg_Wait_Num++;
    }
    else
    {
        MCI_TRACE (MCI_VIDREC_TRC, 0, 
                "[MMC_VIDREC]one sensor image is discarded,Msg_Wait_Num=%d",Msg_Wait_Num);

        camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, MY_NPIX, sensorbuf);
        //camera_CaptureImage();     
    }      
}

void	VidRec_FrameOutput(void)
{
    error_type avi_inte_result=NO_ERRO;
    do {
        MCI_TRACE (MCI_VIDREC_TRC,0,"[MMC_VIDREC]AMR_Data_Store:%d",AMR_Data_Store);
        if(Avi_Out_Index>0)
        {
            if(NO_ERRO!=(avi_inte_result=vidrec_EncodeRun(&g_VidRecCntx)))
            {
                MCI_TRACE (MCI_VIDREC_TRC|TSTDOUT, 0, "[MMC_VIDREC_ERROR]vidrec_EncodeRun Error:%d", avi_inte_result);
                g_VidRecCntx.vidrec_callback((uint16)avi_inte_result);
                return;
            }
        }
        else
        {
            MCI_TRACE (MCI_VIDREC_TRC|TSTDOUT,0,"[MMC_VIDREC]THE FISRST FRAME IS BAD!");
            break;
        }
    }while(AMR_Data_Store>=10&&Voc_JpegEnc_Busy==0);

	sxr_Sleep(48);//release cpu for MMI task

    camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, MY_NPIX, sensorbuf);
    //camera_CaptureImage();   
}

void VidRec_UserMsg(COS_EVENT *pnMsg)
{
    switch (pnMsg->nEventId)
    {
        case  MSG_VIDREC_SENSOR_IMAGE_READY:	
            MCI_TRACE (MCI_VIDREC_TRC,0,"MSG_VIDREC_SENSOR_IMAGE_READY");
            VidRec_PreCap();					
            break;

        case MSG_VIDREC_AVI_OUTPUT_REQ:
            MCI_TRACE (MCI_VIDREC_TRC,0,"MSG_VIDREC_AVI_OUTPUT_REQ");
            VidRec_FrameOutput();
            break;

        case EV_TIMER:
            switch(pnMsg->nParam1)
            {
                case MMC_CAMERA_TIMER_ID:
                    MCI_TRACE(MCI_VIDREC_TRC, 0,"[MMC_VIDREC]Camera_UserMsg  EV_TIMER");
                    if(bset<2)//bset==2 means the camera is power off
                        Lily_Camera_SetEffect(CAM_PARAM_WB,(uint32)g_VidRecCntx.settingInfo.whiteBalance);
                    break;
                default:
                    MCI_TRACE(MCI_VIDREC_TRC, 0,"[MMC_VIDREC]Invalid timer ID in camera");
                    break;
            }			
            break;

        default:
            break;
    }
}

void VidRec_PreCap(void)
{
MCI_TRACE (MCI_VIDREC_TRC,0,"[MMC_VIDREC]VidRec_PreCap! tick %d", hal_TimGetUpTime());
    
    // if video recorder is stopped, as follow not exec.
    if (videoRecordState == VidrecStopStatus) return;
	
    cutwidth  = (srcwidth*10/(10+(CameraInfo.previewZoom<<1)))&0xfffffffc;
    cutheight = (srcheight*10/(10+(CameraInfo.previewZoom<<1)))&0xfffffffe;
   
    if ((videoRecordState == VidrecRecordStatus) && (vidrec_VocFirstSchedule > 0) )
    {
        // the operation related to video recoder is to be added next
        if(Cam_Data_Valid==0)
        {
            if (Voc_AmrEnc_Busy==0 && Voc_JpegEnc_Busy ==0)
            {
                Voc_JpegEnc_Busy=1;
                Voc_Interrupt_From=0;
                Voc_Jpg_Need=0;

                VidRec_JPEGEncode((int32 *)(sensorbuf+srcwidth*((srcheight-cutheight)>>1)+((srcwidth-cutwidth)>>1)), 
                            CameraInfo.image_width, CameraInfo.image_height);
            }
            else
            {
                MCI_TRACE (MCI_VIDREC_TRC, 0, "[MMC_VIDREC_WARN]1 sensor image waiting,Voc_JpegEnc_Busy=%d,Voc_AmrEnc_Busy=%d", 
                            Voc_JpegEnc_Busy, Voc_AmrEnc_Busy);
                Voc_Jpg_Need=1;
            }   
        }
    }
   	
    if(( Cam_Data_Valid == 0 ) && ( Effect_Set_Valid == 0 ))
    { 
        LCDD_ERR_T err;
        HAL_GOUDA_VID_LAYER_DEF_T   gouda_vid_def = {0, };

#if 0
        startPos = ((srcheight - cutheight)/2)*srcwidth + (srcwidth - cutwidth)/2;
#else
        VidRec_bilinear_zoom((uint8*)((uint32)sensorbuf),
                    (int)srcwidth, (int)srcheight, (int)cutwidth, (int)cutheight, (uint8*)blendBuffer, 
                    (int)CameraInfo.preview_width, (int)CameraInfo.preview_height, linebuff_asm); 
#endif
            
        gouda_vid_def.fmt = HAL_GOUDA_IMG_FORMAT_YUYV;
#if 0
        gouda_vid_def.addrY = (UINT32*)(sensorbuf+startPos);
#else
        gouda_vid_def.addrY = (UINT32*)(blendBuffer);
#endif
        gouda_vid_def.alpha = 0xFF;
        gouda_vid_def.cKeyColor = 0;
        gouda_vid_def.cKeyEn = FALSE;
        gouda_vid_def.cKeyMask = HAL_GOUDA_CKEY_MASK_OFF;
        gouda_vid_def.depth = HAL_GOUDA_VID_LAYER_BETWEEN_2_1;
#if 0        
        gouda_vid_def.height    =  cutheight;
        gouda_vid_def.width = cutwidth;
        gouda_vid_def.stride    = srcwidth*2;
#else
        gouda_vid_def.height = CameraInfo.preview_height;
        gouda_vid_def.width = CameraInfo.preview_width;
        gouda_vid_def.stride = CameraInfo.preview_width*2;
#endif
        gouda_vid_def.pos.tlPX = CameraInfo.start_x;
        gouda_vid_def.pos.tlPY = CameraInfo.start_y;
        gouda_vid_def.pos.brPX = CameraInfo.start_x + CameraInfo.preview_width - 1;
        gouda_vid_def.pos.brPY = CameraInfo.start_y + CameraInfo.preview_height - 1;

        while(hal_GoudaIsActive());
        hal_GoudaVidLayerClose();
        hal_GoudaVidLayerOpen(&gouda_vid_def);

        displayFbw.fb.buffer = NULL;

        // Display the blended image until lcdd is available
        err = lcdd_Blit16(&displayFbw, 0, 0);
        while (LCDD_ERR_NO != err)
        {
            sxr_Sleep(10);
            err = lcdd_Blit16(&displayFbw,0 ,CameraInfo.start_y);
        }
        // wait until the image has been sent to the LCD
  //      while(hal_GoudaIsActive());
    }

MCI_TRACE (MCI_VIDREC_TRC,0,"[MMC_VIDREC]finished preview a frame! tick %d", hal_TimGetUpTime());

    if ((videoRecordState == VidrecRecordStatus) && (vidrec_VocFirstSchedule > 0) )
    {
        // the operations related to the video recoder is to be added next
        mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VIDREC_AVI_OUTPUT_REQ);
    }
    else
    {
MCI_TRACE (MCI_VIDREC_TRC,0,"[MMC_VIDREC]start to get next frame! tick %d", hal_TimGetUpTime());

        camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, MY_NPIX, sensorbuf);
        //camera_CaptureImage();   
    }

    Effect_Set_Valid=0;    
    Msg_Wait_Num--;
}


int32 vidrec_VideoSampleStart(MMC_VDOREC_SETTING_STRUCT* previewPara)
{
    CAM_PREVIEW_STRUCT cam_para_temp;
	
    cam_para_temp.image_height = previewPara->imageHeight;
    cam_para_temp.image_width = previewPara->imageWidth;
    cam_para_temp.start_x = previewPara->previewStartX;
    cam_para_temp.start_y = previewPara->previewStartY;
    cam_para_temp.end_x = previewPara->previewWidth + previewPara->previewStartX-1;
    cam_para_temp.end_y = previewPara->previewHeight + previewPara->previewStartY-1;
    cam_para_temp.nightmode = previewPara->nightMode;
    cam_para_temp.imageQuality = previewPara->encodeQuality;
    cam_para_temp.factor = previewPara->zoomFactor;
    cam_para_temp.specialEffect = previewPara->specialEffect;
    cam_para_temp.whiteBlance = previewPara->whiteBalance;
    cam_para_temp.contrast = previewPara->contrast;
    cam_para_temp.brightNess = previewPara->brightnessLevel;
	
    set_mmc_camera_preview();	
    MCI_CamPreviewOpen(&cam_para_temp);

    return 0;
}


#else // (CHIP_HAS_GOUDA != 1)


#define  VSRECBUF_LEN 900 //mutiple of 5 ,2 or 3.
#define AVI_SIZE_MIN 1000

volatile  int32 Voc_JpegEnc_Busy=0;
volatile int32 Voc_AmrEnc_Busy=0;
volatile int32 Voc_Jpg_Need=0;
volatile int32 Voc_Amr_Need=0;
volatile int32 Voc_Interrupt_From=-1;//0: VOC interrupt from jpgenc  and 1: VOC interrupt from amrenc  
volatile int32 Voc_Amr_Count=0;
volatile int32 Pcm_StartPosition=0;
volatile uint32 Avi_Out_Index=0;
volatile uint32 Jpeg_Out_Index=0;
volatile int32 Effect_Set_Valid=0;
volatile int32 Msg_Wait_Num=0;
volatile int32 Vidrec_Sensor_Seq=0;
volatile int32 Voc_JpegOut_Seq=0;

volatile uint16 * sensor_data_buf;
int8* gp_VidRec_Prebuf;
uint8 * linebuff_asm;
volatile int32   Cam_Data_Valid=-1;
volatile uint32 g_AMR_DataInCount=0;
volatile int32 VR_Init=0;
volatile int32 VR_Amr_Init=0;

#define PCM_REC_BUF_SIZE  160 * 2  * 10
#define AMR_TRANS_COUNT 5
#define AVI_TF_MIN 650 //for 32minutes video index

#ifdef VIDREC_USE_QVGA_SCREEN
#define VIDEO_RECORD_FRAME_RATE 6
#else 
#define VIDEO_RECORD_FRAME_RATE 10
#endif

uint8 *AMR_buffer;
volatile uint8 *AMR_buffer_tail;
volatile uint8 *AMR_buffer_heard;
volatile int32 AMR_Data_Store=0;

uint8* RecPcmBuffer;

extern AVI_video_index* avi_viidx;  // bao debug: design the index table according 500 frame 
extern uint16  *sensorbuf;//[640*480];
extern uint16* blendBuffer;
extern int video_i;
extern uint32 cam_wbValue;
extern AUD_ITF_T   audioItf;

// //////////////////////////////////////////////////////////////////////////////
// EXTERN FUNCTION PROTOTYPES
// //////////////////////////////////////////////////////////////////////////////
// TODO.. 

// //////////////////////////////////////////////////////////////////////////////
// DEFINE CONSTANTS 
// //////////////////////////////////////////////////////////////////////////////
// video recorder default settings 
const MMC_VDOREC_SETTING_STRUCT g_VidrecDefaultSetting = {
    0,  // previewStartX;
    0,  // previewStartY;
    VIDEO_RECORD_MJPEG_WIDTH, // previewWidth;
    VIDEO_RECORD_MJPEG_HEIGHT,  // previewHeight;

    0,  // zoomFactor;
    CAM_EV_ZERO,  // exposureVerify;

    CAM_WB_AUTO,  // whiteBalance;
    CAM_EFFECT_ENC_NORMAL,  // specialEffect;
    FALSE,  // nightMode;

    TRUE, // IfRecordAudio;
    0,  // fileSizePermit;
    0,  // recordTimePermit;
    CAM_JPG_QTY_HIGH, // encodeQuality;

    0 // reserved;
};

// //////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
// //////////////////////////////////////////////////////////////////////////////
// TODO... 
VIDREC_CONTEXT g_VidRecCntx;
FS_INFO g_FsVidRecInfo;
int32* vidrec_inbuf;
uint8* vidrec_pAvi_Buff;
uint8* vidrec_avi_out;
uint8* vidrec_pAvi_FS;
int32 vidrec_AviNuminBuff;
uint8* vidrec_JpegBuff0;
uint8* vidrec_JpegBuff1;
volatile uint8* vidrec_pJpegVocOut;
volatile uint8* vidrec_pJpegAviIn;

int32 vidrec_Jpeg0_size;
int32 vidrec_Jpeg1_size;
volatile uint32* vidrec_pJpeg_sizeVoc=NULL;
volatile uint32* vidrec_pJpeg_sizeAvi=NULL;

uint8* vidrec_avi_end;  
uint8 vidrec_avi_head[AVI_HEAD_SIZE];
uint8 vidrec_amr_first_frame[32* 30 + 6];
volatile mmcModeStatus videoRecordState = VidrecStopStatus;
int32 vidrec_VocFirstSchedule;

// //////////////////////////////////////////////////////////////////////////////
// LOCAL VARIABLES
// //////////////////////////////////////////////////////////////////////////////
// TODO... 
static int32 vidrec_AmrEncFirstSchedule = 0;

extern volatile   CAM_INFO_STRUCT  CameraInfo;
extern volatile   uint16  srcwidth,srcheight;
extern volatile   uint16  cutwidth,cutheight;
extern uint16  *sensorbuf;//[640*480];
extern uint16  *sensorbuf1;
extern uint16  *sensorbuf2;
extern uint16  *blendBuffer;//[MMC_LcdWidth*MMC_LcdHeight];
extern uint16  *backbuffer;
extern CAM_SIZE_T   MY_NPIX; 
#if (CSW_EXTENDED_API_LCD == 1)
extern LCDD_FBW_T displayFbw;
#endif
extern volatile uint16  bset;
extern int Lily_Camera_PreviewOff(void);
extern void sxr_Sleep( UINT32 );

void VidRec_EncodeFrameIsr(HAL_VOC_IRQ_STATUS_T* irqStatus)
{
	MCI_TRACE (MCI_VIDREC_TRC,0,"------------Voc_Interrupt_From=%d,Voc_Jpg_Need=%d,Voc_Amr_Need =%d,Voc_Amr_Count=%d",Voc_Interrupt_From,Voc_Jpg_Need,Voc_Amr_Need,Voc_Amr_Count);

	if ((vidrec_VocFirstSchedule != 0)&&(videoRecordState == VidrecRecordStatus))
	{

		if(Voc_Interrupt_From==0)
		{
			Voc_Interrupt_From=-1;
			Avi_Out_Index+=1;
			vpp_AudioJpeg_ENC_OUT_T VocOutputStatus; 
			vpp_AudioJpegEncStatus(&VocOutputStatus);
			*vidrec_pJpeg_sizeVoc=VocOutputStatus.output_len;// + 607;
			if(Voc_JpegOut_Seq==0)
			{
				Voc_JpegOut_Seq=1;
				vidrec_pJpegVocOut=vidrec_JpegBuff0;
				vidrec_pJpegAviIn =vidrec_JpegBuff1;
				vidrec_pJpeg_sizeVoc = &vidrec_Jpeg0_size;
				vidrec_pJpeg_sizeAvi = &vidrec_Jpeg1_size;				
			}
			else{
				Voc_JpegOut_Seq=0;
				vidrec_pJpegVocOut = vidrec_JpegBuff1;
				vidrec_pJpegAviIn = vidrec_JpegBuff0;
				vidrec_pJpeg_sizeVoc = &vidrec_Jpeg1_size;		
				vidrec_pJpeg_sizeAvi = &vidrec_Jpeg0_size;				
			}
			Voc_JpegEnc_Busy=0;

			///////////voc processing is over and try to check if there's waiting one!//////////////

			if (Voc_Amr_Need>0)
				VidRec_Pcm2Amr(); 
		}
		else if (Voc_Interrupt_From==1)
		{
			Voc_Interrupt_From=-1;    
			Voc_AmrEnc_Busy=0;
			AMR_Data_Store++;
			if(Voc_Amr_Count>=AMR_TRANS_COUNT)
			{
				Voc_Amr_Count=0;
				Voc_Amr_Need=0;
			}

			if (Voc_Jpg_Need==1&&AMR_Data_Store>=5)
			{
				Voc_JpegEnc_Busy=1;
				Voc_Interrupt_From=0;
				Voc_Jpg_Need=0;
				VidRec_JPEGEncode((int32 *)(sensor_data_buf+srcwidth*((srcheight-cutheight)>>1)+((srcwidth-cutwidth)>>1)),g_VidRecCntx.settingInfo.imageWidth,g_VidRecCntx.settingInfo.imageHeight);
			}
			else if (Voc_Amr_Need>0)
			{
				VidRec_Pcm2Amr();
			}
			else
			{
				;
				MCI_TRACE (MCI_VIDREC_TRC,0,"------------VOC IDLE!,Voc_Amr_Need=%d",Voc_Amr_Need);
			}
		}
	}
	else
		vidrec_VocFirstSchedule = 1;
	//MCI_TRACE (MCI_VIDREC_TRC,0,"------------Voc_Interrupt over!");	
}

error_type VidRec_Init(uint8* pFileName, VIDREC_CONTEXT* amr_in)
{
	JpegAmr_Avi_Init Avi_Init_Info;
	HAL_ERR_T voc_ret = 0;
	CALIB_CALIBRATION_T* calibPtr = calib_GetPointers();

	/**************init avi file info*********************************/
	Avi_Init_Info.Amr_BitRate      = 12800;
	Avi_Init_Info.Audio_Bits       = 16;
	Avi_Init_Info.Audio_Channel    = 1;
	Avi_Init_Info.Audio_SampleRate = 8000;
	Avi_Init_Info.Image_Height     = g_VidRecCntx.settingInfo.imageHeight;
	Avi_Init_Info.Image_Width      = g_VidRecCntx.settingInfo.imageWidth;
	Avi_Init_Info.Video_fps        = VIDREC_AVI_VIDEO_FRAME_RATE;
	if (MMF_Avi_Enc_Init(&Avi_Init_Info) == FUNC_OK)
		MCI_TRACE (MCI_VIDREC_TRC,0,"Avi Encoder init ok\n");

	// init video record context
	//amr_in->VppCodecMode       = AMR_ENCODE;
	amr_in->AMREncodeUnitCnt   = 0;
	amr_in->AVIFrameCnt        = 0;
	amr_in->curFileSize        = AVI_HEAD_SIZE + VIDREC_AVI_INDEX_TABLE_HEAD_LEN;
	amr_in->lastRecordPeriod   = 0;
	amr_in->JPEGEncodeFrameCnt = 0; // set init value as 0!
	amr_in->RGBBufStatus[0]       = RGB_BUF_NULL;
	amr_in->RGBBufStatus[1]       = RGB_BUF_NULL;
	amr_in->VocState              = VIDREC_VOC_IDLE;

	vidrec_VocFirstSchedule    = 0;
	vidrec_AmrEncFirstSchedule = 0; // first schedule Voc, reset = 1.
	voc_ret                    = vpp_AudioJpegEncOpen(VidRec_EncodeFrameIsr);

	switch (voc_ret)
	{
		case HAL_ERR_NO:
			break;

		case HAL_ERR_RESOURCE_RESET:
		case HAL_ERR_RESOURCE_BUSY:
		case HAL_ERR_RESOURCE_TIMEOUT:
		case HAL_ERR_RESOURCE_NOT_ENABLED:
		case HAL_ERR_BAD_PARAMETER:
		case HAL_ERR_UART_RX_OVERFLOW:
		case HAL_ERR_UART_TX_OVERFLOW:
		case HAL_ERR_UART_PARITY:
		case HAL_ERR_UART_FRAMING:
		case HAL_ERR_UART_BREAK_INT:
		case HAL_ERR_TIM_RTC_NOT_VALID:
		case HAL_ERR_TIM_RTC_ALARM_NOT_ENABLED:
		case HAL_ERR_TIM_RTC_ALARM_NOT_DISABLED:
		case HAL_ERR_COMMUNICATION_FAILED:
		case HAL_ERR_QTY:
			diag_printf("fail to call vpp_AMREncOpen(), result: %d \n", voc_ret);
			return ERROR_AMROPENFAIL;
		default:
			break;
	}

	vpp_AudioJpegEncSetMicDigitalGain(calibPtr->bb->audio[audioItf].audioParams.MicDigitalGainInVoC);

	return NO_ERRO;
}

error_type VidRec_AVIFileClose(VIDREC_CONTEXT* amr_in)
{
    JpegAmr_Avi_End Avi_End_Input_Info;
    Avi_End_Return Avi_End_Output_Info;
    int32 count;
//  int16 res = 0;
//  uint32 resultCount=0;

    /************write avi end data****************************/
    Avi_End_Input_Info.Buf_size         = 512;  // VIDREC_AVI_FRAME_MAX_COUNT * 32 + 8;
    Avi_End_Input_Info.pAvi_End_Buf     = vidrec_avi_end;
    Avi_End_Output_Info.OutByte_size    = 0;
    Avi_End_Output_Info.RemainByte_size = 0;

	while(vidrec_AviNuminBuff>0)
	{
		diag_printf(" vidrec_AviNuminBuff =%d\n",vidrec_AviNuminBuff);
		 sxr_Sleep(100);
	}
    do
    {
        if (MMF_Avi_Enc_End(&Avi_End_Input_Info, &Avi_End_Output_Info) == FUNC_OK)
            FS_Write((int32)amr_in->fileHandle, vidrec_avi_end, (int32)Avi_End_Output_Info.OutByte_size);
    } while (Avi_End_Output_Info.RemainByte_size > 0);
    /*************seek and write avi head********************************/
    count = FS_Seek((int32)amr_in->fileHandle, 0, FS_SEEK_SET);
    MMF_Avi_Enc_Head(vidrec_avi_head);
    count = FS_Write((int32)amr_in->fileHandle, vidrec_avi_head, AVI_HEAD_SIZE);

// here should call the callback function to delete it
 if(amr_in->curFileSize <AVI_SIZE_MIN){
	diag_printf(" amr_in->curFileSize =%d\n",amr_in->curFileSize);
	g_VidRecCntx.vidrec_callback((uint16)MDI_RES_VDOREC_ERR_FILE_SIZE);
 }
    return NO_ERRO;
}

int32 VidRec_PreviewStop(void)
{
	if (videoRecordState == VidrecPreviewStatus)
	{
		videoRecordState = VidrecStopStatus;
		MCI_CamPowerDown();
	}
	return NO_ERRO;
}

int32 VidRec_RecordStop(void)
{
    if ((videoRecordState == VidrecRecordStatus) || (videoRecordState == VidrecPauseStatus))
    {
        videoRecordState = VidrecStopStatus;

	MCI_TRACE (MCI_VIDREC_TRC,0, "[COMM]: GET A MESSAGE - REC_STOP_SAMPLE_REQ%d", g_VidRecCntx.settingInfo.IfRecordAudio);

        if (g_VidRecCntx.settingInfo.IfRecordAudio)
        {
            MCI_TRACE (MCI_VIDREC_TRC,0, "[COMM]: GET A MESSAGE - REC_STOP_SAMPLE_REQ");
        
            if(aud_StreamStop(audioItf)!=0)
                MCI_TRACE (MCI_VIDREC_TRC,0,"aud_StreamStop close failed");

        }
    
        bset=2;
        mmc_SetCurrMode(MMC_MODE_IDLE);
        Lily_Camera_PreviewOff();
        camera_PowerOff();

        g_AMR_DataInCount = 0;
        vpp_AudioJpegEncClose();
        VidRec_AVIFileClose(&g_VidRecCntx);

        mmc_MemFreeAll();

//    camInPreviewState = FALSE;
    }

    //EVENT_Free(GetLastEvent());
    return MDI_RES_VDOREC_SUCCEED;
}

error_type vidrec_EncodeRun(VIDREC_CONTEXT* amr_in)
{

	int32 count=0;
	// ////////////////// JPEG encode a frame finish ////////////////////////////////////

	amr_in->JPEGEncodeFrameCnt++;

	//  check if finish the whole video recored 
	if (((g_VidRecCntx.curFileSize + VIDREC_CHECK_AVI_FILE_SIZE) >=
		g_VidRecCntx.settingInfo.fileSizePermit* 1024)&&(g_VidRecCntx.settingInfo.fileSizePermit > 0))
	{
		diag_printf("[Vidrec Encode Run] file size is reach, file size = %d \n", g_VidRecCntx.curFileSize);
		VidRec_RecordStop();
		g_VidRecCntx.vidrec_callback(MDI_RES_VDOREC_REACH_SIZE_LIMIT);
		diag_printf("Video Record Finished, REACH_SIZE_LIMIT!\n");    
		return MDI_RES_VDOREC_REACH_SIZE_LIMIT; 
	}

	{
		int32 avi_frame_length = 0;
		JpegAmr_Avi_Enc avi_enc_info;
		static uint8 amrHead[6] = {0x23, 0x21, 0x41, 0x4d, 0x52, 0x0a};

		/*************avi frame integrate***************************/
		if ((videoRecordState == VidrecRecordStatus) &&
			(g_VidRecCntx.JPEGEncodeFrameCnt > 0) &&
			(g_VidRecCntx.settingInfo.IfRecordAudio//&& g_AMR_DataInCount--)
			|| ((!g_VidRecCntx.settingInfo.IfRecordAudio) && (g_VidRecCntx.RGBBufStatus[0] == RGB_BUF_NULL))))
		{

			if (!g_VidRecCntx.settingInfo.IfRecordAudio)
			{
				//diag_printf( "[No sound]: AVI frame package now. \n" );        
				g_VidRecCntx.pingInDataBlockNum = VIDREC_AMR_FRAME_NUM_IN_A_PACK;
				//memset(vidrec_outbuf,0x00,VIDREC_AMRENC_OUTBUF_SIZE);
			}

	/**********************VIDREC_AMR_FRAME_MAX_CNT_IN_A_PACK is just for test of zhangl**************************/   

			g_VidRecCntx.pingInDataBlockNum = VIDREC_AMR_FRAME_NUM_IN_A_PACK;
	
			if(AMR_Data_Store<5)
			{
				diag_printf("######discard one jpeg AMR_Data_Store=%d",AMR_Data_Store);
				return NO_ERRO;
			}
			
			if(vidrec_AviNuminBuff<6){
				avi_enc_info.Avi_outBuf = vidrec_avi_out;
				vidrec_avi_out +=20*1024; 
				if(vidrec_avi_out >(vidrec_pAvi_Buff+100*1024))
					vidrec_avi_out = vidrec_pAvi_Buff;
				vidrec_AviNuminBuff++;
//				MCI_TRACE (MCI_VIDREC_TRC,0," ++vidrec_AviNuminBuff =%d \n",vidrec_AviNuminBuff);

			}
			else
			{
				diag_printf("######discard avi,buffer overflow!vidrec_AviNuminBuff =%d,AMR_Data_Store=%d",vidrec_AviNuminBuff,AMR_Data_Store);
				AMR_Data_Store-=g_VidRecCntx.pingInDataBlockNum;
				AMR_buffer_heard += g_VidRecCntx.pingInDataBlockNum * 32;
				if (AMR_buffer_heard >= (AMR_buffer+VSRECBUF_LEN*32))
				{
					AMR_buffer_heard = AMR_buffer;
				}		  
				return NO_ERRO;
			}
			
			//avi_enc_info.Avi_outBuf = vidrec_avi_out;
			avi_enc_info.Jpeg_Frame_Size = * vidrec_pJpeg_sizeAvi;
			avi_enc_info.Jpeg_inBuf      =(uint8*) vidrec_pJpegAviIn + 1;
			
			if (g_VidRecCntx.AVIFrameCnt == 0)
			{
				memcpy((void*)vidrec_amr_first_frame, (const void*)amrHead, 6);
				memcpy((void*)(vidrec_amr_first_frame + 6), (const void*)AMR_buffer_heard, g_VidRecCntx.pingInDataBlockNum* 32);

				avi_enc_info.Amr_Data_Size = g_VidRecCntx.pingInDataBlockNum * 32 + 6;
				avi_enc_info.Amr_inBuf     = vidrec_amr_first_frame;
			}
			else
			{
				avi_enc_info.Amr_Data_Size = g_VidRecCntx.pingInDataBlockNum * 32;
				avi_enc_info.Amr_inBuf =(uint8*) AMR_buffer_heard;
			}
	/****************have no much enough amr data to integrate this frame, just discard this jpeg*********************/

			avi_frame_length = MMF_Avi_Enc_Frame(&avi_enc_info);
			g_VidRecCntx.AVIFrameCnt++;
			g_VidRecCntx.curFileSize += avi_frame_length + VIDREC_AVI_SINGLE_INDEX_LEN;

			AMR_Data_Store-=g_VidRecCntx.pingInDataBlockNum;
			AMR_buffer_heard += g_VidRecCntx.pingInDataBlockNum * 32;
			if (AMR_buffer_heard >= (AMR_buffer+VSRECBUF_LEN*32))
			{
				AMR_buffer_heard = AMR_buffer;
			}
	  

			//MCI_TRACE (MCI_VIDREC_TRC,0,"------begin write %d byte from:%x,at:%d\n",avi_frame_length,avi_enc_info.Avi_outBuf,hal_TimGetUpTime());
			//count = FS_Write(g_VidRecCntx.fileHandle, vidrec_avi_out, (int32)avi_frame_length);
			count =avi_frame_length;
			if(0 != FS_AsynWriteReq(g_VidRecCntx.fileHandle, avi_enc_info.Avi_outBuf, (int32)avi_frame_length,(FS_ASNY_FUNCPTR)VidRec_FS_AsynWrite_CB))
			{
				diag_printf("------FS write fail!");
			 	vidrec_AviNuminBuff--;
				VidRec_RecordStop();
				g_VidRecCntx.vidrec_callback(MDI_RES_VDOREC_ERR_FAILED);
				return MDI_RES_VDOREC_ERR_FAILED;
			}
			
			g_FsVidRecInfo.iUsedSize +=avi_frame_length;
			if ((g_FsVidRecInfo.iUsedSize + AVI_TF_MIN*1024)>= g_FsVidRecInfo.iTotalSize)
			{
				VidRec_RecordStop();
				g_VidRecCntx.vidrec_callback(MDI_RES_VDOREC_DISK_FULL);
				diag_printf("Video Record Finished, DISK_FULL!\n");
				return MDI_RES_VDOREC_DISK_FULL;
			}
			if(video_i >=VIDREC_AVI_FRAME_MAX_COUNT)
			{
				VidRec_RecordStop();
				g_VidRecCntx.vidrec_callback(MDI_RES_VDOREC_REACH_SIZE_LIMIT);
				diag_printf("Video Record File Reach Maximum!\n");
				return MDI_RES_VDOREC_REACH_SIZE_LIMIT;
			}

			if (count !=avi_frame_length)
			{
				diag_printf("Video Record Finished, Write file error!\n");
				VidRec_RecordStop();
				g_VidRecCntx.vidrec_callback(MDI_RES_VDOREC_ERR_FAILED);
				return MDI_RES_VDOREC_ERR_FAILED;
			}

		}
	}
	amr_in->VocState = VIDREC_VOC_IDLE;

	//MCI_TRACE (MCI_VIDREC_TRC,0,"******* end to write avi: %d--*********",hal_TimGetUpTime());
	return NO_ERRO;
}

extern void set_mmc_camera_preview();

int32 VidRec_PreviewStart(MMC_VDOREC_SETTING_STRUCT* previewPara, void(*vid_rec_finish_ind)(uint16 msg_result) )
{
	VOID*  InvalideAddress = (VOID*)convertaddr(NULL);

	MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MMC_VIDREC]VidRec_PreviewStart!");
		 
	if ((videoRecordState == VidrecStopStatus) || (videoRecordState == NoneStatus)) // actually VidrecStopStatus = NoneStatus 
	{
		videoRecordState = VidrecPreviewStatus;

		// copy user's setting to video recorder's context, and save it.
		memset(&g_VidRecCntx, 0x00, sizeof(VIDREC_CONTEXT));
		*(&(g_VidRecCntx.settingInfo)) = *previewPara;
		g_VidRecCntx.vidrec_callback=vid_rec_finish_ind;
			
/*****************************************************************************/
//mmc_memory allocation:
// 1. vidrec_avi_out	--30k
// 2. avi_viidx		--37.5k
// 3. RecPcmBuffer	--6.25k
// 4. AMR_buffer		--32k
// 5. sensorbuf		--300k
// 6. backbuffer		--150k
// 7. vidrec_JpegBuff       --40k
// 8. linebuff_asm      -- 1.25k
// 9. gp_VidRec_Prebuf --previewWidth* previewHeight*2 QVGA Screen:84.375k
//10.vidrec_avi_end	--30k
/*----------------------------------------------*/
//total:        751.375k < 800k
/*****************************************************************************/

		vidrec_pAvi_Buff = (uint8*)convertaddr(mmc_MemMalloc(120* 1024));
		if(vidrec_pAvi_Buff==InvalideAddress)
		{
			MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MMC_VIDREC_ERROR]NO ENOUGH MEMORY!");
			mmc_MemFreeAll();
			return MDI_RES_VDOREC_ERR_MEMORY_NOT_ENOUGH;
		}

		vidrec_avi_out = vidrec_pAvi_Buff;

		avi_viidx = (AVI_video_index *)mmc_MemMalloc((int32)VIDREC_AVI_FRAME_MAX_COUNT * sizeof(AVI_video_index)); 
		RecPcmBuffer = (uint8 *)convertaddr(mmc_MemMalloc((int32)PCM_REC_BUF_SIZE * sizeof(uint8)));  

		AMR_buffer = (uint8 *)convertaddr((mmc_MemMalloc(32*VSRECBUF_LEN)));

		if(avi_viidx==NULL||RecPcmBuffer==InvalideAddress||AMR_buffer==InvalideAddress)
		{
			MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MMC_VIDREC_ERROR]NO ENOUGH MEMORY!");
			mmc_MemFreeAll();
			return MDI_RES_VDOREC_ERR_MEMORY_NOT_ENOUGH;
		}

		AMR_buffer_heard = AMR_buffer_tail = AMR_buffer;

		Msg_Wait_Num=0; 
		Effect_Set_Valid=0;
		AMR_Data_Store=0;
		Vidrec_Sensor_Seq=0;
		
		if(MCI_CamPowerUp(1,(MMC_CAMERA_CB)NULL)!=0)
		{
			MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MMC_VIDREC_ERROR]MCI_CamPowerUp FAILE!!");
			return MDI_RES_VDOREC_ERR_FAILED;   
		}
		
		lily_transfer_data();


		vidrec_JpegBuff0 = (int8* )convertaddr((mmc_MemMalloc(40* 1024)));

		if(vidrec_JpegBuff0==InvalideAddress)
		{
			MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MMC_VIDREC_ERROR]NO ENOUGH MEMORY!");
			VidRec_PreviewStop();
			return MDI_RES_VDOREC_ERR_MEMORY_NOT_ENOUGH;
		}

		
		vidrec_JpegBuff1 = vidrec_JpegBuff0+20*1024;
				
		linebuff_asm=(int8* )mmc_MemMalloc(320*4);
		gp_VidRec_Prebuf=(int8* )convertaddr(mmc_MemMalloc(previewPara->previewWidth* previewPara->previewHeight*2));

		if(linebuff_asm==NULL||gp_VidRec_Prebuf==InvalideAddress)
		{
			MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MMC_VIDREC_ERROR]NO ENOUGH MEMORY!");
			VidRec_PreviewStop();
			return MDI_RES_VDOREC_ERR_MEMORY_NOT_ENOUGH;
		}


		 vidrec_avi_end =(uint8*)mmc_MemMalloc(30*1024);// (uint8*)mmc_MemMalloc( mmc_MemGetFree()- 1024);//VIDREC_AVI_FRAME_MAX_COUNT* 32 + 8  );  // 225*3k

		if(vidrec_avi_end==NULL)
		{
			MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MMC_VIDREC_ERROR]NO ENOUGH MEMORY!");
			VidRec_PreviewStop();
			return MDI_RES_VDOREC_ERR_MEMORY_NOT_ENOUGH;
		}

		MCI_TRACE (MCI_VIDREC_TRC,0,"[MMC_VIDREC]vidrec_avi_out = 0x%x",vidrec_avi_out);
		MCI_TRACE (MCI_VIDREC_TRC,0,"[MMC_VIDREC]avi_viidx =0x%x",avi_viidx); 
		MCI_TRACE (MCI_VIDREC_TRC,0,"[MMC_VIDREC]RecPcmBuffer = 0x%x",RecPcmBuffer);      
		MCI_TRACE (MCI_VIDREC_TRC,0,"[MMC_VIDREC]AMR_buffer_heard = 0x%x,AMR_buffer_tail = 0x%x,AMR_buffer = 0x%x",AMR_buffer_heard,AMR_buffer_tail,AMR_buffer);
		MCI_TRACE (MCI_VIDREC_TRC,0,"[MMC_VIDREC]vidrec_JpegBuff0 = 0x%x",vidrec_JpegBuff0);
		vidrec_VideoSampleStart(previewPara);

		return MDI_RES_VDOREC_SUCCEED;
	}
	return MDI_RES_VDOREC_ERR_FAILED;
}

void Mic_RecMsgHandle_Half(VOID){
	MCI_TRACE (MCI_VIDREC_TRC,0,"Mic_Interrupt_half start!Msg_Wait_Num=%d",Msg_Wait_Num);	
	if ((videoRecordState == VidrecRecordStatus) && (vidrec_VocFirstSchedule > 0) )
	{
		Pcm_StartPosition=0;
		Voc_Amr_Need=1;
		Voc_Amr_Count=0;
		if(Msg_Wait_Num<=0)
			VidRec_Pcm2Amr();
	}
	MCI_TRACE (MCI_VIDREC_TRC,0,"Mic_Interrupt_half over!");		
}
void Mic_RecMsgHandle_End(VOID){
	MCI_TRACE (MCI_VIDREC_TRC,0,"Mic_Interrupt_end start!Msg_Wait_Num=%d",Msg_Wait_Num);		
	if ((videoRecordState == VidrecRecordStatus) && (vidrec_VocFirstSchedule > 0) )
	{
		Pcm_StartPosition=1;
		Voc_Amr_Need=1;
		Voc_Amr_Count=0;
		if(Msg_Wait_Num<=0)
			VidRec_Pcm2Amr();
	}
	MCI_TRACE (MCI_VIDREC_TRC,0,"Mic_Interrupt_end over!");		
}

// Open mic for video recoder zhangl 2008-11-03

AUD_ERR_T VidRec_MicStart(VOID){
    AUD_LEVEL_T audioCfg;
    AUD_ERR_T audErr;
    HAL_AIF_STREAM_T pcmStream;

    // Set audio interface 
    audioCfg.spkLevel = AUD_SPK_MUTE;       
    audioCfg.micLevel = AUD_MIC_ENABLE;
    audioCfg.sideLevel = AUD_SIDE_VOL_3;//AUD_SIDE_MUTE;
    audioCfg.toneLevel = 0;
            
    // PCM Audio stream, input of VPP
    pcmStream.startAddress = (UINT32*)RecPcmBuffer;
    pcmStream.length = PCM_REC_BUF_SIZE;//sizeof(HAL_SPEECH_PCM_BUF_T);
    pcmStream.sampleRate = HAL_AIF_FREQ_8000HZ;
    pcmStream.channelNb = HAL_AIF_MONO;
    pcmStream.voiceQuality = TRUE;
    pcmStream.halfHandler = Mic_RecMsgHandle_Half; // Mechanical interaction with VPP's VOC
    pcmStream.endHandler = Mic_RecMsgHandle_End;//NULL; // Mechanical interaction with VPP's VOC

    audErr = aud_StreamRecord(audioItf, &pcmStream, &audioCfg);     

    if(audErr==AUD_ERR_NO)
        MCI_TRACE (MCI_VIDREC_TRC,0,"----audio recoder of the vr is started: ok-----");
    else
        MCI_TRACE (MCI_VIDREC_TRC|TSTDOUT,0,"[MMC_VIDREC_ERROR]audio recoder of the video recorder is started: failure with %d-----",audErr);
            
    return audErr;
}

/*+ zhouqin modify for video record in secord card20110530*/
#if defined(DUAL_TFLASH_SUPPORT)
extern BOOL mmi_vdorec_check_storage(void);
#endif
/*- zhouqin modify for video record in secord card20110530*/
int32 VidRec_RecordStart(HANDLE filehandle)
{

	g_VidRecCntx.fileHandle = filehandle;
    if (videoRecordState == VidrecPreviewStatus)
    {
		MCI_TRACE(MCI_VIDREC_TRC|TSTDOUT , 0,"[MMC_VIDREC]VidRec_RecordStart!");

/*******initialization of  the file process ********/

        /*+ zhouqin modify for video record in secord card20110530*/
        #if defined(DUAL_TFLASH_SUPPORT)
        if(mmi_vdorec_check_storage()== TRUE)
        FS_GetFSInfo("mmc1", &g_FsVidRecInfo);
        else
        #endif
        /*- zhouqin modify for video record in secord card20110530*/
	 FS_GetFSInfo("mmc0", &g_FsVidRecInfo);

        if ((g_FsVidRecInfo.iUsedSize + AVI_TF_MIN*1024) >= g_FsVidRecInfo.iTotalSize)
        {
            videoRecordState = VidrecStopStatus;

            MCI_CamPowerDown(); 

            diag_printf("Video Record Finished, DISK_FULL!\n");
	      g_VidRecCntx.vidrec_callback(MDI_RES_VDOREC_DISK_FULL);
            return MDI_RES_VDOREC_REACH_DISK_LIMIT;
        }  
		
        memset(vidrec_avi_head, 0x00, AVI_HEAD_SIZE);

        FS_Write(g_VidRecCntx.fileHandle, vidrec_avi_head, AVI_HEAD_SIZE);

        Voc_AmrEnc_Busy=0;
        Voc_JpegEnc_Busy=0;

        // ////////////////// AVI info init, video record context init and voc init  //////////////////////
        if (VidRec_Init("", &g_VidRecCntx) != NO_ERRO)
            return MDI_RES_VDOREC_ERR_FAILED;

	g_AMR_DataInCount = 0;

	if (g_VidRecCntx.settingInfo.IfRecordAudio)
	{
            diag_printf( "----sound recorder is started----\n" );
            VR_Init=0;
            VR_Amr_Init=0;
            Voc_Amr_Count=0;
            memset(RecPcmBuffer, 0x0, PCM_REC_BUF_SIZE*sizeof(uint8));
            memset(AMR_buffer, 0, 32*VSRECBUF_LEN*sizeof(uint8));
            VidRec_MicStart();
      
        }
        else        
        {
            // fill the AMR output buffer with 0
            memset(AMR_buffer, 0, 32*VSRECBUF_LEN*sizeof(uint8));
      
        }

        videoRecordState = VidrecRecordStatus;
        Avi_Out_Index=0;
	vidrec_AviNuminBuff=0;
	Voc_JpegOut_Seq=0;
	vidrec_pJpegVocOut = vidrec_JpegBuff1;
	vidrec_pJpegAviIn = vidrec_JpegBuff0;
	vidrec_pJpeg_sizeVoc = &vidrec_Jpeg1_size;		
	vidrec_pJpeg_sizeAvi = &vidrec_Jpeg0_size;		

        g_VidRecCntx.recordStartTime = hal_TimGetUpTime();//cyg_current_time(); changde by zhangl
        //   test_video=0;
        return MDI_RES_VDOREC_SUCCEED;
    }
    return MDI_RES_VDOREC_ERR_FAILED;
}

int32 VidRec_RecordPause(void)
{
    if (videoRecordState == VidrecRecordStatus)
    {
	videoRecordState = VidrecPauseStatus;

        if (g_VidRecCntx.settingInfo.IfRecordAudio)
        {

            aud_StreamPause(audioItf, TRUE);
        }

        g_VidRecCntx.VocState = VIDREC_VOC_IDLE;

        g_VidRecCntx.RGBBufStatus[0] = RGB_BUF_NULL;
        g_VidRecCntx.RGBBufStatus[1] = RGB_BUF_NULL;

        g_VidRecCntx.lastRecordPeriod +=
            (hal_TimGetUpTime()/*cyg_current_time()*/ - g_VidRecCntx.recordStartTime) / VIDREC_DEFAULT_FREQ_TICK_COUNT_ONE_SECOND;
	}

    return MDI_RES_VDOREC_SUCCEED;
}

int32 VidRec_RecordResume(void)
{
	if (videoRecordState == VidrecPauseStatus)
	{
		if (g_VidRecCntx.settingInfo.IfRecordAudio)
		{
			// begint to sound sample
			AMR_buffer_heard = AMR_buffer_tail = AMR_buffer;
			Avi_Out_Index=0;
			AMR_Data_Store=0;
			Voc_AmrEnc_Busy=0;
			Voc_JpegEnc_Busy=0;
			aud_StreamPause(audioItf, FALSE);
		}

		// begin to count time
		g_VidRecCntx.recordStartTime = hal_TimGetUpTime();//cyg_current_time(); changed by zhangl
		videoRecordState = VidrecRecordStatus;
	}

	return MDI_RES_VDOREC_SUCCEED;
}

void VidRec_Pcm2Amr(void)
{     
	if ((Voc_JpegEnc_Busy==0)&&Voc_AmrEnc_Busy==0)//&&(!((PCM_BUF_INT=1)&&(VOC_AMRENC_BUSY))))
	{
		vpp_AudioJpeg_ENC_IN_T   audio_amrenc;

		Voc_AmrEnc_Busy=1;
		Voc_Interrupt_From=1;
		Voc_Amr_Count++;    
		//Voc_Amr_Need=1;

		audio_amrenc.inStreamBufAddr=(INT32*)(RecPcmBuffer+(Pcm_StartPosition*PCM_REC_BUF_SIZE/2)+(Voc_Amr_Count-1)*320);
		audio_amrenc.outStreamBufAddr=(INT32*)AMR_buffer_tail;
		AMR_buffer_tail+=32;
		if((AMR_buffer_tail-AMR_buffer)>=32*VSRECBUF_LEN)
			AMR_buffer_tail=AMR_buffer;

		audio_amrenc.mode=MMC_AMR_ENCODE;
		audio_amrenc.channel_num = 1;   
		audio_amrenc.SampleRate= HAL_AIF_FREQ_8000HZ;
		audio_amrenc.reset=0;
		if (VR_Amr_Init==0)
		{
			VR_Amr_Init=1;
			audio_amrenc.reset=1;
		}

		if(HAL_ERR_NO!=vpp_AudioJpegEncScheduleOneFrame(&audio_amrenc)) 
		{
			Voc_AmrEnc_Busy=0;
			diag_printf ("Failed to call VOC in VidRec_Pcm2Amr :%d th",Voc_Amr_Count);
		}
	}
	else
	{
		//Voc_Amr_Need=1;
		MCI_TRACE (MCI_VIDREC_TRC,0,"------%d pcm2amr need waiting,Voc_JpegEnc_Busy=%d,Voc_AmrEnc_Busy=%d",Voc_Amr_Need,Voc_JpegEnc_Busy,Voc_AmrEnc_Busy);
	}

}

extern volatile int32 sensorready;

void VidRec_CallbackFun(UINT8 id,  UINT8 ActiveBufNum)
{
	
	Cam_Data_Valid=id;
	
	MCI_TRACE (MCI_VIDREC_TRC,0,"[MMC_VIDREC]VidRec_CallbackFun TIME:%d,NUM:%d,ID:%d",hal_TimGetUpTime(),Msg_Wait_Num,id);
	sensorready = 1;
	//send message for preview and capture the next picture
	if(Msg_Wait_Num<=0)
	{
		mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VIDREC_SENSOR_IMAGE_READY);
		Msg_Wait_Num++;
	}
	else
	{
		MCI_TRACE (MCI_VIDREC_TRC,0,"[MMC_VIDREC]one sensor image is discarded,Msg_Wait_Num=%d",Msg_Wait_Num);
	}


	//start to capture image data to sensorbuf
	if( Vidrec_Sensor_Seq==0)
	{
		camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, MY_NPIX, sensorbuf);
		Vidrec_Sensor_Seq=1;	
	}
	else
	{
		camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, MY_NPIX, sensorbuf1);
		Vidrec_Sensor_Seq=0;
	}
	camera_CaptureImage();    
       
// diag_printf("12:the last four bytes of vidrec_avi_out :%x \n",*((U32*)(((U32)vidrec_avi_out|0xa0000000)+50* 1024)));
}


extern void VidRec_yuv2rgb(char* srcbuf,int src_w,int src_h,char* desbuf,int padding);
extern void VidRec_yuv2rgb_blend(char* srcbuf,int src_w,int src_h,char* desbuf,char*backbuf,int padding);
extern void VidRec_bilinear_zoom(char* srcbuf,int src_w,int src_h,int cut_w,int cut_h,char* decbuf,int dec_w,int dec_h,char* linebuff);



void VidRec_UserMsg(COS_EVENT *pnMsg)
{
	switch (pnMsg->nEventId)
	{
		case  MSG_VIDREC_SENSOR_IMAGE_READY:	
		//MCI_TRACE (MCI_VIDREC_TRC,0,"MSG_VIDREC_SENSOR_IMAGE_READY");
			VidRec_PreCap();					
			break;
		case MSG_VIDREC_AVI_OUTPUT_REQ:
			//MCI_TRACE (MCI_VIDREC_TRC,0,"MSG_VIDREC_AVI_OUTPUT_REQ");
			VidRec_FrameOutput();
			break;
		case EV_TIMER:
			switch(pnMsg->nParam1)
			{
				case MMC_CAMERA_TIMER_ID:
					MCI_TRACE(MCI_VIDREC_TRC, 0,"[MMC_VIDREC]Camera_UserMsg  EV_TIMER");
					if(bset<2)//bset==2 means the camera is power off
					Lily_Camera_SetEffect(CAM_PARAM_WB,(uint32)g_VidRecCntx.settingInfo.whiteBalance);
					break;
				default:
					MCI_TRACE(MCI_VIDREC_TRC, 0,"[MMC_VIDREC]Invalid timer ID in camera");
					break;
			}			
			break;
		default:
		break;
	}
	
}

/**************************************************************
		image from sensor is 160*120, the selected view&record range is 
		92*120, the followint rotate function will clockwise rotate the 
		selected image for LCD size 320*240,dedicated for project N9580
		created by liyongjian  08162009
***************************************************************/
void yuv422_rotate(uint16 * sourcebuf,uint16 * rotatedbuf)
{
	uint8* psrc;
	uint8* pdes;
	uint8 *psrcline;
	uint8 *pdesrow;
	int i,j;
	
	psrc = (uint8*)((uint32)sourcebuf&0xdfffffff) + (34*2);
	pdes = (uint8*)(rotatedbuf + 160*105+20);

	for(i=0;i<60;i++){
		psrcline = psrc + 160*2*i*2;
		pdesrow= pdes+2*i*2;
		for(j=0;j<46;j++){
			*pdesrow = *psrcline;											//y
			*(pdesrow-160*2+1) = *(pdesrow+1) = *(psrcline+1);				//u
			*(pdesrow-160*2) = *(psrcline+2);								//y
			*(pdesrow+3) = *(pdesrow-160*2+3) = *(psrcline+3);				//v
			*(pdesrow+2) = *(psrcline+160*2);								//y
//				*(pdesrow-160*2+1) = *(psrcline+1);			
			*(pdesrow-160*2+2) = *(psrcline+160*2+2);						//y
//				*(pdesrow+3) = *(psrcline+3);

			psrcline +=2*2;
			pdesrow -= 160*2*2;								
		}
	}
		
}

void VidRec_PreCap(void)
{
    
	//MCI_TRACE (MCI_VIDREC_TRC,0,"[MMC_VIDREC]VidRec_PreCap TIME: %d,Cam_Data_Valid=%d",hal_TimGetUpTime(),Cam_Data_Valid);
	MCI_TRACE (MCI_VIDREC_TRC,0,"[MMC_VIDREC]VidRec_PreCap!");

#if (CSW_EXTENDED_API_LCD == 0)
	uint32  local;
#endif
	uint32 count=0;
		 
	// if video recorder is stopped, as follow not exec.
	if (videoRecordState == VidrecStopStatus) return;
	
	if( Vidrec_Sensor_Seq==0)
	{
		sensor_data_buf=sensorbuf;
	}
	else
	{
		sensor_data_buf=sensorbuf1;
	}

	
	count = (CameraInfo.start_y)*MMC_LcdWidth;
	
	if(g_VidRecCntx.settingInfo.lcdrotate==0)
	{
		cutwidth  = (srcwidth*10/(10+(CameraInfo.previewZoom<<1)))&0xfffffffc;
		cutheight = (srcheight*10/(10+(CameraInfo.previewZoom<<1)))&0xfffffffe;
	}
	else
	{
		//MCI_TRACE (MCI_VIDREC_TRC,0,"----VidRec_rotate time begin: %d---",hal_TimGetUpTime());
		yuv422_rotate((uint16*)sensor_data_buf,sensorbuf2);
		sensor_data_buf = sensorbuf2;
		cutwidth  = (120*10/(10+(CameraInfo.previewZoom<<1)))&0xfffffffc;
		cutheight = (90*10/(10+(CameraInfo.previewZoom<<1)))&0xfffffffe;
		//MCI_TRACE (MCI_VIDREC_TRC,0,"----VidRec_rotate time end: %d---%d,%d,%d",hal_TimGetUpTime(),MMC_LcdWidth,MMC_LcdHeight,cutheight);
		CameraInfo.start_x = 0;
		CameraInfo.start_y = 15;		
	}
   
	if ((videoRecordState == VidrecRecordStatus) && (vidrec_VocFirstSchedule > 0) )
	{
		if(Cam_Data_Valid==0)
		{
			if (Voc_AmrEnc_Busy==0 && Voc_JpegEnc_Busy ==0)
			{
				Voc_JpegEnc_Busy=1;
				Voc_Interrupt_From=0;
				Voc_Jpg_Need=0;

				VidRec_JPEGEncode((int32 *)(sensor_data_buf+srcwidth*((srcheight-cutheight)>>1)+((srcwidth-cutwidth)>>1)),CameraInfo.image_width,CameraInfo.image_height);

			}
			else
			{
				MCI_TRACE (MCI_VIDREC_TRC,0,"[MMC_VIDREC_WARN]1 sensor image waiting,Voc_JpegEnc_Busy=%d,Voc_AmrEnc_Busy=%d",Voc_JpegEnc_Busy,Voc_AmrEnc_Busy);
				Voc_Jpg_Need=1;
			}   
		}

	}
	//MCI_TRACE (MCI_VIDREC_TRC,0,"VidRec_preview begin: %d",hal_TimGetUpTime());
   	
	if((Cam_Data_Valid==0)&&(Effect_Set_Valid==0))
	{ 
	       LCDD_ERR_T err;
		//zoom and yuv2rgg565 for preview display
		VidRec_bilinear_zoom((uint8*)((uint32)sensor_data_buf&0xdfffffff),(int)srcwidth,(int)srcheight,(int)cutwidth,(int)cutheight,(uint8*)gp_VidRec_Prebuf,(int)CameraInfo.preview_width,(int)CameraInfo.preview_height,linebuff_asm); 
#if (CSW_EXTENDED_API_LCD == 1)
		if(g_VidRecCntx.settingInfo.lcdrotate==0)
		{
                
			//display video recorder's info bar
			displayFbw.fb.buffer = (UINT16*) backbuffer;
			displayFbw.fb.width = MMC_LcdWidth;
			displayFbw.fb.height = MMC_LcdHeight;
			displayFbw.fb.colorFormat = LCDD_COLOR_FORMAT_RGB_565;
			displayFbw.roi.width = MMC_LcdWidth;
			displayFbw.roi.height = MMC_LcdHeight-(CameraInfo.preview_height+CameraInfo.start_y);
			displayFbw.roi.x = 0;
			displayFbw.roi.y = CameraInfo.start_y+CameraInfo.preview_height;

			err = lcdd_Blit16(&displayFbw,0,CameraInfo.start_y+CameraInfo.preview_height);
			while (LCDD_ERR_NO != err)
			{
			    sxr_Sleep(10);
			    err = lcdd_Blit16(&displayFbw,0 ,CameraInfo.start_y+CameraInfo.preview_height);
			}
			VidRec_yuv2rgb_blend((int8*)gp_VidRec_Prebuf,(int)CameraInfo.preview_width,(int)CameraInfo.preview_height,(int8*)(blendBuffer+count),(int8*)(backbuffer+count),(MMC_LcdWidth-(int)CameraInfo.preview_width)*2);
			displayFbw.fb.buffer = (UINT16*) blendBuffer;
			displayFbw.fb.width = MMC_LcdWidth;
			displayFbw.fb.height = MMC_LcdHeight;
			displayFbw.fb.colorFormat = LCDD_COLOR_FORMAT_RGB_565;
			displayFbw.roi.width = CameraInfo.preview_width;
			displayFbw.roi.height = CameraInfo.preview_height;
			displayFbw.roi.x = 0;
			displayFbw.roi.y = CameraInfo.start_y;
						
			// Display the blended image until lcdd is available
			err = lcdd_Blit16(&displayFbw,0,CameraInfo.start_y);
		        while (LCDD_ERR_NO != err)
		        {
		            sxr_Sleep(10);
			    err = lcdd_Blit16(&displayFbw,0 ,CameraInfo.start_y);
		        }
		}
		else
		{
			VidRec_yuv2rgb((int8*)gp_VidRec_Prebuf,(int)CameraInfo.preview_width,(int)CameraInfo.preview_height,(int8*)(backbuffer+count),(MMC_LcdWidth-(int)CameraInfo.preview_width)*2);
			displayFbw.fb.buffer = (UINT16*) backbuffer;
			displayFbw.fb.width = MMC_LcdWidth;
			displayFbw.fb.height = MMC_LcdHeight;
			displayFbw.fb.colorFormat = LCDD_COLOR_FORMAT_RGB_565;
			displayFbw.roi.width = MMC_LcdWidth;
			displayFbw.roi.height = MMC_LcdHeight;
			displayFbw.roi.x = 0;
			displayFbw.roi.y = 0;
						
			// Display the blended image until lcdd is available
			 err = lcdd_Blit16(&displayFbw,0,0);
		        while (LCDD_ERR_NO != err)
		        {
		            sxr_Sleep(10);
			    err = lcdd_Blit16(&displayFbw,0 ,0);
		        }			
		}
			
#else
		diag_printf ("[MMC_VIDREC]need some new LCD display codes!\n");
	        local=get_lcd_frame_buffer_address();
	        set_lcd_frame_buffer_address((kal_uint32)blendBuffer);
	        previewstatus = 0;
	        MainLCD->BlockWrite(0,0,MMC_LcdWidth-1,MMC_LcdHeight-1);
	        previewstatus = 1;
	        set_lcd_frame_buffer_address(local);
#endif
	}

	if ((videoRecordState == VidrecRecordStatus) && (vidrec_VocFirstSchedule > 0) )
	{
		mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_VIDREC_AVI_OUTPUT_REQ);	
	}
	
	Effect_Set_Valid=0;    
	Msg_Wait_Num--;
	
	//MCI_TRACE (MCI_VIDREC_TRC,0,"VidRec_PreCap end: %d",hal_TimGetUpTime());
}



void	VidRec_FrameOutput(void)
{
	error_type avi_inte_result=NO_ERRO;
	do{
		MCI_TRACE (MCI_VIDREC_TRC,0,"[MMC_VIDREC]AMR_Data_Store:%d",AMR_Data_Store);
		if(Avi_Out_Index>0)
		{
			if(NO_ERRO!=(avi_inte_result=vidrec_EncodeRun(&g_VidRecCntx)))
			{
				MCI_TRACE (MCI_VIDREC_TRC|TSTDOUT,0,"[MMC_VIDREC_ERROR]vidrec_EncodeRun Error:%d",avi_inte_result);
				g_VidRecCntx.vidrec_callback((uint16)avi_inte_result);
				return;
			}
		}
		else
		{
			MCI_TRACE (MCI_VIDREC_TRC|TSTDOUT,0,"[MMC_VIDREC]THE FISRST FRAME IS BAD!");
			break;
		}
	}while(AMR_Data_Store>=10&&Voc_JpegEnc_Busy==0);
}



void VidRec_JPEGEncode(int32* buf_in,short width,short height)
{
  vpp_AudioJpeg_ENC_IN_T VocMP4EncIn; 

  VocMP4EncIn.SampleRate=(cutheight<<16)|cutwidth;
  VocMP4EncIn.mode        = MMC_MJPEG_ENCODE;  // update ram according to global  mode
  VocMP4EncIn.imag_width  = width;//VIDEO_RECORD_DISP_WIDTH;
  VocMP4EncIn.imag_height = height;//VIDEO_RECORD_DISP_HEIGHT;
  g_VidRecCntx.settingInfo.encodeQuality=1;

  if (g_VidRecCntx.settingInfo.encodeQuality == VID_REC_QTY_LOW)
  {
    VocMP4EncIn.imag_quality = 0;
  }
  else
  {
    VocMP4EncIn.imag_quality = 1; 
  }
g_VidRecCntx.VocState = VIDREC_JPEG_ENC_BUSY;

  VPP_WRITE_JPEGHeadr(VocMP4EncIn.imag_quality, width,height,(uint8*)vidrec_pJpegVocOut);


  VocMP4EncIn.inStreamBufAddr  = (INT32*)buf_in;//gp_VideoRecordAFrameRGBBuf;

  VocMP4EncIn.outStreamBufAddr = (INT32*)(vidrec_pJpegVocOut+ 608);


  VocMP4EncIn.channel_num = 1;
   if (VR_Init==0)
   {
	VR_Init=1;
	VocMP4EncIn.reset=1;
   }
	if(HAL_ERR_NO!=vpp_AudioJpegEncScheduleOneFrame(&VocMP4EncIn))
	{
		Voc_JpegEnc_Busy=0;
		diag_printf ("[MMC_VIDREC_ERROR]Failed to call VOC in VidRec_JPEGEncode");
  	}

}



int32 vidrec_VideoSampleStart(MMC_VDOREC_SETTING_STRUCT* previewPara)
{
  CAM_PREVIEW_STRUCT cam_para_temp;
	
//	MCI_TRACE (MCI_VIDREC_TRC,0,"previewStartX:%d--previewStartY:%d",previewPara->previewStartX,previewPara->previewStartY);
//	MCI_TRACE (MCI_VIDREC_TRC,0,"previewWidth:%d--previewHeight:%d",previewPara->previewWidth,previewPara->previewHeight);
//	MCI_TRACE (MCI_VIDREC_TRC,0,"imageWidth:%d--imageHeight:%d",previewPara->imageWidth,previewPara->imageHeight);
//	MCI_TRACE (MCI_VIDREC_TRC,0,"nightMode:%d--encodeQuality:%d",previewPara->nightMode,previewPara->encodeQuality);
//	MCI_TRACE (MCI_VIDREC_TRC,0,"zoomFactor:%d--specialEffect:%d",previewPara->zoomFactor,previewPara->specialEffect);
//	MCI_TRACE (MCI_VIDREC_TRC,0,"whiteBalance:%d--contrast:%d--brightnessLevel:%d",previewPara->whiteBalance,previewPara->contrast,previewPara->brightnessLevel);

	
       cam_para_temp.image_height=previewPara->imageHeight;
	cam_para_temp.image_width=previewPara->imageWidth;
  	cam_para_temp.start_x=previewPara->previewStartX;
  	cam_para_temp.start_y=previewPara->previewStartY;
  	cam_para_temp.end_x=previewPara->previewWidth + previewPara->previewStartX-1;
  	cam_para_temp.end_y=previewPara->previewHeight + previewPara->previewStartY-1;
	cam_para_temp.nightmode=previewPara->nightMode;
	cam_para_temp.imageQuality=previewPara->encodeQuality;
	cam_para_temp.factor=previewPara->zoomFactor;
	cam_para_temp.specialEffect=previewPara->specialEffect;
	cam_para_temp.whiteBlance=previewPara->whiteBalance;
	cam_para_temp.contrast = previewPara->contrast;
	cam_para_temp.brightNess = previewPara->brightnessLevel;
	

       set_mmc_camera_preview();	
  	MCI_CamPreviewOpen(&cam_para_temp);

	return 0;
}

 void VidRec_FS_AsynWrite_CB(void* pParam)
 {
    FS_ASYN_READ_RESULT  *sRsp ;  

    sRsp = (FS_ASYN_READ_RESULT*)pParam;
    if(sRsp->iResult >= 0)
    {
 	vidrec_AviNuminBuff--;
    }
    else //fs write file
    {
        vidrec_AviNuminBuff = 0;

        diag_printf("[MMC_VIDREC_ERROR]FS write fail!");
        VidRec_RecordStop();
	g_VidRecCntx.vidrec_callback(MDI_RES_VDOREC_ERR_FAILED);
    }
    return;
 }
 
#endif // (CHIP_HAS_GOUDA != 1)

