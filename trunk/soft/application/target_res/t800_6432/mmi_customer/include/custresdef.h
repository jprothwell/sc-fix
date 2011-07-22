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


//micha0406
#ifndef _CUST_RES_DEF_H
#define _CUST_RES_DEF_H

#include "mmi_features.h"

#ifdef __MMI_AUTO_LANG_EN__
#define MMI_DEFLANGSSC_FOR_AUTO "*#0044#"
#endif

#define SSC_SW_VERSION							"*#8375#"
#define SSC_HW_MODULE                           "*#8376#"
#define SSC_HW_VERSION							"*#357#"			//Not used if __MMI_HW_VERSION__ is not turned on
#define SSC_ENGINEERING_MODE				"*#3641#"
#define SSC_FACTORY_MODE						"*#36*#"
#define SSC_FACTORY_MODE_AUTOTEST		"*#87#"
#define SSC_SERIAL_NUMBER						"*#33778#"
#define SSC_SET_LCD_CONTRAST						"*#523#"
#define SSC_IR_OPEN                    "*#678#"
#define SSC_IR_CLOSE                    "*#876#"

 
#define SSC_DISABLE_FOR_SHIPMENT	"*#6810#"

#define CUS_STR_RES_FILENAME		"../../mmi_customer/ResGenerator/CustStrList.txt" //Filename containg the array of STRING_LIST.

#define CUST_IMG_BASE_PATH			"../../mmi_customer/Images"

 
	#define CUS_STR_DATA_FILENAME		"../../mmi_customer/CustResource/PLUTO_MMI/CustStrRes.c" //Filename containg the array of CUSTOM_STRING.
	#define CUS_STR_MAP_FILENAME		"../../mmi_customer/CustResource/PLUTO_MMI/CustStrMap.c" //Filename containg the array of CUSTOM_STRING_MAP.
	#define CUS_IMG_DATA_FILENAME		"../../mmi_customer/CustResource/PLUTO_MMI/CustImgRes.c" //Filename containg the array of CUSTOM_IMAGE.
	#define CUS_IMG_MAP_FILENAME		"../../mmi_customer/CustResource/PLUTO_MMI/CustImgMap.c" //Filename containg the array of CUSTOM_IMAGE_MAP.
	#define CUS_MENU_DATA_FILENAME		"../../mmi_customer/CustResource/PLUTO_MMI/CustMenuRes.c" //Filename containg the array of CUSTOM_MENU.
	#define CUS_IMG_HW_FILENAME			"../../mmi_customer/CustResource/PLUTO_MMI/CustImgDataHW.h"
#if defined(__MMI_MAINLCD_128X160__)	
	#define CUST_IMG_PATH				"../../mmi_customer/Images/PLUTO128X160"
	#define CUST_IMG_PATH2				"../../mmi_customer/Images/PLUTO128X160"
	#define CUST_IMG_PATH4ZIP			"../../mmi_customer/Images/PLUTO128X160"
	#define CUST_IMG_NAME				"PLUTO128X160"
#elif defined(__MMI_MAINLCD_176X220__)
	#define CUST_IMG_PATH				"../../mmi_customer/Images/PLUTO176X220"
	#define CUST_IMG_PATH2				"../../mmi_customer/Images/PLUTO176X220"
	#define CUST_IMG_PATH4ZIP			"../../mmi_customer/Images/PLUTO176X220"
	#define CUST_IMG_NAME				"PLUTO176X220"
#elif defined(__MMI_MAINLCD_220X176__)/*cong.li added for product QPAD[220*176] on 2009.08.11. */
	#define CUST_IMG_PATH				"../../mmi_customer/Images/PLUTO220X176"
	#define CUST_IMG_PATH2				"../../mmi_customer/Images/PLUTO220X176"
	#define CUST_IMG_PATH4ZIP			"../../mmi_customer/Images/PLUTO220X176"
	#define CUST_IMG_NAME				"PLUTO220X176"	
#elif defined(__MMI_MAINLCD_240X320__)
	#define CUST_IMG_PATH				"../../mmi_customer/Images/PLUTO240X320"
	#define CUST_IMG_PATH2				"../../mmi_customer/Images/PLUTO240X320"
	#define CUST_IMG_PATH4ZIP			"../../mmi_customer/Images/PLUTO240X320"
	#define CUST_IMG_NAME				"PLUTO240X320"
#else
	#define CUST_IMG_PATH				"../../mmi_customer/Images/PLUTO"
	#define CUST_IMG_PATH2				"../../mmi_customer/Images/PLUTO"
	#define CUST_IMG_PATH4ZIP			"../../mmi_customer/Images/PLUTO"
	#define CUST_IMG_NAME				"PLUTO"
#endif

#if defined(CUST_SUBLCD_PATH)
    #undef CUST_SUBLCD_PATH
#endif
#if defined(__MMI_SUBLCD_48X64BW__)
    #define CUST_SUBLCD_PATH				"../../mmi_customer/Images/SubLCDBW48X64"
#elif defined(__MMI_SUBLCD_64X96__)
    #define CUST_SUBLCD_PATH				"../../mmi_customer/Images/SubLCD64X96"
#elif defined(__MMI_SUBLCD_96X64BW__)
    #define CUST_SUBLCD_PATH				"../../mmi_customer/Images/SubLCDBW96X64"
#elif defined(__MMI_SUBLCD_96X64__)
    #define CUST_SUBLCD_PATH				"../../mmi_customer/Images/SubLCD96X64"
#elif defined(__MMI_SUBLCD_128X128__)
    #define CUST_SUBLCD_PATH				"../../mmi_customer/Images/SubLCD128X128"
#endif
/* 20061103 SubLCD end */
 
	#define CUST_ADO_PATH				"../../mmi_customer/AUDIO/PLUTO"
	#define CUST_ADO_PATH2				"../../mmi_customer/AUDIO/PLUTO"
 

//micha0619
//#define MAX_PLMN_NO_LIST_LENGTH				215
//#define MAX_PLMN_NO_LIST_LENGTH				800

#endif


