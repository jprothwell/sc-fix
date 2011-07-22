/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2006, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      vppp_amjp_asm.h                                                  */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This is private header of VPP Audio Motion JPEG VoC assembly      */
/*                                                                       */
/*************************************************************************/
///
///   @file vppp_amjp_asm.h
///   This is private header of VPP Audio Motion JPEG VoC assembly.
///

#ifndef VPPP_AMJP_ASM_H
#define VPPP_AMJP_ASM_H

#include "cs_types.h"

#ifndef CS_PLATFORM_SIMU

 // avoid double declaration in simulation
 #define RAM_X_BEGIN_ADDR               0x0000
 #define RAM_Y_BEGIN_ADDR               0x4000

#endif

#define VPP_AMJP_MAIN_SIZE               g_vppAmjpCommonCodeSize
#define VPP_AMJP_MAIN_ENTRY              0
#define VPP_AMJP_CRITICAL_SECTION_MIN    0x8a
#define VPP_AMJP_CRITICAL_SECTION_MAX    0xf6

EXPORT CONST INT32 g_vppAmjpCommonCode[];
EXPORT CONST INT32 g_vppAmjpCommonCodeSize;
EXPORT CONST INT32 g_vppAmjpJpegCode[];
EXPORT CONST INT32 g_vppAmjpJpegConstY[];
EXPORT CONST INT32 g_vppAmjpMp12Code[];
EXPORT CONST INT32 g_vppAmjpMp12ConstY[];
EXPORT CONST INT32 g_vppAmjpMp3Code[];
EXPORT CONST INT32 g_vppAmjpMp3ConstX[];
EXPORT CONST INT32 g_vppAmjpMp3ConstY[];
EXPORT CONST INT32 g_vppAmjpAmrCode[];
EXPORT CONST INT32 g_vppAmjpAmrConstY[];
EXPORT CONST INT32 g_vppAmjpAacCode[];
EXPORT CONST INT32 g_vppAmjpAacConstY[];
EXPORT CONST INT32 g_vppAmjpZoomCode[];
EXPORT CONST INT32 g_vppAmjpZoomConstY[];
EXPORT CONST INT16 g_vppAmjpConstAudioEq[];


#endif  // VPPP_AMJP_ASM_H
