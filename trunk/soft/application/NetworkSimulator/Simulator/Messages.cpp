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
// Messages.cpp: implementation of the SMS Message Req Handling.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Messages.h"
#include "stdio.h"
#include "MemAllocator.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"

U8 voicemailcount = 0;
static unreadCount = 0;
static mmi_call_price_unit_req_struct PriceUnitData;

U8 LastListReq = 0;		// 1 For Inbox,	2 For Outbox
U16 ReferenceNumber = 1;
struct InBoxStructType
{	
	smsal_inbox_struct listInfo;
	mmi_sms_get_msg_rsp_struct message;
	char  messageSpace[160];
}InboxMsgStruct;

struct 
{
	U8 count;
	struct InBoxStructType inBoxStruct[MAX_MESSAGES_LIMIT];
}inbox;

//abhi:start:voicemail box structure
struct
{
	U8 count;
	mmi_sms_get_mailbox_address_rsp_struct vmbox; 
}voicemailbox;
//abhi:End

struct OutBoxStructType
{	
	smsal_outbox_struct listInfo;
	mmi_sms_get_msg_rsp_struct message;
	char  messageSpace[160];
}outboxMsgStruct;

struct OutBoxType 
{
	U8 count;
	struct OutBoxStructType outBoxStruct[MAX_MESSAGES_LIMIT];
}outbox;

smsal_prefer_storage_struct gMemoryStatus = { 0 , 1, 2, 4, 5, 6, 20, 20, 20 }; //we can store total 20 messages in inbox+outbox

void *gSMSBuffer;


void *SendSMSResponse()
{
	mmi_sms_send_msg_req_struct	* SendSMSReq = (mmi_sms_send_msg_req_struct *)gSMSBuffer;

	mmi_sms_send_msg_rsp_struct	* SendSMSRsp = (mmi_sms_send_msg_rsp_struct	* )
					CMemAllocator::MyMallocDataPtr(sizeof(*SendSMSRsp));
/* Start : JP : 20050515 */
	SendSMSRsp->result = (kal_bool)TRUE;
	SendSMSRsp->cause = 8;
	SendSMSRsp->mr = 0;
	SendSMSRsp->msg_len = 0;
	SendSMSRsp->ref_count = 0;
	strcpy((S8*)SendSMSRsp->scts,(S8*)"0000000");
	SendSMSRsp->seq_num = 0;
/* End : JP : 20050515 */
	return (void *)SendSMSRsp;
}

void *SendMessageList()
{
	mmi_sms_get_msg_list_req_struct *SMSListReq = ( mmi_sms_get_msg_list_req_struct * )gSMSBuffer;
	mmi_sms_get_msg_list_rsp_struct *ListResponse = (mmi_sms_get_msg_list_rsp_struct *)
			CMemAllocator::MyMallocDataPtr(sizeof(*ListResponse));
	memset( ListResponse, 0, sizeof(mmi_sms_get_msg_list_rsp_struct) );
//	Code Start For Inbox List 
	if( SMSListReq->status == 5 )
	{
		readSMSFile(INBOX_FILE_INDEX);
		LastListReq = 1;
		ListResponse->inbox.msg_count = SMSListReq->no_msg;//inbox.count;
		//for( U8 i = 0 ; i<inbox.count; i++ )
		for( kal_uint16 i = SMSListReq->start_index, j=0 ; i<SMSListReq->start_index+SMSListReq->no_msg; i++,j++ )
		{
			ListResponse->inbox.inbox_list[j].index = i;
			ListResponse->inbox.inbox_list[j].status = inbox.inBoxStruct[i].listInfo.status;
			ListResponse->inbox.inbox_list[j].storage_type = 1;
			memcpy( &ListResponse->inbox.inbox_list[j].oa.number, &inbox.inBoxStruct[i].listInfo.oa.number,
							sizeof(l4c_number_struct) );
			memcpy( &ListResponse->inbox.inbox_list[j].concat_info,
					&inbox.inBoxStruct[i].listInfo.concat_info,
					sizeof(smsal_concat_struct) );
			memcpy( &ListResponse->inbox.inbox_list[j].scts,
					&inbox.inBoxStruct[i].listInfo.scts,
					sizeof(ListResponse->inbox.inbox_list[j].scts) );
		}
	}
	else if( SMSListReq->status == 6 )
	{
		readSMSFile(OUTBOX_FILE_INDEX);
		LastListReq = 2;
		ListResponse->outbox.msg_count = SMSListReq->no_msg;//outbox.count;
		//for( U8 i = 0 ; i<outbox.count; i++ )
		for( kal_uint16 i = SMSListReq->start_index,j=0 ; i<SMSListReq->start_index+SMSListReq->no_msg; i++,j++ )
		{
			ListResponse->outbox.outbox_list[j].index = i;
			ListResponse->outbox.outbox_list[j].status = outbox.outBoxStruct[i].listInfo.status;
			ListResponse->outbox.outbox_list[j].storage_type = 1;
			memcpy( &ListResponse->outbox.outbox_list[j].da.number,
					&outbox.outBoxStruct[i].listInfo.da.number,
							sizeof(l4c_number_struct) );
			memcpy( &ListResponse->outbox.outbox_list[j].concat_info,
					&outbox.outBoxStruct[i].listInfo.concat_info,
					sizeof(smsal_concat_struct) );
		}
	}
	return (void *)ListResponse;
}


void SaveVoiceMailBoxNumbers()
{
	char vmdata1[]="VoiceMailNumber1";
	char vmnumber1[]="11111";

	char vmdata2[]="VoiceMailNumber2";
	char vmnumber2[]="22222";

	mmi_sms_get_mailbox_address_rsp_struct *VoicemailGetRsp= & voicemailbox.vmbox;
	
	memset(& voicemailbox.vmbox,0,sizeof(voicemailbox.vmbox));

	voicemailbox.count=1; //abhi:

	VoicemailGetRsp->result = KAL_TRUE;
	VoicemailGetRsp->mailbox_num = 2;

	VoicemailGetRsp->mailbox_info[0].name.name_length = strlen(vmdata1);
	VoicemailGetRsp->mailbox_info[0].name.name_dcs = 0;
	strcpy((char*)VoicemailGetRsp->mailbox_info[0].name.name,vmdata1);
	
	VoicemailGetRsp->mailbox_info[0].number_info.type = 145;
	strcpy((char*)VoicemailGetRsp->mailbox_info[0].number_info.number,vmnumber1);
	VoicemailGetRsp->mailbox_info[0].number_info.length= strlen(vmnumber1);

	VoicemailGetRsp->mailbox_info[1].name.name_length = strlen(vmdata2);
	VoicemailGetRsp->mailbox_info[1].name.name_dcs = 0;
	strcpy((char*)VoicemailGetRsp->mailbox_info[1].name.name,vmdata2);
	
	VoicemailGetRsp->mailbox_info[1].number_info.type = 129;
	strcpy((char*)VoicemailGetRsp->mailbox_info[1].number_info.number,vmnumber2);
	VoicemailGetRsp->mailbox_info[1].number_info.length= strlen(vmnumber2);


	memcpy (&voicemailbox.vmbox,VoicemailGetRsp,sizeof(mmi_sms_get_mailbox_address_rsp_struct));

	WriteVoiceMailFile();

}

