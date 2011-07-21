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

/*******************************************************************************
 * Filename:
 * ---------
 *	EngineerModeTracer.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This is the tracer for PS log
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#include "mmi_features.h" 
#if defined(MMI_ON_HARDWARE_P) && defined(__MMI_ENGINEER_MODE__)


#include "globaldefs.h"
#include "custdatares.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "protocolevents.h"
#include "custmenures.h"
#include "wgui_categories.h"
#include "hardwareevents.h"
#include "thrdosinterface.h"
#include "osthreadinterfacegprot.h"
#include "allappgprot.h"
//jl #include "profiles_prot.h"
#include "pixcomfontengine.h" //jl

#include "timerevents.h"
#include "mainmenudef.h"
#include "mmi_data_types.h"
#include "customcfg.h"
#include "unicodexdcl.h"
#include "audioinc.h"
#include "idleappdef.h"
#include "factorymodedef.h"
#include "factorymodeprot.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "alarmdef.h"
#include "gpioinc.h"
#include "debuginitdef.h"
#include "datetimegprot.h" 
#include "callmanagementiddef.h"
#include "globaldefs.h"
#include "callmanagementgprot.h"
#include "simdetectiondef.h"
#include "engineermodetracer.h"
#include "idleappdef.h"
#include "historygprot.h"
/*********************************
 ***** TYPE Define *****
 **********************************/
typedef struct {
	kal_uint8	crh;   //CELL-RESELECT-HYSTERESIS
	kal_uint8	ms_txpwr;   // 	maximum TX power level an MS may use when accessing on a CCH
	kal_uint8	rxlev_access_min;   //	minimum received signal level at the MS for which it is permitted to access the system
	kal_uint8	is_present;
}mmi_rr_em_cell_select_para_info_t;

typedef struct {
	kal_uint8	channel_type;   //	channel type
	kal_uint8	hopping_flag;   //	hopping or not
	kal_uint8	hsn;   //	HSN value
	kal_uint8	tn;   //	timeslot number
	kal_uint8	maio;   //	MAIO value
	kal_uint8	num_of_carriers;   //	number of carriers in the BA list(in the non-hopping case, this shall be set to 1)
	kal_uint8	tsc;   //	training sequence code
	kal_uint8	is_present;
}mmi_rr_em_channel_descr_info_t;

typedef struct {
	kal_uint8	mscr;   //	MSC release
	kal_uint8	att;   //	att flag
	kal_uint8	bs_ag_blks_res;   //	BS-AG-BLKS-RES
	kal_uint8	ccch_conf;   //	CCCH-CONF
	kal_uint8	cbq2;   //	Cell Bar Qualify 2
	kal_uint8	bs_pa_mfrms;   //	BS-PA-MFRMS
	kal_uint8	t3212;   //	T3212 timeout value
	kal_uint8	is_present;
}mmi_rr_em_ctrl_channel_descr_info_t;

typedef struct {
	kal_uint8	max_retrans;    
	kal_uint8	tx_integer;   //	TX integer
	kal_uint8	cba;   //	cell bar for access
	kal_uint8	re;   //	call reestablishment allowed
	kal_uint8	acc_class[2];   //	acces control class
	kal_uint8	is_present;
}mmi_rr_em_rach_ctrl_para_info_t;

typedef struct {
	kal_uint8	mcc[3];	//MCC
	kal_uint8	mnc[3];	//MNC
	kal_uint8	lac[2];	//LAC
	kal_uint16	cell_id;	 //cell ID
	kal_uint8	is_present;
}mmi_rr_em_lai_info_t;

typedef struct {
	kal_uint16	max_value;   //	timeout value for the DSF or RLF
	kal_int16	current_value;   //	current value in the DSF or RLF calculation
	kal_uint8	dtx_ind;	//DTX indicator
	kal_uint8	dtx_used;	//DTX used or not
	kal_uint8	is_present;
}mmi_rr_em_radio_link_counter_info_t;

typedef struct {
	kal_uint16	nc_arfcn[12]; //Top 12
	kal_int16	rla_in_quarter_dbm[12];
	kal_int16	c1[12];
	kal_int16	c2[12];
	kal_uint8	nc_info_status[12];
	kal_uint16 	serving_arfcn;
	kal_int16 	serv_rla_in_quarter_dbm;
	kal_int16 	c1_serv_cell;
	kal_int16 	c2_serv_cell;
	kal_uint8 	serving_bsic;
	kal_uint8 	serv_gprs_supported;
	kal_bool     gprs_pbcch_present;
	kal_uint8	num_of_carriers;
	kal_uint8	rr_state;
	kal_uint8	multiband_report;
	kal_uint8	is_present;
} mmi_rr_em_measurement_report_info_t ;


/*********************************
 ***** Define *****
 **********************************/
//#if defined(__GPRS_MODE__)
#if 0
/* under construction !*/
#else
#define MAX_TRACER_SCREEN_NUM	10
#endif
#define MAX_TRACER_LINE_NUM 14
#define EM_LOG_SCREEN_ID 907

#define TEXT_FORMAT_1	"%s:%2d%2d%2d"
#define TEXT_FORMAT_2	"%s:%d"
#define TEXT_FORMAT_3	"%4d %4d %4d %4d %4d %4d"
#define TEXT_FORMAT_4	"%s:%2d %2d"
#define TEXT_FORMAT_5	"%s:%2d%2d%2s"
#define TEXT_FORMAT_6	"%4s %4s %4s %4s %4s %4s"
#define TEXT_FORMAT_7	"%4s %4s %3s %3s %1s"
#define TEXT_FORMAT_8	"%4d %4d %3d %3d %1d"
#define TEXT_FORMAT_9	"%s:%d/%d"
#define TEXT_FORMAT_10 	"%s:%2s"
#define TEXT_FORMAT_11 	"%4d %4d %3s %3s %1d"
#define TEXT_FORMAT_12 	"%s:%s"
#define TEXT_FORMAT_13 	"%4s %4s %3s %3s %1s"
#define NA_SYMBOL	"X"

#define MAX_CHANNEL_TYPE_COUNT (sizeof(rr_channel_type_name)/sizeof(rr_channel_type_name[0]) )
/* below string table is to follow the em_struct.h to display */
const char* const rr_channel_type_name[]={
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
   "SDCCH4",
   "SDCCH4",
   "SDCCH4",
   "SDCCH4",
   "SDCCH8",
   "SDCCH8",
   "SDCCH8",
   "SDCCH8",
   "SDCCH8",
   "SDCCH8",
   "SDCCH8",
   "SDCCH8",
   "SDCCH",
   "SACCH",
   "FACCH",
   "TCH_EF",
   "TCH_F",
   "TCH_H",
   "TCH_H",
   "TCH_H",
   "CBCH",
   "PPCH"
   };

/*********************************
 ***** Local Veriable *****
 **********************************/
