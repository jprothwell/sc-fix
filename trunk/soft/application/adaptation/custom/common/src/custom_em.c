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
 * Filename:
 * ---------
 * custom_em.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is for engineer mode.
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

#ifndef __L1_STANDALONE__

/* System */
#include "fat_fs.h"
#include "stack_common.h"
#include "stack_msgs.h"
#include "app_ltlcom.h"

/* Drv */
#include "lcd_sw.h"
//#include "pwm_sw.h"
//#include "alerter_sw.h"
#include "gpio_sw.h"
#include "rtc_sw.h"
#include "uart_sw.h"

/* Custom_EM */
#include "custom_em.h"
#include "device.h"
//#include "l1audio.h"// wangbei del 20060614
#include "resource_audio.h"

#include "lcd_if.h"

/*WUZC Modify Start For MMIFS Ver:    on 2007-1-12 15:42 */
#include "cswtype.h"
#include "filesystemdef.h"
#include "mmi_trace.h"
#include "ucs2prot.h"
//#define kal_mem_cpy(a,b,c) memcpy(a,b,c)
//#define kal_mem_set(a,b,c) memset(a,b,c)
/*WUZC Modify End  For MMIFS Ver:    on 2007-1-12 15:42 */

/* Globol variable */
kal_bool g_is_custom_em_init=KAL_FALSE;
custom_em_param_context_struct  g_em_context;
#ifdef DUAL_LCD
   extern LCD_Funcs_DI   *SubLCD;
#endif

//CSD00924 20051026:  +EIMG and +EMDY download to MS default folder name customization 
//at most 25 characters in UCS2 format for folder name, and 0x005C and 0x0000 should be given at the end 
kal_wchar CUSTOM_AT_AUDIO_DOWNLOAD_FOLDER_NAME[] = 
{0x0041,0x0075,0x0064,0x0069,0x006F,0x005C,0x0000}; //default folder = Audio
kal_wchar CUSTOM_AT_IMAGE_DOWNLOAD_FOLDER_NAME[] = 
{0x0049,0x006D,0x0061,0x0067,0x0065,0x0073,0x005C,0x0000}; //default folder = Images


#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif


//Lisen 08082005
#ifdef _LOW_COST_SINGLE_BANK_FLASH_

kal_bool custom_em_init( kal_bool is_reset )
{
   kal_uint32 Ret, len;

   kal_mem_set( &g_em_context, 0, sizeof( custom_em_param_context_struct ) );
 
   g_em_context.custom_em_main_lcd_param.bias_idx = lcd_get_parameter(MAIN_LCD,lcd_Bais_func);
   g_em_context.custom_em_main_lcd_param.contrast_idx= lcd_get_parameter(MAIN_LCD,lcd_Contrast_func);
   g_em_context.custom_em_main_lcd_param.linerate_idx= lcd_get_parameter(MAIN_LCD,lcd_LineRate_func);
   g_em_context.custom_em_main_lcd_param.temp_compensate_idx= lcd_get_parameter(MAIN_LCD,lcd_Temperature_Compensation_func);

#ifdef DUAL_LCD

   if( SubLCD && SubLCD->get_param_number )
   {
   g_em_context.custom_em_sub_lcd_param.bias_idx = SubLCD->get_param_number(lcd_Bais_func);
   g_em_context.custom_em_sub_lcd_param.contrast_idx= SubLCD->get_param_number(lcd_Contrast_func);
   g_em_context.custom_em_sub_lcd_param.linerate_idx= SubLCD->get_param_number(lcd_LineRate_func);
   g_em_context.custom_em_sub_lcd_param.temp_compensate_idx= SubLCD->get_param_number(lcd_Temperature_Compensation_func);
    }
   else
      kal_mem_set( &g_em_context.custom_em_sub_lcd_param, 0, sizeof(custom_em_lcd_param_set_struct) );

#else

   kal_mem_set( &g_em_context.custom_em_sub_lcd_param, 0, sizeof(custom_em_lcd_param_set_struct) );

#endif

   g_is_custom_em_init = KAL_TRUE;
   return KAL_TRUE;
}

kal_bool custom_em_flush_to_fs()
{
   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   return KAL_TRUE;
}

kal_bool custom_em_get_from_fs()
{
   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   return KAL_TRUE;
}


void custom_em_lcd_get_value(custom_em_lcd_param_set_struct *lcd_param, CUSTOM_EM_LCD_TYPE type)
{
   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   switch( type )
   {
      case( CUSTOM_EM_LCD_MAIN ):
         kal_mem_cpy( lcd_param, &g_em_context.custom_em_main_lcd_param, sizeof( custom_em_lcd_param_set_struct ) );
         break;
      case ( CUSTOM_EM_LCD_SUB ):
#ifdef DUAL_LCD
         kal_mem_cpy( lcd_param, &g_em_context.custom_em_sub_lcd_param, sizeof( custom_em_lcd_param_set_struct ) );
#endif
         break;
     default:
         break;
   }
}

void custom_em_lcd_set_value(custom_em_lcd_param_set_struct *lcd_param, CUSTOM_EM_LCD_TYPE type)
{
   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   switch( type )
   {
      case( CUSTOM_EM_LCD_MAIN ):
         kal_mem_cpy( &g_em_context.custom_em_main_lcd_param, lcd_param, sizeof( custom_em_lcd_param_set_struct ) );
         break;
      case ( CUSTOM_EM_LCD_SUB ):
#ifdef DUAL_LCD
         kal_mem_cpy( &g_em_context.custom_em_sub_lcd_param, lcd_param, sizeof( custom_em_lcd_param_set_struct ) );
#endif
         break;
     default:
         break;
   }
}

