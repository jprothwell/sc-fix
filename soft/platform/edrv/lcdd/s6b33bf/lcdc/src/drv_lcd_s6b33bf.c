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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/lcdd/s6b33bf/lcdc/src/drv_lcd_s6b33bf.c $ //
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

#define LCM_CONTRAST_LVL0     0
#define LCM_CONTRAST_LVL1     32
#define LCM_CONTRAST_LVL2     64
#define LCM_CONTRAST_LVL3     96
#define LCM_CONTRAST_LVL4     128
#define LCM_CONTRAST_LVL5     160
#define LCM_CONTRAST_LVL6     196
#define LCM_CONTRAST_LVL7     255

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

/// LCDD screen config.
PRIVATE CONST TGT_LCDD_CONFIG_T* g_lcddConfig = NULL;

/// Lock to protect the access to the LCD screen during a DMA access.
/// When 1, access is allowed. When 0, it is denied.
PRIVATE UINT32 g_lcddLock = 0;

/// Opened status of the driver, used to check that no 
/// call to blit is done before the driver is opened
PRIVATE BOOL g_lcddOpened = FALSE;

/// Constant table containing the actual Gain for each Level from 0 to 7
PRIVATE const UINT8 g_lcdContrastTable[8] =
    {
        LCM_CONTRAST_LVL0,
        LCM_CONTRAST_LVL1,
        LCM_CONTRAST_LVL2,
        LCM_CONTRAST_LVL3,
        LCM_CONTRAST_LVL4,
        LCM_CONTRAST_LVL5,
        LCM_CONTRAST_LVL6,
        LCM_CONTRAST_LVL7
    };

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

#if (!defined(LCDD_WITHOUT_DMA) && !defined(LCDD_8_BITS_PARALLEL))
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
        
    
    hal_SysResetOut(FALSE);
    
    // Init code        
    LCM_WR_CMD(0x2C);  
	hal_TimDelay(20 MS_WAITING);
    
    // MTP Initial Disable
    LCM_WR_CMD(0xEB);  
	hal_TimDelay(20 MS_WAITING);

    // Turn ON Oscillator
    LCM_WR_CMD(0x02);  
    LCM_WR_CMD(0x01);  
	hal_TimDelay(20 MS_WAITING);
    
    // DC-DC Select
    LCM_WR_CMD(0x20);  
    LCM_WR_CMD(0x0A); 
	hal_TimDelay(20 MS_WAITING);
    
    // DCDC1 ON
    LCM_WR_CMD(0x26);  
    LCM_WR_CMD(0x01); 
	hal_TimDelay(20 MS_WAITING);

    // AMP ON
    LCM_WR_CMD(0x26);  
    LCM_WR_CMD(0x09); 
	hal_TimDelay(20 MS_WAITING);
    
    // DCDC2 ON
    LCM_WR_CMD(0x26);  
    LCM_WR_CMD(0x0B); 
	hal_TimDelay(20 MS_WAITING);
    
    // DCDC3 ON
    LCM_WR_CMD(0x26);  
    LCM_WR_CMD(0x0F); 
	hal_TimDelay(20 MS_WAITING);
    
    // Temperature Compensation set
    LCM_WR_CMD(0x28);  
    LCM_WR_CMD(0x02);  
    
    // RAM Skip Area set to no skip
    LCM_WR_CMD(0x45);
    LCM_WR_CMD(0x00);
    
    // Driver Output mode set
    LCM_WR_CMD(0x10);
    LCM_WR_CMD(0x25);
    
    // Bias Set
    LCM_WR_CMD(0x22);  
    LCM_WR_CMD(0x11);
    
    // DC/DC Clock Divistion set
    LCM_WR_CMD(0x24);  
    LCM_WR_CMD(0x11);
    
    // Contrast control value (0 to 255)
    LCM_WR_CMD(0x2A);  
    LCM_WR_CMD(0xBE);  
    
    // Contrast control for partial display mode
    LCM_WR_CMD(0x2B);  
    LCM_WR_CMD(0x54);
    
    // Set Addressing mode 65k colors
    LCM_WR_CMD(0x30);
    LCM_WR_CMD(0x05);
    
    // Row Vector mode
    LCM_WR_CMD(0x32); 
    LCM_WR_CMD(0x0E);
    
    // N-block inversion set
    LCM_WR_CMD(0x34);
    LCM_WR_CMD(0x92);
    
    // Frame Frequency control (High Frame Frequ)
    LCM_WR_CMD(0x36);  
    LCM_WR_CMD(0x00);  
    
    // Entry Mode Set (Read Modify Write OFF)
    LCM_WR_CMD(0x40);
    LCM_WR_CMD(0x00);
    
    // X Area Address set 0 -> 159 (160pix)
    LCM_WR_CMD(0x42);
    LCM_WR_CMD(0x00);
    LCM_WR_CMD(0x9F);
    
    // Y Area Address set 4 -> 131 (128pix)
    LCM_WR_CMD(0x43);
    LCM_WR_CMD(0x04);
    LCM_WR_CMD(0x83);
    
    // Specified Display Pattern Set (Normal display)
    LCM_WR_CMD(0x53);
    LCM_WR_CMD(0x00);  
    
    // Partial Display Off
    LCM_WR_CMD(0x55);  
    LCM_WR_CMD(0x00);  
    
    // Scroll Start Line 0
    LCM_WR_CMD(0x5A);       
    LCM_WR_CMD(0x00);   
    
    // Display On
    LCM_WR_CMD(0x51);  
   	
    // Allow access 
    g_lcddLock = 1;
    g_lcddOpened = TRUE;

    LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_Open: LCD Opened");
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
    LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_SetContrast %d",contrast);
    if (contrast>7) 
    {
        return LCDD_ERR_INVALID_PARAMETER;
    }
    LCM_WR_CMD(0x2A);  
    LCM_WR_CMD(g_lcdContrastTable[contrast]);
    
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

    //************* Power OFF sequence **************// 
    LCM_WR_CMD(0x50);

    // Standby Mode On
    LCM_WR_CMD(0x2D);

    // Turn Off AMP and all DC/DC
    LCM_WR_CMD(0x26);
    LCM_WR_CMD(0x00);

    return LCDD_ERR_NO;
}


