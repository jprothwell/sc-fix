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
// CallMgmtIncoming.cpp: implementation of the Call Mgmt Incoming Handling.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "string.h"
#include "stdlib.h"
#include "CallMgmtStruct.h"
#include "MemAllocator.h"

SET_LAST_NUMBER_RSP_STRUCT *gSetRspStruct;
//SET_LAST_NUMBER_RSP_STRUCT *gSetRspStructForReturn;
//SET_LAST_NUMBER_RSP_STRUCT *gSetRspEntries;
//GET_LAST_NUMBER_RSP_STRUCT *gGetRspStruct;
//GET_LAST_NUMBER_RSP_STRUCT *gGetRspStructForReturn;
GET_LAST_NUMBER_RSP_STRUCT *gGetRspEntries;
kal_uint8 DialedCallCount =0;
kal_uint8 RecvdCallCount=0;
kal_uint8 MissedCallCount=0;
FILE *fpCallHistory;
void * gCallHistoryBuffer = NULL;
//GET_SET_LAST_NUMBER_RSP_STRUCT gSetGetRspEntries[MAX_CALL_HISTORY_ENTRIES];
MMI_CC_CALL_RING_IND_STRUCT *gIncomingCallIndStruct;

void SetBufferForCallHistory( void * dataBuf )
{
	gCallHistoryBuffer = dataBuf ;
}

void * SetLastNoRsp(UINT32 nSimNum)
{
	MMI_PHB_SET_LAST_NUMBER_REQ_STRUCT *setLastNoReq = (MMI_PHB_SET_LAST_NUMBER_REQ_STRUCT *)gCallHistoryBuffer;

	printf(" ****SetLastNoRsp******* " );

	gSetRspStruct = (SET_LAST_NUMBER_RSP_STRUCT *)
		CMemAllocator::MyMallocDataPtr(sizeof(SET_LAST_NUMBER_RSP_STRUCT));

	gSetRspStruct->ref_count=setLastNoReq->ref_count;
	gSetRspStruct->msg_len=setLastNoReq->msg_len;

	gSetRspStruct->result.flag=0;
	gSetRspStruct->result.cause=0;

	gSetRspStruct->list[0].tel.type=setLastNoReq->entry.tel.type;
	//No_list set in the end for code optimization

	gSetRspStruct->list[0].tel.length=setLastNoReq->entry.tel.length;
	if(strlen(setLastNoReq->entry.tel.number)!=0)
		strcpy(gSetRspStruct->list[0].tel.number,setLastNoReq->entry.tel.number);
	else
		strcpy(gSetRspStruct->list[0].tel.number,"NULL");
	gSetRspStruct->list[0].alpha_id.name_length=setLastNoReq->entry.alpha_id.name_length;
	gSetRspStruct->list[0].alpha_id.name_dcs=setLastNoReq->entry.alpha_id.name_dcs;
	if(strlen(setLastNoReq->entry.alpha_id.name)!=0)
		strcpy(gSetRspStruct->list[0].alpha_id.name,setLastNoReq->entry.alpha_id.name);
	else
		strcpy(gSetRspStruct->list[0].alpha_id.name,"NULL");
	gSetRspStruct->list[0].time.rtc_sec=setLastNoReq->entry.time.rtc_sec;
	gSetRspStruct->list[0].time.rtc_min=setLastNoReq->entry.time.rtc_min;
	gSetRspStruct->list[0].time.rtc_hour=setLastNoReq->entry.time.rtc_hour;
	gSetRspStruct->list[0].time.rtc_day=setLastNoReq->entry.time.rtc_day;
	gSetRspStruct->list[0].time.rtc_mon=setLastNoReq->entry.time.rtc_mon;
	gSetRspStruct->list[0].time.rtc_wday=setLastNoReq->entry.time.rtc_wday;
	gSetRspStruct->list[0].time.rtc_year=setLastNoReq->entry.time.rtc_year;
	gSetRspStruct->list[0].count= 1; //setLastNoReq->entry.count;

	if(setLastNoReq->type==0)
	{
		++DialedCallCount;
		gSetRspStruct->no_list=DialedCallCount;
		CallHistoryWriteToFile(0,nSimNum);
	}else if(setLastNoReq->type==1)
	{	
		++MissedCallCount;
		gSetRspStruct->no_list=MissedCallCount;
		CallHistoryWriteToFile(1,nSimNum);
	}else if(setLastNoReq->type==2)
	{
		++RecvdCallCount;
		gSetRspStruct->no_list=RecvdCallCount;
		CallHistoryWriteToFile(2,nSimNum);
	}
	
	return (void *)gSetRspStruct;
}

