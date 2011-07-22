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
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file tgt_m.h                                                             //
/// That file provides the TGT API related to KEY and form factor factory
/// setting configuration.                                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _TGT_H_
#define _TGT_H_

#include "cs_types.h"

// =============================================================================
// TGT_IMEI_T
// -----------------------------------------------------------------------------
/// Type used to represent a IMEI number.
// =============================================================================
typedef UINT8 TGT_IMEI_T[9];

// =============================================================================
// TGT_MOBILE_ID_T
// -----------------------------------------------------------------------------
/// Define the mobile identification structure 
/// This structure must be included at the beginning of the factory settings sector
/// The rest of the factory settings sector can be customized to meet the customer needs
// =============================================================================
typedef struct {
    // Hold a valid status bit for the IMEI, check with a CRC, and markers
    // for the factory setting configuration step status.
    UINT32 validMarker;
    // IMEISV for the sims of the phone. (Hardcoded at most 4 sims.)
    TGT_IMEI_T imeiSv[4];
    // Mother board serial numbers.
    UINT8 mbSn[50];
    // Mobile phone serial numbers.
    UINT8 mpSn[50];
} TGT_MOBILE_ID_T;

// =============================================================================
// TGT_MODULE_ID_T
// -----------------------------------------------------------------------------
/// Define the module identification structure 
/// Module IDs are used to speed up LCD and Camera init.
// =============================================================================
typedef struct {
    UINT8  lcd_id;
    UINT8  cam_id[2];
} TGT_MODULE_ID_T;


typedef struct{
    UINT8   BB[40];
    UINT8   RFPA[40];
    UINT8   FLSH[60];
    UINT8   LCD[60];
    UINT8   CAM[60];
    UINT8   BT[40];
    UINT8   ATV[40];
    UINT8   FM[60];    
}TGT_MODULE_T;

// =============================================================================
// TGT_FACTORY_SETTINGS_T
// -----------------------------------------------------------------------------
/// Define the factory settings structure for this target
// =============================================================================
typedef struct
{
    // mobile id
    TGT_MOBILE_ID_T mobileId;
    // module id
    TGT_MODULE_ID_T moduleId;
} TGT_FACTORY_SETTINGS_T;


// Define the banding info of the SIM Card. 	 	 
typedef struct { 	 	 
    UINT8 sim_Sign;     // The sign that indicates if band SIM card. 	 	 
    UINT8 simNo[10][9]; // It means that can band up-to 10 SIM cards and each SIM card number ties up 9 bytes spaces. 	 	 
    UINT8 nw_Sign;      // The sign that indicates if band network. 	 	 
    UINT8 nwNo[5][3];   // It means that can band up-to 5 networks and each network has 3 identifiers. 	 	 
} bind_info_t; 	 	 

// =============================================================================
// tgt_GetBuildVerNo
// -----------------------------------------------------------------------------
/// This function is used to access the version number of this build.
// =============================================================================
CONST CHAR* tgt_GetBuildVerNo(VOID);

// =============================================================================
// tgt_GetBuildDateTime
// -----------------------------------------------------------------------------
/// This function is used to access the date time of this build.
// =============================================================================
CONST CHAR* tgt_GetBuildDateTime(VOID);

// =============================================================================
// tgt_GetMobileId
// -----------------------------------------------------------------------------
/// This function is used to access the Mobile ID description.
// =============================================================================
CONST TGT_MOBILE_ID_T* tgt_GetMobileId(VOID);

// =============================================================================
// tgt_GetFactorySettings
// -----------------------------------------------------------------------------
/// This function is used to access the factory settings.
// =============================================================================
PUBLIC TGT_FACTORY_SETTINGS_T* tgt_GetFactorySettings(VOID);

// =============================================================================
// tgt_SetFactorySettings
// -----------------------------------------------------------------------------
/// This function is used to update the new factory settings to flash.
// =============================================================================
PUBLIC BOOL tgt_SetFactorySettings(TGT_FACTORY_SETTINGS_T* factorySetting);

// =============================================================================
// tgt_GetKeyCode
// -----------------------------------------------------------------------------
/// This function returns the code of a key from its column number. It 
/// implements thus the key mapping, between the key detected and the 
/// keyboard of a given target.
/// @param key Key number as given by the HAL Key driver
/// @return The key code as mapped for a specifc target.
// =============================================================================
PUBLIC UINT8 tgt_GetKeyCode(UINT8 key);



// =============================================================================
// tgt_RegisterAllModules
// -----------------------------------------------------------------------------
/// This function call the register functions for each modules of platform.
// =============================================================================
PUBLIC VOID tgt_RegisterAllModules(VOID);



#endif // _TGT_H_
