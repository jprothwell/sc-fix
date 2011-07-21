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
 *    gpio.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This Module defines the GPIO driver.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#include "drv_comm.h"
 
#include "gpio_hw.h"
#include "gpio_sw.h"

#if ( (defined(MT6208)))
   /*conf_data = 0~3*/
   void GPIO_ModeSetup(kal_uint16 pin, kal_uint16 conf_dada)
    {
      kal_uint16 modeno;
      kal_uint16 remainder;
      kal_uint16 data;
      modeno = pin/8;
      remainder = pin % 8;
      data=DRV_Reg(GPIO_MODE(modeno)); 
      data&=~(0x3 << (2*remainder));
      data|= (conf_dada << (remainder*2));
      DRV_WriteReg(GPIO_MODE(modeno),data);
  

   }
   
   /*conf_data = 0~3*/
   void GPO_ModeSetup(kal_uint16 pin, kal_uint16 conf_dada)
   {

     kal_uint16 mode; 
      
      mode = DRV_Reg(GPI_O_MODE);
      mode &= ~(0x0003 << (pin*2));
      mode |= (conf_dada << (pin*2));
      DRV_WriteReg(GPI_O_MODE,mode);
   }
#endif    

#ifdef MT6205
   /*conf_data = 0~3*/
   void GPIO_ModeSetup(kal_uint16 pin, kal_uint16 conf_dada)
   {
/*
      kal_uint16 modeno;
      kal_uint16 remainder;
      
      modeno = pin/8;
      remainder = pin % 8;
      
      *(GPIO_MODE(modeno)) &= ~(0x0003 << (remainder*2));
      *(GPIO_MODE(modeno)) |= (conf_dada << (remainder*2));
      */
   }
   
   /*conf_data = 0~3*/
   void GPO_ModeSetup(kal_uint16 pin, kal_uint16 conf_dada)
   {
/*
      kal_uint16 mode; 
      
      mode = DRV_Reg(GPIO_MODE4);
      mode &= ~(0x0003 << (pin*2));
      mode |= (conf_dada << (pin*2));
      DRV_WriteReg(GPIO_MODE4,mode);
      */
   }
#endif   

#ifdef MT6205B
   /*conf_data = 0~3*/
   void GPIO_ModeSetup(kal_uint16 pin, kal_uint16 conf_dada)
   {
   /*
      kal_uint16 modeno;
      kal_uint16 remainder;
      
      modeno = pin/8;
      remainder = pin % 8;
      
      DRV_WriteReg(GPIO_MODE_CLR(modeno),(0x0003 << (remainder*2)));
      DRV_WriteReg(GPIO_MODE_SET(modeno),(conf_dada << (remainder*2)));
      */
   }
   
   /*conf_data = 0~3*/
   void GPO_ModeSetup(kal_uint16 pin, kal_uint16 conf_dada)

   {
      /*
      DRV_WriteReg(GPIO_MODE4_CLR,(0x0003 << (pin*2)));
      DRV_WriteReg(GPIO_MODE4_SET,(conf_dada << (pin*2)));
      */
   }
#endif    

#if ( (defined(MT6218)) || (defined(MT6218B))|| (defined(MT6219))|| (defined(MT6217))|| (defined(MT6228))|| defined(MT6229))|| (defined(MT6226))|| (defined(MT6227)) 
   /*conf_data = 0~3*/
   void GPIO_ModeSetup(kal_uint16 pin, kal_uint16 conf_dada)
   {
      kal_uint16 modeno;
      kal_uint16 remainder;
      
      modeno = pin/8;
      remainder = pin % 8;
      #if defined(MT6226)|| defined(MT6227)
      if(pin>=56)/*special case  */
      {
         DRV_WriteReg(GPIO_MODE8_CLR,(0x0003 << (remainder*2)));
         DRV_WriteReg(GPIO_MODE8_SET,(conf_dada << (remainder*2)));
         return;
      }
      #endif                  
      
      DRV_WriteReg(GPIO_MODE_CLR(modeno),(0x0003 << (remainder*2)));
      DRV_WriteReg(GPIO_MODE_SET(modeno),(conf_dada << (remainder*2)));
   }
   
   /*conf_data = 0~3*/
   void GPO_ModeSetup(kal_uint16 pin, kal_uint16 conf_dada)
   {
      DRV_WriteReg(GPO_MODE_CLR,(0x0003 << (pin*2)));
      DRV_WriteReg(GPO_MODE_SET,(conf_dada << (pin*2)));
   }
