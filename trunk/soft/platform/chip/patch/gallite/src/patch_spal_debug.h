//-----------------------------------------------------------------------------
//                                                                            //
/// @file patch_spal_debug.h
/// That file contains the profiling mechanisms implementation.
//                                                                            //
//-----------------------------------------------------------------------------

#ifndef PATCH_SPAL_DEBUG_H
#define PATCH_SPAL_DEBUG_H

#include "cs_types.h"

#include "chip_id.h"

#include "global_macros.h"
#include "debug_port.h"



// =============================================================================
//  MACROS
// =============================================================================

#define CPMASK 0x3fff
#define CPEXITFLAG 0x8000
#define CPFINT 0x7fff

#define CPBCPU 0x4000 // hardcoded in spal_PXTS_SendProfilingCode function

#define CPPULSESSPACESTART 0x2f00
#define CPPULSESSPACEMASK 0x7ff

#define CPWINDOWSPACESTART 0x3700
#define CPWINDOWSPACEMASK 0x7ff

#define CPTASKSPACESTART 0x3f00
#define CPTASKSPACEMASK 0x1f

#define CPIRQSPACESTART 0x3f20
#define CPIRQSPACEMASK 0x1f

#define CPJOBPACESTART 0x3f40
#define CPJOBSPACEMASK 0x1f

// =============================================================================
//  BB IRQ defines for profiling
// =============================================================================
#define BB_IRQ_OTHER    0xF


// SPAL DEBUG PROFILING FUNCTIONS

///@defgroup spal_debug SPAL Debug Functions
///@{



// =============================================================================
// TYPES 
// =============================================================================


// =============================================================================
// SPAL_DBG_PXTS_LEVEL_T
// -----------------------------------------------------------------------------
/// The PXTS level PXTS level on which to send the code. This level is used to
/// filter the PXTS whithout having to recompile. Generally, each software
/// module has a level attributed. 
// =============================================================================
typedef enum
{
    SPAL_DBG_PXTS_RESERVED_0  = 0,
    SPAL_DBG_PXTS_RESERVED_1  = 1,
    SPAL_DBG_PXTS_RESERVED_2  = 2,
    SPAL_DBG_PXTS_RESERVED_3  = 3,
    SPAL_DBG_PXTS_RESERVED_4  = 4,
    SPAL_DBG_PXTS_RESERVED_5  = 5,
    SPAL_DBG_PXTS_RESERVED_6  = 6,
    SPAL_DBG_PXTS_RESERVED_7  = 7,
    SPAL_DBG_PXTS_RESERVED_8  = 8,
    SPAL_DBG_PXTS_RESERVED_9  = 9,
    SPAL_DBG_PXTS_RESERVED_10 = 10,
    SPAL_DBG_PXTS_DUMP        = 11,
    SPAL_DBG_PXTS_BB_IRQ      = 12,
    SPAL_DBG_PXTS_SPAL        = 13,
    SPAL_DBG_PXTS_SPP         = 14,
    SPAL_DBG_PXTS_SPC         = 15,
} SPAL_DBG_PXTS_LEVEL_T;



#ifdef BCPU_PROFILING

// =============================================================================
// FUNCTIONS
// =============================================================================


// =============================================================================
// spal_BufferProfileSendCode
// -----------------------------------------------------------------------------
/// Profiling tool (reserved for Platform profiling). To use this feature, 
/// the PXTS device must have been enabled on the debug bus (spal_DbgPortSetup)
/// This function is reserved for platform profiling. It is used to send 
/// a serie of event to a profiling tool (like CSBJ SA Tool) through the
/// romulator. \n
/// When this function is called, it sends an event represented by its \c code
/// to the PXTS Fifo. This Fifo is read by en external tool. \n
/// \n
/// (Nota: SA tool and Coolprofile cannot be used at the same time)
/// 
/// @param code The event code to send to the profiling tool
/// @param level PXTS level on which to send the code.
// =============================================================================
PUBLIC VOID spal_BufferProfileSendCode(SPAL_DBG_PXTS_LEVEL_T level,
                                       UINT16 code);



