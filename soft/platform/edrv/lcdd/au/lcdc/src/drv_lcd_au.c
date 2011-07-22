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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/lcdd/au/lcdc/src/drv_lcd_au.c $ //
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

#include "lcdd_config.h"
#include "lcdd_m.h"
#include "hal_timers.h"
#include "hal_lcdc.h"
#include "lcddp_debug.h"

#include "pmd_m.h"
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

#define LCM_WR_REG(Addr, Data)  { while(hal_LcdcWriteReg(Addr, Data)!= HAL_ERR_NO);}
#define LCM_WR_DAT(Data)        { while(hal_LcdcWriteData(Data)     != HAL_ERR_NO);}
#define LCM_WR_CMD(Cmd)         { while(hal_LcdcWriteCmd(Cmd)       != HAL_ERR_NO);}


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
PRIVATE CONST TGT_LCDD_CONFIG_T* g_lcddConfig = NULL;

/// Lock to protect the access to the LCD screen during a DMA access.
/// When 1, access is allowed. When 0, it is denied.
PRIVATE UINT32 g_lcddLock = 0;

/// Opened status of the driver, used to check that no 
/// call to blit is done before the driver is opened
PRIVATE BOOL g_lcddOpened = FALSE;

// =============================================================================
//  FUNCTIONS
// =============================================================================


// =============================================================================
// lcdd_ReadReg
// -----------------------------------------------------------------------------
/// Write the address and then read a data from the LCD register
/// 
/// @param addr Address of the register to read.
/// @param pData Pointer to where to store the read data.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PROTECTED VOID lcdd_ReadReg(UINT16 addr, UINT16* pData)
{
    while(hal_LcdcReadReg(addr, pData)!= HAL_ERR_NO); 
}


// =============================================================================
// lcdd_ReadData
// -----------------------------------------------------------------------------
/// Read a data to the LCD screen.
///
/// @param pData Pointer to where to store the read data.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PROTECTED VOID lcdd_ReadData(UINT16* pData)
{
    while(hal_LcdcReadData(pData)!= HAL_ERR_NO);
}


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

#ifndef LCDD_WITHOUT_DMA
// =============================================================================
// lcdd_FreeLock
// -----------------------------------------------------------------------------
/// This function frees the lock to access the screen. It is set as the user 
/// handler called by the DMA driver at the end of the writings on the screen.
// =============================================================================
PRIVATE VOID lcdd_FreeLock(VOID)
{
   lcdd_MutexFree(&g_lcddLock);
}
#endif


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
    hal_LcdcOpen(&g_lcddConfig->config, g_lcddConfig->timings);
    
    // Turn off backlight so that we don't display anything wrong
    
    hal_SysResetOut(FALSE);
    
    // Power On Sequence
	// Delay 20ms
	hal_TimDelay(20 MS_WAITING);
	LCM_WR_REG(0x07, 0x0004);
	LCM_WR_REG(0x0d, 0x0203);
	LCM_WR_REG(0x0e, 0x1115);
	// Delay 20ms
	hal_TimDelay(20 MS_WAITING);
	LCM_WR_REG(0x01, 0x0113);
	LCM_WR_REG(0x02, 0x0700);
	LCM_WR_REG(0x05, 0x1230);
	LCM_WR_REG(0x06, 0x0000);
	LCM_WR_REG(0x0b, 0x4000);
	LCM_WR_REG(0x0c, 0x0003);
	LCM_WR_REG(0x03, 0x0488);
	LCM_WR_REG(0x04, 0x0000);
	// Delay 40ms
	hal_TimDelay(40 MS_WAITING);
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
	hal_TimDelay(20 MS_WAITING);
	LCM_WR_REG(0x07, 0x0025);
	LCM_WR_REG(0x07, 0x0027);
	LCM_WR_REG(0x07, 0x0037);
	LCM_WR_REG(0x21, 0x0000);       //35

	//lcdd_DisplayOn();
	LCM_WR_REG(0x07, 0x0005);
	// Delay 20ms
	hal_TimDelay(20 MS_WAITING);
	LCM_WR_REG(0x07, 0x0025);
	LCM_WR_REG(0x07, 0x0027);
	// Delay 20ms
	hal_TimDelay(20 MS_WAITING);
	LCM_WR_REG(0x07, 0x0037);
    
    // Allow access 
    g_lcddLock      = 1;
    g_lcddOpened    = TRUE;  
    return LCDD_ERR_NO;
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
    
    lcdd_Sleep();
    hal_SysResetOut(TRUE);
    hal_LcdcClose();

    // Disable access
    g_lcddLock = 0;
    g_lcddOpened = FALSE;
    LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_Close: LCD closed");

    return LCDD_ERR_NO;
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
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);
    //#warning This function is not implemented yet
    return LCDD_ERR_NO;
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
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);
    if (standbyMode)
    {
        // Go into standby mode.
        lcdd_Sleep();
    }
    else
    {
        lcdd_WakeUp();
    }
    return LCDD_ERR_NO;
   
}


