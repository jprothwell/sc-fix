/*************************************************************************
 *
 *  FUNCTION:   G_code
 *
 *  PURPOSE:  Compute the innovative codebook gain.
 *
 *  DESCRIPTION:
 *      The innovative codebook gain is given by
 *
 *              g = <x[], y[]> / <y[], y[]>
 *
 *      where x[] is the target vector, y[] is the filtered innovative
 *      codevector, and <> denotes dot product.
 *
 *************************************************************************/

#include "cs_types.h"
#include "vpp_efr_basic_op.h"
#include "vpp_efr_cnst.h"
#include "vpp_efr_basic_macro.h"

#include "vpp_efr_debug.h"
#include "vpp_efr_profile_codes.h"

INT16 G_code (         /* out   : Gain of innovation code         */
    INT16 xn2[],       /* in    : target vector                   */
    INT16 y2[]         /* in    : filtered innovation vector      */
)
{
    INT16 i;
    INT16 xy, yy, exp_xy, exp_yy, gain;
    INT16 scal_y2[L_SUBFR];
    INT32 s;
    register INT32 s_hi=0;
    register UINT32 s_lo=0;

    VPP_EFR_PROFILE_FUNCTION_ENTER(G_code);


    /* Scale down Y[] by 2 to avoid overflow */

    for (i = 0; i < L_SUBFR; i++)
    {
        //scal_y2[i] = shr (y2[i], 1);
		scal_y2[i] = SHR_D(y2[i], 1);

    }

    /* Compute scalar product <X[],Y[]> */

    //s = 1L;                            /* Avoid case of all zeros */
    s_lo = 0;
    for (i = 0; i < L_SUBFR; i++)
    {
        //s = L_mac (s, xn2[i], scal_y2[i]);
		//s = L_MAC(s, xn2[i], scal_y2[i]);
		VPP_MLA16 ( s_hi, s_lo,   xn2[i], scal_y2[i]);
    }
    s = VPP_SCALE64_TO_16(s_hi, s_lo);
    exp_xy = norm_l (s);
    //xy = extract_h (L_shl (s, exp_xy));
    xy = EXTRACT_H (L_SHL (s, exp_xy));

    /* If (xy < 0) gain = 0  */
    if (xy <= 0)
    {
        VPP_EFR_PROFILE_FUNCTION_EXIT(G_code);
        return ((INT16) 0);
    }

    /* Compute scalar product <Y[],Y[]> */

    //s = 0L;
    s_lo = 0;
    for (i = 0; i < L_SUBFR; i++)
    {
        //s = L_mac (s, scal_y2[i], scal_y2[i]);
		//s = L_MAC(s, scal_y2[i], scal_y2[i]);
		VPP_MLA16(s_hi, s_lo, scal_y2[i], scal_y2[i]);
    }
    s = VPP_SCALE64_TO_16(s_hi, s_lo);

    exp_yy = norm_l (s);
    //yy = extract_h (L_shl (s, exp_yy));
    yy = EXTRACT_H(L_SHL(s, exp_yy));

    /* compute gain = xy/yy */

    //xy = shr (xy, 1);                 /* Be sure xy < yy */
    xy = SHR_D(xy, 1);
	gain = div_s (xy, yy);

    /* Denormalization of division */
    //i = add (exp_xy, 5);              /* 15-1+9-18 = 5 */
    i = ADD (exp_xy, 5);
	//i = sub (i, exp_yy);
	i = SUB (i, exp_yy);

    //gain = shr (gain, i);
	gain = SHR_V(gain, i);

    VPP_EFR_PROFILE_FUNCTION_EXIT(G_code);

    return (gain);
}





















