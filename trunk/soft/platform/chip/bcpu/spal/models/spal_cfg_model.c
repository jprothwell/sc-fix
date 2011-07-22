//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spal_cfg.c
///
/// This file provides the configuration of spal.   
///
/// @date 29/01/08
/// @author Nadia Touliou
//-----------------------------------------------------------------------------


#include "spal_cfg.h"


SPAL_GLOBAL_PARAMS_T g_spalCfg;


PUBLIC VOID spal_Open (SPAL_GLOBAL_PARAMS_T* pCfg)
{
    g_spalCfg = *pCfg;
}



PROTECTED BOOL spal_CfgIsUL (VOID)
{
    return g_spalCfg.simuUL;
}

