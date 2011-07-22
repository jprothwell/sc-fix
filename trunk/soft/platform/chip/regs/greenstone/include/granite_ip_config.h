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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/regs/greenstone/include/granite_ip_config.h $                                                                   
//    $Author: admin $                                                                    
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//    $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _GRANITE_IP_CONFIG_H_
#define _GRANITE_IP_CONFIG_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'granite_ip_config'."
#endif



// =============================================================================
//  MACROS
// =============================================================================
/// granite_ip
#define IP_NAME                                  (0)
/// artisan_sage_x_tsmc0_18
#define TECHNO                                   (0)
/// 0= chip option; 1= FPGA option
#define FPGA_OPTION                              (0)
/// 0= no baseband; 1= baseband included
#define BB_OPTION                                (1)
/// 0= no mux as on test chip; 1=mux for MultiMedia debug port part
#define MUX_MM_DEBUG_PORT                        (0)
/// 0= No analog; 1= Acco option or FPGA wrappers if FPGA_OPTION=1
#define ANALOG_OPTION                            (1)
/// acco or frequency of the provided external clock: 
/// 0= acco 156M, run up to 78 ; 1= 156M, run up to 78 ; 2=312M, run up to 104M
#define PLL_RATE_OPTION                          (3)
#define FAST_CLOCK_156M                          (156000000)
#define FAST_CLOCK_312M                          (312000000)
/// Fast clock from the Pll.
#define FAST_CLOCK_PLL                           (FAST_CLOCK_156M)
/// 0= No VOC ; 1= VOC included
#define VOC_OPTION                               (1)
/// 0= No aif channels (0,1) ; 1= All 3 channels
#define AIF_OPTION                               (1)
#define AUBYPASS_OPTION                          (0)
/// 0= No MMI ; 1= MMI included : keypad, PWL/PWT, calendar
#define MMI_OPTION                               (1)
/// GPIO/GPO OPTIONS: numbers
#define NB_GPIO                                  (32)
#define NB_GPIO_INT                              (8)
#define NB_GPO                                   (11)
/// 0= No DMA ; 1= DMA included
#define DMA_OPTION                               (1)
/// 0 = no USB; 1 = USB included
#define USB_OPTION                               (1)
/// System Ifc Number of generic channel (range 2 to 7)
#define SYS_IFC_NB_STD_CHANNEL                   (7)
/// 0 = no LCDC; 1 = LCDC included
#define LCD_OPTION                               (1)
/// 0 = no UART2; 1 = UART2 included
#define UART2_OPTION                             (1)
/// number of SPI CS
#define SPI_NB_CS                                (3)
/// 0 = no SPI2; 1 = SPI2 included
#define SPI2_OPTION                              (1)
/// number of SPI2 CS
#define SPI2_NB_CS                               (2)
/// 0 = no SDMMC; 1 = SDMMC controller included
#define SDMMC_OPTION                             (1)
/// 0 = no Camera; 1 = Camera controller included
#define CAMERA_OPTION                            (1)
/// 0 = EBC, 1 = AHBM
#define MEMBRIDGE_OPTION                         (0)
/// for membridge internal ram: number of 32k blocks
#define MEMBRIDGE_RAM_NB_BLOCK                   (2)
/// for EBC option only
#define EBC_NB_BITS_ADDR                         (24)
/// for AHBM option only: address bus size
#define AHBM_NB_BITS_ADDR                        (25)
#define AHBM_INIT_SB_0                           (0)
#define AHBM_INIT_SB_1                           (0)
#define AHBM_INIT_SB_2                           (0)
#define AHBM_INIT_SB_3                           (0)
#define AHBM_INIT_SB_4                           (0)

// =============================================================================
//  TYPES
// =============================================================================


#endif
