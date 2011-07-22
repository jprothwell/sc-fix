/*************************************************************************
 *
 *  FUNCTION:  Reorder_lsf()
 *
 *  PURPOSE: To make sure that the LSFs are properly ordered and to keep a
 *           certain minimum distance between adjacent LSFs.                               *
 *           The LSFs are in the frequency range 0-0.5 and represented in Q15
 *
 *************************************************************************/

#include "cs_types.h"
#include "vpp_efr_basic_op.h"

#include "vpp_efr_basic_macro.h"

void Reorder_lsf (
    INT16 *lsf,        /* (i/o)     : vector of LSFs   (range: 0<=val<=0.5) */
    INT16 min_dist,    /* (i)       : minimum required distance             */
    INT16 n            /* (i)       : LPC order                             */
)
{
    INT16 i;
    INT16 lsf_min;

    lsf_min = min_dist;
    for (i = 0; i < n; i++)
    {

        //if (sub (lsf[i], lsf_min) < 0)
		if (lsf[i]<lsf_min)
        {
            lsf[i] = lsf_min;
        }
        //lsf_min = add (lsf[i], min_dist);
        lsf_min = ADD (lsf[i], min_dist);

    }
}





















