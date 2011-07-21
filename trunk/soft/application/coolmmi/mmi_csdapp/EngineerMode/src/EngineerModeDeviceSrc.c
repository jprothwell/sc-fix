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
 * EngineerModeDeviceSrc.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for engineer mode device prat
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
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
 
 
#include "mmi_features.h" 
#include "kal_non_specific_general_types.h"
//CSD End

#ifdef __MMI_ENGINEER_MODE__  


/**************************************************************/
/*************************  Header Files *************************/
/**************************************************************/
#ifdef FIRST_PASS
#include "buildcfg.h"
#endif

#include "custdatares.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "protocolevents.h"
#include "custmenures.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "hardwareevents.h"
#include "thrdosinterface.h"
#include "osthreadinterfacegprot.h"

#include "timerevents.h"
#include "mainmenudef.h"
#include "engineermodedef.h"
#include "engineermodetype.h"
#include "engineermodeprot.h"
#include "mmi_data_types.h"
#include "customcfg.h"
#include "unicodexdcl.h"
#include "audioinc.h"
#include "idleappdef.h"
#include "bootup.h"
#include "globaldefs.h"
#include "commonscreens.h"
#include "globalscrenum.h"

#ifdef MMI_ON_HARDWARE_P
#include "device.h"
#include "custom_em.h"
#include "custom_hw_default.h"
//#include "pwm_sw.h"
#include "gpio_sw.h"
#else
#include "debuginitdef.h" /* add by zhuoxz, 2009-7-30 */
#endif
#include "custom_equipment.h"

#include "engineermodedeviceinc.h"
#include "simdetectiongexdcl.h"
#include "custom_hw_default.h"
#if defined(NAND_SUPPORT) //&& defined(MMI_ON_HARDWARE_P) 
#include "settingprofile.h"		/* error tone */
#endif
#include "engineermodepatch.h"

/**************************************************************/
/*********************** Global Variables *************************/
/**************************************************************/
extern U8       isEarphonePlugged;

/*Jinzh Add Start For 6424 Ver: TBM780  on 2007-8-28 17:36 */
extern U8 isEarphonePlugged_1; 
/*Jinzh Add End  For 6424 Ver: TBM780  on 2007-8-28 17:36 */
extern U8       prevMode;
extern U32	gClamOpenCnt;
extern U32	gClamCloseCnt;
extern U8       currentHighlightIndex;
extern PU8 subMenuDataPtrs[MAX_SUB_MENUS];
extern U8 subMenuData[MAX_SUB_MENUS][MAX_SUB_MENU_SIZE];
UI_character_type InputBuf[8];

#ifdef MMI_ON_HARDWARE_P
LCDSET LCDSetting;
PWMSET PWMSetting;
#endif

/* =======  Device -> GPIO =========*/
U8 gMenuListNum=0;
U32 gGPIOState;
static U8 EmGpioIdxMenu2Tbl[GPIO_LABELID_MAX];
#define IS_GPIO_SET(a)       ((gGPIOState & (0x1 << a)) >> a)
#define GPIO_STATE_SET(a)    (gGPIOState |= (0x1 << a))
#define GPIO_STATE_UNSET(a)  (gGPIOState &= ~(0x1 << a))
#define GPIO_STATE_TOGGLE(a) (gGPIOState ^= (0x1 << a))

U32 gGPOState;
#define IS_GPO_SET(a)       ((gGPOState & (0x1 << a)) >> a)
#define GPO_STATE_SET(a)    (gGPOState |= (0x1 << a))
#define GPO_STATE_UNSET(a)  (gGPOState &= ~(0x1 << a))
#define GPO_STATE_TOGGLE(a) (gGPOState ^= (0x1 << a))

/* =======  Device -> Set HW Default Level =========*/
typedef enum {

   EM_HWLEVEL_MENU_MAINLCD_CNTST = 0,
#ifdef __MMI_SUBLCD__      
   EM_HWLEVEL_MENU_SUBLCD_CNTST,
#endif
   EM_HWLEVEL_MENU_BATTERY,
   EM_HWLEVEL_MENU_PWM_1,
   EM_HWLEVEL_MENU_PWM_2,
   EM_HWLEVEL_MENU_PWM_3,

   MAX_EM_HWLEVEL_MENU_NUM
}  HW_LEVEL_MENULIST;

static U16 gHWLevelMenuID;
static U16 gHWLevelHilitItem;
static U8  gIsHWLevelInfoDirty;

U8   GPIOACTION = KAL_TRUE;
extern wgui_inline_item wgui_inline_items[];
U8 TextBuf[20];
WCHAR EMGPIOMode[4][2];
U8 *pEMGPIOMode[4];
WCHAR EMGPIODirection[2][4];
U8 *pEMGPIODirection[2];
WCHAR EMGPIOData[2][5];
U8 *pEMGPIOData[2];
S32   Gport = 0;
S32   Gdirection = 0;
S32   Glevel = 0;
S32   Gmode = 0;
U8  unicodebuf[20];

U8 TextBuf[20];
WCHAR EMPWMLevel[5][2];
U8 *pEMPWMLevel[5];
U8     currPWM = 0;
#ifdef MMI_ON_HARDWARE_P
U32   PWMFreq[CUSTOM_EM_PWM_TYPE_MAX];
U32   PWMDuty[CUSTOM_EM_PWM_TYPE_MAX];
#else
U32   PWMFreq[3];
U32   PWMDuty[3];
#endif

U32   currPWMFreq;
U32   currPWMDuty;
S32   currPWMLevel = 0;
U8  freqBuf[20];
U8  dutyBuf[20];

#ifdef __MMI_BT_SUPPORT__
#define BT_ADDRESS_LEN     (12)
#define BT_PASSWORD_LEN    (16)

U8 g_bt_device_address[BT_ADDRESS_LEN * 2 + 2];
U8 g_bt_password[BT_PASSWORD_LEN * 2 + 2];
#endif /* __MMI_BT_SUPPORT__ */ 


#ifdef __MMI_EM_BT_RF_TEST__

typedef struct {

    U8     test_item;      
    S32    currHoping;
    S32    currPacketType;
    U8     currCtrlPwrLevel; 
    U8     *pEMBtRfHoping[2];
    U8      channel[6];
    U8     *pEMBtRfPacketType[6];

    U8      data_len[8];
    U8      poll_period[6];    
    U8      BdAddr[26];

    U8      last_cmd;
} em_bt_rf_test_struct;

em_bt_rf_test_struct g_bt_rf_test_cntx={0};


typedef enum
{
    EM_BT_RF_TEST_TYPE_POWER_ON = 0,
    EM_BT_RF_TEST_TYPE_TX ,
    EM_BT_RF_TEST_TYPE_RX,
    EM_BT_RF_TEST_TYPE_BD_ADDR,
    EM_BT_RF_TEST_TYPE_ENTER_TEST_MODE,
    EM_BT_RF_TEST_TYPE_PWR_CTRL_LEVEL,
    EM_BT_RF_TEST_TYPE_MAX
} EM_BT_RF_TEST_TYPE;


typedef enum
{
    EM_BT_RF_TEST_HOPING_OFF = 0,
    EM_BT_RF_TEST_HOPING_ON
} EM_BT_RF_TEST_HOPING_TYPE;


typedef enum
{
    EM_BT_RF_TEST_PACKET_TYPE_DH1 = 0,
    EM_BT_RF_TEST_PACKET_TYPE_DH3,
    EM_BT_RF_TEST_PACKET_TYPE_DH5
} EM_BT_RF_TEST_PACKET_TYPE;

     #ifdef __BT_RDA5868__
extern void rda_bt_poweroff_for_test(void);
extern kal_uint8 rda_bt_poweron_for_test(void);
kal_uint8 rdabt_get_lmpversion(void);
extern void RDA5868_send_bbtest_singlehop_inquiry(void);
extern void RDA5868_send_vco_test(void);
extern void RDA_bt_send_cmu200_enable_dut(void);
extern void RDA_bt_send_tc3000_enable_dut(void);
extern void RDA_bt_send_n4010_enable_dut(void);
#endif
extern void TurnOnBacklight(U8 TimerFlag);
extern void ShowCategory64Screen(U16 message, U16 message_icon, U8 *history_buffer);
extern kal_uint8 rdabt_get_chipid(void);

#endif /* __MMI_EM_BT_RF_TEST__ */
//#ifdef __MMI_BT_COOLSAND_SUPPORT__
//#define __MMI_EM_BT_GET_CHIP_VERION__
//#endif /* #ifdef __MMI_BT_COOLSAND_SUPPORT__ */
#ifdef __MMI_EM_BT_GET_CHIP_VERION__
typedef struct
{
    U16          chip_ver;
    S8           chip_name[20];
} em_bt_chip_version_struct;

typedef struct
{
    U16          lmp_ver;
    S8           lmp_name[20];
} em_bt_lmp_version_struct;


#ifdef __BT_RDA5868__
const em_bt_chip_version_struct em_bt_chip_version[]=
{
  {0x0000, "Unknown|no-pwr BT"},
  {0x0001, "RDA5868 R5"},
  {0x0002, "RDA5868 R7"},
};

const em_bt_lmp_version_struct em_bt_lmp_version[]=
{
   {0x00, "Unknown|no-pwr BT"},
  {0x01, "BT V1.0"},
  {0x02, "BT V1.1"},
  {0x03, "BT V1.2"},
  {0x04, "BT V2.1"},
  {0x05, "BT V2.1"},
};

#else

const em_bt_chip_version_struct em_bt_chip_version[7]=
{
  {0x0000, "Unknown|no-pwr BT"},
//  {0x01E3, "MT6601 E3"},
//  {0x01E5, "MT6601 E5"},
//  {0x11E1, "MT6611 E1"},
//  {0x11E2, "MT6611 E2"},
//  {0x11E3, "MT6611 E3"},
//  {0x11E4, "MT6611 E4 "},
  {0x11E4,"RDA5868 R5"}
};

const em_bt_lmp_version_struct em_bt_lmp_version[4]=
{
  {0x0120, "BT V1.2"},
  {0x0200, "BT V2.0"},
  {0x0210, "BT V2.1"},
  {0x0300, "BT V3.0"}
};
#endif
#endif /* __MMI_EM_BT_GET_CHIP_VERION__ */
/**************************************************************/
/*********************** Extern functions *************************/
/**************************************************************/
extern void BatteryIndicationPopup(U16 stringId, U8 abn);
extern void BatteryStatusRsp(void* info);
extern void GpioDetectInd(void * info);
extern void IdleScreenChargerConnected(U8 playtone);
extern void IdleScreenChargerDisconnected(void);
extern void IdleScreenBatteryFullCharged(void);
extern void BatteryStatusIndication(U8 battery_voltage);
extern void DisplayPopup(U8 * string, U16 imageId, U8 imageOnBottom, U32 popupDuration, U8 toneId);

 

extern GPIO_MAP_ENTRY gpio_map_tbl[];
//extern WCHAR *kal_wstrcpy(WCHAR *to, const WCHAR *from);
extern kal_uint32 PWM1_Level_Info[PWM_MAX_LEVEL][2];
extern kal_uint32 PWM2_Level_Info[PWM_MAX_LEVEL][2];
extern kal_uint32 Alter_Level_Info[PWM_MAX_LEVEL][2];
extern GPIO_MAP_ENTRY eint_map_tbl[];
extern kal_uint8   MainLCD_Contrast_Level_Info[LCD_CONTRAST_MAX_LEVEL];
extern kal_uint8   SubLCD_Contrast_Level_Info[LCD_CONTRAST_MAX_LEVEL];
extern kal_uint32   Battery_Level_Info[BATTERY_MAX_LEVEL];
extern kal_uint8   MainLCD_Bias_Level_Info[LCD_PARAM_MAX_LEVEL];
extern kal_uint8   MainLCD_Linerate_Level_Info[LCD_PARAM_MAX_LEVEL];
extern kal_uint8   MainLCD_Temp_Level_Info[LCD_PARAM_MAX_LEVEL];
extern kal_uint8   SubLCD_Bias_Level_Info[LCD_PARAM_MAX_LEVEL];
extern kal_uint8   SubLCD_Linerate_Level_Info[LCD_PARAM_MAX_LEVEL];
extern kal_uint8   SubLCD_Temp_Level_Info[LCD_PARAM_MAX_LEVEL];


/**************************************************************/
/*********************** Code Body**** *************************/
/**************************************************************/

/****************************************
 **
 ** Menu Tree: Device --> LCD
 **
 ****************************************/
#define ___EM_DEVICE_LCD___


/**************************************************************
**	FUNCTION NAME		: HighlightEmLCD
**
**  	PURPOSE				: Highlight function for LCD menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmLCD(void)
{
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmLCDMenu,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmLCDMenu
**
**  	PURPOSE				: Entry function for LCD menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmLCDMenu(void)
{
	U16		ItemList[16];
	U16		ItemIcons[16];
	U16		nItems;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;

	EntryNewScreen(EM_DEV_LCD_MENU_ID, NULL, EntryEmLCDMenu, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_DEV_LCD_MENU_ID);
	nItems = GetNumOfChild(EM_DEV_LCD_MENU_ID);
	GetSequenceStringIds(EM_DEV_LCD_MENU_ID,ItemList);
	SetParentHandler(EM_DEV_LCD_MENU_ID);

       ClearAllKeyHandler();

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	ShowCategory52Screen(EM_DEV_LCD_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

#undef ___EM_DEVICE_LCD___



/***********************************************
 **
 ** Menu Tree: Device --> LCD --> Main LCD
 **
 ***********************************************/
#define ___EM_DEVICE_LCD_MainLCD___


/**************************************************************
**	FUNCTION NAME		: HighlightEmLCDMain
**
**  	PURPOSE				: Highlight function for Main LCD menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmLCDMain(void)
{
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmLCDMainMenu,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: EntryEmLCDMainMenu
**
**  	PURPOSE				: Entry function for Main LCD menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmLCDMainMenu(void)
{
	U16		ItemList[16];
	U16		ItemIcons[16];
	U16		nItems;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;

	EntryNewScreen(EM_DEV_LCD_MAIN_MENU_ID, NULL, EntryEmLCDMainMenu, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_DEV_LCD_MAIN_MENU_ID);
	nItems = GetNumOfChild(EM_DEV_LCD_MAIN_MENU_ID);
	GetSequenceStringIds(EM_DEV_LCD_MAIN_MENU_ID,ItemList);
	SetParentHandler(EM_DEV_LCD_MAIN_MENU_ID);

       ClearAllKeyHandler();

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	ShowCategory52Screen(EM_DEV_LCD_MAIN_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);


#ifdef MMI_ON_HARDWARE_P
       LCDSetting.Type = CUSTOM_EM_LCD_MAIN;
       custom_em_lcd_get_value( &LCDSetting.Param, LCDSetting.Type );
#endif      
}

#undef ___EM_DEVICE_LCD_MainLCD___



/***********************************************
 **
 ** Menu Tree: Device --> LCD --> Sub LCD
 **
 ***********************************************/
#define ___EM_DEVICE_LCD_SubLCD___


/**************************************************************
**	FUNCTION NAME		: HighlightEmLCDSub
**
**  	PURPOSE				: Highlight function for Sub LCD menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmLCDSub(void)
{
#ifdef __MMI_SUBLCD__
       SetLeftSoftkeyFunction(EntryEmLCDSubMenu,KEY_EVENT_UP);
#else
       SetLeftSoftkeyFunction (NULL, KEY_EVENT_UP);
#endif

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmLCDSubMenu
**
**  	PURPOSE				: Entry function for Sub LCD menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmLCDSubMenu(void)
{
#ifdef __MMI_SUBLCD__
	U16		ItemList[16];
	U16		ItemIcons[16];
	U16		nItems;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;


	EntryNewScreen(EM_DEV_LCD_SUB_MENU_ID, NULL, EntryEmLCDSubMenu, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_DEV_LCD_SUB_MENU_ID);
	nItems = GetNumOfChild(EM_DEV_LCD_SUB_MENU_ID);
	GetSequenceStringIds(EM_DEV_LCD_SUB_MENU_ID,ItemList);
	SetParentHandler(EM_DEV_LCD_SUB_MENU_ID);

       ClearAllKeyHandler();

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	ShowCategory52Screen(EM_DEV_LCD_SUB_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);


	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);


#ifdef MMI_ON_HARDWARE_P
	LCDSetting.Type = CUSTOM_EM_LCD_SUB;
       custom_em_lcd_get_value( &LCDSetting.Param, LCDSetting.Type );
#endif	

#endif /* __MMI_SUBLCD__ */
}

#undef ___EM_DEVICE_LCD_SubLCD___



/**********************************************************
 **
 ** Menu Tree: Device --> LCD --> (Main/Sub LCD) --> Set Contrast
 **
 **********************************************************/
#define ___EM_DEVICE_LCD_SetContrast___


/**************************************************************
**	FUNCTION NAME		: HighlightEmLCDSetContrast
**
**  	PURPOSE				: Highlight function for LCD contrast menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmLCDSetContrast(void)
{
	SetLeftSoftkeyFunction(EntryEmLCDSetContrast,KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmLCDSetContrast
**
**  	PURPOSE				: Entry function for LCD Contrast menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmLCDSetContrast(void)
{
	byte *history_buffer;
	S8 tempbuf[16];
	
	EntryNewScreen(EM_DEV_LCD_EDITOR_SCR_ID, NULL, NULL, NULL);
	history_buffer=GetCurrGuiBuffer(EM_DEV_LCD_EDITOR_SCR_ID);

#ifdef MMI_ON_HARDWARE_P
       /*Wangzl Modify Start For 6364 Ver: TBM780  on 2007-8-22 9:31 */
       //kal_wsprintf( InputBuf, "%d", LCDSetting.Param.contrast[0] );
       kal_wsprintf( (unsigned short *)tempbuf, "%d", LCDSetting.Param.contrast[0] );
	AnsiiToUnicodeString((S8 *)InputBuf, (S8 *)tempbuf);
       /*Wangzl Modify End  For 6364 Ver: TBM780  on 2007-8-22 9:31 */
#endif
       //ShowCategory16Screen(STR_GLOBAL_OK,NULL,STR_GLOBAL_BACK,NULL,(U8*)InputBuf, 8,history_buffer);
       ShowCategory5Screen( EM_DEV_LCD_SET_PARAM_1_TEXT,  0, \
                                          STR_GLOBAL_OK, 0 , \
                                          STR_GLOBAL_BACK, 0, \
                                          INPUT_TYPE_NUMERIC, \
                                          (U8*)InputBuf , 4 , history_buffer );

	SetLeftSoftkeyFunction(LSKEmLCDSetLCDHandler,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_END,KEY_EVENT_UP);
	//SetCategory16RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

#ifdef MMI_ON_HARDWARE_P
       LCDSetting.Count = 0;
       LCDSetting.Func = CUSTOM_EM_LCD_FUNC_CONSTRAST;
#endif

}


/**************************************************************
**	FUNCTION NAME		: LSKEmLCDSetLCDHandler
**
**  	PURPOSE				: LSK handler function for LCD setting
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void LSKEmLCDSetLCDHandler(void)
{
     U16 Arg=0;
     #ifdef MMI_ON_HARDWARE_P
     U8 Idx=0;
	 
	U8 *Param = NULL;
     #endif
 	
#ifdef MMI_ON_HARDWARE_P

     UnicodeToAnsii( (S8*) LCDSetting.ParamBuf , (S8*)InputBuf);
     Arg = atoi( (S8*) LCDSetting.ParamBuf );
     if( Arg > 255 ) Arg = 255;

     switch(LCDSetting.Func)
     {
     case CUSTOM_EM_LCD_FUNC_BIAS: 
        Idx=LCDSetting.Param.bias_idx;
        LCDSetting.Param.bias[LCDSetting.Count]= (U8) Arg; 
        Param = LCDSetting.Param.bias;
        break;
     case CUSTOM_EM_LCD_FUNC_CONSTRAST:
        Idx=LCDSetting.Param.contrast_idx;
        LCDSetting.Param.contrast[LCDSetting.Count]= (U8) Arg; 
        Param = LCDSetting.Param.contrast;
        break;
     case CUSTOM_EM_LCD_FUNC_LINERATE: 
        Idx=LCDSetting.Param.linerate_idx;
        LCDSetting.Param.linerate[LCDSetting.Count]= (U8) Arg; 
        Param = LCDSetting.Param.linerate;
        break;
     case CUSTOM_EM_LCD_FUNC_TEMPCOM: 
        Idx=LCDSetting.Param.temp_compensate_idx;
        LCDSetting.Param.temp_compensate[LCDSetting.Count]= (U8) Arg; 
        Param = LCDSetting.Param.temp_compensate;
        break;
     default: Arg = Idx; 
        break;
     }

     LCDSetting.Count++;
     if( LCDSetting.Count < Idx )
       EntryEmLCDSetLCDNextParam();
     else
     {
       custom_em_lcd_set_value( &LCDSetting.Param, LCDSetting.Type);
       custom_em_lcd_exe_function(LCDSetting.Type, LCDSetting.Func, Param );
       EngineerModeSuccessNotice(EM_NOTICE_SUCCESS_TEXT);
     }
#endif
}


/**************************************************************
**	FUNCTION NAME		: EntryEmLCDSetLCDNextParam
**
**  	PURPOSE				: Entry function for Setting LCD parameter menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmLCDSetLCDNextParam(void)
{
	byte *history_buffer;
	U32 title=EM_DEV_LCD_SET_PARAM_1_TEXT;

	EntryNewScreen(EM_DEV_LCD_EDITOR_SCR_ID, NULL, NULL, NULL);

   history_buffer=GetCurrGuiBuffer(EM_DEV_LCD_EDITOR_SCR_ID);

#ifdef MMI_ON_HARDWARE_P
     switch(LCDSetting.Func)
     {
     case CUSTOM_EM_LCD_FUNC_BIAS: 
        kal_wsprintf( InputBuf, "%d", LCDSetting.Param.bias[LCDSetting.Count] );
        break;
     case CUSTOM_EM_LCD_FUNC_CONSTRAST:
        kal_wsprintf( InputBuf, "%d", LCDSetting.Param.contrast[LCDSetting.Count] );
        break;
     case CUSTOM_EM_LCD_FUNC_LINERATE: 
        kal_wsprintf( InputBuf, "%d", LCDSetting.Param.linerate[LCDSetting.Count] );
        break;
     case CUSTOM_EM_LCD_FUNC_TEMPCOM: 
        kal_wsprintf( InputBuf, "%d", LCDSetting.Param.temp_compensate[LCDSetting.Count] );
        break;
     default: kal_wsprintf( InputBuf,""); 
        break;
     }
	 title = EM_DEV_LCD_SET_PARAM_1_TEXT+LCDSetting.Count;
#endif

        ShowCategory5Screen( (U16)title,  0, \
                                          STR_GLOBAL_OK, 0 , \
                                          STR_GLOBAL_BACK, 0, \
                                          INPUT_TYPE_NUMERIC, \
                                          (U8*)InputBuf , 8 , history_buffer );

	SetLeftSoftkeyFunction(LSKEmLCDSetLCDHandler,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_END,KEY_EVENT_UP);
	//SetCategory16RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}

#undef ___EM_DEVICE_LCD_SetContrast___



/***********************************************************
 **
 ** Menu Tree: Device --> LCD --> (Main/Sub LCD) --> Set Bias Ratio
 **
 ***********************************************************/
#define ___EM_DEVICE_LCD_SetBiasRatio___


