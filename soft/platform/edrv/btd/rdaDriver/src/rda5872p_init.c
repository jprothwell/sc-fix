/*************************************************************************
 *
 * MODULE NAME:    rda5872p_init.c
 * PROJECT CODE:   rdabt
 * DESCRIPTION:    rda5872p initiation.
 * MAINTAINER:     mql
 * CREATION DATE:  05/01/10
 *
 * SOURCE CONTROL: $Id: rda5872p_init.c 6502 2011-03-12 06:16:28Z pengzg $
 *
 * LICENSE:
 *     This source code is copyright (c) 2010-2011 rda.
 *     All rights reserved.
 *
 * NOTES TO USERS:
 *   
 *   version 3.23
 *				 
 *************************************************************************/

 #ifdef __BT_RDABT__
#include "kal_release.h"      	/* Basic data type */
#include "stack_common.h"          /* message and module IDs */
#include "stack_msgs.h"            /* enum for message IDs */
#include "app_ltlcom.h"       	/* Task message communiction */
//#include "syscomp_config.h"         /* type of system module components */
//#include "task_config.h"      	/* Task creation */
#include "stacklib.h"        	   /* Basic type for dll, evshed, stacktimer */
#include "event_shed.h"       	/* Event scheduler */
#include "stack_timer.h"      	/* Stack timer */
#include "stdio.h"                  /* Basic C I/O functions */
//#include "eint.h"                   /* External interrupt */

#include "uart_sw.h"               /* header file for UART */
#include "rdabt_drv.h"
#include "mmi_trace.h"


#if defined( __RDA_CHIP_R11_5872P__)
const unsigned int rdabt_rf_init_11_p[][2] = 
{
{0x0000003f,0x00000000},
{0x00000001,0x00001FFF},//;
{0x00000006,0x000007F4},//;padrv_set,increase the power.
{0x00000008,0x000001E7},//;
{0x00000009,0x00000520},//;
{0x0000000B,0x000003DF},//;filter_cap_tuning<3:0>1101
{0x0000000C,0x000085e8},//;
{0x0000000F,0x00001dCC},//;
{0x00000012,0x000007F4},//;padrv_set,increase the power.
{0x00000013,0x00000327},//;padrv_set,increase the power.
{0x00000014,0x00000CFE},//;
{0x00000015,0x00000526},//;Pll_bypass_ontch:1,improve ACPR.
{0x00000016,0x00008918},//;Pll_bypass_ontch:1,improve ACPR.
{0x00000018,0x00008800},//;Pll_bypass_ontch:1,improve ACPR.
{0x00000019,0x000010c8},//;Pll_bypass_ontch:1,improve ACPR.
{0x0000001B,0x000080c2},//;
{0x0000001C,0x00003613},//;
{0x0000001D,0x000013E3},//;Pll_cp_bit_tx<3:0>1110;13D3
{0x0000001E,0x0000300C},//;Pll_lpf_gain_tx<1:0> 00;304C
{0x00000023,0x00003335},//;Pll_bypass_ontch:1,improve ACPR.
{0x00000024,0x00008aaf},//;
{0x00000027,0x00001112},//;Pll_bypass_ontch:1,improve ACPR.
{0x00000028,0x0000345f},//;Pll_bypass_ontch:1,improve ACPR.
{0x0000003f,0x00000001},
{0x00000000,0x0000043F},//;agc
{0x00000001,0x0000467F},//;agc
{0x00000002,0x000068FF},//;agc
{0x00000003,0x000067FF},//;agc
{0x00000004,0x000057FF},//;agc
{0x00000005,0x00007BFF},//;agc
{0x00000006,0x00003FFF},//;agc
{0x00000007,0x00007FFF},//;agc
{0x00000018,0x0000b1B1}, //; 98H,16'hB1B1;                                                             
{0x00000019,0x0000b1b1}, //; 99H,16'hB1B1;                                                             
{0x0000001a,0x0000b1b1}, //; 9AH,16'hB1B1;                                                             
{0x0000001b,0x0000b0b0}, //; 9BH,16'hB0B0;                                                             
{0x0000001c,0x0000d7b0}, //; 9CH,16'hD7B0;                                                             
{0x0000001d,0x0000e4e0}, //; 9DH,16'hE4E0;                                                             
{0x0000001e,0x0000e7e9}, //; 9EH,16'hE7E9;                                                             
{0x0000001F,0x0000e9E9},//;padrv_gain
{0x00000023,0x00004224},//;ext32k
#ifdef __5875_USE_DCDC__
{0x00000024,0x00009b10},
#else
{0x00000024,0x00000110},
#endif
{0x00000025,0x000043A0},//;ldo_vbit:110,2.04v
{0x00000026,0x000049B5},//;reg_ibit:100,reg_vbit:110,1.1v,reg_vbit_deepsleep:110,750mV
{0x00000032,0x00000059},//;TM mod
{0x0000003f,0x00000000}};


