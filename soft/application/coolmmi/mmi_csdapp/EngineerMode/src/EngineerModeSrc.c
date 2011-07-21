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
 * EngineerModeSrc.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is for Engineering mode
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
 *
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
 
 
/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
//... Add More MMI header
#undef __NEWSIMULATOR 
#include "adp_events.h"
//CSD End
#undef __TCPIP__//JIASHUO ADD TEMP
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
#include "globaldefs.h"
#include "globalscrenum.h"

#ifdef MMI_ON_HARDWARE_P
#include "device.h"
#include "uart_sw.h"
#include "custom_em.h"
#include "nvram_data_items.h"
#include "nvram_user_defs.h"
#include "nvram_default_audio.h"
#include "custom_nvram_editor_data_item.h"
#include "datetimegprot.h"
#else
#include "custom_em.h"
#endif

#include "timerevents.h"
#include "mainmenudef.h"
#include "engineermodedef.h"
#include "engineermodetype.h"
#include "engineermodeprot.h"
#include "engineermodedeviceinc.h"
#include "engineermodesocketapp.h"
#include "mmi_data_types.h"
#include "customcfg.h"
#include "unicodexdcl.h"
#include "audioinc.h"
#include "idleappdef.h"
#include "nvramenum.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "gpioinc.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "factorymodedef.h"
 
//#include "fs_type.h"    
#include "sst_export.h"
#include "ex_item.h"
#include "commonscreens.h"
#include "networksetupdefs.h"
//Lisen 011605
#include "debuginitdef.h"

#ifdef __MMI_USB_SUPPORT__
#include "usbdevicegprot.h"
#include "usbdevicedefs.h"
#endif

#if defined(WAP_SUPPORT) 
#include "wapadp.h"
#endif

#ifdef __MMI_TVOUT__ 
	#include "mdi_datatype.h"
	#include "mdi_tv.h"

	#ifdef MMI_ON_HARDWARE_P 
		#include "med_api.h"
	#endif /* MMI_ON_HARDWARE_P */
#endif

#if defined(__MMI_VIDEO__) && defined(ISP_SUPPORT)
	#include "med_api.h"
#endif 

#ifndef __GPRS_MODE__
#define GotoEmGprsMenu EntryEmGprsMenuNotSupport
#else
#define GotoEmGprsMenu EntryEmGprsMenu
#endif

#include "custom_data_account.h"

/**************************************************************/
/*********************** Global Variables *************************/
/**************************************************************/
 
#ifdef MMI_ON_HARDWARE_P
extern UART_PORT TST_PORT;
extern UART_baudrate TST_BAUDRATE;
#ifndef L4_NOT_PRESENT
extern UART_PORT PS_UART_PORT;
extern UART_baudrate PS_BAUDRATE;
#endif
#endif

/* ======= Display Buffer for Engineering Mode and Factory Mode ======== */
extern S8 EMFMUnicodeDisplayBuf[MAX_TEST_STRING_BUF*2];
extern U8 EMFMAsciiDisplayBuf[MAX_TEST_STRING_BUF];

/* ======= UI ======== */
U8 currentHighlightIndex;
EM_APP_UART_SETTING EmPortSetting;
extern PU8 subMenuDataPtrs[MAX_SUB_MENUS];
extern U8 subMenuData[MAX_SUB_MENUS][MAX_SUB_MENU_SIZE];
extern U8 gCurrentMode;
extern PROFILE gprofiles[];
extern PROFILE gcurrentprofile;
extern U8 gprofileId;
extern U8 gactivatedprofile;
extern U16 gstartMidiId;
extern U16 gstartGeneralToneId;
extern U8 glastactivatedprofile;
extern U8  gSec;
extern U8 gprofilenvram;
extern U8 glastColorID ;
extern U8 gcurrentColorId;
extern U8 gpopupToneId;
extern S32 gcurrHiliteIndexOne;					/* Current hilite index value */
extern U16 g_profiles_melody_list_index;
extern S32 gcommonvolumehandler;
extern U16 gcurrentaudioId;
extern U16 glastaudioId;
extern U16 gstartRingId;
extern U16 gtotalRingId;
extern U16 gstartMidiId;
extern U16 gtotalMidiId;
extern S32 gringvol_level;
extern S32 gkeypadvol_level;

/* ======= Network Info ======== */
U8 gEmInfoOn = 0;
U8 gEmInfoPag = 0;
extern UI_character_type InputBuf[8];
EmCommandTypeEnum currentEmCommand=EM_NONE;
U32 currentEmMode = 0;
U32 newEmMode = 0;
U8 EMBandIndex[EM_BANDSEL_MAX];
U8 EmModeEnabled[MAX_EM_MODE]=
{
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0
};

static rr_em_channel_descr_info_struct channel_descr_info;
static rr_em_measurement_report_info_struct meas_report_info;
static rr_em_control_msg_info_struct control_msg_info;
static rr_em_radio_link_counter_info_struct radio_link_counter_info;
S8* channel_type[] =
{
"PBCCH",
"PRACH",
"PCCCH",
"PDTCH",
"PACCH",
"BCCH",
"N_BCCH",
"E_BCCH",
"CCCH",
"PAG_CH",
"RACH",
"SDCCH4_0",
"SDCCH4_1",
"SDCCH4_2",
"SDCCH4_3",
"SDCCH8_0",
"SDCCH8_1",
"SDCCH8_2",
"SDCCH8_3",
"SDCCH8_4",
"SDCCH8_5",
"SDCCH8_6",
"SDCCH8_7",
"SDCCH",
"SACCH",
"FACCH",
"TCH_EF",
"TCH_F",
"TCH_H_0",
"TCH_H_1",
"TCH_H",
"CBCH",
"PPCH"
};

stFontAttribute	idle_screen_em_info_font={ 0,0,0,SMALL_FONT,0,0 };
U8 activeEmModeIndex;

/* ======= Audio ======== */
 static U16 currentVolume = 0;
static EmVolumeTypeEnum currentVolumeType;
static EmVolumeModeEnum currentVolumeMode;
U16    gNoOfRing = 0;
U16    gNoOfImy = 0;
U16    gNoOfMidi = 0;
U16    gNoOfSound = 0;
U8      gFixedImyBegin;
U8      gFixedImyEnd;
U8      gFixedMidiBegin;
U8      gFixedMidiEnd;
U8      gFixedSoundBegin;
U8      gFixedSoundEnd;
EmAudioProfileStruct gAudioProfile;
EmAudioProfileStruct gAudioProfileOrg;
U8 activeRingToneIndex;
S32 currentKeyShift=0;
BOOL isPopup;
EmAudioParamStruct AudioParameter;
static U16 CurAudParam, Delay1, Energy1, Delay2, Energy2, MaxSwing;
extern UI_character_type InputBuf[8];

/* =======  Device ======*/
U8 gIsBackLightOn = 0;

#ifdef __MMI_TVOUT__
	BOOL	gIsTVOut = FALSE;
	U16	gTVOutIdx;
#endif 

#if defined(__MMI_VIDEO__) && defined(ISP_SUPPORT)
	U16	gVideoHiBitrateIdx;
#endif /* __MMI_VIDEO__ && ISP_SUPPORT */


const U8 AUTO_RESET_WARNING[] = {'D', 0, 'o', 0, 'n', 0, '\'', 0, 't', 0, ' ', 0,
                                                            's', 0, 'u', 0, 'b', 0, 'm', 0, 'i', 0, 't', 0, ' ', 0,
                                                            'a', 0, 's', 0, ' ', 0, 'a', 0, ' ', 0, 'b', 0, 'u', 0, 'g', 0, '!', 0, 0,0};

/* =======  UART ======*/
static wgui_inline_item EmInlineEditorStruct[5+1];
static U8* EmUartPortText[EM_UART_MAX_PORT_NUM];
static U8* EmUartBaudText[EM_BAUD_NUM];
U32 EmUartPortHighlight, EmUartBaudHighlight;
U8 EmAppPortArray[EM_APP_MAX_NUM];
U8 EmAppBaudArray[EM_APP_MAX_NUM];
static U8 gIsFromEmSetUartPort=0;

//Lisen 07132005
extern kal_bool UART_setting;
#ifdef MMI_ON_HARDWARE_P
extern kal_bool High_Speed_SIM;
#endif

/* ======= GPRS ======*/
S8 EmGprsSendDataSizeBuffer[10];
S16 EmSendDataScreenDigitKeyCode=-1;
#ifdef __MOD_TCM__
extern U8  g_gprs_pdp_idx;
extern kal_bool  g_gprs_set_minqos_required;
#endif

/* ======= Backlight ======= */
static U8 backlight_mode=BACKLIGHT_NORMAL;

/* ======= Auto Testing Settings ====== */
S16 gAutoTestListSize = 0;
AutoTestItemArray nvramTestItemArray; 
AutoTestResultArray nvramTestResultArray; 
rtc_format_struct myclocktime;
S32 gPriorityValue = 0;
U16 gHiliteAutoTestCurrSelIdx = 0;
U16 gHiliteAllTestListIdx = 0;
U16 newPriority = 0;
PU8 AutoTestSelectedItems[MAX_AUTO_TEST_ITEMS];
//Lisen 0804
//U8   AutoTestItemList[MAX_AUTO_TEST_ITEMS][MAX_TEST_ITEM_NAME_LEN*ENCODING_LENGTH];
extern const testlet Tests[MAX_AUTO_TEST_ITEMS]; 

/* ======= Profile ======= */
extern U8 gprofileId;
extern U8 gactivatedprofile;
extern U8 glastactivatedprofile ;
extern U8 gcurrprofileID;

/* ======= WAP ======= */
#if defined(WAP_SUPPORT) 
extern kal_bool wap_enable_debug_assert;
extern kal_bool global_offline_page;
extern U8 global_wap_user_agent;
extern U8 global_wap_accept_header;
extern U8 global_mms_version;
#elif defined(MMI_ON_WIN32)
kal_bool wap_enable_debug_assert = 0; 
kal_bool global_offline_page = 0;
U8 global_wap_user_agent = 0;
U8 global_wap_accept_header = 0;
U8 global_mms_version = 0;
#endif

/* ======= Power Down Control ======= */
#define  PWRDOWNSTRLEN 10      
extern wgui_inline_item wgui_inline_items[];
U8 Buf_PDN_CON0[PWRDOWNSTRLEN*2];
U8 Buf_PDN_CON1[PWRDOWNSTRLEN*2];
U8 Buf_PDN_CON2[PWRDOWNSTRLEN*2];
U8 Buf_PDN_CON3[PWRDOWNSTRLEN*2];

/* ======= FS Trace Info ======= */
static BOOL FS_TRACE_ON = FALSE;

#if defined(__AMR_SUPPORT__)
/* ======= AMR Info ======= */
static BOOL AMR_ON = FALSE;
U8       mscap[NVRAM_EF_MSCAP_SIZE];
#endif

/* ======= Socket Test ======= */
#ifdef __TCPIP__ 

S8 EmSocketInputBuff[(EM_SOCKET_MAX_INPUT_LEN+1)*ENCODING_LENGTH];
static mmi_soc_demo_app_enum currentSocketAppType;
U8 	EmSocketIsCallbackFuncCalled;
U8	EmSocketIP1[EM_SOCKET_IP_LEN * ENCODING_LENGTH ];
U8	EmSocketIP2[EM_SOCKET_IP_LEN * ENCODING_LENGTH ];
U8	EmSocketIP3[EM_SOCKET_IP_LEN * ENCODING_LENGTH ];
U8	EmSocketIP4[EM_SOCKET_IP_LEN * ENCODING_LENGTH ];
kal_uint8 EmServerIPAddress[4];
mmi_soc_demo_app_result	EmSocketResult;
PS8 EmSocketResultBuffer;
S32 EmSocketResultBufferSize;

#endif /* __TCPIP__ */

//Lisen 0921
/* ======= Power On Duration Info ======= */
extern MYTIME StartUpTime, LastDuration;

/**************************************************************/
/*********************** Extern functions *************************/
/**************************************************************/
//Todo: Shall provide an primitive base interface for MMI to set echo suppression
extern void L1SP_SetEchoSuppressionParameter( U16 timeConst, U16 volConst );
extern void SetBacklightModeExe(U8 mode);
extern void DTGetRTCTime(MYTIME *t);
extern void ShowCategory63Screen(U8* message,U16 message_icon,U8* history_buffer);
extern void DisplayPopup(U8 * string, U16 imageId, U8 imageOnBottom, U32 popupDuration, U8 toneId);
extern void ShowCategory16Screen(U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,U8* Buffer, U32 BufferLength,U8* history_buffer);
extern void SetCategory16RightSoftkeyFunction(void (*f)(void),MMI_key_event_type k);
extern kal_bool SST_AdvRAMTest(void);

//extern void ShowCategory164Screen(U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,U16 message,U16 message_icon,U8* history_buffer);
extern void playRequestedTone(ALL_TONE_ENUM playtone);
extern void ShowCategory64Screen(U16 message,U16 message_icon,U8* history_buffer);
extern void Category53ChangeItemDescription(s32 index,U8 *text);

#ifdef __TCPIP__ 
extern void stopRequestedTone(ALL_TONE_ENUM playtone);
extern void ReturnThreeDigitString(PU8 StringVal, U8 DigitVal );
extern void PreparedIPAddressString(U8* string_buffer,U8 *IP1,U8 *IP2,U8 *IP3,U8 *IP4);
extern void soc_init_win32(void);
#endif /*  __TCPIP__  */

#ifdef __MMI_TVOUT__
extern void mmi_phnset_tvout_event_hdlr(MDI_RESULT result);
#endif /* __MMI_TVOUT__ */

#ifdef MMI_ON_HARDWARE_P
extern void L1sim_Enable_Enhanced_Speed(kal_bool enable);
#endif

//Lisen 09062005
#ifndef __DYNAMIC_UART_PORT_SWITCHING__
extern void ShutdownSystemOperation(void);
#endif

void HighlightEmCellEnvironment(void);

void DisplayConfirm(U16 LSK_str, U16 LSK_img, U16 RSK_str, U16 RSK_img, UI_string_type message, UI_image_ID_type message_image, U8 toneId);

 
#include "engineermodepatch.h"
extern kal_uint32 INT_MemoryDumpFlag;
extern BOOL g_mmi_frm_enable_debug_assert;

/**************************************************************/
/*********************** Code Body**** *************************/
/**************************************************************/



/**************************************************************
**	FUNCTION NAME		: InitEngineerMode
**
**  	PURPOSE				: This function is to initialize hilite handlers for Engineering mode
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void InitEngineerMode(void)
{
	SetHiliteHandler(EM_NETWORK_MENUID   , HighlightEmNetwork);

	SetHiliteHandler(EM_CELL_ENVIRONMENT_MENUID, HighlightEmCellEnvironment);

	SetHiliteHandler(EM_AUDIO_MENUID, HighlightEmAudio);
	SetHiliteHandler(EM_AUD_RING_TONE_MENUID, HighlightEmRingTone);
	SetHiliteHandler(EM_AUD_SET_MODE_MENUID, HighlightEmSetMode);
	SetHiliteHandler(EM_AUD_NORMAL_MODE_MENUID, HighlightEmNormalMode);
	SetHiliteHandler(EM_AUD_LOUDSP_MODE_MENUID, HighlightEmLoudSpMode);
	SetHiliteHandler(EM_AUD_HEADSET_MODE_MENUID, HighlightEmHeadsetMode);

	SetHiliteHandler(EM_AUD_FIR_MENUID, HighlightEmFIR);
	SetHiliteHandler(EM_AUD_SPEECH_MENUID, HighlightEmSpeech);
	SetHiliteHandler(EM_AUD_SIDE_TONE_MENUID, HighlightEmSideTone);
	SetHiliteHandler(EM_AUD_KEY_TONE_MENUID, HighlightEmKeyTone);
	SetHiliteHandler(EM_AUD_CALL_TONE_MENUID, HighlightEmCallTone);
	SetHiliteHandler(EM_AUD_MELODY_MENUID, HighlightEmMelody);
	SetHiliteHandler(EM_AUD_SOUND_MENUID, HighlightEmSound);
	SetHiliteHandler(EM_AUD_MICROPHONE_MENUID, HighlightEmMicrophone);

	SetHiliteHandler(EM_AUD_SET_ECHO_PARAM_MENUID, HighlightEmAudSetEchoParam);
	SetHiliteHandler(EM_AUD_SET_ENERGY_1_MENUID, HighlightEmAudSetEnergy1);
	SetHiliteHandler(EM_AUD_SET_DELAY_1_MENUID, HighlightEmAudSetDelay1);
	SetHiliteHandler(EM_AUD_SET_ENERGY_2_MENUID, HighlightEmAudSetEnergy2);
	SetHiliteHandler(EM_AUD_SET_DELAY_2_MENUID, HighlightEmAudSetDelay2);
	SetHiliteHandler(EM_AUD_SET_MAX_SWING_MENUID, HighlightEmAudSetMaxSwing);
	
#ifdef __MMI_EM_AUDIO_SPEECH_ENHANCEMENT__
	SetHiliteHandler(EM_AUD_SPEECH_ENHANCEMENT_MENUID, mmi_em_sph_enh_main_highlight_hdlr);
	SetHiliteHandler(EM_AUD_SPH_ENH_NORMAL_MODE_MENUID, mmi_em_sph_enh_normal_mode_highlight_hdlr);
	SetHiliteHandler(EM_AUD_SPH_ENH_HEADSET_MODE_MENUID, mmi_em_sph_enh_headset_mode_highlight_hdlr);
	SetHiliteHandler(EM_AUD_SPH_ENH_LOUDSPK_MODE_MENUID, mmi_em_sph_enh_loudspk_mode_highlight_hdlr);
	SetHiliteHandler(EM_AUD_SPH_ENH_BT_EARPHONE_MODE_MENUID, mmi_em_sph_enh_bt_earphone_mode_highlight_hdlr);
	SetHiliteHandler(EM_AUD_SPH_ENH_FM_MENUID, mmi_em_sph_enh_FM_mode_highlight_hdlr);
	SetHiliteHandler(EM_AUD_SPH_ENH_TV_MENUID, mmi_em_sph_enh_TV_mode_highlight_hdlr);
	SetHiliteHandler(EM_AUD_SPH_ENH_VOICE_FLOW, mmi_em_sph_enh_voice_flow_highlight_hdlr);
	SetHiliteHandler(EM_AUD_SPH_ENH_MUSIC_FLOW, mmi_em_sph_enh_mucis_flow_highlight_hdlr);
	SetHiliteHandler(EM_AUD_SPH_ENH_SIDE_TONE, mmi_em_sph_enh_side_tone_highlight_hdlr);
	SetHiliteHandler(EM_AUD_SPH_ENH_MICROPHONE, mmi_em_sph_enh_microphone_highlight_hdlr);
	SetHiliteHandler(EM_AUD_SPH_ENH_DIGIT_GAINS, mmi_em_sph_enh_digit_gains_highlight_hdlr);
	SetHiliteHandler(EM_AUD_SPH_ENH_ANALOG_GAINS, mmi_em_sph_enh_analog_gains_highlight_hdlr);
	SetHiliteHandler(EM_AUD_SPH_ENH_MICROPHONE_GAINS, mmi_em_sph_enh_microphone_gains_highlight_hdlr);
	SetHiliteHandler(EM_AUD_SPH_ENH_DIGIT_GAIN_IN_VOC, mmi_em_sph_enh_voc_gains_highlight_hdlr);
	SetHiliteHandler(EM_AUD_SPH_ENH_SET_DEFAULT, mmi_em_sph_enh_set_defalut_highlight_hdlr);
	SetHiliteHandler(EM_AUD_SPH_ENH_SAVE, mmi_em_sph_enh_save_highlight_hdlr);
#endif

   /* Device */
	SetHiliteHandler(EM_DEVICE_MENUID, HighlightEmDevice);
	SetHiliteHandler(EM_DEV_LCD_MENU_ID, HighlightEmLCD);
	SetHiliteHandler(EM_DEV_LCD_MAIN_MENU_ID, HighlightEmLCDMain);
	SetHiliteHandler(EM_DEV_LCD_SUB_MENU_ID, HighlightEmLCDSub);
       SetHiliteHandler(EM_DEV_LCD_SET_CONSTRAST_MENU_ID, HighlightEmLCDSetContrast);
       SetHiliteHandler(EM_DEV_LCD_SET_BIAS_MENU_ID, HighlightEmLCDSetBias);
       SetHiliteHandler(EM_DEV_LCD_SET_LINERATE_MENU_ID, HighlightEmLCDSetLineRate);
       SetHiliteHandler(EM_DEV_LCD_SET_TEMPERATURE_MENU_ID, HighlightEmLCDSetTemperature);
	SetHiliteHandler(EM_DEV_GPIO_MENU_ID, HighlightEmGPIO);
	SetHiliteHandler(EM_DEV_GPIO_LIST_MENU_ID, HighlightEmGPIOList);
	SetHiliteHandler(EM_DEV_GPIO_SET_MENU_ID, HighlightEmGPIOSet);
	SetHiliteHandler(EM_DEV_GPO_SET_MENU_ID, HighlightEmGPOSet);

       SetHiliteHandler(EM_DEV_PWM_MENU_ID, HighlightEmPWM);
	SetHiliteHandler(EM_DEV_EINT_MENU_ID, HighlightEmEINT);
	SetHiliteHandler(EM_DEV_ADC_MENU_ID, HighlightEmADC);
#if defined(NAND_SUPPORT) //&& defined(MMI_ON_HARDWARE_P) 
	SetHiliteHandler(EM_NAND_FORMAT_MENUID,	HighlightEmNANDFormat);
#endif
	SetHiliteHandler(EM_DEV_CLAM_MENUID, HighlightEmClam);
	SetHiliteHandler(EM_DEV_HW_LEVEL_MENUID, HighlightEmHWLevel);
	SetHiliteHandler(EM_DEV_SET_UART_MENUID  , HighlightEmSetUart);
	SetHiliteHandler(EM_DEV_SLEEPMODE_MENUID  , HighlightEmSetSleepMode);

   /* End of Device */

	SetHiliteHandler(EM_GPRS_ACT_MENUID     , HighlightEmGprs);
	SetHiliteHandler(EM_GPRS_AUTO_ATTACH_MENUID     , HighlightEmAutoAttach);
	SetHiliteHandler(EM_GPRS_ACTIVATE_PDP_MENUID     , HighlightEmActivatePDP);
	SetHiliteHandler(EM_GPRS_DEACTIVATE_PDP_MENUID     , HighlightEmDeactivatePDP);
	SetHiliteHandler(EM_GPRS_SEND_DATA_MENUID     , HighlightEmSendDataMenu);
	SetHiliteHandler(EM_GPRS_SEND_DATA_HDR_MENUID     , HighlightEmSendData);
	SetHiliteHandler(EM_GPRS_SEND_DATA_NORM_MENUID     , HighlightEmSendData);
	SetHiliteHandler(EM_GPRS_PDP_CONTEXT_1_MENUID     , HighlightEmPDPContext1);
	SetHiliteHandler(EM_GPRS_PDP_CONTEXT_2_MENUID     , HighlightEmPDPContext2);
	SetHiliteHandler(EM_GPRS_PDP_CONTEXT_3_MENUID     , HighlightEmPDPContext3);
	SetHiliteHandler(EM_GPRS_PDP_CONTEXT_4_MENUID     , HighlightEmPDPContext4);
	SetHiliteHandler(EM_GPRS_PDP_CONTEXT_5_MENUID     , HighlightEmPDPContext5);
	SetHiliteHandler(EM_GPRS_PDP_CONTEXT_6_MENUID     , HighlightEmPDPContext6);
	SetHiliteHandler(EM_GPRS_PDP_CONTEXT_7_MENUID     , HighlightEmPDPContext7);
	SetHiliteHandler(EM_GPRS_PDP_CONTEXT_8_MENUID     , HighlightEmPDPContext8);
	SetHiliteHandler(EM_GPRS_PDP_CONTEXT_9_MENUID     , HighlightEmPDPContext9);
	SetHiliteHandler(EM_GPRS_PDP_CONTEXT_10_MENUID     , HighlightEmPDPContext10);
	SetHiliteHandler(EM_GPRS_PDP_CONTEXT_11_MENUID     , HighlightEmPDPContext11);
	SetHiliteHandler(EM_GPRS_PDP_CONTEXT_12_MENUID     , HighlightEmPDPContext12);
	SetHiliteHandler(EM_GPRS_PDP_CONTEXT_13_MENUID     , HighlightEmPDPContext13);
	SetHiliteHandler(EM_GPRS_PDP_CONTEXT_14_MENUID     , HighlightEmPDPContext14);
	SetHiliteHandler(EM_GPRS_PDP_CONTEXT_15_MENUID     , HighlightEmPDPContext15);

	SetHiliteHandler(EM_BAND_SEL_MENUID			,HighlightNETSETBandSelection);

	SetHiliteHandler(EM_MISC_MENUID     , HighlightEmMisc);
	SetHiliteHandler(EM_AUTO_ANSWER_MENUID     , HighlightEmAutoAnswer);
   SetHiliteHandler(EM_HIGHSPEED_SIM_MENUID     , HighlightEmHighSpeedSIM);
	SetHiliteHandler(EM_BACKLIGHT_MENUID     , HighlightEmMiscBacklight);
	SetHiliteHandler(EM_SERIAL_PORT_EN_MENUID     , HighlightEmSerialPortEnable );
	SetHiliteHandler(EM_AUTO_RESET_MENUID           , HighlightEmAutoReset);

	SetHiliteHandler(EM_MMI_DEBUG_MENUID,     HighlightEmMMIDebug);
	SetHiliteHandler(EM_MEMORY_DUMP_MENUID, HighlightEmMemoryDump);
   SetHiliteHandler(EM_RAM_TEST_MENUID, HightlightEMRAMTestHdlr);

#ifdef __MMI_TVOUT__
   SetHiliteHandler(EM_TVOUT_MENUID, HighlightEmTVOut);	
#endif /* __MMI_TVOUT__ */

#if defined(__MMI_VIDEO__) && defined(ISP_SUPPORT)
   SetHiliteHandler(EM_VIDEO_HI_BITRATE_MENUID, HighlightVideoHiBitrate);	
#endif /* __MMI_VIDEO__ && ISP_SUPPORT */

#if defined(__AMR_SUPPORT__)
	SetHiliteHandler(EM_AMR_MENUID           , HighlightEmAMR);
#endif
	SetHiliteHandler(EM_WAP_MENUID, HighlightEMWAP);
	SetHiliteHandler(EM_WAP_ENABLE_DEBUG_MENUID, HighlightEmWAPEnableDebug);
	SetHiliteHandler(EM_WAP_OFFLINE_PAGE_MENUID, HighlightEmWAPOfflinePage);
	SetHiliteHandler(EM_WAP_USER_AGENT_MENUID, HighlightEMUserAgent);
	SetHiliteHandler(EM_WAP_ACCEPT_HEADER_MENUID, HighlightEMAcceptHeader);
	SetHiliteHandler(EM_WAP_MMS_VERSION_MENUID, HighlightEMMMSVersion);

#if (defined(MT6229) || defined(MT6227) || defined(MT6228)) 
SetHiliteHandler(EM_SWDBG_MENUID , HighlightEMSWDBG);
#endif

 //Lisen 0921
 SetHiliteHandler(EM_POWER_ON_DURATION_MENUID ,HighlightEMPowerOnDuration);

#ifdef __J2ME__
	SetHiliteHandler(EM_J2ME_TCK_MENUID     , HighlightEmMiscJ2meTck);
#endif

	SetHiliteHandler(EM_AUTO_TEST_SETTING_MENUID, HighlightAutoTestSetting);
       SetHiliteHandler(EM_PWR_DOWN_CONTROL_MENUID, HighlightEmPWRDownCtrl);
       SetHiliteHandler(EM_DEBUG_INFO_MENUID, HighlightEMDebugInfo);
       SetHiliteHandler(EM_FS_TRACE_MENUID, HighlightEMFsTrace);
       SetHiliteHandler(EM_LAST_EXCEPTION_MENUID, HighlightEMLastException);

#ifdef __TCPIP__ 
	 SetHiliteHandler(EM_SOCKET_MENUID ,HighlightEmSocket);
	 SetHiliteHandler(EM_SOCKET_DNS_MENUID ,HighlightEmDNSQuery);
	 SetHiliteHandler(EM_SOCKET_HTTP_MENUID ,HighlightEmHTTPGet);
	 SetHiliteHandler(EM_SOCKET_ECHO_MENUID ,HighlightEmEcho);
	 SetHiliteHandler(EM_SOCKET_DATE_MENUID ,HighlightEmDate);

	SetHiliteHandler(EM_SOCKET_DNS_DONE_MENUID ,HighlightEmDNSQueryDone);
	SetHiliteHandler(EM_SOCKET_DNS_INPUT_METHOD_MENUID ,HighlightEmDNSQueryInputMethod);
	SetHiliteHandler(EM_SOCKET_HTTP_DONE_MENUID ,HighlightEmHTTPGetDone);
	SetHiliteHandler(EM_SOCKET_HTTP_INPUT_METHOD_MENUID ,HighlightEmHTTPGetInputMethod);
	 
#endif

#if defined(__MMI_TOUCH_SCREEN__)
	SetHiliteHandler(EM_TOUCHSCREEN_MENUID,HighlightEmPen);
	SetHiliteHandler(EM_TOUCHSCREEN_TEST_STATISTICS_MENUID,HighlightEmPenStatistics);
	SetHiliteHandler(EM_TOUCHSCREEN_TEST_EXIT_MENUID,HighlightEmPenExit);
#endif

	SetHiliteHandler(EM_GPRS_TEST, HighlightGPRSTest);

 
//               Move SetProtocolEventHandler() register to the moment enter engineer mode
//               Should be review someday
//#if 0
#ifdef MMI_ON_HARDWARE_P
	SetProtocolEventHandler (EngineerModeStartRes, MSG_ID_EM_START_RSP);
	SetProtocolEventHandler (EngineerModeStopRes, MSG_ID_EM_STOP_RSP);
	SetProtocolEventHandler (EngineerModeStatusInd,MSG_ID_EM_STATUS_IND);
   /* device */
	SetProtocolEventHandler (EngineerModeSetUartRes, MSG_ID_EM_SET_UART_RSP);
       SetProtocolEventHandler (EngineerModeAttachUartRes, MSG_ID_MMI_ATTACH_UART_PORT_RSP);
       SetProtocolEventHandler (EngineerModeDetachUartRes, MSG_ID_MMI_DETACH_UART_PORT_RSP);
   
	SetProtocolEventHandler (SendSetHWLevelInfoRsp, MSG_ID_MMI_EQ_SET_HW_LEVEL_RSP);
	SetProtocolEventHandler (EngineerModeSetSleepModeRes, MSG_ID_MMI_EQ_SET_SLEEP_MODE_RSP);

	SetProtocolEventHandler (EngineerModeGetAudioProfileRes, MSG_ID_MMI_EQ_GET_AUDIO_PROFILE_RSP);
	SetProtocolEventHandler (EngineerModeSetAudioProfileRes, MSG_ID_MMI_EQ_SET_AUDIO_PROFILE_RSP);
	SetProtocolEventHandler (EngineerModeGetAudioParamRes, MSG_ID_MMI_EQ_GET_AUDIO_PARAM_RSP);
	SetProtocolEventHandler (EngineerModeSetAudioParamRes, MSG_ID_MMI_EQ_SET_AUDIO_PARAM_RSP);

	/* gprs */
#ifdef __MOD_TCM__
//micha0120
//	SetProtocolEventHandler (EngineerModeGprsAttachRes, MSG_ID_MMI_NW_SET_ATTACH_RSP);
	SetProtocolEventHandler (EngineerModeGprsDeactivatePDPRes, MSG_ID_MMI_PS_ACT_TEST_RSP);
	SetProtocolEventHandler (EngineerModeGprsSendDataRes, MSG_ID_MMI_PS_SEND_DATA_RSP);
	SetProtocolEventHandler (EngineerModeGprsSetDefinitionRes, MSG_ID_MMI_PS_SET_DEFINITION_RSP);
	SetProtocolEventHandler (EngineerModeGprsSetQosRes, MSG_ID_MMI_PS_SET_QOS_RSP);
	SetProtocolEventHandler (EngineerModeGprsActTestRes, MSG_ID_MMI_PS_ACT_TEST_RSP);
#endif /* __MOD_TCM__ */

#endif /* MMI_ON_HARDWARE_P */
//#endif  /* 0 */
 

#if (defined(__MMI_ENGINEER_MODE__) && defined(__MMI_GPRS_PING_APP__) && defined(__TCPIP__) && defined(__GPRS_MODE__) )
      InitEngineerModeSocketApp();
#endif


#if defined(__MMI_BT_SUPPORT__)   
	InitEngineerModeBT();
#endif

}

/**************************************************************
**	FUNCTION NAME		: InitEngineerModeSetting
**
**  	PURPOSE				: This function is to initialize Engineering mode audio and LED pattern
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void InitEngineerModeSetting(void)
{
     gCurrentMode = ENGINEERING_MODE;
     mdi_audio_suspend_background_play();
     StopLEDPatternBackGround();
}


/**************************************************************
**	FUNCTION NAME		: EntryEngineerModeMenu
**
**  	PURPOSE				: Entry function for main EM menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEngineerModeMenu(void)
{
	U16		ItemList[32];
	U16		ItemIcons[32];
	U16		nItems;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;

	EntryNewScreen(EM_MAIN_MENU_SCR, NULL, EntryEngineerModeMenu, NULL);
	
	guiBuffer = GetCurrGuiBuffer(EM_MAIN_MENU_SCR);
	
	nItems = GetNumOfChild(ENGINEER_MODE_MENUID);
	GetSequenceStringIds(ENGINEER_MODE_MENUID,ItemList);
	SetParentHandler(ENGINEER_MODE_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	
       for(i=0;i<nItems;i++)
	{
	       PopUpList[i] = NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	ShowCategory52Screen(	ENGINEER_MODE_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	InitEngineerModeSetting();

}


/**************************************************************
**	FUNCTION NAME		: EnableDisableEmMode
**
**  	PURPOSE				: Engineering mode switch
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EnableDisableEmMode(void)
{
   if(currentEmCommand!=EM_NONE)
      return;

   newEmMode = 0x00000001<<activeEmModeIndex;
   
   if(EmModeEnabled[activeEmModeIndex])
   {
      EmModeEnabled[activeEmModeIndex] = 0;
      currentEmMode&=~newEmMode;
      //SendEmStopMsg();
   }
   else
   {
      EmModeEnabled[activeEmModeIndex] = 1;
      currentEmMode|=newEmMode;
      //SendEmStartMsg();
   }

   standard_check_list_handle_item_select(activeEmModeIndex);
}


/**************************************************************
**	FUNCTION NAME		: SendEmStartMsg
**
**  	PURPOSE				: To start Engineering Mode 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SendEmStartMsg(void)
{
   currentEmCommand = EM_START;
#ifdef MMI_ON_HARDWARE_P
   EngineerModeStartReq(MOD_AS, currentEmMode/*newEmMode*/);
#endif

#ifdef _WIN32
   EngineerModeStatusUpdate(0, currentEmMode/*newEmMode*/);
   EngineerModeSuccessNotice(EM_UPDATE_SUCCESS_TEXT);
#endif
}


/**************************************************************
**	FUNCTION NAME		: SendEmStopMsg
**
**  	PURPOSE				: To stop Engineering Mode 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SendEmStopMsg(void)
{
   currentEmCommand = EM_STOP;
#ifdef MMI_ON_HARDWARE_P
   EngineerModeStopReq(MOD_AS, currentEmMode/*newEmMode*/);
#endif

#ifdef _WIN32
   EngineerModeStatusUpdate(0, currentEmMode/*newEmMode*/);
   EngineerModeSuccessNotice(EM_UPDATE_SUCCESS_TEXT);
#endif
}

/**************************************************************
**	FUNCTION NAME		: CheckEmMode
**
**  	PURPOSE				: To Check Engineering Mode 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void CheckEmMode(void)
{

   U8 i;
   U32 mask = 0x00000001;
   
   newEmMode = 0;

   for(i=0; i<MAX_EM_MODE;i++)
   {
      if(EmModeEnabled[i])
         newEmMode|=mask;
      mask=mask<<1;
   }

   ShowCategory2Screen (EM_UPDATE_TITLE_TEXT, 0,
   							STR_GLOBAL_YES, 0,
   							STR_GLOBAL_NO, 0, 
   							EM_UPDATE_QUERY_TEXT, NULL);
   SetLeftSoftkeyFunction (SendEmStartMsg, KEY_EVENT_UP);
   SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
   ReplaceNewScreenHandler(EM_NETWORK_MENU_SCR, NULL, NULL);
}


/**************************************************************
**	FUNCTION NAME		: SetEmModeIndex
**
**  	PURPOSE				: To set Engineering Mode index
**
**	INPUT PARAMETERS	: nIndex
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SetEmModeIndex(S32 nIndex)
{
	activeEmModeIndex = (U8) nIndex;
}


/**************************************************************
**	FUNCTION NAME		: SetEmModeIndex
**
**  	PURPOSE				: To synchronize Engineering Mode
**
**	INPUT PARAMETERS	: nIndex
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SyncEmMode(void)
{
   U8 i;
   U32 mask = 0x00000001;

   for(i=0; i<MAX_EM_MODE;i++)
   {
      if(currentEmMode&mask)
         EmModeEnabled[i] = 1;
      else
         EmModeEnabled[i] = 0;
      mask=mask<<1;
   }
}


/**************************************************************
**	FUNCTION NAME		: EntryEmNetworkMenu
**
**  	PURPOSE				: Entry function for Network menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmNetworkMenu(void)
{
	U16 nStrItemList[MAX_EM_MODE]; 

	U16 nNumofItem;
	U16 nDispAttribute;
	U8* guiBuffer;
	U16 i, j, count;
	
	EntryNewScreen(EM_NETWORK_MENU_SCR, NULL, EntryEmNetworkMenu, NULL);
	
	guiBuffer = GetCurrGuiBuffer(EM_NETWORK_MENU_SCR);		/* Getting the Index from history  */

	nNumofItem = GetNumOfChild(EM_NETWORK_MENUID);
	nDispAttribute = GetDispAttributeOfItem(EM_NETWORK_MENUID);
	GetSequenceStringIds(EM_NETWORK_MENUID, nStrItemList);

	SetParentHandler(EM_NETWORK_MENUID);

	//CSD add by JL
       count=nNumofItem;
       for(i=j=0;i<nNumofItem;i++)
       {
         if ( (nStrItemList[i] != EM_NET_CC_CHAN_INFO_TEXT) && 
      	        (nStrItemList[i] != EM_NET_CC_CALL_INFO_TEXT) &&
      	        (nStrItemList[i] != EM_NET_CB_INFO_TEXT) &&
      	        (nStrItemList[i] != EM_NET_SS_INFO_TEXT) &&
      	        (nStrItemList[i] != EM_NET_MM_INFO_TEXT) &&
      	        (nStrItemList[i] != EM_NET_UEM_INFO_TEXT) &&
      	        (nStrItemList[i] != EM_NET_RR_CTRL_MSG_TEXT))
         {
            nStrItemList[j] = nStrItemList[i];
            j++;
         }
         else
            count--;

         nNumofItem = count;
       }
       
	RegisterHighlightHandler(SetEmModeIndex);

	ShowCategory13Screen(EM_NETWORK_TEXT, EM_NETWORK_ICON,
						  STR_GLOBAL_OK, 0,
						  STR_GLOBAL_BACK, 0,
						  nNumofItem, nStrItemList, EmModeEnabled, 0, guiBuffer);

	SetLeftSoftkeyFunction(EnableDisableEmMode,KEY_EVENT_UP);
	SetRightSoftkeyFunction(CheckEmMode,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: HighlightEmNetwork
**
**  	PURPOSE				: Highlight function for Network menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmNetwork(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmNetworkMenu,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
void EmGetCellEnvironmentStart(void)
{
	MYQUEUE		Message;	 

	Message.oslSrcId	=	MOD_MMI;	
	Message.oslDestId=	MOD_L4C;	
	Message.oslMsgId	=	PRT_MSG_ID_MMI_GET_CELL_INFO_START;	
	Message.oslDataPtr	=	NULL;	
	Message.oslPeerBuffPtr	=	NULL;	

	OslMsgSendExtQueue(&Message);
}
void EmGetCellEnvironmentEnd(void)
{
	U8 i;
	MYQUEUE		Message;	 
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		ClearProtocolEventHandlerExt(PRT_MSG_ID_MMI_GET_CELL_INFO_RSP_IND, i);
		Message.oslSrcId	=	MOD_MMI;	
		Message.oslDestId=	MOD_L4C + i;	
		Message.oslMsgId	=	PRT_MSG_ID_MMI_GET_CELL_INFO_END;	
		Message.oslDataPtr	=	NULL;	
		Message.oslPeerBuffPtr	=	NULL;
		OslMsgSendExtQueue(&Message);
	}

}
BOOL bCellInfoUpdate[MMI_SIM_NUMBER] = {MMI_FALSE,};
S8 	BigUnicodeDisplayBuf[200 * ENCODING_LENGTH * MMI_SIM_NUMBER];
void EmUpdateCellInfo(void *inmsg)
{
	U8 *guiBuffer;
	CFW_TSM_CURR_CELL_INFO pCurrCellInf = {0,};
	CFW_TSM_ALL_NEBCELL_INFO pNeighborCellInfo ={0,};
	int i;
	U8 	AsciiBuf[MAX_TEST_STRING_BUF];
	S8 	UnicodeBuf[MAX_TEST_STRING_BUF*2];
	U8 simIndex;
	
	guiBuffer = GetCurrGuiBuffer(EM_CELL_ENVIRONMENT_SCR);
	memset(AsciiBuf, 0, sizeof(AsciiBuf));
	memset(UnicodeBuf, 0, sizeof(UnicodeBuf));
	memset(BigUnicodeDisplayBuf, 0, sizeof(BigUnicodeDisplayBuf));
	for(simIndex = 0; simIndex < MMI_SIM_NUMBER; simIndex++)
	{
		if(bCellInfoUpdate[simIndex])
		{
		#ifdef __ADP_MULTI_SIM__
			CFW_GetCellInfo(&pCurrCellInf,&pNeighborCellInfo, simIndex);
		#else
			CFW_GetCellInfo(&pCurrCellInf,&pNeighborCellInfo);
		#endif

		#ifdef __MMI_MULTI_SIM__
			sprintf((char*) AsciiBuf, "          SIM %d Info\n", simIndex + 1);
			AnsiiToUnicodeString((S8*)UnicodeBuf, (S8*)AsciiBuf);
			pfnUnicodeStrcat(BigUnicodeDisplayBuf, UnicodeBuf);
		#endif
			if(simIndex == 0)
			{
				sprintf((char*)AsciiBuf, "      channel    db\n");
				AnsiiToUnicodeString((S8*)UnicodeBuf, (S8*)AsciiBuf);
				pfnUnicodeStrcat(BigUnicodeDisplayBuf, UnicodeBuf);
			}

			sprintf((char*)AsciiBuf, "used  %4d       %d\n",pCurrCellInf.nTSM_Arfcn,pCurrCellInf.nTSM_AvRxLevel);
			AnsiiToUnicodeString((S8*)UnicodeBuf, (S8*)AsciiBuf);
			pfnUnicodeStrcat(BigUnicodeDisplayBuf, UnicodeBuf);

			for(i = 0; i <pNeighborCellInfo.nTSM_NebCellNUM; i++)
			{
				if( i == 0)
					sprintf((char*)AsciiBuf, "neig   %4d       %d\n",pNeighborCellInfo.nTSM_NebCell[i].nTSM_Arfcn, pNeighborCellInfo.nTSM_NebCell[i].nTSM_AvRxLevel);
				else
					sprintf((char*)AsciiBuf, "       %4d       %d\n",pNeighborCellInfo.nTSM_NebCell[i].nTSM_Arfcn, pNeighborCellInfo.nTSM_NebCell[i].nTSM_AvRxLevel);
				AnsiiToUnicodeString((S8*)UnicodeBuf, (S8*)AsciiBuf);
				pfnUnicodeStrcat(BigUnicodeDisplayBuf, UnicodeBuf);
			}
		}
	}
	MMI_ASSERT(pfnUnicodeStrlen(BigUnicodeDisplayBuf) < sizeof(BigUnicodeDisplayBuf)/2);
	ShowCategory74Screen(EM_CELL_ENVIRONMENT_TEXT, 0, 0, 0, STR_GLOBAL_BACK,IMG_GLOBAL_BACK, 
		                               (PU8) BigUnicodeDisplayBuf, pfnUnicodeStrlen(BigUnicodeDisplayBuf), guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	


}
extern U8 MTPNP_AD_get_protocol_event_src_mode(void);
void EmUpdateCellInfoStart(void *inmsg)
{
	U8 nSimID = MTPNP_AD_get_protocol_event_src_mode();
	bCellInfoUpdate[nSimID]= MMI_TRUE;
	EmUpdateCellInfo(inmsg);
}
/**************************************************************
**	FUNCTION NAME		: ExitEmCellEnvironment
**
**  	PURPOSE				: Exit function for cell environment menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitEmCellEnvironment(void)
{
	U8 i;
	EmGetCellEnvironmentEnd();
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		bCellInfoUpdate[i] = MMI_FALSE;
	}
}

U8 EmExitExitEmCellEnvironmentCallback(void* dummy)
{
	U8 i;
	EmGetCellEnvironmentEnd();
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		bCellInfoUpdate[i] = MMI_FALSE;
	}
	return 0;
}
/**************************************************************
**	FUNCTION NAME		: EntryEmCellEnvironment
**
**  	PURPOSE				: Entry function for cell environment menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmCellEnvironment(void)
{
	U8 i;
	U8 *guiBuffer;


	EntryNewScreen(EM_CELL_ENVIRONMENT_SCR, ExitEmCellEnvironment, EntryEmCellEnvironment, NULL);
	
	guiBuffer = GetCurrGuiBuffer(EM_CELL_ENVIRONMENT_SCR);
	
	sprintf((char*) EMFMAsciiDisplayBuf, "Getting...");
	
	AnsiiToUnicodeString((S8*)EMFMUnicodeDisplayBuf, (S8*)EMFMAsciiDisplayBuf);
	
	ShowCategory74Screen(EM_CELL_ENVIRONMENT_TEXT, 0, 0, 0, STR_GLOBAL_BACK,IMG_GLOBAL_BACK, 
		                               (PU8) EMFMUnicodeDisplayBuf, sizeof(EMFMUnicodeDisplayBuf), guiBuffer);

	EmGetCellEnvironmentStart();

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetProtocolEventHandlerExt(EmUpdateCellInfoStart,PRT_MSG_ID_MMI_GET_CELL_INFO_RSP_IND,i);
	}

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	

	SetDelScrnIDCallbackHandler(EM_CELL_ENVIRONMENT_SCR, EmExitExitEmCellEnvironmentCallback);
}

/**************************************************************
**	FUNCTION NAME		: HighlightEmCellEnvironment
**
**  	PURPOSE				: Highlight function for cell environment menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmCellEnvironment(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmCellEnvironment,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
/**************************************************************
**	FUNCTION NAME		: SendSetAudioProfileReq
**
**  	PURPOSE				: Interface to send audio profile request
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SendSetAudioProfileReq(void)
{
   EngineerModeSetAudioProfileReq(&gAudioProfile);
}


/**************************************************************
**	FUNCTION NAME		: SendSetVolumeReq
**
**  	PURPOSE				: Interface to send volumw setting request
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SendSetVolumeReq(void)
{
   EngineerModeSetVolumeReq((U8)currentVolumeType, (U8)currentVolume);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmAudioMenu
**
**  	PURPOSE				: Entry function for Audio menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmAudioMenu(void)
{
	U16		ItemList[32];
	U16		ItemIcons[32];
	U16		nItems;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;


	EntryNewScreen(EM_AUDIO_MENU_SCR, ExitEmAudioMenu, NULL, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_AUDIO_MENU_SCR);
	nItems = GetNumOfChild(EM_AUDIO_MENUID);
	GetSequenceStringIds(EM_AUDIO_MENUID,ItemList);
	SetParentHandler(EM_AUDIO_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

       for(i=0;i<nItems;i++)
	{
	       PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	ShowCategory52Screen(EM_AUDIO_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: ExitEmAudioMenu
**
**  	PURPOSE				: Exit function for Audio menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitEmAudioMenu (void)
{
	history_t currHistory;
	currHistory.scrnID = EM_AUDIO_MENU_SCR;
	//currHistory.entryFuncPtr = EntryEmAudioMenu;
	currHistory.entryFuncPtr = SendGetAudioParamReq;
	pfnUnicodeStrcpy((S8*)currHistory.inputBuffer, (S8*)L"");
	GetCategoryHistory (currHistory.guiBuffer);
	AddHistory (currHistory);
}


/**************************************************************
**	FUNCTION NAME		: EngineerModeRingTonePlay
**
**  	PURPOSE				: To play ring tone
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EngineerModeRingTonePlay(void)
{
   StopTimer(EM_RINGTONE_HIGHLIGHT_TIMER);
   if(activeRingToneIndex>=(gNoOfImy+gNoOfMidi))
      EngineerModePlayAudioReq( (U8)(activeRingToneIndex-(gNoOfImy+gNoOfMidi)), AUD_SOUND);
   else if(activeRingToneIndex>=gNoOfImy)
      EngineerModePlayAudioReq( (U8)(activeRingToneIndex-gNoOfImy), AUD_MIDI);
   else
      EngineerModePlayAudioReq(activeRingToneIndex, AUD_IMELODY);
}


/**************************************************************
**	FUNCTION NAME		: HighlightRingToneIndex
**
**  	PURPOSE				: Ring tone highlight handler
**
**	INPUT PARAMETERS	: nIndex
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightRingToneIndex(S32 nIndex)
{
   if (!isPopup)
   {
      if(activeRingToneIndex>=(gNoOfImy+gNoOfMidi))
         EngineerModeStopAudioReq( (U8)(activeRingToneIndex-(gNoOfImy+gNoOfMidi)), AUD_SOUND);
      else if(activeRingToneIndex>=gNoOfImy)
         EngineerModeStopAudioReq( (U8)(activeRingToneIndex-gNoOfImy), AUD_MIDI);
      else
         EngineerModeStopAudioReq((U8)activeRingToneIndex, AUD_IMELODY);
         
   	activeRingToneIndex = (U8) nIndex;
   	StartTimer(EM_RINGTONE_HIGHLIGHT_TIMER, EM_HIGHTLIGHT_DURATION, EngineerModeRingTonePlay);
   }
}


/**************************************************************
**	FUNCTION NAME		: EmPrepareRingToneList
**
**  	PURPOSE				: Helper function for Ringtone menu display 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmPrepareRingToneList(void)
{
   U16 i;
   S8 buffer[20];

#ifdef MMI_ON_HARDWARE_P
   custom_get_fixed_imelody_range(&gFixedImyBegin, &gFixedImyEnd);
   custom_get_fixed_midi_range(&gFixedMidiBegin, &gFixedMidiEnd);
   custom_get_fixed_sound_range(&gFixedSoundBegin, &gFixedSoundEnd);
#else
   gFixedImyBegin = 20;
   gFixedImyEnd = 31;
   gFixedMidiBegin = 60;
   gFixedMidiEnd = 90;
#endif

   gNoOfImy = (gFixedImyEnd-gFixedImyBegin+1);
   gNoOfMidi = (gFixedMidiEnd-gFixedMidiBegin+1);
   gNoOfSound = (gFixedSoundEnd-gFixedSoundBegin+1);

   for(i=0;i<gNoOfImy && i<MAX_SUB_MENUS;i++)
   {
      sprintf(buffer,"IMY %d", i);
      AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
      subMenuDataPtrs[i] = subMenuData[i];
   }

   
   for(i=gNoOfImy; i<gNoOfImy+gNoOfMidi && i<MAX_SUB_MENUS;i++)
   {
      sprintf(buffer, "MIDI %d", i-gNoOfImy);
      AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
      subMenuDataPtrs[i] = subMenuData[i];
   }

   for(i=gNoOfImy+gNoOfMidi; i<gNoOfImy+gNoOfMidi+gNoOfSound && i<MAX_SUB_MENUS;i++)
   {
      sprintf(buffer, "SOUND %d", i-(gNoOfImy+gNoOfMidi));
      AnsiiToUnicodeString((PS8)subMenuData[i], buffer);
      subMenuDataPtrs[i] = subMenuData[i];
   }

   gNoOfRing = gNoOfImy+gNoOfMidi+gNoOfSound;
   
}


/**************************************************************
**	FUNCTION NAME		: EmPopupFinishHandler
**
**  	PURPOSE				: Popup Finish handler
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmPopupFinishHandler(void)
{
   StopTimer(EM_NOTIFYDURATION_TIMER);
   GoBackHistory();
   isPopup = FALSE;
}


/**************************************************************
**	FUNCTION NAME		: EmAudioKeyUp
**
**  	PURPOSE				: EM key up handler
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmAudioKeyUp(void)
{
   S8 temp[30];
   U8 MessageString[30];


   currentKeyShift+=1;
   if(currentKeyShift > MAX_KEY_SHIFT)
      currentKeyShift = MAX_KEY_SHIFT;
   //currentKeyShift = MIN(currentKeyShift, MAX_KEY_SHIFT);
#ifdef MMI_ON_HARDWARE_P
   if (!custom_em_melody_key_shift(currentKeyShift))
      return;
#endif

   isPopup = TRUE;

   EntryNewScreen(EM_AUD_RING_TONE_MENU_SCR, NULL, NULL, NULL);

   memset(MessageString,0,30);

   if (currentKeyShift>0)
	   sprintf(temp,"%d halftone Up!", currentKeyShift);
   else if(currentKeyShift<0)
	   sprintf(temp,"%d halftone Down!", -currentKeyShift);
   else
	   sprintf(temp,"Key not changed!");

   AnsiiToUnicodeString((PS8)MessageString,temp);
	
	
   ShowCategory63Screen(MessageString,EM_NETWORK_ICON,NULL);

   SetLeftSoftkeyFunction(EmPopupFinishHandler,KEY_EVENT_UP);
	
   StartTimer(EM_NOTIFYDURATION_TIMER, EM_NOTIFY_DURATION, EmPopupFinishHandler);
}


/**************************************************************
**	FUNCTION NAME		: EmAudioKeyDown
**
**  	PURPOSE				: EM key down handler
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmAudioKeyDown(void)
{
   S8 temp[30];
   U8 MessageString[30];

   currentKeyShift-=1;
   if(currentKeyShift < (-MAX_KEY_SHIFT))
      currentKeyShift = -MAX_KEY_SHIFT;
    
#ifdef MMI_ON_HARDWARE_P
   if (!custom_em_melody_key_shift(currentKeyShift))
      return;
#endif

   isPopup = TRUE;
   

   EntryNewScreen(EM_AUD_RING_TONE_MENU_SCR, NULL, NULL, NULL);

   memset(MessageString,0,30);
   
   if (currentKeyShift>0)
	   sprintf(temp,"%d halftone Up!", currentKeyShift);
   else if(currentKeyShift<0)
	   sprintf(temp,"%d halftone Down!", -currentKeyShift);
   else
	   sprintf(temp,"Key not changed!");

   AnsiiToUnicodeString((PS8)MessageString,temp);

   ShowCategory63Screen(MessageString,EM_NETWORK_ICON,NULL);
   SetLeftSoftkeyFunction(EmPopupFinishHandler,KEY_EVENT_UP);	
   StartTimer(EM_NOTIFYDURATION_TIMER, EM_NOTIFY_DURATION, EmPopupFinishHandler);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmRingToneMenu
**
**  	PURPOSE				: Entry function for Ring Tone menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmRingToneMenu(void)
{
	U8* guiBuffer;

	EntryNewScreen(EM_AUD_RING_TONE_MENU_SCR, ExitEmRingToneMenu, NULL, NULL);
	guiBuffer = GetCurrGuiBuffer (EM_AUD_RING_TONE_MENU_SCR);		 

       ClearAllKeyHandler();
	RegisterHighlightHandler (HighlightRingToneIndex);
       EmPrepareRingToneList();
      
	if ( gNoOfRing < 1 )
       {
	   ShowCategory6Screen (EM_AUD_RING_TONE_TEXT, 0,
						  STR_GLOBAL_OK, 0,
						  STR_GLOBAL_BACK, 0,
						  gNoOfRing+1, (PU8*)subMenuDataPtrs, NULL,0,
						  guiBuffer); 
	   SetLeftSoftkeyFunction (NULL, KEY_EVENT_UP);
	   SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
       }
	else 
	{
	   ShowCategory6Screen (EM_AUD_RING_TONE_TEXT, 0,
						  STR_GLOBAL_OK, 0,
						  STR_GLOBAL_BACK, 0,
						  gNoOfRing, (PU8*)subMenuDataPtrs,NULL, 0,
						  guiBuffer); 
	   SetLeftSoftkeyFunction (NULL, KEY_EVENT_UP);
	   SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
   	   SetKeyHandler(EmAudioKeyUp, KEY_VOL_UP, KEY_EVENT_DOWN);
   	   SetKeyHandler(EmAudioKeyDown, KEY_VOL_DOWN, KEY_EVENT_DOWN);
	}
}


/**************************************************************
**	FUNCTION NAME		: ExitEmRingToneMenu
**
**  	PURPOSE				: Exit function for Ring Tone menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitEmRingToneMenu (void)
{
   history_t currHistory;

   if (!isPopup)
   {
   	/* stop timer */
      StopTimer(EM_RINGTONE_HIGHLIGHT_TIMER);
      /* stop ring tone */
      if(activeRingToneIndex>=(gNoOfImy+gNoOfMidi))
         EngineerModeStopAudioReq( (U8)(activeRingToneIndex-(gNoOfImy+gNoOfMidi)), AUD_SOUND);
      else if(activeRingToneIndex>=gNoOfImy)
         EngineerModeStopAudioReq( (U8)(activeRingToneIndex-gNoOfImy), AUD_MIDI);
      else
         EngineerModeStopAudioReq(activeRingToneIndex, AUD_IMELODY);
   }
   
	currHistory.scrnID = EM_AUD_RING_TONE_MENU_SCR;
   currHistory.entryFuncPtr = EntryEmRingToneMenu;
   pfnUnicodeStrcpy((S8*)currHistory.inputBuffer, (S8*)L"");
   GetCategoryHistory (currHistory.guiBuffer);
   AddHistory (currHistory);
}


/**************************************************************
**	FUNCTION NAME		: IsAudioProfileChanged
**
**  	PURPOSE				: Checker function to tell if audio profile is modified
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: TRUE/FALSE
**
 
**************************************************************/
BOOL IsAudioProfileChanged(void)
{
   U8 i;

   for(i=0; i<MAX_EM_VOLUME_LEVEL; i++)
   {
      if (gAudioProfile.melody[i] != gAudioProfileOrg.melody[i] ||
         gAudioProfile.sound[i] != gAudioProfileOrg.sound[i] ||
         gAudioProfile.keytone[i] != gAudioProfileOrg.keytone[i] ||
         gAudioProfile.speech[i] != gAudioProfileOrg.speech[i] ||
         gAudioProfile.mic[i] != gAudioProfileOrg.mic[i])
        {   
           return TRUE;
        }
   }
   
   if (gAudioProfile.sidetone != gAudioProfileOrg.sidetone)
      return TRUE;

   return FALSE;   
}


/**************************************************************
**	FUNCTION NAME		: CheckAudioProfile
**
**  	PURPOSE				: To check audio profile
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void CheckAudioProfile(void)
{
   
   if (IsAudioProfileChanged())
   {
      ShowCategory2Screen (EM_UPDATE_TITLE_TEXT, 0,
      						STR_GLOBAL_YES, 0,
      						STR_GLOBAL_NO, 0, 
      						EM_UPDATE_QUERY_TEXT, NULL);
      SetLeftSoftkeyFunction (SendSetAudioProfileReq, KEY_EVENT_UP);
      SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
      ReplaceNewScreenHandler(EM_UPDATE_TITLE_TEXT, NULL, NULL);
   }
   else
      GoBackHistory();
}


/**************************************************************
**	FUNCTION NAME		: EntryEmSetMode
**
**  	PURPOSE				: Entry function for Audio Set menu 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmSetMode(void)
{
	U8*	guiBuffer;
	U16 ItemList[]=
	{	
	   EM_AUD_NORMAL_MODE_TEXT,
	   EM_AUD_LOUDSP_MODE_TEXT,
	   EM_AUD_HEADSET_MODE_TEXT
	};

	EntryNewScreen(EM_AUD_SET_MODE_MENU_SCR, NULL, EntryEmSetMode, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_AUD_SET_MODE_MENU_SCR);
	RegisterHighlightHandler(SetHighlightIndex);

	ShowCategory11Screen(EM_AUD_SET_MODE_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							3, ItemList, 0, guiBuffer);
   						  
	SetLeftSoftkeyFunction(EmSetMode,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: EmSetMode
**
**  	PURPOSE				: Function for Audio Setting
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmSetMode(void)
{
	U16 ItemList[]=
	{	
	   EM_AUD_NORMAL_MODE_TEXT,
	   EM_AUD_LOUDSP_MODE_TEXT,
	   EM_AUD_HEADSET_MODE_TEXT
	};
	U8 modeList[] = 
	{
          AUD_MODE_NORMAL,
          AUD_MODE_LOUDSPK,
          AUD_MODE_HEADSET
	};
	
	EngineerModeSetAudioMode( modeList[currentHighlightIndex] );
	DisplayPopup( (PU8)GetString( ItemList[currentHighlightIndex] ), IMG_GLOBAL_OK, 1, 1000, (U8)SUCCESS_TONE );
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmSetMode
**
**  	PURPOSE				: Highlight function for Audio Set menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmSetMode(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmSetMode,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmNormalModeMenu
**
**  	PURPOSE				: Entry function for Audio Normal menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmNormalModeMenu(void)
{
	U16		ItemList[32];
	U16		ItemIcons[32];
	U16		nItems;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;


	EntryNewScreen(EM_AUD_NORMAL_MODE_MENU_SCR, NULL, EntryEmNormalModeMenu, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_AUD_NORMAL_MODE_MENU_SCR);
	
	nItems = GetNumOfChild(EM_AUD_NORMAL_MODE_MENUID);
	GetSequenceStringIds(EM_AUD_NORMAL_MODE_MENUID,ItemList);
	SetParentHandler(EM_AUD_NORMAL_MODE_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	ShowCategory52Screen(EM_AUD_NORMAL_MODE_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(CheckAudioProfile,KEY_EVENT_UP);

       /* reset current highlight index */
	SetHighlightIndex(0);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmLoudSpModeMenu
**
**  	PURPOSE				: Entry function for Audio Loud Speaker menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmLoudSpModeMenu(void)
{
	U16		ItemList[32];
	U16		ItemIcons[32];
	U16		nItems;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;


	EntryNewScreen(EM_AUD_LOUDSP_MODE_MENU_SCR, NULL, EntryEmLoudSpModeMenu, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_AUD_LOUDSP_MODE_MENU_SCR);
	
	nItems = GetNumOfChild(EM_AUD_LOUDSP_MODE_MENUID);
	GetSequenceStringIds(EM_AUD_LOUDSP_MODE_MENUID,ItemList);
	SetParentHandler(EM_AUD_LOUDSP_MODE_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	ShowCategory52Screen(EM_AUD_LOUDSP_MODE_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(CheckAudioProfile,KEY_EVENT_UP);

   /* reset current highlight index */
	SetHighlightIndex(0);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmHeadsetModeMenu
**
**  	PURPOSE				: Entry function for Audio Headset menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmHeadsetModeMenu(void)
{
	U16		ItemList[32];
	U16		ItemIcons[32];
	U16		nItems;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;


	EntryNewScreen(EM_AUD_HEADSET_MODE_MENU_SCR, NULL, EntryEmHeadsetModeMenu, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_AUD_HEADSET_MODE_MENU_SCR);
	
	nItems = GetNumOfChild(EM_AUD_HEADSET_MODE_MENUID);
	GetSequenceStringIds(EM_AUD_HEADSET_MODE_MENUID,ItemList);
	SetParentHandler(EM_AUD_HEADSET_MODE_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	ShowCategory52Screen(EM_AUD_HEADSET_MODE_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(CheckAudioProfile,KEY_EVENT_UP);
	
   /* reset current highlight index */
	SetHighlightIndex(0);
}


/**************************************************************
**	FUNCTION NAME		: EmAudioVolumeUp
**
**  	PURPOSE				: Function for Audio Volume setting
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmAudioVolumeUp(void)
{
       if (currentVolume<MAX_VOLUME-VOLUME_STEP)
         currentVolume+=VOLUME_STEP;
       else
         currentVolume=MAX_VOLUME;
      
	ShowCategory204Screen(EM_SET_LSK_TEXT,0,STR_GLOBAL_BACK,0, 
	                        EM_AUD_SET_VOLUME_TEXT, currentVolume, 
							      0, 0);
	SetLeftSoftkeyFunction (EmSetVolume, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(EmAudioVolumeUp,KEY_UP_ARROW,KEY_EVENT_UP);
	SetKeyHandler(EmAudioVolumeDown,KEY_DOWN_ARROW,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: EmAudioVolumeDown
**
**  	PURPOSE				: Function for Audio Volume setting
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmAudioVolumeDown(void)
{
       if (currentVolume>MIN_VOLUME+VOLUME_STEP)
          currentVolume-=VOLUME_STEP;
       else
          currentVolume=MIN_VOLUME;
       
	ShowCategory204Screen(EM_SET_LSK_TEXT,0,STR_GLOBAL_BACK,0, 
	                        EM_AUD_SET_VOLUME_TEXT, currentVolume, 
							      0, 0);
	SetLeftSoftkeyFunction (EmSetVolume, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(EmAudioVolumeUp,KEY_UP_ARROW,KEY_EVENT_UP);
	SetKeyHandler(EmAudioVolumeDown,KEY_DOWN_ARROW,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: EmGetCurrentVolume
**
**  	PURPOSE				: Function for Audio to get current volume
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
U8 EmGetCurrentVolume(void)
{
   switch(currentVolumeType)
   {
      case EM_VOL_TYPE_SPH:
         if(currentHighlightIndex<MAX_EM_VOLUME_LEVEL)
            return gAudioProfile.speech[currentHighlightIndex];
         else
            return gAudioProfile.sp_gain;
         break;
      case EM_VOL_TYPE_KEY:
         if(currentHighlightIndex<MAX_EM_VOLUME_LEVEL)
            return gAudioProfile.keytone[currentHighlightIndex];
         else
            return gAudioProfile.kt_gain;
         break;
      case EM_VOL_TYPE_MIC:
         if(currentHighlightIndex<MAX_EM_VOLUME_LEVEL)
            return gAudioProfile.mic[currentHighlightIndex];
         else
            return gAudioProfile.mic_gain;
         break;
      case EM_VOL_TYPE_SID:
         return gAudioProfile.sidetone;
         break;
      case EM_VOL_TYPE_MEDIA:
         if(currentHighlightIndex<MAX_EM_VOLUME_LEVEL)
            return gAudioProfile.melody[currentHighlightIndex];
         else
            return gAudioProfile.mel_gain;
         break;
      case EM_VOL_TYPE_SND:
         if(currentHighlightIndex<MAX_EM_VOLUME_LEVEL)
            return gAudioProfile.sound[currentHighlightIndex];
         else
            return gAudioProfile.snd_gain;
         break;
	  default:
		  return 0;
   }
}


/**************************************************************
**	FUNCTION NAME		: EmSetVolume
**
**  	PURPOSE				: Function for Audio to set volume
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmSetVolume(void)
{
   switch(currentVolumeType)
   {
      case EM_VOL_TYPE_SPH:
         if(currentHighlightIndex<MAX_EM_VOLUME_LEVEL)
            gAudioProfile.speech[currentHighlightIndex] = (U8)currentVolume;
         else
         {
            SendSetVolumeReq();
            gAudioProfile.sp_gain = (U8)currentVolume;
         }
         break;
      case EM_VOL_TYPE_KEY:
         if(currentHighlightIndex<MAX_EM_VOLUME_LEVEL)
            gAudioProfile.keytone[currentHighlightIndex] = (U8)currentVolume;
         else
         {
            SendSetVolumeReq();
            gAudioProfile.kt_gain = (U8)currentVolume;
         }   
         break;
      case EM_VOL_TYPE_MIC:
         if(currentHighlightIndex<MAX_EM_VOLUME_LEVEL)
            gAudioProfile.mic[currentHighlightIndex] = (U8)currentVolume;
         else
         {
            SendSetVolumeReq();
            gAudioProfile.mic_gain = (U8)currentVolume;
         }
         break;
      case EM_VOL_TYPE_SID:
         gAudioProfile.sidetone = (U8)currentVolume;
         break;
      case EM_VOL_TYPE_MEDIA:
         if(currentHighlightIndex<MAX_EM_VOLUME_LEVEL)
            gAudioProfile.melody[currentHighlightIndex] = (U8)currentVolume;
         else
         {
            SendSetVolumeReq();
            gAudioProfile.mel_gain = (U8)currentVolume;
         }   
         break;
      case EM_VOL_TYPE_SND:
         if(currentHighlightIndex<MAX_EM_VOLUME_LEVEL)
            gAudioProfile.sound[currentHighlightIndex] = (U8)currentVolume;
         else
         {
            SendSetVolumeReq();
            gAudioProfile.snd_gain = (U8)currentVolume;
         }   
         break;
	 
	default:
		break;

   }

   GoBackHistory();
   
}


/**************************************************************
**	FUNCTION NAME		: EmSetVolume
**
**  	PURPOSE				: Entry function for Audio to set volume
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmSetVolume(void)
{

	EntryNewScreen(EM_AUD_SET_VOLUME_MENU_SCR, NULL, NULL, NULL);

       currentVolume = EmGetCurrentVolume();
	ShowCategory204Screen(EM_SET_LSK_TEXT,0,STR_GLOBAL_BACK,0, 
	                        EM_AUD_SET_VOLUME_TEXT, currentVolume, 
							      0, 0);
	SetLeftSoftkeyFunction (EmSetVolume, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(EmAudioVolumeUp,KEY_UP_ARROW,KEY_EVENT_UP);
	SetKeyHandler(EmAudioVolumeDown,KEY_DOWN_ARROW,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EmSetVolume
**
**  	PURPOSE				: Entry function for Audio FIR setting
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmFIR(void)
{
	U8*	guiBuffer;
	U16 ItemList[]=
	{	
	   EM_AUD_FIR_0_TEXT,
	   EM_AUD_FIR_1_TEXT,
	   EM_AUD_FIR_2_TEXT,
	   EM_AUD_FIR_3_TEXT,
	   EM_AUD_FIR_4_TEXT,
	   EM_AUD_FIR_5_TEXT
	};

	EntryNewScreen(EM_AUD_FIR_MENU_SCR, NULL, EntryEmFIR, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_AUD_FIR_MENU_SCR);
	
	RegisterHighlightHandler(SetHighlightIndex);

	ShowCategory11Screen(EM_AUD_FIR_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							6, ItemList, AudioParameter.selected_FIR_output_index, guiBuffer);
   						  
	SetLeftSoftkeyFunction(EmSetFIR,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: EmSetFIR
**
**  	PURPOSE				: Function for Audio FIR setting
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmSetFIR(void)
{
/*
	U16 ItemList[]=
	{	
	   EM_AUD_FIR_0_TEXT,
	   EM_AUD_FIR_1_TEXT,
	   EM_AUD_FIR_2_TEXT,
	   EM_AUD_FIR_3_TEXT,
	   EM_AUD_FIR_4_TEXT,
	   EM_AUD_FIR_5_TEXT
	};
*/
	AudioParameter.selected_FIR_output_index = currentHighlightIndex;
	EngineerModeSetAudioParamReq(&AudioParameter);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmFIR
**
**  	PURPOSE				: highlight function for Audio FIR menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmFIR(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmFIR,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmSpeech
**
**  	PURPOSE				: Entry function for Speech setting menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmSpeech(void)
{
	U16		ItemList[]=
	{	
	   EM_AUD_VOLUME_0_TEXT,
	   EM_AUD_VOLUME_1_TEXT,
	   EM_AUD_VOLUME_2_TEXT,
	   EM_AUD_VOLUME_3_TEXT,
	   EM_AUD_VOLUME_4_TEXT,
	   EM_AUD_VOLUME_5_TEXT,
	   EM_AUD_VOLUME_6_TEXT,
	   EM_AUD_DIRECT_APPLY_TEXT
	};
	U16		ItemIcons[32];
	U16		nItems=7;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;


	EntryNewScreen(EM_AUD_SPEECH_MENU_SCR, NULL, EntryEmSpeech, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_AUD_SPEECH_MENU_SCR);

       ClearAllKeyHandler();

	SetParentHandler(EM_AUD_SPEECH_MENUID);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	currentVolumeType = EM_VOL_TYPE_SPH;

	RegisterHighlightHandler(SetHighlightIndex);

	ShowCategory52Screen(EM_AUD_SPEECH_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

   						  
	SetLeftSoftkeyFunction(EntryEmSetVolume,KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: EntryEmKeyTone
**
**  	PURPOSE				: Entry function for Keytone menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmKeyTone(void)
{
	U16		ItemList[]=
	{	
	   EM_AUD_VOLUME_0_TEXT,
	   EM_AUD_VOLUME_1_TEXT,
	   EM_AUD_VOLUME_2_TEXT,
	   EM_AUD_VOLUME_3_TEXT,
	   EM_AUD_VOLUME_4_TEXT,
	   EM_AUD_VOLUME_5_TEXT,
	   EM_AUD_VOLUME_6_TEXT,
	   EM_AUD_DIRECT_APPLY_TEXT
	};
	U16		ItemIcons[32];
	U16		nItems=7;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;


	EntryNewScreen(EM_AUD_KEY_TONE_MENU_SCR, NULL, EntryEmKeyTone, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_AUD_KEY_TONE_MENU_SCR);

       ClearAllKeyHandler();

	SetParentHandler(EM_AUD_KEY_TONE_MENUID);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	currentVolumeType = EM_VOL_TYPE_KEY;

	RegisterHighlightHandler(SetHighlightIndex);

	ShowCategory52Screen(EM_AUD_KEY_TONE_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

   						  
	SetLeftSoftkeyFunction(EntryEmSetVolume,KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: EntryEmMelody
**
**  	PURPOSE				: Entry function for Melody menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmMelody(void)
{
	U16		ItemList[]=
	{	
	   EM_AUD_VOLUME_0_TEXT,
	   EM_AUD_VOLUME_1_TEXT,
	   EM_AUD_VOLUME_2_TEXT,
	   EM_AUD_VOLUME_3_TEXT,
	   EM_AUD_VOLUME_4_TEXT,
	   EM_AUD_VOLUME_5_TEXT,
	   EM_AUD_VOLUME_6_TEXT,
	   EM_AUD_DIRECT_APPLY_TEXT
	};
	U16		ItemIcons[32];
	U16		nItems=7;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;


	EntryNewScreen(EM_AUD_MELODY_MENU_SCR, NULL, EntryEmMelody, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_AUD_MELODY_MENU_SCR);

       ClearAllKeyHandler();


	SetParentHandler(EM_AUD_MELODY_MENUID);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	currentVolumeType = EM_VOL_TYPE_MEDIA;

	RegisterHighlightHandler(SetHighlightIndex);

	ShowCategory52Screen(EM_AUD_MELODY_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);
						  

	SetLeftSoftkeyFunction(EntryEmSetVolume,KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: EntryEmSideTone
**
**  	PURPOSE				: Entry function for Side tone menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmSideTone(void)
{

	EntryNewScreen(EM_AUD_SIDE_TONE_MENU_SCR, NULL, NULL, NULL);


       ClearAllKeyHandler();

       currentVolumeType = EM_VOL_TYPE_SID;
       currentVolume = EmGetCurrentVolume();
	ShowCategory204Screen(EM_SET_LSK_TEXT,0,STR_GLOBAL_BACK,0, 
	                        EM_AUD_SET_VOLUME_TEXT, currentVolume, 
							      0, 0);
	SetLeftSoftkeyFunction (EmSetVolume, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(EmAudioVolumeUp,KEY_UP_ARROW,KEY_EVENT_UP);
	SetKeyHandler(EmAudioVolumeDown,KEY_DOWN_ARROW,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmCallTone
**
**  	PURPOSE				: Entry function for call tone menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmCallTone(void)
{

	EntryNewScreen(EM_AUD_CALL_TONE_MENU_SCR, NULL, NULL, NULL);

       ClearAllKeyHandler();

       currentVolumeType = EM_VOL_TYPE_CTN;
       currentVolume = INIT_VOLUME;
	ShowCategory204Screen(EM_SET_LSK_TEXT,0,STR_GLOBAL_BACK,0, 
	                        EM_AUD_SET_VOLUME_TEXT, currentVolume, 
							      0, 0);
	SetLeftSoftkeyFunction (EmSetVolume, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(EmAudioVolumeUp,KEY_UP_ARROW,KEY_EVENT_UP);
	SetKeyHandler(EmAudioVolumeDown,KEY_DOWN_ARROW,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmSound
**
**  	PURPOSE				: Entry function for sound menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmSound(void)
{
	U16		ItemList[]=
	{	
	   EM_AUD_VOLUME_0_TEXT,
	   EM_AUD_VOLUME_1_TEXT,
	   EM_AUD_VOLUME_2_TEXT,
	   EM_AUD_VOLUME_3_TEXT,
	   EM_AUD_VOLUME_4_TEXT,
	   EM_AUD_VOLUME_5_TEXT,
	   EM_AUD_VOLUME_6_TEXT,
	   EM_AUD_DIRECT_APPLY_TEXT
	};
	U16		ItemIcons[32];
	U16		nItems=7;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;


	EntryNewScreen(EM_AUD_SOUND_MENU_SCR, NULL, EntryEmSound, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_AUD_SOUND_MENU_SCR);

       ClearAllKeyHandler();


	SetParentHandler(EM_AUD_SOUND_MENUID);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	currentVolumeType = EM_VOL_TYPE_SND;

	RegisterHighlightHandler(SetHighlightIndex);

	ShowCategory52Screen(EM_AUD_SOUND_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);
						  

	SetLeftSoftkeyFunction(EntryEmSetVolume,KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: EntryEmMicrophone
**
**  	PURPOSE				: Entry function for Microphone menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmMicrophone(void)
{
	U16 ItemList[]=
	{	
		EM_AUD_VOLUME_0_TEXT,
		EM_AUD_VOLUME_1_TEXT,
		EM_AUD_VOLUME_2_TEXT,
		EM_AUD_VOLUME_3_TEXT,
		EM_AUD_VOLUME_4_TEXT,
		EM_AUD_VOLUME_5_TEXT,
		EM_AUD_VOLUME_6_TEXT,
		EM_AUD_DIRECT_APPLY_TEXT
	};
	U16 ItemIcons[32];
	U16 nItems=7;
	U8* guiBuffer;
	U8* PopUpList[32];
	S32 i;

	EntryNewScreen( EM_AUD_MICROPHONE_MENU_SCR, NULL, EntryEmMicrophone, NULL );
	guiBuffer = GetCurrGuiBuffer( EM_AUD_MICROPHONE_MENU_SCR );

	ClearAllKeyHandler();
	SetParentHandler(EM_AUD_MICROPHONE_MENUID);
	for( i = 0; i < nItems; i++ )
	{
		PopUpList[i] = NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);
	}

	currentVolumeType = EM_VOL_TYPE_MIC;
	RegisterHighlightHandler(SetHighlightIndex);
	ShowCategory52Screen(EM_AUD_MICROPHONE_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetLeftSoftkeyFunction( EntryEmSetVolume, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
}


/**************************************************************
**	FUNCTION NAME		: EntryEmAudSetEchoParam
**
**  	PURPOSE				: Entry function for Audio set echo menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmAudSetEchoParam(void)
{
	U16		ItemList[32];
	U16		ItemIcons[32];
	U16		nItems;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;


	EntryNewScreen(EM_AUD_SET_ECHO_PARAM_MENU_SCR, NULL, EntryEmAudSetEchoParam, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_AUD_SET_ECHO_PARAM_MENU_SCR);
	
	nItems = GetNumOfChild(EM_AUD_SET_ECHO_PARAM_MENUID);
	GetSequenceStringIds(EM_AUD_SET_ECHO_PARAM_MENUID,ItemList);
	SetParentHandler(EM_AUD_SET_ECHO_PARAM_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	ShowCategory52Screen(EM_AUD_SET_ECHO_PARAM_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmAudSetParamEdit
**
**  	PURPOSE				: Entry function for Audio set parameters menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmAudSetParamEdit(void)
{
	byte *history_buffer;
	U16  text_id=EM_AUD_SET_ENERGY_1_TEXT;
	 
	S32 level = 0;
	S8 ScrBuf[16];

       EntryNewScreen(EM_AUD_PARAM_EDITOR_SCR, NULL, EntryEmAudSetParamEdit, NULL);
	history_buffer=GetCurrGuiBuffer(EM_AUD_PARAM_EDITOR_SCR);

     //ShowCategory16Screen(STR_GLOBAL_OK,NULL,STR_GLOBAL_BACK,NULL,(U8*)InputBuf, 8,history_buffer);

     memset( InputBuf, 0, sizeof(InputBuf) );
     switch(currentHighlightIndex)
      {
       case 0: text_id = EM_AUD_SET_ENERGY_1_TEXT; level = AudioParameter.es_time_const ; break;
       case 1: text_id = EM_AUD_SET_DELAY_1_TEXT; level = AudioParameter.es_vol_const; break;
       case 2: text_id = EM_AUD_SET_ENERGY_2_TEXT; level = AudioParameter.es_time_const2; break;
       case 3: text_id = EM_AUD_SET_DELAY_2_TEXT; level = AudioParameter.es_vol_const2; break;
       case 4: text_id = EM_AUD_SET_MAX_SWING_TEXT; level = AudioParameter.max_swing; break;
       default:  text_id = EM_AUD_SET_ENERGY_1_TEXT; break;
      }
     
	sprintf( ScrBuf, "%d", level );
     AnsiiToUnicodeString((PS8)InputBuf, ScrBuf);
     ShowCategory5Screen( text_id,  0, \
                                          STR_GLOBAL_OK, 0 , \
                                          STR_GLOBAL_BACK, 0, \
                                          INPUT_TYPE_NUMERIC, \
                                          (U8*)InputBuf , 8 , history_buffer );
   
	SetLeftSoftkeyFunction(EmAudSetParam,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_END,KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmAudSetEnergy1
**
**  	PURPOSE				: Entry function for Audio set energy menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmAudSetEnergy1(void)
{
	 CurAudParam = AudioParameter.es_time_const;
	 Energy1 = CurAudParam;
	 currentHighlightIndex = 0;
        EntryEmAudSetParamEdit();
}


/**************************************************************
**	FUNCTION NAME		: EntryEmAudSetDelay1
**
**  	PURPOSE				: Entry function for Audio set delay menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmAudSetDelay1(void)
{
	 CurAudParam = AudioParameter.es_vol_const;
	 Delay1 = CurAudParam;
	 currentHighlightIndex = 1;
	 EntryEmAudSetParamEdit();
}


/**************************************************************
**	FUNCTION NAME		: EntryEmAudSetEnergy2
**
**  	PURPOSE				: Entry function for Audio set energy menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmAudSetEnergy2(void)
{
	 CurAudParam = AudioParameter.es_time_const2;
	 Energy2 = CurAudParam;
	 currentHighlightIndex = 2;
 	 EntryEmAudSetParamEdit();
}


/**************************************************************
**	FUNCTION NAME		: EntryEmAudSetDelay2
**
**  	PURPOSE				: Entry function for Audio set delay menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmAudSetDelay2(void)
{
	 CurAudParam = AudioParameter.es_vol_const2;
	 Delay2 = CurAudParam;
	 currentHighlightIndex = 3;
        EntryEmAudSetParamEdit();
}


/**************************************************************
**	FUNCTION NAME		: EntryEmAudSetMaxSwing
**
 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmAudSetMaxSwing(void)
{
	 CurAudParam = AudioParameter.max_swing;
	 MaxSwing = CurAudParam;
	 currentHighlightIndex = 4;
        EntryEmAudSetParamEdit();
}


/**************************************************************
**	FUNCTION NAME		: EmAudSetParam
**
**  	PURPOSE				: Function for Audio parameters setting
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmAudSetParam(void)
{
     S16 Arg=0;
     U8  U8StrBuf[8];

     //ExecuteCurrExitHandler();
     UnicodeToAnsii( (S8*)U8StrBuf , (S8*)InputBuf);
     Arg = (S16) atoi( (S8*)U8StrBuf );
     CurAudParam = Arg;

	if(currentHighlightIndex == 0) /* Energy 1 */
	{
	  Energy1 = CurAudParam;
  	  //memcpy( &AudioParam[30*4*sizeof(U16)+currentHighlightIndex*sizeof(U16)], &CurAudParam, sizeof(U16) ); //Energy
  	  AudioParameter.es_time_const = CurAudParam;
	}
       else if (currentHighlightIndex == 1)/* Delay 1 */
	{
	  Delay1 = CurAudParam;
	  //memcpy( &AudioParam[30*4*sizeof(U16)+currentHighlightIndex*sizeof(U16)], &CurAudParam, sizeof(U16) ); //Delay
  	  AudioParameter.es_vol_const = CurAudParam;
	}
	else if(currentHighlightIndex == 2) /* Energy 2 */
	{
	  Energy1 = CurAudParam;
  	  //memcpy( &AudioParam[30*4*sizeof(U16)+currentHighlightIndex*sizeof(U16)], &CurAudParam, sizeof(U16) ); //Energy
  	  AudioParameter.es_time_const2 = CurAudParam;
	}
       else if (currentHighlightIndex == 3)/* Delay 2 */
	{
	  Delay1 = CurAudParam;
	  //memcpy( &AudioParam[30*4*sizeof(U16)+currentHighlightIndex*sizeof(U16)], &CurAudParam, sizeof(U16) ); //Delay
  	  AudioParameter.es_vol_const2 = CurAudParam;
	}
	else if(currentHighlightIndex == 4) /* swing */
	{
	  Energy1 = CurAudParam;
  	  //memcpy( &AudioParam[30*4*sizeof(U16)+currentHighlightIndex*sizeof(U16)], &CurAudParam, sizeof(U16) ); //Energy
  	  AudioParameter.max_swing= CurAudParam;
	}

    #if MMI_ON_HARDWARE_P
       EngineerModeSetAudioParamReq(&AudioParameter);
    #else
       EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);
    #endif
	
	
}


/**************************************************************
**	FUNCTION NAME		: EmAudParamUp
**
**  	PURPOSE				: Function for Audio parameters tuning up
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmAudParamUp(void)
{
	 
	U16 Boundary = 0;
       U16 StrId = 0;
	
	if( currentHighlightIndex == 0 ) /* Energy */
	{
          Boundary = MAX_ENERGY_VAL;
	   StrId = EM_AUD_SET_ENERGY_1_TEXT;
	}
       else if ( currentHighlightIndex == 1 )
       {
	   Boundary = MAX_ENERGY_VAL;
	   StrId = EM_AUD_SET_DELAY_1_TEXT;
       }
       else if ( currentHighlightIndex == 2 )
       {
	   Boundary = MAX_ENERGY_VAL;
	   StrId = EM_AUD_SET_ENERGY_2_TEXT;
	}
       else if ( currentHighlightIndex == 3 )
       {
	   Boundary = MAX_ENERGY_VAL;
	   StrId = EM_AUD_SET_DELAY_2_TEXT;
       }
       else if ( currentHighlightIndex == 4 )
       {
	   Boundary = MAX_ENERGY_VAL;
	   StrId = EM_AUD_SET_MAX_SWING_TEXT;
	}

       if (CurAudParam<Boundary- AUD_PARAM_STEP)
         CurAudParam+=AUD_PARAM_STEP;
       else
         CurAudParam=Boundary;
	   
      
	ShowCategory204Screen(EM_SET_LSK_TEXT,0,STR_GLOBAL_BACK,0, 
	                        StrId, CurAudParam, 
							      0, 0);
	SetLeftSoftkeyFunction (EmAudSetParam, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(EmAudParamUp,KEY_UP_ARROW,KEY_EVENT_UP);
	SetKeyHandler(EmAudParamDown,KEY_DOWN_ARROW,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: EmAudParamDown
**
**  	PURPOSE				: Function for Audio parameters tuning down
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmAudParamDown(void)
{
	 
   U16 Boundary = 0;
   U16 StrId = 0;

   if( currentHighlightIndex == 0 ) /* Energy */
   {
	   Boundary = MIN_ENERGY_VAL;
	   StrId = EM_AUD_SET_ENERGY_1_TEXT;
   }
   else if ( currentHighlightIndex == 1 )
   {
	   Boundary = MIN_DELAY_VAL;
	   StrId = EM_AUD_SET_DELAY_1_TEXT;
   }
   else if ( currentHighlightIndex == 2 )
   {
	   Boundary = MIN_DELAY_VAL;
	   StrId = EM_AUD_SET_ENERGY_2_TEXT;
   }
   else if ( currentHighlightIndex == 3 )
   {
	   Boundary = MIN_DELAY_VAL;
	   StrId = EM_AUD_SET_DELAY_2_TEXT;
   }
   else if ( currentHighlightIndex == 4 )
   {
	   Boundary = MIN_DELAY_VAL;
	   StrId = EM_AUD_SET_MAX_SWING_TEXT;
   }

   if (CurAudParam>Boundary+AUD_PARAM_STEP)
      CurAudParam-=AUD_PARAM_STEP;
   else
      CurAudParam=Boundary;

	ShowCategory204Screen(EM_SET_LSK_TEXT,0,STR_GLOBAL_BACK,0, 
	                        StrId, CurAudParam, 
							      0, 0);
	SetLeftSoftkeyFunction (EmAudSetParam, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	SetKeyHandler(EmAudParamUp,KEY_UP_ARROW,KEY_EVENT_UP);
	SetKeyHandler(EmAudParamDown,KEY_DOWN_ARROW,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: SendGetAudioParamReq
**
**  	PURPOSE				: Function for getting Audio parameters 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SendGetAudioParamReq()
{
       EngineerModeGetAudioParamReq();
}


/**************************************************************
**	FUNCTION NAME		: EmUpdateAudioParam
**
**  	PURPOSE				: Function for updating Audio parameters 
**
**	INPUT PARAMETERS	: audp
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmUpdateAudioParam(U8 *audp)
{
#ifdef MMI_ON_HARDWARE_P
	audio_param_struct *p = (audio_param_struct *)audp;
#else
	EmAudioParamStruct *p = (EmAudioParamStruct *)audp;
#endif
	
	AudioParameter.selected_FIR_output_index = p->selected_FIR_output_index;
	AudioParameter.es_time_const = p->es_time_const;
	AudioParameter.es_time_const2 = p->es_time_const2;
	AudioParameter.es_vol_const = p->es_vol_const;
	AudioParameter.es_vol_const2 = p->es_vol_const2;
	AudioParameter.max_swing = p->max_swing;
	
	EntryEmAudioMenu();
       
}


/**************************************************************
**	FUNCTION NAME		: SendGetAudioProfileReq
**
**  	PURPOSE				: Function for sending messge for gettingu Audio profiles 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SendGetAudioProfileReq(void)
{
#ifdef MMI_ON_HARDWARE_P
	EngineerModeGetAudioProfileReq(currentVolumeMode);
#else
       EmUpdateAudioProfile(gAudioProfile.speech, gAudioProfile.keytone, 
                        gAudioProfile.melody, gAudioProfile.sound, 
                        gAudioProfile.mic, gAudioProfile.sidetone);
#endif
}


/**************************************************************
**	FUNCTION NAME		: EmUpdateAudioProfile
**
**  	PURPOSE				: Function updatingu Audio profiles 
**
**	INPUT PARAMETERS	: speech, keytone, melody, sound, mic, sidetone
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmUpdateAudioProfile(U8	*speech,
                     		U8	*keytone,
                     		U8	*melody,
                     		U8	*sound,
                     		U8	*mic,
                     		U8	sidetone)
{
   gAudioProfile.mode = currentVolumeMode;

   memcpy(gAudioProfile.speech, speech, MAX_EM_VOLUME_LEVEL);
   memcpy(gAudioProfile.keytone, keytone, MAX_EM_VOLUME_LEVEL);
   memcpy(gAudioProfile.melody, melody, MAX_EM_VOLUME_LEVEL);
   memcpy(gAudioProfile.sound, sound, MAX_EM_VOLUME_LEVEL);
   memcpy(gAudioProfile.mic, mic, MAX_EM_VOLUME_LEVEL);
   gAudioProfile.sidetone = sidetone;

   memcpy(&gAudioProfileOrg, &gAudioProfile, sizeof(gAudioProfile));

   switch(currentVolumeMode)
   {
      case EM_NORMAL_MODE:
         EntryEmNormalModeMenu();
         break;
      case EM_HEADSET_MODE:
         EntryEmHeadsetModeMenu();
         break;
      case EM_LOUDSP_MODE:
         EntryEmLoudSpModeMenu();
         break;

   }
   
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmAudio
**
**  	PURPOSE				: Highlight function for Audio menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmAudio(void)
{
	SetLeftSoftkeyFunction(SendGetAudioParamReq,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: HighlightEmRingTone
**
**  	PURPOSE				: Highlight function for Ring tone menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmRingTone(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmRingToneMenu,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: HighlightEmNormalMode
**
**  	PURPOSE				: Highlight function for Audio normal mode menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmNormalMode(void)
{
       currentVolumeMode = EM_NORMAL_MODE;

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(SendGetAudioProfileReq,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: HighlightEmLoudSpMode
**
**  	PURPOSE				: Highlight function for Audio loud speaker mode menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmLoudSpMode(void)
{
       currentVolumeMode = EM_LOUDSP_MODE;

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(SendGetAudioProfileReq,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: HighlightEmHeadsetMode
**
**  	PURPOSE				: Highlight function for Audio headset mode menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmHeadsetMode(void)
{
       currentVolumeMode = EM_HEADSET_MODE;

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(SendGetAudioProfileReq,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: HighlightEmSetVolume
**
**  	PURPOSE				: Highlight function for Audio set volume menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmSetVolume(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmSetVolume,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: HighlightEmSpeech
**
**  	PURPOSE				: Highlight function for Speech menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmSpeech(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmSpeech,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: HighlightEmSideTone
**
**  	PURPOSE				: Highlight function for SideTone menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmSideTone(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmSideTone,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmKeyTone
**
**  	PURPOSE				: Highlight function for KeyTone menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmKeyTone(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmKeyTone,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmCallTone
**
**  	PURPOSE				: Highlight function for CallTone menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmCallTone(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmCallTone,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmMelody
**
**  	PURPOSE				: Highlight function for Melody menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmMelody(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmMelody,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmSound
**
**  	PURPOSE				: Highlight function for Sound menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmSound(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmSound,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmMicrophone
**
**  	PURPOSE				: Highlight function for Microphone menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmMicrophone(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmMicrophone,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmAudSetEchoParam
**
**  	PURPOSE				: Highlight function for Audio set echo parameters menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmAudSetEchoParam(void)
{
	/* Register function for left soft key */
	//SetLeftSoftkeyFunction(SendGetAudioParamReq,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(EntryEmAudSetEchoParam,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmAudSetEnergy1
**
**  	PURPOSE				: Highlight function for Audio set energy menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmAudSetEnergy1(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmAudSetEnergy1,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmAudSetDelay1
**
**  	PURPOSE				: Highlight function for Audio set delay menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmAudSetDelay1(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmAudSetDelay1,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmAudSetEnergy2
**
**  	PURPOSE				: Highlight function for Audio set energy menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmAudSetEnergy2(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmAudSetEnergy2,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmAudSetDelay2
**
**  	PURPOSE				: Highlight function for Audio set delay menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmAudSetDelay2(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmAudSetDelay2,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmAudSetMaxSwing
**
 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmAudSetMaxSwing(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmAudSetMaxSwing,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/* ========  Device ======== */

/**************************************************************
**	FUNCTION NAME		: EntryEmDeviceMenu
**
**  	PURPOSE				: Enrty function for Device menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmDeviceMenu(void)
{
	U16		ItemList[32];
	U16		ItemIcons[32];
	U16		nItems;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;


	EntryNewScreen(EM_DEVICE_MENU_SCR, NULL, EntryEmDeviceMenu, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_DEVICE_MENU_SCR);
	
	nItems = GetNumOfChild(EM_DEVICE_MENUID);
	GetSequenceStringIds(EM_DEVICE_MENUID,ItemList);
	SetParentHandler(EM_DEVICE_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	ShowCategory52Screen(EM_DEVICE_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
		
}


/**************************************************************
**	FUNCTION NAME		: EmIsPortAllow
**
**  PURPOSE				: Check port available or not 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
BOOL EmIsPortAllow(U8 port_num )
{
//qiff disable the code for the function we cound not support
#if defined( __MMI_IRDA_SUPPORT__ ) && defined(MMI_ON_HARDWARE_P)
	{
#ifndef L4_NOT_PRESENT
		if( PS_UART_PORT == uart_port_irda )
		{
			return MMI_FALSE;
		}
#endif
	}
#endif

	return MMI_TRUE;
}

/**************************************************************
**	FUNCTION NAME		: SendSetUartReq
**
**  	PURPOSE				: Function to make set UART request
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SendSetUartReq(void)
{
     U8 port_idx=0, port_idx_other=1;

     EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
     gIsFromEmSetUartPort = 1;
     
#if defined(MMI_ON_HARDWARE_P)
	 if( EmIsPortAllow(EmUartPortHighlight) == MMI_FALSE )
	 {
	 	 mmi_eq_set_uart_rsp_struct st_rsp;
	 	 st_rsp.result = MMI_FALSE;
	     EngineerModeSetUartRes(&st_rsp);
	     return;
	 }
#endif
     
     if(EmPortSetting == EM_APP_TST )      /*Config TST Port*/
      {
           port_idx=EM_APP_TST;
           port_idx_other=EM_APP_PS;
           EmAppBaudArray[EM_APP_TST] = (U8) EmUartBaudHighlight;
      }
     else
      {
           port_idx=EM_APP_PS;
           port_idx_other=EM_APP_TST;
           EmAppBaudArray[EM_APP_PS] = (U8) EmUartBaudHighlight;
      }
     
         switch( EmUartPortHighlight )
         {
            case EM_UART_PORT_1:
               if( EmAppPortArray[port_idx_other] == uart_port1 )
               	EmAppPortArray[port_idx_other] = EmAppPortArray[port_idx];

               EmAppPortArray[port_idx] = uart_port1;
               break;
            case EM_UART_PORT_2:
               if( EmAppPortArray[port_idx_other] == uart_port2 )
               	EmAppPortArray[port_idx_other] = EmAppPortArray[port_idx];

               EmAppPortArray[port_idx] = uart_port2;
               break;
#ifdef __UART3_SUPPORT__
            case EM_UATR_PORT_3:
               if( EmAppPortArray[port_idx_other] == uart_port3 )
               	EmAppPortArray[port_idx_other] = EmAppPortArray[port_idx];

               EmAppPortArray[port_idx] = uart_port3;
               break;
#endif

/*
#ifdef  __IRDA_SUPPORT__
            case EM_UART_PORT_IRDA:
               if( EmAppPortArray[port_idx_other] == uart_port_irda)
               	EmAppPortArray[port_idx_other] = EmAppPortArray[port_idx];

               EmAppPortArray[port_idx] = uart_port_irda;
               break;
#endif
*/

/**
 *   USB UART port setting is not necessary here because data/tst will be redirected to usb port automatically after plug in
 *   The redirected object is determetered from "set default usb port app" menu item
 **/

/*
#ifdef __MMI_USB_SUPPORT__
            case EM_UART_PORT_USB:
               if( EmAppPortArray[port_idx_other] == uart_port_usb )
               	EmAppPortArray[port_idx_other] = EmAppPortArray[port_idx];

               EmAppPortArray[port_idx] =uart_port_usb;
               break;
#endif
*/

#ifdef __BLUETOOTH_SUPPORT__               
            case EM_UART_PORT_BLUETOOTH:
               if( EmAppPortArray[port_idx_other] == uart_port_bluetooth )
               	EmAppPortArray[port_idx_other] = EmAppPortArray[port_idx];

               EmAppPortArray[port_idx] =uart_port_bluetooth;
               break;
#endif
            
            default:
               EmAppPortArray[EM_APP_TST] = 1;
               EmAppPortArray[EM_APP_PS]= 0;
               break;
         }
         
            //Lisen 07132005
           UART_setting = TRUE;
	    EngineerModeSetUartReq();
	    
}


/**************************************************************
**	FUNCTION NAME		: EmSetUartMenuLskhdlr
**
**  	PURPOSE				: LSK handler for set UART request
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmSetUartMenuLskhdlr(void)
{
     switch( currentHighlightIndex )
      {
         case 0:
            EmPortSetting = EM_APP_TST;
            EngineerModeGetUartReq( (U8) EmPortSetting );
            break;
         case 1:
            EmPortSetting = EM_APP_PS;
      	 EngineerModeGetUartReq( (U8) EmPortSetting );
            break;
         case 2:
            EntrySetUsbDefAppScr();
         default:
            EmPortSetting = EM_APP_TST;
            break;
      }            

}


/**************************************************************
**	FUNCTION NAME		: HighlightEmSetDefaultUsbApp
**
**  	PURPOSE				: Highlight function for USB app 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmSetDefaultUsbApp(S32 nIndex)
{
   currentHighlightIndex = (U8) nIndex;
}


/**************************************************************
**	FUNCTION NAME		: EmSetDefaultUsbAppLskHdlr
**
**  	PURPOSE				: LSK handler for Default USB app setting
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmSetDefaultUsbAppLskHdlr(void)
{

   S16 pError;
   U8 app;
   
   currentHighlightIndex = (U8)GetCurrHiliteID();
   
   if( currentHighlightIndex == 0)
      app=USB_SWITCH_PORT_APP_DATA;
   else
      app=USB_SWITCH_PORT_APP_DEBUG;
   
   WriteValue( NVRAM_DEFAULT_USB_APP, &app, DS_BYTE, &pError );

   if( pError == NVRAM_WRITE_SUCCESS )
      DisplayPopup( (U8*)GetString(STR_GLOBAL_DONE),  (U16) IMG_GLOBAL_OK, 0, EM_NOTIFY_DURATION, (U8)SUCCESS_TONE );
   else
      DisplayPopup( (U8*)GetString(STR_GLOBAL_UNFINISHED),  (U16) IMG_GLOBAL_UNFINISHED, 0, EM_NOTIFY_DURATION, (U8)ERROR_TONE );


 }


/**************************************************************
**	FUNCTION NAME		: EntrySetUsbDefAppScr
**
**  	PURPOSE				: Entry function for Default USB app menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntrySetUsbDefAppScr(void)
{

#ifdef __MMI_USB_SUPPORT__

	U16		ItemList[32];
	U16		ItemIcons[32];
	U16		nItems;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;
   MMI_USB_STATUS usb_state = GetUsbStatus();

   if( usb_state == MMI_USBSTATUS_CDCACM \
      || usb_state == MMI_USBSTATUS_CDCACM_PROC \
      //|| usb_state == MMI_USBSTATUS_CDCACM_SWCHPORT
      ) 
   {
         /* USB has been configured, or configuring, could not set app again */
         DisplayPopup( (U8*)GetString(STR_GLOBAL_UNFINISHED),  IMG_GLOBAL_UNFINISHED, 0, EM_NOTIFY_DURATION, (U8)ERROR_TONE );
         return;
   }

	EntryNewScreen(EM_DEV_DEFAULT_USB_APP_MENU_SCR, NULL, EntrySetUsbDefAppScr, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_DEV_DEFAULT_USB_APP_MENU_SCR);
	
	nItems = GetNumOfChild(EM_DEV_DEFAULT_USB_APP_MENUID);
	GetSequenceStringIds(EM_DEV_DEFAULT_USB_APP_MENUID,ItemList);
	SetParentHandler(EM_DEV_DEFAULT_USB_APP_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	//SetHiliteHandler(EM_DEV_DEFAULT_USB_DATA_MENUID  , HighlightEmSetDefaultUsbApp);
	//SetHiliteHandler(EM_DEV_DEFAULT_USB_DEBUG_MENUID  , HighlightEmSetDefaultUsbApp);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = IMG_GLOBAL_L1+i;//ReturnRequiredNumberImageId(i);
	}

	ShowCategory52Screen(	ENGINEER_MODE_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetLeftSoftkeyFunction(EmSetDefaultUsbAppLskHdlr,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	
#endif /* __MMI_USB_SUPPORT__ */	
	
}


/**************************************************************
**	FUNCTION NAME		: EntryEmSetUartMenu
**
**  	PURPOSE				: Entry function for Set UART menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmSetUartMenu(void)
{
	U16 nStrItemList[] = {
	   EM_DEV_SET_UART_TST_TEXT,
	   EM_DEV_SET_UART_PS_TEXT
   #ifdef __MMI_USB_SUPPORT__
          ,
          EM_DEV_DEFAULT_USB_APP_TEXT
   #endif
        };

	U8* PopUpList[32];
	U16 ItemIcons[32];
	U8  i, nNumofItem = (sizeof(nStrItemList)/sizeof(nStrItemList[0]));
	U8* guiBuffer;

    //currentHighlightIndex=index;

//Lisen 09062005
// #if defined(MMI_ON_HARDWARE_P)
#if 0
#ifndef L4_NOT_PRESENT
/* under construction !*/
/* under construction !*/
/* under construction !*/
#else
/* under construction !*/
/* under construction !*/
#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

	EntryNewScreen(EM_DEV_SET_UART_MENU_SCR, NULL, EntryEmSetUartMenu, NULL);
	guiBuffer = GetCurrGuiBuffer (EM_DEV_SET_UART_MENU_SCR);
	
	RegisterHighlightHandler (SetHighlightIndex);


	for(i=0;i<nNumofItem;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = IMG_GLOBAL_L1+i;
	}
     
	ShowCategory52Screen(EM_DEV_SET_UART_TEXT,EM_DEV_SET_UART_ICON,
						  STR_GLOBAL_OK, 0,
						  STR_GLOBAL_BACK, 0,
							nNumofItem, nStrItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);


	//SetLeftSoftkeyFunction(SendSetUartReq,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(EmSetUartMenuLskhdlr,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: EmSetUartFillInlineStruct
**
**  	PURPOSE				: Function to fill Set UART inline structure
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmSetUartFillInlineStruct()
{
	EmUartPortText[EM_UART_PORT_1] = (PU8)GetString(EM_DEV_UART_1_TEXT);
	EmUartPortText[EM_UART_PORT_2] = (PU8)GetString(EM_DEV_UART_2_TEXT);

#ifdef __UART3_SUPPORT__
	EmUartPortText[EM_UATR_PORT_3] = (PU8)GetString(EM_DEV_UART_3_TEXT);
#endif

/*
#ifdef  __IRDA_SUPPORT__
	EmUartPortText[EM_UART_PORT_IRDA] = (PU8)GetString(EM_DEV_UART_IRDA_TEXT);
#endif
*/

/**
 *   USB UART port setting is not necessary here because data/tst will be redirected to usb port automatically after plug in
 *   The redirected object is determetered from "set default usb port app" menu item
 **/
/*
#ifdef __MMI_USB_SUPPORT__
	EmUartPortText[EM_UART_PORT_USB] = (PU8)GetString(EM_DEV_UART_USB_TEXT);
#endif
*/
	EmUartBaudText[0] = (PU8)GetString(EM_DEV_BAUD_AUTO_TEXT);
	EmUartBaudText[1] = (PU8)GetString(EM_DEV_BAUD_75_TEXT);
	EmUartBaudText[2] = (PU8)GetString(EM_DEV_BAUD_150_TEXT);
	EmUartBaudText[3] = (PU8)GetString(EM_DEV_BAUD_300_TEXT);
	EmUartBaudText[4] = (PU8)GetString(EM_DEV_BAUD_600_TEXT);
	EmUartBaudText[5] = (PU8)GetString(EM_DEV_BAUD_1200_TEXT);
	EmUartBaudText[6] = (PU8)GetString(EM_DEV_BAUD_2400_TEXT);
	EmUartBaudText[7] = (PU8)GetString(EM_DEV_BAUD_4800_TEXT);
	EmUartBaudText[8] = (PU8)GetString(EM_DEV_BAUD_7200_TEXT);
	EmUartBaudText[9] = (PU8)GetString(EM_DEV_BAUD_9600_TEXT);
	EmUartBaudText[10] = (PU8)GetString(EM_DEV_BAUD_14400_TEXT);
	EmUartBaudText[11] = (PU8)GetString(EM_DEV_BAUD_19200_TEXT);
	EmUartBaudText[12] = (PU8)GetString(EM_DEV_BAUD_28800_TEXT);
	EmUartBaudText[13] = (PU8)GetString(EM_DEV_BAUD_33900_TEXT);
	EmUartBaudText[14] = (PU8)GetString(EM_DEV_BAUD_38400_TEXT);
	EmUartBaudText[15] = (PU8)GetString(EM_DEV_BAUD_57600_TEXT);
	EmUartBaudText[16] = (PU8)GetString(EM_DEV_BAUD_115200_TEXT);
	EmUartBaudText[17] = (PU8)GetString(EM_DEV_BAUD_230400_TEXT);
	EmUartBaudText[18] = (PU8)GetString(EM_DEV_BAUD_460800_TEXT);
	EmUartBaudText[19] = (PU8)GetString(EM_DEV_BAUD_921600_TEXT);

     if( EmPortSetting == EM_APP_TST )
      {
	     EmUartPortHighlight = EmAppPortArray[EM_APP_TST];
  	     EmUartBaudHighlight = EmAppBaudArray[EM_APP_TST];
      }
     else
      {
	     EmUartPortHighlight = EmAppPortArray[EM_APP_PS];
  	     EmUartBaudHighlight = EmAppBaudArray[EM_APP_PS];
      }

	SetInlineItemCaption((EmInlineEditorStruct+EM_INLINE_UART_PORT_STR),(U8*)GetString(EM_DEV_SET_UART_PORT_TEXT));
	SetInlineItemCaption((EmInlineEditorStruct+EM_INLINE_UART_BAUD_STR),(U8*)GetString(EM_DEV_SET_UART_BUAD_TEXT));

 	SetInlineItemActivation((EmInlineEditorStruct+EM_INLINE_UART_PORT),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemActivation((EmInlineEditorStruct+EM_INLINE_UART_BAUD),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);

	SetInlineItemSelect((EmInlineEditorStruct+EM_INLINE_UART_PORT), \
	    EM_UART_MAX_PORT_NUM,   EmUartPortText, (PS32)&EmUartPortHighlight);
	SetInlineItemSelect((EmInlineEditorStruct+EM_INLINE_UART_BAUD), \
	    EM_BAUD_NUM,  EmUartBaudText, (PS32)&EmUartBaudHighlight);

}


/**************************************************************
**	FUNCTION NAME		: EntryEmSetUartPort
**
**  	PURPOSE				: Entry function to UART port setting menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmSetUartPort(void)
{
	U8* inputBuffer;     //added for inline edit history
	U8* guiBuffer;       /* Buffer holding history data */
	U16 inputBufferSize;
	U32 title;

	EntryNewScreen(EM_DEV_INLINE_CONFIG_UART_MENU_SCR, ExitEmSetUartPort, NULL, NULL);

	InitializeCategory57Screen();
	//DisableCategory57ScreenDone();
       EmSetUartFillInlineStruct();
	SetParentHandler(0);
	guiBuffer = GetCurrGuiBuffer(EM_DEV_INLINE_CONFIG_UART_MENU_SCR);
	inputBuffer = GetCurrNInputBuffer (EM_DEV_INLINE_CONFIG_UART_MENU_SCR, &inputBufferSize );    //added for inline edit history
	if(inputBuffer != NULL)		
		SetCategory57Data(EmInlineEditorStruct, EM_INLINE_STRUCT_MAX_NUM, inputBuffer);	

	if( EmPortSetting == EM_APP_TST )
		title = EM_DEV_SET_UART_TST_TEXT;
	else
		title = EM_DEV_SET_UART_PS_TEXT;

	ShowCategory57Screen(EM_DEV_SET_UART_TEXT, EM_DEV_SET_UART_ICON,
						 STR_GLOBAL_OK, ENGINEER_MODE_LSK_ICON,
						 STR_GLOBAL_BACK, ENGINEER_MODE_RSK_ICON,
						 EM_INLINE_STRUCT_MAX_NUM,
						 NULL,
						 EmInlineEditorStruct,
						 0,
						 guiBuffer);
	
	SetCategory57RightSoftkeyFunctions(SendSetUartReq,GoBackHistory);
	SetCategory57LeftSoftkeyFunction(SendSetUartReq);
	
}


/**************************************************************
**	FUNCTION NAME		: ExitEmSetUartPort
**
**  	PURPOSE				: Exit function to UART port setting menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitEmSetUartPort(void)
{
	history_t h;
	U16 inputBufferSize;                //added for inline edit history

	h.scrnID = EM_DEV_INLINE_CONFIG_UART_MENU_SCR;
	CloseCategory57Screen();

	h.entryFuncPtr = EntryEmSetUartPort;
	GetCategoryHistory(h.guiBuffer);
	inputBufferSize =(U16)  GetCategory57DataSize( );     //added for inline edit history
	GetCategory57Data ((U8*) h.inputBuffer );         //added for inline edit history
	AddNHistory(h, inputBufferSize);                       //added for inline edit history

}


/**************************************************************
**	FUNCTION NAME		: SendSetSleepModeReq
**
**  	PURPOSE				: Function to send sleep mode request
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SendSetSleepModeReq(void)
{
	EngineerModeSetSleepModeReq(currentHighlightIndex);
}


/**************************************************************
**	FUNCTION NAME		: SendGetSleepModeReq
**
**  	PURPOSE				: Function to enter sleep mode menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SendGetSleepModeReq(void)
{
     EntryEmSetSleepMode(0);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmSetSleepMode
**
**  	PURPOSE				: Entry function to set sleep mode menu
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmSetSleepMode(U8 index)
{

	U16 nStrItemList[] = {EM_DEV_SLEEPMODE_ON_TEXT,
						  EM_DEV_SLEEPMODE_OFF_TEXT};
	U8 nNumofItem = 2;
	U8* guiBuffer;

       currentHighlightIndex=index;

	EntryNewScreen(EM_DEV_SLEEPMODE_MENU_SCR, ExitEmSetSleepMode, NULL, NULL);
	guiBuffer = GetCurrGuiBuffer (EM_DEV_SLEEPMODE_MENU_SCR);
	
	RegisterHighlightHandler (SetHighlightIndex);
	ShowCategory11Screen (EM_DEV_SLEEPMODE_TEXT, EM_DEV_SLEEPMODE_ICON,
						  STR_GLOBAL_OK, 0,
						  STR_GLOBAL_BACK, 0,
						  nNumofItem, nStrItemList, currentHighlightIndex, guiBuffer);

	SetLeftSoftkeyFunction(SendSetSleepModeReq,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: ExitEmSetSleepMode
**
**  	PURPOSE				: Exit function to set sleep mode menu
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitEmSetSleepMode(void)
{
	history_t   currHistory;
	S16		nHistory	=	0;

	currHistory.scrnID = EM_DEV_SLEEPMODE_MENU_SCR;
	currHistory.entryFuncPtr = SendGetSleepModeReq;
	pfnUnicodeStrcpy ((S8*)currHistory.inputBuffer, (S8*)&nHistory);
	GetCategoryHistory(currHistory.guiBuffer);
	AddHistory(currHistory);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmDevice
**
**  	PURPOSE				: Highlight function for Device menu
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmDevice(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmDeviceMenu,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: HighlightEmSetUart
**
**  	PURPOSE				: Highlight function for Uart setting menu
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmSetUart(void)
{
	/*1 Change text for left soft key */
	change_left_softkey(STR_GLOBAL_OK,0);

	/*2 Redraw left soft key */
	redraw_left_softkey();
	
	/*3 Register function for left soft key */
	//SetLeftSoftkeyFunction(SendGetUartReq,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(EntryEmSetUartMenu,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: HighlightEmSetSleepMode
**
**  	PURPOSE				: Highlight function for sleep mode menu
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmSetSleepMode(void)
{
	/*1 Change text for left soft key */
	change_left_softkey(STR_GLOBAL_OK,0);

	/*2 Redraw left soft key */
	redraw_left_softkey();
	
	/*3 Register function for left soft key */
	SetLeftSoftkeyFunction(SendGetSleepModeReq,KEY_EVENT_UP);

	/*4 Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/* ======== GPRS ACT ======== */

/**************************************************************
**	FUNCTION NAME		: EntryEmGprsMenuNotSupport
**
**  	PURPOSE				: Entry function for GPRS not support menu 
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmGprsMenuNotSupport(void)
{
         U8 StrBuf[40];

  	  EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

         memset( StrBuf, 0, sizeof(StrBuf));
         AnsiiToUnicodeString((PS8)StrBuf, "Not Support!" );
         EngineerModeNoticeText( StrBuf, IMG_GLOBAL_ERROR);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmGprsMenu
**
**  	PURPOSE				: Entry function for GPRS menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmGprsMenu(void)
{
	U16		ItemList[32];
	U16		ItemIcons[32];
	U16		nItems;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;

	EntryNewScreen(EM_GPRS_ACT_MENU_SCR, NULL, EntryEmGprsMenu, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_GPRS_ACT_MENU_SCR);
	
	nItems = GetNumOfChild(EM_GPRS_ACT_MENUID);
	GetSequenceStringIds(EM_GPRS_ACT_MENUID,ItemList);
	SetParentHandler(EM_GPRS_ACT_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	ShowCategory52Screen(EM_GPRS_ACT_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: EntryEmAutoAttach
**
**  	PURPOSE				: Entry function for GPRS Auto detachment  menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmAutoAttach(void)
{
	/* 1 Call Exit Handler*/
	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

       SendGprsAttachReq();
}


/**************************************************************
**	FUNCTION NAME		: EntryEmActivatePDPMenu
**
**  	PURPOSE				: Entry function for GPRS Active PDP menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmActivatePDPMenu(void)
{
	U16		ItemList[32];
	U16		ItemIcons[32];
	U16		nItems;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;


	EntryNewScreen(EM_GPRS_ACTIVATE_PDP_MENU_SCR, NULL, EntryEmActivatePDPMenu, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_GPRS_ACTIVATE_PDP_MENU_SCR);
	
	nItems = GetNumOfChild(EM_GPRS_ACTIVATE_PDP_MENUID);
	GetSequenceStringIds(EM_GPRS_ACTIVATE_PDP_MENUID,ItemList);
	SetParentHandler(EM_GPRS_ACTIVATE_PDP_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	ShowCategory52Screen(EM_GPRS_ACTIVATE_PDP_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: EntryEmSendDataMenu
**
**  	PURPOSE				: Entry function for GPRS Send data menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmSendDataMenu(void)
{
	U16		ItemList[32];
	U16		ItemIcons[32];
	U16		nItems;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;


	EntryNewScreen(EM_GPRS_SEND_DATA_MENU_SCR, NULL, EntryEmSendDataMenu, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_GPRS_SEND_DATA_MENU_SCR);
	
	nItems = GetNumOfChild(EM_GPRS_SEND_DATA_MENUID);
	GetSequenceStringIds(EM_GPRS_SEND_DATA_MENUID,ItemList);
	SetParentHandler(EM_GPRS_SEND_DATA_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

	ShowCategory52Screen(EM_GPRS_SEND_DATA_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: EntryEmSendData
**
**  	PURPOSE				: Entry function for GPRS Send data 
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmSendData(void)
{
	byte *history_buffer;

      currentHighlightIndex = (U8)GetHighlightedItem();

	EntryNewScreen(EM_GPRS_SEND_DATA_EDITOR_SCR, NULL, EntryEmSendData, NULL);
	history_buffer=GetCurrGuiBuffer(EM_GPRS_SEND_DATA_EDITOR_SCR);

	memset( InputBuf, 0, sizeof( InputBuf ) );
	ShowCategory16Screen(STR_GLOBAL_OK,0,STR_GLOBAL_BACK,0,(U8*)InputBuf, 8,history_buffer);

	SetLeftSoftkeyFunction(EmSendDataScreenLSKHandler,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory,KEY_END,KEY_EVENT_UP);
	//SetKeyHandler(DialPadCall,KEY_SEND,KEY_EVENT_UP);
	SetCategory16RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	//SetKeyHandler(SSCStringParsing,KEY_POUND,KEY_EVENT_UP);	
}


/**************************************************************
**	FUNCTION NAME		: EmSendDataScreenLSKHandler
**
**  	PURPOSE				: LSK handler for GPRS Send data menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmSendDataScreenLSKHandler(void)
{
     kal_uint32 size = 0;

     EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

     UnicodeToAnsii( (S8*) EmGprsSendDataSizeBuffer, (S8*) InputBuf);
     size = (kal_uint32) atoi( EmGprsSendDataSizeBuffer );
     if( currentHighlightIndex == 0 )
        SendGprsSendDataReq(size);
     else
        SendGprsSendDataReq( (U32)(size-EM_TCM_PDU_HDR) );
      
}


/**************************************************************
**	FUNCTION NAME		: EntryEmDeactivatePDP
**
**  	PURPOSE				: Entry function for GPRS Deactivate PDP menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmDeactivatePDP(void)
{
	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);
	
       SendGprsDeactivatePDPReq();
}


/**************************************************************
**	FUNCTION NAME		: EntryEmPDPContext1
**
**  	PURPOSE				: Entry function for GPRS PDPContext1 menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmPDPContext1(void)
{
	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

	SendGprsActivatePDPContext(1);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmPDPContext2
**
**  	PURPOSE				: Entry function for GPRS PDPContext2 menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmPDPContext2(void)
{
	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

	SendGprsActivatePDPContext(2);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmPDPContext3
**
**  	PURPOSE				: Entry function for GPRS PDPContext3 menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmPDPContext3(void)
{
	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

	SendGprsActivatePDPContext(3);
	
}


/**************************************************************
**	FUNCTION NAME		: EntryEmPDPContext4
**
**  	PURPOSE				: Entry function for GPRS PDPContext4 menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmPDPContext4(void)
{
 	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

	SendGprsActivatePDPContext(4);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmPDPContext5
**
**  	PURPOSE				: Entry function for GPRS PDPContext5 menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmPDPContext5(void)
{
 	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

	SendGprsActivatePDPContext(5);
	
}


/**************************************************************
**	FUNCTION NAME		: EntryEmPDPContext6
**
**  	PURPOSE				: Entry function for GPRS PDPContext6 menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmPDPContext6(void)
{
 	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

	SendGprsActivatePDPContext(6);

}


/**************************************************************
**	FUNCTION NAME		: EntryEmPDPContext7
**
**  	PURPOSE				: Entry function for GPRS PDPContext7 menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmPDPContext7(void)
{
 	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

	SendGprsActivatePDPContext(7);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmPDPContext8
**
**  	PURPOSE				: Entry function for GPRS PDPContext8 menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmPDPContext8(void)
{
 	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

	SendGprsActivatePDPContext(8);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmPDPContext9
**
**  	PURPOSE				: Entry function for GPRS PDPContext9 menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmPDPContext9(void)
{
 	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

	SendGprsActivatePDPContext(9);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmPDPContext10
**
**  	PURPOSE				: Entry function for GPRS PDPContext10 menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmPDPContext10(void)
{
	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

	SendGprsActivatePDPContext(10);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmPDPContext11
**
**  	PURPOSE				: Entry function for GPRS PDPContext11 menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmPDPContext11(void)
{
	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

	SendGprsActivatePDPContext(11);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmPDPContext12
**
**  	PURPOSE				: Entry function for GPRS PDPContext12 menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmPDPContext12(void)
{
	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

	SendGprsActivatePDPContext(12);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmPDPContext13
**
**  	PURPOSE				: Entry function for GPRS PDPContext13 menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmPDPContext13(void)
{
	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

	SendGprsActivatePDPContext(13);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmPDPContext14
**
**  	PURPOSE				: Entry function for GPRS PDPContext14 menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmPDPContext14(void)
{
	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

	SendGprsActivatePDPContext(14);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmPDPContext15
**
**  	PURPOSE				: Entry function for GPRS PDPContext15 menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmPDPContext15(void)
{
	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

	SendGprsActivatePDPContext(15);
}


/**************************************************************
**	FUNCTION NAME		: SendGprsAttachReq
**
**  	PURPOSE				: Function for sending GPRS attach request
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SendGprsAttachReq(void)
{
   EngineerModeGprsAttachReq();
}


/**************************************************************
**	FUNCTION NAME		: SendGprsDeactivatePDPReq
**
**  	PURPOSE				: Function for sending GPRS Deactivate PDP request
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SendGprsDeactivatePDPReq(void)
{
   EngineerModeGprsDeactivatePDPReq();
}


/**************************************************************
**	FUNCTION NAME		: SendGprsSendDataReq
**
**  	PURPOSE				: Function for sending GPRS Send Data request
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SendGprsSendDataReq(kal_uint32 size)
{
   EngineerModeGprsSendDataReq(size);
}


/**************************************************************
**	FUNCTION NAME		: SendGprsActivatePDPContext
**
**  	PURPOSE				: Function for sending GPRS Activate PDP request
**
**	INPUT PARAMETERS	: pdp_idx
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SendGprsActivatePDPContext(U8 pdp_idx )
{
#ifdef __MOD_TCM__
   g_gprs_pdp_idx = pdp_idx;
   g_gprs_set_minqos_required = KAL_FALSE;
#endif

   EngineerModeGprsActivatePDPContext(pdp_idx);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmGprs
**
**  	PURPOSE				: Highlight function for GPRS menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmGprs(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(GotoEmGprsMenu,KEY_EVENT_UP);
	
	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: HighlightEmAutoAttach
**
**  	PURPOSE				: Highlight function for GPRS auto detach
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmAutoAttach(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmAutoAttach,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: HighlightEmActivatePDP
**
**  	PURPOSE				: Highlight function for GPRS Activate PDP
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmActivatePDP(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmActivatePDPMenu,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmDeactivatePDP
**
**  	PURPOSE				: Highlight function for GPRS Deactivate PDP
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmDeactivatePDP(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmDeactivatePDP,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmSendDataMenu
**
**  	PURPOSE				: Highlight function for GPRS Send data menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmSendDataMenu(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmSendDataMenu,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmSendData
**
**  	PURPOSE				: Highlight function for GPRS Send data
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmSendData(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmSendData,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmPDPContext1
**
**  	PURPOSE				: Highlight function for PDPContext1
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmPDPContext1(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmPDPContext1,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmPDPContext2
**
**  	PURPOSE				: Highlight function for PDPContext2
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmPDPContext2(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmPDPContext2,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmPDPContext3
**
**  	PURPOSE				: Highlight function for PDPContext3
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmPDPContext3(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmPDPContext3,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmPDPContext4
**
**  	PURPOSE				: Highlight function for PDPContext4
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmPDPContext4(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmPDPContext4,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmPDPContext5
**
**  	PURPOSE				: Highlight function for PDPContext5
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmPDPContext5(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmPDPContext5,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmPDPContext6
**
**  	PURPOSE				: Highlight function for PDPContext6
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmPDPContext6(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmPDPContext6,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmPDPContext7
**
**  	PURPOSE				: Highlight function for PDPContext7
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmPDPContext7(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmPDPContext7,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmPDPContext8
**
**  	PURPOSE				: Highlight function for PDPContext8
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmPDPContext8(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmPDPContext8,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmPDPContext9
**
**  	PURPOSE				: Highlight function for PDPContext9
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmPDPContext9(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmPDPContext9,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmPDPContext10
**
**  	PURPOSE				: Highlight function for PDPContext10
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmPDPContext10(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmPDPContext10,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmPDPContext11
**
**  	PURPOSE				: Highlight function for PDPContext11
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmPDPContext11(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmPDPContext11,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmPDPContext12
**
**  	PURPOSE				: Highlight function for PDPContext12
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmPDPContext12(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmPDPContext12,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmPDPContext13
**
**  	PURPOSE				: Highlight function for PDPContext13
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmPDPContext13(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmPDPContext13,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmPDPContext14
**
**  	PURPOSE				: Highlight function for PDPContext14
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmPDPContext14(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmPDPContext14,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmPDPContext15
**
**  	PURPOSE				: Highlight function for PDPContext15
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmPDPContext15(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmPDPContext15,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: SetHighlightIndex
**
**  	PURPOSE				: Function for highlight index
**
**	INPUT PARAMETERS	: nIndex
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SetHighlightIndex(S32 nIndex)
{
	currentHighlightIndex = (U8) nIndex;
}


/**************************************************************
**	FUNCTION NAME		: SendBandSelectReq
**
**  	PURPOSE				: Function for band select request
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SendBandSelectReq(void)
{
 	U8 MessageString[30];

	EngineerModeSetBandReq(currentHighlightIndex);

	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);


	memset(MessageString,0,30);
       AnsiiToUnicodeString((PS8)MessageString,"Searching...");
	
	ShowCategory63Screen(MessageString,EM_NETWORK_ICON,NULL);
	
       ClearAllKeyHandler();
}


/**************************************************************
**	FUNCTION NAME		: EmAssignBandSelMenuItem
**
**  	PURPOSE				: Function for band select request
**
**	INPUT PARAMETERS	: nStrItemList, nNumofItem
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmAssignBandSelMenuItem( U16 *nStrItemList, U8 *nNumofItem )
{
//support of designated band: PGSM900, EGSM900, RGSM900, DCS1800, PCS1900, GSM850, GSM450, GSM480, DUAL900, TRIPLE, QUAD, DUAL850
#ifndef MMI_ON_HARDWARE_P
#define __EGSM900__ 1
#define __DCS1800__ 1
#define __PCS1900__ 1
#endif

	U8 idx=0;
	
//900
#if ( defined(__PGSM900__) ||defined(__EGSM900__) ||defined(__RGSM900__) )
	if( idx >= EM_BANDSEL_MAX )
		return;

    if(nStrItemList)
	{
		nStrItemList[idx] = EM_BAND_900_TEXT;
	}

    EMBandIndex[idx]=EM_BANDSEL_900;
    idx++;
#endif

//1800
#if( defined(__DCS1800__) )
	if( idx >= EM_BANDSEL_MAX )
		return;

    if(nStrItemList)
	{
		nStrItemList[idx] = EM_BAND_1800_TEXT;
	}

    EMBandIndex[idx]=EM_BANDSEL_1800;
    idx++;

#endif


//DUAL
#if ( (defined(__PGSM900__) ||defined(__EGSM900__) ||defined(__RGSM900__)) && defined(__DCS1800__) )
	if( idx >= EM_BANDSEL_MAX )
		return;

    if(nStrItemList)
	{
		nStrItemList[idx] = EM_BAND_DUAL_TEXT;
	}

    EMBandIndex[idx]=EM_BANDSEL_DUAL;
    idx++;
#endif

//1900
#if( defined(__PCS1900__) )
	if( idx >= EM_BANDSEL_MAX )
		return;

    if(nStrItemList)
	{
		nStrItemList[idx] = EM_BAND_1900_TEXT;
	}

    EMBandIndex[idx]=EM_BANDSEL_1900;
    idx++;
#endif

//850
#if( defined(__GSM850__) )
	if( idx >= EM_BANDSEL_MAX )
		return;

    if(nStrItemList)
	{
		nStrItemList[idx] = EM_BAND_850_TEXT;
	}

    EMBandIndex[idx]=EM_BANDSEL_850;
    idx++;

#endif

//Lisen 05312005	
//#if defined(__AUTO_BAND_SWITCH__)
	if( idx >= EM_BANDSEL_MAX )
		return;

    if(nStrItemList)
	{
		nStrItemList[idx] = EM_BAND_AUTO_TEXT;
	}

    EMBandIndex[idx]=EM_BANDSEL_AUTO;
    idx++;

//#endif	


	if( nNumofItem )
	 *nNumofItem = idx;
}


/**************************************************************
**	FUNCTION NAME		: EntryEmBandSelection
**
**  	PURPOSE				: Entry function for band selection menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmBandSelection(void)
{
       U16 nStrItemList[EM_BANDSEL_MAX];
       U8 nNumofItem; 
	U8* guiBuffer;

       EmAssignBandSelMenuItem( nStrItemList, &nNumofItem );

	EntryNewScreen(EM_BAND_SEL_MENU_SCR, NULL, EntryEmBandSelection, NULL);

	guiBuffer = GetCurrGuiBuffer (EM_BAND_SEL_MENU_SCR);
	
	RegisterHighlightHandler(SetHighlightIndex);

	ShowCategory11Screen (EM_BAND_SEL_TEXT, 0,
						  STR_GLOBAL_OK, 0,
						  STR_GLOBAL_BACK, 0,
						  nNumofItem, nStrItemList, currentHighlightIndex, guiBuffer);

						  
	SetLeftSoftkeyFunction(SendBandSelectReq,KEY_EVENT_UP);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: HighlightEmBandSelection
**
**  	PURPOSE				: Highlight function for band selection menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmBandSelection(void)
{
       SetProtocolEventHandler(EngineerModeGetBandRsp, PRT_NW_GET_BAND_RSP);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EngineerModeGetBandReq,(U16)KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,(U16)KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: EntryEmMisc
**
**  	PURPOSE				: Entry function for MISC menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmMisc(void)
{
	U16		ItemList[32];
	U16		ItemIcons[32];
	U16		nItems;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;
	S32		index;

	EntryNewScreen(EM_MISC_MENU_SCR, NULL, EntryEmMisc, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_MISC_MENU_SCR);

	nItems = GetNumOfChild(EM_MISC_MENUID);
	GetSequenceStringIds(EM_MISC_MENUID,ItemList);
	SetParentHandler(EM_MISC_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i); 
	}

	index = 0;

	/* EM_AUTO_ANSWER_MENUID */
   if (gprofiles[0].answeringMode.automatic == 1)
      PopUpList[index] = (U8*)get_string(STR_GLOBAL_ON);
   else
      PopUpList[index] = (U8*)get_string(STR_GLOBAL_OFF);

	index++;

	/* EM_HIGHSPEED_SIM_MENUID */
	#ifdef MMI_ON_HARDWARE_P      
	   if (High_Speed_SIM)
	      PopUpList[index] = (U8*)get_string(STR_GLOBAL_ON);
	   else
	      PopUpList[index] = (U8*)get_string(STR_GLOBAL_OFF);
	#endif   	
	index++;

	/* EM_POWER_ON_DURATION_MENUID */
	index++;
	
	/* EM_BACKLIGHT_MENUID */
	#ifdef MMI_ON_HARDWARE_P      
  	     if (backlight_mode == BACKLIGHT_NORMAL)
  	     	   PopUpList[index] = (U8*)get_string(STR_GLOBAL_OFF);
	     else
	           PopUpList[index] = (U8*)get_string(STR_GLOBAL_ON);
	#endif   	
	index++;
	
	/* EM_SERIAL_PORT_EN_MENUID */
	index++;
	
	/* EM_AUTO_RESET_MENUID */
	PopUpList[index] = (U8*)AUTO_RESET_WARNING;	
	index++;
	
	/* EM_RAM_TEST_MENUID */
	index++;
	
	/* EM_MEMORY_DUMP_MENUID */
    if (INT_MemoryDumpFlag == EX_FORCEMEMORYDUMP)
      PopUpList[index] = (U8*)get_string(STR_GLOBAL_ON);
    else
      PopUpList[index] = (U8*)get_string(STR_GLOBAL_OFF);
    
	index++;
	
	/* EM_MMI_DEBUG_MENUID */
	#ifdef DEBUG_ASSERT
		if (g_mmi_frm_enable_debug_assert)
			PopUpList[index] = (U8*)get_string(STR_GLOBAL_ON);
		else
			PopUpList[index] = (U8*)get_string(STR_GLOBAL_OFF);
	#endif	
	
	index++;

	/* EM_AMR_MENUID */
	#if defined(__AMR_SUPPORT__)
      {
			S16 ret, error_code;
         
			memset(mscap, 0, NVRAM_EF_MSCAP_SIZE);
			ret = ReadRecord( NVRAM_EF_MSCAP_LID, 1, &mscap, NVRAM_EF_MSCAP_SIZE, &error_code);
         if (!(error_code == NVRAM_READ_SUCCESS && ret == NVRAM_EF_MSCAP_SIZE))
         {
             ASSERT(0);              
         }
         
         if (mscap[0] == 0x1f)
         	AMR_ON = TRUE;
         else
         	AMR_ON = FALSE;
      }

      if(AMR_ON)
         PopUpList[index] = (U8*)get_string(STR_GLOBAL_ON);
      else
         PopUpList[index] = (U8*)get_string(STR_GLOBAL_OFF);

	
		index++;
	#endif        
	
	/* EM_WAP_MENUID */
	#if defined(WAP_SUPPORT) || defined(MMI_ON_WIN32)                          
		index++;
	#endif    	
	
	/* EM_J2ME_TCK_MENUID */                        
	#ifdef __J2ME__
		index++;
	#endif     
	
	/* EM_TOUCHSCREEN_MENUID */
	#if defined(__MMI_TOUCH_SCREEN__)
		index++;
	#endif

	/* EM_TVOUT_MENUID */
	#if defined(__MMI_TVOUT__)
      if(gIsTVOut)
         PopUpList[index] = (U8*)get_string(STR_GLOBAL_ON);
      else
         PopUpList[index] = (U8*)get_string(STR_GLOBAL_OFF);

		gTVOutIdx = index;
	
		index++;
	#endif 

	/* EM_VIDEO_HI_BITRATE_MENUID */
	#if defined(__MMI_VIDEO__) && defined(ISP_SUPPORT)
		{
			#ifdef MMI_ON_HARDWARE_P
				U8 mode;
			
				mode = media_vid_get_em_mode();

				if(mode == VID_EM_MODE_DISABLED)
				{
					PopUpList[index] = (U8*)get_string(STR_GLOBAL_OFF);
				}
				else if(mode == VID_EM_MODE_ULTRA_HIGH_BITRATE)
				{
					PopUpList[index] = (U8*)get_string(STR_GLOBAL_ON);	
				}
				else
				{
					ASSERT(0);
				}
				
				gVideoHiBitrateIdx = index;
			#endif /* MMI_ON_HARDWARE_P */				
		}
	
		index++;
	#endif 									

	ShowCategory52Screen(EM_MISC_TEXT,0,
								STR_GLOBAL_OK,0,
								STR_GLOBAL_BACK,0,
								nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: HighlightEmMisc
**
**  	PURPOSE				: Highlight function for MISC menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmMisc(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmMisc,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

#ifdef __MMI_TOUCH_SCREEN__

#undef EM_PEN_TEST_HANW

#include "wgui_touch_screen.h"

#ifdef EM_PEN_TEST_HANW
#include "hw.h"
#endif

/* Previous point in pen test program */
static mmi_pen_point_struct gEmPenPreviousPoint;

/* Capture pen events by test drawing program */
static BOOL gEmPenTestCapture = MMI_FALSE;

/* Line color */
static color_t gEmPenLineColor = {0,0,0,100};

/* Stroke test */
#define EM_PEN_STROKE_BUFFER_SIZE 500
static mmi_pen_point_struct *gEmPenStrokeBuffer;

 
static S16 gEmPenMaxDisplacementX = 0;
static S16 gEmPenMaxDisplacementY = 0;

static mmi_pen_handwriting_area_struct gEmPenStrokeArea;

#ifdef EM_PEN_TEST_HANW
/*Single line input box for display candidates for hand writing*/
static single_line_input_box touchCandidateArea;
extern unsigned char bufRam[12*1024];
extern const unsigned char hand_writing_data[];
#endif

static void EmPenTestPenDown(mmi_pen_point_struct pos)
{
	S16 test_x1 = (S16) MMI_content_x;
	S16 test_x2 = (S16) (MMI_content_x + MMI_content_width - 1);
	S16 test_y1 = (S16) MMI_content_y; 
	S16 test_y2 = (S16) (MMI_content_y + MMI_content_height - 1);
	if( pos.x < test_x1 ||
		pos.x > test_x2 ||
		pos.y < test_y1 ||
		pos.y > test_y2)
	{
		gEmPenPreviousPoint.x = -1;
		gEmPenPreviousPoint.y = -1;
		mmi_wgui_general_pen_down_hdlr(pos);
		gEmPenTestCapture = MMI_FALSE;
	}
	else
	{
		gEmPenPreviousPoint.x=pos.x;
		gEmPenPreviousPoint.y=pos.y;
		gEmPenTestCapture = MMI_TRUE;
		gEmPenLineColor = coolsand_UI_color(0,0,0);
	}
}


static void EmPenTestPenLongTap(mmi_pen_point_struct pos)
{
	if (!gEmPenTestCapture)
	{
		mmi_wgui_general_pen_long_tap_hdlr(pos);
	}
	else
	{
		gEmPenLineColor = coolsand_UI_color(255,0,0);
	}
}


static void EmPenTestPenAbort(mmi_pen_point_struct pos)
{
	if (!gEmPenTestCapture)
	{
		mmi_wgui_general_pen_abort_hdlr(pos);
	}
}


static void EmPenTestPenRepeat(mmi_pen_point_struct pos)
{
	if (!gEmPenTestCapture)
	{
		mmi_wgui_general_pen_repeat_hdlr(pos);
	}
	else
	{
		gEmPenLineColor =coolsand_UI_color(19,7,248);
	}
}


static void EmPenTestPenMove(mmi_pen_point_struct pos )
{
	S16 test_x1 = MMI_content_x;
	S16 test_x2 = MMI_content_x+MMI_content_width-1;
	S16 test_y1 = MMI_content_y; 
	S16 test_y2 = MMI_content_y + MMI_content_height-1;
	S16 disp_x, disp_y;

	if (!gEmPenTestCapture)
	{
		mmi_wgui_general_pen_move_hdlr(pos);
		return;
	}
	
	if( pos.x < test_x1)
	{
		pos.x = test_x1;
	}
	if( pos.x > test_x2)
	{
		pos.x = test_x2;
	}
	if( pos.y < test_y1)
	{
		pos.y = test_y1;
	}
	if( pos.y > test_y2)
	{
		pos.y = test_y2;
	}
	coolsand_UI_lock_double_buffer();
	coolsand_UI_push_clip();
	coolsand_UI_set_clip(0,0,UI_device_width-1,UI_device_height-1);
	coolsand_UI_line(gEmPenPreviousPoint.x,gEmPenPreviousPoint.y,pos.x,pos.y,gEmPenLineColor);
	
	disp_x=PEN_ABS(pos.x, gEmPenPreviousPoint.x);
	disp_y=PEN_ABS(pos.y, gEmPenPreviousPoint.y);
	if(disp_x>gEmPenMaxDisplacementX)
	{
		gEmPenMaxDisplacementX = disp_x;
	}
	if(disp_y>gEmPenMaxDisplacementY)
	{
		gEmPenMaxDisplacementY = disp_y;
	}

	coolsand_UI_pop_clip();
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(PEN_MIN(gEmPenPreviousPoint.x, pos.x),
									PEN_MIN(gEmPenPreviousPoint.y, pos.y),
									PEN_MAX(gEmPenPreviousPoint.x, pos.x),
									PEN_MAX(gEmPenPreviousPoint.y, pos.y));

	gEmPenPreviousPoint.x = pos.x;
	gEmPenPreviousPoint.y = pos.y;
}


static void EmPenTestPenUp(mmi_pen_point_struct pos)
{
	if (!gEmPenTestCapture)
	{
		mmi_wgui_general_pen_up_hdlr(pos);
	}
	else
	{
		coolsand_UI_lock_double_buffer();

		EmPenTestPenMove(pos);
		
		coolsand_UI_line(pos.x-2, pos.y-2, pos.x+2, pos.y+2, gEmPenLineColor);
		coolsand_UI_line(pos.x-2, pos.y+2, pos.x+2, pos.y-2, gEmPenLineColor);
		coolsand_UI_unlock_double_buffer();
		coolsand_UI_BLT_double_buffer(pos.x-2, pos.y-2, pos.x+2, pos.y+2);
		gEmPenPreviousPoint.x = -1;
		gEmPenPreviousPoint.y = -1;
		gEmPenLineColor =coolsand_UI_color(0,0,0);
	}
}


static void EmPenTestStrokeDown(mmi_pen_point_struct pos)
{
	gEmPenPreviousPoint.x=pos.x;
	gEmPenPreviousPoint.y=pos.y;
	gEmPenLineColor = coolsand_UI_color(0,255,0);
}


static void EmPenTestStrokeMove(mmi_pen_point_struct pos)
{
	coolsand_UI_lock_double_buffer();
	coolsand_UI_push_clip();
	coolsand_UI_set_clip(0,0,UI_device_width-1,UI_device_height-1);
	coolsand_UI_line(gEmPenPreviousPoint.x,gEmPenPreviousPoint.y,pos.x,pos.y,gEmPenLineColor);
	coolsand_UI_line(gEmPenPreviousPoint.x+1,gEmPenPreviousPoint.y,pos.x+1,pos.y,gEmPenLineColor);
	coolsand_UI_line(gEmPenPreviousPoint.x,gEmPenPreviousPoint.y+1,pos.x,pos.y+1,gEmPenLineColor);
	coolsand_UI_line(gEmPenPreviousPoint.x-1,gEmPenPreviousPoint.y,pos.x-1,pos.y,gEmPenLineColor);
	coolsand_UI_line(gEmPenPreviousPoint.x,gEmPenPreviousPoint.y-1,pos.x,pos.y-1,gEmPenLineColor);

	coolsand_UI_pop_clip();
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(PEN_MIN(gEmPenPreviousPoint.x, pos.x)-1,
									PEN_MIN(gEmPenPreviousPoint.y, pos.y)-1,
									PEN_MAX(gEmPenPreviousPoint.x, pos.x)+1,
									PEN_MAX(gEmPenPreviousPoint.y, pos.y)+1);

	gEmPenPreviousPoint.x = pos.x;
	gEmPenPreviousPoint.y = pos.y;
}


static void EmPenTestStrokeUpTimerHdlr(void)
{
#ifdef EM_PEN_TEST_HANW
	BOOL has_unfinished_stroke;
	U16* psBufferTouch;
	
	mmi_pen_peek_stroke_state(&has_unfinished_stroke);
	if (!has_unfinished_stroke)
	{
		int num = 0,i=0;
		char szResult[40];
		UI_character_type curCh;

		mmi_pen_end_strokes_of_character();

		psBufferTouch = (U16*)OslMalloc((10+2) * sizeof(U16));
		coolsand_UI_create_single_line_input_box_use_buffer(&touchCandidateArea,
		                                               MMI_multitap_x,
		                                               MMI_multitap_y-2,//+1,
		                                               MMI_multitap_width,
		                                               MMI_multitap_height,
		                                               psBufferTouch,
		                                               ((10+1) * sizeof(U16)));
		touchCandidateArea.flags |= UI_SINGLE_LINE_INPUT_BOX_DISABLE_SCROLLING;
		touchCandidateArea.flags |= UI_SINGLE_LINE_INPUT_BOX_CHARACTER_HIGHLIGHT;
		
		HWSetDictionary( (DWORD)bufRam, (DWORD) hand_writing_data);
		coolsand_UI_single_line_input_box_delete_all(&touchCandidateArea);

		num = HWRecognize( (U16*)gEmPenStrokeBuffer, szResult, 5, ALC_CHINESE_COMMON );
		for (;i<num*2;i+=2)
		{
			U16 c1 = szResult[i];
			U16 c2 = szResult[i+1];
			curCh = c1 | (c2 << 8);
			coolsand_UI_single_line_input_box_insert_character(&touchCandidateArea,(UI_character_type)curCh);
		}
		coolsand_UI_single_line_input_box_goto_first_character(&touchCandidateArea);
		coolsand_UI_single_line_input_box_next(&touchCandidateArea);
		touchCandidateArea.flags |= UI_SINGLE_LINE_INPUT_BOX_STATE_MULTITAP;
		touchCandidateArea.flags |= UI_SINGLE_LINE_INPUT_BOX_FULL_HEIGHT_BLOCK_CURSOR;
		coolsand_UI_lock_double_buffer();
		coolsand_UI_show_single_line_input_box(&touchCandidateArea);
		coolsand_UI_unlock_double_buffer();
		coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
		mmi_pen_begin_strokes_of_character();		

		OslMfree(psBufferTouch);
	}
#else
	BOOL has_unfinished_stroke;
	mmi_pen_peek_stroke_state(&has_unfinished_stroke);
	if (!has_unfinished_stroke)
	{
		mmi_pen_end_strokes_of_character();
		mmi_pen_begin_strokes_of_character();
	}
#endif /* EM_PEN_TEST_HANW */
}


static void EmPenTestStrokeUp(mmi_pen_point_struct pos)
{
	EmPenTestStrokeMove(pos);
	gEmPenPreviousPoint.x = -1;
	gEmPenPreviousPoint.y = -1;
	coolsand_UI_cancel_timer(EmPenTestStrokeUpTimerHdlr);
	coolsand_UI_start_timer(1000, EmPenTestStrokeUpTimerHdlr);
}

static void EmPenTestShowBorder(void)
{
	coolsand_UI_draw_rectangle(gEmPenStrokeArea.x1, gEmPenStrokeArea.y1, gEmPenStrokeArea.x2, gEmPenStrokeArea.y2, coolsand_UI_color(128, 128, 128));
}

void EmPenTestClear(void)
 {
	coolsand_UI_lock_double_buffer();
	coolsand_UI_push_clip();
	coolsand_UI_set_clip(0,0,UI_device_width-1,UI_device_height-1);
	RedrawCategory400Screen();
	EmPenTestShowBorder();
	coolsand_UI_pop_clip();
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);	
 }
 
void EntryEmPenStatisticsList(void)
 {
	U8 *guiBuffer;

	EntryNewScreen(EM_TOUCHSCREEN_TEST_STATISTICS_SCR, NULL, EntryEmPenStatisticsList, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_TOUCHSCREEN_TEST_STATISTICS_SCR);
	
	sprintf((char*) EMFMAsciiDisplayBuf, "Max_distance_X: %d \n Max_distance_Y: %d\n", 
		     gEmPenMaxDisplacementX,gEmPenMaxDisplacementY);
	
	AnsiiToUnicodeString((S8*)EMFMUnicodeDisplayBuf, (S8*)EMFMAsciiDisplayBuf);
	
	ShowCategory74Screen(EM_TOUCHSCREEN_TEST_STATISTICS_TEXT, 0, 0, 0, STR_GLOBAL_BACK,IMG_GLOBAL_BACK, 
		                               (PU8) EMFMUnicodeDisplayBuf, sizeof(EMFMUnicodeDisplayBuf), guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
 }


 void EmPenLeaveTest(void)
 {
	 GoBeyondMarkerScr(EM_TOUCHSCREEN_TEST_SCR);
 }


 void EntryEmPenOptions(void)
 {
	U16		ItemList[32];
	U16		ItemIcons[32];
	U16		nItems;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;

	EntryNewScreen(EM_TOUCHSCREEN_TEST_OPTION_SCR, NULL, EntryEmPenOptions, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_TOUCHSCREEN_TEST_OPTION_SCR);
	
	nItems = GetNumOfChild(EM_TOUCHSCREEN_TEST_OPTION_MENUID);
	GetSequenceStringIds(EM_TOUCHSCREEN_TEST_OPTION_MENUID,ItemList);
	SetParentHandler(EM_TOUCHSCREEN_TEST_OPTION_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);
	}

	ShowCategory52Screen(EM_TOUCHSCREEN_TEST_STATISTICS_TEXT,0,
							STR_GLOBAL_OK, IMG_GLOBAL_OK,
							STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);
	
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
 }


void ExitEmPenTest(void)
{
	mmi_pen_stop_capture_strokes();
	OslMfree(gEmPenStrokeBuffer);
	gEmPenStrokeBuffer = NULL;
}

 void EntryEmPenTest(void)
{
	U8 *guiBuffer; 
	mmi_pen_handwriting_area_struct ext_stroke;

	ext_stroke.x1 = 0;
	ext_stroke.y1 = 0;
	ext_stroke.x2 = UI_device_width - 1;
	ext_stroke.y2 = UI_device_height - 1;
	gEmPenPreviousPoint.x = -1;
	gEmPenPreviousPoint.y = -1;
	EntryNewScreen(EM_TOUCHSCREEN_TEST_SCR, ExitEmPenTest, EntryEmPenTest, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_TOUCHSCREEN_TEST_SCR);

	coolsand_UI_lock_double_buffer();
	ShowCategory400Screen(EM_TOUCHSCREEN_TEST_TEXT, 0,
							STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
							STR_GLOBAL_CLEAR, IMG_GLOBAL_CLEAR, guiBuffer);

	gEmPenStrokeArea.x1 = MMI_content_x + (MMI_content_width >> 3);
	gEmPenStrokeArea.x2 = MMI_content_x + MMI_content_width - (MMI_content_width >> 3);
	gEmPenStrokeArea.y1 = MMI_content_y + (MMI_content_height >> 3);
	gEmPenStrokeArea.y2 = MMI_content_y + MMI_content_height - (MMI_content_height >> 3);

	EmPenTestShowBorder();

	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);

	mmi_wgui_register_pen_down_handler(EmPenTestPenDown);
	mmi_wgui_register_pen_move_handler(EmPenTestPenMove);
	mmi_wgui_register_pen_up_handler(EmPenTestPenUp);
	mmi_wgui_register_pen_repeat_handler(EmPenTestPenRepeat);
	mmi_wgui_register_pen_long_tap_handler(EmPenTestPenLongTap);
	mmi_wgui_register_pen_abort_handler(EmPenTestPenAbort);

	gEmPenStrokeBuffer = (mmi_pen_point_struct*) OslMalloc(sizeof(mmi_pen_point_struct) * EM_PEN_STROKE_BUFFER_SIZE);
	mmi_pen_start_capture_strokes(EM_PEN_STROKE_BUFFER_SIZE, gEmPenStrokeBuffer, 1, &gEmPenStrokeArea, &ext_stroke);
	mmi_pen_register_stroke_down_handler(EmPenTestStrokeDown);
	mmi_pen_register_stroke_move_handler(EmPenTestStrokeMove);
	mmi_pen_register_stroke_up_handler(EmPenTestStrokeUp);
	mmi_pen_begin_strokes_of_character();
	
	SetLeftSoftkeyFunction(EntryEmPenOptions,KEY_EVENT_UP);
	SetRightSoftkeyFunction(EmPenTestClear,KEY_EVENT_UP);
}

 
void HighlightEmPenStatistics(void)
{
	SetLeftSoftkeyFunction(EntryEmPenStatisticsList,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


void HighlightEmPenExit(void)
{
	SetLeftSoftkeyFunction(EmPenLeaveTest,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


void HighlightEmPen(void)
{
	SetLeftSoftkeyFunction(EntryEmPenTest,KEY_EVENT_UP);
}

#endif /* __MMI_TOUCH_SCREEN__ */

void EmGPRSTest(void)
{
#ifdef __GPRS_MODE__
    u8 GprsState;
    u8 Uti = FREE_UTI;
    //add GPRS attach function
    #ifdef __MMI_MULTI_SIM__
	S8 SimId = MTPNP_AD_GET_UsableSide_Index();       
	if (SimId != -1)
	{
		CFW_GetGprsAttState(&GprsState, SimId);
		if (1 == GprsState)//attached, so process detach
		{
		    if (ERR_SUCCESS == CFW_GprsAtt( CFW_GPRS_DETACHED, Uti, SimId))
			{
				mmi_trace (1, "EmGPRSTest: Data Connection Detach req successfull") ;
				DisplayPopup( (U8*)GetString(STR_GLOBAL_DONE),  (U16) IMG_GLOBAL_OK, 0, EM_NOTIFY_DURATION, (U8)SUCCESS_TONE );
			}
			else
			{
				mmi_trace (1, "EmGPRSTest: Data Connection Detach req Error") ;
				DisplayPopup( (U8*)GetString(STR_GLOBAL_ERROR),  (U16) IMG_GLOBAL_ERROR, 0, EM_NOTIFY_DURATION, (U8)ERROR_TONE );
			}
		}
		else//not attached, so process attach
		{
		    if (ERR_SUCCESS == CFW_GprsAtt( CFW_GPRS_ATTACHED, Uti, SimId))
			{
				mmi_trace (1, "EmGPRSTest: Data Connection Attach req successfull") ;
				DisplayPopup( (U8*)GetString(STR_GLOBAL_DONE),  (U16) IMG_GLOBAL_OK, 0, EM_NOTIFY_DURATION, (U8)SUCCESS_TONE );
			}
			else
			{
				mmi_trace (1, "EmGPRSTest: Data Connection Attach req Error") ;
				DisplayPopup( (U8*)GetString(STR_GLOBAL_ERROR),  (U16) IMG_GLOBAL_ERROR, 0, EM_NOTIFY_DURATION, (U8)ERROR_TONE );
			}
		}	    
	}
	else
    {
        DisplayPopup( (U8*)GetString(STR_GLOBAL_ERROR),  (U16) IMG_GLOBAL_ERROR, 0, EM_NOTIFY_DURATION, (U8)ERROR_TONE );
    }
	#else
	#ifdef __ADP_MULTI_SIM__
	CFW_GetGprsAttState(&GprsState, 0);
	#else
	CFW_GetGprsAttState(&GprsState);
	#endif
	if (1 == GprsState)//attached, so process detach
	{
		#ifdef __ADP_MULTI_SIM__
		if (ERR_SUCCESS == CFW_GprsAtt( CFW_GPRS_DETACHED, Uti, 0))
		#else
		if (ERR_SUCCESS == CFW_GprsAtt( CFW_GPRS_DETACHED, Uti))
		#endif
		{
			mmi_trace (1, "EmGPRSTest: Data Connection Detach req successfull") ;
			DisplayPopup( (U8*)GetString(STR_GLOBAL_DONE),  (U16) IMG_GLOBAL_OK, 0, EM_NOTIFY_DURATION, (U8)SUCCESS_TONE );
		}
		else
		{
			mmi_trace (1, "EmGPRSTest: Data Connection Detach req Error") ;
			DisplayPopup( (U8*)GetString(STR_GLOBAL_ERROR),  (U16) IMG_GLOBAL_ERROR, 0, EM_NOTIFY_DURATION, (U8)ERROR_TONE );
		}
	}
	else//not attached, so process attach
	{
		#ifdef __ADP_MULTI_SIM__
		if (ERR_SUCCESS == CFW_GprsAtt( CFW_GPRS_ATTACHED, Uti, 0))
		#else
		if (ERR_SUCCESS == CFW_GprsAtt( CFW_GPRS_ATTACHED, Uti))
		#endif
		{
			mmi_trace (1, "EmGPRSTest: Data Connection Attach req successfull") ;
			DisplayPopup( (U8*)GetString(STR_GLOBAL_DONE),  (U16) IMG_GLOBAL_OK, 0, EM_NOTIFY_DURATION, (U8)SUCCESS_TONE );
		}
		else
		{
			mmi_trace (1, "EmGPRSTest: Data Connection Attach req Error") ;
			DisplayPopup( (U8*)GetString(STR_GLOBAL_ERROR),  (U16) IMG_GLOBAL_ERROR, 0, EM_NOTIFY_DURATION, (U8)ERROR_TONE );
		}
	}
	#endif
	
#else
    U8 StrBuf[40];
    EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

    memset( StrBuf, 0, sizeof(StrBuf));
    AnsiiToUnicodeString((PS8)StrBuf, "GPRS Not Support!" );
    EngineerModeNoticeText( StrBuf, IMG_GLOBAL_ERROR);
#endif

}
void HighlightGPRSTest(void)
{
#ifdef __GPRS_MODE__
    u8 GprsState = 0;
	
    #ifdef __MMI_MULTI_SIM__
    S8 SimId = MTPNP_AD_GET_UsableSide_Index();
    if (-1 != SimId)
    {
       CFW_GetGprsAttState(&GprsState, SimId);
    }
    #else
	#ifdef __ADP_MULTI_SIM__
	CFW_GetGprsAttState(&GprsState, 0);
	#else
    CFW_GetGprsAttState(&GprsState);
	#endif
    #endif
	
    if(1 == GprsState)//test has been started, the left soft key will be used for detach
	ChangeLeftSoftkey(STR_GLOBAL_ABORT, NULL);
    else//test has not been started, the left soft key will be used for attach
	ChangeLeftSoftkey(STR_GLOBAL_OK, NULL);
#endif

    SetLeftSoftkeyFunction(EmGPRSTest,KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
/**************************************************************
**	FUNCTION NAME		: EntryEmAutoAnswer
**
**  	PURPOSE				: Entry function for Auto Answer menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmAutoAnswer(void)
{

   S16 error=0;
   U16 i=1;

   //EntryNewScreen(EM_AUTO_ANSWER_MENUID, NULL, EntryEmAutoAnswer, NULL);
   
   for(i=0; i<4 ;i++)
      gprofiles[i].answeringMode.automatic = 1-gprofiles[i].answeringMode.automatic;

#if (defined(MMI_ON_HARDWARE_P) && !defined(WIN32) )
	if(WriteRecord (NVRAM_EF_PROFILES_LID, 1, &gprofiles[0], NVRAM_PROFILES_RECORD_SIZE,
						& error)<NVRAM_PROFILES_RECORD_SIZE)
	{
              EngineerModeErrorNotice(EM_AUD_SET_ERROR_TEXT);
	}
	if( gactivatedprofile == gprofileId)
	{
		memcpy(&gcurrentprofile,&gprofiles[gprofileId],sizeof(PROFILE));
	}
#endif	

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

   if (gprofiles[0].answeringMode.automatic == 1)
      Category52ChangeItemDescription(0, (U8*)get_string(STR_GLOBAL_ON));
   else
      Category52ChangeItemDescription(0, (U8*)get_string(STR_GLOBAL_OFF));

	RedrawCategoryFunction();
	HighlightEmAutoAnswer();
}


/**************************************************************
**	FUNCTION NAME		: EntryEmTVOut
**
** PURPOSE				: Entry function for TV Out menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
#ifdef __MMI_TVOUT__
void EntryEmTVOut(void)
{
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

   if(gIsTVOut)
   {
      /* turn off */
      mmi_phnset_tvout_event_hdlr(MDI_RES_TV_DISABLE);
      gIsTVOut = FALSE;
   }
   else
   {
      /* turn on */
      mmi_phnset_tvout_event_hdlr(MDI_RES_TV_ENABLE);
      gIsTVOut = TRUE;      
   }

   if(gIsTVOut)
      Category52ChangeItemDescription(0, (U8*)get_string(STR_GLOBAL_ON));
   else
      Category52ChangeItemDescription(0, (U8*)get_string(STR_GLOBAL_OFF));

	RedrawCategoryFunction();
	HighlightEmTVOut();
}
#endif /* __MMI_TVOUT__ */


/**************************************************************
**	FUNCTION NAME		: EntryVideoHiBitrate
**
** PURPOSE				: Entry function for High bitrate video
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
#if defined(__MMI_VIDEO__) && defined(ISP_SUPPORT)
void EntryVideoHiBitrate(void)
{
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

	#ifdef MMI_ON_HARDWARE_P
		{
			U8 mode;
			
			mode = media_vid_get_em_mode();

			if(mode == VID_EM_MODE_DISABLED)
			{
				media_vid_set_em_mode(VID_EM_MODE_ULTRA_HIGH_BITRATE);
				Category52ChangeItemDescription(0, (U8*)get_string(STR_GLOBAL_ON));
			}
			else if(mode == VID_EM_MODE_ULTRA_HIGH_BITRATE)
			{
				media_vid_set_em_mode(VID_EM_MODE_DISABLED);
				Category52ChangeItemDescription(0, (U8*)get_string(STR_GLOBAL_OFF));		
			}
			else
			{
				ASSERT(0);
			}
		}
	#endif /* MMI_ON_HARDWARE_P */
	
	RedrawCategoryFunction();
	HighlightVideoHiBitrate();
}
#endif /* __MMI_VIDEO__ && ISP_SUPPORT */



/**************************************************************
**	FUNCTION NAME		: EntryEmHighSpeedSIM
**
**  	PURPOSE				: Entry function for High Speed SIM menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmHighSpeedSIM(void)
{
#if (defined(MMI_ON_HARDWARE_P) && !defined(WIN32) )
   if (High_Speed_SIM) 
   	High_Speed_SIM = KAL_FALSE;
   else
   	High_Speed_SIM = KAL_TRUE;
   L1sim_Enable_Enhanced_Speed(High_Speed_SIM);
   EngineerModeSetHighSpeedSIMReq();
#endif	

   SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

#ifdef MMI_ON_HARDWARE_P
   if (High_Speed_SIM)
      Category52ChangeItemDescription(1, (U8*)get_string(STR_GLOBAL_ON));
   else
      Category52ChangeItemDescription(1, (U8*)get_string(STR_GLOBAL_OFF));
#endif
   RedrawCategoryFunction();
   HighlightEmHighSpeedSIM();
}


/**************************************************************
**	FUNCTION NAME		: EntryEmWAP
**
**  	PURPOSE				: Entry function for WAP menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmWAP(void)
{
	U16		ItemList[32];
	U16		ItemIcons[32];
	U16		nItems;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;


	EntryNewScreen(EM_WAP_MENU_SCR, NULL, EntryEmWAP, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_WAP_MENU_SCR);
	
	nItems = GetNumOfChild(EM_WAP_MENUID);
	GetSequenceStringIds(EM_WAP_MENUID,ItemList);
	SetParentHandler(EM_WAP_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

#if defined(WAP_SUPPORT)
       if (wap_enable_debug_assert)
         PopUpList[0] = (U8*)get_string(STR_GLOBAL_ON);
       else
         PopUpList[0] = (U8*)get_string(STR_GLOBAL_OFF);

       if (global_offline_page)
         PopUpList[4] = (U8*)get_string(STR_GLOBAL_ON);
       else
         PopUpList[4] = (U8*)get_string(STR_GLOBAL_OFF);

#endif

	ShowCategory52Screen(EM_WAP_TEXT,0,
							STR_GLOBAL_OK,0,
							STR_GLOBAL_BACK,0,
							nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: EntryEmWAPEnableDebug
**
**  	PURPOSE				: Entry function for WAP Enable Debug menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmWAPEnableDebug(void)
{
#if defined(WAP_SUPPORT) || defined(MMI_ON_WIN32)
   wap_enable_debug_assert = 1 - wap_enable_debug_assert;
#endif

   SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

   ReplaceNewScreenHandler(EM_WAP_ENABLE_DEBUG_MENU_SCR, ExitEmWAPEnableDebug, NULL);

#if defined(WAP_SUPPORT) || defined(MMI_ON_WIN32)
   if (wap_enable_debug_assert)
      Category52ChangeItemDescription(0, (U8*)get_string(STR_GLOBAL_ON));
   else
      Category52ChangeItemDescription(0, (U8*)get_string(STR_GLOBAL_OFF));
#endif

   RedrawCategoryFunction();
   HighlightEmWAPEnableDebug();
}


/**************************************************************
**	FUNCTION NAME		: ExitEmWAPEnableDebug
**
**  	PURPOSE				: Exit function for WAP Enable Debug menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitEmWAPEnableDebug(void)
{
	history_t    currHistory;
	S16		nHistory	=	0;

	currHistory.scrnID = EM_WAP_ENABLE_DEBUG_MENU_SCR;
	currHistory.entryFuncPtr = EntryEmWAPEnableDebug;
	pfnUnicodeStrcpy ((S8*)currHistory.inputBuffer, (S8*)&nHistory);
	GetCategoryHistory(currHistory.guiBuffer);
	AddHistory(currHistory);
}

/**************************************************************
**	FUNCTION NAME		: EntryEmOfflinePage
**
**  	PURPOSE				: Entry function for WAP Offline Page menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmOfflinePage(void)
{
#if defined(WAP_SUPPORT) || defined(MMI_ON_WIN32)
   global_offline_page = 1 - global_offline_page;
#if defined(WAP_SUPPORT)
   em_wap_change_setting(EM_WAP_OFFLINE_PAGE_OPT, global_offline_page);
#endif
#endif

   SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

   ReplaceNewScreenHandler(EM_WAP_OFFLINE_PAGE_MENU_SCR, ExitEmOfflinePage, NULL);

#if defined(WAP_SUPPORT) || defined(MMI_ON_WIN32)
   if (global_offline_page)
      Category52ChangeItemDescription(4, (U8*)get_string(STR_GLOBAL_ON));
   else
      Category52ChangeItemDescription(4, (U8*)get_string(STR_GLOBAL_OFF));
#endif

   RedrawCategoryFunction();
   HighlightEmWAPOfflinePage();
}

/**************************************************************
**	FUNCTION NAME		: ExitEmOfflinePage
**
**  	PURPOSE				: Exit function for WAP Offline Page menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitEmOfflinePage(void)
{
	history_t    currHistory;
	S16		nHistory	=	0;

	currHistory.scrnID = EM_WAP_OFFLINE_PAGE_MENU_SCR;
	currHistory.entryFuncPtr = EntryEmOfflinePage;
	pfnUnicodeStrcpy ((S8*)currHistory.inputBuffer, (S8*)&nHistory);
	GetCategoryHistory(currHistory.guiBuffer);
	AddHistory(currHistory);
}

/**************************************************************
**	FUNCTION NAME		: SendUserAgentReq
**
**  	PURPOSE				: Function for sending User Agent request
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SendUserAgentReq(void)
{
#if defined(WAP_SUPPORT)
	global_wap_user_agent = currentHighlightIndex;
	em_wap_change_setting(EM_WAP_USERAGENT_OPT, global_wap_user_agent);
#endif
	DisplayPopup( (U8*)GetString(STR_GLOBAL_DONE),  (U16) IMG_GLOBAL_OK, 0, EM_NOTIFY_DURATION, (U8)SUCCESS_TONE );
}


/**************************************************************
**	FUNCTION NAME		: EmAssignUserAgentMenuItem
**
**  	PURPOSE				: Helper function for assigning User Agent items
**
**	INPUT PARAMETERS	: nStrItemList, nNumofItem
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmAssignUserAgentMenuItem( U16 *nStrItemList, U8 *nNumofItem )
{
    U8 idx=0;
	
    if (nStrItemList)
    {
	nStrItemList[idx] = EM_WAP_USER_AGENT_MAUI_E800_TEXT;
#if defined(WAP_SUPPORT)
	if (idx >= EM_WAP_USERAGENT_MAX) return;
#endif
        idx++;

	nStrItemList[idx] = EM_WAP_USER_AGENT_SONY_P800_TEXT;
#if defined(WAP_SUPPORT)
	if (idx >= EM_WAP_USERAGENT_MAX) return;
#endif
        idx++;
       
	nStrItemList[idx] = EM_WAP_USER_AGENT_NOKIA_7250i_TEXT;
#if defined(WAP_SUPPORT)
	if (idx >= EM_WAP_USERAGENT_MAX) return;
#endif
        idx++;

	nStrItemList[idx] = EM_WAP_USER_AGENT_SHARP_GX21_TEXT;
#if defined(WAP_SUPPORT)
	if (idx >= EM_WAP_USERAGENT_MAX) return;
#endif
        idx++;

	nStrItemList[idx] = EM_WAP_USER_AGENT_SONY_T610_TEXT;
#if defined(WAP_SUPPORT)
	if (idx >= EM_WAP_USERAGENT_MAX) return;
#endif
        idx++;

	nStrItemList[idx] = EM_WAP_USER_AGENT_SAMSUNG_TEXT;
#if defined(WAP_SUPPORT)
	if (idx >= EM_WAP_USERAGENT_MAX) return;
#endif
        idx++;

	nStrItemList[idx] = EM_WAP_USER_AGENT_NOKIA_6230_TEXT;
#if defined(WAP_SUPPORT)
	if (idx >= EM_WAP_USERAGENT_MAX) return;
#endif
        idx++;

	nStrItemList[idx] = EM_WAP_USER_AGENT_MOTO_V303_TEXT;
#if defined(WAP_SUPPORT)
	if (idx >= EM_WAP_USERAGENT_MAX) return;
#endif
        idx++;
    }

    if (nNumofItem )
	*nNumofItem = idx;
}


/**************************************************************
**	FUNCTION NAME		: EntryEmUserAgent
**
**  	PURPOSE				: Entry function for User Agent menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmUserAgent(void)
{
#if defined(WAP_SUPPORT)
   U16 nStrItemList[EM_WAP_USERAGENT_MAX];
#else
   U16 nStrItemList[6];   
#endif
   U8 nNumofItem; 
   U8* guiBuffer;

   EmAssignUserAgentMenuItem( nStrItemList, &nNumofItem );

#if defined(WAP_SUPPORT)
   currentHighlightIndex=
   	(global_wap_user_agent < EM_WAP_USERAGENT_MAX)
   	? global_wap_user_agent 
   	: EM_WAP_USERAGENT_MAUI_E800;
#endif   
   
   EntryNewScreen(EM_WAP_USER_AGENT_MENU_SCR, NULL, EntryEmUserAgent, NULL);

   guiBuffer = GetCurrGuiBuffer (EM_WAP_USER_AGENT_MENU_SCR);
	
   RegisterHighlightHandler(SetHighlightIndex);

   ShowCategory11Screen (EM_WAP_USER_AGENT_TEXT, 0,
						  STR_GLOBAL_OK, 0,
						  STR_GLOBAL_BACK, 0,
						  nNumofItem, nStrItemList, currentHighlightIndex, guiBuffer);

						  
    SetLeftSoftkeyFunction(SendUserAgentReq,KEY_EVENT_UP);

    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: SendAcceptHeaderReq
**
**  	PURPOSE				: Function for sending Accept Header Request
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SendAcceptHeaderReq(void)
{
#if defined(WAP_SUPPORT)
	global_wap_accept_header = currentHighlightIndex;
	em_wap_change_setting(EM_WAP_ACCEPTHEADER_OPT, global_wap_accept_header);
#endif
	DisplayPopup( (U8*)GetString(STR_GLOBAL_DONE),  (U16) IMG_GLOBAL_OK, 0, EM_NOTIFY_DURATION, (U8)SUCCESS_TONE );
}


/**************************************************************
**	FUNCTION NAME		: SendAcceptHeaderReq
**
**  	PURPOSE				: Helper function for assigning Accept Header menu items
**
**	INPUT PARAMETERS	: nStrItemList, nNumofItem
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmAssignAcceptHeaderMenuItem( U16 *nStrItemList, U8 *nNumofItem )
{
    U8 idx=0;
	
    if (nStrItemList)
    {
	nStrItemList[idx] = EM_WAP_ACCEPT_HEADER_WAP121_TEXT;
#if defined(WAP_SUPPORT)
	if (idx >= EM_WAP_ACCEPTHEADER_MAX) return;
#endif
        idx++;

	nStrItemList[idx] = EM_WAP_ACCEPT_HEADER_WAP20_TEXT;
#if defined(WAP_SUPPORT)
	if (idx >= EM_WAP_ACCEPTHEADER_MAX) return;
#endif
        idx++;
       
	nStrItemList[idx] = EM_WAP_ACCEPT_HEADER_WML_TEXT;
#if defined(WAP_SUPPORT)
	if (idx >= EM_WAP_ACCEPTHEADER_MAX) return;
#endif
        idx++;
       
	nStrItemList[idx] = EM_WAP_ACCEPT_HEADER_XHTML_TEXT;
#if defined(WAP_SUPPORT)
	if (idx >= EM_WAP_ACCEPTHEADER_MAX) return;
#endif
        idx++;

	nStrItemList[idx] = EM_WAP_ACCEPT_HEADER_HTML_TEXT;
#if defined(WAP_SUPPORT)
	if (idx >= EM_WAP_ACCEPTHEADER_MAX) return;
#endif
        idx++;
    }

    if (nNumofItem )
	*nNumofItem = idx;
}


/**************************************************************
**	FUNCTION NAME		: EntryEmAcceptHeader
**
**  	PURPOSE				: Entry function for Accept Header menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmAcceptHeader(void)
{
#if defined(WAP_SUPPORT)
   U16 nStrItemList[EM_WAP_ACCEPTHEADER_MAX];
#else
   U16 nStrItemList[5];
#endif
   U8 nNumofItem; 
   U8* guiBuffer;

   EmAssignAcceptHeaderMenuItem( nStrItemList, &nNumofItem );

#if defined(WAP_SUPPORT)
   currentHighlightIndex = 
   	(global_wap_accept_header < EM_WAP_ACCEPTHEADER_MAX)
   	? global_wap_accept_header 
   	: EM_WAP_ACCEPTHEADER_WAP121;
#endif
   
   EntryNewScreen(EM_WAP_ACCEPT_HEADER_MENU_SCR, NULL, EntryEmAcceptHeader, NULL);
   guiBuffer = GetCurrGuiBuffer (EM_WAP_ACCEPT_HEADER_MENU_SCR);
	
   RegisterHighlightHandler(SetHighlightIndex);

   ShowCategory11Screen (EM_WAP_ACCEPT_HEADER_TEXT, 0,
						  STR_GLOBAL_OK, 0,
						  STR_GLOBAL_BACK, 0,
						  nNumofItem, nStrItemList, currentHighlightIndex, guiBuffer);

						  
    SetLeftSoftkeyFunction(SendAcceptHeaderReq,KEY_EVENT_UP);

    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}

//Lisen 05032005     Begin
#if (defined(MT6229) || defined(MT6227) || defined(MT6228)) 
/**************************************************************
**	FUNCTION NAME		: HighlightEMSWDBG
**
**  	PURPOSE				: Highlight function for SWDBG menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEMSWDBG(void)
{
       ChangeLeftSoftkey(STR_GLOBAL_OK,0);
       
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmSWDBG,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

//Lisen 05032005
#ifdef 	MMI_ON_HARDWARE_P
extern kal_uint8 SWDBG_Profile;
#endif 

/**************************************************************
**	FUNCTION NAME		: SetSWDBG
**
**  	PURPOSE				: Function for setting SWDBG
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SetSWDBG(void)
{
#ifdef 	MMI_ON_HARDWARE_P
	SWDBG_Profile = currentHighlightIndex + 1; //SWDBG is 1-based
#endif
	// Set in NVRAM
       EngineerModeSetUartReq();

	DisplayPopup( (U8*)GetString(STR_GLOBAL_DONE),  (U16) IMG_GLOBAL_OK, 0, EM_NOTIFY_DURATION, (U8)SUCCESS_TONE );
}

/**************************************************************
**	FUNCTION NAME		: EmAssignSWDBGMenuItem
**
**  	PURPOSE				: Helper function for assigning SWDBG menu items
**
**	INPUT PARAMETERS	: nStrItemList, nNumofItem
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmAssignSWDBGMenuItem( U16 *nStrItemList, U8 *nNumofItem )
{
#if  (defined(MT6227) || defined(MT6228))
    if (nStrItemList)
    {
	nStrItemList[0] = EM_SWDBG_OFF_TEXT;
	nStrItemList[1] = EM_SWDBG_DSP_L1_PS_TEXT;
	nStrItemList[2] = EM_SWDBG_L1_PS_TEXT;
	nStrItemList[3] = EM_SWDBG_MCU_TEXT;
    }

    if (nNumofItem )
	*nNumofItem = EM_SWDBG_MAX;

#else

    if (nStrItemList)
    {
	nStrItemList[0] = EM_SWDBG_OFF_TEXT;
	nStrItemList[1] = EM_SWDBG_DSP_L1_TEXT;
	nStrItemList[2] = EM_SWDBG_DSP_PS_TEXT;
	nStrItemList[3] = EM_SWDBG_L1_PS_TEXT;
	nStrItemList[4] = EM_SWDBG_MCU_TEXT;
    }

    if (nNumofItem )
	*nNumofItem = EM_SWDBG_MAX;

#endif
}


/**************************************************************
**	FUNCTION NAME		: EntryEmSWDBG
**
**  	PURPOSE				: Entry function for SW debug end point setting
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmSWDBG(void)
{
   U16 nStrItemList[EM_SWDBG_MAX];
   U8 nNumofItem; 
   U8* guiBuffer;

   EmAssignSWDBGMenuItem( nStrItemList, &nNumofItem );

#ifdef 	MMI_ON_HARDWARE_P
   currentHighlightIndex = SWDBG_Profile - 1;  //SWDBG is 1-based
#else
   currentHighlightIndex = 0;
#endif    
   EntryNewScreen(EM_SET_SWDBG_SCR, NULL, EntryEmSWDBG, NULL);
   guiBuffer = GetCurrGuiBuffer (EM_SET_SWDBG_SCR);
	
   RegisterHighlightHandler(SetHighlightIndex);

   ShowCategory11Screen (EM_SWDBG_TEXT, 0,
						  STR_GLOBAL_OK, 0,
						  STR_GLOBAL_BACK, 0,
						  nNumofItem, nStrItemList, currentHighlightIndex, guiBuffer);

						  
    SetLeftSoftkeyFunction(SetSWDBG, KEY_EVENT_UP);

    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

}
#endif
//Lisen 05032005     End

/**************************************************************
**	FUNCTION NAME		: SendMMSVersionReq
**
**  	PURPOSE				: Function for sending MMS Version request
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SendMMSVersionReq(void)
{
#if defined(WAP_SUPPORT)
	global_mms_version = currentHighlightIndex;
	em_wap_change_setting(EM_MMS_VERSION_OPT, global_mms_version);
#endif
	DisplayPopup( (U8*)GetString(STR_GLOBAL_DONE),  (U16) IMG_GLOBAL_OK, 0, EM_NOTIFY_DURATION, (U8)SUCCESS_TONE );
}


/**************************************************************
**	FUNCTION NAME		: EmAssignMMSVersionMenuItem
**
**  	PURPOSE				: Helper function for assigning MMS Version menu items
**
**	INPUT PARAMETERS	: nStrItemList, nNumofItem
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmAssignMMSVersionMenuItem( U16 *nStrItemList, U8 *nNumofItem )
{
    U8 idx=0;
	
    if (nStrItemList)
    {
	nStrItemList[idx] = EM_WAP_MMS_VERSION_MMS_DEFAULT_TEXT;
#if defined(WAP_SUPPORT)
	if (idx >= EM_MMS_VERSION_MAX) return;
#endif
        idx++;

	nStrItemList[idx] = EM_WAP_MMS_VERSION_MMS10_TEXT;
#if defined(WAP_SUPPORT)
	if (idx >= EM_MMS_VERSION_MAX) return;
#endif 
        idx++;

	nStrItemList[idx] = EM_WAP_MMS_VERSION_MMS11_TEXT;
#if defined(WAP_SUPPORT)
	if (idx >= EM_MMS_VERSION_MAX) return;
#endif
        idx++;
	nStrItemList[idx] = EM_WAP_MMS_VERSION_MMS12_TEXT;
#if defined(WAP_SUPPORT)
	if (idx >= EM_MMS_VERSION_MAX) return;
#endif
        idx++;
    }

    if (nNumofItem )
	*nNumofItem = idx;
}


/**************************************************************
**	FUNCTION NAME		: EntryEMMMSVersion
**
**  	PURPOSE				: Entry function for MMSVersion menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEMMMSVersion(void)
{
#if defined(WAP_SUPPORT)
   U16 nStrItemList[EM_MMS_VERSION_MAX];
#else
   U16 nStrItemList[3];
#endif
   U8 nNumofItem; 
   U8* guiBuffer;

   EmAssignMMSVersionMenuItem( nStrItemList, &nNumofItem );
 
#if defined(WAP_SUPPORT)
   currentHighlightIndex = 
   	(global_mms_version < EM_MMS_VERSION_MAX )
   	? global_mms_version 
   	: EM_MMS_VERSION_10;
#endif
   
   EntryNewScreen(EM_WAP_MMS_VERSION_MENU_SCR, NULL, EntryEMMMSVersion, NULL);
   guiBuffer = GetCurrGuiBuffer (EM_WAP_MMS_VERSION_MENU_SCR);
	
   RegisterHighlightHandler(SetHighlightIndex);

   ShowCategory11Screen (EM_WAP_MMS_VERSION_TEXT, 0,
						  STR_GLOBAL_OK, 0,
						  STR_GLOBAL_BACK, 0,
						  nNumofItem, nStrItemList, currentHighlightIndex, guiBuffer);

						  
    SetLeftSoftkeyFunction(SendMMSVersionReq,KEY_EVENT_UP);

    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: HighlightEmAutoAnswer
**
**  	PURPOSE				: Highlight function for AutoAnswer menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmAutoAnswer(void)
{

   if (gprofiles[0].answeringMode.automatic == 1)
      ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
   else
      ChangeLeftSoftkey(STR_GLOBAL_ON,0);
   
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmAutoAnswer,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmTVOut
**
** PURPOSE				: Highlight function TV Out
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
#ifdef __MMI_TVOUT__
void HighlightEmTVOut(void)
{
   if(gIsTVOut)
      ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
   else
      ChangeLeftSoftkey(STR_GLOBAL_ON,0);
   
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmTVOut,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
#endif /* __MMI_TVOUT__ */


/**************************************************************
**	FUNCTION NAME		: HighlightVideoHiBitrate
**
** PURPOSE				: Highlight function for Video high bitrate
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
#if defined(__MMI_VIDEO__) && defined(ISP_SUPPORT)
void HighlightVideoHiBitrate(void)
{

	#ifdef MMI_ON_HARDWARE_P
		{
			U8 mode;
			
			mode = media_vid_get_em_mode();

			if(mode == VID_EM_MODE_DISABLED)
			{
				ChangeLeftSoftkey(STR_GLOBAL_ON,0);
			}
			else if(mode == VID_EM_MODE_ULTRA_HIGH_BITRATE)
			{
				ChangeLeftSoftkey(STR_GLOBAL_OFF,0);		
			}
			else
			{
				ASSERT(0);
			}
		}
	#endif /* MMI_ON_HARDWARE_P */
   
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryVideoHiBitrate,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
#endif /* __MMI_VIDEO__ && ISP_SUPPORT */



/**************************************************************
**	FUNCTION NAME		: HighlightEmHighSpeedSIM
**
**  	PURPOSE				: Highlight function for High Speed SIM menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmHighSpeedSIM(void)
{
#ifdef MMI_ON_HARDWARE_P
   if (High_Speed_SIM)
      ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
   else
      ChangeLeftSoftkey(STR_GLOBAL_ON,0);
#endif      
   
    /* Register function for left soft key */
    SetLeftSoftkeyFunction(EntryEmHighSpeedSIM,KEY_EVENT_UP);

    /* Register function for right soft key */
    SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmMiscBacklight
**
**  	PURPOSE				: Highlight function for Misc Backlight menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmMiscBacklight(void)
{
   if (  backlight_mode == BACKLIGHT_NORMAL )
      ChangeLeftSoftkey(STR_GLOBAL_ON,0);
   else
      ChangeLeftSoftkey(STR_GLOBAL_OFF,0);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmMiscBacklight,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}


/**************************************************************
**	FUNCTION NAME		: HighlightEmSerialPortEnable
**
**  	PURPOSE				: Highlight function for SerialPortEnable menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmSerialPortEnable(void)
{
      ChangeLeftSoftkey(STR_GLOBAL_OK,0);
      ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmSerialPortEnable,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmAutoReset
**
**  	PURPOSE				: Highlight function for Auto Reset menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmAutoReset(void)
{
      ChangeLeftSoftkey(STR_GLOBAL_OK,0);
      ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmAutoReset,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

#if defined(__AMR_SUPPORT__)
/**************************************************************
**	FUNCTION NAME		: HighlightEmAMR
**
**  	PURPOSE				: Highlight function for AMR setting
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmAMR(void)
{
      ChangeRightSoftkey(STR_GLOBAL_BACK,0);

      if (AMR_ON)
         ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
      else
         ChangeLeftSoftkey(STR_GLOBAL_ON,0);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEMFAMR, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************
**	FUNCTION NAME		: EntryEMFAMR
**
**  	PURPOSE				: Entry function for AMR menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEMFAMR(void)
{
   S16     Ret, ErrorCode;

   if (AMR_ON == FALSE)
   {
      mscap[0] = 0x1f;
      Ret = WriteRecord( NVRAM_EF_MSCAP_LID, 1, &mscap, NVRAM_EF_MSCAP_SIZE, &ErrorCode);
      if (!(ErrorCode == NVRAM_WRITE_SUCCESS && Ret == NVRAM_EF_MSCAP_SIZE))
      {
          ASSERT(0);              
      }
      
      AMR_ON = TRUE;
      Category52ChangeItemDescription(6, (U8*)get_string(STR_GLOBAL_ON));      
   }
   else
   {
#if !defined(MT6205)   
      mscap[0] = 0x07;
#else
      mscap[0] = 0x05;
#endif
      Ret = WriteRecord( NVRAM_EF_MSCAP_LID, 1, &mscap, NVRAM_EF_MSCAP_SIZE, &ErrorCode);
      if (!(ErrorCode == NVRAM_WRITE_SUCCESS && Ret == NVRAM_EF_MSCAP_SIZE))
      {
          ASSERT(0);              
      }

      AMR_ON = FALSE;
      Category52ChangeItemDescription(6, (U8*)get_string(STR_GLOBAL_OFF));
   }
   
   SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

   RedrawCategoryFunction();
   HighlightEmAMR();
}
#endif                          

/**************************************************************
**	FUNCTION NAME		: HighlightEMWAP
**
**  	PURPOSE				: Highlight function for WAP menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEMWAP(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmWAP,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmWAPEnableDebug
**
**  	PURPOSE				: Highlight function for WAP Enable Debug menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmWAPEnableDebug(void)
{
#if defined(WAP_SUPPORT) || defined(MMI_ON_WIN32)
      if (wap_enable_debug_assert)
         ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
      else
         ChangeLeftSoftkey(STR_GLOBAL_ON,0);
#endif

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmWAPEnableDebug,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

//Lisen 012605
/**************************************************************
**	FUNCTION NAME		: HighlightEmMMIDebug
**
**  	PURPOSE				: Highlight function for MMI Debug menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmMMIDebug(void)
{
#ifdef DEBUG_ASSERT
      if (g_mmi_frm_enable_debug_assert)
         ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
      else
         ChangeLeftSoftkey(STR_GLOBAL_ON,0);
#endif

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmMMIDebug,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************
**	FUNCTION NAME		: HighlightEmMemoryDump
**
**  	PURPOSE				: Highlight function for Memory Dump menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmMemoryDump(void)
{
//Lisen 04072005
      if (INT_MemoryDumpFlag == EX_FORCEMEMORYDUMP)
         ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
      else
         ChangeLeftSoftkey(STR_GLOBAL_ON,0);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmMemoryDump,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************
**	FUNCTION NAME		: HighlightEmWAPOfflinePage
**
**  	PURPOSE				: Highlight function for WAP Offline Page menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmWAPOfflinePage(void)
{
#if defined(WAP_SUPPORT) || defined(MMI_ON_WIN32)
      if (global_offline_page)
         ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
      else
         ChangeLeftSoftkey(STR_GLOBAL_ON,0);
#endif

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmOfflinePage,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************
**	FUNCTION NAME		: HighlightEMUserAgent
**
**  	PURPOSE				: Highlight function for User Agent menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEMUserAgent(void)
{
       ChangeLeftSoftkey(STR_GLOBAL_OK,0);
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmUserAgent,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEMAcceptHeader
**
**  	PURPOSE				: Highlight function for Accept Header menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEMAcceptHeader(void)
{
       ChangeLeftSoftkey(STR_GLOBAL_OK,0);
       
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmAcceptHeader,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEMMMSVersion
**
**  	PURPOSE				: Highlight function for MMS Version menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEMMMSVersion(void)
{
       ChangeLeftSoftkey(STR_GLOBAL_OK,0);
       
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEMMMSVersion,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: GetSelectedTestList
**
**  	PURPOSE				: Function to get the list tem count
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: List item count
**
 
**************************************************************/
U16 GetSelectedTestList(void)
{
	U16 count, testitem;
	U16 index=0;

	ReadSettingsFromNVRAM();

	if (gAutoTestListSize > MAX_AUTO_TEST_ITEMS) 
	  gAutoTestListSize = MAX_AUTO_TEST_ITEMS;
	
	for (count = 0; count < MAX_AUTO_TEST_ITEMS; count++)
		AutoTestSelectedItems[count] = NULL;
	
	for (count = 0 ; count< gAutoTestListSize; count++, index++) 
	{   
	       testitem = nvramTestItemArray.priority[count];
//		AutoTestSelectedItems[count] = (PU8) AutoTestItemList[testitem];
              AutoTestSelectedItems[count] = (PU8) Tests[testitem].name;
	}
	return count;
}


/**************************************************************
**	FUNCTION NAME		: ReadTestResultFromNVRAM
**
**  	PURPOSE				: Function to read auto test result from NVRAM
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: The result will be stored in the global variable
**
**	RETURNS				: void
**
 
**************************************************************/
void ReadTestResultFromNVRAM(void)
{
	U8       i;

#if ( defined(MMI_ON_HARDWARE_P) && !defined(WIN32) )
       nvram_ef_autotest_struct testdata; 
	S16     ErrorCode, Ret;

       memset( &testdata, 0, sizeof(testdata) );
       Ret = ReadRecord( NVRAM_EF_AUTOTEST_LID, 2, &testdata, NVRAM_EF_AUTOTEST_SIZE, &ErrorCode);
       if (!(ErrorCode == NVRAM_READ_SUCCESS && Ret == NVRAM_EF_AUTOTEST_SIZE))
       {
           ASSERT(0);              
       }

       nvramTestResultArray.count = testdata.testcount;

	for (i=0; i<testdata.testcount; i++)
	{
	    nvramTestResultArray.result[i] = (testdata.testlist[i] < FM_TEST_PASSED) ? testdata.testlist[i] : FM_TEST_PASSED;
	}
       //asm("break 1");
#else

       {
	   nvramTestResultArray.count = MAX_AUTO_TEST_ITEMS;

	   for (i=0; i<MAX_AUTO_TEST_ITEMS; i++)
	   {
	      nvramTestResultArray.result[i] = FM_TEST_UNTESTED;  
	   }
       }
#endif
}


/**************************************************************
**	FUNCTION NAME		: WriteTestResultToNVRAM
**
**  	PURPOSE				: Function to write auto test result to NVRAM
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void WriteTestResultToNVRAM(void)
{
#if ( defined(MMI_ON_HARDWARE_P) && !defined(WIN32) )
   	   U8       i;
          nvram_ef_autotest_struct testdata; 
	   S16     ErrorCode, Ret;

          memset( &testdata, 0, sizeof(testdata) );

          testdata.testcount = gAutoTestListSize; 
          for (i=0; i<testdata.testcount; i++)
          {
	      testdata.testlist[i] = nvramTestResultArray.result[i];
          }
       
          Ret = WriteRecord( NVRAM_EF_AUTOTEST_LID, 2, &testdata, NVRAM_EF_AUTOTEST_SIZE, &ErrorCode);
          if (!(ErrorCode == NVRAM_WRITE_SUCCESS && Ret == NVRAM_EF_AUTOTEST_SIZE))
          {
              ASSERT(0);              
          }
#endif
}


/**************************************************************
**	FUNCTION NAME		: ReadTimeFromNVRAM
**
**  	PURPOSE				: Function to read auto test result time from NVRAM
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ReadTimeFromNVRAM(void)
{
#if ( defined(MMI_ON_HARDWARE_P) && !defined(WIN32) )
       nvram_ef_autotest_struct testdata; 
	S16     ErrorCode, Ret;

       memset( &testdata, 0, sizeof(testdata) );
       Ret = ReadRecord( NVRAM_EF_AUTOTEST_LID, 1, &testdata, NVRAM_EF_AUTOTEST_SIZE, &ErrorCode);
       if (!(ErrorCode == NVRAM_READ_SUCCESS && Ret == NVRAM_EF_AUTOTEST_SIZE))
       {
           ASSERT(0);              
       }

       myclocktime.rtc_sec = testdata.rtc_sec; 
       myclocktime.rtc_min = testdata.rtc_min; 
       myclocktime.rtc_hour = testdata.rtc_hour; 
       myclocktime.rtc_day= testdata.rtc_day; 
       myclocktime.rtc_mon = testdata.rtc_mon; 
       myclocktime.rtc_wday= testdata.rtc_wday; 
       myclocktime.rtc_year= testdata.rtc_year; 
       
#else
       myclocktime.rtc_sec = 0; 
       myclocktime.rtc_min = 0; 
       myclocktime.rtc_hour = 0; 
       myclocktime.rtc_day= 0; 
       myclocktime.rtc_mon = 0; 
       myclocktime.rtc_wday= 0; 
       myclocktime.rtc_year= 0; 
#endif
}


/**************************************************************
**	FUNCTION NAME		: WriteCurrentTimeToNVRAM
**
**  	PURPOSE				: Function to write auto test result time to NVRAM
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void WriteCurrentTimeToNVRAM(void)
{
#if ( defined(MMI_ON_HARDWARE_P) && !defined(WIN32) )
          nvram_ef_autotest_struct testdata; 
          MYTIME t;
	   S16     ErrorCode, Ret;

          memset( &testdata, 0, sizeof(testdata) );
          Ret = ReadRecord( NVRAM_EF_AUTOTEST_LID, 1, &testdata, NVRAM_EF_AUTOTEST_SIZE, &ErrorCode);
          if (!(ErrorCode == NVRAM_READ_SUCCESS && Ret == NVRAM_EF_AUTOTEST_SIZE))
          {
             ASSERT(0);              
          }

          DTGetRTCTime(&t);

          testdata.rtc_sec = t.nSec; 
          testdata.rtc_min = t.nMin; 
          testdata.rtc_hour = t.nHour; 
          testdata.rtc_day = t.nDay; 
          testdata.rtc_mon = t.nMonth;  
          testdata.rtc_wday = 0;  
          testdata.rtc_year = (kal_uint8) (t.nYear-2000); 
       
          Ret = WriteRecord( NVRAM_EF_AUTOTEST_LID, 1, &testdata, NVRAM_EF_AUTOTEST_SIZE, &ErrorCode);
          if (!(ErrorCode == NVRAM_WRITE_SUCCESS && Ret == NVRAM_EF_AUTOTEST_SIZE))
          {
              ASSERT(0);              
          }
#endif
}


/**************************************************************
**	FUNCTION NAME		: ReadSettingsFromNVRAM
**
**  	PURPOSE				: Function to read auto test setting from NVRAM
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ReadSettingsFromNVRAM(void)
{
	U8       i;

#if ( defined(MMI_ON_HARDWARE_P) && !defined(WIN32) )
       nvram_ef_autotest_struct testdata; 
	S16     ErrorCode, Ret;

       memset( &testdata, 0, sizeof(testdata) );
       Ret = ReadRecord( NVRAM_EF_AUTOTEST_LID, 1, &testdata, NVRAM_EF_AUTOTEST_SIZE, &ErrorCode);
       if (!(ErrorCode == NVRAM_READ_SUCCESS && Ret == NVRAM_EF_AUTOTEST_SIZE))
       {
           ASSERT(0);              
       }

       nvramTestItemArray.count = testdata.testcount;

	for (i=0; i<testdata.testcount; i++)
	{
	    nvramTestItemArray.priority[i] = testdata.testlist[i];  
	}
	
	gAutoTestListSize = nvramTestItemArray.count;
	//gAutoTestListSize=14;//tmp chenhe

	myclocktime.rtc_sec = testdata.rtc_sec; 
       myclocktime.rtc_min = testdata.rtc_min; 
       myclocktime.rtc_hour = testdata.rtc_hour; 
       myclocktime.rtc_day= testdata.rtc_day; 
       myclocktime.rtc_mon = testdata.rtc_mon; 
       myclocktime.rtc_wday= testdata.rtc_wday; 
       myclocktime.rtc_year= testdata.rtc_year; 
#ifdef TGT_GALLITE_G800
        gAutoTestListSize = 15;
#endif
#else
	   nvramTestItemArray.count = MAX_AUTO_TEST_ITEMS;

	   for (i=0; i<MAX_AUTO_TEST_ITEMS; i++)
	   {
	      nvramTestItemArray.priority[i] = i;  
	   }
	
	   gAutoTestListSize = nvramTestItemArray.count;
	   myclocktime.rtc_sec = 0; 
          myclocktime.rtc_min = 0; 
          myclocktime.rtc_hour = 0; 
          myclocktime.rtc_day= 0; 
          myclocktime.rtc_mon = 0; 
          myclocktime.rtc_wday= 0; 
          myclocktime.rtc_year= 0; 
#endif
}


/**************************************************************
**	FUNCTION NAME		: SaveNewItemToNVRAM
**
**  	PURPOSE				: Function to write new auto test item to NVRAM
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SaveNewItemToNVRAM(void)
{
       U16 p_val = gPriorityValue - 1; //convert to 0-based 
       U16 i;

       for (i=0; i<gAutoTestListSize; i++)
          if (nvramTestItemArray.priority[i] == gHiliteAllTestListIdx) // the item is already in the list
          {
              //pop up warning
              DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED) ,IMG_GLOBAL_UNFINISHED, 1,1000,ERROR_TONE);
              DeleteUptoScrID(EM_AUTO_TEST_SETTING_MENU_SCR);
              return;
          }

       gAutoTestListSize += 1;
      	nvramTestItemArray.count = (U8) gAutoTestListSize; 

      	// Shift the list 
      	for (i=gAutoTestListSize-1; i>p_val; i--)
       {
           nvramTestItemArray.priority[i] = nvramTestItemArray.priority[i-1];
      	}

      	nvramTestItemArray.priority[p_val] = (U8) gHiliteAllTestListIdx;

#if ( defined(MMI_ON_HARDWARE_P) && !defined(WIN32) )
       {
          nvram_ef_autotest_struct testdata; 
	   S16     ErrorCode, Ret;

          memset( &testdata, 0, sizeof(testdata) );

          testdata.testcount = nvramTestItemArray.count; 
          for (i=0; i<testdata.testcount; i++)
          {
	      testdata.testlist[i] = nvramTestItemArray.priority[i];
          }

          testdata.rtc_sec = myclocktime.rtc_sec; 
          testdata.rtc_min = myclocktime.rtc_min; 
          testdata.rtc_hour = myclocktime.rtc_hour; 
          testdata.rtc_day = myclocktime.rtc_day; 
          testdata.rtc_mon = myclocktime.rtc_mon; 
          testdata.rtc_wday= myclocktime.rtc_wday; 
          testdata.rtc_year = myclocktime.rtc_year; 
       
          Ret = WriteRecord( NVRAM_EF_AUTOTEST_LID, 1, &testdata, NVRAM_EF_AUTOTEST_SIZE, &ErrorCode);
          if (!(ErrorCode == NVRAM_WRITE_SUCCESS && Ret == NVRAM_EF_AUTOTEST_SIZE))
          {
              ASSERT(0);              
          }
          else
          {
      	      DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,1000,0);
	      DeleteUptoScrID(EM_AUTO_TEST_SETTING_MENU_SCR);
          }
       }
#else
      	   DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,1000,0);
	   DeleteUptoScrID(EM_AUTO_TEST_SETTING_MENU_SCR);
#endif       
}


/**************************************************************
**	FUNCTION NAME		: SavePriorityChangeToNVRAM
**
**  	PURPOSE				: Function to write new auto test priority to NVRAM
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SavePriorityChangeToNVRAM(void)
{
       U16 new_pri = gPriorityValue - 1; //convert to 0-based 
       U16 old_pri = gHiliteAutoTestCurrSelIdx; //0-based 
       U16 temp;

       if (new_pri == old_pri) // the priority is the same
       {
      	    DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,1000,0);
	    DeleteUptoScrID(EM_AUTO_TEST_SETTING_MENU_SCR);
           return;
       }

       // Switch the items of new_pri and old_pri
       temp = nvramTestItemArray.priority[new_pri]; 
      	nvramTestItemArray.priority[new_pri] = nvramTestItemArray.priority[old_pri];
      	nvramTestItemArray.priority[old_pri] = (U8) temp;
      	
#if ( defined(MMI_ON_HARDWARE_P) && !defined(WIN32) )
       {
          nvram_ef_autotest_struct testdata; 
	   S16     ErrorCode, Ret;
	   U8       i;

          memset( &testdata, 0, sizeof(testdata) );

          testdata.testcount = nvramTestItemArray.count; 
          for (i=0; i<testdata.testcount; i++)
          {
	      testdata.testlist[i] = nvramTestItemArray.priority[i];
          }

          testdata.rtc_sec = myclocktime.rtc_sec; 
          testdata.rtc_min = myclocktime.rtc_min; 
          testdata.rtc_hour = myclocktime.rtc_hour; 
          testdata.rtc_day = myclocktime.rtc_day; 
          testdata.rtc_mon = myclocktime.rtc_mon; 
          testdata.rtc_wday= myclocktime.rtc_wday; 
          testdata.rtc_year = myclocktime.rtc_year; 
       
          Ret = WriteRecord( NVRAM_EF_AUTOTEST_LID, 1, &testdata, NVRAM_EF_AUTOTEST_SIZE, &ErrorCode);
          if (!(ErrorCode == NVRAM_WRITE_SUCCESS && Ret == NVRAM_EF_AUTOTEST_SIZE))
          {
              ASSERT(0);              
          }
          else
          {
      	      DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,1000,0);
	      DeleteUptoScrID(EM_AUTO_TEST_SETTING_MENU_SCR);
          }
       }
#else
      	   DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,1000,0);
	   DeleteUptoScrID(EM_AUTO_TEST_SETTING_MENU_SCR);
#endif       
}


/**************************************************************
**	FUNCTION NAME		: SaveRemoveItemToNVRAM
**
**  	PURPOSE				: Function to write removed auto test item to NVRAM
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SaveRemoveItemToNVRAM(void)
{
       U16 p_val = gHiliteAutoTestCurrSelIdx;
       U16 i;

       gAutoTestListSize -= 1;
      	nvramTestItemArray.count = (U8) gAutoTestListSize; 

      	// Shift the list 
      	for (i = p_val; i < gAutoTestListSize; i++)
       {
           nvramTestItemArray.priority[i] = nvramTestItemArray.priority[i+1];
      	}

#if ( defined(MMI_ON_HARDWARE_P) && !defined(WIN32) )
       {
          nvram_ef_autotest_struct testdata; 
	   S16     ErrorCode, Ret;

          memset( &testdata, 0, sizeof(testdata) );

          testdata.testcount = nvramTestItemArray.count; 
          for (i=0; i<testdata.testcount; i++)
          {
	      testdata.testlist[i] = nvramTestItemArray.priority[i];
          }

          testdata.rtc_sec = myclocktime.rtc_sec; 
          testdata.rtc_min = myclocktime.rtc_min; 
          testdata.rtc_hour = myclocktime.rtc_hour; 
          testdata.rtc_day = myclocktime.rtc_day; 
          testdata.rtc_mon = myclocktime.rtc_mon; 
          testdata.rtc_wday= myclocktime.rtc_wday; 
          testdata.rtc_year = myclocktime.rtc_year; 
       
          Ret = WriteRecord( NVRAM_EF_AUTOTEST_LID, 1, &testdata, NVRAM_EF_AUTOTEST_SIZE, &ErrorCode);
          if (!(ErrorCode == NVRAM_WRITE_SUCCESS && Ret == NVRAM_EF_AUTOTEST_SIZE))
          {
              ASSERT(0);              
          }
          else
          {
      	      DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,1000,0);
	      DeleteUptoScrID(EM_AUTO_TEST_SETTING_MENU_SCR);
          }
       }
#else
      	   DisplayPopup((PU8)GetString(STR_GLOBAL_DONE) ,IMG_GLOBAL_ACTIVATED, 1,1000,0);
	   DeleteUptoScrID(EM_AUTO_TEST_SETTING_MENU_SCR);
#endif       
}


/**************************************************************
**	FUNCTION NAME		: EntryConfirmRemoveTestItem
**
**  	PURPOSE				: Entry function for Removed test item confirm menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryConfirmRemoveTestItem(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						    STR_GLOBAL_NO, IMG_GLOBAL_NO,
						    get_string(STR_GLOBAL_DELETE),IMG_GLOBAL_QUESTION, WARNING_TONE);


	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(SaveRemoveItemToNVRAM,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryScrEnterItemPriority
**
**  	PURPOSE				: Entry function for test item priority input menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryScrEnterItemPriority(void)
{
	PU8 guiBuffer;

	EntryNewScreen(EM_AUTO_TEST_ENTER_PRIORITY_MENU_SCR, NULL, EntryScrEnterItemPriority, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_AUTO_TEST_ENTER_PRIORITY_MENU_SCR); 

	ClearAllKeyHandler();
	gPriorityValue = gAutoTestListSize+1;
	ShowCategory87Screen(EM_AUTO_TEST_ENTER_PRIORITY_TEXT, EM_AUTO_TEST_ENTER_PRIORITY_ICON,
						STR_GLOBAL_OK, IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						1, (S32)(gAutoTestListSize+1), (S32 *) &gPriorityValue,guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(SaveNewItemToNVRAM, KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryScrChangePriority
**
**  	PURPOSE				: Entry function for test item priority change menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryScrChangePriority(void)
 {
	PU8 guiBuffer;

	EntryNewScreen(EM_AUTO_TEST_CHANGE_PRIORITY_MENU_SCR, NULL, EntryScrChangePriority, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_AUTO_TEST_CHANGE_PRIORITY_MENU_SCR); 

	ClearAllKeyHandler();
	gPriorityValue = gHiliteAutoTestCurrSelIdx+1;
	ShowCategory87Screen(EM_AUTO_TEST_ENTER_PRIORITY_TEXT, EM_AUTO_TEST_ENTER_PRIORITY_ICON,
						STR_GLOBAL_OK, IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						1, (S32)gAutoTestListSize, (S32 *) &gPriorityValue,guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(SavePriorityChangeToNVRAM, KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HiliteAllTestIdx
**
**  	PURPOSE				: Highlight function for test item index
**
**	INPUT PARAMETERS	: hiliteIdx
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HiliteAllTestIdx(S32 hiliteIdx)
{
	gHiliteAllTestListIdx = (U16)hiliteIdx;
}


/**************************************************************
**	FUNCTION NAME		: EntryGetAllTestItemList
**
**  	PURPOSE				: Entry function for get all test item menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryGetAllTestItemList(void)
{
	U16 index;
	PU8 guiBuffer;
	PU8 testItemList[MAX_AUTO_TEST_ITEMS];

	EntryNewScreen(EM_AUTO_TEST_ADD_MENU_SCR, NULL, EntryGetAllTestItemList, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_AUTO_TEST_ADD_MENU_SCR);

	ClearAllKeyHandler();
	
	RegisterHighlightHandler(HiliteAllTestIdx);

       for (index = 0; index < MAX_AUTO_TEST_ITEMS; index++)
       {
           //testItemList[index] = AutoTestItemList[index];
           testItemList[index] = (PU8) Tests[index].name;
       }
	
	ShowCategory6Screen(EM_AUTO_TEST_ADD_TEXT, EM_AUTO_TEST_ADD_ICON,
						STR_GLOBAL_OK, IMG_GLOBAL_OK,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						MAX_AUTO_TEST_ITEMS, testItemList,NULL, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetLeftSoftkeyFunction(EntryScrEnterItemPriority, KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HiliteAutoTestAddFromList
**
**  	PURPOSE				: Highlight function for auto test adding from list
**
**	INPUT PARAMETERS	: hiliteIdx
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HiliteAutoTestAddFromList(void)
{
	SetLeftSoftkeyFunction(EntryGetAllTestItemList,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HiliteAutoTestChangePriority
**
**  	PURPOSE				: Highlight function for auto test priority change
**
**	INPUT PARAMETERS	: hiliteIdx
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HiliteAutoTestChangePriority(void)
{
	SetLeftSoftkeyFunction(EntryScrChangePriority,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HiliteAutoTestRemove
**
**  	PURPOSE				: Highlight function for auto test item removal
**
**	INPUT PARAMETERS	: hiliteIdx
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HiliteAutoTestRemove(void)
{
	SetLeftSoftkeyFunction(EntryConfirmRemoveTestItem,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HiliteAutoTestCurrSelIdx
**
**  	PURPOSE				: Highlight function for current auto test item index
**
**	INPUT PARAMETERS	: hiliteIdx
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HiliteAutoTestCurrSelIdx(S32 hiliteIdx)
{
	gHiliteAutoTestCurrSelIdx= (U16)hiliteIdx;
}


/**************************************************************
**	FUNCTION NAME		: GoBackAutoTestSetting
**
**  	PURPOSE				: Wrapper function for GoBackToHistory in EM
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void GoBackAutoTestSetting(void)
{
	GoBackToHistory(EM_AUTO_TEST_SETTING_MENU_SCR);
}


/**************************************************************
**	FUNCTION NAME		: EM_InitAutoTest
**
**  	PURPOSE				: Function to initialize EM Auto test 
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EM_InitAutoTest(void)
{
//       U16 count;

	SetHiliteHandler(EM_AUTO_TEST_ADD_MENUID, HiliteAutoTestAddFromList);
	SetHiliteHandler(EM_AUTO_TEST_CHANGE_PRIORITY_MENUID, HiliteAutoTestChangePriority);
	SetHiliteHandler(EM_AUTO_TEST_REMOVE_MENUID, HiliteAutoTestRemove);

/*
	for (count = 0; count < MAX_AUTO_TEST_ITEMS; count++)
		memset(AutoTestItemList[count], 0, MAX_TEST_ITEM_NAME_LEN*ENCODING_LENGTH);
	for (count = 0 ; count< MAX_AUTO_TEST_ITEMS; count++) 
	{
	       AnsiiToUnicodeString((S8*)pOutBuffer, (S8*) (Tests[count].name));
		pfnUnicodeStrcpy((S8*)AutoTestItemList[count], (const S8*)pOutBuffer);
	}
*/
}


/**************************************************************
**	FUNCTION NAME		: EntryEmAutoTestSetting
**
**  	PURPOSE				: Entry function for Auto test setting menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmAutoTestSetting(void)
{
	PU8 guiBuffer;

       EM_InitAutoTest();
	
	EntryNewScreen(EM_AUTO_TEST_SETTING_MENU_SCR, NULL, EntryEmAutoTestSetting, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_AUTO_TEST_SETTING_MENU_SCR); 	

	ClearAllKeyHandler();
	
	GetSelectedTestList();
	
	RegisterHighlightHandler(HiliteAutoTestCurrSelIdx);
	if(!gAutoTestListSize)
	{
		AutoTestSelectedItems[0] = (U8*)GetString(STR_GLOBAL_EMPTY);
		ShowCategory6Screen(EM_AUTO_TEST_SETTING_TEXT, EM_AUTO_TEST_SETTING_ICON,
						STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						1, AutoTestSelectedItems,NULL, 0,guiBuffer);
	}
	else
	{
		ShowCategory6Screen(EM_AUTO_TEST_SETTING_TEXT, EM_AUTO_TEST_SETTING_ICON,
						STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,
						STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						gAutoTestListSize, AutoTestSelectedItems,NULL, 0,guiBuffer);
	}
	
	ChangeLeftSoftkey(STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS);
	SetLeftSoftkeyFunction(EntryScrShowAutoTestOptions,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightAutoTestSetting
**
**  	PURPOSE				: Highlight function for auto test setting
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightAutoTestSetting(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmAutoTestSetting, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryScrShowAutoTestOptions
**
**  	PURPOSE				: Entry function for Auto Test Options menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryScrShowAutoTestOptions(void)
{
	PU8 guiBuffer;
	U16 nStrItemList[15]; /* Stores the strings id of submenus returned */
	U16 nNumofItem;      /* Stores no of children in the submenu*/
	U16 nDispAttribute;  /* Stores display attribue */
	U16	ItemIcons[15];

	EntryNewScreen(EM_AUTO_TEST_OPTIONS_MENU_SCR, NULL, EntryScrShowAutoTestOptions, NULL);
	guiBuffer = GetCurrGuiBuffer(EM_AUTO_TEST_OPTIONS_MENU_SCR);

	ClearAllKeyHandler();
	nNumofItem = GetNumOfChild(EM_AUTO_TEST_OPTIONS_MENUID);

	nDispAttribute = GetDispAttributeOfItem(EM_AUTO_TEST_OPTIONS_MENUID);

	ItemIcons[0] = IMG_GLOBAL_L1;
	ItemIcons[1] = IMG_GLOBAL_L2;
	ItemIcons[2] = IMG_GLOBAL_L3;
	ItemIcons[3] = IMG_GLOBAL_L4;
	GetSequenceStringIds(EM_AUTO_TEST_OPTIONS_MENUID,nStrItemList);
	SetParentHandler(EM_AUTO_TEST_OPTIONS_MENUID);

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	if(gAutoTestListSize == 0 )
	{
		nNumofItem = 1;
	}

	ShowCategory15Screen(EM_AUTO_TEST_OPTIONS_TEXT,MAIN_MENU_TITLE_SETTINGS_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem, nStrItemList, ItemIcons,
						  LIST_MENU, 0, guiBuffer);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmPWRDownCtrl
**
**  	PURPOSE				: Highlight function for Power Down Control
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmPWRDownCtrl(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmPWRDownCtrl, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EMPWRDownCtrlHighlightHandler
**
**  	PURPOSE				: Dummy Highlight function for Power Down Control
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EMPWRDownCtrlHighlightHandler(S32 index) 
{

}


/**************************************************************
**	FUNCTION NAME		: EMPWRDownCtrlDoneFunc
**
**  	PURPOSE				: Done  function for Power Down Control to set the parameters
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EMPWRDownCtrlDoneFunc(void)
{
#ifdef MMI_ON_HARDWARE_P   
     U8  textbuf[PWRDOWNSTRLEN];
     CONFIG_PDN_TYPE PDN; 

     CloseCategory57Screen();

     //convert the textedit buffer to currPWMFreq and currPWMDuty
     UnicodeToAnsii((PS8)textbuf,  (PS8)Buf_PDN_CON0);
     PDN.pdn_con0 = (kal_uint16) atoi( (S8*)textbuf );
     UnicodeToAnsii((PS8)textbuf,  (PS8)Buf_PDN_CON1);
     PDN.pdn_con1 = (kal_uint16) atoi( (S8*)textbuf );
     UnicodeToAnsii((PS8)textbuf,  (PS8)Buf_PDN_CON2);
     PDN.pdn_con2 = (kal_uint16) atoi( (S8*)textbuf );
     UnicodeToAnsii((PS8)textbuf,  (PS8)Buf_PDN_CON3);
     PDN.pdn_con3 = (kal_uint16) atoi( (S8*)textbuf );

     // Call setting func 
     INT_SetPowerDownStatus(&PDN);
#endif
     //DisplayPopup((PU8)GetString(EM_NOTICE_SUCCESS_TEXT), IMG_GLOBAL_DEFAULT, 0, EM_NOTIFY_DURATION, 0);
     EngineerModeSuccessNotice(EM_NOTICE_SUCCESS_TEXT);
     DeleteScreenIfPresent(EM_PWR_DOWN_CONTROL_MENU_SCR);
     
}


/**************************************************************
**	FUNCTION NAME		: EntryEmPWRDownCtrl
**
**  	PURPOSE				: Entry function for Power Down Control 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmPWRDownCtrl(void)
{
#ifdef MMI_ON_HARDWARE_P   
       CONFIG_PDN_TYPE PDN; 
#endif 
	U8* guiBuffer;
	U8* inputBuffer;
	U16 inputBufferSize;
       U16 ImageList[8];
       U8  textbuf[PWRDOWNSTRLEN];
              
	EntryNewScreen(EM_PWR_DOWN_CONTROL_MENU_SCR, ExitEmPWRDownCtrl, NULL, NULL);
	
	InitializeCategory57Screen();
	
	guiBuffer = GetCurrGuiBuffer(EM_PWR_DOWN_CONTROL_MENU_SCR);
	
	inputBuffer = GetCurrNInputBuffer(EM_PWR_DOWN_CONTROL_MENU_SCR, &inputBufferSize);
 
	RegisterHighlightHandler(EMPWRDownCtrlHighlightHandler);

       // Get the current setting
#ifdef MMI_ON_HARDWARE_P   
       INT_QueryPowerDownStatus(&PDN);
#endif

       // Desc. String
	SetInlineItemActivation(&wgui_inline_items[0], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&(wgui_inline_items[0]), (U8*)GetString(EM_STR_PWNDOWN_CTRL_PDN_CON0));
       ImageList[0] = IMG_STATUS;

       // PDN_CON0
	SetInlineItemActivation(&wgui_inline_items[1], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
       memset((void*) textbuf, 0, PWRDOWNSTRLEN);
#ifdef MMI_ON_HARDWARE_P   
       sprintf((S8*)textbuf, "%d", PDN.pdn_con0);		
#else
       sprintf((S8*)textbuf, "%d", 0);		
#endif
	AnsiiToUnicodeString((S8*)Buf_PDN_CON0, (S8*)textbuf);
       SetInlineItemTextEdit(&wgui_inline_items[1], (PU8)Buf_PDN_CON0, 10, INPUT_TYPE_NUMERIC);
       ImageList[1] = 0;

       // Desc. String
	SetInlineItemActivation(&wgui_inline_items[2], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&(wgui_inline_items[2]), (U8*)GetString(EM_STR_PWNDOWN_CTRL_PDN_CON1));
       ImageList[2] = IMG_STATUS;

       // PDN_CON1
	SetInlineItemActivation(&wgui_inline_items[3], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
       memset((void*) textbuf, 0, PWRDOWNSTRLEN);
#ifdef MMI_ON_HARDWARE_P   
       sprintf((S8*)textbuf, "%d", PDN.pdn_con1);		
#else
       sprintf((S8*)textbuf, "%d", 0);		
#endif
	AnsiiToUnicodeString((S8*)Buf_PDN_CON1, (S8*)textbuf);
       SetInlineItemTextEdit(&wgui_inline_items[3], (PU8)Buf_PDN_CON1, 10, INPUT_TYPE_NUMERIC);
       ImageList[3] = 0;

       // Desc. String
	SetInlineItemActivation(&wgui_inline_items[4], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&(wgui_inline_items[4]), (U8*)GetString(EM_STR_PWNDOWN_CTRL_PDN_CON2));
       ImageList[4] = IMG_STATUS;

       // PDN_CON2
	SetInlineItemActivation(&wgui_inline_items[5], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
       memset((void*) textbuf, 0, PWRDOWNSTRLEN);
#ifdef MMI_ON_HARDWARE_P   
       sprintf((S8*)textbuf, "%d", PDN.pdn_con2);		
#else
       sprintf((S8*)textbuf, "%d", 0);		
#endif
	AnsiiToUnicodeString((S8*)Buf_PDN_CON2, (S8*)textbuf);
       SetInlineItemTextEdit(&wgui_inline_items[5], (PU8)Buf_PDN_CON2, 10, INPUT_TYPE_NUMERIC);
       ImageList[5] = 0;
       
       // Desc. String
	SetInlineItemActivation(&wgui_inline_items[6], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&(wgui_inline_items[6]), (U8*)GetString(EM_STR_PWNDOWN_CTRL_PDN_CON3));
       ImageList[6] = IMG_STATUS;

       // PDN_CON3
	SetInlineItemActivation(&wgui_inline_items[7], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
       memset((void*) textbuf, 0, PWRDOWNSTRLEN);
#ifdef MMI_ON_HARDWARE_P
       sprintf((S8*)textbuf, "%d", PDN.pdn_con3);		
#else
       sprintf((S8*)textbuf, "%d", 0);		
#endif
	AnsiiToUnicodeString((S8*)Buf_PDN_CON3, (S8*)textbuf);
       SetInlineItemTextEdit(&wgui_inline_items[7], (PU8)Buf_PDN_CON3, 10, INPUT_TYPE_NUMERIC);
       ImageList[7] = 0;

       if(inputBuffer!=NULL) SetCategory57Data(wgui_inline_items, 8, inputBuffer); 
       
       ShowCategory57Screen(EM_PWR_DOWN_CONTROL_TEXT, EM_PWR_DOWN_CONTROL_ICON, 
       	                STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
				  8, ImageList, wgui_inline_items, 0, guiBuffer);	

	SetCategory57RightSoftkeyFunctions(EMPWRDownCtrlDoneFunc, GoBackHistory);
       
}

#undef PWRDOWNSTRLEN


/**************************************************************
**	FUNCTION NAME		: ExitEmPWRDownCtrl
**
**  	PURPOSE				: Ezit function for Power Down Control 
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitEmPWRDownCtrl(void)
{
	history_t currHistory;
       U16 inputBufferSize;
       
	CloseCategory57Screen();
   
       currHistory.scrnID = EM_PWR_DOWN_CONTROL_MENU_SCR;
	currHistory.entryFuncPtr = EntryEmPWRDownCtrl;

	GetCategoryHistory(currHistory.guiBuffer);
	inputBufferSize = (U16)GetCategory57DataSize();  
	GetCategory57Data((U8*) currHistory.inputBuffer);       
	AddNHistory(currHistory, inputBufferSize);           

}


/**************************************************************
**	FUNCTION NAME		: HighlightOnCertainSerialPortMenu
**
**  	PURPOSE				: Highlight function for Serial Port menu
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightOnCertainSerialPortMenu( S32 index )
{
     U8 is_on=MMI_FALSE;
     currentHighlightIndex = (U8) index;

     switch( currentHighlightIndex )
      {
         case EM_MENU_SERIAL_PORT_EN_1:
            is_on = custom_em_get_uart_detection_status( CUSTOM_EM_UART_PORT1 );
            break;
         case EM_MENU_SERIAL_PORT_EN_2:
            is_on = custom_em_get_uart_detection_status( CUSTOM_EM_UART_PORT2 );
            break;
         case EM_MENU_SERIAL_PORT_EN_3:
            is_on = custom_em_get_uart_detection_status( CUSTOM_EM_UART_PORT3 );
            break;
         default:
            break;
      }

      if( is_on == MMI_TRUE )
      {
          ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
          //Category53ChangeItemDescription( index, (U8*)get_string(STR_GLOBAL_ON)  );
       }
      else
      {
          ChangeLeftSoftkey(STR_GLOBAL_ON,0);
          //Category53ChangeItemDescription( index, (U8*)get_string(STR_GLOBAL_OFF)  );
      }
      
      ChangeRightSoftkey(STR_GLOBAL_BACK,0);
      
      
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(SerialPortMenuLSKHandler,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	//RedrawCategoryFunction();

}


/**************************************************************
**	FUNCTION NAME		: SerialPortMenuLSKHandler
**
**  	PURPOSE				: LSK Handler for Serial Port menu
**
**	INPUT PARAMETERS	: void
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void SerialPortMenuLSKHandler(void)
{
     U8 is_on=MMI_FALSE, port=CUSTOM_EM_UART_PORT1;

     switch( currentHighlightIndex )
      {
         case EM_MENU_SERIAL_PORT_EN_1:
            is_on = custom_em_get_uart_detection_status( CUSTOM_EM_UART_PORT1 );
            port = CUSTOM_EM_UART_PORT1;
            break;
         case EM_MENU_SERIAL_PORT_EN_2:
            is_on = custom_em_get_uart_detection_status( CUSTOM_EM_UART_PORT2 );
            port = CUSTOM_EM_UART_PORT2;
            break;
         case EM_MENU_SERIAL_PORT_EN_3:
            is_on = custom_em_get_uart_detection_status( CUSTOM_EM_UART_PORT3 );
            port = CUSTOM_EM_UART_PORT3;
            break;
         default:
            break;
      }
     
      if( is_on == MMI_TRUE )
      {
         custom_em_set_uart_detection_status(port, MMI_FALSE );
         ChangeLeftSoftkey(STR_GLOBAL_ON,0);
         Category53ChangeItemDescription( currentHighlightIndex, (U8*)get_string(STR_GLOBAL_OFF)  );
      }
      else
      {
          custom_em_set_uart_detection_status(port, MMI_TRUE );
          ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
          Category53ChangeItemDescription( currentHighlightIndex, (U8*)get_string(STR_GLOBAL_ON)  );
       }
      
       RedrawCategoryFunction();

}

#ifdef __J2ME__
extern void EntryScrJavaEM(void);


/**************************************************************
**	FUNCTION NAME		: HighlightEmMiscJ2meTck
**
**  	PURPOSE				: Highlight function for MISC J2ME TCK menu
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmMiscJ2meTck(void)
{
      ChangeLeftSoftkey(STR_GLOBAL_OK,0);
      ChangeRightSoftkey(STR_GLOBAL_BACK,0);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryScrJavaEM,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}
#endif


/**************************************************************
**	FUNCTION NAME		: EntryEmMiscBacklight
**
**  	PURPOSE				: Entry function for MISC Backlight menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmMiscBacklight(void)
{
     if( backlight_mode == BACKLIGHT_NORMAL )
     {  //Backlight on forever
         SetBacklightModeExe(0) ;
         backlight_mode = BACKLIGHT_FOREVER;
         Category52ChangeItemDescription(3, (U8*)get_string(STR_GLOBAL_ON));
         ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
     }
     else
     {  //Backlight back to normal 
         SetBacklightModeExe(1) ;
         backlight_mode = BACKLIGHT_NORMAL;
         Category52ChangeItemDescription(3, (U8*)get_string(STR_GLOBAL_OFF));
         ChangeLeftSoftkey(STR_GLOBAL_ON,0);
     }
      
     SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

     RedrawCategoryFunction();
     HighlightEmMiscBacklight();
}


/**************************************************************
**	FUNCTION NAME		: EntryEmSerialPortEnable
**
**  	PURPOSE				: Entry function for Serial Port Enable
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmSerialPortEnable( void )
{
	U8* guiBuffer;
	U8   i;
	U8* PopUpList[32];
	U16 ItemIcons[EM_MENU_SERIAL_PORT_EN_MAX];

	EntryNewScreen(EM_SERIAL_PORT_EN_MENU_SCR, NULL, EntryEmSerialPortEnable, NULL);
	guiBuffer = GetCurrGuiBuffer (EM_SERIAL_PORT_EN_MENU_SCR);		 

	RegisterHighlightHandler (HighlightOnCertainSerialPortMenu);

     subMenuDataPtrs[0] = (PU8)GetString(EM_DEV_UART_1_TEXT);
     subMenuDataPtrs[1] = (PU8)GetString(EM_DEV_UART_2_TEXT);
     subMenuDataPtrs[2] = (PU8)GetString(EM_DEV_UART_3_TEXT);

     for(i=0;i<EM_MENU_SERIAL_PORT_EN_MAX;i++)
      if( custom_em_get_uart_detection_status( CUSTOM_EM_UART_PORT1+i ) == MMI_TRUE)
        PopUpList[i] = (PU8)GetString( STR_GLOBAL_ON );
      else
        PopUpList[i] = (PU8)GetString( STR_GLOBAL_OFF );
     
     for(i=0;i<EM_MENU_SERIAL_PORT_EN_MAX;i++)
		ItemIcons[i] = IMG_GLOBAL_L1+i;

	ShowCategory53Screen(EM_SERIAL_PORT_EN_TEXT,0,
				STR_GLOBAL_OK, 0,
				STR_GLOBAL_BACK, 0,
				EM_MENU_SERIAL_PORT_EN_MAX, 
				(PU8*)subMenuDataPtrs, (U16*)ItemIcons, PopUpList,
				0, 0, guiBuffer);

	SetLeftSoftkeyFunction (SerialPortMenuLSKHandler, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmAutoReset
**
**  	PURPOSE				: Entry function for Auto Reset
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmAutoReset( void )
{
	// ASSERT(0);
	 
	//EXT_ASSERT(0,0,0,0);
}

//Lisen 012605
/**************************************************************
**	FUNCTION NAME		: EntryEmMMIDebug
**
**  	PURPOSE				: Entry function for MMI Enable Debug menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmMMIDebug(void)
{
#ifdef DEBUG_ASSERT
   g_mmi_frm_enable_debug_assert = 1 - g_mmi_frm_enable_debug_assert;
#endif

   SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

   ReplaceNewScreenHandler(EM_MMI_DEBUG_SCR, ExitEmMMIDebug, NULL);

#ifdef DEBUG_ASSERT
   if (g_mmi_frm_enable_debug_assert)
      Category52ChangeItemDescription(8, (U8*)get_string(STR_GLOBAL_ON));
   else
      Category52ChangeItemDescription(8, (U8*)get_string(STR_GLOBAL_OFF));
#endif

   RedrawCategoryFunction();
   HighlightEmMMIDebug();
}


/**************************************************************
**	FUNCTION NAME		: ExitEmMMIDebug
**
**  	PURPOSE				: Exit function for MMI Enable Debug menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitEmMMIDebug(void)
{
	history_t    currHistory;
	S16		nHistory	=	0;

	currHistory.scrnID = EM_MMI_DEBUG_SCR;
	currHistory.entryFuncPtr = EntryEmMMIDebug;
	pfnUnicodeStrcpy ((S8*)currHistory.inputBuffer, (S8*)&nHistory);
	GetCategoryHistory(currHistory.guiBuffer);
	AddHistory(currHistory);
}

/**************************************************************
**	FUNCTION NAME		: EntryEmMemoryDump
**
**  	PURPOSE				: Entry function for Memory Dump menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmMemoryDump(void)
{
   if (INT_MemoryDumpFlag == EX_FORCEMEMORYDUMP)
     INT_MemoryDumpFlag = 0;
   else
     INT_MemoryDumpFlag = EX_FORCEMEMORYDUMP;     

   SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

   ReplaceNewScreenHandler(EM_MEMORY_DUMP_SCR, ExitEmMemoryDump, NULL);

   if (INT_MemoryDumpFlag == EX_FORCEMEMORYDUMP)
      Category52ChangeItemDescription(7, (U8*)get_string(STR_GLOBAL_ON));
   else
      Category52ChangeItemDescription(7, (U8*)get_string(STR_GLOBAL_OFF));

   RedrawCategoryFunction();
   HighlightEmMemoryDump();
}


/**************************************************************
**	FUNCTION NAME		: ExitEmMemoryDump
**
**  	PURPOSE				: Exit function for Memory Dump menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void ExitEmMemoryDump(void)
{
	history_t    currHistory;
	S16		nHistory	=	0;

	currHistory.scrnID = EM_MEMORY_DUMP_SCR;
	currHistory.entryFuncPtr = EntryEmMemoryDump;
	pfnUnicodeStrcpy ((S8*)currHistory.inputBuffer, (S8*)&nHistory);
	GetCategoryHistory(currHistory.guiBuffer);
	AddHistory(currHistory);
}

/**************************************************************
**	FUNCTION NAME		: EngineerModeNoticeFinished
**
**  	PURPOSE				: Function for indicating EM notice finish
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EngineerModeNoticeFinished(void)
{
   StopTimer(EM_NOTIFYDURATION_TIMER);
   if( gIsFromEmSetUartPort )
      {
	    gIsFromEmSetUartPort = 0;
	    GoBacknHistory(1);
      }
   else
   {
      //GoBackHistory();
      GoBacknHistory(1);
   }
}


/**************************************************************
**	FUNCTION NAME		: EngineerModeNoticeText
**
**  	PURPOSE				: Function for showing EM notice text
**
**	INPUT PARAMETERS	: string, icon
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EngineerModeNoticeText(U8 *string, U16 icon)
{
	/*Wangzl Modify Start For 6362 Ver: TBM780  on 2007-8-21 14:53 */
	// display popup directly
	#if 0
       EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

	ShowCategory63Screen(string, icon, NULL );
	SetLeftSoftkeyFunction(EngineerModeNoticeFinished,KEY_EVENT_UP);
	StartTimer(EM_NOTIFYDURATION_TIMER, EM_NOTIFY_DURATION, EngineerModeNoticeFinished);
	#else
	 DisplayPopup(string, icon, 0, EM_NOTIFY_DURATION, 0);
	#endif
	/*Wangzl Modify End  For 6362 Ver: TBM780  on 2007-8-21 14:53 */
}


/**************************************************************
**	FUNCTION NAME		: EngineerModeSuccessNotice
**
**  	PURPOSE				: Function for indicating EM Success notice 
**
**	INPUT PARAMETERS	: stringId
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EngineerModeSuccessNotice(U16 stringId)
{
	/*Wangzl Modify Start For 6362 Ver: TBM780  on 2007-8-21 16:22 */
	#if 0
       EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

	ShowCategory64Screen(EM_NOTICE_SUCCESS_TEXT,IMG_GLOBAL_WARNING,0);
	StartTimer(EM_NOTIFYDURATION_TIMER, EM_NOTIFY_DURATION, EngineerModeNoticeFinished);
	#else
	DisplayPopup((U8 *)get_string(stringId), IMG_GLOBAL_ACTIVATED, 0, EM_NOTIFY_DURATION, 0);
	DeleteScreenIfPresent(EM_DEV_PWM_VIEWER_SCR_ID);
	DeleteScreenIfPresent(EM_DEV_SLEEPMODE_MENU_SCR);
	#endif
	/*Wangzl Modify End  For 6362 Ver: TBM780  on 2007-8-21 16:22 */
}

//Lisen 09062005
#ifndef __DYNAMIC_UART_PORT_SWITCHING__
/**************************************************************
**	FUNCTION NAME		: EngineerModeSuccessRebootNotice
**
**  	PURPOSE				: Function for indicating EM Success notice and then reboot
**
**	INPUT PARAMETERS	: stringId
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EngineerModeSuccessRebootNotice(U16 stringId)
{
       EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

	ShowCategory64Screen(EM_NOTICE_SUCCESS_REBOOT_TEXT, IMG_GLOBAL_WARNING,0);
	StartTimer(EM_NOTIFYDURATION_TIMER, 3000, ShutdownSystemOperation);
}
#endif

/**************************************************************
**	FUNCTION NAME		: EngineerModeErrorNotice
**
**  	PURPOSE				: Function for indicating EM Error notice 
**
**	INPUT PARAMETERS	: stringId
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EngineerModeErrorNotice(U16 stringId)
{
       EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

	ShowCategory64Screen(STR_GLOBAL_ERROR,IMG_GLOBAL_WARNING,0);
	StartTimer(EM_NOTIFYDURATION_TIMER, EM_NOTIFY_DURATION, EngineerModeNoticeFinished);
}


/**************************************************************
**	FUNCTION NAME		: EngineerModeWarningNotice
**
**  	PURPOSE				: Function for indicating EM Warning notice 
**
**	INPUT PARAMETERS	: stringId
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EngineerModeWarningNotice(U16 stringId)
{
       EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

	ShowCategory64Screen(EM_NOTICE_WARNING_TEXT,IMG_GLOBAL_WARNING,0);
	StartTimer(EM_NOTIFYDURATION_TIMER, EM_NOTIFY_DURATION, EngineerModeNoticeFinished);
}


/**************************************************************
**	FUNCTION NAME		: EngineerModeStatusUpdate
**
**  	PURPOSE				: Function for indicating EM status update
**
**	INPUT PARAMETERS	: mod_id, info_req
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EngineerModeStatusUpdate(U32 mod_id, U32 info_req)
{
//   ASSERT(info_req==newEmMode)
   if(currentEmCommand==EM_START)
      currentEmMode|=info_req;
   else
      currentEmMode&=~info_req;

   if(currentEmMode)
      gEmInfoOn=1;
   else
      gEmInfoOn=0;

   channel_descr_info.channel_type = DEFAULT_CHANNEL;

   //standard_check_list_handle_item_select(activeEmModeIndex);
   currentEmCommand = EM_NONE;

}


/**************************************************************
**	FUNCTION NAME		: EngineerModeStatusIndHdlr
**
**  	PURPOSE				: Handler function for EM status change
**
**	INPUT PARAMETERS	: inMsg
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EngineerModeStatusIndHdlr(void* inMsg)
{
#ifdef MMI_ON_HARDWARE_P
   mmi_em_status_ind_struct* status_ind = inMsg;

   switch(status_ind->em_info)
   {
      case RR_EM_MEASUREMENT_REPORT_INFO:
         kal_mem_cpy(&meas_report_info, status_ind->info, sizeof(rr_em_measurement_report_info_struct));
         break;
      case RR_EM_RADIO_LINK_COUNTER_INFO:
         kal_mem_cpy(&radio_link_counter_info, status_ind->info, sizeof(rr_em_radio_link_counter_info_struct));
         break;                        
      case RR_EM_CHANNEL_DESCR_INFO:
         kal_mem_cpy(&channel_descr_info, status_ind->info, sizeof(rr_em_channel_descr_info_struct));
         break;                        
      case RR_EM_CONTROL_MSG_INFO:
         kal_mem_cpy(&control_msg_info, status_ind->info, sizeof(rr_em_control_msg_info_struct));
         break;                        
   }

	if(g_idle_context.IsOnIdleScreen&&gEmInfoOn)
	{
	   //idle_screen_show_em_info();
		//EntryIdleScreen();
	}
#endif
}


/**************************************************************
**	FUNCTION NAME		: idle_screen_show_em_info
**
**  	PURPOSE				: EM info showing function
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void idle_screen_show_em_info(void)
{
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#ifdef __BW_MMI__
/* under construction !*/
#else
/* under construction !*/
#endif
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
#endif
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
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /* 0 */
}


/**************************************************************
**	FUNCTION NAME		: HighlightEMDebugInfo
**
**  	PURPOSE				: Highlight function for Debug info menu
**
**	INPUT PARAMETERS	: index
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEMDebugInfo(void)
{
	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEMDebugInfo, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryEMDebugInfo
**
**  	PURPOSE				: Entry function for EM Debug info
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEMDebugInfo(void)
{
	U16		ItemList[32];
	U16		ItemIcons[32];
	U16		nItems;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;

	EntryNewScreen(EM_DEBUG_INFO_SCR, NULL, EntryEMDebugInfo, NULL);

	guiBuffer = GetCurrGuiBuffer(EM_DEBUG_INFO_SCR);
	nItems = GetNumOfChild(EM_DEBUG_INFO_MENUID);
	GetSequenceStringIds(EM_DEBUG_INFO_MENUID,ItemList);
	SetParentHandler(EM_DEBUG_INFO_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for(i=0;i<nItems;i++)
		PopUpList[i]=NULL;
       for(i=0;i<nItems;i++)
	{
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}

       if (FS_TRACE_ON)
          PopUpList[0] = (U8*)get_string(STR_GLOBAL_ON);
       else
          PopUpList[0] = (U8*)get_string(STR_GLOBAL_OFF);
      
	ShowCategory52Screen(STR_ID_EM_DEBUG_INFO,0,
						     STR_GLOBAL_OK,0,
						     STR_GLOBAL_BACK,0,
						     nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEMFsTrace
**
**  	PURPOSE				: Highlight function for FS Trace menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEMFsTrace(void)
{
       //Lisen 0710
       if (FS_TRACE_ON == FALSE)
       {
           ChangeLeftSoftkey(STR_GLOBAL_ON,0);
       }
       else
       {
           ChangeLeftSoftkey(STR_GLOBAL_OFF,0);
       }

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEMFsTrace, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryEMFsTrace
**
**  	PURPOSE				: Entry function for FS Trace menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEMFsTrace(void)
{
   //EntryNewScreen(SCR_ID_EM_FS_TRACE, NULL, NULL, NULL);

   if (FS_TRACE_ON == FALSE)
   {
      FS_TRACE_ON = TRUE;
      Category52ChangeItemDescription(0, (U8*)get_string(STR_GLOBAL_ON));
#ifdef __FS_TRACE_SUPPORT__
  #ifdef MMI_ON_HARDWARE_P
      FS_SetTrace(FS_TRACE_ERROR | FS_TRACE_ALL_TASK, FS_INFINITE);
  #endif
#endif   
   }
   else
   {
      FS_TRACE_ON = FALSE;
      Category52ChangeItemDescription(0, (U8*)get_string(STR_GLOBAL_OFF));
#ifdef __FS_TRACE_SUPPORT__
  #ifdef MMI_ON_HARDWARE_P
      FS_SetTrace(FS_TRACE_DISABLE, FS_INFINITE);
  #endif
#endif   
   }
   
   SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

   RedrawCategoryFunction();
   HighlightEMFsTrace();
}


/**************************************************************
**	FUNCTION NAME		: HighlightEMLastException
**
**  	PURPOSE				: Highlight function for Exception showing menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEMLastException(void)
{
       ChangeLeftSoftkey(STR_GLOBAL_OK,0);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEMLastException, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

#ifdef MMI_ON_HARDWARE_P  
extern void nvram_report_last_exception(void *task_name_p, void *type_p, void *time_p, void *status_p);
#endif
/**************************************************************
**	FUNCTION NAME		: EntryEMLastException
**
**  	PURPOSE				: Entry function for Exception showing menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEMLastException(void)
{
    U8*	guiBuffer;

    kal_char task_name[8];
    exception_type type = 0;
    ex_rtc_struct time;
    kal_bool status = FALSE;

    EntryNewScreen(EM_LAST_EXCEPTION_SCR, NULL, EntryEMLastException, NULL);

    guiBuffer = GetCurrGuiBuffer(EM_LAST_EXCEPTION_SCR);
    SetParentHandler(EM_LAST_EXCEPTION_MENUID);
    RegisterHighlightHandler(ExecuteCurrHiliteHandler);

  #ifdef MMI_ON_HARDWARE_P    
    nvram_report_last_exception((void *)&task_name, (void *)&type, (void *)&time, (void *)&status);
  #else
    memset(&time, 0, sizeof(time));
  #endif

    if (status)
    {
          sprintf((S8*)EMFMAsciiDisplayBuf, 
             "Last Exception: \n Type: %x\n Task: %s\n Time: \n (%2d/%2d) %2d:%2d\n", 
             type, task_name, 
             time.rtc_mon,  time.rtc_day, time.rtc_hour, time.rtc_min);
    }
    else
    {
          sprintf((S8*)EMFMAsciiDisplayBuf, "No record: \n ");
    }

    AnsiiToUnicodeString((S8*)EMFMUnicodeDisplayBuf, (S8*)EMFMAsciiDisplayBuf);
    ShowCategory7Screen( STR_ID_EM_LAST_EXCEPTION, IMG_ID_EM_LAST_EXCEPTION,
	                        STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK,
	                        IMG_GLOBAL_BACK, ( PU8 ) EMFMUnicodeDisplayBuf, NULL );

    SetLeftSoftkeyFunction(NULL, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

//Lisen 0921   

/**************************************************************
**	FUNCTION NAME		: HighlightEMPowerOnDuration
**
**  	PURPOSE				: Highlight function for Power On Duration menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEMPowerOnDuration(void)
{
       ChangeLeftSoftkey(STR_GLOBAL_OK,0);

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEMPowerOnDurationMenu, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

extern S8   UnicodeDisplayBuf[MAX_TEST_STRING_BUF*4];
/**************************************************************
**	FUNCTION NAME		: EntryEMPowerOnDurationMenu
**
**  	PURPOSE				: Entry function for Power On Duration Info menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEMPowerOnDurationMenu(void)
{
#ifdef MMI_ON_HARDWARE_P
	U8*	guiBuffer;
        MYTIME currentTime, timeDiff;
       
	EntryNewScreen(EM_POWER_ON_DURATION_MENU_SCR, NULL, EntryEMPowerOnDurationMenu, NULL);
       	
	guiBuffer = GetCurrGuiBuffer(EM_POWER_ON_DURATION_MENU_SCR);
	SetParentHandler(EM_POWER_ON_DURATION_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	memset(UnicodeDisplayBuf, 0, sizeof(UnicodeDisplayBuf));

       DTGetRTCTime(&currentTime);

       GetTimeDifference(&currentTime, &StartUpTime, &timeDiff);

	memset(EMFMAsciiDisplayBuf, 0, sizeof(EMFMAsciiDisplayBuf));
	memset(EMFMUnicodeDisplayBuf, 0, sizeof(EMFMUnicodeDisplayBuf));
	sprintf((S8*) EMFMAsciiDisplayBuf, "[PWR Time]\n %02d/%02d %02d:%02d:%02d\n", 
		      StartUpTime.nMonth, StartUpTime.nDay,StartUpTime.nHour,StartUpTime.nMin, StartUpTime.nSec);
	AnsiiToUnicodeString((S8*)EMFMUnicodeDisplayBuf, (S8*)EMFMAsciiDisplayBuf);
       UCS2Strcpy((S8*)UnicodeDisplayBuf, (const S8 *)EMFMUnicodeDisplayBuf);

	memset(EMFMAsciiDisplayBuf, 0, sizeof(EMFMAsciiDisplayBuf));
	memset(EMFMUnicodeDisplayBuf, 0, sizeof(EMFMUnicodeDisplayBuf));
	sprintf((S8*) EMFMAsciiDisplayBuf, "[Current Time]\n %02d/%02d %02d:%02d:%02d\n", 
		      currentTime.nMonth, currentTime.nDay,currentTime.nHour,currentTime.nMin, currentTime.nSec);
	AnsiiToUnicodeString((S8*)EMFMUnicodeDisplayBuf, (S8*)EMFMAsciiDisplayBuf);
       UCS2Strcat((S8*) UnicodeDisplayBuf, (const S8 *)EMFMUnicodeDisplayBuf);
       
	memset(EMFMAsciiDisplayBuf, 0, sizeof(EMFMAsciiDisplayBuf));
	memset(EMFMUnicodeDisplayBuf, 0, sizeof(EMFMUnicodeDisplayBuf));
	sprintf((S8*) EMFMAsciiDisplayBuf, "[Duration]\n %02d/%02d %02d:%02d:%02d\n", 
		      timeDiff.nMonth, timeDiff.nDay,timeDiff.nHour,timeDiff.nMin, timeDiff.nSec);
	AnsiiToUnicodeString((S8*)EMFMUnicodeDisplayBuf, (S8*)EMFMAsciiDisplayBuf);
       UCS2Strcat((S8*) UnicodeDisplayBuf, (const S8 *)EMFMUnicodeDisplayBuf);
       	
	memset(EMFMAsciiDisplayBuf, 0, sizeof(EMFMAsciiDisplayBuf));
	memset(EMFMUnicodeDisplayBuf, 0, sizeof(EMFMUnicodeDisplayBuf));
	sprintf((S8*) EMFMAsciiDisplayBuf, "[Last Duration]\n %02d/%02d %02d:%02d:%02d\n", 
		      LastDuration.nMonth, LastDuration.nDay,LastDuration.nHour,LastDuration.nMin, LastDuration.nSec);
	AnsiiToUnicodeString((S8*)EMFMUnicodeDisplayBuf, (S8*)EMFMAsciiDisplayBuf);
       UCS2Strcat((S8*) UnicodeDisplayBuf, (const S8 *)EMFMUnicodeDisplayBuf);

       ShowCategory74Screen(EM_POWER_ON_DURATION_TEXT, 0, 0, 0, STR_GLOBAL_BACK,0, 
		                               (PU8) UnicodeDisplayBuf, sizeof(UnicodeDisplayBuf), guiBuffer );

 	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

       memcpy(&LastDuration, &timeDiff, sizeof(LastDuration));
#endif

}

//Lisen 03092005
/**************************************************************
**	FUNCTION NAME		: HightlightEMRAMTestHdlr
**
**  	PURPOSE				: Highlight function for RAM Test menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HightlightEMRAMTestHdlr(void)
{
#ifdef MMI_ON_HARDWARE_P
	change_left_softkey(STR_GLOBAL_OK,0);
	SetLeftSoftkeyFunction(EntryEMRAMTestMenu,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	redraw_left_softkey();
#else
	change_left_softkey(0,0);
	SetLeftSoftkeyFunction(NULL,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	redraw_left_softkey();
#endif
}

/**************************************************************
**	FUNCTION NAME		: EntryEMRAMTestMenu
**
**  	PURPOSE				: Entry function for RAM test menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEMRAMTestMenu(void)
{
#ifdef MMI_ON_HARDWARE_P

       if (SST_AdvRAMTest() == KAL_TRUE)
       {
          EngineerModeSuccessNotice(0);
       }
       else
       {
          EngineerModeErrorNotice(0);
       }
#else
       EngineerModeSuccessNotice(0);
#endif
}



#ifdef __TCPIP__ 

/**************************************************************
**	FUNCTION NAME		: EmSocketInitResult
**
**  	PURPOSE				: Set EmSocketResult to default value
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmSocketInitResult(void)
{
	EmSocketResult = EN_SOC_SUCCESS;
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmSocket
**
**  	PURPOSE				: Highlight function for Socket test menu
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmSocket(void)
{

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmSocketMenu,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}



/**************************************************************
**	FUNCTION NAME		: EntryEmSocketMenu
**
**  	PURPOSE				: Entry function for Engineer Mode Socket test
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmSocketMenu(void)
{
	U16		ItemList[32];
	U16		ItemIcons[32];
	U16		nItems;
	U8*		guiBuffer;
	U8*		PopUpList[32];
	S32		i;

	EntryNewScreen(EM_SOCKET_MENU_SCR, NULL, EntryEmSocketMenu, NULL);

	guiBuffer = GetCurrGuiBuffer(EM_SOCKET_MENU_SCR);
	nItems = GetNumOfChild(EM_SOCKET_MENUID);
	GetSequenceStringIds(EM_SOCKET_MENUID,ItemList);
	SetParentHandler(EM_SOCKET_MENUID);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	EmSocketInitResult();
	if (EmSocketResultBuffer != NULL)
		{
			OslMfree(EmSocketResultBuffer);
			EmSocketResultBuffer = NULL;
		}
	
	for(i=0;i<nItems;i++)
	{
		PopUpList[i]=NULL;
		ItemIcons[i] = (U16) (IMG_GLOBAL_L1+i);//ReturnRequiredNumberImageId(i);
	}
      
	ShowCategory52Screen(EM_SOCKET_TEXT,0,
						     STR_GLOBAL_OK,0,
						     STR_GLOBAL_BACK,0,
						     nItems, ItemList, ItemIcons, (U8**) PopUpList, 0, 0, guiBuffer);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);	
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmDNSQuery
**
**  	PURPOSE				: Highlight function for DNS query
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmDNSQuery(void)
{

	currentSocketAppType = DNS;

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmDNSQuery,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmDNSQuery
**
**  	PURPOSE				: Entry function for EM DNS Query
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmDNSQuery(void)
{

	byte *history_buffer;
	EmSocketInitResult();
	   
	EntryNewScreen(EM_SOCKET_DNS_INPUT_SCR, NULL, EntryEmDNSQuery, NULL);
	history_buffer=GetCurrGuiBuffer(EM_SOCKET_DNS_INPUT_SCR);

	if (history_buffer == NULL)
	{
		memset( EmSocketInputBuff, 0, sizeof(EmSocketInputBuff) );
	}
       ShowCategory5Screen( EM_SOCKET_DNS_DOMAIN_NAME_TEXT,  0, \
                                          STR_GLOBAL_OPTIONS, 0 , \
                                          STR_GLOBAL_BACK, 0, \
                                          INPUT_TYPE_ALPHANUMERIC_SENTENCECASE, \
                                          (PU8)EmSocketInputBuff, EM_SOCKET_MAX_INPUT_LEN, history_buffer );

   	SetLeftSoftkeyFunction(EntryEmDNSOption,KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	RegisterInputMethodScreenCloseFunction (EmSocketGoBack2History);

}

/**************************************************************
**	FUNCTION NAME		: EntryEmDNSOption
**
**  	PURPOSE				: Entry function for EM DNS Query Option
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmDNSOption (void)
{
	U8* guiBuffer=NULL;
	U16 numItems=0;
	U16 nStrItemList[32];
	
	EntryNewScreen(EM_SOCKET_DNS_OPTION_SCR, NULL, EntryEmDNSOption, NULL);
	
	guiBuffer = GetCurrGuiBuffer (EM_SOCKET_DNS_OPTION_SCR);		 
	numItems = GetNumOfChild (EM_SOCKET_DNS_OPTION_MENUID);
	GetSequenceStringIds(EM_SOCKET_DNS_OPTION_MENUID, nStrItemList);
	SetParentHandler (EM_SOCKET_DNS_OPTION_MENUID);

	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	
	ShowCategory52Screen (STR_GLOBAL_OPTIONS, 0, 
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  numItems, nStrItemList, (U16 *)gIndexIconsImageList,
						  NULL , 0, 0, guiBuffer );
}


/**************************************************************
**	FUNCTION NAME		: EmSocketSendReq
**
**  	PURPOSE				: function for sending request
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmSocketSendReq(void)
{

#ifdef MMI_ON_HARDWARE_P
	U8 emSocketAccountId = custom_get_csd_profile_num();
#else
	U8 emSocketAccountId = 5;
#endif
	mmi_soc_demo_app_result emSocketTmpResult = EN_SOC_SUCCESS;
	S8 AscTmp[16];
	PS8 emSocketInputdata=OslMalloc((EM_SOCKET_MAX_INPUT_LEN+1)*ENCODING_LENGTH);

	EmSocketIsCallbackFuncCalled = FALSE;


	if (currentSocketAppType == HTTP || currentSocketAppType == DNS || currentSocketAppType == ECHO)
	{
		UnicodeToAnsii(emSocketInputdata, EmSocketInputBuff);
	}

	if (currentSocketAppType == DAYTIME || currentSocketAppType == ECHO)
	{
		UnicodeToAnsii(AscTmp, (S8*)EmSocketIP1);
		 EmServerIPAddress[0] = (U8)atoi(AscTmp);
		 UnicodeToAnsii(AscTmp, (S8*)EmSocketIP2);
		 EmServerIPAddress[1] = (U8)atoi(AscTmp);
		UnicodeToAnsii(AscTmp, (S8*)EmSocketIP3);
		 EmServerIPAddress[2] = (U8)atoi(AscTmp);
		UnicodeToAnsii(AscTmp, (S8*)EmSocketIP4);
		 EmServerIPAddress[3] = (U8)atoi(AscTmp);
	}

#ifndef MMI_ON_HARDWARE_P
	soc_init_win32();
#endif


	if(currentSocketAppType==DNS)
		emSocketTmpResult = mmi_soc_demo_app_request(DNS,emSocketAccountId,NULL,emSocketInputdata, strlen(emSocketInputdata),NULL, 0,EmSocketRsp);
	else if(currentSocketAppType==HTTP)
		emSocketTmpResult = mmi_soc_demo_app_request(HTTP,emSocketAccountId,NULL,emSocketInputdata, strlen(emSocketInputdata),NULL, 0,EmSocketRsp);
	else if(currentSocketAppType==ECHO)
		emSocketTmpResult = mmi_soc_demo_app_request(ECHO,emSocketAccountId,(char*)EmServerIPAddress,NULL, 0,emSocketInputdata, strlen(emSocketInputdata),EmSocketRsp);
	else if(currentSocketAppType==DAYTIME)
		emSocketTmpResult = mmi_soc_demo_app_request(DAYTIME,emSocketAccountId,(char*)EmServerIPAddress,NULL, 0,NULL, 0,EmSocketRsp);	
	else
	{
		emSocketTmpResult = EN_SOC_UNKNOWN_APP;
	}


	/* To handle the scenario that  callback function is called before mmi_soc_demo_app_request() returns  
	by checking if value of EmSocketResult is still the same as its default value (EN_SOC_SUCCESS) */
	if (EmSocketResult == EN_SOC_SUCCESS)
		EmSocketResult = emSocketTmpResult;

}

/**************************************************************
**	FUNCTION NAME		: EntryEmSocketInProgress
**
**  	PURPOSE				: Entry function for sending request
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmSocketInProgress(void)
{

	U8 *guiBuffer;
	
	EntryNewScreen( EM_SOCKET_PROGRESS_SCR, NULL, EntryEmSocketInProgress, NULL );

	guiBuffer = GetCurrGuiBuffer(EM_SOCKET_PROGRESS_SCR);	
	if(guiBuffer == NULL)
	{
		mmi_soc_demo_app_request_abort();
		EmSocketSendReq();
	}
	if (EmSocketIsCallbackFuncCalled == FALSE)
	{
		if (EmSocketResult == EN_SOC_SUCCESS)
		{
			// clear End key
			ClearKeyHandler( KEY_END, KEY_EVENT_DOWN );
			ClearKeyHandler( KEY_END, KEY_LONG_PRESS );

			// Show progress screen
			ShowCategory66Screen( EM_SOCKET_TEXT, 0,
				0, 0, STR_GLOBAL_CANCEL, 0, 
				(U8*)GetString( EM_SOCKET_IN_PROGRESS_TEXT ), IMG_GLOBAL_PROGRESS, NULL );

			// Set RSK to abort progress
			SetRightSoftkeyFunction( EmSocketInProgressAbort, KEY_EVENT_UP );
			
			SetKeyHandler(EmSocketInProgressAbort,KEY_END,KEY_EVENT_DOWN);

		}
		else
		{
			EmSocketShowErrorMsg();
		}
	}
	else
	{
		EntryEmSocketResult();
	}

}

/**************************************************************
**	FUNCTION NAME		: EmSocketInProgressAbort
**
**  	PURPOSE				: abort function for sending request
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmSocketInProgressAbort(void)
{
	mmi_soc_demo_app_request_abort();

	if(currentSocketAppType==DNS || currentSocketAppType==HTTP)
	{
		GoBacknHistory (1);
	}
	
	else if(currentSocketAppType==ECHO || currentSocketAppType==DAYTIME)
	{
		GoBackHistory();
	}
	else
		GoBackHistory();
}


/**************************************************************
**	FUNCTION NAME		: EmSocketShowErrorMsg
**
**  	PURPOSE				: Display Error Message and back to Socket test menu
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmSocketShowErrorMsg(void)
{
	switch(EmSocketResult)
	{
	      case EN_SOC_INVAL_PARA:
	      		DisplayPopup((PU8)GetString(EM_SOCKET_ERROR_INVAL_PARA_TEXT),IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
			break;
			
	       case EN_SOC_ERROR:
	      		DisplayPopup((PU8)GetString(STR_GLOBAL_ERROR),IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
			break;

		case EN_SOC_PEER_NOT_REACHABLE:
	      		DisplayPopup((PU8)GetString(EM_SOCKET_ERROR_PEER_NOT_REACHABLE_TEXT),IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
			break;

 		case EN_SOC_PEER_NO_RESPONSE:
	      		DisplayPopup((PU8)GetString(EM_SOCKET_ERROR_PEER_NO_RESPONSE_TEXT),IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
			break;

		case EN_SOC_CNT_RESET_BY_PEER:
	      		DisplayPopup((PU8)GetString(EM_SOCKET_ERROR_CNT_RESET_BY_PEER_TEXT),IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
			break;

		case EN_SOC_BEARER_FAIL:
	      		DisplayPopup((PU8)GetString(EM_SOCKET_ERROR_BEARER_FAIL_TEXT),IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
			break;

		case EN_SOC_UNKNOWN_APP:
	      		DisplayPopup((PU8)GetString(EM_SOCKET_ERROR_UNKNOWN_APP_TEXT),IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
			break;

		case EN_SOC_BUSY:
	      		DisplayPopup((PU8)GetString(EM_SOCKET_ERROR_BUSY_TEXT),IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
			break;

		case EN_SOC_NO_MEMORY:
	      		DisplayPopup((PU8)GetString(EM_SOCKET_ERROR_NO_MEMORY_TEXT),IMG_GLOBAL_ERROR, 0, 1000, ERROR_TONE);
			break;

	      default:
	         ASSERT(0);
       }


	switch(EmSocketResult)
	{
	      case EN_SOC_INVAL_PARA:
	      case EN_SOC_PEER_NOT_REACHABLE:
	      case EN_SOC_PEER_NO_RESPONSE:
	      case EN_SOC_CNT_RESET_BY_PEER:
	      case EN_SOC_BEARER_FAIL:
	      case EN_SOC_BUSY:
	      case EN_SOC_NO_MEMORY:
	      		switch(currentSocketAppType)
			{
				case DNS:
					DeleteUptoScrID(EM_SOCKET_DNS_INPUT_SCR);
					break;
				case HTTP:
					DeleteUptoScrID(EM_SOCKET_HTTP_INPUT_SCR);
					break;
				case ECHO:
					DeleteUptoScrID(EM_SOCKET_ECHO_INPUT_SCR);
					break;
				case DAYTIME:
					DeleteUptoScrID(EM_SOCKET_DATE_INPUT_SCR);
					break;
				default:
					ASSERT(0);
			}
			break;
			
	       case EN_SOC_ERROR:
	       case EN_SOC_UNKNOWN_APP:
	      		DeleteUptoScrID(EM_SOCKET_MENU_SCR);
	       	break;
	      	
	      default:
	         ASSERT(0);
       }

}


/**************************************************************
**	FUNCTION NAME		: EmSocketCheckAmPm
**
**  	PURPOSE				: replace Chinese characters with AM or PM
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmSocketCheckAmPm(char* response)
{
	if (strlen(response) >= 4 )
	{
		if(response[0] == -92 && response[1] == 85 && response[2] == -92 && response[3] == -56)
		{
			response[0] = 32;
			response[1] = 'P';
			response[2] = 'M';
			response[3] = 32;
		}
		else if(response[0] == -92 && response[1] == 87 && response[2] == -92 && response[3] == -56)
		{
			response[0] = 32;
			response[1] = 'A';
			response[2] = 'M';
			response[3] = 32;
		}

	}

}


/**************************************************************
**	FUNCTION NAME		: EmSocketRsp
**
**  	PURPOSE				: call back function for receiving result
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EmSocketRsp (mmi_soc_demo_app_enum app_id_enum, int result,char* response, int response_len)
{

	U16 currScrnID = GetExitScrnID();

	EmSocketResult = result;
	if (EmSocketResult == EN_SOC_SUCCESS)
	{
		EmSocketResultBufferSize = (response_len+1)*ENCODING_LENGTH;
		currentSocketAppType = (mmi_soc_demo_app_enum)app_id_enum;
		if (EmSocketResultBuffer != NULL)
		{
			OslMfree(EmSocketResultBuffer);
			EmSocketResultBuffer = NULL;
		}
		EmSocketResultBuffer = OslMalloc(EmSocketResultBufferSize);

		EmSocketCheckAmPm(response);

		if (EmSocketResultBuffer != NULL)
			AnsiiToUnicodeString(EmSocketResultBuffer, response);
	}
		
	EmSocketIsCallbackFuncCalled = TRUE;


	// prevent that display of result interrupts current screen except for EM_SOCKET_PROGRESS_SCR
	if( currScrnID == EM_SOCKET_PROGRESS_SCR)
		EntryEmSocketResult();
	
}


/**************************************************************
**	FUNCTION NAME		: EntryEmSocketResult
**
**  	PURPOSE				: Entry function for display result
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmSocketResult (void)
{
		U8*	guiBuffer;

		if (EmSocketResult == EN_SOC_SUCCESS)
		{

			EntryNewScreen(EM_SOCKET_RESULT_SCR, NULL, EntryEmSocketResult, NULL);
			guiBuffer = GetCurrGuiBuffer(EM_SOCKET_RESULT_SCR);

			// clear End key
			ClearKeyHandler( KEY_END, KEY_EVENT_DOWN );
			ClearKeyHandler( KEY_END, KEY_LONG_PRESS );

			ShowCategory74Screen( EM_SOCKET_RESULT_TEXT, 0,
										 0, 0,
										 STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
										 (PU8)EmSocketResultBuffer, EmSocketResultBufferSize, guiBuffer);

			/* go back to socket test menu */
			SetRightSoftkeyFunction(GoToMainEmSocketScreen,KEY_EVENT_UP);
			SetKeyHandler(GoToMainEmSocketScreen,KEY_END,KEY_EVENT_DOWN);
		}
		else
		{
			EmSocketShowErrorMsg();
		}	
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmDNSQueryDone
**
**  	PURPOSE				: Highlight function for DNS Done
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmDNSQueryDone(void)
{

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmSocketInProgress,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************
**	FUNCTION NAME		: HighlightEmHTTPGetDone
**
**  	PURPOSE				: Highlight function for HTTP Done
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmHTTPGetDone(void)
{

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmSocketInProgress,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************
**	FUNCTION NAME		: HighlightEmDNSQueryInputMethod
**
**  	PURPOSE				: Highlight function for DNS Input Method
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmDNSQueryInputMethod(void)
{

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryInputMethodScreen, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/**************************************************************
**	FUNCTION NAME		: HighlightEmHTTPGetInputMethod
**
**  	PURPOSE				: Highlight function for HTTP Input Method
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmHTTPGetInputMethod(void)
{

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryInputMethodScreen, KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmHTTPGet
**
**  	PURPOSE				: Highlight function for HTTP Get
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmHTTPGet(void)
{

	currentSocketAppType = HTTP;

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmHTTPGet,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmHTTPGet
**
**  	PURPOSE				: Entry function for EM HTTP get
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmHTTPGet(void)
{

	byte *history_buffer;
	EmSocketInitResult();
	   
	EntryNewScreen(EM_SOCKET_HTTP_INPUT_SCR, NULL, EntryEmHTTPGet, NULL);
	history_buffer=GetCurrGuiBuffer(EM_SOCKET_HTTP_INPUT_SCR);

	if (history_buffer == NULL)
		kal_wsprintf( (U16*)EmSocketInputBuff, "%s", "http://" );

       ShowCategory5Screen( EM_SOCKET_HTTP_URL_TEXT,  0, \
                                          STR_GLOBAL_OPTIONS, 0 , \
                                          STR_GLOBAL_BACK, 0, \
                                          INPUT_TYPE_ALPHANUMERIC_SENTENCECASE, \
                                          (PU8)EmSocketInputBuff, EM_SOCKET_MAX_INPUT_LEN, history_buffer );

   	SetLeftSoftkeyFunction(EntryEmHTTPOption,KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	RegisterInputMethodScreenCloseFunction (EmSocketGoBack2History);
}


/**************************************************************

	FUNCTION NAME		: EmSocketGoBack2History

  	PURPOSE				: Go back two screen history

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void EmSocketGoBack2History (void)
{
	GoBacknHistory (1);
}

/**************************************************************
**	FUNCTION NAME		: EntryEmHTTPOption
**
**  	PURPOSE				: Entry function for EM HTTP GET Option
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmHTTPOption (void)
{
	U8* guiBuffer=NULL;
	U16 numItems=0;
	U16 nStrItemList[32];
	
	EntryNewScreen(EM_SOCKET_HTTP_OPTION_SCR, NULL, EntryEmHTTPOption, NULL);
	
	guiBuffer = GetCurrGuiBuffer (EM_SOCKET_HTTP_OPTION_SCR);		 
	numItems = GetNumOfChild (EM_SOCKET_HTTP_OPTION_MENUID);
	GetSequenceStringIds(EM_SOCKET_HTTP_OPTION_MENUID, nStrItemList);
	SetParentHandler (EM_SOCKET_HTTP_OPTION_MENUID);

	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	
	ShowCategory52Screen (STR_GLOBAL_OPTIONS, 0, 
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  numItems, nStrItemList, (U16 *)gIndexIconsImageList,
						  NULL , 0, 0, guiBuffer );
}


/**************************************************************
**	FUNCTION NAME		: HighlightEmEcho
**
**  	PURPOSE				: Highlight function for Echo
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmEcho(void)
{

	currentSocketAppType = ECHO;

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmEcho,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmEcho
**
**  	PURPOSE				: Entry function for EM Echo
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmEcho(void)
{
	U8* inputBuffer;
	U8* guiBuffer;
	U16 inputBufferSize;
	EmSocketInitResult();
	
	EntryNewScreen(EM_SOCKET_ECHO_INPUT_SCR, ExitEmEcho, NULL, NULL);

	InitializeCategory57Screen();
	guiBuffer = GetCurrGuiBuffer(EM_SOCKET_ECHO_INPUT_SCR);
	inputBuffer = GetCurrNInputBuffer (EM_SOCKET_ECHO_INPUT_SCR, &inputBufferSize );    //added for inline edit history
	EmSocketEchoFillInlineStruct();

	if(inputBuffer != NULL)		
		SetCategory57Data(wgui_inline_items, ENUM_ECHO_TOTAL, inputBuffer);	
	
	ShowCategory57Screen(EM_SOCKET_ECHO_TEXT,  0,
						 STR_GLOBAL_EDIT, 0,
						 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						 ENUM_ECHO_TOTAL,
						 0,
						 wgui_inline_items,
						 0,
						 guiBuffer);
	SetCategory57RightSoftkeyFunctions(EntryEmSocketConfirm,GoBackHistory);
}


/**************************************************************

	FUNCTION NAME		: ExitEmEcho

  	PURPOSE				: Exit from EM Echo

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void ExitEmEcho(void)
{
	history_t	h;
	U16 inputBufferSize;
	CloseCategory57Screen();
	h.scrnID = EM_SOCKET_ECHO_INPUT_SCR;
	h.entryFuncPtr = EntryEmEcho;
	GetCategoryHistory(h.guiBuffer);
	inputBufferSize =(U16)GetCategory57DataSize( ); 
	GetCategory57Data ((U8*) h.inputBuffer );  
	AddNHistory(h, inputBufferSize);           
}

/**************************************************************

	FUNCTION NAME		: EmSocketEchoFillInlineStruct()

  	PURPOSE				: Call the Inline Functions.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void EmSocketEchoFillInlineStruct(void)
{

	memset( EmSocketInputBuff, 0, sizeof(EmSocketInputBuff) );
	
	ReturnThreeDigitString(EmSocketIP1,0);
	ReturnThreeDigitString(EmSocketIP2,0);
	ReturnThreeDigitString(EmSocketIP3,0);
	ReturnThreeDigitString(EmSocketIP4,0);	

	/* server IP caption */
	SetInlineItemActivation(&wgui_inline_items[ENUM_SERVER_IP_CAPTION], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&wgui_inline_items[ENUM_SERVER_IP_CAPTION], (U8*) GetString(EM_SOCKET_ECHO_SERVER_IP_TEXT));
	
	/* server IP  */
	SetInlineItemActivation((&wgui_inline_items[ENUM_SERVER_IP]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemIP4((&wgui_inline_items[ENUM_SERVER_IP]),(U8*)EmSocketIP1,(U8*)EmSocketIP2,(U8*)EmSocketIP3,(U8*)EmSocketIP4, PreparedIPAddressString);

	/* Echo String caption */
	SetInlineItemActivation(&wgui_inline_items[ENUM_ECHO_STRING_CAPTION], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&wgui_inline_items[ENUM_ECHO_STRING_CAPTION], (U8*) GetString(EM_SOCKET_ECHO_STRING_TEXT));
	
	/* Echo String  */
	SetInlineItemActivation((&wgui_inline_items[ENUM_ECHO_STRING]),KEY_LSK,KEY_EVENT_UP);
	SetInlineItemFullScreenEdit((&wgui_inline_items[ENUM_ECHO_STRING]),EM_SOCKET_ECHO_STRING_TEXT, 0,(U8*)EmSocketInputBuff, EM_SOCKET_MAX_INPUT_LEN, INPUT_TYPE_ALPHANUMERIC_SENTENCECASE|INPUT_TYPE_USE_ENCODING_BASED_LENGTH);
	SetInlineFullScreenEditCustomFunction(&wgui_inline_items[ENUM_ECHO_STRING], EmSocketFullLineEditScreen);

}

/**************************************************************

	FUNCTION NAME		: EntryEmEchoConfirm

  	PURPOSE				: Notification Screen.

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void EntryEmSocketConfirm(void )
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
						STR_GLOBAL_NO ,IMG_GLOBAL_NO,
						get_string(STR_GLOBAL_SEND),
						IMG_GLOBAL_QUESTION,WARNING_TONE);
	
	SetLeftSoftkeyFunction(EntryEmSocketInProgress,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoToMainEmSocketScreen,KEY_EVENT_UP);
}

/**************************************************************

	FUNCTION NAME		: GoToMainEmSocketScreen

  	PURPOSE				: Go back to Em Socket test Main

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GoToMainEmSocketScreen(void)
{
	stopRequestedTone(WARNING_TONE);
	
	GoBackToHistory(EM_SOCKET_MENU_SCR);
}


/**************************************************************

	FUNCTION NAME		: EmSocketFullLineEditScreen()

  	PURPOSE				: EM Socket test for Full Line Edit Screen.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void EmSocketFullLineEditScreen(void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS);

	SetLeftSoftkeyFunction (EntryScrForInputMethodAndDone, KEY_EVENT_UP);
}

/**************************************************************
**	FUNCTION NAME		: HighlightEmDate
**
**  	PURPOSE				: Highlight function for Date
**
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void HighlightEmDate(void)
{

	currentSocketAppType = DAYTIME;

	/* Register function for left soft key */
	SetLeftSoftkeyFunction(EntryEmDate,KEY_EVENT_UP);

	/* Register function for right soft key */
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}


/**************************************************************
**	FUNCTION NAME		: EntryEmDate
**
**  	PURPOSE				: Entry function for EM Date
**	INPUT PARAMETERS	: nil
**
**	OUTPUT PARAMETERS	: void
**
**	RETURNS				: void
**
 
**************************************************************/
void EntryEmDate(void)
{
	U8* inputBuffer;
	U8* guiBuffer;
	U16 inputBufferSize;
	EmSocketInitResult();
	
	EntryNewScreen(EM_SOCKET_DATE_INPUT_SCR, ExitEmDate, NULL, NULL);

	InitializeCategory57Screen();
	guiBuffer = GetCurrGuiBuffer(EM_SOCKET_DATE_INPUT_SCR);
	inputBuffer = GetCurrNInputBuffer (EM_SOCKET_DATE_INPUT_SCR, &inputBufferSize );    //added for inline edit history
	EmSocketDateFillInlineStruct();

	if(inputBuffer != NULL)		
		SetCategory57Data(wgui_inline_items, ENUM_DATE_TOTAL, inputBuffer);	
	
	ShowCategory57Screen(EM_SOCKET_DATE_TEXT,  0,
						 STR_GLOBAL_EDIT, 0,
						 STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						 ENUM_DATE_TOTAL,
						 0,
						 wgui_inline_items,
						 0,
						 guiBuffer);
	SetCategory57RightSoftkeyFunctions(EntryEmSocketConfirm,GoBackHistory);
}


/**************************************************************

	FUNCTION NAME		: ExitEmDate

  	PURPOSE				: Exit from EM Date

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void ExitEmDate(void)
{
	history_t	h;
	U16 inputBufferSize;
	CloseCategory57Screen();
	h.scrnID = EM_SOCKET_DATE_INPUT_SCR;
	h.entryFuncPtr = EntryEmDate;
	GetCategoryHistory(h.guiBuffer);
	inputBufferSize =(U16)GetCategory57DataSize( ); 
	GetCategory57Data ((U8*) h.inputBuffer );  
	AddNHistory(h, inputBufferSize);           
}



/**************************************************************

	FUNCTION NAME		: EmSocketDateFillInlineStruct()

  	PURPOSE				: Call the Inline Functions.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void EmSocketDateFillInlineStruct(void)
{

	ReturnThreeDigitString(EmSocketIP1,0);
	ReturnThreeDigitString(EmSocketIP2,0);
	ReturnThreeDigitString(EmSocketIP3,0);
	ReturnThreeDigitString(EmSocketIP4,0);
	

	/* server IP caption */
	SetInlineItemActivation(&wgui_inline_items[ENUM_DATE_SERVER_IP_CAPTION], INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT, 0);
	SetInlineItemCaption(&wgui_inline_items[ENUM_DATE_SERVER_IP_CAPTION], (U8*) GetString(EM_SOCKET_ECHO_SERVER_IP_TEXT));
	
	/* server IP  */
	SetInlineItemActivation((&wgui_inline_items[ENUM_DATE_SERVER_IP]),INLINE_ITEM_ACTIVATE_WITHOUT_KEY_EVENT,0);
	SetInlineItemIP4((&wgui_inline_items[ENUM_DATE_SERVER_IP]),(U8*)EmSocketIP1,(U8*)EmSocketIP2,(U8*)EmSocketIP3,(U8*)EmSocketIP4, PreparedIPAddressString);

}

#endif /* __TCPIP__ */

#if defined(AUTO_TEST_KEYPRESS_RECORD_SUPPORT)
#include "filemgr.h"
#include "conversions.h"
#include"audioplayerdef.h"

#define MAX_AUTO_TEST_LIST_NUM	1024
#define AUTO_TEST_LIST_RECORD_SIZE ((FMGR_MAX_PATH_LEN+1)*ENCODING_LENGTH)
#define AUTO_TEST_LIST_BUFF_SIZE	8
#define MAX_AUTO_TEST_LIST_BYTES	(MAX_AUTO_TEST_LIST_NUM/8)
#define EOS(nob) ((nob)>1 ? ((nob)>>1)-1: 0)
#define ClrBitSet( index, bitset )		((bitset)[(index)>>3] &= ~(1<<((index)&0x07)))
#define SetBitSet( index, bitset )		((bitset)[(index)>>3] |= (1<<((index)&0x07)))
#define IsBitSetOn( index, bitset )		((bitset)[(index)>>3] & (1<<((index)&0x07)))
#define IS_SNAME(path) ((path)[0] == _T_MMI'S')

const UI_character_type * const autotest_file_restorable_upper_exts[] = {

	(UI_character_type*)	L"TXT",
	(UI_character_type*)	L"\0"	// end of list
};
const UI_character_type * const autotest_file_restorable_lower_exts[] = {
	(UI_character_type*)	L"txt",
	(UI_character_type*)	L"\0"	// end of list
};
#if defined(__MMI_CHSET_BIG5__)
mmi_chset_enum g_text_encoding = MMI_CHSET_BIG5;
#elif defined(__MMI_CHSET_GB2312__)
mmi_chset_enum g_text_encoding = MMI_CHSET_GB2312;
#else 
mmi_chset_enum g_text_encoding = MMI_CHSET_UTF8;
#endif 

typedef struct
{
	S32 n_total_items;
	S32 pick_index;
	S32 pick_count;
	U8 bitset[MAX_AUTO_TEST_LIST_BYTES];
	
	// settings
	U8 list_auto_gen;
	U8 preferred_list;
	U8 skin;
	U8 repeat_mode; // 0: off, 1: repeat one, 2: repeat all
	U8 random_mode; // 0: off, 1: on
	U8 background_play; // o: off, 1: on
	
	// cmd & states
	UI_character_type title[FMGR_MAX_FILE_LEN+1];
	UI_character_type filefullname[FMGR_MAX_PATH_LEN+1];
	U8 state;
	BOOL restorable;
	BOOL need_restore;
	BOOL wait_next;
	BOOL need_replay;
	BOOL in_main_screen;
	BOOL in_list_screen;
	U8 volume;
	
	// buffer for dynamic load
	S32	buff_head;
	S32	n_buff_items;
	//UI_character_type buff_filename[PLAY_LIST_BUFF_SIZE][FMGR_MAX_FILE_LEN+1];
	UI_character_type buff_filename[AUTO_TEST_LIST_BUFF_SIZE][FMGR_MAX_PATH_LEN+1];  //by liwh
	
	// duration
	U32 const_seconds;
	U32	play_seconds;
	kal_uint32 play_time;

	// present play list
	U8 present_list;
	BOOL is_subLCD_shown;
} autotest_struct;
autotest_struct g_autotest;

U16 g_mmi_autotest_list_highlighted_item;
U16 g_autotest_list_play_fg;
U16 g_autotest_list_playall_fg;

#ifdef MMI_ON_WIN32
	FILE *gT_fp = NULL;
#else
	FS_HANDLE gT_fp = -1;
#endif
extern PUBLIC UINT32 hal_TimGetUpTime(VOID);
extern void SetAutoPwrONdata(void);
#define TICK1S 14
#define TICK500MS 13
#define TICK250MS 12


#define TIMER_SPACE 500
#define TIMER_SPACE_Write TICK500MS


U32 WriteTimeSpaceTick = 0;

BOOL OpenAutoTest(U16 Write_flag);
void WriteAutoTestTimeSpace(void);
void StopAutoTestCases(void);
void WriteAutoTestCases(U16 value);
void CloseRecordAutoTest(void);
void mmi_autotest_play_all_next();

#define AT_LOOP_A 0xfffa
#define AT_LOOP_B 0xfffb 

#define AT_TIME_INSERT 0xfffc
#define AT_TIME_LOOP_MAX 0xfff0

#define AT_KEY_EVENT_INSERT 0xfffe
#define AT_TEST_END 0xfffd  
#define AT_TEST_RESTART_END 0xffff


U8 start_write_AT = 0;
U8 start_run_AT = 0;
INT32 write_len = 0;
INT32 read_len = 0;
INT32 Old_read_len = 0;
#define READ_VALUE_MAX (512)

U16 *g_ReadValue = NULL;
U16 readindex = 0;
U16 TimeSpace_num = 0;
U16 loop_flg = 1;
U16 loop_A_index = 0;
U16 loop_B_index = 0;
U16 wait_index = 0;
U16 fastrunflag = 0;
U16 pauserunflag = 0;
U16 OverTimeFlag = 0;
U16 OverTimeAdd = 0;

U16 run_num = 0;

UI_character_type AT_fullfilename_temp[FMGR_MAX_PATH_LEN+1];

void WriteATValue(U16 W_index , U16 Value)
{
	U16 *WValue = NULL;
	WValue = g_ReadValue + W_index;
	*WValue = Value;
}
U16 ReadATValue(U16 read_index)
{
	U16 *RValue = NULL;
	RValue = g_ReadValue + read_index;
	return (*RValue);
}

void WriteAutoTestCasesFile(void)
{
INT32 writelen = 0;
INT32 i;
#ifdef MMI_ON_WIN32
if(TRUE == OpenAutoTest(1))
	{
		fwrite(g_ReadValue,readindex*2,1,gT_fp);
		fclose(gT_fp);
		readindex = 0;
		gT_fp = NULL;
		memset(g_ReadValue, 0, READ_VALUE_MAX);

	}
#else
{
	if(TRUE == OpenAutoTest(0))
	{
		MMI_FS_Seek(gT_fp, 0, FS_FILE_END);
		MMI_FS_Write(gT_fp,g_ReadValue,readindex*2,&writelen);
		MMI_FS_Close(gT_fp);
		gT_fp = -1;
		for(i = 0;i< READ_VALUE_MAX;i++)
		{
		mmi_trace(g_sw_TPL,"000 g_ReadValue %d  [%d]****************",i,g_ReadValue[i]);
		}
		mmi_trace(g_sw_TPL,"000 read_len =  %d  readlen = %d****************",write_len,writelen);
		readindex = 0;
		memset(g_ReadValue, 0, READ_VALUE_MAX);
	}
	else
		{
			WriteAutoTestCases(AT_TEST_END);
			StopTimer(PEN_HANDLE_TIMER);
				write_len = 0;
				start_write_AT = 0;
				readindex = 0;
				gT_fp = -1;
				if(g_ReadValue)
					{
					OslMfree(g_ReadValue);
					g_ReadValue = NULL;
					}
			DisplayPopup((U8*)GetString(STR_GLOBAL_ERROR),IMG_GLOBAL_WARNING,0,1000,1);
		}
}
#endif
}


BOOL ReadAutoTestCasesFile(void)
{
INT32 readlen = 0;
//INT32 i;
if(TRUE == OpenAutoTest(0))
			{
				
				memset(g_ReadValue, 0, READ_VALUE_MAX);
				#ifdef MMI_ON_WIN32
				fseek(gT_fp,read_len,SEEK_SET);
				Old_read_len = read_len;
				fread(g_ReadValue,READ_VALUE_MAX*2,1,gT_fp);
				read_len = ftell(gT_fp);
				fclose(gT_fp);
                            gT_fp = NULL;
				#else
				MMI_FS_Seek(gT_fp, read_len, FS_FILE_BEGIN);
				Old_read_len = read_len;
				MMI_FS_Read(gT_fp,g_ReadValue,READ_VALUE_MAX*2,(INT32*)&readlen);
				read_len = MMI_FS_Seek(gT_fp, 0, FS_FILE_CURRENT);
				#if 0
				for(i = 0;i< READ_VALUE_MAX;i++)
					{
					mmi_trace(g_sw_TPL,"g_ReadValue %d  [%d]****************",i,g_ReadValue[i]);
					}
					mmi_trace(g_sw_TPL,"read_len =  %d  readlen = %d****************",read_len,readlen);
				#endif
				MMI_FS_Close(gT_fp);
                            gT_fp = -1;
			       #endif
				readindex=0;
				return TRUE;
			}
else
{
	DisplayPopup((U8*)GetString(STR_GLOBAL_ERROR),IMG_GLOBAL_WARNING,0,1000,1);
	StopAutoTestCases();
	mmi_trace(g_sw_TPL,"ReadAutoTestCasesFile   open autotest.txt fail !!! ");
	return FALSE;
}
}

void WriteAutoTestCases(U16 value)
{ 
U32 CurrentTick = 0;
	if(0 == start_write_AT)
		return;

if(value == AT_TIME_INSERT)
{
#ifndef MMI_ON_WIN32
	CurrentTick = hal_TimGetUpTime();
	TimeSpace_num = (U16)((CurrentTick - WriteTimeSpaceTick)>>TIMER_SPACE_Write);
	WriteTimeSpaceTick = hal_TimGetUpTime();
#endif
if(readindex)
{
	if(( ReadATValue(readindex - 1)  == AT_LOOP_A)||( ReadATValue(readindex - 1)  == AT_LOOP_B))
	{
		TimeSpace_num = 0;
#ifdef MMI_ON_WIN32
		StopTimer(PEN_HANDLE_TIMER);
		StartTimer(PEN_HANDLE_TIMER, TIMER_SPACE, WriteAutoTestTimeSpace);
#endif
		return;
	}
}
if(TimeSpace_num != 0)
{
	WriteATValue(readindex,AT_TIME_INSERT);
	 readindex++;
	 if(readindex >= READ_VALUE_MAX)
	  	{
	  		WriteAutoTestCasesFile();
	  	}
	WriteATValue(readindex,TimeSpace_num);
	TimeSpace_num = 0;
	 readindex++;
	 if(readindex >= READ_VALUE_MAX)
	  	{
	  		WriteAutoTestCasesFile();
	  	}
}
else
{
	TimeSpace_num = 0;
#ifdef MMI_ON_WIN32
	StopTimer(PEN_HANDLE_TIMER);
	StartTimer(PEN_HANDLE_TIMER, TIMER_SPACE, WriteAutoTestTimeSpace);
#endif
}
}
else
{
		  WriteATValue(readindex,value);
	  readindex++;
	  if((value == AT_LOOP_A)||(value == AT_LOOP_B))
	  	{
	  		TimeSpace_num = 0;
#ifdef MMI_ON_WIN32
			StopTimer(PEN_HANDLE_TIMER);
			StartTimer(PEN_HANDLE_TIMER, TIMER_SPACE, WriteAutoTestTimeSpace);
#else
			WriteTimeSpaceTick = hal_TimGetUpTime();
#endif
	  	}
	  if(readindex >= READ_VALUE_MAX)
	  	{
	  		WriteAutoTestCasesFile();
	  	}
}

}

void Loop_A_to_B(void)
{
if(0 == start_write_AT)
		return;

	if(loop_flg)
		{
			WriteAutoTestCases(AT_LOOP_A);
			loop_flg = 0;
			mmi_trace(g_sw_TPL,"Loop_A *************************************");
	//		DisplayPopup((U8*)GetString(STR_ID_FM_AUTO_TEST_START),IMG_GLOBAL_WARNING,0,1000,(U8)ERROR_TONE);
		}
	else
		{
			WriteAutoTestCases(AT_LOOP_B);
			loop_flg = 1;
	//		DisplayPopup((U8*)GetString(STR_ID_FM_AUTO_TEST_STOP),IMG_GLOBAL_WARNING,0,1000,(U8)ERROR_TONE);
			mmi_trace(g_sw_TPL,"Loop_B *************************************");
		}
}

void WriteAutoTestTimeSpace(void)
{
	if(TimeSpace_num >= AT_TIME_LOOP_MAX)
		{
			WriteAutoTestCases(AT_TIME_INSERT);
		}
	else
		{
			TimeSpace_num++;
		}
	StartTimer(PEN_HANDLE_TIMER, TIMER_SPACE, WriteAutoTestTimeSpace);
}
BOOL OpenAutoTest(U16 Write_flag)
{
	UINT8 fileName[128]; 
	UINT8 tempfileName[256];
	S32	ret_drv;
#ifdef MMI_ON_WIN32
if(g_autotest_list_play_fg)
{
if((gT_fp = fopen(g_autotest.filefullname, "a+")) == NULL)
	{
		return FALSE;
	}
}
else
{
	strcpy(fileName, "D:\\autotest.txt");
	if((gT_fp = fopen(fileName, "a+")) == NULL)
	{
		return FALSE;
	}
}
#else
ret_drv	= MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE , 1, FS_NO_ALT_DRIVE);

		if(ret_drv >=0)
		{
		if(g_autotest_list_play_fg)
			{
				gT_fp = MMI_FS_Open((UINT8*)g_autotest.filefullname, FS_READ_WRITE);
			}
		else
			{
				strcpy(fileName, "D:/autotest.txt");
				AnsiiToUnicodeString(tempfileName,fileName);
				if(Write_flag)
				{
				gT_fp = MMI_FS_Open((UINT8*)tempfileName, FS_CREATE_ALWAYS);
				}
				else
				{
				gT_fp = MMI_FS_Open((UINT8*)tempfileName, FS_READ_WRITE);
				}

				
			}
		if (gT_fp < 0)
			return FALSE;
		}
	

#endif
	return TRUE;
}


void RunAutoTestTimeCasesWaitEvent(void)
{
S16 temp1 = 0;
S16 temp2 = 0;

		if(AT_KEY_EVENT_INSERT == ReadATValue(readindex))
			{
			 wait_index = Old_read_len + readindex*2;
			 readindex++;
			 if(readindex >= READ_VALUE_MAX )
				{
				if(!ReadAutoTestCasesFile())
					{
						return;
					}
				}
			 temp1 = ReadATValue(readindex);
			 readindex++;
			 if(readindex >= READ_VALUE_MAX )
				{
					if(!ReadAutoTestCasesFile())
					{
						return;
					}
				}
			  temp2 = ReadATValue(readindex);
				
			  readindex++;
			if(readindex >= READ_VALUE_MAX )
				{
					if(!ReadAutoTestCasesFile())
					{
						return;
					}
				}
		
			if(GetActiveScreenId() == ReadATValue(readindex))
				{
				wait_index = 0;
				if(OverTimeFlag)
					{
							OverTimeAdd = 0;
					}
				TurnOnBacklight(1);
				if((KEY_EVENT_LONG_PRESS == temp2)&&(KEY_END == temp1))
					{
						SetAutoPwrONdata();
					}
				 ExecuteCurrKeyHandler(temp1,temp2);
				}
			else
				{
				if(readindex<3)
				{
				read_len = wait_index;
				if(!ReadAutoTestCasesFile())
					{
						return;
					}
				}
				else
					{
						readindex = readindex - 3;
					}
				if(OverTimeFlag)
					{
							OverTimeAdd++;
							if(OverTimeAdd > 2000)
								{
									U8 temp_char[100];
									readindex=0;
									loop_A_index = 0;
									loop_B_index = 0;
									read_len = 0;
									fastrunflag = 0;
									pauserunflag = 1;
									OverTimeAdd = 0;
									if(!ReadAutoTestCasesFile())
										{
											return;
										}
									ExecuteCurrKeyHandler(KEY_END,KEY_EVENT_DOWN);
									ExecuteCurrKeyHandler(KEY_END,KEY_EVENT_UP);
									AnsiiToUnicodeString(temp_char,"over time error,wait more then 10 mm");
									DisplayPopup((U8*)temp_char,IMG_GLOBAL_WARNING,0,1000,1);
									mmi_trace(g_sw_TPL,"over time error,wait more then 60s    **********");
									StartTimer(PEN_HANDLE_TIMER, 300, RunAutoTestTimeCasesWaitEvent);
								}
							else
								{
									StartTimer(PEN_HANDLE_TIMER, 300, RunAutoTestTimeCasesWaitEvent);
								}
					}
				else
					{
						StartTimer(PEN_HANDLE_TIMER, 300, RunAutoTestTimeCasesWaitEvent);
					}
				return;
				}

			}
		else if(AT_LOOP_A == ReadATValue(readindex))
			{
				loop_A_index = Old_read_len + readindex*2;
			}
		else if(AT_LOOP_B == ReadATValue(readindex))
			{
				loop_B_index = readindex;
				read_len = loop_A_index;
				if(!ReadAutoTestCasesFile())
					{
						return;
					}
				run_num++;
				mmi_trace(g_sw_TPL,"*****  run times  run_num = %d *****",run_num);
				StartTimer(PEN_HANDLE_TIMER, 10, RunAutoTestTimeCasesWaitEvent);
				return;
			}
		
		if(AT_TIME_INSERT == ReadATValue(readindex))
			{
			readindex++;
			
			if(readindex >= READ_VALUE_MAX )
				{
					if(!ReadAutoTestCasesFile())
					{
						return;
					}
				}
			if(fastrunflag)
				{
					StartTimer(PEN_HANDLE_TIMER, 10, RunAutoTestTimeCasesWaitEvent);
				}
			else
				{
					StartTimer(PEN_HANDLE_TIMER, TIMER_SPACE*ReadATValue(readindex), RunAutoTestTimeCasesWaitEvent);
				}
			}
		else
			{
			if(AT_TEST_END == ReadATValue(readindex))
				{
					read_len = 0;
					if(!ReadAutoTestCasesFile())
					{
						return;
					}
					ExecuteCurrKeyHandler(KEY_END,KEY_EVENT_DOWN);
					ExecuteCurrKeyHandler(KEY_END,KEY_EVENT_UP);
					//DisplayIdleScreen();
					run_num++;
					mmi_trace(g_sw_TPL,"*****  run times  run_num = %d *****",run_num);

					if(g_autotest_list_playall_fg)
						{
							mmi_autotest_play_all_next();
						}
					StartTimer(PEN_HANDLE_TIMER, 10, RunAutoTestTimeCasesWaitEvent);
					return;
				}
			StartTimer(PEN_HANDLE_TIMER, 10, RunAutoTestTimeCasesWaitEvent);
			}
		readindex++;
		
		if(readindex >= READ_VALUE_MAX )
				{
					if(!ReadAutoTestCasesFile())
					{
						return;
					}
				}
		
}



void RunAutoTestTimeCases(void)
{
S16 temp1 = 0;
S16 temp2 = 0;

		if(AT_KEY_EVENT_INSERT == ReadATValue(readindex))
			{
			 readindex++;
			 if(readindex >= READ_VALUE_MAX )
				{
					if(!ReadAutoTestCasesFile())
					{
						return;
					}
				}
			 temp1 = ReadATValue(readindex);
			 readindex++;
			 if(readindex >= READ_VALUE_MAX )
				{
					if(!ReadAutoTestCasesFile())
					{
						return;
					}
				}
			  temp2 = ReadATValue(readindex);

			TurnOnBacklight(1);
			ExecuteCurrKeyHandler(temp1,temp2);
			  readindex++;
			if(readindex >= READ_VALUE_MAX )
				{
					if(!ReadAutoTestCasesFile())
					{
						return;
					}
				}
			}
		else if(AT_LOOP_A == ReadATValue(readindex))
			{
				loop_A_index = Old_read_len + readindex*2;
			}
		else if(AT_LOOP_B == ReadATValue(readindex))
			{
				loop_B_index = readindex;
				read_len =loop_A_index ;
				readindex= 0;
				if(!ReadAutoTestCasesFile())
					{
						return;
					}
				run_num++;
				mmi_trace(g_sw_TPL,"*****  run times  run_num = %d *****",run_num);
				StartTimer(PEN_HANDLE_TIMER, 10, RunAutoTestTimeCases);
				return;
			}
		
		if(AT_TIME_INSERT == ReadATValue(readindex))
			{
			readindex++;
			
			if(readindex >= READ_VALUE_MAX )
				{
					if(!ReadAutoTestCasesFile())
					{
						return;
					}
				}
			StartTimer(PEN_HANDLE_TIMER, TIMER_SPACE*ReadATValue(readindex), RunAutoTestTimeCases);
			}
		else
			{
			if(AT_TEST_END == ReadATValue(readindex))
				{
					read_len = 0;
					if(!ReadAutoTestCasesFile())
					{
						return;
					}
					ExecuteCurrKeyHandler(KEY_END,KEY_EVENT_DOWN);
					ExecuteCurrKeyHandler(KEY_END,KEY_EVENT_UP);
					//DisplayIdleScreen();
					run_num++;
					mmi_trace(g_sw_TPL,"*****  run times  run_num = %d *****",run_num);
					StartTimer(PEN_HANDLE_TIMER, 10, RunAutoTestTimeCases);
					return;
				}
			StartTimer(PEN_HANDLE_TIMER, 10, RunAutoTestTimeCases);
			}
		readindex++;
		
		if(readindex >= READ_VALUE_MAX )
				{
					if(!ReadAutoTestCasesFile())
					{
						return;
					}
				}
		
}

void StopAutoTestCases(void)
{
if(start_run_AT == 0)
	return;
g_autotest_list_play_fg = 0;
g_autotest_list_playall_fg = 0;
	StopTimer(PEN_HANDLE_TIMER);
	start_run_AT = 0;
	if(g_ReadValue)
		{
		OslMfree(g_ReadValue);
		g_ReadValue = NULL;
		}
{
	U8 temp_char[100];
	AnsiiToUnicodeString(temp_char,"stop run auto test");
	DisplayPopup((U8*)temp_char,IMG_GLOBAL_WARNING,0,1000,1);
}
}

void StartRunAutoTestCasesInTime(void)
{
	if(start_write_AT)
		{
		CloseRecordAutoTest();
		}
	if(start_run_AT == 0)
	{
	U8 temp_char[100];
	AnsiiToUnicodeString(temp_char,"start run auto test");
	start_run_AT = 1;
	readindex=0;
	loop_A_index = 0;
	loop_B_index = 0;
	read_len = 0;
	if(NULL == g_ReadValue)
		g_ReadValue = (U16*)OslMalloc(READ_VALUE_MAX*2);
	
	
	if(!ReadAutoTestCasesFile())
					{
						return;
					}
	DisplayIdleScreen();
	DisplayPopup((U8*)temp_char,IMG_GLOBAL_WARNING,0,1000,1);
	StartTimer(PEN_HANDLE_TIMER, 1000, RunAutoTestTimeCases);
	run_num = 0;
	}
}
void StartRunAutoTestCasesWaitEvents(void)
{
	if(start_write_AT)
		{
		CloseRecordAutoTest();
		}
	if(start_run_AT == 0)
	{
	U8 temp_char[100];
	AnsiiToUnicodeString(temp_char,"start run auto test");
	start_run_AT = 1;
	readindex=0;
	loop_A_index = 0;
	loop_B_index = 0;
	read_len = 0;
	if(NULL == g_ReadValue)
		g_ReadValue = (U16*)OslMalloc(READ_VALUE_MAX*2);
	
	
	fastrunflag = 0;
	pauserunflag = 1;
	OverTimeFlag = 0;
	if(!ReadAutoTestCasesFile())
					{
						return;
					}
	DisplayIdleScreen();
	DisplayPopup((U8*)temp_char,IMG_GLOBAL_WARNING,0,1000,1);
	StartTimer(PEN_HANDLE_TIMER, 1000, RunAutoTestTimeCasesWaitEvent);
	run_num = 0;
	}
}
void StartRunAutoTestCasesWaitEventsWithTimerOut(void)
{
       OverTimeAdd = 0;
	StartRunAutoTestCasesWaitEvents();
	OverTimeFlag = 1;
	run_num = 0;
}
void OpenRecordAutoTest(void)
	{
	if(start_run_AT)
	{
	StopAutoTestCases();
	}
	if(0 == start_write_AT)
	{
	U8 temp_char[100];
	AnsiiToUnicodeString(temp_char,"start record auto test");
	if(TRUE == OpenAutoTest(1))
	{
#ifdef MMI_ON_WIN32
	fclose(gT_fp);
	gT_fp = NULL;
#else
	MMI_FS_Close(gT_fp);
	gT_fp = -1;
#endif
	if(NULL == g_ReadValue)
		g_ReadValue = (U16*)OslMalloc(READ_VALUE_MAX*2);
	memset(g_ReadValue, 0, READ_VALUE_MAX);
	
	readindex = 0;
	loop_flg = 1;
	write_len = 0;
	DisplayIdleScreen();
		
	       DisplayPopup((U8*)temp_char,IMG_GLOBAL_WARNING,0,1000,1);
#ifdef MMI_ON_WIN32
	       StartTimer(PEN_HANDLE_TIMER, 1000, WriteAutoTestTimeSpace);
#else
	 WriteTimeSpaceTick = hal_TimGetUpTime();
#endif
	 start_write_AT = 1;
	}
	else
		{
		DisplayPopup((U8*)GetString(STR_GLOBAL_ERROR),IMG_GLOBAL_WARNING,0,1000,1);
		}
	}
}

void CloseRecordAutoTest(void)
{
if(0 == start_write_AT)
		return;

WriteAutoTestCases(AT_TEST_END);
#ifdef MMI_ON_WIN32
StopTimer(PEN_HANDLE_TIMER);
#endif
	
	WriteAutoTestCasesFile();
	write_len = 0;
	start_write_AT = 0;
	readindex = 0;
	gT_fp = -1;
	if(g_ReadValue)
		{
		OslMfree(g_ReadValue);
		g_ReadValue = NULL;
		}
{
		U8 temp_char[100];
		AnsiiToUnicodeString(temp_char,"stop record auto test");
		DisplayPopup((U8*)temp_char,IMG_GLOBAL_WARNING,0,1000,1);
}

}


void CloseRecordAutoTestAndReStartPhone(void)
{
if(0 == start_write_AT)
		return;
WriteAutoTestCases(AT_TEST_RESTART_END);
#ifdef MMI_ON_WIN32
StopTimer(PEN_HANDLE_TIMER);
#endif

if(readindex)	
	WriteAutoTestCasesFile();

	write_len = 0;
	start_write_AT = 0;
	readindex = 0;
	gT_fp = -1;
	if(g_ReadValue)
		{
		OslMfree(g_ReadValue);
		g_ReadValue = NULL;
		}
{
		U8 temp_char[100];
		AnsiiToUnicodeString(temp_char,"stop record and restart later!!!");
		DisplayPopup((U8*)temp_char,IMG_GLOBAL_WARNING,0,1000,1);
}
                                                                                                  
}

void CheckWhetherNeedAutoRunTest(void)
{
	INT32 readlen = 0;
	U16 ReadValue = 0;
	if(TRUE == OpenAutoTest(0))
	{
		if(NULL == g_ReadValue)
			g_ReadValue = (U16*)OslMalloc(READ_VALUE_MAX*2);

	      if(gT_fp >= 0)
	      	{
			MMI_FS_Seek(gT_fp, -2, FS_FILE_END);
			MMI_FS_Read(gT_fp,&ReadValue,2,(INT32*)&readlen);
			MMI_FS_Close(gT_fp);
                     gT_fp = -1; 
	      	}
		
		if(AT_TEST_RESTART_END == ReadValue)
		{
			StartTimer(PEN_HANDLE_TIMER, 1000, StartRunAutoTestCasesWaitEventsWithTimerOut);
		}

		if(g_ReadValue)
		{
			OslMfree(g_ReadValue);
			g_ReadValue = NULL;
		}

	}
}
void RecordCurrKeyHandler(S16 keyCode, S16 keyType)
{
if(start_run_AT)
{
   if((KEY_EVENT_LONG_PRESS == keyType)&&(KEY_STAR == keyCode))
   	{
	StopAutoTestCases();
   	}
   else if((KEY_EVENT_DOWN == keyType)&&(KEY_STAR == keyCode))
   	{
   	if(pauserunflag)
   		{
   			StopTimer(PEN_HANDLE_TIMER);
   			pauserunflag = 0;
   		}
	else
		{
			StartTimer(PEN_HANDLE_TIMER, 1000, RunAutoTestTimeCasesWaitEvent);
			pauserunflag = 1;
		}
   	}
   else if((KEY_EVENT_DOWN == keyType)&&(KEY_POUND == keyCode))
   	{
   	if(fastrunflag)
   		{
   			fastrunflag = 0;
   		}
	else
		{
			fastrunflag = 1;
		}
   	}
}
if(start_write_AT)
{
   if((KEY_EVENT_LONG_PRESS == keyType)&&(KEY_STAR == keyCode))
   	{
	CloseRecordAutoTest();
	StopAutoTestCases();
   	}
   else if((KEY_EVENT_LONG_PRESS == keyType)&&(KEY_POUND == keyCode))
   	{
   	Loop_A_to_B();
   	}
   else if((KEY_EVENT_LONG_PRESS == keyType)&&(KEY_END == keyCode))
   	{
   	WriteAutoTestCases(AT_TIME_INSERT);
   	WriteAutoTestCases(AT_KEY_EVENT_INSERT);
   	WriteAutoTestCases(keyCode);
	WriteAutoTestCases(keyType);
	WriteAutoTestCases(GetActiveScreenId());
	CloseRecordAutoTestAndReStartPhone();
	SetAutoPwrONdata();
   	}
   else
   	{
   	WriteAutoTestCases(AT_TIME_INSERT);
   	WriteAutoTestCases(AT_KEY_EVENT_INSERT);
   	WriteAutoTestCases(keyCode);
	WriteAutoTestCases(keyType);
	WriteAutoTestCases(GetActiveScreenId());
   	}
}
}



U8 mmi_autotest_playlist_get_fullfilename( UI_string_type fullfilename, S32 index )
{
	U8 result = 0;
	INT32 size;
	S8 autotestlist[20] = "D:/AutoTestList.txt";
	S8 uautotestlist[40];
	FS_HANDLE fd;
	AnsiiToUnicodeString(uautotestlist,autotestlist);
	 fd = MMI_FS_Open( (const UINT8 *)uautotestlist, FS_READ_ONLY );
	
	if( fd >= 0 )
	{
		if( MMI_FS_Seek( fd, index * AUTO_TEST_LIST_RECORD_SIZE, FS_FILE_BEGIN ) >= 0 )
		{
			if( MMI_FS_Read( fd, fullfilename, AUTO_TEST_LIST_RECORD_SIZE, (INT32*)&size ) == FS_NO_ERROR )
			{
				if( size > 0 )
				{
					result = IS_SNAME( fullfilename ) ? 2: 1;
					 
					fullfilename[EOS(size)] = 0; // make it must be in boundary
				}
			}
		}
		MMI_FS_Close( fd );
	}
	
	if( !result ) fullfilename[0] = 0;
	
	return result;
}



void mmi_autotest_start_playlist_at( S32 index )
{
	memset( g_autotest.bitset, 0, MAX_AUTO_TEST_LIST_BYTES );
	g_autotest.pick_index = index;
	g_autotest.pick_count = 0;

}

S32 mmi_autotest_pick_playlist_next()
{
	S32 i;
	if( g_autotest.n_total_items == 0 )
		return -1;

	if( g_autotest.random_mode )
	{
		if( g_autotest.n_total_items == g_autotest.pick_count )
			mmi_autotest_start_playlist_at( -1 );

		// randomly pick next item
		i = (g_autotest.pick_index + (rand() % g_autotest.n_total_items) + 1) % g_autotest.n_total_items;
		while( IsBitSetOn( i, g_autotest.bitset ) )
			i = (i + 1) % g_autotest.n_total_items;
		g_autotest.pick_index = i;
		g_autotest.pick_count++;
	}
	else
	{
		if( g_autotest.pick_index == g_autotest.n_total_items - 1 )
			mmi_autotest_start_playlist_at( -1 );

		// sequentially pick next item
		g_autotest.pick_index++;
		if( !IsBitSetOn( g_autotest.pick_index, g_autotest.bitset ) )
			g_autotest.pick_count++;
	}
	SetBitSet( g_autotest.pick_index, g_autotest.bitset );
	
	return g_autotest.pick_index;
}

UI_string_type ATUCS2StrLastChr( UI_string_type ustring1, UI_character_type uchar2 )
{
	UI_string_type result;
	for( result = NULL; *ustring1; ustring1++ )
	{
		if( *ustring1 == uchar2 )
			result = ustring1;
	}
	return result;
}

void mmi_autotest_extract_filename( UI_string_type filename, UI_string_type fullfilename )
{
	UI_string_type ch;
	UINT16 s = 0x002F;
	ch = ATUCS2StrLastChr( fullfilename, s );
	if( ch == NULL )
	{
		TRACE_FMGR_FUNC("( ch == NULL )");
		filename[0] = 0;
	}
	else
	{
		TRACE_FMGR_FUNC("( ch != NULL )");
		pfnUnicodeStrcpy( (S8*)filename, (S8*)(ch+1) );
	}
}

/*****************************************************************************
* FUNCTION
*     mmi_audply_check_restorable
* DESCRIPTION
*     check does the audio type support store/restore function (real resume function)
* IMPACT 
*     
* PARAMETERS
*     UI_string_type ext
* RETURNS
*     BOOL
* GLOBALS AFFECTED
*     
*****************************************************************************/
BOOL mmi_autotest_check_restorable( UI_string_type ext )
{
	U8 i, j;
	for( i = 0; autotest_file_restorable_lower_exts[i][0]; i++ )
	{
		BOOL match = TRUE;
		for( j = 0; j < 4; j++ ) // it's is not necessary to check j < 4, but for safety
		{
			UI_character_type file_ext_char = ext[j];
			if( file_ext_char != autotest_file_restorable_lower_exts[i][j] && file_ext_char != autotest_file_restorable_upper_exts[i][j] )
			{
				match = FALSE;
				break;
			}
			else if( file_ext_char == 0 ) // both are end and length less than 4
				return TRUE;
		}
		if( match )
			return TRUE;
	}
	
	return FALSE;
}


void mmi_autotest_split_filename_ext( UI_string_type filename, UI_string_type filename_without_ext, UI_string_type ext )
{
	UI_string_type ptr = filename;
	UI_string_type dot;

	for( dot = NULL; *ptr; ptr++ )
	{
		if( *ptr == (UI_character_type)_T_MMI'.' )
			dot = ptr;
	}
	{
		pfnUnicodeStrncpy( (S8*)filename_without_ext, (S8*)filename, FMGR_MAX_FILE_LEN-1);
		ext[0] = 0;
	}
}

BOOL mmi_autotest_apply_picked_file()
{
	U8 result;
	BOOL isOK;
	UI_character_type filename[FMGR_MAX_PATH_LEN+1];
	UI_character_type wchar_filename[FMGR_MAX_PATH_LEN+1];
	UI_character_type ext[FMGR_MAX_EXT_LEN+1];
	
	if( (result = mmi_autotest_playlist_get_fullfilename( g_autotest.filefullname, g_autotest.pick_index )) == 0 )
	{
		isOK = FALSE;
		g_autotest.restorable = FALSE;
	}
	else
	{
		
		if( result == 2 ) // if it is a short filename
		{
			mmi_autotest_extract_filename( wchar_filename, g_autotest.filefullname );
			mmi_chset_mixed_text_to_ucs2_str( (U8*)filename, (FMGR_MAX_FILE_LEN+1) * ENCODING_LENGTH, (U8*)wchar_filename, g_text_encoding );
		}
		else
			mmi_autotest_extract_filename( filename, g_autotest.filefullname);
		mmi_autotest_split_filename_ext( filename, g_autotest.title, ext );
		g_autotest.restorable = mmi_autotest_check_restorable( ext );
		isOK = TRUE;
	}
	
	//return isOK;
	return TRUE;
}
void mmi_autotest_init_playlist()
{

	
	
	INT32 i = 0,size = 0,filesize = 0;
	FS_HANDLE fd;
	S8 autotestlist[20] = "D:/AutoTestList.txt";
	S8 uautotestlist[40];
	AnsiiToUnicodeString(uautotestlist,autotestlist);
	
	g_autotest.restorable = FALSE;
	g_autotest.title[0] = 0;
	g_autotest.filefullname[0] = 0;
	g_autotest.buff_head = 0;
	g_autotest.n_buff_items = 0;
	
	fd = MMI_FS_Open( (const UINT8 *)uautotestlist, FS_READ_ONLY );
	if(fd >= 0 && (MMI_FS_GetFileSize(fd,(INT32*)&filesize) == FS_NO_ERROR))
	{
	if(  filesize > 0)
	 {
		for( ; i < MAX_AUTO_TEST_LIST_NUM && MMI_FS_Read( fd, AT_fullfilename_temp, AUTO_TEST_LIST_RECORD_SIZE, (INT32*)&size ) == FS_NO_ERROR; i++ )
		{
			  if (pfnUnicodeStrlen((const S8 *)AT_fullfilename_temp) != 0)
			  {	
				
				
				if( size < AUTO_TEST_LIST_RECORD_SIZE )
				{
					TRACE_FMGR_FUNC("Read Failed!!!");
					break;
				}
				if( i < AUTO_TEST_LIST_BUFF_SIZE )
				{
					// load filename to play list buffer
					AT_fullfilename_temp[EOS(size)] = 0; // make it must be in boundary
					mmi_autotest_extract_filename( g_autotest.buff_filename[i], AT_fullfilename_temp );
					g_autotest.n_buff_items++;
				}
				else
				{
					MMI_FS_GetFileSize(fd, (INT32 *)&size);
					i = size/AUTO_TEST_LIST_RECORD_SIZE-1;
					break;
				}
			}
			else
			{
				break;
			}
		}
		MMI_FS_Close( fd );
	}
	else
		{
		MMI_FS_Close( fd );
		}
		}
	else if (fd >= 0)
		{
		MMI_FS_Close( fd );
	}
	
	g_autotest.n_total_items = i;
	mmi_autotest_start_playlist_at( -1 );

}


S32 mmi_autotest_lookup_audio_file_format( UI_string_type file_ext )
{

	S8 tempAnsii[20]={0};
	UINT16 i=0;
	UnicodeToAnsii(tempAnsii, (S8*)file_ext);
	while(tempAnsii[i] != 0)
	{
		if(tempAnsii[i]>='a' &&tempAnsii[i]<='z' )
		tempAnsii[i]=tempAnsii[i]-('a'-'A');
		i++;
	}
	if(strcmp(tempAnsii,"TXT")==0)
	{
	 return TRUE;
	}
	else
	{
		return FALSE;
	}
}


void mmi_autotest_extract_ext( UI_string_type filename, UI_string_type ext )
{
	S16 i, length;
	S8 *path=NULL;

   	length = (S16)pfnUnicodeStrlen((const S8 *)filename);
	path = (S8 *)filename;
   	i = length - 1;

   	while (i>=0)
   	{
   		
      	if (path[i<<1] == '.' && path[(i<<1)+1] == 0 &&(i != length - 1))
         		break;
     	 	i--;
   	}

	if (i<0)
	{
		//mmi_trace(1,"the dot == null");
		ext[0] = 0;
		return ;
	}
	else
	{
	pfnUnicodeStrncpy((S8*)ext,&path[(i+1)<<1],pfnUnicodeStrlen((const S8 *)&path[(i+1)<<1]));
	}	
}


U8 mmi_autotest_gen_playlist()
{
	INT32 size, i;
	FS_HANDLE fd, fd_cursor;
	FS_DOSDirEntry file_info;
	UI_character_type filename[FMGR_MAX_PATH_LEN+1] = {0};
	UI_character_type filexpr_temp[FMGR_MAX_PATH_LEN+1] = {0};
	UI_character_type ext[FMGR_MAX_EXT_LEN+10];
	
	BOOL is_shortname;
	U8 result = 0;
	int write_result;

	S8 filefliter[8] = "*";
	S8 filefliterbuf[16] ;
	S8 autotestfolder[20] = "D:/Auto Test/";
	S8 uautotestfolder[40];
	S8 autotestlist[20] = "D:/AutoTestList.txt";
	S8 uautotestlist[40];
	
	UI_character_type terminal[FMGR_MAX_PATH_LEN+1] = {0};
	
	AnsiiToUnicodeString(uautotestfolder, autotestfolder);
	AnsiiToUnicodeString(filefliterbuf, filefliter);
	AnsiiToUnicodeString(uautotestlist,autotestlist);

	if( (fd = MMI_FS_Open( (const UINT8 *)uautotestlist, FS_CREATE_ALWAYS | FS_ATTR_HIDDEN )) >= 0 ) //chenhe
	{
		FS_HANDLE folder_handle;
		pfnUnicodeStrcpy( (S8*) filexpr_temp, (const S8*) uautotestfolder);

		// check does the folder exist, create it if it's not there
		folder_handle = MMI_FS_Open( (const UINT8 *)filexpr_temp, FS_OPEN_DIR | FS_READ_ONLY );

		if( folder_handle < 0 )
			result = MMI_FS_CreateDir( (const UINT8 *)filexpr_temp );
		else
			MMI_FS_Close( folder_handle );
		
		pfnUnicodeStrcat( (S8*) filexpr_temp, (const S8*) filefliterbuf );
	
		if( (fd_cursor = MMI_FS_FindFirst( (const UINT8 *)filexpr_temp, 0, 0, &file_info, (UINT8*)filename, (FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH )) >= 0 )
		{	
		
			i = 0;
			do
			{
			  if(!(file_info.Attributes == 16))		
			   {
					size = 0;
					
					is_shortname = ((file_info.NTReserved & FS_SFN_MATCH) != 0);
					is_shortname  = 0;
					
					mmi_autotest_extract_ext( filename, ext );
					if( mmi_autotest_lookup_audio_file_format( ext ) > 0 )
					{
					
						memset(AT_fullfilename_temp,0x0,sizeof(AT_fullfilename_temp));
						pfnUnicodeStrcpy( (S8*) AT_fullfilename_temp, (S8*) uautotestfolder );
						pfnUnicodeStrcat( (S8*) AT_fullfilename_temp, (S8*) filename );
												
						if( i >= MAX_AUTO_TEST_LIST_NUM  ) // list full
						{
							result = 3;
							break;
						}
						else if( (write_result = MMI_FS_Write( fd, AT_fullfilename_temp, AUTO_TEST_LIST_RECORD_SIZE, (INT32*)&size )) != 0 )
						{
							if( write_result == FS_DISK_FULL )
								result = 1; // disk full
							else
								result = 2; // other file access error
							break;
						}

						i++;
					}
				
				
			   	}
			      
				
			} while( MMI_FS_FindNext( fd_cursor, &file_info, (UINT8*)filename, (FMGR_MAX_FILE_LEN+1)*ENCODING_LENGTH ) == FS_NO_ERROR );
			
			MMI_FS_FindClose( fd_cursor );
		}

		
	if(result == 0)
	  {	
	    if( (write_result = MMI_FS_Write( fd, terminal, AUTO_TEST_LIST_RECORD_SIZE, (INT32*)&size )) != 0 )
		{
			if(write_result == FS_DISK_FULL)
				{
				
				result = 1;
				MMI_FS_Seek(fd, -AUTO_TEST_LIST_RECORD_SIZE-size, FS_FILE_CURRENT);
				MMI_FS_Write( fd, terminal, AUTO_TEST_LIST_RECORD_SIZE, (INT32*)&size );
				}
			else
				{
				
				result = 2;
				MMI_FS_Seek(fd, -AUTO_TEST_LIST_RECORD_SIZE-size, FS_FILE_CURRENT);
				MMI_FS_Write( fd, terminal, AUTO_TEST_LIST_RECORD_SIZE, (INT32*)&size );
				}
		}
		MMI_FS_Close( fd );
	   }	
	 else
	 {
		if(result == 1)
		{
			MMI_FS_Seek(fd, -AUTO_TEST_LIST_RECORD_SIZE-size, FS_FILE_CURRENT);
		       MMI_FS_Write( fd, terminal, AUTO_TEST_LIST_RECORD_SIZE, (INT32*)&size );
		}
		else{
			MMI_FS_Seek(fd, -AUTO_TEST_LIST_RECORD_SIZE-size, FS_FILE_CURRENT);
		       MMI_FS_Write( fd, terminal, AUTO_TEST_LIST_RECORD_SIZE, (INT32*)&size );

		}
		MMI_FS_Close( fd );
	  }
	}
	else
		result = 2;


	return result;

}


U8 mmi_autotest_regen_and_reload_playlist( BOOL need_regen_list, BOOL need_reload_list )
{
	U8 result = 0;

	if( need_regen_list )
	{
		result = mmi_autotest_gen_playlist(); // regen list
	}
	
	if( need_reload_list )
	{
		StopAutoTestCases();


		mmi_autotest_init_playlist(); // reload list
			
		// pick one
		if( mmi_autotest_pick_playlist_next() >= 0 )
			mmi_autotest_apply_picked_file();
	}
	return result;
}

void mmi_autotest_list_highlight_hdlr( S32 index )
{
	g_mmi_autotest_list_highlighted_item = (U16)index;
}

void mmi_autotest_load_playlist( UI_string_type list_file, S32 index )
{
	INT32 i = 0, size, no_items;
	FS_HANDLE fd;
	
	index -= (AUTO_TEST_LIST_BUFF_SIZE/2 - 1);
	if( index < 0 )
		index = 0;
	no_items = MAX_AUTO_TEST_LIST_NUM - index;
	if( no_items > AUTO_TEST_LIST_BUFF_SIZE)
		no_items = AUTO_TEST_LIST_BUFF_SIZE;
	g_autotest.buff_head = index;
	g_autotest.n_buff_items = 0;
	fd = MMI_FS_Open((const UINT8 *) list_file, FS_READ_ONLY );
	if( fd >= 0 )
	{
		if( MMI_FS_Seek( fd, index * AUTO_TEST_LIST_RECORD_SIZE, FS_FILE_BEGIN ) >= 0 )
		{
			for( ; i < no_items && MMI_FS_Read( fd, AT_fullfilename_temp, AUTO_TEST_LIST_RECORD_SIZE, (INT32*)&size ) == FS_NO_ERROR; i++ )
			{
				if(pfnUnicodeStrlen((const S8 *)AT_fullfilename_temp) != 0)
					{
				if( size < AUTO_TEST_LIST_RECORD_SIZE )
					{
					break;
					}
				// load filename to play list buffer
				AT_fullfilename_temp[EOS(size)] = 0; // make it must be in boundary
				mmi_autotest_extract_filename( g_autotest.buff_filename[i], AT_fullfilename_temp );
				g_autotest.n_buff_items++;
					}

			}
		}

		MMI_FS_Close( fd );
	}
}

BOOL mmi_autotest_playlist_get_filename( UI_string_type filename, S32 index )
{
	S32 offset;
	S8 autotestlist[20] = "D:/AutoTestList.txt";
	S8 uautotestlist[40];
	AnsiiToUnicodeString(uautotestlist,autotestlist);
	if( index < 0 || index >= g_autotest.n_total_items )
		return FALSE;
	offset = index - g_autotest.buff_head;
	if( offset >= 0 && offset < g_autotest.n_buff_items )
	{

		pfnUnicodeStrncpy((S8*)filename, (S8*)g_autotest.buff_filename[offset] , MAX_SUBMENU_CHARACTERS-1);
		return TRUE;
	}
	
	// miss: load items to buffer
	mmi_autotest_load_playlist((UI_string_type) uautotestlist, index );
	
	// double check if load successfully
	offset = index - g_autotest.buff_head;
	if( offset >= 0 && offset < g_autotest.n_buff_items )
	{

		pfnUnicodeStrncpy((S8*)filename, (S8*)g_autotest.buff_filename[offset] , MAX_SUBMENU_CHARACTERS-1);
		return TRUE;
	}
	return FALSE;
}


pBOOL mmi_autotest_list_getitem( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{
	S32 file_ext_index;
	UI_character_type filename[FMGR_MAX_PATH_LEN+1];
	UI_character_type ext[FMGR_MAX_EXT_LEN+1];
		
	// get file name without ext
	mmi_autotest_playlist_get_filename( filename, item_index );

	mmi_autotest_split_filename_ext( filename, str_buff, ext );
	file_ext_index = mmi_autotest_lookup_audio_file_format( ext );
	if( file_ext_index != 2 )

	{
		if( ext[0] != 0 ) // if its ext is not blank
		{
			pfnUnicodeStrcat( (S8*) str_buff, (S8*)L"." );
			pfnUnicodeStrcat( (S8*) str_buff, (S8*)ext );
		}
	}


	*img_buff_p = (UI_image_type) GetImage( IMG_ID_AUDPLY_FILE_AUD_ICON );
	return TRUE;
}
void mmi_autotest_entry_list();
void mmi_autotest_exit_list()
{
	history_t currHistory;
	
	g_autotest.in_list_screen = FALSE;
	
	currHistory.scrnID = EM_AUTO_TEST_LIST;
	currHistory.entryFuncPtr = mmi_autotest_entry_list;
	GetCategoryHistory( currHistory.guiBuffer );
	pfnUnicodeStrcpy ( (S8*)currHistory.inputBuffer, (S8*)L"" );
	AddHistory( currHistory );
}


void mmi_autotest_pick_playlist_at( S32 index )
{
	g_autotest.pick_index = index;
	if( !IsBitSetOn( index, g_autotest.bitset ) )
	{
		SetBitSet( index, g_autotest.bitset );
		g_autotest.pick_count++;
	}
}

void mmi_autotest_play()
{
	mmi_autotest_start_playlist_at( g_mmi_autotest_list_highlighted_item );
	mmi_autotest_pick_playlist_at( g_mmi_autotest_list_highlighted_item );
	if( mmi_autotest_apply_picked_file() )
	{
		StopAutoTestCases(); 
		g_autotest_list_play_fg = 1;
		StartRunAutoTestCasesWaitEventsWithTimerOut(); 
	}
} 

void mmi_autotest_play_all_start()
{
	mmi_autotest_regen_and_reload_playlist(TRUE,TRUE);
	mmi_autotest_start_playlist_at(0);
	mmi_autotest_pick_playlist_at(0);
	if( mmi_autotest_apply_picked_file() )
	{
		StopAutoTestCases(); 
		g_autotest_list_play_fg = 1;
		g_autotest_list_playall_fg = 1;
		StartRunAutoTestCasesWaitEventsWithTimerOut(); 
	}
} 
void mmi_autotest_play_all_next()
{
	if( mmi_autotest_pick_playlist_next() >= 0 )
	{
		mmi_autotest_apply_picked_file();
	}
}
void mmi_autotest_entry_list()
{
	U8* guiBuffer;
	U16 title_id;
	S32 highlighted_item = 0;
	
	EntryNewScreen( EM_AUTO_TEST_LIST, mmi_autotest_exit_list, NULL, NULL );
	mmi_autotest_regen_and_reload_playlist(TRUE,TRUE);
	g_autotest.in_list_screen = TRUE;
	
	title_id = STR_ID_AUDPLY_PLAYLIST;
	g_autotest.present_list = 1;
	
	// register highligh handler
	RegisterHighlightHandler( mmi_autotest_list_highlight_hdlr );
	
	// show category screen
	if( g_autotest.n_total_items == 0 )
	{
		ShowCategory52Screen( title_id, MAIN_MENU_TITLE_MULTIMEDIA_ICON,
			STR_GLOBAL_SELECT, IMG_GLOBAL_YES,
			STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
			0, NULL, NULL, NULL, 1, 0, NULL );
	}
	else
	{
		// if we delete some items and the previous highlighted item is out of boundary, set it to the last one
		if( (guiBuffer = GetCurrGuiBuffer( EM_AUTO_TEST_LIST )) != NULL )
		{
			list_menu_category_history *hist = (list_menu_category_history*)guiBuffer;
			if( hist->highlighted_item >= g_autotest.n_total_items )
				hist->highlighted_item = g_autotest.n_total_items - 1;
		}
		else if( g_autotest.pick_index >= 0 ) // highlighted item
			highlighted_item = g_autotest.pick_index;
		else
			highlighted_item = 0; // this should never happen

		ShowCategory184Screen( title_id, MAIN_MENU_TITLE_MULTIMEDIA_ICON,	// title text and icon
			STR_GLOBAL_SELECT, IMG_GLOBAL_YES,				// LSK text and icon
			STR_GLOBAL_BACK, IMG_GLOBAL_BACK, 						// RSK text and icon
			g_autotest.n_total_items, mmi_autotest_list_getitem, NULL,
			highlighted_item, guiBuffer );
	}
	
	// register LSK, RSK hanlders
	SetLeftSoftkeyFunction( mmi_autotest_play, KEY_EVENT_UP );
	SetRightSoftkeyFunction( GoBackHistory, KEY_EVENT_UP );
	
}

#endif /*#if defined(AUTO_TEST_KEYPRESS_RECORD_SUPPORT)*/

#ifdef __MMI_EM_AUDIO_SPEECH_ENHANCEMENT__
#include "calib_m.h"
#ifndef MMI_ON_WIN32
#include "memd_m.h"
#include "memdp.h"
#endif
#include "adp_fs.h"
typedef enum
{
    EM_AUD_SPH_ENH_NORMAL_MODE,
    EM_AUD_SPH_ENH_HEADSET_MODE,
    EM_AUD_SPH_ENH_LOUDSPK_MODE,
    EM_AUD_SPH_ENH_BT_EARPHONE_MODE,
    EM_AUD_SPH_ENH_FM_MODE,
    EM_AUD_SPH_ENH_TV_MODE,
} mmi_em_para_enum;

typedef enum
{
	EM_AUD_SPH_ENH_TYPE_VOICE,
	EM_AUD_SPH_ENH_TYPE_MUSICE,
	EM_AUD_SPH_ENH_TYPE_SIDE_TONE,
	EM_AUD_SPH_ENH_TYPE_MICROPHONE,   
} mmi_em_para_type_enun;

typedef enum
{
	EM_AUD_SPH_ENH_TYPE_DIGIT_GAINS,
	EM_AUD_SPH_ENH_TYPE_ANALOG_GAINS,
	EM_AUD_SPH_ENH_TYPE_MICROPHONE_GAINS,
	EM_AUD_SPH_ENH_TYPE_VOC_GAINS,
}mmi_em_para_type_type_enum;

mmi_em_para_enum mmi_em_para_type;
mmi_em_para_type_enun mmi_em_para_second_type;
mmi_em_para_type_type_enum mmi_em_para_third_type;
U8 mmi_em_sph_enh_para_level_index;

/*****************************************************************************
 * FUNCTION
 *  mmi_em_sph_enh_entry_main
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_em_sph_enh_entry_main(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 item_texts[32];
    U16 item_icons[32];
    U16 item_num;
    U8 *gui_buffer;
    U8 *item_hints[32];
    S32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(EM_AUD_SPH_ENH_MAIN_SCR, NULL, mmi_em_sph_enh_entry_main, NULL);
    gui_buffer = GetCurrGuiBuffer(EM_AUD_SPH_ENH_MAIN_SCR);

    item_num = GetNumOfChild(EM_AUD_SPEECH_ENHANCEMENT_MENUID);
    GetSequenceStringIds(EM_AUD_SPEECH_ENHANCEMENT_MENUID, item_texts);
    SetParentHandler(EM_AUD_SPEECH_ENHANCEMENT_MENUID);
    RegisterHighlightHandler(ExecuteCurrHiliteHandler);

    for (i = 0; i < item_num; i++)
    {
        item_hints[i] = NULL;
        item_icons[i] = (U16) (IMG_GLOBAL_L1 + i);
    }

    ShowCategory52Screen(
        EM_AUD_SPEECH_ENHANCEMENT_TEXT,
        0,
        STR_GLOBAL_OK,
        0,
        STR_GLOBAL_BACK,
        0,
        item_num,
        item_texts,
        item_icons,
        (U8 **) item_hints,
        0,
        0,
        gui_buffer);

    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_em_sph_enh_entry_para_type_list
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_em_sph_enh_entry_para_type_list(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U16 item_texts[32];
	U16 item_icons[32];
	U16 item_num;
	U16 title_id = 0, menu_id = 0;
	U8 *gui_buffer;
	U8 *item_hints[32];
	S32 i;
	U8 warning_string[100];

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	if(EM_AUD_SPH_ENH_BT_EARPHONE_MODE == mmi_em_para_type)
	{
		memset(warning_string, 0, sizeof(warning_string));
		AnsiiToUnicodeString((S8*)warning_string, "Not Support!");
		DisplayPopup((U8 *)warning_string, IMG_GLOBAL_WARNING, NULL, 2000, WARNING_TONE);
		return;
	}
	
	EntryNewScreen(EM_AUD_SPH_ENH_PARA_LIST_SCR, NULL, mmi_em_sph_enh_entry_para_type_list, NULL);

	gui_buffer = GetCurrGuiBuffer(EM_AUD_SPH_ENH_PARA_LIST_SCR);

	switch (mmi_em_para_type)
	{
		case EM_AUD_SPH_ENH_NORMAL_MODE:
			title_id = EM_AUD_NORMAL_MODE_TEXT;
			menu_id = EM_AUD_SPH_ENH_NORMAL_MODE_MENUID;
			break;
		case EM_AUD_SPH_ENH_HEADSET_MODE:
			title_id = EM_AUD_HEADSET_MODE_TEXT;
			menu_id = EM_AUD_SPH_ENH_HEADSET_MODE_MENUID;
			break;
		case EM_AUD_SPH_ENH_LOUDSPK_MODE:
			title_id = EM_AUD_LOUDSP_MODE_TEXT;
			menu_id = EM_AUD_SPH_ENH_LOUDSPK_MODE_MENUID;
			break;
		case EM_AUD_SPH_ENH_BT_EARPHONE_MODE:
			title_id = EM_AUD_BT_EARPHONE_MODE_TEXT;
			menu_id = EM_AUD_SPH_ENH_BT_EARPHONE_MODE_MENUID;
			break;
		case EM_AUD_SPH_ENH_FM_MODE:
			title_id = EM_AUD_ENH_FM_MODE_STR;
			menu_id = EM_AUD_SPH_ENH_FM_MENUID;
			break;
		case EM_AUD_SPH_ENH_TV_MODE:
			title_id = EM_AUD_ENH_TV_MODE_STR;
			menu_id = EM_AUD_SPH_ENH_TV_MENUID;
			break;
		default:
			ASSERT(0);
	}


	item_num = GetNumOfChild(menu_id);
	GetSequenceStringIds(menu_id, item_texts);
	SetParentHandler(menu_id);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for (i = 0; i < item_num; i++)
	{
		item_hints[i] = NULL;
		item_icons[i] = (U16) (IMG_GLOBAL_L1 + i);
	}

	ShowCategory52Screen(title_id, 0,
						STR_GLOBAL_OK, 0,
						STR_GLOBAL_BACK,0,
						item_num, item_texts, item_icons, (U8 **) item_hints, 0,0, gui_buffer);

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_em_sph_enh_main_highlight_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_em_sph_enh_main_highlight_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetLeftSoftkeyFunction(mmi_em_sph_enh_entry_main, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_em_sph_enh_set_defalut_highlight_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_em_sph_enh_set_defalut_highlight_hdlr(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	SetLeftSoftkeyFunction(mmi_em_sph_enh_set_default, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_em_sph_enh_save_highlight_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_em_sph_enh_save_highlight_hdlr(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	SetLeftSoftkeyFunction(mmi_em_sph_enh_before_save_param_to_flash, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
/*****************************************************************************
 * FUNCTION
 *  mmi_em_sph_enh_normal_mode_highlight_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_em_sph_enh_normal_mode_highlight_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_em_para_type = EM_AUD_SPH_ENH_NORMAL_MODE;
    SetLeftSoftkeyFunction(mmi_em_sph_enh_entry_para_type_list, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_em_sph_enh_headset_mode_highlight_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_em_sph_enh_headset_mode_highlight_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_em_para_type = EM_AUD_SPH_ENH_HEADSET_MODE;
    SetLeftSoftkeyFunction(mmi_em_sph_enh_entry_para_type_list, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_em_sph_enh_loudspk_mode_highlight_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_em_sph_enh_loudspk_mode_highlight_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_em_para_type = EM_AUD_SPH_ENH_LOUDSPK_MODE;
    SetLeftSoftkeyFunction(mmi_em_sph_enh_entry_para_type_list, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_em_sph_enh_bt_earphone_mode_highlight_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_em_sph_enh_bt_earphone_mode_highlight_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_em_para_type = EM_AUD_SPH_ENH_BT_EARPHONE_MODE;
    SetLeftSoftkeyFunction(mmi_em_sph_enh_entry_para_type_list, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_em_sph_enh_FM_mode_highlight_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_em_sph_enh_FM_mode_highlight_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_em_para_type = EM_AUD_SPH_ENH_FM_MODE;
    SetLeftSoftkeyFunction(mmi_em_sph_enh_entry_para_type_list, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_em_sph_enh_TV_mode_highlight_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_em_sph_enh_TV_mode_highlight_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_em_para_type = EM_AUD_SPH_ENH_TV_MODE;
    SetLeftSoftkeyFunction(mmi_em_sph_enh_entry_para_type_list, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


typedef union
{
    /// The value read from the calibration structure.
    UINT32  calibValue;
        
    /// Calibrated gains.
    struct 
    {
        /// Calibrated gains for a voice flow.
        /// Right gain on the stereo macro for the voice flow. (Only for LS or HS).
        UINT32  voiceRight:5;
        /// Left gain on the stereo macro for the voice flow (Only for LS or HS)..
        UINT32  voiceLeft:5;
        /// Preamplifier of the voice flow before the stereo macro (Only for LS or HS)..
        UINT32  voicePreamp:3;
        /// Analog gain of the voice flow.
        UINT32  voiceAnalog:3;
        /// Digital gain of the voice flow.
        UINT32  voiceDigital:2;

        /// Calibrated gains for a music flow.
        /// Right gain on the stereo macro for the music flow
        UINT32  musicRight:5;
        /// Left gain on the stereo macro for the music flow
        UINT32  musicLeft:5;
        /// Digital gain on the stereo macro for the music flow
        UINT32  musicDigital:3;
        /// Stuff bit.
        UINT32  :1;
    } gains;
} AUD_ANALOG_CALIB_OUT_GAINS_T;
extern void calib_DaemonUsrDataProcess(void);
void mmi_em_sph_enh_save_param(void)
{
#ifndef MMI_ON_WIN32
	S16 Arg=0;
	U8  U8StrBuf[8];
	U8 warning_string[100];
	CALIB_AUDIO_ITF_T *pAudioPara = NULL;
	CALIB_CALIBRATION_T *pCalib = NULL;
	AUD_ANALOG_CALIB_OUT_GAINS_T calibOutGains;
	UINT32 len, addrFsh;

	MEMD_ERR_T flashError;

	//ExecuteCurrExitHandler();
	UnicodeToAnsii( (S8*)U8StrBuf , (S8*)InputBuf);
	Arg = (S16) atoi( (S8*)U8StrBuf );

	if(Arg > 15)
	{
		memset(warning_string, 0, sizeof(warning_string));
		AnsiiToUnicodeString((S8*)warning_string, "Number should less than 15!");
		DisplayPopup((U8 *)warning_string, IMG_GLOBAL_WARNING, NULL, 2000, WARNING_TONE);
		return;
	}
	
	pCalib = calib_GetPointers();
	
	switch (mmi_em_para_type)
	{
		case EM_AUD_SPH_ENH_NORMAL_MODE:
			pAudioPara = &(pCalib->bb->audio[0]);
			break;
		case EM_AUD_SPH_ENH_HEADSET_MODE:
			pAudioPara = &(pCalib->bb->audio[1]);
			break;
		case EM_AUD_SPH_ENH_LOUDSPK_MODE:
			pAudioPara = &(pCalib->bb->audio[2]);
			break;
		case EM_AUD_SPH_ENH_BT_EARPHONE_MODE:
			pAudioPara = &(pCalib->bb->audio[3]);
			break;
		case EM_AUD_SPH_ENH_FM_MODE:
			pAudioPara = &(pCalib->bb->audio[4]);
			break;
		case EM_AUD_SPH_ENH_TV_MODE:
			pAudioPara = &(pCalib->bb->audio[5]);
			break;
		default:
			ASSERT(0);
	}
	
	calibOutGains.calibValue = pAudioPara->audioGains.outGain[mmi_em_sph_enh_para_level_index + 1];
	switch(mmi_em_para_second_type)
	{
		case EM_AUD_SPH_ENH_TYPE_VOICE:
			switch(mmi_em_para_third_type)
			{
				case EM_AUD_SPH_ENH_TYPE_DIGIT_GAINS:
					calibOutGains.gains.voiceDigital = Arg;
					break;
				case EM_AUD_SPH_ENH_TYPE_ANALOG_GAINS:
					calibOutGains.gains.voiceLeft = Arg;
					break;
				default:
					ASSERT(0);
	
			}
			break;
		case EM_AUD_SPH_ENH_TYPE_MUSICE:
			calibOutGains.gains.musicLeft = Arg;
			break;
		case EM_AUD_SPH_ENH_TYPE_SIDE_TONE:
			pAudioPara->audioGains.sideTone[mmi_em_sph_enh_para_level_index + 1] = Arg;
			break;
		case EM_AUD_SPH_ENH_TYPE_MICROPHONE:
			switch(mmi_em_para_third_type)
			{
				case EM_AUD_SPH_ENH_TYPE_MICROPHONE_GAINS:
					 pAudioPara->audioGains.inGain = (pAudioPara->audioGains.inGain|0x00ff)&(Arg&0xffff);
					break;
				case EM_AUD_SPH_ENH_TYPE_VOC_GAINS:
					pAudioPara->audioParams.MicDigitalGainInVoC = Arg;
					break;
				default:
					ASSERT(0);
			}
			break;
		default:
			ASSERT(0);
	}

	 pAudioPara->audioGains.outGain[mmi_em_sph_enh_para_level_index + 1] = calibOutGains.calibValue;
#if 0
	if (CALIB_PARAM_CALIBRATED == pCalib->paramStatus && g_memdFlashLayout.numberOfBank > 1)
	{
		memcpy(pCalib->hstBb, pCalib->bb, sizeof(CALIB_BB_T));
		calib_DaemonUsrDataProcess();
	}


	EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);
	DeleteScreenIfPresent(EM_AUD_SPH_ENH_PARA_EDIT_SCR);
#else
	GoBackHistory();
#endif

#endif	
		
}
void mmi_em_sph_enh_save_param_to_flash(void)
{
#ifndef MMI_ON_WIN32
	CALIB_CALIBRATION_T *pCalib = NULL;
	
	pCalib = calib_GetPointers();
	memcpy(pCalib->hstBb, pCalib->bb, sizeof(CALIB_BB_T));
	calib_DaemonUsrDataProcess();
#endif
}
extern BOOL gSystemResetFlag;
void mmi_em_sph_enh_restart_device(void)
{
	#ifndef MMI_ON_WIN32
	INT32 handle = -1,reason = TRUE;
	UINT8 tmpFilename[256];
	AnsiiToUnicodeString(tmpFilename,PHONE_RESTORE_FLAG_PATH);
	if(!isInCall())
	{
		handle =  FS_Open(tmpFilename,FS_O_CREAT|FS_O_RDWR,0);
		if(handle>=0)
		{
			FS_Write(handle,(UINT8*)&reason,sizeof(reason));
			
		}
		FS_Close(handle);
		gSystemResetFlag = MMI_TRUE;
		ShutdownSystemOperation();
	}
#endif
}
void mmi_em_close_stack_rsp_hdr(void *msg_ptr)
{
	U8 i;
	mmi_nw_cfun_state_rsp_struct *p = (mmi_nw_cfun_state_rsp_struct *)msg_ptr;
	
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		ClearProtocolEventHandlerExt(PRT_NW_CFUN_STATE_RSP,i);
	}

	if (p->result == KAL_TRUE)
	{
		mmi_em_sph_enh_save_param_to_flash();
		mmi_em_sph_enh_restart_device();
	}
}
void mmi_em_sph_enh_close_stack_req(void)
{
	U8 i;
	mmi_nw_cfun_state_req_struct *p = NULL;
	ilm_struct ilm = {0};

	p = (mmi_nw_cfun_state_req_struct *)OslMalloc(sizeof(mmi_nw_cfun_state_req_struct));
	MMI_ASSERT(p != NULL);
	memset(p, 0, sizeof(mmi_nw_cfun_state_req_struct));
	
	p->cfun_state = 0;

	memset(&ilm, 0, sizeof(ilm_struct));

	ilm.dest_mod_id = MOD_L4C;
	ilm.src_mod_id = MOD_MMI;
	ilm.sap_id = CSM_L4C_SAP;
	ilm.local_para_ptr = (local_para_struct *)p;
	ilm.msg_id = PRT_NW_CFUN_TEMP_CLOSE_STACK_REQ;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetProtocolEventHandlerExt(mmi_em_close_stack_rsp_hdr, PRT_NW_CFUN_TEMP_CLOSE_STACK_RSP,i);
	}

	OslMsgSendExtQueue(&ilm);
}
void mmi_em_sph_enh_entry_save_progress_scr(void)
{
	EntryNewScreen(EM_AUD_SPH_ENH_SAVE_PROGRESS_SCR, NULL, mmi_em_sph_enh_entry_save_progress_scr, NULL);
	SetKeyHandler(NULL, KEY_END, KEY_EVENT_DOWN);
	SetKeyHandler(NULL, KEY_END, KEY_EVENT_UP);
	ShowCategory66Screen(STR_GLOBAL_SAVING,0,
						  0, 0, 0, 0,
						 (U8 *) GetString(STR_GLOBAL_SAVING), IMG_GLOBAL_PROGRESS, NULL);
}
void mmi_em_sph_enh_entry_save(void)
{
	mmi_em_sph_enh_entry_save_progress_scr();
	mmi_em_sph_enh_close_stack_req();
}
void mmi_em_sph_enh_before_save_param_to_flash(void)
{
#ifndef MMI_ON_WIN32
	U8 warning_string[100];
	CALIB_CALIBRATION_T *pCalib = NULL;
	pCalib = calib_GetPointers();
	if(CALIB_PARAM_CALIBRATED != pCalib->paramStatus)
	{
		memset(warning_string, 0, sizeof(warning_string));
		AnsiiToUnicodeString((S8*)warning_string, "This device need calibrate first");
		DisplayPopup((U8 *)warning_string, IMG_GLOBAL_WARNING, NULL, 2000, WARNING_TONE);
		return;
	}
	memset(warning_string, 0, sizeof(warning_string));
	AnsiiToUnicodeString((S8*)warning_string, "save and restart?");
	DisplayConfirm(STR_GLOBAL_OK,0, STR_GLOBAL_BACK, 0, (UI_string_type)warning_string, IMG_GLOBAL_WARNING, WARNING_TONE);
	SetLeftSoftkeyFunction(mmi_em_sph_enh_entry_save,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
#endif
}
extern CALIB_BB_T g_calibBbDefault;
void mmi_em_sph_enh_set_default(void)
{
	#ifndef MMI_ON_WIN32
	CALIB_CALIBRATION_T *pCalib = NULL;
	pCalib = calib_GetPointers();

	if (CALIB_PARAM_CALIBRATED == pCalib->paramStatus && g_memdFlashLayout.numberOfBank > 1)
	{
		memcpy(pCalib->hstBb, &g_calibBbDefault, sizeof(CALIB_BB_T));
		calib_DaemonUsrDataProcess();
	}

	EngineerModeSuccessNotice(EM_AUD_SET_SUCCESS_TEXT);
#endif
	
}
void mmi_em_sph_enh_param_edit(void)
{
		#ifndef MMI_ON_WIN32
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U8 *history_buffer;
	U16 text_id = 0;
	S32 level = 0;
	S8 ScrBuf[16];
	S8 number_len = 2;
	
	CALIB_AUDIO_ITF_T *pAudioPara = NULL;
	CALIB_CALIBRATION_T *pCalib = NULL;
	AUD_ANALOG_CALIB_OUT_GAINS_T calibOutGains;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	EntryNewScreen(EM_AUD_SPH_ENH_PARA_EDIT_SCR, NULL, mmi_em_sph_enh_param_edit, NULL);
	history_buffer = GetCurrGuiBuffer(EM_AUD_SPH_ENH_PARA_EDIT_SCR);

	memset(InputBuf, 0, sizeof(InputBuf));
	text_id = (U16) (EM_AUD_PARAMETER_LEVEL1_TEXT + mmi_em_sph_enh_para_level_index);

	pCalib = calib_GetPointers();
	
	switch (mmi_em_para_type)
	{
		case EM_AUD_SPH_ENH_NORMAL_MODE:
			pAudioPara = &(pCalib->bb->audio[0]);
			break;
		case EM_AUD_SPH_ENH_HEADSET_MODE:
			pAudioPara = &(pCalib->bb->audio[1]);
			break;
		case EM_AUD_SPH_ENH_LOUDSPK_MODE:
			pAudioPara = &(pCalib->bb->audio[2]);
			break;
		case EM_AUD_SPH_ENH_BT_EARPHONE_MODE:
			pAudioPara = &(pCalib->bb->audio[3]);
			break;
		case EM_AUD_SPH_ENH_FM_MODE:
			pAudioPara = &(pCalib->bb->audio[4]);
			break;
		case EM_AUD_SPH_ENH_TV_MODE:
			pAudioPara = &(pCalib->bb->audio[5]);
			break;
		default:
			ASSERT(0);
	}
	calibOutGains.calibValue = pAudioPara->audioGains.outGain[mmi_em_sph_enh_para_level_index + 1];
	switch(mmi_em_para_second_type)
	{
		case EM_AUD_SPH_ENH_TYPE_VOICE:
			switch(mmi_em_para_third_type)
			{
				case EM_AUD_SPH_ENH_TYPE_DIGIT_GAINS:
					level = calibOutGains.gains.voiceDigital;
					break;
				case EM_AUD_SPH_ENH_TYPE_ANALOG_GAINS:
					level = calibOutGains.gains.voiceLeft;
					break;
				default:
					ASSERT(0);
	
			}
			break;
		case EM_AUD_SPH_ENH_TYPE_MUSICE:
			level = calibOutGains.gains.musicLeft;
			break;
		case EM_AUD_SPH_ENH_TYPE_SIDE_TONE:
			level = pAudioPara->audioGains.sideTone[mmi_em_sph_enh_para_level_index + 1];
			break;
		case EM_AUD_SPH_ENH_TYPE_MICROPHONE:
			switch(mmi_em_para_third_type)
			{
				case EM_AUD_SPH_ENH_TYPE_MICROPHONE_GAINS:
					 level = (pAudioPara->audioGains.inGain) & 0x00ff;
					break;
				case EM_AUD_SPH_ENH_TYPE_VOC_GAINS:
					level = pAudioPara->audioParams.MicDigitalGainInVoC;
					number_len = 3;
					break;
				default:
					ASSERT(0);
			}
			break;
		default:
			ASSERT(0);
	}
	sprintf(ScrBuf, "%d", level);
	AnsiiToUnicodeString((PS8) InputBuf, ScrBuf);
	ShowCategory5Screen(text_id,
						(U16) NULL,
						STR_GLOBAL_OK,
						(U16) NULL,
						STR_GLOBAL_BACK,
						(U16) NULL,
						INPUT_TYPE_NUMERIC,
						(U8*) InputBuf,
						number_len + 1,
						history_buffer);

	SetLeftSoftkeyFunction(mmi_em_sph_enh_save_param, KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_END, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
#endif
}
void mmi_em_sph_enh_entry_para_level_list(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U16 item_texts[32];
	U16 item_icons[32];
	U16 item_num;
	U16 title_id = 0;
	U8 *gui_buffer;
	U8 *item_hints[32];
	S32 i;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	EntryNewScreen(EM_AUD_SPH_ENH_PARA_LEVEL_SCR, NULL, mmi_em_sph_enh_entry_para_level_list, NULL);
	gui_buffer = GetCurrGuiBuffer(EM_AUD_SPH_ENH_PARA_LEVEL_SCR);

	item_num = 7;
	switch (mmi_em_para_second_type)
	{
		case EM_AUD_SPH_ENH_TYPE_VOICE:
			switch(mmi_em_para_third_type)
			{
				case EM_AUD_SPH_ENH_TYPE_DIGIT_GAINS:
					title_id = EM_AUD_ENH_DIGIT_GAINS_STR;
					break;
				case EM_AUD_SPH_ENH_TYPE_ANALOG_GAINS:
					title_id = EM_AUD_ENH_ANALOG_GAINS_STR;
					break;
				default:
					ASSERT(0);
				
			}
			break;
		case EM_AUD_SPH_ENH_TYPE_MUSICE:
			title_id = EM_AUD_ENH_MUSIC_FLOW;
			break;
		case EM_AUD_SPH_ENH_TYPE_SIDE_TONE:
			title_id = EM_AUD_ENH_SIDE_TONE;
			break;
		case EM_AUD_SPH_ENH_TYPE_MICROPHONE:
			switch(mmi_em_para_third_type)
			{
				case EM_AUD_SPH_ENH_TYPE_MICROPHONE_GAINS:
					title_id = EM_AUD_ENH_MICROPHONE_GAIN_STR;
					break;
				case EM_AUD_SPH_ENH_TYPE_VOC_GAINS:
					title_id = EM_AUD_ENH_GAIN_IN_VOC_STR;
					break;
				default:
					ASSERT(0);
				
			}
			item_num = 1;
			break;
		default:
			ASSERT(0);
			break;
	}

	RegisterHighlightHandler(mmi_em_sph_enh_para_level_list_highlight_hdlr);

	for (i = 0; i < item_num; i++)
	{
		item_hints[i] = NULL;
		item_icons[i] = (U16) (IMG_GLOBAL_L1 + i);
		item_texts[i] = (U16) (EM_AUD_PARAMETER_LEVEL1_TEXT + i);
	}

	ShowCategory52Screen(
		title_id,
		0,
		STR_GLOBAL_OK,
		0,
		STR_GLOBAL_BACK,
		0,
		item_num,
		item_texts,
		item_icons,
		(U8 **) item_hints,
		0,
		0,
		gui_buffer);

	SetLeftSoftkeyFunction(mmi_em_sph_enh_param_edit, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

void mmi_em_sph_enh_entry_voice_flow_list(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U16 item_texts[32];
	U16 item_icons[32];
	U16 item_num;
	U8 *gui_buffer;
	U8 *item_hints[32];
	S32 i;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	EntryNewScreen(EM_AUD_SPH_ENH_VOICE_FLOW_SCR, NULL, mmi_em_sph_enh_entry_voice_flow_list, NULL);
	gui_buffer = GetCurrGuiBuffer(EM_AUD_SPH_ENH_VOICE_FLOW_SCR);

	item_num = GetNumOfChild(EM_AUD_SPH_ENH_VOICE_FLOW);
	GetSequenceStringIds(EM_AUD_SPH_ENH_VOICE_FLOW, item_texts);
	SetParentHandler(EM_AUD_SPH_ENH_VOICE_FLOW);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for (i = 0; i < item_num; i++)
	{
		item_hints[i] = NULL;
		item_icons[i] = (U16) (IMG_GLOBAL_L1 + i);
	}

	ShowCategory52Screen(EM_AUD_ENH_VOICE_FLOW,
						0,
						STR_GLOBAL_OK,
						0,
						STR_GLOBAL_BACK,
						0,
						item_num,
						item_texts,
						item_icons,
						(U8 **) item_hints,
						0,
						0,
						gui_buffer);

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

void mmi_em_sph_enh_entry_microphone_flow_list(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U16 item_texts[32];
	U16 item_icons[32];
	U16 item_num;
	U8 *gui_buffer;
	U8 *item_hints[32];
	S32 i;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	EntryNewScreen(EM_AUD_SPH_ENH_MICROPHONE_FLOW_SCR, NULL, mmi_em_sph_enh_entry_microphone_flow_list, NULL);
	gui_buffer = GetCurrGuiBuffer(EM_AUD_SPH_ENH_MICROPHONE_FLOW_SCR);

	if(EM_AUD_SPH_ENH_FM_MODE == mmi_em_para_type)
	{
		mmi_frm_hide_menu_item(EM_AUD_SPH_ENH_MICROPHONE_GAINS);
	}
	else
	{
		mmi_frm_unhide_menu_item(EM_AUD_SPH_ENH_MICROPHONE_GAINS);
	}
	item_num = GetNumOfChild_Ext(EM_AUD_SPH_ENH_MICROPHONE);
	GetSequenceStringIds_Ext(EM_AUD_SPH_ENH_MICROPHONE, item_texts);
	SetParentHandler(EM_AUD_SPH_ENH_MICROPHONE);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	for (i = 0; i < item_num; i++)
	{
		item_hints[i] = NULL;
		item_icons[i] = (U16) (IMG_GLOBAL_L1 + i);
	}

	ShowCategory52Screen(EM_AUD_ENH_VOICE_FLOW,
						0,
						STR_GLOBAL_OK,
						0,
						STR_GLOBAL_BACK,
						0,
						item_num,
						item_texts,
						item_icons,
						(U8 **) item_hints,
						0,
						0,
						gui_buffer);

	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}
/*****************************************************************************
 * FUNCTION
 *  mmi_em_sph_enh_voice_flow_highlight_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_em_sph_enh_voice_flow_highlight_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_em_para_second_type = EM_AUD_SPH_ENH_TYPE_VOICE;
    SetLeftSoftkeyFunction(mmi_em_sph_enh_entry_voice_flow_list, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_em_sph_enh_mucis_flow_highlight_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_em_sph_enh_mucis_flow_highlight_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_em_para_second_type = EM_AUD_SPH_ENH_TYPE_MUSICE;
    SetLeftSoftkeyFunction(mmi_em_sph_enh_entry_para_level_list, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_em_sph_enh_side_tone_highlight_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_em_sph_enh_side_tone_highlight_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_em_para_second_type = EM_AUD_SPH_ENH_TYPE_SIDE_TONE;
    SetLeftSoftkeyFunction(mmi_em_sph_enh_entry_para_level_list, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_em_sph_enh_microphone_highlight_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_em_sph_enh_microphone_highlight_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_em_para_second_type = EM_AUD_SPH_ENH_TYPE_MICROPHONE;
    SetLeftSoftkeyFunction(mmi_em_sph_enh_entry_microphone_flow_list, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_em_sph_enh_digit_gains_highlight_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_em_sph_enh_digit_gains_highlight_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_em_para_third_type = EM_AUD_SPH_ENH_TYPE_DIGIT_GAINS;
    SetLeftSoftkeyFunction(mmi_em_sph_enh_entry_para_level_list, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_em_sph_enh_analog_gains_highlight_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_em_sph_enh_analog_gains_highlight_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_em_para_third_type = EM_AUD_SPH_ENH_TYPE_ANALOG_GAINS;
    SetLeftSoftkeyFunction(mmi_em_sph_enh_entry_para_level_list, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_em_sph_enh_microphone_gains_highlight_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_em_sph_enh_microphone_gains_highlight_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_em_para_third_type = EM_AUD_SPH_ENH_TYPE_MICROPHONE_GAINS;
    SetLeftSoftkeyFunction(mmi_em_sph_enh_param_edit, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_em_sph_enh_voc_gains_highlight_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_em_sph_enh_voc_gains_highlight_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_em_para_third_type = EM_AUD_SPH_ENH_TYPE_VOC_GAINS;
    SetLeftSoftkeyFunction(mmi_em_sph_enh_param_edit, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_em_sph_enh_para_list_highlight_hdlr
 * DESCRIPTION
 *  
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_em_sph_enh_para_level_list_highlight_hdlr(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_em_sph_enh_para_level_index = index;
}
#endif/*__MMI_EM_AUDIO_SPEECH_ENHANCEMENT__*/
#endif/* __MMI_ENGINEER_MODE__ */