// ============================================================================
// lcdd_Sleep
// ----------------------------------------------------------------------------
/// Set the main LCD screen in sleep mode.
/// @return #LCDD_ERR_NO or #LCDD_ERR_NOT_OPENED
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_Sleep(VOID)
{
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);
    return LCDD_ERR_NO;
    //#warning This function is not implemented yet
}



// ============================================================================
// lcdd_WakeUp
// ----------------------------------------------------------------------------
/// Wake the main LCD screen out of sleep mode
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_WakeUp(VOID)
{
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);
    
    //lcdd_DisplayOn();
	LCM_WR_REG(0x07, 0x0005);
	// Delay 20ms
	hal_TimDelay(20 MS_WAITING);
	LCM_WR_REG(0x07, 0x0025);
	LCM_WR_REG(0x07, 0x0027);
	// Delay 20ms
	hal_TimDelay(20 MS_WAITING);
	LCM_WR_REG(0x07, 0x0037);
    return LCDD_ERR_NO;
    //#warning This function is not implemented yet
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
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);
    screenInfo->width = LCDD_DISP_X;
    screenInfo->height = LCDD_DISP_Y;
    screenInfo->bitdepth = LCDD_COLOR_FORMAT_RGB_565;
    screenInfo->nReserved = 0;
    return LCDD_ERR_NO;
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
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);
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



// ============================================================================
// lcdd_FillRect16
// ----------------------------------------------------------------------------
/// This function performs a fill of the active window  with some color.
/// @param bgColor Color with which to fill the rectangle. It is a 16-bit
/// data, as the one of #lcdd_SetPixel16
/// @return #LCDD_ERR_NO, #LCDD_ERR_RESOURCE_BUSY or #LCDD_ERR_NOT_OPENED.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_FillRect16(CONST LCDD_ROI_T* regionOfInterrest, UINT16 bgColor)
{
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);
    LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_FillRect16 (%d,%d) (%d,%d) %04x",
            regionOfInterrest->x,
            regionOfInterrest->y,
            regionOfInterrest->x+regionOfInterrest->width-1,
            regionOfInterrest->y+regionOfInterrest->height-1,
            bgColor
    );
    
    // Active window coordinates.
    UINT32 hsa = 1;
    UINT32 hea = 1;
    UINT32 vsa = 1;
    UINT32 vea = 1;
    UINT32 x;

    if (0 == lcdd_MutexGet(&g_lcddLock))
    {
        return LCDD_ERR_RESOURCE_BUSY;
    }
    else
    {   
        // Set Active window
        hsa = regionOfInterrest->x;
        hea = regionOfInterrest->x + regionOfInterrest->width - 1;
        vsa = regionOfInterrest->y;
        vea = regionOfInterrest->y + regionOfInterrest->height - 1;

        // Check parameters
        // ROI must be within the screen boundary
        if (    (hea >= LCDD_DISP_X) ||
                (hsa >= LCDD_DISP_X) ||
                (vea >= LCDD_DISP_Y) ||
                (vsa >= LCDD_DISP_Y) 
           )
        {
            lcdd_MutexFree(&g_lcddLock);               
            return LCDD_ERR_INVALID_PARAMETER;
        }

        // Set initial point (?)
        LCM_WR_REG(0x21, ((vsa) << 8) | hsa);
        LCM_WR_REG(0x22, bgColor);
        for (x = 1; x < (hea-hsa+1) * 
                    (vea-vsa+1); ++x)
        {
            LCM_WR_DAT(bgColor);
        }
        
        lcdd_MutexFree(&g_lcddLock);
        return LCDD_ERR_NO;
    }
}



