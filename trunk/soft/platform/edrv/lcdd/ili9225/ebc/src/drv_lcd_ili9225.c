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
//
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/lcdd/ili9225/ebc/src/drv_lcd_ili9225.c $
//	$Author: pengzg $ 
//	$Date: 2011-04-22 14:11:23 +0800 (Fri, 22 Apr 2011) $   
//	$Revision: 7192 $   
//
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file drv_lcd.c                                                           //
/// That file provides an implementation for the ili9225 LCD screen.         //
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
#define LCM_RD_REG(Addr)        (UINT16)(LCM_REG = (UINT16)Addr; LCM_DAT)

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
#define LCDD_DISP_X		176 

// Number of pixels in the display height 
#define LCDD_DISP_Y		220 

#define LCDD_DMA_ONE_SIZE   31680      //180*(176*2)


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

/// LCDD both screen config.
PRIVATE CONST TGT_LCDD_CONFIG_T* g_lcddConfig;

/// Main screen base address
PRIVATE UINT32 g_lcddCsBase = 0;

/// Main screen offset address to access the second register.
PRIVATE UINT32 g_lcddCsOffset = 0;

/// Structure holding the active window status for the main screen
PRIVATE LCDD_ACTIVE_WIN_STATUS_T   g_lcddActiveWin;

/// Lock to protect the access to the LCD screen during a DMA access.
/// When 1, access is allowed. When 0, it is denied.
PRIVATE UINT32 g_lcddLock = 0;

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

