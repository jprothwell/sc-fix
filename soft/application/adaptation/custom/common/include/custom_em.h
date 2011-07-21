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
 *   custom_em.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file provides the custom engineer mode declarations
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

#ifdef  __FS_ASCII__
typedef kal_char VTCHAR;
#else
typedef unsigned short  VTCHAR; 
#endif

#define GPIO_INVAILD   0x0
#define GPIO_VAILD     0x01
#define GPO_VAILD        0x02
#define MAX_NETNAME_TEXT 16

#define CUSTOM_EM_MAX_PATH_LENGTH       32
#define CUSTOM_EM_MAX_LCD_PARAM_NUM  5

#define CUSTOM_EM_PARAM_PATH           "C:\\CUSTOM_EM"
#define CUSTOM_EM_PARAM_FILE            "EM_PARAM.DAT"

#define MAKE_CUSTOM_EM_PARAM_PATHNAME( buf ) \
{												\
	AnsiiToUnicodeString((S8*)buf, (S8*)CUSTOM_EM_PARAM_PATH);			\
}


#define MAKE_CUSTOM_EM_PARAM_FILENAME( buf ) \
{												\
	char buf_tmp[64]={0x0};								\
	sprintf(buf_tmp, "%s\\%s", (S8*)CUSTOM_EM_PARAM_PATH,(S8*)CUSTOM_EM_PARAM_FILE);\
	AnsiiToUnicodeString((S8*)buf, (S8*)buf_tmp);			\
}
                              
#define CUSTOM_EM_GET_FULL_PATH_LEN \
   (strlen(CUSTOM_EM_PARAM_PATH) + strlen(CUSTOM_EM_PARAM_FILE) + 1)

typedef enum {
   CUSTOM_EM_LCD_MAIN,
   CUSTOM_EM_LCD_SUB
} CUSTOM_EM_LCD_TYPE;

typedef enum {
   CUSTOM_EM_LCD_FUNC_BIAS,
   CUSTOM_EM_LCD_FUNC_CONSTRAST,
   CUSTOM_EM_LCD_FUNC_LINERATE,
   CUSTOM_EM_LCD_FUNC_TEMPCOM
} CUSTOM_EM_LCD_FUNC_TYPE;


typedef enum {
   CUSTOM_EM_PWM_LEVEL_1=0x0,
   CUSTOM_EM_PWM_LEVEL_2,
   CUSTOM_EM_PWM_LEVEL_3,
   CUSTOM_EM_PWM_LEVEL_4,
   CUSTOM_EM_PWM_LEVEL_5,
   CUSTOM_EM_PWM_LEVEL_MAX
} CUSTOM_EM_PWM_LEVEL;


typedef enum {
   CUSTOM_EM_PWM_TYPE_1=0x0,
   CUSTOM_EM_PWM_TYPE_2,
   CUSTOM_EM_PWM_TYPE_3,
   CUSTOM_EM_PWM_TYPE_MAX
} CUSTOM_EM_PWM_TYPE;

typedef enum {
      CUSTOM_EM_UART_PORT1=0,
      CUSTOM_EM_UART_PORT2,
      CUSTOM_EM_UART_PORT3,
      CUSTOM_EM_UART_MAX_PORT
} CUSTOM_EM_PORT_TYPE;

typedef struct {
   kal_uint8 bias_idx;
   kal_uint8 contrast_idx;
   kal_uint8 linerate_idx;
   kal_uint8 temp_compensate_idx;
   kal_uint8 bias[CUSTOM_EM_MAX_LCD_PARAM_NUM];
   kal_uint8 contrast[CUSTOM_EM_MAX_LCD_PARAM_NUM];
   kal_uint8 linerate[CUSTOM_EM_MAX_LCD_PARAM_NUM];
   kal_uint8 temp_compensate[CUSTOM_EM_MAX_LCD_PARAM_NUM];  
} custom_em_lcd_param_set_struct;

typedef struct {
   kal_uint32 freq[CUSTOM_EM_PWM_TYPE_MAX];
   kal_uint32 duty_cycle[CUSTOM_EM_PWM_TYPE_MAX];
} custom_em_pwm_param_set_struct;


typedef struct {

   custom_em_lcd_param_set_struct custom_em_main_lcd_param;          /* Main LCD */
   custom_em_lcd_param_set_struct custom_em_sub_lcd_param;           /* Sub LCD */
   custom_em_pwm_param_set_struct custom_em_pwm_param;             /* PWM Current Setting */
   custom_em_pwm_param_set_struct custom_em_pwm_level_param[CUSTOM_EM_PWM_LEVEL_MAX];             /* PWM Level */

} custom_em_param_context_struct;



extern kal_bool custom_em_init( kal_bool is_reset );
extern void custom_em_lcd_get_value(custom_em_lcd_param_set_struct *lcd_param, CUSTOM_EM_LCD_TYPE type);
extern void custom_em_lcd_set_value(custom_em_lcd_param_set_struct *lcd_param, CUSTOM_EM_LCD_TYPE type);
extern kal_uint8 custom_em_lcd_get_param_number(CUSTOM_EM_LCD_TYPE type, CUSTOM_EM_LCD_FUNC_TYPE func);
extern void custom_em_lcd_exe_function(CUSTOM_EM_LCD_TYPE type, CUSTOM_EM_LCD_FUNC_TYPE func, kal_uint8 *param  );
extern kal_bool custom_em_pwm_get_level_value(CUSTOM_EM_PWM_TYPE type, CUSTOM_EM_PWM_LEVEL level, kal_uint16 *freq, kal_uint8 *duty_cycle );
extern kal_bool custom_em_pwm_set_level_value(CUSTOM_EM_PWM_TYPE type, CUSTOM_EM_PWM_LEVEL level, kal_uint16 freq, kal_uint8 duty_cycle );
extern void custom_em_pwm_get_value(kal_uint16 *freq, kal_uint8 *duty_cycle );
extern void custom_em_pwm_set_value(kal_uint16 *freq, kal_uint8 *duty_cycle );
extern void custom_em_pwm_start(CUSTOM_EM_PWM_TYPE type, kal_uint32 freq, kal_uint32 duty );
extern void custom_em_pwm_stop(CUSTOM_EM_PWM_TYPE type);
extern kal_uint8 custom_em_pwm_get_hw_current_level(CUSTOM_EM_PWM_TYPE type);
extern void custom_em_pwm_set_level(CUSTOM_EM_PWM_TYPE type, kal_uint8 level);
extern void custom_em_gpio_start(kal_uint8 port);
extern void custom_em_gpio_stop(kal_uint8 port);
extern void custom_em_gpo_start(kal_uint8 port);
extern void custom_em_gpo_stop(kal_uint8 port);
extern void custom_em_gpo_writeio(kal_int8 data, kal_int8 port);
extern void custom_em_pwn_cycle(kal_bool is_on);
extern void custom_em_rtc_get_time(kal_uint16* year,kal_uint8* mon,kal_uint8* day, kal_uint8* hour,kal_uint8* min, kal_uint8* sec);
extern void custom_em_set_mode(kal_uint8 mode);
extern void custom_em_set_gain(kal_uint8 type, kal_uint8 gain);
extern void custom_em_set_loopback(kal_bool on);
extern kal_bool custom_em_get_melody_version(kal_char *mel_ver);
extern kal_bool custom_em_melody_key_shift(kal_int32 level);
extern kal_bool custom_em_get_uart_detection_status( CUSTOM_EM_PORT_TYPE port );
extern void custom_em_set_uart_detection_status( CUSTOM_EM_PORT_TYPE port, kal_bool enable );


