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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/lcdd/optrex/src/drv_lcd.c $ //
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

#include "lcdd_m.h"

#include "drv_lcd.h"



// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================


// =============================================================================
//  MACROS
// =============================================================================


#define LCM_REG                 *(volatile UINT16*)(g_lcddCsBase)
#define LCM_DAT                 *(volatile UINT16*)(g_lcddCsBase + 2)
#define LCM_WR_REG(Addr, Data)  LCM_REG = (UINT16)Addr; LCM_DAT = (UINT16)Data
#define LCM_RD_REG(Addr)        (UINT16)(LCM_REG = (UINT16)Addr; LCM_DAT)

#define LCM_BRIGHT_LVL0     0
#define LCM_BRIGHT_LVL1     4
#define LCM_BRIGHT_LVL2     8
#define LCM_BRIGHT_LVL3     16
#define LCM_BRIGHT_LVL4     32
#define LCM_BRIGHT_LVL5     64
#define LCM_BRIGHT_LVL6     128
#define LCM_BRIGHT_LVL7     255



// -------------
// CS2 Settings 
// -------------
#define CS2_MODE_SETTING {1,0,1,0, 0,0,0,0, 0,0,0,0, 0,0,0}  
#define CS2_TIME_SETTING {0,10,0,3, 0,0,10,0, 3,2,1}  




// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

/// Main Screen EBC settings
/// FIXME used to be CONST too
PRIVATE HAL_EBC_CS_CFG_T g_lcddCsSettings
         = 
    {CS2_TIME_SETTING,CS2_MODE_SETTING};



/// Main screen base address
PRIVATE UINT32 g_lcddCsBase  = 0;

/// Structure holding the active window status
PRIVATE LCDD_ACTIVE_WIN_STATUS_T   g_lcddActiveWin
        ;

/// Lock to protect the access to the LCD screen during a DMA access.
/// When 1, access is allowed. When 0, it is denied.
PRIVATE UINT32 g_lcddLock  = 0;


/// Constant table containing the actual Gain for each Level from 0 to 7
PRIVATE const UINT8 g_lcdGainTable[8] =
    {
        LCM_BRIGHT_LVL0,
        LCM_BRIGHT_LVL1,
        LCM_BRIGHT_LVL2,
        LCM_BRIGHT_LVL3,
        LCM_BRIGHT_LVL4,
        LCM_BRIGHT_LVL5,
        LCM_BRIGHT_LVL6,
        LCM_BRIGHT_LVL7
    };

// =============================================================================
//  FUNCTIONS
// =============================================================================


// =============================================================================
// lcdd_FreeLock
// -----------------------------------------------------------------------------
/// This function frees the lock to access the screen. It is set as the user 
/// handler called by the DMA driver at the end of the writings on the screen.
// =============================================================================
PRIVATE VOID lcdd_FreeLock(VOID)
{
   hal_SysMutexFree(&g_lcddLock);
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
    g_lcddCsBase     = (UINT32) hal_EbcCsOpen(HAL_EBC_CS2, (HAL_EBC_CS_CFG_T*)&g_lcddCsSettings );

    if (0 == g_lcddCsBase)
    {
        return LCDD_ERR_DEVICE_NOT_FOUND;
    }
    else
    {
// turn on backlight on PWL 0
        lcdd_SetBrightness(6);
	    hal_SysResetOut(FALSE);
        // Allow access 
        g_lcddLock = 1;
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
    #warning This function is not implemented yet
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
    #warning This function may not be implemented yet
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
    #warning This function is not implemented yet
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
        return LCDD_ERR_NO;
    }
    #warning This function is not implemented yet
}