void *SendGetVoicemailAddressRsp()
{
	mmi_sms_get_mailbox_address_rsp_struct *VoicemailGetRsp= (mmi_sms_get_mailbox_address_rsp_struct *)
		CMemAllocator::MyMallocDataPtr(sizeof(*VoicemailGetRsp));

	memset(VoicemailGetRsp,0,sizeof(*VoicemailGetRsp));
	ReadVoiceMailFile();
	memcpy(VoicemailGetRsp,&voicemailbox.vmbox,sizeof(*VoicemailGetRsp));
	
	VoicemailGetRsp->result= KAL_TRUE; // for ok 0 for fail 1;
	
	voicemailcount++;

	return (void*)VoicemailGetRsp; 

}
void *SendSetVoicemailAddressRsp()
{
	mmi_sms_set_mailbox_address_req_struct *VoicemailSetReq = (mmi_sms_set_mailbox_address_req_struct *)gSMSBuffer;

	mmi_sms_set_mailbox_address_rsp_struct *VoicemailSetRsp = (mmi_sms_set_mailbox_address_rsp_struct *)
		CMemAllocator::MyMallocDataPtr(sizeof(mmi_sms_set_mailbox_address_rsp_struct));

	mmi_sms_get_mailbox_address_rsp_struct *VoicemailGetRsp= (mmi_sms_get_mailbox_address_rsp_struct *)
		CMemAllocator::MyMallocDataPtr(sizeof(mmi_sms_get_mailbox_address_rsp_struct));


	memset(VoicemailSetRsp,0,sizeof(mmi_sms_set_mailbox_address_rsp_struct));
	memset(VoicemailGetRsp,0,sizeof(*VoicemailGetRsp));

	ReadVoiceMailFile();
	memcpy(VoicemailGetRsp,&voicemailbox.vmbox,sizeof(*VoicemailGetRsp));

	if(VoicemailSetReq->mailbox_num == 0)
	{
		VoicemailGetRsp->mailbox_info[0].number_info.type = VoicemailSetReq->num_addr.type;
    	memcpy(VoicemailGetRsp->mailbox_info[0].number_info.number,VoicemailSetReq->num_addr.number,VoicemailSetReq->num_addr.length);
		VoicemailGetRsp->mailbox_info[0].number_info.length= VoicemailSetReq->num_addr.length;
	}

	if(VoicemailSetReq->mailbox_num == 1) 
	{
		VoicemailGetRsp->mailbox_info[1].number_info.type = VoicemailSetReq->num_addr.type;
		memcpy(VoicemailGetRsp->mailbox_info[1].number_info.number,VoicemailSetReq->num_addr.number,VoicemailSetReq->num_addr.length);
		VoicemailGetRsp->mailbox_info[1].number_info.length= VoicemailSetReq->num_addr.length;
	}
	
	memcpy (&voicemailbox.vmbox,VoicemailGetRsp,sizeof(mmi_sms_get_mailbox_address_rsp_struct));
	WriteVoiceMailFile();

	VoicemailSetRsp->result = KAL_TRUE;
	VoicemailSetRsp->cause = 0;

	return (void *)VoicemailSetRsp;
}

//abhi:End


void *GetMemoryStorage()
{
	mmi_sms_get_preferred_storage_rsp_struct *MemoryRsp = (	mmi_sms_get_preferred_storage_rsp_struct *)CMemAllocator::MyMallocDataPtr(sizeof(mmi_sms_get_preferred_storage_rsp_struct));
	memset( MemoryRsp, 0, sizeof(mmi_sms_get_preferred_storage_rsp_struct));
	MemoryRsp->result = KAL_TRUE;
	memcpy( &MemoryRsp->info, &gMemoryStatus, sizeof(smsal_prefer_storage_struct));
	//MemoryRsp->info.used2 = 0;
	readSMSFile(INBOX_FILE_INDEX);
	readSMSFile(OUTBOX_FILE_INDEX);
	//abhi
	if (gMemoryStatus.mem2 == 0) //for SIM
		MemoryRsp->info.used2 = 0;
	else 
		MemoryRsp->info.used2 = inbox.count + outbox.count;  
/*	MemoryRsp->info.mem1 = 0;			MemoryRsp->info.used1=4;		MemoryRsp->info.total1 = 10;
	MemoryRsp->info.mem2 = 1;			MemoryRsp->info.used2=5;		MemoryRsp->info.total2 = 10;
	MemoryRsp->info.mem3 = 2;			MemoryRsp->info.used3=6;		MemoryRsp->info.total3 = 20;*/
	return (void*) MemoryRsp;
}	

void *SetMemoryStorage()
{
	mmi_sms_set_preferred_storage_req_struct *SetMemoryReq = (mmi_sms_set_preferred_storage_req_struct *)gSMSBuffer;
	mmi_sms_set_preferred_storage_rsp_struct *SetMemoryRsp = (mmi_sms_set_preferred_storage_rsp_struct *)CMemAllocator::MyMallocDataPtr(sizeof(mmi_sms_set_preferred_storage_rsp_struct));
	memset( SetMemoryRsp, 0, sizeof(mmi_sms_set_preferred_storage_rsp_struct) );
	gMemoryStatus.mem1 = SetMemoryReq->mem1;	gMemoryStatus.mem2 = SetMemoryReq->mem2;	gMemoryStatus.mem3 = SetMemoryReq->mem3;
	SetMemoryRsp->result = KAL_TRUE;	/* JP Added for PC Simulator 20050516 */
	SetMemoryRsp->param = gMemoryStatus ;
	return (void*)SetMemoryRsp;
}

void SetBufferForMessage( void * dataBuf )
{
	gSMSBuffer = dataBuf ;
}

void writeSMSFile(U8 flag)	// flag = 0 for inbox, flag = 1 for outbox
{
	FILE *fp;
	if( flag == INBOX_FILE_INDEX )
	{
		fp = fopen( INBOX_FILE, "wb" );
		fwrite(&inbox, sizeof(inbox), 1, fp );
	}
	else if( flag == OUTBOX_FILE_INDEX )
	{
		fp = fopen( OUTBOX_FILE, "wb" );
		fwrite(&outbox, sizeof(outbox), 1, fp );
	}
	if( fp != NULL )fclose(fp);
}

void readSMSFile(U8 flag)	// flag = 0 for inbox, flag = 1 for outbox
{
	FILE *fp;
	if( flag == INBOX_FILE_INDEX )
	{
		if( (fp = fopen( INBOX_FILE, "rb" )) == NULL )
		{
			//exit(0);
		}
		else
		{
			U8 i;

			unreadCount = 0;
			fread(&inbox, sizeof(inbox), 1, fp );
			for (i = 0; i < inbox.count; i++)
			{
				if (inbox.inBoxStruct[i].listInfo.status == 0)
					unreadCount++;
			}
		}
	}
	else if( flag == OUTBOX_FILE_INDEX )
	{
		if( (fp = fopen( OUTBOX_FILE, "rb" )) == NULL )
		{
			//exit(0);
		}
		else fread(&outbox, sizeof(outbox), 1, fp );
	}
	if( fp != NULL )fclose(fp);
}//abhi:Start:editing of voicemail numbers
void ReadVoiceMailFile()
{
	FILE *fp;
	if( (fp = fopen( "voicemail.txt", "rb" )) == NULL )
		SaveVoiceMailBoxNumbers();
	else 
	{
		fread(&voicemailbox, sizeof(voicemailbox), 1, fp );
		fclose(fp);
}
}
void WriteVoiceMailFile()
{
	FILE *fp;

	fp = fopen( "voicemail.txt", "wb" );
	
	fwrite(&voicemailbox, sizeof(voicemailbox), 1, fp );

	if( fp != NULL )fclose(fp);
}
//abhi:End

////////////////////////////////////////////////////////////
///////////New SMS Box Implementation///////////////////////
////////////////////////////////////////////////////////////
sms_box_struct sms_box;

void write_sms_box_file()
{
	FILE *fp;
	fp = fopen( SMS_BOX_FILE, "wb" );
	if(fp!=NULL)
	{
		fwrite(&sms_box, sizeof(sms_box), 1, fp );
		fclose(fp);
	}
}
void read_sms_box_file()
{
	FILE *fp;
	fp = fopen( SMS_BOX_FILE, "rb" );
	if(fp!=NULL)
	{
		fread(&sms_box, sizeof(sms_box), 1, fp );
		fclose(fp);
	}
	else
	{
		U8 i;
		memset(&sms_box,0,sizeof(sms_box));
		for(i=0;i<MAX_SMS_BOX_LIMIT*2;i++)
			sms_box.sms_box_entries[i].status=0xff;
		write_sms_box_file();
	}
}

void* SMSBootUpBeginInd(UINT32 nSimNum)
{
	read_sms_box_file();
	return NULL;
}