static kal_uint8 index_screen=0;
//static kal_uint8 mmi_screen_text[MAX_TRACER_SCREEN_NUM][MAX_TRACER_LINE_NUM][40];
static kal_uint8 mmi_screen_text[MAX_TRACER_LINE_NUM][40];
static kal_uint8 num_of_carriers=0;
static request_info_type receive_last_info=0x00;
static mmi_rr_em_cell_select_para_info_t mmi_rr_em_cell_select_para_info;
static mmi_rr_em_channel_descr_info_t mmi_rr_em_channel_descr_info;
static mmi_rr_em_ctrl_channel_descr_info_t mmi_rr_em_ctrl_channel_descr_info;
static mmi_rr_em_rach_ctrl_para_info_t mmi_rr_em_rach_ctrl_para_info;
static mmi_rr_em_lai_info_t mmi_rr_em_lai_info;
static mmi_rr_em_radio_link_counter_info_t mmi_rr_em_radio_link_counter_info;
static mmi_rr_em_measurement_report_info_t mmi_rr_em_measurement_report_info;


/*********************************
 ***** Globol Veriable *****
 **********************************/
extern kal_uint32 MAIN_LCD_device_width;
extern kal_uint32 MAIN_LCD_device_height;
kal_uint8 gIsOnEMTracer = 0;

/*********************************
 *****  Global Function *****
 **********************************/
extern void UI_set_clip(s32 x1,s32 y1,s32 x2,s32 y2);
extern void UI_fill_rectangle(s32 x1,s32 y1,s32 x2,s32 y2,color_t c);
extern void UI_set_font(UI_font_type f);
extern void UI_set_text_color(color_t c);

extern U8 EntryNewScreen_Ext(U16 newscrnID, FuncPtr newExitHandler, FuncPtr newEntryHandler, void  *peerBuf);
	
void mmi_em_send_start_req(module_type mod_id, request_info_type info_id);
void mmi_em_send_stop_req(module_type mod_id, request_info_type info_id);
void mmi_em_parsing_data(void *info);
void mmi_em_display_screen_n(kal_uint8 screen_num);
void mmi_em_display_line_n(kal_uint8 line_num, kal_uint8 *str);
void mmi_em_tracer_key_up(void);
void mmi_em_tracer_key_down(void);
void mmi_em_tracer_screen_color_init(kal_uint8 color_r, kal_uint8 color_g, kal_uint8 color_b);
void mmi_em_display_screen_n(kal_uint8 screen_num);
void mmi_em_show_screen(void);

 
#include "engineermodepatch.h"
	
/* 
*   Function 
*      mmi_em_tracer_key_up
*   DESCRIPTION
*      Press key up action
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
void mmi_em_tracer_key_up(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if (index_screen==0)
		index_screen = MAX_TRACER_SCREEN_NUM-1;
	else
		index_screen--;
	
	mmi_em_display_screen_n(index_screen);

}

/* 
*   Function 
*      mmi_em_tracer_key_down
*   DESCRIPTION
*      Press key down action
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
void mmi_em_tracer_key_down(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	index_screen++;
	if (index_screen>=MAX_TRACER_SCREEN_NUM)
		index_screen = 0;
	
	mmi_em_display_screen_n(index_screen);
	
}

/* 
*   Function 
*      mmi_em_tracer_key
*   DESCRIPTION
*      Press key down action
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
void mmi_em_tracer_key(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	ClearKeyHandler(KEY_UP_ARROW, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_DOWN_ARROW, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_UP_ARROW, KEY_EVENT_UP);
	ClearKeyHandler(KEY_DOWN_ARROW, KEY_EVENT_UP);
   SetKeyHandler(mmi_em_tracer_key_up, KEY_UP_ARROW,KEY_EVENT_DOWN);
   SetKeyHandler(mmi_em_tracer_key_down, KEY_DOWN_ARROW,KEY_EVENT_DOWN);
}

/* 
*   Function 
*      mmi_em_exit_screen
*   DESCRIPTION
*      set exit screen flag
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
void mmi_em_exit_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	gIsOnEMTracer = 0;
/*	
	h.scrnID = EM_LOG_SCREEN_ID;
	h.entryFuncPtr = mmi_em_redraw_screen;
	pfnUnicodeStrcpy((S8*)h.inputBuffer, ( S8*)&nHistory);
	GetCategoryHistory(h.guiBuffer);
	AddHistory(h);
*/
}