/*
* FUNCTION                                                            
*	GPIO_PullenSetup
*
* DESCRIPTION                                                           
*   	This function is to enable or disable the pull up/down of the related GPIO pin.
*		You can not decide to pull up or down, it is set inside the chip.
*		And, the setting is different from pin by pin.
*
* PARAMETERS
 
*	enable: enable the pull up/down
*	
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*/
   void GPIO_PullenSetup(kal_uint16 pin, kal_bool enable)
   {
      kal_uint16 modeno;
      kal_uint16 remainder;
      
      modeno = pin/16;
      remainder = pin % 16;

		if(enable)
      	DRV_WriteReg(GPIO_PULLEN_SET(modeno), 1 << remainder);
      else
      	DRV_WriteReg(GPIO_PULLEN_CLR(modeno), 1 << remainder);
   }
/*
* FUNCTION                                                            
*	GPIO_DinvSetup
*
* DESCRIPTION                                                           
*   	This function is to enable data invert of the related GPIO pin
*
* PARAMETERS
 
*	enable: enable the data inversion
*	
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*/
   void GPIO_DinvSetup(kal_uint16 pin, kal_bool enable)
   {
      kal_uint16 modeno;
      kal_uint16 remainder;
      
      modeno = pin/16;
      remainder = pin % 16;

		if(enable)
      	DRV_Reg(GPIO_DINV_ADRS(modeno)) |= (1 << remainder);
      else
      	DRV_Reg(GPIO_DINV_ADRS(modeno)) &= ~(1 << remainder);
   }
   
#endif    
/*
* FUNCTION                                                            
*	GPIO_InitIO
*
* DESCRIPTION                                                           
*   	This function is to initialize one GPIO pin as INPUT or OUTPUT
*
* CALLS  
*	Initialize one GPIO pin as INPUT or OUTPUT
*
* PARAMETERS
*	direction: INPUT or OUTPUT
*	port: pin number
*	
* RETURNS
*	None
*
* GLOBALS AFFECTED
*   external_global
*/
void GPIO_InitIO(char direction, char port)
{
#ifdef MT6205
   if (direction == OUTPUT)
	{
	   if (port < 16)
		   DRV_Reg(GPIO_DIR) |= (1 << port);
      else
         DRV_Reg(GPIO_DIR2) |= (1 << (port-16));
	}
	else
	{
	   if (port < 16)
		   DRV_Reg(GPIO_DIR) &= ~(1 << port);
		else
		   DRV_Reg(GPIO_DIR2) &= ~(1 << (port-16));
	}
#endif /*(MT6205)*/

#ifdef MT6205B
extern UINT32 SRVAPI DRV_GpioSetDirection(UINT8, UINT8);
DRV_GpioSetDirection(port, direction);
/*
   if (direction == OUTPUT)
	{
	   if (port < 16)
	      DRV_WriteReg(GPIO_DIR_SET,(1 << port));
      else
         DRV_WriteReg(GPIO_DIR2_SET,(1 << (port-16)));
	}
	else
	{
	   if (port < 16)
		   DRV_WriteReg(GPIO_DIR_CLR,(1 << port));
		else
		   DRV_WriteReg(GPIO_DIR2_CLR,(1 << (port-16)));
	}
	*/
#endif    

#if ( (defined(MT6218)) || (defined(MT6218B))|| (defined(MT6219))|| (defined(MT6217))|| (defined(MT6228))|| defined(MT6229))|| (defined(MT6226))|| (defined(MT6227)) 
   kal_uint16 no;
   kal_uint16 remainder;
      
   no = port/16;
   remainder = port % 16;
   if (direction == OUTPUT)
      DRV_WriteReg(GPIO_DIR_SET(no),(1 << remainder));
	else
      DRV_WriteReg(GPIO_DIR_CLR(no),(1 << remainder));
#endif    

#if ( (defined(MT6208)))
	if (direction == OUTPUT)
	{
		DRV_Reg(GPIO_DIR) |= (1 << port);
	}
	else
	{
		DRV_Reg(GPIO_DIR) &= ~(1 << port);
	}
#endif    
}