// ============================================================================
// lcdd_TransferData
// ----------------------------------------------------------------------------
// Private function to transfer data to the LCD
// ============================================================================
PROTECTED VOID lcdd_TransferData(CONST UINT16* pPixelData, UINT32 nPixelWrite, BOOL lastLine)
{
#ifndef LCDD_WITHOUT_DMA
    HAL_LCDC_IRQ_HANDLER_T  endOfTransferHandler = NULL;
    if (lastLine)
    {
        // Free the screen after the transfer
        endOfTransferHandler = lcdd_FreeLock;
    }

    // Unleash the beast.
    // RGB565 being used, we have 16bits per pixel
    while (hal_LcdcWriteDataBlock(pPixelData, (nPixelWrite*2), endOfTransferHandler) != HAL_ERR_NO)
    {
    }

#else

    UINT32 x;
 	for (x = 0; x < nPixelWrite; x++)
    {
        LCM_WR_DAT(pPixelData[x]);
    }
    
    if (lastLine)
    {
        lcdd_MutexFree(&g_lcddLock);
    }  
#endif	       
}


// ============================================================================
// lcdd_Blit16
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
PUBLIC LCDD_ERR_T lcdd_Blit16(CONST LCDD_FBW_T* frameBufferWin, UINT16 startX, UINT16 startY)
{
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);
    LCDD_ASSERT((frameBufferWin->fb.width&1) == 0, "LCDD: FBW must have an even number "
            "of pixels per line. Odd support is possible at the price of a huge "
            "performance lost");
    // Active window coordinates.
    UINT32 hsa = 1;
    UINT32 hea = 1;
    UINT32 vsa = 1;
    UINT32 vea = 1;
    
    // Added pixel (first pixel) if the ROI start is not 32 bits aligned.
//    UINT32 deltaOddStart = 0;
    // Added