/* 
*   Function 
*      mmi_em_tracer_start_req
*   DESCRIPTION
*      set turn on the trace
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
void mmi_em_tracer_start_req(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
  mmi_em_send_start_req(MOD_AS, 
  									RR_EM_CELL_SELECT_PARA_INFO|RR_EM_CHANNEL_DESCR_INFO|
  									RR_EM_CTRL_CHANNEL_DESCR_INFO|RR_EM_RACH_CTRL_PARA_INFO|
  									RR_EM_LAI_INFO|RR_EM_RADIO_LINK_COUNTER_INFO|
  									RR_EM_MEASUREMENT_REPORT_INFO|RR_EM_CONTROL_MSG_INFO|
  									RR_EM_TBF_INFO|RR_EM_GPRS_GENERAL_INFO
  									);
}

/* 
*   Function 
*      mmi_em_tracer_end_req
*   DESCRIPTION
*      set turn off the trace
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
void mmi_em_tracer_end_req(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
  mmi_em_send_stop_req(MOD_AS, 
  									RR_EM_CELL_SELECT_PARA_INFO|RR_EM_CHANNEL_DESCR_INFO|
  									RR_EM_CTRL_CHANNEL_DESCR_INFO|RR_EM_RACH_CTRL_PARA_INFO|
  									RR_EM_LAI_INFO|RR_EM_RADIO_LINK_COUNTER_INFO|
  									RR_EM_MEASUREMENT_REPORT_INFO|RR_EM_CONTROL_MSG_INFO|
  									RR_EM_TBF_INFO|RR_EM_GPRS_GENERAL_INFO
  									);

}
	
/* 
*   Function 
*      mmi_em_send_start_req
*   DESCRIPTION
*      set out the request message
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
void mmi_em_send_start_req(module_type mod_id, request_info_type info_id)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	MYQUEUE Message;
   mmi_em_start_req_struct *em_start_req;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	PRINT_INFORMATION_2((MMI_TRACE_INFO, "mmi_em_send_start_req %d", info_id));	
	mmi_em_show_screen();
        receive_last_info = info_id;
   
	Message.oslMsgId = MSG_ID_MMI_EM_START_REQ;
	em_start_req = OslConstructDataPtr(sizeof(mmi_em_start_req_struct));
	em_start_req->mod_id = mod_id;
	em_start_req->info_request = info_id;

   Message.oslDataPtr = (oslParaType *)em_start_req;
   Message.oslPeerBuffPtr = NULL;
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_L4C;
	OslMsgSendExtQueue(&Message);

}

/* 
*   Function 
*      mmi_em_send_stop_req
*   DESCRIPTION
*      set out the request off message
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
void mmi_em_send_stop_req(module_type mod_id, request_info_type info_id)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	MYQUEUE Message;
   mmi_em_stop_req_struct *em_stop_req;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	PRINT_INFORMATION_2((MMI_TRACE_INFO, "mmi_em_send_stop_req"));	

   GoBackHistory();
	
	Message.oslMsgId = MSG_ID_MMI_EM_STOP_REQ;
	em_stop_req = OslConstructDataPtr(sizeof(mmi_em_stop_req_struct));
	em_stop_req->mod_id = mod_id;
	em_stop_req->info_request = info_id;

   Message.oslDataPtr = (oslParaType *)em_stop_req;
   Message.oslPeerBuffPtr = NULL;
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_L4C;
	OslMsgSendExtQueue(&Message);

}

/* 
*   Function 
*      mmi_em_rr_cell_select_logs_value
*   DESCRIPTION
*      keep RR
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
void mmi_em_rr_cell_select_logs_value(rr_em_cell_select_para_info_struct *rr_em_cell_select_para_info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
 mmi_rr_em_cell_select_para_info.is_present = 1;
 mmi_rr_em_cell_select_para_info.crh = rr_em_cell_select_para_info->crh;
 mmi_rr_em_cell_select_para_info.ms_txpwr = rr_em_cell_select_para_info->ms_txpwr;
 mmi_rr_em_cell_select_para_info.rxlev_access_min = rr_em_cell_select_para_info->rxlev_access_min;
}

/* 
*   Function 
*      mmi_em_rr_cell_select_logs
*   DESCRIPTION
*      display RR
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
kal_uint8 mmi_em_rr_cell_select_logs(kal_uint8 screen_num, mmi_rr_em_cell_select_para_info_t *rr_em_cell_select_para_info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	kal_uint8 i=0;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	sprintf((char *)mmi_screen_text[i++],TEXT_FORMAT_9,"Cell Selection",  screen_num+1, MAX_TRACER_SCREEN_NUM);
	sprintf((char *)mmi_screen_text[i++], "-------------------------");
	if ( (receive_last_info&RR_EM_CELL_SELECT_PARA_INFO) && (rr_em_cell_select_para_info->is_present!=0) )
	{
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<CRH>",rr_em_cell_select_para_info->crh);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<MS_TXPWR>", rr_em_cell_select_para_info->ms_txpwr);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2, "<RX_ACC_MIN>", rr_em_cell_select_para_info->rxlev_access_min);
		//JL remove sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<NUM_CARRIERS>", num_of_carriers);
	}
	else
	{
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<CRH>",NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<MS_TXPWR>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10, "<RX_ACC_MIN>", NA_SYMBOL);
		//JL remove sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<NUM_CARRIERS>", NA_SYMBOL);
	}
	mmi_screen_text[i++][0]='\0';
   MMI_ASSERT(i<=MAX_TRACER_LINE_NUM);
   return 1;
}


/* 
*   Function 
*      mmi_em_rr_channel_descr_logs_value
*   DESCRIPTION
*      keep RR channel descr
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
void mmi_em_rr_channel_descr_logs_value(rr_em_channel_descr_info_struct *rr_em_channel_descr_info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	mmi_rr_em_channel_descr_info.is_present = 1;
 	mmi_rr_em_channel_descr_info.channel_type = rr_em_channel_descr_info->channel_type;
 	mmi_rr_em_channel_descr_info.hopping_flag = rr_em_channel_descr_info->hopping_flag;
 	mmi_rr_em_channel_descr_info.hsn = rr_em_channel_descr_info->hsn;
 	mmi_rr_em_channel_descr_info.tn = rr_em_channel_descr_info->tn;
 	mmi_rr_em_channel_descr_info.maio = rr_em_channel_descr_info->maio;
 	mmi_rr_em_channel_descr_info.num_of_carriers = rr_em_channel_descr_info->num_of_carriers;
 	mmi_rr_em_channel_descr_info.tsc = rr_em_channel_descr_info->tsc;
}

/* 
*   Function 
*      mmi_em_rr_channel_descr_logs
*   DESCRIPTION
*      display RR channel descr
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
kal_uint8 mmi_em_rr_channel_descr_logs(kal_uint8 screen_num, mmi_rr_em_channel_descr_info_t *rr_em_channel_descr_info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	kal_uint8 i=0;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_9, "Channel Descr", screen_num+1, MAX_TRACER_SCREEN_NUM);
	sprintf((char *)mmi_screen_text[i++], "-------------------------");
	if ( (receive_last_info&RR_EM_CHANNEL_DESCR_INFO) && (rr_em_channel_descr_info->is_present!=0) )
	{
		if (rr_em_channel_descr_info->channel_type<MAX_CHANNEL_TYPE_COUNT)
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_12,"<CH_TYPE>", rr_channel_type_name[rr_em_channel_descr_info->channel_type]);
		else
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<CH_TYPE>", rr_em_channel_descr_info->channel_type);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<HOPPING>", rr_em_channel_descr_info->hopping_flag);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<HSN>", rr_em_channel_descr_info->hsn);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<TN>", rr_em_channel_descr_info->tn);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<MAIO>", rr_em_channel_descr_info->maio);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<MA_NUMBER>", rr_em_channel_descr_info->num_of_carriers);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<TSC>", rr_em_channel_descr_info->tsc);
	}
	else
	{
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<CHANNEL_TYPE>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<HOPPING>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<HSN>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<TN>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<MAIO>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<MA_NUMBER>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<TSC>", NA_SYMBOL);
	}
	mmi_screen_text[i++][0]='\0';
   MMI_ASSERT(i<=MAX_TRACER_LINE_NUM);
   return 1;
}

/* 
*   Function 
*      mmi_em_rr_ctrl_channel_descr_logs_value
*   DESCRIPTION
*      keep RR control channel
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
void mmi_em_rr_ctrl_channel_descr_logs_value(rr_em_ctrl_channel_descr_info_struct *rr_em_ctrl_channel_descr_info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	mmi_rr_em_ctrl_channel_descr_info.is_present = 1;
	mmi_rr_em_ctrl_channel_descr_info.mscr = rr_em_ctrl_channel_descr_info->mscr;
	mmi_rr_em_ctrl_channel_descr_info.att = rr_em_ctrl_channel_descr_info->att;
	mmi_rr_em_ctrl_channel_descr_info.ccch_conf = rr_em_ctrl_channel_descr_info->ccch_conf;
	mmi_rr_em_ctrl_channel_descr_info.bs_pa_mfrms = rr_em_ctrl_channel_descr_info->bs_pa_mfrms;
	mmi_rr_em_ctrl_channel_descr_info.bs_ag_blks_res = rr_em_ctrl_channel_descr_info->bs_ag_blks_res;
	mmi_rr_em_ctrl_channel_descr_info.cbq2 = rr_em_ctrl_channel_descr_info->cbq2;
	mmi_rr_em_ctrl_channel_descr_info.t3212 = rr_em_ctrl_channel_descr_info->t3212;
}

/* 
*   Function 
*      mmi_em_rr_ctrl_channel_descr_logs
*   DESCRIPTION
*      display RR control channel descr
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
kal_uint8 mmi_em_rr_ctrl_channel_descr_logs(kal_uint8 screen_num, mmi_rr_em_ctrl_channel_descr_info_t *rr_em_ctrl_channel_descr_info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	kal_uint8 i=0;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_ASSERT(screen_num<MAX_TRACER_SCREEN_NUM);
	sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_9, "Ctrl Channel", screen_num+1, MAX_TRACER_SCREEN_NUM);
	sprintf((char *)mmi_screen_text[i++], "-------------------------");
	if ( (receive_last_info&RR_EM_CTRL_CHANNEL_DESCR_INFO) && (rr_em_ctrl_channel_descr_info->is_present!=0) )
	{
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<MSCR>", rr_em_ctrl_channel_descr_info->mscr);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<ATT>", rr_em_ctrl_channel_descr_info->att);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<CCCH_CONF>", rr_em_ctrl_channel_descr_info->ccch_conf);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<MFRMS>", rr_em_ctrl_channel_descr_info->bs_pa_mfrms);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<BS_AG_BLKS>", rr_em_ctrl_channel_descr_info->bs_ag_blks_res);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<CBQ2>", rr_em_ctrl_channel_descr_info->cbq2);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<T3212>", rr_em_ctrl_channel_descr_info->t3212);
	}
	else
	{
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<MSCR>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<ATT>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<CCCH_CONF>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<MFRMS>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<BS_AG_BLKS>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<CBQ2>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<T3212>", NA_SYMBOL);
	}
	mmi_screen_text[i++][0]='\0';
   MMI_ASSERT(i<=MAX_TRACER_LINE_NUM);
   return 1;
}

/* 
*   Function 
*      mmi_em_rr_rach_ctrl_para_info_logs_value
*   DESCRIPTION
*      keep RR rach control pararmeter descr
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
void mmi_em_rr_rach_ctrl_para_info_logs_value(rr_em_rach_ctrl_para_info_struct *rr_em_rach_ctrl_para_info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	mmi_rr_em_rach_ctrl_para_info.is_present = 1;
	mmi_rr_em_rach_ctrl_para_info.max_retrans =	rr_em_rach_ctrl_para_info->max_retrans;
	mmi_rr_em_rach_ctrl_para_info.cba =	 rr_em_rach_ctrl_para_info->cba;
	mmi_rr_em_rach_ctrl_para_info.acc_class[0] = rr_em_rach_ctrl_para_info->acc_class[0];
	mmi_rr_em_rach_ctrl_para_info.acc_class[1] = rr_em_rach_ctrl_para_info->acc_class[1];
	mmi_rr_em_rach_ctrl_para_info.tx_integer = rr_em_rach_ctrl_para_info->tx_integer;
	mmi_rr_em_rach_ctrl_para_info.re = rr_em_rach_ctrl_para_info->re;
}

/* 
*   Function 
*      mmi_em_rr_rach_ctrl_para_info_logs
*   DESCRIPTION
*      display RR rach control pararmeter descr
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
kal_uint8 mmi_em_rr_rach_ctrl_para_info_logs(kal_uint8 screen_num, mmi_rr_em_rach_ctrl_para_info_t *rr_em_rach_ctrl_para_info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	kal_uint8 i=0;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_9, "RACH Ctrl", screen_num+1, MAX_TRACER_SCREEN_NUM);
	sprintf((char *)mmi_screen_text[i++], "-------------------------");
	if ( (receive_last_info&RR_EM_RACH_CTRL_PARA_INFO) && (rr_em_rach_ctrl_para_info->is_present!=0) )
	{
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<MAX RETRANS>", rr_em_rach_ctrl_para_info->max_retrans);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<CBA>", rr_em_rach_ctrl_para_info->cba);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<SPEC_CLASS>", rr_em_rach_ctrl_para_info->acc_class[0]);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<NORM_CLASS>", rr_em_rach_ctrl_para_info->acc_class[1]);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<TX_INTEGER>", rr_em_rach_ctrl_para_info->tx_integer);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2,"<RE_EST>", rr_em_rach_ctrl_para_info->re);
	}
	else
	{
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<MAX RETRANS>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<CBA>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<SPEC_CLASS>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<NORM_CLASS>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<TX_INTEGER>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10,"<RE_EST>", NA_SYMBOL);
	}
	mmi_screen_text[i++][0]='\0';
   MMI_ASSERT(i<=MAX_TRACER_LINE_NUM);
   return 1;
}

/* 
*   Function 
*      mmi_em_rr_lai_info_logs_value
*   DESCRIPTION
*      keep RR LAI information log 
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
void mmi_em_rr_lai_info_logs_value(rr_em_lai_info_struct *rr_em_lai_info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	mmi_rr_em_lai_info.is_present = 1;
   mmi_rr_em_lai_info.mcc[0] = rr_em_lai_info->mcc[0];
 	mmi_rr_em_lai_info.mcc[1] = rr_em_lai_info->mcc[1];
 	mmi_rr_em_lai_info.mcc[2] = rr_em_lai_info->mcc[2];
 	mmi_rr_em_lai_info.mnc[0] = rr_em_lai_info->mnc[0];
 	mmi_rr_em_lai_info.mnc[1] = rr_em_lai_info->mnc[1];
 	mmi_rr_em_lai_info.mnc[2] = rr_em_lai_info->mnc[2];
 	mmi_rr_em_lai_info.lac[0] = rr_em_lai_info->lac[0];
 	mmi_rr_em_lai_info.lac[1] = rr_em_lai_info->lac[1];
 	mmi_rr_em_lai_info.cell_id = rr_em_lai_info->cell_id;
 	
}

/* 
*   Function 
*      mmi_em_rr_lai_info_logs
*   DESCRIPTION
*      display RR LAI information log 
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
kal_uint8 mmi_em_rr_lai_info_logs(kal_uint8 screen_num, mmi_rr_em_lai_info_t *rr_em_lai_info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	kal_uint8 i=0;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_9, "LAI", screen_num+1, MAX_TRACER_SCREEN_NUM);
	sprintf((char *)mmi_screen_text[i++], "-------------------------");
	if ( (receive_last_info&RR_EM_LAI_INFO) && (rr_em_lai_info->is_present!=0) )
	{
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_1, "<MCC>", rr_em_lai_info->mcc[0], rr_em_lai_info->mcc[1], rr_em_lai_info->mcc[2]);
		if (rr_em_lai_info->mnc[2]!=0x0f)
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_1, "<MNC>", rr_em_lai_info->mnc[0], rr_em_lai_info->mnc[1], rr_em_lai_info->mnc[2]);
		else
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_5, "<MNC>", rr_em_lai_info->mnc[0], rr_em_lai_info->mnc[1], "F");
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_4, "<LAC>", rr_em_lai_info->lac[0], rr_em_lai_info->lac[1]);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2, "<CELL_ID>", rr_em_lai_info->cell_id);
	}
	else
	{
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10, "<MCC>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10, "<MNC>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10, "<LAC>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10, "<CELL_ID>", NA_SYMBOL);
	}
	mmi_screen_text[i++][0]='\0';
   MMI_ASSERT(i<=MAX_TRACER_LINE_NUM);
   return 1;
}

/* 
*   Function 
*      mmi_em_rr_radio_link_counter_info_logs_value
*   DESCRIPTION
*      keep RR radio link counter info log 
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
void mmi_em_rr_radio_link_counter_info_logs_value(rr_em_radio_link_counter_info_struct *rr_em_radio_link_counter_info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	mmi_rr_em_radio_link_counter_info.is_present = 1;
	mmi_rr_em_radio_link_counter_info.max_value = rr_em_radio_link_counter_info->max_value;
	mmi_rr_em_radio_link_counter_info.current_value = rr_em_radio_link_counter_info->current_value;
	mmi_rr_em_radio_link_counter_info.dtx_ind = rr_em_radio_link_counter_info->dtx_ind;
	mmi_rr_em_radio_link_counter_info.dtx_used = rr_em_radio_link_counter_info->dtx_used;
}

/* 
*   Function 
*      mmi_em_rr_radio_link_counter_info_logs
*   DESCRIPTION
*      display RR radio link counter info log 
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
kal_uint8 mmi_em_rr_radio_link_counter_info_logs(kal_uint8 screen_num, mmi_rr_em_radio_link_counter_info_t *rr_em_radio_link_counter_info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	kal_uint8 i=0;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_9, "DSC/RLC Cnt", screen_num+1, MAX_TRACER_SCREEN_NUM);
	sprintf((char *)mmi_screen_text[i++], "-------------------------");
	if ( (receive_last_info&RR_EM_RADIO_LINK_COUNTER_INFO) && (rr_em_radio_link_counter_info->is_present!=0) )
	{
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2, "<MAX_VALUE>",rr_em_radio_link_counter_info->max_value);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2, "<CUR_VALUE>",rr_em_radio_link_counter_info->current_value);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2, "<DTX_IND>",rr_em_radio_link_counter_info->dtx_ind);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2, "<DTX_USED>",rr_em_radio_link_counter_info->dtx_used);
	}
	else
	{
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10, "<MAX_VALUE>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10, "<CUR_VALUE>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10, "<DTX_IND>", NA_SYMBOL);
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10, "<DTX_USED>", NA_SYMBOL);
	}
	mmi_screen_text[i++][0]='\0';
   MMI_ASSERT(i<=MAX_TRACER_LINE_NUM);
   return 1;
}


/* 
*   Function 
*      mmi_em_rr_measurement_report_info_logs_value
*   DESCRIPTION
*      display RR measurement report information log 
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
void mmi_em_rr_measurement_report_info_logs_value(rr_em_measurement_report_info_struct *rr_em_measurement_report_info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	int j;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	mmi_rr_em_measurement_report_info.is_present = 1;
	mmi_rr_em_measurement_report_info.serving_arfcn = rr_em_measurement_report_info->serving_arfcn;
	mmi_rr_em_measurement_report_info.serv_rla_in_quarter_dbm = rr_em_measurement_report_info->serv_rla_in_quarter_dbm;
	mmi_rr_em_measurement_report_info.serving_bsic = rr_em_measurement_report_info->serving_bsic;
	mmi_rr_em_measurement_report_info.c1_serv_cell = rr_em_measurement_report_info->c1_serv_cell;
	mmi_rr_em_measurement_report_info.c2_serv_cell = rr_em_measurement_report_info->c2_serv_cell;

	mmi_rr_em_measurement_report_info.serv_gprs_supported = rr_em_measurement_report_info->serv_gprs_supported;
	mmi_rr_em_measurement_report_info.gprs_pbcch_present = rr_em_measurement_report_info->gprs_pbcch_present;
	mmi_rr_em_measurement_report_info.num_of_carriers = rr_em_measurement_report_info->num_of_carriers;
	num_of_carriers = rr_em_measurement_report_info->num_of_carriers;
	mmi_rr_em_measurement_report_info.rr_state = rr_em_measurement_report_info->rr_state;
	mmi_rr_em_measurement_report_info.multiband_report = rr_em_measurement_report_info->multiband_report;

	for (j=0; j<12; j++)
	{
		mmi_rr_em_measurement_report_info.nc_arfcn[j] = rr_em_measurement_report_info->nc_arfcn[j];
		mmi_rr_em_measurement_report_info.rla_in_quarter_dbm[j] = rr_em_measurement_report_info->rla_in_quarter_dbm[j];
		mmi_rr_em_measurement_report_info.c1[j] = rr_em_measurement_report_info->c1[j];
		mmi_rr_em_measurement_report_info.c2[j] = rr_em_measurement_report_info->c2[j];
		mmi_rr_em_measurement_report_info.nc_info_status[j] = rr_em_measurement_report_info->nc_info_status[j];
	}
	
}

/* 
*   Function 
*      mmi_em_rr_measurement_report_info_logs
*   DESCRIPTION
*      keep RR measurement report information log 
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
kal_uint8 mmi_em_rr_measurement_report_info_logs(kal_uint8 screen_num, mmi_rr_em_measurement_report_info_t *rr_em_measurement_report_info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	kal_uint8 i=0;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

//page one
	sprintf((char *)mmi_screen_text[i++],TEXT_FORMAT_9, "Measure Rep", screen_num+1, MAX_TRACER_SCREEN_NUM);
	sprintf((char *)mmi_screen_text[i++], "-------------------------");
	if ( (receive_last_info&RR_EM_MEASUREMENT_REPORT_INFO) && (rr_em_measurement_report_info->is_present!=0) )
	{
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2, "<SERV_ARFCN>",rr_em_measurement_report_info->serving_arfcn);
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2, "<RSSI>",rr_em_measurement_report_info->serv_rla_in_quarter_dbm);
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2, "<BSIC>",rr_em_measurement_report_info->serving_bsic);
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2, "<C1_SERV_CELL>",rr_em_measurement_report_info->c1_serv_cell);
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2, "<C2_SERV_CELL>",rr_em_measurement_report_info->c2_serv_cell);
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10, "<C31>","X"/* reserved rr_em_measurement_report_info->c31*/);		
	}
	else
	{
		   sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10, "<G_SUP>", NA_SYMBOL);
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10, "<PBCCH>", NA_SYMBOL);
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10, "<NUM_CARRIERS>", NA_SYMBOL);
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10, "<RR_STATE>", NA_SYMBOL);
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10, "<MULTIBAND>", NA_SYMBOL);
	}
	mmi_screen_text[i++][0]='\0';
   MMI_ASSERT(i<=MAX_TRACER_LINE_NUM);


   //return 2;
   return 1;
}

