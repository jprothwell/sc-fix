#include "cs_types.h"

#include "hal_uart.h"
#include "device_drv.h"
#include <hal_debug.h>
#include "hal_gpio.h"
#include "hal_i2c.h"
#include "sxr_mem.h"
#include "sxr_sbx.h"
#include "sxs_io.h"
#include "sxr_tls.h"
#include "sxr_tim.h"
#include <string.h>
#include <stdio.h>
#include "mcd_m.h"
#include "fs.h"
#include "cfw.h"
#include "bt.h"
#include "btd_config.h"
#include "tgt_btd_cfg.h"

#ifdef I2C_BASED_ON_GPIO
#include "gpio_i2c.h"
#endif

extern  BOOL hasFreeEnv(UINT8 nMbxId);
extern u8 checkIsNoEnv(void);
extern void rda5868_uart_configure(unsigned baud,char flow_control);
extern void rdabt_bt_restart();

#define CIRCULAR_BUF_LEN  4096


#define MOD_BUFF_LEN(_val, _length)  ((_val) & (_length-1))
#define GET_DATA_BLOCK_LEN(_start,_end,_cycle) ((_start<=_end)?(_end-_start):(_end+_cycle - _start) )
#define STATIC_RX_BUF_LEN		512
#define STATIC_TX_BUF_LEN		512
#define UART_FIFO_LEN    32
#define MAX_UART_DEV_NUM   			   4
#define RDABT_RF_ADDR    0x16
#define RDABT_CORE_ADDR 0x15
#define I2C_MASTER_ACK              (1<<0)
#define I2C_MASTER_RD               (1<<4)
#define I2C_MASTER_STO              (1<<8)
#define I2C_MASTER_WR               (1<<12)
#define I2C_MASTER_STA              (1<<16)

