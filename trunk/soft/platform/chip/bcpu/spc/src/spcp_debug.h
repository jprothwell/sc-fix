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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/bcpu/spc/src/spcp_debug.h $
//    $Author: admin $
//    $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $
//    $Revision: 269 $
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

#ifndef _SPCP_DEBUG_H_
#define _SPCP_DEBUG_H_

#include "spal_debug.h"

// separation marker for dump
#define SPC_DBG_DUMP_EQU_IN_MARKER 0xAAAA
#define SPC_DBG_DUMP_EQU_OUT_MARKER 0xBBBB
#define SPC_DBG_DUMP_DEC_IN_MARKER 0xCCCC
#define SPC_DBG_DUMP_DEC_OUT_MARKER 0xDDDD
// GSM CCH (GPRS)
// dump equalization input: (I,Q) Marker = 0xAAAA
#define SPC_DBG_DUMP_GSM_CCH_EQU_IN 0x1
// dump equalization output: SoftBits Marker = xBBBB
#define SPC_DBG_DUMP_GSM_CCH_EQU_OUT 0x2
// dump decoder input: decoded bit Marker =  0xCCCC
#define SPC_DBG_DUMP_GSM_CCH_DEC_IN 0x4
// dump decoder output: decoded bit Marker =  0xDDDD
#define SPC_DBG_DUMP_GSM_CCH_DEC_OUT 0x8

// GSM TCH
// dump equalization input: (I,Q) Marker = 0xAAAA
#define SPC_DBG_DUMP_GSM_TCH_EQU_IN 0x10
// dump equalization output: SoftBits Marker = xBBBB
#define SPC_DBG_DUMP_GSM_TCH_EQU_OUT 0x20
// dump decoder input: decoded bit Marker =  0xCCCC
#define SPC_DBG_DUMP_GSM_TCH_DEC_IN 0x40
// dump decoder output: decoded bit Marker =  0xDDDD
#define SPC_DBG_DUMP_GSM_TCH_DEC_OUT 0x80
// EGPRS
// dump equalization input: (I,Q) Marker = 0xAAAA
#define SPC_DBG_DUMP_EGPRS_EQU_IN 0x100
// dump equalization output: SoftBits Marker = xBBBB
#define SPC_DBG_DUMP_EGPRS_EQU_OUT 0x200
// dump decoder input: decoded bit Marker =  0xCCCC
#define SPC_DBG_DUMP_EGPRS_DEC_IN 0x400
// dump decoder output: decoded bit Marker =  0xDDDD
#define SPC_DBG_DUMP_EGPRS_DEC_OUT 0x800


#ifdef SPC_FUNC_PROFILING
#define SPC_PROFILE_FUNCTION_ENTER(eventName) \
        spal_DbgPxtsProfileFunctionEnter(SPAL_DBG_PXTS_SPC, (CP_ ## eventName))

#define SPC_PROFILE_FUNCTION_EXIT(eventName) \
        spal_DbgPxtsProfileFunctionExit(SPAL_DBG_PXTS_SPC, (CP_ ## eventName))
#else
#define SPC_PROFILE_FUNCTION_ENTER(eventName)
#define SPC_PROFILE_FUNCTION_EXIT(eventName)
#endif


#ifdef SPC_WIN_PROFILING
#define SPC_PROFILE_WINDOW_ENTER(eventName) \
        spal_DbgPxtsProfileWindowEnter(SPAL_DBG_PXTS_SPC, (CP_ ## eventName))

#define SPC_PROFILE_WINDOW_EXIT(eventName) \
        spal_DbgPxtsProfileWindowExit(SPAL_DBG_PXTS_SPC, (CP_ ## eventName))
#else
#define SPC_PROFILE_WINDOW_ENTER(eventName)
#define SPC_PROFILE_WINDOW_EXIT(eventName)
#endif


#ifdef SPC_PULSE_PROFILING
#define SPC_PROFILE_IRQ_PULSE(eventName) \
        spal_DbgPxtsProfilePulse(SPAL_DBG_PXTS_SPC, (CP_ ## eventName))
#else
#define SPC_PROFILE_IRQ_PULSE(eventName)
#endif
          
           
#endif // _SPCP_DEBUG_H_