/* 
*   Function 
*      mmi_em_rr_measurement_report_info_logs_2
*   DESCRIPTION
*      display RR measurement report information log 
*   PARAMETERS
*      VOID      
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
kal_uint8 mmi_em_rr_measurement_report_info_logs_2(kal_uint8 screen_num, mmi_rr_em_measurement_report_info_t *rr_em_measurement_report_info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	kal_uint8 i=0;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	
//page two
	MMI_ASSERT(screen_num<MAX_TRACER_SCREEN_NUM);
	i = 0;
	sprintf((char *)mmi_screen_text[i++],TEXT_FORMAT_9, "Measure Rep", screen_num+1, MAX_TRACER_SCREEN_NUM);
	sprintf((char *)mmi_screen_text[i++], "-------------------------");
	if ( (receive_last_info&RR_EM_MEASUREMENT_REPORT_INFO) && (rr_em_measurement_report_info->is_present!=0) )
	{
		   sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2, "<G_SUP>",rr_em_measurement_report_info->serv_gprs_supported);
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2, "<PBCCH>",rr_em_measurement_report_info->gprs_pbcch_present);
			num_of_carriers =	 rr_em_measurement_report_info->num_of_carriers;
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2, "<NUM_CARRIERS>",rr_em_measurement_report_info->num_of_carriers);
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2, "<RR_STATE>",rr_em_measurement_report_info->rr_state);
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_2, "<MULTIBAND>",rr_em_measurement_report_info->multiband_report);

	}
	else
	{
		   sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10, "<G_SUP>", NA_SYMBOL);
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10, "<PBCCH>", NA_SYMBOL);
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10, "<NUM_CARRIERS>", NA_SYMBOL);
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10, "<RR_STATE>", NA_SYMBOL);
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_10, "<MULTIBAND>", NA_SYMBOL);

	}
	mmi_screen_text[i++][0]='\0';
   MMI_ASSERT(i<=MAX_TRACER_LINE_NUM);

   return 1;

}

/* 
*   Function 
*      mmi_em_rr_c1_c2_logs
*   DESCRIPTION
*      to keep the GPRS RR C1/C2 information on the screen text.
*   PARAMETERS
*      screen_num 	to keep at specific screen
*   RETURNS
*      void
*   LOCAL AFFECTED
*/
kal_uint8 mmi_em_rr_c1_c2_logs(kal_uint8 screen_num, mmi_rr_em_measurement_report_info_t *rr_em_measurement_report_info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	kal_uint8 i=0,j;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	sprintf((char *)mmi_screen_text[i++],TEXT_FORMAT_9, "Nebr/Srv Cell", screen_num+1, MAX_TRACER_SCREEN_NUM);
	sprintf((char *)mmi_screen_text[i++], "-------------------------");
	if ( (receive_last_info&RR_EM_MEASUREMENT_REPORT_INFO) && (rr_em_measurement_report_info->is_present!=0) )
	{
		sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_7, "arfcn", "rssi", "c1" ,"c2" ,"G");
			for (j=0; j<6; j++)
			{
				if ( ((rr_em_measurement_report_info->nc_info_status[j] & 0x03)==0x03) &&
					 (rr_em_measurement_report_info->c1[j]!=-1) && (rr_em_measurement_report_info->c2[j]!=-1) )
				{
					sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_8, 
																rr_em_measurement_report_info->nc_arfcn[j], 
																rr_em_measurement_report_info->rla_in_quarter_dbm[j],
																rr_em_measurement_report_info->c1[j],
																rr_em_measurement_report_info->c2[j],
																	(rr_em_measurement_report_info->nc_info_status[j]&0x04 ? 1:0));
				}
				else
				{
					sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_11, 
																rr_em_measurement_report_info->nc_arfcn[j], 
																rr_em_measurement_report_info->rla_in_quarter_dbm[j],
																	" X ",
																	" X ",
																	0);
				}
			}
	}
	else
	{
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_7, "arfcn", "rssi", "c1" ,"c2" ,"G");
			for (j=0; j<6; j++)
				sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_13,  " X ", " X ", " X ", " X ", " X ");
	}
	mmi_screen_text[i++][0]='\0';
   MMI_ASSERT(i<=MAX_TRACER_LINE_NUM);

	return 1;
}

