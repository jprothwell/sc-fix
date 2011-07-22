
/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2006, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      vppp_hello_asm.h                                                  */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This is private header of VPP Audio Motion JPEG VoC assembly      */
/*                                                                       */
/*************************************************************************/
///
///   @file vppp_hello_asm.h
///   This is private header of VPP Audio Motion JPEG VoC assembly.
///

#ifndef VPPP_HELLO_ASM_H
#define VPPP_HELLO_ASM_H

#include "cs_types.h"

#ifndef CS_PLATFORM_SIMU

 // avoid double declaration in simulation
 #define RAM_X_BEGIN_ADDR               0x0000
 #define RAM_Y_BEGIN_ADDR               0x4000

#endif

#define VPP_HELLO_MAIN_SIZE               g_vppHelloCommonCodeSize
#define VPP_HELLO_MAIN_ENTRY              0
#define VPP_HELLO_CRITICAL_SECTION_MIN    0
#define VPP_HELLO_CRITICAL_SECTION_MAX    0

EXPORT CONST INT32 g_vppHelloCommonCode[];
EXPORT CONST INT32 g_vppHelloCommonCodeSize;
EXPORT CONST INT32 g_vppHelloHiCode[];
EXPORT CONST INT32 g_vppHelloByeCode[];
EXPORT CONST INT32 g_vppHelloConst[];

#endif  // VPPP_HELLO_ASM_H
