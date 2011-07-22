/*************************************************************************
 *
 * MODULE NAME:    Rdabt_poweron.c
 * PROJECT CODE:   rdabt
 * DESCRIPTION:    Rdabt_poweron for bt
 * MAINTAINER:     mql
 * CREATION DATE:  24/04/10
 *
 * SOURCE CONTROL: $Id: rdabt_poweron.c 7309 2011-04-29 03:21:22Z huangjf $
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
#include "intrctrl.h"
#include "uart_sw.h"               /* header file for UART */
#include "rdabt_drv.h"
#include "mmi_trace.h"

#include "btd_config.h"
#include "tgt_btd_cfg.h"
#include "pmd_m.h"
#include "mmi_trace.h"

extern CONST TGT_BTD_CONFIG_T* g_btdConfig;
unsigned char rdabt_chip_sel = 0;
kal_uint8 bt_sleephdl =0xff;

////////////////////////////////////////io/////////////////////////////////
void RDA_bt_Power_On_Reset(void)
{   
	hal_GpioClr(g_btdConfig->pinReset);
	RDABT_DELAY(10);   
	hal_GpioSet(g_btdConfig->pinReset);
}

void RDA_bt_Power_Off(void)
{
#ifdef __RDA_CHIP_R10_5868E__
    if(rdabt_chip_sel == RDA_BT_R10_5868E_ENUM)
    {
#ifdef __RDABT_USE_IIC__   
//       RDA_bt_SerialCommInit(); 
       RDABT_DELAY(1);
#endif    
       Rdabt_sleep_patch_off_write_r10();  
    }
#endif	
    kal_prompt_trace(0,"RDA_bt_Power_Off");
#ifdef __RDA_SHARE_CRYSTAL__
//   GPIO_ModeSetup(rda_bt_clkreq,0);
//   GPIO_InitIO(0,rda_bt_clkreq);
#endif

   hal_GpioClr(g_btdConfig->pinReset);
   pmd_EnablePower(PMD_POWER_BT, FALSE);
#ifdef __RDA_CHIP_R10_5872__
    if(rdabt_chip_sel == RDA_BT_R10_5872_ENUM)
    {
#ifdef __RDABT_USE_IIC__   
//       RDA_bt_SerialCommInit(); 
       RDABT_DELAY(1);
#endif    
       Rdabt_sleep_patch_off_write_r10();  
    }
#endif	

#ifdef __RDABT_USE_IIC__
   RDA_bt_SerialCommdeInit(); 
#endif   
}







#if defined( __RDA_AUTO_CHIP_5868__)
unsigned int rda5868_chipid = 0;
const unsigned int chipidaddr = 0x40200028;
kal_uint8 rda5868_read_chip[10] = {0x01,0x01,0xfd,0x06,0x00,0x01,0x28,0x00,0x20,0x40};

kal_uint8 rdabt_get_chipid(void)
{
	rdabt_iic_core_read_data(chipidaddr,&rda5868_chipid,1);
	kal_prompt_trace(0,"rda5868_chipid = %x",rda5868_chipid);
	if((rda5868_chipid & 0x00ffff) == 0x5868)
	{
	    if(((rda5868_chipid>>16)&0x000000ff) == 0x11)
			return RDA_BT_R7_5868PLUS_ENUM;
	    else if(((rda5868_chipid>>16)&0x000000ff) == 0x03 || ((rda5868_chipid>>16)&0x000000ff) == 0x01 )
			return RDA_BT_R5_5868_ENUM;
           else 
		   	return RDA_NONE_CHIP_ENUM;
	}
	else if((rda5868_chipid & 0x00ffff) == 0x5872)
	{
	    if(((rda5868_chipid>>16)&0x000000ff) == 0x00)
			return RDA_BT_R10_5868E_ENUM;
	    else if(((rda5868_chipid>>16)&0x000000ff) == 0x01)
			return RDA_BT_R10_AL_5868H_ENUM;
	    else 
			return RDA_NONE_CHIP_ENUM;
	}
	else
	{
	    return RDA_NONE_CHIP_ENUM;
       }

}


#elif defined(__RDA_AUTO_CHIP_587x__)

unsigned short rdabt_chipid,rdabt_revid;
 unsigned char pageaddr = 0x3f;
 unsigned short pagevalue = 0x0001;

