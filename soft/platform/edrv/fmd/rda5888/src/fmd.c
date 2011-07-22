////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2010, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                     //
//   $HeadURL:                                                                                                                  //
//	$Author:                                                                                                                     // 
//	$Date: 2010--03--18                                                                                                 //   
//	$Revision:                                                                                                                  //   
//                                                                                                                                     //
////////////////////////////////////////////////////////////////////////////////
//       
/// @file fmd.c
/// That file implement the FM driver for RDA5888S chip.
//
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "fmd_config.h"
#include "fmd_m.h"
#include "tgt_fmd_cfg.h"
#include "fmdp_debug.h"

#include "pmd_m.h"

#include "hal_i2c.h"
#include "hal_sys.h"
#include "hal_gpio.h"
#include "hal_timers.h"

#include "sxr_tim.h"

#include "sxr_tls.h"
#include "cmn_defs.h"

#ifdef I2C_BASED_ON_GPIO
#include "gpio_i2c.h"
#else
extern HAL_I2C_BUS_ID_T g_atvdI2cBusId;
#endif

#include "atvd_config.h"
#include "tgt_atvd_cfg.h"

#include "atvd_m.h"
#include "rda5888drv.h"
#include "rda5888fm.h"

// =============================================================================
//  MACROS
// =============================================================================

#define BASE_FREQ_USEUROPE  87000
#define BASE_FREQ_JAPAN     76000
#define BASE_FREQ_WORLD     76000
#define BASE_FREQ_EAST_EUROPE     65000

#define CHAN_SPACE          g_chanSpacing[g_fmdConfig->channelSpacing]

#define FMD_I2C_BPS          g_fmdConfig->i2cBps

#define FM_RSSI_OFFSET 	          57
#define FM_RDA5888S_B     TRUE
// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

PRIVATE CONST TGT_FMD_CONFIG_T*  g_fmdConfig ;

PRIVATE FMD_BAND_T g_band = FMD_BAND_QTY; // using FMD_BAND_QTY as a closed state marker
PRIVATE FMD_CALLBACK_T* g_callback = NULL;
UINT32  fmd_i2c_addr= 0x0062;


PRIVATE uint32 fmd_FieldWrite(uint32 dwRegAddr, uint16 wFieldMask, uint16 wFieldData)
{
	
	RDA5888_FieldWriteReg( dwRegAddr , wFieldMask , wFieldData);
	
	return FMD_ERR_NO;
}


PRIVATE uint16 fmd_FieldRead(uint32 dwRegAddr, uint16 wFieldMask)
{
	uint16 wReadData;
	
	wReadData = RDA5888_FieldReadReg(  dwRegAddr , wFieldMask );
	
	return wReadData;
}

//=============================================================================
// fmd_Scheduler
//-----------------------------------------------------------------------------
/// Process tune and seek operations
//=============================================================================
 VOID fmd_Scheduler(UINT32 data)
{
   
}

UINT16 fmd_5888GetRssi(void)
{
    uint16 rssi;

    rssi = fmd_FieldRead(0x1ff, 0x3f80);

    return rssi;
}



