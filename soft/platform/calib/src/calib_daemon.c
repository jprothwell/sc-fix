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
//  $HeadURL$ 
//	$Author$                                                         
//	$Date$                        
//	$Revision$                                                            
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file calib_daemon.c 
/// This file implements the calibration daemon functionality, for both lab
/// and production modes.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"
#include "chip_id.h"
#include "string.h"

#include "calib_m.h"
#include "calibp_defaults.h"
#include "calibp_debug.h" 
#include "calibp_calibration.h"

#include "pal_gsm.h"
#include "hal_mem_map.h"
#include "hal_ana_gpadc.h"
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
#include "hal_ana_audio.h"
#endif // CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE

#include "memd_m.h"

#include "rfd_pa.h"
#include "rfd_sw.h"
#include "rfd_xcv.h"

#include "vois_m.h"
#include "aud_m.h"
#include "pmd_m.h"
#include "pmd_calib.h"

#include "tgt_aud_cfg.h"

#include "stdio.h"


// =============================================================================
// Definitions
// =============================================================================

#define CALIB_UNCACHED_BSS __attribute__((section(".ucbss")))


// =============================================================================
// _calib_start
// -----------------------------------------------------------------------------
/// Start address of the calibration sector, in the flash.
/// Come from the linker.
// =============================================================================
extern CALIB_BUFFER_T _calib_start;


// =============================================================================
// _calib_size
// -----------------------------------------------------------------------------
/// Size of the calibration data
/// Come from the linker.
// =============================================================================
extern UINT32 _calib_size;

//  -------------------------------------------------- 
//  ----- Address of the calibration sector in flash. 

/// Calibrated parameters buffer (used when the platform has been calibrated).
/// This buffer is in flash and can be re-burnt ONLY using CalibTool.
/// The symbol "_calib_start" is in a dedicated sector of the flash and is
/// not erased while programming the flash. It is defined by LD script. 
CONST CALIB_BUFFER_T* CONST g_calibCalibSector = &_calib_start;



//  -------------------------------------------------- 
//  ----- Default values for baseband calibration. 

///  Default calib values. Used when the platform hasn't been calibrated (the
///  status flag of "g_calibCalibSector" is checked during the calib init). 
CONST CALIB_OP_INFO_T g_calibOpInfoDefault =
{
    0 /* The time of epoch. */, CALIB_METH_DEFAULT
};


/// Baseband default calibration
CONST CALIB_BB_T g_calibBbDefault =
{
    DEFAULT_CALIB_PAL_TIMES,
    {
        // ITF 0 - Receiver
        {
            DEFAULT_CALIB_AUDIO_VOC_HS_EC,
            DEFAULT_CALIB_AUDIO_VOC_HS_FILTERS,
            DEFAULT_CALIB_AUDIO_GAINS_HS,
            DEFAULT_CALIB_AUDIO_HS_PARAMS,
        },

        // ITF 1 - Earpiece
        {
            DEFAULT_CALIB_AUDIO_VOC_EP_EC,
            DEFAULT_CALIB_AUDIO_VOC_EP_FILTERS,
            DEFAULT_CALIB_AUDIO_GAINS_EP,
            DEFAULT_CALIB_AUDIO_EP_PARAMS,
        },
        
        // ITF 2 - Loudspeaker
        {
            DEFAULT_CALIB_AUDIO_VOC_LS_EC,
            DEFAULT_CALIB_AUDIO_VOC_LS_FILTERS,
            DEFAULT_CALIB_AUDIO_GAINS_LS,
            DEFAULT_CALIB_AUDIO_LS_PARAMS,
        },
         
        // ITF 3 - Bluetooth Audio
        {
            DEFAULT_CALIB_AUDIO_VOC_BT_EC,
            DEFAULT_CALIB_AUDIO_VOC_BT_FILTERS,
            DEFAULT_CALIB_AUDIO_GAINS_BT,
            DEFAULT_CALIB_AUDIO_BT_PARAMS,
        },
        
        // ITF 4 - FM Radio
        {
            DEFAULT_CALIB_AUDIO_VOC_FM_EC,
            DEFAULT_CALIB_AUDIO_VOC_FM_FILTERS,
            DEFAULT_CALIB_AUDIO_GAINS_FM,
            DEFAULT_CALIB_AUDIO_FM_PARAMS,
        },
        
        // ITF 5 - Not used yet
        {
            DEFAULT_CALIB_AUDIO_VOC_TV_EC,
            DEFAULT_CALIB_AUDIO_VOC_TV_FILTERS,
            DEFAULT_CALIB_AUDIO_GAINS_TV,
            DEFAULT_CALIB_AUDIO_TV_PARAMS,
        },
    },

    DEFAULT_CALIB_GPADC,
    DEFAULT_CALIB_RF_ANALOG,
    // Not used, the defaults for PMD are taken from PMD.
    {0, 0}
};


