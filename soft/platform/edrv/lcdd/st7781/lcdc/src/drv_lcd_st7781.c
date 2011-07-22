////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2009, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL: http://10.10.100.14/svn/developing1/Sources/edrv/trunk/lcdd/ili9328/lcdc/src/drv_lcd_ili9328.c $ //
//	$Author: huazeng $                                                        // 
//	$Date: 2010-08-23 13:47:34 +0800 (一, 2010-08-23) $                     //   
//	$Revision: 1532 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file drv_lcd.c                                                           //
/// That file provides an implementation for the ili9328 LCD screen.               //
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

	
	hal_TimDelay(50 MS_WAITING); // Delay 50ms
#if 0
	LCM_WR_REG(0x00E3, 0x3008); // Set internal timing
	LCM_WR_REG(0x00E7, 0x0012); // Set internal timing
	LCM_WR_REG(0x00EF, 0x1231); // Set internal timing

	LCM_WR_REG(0x0001, 0x0100); // set SS and SM bit
	LCM_WR_REG(0x0002, 0x0700); // set 1 line inversion
	LCM_WR_REG(0x0003, 0x1030); // set GRAM write direction and BGR=1.
	LCM_WR_REG(0x0004, 0x0000); // Resize register
	LCM_WR_REG(0x0008, 0x0202); // set the back porch and front porch
	LCM_WR_REG(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
	LCM_WR_REG(0x000A, 0x0000); // FMARK function
	LCM_WR_REG(0x000C, 0x0000); // RGB interface setting
	LCM_WR_REG(0x000D, 0x0000); // Frame marker Position
	LCM_WR_REG(0x000F, 0x0000); // RGB interface polarity

	// ----------- Power On sequence ----------//
	LCM_WR_REG(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
	LCM_WR_REG(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
	LCM_WR_REG(0x0012, 0x0000); // VREG1OUT voltage
	LCM_WR_REG(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude

	hal_TimDelay(200 MS_WAITING); // Dis-charge capacitor power voltage

	LCM_WR_REG(0x0010, 0x1290); // SAP, BT[3:0], AP, DSTB, SLP, STB
	LCM_WR_REG(0x0011, 0x0227); // R11h=0x0221 at VCI=3.3V, DC1[2:0], DC0[2:0], VC[2:0]

	hal_TimDelay(50 MS_WAITING); // Delay 50ms

	LCM_WR_REG(0x0012, 0x0091); // External reference voltage= Vci

	hal_TimDelay(50 MS_WAITING); // Delay 50ms

	LCM_WR_REG(0x0013, 0x1c00); // VDV[4:0] for VCOM amplitude
	LCM_WR_REG(0x0029, 0x003b); // VCM[5:0] for VCOMH
	LCM_WR_REG(0x002B, 0x000C); // Set Frame Rate

	hal_TimDelay(50 MS_WAITING); // Delay 50ms

	LCM_WR_REG(0x0020, 0x0000); // GRAM horizontal Address
	LCM_WR_REG(0x0021, 0x0000); // GRAM Vertical Address

	// ----------- Adjust the Gamma Curve ----------//
	LCM_WR_REG(0x0030, 0x0004);
	LCM_WR_REG(0x0031, 0x0605);
	LCM_WR_REG(0x0032, 0x0106);
	LCM_WR_REG(0x0035, 0x0302);
	LCM_WR_REG(0x0036, 0x0004);
	LCM_WR_REG(0x0037, 0x0106);
	LCM_WR_REG(0x0038, 0x0202);
	LCM_WR_REG(0x0039, 0x0307);
	LCM_WR_REG(0x003C, 0x0203);
	LCM_WR_REG(0x003D, 0x0004);

	//------------------ Set GRAM area ---------------//
	LCM_WR_REG(0x0050, 0x0000); // Horizontal GRAM Start Address
	LCM_WR_REG(0x0051, 0x00EF); // Horizontal GRAM End Address
	LCM_WR_REG(0x0052, 0x0000); // Vertical GRAM Start Address
	LCM_WR_REG(0x0053, 0x013F); // Vertical GRAM Start Address
	LCM_WR_REG(0x0060, 0xA700); // Gate Scan Line
	LCM_WR_REG(0x0061, 0x0001); // NDL,VLE, REV
	LCM_WR_REG(0x006A, 0x0000); // set scrolling line

	//-------------- Partial Display Control ---------//
	LCM_WR_REG(0x0080, 0x0000);
	LCM_WR_REG(0x0081, 0x0000);
	LCM_WR_REG(0x0082, 0x0000);
	LCM_WR_REG(0x0083, 0x0000);
	LCM_WR_REG(0x0084, 0x0000);
	LCM_WR_REG(0x0085, 0x0000);

	//-------------- Panel Control -------------------//
	LCM_WR_REG(0x0090, 0x0010);
	LCM_WR_REG(0x0092, 0x0600);
	LCM_WR_REG(0x0007, 0x0133); // 262K color and display ON
#else
//************* Start Initial Sequence **********//
LCM_WR_CMD(0x11); //Exit Sleep
hal_TimDelay(50 MS_WAITING); // Delay 50ms
LCM_WR_CMD(0xCB); //AP[2:0]
LCM_WR_DAT (0x01);
LCM_WR_CMD(0xC0); //Power control
LCM_WR_DAT (0x26); //VRH[5:0]
LCM_WR_DAT (0x08); //VC[2:0]
LCM_WR_CMD(0xC1); //Power control
LCM_WR_DAT (0x10); //SAP[2:0];BT[3:0]
LCM_WR_CMD(0xC5); //VCM control
LCM_WR_DAT (0x35);
LCM_WR_DAT (0x3E);
LCM_WR_CMD(0x36); // Memory Access Control
LCM_WR_DAT (0x48);
LCM_WR_CMD(0xB1); // Frame Rate Control
LCM_WR_DAT (0x00);
LCM_WR_DAT (0x16);
LCM_WR_CMD(0xB6); // Display Function Control
LCM_WR_DAT (0x0A);
LCM_WR_DAT (0x82);
LCM_WR_CMD(0xC7); // VCOM Control , VMF[6:0]
LCM_WR_DAT (0xB5);
LCM_WR_CMD(0xF2); // 3Gamma Function Disable
LCM_WR_DAT (0x00);
LCM_WR_CMD(0x26); //Gamma curve selected
LCM_WR_DAT (0x01);
LCM_WR_CMD(0x3A); 
LCM_WR_DAT (0x55);
LCM_WR_CMD(0xE0); //Set Gamma
LCM_WR_DAT (0x1F);
LCM_WR_DAT (0x1A);
LCM_WR_DAT (0x18);
LCM_WR_DAT (0x0A);
LCM_WR_DAT (0x0F);
LCM_WR_DAT (0x06);
LCM_WR_DAT (0x45);
LCM_WR_DAT (0x87);
LCM_WR_DAT (0x32);
LCM_WR_DAT (0x0A);
LCM_WR_DAT (0x07);
LCM_WR_DAT (0x02);
LCM_WR_DAT (0x07);
LCM_WR_DAT (0x05);
LCM_WR_DAT (0x00);
LCM_WR_CMD(0XE1); //Set Gamma
LCM_WR_DAT (0x00);
LCM_WR_DAT (0x25);
LCM_WR_DAT (0x27);
LCM_WR_DAT (0x05);
LCM_WR_DAT (0x10);
LCM_WR_DAT (0x09);
LCM_WR_DAT (0x3A);
LCM_WR_DAT (0x78);
LCM_WR_DAT (0x4D);
LCM_WR_DAT (0x05);
LCM_WR_DAT (0x18);
LCM_WR_DAT (0x0D);
LCM_WR_DAT (0x38);
LCM_WR_DAT (0x3A);
LCM_WR_DAT (0x1F);
LCM_WR_CMD(0x29); //display on
#endif
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
     
     if(g_lcddWakeup==1)
     {  
     	LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_Sleep");
     	
	LCM_WR_REG(0x0007, 0x0131); // Set D1=0, D0=1

	hal_TimDelay(10 MS_WAITING);

	LCM_WR_REG(0x0007, 0x0130); // Set D1=0, D0=0

	hal_TimDelay(10 MS_WAITING);

	LCM_WR_REG(0x0007, 0x0000); // display OFF

	// Power OFF sequence 
	LCM_WR_REG(0x0010, 0x0080); // SAP, BT[3:0], APE, AP, DSTB, SLP
	LCM_WR_REG(0x0011, 0x0000); // DC1[2:0], DC0[2:0], VC[2:0]
	LCM_WR_REG(0x0012, 0x0000); // VREG1OUT voltage
	LCM_WR_REG(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude

	hal_TimDelay(200 MS_WAITING); // Dis-charge capacitor power voltage

	LCM_WR_REG(0x0010, 0x0082); // SAP, BT[3:0], APE, AP, DSTB, SLP
	g_lcddWakeup=0;
     }

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
      if(g_lcddWakeup==0)
      {
      	LCDD_TRACE(LCDD_INFO_TRC, 0, "lcdd_WakeUp");
	// Power On sequence 
	LCM_WR_REG(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
	LCM_WR_REG(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
	LCM_WR_REG(0x0012, 0x0000); // VREG1OUT voltage
	LCM_WR_REG(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude

	hal_TimDelay(200 MS_WAITING); // Dis-charge capacitor power voltage

	LCM_WR_REG(0x0010, 0x1290); // SAP, BT[3:0], AP, DSTB, SLP, STB
	LCM_WR_REG(0x0011, 0x0227); // R11h=0x0221 at VCI=3.3V, DC1[2:0], DC0[2:0], VC[2:0]

	hal_TimDelay(50 MS_WAITING); // Delay 50ms

	LCM_WR_REG(0x0012, 0x0091); // External reference voltage= Vci

	hal_TimDelay(50 MS_WAITING); // Delay 50ms

	LCM_WR_REG(0x0013, 0x1c00); // VDV[4:0] for VCOM amplitude
	LCM_WR_REG(0x0029, 0x003b); // VCM[5:0] for VCOMH

	hal_TimDelay(50 MS_WAITING); // Delay 50ms

	LCM_WR_REG(0x0007, 0x0133); // 262K color and display ON
	
        g_lcddWakeup=1;
      }
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
		LCM_WR_REG(0x0020,x);
		LCM_WR_REG(0x0021,y);
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


		LCM_WR_REG(0x0050,hsa);	//Window Horizontal RAM Address Start
		LCM_WR_REG(0x0051,hea);	//Window Horizontal RAM Address End
		LCM_WR_REG(0x0052,vsa);	//Window Vertical RAM Address Start
		LCM_WR_REG(0x0053,vea);	//Window Vertical RAM Address End 

		LCM_WR_REG(0x0020,hsa);
		LCM_WR_REG(0x0021,vsa);

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


		LCM_WR_REG(0x0050, hsa);//Window Horizontal RAM Address Start
		LCM_WR_REG(0x0051, hea);//Window Horizontal RAM Address End
		LCM_WR_REG(0x0052, vsa);//Window Vertical RAM Address Start
		LCM_WR_REG(0x0053, vea);//Window Vertical RAM Address End

		LCM_WR_REG(0x0020, hsa);
		LCM_WR_REG(0x0021, vsa);

		// Send command after which the data we sent are recognized as pixels.
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