/**************************************************************
**	FUNCTION NAME		: HighlightEmLCDSetBias
**
**  	PURPOSE				: Highlight function for Bias setting menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmLCDSetBias(void)
{
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmLCDSetBias,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmLCDSetBias
**
**  	PURPOSE				: Entry function for Setting Bias menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmLCDSetBias(void)
{
	byte *history_buffer;
	S8 tempbuf[16];
	EntryNewScreen(EM_DEV_LCD_EDITOR_SCR_ID, NULL, NULL, NULL);
	history_buffer=GetCurrGuiBuffer(EM_DEV_LCD_EDITOR_SCR_ID);

#ifdef MMI_ON_HARDWARE_P
       //kal_wsprintf( InputBuf, "%d", LCDSetting.Param.bias[0] );
       kal_wsprintf((unsigned short *)tempbuf,"%d",LCDSetting.Param.bias[0]);
	AnsiiToUnicodeString((S8 *)InputBuf, (S8 *)tempbuf);
#endif
       ShowCategory5Screen( EM_DEV_LCD_SET_PARAM_1_TEXT,  0, \
                                          STR_GLOBAL_OK, 0 , \
                                          STR_GLOBAL_BACK, 0, \
                                          INPUT_TYPE_NUMERIC, \
                                          (U8*)InputBuf , 4 , history_buffer );

	SetLeftSoftkeyFunction(LSKEmLCDSetLCDHandler,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_END,KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

#ifdef MMI_ON_HARDWARE_P
       LCDSetting.Count = 0;
       LCDSetting.Func = CUSTOM_EM_LCD_FUNC_BIAS;
#endif
}


/***********************************************************
 **
 ** Menu Tree: Device --> LCD --> (Main/Sub LCD) --> Set Line Rate
 **
 ***********************************************************/

/**************************************************************
**	FUNCTION NAME		: HighlightEmLCDSetLineRate
**
**  	PURPOSE				: Highlight function for Line rate setting menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmLCDSetLineRate(void)
{
	SetLeftSoftkeyFunction(EntryEmLCDSetLineRate,KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmLCDSetLineRate
**
**  	PURPOSE				: Entry function for Setting Line rate menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmLCDSetLineRate(void)
{
	byte *history_buffer;
	S8 tempbuf[16];
	EntryNewScreen(EM_DEV_LCD_EDITOR_SCR_ID, NULL, NULL, NULL);
	history_buffer=GetCurrGuiBuffer(EM_DEV_LCD_EDITOR_SCR_ID);

#ifdef MMI_ON_HARDWARE_P
       //kal_wsprintf( InputBuf, "%d", LCDSetting.Param.linerate[0] );
	kal_wsprintf( (unsigned short *)tempbuf, "%d", LCDSetting.Param.linerate[0] );
	AnsiiToUnicodeString((S8*)InputBuf,(S8*)tempbuf);
#endif
       ShowCategory5Screen( EM_DEV_LCD_SET_PARAM_1_TEXT,  0, \
                                          STR_GLOBAL_OK, 0 , \
                                          STR_GLOBAL_BACK, 0, \
                                          INPUT_TYPE_NUMERIC, \
                                          (U8*)InputBuf , 4 , history_buffer );

	SetLeftSoftkeyFunction(LSKEmLCDSetLCDHandler,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_END,KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

#ifdef MMI_ON_HARDWARE_P
       LCDSetting.Count = 0;
       LCDSetting.Func = CUSTOM_EM_LCD_FUNC_LINERATE;
#endif
}
#undef ___EM_DEVICE_LCD_SetBiasRatio___



/***********************************************************
 **
 ** Menu Tree: Device --> LCD --> (Main/Sub LCD) --> Set Temperature
 **
 ***********************************************************/
#define ___EM_DEVICE_LCD_SetTemperature___


/**************************************************************
**	FUNCTION NAME		: HighlightEmLCDSetTemperature
**
**  	PURPOSE				: Highlight function for Temperaturee setting menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmLCDSetTemperature(void)
{
	SetLeftSoftkeyFunction(EnrtyEmLCDSetTemperature,KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EnrtyEmLCDSetTemperature
**
**  	PURPOSE				: Entry function for Setting Temperature menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EnrtyEmLCDSetTemperature(void)
{
	byte *history_buffer;
	S8 tempbuf[16];
	
	EntryNewScreen(EM_DEV_LCD_EDITOR_SCR_ID, NULL, NULL, NULL);
	history_buffer=GetCurrGuiBuffer(EM_DEV_LCD_EDITOR_SCR_ID);

#ifdef MMI_ON_HARDWARE_P
       //kal_wsprintf( InputBuf, "%d", LCDSetting.Param.temp_compensate[0] );
       kal_wsprintf((unsigned short *)tempbuf, "%d", LCDSetting.Param.temp_compensate[0] );
	AnsiiToUnicodeString((S8*)InputBuf,(S8*)tempbuf);
#endif
       ShowCategory5Screen( EM_DEV_LCD_SET_PARAM_1_TEXT,  0, \
                                          STR_GLOBAL_OK, 0 , \
                                          STR_GLOBAL_BACK, 0, \
                                          INPUT_TYPE_NUMERIC, \
                                          (U8*)InputBuf , 4 , history_buffer );

	SetLeftSoftkeyFunction(LSKEmLCDSetLCDHandler,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_END,KEY_EVENT_UP);
	//SetCategory16RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

#ifdef MMI_ON_HARDWARE_P
       LCDSetting.Count = 0;
       LCDSetting.Func = CUSTOM_EM_LCD_FUNC_TEMPCOM;
#endif

}
#undef ___EM_DEVICE_LCD_SetTemperature___



/****************************************
 **
 ** Menu Tree: Device --> GPIO
 **
 ****************************************/
#define ___EM_DEVICE_GPIO___


/**************************************************************
**	FUNCTION NAME		: HighlightEmGPIO
**
**  	PURPOSE				: Highlight function for GPIO setting menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmGPIO(void)
{
	SetLeftSoftkeyFunction(EntryEmGPIOMenu,KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EnrtyEmLCDSetTemperature
**
**  	PURPOSE				: Entry function for Setting GPIO menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmGPIOMenu(void)
{
	U16		ItemList[16];
	U16		ItemIcons[16];
	U16		nItems;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;


	EntryNewScreen(EM_DEV_GPIO_SCR_ID, NULL, EntryEmGPIOMenu, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_DEV_GPIO_SCR_ID);
	nItems = GetNumOfChild(EM_DEV_GPIO_MENU_ID);
	GetSequenceStringIds(EM_DEV_GPIO_MENU_ID,ItemList);
	SetParentHandler(EM_DEV_GPIO_MENU_ID);

       ClearAllKeyHandler();

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	ShowCategory52Screen(EM_DEV_GPIO_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);


       gGPIOState=0x0;
       gGPOState=0x0;
       memset( InputBuf, 0, sizeof( InputBuf ) );
}

#undef ___EM_DEVICE_GPIO___



/****************************************
 **
 ** Menu Tree: Device --> GPIO --> List
 **
 ****************************************/
#define ___EM_DEVICE_GPIO_List___

/**************************************************************
**	FUNCTION NAME		: HighlightEmGPIOList
**
**  	PURPOSE				: Highlight function for GPIO List menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmGPIOList(void)
{
	SetLeftSoftkeyFunction(EntryEmGPIOListMenu,KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightOnCertainEmGPIO
**
**  	PURPOSE				: Highlight function for Certain GPIO 
**
**	INPUT PARAMETERS	: nIndex
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightOnCertainEmGPIO(S32 nIndex)
{
	U8 Idx=0;
	
	currentHighlightIndex = (U8) nIndex;
	Idx = EmGpioIdxMenu2Tbl[currentHighlightIndex];

	if( ((gpio_map_tbl[Idx].vaild == GPIO_VAILD) && IS_GPIO_SET(gpio_map_tbl[Idx].port )) 
	   || ((gpio_map_tbl[Idx].vaild == GPO_VAILD) && IS_GPO_SET(gpio_map_tbl[Idx].port )) )
         change_left_softkey(STR_GLOBAL_OFF,0);
     else if( (gpio_map_tbl[Idx].vaild == GPIO_VAILD) || (gpio_map_tbl[Idx].vaild == GPO_VAILD)  )
         change_left_softkey(STR_GLOBAL_ON,0);
     else
      {
         change_left_softkey(STR_GLOBAL_ERROR,0);
      }

      redraw_left_softkey();
}


/**************************************************************
**	FUNCTION NAME		: EmPrepareGPIOList
**
**  	PURPOSE				: Helper function for GPIO Lise preparation
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmPrepareGPIOList(void)
{
   U16 i,j=0;
   S8 buffer[32];

   gMenuListNum=0;
   for(i=0; i<GPIO_LABELID_MAX;i++)
   {
      if( gpio_map_tbl[i].vaild == GPIO_VAILD )
      {
      EmGpioIdxMenu2Tbl[j]=(U8) i; j++;
      sprintf(buffer,"%s", gpio_map_tbl[i].netname);
      AnsiiToUnicodeString((PS8)subMenuData[gMenuListNum], buffer);
      subMenuDataPtrs[gMenuListNum] = subMenuData[gMenuListNum];
      gMenuListNum++;
      }
   }
}


/**************************************************************
**	FUNCTION NAME		: EntryEmGPIOListMenu
**
**  	PURPOSE				: Entry function for GPIO Lise 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmGPIOListMenu(void)
{
	U8* guiBuffer;

	EntryNewScreen(EM_DEV_GPIO_LIST_SCR_ID, NULL, EntryEmGPIOListMenu, NULL);

	guiBuffer = GetCurrGuiBuffer (EM_DEV_GPIO_LIST_SCR_ID);		 

	RegisterHighlightHandler (HighlightOnCertainEmGPIO);

       EmPrepareGPIOList();
	if ( gMenuListNum < 1 )
       {
	   ShowCategory6Screen (EM_DEV_GPIO_LIST_TEXT, 0,
						  STR_GLOBAL_OK, 0,
						  STR_GLOBAL_BACK, 0,
						  gMenuListNum+1, (PU8*)subMenuDataPtrs,NULL, 0,
						  guiBuffer); 
		SetLeftSoftkeyFunction (NULL, KEY_EVENT_UP);
		SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
       }
	else 
	{
	   ShowCategory6Screen (EM_DEV_GPIO_LIST_TEXT, 0,
						  STR_GLOBAL_OK, 0,
						  STR_GLOBAL_BACK, 0,
						  gMenuListNum, (PU8*)subMenuDataPtrs,NULL, 0,
						  guiBuffer); 
		SetLeftSoftkeyFunction (LSKEmGPIOListMenu, KEY_EVENT_UP);
		SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	}
	
}


/**************************************************************
**	FUNCTION NAME		: LSKEmGPIOListMenu
**
**  	PURPOSE				: LSK handler for GPIO Lise 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void LSKEmGPIOListMenu(void)
{
    U8 Idx = EmGpioIdxMenu2Tbl[currentHighlightIndex];
    
    switch( gpio_map_tbl[Idx].vaild )
      {
      case GPIO_VAILD:
         if( IS_GPIO_SET( gpio_map_tbl[Idx].port ) ) 
         {
            change_left_softkey(STR_GLOBAL_ON,0);
#ifdef MMI_ON_HARDWARE_P
            custom_em_gpio_stop( gpio_map_tbl[Idx].port );
#endif
         }
         else
         {
            change_left_softkey(STR_GLOBAL_OFF,0);
#ifdef MMI_ON_HARDWARE_P
            custom_em_gpio_start( gpio_map_tbl[Idx].port );
#endif
         }
         GPIO_STATE_TOGGLE(gpio_map_tbl[Idx].port);
         break;

      case GPO_VAILD:
         if( IS_GPO_SET(gpio_map_tbl[Idx].port) ) 
         {
            change_left_softkey(STR_GLOBAL_ON,0);
            #ifdef MMI_ON_HARDWARE_P
            custom_em_gpo_stop( gpio_map_tbl[Idx].port );
            #endif
         }
         else
         {
            change_left_softkey(STR_GLOBAL_OFF,0);
            #ifdef MMI_ON_HARDWARE_P
            custom_em_gpo_start( gpio_map_tbl[Idx].port );
            #endif
         }
         GPO_STATE_TOGGLE(gpio_map_tbl[Idx].port);
         break;

      default:
         break;
      }

   redraw_left_softkey();

}

#undef ___EM_DEVICE_GPIO_List___



/****************************************
 **
 ** Menu Tree: Device --> GPIO --> Set
 **
 ****************************************/
#define ___EM_DEVICE_GPIO_Set___


/**************************************************************
**	FUNCTION NAME		: HighlightEmGPIOSet
**
**  	PURPOSE				: Highlight function for GPIO Set menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmGPIOSet(void)
{
       currentHighlightIndex = SET_GPIO_ACTION;
	SetLeftSoftkeyFunction(EntryEmGPIOSet,KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmGPOSet
**
**  	PURPOSE				: Highlight function for GPO Set menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmGPOSet(void)
{
       currentHighlightIndex = SET_GPO_ACTION;
	SetLeftSoftkeyFunction(EntryEmGPIOSet,KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmGPIOSet
**
**  	PURPOSE				: Entry function for GPIO Set menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmGPIOSet(void)
{
	byte *history_buffer;
	U16  text_id=EM_STR_GPIO_EDITOR;

	if(currentHighlightIndex == SET_GPO_ACTION)
	   text_id = EM_STR_GPO_EDITOR;
	else
	   text_id = EM_STR_GPIO_EDITOR;
	   
	   
	EntryNewScreen(EM_DEV_GPIO_EDITOR_SCR_ID, NULL, EntryEmGPIOSet, NULL);
	history_buffer=GetCurrGuiBuffer(EM_DEV_GPIO_EDITOR_SCR_ID);

   //ShowCategory16Screen(STR_GLOBAL_OK,NULL,STR_GLOBAL_BACK,NULL,(U8*)InputBuf, 8,history_buffer);
       ShowCategory5Screen( text_id,  0, \
                                          STR_GLOBAL_OK, 0 , \
                                          STR_GLOBAL_BACK, 0, \
                                          INPUT_TYPE_NUMERIC, \
                                          (U8*)InputBuf , 8 , history_buffer );
   
	SetLeftSoftkeyFunction(LSKEmGPIOSetCertain,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_END,KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: ExitGPIODetail
**
**  	PURPOSE				: Exit function for GPIO detail screen
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitEMGPIODetail (void)
{
	history_t currHistory;
       U16 inputBufferSize;

	CloseCategory57Screen();
   
       currHistory.scrnID = EM_DEV_GPIO_VIEWER_SCR_ID;
	currHistory.entryFuncPtr = EntryEMGPIODetail;

	GetCategoryHistory(currHistory.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();  
	GetCategory57Data((U8*) currHistory.inputBuffer);       
	AddNHistory(currHistory, inputBufferSize);           
}


/**************************************************************
**	FUNCTION NAME		: EMGPIOHighlightHandler
**
**  	PURPOSE				: highlight handler function for GPIO menu
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EMGPIOHighlightHandler(S32 index) 
{

}

/**************************************************************

	FUNCTION NAME		: EMGPIODoneFunc()

  	PURPOSE				: Set GPIO parameters

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EMGPIODoneFunc(void) 
{
#ifdef MMI_ON_HARDWARE_P
     if (GPIOACTION == KAL_TRUE)
     {
        GPIO_ModeSetup((kal_uint16) Gport, (kal_uint16) Gmode);
        // DIRECTION............................
        GPIO_WriteIO((char) Glevel, (char) Gport);
     }
     else
     {
        GPO_ModeSetup((kal_uint16) Gport, (kal_uint16) Gmode);
        GPO_WriteIO((char) Glevel, (char) Gport);
     }
#endif
     //DisplayPopup((PU8)GetString(EM_NOTICE_SUCCESS_TEXT), IMG_GLOBAL_DEFAULT, 0, EM_NOTIFY_DURATION, 0);
     EngineerModeSuccessNotice(EM_NOTICE_SUCCESS_TEXT);
     DeleteScreenIfPresent(EM_DEV_GPIO_VIEWER_SCR_ID);
}

/**************************************************************

	FUNCTION NAME		: EntryGPIODetail()

  	PURPOSE				: Show GPIO detail screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryEMGPIODetail(void) 
{
#ifdef MMI_ON_HARDWARE_P
	U8* guiBuffer;
	U8* inputBuffer;
	U16 inputBufferSize;
       U16 ImageList[7];
       U8  textbuf[10];
              
	EntryNewScreen(EM_DEV_GPIO_VIEWER_SCR_ID, ExitEMGPIODetail, NULL, NULL);
	InitializeCategory57Screen();
	
	guiBuffer = GetCurrGuiBuffer(EM_DEV_GPIO_VIEWER_SCR_ID);
	
	inputBuffer = GetCurrNInputBuffer(EM_DEV_GPIO_VIEWER_SCR_ID, &inputBufferSize);
 
	RegisterHighlightHandler(EMGPIOHighlightHandler);

	// PORT
	SetInlineItemActivation(&wgui_inline_items[0], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	if (GPIOACTION == KAL_TRUE)
	{
	   sprintf((S8*)textbuf, "GPIO %d", (U16)Gport);
       }
	else
	{
	   sprintf((S8*)textbuf, "GPO %d", (U16)Gport);
       }
		
	AnsiiToUnicodeString((S8*)unicodebuf, (S8*)textbuf);
	SetInlineItemDisplayOnly(&wgui_inline_items[0], unicodebuf);
	//DisableInlineItemHighlight(&wgui_inline_items[0]);
       ImageList[0] = IMG_STATUS;

	SetInlineItemActivation(&wgui_inline_items[1], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&(wgui_inline_items[1]), (U8*)GetString(EM_STR_GPIO_LEVEL));
       ImageList[1] = IMG_STATUS;

       // Level
       kal_wstrcpy((WCHAR*) EMGPIOData[0], (const WCHAR *) kal_toWCHAR("Low"));
       pEMGPIOData[0] = (U8*) (EMGPIOData[0]);
       kal_wstrcpy((WCHAR*) EMGPIOData[1], (const WCHAR *) kal_toWCHAR("High"));
       pEMGPIOData[1] = (U8*) (EMGPIOData[1]);
	SetInlineItemActivation(&wgui_inline_items[2], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemSelect(&wgui_inline_items[2], 2, (U8**)&pEMGPIOData, (S32*)&Glevel);
       ImageList[2] = EM_DEV_GPIO_SET_ICON;
	
	SetInlineItemActivation(&wgui_inline_items[3], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&(wgui_inline_items[3]), (U8*)GetString(EM_STR_GPIO_MODE));
       ImageList[3] = IMG_STATUS;

       // Mode
       kal_wstrcpy((WCHAR*) EMGPIOMode[0], (const WCHAR *) kal_toWCHAR("0"));
       pEMGPIOMode[0] = (U8*) (EMGPIOMode[0]);
	kal_wstrcpy((WCHAR*) EMGPIOMode[1], (const WCHAR *) kal_toWCHAR("1"));
       pEMGPIOMode[1] = (U8*) (EMGPIOMode[1]);	
	kal_wstrcpy((WCHAR*) EMGPIOMode[2], (const WCHAR *) kal_toWCHAR("2"));
       pEMGPIOMode[2] = (U8*) (EMGPIOMode[2]);
	kal_wstrcpy((WCHAR*) EMGPIOMode[3], (const WCHAR *) kal_toWCHAR("3"));
       pEMGPIOMode[3] = (U8*) (EMGPIOMode[3]);
	SetInlineItemActivation(&wgui_inline_items[4], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemSelect(&wgui_inline_items[4], 4, (U8**)&pEMGPIOMode, (S32*)&Gmode);
       ImageList[4] = EM_DEV_GPIO_SET_ICON;

	SetInlineItemActivation(&wgui_inline_items[5], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&(wgui_inline_items[5]), (U8*)GetString(EM_STR_GPIO_DIRECTION));
       ImageList[5] = IMG_STATUS;

       //Direction
       if (GPIOACTION == KAL_TRUE)
       {
          kal_wstrcpy((WCHAR*) EMGPIODirection[0], (const WCHAR *) kal_toWCHAR("IN"));
          pEMGPIODirection[0] = (U8*) (EMGPIODirection[0]);
          kal_wstrcpy((WCHAR*) EMGPIODirection[1], (const WCHAR *) kal_toWCHAR("OUT"));
          pEMGPIODirection[1] = (U8*) (EMGPIODirection[1]);
	   SetInlineItemActivation(&wgui_inline_items[6], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	   SetInlineItemSelect(&wgui_inline_items[6], 2, (U8**)&pEMGPIODirection, (S32*)&Gdirection);
       }
       else // the Direction of GPO is always OUT
       {
          kal_wstrcpy((WCHAR*) EMGPIODirection[0], (const WCHAR *) kal_toWCHAR("OUT"));
          pEMGPIODirection[0] = (U8*) (EMGPIODirection[0]);
	   SetInlineItemActivation(&wgui_inline_items[6], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	   SetInlineItemSelect(&wgui_inline_items[6], 1, (U8**)&pEMGPIODirection, (S32*)&Gdirection);
       }
       ImageList[6] = EM_DEV_GPIO_SET_ICON;


       if(inputBuffer!=NULL) SetCategory57Data(wgui_inline_items, 7, inputBuffer); 
       
       ShowCategory57Screen((GPIOACTION == KAL_TRUE) ? EM_STR_GPIO_EDITOR : EM_STR_GPO_EDITOR, 
       	                EM_DEV_GPIO_SET_ICON, STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
				  7, ImageList, wgui_inline_items, 0, guiBuffer);	

	SetCategory57RightSoftkeyFunctions(EMGPIODoneFunc, GoBackHistory);
#endif
}


/**************************************************************
**	FUNCTION NAME		: LSKEmGPIOSetCertain
**
**  	PURPOSE				: LSK handler function for GPIO Set certain menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void LSKEmGPIOSetCertain( void )
{
   S8  Arg=0;
   U8  U8StrBuf[8];

   UnicodeToAnsii( (S8*)U8StrBuf , (S8*)InputBuf);
   Arg = (S8) atoi( (S8*)U8StrBuf );
   // Lisen 0906
   if( Arg >= GPIO_LABELID_MAX /*|| Arg <= 0*/) 
   {
   	EngineerModeErrorNotice(0);
   	return;
   }

   Gport = Arg;
  
   switch( currentHighlightIndex )
    {
    case SET_GPIO_ACTION:
         #ifdef MMI_ON_HARDWARE_P
      Gdirection = GPIO_ReturnDir(Arg);
      Glevel = GPIO_ReturnDout(Arg);
      Gmode  = GPIO_ReturnMode(Arg);
      GPIOACTION = KAL_TRUE; 
      EntryEMGPIODetail();
         #endif
      break;

    case SET_GPO_ACTION:
         #ifdef MMI_ON_HARDWARE_P
      Gdirection = GPIO_ReturnDir(Arg);
      Glevel = GPO_ReturnDout(Arg);
      Gmode  = GPO_ReturnMode(Arg);
      GPIOACTION = KAL_FALSE; 
      EntryEMGPIODetail();
#endif
      break;

    default:
      EngineerModeErrorNotice(0);
      break;
    }

}

#undef ___EM_DEVICE_GPIO_Set___



/****************************************
 **
 ** Menu Tree: Device --> PWM
 **
 ****************************************/
#define ___EM_DEVICE_PWM___


