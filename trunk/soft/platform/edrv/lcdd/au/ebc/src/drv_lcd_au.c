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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/lcdd/au/ebc/src/drv_lcd_au.c $ //
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

#define LCM_REG                 *(volatile UINT16*)(g_lcddCsBase)
#define LCM_DAT                 *(volatile UINT16*)(g_lcddCsBase + g_lcddCsOffset)
#define LCM_WR_CMD(Data)        LCM_REG = (UINT16)Data
#define LCM_WR_DATA(Data)       LCM_DAT = (UINT16)Data
#define LCM_WR_REG(Addr, Data)  LCM_REG = (UINT16)Addr; LCM_DAT = (UINT16)Data
#define LCM_RD_REG(Addr)        (UINT16)({LCM_REG = (UINT16)Addr; LCM_DAT;})
#define LCM_SUB_REG             *(volatile UINT16*)(g_lcddCsSubBase)
#define LCM_SUB_DAT             *(volatile UINT16*)(g_lcddCsSubBase + g_lcddCsSubOffset)


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
PRIVATE CONST TGT_LCDD_CONFIG_T* g_lcddConfig
        ;

/// Main screen base address
PRIVATE UINT32 g_lcddCsBase  = 0;

/// Main screen offset address to access the second register.
PRIVATE UINT32 g_lcddCsOffset  = 0;

/// Subscreen base address
/// Let public to be usable by the util driver
 UINT32 g_lcddCsSubBase  = 0;

/// Sub screen offset address to access the second register.
PRIVATE UINT32 g_lcddCsSubOffset  = 0;

/// Structure holding the active window status for the main screen
PRIVATE LCDD_ACTIVE_WIN_STATUS_T   g_lcddActiveWin
        ;

/// Structure holding the active window status for the 
PRIVATE LCDD_ACTIVE_WIN_STATUS_T   g_lcddSubActiveWin
        ;

/// Lock to protect the access to the LCD screen during a DMA access.
/// When 1, access is allowed. When 0, it is denied.
PRIVATE UINT32 g_lcddLock  = 0;

/// Lock to protect the access to the LCD SUB screen during a DMA access.
/// When 1, access is allowed. When 0, it is denied.
PRIVATE UINT32 g_lcddSubLock  = 0;



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
    *pData = LCM_RD_REG(addr);
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
    *pData = LCM_DAT;
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
    g_lcddCsBase     = (UINT32) hal_EbcCsOpen(g_lcddConfig->main.csNum, &g_lcddConfig->main.csConfig);
    g_lcddCsOffset = EXP2(g_lcddConfig->main.rsLine+1);
    
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
	hal_TimDelay(20 MS_WAITING);
	LCM_WR_REG(0x07, 0x0025);
	LCM_WR_REG(0x07, 0x0027);
	// Delay 20ms
	hal_TimDelay(20 MS_WAITING);
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
            // Set Active window
            g_lcddActiveWin.hsa = regionOfInterrest->x;
            g_lcddActiveWin.hea = regionOfInterrest->x + regionOfInterrest->width - 1;
            g_lcddActiveWin.vsa = regionOfInterrest->y;
            g_lcddActiveWin.vea = regionOfInterrest->y + regionOfInterrest->height - 1;

            // Check parameters
            // ROI must be within the screen boundary
            if (    (g_lcddActiveWin.hea >= LCDD_DISP_X) ||
                    (g_lcddActiveWin.hsa >= LCDD_DISP_X) ||
                    (g_lcddActiveWin.vea >= LCDD_DISP_Y) ||
                    (g_lcddActiveWin.vsa >= LCDD_DISP_Y) 
               )
            {
                lcdd_MutexFree(&g_lcddLock);
                return LCDD_ERR_INVALID_PARAMETER;
            }
            LCM_WR_REG(0x16, (g_lcddActiveWin.hea << 8) | g_lcddActiveWin.hsa);
            LCM_WR_REG(0x17, (g_lcddActiveWin.vea << 8) | g_lcddActiveWin.vsa);

            // Set initial point (?)
            LCM_WR_REG(0x21, ((g_lcddActiveWin.vsa) << 8) | g_lcddActiveWin.hsa);
            LCM_REG = (UINT16)0x22;

            // TODO: need to feed 4pixels alignment stuffed row data 
            // see lcdd_Blit16 (but here is should be easier)