void* SMSSendBootupReadInd(UINT32 nSimNum)
{
#if 1 /*_DUAL_SIM_*/
  sms_box_entry *entry = NULL;
  int nTempIndex = 0;

  static int entries_sent[2]={0,0};///count of entries sent till now
  static int last_index_sent[2]={0,MAX_SMS_BOX_LIMIT};
	mmi_sms_startup_read_msg_ind_struct *SMSBootUpReadInd= 
		(mmi_sms_startup_read_msg_ind_struct*)CMemAllocator::MyMallocDataPtr(sizeof(mmi_sms_startup_read_msg_ind_struct));

	memset( SMSBootUpReadInd, 0, sizeof(mmi_sms_startup_read_msg_ind_struct));

#ifdef _DUAL_SIM_
  if (nSimNum == SIM2)
  {
    if(entries_sent[1]+1>=sms_box.total_box_entries[1])
      SMSBootUpReadInd->startup_seq_num=1;///send it 1.. so that mmi does not send ReadNext request
    ////for last entry or no entry case
    
    if(entries_sent[1]<sms_box.total_box_entries[1])
    {
      //fil the valid entries
      for(;(last_index_sent[1]<MAX_SMS_BOX_LIMIT*2)
        &&(sms_box.sms_box_entries[last_index_sent[1]].status==0xff)
        ;last_index_sent[1]++);
      if(last_index_sent[1]<MAX_SMS_BOX_LIMIT*2)
      {
        entry=&(sms_box.sms_box_entries[last_index_sent[1]]);
        nTempIndex = last_index_sent[1]-MAX_SMS_BOX_LIMIT+CM_SMS_MAX_MSG_NUM;
        entries_sent[1]++;
        last_index_sent[1]++;
      }
    }
  }
  else
#endif /*#ifdef _DUAL_SIM_*/
  {
    if(entries_sent[0]+1>=sms_box.total_box_entries[0])
      SMSBootUpReadInd->startup_seq_num=1;///send it 1.. so that mmi does not send ReadNext request
    ////for last entry or no entry case
    
    if(entries_sent[0]<sms_box.total_box_entries[0])
    {
      //fil the valid entries
      for(;(last_index_sent[0]<MAX_SMS_BOX_LIMIT)
        &&(sms_box.sms_box_entries[last_index_sent[0]].status==0xff)
        ;last_index_sent[0]++);
      if(last_index_sent[0]<MAX_SMS_BOX_LIMIT)
      {
        entry=&(sms_box.sms_box_entries[last_index_sent[0]]);
        nTempIndex = last_index_sent[0];
        entries_sent[0]++;
        last_index_sent[0]++;
      }
    }
  }

  SMSBootUpReadInd->status=0xff;

  if (entry)
  {
    memcpy(&SMSBootUpReadInd->addr_number,&entry->addr_number,sizeof(l4c_number_struct));
    SMSBootUpReadInd->concat_info=entry->concat_info;
    SMSBootUpReadInd->dcs=entry->dcs;
    memcpy(SMSBootUpReadInd->dt,entry->dt,sizeof(entry->dt));
    SMSBootUpReadInd->fo=entry->fo;
    SMSBootUpReadInd->index=nTempIndex;
    SMSBootUpReadInd->mr=entry->mr;
    SMSBootUpReadInd->msg_data[0]=0;
    SMSBootUpReadInd->msg_len=0;
    SMSBootUpReadInd->mti=entry->mti;
    SMSBootUpReadInd->no_msg_data=0;
    SMSBootUpReadInd->ori_dcs=entry->ori_dcs;
    SMSBootUpReadInd->pid=entry->pid;
    memcpy(SMSBootUpReadInd->scts,entry->scts,sizeof(entry->scts));
    SMSBootUpReadInd->st=entry->st;
    SMSBootUpReadInd->status=entry->status;
    SMSBootUpReadInd->vp=entry->vp;
  }

	return SMSBootUpReadInd;
#else
	static entries_sent=0;///count of entries sent till now
	static last_index_sent=0;
	mmi_sms_startup_read_msg_ind_struct *SMSBootUpReadInd= 
		(mmi_sms_startup_read_msg_ind_struct*)CMemAllocator::MyMallocDataPtr(sizeof(mmi_sms_startup_read_msg_ind_struct));

	memset( SMSBootUpReadInd, 0, sizeof(mmi_sms_startup_read_msg_ind_struct));

	if(entries_sent+1>=sms_box.total_box_entries[0])
		SMSBootUpReadInd->startup_seq_num=1;///send it 1.. so that mmi does not send ReadNext request
											////for last entry or no entry case
	if(entries_sent<sms_box.total_box_entries[0])
	{//fil the valid entries
		for(;(last_index_sent<MAX_SMS_BOX_LIMIT)
			&&(sms_box.sms_box_entries[last_index_sent].status==0xff)
			;last_index_sent++);
		if(last_index_sent<MAX_SMS_BOX_LIMIT)
		{
			sms_box_entry *entry=&(sms_box.sms_box_entries[last_index_sent]);
			memcpy(&SMSBootUpReadInd->addr_number,&entry->addr_number,sizeof(l4c_number_struct));
			SMSBootUpReadInd->concat_info=entry->concat_info;
			SMSBootUpReadInd->dcs=entry->dcs;
			memcpy(SMSBootUpReadInd->dt,entry->dt,sizeof(entry->dt));
			SMSBootUpReadInd->fo=entry->fo;
			SMSBootUpReadInd->index=last_index_sent;
			SMSBootUpReadInd->mr=entry->mr;
			SMSBootUpReadInd->msg_data[0]=0;
			SMSBootUpReadInd->msg_len=0;
			SMSBootUpReadInd->mti=entry->mti;
			SMSBootUpReadInd->no_msg_data=0;
			SMSBootUpReadInd->ori_dcs=entry->ori_dcs;
			SMSBootUpReadInd->pid=entry->pid;
			memcpy(SMSBootUpReadInd->scts,entry->scts,sizeof(entry->scts));
			SMSBootUpReadInd->st=entry->st;
			SMSBootUpReadInd->status=entry->status;
			SMSBootUpReadInd->vp=entry->vp;
			entries_sent++;
			last_index_sent++;
		}
		else
			SMSBootUpReadInd->status=0xff;
	}
	else
		SMSBootUpReadInd->status=0xff;
	return SMSBootUpReadInd;
#endif
}

