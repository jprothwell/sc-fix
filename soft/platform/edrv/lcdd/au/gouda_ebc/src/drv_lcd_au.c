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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/lcdd/au/gouda_ebc/src/drv_lcd_au.c $ //
//	$Author: pengzg $                                                        // 
//	$Date: 2011-04-22 14:11:23 +0800 (Fri, 22 Apr 2011) $                     //   
//	$Revision: 7192 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file drv_lcd.c                                                           //
/// That file provides an implementation for the AU LCD screen.               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "hal_ebc.h"
#include "hal_pwm.h"
#include "hal_sys.h"
#include "hal_timers.h"
#include "hal_dma.h"
#include "hal_gouda.h"

#include "pmd_m.h"

#include "lcdd_config.h"
#include "lcdd_m.h"
#include "lcddp_debug.h"

#include "drv_lcd.h"

#include "sxr_tls.h"

// To get config
#include "tgt_lcdd_cfg.h"

// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================


// =============================================================================
//  MACROS
// =============================================================================

#define EXP2(n) (1<<(n))

//#define LCM_REG                 *(volatile UINT16*)(g_lcddCsBase)
//#define LCM_DAT                 *(volatile UINT16*)(g_lcddCsBase + g_lcddCsOffset)
//#define LCM_WR_REG(Addr, Data)  LCM_REG = (UINT16)Addr; LCM_DAT = (UINT16)Data
//#define LCM_RD_REG(Addr)        (UINT16)(LCM_REG = (UINT16)Addr; LCM_DAT)
#define LCM_SUB_REG             *(volatile UINT16*)(g_lcddCsSubBase)
#define LCM_SUB_DAT             *(volatile UINT16*)(g_lcddCsSubBase + g_lcddCsSubOffset)



#define LCM_WR_REG(Addr, Data)  { while(hal_GoudaWriteReg(Addr, Data)!= HAL_ERR_NO);}
#define LCM_WR_DAT(Data)        { while(hal_GoudaWriteData(Data)     != HAL_ERR_NO);}
#define LCM_WR_CMD(Cmd)         { while(hal_GoudaWriteCmd(Cmd)       != HAL_ERR_NO);}

#define LCDD_BUILD_CMD_WR_CMD(c,i,r) do{c[i].isData=FALSE; c[i].value=r;}while(0)
#define LCDD_BUILD_CMD_WR_DAT(c,i,d) do{c[i].isData=TRUE; c[i].value=d;}while(0)
#define LCDD_BUILD_CMD_WR_REG(c,i,r,d) do{LCDD_BUILD_CMD_WR_CMD(c,i,r); LCDD_BUILD_CMD_WR_DAT(c,i+1,d);}while(0)


// =============================================================================
// Screen properties
// =============================================================================
// Number of actual pixels in the display width 
#define LCDD_DISP_X		128 

// Number of pixels in the display height 
#define LCDD_DISP_Y		160 


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

/// LCDD both screen config.
PRIVATE CONST TGT_LCDD_CONFIG_T* g_lcddConfig;

/// Main screen base address
PRIVATE UINT32 g_lcddCsBase = 0;

/// Main screen offset address to access the second register.
PRIVATE UINT32 g_lcddCsOffset = 0;

/// Subscreen base address
/// Let public to be usable by the util driver
 UINT32 g_lcddCsSubBase = 0;

/// Sub screen offset address to access the second register.
PRIVATE UINT32 g_lcddCsSubOffset = 0;

/// Structure holding the active window status for the 
PRIVATE LCDD_ACTIVE_WIN_STATUS_T   g_lcddSubActiveWin;

/// Lock to protect the access to the LCD screen during a DMA access.
/// When 1, access is allowed. When 0, it is denied.
PRIVATE UINT32 g_lcddLock = 0;

/// Lock to protect the access to the LCD SUB screen during a DMA access.
/// When 1, access is allowed. When 0, it is denied.
PRIVATE UINT32 g_lcddSubLock = 0;



HAL_GOUDA_LCD_CONFIG_T g_GoudaConfig = {
    {.cs            =   HAL_GOUDA_LCD_MEMORY_IF,
    .outputFormat   =   HAL_GOUDA_LCD_OUTPUT_FORMAT_16_bit_RGB565,
    .cs0Polarity    =   FALSE,
    .cs1Polarity    =   FALSE,
    .rsPolarity     =   FALSE,
    .wrPolarity     =   FALSE,
    .rdPolarity     =   FALSE}};

