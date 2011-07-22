/*************************************************************************
 *
 *  FUNCTION:   Convolve
 *
 *  PURPOSE:
 *     Perform the convolution between two vectors x[] and h[] and
 *     write the result in the vector y[]. All vectors are of length L
 *     and only the first L samples of the convolution are computed.
 *
 *  DESCRIPTION:
 *     The convolution is given by
 *
 *          y[n] = sum_{i=0}^{n} x[i] h[n-i],        n=0,...,L-1
 *
 *************************************************************************/

#include "cs_types.h"
#include "vpp_efr_basic_op.h"
#include "vpp_efr_basic_macro.h"

#include "vpp_efr_debug.h"
#include "vpp_efr_profile_codes.h"

void Convolve (
    INT16 x[],        /* (i)     : input vector                           */
    INT16 h[],        /* (i)     : impulse response                       */
    INT16 y[],        /* (o)     : output vector                          */
    INT16 L           /* (i)     : vector size                            */
)
{
    INT16 i, n;
    //INT32 s;
    register INT32 s_hi=0;
    register UINT32 s_lo=0;


    VPP_EFR_PROFILE_FUNCTION_ENTER(Convolve);

    for (n = 0; n < L; n++)
    {
        s_lo = 0;
        for (i = 0; i <= n; i++)
        {
            //s = L_mac (s, x[i], h[n - i]);
			VPP_MLA16(s_hi, s_lo, x[i], h[n - i]);
        }
        //s = L_shl (s, 3);
        //s = L_SHL_SAT(VPP_SCALE64_TO_16(s_hi, s_lo), 3);
		//y[n] = EXTRACT_H(s);

		y[n] = (INT16)(L_SHR_D(s_lo,12));

    }

    VPP_EFR_PROFILE_FUNCTION_EXIT(Convolve);

    return;
}





















