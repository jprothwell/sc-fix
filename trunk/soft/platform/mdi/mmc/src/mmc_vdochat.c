//coolsand,chenhe ,20100707,create for kk project

#include "mcip_debug.h"
#include "mci.h"
#include "camera_m.h"
#include "mmc.h"
#include "lcdd_m.h"
#include "mmc_camera.h"
#include "pmd_m.h"
#include "mmc_vdochat.h"
#include "mci_lcd.h"

// video chat is currently just supported on greenstone
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)

//macro
#define VDOCHAT_SENSOR_DATA_WIDTH 160
#define VDOCHAT_SENSOR_DATA_HEIGHT 120
#define VDOCHAT_CAM_SIZE CAM_NPIX_QQVGA

//extern
extern uint16 *sensorbuf;
extern uint16 *sensorbuf1;
extern uint16 *blendBuffer;
extern uint16 srcwidth;
extern uint16 srcheight;
extern INT32 MMC_LcdWidth;
extern INT32 MMC_LcdHeight;
#if (CSW_EXTENDED_API_LCD == 1)
extern LCDD_FBW_T displayFbw;
#endif

extern void* memset(void* dst0, int c0, size_t length);
extern void* memcpy(void* dst, const void* src, UINT32 len);
extern void sxr_Sleep (u32 Period);
extern int  Coolsand_bilinerz_zoom_op_yuv2rgb(uint16* srcbuf,uint16 src_w,uint16 src_h,uint16 cut_w,uint16 cut_h,uint16* decbuf,uint16 dec_w,uint16 dec_h);
extern void VidRec_bilinear_zoom(char* srcbuf,int src_w,int src_h,int cut_w,int cut_h,char* decbuf,int dec_w,int dec_h,char* linebuff);
extern INT32 initVocJpegEncode(void);
extern void quitVocJpegEncode(void);
extern uint32 getJpegEncodeLen(void);
extern void  MMC_jpgEn(short width,short height, short quality,char* bufin,char* bufout,short yuvmode);
extern uint32 getJpegEncodeLen(void);
extern  VOID hal_GdbFlushCacheData(VOID);

//global varaible and flags
BOOL vdochat_prevFlag=0;
UINT32 vdochat_preview_width = 0;
UINT32 vdochat_preview_height = 0;
UINT32 vdochat_preview_startX = 0;
UINT32 vdochat_preview_startY = 0;
UINT16 *pSensorbuffer = NULL;
static    BOOL vdochat_IsGetRGB =FALSE;
volatile int32 vdochat_Sensor_Seq=0;
 uint16 * vdochat_sensor_buf;
 uint8 * vdochat_linebuff= NULL;

void vdochat_Callback(UINT8 id,  UINT8 ActiveBufNum)
{
    MCI_TRACE (MCI_VIDREC_TRC,0,"[MMC_VIDREC]vdochat_Callback TIME:%d,ID:%d,vdochat_prevFlag is %d",hal_TimGetUpTime(),id,vdochat_prevFlag);

    if( vdochat_prevFlag == FALSE )
    {
        mmc_vdochat_previewStop();
        return;
    }

    if( id != 0)
    {
        camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, VDOCHAT_CAM_SIZE, sensorbuf);
        camera_CaptureImage();
        return;
    }

    mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), VDOCHAT_PRIVIEW);
}
int MMC_EncodeFrame(unsigned char  * RgbData, int RgbDataSize,int image_width,int image_height,int out_jpeg_width, int out_jpeg_height, 
	unsigned char *p_Out,int bufsize,int q)

{
	uint32 jpegsize;
		if(RgbData == NULL) return 0;
		if(initVocJpegEncode() <0) return 0;//init encode jpeg voc
		if( (image_width ==out_jpeg_width) && (image_height == out_jpeg_height))
		{
			MMC_jpgEn(out_jpeg_width,out_jpeg_height,q,(char*)RgbData,(char*)p_Out,1);
		}
		else
		{
			CoolSand_Cut_Zoom((UINT16 *)RgbData,image_width, image_height,image_width, image_height,(UINT16 *)p_Out,out_jpeg_width, out_jpeg_height);
				                                
			MMC_jpgEn(image_width,image_height,q,(char*)p_Out,(char*)p_Out,1);

		}
             jpegsize = getJpegEncodeLen();
		quitVocJpegEncode();
	return  jpegsize;
}
	
