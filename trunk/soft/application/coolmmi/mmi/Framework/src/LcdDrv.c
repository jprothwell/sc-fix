#if 0 
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
 *
 * Filename:
 * ---------
 *	LcdDrv.c
 *
 * Project:
 * --------
 *   MediaTeck GSM
 *
 * Description:
 * ------------
 *   This Module defines the LCD driver routines.
 *
 * Author:
 * -------
 * -------
 * 				
 *
 * HISTORY
 * -------
 * Created On 05 Dec
 *------------------------------------------------------------------------------
	
 
*******************************************************************************/
#ifdef MMI_ON_HARDWARE_P

//------- Include Files -----------------------------
//#include "stdafx.h"						// for windows compilation
#include "lcddr.h"					    	// hardware address, command words, software macro's and routines

// gShadowBuffer is defined in GUI
extern uint16 gShadowBuffer[LCDrHEIGHT][LCDrWIDTH];

// gShadowBuffer for refrence purpose only
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif


/* ----------------------------------------------------------------------------
 Function 	: 	void LCDrINIT(void)
 Description:	This function Initilizes LCD parameters.
 Input 		:	NULL
 Output		:	None
-------------------------------------------------------------------------------*/

void LCDrINIT(void)		
{
	*(volatile uint16 *)0x8001000c = 0x6106; // This is needed by PLUTO Engineers to resolve timing issues
											 // If we use Preprosser -DUT_ON_TARGET in build enviornment, we have
											 // execute above instruction. 
											 // IMPORTANT : For other cases this instruction is not needed.	  	
	LCDrDISCONTROL();					// display settings
	LCDrSETCOMSCAN();					// scanning flow
	LCDrCTRLOSCON();					// oscillator ON
	LCDrGENSLPOUT();					// Sleep Out
	LCDrVOLTLEVEL(0x28);				// Voltage Set	2a to 16
	LCDrTEMPERATUREGRAD();				// Set Temperature Gradiants 
	LCDrPOWERCNTL(0x0f);				// ON all refrence voltages to LCD
	LCDrSetDATANORCTL();				// Column direction, RGB pattern
	LCDrINVERSE()						// ? Inverse LCD RAM Contents before displaying
	LCDrblockClear(0,0,LCDrWIDTH-1,LCDrHEIGHT-1);	// Clear LCD RAM fill 000H
	LCDrDISPON();						// LCD Display ON
}


/* ----------------------------------------------------------------------------
 Function 	: 	void LCDrDISCONTROL(void)
 Description:	This function Initilizes LCD Display parameters.
 Input 		:	NULL
 Output		:	None
-------------------------------------------------------------------------------*/
void LCDrDISCONTROL(void)
{
   	LCDrCtrlWrite(LCDrDISCTL);
   
  	LCDrDataWrite(0xF);					// ONLY FLICKER NO MOTION // Clock Dividing 2 div, 8 drive pattern 
//	LCDrDataWrite(0xE); 				// ONLY FLICKER NO MOTION
//	LCDrDataWrite(0xA); 				// VERY BAD
//	LCDrDataWrite(0xD); 				// SAME AS E F
//	LCDrDataWrite(0x3); 				// fllicker and motion
//	LCDrDataWrite(0x0); 				// fllicker and motion
 
  	LCDrDataWrite(LCDrDISCTL_160DUTY);	// Duty / module on block basis 160/4-1 = 39  
   	LCDrDataWrite(LCDrDISCTL_11H);		// 10 lines to be inversely highlited	
//	LCDrDataWrite(0xf); 				// flickering increases
	
}


/* ----------------------------------------------------------------------------
 Function 	: 	void LCDrSETCOMSCAN(void)
 Description:	This function Initilizes LCD Display Scan parameter.
 Input 		:	NULL
 Output		:	None
-------------------------------------------------------------------------------*/
void LCDrSETCOMSCAN(void)
{
   LCDrCtrlWrite(LCDrCOMSCN);
   LCDrDataWrite(0x2);					// scan direction 1 -> 80, 81 -> 160	

}

/* ----------------------------------------------------------------------------
 Function 	: 	void LCDrVOLTLEVEL(uint8 voltage)
 Description:	This function Initilizes LCD voltage parameters.
 Input 		:	NULL
 Output		:	None
-------------------------------------------------------------------------------*/
void LCDrVOLTLEVEL(uint8 voltage)
{
   	LCDrCtrlWrite(LCDrVOLCTR);			// 0x2a-0x16
	LCDrDataWrite(voltage); 			// set voltage
  	LCDrDataWrite(0x02);				// resistance ratio
//	LCDrDataWrite(0x30);
//	LCDrDataWrite(0x7);
}

