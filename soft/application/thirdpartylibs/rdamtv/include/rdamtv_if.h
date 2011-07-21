// ---------------------------------------------------------------------------
// RDA Microelectronics, Inc. Company Confidential Strictly Private
//
// $RCSfile: rdamtv_if.h,v $
// $Revision: 0 $
// $Author: wuhp $
// $Date: 2009-05-25 $
//
// ---------------------------------------------------------------------------
// >>>>>>>>>>>>>>>>>>>>>>>>> COPYRIGHT NOTICE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ---------------------------------------------------------------------------
// Copyright 2008-2009 (c) RDA Microelectronics, Inc.
//
// RDA owns the sole copyright to this software. Under international
// copyright laws you (1) may not make a copy of this software except for
// the purposes of maintaining a single archive copy, (2) may not derive
// works herefrom, (3) may not distribute this work to others. These rights
// are provided for information clarification, other restrictions of rights
// may apply as well.
//
// This is an unpublished work.
// ---------------------------------------------------------------------------

#ifndef __RDAMTV_IF_H__
#define __RDAMTV_IF_H__

/*GPIO for analog_tv power interface*/
#if 0
#define RDAMTV_POWER_PIN    0//gpio0--w3     3//gpo3--w2  //47     // GPIO_PORT_47
#else
#define RDAMTV_POWER_PIN    25//gpio0--w3     3//gpo3--w2  //47     // GPIO_PORT_47
#endif	//change by Speed 2009.11.30

/*GPIO for analog_tv I2C interface*/
#if 0
#define RDAMTV_SDA_PIN  	9//  3      // GPIO_PORT_3
#define RDAMTV_SCL_PIN  	8//  4      // GPIO_PORT_4
#else
#define RDAMTV_SDA_PIN  	7//  3      // GPIO_PORT_3
#define RDAMTV_SCL_PIN  	42//  4      // GPIO_PORT_4
#endif	//change by Speed 2009.11.30

#define RDAMTV_APP_MODULE	1
#define CAMERA_APP_MODULE	0

#define ATV_STATE	1
#define FM_STATE	0

typedef enum {
    TV_AREA_CHINA 				= 0,  
	TV_AREA_CHINA_SHENZHEN,
	TV_AREA_CHINA_TAIWAN, 
	TV_AREA_WESTERNEUROP,
	TV_AREA_AMERICA,
	TV_AREA_CHINA_MACAO,
	TV_AREA_INDIA,
	TV_AREA_PAKISTAN,
	TV_AREA_INDONESIA,
	TV_AREA_CHINA_HONGKONG,
	/*TV_AREA_IRAN,*/
	/*TV_AREA_IRAQ,*/
	TV_AREA_YEMEN,    // 10
	/*TV_AREA_SAUDI_ARABIA,*/
	TV_AREA_BAHRAIN,
	TV_AREA_ABU_DHABI,
	TV_AREA_KUWAIT,
	TV_AREA_THAI,
	/*TV_AREA_RUSSIAN,*/
	TV_AREA_PORTUGAL,
	TV_AREA_SPAIN,
	/*TV_AREA_FRANCE,*/
	TV_AREA_PHILIPINES,
	TV_AREA_MALAYSIA,
	TV_AREA_VIETNAM,
	TV_AREA_BRAZIL, // 20
	TV_AREA_UK,
	TV_AREA_SOUTH_AFRICA,
	TV_AREA_TURKEY,
	TV_AREA_SINGAPORE,
	TV_AREA_CAMBODIA,
	TV_AREA_LAOS,
	TV_AREA_AFGHANISTA,
	TV_AREA_CANADA,
	TV_AREA_KOREA,
	TV_AREA_MEXICO, // 30
	TV_AREA_CHILE,
	TV_AREA_VENEZUELA,
	TV_AREA_JAPAN,
	TV_AREA_ARGENTINA,
	TV_AREA_BURMA,	  // 35 
	TV_AREA_RUSSIA, // 36 
	TV_AREA_FACTORYMODE,
    TV_AREA_NUM
} TV_AREA;

typedef enum
{
	RDAMTV_POWER_OFF,
	RDAMTV_POWER_ON
}  RDAMTV_POWER_STATE;

/*functions for MMI/ISP/MEDIA*/
unsigned char rdamtv_if_get_cur_module(void);
void rdamtv_if_set_cur_module(unsigned char is_atv_app);
unsigned short rdamtv_if_get_chn_count(void);
void rdamtv_if_func_config(void);
void rdamtv_delay_ms(uint16 time);

#endif /* __RDAMTV_IF_H__ */

