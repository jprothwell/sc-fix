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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/lcdd/st7787/lcdc/src/drv_lcd_st7787.c $ //
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
#define LCM_WR_DAT(Data)            { while(hal_LcdcWriteData(Data)        != HAL_ERR_NO);}
#define LCM_WR_CMD(Cmd)            { while(hal_LcdcWriteCmd(Cmd)         != HAL_ERR_NO);}


// =============================================================================
// Screen properties
// =============================================================================

// Number of actual pixels in the display width 
#define LCDD_DISP_X		240

// Number of pixels in the display height 
#define LCDD_DISP_Y		320

#define  DRV_TEST			1	//add by puyong@2009-05-05

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

	

#ifdef  DRV_TEST		//add by puyong@2009-05-05
SXS_TRACE(TSTDOUT, "========== lcdd_Open   st7787	");   

 
	//hal_TimDelay(100 MS_WAITING);			   
	hal_TimDelay(120 MS_WAITING);			   


	//hal_TimDelay(40 MS_WAITING);		

	LCM_WR_CMD(0x11);	//sleep_out
	hal_TimDelay(120 MS_WAITING);

	LCM_WR_CMD(0xb1);	//normal_frame
	LCM_WR_DAT(0x26);
	LCM_WR_DAT(0x02);
	LCM_WR_DAT(0x02);

	//hal_TimDelay(40 MS_WAITING);				

	LCM_WR_CMD(0xb2);	//idle_frame
	LCM_WR_DAT(0x3a);
	LCM_WR_DAT(0x10);
	LCM_WR_DAT(0x10);

	LCM_WR_CMD(0xb3);	//partial_frame
	LCM_WR_DAT(0x3e);
	LCM_WR_DAT(0x02);
	LCM_WR_DAT(0x02);
	LCM_WR_DAT(0x3e);
	LCM_WR_DAT(0x02);
	LCM_WR_DAT(0x02);				

	LCM_WR_CMD(0xb6);	//DISPLAY FUNCTION SET
	LCM_WR_DAT(0x02);
	LCM_WR_DAT(0x00);		

	LCM_WR_CMD(0xbc);	//	 
	hal_TimDelay(100 MS_WAITING);		

	LCM_WR_CMD(0xc0);	//set GVDD
	LCM_WR_DAT(0x00);		

	LCM_WR_CMD(0xc1);	//Power Control 2 ( VGH,VGL)
	LCM_WR_DAT(0xa7);
	LCM_WR_DAT(0x00);	

	LCM_WR_CMD(0xc2);	//Power Control 3 ( in normal)
	LCM_WR_DAT(0x03);
	LCM_WR_DAT(0xe6);
	LCM_WR_DAT(0x86);
	LCM_WR_DAT(0x32);
	LCM_WR_DAT(0x03);

	//hal_TimDelay(40 MS_WAITING);				

	LCM_WR_CMD(0xc3);	//Power Control 4 ( in Idle mode)
	LCM_WR_DAT(0x04);
	LCM_WR_DAT(0xc5);
	LCM_WR_DAT(0x05);
	LCM_WR_DAT(0x00);
	LCM_WR_DAT(0x00);				

	//hal_TimDelay(40 MS_WAITING);				

	LCM_WR_CMD(0xc4);	//Power Control 4 ( in partial mode)
	LCM_WR_DAT(0x01);
	LCM_WR_DAT(0xb3);
	LCM_WR_DAT(0x03);
	LCM_WR_DAT(0x00);
	LCM_WR_DAT(0x00);				  

	//hal_TimDelay(40 MS_WAITING);	

	LCM_WR_CMD(0xf4);	//Setting ARBIT_2
	LCM_WR_DAT(0xff);
	LCM_WR_DAT(0x3f);				 

	LCM_WR_CMD(0xf5);
	LCM_WR_DAT(0x10);

	LCM_WR_CMD(0xfb);	//
	LCM_WR_DAT(0x7f);

	LCM_WR_CMD(0xc5);	//vcomH set
	LCM_WR_DAT(0xcb);		;//33,3a,3b
	LCM_WR_DAT(0x25);
	LCM_WR_DAT(0x80);

	LCM_WR_CMD(0xc6);	//VcomAC set
	LCM_WR_DAT(0x25);		   //13,19,1c
	LCM_WR_DAT(0x00);

	LCM_WR_CMD(0x20);	//DISPLAY INVERSE

	LCM_WR_CMD(0x3a);	//65k
	LCM_WR_DAT(0x05);

	LCM_WR_CMD(0x36);	// mx.my,mv,rgb
	LCM_WR_DAT(0x00);	

	LCM_WR_CMD(0xe0);	//gamma
	LCM_WR_DAT(0x03);
	LCM_WR_DAT(0x03);
	LCM_WR_DAT(0x18);
	LCM_WR_DAT(0x1c);
	LCM_WR_DAT(0x19);	
	LCM_WR_DAT(0x14);
	LCM_WR_DAT(0x10);
	LCM_WR_DAT(0x10);
	LCM_WR_DAT(0x18);
	LCM_WR_DAT(0x00);	
	LCM_WR_DAT(0x0f);
	LCM_WR_DAT(0x03);
	LCM_WR_DAT(0x07);

	LCM_WR_CMD(0xe1);	//gamma
	LCM_WR_DAT(0x08);
	LCM_WR_DAT(0x07);
	LCM_WR_DAT(0x1d);
	LCM_WR_DAT(0x1f);
	LCM_WR_DAT(0x1e);	
	LCM_WR_DAT(0x19);
	LCM_WR_DAT(0x15);
	LCM_WR_DAT(0x14);
	LCM_WR_DAT(0x12);
	LCM_WR_DAT(0x05);	
	LCM_WR_DAT(0x0f);
	LCM_WR_DAT(0x03);
	LCM_WR_DAT(0x03);		

	LCM_WR_CMD(0x2A);	//Column Range	
	LCM_WR_DAT(0x000);
	LCM_WR_DAT(0x000);
	LCM_WR_DAT(0x000);
	LCM_WR_DAT(0x0ef);

	LCM_WR_CMD(0x2B);	//Page Range	  
	LCM_WR_DAT(0x000);	
	LCM_WR_DAT(0x000); 
	LCM_WR_DAT(0x001);	
	LCM_WR_DAT(0x03F);		

	LCM_WR_CMD(0x29);	//display on 			
	LCM_WR_CMD(0x2c);	//write
