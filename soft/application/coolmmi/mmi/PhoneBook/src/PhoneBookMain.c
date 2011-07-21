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
 * PhoneBookMain.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for phonebook main function.
 *   Including initialize procedure, main menu, common utilities, etc.
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
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/************************************************************************
  Change Histroy
  Version      Date             Author    Description
  02.01.001  2007-06-21, Li Tao, handle the extended fields, they will not be displayed to end user
 ************************************************************************/
 
#ifndef _PHONEBOOKMAIN_C
#define _PHONEBOOKMAIN_C

#define __NEWSIMULATOR
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "globalconstants.h"
#include "debuginitdef.h"	/*debug info*/
#include "protocolevents.h"
#include "historygprot.h"
#include "wgui_categories.h"
#include "wgui_categories_cm.h"
#include "wgui_categories_inputs.h"
#include "wgui_status_icons.h"
#include "allappgprot.h"
#include "phonebookdef.h"
#include "phonebookprot.h"
#include "phonebookgprot.h"
#include "phonebooktypes.h"
#include "queuegprot.h"
#include "mainmenudef.h"
#include "unicodexdcl.h"
#include "globaldefs.h"
#include "timerevents.h"
#include "idleappdef.h"
#include "speeddial.h"
#include "nvramprot.h"
#include "nvramtype.h"
#include "nvramenum.h"
#include "nvram_data_items.h"
#include "idleappdef.h"
#include "idleappprot.h"
#include "commonscreens.h"
#include "settinggprots.h"
#include "gpioinc.h"
#include "commonscreens.h"
#include "settingdefs.h" /*for fdn on message*/
#include "settingsgdcl.h"
#include "mmi_trace.h"
#include "servicedefs.h"

#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "dual_sim_call.h"
#include "mtpnp_ad_resdef.h"
#endif

#ifdef MMI_ON_HARDWARE_P
#include "nvram_data_items.h"
#endif
#include "gsm7bitdefaultalphabet.h"
#include "statusiconres.h"

#if defined(__MMI_PHB_UPDATE_CALL_LOG_AFTER_SAVE__)  /*Update Call Log after save number*/
#include "callhistorygprots.h"
#endif

#if defined(__MMI_FILE_MANAGER__) && defined(MMI_ON_HARDWARE_P)
#include "wgui_categories_fmgr.h" /*for image list*/
#endif
#if defined(__J2ME__)
#include "phonebookjavahandler.h"
#endif

#if defined( __MMI_INTELLIGENT_CALL_ALERT__ )
#include "profilegprots.h"
#endif
#include "callmanagementgprot.h"

#if defined(__MMI_VRSI__)
#include "mdi_datatype.h"
#include "vrsienum.h"
#include "vrsitype.h"
#include "vrsiprot.h"
#endif

#if defined(__MMI_VCARD__)
#include "vcard.h"
#endif

#if !defined(__MMI_KEYPADLOCK_WITH_KEYTONE__)
#include "keybrd.h"
#endif

#include "pictureeditordefs.h"

#ifdef __FLIGHT_MODE_SUPPORT__
#include "simdetectiongexdcl.h"
#endif
#include "simdetectiongprot.h"
#undef __NEWSIMULATOR
//#ifdef CSD_TARGET	/* support IRAM optimization */
//#pragma arm section rwdata = "INTERNRW" , rodata = "INTERNCONST" , zidata = "INTERNZI"
//#endif

/*
** Local Variable
*/
mmi_phb_context_struct g_phb_cntx;

U16 PhoneBookEntryCount;

#if (MAX_PB_ENTRIES)
U16 g_phb_name_index[MAX_PB_ENTRIES];	/*Sorting Order by Name Field.*/
MMI_PHB_ENTRY_BCD_STRUCT		PhoneBook[MAX_PB_ENTRIES];
#if	!defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
U8 *gp_phb_option_flag = NULL;	/*option Field.*/
#endif
#if (MAX_PB_ENTRIES >= 1000)
U16 g_phb_list_filter[MAX_PB_ENTRIES];
#endif /* MAX_PB_ENTRIES >= 1000 */
#else
U16 g_phb_name_index[1];	/*Sorting Order by Name Field.*/
MMI_PHB_ENTRY_BCD_STRUCT		PhoneBook[1];
#endif /* MAX_PB_ENTRIES */

#ifdef __MMI_MULTI_SIM__
#if defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
MMI_PHB_LOOKUP_NODE_STRUCT LookUpTable[MAX_PB_SIM_ENTRIES*MMI_SIM_NUMBER+MAX_PB_PHONE_ENTRIES+1];
#else
MMI_PHB_LOOKUP_NODE_STRUCT LookUpTable[MAX_PB_SIM_ENTRIES*MMI_SIM_NUMBER+4*MAX_PB_PHONE_ENTRIES+4];
PHB_OPTIONAL_FIELDS_STRUCT	PhoneBookOptionalFields;// = {0};
#endif
#else
#if defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
MMI_PHB_LOOKUP_NODE_STRUCT LookUpTable[MAX_PB_SIM_ENTRIES+MAX_PB_PHONE_ENTRIES+1];
#else
MMI_PHB_LOOKUP_NODE_STRUCT LookUpTable[MAX_PB_SIM_ENTRIES+4*MAX_PB_PHONE_ENTRIES+4];
PHB_OPTIONAL_FIELDS_STRUCT	PhoneBookOptionalFields;// = {0};
#endif
#endif
static PHB_OPTIONAL_IDS_STRUCT	g_phb_optional_ids;
static PHB_CALLER_GROUP_STRUCT	g_phb_caller_group[MAX_PB_CALLER_GROUPS];

S8 pbName[(MAX_PB_NAME_LENGTH+1)*ENCODING_LENGTH];
S8 pbNumber[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];
S8 pbHomeNumber[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];
S8 pbCompanyName[(MAX_PB_COMPANY_LENGTH+1)*ENCODING_LENGTH];
S8 pbEmailAddress[(MAX_PB_EMAIL_LENGTH+1)*ENCODING_LENGTH];
S8 pbOfficeNumber[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];
S8 pbFaxNumber[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];
#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
S8	pbBday[12*ENCODING_LENGTH];	/*Use for display total sstring, Format: xxxx/xx/xx*/
S8	pbDay[3*ENCODING_LENGTH];
S8	pbMon[3*ENCODING_LENGTH];
S8	pbYear[6*ENCODING_LENGTH];
#endif

U8 gPictureItemIndex = 10;
U16 TitleStrings[MAX_PB_FIELDS];
#ifdef __MMI_MULTI_SIM__
BOOL gFDNAskState[MMI_SIM_NUMBER] = {FALSE,}; //if FDN state check is on going or pending
#endif

static U16 g_phb_message;
static U16 g_phb_message_icon;
static U16 g_phb_alpha_index_list[ALPHA_LIST_LENGTH];
static U16 g_phb_highlight_alpha;
extern BOOL gSimReady[];//man for sim init

#ifdef __MMI_INTELLIGENT_CALL_ALERT__
U8 g_phb_is_speaking_name = 0;
void mmi_phb_speak_name_handler( mdi_result result )
{
	g_phb_is_speaking_name = 0;
}
#endif

/*
** Global Variable
*/
extern MMI_PHB_LIST_VIEW phbListView;
extern wgui_inline_item wgui_inline_items[];
extern U8 gKeyPadLockFlag;
extern BOOL r2lMMIFlag;	/*For right to left language*/
extern BOOL g_FixedInit[];//Vivian add for FDL
/*
** Global Function
*/

#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
extern VOID VDS_CacheFlushAlloc(VOID);
#endif

extern void MakeCall(PS8);
extern void set_wgui_inline_list_menu_changed(void);
#if defined(__IP_NUMBER__)
extern void SetTempUseIPNumber(pBOOL flag);
#endif
#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__)
static U8 g_phb_sim_check;
extern kal_bool is_sim_replaced(void);
#endif

#if defined(__MMI_FILE_MANAGER__) && defined(MMI_ON_HARDWARE_P)
extern void Cat212ShortCutHdlr(S32 item_index); /*For Image List Fast Jump*/
#endif


extern void mmi_phb_init_callergroup_name(void);

#define MMI_PHB_INIT
/*****************************************************************************
* FUNCTION
*  mmi_phb_init_protocol()
* DESCRIPTION
 
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   initialize g_phb_cntx
*****************************************************************************/
void mmi_phb_init_protocol(void)
{
	U8 i;
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_init_protocol.>\n",__FILE__,__LINE__));
	#ifdef __MMI_MULTI_SIM__
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetProtocolEventHandlerExt(SECSETGetFdlBdlModeInd, PRT_FDN_GET_STATE_IND,i);
	}
	#endif

	SetProtocolEventHandler(mmi_phb_ind_startup_finish	,PRT_PHB_STARTUP_FINISH_IND);
#if defined(MMI_ON_HARDWARE_P)
	SetProtocolEventHandler(mmi_phb_ind_startup_begin	,PRT_PHB_STARTUP_BEGIN_IND);
	SetProtocolEventHandler(mmi_phb_ind_startup_read	,PRT_PHB_STARTUP_READ_IND);
#endif

#if defined(__J2ME__)
	mmi_phb_java_handler_init();
#endif
/*wangrui Add for fixbug 10195 start on 2008.10.28*/
	g_phb_cntx.phb_ReadyFinishi = FALSE;
/*wangrui Add for fixbug 10195 end*/
	/*Initial Global Context.*/
	g_phb_cntx.phb_ready = FALSE;
	g_phb_cntx.processing = FALSE;
	g_phb_cntx.nvram_data_init = FALSE;
	g_phb_cntx.optional_ids = &g_phb_optional_ids;
	g_phb_cntx.caller_group = &g_phb_caller_group[0];
	g_phb_cntx.list_filter = NULL;
	g_phb_cntx.start_scr_id = 0;
	g_phb_cntx.end_scr_id = 0;
	g_phb_cntx.refresh_list = MMI_PHB_ENTRY_REFRESH;
	g_phb_cntx.highlight_entry = 0;
	g_phb_cntx.new_highlight_entry = 0xffff;
	g_phb_cntx.populate_count = 0;
	g_phb_cntx.lookup_table_count = 0;

	g_phb_cntx.selected_pic_index = 0;
	g_phb_cntx.selected_ring_index = 0;
	g_phb_cntx.selected_grp_index = 0;
	g_phb_cntx.selected_pattern_index = 0;
	g_phb_cntx.selected_alert_index = 0;
	g_phb_cntx.selected_pic_in_view = 0;
	g_phb_cntx.image_location = MMI_PHB_IMAGE_NO_SELECT;
	g_phb_cntx.set_done_flag = 0;
	g_phb_cntx.dial_from_list = MMI_PHB_NONE;
#if defined(__MMI_PHB_PINYIN_SORT__)
	g_phb_cntx.sort_type = MMI_PHB_SORT_PINYIN;
#else
	g_phb_cntx.sort_type = MMI_PHB_SORT_ENCODING;
#endif
#if defined(__MMI_INCOMING_CALL_VIDEO__)
	g_phb_cntx.init_video = 0;
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_init()
* DESCRIPTION
*   Initialize phonebook applications.
*   (This is used to initialize hilite/hint handlers and PS reponse callback functions)
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_init(void)
{
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_init.>\n",__FILE__,__LINE__));

	mmi_phb_search_init();  
	mmi_phb_operate_single_init();  // zrx del 20060620		//add by liuxn 060704
	mmi_phb_operate_mass_init();  // zrx del 20060620            //add by liuxn 060704
	mmi_phb_caller_group_init();
	mmi_phb_extra_number_init();
	mmi_phb_setting_init();
	PhbInitSpeedDial();

	#ifdef __MMI_MULTI_SIM__
	MTPNP_PFAL_Slave_PHB_Init();
	#endif
}


/*****************************************************************************
* FUNCTION
*  mmi_phb_set_main_menu_highlight_handler
* DESCRIPTION
*   Set phonebook main menu highlight handler
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
static void mmi_phb_set_main_menu_highlight_handler(void)
{
	U8 i;
	mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_set_main_menu_highlight_handler.>\n",__FILE__,__LINE__));

	SetHiliteHandler(MITEM101_PBOOK_SEARCH_ENTRY	,mmi_phb_highlight_search_name);
#if defined(__MMI_PHB_QUICK_SEARCH__)
	SetHiliteHandler(MENU_ID_PHB_QUICK_SEARCH		,mmi_phb_highlight_quick_search_list);
#endif

	/*Add Entry*/
	SetHiliteHandler(MITEM102_PBOOK_NEW_ENTRY		,mmi_phb_highlight_op_add_entry);

    SetHiliteHandler(MENU_ID_PHB_IMPORT_AND_EXPORT, mmi_phb_highlight_import_and_export);
	/*Copy All*/
	SetHiliteHandler(MITEM104_PBOOK_COPY_ALL			,mmi_phb_highlight_copy_all);

	/*Delete All*/
	SetHiliteHandler(MITEM103_PBOOK_DELETE_ALL		,mmi_phb_highlight_delete_all);

	SetHiliteHandler(MITEM109_PBOOK_CALLER_GROUPS	,mmi_phb_highlight_callergroup);

	mmi_phb_extra_number_init();

	/*Settings*/
	SetHiliteHandler(MITEM110_PBOOK_SETTINGS				,mmi_phb_highlight_setting);
	SetHiliteHandler(MITEM1101_PBOOK_MEMORY_STATUS		,mmi_phb_highlight_memory_status);
	SetHiliteHandler(MITEM1102_PBOOK_STORAGE_LOCATION	,mmi_phb_highlight_storage_location);
	SetHiliteHandler(MITEM1103_PBOOK_VIEW_FIELDS			,mmi_phb_highlight_view_field);

	SetHintHandler(MITEM1102_PBOOK_STORAGE_LOCATION	,mmi_phb_hint_storage_location);
	SetHintHandler(MITEM_STORAGE_LOCATION_SIM			,mmi_phb_hint_storage_location_in_sim);
	SetHintHandler(MITEM_STORAGE_LOCATION_PHONE		,mmi_phb_hint_storage_location_in_phone);
#if defined(__MMI_VCARD__)
	/*My vCard*/
	SetHiliteHandler(MENU_ID_PHB_VCARD_MYCARD,			mmi_phb_highlight_vcard_mycard);
	SetHiliteHandler(MENU_ID_PHB_VCARD_EDIT_MYCARD,	mmi_phb_highlight_vcard_edit_mycard);
	SetHiliteHandler(MENU_ID_PHB_VCARD_SEND_MYCARD,	mmi_phb_highlight_vcard_send_mycard);	
#endif
/* vCard version */
#if defined(__MMI_VCARD__)  && defined(__MMI_VCARD_V30__)
        SetHiliteHandler(MENU_ID_PHB_VCARD_VERSION, mmi_phb_highlight_vcard_version);
        SetHintHandler(MENU_ID_PHB_VCARD_VERSION, mmi_phb_hint_vcard_version);
        SetHiliteHandler(MENU_ID_PHB_VCARD_VERSION_21, mmi_phb_highlight_vcard_version_21);
        SetHiliteHandler(MENU_ID_PHB_VCARD_VERSION_30, mmi_phb_highlight_vcard_version_30);
#endif /* defined(__MMI_VCARD__)  && defined(__MMI_VCARD_V30__) */

#if defined( __MMI_INTELLIGENT_CALL_ALERT__ )
#if defined(__PROJECT_GALLITE_C01__)
#else
	SetHiliteHandler(MENU_ID_PHB_SPEAK_NAME				,mmi_phb_highlight_speak_name);
	SetHintHandler(MENU_ID_PHB_SPEAK_NAME,mmi_phb_hint_speak_name);	
#endif				
#endif

	/*Forward resource to phonebook entry*/
#if defined(__MMI_PHB_CALLER_RES_SETTING__)
	/*For submenu in phonebook*/
	SetHiliteHandler(MENU_ID_PHB_CALLER_PIC	,mmi_phb_highlight_caller_pic);
	SetHiliteHandler(MENU_ID_PHB_CALLER_RING	,mmi_phb_highlight_caller_ring);
#if defined(__MMI_INCOMING_CALL_VIDEO__)
	SetHiliteHandler(MENU_ID_PHB_CALLER_VIDEO	,mmi_phb_highlight_caller_video);
#endif
#endif	
	
#if defined(__MMI_FILE_MANAGER__) && !defined(__MMI_FILE_MANAGER_HIDE_MENU__)
	/*Save Image From File Manager*/
	SetHiliteHandler(MENU_ID_PHB_FMGR_SAVE_NEW		,mmi_phb_highlight_fmgr_add_entry);
	SetHiliteHandler(MENU_ID_PHB_FMGR_SAVE_LIST		,mmi_phb_highlight_fmgr_edit_entry);
	SetHiliteHandler(MENU_ID_PHB_FMGR_SAVE_CALLGRP	,mmi_phb_highlight_edit_callergroup);
#endif
#if defined(__MMI_VCARD__)
	SetHiliteHandler(MENU_ID_PHB_VCARD_EXPORT		,mmi_phb_highlight_export);
	SetHiliteHandler(MENU_ID_PHB_EXPORT_FROM_PHONE		,mmi_phb_highlight_export_from_phone);
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		SetHiliteHandler(MENU_ID_PHB_EXPORT_FROM_SIM1 + i, mmi_phb_highlight_export_from_sim);
	}
	
	SetHiliteHandler(MENU_ID_PHB_VCARD_IMPORT		,mmi_phb_highlight_import);
	SetHiliteHandler(MENU_ID_PHB_IMPORT_FROM_VCF        ,mmi_phb_highlight_import_from_vcf);
    	SetHiliteHandler(MENU_ID_PHB_IMPORT_FROM_OUTLOOK    ,mmi_phb_highlight_import_from_outlook);
#endif	
	/* Done and Input Method */
	mmi_frm_set_highlight_handler();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_ind_startup_begin()
