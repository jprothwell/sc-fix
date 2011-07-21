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
 * DEVICE.H
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for definition of all hardware device requirement.
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

/********************************************************************************
*  Copyright Statement:
*  --------------------
*
*  This product has been developed using a protocol stack
*  developed by Sasken Communication Technologies Limited.
*
********************************************************************************/


#ifndef _DEVICE_H
#define _DEVICE_H

//Add by Yuchuan
#define UEM_GREETING_LEN      62
#define UEM_EQUIP_ID_LEN      30


/*
** Define the ring compose capability
*/
#define MAX_RING_COMPOSE_NUM        5
#define MAX_RING_COMPOSE_LEN        300

/*
** Define the display text maximum len
*/
#define UEM_DISPLAY_TEXT_LEN  30
#define UEM_MAX_GREETING_LEN  30


typedef enum {
   L4_BCD = 0x01,
   L4_ASCII = 0x04,
   L4_UCS2 = 0x08
} L4_charset_enum;


typedef enum {
   MMI_PS_AT_LANG_DEFAULT,
   MMI_PS_AT_LANG_UNKWN,
   MMI_PS_AT_LANG_EN,
   MMI_PS_AT_LANG_TW,
   MMI_PS_AT_LANG_ZH,
   
   MMI_PS_AT_LANG_LAST
} L4_AT_lang_enum;


/**************************************************************
  **
  **      Audio Interface 
  ** 
  **************************************************************/
  
/* Define the audio device type */
typedef kal_uint8 audio_type_enum;
#define AUDIO_DEVICE_SPEAKER            0 /* Tone, Keypad sound to play on audio device. */
#define AUDIO_DEVICE_MICROPHONE     1 /* Microphone sound to send on audio device. */
#define AUDIO_DEVICE_BUZZER              2 /* Ring tone sound to play on audio device. */
#define AUDIO_DEVICE_GMI                    3 /* MIDI/Melody sound to play on audio device */
#define AUDIO_DEVICE_SPEAKER2          4 /* earphone, carkit */
#define AUDIO_DEVICE_LOUDSPEAKER    5 /* loudspeaker for free sound */
#define AUDIO_DEVICE_SPEAKER_BOTH  6
#define MAX_AUDIO_DEVICE_NUM           7


/* Define the audio device type */
typedef kal_uint8 audio_input_path_enum;
#define AUDIO_DEVICE_MIC1     0    /* ME */
#define AUDIO_DEVICE_MIC2     1   /* Headset */
#define AUDIO_DEVICE_FMRR     2 /* FM Radio */

/* Define volume category */
typedef kal_uint8 volume_category_enum;
#define VOL_NORMAL               0
#define VOL_HEADSET              1
#define VOL_HANDFREE             2
#define MAX_VOL_CATE             3


/* Define volume type.*/
typedef kal_uint8 volume_type_enum;
#define VOL_TYPE_CTN                0  /* MMI can apply to associate volume; call tone attribute */
#define VOL_TYPE_KEY                1  /* MMI can apply to associate volume; keypad tone attribute */
#define VOL_TYPE_MIC                2  /* microphone attribute */
#define VOL_TYPE_GMI                3  /* MMI can apply to associate volume; melody, imelody, midi attribute */
#define VOL_TYPE_SPH                4  /* MMI can apply to associate volume; speech sound attribute */
#define VOL_TYPE_SID				5  /* side tone attribute */
#define VOL_TYPE_MEDIA            6  /* MMI can apply to associate volume; As MP3, Wave,... attribute */
#define VOL_TYPE_VIDEO             7
#define VOL_TYPE_TV            8
#define MAX_VOL_TYPE                9


/* Volume level. based index is from 0..(MAX_VOL_LEVEL-1) */
#ifndef MAX_VOL_LEVEL
#define MAX_VOL_LEVEL 7
#endif


/* Define the audio play style. */
typedef kal_uint8 audio_play_style_enum;
#define DEVICE_AUDIO_PLAY_CRESCENDO    0 /* Play sound for crescendo. */
#define DEVICE_AUDIO_PLAY_INFINITE     1 /* Play sound for infinite. */
#define DEVICE_AUDIO_PLAY_ONCE         2 /* Play sound for once. */
#define DEVICE_AUDIO_PLAY_DESCENDO     3 /* Play sound for descendo. */