void * GetLastNoRsp(UINT32 nSimNum)
{
	MMI_PHB_GET_LAST_NUMBER_REQ_STRUCT *getLastNoReq = (MMI_PHB_GET_LAST_NUMBER_REQ_STRUCT *)gCallHistoryBuffer;
	
	printf(" ****GetLastNoRsp******* " );

	if(getLastNoReq->type==0)
	{
		CallHistoryReadFromFile(0,nSimNum);
		DialedCallCount = gGetRspEntries->no_list;
	}else if(getLastNoReq->type==1)
	{
		CallHistoryReadFromFile(1,nSimNum);
		MissedCallCount = gGetRspEntries->no_list;
	}else if(getLastNoReq->type==2,nSimNum)
	{
		CallHistoryReadFromFile(2,nSimNum);
		RecvdCallCount = gGetRspEntries->no_list;
	}
  else
	{
    // Modified by chenzu,2008-10-10
		gGetRspEntries = (GET_LAST_NUMBER_RSP_STRUCT *)
			CMemAllocator::MyMallocDataPtr(sizeof(GET_LAST_NUMBER_RSP_STRUCT)*MAX_CALL_HISTORY_ENTRIES);	
		gGetRspEntries->ref_count = 0;
		gGetRspEntries->msg_len = 91;
		gGetRspEntries->result.flag = 0;
		gGetRspEntries->result.cause = 0;
		gGetRspEntries->no_list = 0;
		gGetRspEntries->type=3;
	}
	gGetRspEntries->more_data = 0;
	return (void *)gGetRspEntries;
}

void * DeleteAllCallsRsp(UINT32 nSimNum)
{
	MMI_PHB_DEL_LAST_NUMBER_REQ_STRUCT *delNoReq = (MMI_PHB_DEL_LAST_NUMBER_REQ_STRUCT*)gCallHistoryBuffer;
	
  printf(" ****DeleteAllCallsRsp******* " );
  
  if (nSimNum == SIM1)
  {
    if(delNoReq->type==0)
    {
      if (delNoReq->no_data == 1)
      {
        DialedCallCount--;
      }
      else
      {
        fpCallHistory = fopen("DialedCallHistory.txt", "w+");
        DialedCallCount = 0;
        fclose(fpCallHistory);	
      }
    }else if(delNoReq->type==1)
    {
      if (delNoReq->no_data == 1)
      {
        MissedCallCount--;
      }
      else
      {
        fpCallHistory = fopen("MissedCallHistory.txt", "w+");
        MissedCallCount = 0;
        fclose(fpCallHistory);	
      }
    }else if(delNoReq->type==2)
    {
      if (delNoReq->no_data == 1)
      {
        RecvdCallCount--;
      }
      else
      {
        fpCallHistory = fopen("RecvdCallHistory.txt", "w+");
        RecvdCallCount = 0;
        fclose(fpCallHistory);	
      }
    }
  }
  else if (nSimNum == SIM2)
  {
    if(delNoReq->type==0)
    {
      if (delNoReq->no_data == 1)
      {
        DialedCallCount--;
      }
      else
      {
        fpCallHistory = fopen("DialedCallHistory2.txt", "w+");
        DialedCallCount = 0;
        fclose(fpCallHistory);	
      }
    }else if(delNoReq->type==1)
    {
      if (delNoReq->no_data == 1)
      {
        MissedCallCount--;
      }
      else
      {
        fpCallHistory = fopen("MissedCallHistory2.txt", "w+");
        MissedCallCount = 0;
        fclose(fpCallHistory);	
      }
    }else if(delNoReq->type==2)
    {
      if (delNoReq->no_data == 1)
      {
        RecvdCallCount--;
      }
      else
      {
        fpCallHistory = fopen("RecvdCallHistory2.txt", "w+");
        RecvdCallCount = 0;
        fclose(fpCallHistory);	
      }
    }
  }

	gGetRspEntries = (GET_LAST_NUMBER_RSP_STRUCT *)
		CMemAllocator::MyMallocDataPtr(sizeof(GET_LAST_NUMBER_RSP_STRUCT)*MAX_CALL_HISTORY_ENTRIES);	
	memset( gGetRspEntries, 0,sizeof(GET_LAST_NUMBER_RSP_STRUCT ));
	return (void *)gGetRspEntries;
}

