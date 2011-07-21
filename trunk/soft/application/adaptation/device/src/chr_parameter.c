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
 *    chr_parameter.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is defined for charging parameters.
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
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#include "drv_comm.h"
#include "adc.h"
#include "bmt.h"
//#include "batvalue.h"
#include "bmt.h"
#include "gpio_sw.h"

#define _ADC_DETECT_USB_CHARGER_
//#define _GPIO_DETECT_USB_CHARGER_

/*GPIO control*/
bmt_customized_struct  bmt_custom_chr_def = 
{ 
   12,/*GPIO_CHRCTRL*/
    6,/*GPIO_BATDET*/
   15,/*GPIO_VIBRATOR*/

/*charing parameters*/
/*Check Phy parameters,Maybe changed*/
   1100000,/*Typical_LI_BATTYPE*/
   1100000,/*Typical_NI_BATTYPE*/
   1000000,/*ICHARGE_ON_HIGH*/
   20000,/*ICHARGE_ON_LOW*/
   1000000,/*ICHARGE_OFF_HIGH*/  //??50000
   4050000,/*V_FAST2TOPOFF_THRES*/
   2685160,/*BATTMP_MINUS_40C*/
   2264068,/*BATTMP_MINUS_15C*/
   429766,/*BATTMP_45C*/ 
   4500000,/*MAX_VBAT_LI*/ 
   5500000,/*MAX_VBAT_NI*/ 
   3400000,/*V_PRE2FAST_THRES*/ 
   250000,/*I_TOPOFF2FAST_THRES*/  /*250ma,TOPOFF->FAST*/
   120000,/*I_TOPOFF2FULL_THRES*/   /*120ma,TOPOFF->BATFULL*/
   4050000,/*V_FULL2FAST_THRES*/  /*BATFULL->FAST*/
   -100000000,/*V_TEMP_FAST2FULL_THRES_NI*/   /*50oC,FAST->BATFULL*/
   4050000,/*V_FULL2FAST_THRES_NI*/ 
   600000,/*FAST_ICHARGE_HIGHLEVEL*/  /*600ma,for table search*/
   400000,/*FAST_ICHARGE_LOWLEVEL*/   /*400ma,for table search*/
   4050000,/*V_PROTECT_HIGH_LI*/
   3800000,/*V_PROTECT_LOW_LI*/
   6500000,/*VCHARGER_HIGH*/
   0,/*VCHARGER_LOW*/
/*Time delay*/
/* PRE CHARGE ,search table*/
/*TON = 3s,TOFF=2s*/
   3,/*PRE_TON*/
   2,/*PRE_TOFF*/

/* FAST CHARGE ,search table*/
/*TON = 3s,TOFF=0s*/
   3,/*TOPOFF_TON*/
   0,/*TOPOFF_TOFF*/

   6,/*BATFULL_TON_LI*/   /*unit : second*/
   0,/*BATFULL_TOFF_LI*/
   1,/*BATFULL_TON_NI*/   /*unit : second*/
   9,/*BATFULL_TOFF_NI*/
   6,/*BATFULL_TOFF*/
   10,/*BATHOLD_OFF*/

   3,/*ADC_ISENSE_RESISTANCE_FACTOR*/         /*1/0.4*/
   24, /*bmt_measure_discard_time*/ /*24 ticks*/

   //ADC_CALIDATA adc_cali_param 
   {
      {
      5423,
      5423,
      5423,
      5382,
      5423
      },
      {
      (-17010),
      (-17010),
      (-17010),
      (34651),
      (-17010)
      }
   },
   
   /*ratio = adc_volt_factor/100*/
   //const kal_uint16 adc_volt_factor[ADC_MAX_CHANNEL] 
   {
      100,
       50,
       50,
      100,
       50

   },
   
   //const kal_uint8 TONOFFTABLE[6][2] = 
   {
      {7,1},
      {8,1},
      {9,1},
      {2,1},/*talk */
      {2,1},/*talk */
      {2,1}/*talk */
   },
   
   //const kal_int32 CurrOffset[3] =
   {
      0, //100ma
      0,
      0
   },
   KAL_TRUE, /* enable checking temperature while charging */
   KAL_TRUE /* enable checking charging voltage while charging */	   
};

bmt_customized_struct *bmt_get_customized_chr_para(void)         
{
   return (&bmt_custom_chr_def);
}   
/*voltage to temperature*/
#define T1  0*1000000   /*0C*/
#define T2  25*1000000  /*25C*/
#define T3  50*1000000  /*50C*/

#define NTCR1 26490.0   /*resistor at 0C*/
#define NTCR2 10000.0   /*resistor at 25C*/
#define NTCR3 4171.0    /*resistor at 50C*/

#define PUR 24000.0      /*orginal registor*/
#define AVDD 2800000.0
#define SCALE 10.0

#define V1 (AVDD*(NTCR1/(NTCR1+PUR))) /*voltage at 0C*/
#define V2 (AVDD*(NTCR2/(NTCR2+PUR))) /*voltage at 25C*/
#define V3 (AVDD*(NTCR3/(NTCR3+PUR))) /*voltage at 50C*/

