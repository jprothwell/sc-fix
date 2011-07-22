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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/regs/greenstone/include/debug_host_internals.h $                                                                   
//    $Author: admin $                                                                    
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//    $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _DEBUG_HOST_INTERNALS_H_
#define _DEBUG_HOST_INTERNALS_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'debug_host_internals'."
#endif



// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================
// ============================================================================
// DEBUG_HOST_INTERNAL_REGISTERS_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_INT_REG_DBG_HOST_BASE   0x00000000


typedef volatile struct
{
    /// General control signals set.
    REG8                        CTRL_SET;                     //0x00000000
    /// General control signals clear.
    REG8                        CTRL_CLR;                     //0x00000001
    /// Configure Debug UART Clock divider.
    REG8                        CLKDIV;                       //0x00000002
    /// Configure Debug UART.
    REG8                        CFG;                          //0x00000003
    /// Status of CRC.
    REG8                        CRC;                          //0x00000004
    /// Host write, APB readable register.
    REG8                        H2P_STATUS;                   //0x00000005
    /// APB write, Host readable register.
    REG8                        P2H_STATUS;                   //0x00000006
    /// Debug information of system side AHB bus status.
    REG8                        AHB_SYS_MASTER;               //0x00000007
    /// Debug information of AHB bus status HSEL.
    REG8                        AHB_SYS_SLAVE;                //0x00000008
    /// Debug information of baseband side AHB bus status.
    REG8                        AHB_BB_MASTER;                //0x00000009
    /// Debug information of AHB bus status HSEL.
    REG8                        AHB_BB_HSREADY;               //0x0000000A
    /// Debug information of AHB bus status HSEL.
    REG8                        AHB_BB_HSEL;                  //0x0000000B
} PACKED HWP_DEBUG_HOST_INTERNAL_REGISTERS_T;

#define hwp_intRegDbgHost           ((HWP_DEBUG_HOST_INTERNAL_REGISTERS_T*) KSEG1(REG_INT_REG_DBG_HOST_BASE))


//CTRL_SET
#define DEBUG_HOST_INTERNAL_REGISTERS_DEBUG_RESET (1<<0)
#define DEBUG_HOST_INTERNAL_REGISTERS_XCPU_FORCE_RESET (1<<1)
#define DEBUG_HOST_INTERNAL_REGISTERS_FORCE_WAKEUP (1<<2)
#define DEBUG_HOST_INTERNAL_REGISTERS_FORCE_BP_XCPU (1<<3)
#define DEBUG_HOST_INTERNAL_REGISTERS_FORCE_BP_BCPU (1<<4)
#define DEBUG_HOST_INTERNAL_REGISTERS_IT_XCPU (1<<5)
#define DEBUG_HOST_INTERNAL_REGISTERS_IT_BCPU (1<<6)
#define DEBUG_HOST_INTERNAL_REGISTERS_DEBUG_PORT_LOCK (1<<7)

//CTRL_CLR
//#define DEBUG_HOST_INTERNAL_REGISTERS_XCPU_FORCE_RESET (1<<1)
//#define DEBUG_HOST_INTERNAL_REGISTERS_FORCE_WAKEUP (1<<2)
//#define DEBUG_HOST_INTERNAL_REGISTERS_FORCE_BP_XCPU (1<<3)
//#define DEBUG_HOST_INTERNAL_REGISTERS_FORCE_BP_BCPU (1<<4)
//#define DEBUG_HOST_INTERNAL_REGISTERS_DEBUG_PORT_LOCK (1<<7)

//CLKDIV
#define DEBUG_HOST_INTERNAL_REGISTERS_CFG_CLK(n) (((n)&0x3F)<<0)

//CFG
#define DEBUG_HOST_INTERNAL_REGISTERS_DISABLE_UART_H (1<<0)
#define DEBUG_HOST_INTERNAL_REGISTERS_DISABLE_IFC_H (1<<1)
#define DEBUG_HOST_INTERNAL_REGISTERS_DEBUG_PORT_SEL (1<<2)
#define DEBUG_HOST_INTERNAL_REGISTERS_FORCE_PRIO_H (1<<7)

//CRC
#define DEBUG_HOST_INTERNAL_REGISTERS_CRC (1<<0)
#define DEBUG_HOST_INTERNAL_REGISTERS_FC_FIFO_OVF (1<<1)

//H2P_STATUS
#define DEBUG_HOST_INTERNAL_REGISTERS_STATUS(n) (((n)&0xFF)<<0)