void CallHistoryWriteToFile(kal_uint16 FileType,UINT32 nSimNum) 
{
  S8 str[200];
  
  if (nSimNum == SIM1)
  {
    if(FileType==0)
    {
      fpCallHistory = fopen("DialedCallHistory.txt", "a+");
    }else if(FileType==1)
    {
      fpCallHistory = fopen("MissedCallHistory.txt", "a+");
    }else if(FileType==2)
    {
      fpCallHistory = fopen("RecvdCallHistory.txt", "a+");
    }
  }
  else if (nSimNum == SIM2)
  {
    if(FileType==0)
    {
      fpCallHistory = fopen("DialedCallHistory2.txt", "a+");
    }else if(FileType==1)
    {
      fpCallHistory = fopen("MissedCallHistory2.txt", "a+");
    }else if(FileType==2)
    {
      fpCallHistory = fopen("RecvdCallHistory2.txt", "a+");
    }
  }

	
	if(fpCallHistory == NULL) {
		exit(0);
	}
	sprintf(str, "%d,%d,%d,%d,%d,%d,%d,%s,%d,%d,%s,%d,%d,%d,%d,%d,%d,%d,%d\n",
		gSetRspStruct->ref_count,
		gSetRspStruct->msg_len,
		gSetRspStruct->result.flag,
		gSetRspStruct->result.cause,
		gSetRspStruct->no_list,
		gSetRspStruct->list[0].tel.type,
		gSetRspStruct->list[0].tel.length,
		gSetRspStruct->list[0].tel.number,
		gSetRspStruct->list[0].alpha_id.name_length,
		gSetRspStruct->list[0].alpha_id.name_dcs,
		gSetRspStruct->list[0].alpha_id.name,
		gSetRspStruct->list[0].time.rtc_sec,
		gSetRspStruct->list[0].time.rtc_min,
		gSetRspStruct->list[0].time.rtc_hour,
		gSetRspStruct->list[0].time.rtc_day,
		gSetRspStruct->list[0].time.rtc_mon,
		gSetRspStruct->list[0].time.rtc_wday,
		gSetRspStruct->list[0].time.rtc_year,
		gSetRspStruct->list[0].count);
	
	fputs(str, fpCallHistory);
	fclose(fpCallHistory);
}