/*
* FUNCTION                                                            
*	GPIO_ReadIO
*
* DESCRIPTION                                                           
*   	This function is to read data from one GPIO pin
*
* CALLS  
*	Read data from one GPIO pin
*
* PARAMETERS
*	port: pin number
*	
* RETURNS
*	1 or 0
*
* GLOBALS AFFECTED
*   external_global
*/
char GPIO_ReadIO(char port)
{
#if ( (defined(MT6205)) || (defined(MT6205B)) )
 
kal_uint8* pLevel;
kal_uint8 Level;
pLevel=&Level;
extern UINT32 DRV_GpioGetLevel(UINT8,UINT8*);
DRV_GpioGetLevel( port,pLevel);
return Level;
 
/*
   if (port < 16)
	   return((DRV_Reg(GPIO_DIN) >> port) & 1);
	else
	   return((DRV_Reg(GPIO_DIN2) >> (port-16)) & 1);
	   */
#endif  
#if ( (defined(MT6208)) )
   return((DRV_Reg(GPIO_DIN) >> port) & 1);
#endif    
#if ( (defined(MT6218)) || (defined(MT6218B))|| (defined(MT6219))|| (defined(MT6217)))
   if (port < 16)
	   return((DRV_Reg(GPIO_DIN1) >> port) & 1);
	else if (port < 32)
	   return((DRV_Reg(GPIO_DIN2) >> (port-16)) & 1);
	else if (port < 48)
	   return((DRV_Reg(GPIO_DIN3) >> (port-32)) & 1);
	
#endif
#if ((defined(MT6228))|| defined(MT6229))|| (defined(MT6226))|| (defined(MT6227))    	   	
   kal_uint16 no;
   kal_uint16 remainder;
   kal_uint16 data;   
   no = port/16;
   remainder = port % 16;
         		
   data=DRV_Reg(GPIO_DIN(no));   
   data=data&(1 << remainder);
   data=(data >> remainder);
   return (kal_uint8)data;
   
#endif    
}

/*
* FUNCTION                                                            
*	GPIO_WriteIO
*
* DESCRIPTION                                                           
*   	This function is to write data to one GPIO port
*
* CALLS  
*	Write data to one GPIO port
*
* PARAMETERS
*	data: KAL_TRUE(1) or KAL_FALSE(0)
*	port: gpio pin number
*	
* RETURNS
*	None
*
* GLOBALS AFFECTED
*   external_global        
*/
void GPIO_WriteIO(char data, char port)
{
#ifdef MT6205
	if (data == 1)
	{
	   if (port < 16)
		   DRV_Reg(GPIO_DOUT) |= (1 << port);
		else
		   DRV_Reg(GPIO_DOUT2) |= (1 << (port-16));
	}
	else
	{
	   if (port < 16)
		   DRV_Reg(GPIO_DOUT) &= ~(1 << port);
		else
		   DRV_Reg(GPIO_DOUT2) &= ~(1 << (port-16));
	}
	
#endif /*(MT6205)*/

#ifdef MT6205B
extern UINT32 DRV_GpioSetLevel(UINT8,UINT8);
DRV_GpioSetLevel(port,data);
/*
	if (data == 1)
	{
	   if (port < 16)
	      DRV_WriteReg(GPIO_DOUT_SET,(1 << port));
		else
		   DRV_WriteReg(GPIO_DOUT2_SET,(1 << (port-16)));
	}
	else
	{
	   if (port < 16)
	      DRV_WriteReg(GPIO_DOUT_CLR,(1 << port));
		else
		   DRV_WriteReg(GPIO_DOUT2_CLR,(1 << (port-16)));
	}
	*/
#endif    

#if ( (defined(MT6208)))
	if (data == 1)
	{
		DRV_Reg(GPIO_DOUT) |= (1 << port);
	}
	else
	{
		DRV_Reg(GPIO_DOUT) &= ~(1 << port);
	}
#endif    

#if ( (defined(MT6218)) || (defined(MT6218B))|| (defined(MT6219))|| (defined(MT6217))|| (defined(MT6228))|| defined(MT6229)|| (defined(MT6226))|| (defined(MT6227)) )
   kal_uint16 no;
   kal_uint16 remainder;
      
   no = port/16;
   remainder = port % 16;
   if (data == 1)
      DRV_WriteReg(GPIO_DOUT_SET(no),(1 << remainder));
	else
      DRV_WriteReg(GPIO_DOUT_CLR(no),(1 << remainder));
#endif    
}