/**************************************************************
**	FUNCTION NAME		: HighlightEmPWM
**
**  	PURPOSE				: Highlight function for PWM setting menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmPWM(void)
{
	SetLeftSoftkeyFunction(EntryEmPWMMenu,KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: HighlightOnCertainEmPWM
**
**  	PURPOSE				: Highlight function for certain PWM setting menu
**
**	INPUT PARAMETERS	: nIndex
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightOnCertainEmPWM(S32 nIndex)
{
	currentHighlightIndex = (U8) nIndex;
}


/**************************************************************
**	FUNCTION NAME		: EmPreparePWMList
**
**  	PURPOSE				: Helper function for  PWM lisen preparation
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmPreparePWMList(void)
{
   U16 i;
   S8 buffer[32];

#ifdef MMI_ON_HARDWARE_P
   for(i=0; i<CUSTOM_EM_PWM_TYPE_MAX;i++)
#else
   for(i=0; i<3;i++)
#endif
   {
      sprintf(buffer,"PWM%d", i+1);
      AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
      subMenuDataPtrs[i] = subMenuData[i];
   }
}


/**************************************************************
**	FUNCTION NAME		: EntryEmPWMMenu
**
**  	PURPOSE				: Entry function for PWM menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmPWMMenu(void)
{
#ifdef MMI_ON_HARDWARE_P
	U8* guiBuffer;

	EntryNewScreen(EM_DEV_PWM_MENU_ID, NULL, EntryEmPWMMenu, NULL);
	guiBuffer = GetCurrGuiBuffer (EM_DEV_PWM_MENU_ID);		 

	RegisterHighlightHandler (HighlightOnCertainEmPWM);

       EmPreparePWMList();
   
       ShowCategory6Screen (EM_DEV_PWM_TEXT, 0,
						  STR_GLOBAL_OK, 0,
						  STR_GLOBAL_BACK, 0,
						  3, (PU8*)subMenuDataPtrs,NULL, 0,
						  guiBuffer); 
       SetLeftSoftkeyFunction (EntryEMPWMDetail, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);

       PWMSetting.Count=0;
   //custom_em_pwm_get_value(PWMSetting.Param.freq, PWMSetting.Param.duty_cycle);
#endif
}


/**************************************************************
**	FUNCTION NAME		: LSKEmPWMMenu
**
**  	PURPOSE				: LSK hanlder function for PWM menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void LSKEmPWMMenu(void)
{
   U32 Arg=0;

   #ifdef MMI_ON_HARDWARE_P
   U8 StrBuf[16];
   #endif

#ifdef MMI_ON_HARDWARE_P
   if( PWMSetting.Count == 0 )
   {
      EntryEmPWMSetParam();
      switch( currentHighlightIndex )
      {
      case 0: PWMSetting.Type = CUSTOM_EM_PWM_TYPE_1 ; break;
      case 1: PWMSetting.Type = CUSTOM_EM_PWM_TYPE_2 ; break;
      case 2: PWMSetting.Type = CUSTOM_EM_PWM_TYPE_3 ; break;
      default: PWMSetting.Type = CUSTOM_EM_PWM_TYPE_1 ; break;
      }
   }
   else if( PWMSetting.Count == 1)
   {
      UnicodeToAnsii( (S8*)StrBuf , (S8*)InputBuf);
      Arg = atoi( (S8*)StrBuf );
      if (Arg > 9999999 ) Arg = 9999999;
      PWMSetting.Param.freq[currentHighlightIndex] = Arg;
      EntryEmPWMSetParam();
   }
   else
   {
      UnicodeToAnsii( (S8*)StrBuf , (S8*)InputBuf);
      Arg = atoi( (S8*)StrBuf );
      if (Arg > 9999999 ) Arg = 9999999;
      PWMSetting.Param.duty_cycle[currentHighlightIndex] = Arg;

      if( (PWMSetting.Param.freq[currentHighlightIndex] == 0 ) || (PWMSetting.Param.duty_cycle[currentHighlightIndex] == 0 ) )
      {
          /* Divide by zero */
          EngineerModeErrorNotice( STR_GLOBAL_ERROR );
       }
      else
      {
      custom_em_pwm_start(PWMSetting.Type, \
                          PWMSetting.Param.freq[currentHighlightIndex], \
                          PWMSetting.Param.duty_cycle[currentHighlightIndex] );
      
      //custom_em_pwm_set_value(PWMSetting.Param.freq, PWMSetting.Param.duty_cycle);
      //EngineerModeSuccessNotice(EM_NOTICE_SUCCESS_TEXT);
      ShowCategory21Screen(EM_NOTICE_SUCCESS_TEXT, 0,
						 EM_DEV_PWM_STOP_TEXT, 0,
						 0, 0,
						 EM_DEV_PWM_START_TEXT, NULL);
	   SetLeftSoftkeyFunction(LSKEmPWMStop,KEY_EVENT_UP);
       }

      return;
   }

   PWMSetting.Count++;

#endif /* MMI_ON_HARDWARE_P */
}

/**************************************************************

	FUNCTION NAME		: ExitEMPWMDetail()

  	PURPOSE				: Show PWM detail screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitEMPWMDetail (void)
{
	history_t currHistory;
       U16 inputBufferSize;
       
	CloseCategory57Screen();
   
       currHistory.scrnID = EM_DEV_PWM_VIEWER_SCR_ID;
	currHistory.entryFuncPtr = EntryEMPWMDetail;

	GetCategoryHistory(currHistory.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();  
	GetCategory57Data((U8*) currHistory.inputBuffer);       
	AddNHistory(currHistory, inputBufferSize);           
}


/**************************************************************
**	FUNCTION NAME		: EMPWMHighlightHandler
**
**  	PURPOSE				: Highlight function for certain PWM setting menu
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EMPWMHighlightHandler(S32 index) 
{

}

/**************************************************************

	FUNCTION NAME		: EMPWMDoneFunc()

  	PURPOSE				: Set GPIO parameters

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EMPWMDoneFunc(void) 
{
#ifdef MMI_ON_HARDWARE_P   
     U8  textbuf[20];

     CloseCategory57Screen();

     if ((!pfnUnicodeStrlen((const S8*)freqBuf) && !pfnUnicodeStrlen((const S8*)dutyBuf)) 
     	                                                               ||
     	   (pfnUnicodeStrlen((const S8*)freqBuf)>5  || pfnUnicodeStrlen((const S8*)dutyBuf)>3)) 
     {
        DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 0, EM_NOTIFY_DURATION, 0);
 	 DeleteScreenIfPresent(EM_DEV_PWM_VIEWER_SCR_ID);
        return;
     }
     //convert the textedit buffer to currPWMFreq and currPWMDuty
     UnicodeToAnsii((PS8)textbuf,  (PS8)freqBuf);
     currPWMFreq = atoi((const char *) textbuf);

     UnicodeToAnsii((PS8)textbuf,  (PS8)dutyBuf);
     currPWMDuty = atoi((const char *) textbuf);
     
     // Set up PWMFreq and PWMDuty
     if (currPWM == 0)
     {
        PWM1_Level_Info[currPWMLevel][0] = PWMFreq[currPWM] = currPWMFreq;
        PWM1_Level_Info[currPWMLevel][1] = PWMDuty[currPWM] = currPWMDuty;       	
     } 
     else if (currPWM == 1)
     {
        PWM2_Level_Info[currPWMLevel][0] = PWMFreq[currPWM] = currPWMFreq;
        PWM2_Level_Info[currPWMLevel][1] = PWMDuty[currPWM] = currPWMDuty;       	
     } 
     else if (currPWM == 2)
     {
        Alter_Level_Info[currPWMLevel][0] = PWMFreq[currPWM] = currPWMFreq;
        Alter_Level_Info[currPWMLevel][1] = PWMDuty[currPWM] = currPWMDuty;       	
     } 
     
     custom_em_pwm_set_value((kal_uint16 *) PWMFreq, (kal_uint8 *) PWMDuty);
     custom_em_pwm_set_level(currPWM, (kal_uint8) currPWMLevel);
     custom_em_pwm_start(currPWM, currPWMFreq, currPWMDuty);

     SendSetHWLevelInfoReq();

     DeleteScreenIfPresent(EM_DEV_PWM_VIEWER_SCR_ID);
#endif
}


/**************************************************************
**	FUNCTION NAME		: PWMLevelHighlightHandler
**
**  	PURPOSE				: Highlight function for certain PWM level setting menu
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
static void PWMLevelHighlightHandler(S32 index)
{
       U8  textbuf[10];

       currPWMLevel = index; 
       
       if (currPWM == 0)
       {
          currPWMFreq = PWMFreq[currPWM] = PWM1_Level_Info[index][0];
          currPWMDuty = PWMDuty[currPWM] = PWM1_Level_Info[index][1];       	
       } 
       else if (currPWM == 1)
       {
          currPWMFreq = PWMFreq[currPWM] = PWM2_Level_Info[index][0];
          currPWMDuty = PWMDuty[currPWM] = PWM2_Level_Info[index][1];       	
       } 
       else if (currPWM == 2)
       {
          currPWMFreq = PWMFreq[currPWM] = Alter_Level_Info[index][0];
          currPWMDuty = PWMDuty[currPWM] = Alter_Level_Info[index][1];       	
       } 

       // Frequency
	SetInlineItemActivation(&wgui_inline_items[4], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
       sprintf((S8*)textbuf, "%d", (U16) currPWMFreq);		
	AnsiiToUnicodeString((S8*)freqBuf, (S8*)textbuf);
       SetInlineItemTextEdit(&wgui_inline_items[4], (PU8)freqBuf, 20, INPUT_TYPE_NUMERIC);
	
       // Duty
	SetInlineItemActivation(&wgui_inline_items[6], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
       sprintf((S8*)textbuf, "%d", (U16) currPWMDuty);		
	AnsiiToUnicodeString((S8*)dutyBuf, (S8*)textbuf);
       SetInlineItemTextEdit(&wgui_inline_items[6], (PU8)dutyBuf, 20, INPUT_TYPE_NUMERIC);
  
       RedrawCategory57Screen();
}

/**************************************************************

	FUNCTION NAME		: EntryEMPWMDetail()

  	PURPOSE				: Show PWM detail screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryEMPWMDetail(void) 
{
#ifdef MMI_ON_HARDWARE_P
	U8* guiBuffer;
	U8* inputBuffer;
	U16 inputBufferSize;
       U16 ImageList[7];
       U8  textbuf[10];
              
	EntryNewScreen(EM_DEV_PWM_VIEWER_SCR_ID, ExitEMPWMDetail, NULL, NULL);
	InitializeCategory57Screen();
	
	guiBuffer = GetCurrGuiBuffer(EM_DEV_PWM_VIEWER_SCR_ID);
	
	inputBuffer = GetCurrNInputBuffer(EM_DEV_PWM_VIEWER_SCR_ID, &inputBufferSize);
 
	RegisterHighlightHandler(EMPWMHighlightHandler);

       // Get the current setting
	currPWM = currentHighlightIndex;
       currPWMLevel = custom_em_pwm_get_hw_current_level(currPWM) - 1; // 0-based
       if (currPWMLevel >= CUSTOM_EM_PWM_LEVEL_MAX || currPWMLevel < 0)
         currPWMLevel = 0;
       	
       custom_em_pwm_get_value((kal_uint16 *) PWMFreq, (kal_uint8 *) PWMDuty);       

       if (currPWM == 0)
       {
          currPWMFreq = PWMFreq[currPWM] = PWM1_Level_Info[currPWMLevel][0];
          currPWMDuty = PWMDuty[currPWM] = PWM1_Level_Info[currPWMLevel][1];       	
       } 
       else if (currPWM == 1)
       {
          currPWMFreq = PWMFreq[currPWM] = PWM2_Level_Info[currPWMLevel][0];
          currPWMDuty = PWMDuty[currPWM] = PWM2_Level_Info[currPWMLevel][1];       	
       } 
       else if (currPWM == 2)
       {
          currPWMFreq = PWMFreq[currPWM] = Alter_Level_Info[currPWMLevel][0];
          currPWMDuty = PWMDuty[currPWM] = Alter_Level_Info[currPWMLevel][1];       	
       } 
       
	// PWM
	SetInlineItemActivation(&wgui_inline_items[0], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
       sprintf((S8*)textbuf, "PWM %d", currPWM+1);		
	AnsiiToUnicodeString((S8*)unicodebuf, (S8*)textbuf);
	SetInlineItemDisplayOnly(&wgui_inline_items[0], unicodebuf);
       ImageList[0] = IMG_STATUS;

	SetInlineItemActivation(&wgui_inline_items[1], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&(wgui_inline_items[1]), (U8*)GetString(EM_STR_PWM_LEVEL));
       ImageList[1] = IMG_STATUS;

       //Level
       /*Wangzl Modify Start For 6358 Ver: TBM780  on 2007-9-27 13:39 */
       #if 0
       kal_wstrcpy((WCHAR*) EMPWMLevel[0], (const WCHAR *) kal_toWCHAR("1"));
       pEMPWMLevel[0] = (U8*) (EMPWMLevel[0]);
       kal_wstrcpy((WCHAR*) EMPWMLevel[1], (const WCHAR *) kal_toWCHAR("2"));
       pEMPWMLevel[1] = (U8*) (EMPWMLevel[1]);
       kal_wstrcpy((WCHAR*) EMPWMLevel[2], (const WCHAR *) kal_toWCHAR("3"));
       pEMPWMLevel[2] = (U8*) (EMPWMLevel[2]);
       kal_wstrcpy((WCHAR*) EMPWMLevel[3], (const WCHAR *) kal_toWCHAR("4"));
       pEMPWMLevel[3] = (U8*) (EMPWMLevel[3]);
       kal_wstrcpy((WCHAR*) EMPWMLevel[4], (const WCHAR *) kal_toWCHAR("5"));
       pEMPWMLevel[4] = (U8*) (EMPWMLevel[4]);
	#else
	sprintf( textbuf, "%s", "1" );
	AnsiiToUnicodeString((S8 *)EMPWMLevel[0], (S8 *)textbuf);
	pEMPWMLevel[0] = (U8*) (EMPWMLevel[0]);
	sprintf( textbuf, "%s", "2" );
	AnsiiToUnicodeString((S8 *)EMPWMLevel[1], (S8 *)textbuf);
	pEMPWMLevel[1] = (U8*) (EMPWMLevel[1]);
	sprintf( textbuf, "%s", "3" );
	AnsiiToUnicodeString((S8 *)EMPWMLevel[2], (S8 *)textbuf);
	pEMPWMLevel[2] = (U8*) (EMPWMLevel[2]);
	sprintf( textbuf, "%s", "4" );
	AnsiiToUnicodeString((S8 *)EMPWMLevel[3], (S8 *)textbuf);
	pEMPWMLevel[3] = (U8*) (EMPWMLevel[3]);
	sprintf( textbuf, "%s", "5" );
	AnsiiToUnicodeString((S8 *)EMPWMLevel[4], (S8 *)textbuf);
	pEMPWMLevel[4] = (U8*) (EMPWMLevel[4]);
	#endif
       /*Wangzl Modify End  For 6358 Ver: TBM780  on 2007-9-27 13:39 */
	SetInlineItemActivation(&wgui_inline_items[2], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemSelect(&wgui_inline_items[2], 5, (U8**)&pEMPWMLevel, (S32*)&currPWMLevel);
       ImageList[2] = 0;
       RegisterInlineSelectHighlightHandler(&wgui_inline_items[2], PWMLevelHighlightHandler);	
	
	SetInlineItemActivation(&wgui_inline_items[3], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&(wgui_inline_items[3]), (U8*)GetString(EM_STR_PWM_FREQUENCY));
       ImageList[3] = IMG_STATUS;

       // Frequency
	SetInlineItemActivation(&wgui_inline_items[4], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
       sprintf((S8*)textbuf, "%d", (U16)currPWMFreq);		
	AnsiiToUnicodeString((S8*)freqBuf, (S8*)textbuf);
       SetInlineItemTextEdit(&wgui_inline_items[4], (PU8)freqBuf, 20, INPUT_TYPE_NUMERIC);
       ImageList[4] = 0;

	SetInlineItemActivation(&wgui_inline_items[5], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&(wgui_inline_items[5]), (U8*)GetString(EM_STR_PWM_DUTY));
       ImageList[5] = IMG_STATUS;

       // Duty
       SetInlineItemActivation(&wgui_inline_items[6], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
       sprintf((S8*)textbuf, "%d", (U16)currPWMDuty);		
	AnsiiToUnicodeString((S8*)dutyBuf, (S8*)textbuf);
       SetInlineItemTextEdit(&wgui_inline_items[6], (PU8)dutyBuf, 20, INPUT_TYPE_NUMERIC);
       ImageList[6] = 0;

       if(inputBuffer!=NULL) SetCategory57Data(wgui_inline_items, 7, inputBuffer); 
       
       ShowCategory57Screen(EM_STR_PWM_EDITOR, EM_DEV_GPIO_SET_ICON, 
       	                STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
				  7, ImageList, wgui_inline_items, 0, guiBuffer);	

	SetCategory57RightSoftkeyFunctions(EMPWMDoneFunc, GoBackHistory);
#endif
}


/**************************************************************
**	FUNCTION NAME		: LSKEmPWMStop
**
**  	PURPOSE				: LSK handler function for PWM stop menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void LSKEmPWMStop(void)
{
#ifdef MMI_ON_HARDWARE_P
   custom_em_pwm_stop(PWMSetting.Type);
   custom_em_pwm_set_level( PWMSetting.Type, custom_em_pwm_get_hw_current_level(PWMSetting.Type) );
#endif
   GoBackHistory();
}


/**************************************************************
**	FUNCTION NAME		: EntryEmPWMSetParam
**
**  	PURPOSE				: Entry function for PWM parameter setting menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmPWMSetParam(void)
{
	U32 title=EM_DEV_PWM_SET_FREQ_TEXT;
	byte *history_buffer;

	EntryNewScreen(EM_DEV_PWM_EDITOR_SCR_ID, NULL, NULL, NULL);

   history_buffer=GetCurrGuiBuffer(EM_DEV_PWM_EDITOR_SCR_ID);

#ifdef MMI_ON_HARDWARE_P
   if(PWMSetting.Count==0)
   {
     title = EM_DEV_PWM_SET_FREQ_TEXT;
     kal_wsprintf( InputBuf, "%d", PWMSetting.Param.freq[currentHighlightIndex] );
   }
   else
   {
	  title = EM_DEV_PWM_SET_DUTY_TEXT;
      kal_wsprintf( InputBuf, "%d", PWMSetting.Param.duty_cycle[currentHighlightIndex] );
   }
#endif

   ShowCategory5Screen( (U16)title,  0, \
                        STR_GLOBAL_OK, 0 , \
                        STR_GLOBAL_BACK, 0, \
                        INPUT_TYPE_NUMERIC, \
                        (U8*)InputBuf , 8 , history_buffer );

	SetLeftSoftkeyFunction(LSKEmPWMMenu,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_END,KEY_EVENT_UP);
	//SetCategory16RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}

#undef ___EM_DEVICE_PWM___



/****************************************
 **
 ** Menu Tree: Device --> EINT
 **
 ****************************************/
#define ___EM_DEVICE_EINT___


/**************************************************************
**	FUNCTION NAME		: HighlightEmEINT
**
**  	PURPOSE				: Highlight function for EINT menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmEINT(void)
{
	SetLeftSoftkeyFunction(EntryEmEINTMenu,KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EmPrepareEINTList
**
**  	PURPOSE				: Helper function for EINT list preparation
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmPrepareEINTList(void)
{
   U16 i;
   S8 buffer[32];

   gMenuListNum=0;
   for(i=0; i<=EINT_LABELID_MAX;i++)
   {
      if( eint_map_tbl[i].vaild == GPIO_VAILD )
      {

      sprintf(buffer,"%s %d", eint_map_tbl[i].netname, 0);
      AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
      subMenuDataPtrs[i] = subMenuData[i];
      gMenuListNum++;
      
        }
   }

#ifdef MMI_ON_HARDWARE_P
   //Initialize charger value
   if( g_charbat_context.isChargerConnected == 1)
   {
         sprintf(buffer,"%s %d", eint_map_tbl[0].netname, 1);
         AnsiiToUnicodeString((PS8)subMenuData[0], buffer);
         subMenuDataPtrs[0] = subMenuData[0];
   }
   //Initialize earphone value
   // if( prevMode == AUD_MODE_HEADSET)
   extern U8 GetHandsetInPhone();
   if (1 == GetHandsetInPhone())
   {
         sprintf(buffer,"%s %d", eint_map_tbl[3].netname, 1);
         AnsiiToUnicodeString((PS8)subMenuData[3], buffer);
         subMenuDataPtrs[3] = subMenuData[3];
   }
#endif
      
}


/**************************************************************

	FUNCTION NAME		: EntryEmEINTMenu()

  	PURPOSE				: Entry function for EINT menun

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EntryEmEINTMenu(void)
{
    U8* guiBuffer;

	EntryNewScreen(EM_DEV_EINT_MENU_ID, ExitEmEINTMenu, NULL, NULL);
	guiBuffer = GetCurrGuiBuffer (EM_DEV_EINT_MENU_ID);		 

	//RegisterHighlightHandler (HighlightOnCertainEmPWM);

   EmPrepareEINTList();

   if(gMenuListNum<1) gMenuListNum++;

   ShowCategory6Screen (EM_DEV_EINT_TEXT, (U16)0,
						  0, (U16)0,
						  STR_GLOBAL_BACK, (U16)0,
						  gMenuListNum, (PU8*)subMenuDataPtrs,NULL, 0,
						  guiBuffer); 

	SetLeftSoftkeyFunction (NULL, KEY_EVENT_UP);
	SetRightSoftkeyFunction (StopEmEINTMeun, KEY_EVENT_UP);

     //EINTProtocolEventHandlerFp[0] = EmGetProtocolEventHandler(MSG_ID_MMI_EQ_GPIO_DETECT_IND ) );
    // EINTProtocolEventHandlerFp[1] = EmGetProtocolEventHandler(MSG_ID_MMI_EQ_BATTERY_STATUS_IND ) );
#ifdef MMI_ON_HARDWARE_P
     SetProtocolEventHandler( UpdateEmEINTGPIORecvHandler, MSG_ID_MMI_EQ_GPIO_DETECT_IND );
 	SetProtocolEventHandler( UpdateEmEINTBatteryRecvHandler, PRT_BATTERY_STATUS_IND );
#endif

 	//StartTimer(203, 1000, TestUpdateEmEINT);
}


/**************************************************************

	FUNCTION NAME		: UpdateEmEINTGPIORecvHandler

  	PURPOSE				: Helper function for EINT GPIO Recv 

	INPUT PARAMETERS	: inMsg

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void UpdateEmEINTGPIORecvHandler(void *inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   S8 buffer[32];
   mmi_eq_gpio_detect_ind_struct *gpio_detect_struct = (mmi_eq_gpio_detect_ind_struct *) inMsg;

   switch(gpio_detect_struct->gpio_device)
   {
      case EXT_DEV_NONE:
         sprintf(buffer,"%s %d", eint_map_tbl[1].netname, 0);
         AnsiiToUnicodeString((PS8)subMenuData[1], buffer);
         subMenuDataPtrs[1] = subMenuData[1];

         sprintf(buffer,"%s %d", eint_map_tbl[2].netname, 0);
         AnsiiToUnicodeString((PS8)subMenuData[2], buffer);
         subMenuDataPtrs[2] = subMenuData[2];

           sprintf(buffer,"%s %d", eint_map_tbl[3].netname, 0);
         AnsiiToUnicodeString((PS8)subMenuData[3], buffer);
         subMenuDataPtrs[3] = subMenuData[3];
 
         break;
         
      case EXT_DEV_UART:
         if( gpio_detect_struct->on_off == KAL_TRUE )
         sprintf(buffer,"%s %d", eint_map_tbl[1].netname, 1);
         else
           sprintf(buffer,"%s %d", eint_map_tbl[1].netname, 0);
            
         AnsiiToUnicodeString((PS8)subMenuData[1], buffer);
         subMenuDataPtrs[1] = subMenuData[1];
         break;
         
      case EXT_DEV_KEY_1:
         sprintf(buffer,"%s %d", eint_map_tbl[2].netname, 1);
         AnsiiToUnicodeString((PS8)subMenuData[2], buffer);
         subMenuDataPtrs[2] = subMenuData[2];
         break;

      case EXT_DEV_KEY_2:
         sprintf(buffer,"%s %d", eint_map_tbl[2].netname, 0);
         AnsiiToUnicodeString((PS8)subMenuData[2], buffer);
         subMenuDataPtrs[2] = subMenuData[2];
         break;

      case EXT_DEV_EARPHONE:
         if( gpio_detect_struct->on_off == KAL_TRUE )
         sprintf(buffer,"%s %d", eint_map_tbl[3].netname, 1);
         else
           sprintf(buffer,"%s %d", eint_map_tbl[3].netname, 0);
            
         AnsiiToUnicodeString((PS8)subMenuData[3], buffer);
         subMenuDataPtrs[3] = subMenuData[3];
         break;
         
      case EXT_DEV_CLAM_OPEN:
        return; /* Not yet define */
        break;
      
      case EXT_DEV_CLAM_CLOSE:
        return; /* Not yet define */
        break;

      default:
         return;
   }

   UpdateEmEINT();
#endif
}


