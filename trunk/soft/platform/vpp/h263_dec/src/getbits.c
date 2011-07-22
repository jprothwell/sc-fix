/************************************************************************
 *
 *  getbits.c, bit level routines for tmndecode (H.263 decoder)
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


//#include <stdlib.h>

#include "h263_config.h"
#include "tmndec.h"
#include "h263_global.h"
#include "sxr_ops.h"
#include "malloc.h"

//#include "mpeg4_bitstream.h"//for msk[33]
//#ifdef WIN32
//#include <io.h>
//#endif


/* to mask the n least significant bits of an integer */
#if 0
static unsigned int msk[33] =
{
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
#endif

/* initialize buffer, call once before first getbits or showbits */

void initbits (unsigned char *pInbuf)
{
    //int i;
    errorstate=0;
    ld=&base;
    ld->incnt = 0;
    ld->rdbfr=pInbuf;//add shenh
    //ld->rdptr = ld->rdbfr + 2048; shenh change
    ld->rdptr = ld->rdbfr ;
    ld->bitcnt = 0;
    
#ifdef USE_INTERNAL_SRAM
    ld->inbfr=(unsigned char*)INBFR_START;
    ld->block=(short*)BLOCK_START;
    ld->block_temp=(short*)BLOCK_TEMP_START;
#else
    ld->block=NULL;
    ld->block=(short*)h263_myMalloc(sizeof(short)*64*13);//13 for temp block
   
    if(!(ld->block))
    {
        //exit(-1);
        errorstate=-1;
        return;
    }
    /*
      {
      int i;
      for(i=0;i<12;i++)
      ld->block[i]=ld->block[0]+i*64;
      }
    */  
    //ld->block_temp=ld->block[0]+12*64;
    ld->block_temp=ld->block+12*64;
#endif

}


void resetbits (unsigned char *pInbuf, unsigned int datalen)//add shenh
{
    int i;
    ld->rdptr = ld->rdbfr=pInbuf ;
    ld->datalen=datalen;

    ld->incnt = 0;
    ld->bitcnt = 0;
    for(i=0;i<12;i++)
        ld->inbfr[i]=0;
}


void fillbfr ()
{
    int l;
#ifdef  PROFILE_DEBUG
    hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_HAL, 0x29b6);
#endif

#if 1
    ld->inbfr[0] = ld->inbfr[8];
    ld->inbfr[1] = ld->inbfr[9];
    ld->inbfr[2] = ld->inbfr[10];
    ld->inbfr[3] = ld->inbfr[11];
#else
    memcpy(ld->inbfr, ld->inbfr+8, 4);
#endif

    /* shenh change
       if (ld->rdptr >= ld->rdbfr + 2048)
       {
       l = read (ld->infile, ld->rdbfr, 2048);
       ld->rdptr = ld->rdbfr;
       if (l < 2048)
       {
       if (l < 0)
       l = 0;

       while (l < 2048)          // Add recognizable sequence end code 
       {
       ld->rdbfr[l++] = 0;
       ld->rdbfr[l++] = 0;
       ld->rdbfr[l++] = (1 << 7) | (SE_CODE << 2);
       }
       }
       }
    */
  
#if 0
    if (ld->rdptr+8 <= ld->rdbfr + ld->datalen)
    {
    
        for (l = 0; l < 8; l++)
            ld->inbfr[l + 4] = ld->rdptr[l];
    
        /*
          ld->inbfr[4] = ld->rdptr[0];
          ld->inbfr[5] = ld->rdptr[1];
          ld->inbfr[6] = ld->rdptr[2];
          ld->inbfr[7] = ld->rdptr[3];
          ld->inbfr[8] = ld->rdptr[4];
          ld->inbfr[9] = ld->rdptr[5];
          ld->inbfr[10] = ld->rdptr[6];
          ld->inbfr[11] = ld->rdptr[7];
        */
        ld->rdptr += 8;
        ld->incnt += 64;
    }
    else
    {
        for (l = 0; l < 8; l++)
        {
            if(ld->rdptr < ld->rdbfr + ld->datalen)
                ld->inbfr[l + 4] = ld->rdptr[l];
            else if(l <=5)
            {
                ld->inbfr[l + 4] = 0;
                l++;
                ld->inbfr[l + 4] = 0;
                l++;
                ld->inbfr[l + 4] =  (1 << 7) | (SE_CODE << 2);
            }
            else
                ld->inbfr[l + 4] = 0;

            // ld->rdptr += 1;
            //  ld->incnt += 8;
        }
        ld->rdptr += 8;
        ld->incnt += 64;
    }
#else
    if (ld->rdptr+8 <= ld->rdbfr + ld->datalen)
    {
        /*
          for (l = 0; l < 8; l++)
          ld->inbfr[l + 4] = ld->rdptr[l];
        */
        memcpy(ld->inbfr+ 4,  ld->rdptr, 8);

        ld->rdptr += 8;
        ld->incnt += 64;
    }
    else
    {
        for (l = 0; l < 8; l++)
        {
            if(ld->rdptr < ld->rdbfr + ld->datalen)
                ld->inbfr[l + 4] = ld->rdptr[l];
            else if(l <=5)
            {
                ld->inbfr[l + 4] = 0;
                l++;
                ld->inbfr[l + 4] = 0;
                l++;
                ld->inbfr[l + 4] =  (1 << 7) | (SE_CODE << 2);
            }
            else
                ld->inbfr[l + 4] = 0;

            // ld->rdptr += 1;
            //  ld->incnt += 8;
        }
        ld->rdptr += 8;
        ld->incnt += 64;
    }
#endif
#ifdef  PROFILE_DEBUG
    hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_HAL, 0x29b6);
#endif
}


/* return next n bits (right adjusted) without advancing */

unsigned int showbits (int n)
{
    unsigned char *v;
    unsigned int b=0;
    //int c;
    int movR, rBit0;

    if (ld->incnt < n)
        fillbfr ();
#ifdef  PROFILE_DEBUG
    hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_HAL, 0x29b7);
#endif
#if 0
    v = ld->inbfr + ((96 - ld->incnt) >> 3);
    b = (v[0] << 24) | (v[1] << 16) | (v[2] << 8) | v[3];
    c = ((ld->incnt - 1) & 7) + 25;
//  res= (b >> (c - n)) & msk[n];
    movR=32-n;
    b= (b << (32-c))>>movR;
#else

    movR=32-n;
    v = ld->inbfr + ((96 - ld->incnt) >> 3);
    rBit0=31-((ld->incnt-1) &7);
    b |= ((*v++) << rBit0);
    while(rBit0>movR)
    {
        rBit0-=8;
        //if(rBit0>0) maybe need. shenh
        b |= ((*v++) << rBit0);    
    }
    b>>=movR;
#endif

#ifdef  PROFILE_DEBUG
    hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_HAL, 0x29b7);
#endif
  
    return b;
}


/* return next bit (could be made faster than getbits(1)) */

unsigned int getbits1 ()
{
    return getbits (1);
}


/* advance by n bits */

void flushbits (int n)
{

    ld->bitcnt += n;
    ld->incnt -= n;
    if (ld->incnt < 0)
        fillbfr ();
}


/* return next n bits (right adjusted) */

unsigned int getbits (int n)
{
    unsigned int l;

    l = showbits (n);
    flushbits (n);

    return l;
}
