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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/boot/greenstone/src/bootp_mode.h $ //
//    $Author: admin $                                                        // 
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                     //   
//    $Revision: 269 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file halp_boot_asm.h                                                     //
/// That file provides defines used by the assembly boot code.                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _HALP_BOOT_MODE_H_
#define _HALP_BOOT_MODE_H_


#define BOOT_MODE_STD                   0

#define BOOT_MODE_NO_AUTO_PU          (1<<0)
#define BOOT_MODE_FORCE_MONITOR       (1<<1)
#define BOOT_MODE_UART_MONITOR_ENABLE (1<<2)
#define BOOT_MODE_USB_MONITOR_DISABLE (1<<3)
#define BOOT_MODE_FLASH_ADMUX         (1<<5)


#endif // _HALP_BOOT_MODE_H_