#define LCDD_TIMING_GOUDA {                                             \
    {.tas       =  0,                                                   \
    .tah        =  1,                                                   \
    .pwl        =  2,                                                   \
    .pwh        =  2}}                                                  \


CONST HAL_GOUDA_LCD_TIMINGS_T g_GoudaTimings[HAL_GOUDA_LCD_TIMINGS_QTY] = {
    LCDD_TIMING_GOUDA,LCDD_TIMING_GOUDA,LCDD_TIMING_GOUDA,LCDD_TIMING_GOUDA,
    LCDD_TIMING_GOUDA,LCDD_TIMING_GOUDA,LCDD_TIMING_GOUDA,LCDD_TIMING_GOUDA };



/// wheter lcdd_GoudaBlitHandler() has to close ovl layer 0
PRIVATE BOOL g_lcddAutoCloseLayer = FALSE;

// =============================================================================
//  FUNCTIONS
// =============================================================================

// =============================================================================
// lcdd_MutexFree
// -----------------------------------------------------------------------------
/// Free a previously taken mutex. The ownership of the mutex is not checked.
/// Only free a mutex you have previously got.
/// @param mutex Pointer to the integer used as a mutex.
// =============================================================================
INLINE VOID lcdd_MutexFree(UINT32* mutex)
{
    // Writing is an atomic operation
    *mutex = 1;
}



// =============================================================================
// lcdd_MutexGet
// -----------------------------------------------------------------------------
/// This function enter in critical section, read the value of the mutex and,
/// if this is a '1', returns '1' and set the mutex to 0. If this is a '0',
/// it does nothing. Then, in both cases, it exists the Critical Section.
///
/// @param mutex Pointer to the integer used as a mutex.
/// @return '1' if the mutex was taken, '0' otherwise.
// =============================================================================
PRIVATE UINT32 lcdd_MutexGet(UINT32* mutex)
{
    UINT32 status;
    UINT32 result;

    status = hal_SysEnterCriticalSection();
    if ((*mutex) == 1)
    {
        // Take the mutex
        *mutex = 0;
        result = 1;
    }
    else
    {
        // Too bad ...
        result = 0;
    }

    hal_SysExitCriticalSection(status);
    return result;
}

// =============================================================================
// lcdd_GoudaBlitHandler
// -----------------------------------------------------------------------------
/// This function frees the lock to access the screen. It is set as the user 
/// handler called by the DMA driver at the end of the writings on the screen.
// =============================================================================
PRIVATE VOID lcdd_GoudaBlitHandler(VOID)
{
    lcdd_MutexFree(&g_lcddLock);
    if(g_lcddAutoCloseLayer)
    {
        hal_GoudaOvlLayerClose(HAL_GOUDA_OVL_LAYER_ID0);
        g_lcddAutoCloseLayer = FALSE;
    }
}

// ============================================================================
// lcdd_Open
// ----------------------------------------------------------------------------
/// Open the LCDD driver.
/// It must be called before any call to any other function of this driver.
/// This function is to be called only once.
/// @return #LCDD_ERR_NO or #LCDD_ERR_DEVICE_NOT_FOUND.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_Open(VOID)
{
    g_lcddConfig = tgt_GetLcddConfig();
    g_lcddCsBase     = (UINT32) hal_EbcCsOpen(g_lcddConfig->main.csNum, &g_lcddConfig->main.csConfig);
    g_lcddCsOffset = EXP2(g_lcddConfig->main.rsLine+1);
    LCDD_ASSERT(g_lcddConfig->main.rsLine == 0, "lcdd_Open: main.rsLine = 0 is the only configuration possible with gouda (got %d)", g_lcddConfig->main.rsLine);
    
    hal_GoudaOpen(&g_GoudaConfig, g_GoudaTimings, g_lcddCsBase);

    if (0 == g_lcddCsBase)
    {
        return LCDD_ERR_DEVICE_NOT_FOUND;
    }
    else
    {
        // Turn off backlight so that we don't display anything wrong
        
        hal_SysResetOut(FALSE);
        // Allow access 
        g_lcddLock = 1;
        lcdd_PowerOn();
        return LCDD_ERR_NO;
    }
}



