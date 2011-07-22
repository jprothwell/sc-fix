#include "cs_types.h"
#include "sxs_io.h"
#include "hal_camera.h"
#include "hal_gpio.h"
#include "drv_camera.h"
#include "pmd_m.h"
#include "camera_driver.h"
#include "camera_private.h"
#include "tgt_camd_cfg.h"
#include "hal_host.h"
#include "hal_spi.h"
#include "sxr_tls.h"
#include "sxr_mem.h"
#include "hal_sys.h"
#include "hal_timers.h"
#include "camera_m.h"
#define CAM_PAS6175_ID       0x75  
#define PAS6167_SENSOR_ID		(0x6165)
#define PAS6175_SENSOR_ID		(0x6175)


//#define _LCD_CAMERA_USE_SAME_SPI_  //define in def.file
#ifdef _LCD_CAMERA_USE_SAME_SPI_
PRIVATE HAL_SPI_ID_T g_spi_id_sensor = HAL_SPI;
PRIVATE HAL_SPI_CS_T g_spi_cs_sensor = HAL_SPI_CS1;
#else
PRIVATE HAL_SPI_ID_T g_spi_id_sensor = HAL_SPI_2;
PRIVATE HAL_SPI_CS_T g_spi_cs_sensor = HAL_SPI_CS0;
#endif
PRIVATE HAL_I2C_BUS_ID_T g_camdI2cBusId = HAL_I2C_BUS_ID_INVALID;

const UINT8 PAS6175_AeStage_GainStage[][6]	= {
	//R_AE_maxStage[4:0], R_AG_stage_UB, R_AE_stage_LL[4:0], R_AE_stage_NL[4:0], R_Gamma_Strength_Delta[3:0], R_Saturation_LL[4:0]
    {0x14,0x20,0x14,0x10,0x02,0x09},//Normal,
    {0x1B,0x22,0x1B,0x14,0x04,0x07},//Night,
};
const UINT8 PAS6175_WB_MODE[][4] = {
	/* Cmd_Cgn_Gb,	Cmd_Cgn_B, Cmd_Cgn_Gr, Cmd_Cgn_R */
	{   9,  13,   9,  15},//Cloud  //熬屡
	{  10,  14,  10,  14},//DAYLIGHT び锭 //熬独
	{   9,  23,   9,   9},//INCANDESCENCE フ荐 //
	{   9,  15,   9,  15},//FLUORESCENT 棵 //熬屡
	{   7,  20,   7,   7},//TUNGSTEN 码捣縊 //熬屡
};
const UINT8 PAS6175_Effect[][4]	= {
	{0x00, 0x01,0x00, 0x00}, //CAM_EFFECT_ENC_GRAYSCALE 
	{0x03, 0x01, 100,  141}, //CAM_EFFECT_ENC_SEPIA 確
	{0x02, 0x01,0x00, 0x00}, //CAM_EFFECT_ENC_COLORINV
	{0x03, 0x01, 105,  113}, //CAM_EFFECT_ENC_SEPIAGREEN
	{0x03, 0x01, 151,  132}, //CAM_EFFECT_ENC_SEPIABLUE 確屡
};
enum {
	CAM_EFFECT_ENC_NORMAL=0,
	CAM_EFFECT_ENC_GRAYSCALE,
	CAM_EFFECT_ENC_SEPIA,
	CAM_EFFECT_ENC_SEPIAGREEN,
	CAM_EFFECT_ENC_SEPIABLUE,
	CAM_EFFECT_ENC_COLORINV,
	CAM_EFFECT_ENC_GRAYINV,
	CAM_EFFECT_ENC_BLACKBOARD,
	CAM_EFFECT_ENC_WHITEBOARD,
	CAM_EFFECT_ENC_COPPERCARVING,
	CAM_EFFECT_ENC_EMBOSSMENT,
	CAM_EFFECT_ENC_BLUECARVING,
	CAM_EFFECT_ENC_CONTRAST,
	CAM_EFFECT_ENC_JEAN,
	CAM_EFFECT_ENC_SKETCH,
	CAM_EFFECT_ENC_OIL,
	CAM_NO_OF_EFFECT_ENC,
  //add by gugogi
  CAM_EFFECT_ENC_ANTIQUE,
  CAM_EFFECT_ENC_REDRISH,
  CAM_EFFECT_ENC_GREENRISH,
  CAM_EFFECT_ENC_BLUERISH,
  CAM_EFFECT_ENC_BLACKWHITE,
  CAM_EFFECT_ENC_NEGATIVENESS
	
};

BOOL for_serial_sensor = FALSE;

typedef struct
{
  UINT16 HSize;
  UINT16 WSize;
    
} S_SENSOR_FRAME_T;

PRIVATE S_SENSOR_FRAME_T g_sensor_frame;
extern UINT32 gCameraID;

//###
UINT32 dummy_pixels = 0;
typedef enum SensorMirror {
	SENSOR_NO_FLIP = 0x00,
	SENSOR_X_FLIP  = 0x04,
	SENSOR_Y_FLIP  = 0x08,
	SENSOR_XY_FLIP = 0x0c  
} SensorMirror;

UINT16 Hsize = 0;

UINT16 Vsize = 0;
UINT16 ny_value = 0;
UINT16 ne_value = 0;
PRIVATE UINT8 preview_state = FALSE;
PRIVATE UINT8 capture_state = FALSE;
PRIVATE UINT8 preview_first = FALSE;
PRIVATE UINT8 power_on = FALSE;

BOOL PAS6175_LCD_BW =  FALSE;//KAL_TRUE, KAL_FALSE;

BOOL PAS6167_Output_Continuous = TRUE;//KAL_TRUE, KAL_FALSE;
UINT16 lpf_value = 0;
UINT32 exposure_pixels = 0;

const UINT16 PAS6175_DUMMY_PIXELS[]		= {100,1800,1000};/* Preview, Capture, Video */
const UINT8 PAS6175_NP[]				= {8, 12, 4};	/* Preview, Capture, Video */

PRIVATE UINT32 Sysclk = 0;
BOOL PAS6167_Support_220x176 = FALSE; //KAL_TRUE, KAL_FALSE

#ifdef TEMP_C006S_SPICAM_CFG_MACRO
PRIVATE UINT8 PAS6175_Image_Flip[]		= {SENSOR_XY_FLIP};/* SENSOR_NO_FLIP , SENSOR_XY_FLIP, SENSOR_X_FLIP, SENSOR_Y_FLIP */
#else
PRIVATE UINT8 PAS6175_Image_Flip[]		= {SENSOR_NO_FLIP};/* SENSOR_NO_FLIP , SENSOR_XY_FLIP, SENSOR_X_FLIP, SENSOR_Y_FLIP */
#endif

PRIVATE UINT8 Cmd_Cgn_Gb = 0;
PRIVATE UINT8 Cmd_Cgn_B = 0;
PRIVATE UINT8 Cmd_Cgn_Gr = 0;
PRIVATE UINT8 Cmd_Cgn_R = 0;
PRIVATE UINT8 Brightness_Value_LL = 0;
PRIVATE UINT8 Brightness_Value_NL = 0;
PRIVATE volatile UINT8 gIfcChan = 0xff;
PRIVATE BOOL s_spi_interFinished = TRUE;
PRIVATE BOOL s_camrst = FALSE;
PRIVATE UINT16 pas_id = 0xff;
PRIVATE BOOL pas_capture = FALSE;
PRIVATE UINT8 preview_pclk_division = 0;
PRIVATE UINT8 capture_pclk_division = 0;
PRIVATE HAL_SYS_CLOCK_OUT_ID_T g_SerialClockOutId = HAL_SYS_CLOCK_OUT_ID_QTY;
PRIVATE BOOL g_SerialClockOut = FALSE;

const UINT8 PAS6175_OUTPUT_FORMAT[]		= {0,0,0};	/* Preview (RGB565), Capture(YUV422), Capture(RGB565), */

const UINT8 PAS6175_DELAY_FRAME[]		= {2};// PREVIEW_DELAY_FRAME

//##
//CSB
HAL_APO_ID_T g_csb_gpo;/* = {{ 
                          .type = HAL_GPIO_TYPE_O,
                          .id = 5
                        }};*/
//overflow_m
HAL_APO_ID_T g_sensor_ready;/* = {{ 
                            .type = HAL_GPIO_TYPE_IO,
                            .id = 3
                        }};*/
//overflow_s
HAL_APO_ID_T g_sensor_overflow;/* = {{ 
                            .type = HAL_GPIO_TYPE_IO,
                            .id = 2
                        }};*/

HAL_APO_ID_T g_camera_FLASH = {{ 
                          .type = HAL_GPIO_TYPE_O,
                          .id = 0
                        }};


void PAS6175_Output_Enable(BOOL Output_Enable);
void PAS6175_SPI_Slave_Mode_Enable(void);
VOID serial_camer_dma_int(HAL_SPI_IRQ_STATUS_T spi_irq);
VOID test_read_serial_camer_INT();
extern BOOL pas_gpio_i2c_init_sensor(VOID);
void PAS6175_Capture(UINT16 image_target_width, UINT16 image_target_height,UINT8 clk_NP,UINT32 HBlank);
void PAS6175_IO_TriState(BOOL TriState_Enable);
VOID PAS_FrameHeadDetect(VOID);
VOID PAS_Enable_powerdown_voltage( );
VOID PAS_Enable_poweron_voltage( );
UINT32 PAS_Cam_ReadFrame(UINT16 * buff, UINT32 size);
void PAS6175_Preview(UINT16 image_target_width, UINT16 image_target_height,UINT8 clk_NP,UINT32 HBlank);
PRIVATE UINT8 camerap_GetId(VOID);
VOID PAS6175_Clock_Set(UINT8 Sensor_NP);

VOID	Init_Ready_Overflow()
{
    hal_GpioSetIn(g_sensor_ready.gpioId);
    hal_GpioSetIn(g_sensor_overflow.gpioId);

}

VOID lcddp_SensorBB_SPI_SetIO_TriState(BOOL tri)
{
  #ifdef _LCD_CAMERA_USE_SAME_SPI_
  if(tri == FALSE)
  {
    // spi_2
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CS1, HAL_SPI_PIN_STATE_SPI);
    //hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_SPI);
    //hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_DO, HAL_SPI_PIN_STATE_SPI);
  }
  else
  {
    // spi_2
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CS1, HAL_SPI_PIN_STATE_Z);
   // hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_Z);
    //hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_DO, HAL_SPI_PIN_STATE_Z);
  }
  #else
  if(tri == FALSE)
  {
    // spi_2
    hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CS0, HAL_SPI_PIN_STATE_SPI);
    hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_SPI);
    hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_DO, HAL_SPI_PIN_STATE_SPI);
  }
  else
  {
    // spi_2
    hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CS0, HAL_SPI_PIN_STATE_Z);
    hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_Z);
    hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_DO, HAL_SPI_PIN_STATE_Z);
  }

  #endif

}

#ifdef _LCD_CAMERA_USE_SAME_SPI_
VOID close_spi_for_lcd()
{
    hal_SpiDeActivateCs(HAL_SPI,HAL_SPI_CS0);
    hal_SpiClose(HAL_SPI,HAL_SPI_CS0);
   // lcddp_SensorBB_SPI_SetIO_TriState(TRUE);
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CS0, HAL_SPI_PIN_STATE_1);

}
VOID close_spi_for_sensor()
{
    hal_SpiDeActivateCs(HAL_SPI,HAL_SPI_CS1);
    hal_SpiClose(HAL_SPI,HAL_SPI_CS1);
    lcddp_SensorBB_SPI_SetIO_TriState(FALSE);
}

VOID open_spi_for_sensor()
{
    HAL_SPI_CFG_T scamer_spiCfg =
    {
        .enabledCS    = HAL_SPI_CS1,
        .csActiveLow    = TRUE,
        .inputEn        = TRUE,
        .clkFallEdge    = FALSE,
        .clkDelay       = HAL_SPI_HALF_CLK_PERIOD_1,
        .doDelay        = HAL_SPI_HALF_CLK_PERIOD_0,
        .diDelay        = HAL_SPI_HALF_CLK_PERIOD_1,
        .csDelay        = HAL_SPI_HALF_CLK_PERIOD_0,
        .csPulse        = HAL_SPI_HALF_CLK_PERIOD_0,//HAL_SPI_HALF_CLK_PERIOD_1
        .frameSize      = 8,
        .oeRatio        = 0xff,
        .spiFreq        = 6500000,//5M 5000000 // 8M
        .rxTrigger      = HAL_SPI_RX_TRIGGER_4_BYTE,
        .txTrigger      = HAL_SPI_TX_TRIGGER_1_EMPTY,
        .rxMode         = HAL_SPI_DMA_POLLING,
        .txMode         = HAL_SPI_DMA_POLLING,
        .mask           = {0,0,0,0,0},
        .handler        = NULL
    };
    hal_SpiOpen(HAL_SPI,HAL_SPI_CS1, &scamer_spiCfg);
    hal_SpiActivateCs(HAL_SPI,HAL_SPI_CS1);
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CS1, HAL_SPI_PIN_STATE_SPI);
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_SPI);
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_DO, HAL_SPI_PIN_STATE_SPI);

}

VOID open_spi_for_lcd()
{
    HAL_SPI_CFG_T slcd_spiCfg =
    {
        .enabledCS    = HAL_SPI_CS0,
        .csActiveLow    = TRUE,
        .inputEn        = TRUE,
        .clkFallEdge    = TRUE,
        .clkDelay       = HAL_SPI_HALF_CLK_PERIOD_0,
        .doDelay        = HAL_SPI_HALF_CLK_PERIOD_0,
        .diDelay        = HAL_SPI_HALF_CLK_PERIOD_1,
        .csDelay        = HAL_SPI_HALF_CLK_PERIOD_1,
        .csPulse        = HAL_SPI_HALF_CLK_PERIOD_0,//HAL_SPI_HALF_CLK_PERIOD_1
        .frameSize      = 8,
        .oeRatio        = 0,
        .spiFreq        = 20000000,//5M 5000000 15M  20M(33ms-per-frame)  39M(13ms-per-frame)
        .rxTrigger      = HAL_SPI_RX_TRIGGER_4_BYTE,
        .txTrigger      = HAL_SPI_TX_TRIGGER_1_EMPTY,
        .rxMode         = HAL_SPI_DIRECT_POLLING,
        .txMode         = HAL_SPI_DMA_POLLING,
        .mask           = {0,0,0,0,0},
        .handler        = NULL
    };
    hal_SpiOpen(HAL_SPI,HAL_SPI_CS0, &slcd_spiCfg);
    hal_SpiActivateCs(HAL_SPI,HAL_SPI_CS0);
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CS0, HAL_SPI_PIN_STATE_SPI);
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_SPI);
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_DO, HAL_SPI_PIN_STATE_SPI);

}
#endif


VOID pas_close_tri_spi()
{
    #ifdef _LCD_CAMERA_USE_SAME_SPI_
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CS0, HAL_SPI_PIN_STATE_SPI);
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_SPI);
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_DO, HAL_SPI_PIN_STATE_SPI);

    #else
    hal_SpiDeActivateCs(HAL_SPI_2,HAL_SPI_CS0);
    hal_SpiClose(HAL_SPI_2,HAL_SPI_CS0);
    lcddp_SensorBB_SPI_SetIO_TriState(TRUE);

    #endif
}



