/******************************************************************************/
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    adp_CallLog_Events.c                                                    */
/*                                                                            */
/* Description:                                                               */
/*      transform message   .                                  */
/*     @wug  2006-10-23.                                                       */
/******************************************************************************/
//  implementation of the Call Mgmt Incoming Handling.
//
//////////////////////////////////////////////////////////////////////
 

#ifdef MMI_ON_WIN32
//#include "stdafx.h"
#include "string.h"
#include "stdlib.h"
#include "calllog_file_opration.h"
#include "stdio.h"
#include "l4dr1.h"  //luanyy add
//#include "callmgmtstruct.h"
//#include "memallocator.h"
#endif
#include "mmi_features.h"
#include "stdc.h"
#include "globalconstants.h"
#include "globaldefs.h"
#include "debuginitdef.h"
#include "wgui.h"
#include "globalstruct.h"
#include "mmi_data_types.h"
#include "wgui_categories.h"
#include "historygprot.h"
#include "idleappprot.h"
#include "custdatares.h"
#include "protocolevents.h"
#include "eventsgprot.h"
#include "datetimetype.h"
#include "thrdosinterface.h"
#include "osthreadinterfacegprot.h"
#include "custmenures.h"
#include "miscfunctions.h"
#include "phonebooktypes.h"
#include "phonebookgprot.h"
//jl #include "maintaskgexdcl.h"
#include "queuegprot.h"
#include "taskinit.h"
#include "voicememodef.h"
#include "voicememotype.h"
#include "voicememoprot.h"
#include "settingprofile.h"
#include "gpioinc.h"
#include "alarmdef.h"
#include "calllog_file_opration.h"
#include "callhistoryenum.h"
#include "mmi_trace.h"
//zhoumn added for call cost @2006/11/26>>>start
#include "nvram_user_defs.h"
#include "nvramprot.h"
#include "adp_events.h"
#include "nvram_data_items.h"
//zhoumn added for call cost @2006/11/26<<<end
#ifdef MMI_ON_HARDWARE_P
   #include "mmi_msg_struct.h"
   #include "l4dr.h"
   #include "custom_util.h"
#endif

#define CALLLOG_MAX_RECORD 20

#ifdef __ADP_MULTI_SIM__
ADP_CHIS_CONTEXT_T g_CHISContext[ADP_SIM_NUMBER];
#if 0
=
{
	{ADP_SIM_1, 1, },
	{ADP_SIM_2, 2, },
};
#endif
ADP_CHIS_CONTEXT_T *g_pCHISContext = &g_CHISContext[0];
#define ADP_CHIS_MSG_SRC_MOD  adp_SimId2IlmMod(g_pCHISContext->sim_id)
#else
#define ADP_CHIS_MSG_SRC_MOD MOD_L4C
#endif


typedef struct
{
	LOCAL_PARA_HDR
	l4c_result_struct	result;
	kal_uint8	no_list;
	kal_uint8	type;
	kal_uint8	more_data;
	l4c_phb_ln_entry_struct	list[CALLLOG_MAX_RECORD*ADP_SIM_NUMBER];
} mmi_adp_calllog_data_struct;

typedef struct
{
	kal_uint16	nDialedcall;
	kal_uint16	nRejectedcall;
	kal_uint16	nMissedcall;
	kal_uint16	nReceivedcall;
} adp_chis_num_struct;

mmi_adp_calllog_data_struct gStruCallogData;
//mmi_phb_get_last_number_rsp_struct gStruCallogData;

static void Calllog_Init_context(void)
{
#ifdef __ADP_MULTI_SIM__
	static BOOL initFlag = FALSE;
	if(FALSE == initFlag)
	{
		U8 i;
		for(i = 0; i < ADP_SIM_NUMBER; i++)
		{
			g_CHISContext[i].sim_id = i;
			g_CHISContext[i].nRecordId = 1+i;
		}
		initFlag = TRUE;
	}
#endif
	return;
}

static S32 Calllog_Read_Nvram(UINT8 type, mmi_adp_calllog_data_struct* CallogData)
{
	INT32   ret = 0;
  	UINT16 error = 0;
	
#ifdef __ADP_MULTI_SIM__
	switch (type)                   
	{
		case PHB_LND:
			mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Read_Data***PHB_LND*****  ");
			ret = ReadRecord(NVRAM_DIALEDCALL_NVRAM_LID, g_pCHISContext->nRecordId, (void *)CallogData, sizeof(mmi_adp_calllog_data_struct), &error);
			break;

		case PHB_LNM:
			mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Read_Data***PHB_LNM*****  ");
			ret = ReadRecord(NVRAM_MISSEDCALL_NVRAM_LID, g_pCHISContext->nRecordId, (void *)CallogData, sizeof(mmi_adp_calllog_data_struct), &error);
			break;

		case PHB_LNR:
			mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Read_Data***PHB_LNR*****  ");
			ret = ReadRecord(NVRAM_RECVDCALL_NVRAM_LID, g_pCHISContext->nRecordId, (void *)CallogData, sizeof(mmi_adp_calllog_data_struct), &error);
			break;
		case PHB_LNRJ:
	  		mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Read_Data***PHB_LNRJ*****  ");
			ret = ReadRecord(NVRAM_REJECTEDCALL_NVRAM_LID, g_pCHISContext->nRecordId, (void *)CallogData, sizeof(mmi_adp_calllog_data_struct), &error);
	  		break;
		default:
			return -1;
	}
#else  /* No Defined __ADP_MULTI_SIM__ */
	switch (type)                   
	{
		case PHB_LND:
			mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Read_Data***PHB_LND*****  ");
			ret = ReadRecord(NVRAM_DIALEDCALL_NVRAM_LID, 1, (void *)CallogData, sizeof(mmi_adp_calllog_data_struct), &error);
			break;

		case PHB_LNM:
			mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Read_Data***PHB_LNM*****  ");
			ret = ReadRecord(NVRAM_MISSEDCALL_NVRAM_LID, 1, (void *)CallogData, sizeof(mmi_adp_calllog_data_struct), &error);
			break;

		case PHB_LNR:
			mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Read_Data***PHB_LNR*****  ");
			ret = ReadRecord(NVRAM_RECVDCALL_NVRAM_LID, 1, (void *)CallogData, sizeof(mmi_adp_calllog_data_struct), &error);
			break;
		case PHB_LNRJ:
	  		mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Read_Data***PHB_LNRJ*****  ");
			ret = ReadRecord(NVRAM_REJECTEDCALL_NVRAM_LID, 1, (void *)CallogData, sizeof(mmi_adp_calllog_data_struct), &error);
	  		break;
		default:
			return -1;
	}
#endif /* __ADP_MULTI_SIM__ */
	return ret;

}

