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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/lcdd/st7669v/ebc/src/drv_lcd_st7669v.c $ //
//	$Author: pengzg $                                                        // 
//	$Date: 2011-04-22 14:11:23 +0800 (Fri, 22 Apr 2011) $                     //   
//	$Revision: 7192 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file drv_lcd.c                                                           //
/// That file provides an implementation for the st7669v LCD screen.         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "hal_ebc.h"
#include "hal_pwm.h"
#include "hal_sys.h"
#include "hal_timers.h"
#include "hal_dma.h"

#include "pmd_m.h"

#include "lcdd_m.h"
#include "lcdd_config.h"
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

/// LCDD both screen config.
PRIVATE CONST TGT_LCDD_CONFIG_T* g_lcddConfig
;

/// Main screen base address
PRIVATE UINT32 g_lcddCsBase  = 0;

/// Main screen offset address to access the second register.
PRIVATE UINT32 g_lcddCsOffset  = 0;

/// Structure holding the active window status for the main screen
PRIVATE LCDD_ACTIVE_WIN_STATUS_T   g_lcddActiveWin
;

/// Lock to protect the access to the LCD screen during a DMA access.
/// When 1, access is allowed. When 0, it is denied.
PRIVATE UINT32 g_lcddLock  = 0;

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

        //Software Reset 
        LCM_WR_CMD(0x01);
        hal_TimDelay(150 MS_WAITING);
        //-----------Disable Autoread-----------//
        //Autoread control
        LCM_WR_CMD(0xD7);                  
        //Disable Autoread
        LCM_WR_DATA(0x9F);                
        //Control OTP/MTP
        LCM_WR_CMD(0xE0);                  
        //Read Mode
        LCM_WR_DATA(0x00);                
        hal_TimDelay( 10 MS_WAITING);
        //Control OTP/MTP
        LCM_WR_CMD(0xE3);                  
        hal_TimDelay( 20 MS_WAITING);
        //Close Read Mode
        LCM_WR_CMD(0xE1);                  

        //-------------------Sleep Out-----------------------//
        //display OFF
        LCM_WR_CMD(0x28);
        //Sleep Out
        LCM_WR_CMD(0x11); 
        hal_TimDelay( 50 MS_WAITING);
        //--------------------VOP SET -----------------------//
        //V0 Voltage Set
        LCM_WR_CMD(0xC0);
        LCM_WR_DATA(0x44);
        LCM_WR_DATA(0x01);                  
        //Bias set
        LCM_WR_CMD(0x25);
        LCM_WR_DATA(0x3B);
        //Bias Selection
        LCM_WR_CMD(0xC3);
        LCM_WR_DATA(0x04);
        //Booster Set
        LCM_WR_CMD(0xC4);
        LCM_WR_DATA(0x06);
        //Booster Efficiency Set
        LCM_WR_CMD(0xC5);
        LCM_WR_DATA(0x21);
        //VG Booster Set
        LCM_WR_CMD(0xCB);
        LCM_WR_DATA(0x01);
        //Glass Direction
        LCM_WR_CMD(0xB7);
        LCM_WR_DATA(0xC8);
        //Follower Type set
        LCM_WR_CMD(0xD0);
        LCM_WR_DATA(0x1D);

        //-------------------MTP Set------------------------//
        //Autoread control
        LCM_WR_CMD(0xD7);
        LCM_WR_DATA(0xBF);
        //N-line Set
        LCM_WR_CMD(0xB5);
        LCM_WR_DATA(0x00);
        //PTL Saving Mode Set
        LCM_WR_CMD(0xB4);
        //Power Normal Mode
        LCM_WR_DATA(0x18);

        //-------------LCM_WR_CMD Table 1------------//
        //Normal Display
        LCM_WR_CMD(0x20);
        //Column Range
        LCM_WR_CMD(0x2A);
        //Start Address-4
        LCM_WR_DATA(0x00);
        LCM_WR_DATA(0+2);
        //End Address-129
        LCM_WR_DATA(0x00);
        LCM_WR_DATA(127+2);
        //Page Range
        LCM_WR_CMD(0x2B);
        //Start Address-2
        LCM_WR_DATA(0x00);
        LCM_WR_DATA( 0+2);
        //End Address-0
        LCM_WR_DATA(0x00);
        LCM_WR_DATA(159+2);
        //Pixel Format Set
        LCM_WR_CMD(0x3A);
        LCM_WR_DATA(0x05);
        //Memory Access Control
        LCM_WR_CMD(0x36);
        LCM_WR_DATA(0x00);
        //Duty Set
        LCM_WR_CMD(0xB0);
        LCM_WR_DATA(0xA1);

        //------------------Frame Frequency v.s Temp. Set---------------//
        //Normal Mode Frame Frequency/Temp. Set
        LCM_WR_CMD(0xF0);
        //FA
        LCM_WR_DATA(0x06);
        //FB
        LCM_WR_DATA(0x0B);
        //FC
        LCM_WR_DATA(0x0D);
        //FD
        LCM_WR_DATA(0x12);

        //------------------------------Gamma Table Set--------------------------------------//
        LCM_WR_CMD(0xF9);                  //Gamma
        LCM_WR_DATA(0x00);                //
        LCM_WR_DATA(0x02);                //
        LCM_WR_DATA(0x04);                //
        LCM_WR_DATA(0x06);                //
        LCM_WR_DATA(0x08);                //
        LCM_WR_DATA(0x0A);                //
        LCM_WR_DATA(0x0C);                //
        LCM_WR_DATA(0x0E);                //
        LCM_WR_DATA(0x10);                //
        LCM_WR_DATA(0x12);                //
        LCM_WR_DATA(0x14);                //
        LCM_WR_DATA(0x16);                //
        LCM_WR_DATA(0x18);                //
        LCM_WR_DATA(0x1A);                //
        LCM_WR_DATA(0x1C);                //
        LCM_WR_DATA(0x1E);                //
        //Display On
        LCM_WR_CMD(0x29);

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
        
        lcdd_Sleep();
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
        if (contrast>7) 
        {
            return LCDD_ERR_INVALID_PARAMETER;
        }

        LCM_WR_CMD(0xC3);  
        LCM_WR_DATA(g_lcdContrastTable[contrast]);
        return LCDD_ERR_NO;
    }
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
            lcdd_Sleep();
        }
        else
        {
            lcdd_WakeUp();
        }
        return LCDD_ERR_NO;
    }
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
        // Turn display Off
        LCM_WR_CMD(0x28);

        //Sleep in
        LCM_WR_CMD(0x10);

        return LCDD_ERR_NO;
    }
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
    if (0 == lcdd_MutexGet(&g_lcddLock))
    {
        return LCDD_ERR_RESOURCE_BUSY;
    }
    else
    {
        // Partial Display Mode Set
        LCM_WR_CMD(0x30);
        LCM_WR_DATA(0x0);
        LCM_WR_DATA(vsa);
        LCM_WR_DATA(0x0);
        LCM_WR_DATA(vea);

        // Partial Display Mode on
        LCM_WR_CMD(0x12);

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
    if (0 == lcdd_MutexGet(&g_lcddLock))
    {
        return LCDD_ERR_RESOURCE_BUSY;
    }
    else
    {
        // Turn display Off. To prevent tearing effect image displayed.
        //LCM_WR_CMD(0x28); //Optional

        //Normal on.
        LCM_WR_CMD(0x13); 

        //Display on
        //LCM_WR_CMD(0x29); //Optional

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
    if (0 == g_lcddCsBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {
        // Sleep out
        LCM_WR_CMD(0x11);  
        hal_TimDelay(20 MS_WAITING);

        // Display On
        LCM_WR_CMD(0x29);

        return LCDD_ERR_NO;
    }
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
            LCM_WR_CMD( 0x2A);
            LCM_WR_DATA( 0);
            LCM_WR_DATA( x+2);
            LCM_WR_DATA( 0);
            LCM_WR_DATA( x+2);
            LCM_WR_CMD(0x2B);
            LCM_WR_DATA( 0);
            LCM_WR_DATA( y+2);
            LCM_WR_DATA( 0);
            LCM_WR_DATA( y+2);

            LCM_WR_CMD(0x2C);
#ifdef LCDD_8_BITS_PARALLEL
            LCM_WR_DATA(pixelData >> 8);
            LCM_WR_DATA( pixelData);
#else
            LCM_WR_DATA( pixelData);
#endif

            lcdd_MutexFree(&g_lcddLock);
            return LCDD_ERR_NO;
        }
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

            LCM_WR_CMD(0x2A);	 //Set Column Address
            LCM_WR_DATA(0x00);
            LCM_WR_DATA(g_lcddActiveWin.hsa+2);
            LCM_WR_DATA(0x00);
            LCM_WR_DATA(g_lcddActiveWin.hea+2);

            LCM_WR_CMD(0x2B);	 //Set Page Address
            LCM_WR_DATA(0x00);
            LCM_WR_DATA(g_lcddActiveWin.vsa+2);
            LCM_WR_DATA(0x00);
            LCM_WR_DATA(g_lcddActiveWin.vea+2);

            LCM_WR_CMD(0x2c); //WRITE ram Data display 


#if (!defined(LCDD_WITHOUT_DMA) && !defined(LCDD_8_BITS_PARALLEL))
            HAL_DMA_CFG_T dmaCfg;
            dmaCfg.srcAddr = 0;
            dmaCfg.dstAddr = (UINT8*) &LCM_DAT;
            dmaCfg.alterDstAddr = 0;
            dmaCfg.pattern = bgColor;
            // Number of bytes to transfer. One pixel being 2 bytes,
            // transfer size is number of pixel * 2 !
            dmaCfg.transferSize = 2*((g_lcddActiveWin.hea-g_lcddActiveWin.hsa+1) * 
                    (g_lcddActiveWin.vea-g_lcddActiveWin.vsa+1));

            // Write the color in the data register of the screen
            dmaCfg.mode = HAL_DMA_MODE_PATTERN_CONST_ADDR;
            // We free the lock at the end of the transfer
            dmaCfg.userHandler = lcdd_FreeLock;

            hal_DmaStart(&dmaCfg);
#elif defined(LCDD_8_BITS_PARALLEL)
            UINT32 x;
            for (x = 0; x < (g_lcddActiveWin.hea-g_lcddActiveWin.hsa+1) * 
                    (g_lcddActiveWin.vea-g_lcddActiveWin.vsa+1); ++x)
            {
                LCM_WR_DATA( bgColor >> 8);
                LCM_WR_DATA( bgColor & 0x00FF);
            }
            lcdd_MutexFree(&g_lcddLock);
#else
#error 16 bits transfering without DMA is not implemented!
#endif

            return LCDD_ERR_NO;
        }
    }
}