// =============================================================================
// g_calibCalibration
// -----------------------------------------------------------------------------
///  Global calibration structure.
///
/// Global pointers used by garnet's code to get the calibrated parameters. By
/// default, it is initialized with default parameters during calibration
/// init. And pointers used by HST to get the version number of the 
/// calibration, and to find the calibration buffers. 
// =============================================================================
PROTECTED CALIB_CALIBRATION_T g_calibCalibration =
{
    CALIB_VERSION_NUMBER,
    CALIB_PARAM_INIT_ERROR,
    CALIB_CMD_NOT_ACCESSIBLE,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    (CALIB_BUFFER_T*) &_calib_start
};



//  -------------------------------------------------- 
//  ----- Calibration process variables. 

///  We add two sets of calibration buffer in RAM to put data from HST,
///  when a calib is being done. 

CALIB_OP_INFO_T  g_calibUsedOpInfo;
CALIB_XCV_T      g_calibUsedXcv;
CALIB_PA_T       g_calibUsedPa;
CALIB_SW_T       g_calibUsedSw;
CALIB_BB_T       g_calibUsedBb;

VOLATILE CALIB_OP_INFO_T CALIB_UNCACHED_BSS g_calibHstOpInfo;
VOLATILE CALIB_XCV_T     CALIB_UNCACHED_BSS g_calibHstXcv;
VOLATILE CALIB_PA_T      CALIB_UNCACHED_BSS g_calibHstPa;
VOLATILE CALIB_SW_T      CALIB_UNCACHED_BSS g_calibHstSw;
VOLATILE CALIB_BB_T      CALIB_UNCACHED_BSS g_calibHstBb;


// =============================================================================
//  FUNCTIONS
// =============================================================================

//=============================================================================
// calib_GetPointers
//-----------------------------------------------------------------------------
/// Access to the calibration parameters.
///
/// @return The global pointers used by the code to access the calibrated
/// parameters. When the target's flash does not contain a flash sector,
/// the calib parameters are initialized with default parameters during
/// the calibration init. Also returns the pointers used by the remote
/// calibration tools to get the version number of the calibration and to 
/// find the calibration buffers.
//=============================================================================
PUBLIC CALIB_CALIBRATION_T* calib_GetPointers(VOID)
{
    return (&g_calibCalibration);
}




//  -------------------------------------------------- 
//  ----- Calibration trace print. 

//  Display information about the current calibration through the trace. 

#define calib_Print(x)         CALIB_TRACE(_PAL | TSTDOUT, 0, "Calibration: "x)
#define calib_Print1(x, y)     CALIB_TRACE(_PAL | TSTDOUT, 0, "Calibration: "x, y)
#define calib_PrintVal(x)      CALIB_TRACE(_PAL | TSTDOUT, 0, \
                                "Calibration: " #x " = %d", x)
#define calib_PrintUVal(x)     CALIB_TRACE(_PAL | TSTDOUT, 0,\
                                "Calibration: " #x " = %u", x)
#define calib_PrintXVal(x)     CALIB_TRACE(_PAL | TSTDOUT, 0,\
                                "Calibration: " #x " = 0x%x", x)




