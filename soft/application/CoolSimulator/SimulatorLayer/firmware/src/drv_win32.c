/******************************************************************************/
/*                                                                            */
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* This source code is property of Coolsand. The information contained in     */
/* this file is confidential. Distribution, reproduction, as well as          */
/* exploitation,or transmisison of any content of this file is not            */
/* allowed except if expressly permitted.                                     */
/* Infringements result in damage claims!                                     */
/*                                                                            */
/* FILE NAME                                                                  */
/*      drv_win32.c                                                                 */
/*                                                                            */
/* DESCRIPTION                                                                */
/*   drivers API form CSW platform.                                           */
/*   See CSW Development Manual for more.                                     */
/*                                                                            */
/******************************************************************************/
#include "cswtype.h"

#include "drv.h"


typedef enum
{
    CAM_FLASH_NONE = 0x00,
    CAM_FLASH_AUTO = 0x01,
    CAM_FLASH_FORCED = 0x02,
    CAM_FLASH_FILLIN = 0x04,
    CAM_FLASH_REDEYEREDUCE = 0x08
} CAM_FLASH_T;

UINT32 SRVAPI DRV_LcdInit (
                    VOID
                    );

UINT32 SRVAPI DRV_LcdExit (
                    VOID
                    );

UINT32 SRVAPI DRV_LcdIoControl (
                         UINT32 uCode,
                         UINT32 nParamIn,
                         UINT32* pParamOut
                         );

UINT32 SRVAPI DRV_LcdSetPixel16(
                  UINT16 nXDest,
                  UINT16 nYDest,
                  UINT16 pPixelData
                  );

UINT32 SRVAPI DRV_LcdBlt16(
                  UINT16 nXDest,
                  UINT16 nYDest,
                  UINT16 nSrcWidth,
                  UINT16 nSrcHeight,
                  UINT16* pPixelData
                  );

UINT32 SRVAPI DRV_LcdBmpBlt16(
  INT16  nXDest, // LCD position x
  INT16  nYDest, // LCD position y
  INT16  nXSrc,  // bmp   x
  INT16  nYSrc,  // bmp   y
  INT16  nWidth, // bmp   w
  INT16  nHeight, // bmp  h
  UINT16* pBmpData
);

UINT32 SRVAPI DRV_SubLcdInit (
                       VOID
                       );

UINT32 SRVAPI DRV_SubLcdExit (
                       VOID
                       );

UINT32 SRVAPI DRV_SubLcdIoControl (
                            UINT32 uCode,
                            UINT32 nParamIn,
                            UINT32* pParamOut
                            );

UINT32 SRVAPI DRV_SubLcdSetPixel12(
                  UINT16 nXDest,
                  UINT16 nYDest,
                  UINT16 nPixelData
                  );

UINT32 SRVAPI DRV_SubLcdBlt(
                     UINT8 nXDest,
                     UINT8 nYDest,
                     UINT8 nSrcWidth,
                     UINT8 nSrcHeight,
                     UINT16* pRgbData
                     );

//
// GPIO 
//


UINT32 SRVAPI DRV_GpioSetDirection(
  UINT8 nGpio,     
  UINT8 nDirection     
){}

UINT32 DRV_GpioSetMode( 
    UINT8 nGpio,
    UINT8 nMode
){}

UINT32 SRVAPI DRV_GpioGetDirection(
  UINT8 nGpio,     
  UINT8* pDirection     
);

UINT32 SRVAPI DRV_GpioSetLevel(
  UINT8 nGpio,
  UINT8 nLevel
){}

UINT32 SRVAPI DRV_GpioGetLevel(
    UINT8 nGgpio,
    UINT8* pLevel
){}

UINT32 SRVAPI DRV_GpioSetEdgeIndication(
  UINT8 nGpio,
  UINT8 nEdge,
  UINT32 nDebonce
){}

UINT32 SRVAPI DRV_EXTISetEdgeIndication(
    UINT8  nEdge
);



PVOID SRVAPI DRV_MemorySetupCS(
  UINT8 nCS,
  DRV_MEM_CFG*  pMemCfg
);

UINT32 SRVAPI DRV_MemoryDisableCS(
  UINT8 nCS
);

//
// PWML
//
UINT32 SRVAPI DRV_SetPwl1(
  UINT8 nLevel
){}

UINT32 SRVAPI DRV_SetPwl0(
  UINT8 nLevel
){}
/*************************************************
 *** UART
 *************************************************/



/*
description:	open the uart
parameters:	UartID 		the uart number.
			drv_uartcfg	the configuration for the specific uart
			user_uart_callback		the callback function point, which will be
called when the transfer is complete, or there has received data in the fifo the first byte.
return value:	0 for successful, negative value for fail
*/
INT16 DRV_UartOpen(DRV_UARTID UartID, 
	DRV_UARTCFG *drv_uartcfg, UARTCALLBACK user_uart_callback);

/*
description :	send the length bytes to uart, before using it, please call open first.
it will not be returned until the total length of the data has been transfered
parameters:		UartID 	the uart number
				buff		the buffer of the data which will be transfered
				length	the length of the data which will be transfered
return value:	positive value for successful, negative value for fail
*/
INT32 DRV_UartSend(DRV_UARTID UartID, UINT8 *buff, UINT16 length);

/*
description:		receive the length bytes from uart.before using it, please call 
DRV_UartGetReceiveLen to check the length bytes in the fifo. in the DMA mode you 
should call DRV_uart_RxDmaDone the check if the DMA has been completed.
parameters:		UartID 	the uart number
				buff		the buffer of the data which will be transfered
				length	the length of the data which will be transfered
return value:	positive value for successful, negative value for fail
*/
INT32 DRV_UartReceive(DRV_UARTID UartID, UINT8 *buff, UINT16 length);

/*
after finish using the uart , we can call it to close the uart
parameters:		UartID 	the uart number
return value:	positive value for successful, negative value for fail
*/
INT16 DRV_UartClose(DRV_UARTID UartID);

/*
get the length bytes in the fifo, then call receive function to get the data,it just works
in no-DMA mode
parameters:		UartID 	the uart number
return value:	positive value for successful, negative value for fail
*/
INT16 DRV_UartGetReceiveLen(DRV_UARTID UartID);

/*
check if the DMA receive has been done, it can only be called when the uart
works in DMA mode
parameters:		UartID 	the uart number
return value:	it will return 1, otherwise it will return negative value
*/
INT16 DRV_uart_RxDmaDone( DRV_UARTID UartID );


UINT32 DRV_SPIInit( DRV_SpiCfg*spicfg);


VOID DRV_SPIExit(DRV_SpiCs_t cs);

void DRV_WaitSPIRxDmaDone(
	unsigned char *datain,
	unsigned int inlen
	);

UINT32 DRV_SPIWrite(DRV_SpiCs_t cs,UINT8 *data, UINT16 len);

UINT32 DRV_SPIRead(DRV_SpiCs_t cs,UINT8 *outbuf,UINT16 len);

void DRV_SPISetISRmask(
	UINT16 mask
	);

UINT8 DRV_Spi_TxFinished(DRV_SpiCs_t cs);
UINT32 DRV_ISRSpiRead(
	unsigned char *add ,
	unsigned int len
	);
UINT32 DRV_GetSpiIsrFifoLen(
	void
	);

void DRV_SPI_ChangeCs(
	DRV_SpiCfg *spicfg
	);
UINT32 DRV_SetKeyBacklight( UINT8 nLevel)
{
return 0;
}

void camera_SetFlashL(CAM_FLASH_T Flash)
{
}

