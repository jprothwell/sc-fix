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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/lcdd/src/lcdd.c $ //
//	$Author: yanshengzhang $                                                        // 
//	$Date: 2011-06-23 11:57:06 +0800 (Thu, 23 Jun 2011) $                     //   
//	$Revision: 8454 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file lcdd.c                                                           //
/// That file provides an implementation for the  LCD screen.         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"
#include "lcdd_m.h"
#include "lcdd.h"
#include "lcddp_debug.h"
#include "tgt_m.h"

#include "hal_sys.h"
#include "pmd_m.h"

#if defined(RECV_USE_SPK_LINE) || defined(AUD_EXT_PA_ON_WITH_LCD)
#ifdef RECV_USE_SPK_LINE
extern BOOL g_halAudRecvUseSpkLineRight;
#endif // RECV_USE_SPK_LINE
extern VOID aud_LcdPowerOnCallback(VOID);
extern VOID aud_LcdPowerOffCallback(VOID);
#endif // RECV_USE_SPK_LINE || AUD_EXT_PA_ON_WITH_LCD

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

PRIVATE LCDD_REG_T g_lcddInitTable;
PRIVATE LCDD_REG_T  *g_lcddDrv;
PRIVATE BOOL g_lcddLoadSuccess=FALSE;

BOOL (*g_LcddRegInit[])(LCDD_REG_T *) = { LCDD_REGINIT_LIST }; 
#define LCDD_REGINIT_NUM   sizeof(g_LcddRegInit) /sizeof(UINT32) 

// Lock to protect the access to the LCD screen during a DMA access.
// When 1, access is allowed. When 0, it is denied.
PRIVATE VOLATILE UINT32 g_lcddLock = 0;

// Opened status of the driver, used to check that no 
// call to blit is done before the driver is opened
PRIVATE BOOL g_lcddOpened = FALSE;

PRIVATE UINT8 g_lcdSemaphore = 0xff;
PRIVATE BOOL g_lcdSemaphoreBeenTaken=FALSE;

void lcd_SemFree(void)
{
    sxr_ReleaseSemaphore(g_lcdSemaphore);
}
void lcd_SemTake(void)
{
    UINT32 state=hal_SysEnterCriticalSection();
    if(g_lcdSemaphore==0xff)
        g_lcdSemaphore=sxr_NewSemaphore(1);
    
    hal_SysExitCriticalSection(state);
    
    sxr_TakeSemaphore(g_lcdSemaphore);
}

void lcd_lock_all_blt_operation(void)
{
    if(g_lcdSemaphoreBeenTaken)
        return;
    g_lcdSemaphoreBeenTaken = TRUE;
    gdi_layer_lock_frame_buffer();
    lcd_SemTake();
}

void lcd_unlock_all_blt_operation(void)
{
    if(!g_lcdSemaphoreBeenTaken  )
        return;
    g_lcdSemaphoreBeenTaken = FALSE;
    lcd_SemFree();
    gdi_layer_unlock_frame_buffer();
}
// =============================================================================
//  FUNCTIONS
// =============================================================================


// =============================================================================
// lcdd_MutexFree
// -----------------------------------------------------------------------------
/// Free a previously taken mutex. The ownership of the mutex is not checked.
/// Only free a mutex you have previously got.
/// @param
// =============================================================================
PROTECTED VOID lcdd_MutexFree(VOID)
{
    // Writing is an atomic operation
    g_lcddLock = 1;
}