void *SaveSMSMessage(UINT32 nSimNum)
{
#if 1 /*_DUAL_SIM_*/
  U8 free_index=0;
  sms_box_entry *entry = NULL;

	mmi_sms_set_msg_req_struct *SMSSetReq = ( mmi_sms_set_msg_req_struct * )gSMSBuffer;
	SYSTEMTIME  sysTime;
	//struct OutBoxStructType outBoxStruct ;

	mmi_sms_set_msg_rsp_struct *SMSSaveRsp = (mmi_sms_set_msg_rsp_struct *)CMemAllocator::MyMallocDataPtr(sizeof(mmi_sms_set_msg_rsp_struct));
	memset( SMSSaveRsp, 0, sizeof(mmi_sms_set_msg_rsp_struct));

  SMSSaveRsp->result= KAL_FALSE;
	SMSSaveRsp->cause = 322;

#ifdef _DUAL_SIM_
  if (nSimNum == SIM2)
  {
    free_index = MAX_SMS_BOX_LIMIT;
    if( sms_box.total_box_entries[1]< MAX_SMS_BOX_LIMIT*2)
    {
      // read from file
      while((free_index<MAX_SMS_BOX_LIMIT*2)
        &&(sms_box.sms_box_entries[free_index].status!=0xff))
        free_index++;
      if(free_index<MAX_SMS_BOX_LIMIT*2)
      {
        entry= & sms_box.sms_box_entries[free_index];
        sms_box.total_box_entries[1]++;
      }
    }
  }
  else
#endif /*#ifdef _DUAL_SIM_*/
  {
    free_index = 0;
    if( sms_box.total_box_entries[0]< MAX_SMS_BOX_LIMIT)
    {
      // read from file
      while((free_index<MAX_SMS_BOX_LIMIT)
        &&(sms_box.sms_box_entries[free_index].status!=0xff))
        free_index++;
      if(free_index<MAX_SMS_BOX_LIMIT)
      {
        entry= & sms_box.sms_box_entries[free_index];
        sms_box.total_box_entries[0]++;
      }
    }
  }

  if (entry)
  {
    U8 total_segs=0,seg=0;
    U16 ref=0;
    
    memcpy(&entry->addr_number,&SMSSetReq->dest,sizeof(SMSSetReq->dest));
    total_segs=SMSSetReq->udhi & 0x7;
    if(total_segs <= 1)
    {
      total_segs=1;
      seg=1;
      ref=0;
    }
    else
    {
      seg = ((SMSSetReq->udhi >> 3) & 0x7) + 1;
      if(seg == 1)
      {	
        ref = free_index;
        ReferenceNumber  = free_index;
      }
      else
        ref = ReferenceNumber;
    }
    entry->concat_info=0;
    entry->concat_info|=total_segs<<16;				
    entry->concat_info|=seg<<24;
    entry->concat_info|=ref;
    entry->dcs=SMSSetReq->dcs;
    memset(&entry->dt,0,sizeof(entry->dt));///CHECKME
    entry->fo=0;
    entry->length=SMSSetReq->length;
    entry->mr=0;
    memcpy(entry->msg,SMSSetReq->msg,sizeof(entry->msg));
    entry->mti=1;//SMSAL_MTI_SUBMIT; // Outbox message
    entry->ori_dcs=0;
    entry->pid=SMSSetReq->pid;
    memcpy(&entry->sca_number,&SMSSetReq->sc_addr,sizeof(SMSSetReq->sc_addr));
    
    GetLocalTime (&sysTime);
    entry->scts[2] = ((sysTime.wDay%10) << 4)|(sysTime.wDay/10);
    entry->scts[1] = ((sysTime.wMonth%10) << 4)|(sysTime.wMonth/10);
    entry->scts[0] = ((sysTime.wYear%10) << 4)|((sysTime.wYear/10)%10);
    
    entry->scts[3] = ((sysTime.wHour%10) << 4)|(sysTime.wHour/10);
    entry->scts[4] = ((sysTime.wMinute%10) << 4)|(sysTime.wMinute/10);
    entry->scts[5] = ((sysTime.wSecond%10) << 4)|(sysTime.wSecond/10);
    
    entry->st=0;
    entry->status=SMSSetReq->status;
    entry->vp=SMSSetReq->vp;
    write_sms_box_file();
    ///add to file
    SMSSaveRsp->result= KAL_TRUE;//return true
    SMSSaveRsp->cause = 0;
    SMSSaveRsp->index=free_index;
    SMSSaveRsp->status=SMSSetReq->status;
  }

	return SMSSaveRsp;
#else

  	mmi_sms_set_msg_req_struct *SMSSetReq = ( mmi_sms_set_msg_req_struct * )gSMSBuffer;
	SYSTEMTIME  sysTime;
	//struct OutBoxStructType outBoxStruct ;

	mmi_sms_set_msg_rsp_struct *SMSSaveRsp = (mmi_sms_set_msg_rsp_struct *)CMemAllocator::MyMallocDataPtr(sizeof(mmi_sms_set_msg_rsp_struct));
	memset( SMSSaveRsp, 0, sizeof(mmi_sms_set_msg_rsp_struct));

	if( sms_box.total_box_entries[0]< MAX_SMS_BOX_LIMIT)
	{
		// read from file
		U8 free_index=0;
		while((free_index<MAX_SMS_BOX_LIMIT)
			&&(sms_box.sms_box_entries[free_index].status!=0xff))
			free_index++;
		if(free_index<MAX_SMS_BOX_LIMIT)
		{
			sms_box_entry *entry= & sms_box.sms_box_entries[free_index];
			U8 total_segs=0,seg=0;
			U16 ref=0;

			memcpy(&entry->addr_number,&SMSSetReq->dest,sizeof(SMSSetReq->dest));
			total_segs=SMSSetReq->udhi & 0x7;
			if(total_segs <= 1)
			{
				total_segs=1;
				seg=1;
				ref=0;
			}
			else
			{
				seg = ((SMSSetReq->udhi >> 3) & 0x7) + 1;
				if(seg == 1)
				{	
					ref = free_index;
					ReferenceNumber  = free_index;
				}
				else
					ref = ReferenceNumber;
			}
			entry->concat_info=0;
			entry->concat_info|=total_segs<<16;				
			entry->concat_info|=seg<<24;
			entry->concat_info|=ref;
			entry->dcs=SMSSetReq->dcs;
			memset(&entry->dt,0,sizeof(entry->dt));///CHECKME
			entry->fo=0;
			entry->length=SMSSetReq->length;
			entry->mr=0;
			memcpy(entry->msg,SMSSetReq->msg,sizeof(entry->msg));
			entry->mti=1;//SMSAL_MTI_SUBMIT; // Outbox message
			entry->ori_dcs=0;
			entry->pid=SMSSetReq->pid;
			memcpy(&entry->sca_number,&SMSSetReq->sc_addr,sizeof(SMSSetReq->sc_addr));

			GetLocalTime (&sysTime);
			entry->scts[2] = ((sysTime.wDay%10) << 4)|(sysTime.wDay/10);
			entry->scts[1] = ((sysTime.wMonth%10) << 4)|(sysTime.wMonth/10);
			entry->scts[0] = ((sysTime.wYear%10) << 4)|((sysTime.wYear/10)%10);

			entry->scts[3] = ((sysTime.wHour%10) << 4)|(sysTime.wHour/10);
			entry->scts[4] = ((sysTime.wMinute%10) << 4)|(sysTime.wMinute/10);
			entry->scts[5] = ((sysTime.wSecond%10) << 4)|(sysTime.wSecond/10);

			entry->st=0;
			entry->status=SMSSetReq->status;
			entry->vp=SMSSetReq->vp;
			sms_box.total_box_entries[0]++;
			write_sms_box_file();
///add to file
			SMSSaveRsp->result= KAL_TRUE;//return true
			SMSSaveRsp->cause = 0;
			SMSSaveRsp->index=free_index;
			SMSSaveRsp->status=SMSSetReq->status;
		}
		else
		{
			SMSSaveRsp->result= KAL_FALSE;
			SMSSaveRsp->cause = 322;
		}
	}
	else
	{
		SMSSaveRsp->result= KAL_FALSE;
		SMSSaveRsp->cause = 322;
	}
	return SMSSaveRsp;
#endif
}

#define    dummy_sms_text	"Hello Friend - just a test message number .. "
#define    dummy_sms_addr   "9811111111111"

#define    dummy_sms_text2	"Hello Friend - just a test message number from sim2 .. "
#define    dummy_sms_addr2   "9822222222222"