// ============================================================================
// lcdd_TransferData
// ----------------------------------------------------------------------------
// Private function to transfer data to the LCD
// ============================================================================
PRIVATE VOID lcdd_TransferData(CONST UINT16* pPixelData,UINT16 nPixelWrite, BOOL lastLine)
{
#if (!defined(LCDD_WITHOUT_DMA) && !defined(LCDD_8_BITS_PARALLEL))
    // FIXME Unusable when the LCD is configured for the
    // 8 bits wide bus.
    HAL_DMA_CFG_T dmaCfg;


    dmaCfg.srcAddr = (UINT8*)(pPixelData); 
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
#elif defined(LCDD_8_BITS_PARALLEL)
    UINT32 x;

    for (x = 0; x < nPixelWrite; x++)
    {
        LCM_WR_DATA( pPixelData[x] >> 8);
        LCM_WR_DATA( pPixelData[x] & 0x00FF);
    }
    if (lastLine)
    {
        lcdd_MutexFree(&g_lcddLock);
    }
#else
#error 16 bits transfering without DMA is not implemented!
#endif
}

// ============================================================================
// lcdd_Blit16
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_Blit16(CONST LCDD_FBW_T* frameBufferWin, UINT16 startX, UINT16 startY)
{

    LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_Blit16 (%d,%d) (%d,%d)",
            startX,
            startY,
            startX+frameBufferWin->roi.width-1,
            startY+frameBufferWin->roi.height-1
            );

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
       /*     if (    (g_lcddActiveWin.hea >= LCDD_DISP_X) ||
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
				*/
            LCM_WR_CMD(0x2A);	 //Set Column Address
            LCM_WR_DATA(0x00);
            LCM_WR_DATA(g_lcddActiveWin.hsa+2);
            LCM_WR_DATA(0x00);
            LCM_WR_DATA(g_lcddActiveWin.hea+2);

            LCM_WR_CMD(0x2B);	 //Set Page Address
            LCM_WR_DATA(0x00);
            LCM_WR_DATA(g_lcddActiveWin.vsa+2);
            LCM_WR_DATA(0x00);
            LCM_WR_DATA(g_lcddActiveWin.vea+2);

            LCM_WR_CMD(0x2c); //WRITE ram Data display 


            if (frameBufferWin->roi.width == frameBufferWin->fb.width)
            {
                // The source buffer and the roi have the same width, we can
                // do a single linear transfer
                lcdd_TransferData(frameBufferWin->fb.buffer+frameBufferWin->roi.y*frameBufferWin->roi.width
                        ,frameBufferWin->roi.width*frameBufferWin->roi.height,TRUE);

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
                    +(frameBufferWin->roi.x);

                for (curLine=startLine; curLine<=endLine; curLine++)
                {
                    // transfer one line
                    if (curLine == endLine)
                    {
                        lcdd_TransferData(curBuf, frameBufferWin->roi.width, TRUE);
                    }
                    else
                    {
                        lcdd_TransferData(curBuf, frameBufferWin->roi.width, FALSE);
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
}


// ============================================================================
// lcdd_SubWakeUp
// ----------------------------------------------------------------------------
/// Wake the LCD subscreen out of sleep mode
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SubWakeUp(VOID)
{
    return LCDD_ERR_NO;
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
    return LCDD_ERR_NO;
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
PUBLIC LCDD_ERR_T lcdd_SubSetPixel12(UINT16 x, UINT16 y, UINT16 pixelData)
{
    return LCDD_ERR_NO;
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
PUBLIC LCDD_ERR_T lcdd_SubBlit12(CONST LCDD_FBW_T frameBufferWin)
{
    return LCDD_ERR_NO;
}



// =============================================================================
// lcdd_SubCmd
// -----------------------------------------------------------------------------
/// Private function to send a command to the subscreen and then wait 4 ms.
/// @param cmd Command to send to the subscreen
// =============================================================================
//PRIVATE VOID lcdd_SubCmd(UINT16 cmd)
//{
//}


// =============================================================================
// lcdd_SubDat
// -----------------------------------------------------------------------------
/// Private function to write a data in the subscreen registers, and then wait
/// for 4 ms
/// @param dat Data to write.
// =============================================================================
//PRIVATE VOID lcdd_SubDat(UINT16 dat)
//{
//}



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

        
        LCM_WR_CMD(0x36);	 //Set Scanning Direction
        LCM_WR_DATA(0xA0);

        hal_TimDelay(1 MS_WAITING);

    return FALSE;
}
// ============================================================================
// lcdd_SetDirDefault
// ----------------------------------------------------------------------------
///  
// ============================================================================
PUBLIC BOOL lcdd_SetDirDefault(VOID)
{

        
        LCM_WR_CMD(0x36);	 //Set Scanning Direction
        LCM_WR_DATA(0x00);

        hal_TimDelay(1 MS_WAITING);

    return FALSE;
}