const unsigned char chipidaddr = 0x20;
const unsigned char revidaddr = 0x21;



kal_uint8 rdabt_get_chipid(void)
{
	rdabt_iic_rf_write_data(pageaddr,&pagevalue,1);
	rdabt_iic_rf_read_data(chipidaddr,&rdabt_chipid,1);
	rdabt_iic_rf_read_data(revidaddr,&rdabt_revid,1);	
	kal_prompt_trace(0,"rdabt_chipid = %x",rdabt_chipid);
    if(rdabt_chipid == 0x5872)
    {    
        if((rdabt_revid & 0x001f)<0x10)
		return RDA_BT_R10_5872_ENUM;
	else if((rdabt_revid & 0x0018)==0x10)
		return RDA_BT_R10_AL_5872H_ENUM;
	else if((rdabt_revid & 0x0018)==0x18)
		return RDA_BT_R10_5870E_ENUM;
	else
		return RDA_NONE_CHIP_ENUM;
     }
     else if(rdabt_chipid == 0x5875)
	 return RDA_BT_R11_5875_ENUM;
     else if(rdabt_chipid == 0xffff)
	 return RDA_BT_R11_5872P_ENUM;
     else
        return RDA_BT_R8_5870_ENUM;
}

#endif


#ifdef __RDA_SOFT_RESET__
const unsigned short rda5400_swreset[][2] = 
{
{0x30,0x28},
{0x30,0x29}
};

void rdabt_sw_reset_write(void)
{
	rdabt_iic_rf_write_data(rda5400_swreset[0][0],&rda5400_swreset[0][1],1);
	RDABT_DELAY(30);
	rdabt_iic_rf_write_data(rda5400_swreset[1][0],&rda5400_swreset[1][1],1);
	RDABT_DELAY(10);
}

#endif

void rda_bt_32k_on(void)
{
// xiaoyifeng must note
}


void rda_bt_pin_setup(void)
{
	pmd_EnablePower(PMD_POWER_BT, TRUE);
    I2C_Open();
}

void rda_bt_pin_to_high(void)
{
	pmd_EnablePower(PMD_POWER_BT, TRUE);
	RDABT_DELAY(5);   
	hal_GpioSet(g_btdConfig->pinReset);
	RDABT_DELAY(70);   
}

void rda_bt_ldoon_toggle_high(void)
{
	pmd_EnablePower(PMD_POWER_BT, TRUE);
	RDABT_DELAY(1);   
}

