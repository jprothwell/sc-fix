////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2008, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/boot/greenstone/src/boot_usb_monitor.c $
//  $Author: admin $
//  $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $
//  $Revision: 269 $
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file boot_usb_monitor.c
///
/// Boot usb monitor function
//
////////////////////////////////////////////////////////////////////////////////


// =============================================================================
// HEADERS                                                                      
// =============================================================================

#include "hal_sys.h"
#include "boot_usb.h"
#include "global_macros.h"
#include "sys_irq.h"
#include "bootp_debug.h"
#include "boot_usb_monitor.h"
#include "boot_host.h"
#include "debug_host_internals.h"
#include "boot_host.h"
#include "bootp_host.h"
#include "ana_acco.h"
#include "sys_ctrl.h"
#include "cfg_regs.h"
#include "bootp_usb.h"
#include "timer.h"


#include <string.h>

// =============================================================================
// MACROS                                                                       
// =============================================================================

#ifndef BOOT_USB_SEND_EVENT_DELAY
#define BOOT_USB_SEND_EVENT_DELAY    8192 // 0.5s
#endif /* BOOT_USB_SEND_EVENT_DELAY */

// =============================================================================
// TYPES                                                                        
// =============================================================================

// =============================================================================
// BOOT_HOST_USB_HEADER_T
// -----------------------------------------------------------------------------
/// Header of flow
// =============================================================================
typedef struct
{
    UINT16 size;
    UINT8  flowId;
} PACKED BOOT_HOST_USB_HEADER_T;

// =============================================================================
// BOOT_HOST_USB_CMD_RW_T
// -----------------------------------------------------------------------------
/// RW Command value
// =============================================================================
typedef enum
{
    BOOT_HOST_USB_CMD_RW_WRITE = 1,
    BOOT_HOST_USB_CMD_RW_READ  = 0
} BOOT_HOST_USB_CMD_RW_T;

// =============================================================================
// PRIVATE VARIABLES                                                            
// =============================================================================

// Those variables will not be initialized as they are used in the Boot code,
// and the init of the variables is done in the Boot Loader.

// Callback
PRIVATE BOOT_HOST_USB_CALLBACK_T     g_BootHostUsbCallbackOs;

PRIVATE BOOT_HOST_USB_MONITOR_VAR_T  g_BootHostUsbVar;

// This variable will be initialized because it is const and will stay in ROM.
PRIVATE CONST BOOT_HOST_USB_MONITOR_VAR_T CONST *
__attribute__((section (".usbmonitorFixptr"),used))
                                     g_BootHostUsbFixPtr = &g_BootHostUsbVar;

// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================


// =============================================================================
// PRIVATE FUNCTIONS                                                            
// =============================================================================

PRIVATE UINT8 boot_HostUsbDecode(UINT8 *data, UINT32 size);
PRIVATE HAL_USB_CALLBACK_RETURN_T
boot_HostUsbCallback(HAL_USB_CALLBACK_EP_TYPE_T   type,
                     HAL_USB_SETUP_T*             setup);
PRIVATE VOID boot_HostUsbGenerateReadPacket(VOID);
PRIVATE VOID boot_HostUsbSendReadPacket(VOID);
PUBLIC VOID boot_HostUsbInit(VOID);
PUBLIC VOID boot_HostUsbEvent(UINT32 event);