* DESCRIPTION
 
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_ind_startup_begin(void* info)
{
	U8 i;
	g_phb_cntx.phb_ReadyFinishi = FALSE;
	g_phb_cntx.phb_ready = FALSE;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		g_phb_cntx.sim_used[i] = 0;
	}
	g_phb_cntx.phone_used = 0;
	PhoneBookEntryCount = 0;
#if	!defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
	if(gp_phb_option_flag == NULL)
	{
		gp_phb_option_flag = (U8 *)OslMalloc(sizeof(U8) * MAX_PB_PHONE_ENTRIES);
              if(gp_phb_option_flag != NULL)
              {
			memset(gp_phb_option_flag, 0, MAX_PB_PHONE_ENTRIES);
              }
	}
#endif    
	/*Phonebook reload so need reset VR. After sorting done, add phonebook entry again.*/
#if defined(__MMI_VRSI__)
	mmi_vrsi_init_central_reset();
#endif	
}

#if defined(MMI_ON_HARDWARE_P)
/*****************************************************************************
* FUNCTION
*  mmi_phb_ind_startup_read
* DESCRIPTION
 
*	Stores each indication to the global phonebook list.
* PARAMETERS
 
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_ind_startup_read(void* info)
{
	MSG_PHB_STARTUP_READ_IND_STRUCT * LocalPtr;
	U16 store_index;
	U8 num_ascii[MAX_PB_NUMBER_LENGTH+1+1];
	U8 bcd_length = 0;
	U8 i;

	LocalPtr = (MSG_PHB_STARTUP_READ_IND_STRUCT*) info;


	mmi_trace(g_sw_PHB, "PHB Init mmi_phb_ind_startup_read record_index= %d,  storage= %d \n",LocalPtr->phb_entry.record_index,LocalPtr->phb_entry.storage);


	 
	mmi_phb_ind_startup_read_next_req(LocalPtr->access_id, LocalPtr->phb_entry.storage);

	/*Parse and save data from indication*/
	if(LocalPtr->phb_entry.storage - MMI_SIM < MMI_SIM_NUMBER)
	{
		for(i = 0; i < MMI_SIM_NUMBER; i++)
		{
			if(g_SecuritySetupContext.FdlStatus[LocalPtr->phb_entry.storage - MMI_SIM] == TRUE)
			{
				return;
			}
		}
	}

	if(LocalPtr->phb_entry.storage >= MMI_SIM && LocalPtr->phb_entry.storage <= MMI_SIM4)
	{
		U8 nSimIndex = LocalPtr->phb_entry.storage - MMI_SIM;
		g_phb_cntx.sim_used[nSimIndex]++;
		store_index = LocalPtr->phb_entry.record_index + MAX_PB_PHONE_ENTRIES + MAX_PB_SIM_ENTRIES*nSimIndex - 1; /*Storage Location in array.*/
		MMI_ASSERT((g_phb_cntx.sim_used[nSimIndex] <= MAX_PB_SIM_ENTRIES) && (store_index < MAX_PB_PHONE_ENTRIES + MAX_PB_SIM_ENTRIES*(nSimIndex+1)));
	}
	else
	{
		g_phb_cntx.phone_used++;
		store_index = LocalPtr->phb_entry.record_index - 1; /*Storage Location in array.*/
#if	!defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
		gp_phb_option_flag[store_index] = 1;
#endif
		MMI_ASSERT((g_phb_cntx.phone_used <= MAX_PB_PHONE_ENTRIES) && (store_index < MAX_PB_PHONE_ENTRIES));
	}

	/*Name Field*/
	PhoneBook[store_index].alpha_id.name_length	= LocalPtr->phb_entry.alpha_id.name_length;
	PhoneBook[store_index].alpha_id.name_dcs	= LocalPtr->phb_entry.alpha_id.name_dcs;

	if(PhoneBook[store_index].alpha_id.name_length == 0)
		memset(PhoneBook[store_index].alpha_id.name, 0, 2);
	else
		InputboxConvertGSMToDeviceEncoding(LocalPtr->phb_entry.alpha_id.name,
													  PhoneBook[store_index].alpha_id.name,
													  LocalPtr->phb_entry.alpha_id.name_length,
													  (MAX_PB_NAME_LENGTH+1)*ENCODING_LENGTH,
													  LocalPtr->phb_entry.alpha_id.name_dcs,
													  0,
													  1);
#if defined(__PHB_0x81_SUPPORT__)	/*Support maximum length of 0x81 UCS2*/
	if(PhoneBook[store_index].alpha_id.name_dcs == MMI_PHB_UCS2_81 ||
	   PhoneBook[store_index].alpha_id.name_dcs == MMI_PHB_UCS2_82)
				PhoneBook[store_index].alpha_id.name_dcs = MMI_PHB_UCS2;
#endif

#if defined(__MMI_PHB_PINYIN_SORT__)
	mmi_phb_util_make_pinyin_cache(store_index);
#endif

	/*Number Field
 
	* And the length is the total byte of TON + BCD numbers.*/
	PhoneBook[store_index].tel.type	= LocalPtr->phb_entry.tel.addr_bcd[0];
	if(LocalPtr->phb_entry.tel.addr_length > 0)
		bcd_length = ((LocalPtr->phb_entry.tel.addr_length -1) < ((MAX_PB_NUMBER_LENGTH + 1) / 2)) ? (LocalPtr->phb_entry.tel.addr_length -1) : ((MAX_PB_NUMBER_LENGTH + 1) / 2);

	if(bcd_length > 0)
	{
		//strncpy((S8*)PhoneBook[store_index].tel.number, (S8*)(LocalPtr->phb_entry.tel.addr_bcd+1), (MAX_PB_NUMBER_LENGTH + 1) / 2);
		memset(PhoneBook[store_index].tel.number,0xff,((MAX_PB_NUMBER_LENGTH + 1) / 2));
		memcpy(PhoneBook[store_index].tel.number, (LocalPtr->phb_entry.tel.addr_bcd+1), bcd_length);
	}
	else
	{
		PhoneBook[store_index].tel.number[0] = 0xff;
	}
	if (PhoneBook[store_index].tel.type == MMI_CSMCC_INTERNATIONAL_ADDR)
	{
		num_ascii[0] = '+';
		mmi_phb_convert_to_digit(&num_ascii[1], PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH + 1);
	}
	else
		mmi_phb_convert_to_digit(num_ascii, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH+1);
		
	PhoneBook[store_index].tel.length = strlen((S8*)num_ascii);

	/*Update flag for number exists entry, Clear it first.*/
	PhoneBook[store_index].field = 0;
	if(PhoneBook[store_index].tel.number[0] != 0xff)
		PhoneBook[store_index].field |= MMI_PHB_ENTRY_FIELD_NUMBER;

	g_phb_name_index[PhoneBookEntryCount] = store_index;
	PhoneBookEntryCount++;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_ind_startup_read_next_req
* DESCRIPTION
 
* 
* PARAMETERS
 
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_ind_startup_read_next_req(U16 access_id,  U8 storage)
{
	mmi_trace(g_sw_PHB, TSTXT("PHB Init mmi_phb_ind_startup_read_next_req "));	
	MSG_PHB_STARTUP_READ_NEXT_REQ_STRUCT *myMsgPtr;
	MYQUEUE Message;
	module_type	dest_mod_id = MOD_L4C;

	myMsgPtr = (MSG_PHB_STARTUP_READ_NEXT_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_STARTUP_READ_NEXT_REQ_STRUCT));
	myMsgPtr->access_id = access_id;
	
#ifdef __MMI_MULTI_SIM__
	 if(storage >= MMI_SIM && storage <= MMI_SIM4)
 	{
 		dest_mod_id = MOD_L4C + storage - MMI_SIM;
 	}
#endif

	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=dest_mod_id;
	Message.oslMsgId = PRT_PHB_STARTUP_READ_NEXT_REQ;
	Message.oslDataPtr = (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr= NULL;

	OslMsgSendExtQueue(&Message);
}
#endif

extern MMI_BOOL PhnsetGetShowOwnerNumStatus(void);
void mmi_phb_get_disp_owner_num()
{
	U8 i;
	mmi_trace(g_sw_PHB, "PHB func:%s \n", __FUNCTION__);
	if (HasValidSIM())
	{
		for(i = 0; i < MMI_SIM_NUMBER; i++)
		{
			#ifdef __MMI_MULTI_SIM__
			if(!MTPNP_PFAL_Is_CardAbsent(i))
			#endif
			{
				mmi_phb_idle_disp_owner_number_get_entry_req(i);
			}
		}
	}

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_ind_startup_finish()
* DESCRIPTION
*   Reads the Phonebook ready indication from PS and prepare to sort.
*
* PARAMETERS
*  info IN	finish indication
* RETURNS
*  void
* GLOBALS AFFECTED
*   g_phb_cntx
*****************************************************************************/
void mmi_phb_ind_startup_finish (void* info)
{
	MSG_PHB_STARTUP_FINISH_IND_STRUCT* localPtr;
	U8 nSimID;
	U8 i;
	BOOL bFdl = MMI_FALSE;

	nSimID = MTPNP_AD_get_protocol_event_src_mode();

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_ind_startup_finish.>\n",__FILE__,__LINE__));
 	mmi_trace(1, TSTXT("mmi_phb_ind_startup_finish "));
	localPtr = (MSG_PHB_STARTUP_FINISH_IND_STRUCT *)info;
	
	g_phb_cntx.fdn_name_len	=	localPtr->fdn_len;
	g_phb_cntx.bdn_name_len	=	localPtr->bdn_len;
	g_phb_cntx.owner_name_len	=	localPtr->owner_len;
	
	g_phb_cntx.phone_total	=	localPtr->phb_max_num;

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		g_phb_cntx.sim_name_len[i] = localPtr->phb_len;
		g_phb_cntx.sim_total[i] = localPtr->sim_max_num[i];
		if(g_phb_cntx.sim_total[i] > MAX_PB_SIM_ENTRIES)
		{
			g_phb_cntx.sim_total[i] = MAX_PB_SIM_ENTRIES;
		}
	}

        if (g_phb_cntx.phone_total > MAX_PB_PHONE_ENTRIES)
        {
            g_phb_cntx.phone_total = MAX_PB_PHONE_ENTRIES;  /* This two values should be the same */
        }

	if(!g_phb_cntx.nvram_data_init)
	{
		mmi_phb_init_get_data_from_nvram();
		g_phb_cntx.nvram_data_init = TRUE;
	}

#if defined(MMI_ON_HARDWARE_P)
	/*Sort name list*/
	mmi_phb_sort_build_name_index();

	/*
	*   Use SIM storage buffer to store FDN list when FDN is enable.
	*   Because when FDN is enable, the ADN entry cannot be retrieved from SIM card.
	*   This list is for incoming call lookup name usage.
	*
	*   Note: Be sure to retrieve speed dial number after retrieve FDN list.
	*/
//cong.li adds for singleSimCardWapMMS on 2009.07.08
	mmi_trace(g_sw_PHB, "PHB Func: %s g_SecuritySetupContext.FdlStatus[nSimID]=%d", __FUNCTION__, nSimID, g_SecuritySetupContext.FdlStatus[nSimID]);
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_SecuritySetupContext.FdlStatus[i]&&(!g_FixedInit[i]))
		{
			g_FixedInit[i] = TRUE;
			#ifdef __MMI_MULTI_SIM__
			SECSETSetSIMIdforPIN(i);
			#endif
			bFdl = MMI_TRUE;
			mmi_phb_fdn_get_list_to_ram();
			//jump out the loop, other cards should be handled in the response
			break;
		}
		/*
		* Get Speed Dial Info. If FDN on, get speed dial info after FDN list retrieved.
		* Because they all use same message.
		*/
	}
	if(!bFdl)
	{
		if (!HasValidSIM()
  #ifdef __FLIGHT_MODE_SUPPORT__
			|| mmi_bootup_get_active_flight_mode() == 1
  #endif
		)
		{
			g_phb_cntx.phb_ready = TRUE;
			mmi_phb_init_build_lookup_table();
			mmi_phb_get_disp_owner_num();
			 mmi_trace(1, TSTXT("	g_phb_cntx.phb_ready = TRUE; "));
		}
		else
		{
			PhbGetSpeedDialInfo();
			mmi_phb_get_disp_owner_num();
		}
	}
#else/*MMI_ON_HARDWARE_P*/
	mmi_phb_init_refresh_phonebook();
#endif


	g_phb_cntx.phb_ReadyFinishi = TRUE;

	for(i = 0; i < MMI_SIM_NUMBER; i++)
		gSimReady[i] = TRUE;
    #ifdef MMI_ON_HARDWARE_P
#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
      VDS_CacheFlushAlloc();
