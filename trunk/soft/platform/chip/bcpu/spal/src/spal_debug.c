//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/// @file spal_debug.c
///
/// BB debug functions. 
///
/// @date 05/02/08
/// @author Yordan Tabakov
//-----------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "cs_types.h"

#include "chip_id.h"

#include "bb_irq.h"
#include "debug_host.h"
#include "timer.h"
#include "global_macros.h"
#include "debug_port.h"
#include "spal_debug.h"



#define SPAL_DBG_HST_SEND_EVENT_DELAY 2


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------
//======================================================================
// spal_DbgPxtsDump
//----------------------------------------------------------------------
// Send data buffer to host
//======================================================================
PUBLIC VOID spal_DbgPxtsDump(UINT16* StartAdrr, UINT16 length, UINT16 Marker)
{
    UINT16* data=StartAdrr;
    UINT16 i;

    // write the Marker twice
#ifndef CHIP_HAS_PXTS_LEVELS // Granite test chip      
    hwp_debugPort->Pxts_tag_cfg = Marker;
    hwp_debugPort->Pxts_tag_cfg = Marker;
#else
    hwp_debugPort->Pxts_tag[SPAL_DBG_PXTS_DUMP] = Marker;
    hwp_debugPort->Pxts_tag[SPAL_DBG_PXTS_DUMP] = Marker;
#endif

    for (i=0;i<length;i++)
    {
#ifndef CHIP_HAS_PXTS_LEVELS // Granite test chip      
        hwp_debugPort->Pxts_tag_cfg = *data;
#else
        hwp_debugPort->Pxts_tag[SPAL_DBG_PXTS_DUMP] = *data;
#endif
        data++;
    }
    
    // write the Marker twice
#ifndef CHIP_HAS_PXTS_LEVELS // Granite test chip      
    hwp_debugPort->Pxts_tag_cfg = Marker;
    hwp_debugPort->Pxts_tag_cfg = Marker;
#else
    hwp_debugPort->Pxts_tag[SPAL_DBG_PXTS_DUMP] = Marker;
    hwp_debugPort->Pxts_tag[SPAL_DBG_PXTS_DUMP] = Marker;
#endif
 
}
//======================================================================
// spal_DbgHstSendEvent
//----------------------------------------------------------------------
// Send event to host
//======================================================================
PUBLIC BOOL spal_DbgHstSendEvent(UINT32 ch)
{
    UINT32 status       =   hwp_bbIrq->SC;
    UINT32 startTime    =   hwp_timer->HWTimer_CurVal;
    BOOL   timeOut      =   FALSE;

    //  wait for host sema to be free or timeout
    while(hwp_debugHost->event == 0 && !timeOut)
    {
        if ((hwp_timer->HWTimer_CurVal - startTime) >= SPAL_DBG_HST_SEND_EVENT_DELAY)
        {
            timeOut = TRUE;
        }
    }
    
    //  Write the byte in the host event register if possible
    if (!timeOut)
    {
        hwp_debugHost->event = ch;
    }

    hwp_bbIrq->SC = status;
    return (!timeOut);
}

