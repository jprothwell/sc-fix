//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spp_math.c
///
/// This file contains math functions of the Signal Processing Package
///
/// @date 16/05/07
/// @author Laurent Bastide
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "cs_types.h"
#include "sppp_gsm_private.h"


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------



//=============================================================================
// spp_10log10
//-----------------------------------------------------------------------------
// This function performs output=10xlog10(input).
//
// @param input UINT32. Linear power.
//
// @return UINT8. 10Log10(input) => dB.
//
// @addgroup misc_func
// @author  jba
// @date 17-06-2005
//=============================================================================
PROTECTED UINT8 spp_10log10(UINT32 in)
{
    UINT32 x=in;
    UINT16 result;
    UINT16 exp;
    if (x == 0) return 0;
    else if (x <2) exp = 31;
    else
    {
        if (x == 0xffffffff)
        {
            result=96;
            return (UINT8) result;
        }
        else
        {
            for(exp = 0;x < 0x80000000;exp++)
            {
                x <<= 1;
            }
        }
    }

    exp = 31 - exp;
    in = in - (1<<exp);
    if(exp<8) in<<=(8-exp);
    else in>>=(exp-8);
    exp<<=8;
    result=(UINT16)(3*(exp+in));
    if((result&0xFF)>100) result=(result>>8)+1;
    else result=(result>>8);

    return (UINT8) result;
}



//=============================================================================
// spp_Log2
//-----------------------------------------------------------------------------
// This function performs output=Log2(input).
//
// @param input UINT32. Linear power.
//
// @return UINT8. Log2(input).
//
// @addgroup misc_func
// @author  jba
// @date 17-06-2005
//=============================================================================
PROTECTED UINT8 spp_Log2(UINT32 in)
{
    UINT32 x=in;
    UINT16 result;
    UINT16 exp;

    if (x == 0) return 0;
    else if (x <2) exp = 31;
    else
    {
        if (x == 0xffffffff)
        {
            result=96;
            return (UINT8) result;
        }
        else
        {
            for(exp = 0;x < 0x80000000;exp++)
            {
                x <<= 1;
            }
        }
    }
    exp = 31 - exp;
    in = in - (1<<exp);
    if(exp<8) in<<=(8-exp);
    else in>>=(exp-8);
    exp<<=8;
    result=(UINT16)(exp+in);
    if((result&0xFF)>100) result=(result>>8)+1;
    else result=(result>>8);

    return (UINT8) result;
}

//=============================================================================
// spp_DivL
//-----------------------------------------------------------------------------
// This function performs the division of two signed integer
// and generated a result in Q(fracBits) representation.
// Example : with num = 1, denom = 4 and fracBits = 3, the returned value is 2.
//
// @param num INT32. Numerator.
// @param denom INT32 Denominator.
// @param fracBits UINT8 Franctional bits in the result representation
//
// @return INT16. The saturated LO of the 32-bit result of the division in Q(fracBits) representation.
//
// @addgroup misc_func
// @author  yordan
// @date 14-04-2008
//=============================================================================
PROTECTED INT16 spp_DivL(INT32 num, INT32 denom, UINT8 fracBits)
{
    INT32 res = 0;

    if (!denom)
    {
        res = (num < 0) ? MIN_INT32 : MAX_INT32;
    }
    else
    {
        UINT32 rscl = 31 - fracBits;
        UINT8 sign = (((num) ^ (denom)) < 0) ? 1 : 0;
        INT32 n = SPP_ABS(num);
        INT32 d = SPP_ABS(denom);

        // normalize the denominator
        // and compute the rescaling factor
        while ((d <= n) && (rscl >= 0))
        {
            d <<= 1;
            rscl--;
        }

        // if the rescaling is not possible, the division result is too big,
        // so it needs to be saturated
        if (rscl < 0)
        {
            res = (sign) ? MIN_INT32 : MAX_INT32;
        }
        else
        {
            UINT8 i;

            for (i = 0; i < 31; ++i)
            {
                res <<= 1;
                n <<= 1;

                if (n >= d)
                {
                    n -= d;
                    res++;
                }
            }

            if (sign)
            {
                res = -res;
            }

            res >>= rscl;

        }
    }

    // return the saturated LO of the 32-bit result
    return SPP_SAT_INT16(res);

}