#endif
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_init_get_data_from_nvram()
* DESCRIPTION
*   Retrieves the phonebook data like, Caller Group details, view fields,
*   preferred storage, etc from NVRAM
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_init_get_data_from_nvram(void)
{
	U8 i;
	S16 pError;
	U16 tempStorageLocation;	/*Temp Solution For Read/Write 2 bytes NVRAM_PHB_STORAGE_LOCATION*/
	PHB_CALLER_GROUP_STRUCT* callerGroups;
	U8 viewField[NVRAM_PHB_SETTINGS_SIZE];

	callerGroups = (PHB_CALLER_GROUP_STRUCT*)g_phb_cntx.caller_group;

    // dingjian 20080320 this read nvram is fail
	ReadRecord(NVRAM_EF_PHB_SETTINGS_LID, 1, viewField, NVRAM_PHB_SETTINGS_SIZE, &pError);
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d;pError=%d+++++-----", __FUNCTION__, __FILE__, __LINE__, &pError);
	memcpy(g_phb_cntx.view_field, viewField,MAX_PB_VIEW_FIELDS);

// dingjian 20080320 set PHB LID default
    for (i = 0; i < MAX_PB_VIEW_FIELDS; i++)
    {
#if defined(__PROJECT_GALLITE_C01__)
        if ( (i + MAX_PB_MANDATORY_FIELDS == MMI_PHB_FIELD_NAME) // 0
              || (i + MAX_PB_MANDATORY_FIELDS == MMI_PHB_FIELD_NUMBER)// 1
#else
        if (   (i + MAX_PB_MANDATORY_FIELDS == MMI_PHB_FIELD_NUMBER)// 1
#endif
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
              || (i + MAX_PB_MANDATORY_FIELDS == MMI_PHB_FIELD_HOME)// 2
              || (i + MAX_PB_MANDATORY_FIELDS == MMI_PHB_FIELD_COMPANY_NAME)// 3
              //JIASHUO ADD FOR WAP/MMS,ADD FOR EMAIL FIELD,20090407
			  #if defined(JATAAYU_SUPPORT)
              || (i + MAX_PB_MANDATORY_FIELDS == MMI_PHB_FIELD_EMAIL)// 4
              #endif
              //JIASHUO ADD FOR WAP/MMS,ADD FOR EMAIL FIELD,20090407
              || (i + MAX_PB_MANDATORY_FIELDS == MMI_PHB_FIELD_OFFICE)// 5
#if defined(__MMI_PHB_BIRTHDAY_FIELD__)	
              || (i + MAX_PB_MANDATORY_FIELDS == MMI_PHB_FIELD_BIRTHDAY)// 7
#endif 	
#endif 	
              || (i + MAX_PB_MANDATORY_FIELDS == MMI_PHB_FIELD_PIC)// 8
#if defined(__MMI_INCOMING_CALL_VIDEO__)	
              || (i + MAX_PB_MANDATORY_FIELDS == MMI_PHB_FIELD_VIDEO)// 9
#endif 	
              || (i + MAX_PB_MANDATORY_FIELDS == MMI_PHB_FIELD_RING)// 10
              || (i + MAX_PB_MANDATORY_FIELDS == MMI_PHB_FIELD_GROUP))// 11
        {
            g_phb_cntx.view_field[i] = 1;
        }
        else
        {
            g_phb_cntx.view_field[i] = 0;
        }
    }
    
	ReadValue(NVRAM_PHB_STORAGE_LOCATION, &tempStorageLocation, DS_SHORT, &pError);	/*2 bytes for DS_SHORT*/
	g_phb_cntx.prefer_storage = (U8)tempStorageLocation;
	ReadRecord(NVRAM_EF_PHB_CALLER_GROUPS_LID, 1, callerGroups, CALLER_GROUPS_RECORD_SIZE, &pError);

	if(g_phb_cntx.prefer_storage != MMI_SIM && g_phb_cntx.prefer_storage != MMI_NVRAM)
	{
		g_phb_cntx.prefer_storage = MMI_SIM;
		tempStorageLocation = g_phb_cntx.prefer_storage;
		WriteValue(NVRAM_PHB_STORAGE_LOCATION, &tempStorageLocation, DS_SHORT, &pError); /*2 bytes for DS_SHORT*/
	}

	/*No alert or LED seting in caller group.*/
	for (i = 0; i < MAX_PB_CALLER_GROUPS; ++i)
	{
#if	defined(__MMI_CALLERGROUP_NO_ALERT__)
		callerGroups[i].alertType = MMI_ALERT_NONE;
#endif

#if !defined(__MMI_STATUS_LED__)
		callerGroups[i].LEDPatternId = 0;
#endif
	}
   
	/*Should never go into this section in normal situation*/
#if !defined(MMI_ON_HARDWARE_P)
	if (callerGroups[0].ringToneID== 0xFFFF)
		mmi_phb_callergroup_restore_default();
#else
    mmi_phb_init_callergroup_name();            //add by chenqiang for bug 5923 
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_init_build_lookup_table()
* DESCRIPTION
*   This function builds the lookup table for cross-referencing during MO/MT calls.
*   This table contains the home/mobile/fax/office number(s) of a phonebook entry.
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_init_build_lookup_table(void)
{
	g_phb_cntx.lookup_table_count = 0;
	g_phb_cntx.populate_count = 0;

	/*Start a timer to avoid extensive NVRAM access which may cause MMI hang shortly*/
	//mmi_phb_init_populate_lookup_table();
	StartTimer(PHB_READ_OPT_FILED_TIMER,200,mmi_phb_init_populate_lookup_table);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_init_populate_lookup_table()
* DESCRIPTION
*   This function populates the lookup table with home/fax/mobile/office numbers after
*   converting the last 7(or 9) characters of the phone number into digits.
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#define MAX_PB_OPTION_ENTRIES		MAX_PB_PHONE_ENTRIES /*number of items read at one time*/
#define MAX_PB_OPTION_READ_TIMES    (MAX_PB_OPTION_ENTRIES + MAX_PB_OPTION_ENTRIES - 1) / MAX_PB_OPTION_ENTRIES
extern S32 ReadAllPHBFiled(void *pBuffer, U32 nSeekPos, U32 nReadBufferSize,S16 *pError);
void mmi_phb_init_populate_lookup_table(void)
{
    U16 count, store_index = 0;
    S16 pError;
    U8 num_ascii[MAX_PB_NUMBER_LENGTH+1+1];
    PHB_OPTIONAL_FIELDS_STRUCT	 *pPhoneBookOptionalFields = NULL;    
    U16 i = 0, j = 0;

    count = g_phb_cntx.populate_count;
    MMI_TRACE((MMI_TRACE_G4_PHB,MMI_PHB_FUNC_PopulateLookUpTable_BEGIN, g_phb_cntx.populate_count));


#if	!defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
    pPhoneBookOptionalFields = (PHB_OPTIONAL_FIELDS_STRUCT *)OslMalloc(sizeof(PHB_OPTIONAL_FIELDS_STRUCT) * MAX_PB_OPTION_ENTRIES);

    if(pPhoneBookOptionalFields != NULL)
    {
        for(i = 0; i < MAX_PB_OPTION_READ_TIMES; i++)
        {
            ReadAllPHBFiled((void*)pPhoneBookOptionalFields,  i * MAX_PB_OPTION_ENTRIES * OPTIONAL_FIELDS_RECORD_SIZE, MAX_PB_OPTION_ENTRIES * OPTIONAL_FIELDS_RECORD_SIZE, &pError); 
            if(pError != NVRAM_READ_SUCCESS)
            {
                break;
            }
            
            for(j = 0; j < MAX_PB_OPTION_ENTRIES; j++)
            {
#ifdef MMI_ON_HARDWARE_P
                if(gp_phb_option_flag[store_index] == 1)
#endif
                {
                    mmi_phb_op_increase_lookup_table(store_index,
        											  NULL,
        											  (S8*)pPhoneBookOptionalFields[j].faxNumber,
        											  (S8*)pPhoneBookOptionalFields[j].homeNumber,
        											  (S8*) pPhoneBookOptionalFields[j].officeNumber);
                    mmi_phb_op_update_field_flag(store_index,
                									 NULL,
                									 (S8*)pPhoneBookOptionalFields[j].homeNumber,
                									 (S8*)pPhoneBookOptionalFields[j].officeNumber,
                									 (S8*)pPhoneBookOptionalFields[j].faxNumber,
                									 (S8*)pPhoneBookOptionalFields[j].emailAddress,
                									 FALSE);
                }
                store_index++;
            }
        }
    }
    
    OslMfree(pPhoneBookOptionalFields);
    OslMfree(gp_phb_option_flag);
#endif

    while (count < PhoneBookEntryCount)
    {
        store_index = g_phb_name_index[count];

        /*Populate Mobile Number Field. Either in SIM or in NVRAM*/
        mmi_phb_convert_to_digit(num_ascii, PhoneBook[store_index].tel.number, MAX_PB_NUMBER_LENGTH+1);
        mmi_phb_op_increase_lookup_table(store_index, (S8*)num_ascii,NULL,NULL,NULL);

        count++;
        g_phb_cntx.populate_count++;
    }

    MMI_TRACE((MMI_TRACE_G4_PHB,MMI_PHB_FUNC_PopulateLookUpTable_END, g_phb_cntx.populate_count));

    if(count < PhoneBookEntryCount)	/*More entries to populate*/
    {
    	//StopTimer(PHB_READ_OPT_FILED_TIMER);
    	//StartTimer(PHB_READ_OPT_FILED_TIMER,250,mmi_phb_init_populate_lookup_table);
    }
    else	/*All entries populated, begin to sort it.*/
    {
    	mmi_phb_lookup_table_sort();
    }

}


#if !defined(MMI_ON_HARDWARE_P)	/*For PC Simulater Load Entry*/
/*****************************************************************************
* FUNCTION
*  mmi_phb_init_refresh_phonebook()
* DESCRIPTION
*   Syncs the MMI phonebook with the protocol stack phonebook
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_init_refresh_phonebook(void)
{
	U8 i;
	g_phb_cntx.phone_used = 0;
	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		g_phb_cntx.sim_used[i] = 0;
	}
	PhoneBookEntryCount = 0;
	g_phb_cntx.refresh_list = MMI_PHB_ENTRY_REFRESH;
	mmi_phb_get_entry_by_index_req();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_get_entry_by_index_req()
* DESCRIPTION
*   Requests the stack to return the phonebook entries
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern U16 g_phb_read_index;
void mmi_phb_get_entry_by_index_req(void)
{
	MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT *myMsgPtr;
	MYQUEUE	Message;

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_get_entry_by_index_req.>\n",__FILE__,__LINE__));

	if (!PhoneBookEntryCount)
		g_phb_read_index = 1;
	else
		g_phb_read_index += MAX_ENTRIES_IN_LIST;

	myMsgPtr = (MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT*) OslConstructDataPtr(sizeof(MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT));
	myMsgPtr->index			= g_phb_read_index;
	myMsgPtr->record_index	= 0xFFFF;
	myMsgPtr->storage		= MMI_STORAGE_NONE;
	myMsgPtr->type			= MMI_PHB_PHONEBOOK;
	myMsgPtr->no_data		= MAX_ENTRIES_IN_LIST;

	Message.oslSrcId		= MOD_MMI;
	Message.oslDestId		= MOD_L4C;
	Message.oslMsgId		= PRT_PHB_GET_ENTRY_BY_INDEX_REQ;
	Message.oslDataPtr		= (oslParaType *)myMsgPtr;
	Message.oslPeerBuffPtr	= NULL;

	SetProtocolEventHandler(mmi_phb_get_entry_by_index_rsp,	PRT_PHB_GET_ENTRY_BY_INDEX_RSP);
	OslMsgSendExtQueue(&Message);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_get_entry_by_index_rsp()
* DESCRIPTION
*    Handles response from stack, which returns phonebook entries
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_get_entry_by_index_rsp(void* info)
{
	MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT* localPtr;
	U8 i;
	U16 store_index;
	U16 nSimTotal = 0;

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_get_entry_by_index_rsp.>\n",__FILE__,__LINE__));
//qiff modify for bug:10650,the RSP handler should be set in REQ function	
        SetProtocolEventHandler(NULL, PRT_PHB_GET_ENTRY_BY_INDEX_RSP);

	localPtr = (MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT *)info;
	if (localPtr->result.flag == 0 /*OK*/)
	{
		for (i = 0; i < localPtr->no_list; ++i)
		{
			if(localPtr->list[i].storage >= MMI_SIM && localPtr->list[i].storage <= MMI_SIM4)
			{
				U8 nSimIndex = localPtr->list[i].storage - MMI_SIM;
				if(g_phb_cntx.sim_used[nSimIndex] < g_phb_cntx.sim_total[nSimIndex])
				{
					++g_phb_cntx.sim_used[nSimIndex];
					store_index = localPtr->list[i].record_index + MAX_PB_PHONE_ENTRIES + MAX_PB_SIM_ENTRIES*nSimIndex- 1; /*Storage Location in array.*/
				}	
				else
					continue;
			}
			else
			{
				if(g_phb_cntx.phone_used < g_phb_cntx.phone_total)
				{
					++g_phb_cntx.phone_used;
					store_index = localPtr->list[i].record_index - 1; /*Storage Location in array.*/
				}
				else
					continue;
			}

			PhoneBook[store_index].alpha_id.name_length	= localPtr->list[i].alpha_id.name_length;//*ENCODING_LENGTH;
			PhoneBook[store_index].alpha_id.name_dcs	= localPtr->list[i].alpha_id.name_dcs;
			InputboxConvertGSMToDeviceEncoding(localPtr->list[i].alpha_id.name,
															  PhoneBook[store_index].alpha_id.name,
															  localPtr->list[i].alpha_id.name_length,
															  (MAX_PB_NAME_LENGTH+1)*ENCODING_LENGTH,
															  localPtr->list[i].alpha_id.name_dcs,
															  0,
															  1);
#if defined(__PHB_0x81_SUPPORT__)	/*Support maximum length of 0x81 UCS2*/
			if(PhoneBook[store_index].alpha_id.name_dcs == MMI_PHB_UCS2_81 || PhoneBook[store_index].alpha_id.name_dcs == MMI_PHB_UCS2_82)
				PhoneBook[store_index].alpha_id.name_dcs = MMI_PHB_UCS2;
#endif

			PhoneBook[store_index].tel.type				= localPtr->list[i].tel.type;
			PhoneBook[store_index].tel.length				= localPtr->list[i].tel.length;
			mmi_phb_convert_to_bcd(PhoneBook[store_index].tel.number, localPtr->list[i].tel.number, (MAX_PB_NUMBER_LENGTH+1)/2);

			/*Update flag for number exists entry, Clear it first.*/
			PhoneBook[store_index].field = 0;
			if(PhoneBook[store_index].tel.number[0] != 0xff)
				PhoneBook[store_index].field |= MMI_PHB_ENTRY_FIELD_NUMBER;

			g_phb_name_index[PhoneBookEntryCount] = store_index;
			++PhoneBookEntryCount;
		}
		for(i = 0; i < MMI_SIM_NUMBER; i++)
		{
			nSimTotal += g_phb_cntx.sim_total[i];
		}
		if ((localPtr->no_list == MAX_ENTRIES_IN_LIST) && (PhoneBookEntryCount < (nSimTotal+g_phb_cntx.phone_total)))
		{
			mmi_phb_get_entry_by_index_req();
		}
		else
		{
			mmi_phb_sort_build_name_index();
			PhbGetSpeedDialInfo();
			mmi_phb_get_disp_owner_num();
		}
	}
	else
	{
		mmi_phb_sort_build_name_index();
		PhbGetSpeedDialInfo();
		mmi_phb_get_disp_owner_num();
	}

}
#endif

#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__) /* Associate caller group in SIM card entry*/
/*****************************************************************************
* FUNCTION
*  mmi_phb_init_check_sim_change()
* DESCRIPTION
*   This function checks if sim changes for phonebook.
*
* PARAMETERS
* void
* RETURNS
*  TRUE if updated. FALSE if not.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
BOOL mmi_phb_init_check_sim_change(void)
{
#if defined(MMI_ON_HARDWARE_P) /*Check SIM change only on hardware*/
	//U16 count = 0;
	//U16 i;
	//S16 pError;
	//U8 optid_record, optid_index, current_record = 0;
	//PHB_OPTIONAL_IDS_STRUCT PhbOptIDs[OPTIONAL_IDS_RECORD_TOTAL];

	if(g_phb_sim_check == 0)
	{
		g_phb_sim_check = 1;
    mmi_phb_init_delete_caller_group_in_sim();
		return TRUE;
		 
		// use until now.
		#if 0
		{
			/*Count entry number first*/
			for (i = MAX_PB_PHONE_ENTRIES; i < MAX_PB_ENTRIES; ++i)
			{
				/*Get optional IDs record.*/
				optid_record = (i / OPTIONAL_IDS_RECORD_TOTAL) + 1;
				optid_index = i - (optid_record -1) * OPTIONAL_IDS_RECORD_TOTAL;

				if(optid_record != current_record)
				{
					ReadRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);
					current_record = optid_record;
				}

				if(PhbOptIDs[optid_index].callerGroupID != 0)
					count++;
			}

			/*If id exists, prompt user to delete.*/
			if(count)
			{
				//mmi_phb_init_entry_sim_refresh_confirm();
				mmi_phb_init_delete_caller_group_in_sim();
				return TRUE;
			}
		}
		#endif
	}
#endif
	return FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_init_entry_sim_refresh_confirm()
* DESCRIPTION
*   This function is the confirmation screen for SIM changed.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_init_entry_sim_refresh_confirm(void)
{
	//U8* guiBuffer;

	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO,IMG_GLOBAL_NO,
		get_string(STR_ID_PHB_REFRESH_SIM), IMG_GLOBAL_QUESTION, WARNING_TONE);

	//EntryNewScreen(SCR_ID_PHB_REFRESH_SIM_CONFIRM, NULL, mmi_phb_init_entry_sim_refresh_confirm, NULL);
	//guiBuffer = GetCurrGuiBuffer(SCR_ID_PHB_REFRESH_SIM_CONFIRM);

	//ShowCategory164Screen(STR_GLOBAL_YES, IMG_GLOBAL_YES, STR_GLOBAL_NO,IMG_GLOBAL_NO,
	//	STR_ID_PHB_REFRESH_SIM, IMG_GLOBAL_QUESTION, guiBuffer);
	SetLeftSoftkeyFunction(mmi_phb_init_delete_caller_group_in_sim, KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);

	playRequestedTone(WARNING_TONE); //add by panxu 20061228
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_init_delete_caller_group_in_sim()
* DESCRIPTION
*   This function clears all caller group associate to SIM card.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_phb_init_delete_caller_group_in_sim(void)
{
	U16 i;
	S16 pError;
	U8 optid_record, optid_index, current_record = 0;
	PHB_OPTIONAL_IDS_STRUCT PhbOptIDs[OPTIONAL_IDS_RECORD_TOTAL];

	for (i = MAX_PB_PHONE_ENTRIES; i < MAX_PB_ENTRIES; ++i)
	{
		/*Get optional IDs record.*/
		optid_record = (i / OPTIONAL_IDS_RECORD_TOTAL) + 1;
		optid_index = i - (optid_record -1) * OPTIONAL_IDS_RECORD_TOTAL;

		if(optid_record != current_record)
		{
			if(current_record != 0)	/*Write result back for previous optional ID record.*/
				WriteRecord(NVRAM_EF_PHB_IDS_LID, current_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);

			/*Read out next record.*/
			ReadRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);
			current_record = optid_record;
		}

		memset(&PhbOptIDs[optid_index],0,sizeof(PHB_OPTIONAL_IDS_STRUCT));
	}

	/*Write back for last record.*/
	WriteRecord(NVRAM_EF_PHB_IDS_LID, optid_record, (void*)PhbOptIDs, OPTIONAL_IDS_RECORD_SIZE, &pError);
  #if 0
	DisplayPopup((U8*)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED,  TRUE, UI_POPUP_NOTIFYDURATION_TIME, SUCCESS_TONE);
	DeleteNScrId(SCR_ID_PHB_REFRESH_SIM_CONFIRM);
  #endif
}
#endif

