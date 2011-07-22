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
  
 
#ifdef VOCCODE_IN_PSRAM

PROTECTED  INT32   g_vppSpeechMainCode[] = {
#include "vpp_speech_asm_main.tab"
};

PROTECTED  INT32   g_vppHrCode[] = {
#include "vpp_speech_asm_hr.tab"
};

PROTECTED  INT32   g_vppEfrAmrCode[] = {
#include "vpp_speech_asm_amr_efr.tab"
};

PROTECTED  INT32   g_vppFrCode[] = {
#include "vpp_speech_asm_fr.tab"
};

PROTECTED  INT32   g_vppAmrCode[] = {
#include "vpp_speech_asm_amr.tab"
};

PROTECTED  INT32   g_vppEfrCode[] = {
#include "vpp_speech_asm_efr.tab"
};

PROTECTED  INT32   g_vppAmrEncCode475[] = {
#include "vpp_speech_asm_amr475_enc.tab"
};

PROTECTED  INT32   g_vppAmrEncCode515[] = {
#include "vpp_speech_asm_amr515_enc.tab"
};

PROTECTED  INT32   g_vppAmrEncCode59[] = {
#include "vpp_speech_asm_amr59_enc.tab"
};

PROTECTED  INT32   g_vppAmrEncCode67[] = {
#include "vpp_speech_asm_amr67_enc.tab"
};

PROTECTED  INT32   g_vppAmrEncCode74[] = {
#include "vpp_speech_asm_amr74_enc.tab"
};

PROTECTED  INT32   g_vppAmrEncCode795[] = {
#include "vpp_speech_asm_amr795_enc.tab"
};

PROTECTED  INT32   g_vppAmrEncCode102[] = {
#include "vpp_speech_asm_amr102_enc.tab"
};

PROTECTED  INT32   g_vppAmrEncCode122[] = {
#include "vpp_speech_asm_amr122_enc.tab"
};

PROTECTED  INT32   g_vppAmrDecCode475[] = {
#include "vpp_speech_asm_amr475_dec.tab"
};

PROTECTED  INT32   g_vppAmrDecCode515[] = {
#include "vpp_speech_asm_amr515_dec.tab"
};

PROTECTED  INT32   g_vppAmrDecCode59[] = {
#include "vpp_speech_asm_amr59_dec.tab"
};

PROTECTED  INT32   g_vppAmrDecCode67[] = {
#include "vpp_speech_asm_amr67_dec.tab"
};

PROTECTED  INT32   g_vppAmrDecCode74[] = {
#include "vpp_speech_asm_amr74_dec.tab"
};

PROTECTED  INT32   g_vppAmrDecCode795[] = {
#include "vpp_speech_asm_amr795_dec.tab"
};

PROTECTED  INT32   g_vppAmrDecCode102[] = {
#include "vpp_speech_asm_amr102_dec.tab"
};

PROTECTED  INT32   g_vppAmrDecCode122[] = {
#include "vpp_speech_asm_amr122_dec.tab"
};

PROTECTED  INT32   g_vppEfrAmrConstX[] = {
#include "vpp_speech_const_amr_efr_x.tab"
};

PROTECTED INT32   g_vppEfrAmrConstY[] = {
#include "vpp_speech_const_amr_efr_y.tab"
};

PROTECTED  INT32   g_vppHrConstX[] = {
#include "vpp_speech_const_hr_x.tab"
};

PROTECTED  INT32   g_vppHrConstY[] = {
#include "vpp_speech_const_hr_y.tab"
};


PROTECTED  INT32   g_vppSpeechRAMX[1600] = 
{
	0
};

PROTECTED  INT32   g_vppSpeechRAMY[600] = 
{
	0
};

PROTECTED   INT32   g_vppSpeechCode[1400] = 
{
	0
};

PROTECTED  INT32    g_vppAecRAMX[1000] = 
{
	0
};


PROTECTED  INT32   g_vppMorphConstantY[] = {
#include "vpp_morph_const.tab"
};

PROTECTED  INT32   g_vppMorphCode[] = {
#include "vpp_speech_asm_morph.tab"
};


PROTECTED  INT32   g_vppAecConstantX[] = {
#include "vpp_aec_const.tab"
};

PROTECTED  INT32   g_vppAecCode[] = {
#include "vpp_speech_asm_aec.tab"
};

PROTECTED CONST INT32 g_vppSpeechMainCodeSize = sizeof(g_vppSpeechMainCode);