void *SendSMSToMMI(UINT32 nSimNum)
{
#if 1 /*_DUAL_SIM_*/
  U8 free_index=0;
  sms_box_entry *entry = NULL;

	mmi_sms_deliver_msg_ind_struct	*msg_deliver_ind= (mmi_sms_deliver_msg_ind_struct*)
		CMemAllocator::MyMallocDataPtr(sizeof(mmi_sms_deliver_msg_ind_struct));
	int index = 0 ;
	SYSTEMTIME  sysTime;

//	#define    DummySMSSrc   "AirtelLive"
	//char mydata[160];
	//char mynumber[40];

	//save a new dummy message in InboxFile
	//get current Inbox entries

#ifdef _DUAL_SIM_
  if (nSimNum == SIM2)
  {
    if(sms_box.total_box_entries[1] < MAX_MESSAGES_LIMIT*2) 
    {
      // read from file
      free_index = MAX_SMS_BOX_LIMIT;
      while((free_index<MAX_SMS_BOX_LIMIT*2)
        &&(sms_box.sms_box_entries[free_index].status!=0xff))
        free_index++;
      if(free_index<MAX_SMS_BOX_LIMIT*2)
      {
        entry= &sms_box.sms_box_entries[free_index];
        sms_box.total_box_entries[1]++;
      }
    }
  }
  else
#endif /*#ifdef _DUAL_SIM_*/
  {
    if(sms_box.total_box_entries[0] < MAX_MESSAGES_LIMIT) 
    {
      // read from file
      free_index = 0;
      while((free_index<MAX_SMS_BOX_LIMIT)
        &&(sms_box.sms_box_entries[free_index].status!=0xff))
        free_index++;
      if(free_index<MAX_SMS_BOX_LIMIT)
      {
        entry= &sms_box.sms_box_entries[free_index];
        sms_box.total_box_entries[0]++;
      }
    }
  }
  
  if (entry)
  {
    U8 total_segs=1,seg=1;
    U16 ref=0;
    ///fill the Entry
#ifdef _DUAL_SIM_
    if (nSimNum == SIM2)
    {
      entry->addr_number.length=strlen(dummy_sms_addr2);
      entry->addr_number.type=145;//CSMCC_DEFAULT_ADDR_TYPE
      strcpy((char*)&entry->addr_number.number[0],dummy_sms_addr2);
    }
    else
#endif /*#ifdef _DUAL_SIM_*/
    {
      entry->addr_number.length=strlen(dummy_sms_addr);
      entry->addr_number.type=145;//CSMCC_DEFAULT_ADDR_TYPE
      strcpy((char*)&entry->addr_number.number[0],dummy_sms_addr);
    }
    entry->concat_info=0;
    entry->concat_info|=total_segs<<16;				
    entry->concat_info|=seg<<24;
    entry->concat_info|=ref;
    
    entry->dcs=0;//default dcs
    memset(&entry->dt,0,sizeof(entry->dt));
    entry->fo=0;
#ifdef _DUAL_SIM_
    if (nSimNum == SIM2)
    {
      entry->length=strlen(dummy_sms_text2);
      strcpy((char*)&entry->msg[0],dummy_sms_text2);
    }
    else
#endif /*#ifdef _DUAL_SIM_*/
    {
      entry->length=strlen(dummy_sms_text);
      strcpy((char*)&entry->msg[0],dummy_sms_text);
    }
    entry->mr=0;
    entry->mti=0;//SMSAL_MTI_DELIVER
    entry->ori_dcs-=0;
    entry->pid=0;
    memset(&entry->sca_number,0,sizeof(entry->sca_number));
    
    GetLocalTime (&sysTime);
    entry->scts[2] = ((sysTime.wDay%10) << 4)|(sysTime.wDay/10);
    entry->scts[1] = ((sysTime.wMonth%10) << 4)|(sysTime.wMonth/10);
    entry->scts[0] = ((sysTime.wYear%10) << 4)|((sysTime.wYear/10)%10);
    entry->scts[3] = ((sysTime.wHour%10) << 4)|(sysTime.wHour/10);
    entry->scts[4] = ((sysTime.wMinute%10) << 4)|(sysTime.wMinute/10);
    entry->scts[5] = ((sysTime.wSecond%10) << 4)|(sysTime.wSecond/10);
    
    entry->st=0;
    entry->status=0;//SMSAL_REC_UNREAD
    entry->vp=0;
    ///write to file
    write_sms_box_file();
    ////fill deliver struct
    msg_deliver_ind->concat_info=entry->concat_info;
    msg_deliver_ind->dcs=entry->dcs;
    msg_deliver_ind->dest_port=0;
    msg_deliver_ind->display_type=0;
    msg_deliver_ind->fo=entry->fo;
    msg_deliver_ind->index=free_index;
#ifdef _DUAL_SIM_
    if (nSimNum == SIM2)
    {
      msg_deliver_ind->index -= MAX_SMS_BOX_LIMIT;
      msg_deliver_ind->index += CM_SMS_MAX_MSG_NUM;
    }
#endif /*#ifdef _DUAL_SIM_*/
    msg_deliver_ind->msg_data[0]=0;
    msg_deliver_ind->mti=0;////SMSAL_MTI_DELIVER
    msg_deliver_ind->no_msg_data=0;
    memcpy(&msg_deliver_ind->oa_number,&entry->addr_number,sizeof(entry->addr_number));
    msg_deliver_ind->pid=entry->pid;
    memset(&msg_deliver_ind->sca_number,0,sizeof(msg_deliver_ind->sca_number));
    memcpy(&msg_deliver_ind->scts,&entry->scts,sizeof(entry->scts));
    msg_deliver_ind->src_port=0;
  }

	return (void*)msg_deliver_ind;
#else
  mmi_sms_deliver_msg_ind_struct	*msg_deliver_ind = NULL;

//	#define    DummySMSSrc   "AirtelLive"
	//char mydata[160];
	//char mynumber[40];

	//save a new dummy message in InboxFile
	//get current Inbox entries
	if(sms_box.total_box_entries[0]< MAX_MESSAGES_LIMIT) 
	{
		// read from file
		U8 free_index=0;
		while((free_index<MAX_SMS_BOX_LIMIT)
			&&(sms_box.sms_box_entries[free_index].status!=0xff))
			free_index++;
		if(free_index<MAX_SMS_BOX_LIMIT)
		{
      msg_deliver_ind= (mmi_sms_deliver_msg_ind_struct*)CMemAllocator::MyMallocDataPtr(sizeof(mmi_sms_deliver_msg_ind_struct));
      int index = 0 ;
      SYSTEMTIME  sysTime;

			sms_box_entry *entry= &sms_box.sms_box_entries[free_index];
			U8 total_segs=1,seg=1;
			U16 ref=0;
///fill the Entry
			entry->addr_number.length=strlen(dummy_sms_addr);
			entry->addr_number.type=145;//CSMCC_DEFAULT_ADDR_TYPE
			strcpy((char*)&entry->addr_number.number[0],dummy_sms_addr);

			entry->concat_info=0;
			entry->concat_info|=total_segs<<16;				
			entry->concat_info|=seg<<24;
			entry->concat_info|=ref;
			
			entry->dcs=0;//default dcs
			memset(&entry->dt,0,sizeof(entry->dt));
			entry->fo=0;
			entry->length=strlen(dummy_sms_text);
			strcpy((char*)&entry->msg[0],dummy_sms_text);
			entry->mr=0;
			entry->mti=0;//SMSAL_MTI_DELIVER
			entry->ori_dcs-=0;
			entry->pid=0;
			memset(&entry->sca_number,0,sizeof(entry->sca_number));

			GetLocalTime (&sysTime);
			entry->scts[2] = ((sysTime.wDay%10) << 4)|(sysTime.wDay/10);
			entry->scts[1] = ((sysTime.wMonth%10) << 4)|(sysTime.wMonth/10);
			entry->scts[0] = ((sysTime.wYear%10) << 4)|((sysTime.wYear/10)%10);
			entry->scts[3] = ((sysTime.wHour%10) << 4)|(sysTime.wHour/10);
			entry->scts[4] = ((sysTime.wMinute%10) << 4)|(sysTime.wMinute/10);
			entry->scts[5] = ((sysTime.wSecond%10) << 4)|(sysTime.wSecond/10);
			
			entry->st=0;
			entry->status=0;//SMSAL_REC_UNREAD
			entry->vp=0;
///write to file
			sms_box.total_box_entries[0]++;
			write_sms_box_file();
////fill deliver struct
			msg_deliver_ind->concat_info=entry->concat_info;
			msg_deliver_ind->dcs=entry->dcs;
			msg_deliver_ind->dest_port=0;
			msg_deliver_ind->display_type=0;
			msg_deliver_ind->fo=entry->fo;
			msg_deliver_ind->index=free_index;
			msg_deliver_ind->msg_data[0]=0;
			msg_deliver_ind->mti=0;////SMSAL_MTI_DELIVER
			msg_deliver_ind->no_msg_data=0;
			memcpy(&msg_deliver_ind->oa_number,&entry->addr_number,sizeof(entry->addr_number));
			msg_deliver_ind->pid=entry->pid;
			memset(&msg_deliver_ind->sca_number,0,sizeof(msg_deliver_ind->sca_number));
			memcpy(&msg_deliver_ind->scts,&entry->scts,sizeof(entry->scts));
			msg_deliver_ind->src_port=0;
		}
	}
	return (void*)msg_deliver_ind;

#endif
}

