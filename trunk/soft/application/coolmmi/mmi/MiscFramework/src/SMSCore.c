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
 *  SMSCore.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifdef __MOD_SMSAL__

#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
//... Add More MMI header
#include "eventsgprot.h"
#include "historygprot.h"
#include "globalmenuitems.h"
#include "globalconstants.h"
#include "globaldefs.h"
#include "wgui_categories.h"
#include "custmenures.h"
#include "commonscreens.h"
#include "unicodexdcl.h"
#include "timerevents.h"
#include "debuginitdef.h"
#include "protocolevents.h"

//...Add MSG header
#include "customer_ps_inc.h"
#include "mmi_msg_context.h"
#include "messagesl4def.h"
#include "messagesresourcedata.h"
#include "messagesmiscell.h"
#include "messagesexdcl.h"
#include "smsguiinterfaceprot.h"
#include "smsguiinterfacetype.h"
#include "smspshandler.h"
#include "cellbroadcasttype.h"
#ifdef __MMI_MESSAGES_CHAT__
#include "chatappgprot.h"
#include "chatappdef.h"
#endif
//
#include "customer_ps_inc.h"
#include "l4c2smsal_struct.h"
#include "smsal_l4c_enum.h"
#include "ems.h"
//
#include "gui_ems.h"
#include "wgui_ems_categories.h"
#include "gsm7bitdefaultalphabet.h"
#include "wgui_categories_inputs.h"
//
#include "callhistorygprots.h"
#include "callmanagementgprot.h"
#include "idleappprot.h"
#include "settingprofile.h"
#include "profilegprots.h"
#include "alarmframeworkprot.h"
#include "phonebooktypes.h"
#include "simdetectiongexdcl.h"
#include "settingdefs.h"
#include "settingsgdcl.h"
//
#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"
#include "mmi_trace.h"

#include "wgui_status_icons.h"
#include "keybrd.h"
#undef  __NEWSIMULATOR

#include "smsapi.h"
#include "smsstruct.h"
#include "smsfunc.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#include "mtpnp_ad_resdef.h"
#endif

#if 1
#ifndef MMI_FRM_SMS_INVALID_NUM
#define MMI_FRM_SMS_INVALID_NUM		0xff
#endif
#ifndef MMI_FRM_SMS_ACTION_SIZE
#define MMI_FRM_SMS_ACTION_SIZE		10
#endif
#ifndef MMI_FRM_SMS_INVALID_INDEX
#define MMI_FRM_SMS_INVALID_INDEX	0xffff
#endif

#define SWAP(T,A,B) { T TMP = A; A = B; B = TMP; }

static mmi_frm_sms_action_struct mmi_frm_sms_action[MMI_FRM_SMS_ACTION_SIZE];
static U8 mmi_frm_sms_action_head = 0;
static U8 mmi_frm_sms_action_tail = 0;
static U8 mmi_frm_sms_action_num = 0;
static U8 mmi_frm_sms_action_curr = MMI_FRM_SMS_INVALID_NUM;

static mmi_frm_sms_msg_check_struct mmi_frm_sms_msg_check[MMI_FRM_SMS_ACTION_SIZE];
static U8 mmi_frm_sms_msg_check_num = 0;

static mmi_frm_sms_data_check_struct mmi_frm_sms_data_check[MMI_FRM_SMS_ACTION_SIZE];
static U8 mmi_frm_sms_data_check_num = 0;

static U8 mmi_frm_sms_search_phb_type = 0;

mmi_frm_sms_context_struct g_frm_sms_cntx;
#ifdef __MMI_MESSAGES_COPY__
mmi_frm_sms_copy_info_struct g_frm_sms_copy_info;
#endif

/*******************************************************

	mmi sms app test code start

*******************************************************/
 
msg_ems_data_struct	bufForDisplay ;    
//static msg_ems_data_struct	bufForEdit ;   
msg_ems_data_struct	bufForEdit ;   
 kal_uint8 ems_input_buffer[1226]; //wangzl:modify from 1224 for bug 5893,5904             //wangbei add 20060623
 kal_uint8 ems_Display_buffer[1244];          

extern void mmi_phb_frm_enter_send_entry(void) ;


/*****************************************************************************
* FUNCTION
*  InitMessagesEMSBuffer
* DESCRIPTION
*   Reset EMS edit/view buffer
*
* PARAMETERS
*  a  IN/OUT      msg_ems_data_struct EME buffer
*  b  IN       force to clear
* RETURNS
*  EMS buffer valid flag
* GLOBALS AFFECTED
*   bufForDisplay/bufForEdit
*****************************************************************************/
// wangbei add start 20060614
EMSTATUS ReleaseEMSData (EMSData *emsData)
{
   memset(bufForEdit.emsData.textBuffer, 0, bufForEdit.emsData.textBufferSize); 

   return EMS_OK;
}

EMSTATUS InitializeEMSData (EMSData *temEMSdata, kal_uint8 dcs)
{
       temEMSdata->CurrentPosition.OffsetToText=0;
       temEMSdata->CurrentPosition.Object=NULL;
       temEMSdata->CurrentTextFormatObj=NULL;
        temEMSdata->listHead=NULL;
       temEMSdata->listTail=NULL;
       temEMSdata->id=0;
       
       /*modified start by wuzc for psi-mmi at 2006810*/
       #if 0
        
       #else
       temEMSdata->textBuffer=ems_input_buffer;
       #endif
       /*modified end by wuzc for psi-mmi at 2006810*/
       temEMSdata->textLength=0;
       temEMSdata->textBufferSize=1224;//wangzl:modify from 1224 for bug 6021 //guojian restore for bug 10360
       /*modified start by wuzc for psi-mmi at 2006810*/
       temEMSdata->dcs	= dcs;
       /*modified end by wuzc for psi-mmi at 2006810*/
       temEMSdata->ps_dcs=0;
       temEMSdata->udhi=0;
       temEMSdata->isConcat=0;
       temEMSdata->Reminder.isSegChange=0;
       temEMSdata->Reminder.requiredSegment=1;
       temEMSdata->Reminder.segRemainingOctet=160;
       temEMSdata->Reminder.segUsableOctet=160;
       temEMSdata->Reminder.totalRemainingOctet=612;//wangzl:modify from 612 for bug 6021
       temEMSdata->Reminder.maxUsableLenPerSeg=153;//wangzl:modify from 153for bug 6021
       temEMSdata->PortNum.isPortNumSet=0;
       temEMSdata->PortNum.src_port=0;
       temEMSdata->PortNum.dst_port=0;
       temEMSdata->internal=NULL;

       return EMS_OK;
}

EMSTATUS EMSInitialize(void)
{
	return EMS_OK;	
}

// wangbei add end
 
EMSTATUS ReleaseEMSDisplayData (EMSData *emsData)
{
	memset(bufForDisplay.emsData.textBuffer, 0, bufForDisplay.emsData.textBufferSize); 

	return EMS_OK;
}

EMSTATUS InitializeEMSDisplayData (EMSData *temEMSdata, kal_uint8 dcs)
{
       temEMSdata->CurrentPosition.OffsetToText=0;
       temEMSdata->CurrentPosition.Object=NULL;
       temEMSdata->CurrentTextFormatObj=NULL;
       temEMSdata->listHead=NULL;
       temEMSdata->listTail=NULL;
       temEMSdata->id=0;
       //temEMSdata->textBuffer=ems_Display_buffer;
       //temEMSdata->textLength=0;
       temEMSdata->textBufferSize=1224;//wangzl:modify from 1224 for bug 6021 //guojian restore for bug 10360
       //temEMSdata->dcs=0;
       temEMSdata->ps_dcs=0;
       temEMSdata->udhi=0;
       temEMSdata->isConcat=0;
       temEMSdata->Reminder.isSegChange=0;
       temEMSdata->Reminder.requiredSegment=1;
       temEMSdata->Reminder.segRemainingOctet=160;
       temEMSdata->Reminder.segUsableOctet=160;
       temEMSdata->Reminder.totalRemainingOctet=612;//wangzl:modify from 612 for bug 6021 //guojian restore for bug 10360
       temEMSdata->Reminder.maxUsableLenPerSeg=153;//wangzl:modify from 153 for bug 6021 //guojian restore for bug 10360
       temEMSdata->PortNum.isPortNumSet=0;
       temEMSdata->PortNum.src_port=0;
       temEMSdata->PortNum.dst_port=0;
       temEMSdata->internal=NULL;

	   return EMS_OK;
}

