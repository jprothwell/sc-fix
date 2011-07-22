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
//  $HeadURL$
//	$Author$
//	$Date$
//	$Revision$
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file                                                                     //
/// Debug features to be used only inside the module (protected).
/// Is this file is used for all the debug needs of the module, it will be
/// easier to disable those debug features for the whole module (by changing
/// the value of a define while compiling).
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _VPP_EFR_DEBUG_H_
#define _VPP_EFR_DEBUG_H_

#include "hal_debug.h"

#ifdef VPP_EFR_FUNC_PROFILING
#define VPP_EFR_PROFILE_FUNCTION_ENTER(eventName) \
     hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_SVC, (CP_ ## eventName))

#define VPP_EFR_PROFILE_FUNCTION_EXIT(eventName) \
     hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_SVC, (CP_ ## eventName))
#else
#define VPP_EFR_PROFILE_FUNCTION_ENTER(eventName)
#define VPP_EFR_PROFILE_FUNCTION_EXIT(eventName)
#endif


#endif // _SPCP_DEBUG_H_