void vdochat_dsp(void)
{
    hal_GdbFlushCacheData();

    Coolsand_bilinerz_zoom_op_yuv2rgb(sensorbuf, srcwidth, srcheight, srcwidth, srcheight, 
        blendBuffer, vdochat_preview_width, vdochat_preview_height);

   
#if (CSW_EXTENDED_API_LCD == 1)
    displayFbw.fb.buffer = (UINT16*) blendBuffer;
    LCDD_ERR_T  err = lcdd_Blit16(&displayFbw, vdochat_preview_startX, vdochat_preview_startY);
    while (LCDD_ERR_NO != err)
    {
        sxr_Sleep(10);
        err = lcdd_Blit16(&displayFbw,vdochat_preview_startX , vdochat_preview_startY);
    };
#endif

    if(0 == vdochat_Sensor_Seq)
    {
    	camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, VDOCHAT_CAM_SIZE, sensorbuf);
    }		
   else
   {
    	camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, VDOCHAT_CAM_SIZE, sensorbuf1);
   }
    camera_CaptureImage();  
}

void mmc_vdochat_previewStart(UINT32 startX,UINT32 startY,UINT32 preW,UINT32 preH)
{
    MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_VDOCHAT]vdochat_previewStart");
    
    vdochat_prevFlag = TRUE;  
    
    vdochat_preview_width = preW;
    vdochat_preview_height = preH;
    vdochat_preview_startX = startX;
    vdochat_preview_startY = startY;

    displayFbw.fb.height = preH;
    displayFbw.fb.width = preW;
    displayFbw.roi.height = preH;
    displayFbw.roi.width = preW;
    displayFbw.roi.x = 0;
    displayFbw.roi.y = 0;

   vdochat_Sensor_Seq = 0;
   camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, VDOCHAT_CAM_SIZE, sensorbuf);
   camera_CaptureImage();
}

void mmc_vdochat_previewStop(void)
{
    MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,  "[MMC_CAMERA]vdochat_previewStop");
    
    vdochat_prevFlag = FALSE;
    vdochat_preview_width = 0;
    vdochat_preview_height = 0;
    vdochat_preview_startX = 0;
    vdochat_preview_startY = 0;
}

UINT8 *mmc_vdochat_getData(UINT16 width, UINT16 height)
{
       if( vdochat_prevFlag == FALSE ) return NULL;

	if(vdochat_Sensor_Seq == 0)
	{
		vdochat_Sensor_Seq = 1;
		vdochat_sensor_buf = sensorbuf;
	}
	else 
	{
		vdochat_Sensor_Seq = 0;
		vdochat_sensor_buf =sensorbuf1;
	}
	   
	if(vdochat_IsGetRGB)
	{
	    Coolsand_bilinerz_zoom_op_yuv2rgb(vdochat_sensor_buf, srcwidth, srcheight, srcwidth, srcheight, 
            pSensorbuffer, width,height);

	}
      else 
      	{
		VidRec_bilinear_zoom((uint8*)vdochat_sensor_buf,srcwidth, srcheight,srcwidth, srcheight, (uint8*)pSensorbuffer,
		                                          width,height,vdochat_linebuff);
	
      	}
	   return (UINT8*)pSensorbuffer;
 
}

void mmc_vdochat_powerOff(void)
{
    MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,  "[MMC_CAMERA]mmc_vdochat_powerOff");

    mmc_SetCurrMode(MMC_MODE_IDLE);
    if(vdochat_prevFlag == TRUE) mmc_vdochat_previewStop();
    camera_PowerOff();
    mmc_MemFreeAll();
}

int mmc_vdochat_powerOn(BOOL bIsRGB)
{
    UINT8 camId = 0xff;
    UINT16* InvalidateAddress = (UINT16*)convertaddr(NULL);

    MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,  "[MMC_CAMERA]mmc_vdochat_powerOn");

    pmd_EnablePower(PMD_POWER_CAMERA, TRUE);

    mmc_SetCurrMode(MMC_MODE_VDOCHAT);