/* ----------------------------------------------------------------------------
 Function 	: 	void LCDrTEMPERATUREGRAD(void)
 Description:	This function Initilizes LCD temperature parameters.
 Input 		:	NULL
 Output		:	None
-------------------------------------------------------------------------------*/
void LCDrTEMPERATUREGRAD(void)
{
	LCDrCtrlWrite(LCDrTMPGRD);			// temperature Gradient
	LCDrDataWrite(0x00);
	LCDrDataWrite(0x32);
	LCDrDataWrite(0x10);
	LCDrDataWrite(0xfe);
	LCDrDataWrite(0xdc);
}

/* ----------------------------------------------------------------------------
 Function 	: 	void LCDrPOWERCNTL(uint8 data)
 Description:	This function controls LCD power parameters.
 Input 		:	0xF -> All power ON
 				0x0 -> All refrence voltage OFF
 				Other depending upon requirement 
 Output		:	None
-------------------------------------------------------------------------------*/
void LCDrPOWERCNTL(uint8 data)
{
	LCDrCtrlWrite(LCDrPWRCTR);			// Power Control 	
	LCDrDataWrite(data);				// All Ckts ON -> F, OFF -> 00
}


/* ----------------------------------------------------------------------------
 Function 	: 	void LCDrSetDATANORCTL(void)
 Description:	This function Initilizes LCD Display Data parameters.
 Input 		:	NULL
 Output		:	None
-------------------------------------------------------------------------------*/
void LCDrSetDATANORCTL(void)
{
   LCDrCtrlWrite(LCDrDATCTL);

   LCDrDataWrite(0x2);  				// Column direction, Normal dis  
   LCDrDataWrite(0x1);					// RGB Arrangement
   LCDrDataWrite(0x4);  				// 16bit mode for RGB type B
}


/* ----------------------------------------------------------------------------
 Function 	: 	void LCDrClearAll(void)
 Description:	This function clears LCD RAM buffer to 00H.
 Input 		:	NULL
 Output		:	None
-------------------------------------------------------------------------------*/
void LCDrClearAll(void)	
{
	LCDrblockClear(0,0,LCDrWIDTH-1,LCDrHEIGHT-1);
}

/* ----------------------------------------------------------------------------
 Function 	: 	void LCDrblockClear(uint16 x1,uint16 y1,uint16 x2,uint16 y2)
 Description:	This function writes 0x00H in LCD RAM buffer from x1, y1 to x2, y2 location.
 Input 		:	x1, x2 (column addres)
 				y1, y2 (page address)
 Output		:	None
-------------------------------------------------------------------------------*/
void LCDrblockClear(uint16 x1,uint16 y1,uint16 x2,uint16 y2)
{
   uint16 LCDrx;
   uint16 LCDry;
   LCDrSetPageAddr(y1, y2);
   LCDrSetColAddr(x1, x2);
   LCDrWrStart();
   for(LCDry=y1;LCDry<=y2;LCDry++)  	// Y Coordinate Page Addresses
      for(LCDrx=x1;LCDrx<=x2;LCDrx++)  	// X Coordinate Column Addresses
      {
     	LCDrRAMWrite(0x0F00); 
      }
 }

/* ----------------------------------------------------------------------------
 Function 	: 	void LCDrDrawRectangle(uint16 x1,uint16 y1,uint16 x2,uint16 y2, uint16 data)
 Description:	This function draw rectangle on the LCD from location x1, y1 to x2, y2 with data.
 Input 		:	x1, x2
 				y1, y2
 				data	
 Output		:	None
-------------------------------------------------------------------------------*/
void LCDrDrawRectangle(uint16 x1,uint16 y1,uint16 x2,uint16 y2, uint16 data)
{
  
   uint16 LCDrx;
   uint16 LCDry;
   LCDrSetPageAddr(y1, y2); 			// Page Address
   LCDrSetColAddr(x1, x2);				// Column Address
   LCDrWrStart();						// Write Command
   for(LCDry=y1;LCDry<=y2;LCDry++)  	// Filling the LCD RAM 
     for(LCDrx=x1;LCDrx<=x2;LCDrx++)
     {
     		
         LCDrRAMWrite(data); 
      }
}   

/* ----------------------------------------------------------------------------
 Function 	: 	void LCDrBlockWrite(uint16 x1,uint16 y1,uint16 x2,uint16 y2)
 Description:	This function writes in LCD RAM from location x1, y1 to x2, y2 of shadow buffer.
 Input 		:	x1, x2
 				y1, y2
 Output		:	None
-------------------------------------------------------------------------------*/
void LCDrBlockWrite(uint16 x1,uint16 y1,uint16 x2,uint16 y2)
{
  #if 0   
   uint16 LCDrx;
   uint16 LCDry;
   LCDrSetPageAddr(y1, y2); 			// Page Address
   LCDrSetColAddr(x1, x2);				// Column Address
   LCDrWrStart();						// Write Command
   for(LCDry=y1;LCDry<=y2;LCDry++)  	// Filling the LCD RAM 
     for(LCDrx=x1;LCDrx<=x2;LCDrx++)
     {
         LCDrRAMWrite(gShadowBuffer[LCDry][LCDrx]);
//         LCDrDelay();
     }
   #endif
}   