#define MMI_PHB_MAIN_MENU
/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_main_menu()
* DESCRIPTION
*   Shows the Phonebook main menu
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_main_menu(void)
{
	U8* guiBuffer;
	U16 nStrItemList[MAX_SUB_MENUS];
	U16 nImgIltemList[MAX_SUB_MENUS];
	U16 nNumofItem;
	U8 *hintList[MAX_SUB_MENUS]; /*Array for hint*/

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_entry_main_menu.>\n",__FILE__,__LINE__));
	//g_phb_cntx.selected_pic_in_view = 0;

#ifdef __MMI_MAINLCD_220X176__
	if (MTPNP_PFAL_Is_CardValid(0) == 0 && MTPNP_PFAL_Is_CardValid(1) == 0)
	{
		DisplayPopup( (PU8)GetString(SERVICES_NOT_PROVIDED_STRING_ID), IMG_GLOBAL_ERROR, 0, 1000, (U8)ERROR_TONE );
		return;
	};
#endif	

	EntryNewScreen(SCR_PBOOK_MAIN_MENU, mmi_phb_exit_main_menu, mmi_phb_entry_main_menu,NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_PBOOK_MAIN_MENU);		/* Getting the Index from history */
	if (HasValidSIM())
	{
		nNumofItem = GetNumOfChild(MAIN_MENU_PHONEBOOK_MENUID);
		GetSequenceStringIds(MAIN_MENU_PHONEBOOK_MENUID, nStrItemList);
		GetSequenceImageIds(MAIN_MENU_PHONEBOOK_MENUID, nImgIltemList);
		SetParentHandler(MAIN_MENU_PHONEBOOK_MENUID);
		/* Construct List To Show Hint*/
		ConstructHintsList(MAIN_MENU_PHONEBOOK_MENUID, hintList);
	}
	else
	{
		nNumofItem = GetNumOfChild(MAIN_MENU_PHONEBOOK_NO_SIM_MENUID);
		GetSequenceStringIds(MAIN_MENU_PHONEBOOK_NO_SIM_MENUID, nStrItemList);
		GetSequenceImageIds(MAIN_MENU_PHONEBOOK_NO_SIM_MENUID, nImgIltemList);
		SetParentHandler(MAIN_MENU_PHONEBOOK_NO_SIM_MENUID);
		/* Construct List To Show Hint*/
		ConstructHintsList(MAIN_MENU_PHONEBOOK_NO_SIM_MENUID, hintList);
	}

	RegisterHighlightHandler(ExecuteCurrHiliteHandler);

	/* Call option menu may invoke before mmi_phb_init is executed */
	mmi_phb_set_main_menu_highlight_handler();
	
 
#ifdef __MMI_WGUI_MINI_TAB_BAR__
	if (HasValidSIM())
	{
		mmi_wgui_enable_mini_tab_bar(MAIN_MENU_PHONEBOOK_MENUID);
	}
	else
	{
		mmi_wgui_enable_mini_tab_bar(MAIN_MENU_PHONEBOOK_NO_SIM_MENUID);
	}
#endif
 
#ifdef __MMI_NUMBER_MENU__
	/*Screen with hint, if no hint needed, can change to Screen 15*/
	ShowCategory52Screen(  STR_SCR_PBOOK_CAPTION, IMG_SCR_PBOOK_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem,nStrItemList, (PU16)gIndexIconsImageList,
						  hintList,
						  0,0, guiBuffer);
#else
	/*Screen with hint, if no hint needed, can change to Screen 15*/
	ShowCategory52Screen(  STR_SCR_PBOOK_CAPTION, IMG_SCR_PBOOK_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem,nStrItemList,nImgIltemList,
						  hintList,
						  0,0, guiBuffer);
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_main_menu()
* DESCRIPTION
*   Exit function for mmi_phb_entry_main_menu()
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_main_menu(void)
{
	g_phb_cntx.start_scr_id = SCR_PBOOK_MAIN_MENU;
	g_phb_cntx.end_scr_id = SCR_PBOOK_MAIN_MENU;

 
#ifdef __MMI_WGUI_MINI_TAB_BAR__
	mmi_wgui_disable_mini_tab_bar();
#endif
 
}

/*****************************************************************************
* FUNCTION
*  mi_phb_entry_not_ready()
* DESCRIPTION
*   Displays Phobebook not ready screen
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_not_ready(void)
{
	U16 stringid = STR_PROCESSING_PHONEBOOK;
	
#if defined(__PROJECT_GALLITE_C01__)
	if(!g_phb_cntx.phb_ready)
	{
		stringid = STR_INIT_PHONEBOOK;
	}
#endif
    
	EntryNewScreen(SCR_PHB_DUMMY, mmi_phb_exit_not_ready, NULL,NULL);
	StartTimer (PHB_NOTIFYDURATION_TIMER, PHB_NOTIFY_TIMEOUT, mmi_phb_wait_and_go_back);
	ShowCategory66Screen(STR_SCR_PBOOK_CAPTION,IMG_SCR_PBOOK_CAPTION, 0, 0, STR_GLOBAL_BACK, IMG_GLOBAL_BACK, (PU8)GetString(stringid), IMG_PROCESSING_PHONEBOOK, NULL);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_not_ready()
* DESCRIPTION
*   Exit function for mmi_phb_entry_not_ready().
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_not_ready(void)
{
	StopTimer (PHB_NOTIFYDURATION_TIMER);
	g_idle_context.ToNameScrFromIdleApp=0;
	g_idle_context.RskPressedFromIdleApp=0;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_wait_and_go_back()
* DESCRIPTION
*   Goes back to history after timer expires
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_wait_and_go_back(void)
{
	StopTimer (PHB_NOTIFYDURATION_TIMER);
	GoBackHistory();
	g_idle_context.ToNameScrFromIdleApp=0;
	g_idle_context.RskPressedFromIdleApp=0;
}

/*****************************************************************************
* FUNCTION
*   mmi_phb_show_in_progress()
* DESCRIPTION
*   To display work-in-progress status
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_show_in_progress(U16 msg, U16 msgIcon)
{
	g_phb_message = msg;
	g_phb_message_icon = msgIcon;
	mmi_phb_entry_in_progress();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_in_progress()
* DESCRIPTION
*   Draws the in-progress screen
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_in_progress(void)
{
	//EntryNewScreen(SCR_IN_PROGRESS_PHB, mmi_phb_exit_in_progress, NULL,NULL);
	EntryNewScreen(SCR_IN_PROGRESS_PHB, NULL, NULL,NULL);

 
#ifdef __MMI_UI_STYLE_4__
//passing 0xffff for Title string and Title icon for the disbling the status bar and title display.
	ShowCategory8Screen(0xffff, 0xffff, 0, 0, 0, 0, g_phb_message, g_phb_message_icon, NULL);
#else
	ShowCategory8Screen(STR_SCR_PBOOK_CAPTION,IMG_SCR_PBOOK_CAPTION, 0, 0, 0, 0, g_phb_message, g_phb_message_icon, NULL);
#endif
 

	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
}

/**************************************************************

	FUNCTION NAME		: isInPhbOperateMass

  	PURPOSE				: Function to make check if phb is in operating mass

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: if the phb is in operating mass

	RETURNS				: pBOOL

 
**************************************************************/
pBOOL is_phb_in_progress(void)
{
    return g_phb_cntx.processing;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_in_progress()
* DESCRIPTION
*   Exit function to mmi_phb_entry_in_progress()
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
/* No need to add processing to history, the copy all abort is very quick after revise.*/
/*
void mmi_phb_exit_in_progress (void)
{
	history   h;
	S16		nHistory	=	0;

	if(g_phb_cntx.processing)
	{
		g_phb_cntx.end_scr_id = SCR_IN_PROGRESS_PHB;

		h.scrnID = SCR_IN_PROGRESS_PHB;
		h.entryFuncPtr = mmi_phb_entry_in_progress;
		pfnUnicodeStrcpy((PS8)h.inputBuffer, (PS8)&nHistory);
		GetCategoryHistory(h.guiBuffer);
		AddHistory(h);
	}
}
*/

/*****************************************************************************
* FUNCTION
*   mmi_phb_show_mass_processing()
* DESCRIPTION
*   To display work-in-progress status for mass operation. (Copy All, Delete All)
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_show_mass_processing(U16 msg, U16 msgIcon)
{
	g_phb_message = msg;
	g_phb_message_icon = msgIcon;
	mmi_phb_entry_mass_processing();
}

/*****************************************************************************
* FUNCTION
*   mmi_phb_mass_processing_scr_del_callback()
* DESCRIPTION
*   Call back function when the screen is deleted
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_phb_mass_processing_scr_del_callback(void *p)
{
	if(g_phb_cntx.processing) /* if phonebook still copying or deleting, do not deleted this screen */
		return MMI_TRUE;
	else
		return MMI_FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_mass_processing()
* DESCRIPTION
*   Draws the in-progress screen for mass operation. (This screen has abort function.)
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/

//gdi_handle* PhbMassOperateHandlePtr=  NULL;
U8* PhbMassOperateBufPtr = NULL;
#define SCRMEM_PHBMASS_LAYER_POOL_SIZE (GDI_LCD_WIDTH *GDI_LCD_HEIGHT * (16>>3) + 3)

U8 mmi_phb_mass_process_history_hdlr( void* in_param )
{
        if(NULL !=PhbMassOperateBufPtr )
        {
            OslMfree((void*)PhbMassOperateBufPtr);
            PhbMassOperateBufPtr = NULL;
        }   
	return FALSE;
}

void mmi_phb_entry_mass_processing(void)
{
       gdi_handle HandleBase = 0;
	EntryNewScreen(SCR_COPY_ALL_PROGRESS, mmi_phb_exit_mass_processing, NULL, NULL);

        if(NULL !=PhbMassOperateBufPtr )
        {
            gdi_layer_get_active(&HandleBase );
            gdi_layer_lock_frame_buffer();
            memcpy(((gdi_layer_struct*)HandleBase)->buf_ptr,PhbMassOperateBufPtr ,SCRMEM_PHBMASS_LAYER_POOL_SIZE);
            gdi_layer_unlock_frame_buffer();
            OslMfree((void*)PhbMassOperateBufPtr);
            PhbMassOperateBufPtr = NULL;
        }   


	ShowCategory8Screen(STR_SCR_PBOOK_CAPTION,IMG_SCR_PBOOK_CAPTION,
		0, 0, STR_GLOBAL_ABORT, IMG_COPY_ALL_ABORT,
		g_phb_message, g_phb_message_icon, NULL);

	/*If this screen is deleted, call back function.*/
  	SetDelScrnIDCallbackHandler(SCR_COPY_ALL_PROGRESS, (HistoryDelCBPtr) mmi_phb_mass_processing_scr_del_callback);

	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);

	SetRightSoftkeyFunction(mmi_phb_mass_processing_abort,KEY_EVENT_UP);
    
	SetDelScrnIDCallbackHandler( SCR_COPY_ALL_PROGRESS, (HistoryDelCBPtr)mmi_phb_mass_process_history_hdlr );
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_mass_processing()
* DESCRIPTION
*   Exit function to mmi_phb_entry_mass_processing()
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_mass_processing(void)
{
	history_t   h;
	S16		nHistory	=	0;
       gdi_handle HandleBase = 0;
	if(g_phb_cntx.processing)
	{
            gdi_layer_get_active(&HandleBase );
            PhbMassOperateBufPtr = (U8*)OslMalloc(SCRMEM_PHBMASS_LAYER_POOL_SIZE);
            if( PhbMassOperateBufPtr!= NULL)
            {
                gdi_layer_lock_frame_buffer();
                memset(PhbMassOperateBufPtr,0,SCRMEM_PHBMASS_LAYER_POOL_SIZE);
                memcpy(PhbMassOperateBufPtr , ((gdi_layer_struct*)HandleBase)->buf_ptr,SCRMEM_PHBMASS_LAYER_POOL_SIZE);
                gdi_layer_unlock_frame_buffer();
            }
            
		g_phb_cntx.end_scr_id = SCR_COPY_ALL_PROGRESS;
		h.scrnID = SCR_COPY_ALL_PROGRESS;
		h.entryFuncPtr = mmi_phb_entry_mass_processing;
		pfnUnicodeStrcpy((PS8)h.inputBuffer, (PS8)&nHistory);
		GetCategoryHistory(h.guiBuffer);
		AddHistory(h);
	}
}

#define MMI_PHB_LIST
/*****************************************************************************
* FUNCTION
*  IdleHandleKeypadLockProcess()
* DESCRIPTION
*   This fn handle the press of star key , when LSK is pressed first
*	(Should move to Idle Scren App.)
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void IdleHandleKeypadLockProcess(void)
{
	StopTimer(KEYPAD_LOCK_TIMER);
	g_idle_context.RskPressedFromIdleApp=0;

#ifdef	__MMI_KEYPAD_LOCK_PATTERN_2__
	g_idle_context.ToMainMenuScrFromIdleApp=0;
#endif
}

/*****************************************************************************
* FUNCTION
*  IdleHandlePoundKeyForKeypadLock()
* DESCRIPTION
*   This fn handle the press of star key , when LSK is pressed first
*	(Should move to Idle Scren App.)
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void IdleHandlePoundKeyForKeypadLock(void)
{

#ifdef	__MMI_KEYPAD_LOCK_PATTERN_1__
	if(g_idle_context.RskPressedFromIdleApp == 0)
	   return;
	g_idle_context.RskPressedFromIdleApp = 0;
#endif

#ifdef	__MMI_KEYPAD_LOCK_PATTERN_2__
     if( g_idle_context.ToMainMenuScrFromIdleApp == 0 )
        return;
	g_idle_context.ToMainMenuScrFromIdleApp = 0;
#endif

		StopTimer(KEYPAD_LOCK_TIMER);
		gKeyPadLockFlag = 1;

//Lisen 02182005
#ifdef __MMI_KEYPADLOCK_WITH_KEYTONE__

#else
              mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);
#endif

#ifdef __MMI_TOUCH_SCREEN__
		ChangeStatusIconImage(STATUS_ICON_KEYPAD_LOCK, IMG_SI_KEYPAD_LOCK);
		UpdateStatusIcons(); 
#else
		IdleSetStatusIcon(STATUS_ICON_KEYPAD_LOCK);
#endif
		DisplayIdleScreen();
}

/*****************************************************************************
* FUNCTION
*  IdleHandleStarKeyForKeypadLock()
* DESCRIPTION
*   This fn handle the press of star key , when * is long pressed 
*	(Should move to Idle Scren App.)
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void IdleHandleStarKeyForKeypadLock(void)
{
		gKeyPadLockFlag = 1;

//Lisen 02182005
#ifdef __MMI_KEYPADLOCK_WITH_KEYTONE__

#else
              mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);
#endif

#ifdef __MMI_TOUCH_SCREEN__
		ChangeStatusIconImage(STATUS_ICON_KEYPAD_LOCK, IMG_SI_KEYPAD_LOCK);
		UpdateStatusIcons(); 
#else
		IdleSetStatusIcon(STATUS_ICON_KEYPAD_LOCK);
#endif
		DisplayIdleScreen();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_list_allocate_filter()
* DESCRIPTION
*   allocate memory phonebook list filter buffer. The filter is used for display phonebook list
*   with certain conditions.
* PARAMETERS
* void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_list_allocate_filter(void)
{
#if (MAX_PB_ENTRIES >= 1000)
	g_phb_cntx.list_filter = (U16*)g_phb_list_filter;
#else
	if(g_phb_cntx.list_filter == NULL)
	g_phb_cntx.list_filter = OslMalloc(MAX_PB_ENTRIES*2);
#endif /* MAX_PB_ENTRIES >= 1000 */
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_list_free_filter()
* DESCRIPTION
*   free memory phonebook list filter buffer.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_list_free_filter(void)
{
#if (MAX_PB_ENTRIES < 1000)
	if(g_phb_cntx.list_filter)
	{
		OslMfree(g_phb_cntx.list_filter);
		g_phb_cntx.list_filter = NULL;
	}
#endif /* MAX_PB_ENTRIES < 1000 */
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_list_pre_entry()
* DESCRIPTION
*   This function provide call back function before enter phonebook list
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#ifdef __MMI_VIP_FUNCTION__
#include "securitysetup.h"
void mmi_VIP_get_phb_suited_number(void)
{
	U16 index = 0;
	S8 num_buffer[(MAX_PB_NUMBER_LENGTH+1)*ENCODING_LENGTH];
	U16 nSuited = 0;
	U16 i = 0;

	if(!g_phb_cntx.phb_ready || g_phb_cntx.processing ||PhoneBookEntryCount == 0)
	{
		memset(g_vip_context.PHBIndexMap, 0, sizeof(g_vip_context.PHBIndexMap));
		g_vip_context.nPHBSuited = 0;
		return;
	}
		
	for(index = 0; index < PhoneBookEntryCount; index++)
	{
		memset(num_buffer, 0, sizeof(num_buffer));
		mmi_phb_convert_get_ucs2_number(num_buffer, g_phb_name_index[index]);

		if(mmi_VIP_is_vip_number(num_buffer))
		{
			nSuited++;
		}
		else
		{
			g_vip_context.PHBIndexMap[i++] = index;
		}
	}
	g_vip_context.nPHBSuited = nSuited;
}
#endif

void mmi_phb_list_pre_entry(void)
{
	U16 nVIPSuited = 0;
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_list_pre_entry.>\n",__FILE__,__LINE__));
	
	#ifdef __MMI_VIP_FUNCTION__
	mmi_VIP_get_phb_suited_number();
	nVIPSuited = g_vip_context.nPHBSuited;
	#endif
	
	g_phb_highlight_alpha = 0;
#if defined(__MMI_FILE_MANAGER__) && defined(MMI_ON_HARDWARE_P)
	if (UI_device_height == 220 && UI_device_width == 176)
		mmi_phb_entry_image_list1();
	else
#endif
	mmi_phb_entry_list(PhoneBookEntryCount - nVIPSuited, /*Total Entry*/
							  STR_SCR_PBOOK_VIEW_CAPTION, /*Title String*/
							  IMG_SCR_PBOOK_CAPTION, /*Title Image*/
							  STR_GLOBAL_OPTIONS, /*LSK*/
							  IMG_GLOBAL_OPTIONS, /*LSK*/
#if defined(__PROJECT_GALLITE_C01__)
                                                         mmi_phb_get_index, /*Highlight Callback*/
#else
#if defined( __MMI_INTELLIGENT_CALL_ALERT__ )
							  mmi_phb_get_index_speak_name,
#else
							  mmi_phb_get_index, /*Highlight Callback*/
#endif
#endif
							  mmi_phb_entry_op_option, /*LSK Callback*/
							  mmi_phb_choose_number_normal, /*SEND Key Callback*/
							  mmi_phb_list_get_item, /*List Callback*/
							  mmi_phb_list_get_hint, /*Hint Callback*/
							  mmi_phb_list_pre_entry, /*Re-Entry Callback*/
							  TRUE,	/*Alpha Index*/
							  TRUE);	/*Right Arrow Key*/
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_list_pre_entry_second_level()
* DESCRIPTION
*   This function provide call back function before enter phonebook list
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_list_pre_entry_second_level(void)
{
	void (*LSKEntryPtr)(void) = NULL;
	void (*SendKeyPtr)(void) = NULL;

	if((phbListView == MMI_PHB_LIST_FOR_SMS)||(phbListView == MMI_PHB_LIST_FOR_CHAT) || (phbListView == MMI_PHB_LIST_FOR_SMS_SEARCH_NAME)
		||(phbListView == MMI_PHB_LIST_FOR_SMS_INSERT))
	{
		LSKEntryPtr = mmi_phb_sms_send_data_choose_entry;
		SendKeyPtr = mmi_phb_sms_send_data_choose_entry;
		if(phbListView != MMI_PHB_LIST_FOR_SMS_SEARCH_NAME)
			g_phb_cntx.highlight_entry = 0;
	}
	if(phbListView == MMI_PHB_LIST_FOR_CHAT_NO_CHECK ||phbListView == MMI_PHB_LIST_FOR_SMS_INSERT_NO_CHECK)
	{
		LSKEntryPtr = mmi_phb_sms_send_data_for_entry_name_only;
		SendKeyPtr = mmi_phb_sms_send_data_for_entry_name_only;
		g_phb_cntx.highlight_entry = 0;
	}

	if(phbListView == MMI_PHB_LIST_FOR_SPEED_DIAL)
	{
		LSKEntryPtr = PhbSetSpeedDialFormList;
		g_phb_cntx.highlight_entry = 0;
	}
#if defined(MMS_SUPPORT)
	if(phbListView == MMI_PHB_LIST_FOR_MMS_ALL)
	{
		LSKEntryPtr = mmi_phb_entry_mms_choose_number;
		g_phb_cntx.highlight_entry = 0;
	}
#endif
#if defined(__MMI_EMAIL__)
		if(phbListView == MMI_PHB_LIST_FOR_EMAIL_APP)
		{
			LSKEntryPtr = mmi_phb_email_choose_entry;
		g_phb_cntx.highlight_entry = 0;
		}
#endif
	if(phbListView == MMI_PHB_LIST_FOR_ALL_SHARED)
	{
		LSKEntryPtr = mmi_phb_generic_enter_list_result;
		g_phb_cntx.highlight_entry = 0;
	}

	mmi_phb_entry_list(PhoneBookEntryCount, /*Total Entry*/
							  STR_SCR_PBOOK_VIEW_CAPTION, /*Title String*/
							  IMG_SCR_PBOOK_CAPTION, /*Title Image*/
							  STR_GLOBAL_OK, /*LSK*/
							  IMG_GLOBAL_OK, /*LSK*/
							  mmi_phb_get_index_second_level, /*Highlight Callback*/
							  LSKEntryPtr, /*LSK Callback*/
							  SendKeyPtr, /*SEND Key Callback*/
							  mmi_phb_list_get_item, /*List Callback*/
							  mmi_phb_list_get_hint, /*Hint Callback*/
							  mmi_phb_list_pre_entry_second_level, /*Re-Entry Callback*/
							  TRUE,	/*Alpha Index*/
							  TRUE);	/*Right Arrow Key*/
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_list_pre_entry_for_fmgr_in_nvram()
* DESCRIPTION
*   This function provide call back function before enter phonebook list
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#if defined(__MMI_FILE_MANAGER__)
void mmi_phb_list_pre_entry_for_fmgr_in_nvram(void)
{
	U16 i;
	U16 EntryCount = 0;
	U16 store_index;
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_list_pre_entry_for_fmgr_in_nvram.>\n",__FILE__,__LINE__));

	if(g_phb_cntx.phb_ready && !g_phb_cntx.processing)
	{
		/*Allocate buffer for phonebook memeber list filter*/
		mmi_phb_list_allocate_filter();

		/*Find out entry in NVRAM and put it into filter list*/
		for(i=0 ; i < PhoneBookEntryCount ; i++)
		{
			store_index = g_phb_name_index[i];
			if(store_index < MAX_PB_PHONE_ENTRIES)
			{
				g_phb_cntx.list_filter[EntryCount] = store_index;
				EntryCount++;
			}
		}

		if(EntryCount)
		{
			g_phb_cntx.highlight_entry = 0;
			mmi_phb_entry_list(EntryCount, 				/*Total Entry*/
							  STR_SCR_PBOOK_VIEW_CAPTION, /*Title String*/
							  IMG_SCR_PBOOK_CAPTION, /*Title Image*/
							  STR_GLOBAL_OK, 				/*LSK*/
							  IMG_GLOBAL_OK, 				/*LSK*/
							  mmi_phb_get_index_by_store_location, 		/*Highlight Callback*/
							  mmi_phb_fmgr_pre_edit_entry, 	/*LSK Callback*/
							  NULL, 									/*SEND Key Callback*/
							  mmi_phb_filter_list_get_item, 	/*List Callback*/
							  mmi_phb_filter_list_get_hint, 	/*Hint Callback*/
							  mmi_phb_list_pre_entry_for_fmgr_in_nvram,	/*Re-Entry Callback*/
							  FALSE,	/*Alpha Index*/
							  TRUE);	/*Right Arrow Key*/

		}
		else
		{
			DisplayPopup((PU8)GetString(STR_ID_PHB_NO_ENTRY_TO_SELECT), IMG_GLOBAL_EMPTY, TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
			mmi_phb_list_free_filter();
		}
	}
	else
		mmi_phb_entry_not_ready();

}
#endif

/*****************************************************************************
* FUNCTION
*  mmi_phb_list_pre_entry_for_mms_number_and_email()
* DESCRIPTION
*   This function provide call back function before enter phonebook list
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#ifdef JATAAYU_SUPPORT//#ifdefined(MMS_SUPPORT)//fengzb 2009-3-3
void mmi_phb_list_pre_entry_for_mms_number_and_email(void)
{
	U16 i;
	U16 EntryCount = 0;
	U16 store_index;

	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_list_pre_entry_for_mms_number_and_email.>\n",__FILE__,__LINE__));

	if(g_phb_cntx.phb_ready && !g_phb_cntx.processing)
	{
		/*Allocate buffer for phonebook memeber list filter*/
		mmi_phb_list_allocate_filter();

		/*Build list according to filter*/
		for(i=0 ; i < PhoneBookEntryCount ; i++)
		{
			store_index = g_phb_name_index[i];
			if( ((phbListView == MMI_PHB_LIST_FOR_MMS_EMAIL) && (PhoneBook[store_index].field & MMI_PHB_ENTRY_FIELD_EMAIL) )||
				((phbListView == MMI_PHB_LIST_FOR_MMS_NUMBER) && (PhoneBook[store_index].field & 0x1E)) )
			{
				g_phb_cntx.list_filter[EntryCount] = store_index;
				EntryCount++;
			}
		}

		if(EntryCount)
		{
			g_phb_cntx.highlight_entry = 0;
			mmi_phb_entry_list(EntryCount, 				/*Total Entry*/
							  STR_SCR_PBOOK_VIEW_CAPTION, /*Title String*/
							  IMG_SCR_PBOOK_CAPTION, /*Title Image*/
							  STR_GLOBAL_OK, 				/*LSK*/
							  IMG_GLOBAL_OK, 				/*LSK*/
							  mmi_phb_get_index_by_store_location_second, 		/*Highlight Callback*/
							  mmi_phb_entry_mms_choose_number, 	/*LSK Callback*/
							  mmi_phb_entry_mms_choose_number, 		/*SEND Key Callback*/
							  mmi_phb_mms_email_list_get_item, 	/*List Callback*/
							  mmi_phb_mms_email_list_get_hint, 	/*Hint Callback*/
							  mmi_phb_list_pre_entry_for_mms_number_and_email,	/*Re-Entry Callback*/
							  TRUE,	/*Alpha Index*/
							  TRUE);	/*Right Arrow Key*/

		}
		else
		{
			DisplayPopup((PU8)GetString(STR_ID_PHB_NO_ENTRY_TO_SELECT), IMG_GLOBAL_EMPTY, TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
			mmi_phb_list_free_filter();
		}
	}
	else
		mmi_phb_entry_not_ready();
}
#endif

/*****************************************************************************
* FUNCTION
*  mmi_phb_list_pre_delete_one_by_one()
* DESCRIPTION
*   This function provide call back function before enter phonebook list
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_list_pre_delete_one_by_one(void)
{
	 phbListView = MMI_PHB_LIST_FOR_DELETE_ONE_BY_ONE;
	g_phb_cntx.highlight_entry = 0;
	mmi_phb_entry_list(PhoneBookEntryCount, /*Total Entry*/
							  STR_SCR_PBOOK_VIEW_CAPTION, /*Title String*/
							  IMG_SCR_PBOOK_CAPTION, /*Title Image*/
							  STR_GLOBAL_DELETE, /*LSK*/
							  0, /*IMAGE LSK*/
							  mmi_phb_get_index, /*Highlight Callback*/
							  mmi_phb_entry_op_delete_confirm, /*LSK Callback*/
							  NULL, /*SEND Key Callback*/
							  mmi_phb_list_get_item, /*List Callback*/
							  mmi_phb_list_get_hint, /*Hint Callback*/
							  mmi_phb_list_pre_delete_one_by_one, /*Re-Entry Callback*/
							  TRUE,	/*Alpha Index*/
							  FALSE);	/*Right Arrow Key*/
}

/*****************************************************************************
* FUNCTION
*  EntryPhbList()
* DESCRIPTION
*   This function is for entering phonebook entry List
*
* PARAMETERS
* void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_list(U16 EntryCount, U16 STR_TITLE, U16 IMG_TITLE,U16 STR_LSK, U16 IMG_LSK, void (*HighlightPtr)(S32), FunctionPtr LSKPtr, FunctionPtr SendKeyPtr, GetItemPtr ItemPtr, GetHintPtr HintPtr, FunctionPtr EntryPtr, BOOL alpha_index, BOOL right_arrow)
{
	U8* guiBuffer;
	U16 nVIPSuited = 0;
	mmi_trace(g_sw_PHB,"PHB mmi_phb_entry_list ready = %d, process = %d", g_phb_cntx.phb_ready, g_phb_cntx.processing);

	#ifdef __MMI_VIP_FUNCTION__
	nVIPSuited = g_vip_context.nPHBSuited;
	#endif
	
	/*Check if it can be list first*/
	if(!g_phb_cntx.phb_ready || g_phb_cntx.processing)
	{
		mmi_phb_list_free_filter();
		mmi_phb_entry_not_ready();
		return;
	}
	else if(PhoneBookEntryCount - nVIPSuited == 0)
	{
		mmi_phb_list_free_filter();
		DisplayPopup((PU8)GetString(STR_NO_ENTRIES_MESSAGE), IMG_GLOBAL_EMPTY, TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
		return;
	}

	/*Check if phonebook entry has been updated, and if need to use alpha_index function.*/
	if(g_phb_cntx.refresh_list && alpha_index)
	{
		mmi_phb_list_build_alpha_index(NULL,0,TRUE,MMI_STORAGE_NONE);
		g_phb_cntx.refresh_list = MMI_PHB_ENTRY_NO_CHANGE;
	}

	EntryNewScreen(SCR_PBOOK_LIST, mmi_phb_exit_list, EntryPtr, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_PBOOK_LIST);

	if(g_phb_cntx.new_highlight_entry < 0xffff)
	{
		change_list_menu_category_history(guiBuffer, g_phb_cntx.new_highlight_entry, EntryCount, 0);
		g_phb_cntx.new_highlight_entry = 0xffff;
	}

#if (defined __MMI_TOUCH_SCREEN__) && (defined __MMI_WGUI_CSK_ENABLE__)
	if (phbListView == MMI_PHB_LIST_FOR_PHB 

		&& HasValidSIM()
		#ifdef __MMI_MULTI_SIM__
		&& (MTPNP_AD_Get_UsableSide_Number() > 0)
		#endif
  #ifdef __FLIGHT_MODE_SUPPORT__
		&& mmi_bootup_get_active_flight_mode() !=1
  #endif
	)
	{
		EnableCenterSoftkey(0, IMG_CSK_DIAL_ICON);
	}
#endif /* __MMI_TOUCH_SCREEN__ && __MMI_WGUI_CSK_ENABLE__ */
#if ((!defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__))
  	if (phbListView == MMI_PHB_LIST_FOR_PHB 
		&& HasValidSIM()
  #ifdef __FLIGHT_MODE_SUPPORT__
		&& mmi_bootup_get_active_flight_mode() !=1
  #endif
	)
	{
		  DrawSimulateCenterKey();
	}
 
  #endif
	RegisterHighlightHandler(HighlightPtr);
	ShowCategory184Screen(STR_TITLE,
									IMG_TITLE,
									STR_LSK,
									IMG_LSK,
									STR_GLOBAL_BACK,
									IMG_GLOBAL_BACK,
									EntryCount,
									ItemPtr,
									HintPtr,
									g_phb_cntx.highlight_entry,
									guiBuffer);

	/*register multi-tap function for fast jump, for full list only*/
	if(alpha_index)
	{
		register_multitap_no_draw_key_handlers();
		set_multitap_functions(mmi_phb_list_alpha_index, mmi_phb_list_alpha_index_input_callback);
		change_multitap_mode(0);
	}

	SetLeftSoftkeyFunction(LSKPtr,KEY_EVENT_UP);
	if(right_arrow)
		SetKeyHandler(LSKPtr,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	else
		ClearKeyHandler(KEY_RIGHT_ARROW, KEY_EVENT_DOWN);

	if (HasValidSIM()
  #ifdef __FLIGHT_MODE_SUPPORT__
		&& mmi_bootup_get_active_flight_mode() != 1
  #endif
	)
	{
#ifdef __MMI_MULTI_SIM__
	if(SendKeyPtr != NULL)
	{
		MTPNP_PFAL_CC_HandleSendKeys(SendKeyPtr,KEY_EVENT_DOWN);
#if defined(TGT_GALLITE_G800)||defined(TGT_TD_4SIM)
		ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
		SetKeyHandler(LSKPtr,KEY_ENTER,KEY_EVENT_UP );
#endif
	}
#else
    SetKeyHandler(SendKeyPtr, KEY_SEND, KEY_EVENT_DOWN);
#endif

#if (defined __MMI_TOUCH_SCREEN__) && (defined __MMI_WGUI_CSK_ENABLE__)
    if (phbListView == MMI_PHB_LIST_FOR_PHB)
    {
    #ifdef __MMI_MULTI_SIM__
        if(SendKeyPtr != MTPNP_NULL)
        {
            MTPNP_PFAL_CC_SetCenterFunc(SendKeyPtr);
            SetCenterSoftkeyFunction(MTPNP_PFAL_CC_CSKHandler, KEY_EVENT_UP);
        }
        else
        {
            SetCenterSoftkeyFunction(SendKeyPtr, KEY_EVENT_UP);
        }
    #else  /* __MMI_MULTI_SIM__ */
        SetCenterSoftkeyFunction(SendKeyPtr, KEY_EVENT_UP);
    #endif /* __MMI_MULTI_SIM__ */
    }    
#else
    if (phbListView == MMI_PHB_LIST_FOR_PHB)
    {
    #ifdef __MMI_MULTI_SIM__
        if(SendKeyPtr != MTPNP_NULL)
        {
            MTPNP_PFAL_CC_SetCenterFunc(SendKeyPtr);
        }
    #endif /* __MMI_MULTI_SIM__ */
    }  
#endif /* (defined __MMI_TOUCH_SCREEN__) && (defined __MMI_WGUI_CSK_ENABLE__) */

#if ((!defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__))
	if (phbListView == MMI_PHB_LIST_FOR_PHB)
    {
        SimulateCenterKeyDailFunction(SendKeyPtr, KEY_EVENT_UP);
        SetNeedCenterKeyFlag(TRUE);
    }
#endif

	}
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);
#if defined(__MMI_MAINLCD_220X176__)
	SetKeyHandler(mmi_phb_entry_op_view_entry,KEY_ENTER,KEY_EVENT_UP);
#endif
#if defined(__IP_NUMBER__) && defined(__MMI_IP_KEY__)	/*For IP Dial Key*/
/* under construction !*/
  #ifdef __FLIGHT_MODE_SUPPORT__
/* under construction !*/
  #endif
/* under construction !*/
/* under construction !*/
#endif
}

/*****************************************************************************
* FUNCTION
*  ExitPhbList()
* DESCRIPTION
*   This function is for existing entering phonebook entry List
*
* PARAMETERS
* void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_list(void)
{
	if(g_idle_context.ToNameScrFromIdleApp)
		g_phb_cntx.start_scr_id = SCR_PBOOK_LIST;
	g_phb_cntx.end_scr_id = SCR_PBOOK_LIST;

#if defined(__MMI_INTELLIGENT_CALL_ALERT__)
	StopTimer(PHB_LIST_SPEAK_NAME_TIMER);
	if( g_phb_is_speaking_name )
		mdi_audio_stop_string();
#endif

	reset_multitaps();

	StopTimer(KEYPAD_LOCK_TIMER);
#if defined(__MMI_VRSD_DIAL__)
	StopTimer(VRSD_DIAL_PLAYBACK_TIMER);
#endif
	g_idle_context.ToNameScrFromIdleApp=0;
	g_idle_context.RskPressedFromIdleApp=0;

	/* Free Member List Filter if exists*/
	mmi_phb_list_free_filter();
#if (defined __MMI_TOUCH_SCREEN__) && (defined __MMI_WGUI_CSK_ENABLE__)
	if (HasValidSIM()
  #ifdef __FLIGHT_MODE_SUPPORT__
		&& mmi_bootup_get_active_flight_mode() != 1
  #endif
  	)
	{
		ResetCenterSoftkey();
	}
#endif /* __MMI_TOUCH_SCREEN__ && __MMI_WGUI_CSK_ENABLE__ */

#if ((!defined  __MMI_WGUI_CSK_ENABLE__) && defined (__MMI_TOUCH_SCREEN__))
  ClearSimulateCenterKeyDailhandlers();
  SetNeedCenterKeyFlag(FALSE);
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_member_list_get_item()
* DESCRIPTION
*
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
pBOOL mmi_phb_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{
	U16 store_index = 0;
	S32 m_item_idnex = item_index;

	if((item_index < 0) || (item_index >= PhoneBookEntryCount))
		return FALSE;

	
	#ifdef __MMI_VIP_FUNCTION__
	m_item_idnex = (S32)g_vip_context.PHBIndexMap[item_index];
	#endif

	store_index = g_phb_name_index[m_item_idnex];

	if (pfnUnicodeStrlen((S8*)PhoneBook[store_index].alpha_id.name))
		pfnUnicodeStrcpy((S8*) str_buff, (S8*)PhoneBook[store_index].alpha_id.name);
	else
		mmi_phb_convert_get_ucs2_number((S8*)str_buff, store_index);

#ifdef __MMI_MULTI_SIM__
	if (store_index >= MAX_PB_PHONE_ENTRIES)
	{
		U8 nSimIndex = (store_index - MAX_PB_PHONE_ENTRIES) / MAX_PB_SIM_ENTRIES;
	    //*img_buff_p = get_image(IMG_ID_CARD1_PHB_STORAGE_SIM);
		 *img_buff_p = get_image(IMG_STORAGE_SIM_1 + nSimIndex);
	}
	else
	{
	    *img_buff_p = get_image(IMG_STORAGE_HANDSET);
	}
#else /* __MMI_MULTI_SIM__ */
	if (store_index >= MAX_PB_PHONE_ENTRIES)
	{
	    *img_buff_p = get_image(IMG_STORAGE_SIM);
	}
	else
	{
	    *img_buff_p = get_image(IMG_STORAGE_HANDSET);
	}
#endif /* __MMI_MULTI_SIM__ */	

	return TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_member_list_get_hint()
* DESCRIPTION
*
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S32 mmi_phb_list_get_hint ( S32 item_index, UI_string_type *hint_array )
{
	U16 store_index;
	S8 temp_number[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];
	S32 m_item_idnex = item_index;

	#ifdef __MMI_VIP_FUNCTION__
	m_item_idnex = (S32)g_vip_context.PHBIndexMap[item_index];
	#endif

	store_index = g_phb_name_index[m_item_idnex];

	mmi_phb_convert_get_ucs2_number((S8*)temp_number, store_index);

	if (pfnUnicodeStrlen((S8*)PhoneBook[store_index].alpha_id.name) && pfnUnicodeStrlen((S8*)temp_number))
		pfnUnicodeStrcpy((S8*)hint_array[0], (S8*)temp_number);
	else
		return 0; /*No Hint Data*/

	return 1;  /*One hint data only, can be more hints.*/
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_filter_list_get_item()
* DESCRIPTION
*
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
pBOOL mmi_phb_filter_list_get_item( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{
	U16 store_index;

	store_index = g_phb_cntx.list_filter[item_index];

	if (pfnUnicodeStrlen((S8*)PhoneBook[store_index].alpha_id.name))
		pfnUnicodeStrcpy((S8*) str_buff, (S8*)PhoneBook[store_index].alpha_id.name);
	else
		mmi_phb_convert_get_ucs2_number((S8*)str_buff, store_index);

#ifdef __MMI_MULTI_SIM__
		if (store_index < MAX_PB_PHONE_ENTRIES)
		{
			*img_buff_p = get_image(IMG_STORAGE_HANDSET);
		}
		else
		{
			U8 nSimIndex = (store_index - MAX_PB_PHONE_ENTRIES) / MAX_PB_SIM_ENTRIES;
			*img_buff_p = get_image(IMG_STORAGE_SIM_1 + nSimIndex);
		}
#else
		if (store_index < MAX_PB_PHONE_ENTRIES)
			*img_buff_p = get_image(IMG_STORAGE_HANDSET);
		else
			*img_buff_p = get_image(IMG_STORAGE_SIM);
#endif	
	return TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_filter_list_get_hint()
* DESCRIPTION
*
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S32 mmi_phb_filter_list_get_hint ( S32 item_index, UI_string_type *hint_array )
{
	U16 store_index;
	S8 temp_number[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];

	store_index = g_phb_cntx.list_filter[item_index];

	mmi_phb_convert_get_ucs2_number((S8*)temp_number, store_index);

	if (pfnUnicodeStrlen((S8*)PhoneBook[store_index].alpha_id.name) && pfnUnicodeStrlen((S8*)temp_number))
		pfnUnicodeStrcpy((S8*)hint_array[0], (S8*)temp_number);
	else
		return 0;

	return 1;  /*One hint data only, can be more hints.*/
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_list_get_null_hint()
* DESCRIPTION
*   No hint needed in the dynamic list.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
S32 mmi_phb_list_get_null_hint ( S32 item_index, UI_string_type *hint_array )
{
	return 0;
}

#ifdef __MMI_MULTI_SIM__
#if 1
extern MMI_PHB_STORAGE_LOCATION phbcopymovedes;
extern MMI_PHB_STORAGE_LOCATION phbcopymovesrc;
extern void DstSelectHighLightHandler(S32 hiliteid);

void mmi_phb_copy_move_select_dst(FuncPtr lskFun, FuncPtr entryFun)
{
	U8* guiBuffer;
	U8* nStrList[MMI_SIM_NUMBER + 1];
	U16 nNumofItem = 0;
	U8 hiliteStorage = 0;
	U8 i;
	MMI_PHB_STORAGE_LOCATION temp_dst = MMI_STORAGE_NONE;

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(!MTPNP_PFAL_Is_CardAbsent(i) && (g_phb_cntx.sim_used[i] < g_phb_cntx.sim_total[i]) && (phbcopymovesrc != MMI_SIM + i))
		{
			temp_dst = MMI_SIM + i;
			nStrList[nNumofItem++] = (PU8)GetString(STRING_MTPNP_SIM1 + i);
		}
	}

	if(g_phb_cntx.phone_used < g_phb_cntx.phone_total && phbcopymovesrc != MMI_NVRAM)
	{
		nStrList[nNumofItem++] = (PU8)GetString(STR_STORAGE_PHONE);
		if(MMI_STORAGE_NONE == temp_dst)
			temp_dst = MMI_NVRAM;
	}
	
	if(nNumofItem > 1)
	{
		EntryNewScreen(SCR_SOPY_MOVE_SELECT_DST, NULL, entryFun, NULL);
		guiBuffer = GetCurrGuiBuffer(SCR_SOPY_MOVE_SELECT_DST);

		RegisterHighlightHandler(DstSelectHighLightHandler);

		// TODO: prepare hint list , titile

		ShowCategory109Screen(STR_MITEM1013, IMG_SCR_PBOOK_CAPTION,
					STR_GLOBAL_OK, IMG_GLOBAL_OK, STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
					nNumofItem, nStrList, NULL,
					hiliteStorage, guiBuffer);
		SetLeftSoftkeyFunction(lskFun, KEY_EVENT_UP);
		SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
	}
	else if(nNumofItem == 1)
	{
		phbcopymovedes = temp_dst;
		(*lskFun)();
	}
	else
	{
		DisplayPopup((PU8)GetString(STR_NO_ENTRIES_MESSAGE), IMG_GLOBAL_EMPTY,  TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
	}
}

/*****************************************************************************
*
*
*
*
*
*****************************************************************************/
//phone copy
void EntryScrForPHBCopyChoose(void)
{
	U16 store_index = 0;
	store_index = g_phb_name_index[g_phb_cntx.active_index];
	mmi_trace(g_sw_PHB,"PHB Func: %s actvi = %d, store_index = %d", __FUNCTION__, g_phb_cntx.active_index, store_index);

	if(store_index >= MAX_PB_PHONE_ENTRIES)
	{
		phbcopymovesrc = MMI_SIM + (store_index - MAX_PB_PHONE_ENTRIES)/MAX_PB_SIM_ENTRIES;
	}
	else
	{
		phbcopymovesrc = MMI_NVRAM;
	}
	mmi_phb_copy_move_select_dst(mmi_phb_entry_op_copy_confirm, EntryScrForPHBCopyChoose);
	
	
}

//phone move
void EntryScrForPHBMoveChoose(void)
{
	U16 store_index = 0;


	store_index = g_phb_name_index[g_phb_cntx.active_index];
	mmi_trace(g_sw_PHB,"PHB Func: %s actvi = %d, store_index = %d", __FUNCTION__, g_phb_cntx.active_index, store_index);
	
	if(store_index >= MAX_PB_PHONE_ENTRIES)
	{
		phbcopymovesrc = MMI_SIM + (store_index - MAX_PB_PHONE_ENTRIES)/MAX_PB_SIM_ENTRIES;
	}
	else
	{
		phbcopymovesrc = MMI_NVRAM;
	}
	
	mmi_phb_copy_move_select_dst(mmi_phb_entry_op_move_confirm, EntryScrForPHBMoveChoose);
	
}
#else
/*****************************************************************************
*
*
*
*
*
*****************************************************************************/
//phone copy
void EntryScrForPHBCopyChoose(void)
{
	U8* guiBuffer;
	U16 nStrItemList[10];
	U16 numItems;
	U16 store_index = 0;
	U16 nMenuId = 0;

	EntryNewScreen(SCR_PBOOK_COPY_CHOOSE, ExitScrForPHBCopyChoose, NULL, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_PBOOK_COPY_CHOOSE);

	store_index = g_phb_name_index[g_phb_cntx.active_index];
	mmi_trace(g_sw_PHB,"zhangm++ Func: %s actvi = %d, store_index = %d", __FUNCTION__, g_phb_cntx.active_index, store_index);
	if(store_index >= (MAX_PB_PHONE_ENTRIES + MAX_PB_SIM_ENTRIES))
	{
		if(MTPNP_PFAL_Is_Card1Absent() == TRUE)
		{
			nMenuId = MENU_ID_PHB_SIM2_COPY_CHOOSE_NO_SIM1;
		}
		else
		{
			nMenuId = MENU_ID_PHB_SIM2_COPY_CHOOSE;
		}
	}
	else if (store_index >= MAX_PB_PHONE_ENTRIES && store_index < (MAX_PB_PHONE_ENTRIES+MAX_PB_SIM_ENTRIES))
	{
		if(MTPNP_PFAL_Is_Card2Absent() == TRUE)
		{
			nMenuId = MENU_ID_PHB_SIM1_COPY_CHOOSE_NO_SIM2;
		}
		else
		{
			nMenuId = MENU_ID_PHB_SIM1_COPY_CHOOSE;
		}
	}
	else
	{
		if(MTPNP_PFAL_Is_Card1Absent() == TRUE)
		{
			nMenuId = MENU_ID_PHB_COPY_PHONE_TO_SIM2_NOSIM1;
		}
		else if(MTPNP_PFAL_Is_Card2Absent() == TRUE)
		{
			nMenuId = MENU_ID_PHB_COPY_PHONE_TO_SIM1_NOSIM2;
		}
		else
		{
			nMenuId = MENU_ID_PHB_PHONE_COPY_CHOOSE;
		}			
	}
	
	SetParentHandler (nMenuId);
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	numItems = GetNumOfChild (nMenuId);

	GetSequenceStringIds(nMenuId, nStrItemList);
	ShowCategory52Screen (STR_MITEM1015, IMG_SCR_PBOOK_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  numItems, nStrItemList, (PU16)gIndexIconsImageList,
						  NULL , 0, 0, guiBuffer );
      SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP); 
}

void ExitScrForPHBCopyChoose(void)
{
	history_t currHistory;
	S16 ucs2InputBuffer=0;
	currHistory.scrnID = SCR_PBOOK_COPY_CHOOSE;
	currHistory.entryFuncPtr = EntryScrForPHBCopyChoose;
	pfnUnicodeStrcpy ((S8*)currHistory.inputBuffer, (S8*)&ucs2InputBuffer);
	GetCategoryHistory (currHistory.guiBuffer);
	AddHistory (currHistory);
}

//phone move
void EntryScrForPHBMoveChoose(void)
{
	U8* guiBuffer;
	U16 nStrItemList[10];
	U16 numItems;
	U16 store_index = 0;
	U16 nMenuId = 0;

	EntryNewScreen(SCR_PBOOK_MOVE_CHOOSE, ExitScrForPHBMoveChoose, NULL, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_PBOOK_MOVE_CHOOSE);

	store_index = g_phb_name_index[g_phb_cntx.active_index];
	mmi_trace(g_sw_PHB,"PHB Func: %s actvi = %d, store_index = %d", __FUNCTION__, g_phb_cntx.active_index, store_index);
	

	if(store_index >= (MAX_PB_PHONE_ENTRIES + MAX_PB_SIM_ENTRIES))
	{
		if(MTPNP_PFAL_Is_Card1Absent() == TRUE)
		{
			nMenuId = MENU_ID_PHB_SIM2_MOVE_CHOOSE_NO_SIM1;		
		}
		else
		{
			nMenuId = MENU_ID_PHB_SIM2_MOVE_CHOOSE;
		}
	}
	else if (store_index >= MAX_PB_PHONE_ENTRIES && store_index < (MAX_PB_PHONE_ENTRIES+MAX_PB_SIM_ENTRIES))
	{
		if(MTPNP_PFAL_Is_Card2Absent() == TRUE)
		{
			nMenuId = MENU_ID_PHB_SIM1_MOVE_CHOOSE_NO_SIM2;		
		}
		else
		{
			nMenuId = MENU_ID_PHB_SIM1_MOVE_CHOOSE;
		}	
	}
	else
	{
		if(MTPNP_PFAL_Is_Card1Absent() == TRUE)
		{
			nMenuId = MENU_ID_PHB_MOVE_PHONE_TO_SIM2_NOSIM1;
		}
		else if(MTPNP_PFAL_Is_Card2Absent() == TRUE)
		{
			nMenuId = MENU_ID_PHB_MOVE_PHONE_TO_SIM1_NOSIM2;
		}
		else
		{
			nMenuId = MENU_ID_PHB_PHONE_MOVE_CHOOSE;
		}		
	}
	
	SetParentHandler (nMenuId);
	RegisterHighlightHandler (ExecuteCurrHiliteHandler);
	numItems = GetNumOfChild (nMenuId);

	GetSequenceStringIds(nMenuId, nStrItemList);
	ShowCategory52Screen (STR_MITEM1014, IMG_SCR_PBOOK_CAPTION,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  numItems, nStrItemList, (PU16)gIndexIconsImageList,
						  NULL , 0, 0, guiBuffer );
      SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP); 
}

void ExitScrForPHBMoveChoose(void)
{
	history_t currHistory;
	S16 ucs2InputBuffer=0;
	currHistory.scrnID = SCR_PBOOK_MOVE_CHOOSE;
	currHistory.entryFuncPtr = EntryScrForPHBMoveChoose;
	pfnUnicodeStrcpy ((S8*)currHistory.inputBuffer, (S8*)&ucs2InputBuffer);
	GetCategoryHistory (currHistory.guiBuffer);
	AddHistory (currHistory);
}
#endif
#endif

/*****************************************************************************
* FUNCTION
*  mmi_phb_choose_number_normal()
* DESCRIPTION
*   choose normal phonebook entry number if has many(Only when IP number on)
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_choose_number_normal(void)
{
#if defined(__IP_NUMBER__)
	SetTempUseIPNumber(FALSE);
#endif
	mmi_phb_list_pre_choose_number();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_choose_number_ip_number
* DESCRIPTION
*   choose IP Dial phonebook entry number if has many(Only when IP number on)
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_choose_number_ip_number(void)
{
#if defined(__IP_NUMBER__)
	SetTempUseIPNumber(TRUE);
#endif
	mmi_phb_list_pre_choose_number();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_list_alpha_index()

* DESCRIPTION
*   This function calculates the index of the entry to be highlighted, if the user jumps to some
*   entry using multi-tap on PHB list screen.
* PARAMETERS
*  input	IN	input character
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_list_alpha_index(U16 input)
{
	if(input >= 'A' && input <= 'Z')
	{
		g_phb_highlight_alpha = g_phb_alpha_index_list[input-'A'];
		g_phb_highlight_alpha--;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_list_alpha_index_input_callback()
* DESCRIPTION
*   Jumps to the hilited entry
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_list_alpha_index_input_callback(void)
{
#if defined(__MMI_FILE_MANAGER__) && defined(MMI_ON_HARDWARE_P)
	if (UI_device_height == 220 && UI_device_width == 176)
		Cat212ShortCutHdlr(g_phb_highlight_alpha); /*For image list fast jump*/
	else
#endif
		dynamic_list_goto_item(g_phb_highlight_alpha);		/*Use Dynamic List*/
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_list_build_alpha_index()
* DESCRIPTION
*   Build up alpha index for each characters
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_list_build_alpha_index(U16 *filter_list, U16 filter_value, BOOL is_equal, U8 storage)
{
	U16 i,j, count = 0;
	BOOL valid_ch;
	U8 index = 0xff;
	U16 store_index;
	MMI_PHB_NAME_STRUCT *name_ptr;
#if defined(__MMI_PHB_PINYIN_SORT__)
	U8 * pinyin_ptr;
#endif

	/*Clear First*/
	for(i = 0 ; i < ALPHA_LIST_LENGTH ; i++)
		g_phb_alpha_index_list[i] = 0;

	for (i = 0; i < PhoneBookEntryCount; i++)
	{
		store_index = g_phb_name_index[i];

		/*exclude storage location*/
		if(storage == MMI_SIM && store_index < MAX_PB_PHONE_ENTRIES)
			continue;
		else if(storage == MMI_NVRAM && store_index >= MAX_PB_PHONE_ENTRIES)
			continue;

		/*Filter List*/
		if(filter_list != NULL && is_equal)	/*Exclude item not in the filter list.*/
	{
			if(filter_list[store_index] != filter_value)
				continue;
		}
		else if(filter_list != NULL && !is_equal) /*Exclude item in the filter list.*/
		{
			if(filter_list[store_index] == filter_value)
				continue;
		}

		/*All character store in MMI by using UCS2(two-bytes) format*/
		name_ptr = &PhoneBook[store_index].alpha_id;

		if ((name_ptr->name[1] == 0x00) && (name_ptr->name[0] >= 'a') && (name_ptr->name[0] <= 'z')) /*lower case*/
			{
			index = name_ptr->name[0]  - 'a';
       		valid_ch = TRUE;
         	}
         	else if ((name_ptr->name[1] == 0x00) && (name_ptr->name[0] >= 'A') && (name_ptr->name[0] <= 'Z')) /*upper case*/
         	{
			index = name_ptr->name[0] - 'A';
            		valid_ch = TRUE;
         		}
#if defined(__MMI_PHB_PINYIN_SORT__)
         	else if((name_ptr->name[0] != 0x00) && (name_ptr->name[1] != 0x00))	/*A 2-bytes character*/
         		{
         		mmi_phb_util_get_pinyin_cache(store_index, &pinyin_ptr);
         		if(pinyin_ptr[0] >= 'a' && pinyin_ptr[0] <= 'z')
         		{
         			index = pinyin_ptr[0] - 'a';
				valid_ch = TRUE;
			}
			else
				valid_ch = FALSE;
         	}
#endif
		else
			valid_ch = FALSE;

		/*Assign alpha index*/
		if (index < ALPHA_LIST_LENGTH && g_phb_alpha_index_list[index] == 0 && valid_ch)
			{
			for (j = 0; j < index; j++)
					if (g_phb_alpha_index_list[j] == 0)
					g_phb_alpha_index_list[j] = count+1;
			g_phb_alpha_index_list[index] = count+1;
			}

		count++;
		}

	/*Handle unassigned index*/
	count = 1;
	for (j=0; j<ALPHA_LIST_LENGTH; j++)
	{
		if(g_phb_alpha_index_list[j] == 0)
			g_phb_alpha_index_list[j] = count;
		else
			count = g_phb_alpha_index_list[j];
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_list_pre_choose_number()
* DESCRIPTION
*   Allows the user to choose one number to dial  if the entry has more than one number
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/

void mmi_phb_list_pre_choose_number(void)
{
	mmi_phb_entry_list_choose_number(g_phb_name_index[g_phb_cntx.active_index],
													mmi_phb_list_pre_choose_number,
													mmi_phb_list_make_call,
													MakeCall,
													STR_NO_NUMBER_TO_DIAL,
													TRUE);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_list_choose_number()
* DESCRIPTION
*   Allows the user to choose one number to some action  if the entry has more than one number
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_list_choose_number(U16 store_index, void (*EntryFunc)(void), void (*RSKFunc)(void), void(*ActionFunc)(S8*), U16 empty_str_id, BOOL dial_list)
{
	U8 i;
	U8* guiBuffer;
	U16 numberTypeImageList[4];
	S16 pError;

	mmi_phb_convert_get_ucs2_number(pbNumber, store_index);	/*BCD number format.*/

	if(store_index >= MAX_PB_PHONE_ENTRIES) /*Entry in SIM*/
	{
		if(!pfnUnicodeStrlen(pbNumber))
		{
#if defined(__IP_NUMBER__)
			SetTempUseIPNumber(FALSE);
#endif
			DisplayPopup((PU8)GetString(empty_str_id), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		}
		else
		{
			if(dial_list)	/*Will dial from list after select entry*/
			{
				g_phb_cntx.dial_from_list  = MMI_PHB_PHONEBOOK;
			}
			ActionFunc(pbNumber);
		}
	}
	else
	{
		i = 0;
		if(pfnUnicodeStrlen(pbNumber))
		{
			g_phb_cntx.number_to_dial[i] = (U8*)pbNumber;
			numberTypeImageList[i++] = IMG_MOBILE_NUMBER;
		}

#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
		/*Read Optional Number Fields*/
		//ReadRecord(NVRAM_EF_PHB_FIELDS_LID, (U16)(store_index+1), (void*)&PhoneBookOptionalFields, OPTIONAL_FIELDS_RECORD_SIZE, &pError);
		ReadRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 1, (void*)&PhoneBookOptionalFields, store_index*OPTIONAL_FIELDS_RECORD_SIZE, &pError);
		AnsiiToUnicodeString(pbHomeNumber, (PS8)PhoneBookOptionalFields.homeNumber);
		if(pfnUnicodeStrlen(pbHomeNumber))
		{
			g_phb_cntx.number_to_dial[i] = (PU8)pbHomeNumber;
			numberTypeImageList[i++] = IMG_HOME_NUMBER;
		}
		AnsiiToUnicodeString(pbOfficeNumber, (PS8)PhoneBookOptionalFields.officeNumber);
		if(pfnUnicodeStrlen(pbOfficeNumber))
		{
			g_phb_cntx.number_to_dial[i] = (PU8)pbOfficeNumber;
			numberTypeImageList[i++] = IMG_OFFICE_NUMBER;
		}
		AnsiiToUnicodeString(pbFaxNumber, (PS8)PhoneBookOptionalFields.faxNumber);
		if(pfnUnicodeStrlen(pbFaxNumber))
		{
			g_phb_cntx.number_to_dial[i] = (PU8)pbFaxNumber;
			numberTypeImageList[i++] = IMG_FAX_NUMBER;
		}
#endif

		if(!i)
		{
#if defined(__IP_NUMBER__)
			SetTempUseIPNumber(FALSE);
#endif
			DisplayPopup((PU8)GetString(empty_str_id), IMG_GLOBAL_ERROR,  FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		}
		else if(i==1)
		{
			if(dial_list)	/*Will dial from list after select entry*/
			{
				g_phb_cntx.dial_from_list  = MMI_PHB_PHONEBOOK;
			}
			ActionFunc((PS8)g_phb_cntx.number_to_dial[0]);
		}
		else
		{
#if defined(__IP_NUMBER__) /*Do not keep choose number screen in history when IP dial enable*/
			EntryNewScreen(SCR_CHOOSE_NUMBER, mmi_phb_exit_list_choose_number, NULL, NULL);
#else
			EntryNewScreen(SCR_CHOOSE_NUMBER, mmi_phb_exit_list_choose_number, EntryFunc, NULL);
#endif
			guiBuffer = GetCurrGuiBuffer(SCR_CHOOSE_NUMBER);
			RegisterHighlightHandler(mmi_phb_get_index_third_level);

			ShowCategory53Screen(STR_CHOOSE_NUMBER_CAPTION,
										   IMG_SCR_PBOOK_CAPTION,
							 	 		   STR_GLOBAL_OK,
							 	 		   IMG_GLOBAL_OK,
							 	 		   STR_GLOBAL_BACK,
							 	 		   IMG_GLOBAL_BACK,
							 	 		   i,
							 	 		   g_phb_cntx.number_to_dial,
							 	 		   numberTypeImageList,
							 	 		   NULL,
							 	 		   0,
							 	 		   0,
							 	 		   guiBuffer);

			SetLeftSoftkeyFunction(RSKFunc, KEY_EVENT_UP);
		    SetKeyHandler(RSKFunc, KEY_SEND, KEY_EVENT_DOWN);
			SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
		}
	}
}

/*****************************************************************************
 * FUNCTION
 *  mmi_phb_entry_choose_field
 * DESCRIPTION
 *  Allows the user to choose one number to some action  if the entry has more than one number
 * PARAMETERS
 *  store_index         [IN]        
 *  EntryFunc           [IN]        
 *  RSKFunc             [IN]        
 *  ActionFunc          [IN]        
 *  empty_str_id        [IN]        
 *  dial_list           [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_phb_entry_choose_field(
        U16 store_index,
        U16 field_type,
        FunctionPtr entry_func,
        FunctionPtr lsk_func,
        FunctionPtr rsk_func,
        FunctionPtr send_key_func,
        MMI_BOOL show_if_empty)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 i = 0;
    U8 *gui_buffer;
    U16 number_type_image_list[7];
    U8  *title_string;
#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__) || defined(__MMI_PHB_USIM_FIELD__) || defined(__MMI_VOIP__)
    S16 pError;
#endif

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (field_type & MMI_PHB_ENTRY_FIELD_NUMBER)
    {
        mmi_phb_convert_get_ucs2_number(pbNumber, store_index); /* BCD number format. */
        if (mmi_ucs2strlen(pbNumber))
        {
            g_phb_cntx.number_to_dial[i] = (U8*) pbNumber;
            number_type_image_list[i++] = IMG_MOBILE_NUMBER;
        }
        else if (show_if_empty)
        {
            g_phb_cntx.number_to_dial[i] = (U8*) GetString(STR_ID_PHB_MOBILE_NUMBER);
            number_type_image_list[i++] = IMG_MOBILE_NUMBER;
        }
    }
    
    if (field_type & MMI_PHB_ENTRY_FIELD_OPTIONAL)
    {
        if (store_index >= MAX_PB_PHONE_ENTRIES)    /* Entry in SIM */
        {
		if(!pfnUnicodeStrlen(pbNumber))
		{
#if defined(__IP_NUMBER__)
			SetTempUseIPNumber(FALSE);
#endif
			DisplayPopup((PU8)GetString(STR_GLOBAL_EMPTY), IMG_GLOBAL_ERROR, FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);
		}
	}
        else
        {
        #if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
            /* Read Optional Number Fields */
 		ReadRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 
 		1, 
 		(void*)&PhoneBookOptionalFields, 
 		store_index*OPTIONAL_FIELDS_RECORD_SIZE, 
 		&pError);
                
            if (field_type & MMI_PHB_ENTRY_FIELD_HOME)
            { 
                mmi_asc_to_ucs2(pbHomeNumber, (PS8) PhoneBookOptionalFields.homeNumber);
                if (mmi_ucs2strlen(pbHomeNumber))
                {
                    g_phb_cntx.number_to_dial[i] = (PU8) pbHomeNumber;
                    number_type_image_list[i++] = IMG_HOME_NUMBER;
                }
                else if (show_if_empty)
                {
                    g_phb_cntx.number_to_dial[i] = (U8*) GetString(STR_HOME_NUMBER);
                    number_type_image_list[i++] = IMG_HOME_NUMBER;
                }
            }
            
            if (field_type & MMI_PHB_ENTRY_FIELD_OFFICE)
            {    
                mmi_asc_to_ucs2(pbOfficeNumber, (PS8) PhoneBookOptionalFields.officeNumber);
                if (mmi_ucs2strlen(pbOfficeNumber))
                {
                    g_phb_cntx.number_to_dial[i] = (PU8) pbOfficeNumber;
                    number_type_image_list[i++] = IMG_OFFICE_NUMBER;
                }
                else if (show_if_empty)
                {
                    g_phb_cntx.number_to_dial[i] = (U8*) GetString(STR_OFFICE_NUMBER);
                    number_type_image_list[i++] = IMG_OFFICE_NUMBER;
                }
            }
            
            if (field_type & MMI_PHB_ENTRY_FIELD_FAX)
            {
                mmi_asc_to_ucs2(pbFaxNumber, (PS8) PhoneBookOptionalFields.faxNumber);
                if (mmi_ucs2strlen(pbFaxNumber))
                {
                    g_phb_cntx.number_to_dial[i] = (PU8) pbFaxNumber;
                    number_type_image_list[i++] = IMG_FAX_NUMBER;
                }
                else if (show_if_empty)
                {
                    g_phb_cntx.number_to_dial[i] = (U8*) GetString(STR_FAX_NUMBER);
                    number_type_image_list[i++] = IMG_FAX_NUMBER;
                }
            }
            
            if (field_type & MMI_PHB_ENTRY_FIELD_EMAIL)
            {
                mmi_asc_to_ucs2(pbEmailAddress, (PS8)PhoneBookOptionalFields.emailAddress);
                if (mmi_ucs2strlen(pbEmailAddress))
                {
                    g_phb_cntx.number_to_dial[i] = (PU8) pbEmailAddress;
                    number_type_image_list[i++] = IMG_EMAIL_ADDRESS;
                }
                else if (show_if_empty)
                {
                    g_phb_cntx.number_to_dial[i] = (U8*) GetString(mmi_phb_get_email_address_id(1));
                    number_type_image_list[i++] = IMG_EMAIL_ADDRESS;
                }
            #if defined(__MMI_PHB_OPTIONAL_FIELDS_2ND_EMAIL__)
                mmi_asc_to_ucs2(pbEmailAddress2, (PS8) PhoneBookOptionalFields.emailAddress2);
                if (mmi_ucs2strlen(pbEmailAddress2))
                {
                    g_phb_cntx.number_to_dial[i] = (PU8) pbEmailAddress2;
                    number_type_image_list[i++] = IMG_EMAIL_ADDRESS;
                }
                else if (show_if_empty)
                {
                    g_phb_cntx.number_to_dial[i] = (U8*) GetString(STR_ID_PHB_EMAIL2_ADRESS);
                    number_type_image_list[i++] = IMG_EMAIL_ADDRESS;
                }
            #endif /* __MMI_PHB_OPTIONAL_FIELDS_2ND_EMAIL__ */
            }
        #endif /* !defined(__MMI_PHB_NO_OPTIONAL_FIELD__) */ 
        }
    }

#ifdef __MMI_VOIP__
    if ((store_index < MAX_PB_PHONE_ENTRIES && field_type & MMI_PHB_ENTRY_FIELD_VOIP))  /* make call */
    {
        ReadRecord(
            NVRAM_EF_PHB_VOIP_LID,
            (U16) (store_index + 1),
            (void*)&PhoneBookVoipField,
            VOIP_URI_LEN,
            &pError);
        mmi_asc_to_ucs2(pbVoip, (PS8) PhoneBookVoipField);
        if (mmi_ucs2strlen(pbVoip))
        {
            g_phb_cntx.number_to_dial[i] = (PU8) pbVoip;
            number_type_image_list[i++] = IMG_ID_PHB_VOIP;
        }
        else if (show_if_empty)
        {
            g_phb_cntx.number_to_dial[i] = (U8*) GetString(STR_ID_PHB_VOIP_ID);
            number_type_image_list[i++] = IMG_ID_PHB_VOIP;
        }
    }
#endif /* __MMI_VOIP__ */

    if (!i)
    {
        /* leave no number to dial to popup by ucm for unified behavior, so aribitrary send a zero length num to ucm */
	DisplayPopup((PU8)GetString(STR_GLOBAL_EMPTY), IMG_GLOBAL_ERROR,  FALSE, PHB_NOTIFY_TIMEOUT, ERROR_TONE);

    }
    else if (i == 1)
    {
        g_phb_cntx.active_index_third = 0;
        lsk_func();
    }
    else
    {
        EntryNewScreen(SCR_CHOOSE_NUMBER, mmi_phb_exit_choose_field, entry_func, NULL);
        gui_buffer = GetCurrGuiBuffer(SCR_CHOOSE_NUMBER);
        RegisterHighlightHandler(mmi_phb_get_index_third_level);
        if (mmi_ucs2strlen((S8*) PhoneBook[store_index].alpha_id.name))
        {
            title_string = PhoneBook[store_index].alpha_id.name;
        }

	ShowCategory53Screen(STR_CHOOSE_NUMBER_CAPTION,
								   IMG_SCR_PBOOK_CAPTION,
					 	 		   STR_GLOBAL_OK,
					 	 		   IMG_GLOBAL_OK,
					 	 		   STR_GLOBAL_BACK,
					 	 		   IMG_GLOBAL_BACK,
					 	 		   i,
					 	 		   g_phb_cntx.number_to_dial,
					 	 		   number_type_image_list,
					 	 		   NULL,
					 	 		   0,
					 	 		   0,
					 	 		   gui_buffer);
    
        SetLeftSoftkeyFunction(lsk_func, KEY_EVENT_UP);
        SetKeyHandler(send_key_func, KEY_SEND, KEY_EVENT_DOWN);
        SetRightSoftkeyFunction(rsk_func, KEY_EVENT_UP);
    }
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_exit_list_choose_number()
* DESCRIPTION
*   exit function
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_exit_list_choose_number(void)
{
	g_phb_cntx.end_scr_id = SCR_CHOOSE_NUMBER;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_phb_exit_choose_field
 * DESCRIPTION
 *  exit function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_phb_exit_choose_field(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_phb_cntx.end_scr_id = SCR_CHOOSE_NUMBER;
}
/*****************************************************************************
* FUNCTION
*  mmi_phb_list_make_call()
* DESCRIPTION
*   Makes call to a number
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_list_make_call (void)
{
	PRINT_INFORMATION_2((MMI_TRACE_G4_PHB,"File: [%s]  Line: [%d] <<mmi_phb_list_make_call.>\n",__FILE__,__LINE__));
	g_phb_cntx.dial_from_list  = MMI_PHB_PHONEBOOK;
	MakeCall((PS8)g_phb_cntx.number_to_dial[g_phb_cntx.active_index_third]);
}

#define MMI_PHB_UTIL_FUNC
#if defined( __MMI_INTELLIGENT_CALL_ALERT__ )
/*****************************************************************************
* FUNCTION
*  mmi_phb_get_index_speak_name()
* DESCRIPTION
*   Gets the index of the currently highlighted item in Phonebook menu list.
*
* PARAMETERS
*  nIndex	IN	highlight index position
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_get_index_speak_name(S32 nIndex)
{
	g_phb_cntx.active_index = (U16)nIndex;

	StopTimer(PHB_LIST_SPEAK_NAME_TIMER);
	if( g_phb_is_speaking_name )
		mdi_audio_stop_string();

	if( g_phb_cntx.speak_name )
		StartTimer( PHB_LIST_SPEAK_NAME_TIMER, UI_POPUP_NOTIFYDURATION_TIME, mmi_phb_list_speak_name );
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_list_speak_name()
* DESCRIPTION
*   Speak name or number for a phonebook entry.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_list_speak_name(void)
{
	U16 store_index;
	U8 text[(MAX_PB_NAME_LENGTH + MAX_PB_NUMBER_LENGTH + 10) * ENCODING_LENGTH];
	U16 length = 0;

	//g_phb_cntx.active_index = (U16)nIndex;

#ifdef __MMI_SILENT_MEETING_PROFILE__
	if( IsMeetingModeActivated() )
	{
		// not to play sound
	}
	else
#endif /* __MMI_SILENT_MEETING_PROFILE__ */
	/*Flag is on and not in call*/
	if( g_phb_cntx.speak_name && !isInCall() && !IsSilentModeActivated() )
	{
		store_index = g_phb_name_index[g_phb_cntx.active_index];

#if defined( SIMPLE_TTS ) || defined( DIGIT_TONE_SUPPORT )
		/*Name*/
		pfnUnicodeStrcpy((S8*)text, (S8*)PhoneBook[store_index].alpha_id.name);
		length = pfnUnicodeStrlen((S8*)text);
#endif

		/*Append Number*/
#ifdef DIGIT_TONE_SUPPORT
		mmi_phb_convert_get_ucs2_number((S8*)(text + length*ENCODING_LENGTH), store_index);
		length += pfnUnicodeStrlen((S8*)(text + length*ENCODING_LENGTH));
#endif
		if( length > 0 )
		{
			mdi_result result = mdi_audio_play_text_with_vol_path( (U8*)text, length, 100, NULL, mmi_phb_speak_name_handler,
				GetRingVolumeLevel(), MDI_DEVICE_SPEAKER2 );
			if( result == MDI_AUDIO_SUCCESS )
				g_phb_is_speaking_name = 1;
			else
				g_phb_is_speaking_name = 0;
		}
	}
}
#endif

/*****************************************************************************
* FUNCTION
*  mmi_phb_get_index()
* DESCRIPTION
*   Gets the index of the currently highlighted item in Phonebook menu list.
*
* PARAMETERS
*  nIndex	IN	highlight index position
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_get_index(S32 nIndex)
{
	#ifdef __MMI_VIP_FUNCTION__
	g_phb_cntx.active_index = (U16)g_vip_context.PHBIndexMap[nIndex];
	#else
	g_phb_cntx.active_index = (U16)nIndex;
	#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_get_index_second_level()
* DESCRIPTION
*   Gets the index of the currently highlighted item in Phonebook menu list.
*
* PARAMETERS
*  nIndex	IN	highlight index position
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_get_index_second_level(S32 nIndex)
{
	g_phb_cntx.active_index_second = (U16)nIndex;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_get_index_third_level()
* DESCRIPTION
*   Gets the index of the currently highlighted item in Phonebook menu list.
*
* PARAMETERS
*  nIndex	IN	highlight index position
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_get_index_third_level(S32 nIndex)
{
	g_phb_cntx.active_index_third = (U16)nIndex;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_get_index_by_store_location()
* DESCRIPTION
*  call back function for dynamic list to get index according to the fileter list.
*
* PARAMETERS
*  nIndex	IN	highlight index position
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_get_index_by_store_location(S32 nIndex)
{
	g_phb_cntx.active_index = g_phb_cntx.list_filter[nIndex];
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_get_index_by_store_location_second()
* DESCRIPTION
*  call back function for dynamic list to get index according to the fileter list.
*
* PARAMETERS
*  nIndex	IN	highlight index position
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_get_index_by_store_location_second(S32 nIndex)
{
	g_phb_cntx.active_index_second = g_phb_cntx.list_filter[nIndex];
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_util_clear_buffer
* DESCRIPTION
*   Clears buffers used for adding/editing entry
*
* PARAMETERS
*  flag_number	IN	TRUE: clear number buffer. FALSE: Keep number buffer
* RETURNS
*  void
* GLOBALS AFFECTED
*   all phonebook temp buffer
*****************************************************************************/
void mmi_phb_util_clear_buffer(BOOL clear_number)
{
    mmi_trace(g_sw_PHB, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
    memset(pbName, 0, ENCODING_LENGTH);
    if(clear_number)
    	memset(pbNumber, 0, (MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH);
    memset(pbHomeNumber, 0, (MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH);
    memset(pbCompanyName, 0, (MAX_PB_COMPANY_LENGTH+1)*ENCODING_LENGTH);
    memset(pbEmailAddress, 0, (MAX_PB_EMAIL_LENGTH+1)*ENCODING_LENGTH);
    memset(pbOfficeNumber, 0, (MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH);
    memset(pbFaxNumber, 0, (MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH);
    g_phb_cntx.selected_pic_index = 0;
    g_phb_cntx.selected_ring_index = 0;
    g_phb_cntx.selected_grp_index = 0;

#if defined(__MMI_INCOMING_CALL_VIDEO__)
    g_phb_cntx.selected_video_index = 0;
#endif

#if defined(__MMI_PHB_BIRTHDAY_FIELD__)
    mmi_phb_util_clear_bday_buffer();
#endif

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_go_back_2_history()
* DESCRIPTION
*   Goes back two screens in history
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_go_back_2_history(void)
{
	GoBacknHistory(1);
}

/*****************************************************************************
* FUNCTION
* mmi_phb_go_back_3_history_and_clear()
* DESCRIPTION
*   Goes back three screens in history
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_go_back_3_history_and_clear(void)
{
	mmi_phb_util_clear_buffer(TRUE);
	GoBacknHistory(2);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_set_full_scr_lsk()
* DESCRIPTION
*  Sets the LSK for Full-Screen editor
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_set_full_scr_lsk(void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS);
	SetLeftSoftkeyFunction(EntryScrForInputMethodAndDone, KEY_EVENT_UP);
	SetInputMethodAndDoneCaptionIcon(IMG_SCR_PBOOK_CAPTION);
}

/*****************************************************************************
* FUNCTION
*   mmi_phb_clear_old_history()
* DESCRIPTION
*  Deletes old PHB screens from history if Phonebook is re-entered thru some other application
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_clear_old_history(void)
{
	if(g_phb_cntx.start_scr_id && g_phb_cntx.end_scr_id)
	{
		if(g_phb_cntx.start_scr_id == g_phb_cntx.end_scr_id)
		{
			DeleteScreenIfPresent(g_phb_cntx.start_scr_id);
		}
		else
		{
			U16 end_scr_id = 0;
			GetPreviousScrnIdOf (GetCmMarkerScrnID(), &end_scr_id);
			if (end_scr_id) /* since editor common screen won't be save to g_phb_cntx.end_scr_id */
			{
				g_phb_cntx.end_scr_id = end_scr_id;
			}
			DeleteBetweenScreen(g_phb_cntx.start_scr_id, g_phb_cntx.end_scr_id);
		}
	}
	else if(g_phb_cntx.end_scr_id)
		DeleteScreenIfPresent(g_phb_cntx.end_scr_id);

	g_phb_cntx.start_scr_id = g_phb_cntx.end_scr_id = 0;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_reset_scr_id()
* DESCRIPTION
*   Resets the start and end screen ids on main-menu and idle screen.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_reset_scr_id(void)
{
	g_phb_cntx.start_scr_id = g_phb_cntx.end_scr_id = 0;
}

#define MMI_PHB_IMAGE_LIST
#if defined(__MMI_FILE_MANAGER__) && defined(MMI_ON_HARDWARE_P)
/*****************************************************************************
* FUNCTION
*  mmi_phb_image_list1_draw_title_info()
* DESCRIPTION
*   Draw image list 1 title info. It will draw image, name and number here.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_image_list1_draw_title_info(S32 item_idx, S32 x1, S32 y1, S32 x2, S32 y2)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32				char_height;
	S32				box_width, box_height;
	S32				img_box_width = 38, img_box_height = 38;
	S32				img_width,img_height;
	S32				img_draw_width, img_draw_height;
	S32				img_draw_offset_x, img_draw_offset_y;
	S32				spacing;
	S32				offset_x;	/*For name and image offset x*/
	S32				offset_y; /*For name and image offset y*/
	S32				num_offset_y;
	S32				name_width;
	U16				store_index;
	S8*				image_path = NULL;
	PHB_OPTIONAL_IDS_STRUCT* opt_ids;
	S8					buffer_UCS2[(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH];
	S32 				m_item_idx = item_idx;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/*
	*lock and push clip
	*/
	coolsand_UI_lock_double_buffer();
	coolsand_UI_push_text_clip();
	coolsand_UI_push_clip();

	coolsand_UI_reset_clip();
	coolsand_UI_reset_text_clip();

   /*
   * Get hint text info
   */
   	#ifdef __MMI_VIP_FUNCTION__
	m_item_idx = (S32)g_vip_context.PHBIndexMap[item_idx];
	#endif
	store_index = g_phb_name_index[m_item_idx];
	mmi_phb_convert_get_ucs2_number((S8*)buffer_UCS2, store_index);

   /*
   * Calcuate text position
   */
	box_height		= y2-y1+1;
	box_width			= x2-x1+1;

	coolsand_UI_set_font(&MMI_medium_font);
	char_height			= coolsand_UI_get_string_height((UI_string_type)GetString(STR_ID_PHB_MOBILE_NUMBER));
	spacing				= (box_height-char_height*2)/3;

	offset_y 		= spacing+MMI_title_height+MMI_title_y;
	num_offset_y		= spacing*2+char_height+MMI_title_height+MMI_title_y;

	if(r2lMMIFlag) /*For right to left language*/
		offset_x	=	x2 - img_box_width - 6;
	else
		offset_x	=	x1 + img_box_width + 6;

	name_width = box_width - img_box_width - 12;

   /*
   * Draw Hint Text
   */
	coolsand_UI_set_font(&MMI_medium_font);

	coolsand_UI_reset_text_clip();
	coolsand_UI_set_text_color(coolsand_UI_color(0,0,0));

	/*
	* Draw name/number if not empty
	*/
	if (pfnUnicodeStrlen((S8*)PhoneBook[store_index].alpha_id.name))
	{
		coolsand_UI_print_truncated_text(offset_x,offset_y, name_width,(UI_string_type)PhoneBook[store_index].alpha_id.name);

		if(pfnUnicodeStrlen((S8*)buffer_UCS2))
			coolsand_UI_print_truncated_text(offset_x,num_offset_y, name_width,(UI_string_type)buffer_UCS2);
	}
	else
		coolsand_UI_print_truncated_text(offset_x,offset_y, name_width,(UI_string_type)buffer_UCS2);

   /*
   * Get Associate Picture path from file
   */
	if(store_index < MAX_PB_PHONE_ENTRIES)
	{
		mmi_phb_read_optional_ids(store_index); /*i is store index*/
		opt_ids = (PHB_OPTIONAL_IDS_STRUCT*) g_phb_cntx.optional_ids;
		if(opt_ids->pictureTagID == 1)
			image_path =  mmi_phb_image_get_path_from_id((U16)(store_index+1));
	}

   /*
   * Draw Associate Picture thumb image
   */
     coolsand_UI_push_clip();

   	if(r2lMMIFlag) /*For right to left language*/
   		offset_x = x2 - img_box_width- 2;
   	else
		offset_x = x1 + 2;

	offset_y = y1 + 2;

	coolsand_UI_set_clip(offset_x, offset_y, offset_x+img_box_width, offset_y+img_box_height);

	/*Get image from file*/
	if((image_path != NULL) &&
		(gdi_image_get_dimension_file(image_path, &img_width, &img_height) >= 0))
	{

		gdi_util_fit_box(GDI_UTIL_MODE_NO_RESIZE_OR_LONG_SIDE_FIT,
						img_box_width,
						img_box_height,
						img_width,
						img_height,
						&img_draw_offset_x,
						&img_draw_offset_y,
						&img_draw_width,
						&img_draw_height);

   		gdi_image_draw_resized_file(offset_x + img_draw_offset_x, offset_y + img_draw_offset_y, img_draw_width, img_draw_height, image_path);
	}
	else /*Use default image*/
		gdi_image_draw_resized_id(offset_x, offset_y, img_box_width, img_box_height, IMG_ID_PHB_LIST_ICON1);

	coolsand_UI_pop_clip();

	/*
	* pop clip , unlock and blt
	*/
	coolsand_UI_pop_clip();
	coolsand_UI_pop_text_clip();
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0, 0, UI_device_width-1, UI_device_height-1);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_entry_image_list1
* DESCRIPTION
*   This function list all phonebook entry with picture info on the top.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_entry_image_list1(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8* guiBuffer;
	U16 nVIPSuited = 0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   	#ifdef __MMI_VIP_FUNCTION__
	nVIPSuited = g_vip_context.nPHBSuited;
	#endif
	/*
	* Check if it can be list in advance
	*/
	if(!g_phb_cntx.phb_ready || g_phb_cntx.processing)
	{
		mmi_phb_entry_not_ready();
		return;
	}
	else if(PhoneBookEntryCount - nVIPSuited == 0)
	{
		DisplayPopup((PU8)GetString(STR_NO_ENTRIES_MESSAGE), IMG_GLOBAL_EMPTY, TRUE, PHB_NOTIFY_TIMEOUT, EMPTY_LIST_TONE);
		return;
	}

	/*
	*Check if phonebook entry has been updated, and if need to use alpha_index function.
	*/
	if(g_phb_cntx.refresh_list)
	{
		mmi_phb_list_build_alpha_index(NULL,0,TRUE,MMI_STORAGE_NONE);
		g_phb_cntx.refresh_list = MMI_PHB_ENTRY_NO_CHANGE;
	}


	EntryNewScreen(SCR_PBOOK_LIST, mmi_phb_exit_list, mmi_phb_entry_image_list1, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_PBOOK_LIST);

#if defined(__MMI_TOUCH_SCREEN__) && defined(__MMI_WGUI_CSK_ENABLE__)
	if (HasValidSIM()
  #ifdef __FLIGHT_MODE_SUPPORT__
		&& mmi_bootup_get_active_flight_mode() != 1
  #endif
  	)
	{
		EnableCenterSoftkey(0, IMG_CSK_DIAL_ICON);
	}
#endif /* __MMI_TOUCH_SCREEN__ && __MMI_WGUI_CSK_ENABLE__ */

#if defined(__PROJECT_GALLITE_C01__)
        RegisterHighlightHandler(mmi_phb_get_index);
#else
#if defined( __MMI_INTELLIGENT_CALL_ALERT__ )
	RegisterHighlightHandler(mmi_phb_get_index_speak_name);
#else
	RegisterHighlightHandler(mmi_phb_get_index);
#endif
#endif

	ShowCategory212Screen(
		STR_SCR_PBOOK_VIEW_CAPTION, IMG_SCR_PBOOK_CAPTION, /* Title */
		STR_GLOBAL_OPTIONS, IMG_GLOBAL_OPTIONS,	/* LSK */
		STR_GLOBAL_BACK, IMG_GLOBAL_BACK,			/* RSK */
		PhoneBookEntryCount - nVIPSuited,
		mmi_phb_list_get_item,
		mmi_phb_list_get_null_hint,
		mmi_phb_image_list1_draw_title_info,
		NULL,	/*No call back needed when change highlight*/
		2,		/* Info box row count */
		/*wuzc Add Start For GDI/GUI Portting Ver:    on 2008-3-10 14:19 */
		0,
		/*wuzc Add End  For GDI/GUI Portting Ver:    on 2008-3-10 14:19 */
		g_phb_cntx.highlight_entry,				/* Highlight item index */
		(U8*)guiBuffer);


	register_multitap_no_draw_key_handlers();
	set_multitap_functions(mmi_phb_list_alpha_index, mmi_phb_list_alpha_index_input_callback);
	change_multitap_mode(0);

	SetLeftSoftkeyFunction(mmi_phb_entry_op_option,KEY_EVENT_UP);
	SetKeyHandler(mmi_phb_entry_op_option,KEY_RIGHT_ARROW,KEY_EVENT_DOWN);

	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

	if (HasValidSIM()
#ifdef __FLIGHT_MODE_SUPPORT__
		&& mmi_bootup_get_active_flight_mode() != 1
#endif
  	)
	{
#ifdef __MMI_MULTI_SIM__
	    MTPNP_PFAL_CC_HandleSendKeys(mmi_phb_choose_number_normal,KEY_EVENT_DOWN);
	ClearKeyHandler( KEY_ENTER,  KEY_EVENT_DOWN);
	SetKeyHandler(mmi_phb_entry_op_option,KEY_ENTER,KEY_EVENT_UP );

#else
	    SetKeyHandler(mmi_phb_choose_number_normal, KEY_SEND, KEY_EVENT_DOWN);
#endif

#if defined(__MMI_TOUCH_SCREEN__) && defined(__MMI_WGUI_CSK_ENABLE__)
#ifdef __MMI_MULTI_SIM__
	    MTPNP_PFAL_CC_SetCenterFunc(mmi_phb_choose_number_normal);
	    SetCenterSoftkeyFunction(MTPNP_PFAL_CC_CSKHandler, KEY_EVENT_UP);
#else /* __MMI_MULTI_SIM__ */
	    SetCenterSoftkeyFunction(mmi_phb_choose_number_normal, KEY_EVENT_UP);
#endif /* __MMI_MULTI_SIM__ */
#endif

	}
}
#endif

#define MMI_PHB_UTIL_OBSELETE
U8 inPinyinSearchScreen=FALSE;	/*For PinYin Search, Obselete Now*/
/*****************************************************************************
* FUNCTION
*  PbookListSearchEntryReq()
* DESCRIPTION
*   Sends the request to stack to search for an entry based on the name entered by the user
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   void
*****************************************************************************/
void PbookListSearchEntryReq(void)
{
}

/*2004-07-30, Remove by Wilson.*/
//CSD added by JL, 030922 to support IRAM optimization
//#ifdef CSD_TARGET
//#pragma arm section rwdata , rodata , zidata
//#endif /* CSD_TARGET */
//CSD end

U8 PHBGetStorageByStoreIndex(U16 nStoreIndex)
{
	MMI_PHB_STORAGE_LOCATION storage = MMI_STORAGE_NONE;
	
#ifdef __MMI_MULTI_SIM__
	if (nStoreIndex >= MAX_PB_PHONE_ENTRIES)
	{
		storage = MMI_SIM + (nStoreIndex - MAX_PB_PHONE_ENTRIES) / MAX_PB_SIM_ENTRIES;
	}
	else
	{
		storage = MMI_NVRAM;
	}	
#else
	storage = (nStoreIndex >= MAX_PB_PHONE_ENTRIES) ? MMI_SIM : MMI_NVRAM;
#endif

	return storage;
}

U16 PHBGetADPIndexByStorage(U8 storage, U16 storeindex)
{
	U16 index = 0;
	
#ifdef __MMI_MULTI_SIM__
	if(storage >= MMI_SIM && storage <= MMI_SIM4)
	{
		index = storeindex + 1 - MAX_PB_PHONE_ENTRIES - MAX_PB_SIM_ENTRIES*(storage - MMI_SIM);		
	}
	else
	{
		index = storeindex + 1;
	}
#else
	index = (storage == MMI_SIM) ?  (storeindex + 1) - MAX_PB_PHONE_ENTRIES : (storeindex + 1);
#endif

	return index;
}

#endif /* _PHONEBOOKMAIN_C  */

