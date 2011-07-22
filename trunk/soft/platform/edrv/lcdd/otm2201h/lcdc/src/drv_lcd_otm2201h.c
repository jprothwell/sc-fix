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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/lcdd/otm2201h/lcdc/src/drv_lcd_otm2201h.c $ //
//	$Author: pengzg $                                                        // 
//	$Date: 2011-04-22 14:11:23 +0800 (Fri, 22 Apr 2011) $                     //   
//	$Revision: 7192 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file drv_lcd.c                                                           //
/// That file provides an implementation for the otm2201h LCD screen.               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "lcdd_config.h"
#include "hal_timers.h"
#include "hal_lcdc.h"
#include "lcddp_debug.h"

#include "pmd_m.h"
#include "sxr_tls.h"

// To get config
//#include "tgt_lcdd_cfg.h"
#include "lcdd_private.h"
#include "lcdd_tgt_params_greenstone.h"
#include "lcdd.h"

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
PRIVATE UINT16 LCDD_DISP_X = 176; 

// Number of pixels in the display height 
PRIVATE UINT16 LCDD_DISP_Y = 220;

#define  LCD_otm2201h_ID         0x0164
typedef enum
{
    LCDD_ROTATION_0,
    LCDD_ROTATION_90,
    LCDD_ROTATION_180,
    LCDD_ROTATION_270,
    LCDD_MIRROR_ROTATION_0,
    LCDD_MIRROR_ROTATION_90,
    LCDD_MIRROR_ROTATION_180,
    LCDD_MIRROR_ROTATION_270
}LCDD_ROTATION_E;

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================
PRIVATE CONST  LCDD_CONFIG_T g_tgtLcddCfg=TGT_LCDD_CONFIG;
/// LCDD screen config.
PRIVATE CONST  LCDD_CONFIG_T* g_lcddConfig = NULL;

/// Lock to protect the access to the LCD screen during a DMA access.
/// When 1, access is allowed. When 0, it is denied.
PRIVATE UINT32 g_lcddLock = 0;

/// Opened status of the driver, used to check that no 
/// call to blit is done before the driver is opened
PRIVATE BOOL g_lcddOpened = FALSE;
PRIVATE LCDD_ROTATION_E g_lcddRotationStatus = LCDD_ROTATION_90;

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

// ============================================================================
// lcdd_SetDirRotation
// ----------------------------------------------------------------------------
///  
// ============================================================================
PRIVATE BOOL lcdd_SetDirRotationExt(LCDD_ROTATION_E rotation)
{
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);
    g_lcddRotationStatus = rotation;
    switch(rotation)
    { 
        case LCDD_ROTATION_90:
            LCDD_DISP_X = 220;
            LCDD_DISP_Y = 176;
            LCM_WR_REG(0x0003,0x1018); 
            break;
        case LCDD_ROTATION_180:
            LCDD_DISP_X = 176;
            LCDD_DISP_Y = 220;
            LCM_WR_REG(0x0003,0x1000); 
            break;
        case LCDD_ROTATION_270:
            LCDD_DISP_X = 220;
            LCDD_DISP_Y = 176;
            LCM_WR_REG(0x0003,0x1028); 
            break;

        case LCDD_MIRROR_ROTATION_0:
            LCDD_DISP_X = 176;
            LCDD_DISP_Y = 220;
            LCM_WR_REG(0x0003, 0x1020);
            break;

        case LCDD_MIRROR_ROTATION_90:
            LCDD_DISP_X = 220;
            LCDD_DISP_Y = 176;
            LCM_WR_REG(0x0003, 0x1038);
            break;
            
        case LCDD_MIRROR_ROTATION_180:
            LCDD_DISP_X = 176;
            LCDD_DISP_Y = 220;
            LCM_WR_REG(0x0003, 0x1010);
            break;
            
        case LCDD_MIRROR_ROTATION_270:
            LCDD_DISP_X = 220;
            LCDD_DISP_Y = 176;
            LCM_WR_REG(0x0003, 0x1008);
            break;
            
        case LCDD_ROTATION_0:
        default:
            LCDD_DISP_X = 176;
            LCDD_DISP_Y = 220;
            LCM_WR_REG(0x0003,0x1030); 
            g_lcddRotationStatus  = LCDD_ROTATION_0;
            break;
    }
     
    return TRUE;
}