//=============================================================================
// calib_DaemonTracePrint
//-----------------------------------------------------------------------------
/// Display in the trace the current calibration status, used for debug.
//=============================================================================
PUBLIC VOID calib_DaemonTracePrint(VOID)
{
	//  Display the version and the parameters origin. 
    if (g_calibCalibration.paramStatus == CALIB_PARAM_CALIBRATED)
    {
        calib_Print("Using calibrated parameters from flash sector.");
    }
    else if (g_calibCalibration.paramStatus == CALIB_PARAM_DEFAULT)
    {
        calib_Print("Using default calib, platform NOT calibrated.");
    }
    else if (g_calibCalibration.paramStatus == CALIB_PARAM_DEFAULT_RF_MIS)
    {
        calib_Print("Using default calib, "
                 "flash sector NOT compatible with RF chips.");
    }
    else if (g_calibCalibration.paramStatus == CALIB_PARAM_INIT_ERROR)
    {
        calib_Print("ERROR! Problem during the init of the calibration.");
    }
    else
    {
        calib_Print("ERROR! Unknown calibration parameter used!");
    }

    calib_Print1("Calib code version = 0x%x", CALIB_VERSION_NUMBER);
    calib_Print1("Flash version tag = 0x%x", g_calibCalibSector->versionTag);
    calib_Print1("Flash sector address = 0x%x", g_calibCalibSector);
    calib_Print1("Calib method = 0x%x", g_calibCalibration.opInfo->method);
    
	//  Display the values of the calib parameters. 
    calib_Print("Using RF chips:");
    calib_PrintXVal(g_calibCalibration.xcv->palcust.name);
    calib_PrintXVal(g_calibCalibration.pa->palcust.name);
    calib_PrintXVal(g_calibCalibration.sw->palcust.name);

	//  Display the calibration mode used. 
    calib_Print("Code compiled to calibrate platforms.");
}



//  -------------------------------------------------- 
//  ----- Calibration mechanism and structures init. 

//  Get the default calibration parameters values and set the HST calib
//  * buffers with it. This is used to init the calibration buffers and when
//  * the reset calib command is called. 

//  After this, calib_UpdateBuffer() function must be called to copy the
//  * default parameters from HST buffers to used buffers. 

VOID calib_SetBufferToDefault(VOID)
{
    // Default calibration values from the RF drivers. 
    CONST CALIB_XCV_T *xcvDefaults = rfd_XcvCalibGetDefault();
    CONST CALIB_PA_T  *paDefaults =  rfd_PaCalibGetDefault();
    CONST CALIB_SW_T  *swDefaults =  rfd_SwCalibGetDefault();
    CONST CALIB_PMD_T *pmdDefaults = pmd_CalibGetDefault();
    
    // Copy calib param from flash to HST RAM buffer to be able to
    // modify them from HST. 
    memcpy( (UINT8*)&g_calibHstOpInfo,
            (UINT8*)&g_calibOpInfoDefault,
            sizeof(CALIB_OP_INFO_T));
            
    memcpy( (UINT8*)&g_calibHstXcv,
            (UINT8*)xcvDefaults,
            sizeof(CALIB_XCV_T));
            
    memcpy( (UINT8*)&g_calibHstPa,
            (UINT8*)paDefaults,
            sizeof(CALIB_PA_T));
        
    memcpy( (UINT8*)&g_calibHstSw,
            (UINT8*)swDefaults,
            sizeof(CALIB_SW_T));
        
    memcpy( (UINT8*)&g_calibHstBb,
            (UINT8*)&g_calibBbDefault,
            sizeof(CALIB_BB_T));
            
    memcpy( (UINT8*)&g_calibHstBb.pmd,
            (UINT8*)pmdDefaults,
            sizeof(CALIB_PMD_T));
}



/// ----- Updating the HST buffer from the used buffers (USE CALIB). 
/// Copy the calib parameters from the HST RAM buffer to the used RAM
/// buffer. This function is called when the calib parameters are modified
/// by HST. Note: it does not update VoC or the bandgap cal. 
VOID calib_UpdateBuffer(VOID)
{
    // Copy HST calib param to used calib data. 
    memcpy( (UINT8*)&g_calibUsedOpInfo,
            (UINT8*)&g_calibHstOpInfo,
            sizeof(CALIB_OP_INFO_T));
            
    memcpy( (UINT8*)&g_calibUsedXcv,
            (UINT8*)&g_calibHstXcv,
            sizeof(CALIB_XCV_T));
            
    memcpy( (UINT8*)&g_calibUsedPa,
            (UINT8*)&g_calibHstPa,
            sizeof(CALIB_PA_T));
            
    memcpy( (UINT8*)&g_calibUsedSw,
            (UINT8*)&g_calibHstSw,
            sizeof(CALIB_SW_T));
            
    memcpy( (UINT8*)&g_calibUsedBb,
            (UINT8*)&g_calibHstBb,
            sizeof(CALIB_BB_T));

    // Say to HST that the code can accept calib commands. 
    g_calibCalibration.command = CALIB_CMD_DONE;
}