void rda_bt_pin_to_low(void)
{
	pmd_EnablePower(PMD_POWER_BT, FALSE);
	RDABT_DELAY(1);
	hal_GpioClr(g_btdConfig->pinReset);
	RDABT_DELAY(5);
}

   
void rda_bt_power_on(void)
{
    rda_bt_pin_setup();
    rda_bt_32k_on();
#ifdef __RDABT_USE_IIC__   
//   RDA_bt_SerialCommInit(); 
   RDABT_DELAY(1);
#endif
   rda_bt_ldoon_toggle_high();
   rda_bt_pin_to_low();
#ifdef __RDA_BT_5868_SERIES__
   rda_bt_pin_to_high();
#endif

#ifdef __RDABT_USE_IIC__   

   rdabt_chip_sel = 0;
#if defined( __RDA_AUTO_CHIP_5868__) || defined(__RDA_AUTO_CHIP_587x__)
	   rdabt_chip_sel = rdabt_get_chipid();
	   rdabt_chip_sel = rdabt_get_chipid();
	   kal_prompt_trace(0,"rdabt_chip_sel = %d",rdabt_chip_sel);
	   RDABT_DELAY(1);       
#else
#ifdef __RDA_CHIP_R5_5868__
	rdabt_chip_sel = RDA_BT_R5_5868_ENUM;
#endif///__RDA_CHIP_R5_5868__

#ifdef __RDA_CHIP_R7_5868plus__
	rdabt_chip_sel = RDA_BT_R7_5868PLUS_ENUM;	
#endif//__RDA_CHIP_R7_5868plus__

#ifdef __RDA_CHIP_R8_5870__
	rdabt_chip_sel = RDA_BT_R8_5870_ENUM;	
#endif//__RDA_CHIP_R8_5870__

#ifdef __RDA_CHIP_R10_5868E__
	rdabt_chip_sel = RDA_BT_R10_5868E_ENUM;	
#endif//__RDA_CHIP_R10_5868E__

#ifdef __RDA_CHIP_R10_5872__
	rdabt_chip_sel = RDA_BT_R10_5872_ENUM;
#endif//__RDA_CHIP_R10_5872__

#ifdef __RDA_CHIP_R10_5872H__
	rdabt_chip_sel = RDA_BT_R10_AL_5872H_ENUM;
#endif//__RDA_CHIP_R10_5872__

#ifdef __RDA_CHIP_R10_5868H__
	rdabt_chip_sel = RDA_BT_R10_AL_5868H_ENUM;
#endif//__RDA_CHIP_R10_5872__

#ifdef __RDA_CHIP_R10_5870E__
	rdabt_chip_sel = RDA_BT_R10_5870E_ENUM;
#endif//__RDA_CHIP_R10_5872__

#ifdef __RDA_CHIP_R11_5872P__
	rdabt_chip_sel = RDA_BT_R11_5872P_ENUM;
#endif//__RDA_CHIP_R10_5872__

#ifdef __RDA_CHIP_R11_5875__
	rdabt_chip_sel = RDA_BT_R11_5875_ENUM;
#endif
#endif//__RDA_AUTO_CHIP__
	   
#ifdef __RDA_SOFT_RESET__
   if(rdabt_chip_sel == RDA_BT_R7_5868PLUS_ENUM|| rdabt_chip_sel == RDA_BT_R5_5868_ENUM)
   	rdabt_sw_reset_write();
#endif

#else //__RDABT_USE_IIC__

#ifdef __RDA_CHIP_R5_5868__
	rdabt_chip_sel = RDA_BT_R5_5868_ENUM;
#endif///__RDA_CHIP_R5_5868__

#ifdef __RDA_CHIP_R7_5868plus__
	rdabt_chip_sel = RDA_BT_R7_5868PLUS_ENUM;	
#endif//__RDA_CHIP_R7_5868plus__

#ifdef __RDA_CHIP_R8_5870__
	rdabt_chip_sel = RDA_BT_R8_5870_ENUM;	
#endif//__RDA_CHIP_R8_5870__

#ifdef __RDA_CHIP_R10_5868E__
	rdabt_chip_sel = RDA_BT_R10_5868E_ENUM;	
#endif//__RDA_CHIP_R10_5868E__

#ifdef __RDA_CHIP_R10_5872__
	rdabt_chip_sel = RDA_BT_R10_5872_ENUM;
#endif//__RDA_CHIP_R10_5872__

#ifdef __RDA_CHIP_R10_5872H__
	rdabt_chip_sel = RDA_BT_R10_AL_5872H_ENUM;
#endif//__RDA_CHIP_R10_5872__

#ifdef __RDA_CHIP_R10_5868H__
	rdabt_chip_sel = RDA_BT_R10_AL_5868H_ENUM;
#endif//__RDA_CHIP_R10_5872__

#ifdef __RDA_CHIP_R10_5870E__
	rdabt_chip_sel = RDA_BT_R10_5870E_ENUM;
#endif//__RDA_CHIP_R10_5872__

#ifdef __RDA_CHIP_R11_5872P__
	rdabt_chip_sel = RDA_BT_R11_5872P_ENUM;
#endif//__RDA_CHIP_R10_5872__

#ifdef __RDA_CHIP_R11_5875__
	rdabt_chip_sel = RDA_BT_R11_5875_ENUM;
#endif //__RDA_CHIP_R11_5875__

#endif   //__RDABT_USE_IIC__

#ifdef __RDA_BT_5868_SERIES__
   RDA_bt_Power_On_Reset();
   RDABT_DELAY(70);
#endif
   if(rdabt_chip_sel == RDA_NONE_CHIP_ENUM)
   {
//   	ASSERT(0);	//xiaoyifeng
   }

}