PRIVATE HAL_SYS_CLOCK_OUT_ID_T g_RdaBtClockOutId = HAL_SYS_CLOCK_OUT_ID_QTY;
#define HAL_ASSERT(BOOL, format, ...)                   \
    if (!(BOOL)) {            \
        hal_DbgAssert(format, ##__VA_ARGS__);                             \
    }
typedef enum {
      fc_none=1,
      fc_hw,
      fc_sw
} UART_flowCtrlMode;
//#define NO_NODE   0xff

 struct  CircularBuf
{
	UINT8 Buf[CIRCULAR_BUF_LEN];	
	UINT16 Get;
	UINT16 Put;
	UINT16 Buf_len;
	UINT8 NotEmpty;
	UINT8  Name[10];
};
 struct
{
	UINT8 *rx_buf1;	
	UINT8  *tx_buf;	
}SRAM_BUF;

typedef struct
{
     UINT32                 baud; 
      UINT32             dataBits;
      UINT32                 stopBits;
      UINT32                   parity;
      UART_flowCtrlMode      flowControl;
      UINT8                     xonChar;
      UINT8                     xoffChar;
      UINT32                      DSRCheck;
}UARTDCBStruct;

DEVICE Uart_Dev[MAX_UART_DEV_NUM];


 INT32       fileHandle=0;
 HAL_GPIO_GPIO_ID_T 		g_GpioId;
 HAL_GPIO_IRQ_MASK_T       gpioMaskCfg;
 VOID(*g_func)(VOID);
void Uart1_IrqHandler (HAL_UART_IRQ_STATUS_T status, HAL_UART_ERROR_STATUS_T error) ;
void Uart2_IrqHandler (HAL_UART_IRQ_STATUS_T status, HAL_UART_ERROR_STATUS_T error) ;
UINT16 get_CircularBuffer_len(struct  CircularBuf *pCirBuff);


void  set_UART_Parameter(HAL_UART_IRQ_STATUS_T * pMask, HAL_UART_CFG_T *pCfg, UINT8 RxUseDMA,UINT8 TxUseDMA)
{	
	if(RxUseDMA)
		{
			
			pMask->rxDataAvailable=0;			
			pMask->rxTimeout       =0;        
			pMask->rxLineErr       =  0;       
			
			pMask->rxDmaDone       = 1;        
			pMask->rxDmaTimeout    = 1;  			
			pCfg->rx_trigger = HAL_UART_RX_TRIG_1; 	
			
			pCfg->rx_mode = HAL_UART_TRANSFERT_MODE_DMA_IRQ;	 
			//pCfg->tx_mode = HAL_UART_TRANSFERT_MODE_DMA_IRQ;	
		}
	else
		{		      
			pMask->rxDataAvailable=1;
			pMask->rxTimeout       =1;        
			pMask->rxLineErr       =  0;       
		     
			pMask->rxDmaDone       = 0;        
			pMask->rxDmaTimeout    = 0;  	
			
			pCfg->rx_trigger = HAL_UART_RX_TRIG_NEARFULL; 	
			pCfg->rx_mode = HAL_UART_TRANSFERT_MODE_DIRECT_IRQ;	 
			
		}	
	
	if(TxUseDMA)
		{
			pMask->txModemStatus=0;
			pMask->txDataNeeded= 0;
			pMask->txDmaDone       = 1;   
			pCfg->tx_mode = HAL_UART_TRANSFERT_MODE_DMA_IRQ;	
		}
	else
		{
			pMask->txModemStatus=0;
			pMask->txDataNeeded= 0;
			pMask->txDmaDone       = 0;   
			pCfg->tx_mode = HAL_UART_TRANSFERT_MODE_DIRECT_IRQ;		
		}
	
			pCfg->data = HAL_UART_8_DATA_BITS;	
			pCfg->stop = HAL_UART_1_STOP_BIT;
			pCfg->parity = HAL_UART_NO_PARITY;
	
		pCfg->tx_trigger = HAL_UART_TX_TRIG_EMPTY;	
			
		//pCfg->afc = HAL_UART_AFC_MODE_RX_TRIG_12;	
		pCfg->afc = HAL_UART_AFC_MODE_DISABLE;	
		pCfg->irda = HAL_UART_IRDA_MODE_DISABLE;			
		//pCfg->rate = HAL_UART_BAUD_RATE_921600;	
		pCfg->rate  =   HAL_UART_BAUD_RATE_115200;
	
	
}



VOID GPIO_Interrupt_handler()
{
	
	if(g_func!=NULL)
	{
		g_func();
	}

		    gpioMaskCfg.falling = FALSE;
		    gpioMaskCfg.rising = FALSE;
   	 hal_GpioIrqSetMask(g_GpioId, &gpioMaskCfg);
	SXS_TRACE(TSTDOUT,"clean gpio %d ",g_GpioId);
	hal_GpioResetIrq(g_GpioId);
	
	  gpioMaskCfg.falling = FALSE;
	  gpioMaskCfg.rising = TRUE;

	  hal_GpioIrqSetMask(g_GpioId, &gpioMaskCfg);

}
 VOID  GPIO_INT_Registration(HAL_APO_ID_T gpio, UINT8 type, void(*pfun)())
{	
	switch(type)
	{
		case 0:
		 	gpioMaskCfg.rising = TRUE;
			gpioMaskCfg.falling= FALSE;
		 	break;
		 case 1:
		 	gpioMaskCfg.rising = FALSE;
			gpioMaskCfg.falling= TRUE;
		 	break;
		case 2:
		 	gpioMaskCfg.rising = TRUE;
			gpioMaskCfg.falling= TRUE;
		 	break;
	}
	 gpioMaskCfg.debounce=TRUE;
	 gpioMaskCfg.level=FALSE;
	 g_GpioId=gpio.gpioId;
	 g_func=pfun;
	 hal_GpioSetIn(g_GpioId);
        hal_GpioIrqSetMask(g_GpioId, &gpioMaskCfg);
	 hal_GpioIrqSetHandler(g_GpioId ,GPIO_Interrupt_handler);
}

BOOL UART_Open(PORT port,UINT16 Ower_Task)
{		
	DEVICE *pDev=&Uart_Dev[port];
	if(pDev->port!=0)
	{
			SXS_TRACE(TSTDOUT,"error! Uart port %d  have existed",port);
			return FALSE;
	}	
	pDev->port=port;
	pDev->RecMbx=Ower_Task;
	pDev->RxUseDMA = 0;
	pDev->TxUseDMA = 0;//TRUE;
				  			
	 set_UART_Parameter(&pDev->mask, &pDev->uartCfg,pDev->RxUseDMA,pDev->TxUseDMA);			
		 
	pDev->pWriteBuf = (struct  CircularBuf*)sxr_Malloc( sizeof(struct  CircularBuf));
	if(pDev->pWriteBuf ==NULL)
		return FALSE;
	pDev->pReadBuf = (struct  CircularBuf*)sxr_Malloc( sizeof(struct  CircularBuf));
	if(pDev->pReadBuf ==NULL)
		return FALSE;
	SXS_TRACE(TSTDOUT,"UART_Open: pDev->port = %d, pDev->pWriteBuf = 0x%x, pDev->pReadBuf=0x%x\n",pDev->port,pDev->pWriteBuf,pDev->pReadBuf);				
	memset(pDev->pWriteBuf, 0 , sizeof(struct  CircularBuf));
	memset(pDev->pReadBuf,0, sizeof(struct  CircularBuf));
	pDev->pWriteBuf->Buf_len=CIRCULAR_BUF_LEN;	
	pDev->pReadBuf->Buf_len=CIRCULAR_BUF_LEN;
	sprintf(pDev->pReadBuf->Name, "Rx ");
	sprintf(pDev->pWriteBuf->Name, "Tx ");
	SRAM_BUF.rx_buf1=(UINT8*)sxr_Malloc(STATIC_RX_BUF_LEN);
	if(NULL==SRAM_BUF.rx_buf1)
		return FALSE;
	
	SRAM_BUF.tx_buf=(UINT8*)sxr_Malloc(STATIC_TX_BUF_LEN);
	if(NULL==SRAM_BUF.tx_buf)
		return FALSE;
	
	 g_RdaBtClockOutId = hal_SysClkOutOpen(HAL_SYS_CLOCK_OUT_FREQ_32K);
	    if (HAL_SYS_CLOCK_OUT_RESOURCE_UNAVAILABLE == g_RdaBtClockOutId)
	    {
	     SXS_TRACE(_MM," laixf error get sys clock out open:%d\n",pDev->port);	
	        return FALSE;
	    }
	
	
 	 return TRUE ;

}

void UART_SetDCBConfig(PORT port, UARTDCBStruct *pDcb)
{
	void (*Uart_ISR[2])(HAL_UART_IRQ_STATUS_T status, HAL_UART_ERROR_STATUS_T error);
 	DEVICE *pDev=&Uart_Dev[port];
	if(pDev->port!=port)
	{
			SXS_TRACE(TSTDOUT,"error! wrong uart %d",port);
			return ;
	}
	 hal_UartClose(pDev->port);
	 Uart_ISR[0]=Uart1_IrqHandler;
	Uart_ISR[1]=Uart2_IrqHandler;
	 pDev->uartCfg.rate=pDcb->baud; 	
	 SXS_TRACE(TSTDOUT,"set flowcont  %d ,%d",pDcb->flowControl,pDcb->baud);
	 if(pDcb->flowControl==fc_hw)
	 {
	 	pDev->uartCfg.afc = HAL_UART_AFC_MODE_RX_TRIG_16;
		SXS_TRACE(TSTDOUT,"set flowcont  yes");
	 }
	 else
	 {
	 	pDev->uartCfg.afc = HAL_UART_AFC_MODE_DISABLE;
		SXS_TRACE(TSTDOUT,"set flowcont  no");
	 }
	hal_UartOpen(pDev->port, &pDev->uartCfg);	
	hal_UartFifoFlush(pDev->port);	
//	hal_UartAllowSleep(pDev->port,TRUE);
	hal_UartClearErrorStatus(pDev->port);    	
	hal_UartSetRts(pDev->port, TRUE); 

	hal_UartIrqSetMask(pDev->port, pDev->mask);
	hal_UartIrqSetHandler(pDev->port,Uart_ISR[pDev->port-1]);


	 if(pDev->RxUseDMA)
	 {	
		 
	  	hal_UartGetData (pDev->port, SRAM_BUF.rx_buf1, STATIC_RX_BUF_LEN);	
	  	
	 }
	
	return;	 
}

void UART_Register_RX_cb(PORT port,UINT16 owner,void(*callfunc)(UINT8 port))
{
	DEVICE *pDev=&Uart_Dev[port];
	if(pDev->port!=port)
	{
			SXS_TRACE(TSTDOUT,"error! wrong uart %d",port);
			return ;
	}
	pDev->registered_func=callfunc;
	return;
}
BOOL UART_Close(PORT port)
{
	DEVICE *pDev=&Uart_Dev[port];
	if(pDev->port!=port)
	{
			SXS_TRACE(TSTDOUT,"error! wrong uart %d",port);
			return FALSE;
	}
 
	   hal_UartFifoFlush(pDev->port);
	   hal_UartClose(pDev->port);  
	   pDev->registered_func=NULL;
	  
	  if(pDev->pWriteBuf!=NULL)
	  {
	  	sxr_Free(pDev->pWriteBuf);
		pDev->pWriteBuf=NULL;
	  }
	  if(pDev->pReadBuf!=NULL)
	  {
	  	sxr_Free(pDev->pReadBuf);
		pDev->pReadBuf=NULL;
	  }
	  pDev->port=0;
	if(SRAM_BUF.rx_buf1!=NULL)
	{
		sxr_Free(SRAM_BUF.rx_buf1);
		SRAM_BUF.rx_buf1=NULL;
	}

	if(SRAM_BUF.tx_buf!=NULL)
	{
		sxr_Free(SRAM_BUF.tx_buf);
		SRAM_BUF.tx_buf=NULL;
	}
//	hal_UartForcePinsLow(BT_UART_PORT);
       if ((g_RdaBtClockOutId < HAL_SYS_CLOCK_OUT_ID_QTY) &&
            (g_RdaBtClockOutId >= HAL_SYS_CLOCK_OUT_ID_1))
       {
  	      hal_SysClkOutClose(g_RdaBtClockOutId);
       }
 	return TRUE;
  
}

UINT16 put_data_to_CircularBuffer(struct  CircularBuf *pCirBuff, UINT8 *pBuff,UINT16 uDataSize)
{
     UINT16 Block_Len, Remain_Len, real_Len, First_half, Second_half;
	HANDLE h = 0;
       h = hal_SysEnterCriticalSection();
	Block_Len = GET_DATA_BLOCK_LEN(pCirBuff->Get, pCirBuff->Put, pCirBuff->Buf_len);

	if ((Block_Len == 0) && (pCirBuff->NotEmpty))
	{
		Block_Len = pCirBuff->Buf_len;
	}

	Remain_Len = pCirBuff->Buf_len - Block_Len;
	if(Remain_Len==0)
	{
		SXS_TRACE(TSTDOUT,"%s  buffer overflow!!",pCirBuff->Name);
		hal_SysExitCriticalSection(h);
		asm("break 1");
		return 0;
	}
	if (uDataSize > Remain_Len)
		asm("break 1");
	real_Len = (uDataSize < Remain_Len) ? uDataSize : Remain_Len;

	if (real_Len > 0)/*circular buffer not full*/
	{
		if (pCirBuff->Put < pCirBuff->Get)
		{
			memcpy(&pCirBuff->Buf[pCirBuff->Put], pBuff, real_Len);
			pCirBuff->Put = MOD_BUFF_LEN(pCirBuff->Put + real_Len,pCirBuff->Buf_len);
		}
		else
		{
			First_half = pCirBuff->Buf_len - pCirBuff->Put;

			if (real_Len < First_half)
			{
				memcpy(&pCirBuff->Buf[pCirBuff->Put], pBuff, real_Len);
				pCirBuff->Put = MOD_BUFF_LEN(pCirBuff->Put + real_Len, pCirBuff->Buf_len);
			}
			else
			{
				memcpy(&pCirBuff->Buf[pCirBuff->Put], pBuff , First_half);
				Second_half = real_Len - First_half;
				pCirBuff->Put = MOD_BUFF_LEN((pCirBuff->Put + First_half),pCirBuff->Buf_len);

				memcpy(&pCirBuff->Buf[pCirBuff->Put], &pBuff[First_half], Second_half);
				pCirBuff->Put = MOD_BUFF_LEN((pCirBuff->Put + Second_half),pCirBuff->Buf_len);

			}

		}
		
		pCirBuff->NotEmpty = 1;
		
	}
	else
	{
		SXS_TRACE(TSTDOUT,"warning put len %x",real_Len);

	}
     hal_SysExitCriticalSection(h);
	return Remain_Len -real_Len;
}
BOOL Is_CircularBuffer_Empty(struct  CircularBuf *pCirBuff)
{
    return (!pCirBuff->NotEmpty);
}
UINT16 get_data_from_CircularBuffer(struct  CircularBuf *pCirBuff, UINT8 *pBuff,UINT16 uDataSize)
{
	UINT16 Block_Len, First_half, Second_half, real_len, real_sent_len, tmp_Get;
	HANDLE h = 0;
	
	real_sent_len = 0;		
	
    h = hal_SysEnterCriticalSection();
	Block_Len = GET_DATA_BLOCK_LEN(pCirBuff->Get, pCirBuff->Put, pCirBuff->Buf_len);

	if ((Block_Len == 0) && (pCirBuff->NotEmpty))
		Block_Len = pCirBuff->Buf_len;

	real_len = (Block_Len < uDataSize) ? Block_Len :  uDataSize;
	if (real_len > 0)/*have data to send*/
	{
		
		if (pCirBuff->Get < pCirBuff->Put)
		{
			memcpy(pBuff, &pCirBuff->Buf[pCirBuff->Get], real_len);
			tmp_Get = MOD_BUFF_LEN(pCirBuff->Get + real_len,pCirBuff->Buf_len);
		}
		else
		{

			First_half = pCirBuff->Buf_len  - pCirBuff->Get;

			if (real_len < First_half)
			{
				memcpy(pBuff, &pCirBuff->Buf[pCirBuff->Get], real_len);
				tmp_Get = MOD_BUFF_LEN(pCirBuff->Get + real_len,pCirBuff->Buf_len);
			}
			else
			{
				memcpy(pBuff, &pCirBuff->Buf[pCirBuff->Get], First_half);
				Second_half = real_len - First_half;
				tmp_Get = MOD_BUFF_LEN(pCirBuff->Get + First_half,pCirBuff->Buf_len);			
				
				memcpy(&pBuff[First_half], &pCirBuff->Buf[tmp_Get], Second_half);
				tmp_Get = MOD_BUFF_LEN(tmp_Get + Second_half,pCirBuff->Buf_len);				
				
			}
		}	
		
		
		pCirBuff->Get  = tmp_Get;
		  if (pCirBuff->Get == pCirBuff->Put)
				pCirBuff->NotEmpty = 0;		
		   
	}
	else
	{
		//SXS_TRACE(TSTDOUT,"Tx  buffer empty!!");
	}
	 hal_SysExitCriticalSection(h);
	return real_len;
}


static VOID UART_FlushCircuBuffer(PORT *port)
{
	DEVICE * pDev=&Uart_Dev[*port];
       UINT16  send_rel_len;
	if (!Is_CircularBuffer_Empty(pDev->pWriteBuf))
	{
		if(pDev->TxUseDMA)
		{
			
			while (!(hal_UartTxFinished(pDev->port)||hal_UartTxDmaDone(pDev->port)))
			{
				SXS_TRACE(TSTDOUT,"UART_FlashCircuBuffer TX DMA not finished \n");
				sxr_Sleep(165);
			}
			send_rel_len=get_data_from_CircularBuffer(pDev->pWriteBuf, SRAM_BUF.tx_buf,STATIC_TX_BUF_LEN);
			hal_UartSendData(pDev->port, SRAM_BUF.tx_buf, send_rel_len);
			SXS_TRACE(TSTDOUT,"write tx DMA laixf %x,%x",send_rel_len,hal_UartTxDmaDone(pDev->port));
			SXS_TRACE(TSTDOUT," %x,%x,%x", SRAM_BUF.tx_buf[0],SRAM_BUF.tx_buf[1],SRAM_BUF.tx_buf[2]);
		}
	}
}


extern volatile UINT32 total_realen;
extern volatile UINT32 total_uDatasize;
void DealWith_UartRxIrq(DEVICE *pDev,BOOL  DMA_used, BOOL  Timeout)
{
   UINT16 uDataSize, real_len=0;
   UINT8 *rxDmaBufEmpty ;
   // UINT8 *rxDmaBufLoad;
	rxDmaBufEmpty =  SRAM_BUF.rx_buf1 ;

	if(DMA_used)
	{   		
   		uDataSize = STATIC_RX_BUF_LEN;
   		if(Timeout)
             	{	             		
			hal_UartRxDmaPurgeFifo(pDev->port);
              	uDataSize=STATIC_RX_BUF_LEN-hal_UartRxDmaLevel(pDev->port);	
			
			while(!hal_UartRxDmaFifoEmpty(pDev->port));
			
			 if(!hal_UartRxDmaStop(pDev->port))
			  {
			  	 SXS_TRACE(TSTDOUT,"error 3!!laixf \n");
                                return;
			  }                   
             	}	
		
		 real_len = put_data_to_CircularBuffer(pDev->pReadBuf,  (VOID*) HAL_SYS_GET_UNCACHED_ADDR((UINT32)rxDmaBufEmpty),uDataSize);	
		  hal_UartGetData (pDev->port,rxDmaBufEmpty, STATIC_RX_BUF_LEN);
		 //if (real_len <=32)
		 //	hal_UartSetRts(pDev->port, FALSE);

	}
	else
	{			
		uDataSize  = 1;
 		while(uDataSize>0)
 		{
	 		uDataSize = hal_UartGetData (pDev->port,rxDmaBufEmpty, STATIC_RX_BUF_LEN);			
			if(uDataSize>0)	
			{
			  	real_len = put_data_to_CircularBuffer(pDev->pReadBuf, rxDmaBufEmpty,uDataSize);
				if (real_len <=32)
				{
					hal_UartSetRts(pDev->port, FALSE);
		 			SXS_TRACE(TSTDOUT,",hal_UartSetRts FLASE real_len = %d\n",real_len);
				}
			}		 
		 	//SXS_TRACE(TSTDOUT,"Rx laixf %x: %x,%x,%x,%x,\n",uDataSize,rxDmaBufEmpty[0],rxDmaBufEmpty[1],rxDmaBufEmpty[2],rxDmaBufEmpty[3]);
			uDataSize  = hal_UartRxFifoLevel(pDev->port);
		}		
		
	}
	
		if(pDev->registered_func!=NULL)
		{
			pDev->registered_func(pDev->port);
		}	

}
volatile BOOL TxDmaDone=1;
void DealWith_UartTxIrq(DEVICE *pDev,UINT8 DMA_used)
{
	UINT16  real_len=0;
	if(DMA_used)
	{
	 
		HAL_ASSERT(hal_UartTxDmaDone(pDev->port),"tx DMA irq done not 0");
		sxr_StopFunctionTimer(UART_FlushCircuBuffer);
	 	real_len=get_data_from_CircularBuffer(pDev->pWriteBuf, SRAM_BUF.tx_buf,STATIC_TX_BUF_LEN);

		if(real_len>0)
		{	
			HAL_ASSERT(hal_UartSendData(pDev->port, SRAM_BUF.tx_buf, real_len)!=0,"hal_UartSendData return 0");
			//SXS_TRACE(TSTDOUT,"tx DMA laixf %x, %x",real_len,hal_UartTxDmaDone(pDev->port));
			//SXS_TRACE(TSTDOUT," %x,%x,%x", SRAM_BUF.tx_buf[0],SRAM_BUF.tx_buf[1],SRAM_BUF.tx_buf[2]);
			/*if(get_CircularBuffer_len(pDev->pWriteBuf)==0)
			{
				pDev->mask.txDmaDone=0;
				hal_UartIrqSetMask(pDev->port, pDev->mask);
			}*/
					  
		}
		else
			TxDmaDone=1;
	
	 }
	else
	{
		real_len=get_data_from_CircularBuffer(pDev->pWriteBuf, SRAM_BUF.tx_buf,UART_FIFO_LEN);

		if(real_len>0)
		{	
			hal_UartSendData(pDev->port, SRAM_BUF.tx_buf, real_len);	
		
	 		SXS_TRACE(TSTDOUT,"tx laixf %x,%x,Normal ",real_len,DMA_used );
			//SXS_TRACE(TSTDOUT," %x,%x,%x", SRAM_BUF.tx_buf[0],SRAM_BUF.tx_buf[1],SRAM_BUF.tx_buf[2]);

			
		}
	
	 }
}

UINT32 IrqCount = 0;
UINT32 rxDataAvailableCount = 0;
UINT32 rxTimeoutCount = 0;
UINT32 txModemStatusCount = 0;
UINT32 IrqOtherCount = 0;
extern BOOL  IsFlowCtrlStart;
volatile static HAL_UART_IRQ_STATUS_T Uart1status;
volatile static HAL_UART_ERROR_STATUS_T Uart1reg;

void Uart1_GetStatus()
{
	SXS_TRACE(TSTDOUT,"Uart1_GetStatus uart cause = 0x%x, status = 0x%x",Uart1status,Uart1reg);
}
void Uart1_IrqHandler (HAL_UART_IRQ_STATUS_T status, HAL_UART_ERROR_STATUS_T reg) 
{
	Uart1status = status;
	Uart1reg = reg;
     IrqCount++;
     DEVICE *pDev;
     pDev=&Uart_Dev[PORT1];
	if(pDev->port!=PORT1)
	{
			SXS_TRACE(TSTDOUT,"error! wrong uart %d",PORT1);
			return ;
	}
	  if(status.rxDataAvailable)
	 {
	 	rxDataAvailableCount++;
	 	hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_HAL, 0x3907);
	 	  DealWith_UartRxIrq(pDev,FALSE,FALSE);  
		hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_HAL, 0x3907);  
	 }
	 else if(status.rxTimeout)
	 {
	 	rxTimeoutCount++;
	 	hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_HAL, 0x3906);
	 	DealWith_UartRxIrq(pDev,FALSE,TRUE); 
		hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_HAL, 0x3906); 
	 }
	 else if( status.rxDmaTimeout)
	 {  	
		hal_UartSetRts(pDev->port, FALSE); 	 
	 	//hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_HAL, 0x3903);
		DealWith_UartRxIrq(pDev,TRUE,TRUE);  
       	//hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_HAL, 0x3903);         
	  	hal_UartSetRts(pDev->port, TRUE);    	           
	 }
	 else if(status.rxDmaDone)
 	 { 	       
 	     //  hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_HAL, 0x3902);
 		DealWith_UartRxIrq(pDev,TRUE,FALSE);  
           //   hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_HAL, 0x3902);              
 	}
	  else if(status.txDmaDone )
	 {
	  	//hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_HAL, 0x3904);    
	 	DealWith_UartTxIrq(pDev,TRUE);
         	//hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_HAL, 0x3904);    
	 }
	 else if(status.txDataNeeded)
	 {
	 	DealWith_UartTxIrq(pDev,TRUE);
	 }
	 else if(status.txModemStatus)
	 {
	 	txModemStatusCount++;
	 }
	 else
	 {
	 	IrqOtherCount++;
	 }
	if (!hasFreeEnv(3))
	{
	     rda5868_uart_configure(921600/*460800*//*230400*/,IsFlowCtrlStart);	
	     if (!checkIsNoEnv())	 
    	     {
    	     	     rdabt_bt_restart();//rdabt_send_msg_to_head(MOD_BT, MSG_ID_BT_POWEROFF_REQ, NULL, NULL);
	  	     //UINT32 *intMsg = sxr_Malloc(4);
	  	    // *intMsg = 0x12345678;
		   //   sxr_Send(intMsg,pDev->RecMbx,SXR_QUEUE_FIRST);
	     }
		SXS_TRACE(TSTDOUT,"error! mail box have no chance to process wrong uart cause = 0x%x, status = 0x%x",status,reg);
	}
}


