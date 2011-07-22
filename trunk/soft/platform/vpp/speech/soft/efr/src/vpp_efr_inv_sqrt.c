/*************************************************************************
 *
 *  FUNCTION:   Inv_sqrt
 *
 *  PURPOSE:   Computes 1/sqrt(L_x),  where  L_x is positive.
 *             If L_x is negative or zero, the result is 1 (3fff ffff).
 *
 *  DESCRIPTION:
 *       The function 1/sqrt(L_x) is approximated by a table and linear
 *       interpolation. The inverse square root is computed using the
 *       following steps:
 *          1- Normalization of L_x.
 *          2- If (30-exponent) is even then shift right once.
 *          3- exponent = (30-exponent)/2  +1
 *          4- i = bit25-b31 of L_x;  16<=i<=63  because of normalization.
 *          5- a = bit10-b24
 *          6- i -=16
 *          7- L_y = table[i]<<16 - (table[i] - table[i+1]) * a * 2
 *          8- L_y >>= exponent
 *
 *************************************************************************/

#include "cs_types.h"
#include "vpp_efr_basic_op.h"
#include "vpp_efr_basic_macro.h"

#include "vpp_efr_debug.h"
#include "vpp_efr_profile_codes.h"
#include "vpp_efr_inv_sqrt.tab" /* Table for inv_sqrt() */

INT32 Inv_sqrt (       /* (o) : output value   */
    INT32 L_x          /* (i) : input value    */
)
{
    INT16 exp, i, a, tmp;
    INT32 L_y;
    register INT32 Ly_hi=0;
    register UINT32 Ly_lo=0;

    VPP_EFR_PROFILE_FUNCTION_ENTER(Inv_sqrt);

    if (L_x <= (INT32) 0)
    {
	VPP_EFR_PROFILE_FUNCTION_EXIT(Inv_sqrt);
        return ((INT32) 0x3fffffffL);
    }

    exp = norm_l (L_x);
    //L_x = L_shl (L_x, exp);     /* L_x is normalize */
    L_x = L_SHL(L_x, exp);
    //exp = sub (30, exp);
	exp = SUB (30, exp);

    if ((exp & 1) == 0)         /* If exponent even -> shift right */
    {
       // L_x = L_shr (L_x, 1);
       L_x = L_SHR_D(L_x, 1);
       // L_SHR(L_x, 1, L_x);
    }
    //exp = shr (exp, 1);
	exp = SHR_D(exp, 1);
    //exp = add (exp, 1);
	exp = ADD (exp, 1);

    //L_x = L_shr (L_x, 9);
    L_x = L_SHR_D(L_x, 9);
    //i = extract_h (L_x);        /* Extract b25-b31 */
    i = EXTRACT_H(L_x);
    //L_x = l_shr(L_x, 1);
    L_x = L_SHR_D(L_x, 1);
    //a = extract_l (L_x);        /* Extract b10-b24 */
    a = EXTRACT_L(L_x);
	a = a & (INT16) 0x7fff;

    //i = sub (i, 16);
	i = SUB (i, 16);

    //L_y = L_deposit_h (table[i]);         /* table[i] << 16          */
    //L_y = L_DEPOSIT_H(table[i]);

	Ly_lo = SHL(table[i],15); // L_DEPOSIT_H = sHL 16 + SHR 1 = sHL 15
	//tmp = sub (table[i], table[i + 1]); /* table[i] - table[i+1])  */
	tmp = SUB (table[i], table[i + 1]);
    //L_y = L_msu (L_y, tmp, a);  /* L_y -=  tmp*a*2         */
    //L_y = L_MSU(L_y, tmp, a);
    VPP_MLA16(Ly_hi,Ly_lo, tmp, -a);
    //L_y = L_shr (L_y, exp);     /* denormalization */
    L_y = L_SHR_V(VPP_SCALE64_TO_16(Ly_hi,Ly_lo), exp);

    VPP_EFR_PROFILE_FUNCTION_EXIT(Inv_sqrt);
    return (L_y);
}
