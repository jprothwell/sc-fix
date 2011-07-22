/*****************************************************************************
 *
 *  XVID MPEG-4 VIDEO CODEC
 *  - MPEG4 Quantization H263 implementation -
 *
 *  Copyright(C) 2001-2003 Peter Ross <pross@xvid.org>
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
 * $Id: quant_h263.c,v 1.8 2004/03/22 22:36:24 edgomez Exp $
 *
 ****************************************************************************/


#include "mpeg4_global.h"
#include "quant.h"

/*****************************************************************************
 * Global function pointers
 ****************************************************************************/

/* DeQuant */
quant_intraFuncPtr dequant_h263_intra;
quant_interFuncPtr dequant_h263_inter;

/*****************************************************************************
 * Function definitions
 ****************************************************************************/

/*	dequantize intra-block & clamp to [-2048,2047]
 */

uint32_t
dequant_h263_intra_c(int16_t * data,
					 const int16_t * coeff,
					 const uint16_t quant,
					 const uint8_t dcscalar)
{
	const int32_t quant_m_2 = quant << 1;
	const int32_t quant_add = (quant & 1 ? quant : quant - 1);
	int i;

	data[0] = coeff[0] * dcscalar;
//	data[0] = data[0] + (((data[0]+2048) >> 15)&(-2048 - data[0]));
//	data[0] = 2047 + (((data[0]-2047) >> 15)&(data[0] - 2047));
	if (data[0] < -2048) {
		data[0] = -2048;
	} else if (data[0] > 2047) {
		data[0] = 2047;
	}

	for (i = 1; i < 64; i++) {
		int32_t acLevel = coeff[i];

		if (acLevel == 0) {
			data[i] = 0;
		} else if (acLevel < 0) {
			acLevel = quant_m_2 * -acLevel + quant_add;
			data[i] = (acLevel <= 2048 ? -acLevel : -2048);
		} else {
			acLevel = quant_m_2 * acLevel + quant_add;
			data[i] = (acLevel <= 2047 ? acLevel : 2047);
		}
	}

	return(0);
}



/* dequantize inter-block & clamp to [-2048,2047]
 */

uint32_t
dequant_h263_inter_c(int16_t * data,
					 const int16_t * coeff,
					 const uint16_t quant)
{
	const uint16_t quant_m_2 = quant << 1;
	const uint16_t quant_add = (quant & 1 ? quant : quant - 1);
	int i;

	for (i = 0; i < 64; i++) {
		int16_t acLevel = coeff[i];

		if (acLevel == 0) {
			data[i] = 0;
		} else if (acLevel < 0) {
			acLevel = acLevel * quant_m_2 - quant_add;
			data[i] = (acLevel >= -2048 ? acLevel : -2048);
		} else {
			acLevel = acLevel * quant_m_2 + quant_add;
			data[i] = (acLevel <= 2047 ? acLevel : 2047);
		}
	}

	return(0);
}