// ============================================================================
// boot_HostUsbDecode
// -----------------------------------------------------------------------------
/// This function decode the host protocol.
/// For read access this function store all read command in FIFO. If fifo is
/// full RecvState is switch to #BOOT_HOST_USB_RECV_STATE_WAIT
/// For write access this function write directly in memory
/// For execution this function emulate H2P_STATUS and CTRL internal register
/// host and lauch target execute
/// @param data Data you want decode
/// @param size Size of this data
// =============================================================================
PRIVATE UINT8 boot_HostUsbDecode(UINT8 *data, UINT32 size)
{
    BOOT_PROFILE_FUNCTION_ENTER(boot_HostUsbDecode);

    BOOT_HOST_USB_HEADER_T*            header;
    UINT32                             pos;
    UINT8                              fifoUse;
    UINT32                             regTmp;
    UINT8                              indexW;
    BOOT_HOST_USB_MONITOR_FIFO_ELEM_T* curFifoElem;
    UINT8                              exitStatus = 0;

    fifoUse = g_BootHostUsbVar.WritePt-g_BootHostUsbVar.ReadPt;

    pos                        = 0;
    g_BootHostUsbVar.Residue   = 0;
    g_BootHostUsbVar.RecvState = BOOT_HOST_USB_RECV_STATE_RUN;

    while(pos + g_BootHostUsbVar.Residue < size)
    {
        if(fifoUse == BOOT_HOST_USB_FIFO_READ_SIZE)
        {
            g_BootHostUsbVar.Residue   = size-pos;
            g_BootHostUsbVar.RecvState = BOOT_HOST_USB_RECV_STATE_WAIT;
            break;
        }
        switch(g_BootHostUsbVar.State)
        {
        default:
        case BOOT_HOST_USB_STATE_IDLE:
            if(data[pos] == 0x0AD)
            {
                g_BootHostUsbVar.State = BOOT_HOST_USB_STATE_HEADER;
            }
            pos++;
            break;
        case BOOT_HOST_USB_STATE_HEADER:
            if((size - pos) < 3)
            {
                g_BootHostUsbVar.Residue = size - pos;
            }
            else
            {
                header                   = (BOOT_HOST_USB_HEADER_T*) &data[pos];
                g_BootHostUsbVar.DataSize   = HAL_ENDIAN_BIG_16(header->size);
                if(header->flowId == 0xFF)
                {
                    g_BootHostUsbVar.State  = BOOT_HOST_USB_STATE_SXS_CMD;
                }
                else
                {
                    g_BootHostUsbVar.State  = BOOT_HOST_USB_STATE_DATA;
                }
                pos                        += 3;
            }
            break;
        case BOOT_HOST_USB_STATE_DATA:
            if((size - pos) < g_BootHostUsbVar.DataSize)
            {
                g_BootHostUsbVar.DataSize -= size - pos;
                pos                        = size;
            }
            else
            {
                pos                       += g_BootHostUsbVar.DataSize;
                g_BootHostUsbVar.DataSize  = 0;
                g_BootHostUsbVar.State     = BOOT_HOST_USB_STATE_IDLE;
            }
            break;
        case BOOT_HOST_USB_STATE_SXS_CMD:
            g_BootHostUsbVar.Cmd          = *((BOOT_HOST_USB_CMD_T*)&data[pos]);
            pos++;
            g_BootHostUsbVar.DataSize--;
            g_BootHostUsbVar.State        = BOOT_HOST_USB_STATE_SXS_ADDR;
            break;
        case BOOT_HOST_USB_STATE_SXS_ADDR:
            if((size - pos) < 4)
            {
                g_BootHostUsbVar.Residue  = size - pos;
            }
            else
            {
                memcpy(&g_BootHostUsbVar.Addr, (VOID*)&data[pos],
                       sizeof(UINT32));
                pos        += 4;
                switch(g_BootHostUsbVar.Cmd.rw)
                {
                case BOOT_HOST_USB_CMD_RW_WRITE:
                    g_BootHostUsbVar.State     = BOOT_HOST_USB_STATE_SXS_DATA;
                    g_BootHostUsbVar.DataSize -= 4;
                    break;
                case BOOT_HOST_USB_CMD_RW_READ:
                    g_BootHostUsbVar.State     = BOOT_HOST_USB_STATE_SXS_RID;
                    g_BootHostUsbVar.DataSize -= 4;
                    break;
                }
            }
             break;
        case BOOT_HOST_USB_STATE_SXS_DATA:
            if(g_BootHostUsbVar.Cmd.internal &&
               g_BootHostUsbVar.Cmd.rw == BOOT_HOST_USB_CMD_RW_WRITE)
                // Internal Write
            {
                BOOT_PROFILE_FUNCTION_ENTER(boot_HostUsbWriteInternal);
                if((size - pos) < ((UINT32)g_BootHostUsbVar.DataSize - 1))
                { // Not enougth data
                    g_BootHostUsbVar.Residue = size - pos;
                }
                else
                {
                    g_BootHostUsbVar.State     = BOOT_HOST_USB_STATE_DATA;

                    // Emulate H2P_STATUS and CTRL_SET internal register
                    switch(g_BootHostUsbVar.Addr)
                    {
                    case (UINT32)(&hwp_intRegDbgHost->CTRL_SET)&0x1FFFFFFF:
                        regTmp  = data[pos];
                        pos    += g_BootHostUsbVar.DataSize - 1;
                        g_BootHostUsbVar.DataSize = 1;
                        if(regTmp & DEBUG_HOST_INTERNAL_REGISTERS_DEBUG_RESET)
                        {
                            hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_UNLOCK;
                            hwp_sysCtrl->Sys_Rst_Set = SYS_CTRL_SOFT_RST;
                        }
                        break;
                    case (UINT32)(&hwp_intRegDbgHost->H2P_STATUS)&0x1FFFFFFF:
                        // Save current state
                        regTmp                    = data[pos];
                        pos                      += g_BootHostUsbVar.DataSize-1;
                        g_BootHostUsbVar.Residue  = size - pos;
                        if(g_BootHostUsbCallbackOs != 0)
                        {
                            g_BootHostUsbCallbackOs(data, pos);
                        }
                        if(g_BootHostUsbVar.Residue)
                        {
                            memmove((UINT32*)HAL_SYS_GET_UNCACHED_ADDR(g_BootHostUsbVar.Context->BufferOut),
                                    (UINT32*)HAL_SYS_GET_UNCACHED_ADDR(&g_BootHostUsbVar.Context->BufferOut[pos]),
                                    g_BootHostUsbVar.Residue);
                        }
                        g_BootHostUsbVar.DataSize  = 1;
                        pos                        = 0;
                        switch(regTmp)
                        {
                        case BOOT_HST_MONITOR_START_CMD:
                            boot_HostUsbEvent(BOOT_HST_MONITOR_START);
                            break;
                        case BOOT_HST_MONITOR_END_CMD:
                            boot_HostUsbEvent(BOOT_HST_MONITOR_END);
                            exitStatus = 1;
                            break;
                        case BOOT_HST_MONITOR_X_CMD:
                            boot_HostUsbEvent(BOOT_HST_START_CMD_TREATMENT);
                            switch(boot_HstMonitorXCtx.cmdType)
                            {
                            case BOOT_HST_MONITOR_X_CMD:
// Execute at the PC in the execution context with the provided SP
                                boot_HstMonitorX();
                                break;
                            default:
                                if (boot_HstMonitorExtendedHandler.
                                    ExtendedCmdHandler != 0)
                                {
                                    ((VOID(*)(VOID*))boot_HstMonitorExtendedHandler.
                                     ExtendedCmdHandler)((VOID*)&boot_HstMonitorXCtx);
                                }
                            }
                            boot_HostUsbEvent(BOOT_HST_END_CMD_TREATMENT);
                            break;
                        default:
                            boot_HostUsbEvent(BOOT_HST_UNSUPPORTED_CMD);
                            break;
                        }
                    default:
                        pos += g_BootHostUsbVar.DataSize - 1;
                        g_BootHostUsbVar.DataSize = 1;
                        break;
                    }
                    // Restore current state
                    g_BootHostUsbVar.Residue = 0;
                }
                BOOT_PROFILE_FUNCTION_EXIT(boot_HostUsbWriteInternal);
            }
            else // Normal RW
            {
                if((size - pos) < ((UINT32)g_BootHostUsbVar.DataSize - 1))
                {
                    memcpy((VOID*)g_BootHostUsbVar.Addr, &data[pos],
                           size - pos);
                    g_BootHostUsbVar.Addr     += size - pos;
                    g_BootHostUsbVar.DataSize -= size - pos;
                    pos                        = size;
                }
                else
                {
                    memcpy((VOID*)g_BootHostUsbVar.Addr, &data[pos],
                           g_BootHostUsbVar.DataSize-1);
                    pos                       += g_BootHostUsbVar.DataSize-1;
                    g_BootHostUsbVar.DataSize  = 1;
                    g_BootHostUsbVar.State     = BOOT_HOST_USB_STATE_DATA;
                }
            }
            break;
        case BOOT_HOST_USB_STATE_SXS_RID:
            if(data[pos] == 0) // Event
            {
                pos++;
                g_BootHostUsbVar.DataSize--;
                g_BootHostUsbVar.State = BOOT_HOST_USB_STATE_DATA;
                break;
            }
            indexW      = g_BootHostUsbVar.WritePt%BOOT_HOST_USB_FIFO_READ_SIZE;
            curFifoElem = &g_BootHostUsbVar.Context->Fifo[indexW];
            curFifoElem->Rid  = data[pos];
            curFifoElem->Addr = g_BootHostUsbVar.Addr;
            switch(g_BootHostUsbVar.Cmd.access) {
            case BOOT_HOST_USB_CMD_ACCESS_BYTE     :
                curFifoElem->Size = 1;
                break;
            case BOOT_HOST_USB_CMD_ACCESS_HALF_WORD:
                curFifoElem->Size = 2;
                break;
            case BOOT_HOST_USB_CMD_ACCESS_WORD     :
                curFifoElem->Size = 4;
                break;
            case BOOT_HOST_USB_CMD_ACCESS_BLOCK    :
                g_BootHostUsbVar.State = BOOT_HOST_USB_STATE_SXS_READ_SIZE;
                break;
            }

            if(g_BootHostUsbVar.Cmd.access != BOOT_HOST_USB_CMD_ACCESS_BLOCK)
            {
                g_BootHostUsbVar.State = BOOT_HOST_USB_STATE_DATA;
                fifoUse++;
                g_BootHostUsbVar.WritePt++;
            }
            pos++;
            g_BootHostUsbVar.DataSize--;
            break;
        case BOOT_HOST_USB_STATE_SXS_READ_SIZE:
            indexW      = g_BootHostUsbVar.WritePt%BOOT_HOST_USB_FIFO_READ_SIZE;
            curFifoElem = &g_BootHostUsbVar.Context->Fifo[indexW];

            if((size - pos) < 2)
            {
                g_BootHostUsbVar.Residue = size - pos;
            }
            else
            {
                curFifoElem->Size      =
                    HAL_ENDIAN_BIG_16(*((UINT16 *)&data[pos]));
                if(curFifoElem->Size <= BOOT_HOST_USB_BUFFER_IN_SIZE-6)
                {
                    fifoUse++;
                    g_BootHostUsbVar.WritePt++;
                }
                pos                       += 2;
                g_BootHostUsbVar.DataSize -= 2;
                g_BootHostUsbVar.State = BOOT_HOST_USB_STATE_DATA;
            }
            break;
        }
        if(exitStatus)
        {
            break;
        }
    }

    if(g_BootHostUsbCallbackOs != 0)
    {
        g_BootHostUsbCallbackOs(data, size-g_BootHostUsbVar.Residue);
    }
    if(g_BootHostUsbVar.Residue != 0 && pos != 0)
    {
        memmove((UINT32*)HAL_SYS_GET_UNCACHED_ADDR(g_BootHostUsbVar.Context->BufferOut),
                (UINT32*)HAL_SYS_GET_UNCACHED_ADDR(&g_BootHostUsbVar.Context->BufferOut[pos]),
                g_BootHostUsbVar.Residue);
    }
    BOOT_PROFILE_FUNCTION_EXIT(boot_HostUsbDecode);

    return(exitStatus);
}


