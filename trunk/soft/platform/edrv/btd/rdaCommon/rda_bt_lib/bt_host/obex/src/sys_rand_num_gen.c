/*****************************************************************************
 *
 * MODULE NAME:    sys_rand_num_gen.c
 * PROJECT CODE:    pimiento
 * DESCRIPTION:    32-bit pseudo random number generation
 * MAINTAINER:     Ivan Griffin
 * CREATION DATE:  17 February 2000
 *
 * SOURCE CONTROL: $Id: sys_rand_num_gen.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000 Parthus Technologies plc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *     17.Feb.2000       IG      Initial Version
 *
 * ISSUES:
 *     This represents a trade-off between spatial dispersion and
 *     speed/processing power
 *
 *     For more interesting random number algorithms, see:
 *
 *     "The Art of Computer Programming: Seminumerical Algorithms",
 *     Donald E. Knuth, 3rd Edition, Section 3.6
 *
 *     "Towards a Universal Random Number Generator" by George Marsaglia
 *     and Arif Zaman, Florida State University Report: FSU-SCRI-87-50 (1987)
 *     
 *     and "A Review of Pseuo-random Number Generators" by F. James.
 *****************************************************************************/


#include "sys_rand_num_gen.h"
#include "host_types.h"
#include "papi.h"

/*
 * simple Random Algorithm from ISO C.
 * extended to permutate lower bits more
 */

/*static */ u_int32 srand_seed;

void SYSrand_Seed_Rand(u_int32 seed)
{
    srand_seed = seed;
}

int SYSrand_Get_Rand(void)
{
    return (SYSrand_Get_Rand_R(&srand_seed));
}

int SYSrand_Get_Rand_R(u_int32 *seed) /* reentrant */
{
    u_int32 result;
    u_int32 next_seed = *seed;

    next_seed = (next_seed * 1103515245) + 12345; /* permutate seed */
    result = next_seed & 0xfffc0000; /* use only top 14 bits */

    next_seed = (next_seed * 1103515245) + 12345; /* permutate seed */
    result = result + ((next_seed & 0xffe00000) >> 14); /* top 11 bits */

    next_seed = (next_seed * 1103515245) + 12345; /* permutate seed */
    result = result + ((next_seed & 0xfe000000) >> (25)); /* use only top 7 bits */

    *seed = next_seed;

    return (((int)result) & SYS_RAND_MAX);
}

u_int8* SYSrand_Get_Rand_128_Ex(u_int8* buf)
{
    u_int32 result;
    int i;

    for (i = 0; i < 4; i++)
    {
        result = (u_int32)SYSrand_Get_Rand();

        *(buf + (4 * i) + 0) = result & 0xff;
        *(buf + (4 * i) + 1) = (result & 0xff00) >> 8;
        *(buf + (4 * i) + 2) = (result & 0xff0000) >> 16;
        *(buf + (4 * i) + 3) = (result & 0xff000000) >> 24;
    }

    return buf;
}
