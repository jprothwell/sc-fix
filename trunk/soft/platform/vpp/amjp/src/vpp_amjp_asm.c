/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2006, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      vpp_amjp_asm.c                                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This contains the VoC assembly code for Multimedia.               */
/*                                                                       */
/*************************************************************************/
///
///   @file vpp_amjp_asm.c
///   This contains the VoC assembly code for Multimedia.
///

#include "cs_types.h"
#include "vppp_amjp_asm_map.h"

#define RAM_X_BEGIN_ADDR               0x0000
#define RAM_Y_BEGIN_ADDR               0x4000

#define TO32(lo,hi)                    ((lo & 0xffff) | ((hi &0xffff) << 16))


// ******************************
//            COMMON
// ******************************

CONST INT32 g_vppAmjpCommonCode[] = {
#include "vpp_amjp_asm_common.tab"
};

CONST INT32 g_vppAmjpCommonCodeSize = sizeof(g_vppAmjpCommonCode);

// ******************************
//            JPEG
// ******************************

CONST INT32 g_vppAmjpJpegCode[] = {
#include "vpp_amjp_asm_jpeg.tab"
};

CONST INT32 g_vppAmjpJpegConstY[] = {
#include "vpp_amjp_const_jpeg_y.tab"
};

// ******************************
//            MP12
// ******************************

CONST INT32 g_vppAmjpMp12Code[] ={
#include "vpp_amjp_asm_mp12.tab"
};

CONST INT32 g_vppAmjpMp12ConstY[] = {
#include "vpp_amjp_const_mp12_y.tab"
};

// ******************************
//            MP3
// ******************************

CONST INT32 g_vppAmjpMp3Code[] ={
#include "vpp_amjp_asm_mp3.tab"
};

CONST INT32 g_vppAmjpMp3ConstX[] = {
#include "vpp_amjp_const_mp3_x.tab"
};

CONST INT32 g_vppAmjpMp3ConstY[] = {
#include "vpp_amjp_const_mp3_y.tab"
};

// ******************************
//            AMR
// ******************************

CONST INT32 g_vppAmjpAmrCode[] = {
#include "vpp_amjp_asm_amr.tab"
};

CONST INT32 g_vppAmjpAmrConstY[] = {
#include "vpp_amjp_const_amr_y.tab"
};

// ******************************
//            AAC
// ******************************

CONST INT32 g_vppAmjpAacCode[] = {
#include "vpp_amjp_asm_aac.tab"
};

CONST INT32 g_vppAmjpAacConstY[] = {
#include "vpp_amjp_const_aac_y.tab"
};

// ******************************
//            ZOOM
// ******************************

CONST INT32 g_vppAmjpZoomCode[] = {
#include "vpp_amjp_asm_zoom.tab"
};

CONST INT32 g_vppAmjpZoomConstY[] = {
#include "vpp_amjp_const_zoom_y.tab"
};

// ******************************
//            EQUALIZER
// ******************************

CONST INT16 g_vppAmjpConstAudioEq[] = {
#include "vpp_amjp_const_audio_eq.tab"
};