/**************************************************************
**	FUNCTION NAME		: EMFMBatteryStatusMsgHandler
**
**  	PURPOSE				: Hanldery function for EM/FM Battery Status
**
**	INPUT PARAMETERS	: inMsg
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EMFMBatteryStatusMsgHandler( void  *inMsg )
{

#ifdef MMI_ON_HARDWARE_P
   mmi_eq_battery_status_ind_struct *BatState = (mmi_eq_battery_status_ind_struct *) inMsg;
   switch(BatState->battery_status)
   {
         case PMIC_CHARGER_IN:
      	    g_charbat_context.isChargerConnected=1;
      	    g_charbat_context.ChargingComplete = 0;  
      	    IdleScreenChargerConnected(KAL_TRUE);
              break;
         
          case PMIC_CHARGER_OUT:
      	    g_charbat_context.ChargingComplete = 0;  
      	    g_charbat_context.isChargerConnected=0;
      	    IdleScreenChargerDisconnected();
              break;

         case PMIC_CHARGE_COMPLETE:
            IdleScreenBatteryFullCharged();
               break;
                  
         case PMIC_VBAT_STATUS:
		  BatteryStatusIndication(BatState->battery_voltage);
		  break;
            
         case PMIC_OVERVOLPROTECT:
         case PMIC_OVERBATTEMP:
         case PMIC_LOWBATTEMP:
         case PMIC_OVERCHARGECURRENT:
	    case PMIC_INVALID_BATTERY:
         case PMIC_INVALID_CHARGER:
         case PMIC_CHARGING_TIMEOUT:
         case PMIC_LOWCHARGECURRENT:
			break;

         default:
             BatteryStatusRsp(BatState);
         return;
   }

#endif

}


/**************************************************************
**	FUNCTION NAME		: UpdateEmEINTBatteryRecvHandler
**
**  	PURPOSE				: Helper function for EINT Battery Status
**
**	INPUT PARAMETERS	: inMsg
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void UpdateEmEINTBatteryRecvHandler(void *inMsg)
{
#ifdef MMI_ON_HARDWARE_P

	S8 buffer[32];
   
   mmi_eq_battery_status_ind_struct *batstat_struct = (mmi_eq_battery_status_ind_struct*) inMsg;

   switch(batstat_struct->battery_status)
   {

         case PMIC_CHARGER_IN:
         {
         sprintf(buffer,"%s %d", eint_map_tbl[0].netname, 1);
         AnsiiToUnicodeString((PS8)subMenuData[0], buffer);
         subMenuDataPtrs[0] = subMenuData[0];
            break;
   }
         
          case PMIC_CHARGER_OUT:
   {
         sprintf(buffer,"%s %d", eint_map_tbl[0].netname, 0 );
         AnsiiToUnicodeString((PS8)subMenuData[0], buffer);
         subMenuDataPtrs[0] = subMenuData[0];
            break;
   }

         default:
            EMFMBatteryStatusMsgHandler(inMsg);
      return;
   }

   EMFMBatteryStatusMsgHandler(inMsg);
   UpdateEmEINT();
   
#endif
}


/**************************************************************
**	FUNCTION NAME		: UpdateEmEINT
**
**  	PURPOSE				: Helper function for EINT updating
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void UpdateEmEINT(void)
{
   ShowCategory6Screen (EM_DEV_EINT_TEXT, (U16)0,
						  0, (U16)0,
						  STR_GLOBAL_BACK, (U16)0,
						  gMenuListNum, (PU8*)subMenuDataPtrs,NULL, 0,
						  NULL);

   SetRightSoftkeyFunction (StopEmEINTMeun, KEY_EVENT_UP);
   ReplaceNewScreenHandler(EM_DEV_EINT_MENU_ID, ExitEmEINTMenu, NULL);
}


/**************************************************************
**	FUNCTION NAME		: StopEmEINTMeun
**
**  	PURPOSE				: Helper function for EINT stop
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void StopEmEINTMeun(void)
{
#ifdef MMI_ON_HARDWARE_P
   //SetProtocolEventHandler( EINTProtocolEventHandlerFp[0], MSG_ID_MMI_EQ_GPIO_DETECT_IND);
	//SetProtocolEventHandler( EINTProtocolEventHandlerFp[1], PRT_BATTERY_STATUS_IND);
	SetProtocolEventHandler( GpioDetectInd, MSG_ID_MMI_EQ_GPIO_DETECT_IND);
	SetProtocolEventHandler(BatteryStatusRsp,PRT_BATTERY_STATUS_IND);
#endif
   GoBackHistory();
}


/**************************************************************
**	FUNCTION NAME		: ExitEmEINTMenu
**
**  	PURPOSE				: Exitr function for EINT menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitEmEINTMenu(void)
{
#ifdef MMI_ON_HARDWARE_P
	SetProtocolEventHandler( GpioDetectInd, MSG_ID_MMI_EQ_GPIO_DETECT_IND);
	SetProtocolEventHandler(BatteryStatusRsp,PRT_BATTERY_STATUS_IND);
#endif
}
#undef ___EM_DEVICE_PWM___



/****************************************
 **
 ** Menu Tree: Device --> ADC
 **
 ****************************************/
#define ___EM_DEVICE_ADC___


/**************************************************************
**	FUNCTION NAME		: HighlightEmADC
**
**  	PURPOSE				: Highlight function for ADC menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmADC(void)
{
	SetLeftSoftkeyFunction(EntryEmADCMenu,KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************
**	FUNCTION NAME		: EmPrepareADCList
**
**  	PURPOSE				: Helpert function for ADC preparation
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
extern GPIO_MAP_ENTRY adc_map_tbl[];
void EmPrepareADCList(void)
{
   U16 i;
   S8 buffer[32];

   gMenuListNum=0;
   for(i=0; i<=ADC_LABELID_MAX;i++)
   {
      if( adc_map_tbl[i].vaild == GPIO_VAILD )
       { 

      sprintf(buffer,"%s %d", adc_map_tbl[i].netname, 0);
      AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
      subMenuDataPtrs[i] = subMenuData[i];
      gMenuListNum++;
       }
   }
}


/**************************************************************
**	FUNCTION NAME		: EntryEmADCMenu
**
**  	PURPOSE				: Entry function for ADC menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmADCMenu(void)
{
	U8* guiBuffer;

	EntryNewScreen(EM_DEV_ADC_MENU_ID, ExitEmADCMenu, NULL, NULL);
	guiBuffer = GetCurrGuiBuffer (EM_DEV_ADC_MENU_ID);		 

	//RegisterHighlightHandler (HighlightOnCertainEmPWM);

   EmPrepareADCList();

   if(gMenuListNum<1) gMenuListNum++;

   ShowCategory6Screen (EM_DEV_ADC_TEXT, (U16)0,
						  0, (U16)0,
						  STR_GLOBAL_BACK, (U16)0,
						  gMenuListNum, (PU8*)subMenuDataPtrs,NULL, 0,
						  guiBuffer); 

	SetLeftSoftkeyFunction (NULL, KEY_EVENT_UP);
	SetRightSoftkeyFunction (StopEmADCMenu, KEY_EVENT_UP);

#ifdef MMI_ON_HARDWARE_P
   SetProtocolEventHandler( EMFMBatteryStatusMsgHandler, PRT_BATTERY_STATUS_IND);
   SetProtocolEventHandler( UpdateEmADC, MSG_ID_MMI_EQ_ADC_ALL_CHANNEL_IND );
   SendEmADCStartReq();   
#endif
 	
}



/**************************************************************
**	FUNCTION NAME		: SendEmADCStartReq
**
**  	PURPOSE				: helper function for ADC measurement start request
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SendEmADCStartReq(void)
{
#ifdef MMI_ON_HARDWARE_P
   EngineerModeSendMsg(MSG_ID_MMI_EQ_START_ADC_ALL_CHANNEL_REQ, NULL, NULL);
#endif
}


/**************************************************************
**	FUNCTION NAME		: SendEmADCStopReq
**
**  	PURPOSE				: helper function for ADC measurement stop request
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SendEmADCStopReq(void)
{
#ifdef MMI_ON_HARDWARE_P
   EngineerModeSendMsg(MSG_ID_MMI_EQ_STOP_ADC_ALL_CHANNEL_REQ, NULL, NULL);
#endif
}


/**************************************************************
**	FUNCTION NAME		: UpdateEmADC
**
**  	PURPOSE				: helper function for ADC update
**
**	INPUT PARAMETERS	: inMsg
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void UpdateEmADC(void* inMsg)
{
 
#if 0
#ifdef MMI_ON_HARDWARE_P
   U8 i=0, j=0;
   S8 buffer[32];
   double adc_value=0.0;
   mmi_eq_adc_all_channel_ind_struct *adc_struct = (mmi_eq_adc_all_channel_ind_struct *) inMsg;

   if( adc_map_tbl[j].vaild == GPIO_VAILD )
   {
      adc_value = (double)adc_struct->vbat;
      sprintf(buffer,"%s %4.2f V", adc_map_tbl[j].netname,  adc_value/1000000.0 );

      AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
      subMenuDataPtrs[i] = subMenuData[i];
      i++;
    }
    
   j++;
   if( adc_map_tbl[j].vaild == GPIO_VAILD )
   {
      adc_value = (double)adc_struct->bat_temp;
      sprintf(buffer,"%s %4.2f C", adc_map_tbl[j].netname,  adc_value/1000.0 );

      AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
      subMenuDataPtrs[i] = subMenuData[i];
      i++;
   }

   j++;
   if( adc_map_tbl[j].vaild == GPIO_VAILD )
   {
      adc_value = (double)adc_struct->vaux;
      sprintf(buffer,"%s %4.2f V", adc_map_tbl[j].netname,  adc_value/1000000.0 );

      AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
      subMenuDataPtrs[i] = subMenuData[i];
      i++;
   }
   
   j++;
   if( adc_map_tbl[j].vaild == GPIO_VAILD )
   {
      adc_value = (double) adc_struct->charge_current;
      if( (adc_struct->charge_current)  & 0x80000000 )
        sprintf(buffer,"%s n/a",  adc_map_tbl[j].netname );
      else
        sprintf(buffer,"%s %4.2f A", adc_map_tbl[j].netname, (adc_value)/1000000.0 );

      AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
      subMenuDataPtrs[i] = subMenuData[i];
      i++;
    }
    
   j++;  
   if( adc_map_tbl[j].vaild == GPIO_VAILD )
   {
      adc_value = (double)adc_struct->vcharge;
      sprintf(buffer,"%s %4.2f V", adc_map_tbl[j].netname,  adc_value/1000000.0 );
      
      AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
      subMenuDataPtrs[i] = subMenuData[i];
      i++;
   }
   
   ShowCategory6Screen (EM_DEV_ADC_TEXT, 0,
						  NULL, 0,
						  STR_GLOBAL_BACK, 0,
						  i/*gMenuListNum*/, (PU8*)subMenuDataPtrs,NULL, 0,
						  NULL);

   SetRightSoftkeyFunction (StopEmADCMenu, KEY_EVENT_UP);
   ReplaceNewScreenHandler(EM_DEV_EINT_MENU_ID, ExitEmADCMenu, NULL);

#endif  
#endif  
}


/**************************************************************
**	FUNCTION NAME		: StopEmADCMenu
**
**  	PURPOSE				: helper function to stop ADC measurement
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void StopEmADCMenu(void)
{
   //StopTimer(204);
#ifdef MMI_ON_HARDWARE_P
   SetProtocolEventHandler( NULL, MSG_ID_MMI_EQ_ADC_ALL_CHANNEL_IND );
   SetProtocolEventHandler(BatteryStatusRsp, PRT_BATTERY_STATUS_IND);
   SendEmADCStopReq();
#endif
   GoBackHistory();
}


/**************************************************************
**	FUNCTION NAME		: ExitEmADCMenu
**
**  	PURPOSE				: Exit function to ADC menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitEmADCMenu(void)
{
 	//StopTimer(204);
#ifdef MMI_ON_HARDWARE_P
 	SetProtocolEventHandler( NULL, MSG_ID_MMI_EQ_ADC_ALL_CHANNEL_IND );
	SetProtocolEventHandler(BatteryStatusRsp, PRT_BATTERY_STATUS_IND);
     SendEmADCStopReq();
#endif
}

#undef  ___EM_DEVICE_ADC___



/****************************************
 **
 ** Menu Tree: Device --> Clam
 **
 ****************************************/
 #define ___EM_DEVICE_Clam___


/**************************************************************
**	FUNCTION NAME		: HighlightEmClam
**
**  	PURPOSE				: Highlight function for Clam menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmClam(void)
{
	SetLeftSoftkeyFunction(EntryClamMenu,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryClamMenu
**
**  	PURPOSE				: Entry function for Clam menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryClamMenu(void)
{
    U8* guiBuffer;
	S8 buffer[16];

	EntryNewScreen(EM_DEV_CLAM_MENUID, NULL, NULL, NULL);
	guiBuffer = GetCurrGuiBuffer (EM_DEV_CLAM_MENUID);		 
	gMenuListNum=0;

    sprintf(buffer,"ClamOpen %u", gClamOpenCnt);
    AnsiiToUnicodeString((PS8)subMenuData[0], buffer);
    subMenuDataPtrs[0] = subMenuData[0];
    gMenuListNum++;

    sprintf(buffer,"ClamClose %u", gClamCloseCnt);
    AnsiiToUnicodeString((PS8)subMenuData[1], buffer);
    subMenuDataPtrs[1] = subMenuData[1];
    gMenuListNum++;

    ShowCategory6Screen (EM_DEV_CLAM_TEXT, (U16)0,
						  0, (U16)0,
						  STR_GLOBAL_BACK, (U16)0,
						  gMenuListNum, (PU8*)subMenuDataPtrs,NULL, 0,
						  guiBuffer); 

	SetLeftSoftkeyFunction (NULL, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
}
 #undef ___EM_DEVICE_Clam___




/****************************************
 **
 ** Menu Tree: Device --> HW Level
 **
 ****************************************/
  #define ___EM_DEVICE_SetHwLevel___


/**************************************************************
**	FUNCTION NAME		: HighlightEmHWLevel
**
**  	PURPOSE				: Highlight function for Hardware Level menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmHWLevel(void)
{
	SetLeftSoftkeyFunction(EntryHWLevelMenu,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryHWLevelMenu
**
**  	PURPOSE				: Entry function for Hardware Level menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryHWLevelMenu(void)
{
	U16		ItemList[16];
	U16		ItemIcons[16];
	U16		nItems;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;


	EntryNewScreen(EM_DEV_HW_LEVEL_MENUID, NULL, EntryHWLevelMenu, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_DEV_HW_LEVEL_MENUID);
	nItems = GetNumOfChild(EM_DEV_HW_LEVEL_MENUID);
	GetSequenceStringIds(EM_DEV_HW_LEVEL_MENUID,ItemList);
	SetParentHandler(EM_DEV_HW_LEVEL_MENUID);

       ClearAllKeyHandler();

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	ShowCategory52Screen(EM_DEV_HW_LEVEL_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetLeftSoftkeyFunction(GetHightlightAndEnterHWList,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: GetHightlightAndEnterHWList
**
**  	PURPOSE				: Helper function for Hardware Level list
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void GetHightlightAndEnterHWList(void)
{
	U8 idx=0;

	idx = (U8)GetHighlightedItem();

	switch( idx )
	{
	case EM_HWLEVEL_MENU_MAINLCD_CNTST: /* Main LCD Contrast */
		gHWLevelMenuID = EM_DEV_HW_LEVEL_MLCD_CON_MENUID;
		break;
#ifdef __MMI_SUBLCD__
	case EM_HWLEVEL_MENU_SUBLCD_CNTST: /* Sub LCD Contrast */
		gHWLevelMenuID = EM_DEV_HW_LEVEL_SLCD_CON_MENUID;
		break;
#endif
	case EM_HWLEVEL_MENU_BATTERY: /* Battery */
		gHWLevelMenuID = EM_DEV_HW_LEVEL_BATTERY_MENUID;
		break;
	case EM_HWLEVEL_MENU_PWM_1: /* PWM 1 */
		gHWLevelMenuID = EM_DEV_HW_LEVEL_PWM1_MENUID;
		break;
	case EM_HWLEVEL_MENU_PWM_2: /* PWM 2 */
		gHWLevelMenuID = EM_DEV_HW_LEVEL_PWM2_MENUID;
		break;
	case EM_HWLEVEL_MENU_PWM_3: /* PWM 3 */
		gHWLevelMenuID = EM_DEV_HW_LEVEL_PWM3_MENUID;
		break;

	default:
		break;
	}

	PrepareHWLevelList();
}


/**************************************************************
**	FUNCTION NAME		: PrepareHWLevelList
**
**  	PURPOSE				: Helper function for Hardware Level list preparation
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void PrepareHWLevelList(void)
{
    U8 i=0;
    S8 buffer[32];

	gMenuListNum = 0;
	memset( subMenuDataPtrs, 0, sizeof(subMenuDataPtrs) );
	memset( subMenuData, 0, sizeof(subMenuData) );

	switch( gHWLevelMenuID )
	{
	case EM_DEV_HW_LEVEL_MLCD_CON_MENUID: /* Main LCD Contrast */
		for( i=0; i<LCD_CONTRAST_MAX_LEVEL; i++ )
		{
		   sprintf(buffer,"[LEV %2d] %d", (i+1), MainLCD_Contrast_Level_Info[i] );
		   AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
		   subMenuDataPtrs[i] = subMenuData[i];
		   gMenuListNum++;
		}
		break;
	case EM_DEV_HW_LEVEL_SLCD_CON_MENUID: /* Sub LCD Contrast */
		for( i=0; i<LCD_CONTRAST_MAX_LEVEL; i++ )
		{
		   sprintf(buffer,"[LEV %2d] %d", (i+1), SubLCD_Contrast_Level_Info[i] );
		   AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
		   subMenuDataPtrs[i] = subMenuData[i];
		   gMenuListNum++;
		}
		break;
	case EM_DEV_HW_LEVEL_BATTERY_MENUID: /* Battery */
		for( i=0; i<BATTERY_MAX_LEVEL; i++ )
		{
		   sprintf(buffer,"LEV%2d:%d", (i+1), Battery_Level_Info[i] );
		   AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
		   subMenuDataPtrs[i] = subMenuData[i];
		   gMenuListNum++;
		}
		break;
	case EM_DEV_HW_LEVEL_PWM1_MENUID: /* PWM 1 */
		for( i=0; i<PWM_MAX_LEVEL; i++  )
		{
		   sprintf(buffer,"[Freq %2d] %u", (i+1), (U32)PWM1_Level_Info[i][0] );
		   AnsiiToUnicodeString((PS8)subMenuData[gMenuListNum], buffer);
		   subMenuDataPtrs[gMenuListNum] = subMenuData[gMenuListNum];
		   gMenuListNum++;

		   sprintf(buffer,"[Duty %2d] %u", (i+1), (U32)PWM1_Level_Info[i][1] );
		   AnsiiToUnicodeString((PS8)subMenuData[gMenuListNum], buffer);
		   subMenuDataPtrs[gMenuListNum] = subMenuData[gMenuListNum];
		   gMenuListNum++;
		}
		break;
	case EM_DEV_HW_LEVEL_PWM2_MENUID: /* PWM 2 */
		for( i=0; i<PWM_MAX_LEVEL; i++  )
		{
		   sprintf(buffer,"[Freq %2d] %u", (i+1), (U32)PWM2_Level_Info[i][0] );
		   AnsiiToUnicodeString((PS8)subMenuData[gMenuListNum], buffer);
		   subMenuDataPtrs[gMenuListNum] = subMenuData[gMenuListNum];
		   gMenuListNum++;

		   sprintf(buffer,"[Duty %2d] %u", (i+1), (U32)PWM2_Level_Info[i][1] );
		   AnsiiToUnicodeString((PS8)subMenuData[gMenuListNum], buffer);
		   subMenuDataPtrs[gMenuListNum] = subMenuData[gMenuListNum];
		   gMenuListNum++;
		}
		break;
	case EM_DEV_HW_LEVEL_PWM3_MENUID: /* PWM 3 */
		for( i=0; i<PWM_MAX_LEVEL; i++  )
		{
		   sprintf(buffer,"[Freq %2d] %u", (i+1), (U32)Alter_Level_Info[i][0] );
		   AnsiiToUnicodeString((PS8)subMenuData[gMenuListNum], buffer);
		   subMenuDataPtrs[gMenuListNum] = subMenuData[gMenuListNum];
		   gMenuListNum++;

		   sprintf(buffer,"[Duty %2d] %u", (i+1), (U32)Alter_Level_Info[i][1] );
		   AnsiiToUnicodeString((PS8)subMenuData[gMenuListNum], buffer);
		   subMenuDataPtrs[gMenuListNum] = subMenuData[gMenuListNum];
		   gMenuListNum++;
		}
		break;

	default:
		break;
	}

	EntryHWLevelDisplyDetail();
}


/**************************************************************
**	FUNCTION NAME		: EntryHWLevelDisplyDetail
**
**  	PURPOSE				: Entry function for Hardware level display menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryHWLevelDisplyDetail()
{

    U8* guiBuffer;

	EntryNewScreen(gHWLevelMenuID, NULL, EntryHWLevelDisplyDetail, NULL);
	guiBuffer = GetCurrGuiBuffer (gHWLevelMenuID);		 

	//RegisterHighlightHandler(ExecuteCurrHiliteHandler);

     ShowCategory6Screen (EM_DEV_HW_LEVEL_TEXT, 0,
						  STR_GLOBAL_OK, 0,
						  STR_GLOBAL_BACK, 0,
						  gMenuListNum, (PU8*)subMenuDataPtrs,NULL, 0,
						  guiBuffer); 
	SetLeftSoftkeyFunction (EntryChangeHWLevel, KEY_EVENT_UP);
	SetRightSoftkeyFunction (CheckHWLevelInfoDirty, KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EmHWLevelInfoAccess
**
**  	PURPOSE				: Helper function for Hardware Level info access 
**
**	INPUT PARAMETERS	: Type, Item, action, value 
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmHWLevelInfoAccess( U16 Type, U8 Item, U8 action, S32 *value )
{
	switch( Type )
	{
	case EM_DEV_HW_LEVEL_MLCD_CON_MENUID:
		if( action == EM_HW_LEVEL_ACT_GET ) /* GET */
			*value = MainLCD_Contrast_Level_Info[Item];
		else  /* SET */
		{
			if( *value > 255 )
			 *value=255;
			MainLCD_Contrast_Level_Info[Item] = (U8)*value;
		}
		break;

    case EM_DEV_HW_LEVEL_SLCD_CON_MENUID:
		if( action == EM_HW_LEVEL_ACT_GET ) /* GET */
			*value = SubLCD_Contrast_Level_Info[Item];
		else  /* SET */
		{
			if( *value > 255 )
			 *value=255;
			SubLCD_Contrast_Level_Info[Item] = (U8)*value;
		}
		break;

	case EM_DEV_HW_LEVEL_BATTERY_MENUID:
		if( action == EM_HW_LEVEL_ACT_GET ) /* GET */
			*value = Battery_Level_Info[Item];
		else  /* SET */
		{
			if( *value > 9999999 )
			 *value=9999999;
			Battery_Level_Info[Item] = (U32)*value;
		}
		break;

	case EM_DEV_HW_LEVEL_PWM1_MENUID:
		if( action == EM_HW_LEVEL_ACT_GET ) /* GET */
			*value = PWM1_Level_Info[(Item/2)][(Item%2)];
		else  /* SET */
		{
			if( *value > 9999999 )
			 *value=9999999;
#ifdef MMI_ON_HARDWARE_P			
			PWM1_Level_Info[(Item/2)][(Item%2)] = (U32)*value;
#else
                     PWM1_Level_Info[(Item/2)][(Item%2)] = (kal_uint16)*value;
#endif
		}
		break;

	case EM_DEV_HW_LEVEL_PWM2_MENUID:
		if( action == EM_HW_LEVEL_ACT_GET ) /* GET */
			*value = PWM2_Level_Info[(Item/2)][(Item%2)];
		else  /* SET */
		{
			if( *value > 9999999 )
			 *value=9999999;
#ifdef MMI_ON_HARDWARE_P			
			PWM2_Level_Info[(Item/2)][(Item%2)] = (U32)*value;
#else
                     PWM2_Level_Info[(Item/2)][(Item%2)] = (kal_uint16)*value;
#endif
		}
		break;

	case EM_DEV_HW_LEVEL_PWM3_MENUID:
		if( action == EM_HW_LEVEL_ACT_GET ) /* GET */
			*value = Alter_Level_Info[(Item/2)][(Item%2)];
		else  /* SET */
		{
			if( *value > 9999999 )
			 *value=9999999;
#ifdef MMI_ON_HARDWARE_P			
			Alter_Level_Info[(Item/2)][(Item%2)] = (U32)*value;
#else
			Alter_Level_Info[(Item/2)][(Item%2)] = (kal_uint16)*value;
#endif
		}
		break;

	default:
		break;
	}	
}