PRIVATE HAL_USB_CALLBACK_RETURN_T
boot_HostUsbCallback(HAL_USB_CALLBACK_EP_TYPE_T   type,
                     HAL_USB_SETUP_T*             setup)
{
    switch(type)
    {
    case HAL_USB_CALLBACK_TYPE_CMD:
        if(setup->value == 0)
        {
            boot_HostUsbInit();
        }
        return(HAL_USB_CALLBACK_RETURN_OK);
        break;
    case HAL_USB_CALLBACK_TYPE_DATA_CMD:
        break;
    case HAL_USB_CALLBACK_TYPE_RECEIVE_END:
        break;
    case HAL_USB_CALLBACK_TYPE_TRANSMIT_END:
        break;
    case HAL_USB_CALLBACK_TYPE_ENABLE:
        boot_HostUsbInit();
        break;
    case HAL_USB_CALLBACK_TYPE_DISABLE:
        break;
    }
    return(HAL_USB_CALLBACK_RETURN_KO);
}

// ============================================================================
// boot_HostUsbGenerateReadPacket
// -----------------------------------------------------------------------------
/// This function generate host packet.
/// Fifo read request is fetched and the host packet is generate
// =============================================================================
PRIVATE VOID boot_HostUsbGenerateReadPacket(VOID)
{
    BOOT_HOST_USB_HEADER_T*            header;
    UINT8*                             pos;
    UINT16                             i;
    UINT8                              sum;
    UINT8                              indexR;
    BOOT_HOST_USB_MONITOR_FIFO_ELEM_T* curFifoElem;

    BOOT_PROFILE_FUNCTION_ENTER(boot_HostUsbGenerateReadPacket);

    while(g_BootHostUsbVar.ReadPt != g_BootHostUsbVar.WritePt)
    {
        indexR      = g_BootHostUsbVar.ReadPt%BOOT_HOST_USB_FIFO_READ_SIZE;
        curFifoElem = &g_BootHostUsbVar.Context->Fifo[indexR];

        if(curFifoElem->Size+1+5 >
           BOOT_HOST_USB_BUFFER_IN_SIZE-g_BootHostUsbVar.PositionBufferIn)
        {
            break;
        }
        pos  = &g_BootHostUsbVar.Context
            ->BufferIn[g_BootHostUsbVar.CurrentBufferIn]
            .Buffer[g_BootHostUsbVar.PositionBufferIn];
        *pos            = 0xAD;
        pos++;
        header          = (BOOT_HOST_USB_HEADER_T*) pos;
        header->size    = HAL_ENDIAN_BIG_16(curFifoElem->Size+2);
        header->flowId  = 0xFF;
        pos            += sizeof(BOOT_HOST_USB_HEADER_T);
        *pos            = curFifoElem->Rid;
        memcpy(pos+1,
               (VOID*)HAL_SYS_GET_UNCACHED_ADDR(curFifoElem->Addr),
               curFifoElem->Size);
        // Generate checksum
        for(i = 0, sum = 0;
            i < curFifoElem->Size+1;
            ++i)
        {
            sum ^= *pos;
            pos++;
        }
        *pos                               = ~sum;
        g_BootHostUsbVar.PositionBufferIn += curFifoElem->Size+5+1;
        g_BootHostUsbVar.ReadPt++;
    }

    BOOT_PROFILE_FUNCTION_EXIT(boot_HostUsbGenerateReadPacket);
}