static S32 Calllog_Write_Nvram(UINT8 type, mmi_adp_calllog_data_struct* CallogData)
{
	INT32   ret = 0;
  	UINT16 error = 0;

#ifdef __ADP_MULTI_SIM__
	switch (type)                   
	{
		case PHB_LND:
			ret = WriteRecord(NVRAM_DIALEDCALL_NVRAM_LID, g_pCHISContext->nRecordId, (void *)CallogData, sizeof(mmi_adp_calllog_data_struct), &error);
			break;

		case PHB_LNM:
			ret = WriteRecord(NVRAM_MISSEDCALL_NVRAM_LID, g_pCHISContext->nRecordId, (void *)CallogData, sizeof(mmi_adp_calllog_data_struct), &error);
			break;
			   
		case PHB_LNR:
			ret = WriteRecord(NVRAM_RECVDCALL_NVRAM_LID, g_pCHISContext->nRecordId, (void *)CallogData, sizeof(mmi_adp_calllog_data_struct), &error);
			break;

		case PHB_LNRJ:
			ret = WriteRecord(NVRAM_REJECTEDCALL_NVRAM_LID, g_pCHISContext->nRecordId, (void *)CallogData, sizeof(mmi_adp_calllog_data_struct), &error);
			break;
		default :
			return -1;
	}

	
#else  /* No Defined __ADP_MULTI_SIM__ */
	switch (type)                   
	{
		case PHB_LND:
			ret = WriteRecord(NVRAM_DIALEDCALL_NVRAM_LID, 1, (void *)CallogData, sizeof(mmi_adp_calllog_data_struct), &error);
			break;

		case PHB_LNM:
			ret = WriteRecord(NVRAM_MISSEDCALL_NVRAM_LID, 1, (void *)CallogData, sizeof(mmi_adp_calllog_data_struct), &error);
			break;
			   
		case PHB_LNR:
			ret = WriteRecord(NVRAM_RECVDCALL_NVRAM_LID, 1, (void *)CallogData, sizeof(mmi_adp_calllog_data_struct), &error);
			break;

		case PHB_LNRJ:
			ret = WriteRecord(NVRAM_REJECTEDCALL_NVRAM_LID, 1, (void *)CallogData, sizeof(mmi_adp_calllog_data_struct), &error);
			break;
		default :
			return -1;
	}
#endif /* __ADP_MULTI_SIM__ */
	return ret;

}
/**************************************************************

	FUNCTION NAME		: Calllog_Read_Data

  	PURPOSE				: Read Calllog Data
	INPUT PARAMETERS	: UINT8 type

	OUTPUT PARAMETERS	:

	RETURNS				: 
	REMARKS			: 

**************************************************************/
static BOOL Calllog_Read_Data(UINT8 type)
{
	INT32   ret = 0;
//	BOOL    cRet = FALSE;
	TBM_ENTRY(0x29ed);
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Read_Data***start*****  ");

	memset(&gStruCallogData, 0x00, sizeof(mmi_adp_calllog_data_struct));

	ret = Calllog_Read_Nvram(type, &gStruCallogData);	

	gStruCallogData.type = type;//用于确定是那一个操作
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@FS_Read***end***gStruCallogData.no_list** %d ", gStruCallogData.no_list);
	if (ret < 0)
	{ 
	 	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@FS_Read***error*****  ");
	 	TBM_EXIT(0x29ed);
		//pfclose(hFile);
		return FALSE;
	}
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Read_Data***end*****  ");
	TBM_EXIT(0x29ed);
	return TRUE;
}

/**************************************************************

	FUNCTION NAME		: Calllog_Read_Note_Rsp

  	PURPOSE				:send Read_Note_Rsp to MMI layer
	INPUT PARAMETERS	: COS_EVENT *pCosEvent

	OUTPUT PARAMETERS	:

	RETURNS				: 
	REMARKS			: 

**************************************************************/
static BOOL Calllog_Read_Note_Rsp(COS_EVENT *pCosEvent)
{
	BOOL        read = FALSE;
	ilm_struct *Message = NULL;
	mmi_phb_get_last_number_req_struct *dataPtr = NULL;
	ASSERT(pCosEvent != NULL);

	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Read_Note_Rsp***start*****  ");
	
	Message = (ilm_struct *)pCosEvent->nParam1;
	dataPtr = (mmi_phb_get_last_number_req_struct *)Message->local_para_ptr;
	
	read = Calllog_Read_Data(dataPtr->type);	
	if (!read)
	{
		gStruCallogData.result.flag= CALL_LOG_RESULT_FAIL;
		Adp_SendMessageToMMI((local_para_struct *)&gStruCallogData, PRT_MMI_PHB_GET_LAST_NUMBER_RSP, ADP_CHIS_MSG_SRC_MOD);
		return FALSE;
	}

	gStruCallogData.result.flag= CALL_LOG_RESULT_OK;
	Adp_SendMessageToMMI((local_para_struct *)&gStruCallogData, PRT_MMI_PHB_GET_LAST_NUMBER_RSP, ADP_CHIS_MSG_SRC_MOD);
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Read_Note_Rsp***end*****  ");
	return TRUE;
}

/**************************************************************

	FUNCTION NAME		: Calllog_Compare_Number

  	PURPOSE				: Compare Number
	INPUT PARAMETERS	: U8 *number, U8 *doub

	OUTPUT PARAMETERS	:

	RETURNS				: 
	REMARKS			: 

**************************************************************/
static BOOL Calllog_Compare_Number(U8 *number, U8 *iUpdateDest)
{
	U8 i = 0;
//	U32 temp1, temp2;
	ASSERT(number != NULL);
	ASSERT(iUpdateDest != NULL);
	TBM_ENTRY(0x29ef);


	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Compare_Number***start no_list=%d number:%s", gStruCallogData.no_list, number);


	
	for (i = 0; i < gStruCallogData.no_list; i++)
	{

		if (strcmp((const char *)number, (const char *)gStruCallogData.list[i].tel.number) == 0)
		{			
			*iUpdateDest = i;
			mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Compare_Number****list[%d]:%s  ", i, gStruCallogData.list[i].tel.number);

			return TRUE;
		}

	}
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Compare_Number***end*****  ");	
	TBM_EXIT(0x29ef);
	return FALSE;
}

