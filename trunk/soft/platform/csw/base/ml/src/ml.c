// //////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2002-2008, Coolsand Technologies, Inc.
// All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmission of any content of this file is not allowed except with the 
// written permission of Coolsand Technologies, Inc.
// 
// FILENAME: ML.cpp
// 
// DESCRIPTION:
// TODO: ...
// 
// REVISION HISTORY:
// NAME              DATE                REMAKS
// EP                2008-1-25       Created initial version 1.0
// 
// //////////////////////////////////////////////////////////////////////////////


#include "ml.h"
#include "sul.h"

static struct ML_Table ML_table;
static struct ML_Table* pML_table = &ML_table;

BOOL g_Flag           = FALSE;
UINT8 g_Character[12] = { 0, };


UINT32 ML_RegisterTable(struct ML_Table* ML)
{
  struct ML_Table** p = &pML_table;

  if (!p)
  {
    return ERR_INVALID_HANDLE;
  }

  if (ML->next)
  {
    return ERR_BUSY_DEVICE;
  }

  if (1 == g_Flag)
  {
    return ERR_BUSY_DEVICE;
  }
  else
  {
    g_Flag = TRUE;
  }

  while (*p)
  {
    if (ML == *p)
    {
      g_Flag = FALSE;
      return ERR_BUSY_DEVICE;
    }

    p = &(*p)->next;
  }

  ML->next = pML_table;

  pML_table = ML;
  g_Flag    = FALSE;

  return ERR_SUCCESS;
}

UINT32 ML_UnRegisterTable(struct ML_Table* ML)
{
  return ERR_SUCCESS;
}

static struct ML_Table* ML_FindTable(UINT8* charset)
{
  struct ML_Table* p;

  if (NULL == charset)
  {
    return NULL;
  }
  for (p = pML_table; p; p = p->next)
  {
    if( NULL == p->charset )
    {
      return NULL;
    }
    if (!SUL_StrCaselessCompare((const char*)p->charset, (const char*)charset))
      break;
  }
  return p;
}

UINT32 ML_SetCodePage(UINT8 nCharset[12])
{
  SUL_StrCopy(g_Character, nCharset);
  return ERR_SUCCESS;
}

UINT32 ML_Unicode2LocalLanguage(const UINT8* in, UINT32 in_len, UINT8** out, UINT32* out_len,UINT8 nCharset[12])
{
  UINT32 i, o;
  //UINT16 uni = 0x0000;
  INT32 unilen, utflen;
  UINT8* result;
  struct ML_Table* pTable = NULL;
  unilen = utflen = 0x00;

  //
  //Add "\0"
  //
  int maxlen = in_len + 1;

//  CSW_TRACE(CFW_ML_TS_ID,"---> ML_Unicode2LocalLanguage()\n");
  maxlen = ALIGN(maxlen,4);

  *out = result = (UINT8*)CSW_ML_MALLOC(maxlen);
  if (!*out)
  {
    *out_len = 0;
    return ERR_INVALID_HANDLE;
  }
  SUL_MemSet8(result, 0x00, maxlen);
  //Add by lixp at 20090211 for mult task conflicting
  if( NULL != nCharset )
  {
    pTable = ML_FindTable(nCharset);
	  CSW_TC_MEMBLOCK(CFW_ML_TS_ID, (UINT8 *)nCharset,12 ,16);
  }
  else
  {
    pTable = ML_FindTable(g_Character);
	  CSW_TC_MEMBLOCK(CFW_ML_TS_ID, (UINT8 *)g_Character,12 ,16);
  }
  if( NULL == pTable )
  {
    
      goto conv_err;  
  }
  for (i = o = 0; i < in_len; i += 2, o += unilen)
  {

    /* convert from Unicode to locallanguage */
    unilen = pTable->uni2char((*(UINT16*)(&in[i])), &result[o], in_len - o);
    if (unilen < 0)
    {
      CSW_TRACE(CFW_ML_TS_ID,"Input Data[0x%x]\n",(*(UINT16*)(&in[i])));
      goto conv_err;
    }
  }
  result[o] = '\0';
  *out_len  = o;

 // CSW_TRACE(CFW_ML_TS_ID,"<--- ML_Unicode2LocalLanguage()\n");

  return ERR_SUCCESS;

conv_err:
  CSW_TRACE(CFW_ML_TS_ID,"conv_err <--- ML_Unicode2LocalLanguage()\n");
  CSW_ML_FREE(result);
  *out = NULL;
  return ERR_INVALID_HANDLE;
}

