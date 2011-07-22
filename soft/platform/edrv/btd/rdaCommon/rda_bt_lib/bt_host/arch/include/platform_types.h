/***********************************************************************
 *
 * MODULE NAME:    platform_types.h
 * DESCRIPTION:    eCos specific types for Host Side Stack
 * AUTHOR:         Dave Airlie
 *
 * SOURCE CONTROL: $Id: platform_types.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies.
 *     All rights reserved.
 *
 ***********************************************************************/

#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

#ifndef PRH_COMBINED_SYS_COMMON_TYPES
#define PRH_COMBINED_SYS_COMMON_TYPES

#ifndef PARTHUS_SYS_TYPES
#define PARTHUS_SYS_TYPES
typedef unsigned char u_int8;       /* unsigned integer,  8 bits long */
typedef unsigned short u_int16;     /* unsigned integer, 16 bits long */
typedef unsigned int u_int32;       /* unsigned integer, 32 bits long */
#endif

typedef unsigned int u_int;

typedef signed char s_int8;         /* unsigned integer,  8 bits long */
typedef signed short s_int16;       /* unsigned integer,  8 bits long */
typedef signed long s_int32;        /* unsigned integer,  8 bits long */
#endif


#endif /* PLATFORM_TYPES_H */
