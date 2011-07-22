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
//  $HeadURL: http://10.10.100.14/svn/developing/Sources/svc/branches/Total_BJ/uctls/src/uctls_fnc.c $
//  $Author: mathieu $
//  $Date$
//  $Revision: 31044 $
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file uctls_fnc.c
///
/// USB Controler service
//
////////////////////////////////////////////////////////////////////////////////

#include "uctls_m.h"

#include "sxr_ops.h"
#include "sxs_io.h"
#include "sxr_tim.h"

#include "tgt_uctls_cfg.h"

typedef UCTLS_BODY_MSG_T MsgBody_t;
#include "itf_msg.h"

#include "string.h"
#include "uctlsp_debug.h"

// FIXME TODO Add comments.

// =============================================================================
// UCTLS_INTERNAL_STATUS_T
// -----------------------------------------------------------------------------
/// The status of uctls service
// =============================================================================
typedef enum
{
    UCTLS_STATUS_CLOSE = 0,
    UCTLS_STATUS_OPEN,
    UCTLS_STATUS_CHARGER,
    UCTLS_STATUS_SPEED_ENUM,
    UCTLS_STATUS_CONNECTED
} UCTLS_STATUS_T ;


PRIVATE UINT8                        g_UctlsMbx;

PRIVATE UCTLS_ID_T                   g_UctlsId              = -1;
PRIVATE HAL_USB_DEVICE_DESCRIPTOR_T* g_UctlsDeviceDesc      =  0;
PRIVATE UCTLS_CHARGER_STATUS_T       g_UctlsChargerStatus;

UCTLS_STATUS_T                       g_UctlsStatus          =
    UCTLS_STATUS_CLOSE;

PRIVATE VOID uctls_UsbDeviceNotDetected(VOID*);

PRIVATE HAL_USB_DEVICE_DESCRIPTOR_T* uctls_GetDescriptor(BOOL SPEED_ENUM)
{
    Msg_t*                           msg;

    UCTLS_TRACE(UCTLS_INFO_TRC, 0, "uctls_GetDescriptor");

    switch(g_UctlsStatus)
    {
        case UCTLS_STATUS_CHARGER:
            if(SPEED_ENUM)
                g_UctlsStatus = UCTLS_STATUS_SPEED_ENUM;
            break;
        case UCTLS_STATUS_SPEED_ENUM:
            if(g_UctlsMbx != 0xFF && g_UctlsStatus != UCTLS_STATUS_CONNECTED)
            {
                msg              = (Msg_t*) sxr_Malloc(sizeof(Msg_t));
                msg->H.Id        = UCTLS_HEADER_ID;
                msg->B           = UCTLS_MSG_USB_DEVICE_CONNECTED;
                sxr_Send(msg, g_UctlsMbx, SXR_SEND_MSG);
            }

            g_UctlsStatus = UCTLS_STATUS_CONNECTED;

            sxr_StopFunctionTimer(uctls_UsbDeviceNotDetected);
            break;
        default :
            break;
    }

    return(g_UctlsDeviceDesc);
}

PUBLIC VOID uctls_Open(UINT8                    mbx,
                       UINT16                   usbVendor,
                       UINT16                   usbProduct,
                       UINT16                   usbVersion,
                       UINT8*                   usbSerialNumber)
{
    HAL_USB_CONFIG_DESCRIPTOR_T** usbConfigs;

    UCTLS_TRACE(UCTLS_INFO_TRC, 0, "uctls_Open");

    g_UctlsMbx        = mbx;

    if(g_UctlsStatus == UCTLS_STATUS_CLOSE)
    {
        usbConfigs        = (HAL_USB_CONFIG_DESCRIPTOR_T**)
            sxr_Malloc(sizeof(HAL_USB_CONFIG_DESCRIPTOR_T*)*2);

        // Set config MaxPower 100mA
        usbConfigs[0]     = uctls_NewConfigDescriptor(50, 0, 0, 0);
        usbConfigs[1]     = 0;
        // Set device Class = Not defined
        g_UctlsDeviceDesc =  uctls_NewDeviceDescriptor(0, 0, 0, usbVendor,
                                                       usbProduct, usbVersion,
                                                       usbConfigs,
                                                       usbSerialNumber,
                                                       "Coolsand");

        uctls_SetMode(UCTLS_ID_1);
    }

    g_UctlsStatus        = UCTLS_STATUS_OPEN;
    g_UctlsChargerStatus = UCTLS_CHARGER_STATUS_DISCONNECTED;
}

