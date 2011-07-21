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
 * EngineerModeDummySrc.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is dummy functions for MNT
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

 
#include "mmi_features.h" 
//CSD End

#if (defined(__MMI_ENGINEER_MODE__) || defined(__MMI_FACTORY_MODE__) )

#include "engineermodedummyinc.h"


/*************************************
 * Dummy Values for PC simulator only
 *************************************/
#ifndef MMI_ON_HARDWARE_P

kal_uint8 INT_MemoryDumpFlag = 0;

unsigned char netname[][16] = {
   "TEST#0",
   "TEST#1",
   "TEST#2",
   "TEST#3",
   "TEST#4",
   "TEST#5",
};

GPIO_MAP_ENTRY gpio_map_tbl[] = {
/*GPIO_LABELID_0 */      {GPIO_VAILD, 0, netname[0], 0 },
/*GPIO_LABELID_1 */      {GPIO_VAILD, 1, netname[1], 0 },
/*GPIO_LABELID_2 */      {GPIO_VAILD, 2, netname[2], 0 },
/*GPIO_LABELID_3 */      {GPIO_VAILD, 3, netname[3], 0 },
/*GPIO_LABELID_4 */      {GPIO_INVAILD, 4, netname[4], 0 },
/*GPIO_LABELID_5 */      {GPIO_INVAILD, 5, netname[5], 0 }
};


GPIO_MAP_ENTRY eint_map_tbl[] = {
/*GPIO_LABELID_0 */      {GPIO_VAILD, 0, netname[0], 0 },
/*GPIO_LABELID_1 */      {GPIO_VAILD, 1, netname[1], 0 },
/*GPIO_LABELID_2 */      {GPIO_VAILD, 2, netname[2], 0 },
/*GPIO_LABELID_3 */      {GPIO_VAILD, 3, netname[3], 0 },
/*GPIO_LABELID_4 */      {GPIO_INVAILD, 4, netname[4], 0 },
/*GPIO_LABELID_5 */      {GPIO_INVAILD, 5, netname[5], 0 }
};

GPIO_MAP_ENTRY adc_map_tbl[] = {
/*GPIO_LABELID_0 */      {GPIO_VAILD, 0, netname[0], 0 },
/*GPIO_LABELID_1 */      {GPIO_VAILD, 1, netname[1], 0 },
/*GPIO_LABELID_2 */      {GPIO_VAILD, 2, netname[2], 0 },
/*GPIO_LABELID_3 */      {GPIO_VAILD, 3, netname[3], 0 },
/*GPIO_LABELID_4 */      {GPIO_INVAILD, 4, netname[4], 0 },
/*GPIO_LABELID_5 */      {GPIO_INVAILD, 5, netname[5], 0 }
};


#define PWM_MAX_LEVEL 5
#define LCD_CONTRAST_MAX_LEVEL 15
#define LCD_PARAM_MAX_LEVEL 5
#define BATTERY_MAX_LEVEL 10


kal_uint16 PWM1_Level_Info[PWM_MAX_LEVEL][2] = {                  
                  64, 20,  /* PWM 1  Level 1 */
                  64, 40,  /* PWM 1  Level 2*/
                  64, 60,  /* PWM 1  Level 3*/
                  64, 80,  /* PWM 1  Level 4*/
                  64, 100, /* PWM 1 Level 5 */
};
kal_uint16 PWM2_Level_Info[PWM_MAX_LEVEL][2] = {
                  64, 20,  /* PWM 2  Level 1 */
                  64, 40,  /* PWM 2  Level 2*/
                  64, 60,  /* PWM 2  Level 3*/
                  64, 80,  /* PWM 2  Level 4*/
                  64, 100, /* PWM 2 Level 5 */
};
kal_uint16 Alter_Level_Info[PWM_MAX_LEVEL][2] = {
                  64, 20,  /* PWM 3  Level 1 */
                  64, 40,  /* PWM 3  Level 2*/
                  64, 60,  /* PWM 3  Level 3*/
                  64, 80,  /* PWM 3  Level 4*/
                  64, 100, /* PWM 3 Level 5 */
};

kal_uint8   MainLCD_Contrast_Level_Info[LCD_CONTRAST_MAX_LEVEL] = {
                   /* Main LCD contrast level 1 ~ 15 */
                  120, 122, 124, 126, 128, 130, 132, 134, 136, 138, 
                  140, 142, 144, 146, 148,    
};
kal_uint8   MainLCD_Bias_Level_Info[LCD_PARAM_MAX_LEVEL] = {
                   /* Main LCD Bias Param (Reserved) */
                  0, 0, 0, 0, 0,    
};
kal_uint8   MainLCD_Linerate_Level_Info[LCD_PARAM_MAX_LEVEL] = {
                   /* Main LCD Linerate Param (Reserved) */
                  0, 0, 0, 0, 0,    
};
kal_uint8   MainLCD_Temp_Level_Info[LCD_PARAM_MAX_LEVEL] = {
                   /* Main LCD Temperature Param (Reserved) */
                  0, 0, 0, 0, 0,    
};
   
kal_uint8   SubLCD_Contrast_Level_Info[LCD_CONTRAST_MAX_LEVEL] = {
                   /* Sub LCD contrast level 1 ~ 15 */
                  20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 
                  40, 42, 44, 46, 48,    
};
kal_uint8   SubLCD_Bias_Level_Info[LCD_PARAM_MAX_LEVEL] = {
                   /* Sub LCD Bias Param (Reserved) */
                  0, 0, 0, 0, 0,    
};
kal_uint8   SubLCD_Linerate_Level_Info[LCD_PARAM_MAX_LEVEL] = {
                   /* Sub LCD Linerate Param (Reserved) */
                  0, 0, 0, 0, 0,    
};
kal_uint8   SubLCD_Temp_Level_Info[LCD_PARAM_MAX_LEVEL]= {
                   /* Sub LCD Temperature Param (Reserved) */
                  0, 0, 0, 0, 0,    
};

kal_uint32   Battery_Level_Info[BATTERY_MAX_LEVEL] = {
                  /* Battery voltage Level */
                  3200000, /* Low Battery Power off */
                  3500000, /* Low Battery */
                  /* battery level 1 ~ 8 */
                  3698671, 3801801, 4003424, 9999999, 
                  9999999, 9999999, 9999999, 9999999
};

static kal_uint8 port_en[3];

kal_uint8 custom_em_get_uart_detection_status( CUSTOM_EM_PORT_TYPE port )
{
   return port_en[port];
}

void custom_em_set_uart_detection_status( CUSTOM_EM_PORT_TYPE port, kal_uint8 enable )
{
   port_en[port] = enable;
}


#endif /* !MMI_ON_HARDWARE_P */

#endif /* (defined(__MMI_ENGINEER_MODE__) || defined(__MMI_FACTORY_MODE__) ) */