VOID serial_camer_open_spi(VOID) 
{
    #ifdef _LCD_CAMERA_USE_SAME_SPI_
    #else
    HAL_SPI_CFG_T scamer_spiCfg =
    {
        .enabledCS    = HAL_SPI_CS0,
        .csActiveLow    = TRUE,
        .inputEn        = TRUE,
        .clkFallEdge    = FALSE,
        .clkDelay       = HAL_SPI_HALF_CLK_PERIOD_1,
        .doDelay        = HAL_SPI_HALF_CLK_PERIOD_0,
        .diDelay        = HAL_SPI_HALF_CLK_PERIOD_1,
        .csDelay        = HAL_SPI_HALF_CLK_PERIOD_0,
        .csPulse        = HAL_SPI_HALF_CLK_PERIOD_0,//HAL_SPI_HALF_CLK_PERIOD_1
        .frameSize      = 8,
        .oeRatio        = 0,
        .spiFreq        = 6500000,//5M 5000000 // 8M
        .rxTrigger      = HAL_SPI_RX_TRIGGER_4_BYTE,
        .txTrigger      = HAL_SPI_TX_TRIGGER_1_EMPTY,
        .rxMode         = HAL_SPI_DMA_POLLING,
        .txMode         = HAL_SPI_DMA_POLLING,
        .mask           = {0,0,0,0,0},
        .handler        = NULL
    };
    hal_SpiOpen(HAL_SPI_2,HAL_SPI_CS0, &scamer_spiCfg);
    hal_SpiActivateCs(HAL_SPI_2,HAL_SPI_CS0);
    hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_DO,HAL_SPI_PIN_STATE_Z);

    #endif
}
#if 0
VOID serial_camer_open_spi_dma_int(VOID) 
{
    HAL_SPI_CFG_T scamer_spiCfg =
    {
        .enabledCS    = HAL_SPI_CS0,
        .csActiveLow    = TRUE,
        .inputEn        = TRUE,
        .clkFallEdge    = FALSE,
        .clkDelay       = HAL_SPI_HALF_CLK_PERIOD_1,
        .doDelay        = HAL_SPI_HALF_CLK_PERIOD_0,
        .diDelay        = HAL_SPI_HALF_CLK_PERIOD_1,
        .csDelay        = HAL_SPI_HALF_CLK_PERIOD_0,
        .csPulse        = HAL_SPI_HALF_CLK_PERIOD_0,//HAL_SPI_HALF_CLK_PERIOD_1
        .frameSize      = 8,
        .oeRatio        = 0,
        .spiFreq        = 6500000,//5M 5000000 // 8M
        .rxTrigger      = HAL_SPI_RX_TRIGGER_4_BYTE,
        .txTrigger      = HAL_SPI_TX_TRIGGER_1_EMPTY,
        .rxMode         = HAL_SPI_DMA_IRQ,
        .txMode         = HAL_SPI_DMA_IRQ,
        .mask           = {0,0,1,0,1},
        .handler        = serial_camer_dma_int
    };
    hal_SpiOpen(HAL_SPI_2,HAL_SPI_CS0, &scamer_spiCfg);
    hal_SpiActivateCs(HAL_SPI_2,HAL_SPI_CS0);
    hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_DO,HAL_SPI_PIN_STATE_Z);
}
#endif

PRIVATE BOOL camerap_OpenClk()
{
    HAL_CAMERA_CFG_T CamConfig = {0,};
    CamConfig.rstActiveH = FALSE;
    CamConfig.pdnActiveH = TRUE;
    CamConfig.dropFrame = FALSE;
    CamConfig.camClkDiv = 6; // 156MHz/6 = 26MHz
    CamConfig.endianess = NO_SWAP;//BYTE_SWAP;//
    CamConfig.camId = 0;
    CamConfig.cropEnable = FALSE;
    hal_CameraOpen(&CamConfig);
    //camerap_Delay(500);
    return TRUE;

}
PRIVATE BOOL camerap_CloseClk()
{
    hal_CameraClose();
    return FALSE;
}
PRIVATE VOID camerap_PowerOnForPreview(CAM_SIZE_T CamOutSize, UINT16 IspOutWidth, UINT16 IspOutHeight, CAM_FORMAT_T Format)
{   
 //   hal_HstSendEvent(0x8844009); 
    SXS_TRACE(TSTDOUT,"DRV_CAM: Power On, camOutSize %d ispw %d isph %d", CamOutSize, IspOutWidth, IspOutHeight);
    HAL_CAMERA_CFG_T CamConfig = {0,};
    UINT16 camOutWidth = 0, camOutHeight = 0;

    g_csb_gpo = tgt_GetCamdConfig()->spiCamPinCSB;
    g_sensor_ready = tgt_GetCamdConfig()->spiCamPinOverflowM;
    g_sensor_overflow = tgt_GetCamdConfig()->spiCamPinOverflowS;    
    
    #ifdef TEMP_C006S_SPICAM_CFG_MACRO
    if( g_SerialClockOutId == HAL_SYS_CLOCK_OUT_ID_QTY )
    {
       g_SerialClockOutId = hal_SysClkOutOpen(HAL_SYS_CLOCK_OUT_FREQ_26M);// A15
       if (HAL_SYS_CLOCK_OUT_RESOURCE_UNAVAILABLE == g_SerialClockOutId) hal_DbgAssert("26M clock does not opend");
    }
    #else
    PAS_Enable_poweron_voltage();
    hal_GpioSet(g_csb_gpo);

    
    if(g_SerialClockOut == FALSE)
    {
        g_SerialClockOut = camerap_OpenClk();
    }
    
    #endif
     lcddp_SensorBB_SPI_SetIO_TriState(FALSE);
     serial_camer_open_spi();
     hal_GpioClr(g_csb_gpo);
     camerap_Delay(10);

   if(0)
    { 
      	camerap_WriteOneReg(0xef,0x00);
        s_camrst = TRUE;
        camerap_WriteOneReg(0xee,0x00);
        s_camrst = FALSE;
      	camerap_WriteOneReg(0xef,0x01); 
      	camerap_WriteOneReg(0x22,0x06);
        camerap_Delay(220);
    }
    camerap_SendRgSeq(RG_InitPowerOnQvga,  (sizeof(RG_InitPowerOnQvga) / sizeof(CAM_REG_T)));

  	/* 設定影像成像方向 */
    camerap_WriteOneReg(0xef,0x01);
  	camerap_WriteOneReg(0x21,(camerap_ReadReg(0x21)&0xf3)|PAS6175_Image_Flip[0]);
  	camerap_WriteOneReg(0x11,0x01);

    
  	Cmd_Cgn_Gb = camerap_ReadReg(0x06);
  	Cmd_Cgn_B = camerap_ReadReg(0x07);
  	Cmd_Cgn_Gr = camerap_ReadReg(0x08);
  	Cmd_Cgn_R = camerap_ReadReg(0x09);
  	camerap_WriteOneReg(0xef,0x02);
  	Brightness_Value_LL = camerap_ReadReg(0x69);
  	Brightness_Value_NL = camerap_ReadReg(0x6A);
    camerap_WriteOneReg(0xef,0x00);
  	camerap_WriteOneReg(0xCE,Cmd_Cgn_R -15);//Cmd_Cgn_R
  	camerap_WriteOneReg(0xCF,Cmd_Cgn_Gb -15);//Cmd_Cgn_G
  	camerap_WriteOneReg(0xD0,Cmd_Cgn_B -15);//Cmd_Cgn_B
    camerap_WriteOneReg(0xed,0x01);
    lpf_value = 0;
   if (CamOutSize == CAM_NPIX_QVGA)
    {
        camOutWidth = 240;
        camOutHeight = 320;
    }
    else if (CamOutSize == CAM_NPIX_QQVGA)
    {
        camOutWidth = 128;
        camOutHeight = 160;    
    }
    else
    {
        hal_DbgAssert("don't support the Resolution");
        return;
    }
    SXS_TRACE(TSTDOUT,"DRV_CAM: camerap_PowerOn IspOutWidth  %d IspOutHeight %d",IspOutWidth, IspOutHeight);    
//    hal_HstSendEvent(0x8844002); 

}
PRIVATE VOID camerap_PowerOn(CAM_SIZE_T CamOutSize, UINT16 IspOutWidth, UINT16 IspOutHeight, CAM_FORMAT_T Format)
{

}
PRIVATE BOOL pas_start_preview = FALSE;
 
PRIVATE UINT8 time_mmc = 0;

PRIVATE void camerap_PowerOff(void)
{
    // Turn off the Camera
     hal_GpioSet(g_csb_gpo);
   //  pmd_EnablePower(PMD_POWER_CAMERA,FALSE); 
   //  PAS_FrameStopDetect();
    if(gSensorInfo.cammode==CAM_MODE_IMG_CAPTURE)
    {
        gSensorInfo.cammode = CAM_MODE_IDLE;
    }
    pas_close_tri_spi();
    pas_capture = FALSE;
    pas_start_preview = FALSE;
    for_serial_sensor= FALSE;
    time_mmc = 0;
//    hal_HstSendEvent(0x70017003);
    Lcd_SetIO_TriState(FALSE);
    power_on = FALSE;
   // hal_SpiDeActivateCs(HAL_SPI_2,HAL_SPI_CS0);
   // hal_SpiClose(HAL_SPI_2,HAL_SPI_CS0);
#ifdef TEMP_C006S_SPICAM_CFG_MACRO
    hal_SysClkOutClose(g_SerialClockOutId);
    g_SerialClockOutId = HAL_SYS_CLOCK_OUT_ID_QTY;
#else
    if(g_SerialClockOut == TRUE)
    {
        g_SerialClockOut = camerap_CloseClk();
    }
#endif

}

/**
 * This must be called first before camer_PowerOn()
 */
PRIVATE void camerap_Reserve(CAM_IRQ_HANDLER_T FrameReady)
{
    return ;

    gSensorInfo.camirqhandler = FrameReady;
    hal_CameraIrqSetHandler(camerap_InteruptHandler);
    gIfcChan = 0xff;
}

PRIVATE void camerap_Release(void)
{
    // 'De-initialize' camera
    // Release resource
}


PRIVATE void camerap_SetZoomFactorL(UINT32 ZoomFactor)
{
}

PRIVATE UINT32 camerap_ZoomFactor(void)
{
    return(0);
}

PRIVATE void camerap_SetDigitalZoomFactorL(UINT32 DigZoomFactor)
{
}

PRIVATE UINT32 camerap_DigitalZoomFactor(void)
{
    return(0);
}

PRIVATE void camerap_SetContrastL(UINT32 Contrast)
{         
    
}

PRIVATE UINT32 camerap_Contrast(void)
{
    return(0);
}

PRIVATE void camerap_SetBrightnessL(UINT32 Brightness)
{ 	
  return;
      UINT8 Brightness_Offset = 0;  //pas6175

     switch(Brightness)
    {
        case 1:    //0
             Brightness_Offset = 0;
            break;
        case 2:   //-2
             Brightness_Offset = 255-40;     
            break;
        case 3: //-1
              Brightness_Offset = 255-80;       
              break;
        case 4:  //+1
             Brightness_Offset = 40;
             break;
        case 5:  //+2
           Brightness_Offset = 80;
            break;		 
        default : break;
    }

    	camerap_WriteOneReg(0xef,0x02);
    	camerap_WriteOneReg(0x69,Brightness_Value_LL + Brightness_Offset);
    	camerap_WriteOneReg(0x6A,Brightness_Value_NL + Brightness_Offset);
    	camerap_WriteOneReg(0x00,0x01);

     
 }


PRIVATE UINT32 camerap_Brightness(void)
{
    return(0);
}

// =============================================================================
// camd_SetFlashL
// -----------------------------------------------------------------------------
/// Configure the camera flash light.
///
/// @param flash Flash configuration to apply.
// =============================================================================
PRIVATE void camerap_SetFlashL(CAM_FLASH_T Flash)
{
   return;
    SXS_TRACE(TSTDOUT,"wyk: camera_SetFlashL %x\n",Flash); //wyk reano r1113
    if(gCameraID == 0) return; //Only light on flash led when the back sensor is used.
    switch(Flash)
    {
        case CAM_FLASH_NONE:
            pmd_EnablePower(PMD_POWER_CAMERA_FLASH, FALSE);//FALSE  //wyk reano r1113
            break;

        case CAM_FLASH_FORCED:
            pmd_EnablePower(PMD_POWER_CAMERA_FLASH, TRUE);
            break;
        case CAM_FLASH_AUTO:
        case CAM_FLASH_FILLIN:
        case CAM_FLASH_REDEYEREDUCE:
        default : 
            break;
    }
}

PRIVATE CAM_FLASH_T camerap_Flash(void)
{
    return(CAM_FLASH_NONE);
}

PRIVATE void camerap_SetExposureL(CAM_EXPOSURE_T Exposure)
{

  return;

  //Actually this is the same with camera_SetBrightnessL
    UINT8 Brightness_Offset;  //pas6175
     switch(Exposure)
    {
        case CAM_EXPOSURE_AUTO:     //0
             Brightness_Offset = 0;

            break;
        case CAM_EXPOSURE_NEGATIVE_2:   //-2
             Brightness_Offset = 255-40;     
            break;
        case CAM_EXPOSURE_NEGATIVE_1: //-1
             Brightness_Offset = 255-80;
            break;
        case CAM_EXPOSURE_POSITIVE_1:  //+1
             Brightness_Offset = 40;
            break;
        case CAM_EXPOSURE_POSITIVE_2:  //+2
             Brightness_Offset = 80;
			
            break;		 
        default : break;
    }

    	camerap_WriteOneReg(0xef,0x02);
    	camerap_WriteOneReg(0x69,Brightness_Value_LL + Brightness_Offset);
    	camerap_WriteOneReg(0x6A,Brightness_Value_NL + Brightness_Offset);
    	camerap_WriteOneReg(0x00,0x01);
     
 }

PRIVATE CAM_EXPOSURE_T camerap_Exposure(void)
{
    return(CAM_EXPOSURE_AUTO);
}

PRIVATE void camerap_SetWhiteBalanceL(CAM_WHITEBALANCE_T WhiteBalance)
{
    return ;
   UINT8 temp = 0;
  temp =  camerap_ReadReg(0x80);

    switch(WhiteBalance)
    {
        case CAM_WB_AUTO:  /*Auto */ // pas6175 
    
     		camerap_WriteOneReg(0xef,0x01);
     		camerap_WriteOneReg(0x06,Cmd_Cgn_Gb);//Cmd_Cgn_Gb
     		camerap_WriteOneReg(0x07,Cmd_Cgn_B);//Cmd_Cgn_B
     		camerap_WriteOneReg(0x08,Cmd_Cgn_Gr);//Cmd_Cgn_Gr
     		camerap_WriteOneReg(0x09,Cmd_Cgn_R);//Cmd_Cgn_R
     		camerap_WriteOneReg(0x11,0x01);
     	
     		camerap_WriteOneReg(0xef,0x00);
     		camerap_WriteOneReg(0x72,0x01);       
     		camerap_WriteOneReg(0xed,0x01);
     		camerap_WriteOneReg(0x80,temp |0x78);  // set[11~14] to 1
     		camerap_WriteOneReg(0xed,0x01);
            break;
        case CAM_WB_DAYLIGHT:   /*晴天太阳光*/
    		camerap_WriteOneReg(0xef,0x01);
    		camerap_WriteOneReg(0x06,PAS6175_WB_MODE[1][0]);//Cmd_Cgn_Gb
    		camerap_WriteOneReg(0x07,PAS6175_WB_MODE[1][1]);//Cmd_Cgn_B
    		camerap_WriteOneReg(0x08,PAS6175_WB_MODE[1][2]);//Cmd_Cgn_Gr
    		camerap_WriteOneReg(0x09,PAS6175_WB_MODE[1][3]);//Cmd_Cgn_R
    		camerap_WriteOneReg(0x11,0x01);
            break;
        case CAM_WB_OFFICE:   /*日光灯*/
    		camerap_WriteOneReg(0xef,0x01);
    		camerap_WriteOneReg(0x06,PAS6175_WB_MODE[3][0]);//Cmd_Cgn_Gb
    		camerap_WriteOneReg(0x07,PAS6175_WB_MODE[3][1]);//Cmd_Cgn_B
    		camerap_WriteOneReg(0x08,PAS6175_WB_MODE[3][2]);//Cmd_Cgn_Gr
    		camerap_WriteOneReg(0x09,PAS6175_WB_MODE[3][3]);//Cmd_Cgn_R
    		camerap_WriteOneReg(0x11,0x01);
            break;
        case CAM_WB_CLOUDY:  /*阴天*/ // pas6175
    		camerap_WriteOneReg(0xef,0x01);
    		camerap_WriteOneReg(0x06,PAS6175_WB_MODE[0][0]);//Cmd_Cgn_Gb
    		camerap_WriteOneReg(0x07,PAS6175_WB_MODE[0][1]);//Cmd_Cgn_B
    		camerap_WriteOneReg(0x08,PAS6175_WB_MODE[0][2]);//Cmd_Cgn_Gr
    		camerap_WriteOneReg(0x09,PAS6175_WB_MODE[0][3]);//Cmd_Cgn_R
    		camerap_WriteOneReg(0x11,0x01);

            break;		
        case CAM_WB_TUNGSTEN:  /*钨丝灯*/
    		camerap_WriteOneReg(0xef,0x01);
    		camerap_WriteOneReg(0x06,PAS6175_WB_MODE[4][0]);//Cmd_Cgn_Gb
    		camerap_WriteOneReg(0x07,PAS6175_WB_MODE[4][1]);//Cmd_Cgn_B
    		camerap_WriteOneReg(0x08,PAS6175_WB_MODE[4][2]);//Cmd_Cgn_Gr
    		camerap_WriteOneReg(0x09,PAS6175_WB_MODE[4][3]);//Cmd_Cgn_R
    		camerap_WriteOneReg(0x11,0x01);
            break;
        default : break;
    }
 }

