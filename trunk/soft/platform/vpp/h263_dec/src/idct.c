/************************************************************************
 *
 *  idct.c, inverse fast DCT for tmndecode (H.263 decoder)
 *  Copyright (C) 1995, 1996  Telenor R&D, Norway
 *
 *  Contacts:
 *  Robert Danielsen                  <Robert.Danielsen@nta.no>
 *
 *  Telenor Research and Development  http://www.nta.no/brukere/DVC/
 *  P.O.Box 83                        tel.:   +47 63 84 84 00
 *  N-2007 Kjeller, Norway            fax.:   +47 63 81 00 76
 *
 *  Copyright (C) 1997  University of BC, Canada
 *  Modified by: Michael Gallant <mikeg@ee.ubc.ca>
 *               Guy Cote <guyc@ee.ubc.ca>
 *               Berna Erol <bernae@ee.ubc.ca>
 *
 *  Contacts:
 *  Michael Gallant                   <mikeg@ee.ubc.ca>
 *
 *  UBC Image Processing Laboratory   http://www.ee.ubc.ca/image
 *  2356 Main Mall                    tel.: +1 604 822 4051
 *  Vancouver BC Canada V6T1Z4        fax.: +1 604 822 5949
 *
 ************************************************************************/

/* Disclaimer of Warranty
 * 
 * These software programs are available to the user without any license fee
 * or royalty on an "as is" basis. The University of British Columbia
 * disclaims any and all warranties, whether express, implied, or
 * statuary, including any implied warranties or merchantability or of
 * fitness for a particular purpose.  In no event shall the
 * copyright-holder be liable for any incidental, punitive, or
 * consequential damages of any kind whatsoever arising from the use of
 * these programs.
 * 
 * This disclaimer of warranty extends to the user of these programs and
 * user's customers, employees, agents, transferees, successors, and
 * assigns.
 * 
 * The University of British Columbia does not represent or warrant that the
 * programs furnished hereunder are free of infringement of any
 * third-party patents.
 * 
 * Commercial implementations of H.263, including shareware, are subject to
 * royalty fees to patent holders.  Many of these patents are general
 * enough such that they are unavoidable regardless of implementation
 * design.
 * 
 */



/* based on mpeg2decode, (C) 1994, MPEG Software Simulation Group and
 * mpeg2play, (C) 1994 Stefan Eckart <stefan@lis.e-technik.tu-muenchen.de>
 * 
 */


/**********************************************************/
/* inverse two dimensional DCT, Chen-Wang algorithm       */
/* (cf. IEEE ASSP-32, pp. 803-816, Aug. 1984)             */
/* 32-bit integer arithmetic (8 bit coefficients)         */
/* 11 mults, 29 adds per DCT                              */
/* sE, 18.8.91       */
/**********************************************************/
/* coefficients extended to 12 bit for IEEE1180-1990      */
/* compliance                           sE,  2.1.94       */
/**********************************************************/

/* this code assumes >> to be a two's-complement arithmetic */
/* right shift: (-2)>>1 == -1 , (-3)>>1 == -2               */

#include "h263_config.h"
#include "h263_global.h"
#include "sxr_ops.h"
#include "malloc.h"


#define W1 2841                 /* 2048*sqrt(2)*cos(1*pi/16) */
#define W2 2676                 /* 2048*sqrt(2)*cos(2*pi/16) */
#define W3 2408                 /* 2048*sqrt(2)*cos(3*pi/16) */
#define W5 1609                 /* 2048*sqrt(2)*cos(5*pi/16) */
#define W6 1108                 /* 2048*sqrt(2)*cos(6*pi/16) */
#define W7 565                  /* 2048*sqrt(2)*cos(7*pi/16) */

/* global declarations */
int init_idct _ANSI_ARGS_ ((void));
void idct _ANSI_ARGS_ ((short *block));

static short *iclip;       /* clipping table */
/* private data */
//static short iclip[1024];       /* clipping table */ //change shenh
static short *iclp;

/* private prototypes */
static void idctrow _ANSI_ARGS_ ((short *blk));
static void idctcol _ANSI_ARGS_ ((short *blk));

