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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/regs/esperite/include/dmasc.h $                                                                   
//  $Author: admin $                                                                    
//  $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//  $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _DMASC_H_
#define _DMASC_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'dmasc'."
#endif


#include "globals.h"

// =============================================================================
//  MACROS
// =============================================================================
#define NB_BITS_ADDR                             (25)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// DMASC_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_DMASC_BASE              0x01910000

typedef volatile struct
{
    REG32                          Ctrl;                         //0x00000000
    REG32                          Status;                       //0x00000004
    REG32                          Length;                       //0x00000008
    REG32 Reserved_0000000C;                    //0x0000000C
    REG32                          WAddr;                        //0x00000010
    REG32                          WOffset;                      //0x00000014
    REG32                          WPunctAddr;                   //0x00000018
    REG32                          WPunctOffset;                 //0x0000001C
    REG32                          R1Addr;                       //0x00000020
    REG32                          R1Offset;                     //0x00000024
    REG32                          R1PunctAddr;                  //0x00000028
    REG32                          R1PunctOffset;                //0x0000002C
    REG32                          R2Addr;                       //0x00000030
    REG32                          R2Offset;                     //0x00000034
    REG32                          R2PunctAddr;                  //0x00000038
    REG32                          R2PunctOffset;                //0x0000003C
} HWP_DMASC_T;

#define hwp_dmasc                   ((HWP_DMASC_T*) KSEG1(REG_DMASC_BASE))


//Ctrl
#define DMASC_ENABLE                (1<<0)
#define DMASC_WPUNCT_ENABLE         (1<<1)
#define DMASC_WPUNCT_DISABLE        (0<<1)
#define DMASC_COMPRESS_ENABLE       (1<<2)
#define DMASC_COMPRESS_DISABLE      (0<<2)
#define DMASC_R1PUNCT_ENABLE        (1<<5)
#define DMASC_R1PUNCT_DISABLE       (0<<5)
#define DMASC_DECOMPRESS_ENABLE     (1<<6)
#define DMASC_DECOMPRESS_DISABLE    (0<<6)
#define DMASC_R2ENABLE_ENABLE       (1<<8)
#define DMASC_R2ENABLE_DISABLE      (0<<8)
#define DMASC_R2PUNCT_ENABLE        (1<<9)
#define DMASC_R2PUNCT_DISABLE       (0<<9)
#define DMASC_MODE_DIRECT_COPY      (0<<12)
#define DMASC_MODE_ADD              (1<<12)
#define DMASC_MODE_MAX              (2<<12)
#define DMASC_MODE_COMPRESSED_MAX   (3<<12)
#define DMASC_DONE_IRQ_MASK_ENABLE  (1<<16)
#define DMASC_DONE_IRQ_MASK_DISABLE (0<<16)
#define DMASC_WPARTIAL_ENABLE       (1<<24)
#define DMASC_WPARTIAL_DISABLE      (0<<24)
#define DMASC_R1PARTIAL_ENABLE      (1<<25)
#define DMASC_R1PARTIAL_DISABLE     (0<<25)
#define DMASC_R2PARTIAL_ENABLE      (1<<26)
#define DMASC_R2PARTIAL_DISABLE     (0<<26)

//Status
#define DMASC_ACTIVE                (1<<0)
#define DMASC_DONE_IRQ_CAUSE        (1<<16)

//Length
#define DMASC_LENGTH(n)             (((n)&0xFFF)<<0)

//WAddr
#define DMASC_WADDR(n)              (((n)&0xFFFFFF)<<2)

//WOffset
#define DMASC_WOFFSET(n)            (((n)&7)<<0)
#define DMASC_WOFFSET_FIFO(n)       (((n)&3)<<3)
#define DMASC_WOFFSET_FULL(n)       (((n)&31)<<0)
#define DMASC_WOFFSET_FULL_MASK     (31<<0)
#define DMASC_WOFFSET_FULL_SHIFT    (0)

//WPunctAddr
#define DMASC_WPUNCTADDR(n)         (((n)&0xFFFFFF)<<2)

//WPunctOffset
#define DMASC_WPUNCTOFFSET(n)       (((n)&31)<<0)

//R1Addr
#define DMASC_R1ADDR(n)             (((n)&0xFFFFFF)<<2)
#define DMASC_R1BYTEOFFSET(n)       (((n)&3)<<0)

//R1Offset
#define DMASC_R1OFFSET(n)           (((n)&7)<<0)
#define DMASC_R1OFFSET_FIFO(n)      (((n)&3)<<3)
#define DMASC_R1OFFSET_FULL(n)      (((n)&31)<<0)
#define DMASC_R1OFFSET_FULL_MASK    (31<<0)
#define DMASC_R1OFFSET_FULL_SHIFT   (0)

//R1PunctAddr
#define DMASC_R1PUNCTADDR(n)        (((n)&0xFFFFFF)<<2)

//R1PunctOffset
#define DMASC_R1PUNCTOFFSET(n)      (((n)&31)<<0)

//R2Addr
#define DMASC_R2ADDR(n)             (((n)&0xFFFFFF)<<2)
#define DMASC_R2BYTEOFFSET(n)       (((n)&3)<<0)

//R2Offset
#define DMASC_R2OFFSET(n)           (((n)&7)<<0)

//R2PunctAddr
#define DMASC_R2PUNCTADDR(n)        (((n)&0xFFFFFF)<<2)

//R2PunctOffset
#define DMASC_R2PUNCTOFFSET(n)      (((n)&31)<<0)





#endif

