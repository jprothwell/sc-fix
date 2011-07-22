/*****************************************************************************
 *
 *  XVID MPEG-4 VIDEO CODEC
 *  - Decoder related header -
 *
 *  Copyright(C) 2002-2003 Peter Ross <pross@xvid.org>
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
 * $Id: decoder.h,v 1.17.2.1 2006/07/10 15:05:30 Isibaar Exp $
 *
 ****************************************************************************/
 
#ifndef _DECODER_H_
#define _DECODER_H_

#include "xvid.h"
#include "portab.h"
#include "mpeg4_global.h"
#include "image.h"
//#include "postprocessing.h"

/*****************************************************************************
 * Structures
 ****************************************************************************/

typedef struct
{
	/* vol bitstream */

	int16_t time_inc_resolution;
	int16_t fixed_time_inc;
	uint32_t time_inc_bits;

	int16_t shape;
	int16_t ver_id;
	int16_t mode;
	int16_t quant_bits;
	int16_t quant_type;

	int16_t aspect_ratio;
	int16_t par_width;
	int16_t par_height;
	int16_t bs_version;

	/* image */

	int16_t fixed_dimensions;
	int16_t width;
	int16_t height;
	int16_t edged_width;
	int16_t edged_height;
	IMAGE cur;
	IMAGE refn[1];				/* 0   -- last I or P VOP */
								/* 1   -- first I or P */
	IMAGE tmp;		// bframe interpolation, and post processing tmp buffer 

	/* macroblock */
	uint16_t mb_width;
	uint16_t mb_height;
    int16_t last_coding_type;           /* last coding type value */
	int32_t frames;				/* total frame number */
	VECTOR p_fmv;		/* pred forward & backward motion vector */
	int32_t time;				/* for record time */
	int32_t time_base;
	uint16_t low_delay;			/* low_delay flage (1 means no B_VOP) */
	uint16_t low_delay_default;	/* default value for low_delay flag */

	xvid_image_t* out_frm;                /* This is used for slice rendering */
	/* Tells if the reference image is edged or not */
	int16_t is_edged[1];
	VECTOR *mv_buffer_base;
	VECTOR *mv_buffer;
	VECTOR mvs[4];

	int16_t *ac_pred_base;
	int16_t *ac_pred_above_y;
	int16_t *ac_pred_left_y;
	int16_t *ac_pred_above_u;
	int16_t *ac_pred_left_u;
	int16_t *ac_pred_above_v;
	int16_t *ac_pred_left_v;

	int16_t *qscale;
	int16_t *qscale_current;
	int16_t *qscale_above;
	int16_t *qscale_left;

	int16_t *block;
	int16_t *data;
	int16_t block_size;
	uint16_t stride[6];
	uint8_t *y_row_start;
	uint8_t *u_row_start;
	uint8_t *v_row_start;
}
DECODER;
/*****************************************************************************
 * Decoder prototypes
 ****************************************************************************/

void init_decoder(uint32_t cpu_flags);

int decoder_create(xvid_dec_create_t * param);
int decoder_destroy(DECODER * dec);
int decoder_decode(DECODER * dec,
				   xvid_dec_frame_t * frame, xvid_dec_stats_t * stats);

typedef void (DECMBINTER) (DECODER *dec,VECTOR *pMVBuffPred[3],uint8_t **block,int16_t x,int16_t y,uint16_t cbp,
						Bitstream *bs,int fcode,int rounding);
typedef DECMBINTER *DECMBINTER_PTR;

void dec_mb_inter1v(DECODER *dec,VECTOR *pMVBuffPred[3],uint8_t **block,int16_t x,int16_t y,uint16_t cbp,
						Bitstream *bs,int fcode,int rounding);
void dec_mb_inter4v(DECODER *dec,VECTOR *pMVBuffPred[3],uint8_t **block,int16_t x,int16_t y,uint16_t cbp,
						Bitstream *bs,int fcode,int rounding);
#endif

