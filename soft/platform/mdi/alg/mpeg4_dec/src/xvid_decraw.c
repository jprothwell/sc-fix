/*****************************************************************************
 *
 *  XVID MPEG-4 VIDEO CODEC
 *  - Console based decoding test application  -
 *
 *  Copyright(C) 2002-2003 Christoph Lampert
 *               2002-2003 Edouard Gomez <ed.gomez@free.fr>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * $Id: xvid_decraw.c,v 1.22.2.1 2006/07/10 15:19:41 Isibaar Exp $
 *
 ****************************************************************************/

/*****************************************************************************
 *		                    
 *  Application notes :
 *		                    
 *  An MPEG-4 bitstream is read from an input file (or stdin) and decoded,
 *  the speed for this is measured.
 *
 *  The program is plain C and needs no libraries except for libxvidcore, 
 *  and maths-lib.
 *		                   
 *  Use ./xvid_decraw -help for a list of options
 * 
 ****************************************************************************/
#ifndef SHEEN_VC_DEBUG 
#include "global.h"
#endif
 #ifdef MP4_3GP_SUPPORT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <math.h>
#ifndef WIN32
//#include <sys/time.h>
#else
#include <time.h>
#endif

//#include "xvid.h"
#include "image.h"
#include "mpeg4_global.h"
#include "xvid_decraw.h"
#include "decoder.h"
/*****************************************************************************
 *               Global vars in module and constants
 ****************************************************************************/

//#define USE_PNM 0
//#define USE_TGA 1

static int XDIM = 0;
static int YDIM = 0;
//static int ARG_SAVEDECOUTPUT = 0;
//static int ARG_SAVEMPEGSTREAM = 0;
//static char *ARG_INPUTFILE = NULL;
//static char *ARG_OUTPUTFILE = NULL;
//static int CSP = XVID_CSP_I420;
//static int BPP = 1;
//static int FORMAT = USE_PNM;

//static char filepath[256] = "./";
static void *dec_handle = NULL;

//#define BUFFER_SIZE (2*176*144)

//static const int display_buffer_bytes = 0;

//#define MIN_USEFUL_BYTES 1
//static int num = 0;
//FILE * f;
//#define uint16 unsigned int
//unsigned char out_buffer[50688] = {0};//QCIF-RGB565: 176*144*2
//#define MP4_SIZE (599338)
//unsigned char dongwl_mp4[]=
//{	
//	#include "carphone_ion.tab"	
//};
//int dwl_display=0;
//extern int frame_number;//test by longyl
/*****************************************************************************
 *               Local prototypes
 ****************************************************************************/

// double msecond();
/*
static int dec_init(int use_assembler, int debug_level);
static int dec_main(unsigned char *istream,
					unsigned char *ostream,
					int istream_size,
					xvid_dec_stats_t *xvid_dec_stats);
static int dec_stop();
*/
//static void usage();
//static int write_image(int8 *prefix, uint8 *image);
//static int write_pnm(int8 *filename, uint8 *image);
//static int write_tga(int8 *filename, uint8*image);

const int8 * type2str(int type)
{
    if (type==XVID_TYPE_IVOP)
        return "I";
    if (type==XVID_TYPE_PVOP)
        return "P";
    if (type==XVID_TYPE_BVOP)
        return "B";
    return "S";
}

/*****************************************************************************
 *        Main program
 ****************************************************************************/
#ifdef SHEEN_VC_DEBUG

#define INBUF_SIZE (40*1024) //4096

void main(void)
{
	const char *outrecfilename = "F:\\测试文件\\test3.yuv";
	const char *filename = "F:\\测试文件\\test.m4v";
	int frame, size, got_picture;
	FILE *fin, *fout;
	uint8_t inbuf[INBUF_SIZE ], *inbuf_ptr;
	int v_YuvInfo[5];
	int32 used_bytes;
	xvid_dec_stats_t xvid_dec_stats;
	char *pZoom;
	int i;
	int h;

	
	fin = fopen(filename, "rb");
	if (!fin)
	{
		fprintf(stderr, "could not open %s\n", filename);
		exit(1);
	}
	
	fout = fopen(outrecfilename, "wb");
	if (!fin) 
	{
		fprintf(stderr, "could not open %s\n", outrecfilename);
		exit(1);
	}

	dec_init(0, 0);

	size=0;
	used_bytes=0;
	i=0;
	while (1)
	{
		size += fread(inbuf+size, 1, INBUF_SIZE-size, fin);
	
		inbuf_ptr = inbuf;
		used_bytes=dec_main(inbuf_ptr,   
			v_YuvInfo, 
			size, &xvid_dec_stats, pZoom,
			0);
		if(used_bytes>0 && xvid_dec_stats.type>0)
		{
			for(h=0;h< YDIM;h++)
				fwrite(v_YuvInfo[0]+v_YuvInfo[3]*h, 1, XDIM, fout );
			for(h=0;h<YDIM/2;h++)
				fwrite(v_YuvInfo[1]+v_YuvInfo[3]*h/2, 1, XDIM/2, fout );
			for(h=0;h<YDIM/2;h++)
				fwrite(v_YuvInfo[2]+v_YuvInfo[3]*h/2, 1, XDIM/2, fout );
		}

		if(used_bytes>0)
		{
			size -= used_bytes;
			inbuf_ptr += used_bytes;
			memmove(inbuf, inbuf_ptr, size);
		}

		i++;
		if(i>500)break;
	}

	dec_stop();
	fclose(fin);
	fclose(fout);

}
#endif

