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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/src/halp_fint.h $ //
//    $Author: admin $                                                        // 
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                     //   
//    $Revision: 269 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file halp_fint.h                                                         //
/// Fint internal Irq handler.                                                //
//                                                                            //
//////////////////////////////////////////////////////////////////////////////// 

// =============================================================================
// hal_FintIrqHandler
// -----------------------------------------------------------------------------
/// Handles IRQ.
// =============================================================================
PROTECTED VOID hal_FintIrqHandler(UINT8 interruptId);


// =============================================================================
// hal_FintUserHandler
// -----------------------------------------------------------------------------
/// called by hal_FintIrqHandler()
// =============================================================================
PROTECTED VOID hal_FintUserHandler(VOID);