/**************************************************************
**	FUNCTION NAME		: EntryChangeHWLevel
**
**  	PURPOSE				: Entry function for Hardware level change menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryChangeHWLevel(void)
{
	byte *history_buffer;
	S32 level;
	S8 ScrBuf[16];

//Lisen 09062005
	gHWLevelHilitItem = (U16)GetHighlightedItem();

	EntryNewScreen(EM_DEV_HW_LEVEL_EDITOR_SCR_ID, NULL, EntryChangeHWLevel, NULL);
	history_buffer=GetCurrGuiBuffer(EM_DEV_HW_LEVEL_EDITOR_SCR_ID);

	EmHWLevelInfoAccess( gHWLevelMenuID, (U8)gHWLevelHilitItem, EM_HW_LEVEL_ACT_GET, &level );
	sprintf( ScrBuf, "%d", level );
    AnsiiToUnicodeString((PS8)InputBuf, ScrBuf);
    ShowCategory5Screen( EM_DEV_LCD_SET_PARAM_1_TEXT,  0, \
                                          STR_GLOBAL_OK, 0 , \
                                          STR_GLOBAL_BACK, 0, \
                                          INPUT_TYPE_NUMERIC, \
                                          (U8*)InputBuf , 8 , history_buffer );

	SetLeftSoftkeyFunction(ApplyHWLevel,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_END,KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: ApplyHWLevel
**
**  	PURPOSE				: Helper function for Hardware Level application
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ApplyHWLevel(void)
{
	S8 ArgStrBuf[8];
	S32 Arg;

    UnicodeToAnsii( (S8*)ArgStrBuf , (S8*)InputBuf);
    Arg = atoi( (S8*) ArgStrBuf);

	EmHWLevelInfoAccess( gHWLevelMenuID, (U8)gHWLevelHilitItem, EM_HW_LEVEL_ACT_SET, &Arg );
	gIsHWLevelInfoDirty = 1;
	GoBackHistory();
}


/**************************************************************
**	FUNCTION NAME		: CheckHWLevelInfoDirty
**
**  	PURPOSE				: Helper function for Hardware Level info check
**
**	INPUT PARAMETERS	: Type, Item, action, value 
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void CheckHWLevelInfoDirty(void)
{
   if (gIsHWLevelInfoDirty)
   {
   	/*Wangzl Add Start For 6369 Ver: TBM780  on 2007-8-22 10:43 */
   	ClearAllKeyHandler();
   	/*Wangzl Add End  For 6369 Ver: TBM780  on 2007-8-22 10:43 */
      ShowCategory2Screen (EM_UPDATE_TITLE_TEXT, 0,
      						STR_GLOBAL_YES, 0,
      						STR_GLOBAL_NO, 0, 
      						EM_UPDATE_QUERY_TEXT, NULL);
      SetLeftSoftkeyFunction (SendSetHWLevelInfoReq, KEY_EVENT_UP);
      SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
      ReplaceNewScreenHandler(EM_UPDATE_TITLE_TEXT, NULL, NULL);
   }
   else
      GoBackHistory();

}


/**************************************************************
**	FUNCTION NAME		: SendSetHWLevelInfoReq
**
**  	PURPOSE				: Helper function to send Hardware Level info update request
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SendSetHWLevelInfoReq(void)
{

#ifdef MMI_ON_HARDWARE_P

	U8 i=0;
	mmi_eq_set_hw_level_req_struct *set_hw_struct;
    set_hw_struct = OslConstructDataPtr(sizeof(mmi_eq_set_hw_level_req_struct));

    for( i=0; i<5; i++)
    {
       set_hw_struct->hw_profile.PWM1_freq_Level_Info[i] = PWM1_Level_Info[i][0];
       set_hw_struct->hw_profile.PWM1_duty_Level_Info[i] = PWM1_Level_Info[i][1];

       set_hw_struct->hw_profile.PWM2_freq_Level_Info[i] = PWM2_Level_Info[i][0];
       set_hw_struct->hw_profile.PWM2_duty_Level_Info[i] = PWM2_Level_Info[i][1];

       set_hw_struct->hw_profile.PWM3_freq_Level_Info[i] = Alter_Level_Info[i][0];
       set_hw_struct->hw_profile.PWM3_duty_Level_Info[i] = Alter_Level_Info[i][1];
    }

    memcpy(set_hw_struct->hw_profile.MainLCD_Contrast_Level_Info, MainLCD_Contrast_Level_Info, sizeof(set_hw_struct->hw_profile.MainLCD_Contrast_Level_Info) );
    memcpy(set_hw_struct->hw_profile.MainLCD_Bias_Level_Info,     MainLCD_Bias_Level_Info,  sizeof(set_hw_struct->hw_profile.MainLCD_Bias_Level_Info) );
    memcpy(set_hw_struct->hw_profile.MainLCD_Linerate_Level_Info, MainLCD_Linerate_Level_Info,  sizeof(set_hw_struct->hw_profile.MainLCD_Linerate_Level_Info) );
    memcpy(set_hw_struct->hw_profile.MainLCD_Temp_Level_Info,     MainLCD_Temp_Level_Info,  sizeof(set_hw_struct->hw_profile.MainLCD_Temp_Level_Info) );

    memcpy(set_hw_struct->hw_profile.SubLCD_Contrast_Level_Info, SubLCD_Contrast_Level_Info, sizeof(set_hw_struct->hw_profile.SubLCD_Contrast_Level_Info) );
    memcpy(set_hw_struct->hw_profile.SubLCD_Bias_Level_Info,     SubLCD_Bias_Level_Info,  sizeof(set_hw_struct->hw_profile.SubLCD_Bias_Level_Info) );
    memcpy(set_hw_struct->hw_profile.SubLCD_Linerate_Level_Info, SubLCD_Linerate_Level_Info,  sizeof(set_hw_struct->hw_profile.SubLCD_Linerate_Level_Info) );
    memcpy(set_hw_struct->hw_profile.SubLCD_Temp_Level_Info,     SubLCD_Temp_Level_Info,  sizeof(set_hw_struct->hw_profile.SubLCD_Temp_Level_Info) );


    memcpy(set_hw_struct->hw_profile.Battery_Level_Info, Battery_Level_Info, sizeof(set_hw_struct->hw_profile.Battery_Level_Info) );

	EngineerModeSendMsg(MSG_ID_MMI_EQ_SET_HW_LEVEL_REQ, set_hw_struct, NULL);

#else 

    EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);

#endif /* MMI_ON_HARDWARE_P */

}


/**************************************************************
**	FUNCTION NAME		: SendSetHWLevelInfoRsp
**
**  	PURPOSE				: Helper function for reponse of Hardware Level info update request
**
**	INPUT PARAMETERS	: inMsg
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SendSetHWLevelInfoRsp(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_eq_set_hw_level_rsp_struct* rsp_p = inMsg;
   if (rsp_p->result)
   {
      EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);
   }
   else
      EngineerModeErrorNotice(EM_AUD_SET_ERROR_TEXT);
#endif
   gIsHWLevelInfoDirty = 0;
}
#undef ___EM_DEVICE_SetHwLevel___

#if defined(NAND_SUPPORT) //&& defined(MMI_ON_HARDWARE_P) 

void HighlightEmNANDFormat(void)
{
	SetLeftSoftkeyFunction(EntryEmNANDFormatMenu,KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

void EntryEmNANDFormatMenu(void)
{
    int drive = FS_GetDrive(FS_DRIVE_V_NORMAL, 1, FS_NO_ALT_DRIVE);
    int reval=0; 
    WCHAR	K_filename[20];
   
    if (drive > FS_NO_ERROR)
    {
#ifndef _NAND_FLASH_BOOTING_
	if (drive == 0x44)   // D
	{
          kal_wsprintf(K_filename, "\\\\.\\%c:", 'D'); 
          reval = FS_GeneralFormat (K_filename, FS_FORMAT_LOW, NULL); 
	}
#else
	if (drive == 0x43)   // C
	{
          kal_wsprintf(K_filename, "\\\\.\\%c:", 'C'); 
          reval = FS_GeneralFormat (K_filename, FS_FORMAT_LOW, NULL); 
	}
#endif

       if (reval >= FS_NO_ERROR)
          DisplayPopup( (U8*)GetString(STR_GLOBAL_DONE),  (U16) IMG_GLOBAL_OK, (U8) NULL, EM_NOTIFY_DURATION, (U8)SUCCESS_TONE );
       else
          DisplayPopup( (U8*)GetString(STR_GLOBAL_ERROR),  (U16) IMG_GLOBAL_ERROR, (U8) NULL, EM_NOTIFY_DURATION, (U8)ERROR_TONE );
   }
}
#endif

#ifdef __MMI_BT_SUPPORT__       /* __MMI_BT_SUPPORT__ */
#include "settingprofile.h"     /* for tone id */
//#include "btmmihpgprots.h"
#include "btmmiscrgprots.h"
#include "bluetooth_struct.h"

extern BOOL mmi_profiles_bt_connect_sco(void (*callback) (void));
extern BOOL mmi_profiles_bt_disconnect_sco(void);

#ifdef __MMI_BT_BCHS_SUPPORT__
extern kal_bool  bchs_debug_trace;
#endif


/*****************************************************************************
 * FUNCTION
 *  InitEngineerModeBT
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void InitEngineerModeBT(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetHiliteHandler(EM_BT_MENUID, HighlightEmBTMenu);

    /* general test */
    SetHiliteHandler(EM_BT_GENERNAL_TEST_MENUID, HighlightEmBTGeneralTestMenu);
    
    SetHiliteHandler(EM_BT_ENTRY_TEST_MENUID, HighlightEmBTTestMenu);
#ifdef __MMI_BT_BCHS_SUPPORT__	
	SetHiliteHandler(EM_BT_DEBUG_TRACE_MENUID, HighlightEmBTDebugTrace);
#endif
    SetHiliteHandler(EM_BT_ADD_AUDIO_CONNECTION_MENUID, HighlightEmBTAddAudioConnection);
    SetHiliteHandler(EM_BT_REMOVE_AUDIO_CONNECTION_MENUID, HighlightEmBTRemoveAudioConnection);
    SetHiliteHandler(EM_BT_ENTRY_LOOPBACK_MENUID, HighlightEmBTLoopback);

#if defined(__MMI_BT_COOLSAND_SUPPORT__)
    SetHiliteHandler(EM_BT_EXIT_TEST_MODE_MENUID, HighlightEmBTExitTestMode);
#endif

#if defined(__MMI_BT_COOLSAND_SUPPORT__) && defined(__MMI_AVRCP_DEBUG__) && defined(__MMI_AVRCP_CT_SUPPORT__)
    SetHiliteHandler(EM_BT_AVRCP_CMD_LIST_MENUID, HighlightEmBTAVRCPCmdList);
#endif


#ifdef __MMI_EM_BT_RF_TEST__
    SetHiliteHandler(EM_DEVICE_BT_RF_TEST_MENUID, HighlightEmDevBtRfTest);        

	#ifdef __BT_RDA5868__

	
    SetHiliteHandler(EM_DEVICE_RDA_BT_TEST_VCO, HighlightEmDevBtRfVCO);        
    SetHiliteHandler(EM_DEVICE_RDA_BT_TEST_SINGLEHOP, HighlightEmDevBtRfSingleHop);        
    SetHiliteHandler(EM_DEVICE_RDA_BT_TEST_CMU200, HighlightEmDevBtRfCMU200);        
    SetHiliteHandler(EM_DEVICE_RDA_BT_TEST_TC3000, HighlightEmDevBtRfTC3000);        
    SetHiliteHandler(EM_DEVICE_RDA_BT_TEST_N4010, HighlightEmDevBtRfN4010);        
	
   #else
    SetHiliteHandler(EM_DEVICE_BT_RF_TEST_TX_MENUID, HighlightEmDevBtRfTX);        
    SetHiliteHandler(EM_DEVICE_BT_RF_TEST_RX_MENUID, HighlightEmDevBtRfRX);        
    SetHiliteHandler(EM_DEVICE_BT_RF_TEST_BD_ADDR_MENUID, HighlightEmDevBtRfBdAddr);        
    SetHiliteHandler(EM_DEVICE_BT_RF_TEST_MODE_MENUID, HighlightEmDevBtRfBdEnterTestMode);        
	#endif

#endif /* __MMI_EM_BT_RF_TEST__ */


#ifdef __MMI_EM_BT_GET_CHIP_VERION__
    SetHiliteHandler(EM_BT_GET_CHIP_VERSION_MENUID, HighlightEmBTGetChipVersionMenu);
#endif

}


/*****************************************************************************
 * FUNCTION
 *  HighlightEmBTMenu
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void HighlightEmBTMenu(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
    ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
    SetLeftSoftkeyFunction(EntryEmBTMenu, KEY_EVENT_UP);
    SetKeyHandler(EntryEmBTMenu, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

#ifdef __MMI_FOURWAYS_KEY_DISABLE__
    SetKeyHandler(EntryEmBTMenu, KEY_VOL_UP, KEY_EVENT_DOWN);
    SetKeyHandler(GoBackHistory, KEY_VOL_DOWN, KEY_EVENT_DOWN);
#endif /* __MMI_FOURWAYS_KEY_DISABLE__ */ 

}


/*****************************************************************************
 * FUNCTION
 *  HighlightEmBTGeneralTestMenu
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void HighlightEmBTGeneralTestMenu(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
    ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
    SetLeftSoftkeyFunction(EntryEmBTGeneralTestMenu, KEY_EVENT_UP);
    SetKeyHandler(EntryEmBTGeneralTestMenu, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

#ifdef __MMI_FOURWAYS_KEY_DISABLE__
    SetKeyHandler(EntryEmBTGeneralTestMenu, KEY_VOL_UP, KEY_EVENT_DOWN);
    SetKeyHandler(GoBackHistory, KEY_VOL_DOWN, KEY_EVENT_DOWN);
#endif /* __MMI_FOURWAYS_KEY_DISABLE__ */ 

}


/*****************************************************************************
 * FUNCTION
 *  HighlightEmBTTestMenu
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void HighlightEmBTTestMenu(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
    ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
    SetLeftSoftkeyFunction(EntryEmBTTestMode, KEY_EVENT_UP);
    SetKeyHandler(NULL, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

#ifdef __MMI_FOURWAYS_KEY_DISABLE__
    SetKeyHandler(NULL, KEY_VOL_UP, KEY_EVENT_DOWN);
    SetKeyHandler(GoBackHistory, KEY_VOL_DOWN, KEY_EVENT_DOWN);
#endif /* __MMI_FOURWAYS_KEY_DISABLE__ */ 

}

#ifdef __MMI_BT_BCHS_SUPPORT__
/*****************************************************************************
 * FUNCTION
 *  HighlightEmBTDebugTrace
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void HighlightEmBTDebugTrace(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (bchs_debug_trace)
    {
        ChangeLeftSoftkey(STR_GLOBAL_OFF, 0);
    }
    else
    {
        ChangeLeftSoftkey(STR_GLOBAL_ON, 0);
    }

    ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
    SetLeftSoftkeyFunction(EntryEmBTDebugTrace, KEY_EVENT_UP);
    SetKeyHandler(NULL, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

#ifdef __MMI_FOURWAYS_KEY_DISABLE__
    SetKeyHandler(NULL, KEY_VOL_UP, KEY_EVENT_DOWN);
    SetKeyHandler(GoBackHistory, KEY_VOL_DOWN, KEY_EVENT_DOWN);
#endif /* __MMI_FOURWAYS_KEY_DISABLE__ */ 

}
#endif  /* __MMI_BT_BCHS_SUPPORT__ */




/*****************************************************************************
 * FUNCTION
 *  HighlightEmBTAddAudioConnection
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void HighlightEmBTAddAudioConnection(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
    ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
    SetLeftSoftkeyFunction(EntryEmBTAddAudioConnection, KEY_EVENT_UP);
    SetKeyHandler(NULL, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

#ifdef __MMI_FOURWAYS_KEY_DISABLE__
    SetKeyHandler(NULL, KEY_VOL_UP, KEY_EVENT_DOWN);
    SetKeyHandler(GoBackHistory, KEY_VOL_DOWN, KEY_EVENT_DOWN);
#endif /* __MMI_FOURWAYS_KEY_DISABLE__ */ 

}


/*****************************************************************************
 * FUNCTION
 *  HighlightEmBTRemoveAudioConnection
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void HighlightEmBTRemoveAudioConnection(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
    ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
    SetLeftSoftkeyFunction(EntryEmBTRemoveAudioConnection, KEY_EVENT_UP);
    SetKeyHandler(NULL, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

#ifdef __MMI_FOURWAYS_KEY_DISABLE__
    SetKeyHandler(NULL, KEY_VOL_UP, KEY_EVENT_DOWN);
    SetKeyHandler(GoBackHistory, KEY_VOL_DOWN, KEY_EVENT_DOWN);
#endif /* __MMI_FOURWAYS_KEY_DISABLE__ */ 

}


/*****************************************************************************
 * FUNCTION
 *  HighlightEmBTLoopback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void HighlightEmBTLoopback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
    ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

    SetLeftSoftkeyFunction(EntryEmBTLoopback, KEY_EVENT_UP);
    SetKeyHandler(NULL, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}


#if defined(__MMI_BT_COOLSAND_SUPPORT__)
/*****************************************************************************
 * FUNCTION
 *  HighlightEmBTExitTestMode
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void HighlightEmBTExitTestMode(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
    ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

    SetLeftSoftkeyFunction(EMExitBTTestMode, KEY_EVENT_UP);
    SetKeyHandler(NULL, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
#endif /* defined(__MMI_BT_COOLSAND_SUPPORT__) */


#if defined(__MMI_BT_COOLSAND_SUPPORT__) && defined(__MMI_AVRCP_DEBUG__) && defined(__MMI_AVRCP_CT_SUPPORT__)
/*****************************************************************************
 * FUNCTION
 *  HighlightEmBTAVRCPCmdList
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
extern void mmi_bt_avrcp_ct_main(void);

void HighlightEmBTAVRCPCmdList(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
    ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);

    SetLeftSoftkeyFunction(mmi_bt_avrcp_ct_main, KEY_EVENT_UP);
    SetKeyHandler(NULL, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
}
#endif /* defined(__MMI_BT_COOLSAND_SUPPORT__) && defined(__MMI_AVRCP_DEBUG__) && defined(__MMI_AVRCP_CT_SUPPORT__) */


/*****************************************************************************
 * FUNCTION
 *  EntryEmBTMenu
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryEmBTMenu(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 ItemList[32];
    U16 ItemIcons[32];
    U16 nItems;
    U8 *guiBuffer;
    U8 *PopUpList[32];
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(EM_BT_SCR_ID, NULL, EntryEmBTMenu, NULL);

    guiBuffer = GetCurrGuiBuffer(EM_BT_SCR_ID);
    nItems = GetNumOfChild(EM_BT_MENUID);
    GetSequenceStringIds(EM_BT_MENUID, ItemList);
    SetParentHandler(EM_BT_MENUID);
    RegisterHighlightHandler(ExecuteCurrHiliteHandler);

    for (i = 0; i < nItems; i++)
    {
        PopUpList[i] = NULL;
    }
    for (i = 0; i < nItems; i++)
    {
        ItemIcons[i] = (U16) (IMG_GLOBAL_L1 + i);  
    }


    ShowCategory52Screen(
        EM_BT_STR_ID,
        0,
        STR_GLOBAL_OK,
        0,
        STR_GLOBAL_BACK,
        0,
        nItems,
        ItemList,
        ItemIcons,
        (U8 **) PopUpList,
        0,
        0,
        guiBuffer);

    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}



/*****************************************************************************
 * FUNCTION
 *  EntryEmBTGeneralTestMenu
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryEmBTGeneralTestMenu(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 nStrItemList[MAX_SUB_MENUS];    /* Stores the strings id of submenus returned */
    U16 nNumofItem;                     /* Stores no of children in the submenu */
    U16 nDispAttribute, i;              /* Stores display attribue */
    U8 *guiBuffer;                      /* Buffer holding history data */
    U8 *HintList[MAX_SUB_MENUS];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
     #ifdef __BT_RDA5868__

	    if (mmi_bt_is_power_on(MMI_FALSE))
    {
        DisplayPopup(
            (U8*) GetString(EM_DEV_BT_RF_TEST_TURN_OFF_BT_TEXT),
            IMG_GLOBAL_WARNING,
            1,
            UI_POPUP_NOTIFYDURATION_TIME,
            (U8) WARNING_TONE);
        return;
    }
#endif

    /* 1 Call Exit Handler */
    EntryNewScreen(EM_BT_GENERNAL_TEST_SCR_ID, NULL, EntryEmBTGeneralTestMenu, NULL);//EntryEmBTMenu

    /* 2 Get current screen to gui buffer  for history purposes */
    guiBuffer = GetCurrGuiBuffer(EM_BT_GENERNAL_TEST_SCR_ID);

    /* 3. Retrieve no of child of menu item to be displayed */
    nNumofItem = GetNumOfChild(EM_BT_GENERNAL_TEST_MENUID);

    /* 4. Get attribute of menu to be displayed */
    nDispAttribute = GetDispAttributeOfItem(EM_BT_GENERNAL_TEST_MENUID);

    /* 5. Retrieve string ids in sequence of given menu item to be displayed */
    GetSequenceStringIds(EM_BT_GENERNAL_TEST_MENUID, nStrItemList);

    /* 6 Set current parent id */
    SetParentHandler(EM_BT_GENERNAL_TEST_MENUID /* ENGINEER_MODE_MENUID */ );

    /* 7 Register highlight handler to be called in menu screen */
    RegisterHighlightHandler(ExecuteCurrHiliteHandler);

    /* 8 Construct the Hints List */
    ConstructHintsList(EM_BT_GENERNAL_TEST_MENUID, HintList);

    for (i = 0; i < nNumofItem; i++)
    {
        HintList[i] = NULL;
    }