/**************************************************************

	FUNCTION NAME		: Calllog_Move_Del_One_Number

  	PURPOSE				: Move a record
	INPUT PARAMETERS	: U8 startPos

	OUTPUT PARAMETERS	:

	RETURNS				:
	REMARKS			: 

**************************************************************/
static BOOL Calllog_Move_One_Number(U8 startPos, U8 nMoveDirection)
{
	U8 i = 0;
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Move_One_Number***start*****  ");
		
	if (nMoveDirection == 0)
	{
		for (i = startPos; i > 0; i--)
			memcpy((void *)&gStruCallogData.list[i], (void *)&gStruCallogData.list[i - 1], sizeof(l4c_phb_ln_entry_struct));
	}
	else 
	{
		for (i = startPos; i < gStruCallogData.no_list; i++)
			memcpy((void *)&gStruCallogData.list[i], (void *)&gStruCallogData.list[i + 1], sizeof(l4c_phb_ln_entry_struct));
	}
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Move_One_Number***start*****  ");
	
	return TRUE;
}
/**************************************************************

	FUNCTION NAME		: Calllog_Updata_Number

  	PURPOSE				: Updata record
	INPUT PARAMETERS	: l4c_phb_ln_entry_struct *record, U8 index

	OUTPUT PARAMETERS	:

	RETURNS				: 
	REMARKS			: 

**************************************************************/
static BOOL Calllog_Update_Number(l4c_phb_ln_entry_struct *record, U8 index, U8 count)
{
	ASSERT(record != NULL);
	
	memcpy(&gStruCallogData.list[index], record, sizeof(l4c_phb_ln_entry_struct));
	gStruCallogData.list[index].count = count;
	
	return TRUE;
}

/**************************************************************

	FUNCTION NAME		: Calllog_Add_Number

  	PURPOSE				: Add a  new record
	INPUT PARAMETERS	: l4c_phb_ln_entry_struct *record

	OUTPUT PARAMETERS	:

	RETURNS				: 
	REMARKS			: 

**************************************************************/
static BOOL Calllog_Add_Number(l4c_phb_ln_entry_struct *record)
{
	U8 i = 0;
	ASSERT(record != NULL);
	
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Add_Number***start no_list=%d*****  ", gStruCallogData.no_list);
	
	for (i = gStruCallogData.no_list; i > 0; i--)
	{
		memcpy((void *)&gStruCallogData.list[i], (void *)&gStruCallogData.list[i - 1], sizeof(l4c_phb_ln_entry_struct));				
	}
	memcpy(&gStruCallogData.list[0], record, sizeof(l4c_phb_ln_entry_struct));

	gStruCallogData.no_list++;
	gStruCallogData.list[gStruCallogData.no_list].count = 1;
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Add_Number***end no_list=%d*****  ", gStruCallogData.no_list);
	
	return TRUE;
}

/**************************************************************

	FUNCTION NAME		: Calllog_Write_Data

  	PURPOSE				: Write Calllog Data
	INPUT PARAMETERS	: mmi_phb_set_last_number_req_struct *dataPtr

	OUTPUT PARAMETERS	:

	RETURNS				: void
	REMARKS			: 

**************************************************************/
static BOOL Calllog_Write_Data(mmi_phb_set_last_number_req_struct *dataPtr)
{
	U8 *number = NULL;
	U8 type = 0xff;
	U8 iUpdateDest = 0;
	INT32   ret = 0;
	BOOL flag = FALSE;
//	BOOL cRet  = FALSE;
	ASSERT(dataPtr != NULL);
	TBM_ENTRY(0x29eb);
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Write_Data***start*****  ");	
	number = (U8 *)dataPtr->entry.tel.number;
	type  = dataPtr->type;

	memset(&gStruCallogData, 0x00, sizeof(mmi_adp_calllog_data_struct));

	ret = Calllog_Read_Nvram(type, &gStruCallogData);	

  if (ret < 0)
  {
  	TBM_EXIT(0x29eb);
    return FALSE;

	}
	//used to test start
	{
       	int i = 0;
		for (i = 0; i < 11; i++)
		{
			mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Write_Data**0x%x *****  ",number[i]);
		}
	}
	//used to test end
	
	flag = Calllog_Compare_Number(number, &iUpdateDest);
	if (flag)  //重复
	{
		U8 count = 0;
		MYTIME t1 = {0}, t2 = {0};
		extern TIME_EQUALITY CompareTime(MYTIME,MYTIME,MYTIME*);
		
		mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Write_Data***have the same record *****  ");
		
		t1.nYear = gStruCallogData.list[iUpdateDest].time.rtc_year;
		t1.nMonth = gStruCallogData.list[iUpdateDest].time.rtc_mon;
		t1.nDay = gStruCallogData.list[iUpdateDest].time.rtc_day;
		t1.nHour = gStruCallogData.list[iUpdateDest].time.rtc_hour;
		t1.nMin = gStruCallogData.list[iUpdateDest].time.rtc_min;
		t1.nSec = gStruCallogData.list[iUpdateDest].time.rtc_sec;
		t1.DayIndex = gStruCallogData.list[iUpdateDest].time.rtc_wday;

		t2.nYear = dataPtr->entry.time.rtc_year;
		t2.nMonth = dataPtr->entry.time.rtc_mon;
		t2.nDay = dataPtr->entry.time.rtc_day;
		t2.nHour = dataPtr->entry.time.rtc_hour;
		t2.nMin = dataPtr->entry.time.rtc_min;
		t2.nSec = dataPtr->entry.time.rtc_sec;
		t2.DayIndex = dataPtr->entry.time.rtc_wday;
		
		if (CompareTime(t1, t2, NULL) != TIME_EQUAL) 
		{
			gStruCallogData.list[iUpdateDest].count++;
			count = gStruCallogData.list[iUpdateDest].count;

			if (iUpdateDest != 0)
			{
				Calllog_Move_One_Number(iUpdateDest, 0);			
			}
			Calllog_Update_Number(&dataPtr->entry, 0, count);
		}
		else 
		{
			Calllog_Update_Number(&dataPtr->entry, iUpdateDest, gStruCallogData.list[iUpdateDest].count);	
		}
	}
	else
	{
		mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Write_Data***no same record ***** no_list=%d ", gStruCallogData.no_list);

		//达到最大值
		if (CALLLOG_MAX_RECORD == gStruCallogData.no_list) 
		{
			Calllog_Move_One_Number(gStruCallogData.no_list, 0);
			Calllog_Update_Number((l4c_phb_ln_entry_struct *)&dataPtr->entry, 0, 0);
		}
		else  //加在后面即可
		{
			Calllog_Add_Number(&dataPtr->entry);
		}
	}
  
	ret = Calllog_Write_Nvram(type, &gStruCallogData);	

	if (ret < 0)
	{ 
		TBM_EXIT(0x29eb);
		return FALSE;
	}
  
  //TBM780 hansl 2007/6/15 E
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Write_Data***end*****  ");	
	TBM_EXIT(0x29eb);
	return TRUE;
}

//mmi_phb_set_last_number_rsp_struct  gsendWriteData;

