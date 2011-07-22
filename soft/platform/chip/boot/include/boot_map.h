//==============================================================================
//                                                                              
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.              
//                            All Rights Reserved                               
//                                                                              
//      This source code is the property of Coolsand Technologies and is        
//      confidential.  Any  modification, distribution,  reproduction or        
//      exploitation  of  any content of this file is totally forbidden,        
//      except  with the  written permission  of  Coolsand Technologies.        
//                                                                              
//==============================================================================
//                                                                              
//    THIS FILE WAS GENERATED FROM ITS CORRESPONDING XML VERSION WITH COOLXML.  
//                                                                              
//                       !!! PLEASE DO NOT EDIT !!!                             
//                                                                              
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/boot/include/boot_map.h $                                                                   
//  $Author: huazeng $                                                                    
//  $Date: 2010-07-21 20:09:53 +0800 (Wed, 21 Jul 2010) $                                                                      
//  $Revision: 642 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _BOOT_MAP_H_
#define _BOOT_MAP_H_



// =============================================================================
//  MACROS
// =============================================================================

// ============================================================================
// BOOT_MAIN_TAG_T
// -----------------------------------------------------------------------------
/// Magic tags used by the boot loader to switch between boot modes.
// =============================================================================
typedef enum
{
    BOOT_MAIN_TAG_NORMAL                        = 0x00000000,
    BOOT_MAIN_TAG_BREAKBOOT                     = 0xB4EAB007,
    BOOT_MAIN_TAG_CALIB                         = 0xCA1BCA1B
} BOOT_MAIN_TAG_T;


// ============================================================================
// BOOT_SECTOR_CMD_T
// -----------------------------------------------------------------------------
/// List of Boot Sector commands.
// =============================================================================
typedef enum
{
/// The Boot Sector must jump into the ROMed Boot Monitor.
    BOOT_SECTOR_CMD_ENTER_BOOT_MONITOR          = 0x4E6A821C
} BOOT_SECTOR_CMD_T;

/// Value written in the Boot Sector structure to say that the EBC configuration
/// present in this structure is valid and can be programmed. Used for the field
/// ebcConfigValidTag.
#define BOOT_SECTOR_EBC_VALID_TAG                (0XB0075EC7)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// BOOT_SECTOR_STRUCT_T
// -----------------------------------------------------------------------------
/// Structure type used by the Boot Sector to determine in which mode it must boot
/// and how the EBC RAM must be configured (this is useful in case of burst mode
/// RAM). This structure contains information written by HAL during the normal execution
/// of the code and used by the Boot Sector when the phone boots. NOTE: The size
/// of this structure is hard-coded for the BOOT_SECTOR_STRUCT_BASE variable in the
/// chip definition file, ???_base.def, which will be used in the ld file. At present
/// the size is 4*4. If the structure is changed, the def file must be modified correspondingly.
// =============================================================================
typedef struct
{
    BOOT_SECTOR_CMD_T              command;                      //0x00000000
    UINT32                         ebcConfigValidTag;            //0x00000004
    UINT32                         ebcConfigRamTimings;          //0x00000008
    UINT32                         ebcConfigRamMode;             //0x0000000C
} BOOT_SECTOR_STRUCT_T; //Size : 0x10



// ============================================================================
// BOOT_MAP_GLOBALS_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef struct
{
} BOOT_MAP_GLOBALS_T; //Size : 0x0





#endif