/* ----------------------------------------------------------------------------
 Function 	: 	void LCDrblockRead(uint16 x1,uint16 y1,uint16 x2,uint16 y2)
 Description:	This function reads data from LCD RAM from location x1, y1 to x2, y2 in shadow buffer.
 Input 		:	x1, x2
 				y1, y2
 Output		:	None
-------------------------------------------------------------------------------*/
void LCDrblockRead(uint16 x1,uint16 y1,uint16 x2,uint16 y2)
{
   uint16 LCDrx;
   uint16 LCDry;
   LCDrSetPageAddr(y1, y2);
   LCDrSetColAddr(x1, x2);
   LCDrRdStart();
   for(LCDry=y1;LCDry<=y2;LCDry++) 
      for(LCDrx=x1;LCDrx<=x2;LCDrx++) 
      {
    //     gShadowBuffer[LCDrx][LCDry]=LCDrbg;
         LCDrRAMRead(gShadowBuffer[LCDry][LCDrx]);
      }
}


/* ----------------------------------------------------------------------------
 Function 	: 	void LCDrSetPageAddr(uint8 startPage, uint8 endPage)
 Description:	This function sets start and end page address for LCD RAM read or write.
 Input 		:	start
 				stop page address
 Output		:	None
-------------------------------------------------------------------------------*/
void LCDrSetPageAddr(uint8 startPage, uint8 endPage)
{
   LCDrCtrlWrite(LCDrPASET);
   LCDrDataWrite(startPage);
   LCDrDataWrite(endPage);
}


/* ----------------------------------------------------------------------------
 Function 	: 	void LCDrSetColAddr(uint8 startColumn, uint8 endColumn)
 Description:	This function sets start and end column address for LCD RAM read or write.
 Input 		:	start
 				stop column address
 Output		:	None
-------------------------------------------------------------------------------*/
void LCDrSetColAddr(uint8 startColumn, uint8 endColumn)
{
   LCDrCtrlWrite(LCDrCASET);
   LCDrDataWrite(startColumn);
   LCDrDataWrite(endColumn);
}


/* ----------------------------------------------------------------------------
 Function 	: 	void LCDrDISPartial(uint8 startBlock, uint8 endBlock)
 Description:	This function display the partial LCD RAM content specified by start and end block address.
 Input 		:	start and end block address
 Output		:	None
-------------------------------------------------------------------------------*/
void LCDrDISPartial(uint8 startBlock, uint8 endBlock)
{
   LCDrCtrlWrite(LCDrPTLIN);
   LCDrDataWrite(startBlock);
   LCDrDataWrite(endBlock);
}


/* ----------------------------------------------------------------------------
 Function 	: 	void LCDrDISPartialExit(void)
 Description:	This function is called to exit from the partial mode.
 Input 		:	NULL
 Output		:	None
-------------------------------------------------------------------------------*/
void LCDrDISPartialExit(void)
{
   LCDrCtrlWrite(LCDrPTLOUT);
}


/* ----------------------------------------------------------------------------
 Function 	: 	void LCDrConfAREAScroll(uint8 topBlock, uint8 bottomBlock, uint8 noOfBlocks, uint8 areaMode)
 Description:	This function is called for scroll option.
 Input 		:	top block		-> 	upper fixed area	 
 				bottom block	->	lower fixed area
 				no of blocks	-> 	number of blocks to be moved
 				areaMode		->	mode of moving LCDrCENTERSCROLL, LCDrTOPSCROLL 
                              		LCDrBOTTOMSCROLL, LCDrALLSCROLL
 Output		:	None
---------------------------------------------------------------------------------*/
void LCDrConfAREAScroll(uint8 topBlock, uint8 bottomBlock, uint8 noOfBlocks, uint8 areaMode)
{
   LCDrCtrlWrite(LCDrASCSET);
   LCDrDataWrite(topBlock);
   LCDrDataWrite(bottomBlock);
   LCDrDataWrite(noOfBlocks);
   LCDrDataWrite(areaMode);  
}


/* ----------------------------------------------------------------------------
 Function 	: 	void LCDrScrollStart(uint8 startBlock)
 Description:	This function is used to start scrolling.
 Input 		:	start block address
 Output		:	None
-------------------------------------------------------------------------------*/
void LCDrScrollStart(uint8 startBlock)
{
   LCDrCtrlWrite(LCDrSCSTART);
   LCDrDataWrite(startBlock);
}


