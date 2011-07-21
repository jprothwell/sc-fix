
/*************************************************************************/
/*                                                                                                                            */
/*            Copyright (C) 2007, Coolsand Technologies, Inc.                                          */
/*                            All Rights Reserved                                                                    */
/*                                                                                                                            */
/*************************************************************************/

#include "cs_types.h"
#include "drv.h"
#include "fm_drv_rda5800.h"
#include "fm_drv_i2c.h"

// Define
#define DELAY  100

//#define SCL_PIN  1<<24
//#define SDA_PIN 1<<25

// Global variant

u32 SCL_PIN,SDA_PIN;

//
//-------------------------------------------------------------------
// Function:  I2C_Delay
// Purpose:  This function is used to delay   
// Return:    

//-------------------------------------------------------------------
void _I2CDelay(u16 iCount)
{
	u16 i = 0;

	for(i = 0;i < iCount;i++) ;	
}

//
//-------------------------------------------------------------------
// Function:  HWInitI2C
// Purpose:  This function is used to init I2C port of the FM device   
// In: 
// Return:    bool

//-------------------------------------------------------------------
bool HWInitI2C(u8 scl,u8 sda)
{
	//SXS_TRACE(TSTDOUT,"init I2C ports... \r\n");
	//u32 iGpioSetting = hal_board_config.Used_Gpio;

	SCL_PIN = scl;
	SDA_PIN = sda;
	 
	//iTemp = SCL_PIN | SDA_PIN ;

#if (iTemp & iGpioSetting != iTemp)
	//#error "GPIO FUNCTION not been set in board_config file"
#endif

	// Set the GPIO pin to output status
	DRV_GpioSetDirection(SCL_PIN,1);
	DRV_GpioSetDirection(SDA_PIN,1);
	_I2CDelay(DELAY);
	
	// Make the I2C bus in idle status
	DRV_GpioSetLevel(SCL_PIN,1);
	_I2CDelay(DELAY);
	DRV_GpioSetLevel(SDA_PIN,1);
	_I2CDelay(DELAY);
	
	return (TRUE);
}
//
//-------------------------------------------------------------------
// Function:  _I2CStart
// Purpose:  This function is used to generate start conditions in I2C bus  
// In: 
// Return:    

//-------------------------------------------------------------------
static void _I2CStart(void)
{
    // How to generate the start conditions
	// The SDA pin change from High to low when the SCL pin is kept in high level
	
	// Make sure the SDA pin is a ouput port
	DRV_GpioSetDirection(SDA_PIN,1);
	_I2CDelay(DELAY);

    	DRV_GpioSetLevel(SDA_PIN,1);
	_I2CDelay(DELAY);
	DRV_GpioSetLevel(SCL_PIN,1);
	_I2CDelay(DELAY);
 
	DRV_GpioSetLevel(SDA_PIN,0);	
	_I2CDelay(DELAY);
    	DRV_GpioSetLevel(SCL_PIN,0);
	_I2CDelay(DELAY);
	
}

//
//-------------------------------------------------------------------
// Function:  _I2CStop
// Purpose:  This function is used to generate stop conditions in I2C bus  
// In: 
// Return:    

//-------------------------------------------------------------------
static void _I2CStop(void)
{
    // How to generate the stop conditions
	// The SDA pin change from Low to High when the SCL pin is kept in high level
	
	// Make sure the SDA pin is a ouput port
	DRV_GpioSetDirection(SDA_PIN,1);
	_I2CDelay(DELAY);

    	DRV_GpioSetLevel(SDA_PIN,0);
	_I2CDelay(DELAY);
	DRV_GpioSetLevel(SCL_PIN,1);
	_I2CDelay(DELAY);
 
	DRV_GpioSetLevel(SDA_PIN,1);	
	_I2CDelay(DELAY * 2);
    
	
}

//
//-------------------------------------------------------------------
// Function:  _I2CGetAck
// Purpose:  This function is used to get ACK/NACK bit from the RECV device 
// In: 
// Return:  bool TRUE, ACK bit FALSE,NON-ACK bit    

//-------------------------------------------------------------------
static bool _I2CGetAck(void)
{
	bool isAck;
	u8 level = 0;

	DRV_GpioSetDirection(SDA_PIN,1);
	_I2CDelay(DELAY);
	
	DRV_GpioSetLevel(SDA_PIN,1);
	_I2CDelay(DELAY);
	
	DRV_GpioSetDirection(SDA_PIN,0);
	DRV_GpioSetLevel(SCL_PIN,1);	
	_I2CDelay(DELAY);

	DRV_GpioGetLevel(SDA_PIN,&level);
	if(0 == level)
		isAck = TRUE;
	else
		isAck = FALSE;
	
	DRV_GpioSetLevel(SCL_PIN,0);	
	_I2CDelay(DELAY);
	
	return isAck;
}


//
//-------------------------------------------------------------------
// Function:  _I2CGetAck
// Purpose:  This function is used to send ACK/NACK bit to the transmit device 
// In: bool TRUE, ACK bit FALSE,NON-ACK bit
// Return:      

