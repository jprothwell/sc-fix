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
 *    uart_sw.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for UART driver.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef UART_SW_H
#define UART_SW_H

#include "stack_config.h"
#include "bmd.h"
#include "drv_comm.h"

#ifdef __UART3_SUPPORT__
   #define  MAX_UART_PORT_NUM       3
#else /*__UART3_SUPPORT__*/
   #define  MAX_UART_PORT_NUM       2
#endif   /*__UART3_SUPPORT__*/

#define  UART_STAT_EscDet        0x01
#define  UART_STAT_Break         0x02

#define  UART_RecNormal          0
#define  UART_Get3EscChar        1
#define  UART_StartCheckESC      2

#if ( (defined(MT6205)) || (defined(MT6205B)) || (defined(MT6218)) || (defined(MT6218B))|| (defined(MT6219)) || (defined(MT6217))|| (defined(MT6228))|| (defined(MT6229))|| (defined(MT6226))|| (defined(MT6227)) )
#define  UART_DMA_SIZE           128
#endif    

//Size = 8bit, sinc en, dinc disable, hw management, 
//1 trans/dma cycle, UART1 master,Interrupt enable
#define DMA_CON_UART1TxNormal 0x8034
//Size = 8bit, sinc disable, dinc enable, hw management, 
//1 trans/dma cycle, UART1 master,Interrupt enable
#define DMA_CON_UART1RxNormal 0x8038
//Size = 8bit, sinc en, dinc disable, hw management, 
//1 trans/dma cycle, UART2 master,Interrupt enable
#define DMA_CON_UART2TxNormal 0x8054
//Size = 8bit, sinc disable, dinc enable, hw management, 
//1 trans/dma cycle, UART2 master,Interrupt enable
#define DMA_CON_UART2RxNormal 0x8058

typedef enum {
      uart_port1=0,
      uart_port2,
      uart_port3,
		uart_port_irda,
		uart_port_usb,
		uart_port_bluetooth,
      uart_port_swdbg,
		start_of_virtual_port=10,
		end_of_virtual_port=14,
      uart_max_port,      
      uart_port_null = 99	/* a dummy port for those who doesn't use physical port */
} UART_PORT;

typedef enum {
      RX_BUF,
      TX_BUF
} UART_buffer;

typedef enum {
      uart_sleep_on_tx_allow = 0,
      uart_sleep_on_tx_forbid
} UART_SLEEP_ON_TX;

typedef kal_uint32 UART_baudrate;

#define     UART_BAUD_AUTO          0
#define     UART_BAUD_75            75
#define     UART_BAUD_150           150
#define     UART_BAUD_300           300
#define     UART_BAUD_600           600
#define     UART_BAUD_1200          1200
#define     UART_BAUD_2400          2400    
#define     UART_BAUD_4800          4800
#define     UART_BAUD_7200          7200
#define     UART_BAUD_9600          9600
#define     UART_BAUD_14400         14400
#define     UART_BAUD_19200         19200
#define     UART_BAUD_28800         28800
#define     UART_BAUD_33900         33900
#define     UART_BAUD_38400         38400
#define     UART_BAUD_57600         57600
#define     UART_BAUD_115200        115200
#define     UART_BAUD_230400        230400
#define     UART_BAUD_460800        460800
#define     UART_BAUD_921600        921600

typedef enum {
      len_5=5,
      len_6,
      len_7,
      len_8
} UART_bitsPerCharacter;

typedef enum {
      sb_1=1,
      sb_2,
      sb_1_5
} UART_stopBits;

typedef enum {
      pa_none=0,
      pa_odd,
      pa_even,
      pa_space
} UART_parity;

typedef enum {
      fc_none=1,
      fc_hw,
      fc_sw
} UART_flowCtrlMode;

