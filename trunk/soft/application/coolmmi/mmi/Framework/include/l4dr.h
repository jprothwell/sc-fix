/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *	KeyMap.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifdef MMI_ON_HARDWARE_P

#ifndef COOLSAND_L4_DR_H
#define COOLSAND_L4_DR_H

/***************************************************************************** 
* Include
*****************************************************************************/
/* added by JL */
#include <stdarg.h>
#include "kal_release.h"      	/* basic data type */
#include "stack_common.h"
#include "stack_msgs.h"
#include "app_ltlcom.h"       	/* task message communiction */
#include "stacklib.h"        	/* basic type for dll, evshed, stacktimer */
#include "stack_timer.h"  
#include "event_shed.h"  
#include "l4a.h"
#include "fat_fs.h"
#include "stack_config.h"
 

#include "l3_inc_enums.h"
#include "phb_defs.h"
#include "mmi_msg_struct.h"
/* end added by JL */
#include "mmi_data_types.h"

/***************************************************************************** 
* Define
*****************************************************************************/

/***************************************************************************** 
* Typedef 
*****************************************************************************/

/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/

extern void L4KeyHandle(void *paraBuff);
extern void L4SetClock(rtc_format_struct datetime, oslTaskFuncPtr FuncRspPtr);
extern void L4SetClockRSP(void *buf);
extern void L4GetClockTime(oslTaskFuncPtr FuncRspPtr);
extern void L4GetClockTimeRSP(void *buf);
extern void L4StartTimer(unsigned short nTimerId, oslTimerFuncPtr TimerExpiry, void * funcArg, unsigned long nTimeDurationInTicks, unsigned char alignment);
extern void L4StopTimer(unsigned short  nTimerId);
extern void L4StopTimerRSP(void *paraBuff);
extern void L4StartTimerRSP(void *paraBuff);
extern void L4InitSerialPort(void);
extern long L4UARTWrite(kal_uint8 *arrayRx, kal_uint16 size);
extern long L4UARTRead(kal_uint8 *arrayRx, kal_uint16 size);
// extern void SetL4EventHandler(L4FuncPtr funcPtr, U16 eventID);

extern void L4NvramWriteRsp(void *buf);
extern void L4ReadNvram(kal_uint8 gstructtype, kal_uint8 index);
extern void L4WriteNvram(kal_uint8 gstructtype, void * buf, kal_uint8 index);
extern void L4NvramReadRsp(void *buf);

extern void L4InitTimer(void);
extern void L4StopBaseTimer(void *base_timer_ptr);
extern void L4StartBaseTimer(void *base_timer_ptr, unsigned int time_out);

#endif /* COOLSAND_L4_DR_H */
#endif /* MMI_ON_HARDWARE_P */


