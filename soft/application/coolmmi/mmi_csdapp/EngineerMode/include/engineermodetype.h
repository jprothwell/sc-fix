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
 * EngineerModeType.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for engineer mode
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

#ifdef __MMI_ENGINEER_MODE__  

#ifndef _ENGINEER_MODE_TYPE_H
#define _ENGINEER_MODE_TYPE_H
#include "mmi_features.h" 


/**************************
  * Band Selection
  **************************/
typedef enum{
	EM_BANDSEL_900,
	EM_BANDSEL_1800,
	EM_BANDSEL_DUAL,
	EM_BANDSEL_1900,
	EM_BANDSEL_850,
//Lisen 05312005	
//#if defined(__AUTO_BAND_SWITCH__)
	EM_BANDSEL_AUTO,
//#endif	
	EM_BANDSEL_MAX
} BANDENUM;


/**************************
  * UART Port Selection
  **************************/
#define     UART_BAUD_AUTO          0
#define     UART_BAUD_75            75
#define     UART_BAUD_150           150
#define     UART_BAUD_300           300
#define     UART_BAUD_600           600
#define     UART_BAUD_1200          1200
#define     UART_BAUD_2400          2400    
#define     UART_BAUD_4800          4800
#define     UART_BAUD_7200          7200
#define     UART_BAUD_9600          9600
#define     UART_BAUD_14400         14400
#define     UART_BAUD_19200         19200
#define     UART_BAUD_28800         28800
#define     UART_BAUD_33900         33900
#define     UART_BAUD_38400         38400
#define     UART_BAUD_57600         57600
#define     UART_BAUD_115200        115200
#define     UART_BAUD_230400        230400
#define     UART_BAUD_460800        460800
#define     UART_BAUD_921600        921600
#define     EM_BAUD_NUM  20

//#define EM_MAX_PORT_NUM   2
typedef enum {

   EM_UART_PORT_1 = 0,
   EM_UART_PORT_2 ,
#ifdef __UART3_SUPPORT__
   EM_UATR_PORT_3,
#endif

/*
#ifdef  __IRDA_SUPPORT__
   EM_UART_PORT_IRDA,
#endif
*/

/**
 *   USB UART port setting is not necessary here because data/tst will be redirected to usb port automatically after plug in
 *   The redirected object is determetered from "set default usb port app" menu item
 **/
/*
#ifdef __MMI_USB_SUPPORT__
   EM_UART_PORT_USB,
#endif
*/

/*
#ifdef __BLUETOOTH_SUPPORT__
   EM_UART_PORT_BLUETOOTH,
#endif
*/

   EM_UART_MAX_PORT_NUM
   
} EM_PORT_ENUM ;

typedef enum {

   EM_INLINE_UART_PORT_STR = 0,
   EM_INLINE_UART_PORT ,

   EM_INLINE_UART_BAUD_STR,
   EM_INLINE_UART_BAUD,
    
   EM_INLINE_STRUCT_MAX_NUM
   
} EM_SET_UART_INLINE ;

typedef enum {

   EM_APP_TST = 0,
   EM_APP_PS,
  
   EM_APP_MAX_NUM
} EM_APP_UART_SETTING ;


/**************************
  * Audio
  **************************/
typedef enum
{
   EM_NONE,
   EM_START,
   EM_STOP
}
EmCommandTypeEnum;



typedef enum
{
   EM_VOL_TYPE_CTN,
   EM_VOL_TYPE_KEY,
   EM_VOL_TYPE_MIC,
   EM_VOL_TYPE_SND,
   EM_VOL_TYPE_SPH,
   EM_VOL_TYPE_SID,
   EM_VOL_TYPE_MEDIA
}
EmVolumeTypeEnum;

typedef enum
{
   EM_NORMAL_MODE,
   EM_HEADSET_MODE,
   EM_LOUDSP_MODE
}
EmVolumeModeEnum;



typedef struct
{
   U8 mode;
	U8	speech[MAX_EM_VOLUME_LEVEL];
	U8 sp_gain;
	U8	keytone[MAX_EM_VOLUME_LEVEL];
	U8 kt_gain;
	U8	melody[MAX_EM_VOLUME_LEVEL];
	U8 mel_gain;
	U8	sound[MAX_EM_VOLUME_LEVEL];
	U8 snd_gain;
	U8	mic[MAX_EM_VOLUME_LEVEL];
	U8	mic_gain;
	U8	sidetone;
}
EmAudioProfileStruct;

/* refer to "aud_struct.h", audio_param_struct chang, this struct should change as well. */
typedef struct
{
   kal_uint16 selected_FIR_output_index;
   kal_uint16  es_time_const;
   kal_uint16  es_vol_const;
   kal_uint16  es_time_const2;
   kal_uint16  es_vol_const2;
   kal_uint16  max_swing;
}
EmAudioParamStruct;


#endif /* _ENGINEER_MODE_TYPE_H */

#endif /* __MMI_ENGINEER_MODE__ */