/**************************************************************

	FUNCTION NAME		: Calllog_Write_Note_Rsp

  	PURPOSE				: send Write calllog Rsp to MMI layer 
	INPUT PARAMETERS	: COS_EVENT *pCosEvent

	OUTPUT PARAMETERS	:

	RETURNS				: void
	REMARKS			: 

**************************************************************/
static BOOL Calllog_Write_Note_Rsp(COS_EVENT *pCosEvent)
{
	BOOL        write = FALSE;
	ilm_struct *Message = NULL;
	mmi_phb_set_last_number_req_struct *dataPtr = NULL;
	mmi_phb_set_last_number_rsp_struct *gsendWriteData;
	ASSERT(pCosEvent != NULL);
	
	gsendWriteData = (mmi_phb_set_last_number_rsp_struct*)OslMalloc(sizeof(mmi_phb_set_last_number_rsp_struct));
	ASSERT(NULL != gsendWriteData);
	memset(gsendWriteData, 0x00, sizeof(mmi_phb_set_last_number_rsp_struct));

	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Write_Note_Rsp***start*****  ");
	Message = (ilm_struct *)pCosEvent->nParam1;
	dataPtr = (mmi_phb_set_last_number_req_struct *)Message->local_para_ptr;

	write = Calllog_Write_Data(dataPtr);
	if (!write)
	{
		gsendWriteData->result.flag= CALL_LOG_RESULT_FAIL;
		Adp_SendMessageToMMI((local_para_struct *)gsendWriteData, PRT_MMI_PHB_SET_LAST_NUMBER_RSP, ADP_CHIS_MSG_SRC_MOD);
		return FALSE;
	}

	gsendWriteData->result.flag = CALL_LOG_RESULT_OK;
	gsendWriteData->no_list = gStruCallogData.no_list;
	
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Write_Note_Rsp***gsendWriteData.no_list***** %d ", gsendWriteData->no_list);
	Adp_SendMessageToMMI((local_para_struct *)gsendWriteData, PRT_MMI_PHB_SET_LAST_NUMBER_RSP, ADP_CHIS_MSG_SRC_MOD);
	return TRUE;
}

/**************************************************************

	FUNCTION NAME		: Calllog_Del_All_Record

  	PURPOSE				: Delete all Records
	INPUT PARAMETERS	: U8 type

	OUTPUT PARAMETERS	: 

	RETURNS				: void
	REMARKS			: 

**************************************************************/
static BOOL Calllog_Del_All_Record(U8 type)
{
//	HANDLE hFile = NULL;
	INT32   ret = 0;
//	BOOL   cRet = FALSE;
	
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Del_All_Record***start*****  ");
	memset(&gStruCallogData, 0x00, sizeof(mmi_adp_calllog_data_struct));
	ret = Calllog_Write_Nvram(type, &gStruCallogData);	
	if (ret < 0)
	{
		return FALSE;
	}

  
	#if 0
	if (NULL == hFile)
	{
		return FALSE;
	}

	memset(&gStruCallogData, 0x00, sizeof(mmi_adp_calllog_data_struct));

	ret = pfwrite(hFile, &gStruCallogData, sizeof(mmi_adp_calllog_data_struct));
	if (-1 == ret)
	{ 
		pfclose(hFile);
		return FALSE;
	}

	cRet = pfclose(hFile);
	if (!cRet)
	{
		mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@FS_Close***error*****  ");
		return FALSE;
	}
  #endif
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Del_All_Record***end*****  ");

	return TRUE;
}

/**************************************************************

	FUNCTION NAME		: Calllog_Del_One_Record

  	PURPOSE				: Delete a record
	INPUT PARAMETERS	: mmi_phb_del_last_number_req_struct *dataPtr

	OUTPUT PARAMETERS	:

	RETURNS				: void
	REMARKS			: 

**************************************************************/
static BOOL Calllog_Del_One_Record(mmi_phb_del_last_number_req_struct *dataPtr)
{
//	HANDLE hFile = NULL;
	INT32   ret = 0;
//	BOOL   cRet = FALSE;
	ASSERT(dataPtr != NULL);
	TBM_ENTRY(0x29ec);
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Del_One_Record***start*****  ");
	memset(&gStruCallogData, 0x00, sizeof(mmi_adp_calllog_data_struct));
	ret = Calllog_Read_Nvram(dataPtr->type, &gStruCallogData);	
  
	if (ret < 0)
	{
		TBM_EXIT(0x29ec);
		return FALSE;
	}
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Del_One_Record***dataPtr->index*****%d  ", dataPtr->index);
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Del_One_Record***gStruCallogData.no_list*****%d  ", gStruCallogData.no_list);

	if(( gStruCallogData.no_list > 1) &&
		((gStruCallogData.no_list -1) != dataPtr->index))
	{
			Calllog_Move_One_Number(dataPtr->index, 1);
	}
	gStruCallogData.no_list--;

	ret = Calllog_Write_Nvram(dataPtr->type, &gStruCallogData);	
  
  	if (ret < 0)
  	{ 
  			TBM_EXIT(0x29ec);
  		return FALSE;
  	}

	#if 0
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Del_One_Record***dataPtr->index*****%d  ", gStruCallogData.no_list);
	pfseek(hFile, 0, FS_SEEK_SET);
	ret = pfwrite(hFile, &gStruCallogData, sizeof(mmi_adp_calllog_data_struct));
	if (-1 == ret)
  	{ 
  		pfclose(hFile);
  		return FALSE;
  	}

	cRet = pfclose(hFile);
	if (!cRet)
	{
		return FALSE;
	}
  #endif
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Del_One_Record***end*****  ");
	TBM_EXIT(0x29ec);
	return TRUE;
	
}