//  -------------------------------------------------- 
//  ----- Calibration mechanism and structures init (USE CALIB). 

// At boot time, uses the the HST parameter Ram buffer 0 and
// copy the default parameters in it. 

PUBLIC VOID calib_DaemonInit(VOID)
{
    UINT32  tag = g_calibCalibSector->versionTag;
    UINT32  mark = CALIB_MARK_MASK(tag);
    UINT8   majVer = CALIB_MAJ_MASK(tag);
    //UINT8   minVer = CALIB_MIN_MASK(tag);
                                        
	//  Default calibration values from the RF drivers. 
    CONST CALIB_XCV_T *xcvDefaults = rfd_XcvCalibGetDefault();
    CONST CALIB_PA_T  *paDefaults =  rfd_PaCalibGetDefault();
    CONST CALIB_SW_T  *swDefaults =  rfd_SwCalibGetDefault();
    
    //  Check if the platform has been calibrated. 
    if ((mark == CALIB_MARK_VERSION) && (majVer == CALIB_MAJ_VERSION))
    {
        // Check the RF chip names. If they are the same as the default
        // ones, don't use the flash structure. 
        if (g_calibCalibSector->xcv.palcust.name == xcvDefaults->palcust.name &&
            g_calibCalibSector->pa.palcust.name ==  paDefaults->palcust.name &&
            g_calibCalibSector->sw.palcust.name ==  swDefaults->palcust.name)
        {
            g_calibCalibration.paramStatus = CALIB_PARAM_CALIBRATED;
        }
        else
        {
			//  Calibration parameters are present but RF chipset mismatch. 
            g_calibCalibration.paramStatus = CALIB_PARAM_DEFAULT_RF_MIS;
        }
    }
    else
    {
        g_calibCalibration.paramStatus = CALIB_PARAM_DEFAULT;
    }

	//  The platform has been calibrated, get parameters from flash. 
    if (CALIB_PARAM_CALIBRATED == g_calibCalibration.paramStatus)
    {
        // Copy calib param from flash to HST RAM buffer to be able to
        // modify them from HST. 
        memcpy( (UINT8*)&g_calibHstOpInfo,
                (UINT8*)&g_calibCalibSector->opInfo,
                sizeof(CALIB_OP_INFO_T));
    
        memcpy( (UINT8*)&g_calibHstXcv,
                (UINT8*)&g_calibCalibSector->xcv,
                sizeof(CALIB_XCV_T));
    
        memcpy( (UINT8*)&g_calibHstPa,
                (UINT8*)&g_calibCalibSector->pa,
                sizeof(CALIB_PA_T));
    
        memcpy( (UINT8*)&g_calibHstSw,
                (UINT8*)&g_calibCalibSector->sw,
                sizeof(CALIB_SW_T));
    
        memcpy( (UINT8*)&g_calibHstBb,
                (UINT8*)&g_calibCalibSector->bb,
                sizeof(CALIB_BB_T));
    }
    //  The platform hasn't been calibrated, we get default values from
    // the different RF chip modules (transceiver, PA and switch). 
    else
    {
        calib_SetBufferToDefault();
    }

    //  Set the global pointers to RAM buffers. 
    g_calibCalibration.opInfo =     &g_calibUsedOpInfo;
    g_calibCalibration.xcv =        &g_calibUsedXcv;
    g_calibCalibration.pa =         &g_calibUsedPa;
    g_calibCalibration.sw =         &g_calibUsedSw;
    g_calibCalibration.bb =         &g_calibUsedBb;

    //  Set the global pointers used by HST to RAM buffers. 
    g_calibCalibration.hstOpInfo =     &g_calibHstOpInfo;
    g_calibCalibration.hstXcv =        &g_calibHstXcv;
    g_calibCalibration.hstPa =         &g_calibHstPa;
    g_calibCalibration.hstSw =         &g_calibHstSw;
    g_calibCalibration.hstBb =         &g_calibHstBb;

    // Copy the calib param from HST buffers to HST buffers to
    // let HST read and modify them. 
    calib_UpdateBuffer();
}