kal_uint8 custom_em_lcd_get_param_number(CUSTOM_EM_LCD_TYPE type, CUSTOM_EM_LCD_FUNC_TYPE func)
{
   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   switch( type )
   {
      case( CUSTOM_EM_LCD_MAIN ):
	     return  lcd_get_parameter(MAIN_LCD, func);         

      case ( CUSTOM_EM_LCD_SUB ):
#ifdef DUAL_LCD
	#if (defined(MT6218B)||defined(MT6219))
	     return  lcd_get_parameter(SUB_LCD, func);
	#else /* old platform */
            if( SubLCD && SubLCD->get_param_number )
               return SubLCD->get_param_number(func);
            else
               return 0;
      #endif           
#endif
     default:
         return 0;
   }
}

void custom_em_lcd_exe_function(CUSTOM_EM_LCD_TYPE type, CUSTOM_EM_LCD_FUNC_TYPE func, kal_uint8 *param  )
{
   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   switch( func )
    {
         case( CUSTOM_EM_LCD_FUNC_BIAS ):
            if( type == CUSTOM_EM_LCD_MAIN )
            {   
		    	lcd_set_bias(MAIN_LCD, param);        
            }
#ifdef DUAL_LCD
            else if( type == CUSTOM_EM_LCD_SUB )
            {
	#if (defined(MT6218B)||defined(MT6219))
		    lcd_set_bias(SUB_LCD, param);
	#else /* old platform */
                  if( SubLCD && SubLCD->set_bias )
                    SubLCD->set_bias( param );
       #endif           
             }
#endif
            else
               ASSERT(0);
            break;

         case( CUSTOM_EM_LCD_FUNC_CONSTRAST ):
            if( type == CUSTOM_EM_LCD_MAIN )
            {
		    	lcd_set_contrast(MAIN_LCD, param);       
            }
#ifdef DUAL_LCD
            else if( type == CUSTOM_EM_LCD_SUB )
            {
	#if (defined(MT6218B)||defined(MT6219))
		    lcd_set_contrast(SUB_LCD, param);
	#else /* old platform */
                  if( SubLCD && SubLCD->set_contrast )
                    SubLCD->set_contrast( param );
       #endif           
            }
#endif
            else
               ASSERT(0);
            break;

         case( CUSTOM_EM_LCD_FUNC_LINERATE ):
            if( type == CUSTOM_EM_LCD_MAIN )
            {
		    	lcd_set_linerate(MAIN_LCD, param);
            }
#ifdef DUAL_LCD
            else if( type == CUSTOM_EM_LCD_SUB )
            {
	#if (defined(MT6218B)||defined(MT6219))
		    lcd_set_linerate(SUB_LCD, param);
	#else /* old platform */
                  if( SubLCD && SubLCD->set_linerate )
                    SubLCD->set_linerate( param );
       #endif           
            }
#endif
            else
               ASSERT(0);
            break;

         case( CUSTOM_EM_LCD_FUNC_TEMPCOM ):
            if( type == CUSTOM_EM_LCD_MAIN )
            {
		    	lcd_set_temp_compensate(MAIN_LCD, param);         
            }
#ifdef DUAL_LCD
            else if( type == CUSTOM_EM_LCD_SUB )
            {
	#if (defined(MT6218B)||defined(MT6219))
		    lcd_set_temp_compensate(SUB_LCD, param);
	#else /* old platform */
                  if( SubLCD && SubLCD->set_temp_compensate )
                    SubLCD->set_temp_compensate( param );
       #endif           
            }
#endif
            else
               ASSERT(0);
            break;
         default:
            ASSERT(0);
            break;
    }
}

void custom_em_pwm_get_value(kal_uint16 *freq, kal_uint8 *duty_cycle )
{
   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   kal_mem_cpy( freq, g_em_context.custom_em_pwm_param.freq, sizeof( g_em_context.custom_em_pwm_param.freq) );
   kal_mem_cpy( duty_cycle, g_em_context.custom_em_pwm_param.duty_cycle, sizeof( g_em_context.custom_em_pwm_param.duty_cycle) );
}

void custom_em_pwm_set_value(kal_uint16 *freq, kal_uint8 *duty_cycle )
{
   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   kal_mem_cpy( g_em_context.custom_em_pwm_param.freq, freq, sizeof( g_em_context.custom_em_pwm_param.freq) );
   kal_mem_cpy( g_em_context.custom_em_pwm_param.duty_cycle, duty_cycle, sizeof( g_em_context.custom_em_pwm_param.duty_cycle) );
 }

kal_bool custom_em_pwm_get_level_value(CUSTOM_EM_PWM_TYPE type, CUSTOM_EM_PWM_LEVEL level, kal_uint16 *freq, kal_uint8 *duty_cycle )
{
   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   if( level >= CUSTOM_EM_PWM_LEVEL_MAX || type >= CUSTOM_EM_PWM_TYPE_MAX )
      return KAL_FALSE;
   
   *freq = g_em_context.custom_em_pwm_level_param[level].freq[type];
   *duty_cycle = g_em_context.custom_em_pwm_level_param[level].duty_cycle[type];

   return KAL_TRUE;
}


