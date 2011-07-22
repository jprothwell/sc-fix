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

#include "cfw.h"

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

#ifdef ML_SUPPORT_CP936
#define CP936 
#endif

#ifdef ML_SUPPORT_CP0
#define CP0
#endif

#ifdef ML_SUPPORT_CP1256
#define CP1256
#endif

#ifdef ML_SUPPORT_CP1258
#define CP1258
#endif

#ifdef ML_SUPPORT_CP874
#define CP874
#endif

#ifdef ML_SUPPORT_ISO8859_1
#define ISO8859_1
#endif

#ifdef ML_SUPPORT_ISO8859_5
#define ISO8859_5
#endif

#ifdef ML_SUPPORT_ISO8859_6
#define ISO8859_6
#endif

//#define ML_DEF

#endif
 

