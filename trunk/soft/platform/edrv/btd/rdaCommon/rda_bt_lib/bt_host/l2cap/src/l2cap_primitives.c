/**************************************************************************
 * MODULE NAME:    l2cap_primitives.c
 * PROJECT CODE:
 * DESCRIPTION:    l2cap primitives
 * MAINTAINER:     Martin Mellody
 * DATE:           02-12-1999x
 *
 * SOURCE CONTROL: $Id: l2cap_primitives.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 Parthus Technologies.
 *     All rights reserved.
 *
 *
 *************************************************************************/


#include "host_config.h"
#include "host_types.h"
#include "l2cap_lower_layer.h"
#include "l2_types.h"
#include "l2_channel.h"


#include "l2_connect_sm.h"
#include "l2_sar.h"
#include "l2cap_primitives.h"
#include "papi.h"
#include "host_types.h"
#include "host_error.h"

APIDECL1 t_api APIDECL2 L2_GetWriteBuffer(u_int16 cid, u_int16 len, t_DataBuf **writeBuffer) {
    return prh_l2_GetWriteBuffer(cid, len, 0, (struct host_buf **)writeBuffer);

}

APIDECL1 t_api APIDECL2 L2_Write(u_int16 cid, t_DataBuf *writeBuffer) {
    return prh_l2_sar_data_req(0,cid,(struct host_buf *)writeBuffer);  
}

APIDECL1 t_api APIDECL2 L2_FreeWriteBuffer(t_DataBuf *writeBuffer) {
    prh_l2_FreeWriteBuffer((struct host_buf *)writeBuffer);
    return BT_NOERROR;
}


APIDECL1 t_api APIDECL2 L2_RetrieveHandle(u_int16 cid, u_int16* p_handle) {
    prh_t_l2_channel* p_channel;
#if pDEBUG
    pDebugPrintfEX((pLOGNOTICE,pLOGL2CAP,"L2CAP: L2_RetrieveHandle called, cid=0x%02X\n",cid));
#elif (PRH_BS_CFG_SYS_TRACE_DEBUG_VIA_UART_SUPPORTED)
    u_int8 temp[2];
    BT_WRITE_LE_UINT16(temp,cid);
    SYSDebug_PrintEx(pLOGDEBUG,pLOGL2CAP, LOG_L2CAP_L2CAP_PRIMITIVES_INDEX, temp, 2,BT_CODE_WORD);
#endif
    p_channel = prh_l2_chn_get_p_channel(cid);
    if (p_channel) {
        *p_handle = p_channel->handle;
        return BT_NOERROR;
    } else {
        return BT_INVALIDPARAM;
    }
}