void Uart2_IrqHandler (HAL_UART_IRQ_STATUS_T status, HAL_UART_ERROR_STATUS_T error) 
{
     DEVICE *pDev;
     pDev=&Uart_Dev[PORT2];

	if(pDev->port!=PORT2)
	{
			SXS_TRACE(TSTDOUT,"error! wrong uart %d",PORT2);
			return ;
	}

	 if( status.rxDmaTimeout)
	 {  	
		hal_UartSetRts(pDev->port, FALSE); 	 
	 	//hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_HAL, 0x3903);
		DealWith_UartRxIrq(pDev,TRUE,TRUE);  
       	//hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_HAL, 0x3903);         
	  	hal_UartSetRts(pDev->port, TRUE);    	           
	 }
	 else if(status.rxDmaDone)
 	 {
 	      
 	      // hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_HAL, 0x3902);
 		DealWith_UartRxIrq(pDev,TRUE,FALSE);  
             // hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_HAL, 0x3902);    
               
 	}
	 else if(status.rxDataAvailable)
	 {
	 	// hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_HAL, 0x3905);
	 	  DealWith_UartRxIrq(pDev,FALSE,FALSE);  
		 //hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_HAL, 0x3905);  
	 }
	 else if(status.rxTimeout)
	 {
	 	//hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_HAL, 0x3906);
	 	DealWith_UartRxIrq(pDev,FALSE,TRUE); 
		// hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_HAL, 0x3906); 
	 }
 
	 if(status.txDmaDone )
	 {
	  	//hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_HAL, 0x3904);    
	 	DealWith_UartTxIrq(pDev,TRUE);
         	//hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_HAL, 0x3904);    
	 }
	 else if(status.txDataNeeded)
	 {
	 	DealWith_UartTxIrq(pDev,TRUE);
	 }
  
}

