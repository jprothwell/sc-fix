/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2006, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      vpp_hello_asm.c                                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This contains the VoC assembly code for the multimedia encoder    */
/*                                                                       */
/*************************************************************************/
///
///   @file vpp_Hello_asm.c
///   This contains the VoC assembly code for Multimedia.
///

#include "cs_types.h"
#include "vppp_morphvoice_asm_map.h"


#define RAM_X_BEGIN_ADDR               0x0000
#define RAM_Y_BEGIN_ADDR               0x4000

#define TO32(lo,hi)                    ((lo & 0xffff) | ((hi &0xffff) << 16))



// ******************************
//            COMMON
// ******************************

CONST INT32 g_vppHelloCommonCode[] = 
{
	#include "vpp_morphvoice_asm_common.tab"
};

CONST INT32 g_vppHelloCommonCodeSize = sizeof(g_vppHelloCommonCode);


// ******************************
//         OTHER CODE
// ******************************

CONST INT32 g_vppHelloHiCode[] = 
{
	#include "vpp_morphvoice_asm_hi.tab"
};

// ******************************
//            CONST
// ******************************

CONST INT32 g_vppHelloConst[] =
{
	#include "vpp_morphvoice_const.tab"
};