PUBLIC FMD_ERR_T fmd_Open(CONST TGT_FMD_CONFIG_T* fmdTgtCfg,
                          FMD_BAND_T band,
                          FMD_CALLBACK_T* callback,
                          FMD_INIT_T* initValues)
{
    UINT8 dacVol;
    UINT32 chan;
 
    // Check if already opened
    // -----------------------
    if (g_band != FMD_BAND_QTY)
    {
        return FMD_ERR_ALREADY_OPENED;
    }
    // Check the parameters and store them
    // -----------------------------------
    switch (band)
    {
        case FMD_BAND_US_EUROPE:
        case FMD_BAND_JAPAN:
        case FMD_BAND_WORLD:
        case FMD_BAND_EAST_EUROPE:
            g_band = band;
            break;
        default:
            // this chip has limited band support...
            return FMD_ERR_BAD_PARAMETER;
    }
    if (fmdTgtCfg == NULL)
    {
        g_band = FMD_BAND_QTY; // close marker
        return FMD_ERR_BAD_PARAMETER;
    }
    g_fmdConfig = fmdTgtCfg;
    g_callback = callback;
    // default : mute , stereo, no bass boost
    dacVol = 0;
    chan = 0;
    // set the required initial state
    // ------------------------------
    if (initValues != NULL)
    {
        // set given parameters 
        dacVol = g_fmdConfig->volumeVal[initValues->volume];
        chan = (initValues->freqKHz) ;
    }
     SXS_TRACE(TSTDOUT,"fmd_Open  chanfreqKHz:%d ",chan);

    // Sets the PowerDown
   
    if(g_fmdConfig->pinPdn.type==HAL_GPIO_TYPE_IO)
    {
        hal_GpioSetOut(g_fmdConfig->pinPdn.gpioId);
    }
    hal_GpioSet(g_fmdConfig->pinPdn);
   
 
#ifdef I2C_BASED_ON_GPIO
	   gpio_i2c_open();
#else
 //xiaoyifeng for atv
 //for IIC pull up
	 pmd_EnablePower(PMD_POWER_CAMERA,TRUE); 
 
	 g_atvdI2cBusId = tgt_GetAtvdConfig()->i2cBusId; 
	 hal_I2cOpen(g_atvdI2cBusId); 
#endif
#ifdef RDA5888_FM_26MCRYSTAL_ENABLE    
	{
		hal_SysAuxClkOut(TRUE);
	}
#endif

	RDA5888FM_Init();

//open 32k here
#ifdef RDA5888_32KFMMODE_ENALBE
#ifdef RDA5888_FM_26MCRYSTAL_ENABLE    
	{
		hal_SysAuxClkOut(FALSE);
	}
#endif
    {
        RDA_Open32KOnChip();
    }
#endif

     //set volume about dacVol
     if (dacVol == FMD_ANA_MUTE)
    {
         // fmd_Write(0x10,0xc500);  // close adac
    }
    else
    {
	   //fmd_Write(0x10,0x8500);  // open adac	
    }
  
  //  sxr_Sleep(10 MILLI_SECONDS);
   // fmd_FieldWrite(0x125, 0xff80, dacVol);

   // sxr_Sleep(50 MILLI_SECONDS);
    return FMD_ERR_NO;
}

// =============================================================================
// fmd_Tune
// -----------------------------------------------------------------------------
/// This function tunes the FM channel to the desired frequency.
/// Once the tune operation has locked the channel, the callback registered 
/// at #fmd_Open will be called.
/// @param freqKHz  FM frequency
/// @return         \c FMD_ERR_NO, or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_Tune(UINT32 freqKHz)
{
   
    freqKHz/=100;

	RDA5888FM_SetFreq(freqKHz);
   
    return FMD_ERR_NO;
}

PUBLIC BOOL  fmd_ValidStop(UINT32 freqKHz)
{
	UINT8 rt;

    freqKHz/=100;

	rt = RDA5888FM_IsValidFreq(freqKHz);

	return rt;
}

// =============================================================================
// fmd_Close
// -----------------------------------------------------------------------------
/// This function closes the FM driver and desactivate the FM chip 
/// and stops any pending #fmd_Tune or #fmd_Seek operation.
/// @return             \c FMD_ERR_NO, or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_Close(VOID)
{ 
    //set mute,close adac
   // fmd_Write(0x10,0xc500);
    //set pdn low
    RDA5888FM_Exit();
    hal_GpioClr(g_fmdConfig->pinPdn);

#ifdef RDA5888_FM_26MCRYSTAL_ENABLE    
//if ( 0==rdabt_getCurrentState() )
{
	hal_SysAuxClkOut(FALSE);
}
#endif
//close 32k here
#ifdef RDA5888_32KFMMODE_ENALBE
    {
        RDA_Close32KOnClip();
    }
#endif

#ifdef I2C_BASED_ON_GPIO
		   gpio_i2c_close();
#else 
		//xiaoyifeng for atv
		 hal_I2cClose(g_atvdI2cBusId); 
#endif

    g_band = FMD_BAND_QTY; // close marker
 
    return FMD_ERR_NO;
}

// =============================================================================
// fmd_Seek
// -----------------------------------------------------------------------------
/// This function seeks to the next FM channel.
/// Once the seek operation has found a channel, the callback registered 
/// at #fmd_Open will be called.
/// @param seekUp   \c TRUE : increasing frequency, \c FALSE: decreasing frequency
/// @return         \c FMD_ERR_NO, or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_Seek(BOOL seekUp)
{   

    return FMD_ERR_NO;
}