/* ----------------------------------------------------------------------------
 Function 	: 	void LCDrSize(uint16 *outLCDWidth,uint16 *outLCDHeight)
 Description:	This function return LCD height and width.
 Input 		:	NULL
 Output		:	None
-------------------------------------------------------------------------------*/
void LCDrSize(uint16 *outLCDWidth,uint16 *outLCDHeight)
{
	*outLCDWidth = LCDrWIDTH;
	*outLCDHeight = LCDrHEIGHT;
}


/* ----------------------------------------------------------------------------
 Function 	: 	void LCDrTest(void)
 Description:	This function test different modes of LCD.
 Input 		:	NULL
 Output		:	None
-------------------------------------------------------------------------------*/
void LCDrTest(void)	
{

	LCDrBlockWrite(0,0,LCDrWIDTH-1,LCDrHEIGHT-1);

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

//	LCDrCTRLOSCOFF();
			
//	LCDrDISPOFF();
//  LCDrPOWERCNTL(0x00);
//	LCDrGENSLPIN();
	
/*	
	LCDrCtrlWrite(LCDrASCSET);
	LCDrDataWrite(10);
	LCDrDataWrite(10);
	LCDrDataWrite(20);
	LCDrDataWrite(01);
	
	LCDrCtrlWrite(LCDrSCSTART);
	LCDrDataWrite(10);
*/	
/*
    LCDrDrawRectangle(0,5,LCDrWIDTH-1,25,   0x0F00);
 
    LCDrDrawRectangle(0,30,LCDrWIDTH-1,50,  0x00F0);

    LCDrDrawRectangle(0,55,LCDrWIDTH-1,75,  0x000F);

    LCDrDrawRectangle(0,80,LCDrWIDTH-1,100, 0x0FF0);

    LCDrDrawRectangle(0,105,LCDrWIDTH-1,125,0x00FF);

    LCDrDrawRectangle(0,130,LCDrWIDTH-1,150,0x0F0F);
    
    LCDrDrawRectangle(0,155,LCDrWIDTH-1,160,0x0FFF);
	
	
	L1_LCDrOFF();
	LCDrCtrlWrite(LCDrPWRCTR);	// Power Control 	
	LCDrDataWrite(0x0000);		// All Ckts ON

	LCDrCtrlWrite(LCDrPWRCTR);	// Power Control 	
	LCDrDataWrite(0x0f);		// All Ckts ON
	
	L1_LCDrON();
*/	
}



/* amit not required */
/* ----------------------------------------------------------------------------
 Function 	: 	LCDrDelay
 Description:	This function gives delay.
 Input 		:	NULL
 Output		:	None
-------------------------------------------------------------------------------*/
void LCDrDelay(void)
{
	uint16 delay;
	for (delay =0;delay <300;delay++)	{}
} 

/* ----------------------------------------------------------------------------
 Function 	: 	LCDrTestColours
 Description:	This function test colours on LCD.
 Input 		:	NULL
 Output		:	None
-------------------------------------------------------------------------------*/
void LCDrTestColours(void)
{
	LCDrDrawRectangle(0,5,LCDrWIDTH-1,25,   0x0F00);
	
	LCDrDrawRectangle(0,30,LCDrWIDTH-1,50,  0x00F0);
	
	LCDrDrawRectangle(0,55,LCDrWIDTH-1,75,  0x000F);
	
	LCDrDrawRectangle(0,80,LCDrWIDTH-1,100, 0x0FF0);
	
	LCDrDrawRectangle(0,105,LCDrWIDTH-1,125,0x00FF);
	
	LCDrDrawRectangle(0,130,LCDrWIDTH-1,150,0x0F0F);
	
	LCDrDrawRectangle(0,155,LCDrWIDTH-1,160,0x0FFF);
}

/* ----------------------------------------------------------------------------
 Function 	: 	LCDrTestWritep
 Description:	This function is for testing.
 Input 		:	x1, x2, y1, y2
 Output		:	None
-------------------------------------------------------------------------------*/
void LCDrTestWritep(uint16 x1,uint16 y1,uint16 x2,uint16 y2, uint16 shadow_bufferTest)
{
  
   uint16 LCDrx;
   uint16 LCDry;
   LCDrSetPageAddr(y1, y2); 			// Page Address
   LCDrSetColAddr(x1, x2);				// Column Address
   LCDrWrStart();						// Write Command
   for(LCDry=y1;LCDry<=y2;LCDry++)  	// Filling the LCD RAM 
     for(LCDrx=x1;LCDrx<=x2;LCDrx++)
     {
         LCDrRAMWrite(gShadowBuffer[LCDry][LCDrx]);
     }
}   
#endif
/* -----------------------------------------------------------------------------------------------------------------
									End Of File LcdDrv.c
--------------------------------------------------------------------------------------------------------------------*/
#endif