const kal_uint32 rdabt_enable_spi2_r11_p[][2] =
{
{0x40240000,0x0004f39c}, //        SPI2_CLK_EN PCLK_SPI2_EN                                  
};


const kal_uint32 rdabt_disable_spi2_r11_p[][2] = 
{
{0x40240000,0x0000f29c},//; SPI2_CLK_EN PCLK_SPI2_EN
};

const kal_uint32 rdabt_pskey_rf_11_p[][2] =
{
{0x40240000,0x0004f39c}, //; SPI2_CLK_EN PCLK_SPI2_EN
{0x800000C0,0x00000020}, //; CHIP_PS PSKEY: Total number -----------------
{0x800000C4,0x003F0000}, //;         PSKEY: Page 0
{0x800000C8,0x00410003}, //;         PSKEY: Swch_clk_ADC
{0x800000CC,0x004225BD}, //;         PSKEY: dig IF 625K IF  by lihua20101231
{0x800000D0,0x004908E4}, //;         PSKEY: freq_offset_for rateconv  by lihua20101231(replace 47H)
{0x800000D4,0x0043B074}, //;         PSKEY: AM dac gain, 20100605
{0x800000D8,0x0044D01A}, //;         PSKEY: gfsk dac gain, 20100605//22
{0x800000DC,0x004A0800}, //;         PSKEY: 4AH=0800
{0x800000E0,0x0054A020}, //;         PSKEY: 54H=A020;agc_th_max=A0;agc_th_min=20
{0x800000E4,0x0055A020}, //;         PSKEY: 55H=A020;agc_th_max_lg=A0;agc_th_min_lg=20
{0x800000E8,0x0056A542}, //;         PSKEY: 56H=A542
{0x800000EC,0x00574C18}, //;         PSKEY: 57H=4C18
{0x800000F0,0x003F0001}, //;         PSKEY: Page 1               
{0x800000F4,0x00410900}, //;         PSKEY: C1=0900;Phase Delay, 20101029 
{0x800000F8,0x0046033F}, //;         PSKEY: C6=033F;modulation Index;delta f2=160KHZ,delta f1=164khz
{0x800000FC,0x004C0000}, //;         PSKEY: CC=0000;20101108   
{0x80000100,0x004D0015}, //;         PSKEY: CD=0015;           
{0x80000104,0x004E002B}, //;         PSKEY: CE=002B;           
{0x80000108,0x004F0042}, //;         PSKEY: CF=0042            
{0x8000010C,0x0050005A}, //;         PSKEY: D0=005A            
{0x80000110,0x00510073}, //;         PSKEY: D1=0073            
{0x80000114,0x0052008D}, //;         PSKEY: D2=008D            
{0x80000118,0x005300A7}, //;         PSKEY: D3=00A7            
{0x8000011C,0x005400C4}, //;         PSKEY: D4=00C4            
{0x80000120,0x005500E3}, //;         PSKEY: D5=00E3            
{0x80000124,0x00560103}, //;         PSKEY: D6=0103            
{0x80000128,0x00570127}, //;         PSKEY: D7=0127            
{0x8000012C,0x0058014E}, //;         PSKEY: D8=014E            
{0x80000130,0x00590178}, //;         PSKEY: D9=0178            
{0x80000134,0x005A01A1}, //;         PSKEY: DA=01A1            
{0x80000138,0x005B01CE}, //;         PSKEY: DB=01CE            
{0x8000013C,0x005C01FF}, //;         PSKEY: DC=01FF            
{0x80000140,0x003F0000}, //;         PSKEY: Page 0
{0x80000040,0x10000000}, //;         PSKEY: Flage
//{0x40240000,0x0000f29c}, //; SPI2_CLK_EN PCLK_SPI2_EN 
{0x40200044,0x0000003c},
{0x50000010,0x00000122}
};


