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

#ifndef vpp_AudioJpeg_ENC_ASM_H
#define vpp_AudioJpeg_ENC_ASM_H

#include "cs_types.h"

#define vpp_AudioJpeg_ENC_CODE_SIZE       120*4
   
#define vpp_AudioJpeg_ENC_CODE_ENTRY 0
#define vpp_AudioJpeg_ENC_CRITICAL_SECTION_MIN 0
#define vpp_AudioJpeg_ENC_CRITICAL_SECTION_MAX 0

#define vpp_AudioJpeg_ENC_STREAM_BUF_STRUCT (   0 + 0x0)
#define vpp_AudioJpeg_ENC_IN_STRUCT         (0   + 0x4000) 
#define vpp_AudioJpeg_ENC_OUT_STRUCT        (14    + 0x4000)


#define VPP_MP3_Enc_Code_ExternalAddr_addr        (22 + 0x4000)
#define VPP_AMR_Enc_Code_ExternalAddr_addr        (32 + 0x4000)

#define VPP_GLOBAL_MIC_DIGITAL_GAIN_ADDR_addr        (38 + 0x4000)

#ifdef VOCCODE_IN_PSRAM

//common code
#if 0
 INT32 G_VppCommonEncCode[] = 
{
#include "audiojpeg_enc_common.tab"
};
#else
#ifdef _CAM_SMALL_MEM_
 INT32 G_VppCommonEncCode[] = 
{
#include "audiojpeg_enc_common_qvga.tab"
};
#else
 INT32 G_VppCommonEncCode[] = 
{
#include "audiojpeg_enc_common_vga.tab"
};
#endif
#endif



//mp3

 INT32 G_VppMp3EncCode[] =
{
#include "mp3enc.tab"
};

 INT32 G_VppMp3EncConstX[] = 
{
#include "vpp_mp3_enc_const_x.tab"
};

 INT32 G_VppMp3EncConstY[] = 
{
#include "vpp_mp3_enc_const_y.tab"
};


 INT32 G_VppMp3EncConst_rqmy[] = 
{
#include "vpp_mp3_enc_const_rqmy.tab"
};

 INT32 G_VppMp3EncConst_Zig[] = 
{
#include "vpp_mp3_enc_const_zig.tab"
};



//amrjpeg
#if 0
 INT32 G_VppAmrJpegEncCode[] =
{
 #include "amrjpegenc.tab"
};
#else
#ifdef _CAM_SMALL_MEM_
 INT32 G_VppAmrJpegEncCode[] =
{
 #include "amrjpegenc_qvga.tab"
};
#else
 INT32 G_VppAmrJpegEncCode[] =
{
 #include "amrjpegenc_vga.tab"
};
#endif
#endif

 INT32 G_VppAmrJpegEncConstX[] = 
{
#include "vpp_amrjpeg_enc_const_x.tab"
};

 INT32 G_VppAmrJpegEncConstY[] = 
{
#include "vpp_amrjpeg_enc_const_y.tab"
};


#else
//common code
#if 0
const INT32 G_VppCommonEncCode[] = 
{
#include "audiojpeg_enc_common.tab"
};
#else
#ifdef _CAM_SMALL_MEM_
const INT32 G_VppCommonEncCode[] = 
{
#include "audiojpeg_enc_common_qvga.tab"
};
#else
const INT32 G_VppCommonEncCode[] = 
{
#include "audiojpeg_enc_common_vga.tab"
};
#endif
#endif



//mp3

const INT32 G_VppMp3EncCode[] =
{
#include "mp3enc.tab"
};

const INT32 G_VppMp3EncConstX[] = 
{
#include "vpp_mp3_enc_const_x.tab"
};

const INT32 G_VppMp3EncConstY[] = 
{
#include "vpp_mp3_enc_const_y.tab"
};


const INT32 G_VppMp3EncConst_rqmy[] = 
{
#include "vpp_mp3_enc_const_rqmy.tab"
};

const INT32 G_VppMp3EncConst_Zig[] = 
{
#include "vpp_mp3_enc_const_zig.tab"
};



//amrjpeg
#if 0
const INT32 G_VppAmrJpegEncCode[] =
{
 #include "amrjpegenc.tab"
};
#else
#ifdef _CAM_SMALL_MEM_
const INT32 G_VppAmrJpegEncCode[] =
{
 #include "amrjpegenc_qvga.tab"
};
#else
const INT32 G_VppAmrJpegEncCode[] =
{
 #include "amrjpegenc_vga.tab"
};
#endif
#endif

const INT32 G_VppAmrJpegEncConstX[] = 
{
#include "vpp_amrjpeg_enc_const_x.tab"
};

const INT32 G_VppAmrJpegEncConstY[] = 
{
#include "vpp_amrjpeg_enc_const_y.tab"
};

#endif


#endif  // vpp_AudioJpeg_ENC_ASM_H

