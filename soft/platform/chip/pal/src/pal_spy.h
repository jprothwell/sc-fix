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
/* FILE NAME                                                             */
/*      pal_spy.h                                                        */
/*                                                                       */
/* DESCRIPTION                                                           */
/*      This file is header for the PAL spy functions.           */
/*                                                                       */
/*************************************************************************/

#ifndef PAL_SPY_H
#define PAL_SPY_H

#include "cs_types.h"

enum palSpyId {
  PALSPY_CPU_FREE,
  PALSPY_CPU_IT,
  PALSPY_WRAP_ADJUST,
  PALSPY_MAX_TCU_VALUE,
  PALSPY_TOTAL_TIME_OFFSET,
  PALSPY_TOTAL_FREQUENCY_OFFSET,
  PALSPY_MON_WIN_QTY,
  PALSPY_MON_VALUE,
  PALSPY_FCCH_WIN_QTY,
  PALSPY_FCCH_FOF_VALUE,
  PALSPY_FCCH_TOF_VALUE,
  PALSPY_FCCH_OK_QTY,
  PALSPY_FCCH_KO_QTY,
  PALSPY_SCH_WIN_QTY,
  PALSPY_SCH_FOF_VALUE,
  PALSPY_SCH_TOF_VALUE,
  PALSPY_SCH_SNR_VALUE,
  PALSPY_SCH_OK_QTY,
  PALSPY_SCH_KO_QTY,
  PALSPY_INTER_WIN_QTY,
  PALSPY_NBURST_WIN_QTY,
  PALSPY_NBURST_FOF_VALUE,
  PALSPY_NBURST_TOF_VALUE,
  PALSPY_NBURST_SNR_VALUE,
  PALSPY_NBURST_PWR_VALUE,
  PALSPY_NBURST_BER_VALUE,
  PALSPY_NBURST_OK_QTY,
  PALSPY_NBURST_KO_QTY,
  PALSPY_TX_ABURST_QTY,
  PALSPY_TX_NBLOCK_QTY,
  PALSPY_TX_WIN_QTY,

  PALSPY_QTY
};

#ifdef PAL_SPY
#define PALSPY_AFFECT(id,val)               (SpyPalValues[(id)]=(val))
#define PALSPY_ADD(id,val)              (SpyPalValues[(id)]+=(val))
#else
#define PALSPY_AFFECT(id,val)
#define PALSPY_ADD(id,val)
#endif

extern INT16 SpyPalValues[];
extern CHAR *SpyPalText;
extern const CHAR *SpyPalItem[];

#endif /* PAL_SPY_H */
