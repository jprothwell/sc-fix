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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/regs/greenstone/include/granite_ip.h $                                                                   
//    $Author: admin $                                                                    
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//    $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _GRANITE_IP_H_
#define _GRANITE_IP_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'granite_ip'."
#endif


#include "globals.h"
#include "global_macros.h"
#include "bb_ahb_monitor.h"
#include "bb_cp2.h"
#include "bb_ifc.h"
#include "bb_irq.h"
#include "bb_sram.h"
#include "comregs.h"
#include "bcpu_cache.h"
#include "bcpu_tag.h"
#include "bist.h"
#include "cipher.h"
#include "debug_host.h"
#include "debug_host_internals.h"
#include "debug_uart.h"
#include "dma.h"
#include "mem_bridge.h"
#include "debug_port.h"
#include "gpio.h"
#include "i2c_master.h"
#include "itlv.h"
#include "page_spy.h"
#include "rf_if.h"
#include "rf_spi.h"
#include "sci.h"
#include "spi.h"
#include "sys_ahb_monitor.h"
#include "sys_ctrl.h"
#include "sys_ifc.h"
#include "sys_irq.h"
#include "tcu.h"
#include "timer.h"
#include "uart.h"
#include "vitac.h"
#include "xcor.h"
#include "cp0.h"
#include "regdef.h"
#include "xcpu_cache.h"
#include "xcpu_tag.h"
#include "xcpu.h"
#include "keypad.h"
#include "pwm.h"
#include "calendar.h"
#include "ana_acco.h"
#include "pa_ctrl.h"
#include "afc.h"
#include "gpadc.h"
#include "aif.h"
#include "usbc.h"
#include "voc_ram.h"
#include "voc_ahb.h"
#include "voc_cfg.h"
#include "lcdc.h"
#include "sdmmc.h"
#include "camera.h"

// =============================================================================
//  MACROS
// =============================================================================
/// System APB base
#define REG_SYS_APB_BASE                         (0X00B80000)
/// Baseband APB base
#define REG_BB_APB_BASE                          (0X00AC0000)
/// VOC AHB base
#define REG_VOC_BASE                             (0X00B00000)

// =============================================================================
//  TYPES
// =============================================================================


#endif