PRIVATE CAM_WHITEBALANCE_T camerap_WhiteBalance(void)
{
    return(CAM_WB_AUTO);
}

PRIVATE VOID camerap_SetSpecialEffect(CAM_SPECIALEFFECT_T SpecialEffect)
{
  //return;
    // UINT8 TempReg;
   // TempReg = camera_ReadReg(0x47);
    switch(SpecialEffect)
    {

        case CAM_SPE_NORMAL	:
             camerap_WriteOneReg(0xef,0x00);
             camerap_WriteOneReg(0x94,0x00);//R_ISP_ImgEffect_En
             camerap_WriteOneReg(0xed,0x01);

            break;
        case CAM_SPE_ANTIQUE	:	
             camerap_WriteOneReg(0xef,0x00);
             camerap_WriteOneReg(0x8f,PAS6175_Effect[1][2]);//R_ImgEffect_c0
             camerap_WriteOneReg(0x90,PAS6175_Effect[1][3]);//R_ImgEffect_c1
             camerap_WriteOneReg(0x93,PAS6175_Effect[1][0]);//R_ImgEffectMode
             camerap_WriteOneReg(0x94,PAS6175_Effect[1][1]);//R_ISP_ImgEffect_En
             camerap_WriteOneReg(0xed,0x01);
            break;
        case CAM_SPE_REDISH	:
            break;
        case CAM_SPE_GREENISH:	
             camerap_WriteOneReg(0xef,0x00);
             camerap_WriteOneReg(0x8f,PAS6175_Effect[3][2]);//R_ImgEffect_c0
             camerap_WriteOneReg(0x90,PAS6175_Effect[3][3]);//R_ImgEffect_c1
             camerap_WriteOneReg(0x93,PAS6175_Effect[3][0]);//R_ImgEffectMode
             camerap_WriteOneReg(0x94,PAS6175_Effect[3][1]);//R_ISP_ImgEffect_En
             camerap_WriteOneReg(0xed,0x01);
            break;
        case CAM_SPE_BLUEISH	:	
             camerap_WriteOneReg(0xef,0x00);
             camerap_WriteOneReg(0x8f,PAS6175_Effect[4][2]);//R_ImgEffect_c0
             camerap_WriteOneReg(0x90,PAS6175_Effect[4][3]);//R_ImgEffect_c1
             camerap_WriteOneReg(0x93,PAS6175_Effect[4][0]);//R_ImgEffectMode
             camerap_WriteOneReg(0x94,PAS6175_Effect[4][1]);//R_ISP_ImgEffect_En
             camerap_WriteOneReg(0xed,0x01);
            break;
        case CAM_SPE_BLACKWHITE	:
            break;
        case CAM_SPE_NEGATIVE:	
             camerap_WriteOneReg(0xef,0x00);
             camerap_WriteOneReg(0x8f,PAS6175_Effect[3][2]);//R_ImgEffect_c0
             camerap_WriteOneReg(0x90,PAS6175_Effect[3][3]);//R_ImgEffect_c1
             camerap_WriteOneReg(0x93,PAS6175_Effect[3][0]);//R_ImgEffectMode
             camerap_WriteOneReg(0x94,PAS6175_Effect[3][1]);//R_ISP_ImgEffect_En
             camerap_WriteOneReg(0xed,0x01);
            break;
        case CAM_SPE_BLACKWHITE_NEGATIVE:
            break;

        default: break;

    }
 }


PRIVATE CAM_SPECIALEFFECT_T camerap_SpecialEffect(VOID)
{
    return(CAM_SPE_NORMAL);

}
// NightMode : 1: NightModeEN; 0:NightModeDis
PRIVATE VOID camerap_SetNightMode(UINT32 NightMode)
{
  return;

  	UINT8 Index = 0;
  
  	if (NightMode == TRUE)
  		Index = 1;
  	else
  		Index = 0;

    camerap_WriteOneReg(0xef,0x00);
    camerap_WriteOneReg(0x6C,PAS6175_AeStage_GainStage[Index][0]);//R_AE_maxStage[4:0]
    camerap_WriteOneReg(0x6D,PAS6175_AeStage_GainStage[Index][1]);//R_AG_stage_UB
    camerap_WriteOneReg(0xed,0x01);	
    
    camerap_WriteOneReg(0xef,0x02);
    camerap_WriteOneReg(0x2F,PAS6175_AeStage_GainStage[Index][2]);//R_AE_stage_LL[4:0]
    camerap_WriteOneReg(0x30,PAS6175_AeStage_GainStage[Index][3]);//R_AE_stage_NL[4:0]
    camerap_WriteOneReg(0x36,PAS6175_AeStage_GainStage[Index][4]);//R_Gamma_Strength_Delta[3:0]
    //    write_cmos_sensor(0x5F,PAS6175_AeStage_GainStage[Index][5]);//R_Saturation_LL[4:0]
    camerap_WriteOneReg(0x00,0x01);
 }


PRIVATE UINT32 camerap_NightMode(VOID)
{
    return(FALSE);

}
// =============================================================================
// camera_StartViewFinderDirectL(some parameters)
// -----------------------------------------------------------------------------
/// Configure the camera to capture frames for viewfinder.  This should be
/// not the maximum resolution of the camera, but a resolution as close to
/// the LCD as possible.  The parameters for the screen should be passed here
/// and this function should automatically 'calculate' the best size based on
/// the possible output formatter configurations.
/// 
/// @param Some stuff to be defined
// =============================================================================

PRIVATE void camerap_StartViewFinderDirectL()
{
    // Not really direct, but sets the camera into viewfinder mode (lower resolution to fit the screen)
    // Needs
}

PRIVATE void camerap_StopViewFinder(void)
{
}

PRIVATE BOOL camerap_ViewFinderActive(void)
{
    return(FALSE);
}

PRIVATE void camerap_SetViewFinderMirror(BOOL Mirror)
{
}

PRIVATE BOOL camerap_ViewFinderMirror(void)
{
    return(FALSE);
}
extern VOID pas_send_message();

PRIVATE void camerap_PrepareImageCaptureL(CAM_FORMAT_T ImageFormat, UINT32 SizeIndex, CAM_IMAGE_T Buffer)
{

}

void camerap_StartCapture(CAM_SIZE_T CamOutSize, UINT16 Width, UINT16 Height)
{
    if (CamOutSize == CAM_NPIX_QVGA)
    {
    }
    else if (CamOutSize == CAM_NPIX_QQVGA)
    {
    }
    else
    {
        hal_DbgAssert("don't support the Resolution");
        return;
    }
    g_sensor_frame.HSize = Height;
    g_sensor_frame.WSize = Width;
   	if((Width == 128)&&(Height == 160))
   	{	
        PAS6175_Capture(Width,Height,16,150);
    }
    else if((Width == 240)&&(Height == 320))

    {
        PAS6175_Capture(Width,Height,32,800);
    }
    for_serial_sensor = TRUE;
}
void camerap_StartPreview(CAM_SIZE_T CamOutSize, UINT16 Width, UINT16 Height)
{

    if (CamOutSize == CAM_NPIX_QVGA)
    {
    }
    else if (CamOutSize == CAM_NPIX_QQVGA)
    {
    }
    else
    {
        hal_DbgAssert("don't support the Resolution");
        return;
    }

   	if((Width == 128)&&(Height == 160))
   	{	 
    //   hal_HstSendEvent(0x123);
       PAS6175_Preview(Width,Height,8,60);
    }
    else if((Width == 240)&&(Height == 320))
    {
       hal_DbgAssert("don't support the Resolution");
       PAS6175_Preview(Width,Height,32,800);
    }
    for_serial_sensor = TRUE;
}
UINT32 camerap_GetOneFrame(UINT16 * buff,UINT32 size)
{

   // PAS_Cam_ReadFrame(buff, g_sensor_frame.WSize*g_sensor_frame.HSize);
   return PAS_Cam_ReadFrame(buff, 240*320);
}

PRIVATE void camerap_CaptureImage()
{

}

PRIVATE void camerap_CancelCaptureImage(void)
{
}

PRIVATE void camerap_EnumerateCaptureSizes(CAM_SIZE_T *Size,UINT32 SizeIndex, CAM_FORMAT_T Format)
{
}

PRIVATE void camerap_PrepareVideoCaptureL(CAM_FORMAT_T Format, UINT32 SizeIndex, UINT32 RateIndex, UINT32 BuffersToUse, UINT32 FramesPerBuffer)
{
}

PRIVATE void camerap_StartVideoCapture(void)
{
}

PRIVATE void camerap_StopVideoCapture(void)
{
}

PRIVATE BOOL camerap_VideoCaptureActive(void)
{
    return(gSensorInfo.capturevideo);
}

PRIVATE void camerap_EnumerateVideoFrameSizes(
        CAM_SIZE_T Size,
        UINT32 SizeIndex,
        CAM_FORMAT_T Format)
{
}

PRIVATE void camerap_EnumerateVideoFrameRates()
{
}

PRIVATE void camerap_GetFrameSize(CAM_SIZE_T Size)
{
}

PRIVATE VOID camerap_SetFrameRate(UINT32 FrameRate)
{
}

PRIVATE UINT32 camerap_FrameRate(void)
{
    return(0);
}

PRIVATE UINT32 camerap_BuffersInUse(void)
{
    return(0);
}

PRIVATE UINT32 camerap_FramesPerBuffer(void)
{
    return(0);
}
PRIVATE UINT8 camerap_GetId(VOID)
{
//   hal_HstSendEvent(0x811944);hal_HstSendEvent(pas_id);
   if(pas_id == 0xff)
   {

    CAM_REG_T id_reg={0xef,0,1};
    UINT16 id = 0xff,i=0;
    hal_TimDelay(10); 
    camerap_WriteReg(id_reg);
    for(i=0; i<2; i++)
    {
        id = (camerap_ReadReg(0x00)<<8)|camerap_ReadReg(0x01);
        if(id == PAS6175_SENSOR_ID)
        {
            break;
        }
    }
    hal_HstSendEvent(0x322002);  
    hal_HstSendEvent((UINT32)id);  
    pas_id = id;
   }
    return CAM_PAS6175_ID ;
 
}

PRIVATE VOID camerap_Delay(UINT16 Wait_mS)
{
    UINT32 Ticks_16K;

    // Here is the delay function used by the camera driver.  It can be changed to use a non-blocking
    // wait if necessary, but for now, just use hal_TimDelay
    Ticks_16K = (16348 * Wait_mS)/1000;
    hal_TimDelay(Ticks_16K);
}

PRIVATE VOID CAMD_POSSIBLY_UNUSED camerap_DbgSendSequence(UINT8 SeqNum)
{
  return;
    UINT16 RegCount;
    UINT16 Len;
    CAM_REG_T *RegVal;

    switch (SeqNum)
    {
        case (1) :
            Len = sizeof(RG_InitPowerOnVga) / sizeof(CAM_REG_T);
            RegVal =  (CAM_REG_T*)&RG_InitPowerOnVga;
            break;
        case (2) :
            Len = sizeof(RG_VidFormatVga) / sizeof(CAM_REG_T);
            RegVal =  (CAM_REG_T*)&RG_VidFormatVga;
            break;
        case (3) :
            Len = sizeof(RG_VidFormatQvga) / sizeof(CAM_REG_T);
            RegVal =  (CAM_REG_T*)&RG_VidFormatQvga;
            break;
        case (4) :
            Len = sizeof(RG_VidFormatQqvga) / sizeof(CAM_REG_T);
            RegVal =  (CAM_REG_T*)&RG_VidFormatQqvga;
            break;
        default :
            Len = 0;
            RegVal = NULL;
            break;
    }
    for (RegCount = 0; RegCount < Len; ++RegCount)
    {
        camerap_WriteReg(RegVal[RegCount]);
    }
}

extern BOOL pas_gpio_i2c_write_data_sensor(UINT32 addr,UINT8 * regaddr,UINT8 addrlen,UINT8 *data, UINT8 datalen);
extern UINT8 pas_gpio_i2c_read_data_sensor(UINT32 addr,UINT8 * regaddr, UINT8 addrlen,UINT8 *datas, UINT8 datalen);


PRIVATE VOID camerap_SendRgSeq(const CAM_REG_T *RegList, UINT16 Len)
{
    UINT16 RegCount;

    for (RegCount = 0; RegCount < Len; ++RegCount)
    {
        camerap_WriteReg(RegList[RegCount]);

    }
}

PRIVATE VOID camerap_WriteReg(CAM_REG_T RegWrite)
{
   if(  pas_gpio_i2c_write_data_sensor(0x40,(UINT8 *)&RegWrite.Addr,1,&RegWrite.Data,1) == FALSE)
   {
        if(  pas_gpio_i2c_write_data_sensor(0x40,(UINT8 *)&RegWrite.Addr,1,&RegWrite.Data,1) == FALSE)
        {
             if(  pas_gpio_i2c_write_data_sensor(0x40,(UINT8 *)&RegWrite.Addr,1,&RegWrite.Data,1) == FALSE) 
             {
                //hal_DbgAssert("no ack");
             }
        }
   }

}

PRIVATE VOID  camerap_WriteOneReg( UINT8 Addr, UINT8 Data)
{
   if( pas_gpio_i2c_write_data_sensor(0x40,&Addr,1,&Data,1) == FALSE)
   {
       if( pas_gpio_i2c_write_data_sensor(0x40,&Addr,1,&Data,1) == FALSE)
       {
             if( pas_gpio_i2c_write_data_sensor(0x40,&Addr,1,&Data,1) == FALSE) 
              {
                if(s_camrst == FALSE) 
                {
                    //hal_DbgAssert("no ack");
                }
             }

       }

   }

}


PRIVATE UINT8 camerap_ReadReg(UINT8 Addr)
{
    UINT8 data = 0;
    pas_gpio_i2c_read_data_sensor(0x40,&Addr,1,&data,1);
    return data;

}


PRIVATE VOID camerap_InteruptHandler(HAL_CAMERA_IRQ_CAUSE_T cause)
{
  #if 0
    HAL_CAMERA_IRQ_CAUSE_T mask = {0,0,0,0};

//    SXS_TRACE(TSTDOUT,"DRV_CAM: IRQ");
    switch (gSensorInfo.cammode)
    {
        case CAM_MODE_PREP_IMG_CAPTURE:
            // Configure IFC        
            gIfcChan = hal_CameraStartXfer((gSensorInfo.npixels*2), (UINT8*)gSensorInfo.vidbuffer);
            // Reset interrupt
            mask.vsync= 1;
            hal_CameraIrqSetMask(mask);
            gSensorInfo.cammode = CAM_MODE_IMG_CAPTURE;
            break;
        case CAM_MODE_IMG_CAPTURE:
            // TODO: When capture is done, should go back to viewfinder mode, or just shut down the camera?
            //SXS_TRACE(TSTDOUT,"DRV_CAM: End of Image Capture");
            hal_CameraIrqSetMask(mask);
            gSensorInfo.cammode = CAM_MODE_IDLE;
            if(hal_CameraStopXfer(FALSE)==XFER_SUCCESS)
            {
                //hal_CameraReset(TRUE, TRUE);
                gSensorInfo.camirqhandler(0, gSensorInfo.currentbuf);
            }
            else
            {
                gSensorInfo.camirqhandler(1, gSensorInfo.currentbuf);
            }

            break;
        default:
            gSensorInfo.camirqhandler(255, 255);
            break;
    }

   #endif
}

VOID camerap_PAS6175_Pause()
{
    PAS6175_Pause();
}
VOID camerap_PAS6175_Resume()
{
    PAS6175_Resume();
}

VOID PAS6175_Pause(VOID)
{
    PAS6175_IO_TriState(TRUE);
    lcddp_SensorBB_SPI_SetIO_TriState(TRUE);
    hal_GpioSet(g_csb_gpo);
}
VOID PAS6175_Resume(VOID)
{
    hal_GpioClr(g_csb_gpo);
    PAS6175_IO_TriState(FALSE);
    lcddp_SensorBB_SPI_SetIO_TriState(FALSE);
    
}

//交换高低8位的128*160的buffer至少需要5ms
BOOL pas_pown_on = FALSE;
UINT8 pas_count = 0;