/* 
*   Function 
*      mmi_em_rr_c1_c2_logs_2
*   DESCRIPTION
*      to keep the GPRS RR C1/C2 information on the screen text.
*   PARAMETERS
*      screen_num 	to keep at specific screen
*   RETURNS
*      void
*   LOCAL AFFECTED
*/
kal_uint8 mmi_em_rr_c1_c2_logs_2(kal_uint8 screen_num, mmi_rr_em_measurement_report_info_t *rr_em_measurement_report_info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	kal_uint8 i=0,j;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

//page two
	
	sprintf((char *)mmi_screen_text[i++],TEXT_FORMAT_9, "Nebr/Srv Cell", screen_num+1, MAX_TRACER_SCREEN_NUM);
	sprintf((char *)mmi_screen_text[i++], "-------------------------");
	if ( (receive_last_info&RR_EM_MEASUREMENT_REPORT_INFO) && (rr_em_measurement_report_info->is_present!=0) )
	{
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_7, "arfcn", "rssi", "c1" ,"c2" ,"G");
			for (j=6; j<12; j++)
			{
				if ( ((rr_em_measurement_report_info->nc_info_status[j] & 0x03)==0x03) &&
					 (rr_em_measurement_report_info->c1[j]!=-1) && (rr_em_measurement_report_info->c2[j]!=-1) )
				{
					sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_8, 
																rr_em_measurement_report_info->nc_arfcn[j], 
																rr_em_measurement_report_info->rla_in_quarter_dbm[j],
																rr_em_measurement_report_info->c1[j],
																rr_em_measurement_report_info->c2[j],
																	(rr_em_measurement_report_info->nc_info_status[j]&0x04 ? 1:0));
				}
				else
				{
					sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_11, 
																rr_em_measurement_report_info->nc_arfcn[j], 
																rr_em_measurement_report_info->rla_in_quarter_dbm[j],
																	" X ",
																	" X ",
																	0);
				}
			}
	}
	else
	{
			sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_7, "arfcn", "rssi", "c1" ,"c2" ,"G");
			for (j=0; j<6; j++)
				sprintf((char *)mmi_screen_text[i++], TEXT_FORMAT_13,  " X ", " X ", " X ", " X ", " X ");
	}
	mmi_screen_text[i++][0]='\0';
   MMI_ASSERT(i<=MAX_TRACER_LINE_NUM);

	return 1;

}