typedef struct
{
      UART_baudrate                 baud; 
      UART_bitsPerCharacter         dataBits;
      UART_stopBits                 stopBits;
      UART_parity                   parity;
      UART_flowCtrlMode             flowControl;
      kal_uint8                     xonChar;
      kal_uint8                     xoffChar;
      kal_bool                      DSRCheck;
} UARTDCBStruct;

typedef struct
{
      kal_uint8                      EscChar;
      kal_uint16                     GuardTime;
} UART_ESCDetectStruct;

typedef struct
{
      kal_uint8                      *rx_buffer;   /* data buffer */
      kal_uint8                      *tx_buffer;   /* data buffer */
      kal_uint8                      *txISR_buffer;  /* data buffer */
} UART_RingBufferStruct;

#if ( (defined(MT6218)) || (defined(MT6218B)) || (defined(MT6219)) || (defined(MT6217))|| (defined(MT6228))|| (defined(MT6229))|| (defined(MT6226))|| (defined(MT6227)) )
   typedef enum {
      uart_null_buffer=0,
      uart_isr_buffer,
      uart_normal_buffer
   } UART_RingBuffer_Type;

   typedef struct
   {
         UART_RingBuffer_Type            type;
         kal_uint16                      index;
   } UART_RingBuffer_SaveIndex;
#endif    

typedef struct
{
	kal_uint16	       tst_port;
	kal_uint16         ps_port;	
	UART_baudrate      tst_baudrate;
	UART_baudrate      ps_baudrate;
	// Lisen 0813
	kal_bool          High_Speed_SIM_Enabled;
	kal_uint8         swdbg;
} port_setting_struct;

/*TY adds these to expand flexibility 2004/10/15*/
typedef void (*UART_TX_FUNC)(UART_PORT port); 
typedef void (*UART_RX_FUNC)(UART_PORT port) ; 

typedef struct
{
      UART_PORT                      port_no;
      kal_bool                       initialized;
      module_type                    ownerid;
      module_type                    UART_id;
      kal_bool                       breakDet;
      kal_bool                       EscFound;
      UARTDCBStruct                  DCB;
      UART_RingBufferStruct          RingBuffers;
      UART_ESCDetectStruct           ESCDet;
      BUFFER_INFO                    Tx_Buffer_ISR; /* receive buffer */
      BUFFER_INFO                    Rx_Buffer;  /* receive buffer */
      BUFFER_INFO                    Tx_Buffer;  /* transmit buffer */
//      kal_hisrid                     hisr;   //dyj del 20060519
      IO_level                       DSR;
      /*detect Escape*/
      kal_uint8                      handle;    /*GPT handle*/
      kal_uint8                      EscCount;
      kal_uint8                      Rec_state; /**/
      UART_SLEEP_ON_TX               sleep_on_tx;
      kal_bool               		EnableTX;
      /*TY adds these to expand flexibility 2004/10/15*/
      UART_TX_FUNC                  tx_cb;
      UART_RX_FUNC                  rx_cb;
      #ifdef __DMA_UART_VIRTUAL_FIFO__
		kal_uint8 							Rx_DMA_Ch;
		kal_uint8 							Tx_DMA_Ch;
      #endif
} UARTStruct;

// for uart dispatch table 
typedef enum
{
	UART_TYPE = 0,
	IRDA_TYPE,
	USB_TYPE
}UartType_enum;

