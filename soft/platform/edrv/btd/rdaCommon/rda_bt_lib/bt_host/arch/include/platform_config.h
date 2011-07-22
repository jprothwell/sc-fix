/***********************************************************************
 *
 * MODULE NAME:    platform_config.h
 * DESCRIPTION:    Host Side Stack ecos specific configuration options 
 * AUTHOR:         Dave Airlie
 *
 * SOURCE CONTROL: $Id: platform_config.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies.
 *     All rights reserved.
 *
 ***********************************************************************/

#ifndef PLATFORM_CONFIG_H
#define PLATFORM_CONFIG_H

#include "platform_types.h"



#ifdef APIDECL1
#    undef APIDECL1
#endif

#ifdef APIDECL2
#    undef APIDECL2
#endif

#define APIDECL1
#define APIDECL2

#define PLATFORMSTRING "MIPS"

/* This is used in the HCI */
#define TOGGLE_ENDIANESS_UINT32(x)  (((x)>>24)&0xFF)|(((x)>>8)&0xFF00)|(((x)<<8)&0xFF0000)|(((x)<<24)&0xFF000000)

#define BTHOST_BIGENDIAN 0
/* 
    These are #defined to the smallest way to write/read u_intXX to/from a buffer on the platform 
    The UINT8 version is included for completeness, and probably won't/shouldn't be used....     -MM
*/
 

#define BT_WRITE_LE_UINT8(ptr,val)  (((*((u_int8 *)(ptr)))=(val)))
#define BT_WRITE_LE_UINT16(ptr,val) (((*((u_int8 *)(ptr)))=((u_int8)(val))),((*((u_int8 *)((ptr)+1)))=((u_int8)((val)>>8))))
#define BT_WRITE_LE_UINT24(ptr,val) (((*((u_int8 *)(ptr)))=((u_int8)(val))),((*((u_int8 *)((ptr)+1)))=((u_int8)((val)>>8))),((*((u_int8 *)((ptr)+2)))=((u_int8)((val)>>16))))
#define BT_WRITE_LE_UINT32(ptr,val) (((*((u_int8 *)(ptr)))=((u_int8)(val))),((*((u_int8 *)((ptr)+1)))=((u_int8)((val)>>8))),((*((u_int8 *)((ptr)+2)))=((u_int8)((val)>>16))),((*((u_int8 *)((ptr)+3)))=((u_int8)((val)>>24))))

#define BT_READ_LE_UINT8(ptr)       ((*((u_int8 *)(ptr)))) 
#define BT_READ_LE_UINT16(ptr)      ((*((u_int8 *)(ptr)))|((*((u_int8 *)((ptr)+1)))<<8))  
#define BT_READ_LE_UINT24(ptr)      ((*((u_int8 *)(ptr)))|((*((u_int8 *)((ptr)+1)))<<8)|((*((u_int8 *)((ptr)+2)))<<16))  
#define BT_READ_LE_UINT32(ptr)      ((*((u_int8 *)(ptr)))|((*((u_int8 *)((ptr)+1)))<<8)|((*((u_int8 *)((ptr)+2)))<<16)|((*((u_int8 *)((ptr)+3)))<<24))  



/* This may be more optimal on the ARM .....  these functions are provided in platform_operations.c */

/*

void BT_Write_LE_uint8(u_int8 *ptr, u_int32 val);
void BT_Write_LE_uint16(u_int8 *ptr, u_int32 val);
void BT_Write_LE_uint24(u_int8 *ptr, u_int32 val);
void BT_Write_LE_uint32(u_int8 *ptr, u_int32 val);
u_int8 BT_Read_LE_uint8(u_int8 *ptr);
u_int16 BT_Read_LE_uint16(u_int8 *ptr);
u_int32 BT_Read_LE_uint24(u_int8 *ptr);
u_int32 BT_Read_LE_uint32(u_int8 *ptr);

#define BT_WRITE_LE_UINT8(ptr,val)  BT_Write_LE_uint8((u_int8 *)ptr,(u_int32)val)
#define BT_WRITE_LE_UINT16(ptr,val) BT_Write_LE_uint16((u_int8 *)ptr,(u_int32)val)
#define BT_WRITE_LE_UINT24(ptr,val) BT_Write_LE_uint24((u_int8 *)ptr,(u_int32)val)
#define BT_WRITE_LE_UINT32(ptr,val) BT_Write_LE_uint32((u_int8 *)ptr,(u_int32)val)

#define BT_READ_LE_UINT8(ptr)       BT_Read_LE_uint8((u_int8 *)ptr)
#define BT_READ_LE_UINT16(ptr)      BT_Read_LE_uint16((u_int8 *)ptr)
#define BT_READ_LE_UINT24(ptr)      BT_Read_LE_uint24((u_int8 *)ptr)
#define BT_READ_LE_UINT32(ptr)      BT_Read_LE_uint32((u_int8 *)ptr)
*/

#endif /* PLATFORM_CONFIG_H */
