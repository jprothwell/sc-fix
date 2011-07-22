/***********************************************************************
 *
 * MODULE NAME:    bt_sys.h
 * DESCRIPTION:    Bluetooth Host Stack API system-specific header
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: bt_sys.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 * REMARKS:
 *     This file should be incuded via bt.h only.
 *
 ***********************************************************************/


#ifndef BLUETOOTH_HOST_SYS_DECLARED
#define BLUETOOTH_HOST_SYS_DECLARED


#ifdef _WIN32
/* These definitions are to enable interfacing to a DLL */
#    ifndef APIDECL1
#        define APIDECL1 _declspec(dllimport)
#    endif
#    ifndef APIDECL2
#        define APIDECL2 __stdcall
#    endif
#else
#    define APIDECL1 
#    define APIDECL2  
#endif


/* Platform specific type definitions */

#ifndef PARTHUS_SYS_TYPES
#define PARTHUS_SYS_TYPES
typedef unsigned char  u_int8;		/* unsigned integer,  8 bits long */
typedef unsigned short u_int16;		/* unsigned integer, 16 bits long */
typedef unsigned int   u_int32;		/* unsigned integer, 32 bits long */
#endif

#endif /* BLUETOOTH_HOST_SYS_DECLARED */