#endif

	// Allow access 
	g_lcddLock = 1;
	g_lcddOpened = TRUE;

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
		//lcdd_Sleep();
	}
	else
	{
		//lcdd_WakeUp();
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

#ifdef  DRV_TEST		//add by puyong@2009-05-05
SXS_TRACE(TSTDOUT, "========== lcdd_Sleep   "); 

	LCM_WR_CMD(0x28);
	LCM_WR_CMD(0x10);			
	hal_TimDelay(100 MS_WAITING);
#endif

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
	LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);	

#ifdef  DRV_TEST		//add by puyong@2009-05-05
#if 0
	lcdd_Open();
#else
SXS_TRACE(TSTDOUT, "========== lcdd_WakeUp   "); 


hal_TimDelay(200 MS_WAITING);

LCM_WR_CMD(0x11);	//sleep_out
hal_TimDelay(120 MS_WAITING);

LCM_WR_CMD(0xb1);	//normal_frame
LCM_WR_DAT(0x26);
LCM_WR_DAT(0x02);
LCM_WR_DAT(0x02);

//hal_TimDelay(40 MS_WAITING);				

LCM_WR_CMD(0xb2);	//idle_frame
LCM_WR_DAT(0x3a);
LCM_WR_DAT(0x10);
LCM_WR_DAT(0x10);

LCM_WR_CMD(0xb3);	//partial_frame
LCM_WR_DAT(0x3e);
LCM_WR_DAT(0x02);
LCM_WR_DAT(0x02);
LCM_WR_DAT(0x3e);
LCM_WR_DAT(0x02);
LCM_WR_DAT(0x02);				

LCM_WR_CMD(0xb6);	//DISPLAY FUNCTION SET
LCM_WR_DAT(0x02);
LCM_WR_DAT(0x00);		

LCM_WR_CMD(0xbc);	//	 
hal_TimDelay(100 MS_WAITING);		

LCM_WR_CMD(0xc0);	//set GVDD
LCM_WR_DAT(0x00);		

LCM_WR_CMD(0xc1);	//Power Control 2 ( VGH,VGL)
LCM_WR_DAT(0xa7);
LCM_WR_DAT(0x00);	

LCM_WR_CMD(0xc2);	//Power Control 3 ( in normal)
LCM_WR_DAT(0x03);
LCM_WR_DAT(0xe6);
LCM_WR_DAT(0x86);
LCM_WR_DAT(0x32);
LCM_WR_DAT(0x03);

//hal_TimDelay(40 MS_WAITING);				

LCM_WR_CMD(0xc3);	//Power Control 4 ( in Idle mode)
LCM_WR_DAT(0x04);
LCM_WR_DAT(0xc5);
LCM_WR_DAT(0x05);
LCM_WR_DAT(0x00);
LCM_WR_DAT(0x00);				

//hal_TimDelay(40 MS_WAITING);				

LCM_WR_CMD(0xc4);	//Power Control 4 ( in partial mode)
LCM_WR_DAT(0x01);
LCM_WR_DAT(0xb3);
LCM_WR_DAT(0x03);
LCM_WR_DAT(0x00);
LCM_WR_DAT(0x00);				  

