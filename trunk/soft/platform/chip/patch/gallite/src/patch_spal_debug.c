
//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "patch_spal_debug.h"


#ifdef BCPU_PROFILING

#include "hal_map_engine.h"
#include "spal_mem.h"
#include "timer.h"
#include "spal_irq.h"

// =============================================================================
// MACROS 
// =============================================================================

// We do not need to bypass the BCPU cache for buffer data,
// since the cache is configured in write-through mode.

// NOTE:
// The number of events recorded per frame should be limited in order not to impact
// BCPU normal processing.
// At present we only profile the paths in spc_SchedulerMain(), spal_IrqHandler()
// and patch_bb_prog_rx_win().
#ifdef BCPU_PROFILING_IN_EXTRAM
#define SPAL_BUFFER_PROFILE_LOCATION
#define SPAL_PXTX_BUF_SIZE	(1024)
#else // no BCPU_PROFILING_IN_EXTRAM
#define SPAL_BUFFER_PROFILE_LOCATION __attribute__((section(".srambss")))
#define SPAL_PXTX_BUF_SIZE	(512)
#endif // no BCPU_PROFILING_IN_EXTRAM

// =============================================================================
// TYPES 
// =============================================================================

typedef struct
{
    /// pxts buffer start address
    UINT32                         buffer_start;                 //0x00000000
    /// pxts buffer current position
    UINT32                         buffer_pos;                   //0x00000004
    /// pxts buffer size
    UINT32                         buffer_size;                  //0x00000008
} SPAL_PROFILE_ON_BUFFER_T; //Size : 0xC

// =============================================================================
// Globals 
// =============================================================================

EXPORT PROTECTED HAL_MAP_VERSION_T g_halMapVersion;

UINT32 g_spal_prof_buf[SPAL_PXTX_BUF_SIZE] SPAL_BUFFER_PROFILE_LOCATION =
{
    0,
};

SPAL_PROFILE_ON_BUFFER_T g_spalProfileOnBuffer SPAL_BUFFER_PROFILE_LOCATION = 
{
    .buffer_start = (UINT32)g_spal_prof_buf,
    .buffer_pos = 0,
    .buffer_size = SPAL_PXTX_BUF_SIZE
};

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
PUBLIC VOID SPAL_FUNC_INTERNAL spal_BufferProfileSendCode(SPAL_DBG_PXTS_LEVEL_T level,
                                                          UINT16 code)
{
    volatile UINT32 critical_sec;
    critical_sec = spal_IrqEnterCriticalSection();
    
	UINT32 time = hwp_timer->HWTimer_CurVal;
	if(g_spalProfileOnBuffer.buffer_pos >= SPAL_PXTX_BUF_SIZE)
	{
		g_spalProfileOnBuffer.buffer_pos = 0;
	}

	g_spal_prof_buf[g_spalProfileOnBuffer.buffer_pos++] = ((time & 0xffff) << 16) | code;
	
    spal_IrqExitCriticalSection(critical_sec);
}

#endif // BCPU_PROFILING


PUBLIC VOID spal_RegisterProfileBuffer(VOID)
{
#ifdef BCPU_PROFILING
    g_spalProfileOnBuffer.buffer_start = (UINT32)g_spal_prof_buf;
    g_spalProfileOnBuffer.buffer_pos = 0;
    g_spalProfileOnBuffer.buffer_size = SPAL_PXTX_BUF_SIZE;
    
    g_halMapVersion.date = (UINT32)&g_spalProfileOnBuffer;
#endif // BCPU_PROFILING
}

