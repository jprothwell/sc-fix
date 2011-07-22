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


//#define  SWAP_SPI_SENSOR_BYTE
#define LCD_SPI_SENSOR_TIMEING_TYPE_3




#define SENSOR_TEST_CODE

BOOL for_serial_sensor = FALSE;


#define CAM_sp0828_ID      0x0c
#ifdef SENSOR_TEST_CODE
UINT16 display_sensor240320[240*320]={0};
#endif
UINT16 display_sensor2[128*160*4]={0};
UINT16 display_sensor3[128*160*2]={0};
PRIVATE VOLATILE UINT16 g_sensor_count = 0;
PRIVATE UINT8 SP0828_banding = 0;// 0 for 50hz ,1 for 60hz
SP0828_Cam_Vid_enum SP0828_State = CAMERA;

//#define _LCD_CAMERA_USE_SAME_SPI_  //define in def.file
// SP0828_Preview
#ifdef _LCD_CAMERA_USE_SAME_SPI_
PRIVATE HAL_SPI_ID_T g_spi_id_sensor = HAL_SPI; 
PRIVATE HAL_SPI_CS_T g_spi_cs_sensor = HAL_SPI_CS1;
#else
//PRIVATE HAL_SPI_ID_T g_spi_id_sensor = HAL_SPI_2;
//PRIVATE HAL_SPI_CS_T g_spi_cs_sensor = HAL_SPI_CS0;
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

#if 0  // x95
//##
//CSB
HAL_APO_ID_T g_csb_gpo = {{ 
                          .type = HAL_GPIO_TYPE_O,
                          .id = 5 //x95 = gp0 5; r410 = gp0  4
                        }};
//overflow_m
HAL_APO_ID_T g_sensor_ready = {{ 
                            .type = HAL_GPIO_TYPE_IO,
                            .id = 3
                        }};
//overflow_s
HAL_APO_ID_T g_sensor_overflow = {{ 
                            .type = HAL_GPIO_TYPE_IO,
                            .id = 2
                        }};

HAL_APO_ID_T g_camera_FLASH = {{ 
                          .type = HAL_GPIO_TYPE_O,
                          .id = 0
                        }};

 #else  // r410
HAL_APO_ID_T g_csb_gpo = {{ 
                          .type = HAL_GPIO_TYPE_O,
                          .id = 4 //x95 = gp0 5; r410 = gp0  4
                        }};
//overflow_m
HAL_APO_ID_T g_sensor_ready = {{ 
                            .type = HAL_GPIO_TYPE_IO,
                            .id = 2
                        }};
//overflow_s
HAL_APO_ID_T g_sensor_overflow = {{ 
                            .type = HAL_GPIO_TYPE_IO,
                            .id = 3
                        }};

HAL_APO_ID_T g_camera_FLASH = {{ 
                          .type = HAL_GPIO_TYPE_O,
                          .id = 0
                        }};

                        #endif


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
void SP0828_Preview(UINT16 image_target_width, UINT16 image_target_height,UINT8 clk_NP,UINT32 HBlank);
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
    hal_SpiDeActivateCs(HAL_SPI,HAL_SPI_CS0);  //temp
    hal_SpiClose(HAL_SPI,HAL_SPI_CS0);
    lcddp_SensorBB_SPI_SetIO_TriState(TRUE);
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
        .clkFallEdge    = TRUE,
        .clkDelay       = HAL_SPI_HALF_CLK_PERIOD_1,//HAL_SPI_HALF_CLK_PERIOD_1
        .doDelay        = HAL_SPI_HALF_CLK_PERIOD_0,
        .diDelay        = HAL_SPI_HALF_CLK_PERIOD_1,//HAL_SPI_HALF_CLK_PERIOD_1,
        .csDelay        = HAL_SPI_HALF_CLK_PERIOD_0,
        .csPulse        = HAL_SPI_HALF_CLK_PERIOD_0,//HAL_SPI_HALF_CLK_PERIOD_1
        .frameSize      = 8,
        .oeRatio        = 0xff,
        .spiFreq        = 8000000,//5M 5000000 // 8M
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
    hal_HstSendEvent(0xaaaabbbb);
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

  // g_csb_gpo = tgt_GetCamdConfig()->spiCamPinCSB;
  //  g_sensor_ready = tgt_GetCamdConfig()->spiCamPinOverflowM;
   // g_sensor_overflow = tgt_GetCamdConfig()->spiCamPinOverflowS;    
    
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

   if (CamOutSize == CAM_NPIX_QVGA)
    {
        camOutWidth = 240;
        camOutHeight = 320;
    }
    else if (CamOutSize == CAM_NPIX_QQVGA)
    {
        camOutWidth = 128;
        camOutHeight = 160;    
        hal_HstSendEvent(0x8844002); 
    }
    else
    {
        hal_DbgAssert("don't support the Resolution");
        return;
    }
    SXS_TRACE(TSTDOUT,"DRV_CAM: camerap_PowerOn IspOutWidth  %d IspOutHeight %d",IspOutWidth, IspOutHeight);    
//    

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
    
            break;
        case CAM_WB_DAYLIGHT:   /*晴天太阳光*/
            break;
        case CAM_WB_OFFICE:   /*日光灯*/
            break;
        case CAM_WB_CLOUDY:  /*阴天*/ // pas6175

            break;		
        case CAM_WB_TUNGSTEN:  /*钨丝灯*/
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

            break;
        case CAM_SPE_ANTIQUE	:	
            break;
        case CAM_SPE_REDISH	:
            break;
        case CAM_SPE_GREENISH:	
            break;
        case CAM_SPE_BLUEISH	:	
            break;
        case CAM_SPE_BLACKWHITE	:
            break;
        case CAM_SPE_NEGATIVE:	
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
       hal_HstSendEvent(0x123);
       SP0828_Preview(Width,Height,8,60);
    }
    else if((Width == 240)&&(Height == 320))
    {
       hal_DbgAssert("don't support the Resolution");
       SP0828_Preview(Width,Height,32,800);
    }
    for_serial_sensor = TRUE;
}
UINT32 camerap_GetOneFrame(UINT16 * buff,UINT32 size)
{
    if(size == 128*160)
    {
        hal_HstSendEvent(0xaabb);
        return PAS_Cam_ReadFrame(buff, 128*160);
    }
    if(size == 240*320)
    {
    
        hal_HstSendEvent(0xaabbcc);
        return PAS_Cam_ReadFrame(buff, 240*320);
    }
    else
    {
        hal_DbgAssert("don't support the size");
        return 1;
    }
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
   hal_HstSendEvent(0x811944);hal_HstSendEvent(pas_id);
   if(pas_id == 0xff)
   {

    CAM_REG_T id_reg={0xef,0,1};
    UINT8 id = 0xff,i=0;
    hal_TimDelay(10); 
   // camerap_WriteReg(id_reg);
    hal_HstSendEvent(0x322002);  

    for(i=0; i<10; i++)
    {
       id = camerap_ReadReg(0x02); hal_HstSendEvent((UINT32)id);  
      if(id == CAM_sp0828_ID)
      {
      break;
      }
    }
    hal_HstSendEvent(0x322003);  
    hal_HstSendEvent((UINT32)id);  
    pas_id = CAM_sp0828_ID;
   }
    return CAM_sp0828_ID ;
 
}

PRIVATE VOID camerap_Delay(UINT16 Wait_mS)
{
    UINT32 Ticks_16K;

    // Here is the delay function used by the camera driver.  It can be changed to use a non-blocking
    // wait if necessary, but for now, just use hal_TimDelay
    Ticks_16K = (16348 * Wait_mS)/1000;
    hal_TimDelay(Ticks_16K);
}
PRIVATE VOID camerap_Delay_get(UINT16 tick)
{

    hal_TimDelay(tick);
}
PRIVATE VOID CAMD_POSSIBLY_UNUSED camerap_DbgSendSequence(UINT8 SeqNum)
{
  return;
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
   if(  pas_gpio_i2c_write_data_sensor(0x18,(UINT8 *)&RegWrite.Addr,1,&RegWrite.Data,1) == FALSE)
   {
        if(  pas_gpio_i2c_write_data_sensor(0x18,(UINT8 *)&RegWrite.Addr,1,&RegWrite.Data,1) == FALSE)
        {
             if(  pas_gpio_i2c_write_data_sensor(0x18,(UINT8 *)&RegWrite.Addr,1,&RegWrite.Data,1) == FALSE) hal_DbgAssert("no ack");
        }
   }

}

PRIVATE VOID  camerap_WriteOneReg( UINT8 Addr, UINT8 Data)
{
   if( pas_gpio_i2c_write_data_sensor(0x18,&Addr,1,&Data,1) == FALSE)
   {
       if( pas_gpio_i2c_write_data_sensor(0x18,&Addr,1,&Data,1) == FALSE)
       {
             if( pas_gpio_i2c_write_data_sensor(0x18,&Addr,1,&Data,1) == FALSE) 
              {

                if(s_camrst == FALSE)  hal_DbgAssert("no ack");
             }

       }

   }

}


PRIVATE UINT8 camerap_ReadReg(UINT8 Addr)
{
    UINT8 data = 0;
    pas_gpio_i2c_read_data_sensor(0x18,&Addr,1,&data,1);
    return data;

}


PRIVATE VOID camerap_InteruptHandler(HAL_CAMERA_IRQ_CAUSE_T cause)
{
}

VOID camerap_PAS6175_Pause()
{
   
}
VOID camerap_PAS6175_Resume()
{
   
}

VOID PAS6175_Pause(VOID)
{
}
VOID PAS6175_Resume(VOID)
{
    
}

//交换高低8位的128*160的buffer至少需要5ms
BOOL pas_pown_on = FALSE;
UINT8 pas_count = 0;

