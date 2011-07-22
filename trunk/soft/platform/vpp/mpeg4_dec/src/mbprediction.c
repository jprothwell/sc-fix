/*****************************************************************************
 *
 *  XVID MPEG-4 VIDEO CODEC
 *  - Prediction module -
 *
 *  Copyright (C) 2001-2003 Michael Militzer <isibaar@xvid.org>
 *                2001-2003 Peter Ross <pross@xvid.org>
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
 * $Id: mbprediction.c,v 1.18 2005/11/22 10:23:01 suxen_drol Exp $
 *
 ****************************************************************************/


#include <stdlib.h>

#include "mpeg4_global.h"
//#include "../encoder.h"
#include "mbprediction.h"
//#include "../utils/mbfunctions.h"
//#include "../bitstream/cbp.h"
#include "mbcoding.h"
#include "zigzag.h"

//const uint32_t roundtab[16] =
//{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2 };

/* K = 4 */
const uint32_t roundtab_76[16] =
{ 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1 };

/* K = 2 */
//const uint32_t roundtab_78[8] =
//{ 0, 0, 1, 1, 0, 0, 0, 1  };

/* K = 1 */
const uint32_t roundtab_79[4] =
{ 0, 1, 0, 0 };

VECTOR
get_pmv2(uint16_t mb_width,VECTOR **pMVBuffPred,const int block)
{
	VECTOR ret_mv;
	int32_t mv1_x=0,mv2_x=0,mv3_x=0,mv1_y=0,mv2_y=0,mv3_y=0;
	int num_cand = 0;	
	
	switch(block)
	{
		case 0:
			if(pMVBuffPred[0][1].x == 0x80000000)
			{
				mv1_x = mv1_y = 0;
			}else{
				mv1_x = pMVBuffPred[0][1].x;
				mv1_y = pMVBuffPred[0][1].y;
				num_cand ++;
			}
			if(pMVBuffPred[1][2].x == 0x80000000)
			{
				mv2_x = mv2_y = 0;
			}else{
				mv2_x = pMVBuffPred[1][2].x;
				mv2_y = pMVBuffPred[1][2].y;
				num_cand ++;
			}
			if(pMVBuffPred[2][2].x == 0x80000000)
			{
				mv3_x = mv3_y = 0;
			}else{
				mv3_x = pMVBuffPred[2][2].x;
				mv3_y = pMVBuffPred[2][2].y;
				num_cand ++;
			}
			if(num_cand < 2)
			{
				ret_mv.x = mv1_x + mv2_x + mv3_x;
				ret_mv.y = mv1_y + mv2_y + mv3_y;
				return ret_mv;
			}
			break;
		case 1:
			mv1_x = pMVBuffPred[0][0].x;
			mv1_y = pMVBuffPred[0][0].y;
			num_cand ++;
			if(pMVBuffPred[1][3].x == 0x80000000)
			{
				mv2_x = mv2_y = 0;
			}else{
				mv2_x = pMVBuffPred[1][3].x;
				mv2_y = pMVBuffPred[1][3].y;
				num_cand ++;
			}
			if(pMVBuffPred[2][2].x == 0x80000000)
			{
				mv3_x = mv3_y = 0;
			}else{
				mv3_x = pMVBuffPred[2][2].x;
				mv3_y = pMVBuffPred[2][2].y;
				num_cand ++;				
			}
			if(num_cand < 2)
			{
				ret_mv.x = mv1_x + mv2_x + mv3_x;
				ret_mv.y = mv1_y + mv2_y + mv3_y;
				return ret_mv;
			}
			break;
		case 2:
			if(pMVBuffPred[0][3].x == 0x80000000)
			{
				mv1_x = mv1_y = 0;
			}else{
				mv1_x = pMVBuffPred[0][3].x;
				mv1_y = pMVBuffPred[0][3].y;
			}
			mv2_x = pMVBuffPred[0][0].x;
			mv2_y = pMVBuffPred[0][0].y;
			mv3_x = pMVBuffPred[0][1].x;
			mv3_y = pMVBuffPred[0][1].y;
			break;
		case 3:
			mv1_x = pMVBuffPred[0][2].x;
			mv1_y = pMVBuffPred[0][2].y;

			mv2_x = pMVBuffPred[0][0].x;
			mv2_y = pMVBuffPred[0][0].y;

			mv3_x = pMVBuffPred[0][1].x;
			mv3_y = pMVBuffPred[0][1].y;
			break;
	}

	ret_mv.x = mv1_x + mv2_x + mv3_x - MAX_32(mv1_x, MAX_32(mv2_x, mv3_x)) - MIN_32(mv1_x, MIN_32(mv2_x, mv3_x));
	ret_mv.y = mv1_y + mv2_y + mv3_y - MAX_32(mv1_y, MAX_32(mv2_y, mv3_y)) - MIN_32(mv1_y, MIN_32(mv2_y, mv3_y));

	return ret_mv;
}


