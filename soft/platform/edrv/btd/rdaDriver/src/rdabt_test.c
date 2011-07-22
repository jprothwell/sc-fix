/*************************************************************************
 *
 * MODULE NAME:    rdabt_test.c
 * PROJECT CODE:   rdabt
 * DESCRIPTION:    rda bt test mode.
 * MAINTAINER:     mql
 * CREATION DATE:  24/04/10
 *
 * SOURCE CONTROL: $Id: rdabt_test.c 6568 2011-03-16 03:52:55Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2008-2009 rda.
 *     All rights reserved.
 *
 * NOTES TO USERS:
 *   None.
 *
 *   version 3.20
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
#include "bluetooth_struct.h"
#include "rdabt_drv.h"
#include "mmi_trace.h"

#include "btd_config.h"
#include "tgt_btd_cfg.h"
#include "bt.h"
#include "pmd_m.h"
#include "mmi_trace.h"

extern CONST TGT_BTD_CONFIG_T* g_btdConfig;
extern unsigned char rdabt_chip_sel;
extern kal_uint8 bt_sleephdl;




const kal_uint32 rdabt_vco_test_578[][2] = 
{
{0x40240000,0x0004F398},//enable spi2
{0x000002FC,0x00000000},//page 0
{0x00000268,0x000003D1},//1a
{0x00000208,0x0000f180},//02
{0x40240000,0x0000F298},//disable spi2
{0x40200000,0x38001aaf},
{0x40200020,0x00000157}
};

const kal_uint8 rdabt_rf_singlehop[] = 
{
0x01,0x13,0xFc,0x01,0x00
};

const kal_uint8 rdabt_rf_inquiry[] = 
{
0x01,0x01,0x04, 0x05,0x33, 0x8B,0x9E, 0x30,0x00
};

const kal_uint8 rdabt_enable_dut[] = 
{
0x01,0x03, 0x18, 0x00
};
const kal_uint8 rdabt_enable_allscan[] = 
{
0x01, 0x1a, 0x0c, 0x01, 0x03
};
const kal_uint8 rdabt_autoaccept_connect[] = 
{
0x01,0x05, 0x0c, 0x03, 0x02, 0x00, 0x02
};

const kal_uint8 rdabt_hci_reset[] = 
{
0x01,0x03,0x0c,0x00
};





const kal_uint32 rdabt_test_pskey_578[][2] = 
{
{0x40240000,0x1004fb98},//SCU: SPI2_clk_en pclk_SPI2_en
{0x4024002c,0x00501000},//hostwake, cbt test, yuanwei_20100113
{0x10000030,0x000000fd},//Max_output_power
{0x10000038,0x0f054000},//Radio_setup_time
{0x1000003c,0xa0a0bf30},//TxRx_low_setup_time TxRx_setup_time
{0x10000034,0x05025040},//disable sleep
{0x10000000,0x0000f000},//PSKEY: Modify Flag


};

const kal_uint32 rdabt_dut_trap_5[][2] = 
{
{0x40180004,0x000146c4},//patch
{0x40180024,0x00021800},//patch
{0x4018000c,0x00013d84},//patch
{0x4018002c,0x00021800},//patch
                      
                      
                      
{0x100000c0,0xe248fc8c},//DV packet and packet length patch
{0x100000c4,0xe3540008},
{0x100000c8,0x03a06009},
{0x100000cc,0x11a06002},
{0x100000d0,0xe1c060bc},
{0x100000d4,0xe3a00470},
{0x100000d8,0xe2800e61},
{0x100000dc,0xe5901000},
{0x100000e0,0xe2400e40},
{0x100000e4,0xe5801000},
{0x100000e8,0xe2800e40},
{0x100000ec,0xe5901004},
{0x100000f0,0xe2400e40},
{0x100000f4,0xe5801004},
{0x100000f8,0xe3a01bb9},
{0x100000fc,0xe281ffc9},
                      
{0x40180008,0x0002e6fc},//patch
{0x40180028,0x100000c0},//patch
                      
{0x10009db0,0xe3d20007},//patch//patch for power control
{0x40180010,0x00007578},//patch
{0x40180030,0x10009db0},//patch
{0x40180000,0x0000000f},//patch

};


const kal_uint32 rdabt_dut_trap_7[][2] = 
{
{0x40180000,0x00000000},
{0x40180044,0x00000001},
{0x40180004,0x0002fbdc},//for cmu200 DH3&DH5, 5868+ R7
{0x40180024,0x00030010},
{0x40180008,0x0002fbe8},
{0x40180028,0x00001fbc},
                      
{0x1000affc,0xe3520006},//for power ctrl, 5868+ R7
{0x4018000c,0x000073b4},
{0x4018002c,0x1000affc},
                      
{0x40180000,0x00000007},//flag
};

const kal_uint32 rdabt_dut_trap_8[][2] = 
{

{0x10007ffc,0xe352000b},//patch//patch for power control
{0x40180004,0x000067fc},//patch
{0x40180024,0x10007ffc},//patch
                      
                      
{0x40180008,0x0002F394},//patch
{0x40180028,0x00001788},//patch
                      
{0x4018000C,0x0002F3A0},//patch
{0x4018002C,0x0002FBA8},//patch
                      
                      
{0x40180000,0x00000007},//patch
};

const kal_uint32 rdabt_test_pskey_10[][2]=
{
{0x800000a4,0x00000000},//sleep
{0x800000a8,0x0bbfbf30},//min power level
{0x80000040,0x06000000} //PSKEY: modify flag
};

const kal_uint32 rdabt_dut_trap_10[][2] = 
{

{0x40180004,0x0002F9C4},//patch//DH3$DH5 PATCH
{0x40180024,0x00001760},//patch
                      
{0x40180008,0x0002F9D0},//patch
{0x40180028,0x000301D8},//patch
                      
{0x40180000,0x00000003},//patch
};


const kal_uint32 rdabt_dut_trap_11[][2] = 
{

{0x40180100,0x0002ED2C},
{0x40180120,0x000309E4},
{0x80008ba8,0xe3a01007},
{0x40180104,0x0001cec8},
{0x40180124,0x80008ba8},
{0x40180108,0x00002A98},
{0x40180128,0x00003074},
{0x40180000,0x00000700},

};

const kal_uint32 rdabt_test_pskey_11[][2]=
{
{0x800000a4,0x00000000},//hostwake
{0x800000a8,0x0bbfbf30},//min power level
{0x80000040,0x06000000} //PSKEY: modify flag
};

const kal_uint32 rdabt_vco_test_10[][2] = 
{
{0x000002ac,0x000000b2},//3//2b
{0x00000208,0x0000f180},//02
{0x40200020,0x00000041},//tx on
};


void Rdabt_test_pskey_write_r10(void)
{
   kal_uint8 i;
   for(i=0;i<sizeof(rdabt_test_pskey_10)/sizeof(rdabt_test_pskey_10[0]);i++)
   {
      rdabt_wirte_memory(rdabt_test_pskey_10[i][0],&rdabt_test_pskey_10[i][1],1,0);
      RDABT_DELAY(1);
   }
}


void rdabt_vco_test_write_r10(void)
{
   kal_uint8 i;
   for(i=0;i<sizeof(rdabt_vco_test_10)/sizeof(rdabt_vco_test_10[0]);i++)
   {
      rdabt_wirte_memory(rdabt_vco_test_10[i][0],&rdabt_vco_test_10[i][1],1,0);
      RDABT_DELAY(1);
   }
}



void Rdabt_dut_pskey_write_r578(void)
{
   kal_uint8 i;
   for(i=0;i<sizeof(rdabt_test_pskey_578)/sizeof(rdabt_test_pskey_578[0]);i++)
   {
      rdabt_wirte_memory(rdabt_test_pskey_578[i][0],&rdabt_test_pskey_578[i][1],1,0);
      RDABT_DELAY(1);
   }
}

void Rdabt_dut_trap_write_r5(void)
{
   kal_uint8 i;
   for(i=0;i<sizeof(rdabt_dut_trap_5)/sizeof(rdabt_dut_trap_5[0]);i++)
   {
      rdabt_wirte_memory(rdabt_dut_trap_5[i][0],&rdabt_dut_trap_5[i][1],1,0);
      RDABT_DELAY(1);
   }
}


void Rdabt_dut_trap_write_r7(void)
{
   kal_uint8 i;
   for(i=0;i<sizeof(rdabt_dut_trap_7)/sizeof(rdabt_dut_trap_7[0]);i++)
   {
      rdabt_wirte_memory(rdabt_dut_trap_7[i][0],&rdabt_dut_trap_7[i][1],1,0);
      RDABT_DELAY(1);
   }
}

void Rdabt_dut_trap_write_r8(void)
{
   kal_uint8 i;
   for(i=0;i<sizeof(rdabt_dut_trap_8)/sizeof(rdabt_dut_trap_8[0]);i++)
   {
      rdabt_wirte_memory(rdabt_dut_trap_8[i][0],&rdabt_dut_trap_8[i][1],1,0);
      RDABT_DELAY(1);
   }
}

void Rdabt_dut_trap_write_r10(void)
{
   kal_uint8 i;
   for(i=0;i<sizeof(rdabt_dut_trap_10)/sizeof(rdabt_dut_trap_10[0]);i++)
   {
      rdabt_wirte_memory(rdabt_dut_trap_10[i][0],&rdabt_dut_trap_10[i][1],1,0);
      RDABT_DELAY(1);
   }
}

void Rdabt_test_pskey_write_r11(void)
{
   kal_uint8 i;
   for(i=0;i<sizeof(rdabt_test_pskey_11)/sizeof(rdabt_test_pskey_11[0]);i++)
   {
      rdabt_wirte_memory(rdabt_test_pskey_11[i][0],&rdabt_test_pskey_11[i][1],1,0);
      RDABT_DELAY(1);
   }
}


void Rdabt_dut_trap_write_r11(void)
{
   kal_uint8 i;
   for(i=0;i<sizeof(rdabt_dut_trap_11)/sizeof(rdabt_dut_trap_11[0]);i++)
   {
      rdabt_wirte_memory(rdabt_dut_trap_11[i][0],&rdabt_dut_trap_11[i][1],1,0);
      RDABT_DELAY(1);
   }
}


void Rdabt_vco_test_Write_r578()
{
   kal_uint8 i;
   for(i=0;i<sizeof(rdabt_vco_test_578)/sizeof(rdabt_vco_test_578[0]);i++)
   {
      rdabt_wirte_memory(rdabt_vco_test_578[i][0],&rdabt_vco_test_578[i][1],1,0);
      RDABT_DELAY(1);
   }
}



void Rdabt_rf_singlehop_Write()
{
      kal_uint16 num_send;	
      rdabt_uart_tx(rdabt_rf_singlehop,sizeof(rdabt_rf_singlehop),&num_send);
      RDABT_DELAY(1);
      rdabt_uart_tx(rdabt_rf_inquiry,sizeof(rdabt_rf_inquiry),&num_send);
      RDABT_DELAY(1);	  
}

void RDAbt_enable_dut_write(void)
{
      kal_uint16 num_send;
      rdabt_uart_tx(rdabt_enable_allscan,sizeof(rdabt_enable_allscan),&num_send);
      RDABT_DELAY(5);
      rdabt_uart_tx(rdabt_autoaccept_connect,sizeof(rdabt_autoaccept_connect),&num_send);
      RDABT_DELAY(5);	  	  
      rdabt_uart_tx(rdabt_enable_dut,sizeof(rdabt_enable_dut),&num_send);
      RDABT_DELAY(5);
}







void rda_bt_poweroff_for_test(void)
{

   pmd_EnablePower(PMD_POWER_BT, FALSE);
   hal_GpioClr(g_btdConfig->pinReset);
   rdabt_uart_stop();
#ifdef __RDA_SHARE_CRYSTAL__
       L1SM_SleepEnable(bt_sleephdl);
#endif
}


kal_uint8 rda_bt_poweron_for_test(void)
{
    // Enable 26M AUX clock
    hal_SysAuxClkOut(TRUE);

#ifdef __RDA_SHARE_CRYSTAL__
	if(bt_sleephdl == 0xff)
		bt_sleephdl = L1SM_GetHandle();
       L1SM_SleepDisable(bt_sleephdl);
#endif
   rdabt_chip_sel = 0;
   rda_bt_power_on();
   rda_bt_rf_init();
   rda_bt_uart_init();
   RDA_bt_Power_On_Reset();
   RDABT_DELAY(70);   
   switch(rdabt_chip_sel)
   {
#ifdef __RDA_CHIP_R5_5868__   
       case RDA_BT_R5_5868_ENUM:
	   	Rdabt_Pskey_Write_rf_r5();
              RDABT_DELAY(10);     
              Rdabt_dut_pskey_write_r578();
              break;
#endif
#ifdef __RDA_CHIP_R7_5868plus__
	case RDA_BT_R7_5868PLUS_ENUM:
		Rdabt_Pskey_Write_rf_r7();
		RDABT_DELAY(10);
		Rdabt_dut_pskey_write_r578();
		break;
#endif
#ifdef __RDA_CHIP_R8_5870__
	case RDA_BT_R8_5870_ENUM:
		Rdabt_Pskey_Write_rf_r8();
		RDABT_DELAY(10);
		Rdabt_dut_pskey_write_r578();
		break;
#endif
#if defined( __RDA_CHIP_R10_5868E__ ) || defined(__RDA_CHIP_R10_5872__)
	case RDA_BT_R10_5872_ENUM:
	case RDA_BT_R10_5868E_ENUM:
	      Rdabt_core_uart_Intialization_r10();    
      	      RDABT_DELAY(10); //wait for first uart data	
             Rdabt_Pskey_Write_rf_r10();
      	      Rdabt_test_pskey_write_r10();
	      break;
#endif
#if defined(__RDA_CHIP_R10_5868H__) || defined(__RDA_CHIP_R10_5872H__)
       case RDA_BT_R10_AL_5868H_ENUM:
	case RDA_BT_R10_AL_5872H_ENUM:
	     RDABT_rf_Intialization_r10_al();
    	     Rdabt_Pskey_Write_rf_r10_al();
            RDABT_DELAY(5); //wait for digi running
            rdabt_DC_write_r10_al();	 
            Rdabt_test_pskey_write_r10(); 
	     break;
#endif		
#ifdef __RDA_CHIP_R10_5870E__
      case RDA_BT_R10_5870E_ENUM:
	     RDABT_rf_Intialization_r10_e();
    	     Rdabt_Pskey_Write_rf_r10_e();
            RDABT_DELAY(5); //wait for digi running
            rdabt_DC_write_r10_e();	 
            Rdabt_test_pskey_write_r10(); 
	     break;
#endif		 
#ifdef __RDA_CHIP_R11_5872P__
      case RDA_BT_R11_5872P_ENUM:
	    Rdabt_Pskey_Write_rf_r11_p();
  	    RDABT_rf_Intialization_r11_p();
           rda_bt_pin_to_low();
           rda_bt_ldoon_toggle_high();     
  	    RDABT_DELAY(10); //wait for digi running
	    Rdabt_Pskey_Write_rf_r11_p();  	    
  	    RDABT_rf_Intialization_r11_p();
  	    rdabt_DC_write_r11_p(); 
           Rdabt_test_pskey_write_r11(); 
		   
	    break;
#endif		
#ifdef __RDA_CHIP_R11_5875__
      case RDA_BT_R11_5875_ENUM:
           rda_bt_ldoon_toggle_high();     
  	    RDABT_DELAY(10); //wait for digi running
  	    RDABT_rf_Intialization_r11();
  	    Rdabt_Pskey_Write_rf_r11();
  	    rdabt_DC_write_r11(); 
           Rdabt_test_pskey_write_r11(); 
	    break;
#endif		 

      default:
//		ASSERT(0);	// xiaoyifeng
		break;
   }

    // Disable 26M AUX clock
    hal_SysAuxClkOut(FALSE);
    return 1;
}

void RDAbt_send_bbtest_singlehop_inquiry(void)
{
   Rdabt_rf_singlehop_Write();
}


void RDAbt_send_vco_test(void)
{
   if(rdabt_chip_sel == RDA_BT_R5_5868_ENUM|| rdabt_chip_sel == RDA_BT_R7_5868PLUS_ENUM|| rdabt_chip_sel == RDA_BT_R8_5870_ENUM)
      Rdabt_vco_test_Write_r578();
   else if(rdabt_chip_sel == RDA_BT_R10_5872_ENUM || rdabt_chip_sel == RDA_BT_R10_5868E_ENUM 
   	       || rdabt_chip_sel ==RDA_BT_R10_AL_5868H_ENUM || rdabt_chip_sel ==RDA_BT_R10_AL_5872H_ENUM 
   	       ||rdabt_chip_sel == RDA_BT_R10_5870E_ENUM || rdabt_chip_sel == RDA_BT_R11_5872P_ENUM
   	       ||rdabt_chip_sel == RDA_BT_R11_5875_ENUM )
   	rdabt_vco_test_write_r10();
}


void rda_bt_test_enable_dut(void)
{
   switch(rdabt_chip_sel)
   {
       case RDA_BT_R5_5868_ENUM:
	   	Rdabt_dut_trap_write_r5();
	         break;
	case RDA_BT_R7_5868PLUS_ENUM:
		Rdabt_dut_trap_write_r7();
		break;
	case RDA_BT_R8_5870_ENUM:
		Rdabt_dut_trap_write_r8();
		break;
	case RDA_BT_R10_5872_ENUM:
	case RDA_BT_R10_5868E_ENUM:
       case RDA_BT_R10_AL_5868H_ENUM:
	case RDA_BT_R10_AL_5872H_ENUM:
      case RDA_BT_R10_5870E_ENUM:
	  	Rdabt_dut_trap_write_r10();
	     break;
	case RDA_BT_R11_5872P_ENUM:
	case RDA_BT_R11_5875_ENUM:		
		Rdabt_dut_trap_write_r11();
		break;
      default:
//	  	ASSERT(0);	// xiaoyifeng
		break;
   }

  RDAbt_enable_dut_write();

}

void RDA5868_send_vco_test(void)
{
	RDAbt_send_vco_test();
}

void RDA5868_send_bbtest_singlehop_inquiry(void)
{
	RDAbt_send_bbtest_singlehop_inquiry();
}
void RDA_bt_send_cmu200_enable_dut(void)
{
	rda_bt_test_enable_dut();
}

void RDA_bt_send_tc3000_enable_dut(void)
{
	rda_bt_test_enable_dut();
}

void RDA_bt_send_n4010_enable_dut(void)
{
	rda_bt_test_enable_dut();
}


const kal_uint8 rda5868_read_lmpversion[10] = {0x01 ,0x01 ,0x10 ,0x00};

kal_uint8 rdabt_get_lmpversion(void)
{
   kal_uint16 rx_length=0;
   kal_uint8 rx_buff[128];
   kal_uint8 status;
   kal_uint16 num_send;	
   kal_uint8 i;
    mmi_trace(1,"rdabt_get_lmpversion  is called!");
	mmi_trace(1,"rdabt_get_lmpversion  is called!");
   //UART_ClrRxBuffer(BT_UART_PORT,MOD_MMI);
  i= rdabt_uart_tx((kal_uint8 *)rda5868_read_lmpversion,sizeof(rda5868_read_lmpversion),&num_send);
  mmi_trace(1,"rda5868_uart_tx i=%d",i);
   sxr_Sleep(156*5);//90ms
   rx_length = UART_GetBytes(BT_UART_PORT, rx_buff, 128, &status, MOD_MMI); 
   //kal_prompt_trace(0,"rda5868_chipid = %x",rda5868_chipid);
   mmi_trace(1,"rdabt_get_lmpversion rx_length=%x",rx_length);
   for(i=0;i<rx_length;i++)
   	mmi_trace(1,"rx_buff[%d]=0x%x",i,rx_buff[i]);
   return rx_buff[66];
		
}


#endif