/* 
*   Function 
*      mmi_show_screen_text_log
*   DESCRIPTION
*      show current log screen
*   PARAMETERS
*      screen_num 	to keep at specific screen
*   RETURNS
*      void
*   LOCAL AFFECTED
*/
kal_uint8 mmi_show_screen_text_log(int screen_number)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	kal_uint8	screen_num_id=0;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	switch(screen_number)
	{
	case 0: 
		screen_num_id = mmi_em_rr_measurement_report_info_logs(screen_number, &mmi_rr_em_measurement_report_info);
		break;
	case 1: 
		screen_num_id = mmi_em_rr_measurement_report_info_logs_2(screen_number, &mmi_rr_em_measurement_report_info);
		break;
 	case 2: 
 		screen_num_id = mmi_em_rr_cell_select_logs(screen_number, &mmi_rr_em_cell_select_para_info);
		break;
	case 3: 
		screen_num_id = mmi_em_rr_channel_descr_logs(screen_number, &mmi_rr_em_channel_descr_info);
		break;
	case 4: 
		screen_num_id = mmi_em_rr_ctrl_channel_descr_logs(screen_number, &mmi_rr_em_ctrl_channel_descr_info);
		break;
	case 5: 
		screen_num_id = mmi_em_rr_rach_ctrl_para_info_logs(screen_number, &mmi_rr_em_rach_ctrl_para_info);
		break;
	case 6: 
		screen_num_id = mmi_em_rr_lai_info_logs(screen_number, &mmi_rr_em_lai_info);
		break;
	case 7: 
		screen_num_id = mmi_em_rr_radio_link_counter_info_logs(screen_number, &mmi_rr_em_radio_link_counter_info);
		break;
	case 8: 
		screen_num_id = mmi_em_rr_c1_c2_logs(screen_number, &mmi_rr_em_measurement_report_info);
		break;
	case 9: 
		screen_num_id = mmi_em_rr_c1_c2_logs_2(screen_number, &mmi_rr_em_measurement_report_info);
		break;
	default:
		break;
	}
	return screen_num_id;
	
}