UINT16 get_CircularBuffer_len(struct  CircularBuf *pCirBuff)
{
   UINT16 Block_Len;
    Block_Len = GET_DATA_BLOCK_LEN(pCirBuff->Get, pCirBuff->Put, pCirBuff->Buf_len);

	if ((Block_Len == 0) && (pCirBuff->NotEmpty))
		Block_Len = pCirBuff->Buf_len;
	return Block_Len;
	
}
extern BOOL reset_in_process;
BOOL bt_test_uart=0;

UINT16  UART_PutBytes( PORT port, UINT8 *pBuff, UINT16 NumToWrite,UINT16 Owner)
{
 	UINT16 real_len,send_rel_len,Tx_fifo_avail;
	DEVICE * pDev=&Uart_Dev[port];
 	UINT8 Param = BT_UART_PORT;
	sxr_StopFunctionTimer(UART_FlushCircuBuffer);
	if(pDev->port!=port)
	{
			SXS_TRACE(TSTDOUT,"error! wrong uart %d",port);
			return 0;
	}
	 
	 if(pDev->pWriteBuf==NULL)
	 	return 0;
	real_len=put_data_to_CircularBuffer(pDev->pWriteBuf, pBuff,NumToWrite);

	if(pDev->TxUseDMA)
	{
		
		if(hal_UartTxDmaDone(pDev->port))
			{
				send_rel_len=get_data_from_CircularBuffer(pDev->pWriteBuf, SRAM_BUF.tx_buf,STATIC_TX_BUF_LEN);
				if(send_rel_len>0)
				{
					HAL_ASSERT(hal_UartSendData(pDev->port, SRAM_BUF.tx_buf, send_rel_len)!=0,"hal_UartSendData return 0");
					SXS_TRACE(TSTDOUT,"write tx DMA laixf %d,%x",send_rel_len,hal_UartTxDmaDone(pDev->port));
					//SXS_TRACE(TSTDOUT," %x,%x,%x", SRAM_BUF.tx_buf[0],SRAM_BUF.tx_buf[1],SRAM_BUF.tx_buf[2]);
					/*if(get_CircularBuffer_len(pDev->pWriteBuf)==0)
					{
						pDev->mask.txDmaDone=0;
						hal_UartIrqSetMask(pDev->port, pDev->mask);
					}*/
				}
			
			}
		else
		{
				pDev->mask.txDmaDone=1;
				hal_UartIrqSetMask(pDev->port, pDev->mask);
			SXS_TRACE(TSTDOUT,"TX DMA not finished \n");
			sxr_StartFunctionTimer(16384>>1, UART_FlushCircuBuffer, &Param, 0);
		}
	}
	else
	{
	
                    UINT32 suspend_time = 0;
                    UINT32  send_timer_out=0;
         
                    send_timer_out = hal_TimGetUpTime();
         
			while(!Is_CircularBuffer_Empty(pDev->pWriteBuf))
			{
		
				if (reset_in_process)
					return -1;
				bt_test_uart = 1;
				while(!hal_UartTxFinished(pDev->port))
				{
					if ((reset_in_process))
					{

                                     SXS_TRACE(TSTDOUT,"Uart Tx failed,reset_in_process = %d, send_timer_out =%d  \n",reset_in_process,send_timer_out);
					
						return -1;
					}
                            	suspend_time = hal_TimGetUpTime();

                             if (suspend_time - send_timer_out > 2*16384) {
                                return -1;
                              }
                         
				   //sxr_Sleep(1);
				}			
				Tx_fifo_avail=hal_UartTxFifoAvailable(pDev->port);
				send_rel_len=get_data_from_CircularBuffer(pDev->pWriteBuf, SRAM_BUF.tx_buf,Tx_fifo_avail);
				hal_UartSendData(pDev->port, SRAM_BUF.tx_buf, send_rel_len);
				/*SXS_TRACE(TSTDOUT,"tx laixf %x dir",send_rel_len);
				SXS_TRACE(TSTDOUT," %x,%x,%x", SRAM_BUF.tx_buf[0],SRAM_BUF.tx_buf[1],SRAM_BUF.tx_buf[2]);*/
			}
			bt_test_uart = 0;

		     send_timer_out=0;		
	
	}

       return NumToWrite;
 }