kal_bool custom_em_pwm_set_level_value(CUSTOM_EM_PWM_TYPE type, CUSTOM_EM_PWM_LEVEL level, kal_uint16 freq, kal_uint8 duty_cycle )
{
   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   if( level >= CUSTOM_EM_PWM_LEVEL_MAX || type >= CUSTOM_EM_PWM_TYPE_MAX )
      return KAL_FALSE;

   g_em_context.custom_em_pwm_level_param[level].freq[type] = freq;
   g_em_context.custom_em_pwm_level_param[level].duty_cycle[type] = duty_cycle;

   return KAL_TRUE;
   
 }


#else
//wangbei add start 20060614
//wuzc del start 20061219
#if 0
typedef int FS_HANDLE;
#define FS_READ_WRITE            0x00000000L
#define FS_READ_ONLY             0x00000100L
#define FS_OPEN_SHARED           0x00000200L
#define FS_OPEN_NO_DIR           0x00000400L
#define FS_OPEN_DIR              0x00000800L
#define FS_CREATE                0x00010000L
#define FS_CREATE_ALWAYS         0x00020000L 
#define FS_COMMITTED		         0x01000000L
#define FS_CACHE_DATA	         0x02000000L
#define FS_LAZY_DATA		         0x04000000L
#define FS_NONBLOCK_MODE			0x10000000L
#define FS_PROTECTION_MODE		   0x20000000L
int FS_CreateDir(const WCHAR *DirName)
{
            return 0;
}
#endif
//wuzc del end 20061219
void kal_wsprintf(unsigned short *outstr, char *fmt,...)
{
#if 0
csd_wsprintf(outstr,fmt);
#else

	/*WUZC Modify Start For MMIFS Ver:    on 2007-1-23 10:18 */
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char *)(outstr), fmt, ap);
	va_end(ap);	
	/*WUZC Modify End  For MMIFS Ver:    on 2007-1-23 10:18 */
#endif	
}
void PWM2_Configure(kal_uint32 freq, kal_uint8 duty)
{
}
void PWM2_Start(void)
{
}
void PWM2_Stop(void)
{
}
void PWM2_level(kal_uint8 level)
{
}
void PWM_level(kal_uint8 level)
{
}
void PWM1_Configure(kal_uint32 freq, kal_uint8 duty)
{
}
void PWM1_Start(void)
{
}
void PWM1_Stop(void)
{
}
kal_uint8 PWM2_GetCurrentLevel(void)
{
   return 1;
}
kal_uint8 PWM1_GetCurrentLevel(void)
{
   return 1;
}
void Alter_Configure(kal_uint32 freq, kal_uint8 duty)
{
}

void Alter_Start(void)
{
}

void Alter_Stop(void)
{
}

void Alter_level(kal_uint8 level)
{
}
kal_uint8 Alter_GetCurrentLevel(void)
{
   return 0;
}

void DRV_POWEROFF(void)  /*PW_KEYPRESS*/
{
}
void DRV_RESET(void)
{
}

void   L1SP_SetSidetoneVolume( kal_uint8 sidetone )
{
}
void L1SP_SetMicrophoneVolume( unsigned char mic_volume )
{
}
void L1SP_SetOutputVolume( unsigned char volume1, unsigned char volume2 )
{
}