// ============================================================================
// lcdd_GetScreenInfo
// ----------------------------------------------------------------------------
/// Get information about the main LCD device.
/// @param pscreenInfo Pointer to the structure where the information
/// obtained will be stored
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED or 
/// #LCDD_ERR_INVALID_PARAMETER.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_GetScreenInfo(LCDD_SCREEN_INFO_T pscreenInfo)
{
    if (0 == g_lcddCsBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {
        return LCDD_ERR_NO;
    }
    #warning This function is not implemented yet
}



// =============================================================================
// lcdd_PowerOn
// -----------------------------------------------------------------------------
/// Switch ON the LCD screen power
// =============================================================================
PRIVATE VOID lcdd_PowerOn(VOID)
{
// Power On Sequence
	// Delay 2ms
	hal_TimDelay(2 MS_WAITING);
// Initialize sequence    
	LCM_WR_REG(0x0, 0x0);
	LCM_WR_REG(0x0, 0x0);
	LCM_WR_REG(0x0, 0x0);
	LCM_WR_REG(0x0, 0x0);
	LCM_WR_REG(0x0, 0x1);
	// Delay 10ms
	hal_TimDelay(10 MS_WAITING);
// Display Settings Sequence    
	LCM_WR_REG(0x07, 0x0001);
	LCM_WR_REG(0x17, 0x0001);
	LCM_WR_REG(0x10, 0x24C0);
	LCM_WR_REG(0x11, 0x0001);
	LCM_WR_REG(0x12, 0x001A);
	LCM_WR_REG(0x13, 0x0828);
	LCM_WR_REG(0x12, 0x003A);
	// Delay 150ms
	hal_TimDelay(150 MS_WAITING);
	LCM_WR_REG(0x01, 0x0110);
	LCM_WR_REG(0x03, 0x1030);
	LCM_WR_REG(0x08, 0x0404);
	LCM_WR_REG(0x0B, 0x000A);
	LCM_WR_REG(0x0D, 0x5559);
	LCM_WR_REG(0x15, 0x0000);
	LCM_WR_REG(0x30, 0x0000);
	LCM_WR_REG(0x31, 0x0000);
	LCM_WR_REG(0x32, 0x0303);
	LCM_WR_REG(0x33, 0x0100);
	LCM_WR_REG(0x34, 0x0404);
	LCM_WR_REG(0x35, 0x0707);
	LCM_WR_REG(0x36, 0x0707);
	LCM_WR_REG(0x37, 0x0001);
	LCM_WR_REG(0x38, 0x1F04);
	LCM_WR_REG(0x39, 0x040F);
	LCM_WR_REG(0x40, 0x0000);
	LCM_WR_REG(0x41, 0x0000);
	LCM_WR_REG(0x42, 0x8300);
    // ...
    // Drawing possible here

	lcdd_DisplayOn();
}


// =============================================================================
// lcdd_DisplayOn
// -----------------------------------------------------------------------------
/// Switch on the display of the LCD screen 
// =============================================================================
PRIVATE VOID lcdd_DisplayOn(VOID)
{
	LCM_WR_REG(0x02, 0x0600);
	LCM_WR_REG(0x07, 0x0021);
	// Delay 1ms
	hal_TimDelay(1 MS_WAITING);
	LCM_WR_REG(0x07, 0x0072);
	// Delay 100ms
	hal_TimDelay(100 MS_WAITING);
	LCM_WR_REG(0x02, 0x0700);
	LCM_WR_REG(0x07, 0x0077);
}



// ============================================================================
// lcdd_SetActiveWindow
// ----------------------------------------------------------------------------
/// This function sets the active window coordinates as physical screen pixel
/// numbers.
/// @param hsa Horizontal start address.
/// @param vsa Vertical   start address.
/// @param hea Horizontal end   address.
/// @param vea Vertical   end   address.
/// @return #LCDD_ERR_NO or  #LCDD_ERR_RESOURCE_BUSY
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SetActiveWindow(UINT8 hsa, UINT8 vsa, UINT8 hea, UINT8 vea)
{
    if (0 == hal_SysMutexGet(&g_lcddLock))
    {
        return LCDD_ERR_RESOURCE_BUSY;
    }
    else
    {
        LCM_WR_REG(0x44, (hea << 8) | hsa);
        LCM_WR_REG(0x45, (vea << 8) | vsa);
        g_lcddActiveWin.hsa = hsa;
        g_lcddActiveWin.hea = hea;
        g_lcddActiveWin.vsa = vsa;
        g_lcddActiveWin.vea = vea;
        hal_SysMutexFree(&g_lcddLock);
        return LCDD_ERR_NO;
    }
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
        if (0 == hal_SysMutexGet(&g_lcddLock))
        {
            return LCDD_ERR_RESOURCE_BUSY;
        }
        else
        {

            LCM_WR_REG(0x21, (y << 8) | x);
            LCM_WR_REG(0x22, pixelData);
            hal_SysMutexFree(&g_lcddLock);
            return LCDD_ERR_NO;
        }
    }
}

