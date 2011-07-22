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
//
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/boot/esperite/include/boot_sector_chip.h $
//  $Author: admin $
//  $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $
//  $Revision: 269 $
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _BOOT_SECTOR_CHIP_H_
#define _BOOT_SECTOR_CHIP_H_


//=============================================================================
// BOOT_SECTOR_ENTRY_POINT
//-----------------------------------------------------------------------------
/// First address where to jump in flash, to execute code
//=============================================================================
#define BOOT_SECTOR_ENTRY_POINT 0x82000010


//=============================================================================
// BOOT_FLASH_PROGRAMMED_PATTERN_ADDRESS
//-----------------------------------------------------------------------------
/// Address of the programmed flash pattern.
//=============================================================================
#define BOOT_FLASH_PROGRAMMED_PATTERN_ADDRESS ((UINT32*)0x82000000)


#endif // _BOOT_SECTOR_CHIP_H_


