/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2006, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      vpp_amjr_asm.c                                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This contains the VoC assembly code for the multimedia encoder    */
/*                                                                       */
/*************************************************************************/
///
///   @file vpp_Amjr_asm.c
///   This contains the VoC assembly code for Multimedia.
///

#include "cs_types.h"
#include "vppp_amjr_asm_map.h"


#define RAM_X_BEGIN_ADDR               0x0000
#define RAM_Y_BEGIN_ADDR               0x4000

#define TO32(lo,hi)                    ((lo & 0xffff) | ((hi &0xffff) << 16))



// ******************************
//            COMMON
// ******************************

CONST INT32 g_vppAmjrCommonCode[] = {
#include "vpp_amjr_asm_common.tab"
};

CONST INT32 g_vppAmjrCommonCodeSize = sizeof(g_vppAmjrCommonCode);


// ******************************
//            JPEG
// ******************************

CONST INT32 g_vppAmjrJpegCode[] = {
#include "vpp_amjr_asm_jpeg.tab"
};

CONST INT32 g_vppAmjrJpegConstY[] = {
#include "vpp_amjr_const_jpeg_y.tab"
};

// ******************************
//            AMR
// ******************************

CONST INT32 g_vppAmjrAmrCode[] ={
#include "vpp_amjr_asm_amr.tab"
};

CONST INT32 g_vppAmjrAmrConstX[] =
{
#include "vpp_amjr_const_amr_x.tab"
};

CONST INT32 g_vppAmjrAmrConstY[] =
{
#include "vpp_amjr_const_amr_y.tab"
};

// ******************************
//            MP3
// ******************************

CONST INT32 g_vppAmjrMp3Code[] ={
#include "vpp_amjr_asm_mp3.tab"
};

CONST INT32 g_vppAmjrMp3ConstX[] =
{
#include "vpp_amjr_const_mp3_x.tab"
};

CONST INT32 g_vppAmjrMp3ConstY[] =
{
#include "vpp_amjr_const_mp3_y.tab"
};