void rda_bt_rf_init(void)
{
#ifdef __RDABT_USE_IIC__   
    switch(rdabt_chip_sel)
    {
#ifdef __RDA_CHIP_R7_5868plus__	   
    case RDA_BT_R7_5868PLUS_ENUM:
        RDABT_rf_Intialization_r7(); 		
	 break;
#endif
#ifdef __RDA_CHIP_R5_5868__	   
    case RDA_BT_R5_5868_ENUM:
       RDABT_rf_Intialization_r5();
	break;
#endif
#ifdef __RDA_CHIP_R8_5870__	   
    case RDA_BT_R8_5870_ENUM:
        RDABT_rf_Intialization_r8();
        break;
#endif
#ifdef __RDA_CHIP_R10_5868E__	   
    case RDA_BT_R10_5868E_ENUM:
        RDABT_rf_Intialization_r10();
        break;
#endif
#ifdef __RDA_CHIP_R10_5872__	   
    case RDA_BT_R10_5872_ENUM:
        RDABT_rf_Intialization_r10();
        break;
#endif
#ifdef __RDA_CHIP_R10_5868H__
    case RDA_BT_R10_AL_5868H_ENUM:
        RDABT_rf_Intialization_r10_al();   	   
        break;
#endif
#ifdef __RDA_CHIP_R10_5872H__	   
    case RDA_BT_R10_AL_5872H_ENUM:
        RDABT_rf_Intialization_r10_al();
        break;
#endif
#ifdef __RDA_CHIP_R10_5870E__
    case RDA_BT_R10_5870E_ENUM:
        RDABT_rf_Intialization_r10_e();
        break;
#endif
#ifdef __RDA_CHIP_R11_5872P__
    case RDA_BT_R11_5872P_ENUM:
	 break; 
#endif

#ifdef __RDA_CHIP_R11_5875__
    case RDA_BT_R11_5875_ENUM:
	 RDABT_rf_Intialization_r11();	
	 break; 
#endif

    default:
        break;

    }

#endif//__RDA5868_USE_IIC__

#ifdef __RDA_BT_587x_SERIES__ 
	rda_bt_pin_to_high();
#endif



}

void rda_bt_uart_init(void)
{
	rdabt_uart_start();
	rda5868_uart_configure(115200,FALSE);
}

int rdabt_poweron_init(void)
{
	kal_prompt_trace(0,"rdabt_poweron_init");

#ifdef __RDA_SHARE_CRYSTAL__
	if(bt_sleephdl == 0xff)
		bt_sleephdl = L1SM_GetHandle();
       L1SM_SleepDisable(bt_sleephdl);
#endif
	rda_bt_power_on();
	rda_bt_rf_init();
//	rda_bt_uart_init();


    switch(rdabt_chip_sel)
    {
#ifdef __RDA_CHIP_R7_5868plus__	   
    case RDA_BT_R7_5868PLUS_ENUM:
        RDABT_core_Intialization_r7(); 		
	 break;
#endif
#ifdef __RDA_CHIP_R5_5868__	   
    case RDA_BT_R5_5868_ENUM:
       RDABT_core_Intialization_r5();
	break;
#endif
#ifdef __RDA_CHIP_R8_5870__	   
    case RDA_BT_R8_5870_ENUM:
        RDABT_core_Intialization_r8();
        break;
#endif
#ifdef __RDA_CHIP_R10_5868E__	   
    case RDA_BT_R10_5868E_ENUM:
        RDABT_core_Intialization_r10();
        break;
#endif
#ifdef __RDA_CHIP_R10_5872__	   
    case RDA_BT_R10_5872_ENUM:
        RDABT_core_Intialization_r10();
        break;
#endif
#ifdef __RDA_CHIP_R10_5868H__
    case RDA_BT_R10_AL_5868H_ENUM: 
        RDABT_core_Intialization_r10_al();   	   
        break;
#endif
#ifdef __RDA_CHIP_R10_5872H__	   
    case RDA_BT_R10_AL_5872H_ENUM:
        RDABT_core_Intialization_r10_al();
        break;
#endif
#ifdef __RDA_CHIP_R10_5870E__
    case RDA_BT_R10_5870E_ENUM:
        RDABT_core_Intialization_r10_e();
        break;
#endif
#ifdef __RDA_CHIP_R11_5872P__
    case RDA_BT_R11_5872P_ENUM:
        RDABT_core_Intialization_r11_p();
        break;
#endif

#ifdef __RDA_CHIP_R11_5875__
    case RDA_BT_R11_5875_ENUM:
        RDABT_core_Intialization_r11();
        break;
#endif
  	default:
		return (-1);
		break;
		
    }
	//just to ensure the I2C return to normal
	{
		unsigned int chipidaddr = 0x40200028;
		unsigned int rda5868_chipid = 0;
		rdabt_iic_core_read_data(chipidaddr,&rda5868_chipid,1);
	}

	// rdabt_uart_stop();    
	RDABT_DELAY(5);     

#ifdef __RDA_SHARE_CRYSTAL__
       L1SM_SleepEnable(bt_sleephdl);
#endif
	return (0);	
}