void L1SP_SetAfeLoopback( kal_bool enable )
{
}
void L1SP_Speech_On( void )
{
}
void L1SP_Speech_Off( void )
{
}
kal_bool Get_UART_Detection_Status(UART_PORT port)
{
	return 0;
}
void UART_Detection_Enable(UART_PORT port, kal_bool enable_flag)
{
}
// wangbei add end
kal_bool custom_em_init( kal_bool is_reset )
{
   VTCHAR filename[CUSTOM_EM_MAX_PATH_LENGTH];
   FS_HANDLE hFile=0;
   kal_uint32 Ret,len;

   /* check buffer boundary */
   Ret = CUSTOM_EM_GET_FULL_PATH_LEN;
   ASSERT( CUSTOM_EM_MAX_PATH_LENGTH > Ret );

   /* Create Folder if not exist */
   MAKE_CUSTOM_EM_PARAM_PATHNAME(filename);
//   hFile=MMI_FS_Open((const UINT8  *)(filename), FS_READ_ONLY | FS_OPEN_DIR);    
 //  if( hFile == FS_FILE_NOT_FOUND ) 
   if(!MMI_FS_IsExist((const UINT8  *)(filename)))
   {
      /* Return value should be FS_FILE_NOT_FOUND (-9) only, others treat as fatal error */
      /*WUZC Modify Start For FMGR Ver:    on 2007-1-17 11:14 */
      Ret = MMI_FS_CreateDir((const UINT8*)filename);
      /*WUZC Modify End  For FMGR Ver:    on 2007-1-17 11:14 */
      ASSERT( Ret == FS_NO_ERROR );
      is_reset = KAL_TRUE;
   }
   else
   {
      is_reset = KAL_FALSE;
   }

   /* Create File if not exist */
   MAKE_CUSTOM_EM_PARAM_FILENAME(filename);
   if( is_reset == KAL_TRUE )
   {
      /* Create always, get default value */
       //kal_mem_cpy( &g_em_context, CUSTOM_EM_PARAM_DEFAULT, sizeof( custom_em_param_context_struct ) );
       kal_mem_set( &g_em_context, 0, sizeof( custom_em_param_context_struct ) );
       hFile = MMI_FS_Open((const UINT8  *)(filename), FS_CREATE_ALWAYS | FS_READ_WRITE | FS_OPEN_NO_DIR);  
       ASSERT( hFile >= FS_NO_ERROR );
   }
   else
   {
      /* Read from file */
      hFile = MMI_FS_Open((const UINT8  *) filename,  FS_READ_WRITE | FS_OPEN_NO_DIR);   
      if( hFile == FS_NO_ERROR )
      {
         //Ret = pfread(hFile, &g_em_context, sizeof(custom_em_param_context_struct), &len );
         Ret = MMI_FS_Read((INT32)hFile, ( UINT8 *)(&g_em_context), sizeof(custom_em_param_context_struct),&len);
         ASSERT( Ret == FS_NO_ERROR );
         if( Ret == 0 )   //changed from len to ret by elisa
            is_reset = KAL_TRUE;
      }
      else
      {
          //kal_mem_cpy( &g_em_context, CUSTOM_EM_PARAM_DEFAULT, sizeof( custom_em_param_context_struct ) );
          kal_mem_set( &g_em_context, 0, sizeof( custom_em_param_context_struct ) );
          hFile = MMI_FS_Open((const UINT8  *) filename,  FS_CREATE_ALWAYS | FS_READ_WRITE | FS_OPEN_NO_DIR);  
          ASSERT( hFile >= FS_NO_ERROR );
          is_reset = KAL_TRUE;
      }
   }

   g_em_context.custom_em_main_lcd_param.bias_idx = lcd_get_parameter(MAIN_LCD,lcd_Bais_func);
   g_em_context.custom_em_main_lcd_param.contrast_idx= lcd_get_parameter(MAIN_LCD,lcd_Contrast_func);
   g_em_context.custom_em_main_lcd_param.linerate_idx= lcd_get_parameter(MAIN_LCD,lcd_LineRate_func);
   g_em_context.custom_em_main_lcd_param.temp_compensate_idx= lcd_get_parameter(MAIN_LCD,lcd_Temperature_Compensation_func);

#ifdef DUAL_LCD

   if( SubLCD && SubLCD->get_param_number )
   {
   g_em_context.custom_em_sub_lcd_param.bias_idx = SubLCD->get_param_number(lcd_Bais_func);
   g_em_context.custom_em_sub_lcd_param.contrast_idx= SubLCD->get_param_number(lcd_Contrast_func);
   g_em_context.custom_em_sub_lcd_param.linerate_idx= SubLCD->get_param_number(lcd_LineRate_func);
   g_em_context.custom_em_sub_lcd_param.temp_compensate_idx= SubLCD->get_param_number(lcd_Temperature_Compensation_func);
    }
   else
      kal_mem_set( &g_em_context.custom_em_sub_lcd_param, 0, sizeof(custom_em_lcd_param_set_struct) );

#else

   kal_mem_set( &g_em_context.custom_em_sub_lcd_param, 0, sizeof(custom_em_lcd_param_set_struct) );

#endif

   if( is_reset == KAL_TRUE )
   {
       Ret = MMI_FS_Write( (INT32)hFile, ( UINT8 *)(&g_em_context), (kal_uint32)sizeof(g_em_context), &len );
       ASSERT( Ret == FS_NO_ERROR && len == sizeof(g_em_context) );
   }

   if( hFile )
      MMI_FS_Close( hFile );
 
   g_is_custom_em_init = KAL_TRUE;
   return KAL_TRUE;
}

kal_bool custom_em_flush_to_fs()
{
   VTCHAR filename[CUSTOM_EM_MAX_PATH_LENGTH];
   FS_HANDLE hFile=0;
   kal_uint32 Ret;//, len;

   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   MAKE_CUSTOM_EM_PARAM_FILENAME(filename);
   hFile = MMI_FS_Open((const UINT8  *) filename,  FS_READ_WRITE | FS_OPEN_NO_DIR);     
   ASSERT( hFile > FS_NO_ERROR );

   Ret = pfwrite( (INT32)hFile, ( UINT8 *)(&g_em_context), (kal_uint32)sizeof(custom_em_param_context_struct), &len );
   ASSERT( Ret == FS_NO_ERROR && len == sizeof(g_em_context) );

   pfclose( hFile );

   return KAL_TRUE;
}

kal_bool custom_em_get_from_fs()
{
   VTCHAR filename[CUSTOM_EM_MAX_PATH_LENGTH];
   FS_HANDLE hFile=0;
   kal_uint32 Ret;//, len;

   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   MAKE_CUSTOM_EM_PARAM_FILENAME(filename);
   hFile = pfopen((const UINT8  *) filename,  FS_READ_WRITE | FS_OPEN_NO_DIR ,0);    
   ASSERT( hFile > FS_NO_ERROR );

  // Ret = pfread( hFile, &g_em_context, (kal_uint32)sizeof(custom_em_param_context_struct), &len );
     Ret = pfread( (INT32)hFile, ( UINT8 *)(&g_em_context), (kal_uint32)sizeof(custom_em_param_context_struct));

   ASSERT( Ret == FS_NO_ERROR && len == sizeof(g_em_context) );

   pfclose( hFile );

   return KAL_TRUE;
}


