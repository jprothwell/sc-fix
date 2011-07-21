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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *    uart_def.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This Module defines the size of UART ring buffer
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#include "drv_comm.h"
#include "uart_sw.h"

/*
1. remove get_mem by using allocate static array
2. use different buffer size for __PRODUCTION_RELEASE__
3. the buffer size is depending on the owner instead uart port
*/

UART_flowCtrlMode  *UART_Get_FlowCtrl_Mode(void);
const UART_rings_struct *UART_Get_Data(void);


#ifndef __PRODUCTION_RELEASE__
// define the uart ring buffer size
#define L4_RX_LENGTH				2048
#define L4_TX_LENGTH				2048
#define L4_TXISR_LENGTH			3072
#define TST_RX_LENGTH			2048
#define TST_TX_LENGTH			2048
#define TST_TXISR_LENGTH		3072
#define CUSTOM_RX_LENGTH		2048
#define CUSTOM_TX_LENGTH		2048
#define CUSTOM_TXISR_LENGTH	3072
#else	
// define the uart ring buffer size
#define L4_RX_LENGTH				2048
#define L4_TX_LENGTH				2048
#define L4_TXISR_LENGTH			3072
#define TST_RX_LENGTH			2048
#define TST_TX_LENGTH			2048
#define TST_TXISR_LENGTH		3072
#define CUSTOM_RX_LENGTH		2048
#define CUSTOM_TX_LENGTH		2048
#define CUSTOM_TXISR_LENGTH	3072
#endif	// __PRODUCTION_RELEASE__

/*added by elisa for declaration */
#include "ts.h"            
//ended of elisa         2008_9_27


 
kal_uint8 L4_RxRingBuffer[L4_RX_LENGTH];

#ifndef __DMA_UART_VIRTUAL_FIFO__
kal_uint8 L4_TxRingBuffer[L4_TX_LENGTH];
kal_uint8 L4_TxISRRingBuffer[L4_TXISR_LENGTH];
#else
kal_uint8 L4_TxRingBuffer[L4_TX_LENGTH+L4_TXISR_LENGTH];	
#endif

// tst ring buffer
kal_uint8 TST_RxRingBuffer[TST_RX_LENGTH];
#ifndef __DMA_UART_VIRTUAL_FIFO__
kal_uint8 TST_TxRingBuffer[TST_TX_LENGTH];
kal_uint8 TST_TxISRRingBuffer[TST_TXISR_LENGTH];
#else
kal_uint8 TST_TxRingBuffer[L4_TX_LENGTH+L4_TXISR_LENGTH];	
#endif


#ifdef __UART3_SUPPORT__
// uart3 ring buffer
kal_uint8 CUSTOM_RxRingBuffer[CUSTOM_RX_LENGTH];
#ifndef __DMA_UART_VIRTUAL_FIFO__
kal_uint8 CUSTOM_TxRingBuffer[CUSTOM_TX_LENGTH];
kal_uint8 CUSTOM_TxISRRingBuffer[CUSTOM_TXISR_LENGTH];
#else
kal_uint8 CUSTOM_TxRingBuffer[L4_TX_LENGTH+L4_TXISR_LENGTH];	
#endif
#endif //__UART3_SUPPORT__

const UART_rings_struct	UART_custom_rings = 
{
	{
		{
			L4_RxRingBuffer,
			L4_TxRingBuffer,
			#ifndef __DMA_UART_VIRTUAL_FIFO__
			L4_TxISRRingBuffer,
			#endif
			sizeof L4_RxRingBuffer,
			sizeof L4_TxRingBuffer,
			#ifndef __DMA_UART_VIRTUAL_FIFO__
			sizeof L4_TxISRRingBuffer,
			#endif
			MOD_L4C,
		},		
		{
			TST_RxRingBuffer,
			TST_TxRingBuffer,
			#ifndef __DMA_UART_VIRTUAL_FIFO__
			TST_TxISRRingBuffer,
			#endif
			sizeof TST_RxRingBuffer,
			sizeof TST_TxRingBuffer,
			#ifndef __DMA_UART_VIRTUAL_FIFO__
			sizeof TST_TxISRRingBuffer,
			#endif
			MOD_TST_READER,
		}
		#ifdef __UART3_SUPPORT__
		,
		{
			CUSTOM_RxRingBuffer,
			CUSTOM_TxRingBuffer,
			CUSTOM_TxISRRingBuffer,
			sizeof CUSTOM_RxRingBuffer,
			sizeof CUSTOM_TxRingBuffer,
			sizeof CUSTOM_TxISRRingBuffer,
			MOD_UART3_HISR
		}		
		#endif	// __UART3_SUPPORT__
	}
};
const UART_rings_struct *UART_Get_Data(void) 
{
   return (&UART_custom_rings);
}

const UART_customize_function_struct uart_custom_func =
{
     UART_Get_Data
};            

const UART_customize_function_struct *UART_GetFunc(void)
{
   return (&uart_custom_func);  
}   
#if 0    //added by elisa for not calling this function       
UART_flowCtrlMode  UART_GetFlowCtrl(UART_PORT uart_port)
{
 UART_flowCtrlMode flow_ctrl = fc_none;
 
   switch(uart_port)
   {
      case uart_port1:
         flow_ctrl=fc_none;
         break;
      case uart_port2:
         flow_ctrl=fc_none;
         break;
#ifdef __UART3_SUPPORT__
      case uart_port3:
         flow_ctrl=fc_none;
         break;
#endif         
#ifdef __IRDA_SUPPORT__
      case uart_port_irda:
         flow_ctrl=fc_none;
         break;
#endif
#ifdef __USB_ENABLE__
      case uart_port_usb:
         flow_ctrl=fc_none;
         break;    
#endif      
#ifdef __BLUETOOTH_SUPPORT__
      case uart_port_bluetooth:
         flow_ctrl=fc_none;
         break;
#endif         
      default:
         break;
   } 
   return(flow_ctrl);      
}
#endif