// ============================================================================
// lcdd_PartialOn
// ----------------------------------------------------------------------------
/// Set the Partial mode of the LCD
/// @param vsa : Vertical Start Active
/// @param vea : Vertical End Active
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_PartialOn(UINT16 vsa, UINT16 vea)
{
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);    
    LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_PartialOn vsa %d, vea %d",vsa, vea);
    if (0 == lcdd_MutexGet(&g_lcddLock))
    {
        return LCDD_ERR_RESOURCE_BUSY;
    }
    else
    {
        // Partial Display Mode Set
        LCM_WR_CMD(0x56);
        LCM_WR_CMD(vsa);
        
        // Partial Display Mode Set
        LCM_WR_CMD(0x57);
        LCM_WR_CMD(vea);
        
        // Partial Display Mode Set
        LCM_WR_CMD(0x55);
        LCM_WR_CMD(0x01);

        lcdd_MutexFree(&g_lcddLock);
        return LCDD_ERR_NO;
    }
}


// ============================================================================
// lcdd_PartialOff
// ----------------------------------------------------------------------------
/// return to Normal Mode
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_PartialOff(VOID)
{
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);
    LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_PartialOff");
    if (0 == lcdd_MutexGet(&g_lcddLock))
    {
        return LCDD_ERR_RESOURCE_BUSY;
    }
    else
    {
        // Partial Display Mode Set
        LCM_WR_CMD(0x55);
        LCM_WR_CMD(0x00);
  
        lcdd_MutexFree(&g_lcddLock);
        return LCDD_ERR_NO;
    }
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
    // Standby Mode OFF
    LCM_WR_CMD(0x2C);  
	hal_TimDelay(20 MS_WAITING);
    
    // Turn ON Oscillator
    LCM_WR_CMD(0x02);  
    LCM_WR_CMD(0x01);  
	hal_TimDelay(20 MS_WAITING);
    
    // DCDC1 ON
    LCM_WR_CMD(0x26);  
    LCM_WR_CMD(0x01); 
	hal_TimDelay(20 MS_WAITING);
    
    // AMP ON
    LCM_WR_CMD(0x26);  
    LCM_WR_CMD(0x09); 
	hal_TimDelay(20 MS_WAITING);
    
    // DCDC2 ON
    LCM_WR_CMD(0x26);  
    LCM_WR_CMD(0x0B); 
	hal_TimDelay(20 MS_WAITING);
    
    // DCDC3 ON
    LCM_WR_CMD(0x26);  
    LCM_WR_CMD(0x0F); 
	hal_TimDelay(100 MS_WAITING);
    
    // Display On
    LCM_WR_CMD(0x51);

    return LCDD_ERR_NO;
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
        LCM_WR_CMD(0x43);
        LCM_WR_CMD(y);
        LCM_WR_CMD(y);
        LCM_WR_CMD(0x42);
        LCM_WR_CMD(x);
        LCM_WR_CMD(x);
#if (!defined(LCDD_WITHOUT_DMA) && !defined(LCDD_8_BITS_PARALLEL))
        LCM_WR_DAT(pixelData);
#else
        LCM_WR_DAT(pixelData>>8);
        LCM_WR_DAT(pixelData);
