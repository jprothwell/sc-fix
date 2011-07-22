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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/lcdd/s6d0154/lcdc/src/drv_lcd_s6d0154.c $ //
//	$Author: pengzg $     xiaoke                                                   // 
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

// 0x36/0x37 - Horizontal RAM starting/end address (X or Y depending on 0x03's AM flag)
// 0x38/0x39 - Vertical RAM starting/end address (X or Y depending on 0x03's AM flag)
#define LCM_SET_WIN_POS(hsa, vsa, hea, vea) { \
                    LCM_WR_REG(0x37, hsa); LCM_WR_REG(0x36, hea); \
                    LCM_WR_REG(0x39, vsa); LCM_WR_REG(0x38, vea); }


// =============================================================================
// Screen properties
// =============================================================================

// Number of actual pixels in the display width 
#define LCDD_DISP_X		240 

// Number of pixels in the display height 
#define LCDD_DISP_Y		320 


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

PRIVATE BOOL g_lcddWakeup=0;
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
   lcdd_MutexFree(&g_lcddLock);
}
#endif


VOID LcdPowerOnInit(VOID)
{
    LCM_WR_REG(0x00, 0x0000);
    LCM_WR_REG(0x00, 0x0000);
    LCM_WR_REG(0x00, 0x0000);
    LCM_WR_REG(0x00, 0x0000);
    LCM_WR_REG(0x00, 0x0000);

    LCM_WR_REG(0x80, 0x008D);// To solve instant screen flashing issue
    LCM_WR_REG(0x92, 0x0010);// To solve instant screen flashing issue
    LCM_WR_REG(0x11, 0x001C);// Power Control 2
    LCM_WR_REG(0x12, 0x4112);// Power Control 3  BT2-0, DC11-10, DC21-20, DC31-30
    LCM_WR_REG(0x13, 0x0053);// Power Control 4  DCR_EX=0, DCR2-0, GVD6-0
    LCM_WR_REG(0x14, 0x4269);// Power Control 5  VCOMG=0, VCM6-0, VCMR=0, VML6-0
    LCM_WR_REG(0x10, 0x0800);// Power Control 1  current consumption  STB
    LCM_WR_REG(0x11, 0x011C);// Power Control 2
    hal_TimDelay(10 MS_WAITING);
    LCM_WR_REG(0x11, 0x031C);// Power Control 2
    hal_TimDelay(10 MS_WAITING);
    LCM_WR_REG(0x11, 0x071C);// Power Control 2
    hal_TimDelay(10 MS_WAITING);
    LCM_WR_REG(0x11, 0x0F1C);// Power Control 2
    hal_TimDelay(20 MS_WAITING);
    LCM_WR_REG(0x11, 0x0F3C);// Power Control 2
    hal_TimDelay(30 MS_WAITING);
    LCM_WR_REG(0x01, 0x0128); // Driver Output Control
    LCM_WR_REG(0x02, 0x0100); // LCD-Driving-Waveform Control
    LCM_WR_REG(0x03, 0x1030); // Entry Mode (0, 0) -> (0, 239) -> (1, 0) -> ... -> (319, 239)
    //LCM_WR_REG(0x03, 0x1028); // Entry Mode (0, 239) -> (319, 239) -> (0, 238) -> ... -> (319, 0)
    LCM_WR_REG(0x07, 0x1012); // Blank Period Control 1
    LCM_WR_REG(0x08, 0x0303); // Frame Cycle Control
    LCM_WR_REG(0x0B, 0x1100); // External Display Interface Control
    LCM_WR_REG(0x0C, 0x0000); // General Input
    LCM_WR_REG(0x0F, 0x1801); // Start Oscillation
    LCM_WR_REG(0x15, 0x0020); //VCI Recycling (R15H)

    LCM_WR_REG(0x50, 0x0101);// Gamma Control
    LCM_WR_REG(0x51, 0x0603);
    LCM_WR_REG(0x52, 0x0408);
    LCM_WR_REG(0x53, 0x0000);
    LCM_WR_REG(0x54, 0x0605);
    LCM_WR_REG(0x55, 0x0406);
    LCM_WR_REG(0x56, 0x0303);
    LCM_WR_REG(0x57, 0x0303);
    LCM_WR_REG(0x58, 0x0010);
    LCM_WR_REG(0x59, 0x1000);// Gamma Control
    LCM_WR_REG(0x07, 0x0012);// Display Control
    hal_TimDelay(40 MS_WAITING);
    LCM_WR_REG(0x07, 0x0013);// GRAM Address Set
    LCM_WR_REG(0x07, 0x0017);// Display Control  DISPLAY ON
    
    LCM_SET_WIN_POS(0x0000, 0x0000, 0x00EF, 0x013F);
    LCM_WR_REG(0x20, 0x0000);// GRAM Address Set (X, 0 - 239)
    //LCM_WR_REG(0x20, 0x00EF);// GRAM Address Set
    LCM_WR_REG(0x21, 0x0000);// GRAM Address Set (Y, 0 - 319)
    
    LCM_WR_CMD(0x22);
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
  hal_LcdcOpen(&g_lcddConfig->config, g_lcddConfig->timings);
      
  

  LcdPowerOnInit();

  // Allow access 
  g_lcddLock = 1;
  g_lcddOpened = TRUE;
  g_lcddWakeup=1;

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
    
    hal_LcdcClose();

    // Disable access
    g_lcddLock = 0;
    g_lcddOpened = FALSE;
    g_lcddWakeup=0;

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
#if 0	
      if(g_lcddWakeup==1)
    	{
       LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_Sleep");
	hal_TimDelay(50 MS_WAITING);
	LCM_WR_REG(0x0010,0x0001);
	LCM_WR_REG(0x0007,0x0000);
       hal_TimDelay(50 MS_WAITING);
	g_lcddWakeup=0; 	
       }
#endif
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
    
    return LCDD_ERR_NO;
   
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
  
    return LCDD_ERR_NO;
    
}