// =============================================================================
// spal_BufferProfileFunctionEnter
// -----------------------------------------------------------------------------
/// This function has to be called when entering the function you want to profile.
/// 
/// @param functionId Representing the function.
/// @param level PXTS level on which to send the code.
// =============================================================================
__inline__ VOID spal_BufferProfileFunctionEnter(SPAL_DBG_PXTS_LEVEL_T level,
                                            UINT16 functionId)
{
    spal_BufferProfileSendCode(level, (functionId & CPMASK) | CPBCPU);
}



// =============================================================================
// spal_BufferProfileFunctionExit
// -----------------------------------------------------------------------------
/// This function has to be called when exiting the function you want to profile.
///
/// @param functionId Code representing the function.
/// @param level PXTS level on which to send the code.
// =============================================================================
__inline__ VOID spal_BufferProfileFunctionExit(SPAL_DBG_PXTS_LEVEL_T level,
                                           UINT16 functionId)
{
    spal_BufferProfileSendCode(level, (functionId & CPMASK) | CPEXITFLAG | CPBCPU);
}



// =============================================================================
// spal_BufferProfileWindowEnter
// -----------------------------------------------------------------------------
/// This function has to be called to mark the entrance in a window.
/// 
/// @param windowId Code representing the window which has been entered in.
/// @param level PXTS level on which to send the code.
// =============================================================================
__inline__ VOID spal_BufferProfileWindowEnter(SPAL_DBG_PXTS_LEVEL_T level,
                                          UINT16 windowId)
{
    spal_BufferProfileSendCode(level, (windowId & CPMASK) | CPBCPU);
}



// =============================================================================
// spal_BufferProfileWindowExit
// -----------------------------------------------------------------------------
/// This function has to be called to mark the exit of a window.
/// 
/// @param windowId Code representing the window which has been exited.
/// @param level PXTS level on which to send the code.
// =============================================================================
__inline__ VOID spal_BufferProfileWindowExit(SPAL_DBG_PXTS_LEVEL_T level,
                                         UINT16 windowId)
{
    spal_BufferProfileSendCode(level, (windowId & CPMASK) | CPEXITFLAG | CPBCPU);
}



// =============================================================================
// spal_BufferProfileTaskEnter
// -----------------------------------------------------------------------------
/// Send to the profiling tool the code corresponding to scheduled task Id.
/// 
/// @param taskId Task id of the scheduled task.
/// @param level PXTS level on which to send the code.
// =============================================================================
__inline__ VOID spal_BufferProfileTask(SPAL_DBG_PXTS_LEVEL_T level,
                                   UINT16 taskId)
{
    spal_BufferProfileSendCode(level, CPTASKSPACESTART | ((taskId) & CPTASKSPACEMASK) | CPBCPU);
}


// =============================================================================
// spal_BufferProfilePulse
// -----------------------------------------------------------------------------
/// Send to the profiling tool the code correponding to a pulse.
/// 
/// @param code Code representing the pulse
/// @param level PXTS level on which to send the code.
// =============================================================================
__inline__ VOID spal_BufferProfilePulse(SPAL_DBG_PXTS_LEVEL_T level,
                                    UINT16 code)
{
    spal_BufferProfileSendCode(level, (code & CPMASK) | CPBCPU);
}

