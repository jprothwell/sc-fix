/*****************************************************************************
 *
 *  XVID MPEG-4 VIDEO CODEC
 *  - Global definitions  -
 *
 *  Copyright(C) 2002 Michael Militzer <isibaar@xvid.org>
 *
 *  This program is free software ; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation ; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY ; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program ; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * $Id: global.h,v 1.24 2005/09/13 12:12:15 suxen_drol Exp $
 *
 ****************************************************************************/
#ifndef SHEEN_VC_DEBUG 
#include "global.h"
#endif

#ifdef SHEEN_VC_DEBUG 
typedef unsigned char uint8;
typedef char int8;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef int int32;

typedef unsigned char uint8_t;
typedef char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

typedef char bool;
typedef char zoomPar;
#define mmc_MemMalloc malloc
#define mmc_MemGetFree() 1024*1024
#define ABS abs
#endif

 #ifdef MP4_3GP_SUPPORT

#ifndef _MPEG4_GLOBAL_H_
#define _MPEG4_GLOBAL_H_

#include "xvid.h"
#include "portab.h"
//add by longyl
#define longyl
//#define _BIG_BUFFER_ADJUST_
/////////////////////////
/* --- macroblock modes --- */

#define MODE_INTER		0
#define MODE_INTER_Q	1
#define MODE_INTER4V	2
#define	MODE_INTRA		3
#define MODE_INTRA_Q	4
#define MODE_NOT_CODED	16
/*
 * vop coding types
 * intra, prediction, backward, sprite, not_coded
 */
#define I_VOP	0
#define P_VOP	1
#define B_VOP	2
#define S_VOP	3
#define N_VOP	4
/////////
/* convert mpeg-4 coding type i/p/b/s_VOP to XVID_TYPE_xxx */
static __inline int
coding2type(int coding_type)
{
	return coding_type + 1;
}

/* convert XVID_TYPE_xxx to bitstream coding type i/p/b/s_VOP */
static __inline int
type2coding(int xvid_type)
{
	return xvid_type - 1;
}


typedef struct
{
	int x;
	int y;
}
VECTOR;

typedef struct
{
	uint8_t *y;
	uint8_t *u;
	uint8_t *v;
}
IMAGE;

typedef struct
{
	uint32_t buf;
	uint16_t pos;//left bits num in buf,diff with original code.sheen
	uint8_t *tail;
	uint8_t *start;
	uint32_t length;
	uint32_t initpos;
}
Bitstream;

/* useful macros */
//#define MIN(X, Y) ((X)<(Y)?(X):(Y))
#define MIN_16(X, Y) ((Y) + (((X-Y)>>15)&((X)-(Y))))
#define MIN_32(X, Y) ((Y) + (((X-Y)>>31)&((X)-(Y))))

//#define MAX(X, Y) ((X)>(Y)?(X):(Y))
#define MAX_16(X, Y) ((Y) + (((Y-X)>>15)&((X)-(Y))))
#define MAX_32(X, Y) ((Y) + (((Y-X)>>31)&((X)-(Y))))

#define SIGN(X)   (((X)>0)?1:-1)
#define CLIP(X,AMIN,AMAX)   (((X)<(AMIN)) ? (AMIN) : ((X)>(AMAX)) ? (AMAX) : (X))
#define SWAP(_T_,A,B)    { _T_ tmp = A; A = B; B = tmp; }


#endif							/* _GLOBAL_H_ */
#endif