typedef struct _uartdriver
{
	
	kal_bool (*Open)(UART_PORT port, module_type owner);
	void (*Close)(UART_PORT port);
	kal_uint16 (*GetBytes)(UART_PORT port, kal_uint8 *Buffaddr, kal_uint16 Length, kal_uint8 *status, module_type ownerid);
	kal_uint16 (*PutBytes)(UART_PORT port, kal_uint8 *Buffaddr, kal_uint16 Length, module_type ownerid);
	kal_uint16 (*GetRxAvail)(UART_PORT port);
	kal_uint16 (*GetTxAvail)(UART_PORT port);
	kal_uint16 (*PutISRBytes)(UART_PORT port, kal_uint8 *Buffaddr, kal_uint16 Length, module_type ownerid);
	kal_uint16 (*GetISRTxAvail)(UART_PORT port);
	void (*Purge)(UART_PORT port, UART_buffer dir);	
	void (*SetOwner)(UART_PORT port, kal_uint8 owner);
	void (*SetFlowCtrl)(UART_PORT port, kal_bool XON);
	void (*ConfigEscape)(UART_PORT port, kal_uint8 EscChar, kal_uint16 ESCGuardtime);
	void (*SetDCBConfig)(UART_PORT port, UARTDCBStruct *UART_Config);
	void (*CtrlDCD)(UART_PORT port, IO_level SDCD);
	void (*CtrlBreak)(UART_PORT port, IO_level SBREAK);
	void (*ClrRxBuffer)(UART_PORT port);
	void (*ClrTxBuffer)(UART_PORT port);	
	void (*SetBaudRate)(UART_PORT port, UART_baudrate baudrate);	
	kal_uint16 (*SendISRData)(UART_PORT port, kal_uint8 *Buffaddr, kal_uint16 Length,kal_uint8 mode, kal_uint8 escape_char, module_type ownerid);
	kal_uint16 (*SendData)(UART_PORT port, kal_uint8 *Buffaddr, kal_uint16 Length,kal_uint8 mode,kal_uint8 escape_char, module_type ownerid);
	module_type (*GetOwnerID)(UART_PORT port);	
	void (*SetAutoBaud_Div)(UART_PORT port);			
	/*TY adds these to expand flexibility 2004/10/15*/
	void (*UART_Register_TX_cb)(UART_PORT port, module_type owner, UART_TX_FUNC func);
	void (*UART_Register_RX_cb)(UART_PORT port, module_type owner, UART_RX_FUNC func);	
	/*TY adds these to let virtual COM port can retrive exception log 2005/3/8*/
	kal_uint8 (*GetUARTByte)(UART_PORT port);	
	void (*PutUARTByte)(UART_PORT port, kal_uint8 data);	
	void (*PutUARTBytes)(UART_PORT port, kal_uint8 *data, kal_uint16 len);		
}UartDriver_strcut;

typedef struct _uarthandle
{
	UartType_enum type;
	UartDriver_strcut* drv;
	void* dev;
}UartHandle_struct;

/*start of local parameter struct */

typedef struct  
{       
	kal_uint8 	ref_count;
	kal_uint16 	msg_len;			/* LOCAL_PARA_HDR */	
	UART_PORT 	port;
}uart_ready_to_read_ind_struct; 

typedef struct  
{       
	kal_uint8 	ref_count;
	kal_uint16 	msg_len;			/* LOCAL_PARA_HDR */	
	UART_PORT 	port;
}uart_ready_to_write_ind_struct;
 
 typedef struct  
{       
	kal_uint8 	ref_count;
	kal_uint16 	msg_len;			/* LOCAL_PARA_HDR */	
	UART_PORT 	port;
}uart_dsr_change_ind_struct;
 
 typedef struct  
{       
	kal_uint8 	ref_count;
	kal_uint16 	msg_len;			/* LOCAL_PARA_HDR */	
	UART_PORT 	port;
}uart_escape_detected_ind_struct;

 typedef struct  
{       
	kal_uint8 	ref_count;
	kal_uint16 	msg_len;			/* LOCAL_PARA_HDR */	
	UART_PORT 	port;
}uart_plugin_ind_struct;

 typedef struct  
{       
	kal_uint8 	ref_count;
	kal_uint16 	msg_len;			/* LOCAL_PARA_HDR */	
	UART_PORT 	port;
}uart_plugout_ind_struct;    
/*end of local parameter struct */
#define EnableRxIntr(_baseaddr)   \
{\
      kal_uint32 _savedMask;\
      kal_uint16 _IER;\
      _savedMask = SaveAndSetIRQMask();\
      _IER = DRV_Reg(UART_IER(_baseaddr));\
      _IER |= (UART_IER_ERBFI | UART_IER_ELSI);\
      DRV_WriteReg(UART_IER(_baseaddr),_IER);\
      RestoreIRQMask(_savedMask);\
}