// ============================================================================
// boot_HostUsbSendReadPacket
// -----------------------------------------------------------------------------
/// Send packet generated by boot_HostUsbGenerateReadPacket and switch output
/// swap buffer
// =============================================================================
PRIVATE VOID boot_HostUsbSendReadPacket(VOID)
{
    BOOT_PROFILE_FUNCTION_ENTER(boot_HostUsbSendReadPacket);

    boot_UsbSend(1, g_BootHostUsbVar.Context
                 ->BufferIn[g_BootHostUsbVar.CurrentBufferIn].Buffer,
                 g_BootHostUsbVar.PositionBufferIn, 0);
    g_BootHostUsbVar.PositionBufferIn  = 0;
    g_BootHostUsbVar.CurrentBufferIn++;
    g_BootHostUsbVar.CurrentBufferIn  %= 2;

    BOOT_PROFILE_FUNCTION_EXIT(boot_HostUsbSendReadPacket);
}

PUBLIC VOID boot_HostUsbInit(VOID)
{
    BOOT_PROFILE_FUNCTION_ENTER(boot_HostUsbInit);
    g_BootHostUsbVar.State             = BOOT_HOST_USB_STATE_IDLE;
    g_BootHostUsbVar.ReadPt            = 0;
    g_BootHostUsbVar.WritePt           = 0;
    g_BootHostUsbVar.CurrentBufferIn   = 0;
    g_BootHostUsbVar.PositionBufferIn  = 0;
    g_BootHostUsbVar.Residue           = 0;
    g_BootHostUsbVar.RecvState         = BOOT_HOST_USB_RECV_STATE_RUN;
    boot_UsbEpEnableInterrupt(HAL_USB_EP_DIRECTION_IN (1), FALSE);
    boot_UsbEpEnableInterrupt(HAL_USB_EP_DIRECTION_OUT(1), FALSE);
    boot_UsbRecv(1, g_BootHostUsbVar.Context->BufferOut,
                 BOOT_HOST_USB_BUFFER_OUT_SIZE, 0);

    BOOT_PROFILE_FUNCTION_EXIT(boot_HostUsbInit);
}