void *SendSMSContectByIndex(UINT32 nSimNum)
{
#if 1 /*_DUAL_SIM_*/
  sms_box_entry *entry = NULL;

  mmi_sms_get_msg_req_struct *smsGetReq = (mmi_sms_get_msg_req_struct *)gSMSBuffer;
  //mmi_sms_set_msg_req_struct *SMSSetReq;
  kal_uint16 index= (smsGetReq->index);
#ifdef _DUAL_SIM_
  if (nSimNum == SIM2)
  {
    index -= CM_SMS_MAX_MSG_NUM;
    index += MAX_SMS_BOX_LIMIT;

    if((index<MAX_SMS_BOX_LIMIT*2)
      &&(sms_box.sms_box_entries[index].status!=0xff))
    {
      entry=&(sms_box.sms_box_entries[index]);
    }
  }
  else
#endif /*_DUAL_SIM_*/
  {
    if((index<MAX_SMS_BOX_LIMIT)
      &&(sms_box.sms_box_entries[index].status!=0xff))
    {
      entry=&(sms_box.sms_box_entries[index]);
    }
  }

  mmi_sms_get_msg_rsp_struct *SMSGetResponse=
      (mmi_sms_get_msg_rsp_struct*)CMemAllocator::MyMallocDataPtr(sizeof(*SMSGetResponse)+160);

  SMSGetResponse->result=KAL_FALSE;

  if(entry)
  {
    memset (SMSGetResponse, 0, sizeof(*SMSGetResponse)+
      160);
    
    memcpy(&SMSGetResponse->addr_number,&entry->addr_number,sizeof(entry->addr_number));
    SMSGetResponse->cause=0;
    SMSGetResponse->dcs=entry->dcs;
    memcpy(&SMSGetResponse->dt,&entry->dt,sizeof(entry->dt));
    SMSGetResponse->fo=entry->fo;
    SMSGetResponse->mr=entry->mr;
    strncpy((char*)&SMSGetResponse->msg_data[0],(char*)&entry->msg[0],160);
    SMSGetResponse->no_msg_data=entry->length;
    SMSGetResponse->mti=entry->mti;
    SMSGetResponse->ori_dcs=entry->ori_dcs;
    SMSGetResponse->pid=entry->pid;
    SMSGetResponse->result=KAL_TRUE;
    memcpy(&SMSGetResponse->sca_number,&entry->sca_number,sizeof(entry->sca_number));
    memcpy(SMSGetResponse->scts,entry->scts,sizeof(entry->scts));
    SMSGetResponse->st=entry->st;
    if(smsGetReq->change_status==KAL_TRUE)
    {
      if(entry->status==0)//SMSAL_REC_UNREAD
        entry->status=1;//SMSAL_REC_READ
      write_sms_box_file();
    }
    SMSGetResponse->status=entry->status;
    SMSGetResponse->vp=entry->vp;
  }

  return (void*)SMSGetResponse;
#else
	mmi_sms_get_msg_req_struct *smsGetReq = (mmi_sms_get_msg_req_struct *)gSMSBuffer;
	//mmi_sms_set_msg_req_struct *SMSSetReq;
	kal_uint16 index= (smsGetReq->index);
		mmi_sms_get_msg_rsp_struct *SMSGetResponse=
			(mmi_sms_get_msg_rsp_struct*)
			CMemAllocator::MyMallocDataPtr(sizeof(*SMSGetResponse)+
			160);		
	if((index<MAX_SMS_BOX_LIMIT)
		&&(sms_box.sms_box_entries[index].status!=0xff))
	{
		
		sms_box_entry *entry=&(sms_box.sms_box_entries[index]);

		memset (SMSGetResponse, 0, sizeof(*SMSGetResponse)+
			160);

		memcpy(&SMSGetResponse->addr_number,&entry->addr_number,sizeof(entry->addr_number));
		SMSGetResponse->cause=0;
		SMSGetResponse->dcs=entry->dcs;
		memcpy(&SMSGetResponse->dt,&entry->dt,sizeof(entry->dt));
		SMSGetResponse->fo=entry->fo;
		SMSGetResponse->mr=entry->mr;
		strncpy((char*)&SMSGetResponse->msg_data[0],(char*)&entry->msg[0],160);
		SMSGetResponse->no_msg_data=entry->length;
		SMSGetResponse->mti=entry->mti;
		SMSGetResponse->ori_dcs=entry->ori_dcs;
		SMSGetResponse->pid=entry->pid;
		SMSGetResponse->result=KAL_TRUE;
		memcpy(&SMSGetResponse->sca_number,&entry->sca_number,sizeof(entry->sca_number));
		memcpy(SMSGetResponse->scts,entry->scts,sizeof(entry->scts));
		SMSGetResponse->st=entry->st;
		if(smsGetReq->change_status==KAL_TRUE)
		{
			if(entry->status==0)//SMSAL_REC_UNREAD
				entry->status=1;//SMSAL_REC_READ
			write_sms_box_file();
		}
		SMSGetResponse->status=entry->status;
		SMSGetResponse->vp=entry->vp;
	}
	else
	{
		SMSGetResponse->result=KAL_FALSE;
	}
	return (void*)SMSGetResponse;
#endif
}
U8 IsBitOnInBitmap(U8* bitmap, U8 index)
{
	U8 ret_val=0;
	U16  byte_index;
	U8   bit_index;

	byte_index = (index >> 3);  /* divide 8 */
	bit_index  = (index & 0x7);  /* reminder */

	if(bitmap[byte_index] & (1<<bit_index))
		ret_val=1;
	return ret_val;
}

void* DeleteSMSRequest(UINT32 nSimNum)
{
#if 1 /*_DUAL_SIM_*/
	mmi_sms_del_msg_req_struct * DelReq = ( mmi_sms_del_msg_req_struct *)gSMSBuffer;
	mmi_sms_del_msg_rsp_struct * DelRsp = (	mmi_sms_del_msg_rsp_struct *)CMemAllocator::MyMallocDataPtr(sizeof(mmi_sms_del_msg_rsp_struct ));
	if(DelReq->del_flag==7)//SMSAL_DEL_BITMAP
	{
		U8 i;
#ifdef _DUAL_SIM_
    if (nSimNum == SIM2)
    {
      for(i=MAX_SMS_BOX_LIMIT;i<MAX_SMS_BOX_LIMIT*2;i++)
      {
        if(IsBitOnInBitmap(DelReq->del_bitmap,i-MAX_SMS_BOX_LIMIT))
        {
          sms_box.total_box_entries[1]--;
          memset(&sms_box.sms_box_entries[i],0,sizeof(sms_box_entry));
          sms_box.sms_box_entries[i].status=0xff;
        }
      }
    }
    else
#endif /*#ifdef _DUAL_SIM_*/
    {
      for(i=0;i<MAX_SMS_BOX_LIMIT;i++)
      {
        if(IsBitOnInBitmap(DelReq->del_bitmap,i))
        {
          sms_box.total_box_entries[0]--;
          memset(&sms_box.sms_box_entries[i],0,sizeof(sms_box_entry));
          sms_box.sms_box_entries[i].status=0xff;
        }
      }
    }
		write_sms_box_file();
	}
	DelRsp->cause=0;
	memcpy(DelRsp->del_bitmap,DelReq->del_bitmap,480);
	DelRsp->del_flag=7;
	DelRsp->index=DelReq->index;
	DelRsp->result=KAL_TRUE;
	return (void *)DelRsp;
#else
  	mmi_sms_del_msg_req_struct * DelReq = ( mmi_sms_del_msg_req_struct *)gSMSBuffer;
	mmi_sms_del_msg_rsp_struct * DelRsp = (	mmi_sms_del_msg_rsp_struct *)CMemAllocator::MyMallocDataPtr(sizeof(mmi_sms_del_msg_rsp_struct ));
	if(DelReq->del_flag==7)//SMSAL_DEL_BITMAP
	{
		U8 i;
		for(i=0;i<MAX_SMS_BOX_LIMIT;i++)
		{
			if(IsBitOnInBitmap(DelReq->del_bitmap,i))
			{
				sms_box.total_box_entries[0]--;
				memset(&sms_box.sms_box_entries[i],0,sizeof(sms_box_entry));
				sms_box.sms_box_entries[i].status=0xff;
			}
		}
		write_sms_box_file();
	}
	DelRsp->cause=0;
	memcpy(DelRsp->del_bitmap,DelReq->del_bitmap,480);
	DelRsp->del_flag=7;
	DelRsp->index=DelReq->index;
	DelRsp->result=KAL_TRUE;
	return (void *)DelRsp;
#endif
}

#define SIM_INDEX_BEGIN 0
#define SIM_INDEX_END 39
#define ME_INDEX_BEGIN 40
#define ME_INDEX_END 49