typedef struct {
   kal_uint8 first_out_amp[MAX_AUDIO_DEVICE_NUM][MAX_VOL_LEVEL];
   kal_uint8 second_out_amp[MAX_AUDIO_DEVICE_NUM][MAX_VOL_LEVEL];
   kal_uint8 side[MAX_AUDIO_DEVICE_NUM][MAX_VOL_LEVEL];
   kal_uint8 volume[MAX_AUDIO_DEVICE_NUM];
} acoustic_gain_struct;



/**************************************************************
  **
  **      Keypad  Interface 
  ** 
  **************************************************************/
/*
** Define the keypad status.
*/
typedef kal_uint8 keypad_status_enum;
#define KEY_PRESSED  0 /* Keypad is pressed. */
#define KEY_RELEASED 1 /* Keypad is released. */
#define KEY_LONGPRESS 2 /* Keypad is longpressed. */
#define KEY_REPEATED 3 /* Keypad is repeated. */
#ifdef TWO_KEY_ENABLE
#define KEY_PRESSED_TWOKEY  4 /* Two Keypad is pressed. */
#define KEY_RELEASED_TWOKEY 5 /* Two Keypad is released. */
#endif   /*TWO_KEY_ENABLE*/

/*
** Define the keypad code value.
*/
typedef kal_uint8 keypad_code_enum;


/**************************************************************
  **
  **     RTC  Interface 
  ** 
  **************************************************************/

/* Define the RTC type. */
typedef kal_uint8 rtc_type_enum;
#define RTC_TIME_CLOCK_IND 0 /* The time clock indication. */
#define RTC_ALARM_IND      1 /* The alarm indication */


/* Define the type to setting RTC. */
typedef kal_uint8 rtc_setting_type_enum;
#define RTC_SETTING_TYPE_DATETIME   0
#define RTC_SETTING_TYPE_DATE       1
#define RTC_SETTING_TYPE_TIME       2


/* Define the RTC alarm id. */
typedef kal_uint8 rtc_alarm_id_enum;
#define RTC_ALARM1_IND     0 /* The alarm indication */
#define RTC_ALARM2_IND     1 /* The alarm indication */
#define RTC_ALARM3_IND     2 /* The alarm indication */
#define MAX_ALARM_NUM      3

/*
** Define the Alarm type
*/
typedef kal_uint8 rtc_alarm_type_enum;
#define RTC_ALARM_VIBRATOR    0x01
#define RTC_ALARM_LED         0x02
#define RTC_ALARM_SOUND       0x04
#define RTC_ALARM_SILENT      0x08

/*
** Define the Alarm recurr
*/
typedef kal_uint8 rtc_alarm_recurr_enum;
#define RTC_ALARM_DAY1        0x01
#define RTC_ALARM_DAY2        0x02
#define RTC_ALARM_DAY3        0x04
#define RTC_ALARM_DAY4        0x08
#define RTC_ALARM_DAY5        0x10
#define RTC_ALARM_DAY6        0x20
#define RTC_ALARM_DAY7        0x40
#define RTC_ALARM_DAY_ALL     0x7F

/*
** Define the Date/Time format type.
*/
typedef kal_uint8 date_format_enum;
#define   DATE_DD_MMM_YYYY    1
/*
#define   DATE_DD_MM_YY_H     2
#define   DATE_MM_DD_YY       3
#define   DATE_DD_MM_YY_S     4
#define   DATE_DD_MM_YY_P     5
#define   DATE_YYMMDD         6
#define   DATE_YY_MM_DD       7
*/

#define DATE_DD_MM_YYYY_S 	8
#define DATE_MM_DD_YYYY_S		9
#define DATE_YYYY_MM_DD_S		10
#define DATE_YYYY_MM_DD_H		11
#define DATE_MMM_DD_YYYY		12

#define   DATE_AUX_YY_MM_DD   1
#define   DATE_AUX_YYYY_MM_DD 2


typedef kal_uint8 time_format_enum;
#define   TIME_HH_MM_24       1
#define   TIME_HH_MM_12       2

