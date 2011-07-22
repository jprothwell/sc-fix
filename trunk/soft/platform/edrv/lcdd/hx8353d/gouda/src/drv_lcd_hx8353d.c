////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL: http://10.10.100.14/svn/developing1/Sources/edrv/trunk/lcdd/ili9340/gouda/src/drv_lcd_ili9340.c $ //
//	$Author: huazeng $                                                        // 
//	$Date: 2010-10-13 14:55:11 +0800 (三, 2010-10-13) $                     //   
//	$Revision: 2773 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file drv_lcd.c                                                           //
/// That file provides an implementation for the HX8347G LCD screen.               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "lcdd_config.h"
#include "lcdd_private.h"
#include "hal_timers.h"
#include "hal_gouda.h"
#include "lcddp_debug.h"

#include "pmd_m.h"
#include "sxr_tls.h"

// To get config
#include "lcdd_tgt_params_gallite.h"
#include "lcdd.h"

// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================
//
// =============================================================================
//  MACROS
// =============================================================================

#define LCM_WR_REG(Addr, Data)  { while(hal_GoudaWriteReg(Addr, Data)!= HAL_ERR_NO);}
#define LCM_WR_DAT(Data)        { while(hal_GoudaWriteData(Data)     != HAL_ERR_NO);}
#define LCM_WR_CMD(Cmd)         { while(hal_GoudaWriteCmd(Cmd)       != HAL_ERR_NO);}

#define LCDD_BUILD_CMD_WR_CMD(c,i,r) do{c[i].isData=FALSE; c[i].value=r;}while(0)
#define LCDD_BUILD_CMD_WR_DAT(c,i,d) do{c[i].isData=TRUE; c[i].value=d;}while(0)
#define LCDD_BUILD_CMD_WR_REG(c,i,r,d) do{LCDD_BUILD_CMD_WR_CMD(c,i,r); LCDD_BUILD_CMD_WR_DAT(c,i+1,d);}while(0)

#define LCDD_TIME_MUTEX_RETRY 50

// =============================================================================
// Screen properties
// =============================================================================

// Number of actual pixels in the display width 
#define LCDD_DISP_X		        128

// Number of pixels in the display height 
#define LCDD_DISP_Y           160


#define  LCD_HX8353D_ID       0x6404

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

PRIVATE CONST LCDD_CONFIG_T g_tgtLcddCfg = TGT_LCDD_CONFIG;

PRIVATE BOOL g_lcddRotate = FALSE;

// wheter lcddp_GoudaBlitHandler() has to close ovl layer 0
PRIVATE BOOL g_lcddAutoCloseLayer = FALSE;

// Sleep status of the LCD
PRIVATE BOOL g_lcddInSleep = FALSE;

// =============================================================================
//  FUNCTIONS
// =============================================================================


// =============================================================================
// lcddp_GoudaBlitHandler
// -----------------------------------------------------------------------------
/// This function frees the lock to access the screen. It is set as the user 
/// handler called by the DMA driver at the end of the writings on the screen.
// =============================================================================
PRIVATE VOID lcddp_GoudaBlitHandler(VOID)
{
    lcdd_MutexFree();
    if(g_lcddAutoCloseLayer)
    {
        hal_GoudaOvlLayerClose(HAL_GOUDA_OVL_LAYER_ID0);
        g_lcddAutoCloseLayer = FALSE;
    }
}