/*
* FUNCTION                                                            
*	GPI_O_ReadIO
*
* DESCRIPTION                                                           
*   	This function is to read data from one GPI pin
*
* CALLS  
*	Read data from one GPI pin
*
* PARAMETERS
*	None
*	
* RETURNS
*	1 or 0
*
* GLOBALS AFFECTED
*   external_global
*/
#if ( (defined(MT6208)) )
char GPI_ReadIO(void)
{
	return((DRV_Reg(GPI_O_DIO) >> 15) & 1);
}
#endif    
/*
* FUNCTION                                                            
*	GPI_O_WriteIO
*
* DESCRIPTION                                                           
*   	This function is to write data to one GPO pin
*
* CALLS  
*	Write data to one GPO pin
*
* PARAMETERS
*	data: KAL_TRUE(1) or KAL_FALSE(0)
*	port: gpo pin number
*	
* RETURNS
*	None
*
* GLOBALS AFFECTED
*   external_global
*/

void GPO_WriteIO(char data,char port)
{
#ifdef MT6205

	if (data == 1)
	{
		DRV_Reg(GPO_DOUT) |= (1 << port);
	}
	else
	{
		DRV_Reg(GPO_DOUT) &= ~(1 << port);
	}
	
#endif   /*(MT6205)*/

#ifdef MT6205B
/*
	if (data == 1)
	{
	   DRV_WriteReg(GPO_DOUT_SET,(1 << port));
	}
	else
	{
	   DRV_WriteReg(GPO_DOUT_CLR,(1 << port));
	}
	*/
#endif    

#if( (defined(MT6218)) || (defined(MT6218B))|| (defined(MT6219))|| (defined(MT6217))|| (defined(MT6228))|| defined(MT6229)|| (defined(MT6226))|| (defined(MT6227)) )
	if (data == 1)
	{
	   DRV_WriteReg(GPO_DOUT_SET,(1 << port));
	}
	else
	{
	   DRV_WriteReg(GPO_DOUT_CLR,(1 << port));
	}
#endif    

#if ( (defined(MT6208)) )
	if (data == 1)
	{
		DRV_Reg(GPI_O_DIO) |= (1 << port);
	}
	else
	{
		DRV_Reg(GPI_O_DIO) &= ~(1 << port);
	}
#endif    
}

