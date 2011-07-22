/*************************************************************************
 *
 *   FUNCTION:   Log2()
 *
 *   PURPOSE:   Computes log2(L_x),  where   L_x is positive.
 *              If L_x is negative or zero, the result is 0.
 *
 *   DESCRIPTION:
 *        The function Log2(L_x) is approximated by a table and linear
 *        interpolation. The following steps are used to compute Log2(L_x)
 *
 *           1- Normalization of L_x.
 *           2- exponent = 30-exponent
 *           3- i = bit25-b31 of L_x;  32<=i<=63  (because of normalization).
 *           4- a = bit10-b24
 *           5- i -=32
 *           6- fraction = table[i]<<16 - (table[i] - table[i+1]) * a * 2
 *
 *************************************************************************/

#include "cs_types.h"
#include "vpp_efr_basic_op.h"
#include "vpp_efr_basic_macro.h"

#include "vpp_efr_log2.tab"     /* Table for Log2() */

void Log2 (
    INT32 L_x,         /* (i) : input value                                 */
    INT16 *exponent,   /* (o) : Integer part of Log2.   (range: 0<=val<=30) */
    INT16 *fraction    /* (o) : Fractional part of Log2. (range: 0<=val<1) */
)
{
    INT16 exp, i, a, tmp;
    //INT32 L_y;
    register INT32 Ly_hi=0;
    register UINT32 Ly_lo=0;


    if (L_x <= (INT32) 0)
    {
        *exponent = 0;
        *fraction = 0;
        return;
    }
    exp = norm_l (L_x);
    //L_x = L_shl (L_x, exp);     /* L_x is normalized */
    L_x = L_SHL(L_x, exp);

    //*exponent = sub (30, exp);
    *exponent = SUB (30, exp);


   //L_x = L_shr (L_x, 9);
   L_x = L_SHR_D(L_x, 9);
    //i = extract_h (L_x);        /* Extract b25-b31 */
    i = EXTRACT_H(L_x);
	//L_x = L_shr (L_x, 1);
	L_x = L_SHR_D(L_x, 1);
    //a = extract_l (L_x);        /* Extract b10-b24 of fraction */
    a = EXTRACT_L(L_x);
	a = a & (INT16) 0x7fff;

    //i = sub (i, 32);
	i = SUB (i, 32);

    //L_y = L_deposit_h (table[i]);       /* table[i] << 16        */
    Ly_lo = SHL(table[i],15); // L_DEPOSIT_H = sHL 16 + SHR 1 = sHL 15
	//tmp = sub (table[i], table[i + 1]); /* table[i] - table[i+1] */
    tmp = SUB (table[i], table[i + 1]);
	//L_y = L_msu (L_y, tmp, a);  /* L_y -= tmp*a*2        */
    //L_y = L_MSU(L_y, tmp, a);
    VPP_MLA16(Ly_hi,Ly_lo, tmp, -a);
	//*fraction = extract_h (L_y);
    *fraction = EXTRACT_H(VPP_SCALE64_TO_16(Ly_hi,Ly_lo));

    return;
}