//P2H_STATUS
//#define DEBUG_HOST_INTERNAL_REGISTERS_STATUS(n) (((n)&0xFF)<<0)

//AHB_SYS_MASTER
#define DEBUG_HOST_INTERNAL_REGISTERS_SYS_IFC_HMBURSREQ (1<<0)
#define DEBUG_HOST_INTERNAL_REGISTERS_SYS_DMA_HMBURSREQ (1<<1)
#define DEBUG_HOST_INTERNAL_REGISTERS_SYS_AHB2AHB_HMBURSREQ (1<<2)
#define DEBUG_HOST_INTERNAL_REGISTERS_XCPU_HMBRSREQ (1<<3)
#define DEBUG_HOST_INTERNAL_REGISTERS_SYS_IFC_HMGRANT (1<<4)
#define DEBUG_HOST_INTERNAL_REGISTERS_SYS_DMA_HMGRANT (1<<5)
#define DEBUG_HOST_INTERNAL_REGISTERS_SYS_AHB2AHB_HMGRANT (1<<6)
#define DEBUG_HOST_INTERNAL_REGISTERS_XCPU_HMGRANT (1<<7)

//AHB_SYS_SLAVE
#define DEBUG_HOST_INTERNAL_REGISTERS_SYS_MEM_EXT_HSEL (1<<0)
#define DEBUG_HOST_INTERNAL_REGISTERS_SYS_MEM_INT_HSEL (1<<1)
#define DEBUG_HOST_INTERNAL_REGISTERS_SYS_IFC_HSEL (1<<2)
#define DEBUG_HOST_INTERNAL_REGISTERS_SYS_AHB2AHB_HSEL (1<<3)
#define DEBUG_HOST_INTERNAL_REGISTERS_SYS_IFC_HSREADY (1<<4)
#define DEBUG_HOST_INTERNAL_REGISTERS_SYS_MEM_HSREADY (1<<5)
#define DEBUG_HOST_INTERNAL_REGISTERS_SYS_AHB2AHB_HSREADY (1<<6)
#define DEBUG_HOST_INTERNAL_REGISTERS_SYS_HREADY (1<<7)

//AHB_BB_MASTER
#define DEBUG_HOST_INTERNAL_REGISTERS_BB_IFC_HMBURSREQ (1<<0)
#define DEBUG_HOST_INTERNAL_REGISTERS_BB_VOC_HMBURSREQ (1<<1)
#define DEBUG_HOST_INTERNAL_REGISTERS_BB_AHB2AHB_HMBURSREQ (1<<2)
#define DEBUG_HOST_INTERNAL_REGISTERS_BCPU_HMBRSREQ (1<<3)
#define DEBUG_HOST_INTERNAL_REGISTERS_BB_IFC_HMGRANT (1<<4)
#define DEBUG_HOST_INTERNAL_REGISTERS_BB_VOC_HMGRANT (1<<5)
#define DEBUG_HOST_INTERNAL_REGISTERS_BB_AHB2AHB_HMGRANT (1<<6)
#define DEBUG_HOST_INTERNAL_REGISTERS_BCPU_HMGRANT (1<<7)

//AHB_BB_HSREADY
#define DEBUG_HOST_INTERNAL_REGISTERS_BB_MEM_HSREADY (1<<0)
#define DEBUG_HOST_INTERNAL_REGISTERS_BB_VOC_HSREADY (1<<1)
#define DEBUG_HOST_INTERNAL_REGISTERS_BB_SRAM_HSREADY (1<<2)
#define DEBUG_HOST_INTERNAL_REGISTERS_BB_IFC_HSREADY (1<<3)
#define DEBUG_HOST_INTERNAL_REGISTERS_BB_AHB2AHB_HSREADY (1<<4)
#define DEBUG_HOST_INTERNAL_REGISTERS_BB_HREADY (1<<7)

//AHB_BB_HSEL
#define DEBUG_HOST_INTERNAL_REGISTERS_BB_MEM_EXT_HSEL (1<<0)
#define DEBUG_HOST_INTERNAL_REGISTERS_BB_MEM_INT_HSEL (1<<1)
#define DEBUG_HOST_INTERNAL_REGISTERS_BB_VOC_HSEL (1<<2)
#define DEBUG_HOST_INTERNAL_REGISTERS_BB_SRAM_HSEL (1<<3)
#define DEBUG_HOST_INTERNAL_REGISTERS_BB_IFC_HSEL (1<<4)
#define DEBUG_HOST_INTERNAL_REGISTERS_BB_AHB2AHB_HSEL (1<<5)





#endif