U8 InitMessagesEMSDisplayBuffer (msg_ems_data_struct * pE, U8 force)
{
	EMSTATUS  retVal;
	U8 oldFlag = pE->validFlag;

	if ((pE->validFlag == 0) || force)
	{
		if (pE->validFlag)
		{
			retVal = ReleaseEMSDisplayData (& pE->emsData) ;
			mmi_trace(g_sw_SMS, "MMI_SMS: InitMessagesEMSBuffer: ReleaseEMSData() returns %d", (int) retVal);
		}
		/*modified start by wuzc for psi-mmi at 2006810*/
		retVal = InitializeEMSDisplayData (& pE->emsData, SMSAL_UCS2_DCS) ;   // = SMSAL_DEFAULT_DCS
		/*modified end by wuzc for psi-mmi at 2006810*/
		
		mmi_trace(g_sw_SMS, "MMI_SMS: InitMessagesEMSBuffer: InitializeEMSData() returns %d, dcs set to 0X%.2X", (int) retVal, (int) pE->emsData.dcs);
	}

	pE->validFlag = 1;
	return oldFlag;
}
 
U8 InitMessagesEMSBuffer (msg_ems_data_struct * pE, U8 force)
{
	EMSTATUS  retVal;
	U8 oldFlag = pE->validFlag;

	if ((pE->validFlag == 0) || force)
	{
		if (pE->validFlag)
		{
			retVal = ReleaseEMSData (& pE->emsData) ;
			mmi_trace(g_sw_SMS, "MMI_SMS: InitMessagesEMSBuffer: ReleaseEMSData() returns %d-------*\n", (int) retVal);
		}
		retVal = InitializeEMSData (& pE->emsData, 0) ;   // = SMSAL_DEFAULT_DCS
		mmi_trace(g_sw_SMS, "MMI_SMS: InitMessagesEMSBuffer: InitializeEMSData() returns %d, dcs set to 0X%.2X", (int) retVal,(int) pE->emsData.dcs);
	}

	pE->validFlag = 1;
	return oldFlag;
}

/*****************************************************************************
* FUNCTION
*  DeInitMessagesEMSBuffer
* DESCRIPTION
*   Releaset EMS buffer
*
* PARAMETERS
*  a  IN/OUT      msg_ems_data_struct EMS buffer
*  b  IN       
* RETURNS
*  EMS buffer valid flag
* GLOBALS AFFECTED
*   bufForDisplay/bufForEdit
*****************************************************************************/
U8 DeInitMessagesEMSBuffer (msg_ems_data_struct * pE)
{
	U8 oldFlag = pE->validFlag;
	EMSTATUS retVal;

	if (pE->validFlag)
	{
		retVal = ReleaseEMSData (& pE->emsData) ;
		mmi_trace(g_sw_SMS, "MMI_SMS: DeInitMessagesEMSBuffer: ReleaseEMSData() returns %d", (int) retVal);
	}
	pE->validFlag = 0;
	return oldFlag;
}

/*****************************************************************************
* FUNCTION
*  GetEMSDataForView
* DESCRIPTION
*   Get EMS view buffer
*
* PARAMETERS
*  p  	IN		EMS data pointer
*  force  	IN		force to clear
* RETURNS
*  EMS buffer for display
* GLOBALS AFFECTED
*   none
*****************************************************************************/
EMSData * GetEMSDataForView (EMSData ** p, U8 force)
{
	if (p) *p = & bufForDisplay.emsData;

	 
	InitMessagesEMSDisplayBuffer (& bufForDisplay, force);   

	return & bufForDisplay.emsData;
}

/*****************************************************************************
* FUNCTION
*  GetEMSDataForEdit
* DESCRIPTION
*   Get EMS edit buffer
*
* PARAMETERS
*  p  	IN		EMS data pointer
*  force  	IN		force to clear
* RETURNS
*  EMS buffer for edit
* GLOBALS AFFECTED
*   none
*****************************************************************************/
EMSData * GetEMSDataForEdit (EMSData ** p, U8 force)
{
	if (p) *p = & bufForEdit.emsData;

	InitMessagesEMSBuffer (& bufForEdit, force);
	return & bufForEdit.emsData;
}

/*****************************************************************************
* FUNCTION
*  CopyEMSViewToEditBuffer
* DESCRIPTION
*   Copy EMS view buffer to edit buffer
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 CopyEMSViewToEditBuffer (void)
{
	U8 result = EMS_OK;
	
	ReleaseEMSEditBuffer ();
	InitMessagesEMSBuffer (& bufForEdit, 1);
	if (bufForDisplay.validFlag)
	{	
		/*modified start by wuzc for psi-mmi at 2006810*/	
		#if 0
		result = CopyEMSData (&bufForEdit.emsData, &bufForDisplay.emsData);
		#else
		memcpy (&bufForEdit.emsData, &bufForDisplay.emsData,sizeof(EMSData));
		bufForEdit.emsData.textBuffer = ems_input_buffer;
		memcpy(bufForEdit.emsData.textBuffer,bufForDisplay.emsData.textBuffer,bufForDisplay.emsData.textLength);
		bufForEdit.emsData.CurrentPosition.OffsetToText = bufForDisplay.emsData.textLength;
		result = EMS_OK;
		#endif
		/*modified end by wuzc for psi-mmi at 2006810*/	
			
		bufForEdit.validFlag = 1;
		if (result == EMS_OK)
		{
		#ifdef MMI_ON_HARDWARE_P
			EMSResetPortNum(&bufForEdit.emsData);
		#endif
			GoToEndPosition (&bufForEdit.emsData);
		}
		else
		{
			ReleaseEMSEditBuffer(); /* because validFlag for edit buffer is 1, it is equivalent to force to realease */
		}
	}
	else
		mmi_trace(g_sw_SMS, "MMI_SMS: ERROR : CopyEMSViewToEditBuffer: src buffer uninitialized -------*\n");
	return result;
}

/*****************************************************************************
* FUNCTION
*  ReleaseEMSViewBuffer
* DESCRIPTION
*   Release EMS view buffer
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   bufForDisplay
*****************************************************************************/
void ReleaseEMSViewBuffer (void)
{
	DeInitMessagesEMSBuffer (& bufForDisplay);
}

/*****************************************************************************
* FUNCTION
*  ReleaseEMSEditBuffer
* DESCRIPTION
*   Release EMS edit buffer
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   bufForEdit
*****************************************************************************/
void ReleaseEMSEditBuffer (void)
{
	DeInitMessagesEMSBuffer (& bufForEdit);
}

/*****************************************************************************
* FUNCTION
*  GetRefFromNVRAM
* DESCRIPTION
*   Get concatenate reference from NVRAM
*
* PARAMETERS
*  none
* RETURNS
*  reference
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U16 GetRefFromNVRAM (void)
{
	S16 error;
	U16  ref;

	if (ReadValue (NVRAM_SMS_CONCAT_MSG_REF_VALUE, (void *) &ref, DS_SHORT, &error) < 0)
	{
		mmi_trace(g_sw_SMS, "MMI_SMS: GetRefFromNVRAM:: Read from NVRAM failed (ecode == %d)", (int)error);
	}
	return ref;
}

#if 0
/*****************************************************************************
* FUNCTION
*  WriteRefToNVRAM
* DESCRIPTION
*   Write referemce to NVRAM
*
* PARAMETERS
*  val  IN		reference
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
static void WriteRefToNVRAM (U16 val)
{
	S16 error;

	if (WriteValue (NVRAM_SMS_CONCAT_MSG_REF_VALUE, (void *) &val, DS_SHORT, &error) < 0)
	{
		mmi_trace(g_sw_SMS, "MMI_SMS: WriteRefToNVRAM:: Write to NVRAM failed (ecode == %d)", (int)error);
	}
}

/*****************************************************************************
* FUNCTION
*  RefGeneratorCallback
* DESCRIPTION
*   Call back for reference generator
*
* PARAMETERS
*  none
* RETURNS
*  reference
* GLOBALS AFFECTED
*   none
*****************************************************************************/
static U16 RefGeneratorCallback (void)
{
	U16 static ref=65534;

	if (ref == 65534)
	{
		ref = GetRefFromNVRAM();
	}
	++ref;
	WriteRefToNVRAM(ref);
	mmi_trace(g_sw_SMS, "MMI_SMS: RefGeneratorCallback:: New ref number %d", (int) ref);
	return ref;
}
#endif