//-------------------------------------------------------------------
static void _I2CPutAck(bool isAck)
{
    	DRV_GpioSetDirection(SDA_PIN,1);
	_I2CDelay(DELAY);
	
	if(isAck)
	   DRV_GpioSetLevel(SDA_PIN,0);
    else
	   DRV_GpioSetLevel(SDA_PIN,1);	

	_I2CDelay(DELAY);
	DRV_GpioSetLevel(SCL_PIN,1);
	_I2CDelay(DELAY);
	DRV_GpioSetLevel(SCL_PIN,0);
	_I2CDelay(DELAY);
	
}

//
//-------------------------------------------------------------------
// Function:  _I2SendByte
// Purpose:  This function is used to send a byte data
// In: dat, the data ready to send
// Return:      

//-------------------------------------------------------------------
static void _I2CSendByte(u8 dat)
{
	u8 iSendCount = 8;

	DRV_GpioSetDirection(SDA_PIN,1);
	_I2CDelay(DELAY);
	
	do
	{
       	 if((dat & 0x80)==0)
        	{
			DRV_GpioSetLevel(SDA_PIN,0);
       	 }
		else
		{
            		DRV_GpioSetLevel(SDA_PIN,1);
		}	
		dat <<= 1;
		_I2CDelay(DELAY);
		DRV_GpioSetLevel(SCL_PIN,1);	
		_I2CDelay(DELAY);
		DRV_GpioSetLevel(SCL_PIN,0);	
		_I2CDelay(DELAY);
	} while ( --iSendCount != 0 );

}

//
//-------------------------------------------------------------------
// Function:  _I2RecvByte
// Purpose:  This function is used to receive a byte data
// In: 
// Return:   a byte of recv data     

//-------------------------------------------------------------------
static u8 _I2C_RecvByte(void)
{
	u8 dat = 0,iRecvCount = 8;
	u8 level = 0;
		
	DRV_GpioSetDirection(SDA_PIN,1); 
    	_I2CDelay(DELAY);
    	DRV_GpioSetLevel(SDA_PIN,1);
    	_I2CDelay(DELAY);
    	DRV_GpioSetDirection(SDA_PIN,0);
    	_I2CDelay(DELAY);
	
	do
	{
		DRV_GpioSetLevel(SCL_PIN,1);
		_I2CDelay(DELAY);
		dat <<= 1;

		DRV_GpioGetLevel(SDA_PIN,&level);
		if( 0 != level)
		{
			dat++;
		}
		
		DRV_GpioSetLevel(SCL_PIN,0);
		_I2CDelay(DELAY);
	} while ( --iRecvCount != 0 );

	return dat;

}



//
//-------------------------------------------------------------------
// Function:  I2SendData
// Purpose:  This function is used to Send a bunch of data to I2C bus 
// In: pSendBuff, the buffer save the data ready to send
//     iSendCount, the byte count ready to send 
//     iDeviceAddr, the device Addr  
//     iDeviceSubAddr, the device sub Addr
//     bHaveSubAddr,  check if need to seek the sub Addr
// Return:   pSendCount ,the count actually send
             

//-------------------------------------------------------------------
bool I2CSendData(u8 iDeviceAddr,u8 iDeviceSubAddr,u8 *pSendBuff,u8 iSendCount,u8 *pSendCount,bool bHaveSubAddr)
{
   u8 iCount = 0;

   *pSendCount = 0;
   
   // Start I2C bus
   _I2CStart();

   // Send the device addr
   _I2CSendByte(iDeviceAddr);   
   if(!_I2CGetAck())
	   return FALSE;

   if(bHaveSubAddr)
   {    
	   // Send the device addr
       _I2CSendByte(iDeviceSubAddr);   
       if(!_I2CGetAck())
			return FALSE;
   }
  
   for(iCount = 0;iCount < iSendCount;iCount++)
   {
	   _I2CSendByte( pSendBuff[iCount]);	   

	   if(!_I2CGetAck())
	   {
		  _I2CStop();
		  return FALSE;
	   }
	   *pSendCount += 1;
    }

   _I2CStop();
   return TRUE;
		
}

//
//-------------------------------------------------------------------
// Function:  I2RecvData
// Purpose:  This function is used to receive a bunch of data from I2C bus 
// In: pRecvBuff, the buffer save the recv data
//     iRecvCount, the byte count prepare to receive 
//     iDeviceAddr, the device Addr  
//     iDeviceSubAddr, the device sub Addr
//     bHaveSubAddr,  check if need to seek the sub Addr
// Return:   pSendCount ,the count actually send
             

//-------------------------------------------------------------------
bool I2CRecvData(u8 iDeviceAddr,u8 iDeviceSubAddr,u8 *pRecvBuff,u8 iRecvCount,bool bHaveSubAddr)
{
   u8 iCount = 0;
   
   // Start I2C bus
   _I2CStart();

   // Send the device addr
   _I2CSendByte(iDeviceAddr + 1);   
   if(!_I2CGetAck())
	   return FALSE;

   if(bHaveSubAddr)
   {    
	   // Send the device addr
       _I2CSendByte(iDeviceSubAddr);   
       if(!_I2CGetAck())
		return FALSE;
   }
  
   for(iCount = 0;iCount < (iRecvCount - 1);iCount++)
   {
	   pRecvBuff[iCount] = _I2C_RecvByte();	   
	   _I2CPutAck(TRUE);
    }

   pRecvBuff[iCount] = _I2C_RecvByte();
   _I2CPutAck(FALSE);
   _I2CStop();
   return TRUE;   
}


// EOF








