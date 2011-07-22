/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2006, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      vppp_speech_asm.h                                                */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This is private header of VPP Speech VoC assembly.                */
/*                                                                       */
/*************************************************************************/
///
///   @file vppp_speech.h
///   This is private header of VPP Speech VoC assembly.
///

#ifndef VPPP_SPEECH_ASM_H
#define VPPP_SPEECH_ASM_H

#include "cs_types.h"

#ifndef CS_PLATFORM_SIMU

 #define RAM_X_BEGIN_ADDR 0x0
 #define RAM_Y_BEGIN_ADDR 0x4000

#endif

#define VPP_SPEECH_MAIN_ENTRY           0
#define VPP_SPEECH_CRITICAL_SECTION_MIN 0x210
#define VPP_SPEECH_CRITICAL_SECTION_MAX 0x2d2

EXPORT INT32 g_vppEfrAmrConstX[];
EXPORT INT32 g_vppEfrAmrConstY[];
EXPORT INT32 g_vppHrConstX[];
EXPORT INT32 g_vppHrConstY[];

EXPORT  INT32 g_vppSpeechRAMX[];
EXPORT  INT32 g_vppSpeechCode[];
EXPORT  INT32 g_vppAecRAMX[];
EXPORT INT32 g_vppAecConstantX[];
EXPORT INT32 g_vppAecCode[];


EXPORT  INT32   g_vppSpeechRAMY[];
EXPORT  INT32   g_vppMorphConstantY[];
EXPORT  INT32   g_vppMorphCode[];


EXPORT INT32 g_vppSpeechMainCodeSize;
EXPORT INT32 g_vppSpeechMainCode[];
EXPORT INT32 g_vppEfrAmrCode[];
EXPORT INT32 g_vppFrCode[];
EXPORT INT32 g_vppHrCode[];
EXPORT INT32 g_vppAmrCode[];
EXPORT INT32 g_vppEfrCode[];
EXPORT INT32 g_vppAmrEncCode475[];
EXPORT INT32 g_vppAmrEncCode515[];
EXPORT INT32 g_vppAmrEncCode59[];
EXPORT INT32 g_vppAmrEncCode67[];
EXPORT INT32 g_vppAmrEncCode74[];
EXPORT INT32 g_vppAmrEncCode795[];
EXPORT INT32 g_vppAmrEncCode102[];
EXPORT INT32 g_vppAmrEncCode122[];
EXPORT INT32 g_vppAmrDecCode475[];
EXPORT INT32 g_vppAmrDecCode515[];
EXPORT INT32 g_vppAmrDecCode59[];
EXPORT INT32 g_vppAmrDecCode67[];
EXPORT INT32 g_vppAmrDecCode74[];
EXPORT INT32 g_vppAmrDecCode795[];
EXPORT INT32 g_vppAmrDecCode102[];
EXPORT INT32 g_vppAmrDecCode122[];


#endif  // VPPP_SPEECH_ASM_H