// =============================================================================
// lcddp_Init
// -----------------------------------------------------------------------------
/// This function initializes LCD registers after powering on or waking up.
// =============================================================================
PRIVATE VOID lcddp_Init(VOID)
{
	UINT32 i;
  //hx8353d
	LCM_WR_CMD(0xB9); //SETEXTC
	LCM_WR_DAT(0xFF);
	LCM_WR_DAT(0x83);
	LCM_WR_DAT(0x53);
	
	LCM_WR_CMD(0x2D);     // SET Color
	
	for(i=0;i<32;i++)     // RED 
	{
	   if(i<16) 
	   	{
	      LCM_WR_DAT(2*i); 
	   	}
	   else 
	   	{
	      LCM_WR_DAT(2*i+1); 
	   	}
	}
	
	for(i=0;i<64;i++)     // Green 
		LCM_WR_DAT(1*i); 
	
	for(i=0;i<32;i++)     // BLUE 
	{
	   if(i<16) 
	   	{
	      LCM_WR_DAT(2*i); 
	   	}
	   else 
	   	{
	      LCM_WR_DAT(2*i+1); 
	   	}
	}
	hal_TimDelay(25 MS_WAITING);
	
	LCM_WR_CMD(0xC6);
	LCM_WR_DAT(0x31);    //UADJ
	
	LCM_WR_CMD(0xB1);          //SETPWCTR 
	LCM_WR_DAT(0x00);               //DP_STB 
	LCM_WR_DAT(0x01);               //BT         // VGH=2*DDVDH+VCI //VGL=-2DDVDH 
	  
	LCM_WR_CMD(0xB5);          //SETBGP 
	LCM_WR_DAT(0x0B);             //VVDHS_SEL≧5V 
	LCM_WR_DAT(0x0B);             //VREF = 4.8V 
	  
	LCM_WR_CMD(0xBF);          //SETPTBA 
	LCM_WR_DAT(0x04);             //PTBA[15:8]           // Power driving setting 
	LCM_WR_DAT(0x38);             //PTBA[7:0]             // Power driving setting 
	  
	LCM_WR_CMD(0xC0);          //SETSTBA 
	LCM_WR_DAT(0x18);             //N_OPON              // OP driving 
	LCM_WR_DAT(0x08);             //I_OPON                 // Idle for OP driving 
	LCM_WR_DAT(0x0C);             //STBA[15:8]           // Source Driving setting 
	LCM_WR_DAT(0xC4);             //STBA[7:0]             // Source Driving setting 
	  
	LCM_WR_CMD(0xCC);         //SETPANEL 
	LCM_WR_DAT(0x0C);             //SS、GS、REV、BGR 
	  
	LCM_WR_CMD(0xE3);          //EQ 
	LCM_WR_DAT(0x08);             //VCI_M1 
	LCM_WR_DAT(0x00);             //GND_M1 
	LCM_WR_DAT(0x04);             //VCI_M0 
	LCM_WR_DAT(0x10);             //GND_M0 
	   
	LCM_WR_CMD(0xB6);          //VCOM 
	LCM_WR_DAT(0x9f); //9f //8c           //VMF 
	LCM_WR_DAT(0x5a);  //4f  //78         //VMH 
	LCM_WR_DAT(0x75);  //8f  //64         //VML 
	
	LCM_WR_CMD(0xE0);          //Gamma setting for CPT Panel 
	LCM_WR_DAT(0x00);               //1   
	LCM_WR_DAT(0x74);               //2   
	LCM_WR_DAT(0x71);               //3   
	LCM_WR_DAT(0x0A);             //4   
	LCM_WR_DAT(0xFF);             //5   
	LCM_WR_DAT(0x01);               //6   
	LCM_WR_DAT(0x0C);             //7 
	LCM_WR_DAT(0x0F);              //8   
	LCM_WR_DAT(0x06);               //9   
	LCM_WR_DAT(0x01);               //10 
	LCM_WR_DAT(0x60);               //11 
	LCM_WR_DAT(0x30);               //12 
	LCM_WR_DAT(0x77);               //13 
	LCM_WR_DAT(0x0D);             //14 
	LCM_WR_DAT(0xF0);               //15 
	LCM_WR_DAT(0x0E);             //16 
	LCM_WR_DAT(0x0A);             //17 
	LCM_WR_DAT(0x0E);             //18 
	LCM_WR_DAT(0x0F);              //19 
	
	LCM_WR_CMD(0x36); // Memory Access Control// //BGR 
	LCM_WR_DAT(0x00); 
	LCM_WR_CMD(0x3A);//Interface pixel format
	LCM_WR_DAT(0x05); 
	hal_TimDelay(150 MS_WAITING); 
	
	LCM_WR_CMD(0x11); // SLPOUT
	hal_TimDelay(150 MS_WAITING);
	LCM_WR_CMD(0x29); // display on
	hal_TimDelay(150 MS_WAITING);
	LCM_WR_CMD(0x2C); 
	hal_TimDelay(50 MS_WAITING);
}