#ifndef LCDD_WITHOUT_DMA
            HAL_DMA_CFG_T dmaCfg;
            dmaCfg.srcAddr = 0;
            dmaCfg.dstAddr = (UINT8*) &LCM_DAT;
            dmaCfg.alterDstAddr = 0;
            dmaCfg.pattern = bgColor;
            // Number of bytes to transfer. One pixel being 2 bytes,
            // transfer size is number of pixel * 2 !
            dmaCfg.transferSize = 2*((g_lcddActiveWin.hea-g_lcddActiveWin.hsa+1) * 
                                     (g_lcddActiveWin.vea-g_lcddActiveWin.vsa+1) );

            // Write the color in the data register of the screen
            dmaCfg.mode = HAL_DMA_MODE_PATTERN_CONST_ADDR;
            // We free the lock at the end of the transfer
            dmaCfg.userHandler = lcdd_FreeLock;

            hal_DmaStart(&dmaCfg);
#else
            UINT32 x;
            for (x = 0; x < (g_lcddActiveWin.hea-g_lcddActiveWin.hsa+1) * 
                                     (g_lcddActiveWin.vea-g_lcddActiveWin.vsa+1); ++x)
            {
                LCM_DAT = bgColor;
            }
            lcdd_MutexFree(&g_lcddLock);
#endif

            return LCDD_ERR_NO;
        }
    }
}

PRIVATE VOID lcdd_TransferData(CONST UINT16* pPixelData,UINT16 nPixelWrite, BOOL lastLine)
{
#ifndef LCDD_WITHOUT_DMA
            HAL_DMA_CFG_T dmaCfg;


            dmaCfg.srcAddr = (UINT8*)(pPixelData); 
            // the initial value of the command reg.
            dmaCfg.dstAddr = (UINT8*) &LCM_DAT;
            dmaCfg.alterDstAddr = 0;
            dmaCfg.pattern = 0;
            // Number of bytes to transfer. One pixel being 16 bits,
            // transfer size is number of pixel * 2 !
            dmaCfg.transferSize = nPixelWrite*2;

            // Write the color in the data register of the screen
            dmaCfg.mode = HAL_DMA_MODE_CONST_ADDR;
            if (lastLine)
            {
                dmaCfg.userHandler = lcdd_FreeLock;
            } 
            else
            {
                dmaCfg.userHandler = NULL;
            }



            while(hal_DmaStart(&dmaCfg) != HAL_ERR_NO)
            {
            }
#else
            UINT32 x;
            
            for (x = 0; x < nPixelWrite; x++)
            {
                LCM_DAT = pPixelData[x];
            }
            if (lastLine)
            {
                lcdd_MutexFree(&g_lcddLock);
            }

#endif
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
            // Set Active window
            g_lcddActiveWin.hsa = startX;
            g_lcddActiveWin.hea = startX + frameBufferWin->roi.width - 1;
            g_lcddActiveWin.vsa = startY;
            g_lcddActiveWin.vea = startY + frameBufferWin->roi.height - 1;

            // Check parameters
            // ROI must be within the screen boundary
            // ROI must be within the Frame buffer
            // Color format must be 16 bits
            if (    (g_lcddActiveWin.hea >= LCDD_DISP_X) ||
                    (g_lcddActiveWin.hsa >= LCDD_DISP_X) ||
                    (g_lcddActiveWin.vea >= LCDD_DISP_Y) ||
                    (g_lcddActiveWin.vsa >= LCDD_DISP_Y) ||
                    (frameBufferWin->roi.width > frameBufferWin->fb.width) ||
                    (frameBufferWin->roi.height > frameBufferWin->fb.height) ||
                    (frameBufferWin->fb.colorFormat != LCDD_COLOR_FORMAT_RGB_565)
               )
            {
                lcdd_MutexFree(&g_lcddLock);
                return LCDD_ERR_INVALID_PARAMETER;
            }

            LCM_WR_REG(0x16, ((g_lcddActiveWin.hea) << 8) | g_lcddActiveWin.hsa);
            LCM_WR_REG(0x17, (g_lcddActiveWin.vea << 8) | g_lcddActiveWin.vsa);

            // Set initial point (?)
            LCM_WR_REG(0x21, ((g_lcddActiveWin.vsa) << 8) | g_lcddActiveWin.hsa);
            LCM_REG = (UINT16)0x22;

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
}