#ifndef LCDD_WITHOUT_DMA
// =============================================================================
// lcdd_FreeLock
// -----------------------------------------------------------------------------
/// This function frees the lock to access the screen. It is set as the user 
/// handler called by the DMA driver at the end of the writings on the screen.
// =============================================================================
PRIVATE VOID lcdd_FreeLock(VOID)
{
    // Blitting op finish here
    LCDD_PROFILE_WINDOW_EXIT(lcdd_BlitWin);
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
    g_lcddCsBase     = (UINT32) hal_EbcCsOpen(g_lcddConfig->csNum, &g_lcddConfig->csConfig);
    g_lcddCsOffset = EXP2(g_lcddConfig->rsLine+1);
    
    if (0 == g_lcddCsBase)
    {
        return LCDD_ERR_DEVICE_NOT_FOUND;
    }
    else
    {
        // Turn off backlight so that we don't display anything wrong
        
        hal_SysResetOut(FALSE);


        LCM_WR_REG(0x0000,0x0001);
        LCM_WR_REG(0x0001,0x011c);
	 LCM_WR_REG(0x0002,0x0201); 
	 LCM_WR_REG(0x0003,0x1030); 

	 LCM_WR_REG(0x0008,0x0202);

	 LCM_WR_REG(0x000B,0x3000);
	 LCM_WR_REG(0x000C,0x0000);
	 LCM_WR_REG(0x000f,0x0e01);
	 hal_TimDelay(50 MS_WAITING);
	 LCM_WR_REG(0x0020,0x0000);
	 LCM_WR_REG(0x0021,0x0000);
	 hal_TimDelay(50 MS_WAITING);
		

	 LCM_WR_REG(0x0010,0x0800);
	 LCM_WR_REG(0x0011,0x1038);
	 hal_TimDelay(50 MS_WAITING);
	 LCM_WR_REG(0x0012,0x2101);
	 LCM_WR_REG(0x0013,0x0004);

	 LCM_WR_REG(0x0014,0x7572);
	 LCM_WR_REG(0x0030,0x0000);
	
	 LCM_WR_REG(0x0036,0x00af);
        LCM_WR_REG(0x0037,0x0000);

        LCM_WR_REG(0x0038,0x00db);          
        LCM_WR_REG(0x0039,0x0000); 

        LCM_WR_REG(0x0050,0x0400); 
        LCM_WR_REG(0x0051,0x0607); 
        LCM_WR_REG(0x0052,0x0c0a); 
        LCM_WR_REG(0x0053,0x0105);

        LCM_WR_REG(0x0054,0x0a0c);
        LCM_WR_REG(0x0055,0x0b06);
        LCM_WR_REG(0x0056,0x0004);
        LCM_WR_REG(0x0057,0x0301);

        LCM_WR_REG(0x0058,0x0007);          
        LCM_WR_REG(0x0059,0x0700);
    

	 LCM_WR_REG(0x0007,0x1017);
	 hal_TimDelay(10 MS_WAITING);
	
	 LCM_WR_REG(0x0021,0x0000);  
	 LCM_WR_CMD(0x0022);
	 
    
        // Allow access 
        g_lcddLock = 1;

        LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_Open: LCD Opened");
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
        hal_EbcCsClose(g_lcddConfig->csNum);
        LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_Close: LCD closed");
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
    LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_SetContrast %d",contrast);
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
        LCM_WR_CMD(0x2A);  
        LCM_WR_CMD(g_lcdContrastTable[contrast]);
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
    LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_SetStandbyMode %d",standbyMode);
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
    LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_Sleep");
    if (0 == g_lcddCsBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {      
    	   hal_TimDelay(50 MS_WAITING);
           LCM_WR_REG(0x0007,0x1012);
           LCM_WR_REG(0x0007,0x0002);
           LCM_WR_REG(0x0007,0x0000);
           LCM_WR_REG(0x0011,0x0007);
      
           LCM_WR_REG(0x0010,0x0701);//0x0701  enter to sleepmode
	    LCM_WR_REG(0x0011,0x0000);
    	    hal_TimDelay(50 MS_WAITING);
	    LCM_WR_REG(0x0012,0x0000);
	    LCM_WR_REG(0x0013,0x0000);
          

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
    LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_PartialOn vsa %d, vea %d",vsa, vea);
    if (0 == lcdd_MutexGet(&g_lcddLock))
    {
        return LCDD_ERR_RESOURCE_BUSY;
    }
    else
    {
   
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
    LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_PartialOff");
    if (0 == lcdd_MutexGet(&g_lcddLock))
    {
        return LCDD_ERR_RESOURCE_BUSY;
    }
    else
    {

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
    LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_WakeUp");
    if (0 == g_lcddCsBase)
    {
        return LCDD_ERR_NOT_OPENED;
    }
    else
    {  
    	hal_TimDelay(50 MS_WAITING);
       LCM_WR_REG(0x0010,0x0700);
	LCM_WR_REG(0x0011,0x1038);
	hal_TimDelay(50 MS_WAITING);
	LCM_WR_REG(0x0012,0x2101);
	LCM_WR_REG(0x0013,0x0004);
	
	LCM_WR_REG(0x0014,0x7572);
	
	LCM_WR_REG(0x0007,0x1017);
	hal_TimDelay(10 MS_WAITING);
	
	LCM_WR_REG(0x0021,0x0000);  

	LCM_WR_CMD(0x0022);
	hal_TimDelay(50 MS_WAITING);
 
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
    LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_GetScreenInfo");
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
    LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_SetPixel16 (%d,%d) %04x",x,y,pixelData);
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
            LCM_WR_REG(0x0020,x);
            LCM_WR_REG(0x0021,y);
    
            LCM_WR_CMD(0x0022); 
    
            LCM_WR_DATA(pixelData);
            lcdd_MutexFree(&g_lcddLock);
            return LCDD_ERR_NO;
        }
    }
}

#ifndef LCDD_WITHOUT_DMA
PRIVATE VOID lcdd_TransferRectDmaData(UINT32 nPixelLength, UINT16 dambgColor,BOOL lastTime)
{
        
            HAL_DMA_CFG_T dmaCfg;
			
            dmaCfg.srcAddr = 0;
            dmaCfg.dstAddr = (UINT8*) &LCM_DAT;
            dmaCfg.alterDstAddr = 0;
            dmaCfg.pattern = dambgColor;
            dmaCfg.transferSize = nPixelLength*2;

            // Write the color in the data register of the screen
            dmaCfg.mode = HAL_DMA_MODE_PATTERN_CONST_ADDR;
            // We free the lock at the end of the transfer
         		
            if (lastTime)
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
           
}

#endif 
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

    LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_FillRect16 (%d,%d) (%d,%d) %04x",
            regionOfInterrest->x,
            regionOfInterrest->y,
            regionOfInterrest->x+regionOfInterrest->width-1,
            regionOfInterrest->y+regionOfInterrest->height-1,
            bgColor
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

	     LCM_WR_REG(0x0037,g_lcddActiveWin.hsa);//Window Horizontal RAM Address Start                                                                                         
            LCM_WR_REG(0x0036,g_lcddActiveWin.hea);//Window Horizontal RAM Address End                                                                                           
            LCM_WR_REG(0x0039,g_lcddActiveWin.vsa);//Window Vertical RAM Address Start                                                                                           
            LCM_WR_REG(0x0038,g_lcddActiveWin.vea);//Window Vertical RAM Address End 

	     LCM_WR_REG(0x0020,g_lcddActiveWin.hsa);
            LCM_WR_REG(0x0021,g_lcddActiveWin.vsa);

            LCM_WR_CMD(0x0022); 


#ifndef LCDD_WITHOUT_DMA
            UINT32 transferlength;   //176*220 Must <32640*2
            transferlength=(g_lcddActiveWin.hea-g_lcddActiveWin.hsa+1) 
				   * (g_lcddActiveWin.vea-g_lcddActiveWin.vsa+1);
			
            if(transferlength<LCDD_DMA_ONE_SIZE)
            {
               lcdd_TransferRectDmaData(transferlength,bgColor,TRUE);
            }
            else 
            {
               lcdd_TransferRectDmaData(LCDD_DMA_ONE_SIZE,bgColor,FALSE);
	        lcdd_TransferRectDmaData(transferlength-LCDD_DMA_ONE_SIZE,bgColor,TRUE);
            }
	
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

// ============================================================================
// lcdd_TransferData
// ----------------------------------------------------------------------------
// Private function to transfer data to the LCD
// ============================================================================
PRIVATE VOID lcdd_TransferData(CONST UINT16* pPixelData,UINT16 nPixelWrite, BOOL lastLine)
{
#ifndef LCDD_WITHOUT_DMA
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
            
#else
            UINT32 x;
            
            for (x = 0; x < nPixelWrite; x++)
            {
                LCM_DAT = pPixelData[x];
            }
            if (lastLine)
            {
                // Blitting op finish here
                LCDD_PROFILE_WINDOW_EXIT(lcdd_BlitWin);
                lcdd_MutexFree(&g_lcddLock);
            }

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
            
            // Blitting op starts here
            LCDD_PROFILE_WINDOW_ENTER(lcdd_BlitWin);
            
	     LCM_WR_REG(0x0037,g_lcddActiveWin.hsa);//Window Horizontal RAM Address Start                                                                                         
            LCM_WR_REG(0x0036,g_lcddActiveWin.hea);//Window Horizontal RAM Address End                                                                                           
            LCM_WR_REG(0x0039,g_lcddActiveWin.vsa);//Window Vertical RAM Address Start                                                                                           
            LCM_WR_REG(0x0038,g_lcddActiveWin.vea);//Window Vertical RAM Address End 

	     LCM_WR_REG(0x0020,g_lcddActiveWin.hsa);
            LCM_WR_REG(0x0021,g_lcddActiveWin.vsa);

	     LCM_WR_CMD(0x0022); 

            if (frameBufferWin->roi.width == frameBufferWin->fb.width)
            {
                     UINT32 transferlength=frameBufferWin->roi.width*frameBufferWin->roi.height;    //Must <32640*3
                     UINT16* curBufAdd =frameBufferWin->fb.buffer+frameBufferWin->roi.y*frameBufferWin->roi.width;

                   if(transferlength<LCDD_DMA_ONE_SIZE)
                   {
                          lcdd_TransferData(curBufAdd,transferlength,TRUE);
                    }
                   else 
                   {                       
			     lcdd_TransferData(curBufAdd,LCDD_DMA_ONE_SIZE,FALSE);	
                          lcdd_TransferData(curBufAdd+LCDD_DMA_ONE_SIZE,transferlength-LCDD_DMA_ONE_SIZE,TRUE);				
                   }
	        
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