UINT16  UART_GetBytes(  PORT port, UINT8 *pBuff,UINT16 NumToRead,UINT8 *pstate,UINT16 Owner)
{
	UINT16 real_len;	
	DEVICE * pDev=&Uart_Dev[port];
 
	if(pDev->port!=port)
	{
			SXS_TRACE(TSTDOUT,"error! wrong uart %d",port);
			return 0;
	}
	  if(pDev->pReadBuf==NULL)
	  {
	  	*pstate=READ_BUFF_NULL;
	 	return 0;
	  }
	
	  
   	real_len=get_data_from_CircularBuffer(pDev->pReadBuf, pBuff,NumToRead);
	hal_UartSetRts(pDev->port,TRUE); 
	return real_len;
}

HAL_I2C_BUS_ID_T g_btdI2cBusId = HAL_I2C_BUS_ID_1;

VOID I2C_Open(VOID)
{

#ifdef I2C_BASED_ON_GPIO
    gpio_i2c_open();
#else
    g_btdI2cBusId = tgt_GetBtdConfig()->i2cBusId;
    hal_I2cOpen(g_btdI2cBusId);
#endif
 	
	return;
}
VOID I2C_Close(VOID)
{

#ifdef I2C_BASED_ON_GPIO
    gpio_i2c_close();
#else
    hal_I2cClose(g_btdI2cBusId);
#endif
 	
       return;
	 
}

