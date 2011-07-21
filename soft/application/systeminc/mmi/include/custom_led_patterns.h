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
 *   custom_led_patterns.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   The file contains definition of custom component module configuration
 *   variables, and routines handle for equipment device.
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _UEM_LED_PATTERNS_H
#define _UEM_LED_PATTERNS_H

#define MAX_LED_GPIO	      3 // 3 LED lights
#define MAX_LCD_BACKLIGHT  1 // 1 backlight
#define MAX_VIBRATOR       1 // 1 vibrator

typedef enum {
 /* LED status pattern */
 LED_PATTERN_BEGIN,

 LED_POWER_ON = LED_PATTERN_BEGIN,
 LED_POWER_OFF,
 LED_BATTERY_WARNING,
 LED_BATTERY_LOW,
 LED_Connect900,
 LED_Connect1800,
 LED_OnCharging,
 LED_FullCharging,
 LED_MessageIn,
 LED_MessageRemind,
 LED_Caller1,
 LED_Caller2,
 LED_Caller3,
 LED_Caller4,
 LED_Caller5,
 LED_Caller6,
 LED_LimitService,
 LED_NoService,
 LED_Connect1900,
 LED_Roaming,
 LED_PATTERN_END = 50,

 /* lcd backlight pattern */
 LCD_PATTERN_BEGIN,
 LCD_PATTERN_1 = LCD_PATTERN_BEGIN,
 LCD_PATTERN_2,
 LCD_PATTERN_END = 100,

 /* vibrator pattern */
 VIB_PATTERN_BEGIN,
 VIB_PATTERN_1 =VIB_PATTERN_BEGIN,
 VIB_PATTERN_2,
 VIB_PATTERN_3,   /* Incomming SMS */
 VIB_PATTERN_END = 150,

 MAX_PATTERN_ID = VIB_PATTERN_END

}PATTERN_ID ;

typedef enum{
   PLAY_STOP,
   PLAY_REPEAT,
   PLAY_ONCE,
   PLAY_DUR,
   MAX_PATTERN_TYPE = PLAY_DUR
} PATTERN_TYPE;

typedef struct {
 kal_uint8     gpio_control[MAX_LED_GPIO];
 kal_uint16	   duration_time;  
} LED_PATTERN_STRUCT;

typedef struct {
 kal_uint8     gpio_control[MAX_LCD_BACKLIGHT];
 kal_uint16	   duration_time;  
} LCD_PATTERN_STRUCT;

typedef struct {
 kal_uint8     gpio_control[MAX_VIBRATOR];
 kal_uint16	   duration_time;  
} VIB_PATTERN_STRUCT;

extern const LCD_PATTERN_STRUCT *custom_get_lcd_patterns(PATTERN_ID lcd_pattern_id);
extern const LED_PATTERN_STRUCT *custom_get_led_patterns(PATTERN_ID led_pattern_id);
extern const VIB_PATTERN_STRUCT *custom_get_vib_patterns(PATTERN_ID vib_pattern_id);
extern kal_bool uem_play_pattern(PATTERN_ID pattern_id, PATTERN_TYPE pattern_type);
extern const kal_uint8 custom_get_led_label( const kal_uint8 id);
extern const kal_uint8 custom_get_lcd_label( const kal_uint8 id);
extern const kal_uint8 custom_get_vib_label( const kal_uint8 id);

#endif
