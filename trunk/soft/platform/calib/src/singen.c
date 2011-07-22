/*************************************************************************/
/*                                                                       */
/*            Copyright (C) 2003-2005, Coolsand Technologies, Inc.       */
/*                            All Rights Reserved                        */
/*                                                                       */
/*  This source code is the property of Coolsand Technologies and is     */
/*  confidential. Any modification, distribution, reproduction or        */
/*  exploitation of any content of this file is totally forbidden,       */
/*  except with the written permission of Coolsand Technologies.         */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*                                                                       */
/*      singen.c                                                         */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      Generate a sine wave at a specified frequency and amplitude.     */
/*                                                                       */
/*      !!! COOLSAND CONFIDENTIAL !!!                                    */
/*                                                                       */
/*************************************************************************/

#include "cs_types.h"

#include "hal_sys.h"

#include "singen.h"
#include "sinref.h"



/* -------------------------------------------------- */
/* ----- Destination buffer. */

#define AMPL_1			255
#define FN				f / SINREF_F



// =============================================================================
// sinGen
// -----------------------------------------------------------------------------
/// Generate a sine wave
/// @param f Frequency
/// @param ampl Amplitude
/// @param sinBuf Pointer to the buffer where the sinwave is stored.
/// @param bufSize Size of the buffer, in bytes.
// =============================================================================
PROTECTED VOID sinGen(UINT16 f, UINT8 ampl, UINT32 *sinBuf, UINT32 bufSize)
{
	/* fn and nn are the normalized versions of f and n. */
	UINT32 n, nn;
	/* fn = n / SINREF_F */
	UINT32 tmpSpl;
	UINT16	*sinPtr = (UINT16*)HAL_SYS_GET_UNCACHED_ADDR(sinBuf);

	/* Generates the sinwave. */
	for (n = 0; n < bufSize / 2; n++)
	{
		/* Scale the samples on a period of a sine. */
		nn = ( (n * f) / SINREF_F ) % (SINREF_SIZE * 4);
		
		/* Scale the samples on a quarter of a period of a sine. */
		if (nn >= SINREF_SIZE * 3)
		{
			/* Sample calculation, with good amplitude. */
			tmpSpl = (((UINT32)sinRef[(SINREF_SIZE * 4) - nn - 1]) *ampl) /AMPL_1;
			
			/* Saturation check on 15 bits. */
			if (tmpSpl > 0x7fff) tmpSpl = 0x7fff;
			
			sinPtr[n] = ~((UINT16)tmpSpl) + 1;
		}
		else if (nn >= SINREF_SIZE * 2)
		{
			/* Sample calculation, with good amplitude. */
			tmpSpl = (((UINT32)sinRef[nn - (SINREF_SIZE * 2)]) *ampl) /AMPL_1;
			
			/* Saturation check. */
			if (tmpSpl > 0x7fff) tmpSpl = 0x7fff;
			
			sinPtr[n] = ~((UINT16)tmpSpl) + 1;
		}
		else if (nn >= SINREF_SIZE)
		{
			/* Sample calculation, with good amplitude. */
			tmpSpl = (((UINT32)sinRef[(SINREF_SIZE * 2) - nn - 1]) *ampl) /AMPL_1;
			
			/* Saturation check. */
			if (tmpSpl > 0x7fff) tmpSpl = 0x7fff;
			
			sinPtr[n] = (UINT16)tmpSpl;
		}
		else
		{
			/* Sample calculation, with good amplitude. */
			tmpSpl = (((UINT32)sinRef[nn]) *ampl) /AMPL_1;
			
			/* Saturation check. */
			if (tmpSpl > 0x7fff) tmpSpl = 0x7fff;

			sinPtr[n] = (UINT16)tmpSpl;
		}
	}
}