#define DisableRxIntr(_baseaddr)   \
{\
   kal_uint16 _IER;\
   kal_uint32 _savedMask;\
   _savedMask = SaveAndSetIRQMask();\
   _IER = DRV_Reg(UART_IER(_baseaddr));\
   _IER &= ~(UART_IER_ERBFI|UART_IER_ELSI);\
   DRV_WriteReg(UART_IER(_baseaddr),_IER);\
   RestoreIRQMask(_savedMask);\
}


#define EnableTxIntr(_baseaddr)   \
{\
   kal_uint16 _IER;\
   kal_uint32 _savedMask;\
   _savedMask = SaveAndSetIRQMask();\
   _IER = DRV_Reg(UART_IER(_baseaddr));\
   _IER |= UART_IER_ETBEI;\
   DRV_WriteReg(UART_IER(_baseaddr),_IER);\
   RestoreIRQMask(_savedMask);\
}

#define DisableTxIntr(_baseaddr)   \
{\
   kal_uint16 _IER;\
   kal_uint32 _savedMask;\
   _savedMask = SaveAndSetIRQMask();\
   _IER = DRV_Reg(UART_IER(_baseaddr));\
   _IER &= ~UART_IER_ETBEI;\
   DRV_WriteReg(UART_IER(_baseaddr),_IER);\
   RestoreIRQMask(_savedMask);\
}

#define UART_SetDMAIntr(_baseaddr) \
{\
   kal_uint16 _IER;\
   kal_uint32 _savedMask;\
   _savedMask = SaveAndSetIRQMask();\
   _IER = DRV_Reg(UART_IER(_baseaddr));\
   _IER &= ~(UART_IER_ETBEI);\
   DRV_WriteReg(UART_IER(_baseaddr),_IER);\
   RestoreIRQMask(_savedMask);\
}

#define DisableRLSIntr(_baseaddr)   \
{\
   kal_uint16 _IER;\
   kal_uint32 _savedMask;\
   _savedMask = SaveAndSetIRQMask();\
   _IER = DRV_Reg(UART_IER(_baseaddr));\
   _IER &= ~(UART_IER_ELSI);\
   DRV_WriteReg(UART_IER(_baseaddr),_IER);\
   RestoreIRQMask(_savedMask);\
}

/*UART Customization */
typedef struct{
	kal_uint8*	rx_adrs;
	kal_uint8*	tx_adrs;
	#ifndef __DMA_UART_VIRTUAL_FIFO__
	kal_uint8*	txisr_adrs;
	#endif
	kal_uint16	rx_len;
	kal_uint16	tx_len;
	#ifndef __DMA_UART_VIRTUAL_FIFO__
	kal_uint16	txisr_len;
	#endif
	module_type	owerid;		// only the specified owner can use it
}UART_ring_buffer_struct;

typedef struct {
	UART_ring_buffer_struct ring[MAX_UART_PORT_NUM];
} UART_rings_struct;

typedef struct {  
   const UART_rings_struct * (*UART_Get_Data)(void);
}UART_customize_function_struct; 