void CallHistoryReadFromFile(kal_uint16 FileType,UINT32 nSimNum)
{
	S8 str[100];
	S8 *token;
	S8 index = 0;

  if (nSimNum == SIM1)
  {
    if(FileType==0)
    {
      fpCallHistory = fopen("DialedCallHistory.txt", "r");
    }else if(FileType==1)
    {
      fpCallHistory = fopen("MissedCallHistory.txt", "r");
    }else if(FileType==2)
    {
      fpCallHistory = fopen("RecvdCallHistory.txt", "r");
    }
  }
  else if (nSimNum == SIM2)
  {
    if(FileType==0)
    {
      fpCallHistory = fopen("DialedCallHistory2.txt", "r");
    }else if(FileType==1)
    {
      fpCallHistory = fopen("MissedCallHistory2.txt", "r");
    }else if(FileType==2)
    {
      fpCallHistory = fopen("RecvdCallHistory2.txt", "r");
    }
  }


	if(fpCallHistory == NULL) 
	{
		gGetRspEntries = (GET_LAST_NUMBER_RSP_STRUCT *)
			CMemAllocator::MyMallocDataPtr(sizeof(GET_LAST_NUMBER_RSP_STRUCT)*MAX_CALL_HISTORY_ENTRIES);	
		gGetRspEntries->ref_count = 0;
	 	gGetRspEntries->msg_len = 91;
		gGetRspEntries->result.flag = 0;
		gGetRspEntries->result.cause = 0;
		gGetRspEntries->no_list = index;
		if(FileType==0)
		{
			gGetRspEntries->type=0;
		}
		else if(FileType==1)
		{
			gGetRspEntries->type=1;
		}
		else if(FileType==2)
		{
			gGetRspEntries->type=2;
		}
		return;
	}else
	{
		gGetRspEntries = (GET_LAST_NUMBER_RSP_STRUCT *)
			CMemAllocator::MyMallocDataPtr(sizeof(GET_LAST_NUMBER_RSP_STRUCT)*MAX_CALL_HISTORY_ENTRIES);	
		
		while (fgets(str,91,fpCallHistory)) {
			str[strlen(str) - 1]='\0';
			token = strtok(str, ",");
			gGetRspEntries->ref_count = atoi(token);
			token = strtok(NULL, ",");
	 		gGetRspEntries->msg_len = atoi(token);
			token = strtok(NULL, ",");
			gGetRspEntries->result.flag = atoi(token);
			token = strtok(NULL, ",");
			gGetRspEntries->result.cause = atoi(token);
			token = strtok(NULL, ",");
	 		//gSetGetRspEntries->no_list = atoi(token);
			token = strtok(NULL, ",");
			gGetRspEntries->list[index].tel.type = atoi(token);
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].tel.length = atoi(token);
			token = strtok(NULL, ",");
			if(!strcmp(token,"NULL"))
				gGetRspEntries->list[index].tel.number[0] ='\0';
			else
				strcpy((S8 *)gGetRspEntries->list[index].tel.number, token);
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].alpha_id.name_length = atoi(token);			
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].alpha_id.name_dcs = atoi(token);
			token = strtok(NULL, ",");
			if(!strcmp(token,"NULL"))
				gGetRspEntries->list[index].alpha_id.name[0] ='\0';
			else
				strcpy((S8 *)gGetRspEntries->list[index].alpha_id.name, token);
		
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].time.rtc_sec = atoi(token);
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].time.rtc_min = atoi(token);
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].time.rtc_hour = atoi(token);
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].time.rtc_day = atoi(token);
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].time.rtc_mon = atoi(token);
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].time.rtc_wday = atoi(token);
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].time.rtc_year = atoi(token);
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].count = atoi(token);		
			index++;
		}
		gGetRspEntries->no_list = index;
		if(FileType==0)
		{
			gGetRspEntries->type=0;
		}
		else if(FileType==1)
		{
			gGetRspEntries->type=1;
		}
		else if(FileType==2)
		{
			gGetRspEntries->type=2;
		}
	}
	fclose(fpCallHistory);
}

void * IncomingCallInd(void)
{
	static kal_uint16 callId=1;

	printf(" ****IncomingCallInd******* " );
	
	gIncomingCallIndStruct = (MMI_CC_CALL_RING_IND_STRUCT *)
		CMemAllocator::MyMallocDataPtr(sizeof(MMI_CC_CALL_RING_IND_STRUCT));

	gIncomingCallIndStruct->ref_count=0;
	gIncomingCallIndStruct->msg_len=200;
	gIncomingCallIndStruct->call_id=(kal_uint8)callId;
	gIncomingCallIndStruct->num.type=0;
	gIncomingCallIndStruct->num.length=6;
	strcpy(gIncomingCallIndStruct->num.number,"9810098100");
	gIncomingCallIndStruct->sub_addr.type=0;
	gIncomingCallIndStruct->sub_addr.length=0;
	strcpy(gIncomingCallIndStruct->sub_addr.number,"\0");
	strcpy(gIncomingCallIndStruct->name,"Shweta");
	gIncomingCallIndStruct->auto_answer=0;
	
	callId++;
	return (void *)gIncomingCallIndStruct;
}