void rdabt_iic_rf_write_data(unsigned char regaddr, const unsigned short *data, unsigned char datalen)
{
#ifdef I2C_BASED_ON_GPIO
     gpio_i2c_rdabt_rf_write_data(RDABT_RF_ADDR<<1,regaddr, data,datalen);
#else
    
	HAL_ERR_T halErr = HAL_ERR_RESOURCE_BUSY;
	UINT8 Data_half[2];
	UINT8 i=0;
	
	hal_I2cSendRawByte(g_btdI2cBusId,(RDABT_RF_ADDR<<1),I2C_MASTER_WR | I2C_MASTER_STA);
	hal_I2cSendRawByte(g_btdI2cBusId,regaddr,I2C_MASTER_WR);

	for(i=0;i<datalen-1;i++,data++)//data
	{
		Data_half[0]=*data>>8;
		Data_half[1]=*data&0x00FF;

		halErr=hal_I2cSendRawByte(g_btdI2cBusId,Data_half[0],I2C_MASTER_WR);
		halErr=hal_I2cSendRawByte(g_btdI2cBusId,Data_half[1],I2C_MASTER_WR);
	}
	Data_half[0]=*data>>8;
	Data_half[1]=*data&0x00FF;

	halErr=hal_I2cSendRawByte(g_btdI2cBusId,Data_half[0],I2C_MASTER_WR);
	halErr=hal_I2cSendRawByte(g_btdI2cBusId,Data_half[1],I2C_MASTER_WR|I2C_MASTER_STO);
#endif
//	SXS_TRACE(TSTDOUT, "rdabt_iic_rf_write_data: %d\n.",halErr);
}

void rdabt_iic_rf_read_data(unsigned char regaddr, unsigned short *data, unsigned char datalen)
{
#ifdef I2C_BASED_ON_GPIO
     gpio_i2c_rdabt_rf_read_data((RDABT_RF_ADDR<<1),regaddr, data,datalen);
#else

	UINT8 Data_half[2];
	UINT8 i=0;
	
	hal_I2cSendRawByte(g_btdI2cBusId,(RDABT_RF_ADDR<<1),I2C_MASTER_WR | I2C_MASTER_STA);
	hal_I2cSendRawByte(g_btdI2cBusId,regaddr,I2C_MASTER_WR);
	hal_I2cSendRawByte(g_btdI2cBusId,((RDABT_RF_ADDR<<1)|1),I2C_MASTER_WR | I2C_MASTER_STA);

	for(i=0;i<datalen-1;i++,data++)//data
	{	
		Data_half[0]=hal_I2cReadRawByte(g_btdI2cBusId,I2C_MASTER_RD);
		Data_half[1]=hal_I2cReadRawByte(g_btdI2cBusId,I2C_MASTER_RD);

		(*data)=Data_half[0]<<8;
		(*data)|=Data_half[1];
	}

	Data_half[0]=hal_I2cReadRawByte(g_btdI2cBusId,I2C_MASTER_RD);
	Data_half[1]=hal_I2cReadRawByte(g_btdI2cBusId,I2C_MASTER_RD | I2C_MASTER_ACK | I2C_MASTER_STO);

	(*data)=Data_half[0]<<8;
	(*data)|=Data_half[1];
#endif
//	SXS_TRACE(TSTDOUT, "rdabt_iic_rf_read_data: %d\n.");
}