void *SendSmsMemStatus()
{
	mmi_sms_get_msg_num_rsp_struct *get_msg_mem_num_resp= (mmi_sms_get_msg_num_rsp_struct *)CMemAllocator::MyMallocDataPtr(sizeof(mmi_sms_get_msg_num_rsp_struct));
	U8 i;
	memset( get_msg_mem_num_resp, 0, sizeof(mmi_sms_get_msg_num_rsp_struct) );
	for(i=SIM_INDEX_BEGIN;i<SIM_INDEX_END;i++)
	{
		switch(sms_box.sms_box_entries[i].status)
		{
		case 0://unread
		case 1://read
			get_msg_mem_num_resp->in_sim_no++;
			break;
		case 2://sent
		case 3://unsent
			get_msg_mem_num_resp->out_sim_no++;
			break;
		}
	}
	for(i=ME_INDEX_BEGIN;i<ME_INDEX_END;i++)
	{
		switch(sms_box.sms_box_entries[i].status)
		{
		case 0://unread
		case 1://read
			get_msg_mem_num_resp->in_me_no++;
			break;
		case 2://sent
		case 3://unsent
			get_msg_mem_num_resp->out_me_no++;
			break;
		}
	}
	get_msg_mem_num_resp->total_sim_num=SIM_INDEX_END-SIM_INDEX_BEGIN+1;
	get_msg_mem_num_resp->total_me_num=ME_INDEX_END-ME_INDEX_BEGIN+1;
	get_msg_mem_num_resp->result=KAL_TRUE;
	get_msg_mem_num_resp->is_sim_card_change=KAL_TRUE;
	return (void *)get_msg_mem_num_resp;
}

// Arvind Start: Added for Simulator Ver 2.5
/*****************************************************************************
* FUNCTION
*	ReadCallCostPriceUnitFile
* DESCRIPTION
*	read the Call Cost Price and Unit
* PARAMETERS
*	void 
* RETURNS
*	void
*****************************************************************************/
void ReadCallCostPriceUnitFile()
{
	FILE *fp;
	
	if( (fp = fopen( "PriceUnit.txt", "rb" )) != NULL )
	{
		fread(&PriceUnitData, sizeof(PriceUnitData), 1, fp );
	}

	if( fp != NULL )fclose(fp);

}
/*****************************************************************************
* FUNCTION
*	WriteCallCostPriceUnitFile
* DESCRIPTION
*	write the Call Cost Price and Unit
* PARAMETERS
*	void 
* RETURNS
*	void
*****************************************************************************/
void WriteCallCostPriceUnitFile()
{
	FILE *fp;

	if( (fp = fopen( "PriceUnit.txt", "wb" )) != NULL )
	{
		fwrite(&PriceUnitData, sizeof(PriceUnitData), 1, fp );
	}

	if( fp != NULL )fclose(fp);
}

/*****************************************************************************
* FUNCTION
*	SetCallPriceUnit
* DESCRIPTION
*	Set the Call Cost Price and Unit
* PARAMETERS
*	void 
* RETURNS
*	void*
*****************************************************************************/
void *SetCallPriceUnit(void)
{
	mmi_call_price_unit_req_struct *PriceUnitReq = ( mmi_call_price_unit_req_struct * )gSMSBuffer;
	mmi_sms_del_msg_rsp_struct *SetPriceUnitResponse = (mmi_sms_del_msg_rsp_struct *)
			CMemAllocator::MyMallocDataPtr(sizeof(mmi_sms_del_msg_rsp_struct));
	
	memset( SetPriceUnitResponse, 0, sizeof(SetPriceUnitResponse) );

	memcpy(&PriceUnitData,PriceUnitReq,sizeof(mmi_call_price_unit_req_struct));

	WriteCallCostPriceUnitFile();

	SetPriceUnitResponse->result = KAL_TRUE;
	
	return (void*)SetPriceUnitResponse;
}

/*****************************************************************************
* FUNCTION
*	GetCallPriceUnit
* DESCRIPTION
*	Get the Call Cost Price and Unit
* PARAMETERS
*	void 
* RETURNS
*	void*
*****************************************************************************/
void *GetCallPriceUnit(void)
{
	int i=0;

	mmi_call_price_unit_rsp_struct *GetPriceUnitResponse = (mmi_call_price_unit_rsp_struct *)
			CMemAllocator::MyMallocDataPtr(sizeof(mmi_call_price_unit_rsp_struct));


	memset( GetPriceUnitResponse, 0, sizeof(GetPriceUnitResponse) );

	ReadCallCostPriceUnitFile();

	GetPriceUnitResponse->result = 1;

	for(i=0;i<3;i++)
		GetPriceUnitResponse->currency[i] = PriceUnitData.currency[i];

	for(i=0;i<18;i++)
		GetPriceUnitResponse->ppu[i] = PriceUnitData.ppu[i];


	return (void*)GetPriceUnitResponse;
}

/*	For 1543 */
void *SendMessageNumber()
{
	static int smsNotReady;
	mmi_sms_get_msg_num_rsp_struct *ListResponse = (mmi_sms_get_msg_num_rsp_struct *)CMemAllocator::MyMallocDataPtr(sizeof(mmi_sms_get_msg_num_rsp_struct));
	memset( ListResponse, 0, sizeof(mmi_sms_get_msg_num_rsp_struct) );
	/*	Code Start For Inbox List */	
	readSMSFile(0);
	readSMSFile(1);

	ListResponse->unread_msg_num = unreadCount;
	ListResponse->is_sim_card_change = KAL_TRUE;
	ListResponse->in_sim_no = inbox.count;
	ListResponse->in_me_no = 0;
	ListResponse->out_me_no = 0;
	ListResponse->out_sim_no = outbox.count;
	ListResponse->total_sim_num = 10;
	ListResponse->total_me_num = 10;
/*	if(smsNotReady> 3)  // Delay for "SMS not Ready yet"
	{
	    ListResponse->result = KAL_TRUE;
		ListResponse->is_sms_ready = KAL_TRUE;
	}
	else
	{
		ListResponse->result = KAL_FALSE;
		ListResponse->is_sms_ready = KAL_FALSE;
		smsNotReady++;
	}
*/
    ListResponse->result = KAL_TRUE;	
	return (void *)ListResponse;
}

// Arvind End: Added for Simulator Ver 2.5
struct 
{
	kal_uint8	pid;
	kal_uint8	dcs;
	kal_uint8	vp;
	l4c_number_struct	sc_addr;
	l4_name_struct	profile_name;
} profile_setting_box;

void ReadProfileSettingsFile(void);
void WriteProfileSettingsInFile(void);
void SaveProfileSettings(void);

void *GetProfileParamsRsp(UINT32 nSimNum)
{
//	mmi_sms_get_profile_params_req_struct *ProfileParamReq = (mmi_sms_get_profile_params_req_struct *)gSMSBuffer;
	
	mmi_sms_get_profile_params_rsp_struct *ProfileParamRsp= (mmi_sms_get_profile_params_rsp_struct *)
	CMemAllocator::MyMallocDataPtr(sizeof(*ProfileParamRsp));
	memset(ProfileParamRsp,0,sizeof(*ProfileParamRsp));

  if (nSimNum == SIM2)
  {

  }
  else
  {
    
  }


	ReadProfileSettingsFile();
	ProfileParamRsp->result = KAL_TRUE;
	ProfileParamRsp->pid = profile_setting_box.pid;
	ProfileParamRsp->dcs = profile_setting_box.dcs;
	ProfileParamRsp->vp = profile_setting_box.vp;
	ProfileParamRsp->sc_addr.length = profile_setting_box.sc_addr.length;
	ProfileParamRsp->sc_addr.type = profile_setting_box.sc_addr.type;
	strcpy((char *)ProfileParamRsp->sc_addr.number, (char*)profile_setting_box.sc_addr.number);
	ProfileParamRsp->profile_name.name_length = profile_setting_box.profile_name.name_length;
	ProfileParamRsp->profile_name.name_dcs = profile_setting_box.profile_name.name_dcs;
	strcpy((char*)ProfileParamRsp->profile_name.name, (char*)profile_setting_box.profile_name.name);

	return (void*)ProfileParamRsp; 
}