// SPAL
#define SPAL_BUFFER_PROFILE_FUNCTION_ENTER(eventName) \
        spal_BufferProfileFunctionEnter(SPAL_DBG_PXTS_SPAL, (CP_ ## eventName))

#define SPAL_BUFFER_PROFILE_FUNCTION_EXIT(eventName) \
        spal_BufferProfileFunctionExit(SPAL_DBG_PXTS_SPAL, (CP_ ## eventName))

#define SPAL_BUFFER_PROFILE_WINDOW_ENTER(eventName) \
        spal_BufferProfileWindowEnter(SPAL_DBG_PXTS_SPAL, (CP_ ## eventName))

#define SPAL_BUFFER_PROFILE_WINDOW_EXIT(eventName) \
        spal_BufferProfileWindowExit(SPAL_DBG_PXTS_SPAL, (CP_ ## eventName))

#define SPAL_BUFFER_PROFILE_PULSE(pulseName)  \
        spal_BufferProfilePulse(SPAL_DBG_PXTS_SPAL, (CP_ ## pulseName))

#define SPAL_BUFFER_PROFILE_IRQ_ENTER(eventName) \
        spal_BufferProfileSendCode(SPAL_DBG_PXTS_BB_IRQ, \
        (CPIRQSPACESTART |((eventName) & CPIRQSPACEMASK)) | CPBCPU)
#define SPAL_BUFFER_PROFILE_IRQ_EXIT(eventName) \
        spal_BufferProfileSendCode(SPAL_DBG_PXTS_BB_IRQ, \
        (CPIRQSPACESTART | ((eventName) & CPIRQSPACEMASK) | CPEXITFLAG)| CPBCPU)

#define SPAL_BUFFER_PROFILE_FINT spal_BufferProfileSendCode(SPAL_DBG_PXTS_BB_IRQ, CPFINT)

#define SPAL_BUFFER_PROFILE_WINDOW_ENTER_RAW(event) \
        spal_BufferProfileWindowEnter(SPAL_DBG_PXTS_SPAL, (event))

#define SPAL_BUFFER_PROFILE_WINDOW_EXIT_RAW(event) \
        spal_BufferProfileWindowExit(SPAL_DBG_PXTS_SPAL, (event))

// SPC
#define SPC_BUFFER_PROFILE_FUNCTION_ENTER(eventName) \
        spal_BufferProfileFunctionEnter(SPAL_DBG_PXTS_SPC, (CP_ ## eventName))

#define SPC_BUFFER_PROFILE_FUNCTION_EXIT(eventName) \
        spal_BufferProfileFunctionExit(SPAL_DBG_PXTS_SPC, (CP_ ## eventName))

#define SPC_BUFFER_PROFILE_WINDOW_ENTER(eventName) \
        spal_BufferProfileWindowEnter(SPAL_DBG_PXTS_SPC, (CP_ ## eventName))

#define SPC_BUFFER_PROFILE_WINDOW_EXIT(eventName) \
        spal_BufferProfileWindowExit(SPAL_DBG_PXTS_SPC, (CP_ ## eventName))

#define SPC_BUFFER_PROFILE_IRQ_PULSE(eventName) \
        spal_BufferProfilePulse(SPAL_DBG_PXTS_SPC, (CP_ ## eventName))

#else // no BCPU_PROFILING

// SPAL
#define SPAL_BUFFER_PROFILE_FUNCTION_ENTER(eventName)
#define SPAL_BUFFER_PROFILE_FUNCTION_EXIT(eventName)
#define SPAL_BUFFER_PROFILE_WINDOW_ENTER(eventName)
#define SPAL_BUFFER_PROFILE_WINDOW_EXIT(eventName)
#define SPAL_BUFFER_PROFILE_PULSE(pulseName)
#define SPAL_BUFFER_PROFILE_IRQ_ENTER(eventName)
#define SPAL_BUFFER_PROFILE_IRQ_EXIT(eventName)
#define SPAL_BUFFER_PROFILE_FINT
#define SPAL_BUFFER_PROFILE_WINDOW_ENTER_RAW(event)
#define SPAL_BUFFER_PROFILE_WINDOW_EXIT_RAW(event)

// SPC
#define SPC_BUFFER_PROFILE_FUNCTION_ENTER(eventName) 
#define SPC_BUFFER_PROFILE_FUNCTION_EXIT(eventName) 
#define SPC_BUFFER_PROFILE_WINDOW_ENTER(eventName)
#define SPC_BUFFER_PROFILE_WINDOW_EXIT(eventName)
#define SPC_BUFFER_PROFILE_IRQ_PULSE(eventName)

#endif // no BCPU_PROFILING

///@}


#endif // PATCH_SPAL_DEBUG_H

