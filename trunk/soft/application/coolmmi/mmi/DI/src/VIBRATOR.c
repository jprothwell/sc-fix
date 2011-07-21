////////////////////////////////////////////////////////////////////////////////
//        Copyright (C) 2002-2006, Coolsand Technologies, Inc.
//                       All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmisison of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
//
// FILENAME: VIBRATOR.c
// 
// DESCRIPTION:
//   TODO... 
// 
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   TODO... 
//   dingjian        20070925      build
// 
////////////////////////////////////////////////////////////////////////////////

#include "cswtype.h"
#include "mmi_trace.h"
#include "ts.h"
#include "drv.h"
#include "dm.h"
#include "vibrator.h"
#include "di.h"
#include "chip_id.h"
#include "pmd_m.h"
////////////////////////////////////////////////////////////////////////////////
//	FUNCTION NAME		: open viabrator device
//  	PURPOSE				:
//	INPUT PARAMETERS	: void
//	OUTPUT PARAMETERS	: void
//	RETURNS				:
 
////////////////////////////////////////////////////////////////////////////////
VOID vibrator_open(VOID)
{	
    // set flag(5)=1
    g_sw_DI_FLAG = (g_sw_DI_FLAG | 0x10);
   pmd_SetLevel(PMD_LEVEL_VIBRATOR, 1);
}

////////////////////////////////////////////////////////////////////////////////
//	FUNCTION NAME		: close viabrator device
//  	PURPOSE				:
//	INPUT PARAMETERS	: void
//	OUTPUT PARAMETERS	: void
//	RETURNS				:
 
////////////////////////////////////////////////////////////////////////////////
VOID vibrator_close(VOID)
{	
    // set flag(5)=0
    g_sw_DI_FLAG = (g_sw_DI_FLAG & 0xEF);
    pmd_SetLevel(PMD_LEVEL_VIBRATOR, 0);	
}

////////////////////////////////////////////////////////////////////////////////
//	FUNCTION NAME		: get viabrator status
//  	PURPOSE				:
//	INPUT PARAMETERS	: void
//	OUTPUT PARAMETERS	: void
//	RETURNS				: bool
 
////////////////////////////////////////////////////////////////////////////////
BOOL vibrator_status(VOID)
{
    if(0x10 == (g_sw_DI_FLAG & 0x10))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