void ReadProfileSettingsFile(void)
{
	FILE *fp = NULL;

	fp = fopen("profile_settings.txt", "rb");

	if(fp == NULL)
		SaveProfileSettings();
	else {
	
		fread(&profile_setting_box, sizeof(profile_setting_box), 1, fp);
		fclose(fp);
	}
}



void WriteProfileSettingsInFile(void)
{

	FILE *fp;

	fp = fopen( "profile_settings.txt", "wb" );
	fwrite(&profile_setting_box, sizeof(profile_setting_box), 1, fp );
	if( fp != NULL )fclose(fp);
}

void SaveProfileSettings(void)
{
	
	memset(&profile_setting_box, 0x00, sizeof(profile_setting_box));
	
	profile_setting_box.pid = 0;
	profile_setting_box.dcs = 0;
	profile_setting_box.vp = 0;
	profile_setting_box.sc_addr.type = 145;
	profile_setting_box.sc_addr.length = 10;
	strcpy((char*)profile_setting_box.sc_addr.number, (char*)"9812345678");
	profile_setting_box.profile_name.name_length = 8;
	profile_setting_box.profile_name.name_dcs = 0;
	strcpy((char*)profile_setting_box.profile_name.name, (char*)"Profile1");

	WriteProfileSettingsInFile();
}


void *SetProfileSettingParamsRsp(void)
{
	mmi_sms_set_profile_params_req_struct	* SetProfileReq = (mmi_sms_set_profile_params_req_struct *)gSMSBuffer;

	mmi_sms_set_profile_params_rsp_struct	* SetProfileRsp = (mmi_sms_set_profile_params_rsp_struct * )
					CMemAllocator::MyMallocDataPtr(sizeof(*SetProfileRsp));

	

	memset(SetProfileRsp, 0x00, sizeof(*SetProfileRsp));



	SetProfileRsp->result = KAL_TRUE;
	
	profile_setting_box.pid = SetProfileReq->pid;
	profile_setting_box.dcs = SetProfileReq->dcs;
	profile_setting_box.vp = SetProfileReq->vp;
	profile_setting_box.sc_addr.type = SetProfileReq->sc_addr.type;
	profile_setting_box.sc_addr.length = SetProfileReq->sc_addr.length;
	strcpy((char*)profile_setting_box.sc_addr.number, (char*)SetProfileReq->sc_addr.number);
	profile_setting_box.profile_name.name_length = SetProfileReq->profile_name.name_length;
	profile_setting_box.profile_name.name_dcs = SetProfileReq->profile_name.name_dcs;
	strcpy((char*)profile_setting_box.profile_name.name, (char*)SetProfileReq->profile_name.name);

	WriteProfileSettingsInFile();

	return (void *)SetProfileRsp;

}

void writeDefaultCBSettings()
{	
	l4c_cb_info_struct *cb_Struct = (l4c_cb_info_struct * )
					malloc(sizeof(l4c_cb_info_struct));
	
	memset((void*)cb_Struct,0,sizeof(l4c_cb_info_struct));
	cb_Struct->dcs[0]= 4; 
	cb_Struct->dcs[1]= 5;
	cb_Struct->dcs[2]= 7;
	cb_Struct->dcs[3]= 9;
	

	cb_Struct->dcs_mask[0] = 0;
	cb_Struct->dcs_mask[1] = 1;
	cb_Struct->dcs_mask[2] = 1;
	cb_Struct->dcs_mask[3] = 1;
	cb_Struct->is_all_language_on = KAL_FALSE;
	cb_Struct->is_cb_ready = KAL_TRUE;
	cb_Struct->is_sim_change = KAL_FALSE;

	cb_Struct->mid[0] = 1;
	cb_Struct->mid[1] = 2;
	cb_Struct->mid[2] = 3;
	cb_Struct->mid[3] = 4;

	cb_Struct->mid_mask[0] = 0;
	cb_Struct->mid_mask[1] = 1;
	cb_Struct->mid_mask[2] = 1;
	cb_Struct->mid_mask[3] = 1;

	cb_Struct->mir[0] = 0;
	cb_Struct->mir[1] = 6;
	cb_Struct->mir[2] = 1;
	cb_Struct->mir[3] = 2;
		

	cb_Struct->mir_mask[0] = 0;
	cb_Struct->mir_mask[1] = 1;
	cb_Struct->mir_mask[2] = 1;
	cb_Struct->mir_mask[3] = 1;

	cb_Struct->para_ind = 2;

	FILE *fp;

	   fp = fopen( "CB_settings.txt", "w" );
	   if(NULL !=fp)
	   {
			fwrite(cb_Struct, sizeof(l4c_cb_info_struct), 1, fp );
			fclose(fp);
	   }
	 free((void*)cb_Struct);
   
}	




void *SendCBMessageSetting (void)
{
		
	//plugin for sending the CB language information
	//gSMSBuffer // contains the stuff receieved for Cell broadcast

		//Read the settings from the file
	FILE *fp;
	l4c_cb_info_struct cb_Struct;
	mmi_sms_get_cb_mode_rsp_struct	* SetCBRsp = (mmi_sms_get_cb_mode_rsp_struct * )
					CMemAllocator::MyMallocDataPtr(sizeof(mmi_sms_get_cb_mode_rsp_struct));
	memset((void*)SetCBRsp,0,sizeof(mmi_sms_get_cb_mode_rsp_struct));
	fp = fopen( "CB_settings.txt", "r" );
	if(NULL == fp)
	{
	//	fclose(fp);
		//write the default values here in file after opening the new file
		writeDefaultCBSettings();
	}	
	//reopen the file in read mode
	fp = fopen( "CB_settings.txt", "r" );
	if(NULL != fp)
	{
		//read the new settings here
		fread(&cb_Struct, sizeof(l4c_cb_info_struct), 1, fp );		
		//stuff the value in the other structure and send it 
		
		SetCBRsp->result = KAL_TRUE;
		SetCBRsp->total_mid_num = 3;
		SetCBRsp->total_mir_num = 3;
		SetCBRsp->total_dcs_num = 3;
		memcpy((void*)&SetCBRsp->info,&cb_Struct,sizeof(l4c_cb_info_struct));		
	}
	fclose(fp);
	return (void*)SetCBRsp;	

}


void SaveCBMessageSetting()
{
	//get the settings from mmi and save it now;
	FILE *fp;
	mmi_sms_set_cb_mode_req_struct recv;
	l4c_cb_info_struct cb_Struct;
	memcpy(&recv,gSMSBuffer,sizeof(mmi_sms_set_cb_mode_req_struct));

	fp = fopen( "CB_settings.txt", "r+" );
	if(NULL != fp)
	{
		//read the old settings here
	int j = fread(&cb_Struct, sizeof(l4c_cb_info_struct), 1, fp );

		if((recv.info.para_ind & SMSAL_CB_MI)==SMSAL_CB_MI)
		{
			//copy the msgReq->info.mid
			memcpy(&cb_Struct.mid,recv.info.mid,sizeof(recv.info.mid));
		}
		if(((recv.info.para_ind & SMSAL_CB_MI)==SMSAL_CB_MI)||((recv.info.para_ind & SMSAL_CB_MI_MASK)==SMSAL_CB_MI_MASK))
		{
				//update info.mid_mask
			memcpy(&cb_Struct.mid_mask,recv.info.mid_mask,sizeof(recv.info.mid_mask));
		}
		if((recv.info.para_ind & SMSAL_CB_DCS_MASK)==SMSAL_CB_DCS_MASK)
		{

			//	msgReq->info.dcs_mask[i]=1;
			memcpy(&cb_Struct.dcs_mask,recv.info.dcs_mask,sizeof(recv.info.dcs_mask));
		}
		if ((recv.info.para_ind & SMSAL_CB_ALL_LANG)==SMSAL_CB_ALL_LANG)
			cb_Struct.is_all_language_on = recv.info.is_all_language_on;
		fclose(fp);
		
		fp = fopen( "CB_settings.txt", "w" );
		if(NULL != fp)
		{
			int i=	fwrite(&cb_Struct, sizeof(l4c_cb_info_struct), 1, fp );
			fclose(fp);
		}
  
	}
}