#ifdef __MMI_BT_BCHS_SUPPORT__
    if (bchs_debug_trace)
    {
        HintList[1] = (U8*) get_string(STR_GLOBAL_ON);
    }
    else
    {
        HintList[1] = (U8*) get_string(STR_GLOBAL_OFF);
    }
#endif        


    /* 9. Display Category screen */
    ShowCategory52Screen(
        EM_BT_GENERNAL_TEST_STR_ID,
        0 /* Title Icon */ ,
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        nNumofItem,
        nStrItemList,
        (PU16) gIndexIconsImageList,
        HintList,
        0,
        0,
        guiBuffer);

    /* 10.Highlight handler will register function with softkey */
    /*
     * SetLeftSoftkeyFunction(mmi_irda_setup_exec,KEY_EVENT_UP);
     * SetKeyHandler(NULL, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
     * SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
     * SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
     */

}


/*****************************************************************************
 * FUNCTION
 *  EntryEmBTTestMode
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryEmBTTestMode(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ilm_struct *ilm_ptr = NULL;
#if defined(__MMI_BT_COOLSAND_SUPPORT__)
    bt_test_mode_req_struct *param_ptr;
#endif

#ifdef __MMI_BT_BCHS_SUPPORT__

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((mmi_bt_is_hw_ready) (MMI_TRUE) == MMI_FALSE)
    {
        return;
    }

    ilm_ptr = allocate_ilm(MOD_MMI);
    ilm_ptr->src_mod_id = MOD_MMI;
    ilm_ptr->sap_id = BCHS_MMI_SAP;
    ilm_ptr->dest_mod_id = MOD_BCHS;

    ilm_ptr->msg_id = (kal_uint16) MSG_ID_MMI_BCHS_TEST_MODE_REQ;
    ilm_ptr->local_para_ptr = NULL;
    ilm_ptr->peer_buff_ptr = NULL;

    msg_send_ext_queue(ilm_ptr);

#endif /* __MMI_BT_BCHS_SUPPORT__ */

#if defined(__MMI_BT_COOLSAND_SUPPORT__)

	param_ptr = (bt_test_mode_req_struct *)construct_local_para(
	  			  (kal_uint16)sizeof(bt_test_mode_req_struct), TD_CTRL);	
    param_ptr->mode = 1;	

    ilm_ptr = allocate_ilm(MOD_MMI);
    ilm_ptr->src_mod_id = MOD_MMI;
    ilm_ptr->sap_id = BT_APP_SAP;
    ilm_ptr->dest_mod_id = MOD_BT;

    ilm_ptr->msg_id = (kal_uint16) MSG_ID_BT_TEST_MODE_REQ;
    ilm_ptr->local_para_ptr = (local_para_struct *)param_ptr;
    ilm_ptr->peer_buff_ptr = NULL;

    msg_send_ext_queue(ilm_ptr);

#endif /* defined(__MMI_BT_COOLSAND_SUPPORT__) */


    DisplayPopup(
        (U8*) GetString(STR_GLOBAL_DONE),
        IMG_GLOBAL_ACTIVATED,
        0,
        UI_POPUP_NOTIFYDURATION_TIME,
        SUCCESS_TONE);

}


#ifdef __MMI_BT_BCHS_SUPPORT__
/*****************************************************************************
 * FUNCTION
 *  EntryEmBTDebugTrace
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryEmBTDebugTrace(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (bchs_debug_trace == FALSE)
    {
        bchs_debug_trace = TRUE;
        Category52ChangeItemDescription(1, (U8*) get_string(STR_GLOBAL_ON));
    }
    else
    {
        bchs_debug_trace = FALSE;
        Category52ChangeItemDescription(1, (U8*) get_string(STR_GLOBAL_OFF));
    }

    RedrawCategoryFunction();
    HighlightEmBTDebugTrace();
}
#endif


/*****************************************************************************
 * FUNCTION
 *  EntryEmBTAddAudioConnection
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryEmBTAddAudioConnection(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    //mmi_bt_hfp_sco_link_connect_req();
    mmi_profiles_bt_connect_sco(NULL);
}


/*****************************************************************************
 * FUNCTION
 *  EntryEmBTRemoveAudioConnection
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryEmBTRemoveAudioConnection(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    //mmi_bt_hfp_sco_link_disconnect_req();
    mmi_profiles_bt_disconnect_sco();
}


/*****************************************************************************
 * FUNCTION
 *  EntryEmBTLoopback
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryEmBTLoopback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;
    U8 *inputBuffer;
    U16 inputBufferSize;
    U16 ImageList[4];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(EM_BT_LOOKBACK_SCR_ID, ExitEMBTLoopback, EntryEmBTLoopback, NULL);
    InitializeCategory57Screen();

    //g_bt_password[0] = 0;
    //g_bt_device_address[0] = 0;

    guiBuffer = GetCurrGuiBuffer(EM_BT_LOOKBACK_SCR_ID);
    inputBuffer = GetCurrNInputBuffer(EM_BT_LOOKBACK_SCR_ID, &inputBufferSize);

    RegisterHighlightHandler(SetHighlightIndex);

    /* BT Address */
    SetInlineItemActivation(&wgui_inline_items[0], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemCaption(&(wgui_inline_items[0]), (U8*) GetString(EM_BT_BD_ADDR_STR_ID));
    ImageList[0] = IMG_STATUS;

    SetInlineItemActivation(&wgui_inline_items[1], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemTextEdit(
        &wgui_inline_items[1],
        (PU8) g_bt_device_address,
        BT_ADDRESS_LEN + 1,
        INPUT_TYPE_ALPHANUMERIC_LOWERCASE);
    ImageList[1] = 0;

    /* BT Password */
    SetInlineItemActivation(&wgui_inline_items[2], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemCaption(&(wgui_inline_items[2]), (U8*) GetString(EM_BT_BD_PASSWORD_STR_ID));
    ImageList[2] = IMG_STATUS;

    SetInlineItemActivation(&wgui_inline_items[3], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemTextEdit(&wgui_inline_items[3], (PU8) g_bt_password, BT_PASSWORD_LEN + 1, INPUT_TYPE_NUMERIC);
    ImageList[3] = 0;

    if (inputBuffer != NULL)
    {
        SetCategory57Data(wgui_inline_items, 4, inputBuffer);
    }

    DisableCategory57ScreenDone();

    ShowCategory57Screen(
        EM_BT_LOOKBACK_STR_ID,
        EM_DEV_GPIO_SET_ICON,
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        4,
        ImageList,
        wgui_inline_items,
        0,
        guiBuffer);

    SetCategory57RightSoftkeyFunctions(EMBTLoopbackReq, GoBackHistory);

}


/*****************************************************************************
 * FUNCTION
 *  ExitEMBTLoopback
 * DESCRIPTION
 *  exit bt loopback test
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitEMBTLoopback(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CloseCategory57Screen();
}


/*****************************************************************************
 * FUNCTION
 *  EMBTLoopbackReq
 * DESCRIPTION
 *  BT PCM loopback test request
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EMBTLoopbackReq(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 addrbuf[BT_ADDRESS_LEN + 1];
    U8 passwdbuf[BT_PASSWORD_LEN + 1];
    U32 lap = 0;
    U8 uap = 0;
    U16 nap = 0;
    U8 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CloseCategory57Screen();

    /* check if bt address & password is None or too large */
    if ((!UCS2Strlen((const S8*)g_bt_device_address) && !UCS2Strlen((const S8*)g_bt_password))
        ||
        ((UCS2Strlen((const S8*)g_bt_device_address) > BT_ADDRESS_LEN)
         || (UCS2Strlen((const S8*)g_bt_password) > BT_PASSWORD_LEN)))
    {
        DisplayPopup((PU8) GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 0, EM_NOTIFY_DURATION, 0);
        DeleteUptoScrID(EM_BT_GENERNAL_TEST_SCR_ID);
        return;
    }

    UnicodeToAnsii((PS8) addrbuf, (PS8) g_bt_device_address);

    /* Re-transfer BT address. And check if the bt address is valid.  */
    for (i = 0; i < 12; i++)
    {
        if (addrbuf[i] >= '0' && addrbuf[i] <= '9') /* 0 ~ 9 */
        {
            addrbuf[i] = addrbuf[i] - '0';
        }
        else if (addrbuf[i] >= 'A' && addrbuf[i] <= 'F')    /* A ~ F */
        {
            addrbuf[i] = addrbuf[i] - 'A' + 10;
        }
        else if (addrbuf[i] >= 'a' && addrbuf[i] <= 'f')    /* a ~ f */
        {
            addrbuf[i] = addrbuf[i] - 'a' + 10;
        }
        else
        {
            DisplayPopup((PU8) GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 0, EM_NOTIFY_DURATION, 0);
            DeleteUptoScrID(EM_BT_GENERNAL_TEST_SCR_ID);
            return;
        }
    }

    /* transfer bt address from U8 array to low-layer format */
    /* nap : array[0]~array[3] */
    for (i = 0; i < 4; i++)
    {
        nap |= ((U16) addrbuf[i] << (((3 - i) << 2)));
    }

    /* uap : array[4]~array[5] */
    for (i = 4; i < 6; i++)
    {
        uap |= ((U8) addrbuf[i] << (((5 - i) << 2)));
    }

    /* lap : array[6]~array[11] */
    for (i = 6; i < 12; i++)
    {
        lap |= ((U32) addrbuf[i] << (((11 - i) << 2)));
    }

    UnicodeToAnsii((PS8) passwdbuf, (PS8) g_bt_password);

    /* enter in progress screen */
    EntryEmBTInProgress();

    /* call mmi_bt function to do pcm loopback test */
    mmi_bt_sco_far_end_loopback_test(lap, uap, nap, passwdbuf, (U8) strlen((const S8*)passwdbuf));

}


/*****************************************************************************
 * FUNCTION
 *  EntryEmBTInProgress
 * DESCRIPTION
 *  BT progress screen
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryEmBTInProgress(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(EM_BT_PROGRESS_SCR, NULL, EntryEmBTInProgress, NULL);

    guiBuffer = GetCurrGuiBuffer(EM_BT_PROGRESS_SCR);

    /* clear End key */
    ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
    ClearKeyHandler(KEY_END, KEY_LONG_PRESS);

    /* Show progress screen */
    ShowCategory66Screen(
        EM_BT_LOOKBACK_STR_ID,
        0,
        0,
        0,
        STR_GLOBAL_CANCEL,
        0,
        (U8*) GetString(EM_SOCKET_IN_PROGRESS_TEXT),    /* in progress */
        IMG_GLOBAL_PROGRESS,
        NULL);

    /* Set RSK to abort progress */
    SetRightSoftkeyFunction(EmBTInProgressAbort, KEY_EVENT_UP);
    SetKeyHandler(EmBTInProgressAbort, KEY_END, KEY_EVENT_DOWN);

}


/*****************************************************************************
 * FUNCTION
 *  EmBTNotifyLoopbackDoneCnf
 * DESCRIPTION
 *  Loopback test Done.
 *  This function will be called by MMI_BT
 *  after BT PCM loopback test is done.
 * PARAMETERS
 *  result      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void EmBTNotifyLoopbackDoneCnf(MMI_BOOL result)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (result) /* success */
    {
        DisplayPopup((PU8) GetString(STR_GLOBAL_DONE), IMG_GLOBAL_OK, 0, EM_NOTIFY_DURATION, 0);
    }
    else    /* fail */
    {
        DisplayPopup((PU8) GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 0, EM_NOTIFY_DURATION, 0);
    }

    DeleteUptoScrID(EM_BT_GENERNAL_TEST_SCR_ID);
}


/*****************************************************************************
 * FUNCTION
 *  EmBTInProgressAbort
 * DESCRIPTION
 *  BT progress abort
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EmBTInProgressAbort(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_bt_sco_far_end_loopback_cancel_test();

    DisplayPopup((PU8) GetString(STR_GLOBAL_CANCEL), IMG_GLOBAL_ERROR, 0, EM_NOTIFY_DURATION, 0);
    DeleteUptoScrID(EM_BT_GENERNAL_TEST_SCR_ID);
}


#if defined(__MMI_BT_COOLSAND_SUPPORT__)
/*****************************************************************************
 * FUNCTION
 *  EMExitBTTestMode
 * DESCRIPTION
 *  Leave BT Test Mode
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EMExitBTTestMode(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    bt_test_mode_req_struct *param_ptr;
    ilm_struct *ilm_ptr = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	param_ptr = (bt_test_mode_req_struct *)construct_local_para(
	  			  (kal_uint16)sizeof(bt_test_mode_req_struct), TD_CTRL);	
    param_ptr->mode = 2;	

    ilm_ptr = allocate_ilm(MOD_MMI);
    ilm_ptr->src_mod_id = MOD_MMI;
    ilm_ptr->sap_id = BT_APP_SAP;
    ilm_ptr->dest_mod_id = MOD_BT;

    ilm_ptr->msg_id = (kal_uint16) MSG_ID_BT_TEST_MODE_REQ;
    ilm_ptr->local_para_ptr = (local_para_struct *)param_ptr;
    ilm_ptr->peer_buff_ptr = NULL;

    msg_send_ext_queue(ilm_ptr);


    DisplayPopup(
        (U8*) GetString(STR_GLOBAL_DONE),
        IMG_GLOBAL_ACTIVATED,
        0,
        UI_POPUP_NOTIFYDURATION_TIME,
        SUCCESS_TONE);
}

#endif /* defined(__MMI_BT_COOLSAND_SUPPORT__) */
#endif 
#ifdef __MMI_EM_BT_RF_TEST__
/**************************************************************
**	FUNCTION NAME		: HighlightEmDevBtRfTest
**
**  	PURPOSE			: High light function to Bluetooth RF Test menu
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmDevBtRfTest(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_bt_rf_test_cntx.test_item = EM_BT_RF_TEST_TYPE_POWER_ON;

    g_bt_rf_test_cntx.last_cmd = EM_BT_RF_TEST_TYPE_POWER_ON;
    
    SetLeftSoftkeyFunction(EntryEmDevBtRfTestConfirm, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	
}


/*****************************************************************************
 * FUNCTION
 *  EntryEmDevBtRfTestConfirm
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryEmDevBtRfTestConfirm(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (mmi_bt_is_power_on(MMI_FALSE))
    {
        DisplayPopup(
            (U8*) GetString(EM_DEV_BT_RF_TEST_TURN_OFF_BT_TEXT),
            IMG_GLOBAL_WARNING,
            1,
            UI_POPUP_NOTIFYDURATION_TIME,
            (U8) WARNING_TONE);
        return;
    }

    DisplayConfirm(
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        get_string(EM_RF_TEST_GSM_CONFIRM_NOTIFY_TEST) ,
        IMG_GLOBAL_INFO,
        WARNING_TONE);
#ifdef __BT_RDA5868__
    SetLeftSoftkeyFunction(EmBtRfTestPowerOnBT, KEY_EVENT_UP);
#else
    SetLeftSoftkeyFunction(EmBtRfTestDisableSleepMode, KEY_EVENT_UP);
#endif
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EmBtRfTestDisableSleepMode
**
**  PURPOSE				: BT RF Test Disable Sleep Mode
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmBtRfTestDisableSleepMode(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* re-register protocol event handler of MSG_ID_MMI_EQ_SET_SLEEP_MODE_RSP */
    SetProtocolEventHandler(EmBtRfTestSleepModeHdlr, MSG_ID_MMI_EQ_SET_SLEEP_MODE_RSP);    
    EngineerModeSetSleepModeReq(MMI_TRUE);     /* 1 means disable */
}


/**************************************************************
**	FUNCTION NAME		: EmBtRfTestSleepModeHdlr
**
**  PURPOSE				: BT RF Test Sleep Mode Handler
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmBtRfTestSleepModeHdlr(void* inMsg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* stop MMI sleep mode */
    TurnOnBacklight(0);
             
#ifdef __FLIGHT_MODE_SUPPORT__
    /* Enter Flight mode */
    EmBtRfTestEnterFlightMode();
#else
    EmBtRfTestPowerOnBT();
#endif /* #ifdef __FLIGHT_MODE_SUPPORT__ */
}


#ifdef __FLIGHT_MODE_SUPPORT__
/**************************************************************
**	FUNCTION NAME		: EmBtRfTestEnterFlightMode
**
**  PURPOSE				: BT RF Test menu Enter Flight Mode
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmBtRfTestEnterFlightMode(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
  //  extern void mmi_em_enter_flight_mode(FuncPtr callback);
    
 //   mmi_em_enter_flight_mode((FuncPtr)EmBtRfTestPowerOnBT);

	/* Fion test */
	EmBtRfTestPowerOnBT();
}
#endif /* __FLIGHT_MODE_SUPPORT__ */


/**************************************************************
**	FUNCTION NAME		: EmBtRfTestPowerOnBT
**
**  PURPOSE				: BT RF Test Disable Sleep Mode
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmBtRfTestPowerOnBT(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __BT_RDA5868__
	mmi_trace(1,"EmBtRfTestPowerOnBT is called");
  //  rda_bt_poweron_for_test();
    EmBtRfTestPowerOnBTRspHdlr(NULL);
#else
    SetProtocolEventHandler(EmBtRfTestPowerOnBTRspHdlr, MSG_ID_BT_ENGINEER_MODE_POWERON_CNF );    
    EMBtRfTestSendReq();
#endif
 
	/* Fion Add, Test */
	//EmBtRfTestPowerOnBTRspHdlr(NULL);

}


/*****************************************************************************
 * FUNCTION
 *  EmBtRfTestPowerOnBTRspHdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  inMsg       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void EmBtRfTestPowerOnBTRspHdlr(void *inMsg)
{

#ifdef __BT_RDA5868__
    EntryEmDevBtRfTest();
#else
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    bt_engineer_mode_poweron_cnf_struct *rsp_p = inMsg;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (rsp_p->result)
    {
        EntryEmDevBtRfTest();
    }
    else
    {
        EngineerModeErrorNotice(EM_AUD_SET_ERROR_TEXT);
		DeleteUptoScrID(EM_DEVICE_MENU_SCR);
    }
#endif
}


/**************************************************************
**	FUNCTION NAME		: EntryEmDevBtRfTest
**  	PURPOSE			: Entry function of BT RF Test menu item
**	INPUT PARAM		: void
**	OUTPUT PARAM		: void
**	RETURNS			: void
 
**************************************************************/
void EntryEmDevBtRfTest(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16	ItemList[8];
    U16	nItems;
    U8*	guiBuffer;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(EM_DEV_BT_RF_TEST_SCR_ID, NULL, EntryEmDevBtRfTest, NULL);
    guiBuffer = GetCurrGuiBuffer(EM_DEV_BT_RF_TEST_SCR_ID);
    nItems = GetNumOfChild(EM_DEVICE_BT_RF_TEST_MENUID);
    GetSequenceStringIds(EM_DEVICE_BT_RF_TEST_MENUID,ItemList);
    SetParentHandler(EM_DEVICE_BT_RF_TEST_MENUID);

    RegisterHighlightHandler(ExecuteCurrHiliteHandler);

    ShowCategory52Screen(EM_DEV_BT_RF_TEST_TEXT,0,
                         STR_GLOBAL_OK ,0, STR_GLOBAL_BACK,0,
                         nItems, ItemList, 
                         (U16 *)gIndexIconsImageList, 0, 0, 0, guiBuffer);			 
   						  
    SetRightSoftkeyFunction(EmBtRfTestExit,KEY_EVENT_UP);

    /* Register END key handler */
    SetKeyHandler(EmBtRfTestExit,KEY_END,KEY_EVENT_DOWN);

}


/**************************************************************
**	FUNCTION NAME		: EmBtRfTestExit
**
**  PURPOSE				: BT RF Test menu Exit
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmBtRfTestExit(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    extern void ShutdownSystemOperation(void);

    ShutdownSystemOperation();
}
     #ifdef __BT_RDA5868__
void ExitEmDevBtRfVCO(void)
{
	rda_bt_poweroff_for_test();
}
	 

void EntryEmDevBtRfVCO(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(EM_DEVICE_RDA_BT_TEST_VCO_SCR, ExitEmDevBtRfVCO, EntryEmDevBtRfVCO, NULL);
    guiBuffer = GetCurrGuiBuffer(EM_DEVICE_RDA_BT_TEST_VCO_SCR);
    rda_bt_poweron_for_test();
	RDA5868_send_vco_test();   
    /* Show progress screen */
    ShowCategory66Screen(
        EM_DEVICE_RDA_BT_TEST_VCO_TEXT,
        0,
        0,
        0,
        STR_GLOBAL_CANCEL,
        0,
        (PU8) GetString(EM_DEVICE_RDA_BT_TEST_VCO_TEXT),
        IMG_GLOBAL_PROGRESS,
        guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_END, KEY_EVENT_DOWN);
}

void ExitEmDevBtRfSingleHop(void)
{
	rda_bt_poweroff_for_test();
}

void EntryEmDevBtRfSingleHop(void)
{
   /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    EntryNewScreen(EM_DEVICE_RDA_BT_TEST_VCO_SCR, ExitEmDevBtRfSingleHop, EntryEmDevBtRfSingleHop, NULL);
    guiBuffer = GetCurrGuiBuffer(EM_DEVICE_RDA_BT_TEST_VCO_SCR);
     rda_bt_poweron_for_test();
	RDA5868_send_bbtest_singlehop_inquiry();	
    /* Show progress screen */
    ShowCategory66Screen(
        EM_DEVICE_RDA_BT_TEST_SINGLEHOP_TEXT,
        0,
        0,
        0,
        STR_GLOBAL_CANCEL,
        0,
        (PU8) GetString(EM_DEVICE_RDA_BT_TEST_SINGLEHOP_TEXT),
        IMG_GLOBAL_PROGRESS,
        guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_END, KEY_EVENT_DOWN);
}

void ExitEmDevBtRfCMU200(void)
{
	rda_bt_poweroff_for_test();
}

void EntryEmDevBtRfCMU200(void)
{
   /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    EntryNewScreen(EM_DEVICE_RDA_BT_TEST_CMU200_SCR, ExitEmDevBtRfCMU200, EntryEmDevBtRfCMU200, NULL);
    guiBuffer = GetCurrGuiBuffer(EM_DEVICE_RDA_BT_TEST_CMU200_SCR);
	rda_bt_poweron_for_test();
	RDA_bt_send_cmu200_enable_dut();
    /* Show progress screen */
    ShowCategory66Screen(
        EM_DEVICE_RDA_BT_TEST_CMU200_TEXT,
        0,
        0,
        0,
        STR_GLOBAL_CANCEL,
        0,
        (PU8) GetString(EM_DEVICE_RDA_BT_TEST_CMU200_TEXT),
        IMG_GLOBAL_PROGRESS,
        guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_END, KEY_EVENT_DOWN);
}
void ExitEmDevBtRfC3000(void)
{
	rda_bt_poweroff_for_test();
}

