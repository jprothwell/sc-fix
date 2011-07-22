//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spal_misc.c
///
/// This file contains miscellanous functions for SPAL
/// !!! THIS FILE IS COMPILED IN MIPS32 !!!
///
/// @date 27/06/07
/// @author Laurent Bastide, Guillaume LEBRUN
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

// include
#include "cs_types.h"


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------


//======================================================================
// spal_InvalidateDcache 
//----------------------------------------------------------------------
// Data cache invalidation
// !!! THIS FUNCTION SHOULD BE COMPILED IN MIPS32 !!!
//
//======================================================================
PROTECTED VOID spal_InvalidateDcache(VOID)
{
    asm volatile (".align    4");
    asm volatile ("nop ");
    asm volatile (".align    4");
    asm volatile (".word 0xbc000002");
    asm volatile ("nop ");
    asm volatile ("nop ");
    asm volatile ("nop ");
}

//======================================================================
// spal_InvalidateCache 
//----------------------------------------------------------------------
// Both caches invalidation
// !!! THIS FUNCTION SHOULD BE COMPILED IN MIPS32 !!!
//
//======================================================================
PROTECTED VOID spal_InvalidateCache(VOID)
{
    asm volatile (".align    4");
    asm volatile ("nop ");
    asm volatile (".align    4");
    asm volatile (".word 0xbc000000");
    asm volatile ("nop ");
    asm volatile ("nop ");
    asm volatile ("nop ");
}


