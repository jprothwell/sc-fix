/*****************************************************************************
 *
 *  XVID MPEG-4 VIDEO CODEC
 *  - Prediction header -
 *
 *  Copyright(C) 2002-2003 xvid team <xvid-devel@xvid.org>
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
 * $Id: mbprediction.h,v 1.25 2005/09/13 12:12:15 suxen_drol Exp $
 *
 ****************************************************************************/
#ifndef SHEEN_VC_DEBUG 
#include "global.h"
#endif
#ifdef MP4_3GP_SUPPORT

#ifndef _MBPREDICTION_H_
#define _MBPREDICTION_H_

#include "portab.h"
#include "decoder.h"
#include "mpeg4_global.h"

//#define MIN(X, Y) ((X)<(Y)?(X):(Y))
//#define MAX(X, Y) ((X)>(Y)?(X):(Y))

VECTOR
get_pmv2(uint16_t mb_width,VECTOR *pMVBuffPred[3],const int block);

#endif							/* _MBPREDICTION_H_ */
#endif

