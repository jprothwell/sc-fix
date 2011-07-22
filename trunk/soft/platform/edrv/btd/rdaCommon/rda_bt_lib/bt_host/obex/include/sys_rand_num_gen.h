#ifndef _PARTHUS_SYS_RAND_NUM_GEN_
#define _PARTHUS_SYS_RAND_NUM_GEN_

/***********************************************************************
 *
 * MODULE NAME:    sys_rand_num_gen.h
 * PROJECT CODE:    pimiento
 * DESCRIPTION:    Function Prototypes for Random Number Generation
 * MAINTAINER:     Ivan Griffin
 * DATE:           17 February 2000
 *
 * SOURCE CONTROL: $Id: sys_rand_num_gen.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000 Parthus Technologies plc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    17.Feb.2000 -   IG       - initial version
 *
 * ISSUES:
 *    See sys/sys_rand_num_gen.c for issues.
 *    
 ***********************************************************************/


#include "papi.h"


#define SYS_RAND_MAX 0xffffffff

void SYSrand_Seed_Rand(unsigned int seed);
int SYSrand_Get_Rand(void);
int SYSrand_Get_Rand_R(unsigned int *seed);

u_int8* SYSrand_Get_Rand_128_Ex(u_int8* buf);

#endif
