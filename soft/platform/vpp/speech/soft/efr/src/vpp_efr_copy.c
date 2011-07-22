/*************************************************************************
 *
 *   FUNCTION:   Copy
 *
 *   PURPOSE:   Copy vector x[] to y[]
 *
 *
 *************************************************************************/

#include "cs_types.h"
#include "vpp_efr_basic_op.h"

#include "vpp_efr_debug.h"
#include "vpp_efr_profile_codes.h"

void Copy (
    INT16 x[],         /* (i)   : input vector   */
    INT16 y[],         /* (o)   : output vector  */
    INT16 L            /* (i)   : vector length  */
)
{
    INT16 i;
    VPP_EFR_PROFILE_FUNCTION_ENTER(Copy);

    for (i = 0; i < L; i++)
    {
        y[i] = x[i];
    }

    VPP_EFR_PROFILE_FUNCTION_EXIT(Copy);
    return;
}