// ============================================================================
// lcddp_Open
// ----------------------------------------------------------------------------
/// Open the LCDD driver.
/// It must be called before any call to any other function of this driver.
/// This function is to be called only once.
/// @return #LCDD_ERR_NO or #LCDD_ERR_DEVICE_NOT_FOUND.
// ============================================================================
PRIVATE LCDD_ERR_T lcddp_Open(VOID)
{
    hal_GoudaOpen(&g_tgtLcddCfg.config, g_tgtLcddCfg.timings, 0);

    

    // Init code 
    sxr_Sleep(50 MS_WAITING);                         // Delay 50 ms

    lcddp_Init();

    g_lcddInSleep = FALSE;

    return LCDD_ERR_NO;
}


// ============================================================================
// lcddp_Close
// ----------------------------------------------------------------------------
/// Close the LCDD driver
/// No other functions of this driver should be called after a call to 
/// #lcddp_Close.
/// @return #LCDD_ERR_NO or #LCDD_ERR_DEVICE_NOT_FOUND.
// ============================================================================
PRIVATE LCDD_ERR_T lcddp_Close(VOID)
{
    
    hal_GoudaClose();

    return LCDD_ERR_NO;
}


// ============================================================================
// lcddp_SetContrast
// ----------------------------------------------------------------------------
/// Set the contrast of the 'main'LCD screen.
/// @param contrast Value to set the contrast to.
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED or 
/// #LCDD_ERR_INVALID_PARAMETER.
// ============================================================================
PRIVATE LCDD_ERR_T lcddp_SetContrast(UINT32 contrast)
{
    //#warning This function is not implemented yet
    return LCDD_ERR_NO;
}


// ============================================================================
// lcddp_SetStandbyMode
// ----------------------------------------------------------------------------
/// Set the main LCD in standby mode or exit from it
/// @param standbyMode If \c TRUE, go in standby mode. 
///                    If \c FALSE, cancel standby mode. 
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED or 
/// #LCDD_ERR_INVALID_PARAMETER.
// ============================================================================
PRIVATE LCDD_ERR_T lcddp_SetStandbyMode(BOOL standbyMode)
{
    if (standbyMode)
    {
        lcddp_Sleep();
    }
    else
    {
        lcddp_WakeUp();
    }
    return LCDD_ERR_NO;
}


// ============================================================================
// lcddp_Sleep
// ----------------------------------------------------------------------------
/// Set the main LCD screen in sleep mode.
/// @return #LCDD_ERR_NO or #LCDD_ERR_NOT_OPENED
// ============================================================================

PRIVATE LCDD_ERR_T lcddp_Sleep(VOID)
{
    while (0 == lcdd_MutexGet())
    {
        sxr_Sleep(LCDD_TIME_MUTEX_RETRY);
        LCDD_TRACE(TSTDOUT, 0, "LCDD: Sleep while another LCD operation in progress. Sleep %d ticks",
            LCDD_TIME_MUTEX_RETRY);
    }

    if (g_lcddInSleep)
    {
        lcdd_MutexFree();
        return LCDD_ERR_NO;
    }

		LCM_WR_CMD(0x10); //sleep in
		hal_TimDelay(10 MS_WAITING);
    LCDD_TRACE(TSTDOUT, 0, "lcddp_Sleep: calling hal_GoudaClose");
    hal_GoudaClose();

    g_lcddInSleep = TRUE;

    lcdd_MutexFree();

    return LCDD_ERR_NO;
}


// ============================================================================
// lcddp_PartialOn
// ----------------------------------------------------------------------------
/// Set the Partial mode of the LCD
/// @param vsa : Vertical Start Active
/// @param vea : Vertical End Active
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED
// ============================================================================
PRIVATE LCDD_ERR_T lcddp_PartialOn(UINT16 vsa, UINT16 vea)
{
    return LCDD_ERR_NO;
}


// ============================================================================
// lcddp_PartialOff
// ----------------------------------------------------------------------------
/// return to Normal Mode
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED
// ============================================================================
PRIVATE LCDD_ERR_T lcddp_PartialOff(VOID)
{
    return LCDD_ERR_NO;
}