PRIVATE VOID lcdd_SetBlitWindow(UINT32 hsa,UINT32 hea,UINT32 vsa,UINT32 vea)
{
	UINT32 sax,say,eax,eay,startx,starty;
	
	switch(g_lcddRotationStatus)
	{ 
        case LCDD_ROTATION_90:
        	sax = vsa;
        	eax = vea;
        	say = LCDD_DISP_X - 1 - hea;
        	eay = LCDD_DISP_X - 1 - hsa;
        	startx = vsa;
        	starty = LCDD_DISP_X - 1 - hsa;			
        	break;
        	
    	case LCDD_ROTATION_180:
        	sax = LCDD_DISP_X - 1 - hea;
        	eax = LCDD_DISP_X - 1 - hsa;
        	say = LCDD_DISP_Y - 1 - vea;
        	eay = LCDD_DISP_Y - 1 - vsa;
        	startx = LCDD_DISP_X - 1 - hsa;
        	starty = LCDD_DISP_Y - 1 - vsa;
        
			break;
			
    	case LCDD_ROTATION_270:
        	sax = LCDD_DISP_Y - 1 - vea;
        	eax = LCDD_DISP_Y - 1 - vsa;
        	say = hsa;
        	eay = hea;
        	startx = LCDD_DISP_Y - 1 - vsa;
        	starty = hsa;	        
		    break;

      case LCDD_MIRROR_ROTATION_0:
        	sax = hsa;
        	eax = hea;
        	say = vsa;
        	eay = vea;
        	startx = hea;
        	starty = vsa;
	              
                break;
                
      case LCDD_MIRROR_ROTATION_90:
        	sax = vsa;
        	eax = vea;
        	say = LCDD_DISP_X - 1 - hea;
        	eay = LCDD_DISP_X - 1 - hsa;
        	startx = vea;
        	starty = LCDD_DISP_X - 1 - hsa;
	        
                break;

      case LCDD_MIRROR_ROTATION_180:
        	sax = LCDD_DISP_X - 1 - hea;
        	eax = LCDD_DISP_X - 1 - hsa;
        	say = LCDD_DISP_Y - 1 - vea;
        	eay = LCDD_DISP_Y - 1 - vsa;
        	startx = LCDD_DISP_X - 1 - hea;
        	starty = LCDD_DISP_Y - 1 - vsa;      		
        
                break;

       case LCDD_MIRROR_ROTATION_270:
        	sax = LCDD_DISP_Y - 1 - vea;
        	eax = LCDD_DISP_Y - 1 - vsa;
        	say = hsa;
        	eay = hea;
        	startx = LCDD_DISP_Y - 1 - vea;
        	starty = hsa;        	
	        
                break;
                
    	case LCDD_ROTATION_0:
    	default:
        	sax = hsa;
        	eax = hea;
        	say = vsa;
        	eay = vea;
        	startx = hsa;
        	starty = vsa;     		    		  
	        break;
    }	
    
    LCM_WR_REG(0x0037,sax);
    LCM_WR_REG(0x0036,eax);
    LCM_WR_REG(0x0039,say);
    LCM_WR_REG(0x0038,eay);
    LCM_WR_REG(0x0020,startx);
    LCM_WR_REG(0x0021,starty);
}