PRIVATE VOID uctls_UsbDeviceNotDetected(VOID* data)
{
    UCTLS_TRACE(UCTLS_INFO_TRC, 0, "uctls_UsbDeviceNotDetected: No device detected");

    data = data;

    hal_UsbClose();

    /// @todo add link with pmd/pms
}

PUBLIC VOID uctls_ChargerStatus(UCTLS_CHARGER_STATUS_T status)
{
    Msg_t*                           msg;

    UCTLS_TRACE(UCTLS_INFO_TRC, 0, "uctls_ChargerStatus %i", status);

    if(status != g_UctlsChargerStatus)
    {
        UCTLS_TRACE(UCTLS_INFO_TRC, 0, "uctls_ChargerStatus: Change %i", status);
        switch(status)
        {
        case UCTLS_CHARGER_STATUS_CONNECTED:
            g_UctlsStatus = UCTLS_STATUS_CHARGER;
            sxr_StartFunctionTimer(10*(1<<14) , uctls_UsbDeviceNotDetected,
                                   (VOID*)NULL, 0x02);
            hal_UsbOpen(uctls_GetDescriptor);
            break;
        case UCTLS_CHARGER_STATUS_DISCONNECTED:
            hal_UsbClose();
            if(g_UctlsMbx != 0xFF)
            {
                msg              = (Msg_t*) sxr_Malloc(sizeof(Msg_t));
                msg->H.Id        = UCTLS_HEADER_ID;
                msg->B           = UCTLS_MSG_USB_DEVICE_DISCONNECTED;
                sxr_Send(msg, g_UctlsMbx, SXR_SEND_MSG);
            }
            g_UctlsStatus = UCTLS_STATUS_OPEN;
            sxr_StopFunctionTimer(uctls_UsbDeviceNotDetected);
            break;
        }

        g_UctlsChargerStatus = status;
    }
}

PUBLIC VOID uctls_Close(VOID)
{
    Msg_t*                           msg;

    UCTLS_TRACE(UCTLS_INFO_TRC, 0, "uctls_Close");

    if(g_UctlsStatus == UCTLS_STATUS_CONNECTED ||
       g_UctlsStatus == UCTLS_STATUS_CHARGER ||
       g_UctlsStatus == UCTLS_STATUS_SPEED_ENUM)
    {
        hal_UsbClose();
        if(g_UctlsDeviceDesc)
        {
            uctls_CleanDeviceDescriptor(g_UctlsDeviceDesc);
            g_UctlsDeviceDesc = 0;
        }
    }
    if(g_UctlsStatus == UCTLS_STATUS_CONNECTED)
    {
        if(g_UctlsMbx != 0xFF)
        {
            msg              = (Msg_t*) sxr_Malloc(sizeof(Msg_t));
            msg->H.Id        = UCTLS_HEADER_ID;
            msg->B           = UCTLS_MSG_USB_DEVICE_DISCONNECTED;
            sxr_Send(msg, g_UctlsMbx, SXR_SEND_MSG);
        }
    }

    sxr_StopFunctionTimer(uctls_UsbDeviceNotDetected);

    g_UctlsMbx    = 0xFF;
    g_UctlsStatus = UCTLS_STATUS_CLOSE;
}