UINT32 ML_LocalLanguage2Unicode(const UINT8* in, UINT32 in_len, UINT8** out, UINT32* out_len,UINT8 nCharset[12])
{
  struct ML_Table* pTable = NULL;
  INT32 i, o;
  UINT16 uni;
  INT32 unilen, utflen;
  UINT8* result;
  unilen = utflen = 0;

  int maxlen = (3 * in_len)+1;

  CSW_TRACE(CFW_ML_TS_ID,"---> ML_LocalLanguage2Unicode()\n");

  maxlen = ALIGN(maxlen,4);
  *out = result = (UINT8*)CSW_ML_MALLOC(maxlen);
  if (!*out)
  {
    CSW_TRACE(CFW_ML_TS_ID,"ML_LocalLanguage2Unicode() cannot allocate memory\n");
    *out_len = 0;
    return ERR_INVALID_HANDLE;
  }
  SUL_MemSet8(result,0x00,maxlen);
  //Add by lixp at 20090211 for mult task conflicting
  if( NULL != nCharset )
  {
    pTable = ML_FindTable(nCharset);
  }
  else
  {
    pTable = ML_FindTable(g_Character);
  }  
  if( NULL == pTable )
  {
    
      goto conv_err;  
  }
  for (i = o = 0; i < (INT32)in_len; i += unilen, o += 2)
  {

    /* convert from locallanguage to unicode */
    unilen = pTable->char2uni((UINT8*)&in[i], in_len - i, &uni);
    if (unilen < 0)
    {
    
      goto conv_err;
    }
    result[o] = uni >> 8;
    result[o + 1] = uni & 0xFF;

  }

  result[o] = '\0';
  *out_len  = o;

  CSW_TRACE(CFW_ML_TS_ID,"<--- ML_LocalLanguage2Unicode()\n");

  return ERR_SUCCESS;

conv_err:
  CSW_TRACE(CFW_ML_TS_ID,"conv_err<--- ML_LocalLanguage2Unicode()\n");
  CSW_ML_FREE(result);
    *out = NULL;
  return ERR_INVALID_HANDLE;
}

UINT32 ML_LocalLanguage2UnicodeBigEnding(const UINT8* in, UINT32 in_len, UINT8** out, UINT32* out_len,UINT8 nCharset[12])
{
  struct ML_Table* pTable = NULL;
  INT32 i, o;
  UINT16 uni;
  INT32 unilen, utflen;
  UINT8* result;
  unilen = utflen = 0;

  int maxlen = (3 * in_len)+1;

//  CSW_TRACE(CFW_ML_TS_ID,"---> ML_LocalLanguage2UnicodeBigEnding()\n");
  maxlen = ALIGN(maxlen,4);
  *out = result = (UINT8*)CSW_ML_MALLOC(maxlen);
  if (!*out)
  {
    CSW_TRACE(CFW_ML_TS_ID,"ML_LocalLanguage2UnicodeBigEnding() cannot allocate memory\n");
    *out_len = 0;
    return ERR_INVALID_HANDLE;
  }
  SUL_MemSet8(result,0x00,maxlen);

  //Add by lixp at 20090211 for mult task conflicting
  if( NULL != nCharset )
  {
    pTable = ML_FindTable(nCharset);
  }
  else
  {
    pTable = ML_FindTable(g_Character);
  }  
  if( NULL == pTable )
  {
    
      goto conv_err;  
  }

  for (i = o = 0; i < (INT32)in_len; i += unilen, o += 2)
  {

    unilen = pTable->char2uni((UINT8*)&in[i], in_len - i, &uni);
    if (unilen < 0)
    {
      
      goto conv_err;
    }

//    CSW_TRACE(CFW_ML_TS_ID,"The uni is 0x%x\r\n",uni);
    result[o] = uni & 0xFF ;
    result[o + 1] = uni >> 8;

  }

  result[o] = '\0';
  *out_len  = o;

//  CSW_TRACE(CFW_ML_TS_ID,"<--- ML_LocalLanguage2UnicodeBigEnding()\n");

  return ERR_SUCCESS;

conv_err:
  CSW_ML_FREE(result);
  *out = NULL;
  return ERR_INVALID_HANDLE;
}
UINT32 ML_InitCP0(void);
UINT32 ML_InitCP1256(void);
UINT32 ML_InitCP1258(void);
UINT32 ML_InitCP874(void);
UINT32 ML_InitCP936(void);
UINT32 ML_InitISO8859_1(void);
UINT32 ML_InitISO8859_5(void);
UINT32 ML_InitISO8859_6(void);

UINT32 ML_Init()
{
//#ifdef ISO8859_1
  ML_InitISO8859_1();
//#endif

#ifdef CP0
  ML_InitCP0();
#endif
#ifdef CP1256
  ML_InitCP1256();
#endif
#ifdef CP1258
  ML_InitCP1258();
#endif
#ifdef CP874
  ML_InitCP874();
#endif
#ifdef CP936
  ML_InitCP936();
#endif

#ifdef ISO8859_5
  ML_InitISO8859_5();
#endif
#ifdef ISO8859_6
  ML_InitISO8859_6();
#endif
  return ERR_SUCCESS;
}