/*
UINT8 RDA_RF_I2C_Read(UINT8 Regiter, UINT16 *pData)
{
	HAL_ERR_T halErr = HAL_ERR_RESOURCE_BUSY;
	UINT8 Data_half[2];
	
#ifdef I2C_BASED_ON_GPIO
     gpio_i2c_read_data(RDABT_RF_ADDR,Regiter, 1,Data_half, 2);
#else
	halErr=hal_I2cGetData(g_btdI2cBusId,RDABT_RF_ADDR,Regiter,Data_half,2);
#endif
  	
	 (*pData)=Data_half[1];
	  (*pData)|=Data_half[0]<<8;
	if(halErr==HAL_ERR_NO)
		return TRUE;
	else
		return FALSE;
}
*/

/*
UINT8  RDA_Core_I2C_Write(UINT32 Regiter, UINT32 Data)
{
	HAL_ERR_T halErr = HAL_ERR_RESOURCE_BUSY;
	UINT8 Data_half[4];
	UINT8 i;
	Data_half[0]=Data>>24;
	Data_half[1]=Data>>16;
	Data_half[2]=Data>>8;
	Data_half[3]=Data&0x00FF;
	
	hal_I2cSendRawByte(g_btdI2cBusId,(RDABT_CORE_ADDR<<1),I2C_MASTER_WR | I2C_MASTER_STA);
	hal_I2cSendRawByte(g_btdI2cBusId,Regiter>>24,I2C_MASTER_WR);
	hal_I2cSendRawByte(g_btdI2cBusId,Regiter>>16,I2C_MASTER_WR);
	hal_I2cSendRawByte(g_btdI2cBusId,Regiter>>8,I2C_MASTER_WR);
	hal_I2cSendRawByte(g_btdI2cBusId,Regiter,I2C_MASTER_WR);
	hal_I2cSendRawByte(g_btdI2cBusId,(RDABT_CORE_ADDR<<1),I2C_MASTER_WR | I2C_MASTER_STA);
	for(i=0;i<3;i++)
	{
		halErr=hal_I2cSendRawByte(g_btdI2cBusId,Data_half[i],I2C_MASTER_WR);
	}
	halErr=hal_I2cSendRawByte(g_btdI2cBusId,Data_half[3],I2C_MASTER_WR|I2C_MASTER_STO);
	//halErr=hal_I2cSendData(g_btdI2cBusId,RDABT_CORE_ADDR,Regiter,Data_half,4);	
	  SXS_TRACE(TSTDOUT, "write I2C: %d\n.",halErr);
	if(halErr==HAL_ERR_NO)
		return TRUE;
	else
		return FALSE;
}

*/


UINT8  rdabt_iic_core_write_data(UINT32 Regiter, CONST UINT32 *pData, UINT8 datalen)
{
#ifdef I2C_BASED_ON_GPIO
     gpio_i2c_rdabt_core_write_data(RDABT_CORE_ADDR<<1,Regiter, pData,datalen);
     return TRUE;
#else
	HAL_ERR_T halErr = HAL_ERR_RESOURCE_BUSY;
	UINT8 Data_half[4];
	UINT8 i,j;

	hal_I2cSendRawByte(g_btdI2cBusId,(RDABT_CORE_ADDR<<1),I2C_MASTER_WR | I2C_MASTER_STA);
	hal_I2cSendRawByte(g_btdI2cBusId,Regiter>>24,I2C_MASTER_WR);
	hal_I2cSendRawByte(g_btdI2cBusId,Regiter>>16,I2C_MASTER_WR);
	hal_I2cSendRawByte(g_btdI2cBusId,Regiter>>8,I2C_MASTER_WR);
	hal_I2cSendRawByte(g_btdI2cBusId,Regiter,I2C_MASTER_WR);
	hal_I2cSendRawByte(g_btdI2cBusId,(RDABT_CORE_ADDR<<1),I2C_MASTER_WR | I2C_MASTER_STA);

	for(j=0;j<datalen-1;j++,pData++)//data
	{
		Data_half[0]=*pData>>24;
		Data_half[1]=*pData>>16;
		Data_half[2]=*pData>>8;
		Data_half[3]=*pData&0x00FF;
		for(i=0;i<4;i++)
		{
			halErr=hal_I2cSendRawByte(g_btdI2cBusId,Data_half[i],I2C_MASTER_WR);
		}
	}
	Data_half[0]=*pData>>24;
	Data_half[1]=*pData>>16;
	Data_half[2]=*pData>>8;
	Data_half[3]=*pData&0x00FF;
	for(i=0;i<3;i++)
	{
		halErr=hal_I2cSendRawByte(g_btdI2cBusId,Data_half[i],I2C_MASTER_WR);
	}
	halErr=hal_I2cSendRawByte(g_btdI2cBusId,Data_half[3],I2C_MASTER_WR|I2C_MASTER_STO);
	//halErr=hal_I2cSendData(RDABT_CORE_ADDR,Regiter,Data_half,4);	
	SXS_TRACE(TSTDOUT, "write I2C: %d\n.",halErr);
	
	if(halErr==HAL_ERR_NO)
		return TRUE;
	else
		return FALSE;
 #endif
}

