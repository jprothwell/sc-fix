/*****************************************************************************
 *
 *  XVID MPEG-4 VIDEO CODEC
 *  - Bitstream reader/writer inlined functions and constants-
 *
 *  Copyright (C) 2001-2003 Peter Ross <pross@xvid.org>
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
 * $Id: bitstream.h,v 1.22.2.2 2006/11/01 09:26:52 Isibaar Exp $
 *
 ****************************************************************************/
#ifndef SHEEN_VC_DEBUG 
#include "global.h"
#endif
#ifdef MP4_3GP_SUPPORT

#ifndef _MPEG4_BITSTREAM_H_
#define _MPEG4_BITSTREAM_H_

#include "portab.h"
#include "decoder.h"
//#include "../encoder.h"


/*****************************************************************************
 * Constants
 ****************************************************************************/

/* comment any #defs we dont use */

#define VIDOBJ_START_CODE		0x00000100	/* ..0x0000011f  */
#define VIDOBJLAY_START_CODE	0x00000120	/* ..0x0000012f */
#define VISOBJSEQ_START_CODE	0x000001b0
#define VISOBJSEQ_STOP_CODE		0x000001b1	/* ??? */
#define USERDATA_START_CODE		0x000001b2
#define GRPOFVOP_START_CODE		0x000001b3
#define VISOBJ_START_CODE		0x000001b5
#define VOP_START_CODE			0x000001b6

#define VISOBJ_TYPE_VIDEO				1

#define VIDOBJLAY_TYPE_SIMPLE			1
#define VIDOBJLAY_TYPE_ART_SIMPLE		10
#define VIDOBJLAY_TYPE_ASP              17

#define VIDOBJLAY_AR_EXTPAR				15


#define VIDOBJLAY_SHAPE_RECTANGULAR		0
#define VIDOBJLAY_SHAPE_BINARY			1
#define VIDOBJLAY_SHAPE_BINARY_ONLY		2
#define VIDOBJLAY_SHAPE_GRAYSCALE		3


#define SPRITE_NONE		0
#define SPRITE_STATIC	1
#define SPRITE_GMC		2

/* vop coding types  */
/* intra, prediction, backward, sprite, not_coded */
#define I_VOP	0
#define P_VOP	1
#define B_VOP	2
#define S_VOP	3
#define N_VOP	4

/* resync-specific */
#define NUMBITS_VP_RESYNC_MARKER  17
#define RESYNC_MARKER 1

extern const uint32_t msk[33];

/*
static const uint32_t msk[33] = {
  0x00000000, 0x00000001, 0x00000003, 0x00000007,
  0x0000000f, 0x0000001f, 0x0000003f, 0x0000007f,
  0x000000ff, 0x000001ff, 0x000003ff, 0x000007ff,
  0x00000fff, 0x00001fff, 0x00003fff, 0x00007fff,
  0x0000ffff, 0x0001ffff, 0x0003ffff, 0x0007ffff,
  0x000fffff, 0x001fffff, 0x003fffff, 0x007fffff,
  0x00ffffff, 0x01ffffff, 0x03ffffff, 0x07ffffff,
  0x0fffffff, 0x1fffffff, 0x3fffffff, 0x7fffffff,
  0xffffffff
};
*/

/*****************************************************************************
 * Prototypes
 ****************************************************************************/

int read_video_packet_header(Bitstream *bs,
							 DECODER * dec,
							 const int addbits,
							 int *quant,
							 int *fcode_forward,
							 int *fcode_backward,
							 int *intra_dc_threshold);

/* header stuff */
int BitstreamReadHeaders(Bitstream * bs,
						 DECODER * dec,
						 uint16_t * rounding,
						 uint16_t * quant,
						 uint16_t * fcode_forward,
						 uint16_t * intra_dc_threshold);

/*****************************************************************************
 * Bitstream
 ****************************************************************************/

/* Input buffer should be readable as full chunks of 8bytes, including
the end of the buffer. Padding might be appropriate. If only chunks
of 4bytes are applicable, define XVID_SAFE_BS_TAIL. Note that this will
slow decoding, so consider this as a last-resort solution */
/* #define XVID_SAFE_BS_TAIL */

/* initialise bitstream structure */

#define _SHOWBITS(curword,leftpos,num,code) {                  \
	(code) = ((curword) >> ((leftpos) - (num))) & msk[num];				\
}

#define _GETBITS(curword,leftpos,num,code){						\
	(leftpos) -= (num);													\
	(code) = ((curword) >> (leftpos)) & msk[num];	 					\
} 

#define _PREVIEW8BITS(bstream, curword, leftpos)				\
{																		\
	(curword) = ((curword) << 8) | (uint32_t)(*(bstream));				\
	(leftpos) += 8;														\
	(bstream) += 1;														\
}

#define _PREVIEW16BITS(bstream, curword, leftpos)				\
{														        		\
	(curword) = ((curword) << 16) | ((uint32_t)(*(bstream)) << 8);		\
	(curword) |= (uint32_t)(*((bstream)+1));							\
	(leftpos) += 16;													\
	(bstream) += 2;												    	\
}

#define _PREVIEW24BITS(bstream, curword, leftpos)				\
{																		\
	(curword) = ((curword) << 8) | (uint32_t)(*(bstream));				\
	(curword) = ((curword) << 8) | (uint32_t)(*(bstream + 1));			\
	(curword) = ((curword) << 8) | (uint32_t)(*(bstream + 2));			\
	(leftpos) += 24;													\
	(bstream) += 3;													    \
}

#define _PREVIEW32BITS(bstream, curword, leftpos)				\
{																		\
	(curword) = ((curword) << 8) | (uint32_t)(*(bstream));				\
	(curword) = ((curword) << 8) | (uint32_t)(*(bstream + 1));			\
	(curword) = ((curword) << 8) | (uint32_t)(*(bstream + 2));			\
	(curword) = ((curword) << 8) | (uint32_t)(*(bstream + 3));          \
	(leftpos) += 32;													\
	(bstream) += 4;													    \
}

#define _NUMBITSTOBYTEALIGN(leftpos,nbit){						\
	(nbit) = (leftpos) & (7);                                           \
	(nbit) = ((nbit) == 0) ? (8) : (nbit);								\
}

#define _BITSTREAMBYTEALIGN(leftpos){							\
	(leftpos) = (leftpos) - ((leftpos) & (7));								\
}
/*
static __inline void
BitstreamByteAlign(Bitstream * const bs)
{
	uint32_t remainder = bs->pos & 7;

	if (remainder) {
		bs->pos -= remainder;
	}
}
*/

/*
static void BitstreamInit(Bitstream * const bs,
			  void *const bitstream,
			  uint32_t length)
{
	bs->tail = bs->start = (int8 *)bitstream;
	bs->buf = ((*bs->tail)<<24)|((*(bs->tail+1))<<16)|((*(bs->tail+2))<<8)|(*(bs->tail+3));
	bs->tail += 4;
	bs->pos = 32;
	bs->length = length;
}
*/
/* move forward to the next byte boundary */

static __inline void
BitstreamByteAlign(Bitstream * const bs)
{
	uint32_t remainder = bs->pos & 7;

	if (remainder) {
		bs->pos -= remainder;
	}
}


/* bitstream length (unit bits) */

static uint32_t __inline
BitstreamPos(const Bitstream * const bs)
{
	return((uint32_t)((((ptr_t)bs->tail - (ptr_t)bs->start)<<3) - bs->pos));
}
#endif
#endif