/**************************************************************

	FUNCTION NAME		: Calllog_Del_Data

  	PURPOSE				: Delete calllog Data
	INPUT PARAMETERS	: mmi_phb_del_last_number_req_struct *dataPtr

	OUTPUT PARAMETERS	:

	RETURNS				: void
	REMARKS			: 

**************************************************************/
static BOOL Calllog_Del_Data(mmi_phb_del_last_number_req_struct *dataPtr)
{
	ASSERT(dataPtr != NULL);
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Del_Data***start*****  ");

	//del all record
	if ((0xff == dataPtr->index) && (0 == dataPtr->no_data))
	{
		mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Del_Data***if((0xff == dataPtr->index)&&*****  ");
		Calllog_Del_All_Record(dataPtr->type);
	}
	else  //del one record
	{
		Calllog_Del_One_Record(dataPtr);
	}
	
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Del_Data***end*****  ");

	return TRUE;
}	

//mmi_phb_del_last_number_rsp_struct gsendDelData;

/**************************************************************

	FUNCTION NAME		: Calllog_Del_Note_Rsp

  	PURPOSE				:send Delete_calllog_Rsp to MMI layer
	INPUT PARAMETERS	: COS_EVENT *pCosEvent

	OUTPUT PARAMETERS	:

	RETURNS				: void
	REMARKS			: 

**************************************************************/
static BOOL Calllog_Del_Note_Rsp(COS_EVENT *pCosEvent)
{
	ilm_struct *Message = NULL;
	mmi_phb_del_last_number_req_struct *dataPtr = NULL;
	mmi_phb_del_last_number_rsp_struct *gsendDelData;
	BOOL        del = FALSE;
	ASSERT(pCosEvent != NULL);

	gsendDelData = (mmi_phb_del_last_number_rsp_struct*)OslMalloc(sizeof(mmi_phb_del_last_number_rsp_struct));
	ASSERT(NULL != gsendDelData);
		memset(gsendDelData, 0x00, sizeof(mmi_phb_del_last_number_rsp_struct));
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Del_Note_Rsp***start*****  ");
	Message = (ilm_struct *)pCosEvent->nParam1;
	dataPtr = (mmi_phb_del_last_number_req_struct *)Message->local_para_ptr;
	del = Calllog_Del_Data(dataPtr);
	if (!del)
	{
		gsendDelData->result.flag= CALL_LOG_RESULT_FAIL;
		Adp_SendMessageToMMI((local_para_struct *)gsendDelData, PRT_MMI_PHB_DEL_LAST_NUMBER_RSP, ADP_CHIS_MSG_SRC_MOD);

		return FALSE;
	}

	memset(gsendDelData, 0x00, sizeof(mmi_phb_set_last_number_rsp_struct));
	gsendDelData->result.flag = CALL_LOG_RESULT_OK;

	Adp_SendMessageToMMI((local_para_struct *)gsendDelData, PRT_MMI_PHB_DEL_LAST_NUMBER_RSP, ADP_CHIS_MSG_SRC_MOD);
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_Del_Note_Rsp***end*****  ");
	
	return TRUE;
}

//mmi_cc_get_max_acm_rsp_struct gMaxAcmRsp;
/**************************************************************

	FUNCTION NAME		: Calllog_GetMaxAcm

  	PURPOSE				: send Get_MaxCallCost_RSP to MMI layer
	INPUT PARAMETERS	: COS_EVENT *pCosEvent

	OUTPUT PARAMETERS	:

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static BOOL Calllog_GetMaxAcm_Rsp(COS_EVENT *pCosEvent)
{
	INT16 error = 0;
  mmi_cc_get_max_acm_rsp_struct *gMaxAcmRsp;

	gMaxAcmRsp = (mmi_cc_get_max_acm_rsp_struct*)OslMalloc(sizeof(mmi_cc_get_max_acm_rsp_struct));
	ASSERT(NULL != gMaxAcmRsp);
	memset(gMaxAcmRsp, 0x00, sizeof(mmi_cc_get_max_acm_rsp_struct));
#ifdef __ADP_MULTI_SIM__
	ReadRecord(NVRAM_EF_CALL_COST_GET_MAX_LID, g_pCHISContext->nRecordId, 
		(void *)&gMaxAcmRsp->acm_max, sizeof(gMaxAcmRsp->acm_max), &error); 
#else  /* No Defined __ADP_MULTI_SIM__ */
	ReadRecord(NVRAM_EF_CALL_COST_GET_MAX_LID, 1, 
		(void *)&gMaxAcmRsp->acm_max, sizeof(gMaxAcmRsp->acm_max), &error); 
#endif /* __ADP_MULTI_SIM__ */

	gMaxAcmRsp->result = TRUE;
	Adp_SendMessageToMMI((local_para_struct *)gMaxAcmRsp, PRT_MMI_CC_GET_MAX_ACM_RES, ADP_CHIS_MSG_SRC_MOD);

	return TRUE;
}

//mmi_cc_set_max_acm_rsp_struct gSetMaxAcmRsp;
/**************************************************************

	FUNCTION NAME		: Calllog_SetMaxAcm_Rsp

  	PURPOSE				:send Set_MaxCallCost_RSP to MMI layer
	INPUT PARAMETERS	: COS_EVENT *pCosEvent

	OUTPUT PARAMETERS	:

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static BOOL Calllog_SetMaxAcm_Rsp(COS_EVENT *pCosEvent)
{
	ilm_struct *Message= NULL;
	mmi_cc_set_max_acm_req_struct *dataPtr = NULL;
	INT16   error = 0;
	mmi_cc_set_max_acm_rsp_struct *gSetMaxAcmRsp;
	gSetMaxAcmRsp = (mmi_cc_set_max_acm_rsp_struct*)OslMalloc(sizeof(mmi_cc_set_max_acm_rsp_struct));
	ASSERT(NULL != gSetMaxAcmRsp);
	memset(gSetMaxAcmRsp, 0x00, sizeof(mmi_cc_set_max_acm_rsp_struct));

	Message = (ilm_struct *)pCosEvent->nParam1;
	dataPtr = (mmi_cc_set_max_acm_req_struct *)Message->local_para_ptr;
	 
	// Save maximum acm into flash device.
#ifdef __ADP_MULTI_SIM__
	WriteRecord(NVRAM_EF_CALL_COST_GET_MAX_LID, g_pCHISContext->nRecordId, 
		(void *)&dataPtr->val, sizeof(dataPtr->val), &error);
#else  /* No Defined __ADP_MULTI_SIM__ */
	WriteRecord(NVRAM_EF_CALL_COST_GET_MAX_LID, 1, 
		(void *)&dataPtr->val, sizeof(dataPtr->val), &error);
#endif /* __ADP_MULTI_SIM__ */

	// Send RSP message to MMI layer.
	gSetMaxAcmRsp->result = TRUE;
	Adp_SendMessageToMMI((local_para_struct *)gSetMaxAcmRsp, PRT_MMI_CC_SET_MAX_ACM_RSP, ADP_CHIS_MSG_SRC_MOD);

	return TRUE;
}

//mmi_cc_reset_acm_rsp_struct gResetAcmRsp;
/**************************************************************

	FUNCTION NAME		: Calllog_ResetAcm_Rsp

  	PURPOSE				:send Reset_Acm_Rsp to MMI layer
	INPUT PARAMETERS	: COS_EVENT *pCosEvent

	OUTPUT PARAMETERS	:

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static BOOL Calllog_ResetAcm_Rsp(COS_EVENT *pCosEvent)
{
	UINT16 error = 0;
	mmi_cc_reset_acm_rsp_struct *gResetAcmRsp;
	
#ifdef _MMI_OPERATOR_
	U8 sLast[4 + 1 + 2] = {'0',0, 0, 0, 0, 0, 0};
	U8 sAll[6 + 1 + 2] = {'0',0, 0, 0, 0, 0, 0, 0, 0};

#ifdef __ADP_MULTI_SIM__
	WriteRecord(NVRAM_EF_CALL_COST_GET_ALL_COST_LID, g_pCHISContext->nRecordId, 
		(void *)sLast, sizeof(sLast), &error);
	WriteRecord(NVRAM_EF_CALL_COST_GET_LAST_COST_LID, g_pCHISContext->nRecordId, 
		(void *)sAll, sizeof(sAll), &error);
#else  /* No Defined __ADP_MULTI_SIM__ */
	WriteRecord(NVRAM_EF_CALL_COST_GET_ALL_COST_LID, 1, 
		(void *)sLast, sizeof(sLast), &error);
	WriteRecord(NVRAM_EF_CALL_COST_GET_LAST_COST_LID, 1, 
		(void *)sAll, sizeof(sAll), &error);
