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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/hal/src/hal_assert.c $ //
//    $Author: admin $                                                        // 
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                     //   
//    $Revision: 269 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
///     @file hal_assert.c
///     Provide services for driver internal tests
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "string.h"
#include <stdarg.h>
#include "stdio.h"

#include "hal_host.h"
#include "hal_sys.h"

#include "halp_debug.h"
#include "boot.h"

#include "sxs_io.h"


#if (ASICSIMU == 1)
#include "asicsimu_m.h"
#endif


#define HAL_ASSERT_ERROR_FIFO_SIZE       32
#define HAL_ASSERT_CODE              0xa55E
#define HAL_ASSERT_DELAY             100000
#define HAL_ASSERT_LOOP_QTY          20


// =============================================================================
// hal_DbgAssert
// -----------------------------------------------------------------------------
/// Function to call when 'building' an assert, if the condition is false.
/// @param format String format a-la printf, displayed in the trace.
// =============================================================================
PUBLIC VOID hal_DbgAssert(CONST CHAR* format, ...)
{
    va_list args;
    CHAR msg[256];
    
    va_start(args, format);
    vsprintf(msg, format, args);

#if (ASICSIMU == 1)
    asicsimu_Fatal((format==NULL)? "FATAL assert":(CHAR *)msg);
#endif

    hal_HstSendEvent(0xA55E47);
    // We never go out of this function, so we can safely assume
    // it is possible to read it from coolwatcher.
    hal_HstSendEvent((UINT32)msg);
    HAL_PROFILE_PULSE(XCPU_ERROR);
    hal_DbgFatalTrigger(HAL_DBG_EXL_FATAL);

    xcpu_error_code = HAL_ASSERT_CODE;
    xcpu_error_status = 0;
    
    //  Print string to display 
    // TSMAP is the bitfield of parameters which are string
    // sxs_Raise is a no-exit function    
    UINT32 raiseId = _PAL|TSMAP(0x1)|TABORT;
    sxs_Raise(raiseId,
          "\n##############################################\n"
          "# FATAL assert \n"
          "#    --> %s\n"
          "##############################################",
          (format==NULL? "/no string/":(CHAR *)msg));

//  ...and at last end loop
    while (1);

    va_end(args);
}