PRIVATE CONST HAL_USB_EP_DESCRIPTOR_T boot_HostUsbEp1In = {
    .ep        = HAL_USB_EP_DIRECTION_IN(1),
    .type      = HAL_USB_EP_TYPE_BULK,
    .interval  = 1,
    .callback  = 0
};

PRIVATE  CONST HAL_USB_EP_DESCRIPTOR_T boot_HostUsbEp1Out = {
    .ep        = HAL_USB_EP_DIRECTION_OUT(1),
    .type      = HAL_USB_EP_TYPE_BULK,
    .interval  = 1,
    .callback  = 0
};

// =============================================================================
// HAL_USB_EP_DESCRIPTOR_CONST_PTR_T
// -----------------------------------------------------------------------------
/// constante ep ep list
// =============================================================================
typedef CONST HAL_USB_EP_DESCRIPTOR_T * HAL_USB_EP_DESCRIPTOR_CONST_PTR_T;

PRIVATE CONST HAL_USB_EP_DESCRIPTOR_CONST_PTR_T boot_HostUsbEpList[] = {
    &boot_HostUsbEp1In,
    &boot_HostUsbEp1Out,
    0
};

PUBLIC CONST HAL_USB_INTERFACE_DESCRIPTOR_T boot_HostUsbInterface = {
    .usbClass    = 0xFF,
    .usbSubClass = 0x66,
    .usbProto    = 0x66,
    .epList      = (HAL_USB_EP_DESCRIPTOR_T**) boot_HostUsbEpList,
    .description = "Coolsand Usb",
    .callback    = boot_HostUsbCallback
};