// =============================================================================
// lcdd_MutexGet
// -----------------------------------------------------------------------------
/// This function enter in critical section, read the value of the mutex and,
/// if this is a '1', returns '1' and set the mutex to 0. If this is a '0',
/// it does nothing. Then, in both cases, it exists the Critical Section.
///
/// @param
/// @return '1' if the mutex was taken, '0' otherwise.
// =============================================================================
PROTECTED UINT32 lcdd_MutexGet(VOID)
{
    UINT32 status;
    UINT32 result;

    status = hal_SysEnterCriticalSection();
    if (g_lcddLock == 1)
    {
        // Take the mutex
        g_lcddLock = 0;
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



// ============================================================================
// lcdd_InitTable
// ----------------------------------------------------------------------------
/// This function is called in the lcdd_LoadDrv for init function pointer
// ============================================================================
PRIVATE VOID  lcdd_InitTable(LCDD_REG_T  *pLcd)
{
    pLcd->lcdd_Open=NULL;
    pLcd->lcdd_Close=NULL;
    pLcd->lcdd_SetContrast=NULL;
   // pLcd->lcdd_SetBrightness=NULL;	
    pLcd->lcdd_SetStandbyMode=NULL;
    pLcd->lcdd_PartialOn=NULL;
    pLcd->lcdd_PartialOff=NULL;
    pLcd->lcdd_Blit16=NULL;
    pLcd->lcdd_Busy=NULL;
    pLcd->lcdd_FillRect16=NULL;
    pLcd->lcdd_GetScreenInfo=NULL;
    pLcd->lcdd_WakeUp=NULL;
    pLcd->lcdd_SetPixel16=NULL;
    pLcd->lcdd_Sleep=NULL;
    pLcd->lcdd_SetDirRotation=NULL;
    pLcd->lcdd_SetDirDefault=NULL;	
    pLcd->lcdd_GetStringId=NULL;
}

// ============================================================================
// lcdd_LoadDrv
// ----------------------------------------------------------------------------
/// Load the the right LCDD driver by checking LCD ID.
/// This function is called in the lcdd_open 
/// @return #TRUE or #FALSE.
// ============================================================================
PRIVATE BOOL lcdd_LoadDrv(VOID)
{
    UINT8 i;
    TGT_FACTORY_SETTINGS_T *pFS;
	
#ifdef RECV_USE_SPK_LINE
#ifdef RECV_USE_SPK_LINE_R
    g_halAudRecvUseSpkLineRight = TRUE;
#endif // RECV_USE_SPK_LINE_R
#endif // RECV_USE_SPK_LINE

    g_lcddDrv=&g_lcddInitTable;	
    lcdd_InitTable(g_lcddDrv);

    pFS = tgt_GetFactorySettings();
    
    if ((*pFS).moduleId.lcd_id < LCDD_REGINIT_NUM)
    {// Valid ID
        if(g_LcddRegInit[(*pFS).moduleId.lcd_id](g_lcddDrv))
        {        
            g_lcddLoadSuccess=TRUE;
            LCDD_TRACE(LCDD_INFO_TRC|TSTDOUT, 0, "Lcdd load sucess, ID is %d", (*pFS).moduleId.lcd_id);
            return TRUE;
        }
    }

    for (i = 0; i < LCDD_REGINIT_NUM;  i++) 
    {  
        if(g_LcddRegInit[i](g_lcddDrv))
        {     
            TGT_FACTORY_SETTINGS_T factorySettings;
            factorySettings = *pFS;
            g_lcddLoadSuccess=TRUE;
            LCDD_TRACE(LCDD_INFO_TRC|TSTDOUT, 0, "Lcdd load sucess, ID is %d", i);
            factorySettings.moduleId.lcd_id = i;
            tgt_SetFactorySettings(&factorySettings);
            return TRUE;
        }
    } 
    return FALSE;
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
    LCDD_ERR_T ret = LCDD_ERR_NO;

    // Allow access to other LCD functions
    g_lcddOpened = TRUE;

    if (lcdd_LoadDrv())
    {
        if (g_lcddDrv->lcdd_Open != NULL)
        {
            lcdd_SetBrightness(0);
            ret = g_lcddDrv->lcdd_Open();
            if (ret == LCDD_ERR_NO)
            {
                // Allow access 
                g_lcddLock = 1;
#if defined(RECV_USE_SPK_LINE) || defined(AUD_EXT_PA_ON_WITH_LCD)
                aud_LcdPowerOnCallback();
#endif // RECV_USE_SPK_LINE || AUD_EXT_PA_ON_WITH_LCD
            }
        }
    }
    else
    {
        LCDD_TRACE(LCDD_INFO_TRC, 0, "Lcdd load error"); 		
    }
    //if not find lcdd ,must return LCDD_ERR_NO for produce
     return ret;
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
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);

    LCDD_ERR_T ret = LCDD_ERR_NO;

    // Forbid any access to other LCD functions
    g_lcddOpened = FALSE;

    if (TRUE == g_lcddLoadSuccess)
    { 
        g_lcddLoadSuccess=FALSE;
        if (g_lcddDrv->lcdd_Close != NULL)
        {
            lcdd_SetBrightness(0);
            ret = g_lcddDrv->lcdd_Close();
            if (ret == LCDD_ERR_NO)
            {
                // Disable access
                g_lcddLock = 0;
            }
        }
    }
    
    return ret;
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

    if(TRUE==g_lcddLoadSuccess)
    {
        if(g_lcddDrv->lcdd_SetContrast!=NULL)
        return g_lcddDrv->lcdd_SetContrast(contrast);
    }	
    return LCDD_ERR_NO;
}

// ============================================================================
// lcdd_SetBrightness
// ----------------------------------------------------------------------------
/// Set the brightness of the 'main'LCD screen.
/// @param brightness Value to set the brightness to.
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED or 
/// #LCDD_ERR_INVALID_PARAMETER.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_SetBrightness(UINT32 brightness)
{
    if (brightness>7) 
    {
        return LCDD_ERR_INVALID_PARAMETER;
    }
    pmd_SetLevel(PMD_LEVEL_LCD, brightness);
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

    if(TRUE==g_lcddLoadSuccess)
    {
        if(g_lcddDrv->lcdd_SetStandbyMode!=NULL)
        return g_lcddDrv->lcdd_SetStandbyMode(standbyMode);
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
    
    if(TRUE==g_lcddLoadSuccess)
    {
        if(g_lcddDrv->lcdd_PartialOn!=NULL)
        return g_lcddDrv->lcdd_PartialOn( vsa,  vea);
    }
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
  
    if(TRUE==g_lcddLoadSuccess)
    {
        if(g_lcddDrv->lcdd_PartialOff!=NULL)
        return g_lcddDrv->lcdd_PartialOff();
    }
    return LCDD_ERR_NO;
}


// ============================================================================
// lcdd_WakeUp
// ----------------------------------------------------------------------------
/// Wake the main LCD screen out of sleep mode
/// @return #LCDD_ERR_NO, #LCDD_ERR_NOT_OPENED
// ============================================================================
#if defined(SYS_PSRAM_EBC_DYNAMIC_CLK_CFG_SUPPORT)
extern VOID hal_EbcCsSetup(HAL_SYS_MEM_FREQ_T ebcFreq);
#endif
PUBLIC LCDD_ERR_T lcdd_WakeUp(VOID)
{
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);

    if(TRUE==g_lcddLoadSuccess)
    {
#if defined(SYS_PSRAM_EBC_DYNAMIC_CLK_CFG_SUPPORT)
         hal_EbcCsSetup(HAL_SYS_FREQ_104M);
#endif
        
        if(g_lcddDrv->lcdd_WakeUp!=NULL)
        {
#if defined(RECV_USE_SPK_LINE) || defined(AUD_EXT_PA_ON_WITH_LCD)
            aud_LcdPowerOnCallback();
#endif // RECV_USE_SPK_LINE || AUD_EXT_PA_ON_WITH_LCD
			// Power on LCD last to save energy
            return g_lcddDrv->lcdd_WakeUp();
        }
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

    if(TRUE==g_lcddLoadSuccess)
    {
#if defined(SYS_PSRAM_EBC_DYNAMIC_CLK_CFG_SUPPORT)
        hal_EbcCsSetup(HAL_SYS_FREQ_52M);
#endif
        if(g_lcddDrv->lcdd_Sleep!=NULL)
        {
			// Power off LCD first to save energy
			LCDD_ERR_T ret = g_lcddDrv->lcdd_Sleep();
#if defined(RECV_USE_SPK_LINE) || defined(AUD_EXT_PA_ON_WITH_LCD)
            aud_LcdPowerOffCallback();
#endif // RECV_USE_SPK_LINE || AUD_EXT_PA_ON_WITH_LCD
            return ret;
        }
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

    if(TRUE==g_lcddLoadSuccess)
    {
        if(g_lcddDrv->lcdd_GetScreenInfo!=NULL)
        return g_lcddDrv->lcdd_GetScreenInfo(screenInfo);
    }
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

    if(TRUE==g_lcddLoadSuccess)
    {
        if(g_lcddDrv->lcdd_SetPixel16!=NULL)
        return g_lcddDrv->lcdd_SetPixel16( x,  y,  pixelData);
    }
    return LCDD_ERR_NO;
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
    
    if(TRUE==g_lcddLoadSuccess)
    {
#if (CHIP_HAS_GOUDA == 1) && defined(__PRJ_WITH_SPILCD__)
        hal_GoudaSetBgColor(bgColor);
#endif
        if(g_lcddDrv->lcdd_FillRect16!=NULL)
        return g_lcddDrv->lcdd_FillRect16(regionOfInterrest,bgColor);
    }
    return LCDD_ERR_NO;
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
#if (CHIP_HAS_GOUDA == 1)
// whether lcddp_GoudaBlitHandler() has to close merged overlay layers
PRIVATE UINT8 g_lcddAutoCloseMergedLayer = 0;
#endif

PUBLIC LCDD_ERR_T lcdd_Blit16(CONST LCDD_FBW_T* frameBufferWin, UINT16 startX, UINT16 startY)
{
    LCDD_ERR_T ret=LCDD_ERR_NO;
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);
    if(TRUE==g_lcddLoadSuccess)
    {
    #if (CHIP_HAS_GOUDA == 1) && defined(__PRJ_WITH_SPILCD__)
    
        lcdd_LayerMerge(frameBufferWin,startX,startY,FALSE);
        while(g_lcddAutoCloseMergedLayer)
        {
            sxr_Sleep(64);
        }
    #endif
        if(g_lcddDrv->lcdd_Blit16!=NULL)
        {
            lcd_SemTake();
            ret = g_lcddDrv->lcdd_Blit16(frameBufferWin,startX,startY);
            lcd_SemFree();
            return ret;
        }
    }
    return LCDD_ERR_NO;
}



// ============================================================================
// lcdd_Busy
// ----------------------------------------------------------------------------
/// This function is not implemented now for the driver
// ============================================================================
PUBLIC BOOL lcdd_Busy(VOID)
{
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);

    if(TRUE==g_lcddLoadSuccess)
    {
        if(g_lcddDrv->lcdd_Busy!=NULL)
        return g_lcddDrv->lcdd_Busy();
    }
    return FALSE;
}

PUBLIC char* lcdd_GetStringId(VOID)
{
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);

    if(TRUE==g_lcddLoadSuccess)
    {
        if(g_lcddDrv->lcdd_GetStringId!=NULL)
        return g_lcddDrv->lcdd_GetStringId();
    }
    return NULL;
}

// ============================================================================
// lcdd_SetDirRotation
// ----------------------------------------------------------------------------
///  This function set lcd show rotation
// ============================================================================
PUBLIC BOOL lcdd_SetDirRotation(VOID)
{
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);

    if(TRUE==g_lcddLoadSuccess)
    {
        if(g_lcddDrv->lcdd_SetDirRotation!=NULL)
        return g_lcddDrv->lcdd_SetDirRotation();
    }
    return FALSE;
}



