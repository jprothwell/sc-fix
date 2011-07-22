////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2010-2010, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/include/hal_rda_audio.h $ //
//    $Author: huazeng $                                                        //
//    $Date: 2011-06-03 20:22:01 +0800 (Fri, 03 Jun 2011) $                     //
//    $Revision: 8054 $                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_rda_audio.h                                                           //
///                                                                           //
/// This file decribes the RDA Audio driver API.         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _HAL_RDA_AUDIO_H_
#define _HAL_RDA_AUDIO_H_

#include "cs_types.h"
#include "hal_gpio.h"

// =============================================================================
//  MACROS
// =============================================================================

#define AUD_TIME_POWER_ON_DEEP   (1 MS_WAITING)
#define AUD_TIME_POWER_ON_REF  (11)    // 500 us, upper_int(HAL_TICK1S/1000/2) + 2
#define AUD_TIME_POWER_ON_OTHER  (6)    // 200 us, upper_int(HAL_TICK1S/1000/5) + 2
#define AUD_TIME_SET_VCOM_MODE  (1 MS_WAITING)

#define AUD_TIME_RETRY (2 MS_WAITING)

// =============================================================================
//  TYPES
// =============================================================================

typedef VOID (*HAL_AUD_CALLBACK_T)(VOID);

typedef enum
{
    // Identifiers for internal only
    HAL_AUD_USER_NULL,
    HAL_AUD_USER_CLOSING,
    HAL_AUD_USER_OPENING,
    // Identifiers for API parameter
    HAL_AUD_USER_CODEC,
    HAL_AUD_USER_FM,
    HAL_AUD_USER_LINEIN,
#ifdef VMIC_POWER_ON_WORKAROUND
    HAL_AUD_USER_EARPIECE,
#endif // VMIC_POWER_ON_WORKAROUND
} HAL_AUD_USER_T;

typedef union
{
    UINT32 reg;
    struct
    {
        BOOL noDeepPd : 1;
        BOOL noRefPd : 1;
        BOOL noMicPd : 1;
        BOOL noAuxMicPd : 1;
        BOOL noAdcPd : 1;
        BOOL noDacPd : 1;
        UINT32 : 2;
        BOOL noDacReset : 1;
    };
} HAL_AUD_PD_T;
#define HAL_AUD_PD_MASK (0x13f)

typedef union
{
    UINT32 reg;
    struct
     {
        BOOL auxMicSel : 1;
        BOOL spkSel : 1;
        BOOL spkMonoSel : 1;
        BOOL rcvSel : 1;
        BOOL headSel : 1;
        BOOL fmSel : 1;
    };
} HAL_AUD_SEL_T;

typedef union
{
    UINT32 reg;
    struct
    {
        UINT32 micGain : 4;
        BOOL noMicMute : 1;
    };
} HAL_AUD_MIC_CFG_T;

typedef union
{
    UINT32 reg;
    struct
    {
        UINT32 spkGain : 4;
        BOOL noSpkMute : 1;
    };
} HAL_AUD_SPK_CFG_T;


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

//  

// =============================================================================
//  FUNCTIONS
// =============================================================================

PUBLIC VOID hal_AudConfigExtSpkPa(HAL_APO_ID_T paCtrl, BOOL pinActiveLow);

PUBLIC BOOL hal_AudOpen(HAL_AUD_USER_T user, HAL_AUD_CALLBACK_T cbFunc);
PUBLIC BOOL hal_AudClose(HAL_AUD_USER_T user);
PUBLIC VOID hal_AudPowerOn(HAL_AUD_PD_T pdStatus);
PUBLIC VOID hal_AudPowerDown(HAL_AUD_PD_T pdStatus);
PUBLIC VOID hal_AudDeepPowerDown(VOID);
/// Must wait for AUD_TIME_SET_VCOM_MODE if setting to non-FM mode
PUBLIC VOID hal_AudSetVcomMode(BOOL isFmMode);
PUBLIC VOID hal_AudSetVoiceMode(BOOL isVoiceMode);
PUBLIC VOID hal_AudSetSelCfg(HAL_AUD_SEL_T selCfg);
PUBLIC VOID hal_AudSetMicCfg(HAL_AUD_MIC_CFG_T micCfg);
PUBLIC VOID hal_AudSetSpkCfg(HAL_AUD_SPK_CFG_T spkCfg);
PUBLIC VOID hal_AudSetRcvGain(UINT32 gain);
PUBLIC VOID hal_AudSetHeadGain(UINT32 gain);
PUBLIC HAL_AUD_PD_T hal_AudGetPowerStatus(VOID);
PUBLIC HAL_AUD_SEL_T hal_AudGetSelCfg(VOID);
PUBLIC HAL_AUD_MIC_CFG_T hal_AudGetMicCfg(VOID);
PUBLIC HAL_AUD_SPK_CFG_T hal_AudGetSpkCfg(VOID);
PUBLIC UINT32 hal_AudGetRcvGain(VOID);
PUBLIC UINT32 hal_AudGetHeadGain(VOID);


#endif // _HAL_RDA_AUDIO_H_

