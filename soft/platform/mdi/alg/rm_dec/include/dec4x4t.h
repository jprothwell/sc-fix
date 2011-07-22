/* ***** BEGIN LICENSE BLOCK *****
 * Source last modified: $Id: dec4x4t.c,v 1.3 2005/04/27 19:35:00 hubbe Exp $
 * 
 * REALNETWORKS CONFIDENTIAL--NOT FOR DISTRIBUTION IN SOURCE CODE FORM
 * Portions Copyright (c) 1995-2002 RealNetworks, Inc.
 * All Rights Reserved.
 * 
 * The contents of this file, and the files included with this file,
 * are subject to the current version of the Real Format Source Code
 * Porting and Optimization License, available at
 * https://helixcommunity.org/2005/license/realformatsource (unless
 * RealNetworks otherwise expressly agrees in writing that you are
 * subject to a different license).  You may also obtain the license
 * terms directly from RealNetworks.  You may not use this file except
 * in compliance with the Real Format Source Code Porting and
 * Optimization License. There are no redistribution rights for the
 * source code of this file. Please see the Real Format Source Code
 * Porting and Optimization License for the rights, obligations and
 * limitations governing use of the contents of the file.
 * 
 * RealNetworks is the developer of the Original Code and owns the
 * copyrights in the portions it created.
 * 
 * This file, and the files included with this file, is distributed and
 * made available on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, AND REALNETWORKS HEREBY DISCLAIMS ALL
 * SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT
 * OR NON-INFRINGEMENT.
 * 
 * Technology Compatibility Kit Test Suite(s) Location:
 * https://rarvcode-tck.helixcommunity.org
 * 
 * Contributor(s):
 * 
 * ***** END LICENSE BLOCK ***** */

#ifndef DEC4X4T_H__
#define DEC4X4T_H__

/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
/* 4614 frames processed */
#include "dec4x4m.h"

typedef struct {
    unsigned long intra_cbp[MAX_INTRA_QP_REGIONS][2][(MAX_CBP+7)/8];
    unsigned long intra_8x8_dsc[MAX_INTRA_QP_REGIONS][2][4][(MAX_8x8_DSC+7)/8];
    unsigned long intra_luma_4x4_dsc[MAX_INTRA_QP_REGIONS][3][(MAX_4x4_DSC+7)/8];
    unsigned long intra_luma_2x2_dsc[MAX_INTRA_QP_REGIONS][2][(MAX_2x2_DSC+7)/8];
    unsigned long intra_chroma_4x4_dsc[MAX_INTRA_QP_REGIONS][(MAX_4x4_DSC+7)/8];
    unsigned long intra_chroma_2x2_dsc[MAX_INTRA_QP_REGIONS][2][(MAX_2x2_DSC+7)/8];
    unsigned long intra_level_dsc[MAX_INTRA_QP_REGIONS][(MAX_LEVEL_DSC+7)/8];
    unsigned long inter_cbp[MAX_INTER_QP_REGIONS][(MAX_CBP+7)/8];
    unsigned long inter_8x8_dsc[MAX_INTER_QP_REGIONS][4][(MAX_8x8_DSC+7)/8];
    unsigned long inter_luma_4x4_dsc[MAX_INTER_QP_REGIONS][(MAX_4x4_DSC+7)/8];
    unsigned long inter_luma_2x2_dsc[MAX_INTER_QP_REGIONS][2][(MAX_2x2_DSC+7)/8];
    unsigned long inter_chroma_4x4_dsc[MAX_INTER_QP_REGIONS][(MAX_4x4_DSC+7)/8];
    unsigned long inter_chroma_2x2_dsc[MAX_INTER_QP_REGIONS][2][(MAX_2x2_DSC+7)/8];
    unsigned long inter_level_dsc[MAX_INTER_QP_REGIONS][(MAX_LEVEL_DSC+7)/8];
} rv_table_x;

extern const rv_table_x _x_rv; 

extern const int intra_qp_to_idx [MAX_QP+1];

extern const int inter_qp_to_idx [MAX_QP+1];

extern const unsigned int _y[8] ;

#endif