/*****************************************************************************
* FUNCTION
*  InitEMSDataStruct
* DESCRIPTION
*   Init EMS data structure
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   bufForDisplay, bufForEdit
*****************************************************************************/
void InitEMSDataStruct (void)
{
	EMSInitialize ();
#ifdef __EMS_REL5__
	EMSSetCompressMode(TRUE);	
#endif
	//EMSConcatMsgRefFunc(RefGeneratorCallback);// wangbei del 20060614
	bufForDisplay.validFlag = 0;
	bufForEdit.validFlag = 0;
#ifdef __MMI_MESSAGES_EMS__
	mmi_ems_create_ems_folder();
#endif	
}

/*****************************************************************************
* FUNCTION
*  DeInitEMSDataStruct
* DESCRIPTION
*   Deinit EMS data structure
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void DeInitEMSDataStruct (void)
{
	//EMSDeInitialize ();// wangbei del 20060614
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_reg_interrupt_check
* DESCRIPTION
*   Register SMS related interrupt check
*
* PARAMETERS
*  mod	IN	module
*  msgid	IN	message ID
*  callback	IN	callback for interrupt
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_reg_interrupt_check (module_type mod, /*U16*/U32 msgid, PsFuncPtrU16 callback)   
{
	mmi_frm_sms_reg_interrupt_handler(mod, msgid, callback);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_reg_msg_check
* DESCRIPTION
*   Register SMS check
*
* PARAMETERS
*  action	IN	check action
*  callback	IN	callback for interrupt
* RETURNS
*  none
* GLOBALS AFFECTED
*   mmi_frm_sms_msg_check
*****************************************************************************/
void mmi_frm_sms_reg_msg_check (PsExtPeerIntFuncPtr action, PsFuncPtrFuncPtr callback)
{
	mmi_frm_sms_msg_check[mmi_frm_sms_msg_check_num].action=action;
	mmi_frm_sms_msg_check[mmi_frm_sms_msg_check_num].callback=callback;
	mmi_frm_sms_msg_check_num++;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_indicate_sms
* DESCRIPTION
*   Indicate new SMS
*
* PARAMETERS
*  action	IN	check action
*  callback	IN	callback for interrupt
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_indicate_sms (U16 index)
{
	U8 i=0;
	mmi_frm_sms_deliver_msg_struct* data=OslMalloc(sizeof(mmi_frm_sms_deliver_msg_struct));
	 
	U16 content_len=((MMI_FRM_SMS_MSG_LEN*g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)<(MMI_FRM_SMS_MSG_LEN*10))?(MMI_FRM_SMS_MSG_LEN*g_frm_sms_cntx.mmi_frm_sms_msg_seg_size):(MMI_FRM_SMS_MSG_LEN*10);
	U8* content=OslMalloc(content_len);
	U8 withobject;
	U8 updatelist=FALSE;
	

	memset(content,0x00,content_len);
	withobject=mmi_frm_sms_get_awaited_sms((U8)index, data, content);
	data->msg_data[0]=(U8)index;
	mmi_trace(g_sw_SMS,"MMI_SMS: mmi_frm_sms_indicate_sms, data->index is:%d,data->display_type is:%d",data->index,data->display_type)	;	
	if(data->display_type==SMSAL_MSG_TYPE_UPDATE_ONLY)
	{
		/*default indication here*/
		mmi_frm_sms_msgbox_struct * entry=OslMalloc(sizeof(mmi_frm_sms_msgbox_struct));
		//mmi_frm_sms_concat_struct* concatinfo = (mmi_frm_sms_concat_struct*)&(data->concat_info);
		//U8 segment=((concatinfo->seg%g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)==0)?
		//g_frm_sms_cntx.mmi_frm_sms_msg_seg_size:(concatinfo->seg%g_frm_sms_cntx.mmi_frm_sms_msg_seg_size);
		U8 segment= mmi_frm_sms_index_list[data->index].segment;
		mmi_frm_sms_convert_mt_to_entry(data, entry);
		mmi_frm_sms_add_sms_to_msgbox(entry, data->index, segment);
		OslMfree(entry);
	}
	else
	{
		while(i<mmi_frm_sms_msg_check_num)
		{
			if(mmi_frm_sms_msg_check[i].action((void*)data, (int)withobject, (void*)content)==TRUE)
			{
				mmi_frm_sms_convert_time_stamp(data->scts, data->scts);
				if(mmi_frm_sms_msg_check[i].callback((void*)data, (void*)content))
					updatelist=TRUE;
				break;
			}
			i++;
		}
		if((i==mmi_frm_sms_msg_check_num)||(updatelist==TRUE))
		{
			/*default indication here*/
			mmi_frm_sms_msgbox_struct * entry=OslMalloc(sizeof(mmi_frm_sms_msgbox_struct));
			//mmi_frm_sms_concat_struct* concatinfo = (mmi_frm_sms_concat_struct*)&(data->concat_info);
			//U8 segment=((concatinfo->seg%g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)==0)?
			//g_frm_sms_cntx.mmi_frm_sms_msg_seg_size:(concatinfo->seg%g_frm_sms_cntx.mmi_frm_sms_msg_seg_size);
			U8 segment= mmi_frm_sms_index_list[data->index].segment;
			U16 msgbox_index;
			mmi_frm_sms_convert_mt_to_entry(data, entry);
      			mmi_trace(g_sw_SMS,"MMI_SMS: mmi_frm_sms_indicate_sms, data->index is:%d",data->index)	;	
			msgbox_index = mmi_frm_sms_add_sms_to_msgbox(entry, data->index, segment);
			if(i==mmi_frm_sms_msg_check_num)
			mmi_frm_sms_new_msg_ind(msgbox_index);
			OslMfree(entry);
		}
	}
	mmi_frm_sms_free_awaited((U8)index);
	if(data!=NULL)
	{
		OslMfree(data);
		data=NULL;
	}
	if(content!=NULL)
	{
		OslMfree(content);
		content=NULL;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_indicate_data
* DESCRIPTION
*   Indicate new SMS with port number
*
* PARAMETERS
*  index	IN	data index
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_indicate_data (U8 index)
{
	U8 i=0;
	mmi_frm_sms_deliver_data_struct* data=OslMalloc(sizeof(mmi_frm_sms_deliver_data_struct));
	 
	U16 content_len=((MMI_FRM_SMS_MSG_LEN*g_frm_sms_cntx.mmi_frm_sms_msg_seg_size)<(MMI_FRM_SMS_MSG_LEN*10))?(MMI_FRM_SMS_MSG_LEN*g_frm_sms_cntx.mmi_frm_sms_msg_seg_size):(MMI_FRM_SMS_MSG_LEN*10);
	U8* content=OslMalloc(content_len);
	
	memset(content,0,content_len);
	mmi_frm_sms_set_data_check(index, 0);
	mmi_frm_sms_get_data(index, data, content);
	while(i<mmi_frm_sms_data_check_num)
	{
		if((mmi_frm_sms_data_check[i].module==data->dest_mod_id)
			&&(mmi_frm_sms_data_check[i].desport==data->dest_port))
		{
			mmi_frm_sms_data_check[i].callback((void*)data, (module_type)data->dest_mod_id, mmi_frm_sms_data_check[i].serial);
			if(mmi_frm_sms_data_check[i].withdata)
				mmi_frm_sms_free_data(index);
			else
			{
				mmi_frm_sms_data_check[i].serial++;
				if(mmi_frm_sms_data_check[i].serial==MMI_FRM_SMS_INVALID_NUM)
					mmi_frm_sms_data_check[i].serial=0;
				if(index==MMI_FRM_SMS_ACTION_SIZE)
				{
					/* add one entry with index awaited_index in the mmi_frm_sms_data */
					U8 awaited_index=mmi_frm_sms_add_data_entry();
					/* find an empty entry in the data list and add successfully */
					if(awaited_index!=MMI_FRM_SMS_INVALID_NUM)
					{
						/* Reset the data check bit to zero  */
					        mmi_frm_sms_set_data_check(awaited_index, 0);
					        mmi_frm_sms_free_new_data();
					}
				}
			}
			break;
		}
		i++;
	}
	if(i==mmi_frm_sms_data_check_num)
		mmi_frm_sms_free_data(index);
	if(data!=NULL)
	{
		OslMfree(data);
		data=NULL;
	}
	if(content != NULL)
	{
		OslMfree(content);
		content=NULL;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_new_sms
* DESCRIPTION
*   handle new SMS
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_new_sms (void)
{
	U16 index;

	/*check segment*/
	switch(mmi_frm_sms_check_concatenate_sms(&index))
	{
		case MMI_FRM_SMS_AWAITS:
		{
			mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_new_sms, MMI_FRM_SMS_AWAITS");
			mmi_frm_sms_indicate_sms(index);
		}
		break;
		case MMI_FRM_SMS_INBOX:
		{
			/* when there is a new segment coming, always set the status as unread, in case the previous segments in inbox is read */
			mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_new_sms, MMI_FRM_SMS_INBOX");
			mmi_frm_sms_set_sms_status(MMI_FRM_SMS_NOBOX, index, MMI_FRM_SMS_UNREAD);
			mmi_frm_sms_new_msg_ind(index);
			return;
		}
	}
	
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_new_data
* DESCRIPTION
*   handle new SMS with port number
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_new_data (void)
{
	U8 index;

	/*check segment*/
	switch(mmi_frm_sms_check_concatenate_data(&index))
	{
		case MMI_FRM_SMS_DATA:
		{
			mmi_frm_sms_indicate_data(index);
		}
		break;
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_entry_abort
* DESCRIPTION
*   Entry abort screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_entry_abort (void)
{
	EntryNewScreen(SCR_ID_MSG_PROCESSING, NULL, mmi_frm_sms_entry_abort, NULL);
	ShowCategory8Screen (STR_ABORT_SENDING_SMS, IMG_SMS_ENTRY_SCRN_CAPTION,
							0, 0, 0, 0, STR_SENDIING_SMS_BODY, IMG_GLOBAL_PROGRESS, NULL);

	ClearAllKeyHandler ();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
	if(isInCall())
		SetKeyHandler(HangupAllCalls, KEY_END, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_entry_send
* DESCRIPTION
*   Entry send screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_entry_send (void)
{
	U8 bAbleCancel = FALSE;
	U16 titleId = STR_SENDING_SMS;
	EntryNewScreen(SCR_ID_MSG_SENDING, NULL,mmi_frm_sms_entry_send, NULL);
	/* reset the status because sending SMS in the idle screen background is allowable */
	g_msg_cntx.msg_status = MSG_STATUS_IDLE;
	//ShowCategory8Screen (STR_SENDING_SMS, 0, 0, 0, STR_GLOBAL_ABORT, IMG_SMS_COMMON_NOIMAGE, 
	#ifdef __MMI_MULTI_SIM__
	titleId = STRING_MTPNP_MASTER_SENDING_SMS + MTPNP_PFAL_Get_Channel_SIMID(MTPNP_AD_SMS_CHANNEL);
	#endif
	if(bAbleCancel)
	{
		ShowCategory8Screen (titleId, 0, 0,0, STR_GLOBAL_ABORT, 0, 
							 STR_SENDIING_SMS_BODY, IMG_NEW_SMS_SEND, NULL);
	}
	else
	{
		ShowCategory8Screen (titleId, 0, 0,0, 0, 0, 
							 STR_SENDIING_SMS_BODY, IMG_NEW_SMS_SEND, NULL);
	}
		
	
	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
	if(bAbleCancel)
	{
		SetKeyHandler(mmi_frm_sms_abort_sms, KEY_END, KEY_EVENT_DOWN);
		SetRightSoftkeyFunction (mmi_frm_sms_abort_sms, KEY_EVENT_UP);                            //deleted by yaosq 2006.11.30
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_phb_number
* DESCRIPTION
*   Get phonebook number
*
* PARAMETERS
*  name	IN	name
*  number	IN	number
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_phb_number (U8 name[], U8 number[])
{
	if(pfnUnicodeStrlen ((PS8)number) >  (MAX_DIGITS_SMS-1))
	{
		DisplayPopup ((PU8)GetString (STR_SMS_NUM_LEN_EXCEEDED), IMG_GLOBAL_UNFINISHED, 1, PHB_NOTIFY_TIMEOUT, (U8)ERROR_TONE);
		return;
	}

	memset(g_frm_sms_cntx.mmi_frm_sms_number, 0, (MAX_DIGITS_SMS+1)*ENCODING_LENGTH);
	if(number)
		pfnUnicodeStrcpy((S8*)g_frm_sms_cntx.mmi_frm_sms_number, (PS8)number);
	
	if(mmi_frm_sms_search_phb_type == MMI_FRM_SMS_SC)
	{
		HistoryReplace(SCR_ID_MSG_SC_NUMBER, SCR_ID_MSG_SC_NUMBER, mmi_frm_sms_entry_sc);
		GoBackToHistory(SCR_ID_MSG_SC_NUMBER);
	}
	else
	{
		HistoryReplace(SCR_ID_MSG_NUMBER, SCR_ID_MSG_NUMBER, mmi_frm_sms_entry_number);
		GoBackToHistory(SCR_ID_MSG_NUMBER);
	}	
	// GoBackHistory ();
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_search_phb
* DESCRIPTION
*   Interface to enter phone book
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_search_phb (void)
{
	mmi_phb_frm_enter_send_entry (); 
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_number_empty
* DESCRIPTION
*   LSK handle when number input is empty
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_number_empty (void)
{
	mmi_frm_sms_search_phb_type = MMI_FRM_SMS_DA;
	ChangeLeftSoftkey (STR_SMS_COMMON_SEARCH, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (mmi_frm_sms_search_phb, KEY_EVENT_UP);
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_number_not_empty
* DESCRIPTION
*   LSK handle when number input is not empty
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_number_not_empty (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (mmi_frm_sms_entry_number_done, KEY_EVENT_UP);
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_go_back_from_send
* DESCRIPTION
*   RSK handle in input SC/number screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_go_back_from_send (void)
{
	g_msg_cntx.msg_status = MSG_STATUS_IDLE; 
	mmi_frm_sms_callback_action(NULL, MMI_FRM_SMS_RETURN);
	AlmEnableSPOF();
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_end_key_from_send
* DESCRIPTION
*   END key handle in input SC/number screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_end_key_from_send (void)
{
	g_msg_cntx.msg_status = MSG_STATUS_IDLE;
	mmi_frm_sms_callback_action(NULL, MMI_FRM_SMS_END);
	AlmEnableSPOF();
	ExecCurrEndKeyDownHandler();
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_entry_number
* DESCRIPTION
*   Entry SMS number screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
extern UINT16 TitlePlaceFlag ;
void mmi_frm_sms_entry_number (void)
{
	U8* 	guiBuffer;
	TitlePlaceFlag = 1;
	EntryNewScreen(SCR_ID_MSG_NUMBER, NULL, mmi_frm_sms_entry_number, NULL);
	AlmDisableSPOF(); /* in case schedule power off in sending a message */
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_NUMBER);
	/* keep the status in number editing screen and check the status in idle screeen.
	   in the normal case, the status should be MSG_STATUS_IDLE in idle screen */
	g_msg_cntx.msg_status = MSG_STATUS_INPUT_NUM;
	GetCurrEndKeyDownHandler();

	RegisterInputBoxEmptyFunction (mmi_frm_sms_number_empty);
	RegisterInputBoxNotEmptyFunction (mmi_frm_sms_number_not_empty);

	ShowCategory5Screen (STR_ENTER_PHONE_NUMBER_CAPTION, IMG_SMS_ENTRY_SCRN_CAPTION,
						  STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE,
						  STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE, 
						  INPUT_TYPE_PHONE_NUMBER/*|INPUT_TYPE_PLUS_CHARACTER_HANDLING*/,
						  (U8*) g_frm_sms_cntx.mmi_frm_sms_number, MAX_DIGITS_SMS, guiBuffer); 

	SetKeyHandler(mmi_frm_sms_entry_number_done, KEY_SEND, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction (mmi_frm_sms_go_back_from_send, KEY_EVENT_UP);
	SetKeyHandler(mmi_frm_sms_end_key_from_send, KEY_END, KEY_EVENT_DOWN);
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_entry_sc_done
* DESCRIPTION
*   save entered SC number
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_entry_sc_done (void)
{
	S16	errorCode=0;
	S8 nvramActiveProfileIndex=0;
	msg_profile_edit_struct* profileSC=OslMalloc(sizeof(msg_profile_edit_struct));
	
	ReadValue(NVRAM_SMS_ACTIVE_PROFILE_INDEX, &nvramActiveProfileIndex, DS_BYTE, &errorCode);
	profileSC->profileIndex=nvramActiveProfileIndex;
	pfnUnicodeStrcpy((S8*)profileSC->scAddress, (S8*)g_frm_sms_cntx.mmi_frm_sms_number);
	mmi_frm_sms_set_sc_addr_req(profileSC);
	OslMfree(profileSC);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_sc_empty
* DESCRIPTION
*   LSK handle when sc input is empty
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_sc_empty (void)
{
	mmi_frm_sms_search_phb_type = MMI_FRM_SMS_SC;
	ChangeLeftSoftkey (STR_SMS_COMMON_SEARCH, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (mmi_frm_sms_search_phb, KEY_EVENT_UP);
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_sc_not_empty
* DESCRIPTION
*   LSK handle when sc input is not empty
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_sc_not_empty (void)
{
	ChangeLeftSoftkey (STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE);
	SetLeftSoftkeyFunction (mmi_frm_sms_entry_sc_done, KEY_EVENT_UP);
}



/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_entry_sc
* DESCRIPTION
*   Entry input SC screen
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_entry_sc (void)
{
	U8* 	guiBuffer;
	
	EntryNewScreen(SCR_ID_MSG_SC_NUMBER, NULL, mmi_frm_sms_entry_sc, NULL);
	AlmDisableSPOF(); /* in case schedule power off in sending a message */
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MSG_SC_NUMBER);
	/* keep the status in number editing screen and check the status in idle screeen.
	   in the normal case, the status should be MSG_STATUS_IDLE in idle screen */
	g_msg_cntx.msg_status = MSG_STATUS_INPUT_NUM;
	GetCurrEndKeyDownHandler();

	RegisterInputBoxEmptyFunction (mmi_frm_sms_sc_empty);
	RegisterInputBoxNotEmptyFunction (mmi_frm_sms_sc_not_empty);

	ShowCategory5Screen (STR_SC_EMPTY_CAPTION_ID, IMG_SMS_ENTRY_SCRN_CAPTION,
						  STR_GLOBAL_OK, IMG_SMS_COMMON_NOIMAGE,
						  STR_GLOBAL_BACK, IMG_SMS_COMMON_NOIMAGE, 
						  INPUT_TYPE_PHONE_NUMBER/*|INPUT_TYPE_PLUS_CHARACTER_HANDLING*/,
						  (U8*) g_frm_sms_cntx.mmi_frm_sms_number, MAX_DIGITS_SMS, guiBuffer); 
	SetCategory5RightSoftkeyFunction(mmi_frm_sms_go_back_from_send, KEY_EVENT_UP);
	SetKeyHandler(mmi_frm_sms_end_key_from_send, KEY_END, KEY_EVENT_DOWN);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_write_action
* DESCRIPTION
*  mmi_frm_sms_write_action
*
* PARAMETERS
*  a  action		action
* RETURNS
*  none
* GLOBALS AFFECTED
*  mmi_frm_sms_action, mmi_frm_sms_action_tail, mmi_frm_sms_action_num
*****************************************************************************/
U8 mmi_frm_sms_write_action (PsFuncPtrU16 callback, module_type mod_src, void* data, PsFuncPtr function)
{
	mmi_frm_sms_action_struct action;
	action.mod_src = mod_src;
	action.data = data;
	action.callback=callback;
	action.action=function;

	if(mmi_frm_sms_action_num == MMI_FRM_SMS_ACTION_SIZE)
	{
		mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_write_action:: mmi_frm_sms_action_num %d", (int) mmi_frm_sms_action_num);
		return FALSE;
	}

	MMI_ASSERT((mmi_frm_sms_action_tail < MMI_FRM_SMS_ACTION_SIZE));

	memcpy(&mmi_frm_sms_action[mmi_frm_sms_action_tail], (S8*)&action,
		sizeof(mmi_frm_sms_action_struct));

	if(++mmi_frm_sms_action_tail==MMI_FRM_SMS_ACTION_SIZE)
		mmi_frm_sms_action_tail=0;

	mmi_frm_sms_action_num++;

	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_write_action:: mmi_frm_sms_action_curr %d, callback is: 0x%x", (int) mmi_frm_sms_action_curr, callback);

	if(mmi_frm_sms_action_curr==MMI_FRM_SMS_INVALID_NUM)
		mmi_frm_sms_read_action();

	return TRUE;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_read_action
* DESCRIPTION
*  mmi_frm_sms_read_action
*
* PARAMETERS
*  a  action		action
* RETURNS
*  none
* GLOBALS AFFECTED
*  mmi_frm_sms_action, mmi_frm_sms_action_head, mmi_frm_sms_action_num
*****************************************************************************/
U8 mmi_frm_sms_read_action (void)
{	
	PsFuncPtr currFuncPtr = mmi_frm_sms_action[mmi_frm_sms_action_head].action;
	void* currFuncData = mmi_frm_sms_action[mmi_frm_sms_action_head].data;
	
	if(mmi_frm_sms_action_num == 0)
	{
		mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_read_action:: mmi_frm_sms_action_num %d", (int) mmi_frm_sms_action_num);	
		return FALSE;
	}

	MMI_ASSERT((mmi_frm_sms_action_head < MMI_FRM_SMS_ACTION_SIZE));

	mmi_frm_sms_action_curr = mmi_frm_sms_action_head;
	mmi_frm_sms_action_num--;

	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_read_action:: mmi_frm_sms_action_curr %d", (int) mmi_frm_sms_action_curr);

	if(currFuncPtr!= NULL)
		(*currFuncPtr)(currFuncData);
	

	if(++mmi_frm_sms_action_head == MMI_FRM_SMS_ACTION_SIZE)
		mmi_frm_sms_action_head = 0;
	
	return TRUE;
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_callback_action
* DESCRIPTION
*  mmi_frm_sms_callback_action
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_callback_action (void* data, U16 result)
{
	PsFuncPtrU16 callback = mmi_frm_sms_action[mmi_frm_sms_action_curr].callback;
	module_type mod_src = mmi_frm_sms_action[mmi_frm_sms_action_curr].mod_src;

	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_callback_action:: mmi_frm_sms_action_curr %d, callback is: 0x%x", (int) mmi_frm_sms_action_curr, callback);
		//qiff modify for GS PROJ 11215 09-02-18
	if((callback!=NULL)&& (mmi_frm_sms_action_curr != MMI_FRM_SMS_INVALID_NUM))
	{
		MMI_ASSERT((mmi_frm_sms_action_curr <MMI_FRM_SMS_ACTION_SIZE));
		callback(data, (module_type)mod_src, result);
	}
	mmi_frm_sms_action_curr=MMI_FRM_SMS_INVALID_NUM;

	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_callback_action:: mmi_frm_sms_action_curr %d", (int) mmi_frm_sms_action_curr);
	
	mmi_frm_sms_read_action ();
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_check_action_pending
* DESCRIPTION
*  check if there is a pending action in SMS framework
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
U8 mmi_frm_sms_check_action_pending(void)
{
	mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_check_action_pending:: mmi_frm_sms_action_curr %d", (int) mmi_frm_sms_action_curr);
	MMI_ASSERT((mmi_frm_sms_action_curr == MMI_FRM_SMS_INVALID_NUM)||(mmi_frm_sms_action_curr <MMI_FRM_SMS_ACTION_SIZE));
	
	if (mmi_frm_sms_action_curr!=MMI_FRM_SMS_INVALID_NUM)
		return TRUE;
	else
		return FALSE;
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_delete_action_pending
* DESCRIPTION
*  delete the action from queue if SMS application is re-entering
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_delete_action_pending(void)
{
	if (IsScreenPresent(SCR_ID_MSG_SENDING) || IsScreenPresent(SCR_ID_MSG_PROCESSING))
		mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_delete_action_pending DO NOTHING");
	else
	{
		mmi_trace (g_sw_SMS, "MMI_SMS: mmi_frm_sms_delete_action_pending DELETE ACTION QUEUE");
		/* reset the status after deleting the action queue */
		g_msg_cntx.msg_status = MSG_STATUS_IDLE;
		mmi_frm_sms_callback_action(NULL, MMI_FRM_SMS_END);
	}
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_delete_screen_history
* DESCRIPTION
*  delete the screen history that is not set to g_msg_cntx.MessagesScrnIdDelUptoHistoryNodes
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_delete_screen_history(void)
{
	DeleteScreenIfPresent(SCR_ID_MSG_PROCESSING);
	DeleteScreenIfPresent(SCR_ID_MSG_SENDING);
	DeleteScreenIfPresent(SCR_ID_MSG_NUMBER);
	DeleteScreenIfPresent(SCR_ID_MSG_SC_NUMBER);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_check_complete
* DESCRIPTION
*   Check message completeneww
*
* PARAMETERS
*  type	IN	message type
*  index	IN	message index
* RETURNS
*  TRUE: complete; FALSE: not
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_frm_sms_check_complete(U8 type, U16 index)
{
	return mmi_frm_sms_check_sms_complete(type, index);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_status
* DESCRIPTION
*   Get message status
*
* PARAMETERS
*  type	IN	message type
*  index	IN	message index
* RETURNS
*  status
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_frm_sms_get_status(U8 type, U16 index)
{
	return mmi_frm_sms_get_sms_status(type, index);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_dcs
* DESCRIPTION
*   Get message DCS
*
* PARAMETERS
*  type	IN	message type
*  index	IN	message index
* RETURNS
*  dcs
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8 mmi_frm_sms_get_dcs (U8 type, U16 index)
{
	return mmi_frm_sms_get_sms_dcs (type, index);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_address
* DESCRIPTION
*   Get message address
*
* PARAMETERS
*  type	IN	message type
*  index	IN	message index
* RETURNS
*  address
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8* mmi_frm_sms_get_address (U8 type, U16 index)
{
	return mmi_frm_sms_get_sms_address(type, index);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_timestamp
* DESCRIPTION
*   Get message timestamp
*
* PARAMETERS
*  type	IN	message type
*  index	IN	message index
* RETURNS
*  timestamp
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U8* mmi_frm_sms_get_timestamp (U8 type, U16 index)
{
	return mmi_frm_sms_get_sms_timestamp(type, index);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_list_index
* DESCRIPTION
*   Get SMS type/index
*
* PARAMETERS
*  type	OUT	message type
*  index	OUT	message index
*  msgbox_index	IN	message index
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_list_index(U8* type, U16* index, U16 msgbox_index)
{
	mmi_frm_sms_get_sms_list_index(type, index, msgbox_index);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_abort_sms
* DESCRIPTION
*   Abort MO SMS
*
* PARAMETERS
*   none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_abort_sms (void)
{
	mmi_frm_sms_abort_sms_req(NULL);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_send_sms
* DESCRIPTION
*   Send SMS
*
* PARAMETERS
*  callback	IN	callback after action
*  mod_src	IN	callback module
*  sendData	IN	Send data
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
#ifdef __MMI_MULTI_SIM__
extern void mmi_frm_msg_set_send_data(PsFuncPtrU16 callback, module_type mod_src, mmi_frm_sms_send_struct *sendData);
extern void mmi_frm_msg_entry_Select_option_screen(void);
#endif

void mmi_frm_sms_send_sms (PsFuncPtrU16 callback, module_type mod_src, mmi_frm_sms_send_struct* sendData)
{
	mmi_frm_sms_send_struct* data=OslMalloc(sizeof(mmi_frm_sms_send_struct));
	memcpy((S8*)data, (S8*)sendData, sizeof(mmi_frm_sms_send_struct));
	mmi_trace(g_sw_SMS,"MMI_SMS: mmi_frm_sms_send_sms, data->sendrequire is 0x%x",data->sendrequire);
	#ifdef __MMI_MULTI_SIM__
	if (sendData->sendrequire&MMI_FRM_SMS_DISP_SIM_OPT ||g_msg_cntx.needSelectSIM)
	{	
		mmi_frm_msg_set_send_data(callback, mod_src, data);
		mmi_frm_msg_entry_Select_option_screen();
	}
	else
	#endif
	{
		mmi_frm_sms_write_action(callback, mod_src, data, mmi_frm_sms_pre_send_sms);
	}
}

#ifdef __MMI_MULTI_SIM__
U8 mmi_frm_msg_get_simid_by_l4_index(U16 l4_index)
{
	return l4_index/( PHONE_SMS_ENTRY_COUNT/MMI_SIM_NUMBER + SIM_SMS_ENTRY_COUNT );
}

/*****************************************************************************
 * FUNCTION
 *  mmi_msg_is_from_master_sim
 * DESCRIPTION
 *  whether current sms is from master sim
 * PARAMETERS
 *  list_index         [IN]
 * RETURNS
 *  BOOL
 *****************************************************************************/
BOOL mmi_frm_msg_get_simid_by_list_index(U16 list_index)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U16 l4_index;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	l4_index = mmi_frm_sms_msg_box[list_index].startindex;	

	return mmi_frm_msg_get_simid_by_l4_index(l4_index);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_frm_sms_delete_sms
 * DESCRIPTION
 *  Delete SMS
 * PARAMETERS
 *  side            [IN]        witch side to delete, master or slave
 *  callback        [IN]        Callback after action
 *  mod_src         [IN]        Callback module
 *  type            [IN]        Message type
 *  index           [IN]        Message index
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_frm_sms_dm_delete_sms(U8 side, PsFuncPtrU16 callback, module_type mod_src, U16 type, U16 index)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U8 simIndex;
	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	// TODO: need check the response function to delete other box if the show style is mixed
	for(simIndex = 0; simIndex < MMI_SIM_NUMBER; simIndex++)
	{
		if(side&(DM_MASTER_BOX<<simIndex))
		{
			U8 *data = OslMalloc(sizeof(U8) * 480);
			mmi_trace(g_sw_SMS, "MMI_SMS: mmi_frm_sms_dm_delete_sms, SIM%d", simIndex);

			memset((S8*)data, 0, sizeof(U8) * 480);

			/* delete master sms */
			if (mmi_frm_sms_get_multi_sms_bitmap(type, index, data, simIndex) == FALSE)
			{
				callback(NULL, MOD_MMI, MMI_FRM_SMS_ERROR);
			}
			else
			{
				mmi_frm_sms_set_active_simid(simIndex);
				mmi_frm_sms_write_action(callback, mod_src, data, mmi_frm_sms_delete_sms_req);	
			}
			break;
		}
	}
}
#endif /*__MMI_MULTI_SIM__*/
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_read_sms
* DESCRIPTION
*   Read SMS
*
* PARAMETERS
*  callback	IN	callback after action
*  mod_src	IN	callback module
*  type	IN	message type
*  index	IN	message index
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_read_sms (PsFuncPtrU16 callback, module_type mod_src, U8 type, U16 index)
{
	U16* data=OslMalloc(sizeof(U16)*(g_frm_sms_cntx.mmi_frm_sms_msg_seg_size+1));
	mmi_frm_sms_get_sms_index((mmi_frm_sms_msgbox_enum)type, index, data);
	mmi_frm_sms_set_sms_status(type, index, MMI_FRM_SMS_INBOX);
	data[g_frm_sms_cntx.mmi_frm_sms_msg_seg_size]=(U16)mmi_frm_sms_get_sms_size(type, index);
	mmi_frm_sms_write_action(callback, mod_src, data, mmi_frm_sms_read_sms_req);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_sms
* DESCRIPTION
*   Get SMS, for AT sync indication
*
* PARAMETERS
*  callback	IN	callback after action
*  mod_src	IN	callback module
*  index	IN	message index
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_sms (PsFuncPtrU16 callback, module_type mod_src, U16 index)
{
	U16* data=OslMalloc(sizeof(U16));
	*data = index;
	mmi_frm_sms_write_action(callback, mod_src, data, mmi_frm_sms_get_sms_req);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_save_sms
* DESCRIPTION
*   Save SMS
*
* PARAMETERS
*  callback	IN	callback after action
*  mod_src	IN	callback module
*  sendData	IN	Save data
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_save_sms (PsFuncPtrU16 callback, module_type mod_src, mmi_frm_sms_send_struct* sendData)
{
	mmi_frm_sms_send_struct* data=OslMalloc(sizeof(mmi_frm_sms_send_struct));
	memcpy((S8*)data, (S8*)sendData, sizeof(mmi_frm_sms_send_struct));
	#ifdef __MMI_MULTI_SIM__
	if (!(sendData->sendrequire&MMI_FRM_SMS_DISP_SIM_OPT))
	#endif
	{
		mmi_frm_sms_write_action(callback, mod_src, data, mmi_frm_sms_save_sms_req);
	}
	#ifdef __MMI_MULTI_SIM__
	else
	{	
		mmi_frm_msg_set_send_data(callback, mod_src, data);
		mmi_msg_entry_select_save_option_screen();
	}
	#endif
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_delete_sms
* DESCRIPTION
*   Delete SMS
*
* PARAMETERS
*  callback	IN	callback after action
*  mod_src	IN	callback module
*  type	IN	message type
*  index	IN	message index
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_delete_sms (PsFuncPtrU16 callback, module_type mod_src, U8 type, U16 index)
{
	U8* data=OslMalloc(sizeof(U8)*480);
	memset((S8*)data, 0, sizeof(U8)*480);
#ifndef __MMI_MULTI_SIM__
	if(mmi_frm_sms_get_sms_bitmap((mmi_frm_sms_msgbox_enum)type, index, data)==FALSE)
	{
		callback(NULL, MOD_MMI, MMI_FRM_SMS_ERROR);
		OslMfree(data);
	}
	else
	mmi_frm_sms_write_action(callback, mod_src, data, mmi_frm_sms_delete_sms_req);
#else
	{
		U8 nSimID = mmi_frm_sms_record_get_sim_id(type, index);
		if (mmi_frm_sms_get_multi_sms_bitmap(type, index, data, nSimID) == FALSE)
		{
			callback(NULL, MOD_MMI, MMI_FRM_SMS_ERROR);
			OslMfree(data);
		}
		else
		{
			mmi_frm_sms_set_active_simid(nSimID);
			mmi_frm_sms_write_action(callback, mod_src, data, mmi_frm_sms_delete_sms_req);
		}
	}
#endif
}

void mmi_frm_sms_delete_chat_sms (PsFuncPtrU16 callback, module_type mod_src, U8 type, U16 index)
{
	U8* data=OslMalloc(sizeof(U8)*480);
	memset((S8*)data, 0, sizeof(U8)*480);
	mmi_frm_sms_write_action(callback, mod_src, data, mmi_frm_sms_delete_sms_req);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_reg_port
* DESCRIPTION
*   Register SMS port
*
* PARAMETERS
*  callback	IN	callback after action
*  mod_src	IN	callback module
*  port_num	IN	port number
*  enable	IN	enable/disable
*  withdata	IN	indicate with data
*  regcallback	IN	regiter callback module
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_reg_port (PsFuncPtrU16 callback, module_type mod_src, U16 port_num, BOOL enable,
								BOOL withdata, PsFuncPtrU16 regcallback)
{
	mmi_sms_reg_port_num_req_struct* data=OslMalloc(sizeof(mmi_sms_reg_port_num_req_struct));
	data->mod_id = mod_src;
	data->port_num = port_num;
	data->enable = enable;
	if(enable)
	{
		mmi_frm_sms_data_check[mmi_frm_sms_data_check_num].desport=port_num;
		mmi_frm_sms_data_check[mmi_frm_sms_data_check_num].module=mod_src;
		mmi_frm_sms_data_check[mmi_frm_sms_data_check_num].serial=0;
		mmi_frm_sms_data_check[mmi_frm_sms_data_check_num].withdata=withdata;
		mmi_frm_sms_data_check[mmi_frm_sms_data_check_num].callback=regcallback;
		mmi_frm_sms_data_check_num++;
	}
	else
	{
		U8 i=mmi_frm_sms_data_check_num;
		while(i>0)
		{
			i--;
			if((mmi_frm_sms_data_check[i].desport==port_num)&&(mmi_frm_sms_data_check[i].module==mod_src))
			{
				U8 j=i+1;
				for(j=(i+1); j<mmi_frm_sms_data_check_num; j++)
					memcpy((S8*)&mmi_frm_sms_data_check[j-1], (S8*)&mmi_frm_sms_data_check[j], sizeof(mmi_frm_sms_data_check_struct));
				mmi_frm_sms_data_check_num--;
				break;
			}
		}
	}
	mmi_frm_sms_write_action(callback, mod_src, data, mmi_frm_sms_reg_port_req);
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_profile_active
* DESCRIPTION
*   Set active profile
*
* PARAMETERS
*  callback	IN	callback after action
*  mod_src	IN	callback module
*  index	IN	active profile index
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_set_profile_active (PsFuncPtrU16 callback, module_type mod_src, U8* index)
{
	S16	errorCode=0;
	WriteValue(NVRAM_SMS_ACTIVE_PROFILE_INDEX, index, DS_BYTE, &errorCode);		
	callback(index, mod_src, MMI_FRM_SMS_OK);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_profile_list
* DESCRIPTION
*   Get profile list, as well as active profile index
*
* PARAMETERS
*  callback	IN	callback after action
*  mod_src	IN	callback module
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_profile_list (PsFuncPtrU16 callback, module_type mod_src, U8 nSimID)
{
	U8 *pSimID = (U8*)OslMalloc(sizeof(U8));
	*pSimID = nSimID;
	mmi_frm_sms_write_action(callback, mod_src, (void*)pSimID, mmi_frm_sms_get_profile_num_req);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_sc_addr
* DESCRIPTION
*   Get SC address
*
* PARAMETERS
*  callback	IN	callback after action
*  mod_src	IN	callback module
*  index	IN	profile index
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_sc_addr (PsFuncPtrU16 callback, module_type mod_src, U8 index)
{
	U8* data=OslMalloc(sizeof(U8));
	memset(data, index, sizeof(U8));
	mmi_frm_sms_write_action(callback, mod_src, data, mmi_frm_sms_get_sc_addr_req);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_sc_addr
* DESCRIPTION
*   handle new SMS
*
* PARAMETERS
*  callback	IN	callback after action
*  mod_src	IN	callback module
*  index	IN	profile index
*  scAddr	IN	SC address
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_set_sc_addr (PsFuncPtrU16 callback, module_type mod_src, U8 index, S8 scAddr[])
{
	msg_profile_edit_struct* data=OslMalloc(sizeof(msg_profile_edit_struct));
	memcpy(data->scAddress, scAddr, MAX_CC_ADDR_LEN*ENCODING_LENGTH);
	memset(data->scAddress + MAX_CC_ADDR_LEN*ENCODING_LENGTH, 0, ENCODING_LENGTH);
	data->profileIndex = index;
	mmi_frm_sms_write_action(callback, mod_src, data, mmi_frm_sms_set_sc_addr_req);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_profile_detail
* DESCRIPTION
*   Get profile detail info
*
* PARAMETERS
*  callback	IN	callback after action
*  mod_src	IN	callback module
*  index	IN	profile index
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_profile_detail (PsFuncPtrU16 callback, module_type mod_src, U8 index)
{
	U8* data=OslMalloc(sizeof(U8));
	memset(data, index, sizeof(U8));
	mmi_frm_sms_write_action(callback, mod_src, data, mmi_frm_sms_get_profile_detail_req);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_profile_detail
* DESCRIPTION
*   Set profile detail info
*
* PARAMETERS
*  callback	IN	callback after action
*  mod_src	IN	callback module
*  profile	IN	profile content
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_set_profile_detail (PsFuncPtrU16 callback, module_type mod_src, void* profile)
{
	msg_profile_edit_struct* data=OslMalloc(sizeof(msg_profile_edit_struct));
	memcpy(data, profile, sizeof(msg_profile_edit_struct));
	mmi_frm_sms_write_action(callback, mod_src, data, mmi_frm_sms_set_profile_detail_req);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_common_settings
* DESCRIPTION
*   Get common setting
*
* PARAMETERS
*  callback	IN	callback after action
*  mod_src	IN	callback module
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_common_settings (PsFuncPtrU16 callback, module_type mod_src)
{
	mmi_frm_sms_write_action(callback, mod_src, NULL, mmi_frm_sms_get_common_settings_req);
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_common_settings
* DESCRIPTION
*   Set common setting
*
* PARAMETERS
*  callback	IN	callback after action
*  mod_src	IN	callback module
*  setting	IN	settings
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_set_common_settings (PsFuncPtrU16 callback, module_type mod_src, U8* setting)
{
	U8* data=OslMalloc(sizeof(U8)*2);
	memcpy(data, setting, sizeof(U8)*2);
	mmi_frm_sms_write_action(callback, mod_src, data, mmi_frm_sms_set_common_settings_req);
}

#ifdef __MMI_MESSAGES_PREFER_STORAGE_MEMORY_STATUS__
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_memory_status
* DESCRIPTION
*   Get memory status
*
* PARAMETERS
*  callback	IN	callback after action
*  mod_src	IN	callback module
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_memory_status (PsFuncPtrU16 callback, module_type mod_src)
{
	mmi_frm_sms_write_action(callback, mod_src, NULL, mmi_frm_sms_get_memory_status_req);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_preferred_storage
* DESCRIPTION
*   Get preferred storage
*
* PARAMETERS
*  callback	IN	callback after action
*  mod_src	IN	callback module
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_preferred_storage (PsFuncPtrU16 callback, module_type mod_src)
{
	mmi_frm_sms_write_action(callback, mod_src, NULL, mmi_frm_sms_get_preferred_storage_req);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_preferred_storage
* DESCRIPTION
*   Set preferred storage
*
* PARAMETERS
*  callback	IN	callback after action
*  mod_src	IN	callback module
*  storage	IN	storage
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_set_preferred_storage (PsFuncPtrU16 callback, module_type mod_src, U8 storage)
{
	U8* data=OslMalloc(sizeof(U8));
	memset(data, storage, sizeof(U8));
	mmi_frm_sms_write_action(callback, mod_src, data, mmi_frm_sms_set_preferred_storage_req);
}
#endif

#ifdef __MMI_GPRS_FEATURES__
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_preferred_bearer
* DESCRIPTION
*   Get preferred bearer
*
* PARAMETERS
*  callback	IN	callback after action
*  mod_src	IN	callback module
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_preferred_bearer (PsFuncPtrU16 callback, module_type mod_src)
{
	mmi_frm_sms_write_action(callback, mod_src, NULL, mmi_frm_sms_get_preferred_bearer_req);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_preferred_bearer
* DESCRIPTION
*   Set preferred bearer
*
* PARAMETERS
*  callback	IN	callback after action
*  mod_src	IN	callback module
*  bearer	IN	bearer
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_set_preferred_bearer (PsFuncPtrU16 callback, module_type mod_src, U8 bearer)
{
	U8* data=OslMalloc(sizeof(U8));
	memset(data, bearer, sizeof(U8));
	mmi_frm_sms_write_action(callback, mod_src, data, mmi_frm_sms_set_preferred_bearer_req);
}
#endif

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_mailbox_info
* DESCRIPTION
*   Get mailbox info
*
* PARAMETERS
*  callback	IN	callback after action
*  mod_src	IN	callback module
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_get_mailbox_info (PsFuncPtrU16 callback, module_type mod_src)
{
	mmi_frm_sms_write_action(callback, mod_src, NULL, mmi_frm_sms_get_mailbox_info_req);
}

/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_set_mailbox_info
* DESCRIPTION
*   Set mailbox info
*
* PARAMETERS
*  callback	IN	callback after action
*  mod_src	IN	callback module
*  mailbox	IN	mailbox info to be set
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_set_mailbox_info (PsFuncPtrU16 callback, module_type mod_src, void* mailbox)
{
	msg_mailbox_edit_struct* data=OslMalloc(sizeof(msg_mailbox_edit_struct));
	memcpy(data, mailbox, sizeof(msg_mailbox_edit_struct));
	mmi_frm_sms_write_action(callback, mod_src, data, mmi_frm_sms_set_mailbox_info_req);
}

#if defined (__MMI_MESSAGES_COPY__)||defined (__MMI_MESSAGE_DIFFERENTIATE_STORAGE__)
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_get_storage
* DESCRIPTION
*  get SMS storage type
*
* PARAMETERS
*  a  type :	mmi_frm_sms_app_msgbox_enum
*  b  index:	message list index
* RETURNS
*  smsal_storage_enum (SMSAL_SM or SMSAL_ME)
* GLOBALS AFFECTED
*  none
*****************************************************************************/
extern U8 mmi_frm_sms_get_sms_storage(U8 type, U16 index);
U8 mmi_frm_sms_get_storage(U8 type, U16 index)
{
	return mmi_frm_sms_get_sms_storage(type,index);
}
#ifdef __MMI_MESSAGES_COPY__
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_copy_abort
* DESCRIPTION
*  abort SMS copy action
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*  none
*****************************************************************************/
void mmi_frm_sms_copy_abort(void)
{
	mmi_frm_sms_copy_sms_abort();
	return;
}
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_copy_sms
* DESCRIPTION
*  copy SMS
*
* PARAMETERS
*			callback(void* data, module_type mod_src, U16 result);
*			mod_src
*			type			:	mmi_frm_sms_app_msgbox_enum, (only UNREAD,INBOX,OUTBOX,DRAFTS are valid)
*			index			:	message list index, MMI_FRM_SMS_INVALID_INDEX means ALL
*			action		:	smsal_copy_action_enum, copy or move
*			dst_storage	:	smsal_storage_enum (SMSAL_SM or SMSAL_ME)
*
* RETURNS
*  none
* GLOBALS AFFECTED
*  duplicate SMS
*****************************************************************************/
void mmi_frm_sms_copy_sms(PsFuncPtrU16 callback, module_type mod_src, U8 type, U16 index, U8 action,U8 dst_storage)
{
	if(index != MMI_FRM_SMS_INVALID_INDEX)
	{
		g_frm_sms_copy_info.total_msg_number = 1;
		g_frm_sms_copy_info.present_msg_index = index;
	}
	else
	{
		g_frm_sms_copy_info.total_msg_number = mmi_frm_sms_get_sms_list_size (type);
		g_frm_sms_copy_info.present_msg_index = 0; // first element of list
	}
	g_frm_sms_copy_info.handled_msg_number = 0; 
	g_frm_sms_copy_info.copy_msg_number = 0;
	g_frm_sms_copy_info.msg_list_type = type;
	g_frm_sms_copy_info.present_L4_index = MMI_FRM_SMS_INVALID_INDEX;
	g_frm_sms_copy_info.dst_storage = dst_storage;
	g_frm_sms_copy_info.dst_msg_index = MMI_FRM_SMS_INVALID_INDEX;
	g_frm_sms_copy_info.action = action;	
	g_frm_sms_copy_info.is_aborted = FALSE;
	g_frm_sms_copy_info.is_memory_full = FALSE;

	mmi_frm_sms_write_action(callback, mod_src, NULL, mmi_frm_sms_pre_copy_sms_req);
	
}
#endif  // __MMI_MESSAGES_COPY__
#endif
/*****************************************************************************
* FUNCTION
*  mmi_frm_sms_init
* DESCRIPTION
*   Init framework SMS: protocol event handler and data init
*
* PARAMETERS
*  none
* RETURNS
*  none
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_frm_sms_init (void)
{
	U8 i;
	for(i = 0; i < MMI_SIM_NUMBER; i ++)
	{
		g_frm_sms_cntx.mmi_frm_sms_ready[i]=FALSE;
		#ifdef __MMI_MESSAGES_SINGLE_DELIVERY_REPORT__ 
		g_frm_sms_cntx.mmi_frm_sms_delivery_report_setting[i] = FALSE;
		#endif
	}
	g_frm_sms_cntx.mmi_frm_sms_msg_box_size=mmi_msg_get_msgbox_size();
	g_frm_sms_cntx.mmi_frm_sms_msg_seg_size=mmi_msg_get_seg_num();

#ifdef __MMI_MESSAGES_COPY__
	g_frm_sms_copy_info.is_aborted = FALSE;
#endif
	/* init msgbox in case of receiving class 0 message before startup_begin_ind */
	#ifndef __MMI_MULTI_SIM__
    	mmi_frm_sms_init_msgbox();
	#else 
	if (!MTPNP_AD_Msgbox_Is_Init())
	{
		mmi_frm_sms_init_msgbox();
		MTPNP_AD_Msgbox_HasInit();
	}
	#endif
	mmi_frm_sms_set_protocol_event_handler();
}

#endif //__MMI_MESSAGES_VERSION_3__

#endif