//hal_TimDelay(40 MS_WAITING);	

LCM_WR_CMD(0xf4);	//Setting ARBIT_2
LCM_WR_DAT(0xff);
LCM_WR_DAT(0x3f);				 

LCM_WR_CMD(0xf5);
LCM_WR_DAT(0x10);

LCM_WR_CMD(0xfb);	//
LCM_WR_DAT(0x7f);

LCM_WR_CMD(0xc5);	//vcomH set
LCM_WR_DAT(0xcb);		;//33,3a,3b
LCM_WR_DAT(0x25);
LCM_WR_DAT(0x80);

LCM_WR_CMD(0xc6);	//VcomAC set
LCM_WR_DAT(0x25);		   //13,19,1c
LCM_WR_DAT(0x00);

LCM_WR_CMD(0x20);	//DISPLAY INVERSE

LCM_WR_CMD(0x3a);	//65k
LCM_WR_DAT(0x05);

LCM_WR_CMD(0x36);	// mx.my,mv,rgb
LCM_WR_DAT(0x00);	

LCM_WR_CMD(0xe0);	//gamma
LCM_WR_DAT(0x03);
LCM_WR_DAT(0x03);
LCM_WR_DAT(0x18);
LCM_WR_DAT(0x1c);
LCM_WR_DAT(0x19);	
LCM_WR_DAT(0x14);
LCM_WR_DAT(0x10);
LCM_WR_DAT(0x10);
LCM_WR_DAT(0x18);
LCM_WR_DAT(0x00);	
LCM_WR_DAT(0x0f);
LCM_WR_DAT(0x03);
LCM_WR_DAT(0x07);

LCM_WR_CMD(0xe1);	//gamma
LCM_WR_DAT(0x08);
LCM_WR_DAT(0x07);
LCM_WR_DAT(0x1d);
LCM_WR_DAT(0x1f);
LCM_WR_DAT(0x1e);	
LCM_WR_DAT(0x19);
LCM_WR_DAT(0x15);
LCM_WR_DAT(0x14);
LCM_WR_DAT(0x12);
LCM_WR_DAT(0x05);	
LCM_WR_DAT(0x0f);
LCM_WR_DAT(0x03);
LCM_WR_DAT(0x03);		

LCM_WR_CMD(0x2A);	//Column Range	
LCM_WR_DAT(0x000);
LCM_WR_DAT(0x000);
LCM_WR_DAT(0x000);
LCM_WR_DAT(0x0ef);

LCM_WR_CMD(0x2B);	//Page Range	  
LCM_WR_DAT(0x000);	
LCM_WR_DAT(0x000); 
LCM_WR_DAT(0x001);	
LCM_WR_DAT(0x03F);		

LCM_WR_CMD(0x29);	//display on			
LCM_WR_CMD(0x2c);	//write

#endif
#endif
	return LCDD_ERR_NO;
}


// ============================================================================
// lcdd_PartialOn
// ----------------------------------------------------------------------------
/// Set the Partial mode of the LCD
/// @param y1 : Vertical Start Active
/// @param y2 : Vertical End Active
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_PartialOn(UINT16 y1, UINT16 y2)
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