// =============================================================================
// HAL_USB_INTERFACE_DESCRIPTOR_CONST_PTR_T
// -----------------------------------------------------------------------------
/// constante interface config
// =============================================================================
typedef CONST HAL_USB_INTERFACE_DESCRIPTOR_T *
          HAL_USB_INTERFACE_DESCRIPTOR_CONST_PTR_T;

PRIVATE CONST HAL_USB_INTERFACE_DESCRIPTOR_CONST_PTR_T
boot_HostUsbInterfaceList[] = {
    &boot_HostUsbInterface,
    0
};

PRIVATE CONST HAL_USB_CONFIG_DESCRIPTOR_T boot_HostUsbConfig = {
    .attrib        =  0,
    .maxPower      = 50,
    .interfaceList = (HAL_USB_INTERFACE_DESCRIPTOR_T**)
                       boot_HostUsbInterfaceList,
    .description   = "Coolsand default"
};

// =============================================================================
// HAL_USB_CONFIG_DESCRIPTOR_CONST_PTR_T
// -----------------------------------------------------------------------------
/// Constant usb config
// =============================================================================
typedef CONST HAL_USB_CONFIG_DESCRIPTOR_T *
          HAL_USB_CONFIG_DESCRIPTOR_CONST_PTR_T;


PRIVATE CONST HAL_USB_CONFIG_DESCRIPTOR_CONST_PTR_T boot_HostUsbConfigList[] = {
    &boot_HostUsbConfig,
    0
};