// =============================================================================
// fmd_SetVolume
// -----------------------------------------------------------------------------
/// This function sets the volume and other audio related paramters.
/// @param volume       analog volume
/// @param bassBoost    enable/disable bass boost
/// @param forceMono    enable/disable mono output of stereo radios
/// @return             \c FMD_ERR_NO or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_SetVolume(FMD_ANA_LEVEL_T volume,
                               BOOL bassBoost,
                               BOOL forceMono)
{
    FMD_ERR_T errStatus=0;
    UINT8 dacVol;
 
    FMD_ASSERT(volume < FMD_ANA_VOL_QTY, "FMD volume out of range (%d)", volume);
    // compute the mute bit
    if (volume == FMD_ANA_MUTE)
    {
       // fmd_Write(0x10,0xc500);  // close adac
    }
    else
    {
        //fmd_Write(0x10,0x8500);  // open adac	
    }
  
    sxr_Sleep(50 MILLI_SECONDS);
    //not use bassBoost

    //not use forceMono 
    // get volume value from config
    dacVol = g_fmdConfig->volumeVal[6];


#if ((RDA5888_CHIP_VER == 6) || (RDA5888_CHIP_VER == 7))
    atvd_SetVolume(30);
    //RDA5888_WriteReg(0x12f, 0x0076);  //0x0075
#else
    fmd_FieldWrite(0x125, 0xff80, dacVol);
#endif
    
    return errStatus;

}



// =============================================================================
// fmd_GetFreq
// -----------------------------------------------------------------------------
/// @param pFreqKHz     pointer to a UINT32 receiving the channel frequency
/// @return             \c FMD_ERR_NO, \c FMD_ERR_RESOURCE_BUSY or
///                     \c FMD_ERR_BAD_PARAMETER if \p pFreqKHz is \c NULL
// =============================================================================
PUBLIC FMD_ERR_T fmd_GetFreq(UINT32 *pFreqKHz)
{
  return FMD_ERR_NO;
}


// =============================================================================
// fmd_GetStereoStatus
// -----------------------------------------------------------------------------
/// @param pStereoStatus    pointer to a BOOL receiving the stereo status:
///                         \c TRUE the channel is received in stereo,
///                         \c FALSE the channel is recieved in mono.
/// @return                 \c FMD_ERR_NO, \c FMD_ERR_RESOURCE_BUSY or
///                         \c FMD_ERR_BAD_PARAMETER if \p pStereoStatus is \c NULL
// =============================================================================
PUBLIC FMD_ERR_T fmd_GetStereoStatus(BOOL *pStereoStatus)
{
  return FMD_ERR_NO;
}





// =============================================================================
// fmd_GetRssi
// -----------------------------------------------------------------------------
/// @param pRSSI        pointer to a UINT32 receiving a measure of the channel quality, should not be used as the meaning of this is dependant on the actual FM chip.
/// @return             \c FMD_ERR_NO, \c FMD_ERR_RESOURCE_BUSY or
///                     \c FMD_ERR_BAD_PARAMETER if \p pRSSI is \c NULL
// =============================================================================
#define FM_SEEK_CHECK_TIMES        2

DEPRECATED PUBLIC FMD_ERR_T fmd_GetRssi(UINT32* pRSSI)
{
    *pRSSI =(UINT32)RDA5888FM_GetSigLevel(0);
    return FMD_ERR_NO;
}


// =============================================================================
// fmd_GetRssiThreshold
// -----------------------------------------------------------------------------
/// @return             a Threshold used to compare with RSSI measure of the channel quality, only useful with fmd_GetRssi.
// =============================================================================
DEPRECATED PUBLIC UINT32 fmd_GetRssiThreshold(VOID)
{
    return g_fmdConfig->seekRSSIThreshold;
}


// =============================================================================
// fmd_I2sOpen
// -----------------------------------------------------------------------------
/// This function open the I2S interface
/// @param fmdI2sCfg I2S configuration 
/// @return         \c FMD_ERR_NO, or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_I2sOpen(FMD_I2S_CFG_T fmdI2sCfg)
{  
	RDA5888_I2SOpen(0);

    return FMD_ERR_NO;
}

// =============================================================================
// fmd_I2sClose
// -----------------------------------------------------------------------------
/// This function close the I2S interface
/// @return         \c FMD_ERR_NO, or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_I2sClose()
{
	RDA5888_I2SClose();

    return FMD_ERR_NO;
}

// =============================================================================
// fmd_PreInit
// -----------------------------------------------------------------------------
/// This function initial fm in handset power on.
// =============================================================================
PROTECTED  VOID  fmd_PreInit(VOID)
{
   

}

