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

#ifdef WIN32

#include "fat_fs.h"

#include "stack_common.h"
#include "stack_msgs.h"

#include "lcd_sw.h"

void PWM1_Configure(kal_uint16 freq, kal_uint8 duty){}
void PWM1_Start(void){}
void PWM1_Stop(void){}
void PWM2_Configure(kal_uint16 freq, kal_uint8 duty){}
void PWM2_Start(void){}
void PWM2_Stop(void){}
void Alter_Configure(kal_uint16 freq, kal_uint8 duty){}
void Alter_Start(void){}
void Alter_Stop(void){}

//void LCD_Init_MNT(kal_uint32 bkground, void **buf_addr){};
void LCD_PWRON_MNT(kal_bool on){};
void LCD_SetContrast_MNT(kal_uint8 voltage){};
void LCD_ON_MNT(kal_bool on){};
void LCD_BlockWrite_MNT(kal_uint16 startx,kal_uint16 starty,kal_uint16 endx,kal_uint16 endy){};
void LCD_Size_MNT(kal_uint16 *out_LCD_width,kal_uint16 *out_LCD_height){};
kal_uint8 LCD_GetParm_MNT(lcd_func_type type){ return 3; }
void LCD_SetBias_MNT(kal_uint8 *bias){};
void LCD_Contrast_MNT(kal_uint8 *contrast){};
void LCD_LineRate_MNT(kal_uint8 *linerate){};
void LCD_Temp_Compensate_MNT(kal_uint8 *compensate){};

/*
LCD_Funcs_DI LCD_func_MNT = {
   NULL,
   LCD_PWRON_MNT,
   LCD_SetContrast_MNT,
   LCD_ON_MNT,
   LCD_BlockWrite_MNT,
   LCD_Size_MNT,

   LCD_GetParm_MNT,
   LCD_SetBias_MNT,
   LCD_Contrast_MNT,
   LCD_LineRate_MNT,
   LCD_Temp_Compensate_MNT
};


#ifdef DUAL_LCD
LCD_Funcs_DI   *SubLCD;
#endif

void custom_em_LCD_FunConfig(void)
{
   MainLCD = &LCD_func_MNT;
#ifdef DUAL_LCD
   SubLCD = &LCD_func_MNT;
#endif
}
*/

#endif /* WIN32 */