// ============================================================================
// lcdd_Open
// ----------------------------------------------------------------------------
/// Open the LCDD driver.
/// It must be called before any call to any other function of this driver.
/// This function is to be called only once.
/// @return #LCDD_ERR_NO or #LCDD_ERR_DEVICE_NOT_FOUND.
// ============================================================================
PRIVATE LCDD_ERR_T lcdd_Open(VOID)
{
    g_lcddConfig = &g_tgtLcddCfg;
    hal_LcdcOpen(&g_lcddConfig->config, g_lcddConfig->timings);
   
    
    
    hal_TimDelay(20 MS_WAITING);

    //************* Start Initial Sequence **********//
    LCM_WR_REG(0x0001, 0x021c); // set SS and NL bit  0x001c
    LCM_WR_REG(0x0002, 0x0100); // set 1 line inversion
//    LCM_WR_REG(0x0003, 0x1028); // set GRAM write direction and BGR=1. //0x1030 modify for T33
    LCM_WR_REG(0x0008, 0x0808); // set BP and FP
    LCM_WR_REG(0x000B, 0x1100);
    LCM_WR_REG(0x000C, 0x0000); // RGB interface setting R0Ch=0x0110 for RGB 18Bit and R0Ch=0111for RGB16Bit
    LCM_WR_REG(0x000F, 0x0e01); // Set frame rate  //0a01
    //*************Power On sequence ****************//
    hal_TimDelay(50 MS_WAITING);                         // Delay 50ms
    LCM_WR_REG(0x0010, 0x0800); // Set SAP,DSTB,STB
    LCM_WR_REG(0x0011, 0x1038); // Set APON,PON,AON,VCI1EN,VC
    hal_TimDelay(50 MS_WAITING);                         // Delay 50ms
    LCM_WR_REG(0x0012, 0x1000); // Internal reference voltage= Vci;
    LCM_WR_REG(0x0013, 0x0068); // Set GVDD
    LCM_WR_REG(0x0014, 0x4360); // Set VCOMH/VCOML voltage
    LCM_WR_REG(0x0015, 0x0020);
    //------------- Set GRAM area ------------------//
    LCM_WR_REG(0x0030, 0x0000);
    LCM_WR_REG(0x0031, 0x00DB);
    LCM_WR_REG(0x0032, 0x0000);
    LCM_WR_REG(0x0033, 0x0000);
    LCM_WR_REG(0x0034, 0x00DB);
    LCM_WR_REG(0x0035, 0x0000);
    LCM_WR_REG(0x0036, 0x00AF);
    LCM_WR_REG(0x0037, 0x0000);
    LCM_WR_REG(0x0038, 0x00DB);
    LCM_WR_REG(0x0039, 0x0000);
      
    LCM_WR_REG(0x0050, 0x0101); 
    LCM_WR_REG(0x0051, 0x0b04); 
    LCM_WR_REG(0x0052, 0x0006); 
    LCM_WR_REG(0x0053, 0x0600); 
    LCM_WR_REG(0x0054, 0x040b); 
    LCM_WR_REG(0x0055, 0x0101); 
    LCM_WR_REG(0x0056, 0x0503); 
    LCM_WR_REG(0x0057, 0x0305); 
    LCM_WR_REG(0x0058, 0x0106); 
    LCM_WR_REG(0x0059, 0x0104);
    hal_TimDelay(50 MS_WAITING);                         // Delay 50ms
    LCM_WR_REG(0x0007, 0x1017);
    LCM_WR_CMD(0x0022);


    // Allow access 
    g_lcddLock = 1;
    g_lcddOpened = TRUE;
    lcdd_SetDirRotationExt(LCDD_ROTATION_90);

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
PRIVATE LCDD_ERR_T lcdd_Close(VOID)
{
    
    hal_LcdcClose();

    // Disable access
    g_lcddLock = 0;
    g_lcddOpened = FALSE;

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
PRIVATE LCDD_ERR_T lcdd_SetContrast(UINT32 contrast)
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
PRIVATE LCDD_ERR_T lcdd_SetStandbyMode(BOOL standbyMode)
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
PRIVATE LCDD_ERR_T lcdd_Sleep(VOID)
{
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);

    LCM_WR_REG(0x0007, 0x0000); // Set D1=0, D0=1
    hal_TimDelay(50 MS_WAITING);
    LCM_WR_REG(0x0011, 0x0007); // Set APON,PON,AON,VCI1EN,VC
    hal_TimDelay(50 MS_WAITING);
    LCM_WR_REG(0x0010, 0x0A01); // Enter Standby mode
    
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
PRIVATE LCDD_ERR_T lcdd_PartialOn(UINT16 vsa, UINT16 vea)
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
PRIVATE LCDD_ERR_T lcdd_PartialOff(VOID)
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
PRIVATE LCDD_ERR_T lcdd_WakeUp(VOID)
{
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);

    LCM_WR_REG(0x0010, 0x0A00); // Exit Sleep/ Standby mode
    LCM_WR_REG(0x0011, 0x1038); // Set APON,PON,AON,VCI1EN,VC
    hal_TimDelay(50 MS_WAITING);
    LCM_WR_REG(0x0007, 0x1017); // Set D1=0, D0=1
    
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
PRIVATE LCDD_ERR_T lcdd_GetScreenInfo(LCDD_SCREEN_INFO_T* screenInfo)
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
PRIVATE LCDD_ERR_T lcdd_SetPixel16(UINT16 x, UINT16 y, UINT16 pixelData)
{
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);
    if (0 == lcdd_MutexGet(&g_lcddLock))
    {
        return LCDD_ERR_RESOURCE_BUSY;
    }
    else
    {
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
PRIVATE LCDD_ERR_T lcdd_FillRect16(CONST LCDD_ROI_T* regionOfInterrest, UINT16 bgColor)
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
        
        lcdd_SetBlitWindow(hsa, hea, vsa, vea);

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
PRIVATE VOID lcdd_TransferData(CONST UINT16* pPixelData, UINT32 nPixelWrite, BOOL lastLine)
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
PRIVATE LCDD_ERR_T lcdd_Blit16(CONST LCDD_FBW_T* frameBufferWin, UINT16 startX, UINT16 startY)
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

        lcdd_SetBlitWindow(hsa, hea, vsa, vea);
        LCM_WR_CMD(0x0022);  


        if (frameBufferWin->roi.width + deltaOddStart + deltaOddLength == frameBufferWin->fb.width)
        {
            INT32 transferlength=(frameBufferWin->roi.width + deltaOddStart + deltaOddLength)*frameBufferWin->roi.height;    
            INT16* curBufAdd =frameBufferWin->fb.buffer+frameBufferWin->roi.y*frameBufferWin->fb.width;
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
PRIVATE BOOL lcdd_Busy(VOID)
{
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);
    return FALSE;
}

// ============================================================================
// lcdd_SetDirRotation
// ----------------------------------------------------------------------------
///  
// ============================================================================
PRIVATE BOOL lcdd_SetDirRotation(VOID)
{
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);

        
    return FALSE;
}
// ============================================================================
// lcdd_SetDirDefault
// ----------------------------------------------------------------------------
///  
// ============================================================================
PRIVATE BOOL lcdd_SetDirDefault(VOID)
{
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);

    return FALSE;
}

// ============================================================================
// lcdd_CheckProductId
// ----------------------------------------------------------------------------
/// read Lcd id,compare with define lcd id 
/// @return #TRUE, #FALSE
// ============================================================================
PRIVATE BOOL lcdd_CheckProductId()
{
	UINT16 productId=0;         
	LCDD_CONFIG_T lcddReadConfig=LCDD_READ_CONFIG;
	 
	hal_LcdcOpen(&lcddReadConfig.config, lcddReadConfig.timings);
	hal_TimDelay(20 MS_WAITING);
       hal_LcdcReadReg(0x0, &productId);
       hal_LcdcClose(); 
       // force trace
       LCDD_TRACE(LCDD_INFO_TRC|TSTDOUT, 0, "1LCD otm2201h Id:%x ",productId);
		
	// if(productId ==LCD_otm2201h_ID)
		return TRUE;
	// else
	// 	return FALSE;
   
}

// ============================================================================
// lcdd_otm2201h_RegInit
// ----------------------------------------------------------------------------
/// register the right lcd driver, according to lcdd_CheckProductId
/// @return #TRUE, #FALSE
// ============================================================================
PUBLIC BOOL  lcdd_otm2201h_RegInit(LCDD_REG_T *pLcdDrv)
{  
    if( lcdd_CheckProductId())
    {
        pLcdDrv->lcdd_Open=lcdd_Open;
        pLcdDrv->lcdd_Close=lcdd_Close;
        pLcdDrv->lcdd_SetContrast=lcdd_SetContrast;
        pLcdDrv->lcdd_SetStandbyMode=lcdd_SetStandbyMode;
        pLcdDrv->lcdd_PartialOn=lcdd_PartialOn;
        pLcdDrv->lcdd_PartialOff=lcdd_PartialOff;
        pLcdDrv->lcdd_Blit16=lcdd_Blit16;
        pLcdDrv->lcdd_Busy=lcdd_Busy;
        pLcdDrv->lcdd_FillRect16=lcdd_FillRect16;
        pLcdDrv->lcdd_GetScreenInfo=lcdd_GetScreenInfo;
        pLcdDrv->lcdd_WakeUp=lcdd_WakeUp;
        pLcdDrv->lcdd_SetPixel16=lcdd_SetPixel16;
        pLcdDrv->lcdd_Sleep=lcdd_Sleep;
        pLcdDrv->lcdd_SetDirRotation=lcdd_SetDirRotation;
        pLcdDrv->lcdd_SetDirDefault=lcdd_SetDirDefault;		                               
        return TRUE;	 
    }
    return FALSE;
}




								          
	
