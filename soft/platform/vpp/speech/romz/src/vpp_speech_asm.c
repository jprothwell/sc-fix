/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2006, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      vppp_speech_asm.c                                                */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This contains the VoC assembly code for the Speech.               */
/*                                                                       */
/*************************************************************************/
///
///   @file vppp_speech.h
///   This contains the VoC assembly code for the Speech.
///

#include "cs_types.h"

// -----------------------------------
// VPP Speech VoC assembly Code Images
// for the code using a ROM
// -----------------------------------

// this flag is also used to define if the
// VPP speech code is in the Membridge ROM

PROTECTED CONST INT32 g_vppSpeechMainCode[] = {
#include "../obj/vpp_speech_asm_main.tab"
};

PROTECTED CONST INT32 g_vppHrCode[] = {
#include "../obj/vpp_speech_asm_hr.tab"
};


#if (CHIP_BYPASS_ROM) || defined(FPGA) || defined(PLATFORM_SIMU)


PROTECTED CONST INT32 g_vppEfrAmrCode[] = {
#include "../obj/vpp_speech_asm_amr_efr.tab"
};

PROTECTED CONST INT32 g_vppFrCode[] = {
#include "../obj/vpp_speech_asm_fr.tab"
};

PROTECTED CONST INT32 g_vppAmrCode[] = {
#include "../obj/vpp_speech_asm_amr.tab"
};

PROTECTED CONST INT32 g_vppEfrCode[] = {
#include "../obj/vpp_speech_asm_efr.tab"
};

PROTECTED CONST INT32 g_vppAmrEncCode475[] = {
#include "../obj/vpp_speech_asm_amr475_enc.tab"
};

PROTECTED CONST INT32 g_vppAmrEncCode515[] = {
#include "../obj/vpp_speech_asm_amr515_enc.tab"
};

PROTECTED CONST INT32 g_vppAmrEncCode59[] = {
#include "../obj/vpp_speech_asm_amr59_enc.tab"
};

PROTECTED CONST INT32 g_vppAmrEncCode67[] = {
#include "../obj/vpp_speech_asm_amr67_enc.tab"
};

PROTECTED CONST INT32 g_vppAmrEncCode74[] = {
#include "../obj/vpp_speech_asm_amr74_enc.tab"
};

PROTECTED CONST INT32 g_vppAmrEncCode795[] = {
#include "../obj/vpp_speech_asm_amr795_enc.tab"
};

PROTECTED CONST INT32 g_vppAmrEncCode102[] = {
#include "../obj/vpp_speech_asm_amr102_enc.tab"
};

PROTECTED CONST INT32 g_vppAmrEncCode122[] = {
#include "../obj/vpp_speech_asm_amr122_enc.tab"
};

PROTECTED CONST INT32 g_vppAmrDecCode475[] = {
#include "../obj/vpp_speech_asm_amr475_dec.tab"
};

PROTECTED CONST INT32 g_vppAmrDecCode515[] = {
#include "../obj/vpp_speech_asm_amr515_dec.tab"
};

PROTECTED CONST INT32 g_vppAmrDecCode59[] = {
#include "../obj/vpp_speech_asm_amr59_dec.tab"
};

PROTECTED CONST INT32 g_vppAmrDecCode67[] = {
#include "../obj/vpp_speech_asm_amr67_dec.tab"
};

PROTECTED CONST INT32 g_vppAmrDecCode74[] = {
#include "../obj/vpp_speech_asm_amr74_dec.tab"
};

PROTECTED CONST INT32 g_vppAmrDecCode795[] = {
#include "../obj/vpp_speech_asm_amr795_dec.tab"
};

PROTECTED CONST INT32 g_vppAmrDecCode102[] = {
#include "../obj/vpp_speech_asm_amr102_dec.tab"
};

PROTECTED CONST INT32 g_vppAmrDecCode122[] = {
#include "../obj/vpp_speech_asm_amr122_dec.tab"
};

#else // (!CHIP_BYPASS_ROM)

// -----------------------------------
// use symbols from memory bridge ROM
// -----------------------------------


#endif // (CHIP_BYPASS_ROM)


PROTECTED CONST INT32 g_vppSpeechMainCodeSize = sizeof(g_vppSpeechMainCode);


