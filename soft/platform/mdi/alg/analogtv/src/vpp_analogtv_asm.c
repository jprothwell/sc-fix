/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2006, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      vppp_analogtv_asm.c                                                */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This contains the VoC assembly code for the Analog TV.               */
/*                                                                       */
/*************************************************************************/

#include "cs_types.h"

PROTECTED CONST UINT16  g_vppAnalogTVConstX[1536+104]=
{
	#include "vpp_analogtv_const_x.tab"
};

PROTECTED CONST UINT16  g_vppAnalogTVConstY[1024]=
{
	#include "vpp_analogtv_const_y.tab"
};


PROTECTED CONST INT32 g_vppAnalogTVMainCode[]=
{
   #include "vpp_analogtv_asm_main.tab"
};


PROTECTED CONST INT32 g_vppAnalogTVMainCodeSize = sizeof(g_vppAnalogTVMainCode);
PROTECTED CONST INT32 g_vppAnalogTVConstXSize = sizeof(g_vppAnalogTVConstX);
PROTECTED CONST INT32 g_vppAnalogTVConstYSize = sizeof(g_vppAnalogTVConstY);