/* 
*   Function 
*      mmi_em_rr_process_logs
*   DESCRIPTION
*      to dispatch the log information.
*   PARAMETERS
*      info 	indication message
*   RETURNS
*      void
*   LOCAL AFFECTED
*/
void mmi_em_rr_process_logs(kal_uint32 em_info, peer_buff_struct *pdu_buff_ptr)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   
	PRINT_INFORMATION_2((MMI_TRACE_INFO, "mmi_em_rr_process_logs=%d", em_info));	
   switch(em_info)
   {
   case RR_EM_CELL_SELECT_PARA_INFO:
   	{
	      rr_em_cell_select_para_info_struct *data_ptr;
	   	kal_uint16  mm_pdu_len;
	   	/* Get the pdu_ptr where MM peer msg will be stored for transmission */
	   	data_ptr = (rr_em_cell_select_para_info_struct *) get_pdu_ptr( pdu_buff_ptr, &mm_pdu_len);
//			data_ptr = (rr_em_cell_select_para_info_struct *) pdu_buff_ptr;
	   	mmi_em_rr_cell_select_logs_value((rr_em_cell_select_para_info_struct *)data_ptr);
   	}
   	break;
   case RR_EM_CHANNEL_DESCR_INFO:
   	{
	      rr_em_channel_descr_info_struct *data_ptr;
	   	kal_uint16  mm_pdu_len;
	   	/* Get the pdu_ptr where MM peer msg will be stored for transmission */
	   	data_ptr = (rr_em_channel_descr_info_struct *) get_pdu_ptr( pdu_buff_ptr, &mm_pdu_len);
	   	mmi_em_rr_channel_descr_logs_value((rr_em_channel_descr_info_struct *)data_ptr);
   	}
   	break;
   case RR_EM_CTRL_CHANNEL_DESCR_INFO:
   	{
	      rr_em_ctrl_channel_descr_info_struct *data_ptr;
	   	kal_uint16  mm_pdu_len;
	   	/* Get the pdu_ptr where MM peer msg will be stored for transmission */
	   	data_ptr = (rr_em_ctrl_channel_descr_info_struct *) get_pdu_ptr( pdu_buff_ptr, &mm_pdu_len);
//			data_ptr = (rr_em_ctrl_channel_descr_info_struct *) pdu_buff_ptr;
	   	mmi_em_rr_ctrl_channel_descr_logs_value((rr_em_ctrl_channel_descr_info_struct *)data_ptr);
   	}
   	break;
  	case RR_EM_RACH_CTRL_PARA_INFO:
   	{
	      rr_em_rach_ctrl_para_info_struct *data_ptr;
	   	kal_uint16  mm_pdu_len;
	   	/* Get the pdu_ptr where MM peer msg will be stored for transmission */
	   	data_ptr = (rr_em_rach_ctrl_para_info_struct *) get_pdu_ptr( pdu_buff_ptr, &mm_pdu_len);
//			data_ptr = (rr_em_rach_ctrl_para_info_struct *) pdu_buff_ptr;
	  		mmi_em_rr_rach_ctrl_para_info_logs_value((rr_em_rach_ctrl_para_info_struct *)data_ptr);
  		}
  		break;
  	case RR_EM_LAI_INFO:
   	{
	      rr_em_lai_info_struct *data_ptr;
	   	kal_uint16  mm_pdu_len;
	   	/* Get the pdu_ptr where MM peer msg will be stored for transmission */
	   	data_ptr = (rr_em_lai_info_struct *) get_pdu_ptr( pdu_buff_ptr, &mm_pdu_len);
	  		mmi_em_rr_lai_info_logs_value((rr_em_lai_info_struct *)data_ptr);
  		}
  		break;
	case RR_EM_RADIO_LINK_COUNTER_INFO:
		{
	      rr_em_radio_link_counter_info_struct *data_ptr;
	   	kal_uint16  mm_pdu_len;
	   	/* Get the pdu_ptr where MM peer msg will be stored for transmission */
	   	data_ptr = (rr_em_radio_link_counter_info_struct *) get_pdu_ptr( pdu_buff_ptr, &mm_pdu_len);
			mmi_em_rr_radio_link_counter_info_logs_value((rr_em_radio_link_counter_info_struct *)data_ptr);
		}
		break;
	case RR_EM_MEASUREMENT_REPORT_INFO:
		{
	      rr_em_measurement_report_info_struct *data_ptr;
	   	kal_uint16  mm_pdu_len;
	   	/* Get the pdu_ptr where MM peer msg will be stored for transmission */
	   	data_ptr = (rr_em_measurement_report_info_struct *) get_pdu_ptr( pdu_buff_ptr, &mm_pdu_len);
			mmi_em_rr_measurement_report_info_logs_value((rr_em_measurement_report_info_struct *)data_ptr);
		}
		break;
  	default:
  		break;
   }
   free_peer_buff(pdu_buff_ptr);

}

/* 
*   Function 
*      mmi_em_parsing_data
*   DESCRIPTION
*      to keep the indication message data.
*   PARAMETERS
*      info 	indication message
*   RETURNS
*      void
*   LOCAL AFFECTED
*/
void mmi_em_parsing_data(void *info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	mmi_em_status_ind_struct* msg = (mmi_em_status_ind_struct*)info;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	PRINT_INFORMATION_2((MMI_TRACE_INFO, "mmi_em_parsing_data=%d, %d", msg->mod_id, msg->em_info));	

	if (msg->mod_id==MOD_AS) 
	{
 		mmi_em_rr_process_logs(msg->em_info, msg->info);
 		
 		/* display the trace information on screen */
 		mmi_em_show_screen();
	}
	else
	{

		/* no such module */
		//MMI_ASSERT(0);
	}
	

}
	
/* 
*   Function 
*      mmi_em_trace_on_ind
*   DESCRIPTION
*      turn on the trace.
*   PARAMETERS
*      void
*   RETURNS
*      void
*   LOCAL AFFECTED
*/
void mmi_em_trace_on_ind(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	PRINT_INFORMATION_2((MMI_TRACE_INFO, "mmi_em_trace_data_ind"));	
	SetProtocolEventHandler(mmi_em_parsing_data, MSG_ID_MMI_EM_STATUS_IND);
}

/* 
*   Function 
*      mmi_em_trace_off_ind
*   DESCRIPTION
*      turn off the trace.
*   PARAMETERS
*      void
*   RETURNS
*      void
*   LOCAL AFFECTED
*/
void mmi_em_trace_off_ind(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	PRINT_INFORMATION_2((MMI_TRACE_INFO, "mmi_em_trace_off_ind"));	
	ClearProtocolEventHandler((U16)MSG_ID_MMI_EM_STATUS_IND);
}