#endif /* __ADP_MULTI_SIM__ */
#else
	double fCallCost = 0;

#ifdef __ADP_MULTI_SIM__
	WriteRecord(NVRAM_EF_CALL_COST_GET_ALL_COST_LID, g_pCHISContext->nRecordId, 
		(void *)&sLast, sizeof(sLast), &error);
	WriteRecord(NVRAM_EF_CALL_COST_GET_LAST_COST_LID, g_pCHISContext->nRecordId, 
		(void *)&sLast, sizeof(sLast), &error);
#else  /* No Defined __ADP_MULTI_SIM__ */
	WriteRecord(NVRAM_EF_CALL_COST_GET_ALL_COST_LID, 1, 
		(void *)&sLast, sizeof(sLast), &error);
	WriteRecord(NVRAM_EF_CALL_COST_GET_LAST_COST_LID, 1, 
		(void *)&sLast, sizeof(sLast), &error);
#endif /* __ADP_MULTI_SIM__ */
#endif

	gResetAcmRsp = (mmi_cc_reset_acm_rsp_struct*)OslMalloc(sizeof(mmi_cc_reset_acm_rsp_struct));
	ASSERT(NULL != gResetAcmRsp);
	
	memset(gResetAcmRsp, 0, sizeof(mmi_cc_reset_acm_rsp_struct));
	gResetAcmRsp->result = TRUE;
	
	Adp_SendMessageToMMI((local_para_struct *)gResetAcmRsp, PRT_MMI_CC_RESET_ACM_RES, ADP_CHIS_MSG_SRC_MOD);

	return TRUE;
}

//mmi_cc_get_acm_rsp_struct gGetAllAcmRsp;
/**************************************************************

	FUNCTION NAME		: Calllog_GetAllCallcost_Rsp

  	PURPOSE				:send Get_AllCallcost_Rsp to MMI layer
	INPUT PARAMETERS	: COS_EVENT *pCosEvent

	OUTPUT PARAMETERS	:

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static BOOL Calllog_GetAllCallcost_Rsp(COS_EVENT *pCosEvent)
{
	UINT16 error = 0;
	mmi_cc_get_acm_rsp_struct *gGetAllAcmRsp;
	
	gGetAllAcmRsp = (mmi_cc_get_acm_rsp_struct*)OslMalloc(sizeof(mmi_cc_get_acm_rsp_struct));
	ASSERT(NULL != gGetAllAcmRsp);

	memset(gGetAllAcmRsp, 0x00, sizeof(mmi_cc_get_acm_rsp_struct));
	//gGetAllAcmRsp.acm = 
#ifdef __ADP_MULTI_SIM__
#ifdef _MMI_OPERATOR_
	ReadRecord(NVRAM_EF_CALL_COST_GET_ALL_COST_LID, g_pCHISContext->nRecordId, 
		(void *)&gGetAllAcmRsp->acm, sizeof(gGetAllAcmRsp->acm), &error);
#else
	ReadRecord(NVRAM_EF_CALL_COST_GET_ALL_COST_LID, g_pCHISContext->nRecordId, 
		(void *)&gGetAllAcmRsp->acm, sizeof(gGetAllAcmRsp->acm), &error);
#endif
#else  /* No Defined __ADP_MULTI_SIM__ */
#ifdef _MMI_OPERATOR_
	ReadRecord(NVRAM_EF_CALL_COST_GET_ALL_COST_LID, 1, 
		(void *)&gGetAllAcmRsp->acm, sizeof(gGetAllAcmRsp->acm), &error);
#else
	ReadRecord(NVRAM_EF_CALL_COST_GET_ALL_COST_LID, 1, 
		(void *)&gGetAllAcmRsp->acm, sizeof(gGetAllAcmRsp->acm), &error);
#endif
#endif /* __ADP_MULTI_SIM__ */
	gGetAllAcmRsp->result = TRUE;

	Adp_SendMessageToMMI((local_para_struct *)gGetAllAcmRsp, PRT_MMI_CC_GET_ACM_RES, ADP_CHIS_MSG_SRC_MOD);

	return TRUE;
}

//mmi_cc_get_ccm_rsp_struct gGetLastCcmRsp;
/**************************************************************

	FUNCTION NAME		: Calllog_GetLastCallcost_Rsp

  	PURPOSE				: Send Get_LastCallcost_Rsp to MMI layer
	INPUT PARAMETERS	: COS_EVENT *pCosEvent

	OUTPUT PARAMETERS	:

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static BOOL Calllog_GetLastCallcost_Rsp(COS_EVENT *pCosEvent)
{
	UINT16 error = 0;
	mmi_cc_get_ccm_rsp_struct *gGetLastCcmRsp;
	
	gGetLastCcmRsp = (mmi_cc_get_ccm_rsp_struct*)OslMalloc(sizeof(mmi_cc_get_ccm_rsp_struct));
	ASSERT(NULL != gGetLastCcmRsp);

	memset(gGetLastCcmRsp, 0x00, sizeof(mmi_cc_get_ccm_rsp_struct));
	gGetLastCcmRsp->result = TRUE;
	//gGetLastCcmRsp.ccm =
#ifdef _MMI_OPERATOR_
#ifdef __ADP_MULTI_SIM__
	ReadRecord(NVRAM_EF_CALL_COST_GET_LAST_COST_LID, g_pCHISContext->nRecordId, 
		(void *)&gGetLastCcmRsp->ccm, sizeof(gGetLastCcmRsp->ccm), &error);
#else  /* No Defined __ADP_MULTI_SIM__ */
	ReadRecord(NVRAM_EF_CALL_COST_GET_LAST_COST_LID, 1, 
		(void *)&gGetLastCcmRsp->ccm, sizeof(gGetLastCcmRsp->ccm), &error);
#endif /* __ADP_MULTI_SIM__ */
#else	
#ifdef __ADP_MULTI_SIM__
	ReadRecord(NVRAM_EF_CALL_COST_GET_LAST_COST_LID, g_pCHISContext->nRecordId, 
		(void *)&gGetLastCcmRsp->ccm, sizeof(gGetLastCcmRsp->ccm), &error);
#else  /* No Defined __ADP_MULTI_SIM__ */
	ReadRecord(NVRAM_EF_CALL_COST_GET_LAST_COST_LID, 1, 
		(void *)&gGetLastCcmRsp->ccm, sizeof(gGetLastCcmRsp->ccm), &error);
#endif /* __ADP_MULTI_SIM__ */
#endif

	Adp_SendMessageToMMI((local_para_struct *)gGetLastCcmRsp, PRT_MMI_CC_GET_CCM_RES, ADP_CHIS_MSG_SRC_MOD);

	return TRUE;
}