VOID PAS_Enable_power_voltage(BOOL open);
PRIVATE BOOL camerap_CheckProductId()
{
    if(pas_id == 0xff)
    {       
        pas_gpio_i2c_init_sensor();
        PAS_Enable_powerdown_voltage();  
        camerap_Delay(100);
        PAS_Enable_poweron_voltage();
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
        camerap_GetId();
 #ifdef TEMP_C006S_SPICAM_CFG_MACRO
       hal_SysClkOutClose(g_SerialClockOutId);
       g_SerialClockOutId = HAL_SYS_CLOCK_OUT_ID_QTY;
 #else
       if(g_SerialClockOut == TRUE)
        {
            g_SerialClockOut = camerap_CloseClk();       
        }
        hal_GpioSet(g_csb_gpo);
        hal_GpioClr(g_sensor_ready);
        
 #endif

    }
        return TRUE;

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
{}


void PAS6175_Set_H_Blank(UINT32 HBlank)
{}

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
{}

VOID PAS6175_AWB_Mode(BOOL AWB_Enable)
{
}

void PAS6175_AE_Mode(BOOL AE_Enable)
{

  
}

VOID PAS6175_Skip_Frame(UINT8 SkipFrameNumber)
{}


void PAS6175_SPI_Slave_Mode_Enable(void)
{
}

void PAS6175_SPI_Master_Mode_Enable(void)
{

}


VOID PAS6167_Output_Enable(BOOL Output_Enable)
{ 
}


void PAS6175_Output_Enable(BOOL Output_Enable)
{ 
}

void PAS6175_IO_TriState(BOOL TriState_Enable)
{
}

UINT32 PAS6175_Set_Para_WB(UINT32 para)
{}


UINT32 PAS6175_Set_Para_Effect(UINT32 iPara)
{}

void PAS6175_NightMode(BOOL bEnable)
{}



UINT32 PAS6175_Set_Para_EV(UINT32 iPara)
{
return 1;
}



BOOL g_bBanding50Hz = TRUE;

UINT32 PAS6175_Set_Para_Banding(UINT32 iPara)
{	

	return TRUE;
}
VOID PAS_FrameHeadDetect(VOID);
VOID PAS_FrameStopDetect();



//UINT16 display_sensor[128*160]={0};
//UINT16 display_sensor2[128*160]={0};
VOID test_reg_all()
{

}

#define pas_buffer_len 32*2*4   // u8 type

UINT16 display_sensor_temp[128]={0};

VOID PAS_SPI_GetDateDma(VOID)
{
}
  UINT32 pas_now1  ;
  UINT32 pas_now2  ; 
 BOOL pasflag=TRUE;
 UINT32 delay_frame = 0;

VOID serial_camer_dma_int(HAL_SPI_IRQ_STATUS_T spi_irq)
{
}



VOID PAS_Frame_ready_int(VOID)
{ 
}


VOID PAS_FundFrameHeader(VOID)
{
}


VOID PAS_FrameHeadDetect(VOID)
{
}

VOID PAS_FrameStopDetect()
{

}

UINT8 test_color = 5;

SP0828_Size_enum SP0828_Capture_Size,SP0828_Preview_Size;
UINT16 sp0828_preview_startx = 0;
UINT16 sp0828_preview_starty = 0;
UINT16 sp0828_preview_endx = 0;
UINT16 sp0828_preview_endy = 0;

void SP0828_Data_Output_Disable(void)
{
	camerap_WriteOneReg(0x1c,0xff);
	camerap_WriteOneReg(0xe7,0x03);
	camerap_WriteOneReg(0xe7,0x00);
	
}

void SP0828_Slave_Data_Output_Enable(void)
{
	camerap_WriteOneReg(0x1c,0x08);
	camerap_WriteOneReg(0xe7,0x03);
	camerap_WriteOneReg(0xe7,0x00);
	
}
void SP0828_Master_Data_Output_Enable(void)
{
	camerap_WriteOneReg(0x1c,0x00);
	camerap_WriteOneReg(0xe7,0x03);
	camerap_WriteOneReg(0xe7,0x00);
	
}


void SP0828_Output_Disable(void)
{
	hal_HstSendEvent(0x08288012);
#if 0//remove for test
	camerap_WriteOneReg(0x1c,0xff);
	camerap_WriteOneReg(0xe7,0x03);
	camerap_WriteOneReg(0xe7,0x00);
#endif
	hal_GpioSetOut(g_sensor_ready.gpioId);
	hal_GpioClr(g_sensor_ready);
	
}
void SP0828_IO_TriState(BOOL TriState_Enable)
{
	if(TriState_Enable == TRUE)
		hal_GpioSet(g_csb_gpo);
	else
		hal_GpioClr(g_csb_gpo);
}
void SP0828_Master_Init(void)
{
	UINT32 i,t0,t1,reg_count;
	unsigned char *SP0828_CONFIG_TYPE;
	unsigned char *val;
  

	hal_HstSendEvent(0x08288007);
	if(SP0828_Preview_Size == Size_60_80)
	{
		SP0828_CONFIG_TYPE = SP0828_MASTER_CONFIG_60_80;
		sp0828_preview_startx = SP0828_CONFIG_TYPE[0];
		sp0828_preview_starty = SP0828_CONFIG_TYPE[1];
		sp0828_preview_endx = sp0828_preview_startx + 59;
		sp0828_preview_endy = sp0828_preview_starty + 79;
		reg_count = M_Reg_Count_60_80/2;
		
	}
	else if(SP0828_Preview_Size == Size_128_96)
	{
		SP0828_CONFIG_TYPE = SP0828_MASTER_CONFIG_128_96;
		sp0828_preview_startx = SP0828_CONFIG_TYPE[0];
		sp0828_preview_starty = SP0828_CONFIG_TYPE[1];
		sp0828_preview_endx = sp0828_preview_startx + 127;
		sp0828_preview_endy = sp0828_preview_starty + 95;
		reg_count = M_Reg_Count_128_96/2;
		
	}
	else if(SP0828_Preview_Size == Size_128_104)
	{
		SP0828_CONFIG_TYPE = SP0828_MASTER_CONFIG_128_104;
		sp0828_preview_startx = SP0828_CONFIG_TYPE[0];
		sp0828_preview_starty = SP0828_CONFIG_TYPE[1];
		sp0828_preview_endx = sp0828_preview_startx + 127;
		sp0828_preview_endy = sp0828_preview_starty + 103;
		reg_count = M_Reg_Count_128_104/2;
		
	}
		
	else if(SP0828_Preview_Size == Size_128_160)
	{
		hal_HstSendEvent(0x08288008);
		SP0828_CONFIG_TYPE = SP0828_MASTER_CONFIG_128_160;
		sp0828_preview_startx = SP0828_CONFIG_TYPE[0];
		sp0828_preview_starty = SP0828_CONFIG_TYPE[1];
		sp0828_preview_endx = sp0828_preview_startx + 127;
		sp0828_preview_endy = sp0828_preview_starty + 159;
		
		reg_count = M_Reg_Count_128_160/2;
		
	}
	else if(SP0828_Preview_Size == Size_144_176)
	{
		SP0828_CONFIG_TYPE = SP0828_MASTER_CONFIG_144_176;
		sp0828_preview_startx = SP0828_CONFIG_TYPE[0];
		sp0828_preview_starty = SP0828_CONFIG_TYPE[1];
		sp0828_preview_endx = sp0828_preview_startx + 143;
		sp0828_preview_endy = sp0828_preview_starty + 175;
		reg_count = M_Reg_Count_144_176/2;
		
	}
	else if(SP0828_Preview_Size == Size_176_220)
	{
		SP0828_CONFIG_TYPE = SP0828_MASTER_CONFIG_176_220;
		sp0828_preview_startx = SP0828_CONFIG_TYPE[0];
		sp0828_preview_starty = SP0828_CONFIG_TYPE[1];
		sp0828_preview_endx = sp0828_preview_startx + 175;
		sp0828_preview_endy = sp0828_preview_starty + 219;
		reg_count = M_Reg_Count_176_220 / 2;
	}
	else if(SP0828_Preview_Size == Size_240_320)
	{
		SP0828_CONFIG_TYPE = SP0828_MASTER_CONFIG_240_320;
		sp0828_preview_startx = SP0828_CONFIG_TYPE[0];
		sp0828_preview_starty = SP0828_CONFIG_TYPE[1];
		sp0828_preview_endx = sp0828_preview_startx + 239;
		sp0828_preview_endy = sp0828_preview_starty + 319;
		reg_count = M_Reg_Count_240_320/2;
	}
//	dbg_print("num=%d,SP0828_Preview_Size=%d,count=%d\r\n",M_Reg_Count_128_160,SP0828_Preview_Size,reg_count);
//	t0 = drv_get_current_time(); 
		
	for (i = 1;i <= (SP0828_Reg_Count/2) -1;i++)
		camerap_WriteOneReg(SP0828_CONFIG[i*2],SP0828_CONFIG[i*2+1] );
		
	for (i = 1;i <= reg_count;i++)
		camerap_WriteOneReg(SP0828_CONFIG_TYPE[i*2],SP0828_CONFIG_TYPE[i*2+1] );
//	sp0828_i2c_time = drv_get_duration_ms(t0);
//	dbg_print("I2C write cost %d ms\r\n",sp0828_i2c_time);

    SP0828_Lcd_Clock_Set();
//	SP0828_Working_Type = PREVIEW;	
		
}

SP0828_Open_Auto_Exp(void)
{
	UINT8 tmp;
	camerap_WriteOneReg(0xfd,0x00);
	tmp = camerap_ReadReg(0X32);
	camerap_WriteOneReg(0x32,(tmp)|0x05);
	camerap_WriteOneReg(0xe7,0x03);
	camerap_WriteOneReg(0xe7,0x00);
}

typedef enum {
     Lcd_Type0,     /*0 0x40=0x08,0x41=0x00*/  
     Lcd_Type1,  /*1 0x40=0x08,0x41=0x80*/
     Lcd_Type2,     /*2 0x40=0x09,0x41=0x00*/
     Lcd_Type3  /*3 0x40=0x09,0x41=0x80*/ 
     
} SP0828_Lcd_Type;


unsigned char SP0828_Register_0x40_Value = 0x08;
unsigned char SP0828_Register_0x41_Value = 0x80;

SP0828_Lcd_Type SP0828_Lcd_Select;

void SP0828_Lcd_Clock_Set(void)
{
	UINT8 val;
	//TODO: 按照不同LCD的时序要求设置0x40和0x41值的组合类型共四种;
	
   /***if( lcdID == LCM_TRULY_S6B33BF)//根据屏的ID来选择，此处也可根据宏定义选择***/
	#ifdef LCD_SPI_SENSOR_TIMEING_TYPE_0
	SP0828_Lcd_Select = Lcd_Type0;
	#endif
    #ifdef LCD_SPI_SENSOR_TIMEING_TYPE_1
	SP0828_Lcd_Select = Lcd_Type1;
	#endif
	#ifdef LCD_SPI_SENSOR_TIMEING_TYPE_2
	SP0828_Lcd_Select = Lcd_Type2;
	#endif
	#ifdef LCD_SPI_SENSOR_TIMEING_TYPE_3
	SP0828_Lcd_Select = Lcd_Type3;
	#endif

    //SP0828_Lcd_Select = Lcd_Type3;
	
//TODO END

	switch(SP0828_Lcd_Select)
	{
	case Lcd_Type0:
		SP0828_Register_0x40_Value = 0x08;
		SP0828_Register_0x41_Value = 0x00;
		break;
		
	case Lcd_Type1:
		SP0828_Register_0x40_Value = 0x08;
		SP0828_Register_0x41_Value = 0x80;
		break;
		
	case Lcd_Type2:
		SP0828_Register_0x40_Value = 0x09;
		SP0828_Register_0x41_Value = 0x00;
		break;
		
	case Lcd_Type3:
		SP0828_Register_0x40_Value = 0x09;
		SP0828_Register_0x41_Value = 0x80;
		break;
		
	 default:
	 	SP0828_Register_0x40_Value = 0x08;
		SP0828_Register_0x41_Value = 0x80;
		break;
		
	}

	camerap_WriteOneReg(0x40,SP0828_Register_0x40_Value);
	camerap_WriteOneReg(0x41,SP0828_Register_0x41_Value);
	
	val = camerap_ReadReg(0x40);
	   SXS_TRACE(TSTDOUT, "0x40 = 0x%x ",val);

	val = camerap_ReadReg(0x41);
      SXS_TRACE(TSTDOUT, "0x41 = 0x%x ",val);


}

UINT32 SP0828_set_param_wb(UINT32 para)
{ 
	UINT8 tmp;
	camerap_WriteOneReg(0xfd,0x00);
    tmp = 	camerap_ReadReg(0x32);
	switch (para)
	{
		case CAM_WB_AUTO:   // enable AWB 
			//SP0828_reg_WB_auto         自动
			camerap_WriteOneReg(0xfd,0x01);                                                          
			camerap_WriteOneReg(0x28,0x61);		                                                       
			camerap_WriteOneReg(0x29,0x49);
			camerap_WriteOneReg(0xfd,0x00);  // AUTO 3000K~7000K   		
			camerap_WriteOneReg(0x32,tmp&0xbf|0x10);                                                                                                                                                                      
		break;

		case CAM_WB_CLOUD: 
			// SP0828_reg_WB_auto   阴天
			camerap_WriteOneReg(0xfd,0x00);   //7000K    
			camerap_WriteOneReg(0x32,tmp&0xef);                                                          
			camerap_WriteOneReg(0xfd,0x01);                                                          
			camerap_WriteOneReg(0x28,0x71);		                                                       
			camerap_WriteOneReg(0x29,0x41);		                                                       
			camerap_WriteOneReg(0xfd,0x00);                                             
		break;

		case CAM_WB_DAYLIGHT: 
			// SP0828_reg_WB_auto  白天 
			camerap_WriteOneReg(0xfd,0x00);  //6500K                                     
			camerap_WriteOneReg(0x32,tmp&0xef);                                                          
			camerap_WriteOneReg(0xfd,0x01);                                                          
			camerap_WriteOneReg(0x28,0x6b);		                                                       
			camerap_WriteOneReg(0x29,0x48);		                                                       
			camerap_WriteOneReg(0xfd,0x00);                                                         
		break;

		case CAM_WB_INCANDESCENCE: 
			// SP0828_reg_WB_auto 白炽灯 
			camerap_WriteOneReg(0xfd,0x00);  //2800K~3000K                                     
			camerap_WriteOneReg(0x32,tmp&0xef);                                                          
			camerap_WriteOneReg(0xfd,0x01);                                                          
			camerap_WriteOneReg(0x28,0x41);		                                                       
			camerap_WriteOneReg(0x29,0x71);		                                                       
			camerap_WriteOneReg(0xfd,0x00);                                                          
		break;

		case CAM_WB_FLUORESCENT: 
			//SP0828_reg_WB_auto  荧光灯 
			camerap_WriteOneReg(0xfd,0x00);  //4200K~5000K                                     
			camerap_WriteOneReg(0x32,tmp&0xef);                                                          
			camerap_WriteOneReg(0xfd,0x01);                                                          
			camerap_WriteOneReg(0x28,0x5a);		                                                       
			camerap_WriteOneReg(0x29,0x62);		                                                       
			camerap_WriteOneReg(0xfd,0x00);                                                         
		break;

		case CAM_WB_TUNGSTEN: 
			// SP0828_reg_WB_auto 白热光
			camerap_WriteOneReg(0xfd,0x00);  //4000K                                   
			camerap_WriteOneReg(0x32,tmp&0xef);                                                          
			camerap_WriteOneReg(0xfd,0x01);                                                          
			camerap_WriteOneReg(0x28,0x57);		                                                       
			camerap_WriteOneReg(0x29,0x66);		                                                       
			camerap_WriteOneReg(0xfd,0x00);                                                         
		break;



		default:
		return FALSE;
	}

	return TRUE;

}

UINT32 SP0828_Set_Para_Effect(UINT32 para)
{

    UINT32  ret = TRUE;

	switch (para)
	{
		case CAM_EFFECT_ENC_NORMAL:
			camerap_WriteOneReg(0xfd, 0x00);
			camerap_WriteOneReg(0x62, 0x00);
			camerap_WriteOneReg(0x63, 0x80);
			camerap_WriteOneReg(0x64, 0x80);
		break;

		case CAM_EFFECT_ENC_GRAYSCALE:
			camerap_WriteOneReg(0xfd, 0x00);
			camerap_WriteOneReg(0x62, 0x40);
			camerap_WriteOneReg(0x63, 0x80);
			camerap_WriteOneReg(0x64, 0x80);

		break;

		case CAM_EFFECT_ENC_SEPIA:
			camerap_WriteOneReg(0xfd, 0x00);
			camerap_WriteOneReg(0x62, 0x20);
			camerap_WriteOneReg(0x63, 0xc0);
			camerap_WriteOneReg(0x64, 0x20);

		break;

		case CAM_EFFECT_ENC_COLORINV:
			camerap_WriteOneReg(0xfd, 0x00);
			camerap_WriteOneReg(0x62, 0x10);
			camerap_WriteOneReg(0x63, 0x80);
			camerap_WriteOneReg(0x64, 0x80);
		break;

		case CAM_EFFECT_ENC_SEPIAGREEN:
			camerap_WriteOneReg(0xfd, 0x00);
			camerap_WriteOneReg(0x62, 0x20);
			camerap_WriteOneReg(0x63, 0x20);
			camerap_WriteOneReg(0x64, 0x20);
		break;

		case CAM_EFFECT_ENC_SEPIABLUE:
			camerap_WriteOneReg(0xfd, 0x00);
			camerap_WriteOneReg(0x62, 0x20);
			camerap_WriteOneReg(0x63, 0x20);
			camerap_WriteOneReg(0x64, 0xf0);

		break;

		case CAM_EFFECT_ENC_GRAYINV:
			camerap_WriteOneReg(0xfd, 0x00);
			camerap_WriteOneReg(0x62, 0x10);
			camerap_WriteOneReg(0x63, 0x80);
			camerap_WriteOneReg(0x64, 0x80);

		break;

		case CAM_EFFECT_ENC_EMBOSSMENT:
			camerap_WriteOneReg(0xfd, 0x00);
			camerap_WriteOneReg(0x62, 0x02);
			camerap_WriteOneReg(0x63, 0x80);
			camerap_WriteOneReg(0x64, 0x80);

		break;

		case CAM_EFFECT_ENC_SKETCH:
			camerap_WriteOneReg(0xfd, 0x00);
			camerap_WriteOneReg(0x62, 0x01);
			camerap_WriteOneReg(0x63, 0x80);
			camerap_WriteOneReg(0x64, 0x80);

		break;

		case CAM_EFFECT_ENC_WHITEBOARD:
		case CAM_EFFECT_ENC_COPPERCARVING:        
		case CAM_EFFECT_ENC_BLUECARVING:
		case CAM_EFFECT_ENC_CONTRAST:
		case CAM_EFFECT_ENC_JEAN:       
		case CAM_EFFECT_ENC_OIL:
		default:
		return 
			ret = FALSE;
	}

	return ret;

}

UINT32 SP0828_set_param_banding(UINT32 para)
{

    SP0828_banding = 0;
    return 1;
	switch (para)
	{
		case CAM_BANDING_50HZ:
			SP0828_banding = 0;
		 
		break;

		case CAM_BANDING_60HZ:
			SP0828_banding = 1;
		 
		break;

		default:
		return FALSE;
	}

	return  TRUE;
}

void SP0828_night_mode(BOOL enable)
{
    SP0828_State = CAMERA;
	if (!enable) //daylight
	{	
		if(SP0828_State == VIDEO)	
		{
			if(SP0828_banding == 0)
			{
			//Video record daylight 26->13M 50hz fix 16FPS maxgain:0x78
			camerap_WriteOneReg(0xfd,0x00);
			camerap_WriteOneReg(0x05,0x0 );
			camerap_WriteOneReg(0x06,0x0 );
			camerap_WriteOneReg(0x09,0x3 );
			camerap_WriteOneReg(0x0a,0xcf);
			camerap_WriteOneReg(0xf0,0x34);
			camerap_WriteOneReg(0xf1,0x0 );
			camerap_WriteOneReg(0xf2,0x52);
			camerap_WriteOneReg(0xf5,0x6b);
			camerap_WriteOneReg(0xfd,0x01);
			camerap_WriteOneReg(0x00,0x9f);
			camerap_WriteOneReg(0x0f,0x53);
			camerap_WriteOneReg(0x16,0x53);
			camerap_WriteOneReg(0x17,0x87);
			camerap_WriteOneReg(0x18,0x8f);
			camerap_WriteOneReg(0x1b,0x53);
			camerap_WriteOneReg(0x1c,0x8f);
			camerap_WriteOneReg(0xb4,0x20);
			camerap_WriteOneReg(0xb5,0x32);
			camerap_WriteOneReg(0xb6,0x32);
			camerap_WriteOneReg(0xb9,0x40);
			camerap_WriteOneReg(0xba,0x4f);
			camerap_WriteOneReg(0xbb,0x47);
			camerap_WriteOneReg(0xbc,0x45);
			camerap_WriteOneReg(0xbd,0x43);
			camerap_WriteOneReg(0xbe,0x42);
			camerap_WriteOneReg(0xbf,0x78);
			camerap_WriteOneReg(0xc0,0x42);
			camerap_WriteOneReg(0xc1,0x41);
			camerap_WriteOneReg(0xc2,0x41);
			camerap_WriteOneReg(0xc3,0x41);
			camerap_WriteOneReg(0xc4,0x41);
			camerap_WriteOneReg(0xc5,0x41);
			camerap_WriteOneReg(0xc6,0x41);
			camerap_WriteOneReg(0xca,0x78);
			camerap_WriteOneReg(0xcb,0x6 );	
			camerap_WriteOneReg(0x14,0x20);
			camerap_WriteOneReg(0x15,0x0f);			
			camerap_WriteOneReg(0xfd,0x00);
			
			}
			else if(SP0828_banding == 1)
			{
			//Video record daylight 26->13M 60hz fix 16FPS maxgain:0x78
			camerap_WriteOneReg(0xfd,0x00);
			camerap_WriteOneReg(0x05,0x0 );
			camerap_WriteOneReg(0x06,0x0 );
			camerap_WriteOneReg(0x09,0x3 );
			camerap_WriteOneReg(0x0a,0xcf);
			camerap_WriteOneReg(0xf0,0x2c);
			camerap_WriteOneReg(0xf1,0x0 );
			camerap_WriteOneReg(0xf2,0x4e);
			camerap_WriteOneReg(0xf5,0x67);
			camerap_WriteOneReg(0xfd,0x01);
			camerap_WriteOneReg(0x00,0x9e);
			camerap_WriteOneReg(0x0f,0x4f);
			camerap_WriteOneReg(0x16,0x4f);
			camerap_WriteOneReg(0x17,0x86);
			camerap_WriteOneReg(0x18,0x8e);
			camerap_WriteOneReg(0x1b,0x4f);
			camerap_WriteOneReg(0x1c,0x8e);
			camerap_WriteOneReg(0xb4,0x20);
			camerap_WriteOneReg(0xb5,0x2a);
			camerap_WriteOneReg(0xb6,0x2a);
			camerap_WriteOneReg(0xb9,0x40);
			camerap_WriteOneReg(0xba,0x4f);
			camerap_WriteOneReg(0xbb,0x47);
			camerap_WriteOneReg(0xbc,0x45);
			camerap_WriteOneReg(0xbd,0x43);
			camerap_WriteOneReg(0xbe,0x42);
			camerap_WriteOneReg(0xbf,0x42);
			camerap_WriteOneReg(0xc0,0x78);
			camerap_WriteOneReg(0xc1,0x41);
			camerap_WriteOneReg(0xc2,0x41);
			camerap_WriteOneReg(0xc3,0x41);
			camerap_WriteOneReg(0xc4,0x41);
			camerap_WriteOneReg(0xc5,0x41);
			camerap_WriteOneReg(0xc6,0x41);
			camerap_WriteOneReg(0xca,0x78);
			camerap_WriteOneReg(0xcb,0x7 );
			camerap_WriteOneReg(0x14,0x20);
			camerap_WriteOneReg(0x15,0x0f);
			camerap_WriteOneReg(0xfd,0x00);
			}
		}
	else
		{
			if(SP0828_banding == 0)
			{
			//caprure preview daylight 26->13M 50hz 16-8FPS maxgain:0x70	
			camerap_WriteOneReg(0xfd,0x00);
			camerap_WriteOneReg(0x05,0x0 );
			camerap_WriteOneReg(0x06,0x0 );
			camerap_WriteOneReg(0x09,0x3 );
			camerap_WriteOneReg(0x0a,0xcf);
			camerap_WriteOneReg(0xf0,0x34);
			camerap_WriteOneReg(0xf1,0x0 );
			camerap_WriteOneReg(0xf2,0x52);
			camerap_WriteOneReg(0xf5,0x6b);
			camerap_WriteOneReg(0xfd,0x01);
			camerap_WriteOneReg(0x00,0xa5);
			camerap_WriteOneReg(0x0f,0x53);
			camerap_WriteOneReg(0x16,0x53);
			camerap_WriteOneReg(0x17,0x95);
			camerap_WriteOneReg(0x18,0x9d);
			camerap_WriteOneReg(0x1b,0x53);
			camerap_WriteOneReg(0x1c,0x9d);
			camerap_WriteOneReg(0xb4,0x20);
			camerap_WriteOneReg(0xb5,0x32);
			camerap_WriteOneReg(0xb6,0x32);
			camerap_WriteOneReg(0xb9,0x40);
			camerap_WriteOneReg(0xba,0x4f);
			camerap_WriteOneReg(0xbb,0x47);
			camerap_WriteOneReg(0xbc,0x45);
			camerap_WriteOneReg(0xbd,0x43);
			camerap_WriteOneReg(0xbe,0x42);
			camerap_WriteOneReg(0xbf,0x42);
			camerap_WriteOneReg(0xc0,0x42);
			camerap_WriteOneReg(0xc1,0x41);
			camerap_WriteOneReg(0xc2,0x41);
			camerap_WriteOneReg(0xc3,0x41);
			camerap_WriteOneReg(0xc4,0x41);
			camerap_WriteOneReg(0xc5,0x70);
			camerap_WriteOneReg(0xc6,0x41);
			camerap_WriteOneReg(0xca,0x70);
			camerap_WriteOneReg(0xcb,0xc );
			camerap_WriteOneReg(0x14,0x20);
			camerap_WriteOneReg(0x15,0x0f);
			camerap_WriteOneReg(0xfd,0x00);
			
			}
			else if(SP0828_banding == 1)
			{
			//caprure preview daylight 26->13M 60hz 16-8FPS maxgain:0x70	  	
			camerap_WriteOneReg(0xfd,0x00);
			camerap_WriteOneReg(0x05,0x0 );
			camerap_WriteOneReg(0x06,0x0 );
			camerap_WriteOneReg(0x09,0x3 );
			camerap_WriteOneReg(0x0a,0xcf);
			camerap_WriteOneReg(0xf0,0x2c);
			camerap_WriteOneReg(0xf1,0x0 );
			camerap_WriteOneReg(0xf2,0x4e);
			camerap_WriteOneReg(0xf5,0x67);
			camerap_WriteOneReg(0xfd,0x01);
			camerap_WriteOneReg(0x00,0xa6);
			camerap_WriteOneReg(0x0f,0x4f);
			camerap_WriteOneReg(0x16,0x4f);
			camerap_WriteOneReg(0x17,0x96);
			camerap_WriteOneReg(0x18,0x9e);
			camerap_WriteOneReg(0x1b,0x4f);
			camerap_WriteOneReg(0x1c,0x9e);
			camerap_WriteOneReg(0xb4,0x20);
			camerap_WriteOneReg(0xb5,0x2a);
			camerap_WriteOneReg(0xb6,0x2a);
			camerap_WriteOneReg(0xb9,0x40);
			camerap_WriteOneReg(0xba,0x4f);
			camerap_WriteOneReg(0xbb,0x47);
			camerap_WriteOneReg(0xbc,0x45);
			camerap_WriteOneReg(0xbd,0x43);
			camerap_WriteOneReg(0xbe,0x42);
			camerap_WriteOneReg(0xbf,0x42);
			camerap_WriteOneReg(0xc0,0x42);
			camerap_WriteOneReg(0xc1,0x41);
			camerap_WriteOneReg(0xc2,0x41);
			camerap_WriteOneReg(0xc3,0x41);
			camerap_WriteOneReg(0xc4,0x41);
			camerap_WriteOneReg(0xc5,0x41);
			camerap_WriteOneReg(0xc6,0x41);
			camerap_WriteOneReg(0xca,0x70);
			camerap_WriteOneReg(0xcb,0xf );
			camerap_WriteOneReg(0x14,0x20);
			camerap_WriteOneReg(0x15,0x0f);
			camerap_WriteOneReg(0xfd,0x00);
			}
		}	
	}
else //night mode
{
        if(SP0828_State == VIDEO)		
		{
			if(SP0828_banding == 0)
			{
			//Video record night 26->13M 50hz 16-10FPS maxgain:0x78 
			camerap_WriteOneReg(0xfd,0x00);
			camerap_WriteOneReg(0x05,0x0 );
			camerap_WriteOneReg(0x06,0x0 );
			camerap_WriteOneReg(0x09,0x3 );
			camerap_WriteOneReg(0x0a,0xcf);
			camerap_WriteOneReg(0xf0,0x34);
			camerap_WriteOneReg(0xf1,0x0 );
			camerap_WriteOneReg(0xf2,0x52);
			camerap_WriteOneReg(0xf5,0x6b);
			camerap_WriteOneReg(0xfd,0x01);
			camerap_WriteOneReg(0x00,0xa9);
			camerap_WriteOneReg(0x0f,0x53);
			camerap_WriteOneReg(0x16,0x53);
			camerap_WriteOneReg(0x17,0x91);
			camerap_WriteOneReg(0x18,0x99);
			camerap_WriteOneReg(0x1b,0x53);
			camerap_WriteOneReg(0x1c,0x99);
			camerap_WriteOneReg(0xb4,0x20);
			camerap_WriteOneReg(0xb5,0x32);
			camerap_WriteOneReg(0xb6,0x32);
			camerap_WriteOneReg(0xb9,0x40);
			camerap_WriteOneReg(0xba,0x4f);
			camerap_WriteOneReg(0xbb,0x47);
			camerap_WriteOneReg(0xbc,0x45);
			camerap_WriteOneReg(0xbd,0x43);
			camerap_WriteOneReg(0xbe,0x42);
			camerap_WriteOneReg(0xbf,0x42);
			camerap_WriteOneReg(0xc0,0x42);
			camerap_WriteOneReg(0xc1,0x41);
			camerap_WriteOneReg(0xc2,0x41);
			camerap_WriteOneReg(0xc3,0x78);
			camerap_WriteOneReg(0xc4,0x41);
			camerap_WriteOneReg(0xc5,0x41);
			camerap_WriteOneReg(0xc6,0x41);
			camerap_WriteOneReg(0xca,0x78);
			camerap_WriteOneReg(0xcb,0xa );
			camerap_WriteOneReg(0x14,0x20);
			camerap_WriteOneReg(0x15,0x1f);
			camerap_WriteOneReg(0xfd,0x00);
			}
			else if(SP0828_banding == 1)
			{
			//Video record night 26->13M 60hz 16-10FPS maxgain:0x78 
			camerap_WriteOneReg(0xfd,0x00);
			camerap_WriteOneReg(0x05,0x0 );
			camerap_WriteOneReg(0x06,0x0 );
			camerap_WriteOneReg(0x09,0x3 );
			camerap_WriteOneReg(0x0a,0xcf);
			camerap_WriteOneReg(0xf0,0x2c);
			camerap_WriteOneReg(0xf1,0x0 );
			camerap_WriteOneReg(0xf2,0x4e);
			camerap_WriteOneReg(0xf5,0x67);
			camerap_WriteOneReg(0xfd,0x01);
			camerap_WriteOneReg(0x00,0xa9);
			camerap_WriteOneReg(0x0f,0x4f);
			camerap_WriteOneReg(0x16,0x4f);
			camerap_WriteOneReg(0x17,0x91);
			camerap_WriteOneReg(0x18,0x99);
			camerap_WriteOneReg(0x1b,0x4f);
			camerap_WriteOneReg(0x1c,0x99);
			camerap_WriteOneReg(0xb4,0x20);
			camerap_WriteOneReg(0xb5,0x2a);
			camerap_WriteOneReg(0xb6,0x2a);
			camerap_WriteOneReg(0xb9,0x40);
			camerap_WriteOneReg(0xba,0x4f);
			camerap_WriteOneReg(0xbb,0x47);
			camerap_WriteOneReg(0xbc,0x45);
			camerap_WriteOneReg(0xbd,0x43);
			camerap_WriteOneReg(0xbe,0x42);
			camerap_WriteOneReg(0xbf,0x42);
			camerap_WriteOneReg(0xc0,0x42);
			camerap_WriteOneReg(0xc1,0x41);
			camerap_WriteOneReg(0xc2,0x41);
			camerap_WriteOneReg(0xc3,0x41);
			camerap_WriteOneReg(0xc4,0x41);
			camerap_WriteOneReg(0xc5,0x78);
			camerap_WriteOneReg(0xc6,0x41);
			camerap_WriteOneReg(0xca,0x78);
			camerap_WriteOneReg(0xcb,0xc );
			camerap_WriteOneReg(0x14,0x20);
			camerap_WriteOneReg(0x15,0x1f);
			camerap_WriteOneReg(0xfd,0x00);
			}
		}
       else
		{
			if(SP0828_banding == 0)
			{
			//caprure preview night 26->13M 50hz 16-6FPS maxgain:0x78 
			camerap_WriteOneReg(0xfd,0x00);
			camerap_WriteOneReg(0x05,0x0 );
			camerap_WriteOneReg(0x06,0x0 );
			camerap_WriteOneReg(0x09,0x3 );
			camerap_WriteOneReg(0x0a,0xcf);
			camerap_WriteOneReg(0xf0,0x34);
			camerap_WriteOneReg(0xf1,0x0 );
			camerap_WriteOneReg(0xf2,0x52);
			camerap_WriteOneReg(0xf5,0x6b);
			camerap_WriteOneReg(0xfd,0x01);
			camerap_WriteOneReg(0x00,0xb3);
			camerap_WriteOneReg(0x0f,0x53);
			camerap_WriteOneReg(0x16,0x53);
			camerap_WriteOneReg(0x17,0x9b);
			camerap_WriteOneReg(0x18,0xa3);
			camerap_WriteOneReg(0x1b,0x53);
			camerap_WriteOneReg(0x1c,0xa3);
			camerap_WriteOneReg(0xb4,0x20);
			camerap_WriteOneReg(0xb5,0x32);
			camerap_WriteOneReg(0xb6,0x32);
			camerap_WriteOneReg(0xb9,0x40);
			camerap_WriteOneReg(0xba,0x4f);
			camerap_WriteOneReg(0xbb,0x47);
			camerap_WriteOneReg(0xbc,0x45);
			camerap_WriteOneReg(0xbd,0x43);
			camerap_WriteOneReg(0xbe,0x42);
			camerap_WriteOneReg(0xbf,0x42);
			camerap_WriteOneReg(0xc0,0x42);
			camerap_WriteOneReg(0xc1,0x41);
			camerap_WriteOneReg(0xc2,0x41);
			camerap_WriteOneReg(0xc3,0x41);
			camerap_WriteOneReg(0xc4,0x41);
			camerap_WriteOneReg(0xc5,0x41);
			camerap_WriteOneReg(0xc6,0x41);
			camerap_WriteOneReg(0xca,0x78);
			camerap_WriteOneReg(0xcb,0x10);
			camerap_WriteOneReg(0x14,0x20);
			camerap_WriteOneReg(0x15,0x1f);
			camerap_WriteOneReg(0xfd,0x00);
			}
			else if(SP0828_banding == 1)
			{
			//caprure preview night 26->13M 60hz 16-6FPS maxgain:0x78		
			camerap_WriteOneReg(0xfd,0x00);
			camerap_WriteOneReg(0x05,0x0 );
			camerap_WriteOneReg(0x06,0x0 );
			camerap_WriteOneReg(0x09,0x3 );
			camerap_WriteOneReg(0x0a,0xcf);
			camerap_WriteOneReg(0xf0,0x2c);
			camerap_WriteOneReg(0xf1,0x0 );
			camerap_WriteOneReg(0xf2,0x4e);
			camerap_WriteOneReg(0xf5,0x67);
			camerap_WriteOneReg(0xfd,0x01);
			camerap_WriteOneReg(0x00,0xb3);
			camerap_WriteOneReg(0x0f,0x4f);
			camerap_WriteOneReg(0x16,0x4f);
			camerap_WriteOneReg(0x17,0x9b);
			camerap_WriteOneReg(0x18,0xa3);
			camerap_WriteOneReg(0x1b,0x4f);
			camerap_WriteOneReg(0x1c,0xa3);
			camerap_WriteOneReg(0xb4,0x20);
			camerap_WriteOneReg(0xb5,0x2a);
			camerap_WriteOneReg(0xb6,0x2a);
			camerap_WriteOneReg(0xb9,0x40);
			camerap_WriteOneReg(0xba,0x4f);
			camerap_WriteOneReg(0xbb,0x47);
			camerap_WriteOneReg(0xbc,0x45);
			camerap_WriteOneReg(0xbd,0x43);
			camerap_WriteOneReg(0xbe,0x42);
			camerap_WriteOneReg(0xbf,0x42);
			camerap_WriteOneReg(0xc0,0x42);
			camerap_WriteOneReg(0xc1,0x41);
			camerap_WriteOneReg(0xc2,0x41);
			camerap_WriteOneReg(0xc3,0x41);
			camerap_WriteOneReg(0xc4,0x41);
			camerap_WriteOneReg(0xc5,0x41);
			camerap_WriteOneReg(0xc6,0x41);
			camerap_WriteOneReg(0xca,0x78);
			camerap_WriteOneReg(0xcb,0x14);
			camerap_WriteOneReg(0x14,0x20);
			camerap_WriteOneReg(0x15,0x1f);
			camerap_WriteOneReg(0xfd,0x00);
			}
		}
}

}

UINT32 SP0828_set_param_exposure(UINT32 para)
{
	switch (para)
	{
		case CAM_EV_NEG_4_3:    // -4 EV
			camerap_WriteOneReg(0xfd, 0x00);
			camerap_WriteOneReg(0xdc, 0xc0);
		break;

		case CAM_EV_NEG_3_3:    // -3 EV
			camerap_WriteOneReg(0xfd, 0x00);
			camerap_WriteOneReg(0xdc, 0xd0);
		break;

		case CAM_EV_NEG_2_3:    // -2 EV
			camerap_WriteOneReg(0xfd, 0x00);
			camerap_WriteOneReg(0xdc, 0xe0);
		break;

		case CAM_EV_NEG_1_3:    // -1 EV
			camerap_WriteOneReg(0xfd, 0x00);
			camerap_WriteOneReg(0xdc, 0xf0);
		break;

		case CAM_EV_ZERO:   // +0 EV
			camerap_WriteOneReg(0xfd, 0x00);
			camerap_WriteOneReg(0xdc, 0x00);
		break;

		case CAM_EV_POS_1_3:    // +1 EV
			camerap_WriteOneReg(0xfd, 0x00);
			camerap_WriteOneReg(0xdc, 0x10);
		break;

		case CAM_EV_POS_2_3:    // +2 EV
			camerap_WriteOneReg(0xfd, 0x00);
			camerap_WriteOneReg(0xdc, 0x20);
		break;

		case CAM_EV_POS_3_3:    // +3 EV
			camerap_WriteOneReg(0xfd, 0x00);
			camerap_WriteOneReg(0xdc, 0x30);
		break;

		case CAM_EV_POS_4_3:    // +4 EV
			camerap_WriteOneReg(0xfd, 0x00);
			camerap_WriteOneReg(0xdc, 0x40);
		break;

		default:
		return  FALSE;    
	}
	return  TRUE;	

}

UINT32 SP0828_set_param_ev(UINT32 para)
{
	return SP0828_set_param_exposure(para);
}
extern VOID lcd_ili9163c_SetWidows_sensor();
extern VOID lcddp_SetWindowsForSpiSensor();

void SP0828_Preview(UINT16 image_target_width, UINT16 image_target_height,UINT8 clk_NP,UINT32 HBlank)
{

  if (power_on == FALSE)
  {
      power_on = TRUE;
      camerap_PowerOnForPreview(CAM_NPIX_QQVGA,128,160,CAM_FORMAT_RGB565);
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
   hal_GpioSetOut(g_sensor_ready.gpioId);
   hal_GpioClr(g_sensor_ready);
   lcddp_SetWindowsForSpiSensor();
   hal_GpioClr(g_csb_gpo);
  //SP0828_IO_TriState(FALSE);
  //SP0828_Output_Disable();
  hal_HstSendEvent(0x123456);
   SP0828_Preview_Size = Size_128_160;
   SP0828_Master_Init();
   //特效函数
   SP0828_set_param_wb(CAM_WB_AUTO);
   SP0828_Set_Para_Effect(CAM_EFFECT_ENC_NORMAL);
   SP0828_set_param_ev(CAM_EV_ZERO);
   SP0828_set_param_banding(0);
   SP0828_night_mode(FALSE);

   camerap_Delay(100); 
   SP0828_Open_Auto_Exp();
   hal_GpioSet(g_sensor_ready);


  // hal_GpioClr(g_sensor_ready);
  // SP0828_Output_Disable();
  // hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CS1, HAL_SPI_PIN_STATE_Z);
  // SP0828_Output_Enable();
  // set the lcd widowns
   
}

UINT8 SP0828_Clock_Return_Value(UINT8 data_in)
{
	UINT8 data_out;
	switch(data_in)
	{
	case 0x00:
		data_out = 1;
		break;
	case 0x02:
		data_out = 2;
		break;
	case 0x04:
		data_out = 4;
		break;
	case 0x06:
		data_out = 8;
		break;
	case 0x0c:
		data_out = 16;
		break;
	case 0x0e:
		data_out = 32;
		break;
	}

	return data_out;
}


UINT8 SP0828_Log2_Return_Value(UINT8 data_in)
{
	UINT8 data_out;
	switch(data_in)
	{
	case 0x01:
		data_out = 0;
		break;
	case 0x02:
		data_out = 1;
		break;
	case 0x04:
		data_out = 2;
		break;
	case 0x08:
		data_out = 3;
		break;
	case 0x10:
		data_out = 4;
		break;
	case 0x20:
		data_out = 5;
		break;
	}

	return data_out;
}


void SP0828_Spi_Sensor_Exp_Set(void)    
{
	int i;
	UINT32 captureWaitTime = 0;  
    UINT8  RegVal = 0, RegVal1 = 0, ExpH = 0, ExpL = 0,ExpH_a =0, ExpL_a =0,Yset=0;
	UINT16  old_0x30 = 0, old_0x09 = 0, old_0x0a = 0;
	UINT16 ExpVal = 0,  ExpVal_tmp = 0,tmp2 = 0,tmp1 = 0,tmp  = 0;
	UINT8 tmp_p0_0xf0,tmp_p1_0xcb;
	
	camerap_WriteOneReg(0xfd,0x00);

    RegVal1= camerap_ReadReg(0x23);  
	ExpH = camerap_ReadReg(0x03);
	ExpL = camerap_ReadReg(0x04);
	tmp_p0_0xf0 = camerap_ReadReg(0xf0);
	old_0x30 = camerap_ReadReg(0x30);
	old_0x09 = camerap_ReadReg(0x09);
	old_0x0a = camerap_ReadReg(0x0a);
	camerap_WriteOneReg(0xfd,0x01);
	tmp_p1_0xcb = camerap_ReadReg(0xcb);
	camerap_WriteOneReg(0xfd,0x00);
	
	camerap_WriteOneReg(0x32,0x50); 
	ExpVal_tmp = ((ExpH<<8) | ExpL) ;

	
	if(ExpVal_tmp<tmp_p0_0xf0) 
	{
		camerap_WriteOneReg(0xfd,0x00);
		camerap_WriteOneReg(0x56,Pre_Value_P0_0x56);

		camerap_WriteOneReg(0x51,0x3f);
		camerap_WriteOneReg(0x57,Pre_Value_P0_0x57);		
		camerap_WriteOneReg(0x5a,Pre_Value_P0_0x5a);	

		camerap_WriteOneReg(0x65,Pre_Value_P0_0x65);	
		camerap_WriteOneReg(0x66,Pre_Value_P0_0x66);
		camerap_WriteOneReg(0x67,Pre_Value_P0_0x67);	
		camerap_WriteOneReg(0x68,Pre_Value_P0_0x68);
		camerap_WriteOneReg(0x69,0x7f);
		camerap_WriteOneReg(0x6a,0x01);
		
		camerap_WriteOneReg(0xcc,0x04);	
		camerap_WriteOneReg(0xce,0xff);			
		camerap_WriteOneReg(0xcf,0x10);	
		
		camerap_WriteOneReg(0xd8,Pre_Value_P0_0xd8);	
					
	} 
	else if((tmp_p0_0xf0<=ExpVal_tmp)&&(ExpVal_tmp<=((tmp_p1_0xcb-1)*tmp_p0_0xf0)))
	{
		camerap_WriteOneReg(0xfd,0x00);
	    camerap_WriteOneReg(0x56,Pre_Value_P0_0x56);

		camerap_WriteOneReg(0x51,0x09);	
		camerap_WriteOneReg(0x57,Pre_Value_P0_0x58);	
		camerap_WriteOneReg(0x5a,Pre_Value_P0_0x5b);	
		
		camerap_WriteOneReg(0x65,Pre_Value_P0_0x6b);	
		camerap_WriteOneReg(0x66,Pre_Value_P0_0x6c);
		camerap_WriteOneReg(0x67,Pre_Value_P0_0x6d);	
		camerap_WriteOneReg(0x68,Pre_Value_P0_0x6e);
		camerap_WriteOneReg(0x69,0x7f);
		camerap_WriteOneReg(0x6a,0x01);
		
		camerap_WriteOneReg(0xcc,0x20);	
		camerap_WriteOneReg(0xce,0x1c);			
		camerap_WriteOneReg(0xcf,0x16);
		
		camerap_WriteOneReg(0xd8,Pre_Value_P0_0xd9);
	}
	else if(RegVal1<0x65)
	{
		camerap_WriteOneReg(0xfd,0x00);
		camerap_WriteOneReg(0x56,Pre_Value_P0_0x56);

		camerap_WriteOneReg(0x51,0x09);
		camerap_WriteOneReg(0x57,(Pre_Value_P0_0x58-0x10));	
		camerap_WriteOneReg(0x5a,(Pre_Value_P0_0x5b+0x05));
		
		camerap_WriteOneReg(0x65,(Pre_Value_P0_0x6b+0x02));	
		camerap_WriteOneReg(0x66,Pre_Value_P0_0x6c);
		camerap_WriteOneReg(0x67,Pre_Value_P0_0x6d);	
		camerap_WriteOneReg(0x68,Pre_Value_P0_0x6e);
		camerap_WriteOneReg(0x69,0x7f);
		camerap_WriteOneReg(0x6a,0x01);
		
		camerap_WriteOneReg(0xcc,0x00);	
		camerap_WriteOneReg(0xce,0x1c);			
		camerap_WriteOneReg(0xcf,0x16);
		
		camerap_WriteOneReg(0xd8,Pre_Value_P0_0xda);
	}
	else
	{
		camerap_WriteOneReg(0xfd,0x00);
		camerap_WriteOneReg(0x56,Pre_Value_P0_0x56|0x04);
		
		camerap_WriteOneReg(0x51,0x20);
		camerap_WriteOneReg(0x57,Pre_Value_P0_0x59);
		camerap_WriteOneReg(0x5a,Pre_Value_P0_0x5c);
		
		camerap_WriteOneReg(0x65,Pre_Value_P0_0x71);
		camerap_WriteOneReg(0x66,Pre_Value_P0_0x72);
		camerap_WriteOneReg(0x67,Pre_Value_P0_0x73);
		camerap_WriteOneReg(0x68,Pre_Value_P0_0x74);
		camerap_WriteOneReg(0x69,0x7f);
		camerap_WriteOneReg(0x6a,0x01);
		
		camerap_WriteOneReg(0xcc,0x00);
		camerap_WriteOneReg(0xce,0x1c);
		camerap_WriteOneReg(0xcf,0x16);
		
		camerap_WriteOneReg(0xd8,Pre_Value_P0_0xdb);
	}
	
   if(ExpVal_tmp>=6)
	{

		
		tmp = (old_0x09 << 8) + old_0x0a +256;

		if(SP0828_Capture_Size == Size_128_104)
		{
			tmp1 = (((UINT16)S_128_104_0x09_Value) << 8) + S_128_104_0x0a_Value + 256;
			tmp2 = SP0828_Clock_Return_Value((UINT8)S_128_104_0x30_Value)/SP0828_Clock_Return_Value((UINT8)old_0x30);
			ExpVal = ExpVal_tmp >> (SP0828_Log2_Return_Value(tmp2));
			
		}
		else if(SP0828_Capture_Size == Size_128_160)
		{
			tmp1 = (((UINT16)S_128_160_0x09_Value) << 8) + S_128_160_0x0a_Value + 256;
			tmp2 = SP0828_Clock_Return_Value((UINT8)S_128_160_0x30_Value)/SP0828_Clock_Return_Value((UINT8)old_0x30);
			ExpVal = ExpVal_tmp >> (SP0828_Log2_Return_Value(tmp2));
		}
		else if(SP0828_Capture_Size == Size_144_176)
		{
			tmp1 = (((UINT16)S_144_176_0x09_Value) << 8) + S_144_176_0x0a_Value + 256;
			tmp2 = SP0828_Clock_Return_Value((UINT8)S_144_176_0x30_Value)/SP0828_Clock_Return_Value((UINT8)old_0x30);
			ExpVal = ExpVal_tmp >> (SP0828_Log2_Return_Value(tmp2));
		}
		else if(SP0828_Capture_Size == Size_64_56)
		{
			tmp1 = (((UINT16)S_64_56_0x09_Value) << 8) + S_64_56_0x0a_Value + 256;
			tmp2 = SP0828_Clock_Return_Value((UINT8)S_64_56_0x30_Value)/SP0828_Clock_Return_Value((UINT8)old_0x30);
			ExpVal = ExpVal_tmp >> (SP0828_Log2_Return_Value(tmp2));
		}
		else if(SP0828_Capture_Size == Size_88_72)
		{
			tmp1 = (((UINT16)S_88_72_0x09_Value) << 8) + S_88_72_0x0a_Value + 256;
			tmp2 = SP0828_Clock_Return_Value((UINT8)S_88_72_0x30_Value)/SP0828_Clock_Return_Value((UINT8)old_0x30);
			ExpVal = ExpVal_tmp >> (SP0828_Log2_Return_Value(tmp2));
		}
		else if(SP0828_Capture_Size == Size_240_320)
		{
			tmp1 = (((UINT16)S_240_320_0x09_Value) << 8) + S_240_320_0x0a_Value + 256;
			tmp2 = SP0828_Clock_Return_Value((UINT8)S_240_320_0x30_Value)/SP0828_Clock_Return_Value((UINT8)old_0x30);
			ExpVal = ExpVal_tmp >> (SP0828_Log2_Return_Value(tmp2));
		}
		
		ExpVal = ExpVal * tmp/tmp1;


	/*	
	//	ExpVal = ExpVal * (256+0x3cf)/(256+0xacf);
		if(SP0828_Capture_Size == Size_128_104)
			ExpVal = ExpVal * (256+0x3cf)/(256+0xacf);
		else if(SP0828_Capture_Size == Size_128_160)
			ExpVal = ExpVal * (256+0x3cf)/(256+0xecf);
		else if(SP0828_Capture_Size == Size_144_176)
			ExpVal = ExpVal * (256+0x3cf)/(256+0xecf);
		else if(SP0828_Capture_Size == Size_240_320)
			ExpVal = ExpVal * (256+0x3cf)/(256+0x1fcf);
		else if(SP0828_Capture_Size == Size_64_56)
		{
			ExpVal = ExpVal_tmp;
			ExpVal = ExpVal * (256+0x3cf)/(256+0x2cf);
		}
		else if(SP0828_Capture_Size == Size_88_72)
		{
			ExpVal = ExpVal_tmp;
			ExpVal = ExpVal * (256+0x3cf)/(256+0x4cf);
		}
	*/
	}
	else
	{
		if(1<=ExpVal_tmp<=5)
		{
			ExpVal=1;
			if(ExpVal_tmp==1)
				Yset=0x96;
			else if(ExpVal_tmp==2)
				Yset=0xae;//
			else if(ExpVal_tmp==3)
				Yset=0xc6;//	
			else if(ExpVal_tmp==4)
				Yset=0xdd;//
			else if(ExpVal_tmp==5)
				Yset=0xec;//
		}	
		
		camerap_WriteOneReg(0xdc, Yset);	
	}
		

	camerap_WriteOneReg(0x03, (UINT8)((ExpVal>>8)&0x00ff));
	camerap_WriteOneReg(0x04, (UINT8)(ExpVal&0x00ff));
	camerap_WriteOneReg(0x24,RegVal1);
	
  	camerap_WriteOneReg(0xe7,0x03);
    camerap_WriteOneReg(0xe7,0x00);
   
	//camerap_ReadReg(0x03, &ExpH_a);
	//camerap_ReadReg(0x04, &ExpL_a);
}

//UINT16 sp0828_preview_startx = 0;
//UINT16 sp0828_preview_endy = 0;
UINT32 sp0828_capture_size = 0;
UINT32 sp0828_mem_size = 0;
UINT32 gSP0828_width_size = 0;
UINT32 gSP0828_length_size = 0;

void SP0828_Slave_To_Master(void)
{
	UINT32 i,reg_count;
	UINT8 *SP0828_CONFIG_TYPE;

	if(SP0828_Preview_Size == Size_128_104)
	{
		SP0828_CONFIG_TYPE = SP0828_SLAVE_TO_MASTER_CONFIG_128_104;
		sp0828_preview_startx = SP0828_CONFIG_TYPE[0];
		sp0828_preview_starty = SP0828_CONFIG_TYPE[1];
		sp0828_preview_endx = sp0828_preview_startx + 127;
		sp0828_preview_endy = sp0828_preview_starty + 103;
		reg_count = S_M_Reg_Count_128_104 / 2;
		
	}
	
	else if(SP0828_Preview_Size == Size_144_176)
	{
		SP0828_CONFIG_TYPE = SP0828_SLAVE_TO_MASTER_CONFIG_144_176;
		sp0828_preview_startx = SP0828_CONFIG_TYPE[0];
		sp0828_preview_starty = SP0828_CONFIG_TYPE[1];
		sp0828_preview_endx = sp0828_preview_startx + 143;
		sp0828_preview_endy = sp0828_preview_starty + 175;
		reg_count = S_M_Reg_Count_144_176 / 2;
		
	}
	else if(SP0828_Preview_Size == Size_128_160)
	{
		SP0828_CONFIG_TYPE = SP0828_SLAVE_TO_MASTER_CONFIG_128_160;
		sp0828_preview_startx = SP0828_CONFIG_TYPE[0];
		sp0828_preview_starty = SP0828_CONFIG_TYPE[1];
		sp0828_preview_endx = sp0828_preview_startx + 127;
		sp0828_preview_endy = sp0828_preview_starty + 159;
		reg_count = S_M_Reg_Count_128_160 / 2;
		
	}
	else if(SP0828_Preview_Size == Size_176_220)
	{
		SP0828_CONFIG_TYPE = SP0828_SLAVE_TO_MASTER_CONFIG_176_220;
		sp0828_preview_startx = SP0828_CONFIG_TYPE[0];
		sp0828_preview_starty = SP0828_CONFIG_TYPE[1];
		sp0828_preview_endx = sp0828_preview_startx + 175;
		sp0828_preview_endy = sp0828_preview_starty + 219;
		reg_count = S_M_Reg_Count_176_220 / 2;
	}
	else if(SP0828_Preview_Size == Size_240_320)
	{
		SP0828_CONFIG_TYPE = SP0828_SLAVE_TO_MASTER_CONFIG_240_320;
		sp0828_preview_startx = SP0828_CONFIG_TYPE[0];
		sp0828_preview_starty = SP0828_CONFIG_TYPE[1];
		sp0828_preview_endx = sp0828_preview_startx + 239;
		sp0828_preview_endy = sp0828_preview_starty + 319;
		reg_count = S_M_Reg_Count_240_320 / 2;
	}

	for (i = 1;i <= reg_count;i++)
		camerap_WriteOneReg(SP0828_CONFIG_TYPE[i*2],SP0828_CONFIG_TYPE[i*2+1] );

	SP0828_Lcd_Clock_Set();
	
}
void SP0828_Master_To_Slave(void)
{
	UINT16 i,reg_count;
	UINT8 *val;
	UINT8 *SP0828_CONFIG_TYPE;

	if(SP0828_Capture_Size == Size_60_80)
	{
		SP0828_CONFIG_TYPE = SP0828_SLAVE_CONFIG_60_80;
		sp0828_capture_size = 60*80;
 		sp0828_mem_size = 60*8;
		gSP0828_width_size = 60;
	 	gSP0828_length_size = 80;
		reg_count = S_Reg_Count_60_80/2;
	}
	else if(SP0828_Capture_Size == Size_64_56)
	{
		SP0828_CONFIG_TYPE = SP0828_SLAVE_CONFIG_64_56;
 		sp0828_capture_size = 64*56;
 		sp0828_mem_size = 64*8;
		gSP0828_width_size = 64;
	 	gSP0828_length_size = 52;//resize 时丢4行
		reg_count = S_Reg_Count_64_56/2;

	}
	else if(SP0828_Capture_Size == Size_88_72)
	{
		SP0828_CONFIG_TYPE = SP0828_SLAVE_CONFIG_88_72;
 		sp0828_capture_size = 88*72;
 		sp0828_mem_size = 88*8;
		gSP0828_width_size = 88;
	 	gSP0828_length_size = 72;
		reg_count = S_Reg_Count_88_72/2;

	}

	else if(SP0828_Capture_Size == Size_128_104)
	{
		SP0828_CONFIG_TYPE = SP0828_SLAVE_CONFIG_128_104;
 		sp0828_capture_size = 128*104;
 		sp0828_mem_size = 128*4;
		gSP0828_width_size = 128;
	 	gSP0828_length_size = 104;
		reg_count = S_Reg_Count_128_104/2;

	}
	
	else if(SP0828_Capture_Size == Size_128_160)
	{
		SP0828_CONFIG_TYPE = SP0828_SLAVE_CONFIG_128_160;
 		sp0828_capture_size = 128*160;
 		sp0828_mem_size = 128*5;
		gSP0828_width_size = 128;
	 	gSP0828_length_size = 160;
		reg_count = S_Reg_Count_128_160/2;

	}
	else if(SP0828_Capture_Size == Size_144_176)
	{
		
		SP0828_CONFIG_TYPE = SP0828_SLAVE_CONFIG_144_176;
		sp0828_capture_size = 144*176;
 		sp0828_mem_size = 144*4;
 		gSP0828_width_size = 144;
	 	gSP0828_length_size = 176;
		reg_count = S_Reg_Count_144_176/2;
 
	}
	else if(SP0828_Capture_Size == Size_176_144)
	{
	
		SP0828_CONFIG_TYPE = SP0828_SLAVE_CONFIG_88_144;
		sp0828_capture_size = 88*144;
 		sp0828_mem_size = 88*8;
		gSP0828_width_size = 88;
	 	gSP0828_length_size = 144;
		reg_count = S_Reg_Count_88_144/2;
	}
	else if(SP0828_Capture_Size == Size_176_220)
	{
	/*
		SP0828_CONFIG_TYPE = SP0828_SLAVE_CONFIG_176_220;
		sp0828_capture_size = 176*220;
 		sp0828_mem_size = 176*4;
		gSP0828_width_size = 176;
	 	gSP0828_length_size = 220;
		reg_count = S_Reg_Count_176_220/2;
	*/

		SP0828_CONFIG_TYPE = SP0828_SLAVE_CONFIG_88_220;
		sp0828_capture_size = 88*220;
 		sp0828_mem_size = 88*5;
		gSP0828_width_size = 88;
	 	gSP0828_length_size = 220;
		reg_count = S_Reg_Count_88_220/2;
	}
	else if(SP0828_Capture_Size == Size_240_320)
	{
		SP0828_CONFIG_TYPE = SP0828_SLAVE_CONFIG_240_320;
		sp0828_capture_size = 240*320;
 		sp0828_mem_size = 240*2;
		gSP0828_width_size = 240;
	 	gSP0828_length_size = 320;
		reg_count = S_Reg_Count_240_320/2;
	}
	
	for (i = 0;i < reg_count;i++)
		camerap_WriteOneReg(SP0828_CONFIG_TYPE[i*2],SP0828_CONFIG_TYPE[i*2+1] );

	/*
	for (i = 0;i < reg_count;i++)
	{
		SP0828_i2c_read(SP0828_CONFIG_TYPE[i*2],val );
		dbg_print("%x val = %x ,i=%d\r\n",SP0828_CONFIG_TYPE[i*2],*val,i);
	}	
	*/
	//SP0828_i2c_read(0x2b,val );
	//SP0828_Working_Type = CAPTURE;		

}


void SP0828_Capture_Enable(void)
{	
	SP0828_Slave_Data_Output_Enable();

}
void SP0828_Preview_Disable(void)
{	
	hal_GpioClr(g_sensor_ready);
	camerap_Delay(50);
	
}
VOID test_color_bar(UINT8 mode)
{
	camerap_WriteOneReg(0x0d,0x11);
	camerap_WriteOneReg(0xe7,0x03);
	camerap_WriteOneReg(0xe7,0x00);

}
void SP0828_Capture_Process_AD(SP0828_Size_enum size)
{	
    SP0828_Preview_Disable();
    SP0828_Capture_Size =size;// Size_240_320;// Size_128_160;
	SP0828_Spi_Sensor_Exp_Set();   
	SP0828_Master_To_Slave();
	camerap_Delay(10); 
	//test_color_bar(1);
	#ifdef SWAP_SPI_SENSOR_BYTE
    for_serial_sensor = TRUE;
    #endif
	SP0828_Capture_Enable();
}

VOID SP0828_Capture_Process_AD1111() // for test
{
    SP0828_Capture_Process_AD(Size_240_320);
}
VOID PAS6175_ReadShutterOfPreview()
{

}
void PAS6175_Capture(UINT16 image_target_width, UINT16 image_target_height,UINT8 clk_NP,UINT32 HBlank)
{}

UINT8 for_serial_camer = 0;
extern  VOID test_display_sensor(UINT16 * temp);
UINT32 PAS_Cam_ReadFrame_continue_display(UINT16 * buff, UINT32 size)
{
 	return 1;

}

UINT32 PAS_Cam_ReadFrame_continue(UINT16 * buff, UINT32 size)
{
  return 1;
}
VOID SP0828_Data_Count(VOID)
{
    g_sensor_count ++;
}
VOID  SP0828_INT_ReadyDisable(VOID)
{
    hal_GpioResetIrq(g_sensor_overflow.gpioId);

}

VOID  SP0828_INT_Ready(VOID)
{	
    HAL_GPIO_IRQ_MASK_T  gpioIntMaskCfg;
    hal_GpioResetIrq(g_sensor_overflow.gpioId);
    g_sensor_count = 0;
    gpioIntMaskCfg.rising = TRUE;
    gpioIntMaskCfg.falling= FALSE;
    gpioIntMaskCfg.debounce=TRUE;
    gpioIntMaskCfg.level=FALSE;
    hal_GpioIrqSetMask(g_sensor_overflow.gpioId, &gpioIntMaskCfg);
    hal_GpioIrqSetHandler(g_sensor_overflow.gpioId ,SP0828_Data_Count);
}
UINT32 PAS_Cam_ReadFrame_Data(UINT16 * buff, UINT32 size)
{
   UINT16 bufflend = sp0828_mem_size *2;
   UINT16 timer = 0 ;
   UINT32 j = 0;
   UINT32 counts = 0;
   UINT8 * buff1;
   UINT16 g_dmaBusy=0;
   buff1 = (UINT8 *)buff;
   UINT8 tempbuff[1280]={0};
   BOOL hi = TRUE;
   UINT8 ret = 0,te;
   SP0828_INT_Ready();
   hal_HstSendEvent(0x38000001);hal_HstSendEvent(size);
   hal_SpiClearRxDmaDone(g_spi_id_sensor);
   hal_SpiClearTxDmaDone(g_spi_id_sensor);
   hal_SpiFlushFifos(g_spi_id_sensor);
   hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CS1, HAL_SPI_PIN_STATE_0);
   hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_DI, HAL_SPI_PIN_STATE_Z);
   hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_DO, HAL_SPI_PIN_STATE_Z);
   counts = 0;
   buff1 = (UINT8 *)buff;

   SP0828_Slave_Data_Output_Enable();
   hal_GpioSet(g_sensor_ready);
   while(timer>=g_sensor_count)
   {
   te++;
   }

  // while(hal_GpioGet(g_sensor_overflow.gpioId)== TRUE)
   {
      //sxr_Sleep(2 MS_WAITING);
   };
   hal_GpioClr(g_sensor_ready);
   UINT32 now = hal_TimGetUpTime();
		while(counts < size)
		{

                //camerap_Delay_get(16384/3000);
                counts +=bufflend/2;
                g_dmaBusy = hal_SpiGetData(HAL_SPI,HAL_SPI_CS1,buff1, bufflend);
                g_dmaBusy = hal_SpiSendData(HAL_SPI,HAL_SPI_CS1,tempbuff, bufflend);
                if (g_dmaBusy == 0)
                {
                 hal_DbgAssert("spi rx dma busy count = %d",counts);
                 return 1;
                }
                #if 0
                 ret = camerap_ReadReg(0x2e);
                 if((ret&0x80) == 0x80) 
                 {
                    SP0828_Data_Output_Disable();
                    hal_HstSendEvent(0xaa00003);hal_HstSendEvent(timer);
                    SP0828_INT_ReadyDisable();hal_DbgAssert("frame miss here");
                    return 0xffffffff;// 
                 }
                #endif
                while(!hal_SpiRxDmaDone(g_spi_id_sensor,g_spi_cs_sensor))
                {
                  //sxr_Sleep(100 MS_WAITING); 
                }  
               // memcpy((UINT8*)buff1, (CONST UINT8*)HAL_SYS_GET_UNCACHED_ADDR(buff1),bufflend);
                hal_SpiClearRxDmaDone(g_spi_id_sensor);
                hal_SpiClearTxDmaDone(g_spi_id_sensor);
                hal_SpiFlushFifos(g_spi_id_sensor);
                buff1 = buff1 + bufflend;
                timer ++;
                //hal_HstSendEvent(timer);
                if(counts == size) break;
                 //hal_HstSendEvent(g_sensor_count);
        		//while(hal_GpioGet(g_sensor_overflow.gpioId)==TRUE)
        		{  
                   // sxr_Sleep(2 MS_WAITING);
                }
               // hal_HstSendEvent(0x1111111); hal_HstSendEvent(g_sensor_count);hal_HstSendEvent(timer);
                while(timer>=g_sensor_count)
                {
                te++;
                }
                
           		
    }	
    SP0828_Data_Output_Disable();SP0828_INT_ReadyDisable();
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CS1, HAL_SPI_PIN_STATE_SPI);
    hal_HstSendEvent(0x38886255); hal_HstSendEvent(timer);
   ret = camerap_ReadReg(0x2e);
   if((ret&0x80) == 0x80)
   {   
       SP0828_Data_Output_Disable();SP0828_INT_ReadyDisable();//hal_DbgAssert("frame miss ");
       hal_HstSendEvent(0xaa00002);
       return 0xffffffff;
       
   }
 
   UINT32 now2 = hal_TimGetUpTime();
   hal_HstSendEvent(0xbb388862);hal_HstSendEvent(g_sensor_count); hal_HstSendEvent(timer);
   hal_HstSendEvent((now2-now)*1000/16384);  
   SXS_TRACE(TSTDOUT, "sensor speed on frame %d ms ",(now2-now)*1000/16384);
   return counts;
}

