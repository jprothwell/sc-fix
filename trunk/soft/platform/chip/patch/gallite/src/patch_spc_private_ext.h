/******************************************************************************/
/*                  Copyright (C) 2003, Coolsand Technologies, Inc.           */
/*                               All Rights Reserved                          */
/******************************************************************************/
/* Filename:  patch_spp_private_ext.h                                                   */
/* Description:                                                               */
/*   Header for extra private SPC functions and definitions                         */
/*                                                                            */
/******************************************************************************/
#ifndef SPC_PRIVATE_EXT_H
#define SPC_PRIVATE_EXT_H

#include "cs_types.h"

/*************************************************
             SPC PRIVATE functions
 *************************************************/
//=============================================================================
// spc_RxProcessExt()
//-----------------------------------------------------------------------------
/// This function executes the extended Rx for the current frame 
//=============================================================================
PROTECTED VOID SPAL_FUNC_INTERNAL spc_RxProcessExt(VOID);

//=============================================================================
// spc_UpdateOnFintExt()
//-----------------------------------------------------------------------------
/// This function updates all the external context variables that change on FINT
//=============================================================================

PROTECTED VOID SPAL_FUNC_INTERNAL spc_UpdateOnFintExt(VOID);

#endif // SPC_PRIVATE_EXT_H
