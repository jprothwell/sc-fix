// ---------------------------------------------------------------------------
// RDA Microelectronics, Inc. Company Confidential Strictly Private
//
// $RCSfile: rdamtv_fm.h,v $
// $Revision: 0 $
// $Author: wuhp $
// $Date: 2009-10-21 $
//
// ---------------------------------------------------------------------------
// >>>>>>>>>>>>>>>>>>>>>>>>> COPYRIGHT NOTICE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ---------------------------------------------------------------------------
// Copyright 2008-2009 (c) RDA Microelectronics, Inc.
//
// RDA owns the sole copyright to this software. Under international
// copyright laws you (1) may not make a copy of this software except for
// the purposes of maintaining a single archive copy, (2) may not derive
// works herefrom, (3) may not distribute this work to others. These rights
// are provided for information clarification, other restrictions of rights
// may apply as well.
//
// This is an unpublished work.
// ---------------------------------------------------------------------------
#ifndef __RDAMTV_FM_H__
#define __RDAMTV_FM_H__
#include "rdatypes.h"
#include "kal_non_specific_general_types.h"

void FMDrv_PowerOnReset(void);
void FMDrv_PowerOffProc(void);
void FMDrv_SetFreq(signed short curf);
kal_uint8 FMDrv_GetSigLvl(kal_int16 curf);
void FMDrv_FineTune(kal_int16 freq);
kal_uint8 FMDrv_ValidStop(kal_int16 freq, kal_int8 signalvl, kal_bool is_step_up);
void FMDrv_Mute(kal_uint8 mute);
void FMDrv_ChipInit(void);
kal_bool FMDrv_IsChipValid(void);

#endif