// ============================================================================
// lcddp_WakeUp
// ----------------------------------------------------------------------------
/// Wake the main LCD screen out of sleep mode
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED
// ============================================================================
PRIVATE LCDD_ERR_T lcddp_WakeUp(VOID)
{
    if (0 == lcdd_MutexGet())
    {
        sxr_Sleep(LCDD_TIME_MUTEX_RETRY);
        LCDD_TRACE(TSTDOUT, 0, "LCDD: Wakeup while another LCD operation in progress. Sleep %d ticks",
            LCDD_TIME_MUTEX_RETRY);
    }

    if (!g_lcddInSleep)
    {
        lcdd_MutexFree();
        return LCDD_ERR_NO;
    }

    LCDD_TRACE(TSTDOUT, 0, "lcddp_WakeUp: calling hal_GoudaOpen");
    hal_GoudaOpen(&g_tgtLcddCfg.config, g_tgtLcddCfg.timings, 0);

    sxr_Sleep(50 MS_WAITING);                         // Delay 50 ms

    lcddp_Init();

    g_lcddInSleep = FALSE;

    lcdd_MutexFree();

    // Set a comfortable background color to avoid screen flash
    LCDD_FBW_T frameBufferWin;
    frameBufferWin.fb.buffer = NULL;
    frameBufferWin.fb.colorFormat = LCDD_COLOR_FORMAT_RGB_565;
    frameBufferWin.roi.x=0;
    frameBufferWin.roi.y=0;

    if (g_lcddRotate)
    {
        frameBufferWin.roi.width = LCDD_DISP_Y;
        frameBufferWin.roi.height = LCDD_DISP_X;
        frameBufferWin.fb.width = LCDD_DISP_Y;
        frameBufferWin.fb.height = LCDD_DISP_X;
    }
    else
    {
        frameBufferWin.roi.width = LCDD_DISP_X;
        frameBufferWin.roi.height = LCDD_DISP_Y;
        frameBufferWin.fb.width = LCDD_DISP_X;
        frameBufferWin.fb.height = LCDD_DISP_Y;
    }
    lcddp_Blit16(&frameBufferWin,frameBufferWin.roi.x,frameBufferWin.roi.y);   
  

    return LCDD_ERR_NO;
}


// ============================================================================
// lcddp_GetScreenInfo
// ----------------------------------------------------------------------------
/// Get information about the main LCD device.
/// @param screenInfo Pointer to the structure where the information
/// obtained will be stored
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED or 
/// #LCDD_ERR_INVALID_PARAMETER.
// ============================================================================
PRIVATE LCDD_ERR_T lcddp_GetScreenInfo(LCDD_SCREEN_INFO_T* screenInfo)
{

   screenInfo->width = LCDD_DISP_X;
   screenInfo->height = LCDD_DISP_Y;
   screenInfo->bitdepth = LCDD_COLOR_FORMAT_RGB_565;
   screenInfo->nReserved = 0;
   return LCDD_ERR_NO;
}


// ============================================================================
// lcddp_SetPixel16
// ----------------------------------------------------------------------------
/// Draw a 16-bit pixel a the specified position.
/// @param x X coordinate of the point to set.
/// @param y Y coordinate of the point to set.
/// @param pixelData 16-bit pixel data to draw.
/// @return #LCDD_ERR_NO, #LCDD_ERR_RESOURCE_BUSY or #LCDD_ERR_NOT_OPENED.
// ============================================================================
PRIVATE LCDD_ERR_T lcddp_SetPixel16(UINT16 x, UINT16 y, UINT16 pixelData)
{
    if (0 == lcdd_MutexGet())
    {
        return LCDD_ERR_RESOURCE_BUSY;
    }
    else
    {
        if (g_lcddInSleep)
        {
            lcdd_MutexFree();
            return LCDD_ERR_NO;
        }

				LCM_WR_CMD(0x2A);
				LCM_WR_DAT(0x00);
				LCM_WR_DAT(x);
				LCM_WR_DAT(0x00);
				LCM_WR_DAT(x);
				
				LCM_WR_CMD(0x2B);
				LCM_WR_DAT(0x00);
				LCM_WR_DAT(y);
				LCM_WR_DAT(0x00);
				LCM_WR_DAT(y);
								
 				LCM_WR_CMD(0x2C);
 				LCM_WR_DAT(pixelData&0x00ff);
 				LCM_WR_DAT((pixelData>>8)&0x00ff);

        lcdd_MutexFree();              
        return LCDD_ERR_NO;
    }
}