PUBLIC VOID uctls_SetMode(UCTLS_ID_T mode)
{
    CONST UCTLS_SERVICE_LIST_T*      modes;
    UINT8                            nbInterface;
    UINT8                            i;
    UINT8                            j;
	UINT8                            indexInterfaceList;
    HAL_USB_INTERFACE_DESCRIPTOR_T** interfaceList;
    HAL_USB_INTERFACE_DESCRIPTOR_T** interfaceTmpList;
	
    if(g_UctlsId == mode)
    {
        return;
    }
	
    modes = tgt_GetUctlsConfig();
	//close previous service
	if(g_UctlsId != -1)
	{
	    for(i = 0; i < UCTLS_SERVICE_LIST_SIZE; ++i) {
	        if(modes[g_UctlsId].servicesList[i].serviceCallback == NULL)
	        {
	            continue;
	        }
	        modes[g_UctlsId].servicesList[i].serviceCallback->close();
	    }
	}

    UCTLS_TRACE(UCTLS_INFO_TRC, 0, "uctls_SetMode: Change %i", mode);

    g_UctlsDeviceDesc->usbMode = mode;

    if(g_UctlsDeviceDesc->configList[0]->interfaceList)
	{         
		uctls_CleanInterfaceListDescriptor(             
			g_UctlsDeviceDesc->configList[0]->interfaceList);        
		g_UctlsDeviceDesc->configList[0]->interfaceList = 0;    
	}

    g_UctlsId                                   = mode;
    g_UctlsDeviceDesc->configList[0]->configIdx = mode + 1;

    for(i = 0, nbInterface = 0; i < UCTLS_SERVICE_LIST_SIZE; ++i)
    {
        if(modes[g_UctlsId].servicesList[i].serviceCallback == NULL)
        {
            continue;
        }
        nbInterface++;
    }

    if(nbInterface == 0)
    {
        if(g_UctlsStatus == UCTLS_STATUS_CONNECTED ||
           g_UctlsStatus == UCTLS_STATUS_CHARGER ||
           g_UctlsStatus == UCTLS_STATUS_SPEED_ENUM)
        {
            hal_UsbReset(TRUE);
            sxr_Sleep(8000); // Wait 0.5s
            hal_UsbReset(FALSE);
        }
        return;
    }

    if(nbInterface != 0)
    {
        interfaceList = (HAL_USB_INTERFACE_DESCRIPTOR_T**)
            sxr_Malloc(sizeof(HAL_USB_INTERFACE_DESCRIPTOR_T*) * 16);

        UCTLS_TRACE(UCTLS_INFO_TRC, 0, "uctls: interface list %#x\n", interfaceList);

        memset(interfaceList, 0,
               sizeof(HAL_USB_INTERFACE_DESCRIPTOR_T*) * 16);

        for(i = 0, indexInterfaceList = 0; i < UCTLS_SERVICE_LIST_SIZE; ++i)
        {
            if(modes[g_UctlsId].servicesList[i].serviceCallback == NULL)
            {
                continue;
            }
            interfaceTmpList =
                modes[g_UctlsId].servicesList[i].serviceCallback->open(
                    &modes[g_UctlsId].servicesList[i].serviceCfg);
            if(interfaceTmpList)
            {
                for(j = 0; interfaceTmpList[j]; ++j)
                {
                    interfaceList[indexInterfaceList] = interfaceTmpList[j];
                    UCTLS_TRACE(UCTLS_INFO_TRC, 0, "uctls: interface %i %#x\n",
                                indexInterfaceList, interfaceTmpList[j]);
                    ++indexInterfaceList;
                }
                sxr_Free(interfaceTmpList);
            }
        }


        UCTLS_TRACE(UCTLS_INFO_TRC, 0, "uctls: end of descriptor generation"
                    " nb interfaces %i\n", indexInterfaceList);

        // Set interfaces list
        g_UctlsDeviceDesc->configList[0]->interfaceList = interfaceList;
		
		//video iad
		if(g_UctlsId == 2)
			g_UctlsDeviceDesc->configList[0]->iad = modes[g_UctlsId].servicesList[0].serviceCfg.videos.iad;
		else
			g_UctlsDeviceDesc->configList[0]->iad = NULL;
    }


    if(g_UctlsStatus == UCTLS_STATUS_CONNECTED ||
       g_UctlsStatus == UCTLS_STATUS_CHARGER ||
       g_UctlsStatus == UCTLS_STATUS_SPEED_ENUM)
    {
        hal_UsbReset(TRUE);
        sxr_Sleep(8000); // Wait 0.5s
        hal_UsbReset(FALSE);
    }
}

PUBLIC UCTLS_ID_T uctls_GetMode(VOID)
{
    return(g_UctlsId);
}
