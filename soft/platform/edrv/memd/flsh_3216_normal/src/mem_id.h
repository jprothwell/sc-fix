////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL: http://10.10.100.14/svn/developing/modem2G/trunk/platform/edrv/memd/flsh_k5l2731caa_d770/src/memdp.h $ //
//	$Author: Jixj $                                                        // 
//	$Date: 2008-07-22 11:00:00 +0800 (Tue, 22 Jul 2007) $                     //   
//	$Revision: 7769 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file memdp.h                                                             //
/// Private interface in the flash driver
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MEMID_H_
#define _MEMID_H_

typedef enum {
  FLASH_EN71GL032A0,
  FLASH_EN71PL032A0,
  FLASH_M36COW5040TOZSP,
  FLASH_SVME3216UTA,
  FLASH_FM91L03216UA,
  FLASH_NAME_QTY
}FLASH_NAME_ID;

typedef struct
{
    UINT16  manufacturerID0;
    UINT16  manufacturerID1;
    UINT16  deviceID0;
    UINT16  deviceID1;
    UINT16  deviceID2;
    UINT16  cfiBankNum;
    FLASH_NAME_ID flashName;
} FLASH_AUTOSELECTOR_ID;


#endif // FLDP_H




