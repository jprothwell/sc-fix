/*************************************************************************
 *
 *  FUNCTION:  Pre_Process()
 *
 *  PURPOSE: Preprocessing of input speech.
 *
 *  DESCRIPTION:
 *     - 2nd order high pass filtering with cut off frequency at 80 Hz.
 *     - Divide input by two.
 *
 *************************************************************************/

#include "cs_types.h"
#include "vpp_efr_basic_op.h"
#include "vpp_efr_oper_32b.h"

#include "vpp_efr_basic_macro.h"

#include "vpp_efr_profile_codes.h"
#include "vpp_efr_debug.h"

/*------------------------------------------------------------------------*
 *                                                                        *
 * Algorithm:                                                             *
 *                                                                        *
 *  y[i] = b[0]*x[i]/2 + b[1]*x[i-1]/2 + b[2]*x[i-2]/2                    *
 *                     + a[1]*y[i-1]   + a[2]*y[i-2];                     *
 *                                                                        *
 *                                                                        *
 *  Input is divided by two in the filtering process.                     *
 *------------------------------------------------------------------------*/

/* filter coefficients (fc = 80 Hz, coeff. b[] is divided by 2) */

static const INT16 b[3] = {1899, -3798, 1899};
static const INT16 a[3] = {4096, 7807, -3733};

/* Static values to be preserved between calls */
/* y[] values are kept in double precision     */

static INT16 y2_hi, y2_lo, y1_hi, y1_lo, x0, x1;

/* Initialization of static values */

void Init_Pre_Process (void)
{
    y2_hi = 0;
    y2_lo = 0;
    y1_hi = 0;
    y1_lo = 0;
    x0 = 0;
    x1 = 0;
}

void Pre_Process (
    INT16 signal[], /* input/output signal */
    INT16 lg)       /* lenght of signal    */
{
    INT16 i, x2;
    INT32 L_tmp;
    register INT32 tmp_hi=0;
    register UINT32 tmp_lo=0;

    VPP_EFR_PROFILE_FUNCTION_ENTER(Pre_Process);

    for (i = 0; i < lg; i++)
    {
        x2 = x1;
        x1 = x0;
        x0 = signal[i];

        /*  y[i] = b[0]*x[i]/2 + b[1]*x[i-1]/2 + b140[2]*x[i-2]/2  */
        /*                     + a[1]*y[i-1] + a[2] * y[i-2];      */

        L_tmp = Mpy_32_16 (y1_hi, y1_lo, a[1]);
        //L_tmp = L_add (L_tmp, Mpy_32_16 (y2_hi, y2_lo, a[2]));
        L_tmp = L_ADD(L_tmp, Mpy_32_16 (y2_hi, y2_lo, a[2]));

        tmp_lo = L_tmp>>1;

		//L_tmp = L_mac (L_tmp, x0, b[0]);
        //L_tmp = L_mac (L_tmp, x1, b[1]);
        //L_tmp = L_mac (L_tmp, x2, b[2]);

		//L_tmp = L_MAC (L_tmp, x0, b[0]);
        //L_tmp = L_MAC (L_tmp, x1, b[1]);
        //L_tmp = L_MAC (L_tmp, x2, b[2]);
        VPP_MLA16(tmp_hi, tmp_lo, x0, b[0]);
        VPP_MLA16(tmp_hi, tmp_lo, x1, b[1]);
        VPP_MLA16(tmp_hi, tmp_lo, x2, b[2]);

        L_tmp = VPP_SCALE64_TO_16(tmp_hi, tmp_lo);
		//L_tmp = L_shl (L_tmp, 3);
        L_tmp = L_SHL_SAT(L_tmp, 3);
        //signal[i] = round(L_tmp);
		signal[i] = ROUND(L_tmp);


        y2_hi = y1_hi;
        y2_lo = y1_lo;
        L_Extract (L_tmp, &y1_hi, &y1_lo);
    }

    VPP_EFR_PROFILE_FUNCTION_EXIT(Pre_Process);
    return;
}