#if (CSW_EXTENDED_API_LCD == 1)
    memset(&displayFbw, 0, sizeof(LCDD_FBW_T));
    displayFbw.fb.colorFormat = LCDD_COLOR_FORMAT_RGB_565;
#endif

    sensorbuf = (UINT16*)mmc_MemMalloc(VDOCHAT_SENSOR_DATA_WIDTH*VDOCHAT_SENSOR_DATA_HEIGHT*8);
//(UINT16*)convertaddr(mmc_MemMalloc(VDOCHAT_SENSOR_DATA_WIDTH*VDOCHAT_SENSOR_DATA_HEIGHT*8));
    if( sensorbuf == InvalidateAddress)
    {
        MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,  "[MMC_VDOCHAT]Memory Not Enough!");
        return MDI_RES_CAMERA_ERR_MEMORY_NOT_ENOUGH;
    }
    memset(sensorbuf, 0, (VDOCHAT_SENSOR_DATA_WIDTH*VDOCHAT_SENSOR_DATA_HEIGHT*8));

    srcwidth = VDOCHAT_SENSOR_DATA_WIDTH;
    srcheight = VDOCHAT_SENSOR_DATA_HEIGHT;
    sensorbuf1 = sensorbuf + VDOCHAT_SENSOR_DATA_WIDTH*VDOCHAT_SENSOR_DATA_HEIGHT;
    blendBuffer = sensorbuf1 + VDOCHAT_SENSOR_DATA_WIDTH*VDOCHAT_SENSOR_DATA_HEIGHT;

    pSensorbuffer = blendBuffer + VDOCHAT_SENSOR_DATA_WIDTH*VDOCHAT_SENSOR_DATA_HEIGHT;
	
    vdochat_linebuff =(int8* )mmc_MemMalloc(320*4);

    camera_Reserve(vdochat_Callback);
   
#if (CHIP_HAS_GOUDA)
    camera_PowerOn(VDOCHAT_CAM_SIZE, VDOCHAT_SENSOR_DATA_WIDTH, VDOCHAT_SENSOR_DATA_HEIGHT, CAM_FORMAT_RGB565);
#else // !CHIP_HAS_GOUDA
     camera_PowerOn(VDOCHAT_CAM_SIZE, CAM_FORMAT_RGB565);
#endif // !CHIP_HAS_GOUDA

    camId = camera_GetId();
    MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,  "[MMC_VDOCHAT]cameraId %d", camId);
    if( camId == 0xff || camId == 0x1F || camId == 0x1E || camId == 0x1D)
    {
        MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,  "[MMC_VDOCHAT]ID ERROR!");
        mmc_vdochat_powerOff();
        return MDI_RES_CAMERA_ERR_HW_NOT_READY;
    }
	vdochat_IsGetRGB = bIsRGB;
	
    return MDI_RES_CAMERA_SUCCEED;
}

void vdochat_usrmsgHandle(COS_EVENT *pnMsg) 
{
    switch (pnMsg->nEventId)
    {
        case  VDOCHAT_PRIVIEW:	
            MCI_TRACE (MCI_VIDREC_TRC,0,"vdochat_usrmsgHandle: msg 0");
            vdochat_dsp();
            break;
        default:
            break;
    }
}
#else
int   mmc_vdochat_powerOn(BOOL bIsRGB)
{
	return 1;

}
void mmc_vdochat_powerOff(void)
{
}

void mmc_vdochat_previewStart(UINT32 startX,UINT32 startY,UINT32 preW,UINT32 preH)
{
}
void mmc_vdochat_previewStop(void)
{
}
int MMC_EncodeFrame(unsigned char  * RgbData, int RgbDataSize,int image_width,int image_height,int out_jpeg_width, int out_jpeg_height, 
	unsigned char *p_Out,int bufsize,int q)
{
	return 1;
}
UINT8 *mmc_vdochat_getData(UINT16 width, UINT16 height)
{
	return (UINT8*)0;
}
#endif // (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)

