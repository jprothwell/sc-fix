/*****************************************************************************
 *
 *  XVID MPEG-4 VIDEO CODEC
 *  - Image related header  -
 *
 *  Copyright(C) 2001-2004 Peter Ross <pross@xvid.org>
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
 * $Id: image.h,v 1.15 2005/09/09 12:18:10 suxen_drol Exp $
 *
 ****************************************************************************/
#ifndef SHEEN_VC_DEBUG 
#include "global.h"
#endif

 #ifdef MP4_3GP_SUPPORT

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdlib.h>

#include "portab.h"
#include "mpeg4_global.h"
//#include "colorspace.h"
#include "xvid.h"

#define EDGE_SIZE  16
//#define SAFETY	64
#define EDGE_SIZE2  (EDGE_SIZE>>1)

void init_image(uint32_t cpu_flags);


static void __inline
image_null(IMAGE * image)
{
	image->y = image->u = image->v = NULL;
}

int32_t image_create(IMAGE * image,
					 int16_t edged_width,
					 int16_t edged_height);

void image_destroy(IMAGE * image,
				   int16_t edged_width,
				   int16_t edged_height);

void image_swap(IMAGE * image1,
				IMAGE * image2);

void image_copy(IMAGE * image1,
				IMAGE * image2,
				uint32_t edged_width,
				uint32_t height);

void image_setedges(IMAGE * image,
					uint32_t edged_width,
					uint32_t edged_height,
					uint32_t width,
					uint32_t height,
					int bs_version);

int image_output(IMAGE * image,
				 uint32_t width,
				 uint32_t height,
				 uint32_t edged_width,
				 uint8_t * dst[4],
				 zoomPar *pZoom);

void
image_clear(IMAGE * img, int width, int height, int edged_width,
					int y, int u, int v);

#endif							/* _IMAGE_H_ */
#endif

