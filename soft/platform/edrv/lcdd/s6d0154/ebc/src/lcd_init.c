#include "lcd_if_6230_series_hw.h"

#ifndef __ADS__
#include "hal_timers.h"
#endif

// Macros to access APB
#define GD_WR_CMD(cmd) REG_GD_LCD_SINGLE_ACCESS = GD_LCD_START_WRITE_BIT | cmd
#define GD_WR_DATA(data) REG_GD_LCD_SINGLE_ACCESS = GD_LCD_START_WRITE_BIT | GD_LCD_YTPE_BIT | data

#define GD_RD_DATA  REG_GD_LCD_SINGLE_ACCESS = GD_LCD_START_READ_BIT | GD_LCD_YTPE_BIT


// Write LCD register function
void gouda_WriteReg(UINT16 Addr, UINT16 Data)
{
while((REG_GD_STATUS & GD_STATUS_LCD_BUSY_BIT));
GD_WR_CMD(Addr);
while((REG_GD_STATUS & GD_STATUS_LCD_BUSY_BIT));
GD_WR_DATA(Data);
}

UINT16 gouda_ReadReg(UINT16 Addr)
{
while((REG_GD_STATUS & GD_STATUS_LCD_BUSY_BIT));
GD_WR_CMD(Addr);
while((REG_GD_STATUS & GD_STATUS_LCD_BUSY_BIT));
GD_RD_DATA;
while((REG_GD_STATUS & GD_STATUS_LCD_BUSY_BIT));
return REG_GD_LCD_SINGLE_ACCESS;
}

/************************************************************************/
// Function:	DelayXms
// Description:	Delay Xms.
// Inputs:		
// Outputs:		
// Return:		
// Note:		
/************************************************************************/
void DelayXms(UINT32 ms)
{
#ifdef __ADS__
	UINT32 Cnt;

	//SysStatus.CurFreq = 54;//For Test;
	
	while(ms--)
	{
		Cnt = 54*100;
		__asm
		{
		_delay_nops:
			NOP
			NOP
			NOP
			NOP
			NOP
			SUBS	Cnt,Cnt,1 
			BNE		_delay_nops
		}
	}
#else
       hal_TimDelay(ms MS_WAITING);
#endif
}