//mmi_smu_get_puc_rsp_struct gSetPriceAndUnitRsp;
/**************************************************************

	FUNCTION NAME		: Calllog_SetPriceandUnit_Rsp

  	PURPOSE				:send Set_PriceandUnit_Rsp to MMI layer
	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			:

**************************************************************/
BOOL Calllog_SetPriceAndUnit_Rsp(COS_EVENT *pCosEvent)
{
	ilm_struct *Message= NULL;
	mmi_smu_set_puc_req_struct *dataPtr = NULL;
	UINT16 error = 0;
	mmi_smu_set_puc_rsp_struct *gSetPriceAndUnitRsp;
	
	gSetPriceAndUnitRsp = (mmi_smu_set_puc_rsp_struct*)OslMalloc(sizeof(mmi_smu_set_puc_rsp_struct));
	ASSERT(NULL != gSetPriceAndUnitRsp);

	Message = (ilm_struct *)pCosEvent->nParam1;
	dataPtr = (mmi_smu_set_puc_req_struct *)Message->local_para_ptr;
	
#ifdef __ADP_MULTI_SIM__
	WriteRecord(NVRAM_EF_CALL_COST_GET_CURRENCY_LID, g_pCHISContext->nRecordId, 
		(void *)dataPtr->currency, sizeof(dataPtr->currency), &error);
	WriteRecord(NVRAM_EF_CALL_COST_GET_PPU_LID, g_pCHISContext->nRecordId, 
		(void *)dataPtr->ppu, sizeof(dataPtr->ppu), &error);
#else  /* No Defined __ADP_MULTI_SIM__ */
	WriteRecord(NVRAM_EF_CALL_COST_GET_CURRENCY_LID, 1, 
		(void *)dataPtr->currency, sizeof(dataPtr->currency), &error);
	WriteRecord(NVRAM_EF_CALL_COST_GET_PPU_LID, 1, 
		(void *)dataPtr->ppu, sizeof(dataPtr->ppu), &error);
#endif /* __ADP_MULTI_SIM__ */
	
	memset(gSetPriceAndUnitRsp, 0, sizeof(gSetPriceAndUnitRsp));
	gSetPriceAndUnitRsp->result = TRUE;

	Adp_SendMessageToMMI((local_para_struct *)gSetPriceAndUnitRsp, PRT_MMI_CALL_COST_PRICEUNIT_SET_RSP, ADP_CHIS_MSG_SRC_MOD);

	return TRUE;
}


//mmi_smu_get_puc_rsp_struct gGetPriceAndUnitRsp;
/**************************************************************

	FUNCTION NAME		: Calllog_GetPriceandUnit_Rsp

  	PURPOSE				: send Get_PriceandUnit_Rsp to MMI layer

	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS			:

**************************************************************/
BOOL Calllog_GetPriceAndUnit_Rsp(COS_EVENT *pCosEvent)
{
	UINT16 error = 0;
	mmi_smu_get_puc_rsp_struct *gGetPriceAndUnitRsp;
	
	gGetPriceAndUnitRsp = (mmi_smu_get_puc_rsp_struct*)OslMalloc(sizeof(mmi_smu_get_puc_rsp_struct));
	ASSERT(NULL != gGetPriceAndUnitRsp);
	memset(gGetPriceAndUnitRsp, 0, sizeof(mmi_smu_get_puc_rsp_struct));
	gGetPriceAndUnitRsp->result = TRUE;
	
#ifdef __ADP_MULTI_SIM__
	ReadRecord(NVRAM_EF_CALL_COST_GET_CURRENCY_LID, g_pCHISContext->nRecordId, 
		(void *)&gGetPriceAndUnitRsp->currency, sizeof(gGetPriceAndUnitRsp->currency), &error);
	ReadRecord(NVRAM_EF_CALL_COST_GET_PPU_LID, g_pCHISContext->nRecordId, 
		(void *)&gGetPriceAndUnitRsp->ppu, sizeof(gGetPriceAndUnitRsp->ppu), &error);
#else  /* No Defined __ADP_MULTI_SIM__ */
	ReadRecord(NVRAM_EF_CALL_COST_GET_CURRENCY_LID, 1, 
		(void *)&gGetPriceAndUnitRsp->currency, sizeof(gGetPriceAndUnitRsp->currency), &error);
	ReadRecord(NVRAM_EF_CALL_COST_GET_PPU_LID, 1, 
		(void *)&gGetPriceAndUnitRsp->ppu, sizeof(gGetPriceAndUnitRsp->ppu), &error);
#endif /* __ADP_MULTI_SIM__ */


	Adp_SendMessageToMMI((local_para_struct *)gGetPriceAndUnitRsp, PRT_MMI_CALL_COST_PRICEUNIT_GET_RSP, ADP_CHIS_MSG_SRC_MOD);

	return TRUE;
}


/**************************************************************

	FUNCTION NAME		: ADP_DispatchCalllogMsg

  	PURPOSE				: Recevied adaption Message
	INPUT PARAMETERS	: COS_EVENT *pCosEvent

	OUTPUT PARAMETERS	:

	RETURNS				: void

	REMARKS			: 

**************************************************************/
BOOL ADP_DispatchCalllogMsg(COS_EVENT *pCosEvent)
{
	TBM_ENTRY(0x29b3);
	ASSERT(pCosEvent != NULL);

#ifdef __ADP_MULTI_SIM__
	adp_ConfigureContext(ADP_MODULE_CALL_LOG, adp_GetMMISimID(ADP_GET_MMI_DEST_MOD(pCosEvent->nParam3)));
#endif
	switch (pCosEvent->nEventId)                   
	{
		case PRT_MMI_PHB_GET_LAST_NUMBER_REQ:
			mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@ADP_DispatchCalllogMsg**Read*start*****  ");
			Calllog_Read_Note_Rsp(pCosEvent);
			break;
		   
		case PRT_MMI_PHB_SET_LAST_NUMBER_REQ:
			mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@ADP_DispatchCalllogMsg**Write*start*****  ");
			Calllog_Write_Note_Rsp(pCosEvent);
			break;
		   
		case PRT_MMI_PHB_DEL_LAST_NUMBER_REQ:
			mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@ADP_DispatchCalllogMsg**Del*start*****  ");
			Calllog_Del_Note_Rsp(pCosEvent);
			break;
			
		case PRT_MMI_CC_GET_MAX_ACM_REQ:
			mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@ADP_DispatchCalllogMsg**GetMaxAcm*start*****  ");
			Calllog_GetMaxAcm_Rsp(pCosEvent);
			break;

		case PRT_MMI_CC_SET_MAX_ACM_REQ:
			mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@ADP_DispatchCalllogMsg**SetMaxAcm*start*****  ");
			Calllog_SetMaxAcm_Rsp(pCosEvent);
			break;

		case PRT_MMI_CC_RESET_ACM_REQ:
			mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@ADP_DispatchCalllogMsg**ResetAcm*start*****  ");
			Calllog_ResetAcm_Rsp(pCosEvent);
			break;

		case PRT_MMI_CC_GET_ACM_REQ:
			mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@ADP_DispatchCalllogMsg**GetAllCallcost*start*****  ");
			Calllog_GetAllCallcost_Rsp(pCosEvent);
			break;

		case PRT_MMI_CC_GET_CCM_REQ:
			mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@ADP_DispatchCalllogMsg**GetLastCallcost*start*****  ");
			Calllog_GetLastCallcost_Rsp(pCosEvent);
			break;
		case PRT_MMI_CALL_COST_PRICEUNIT_GET_REQ:
			mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_GetPriceandUnit_Rsp***start*****  ");
			Calllog_GetPriceAndUnit_Rsp(pCosEvent);
			break;

		case PRT_MMI_CALL_COST_PRICEUNIT_SET_REQ:
			mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Calllog_SetPriceandUnit_Rsp***start*****  ");
			Calllog_SetPriceAndUnit_Rsp(pCosEvent);
			break;

	
		default :
			TBM_EXIT(0x29b3);
			return FALSE;
	}
	TBM_EXIT(0x29b3);
	return TRUE;
}

