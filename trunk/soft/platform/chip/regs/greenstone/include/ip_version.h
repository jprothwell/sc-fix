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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/regs/greenstone/include/ip_version.h $                                                                   
//    $Author: admin $                                                                    
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//    $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _IP_VERSION_H_
#define _IP_VERSION_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'ip_version'."
#endif



// =============================================================================
//  MACROS
// =============================================================================
/// This is a released IP!
#define RELEASED_IP                              (1)
#define IP_VERSION_NUMBER                        (0X12080314)

// =============================================================================
//  TYPES
// =============================================================================
// ============================================================================
// IP_VERSION_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================


typedef struct
{
    /// This register contains the IP version and release date for released IP 
    /// If the version Major is 0 the the register contains the svn revision used
    /// and the IP is not a released IP!
    REG32                          IP_Version;                   //0x00000000
} IP_VERSION_T; //Size : 0x4

//IP_Version
#define IP_VERSION_MAJOR(n)         (((n)&15)<<28)
#define IP_VERSION_MINOR(n)         (((n)&15)<<24)
#define IP_VERSION_YEAR(n)          (((n)&15)<<16)
#define IP_VERSION_MONTH(n)         (((n)&0xFF)<<8)
#define IP_VERSION_DAY(n)           (((n)&0xFF)<<0)





#endif
