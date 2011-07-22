/*************************************************************************
 *
 *   FUNCTION:  Pow2()
 *
 *   PURPOSE: computes  L_x = pow(2.0, exponent.fraction)
 *
 *   DESCRIPTION:
 *       The function Pow2(L_x) is approximated by a table and linear
 *       interpolation.
 *          1- i = bit10-b15 of fraction,   0 <= i <= 31
 *          2- a = bit0-b9   of fraction
 *          3- L_x = table[i]<<16 - (table[i] - table[i+1]) * a * 2
 *          4- L_x = L_x >> (30-exponent)     (with rounding)
 *
 *************************************************************************/

#include "cs_types.h"
#include "vpp_efr_basic_op.h"

#include "vpp_efr_basic_macro.h"

#include "vpp_efr_pow2.tab"     /* Table for Pow2() */

INT32 Pow2 (           /* (o)  : result       (range: 0<=val<=0x7fffffff) */
    INT16 exponent,    /* (i)  : Integer part.      (range: 0<=val<=30)   */
    INT16 fraction     /* (i)  : Fractional part.  (range: 0.0<=val<1.0) */
)
{
    INT16 exp, i, a, tmp;
    INT32 L_x;
    register INT32 Lx_hi=0;
    register UINT32 Lx_lo=0;

    //L_x = L_mult (fraction, 32);/* L_x = fraction<<6           */
    L_x = L_MULT(fraction, 32);
	//i = extract_h (L_x);        /* Extract b10-b16 of fraction */
    i = EXTRACT_H(L_x);
	//L_x = L_shr (L_x, 1);
	L_x = L_SHR_D(L_x, 1);
    //a = extract_l (L_x);        /* Extract b0-b9   of fraction */
    a = EXTRACT_L(L_x);
	a = a & (INT16) 0x7fff;

    //L_x = L_deposit_h (table[i]);       /* table[i] << 16        */
    //L_x = L_DEPOSIT_H(table[i]);
    Lx_lo = SHL(table[i],15); // L_DEPOSIT_H = <<16 + >>1 = << 15
	//tmp = sub (table[i], table[i + 1]); /* table[i] - table[i+1] */
    tmp = SUB (table[i], table[i + 1]);
	//L_x = L_msu (L_x, tmp, a);  /* L_x -= tmp*a*2        */
    //L_x = L_MSU(L_x, tmp, a);
    VPP_MLA16(Lx_hi,Lx_lo, tmp, -a);

    //exp = sub (30, exponent);
	exp = SUB (30, exponent);
    L_x = L_shr_r (VPP_SCALE64_TO_16(Lx_hi,Lx_lo), exp);

    return (L_x);
}





