VOID PAS_Enable_power_voltage(BOOL open);
PRIVATE BOOL camerap_CheckProductId()
{

//   hal_HstSendEvent(0x933911); 

    g_csb_gpo = tgt_GetCamdConfig()->spiCamPinCSB;
    g_sensor_ready = tgt_GetCamdConfig()->spiCamPinOverflowM;
    g_sensor_overflow = tgt_GetCamdConfig()->spiCamPinOverflowS;
   // pas_CheckProductId();
   // pas_PowerOn(CAM_NPIX_QVGA, 128, 160, CAM_FORMAT_RGB565);
   // camerap_PowerOff();
   // pas_CheckProductId();
   // pas_PowerOn(CAM_NPIX_QVGA, 128, 160, CAM_FORMAT_RGB565);
   // PAS_Enable_power_voltage(FALSE);
    if(pas_id == 0xff)
    {        
        PAS_Enable_powerdown_voltage();
        camerap_Delay(100);
        PAS_Enable_poweron_voltage();
        pas_gpio_i2c_init_sensor();

   #ifdef  TEMP_C006S_SPICAM_CFG_MACRO
        g_SerialClockOutId = hal_SysClkOutOpen(HAL_SYS_CLOCK_OUT_FREQ_26M);// A15
        if (HAL_SYS_CLOCK_OUT_RESOURCE_UNAVAILABLE == g_SerialClockOutId) hal_DbgAssert("26M clock does not opend");
   #else
        if(g_SerialClockOut == FALSE)
        {
            g_SerialClockOut = camerap_OpenClk();            
        }
   #endif

        camerap_Delay(10);

        #if 0
        PAS_Enable_power_voltage(FALSE);
        camerap_Delay(300);
        PAS_Enable_power_voltage(TRUE);
        #endif
        camerap_GetId();
 //       hal_HstSendEvent(0x811922);
       if(0)
        { 
          	camerap_WriteOneReg(0xef,0x00);
            s_camrst = TRUE;
            camerap_WriteOneReg(0xee,0x00);
            s_camrst = FALSE;
          	camerap_WriteOneReg(0xef,0x01); 
          	camerap_WriteOneReg(0x22,0x06);
            camerap_Delay(220);
        }
       PAS6175_Output_Enable(FALSE);
       PAS6175_Pause();
       camerap_Delay(2);

 #ifdef TEMP_C006S_SPICAM_CFG_MACRO
       hal_SysClkOutClose(g_SerialClockOutId);
       g_SerialClockOutId = HAL_SYS_CLOCK_OUT_ID_QTY;
 #else
       if(g_SerialClockOut == TRUE)
        {
            g_SerialClockOut = camerap_CloseClk();       
        }
      // pmd_EnablePower(PMD_POWER_CAMERA,FALSE); 
      //  PAS_Enable_powerdown_voltage_tri();
        //PAS_Enable_powerdown_voltage_tri();
//        hal_HstSendEvent(0x811988);
 #endif

    }
    SXS_TRACE(TSTDOUT,"pas6175(0x%02x): read id is 0x%04x", CAM_PAS6175_ID, pas_id);

    if(pas_id == PAS6175_SENSOR_ID)
        return TRUE;
    else
        return FALSE;

}



VOID PAS6175_Output_Format(UINT16 OutputFormat)
{
	camerap_WriteOneReg(0xef,0x02);	
	camerap_WriteOneReg(0xC0,OutputFormat);
	camerap_WriteOneReg(0x00,0x01);		
}

VOID PAS6175_Clock_Set(UINT8 Sensor_NP)
{
	UINT32 R_SysClk_freq = 0;
	camerap_WriteOneReg(0xef,0x01);				   
	camerap_WriteOneReg(0x02,Sensor_NP); 
	camerap_WriteOneReg(0x11,0x01);	

	Sysclk =26000000/Sensor_NP;

	R_SysClk_freq = Sysclk>>8;
	camerap_WriteOneReg(0xef,0x00);		   
	camerap_WriteOneReg(0x67,R_SysClk_freq&0x00ff);
	camerap_WriteOneReg(0x68,0x80|((R_SysClk_freq&0xff00)>>8));	
	camerap_WriteOneReg(0xed,0x01);	
}

VOID PAS6175_Set_Image_Size(UINT16 HSize, UINT16 VSize)
{ 
	UINT8 Cmd_HStart = 0;
	UINT8 Cmd_VStart = 0;
	UINT16 Cmd_HSize = 255;
	UINT16 Cmd_VSize = 335; 
	
	UINT16 R_ISP_HSize = 242;
	UINT16 R_ISP_VSize = 320;
	
	UINT8 R_AE_Window_X_By4 = 58;
	UINT8 R_AE_Window_Y_By4 = 78;
	UINT8 R_AWB_Window_X_By4 = 58;
	UINT8 R_AWB_Window_Y_By4 = 60;

	UINT8 R_ScalingFIFO_En = 1;
	UINT8 R_ScalingFIFO_out_NP = 4; //For Master Mode,

	UINT16 R_Ptr_ScalingFIFO = 0;
	
	UINT16 R_FIFO_Hsync = 24;
	UINT16 R_FIFO_Hsize = HSize;
	
	UINT16 R_ISP_WOIa_HSize = 242;
	UINT16 R_ISP_WOIa_VSize = 320;
	UINT16 R_ISP_WOIa_HOffset = 0;
	UINT16 R_ISP_WOIa_VOffset = 0;
	
	UINT16 R_ISP_WOIb_HSize = HSize;
	UINT16 R_ISP_WOIb_VSize = VSize;
	UINT16 R_ISP_WOIb_HOffset = 0;
	UINT16 R_ISP_WOIb_VOffset = 0;
	
	UINT8 Scaling = 0;
	UINT8 R_EncDecimationNo = 0;

   UINT8 R_SPI_FIFO_Ptr = 0x1e;


	if((HSize == 176)&&(VSize == 220))
	{	
        R_Ptr_ScalingFIFO = 144;
        R_ISP_WOIb_VOffset = 6;
        Scaling = 22;    
	}
	else if((HSize == 176)&&(VSize == 176))
	{	
		R_Ptr_ScalingFIFO = 144;
		R_ISP_WOIb_VOffset = 6;
		Scaling = 22;	 
	}
	else if((HSize == 176)&&(VSize == 144))
	{	
        R_Ptr_ScalingFIFO = 144;
        R_ISP_WOIb_VOffset = 50;
        Scaling = 22;    
	}	
	else if((HSize == 176)&&(VSize == 88))
	{	
		//R_ScalingFIFO_out_NP=4;
		R_Ptr_ScalingFIFO=144;
		R_FIFO_Hsync=24;
		R_FIFO_Hsize=176;
		
		R_ISP_WOIa_HSize=242;
		R_ISP_WOIa_VSize=320;
		R_ISP_WOIa_HOffset=0;
		R_ISP_WOIa_VOffset=0;
		
		R_ISP_WOIb_HSize = 176;
		R_ISP_WOIb_VSize = 88;
		R_ISP_WOIb_HOffset = 0;
		R_ISP_WOIb_VOffset=50;
		Scaling=22;    
	}	
	else if((HSize == 160)&&(VSize == 128))
	{	
        R_Ptr_ScalingFIFO = 144;
        R_ISP_WOIb_VOffset = 42;
        Scaling = 24;    
	}	
	else if((HSize == 160)&&(VSize == 120))
	{	
        R_Ptr_ScalingFIFO = 144;
        R_ISP_WOIb_VOffset = 42;
        Scaling = 24;    
	}		
	else if((HSize == 152)&&(VSize == 124))
	{	
        R_Ptr_ScalingFIFO = 144;
        R_ISP_WOIb_VOffset = 43;
        Scaling = 24;    
	}	
	else if((HSize == 144)&&(VSize == 176))
	{
        R_Ptr_ScalingFIFO = 100;
        R_ISP_WOIb_VOffset = 10;
        Scaling = 26;    		
	}		
	else if ((HSize == 132)&&(VSize == 162))
	{	
        R_Ptr_ScalingFIFO = 100;
        Scaling = 28;    			
	}	
	else if((HSize == 128)&&(VSize == 160))
	{	
        R_Ptr_ScalingFIFO = 100;
        R_ISP_WOIb_VOffset = 5;
        Scaling = 30;    	
	}		
	
	else if((HSize == 128)&&(VSize == 128))
	{	
        R_Ptr_ScalingFIFO = 100;
        R_ISP_WOIb_VOffset = 21;
        Scaling = 30;    	
	}	
	else if ((HSize == 128)&&(VSize == 104))
	{	
        R_Ptr_ScalingFIFO = 100;
        R_ISP_WOIb_VOffset = 33;
        Scaling = 30;  	
	}	
	else if ((HSize == 128)&&(VSize == 96))
	{	
        R_Ptr_ScalingFIFO = 100;
        R_ISP_WOIb_VOffset = 37;
        Scaling = 30;  			
	}		
	else if((HSize == 124)&&(VSize == 152))
	{	
        R_Ptr_ScalingFIFO = 100;
        R_ISP_WOIb_VOffset = 6;
        Scaling = 31;    				
	}	

	else if ((HSize == 112)&&(VSize == 96))
	{	
        R_Ptr_ScalingFIFO = 100;
        R_ISP_WOIb_VOffset = 27;
		R_EncDecimationNo = 0x11;
        Scaling = 17;//34;  					
	}		
	else if ((HSize == 104)&&(VSize == 128))
	{	
		R_Ptr_ScalingFIFO = 90;
		R_ISP_WOIb_VOffset = 5;
		Scaling = 37;							
	}		
	else if ((HSize == 96)&&(VSize == 128))
	{	
		R_Ptr_ScalingFIFO = 80;
		Scaling = 40;							
	}		
	else if ((HSize == 96)&&(VSize == 112))
	{	
		R_Ptr_ScalingFIFO = 80;
		R_ISP_WOIb_VOffset = 8;
		Scaling = 40;							
	}		
	else if ((HSize == 88)&&(VSize == 72))
	{	
        R_Ptr_ScalingFIFO = 80;
        R_ISP_WOIb_VOffset = 22;		
		R_EncDecimationNo = 0x11;
        Scaling = 22;  							
	}	
	else if ((HSize == 80)&&(VSize == 64))
	{	
        R_Ptr_ScalingFIFO = 70;
        R_ISP_WOIb_VOffset = 21;
        Scaling = 48;  									
	}				
	else if ((HSize == 72)&&(VSize == 88))
	{	
		R_Ptr_ScalingFIFO = 60;
		R_ISP_WOIb_VOffset = 8;
		Scaling = 53;							
	}	
	else if ((HSize == 64)&&(VSize == 80))
	{	
        R_Ptr_ScalingFIFO = 50;
        R_ISP_WOIb_VOffset = 3;
        Scaling = 60;  									
	}	
	else if ((HSize == 64)&&(VSize == 52))
	{	
        R_Ptr_ScalingFIFO = 50;
        R_ISP_WOIb_VOffset = 3;
        Scaling = 60;  									
	}					
	else if ((HSize == 52)&&(VSize == 64))
	{	
		R_Ptr_ScalingFIFO = 40;
		R_ISP_WOIb_HOffset = 4;
		R_ISP_WOIb_VOffset = 8;
		Scaling = 63;							
	}
	else if ((HSize == 240)&&(VSize == 320))
	{	 
      R_ScalingFIFO_En = 1;
      R_ScalingFIFO_out_NP = 2;
      R_Ptr_ScalingFIFO = 0;
      
      R_FIFO_Hsync = 24;
      R_FIFO_Hsize = HSize;
      
      R_ISP_WOIa_HSize = 242;
      R_ISP_WOIa_VSize = 320;
      R_ISP_WOIa_HOffset = 0;
      R_ISP_WOIa_VOffset = 0;
      
      R_ISP_WOIb_HSize = HSize;
      R_ISP_WOIb_VSize = VSize;
      R_ISP_WOIb_HOffset = 0;
      R_ISP_WOIb_VOffset = 0;
      Scaling = 16;
      R_EncDecimationNo = 0;
      R_SPI_FIFO_Ptr = 0x1e;
	}		
	else
	{
    hal_DbgAssert(" error Resolution");
  };
  
  
  // Set LPF & Min Lpf

	Hsize=Cmd_HSize+1;
	Vsize=Cmd_VSize+16; /* 將Dark Line也包含進來，不加進來可能會造成AE不動作。*/

	camerap_WriteOneReg(0xef,0x00);
	camerap_WriteOneReg(0x8c,0x00);
	camerap_WriteOneReg(0xE0,R_ISP_HSize&0x00ff); 	/* R_ISP_HSize[7:0] */
	camerap_WriteOneReg(0xE1,(R_ISP_HSize&0x0300)>>8);		/* R_ISP_HSize[9:8] */
	camerap_WriteOneReg(0xE2,R_ISP_VSize&0x00ff); 	/* R_ISP_VSize[7:0] */
	camerap_WriteOneReg(0xE3,(R_ISP_VSize&0x0300)>>8);		/* R_ISP_HSize[7:0] */
	camerap_WriteOneReg(0x15,R_AE_Window_X_By4);		/* R_AE_Window_X[7:0] */
	camerap_WriteOneReg(0x17,R_AE_Window_Y_By4);		/* R_AE_Window_Y[7:0] */
	camerap_WriteOneReg(0x0f,R_AWB_Window_X_By4); 	/* R_AWB_Window_X[7:0] */
	camerap_WriteOneReg(0x11,R_AWB_Window_Y_By4); 	/* R_AWB_Window_Y[7:0] */		
	camerap_WriteOneReg(0xed,0x01);

	camerap_WriteOneReg(0xef,0x01);
	camerap_WriteOneReg(0x17,Cmd_HStart); 		/* Cmd_HStart[7:0] */
	camerap_WriteOneReg(0x19,Cmd_VStart); 		/* Cmd_VStart[7:0] */		
	camerap_WriteOneReg(0x1b,Cmd_HSize&0x00FF);			/* Cmd_HSize[7:0] */
	camerap_WriteOneReg(0x1c,(Cmd_HSize&0x0100)>>8);			/* Cmd_VSize[7:0] */
	camerap_WriteOneReg(0x1d,Cmd_VSize&0x00FF);			/* Cmd_HSize[7:0] */
	camerap_WriteOneReg(0x1e,(Cmd_VSize&0x0100)>>8);			/* Cmd_VSize[7:0] */
	camerap_WriteOneReg(0x11,0x01);

	camerap_WriteOneReg(0xef,0x02);
	camerap_WriteOneReg(0xA3,R_ScalingFIFO_En);	/* R_ScalingFIFO_En */
	camerap_WriteOneReg(0xA4,R_ScalingFIFO_out_NP);	/* R_ScalingFIFO_En */
	camerap_WriteOneReg(0xA5,(R_Ptr_ScalingFIFO&0x0100)>>8);	/* R_Ptr_ScalingFIFO[8] */
	camerap_WriteOneReg(0xA6,R_Ptr_ScalingFIFO&0x00FF);	/* R_Ptr_ScalingFIFO[7:0] */
	camerap_WriteOneReg(0xA8,(R_FIFO_Hsync&0x0100)>>8);		/* R_FIFO_Hsync[8] */ 
	camerap_WriteOneReg(0xA9,R_FIFO_Hsync&0x00FF);		/* R_FIFO_Hsync[7:0] */ 
	camerap_WriteOneReg(0xAA,(R_FIFO_Hsize&0x0100)>>8);		/* R_FIFO_Hsize[8] */ 
	camerap_WriteOneReg(0xAB,R_FIFO_Hsize&0x00FF);		/* R_FIFO_Hsize[7:0] */ 
	camerap_WriteOneReg(0x9b,(R_ISP_WOIa_HSize&0x0100)>>8);	/* R_ISP_WOIa_HSize[8] */
	camerap_WriteOneReg(0x9c,R_ISP_WOIa_HSize&0x00FF);	/* R_ISP_WOIa_HSize[7:0] */
	camerap_WriteOneReg(0x9d,(R_ISP_WOIa_VSize&0x0100)>>8);	/* R_ISP_WOIa_VSize[8] */
	camerap_WriteOneReg(0x9e,R_ISP_WOIa_VSize&0x00FF);	/* R_ISP_WOIa_VSize[7:0] */
	camerap_WriteOneReg(0x9f,(R_ISP_WOIa_HOffset&0x0100)>>8);	/* R_ISP_WOIb_HOffset[8] */ 
	camerap_WriteOneReg(0xa0,R_ISP_WOIa_HOffset&0x00FF);	/* R_ISP_WOIb_HOffset[7:0] */ 
	camerap_WriteOneReg(0xa1,(R_ISP_WOIa_VOffset&0x0100)>>8);	/* R_ISP_WOIb_VOffset[8] */ 	
	camerap_WriteOneReg(0xa2,R_ISP_WOIa_VOffset&0x00FF);	/* R_ISP_WOIb_VOffset[7:0] */ 	
	camerap_WriteOneReg(0xb4,(R_ISP_WOIb_HSize&0x0300)>>8);	/* R_ISP_WOIb_HSize[9:8] */ 
	camerap_WriteOneReg(0xb5,R_ISP_WOIb_HSize&0x00FF);	/* R_ISP_WOIb_HSize[7:0] */ 
	camerap_WriteOneReg(0xb6,(R_ISP_WOIb_VSize&0x0300)>>8);	/* R_ISP_WOIb_VSize[9:8] */ 
	camerap_WriteOneReg(0xb7,R_ISP_WOIb_VSize&0x00FF);	/* R_ISP_WOIb_VSize[7:0] */ 
	camerap_WriteOneReg(0xb8,(R_ISP_WOIb_HOffset&0x0100)>>8);	/* R_ISP_WOIb_HOffset[8] */ 
	camerap_WriteOneReg(0xb9,R_ISP_WOIb_HOffset&0x00FF);	/* R_ISP_WOIb_HOffset[7:0] */ 
	camerap_WriteOneReg(0xba,(R_ISP_WOIb_VOffset&0x0100)>>8);	/* R_ISP_WOIb_VOffset[8] */ 	
	camerap_WriteOneReg(0xbb,R_ISP_WOIb_VOffset&0x00FF);	/* R_ISP_WOIb_VOffset[7:0] */ 	
	camerap_WriteOneReg(0xb0,0x80|Scaling);		/* Scaling */
	camerap_WriteOneReg(0xb1,0x80|Scaling);		/* Scaling */ 	
	camerap_WriteOneReg(0xb2,R_EncDecimationNo);		/*R_EncDecimationNo_Y[3:0],R_EncDecimationNo_X[3:0] */ 		
	camerap_WriteOneReg(0xC5,R_SPI_FIFO_Ptr);
	camerap_WriteOneReg(0x00,0x01);
}


