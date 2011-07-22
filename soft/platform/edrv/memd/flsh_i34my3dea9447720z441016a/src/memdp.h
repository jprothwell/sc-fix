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
//	$Author: huazeng $                                                        // 
//	$Date: 2010-08-23 13:47:34 +0800 (Mon, 23 Aug 2010) $                     //   
//	$Revision: 1532 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file memdp.h                                                             //
/// Private interface in the flash driver
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MEMDP_H_
#define _MEMDP_H_



// =============================================================================
// g_memdFlashLayout
// -----------------------------------------------------------------------------
/// This constant describes the flash layout structure 
/// using the previously defined types
// =============================================================================
EXPORT CONST MEMD_FLASH_LAYOUT_T g_memdFlashLayout;

typedef enum
{
    MEMD_FLASH_OBJECTMODE_CODE_SECTOR,
    MEMD_FLASH_CONTROLMODE_USERDATA_SECTOR,
    MEMD_FLASH_OBJECTMODE_USERDATA_SECTOR
}MEMD_INTEL_FLASH_SECTOR_T;


#endif // FLDP_H