// =============================================================================
// calib_DaemonSetContext
// -----------------------------------------------------------------------------
/// Defines the context structure that HST and JCES must use.
/// @param ctx Pointer to the context to use.
// =============================================================================
PUBLIC VOID calib_DaemonSetContext(VOID* ctx)
{
    g_calibCalibration.stubCtx = ctx;
}




//  -------------------------------------------------- 
//  ----- Calibration command execution. 


/// Function used to program the flash calibration sector.
/// Hyp: MEMD Flash Driver opened before hand.
PRIVATE BOOL burn(UINT8* fshPptr, UINT8* srcPtr, UINT32 size)
{
    MEMD_ERR_T flashError;
    UINT32 len, addrFsh;

	//  Find the address local in the flash. 
    addrFsh = (UINT32)fshPptr - (UINT32)memd_FlashGetGlobalAddress(0);
    addrFsh &= 0x0fffffff;

	// Write calib flash sector. 
    // This function is in RAM, so it can be executed no matter
    // which sector is accessed to burn the calibration sector.
    // Moreover, this code is executed during the Fint interrupt,
    // which has a very high priority, so no other code (That could
    // come from the same bank) is executed.
    flashError = memd_FlashWrite((UINT8*)addrFsh, size, &len, srcPtr);
    
	//  Check flash writting. 
    if ((flashError != MEMD_ERR_NO) || (len != size))
    {
		//  Here we are screwed-up. Just return false. 
        calib_PrintVal(flashError);
        return FALSE;
    }

    return TRUE;
}



//  Function used to update the Platform's used calibration buffers when the
//  user modifies them with HST. It also executes calib cmd. 