void PAS6175_Set_H_Blank(UINT32 HBlank)
{	
	UINT32 lpf_min = 0;

	camerap_WriteOneReg(0xef,0x01);		
	camerap_WriteOneReg(0x03,HBlank&0xFF);
	camerap_WriteOneReg(0x80,HBlank>>8);
	camerap_WriteOneReg(0x11,0x01);		
	if(capture_state == TRUE)//Capture Mode
		return;
	
	if((PAS6167_Output_Continuous == TRUE) ||(PAS6175_LCD_BW==TRUE))
		lpf_min = 366;
	else
		lpf_min = 400;  //For Output One Frame
		 				// 500 => 9.25 fps
		 				// 400 => 11.49 fps
	lpf_min = 366;
	#if 1
	camerap_WriteOneReg(0xef,0x01);
	camerap_WriteOneReg(0x04,lpf_min&0x00FF);		/* Cmd_Lpf[7:0] */
	camerap_WriteOneReg(0x05,(lpf_min&0x0700)>>8);	/* Cmd_Lpf[13:8] */
	camerap_WriteOneReg(0x0E,0x09);		/* Cmd_OffNy[7:0] */
	camerap_WriteOneReg(0x0F,0x00);	/* Cmd_OffNy[13:8] */
	camerap_WriteOneReg(0x11,0x01);	
	
	camerap_WriteOneReg(0xef,0x00);
	camerap_WriteOneReg(0x13,lpf_min&0x00FF);				/* R_lpf_min[7:0] */
	camerap_WriteOneReg(0x14,(9<<4)|((lpf_min&0x0700)>>8));	/* R_lpf_min[10:8] */
	camerap_WriteOneReg(0xed,0x01);
  #endif
}

UINT32 PAS6175_Read_Shutter(void)
{
	UINT16 nov_size = 0;
	UINT32 ex_pixels = 0;
	camerap_WriteOneReg(0xef,0x01);		
	lpf_value = ((camerap_ReadReg(0x05)&0x3F)<<8)|camerap_ReadReg(0x04);	/* Cmd_Lpf[13:0] */
	ny_value = ((camerap_ReadReg(0x0f)&0x3F)<<8)|camerap_ReadReg(0x0e);	/* Cmd_OffNy[13:0] */
	ne_value = (camerap_ReadReg(0x91)<<8)|camerap_ReadReg(0x0d);			/* Cmd_OffNe[15:0] */
	nov_size = (camerap_ReadReg(0x80)<<8)|camerap_ReadReg(0x03);			/* Cmd_Nov_Size[15:0] */

	ex_pixels = (lpf_value+1-ny_value)*(Hsize+nov_size+1)-ne_value;	

	return ex_pixels;
}

void PAS6175_Write_Shutter(UINT32 ex_pixels)
{
	UINT16 lpf = 0;
	UINT16 ny = 0;
	UINT16 ne = 0;
	UINT16 shutter = 0;
	UINT16 line_size = 0;
	UINT8 AE_stage = 0;
  UINT8 AG_stage = 0;
	camerap_WriteOneReg(0xef,0x01);
	if(preview_state ==TRUE)
	{
    /* 如果是預覽模式，回填剛剛拍照時所以讀取的曝光參數。 */
		camerap_WriteOneReg(0x0d,(ne_value&0x00ff));		/* Cmd_OffNe[7:0] */
		camerap_WriteOneReg(0x91,(ne_value&0xff00)>>8); 	/* Cmd_OffNe[15:8] */				
		camerap_WriteOneReg(0x04,(lpf_value&0x00ff));		/* Cmd_Lpf[7:0] */
		camerap_WriteOneReg(0x05,(lpf_value&0x3f00)>>8);	/* Cmd_Lpf[13:8] */
		camerap_WriteOneReg(0x0e,(ny_value&0x00ff));		/* Cmd_OffNy[7:0] */
		camerap_WriteOneReg(0x0f,(ny_value&0x3f00)>>8); 	/* Cmd_OffNy[13:8] */	
	}
	else
	{
        line_size = Hsize +  dummy_pixels+1;//((read_cmos_sensor(0x80)<<8)|read_cmos_sensor(0x03));

		if(ex_pixels > line_size)/*  超過一條 */
		{
			shutter = ex_pixels/line_size;
			shutter++;
		}
		else
			shutter = 1;

		lpf = lpf_value; //366;

		ny = lpf - shutter + 1;

		ne=  (lpf+1-ny)*line_size- ex_pixels;

		if(ny%2!=0)
		{
			lpf+=1;
			ny+=1;
		}
		 
		camerap_WriteOneReg(0x0d,(ne&0x00ff));		/* Cmd_OffNe[7:0] */
		camerap_WriteOneReg(0x91,(ne&0x1f00)>>8); 	/* Cmd_OffNe[15:8] */			
		camerap_WriteOneReg(0x04,(lpf&0x00ff));		/* Cmd_Lpf[7:0] */
		camerap_WriteOneReg(0x05,(lpf&0x3f00)>>8);	/* Cmd_Lpf[13:8] */
		camerap_WriteOneReg(0x0e,(ny&0x00ff));		/* Cmd_OffNy[7:0] */
		camerap_WriteOneReg(0x0f,(ny&0x3f00)>>8); 	/* Cmd_OffNy[13:8] */
	}
	camerap_WriteOneReg(0x11,0x01);
//解在高亮度拍照回預覽會畫面過曝的問題 
	if(preview_state == TRUE)
	{
		camerap_WriteOneReg(0xef,0x00);
		AG_stage=camerap_ReadReg(0xBC);
		AE_stage=camerap_ReadReg(0xBD);

		if(AG_stage<=1)				
			camerap_WriteOneReg(0xBC,2);
		else
			camerap_WriteOneReg(0xBC,AG_stage-1);	

		if(AE_stage<=1)				
			camerap_WriteOneReg(0xBD,2);
		else
			camerap_WriteOneReg(0xBD,AE_stage-1);	

		camerap_WriteOneReg(0xed,0x01);
	}
}

VOID PAS6175_AWB_Mode(BOOL AWB_Enable)
{
	UINT8 Reg_vlaue = 0;
	camerap_WriteOneReg(0xef,0x00);
	Reg_vlaue = camerap_ReadReg(0x72);
	if(AWB_Enable == TRUE)
		Reg_vlaue |= 0x01;
	else
		Reg_vlaue &= 0xfe;
	
	camerap_WriteOneReg(0x72,Reg_vlaue);	
  

}

void PAS6175_AE_Mode(BOOL AE_Enable)
{
	UINT8 Reg_vlaue = 0;
	
	camerap_WriteOneReg(0xef,0x00);
	Reg_vlaue = camerap_ReadReg(0x66);


	if(AE_Enable == TRUE)
		Reg_vlaue |= 0x10;
	else
		Reg_vlaue &= 0xef;

	camerap_WriteOneReg(0x66,Reg_vlaue);	
  camerap_WriteOneReg(0xed,0x01);



  
}

VOID PAS6175_Skip_Frame(UINT8 SkipFrameNumber)
{
	UINT8 FrameCnt1 = 0;
	UINT8 FrameCnt2 = 0;
	UINT8 SkipFrameCnt = 0;
	UINT32 i = 0;
	UINT32 j = 0;


	SkipFrameCnt = SkipFrameNumber;

	camerap_WriteOneReg(0xef,0x00);

	for(j=0;j<(SkipFrameCnt/3);j++)
	{
		FrameCnt1 = (camerap_ReadReg(0xb9)&0x70)>>4;

		for(i=0;i<0xffffffff;i++)
		{
			FrameCnt2 = (camerap_ReadReg(0xb9)&0x70)>>4;

			if((FrameCnt1-FrameCnt2>=3)||(FrameCnt2-FrameCnt1>=3))
				break;
		};
	}

	SkipFrameCnt -= (j*3);

	if(SkipFrameCnt == 0)
		return;

	FrameCnt1 = (camerap_ReadReg(0xb9)&0x70)>>4;
	
	for(i=0;i<0xffffffff;i++)
	{
		FrameCnt2 = (camerap_ReadReg(0xb9)&0x70)>>4;
	
		if((FrameCnt1-FrameCnt2>=SkipFrameCnt)||(FrameCnt2-FrameCnt1>=SkipFrameCnt))
			break;
	};
}


void PAS6175_SPI_Slave_Mode_Enable(void)
{
	camerap_WriteOneReg(0xef,0x00);
	camerap_WriteOneReg(0x8C,0x00);	
	camerap_WriteOneReg(0xef,0x02);// enter slave mode
	camerap_WriteOneReg(0xc1,0x00);// 
	camerap_WriteOneReg(0xbf,0x00);//  YC swap [2] 
	camerap_WriteOneReg(0xc4,0x04);//{ 5'b0,R_SPI_Slave_En,R_SPI_SSn_VsyncTim, R_SPI_En };
//write_cmos_sensor(0xC5,0x1e);// R_SPI_FIFO_Ptr[5:0]
	camerap_WriteOneReg(0xC6,0x01);//
	camerap_WriteOneReg(0xa4,0x02);//
	camerap_WriteOneReg(0xa5,0x00);//

  
//	camerap_WriteOneReg(0xa6,0x04);//set ready trig when > n words .  2=>32words
  	camerap_WriteOneReg(0xa6,0x20);//set ready trig when > n words .  2=>32words
  	camerap_WriteOneReg(0x00,0x01);
}

void PAS6175_SPI_Master_Mode_Enable(void)
{

	camerap_WriteOneReg(0xef,0x00);
	camerap_WriteOneReg(0x8C,0x00);	
	camerap_WriteOneReg(0xef,0x02);
	camerap_WriteOneReg(0xc1,0x10);	
	camerap_WriteOneReg(0xC4,0x43);//SPI Master Mode enable
	camerap_WriteOneReg(0xC6,0x02);//SPI Master Mode enable
	camerap_WriteOneReg(0xa4,0x04);//
	camerap_WriteOneReg(0xa5,0x00);//
//	write_cmos_sensor(0xa6,0x64);//
	camerap_WriteOneReg(0x00,0x01);
}


VOID PAS6167_Output_Enable(BOOL Output_Enable)
{ 
	UINT8 Reg_vlaue = 0;
	camerap_WriteOneReg(0xef,0x02);	
	if(Output_Enable == TRUE)
		Reg_vlaue = 1;
	else
		Reg_vlaue = 0;
	camerap_WriteOneReg(0xbf,Reg_vlaue);//camerap_WriteOneReg(0xbf,0x02); //output one frame
	camerap_WriteOneReg(0x00,0x01);	
}


void PAS6175_Output_Enable(BOOL Output_Enable)
{ 
	UINT8 Reg_vlaue = 0;

	
	camerap_WriteOneReg(0xef,0x02);	
	if(Output_Enable == TRUE)
		Reg_vlaue = 1;
	else
		Reg_vlaue = 0;
	camerap_WriteOneReg(0xbd,Reg_vlaue);
	camerap_WriteOneReg(0x00,0x01);	
}

void PAS6175_IO_TriState(BOOL TriState_Enable)
{
	UINT8 Reg_vlaue = 0;
	
	camerap_WriteOneReg(0xef,0x01);
	Reg_vlaue = camerap_ReadReg(0x22);

    Reg_vlaue= 0x8;// for test ytt
	if(TriState_Enable == TRUE)
		Reg_vlaue |= 0x04;  // chris 0906
	else
		Reg_vlaue &= 0xFB;  // chris 0906
	
	camerap_WriteOneReg(0x22,Reg_vlaue);	
}

UINT32 PAS6175_Set_Para_WB(UINT32 para)
{
	UINT8 Index = 0;
  UINT8 temp = 0;
  camerap_WriteOneReg(0xef,0x01);
  temp = camerap_ReadReg(0x80);
  //hal_HstSendEvent(0x77013); 
 // hal_HstSendEvent((UINT32)temp); // first 0x0;
	if(para == 0)
	{
		camerap_WriteOneReg(0xef,0x01);
		camerap_WriteOneReg(0x06,Cmd_Cgn_Gb);//Cmd_Cgn_Gb
		camerap_WriteOneReg(0x07,Cmd_Cgn_B);//Cmd_Cgn_B
		camerap_WriteOneReg(0x08,Cmd_Cgn_Gr);//Cmd_Cgn_Gr
		camerap_WriteOneReg(0x09,Cmd_Cgn_R);//Cmd_Cgn_R
		camerap_WriteOneReg(0x11,0x01);
	
		camerap_WriteOneReg(0xef,0x00);
		camerap_WriteOneReg(0x72,0x01);       
		camerap_WriteOneReg(0xed,0x01);
		camerap_WriteOneReg(0x80,temp|0x78);  // set[11~14] to 1
		camerap_WriteOneReg(0xed,0x01);

		return TRUE;
	}           

	if(preview_state==TRUE)
	{
		//if(para!=CAM_WB_AUTO)
		{
			camerap_WriteOneReg(0xef,0x00);
			camerap_WriteOneReg(0x72,0x10);
			camerap_WriteOneReg(0xed,0x01);
			camerap_WriteOneReg(0x80, temp&0x87);  // set[11~14] to 0
			camerap_WriteOneReg(0xed,0x01);

		}

			

		camerap_WriteOneReg(0xef,0x01);
		camerap_WriteOneReg(0x06,PAS6175_WB_MODE[Index-1][0]);//Cmd_Cgn_Gb
		camerap_WriteOneReg(0x07,PAS6175_WB_MODE[Index-1][1]);//Cmd_Cgn_B
		camerap_WriteOneReg(0x08,PAS6175_WB_MODE[Index-1][2]);//Cmd_Cgn_Gr
		camerap_WriteOneReg(0x09,PAS6175_WB_MODE[Index-1][3]);//Cmd_Cgn_R
		camerap_WriteOneReg(0x11,0x01);
	}
	return TRUE;
    
}


UINT32 PAS6175_Set_Para_Effect(UINT32 iPara)
{
	UINT8 index = 0;
	switch (iPara)
	{	
		case CAM_EFFECT_ENC_NORMAL:	
			index = 0;
		break;	
			
		case CAM_EFFECT_ENC_GRAYSCALE:	
			index = 1;
		break;	
			
		case CAM_EFFECT_ENC_SEPIA:	
			index = 2;
		break;	
			
		case CAM_EFFECT_ENC_COLORINV:	
			index = 3;
		break;	
			
		case CAM_EFFECT_ENC_SEPIAGREEN:
			index = 4;
		break;	
			
		case CAM_EFFECT_ENC_SEPIABLUE:
			index = 5;
		break;	

		default:
			index = 0; //CAM_EFFECT_ENC_NORMAL
		break;
	}	
	camerap_WriteOneReg(0xef,0x00);
	if(index == 0)
	{
		camerap_WriteOneReg(0x94,0x00);//R_ISP_ImgEffect_En
	}
	else
	{
		camerap_WriteOneReg(0x8f,PAS6175_Effect[index-1][2]);//R_ImgEffect_c0
		camerap_WriteOneReg(0x90,PAS6175_Effect[index-1][3]);//R_ImgEffect_c1
		camerap_WriteOneReg(0x93,PAS6175_Effect[index-1][0]);//R_ImgEffectMode
		camerap_WriteOneReg(0x94,PAS6175_Effect[index-1][1]);//R_ISP_ImgEffect_En
	}
	camerap_WriteOneReg(0xed,0x01);
	return TRUE;
}