void EntryEmDevBtRfC3000(void)
{
   /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    EntryNewScreen(EM_DEVICE_RDA_BT_TEST_TC3000_SCR, ExitEmDevBtRfC3000, EntryEmDevBtRfC3000, NULL);
    guiBuffer = GetCurrGuiBuffer(EM_DEVICE_RDA_BT_TEST_TC3000_SCR);
	rda_bt_poweron_for_test();
	RDA_bt_send_tc3000_enable_dut();
    /* Show progress screen */
    ShowCategory66Screen(
        EM_DEVICE_RDA_BT_TEST_TC3000_TEXT,
        0,
        0,
        0,
        STR_GLOBAL_CANCEL,
        0,
        (PU8) GetString(EM_DEVICE_RDA_BT_TEST_TC3000_TEXT),
        IMG_GLOBAL_PROGRESS,
        guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_END, KEY_EVENT_DOWN);
}
void ExitEmDevBtRf4010(void)
{
	rda_bt_poweroff_for_test();
}
void EntryEmDevBtRf4010(void)
{
   /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    EntryNewScreen(EM_DEVICE_RDA_BT_TEST_N4010_SCR, ExitEmDevBtRf4010, EntryEmDevBtRf4010, NULL);
    guiBuffer = GetCurrGuiBuffer(EM_DEVICE_RDA_BT_TEST_N4010_SCR);
	rda_bt_poweron_for_test();
	RDA_bt_send_n4010_enable_dut();
    /* Show progress screen */
    ShowCategory66Screen(
        EM_DEVICE_RDA_BT_TEST_N4010_TEXT,
        0,
        0,
        0,
        STR_GLOBAL_CANCEL,
        0,
        (PU8) GetString(EM_DEVICE_RDA_BT_TEST_N4010_TEXT),
        IMG_GLOBAL_PROGRESS,
        guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_END, KEY_EVENT_DOWN);
}
	
