// //////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2002-2008, Coolsand Technologies, Inc.
// All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmission of any content of this file is not allowed except with the 
// written permission of Coolsand Technologies, Inc.
// 
// FILENAME: ML.H
// 
// DESCRIPTION:
// TODO: ...
// 
// REVISION HISTORY:
// NAME              DATE                REMAKS
// EP                2008-1-25       Created initial version 1.0
// 
// //////////////////////////////////////////////////////////////////////////////
#ifndef __ML_H__
#define __ML_H__
#if 0
#define WIN32_DEBUG
#define CSW_ML_MALLOC malloc
#define CSW_ML_FREE   free
typedef unsigned short UINT16;
typedef long INT32;
typedef unsigned long UINT32;
typedef unsigned char UINT8;
typedef char BOOL;
typedef void VOID;

#define FALSE  0x00;
#define TRUE   0x01;
#define ERR_SUCCESS 0x0000
#define ERR_INVALID_HANDLE       0x0003
#endif
#include "cfw.h"
#define ERR_ML_NAME_TOO_LONG 0x0001
#define ERR_ML_INVALID_CHARACTER 0x0002

//#define ERR_BUSY_DEVICE       0x0004

struct ML_Table
{
  UINT8* charset;
  INT32 (*uni2char) (UINT16 uni, UINT8 * out, UINT32 boundlen);
  INT32 (*char2uni) (UINT8 * rawstring, UINT32 boundlen, UINT16 * uni);
  UINT8* charset2lower;
  UINT8* charset2upper;
  struct ML_Table* next;
};
UINT32 ML_RegisterTable(struct ML_Table* ML);
UINT32 ML_UnRegisterTable(struct ML_Table* ML);
UINT32 ML_LocalLanguage2Unicode(const UINT8* in, UINT32 in_len, UINT8** out, UINT32* out_len);
UINT32 ML_Unicode2LocalLanguage(const UINT8* in, UINT32 in_len, UINT8** out, UINT32* out_len);
UINT32 ML_LocalLanguage2UnicodeBigEnding(const UINT8* in, UINT32 in_len, UINT8** out, UINT32* out_len);
//#define ALIGN(val,exp)      (((val) + ((exp)-1)) & ~((exp)-1))
//#define CP936
#define CP0
#if 0
enum E_LAN_ID
{
  CP0 = 0,
  CP1256,
  CP1258,
  CP874,
  CP936,
  ISO8859_1,
  ISO8859_5,
  ISO8859_6
};

typedef struct _ML_CFG{
  enum E_LAN_ID eLanId;
  BOOL bStatus;  
}ML_CFG;

extern ML_CFG g_MLCfg[];
#define LAN_STATUS(LanId) ((g_MLCfg + LanId).bStatus)  


// demo end.
#endif
#define ML_DEF
 #endif