void custom_em_lcd_get_value(custom_em_lcd_param_set_struct *lcd_param, CUSTOM_EM_LCD_TYPE type)
{
   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   switch( type )
   {
      case( CUSTOM_EM_LCD_MAIN ):
         kal_mem_cpy( lcd_param, &g_em_context.custom_em_main_lcd_param, sizeof( custom_em_lcd_param_set_struct ) );
         break;
      case ( CUSTOM_EM_LCD_SUB ):
#ifdef DUAL_LCD
         kal_mem_cpy( lcd_param, &g_em_context.custom_em_sub_lcd_param, sizeof( custom_em_lcd_param_set_struct ) );
#endif
         break;
     default:
         break;
   }
}

void custom_em_lcd_set_value(custom_em_lcd_param_set_struct *lcd_param, CUSTOM_EM_LCD_TYPE type)
{
   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   switch( type )
   {
      case( CUSTOM_EM_LCD_MAIN ):
         kal_mem_cpy( &g_em_context.custom_em_main_lcd_param, lcd_param, sizeof( custom_em_lcd_param_set_struct ) );
         break;
      case ( CUSTOM_EM_LCD_SUB ):
#ifdef DUAL_LCD
         kal_mem_cpy( &g_em_context.custom_em_sub_lcd_param, lcd_param, sizeof( custom_em_lcd_param_set_struct ) );
#endif
         break;
     default:
         break;
   }
   custom_em_flush_to_fs();
}

kal_uint8 custom_em_lcd_get_param_number(CUSTOM_EM_LCD_TYPE type, CUSTOM_EM_LCD_FUNC_TYPE func)
{
   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   switch( type )
   {
      case( CUSTOM_EM_LCD_MAIN ):
	     return  lcd_get_parameter(MAIN_LCD, func);  

      case ( CUSTOM_EM_LCD_SUB ):
#ifdef DUAL_LCD
	#if (defined(MT6218B)||defined(MT6219))
	     return  lcd_get_parameter(SUB_LCD, func);
	#else /* old platform */
            if( SubLCD && SubLCD->get_param_number )
               return SubLCD->get_param_number(func);
            else
               return 0;
      #endif           
#endif
     default:
         return 0;
   }
}

void custom_em_lcd_exe_function(CUSTOM_EM_LCD_TYPE type, CUSTOM_EM_LCD_FUNC_TYPE func, kal_uint8 *param  )
{
   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   switch( func )
    {
         case( CUSTOM_EM_LCD_FUNC_BIAS ):
            if( type == CUSTOM_EM_LCD_MAIN )
            {   
		    lcd_set_bias(MAIN_LCD, param);
            }
#ifdef DUAL_LCD
            else if( type == CUSTOM_EM_LCD_SUB )
            {
	#if (defined(MT6218B)||defined(MT6219))
		    lcd_set_bias(SUB_LCD, param);
	#else /* old platform */
                  if( SubLCD && SubLCD->set_bias )
                    SubLCD->set_bias( param );
       #endif           
             }
#endif
            else
               ASSERT(0);
            break;

         case( CUSTOM_EM_LCD_FUNC_CONSTRAST ):
            if( type == CUSTOM_EM_LCD_MAIN )
            {
		    	lcd_set_contrast(MAIN_LCD, param);  
            }
#ifdef DUAL_LCD
            else if( type == CUSTOM_EM_LCD_SUB )
            {
	#if (defined(MT6218B)||defined(MT6219))
		    lcd_set_contrast(SUB_LCD, param);
	#else /* old platform */
                  if( SubLCD && SubLCD->set_contrast )
                    SubLCD->set_contrast( param );
       #endif           
            }
#endif
            else
               ASSERT(0);
            break;

         case( CUSTOM_EM_LCD_FUNC_LINERATE ):
            if( type == CUSTOM_EM_LCD_MAIN )
            {
		    	lcd_set_linerate(MAIN_LCD, param);          
            }
#ifdef DUAL_LCD
            else if( type == CUSTOM_EM_LCD_SUB )
            {
	#if (defined(MT6218B)||defined(MT6219))
		    lcd_set_linerate(SUB_LCD, param);
	#else /* old platform */
                  if( SubLCD && SubLCD->set_linerate )
                    SubLCD->set_linerate( param );
       #endif           
            }
#endif
            else
               ASSERT(0);
            break;

         case( CUSTOM_EM_LCD_FUNC_TEMPCOM ):
            if( type == CUSTOM_EM_LCD_MAIN )
            {
		    	lcd_set_temp_compensate(MAIN_LCD, param);       
            }
#ifdef DUAL_LCD
            else if( type == CUSTOM_EM_LCD_SUB )
            {
	#if (defined(MT6218B)||defined(MT6219))
		    lcd_set_temp_compensate(SUB_LCD, param);
	#else /* old platform */
                  if( SubLCD && SubLCD->set_temp_compensate )
                    SubLCD->set_temp_compensate( param );
       #endif           
            }
#endif
            else
               ASSERT(0);
            break;
         default:
            ASSERT(0);
            break;
    }
}

void custom_em_pwm_get_value(kal_uint16 *freq, kal_uint8 *duty_cycle )
{
   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   kal_mem_cpy( freq, g_em_context.custom_em_pwm_param.freq, sizeof( g_em_context.custom_em_pwm_param.freq) );
   kal_mem_cpy( duty_cycle, g_em_context.custom_em_pwm_param.duty_cycle, sizeof( g_em_context.custom_em_pwm_param.duty_cycle) );
}

