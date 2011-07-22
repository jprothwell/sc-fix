#include "lcd_if_6230_series_hw.h"
#include "lcd_init.h"

#include "hal_timers.h"
#include <uart_trc.h>

#define GOUDA_DISABLED

extern char gouda_memory[];
UINT16 gouda_ReadReg(UINT16 Addr);

void gouda_init()
{
#ifndef GOUDA_DISABLED

	SET_LCD_ROI_WINDOW_TL(0,0);
	SET_LCD_ROI_WINDOW_BR(239,319);
	SET_LCD_BG_COLOR(0,0,16);
	
	SET_LCD_VL_TL(0,0);
	SET_LCD_VL_BR(239,319);
	REG_GD_VL_RESC_RATIO = 0x00000100 | 0x01000000;
	REG_GD_VL_BLEND_OPT = GD_VL_BLEND_ALPHA_BIT ;//| GD_VL_BLEND_ROTATE_90_BIT;
	//not support roration
	
	//active video layer
	REG_GD_VL_INPUT_FMT = GD_VL_ACTIVE | GD_VL_INPUT_FMT_RGB565_BIT | (240 << 3);
	SET_LCD_VL_EXTENTS(240,320);	

    REG_GD_VL_Y_SRC = (UINT32)gouda_memory;
    SET_LCD_TIMING(1,1,10,10);
    REG_GD_LCD_CTRL = GD_LCD_OUT_FMT_16BIT_565_BIT;
    
    LCD_RESET_CLR;

#ifdef __ADS__
    __asm{
    	NOP
    	NOP
    	NOP
    	NOP
    	NOP
    	NOP
    }
#else
    asm volatile("nop\n nop\n nop\n nop\n nop\n nop");
#endif

    LCD_RESET_SET;
    
    lcd_init();

#if 0
    UINT16 *mem = (UINT16 *)gouda_memory;
    int i;
    for(i=0; i<240*320; i++) {
        mem[i] = 0x1f;
    }
    gouda_display();
    hal_TimDelay(1638);
    UINT16 tmp;
    char buf[20];
    int len;
    for(i=0; i<240*320; i++)
    {
        tmp = gouda_ReadReg(0x22);
        len = sprintf(buf, "0x%04x %s", tmp, i%7==0?"\n":"");
        HZ_UART_OUTPUT_DATA(buf, len);
    }
    
    while(1);
#endif

#endif // GOUDA_DISABLED
}

void gouda_display()
{
#ifndef GOUDA_DISABLED

    GD_START_RUN;

    while((REG_GD_STATUS & GD_STATUS_LCD_BUSY_BIT));
  
	
	//not support roration
	
	//active video layer

//    REG_GD_VL_Y_SRC = 0x00200000;
    
    
    
//    while(1);
#endif // GOUDA_DISABLED
}