// LCD initialization
void lcd_init(void)
{
 /*
  int i;
    for(i=0;i<320*240;i++)
    {
	gouda_WriteReg(0x22, i);//解决画面会出现瞬间的跳动S6D0154X01必须加上。
	//gouda_ReadReg(0x80);
	//gouda_WriteReg(0x22, 0xffff);//解决画面会出现瞬间的跳动S6D0154X01必须加上。
	//gouda_ReadReg(0x80);
	}
	
	for(i=0;i<320*240;i++)
    {
	//gouda_WriteReg(0x22, 0x0000);//解决画面会出现瞬间的跳动S6D0154X01必须加上。
	//gouda_ReadReg(0x80);
	//gouda_WriteReg(0x22, 0xffff);//解决画面会出现瞬间的跳动S6D0154X01必须加上。
	gouda_ReadReg(0x22);
	}
	*/
    //gouda_ReadReg(0x00);
	gouda_WriteReg(0x00, 0x0000);
	gouda_WriteReg(0x00, 0x0000);
	gouda_WriteReg(0x00, 0x0000);
	gouda_WriteReg(0x00, 0x0000);
	gouda_WriteReg(0x00, 0x0000);

    // test
   // gouda_WriteReg(0x80, 0x0000);//解决画面会出现瞬间的跳动S6D0154X01必须加上。
	//gouda_ReadReg(0x80);
   // gouda_WriteReg(0x80, 0xffff);//解决画面会出现瞬间的跳动S6D0154X01必须加上。
	//gouda_ReadReg(0x80);
	//gouda_WriteReg(0x80, 0x0000);//解决画面会出现瞬间的跳动S6D0154X01必须加上。
	//gouda_ReadReg(0x80);


	gouda_WriteReg(0x80, 0x008D);//解决画面会出现瞬间的跳动S6D0154X01必须加上。
	//gouda_ReadReg(0x80);
	gouda_WriteReg(0x92, 0x0010);//解决画面会出现瞬间的跳动S6D0154X01必须加上。
	gouda_WriteReg(0x11, 0x001C);// Power Control 2 
	gouda_WriteReg(0x12, 0x4112);// Power Control 3  BT2-0, DC11-10, DC21-20, DC31-30 
	gouda_WriteReg(0x13, 0x0053);// Power Control 4  DCR_EX=0, DCR2-0, GVD6-0         
	gouda_WriteReg(0x14, 0x4269);// Power Control 5  VCOMG=0, VCM6-0, VCMR=0, VML6-0  
	gouda_WriteReg(0x10, 0x0800);// Power Control 1  current consumption  STB
	gouda_WriteReg(0x11, 0x011C);// Power Control 2
	DelayXms(10);
	gouda_WriteReg(0x11, 0x031C);// Power Control 2 
	DelayXms(10);
	gouda_WriteReg(0x11, 0x071C);// Power Control 2 
	DelayXms(10);
	gouda_WriteReg(0x11, 0x0F1C);// Power Control 2 
	DelayXms(20);
	gouda_WriteReg(0x11, 0x0F3C);// Power Control 2 
	DelayXms(30);
	gouda_WriteReg(0x01, 0x0128); // Driver Output Control                             
	gouda_WriteReg(0x02, 0x0100); // LCD-Driving-Waveform Control           
	gouda_WriteReg(0x03, 0x1030); // Entry Mode        
	//gouda_WriteReg(0x03, 0x1028); // Entry Mode        
	gouda_WriteReg(0x07, 0x1012); // Blank Period Control 1                                
	gouda_WriteReg(0x08, 0x0303); // Frame Cycle Control         
	gouda_WriteReg(0x0B, 0x1100); // External Display Interface Control                  
	gouda_WriteReg(0x0C, 0x0000); // General Input   
	gouda_WriteReg(0x0F, 0x1801); // Start Oscillation                       
	gouda_WriteReg(0x15, 0x0020); //VCI Recycling (R15H)
 
	gouda_WriteReg(0x50, 0x0101);// Gamma Control                        
	gouda_WriteReg(0x51, 0x0603);
	gouda_WriteReg(0x52, 0x0408);
	gouda_WriteReg(0x53, 0x0000);
	gouda_WriteReg(0x54, 0x0605);
	gouda_WriteReg(0x55, 0x0406);
	gouda_WriteReg(0x56, 0x0303);
	gouda_WriteReg(0x57, 0x0303);
	gouda_WriteReg(0x58, 0x0010);
	gouda_WriteReg(0x59, 0x1000);// Gamma Control  	                
	gouda_WriteReg(0x07, 0x0012);// Display Control
	DelayXms(40);
	gouda_WriteReg(0x07, 0x0013);// GRAM Address Set                                     
	gouda_WriteReg(0x07, 0x0017);// Display Control  DISPLAY ON                                   
	//gouda_WriteReg(0x20, 0x0000);// GRAM Address Set
	gouda_WriteReg(0x20, 0x0000);// GRAM Address Set
	gouda_WriteReg(0x21, 0x0000);// GRAM Address Set
	//LcdSetCmd(0x22);
	while((REG_GD_STATUS & GD_STATUS_LCD_BUSY_BIT));
	GD_WR_CMD(0x0022);
	
	//while(1){
	//   GD_WR_DATA(0x0000);
	//}
	
	

/*
	gouda_WriteReg(0x0015,0x7040);
	gouda_WriteReg(0x0011,0x0510);
	gouda_WriteReg(0x0010,0x3628);
	gouda_WriteReg(0x0012,0x0002); 
	gouda_WriteReg(0x0013,0x0e44);                                                       
	//delay_nops(100000);
	DelayXms(2);
	gouda_WriteReg(0x0012,0x0012);                               
	//delay_nops(100000);
	DelayXms(2);
	gouda_WriteReg(0x0010,0x5660);
	gouda_WriteReg(0x0013,0x2e44);
	//delay_nops(50000);
	DelayXms(1);

	gouda_WriteReg(0x0030,0x0000);
	gouda_WriteReg(0x0031,0x0502);
	gouda_WriteReg(0x0032,0x0307);
	gouda_WriteReg(0x0033,0x0307);
	gouda_WriteReg(0x0034,0x0004);
	gouda_WriteReg(0x0035,0x0401);
	gouda_WriteReg(0x0036,0x0707);
	gouda_WriteReg(0x0037,0x0703);
	gouda_WriteReg(0x0038,0x1e02);
	gouda_WriteReg(0x0039,0x1e02);
    
	gouda_WriteReg(0x0001,0x0000);
	gouda_WriteReg(0x0002,0x0300);

	gouda_WriteReg(0x0003,0x10B8);  //0x10B8
	gouda_WriteReg(0x0008,0x0204);
	gouda_WriteReg(0x000A,0x0008);
    
	gouda_WriteReg(0x0060,0x3100);
	gouda_WriteReg(0x0061,0x0001);
	gouda_WriteReg(0x0090,0x006d);
	gouda_WriteReg(0x0092,0x000F);
	gouda_WriteReg(0x0093,0x0001);
	gouda_WriteReg(0x009A,0x0011);
	gouda_WriteReg(0x00A3,0x0010);

#ifdef LCD_400x240
	gouda_WriteReg(0x0050,0x0000);
	gouda_WriteReg(0x0051,0x00EF);//239
	gouda_WriteReg(0x0052,0x0000);
	gouda_WriteReg(0x0053,0x018F);//399
#else
	gouda_WriteReg(0x0050,0x0000);
	gouda_WriteReg(0x0051,0x00EF);//239
	gouda_WriteReg(0x0052,0x0000);
	gouda_WriteReg(0x0053,0x013F);//319
#endif

//===Display_On_Function===     
	gouda_WriteReg(0x0007,0x0001);
	// delay_nops(1000);
	//delay_nops(40000);
	gouda_WriteReg(0x0007,0x0021);
	// delay_nops(40000);
	// delay_nops(1000);
	gouda_WriteReg(0x0007,0x0023);
	//delay_nops(40000);
	gouda_WriteReg(0x0007,0x0033);
	//delay_nops(40000);
	//  delay_nops(1000);
	gouda_WriteReg(0x0007,0x0133);
		
  	//gouda_WriteReg(0x0020, 0x00EF);
  	gouda_WriteReg(0x0020, 0x0000);
	gouda_WriteReg(0x0021, 0x0000);
	//  delay_nops(1000);
	//LcdSetCmd(0x0022);                  //Write Data to GRAM	 
	while((REG_GD_STATUS & GD_STATUS_LCD_BUSY_BIT));
	GD_WR_CMD(0x0022); 
	//delay_nops(1000);
	DelayXms(1);
	*/
	
	
}