const kal_uint32 rdabt_dccal_11_p[][2]=
{
{0x00000030,0x00000129},
{0x00000030,0x0000012b}
};

void  RDABT_rf_Intialization_r11_p(void)
{
	unsigned char i=0;
//       rdabt_wirte_memory(rdabt_enable_spi2_r11_p[0][0],&rdabt_enable_spi2_r11_p[0][1],1,0);	
//       RDABT_DELAY(1);
	for(i =0;i<sizeof(rdabt_rf_init_11_p)/sizeof(rdabt_rf_init_11_p[0]); i++)
	{
		rdabt_wirte_memory(rdabt_rf_init_11_p[i][0],&rdabt_rf_init_11_p[i][1],1,1);		
	       RDABT_DELAY(2);

	}	
	RDABT_DELAY(10);  
}



void Rdabt_Pskey_Write_rf_r11_p(void)
{
   kal_int32 i;
   for(i=0;i<sizeof(rdabt_pskey_rf_11_p)/sizeof(rdabt_pskey_rf_11_p[0]);i++)
   {

      rdabt_wirte_memory(rdabt_pskey_rf_11_p[i][0],&rdabt_pskey_rf_11_p[i][1],1,0);
      RDABT_DELAY(3);
   }	
}

void rdabt_DC_write_r11_p(void)
{
   kal_int32 i;
   for(i=0;i<sizeof(rdabt_dccal_11_p)/sizeof(rdabt_dccal_11_p[0]);i++)
   {

      rdabt_wirte_memory(rdabt_dccal_11_p[i][0],&rdabt_dccal_11_p[i][1],1,1);
      RDABT_DELAY(3);
   }	
   rdabt_wirte_memory(rdabt_disable_spi2_r11_p[0][0],&rdabt_disable_spi2_r11_p[0][1],1,0);	
   RDABT_DELAY(1);
   
}


void RDABT_core_Intialization_r11_p(void)
{
    kal_prompt_trace(0,"RDABT_core_Intialization_r11_p begin");
    Rdabt_Pskey_Write_rf_r11_p(); 	
    RDABT_rf_Intialization_r11_p();
    rda_bt_pin_to_low();
    rda_bt_ldoon_toggle_high();    
    RDABT_DELAY(10); //wait for digi running
    rda_bt_pin_to_high();
    RDABT_DELAY(10); //wait for digi running
    Rdabt_Pskey_Write_rf_r11_p();    
    RDABT_rf_Intialization_r11_p();
    rdabt_DC_write_r11_p();
    Rdabt_Pskey_Write_r11();
#ifdef __RDA_ESERMODE_CHANGE__
    Rdabt_esermode_write_r11();
#endif	
    Rdabt_unsniff_prerxon_write_r11();
    Rdabt_setfilter_r11();
    Rdabt_trap_write_r11();
    kal_prompt_trace(0,"RDABT_core_Intialization_r11_p end");	
}

extern const kal_uint8 rdabt_hard_flow_ctrl[14]; //hard flow ctrl enable 
static kal_uint8 rdabt_aotu_baudrate_data[] = {0x01,0x77,0xfc,0x00};
extern BOOL  IsFlowCtrlStart;

void rdabt_5872p_baudrate_flow_ctrl(void)
{
       kal_uint16 num_send;	
	rda5868_uart_tx(rdabt_aotu_baudrate_data,sizeof(rdabt_aotu_baudrate_data),&num_send); 
	sxr_Sleep(100 MS_WAITING);
	//DEVICE *pDev=&Uart_Dev[BT_UART_PORT];
   	rda5868_uart_configure(921600/*460800*//*230400*/,FALSE);
	sxr_Sleep(200 MS_WAITING);
	//pDev->uartCfg.afc = HAL_UART_AFC_MODE_RX_TRIG_16;
	rda5868_uart_tx(rdabt_hard_flow_ctrl,sizeof(rdabt_hard_flow_ctrl),&num_send); 
	IsFlowCtrlStart=TRUE;
   	sxr_Sleep(200 MS_WAITING);
	rda5868_uart_configure(921600/*460800*//*230400*/,IsFlowCtrlStart);
}

#endif

#endif



 
