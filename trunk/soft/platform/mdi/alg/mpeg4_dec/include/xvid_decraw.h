#ifndef SHEEN_VC_DEBUG 
#include "global.h"
#endif

#ifdef MP4_3GP_SUPPORT

#ifndef _XVID_DECRAW_H_
#define _XVID_DECRAW_H_

#include "xvid.h"

#define MIN_USEFUL_BYTES 1
 int dec_init(int use_assembler, int debug_level);
 int dec_main(uint8 *istream,
					uint8 *ostream,
					int istream_size,
					xvid_dec_stats_t *xvid_dec_stats,
					zoomPar *pZoom,
					bool isAviFile);
 int dec_stop();
 char * dec_get_cur_y_end();


#endif
#endif