//SXS_TRACE(TSTDOUT, "\n========== lcdd_SetPixel16   x=%d	y=%d   pixelData=%d", x,y,pixelData); 
	if (0 == lcdd_MutexGet(&g_lcddLock))
	{
		return LCDD_ERR_RESOURCE_BUSY;
	}
	else
	{
#ifdef  DRV_TEST		//add by puyong@2009-05-05
SXS_TRACE(TSTDOUT, "\n========== lcdd_SetPixel16   "); 
		LCM_WR_CMD(0x2A);
		LCM_WR_DAT(x>>8);			 
		LCM_WR_DAT(x&0xff);			  
		LCM_WR_DAT(x>>8); 
		LCM_WR_DAT(x&0xff); 

		LCM_WR_CMD(0x2B);
		LCM_WR_DAT(y>>8);			  
		LCM_WR_DAT(y&0xff);			  
		LCM_WR_DAT(y>>8); 
		LCM_WR_DAT(y&0xff); 

		LCM_WR_CMD(0x2C);
		//LCM_WR_DAT(pixelData); 
		//LCM_WR_DAT(pixelData); 

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

	// Active window coordinates.
	UINT16 x1 = 1;
	UINT16 x2 = 1;
	UINT16 y1 = 1;
	UINT16 y2 = 1;

	if (0 == lcdd_MutexGet(&g_lcddLock))
	{
		return LCDD_ERR_RESOURCE_BUSY;
	}
	else
	{   
		// Set Active window
		x1 = regionOfInterrest->x;
		x2 = regionOfInterrest->x + regionOfInterrest->width - 1;
		y1 = regionOfInterrest->y;
		y2 = regionOfInterrest->y + regionOfInterrest->height - 1;

		// Check parameters
		// ROI must be within the screen boundary
		if (    (x2 >= LCDD_DISP_X) ||
			(x1 >= LCDD_DISP_X) ||
			(y2 >= LCDD_DISP_Y) ||
			(y1 >= LCDD_DISP_Y) 	)
		{
		lcdd_MutexFree(&g_lcddLock);               
		return LCDD_ERR_INVALID_PARAMETER;
		}

#ifdef  DRV_TEST		//add by puyong@2009-05-05
SXS_TRACE(TSTDOUT, "\n========== lcdd_FillRect16   x1=%d  x2=%d  y1=%d  y2=%d   bgColor=%d", x1,x2,y1,y2,bgColor);

		LCM_WR_CMD(0x2A);
		LCM_WR_DAT(x1>>8);			 
		LCM_WR_DAT(x1&0xff);			  
		LCM_WR_DAT(x2>>8); 
		LCM_WR_DAT(x2&0xff); 

		LCM_WR_CMD(0x2B);
		LCM_WR_DAT(y1>>8);			  
		LCM_WR_DAT(y1&0xff);			  
		LCM_WR_DAT(y2>>8); 
		LCM_WR_DAT(y2&0xff); 

		LCM_WR_CMD(0x2C);
		//LCM_WR_DAT(bgColor); 
		//LCM_WR_DAT(bgColor>>8); 
		//LCM_WR_DAT(bgColor&0xff); 
#endif

		UINT32 x;
		for (x = 1; x < (x2-x1+1) * (y2-y1+1); ++x)
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
	LCDD_ASSERT((frameBufferWin->fb.width&1) == 0, "LCDD: FBW must have an even number " "of pixels per line. Odd support is possible at the price of a huge " "performance lost");

	// Active window coordinates.
	UINT32 x1 = 1;
	UINT32 x2 = 1;
	UINT32 y1 = 1;
	UINT32 y2 = 1;

	// Added pixel (first pixel) if the ROI start is not 32 bits aligned.
	UINT32 deltaOddStart = 0;
	// Added
	UINT32 deltaOddLength = 0;

SXS_TRACE(TSTDOUT, "\n========== lcdd_Blit16   00 startX=%d  startY=%d  x=%d  y=%d  width=%d  height=%d  fb.width1=%d  fb.height1=%d", startX,startY, frameBufferWin->roi.x,frameBufferWin->roi.y,frameBufferWin->roi.width,frameBufferWin->roi.height,frameBufferWin->fb.width,frameBufferWin->fb.height);

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
		x1 = startX - deltaOddStart;
		x2 = startX + frameBufferWin->roi.width - 1 + deltaOddLength;
		y1 = startY;
		y2 = startY + frameBufferWin->roi.height - 1;

//SXS_TRACE(TSTDOUT, "\n========== lcdd_Blit16   01 x1=%d  x2=%d  y1=%d  y2=%d deltaOddStart=%d  deltaOddLength=%d  ", x1,x2, y1,y2, deltaOddStart, deltaOddLength);

		// Check parameters
		// ROI must be within the screen boundary
		// ROI must be within the Frame buffer
		// Color format must be 16 bits
		if (    (x2 >= LCDD_DISP_X) ||
			(x1 >= LCDD_DISP_X) ||
			(y2 >= LCDD_DISP_Y) ||
			(y1 >= LCDD_DISP_Y) ||
			(frameBufferWin->roi.width + deltaOddStart + deltaOddLength > frameBufferWin->fb.width) ||
			(frameBufferWin->roi.height > frameBufferWin->fb.height) ||
			(frameBufferWin->fb.colorFormat != LCDD_COLOR_FORMAT_RGB_565)		)
		{
			lcdd_MutexFree(&g_lcddLock);  
			return LCDD_ERR_INVALID_PARAMETER;
		}

#ifdef  DRV_TEST		//add by puyong@2009-05-05
//SXS_TRACE(TSTDOUT, "\n========== lcdd_Blit16   02 x1=%d  x2=%d  y1=%d  y2=%d  ", x1,x2, y1,y2);    

		LCM_WR_CMD(0x2A);
		LCM_WR_DAT(x1>>8);			 
		LCM_WR_DAT(x1&0xff);			  
		LCM_WR_DAT(x2>>8); 
		LCM_WR_DAT(x2&0xff); 

		LCM_WR_CMD(0x2B);
		LCM_WR_DAT(y1>>8);			  
		LCM_WR_DAT(y1&0xff);			  
		LCM_WR_DAT(y2>>8); 
		LCM_WR_DAT(y2&0xff); 

		LCM_WR_CMD(0x2C);
#endif

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
			UINT16* curBuf = 	   frameBufferWin->fb.buffer
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