// ============================================================================
// lcdd_SetDirDefault
// ----------------------------------------------------------------------------
//  This function set lcd show default
// ============================================================================
PUBLIC BOOL lcdd_SetDirDefault(VOID)
{
    LCDD_ASSERT(g_lcddOpened, "LCDD: Call to %s while LCDD is not opened", __FUNCTION__);

    if(TRUE==g_lcddLoadSuccess)
    {
        if(g_lcddDrv->lcdd_SetDirDefault!=NULL)
        return g_lcddDrv->lcdd_SetDirDefault();
    }
    return FALSE;
}
  

#if (CHIP_HAS_GOUDA == 1)

#include "hal_gouda.h"
#include "mci_lcd.h"

#define OVERLAY_ALL_MAP 1

// whether lcddp_GoudaBlitHandler() has to close merged overlay layers
//PRIVATE UINT8 g_lcddAutoCloseMergedLayer = 0;

// =============================================================================
// lcdd_GoudaMergeHandler
// -----------------------------------------------------------------------------
/// This function frees the lock to access the lcd driver. It is set as the user 
/// handler called by the DMA driver at the end of the writings on the merged buffer.
// =============================================================================
PRIVATE VOID lcdd_GoudaMergeHandler(VOID)
{
    lcdd_MutexFree();
	if(g_lcddAutoCloseMergedLayer==1)
	{
	    hal_GoudaVidLayerClose();
	    g_lcddAutoCloseMergedLayer = 0;
	}
	else if(g_lcddAutoCloseMergedLayer==2)
	{
		hal_GoudaOvlLayerClose(HAL_GOUDA_OVL_LAYER_ID0);
		g_lcddAutoCloseMergedLayer = 0;
	}
	else if(g_lcddAutoCloseMergedLayer==3)
	{
		g_lcddAutoCloseMergedLayer = 0;
	}
}