/*
** Define the RTC clock/alarm format type.
*/
/* Could be rtc_alarm_format_enum or rtc_clock_format_enum */
typedef kal_uint8 rtc_format_enum; 
/*Y:Year,M:Month,W:DOW,D:DOM,H:Hour,M:Min,S:SEC*/
typedef kal_uint8 rtc_alarm_format_enum;
#define   DEVICE_AL_EN_None   0
#define   DEVICE_AL_EN_ChkS   1
#define   DEVICE_AL_EN_ChkMS  2
#define   DEVICE_AL_EN_ChkHMS 3
#define   DEVICE_AL_EN_ChkDHMS   4
#define   DEVICE_AL_EN_ChkWHMS   5
#define   DEVICE_AL_EN_ChkMDHMS  6
#define   DEVICE_AL_EN_ChkYMDHMS 7
#define   DEVICE_AL_EN_NoChange  8

typedef kal_uint8 rtc_clock_format_enum;
#define   DEVICE_TC_EN_None   0
#define   DEVICE_TC_EN_ChkS   1
#define   DEVICE_TC_EN_ChkM   2
#define   DEVICE_TC_EN_ChkH   3
#define   DEVICE_TC_EN_ChkD   4
#define   DEVICE_TC_EN_ChkW   5
#define   DEVICE_TC_EN_ChkMon 6
#define   DEVICE_TC_EN_ChkY   7
#define   DEVICE_TC_EN_NoChange  8

/*
** Define the alarm status.
*/
typedef kal_uint8 alarm_status_enum ;
#define  ALARM_STATUS_ON_POWEROFF_CHARING 0 /* The alarm is started when it's power off with charging status. */
#define  ALARM_STATUS_ON_POWEROFF         1 /* The alarm is started when it's power off status without charging. */
#define  ALARM_STATUS_ON_POWERON          2 /* The alarm is started when it's power on status. */

/*
** Define the calling status
*/
#define  UEM_CALL_UNKNOW          0 
#define  UEM_CALL_SETUP              1 
#define  UEM_CALL_CONNECT         2 
#define  UEM_CALL_DISCONNECT   3 

/**************************************************************
  **
  **      GPIO / EINT  Interface 
  ** 
  **************************************************************/

/* Define the internal GPIO device type. */
/**
 * Becareful!!!! 
 * MAX_GPIO_DEVICE_NUM Will impact the size of uem_context_rmi_struct data item !!!!!
 **/
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
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif 

typedef kal_uint8 gpio_device_enum;
#define GPIO_DEV_LED_MAINLCD                  0      /* Main LCD backlight */
#define GPIO_DEV_LED_SUBLCD                    1      /* Sub LCD backlight */
#define GPIO_DEV_LED_STATUS_1                2      /* Status LED  R*/
#define GPIO_DEV_LED_STATUS_2                3      /* Status LED  G*/
#define GPIO_DEV_LED_STATUS_3                4     /* Status LED   B*/
#define GPIO_DEV_LED_KEY                          5     /* Keypad backlight */
#define GPIO_DEV_VIBRATOR                        6      /* Vibrator */
#define GPIO_DEV_FLASHLIGHT                    7      /* FlashLight */
#define GPIO_DEV_RESERVED1                      8      /* Reserve_1 */
#define GPIO_DEV_RESERVED2                      9      /* Reserve_2 */
#define GPIO_DEV_RESERVED3                      10      /* Reserve_3 */
#define GPIO_DEV_RESERVED4                      11      /* Reserve_4 */
#define GPIO_DEV_RESERVED5                      12      /* Reserve_4 */
#define GPIO_DEV_RESERVED6                      13      /* Reserve_4 */
#define GPIO_DEV_RESERVED7                      14      /* Reserve_4 */
#define GPIO_DEV_RESERVED8                      15      /* Reserve_4 */
#define GPIO_DEV_RESERVED9                      16      /* Reserve_4 */
#define GPIO_DEV_RESERVED10                    17      /* Reserve_4 */
#define GPIO_DEV_RESERVED11                      18      /* Reserve_4 */
#define GPIO_DEV_RESERVED12                      19      /* Reserve_4 */
#define MAX_GPIO_DEVICE_NUM                   20


/* Define the LED light level. For LCD bright level, Keypad backlight,  Flashlight */
typedef kal_uint8 gpio_device_led_level_typedef;
#define LED_LIGHT_LEVEL0               0   /* turn off */
#define LED_LIGHT_LEVEL1               1
#define LED_LIGHT_LEVEL2               2
#define LED_LIGHT_LEVEL3               3
#define LED_LIGHT_LEVEL4               4
#define LED_LIGHT_LEVEL5               5
#define LED_LIGHT_LEVEL_MAX         6


