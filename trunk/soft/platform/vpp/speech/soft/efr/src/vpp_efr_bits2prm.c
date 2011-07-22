/*************************************************************************
 *
 *  FUNCTION:  Bits2prm_12k2
 *
 *  PURPOSE: Retrieves the vector of encoder parameters from the received
 *           serial bits in a frame.
 *
 *  DESCRIPTION: The encoder parameters are:
 *
 *     BFI      bad frame indicator      1 bit
 *
 *     LPC:
 *              1st codebook             7 bit
 *              2nd codebook             8 bit
 *              3rd codebook             8+1 bit
 *              4th codebook             8 bit
 *              5th codebook             6 bit
 *
 *     1st and 3rd subframes:
 *           pitch period                9 bit
 *           pitch gain                  4 bit
 *           codebook index              35 bit
 *           codebook gain               5 bit
 *
 *     2nd and 4th subframes:
 *           pitch period                6 bit
 *           pitch gain                  4 bit
 *           codebook index              35 bit
 *           codebook gain               5 bit
 *
 *************************************************************************/
#include <stdio.h>

#include "cs_types.h"

#include "vpp_efr_cnst.h"
#include "vpp_efr_codec.h"

//For profiling
#include "vpp_efr_debug.h"
#include "vpp_efr_profile_codes.h"

/* Local function */



void Bits2prm_12k2 (
    INT16 bits[],      /* input : serial bits (244 + bfi)                */
    INT16 prm[]        /* output: analysis parameters  (57+1 parameters) */
)
{
    INT16 i;
    INT16 j=1, k=0;

    VPP_EFR_PROFILE_FUNCTION_ENTER(Bits2prm_12k2);

    for (i=1;i<VPP_EFR_COD_FRAME_LEN+1; i++)
    {
        if(k < bitno [j-1])
        {
            prm[j] =  (prm[j]<<1) | ((bits [i/16]>> (i%16))& 1);
            k++;
            if(k==bitno [j-1])
            {
               j++;
               k=0;
            }
        }


    }
    VPP_EFR_PROFILE_FUNCTION_EXIT(Bits2prm_12k2);
    return;
}

/*************************************************************************
 *
 *  FUNCTION:  Bin2int
 *
 *  PURPOSE: Read "no_of_bits" bits from the array bitstream[] and convert
 *           to integer.
 *
 *************************************************************************/

 /*INT16 Bin2int (        // Reconstructed parameter
    INT16 no_of_bits,  // input : number of bits associated with value
    INT16 *bitstream   // output: address where bits are written
)
{
    INT16 value, i, bit;

    value = 0;
    for (i = 0; i < no_of_bits; i++)
    {
        //value = shl (value, 1);
        value = SHL(value, 1);
        bit = *bitstream++;
        //if (sub (bit, BIT_1) == 0)
        if (SUB (bit, BIT_1) == 0)
		{
            //value = add (value, 1);
			value = ADD (value, 1);
		}
    }
    return (value);
}*/





