#if 1
/* row (horizontal) IDCT
 * 
 * 7                       pi         1 dst[k] = sum c[l] * src[l] * cos( -- *
 * ( k + - ) * l ) l=0                      8          2
 * 
 * where: c[0]    = 128 c[1..7] = 128*sqrt(2) */


static void idctrow (short *blk)
{
    int x0, x1, x2, x3, x4, x5, x6, x7, x8;

    /* shortcut */
    if (!((x1 = blk[4] << 11) | (x2 = blk[6]) | (x3 = blk[2]) |
          (x4 = blk[1]) | (x5 = blk[7]) | (x6 = blk[5]) | (x7 = blk[3])))
    {
        blk[0] = blk[1] = blk[2] = blk[3] = blk[4] = blk[5] = blk[6] = blk[7] = blk[0] << 3;
        return;
    }
    x0 = (blk[0] << 11) + 128;    /* for proper rounding in the fourth stage */

    /* first stage */
    x8 = W7 * (x4 + x5);
    x4 = x8 + (W1 - W7) * x4;
    x5 = x8 - (W1 + W7) * x5;
    x8 = W3 * (x6 + x7);
    x6 = x8 - (W3 - W5) * x6;
    x7 = x8 - (W3 + W5) * x7;

    /* second stage */
    x8 = x0 + x1;
    x0 -= x1;
    x1 = W6 * (x3 + x2);
    x2 = x1 - (W2 + W6) * x2;
    x3 = x1 + (W2 - W6) * x3;
    x1 = x4 + x6;
    x4 -= x6;
    x6 = x5 + x7;
    x5 -= x7;

    /* third stage */
    x7 = x8 + x3;
    x8 -= x3;
    x3 = x0 + x2;
    x0 -= x2;
    x2 = (181 * (x4 + x5) + 128) >> 8;
    x4 = (181 * (x4 - x5) + 128) >> 8;

    /* fourth stage */
    blk[0] = (x7 + x1) >> 8;
    blk[1] = (x3 + x2) >> 8;
    blk[2] = (x0 + x4) >> 8;
    blk[3] = (x8 + x6) >> 8;
    blk[4] = (x8 - x6) >> 8;
    blk[5] = (x0 - x4) >> 8;
    blk[6] = (x3 - x2) >> 8;
    blk[7] = (x7 - x1) >> 8;
}

/* column (vertical) IDCT
 * 
 * 7                         pi         1 dst[8*k] = sum c[l] * src[8*l] *
 * cos( -- * ( k + - ) * l ) l=0                        8          2
 * 
 * where: c[0]    = 1/1024 c[1..7] = (1/1024)*sqrt(2) */
static void idctcol (short *blk)
{
    int x0, x1, x2, x3, x4, x5, x6, x7, x8;

    /* shortcut */
    if (!((x1 = (blk[32] << 8)) | (x2 = blk[48]) | (x3 = blk[16]) |
          (x4 = blk[8 ]) | (x5 = blk[56]) | (x6 = blk[40]) | (x7 = blk[24])))
    {
        blk[ 0] = blk[8 ] = blk[16] = blk[24] = blk[32] = blk[40] = blk[48] = blk[56] =
            iclp[(blk[ 0] + 32) >> 6];
        return;
    }
    x0 = (blk[ 0] << 8) + 8192;

    /* first stage */
    x8 = W7 * (x4 + x5) + 4;
    x4 = (x8 + (W1 - W7) * x4) >> 3;
    x5 = (x8 - (W1 + W7) * x5) >> 3;
    x8 = W3 * (x6 + x7) + 4;
    x6 = (x8 - (W3 - W5) * x6) >> 3;
    x7 = (x8 - (W3 + W5) * x7) >> 3;

    /* second stage */
    x8 = x0 + x1;
    x0 -= x1;
    x1 = W6 * (x3 + x2) + 4;
    x2 = (x1 - (W2 + W6) * x2) >> 3;
    x3 = (x1 + (W2 - W6) * x3) >> 3;
    x1 = x4 + x6;
    x4 -= x6;
    x6 = x5 + x7;
    x5 -= x7;

    /* third stage */
    x7 = x8 + x3;
    x8 -= x3;
    x3 = x0 + x2;
    x0 -= x2;
    x2 = (181 * (x4 + x5) + 128) >> 8;
    x4 = (181 * (x4 - x5) + 128) >> 8;

    /* fourth stage */
    blk[ 0] = iclp[(x7 + x1) >> 14];
    blk[8 ] = iclp[(x3 + x2) >> 14];
    blk[16] = iclp[(x0 + x4) >> 14];
    blk[24] = iclp[(x8 + x6) >> 14];
    blk[32] = iclp[(x8 - x6) >> 14];
    blk[40] = iclp[(x0 - x4) >> 14];
    blk[48] = iclp[(x3 - x2) >> 14];
    blk[56] = iclp[(x7 - x1) >> 14];
}