#ifdef __ADP_MULTI_SIM__
BOOL ADP_CalllogInit(U8 nSimId)
#else  /* No Defined __ADP_MULTI_SIM__ */
BOOL ADP_CalllogInit()
#endif /* __ADP_MULTI_SIM__ */
{
	int sim_id;
	U8 init_flag;
	U16 error;

#ifdef __ADP_MULTI_SIM__
	sim_id =  nSimId;
#else  /* No Defined __ADP_MULTI_SIM__ */
	sim_id = 0;
#endif /* __ADP_MULTI_SIM__ */

	ReadRecord(NVRAM_EF_CHIS_NUM_LID, sim_id + 1, (void *)&init_flag, 1, &error);
	mmi_trace(g_sw_ADP_CALLLOG,"MMI_Calllog: Func: %s init_flag=%d", __FUNCTION__, init_flag);
	if (init_flag == 0xFF)
	{
		memset(&gStruCallogData, 0x00, sizeof(mmi_adp_calllog_data_struct));

		WriteRecord(NVRAM_DIALEDCALL_NVRAM_LID, sim_id + 1, (void *)&gStruCallogData, sizeof(mmi_adp_calllog_data_struct), &error);
		WriteRecord(NVRAM_MISSEDCALL_NVRAM_LID, sim_id + 1, (void *)&gStruCallogData, sizeof(mmi_adp_calllog_data_struct), &error);
		WriteRecord(NVRAM_RECVDCALL_NVRAM_LID, sim_id + 1, (void *)&gStruCallogData, sizeof(mmi_adp_calllog_data_struct), &error);
		WriteRecord(NVRAM_REJECTEDCALL_NVRAM_LID, sim_id + 1, (void *)&gStruCallogData, sizeof(mmi_adp_calllog_data_struct), &error);
	
		init_flag = 1;
		WriteRecord(NVRAM_EF_CHIS_NUM_LID, sim_id + 1, (void *)&init_flag, 1, &error);

		return FALSE;
	}
	return TRUE;

}

/**************************************************************

	FUNCTION NAME		: Adp_Calllog_Send_Start_Ind

  	PURPOSE				: Send calllog init IND
	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: 

	RETURNS				: void
						 
	REMARKS			: 

**************************************************************/
#ifdef __ADP_MULTI_SIM__
void Adp_Calllog_Send_Start_Ind(U8 nSimId)
#else  /* No Defined __ADP_MULTI_SIM__ */
void Adp_Calllog_Send_Start_Ind(void)
#endif /* __ADP_MULTI_SIM__ */
{
	U32 src_mod = MOD_L4C;
	mmi_chis_num_struct *pchis_num;
	U16 error;

	Calllog_Init_context();

#ifdef __ADP_MULTI_SIM__
	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Adp_Calllog_Send_Start_Ind***start*****  nSimId: %d", nSimId);
	src_mod = MOD_L4C + nSimId - ADP_SIM_1;
#endif /* __ADP_MULTI_SIM__ */
	
	pchis_num = 
		(mmi_chis_num_struct*)OslMalloc(sizeof(mmi_chis_num_struct));

	ASSERT(pchis_num != NULL);
	memset(pchis_num, 0, sizeof(mmi_chis_num_struct));
#ifdef __ADP_MULTI_SIM__
	if (ADP_CalllogInit(nSimId))
	{
		ReadRecord(NVRAM_DIALEDCALL_NVRAM_LID, nSimId + 1, (void *)&gStruCallogData, sizeof(mmi_adp_calllog_data_struct), &error);
		pchis_num->nDialedcall =  gStruCallogData.no_list;
		ReadRecord(NVRAM_MISSEDCALL_NVRAM_LID, nSimId + 1, (void *)&gStruCallogData, sizeof(mmi_adp_calllog_data_struct), &error);
		pchis_num->nMissedcall =  gStruCallogData.no_list;
		ReadRecord(NVRAM_RECVDCALL_NVRAM_LID, nSimId + 1, (void *)&gStruCallogData, sizeof(mmi_adp_calllog_data_struct), &error);
		pchis_num->nReceivedcall =  gStruCallogData.no_list;
		ReadRecord(NVRAM_REJECTEDCALL_NVRAM_LID, nSimId + 1, (void *)&gStruCallogData, sizeof(mmi_adp_calllog_data_struct), &error);
		pchis_num->nRejectedcall =  gStruCallogData.no_list;
	}
#else  /* No Defined __ADP_MULTI_SIM__ */
	if (ADP_CalllogInit())
	{
		ReadRecord(NVRAM_DIALEDCALL_NVRAM_LID, 1, (void *)&gStruCallogData, sizeof(mmi_adp_calllog_data_struct), &error);
		pchis_num->nDialedcall =  gStruCallogData.no_list;
		ReadRecord(NVRAM_MISSEDCALL_NVRAM_LID, 1, (void *)&gStruCallogData, sizeof(mmi_adp_calllog_data_struct), &error);
		pchis_num->nMissedcall =  gStruCallogData.no_list;
		ReadRecord(NVRAM_RECVDCALL_NVRAM_LID, 1, (void *)&gStruCallogData, sizeof(mmi_adp_calllog_data_struct), &error);
		pchis_num->nReceivedcall =  gStruCallogData.no_list;
		ReadRecord(NVRAM_REJECTEDCALL_NVRAM_LID, 1, (void *)&gStruCallogData, sizeof(mmi_adp_calllog_data_struct), &error);
		pchis_num->nRejectedcall =  gStruCallogData.no_list;
	}
#endif /* __ADP_MULTI_SIM__ */

	Adp_SendMessageToMMI((local_para_struct *)pchis_num, PRT_MMI_PHB_LAST_NUMBER_READY_IND, src_mod);

	mmi_trace(g_sw_ADP_CALLLOG, "@@@@@@Adp_Calllog_Send_Start_Ind***start*****  pchis_num=%p",pchis_num);
}