PRIVATE CONST HAL_USB_DEVICE_DESCRIPTOR_T boot_HostUsbDesc = {
    .usbClass     = 0,
    .usbSubClass  = 0,
    .usbProto     = 0,
    .vendor       = 0xC001,
    .product      = 0x0002,
    .version      = 0x0001,
    .configList   = (HAL_USB_CONFIG_DESCRIPTOR_T**) boot_HostUsbConfigList,
    .description  = "Coolsand",
    .serialNumber = "Host USB"
};

// =============================================================================
// boot_HostUsbDeviceCallback
// -----------------------------------------------------------------------------
/// Callback use to define the host usb descriptor. It's call by USB driver
// =============================================================================
PUBLIC HAL_USB_DEVICE_DESCRIPTOR_T* boot_HostUsbDeviceCallback(VOID)
{
    return (HAL_USB_DEVICE_DESCRIPTOR_T*) &boot_HostUsbDesc;
}

// =============================================================================
// boot_HostUsbEvent
// -----------------------------------------------------------------------------
/// Send USB host event with a timeout
// =============================================================================
PUBLIC VOID boot_HostUsbEvent(UINT32 event)
{
    BOOT_HOST_USB_HEADER_T*            header;
    UINT8                              trame[10];
    UINT32                             startTime = hwp_timer->HWTimer_CurVal;

    header = (BOOT_HOST_USB_HEADER_T*) &trame[1];

    // Generate Event packet
    trame[0]               = 0xAD;
    header->size           = HAL_ENDIAN_BIG_16(0x06);
    header->flowId         = 0xFF;
    trame[4]               = 0;
    memcpy(&trame[5], &event, sizeof(UINT32));
    trame[9]               = ~(trame[4]^trame[5]^trame[6]^trame[7]^trame[8]);

    // Wait the last packet
    while(boot_UsbEpTransfertDone(HAL_USB_EP_DIRECTION_IN(1)) == FALSE)
    {
        if(hwp_timer->HWTimer_CurVal >= startTime + BOOT_USB_SEND_EVENT_DELAY)
        {
            BOOT_PROFILE_PULSE(BOOT_USB_MONITOR_EVENT_TIMEOUT);
            return;
        }
    }
    boot_UsbSend(1, trame, sizeof(trame), 0);
    while(boot_UsbEpTransfertDone(HAL_USB_EP_DIRECTION_IN(1)) == FALSE)
    {
        if(hwp_timer->HWTimer_CurVal >= startTime + BOOT_USB_SEND_EVENT_DELAY)
        {
            BOOT_PROFILE_PULSE(BOOT_USB_MONITOR_EVENT_TIMEOUT);
            break;
        }
    }
}

// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================

// ============================================================================
// boot_HostUsbClockEnable
// -----------------------------------------------------------------------------
/// Enable usb clock
// =============================================================================
PUBLIC VOID boot_HostUsbClockEnable(VOID)
{
    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_UNLOCK;
    hwp_extApb->ExtPll_Config = REGS_EXTPLL_INPUT_DIVIDER(0)
        | REGS_EXTPLL_FEEDBACK_DIVIDER(22)
        | REGS_EXTPLL_OUTPUT_DIVIDER_DIV_BY_1;
    hwp_anaAccoFe->pll_ctrl = ANA_ACCO_PLL_PD_N |
        ANA_ACCO_PLL_IN_SOURCE_EXT |
        ANA_ACCO_PLL_CP_TUNE_100_PERCENT |
        ANA_ACCO_PLL_SYS_EON_ENABLE |
        ANA_ACCO_PLL_EXT_EON_ENABLE;
    hwp_extApb->ExtPll_Ctrl = REGS_EXTPLL_ENABLE_ENABLE |
        REGS_EXTPLL_CLK_FAST_ENABLE_ENABLE |
        REGS_EXTPLL_OUTPUT_ENABLE_ENABLE;
    while(!(hwp_extApb->ExtPll_Ctrl & REGS_EXTPLL_LOCKED_LOCKED));
    while(!(hwp_anaAccoFe->pll_ctrl & ANA_ACCO_PLL_LOCKED));
    hwp_sysCtrl->REG_DBG    = SYS_CTRL_PROTECT_LOCK;
}