// ============================================================================
// lcdd_WakeUp
// ----------------------------------------------------------------------------
/// Wake the main LCD screen out of sleep mode
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_WakeUp(VOID)
{
#if 0
   if(g_lcddWakeup==0)
   {
   LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_WakeUp");
   hal_TimDelay(50 MS_WAITING);
   LCM_WR_REG(0x0028,0x0006);
   LCM_WR_REG(0x0000,0x0001);
   LCM_WR_REG(0x000C,0x0005);
   LCM_WR_REG(0x000d,0x0008);//white black display//0x07
   LCM_WR_REG(0x000E,0x3100);//for filcker
   LCM_WR_REG(0x0001,0x2B3F);//0X2B3F
   LCM_WR_REG(0x0002,0x0600);
   LCM_WR_REG(0x0010,0x0000);
   LCM_WR_REG(0x0007,0x0033);
   LCM_WR_REG(0x0011,0x6830);
   LCM_WR_REG(0x0003,0xAAA8);//0x6068
   LCM_WR_REG(0x000B,0x5308);//don't change for crosstalk//0x5375
   LCM_WR_REG(0x000F,0x0000);
   LCM_WR_REG(0x001E,0x00b2);//0xB0
   LCM_WR_REG(0x0025,0xc000);//0XE000
   LCM_WR_REG(0x003f,0xbb84);//internal command
   LCM_WR_REG(0x0044,0xEF00);
   LCM_WR_REG(0x0045,0x0000);
   LCM_WR_REG(0x0046,0x013F);

   LCM_WR_REG(0x0020,0xB0eb);//internal vcom strength //internal register
   LCM_WR_CMD(0x0022);
   hal_TimDelay(200 MS_WAITING);
   g_lcddWakeup=1;
   }
#endif
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
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);
    if (0 == lcdd_MutexGet(&g_lcddLock))
    {
        return LCDD_ERR_RESOURCE_BUSY;
    }
    else
    {
        LCM_SET_WIN_POS(0x0000, 0x0000, 0x00EF, 0x013F);
        LCM_WR_REG(0x20,x);
        LCM_WR_REG(0x21,y);
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

        LCM_SET_WIN_POS(hsa, vsa, hea, vea);
        LCM_WR_REG(0x20, hsa);
        LCM_WR_REG(0x21, vsa);

        // Set initial point
        LCM_WR_REG(0x22, bgColor);

        UINT32 x;
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
        sxr_Sleep(4);
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
  if(g_lcddWakeup==1)
  {
   LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_Blit16 (%d,%d) (%d,%d)",
            startX,
            startY,
            startX+frameBufferWin->roi.width-1,
            startY+frameBufferWin->roi.height-1
    );  
        
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

        LCM_SET_WIN_POS(hsa, vsa, hea, vea);
        LCM_WR_REG(0x20, hsa);
        LCM_WR_REG(0x21, vsa);
        
        // Send command after which the data we sent
        // are recognized as pixels.
        LCM_WR_CMD(0x22);      

        if (frameBufferWin->roi.width + deltaOddStart + deltaOddLength == frameBufferWin->fb.width)
        {
            INT32 transferlength=(frameBufferWin->roi.width + deltaOddStart + deltaOddLength)*frameBufferWin->roi.height;    
            INT16* curBufAdd =frameBufferWin->fb.buffer+frameBufferWin->roi.y*frameBufferWin->roi.width;
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
   else
   {
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