// ============================================================================
// lcdd_FillRect16
// ----------------------------------------------------------------------------
/// This function performs a fill of a given rectangle with some color
/// @param pRect Pointer to the structure holding the information about
/// the rectangle to fill
/// @param bgColor Color with which to fill the rectangle. It is a 16-bit
/// data, as the one of #lcdd_SetPixel16
/// @return #LCDD_ERR_NO, #LCDD_ERR_RESOURCE_BUSY or #LCDD_ERR_NOT_OPENED.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_FillRect16(LCDD_RECT_T* pRect, UINT16 bgColor)
{
	// UINT32 x;
    HAL_DMA_CFG_T dmaCfg;


    if (0 == g_lcddCsBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {        
        if (0 == hal_SysMutexGet(&g_lcddLock))
        {
            return LCDD_ERR_RESOURCE_BUSY;
        }
        else
        {
            // Set the window corresponding to the rectangle
            LCM_WR_REG(0x44, ((pRect->xPos + pRect->width-1) << 8) | pRect->xPos);
            LCM_WR_REG(0x45, ((pRect->yPos + pRect->height-1) << 8) | pRect->yPos);

            // Set initial point (?)
            LCM_WR_REG(0x21, ((pRect->yPos) << 8) | pRect->xPos);
            LCM_WR_REG(0x22, bgColor);

            dmaCfg.srcAddr = 0;
            dmaCfg.dstAddr = (UINT8*) &LCM_DAT;
            dmaCfg.alterDstAddr = 0;
            dmaCfg.pattern = bgColor;
            // Number of bytes to transfer. One pixel being 1 bits,
            // trnasfer size is number of pixle * 2 !
            // First one already written to set the addr reg to 0x22.
            // thus the '- 1'
            dmaCfg.transferSize = 2*((pRect->width * pRect->height)-1);

            // Write the color in the data register of the screen
            dmaCfg.mode = HAL_DMA_MODE_PATTERN_CONST_ADDR;
            // We free the lock at the end of the transfer
            dmaCfg.userHandler = lcdd_FreeLock;

            hal_DmaStart(&dmaCfg);

            //UINT32 x;
            //for (x = 1; x < (pRect->width * pRect->height); ++x)
            //{
            //    LCM_DAT = bgColor;
            //}
            


            return LCDD_ERR_NO;
        }
    }
}

// ============================================================================
// lcdd_Blit16
// ----------------------------------------------------------------------------
/// This function provides the basic bit-block transfer capabilities. 
/// This function copies the data (such as characters/bmp) on the LCD directly
/// as a (rectangular) block. The dimensions and position where the data is 
/// to be drawn is given by the rectangle parameter.
/// The buffer has to be properly aligned (@todo define properly 'properly')
///
/// @param pDestRect Destination where the data block is to be copied and 
/// displayed. The position is given by the rectangle's X and Y coordinates.
/// The height and width of the rectangle are used as the source dimension.
/// @param pPixelData Pointer to the buffer holding the data to be displayed
/// as a block. The dimension of this block are the one of the #pDestRect 
/// parameter
/// @return #LCDD_ERR_NO, #LCDD_ERR_RESOURCE_BUSY or #LCDD_ERR_NOT_OPENED.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_Blit16(LCDD_RECT_T* pDestRect, UINT16* pPixelData)
{
//	UINT32 x, y;
//	UINT16 nX_Draw, nY_Draw;
//	UINT16 initAddr;
//    UINT16 pixel;
    HAL_DMA_CFG_T dmaCfg;
    
    if (0 == g_lcddCsBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {
        if (0 == hal_SysMutexGet(&g_lcddLock))
        {
            return LCDD_ERR_RESOURCE_BUSY;
        }
        else
        {   
            // Set the window corresponding to the rectangle
            LCM_WR_REG(0x44, ((pDestRect->xPos + pDestRect->width-1) << 8) | pDestRect->xPos);
            LCM_WR_REG(0x45, ((pDestRect->yPos + pDestRect->height-1) << 8) | pDestRect->yPos);

            // Set initial point (?)
            LCM_WR_REG(0x21, ((pDestRect->yPos) << 8) | pDestRect->xPos);
            LCM_WR_REG(0x22, pPixelData[0]);
            
            dmaCfg.srcAddr = (UINT8*)(pPixelData)+2; // As the first pixel is printed to set 
            // the initial value of the command reg.
            dmaCfg.dstAddr = (UINT8*) &LCM_DAT;
            dmaCfg.alterDstAddr = 0;
            dmaCfg.pattern = 0;
            // Number of bytes to transfer. One pixel being 16 bits,
            // transfer size is number of pixel * 2 !
            // First one already written to set the addr reg to 0x22.
            // thus the '- 1'
            dmaCfg.transferSize = 2*((pDestRect->width * pDestRect->height)-1);

            // Write the color in the data register of the screen
            dmaCfg.mode = HAL_DMA_MODE_CONST_ADDR;
            dmaCfg.userHandler = lcdd_FreeLock;

            while(hal_DmaStart(&dmaCfg) != HAL_ERR_NO)
            {
                sxr_Sleep(0);
            }


            
    //        nX_Draw = pDestRect->width;
    //        nY_Draw = pDestRect->height;
    //        for (y = 0; y < nY_Draw; ++y)
    //        {
    //            for (x = 0; x < nX_Draw; ++x)
    //            {
    //                pixel = pPixelData[(y*nX_Draw)+x];
    //                LCM_WR_REG(0x22, pixel);
    //            }
    //        }
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
/// @param pDestRect Destination where the data block is to be copied and 
/// displayed. The position is given by the rectangle's X and Y coordinates.
/// The height and width of the rectangle are used as the source dimension.
/// @param pPixelData Pointer to the buffer holding the data to be displayed
/// as a block. The dimension of this block are the one of the #pDestRect 
/// parameter
/// @return #LCDD_ERR_NO, #LCDD_ERR_RESOURCE_BUSY or #LCDD_ERR_NOT_OPENED.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_Blit16_2(LCDD_RECT_T* pDestRect, UINT16* pPixelData)
{
//	UINT32 x, y;
//	UINT16 nX_Draw, nY_Draw;
//	UINT16 initAddr;
//    UINT16 pixel;
    HAL_DMA_CFG_T dmaCfg;
    
    if (0 == g_lcddCsBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {
        if (0 == hal_SysMutexGet(&g_lcddLock))
        {
            return LCDD_ERR_RESOURCE_BUSY;
        }
        else
        {   
            // Set the window corresponding to the rectangle
            LCM_WR_REG(0x44, ((pDestRect->xPos + pDestRect->width-1) << 8) | pDestRect->xPos);
            LCM_WR_REG(0x45, ((pDestRect->yPos + pDestRect->height-1) << 8) | pDestRect->yPos);

            // Set initial point (?)
            LCM_WR_REG(0x21, ((pDestRect->yPos) << 8) | pDestRect->xPos);
            
            dmaCfg.srcAddr = (UINT8*)(pPixelData);
            dmaCfg.dstAddr = (UINT8*) &LCM_REG;
            dmaCfg.alterDstAddr = (UINT8*) &LCM_DAT;
            dmaCfg.pattern = 0x22;
            // Number of bytes to transfer. One pixel being 16 bits,
            // transfer size is number of pixel * 2 !
            dmaCfg.transferSize = 2*((pDestRect->width * pDestRect->height));

            // Write the color in the data register of the screen
            dmaCfg.mode = HAL_DMA_MODE_ALTERN_ADDR;
            dmaCfg.userHandler = lcdd_FreeLock;

            while(hal_DmaStart(&dmaCfg) != HAL_ERR_NO)
            {
                sxr_Sleep(0);
            }


            
    //        nX_Draw = pDestRect->width;
    //        nY_Draw = pDestRect->height;
    //        for (y = 0; y < nY_Draw; ++y)
    //        {
    //            for (x = 0; x < nX_Draw; ++x)
    //            {
    //                pixel = pPixelData[(y*nX_Draw)+x];
    //                LCM_WR_REG(0x22, pixel);
    //            }
    //        }
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
/// @param pDestRect Destination where the data block is to be copied and 
/// displayed. The position is given by the rectangle's X and Y coordinates.
/// The height and width of the rectangle are used as the source dimension.
/// @param pPixelData Pointer to the buffer holding the data to be displayed
/// as a block. The dimension of this block are the one of the #pDestRect 
/// parameter
/// @return #LCDD_ERR_NO or #LCDD_ERR_NOT_OPENED.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_Blit24(LCDD_RECT_T* pDestRect, UINT8* pPixelData)
{
	UINT32 x, y, d;
	UINT16 red, green, blue;
	UINT16 nDummyStart, nDummyEnd, nX_Draw, nY_Draw;
	UINT16 initAddr;
    
    if (0 == g_lcddCsBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {
        if (0 == hal_SysMutexGet(&g_lcddLock))
        {
            return LCDD_ERR_RESOURCE_BUSY;
        }
        else
        {
            // Calc initial GRAM address from hsa, vsa and the specified offsets
            initAddr = ((pDestRect->yPos) << 8) | (pDestRect->xPos);
            initAddr &= 0xfffc;
            LCM_WR_REG(0x21, initAddr);

            nX_Draw = pDestRect->width;
            nY_Draw = pDestRect->height;

            nDummyStart = pDestRect->xPos & 0x0003;
            nDummyEnd = (~(pDestRect->xPos+pDestRect->width-1)) & 0x0003;

            for (y = 0; y < nY_Draw; ++y)
            {
                for (d = 0; d < nDummyStart; ++d)
                {
                    LCM_WR_REG(0x22, 0);
                }
                
                for (x = 0; x < nX_Draw; ++x)
                {
                    red		= (pPixelData[(((y*nX_Draw)+x)*3)+0] << 8) & 0xf800;
                    green	= (pPixelData[(((y*nX_Draw)+x)*3)+1] << 3) & 0x07e0;
                    blue	= (pPixelData[(((y*nX_Draw)+x)*3)+2] >> 2) & 0x001f;
                    LCM_WR_REG(0x22, red|green|blue);
                }
                for (d = 0; d < nDummyEnd; ++d)
                {
                    LCM_WR_REG(0x22, 0);
                }
            }
            hal_SysMutexFree(&g_lcddLock);
            return LCDD_ERR_NO;
        }
    }
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