//    UINT32 deltaOddLength = 0;

    LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_Blit16 (%d,%d) (%d,%d)",
            startX,
            startY,
            startX+frameBufferWin->roi.width-1,
            startY+frameBufferWin->roi.height-1
    );
    
    if (0 == lcdd_MutexGet(&g_lcddLock))
    {
        return LCDD_ERR_RESOURCE_BUSY;
    }
    else
    {   
        // Set Active window
        hsa = startX;
        hea = startX + frameBufferWin->roi.width - 1;
        vsa = startY;
        vea = startY + frameBufferWin->roi.height - 1;

        // Check parameters
        // ROI must be within the screen boundary
        // ROI must be within the Frame buffer
        // Color format must be 16 bits
        if (    (hea >= LCDD_DISP_X) ||
                (hsa >= LCDD_DISP_X) ||
                (vea >= LCDD_DISP_Y) ||
                (vsa >= LCDD_DISP_Y) ||
                (frameBufferWin->roi.width > frameBufferWin->fb.width) ||
                (frameBufferWin->roi.height > frameBufferWin->fb.height) ||
                (frameBufferWin->fb.colorFormat != LCDD_COLOR_FORMAT_RGB_565)
           )
        {
            lcdd_MutexFree(&g_lcddLock);
            return LCDD_ERR_INVALID_PARAMETER;
        }

        LCM_WR_REG(0x16, ((hea) << 8) | hsa);
        LCM_WR_REG(0x17, (vea << 8) | vsa);

        // Set initial point (?)
        LCM_WR_REG(0x21, ((vsa) << 8) | hsa);
        LCM_WR_CMD((UINT16)0x22);

        // need to feed 4pixels alignment stuffed row data

        if ((frameBufferWin->roi.width == frameBufferWin->fb.width)
            && ((startX & 3) == 0) 
            && ((frameBufferWin->roi.width & 3) == 0))
        {
            // The source buffer and the roi have the same width,
            // the active window is 4pixel aligned,
            // we can do a single linear transfer
            lcdd_TransferData(frameBufferWin->fb.buffer+frameBufferWin->roi.y*frameBufferWin->roi.width
                    ,frameBufferWin->roi.width*frameBufferWin->roi.height, TRUE);

        }
        else
        {
            // The source buffer is wider than the roi 
            // we have to do a 2D transfer
            UINT16 curLine=0;
            UINT16 startLine = frameBufferWin->roi.y;
            UINT16 endLine = frameBufferWin->roi.y+frameBufferWin->roi.height-1;
            // initial dummy pixels to align on 4 pixels
            UINT16 tfOffset = startX & 3;
            // initial + final pixels rounding
            UINT16 tfWidth = (frameBufferWin->roi.width + tfOffset + 3)&~3;
            // Start at the base of the buffer
            // add the number of pixels corresponding to the start line
            // add the number of pixel corresponding to the startx
            UINT16* curBuf = frameBufferWin->fb.buffer
                +(frameBufferWin->roi.y*frameBufferWin->fb.width)
                +(frameBufferWin->roi.x);

            for (curLine=startLine; curLine<=endLine; curLine++)
            {
                // transfer one line
                if (curLine == endLine)
                {
                    lcdd_TransferData(curBuf-tfOffset, tfWidth, TRUE);
                }
                else
                {
                    lcdd_TransferData(curBuf-tfOffset, tfWidth, FALSE);
                }
                // goto next line
                curBuf+=frameBufferWin->fb.width;
            }
        }     
        return LCDD_ERR_NO;
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
    return LCDD_ERR_NO;
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
    return LCDD_ERR_NO;
    //#warning This function is not implemented yet
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
    return LCDD_ERR_NO;
    //#warning This function is not implemented yet
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
    return LCDD_ERR_NO;
    //#warning This function is not implemented yet
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
    return LCDD_ERR_NO;
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
#if 0 
    // TODO FIXME Implement !
    {
        if (standbyMode)
        {
            // Go into standby mode
        }
        else
        {
            // Initialization sequence
            LCM_SUB_REG = (UINT16)0xd1d1; hal_TimDelay(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0xcaca; hal_TimDelay(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0000; hal_TimDelay(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0f0f; hal_TimDelay(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0000; hal_TimDelay(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0xfbfb; hal_TimDelay(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0303; hal_TimDelay(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0xbbbb; hal_TimDelay(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0101; hal_TimDelay(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0xbcbc; hal_TimDelay(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0000; hal_TimDelay(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0000; hal_TimDelay(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0202; hal_TimDelay(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0x8181; hal_TimDelay(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x1b1b; hal_TimDelay(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0505; hal_TimDelay(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0xf2f2; hal_TimDelay(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0808; hal_TimDelay(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x6464; hal_TimDelay(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0x8282; hal_TimDelay(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0101; hal_TimDelay(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0xf7f7; hal_TimDelay(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x2828; hal_TimDelay(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x2c2c; hal_TimDelay(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0505; hal_TimDelay(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0x9494; hal_TimDelay(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0x2020; hal_TimDelay(8 MS_WAITING);
            LCM_SUB_DAT = (UINT16)0x0f0f; hal_TimDelay(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0xa6a6; hal_TimDelay(8 MS_WAITING);

            LCM_SUB_REG = (UINT16)0xafaf; hal_TimDelay(8 MS_WAITING);
        }

        return LCDD_ERR_NO;
    }
    //#warning This function may not be implemented yet: we need the stand by mode ...
#endif
    return LCDD_ERR_NO;
}


// ============================================================================
// lcdd_SubSleep
// ----------------------------------------------------------------------------
/// Set the LCD subscreen in sleep mode.
/// @return #LCDD_ERR_NO or #LCDD_ERR_NOT_OPENED
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SubSleep(VOID)
{
    return LCDD_ERR_NO;
    //#warning This function is not implemented yet
}


// ============================================================================
// lcdd_SubWakeUp
// ----------------------------------------------------------------------------
/// Wake the LCD subscreen out of sleep mode
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SubWakeUp(VOID);


// ============================================================================
// lcdd_SubGetScreenInfo
// ----------------------------------------------------------------------------
/// Get information about the sub LCD device.
/// @param pscreenInfo Pointer to the structure where the information
/// obtained will be stored
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED or 
/// #LCDD_ERR_INVALID_PARAMETER.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SubGetScreenInfo(LCDD_SCREEN_INFO_T* pscreenInfo);

// ============================================================================
// lcdd_Busy
// ----------------------------------------------------------------------------
/// This function checks the status of the LCD Controller.  It is used to
/// avoid starting a new transfer before the previous one has completed
/// @return TRUE if the controller is busy with a transfer, FALSE if the
/// controller can be used for a new transfer
// ============================================================================
PUBLIC BOOL lcdd_Busy()
{
    return FALSE;
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
LCDD_ERR_T lcdd_SubSetActiveWindow(UINT8 hsa, UINT8 vsa, UINT8 hea, UINT8 vea);





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
PUBLIC LCDD_ERR_T lcdd_SubBlit12(LCDD_FBW_T frameBufferWin);



#if 0
// =============================================================================
// lcdd_SubCmd
// -----------------------------------------------------------------------------
/// Private function to send a command to the subscreen and then wait 4 ms.
/// @param cmd Command to send to the subscreen
// =============================================================================
PRIVATE VOID lcdd_SubCmd(UINT16 cmd)
{
	LCM_SUB_REG = cmd;
	hal_TimDelay(4 MS_WAITING);
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
	hal_TimDelay(4 MS_WAITING);
}

#endif

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
PUBLIC LCDD_ERR_T lcdd_SubFillRect24(UINT8 dat1, UINT8 dat2, UINT8 dat3);


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
PUBLIC LCDD_ERR_T lcdd_SubBlit12(LCDD_FBW_T frameBufferWin);


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