void custom_em_pwm_set_value(kal_uint16 *freq, kal_uint8 *duty_cycle )
{
   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   kal_mem_cpy( g_em_context.custom_em_pwm_param.freq, freq, sizeof( g_em_context.custom_em_pwm_param.freq) );
   kal_mem_cpy( g_em_context.custom_em_pwm_param.duty_cycle, duty_cycle, sizeof( g_em_context.custom_em_pwm_param.duty_cycle) );
   custom_em_flush_to_fs();
 }

kal_bool custom_em_pwm_get_level_value(CUSTOM_EM_PWM_TYPE type, CUSTOM_EM_PWM_LEVEL level, kal_uint16 *freq, kal_uint8 *duty_cycle )
{
   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   if( level >= CUSTOM_EM_PWM_LEVEL_MAX || type >= CUSTOM_EM_PWM_TYPE_MAX )
      return KAL_FALSE;
   
   *freq = g_em_context.custom_em_pwm_level_param[level].freq[type];
   *duty_cycle = g_em_context.custom_em_pwm_level_param[level].duty_cycle[type];

   return KAL_TRUE;
}


kal_bool custom_em_pwm_set_level_value(CUSTOM_EM_PWM_TYPE type, CUSTOM_EM_PWM_LEVEL level, kal_uint16 freq, kal_uint8 duty_cycle )
{
   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   if( level >= CUSTOM_EM_PWM_LEVEL_MAX || type >= CUSTOM_EM_PWM_TYPE_MAX )
      return KAL_FALSE;

   g_em_context.custom_em_pwm_level_param[level].freq[type] = freq;
   g_em_context.custom_em_pwm_level_param[level].duty_cycle[type] = duty_cycle;

   custom_em_flush_to_fs();

   return KAL_TRUE;
   
 }

#endif
void custom_em_pwm_start(CUSTOM_EM_PWM_TYPE type, kal_uint32 freq, kal_uint32 duty )
{
   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   switch( type )
   {
      case (CUSTOM_EM_PWM_TYPE_1) :
         PWM1_Configure( freq, duty );
         PWM1_Start();
         break;
      case (CUSTOM_EM_PWM_TYPE_2) :
#if ( (defined(MT6205B)) || (defined(MT6218))|| (defined(MT6218B)) )
         PWM2_Configure( freq, duty );
         PWM2_Start();
#endif
         break;
      case (CUSTOM_EM_PWM_TYPE_3) :
         Alter_Configure( freq, duty );
         Alter_Start();
         break;
      default:
         ASSERT(0);
         break;
   }
}

void custom_em_pwm_stop(CUSTOM_EM_PWM_TYPE type)
{
   if( g_is_custom_em_init != KAL_TRUE )
      custom_em_init( KAL_FALSE );

   switch( type )
   {
      case (CUSTOM_EM_PWM_TYPE_1) :
         PWM1_Stop();
         break;
      case (CUSTOM_EM_PWM_TYPE_2) :
#if ( (defined(MT6205B)) || (defined(MT6218))|| (defined(MT6218B)) )
         PWM2_Stop();
#endif
         break;
      case (CUSTOM_EM_PWM_TYPE_3) :
         Alter_Stop();
         break;
      default:
         ASSERT(0);
         break;
   }
}

extern kal_uint8 PWM1_GetCurrentLevel(void);
extern kal_uint8 PWM2_GetCurrentLevel(void);
extern kal_uint8 Alter_GetCurrentLevel(void);
extern void PWM_level(kal_uint8 level);
extern void Alter_level(kal_uint8 level);
extern void DRV_RESET(void);
extern void DRV_POWEROFF(void);
extern kal_uint8 custom_cfg_hw_aud_output_path(  kal_uint8 speaker_id );
extern kal_uint8 custom_cfg_hw_aud_input_path(  kal_uint8 mic_id );
extern kal_bool Get_UART_Detection_Status(UART_PORT port);
extern void UART_Detection_Enable(UART_PORT port, kal_bool enable_flag);

kal_uint8 custom_em_pwm_get_hw_current_level(CUSTOM_EM_PWM_TYPE type)
{
   switch( type )
   {
      case (CUSTOM_EM_PWM_TYPE_1) :
         return PWM1_GetCurrentLevel();
      case (CUSTOM_EM_PWM_TYPE_2) :
#if ( (defined(MT6205B)) || (defined(MT6218))|| (defined(MT6218B)) )
         return PWM2_GetCurrentLevel();
#else
         return 0;
#endif
      case (CUSTOM_EM_PWM_TYPE_3) :
         return Alter_GetCurrentLevel();
      default:
         ASSERT(0);
         break;
   }
   return 0;
}

void custom_em_pwm_set_level(CUSTOM_EM_PWM_TYPE type, kal_uint8 level)
{
   switch( type )
   {
      case (CUSTOM_EM_PWM_TYPE_1) :
         PWM_level(level);
         break;
      case (CUSTOM_EM_PWM_TYPE_2) :
#if ( (defined(MT6205B)) || (defined(MT6218))|| (defined(MT6218B)) )
         PWM2_level(level);
#endif
         break;
      case (CUSTOM_EM_PWM_TYPE_3) :
         Alter_level(level);
         break;
      default:
         ASSERT(0);
         break;
   }

}


