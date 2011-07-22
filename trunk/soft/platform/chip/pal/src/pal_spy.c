/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2003, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*  This source code is the property of Coolsand Technologies and is     */
/*  confidential. Any modification, distribution, reproduction or        */
/*  exploitation of any content of this file is totally forbidden,       */
/*  except with the written permission of Coolsand Technologies.         */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/** 
    @file   pal_gsm.c 
    
    @brief This file is the main PAL code file.
*/
/*                                                                       */
/*************************************************************************/

#include "pal_spy.h"


/**********************
 ** Global variables **
 **********************/
CHAR *SpyPalText = "PAL";
const CHAR *SpyPalItem[PALSPY_QTY]={
  "FCPU  %d",       /* CPU free time (in idle task) */
  "CPUIT %d",       /* CPU load (in interrupt task) */
  "wrAdj %d",       /* PAL TCU wrap adjust quantity */
  "TcuMx %d",       /* Maximum TCU number used */
  "ToTof %d",       /* Total time offset from reset */
  "ToFof %d",       /* Total freq offset from reset */
  "MonQt %d",       /* Monitoring windows quantity */
  "MonPw %d",       /* Last monitoring value */
  "FccQt %d",       /* FCCH windows quantity */
  "FcFof %d",       /* Last FCCH FOf value */
  "FcTof %d",       /* Last FCCH TOf value */
  "FcOk  %d",       /* FCCH OK quantity */
  "FcKo  %d",       /* FCCH KO quantity */
  "SchQt %d",       /* SCH windows quantity */
  "SchFo %d",       /* Last SCH FOf value */
  "SchTo %d",       /* Last SCH TOf value */
  "SchSn %d",       /* Last SCH SNR value */
  "SchOk %d",       /* SCH OK quantity */
  "SchKo %d",       /* SCH KO quantity */
  "IntQt %d",       /* Interference windows quantity */
  "NbQt  %d",       /* Normal burst windows quantity */
  "NbFof %d",       /* Last Normal burst FOf value */
  "NbTof %d",       /* Last Normal burst TOf value */
  "NbSnr %d",       /* Last Normal burst SNR value */
  "NbPwr %d",       /* Last Normal burst power value */
  "NbBer %d",       /* Last Normal burst BER value */
  "NbOk  %d",       /* Normal burst OK quantity */
  "NbKo  %d",       /* Normal burst KO quantity */
  "TxAcc %d",       /* Tx Access burst encoding quantity */
  "TxNbl %d",       /* Tx Normal burst encoding quantity */
  "TxQty %d"        /* Tx windows quantity */
};

INT16 SpyPalValues[PALSPY_QTY];