// ============================================================================
// lcdd_BlitRoi2Merge
// ----------------------------------------------------------------------------
// Private function to merge overlay layers into a given buffer
// ============================================================================
PRIVATE VOID lcdd_BlitRoi2Merge(CONST HAL_GOUDA_WINDOW_T* pRoi, UINT16 *layer_Merge_buffer)
{
    if(!((pRoi->tlPX < pRoi->brPX) && (pRoi->tlPY < pRoi->brPY)))
    {
        lcdd_GoudaMergeHandler();
        return;
    }
    // building set roi sequence:
    while(HAL_ERR_NO != hal_GoudaBlitRoi2Ram((UINT32*)layer_Merge_buffer, 0, pRoi, lcdd_GoudaMergeHandler));
}

extern PUBLIC MCI_LCD_BPFUN_T g_mciLcdBypassFunction;

// ============================================================================
// lcdd_LayerMerge
// ----------------------------------------------------------------------------
/// This function provides the layer merging capabilities. 
/// This function merges the overlay layer data to a given buffer or to a LCD screen.
///
/// @param frameBufferWin Pointer to the LCD frame buffer structure.
/// @param startX The coordinates of the start position.
/// @param startY The coordinates of the start position.
/// @return #LCDD_ERR_NO, #LCDD_ERR_RESOURCE_BUSY or #LCDD_ERR_INVALID_PARAMETER.
// ============================================================================
PUBLIC LCDD_ERR_T lcdd_LayerMerge(CONST LCDD_FBW_T* frameBufferWin, UINT16 startX, UINT16 startY,BOOL merge_state)
{
    // Active window coordinates.
    HAL_GOUDA_WINDOW_T inputWin;
    
    if(TRUE==g_lcddLoadSuccess)
    {

        if (0 == lcdd_MutexGet())
        {
            return LCDD_ERR_RESOURCE_BUSY;
        }
        else
        {
#ifdef OVERLAY_ALL_MAP
            // Set Input window
            inputWin.tlPX = 0;
            inputWin.brPX =(frameBufferWin->fb.width )- 1;
            inputWin.tlPY = 0;
            inputWin.brPY = (frameBufferWin->fb.height) - 1;
#else // !OVERLAY_ALL_MAP
            // Set Input window
            inputWin.tlPX = frameBufferWin->roi.x;
            inputWin.brPX = frameBufferWin->roi.x + frameBufferWin->roi.width - 1;
            inputWin.tlPY = frameBufferWin->roi.y;
            inputWin.brPY = frameBufferWin->roi.y + frameBufferWin->roi.height - 1;
#endif // !OVERLAY_ALL_MAP

            // Check parameters
            // ROI must be within the screen boundary
            // ROI must be within the Frame buffer
            // Color format must be 16 bits
            BOOL badParam = FALSE;
            if (!badParam)
            {
                if (    (frameBufferWin->roi.width > frameBufferWin->fb.width) ||
                        (frameBufferWin->roi.height > frameBufferWin->fb.height) ||
                        (frameBufferWin->fb.colorFormat != LCDD_COLOR_FORMAT_RGB_565)
                    )
                {
                    badParam = TRUE;;
                }
            }
            if (badParam)
            {
                lcdd_MutexFree();
                return LCDD_ERR_INVALID_PARAMETER;
            }

     if(merge_state)
     {
            if (frameBufferWin->fb.buffer != NULL )
            {
    		if(hal_GoudaVidLayerState()==0 || g_mciLcdBypassFunction!=NULL)
    		{
    		        HAL_GOUDA_VID_LAYER_DEF_T   gouda_vid_def = {0, };

    		        gouda_vid_def.fmt = HAL_GOUDA_IMG_FORMAT_RGB565;

    		        gouda_vid_def.addrY = (UINT32*)frameBufferWin->fb.buffer; // what about aligment ?                    

    		        gouda_vid_def.alpha = 0xFF;
    		        gouda_vid_def.cKeyColor = 0;
    		        gouda_vid_def.cKeyEn = FALSE;
    		        gouda_vid_def.cKeyMask = HAL_GOUDA_CKEY_MASK_OFF;
    		        gouda_vid_def.depth = HAL_GOUDA_VID_LAYER_BEHIND_ALL;

    		        gouda_vid_def.stride = 0;
#ifdef OVERLAY_ALL_MAP
    		        gouda_vid_def.width = frameBufferWin->fb.width;
    		        gouda_vid_def.height = frameBufferWin->fb.height;

    		        gouda_vid_def.pos.tlPX = 0;
    		        gouda_vid_def.pos.tlPY = 0;

    		        gouda_vid_def.pos.brPX = (frameBufferWin->fb.width )- 1;
    		        gouda_vid_def.pos.brPY = (frameBufferWin->fb.height) - 1;
#else // !OVERLAY_ALL_MAP
    		        gouda_vid_def.width = frameBufferWin->roi.width;
    		        gouda_vid_def.height = frameBufferWin->roi.height;

    		        gouda_vid_def.pos.tlPX = 0;
    		        gouda_vid_def.pos.tlPY = 0;

    		        gouda_vid_def.pos.brPX = (frameBufferWin->roi.width )- 1;
    		        gouda_vid_def.pos.brPY = (frameBufferWin->roi.height) - 1;
#endif // !OVERLAY_ALL_MAP
    		        hal_GoudaVidLayerClose();
    		        hal_GoudaVidLayerOpen(&gouda_vid_def);
    		        g_lcddAutoCloseMergedLayer = 1;

    		}
    		else
    		{
    		        HAL_GOUDA_OVL_LAYER_DEF_T def;
    		        // configure ovl layer 0 as buffer
    		        def.addr = (UINT32*)frameBufferWin->fb.buffer; // what about aligment ?
    		        def.fmt = HAL_GOUDA_IMG_FORMAT_RGB565; //TODO convert from .colorFormat
    		        //def.stride = frameBufferWin->fb.width * 2;
    		        def.stride = 0; // let hal gouda decide
    		        def.pos.tlPX = 0;
    		        def.pos.tlPY = 0;
    		        def.pos.brPX = frameBufferWin->fb.width - 1;
    		        def.pos.brPY = frameBufferWin->fb.height - 1;
    		        def.alpha = 255;
    		        def.cKeyEn = FALSE;
    		        //hal_HstSendEvent(0x1ee01);
    		        // open the layer
    		        hal_GoudaOvlLayerClose(HAL_GOUDA_OVL_LAYER_ID0);
    		        hal_GoudaOvlLayerOpen(HAL_GOUDA_OVL_LAYER_ID0, &def);
    		        // tell the end handler to close the layer when we are done ;)
    		        g_lcddAutoCloseMergedLayer = 2;
    		
    		}
                lcdd_BlitRoi2Merge(&inputWin, frameBufferWin->fb.buffer);          
            }
        }
        else
        {
             if (frameBufferWin->fb.buffer != NULL )
            {
      	            g_lcddAutoCloseMergedLayer = 3;
                    lcdd_BlitRoi2Merge(&inputWin, frameBufferWin->fb.buffer);
             }
        }  
            // gouda is doing everything ;)
            return LCDD_ERR_NO;
        }
    }
    return LCDD_ERR_NO;
}

#endif // (CHIP_HAS_GOUDA == 1)