// ============================================================================
// lcddp_BlitRoi2Win
// ----------------------------------------------------------------------------
// Private function to transfer data to the LCD
// ============================================================================
PRIVATE VOID lcddp_BlitRoi2Win(CONST HAL_GOUDA_WINDOW_T* pRoi, CONST HAL_GOUDA_WINDOW_T* pActiveWin)
{
    HAL_GOUDA_LCD_CMD_T cmd[1];

    if(!((pRoi->tlPX < pRoi->brPX) && (pRoi->tlPY < pRoi->brPY)))
    {
        LCDD_TRACE(LCDD_WARN_TRC,0,"lcddp_BlitRoi2Win: Invalid Roi x:%d < %d, y:%d < %d",pRoi->tlPX, pRoi->brPX, pRoi->tlPY, pRoi->brPY);
        lcddp_GoudaBlitHandler();
        return;
    }

    // building set roi sequence:
		LCM_WR_CMD(0x2A);
		LCM_WR_DAT(0x00);
		LCM_WR_DAT(pActiveWin->tlPX);
		LCM_WR_DAT(0x00);
		LCM_WR_DAT(pActiveWin->brPX);
				
		LCM_WR_CMD(0x2B);
		LCM_WR_DAT(0x00);
		LCM_WR_DAT(pActiveWin->tlPY);
		LCM_WR_DAT(0x00);
		LCM_WR_DAT(pActiveWin->brPY);

 		LCM_WR_CMD(0x2C);
    
    // Send command after which the data we sent
    // are recognized as pixels.
    while(HAL_ERR_NO != hal_GoudaBlitRoi(pRoi, 0, cmd, lcddp_GoudaBlitHandler));
}


// ============================================================================
// lcddp_FillRect16
// ----------------------------------------------------------------------------
/// This function performs a fill of the active window  with some color.
/// @param bgColor Color with which to fill the rectangle. It is a 16-bit
/// data, as the one of #lcddp_SetPixel16
/// @return #LCDD_ERR_NO, #LCDD_ERR_RESOURCE_BUSY or #LCDD_ERR_NOT_OPENED.
// ============================================================================
PRIVATE LCDD_ERR_T lcddp_FillRect16(CONST LCDD_ROI_T* regionOfInterrest, UINT16 bgColor)
{
    // Active window coordinates.
    HAL_GOUDA_WINDOW_T activeWin;

    if (0 == lcdd_MutexGet())
    {
        return LCDD_ERR_RESOURCE_BUSY;
    }
    else
    {   
        if (g_lcddInSleep)
        {
            lcdd_MutexFree();
            return LCDD_ERR_NO;
        }
        
        // Set Active window
        activeWin.tlPX = regionOfInterrest->x;
        activeWin.brPX = regionOfInterrest->x + regionOfInterrest->width - 1;
        activeWin.tlPY = regionOfInterrest->y;
        activeWin.brPY = regionOfInterrest->y + regionOfInterrest->height - 1;

        // Check parameters
        // ROI must be within the screen boundary
        BOOL badParam = FALSE;
        if (g_lcddRotate)
        {
            if (    (activeWin.tlPX >= LCDD_DISP_Y) ||
                    (activeWin.brPX >= LCDD_DISP_Y) ||
                    (activeWin.tlPY >= LCDD_DISP_X) ||
                    (activeWin.brPY >= LCDD_DISP_X)
               )
            {
                badParam = TRUE;
            }
        }
        else
        {
            if (    (activeWin.tlPX >= LCDD_DISP_X) ||
                    (activeWin.brPX >= LCDD_DISP_X) ||
                    (activeWin.tlPY >= LCDD_DISP_Y) ||
                    (activeWin.brPY >= LCDD_DISP_Y)
               )
            {
                badParam = TRUE;
            }
        }
        if (badParam)
        {
            lcdd_MutexFree();               
            return LCDD_ERR_INVALID_PARAMETER;
        }

        hal_GoudaSetBgColor(bgColor);
        lcddp_BlitRoi2Win(&activeWin,&activeWin);

        return LCDD_ERR_NO;
    }
}