UINT8 rdabt_iic_core_read_data(UINT32 Regiter, UINT32 *pData, UINT8 datalen)
{
//	HAL_ERR_T halErr = HAL_ERR_RESOURCE_BUSY;
#ifdef I2C_BASED_ON_GPIO
     gpio_i2c_rdabt_core_read_data(RDABT_CORE_ADDR<<1,Regiter, pData,datalen);
#else
	UINT8 Data_half[4];
	UINT8 i,j;

	hal_I2cSendRawByte(g_btdI2cBusId,(RDABT_CORE_ADDR<<1),I2C_MASTER_WR | I2C_MASTER_STA);
	hal_I2cSendRawByte(g_btdI2cBusId,Regiter>>24,I2C_MASTER_WR);
	hal_I2cSendRawByte(g_btdI2cBusId,Regiter>>16,I2C_MASTER_WR);
	hal_I2cSendRawByte(g_btdI2cBusId,Regiter>>8,I2C_MASTER_WR);
	hal_I2cSendRawByte(g_btdI2cBusId,Regiter,I2C_MASTER_WR);
	hal_I2cSendRawByte(g_btdI2cBusId,((RDABT_CORE_ADDR<<1)|1),I2C_MASTER_WR | I2C_MASTER_STA);

	for(j=0;j<datalen-1;j++,pData++)//data
	{	
		for(i=0;i<4;i++)
		{
			Data_half[i]=hal_I2cReadRawByte(g_btdI2cBusId,I2C_MASTER_RD);
		}
		(*pData)=Data_half[3];
		(*pData)|=Data_half[2]<<8;
		(*pData)|=Data_half[1]<<16;
		(*pData)|=Data_half[0]<<24;
		//	 SXS_TRACE(TSTDOUT, "Read I2C: %d\n.",halErr);
	}
	for(i=0;i<3;i++)
	{
		Data_half[i]=hal_I2cReadRawByte(g_btdI2cBusId,I2C_MASTER_RD);
	}
	Data_half[3]=hal_I2cReadRawByte(g_btdI2cBusId,I2C_MASTER_RD | I2C_MASTER_ACK | I2C_MASTER_STO);
	(*pData)=Data_half[3];
	(*pData)|=Data_half[2]<<8;
	(*pData)|=Data_half[1]<<16;
	(*pData)|=Data_half[0]<<24;
#endif
    SXS_TRACE(TSTDOUT, "Read I2C: 0x%x\n.",*pData);
	
	return TRUE;
}
/*
BOOL Is_Mcd_exist()
{
    MCD_CSD_T mcdCsd;
    MCD_ERR_T errorStatus;
    MCD_CARD_VER mcdVer = MCD_CARD_V2;
    BOOL bRet = TRUE;
    do
    {
#ifdef DUAL_TFLASH_SUPPORT    
        errorStatus = mcd_Open(MCD_CARD_ID_0,&mcdCsd,MCD_CARD_V2);
        if(errorStatus != MCD_ERR_NO)
        {
           mcd_Close(MCD_CARD_ID_0); 
           errorStatus = mcd_Open(MCD_CARD_ID_0,&mcdCsd,MCD_CARD_V2);
           if(errorStatus != MCD_ERR_NO)
           {
               mcd_Close(MCD_CARD_ID_0); 
               errorStatus = mcd_Open(MCD_CARD_ID_0,&mcdCsd,MCD_CARD_V1);
           }
        }
       SXS_TRACE(TSTDOUT, "T-flash: %d\n.",errorStatus);
        if(errorStatus != MCD_ERR_NO)
        {     
            bRet =  FALSE;
        }   
        else
        {
            bRet = TRUE;
            mcd_Close(MCD_CARD_ID_0);
            break;
        }

        errorStatus = mcd_Open(MCD_CARD_ID_1,&mcdCsd,MCD_CARD_V2);
        if(errorStatus != MCD_ERR_NO)
        {
           mcd_Close(MCD_CARD_ID_1); 
           errorStatus = mcd_Open(MCD_CARD_ID_1,&mcdCsd,MCD_CARD_V2);
           if(errorStatus != MCD_ERR_NO)
           {
               mcd_Close(MCD_CARD_ID_1); 
               errorStatus = mcd_Open(MCD_CARD_ID_1,&mcdCsd,MCD_CARD_V1);
           }
        }
       SXS_TRACE(TSTDOUT, "T-flash: %d\n.",errorStatus);
        if(errorStatus != MCD_ERR_NO)
        {     
            bRet =  FALSE;
        }   
        else
        {
            bRet = TRUE;
            mcd_Close(MCD_CARD_ID_1);
            break;
        }       
#else    
        errorStatus = mcd_Open(&mcdCsd,MCD_CARD_V2);
        if(errorStatus != MCD_ERR_NO)
        {
           mcd_Close(); 
           errorStatus = mcd_Open(&mcdCsd,MCD_CARD_V2);
           if(errorStatus != MCD_ERR_NO)
           {
               mcd_Close(); 
               errorStatus = mcd_Open(&mcdCsd,MCD_CARD_V1);
           }
        }
       SXS_TRACE(TSTDOUT, "T-flash: %d\n.",errorStatus);
        if(errorStatus != MCD_ERR_NO)
        {     
            bRet =  FALSE;
        }   
        else
        {
            bRet = TRUE;
            mcd_Close();
            break;
        }

        errorStatus = mcd_Open(&mcdCsd,MCD_CARD_V2);
        if(errorStatus != MCD_ERR_NO)
        {
           mcd_Close(); 
           errorStatus = mcd_Open(&mcdCsd,MCD_CARD_V2);
           if(errorStatus != MCD_ERR_NO)
           {
               mcd_Close(); 
               errorStatus = mcd_Open(&mcdCsd,MCD_CARD_V1);
           }
        }
       SXS_TRACE(TSTDOUT, "T-flash: %d\n.",errorStatus);
        if(errorStatus != MCD_ERR_NO)
        {     
            bRet =  FALSE;
        }   
        else
        {
            bRet = TRUE;
            mcd_Close();
            break;
        }    
#endif 
    }while(0);
  
    return bRet;
}

*/
VOID comm_demo_OEM2Unicode(UINT8* pszBuff)
{
   
    UINT8* pUBuff = NULL;
    UINT32 iOLen = 0;
    UINT32 iULen = 0;
    UINT32 iResult;

    if(NULL == pszBuff)
    {
        return;
    }    
    iOLen = (UINT16)strlen(pszBuff);
    iResult = ML_LocalLanguage2UnicodeBigEnding(pszBuff,iOLen,&pUBuff,&iULen,NULL);
    if(0 == iResult)
    {
        memcpy(pszBuff,pUBuff,iULen);
        pszBuff[iULen] = 0;
        pszBuff[iULen + 1] = 0;
    }
    if(NULL != pUBuff)
    {
       CSW_Free(pUBuff);
    }
    
}