/*Function Declaration*/
extern kal_bool UART1DMA_Ini(kal_bool Tx);
extern kal_bool UART2DMA_Ini(kal_bool Tx);
extern kal_uint8 GetUARTByte(UART_PORT port);
extern void PutUARTByte(UART_PORT port, kal_uint8 data);
extern void UART_SetBaudRate(UART_PORT port, UART_baudrate baud_rate);
extern void UART_SetDCBConfig(UART_PORT port, UARTDCBStruct *UART_Config);
extern void UART_ReadDCBConfig (UART_PORT port, UARTDCBStruct *DCB);
extern void UART_loopback(UART_PORT port);
extern void UART_HWInit(UART_PORT port);
extern kal_bool UART_Open(UART_PORT port, module_type owner);
extern void UART_Close(UART_PORT port);
extern void UART_SetOwner (UART_PORT port, kal_uint8 owner);
extern void UART_ConfigEscape (UART_PORT port, kal_uint8 EscChar, kal_uint16 ESCGuardtime);
extern void UART_CtrlDTR (UART_PORT port, IO_level SDTR);
extern void UART_ReadHWStatus(UART_PORT port, IO_level *SDSR, IO_level *SCTS);
extern void UART_CtrlBreak(UART_PORT port, IO_level SBREAK);
extern void UART_Purge(UART_PORT port, UART_buffer dir);
extern void UART_Register_RX_cb(UART_PORT port, module_type owner, UART_RX_FUNC func);
extern void UART_Register_TX_cb(UART_PORT port, module_type owner, UART_TX_FUNC func);

extern kal_uint16 UART_GetBytes(UART_PORT port, kal_uint8 *Buffaddr, kal_uint16 Length, kal_uint8 *status, module_type ownerid);
extern kal_uint16 UART_PutBytes(UART_PORT port, kal_uint8 *Buffaddr, kal_uint16 Length, module_type ownerid);
extern kal_uint16 UART_PutISRBytes(UART_PORT port, kal_uint8 *Buffaddr, kal_uint16 Length, module_type ownerid);
extern kal_uint16 UART_SendISRData(UART_PORT port, kal_uint8 *Buffaddr, kal_uint16 Length,kal_uint8 mode, kal_uint8 escape_char, module_type ownerid);
extern kal_uint16 UART_SendData(UART_PORT port, kal_uint8 *Buffaddr, kal_uint16 Length,kal_uint8 mode,kal_uint8 escape_char, module_type ownerid );
extern void UART_SetFlowCtrl(UART_PORT port, kal_bool XON);
extern void UART_CtrlDCD(UART_PORT port, IO_level SDCD);
extern kal_uint16 UART_GetBytesAvail(UART_PORT port);
extern void UART_SleepOnTx_Enable(UART_PORT port, UART_SLEEP_ON_TX enable_flag);
extern void UART_SwitchPort(UART_PORT *APP_port, UART_PORT new_uart_port);
extern void UART_dafault_tx_cb(UART_PORT port);
extern void UART_dafault_rx_cb(UART_PORT port);
extern void UART1_HISR(void);
extern void UART2_HISR(void);
extern void UART1_LISR(void);
extern void UART2_LISR(void);

/* Note: for ROM code start */
#ifdef __ROMSA_SUPPORT__
/*for mcu rom*/
extern kal_uint16 U_GetTxISRRoomLeft(UART_PORT port);
extern kal_uint16 U_PutISRBytes(UART_PORT port, kal_uint8 *Buffaddr, kal_uint16 Length, module_type ownerid);
extern kal_uint16 U_ROM_GetTxISRRoomLeft(UART_PORT port);
extern UARTStruct *U_ROM_GetUARTPort(UART_PORT port);
extern kal_uint8 *U_ROM_GetUART_TXilm(UART_PORT port);
extern void U_ROM_InformUARTOwner(UART_PORT port);
extern void U_ROM_PushDataToBuf(UART_PORT port, kal_uint8 *data, kal_uint32 real_count);
extern void U_ROM_EnableTxIntr(UART_PORT port);
//extern void DRVPDN_Disable(kal_uint32 addr,kal_uint16 code,kal_uint8 handle);
#endif
/* Note: for ROM code end */

#ifdef __UART3_SUPPORT__
   extern void UART3_HISR(void);
   extern void UART3_LISR(void);
#endif    
// for Uart Dispatch
extern void UART_Register(UART_PORT port, UartType_enum type, UartDriver_strcut* drv);
/*UART customize*/
extern void uart_customize_init(void);
#endif   /*UART_SW_H*/