void custom_em_gpio_start(kal_uint8 port)
{
      GPIO_WriteIO( 1, port );
}

void custom_em_gpio_stop(kal_uint8 port)
{
      GPIO_WriteIO( 0, port );
}

void custom_em_gpo_start(kal_uint8 port)
{
      GPO_WriteIO( 1, port );
}

void custom_em_gpo_stop(kal_uint8 port)
{
      GPO_WriteIO( 0, port );
}

void custom_em_gpo_writeio(kal_int8 data, kal_int8 port)
{
      GPO_WriteIO( data, port );
}


void custom_em_pwn_cycle(kal_bool is_on)
{
	if(is_on)
	{
		DRV_RESET();
	}
	else
	{
		DRV_POWEROFF();
		DRV_RESET();
	}
}



void custom_em_rtc_get_time(kal_uint16* year,kal_uint8* mon,kal_uint8* day, kal_uint8* hour,kal_uint8* min, kal_uint8* sec)
{
     t_rtc l_rtctime;
     RTC_GetTime( &l_rtctime );
     *sec=l_rtctime.rtc_sec;
     *min=l_rtctime.rtc_min;
     *hour=l_rtctime.rtc_hour;
     *day=l_rtctime.rtc_day;
     *mon=l_rtctime.rtc_mon;
     *year= l_rtctime.rtc_year;
}



void custom_em_test()
{
/*
   custom_em_lcd_param_set_struct aaa;
   kal_uint16 freq[3];
   kal_uint8 duty_cycle[3];

   custom_em_init(KAL_FALSE);
   custom_em_lcd_get_value( &aaa, CUSTOM_EM_LCD_MAIN );
   aaa.bias_idx = 1;
   aaa.contrast_idx = 2;
   aaa.linerate_idx = 3;
   aaa.temp_compensate_idx = 4;
   kal_mem_set( &aaa.bias[0], 0x5, 10 );
   kal_mem_set( &aaa.contrast[0], 0x6, 10 );
   kal_mem_set( &aaa.linerate[0], 0x7, 10 );
   kal_mem_set( &aaa.temp_compensate[0], 0x8, 10 );

   custom_em_lcd_set_value( &aaa, CUSTOM_EM_LCD_MAIN );

   custom_em_lcd_set_value( &aaa, CUSTOM_EM_LCD_SUB );

   kal_mem_set( &aaa, 0x0, sizeof(custom_em_lcd_param_set_struct) );
   custom_em_lcd_get_value( &aaa, CUSTOM_EM_LCD_SUB );

   custom_em_lcd_exe_function(CUSTOM_EM_LCD_MAIN, lcd_Bais_func, &aaa.bias[0] );
   custom_em_lcd_exe_function(CUSTOM_EM_LCD_MAIN, lcd_Contrast_func, &aaa.contrast[0] );
   custom_em_lcd_exe_function(CUSTOM_EM_LCD_MAIN, lcd_LineRate_func, &aaa.linerate[0] );
   custom_em_lcd_exe_function(CUSTOM_EM_LCD_MAIN, lcd_Temperature_Compensation_func, &aaa.temp_compensate[0] );

   custom_em_lcd_exe_function(CUSTOM_EM_LCD_SUB, lcd_Bais_func, &aaa.bias[0] );
   custom_em_lcd_exe_function(CUSTOM_EM_LCD_SUB, lcd_Contrast_func, &aaa.contrast[0] );
   custom_em_lcd_exe_function(CUSTOM_EM_LCD_SUB, lcd_LineRate_func, &aaa.linerate[0] );
   custom_em_lcd_exe_function(CUSTOM_EM_LCD_SUB, lcd_Temperature_Compensation_func, &aaa.temp_compensate[0] );

   custom_em_pwm_get_value(freq, duty_cycle );
   freq[0] = 1;  duty_cycle[0] = 1;
   freq[1] = 2;  duty_cycle[1] = 2;
   freq[2] = 3;  duty_cycle[2] = 3;

   custom_em_pwm_set_value(freq, duty_cycle );
   custom_em_pwm_start(CUSTOM_EM_PWM_TYPE_1, freq[0], duty_cycle[0] );
   custom_em_pwm_start(CUSTOM_EM_PWM_TYPE_2, freq[1], duty_cycle[1] );
   custom_em_pwm_start(CUSTOM_EM_PWM_TYPE_3, freq[2], duty_cycle[2] );
   custom_em_pwm_stop(CUSTOM_EM_PWM_TYPE_1);
   custom_em_pwm_stop(CUSTOM_EM_PWM_TYPE_2);
   custom_em_pwm_stop(CUSTOM_EM_PWM_TYPE_3);
*/
}