// ============================================================================
// lcdd_Close
// ----------------------------------------------------------------------------
/// Close the LCDD driver
/// No other functions of this driver should be called after a call to 
/// #lcdd_Close.
/// @return #LCDD_ERR_NO or #LCDD_ERR_DEVICE_NOT_FOUND.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_Close(VOID)
{
    if (0 == g_lcddCsBase)
    {
        return LCDD_ERR_DEVICE_NOT_FOUND;
    }
    else
    {
        // Turn off backlight 
        
        hal_SysResetOut(TRUE);
        hal_EbcCsClose(HAL_EBC_CS2);
        return LCDD_ERR_NO;
    }
}



// ============================================================================
// lcdd_SetContrast
// ----------------------------------------------------------------------------
/// Set the contrast of the 'main'LCD screen.
/// @param contrast Value to set the contrast to.
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED or 
/// #LCDD_ERR_INVALID_PARAMETER.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SetContrast(UINT32 contrast)
{    
    if (0 == g_lcddCsBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {
        return LCDD_ERR_NO;
    }
    
    // TODO: implement.
    LCDD_ASSERT(FALSE, "This function may not be implemented yet\n");
}



// ============================================================================
// lcdd_SetStandbyMode
// ----------------------------------------------------------------------------
/// Set the main LCD in standby mode or exit from it
/// @param standbyMode If \c TRUE, go in standby mode. 
///                    If \c FALSE, cancel standby mode. 
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED or 
/// #LCDD_ERR_INVALID_PARAMETER.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SetStandbyMode(BOOL standbyMode)
{
    if (0 == g_lcddCsBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {
        if (standbyMode)
        {
            // Go into standby mode
        }
        else
        {
            // Power on ?
            lcdd_PowerOn();
        }

        return LCDD_ERR_NO;
    }
    
    // TODO: implement.
    LCDD_ASSERT(FALSE, "This function may not be implemented yet\n");
}


// ============================================================================
// lcdd_Sleep
// ----------------------------------------------------------------------------
/// Set the main LCD screen in sleep mode.
/// @return #LCDD_ERR_NO or #LCDD_ERR_NOT_OPENED
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_Sleep(VOID)
{
    if (0 == g_lcddCsBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {
        return LCDD_ERR_NO;
    }
    
    // TODO: implement.
    LCDD_ASSERT(FALSE, "This function may not be implemented yet\n");
}



// ============================================================================
// lcdd_WakeUp
// ----------------------------------------------------------------------------
/// Wake the main LCD screen out of sleep mode
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_WakeUp(VOID)
{
    if (0 == g_lcddCsBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {
    	lcdd_DisplayOn();
        return LCDD_ERR_NO;
    }
    
    // TODO: implement.
    LCDD_ASSERT(FALSE, "This function may not be implemented yet\n");
}


// ============================================================================
// lcdd_GetScreenInfo
// ----------------------------------------------------------------------------
/// Get information about the main LCD device.
/// @param screenInfo Pointer to the structure where the information
/// obtained will be stored
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED or 
/// #LCDD_ERR_INVALID_PARAMETER.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_GetScreenInfo(LCDD_SCREEN_INFO_T* screenInfo)
{
    if (0 == g_lcddCsBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {
        screenInfo->width = LCDD_DISP_X;
        screenInfo->height = LCDD_DISP_Y;
        screenInfo->bitdepth = LCDD_COLOR_FORMAT_RGB_565;
        screenInfo->nReserved = 0;
        return LCDD_ERR_NO;
    }
}



// =============================================================================
// lcdd_PowerOn
// -----------------------------------------------------------------------------
/// Switch ON the LCD screen power
// =============================================================================
PRIVATE VOID lcdd_PowerOn(VOID)
{
// Power On Sequence
	// Delay 20ms
	sxr_Sleep(20 MS_WAITING);
	LCM_WR_REG(0x07, 0x0004);
	LCM_WR_REG(0x0d, 0x0203);
	LCM_WR_REG(0x0e, 0x1115);
	// Delay 20ms
	sxr_Sleep(20 MS_WAITING);
	LCM_WR_REG(0x01, 0x0113);
	LCM_WR_REG(0x02, 0x0700);
	LCM_WR_REG(0x05, 0x1230);
	LCM_WR_REG(0x06, 0x0000);
	LCM_WR_REG(0x0b, 0x4000);
	LCM_WR_REG(0x0c, 0x0003);
	LCM_WR_REG(0x03, 0x0488);
	LCM_WR_REG(0x04, 0x0000);
	// Delay 40ms
	sxr_Sleep(40 MS_WAITING);
	LCM_WR_REG(0x0e, 0x3115);
	LCM_WR_REG(0x0d, 0x0213);
	LCM_WR_REG(0x21, 0x0000);
	LCM_WR_REG(0x30, 0x0000);
	LCM_WR_REG(0x31, 0x0205);
	LCM_WR_REG(0x32, 0x0202);
	LCM_WR_REG(0x33, 0x0000);
	LCM_WR_REG(0x34, 0x0504);
	LCM_WR_REG(0x35, 0x0205);
	LCM_WR_REG(0x36, 0x0707);
	LCM_WR_REG(0x37, 0x0000);
	LCM_WR_REG(0x0f, 0x0000);
	LCM_WR_REG(0x11, 0x0000);
	LCM_WR_REG(0x14, 0x9f00);
	LCM_WR_REG(0x15, 0x0000);
	LCM_WR_REG(0x16, 0x7f00);
	LCM_WR_REG(0x17, 0x9f00);
	LCM_WR_REG(0x3a, 0x1000);
	LCM_WR_REG(0x3b, 0x000b);
	LCM_WR_REG(0x07, 0x0005);
	// Delay 20ms
	sxr_Sleep(20 MS_WAITING);
	LCM_WR_REG(0x07, 0x0025);
	LCM_WR_REG(0x07, 0x0027);
	LCM_WR_REG(0x07, 0x0037);
	LCM_WR_REG(0x21, 0x0000);

	lcdd_DisplayOn();
}


// =============================================================================
// lcdd_DisplayOn
// -----------------------------------------------------------------------------
/// Switch on the display of the LCD screen 
// =============================================================================
PRIVATE VOID lcdd_DisplayOn(VOID)
{
	LCM_WR_REG(0x07, 0x0005);
	// Delay 20ms
	sxr_Sleep(20 MS_WAITING);
	LCM_WR_REG(0x07, 0x0025);
	LCM_WR_REG(0x07, 0x0027);
	// Delay 20ms
	sxr_Sleep(20 MS_WAITING);
	LCM_WR_REG(0x07, 0x0037);
}



// ============================================================================
// lcdd_SetPixel16
// ----------------------------------------------------------------------------
/// Draw a 16-bit pixel a the specified position.
/// @param x X coordinate of the point to set.
/// @param y Y coordinate of the point to set.
/// @param pixelData 16-bit pixel data to draw.
/// @return #LCDD_ERR_NO, #LCDD_ERR_RESOURCE_BUSY or #LCDD_ERR_NOT_OPENED.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SetPixel16(UINT16 x, UINT16 y, UINT16 pixelData)
{
    if (0 == g_lcddCsBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    { 
        if (0 == lcdd_MutexGet(&g_lcddLock))
        {
            return LCDD_ERR_RESOURCE_BUSY;
        }
        else
        {

            LCM_WR_REG(0x21, (y << 8) | x);
            LCM_WR_REG(0x22, pixelData);
            lcdd_MutexFree(&g_lcddLock);
            return LCDD_ERR_NO;
        }
    }
}

// ============================================================================
// lcdd_BlitRoi2Win
// ----------------------------------------------------------------------------
// Private function to transfer data to the LCD
// ============================================================================
PROTECTED VOID lcdd_BlitRoi2Win(CONST HAL_GOUDA_WINDOW_T* pRoi, CONST HAL_GOUDA_WINDOW_T* pActiveWin)
{
    HAL_GOUDA_LCD_CMD_T cmd[7];
    // building set roi sequence:

#if 1
    //Window Horizontal RAM Address End<<8, Start
    LCDD_BUILD_CMD_WR_REG(cmd,0,0x16, (pActiveWin->brPX << 8) | pActiveWin->tlPX);
    //Window Vertical RAM Address End<<8, Start
    LCDD_BUILD_CMD_WR_REG(cmd,2,0x17, (pActiveWin->brPY << 8) | pActiveWin->tlPY);

    //Start point
    LCDD_BUILD_CMD_WR_REG(cmd,4,0x21, ((pActiveWin->tlPY) << 8) | pActiveWin->tlPX);
        
    // Send command after which the data we sent
    // are recognized as pixels.
    LCDD_BUILD_CMD_WR_CMD(cmd,6,0x0022);
    while(HAL_ERR_NO != hal_GoudaBlitRoi(pRoi, 7, cmd, lcdd_GoudaBlitHandler));
#else
    //Window Horizontal RAM Address End<<8, Start
    LCM_WR_REG(0x16, (pActiveWin->brPX << 8) | pActiveWin->tlPX);
    //Window Vertical RAM Address End<<8, Start
    LCM_WR_REG(0x17, (pActiveWin->brPY << 8) | pActiveWin->tlPY);
    // Set initial point
    LCM_WR_REG(0x21, ((pActiveWin->tlPY) << 8) | pActiveWin->tlPX);

    LCDD_BUILD_CMD_WR_CMD(cmd,0,0x0022);


    while(HAL_ERR_NO != hal_GoudaBlitRoi(pRoi, 1, cmd, lcdd_GoudaBlitHandler));
#endif
}

// ============================================================================
// lcdd_FillRect16
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_FillRect16(CONST LCDD_ROI_T* regionOfInterrest, UINT16 bgColor)
{

    if (0 == g_lcddCsBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {        
        if (0 == lcdd_MutexGet(&g_lcddLock))
        {
            return LCDD_ERR_RESOURCE_BUSY;
        }
        else
        {
            // Active window coordinates.
            HAL_GOUDA_WINDOW_T inputWin;
            HAL_GOUDA_WINDOW_T activeWin;

            // Set Active window
            activeWin.tlPX = regionOfInterrest->x;
            activeWin.brPX = regionOfInterrest->x + regionOfInterrest->width - 1;
            activeWin.tlPY = regionOfInterrest->y;
            activeWin.brPY = regionOfInterrest->y + regionOfInterrest->height - 1;

            // Check parameters
            // ROI must be within the screen boundary
            if (    (activeWin.tlPX >= LCDD_DISP_X) ||
                    (activeWin.brPX >= LCDD_DISP_X) ||
                    (activeWin.tlPY >= LCDD_DISP_Y) ||
                    (activeWin.brPY >= LCDD_DISP_Y) 
               )
            {
                lcdd_MutexFree(&g_lcddLock);               
                return LCDD_ERR_INVALID_PARAMETER;
            }

            // need to feed 4pixels alignment stuffed row data
            inputWin.tlPX = (activeWin.tlPX)&~3;
            inputWin.brPX = (activeWin.brPX)|3;
            inputWin.tlPY = activeWin.tlPY;
            inputWin.brPY = activeWin.brPY;

            hal_GoudaSetBgColor(bgColor);
            lcdd_BlitRoi2Win(&inputWin,&activeWin);


            return LCDD_ERR_NO;
        }
    }
}


// ============================================================================
// lcdd_Blit16
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_Blit16(CONST LCDD_FBW_T* frameBufferWin, UINT16 startX, UINT16 startY)
{

    
    if (0 == g_lcddCsBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {
        if (0 == lcdd_MutexGet(&g_lcddLock))
        {
            return LCDD_ERR_RESOURCE_BUSY;
        }
        else
        {   
            // Active window coordinates.
            HAL_GOUDA_WINDOW_T inputWin;
            HAL_GOUDA_WINDOW_T activeWin;
            // Set Input window
            // need to feed 4pixels alignment stuffed row data
            inputWin.tlPX = (frameBufferWin->roi.x)&~3;
            inputWin.brPX = (frameBufferWin->roi.x + frameBufferWin->roi.width - 1)|3;
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
            if (    (activeWin.tlPX >= LCDD_DISP_X) ||
                    (activeWin.brPX >= LCDD_DISP_X) ||
                    (activeWin.tlPY >= LCDD_DISP_Y) ||
                    (activeWin.brPY >= LCDD_DISP_Y) ||
                    (frameBufferWin->roi.width > frameBufferWin->fb.width) ||
                    (frameBufferWin->roi.height > frameBufferWin->fb.height) ||
                    (frameBufferWin->fb.colorFormat != LCDD_COLOR_FORMAT_RGB_565)
                )
            {
                lcdd_MutexFree(&g_lcddLock);  
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
                hal_GoudaOvlLayerOpen(HAL_GOUDA_OVL_LAYER_ID0, &def);
                // tell the end handler to close the layer when we are done ;)
                g_lcddAutoCloseLayer = TRUE;
            }

            // gouda is doing everything ;)
            lcdd_BlitRoi2Win(&inputWin, &activeWin);


            return LCDD_ERR_NO;
        }
    }
}



// ============================================================================
// lcdd_SubOpen
// ----------------------------------------------------------------------------
/// Open the LCDD subscreen driver.
/// It must be called before any call to any other function of this driver
/// concerning a subscreen
/// This function is to be called only once.
/// @return #LCDD_ERR_NO or #LCDD_ERR_DEVICE_NOT_FOUND.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SubOpen(VOID)
{
    // FIXME ... 
    g_lcddConfig = tgt_GetLcddConfig();
    g_lcddCsSubBase = (UINT32) hal_EbcCsOpen(g_lcddConfig->sub.csNum, &g_lcddConfig->sub.csConfig);
    g_lcddCsSubOffset = EXP2(g_lcddConfig->sub.rsLine+1);

    if (0 == g_lcddCsSubBase)
    {
        return LCDD_ERR_DEVICE_NOT_FOUND;
    }
    else
    {
// turn on backlight on PWL 0
// Well, supposing the two screens aren't used at the same time,
// that make sense to go out of reset and turn on the backlight
// here too.       
	    pmd_SetLevel(PMD_LEVEL_LCD, 5);
	    hal_SysResetOut(FALSE);

        // Allow access 
        g_lcddSubLock = 1;
        return LCDD_ERR_NO;
    }
}



// ============================================================================
// lcdd_SubClose
// ----------------------------------------------------------------------------
/// Close the LCDD subscreen driver
/// No other subscreen functions of this driver should be called after a call to 
/// #lcdd_Close.
/// @return #LCDD_ERR_NO or #LCDD_ERR_DEVICE_NOT_FOUND.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SubClose(VOID)
{    
    if (0 == g_lcddCsSubBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {
        return LCDD_ERR_NO;
    }
    
    // TODO: implement.
    LCDD_ASSERT(FALSE, "This function may not be implemented yet\n");
}


// ============================================================================
// lcdd_SubSetContrast
// ----------------------------------------------------------------------------
/// Set the contrast of the LCD subscreen.
/// @param contrast Value to set the contrast to.
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED or 
/// #LCDD_ERR_INVALID_PARAMETER.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SubSetContrast(UINT32 contrast)
{    
    if (0 == g_lcddCsSubBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {
        return LCDD_ERR_NO;
    }
    
    // TODO: implement.
    LCDD_ASSERT(FALSE, "This function may not be implemented yet\n");
}


// ============================================================================
// lcdd_SubSetBrightness
// ----------------------------------------------------------------------------
/// Set the brightness of the LCD subscreen.
/// @param brightness Value to set the brightness to.
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED or 
/// #LCDD_ERR_INVALID_PARAMETER.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SubSetBrightness(UINT32 brightness)
{    
    if (0 == g_lcddCsSubBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {
        return LCDD_ERR_NO;
    }
    
    // TODO: implement.
    LCDD_ASSERT(FALSE, "This function may not be implemented yet\n");
}



// ============================================================================
// lcdd_SubSetBacklight
// ----------------------------------------------------------------------------
/// Set the backlight of the LCD subscreen.
/// @param  backlight Value to set the contrast to.
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED or 
/// #LCDD_ERR_INVALID_PARAMETER.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SubSetBacklight(UINT32 backlight)
{
    if (0 == g_lcddCsSubBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {
        pmd_SetLevel(PMD_LEVEL_LCD, backlight);
        return LCDD_ERR_NO;
    }
}


// ============================================================================
// lcdd_SubSetStandbyMode
// ----------------------------------------------------------------------------
/// Set the LCD subscreen in standby mode or exit from it
/// @param standbyMode If \c TRUE, go in standby mode. 
///                    If \c FALSE, cancel standby mode. 
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED or 
/// #LCDD_ERR_INVALID_PARAMETER.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SubSetStandbyMode(BOOL standbyMode)
{
    if (0 == g_lcddCsSubBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {
        if (standbyMode)
        {
            // Go into standby mode
        }
        else
        {
            // Initialization sequence
            LCM_SUB_REG = (UINT16)0xd1d1; sxr_Sleep(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0xcaca; sxr_Sleep(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0000; sxr_Sleep(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0f0f; sxr_Sleep(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0000; sxr_Sleep(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0xfbfb; sxr_Sleep(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0303; sxr_Sleep(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0xbbbb; sxr_Sleep(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0101; sxr_Sleep(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0xbcbc; sxr_Sleep(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0000; sxr_Sleep(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0000; sxr_Sleep(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0202; sxr_Sleep(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0x8181; sxr_Sleep(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x1b1b; sxr_Sleep(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0505; sxr_Sleep(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0xf2f2; sxr_Sleep(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0808; sxr_Sleep(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x6464; sxr_Sleep(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0x8282; sxr_Sleep(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0101; sxr_Sleep(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0xf7f7; sxr_Sleep(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x2828; sxr_Sleep(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x2c2c; sxr_Sleep(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0505; sxr_Sleep(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0x9494; sxr_Sleep(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0x2020; sxr_Sleep(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0f0f; sxr_Sleep(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0xa6a6; sxr_Sleep(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0xafaf; sxr_Sleep(8 MS_WAITING);
        }

        return LCDD_ERR_NO;
    }
    
    // This function may not be implemented yet: we need the stand by mode...
    // TODO: implement.
    LCDD_ASSERT(FALSE, "This function may not be implemented yet\n");
}


// ============================================================================
// lcdd_SubSleep
// ----------------------------------------------------------------------------
/// Set the LCD subscreen in sleep mode.
/// @return #LCDD_ERR_NO or #LCDD_ERR_NOT_OPENED
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SubSleep(VOID)
{
    if (0 == g_lcddCsSubBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {
        return LCDD_ERR_NO;
    }
    
    // TODO: implement.
    LCDD_ASSERT(FALSE, "This function may not be implemented yet\n");
}


// ============================================================================
// lcdd_SubWakeUp
// ----------------------------------------------------------------------------
/// Wake the LCD subscreen out of sleep mode
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SubWakeUp(VOID)
{
    if (0 == g_lcddCsSubBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {
        return LCDD_ERR_NO;
    }
    
    // TODO: implement.
    LCDD_ASSERT(FALSE, "This function may not be implemented yet\n");
}


// ============================================================================
// lcdd_SubGetScreenInfo
// ----------------------------------------------------------------------------
/// Get information about the sub LCD device.
/// @param pscreenInfo Pointer to the structure where the information
/// obtained will be stored
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED or 
/// #LCDD_ERR_INVALID_PARAMETER.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SubGetScreenInfo(LCDD_SCREEN_INFO_T* pscreenInfo)
{
    if (0 == g_lcddCsSubBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {
        return LCDD_ERR_NO;
    }
    
    // TODO: implement.
    LCDD_ASSERT(FALSE, "This function may not be implemented yet\n");
}



// ============================================================================
// lcdd_SubSetActiveWindow
// ----------------------------------------------------------------------------
/// This function sets the active window coordinates of the subscreen 
/// as physical screen pixel numbers.
/// @param hsa Horizontal start address.
/// @param vsa Vertical   start address.
/// @param hea Horizontal end   address.
/// @param vea Vertical   end   address.
/// @return #LCDD_ERR_NO 
// ============================================================================
LCDD_ERR_T lcdd_SubSetActiveWindow(UINT8 hsa, UINT8 vsa, UINT8 hea, UINT8 vea)
{
    if (0 == lcdd_MutexGet(&g_lcddSubLock))
    {
        return LCDD_ERR_RESOURCE_BUSY;
    }
    else
    {
        LCM_WR_REG(0x16, (hea << 8) | hsa);
        LCM_WR_REG(0x17, (vea << 8) | vsa);
        g_lcddSubActiveWin.hsa = hsa;
        g_lcddSubActiveWin.hea = hea;
        g_lcddSubActiveWin.vsa = vsa;
        g_lcddSubActiveWin.vea = vea;
        lcdd_MutexFree(&g_lcddSubLock);
        return LCDD_ERR_NO;
    }
}





// ============================================================================
// lcdd_SubSetPixel12
// ----------------------------------------------------------------------------
/// Draw a 16-bit pixel a the specified position.
/// @param x X coordinate of the point to set.
/// @param y Y coordinate of the point to set.
/// @param pixelData 12-bit pixel data to draw.
/// @return #LCDD_ERR_NO or #LCDD_ERR_NOT_OPENED.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SubSetPixel12(UINT16 x, UINT16 y, UINT16 pixelData);



// =============================================================================
// lcdd_SubCmd
// -----------------------------------------------------------------------------
/// Private function to send a command to the subscreen and then wait 4 ms.
/// @param cmd Command to send to the subscreen
// =============================================================================
PRIVATE VOID lcdd_SubCmd(UINT16 cmd)
{
	LCM_SUB_REG = cmd;
	sxr_Sleep(4 MS_WAITING);
}


// =============================================================================
// lcdd_SubDat
// -----------------------------------------------------------------------------
/// Private function to write a data in the subscreen registers, and then wait
/// for 4 ms
/// @param dat Data to write.
// =============================================================================
PRIVATE VOID lcdd_SubDat(UINT16 dat)
{
	LCM_SUB_DAT = dat;
	sxr_Sleep(4 MS_WAITING);
}



// ============================================================================
// lcdd_SubFillRect24
// ----------------------------------------------------------------------------
/// This function performs a fill of a given rectangle with some color
/// on the subscreen, the color is specified in dat1, dat2, dat3.
/// See the Salamon spec for data format.
/// @param dat1 Parameter defining the color.
/// @param dat2 Parameter defining the color.
/// @param dat3 Parameter defining the color.
/// @return #LCDD_ERR_NO, #LCDD_ERR_RESOURCE_BUSY or #LCDD_ERR_NOT_OPENED.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SubFillRect24(UINT8 dat1, UINT8 dat2, UINT8 dat3)
{
    UINT8 x;
    UINT8 y;
    
    if (0 == g_lcddCsSubBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {        
        if (0 == lcdd_MutexGet(&g_lcddSubLock))
        {
            return LCDD_ERR_RESOURCE_BUSY;
        }
        else
        {
 
            // Set window start and end address//
            lcdd_SubCmd(0x15);//set start/end X addr
            lcdd_SubDat(g_lcddSubActiveWin.hsa);
            lcdd_SubDat(g_lcddSubActiveWin.hea);

            lcdd_SubCmd(0x75);//set start/end Y addr
            lcdd_SubDat(g_lcddSubActiveWin.vsa);
            lcdd_SubDat(g_lcddSubActiveWin.vea);


            // Send Data
            lcdd_SubCmd(0x5C);
            for(x=0 ; x<g_lcddSubActiveWin.hea - g_lcddSubActiveWin.hsa +1 ; x++)
            {
                for(y=0 ; y<g_lcddSubActiveWin.vea - g_lcddSubActiveWin.vsa +1  ; y++)
                {
                    LCM_SUB_DAT = ~dat1;
                    LCM_SUB_DAT = ~dat2;
                    LCM_SUB_DAT = ~dat3;
                }	
            }
            lcdd_MutexFree(&g_lcddSubLock);
            return LCDD_ERR_NO;
        }
    }

}



// ============================================================================
// lcdd_SubBlit12
// ----------------------------------------------------------------------------
/// This function provides the basic bit-block transfer capabilities for the 
/// subcreen. 
/// This function copies the data (such as characters/bmp) on the LCD directly
/// as a (rectangular) block. The data is drawn in the subscreen active window.
/// @param pPixelData Pointer to the buffer holding the data to be displayed
/// as a block. The dimension of this block is the one of the subscreen active
/// window.
/// @return #LCDD_ERR_NO or #LCDD_ERR_NOT_OPENED.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SubBlit12(LCDD_FBW_T frameBufferWin)
{ 
    return LCDD_ERR_INVALID_PARAMETER;
}



// ============================================================================
// lcdd_Busy
// ----------------------------------------------------------------------------
/// This function is not implemented for the ebc version of the driver
// ============================================================================
PUBLIC BOOL lcdd_Busy(VOID)
{
    return FALSE;
}

// ============================================================================
// lcdd_SetDirRotation
// ----------------------------------------------------------------------------
///  
// ============================================================================
PUBLIC BOOL lcdd_SetDirRotation(VOID)
{

        


    return FALSE;
}
// ============================================================================
// lcdd_SetDirDefault
// ----------------------------------------------------------------------------
///  
// ============================================================================
PUBLIC BOOL lcdd_SetDirDefault(VOID)
{


    return FALSE;
}




