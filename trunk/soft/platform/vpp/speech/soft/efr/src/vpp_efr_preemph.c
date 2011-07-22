/*---------------------------------------------------------------------*
 * routine preemphasis()                                               *
 * ~~~~~~~~~~~~~~~~~~~~~                                               *
 * Preemphasis: filtering through 1 - g z^-1                           *
 *---------------------------------------------------------------------*/

#include "cs_types.h"
#include "vpp_efr_basic_op.h"

#include "vpp_efr_basic_macro.h"

INT16 mem_pre;

void preemphasis (
    INT16 *signal, /* (i/o)   : input signal overwritten by the output */
    INT16 g,       /* (i)     : preemphasis coefficient                */
    INT16 L        /* (i)     : size of filtering                      */
)
{
    INT16 *p1, *p2, temp, i;

    p1 = signal + L - 1;
    p2 = p1 - 1;
    temp = *p1;

    for (i = 0; i <= L - 2; i++)
    {
        //*p1 = sub (*p1, mult (g, *p2--));
		*p1 = SUB (*p1, MULT(g, *p2--));

        p1--;
    }

    //*p1 = sub (*p1, mult (g, mem_pre));
    *p1 = SUB (*p1, MULT(g, mem_pre));


    mem_pre = temp;

    return;
}