VOID calib_DaemonProcess(VOID)
{
    //  Check out if some calibration update has to be done.   
    CALIB_COMMAND_T tag = *((CALIB_COMMAND_T *)HAL_SYS_GET_UNCACHED_ADDR(
                                        (UINT32)(&g_calibCalibration.command)));
    UINT32 i;
    
    //  An error is pending or there is no command to execute. 
    if (CALIB_CMD_NOT_ACCESSIBLE == tag || CALIB_CMD_DONE == tag ||
        CALIB_CMD_FLASH_ERROR == tag)
    {
        return;
    }

    //  Debug print. 
    calib_Print("A calib command has been found. Executing...");

    //  --- The flash must be erased! --- 
    if (CALIB_CMD_FLASH_ERASE == tag || CALIB_CMD_FLASH_BURN == tag)
    {
        //  The dst_ptr is a local flash address, this is UINT32. 
        UINT32     calibSectorStart;
        UINT32     calibSectorEnd;
        MEMD_ERR_T flashError;

        //  Erase calib flash sector.
        calibSectorStart = HAL_SYS_GET_UNCACHED_ADDR(g_calibCalibSector) -
                    HAL_SYS_GET_UNCACHED_ADDR(memd_FlashGetGlobalAddress(0));
                    
        calibSectorEnd = HAL_SYS_GET_UNCACHED_ADDR(&_factory_start) -
                    HAL_SYS_GET_UNCACHED_ADDR(memd_FlashGetGlobalAddress(0));
                    
        // This function is in RAM, so it can be executed no matter
        // which sector is accessed to burn the calibration sector.
        // Moreover, this code is executed during the Fint interrupt,
        // which has a very high priority, so no other code (That could
        // come from the same bank) is executed.
        if ((flashError = memd_FlashErase((UINT8*)calibSectorStart,
                            (UINT8*)calibSectorEnd)) != MEMD_ERR_NO)
        {
            //  In case of error, just say it to HST. 
            calib_Print("ERROR! Calib flash sector erase problem!");
            calib_PrintVal(flashError);
            g_calibCalibration.command = CALIB_CMD_FLASH_ERROR;
            return;
        }

        //  Say that the flash has been erased. 
        calib_Print("The calib flash sector has been erased successfully!");
    }

    //  --- The flash must be burnt! --- 
    if (CALIB_CMD_FLASH_BURN == tag)
    {
        CALIB_VERSION_TAG_T tagConfigured = CALIB_VERSION_NUMBER;
        BOOL ok = TRUE;

        //  The calib sector in flash has already been erased. 
        
        //  Write all the calibration buffers in the calib flash sector. 
        ok &= burn( (UINT8*)&g_calibCalibSector->opInfo,
                    (UINT8*)g_calibCalibration.opInfo,
                    sizeof(CALIB_OP_INFO_T));
                    
        ok &= burn( (UINT8*)&g_calibCalibSector->xcv,
                    (UINT8*)g_calibCalibration.xcv,
                    sizeof(CALIB_XCV_T));

        ok &= burn( (UINT8*)&g_calibCalibSector->pa,
                    (UINT8*)g_calibCalibration.pa,
                    sizeof(CALIB_PA_T));

        ok &= burn( (UINT8*)&g_calibCalibSector->sw,
                    (UINT8*)g_calibCalibration.sw,
                    sizeof(CALIB_SW_T));

        ok &= burn( (UINT8*)&g_calibCalibSector->bb,
                    (UINT8*)g_calibCalibration.bb,
                    sizeof(CALIB_BB_T));
        
        //  Check flash writting. 
        if (!ok)
        {
            //  In case of error, just say it to HST. 
            calib_Print("ERROR! Calib flash sector burn problem!");
            g_calibCalibration.command = CALIB_CMD_FLASH_ERROR;
            return;
        }
        
        //  Burn in flash the tag saying that the Platform has been calibrated. 
        if ( !burn( (UINT8*)(0x0fffffff & (UINT32)&g_calibCalibSector->versionTag),
                    (UINT8*)&tagConfigured,
                    sizeof(CALIB_VERSION_TAG_T)) )
        {
            //  Oops! This is bad, but can NOT occur. 
            calib_Print("ERROR! Calib burnt but valid tag burn problem!");
            g_calibCalibration.command = CALIB_CMD_FLASH_ERROR;
            return;
        }
        
        //  Say that the flash has been burnt. 
        calib_Print("The calib flash sector has been burnt successfully!");
    }

	//  --- Calibration parameters must be updated. --- 
    if (CALIB_CMD_UPDATE == tag)
    {
        //  Copy the calib param from HST buffers to HST buffers to
        //  let HST read and modify them. 
        calib_UpdateBuffer();
         
        // Update the audio interface with the new parameters.
        vois_CalibUpdateValues();
        for (i=0; i<SND_ITF_QTY; i++)
        {
            aud_CalibUpdateValues(i);
        }
        
        //  Update the bandgap of the analog macro, if needed.
        //  (get nb itf from tgt, for i=o to tgt.nb-1)
        hal_AnaGpadcCalibUpdateBandgap();

        // Update RFD calibrations
        rfd_XcvCalibUpdateValues();
        rfd_SwCalibUpdateValues();
        rfd_PaCalibUpdateValues();
        
        calib_Print("Calibration parameters updated.");
    }
    
	//  --- Calibration parameters must be reset to default values. --- 
    if (CALIB_CMD_RESET == tag)
    {
        // Set the HST calib buffers to the default values. 
        calib_SetBufferToDefault();

        // Copy the calib param from HST buffers to HST buffers. 
        calib_UpdateBuffer();

        // Update the audio interface with the new parameters.
 	    vois_CalibUpdateValues();
        for (i=0; i<SND_ITF_QTY; i++)
        {
            aud_CalibUpdateValues(i);
        }


 	    // Update the bandgap of the analog macro, if needed.
 	    hal_AnaGpadcCalibUpdateBandgap();

        //  Call each module to ask him to reset its calib param,
        //  if needed (i.e. for custom parameters like CDAC, HAL_A, etc.). 
        rfd_XcvCalibResetValues();
        rfd_PaCalibResetValues();
        rfd_SwCalibResetValues();

        calib_Print("Calibration parameters reseted.");        
    }
    
	//  --- Calibration is done. --- 
	//  Inform HST that the command has been processed. 
    g_calibCalibration.command = CALIB_CMD_DONE;
}



VOID calib_DaemonUsrDataProcess(VOID)
{
    calib_UpdateBuffer();
    g_calibCalibration.command = CALIB_CMD_FLASH_BURN;
    pal_SetCalibProcess(FALSE);
    calib_DaemonProcess();
    pal_SetCalibProcess(TRUE);    
}