void custom_em_set_mode(kal_uint8 mode)
{
// wangbei del 20060614
#if 0
   switch(mode)
   {
      case AUD_MODE_NORMAL:
         L1SP_SetOutputDevice(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_SPEAKER) );
         KT_SetOutputDevice(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_SPEAKER) );
         TONE_SetOutputDevice(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_SPEAKER) );
         Media_SetOutputDevice(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_LOUDSPEAKER) );
         L1SP_SetInputSource(custom_cfg_hw_aud_input_path(AUDIO_DEVICE_MIC1) );
         L1SP_SetFIR(KAL_TRUE);
         TONE_SetFIR(KAL_TRUE);
         KT_SetFIR(KAL_TRUE);
         L1SP_SetEchoSuppression(KAL_FALSE);
         break;
      case AUD_MODE_HEADSET:
         L1SP_SetOutputDevice(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_SPEAKER2) );
         KT_SetOutputDevice(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_SPEAKER2) );
         TONE_SetOutputDevice(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_SPEAKER2) );
         Media_SetOutputDevice(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_SPEAKER_BOTH) );
         L1SP_SetInputSource(custom_cfg_hw_aud_input_path(AUDIO_DEVICE_MIC2) );
         L1SP_SetFIR(KAL_FALSE);
         TONE_SetFIR(KAL_FALSE);
         KT_SetFIR(KAL_FALSE);
         L1SP_SetEchoSuppression(KAL_FALSE);
         break;
      case AUD_MODE_LOUDSPK:
         L1SP_SetOutputDevice(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_LOUDSPEAKER) );
         KT_SetOutputDevice(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_LOUDSPEAKER) );
         TONE_SetOutputDevice(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_LOUDSPEAKER) );
         Media_SetOutputDevice(custom_cfg_hw_aud_output_path(AUDIO_DEVICE_LOUDSPEAKER) );
         L1SP_SetInputSource(custom_cfg_hw_aud_input_path(AUDIO_DEVICE_MIC1) );
         L1SP_SetFIR(KAL_FALSE);
         TONE_SetFIR(KAL_FALSE);
         KT_SetFIR(KAL_FALSE);
         L1SP_SetEchoSuppression(KAL_TRUE);
         break;
   }
#endif
// wangbei del end   

}

void custom_em_set_gain(kal_uint8 type, kal_uint8 gain)
{

   switch(type)
   {
      case VOL_TYPE_CTN:
         TONE_SetOutputVolume(gain, gain);
         break;
      case VOL_TYPE_KEY:
         KT_SetOutputVolume(gain, gain);
         break;
      case VOL_TYPE_MIC:
         L1SP_SetMicrophoneVolume(gain);
         break;
      case VOL_TYPE_SPH:
         L1SP_SetOutputVolume(gain, gain);
         break;
      case VOL_TYPE_SID:
         L1SP_SetSidetoneVolume(gain);
         break;
      case VOL_TYPE_MEDIA:
         Media_SetOutputVolume(gain, gain);
         break;
      default:
         break;
   }      

}

void custom_em_set_loopback(kal_bool on)
{
   L1SP_SetAfeLoopback(on);
   if (on)
   {
      L1SP_Speech_On();
   }
   else
   {
      L1SP_Speech_Off();
   }   
}

kal_bool custom_em_get_melody_version(kal_char *mel_ver)
{
   kal_int32 file_handle;
   kal_wchar file_name[50];
   
   kal_wsprintf(file_name, "%s\\melody_ver.txt", "c:\\def_sound\\imelody");
   file_handle = pfopen((PCSTR)(file_name), FS_READ_ONLY|FS_OPEN_NO_DIR,0);    

   if (file_handle >= 0)
   {
      kal_uint32 len;
      len = pfread(file_handle, mel_ver, 50);

      /* close the file */
      pfclose(file_handle);
   }
   else
   {
      strcpy(mel_ver, "Melody V0.0"); 
   }
   return KAL_TRUE;
}

kal_bool custom_em_melody_key_shift(kal_int32 level)
{

   //if(Media_Control(MEDIA_CTRL_KEY_SHIFT, level)!=MEDIA_SUCCESS)// wangbei del 20060614
     // return KAL_FALSE;;// wangbei del 20060614
   return KAL_TRUE;
}

kal_bool custom_em_get_uart_detection_status( CUSTOM_EM_PORT_TYPE port )
{
   switch( port )
   {
      case CUSTOM_EM_UART_PORT1: return Get_UART_Detection_Status( uart_port1 );
      case CUSTOM_EM_UART_PORT2: return Get_UART_Detection_Status( uart_port2 );
#ifdef __UART3_SUPPORT__
      case CUSTOM_EM_UART_PORT3: return Get_UART_Detection_Status( uart_port3 );
#endif
      default: return KAL_FALSE;      
   }
}

void custom_em_set_uart_detection_status( CUSTOM_EM_PORT_TYPE port, kal_bool enable )
{
   switch( port )
   {
      case CUSTOM_EM_UART_PORT1: UART_Detection_Enable( uart_port1, enable ); break;
      case CUSTOM_EM_UART_PORT2: UART_Detection_Enable( uart_port2, enable ); break;
#ifdef __UART3_SUPPORT__
      case CUSTOM_EM_UART_PORT3: UART_Detection_Enable( uart_port3, enable ); break;
#else	                                   
		//added by elisa for enumeration value `CUSTOM_EM_UART_PORT3' not handled in switch
	  case CUSTOM_EM_UART_PORT3: 
	  	break;
#endif
	  case CUSTOM_EM_UART_MAX_PORT:    //added by elisa for enumeration value `CUSTOM_EM_UART_MAX_PORT' not handled in switch
	  	break;                            
   }
}


kal_wchar* custom_get_at_audio_download_folder()
{
   return CUSTOM_AT_AUDIO_DOWNLOAD_FOLDER_NAME;
}

kal_wchar* custom_get_at_image_download_folder()
{
   return CUSTOM_AT_IMAGE_DOWNLOAD_FOLDER_NAME;
}

#endif /* __L1_STANDALONE__ */