void HighlightEmDevBtRfVCO(void)
{
    SetLeftSoftkeyFunction(EntryEmDevBtRfVCO, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	

}
void HighlightEmDevBtRfSingleHop(void)
{
    SetLeftSoftkeyFunction(EntryEmDevBtRfSingleHop, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	

}
void HighlightEmDevBtRfCMU200(void)
{
    SetLeftSoftkeyFunction(EntryEmDevBtRfCMU200, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	

}
void HighlightEmDevBtRfTC3000(void)
{
    SetLeftSoftkeyFunction(EntryEmDevBtRfC3000, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	

}
void HighlightEmDevBtRfN4010(void)
{
    SetLeftSoftkeyFunction(EntryEmDevBtRf4010, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	

}

#endif


/**************************************************************
**	FUNCTION NAME		: HighlightEmDevBtRfTX
**
**  	PURPOSE			: High light function to BT RF Test TX menu
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmDevBtRfTX(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_bt_rf_test_cntx.test_item = EM_BT_RF_TEST_TYPE_TX;
    
    SetLeftSoftkeyFunction(EntryEmDevBtRfTXRX, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmDevBtRfRX
**
 
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmDevBtRfRX(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_bt_rf_test_cntx.test_item = EM_BT_RF_TEST_TYPE_RX;
#ifdef __BT_RDA5868__
    SetLeftSoftkeyFunction(EMBtRfTestSendReq, KEY_EVENT_UP);
#else
    SetLeftSoftkeyFunction(EntryEmDevBtRfTXRX, KEY_EVENT_UP);
#endif    
    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	
}


/**************************************************************
**	FUNCTION NAME		: EntryEmDevBtRfTXRX
 
**	INPUT PARAM		: void
**	OUTPUT PARAM		: void
**	RETURNS			: void
 
**************************************************************/
void EntryEmDevBtRfTXRX(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8* guiBuffer;
    U16 ImageList[9];
    U16 title=0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((g_bt_rf_test_cntx.last_cmd != EM_BT_RF_TEST_TYPE_POWER_ON) &&
         ((g_bt_rf_test_cntx.last_cmd != EM_BT_RF_TEST_TYPE_TX) &&
         (g_bt_rf_test_cntx.last_cmd != EM_BT_RF_TEST_TYPE_RX)))
    {
        DisplayPopup((PU8)GetString(STR_GLOBAL_NOT_AVAILABLE), IMG_GLOBAL_ERROR, 0, EM_NOTIFY_DURATION, 0);
        return;
    }
    
    EntryNewScreen(EM_DEV_BT_RF_TEST_TXRX_SCR_ID, ExitEmDevBtRfTXRX, NULL, NULL);
    InitializeCategory57Screen();
	
    guiBuffer = GetCurrGuiBuffer(EM_DEV_BT_RF_TEST_TXRX_SCR_ID);
  
    RegisterHighlightHandler(HighlightHandlerEmBtRFTestTxRxnline);

    /* Hoping/Non Hoping */
    g_bt_rf_test_cntx.pEMBtRfHoping[0] = (U8*)GetString(EM_DEV_BT_RF_TEST_NO_HOPING_TEXT);
    g_bt_rf_test_cntx.pEMBtRfHoping[1] = (U8*)GetString(EM_DEV_BT_RF_TEST_HOPING_TEXT);
    SetInlineItemActivation(&wgui_inline_items[0], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemSelect(&wgui_inline_items[0], 2, (U8**)g_bt_rf_test_cntx.pEMBtRfHoping, &g_bt_rf_test_cntx.currHoping);    
    RegisterInlineSelectHighlightHandler(&wgui_inline_items[0],HighlightHandlerBtRfTxRxHopingInline);    
    ImageList[0] = IMG_STATUS;

    /* channel */
    SetInlineItemActivation(&wgui_inline_items[1], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemCaption(&wgui_inline_items[1], (U8*)GetString(EM_DEV_BT_RF_TEST_CHANNEL_TEXT));
    ImageList[1] = IMG_STATUS;

    SetInlineItemActivation(&wgui_inline_items[2], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemTextEdit(&wgui_inline_items[2], (PU8)g_bt_rf_test_cntx.channel, 3, INPUT_TYPE_DECIMAL_NUMERIC);
    ImageList[2] = 0;

    /* Packet Type */       
    SetInlineItemActivation(&wgui_inline_items[3], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemCaption(&wgui_inline_items[3], (U8*)GetString(EM_DEV_BT_RF_TEST_PACKET_TYPE_TEXT));
    ImageList[3] = IMG_STATUS;

    g_bt_rf_test_cntx.pEMBtRfPacketType[0] = (U8*)GetString(EM_DEV_BT_RF_TEST_PACKET_TYPE_DH1_TEXT);
    g_bt_rf_test_cntx.pEMBtRfPacketType[1] = (U8*)GetString(EM_DEV_BT_RF_TEST_PACKET_TYPE_DH3_TEXT);
    g_bt_rf_test_cntx.pEMBtRfPacketType[2] = (U8*)GetString(EM_DEV_BT_RF_TEST_PACKET_TYPE_DH5_TEXT);
    SetInlineItemActivation(&wgui_inline_items[4], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemSelect(&wgui_inline_items[4], 3, (U8**)g_bt_rf_test_cntx.pEMBtRfPacketType, &g_bt_rf_test_cntx.currPacketType);    
    RegisterInlineSelectHighlightHandler(&wgui_inline_items[4],HighlightHandlerBtRfPacketTypeInline);    
    ImageList[4] = 0;

    /* Data Len. */    
    SetInlineItemActivation(&wgui_inline_items[5], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemCaption(&wgui_inline_items[5], (U8*)GetString(EM_DEV_BT_RF_TEST_DATA_LEN_TEXT));
    ImageList[5] = IMG_STATUS;
    
    SetInlineItemActivation(&wgui_inline_items[6], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemTextEdit(&wgui_inline_items[6], (PU8)g_bt_rf_test_cntx.data_len, 4, INPUT_TYPE_DECIMAL_NUMERIC);
    ImageList[6] = 0;

    /* Poll Period */    
    SetInlineItemActivation(&wgui_inline_items[7], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemCaption(&wgui_inline_items[7], (U8*)GetString(EM_DEV_BT_RF_TEST_POLL_PERIOD_TEXT));
    ImageList[7] = IMG_STATUS;

    SetInlineItemActivation(&wgui_inline_items[8], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemTextEdit(&wgui_inline_items[8], (PU8)g_bt_rf_test_cntx.poll_period, 3, INPUT_TYPE_DECIMAL_NUMERIC);
    ImageList[8] = 0;  

    DisableCategory57ScreenDone();
     	
    if (g_bt_rf_test_cntx.test_item == EM_BT_RF_TEST_TYPE_TX )
    {
        title = EM_RF_GSM_TX_TEST_TEXT;
    }
    else if (g_bt_rf_test_cntx.test_item == EM_BT_RF_TEST_TYPE_RX)
    {
        title = EM_RF_GSM_RX_TEST_TEXT;
    }

		
    ShowCategory57Screen(title, 0, 
                         STR_GLOBAL_OK, 
                         IMG_GLOBAL_OK, 
       	                 STR_GLOBAL_BACK, 
       	                 IMG_GLOBAL_BACK,
				         9, ImageList, wgui_inline_items, 0, guiBuffer);	
      
    SetCategory57RightSoftkeyFunctions( EMBtRfTestTXRXSendReq, GoBackHistory );
    //SetLeftSoftkeyFunction(EMBtRfTestTXRXSendReq, KEY_EVENT_UP);

    /* Register END key handler */
    SetKeyHandler(GoBackHistory,KEY_END,KEY_EVENT_DOWN);

}


/**************************************************************
**	FUNCTION NAME		: HighlightHandlerEmBtRFTestTxRxnline
**
**  PURPOSE				: Highlight handler of TXRX inline editor in input screen.
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightHandlerEmBtRFTestTxRxnline(S32 index) 
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
    SetLeftSoftkeyFunction(EMBtRfTestTXRXSendReq, KEY_EVENT_UP);

    /* Register END key handler, for inline editor might register END key */
    SetKeyHandler(GoBackHistory,KEY_END,KEY_EVENT_DOWN);
}


/**************************************************************
**	FUNCTION NAME		: HighlightHandlerBtRfTxRxHopingInline
**
**  PURPOSE				: Highlight handler of BT Rf Hoping Inline highlight handler.
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightHandlerBtRfTxRxHopingInline(S32 index) 
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 buffer[32];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch(g_bt_rf_test_cntx.currHoping)
    {
        case EM_BT_RF_TEST_HOPING_OFF:
        {
            sprintf(buffer,"0");
            AnsiiToUnicodeString((PS8)g_bt_rf_test_cntx.channel, buffer);
//            SetInlineItemDisplayOnly(&wgui_inline_items[2], (PU8)g_bt_rf_test_cntx.channel);        
            break;
        }
        case EM_BT_RF_TEST_HOPING_ON:
        {
            sprintf(buffer,"0");
            AnsiiToUnicodeString((PS8)g_bt_rf_test_cntx.channel, buffer);
//            SetInlineItemTextEdit(&wgui_inline_items[2], (PU8)g_bt_rf_test_cntx.channel, 3, INPUT_TYPE_DECIMAL_NUMERIC);
            break;
        }
        default:
            ASSERT(0);
    }

    switch(g_bt_rf_test_cntx.currPacketType)
    {
        case EM_BT_RF_TEST_PACKET_TYPE_DH1:
        {
            sprintf(buffer,"27");
            AnsiiToUnicodeString((PS8)g_bt_rf_test_cntx.data_len, buffer);

            sprintf(buffer,"2");
            AnsiiToUnicodeString((PS8)g_bt_rf_test_cntx.poll_period, buffer);

            break;
        }
        case EM_BT_RF_TEST_PACKET_TYPE_DH3:
        {
            sprintf(buffer,"183");
            AnsiiToUnicodeString((PS8)g_bt_rf_test_cntx.data_len, buffer);

            sprintf(buffer,"6");
            AnsiiToUnicodeString((PS8)g_bt_rf_test_cntx.poll_period, buffer);
            break;
        }
        case EM_BT_RF_TEST_PACKET_TYPE_DH5:
        {
            sprintf(buffer,"339");
            AnsiiToUnicodeString((PS8)g_bt_rf_test_cntx.data_len, buffer);

            sprintf(buffer,"10");
            AnsiiToUnicodeString((PS8)g_bt_rf_test_cntx.poll_period, buffer);
            break;
        }
        default:
            ASSERT(0);
    }
    
    redraw_fixed_list();
}


/**************************************************************
**	FUNCTION NAME		: HighlightHandlerBtRfPacketTypeInline
**
**  PURPOSE				: Highlight handler of BT Rf Packet type Inline highlight handler.
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightHandlerBtRfPacketTypeInline(S32 index) 
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 buffer[32];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch(g_bt_rf_test_cntx.currPacketType)
    {
        case EM_BT_RF_TEST_PACKET_TYPE_DH1:
        {
            sprintf(buffer,"27");
            AnsiiToUnicodeString((PS8)g_bt_rf_test_cntx.data_len, buffer);

            sprintf(buffer,"2");
            AnsiiToUnicodeString((PS8)g_bt_rf_test_cntx.poll_period, buffer);

            break;
        }
        case EM_BT_RF_TEST_PACKET_TYPE_DH3:
        {
            sprintf(buffer,"183");
            AnsiiToUnicodeString((PS8)g_bt_rf_test_cntx.data_len, buffer);

            sprintf(buffer,"6");
            AnsiiToUnicodeString((PS8)g_bt_rf_test_cntx.poll_period, buffer);
            break;
        }
        case EM_BT_RF_TEST_PACKET_TYPE_DH5:
        {
            sprintf(buffer,"339");
            AnsiiToUnicodeString((PS8)g_bt_rf_test_cntx.data_len, buffer);

            sprintf(buffer,"10");
            AnsiiToUnicodeString((PS8)g_bt_rf_test_cntx.poll_period, buffer);
            break;
        }
        default:
            ASSERT(0);
    }

     redraw_fixed_list();
}


/**************************************************************

	FUNCTION NAME		: EMBtRfTestTXRXSendReq

  	PURPOSE				: Send BT RF Test req

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EMBtRfTestTXRXSendReq(void) 
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CloseCategory57Screen();

    /* Send request to RF,  now req_type = RF_TEST_GSM_RX_TEST */
#ifdef __BT_RDA5868__	
    g_bt_rf_test_cntx.currCtrlPwrLevel = 3;
#else
    g_bt_rf_test_cntx.currCtrlPwrLevel = 1;
#endif
    EMBtRfTestSendReq();

    /* show BT RF Testing screen */
//    EntryEmDevBtRfTesting();

}


/**************************************************************
**	FUNCTION NAME		: EMBtRfTestSendReq
**
** 	PURPOSE				: BT RF Test send request function. 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EMBtRfTestSendReq(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8  textbuf[20];
    U8 value;
	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch(g_bt_rf_test_cntx.test_item)
    {
        case EM_BT_RF_TEST_TYPE_POWER_ON:
        {
            
#ifdef __GEMINI__           
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#else            
            EngineerModeSendMsg_Ext(MSG_ID_BT_ENGINEER_MODE_POWERON_REQ, MOD_BT, NULL, NULL);
#endif
            break;
        }
        case EM_BT_RF_TEST_TYPE_TX:
        {
            bt_engineer_mode_txrx_test_req_struct *msg_p;
            MMI_BOOL result = MMI_TRUE;

            result = EmBtRfTestCheckTXRXValue();

            if (result == MMI_FALSE)
            {
                DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 0, EM_NOTIFY_DURATION, 0);
                return;
            }
          
            g_bt_rf_test_cntx.last_cmd = EM_BT_RF_TEST_TYPE_TX;
            SetProtocolEventHandler(EmBtRfTestTXRXRspHdlr, MSG_ID_BT_ENGINEER_MODE_TXRX_TEST_CNF );    
            ClearInputEventHandler(MMI_DEVICE_ALL);

            msg_p = OslConstructDataPtr(sizeof(bt_engineer_mode_txrx_test_req_struct));

            msg_p->pattern = 1; /* means 0000 */

            if (g_bt_rf_test_cntx.currHoping == EM_BT_RF_TEST_HOPING_OFF)
            {
                msg_p->channel_hopping = 0;
                msg_p->tx_freq= 0; 
                msg_p->rx_freq= 0;
            }
            else /* EM_BT_RF_TEST_HOPING_ON */
            {
                msg_p->channel_hopping = 1;

                UnicodeToAnsii((PS8)textbuf,  (PS8)g_bt_rf_test_cntx.channel);
                value = atoi((const char *) textbuf);
                msg_p->tx_freq= value; 
                msg_p->rx_freq= 0;
            }

            switch (g_bt_rf_test_cntx.currPacketType)
            {
                case EM_BT_RF_TEST_PACKET_TYPE_DH1:
                    msg_p->packet_type= 0x04;
                    break;

                case EM_BT_RF_TEST_PACKET_TYPE_DH3:
                    msg_p->packet_type= 0x0B;
                    break;

                case EM_BT_RF_TEST_PACKET_TYPE_DH5:
                    msg_p->packet_type= 0x0F;
                    break;
					
            }

            UnicodeToAnsii((PS8)textbuf,  (PS8)g_bt_rf_test_cntx.data_len);
            value = atoi((const char *) textbuf);
            msg_p->packet_length= value;

            UnicodeToAnsii((PS8)textbuf,  (PS8)g_bt_rf_test_cntx.poll_period);
            value = atoi((const char *) textbuf);
            msg_p->poll_period = value; 
#ifdef __BT_RDA5868__
    //        RDA5868_send_bbtest(msg_p);//mql
            EmBtRfTestTXRXRspHdlr(NULL);
#else
            EngineerModeSendMsg_Ext(MSG_ID_BT_ENGINEER_MODE_TXRX_TEST_REQ, MOD_BT, msg_p, NULL);

			/* Fion Add, Test */
            //EmBtRfTestTXRXRspHdlr(NULL);
#endif	            

            break;
        }
        case EM_BT_RF_TEST_TYPE_RX:
        {
#ifdef __BT_RDA5868__
            g_bt_rf_test_cntx.last_cmd = EM_BT_RF_TEST_TYPE_RX;
//            RDA5868_send_bbtest_singlehop_inquiry();
            EmBtRfTestTXRXRspHdlr(NULL);		
#else
            bt_engineer_mode_txrx_test_req_struct *msg_p;
            MMI_BOOL result = MMI_TRUE;

            result = EmBtRfTestCheckTXRXValue();

            if (result == MMI_FALSE)
            {
                DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 0, EM_NOTIFY_DURATION, 0);
                return;
            }

            g_bt_rf_test_cntx.last_cmd = EM_BT_RF_TEST_TYPE_RX;
            
            SetProtocolEventHandler(EmBtRfTestTXRXRspHdlr, MSG_ID_BT_ENGINEER_MODE_TXRX_TEST_CNF );    
	
            ClearInputEventHandler(MMI_DEVICE_ALL);
    
            msg_p = OslConstructDataPtr(sizeof(bt_engineer_mode_txrx_test_req_struct));

            msg_p->pattern = 1; /* means 0000 */

            if (g_bt_rf_test_cntx.currHoping == EM_BT_RF_TEST_HOPING_OFF)
            {
                msg_p->channel_hopping = 0;
                msg_p->tx_freq= 0; 
                msg_p->rx_freq= 0;
            }
            else /* EM_BT_RF_TEST_HOPING_ON */
            {
                msg_p->channel_hopping = 1;

                UnicodeToAnsii((PS8)textbuf,  (PS8)g_bt_rf_test_cntx.channel);
                value = atoi((const char *) textbuf);
                msg_p->tx_freq= 0; 
                msg_p->rx_freq= value;
            }

            switch (g_bt_rf_test_cntx.currPacketType)
            {
                case EM_BT_RF_TEST_PACKET_TYPE_DH1:
                    msg_p->packet_type= 0x04;
                    break;

                case EM_BT_RF_TEST_PACKET_TYPE_DH3:
                    msg_p->packet_type= 0x0B;
                    break;

                case EM_BT_RF_TEST_PACKET_TYPE_DH5:
                    msg_p->packet_type= 0x0F;
                    break;
					
            }

            UnicodeToAnsii((PS8)textbuf,  (PS8)g_bt_rf_test_cntx.data_len);
            value = atoi((const char *) textbuf);
            msg_p->packet_length= value;

            UnicodeToAnsii((PS8)textbuf,  (PS8)g_bt_rf_test_cntx.poll_period);
            value = atoi((const char *) textbuf);
            msg_p->poll_period = value; 

            EngineerModeSendMsg_Ext(MSG_ID_BT_ENGINEER_MODE_TXRX_TEST_REQ, MOD_BT, msg_p, NULL);

            /* Fion Add, Test */
            //EmBtRfTestTXRXRspHdlr(NULL);
#endif

            break;
        }
        case EM_BT_RF_TEST_TYPE_BD_ADDR:
        {
            U8 i=0;
            U8 addrbuf[13];
            bt_set_bd_param_req_struct *msg_p;

            g_bt_rf_test_cntx.last_cmd = EM_BT_RF_TEST_TYPE_BD_ADDR;
            
            msg_p = OslConstructDataPtr(sizeof(bt_set_bd_param_req_struct));

            UnicodeToAnsii((PS8) addrbuf, (PS8) g_bt_rf_test_cntx.BdAddr);
            
            /* Re-transfer BT address. And check if the bt address is valid.  */
            for (i = 0; i < 12; i++)
            {
                if (addrbuf[i] >= '0' && addrbuf[i] <= '9') /* 0 ~ 9 */
                {
                    addrbuf[i] = addrbuf[i] - '0';
                }
                else if (addrbuf[i] >= 'A' && addrbuf[i] <= 'F')    /* A ~ F */
                {
                    addrbuf[i] = addrbuf[i] - 'A' + 10;
                }
                else if (addrbuf[i] >= 'a' && addrbuf[i] <= 'f')    /* a ~ f */
                {
                    addrbuf[i] = addrbuf[i] - 'a' + 10;
                }
                else
                {
                    DisplayPopup((PU8) GetString(STR_GLOBAL_ERROR), IMG_GLOBAL_ERROR, 0, EM_NOTIFY_DURATION, 0);
                    DeleteUptoScrID(EM_DEV_BT_RF_TEST_TESTING_SCR_ID);
                    return;
                }
            }

            for (i=0;i<12;i+=2)
            {
                msg_p->bd_addr[i/2] = addrbuf[i]+addrbuf[i+1]*16;
            }

            EngineerModeSendMsg_Ext(MSG_ID_BT_SET_BD_ADDR_REQ, MOD_BT, msg_p, NULL);

            break;       	
        }
        case EM_BT_RF_TEST_TYPE_ENTER_TEST_MODE:
        {
            bt_test_mode_req_struct *msg_p;

            if (g_bt_rf_test_cntx.test_item == EM_BT_RF_TEST_TYPE_ENTER_TEST_MODE)
            {
                if ((g_bt_rf_test_cntx.last_cmd != EM_BT_RF_TEST_TYPE_POWER_ON) &&
                    (g_bt_rf_test_cntx.last_cmd != EM_BT_RF_TEST_TYPE_ENTER_TEST_MODE))
                {
                    DisplayPopup((PU8)GetString(STR_GLOBAL_NOT_AVAILABLE), IMG_GLOBAL_ERROR, 0, EM_NOTIFY_DURATION, 0);
                    return;
                }
            }

            g_bt_rf_test_cntx.last_cmd = EM_BT_RF_TEST_TYPE_ENTER_TEST_MODE;
            
            msg_p = OslConstructDataPtr(sizeof(bt_test_mode_req_struct));
            msg_p->mode = MMI_TRUE;
#ifdef __GEMINI__
/* under construction !*/
#endif
            EngineerModeSendMsg_Ext(MSG_ID_BT_TEST_MODE_REQ, MOD_BT, msg_p, NULL);

            /* no response, display sucess directly */
            EntryEmDevBtRfTesting();
            //EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);

            break;       	
        }
	 default:
	     ASSERT(0);
            break;
    }
}


/**************************************************************
**	FUNCTION NAME		: EmBtRfTestCheckTXRXValue
**
**  PURPOSE				:  
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
MMI_BOOL EmBtRfTestCheckTXRXValue(void)
{
    U8 value;
    U8 textbuf[10];

    /* To check if channel is valid */
    UnicodeToAnsii((PS8)textbuf,  (PS8)g_bt_rf_test_cntx.channel);
    value = atoi((const char *) textbuf);
    if (value > 78)
    {
        return MMI_FALSE;
    }
    return MMI_TRUE;    
}



/*****************************************************************************
 * FUNCTION
 *  EmBtRfTestTXRXRspHdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  inMsg       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void EmBtRfTestTXRXRspHdlr(void *inMsg)
{
#ifdef __BT_RDA5868__
        EntryEmDevBtRfTesting();
#else
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    bt_engineer_mode_txrx_test_cnf_struct *rsp_p = inMsg;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (rsp_p->result)
    {
        EntryEmDevBtRfTesting();
    }
    else
    {
        EngineerModeErrorNotice(0);
    }
#endif
}


/*****************************************************************************
 * FUNCTION
 *  EmBtRfTestPwrCtrlLevelRspHdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  inMsg       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void EmBtRfTestPwrCtrlLevelRspHdlr(void *inMsg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    bt_engineer_mode_txrx_power_cnf_struct *rsp_p = inMsg;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (rsp_p->result)
    {
        EngineerModeSuccessNotice(0);
    }
    else
    {
        EngineerModeErrorNotice(0);
    }
}


/*****************************************************************************
 * FUNCTION
 *  EntryEmDevBtRfTesting
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryEmDevBtRfTesting(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;
    U16 title=0, image=0;
    U8 *info;
    S8 buffer[32];
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(EM_DEV_BT_RF_TEST_TESTING_SCR_ID, NULL, EntryEmDevBtRfTesting, NULL);
    guiBuffer = GetCurrGuiBuffer(EM_DEV_BT_RF_TEST_TESTING_SCR_ID);
    
    info = (PU8) subMenuData;
    
    if (g_bt_rf_test_cntx.test_item == EM_BT_RF_TEST_TYPE_TX)
    {
        title = EM_DEV_BT_RF_TEST_TX_TEXT;

        UCS2Strcpy((PS8) info, (PS8) GetString(EM_DEV_BT_RF_TEST_CTRL_PWR_LEVEL_TEXT));
        UCS2Strcat((PS8) info, (PS8) L":\n\n\n");
        sprintf(buffer, "%d", (U16)g_bt_rf_test_cntx.currCtrlPwrLevel);
        AnsiiToUnicodeString((PS8) & info[UCS2Strlen((PS8) info) << 1], (PS8) buffer);
        UCS2Strcat((PS8) info, (PS8) L"\n");

        SetKeyHandler(EmBtRfTestPwrLevelUp, KEY_UP_ARROW, KEY_EVENT_UP);
        SetKeyHandler(EmBtRfTestPwrLevelDown, KEY_DOWN_ARROW, KEY_EVENT_UP);
		
    }
    else if (g_bt_rf_test_cntx.test_item == EM_BT_RF_TEST_TYPE_RX)
    {
        title = EM_DEV_BT_RF_TEST_RX_TEXT;
#ifdef __BT_RDA5868__
        g_bt_rf_test_cntx.currCtrlPwrLevel = 15;
#endif
        UCS2Strcpy((PS8) info, (PS8) GetString(EM_DEV_BT_RF_TEST_CTRL_PWR_LEVEL_TEXT));
        UCS2Strcat((PS8) info, (PS8) L":\n\n\n");
        sprintf(buffer, "%d", (U16)g_bt_rf_test_cntx.currCtrlPwrLevel);
        AnsiiToUnicodeString((PS8) & info[UCS2Strlen((PS8) info) << 1], (PS8) buffer);
        UCS2Strcat((PS8) info, (PS8) L"\n");

        SetKeyHandler(EmBtRfTestPwrLevelUp, KEY_UP_ARROW, KEY_EVENT_UP);
        SetKeyHandler(EmBtRfTestPwrLevelDown, KEY_DOWN_ARROW, KEY_EVENT_UP);
		
    }
    else if (g_bt_rf_test_cntx.test_item == EM_BT_RF_TEST_TYPE_BD_ADDR)
    {
        UCS2Strcpy((PS8) info, (PS8) GetString(EM_GLOBAL_PROCESSING_TEXT));
        title = EM_DEV_BT_RF_TEST_BD_ADDR_TEXT;
        image = IMG_GLOBAL_PROGRESS;
    }
    else if (g_bt_rf_test_cntx.test_item == EM_BT_RF_TEST_TYPE_ENTER_TEST_MODE)
    {
        UCS2Strcpy((PS8) info, (PS8) GetString(STR_GLOBAL_ACTIVATED));
        title = EM_DEV_BT_RF_TEST_MODE_TEXT;
        image = IMG_GLOBAL_PROGRESS;
    }

    /* Show progress screen */
    ShowCategory66Screen(
        title,
        0,
        0,
        0,
        STR_GLOBAL_CANCEL,
        0,
        (PU8) subMenuData,
        image,
        NULL);

    if ((g_bt_rf_test_cntx.test_item == EM_BT_RF_TEST_TYPE_TX)||
        (g_bt_rf_test_cntx.test_item == EM_BT_RF_TEST_TYPE_RX)     
#ifdef __BT_RDA5868__
        ||(g_bt_rf_test_cntx.test_item == EM_BT_RF_TEST_TYPE_BD_ADDR)
#endif
        )        
    {
		SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
		SetKeyHandler(GoBackHistory, KEY_END, KEY_EVENT_DOWN);
//        SetLeftSoftkeyFunction(EMRFTESTWifiTXContPKTCheckAndSendReq, KEY_EVENT_UP);
    }
    else if (g_bt_rf_test_cntx.test_item == EM_BT_RF_TEST_TYPE_ENTER_TEST_MODE)
    {
		SetRightSoftkeyFunction(EmBtRfTestExit, KEY_EVENT_UP);
        SetKeyHandler(EmBtRfTestExit, KEY_END, KEY_EVENT_DOWN);
    }

    /* Set RSK to abort progress */
    

}


/*****************************************************************************
 * FUNCTION
 *  EmBtRfTestPwrLevelUp
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EmBtRfTestPwrLevelUp(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 title=0;
    U8 *info;
    S8 buffer[32];
    bt_engineer_mode_power_test_req_struct *msg_p;
		
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    if (g_bt_rf_test_cntx.currCtrlPwrLevel < 7)
    {
        g_bt_rf_test_cntx.currCtrlPwrLevel ++;
    }
    else
    {
        g_bt_rf_test_cntx.currCtrlPwrLevel  = 7;
    }

    title = EM_DEV_BT_RF_TEST_RX_TEXT;
    info = (PU8) subMenuData;
	
    UCS2Strcpy((PS8) info, (PS8) GetString(EM_DEV_BT_RF_TEST_CTRL_PWR_LEVEL_TEXT));
    UCS2Strcat((PS8) info, (PS8) L":\n\n\n");
    sprintf(buffer, "%d", (U16)g_bt_rf_test_cntx.currCtrlPwrLevel);
    AnsiiToUnicodeString((PS8) & info[UCS2Strlen((PS8) info) << 1], (PS8) buffer);
    UCS2Strcat((PS8) info, (PS8) L"\n");

    /* Show progress screen */
    ShowCategory66Screen(
        title,
        0,
        0,
        0,
        STR_GLOBAL_CANCEL,
        0,
        (PU8) subMenuData,
        0,
        NULL);

    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_END, KEY_EVENT_DOWN);

    SetKeyHandler(EmBtRfTestPwrLevelUp, KEY_UP_ARROW, KEY_EVENT_UP);
    SetKeyHandler(EmBtRfTestPwrLevelDown, KEY_DOWN_ARROW, KEY_EVENT_UP);

#ifdef __MMI_FOURWAYS_KEY_DISABLE__
    SetKeyHandler(EmBtRfTestPwrLevelUp, KEY_VOL_UP, KEY_EVENT_UP);
    SetKeyHandler(EmBtRfTestPwrLevelDown, KEY_VOL_DOWN, KEY_EVENT_UP);
#endif /* __MMI_FOURWAYS_KEY_DISABLE__ */ 

    SetProtocolEventHandler(EmBtRfTestPwrCtrlLevelRspHdlr, MSG_ID_BT_ENGINEER_MODE_POWER_TEST_CNF );    
    msg_p = OslConstructDataPtr(sizeof(bt_engineer_mode_power_test_req_struct));
    msg_p->level = g_bt_rf_test_cntx.currCtrlPwrLevel;
    EngineerModeSendMsg_Ext(MSG_ID_BT_ENGINEER_MODE_POWER_TEST_REQ, MOD_BT, msg_p, NULL);

    /* Fion Add, Test */
    //EmBtRfTestPwrCtrlLevelRspHdlr(NULL);


}



/*****************************************************************************
 * FUNCTION
 *  EmBtRfTestPwrLevelDown
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EmBtRfTestPwrLevelDown(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 title=0;
    U8 *info;
    S8 buffer[32];
    bt_engineer_mode_power_test_req_struct *msg_p;
		
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    if (g_bt_rf_test_cntx.currCtrlPwrLevel > 1)
    {
        g_bt_rf_test_cntx.currCtrlPwrLevel --;
    }
    else
    {
        g_bt_rf_test_cntx.currCtrlPwrLevel = 1;
    }

    title = EM_DEV_BT_RF_TEST_RX_TEXT;
    info = (PU8) subMenuData;
	
    UCS2Strcpy((PS8) info, (PS8) GetString(EM_DEV_BT_RF_TEST_CTRL_PWR_LEVEL_TEXT));
    UCS2Strcat((PS8) info, (PS8) L":\n\n\n");
    sprintf(buffer, "%d", (U16)g_bt_rf_test_cntx.currCtrlPwrLevel);
    AnsiiToUnicodeString((PS8) & info[UCS2Strlen((PS8) info) << 1], (PS8) buffer);
    UCS2Strcat((PS8) info, (PS8) L"\n");

    /* Show progress screen */
    ShowCategory66Screen(
        title,
        0,
        0,
        0,
        STR_GLOBAL_CANCEL,
        0,
        (PU8) subMenuData,
        0,
        NULL);

    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    SetKeyHandler(GoBackHistory, KEY_END, KEY_EVENT_DOWN);

    SetKeyHandler(EmBtRfTestPwrLevelUp, KEY_UP_ARROW, KEY_EVENT_UP);
    SetKeyHandler(EmBtRfTestPwrLevelDown, KEY_DOWN_ARROW, KEY_EVENT_UP);

#ifdef __MMI_FOURWAYS_KEY_DISABLE__
    SetKeyHandler(EmBtRfTestPwrLevelUp, KEY_VOL_UP, KEY_EVENT_UP);
    SetKeyHandler(EmBtRfTestPwrLevelDown, KEY_VOL_DOWN, KEY_EVENT_UP);
#endif /* __MMI_FOURWAYS_KEY_DISABLE__ */ 


    SetProtocolEventHandler(EmBtRfTestPwrCtrlLevelRspHdlr, MSG_ID_BT_ENGINEER_MODE_POWER_TEST_CNF );    
    msg_p = OslConstructDataPtr(sizeof(bt_engineer_mode_power_test_req_struct));
    msg_p->level = g_bt_rf_test_cntx.currCtrlPwrLevel;
    EngineerModeSendMsg_Ext(MSG_ID_BT_ENGINEER_MODE_POWER_TEST_REQ, MOD_BT, msg_p, NULL);

}


/**************************************************************

	FUNCTION NAME		: ExitEmDevBtRfTXRX()

  	PURPOSE				: Exit Em BT RF Test inline select screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitEmDevBtRfTXRX(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/


    history_t h;
    U16 inputBufferSize;
	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CloseCategory57Screen();

    h.scrnID = EM_DEV_BT_RF_TEST_TXRX_SCR_ID;
    h.entryFuncPtr = EntryEmDevBtRfTXRX;
    GetCategory57History(h.guiBuffer);
    inputBufferSize = (U16) GetCategory57DataSize();
    GetCategory57Data((U8*) h.inputBuffer);
    AddNHistory(h, inputBufferSize);

}


/**************************************************************
**	FUNCTION NAME		: HighlightEmDevBtRfBdAddr
**
**  	PURPOSE			: High light function to BT RF Test BD Addr menu
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmDevBtRfBdAddr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_bt_rf_test_cntx.test_item = EM_BT_RF_TEST_TYPE_BD_ADDR;

    SetProtocolEventHandler(EmBtRfTestSetBdAddrRspHdlr, MSG_ID_BT_ENGINEER_MODE_CNF );    
	
    SetLeftSoftkeyFunction(EntryEmDevBtRfBdAddr, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	
}


/**************************************************************
**	FUNCTION NAME		: EntryEmDevBtRfBdAddr
**
**  PURPOSE				: Enrty function for BT RF Test BD ADDR
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmDevBtRfBdAddr(void)
{
    U16 ImageList[2];
    ImageList[0] = ImageList[0];

    if ((g_bt_rf_test_cntx.last_cmd != EM_BT_RF_TEST_TYPE_POWER_ON) &&
        (g_bt_rf_test_cntx.last_cmd != EM_BT_RF_TEST_TYPE_BD_ADDR))
    {
        DisplayPopup((PU8)GetString(STR_GLOBAL_NOT_AVAILABLE), IMG_GLOBAL_ERROR, 0, EM_NOTIFY_DURATION, 0);
        return;
    }
#ifdef __BT_RDA5868__
    g_bt_rf_test_cntx.last_cmd = EM_BT_RF_TEST_TYPE_BD_ADDR;
 //   RDA5868_send_vco_test();
    EntryEmDevBtRfTesting();
#else
    U8* guiBuffer;
    EntryNewScreen(EM_DEV_BT_RF_TEST_BD_ADDR_SCR_ID, ExitEmDevBtRfBdAddr, NULL, NULL);
    InitializeCategory57Screen();
	
    guiBuffer = GetCurrGuiBuffer(EM_DEV_BT_RF_TEST_BD_ADDR_SCR_ID);

   /* BD Addr */
    SetInlineItemActivation(&wgui_inline_items[0], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemCaption(&wgui_inline_items[0], (U8*)GetString(EM_DEV_BT_RF_TEST_BD_ADDR_TEXT));
    ImageList[0] = 0;              
    
    SetInlineItemActivation(&wgui_inline_items[1], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
    SetInlineItemTextEdit(&wgui_inline_items[1], (PU8)g_bt_rf_test_cntx.BdAddr, 13, INPUT_TYPE_ALPHANUMERIC_LOWERCASE);
    ImageList[1] = IMG_STATUS;

    DisableCategory57ScreenDone();

    ShowCategory57Screen(EM_DEV_BT_RF_TEST_BD_ADDR_TEXT, 0, 
       	                 STR_GLOBAL_OK, 
       	                 IMG_GLOBAL_OK, 
       	                 STR_GLOBAL_BACK, 
       	                 IMG_GLOBAL_BACK,
				         2, ImageList, wgui_inline_items, 0, guiBuffer);	
    
    SetCategory57RightSoftkeyFunctions( EMBtRfTestSendReq, GoBackHistory );

    /* Register END key handler */
    SetKeyHandler(GoBackHistory,KEY_END,KEY_EVENT_DOWN);
#endif
   
}


/**************************************************************

	FUNCTION NAME		: ExitEmDevBtRfBdAddr()

  	PURPOSE				: Exit Em BT RF Test inline select screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExitEmDevBtRfBdAddr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    CloseCategory57Screen();
}


/*****************************************************************************
 * FUNCTION
 *  EmBtRfTestSetBdAddrRspHdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  inMsg       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void EmBtRfTestSetBdAddrRspHdlr(void *inMsg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    bt_engineer_mode_cnf_struct *rsp_p = inMsg;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    extern void ShutdownSystemOperation(void);
    
    if (rsp_p->result)
    {
        EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

        ShowCategory64Screen(EM_NOTICE_SUCCESS_REBOOT_TEXT, IMG_GLOBAL_WARNING, 0);
        StartTimer(EM_NOTIFYDURATION_TIMER, 3000, ShutdownSystemOperation);
    }
    else
    {
        EngineerModeErrorNotice(0);
    }
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmDevBtRfBdEnterTestMode
**
**  	PURPOSE			: High light function to BT RF Test Test Mode menu
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmDevBtRfBdEnterTestMode(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_bt_rf_test_cntx.test_item = EM_BT_RF_TEST_TYPE_ENTER_TEST_MODE;

    SetLeftSoftkeyFunction(EMBtRfTestSendReq, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	
}



#endif /* #ifdef __MMI_EM_BT_RF_TEST__ */

#ifdef __MMI_EM_BT_GET_CHIP_VERION__

/**************************************************************
**	FUNCTION NAME		: HighlightEmBTGetChipVersionMenu
**
**  	PURPOSE			: 
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmBTGetChipVersionMenu(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetLeftSoftkeyFunction(EmDevBtChipVersion, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	
}


/**************************************************************

	FUNCTION NAME		: EmDevBtChipVersion

  	PURPOSE				: Send BT RF Test req

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EmDevBtChipVersion(void) 
{
#ifdef __BT_RDA5868__
    if (mmi_bt_is_power_on(MMI_FALSE))
    {
        DisplayPopup(
            (U8*) GetString(EM_DEV_BT_RF_TEST_TURN_OFF_BT_TEXT),
            IMG_GLOBAL_WARNING,
            1,
            UI_POPUP_NOTIFYDURATION_TIME,
            (U8) WARNING_TONE);
        return;
    }

    EmBtGetChipVersionRspHdlr(NULL);
#else
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    bt_get_bt_version_cnf_struct test;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearInputEventHandler(MMI_DEVICE_ALL);

    SetProtocolEventHandler(EmBtGetChipVersionRspHdlr, MSG_ID_BT_GET_BT_VERSION_CNF );    
    EngineerModeSendMsg_Ext(MSG_ID_BT_GET_BT_VERSION_REQ, MOD_BT, NULL, NULL);

    /* Fion for test */
    EmBtGetChipVersionRspHdlr(&test);
#endif
}

void ExitEmBtGetChipVersionRspHdlr()
{
	rda_bt_poweroff_for_test();
}
/*****************************************************************************
 * FUNCTION
 *  EmBtGetChipVersionRspHdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  inMsg       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void EmBtGetChipVersionRspHdlr(void *inMsg)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
 
    U8 *guiBuffer, *info;
    S8 buffer[40];
  
     
#ifdef __BT_RDA5868__
	U8 chip_id=0;
       U8 lmp_id=0;
	

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
        EntryNewScreen(EM_BT_GET_CHIP_VERSION_SCR_ID, ExitEmBtGetChipVersionRspHdlr, NULL, NULL);

        guiBuffer = GetCurrGuiBuffer(EM_BT_GET_CHIP_VERSION_SCR_ID);

        memset(subMenuData, 0, sizeof(subMenuData));
        info = (PU8) subMenuData;
        rda_bt_poweron_for_test();
	lmp_id = rdabt_get_lmpversion();
	chip_id = rdabt_get_chipid();

     mmi_trace(g_sw_BT, "lmp_id=%d, chip_id=%d", lmp_id, chip_id);
        /* BT Chip version */
        sprintf(buffer, "BT Chip Version :");
        AnsiiToUnicodeString((PS8) & info[UCS2Strlen((PS8) info) << 1], (PS8) buffer);
        UCS2Strcat((PS8) info, (PS8) L"\n");               

        sprintf(buffer, "%s", em_bt_chip_version[chip_id].chip_name);
        AnsiiToUnicodeString((PS8) & info[UCS2Strlen((PS8) info) << 1], (PS8) buffer);
        UCS2Strcat((PS8) info, (PS8) L"\n");	
	
            
        /* BT Lmp version */
        sprintf(buffer, "BT Lmp Version :");
        AnsiiToUnicodeString((PS8) & info[UCS2Strlen((PS8) info) << 1], (PS8) buffer);
        UCS2Strcat((PS8) info, (PS8) L"\n");        

        sprintf(buffer, "%s", em_bt_lmp_version[lmp_id].lmp_name);
        AnsiiToUnicodeString((PS8) & info[UCS2Strlen((PS8) info) << 1], (PS8) buffer);
        UCS2Strcat((PS8) info, (PS8) L"\n");



        ShowCategory74Screen(EM_BT_GET_CHIP_VERSION_STR_ID,
                            (U16) 0,
                            0,
                            (U16) 0,
                            STR_GLOBAL_BACK,
                            0,
                            (PU8) subMenuData,
                            MAX_SUB_MENUS * MAX_SUB_MENU_SIZE,
                            guiBuffer);
        
        SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);



#else

     U8 i;
     bt_get_bt_version_cnf_struct *rsp_p = inMsg;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (rsp_p->result)
    {
        EntryNewScreen(EM_BT_GET_CHIP_VERSION_SCR_ID, NULL, NULL, NULL);

        guiBuffer = GetCurrGuiBuffer(EM_BT_GET_CHIP_VERSION_SCR_ID);

        memset(subMenuData, 0, sizeof(subMenuData));
        info = (PU8) subMenuData;
        
        /* BT Chip version */
        sprintf(buffer, "BT Chip Version :");
        AnsiiToUnicodeString((PS8) & info[UCS2Strlen((PS8) info) << 1], (PS8) buffer);
        UCS2Strcat((PS8) info, (PS8) L"\n");
               
        for (i=0;i<(sizeof(em_bt_chip_version)/sizeof(em_bt_chip_version_struct));i++)
        {
            if (rsp_p->chip_version == em_bt_chip_version[i].chip_ver)
            {
                sprintf(buffer, "%s", em_bt_chip_version[i].chip_name);
                AnsiiToUnicodeString((PS8) & info[UCS2Strlen((PS8) info) << 1], (PS8) buffer);
                UCS2Strcat((PS8) info, (PS8) L"\n");
				break;
            }
        }

            
        /* BT Lmp version */
        sprintf(buffer, "BT Lmp Version :");
        AnsiiToUnicodeString((PS8) & info[UCS2Strlen((PS8) info) << 1], (PS8) buffer);
        UCS2Strcat((PS8) info, (PS8) L"\n");
        
        for (i=0;i<(sizeof(em_bt_lmp_version)/sizeof(em_bt_lmp_version_struct));i++)
        {
            if (rsp_p->lmp_version == em_bt_lmp_version[i].lmp_ver)
            {
                sprintf(buffer, "%s", em_bt_lmp_version[i].lmp_name);
                AnsiiToUnicodeString((PS8) & info[UCS2Strlen((PS8) info) << 1], (PS8) buffer);
                UCS2Strcat((PS8) info, (PS8) L"\n");
				break;
            }
        }

        ShowCategory74Screen(EM_BT_GET_CHIP_VERSION_STR_ID,
                            (U16) 0,
                            0,
                            (U16) 0,
                            STR_GLOBAL_BACK,
                            0,
                            (PU8) subMenuData,
                            MAX_SUB_MENUS * MAX_SUB_MENU_SIZE,
                            guiBuffer);
        
        SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    }
    else
    {
        EngineerModeErrorNotice(0);
    }
#endif
}
#endif /* #ifdef __MMI_EM_BT_GET_CHIP_VERION__ */
#include "lcd_if.h"
#include "lcd_sw_inc.h"
extern kal_uint32 lcd_frame_buffer_address;
extern void gdi_memset16(U8 *address, U16 pixel, int size);


void TestLCD()
{
	gdi_memset16( (U8*)lcd_frame_buffer_address,0x1f, LCD_WIDTH*LCD_HEIGHT*2);
	lcd_power_up();
       BlockWrite_DI(0,0, LCD_WIDTH-1, LCD_HEIGHT-1);
       lcd_power_down();
	COS_Sleep(1000);
	
	gdi_memset16( (U8*)lcd_frame_buffer_address,0x7e0, LCD_WIDTH*LCD_HEIGHT*2);
	lcd_power_up();
       BlockWrite_DI(0,0, LCD_WIDTH-1, LCD_HEIGHT-1);
       lcd_power_down();
	COS_Sleep(1000);

	gdi_memset16( (U8*)lcd_frame_buffer_address,0xf800, LCD_WIDTH*LCD_HEIGHT*2);
	lcd_power_up();
       BlockWrite_DI(0,0, LCD_WIDTH-1, LCD_HEIGHT-1);
       lcd_power_down();
	COS_Sleep(1000);

}
#endif /* __MMI_ENGINEER_MODE__ */

