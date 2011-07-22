/************************************************************************
 *
 *  config.h, configuration defines for tmndecode (H.263 decoder)
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
 * These software programs are available to the user without any
 * license fee or royalty on an "as is" basis. The University of
 * British Columbia disclaims any and all warranties, whether
 * express, implied, or statuary, including any implied warranties
 * or merchantability or of fitness for a particular purpose.  In no
 * event shall the copyright-holder be liable for any incidental,
 * punitive, or consequential damages of any kind whatsoever arising
 * from the use of these programs.
 *
 * This disclaimer of warranty extends to the user of these programs
 * and user's customers, employees, agents, transferees, successors,
 * and assigns.
 *
 * The University of British Columbia does not represent or warrant
 * that the programs furnished hereunder are free of infringement of
 * any third-party patents.
 *
 * Commercial implementations of H.263, including shareware, are
 * subject to royalty fees to patent holders.  Many of these patents
 * are general enough such that they are unavoidable regardless of
 * implementation design.
 *
*/



/* define NON_ANSI_COMPILER for compilers without function prototyping */
/* #define NON_ANSI_COMPILER */

#ifndef H263_CONFIG_H
#define H263_CONFIG_H

#include "cs_types.h"
#include "string.h"
#include "hal_debug.h"

#ifdef NON_ANSI_COMPILER
#define _ANSI_ARGS_(x) ()
#else
#define _ANSI_ARGS_(x) x
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

/************
internal ram start address
************/
//#define PROFILE_DEBUG

//#define USE_INTERNAL_SRAM

#ifdef USE_INTERNAL_SRAM
/*
	internal SRAM
	0x80c00000 - 0x80c0ffff  total 64kB SRAM
	0x80c00000- 0x80c068b0 be used 26kB
	0x80c0fdb0 -0x80c0ffff be used 592 bytes
	0x80c068b0-0x80c0fdaf can be use 38143 bytes

	new position
	start: 0x80c06920
	end: 0x80c0eefc
*/
#define START_OFFSET 0x70

#define YUV2RGB_START 0x80c068b0+START_OFFSET //size 2048*3 bytes
#define RESAMPLE_START 0x80c080b0+START_OFFSET //size uncertain (128x96 image=1408 bytes)

#define IDCTCLIP_START 0x80c08bb0+START_OFFSET  //size 2048 bytes
#define CLP_START 0x80c093b0+START_OFFSET //size 1024 bytes

#define DCT3DTAB0_START 0x80c097b0+START_OFFSET // size 448 bytes
#define DCT3DTAB1_START 0x80c09970+START_OFFSET // size 384 bytes
#define DCT3DTAB2_START 0x80c09af0+START_OFFSET // size 480 bytes
#define INTRA_DCT3DTAB0_START 0x80c09cd0+START_OFFSET // size 448 bytes
#define INTRA_DCT3DTAB1_START 0x80c09e90+START_OFFSET // size 384 bytes
#define INTRA_DCT3DTAB2_START 0x80c0a010+START_OFFSET // size 480 bytes

#define STORE_CODE_START 0x80c0a1f0+START_OFFSET // size 1536 bytes
#define ZIGZAG_SCAN_START 0x80c0a7f0+START_OFFSET // size 64 bytes
#define MQ_CHROMA_QP_TABLE_START 0x80c0a830+START_OFFSET // size 32 bytes
#define INBFR_START 0x80c0a850+START_OFFSET // size 16 bytes
#define BLOCK_START 0x80c0a860+START_OFFSET // size 768 bytes . blk_cnt=6
#define BLOCK_TEMP_START 0x80c0ab60+START_OFFSET // size 128 bytes .  for temp block

#if 0
#define MCBPCTABINTRA_START 0x80c0abe0 // size 128 bytes 
#define CBPYTAB_START 0x80c0aca0 // size 192 bytes 

#define TMNMVTAB0_START 0x80c0ad60 // size 56 bytes 
#define TMNMVTAB1_START 0x80c0ad98 // size 384 bytes 
#define TMNMVTAB2_START 0x80c0af18 // size 492 bytes 

#define MCBPCTAB0_START 0x80c0b104 // size 1024 bytes 
#define MCBPCTAB1_START 0x80c0b504 // size 32 bytes 
#endif

#endif

#endif 