void PAS6175_NightMode(BOOL bEnable)
{  
	UINT8 Index = 0;


	if (bEnable == TRUE)
		Index = 1;
	else
		Index = 0;

	camerap_WriteOneReg(0xef,0x00);
    camerap_WriteOneReg(0x6C,PAS6175_AeStage_GainStage[Index][0]);//R_AE_maxStage[4:0]
    camerap_WriteOneReg(0x6D,PAS6175_AeStage_GainStage[Index][1]);//R_AG_stage_UB
	camerap_WriteOneReg(0xed,0x01);	

	camerap_WriteOneReg(0xef,0x02);
    camerap_WriteOneReg(0x2F,PAS6175_AeStage_GainStage[Index][2]);//R_AE_stage_LL[4:0]
    camerap_WriteOneReg(0x30,PAS6175_AeStage_GainStage[Index][3]);//R_AE_stage_NL[4:0]
    camerap_WriteOneReg(0x36,PAS6175_AeStage_GainStage[Index][4]);//R_Gamma_Strength_Delta[3:0]
//    write_cmos_sensor(0x5F,PAS6175_AeStage_GainStage[Index][5]);//R_Saturation_LL[4:0]
	camerap_WriteOneReg(0x00,0x01);
}


enum {
	CAM_EV_NEG_4_3 = 0,
	CAM_EV_NEG_3_3,
	CAM_EV_NEG_2_3,
	CAM_EV_NEG_1_3,
	CAM_EV_ZERO,
	CAM_EV_POS_1_3,
	CAM_EV_POS_2_3,
	CAM_EV_POS_3_3,
	CAM_EV_POS_4_3,
	CAM_EV_NIGHT_SHOT,
	CAM_NO_OF_EV	
};

UINT32 PAS6175_Set_Para_EV(UINT32 iPara)
{	
	UINT8 Brightness_Offset = 0;

	switch (iPara)
	{
		case CAM_EV_NEG_4_3:
			Brightness_Offset = 255-80;
		break;	
		
		case CAM_EV_NEG_3_3:
			Brightness_Offset = 255-60;
		break;	
			
		case CAM_EV_NEG_2_3:	
			Brightness_Offset = 255-40;
		break;	
			
		case CAM_EV_NEG_1_3://EV-1
			Brightness_Offset = 255-20;
		break;	
			
		case CAM_EV_ZERO:	//EV 0
		
		break;	
			
		case CAM_EV_POS_1_3://EV +1
			Brightness_Offset = 20;
		break;	
			
		case CAM_EV_POS_2_3:
			Brightness_Offset = 40;
		break;	
			
		case CAM_EV_POS_3_3:
			Brightness_Offset = 60;
		break;	
			
		case CAM_EV_POS_4_3:				
			Brightness_Offset = 80;
		break;
			
		default:
			return FALSE;

	}	
	camerap_WriteOneReg(0xef,0x02);
	Brightness_Value_LL = camerap_ReadReg(0x69);

	Brightness_Value_NL = camerap_ReadReg(0x6A);
 
	camerap_WriteOneReg(0x69,Brightness_Value_LL + Brightness_Offset);
	camerap_WriteOneReg(0x6A,Brightness_Value_NL + Brightness_Offset);
	camerap_WriteOneReg(0x00,0x01);

	return TRUE;	
}



BOOL g_bBanding50Hz = TRUE;
enum
{
	CAM_BANDING_50HZ=0,
	CAM_BANDING_60HZ,
	CAM_NO_OF_BANDING
};


UINT32 PAS6175_Set_Para_Banding(UINT32 iPara)
{	
	UINT8 Reg_vlaue = 0;
	

	camerap_WriteOneReg(0xef,0x00);
	Reg_vlaue = camerap_ReadReg(0x66)&0xFE;

	switch (iPara)
	{		
		case CAM_BANDING_50HZ:
			g_bBanding50Hz = TRUE;
		break;

		case CAM_BANDING_60HZ:
			g_bBanding50Hz = FALSE;
		break;     		

		default:
			return FALSE;		
	}	
	camerap_WriteOneReg(0x66,Reg_vlaue|(!g_bBanding50Hz));
	camerap_WriteOneReg(0xed,0x01);
	
	return TRUE;
}
VOID PAS_FrameHeadDetect(VOID);
VOID PAS_FrameStopDetect();



//UINT16 display_sensor[128*160]={0};
//UINT16 display_sensor2[128*160]={0};
VOID test_reg_all()
{

#if 0
  return;
    UINT32 kk = 0;
    UINT32 data = 0;
    camerap_WriteOneReg(0xef,0x00);
    hal_HstSendEvent(0x36900000);  

    for(kk = 0;kk<0xed;kk++)
    {
       data = camerap_ReadReg(kk);
       hal_HstSendEvent(0x99000000|(kk<<8)|(data));  
    }
    
    camerap_WriteOneReg(0xef,0x01);
    hal_HstSendEvent(0x36900001);  

    for(kk = 0;kk<0xbe;kk++)
    {
       data = camerap_ReadReg(kk);
       hal_HstSendEvent(0x99010000|(kk<<8)|(data));  
    }

    camerap_WriteOneReg(0xef,0x02);
    hal_HstSendEvent(0x36900002);  

     for(kk = 0;kk<0xeb;kk++)
    {
       data = camerap_ReadReg(kk);
       hal_HstSendEvent(0x99020000|(kk<<8)|(data));  
    }

       hal_HstSendEvent(0x36900111);  

#endif
}

#define pas_buffer_len 32*2*4   // u8 type

UINT16 display_sensor_temp[128]={0};

VOID PAS_SPI_GetDateDma(VOID)
{
    UINT16 g_dmaBusy=0;
    UINT8 tempbuff[pas_buffer_len]={0};
    s_spi_interFinished = FALSE;
    //sensor_frame_info.read_control = TRUE;
    hal_SpiClearRxDmaDone(HAL_SPI_2);
    hal_SpiClearTxDmaDone(HAL_SPI_2);
    hal_SpiFlushFifos(HAL_SPI_2);

   // g_dmaBusy = hal_SpiGetData(HAL_SPI_2,HAL_SPI_CS0,(UINT8*)sensor_frame_info.current_buff, pas_buffer_len);
    g_dmaBusy = hal_SpiSendData(HAL_SPI_2,HAL_SPI_CS0,tempbuff, pas_buffer_len);
    if (g_dmaBusy == 0)
    {
     hal_DbgAssert("spi rx dma busy");
    }
}
  UINT32 pas_now1  ;
  UINT32 pas_now2  ; 
 BOOL pasflag=TRUE;
 UINT32 delay_frame = 0;

VOID serial_camer_dma_int(HAL_SPI_IRQ_STATUS_T spi_irq)
{
  #if 0
    BOOL ready = FALSE;
    if(spi_irq.rxDmaDone != 1)return;
    sensor_frame_info.count+=pas_buffer_len / 2;
    if(sensor_frame_info.buff1_full == FALSE)
    {
        sensor_frame_info.current_buff += pas_buffer_len;
        if (sensor_frame_info.count >= 128*160)
        {
           hal_HstSendEvent(0x9900444);
           sensor_frame_info.buff1_full = TRUE;
           sensor_frame_info.buff2_full = FALSE;
           sensor_frame_info.current_buff = sensor_frame_info.buff2;
           sensor_frame_info.count = 0;
         // Cam_CallbackFun(0, (UINT32)display_sensor);
        }
    }
    else
    {
         sensor_frame_info.current_buff += pas_buffer_len;
         if (sensor_frame_info.count >= 128*160)
         {
            hal_HstSendEvent(0x9900555);
            delay_frame++;
            sensor_frame_info.buff2_full = TRUE;
            sensor_frame_info.buff1_full = FALSE;
            sensor_frame_info.current_buff = sensor_frame_info.buff1;//1
            sensor_frame_info.count = 0;
            //sensor_frame_info.read_control=FALSE;
            //return;
           // Cam_CallbackFun(0, (UINT32)display_sensor2);

         }
    }
   // memcpy((UINT8*)display_sensor_temp, (CONST UINT8*)HAL_SYS_GET_UNCACHED_ADDR(display_sensor_temp),pas_buffer_len);
    
    ready = hal_GpioGet(g_sensor_ready.gpioId);
    if(ready == TRUE)
    {
      PAS_SPI_GetDateDma();
    }
    else
    {
      sensor_frame_info.read_control = FALSE;
      hal_SpiClearRxDmaDone(HAL_SPI_2);
      hal_SpiClearTxDmaDone(HAL_SPI_2);

    }
    s_spi_interFinished = TRUE;
#endif
}



VOID PAS_Frame_ready_int(VOID)
{ 
  #if 0

    BOOL ready = FALSE;

    ready = hal_GpioGet(g_sensor_overflow.gpioId);
    if (ready == TRUE)
    {
        hal_HstSendEvent(0x6600777);
        PAS_FrameStopDetect();
        PAS_FrameHeadDetect();
        return;

    }
   if((sensor_frame_info.startSample == TRUE)&&(sensor_frame_info.read_control==FALSE))
    {

     PAS_SPI_GetDateDma();
    }
   #endif
}


VOID PAS_FundFrameHeader(VOID)
{
  #if 0
    HAL_GPIO_CFG_T ready_Cfg;

    hal_HstSendEvent(0x1234567);
   // BOOL ready = FALSE;
 //   hal_HstSendEvent((UINT32)display_sensor);

  //  ready = hal_GpioGet(g_sensor_ready.gpioId);    hal_HstSendEvent((UINT32)ready);

    sensor_frame_info.startSample = TRUE;
   // sensor_frame_info.current_buff = sensor_frame_info.buff1;
    sensor_frame_info.count = 0;
    sensor_frame_info.read_control=FALSE;
    #if 0
    HAL_GPIO_CFG_T overflow_Cfg;
    overflow_Cfg.direction = HAL_GPIO_DIRECTION_INPUT;
    overflow_Cfg.irqHandler = NULL;
    overflow_Cfg.irqMask.rising=FALSE;
    overflow_Cfg.irqMask.falling = FALSE;
    overflow_Cfg.irqMask.debounce = FALSE;
    overflow_Cfg.irqMask.level=FALSE;
    hal_GpioOpen(g_sensor_overflow.gpioId,&overflow_Cfg);
    hal_GpioResetIrq(g_sensor_overflow.gpioId);
    #endif

    ready_Cfg.direction = HAL_GPIO_DIRECTION_INPUT;
    ready_Cfg.irqHandler = PAS_Frame_ready_int;
    ready_Cfg.irqMask.rising=TRUE;
    ready_Cfg.irqMask.falling = FALSE;
    ready_Cfg.irqMask.debounce = TRUE;
    ready_Cfg.irqMask.level=FALSE;
    hal_GpioOpen(g_sensor_ready.gpioId,&ready_Cfg);

    #if 0
    HAL_GPIO_CFG_T eraCfg;
    eraCfg.irqMask.rising = FALSE;
    eraCfg.irqMask.falling = FALSE;
    eraCfg.irqMask.debounce = TRUE;
    eraCfg.irqMask.level = FALSE;
    hal_GpioIrqSetMask(g_ear_gpio, &eraCfg.irqMask);
    hal_GpioResetIrq(g_ear_gpio);
    sxr_StopFunctionTimer(pmd_EarDetectDelay);
    sxr_StartFunctionTimer(HAL_TICK1S, pmd_EarDetectDelay, NULL, 0);
    #endif
    #endif
}


VOID PAS_FrameHeadDetect(VOID)
{
  #if 0
   #if 0
    if ( gpioId != HAL_GPIO_NONE &&
         (gpioId < HAL_GPIO_1 || gpioId > HAL_GPIO_7) )
    {
        PMD_ASSERT(FALSE, "Earpice detect GPIO should be none or from 1 to 7: 0x%08x", gpioId);
    }
    if (gpioId == HAL_GPIO_NONE)
    {
        return;
    }
   #endif
    hal_HstSendEvent(0x670001);
    sensor_frame_info.startSample = TRUE;
    sensor_frame_info.buff1 =(UINT8 *) (display_sensor2+128*160);
    sensor_frame_info.buff2 =(UINT8 *) display_sensor2;
    sensor_frame_info.current_buff = sensor_frame_info.buff1;
    sensor_frame_info.count = 0;
   // hal_HstSendEvent((UINT32)display_sensor);
    hal_HstSendEvent((UINT32)display_sensor2);
    
    HAL_GPIO_CFG_T overflow_Cfg;
    overflow_Cfg.direction = HAL_GPIO_DIRECTION_INPUT;
    overflow_Cfg.irqHandler = PAS_FundFrameHeader;
    overflow_Cfg.irqMask.rising=FALSE;
    overflow_Cfg.irqMask.falling = TRUE;
    overflow_Cfg.irqMask.debounce = TRUE;
    overflow_Cfg.irqMask.level=FALSE;
    hal_GpioOpen(g_sensor_overflow.gpioId,&overflow_Cfg);
    #endif
}

VOID PAS_FrameStopDetect()
{
  #if 0
    HAL_GPIO_CFG_T ready_Cfg;
    hal_HstSendEvent(0x6600005);
    
    ready_Cfg.direction = HAL_GPIO_DIRECTION_INPUT;
    ready_Cfg.irqHandler = NULL;
    ready_Cfg.irqMask.rising=FALSE;
    ready_Cfg.irqMask.falling = FALSE;
    ready_Cfg.irqMask.debounce = FALSE;
    ready_Cfg.irqMask.level=FALSE;
    hal_GpioOpen(g_sensor_ready.gpioId,&ready_Cfg);
    hal_GpioResetIrq(g_sensor_ready.gpioId);
    HAL_GPIO_CFG_T overflow_Cfg;
    overflow_Cfg.direction = HAL_GPIO_DIRECTION_INPUT;
    overflow_Cfg.irqHandler = NULL;
    overflow_Cfg.irqMask.rising=FALSE;
    overflow_Cfg.irqMask.falling = FALSE;
    overflow_Cfg.irqMask.debounce = FALSE;
    overflow_Cfg.irqMask.level=FALSE;
    hal_GpioOpen(g_sensor_overflow.gpioId,&overflow_Cfg);
    hal_GpioResetIrq(g_sensor_ready.gpioId);
    sensor_frame_info.startSample = FALSE;
    sensor_frame_info.count = 0;
    sensor_frame_info.overfolw = FALSE;
    sensor_frame_info.close = FALSE;
    sensor_frame_info.full=FALSE;
#endif

}

#if 0
UINT32 PAS_Cam_ReadFrame_INT(UINT16 * buff, UINT16 size)
{
  PAS_FrameHeadDetect();
  return 1;
}


VOID test_read_serial_camer_INT()
{
    PAS_Cam_ReadFrame_INT(NULL,128*160);
    //camerap_PowerOff();SXS_TRACE(TSTDOUT,"DRV_CAM: power down") ;
}
#endif
UINT8 test_color = 5;


extern VOID lcd_ili9163c_SetWidows_sensor();