/* Define the Vibrator level. */
typedef kal_uint8 gpio_device_vibrator_level_typedef;
#define VIBRATOR_OFF              0   /* turn off */
#define VIBRATOR_ON                1   /* turn on */


typedef kal_uint8 audio_mode_enum;
#define AUD_MODE_NORMAL        0   /* Normal Mode */
#define AUD_MODE_HEADSET        1   /* HeadSet (Earphone) Mode */
#define AUD_MODE_LOUDSPK        2   /* Loudspeaker Mode */
#define AUD_MODE_BLUETOOTH   3
#define MAX_AUD_MODE_NUM      4   

/*
** Define the external GPIO device interrupt.
*/
#if 0
typedef kal_uint8 ext_device_enum;
#define EXT_DEV_NONE                0   /* None, if there is no external device */
#define EXT_DEV_HANDFREE        1   /* Hand-free */
#define EXT_DEV_EARPHONE        2   /* Earphone */
#define EXT_DEV_CARKIT             3   /* Car-Kit */
#define EXT_DEV_KEY_1               4  /* Send Key */
#define EXT_DEV_KEY_2               5  /* Reserved */
#define EXT_DEV_UART                6
#define EXT_DEV_CLAM_OPEN     8
#define EXT_DEV_CLAM_CLOSE     9
#define EXT_DEV_TFLASH_DETECT     10
#define EXT_DEV_2D_SENSOR	11
#define MAX_EXT_DEVICE_NUM   12
#else
	typedef enum {
		EXT_DEV_NONE	  = 0  , /* None, if there is no external device */
		EXT_DEV_HANDFREE	   , /* Hand-free */
		EXT_DEV_EARPHONE	   , /* Earphone */
		EXT_DEV_CARKIT		   , /* Car-Kit */
		EXT_DEV_KEY_1		   , /* Send Key */
		EXT_DEV_KEY_2		   , /* Reserved */
		EXT_DEV_UART		   , 
		EXT_DEV_CLAM_OPEN  = 8 ,
		EXT_DEV_CLAM_CLOSE	   , 
		EXT_DEV_TFLASH_DETECT ,
		EXT_DEV_SLIDE_DOWN_OPEN 	 ,
		EXT_DEV_SLIDE_DOWN_CLOSE	 , 
		EXT_DEV_CAMERA_SLDE_OPEN	  ,
		EXT_DEV_CAMERA_SLDE_CLOSE	  , 
		//ugrec_tky 0409
		EXT_DEV_CAM_OPEN,
		EXT_DEV_CAM_CLOSE,
		EXT_DEV_FLIP_UP,
		EXT_DEV_FLIP_DOWN,
		EXT_DEV_FLIP_CENTER,
		//ugrec_tky 0409
		EXT_DEV_2D_SENSOR,
		MAX_EXT_DEVICE_NUM	 
	}ext_device_enum;
#endif


/**************************************************************
  **
  **      PMIC  Interface 
  ** 
  **************************************************************/
typedef enum
{
   PMIC_VBAT_STATUS,    /* Notify the battery voltage, BMT_VBAT_STATUS */
   PMIC_CHARGER_IN,     /* Charger plug in, BMT_CHARGER_IN */
   PMIC_CHARGER_OUT,    /* Charger plug out, BMT_CHARGER_OUT */
   PMIC_OVERVOLPROTECT,    /* The voltage of battery is too high. BMT_OVERVOLPROTECT */
   PMIC_OVERBATTEMP,    /* The temperature of battery is too high. BMT_OVERBATTEMP */
   PMIC_LOWBATTEMP,     /* The temperature of battery is too low. BMT_LOWBATTEMP */
   PMIC_OVERCHARGECURRENT, /* Charge current is too large. BMT_OVERCHARGECURRENT */
   PMIC_CHARGE_COMPLETE,   /* Charge is completed. BMT_CHARGE_COMPLETE */
   PMIC_INVALID_BATTERY,    /* invalid battery  BMT_INVALID_BAT*/
   PMIC_INVALID_CHARGER,   /* invalid charger BMT_INVALID_CHARGER*/
   PMIC_CHARGING_TIMEOUT, /* Bad battery BMT_CHARGE_TIMEOUT */ 
   PMIC_LOWCHARGECURRENT, /* Charge current is too Low. BMT_LOWCHARGECURRENT */
   PMIC_CHARGE_BAD_CONTACT, /* Charger Bad Contact */
   PMIC_BATTERY_BAD_CONTACT, /* Battery Bad Contact */
   PMIC_USB_CHARGER_IN,   /* Usb Charger plug in */
   PMIC_USB_CHARGER_OUT   /* Usb Charger plug out */
} pmic_status_enum;
typedef pmic_status_enum battery_status_enum;