#else
PROTECTED CONST INT32   g_vppSpeechMainCode[] = {
#include "vpp_speech_asm_main.tab"
};

PROTECTED CONST INT32   g_vppHrCode[] = {
#include "vpp_speech_asm_hr.tab"
};

PROTECTED CONST INT32   g_vppEfrAmrCode[] = {
#include "vpp_speech_asm_amr_efr.tab"
};

PROTECTED CONST INT32   g_vppFrCode[] = {
#include "vpp_speech_asm_fr.tab"
};

PROTECTED CONST INT32   g_vppAmrCode[] = {
#include "vpp_speech_asm_amr.tab"
};

PROTECTED CONST INT32   g_vppEfrCode[] = {
#include "vpp_speech_asm_efr.tab"
};

PROTECTED CONST INT32   g_vppAmrEncCode475[] = {
#include "vpp_speech_asm_amr475_enc.tab"
};

PROTECTED CONST INT32   g_vppAmrEncCode515[] = {
#include "vpp_speech_asm_amr515_enc.tab"
};

PROTECTED CONST INT32   g_vppAmrEncCode59[] = {
#include "vpp_speech_asm_amr59_enc.tab"
};

PROTECTED CONST INT32   g_vppAmrEncCode67[] = {
#include "vpp_speech_asm_amr67_enc.tab"
};

PROTECTED CONST INT32   g_vppAmrEncCode74[] = {
#include "vpp_speech_asm_amr74_enc.tab"
};

PROTECTED CONST INT32   g_vppAmrEncCode795[] = {
#include "vpp_speech_asm_amr795_enc.tab"
};

PROTECTED CONST INT32   g_vppAmrEncCode102[] = {
#include "vpp_speech_asm_amr102_enc.tab"
};

PROTECTED CONST INT32   g_vppAmrEncCode122[] = {
#include "vpp_speech_asm_amr122_enc.tab"
};

PROTECTED CONST INT32   g_vppAmrDecCode475[] = {
#include "vpp_speech_asm_amr475_dec.tab"
};

PROTECTED CONST INT32   g_vppAmrDecCode515[] = {
#include "vpp_speech_asm_amr515_dec.tab"
};

PROTECTED CONST INT32   g_vppAmrDecCode59[] = {
#include "vpp_speech_asm_amr59_dec.tab"
};

PROTECTED CONST INT32   g_vppAmrDecCode67[] = {
#include "vpp_speech_asm_amr67_dec.tab"
};

PROTECTED CONST INT32   g_vppAmrDecCode74[] = {
#include "vpp_speech_asm_amr74_dec.tab"
};

PROTECTED CONST INT32   g_vppAmrDecCode795[] = {
#include "vpp_speech_asm_amr795_dec.tab"
};

PROTECTED CONST INT32   g_vppAmrDecCode102[] = {
#include "vpp_speech_asm_amr102_dec.tab"
};

PROTECTED CONST INT32   g_vppAmrDecCode122[] = {
#include "vpp_speech_asm_amr122_dec.tab"
};

PROTECTED CONST INT32   g_vppEfrAmrConstX[] = {
#include "vpp_speech_const_amr_efr_x.tab"
};

PROTECTED CONST INT32   g_vppEfrAmrConstY[] = {
#include "vpp_speech_const_amr_efr_y.tab"
};

PROTECTED CONST INT32   g_vppHrConstX[] = {
#include "vpp_speech_const_hr_x.tab"
};

PROTECTED CONST INT32   g_vppHrConstY[] = {
#include "vpp_speech_const_hr_y.tab"
};


PROTECTED  INT32   g_vppSpeechRAMX[1600] = 
{
	0
};

PROTECTED  INT32   g_vppSpeechRAMY[600] = 
{
	0
};

PROTECTED   INT32   g_vppSpeechCode[1400] = 
{
	0
};

PROTECTED  INT32    g_vppAecRAMX[1000] = 
{
	0
};


PROTECTED CONST INT32   g_vppMorphConstantY[] = {
#include "vpp_morph_const.tab"
};

PROTECTED CONST INT32   g_vppMorphCode[] = {
#include "vpp_speech_asm_morph.tab"
};






PROTECTED CONST INT32   g_vppAecConstantX[] = {
#include "vpp_aec_const.tab"
};

PROTECTED CONST INT32   g_vppAecCode[] = {
#include "vpp_speech_asm_aec.tab"
};

PROTECTED CONST INT32 g_vppSpeechMainCodeSize = sizeof(g_vppSpeechMainCode);

#endif