void PAS6175_Preview(UINT16 image_target_width, UINT16 image_target_height,UINT8 clk_NP,UINT32 HBlank)
{

  if (power_on == FALSE)
  {
      power_on = TRUE;
      camerap_PowerOnForPreview(CAM_NPIX_QVGA,128,160,CAM_FORMAT_RGB565);
  }
  if (preview_first == FALSE)
  {
      preview_first = TRUE;
  }
  else
  {
    hal_GpioSet(g_csb_gpo);
    #ifndef _LCD_CAMERA_USE_SAME_SPI_
    Lcd_SetIO_TriState(TRUE);
    lcddp_SensorBB_SPI_SetIO_TriState(TRUE);
    #endif
    hal_GpioClr(g_csb_gpo);
  }
  capture_state = FALSE;
  preview_state = TRUE;

  PAS6175_Output_Enable(FALSE);
  PAS6175_IO_TriState(FALSE);
  PAS6175_AE_Mode(FALSE);
	//UINT32 exposure_pixels = PAS6175_Read_Shutter();
	#if 0
  test_color++;
  if(test_color ==7) test_color = 0;
  camerap_WriteOneReg(0xef,0x0);
  camerap_WriteOneReg(0x0c,3);//test_color
  camerap_WriteOneReg(0xed,0x01);	
  if(test_color==0) test_color=5;
  #endif

   dummy_pixels = HBlank;
   preview_pclk_division = clk_NP;
   PAS6175_Output_Format(0x10);
 //  PAS6175_Output_Format(0x0);
   PAS6175_Clock_Set(clk_NP);//preview_pclk_division
   PAS6175_Set_Image_Size(image_target_width,image_target_height);
   PAS6175_Set_H_Blank(HBlank);//60 preview
   if(lpf_value!=0)
   PAS6175_Write_Shutter(exposure_pixels);		
   PAS6175_AWB_Mode(TRUE);
   PAS6175_AE_Mode(TRUE);
   #if 1
   PAS6175_Set_Para_WB(0);
   PAS6175_Set_Para_Effect(0);
   PAS6175_Set_Para_EV(4);
   PAS6175_NightMode(FALSE);
   PAS6175_Set_Para_Banding(0);
   #endif
   // turn on AE write to bank1 
   camerap_WriteOneReg(0xef,0x00);
   camerap_WriteOneReg(0x7f,0xff);
   camerap_WriteOneReg(0x80,0xfe);
   PAS6175_Skip_Frame(5);
	PAS6175_SPI_Master_Mode_Enable();

  PAS6175_IO_TriState(TRUE);
// set the lcd widowns
  lcd_ili9163c_SetWidows_sensor();

  PAS6175_IO_TriState(FALSE);
  
 	PAS6175_Output_Enable(TRUE);
   
}



VOID PAS6175_ReadShutterOfPreview()
{
  PAS6175_Output_Enable(FALSE);
  PAS6175_AE_Mode(FALSE);
  	// turn off AE write to bank1
	camerap_WriteOneReg(0xef,0x00);
	camerap_WriteOneReg(0x7f,0x00);
	camerap_WriteOneReg(0x80,0x00);
  exposure_pixels = PAS6175_Read_Shutter();
//  hal_HstSendEvent(exposure_pixels);  
//  hal_HstSendEvent(0x4444666);  

}
void PAS6175_Capture(UINT16 image_target_width, UINT16 image_target_height,UINT8 clk_NP,UINT32 HBlank)
{
//  hal_HstSendEvent(0x3000666);  
  capture_state = TRUE;
  preview_state = FALSE;
  PAS6175_Output_Enable(FALSE);
  PAS6175_AE_Mode(FALSE);
  #if 0
 // turn on AE write to bank1 
  camerap_WriteOneReg(0xef,0x00);
  camerap_WriteOneReg(0x7f,0xff);
  camerap_WriteOneReg(0x80,0xfe);
  #else
	// turn off AE write to bank1
//	camerap_WriteOneReg(0xef,0x00);
//	camerap_WriteOneReg(0x7f,0x00);
	//camerap_WriteOneReg(0x80,0x00);
 // exposure_pixels = PAS6175_Read_Shutter();hal_HstSendEvent(exposure_pixels);  
  #endif
	#if 0
  test_color++;
  if(test_color ==7) test_color = 0;
  camerap_WriteOneReg(0xef,0x0);
  camerap_WriteOneReg(0x0c,3);//test_color
  camerap_WriteOneReg(0xed,0x01);	
  if(test_color==0) test_color=5;
  //test color 
  #endif
  dummy_pixels = HBlank;
  capture_pclk_division = clk_NP;
 // PAS6175_Output_Format(0x10);
   PAS6175_Output_Format(0x0);//YUV
  PAS6175_Clock_Set(clk_NP);//preview_pclk_division
  PAS6175_Set_Image_Size(image_target_width,image_target_height);
  PAS6175_Set_H_Blank(HBlank);//60        150(128*160)
  #if 0
  PAS6175_Set_Para_WB(0);
  PAS6175_Set_Para_Effect(0);
  PAS6175_Set_Para_EV(4);
  PAS6175_NightMode(FALSE);
  PAS6175_Set_Para_Banding(0);
  #endif
	exposure_pixels = exposure_pixels * preview_pclk_division / capture_pclk_division;
   PAS6175_Write_Shutter(exposure_pixels);
   PAS6175_SPI_Slave_Mode_Enable();
   PAS6175_IO_TriState(FALSE);
   PAS6175_Output_Enable(TRUE);
   PAS6175_Skip_Frame(1);
   camerap_Delay(30);
   
}

UINT8 for_serial_camer = 0;
extern  VOID test_display_sensor(UINT16 * temp);
UINT32 PAS_Cam_ReadFrame_continue_display(UINT16 * buff, UINT32 size)
{
 #if 0
  UINT16 bufflend = 64;
  UINT16 timer = 0 ;
	UINT32 j = 0;
	UINT32 counts = 0,now1=0,now2=0;
	UINT8 * buff1;
  UINT16 g_dmaBusy=0;
   hal_HstSendEvent(0x3000212);  
  buff1 = (UINT8 *)buff;
  UINT8 tempbuff[128]={0};

	for(j = 0; j < 100;j++)
	{
    if(counts > 128*160)
    {
  	    counts = 128*160;
    }
    else
    {
       counts = 0;
    }
    buff1 = (UINT8 *)buff;

   	while(hal_GpioGet(g_sensor_overflow.gpioId)== FALSE)
       {
         if( pas_capture == FALSE)
         {
            pas_close_tri_spi();         
            return 1;
         }

         sxr_Sleep(2 MS_WAITING);
       };
   	while(hal_GpioGet(g_sensor_overflow.gpioId)== TRUE)
   	{
       sxr_Sleep(2 MS_WAITING);
        if( pas_capture == FALSE)
         {
            pas_close_tri_spi();         
            return 1;
         }

   	}
      hal_SpiClearRxDmaDone(HAL_SPI_2);
      hal_SpiClearTxDmaDone(HAL_SPI_2);
      hal_SpiFlushFifos(HAL_SPI_2);

  
		//while(counts < size )
    while(1)   
		{     
          timer++;
        if( pas_capture == FALSE)
          {
            pas_close_tri_spi();         
            return 1;
          }

    			while(hal_GpioGet(g_sensor_ready.gpioId)==FALSE)
    			{  

            if( pas_capture == FALSE)
              {
                  pas_close_tri_spi();         
                  return 1;
              }

            
    				if(hal_GpioGet(g_sensor_overflow.gpioId)==TRUE)
    				{			 
           		hal_HstSendEvent(0x30013002);  hal_HstSendEvent(timer); 
             // hal_DbgAssert(" next frame");
    					goto new_frame;
    				}
    	    }
         
        counts +=bufflend/2;
        g_dmaBusy = hal_SpiGetData(HAL_SPI_2,HAL_SPI_CS0,buff1, bufflend);
        g_dmaBusy = hal_SpiSendData(HAL_SPI_2,HAL_SPI_CS0,tempbuff, bufflend);
        if (g_dmaBusy == 0)
        {
         hal_DbgAssert("spi rx dma busy count = %d",counts);
         return 1;
        }
        
        while(!hal_SpiRxDmaDone(HAL_SPI_2,HAL_SPI_CS0))
        {
          if( pas_capture == FALSE)
            {
                pas_close_tri_spi();         
                return 1;
            }
         }  
        // memcpy((UINT8*)buff1, (CONST UINT8*)HAL_SYS_GET_UNCACHED_ADDR(buff1),bufflend);
        hal_SpiClearRxDmaDone(HAL_SPI_2);
        hal_SpiClearTxDmaDone(HAL_SPI_2);
        hal_SpiFlushFifos(HAL_SPI_2);
    
        //hal_HstSendEvent((UINT32)buff1[counts]); 
        buff1 = buff1 + bufflend;
        if(counts == size/2)
        {
          test_display_sensor(buff);
        }
        if(counts == size)
        {
           
            counts = 0;
            buff1 = (UINT8 *)buff;
            hal_HstSendEvent(0x2002);
            now2 = hal_TimGetUpTime();
            hal_HstSendEvent((now2-now1)*1000/16384);  
            test_display_sensor(buff+128*160);

        }
        if(counts == 0)
        {
           now1 = hal_TimGetUpTime();
        }
        if( pas_capture == FALSE)
          {
            pas_close_tri_spi();         
            return 1;

          }

		}
    
		new_frame:
      
		if(counts == size)
			counts = 0;
    if(j == 500)  hal_DbgAssert(" too many bad image");
	}
  
   
   
   hal_HstSendEvent(0x388862); 
   SXS_TRACE(TSTDOUT, "sensor speed on frame %d ms ",(now2-now)*1000/16384);

	return counts;
  #endif 
 	return 1;

}

UINT32 PAS_Cam_ReadFrame_continue(UINT16 * buff, UINT32 size)
{
  #if 0

  UINT16 bufflend = 64*2;
  UINT16 timer = 0 ;
	UINT32 j = 0;
	UINT32 counts = 0,now1=0,now2=0;
	UINT8 * buff1;
  UINT16 g_dmaBusy=0;
   hal_HstSendEvent(0x3000212);  
  buff1 = (UINT8 *)buff;
  UINT8 tempbuff[128]={0};

	for(j = 0; j < 100;j++)
	{
    if(counts > 128*160)
    {
  	    counts = 128*160;
    }
    else
    {
       counts = 0;
    }
    buff1 = (UINT8 *)buff;

   	while(hal_GpioGet(g_sensor_overflow.gpioId)== FALSE)
       {
         sxr_Sleep(2 MS_WAITING);
       };
   	while(hal_GpioGet(g_sensor_overflow.gpioId)== TRUE)
   	{
       sxr_Sleep(2 MS_WAITING);
   	}
      hal_SpiClearRxDmaDone(HAL_SPI_2);
      hal_SpiClearTxDmaDone(HAL_SPI_2);
      hal_SpiFlushFifos(HAL_SPI_2);

  
		//while(counts < size )
    while(1)   
		{     
          timer++;

    			while(hal_GpioGet(g_sensor_ready.gpioId)==FALSE)
    			{  
            // sxr_Sleep(2 MS_WAITING);
    				if(hal_GpioGet(g_sensor_overflow.gpioId)==TRUE)
    				{			 
           		hal_HstSendEvent(0x30013002);  hal_HstSendEvent(timer); 
             // hal_DbgAssert(" next frame");
    					goto new_frame;
    				}
    	    }
         
        counts +=bufflend/2;
        g_dmaBusy = hal_SpiGetData(HAL_SPI_2,HAL_SPI_CS0,buff1, bufflend);
        g_dmaBusy = hal_SpiSendData(HAL_SPI_2,HAL_SPI_CS0,tempbuff, bufflend);
        if (g_dmaBusy == 0)
        {
         hal_DbgAssert("spi rx dma busy count = %d",counts);
         return 1;
        }
        while(!hal_SpiRxDmaDone(HAL_SPI_2,HAL_SPI_CS0))
        {
          //sxr_Sleep(100 MS_WAITING); 
        }  
        // memcpy((UINT8*)buff1, (CONST UINT8*)HAL_SYS_GET_UNCACHED_ADDR(buff1),bufflend);
        hal_SpiClearRxDmaDone(HAL_SPI_2);
        hal_SpiClearTxDmaDone(HAL_SPI_2);
        hal_SpiFlushFifos(HAL_SPI_2);
    
        //hal_HstSendEvent((UINT32)buff1[counts]); 
        buff1 = buff1 + bufflend;
        if(counts == size)
        {
           
            counts = 0;
            buff1 = (UINT8 *)buff;
            hal_HstSendEvent(0x2002);
            now2 = hal_TimGetUpTime();
            hal_HstSendEvent((now2-now1)*1000/16384);  

        }
        if(counts == 0)
        {
           now1 = hal_TimGetUpTime();
        }
        if( pas_capture == FALSE){hal_HstSendEvent(0x77777); return 1;}

		}
    
		new_frame:
      
		if(counts == size)
			counts = 0;
    if(j == 99)  hal_DbgAssert(" too many bad image");
	}
  
 //  test_display_sensor(buff);
   
   hal_HstSendEvent(0x388862); 
   SXS_TRACE(TSTDOUT, "sensor speed on frame %d ms ",(now2-now)*1000/16384);

	return counts;
  #endif
  return 1;
}

UINT32 PAS_Cam_ReadFrame(UINT16 * buff, UINT32 size)
{

   UINT16 bufflend = 64;
   UINT16 timer = 0 ;
   UINT32 j = 0;
   UINT32 counts = 0;
   UINT8 * buff1;
   UINT16 g_dmaBusy=0;
   buff1 = (UINT8 *)buff;
   UINT8 tempbuff[128]={0};


   hal_SpiClearRxDmaDone(g_spi_id_sensor);
   hal_SpiClearTxDmaDone(g_spi_id_sensor);
   hal_SpiFlushFifos(g_spi_id_sensor);

   while(hal_GpioGet(g_sensor_overflow.gpioId)== FALSE)
   {
      sxr_Sleep(2 MS_WAITING);
   };
   
   while(hal_GpioGet(g_sensor_overflow.gpioId)== TRUE)
   {
     sxr_Sleep(1 MS_WAITING);
   }
   
   UINT32 now = hal_TimGetUpTime();
	for(j = 0; j < 1000;j++)
	{
		counts = 0;
        buff1 = (UINT8 *)buff;
  
		while(counts < size )
		{
            timer ++;
     
			while(hal_GpioGet(g_sensor_ready.gpioId)==FALSE)
			{  
         
               // sxr_Sleep(2 MS_WAITING);
               if(hal_GpioGet(g_sensor_overflow.gpioId)==TRUE)
               {			 
               
               if(j>900) 
               {
                   //hal_DbgAssert(" next frame");
                   hal_HstSendEvent(0x333ff33); 
                   return 0xffffffff;
               }
                   goto new_frame;
               }
            }

    counts +=bufflend/2;
    g_dmaBusy = hal_SpiGetData(g_spi_id_sensor,g_spi_cs_sensor,buff1, bufflend);
    g_dmaBusy = hal_SpiSendData(g_spi_id_sensor,g_spi_cs_sensor,tempbuff, bufflend);
    if (g_dmaBusy == 0)
    {
      
     hal_DbgAssert("spi rx dma busy count = %d",counts);
     return 1;
    }
       

    while(!hal_SpiRxDmaDone(g_spi_id_sensor,g_spi_cs_sensor))
    {
      //sxr_Sleep(100 MS_WAITING); 
    }  
   // hal_HstSendEvent(0x6666662); 
   // memcpy((UINT8*)buff1, (CONST UINT8*)HAL_SYS_GET_UNCACHED_ADDR(buff1),bufflend);
    hal_SpiClearRxDmaDone(g_spi_id_sensor);
    hal_SpiClearTxDmaDone(g_spi_id_sensor);
    hal_SpiFlushFifos(g_spi_id_sensor);
    buff1 = buff1 + bufflend;
   	//	while(hal_GpioGet(g_sensor_ready.gpioId)==TRUE){timer2++;};
    // hal_HstSendEvent((UINT32)timer2); 
    // while(hal_GpioGet(g_sensor_ready.gpioId)==TRUE){;};
   //LCD_CAM_SSN_HIGH(PAS6167_SSN_PIN_HIGH_ADDRESS,PAS6167_SSN_PIN_VALUE);
		}
		new_frame:
		if(counts == size)
			break;
	}
  
   //test_display_sensor(buff);
   UINT32 now2 = hal_TimGetUpTime();
//   hal_HstSendEvent(0x388862);  hal_HstSendEvent((now2-now)*1000/16384);  
   SXS_TRACE(TSTDOUT, "sensor speed on frame %d ms ",(now2-now)*1000/16384);

	return counts;
}


VOID camerap_pas6175_Capture_After_Preview(CAM_SIZE_T CamOutSize, UINT16 Width, UINT16 Height)
{
    PAS6175_ReadShutterOfPreview();
    PAS6175_Pause();
    Lcd_SetIO_TriState(TRUE);
    PAS6175_Resume();
    camerap_StartCapture(CAM_NPIX_QVGA,Width,Height);
}

VOID camerap_GetImageFrame(UINT16 * buff,UINT32 size)
{
  #ifdef _LCD_CAMERA_USE_SAME_SPI_
  close_spi_for_lcd();
  open_spi_for_sensor();
  #endif

  if(camerap_GetOneFrame(buff,size) == 0xffffffff)
  {
      sxr_Sleep(1000 MS_WAITING);
      camerap_GetOneFrame(buff,size);
  }
  PAS6175_Pause();
  Lcd_SetIO_TriState(FALSE);
  #ifdef _LCD_CAMERA_USE_SAME_SPI_
  close_spi_for_sensor();
  open_spi_for_lcd();
  #endif

}



VOID power_off_pas6175()
{
  camerap_PowerOff();
}
VOID powre_on_pas6175()
{      
    camerap_PowerOn(CAM_NPIX_QVGA, 128, 160, CAM_FORMAT_RGB565);  
}
VOID check_sensorID()
{      

  camerap_CheckProductId();
}

