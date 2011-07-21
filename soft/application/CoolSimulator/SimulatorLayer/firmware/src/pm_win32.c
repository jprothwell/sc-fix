/******************************************************************************/
/*                                                                            */
/*              Copyright (C) 2005, Coolsabd Technologies, Inc.               */
/*                            All Rights Reserved                             */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* This source code is property of Coolsand. The information contained in     */
/* this file is confidential. Distribution, reproduction, as well as          */
/* exploitation,or transmisison of any content of this file is not            */
/* allowed except if expressly permitted.                                     */
/* Infringements result in damage claims!                                     */
/*                                                                            */
/* FILE NAME                                                                  */
/*      pm_win32.c                                                            */
/*                                                                            */
/* DESCRIPTION                                                                */
/*      PM service declarations, constant definitions and macros.             */
/*                                                                            */
/******************************************************************************/
#include "cswtype.h"

#include "pm.h"


BOOL SRVAPI PM_GetACPowerStatus (
  UINT8* pACStatus
)
{
  return FALSE;
}

BOOL SRVAPI PM_GetBatteryState(
  PM_BATTERY_INFO* pBatInfo
)
{
  return FALSE;
}

BOOL SRVAPI PM_GetBatteryInfo (
  UINT8* pBcs,
  UINT8* pBcl,
  UINT16* pMpc
)
{
  return FALSE;
}


BOOL SRVAPI PM_BatteryChemistry (
  UINT8* pChemistryStatus
)
{
  return FALSE;
}

BOOL SRVAPI PM_SetBatteryChargeMode (
  UINT8 nMode
)
{
  return FALSE;
}

BOOL SRVAPI PM_SetPhoneFunctionality(
  UINT16 nMode,
  UINT16 nValue
)
{
  return FALSE;
}

BOOL SRVAPI PM_GetPhoneFunctionality(
  UINT16* pMode
)
{
  return FALSE;
}

UINT16 PM_GetGpadcValue(PM_GPADC_CHAN_T channel)
{
  return 0;
}

UINT8 pmd_GetEarModeStatus(VOID)
{
  return 0;
}