// =============================================================================
// boot_HostUsbOpen
// -----------------------------------------------------------------------------
/// Configure USB host
/// @param context Define the context use. The context containt read request
/// fifo and buffer for read and write on USB
/// @param callback This callback is use to change the parsing process of host
/// protocol
// =============================================================================
PUBLIC VOID boot_HostUsbOpen(BOOT_HOST_USB_MONITOR_CTX_T* context,
                             BOOT_HOST_USB_CALLBACK_T     callback)
{
    g_BootHostUsbVar.Context = context;
    g_BootHostUsbCallbackOs  = callback;
    // Enable Usb Interrupt
    hwp_sysIrq->Mask_Set     = SYS_IRQ_SYS_IRQ_USBC;
    boot_HostUsbInit();
}

// =============================================================================
// boot_HostUsbClose
// -----------------------------------------------------------------------------
/// Close host usb (disable USB interrupt)
// =============================================================================
PUBLIC VOID boot_HostUsbClose(VOID)
{
    // Disable Usb Interrupt
    hwp_sysIrq->Mask_Clear = SYS_IRQ_SYS_IRQ_USBC;
}

// =============================================================================
// boot_HostUsbSend
// -----------------------------------------------------------------------------
/// Generate packet must be send and send it
// =============================================================================
PUBLIC BOOL boot_HostUsbSend(VOID)
{
    if(g_BootHostUsbVar.ReadPt != g_BootHostUsbVar.WritePt) // FIFO not empty
    {
        boot_HostUsbGenerateReadPacket();
    }

    // Previous packet send & new packet ready to be send ?
    if(boot_UsbEpTransfertDone(HAL_USB_EP_DIRECTION_IN(1)) == TRUE &&
       g_BootHostUsbVar.PositionBufferIn != 0)
    {
        boot_HostUsbSendReadPacket();
    }

    if(boot_UsbEpTransfertDone(HAL_USB_EP_DIRECTION_IN(1) == FALSE))
    {
        return(TRUE);
    }
    return(FALSE);
}

// =============================================================================
// boot_HostUsbRecv
// -----------------------------------------------------------------------------
/// Receive USB packet and parse it
// =============================================================================
PUBLIC UINT8 boot_HostUsbRecv(VOID)
{
    UINT8  *data;
    UINT32  size;
    UINT8   fifoUse;
    UINT8   flag       = 0;
    UINT8   exitStatus = 0;

    fifoUse = g_BootHostUsbVar.WritePt-g_BootHostUsbVar.ReadPt;

    data = (UINT8*) HAL_SYS_GET_UNCACHED_ADDR(g_BootHostUsbVar.Context->BufferOut);

    switch(g_BootHostUsbVar.RecvState)
    {
    case BOOT_HOST_USB_RECV_STATE_RUN:
        if(boot_UsbEpTransfertDone(HAL_USB_EP_DIRECTION_OUT(1)))
        {
            size = boot_UsbEpRxTransferedSize(1)+g_BootHostUsbVar.Residue;
            exitStatus = boot_HostUsbDecode(data, size);
            flag       = 1;
        }
        break;

    case BOOT_HOST_USB_RECV_STATE_WAIT: // Fifo Full
        if(fifoUse != BOOT_HOST_USB_FIFO_READ_SIZE)
        {
            exitStatus = boot_HostUsbDecode(data, g_BootHostUsbVar.Residue);
            flag       = 1;
        }
        break;
    }

    if(g_BootHostUsbVar.RecvState == BOOT_HOST_USB_RECV_STATE_RUN &&
       flag == 1)
    {
        // Fetch new usb packet
        boot_UsbRecv(1, &g_BootHostUsbVar.Context->BufferOut
                     [g_BootHostUsbVar.Residue],
                     BOOT_HOST_USB_BUFFER_OUT_SIZE-g_BootHostUsbVar.Residue, 0);
    }
    return(exitStatus);
}