/*
* FUNCTION                                                            
*	GPI_O_WriteAll
*
* DESCRIPTION                                                           
*   	This function is to write data to all GPO pins
*
* CALLS  
*	Write data to all GPO pins
*
* PARAMETERS
*	write_data: expected GPO data to all pins
*	
* RETURNS
*	None
*
* GLOBALS AFFECTED
*   external_global
*/
void GPO_WriteAll(kal_uint16 write_data)
{
#if ( (defined(MT6205)) || (defined(MT6205B)) )
  
#endif    
#if ( (defined(MT6218)) || (defined(MT6218B))|| (defined(MT6219))|| (defined(MT6217))|| (defined(MT6228))|| defined(MT6229)|| (defined(MT6226))|| (defined(MT6227)) )
   DRV_WriteReg(GPO_DOUT,write_data);
#endif    
#if ( (defined(MT6208)))
   DRV_WriteReg(GPI_O_DIO,write_data);
#endif    
}

/*
* FUNCTION                                                            
*	GPIO_ReturnDout
*
* DESCRIPTION                                                           
*   	This function is to report GPIO output value
*
* CALLS  
*	Report GPIO output value accoding to input pin
*
* PARAMETERS
*	GPIO pin 
*	
* RETURNS
*	GPIO output value
*
* GLOBALS AFFECTED
*   external_global
*/
char GPIO_ReturnDout(char port)
{
   kal_uint16 no;
   kal_uint16 remainder;
   kal_uint16 data = 0;   
   no = port/16;
   remainder = port % 16;

#if ( (defined(MT6205)) || (defined(MT6205B)) )
/*
   if(0==no)
      data=DRV_Reg(GPIO_DOUT); 
   else if(1==no)
      data=DRV_Reg(GPIO_DOUT2); 
      */
#endif

#if ( (defined(MT6208)))
   data=DRV_Reg(GPIO_DOUT); 	
#endif

#if ( (defined(MT6218)) || (defined(MT6218B)) || (defined(MT6219))|| (defined(MT6217))|| (defined(MT6228))|| defined(MT6229)|| (defined(MT6226))|| (defined(MT6227)) )
   data=DRV_Reg(GPIO_DOUT(no)); 
#endif   
   
   data=data&(1 << remainder);
   data=(data >> remainder);
   return (kal_uint8)data;
}
/*
* FUNCTION                                                            
*	GPO_ReturnDout
*
* DESCRIPTION                                                           
*   	This function is to report GPO output value
*
* CALLS  
*	Report GPO output value accoding to input pin
*
* PARAMETERS
*	GPO pin 
*	
* RETURNS
*	GPO output value
*
* GLOBALS AFFECTED
*   external_global
*/
char GPO_ReturnDout(char port)
{
   kal_uint16 no;
   kal_uint16 remainder;
   kal_uint16 data = 0;   
   no = port/16;
   remainder = port % 16;


#if ( (defined(MT6208)))
   data=DRV_Reg(GPI_O_DIO); 	
#endif

#if ( (defined(MT6205)) || (defined(MT6205B))||(defined(MT6218)) || (defined(MT6218B)) || (defined(MT6219))|| (defined(MT6217))|| (defined(MT6228))|| defined(MT6229)|| (defined(MT6226))|| (defined(MT6227)) )
   
#endif   
   
   data=data&(1 << remainder);
   data=(data >> remainder);
   return (kal_uint8)data;
}
/*
* FUNCTION                                                            
*	GPIO_ReturnDir
*
* DESCRIPTION                                                           
*   	This function is to report GPIO direction value
*
* CALLS  
*	Report GPIO direction value accoding to input pin
*
* PARAMETERS
*	GPIO pin 
*	
* RETURNS
*	GPIO direction value
*
* GLOBALS AFFECTED
*   external_global
*/
char GPIO_ReturnDir(char port)
{
	
   kal_uint16 no;
   kal_uint16 remainder;
   kal_uint16 data;   
   no = port/16;
   remainder = port % 16;
#if ( (defined(MT6205)) || (defined(MT6205B)) )
 
 kal_uint8* pDirection;
kal_uint8 Direction;
pDirection=&Direction;
extern UINT32 DRV_GpioGetDirection(UINT8,UINT8*);
DRV_GpioGetDirection(port, pDirection);
return Direction;
/*   
   if(0==no)
      data=DRV_Reg(GPIO_DIR); 
   else if(1==no)
      data=DRV_Reg(GPIO_DIR2); 
      */
#endif
#if ( (defined(MT6208)))
   data=DRV_Reg(GPIO_DIR); 	
#endif
#if ( (defined(MT6218)) || (defined(MT6218B)) || (defined(MT6219))|| (defined(MT6217))|| (defined(MT6228))|| defined(MT6229)|| (defined(MT6226))|| (defined(MT6227)) )
   data=DRV_Reg(GPIO_DIR(no)); 
#endif   
   
   data=data&(1 << remainder);
   data=(data >> remainder);
   return (kal_uint8)data;

}
/*
* FUNCTION                                                            
*	GPIO_ReturnMode
*
* DESCRIPTION                                                           
*   	This function is to report GPIO mode value
*
* CALLS  
*	Report GPIO mode value accoding to input pin
*
* PARAMETERS
*	GPIO pin 
*	
* RETURNS
*	GPIO mode value
*
* GLOBALS AFFECTED
*   external_global
*/
char GPIO_ReturnMode(char port)
{

   kal_uint16 no;
   kal_uint16 remainder;
   kal_uint32 data = 0;   
   no = port/8;
   remainder = port % 8;
   
 
   
   data=data&(0x3 << (2*remainder));
   data=(data >> (2*remainder));
   return (kal_uint8)data;

}

