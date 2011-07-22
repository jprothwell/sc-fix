/*************************************************************************
 *
 *  FUNCTION:  Weight_Ai
 *
 *  PURPOSE: Spectral expansion of LP coefficients.  (order==10)
 *
 *  DESCRIPTION:
 *      a_exp[i] = a[i] * fac[i-1]    ,i=1,10
 *
 *************************************************************************/

#include "cs_types.h"
#include "vpp_efr_basic_op.h"

#include "vpp_efr_basic_macro.h"

#include "vpp_efr_profile_codes.h"
#include "vpp_efr_debug.h"

/* m = LPC order == 10 */
#define m 10

void Weight_Ai (
    INT16 a[],         /* (i)     : a[m+1]  LPC coefficients   (m=10)    */
    const INT16 fac[],       /* (i)     : Spectral expansion factors.          */
    INT16 a_exp[]      /* (o)     : Spectral expanded LPC coefficients   */
)
{
    INT16 i;
    register INT32 hi = 0;
    register UINT32 lo =0;

    VPP_EFR_PROFILE_FUNCTION_ENTER(Weight_Ai);

    a_exp[0] = a[0];
    for (i = 1; i <= m; i++)
    {
		//a_exp[i] = ROUND(L_MULT (a[i], fac[i - 1]));
		VPP_MLX16(hi,lo, a[i], fac[i - 1]);
		a_exp[i] =  L_SHR_D(L_ADD((INT32)lo, 0x4000), 15);
    }

    VPP_EFR_PROFILE_FUNCTION_EXIT(Weight_Ai);

    return;
}
