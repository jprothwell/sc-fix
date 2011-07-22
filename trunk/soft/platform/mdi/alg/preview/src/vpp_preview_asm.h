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

#ifndef vpp_PREVIEW_ASM_H
#define vpp_PREVIEW_ASM_H

#include "cs_types.h"

#define vpp_Preview_CODE_SIZE       1200*4
   
#define vpp__Preview_CODE_ENTRY 0
#define vpp__Preview_CRITICAL_SECTION_MIN 0
#define vpp__Preview_CRITICAL_SECTION_MAX 0
#define vpp__Preview_CONST_DATA_SIZE      (1536+104)*2

#define vpp__Preview_IN_STRUCT                                    (0   + 0x4000)
#define CONST_TabV2R_ADDR                                            (0) //768 short
#define CONST_TabU2G_ADDR                       			(14 + 0x4000) //256 short

 

const INT32 G_VppPreviewCode[] =
{
           #include "bilinerzoom.tab"
};

const INT32 G_VppCutzzomCode[] =
{
           #include "cutzoom.tab"
};

const INT16 G_CUTZOOMDATA_TABX_ADDR[]=
{
         #include "Cutzoom_contdata_x.tab"
};

const INT16 G_CUTZOOMDATA_TABY_ADDR[]=
{
         #include "Cutzoom_contdata_y.tab"
};

#endif  // vpp_PREVIEW_ASM_H