/*
* FUNCTION                                                            
*	GPO_ReturnMode
*
* DESCRIPTION                                                           
*   	This function is to report GPO mode value
*
* CALLS  
*	Report GPO mode value accoding to input pin
*
* PARAMETERS
*	GPO pin 
*	
* RETURNS
*	GPO mode value
*
* GLOBALS AFFECTED
*   external_global
*/
char GPO_ReturnMode(char port)
{
   kal_uint16 no;
   kal_uint16 remainder;
   kal_uint32 data = 0;   
   no = port/8;
   remainder = port % 8;

#if ( (defined(MT6208)))
    data=DRV_Reg(GPIO_MODE2); 
#endif      
#if ( (defined(MT6205)) || (defined(MT6205B)) )
   
#endif   
#if ( (defined(MT6218)) || (defined(MT6218B)) || (defined(MT6219))|| (defined(MT6217))|| (defined(MT6228))|| defined(MT6229)|| (defined(MT6226))|| (defined(MT6227)) )
   data=DRV_Reg(GPO_MODE); 
#endif   
   data=data&(0x3 << (2*remainder));
   data=(data >> (2*remainder));
   return (kal_uint8)data;
   
}   






#ifdef IC_MODULE_TEST
   #define IC_MODULE_Ready_BOT   13
   #define IC_MODULE_Ready_EOT   14
   #define IC_MODULE_Ready_BIN   15
   kal_uint8 IC_MODULE_GPT_Handle;
   void IC_MODULE_Calback(void *parameter)
   {
   	GPIO_WriteIO(1,IC_MODULE_Ready_EOT);
   }

   void IC_ModuleTest_Start(void)
   {
      #ifdef MT6205B
         GPIO_WriteIO(0,IC_MODULE_Ready_BIN);
         GPIO_WriteIO(1,IC_MODULE_Ready_EOT);
         GPIO_WriteIO(1,IC_MODULE_Ready_BOT);
         GPTI_GetHandle(&IC_MODULE_GPT_Handle);
      #endif
   }
   
   void IC_ModuleTest_End(kal_bool status)
   {
      #ifdef MT6205B
         GPIO_WriteIO(1,IC_MODULE_Ready_BOT);
         if (status)
            GPIO_WriteIO(1,IC_MODULE_Ready_BIN);
         else
            GPIO_WriteIO(0,IC_MODULE_Ready_BIN);
         
         GPIO_WriteIO(0,IC_MODULE_Ready_EOT);
         GPTI_StartItem(IC_MODULE_GPT_Handle,
                        3,
                        IC_MODULE_Calback,
                        NULL);
      #endif
   }
#endif   /*IC_MODULE_TEST*/