// ============================================================================
// lcddp_Blit16
// ----------------------------------------------------------------------------
/// This function provides the basic bit-block transfer capabilities. 
/// This function copies the data (such as characters/bmp) on the LCD directly
/// as a (rectangular) block. The data is drawn in the active window.
/// The buffer has to be properly aligned (@todo define properly 'properly')
///
/// @param pPixelData Pointer to the buffer holding the data to be displayed
/// as a block. The dimension of this block are the one of the #pDestRect 
/// parameter
/// @return #LCDD_ERR_NO, #LCDD_ERR_RESOURCE_BUSY or #LCDD_ERR_NOT_OPENED.
// ============================================================================
PRIVATE LCDD_ERR_T lcddp_Blit16(CONST LCDD_FBW_T* frameBufferWin, UINT16 startX, UINT16 startY)
{
    LCDD_ASSERT((frameBufferWin->fb.width&1) == 0, "LCDD: FBW must have an even number "
            "of pixels per line. Odd support is possible at the price of a huge "
            "performance lost");
    // Active window coordinates.
    HAL_GOUDA_WINDOW_T inputWin;
    HAL_GOUDA_WINDOW_T activeWin;

    if (0 == lcdd_MutexGet())
    {
        return LCDD_ERR_RESOURCE_BUSY;
    }
    else
    {
        if (g_lcddInSleep)
        {
            lcdd_MutexFree();
            return LCDD_ERR_NO;
        }

        // Set Input window
        inputWin.tlPX = frameBufferWin->roi.x;
        inputWin.brPX = frameBufferWin->roi.x + frameBufferWin->roi.width - 1;
        inputWin.tlPY = frameBufferWin->roi.y;
        inputWin.brPY = frameBufferWin->roi.y + frameBufferWin->roi.height - 1;

        // Set Active window
        activeWin.tlPX = startX;
        activeWin.brPX = startX + frameBufferWin->roi.width - 1;
        activeWin.tlPY = startY;
        activeWin.brPY = startY + frameBufferWin->roi.height - 1;

        // Check parameters
        // ROI must be within the screen boundary
        // ROI must be within the Frame buffer
        // Color format must be 16 bits
        BOOL badParam = FALSE;
        if (g_lcddRotate)
        {
            if (    (activeWin.tlPX >= LCDD_DISP_Y) ||
                    (activeWin.brPX >= LCDD_DISP_Y) ||
                    (activeWin.tlPY >= LCDD_DISP_X) ||
                    (activeWin.brPY >= LCDD_DISP_X)
               )
            {
                badParam = TRUE;
            }
        }
        else
        {
            if (    (activeWin.tlPX >= LCDD_DISP_X) ||
                    (activeWin.brPX >= LCDD_DISP_X) ||
                    (activeWin.tlPY >= LCDD_DISP_Y) ||
                    (activeWin.brPY >= LCDD_DISP_Y)
               )
            {
                badParam = TRUE;
            }
        }
        if (!badParam)
        {
            if (    (frameBufferWin->roi.width > frameBufferWin->fb.width) ||
                    (frameBufferWin->roi.height > frameBufferWin->fb.height) ||
                    (frameBufferWin->fb.colorFormat != LCDD_COLOR_FORMAT_RGB_565)
                )
            {
                badParam = TRUE;;
            }
        }
        if (badParam)
        {
            lcdd_MutexFree();  
            return LCDD_ERR_INVALID_PARAMETER;
        }

        // this will allow to keep LCDD interface for blit while using gouda
        // directly for configuring layers
        if (frameBufferWin->fb.buffer != NULL)
        {
            HAL_GOUDA_OVL_LAYER_DEF_T def;
            // configure ovl layer 0 as buffer
            def.addr = (UINT32*)frameBufferWin->fb.buffer; // what about aligment ?
            def.fmt = HAL_GOUDA_IMG_FORMAT_RGB565; //TODO convert from .colorFormat
            //def.stride = frameBufferWin->fb.width * 2;
            def.stride = 0; // let hal gouda decide
            def.pos.tlPX = 0;
            def.pos.tlPY = 0;
            def.pos.brPX = frameBufferWin->fb.width - 1;
            def.pos.brPY = frameBufferWin->fb.height - 1;
            def.alpha = 255;
            def.cKeyEn = FALSE;

            // open the layer
            hal_GoudaOvlLayerClose(HAL_GOUDA_OVL_LAYER_ID0);
            hal_GoudaOvlLayerOpen(HAL_GOUDA_OVL_LAYER_ID0, &def);
            // tell the end handler not to close the layer when we are done
            g_lcddAutoCloseLayer = FALSE;
        }

        // gouda is doing everything ;)
        lcddp_BlitRoi2Win(&inputWin, &activeWin);

        return LCDD_ERR_NO;
    }
}