/* 
*   Function 
*      mmi_em_show_screen
*   DESCRIPTION
*      focus on screen to display.
*   PARAMETERS
*      void
*   RETURNS
*      void
*   LOCAL AFFECTED
*/
void mmi_em_show_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	//extern kal_uint8 gIsOnIdleScreen;
	//extern kal_int8 gPrevScreenIndicator;
	kal_uint16	current_screen;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if (gIsOnEMTracer==0)
	{
         EntryNewScreen_Ext(EM_LOG_SCREEN_ID, mmi_em_exit_screen, NULL, NULL);
	}

	current_screen = GetCurrScrnId();
	PRINT_INFORMATION_2((MMI_TRACE_INFO, "mmi_em_show_screen %d", current_screen));	

   if ( (current_screen==IDLE_SCREEN_ID) || (current_screen==SCR_SIM_INSERTION) || (current_screen==SCR_CM_ACTIVECALLSCREEN)) 
   {
		if (IsScreenPresent(EM_LOG_SCREEN_ID)) DeleteScreenIfPresent(EM_LOG_SCREEN_ID); //DeleteNScrId(EM_LOG_SCREEN_ID);

		gIsOnEMTracer = 1;

		/* register key handler */
		mmi_em_tracer_key();
		if (current_screen == SCR_CM_ACTIVECALLSCREEN)
		{
			U16 IdleScreenDigits[MAX_CM_SCREEN_DIGITS] =
			{
																KEY_0,KEY_1,KEY_2,KEY_3,
																KEY_4,KEY_5,KEY_6,KEY_7,
																KEY_8,KEY_9,KEY_STAR,
																KEY_POUND
															};
			SetGroupKeyHandler(EnterScreenAndHandleDigit,IdleScreenDigits,MAX_CM_SCREEN_DIGITS-1,KEY_EVENT_DOWN);
		}

		mmi_em_display_screen_n(index_screen);

   }
   else if (gIsOnEMTracer==0)
   	GoBackHistory();


}

/* 
*   Function 
*      mmi_em_display_screen_n
*   DESCRIPTION
*      display page of screen.
*   PARAMETERS
*      screen_num   page of screen
*   RETURNS
*      void
*   LOCAL AFFECTED
*/
void mmi_em_display_screen_n(kal_uint8 screen_num)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	kal_int32 i;
	//extern kal_uint8 gIsOnIdleScreen;
	extern bitmap*	current_LCD_device_bitmap;
	extern bitmap	main_LCD_device_bitmap;
	extern bitmap	sub_LCD_device_bitmap;
	//extern kal_int8 gPrevScreenIndicator;
   bitmap	*saved_graphics_context=current_LCD_device_bitmap;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	PRINT_INFORMATION_2((MMI_TRACE_INFO, "mmi_em_display_screen_n=%d",screen_num));	

   MMI_ASSERT(screen_num<MAX_TRACER_SCREEN_NUM);
   
//	if ( (gIsOnIdleScreen!=1) && (gPrevScreenIndicator!=ENTER_SIM_INSERTION) && (GetCurrentState()!=CM_ACTIVE_STATE) ) return;

//	if (IsScreenPresent(EM_LOG_SCREEN_ID)) DeleteNScrId(EM_LOG_SCREEN_ID);
//	ExecuteCurrExitHandler();
	
	/* register key handler */
	//mmi_em_tracer_key();
//	if (gIsOnEMTracer!=1) return;

#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_lock_double_buffer();
#endif

   mmi_em_tracer_screen_color_init(255,255,255);
	UI_set_main_LCD_graphics_context();

	mmi_show_screen_text_log(screen_num);
	
	for (i=0; i<MAX_TRACER_LINE_NUM; i++)
	{
		if (mmi_screen_text[i][0] == '\0')
			break;
		mmi_em_display_line_n(i, mmi_screen_text[i]);
	}

     //RollBack to original LCD
	if(saved_graphics_context==&main_LCD_device_bitmap)
		UI_set_main_LCD_graphics_context();
	else if(saved_graphics_context==&sub_LCD_device_bitmap)
		UI_set_sub_LCD_graphics_context();
	
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0,0,MAIN_LCD_device_width-1,MAIN_LCD_device_height-1);
#endif


}

/* 
*   Function 
*      mmi_em_display_line_n
*   DESCRIPTION
*      display screen text line.
*   PARAMETERS
*      line_num   index line number
*      str	display text string
*   RETURNS
*      void
*   LOCAL AFFECTED
*/
void mmi_em_display_line_n(kal_uint8 line_num, kal_uint8 *str)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	kal_uint8 temp_str[200];
    
	kal_uint32 alignment_width=0, alignment_height=0;
	extern s32 UI_get_font_height(UI_font_type f);

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	PRINT_INFORMATION_2((MMI_TRACE_INFO, "mmi_em_display_line_n"));	
   MMI_ASSERT(line_num<MAX_TRACER_LINE_NUM);

   AnsiiNToUnicodeString((S8*)temp_str, (S8*)str, sizeof(temp_str)/2-1);
   Get_StringWidthHeight((U8*)temp_str, (S32 *)&alignment_width, (S32 *)&alignment_height);
   
	coolsand_UI_set_font(&MMI_small_font);
	coolsand_UI_set_text_clip(UI_clip_x1,UI_clip_y1,UI_clip_x2,UI_clip_y2);
	coolsand_UI_set_text_color(coolsand_UI_color(0,0,0));
   coolsand_UI_move_text_cursor(0/*(MAIN_LCD_device_width-alignment_width)/2*/, UI_clip_y1+line_num*UI_get_font_height(&MMI_small_font));
	coolsand_UI_print_text((UI_string_type )temp_str);

}

/* 
*   Function 
*      mmi_em_tracer_screen_color_init
*   DESCRIPTION
*      Set the text color value of screen.
*   PARAMETERS
*      void
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
void mmi_em_tracer_screen_color_init(kal_uint8 color_r, kal_uint8 color_g, kal_uint8 color_b)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   stFontAttribute	debug_print_font={ 0,0,0,SMALL_FONT,0,0 };
   color_t	c;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   setup_UI_wrappers();
   c=coolsand_UI_color(color_r,color_g,color_b);
   UI_set_clip(0,0,MAIN_LCD_device_width, MAIN_LCD_device_height-16);
   UI_fill_rectangle(0,0,MAIN_LCD_device_width, MAIN_LCD_device_height-16,c);
//	UI_reset_text_clip();
	UI_set_font(&debug_print_font);
   c=coolsand_UI_color(0,0,0);  
   UI_set_text_color(c);
}

/* 
*   Function 
*      mmi_em_display_init
*   DESCRIPTION
*      Set the default value of text screen.
*   PARAMETERS
*      void
*   RETURNS
*      void
*   GLOBALS AFFECTED
*/
void mmi_em_display_init(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
    
   //kal_int8	page_count=0;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   memset(mmi_screen_text, 0, sizeof(mmi_screen_text));

	mmi_rr_em_cell_select_para_info.is_present = 0;
	mmi_rr_em_channel_descr_info.is_present = 0;
	mmi_rr_em_ctrl_channel_descr_info.is_present = 0;
	mmi_rr_em_rach_ctrl_para_info.is_present = 0;
	mmi_rr_em_lai_info.is_present = 0;
	mmi_rr_em_radio_link_counter_info.is_present = 0;
	mmi_rr_em_measurement_report_info.is_present = 0;

}

#endif