/* two dimensional inverse discrete cosine transform */
void idct (short *block)
{
    int i;

    for (i = 0; i < 8; i++)
        idctrow (block + 8 * i);

    for (i = 0; i < 8; i++)
        idctcol (block + i);
/*
  idctrow (block );
  idctrow (block + 8);
  idctrow (block + 16);
  idctrow (block + 24);
  idctrow (block + 32);
  idctrow (block + 40);
  idctrow (block + 48 );
  idctrow (block + 56);

  idctcol (block );
  idctcol (block + 1);
  idctcol (block + 2);
  idctcol (block + 3);
  idctcol (block + 4);
  idctcol (block + 5);
  idctcol (block + 6);
  idctcol (block + 7);
*/
}
#else

/* two dimensional inverse discrete cosine transform */
void idct (short *block)
{
/*
  int i;

  for (i = 0; i < 8; i++)
  idctrow (block + 8 * i);

  for (i = 0; i < 8; i++)
  idctcol (block + i);
*/
    int i;
    short *blkIn, *blkOut;
    int x0, x1, x2, x3, x4, x5, x6, x7, x8;
    short *blk;
#if 1
    /* row (horizontal) IDCT
     * 
     * 7                       pi         1 dst[k] = sum c[l] * src[l] * cos( -- *
     * ( k + - ) * l ) l=0                      8          2
     * 
     * where: c[0]    = 128 c[1..7] = 128*sqrt(2) */
    blkIn=block;
    blkOut=ld->block_temp;
    for(i=0;i<8;i++)
    {
        /* shortcut */
        if (!((x1 = blkIn[4] << 11) | (x2 = blkIn[6]) | (x3 = blkIn[2]) |
              (x4 = blkIn[1]) | (x5 = blkIn[7]) | (x6 = blkIn[5]) | (x7 = blkIn[3])))
        {
            blkOut[0] =blkOut[8] = blkOut[16] = blkOut[24] =blkOut[32] = blkOut[40] = blkOut[48] = blkOut[56] = blkIn[0] << 3;
            
            blkIn+=8;
            blkOut++;
            continue;
        }
        x0 = (blkIn[0] << 11) + 128;    /* for proper rounding in the fourth stage */

        /* first stage */
        x8 = W7 * (x4 + x5);
        x4 = x8 + (W1 - W7) * x4;
        x5 = x8 - (W1 + W7) * x5;
        x8 = W3 * (x6 + x7);
        x6 = x8 - (W3 - W5) * x6;
        x7 = x8 - (W3 + W5) * x7;

        /* second stage */
        x8 = x0 + x1;
        x0 -= x1;
        x1 = W6 * (x3 + x2);
        x2 = x1 - (W2 + W6) * x2;
        x3 = x1 + (W2 - W6) * x3;
        x1 = x4 + x6;
        x4 -= x6;
        x6 = x5 + x7;
        x5 -= x7;

        /* third stage */
        x7 = x8 + x3;
        x8 -= x3;
        x3 = x0 + x2;
        x0 -= x2;
        x2 = (181 * (x4 + x5) + 128) >> 8;
        x4 = (181 * (x4 - x5) + 128) >> 8;

        /* fourth stage */
        blkOut[0] = (x7 + x1) >> 8;
        blkOut[8] = (x3 + x2) >> 8;
        blkOut[16] = (x0 + x4) >> 8;
        blkOut[24] = (x8 + x6) >> 8;
        blkOut[32] = (x8 - x6) >> 8;
        blkOut[40] = (x0 - x4) >> 8;
        blkOut[48] = (x3 - x2) >> 8;
        blkOut[56] = (x7 - x1) >> 8;

        blkIn+=8;
        blkOut++;
    }

    /* column (vertical) IDCT
     * 
     * 7                         pi         1 dst[8*k] = sum c[l] * src[8*l] *
     * cos( -- * ( k + - ) * l ) l=0                        8          2
     * 
     * where: c[0]    = 1/1024 c[1..7] = (1/1024)*sqrt(2) */
    blkIn=ld->block_temp;
    blkOut=block;
    for(i=0;i<8;i++)
    {
        /* shortcut */
        if (!((x1 = (blkIn[ 4] << 8)) | (x2 = blkIn[ 6]) | (x3 = blkIn[ 2]) |
              (x4 = blkIn[ 1]) | (x5 = blkIn[ 7]) | (x6 = blkIn[ 5]) | (x7 = blkIn[ 3])))
        {
            blkOut[ 0] = blkOut[ 8] = blkOut[ 16] = blkOut[ 24] = blkOut[ 32] = blkOut[ 40] = blkOut[ 48] = blkOut[ 56] =
                iclp[(blkIn[0] + 32) >> 6];
            
            blkIn+=8;
            blkOut++;
            continue;
        }
        x0 = (blkIn[ 0] << 8) + 8192;

        /* first stage */
        x8 = W7 * (x4 + x5) + 4;
        x4 = (x8 + (W1 - W7) * x4) >> 3;
        x5 = (x8 - (W1 + W7) * x5) >> 3;
        x8 = W3 * (x6 + x7) + 4;
        x6 = (x8 - (W3 - W5) * x6) >> 3;
        x7 = (x8 - (W3 + W5) * x7) >> 3;

        /* second stage */
        x8 = x0 + x1;
        x0 -= x1;
        x1 = W6 * (x3 + x2) + 4;
        x2 = (x1 - (W2 + W6) * x2) >> 3;
        x3 = (x1 + (W2 - W6) * x3) >> 3;
        x1 = x4 + x6;
        x4 -= x6;
        x6 = x5 + x7;
        x5 -= x7;

        /* third stage */
        x7 = x8 + x3;
        x8 -= x3;
        x3 = x0 + x2;
        x0 -= x2;
        x2 = (181 * (x4 + x5) + 128) >> 8;
        x4 = (181 * (x4 - x5) + 128) >> 8;

        /* fourth stage */
        blkOut[ 0] = iclp[(x7 + x1) >> 14];
        blkOut[ 8] = iclp[(x3 + x2) >> 14];
        blkOut[ 16] = iclp[(x0 + x4) >> 14];
        blkOut[ 24] = iclp[(x8 + x6) >> 14];
        blkOut[ 32] = iclp[(x8 - x6) >> 14];
        blkOut[ 40] = iclp[(x0 - x4) >> 14];
        blkOut[ 48] = iclp[(x3 - x2) >> 14];
        blkOut[ 56] = iclp[(x7 - x1) >> 14];
        
        blkIn+=8;
        blkOut++;
    }
#else
    /* row (horizontal) IDCT
     * 
     * 7                       pi         1 dst[k] = sum c[l] * src[l] * cos( -- *
     * ( k + - ) * l ) l=0                      8          2
     * 
     * where: c[0]    = 128 c[1..7] = 128*sqrt(2) */
    for(i=0;i<8;i++)
    {
        blk=block+(i<<3);
        /* shortcut */
        if (!((x1 = blk[4] << 11) | (x2 = blk[6]) | (x3 = blk[2]) |
              (x4 = blk[1]) | (x5 = blk[7]) | (x6 = blk[5]) | (x7 = blk[3])))
        {
            blk[0] = blk[1] = blk[2] = blk[3] = blk[4] = blk[5] = blk[6] = blk[7] = blk[0] << 3;
            continue;
        }
        x0 = (blk[0] << 11) + 128;    /* for proper rounding in the fourth stage */

        /* first stage */
        x8 = W7 * (x4 + x5);
        x4 = x8 + (W1 - W7) * x4;
        x5 = x8 - (W1 + W7) * x5;
        x8 = W3 * (x6 + x7);
        x6 = x8 - (W3 - W5) * x6;
        x7 = x8 - (W3 + W5) * x7;

        /* second stage */
        x8 = x0 + x1;
        x0 -= x1;
        x1 = W6 * (x3 + x2);
        x2 = x1 - (W2 + W6) * x2;
        x3 = x1 + (W2 - W6) * x3;
        x1 = x4 + x6;
        x4 -= x6;
        x6 = x5 + x7;
        x5 -= x7;

        /* third stage */
        x7 = x8 + x3;
        x8 -= x3;
        x3 = x0 + x2;
        x0 -= x2;
        x2 = (181 * (x4 + x5) + 128) >> 8;
        x4 = (181 * (x4 - x5) + 128) >> 8;

        /* fourth stage */
        blk[0] = (x7 + x1) >> 8;
        blk[1] = (x3 + x2) >> 8;
        blk[2] = (x0 + x4) >> 8;
        blk[3] = (x8 + x6) >> 8;
        blk[4] = (x8 - x6) >> 8;
        blk[5] = (x0 - x4) >> 8;
        blk[6] = (x3 - x2) >> 8;
        blk[7] = (x7 - x1) >> 8;
    }

    /* column (vertical) IDCT
     * 
     * 7                         pi         1 dst[8*k] = sum c[l] * src[8*l] *
     * cos( -- * ( k + - ) * l ) l=0                        8          2
     * 
     * where: c[0]    = 1/1024 c[1..7] = (1/1024)*sqrt(2) */
    for(i=0;i<8;i++)
    {
        blk=block+i;
        /* shortcut */
        if (!((x1 = (blk[32] << 8)) | (x2 = blk[48]) | (x3 = blk[16]) |
              (x4 = blk[8 ]) | (x5 = blk[56]) | (x6 = blk[40]) | (x7 = blk[24])))
        {
            blk[ 0] = blk[8 ] = blk[16] = blk[24] = blk[32] = blk[40] = blk[48] = blk[56] =
                iclp[(blk[ 0] + 32) >> 6];
            continue;
        }
        x0 = (blk[ 0] << 8) + 8192;

        /* first stage */
        x8 = W7 * (x4 + x5) + 4;
        x4 = (x8 + (W1 - W7) * x4) >> 3;
        x5 = (x8 - (W1 + W7) * x5) >> 3;
        x8 = W3 * (x6 + x7) + 4;
        x6 = (x8 - (W3 - W5) * x6) >> 3;
        x7 = (x8 - (W3 + W5) * x7) >> 3;

        /* second stage */
        x8 = x0 + x1;
        x0 -= x1;
        x1 = W6 * (x3 + x2) + 4;
        x2 = (x1 - (W2 + W6) * x2) >> 3;
        x3 = (x1 + (W2 - W6) * x3) >> 3;
        x1 = x4 + x6;
        x4 -= x6;
        x6 = x5 + x7;
        x5 -= x7;

        /* third stage */
        x7 = x8 + x3;
        x8 -= x3;
        x3 = x0 + x2;
        x0 -= x2;
        x2 = (181 * (x4 + x5) + 128) >> 8;
        x4 = (181 * (x4 - x5) + 128) >> 8;

        /* fourth stage */
        blk[ 0] = iclp[(x7 + x1) >> 14];
        blk[8 ] = iclp[(x3 + x2) >> 14];
        blk[16] = iclp[(x0 + x4) >> 14];
        blk[24] = iclp[(x8 + x6) >> 14];
        blk[32] = iclp[(x8 - x6) >> 14];
        blk[40] = iclp[(x0 - x4) >> 14];
        blk[48] = iclp[(x3 - x2) >> 14];
        blk[56] = iclp[(x7 - x1) >> 14];
    }

#endif
}
#endif

int init_idct ()
{
    int i;
#ifdef USE_INTERNAL_SRAM
    iclip=(short*)IDCTCLIP_START;//h263_myMalloc(sizeof(short)*1024);//add shenh
#else
    iclip=(short*)h263_myMalloc(sizeof(short)*1024);//add shenh
#endif
    if(!iclip)
    {
        //exit (-1);
        return -1;
    }
    iclp = iclip + 512;
    for (i = -512; i < 512; i++)
        iclp[i] = (i < -256) ? -256 : ((i > 255) ? 255 : i);
    return 0;
}