#define M12 (((T1-T2)*SCALE)/(V1-V2))  /*T1T2's slope */
#define M23 (((T2-T3)*SCALE)/(V2-V3))  /*T2T3's slppe*/

const kal_int32 TEMPM12  = M12;      /*T1T2's slope */
const kal_int32 TEMPM23  = M23;      /*T2T3's slppe*/

const kal_int32 TEMPV2 = V2;         /*boundary*/
const kal_int32 TEMPSCALE = SCALE;

const kal_int32 TEMPA12  = T1-M12/SCALE*V1;  /*T1T2's offset*/
const kal_int32 TEMPA23  = T2-M23/SCALE*V2;  /*T2T3's offset*/

VolToTempStruct  bmt_custom_volTotemp_def = { 
        T1-M12/SCALE*V1,/*T1T2's offset*/
        T2-M23/SCALE*V2,/*T2T3's offset*/
        M12,/*T1T2's slope */
        M23,/*T2T3's slppe*/
        SCALE
};        
         
VolToTempStruct *bmt_get_customized_volTOtemp(void)         
{
   return (&bmt_custom_volTotemp_def);
}   

/*usb charging*/
#if defined(__USB_ENABLE__)
const kal_bool enable_usb_charge = KAL_TRUE;
#else
const kal_bool enable_usb_charge = KAL_FALSE;
#endif

kal_bool bmt_support_usb_charge(void)
{
   return (enable_usb_charge);
}   
/*************************************************************************
* FUNCTION
*	bmt_charger_action
*
* DESCRIPTION
*	This function is to do something whenever AC is plug-in
*
* PARAMETERS
*	AC plug in or plug out
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void bmt_charger_action(kal_bool in)
{
   kal_uint16 gpio_pin=0xff;
      
   if(gpio_pin!=0xff)
   { 
      GPIO_ModeSetup(gpio_pin, 0);
      GPIO_InitIO(OUTPUT, gpio_pin);        
      if(in==KAL_TRUE)
         GPIO_WriteIO(1, gpio_pin);
      else
         GPIO_WriteIO(0, gpio_pin);   
   }      
}   


#if defined(__CHARGER_USB_DETECT_WIHT_ONE_EINT__)
/*************************************************************************
* FUNCTION
*	bmt_get_chr_usb_detect_volt
*
* DESCRIPTION
*	This function is to get the voltage of USB threshold
*
* PARAMETERS
*	None
*
* RETURNS
*	USB voltage
*
* GLOBALS AFFECTED
*
*************************************************************************/
// usb is plugged in if  < 1.4V, otherwise it is charger.
/* <SEML SCN="SCN-MXP-00006" AUTHOR="jonathanchen" DATE="1/12/2005"> */
#ifdef __A2_HARDWARE__
static const kal_int32	chr_usb_detect_volt = 2000000;  
#else
static const kal_int32	chr_usb_detect_volt = 2000000;//1400000; ///jandy modified for milan 
#endif
/* </SEML> // SCN="SCN-MXP-00006" */

kal_int32 bmt_get_chr_usb_detect_volt(void)
{
   return (kal_int32)(chr_usb_detect_volt);
}   

/*************************************************************************
* FUNCTION
*	bmt_check_ac_usb
*
* DESCRIPTION
*	This function is to tell USB and AC
*
* PARAMETERS
*	measurement voltage
*
* RETURNS
*  AC or USB
*
* GLOBALS AFFECTED
*
*************************************************************************/
charger_usb_present_enum bmt_check_ac_usb(kal_int32 vol)
{
   kal_uint16 gpio_pin;
   kal_uint8  gpio_state;

   #ifdef _ADC_DETECT_USB_CHARGER_
   // use ADC to get voltage value (Analog method)
   /*use voltage to tell AC/USB*/
   if(vol<chr_usb_detect_volt)      
      return USB_PRESENT;
   else 
      return CHARGER_PRESENT;  
   #elif defined(_GPIO_DETECT_USB_CHARGER_) 
   /* 
        Kerwin:
        use GPIO state(high/low) to differentiate voltage value,
        because for digital(GPIO is digital due to 0/1),
        GPIO=1, if voltage > V
        GPIO=0, if voltage < V
        use V to differentiate USB or CHARGER,
        the following gpio_pin should be customized according to physical layout
   */
   /*use GPIO to tell AC/USB*/
   gpio_pin=1;/*GPIO port number*/
   GPIO_ModeSetup(gpio_pin, 0);
   GPIO_InitIO(INPUT, gpio_pin);
   gpio_state=GPIO_ReadIO(gpio_pin);
   if(gpio_state)      
      return USB_PRESENT;
   else 
      return CHARGER_PRESENT;        
   #endif   
                
}   

#endif // __CHARGER_USB_DETECT_WIHT_ONE_EINT__#endif //__USB_ENABLE__#endif //__USB_ENABLE__