typedef enum 
{

   BATTERY_LOW_POWEROFF = 0,
   BATTERY_LOW_TX_PROHIBIT,
   BATTERY_LOW_WARNING,
   BATTERY_LEVEL_0,
   BATTERY_LEVEL_1,
   BATTERY_LEVEL_2,
   BATTERY_LEVEL_3, 
   BATTERY_LEVEL_4 , /* BATTERY_LEVEL_4 */
   BATTERY_LEVEL_5 , /* BATTERY_LEVEL_5 */
   BATTERY_LEVEL_6 = BATTERY_LEVEL_3, /* BATTERY_LEVEL_6 */
   BATTERY_LEVEL_LAST = 9

} battery_level_enum;


/* Define the power on status. */
typedef kal_uint8 power_on_status_enum ;
#define  POWER_ON_KEYPAD         0
#define  POWER_ON_ALARM          1
#define  POWER_ON_CHARGER_IN     2
#define  POWER_ON_EXCEPTION      3
#define  POWER_ON_USB               4
#define  POWER_ON_UNINIT           5
#define  POWERON(mode) ((mode == POWER_ON_KEYPAD) || \
	                                    (mode == POWER_ON_ALARM) || \
	                                    (mode == POWER_ON_CHARGER_IN) ||\
	                                    (mode == POWER_ON_EXCEPTION) ||\
	                                    (mode == POWER_ON_USB) )

/* Define the POWER reset type. This is to use for the power restablish, while it's on charging or alarm status. */
typedef enum
{
   NORMAL_RESET,
   CHARGING_RESET,
   ALARM_RESET
} pmic_reset_type_enum;


/**************************************************************
  **
  **      Misc  Interface 
  ** 
  **************************************************************/

/* Indicator data type */
typedef kal_uint8 indicator_type_enum;
#define IND_BATTCHG        0
#define IND_SIGNAL         1
#define IND_SERVICE        2
#define IND_SOUNDER        3
#define IND_MESSAGE        4
#define IND_CALL           5
#define IND_VOX            6
#define IND_ROAM           7
#define IND_SMSFULL        8

/*
** Define the equipment to return the string mapping.
*/
typedef kal_uint8 equip_id_enum;
#define EQ_ME_MANUFACTURE_ID        0
#define EQ_ME_MODEL_ID              1
#define EQ_ME_REVISION_ID           2
#define EQ_ME_SERIAL_NUMBER_ID      3
#define EQ_ME_GLOBAL_OBJECT_ID      4
#define EQ_ME_MANUFACTURE_INFO_ID   5
#define EQ_ME_COMP_CAP_LIST_ID      6
#define MAX_ME_ID_NUM               7
#define MAX_ME_ID_LEN               30

/*
** Define the device process status.
*/
typedef kal_uint8 dev_status_enum;
#define DEV_OK             0 /* The device status is ok to process. */
#define DEV_FAIL           1 /* The device status is fail to process*/
#define DEV_FAIL_VM_NO_FREE_ENTRY   2 /* voice mail fail cause */
#define DEV_FAIL_VM_BUFFER_FULL     3
#define DEV_FAIL_VM_PLAY_NO_DATA    4



/**************************************************************
  **
  **      USB  Interface 
  ** 
  **************************************************************/
#define  DEVUSB_DETECT_ACTION_UNKNOWN     0
#define  DEVUSB_DETECT_ACTION_PLUGIN         1
#define  DEVUSB_DETECT_ACTION_PLUGOUT       2
#define  DEVUSB_DETECT_ACTION_MAX_ID         3

#define  DEVUSB_CFG_ACTION_UNKNOWN              0
#define  DEVUSB_CFG_ACTION_CDC_ACM               1
#define  DEVUSB_CFG_ACTION_MASS_STORAGE    2
#define  DEVUSB_CFG_ACTION_COMPOSITE           3
#define  DEVUSB_CFG_ACTION_WEBCAM           4
#define  DEVUSB_CFG_ACTION_MAX_ID                 5

#define USB_SWITCH_PORT_APP_DATA             0
#define USB_SWITCH_PORT_APP_DEBUG           1

#endif