/*****************************************************************************
 * Routines for decoding: init decoder, use, and stop decoder
 ****************************************************************************/

/* init decoder before first run */
 int
dec_init(int use_assembler, int debug_level)
{
	int ret;
	xvid_gbl_init_t   xvid_gbl_init;
	xvid_dec_create_t xvid_dec_create;

	/* Reset the structure with zeros */
	memset(&xvid_gbl_init, 0, sizeof(xvid_gbl_init_t));
	memset(&xvid_dec_create, 0, sizeof(xvid_dec_create_t));
	dec_handle=NULL;
	XDIM=0;
	YDIM=0;

	/*------------------------------------------------------------------------
	 * XviD core initialization
	 *----------------------------------------------------------------------*/

	/* Version */
	xvid_gbl_init.version = XVID_VERSION;

	xvid_gbl_init.cpu_flags = XVID_CPU_FORCE;

	xvid_gbl_init.debug = debug_level;

	xvid_global(NULL, 0, &xvid_gbl_init, NULL);

	/*------------------------------------------------------------------------
	 * XviD encoder initialization
	 *----------------------------------------------------------------------*/

	/* Version */
	xvid_dec_create.version = XVID_VERSION;

	/*
	 * Image dimensions -- set to 0, xvidcore will resize when ever it is
	 * needed
	 */
	xvid_dec_create.width = 0;
	xvid_dec_create.height = 0;

	ret = xvid_decore(NULL, XVID_DEC_CREATE, &xvid_dec_create, NULL, 0);
	if(ret!=0) return ret;
	
	dec_handle = xvid_dec_create.handle;
	return(ret);
}

/* decode one frame  */

/* decode one frame  */
 int dec_main(uint8 *istream,
		 uint8 *ostream,
		 int istream_size,
		 xvid_dec_stats_t *xvid_dec_stats, zoomPar *pZoom, bool isAviFile)
{
	int length = 0;
	xvid_dec_frame_t xvid_dec_frame;

	/* Reset all structures */
	memset(&xvid_dec_frame, 0, sizeof(xvid_dec_frame_t));
	/* Set version */
	xvid_dec_frame.version = XVID_VERSION;
	/* No general flags to set */
	xvid_dec_frame.general	= 0;

//repeat:
	/* Input stream */
	xvid_dec_frame.bitstream	= (void*)istream;
	xvid_dec_frame.length	= istream ? istream_size : -1;

	/* Output frame structure */
	xvid_dec_frame.output.plane[0]	= ostream;
	xvid_dec_frame.output.stride[0]	= XDIM;
//	xvid_dec_frame.output.csp = CSP;

	if( xvid_dec_stats )
	{
		memset(xvid_dec_stats, 0, sizeof(xvid_dec_stats_t));
		xvid_dec_stats->version = XVID_VERSION;
	}

	length = xvid_decore(dec_handle, XVID_DEC_DECODE, &xvid_dec_frame, xvid_dec_stats, pZoom);

	if( length == XVID_ERR_FRAME )
		return -2;

	if( isAviFile == TRUE )
	{
		if( length < 0 || (xvid_dec_stats->type == XVID_TYPE_NOTHING && length > 0) )
			return -1;
	}

	if( xvid_dec_stats->type == XVID_TYPE_VOL )
	{
		if( XDIM * YDIM <= xvid_dec_stats->data.vol.width * xvid_dec_stats->data.vol.height ) 
		{
			/* Copy new witdh and new height from the vol structure */
			XDIM = xvid_dec_stats->data.vol.width;
			YDIM = xvid_dec_stats->data.vol.height;
		}
		/*
		if( isAviFile == TRUE )
		{
			istream += length;
			istream_size -= length;

			if( istream_size >= 0 )
				goto repeat;
		}*/
	}
	return length;
}

/* close decoder to release resources */
 int
dec_stop()
{
	int ret;
	ret=0;
	if(dec_handle)
	{
		ret = xvid_decore(dec_handle, XVID_DEC_DESTROY, NULL, NULL, 0);
		dec_handle=NULL;
	}
	
	return(ret);
}

/*
get current end position of y buffer  for reuse outside.
reuse y buffer from end of .
call before dec_main.
y buffer will be swap in dec_main.
*/
char * dec_get_cur_y_end()
{
	DECODER *phdl=(DECODER *)dec_handle;

	if(!phdl)return 0;
	if(!phdl->cur.y)return 0;
	
	return phdl->cur.y+phdl->edged_width*(phdl->edged_height-EDGE_SIZE)-EDGE_SIZE; 
}
#endif

