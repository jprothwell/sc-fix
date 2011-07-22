/***********************************************************************
 *
 * MODULE NAME:    papi_impl.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    PAPI eCos specific include
 * MAINTAINER:     John Sheehy
 * CREATION DATE:  1 May 2000
 *
 * SOURCE CONTROL: $Id: papi_impl.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    1.May.2000 - JS - first import
 *
 *
 * ISSUES:
 *       
 ***********************************************************************/
#ifndef PAPI_IMPL_H
#define PAPI_IMPL_H

#include "host_config.h"
#include "cs_types.h"
#define pNULL NULL
#if 0 
#ifndef __size_t
#define __size_t 1
typedef u_int size_t;
#endif
#endif
typedef u_int pSem_t;
typedef u_int t_pEvent;
typedef u_int t_pMutex;
typedef u_int t_pTimer;

void papiSched(unsigned int dummy);


#endif /* PAPI_IMPL_H */
