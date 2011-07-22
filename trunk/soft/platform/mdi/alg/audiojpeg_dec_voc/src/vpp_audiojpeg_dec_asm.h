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

#ifndef vpp_AudioJpeg_DEC_ASM_H
#define vpp_AudioJpeg_DEC_ASM_H

#include "cs_types.h"

#define vpp_AudioJpeg_DEC_CODE_SIZE       160*4
   
#define vpp_AudioJpeg_DEC_CODE_ENTRY 0
#define vpp_AudioJpeg_DEC_CRITICAL_SECTION_MIN 0
#define vpp_AudioJpeg_DEC_CRITICAL_SECTION_MAX 0

//#define vpp_AudioJpeg_DEC_STREAM_BUF_STRUCT (   0 + 0x0)
#define vpp_AudioJpeg_DEC_IN_STRUCT           (0   + 0x4000) 
#define vpp_AudioJpeg_DEC_OUT_STRUCT        (20  + 0x4000)

#define VPP_Current_Audio_Mode_addr             (36 + 0x4000)


#define VPP_AAC_Code_ExternalAddr_addr             (38 + 0x4000)
#define VPP_AAC_Tab_huffTabSpec_START_addr    (44 + 0x4000)

#define VPP_MP3_Code_ExternalAddr_addr          (52 + 0x4000)
#define VPP_AMR_Code_ExternalAddr_addr          (58 + 0x4000)
#define VPP_JPEG_Code_ExternalAddr_addr         (64+ 0x4000)
#define VPP_H263_Zoom_Code_ExternalAddr_addr     (68+ 0x4000)
#define VPP_SBC_ENC_Code_ExternalAddr_addr          (72+0x4000)

#define VPP_Speak_AntiDistortion_Filter_Coef_addr          (34+0x4000)

#ifdef VOCCODE_IN_PSRAM
//common code
 INT32 G_VppCommonDecCode[] = 
{
#include "audiojpeg_dec_common.tab"
};



//aac
 INT32 G_VppAacDecCode[] = 
{
#include "aacdec.tab"
};

 INT32 G_VppAacDecConstY[] = 
{
#include "vpp_aac_dec_const_y.tab"
};

//mp3

 INT32 G_VppMp3DecCode[] =
{
#include "mp123dec.tab"
};

 INT32 G_VppMp3DecConstX[] = 
{
#include "vpp_mp3_dec_const_x.tab"
};

 INT32 G_VppMp3DecConstY[] = 
{
#include "vpp_mp3_dec_const_y_v1.tab"
};

 INT32 G_VppMp3Layer12Dec_DMA_ConstY[] = 
{
#include "tab_layer_1_2.tab"
};

//amr

 INT32 G_VppAmrDecCode[] =
{
#include "amrdec.tab"
};

 INT32 G_VppMp3Dec_EQ_DMA_ConstY[] = 
{
#include "mp3decequalizer.tab" // mp3 eq
};

 INT32 G_VppAmrDecConstY[] = 
{
#include "vpp_amr_dec_const_y.tab"
};

//jpeg
#ifndef TARGET_VIDEO_PLAYER_SUPPORT_OFF
 INT32 G_VppJpegDecCode[] =
{
#include "jpegdec.tab"
};


 INT32 G_VppJpegDecConstY[] = 
{
#include "vpp_jpeg_dec_const_y.tab"
};



// huffTabSpec
 INT32 G_VppAacDecHuffTabSpec[] = 
{
	#include "tab_hufftabspec.tab"
};

// cos4sin4tab
 INT32 G_VppAacDecCos4sin4tab[] = 
{
	#include "tab_cos4sin4tab.tab"
};

// twidTabOdd
 INT32 G_VppAacDecTwidTabOdd[] = 
{
	#include "tab_twidtabodd.tab"
	#include "aacdecequalizer.tab"
};

// sinWindow
 INT32 G_VppAacDecSinWindow[] = 
{
	#include "tab_sinwindow.tab"
	#include "tab_kbdwindow.tab"
};

// kbdWindow
 INT32 G_VppH263ZoomCode[] = 
{
	#include "H263zoom.tab"
};


 INT32 G_VppH263ZoomConstX[] = 
{
#include "H263_ConstX.tab"
};
 INT32 G_VppRMDecCode[] = 
{
	#include "rmdec.tab"
};


 INT32 G_VppRMDecConsty[] = 
{
#include "vpp_rm_dec_const_y.tab"
};


	 
#endif

//SBC ENCODER
 INT32 G_VppSBCCode[] = 
{
#include "sbcenc.tab"
};

 INT32 G_VppSBCConstX[] = 
{
#include "sbc_constx.tab"
};
#else

//common code
const INT32 G_VppCommonDecCode[] = 
{
#include "audiojpeg_dec_common.tab"
};



//aac
const INT32 G_VppAacDecCode[] = 
{
#include "aacdec.tab"
};

const INT32 G_VppAacDecConstY[] = 
{
#include "vpp_aac_dec_const_y.tab"
};

//mp3

const INT32 G_VppMp3DecCode[] =
{
#include "mp123dec.tab"
};

const INT32 G_VppMp3DecConstX[] = 
{
#include "vpp_mp3_dec_const_x.tab"
};

const INT32 G_VppMp3DecConstY[] = 
{
#include "vpp_mp3_dec_const_y_v1.tab"
};

const INT32 G_VppMp3Layer12Dec_DMA_ConstY[] = 
{
#include "tab_layer_1_2.tab"
};

//amr

const INT32 G_VppAmrDecCode[] =
{
#include "amrdec.tab"
};

const INT32 G_VppMp3Dec_EQ_DMA_ConstY[] = 
{
#include "mp3decequalizer.tab" // mp3 eq
};

const INT32 G_VppAmrDecConstY[] = 
{
#include "vpp_amr_dec_const_y.tab"
};

//jpeg
#ifndef TARGET_VIDEO_PLAYER_SUPPORT_OFF
const INT32 G_VppJpegDecCode[] =
{
#include "jpegdec.tab"
};


const INT32 G_VppJpegDecConstY[] = 
{
#include "vpp_jpeg_dec_const_y.tab"
};



// huffTabSpec
const INT32 G_VppAacDecHuffTabSpec[] = 
{
	#include "tab_hufftabspec.tab"
};

// cos4sin4tab
const INT32 G_VppAacDecCos4sin4tab[] = 
{
	#include "tab_cos4sin4tab.tab"
};

// twidTabOdd
const INT32 G_VppAacDecTwidTabOdd[] = 
{
	#include "tab_twidtabodd.tab"
	#include "aacdecequalizer.tab"
};

// sinWindow
const INT32 G_VppAacDecSinWindow[] = 
{
	#include "tab_sinwindow.tab"
	#include "tab_kbdwindow.tab"
};

// kbdWindow
const INT32 G_VppH263ZoomCode[] = 
{
	#include "H263zoom.tab"
};


const INT32 G_VppH263ZoomConstX[] = 
{
#include "H263_ConstX.tab"
};
const INT32 G_VppRMDecCode[] = 
{
	#include "rmdec.tab"
};


const INT32 G_VppRMDecConsty[] = 
{
#include "vpp_rm_dec_const_y.tab"
};


	 
#endif

//SBC ENCODER
const INT32 G_VppSBCCode[] = 
{
#include "sbcenc.tab"
};

const INT32 G_VppSBCConstX[] = 
{
#include "sbc_constx.tab"
};
#endif
 //INT32 G_VppSBCConstY[552] = //**must can't be const type**
//{
//#include "sbc_consty.tab"
//};
#endif  // vpp_AudioJpeg_DEC_ASM_H