#endif
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

        LCM_WR_CMD(0x42);
        LCM_WR_CMD(vsa);
        LCM_WR_CMD(vea);
        LCM_WR_CMD(0x43);
        LCM_WR_CMD(hsa+4);
        LCM_WR_CMD(hea+4);

        UINT32 x;
        for (x = 0; x < (hea-hsa+1) * (vea-vsa+1); ++x)
        {
#if (!defined(LCDD_WITHOUT_DMA) && !defined(LCDD_8_BITS_PARALLEL))
            LCM_WR_DAT(bgColor);
#else
            LCM_WR_DAT(bgColor>>8);
            LCM_WR_DAT(bgColor);
#endif
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
PRIVATE VOID lcdd_TransferData(CONST UINT16* pPixelData,UINT32 nPixelWrite, BOOL lastLine)
{
#if (!defined(LCDD_WITHOUT_DMA) && !defined(LCDD_8_BITS_PARALLEL))
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
    UINT32 deltaOddStart = 0;
    // Added
    UINT32 deltaOddLength = 0;

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
        // Ensure alignement
        if ((frameBufferWin->roi.x & 0x1) != 0)
        {
            // First pixel is odd, offset of one pixel
            // on the left for both source and window on
            // the screen.
            deltaOddStart = 1;
        }

        // Ensure line length is multiple of 32 bits, ie
        // contains an even number of pixels.
        if (((frameBufferWin->roi.width + deltaOddStart) & 0x1) != 0)
        {
            // Add one pixel on the right
            deltaOddLength = 1;
        }

        // Set Active window
        hsa = startX - deltaOddStart;
        hea = startX + frameBufferWin->roi.width - 1 + deltaOddLength;
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
                (frameBufferWin->roi.width + deltaOddStart + deltaOddLength > frameBufferWin->fb.width) ||
                (frameBufferWin->roi.height > frameBufferWin->fb.height) ||
                (frameBufferWin->fb.colorFormat != LCDD_COLOR_FORMAT_RGB_565)
            )
        {               
            lcdd_MutexFree(&g_lcddLock);
            return LCDD_ERR_INVALID_PARAMETER;
        }

        LCM_WR_CMD(0x42);
        LCM_WR_CMD(vsa);
        LCM_WR_CMD(vea);
        LCM_WR_CMD(0x43);
        LCM_WR_CMD(hsa+4);
        LCM_WR_CMD(hea+4);
            
        if (frameBufferWin->roi.width + deltaOddStart + deltaOddLength == frameBufferWin->fb.width)
        {
            INT32 transferlength=(frameBufferWin->roi.width + deltaOddStart + deltaOddLength)*frameBufferWin->roi.height;    
            INT16* curBufAdd = frameBufferWin->fb.buffer+frameBufferWin->roi.y*frameBufferWin->fb.width;
            lcdd_TransferData(curBufAdd, transferlength, TRUE);
        }
        else
        {    
            // The source buffer is wider than the roi 
            // we have to do a 2D transfer
            UINT16 curLine=0;
            UINT16 startLine = frameBufferWin->roi.y;
            UINT16 endLine = frameBufferWin->roi.y+frameBufferWin->roi.height-1;
            // Start at the base of the buffer
            // add the number of pixels corresponding to the start line
            // add the number of pixel corresponding to the startx
            UINT16* curBuf = frameBufferWin->fb.buffer
                   +(frameBufferWin->roi.y*frameBufferWin->fb.width)
                   +(frameBufferWin->roi.x - deltaOddStart);

            for (curLine=startLine; curLine<=endLine; curLine++)
            {          
                // transfer one line
                if (curLine == endLine)
                {
                    lcdd_TransferData(curBuf, (frameBufferWin->roi.width + deltaOddStart + deltaOddLength), TRUE);
                }
                else
                {
                    lcdd_TransferData(curBuf, (frameBufferWin->roi.width + deltaOddStart + deltaOddLength), FALSE);
                }
                // goto next line
                curBuf+=frameBufferWin->fb.width;
            }
        }
        return LCDD_ERR_NO;
    }
}


// ============================================================================
// lcdd_Busy
// ----------------------------------------------------------------------------
/// This function is not implemented for the ebc version of the driver
// ============================================================================
PUBLIC BOOL lcdd_Busy(VOID)
{
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);
    return FALSE;
}

// ============================================================================
// lcdd_SetDirRotation
// ----------------------------------------------------------------------------
///  
// ============================================================================
PUBLIC BOOL lcdd_SetDirRotation(VOID)
{
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);      
    return FALSE;
}


// ============================================================================
// lcdd_SetDirDefault
// ----------------------------------------------------------------------------
///  
// ============================================================================
PUBLIC BOOL lcdd_SetDirDefault(VOID)
{
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);
    return FALSE;
}