UINT16 ttcount = 0;UINT16 ttcount333 = 0;
VOID test_read_serial_camer_240320()
{
  #if 0
   // for(ttcount333=0;ttcount333<128*160;ttcount333++) display_sensor2[ttcount333] = 0xf8;
    PAS_Cam_ReadFrame(display_sensor240320,240*320);

   
    hal_HstSendEvent(0x654320);
  #endif
}

VOID test_read_serial_camer()
{
  #if 0
   // for(ttcount333=0;ttcount333<128*160;ttcount333++) display_sensor2[ttcount333] = 0xf8;
    PAS_Cam_ReadFrame(display_sensor2,128*160);

   
    hal_HstSendEvent(0x654321);
    hal_HstSendEvent((UINT32)display_sensor2);

    
    ttcount++;
    if(ttcount ==15)
    {
      hal_HstSendEvent(0x654321);
   // PAS6175_Set_Para_Effect(CAM_EFFECT_ENC_SEPIAGREEN);
    }
    //camerap_PowerOff();SXS_TRACE(TSTDOUT,"DRV_CAM: power down") ;
    #endif
}
UINT16 tttt=0;
VOID test_read_serial_camertest()
{
  #if 0
  UINT8 Reg_vlaue = 0;
    tttt++;
        if(tttt  == 1)
    {  //  hal_HstSendEvent(0x300006);  

      camerap_WriteOneReg(0xef,0x0);
      camerap_WriteOneReg(0x0c,0x3);
    }

    if(tttt % 10  == 1)
    {  //  hal_HstSendEvent(0x300006);  

      camerap_WriteOneReg(0xef,0x0);
      camerap_WriteOneReg(0x0c,0x3);
    }
    if(tttt % 10  == 4)
    { //   hal_HstSendEvent(0x300005);  

      camerap_WriteOneReg(0xef,0x0);
      camerap_WriteOneReg(0x0c,0x6);
    }

	camerap_WriteOneReg(0xef,0x02);
	Reg_vlaue = camerap_ReadReg(0xa6);
   SXS_TRACE(TSTDOUT, "sensor  0x%x ms ",Reg_vlaue);


    PAS_Cam_ReadFrame(NULL,128*160);


    
    //camerap_PowerOff();SXS_TRACE(TSTDOUT,"DRV_CAM: power down") ;

    #endif
}
PUBLIC BOOL camera_pas6175_RegInit(CAM_DRV_T  * pCamDrv)
{
  
 
	if(camerap_CheckProductId())
	{
    // camerap_PowerOn(CAM_NPIX_QVGA, 128, 160, CAM_FORMAT_RGB565);  
    // camerap_Delay(20);
    //  test_read_serial_camer_INT();
    //   return FALSE; 	
		pCamDrv->camera_PowerOn=camerap_PowerOn;
		pCamDrv->camera_PowerOff=camerap_PowerOff;
		pCamDrv->camera_Reserve=camerap_Reserve;
	 	pCamDrv->camera_Release=camerap_Release;
		pCamDrv->camera_PrepareImageCaptureL=camerap_PrepareImageCaptureL;
		pCamDrv->camera_CaptureImage=camerap_CaptureImage;
		pCamDrv->camera_SetZoomFactorL=camerap_SetZoomFactorL;
		pCamDrv->camera_ZoomFactor=camerap_ZoomFactor;
	    pCamDrv->camera_SetDigitalZoomFactorL=camerap_SetDigitalZoomFactorL;
		pCamDrv->camera_DigitalZoomFactor=camerap_DigitalZoomFactor;
	    pCamDrv->camera_SetContrastL=camerap_SetContrastL;
	 	pCamDrv->camera_Contrast=camerap_Contrast;
	 	pCamDrv->camera_SetBrightnessL=camerap_SetBrightnessL;
	 	pCamDrv->camera_Brightness=camerap_Brightness;
		pCamDrv->camera_SetFlashL=camerap_SetFlashL;
		pCamDrv->camera_Flash=camerap_Flash;
	 	pCamDrv->camera_SetExposureL=camerap_SetExposureL;
		pCamDrv->camera_Exposure=camerap_Exposure;
	 	pCamDrv->camera_SetWhiteBalanceL=camerap_SetWhiteBalanceL;
	 	pCamDrv->camera_WhiteBalance=camerap_WhiteBalance;
	 	pCamDrv->camera_SetSpecialEffect=camerap_SetSpecialEffect;
		pCamDrv->camera_SpecialEffect=camerap_SpecialEffect;
	 	pCamDrv->camera_SetNightMode=camerap_SetNightMode;
		pCamDrv->camera_NightMode=camerap_NightMode;
		pCamDrv->camera_StartViewFinderDirectL=camerap_StartViewFinderDirectL;
		pCamDrv->camera_StopViewFinder=camerap_StopViewFinder;
		pCamDrv->camera_ViewFinderActive=camerap_ViewFinderActive;

		pCamDrv->camera_SetViewFinderMirror=camerap_SetViewFinderMirror;
		pCamDrv->camera_ViewFinderMirror=camerap_ViewFinderMirror;
	 	pCamDrv->camera_CancelCaptureImage=camerap_CancelCaptureImage;
		pCamDrv->camera_EnumerateCaptureSizes=camerap_EnumerateCaptureSizes;
		pCamDrv->camera_PrepareVideoCaptureL=camerap_PrepareVideoCaptureL;
		pCamDrv->camera_StartVideoCapture=camerap_StartVideoCapture;
		pCamDrv->camera_StopVideoCapture=camerap_StopVideoCapture;
	 	pCamDrv->camera_VideoCaptureActive=camerap_VideoCaptureActive;
		pCamDrv->camera_EnumerateVideoFrameSizes=camerap_EnumerateVideoFrameSizes;
		pCamDrv->camera_EnumerateVideoFrameRates=camerap_EnumerateVideoFrameRates;
	 	pCamDrv->camera_GetFrameSize=camerap_GetFrameSize;
		pCamDrv->camera_SetFrameRate=camerap_SetFrameRate;
		pCamDrv->camera_FrameRate=camerap_FrameRate;
	 	pCamDrv->camera_BuffersInUse=camerap_BuffersInUse;
		pCamDrv->camera_FramesPerBuffer=camerap_FramesPerBuffer;
		pCamDrv->camera_GetId=camerap_GetId;
		return TRUE;
	}

	return FALSE;
}
VOID buffer_ready(UINT16 * buffer)
{

}

VOID testPAS6175_Capture()
{
//PAS6175_Capture(128,160,16,150);

}
VOID test_bar()
{
#if  0
  camerap_WriteOneReg(0xef,0x0);
  camerap_WriteOneReg(0x0c,0x00);
  
  camerap_WriteOneReg(0xed,0x01);	

  //test color 
#endif

}

#if 0
void PAS6167_IO_TriState(kal_bool TriState_Enable)
{
	kal_uint8 Reg_vlaue = 0;
	write_cmos_sensor(0xef,0x01);
	Reg_vlaue = read_cmos_sensor(0x22);
	if(TriState_Enable == KAL_TRUE)
		Reg_vlaue = 0x04;
	else
		Reg_vlaue = 0x00;
	write_cmos_sensor(0x22,Reg_vlaue);	
}
#endif


VOID pas_start()
{
  #if 0
 if(time_mmc == 0)
 {  
     hal_HstSendEvent(0x444444); time_mmc++;
    // PAS_Cam_ReadFrame_continue(display_sensor2,128*160*2);
  PAS_Cam_ReadFrame_continue_display(display_sensor2,128*160*2);
     //PAS_Cam_ReadFrame(display_sensor2,128*160);
    
 }
 else
   hal_HstSendEvent(0x55555);
#endif

}

UINT8 spi_clk = 0;
extern VOID tse_II2c_ldo();

VOID tset_spi_clk()
{
  #if 0
  spi_clk++;
 // if(spi_clk==1)
  {
  hal_GpioClr(g_csb_gpo);
  hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CS0, HAL_SPI_PIN_STATE_0);
  hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_0);
  tse_II2c_ldo();
  }

 #endif
}
extern VOID change_II2c_voltage(BOOL open);

VOID PAS_Enable_power_voltage(BOOL open)
{
     #ifdef _LCD_CAMERA_USE_SAME_SPI_
     hal_GpioClr(g_csb_gpo);
     if(open == FALSE)
     {
         hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CS1, HAL_SPI_PIN_STATE_0);
         //hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_0);
     }
     else
     {
         hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CS1, HAL_SPI_PIN_STATE_SPI);
         //hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_SPI);
     }
     change_II2c_voltage(open);
     pmd_EnablePower(PMD_POWER_CAMERA,open); 
     #else
     hal_GpioClr(g_csb_gpo);
     if(open == FALSE)
     {
         hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CS0, HAL_SPI_PIN_STATE_0);
         hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_0);
     }
     else
     {
         hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CS0, HAL_SPI_PIN_STATE_SPI);
         hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_SPI);
     }
     change_II2c_voltage(open);
     pmd_EnablePower(PMD_POWER_CAMERA,open); 

     #endif

}

VOID PAS_Enable_poweron_voltage( )
{
    #ifdef _LCD_CAMERA_USE_SAME_SPI_

    pmd_EnablePower(PMD_POWER_CAMERA,TRUE); 
    camerap_Delay(5); 
    hal_GpioSet(g_csb_gpo);
    camerap_Delay(5); 
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CS1, HAL_SPI_PIN_STATE_SPI);
    //hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_SPI);
    //hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_DO, HAL_SPI_PIN_STATE_SPI);
    change_II2c_voltage(TRUE);
    hal_GpioClr(g_csb_gpo);
    camerap_Delay(20); 
    #else
    pmd_EnablePower(PMD_POWER_CAMERA,TRUE); 
    camerap_Delay(5); 
    hal_GpioSet(g_csb_gpo);
    camerap_Delay(5); 
    hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CS0, HAL_SPI_PIN_STATE_SPI);
    hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_SPI);
    hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_DO, HAL_SPI_PIN_STATE_SPI);
    change_II2c_voltage(TRUE);
    hal_GpioClr(g_csb_gpo);
    camerap_Delay(20); 

    #endif

}

VOID PAS_Enable_powerdown_voltage( )
{
    #ifdef _LCD_CAMERA_USE_SAME_SPI_

    hal_GpioClr(g_csb_gpo);
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CS1, HAL_SPI_PIN_STATE_0);
    //hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_0);
    //hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_DO, HAL_SPI_PIN_STATE_0);

    change_II2c_voltage(FALSE);
    pmd_EnablePower(PMD_POWER_CAMERA,FALSE); 
    #else
    hal_GpioClr(g_csb_gpo);
    hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CS0, HAL_SPI_PIN_STATE_0);
    hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_0);
    hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_DO, HAL_SPI_PIN_STATE_0);

    change_II2c_voltage(FALSE);
    pmd_EnablePower(PMD_POWER_CAMERA,FALSE); 

    #endif

}

VOID PAS_Enter_LowPower(BOOL tri)
{
    #ifdef _LCD_CAMERA_USE_SAME_SPI_

    if(tri == TRUE)
    {
        hal_GpioClr(g_csb_gpo);
        hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CS1, HAL_SPI_PIN_STATE_Z);
        //hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_Z);
        //hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_DO, HAL_SPI_PIN_STATE_Z);
        change_II2c_voltage(FALSE);
        pmd_EnablePower(PMD_POWER_CAMERA,FALSE); 
    }
    else
    {
        hal_GpioSet(g_csb_gpo);
        hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CS1, HAL_SPI_PIN_STATE_SPI);
        //hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_Z);
        //hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_DO, HAL_SPI_PIN_STATE_Z);
        pmd_EnablePower(PMD_POWER_CAMERA,TRUE); 
    }
    #else
    if(tri == TRUE)
    {
        hal_GpioClr(g_csb_gpo);
        hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CS0, HAL_SPI_PIN_STATE_Z);
        hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_Z);
        hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_DO, HAL_SPI_PIN_STATE_Z);
        change_II2c_voltage(FALSE);
        pmd_EnablePower(PMD_POWER_CAMERA,FALSE); 
    }
    else
    {
        hal_GpioSet(g_csb_gpo);
        hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CS0, HAL_SPI_PIN_STATE_SPI);
        hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_SPI);
        hal_SpiForcePin(HAL_SPI_2,HAL_SPI_PIN_DO, HAL_SPI_PIN_STATE_SPI);
        change_II2c_voltage(TRUE);
        pmd_EnablePower(PMD_POWER_CAMERA,TRUE); 
    }

    #endif

}

VOID camerap_PAS_Enter_LowPower(BOOL tri)
{
    PAS_Enter_LowPower(tri);
}


VOID pas6175_preview(UINT16 Width, UINT16 Height)
{
    camerap_StartPreview(CAM_NPIX_QVGA,Width,Height);
}
VOID camerap_Capture_After_Preview(UINT16 Width, UINT16 Height)
{
    camerap_pas6175_Capture_After_Preview(CAM_NPIX_QVGA,Width,Height);
   #if 0
    PAS6175_ReadShutterOfPreview();
    PAS6175_Pause();
    Lcd_SetIO_TriState(TRUE);
    PAS6175_Resume();
   // camerap_StartCapture(CAM_NPIX_QVGA,240,320);
    camerap_StartCapture(CAM_NPIX_QVGA,Width,Height);
   #endif

}

//----------------------------- following  test ----------------------------------------------//
VOID test_pas6175_preview(UINT16 Width, UINT16 Height)
{
    camerap_StartPreview(CAM_NPIX_QVGA,Width,Height);
}
VOID test_pas6175_ReadFrame(UINT16 * buff,UINT16 Width, UINT16 Height)
{
  camerap_GetImageFrame(buff,Width*Height);
  #if 0
 // camerap_GetOneFrame(display_sensor240320,240*320);
  camerap_GetOneFrame(display_sensor2,128*160);
  PAS6175_Pause();
  Lcd_SetIO_TriState(FALSE);
  #endif
}



VOID test_pas6175_Capture_After_Preview(UINT16 Width, UINT16 Height)
{
    camerap_pas6175_Capture_After_Preview(CAM_NPIX_QVGA,Width,Height);
   #if 0
    PAS6175_ReadShutterOfPreview();
    PAS6175_Pause();
    Lcd_SetIO_TriState(TRUE);
    PAS6175_Resume();
   // camerap_StartCapture(CAM_NPIX_QVGA,240,320);
    camerap_StartCapture(CAM_NPIX_QVGA,Width,Height);
   #endif

}

#if 0
VOID test_pas6175_Preview_After_Capture()
{
    #if 0
    hal_GpioSet(g_csb_gpo);
    Lcd_SetIO_TriState(TRUE);
    lcddp_SensorBB_SPI_SetIO_TriState(TRUE);
    hal_GpioClr(g_csb_gpo);
    camerap_StartPreview(CAM_NPIX_QVGA,128,160);
    #endif
    camerap_StartPreview(CAM_NPIX_QVGA,128,160);
}

VOID test_pas6175_Capture()
{
    Lcd_SetIO_TriState(TRUE);
    PAS6175_Resume();
    camerap_PowerOn(CAM_NPIX_QVGA, 128, 160, CAM_FORMAT_RGB565);  
    camerap_StartCapture(CAM_NPIX_QVGA,240,320);

}
VOID test_pas6175_ReadFrame_next()
{
  Lcd_SetIO_TriState(TRUE);
  PAS6175_Resume();
  camerap_GetOneFrame(display_sensor240320,240*320);
  PAS6175_Pause();
  Lcd_SetIO_TriState(FALSE);
}
#endif


#if 0
extern UINT16* ggggimgBuf ;
UINT8 dddddd = 0;
VOID Pas_handler()
{
     
      if (dddddd++ >5)
      {
         hal_HstSendEvent(0x670002);
      camerap_GetImageFrame(ggggimgBuf,240*320);
      }

  
}
VOID PAS_FrameHeadDetect11(VOID)
{
  
    hal_HstSendEvent(0x670001);
   
    HAL_GPIO_CFG_T overflow_Cfg;
    overflow_Cfg.direction = HAL_GPIO_DIRECTION_INPUT;
    overflow_Cfg.irqHandler = Pas_handler;
    overflow_Cfg.irqMask.rising=FALSE;
    overflow_Cfg.irqMask.falling = TRUE;
    overflow_Cfg.irqMask.debounce = TRUE;
    overflow_Cfg.irqMask.level=FALSE;
    hal_GpioOpen(g_sensor_overflow.gpioId,&overflow_Cfg);
}
#endif