void rdabt_poweronphone_init(void)
{
	pmd_EnablePower(PMD_POWER_BT, FALSE);
	hal_GpioClr(g_btdConfig->pinReset);
}


void rdabt_poweroffphone(void)
{
#if defined( __RDA_CHIP_R10_5872__) || defined(__RDA_CHIP_R10_5868E__)
    if(rdabt_chip_sel == RDA_BT_R10_5872_ENUM || rdabt_chip_sel == RDA_BT_R10_5868E_ENUM)
    {
       Rdabt_sleep_patch_off_write_r10();  
    }
#endif	

}

const kal_uint8 rdabt_hard_flow_ctrl[] = {0x01,0x02,0xfd,0x0a,0x00,0x01,0x10,0x00,0x00,0x50,0x22,0x01,0x00,0x00}; //hard flow ctrl enable 
extern BOOL  IsFlowCtrlStart;
extern const kal_uint8 rdabt_wakeup_data[4];

void rdabt_baudrate_ctrl(void)
{
	kal_prompt_trace(0,"rdabt_baudrate_ctrl");

#if defined( __RDA_CHIP_R10_5872__) || defined(__RDA_CHIP_R10_5868E__) || defined(__RDA_CHIP_R11_5875__)  
    if(rdabt_chip_sel == RDA_BT_R10_5872_ENUM || rdabt_chip_sel == RDA_BT_R10_5868E_ENUM || rdabt_chip_sel == RDA_BT_R11_5875_ENUM)
    {
          rdabt_5872_baudrate_flow_ctrl();
    }
    else
#elif defined(__RDA_CHIP_R11_5872P__) 
    if(rdabt_chip_sel == RDA_BT_R11_5872P_ENUM)
    {
          rdabt_5872p_baudrate_flow_ctrl();
    }
    else
#endif
    {
#if 1   
       kal_uint16 num_send;	      
	sxr_Sleep(500);
	//DEVICE *pDev=&Uart_Dev[BT_UART_PORT];
   	rda5868_uart_configure(921600/*460800*//*230400*/,FALSE);
    //sxr_Sleep(500);
	rda5868_uart_tx(rdabt_wakeup_data,sizeof(rdabt_wakeup_data),&num_send);
	//sxr_Sleep(16384);
	//pDev->uartCfg.afc = HAL_UART_AFC_MODE_RX_TRIG_16;
	rda5868_uart_tx(rdabt_hard_flow_ctrl,sizeof(rdabt_hard_flow_ctrl),&num_send); 
	IsFlowCtrlStart=TRUE;
   	sxr_Sleep(500);
	rda5868_uart_configure(921600/*460800*//*230400*/,IsFlowCtrlStart);
#else

	       kal_uint16 num_send;	      
	sxr_Sleep(500);
	//DEVICE *pDev=&Uart_Dev[BT_UART_PORT];
   //	rda5868_uart_configure(921600/*460800*//*230400*/,FALSE);
    //sxr_Sleep(500);
	rda5868_uart_tx(rdabt_wakeup_data,sizeof(rdabt_wakeup_data),&num_send);
	//sxr_Sleep(16384);
	//pDev->uartCfg.afc = HAL_UART_AFC_MODE_RX_TRIG_16;
	rda5868_uart_tx(rdabt_hard_flow_ctrl,sizeof(rdabt_hard_flow_ctrl),&num_send); 
	IsFlowCtrlStart=TRUE;
   	sxr_Sleep(500);
//	rda5868_uart_configure(921600/*460800*//*230400*/,IsFlowCtrlStart);
#endif
    }
}

#endif




