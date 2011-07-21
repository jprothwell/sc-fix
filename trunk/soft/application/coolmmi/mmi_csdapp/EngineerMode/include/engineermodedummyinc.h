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
 * EngineerModeDummyInc.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is a dummy functions for MNT
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

#ifndef ENGINEER_MODE_DUMMY_INC_H
#define ENGINEER_MODE_DUMMY_INC_H
 
#include "mmi_features.h" 
//CSD End

#if (defined(__MMI_ENGINEER_MODE__) || defined(__MMI_FACTORY_MODE__) )

#if (defined(MMI_ON_WIN32) || defined(WIN32) )

typedef unsigned char   kal_uint8;
typedef unsigned short int      kal_uint16;
typedef unsigned int            kal_uint32;


#define GPIO_INVAILD 0x0
#define GPIO_VAILD 0x1
#define GPO_VAILD 0x2
#define GPIO_LABELID_MAX 5
#define EINT_LABELID_MAX 5
#define ADC_LABELID_MAX  5
#define PWM_MAX_LEVEL 5
#define LCD_CONTRAST_MAX_LEVEL 15
#define LCD_PARAM_MAX_LEVEL 5
#define BATTERY_MAX_LEVEL 10



typedef struct _GPIO_MAP_ENTRY {
   unsigned char vaild;
   unsigned char port;
   const unsigned char *netname;
   const unsigned char *netname2;
} GPIO_MAP_ENTRY;

typedef enum {
      CUSTOM_EM_UART_PORT1=0,
      CUSTOM_EM_UART_PORT2,
      CUSTOM_EM_UART_PORT3,
      CUSTOM_EM_UART_MAX_PORT
} CUSTOM_EM_PORT_TYPE;


extern unsigned char netname[][16];

extern GPIO_MAP_ENTRY gpio_map_tbl[];

GPIO_MAP_ENTRY eint_map_tbl[];

GPIO_MAP_ENTRY adc_map_tbl[];

kal_uint16 PWM1_Level_Info[PWM_MAX_LEVEL][2];
kal_uint16 PWM2_Level_Info[PWM_MAX_LEVEL][2];
kal_uint16 Alter_Level_Info[PWM_MAX_LEVEL][2];

kal_uint8   MainLCD_Contrast_Level_Info[LCD_CONTRAST_MAX_LEVEL];
kal_uint8   MainLCD_Bias_Level_Info[LCD_PARAM_MAX_LEVEL] ;
kal_uint8   MainLCD_Linerate_Level_Info[LCD_PARAM_MAX_LEVEL] ;
kal_uint8   MainLCD_Temp_Level_Info[LCD_PARAM_MAX_LEVEL];
   
kal_uint8   SubLCD_Contrast_Level_Info[LCD_CONTRAST_MAX_LEVEL] ;
kal_uint8   SubLCD_Bias_Level_Info[LCD_PARAM_MAX_LEVEL];
kal_uint8   SubLCD_Linerate_Level_Info[LCD_PARAM_MAX_LEVEL];
kal_uint8   SubLCD_Temp_Level_Info[LCD_PARAM_MAX_LEVEL];

kal_uint32   Battery_Level_Info[BATTERY_MAX_LEVEL] ;
extern kal_uint8 INT_MemoryDumpFlag;


#endif /* (defined(MMI_ON_WIN32) || defined(WIN32) ) */

#endif /* (defined(__MMI_ENGINEER_MODE__) || defined(__MMI_FACTORY_MODE__) ) */

#endif //ENGINEER_MODE_DUMMY_INC_H
