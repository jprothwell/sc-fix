/*************************************************************************
 *
 * MODULE NAME:    rda5870e_init.c
 * PROJECT CODE:   rdabt
 * DESCRIPTION:    rda5870e initiation.
 * MAINTAINER:     mql
 * CREATION DATE:  16/04/10
 *
 * SOURCE CONTROL: $Id: rda5870e_init.c 5857 2011-02-12 09:15:12Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2010-2011 rda.
 *     All rights reserved.
 *
 * NOTES TO USERS:
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
#include "rdabt_drv.h"
#include "mmi_trace.h"


#ifdef  __RDA_CHIP_R10_5870E__
const unsigned short rdabt_dccal_10_e[][2]=
{
{0x3f,0x0000},
{0x30,0x0169},
{0x3C,0x0002},
{0x30,0x016B},
{0x30,0x012B}
};



const unsigned short rdabt_rf_init_10_e[][2] = 
{
{0x3f,0x0000},//page 0
{0x01,0x0FFF},//;Padrv_gain_tb_en
{0x06,0x03FF},//;PSK 
{0x08,0x00FF},//;
{0x09,0x046C},//;
{0x0B,0x021F},//;
{0x0C,0x85D8},//;
{0x0F,0x1CC8},//;adc_refi_cal_reg<2:0> Set to 000,20100820.
{0x12,0x0107},//;GFSK
{0x1B,0xE224},//;
{0x1C,0xF5F3},//;Xtal_capbank,20100820
{0x1D,0x21BB},//;5870E EVM,20100827.
{0x21,0x0000},//;Gain_psk_hi4_PSK_4>5>6>7
{0x22,0x0000},//;Gain_psk_hi4_PSK_4>5>6>7
{0x23,0x2458},//;Gain_psk_hi4_PSK_8>9>A>B
{0x24,0x8ddd},//;Gain_psk_hi4_PSK_C>D>E>F
{0x25,0x0000},//;Gain_fsk_hi4 GFSK 0>1>2>3
{0x26,0x0000},//;Gain_fsk_hi4 GFSK 4>5>6>7
{0x27,0x1235},//;Gain_fsk_hi4 GFSK 8>9>A>B
{0x28,0x5888},//;Gain_fsk_hi4 GFSK C>D>E>F
{0x3f,0x0001},//page 1
{0x00,0x4005},//;;agc0
{0x01,0x4025},//;;agc1
{0x02,0x5025},//;;agc2
{0x03,0x506D},//;;agc3
{0x04,0x50bD},//;;agc4
{0x05,0x713D},//;;agc5
{0x06,0x7A3D},//;;agc6
{0x07,0x7E3E},//;;agc7
{0x0A,0x001F},//;
{0x0D,0x0017},//;APC
{0x11,0x0000},//;;padrv_gain_1
{0x12,0x0000},//;;padrv_gain_2
{0x13,0x0000},//;;padrv_gain_3
{0x14,0x0000},//;;padrv_gain_4
{0x15,0x0000},//;;padrv_gain_5
{0x16,0x0000},//;;padrv_gain_6
{0x17,0x0000},//;;padrv_gain_7
{0x18,0x0000},//;;padrv_gain_8
{0x19,0x0000},//;;padrv_gain_9
{0x1A,0x1818},//;;padrv_gain_A
{0x1B,0x1818},//;;padrv_gain_B
{0x1C,0x6e6e},//;;padrv_gain_C
{0x1D,0x6e6e},//;;padrv_gain_D
{0x1E,0xa7a7},//;;padrv_gain_E
{0x1F,0xb5b5},//;;padrv_gain_F
//{0x1F,0xd8d8},//;;padrv_gain_F gfsk 4dBm,psk 2.4dBm
//{0x1F,0xdddd},//;;padrv_gain_F gfsk 5dBm,psk 3dBm
//{0x1F,0xe4e4},//;;padrv_gain_F gfsk 6dBm,psk 4.1dBm
//{0x1F,0xecec},//;;padrv_gain_F gfsk 7dBm,psk 5.2dBm (max power)
{0x23,0x4221},//;;use EXT32k
#ifdef __587x_USE_DCDC__
{0x24,0x48d1},//dcdc_enable set to 1
{0x26,0x45f5},//lower regulator voltage, current goes from DCDC
#else
{0x24,0x0090},//dcdc_enable set to 1
{0x26,0x4535},//;;1,reg_vbit_normal<2:0>Set to 010£»2£¬reg_vbit_deepsleep<2:0> Set to 111¡£
#endif
{0x2F,0x114E},//;;
{0x3f,0x0000},//page 0
};

const kal_uint32 rdabt_pskey_rf_10_e[][2] =
{
#if 0
//rf para setting
{0x40240000,0x0004F39c},//enable spi2
{0x800000C0,0x0000000f},//PSKEY: Total number
{0x800000C4,0x003F0000},//PSKEY: page0
{0x800000C8,0x00410003},//PSKEY: Swch_clk_adc
{0x800000CC,0x004224EC},//PSKEY: 625k if
{0x800000D0,0x0047C939},//PSKEY: mod_adc_clk
{0x800000D4,0x00431a74},//PSKEY: AM dac gain
{0x800000D8,0x0044d01A},//PSKEY: gfsk dac gain
{0x800000DC,0x004a0800},//PSKEY: 4A=0800
{0x800000E0,0x0054a020},//PSKEY: 54=A020 AGCMAX=A0 AGCMIN=20
{0x800000E4,0x0055a020},//PSKEY: 55=A020 AGC_TH_max_lg=a0 agc_th_min_lg =0x20
{0x800000E8,0x0056a542},//PSKEY: 56=a542
{0x800000EC,0x00574c18},//PSKEY: 57=4c18
{0x800000F0,0x003f0001},//PSKEY: page=1
{0x800000F4,0x00410c80},//PSKEY: phase delay
{0x800000F8,0x003f0000},//PSKEY: page =0
{0x800000Fc,0x00000000},//PSKEY: page =0
{0x80000040,0x10000000},//PSKEY: flag
{0x40240000,0x0000F29c},//enable spi2

#else
//rf para setting
{0x40240000,0x0004F39c},//enable spi2
{0x800000C0,0x00000014},//PSKEY: Total number
{0x800000C4,0x003F0000},//PSKEY: page0
{0x800000C8,0x00410003},//PSKEY: Swch_clk_adc
{0x800000CC,0x004224EC},//PSKEY: 625k if
{0x800000D0,0x0047C939},//PSKEY: mod_adc_clk
{0x800000D4,0x00431a74},//PSKEY: AM dac gain
{0x800000D8,0x0044d01A},//PSKEY: gfsk dac gain
{0x800000DC,0x004a0800},//PSKEY: 4A=0800
{0x800000E0,0x004d008a},
{0x800000E4,0x004e1f1f},
{0x800000E8,0x00694094},
{0x800000EC,0x006a1b1b},
{0x800000F0,0x0054a020},//PSKEY: 54=A020 AGCMAX=A0 AGCMIN=20                    
{0x800000F4,0x0055a020},//PSKEY: 55=A020 AGC_TH_max_lg=a0 agc_th_min_lg =0x20   
{0x800000F8,0x0056a542},//PSKEY: 56=a542                                        
{0x800000Fc,0x00574c18},//PSKEY: 57=4c18                                        
{0x80000100,0x003f0001},//PSKEY: page=1                                         
{0x80000104,0x00410c80},//PSKEY: phase delay                                    
{0x80000108,0x004603f1},
{0x8000010c,0x003f0000},//PSKEY: page =0                                        
{0x80000110,0x00000000},//PSKEY: page =0                                        
{0x80000040,0x10000000},//PSKEY: flag                                           
{0x40240000,0x0000F29c},//enable spi2

#endif
};

#ifdef __RDA_USE_LPO_32K__
const kal_uint8 rda_lpo_setting_10_e[][14] = 
{
{0x80000080,0x000060cc},//phy cfg
{0x80000040,0x00010000},//flag
{0x40240038,0x03500814},//20 div
{0x40240030,0x29010800},//
{0x4024001c,0x40408140},//
{0x40240000,0x0100f29c},//lpo enable

{0x80000018,0xea000064},
{0x800001b0,0xe5947004},
{0x800001b4,0xe1a00004},
{0x800001b8,0xe3a0eba6},
{0x800001bc,0xe28ee06c},
{0x800001c0,0xea000003},
                      
{0x800002b8,0xeafffff2},
{0x800002bc,0x800002cc},
{0x800002c0,0x40240000},
{0x800002c4,0x0000cb20},
{0x800002c8,0x000004e2},
{0x800002cc,0x00000000},
            
            
{0x40180114,0x00029868},
{0x40180134,0x00032e90},
{0x40180118,0x00024df8},
{0x40180138,0x00000014},
#if defined(_RDA_PATCH_UART2_) || defined(_RDA_PATCH_UART3_)
{0x40180000,0x00007fff},//patch flag
#else
{0x40180000,0x00007ff8},//patch flag
#endif

};
const kal_uint32 rdabt_lpo_patch_10_e[] =
{
0x800001C4,0xE51FF004,0x00005584,0xE51FF004,0x00000360,0xE92D43F8,0xE1A05000,0xE1D000B0,0xE59F90D4,0xE3500001,0x1A000027,0xE59F40CC,0xE5947000,0xE5948038,0xE3870480,0xE5840000,0xE3A01000,0xE3A00001,0xEBFFFFED,0xE3880980,0xE5840038,0xE3A01000,0xE3A00004,0xEBFFFFE8,0xE594003C,0xE1A06700,0xE1A06726,0xE5848038,0xE5847000,0xE5954008,0xE59F0084,0xE0010490,0xE1A00006,0xEBFFFFE0,0xE1A07000,0xE59F0074,0xE0010190,0xE1A00006,0xEBFFFFDB,0xE1A01000,0xE3C76001,0xE0440006,0xE1A04100,0xE3A00F4E,0xEBFFFFD5,0xE2071001,0xE0800101,0xE0440000,0xE5890000,0xE5856008,0xE8BD83F8,0xE3500020,0x03A01470,0x0591002C,0x01A00800,0x01B00FA0,0x1AFFFFF8,0xE591001C,0xE5992000,0xE0800002,0xE2800001,0xE581001C
};
#endif


void rdabt_DC_write_r10_e(void)
{
	unsigned char i=0;
	for(i =0;i<(sizeof(rdabt_dccal_10_e)/sizeof(rdabt_dccal_10_e[0])-1); i++)
	{
		rdabt_iic_rf_write_data(rdabt_dccal_10_e[i][0],&rdabt_dccal_10_e[i][1],1);
	}
	RDABT_DELAY(40);
	rdabt_iic_rf_write_data(rdabt_dccal_10_e[i][0],&rdabt_dccal_10_e[i][1],1);
	RDABT_DELAY(10);


}


void RDABT_rf_Intialization_r10_e(void)
{
	unsigned char i=0;
	for(i =0;i<sizeof(rdabt_rf_init_10_e)/sizeof(rdabt_rf_init_10_e[0]); i++)
	{
		rdabt_iic_rf_write_data(rdabt_rf_init_10_e[i][0],&rdabt_rf_init_10_e[i][1],1);
	}
	RDABT_DELAY(10); 
}


void Rdabt_Pskey_Write_rf_r10_e(void)
{
   kal_int32 i;
   for(i=0;i<sizeof(rdabt_pskey_rf_10_e)/sizeof(rdabt_pskey_rf_10_e[0]);i++)
   {
      rdabt_wirte_memory(rdabt_pskey_rf_10_e[i][0],&rdabt_pskey_rf_10_e[i][1],1,0);
      RDABT_DELAY(2);
   }	
}

#ifdef __RDA_USE_LPO_32K__
void Rdabt_lpo_Setting__write_r10_e(void)
{
   kal_int32 i;

   for(i=0;i<sizeof(rda_lpo_setting_10_e)/sizeof(rda_lpo_setting_10_e[0]);i++)
   {
      rdabt_wirte_memory(rda_lpo_setting_10_e[i][0],&rda_lpo_setting_10_e[i][1],1,0);
      RDABT_DELAY(1);
   }	

}

void Rdabt_lpo_patch_write_r10_e(void)
{
   rdabt_wirte_memory(rdabt_lpo_patch_10_e,sizeof(rdabt_lpo_patch_10_e),61,0);
   RDABT_DELAY(10);
}

#endif   


void RDABT_core_Intialization_r10_e(void)
{
    kal_prompt_trace(0,"RDABT_core_Intialization_r10_al");


    RDA_bt_Power_On_Reset();
    RDABT_DELAY(10); //wait for digi running
    RDABT_rf_Intialization_r10_e();
    Rdabt_Pskey_Write_rf_r10_e();
    RDABT_DELAY(5); //wait for digi running
    rdabt_DC_write_r10_e();	 

    Rdabt_Pskey_Write_r10();
    Rdabt_unsniff_prerxon_write_r10();
    Rdabt_patch_write_r10();    
    Rdabt_setfilter_r10();
    Rdabt_acl_patch_write_r10();

    Rdabt_trap_write_r10();
#ifdef __RDA_USE_LPO_32K__	
    Rdabt_lpo_Setting__write_r10_e();	
    Rdabt_lpo_patch_write_r10_e();
#endif	
	
}


#endif


#endif
