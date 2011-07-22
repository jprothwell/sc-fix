/*************************************************************************
 *
 *  FUNCTION:  Lag_window()
 *
 *  PURPOSE:  Lag windowing of autocorrelations.
 *
 *  DESCRIPTION:
 *         r[i] = r[i]*lag_wind[i],   i=1,...,10
 *
 *     r[i] and lag_wind[i] are in special double precision format.
 *     See "oper_32b.c" for the format.
 *
 *************************************************************************/

#include "cs_types.h"
#include "vpp_efr_basic_op.h"
#include "vpp_efr_oper_32b.h"

#include "vpp_efr_lag_wind.tab"

void Lag_window (
    INT16 m,           /* (i)     : LPC order                        */
    INT16 r_h[],       /* (i/o)   : Autocorrelations  (msb)          */
    INT16 r_l[]        /* (i/o)   : Autocorrelations  (lsb)          */
)
{
    INT16 i;
    INT32 x;

    for (i = 1; i <= m; i++)
    {
        x = Mpy_32 (r_h[i], r_l[i], lag_h[i - 1], lag_l[i - 1]);
        L_Extract (x, &r_h[i], &r_l[i]);
    }
    return;
}