// ============================================================================
// lcdd_Blit16_2
// ----------------------------------------------------------------------------
/// Dma mode altern address with pattern AND buffer
/// This function provides the basic bit-block transfer capabilities. 
/// This function copies the data (such as characters/bmp) on the LCD directly
/// as a (rectangular) block. The dimensions and position where the data is 
/// to be drawn is given by the rectangle parameter.
/// The buffer has to be properly aligned (@todo define properly 'properly')
///
/// @param pPixelData Pointer to the buffer holding the data to be displayed
/// as a block. The dimension of this block are the one of the #pDestRect 
/// parameter
/// @return #LCDD_ERR_NO, #LCDD_ERR_RESOURCE_BUSY or #LCDD_ERR_NOT_OPENED.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_Blit16_2(CONST UINT16* pPixelData)
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
            // Set initial point (?)
            LCM_WR_REG(0x21, ((g_lcddActiveWin.vsa) << 8) | g_lcddActiveWin.hsa);
            
#ifndef LCDD_WITHOUT_DMA
            HAL_DMA_CFG_T dmaCfg;

            dmaCfg.srcAddr = (UINT8*)(pPixelData);
            dmaCfg.dstAddr = (UINT8*) &LCM_REG;
            dmaCfg.alterDstAddr = (UINT8*) &LCM_DAT;
            dmaCfg.pattern = 0x22;
            // Number of bytes to transfer. One pixel being 16 bits,
            // transfer size is number of pixel * 2 !
            dmaCfg.transferSize = 2*((g_lcddActiveWin.hea-g_lcddActiveWin.hsa+1) * 
                                     (g_lcddActiveWin.vea-g_lcddActiveWin.vsa+1));

            // Write the color in the data register of the screen
            dmaCfg.mode = HAL_DMA_MODE_ALTERN_ADDR;
            dmaCfg.userHandler = lcdd_FreeLock;

            while(hal_DmaStart(&dmaCfg) != HAL_ERR_NO)
            {
            }
#else

            UINT32 x;
            UINT32 nPixelWrite;
            
            LCM_REG = (UINT16)0x22;
            nPixelWrite = (g_lcddActiveWin.hea-g_lcddActiveWin.hsa+1) * (g_lcddActiveWin.vea-g_lcddActiveWin.vsa+1);
            for (x = 1; x < nPixelWrite; ++x)
            {
                LCM_DAT = pPixelData[x];
            }

            lcdd_MutexFree(&g_lcddLock);

#endif
            return LCDD_ERR_NO;
        }
    }
}



// ============================================================================
// lcdd_Blit24
// ----------------------------------------------------------------------------
/// This function provides the basic bit-block transfer capabilities. 
/// This function copies the data (such as characters/bmp) on the LCD directly
/// as a (rectangular) block. The dimensions and position where the data is 
/// to be drawn is given by the rectangle parameter.
/// @param pPixelData Pointer to the buffer holding the data to be displayed
/// as a block. The dimension of this block are the one of the #pDestRect 
/// parameter
/// @return #LCDD_ERR_NO or #LCDD_ERR_NOT_OPENED.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_Blit24(CONST UINT8* pPixelData)
{
	UINT32 x, y, d;
	UINT16 red, green, blue;
	UINT16 nDummyStart, nDummyEnd, nXDraw, nYDraw;
	UINT16 initAddr;
    
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
            // Calc initial GRAM address from hsa, vsa and the specified offsets
            initAddr = (g_lcddActiveWin.vsa << 8) | g_lcddActiveWin.hsa;
            initAddr &= 0xfffc;
            LCM_WR_REG(0x21, initAddr);

            nXDraw = (g_lcddActiveWin.hea-g_lcddActiveWin.hsa+1);
            nYDraw = (g_lcddActiveWin.vea-g_lcddActiveWin.vsa+1);

            nDummyStart = g_lcddActiveWin.hsa & 0x0003;
            nDummyEnd = ~(g_lcddActiveWin.hea) & 0x0003;

            for (y = 0; y < nYDraw; ++y)
            {
                for (d = 0; d < nDummyStart; ++d)
                {
                    LCM_WR_REG(0x22, 0);
                }
                
                for (x = 0; x < nXDraw; ++x)
                {
                    red		= (pPixelData[(((y*nXDraw)+x)*3)+0] << 8) & 0xf800;
                    green	= (pPixelData[(((y*nXDraw)+x)*3)+1] << 3) & 0x07e0;
                    blue	= (pPixelData[(((y*nXDraw)+x)*3)+2] >> 2) & 0x001f;
                    LCM_WR_REG(0x22, red|green|blue);
                }
                for (d = 0; d < nDummyEnd; ++d)
                {
                    LCM_WR_REG(0x22, 0);
                }
            }
            lcdd_MutexFree(&g_lcddLock);
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