// ============================================================================
// lcddp_Busy
// ----------------------------------------------------------------------------
/// This function is not implemented for the ebc version of the driver
// ============================================================================
PRIVATE BOOL lcddp_Busy(VOID)
{
    return FALSE;
}


// ============================================================================
// lcddp_SetDirRotation
// ----------------------------------------------------------------------------
///  
// ============================================================================
PRIVATE BOOL lcddp_SetDirRotation(VOID)
{
    while (0 == lcdd_MutexGet())
    {
        sxr_Sleep(LCDD_TIME_MUTEX_RETRY);
    }
    
    g_lcddRotate = TRUE;
    
    if (g_lcddInSleep)
    {
        lcdd_MutexFree();
        return TRUE;
    }

    LCM_WR_CMD(0x36);    //rotate 90
    LCM_WR_DAT(0x60);

    lcdd_MutexFree();              

    return TRUE;
}

// ============================================================================
// lcddp_SetDirDefault
// ----------------------------------------------------------------------------
///  
// ============================================================================
PRIVATE BOOL lcddp_SetDirDefault(VOID)
{
    while (0 == lcdd_MutexGet())
    {
        sxr_Sleep(LCDD_TIME_MUTEX_RETRY);
    }
    
    g_lcddRotate = FALSE;
    
    if (g_lcddInSleep)
    {
        lcdd_MutexFree();
        return TRUE;
    }

    LCM_WR_CMD(0x36);   //no rotate
    LCM_WR_DAT(0x00);

    lcdd_MutexFree();              

    return TRUE;
}

PRIVATE char* lcdd_get_id_string(void)
{
   static char hx8353d_id_str[] = "hx8353d\n"; 
   return hx8353d_id_str; 
}
PRIVATE BOOL lcddp_CheckProductId()
{
    UINT16 productId;
    LCDD_CONFIG_T lcddReadConfig=LCDD_READ_CONFIG;

    hal_GoudaOpen(&lcddReadConfig.config, lcddReadConfig.timings, 0);
    sxr_Sleep(20 MS_WAITING);

    hal_GoudaReadReg(0xD0, &productId);
    hal_GoudaClose();
    SXS_TRACE(TSTDOUT, "hx8353d(0x%x): lcd read id is 0x%x ", LCD_HX8353D_ID, productId);

    if(productId == LCD_HX8353D_ID)
        return TRUE;
    else
        return FALSE;
}

// ============================================================================
// lcdd_hx8347g_RegInit
// ----------------------------------------------------------------------------
/// register the right lcd driver, according to lcddp_CheckProductId
/// @return #TRUE, #FALSE
// ============================================================================
PUBLIC BOOL  lcdd_hx8353d_RegInit(LCDD_REG_T *pLcdDrv)
{  
    if( lcddp_CheckProductId())
    {
        pLcdDrv->lcdd_Open=lcddp_Open;
        pLcdDrv->lcdd_Close=lcddp_Close;
        pLcdDrv->lcdd_SetContrast=lcddp_SetContrast;
        pLcdDrv->lcdd_SetStandbyMode=lcddp_SetStandbyMode;
        pLcdDrv->lcdd_PartialOn=lcddp_PartialOn;
        pLcdDrv->lcdd_PartialOff=lcddp_PartialOff;
        pLcdDrv->lcdd_Blit16=lcddp_Blit16;
        pLcdDrv->lcdd_Busy=lcddp_Busy;
        pLcdDrv->lcdd_FillRect16=lcddp_FillRect16;
        pLcdDrv->lcdd_GetScreenInfo=lcddp_GetScreenInfo;
        pLcdDrv->lcdd_WakeUp=lcddp_WakeUp;
        pLcdDrv->lcdd_SetPixel16=lcddp_SetPixel16;
        pLcdDrv->lcdd_Sleep=lcddp_Sleep;
        pLcdDrv->lcdd_SetDirRotation=lcddp_SetDirRotation;
        pLcdDrv->lcdd_SetDirDefault=lcddp_SetDirDefault;		
        pLcdDrv->lcdd_GetStringId=lcdd_get_id_string;

        return TRUE;
    }

    return FALSE;
}