UINT32 PAS_Cam_ReadFrame(UINT16 * buff, UINT32 size)
{
   return  PAS_Cam_ReadFrame_Data(buff,size);
   // PAS_Cam_ReadFrame_Data(buff,size);
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
}
UINT16 tttt=0;
VOID test_read_serial_camertest()
{
}
PUBLIC BOOL camera_sp0828_RegInit(CAM_DRV_T  * pCamDrv)
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
{}

UINT8 spi_clk = 0;
extern VOID tse_II2c_ldo();

VOID tset_spi_clk()
{}
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
    hal_GpioSet(g_csb_gpo);
    camerap_Delay(5); 
    pmd_EnablePower(PMD_POWER_CAMERA,TRUE); 
    camerap_Delay(5); 
    hal_GpioClr(g_csb_gpo);
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CS1, HAL_SPI_PIN_STATE_SPI);
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_SPI);
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_DO, HAL_SPI_PIN_STATE_SPI);
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_DI, HAL_SPI_PIN_STATE_SPI);
    change_II2c_voltage(TRUE);
    
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
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_CLK, HAL_SPI_PIN_STATE_0);
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_DO, HAL_SPI_PIN_STATE_0);
    hal_SpiForcePin(HAL_SPI,HAL_SPI_PIN_DI, HAL_SPI_PIN_STATE_0);

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


VOID pas6175_Capture_After_Preview(UINT16 Width, UINT16 Height)
{

    SP0828_Capture_Process_AD(Size_240_320);

    //camerap_pas6175_Capture_After_Preview(CAM_NPIX_QVGA,Width,Height);

}
VOID camerap_Capture_After_Preview(UINT16 Width, UINT16 Height)
{

    SP0828_Capture_Process_AD(Size_240_320);

    //camerap_pas6175_Capture_After_Preview(CAM_NPIX_QVGA,Width,Height);

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







