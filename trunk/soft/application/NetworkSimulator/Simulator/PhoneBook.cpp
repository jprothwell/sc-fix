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
/**************************************************************
	FILENAME	: PhoneBook.cpp
  	PURPOSE		: PhoneBook Plugins for Network Simulator
	REMARKS		: nil
	AUTHOR		: Pranav, Gurinder
	DATE		: Jun 16,03
**************************************************************/

/* Header files require for PhoneBook.cpp */
#include "stdafx.h"
#include "string.h"
#include "stdlib.h"
#include "PhoneBookAPI.h"
#include "MemAllocator.h"

/*	Global Variable Declearation	*/
U16 gTotalPbookEntry = 0;
U16 gTotalPbookEntryMSISDN = 0;
U16 gTotalPbookEntrySim=0;
U16 gTotalFDNEntries=0;
U16 gTotalPbookEntryPhone=0;
U16 gTotalPbookEntrySDN = 0;

/* Global Structure Decleartion		*/
PHB_STORAGE_LOCATION_ENUM PreferredStorageLocation = MMI_SIM;
PHB_ENTRY_STRUCT phbEntries[MAX_PB_ENTRIES];
PHB_ENTRY_STRUCT FDNEntries[MAX_PB_ENTRIES];
PHB_ENTRY_STRUCT phbEntriesSim[MAX_PB_ENTRIES_SIM];
PHB_ENTRY_STRUCT phbEntriesPhone[MAX_PB_ENTRIES_PHONE];

/* Global Message Stucture Declearation	*/
MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT *greadByIndexRsp = NULL;
MSG_PHB_GET_ENTRY_BY_NAME_RSP_STRUCT *searchEntryRsp = NULL;

/* Global File Pointer to store PhoneBook Data */
FILE *pb;
void * gBuffer = NULL;

/*****************************************************************************
* FUNCTION
*	HandleWriteEntryRequest
* DESCRIPTION
*	This Plugin API write the incoming phone book entry request to phonebook
*	database file. Incase number length is increased proper error is returned.
* PARAMETERS
*	void
* RETURNS
*	void * - MSG_PHB_SET_ENRTY_RSP_STRUCT is cast into void * to return to MMI
*****************************************************************************/

void *HandleWriteEntryRequest() {
	MSG_PHB_SET_ENRTY_REQ_STRUCT *setEntryReq = (MSG_PHB_SET_ENRTY_REQ_STRUCT*)gBuffer;
	MSG_PHB_SET_ENRTY_RSP_STRUCT *setEntryRsp;
	U16 i;

	setEntryRsp = (MSG_PHB_SET_ENRTY_RSP_STRUCT *)CMemAllocator::MyMallocDataPtr(sizeof(MSG_PHB_SET_ENRTY_RSP_STRUCT));
	memset( setEntryRsp, '\0', sizeof(MSG_PHB_SET_ENRTY_RSP_STRUCT) );
	setEntryRsp->result.flag = 0;	//OK
	
	/*Check the Phone Number Length for Maximum Value*/
	if((strlen((const char*)setEntryReq->list.tel.number) > 21) && (setEntryReq->list.storage == MMI_SIM)) {
		setEntryRsp->result.flag = 1;
		setEntryRsp->result.cause = 2561;
		return (void *)setEntryRsp;
	}

	/*If Phone Number Save Request is for FDN*/
	if(setEntryReq->type == MMI_PHB_FDN) 
	{
		if (setEntryReq->list.record_index==65535) 
		{
			if (setEntryReq->list.storage==1) 
			{	
				if (gTotalFDNEntries==0) 
				{
					memcpy((void *)&FDNEntries[gTotalFDNEntries], (void *)&setEntryReq->list, sizeof(setEntryReq->list));
					FDNEntries[gTotalFDNEntries].index = 1;
 					FDNEntries[gTotalFDNEntries].record_index = 1;
 					memcpy((void *)setEntryRsp->list, (void *)&FDNEntries[gTotalFDNEntries], sizeof(setEntryRsp->list[0]));
				}
				else if (setEntryReq->list.index == 65535 && gTotalFDNEntries> 0)  
				{
					memcpy((void *)&FDNEntries[gTotalFDNEntries], (void *)&setEntryReq->list, sizeof(setEntryReq->list));
					FDNEntries[gTotalFDNEntries].index = FDNEntries[gTotalFDNEntries - 1].index + 1;
 					FDNEntries[gTotalFDNEntries].record_index  = FDNEntries[gTotalFDNEntries - 1].record_index + 1;
 					memcpy((void *)setEntryRsp->list, (void *)&FDNEntries[gTotalFDNEntries], sizeof(setEntryRsp->list[0]));
				}
				gTotalFDNEntries++;
				WriteToFile(MMI_PHB_FDN);
			}
		}
		else 
		{
			if (setEntryReq->list.storage==1) 
			{
				i=setEntryReq->list.index;
				i--;
				memcpy((void *)&FDNEntries[i], (void *)&setEntryReq->list, sizeof(setEntryReq->list));
				memcpy((void *)setEntryRsp->list, (void *)&FDNEntries[i], sizeof(setEntryRsp->list[0]));
				WriteToFile(MMI_PHB_FDN);
			}
		}
	}

	/*If Phone Number Save Request Is For MSISDN*/
	else if(setEntryReq->type == MMI_PHB_MSISDN) {	
		for (U16 i = 0; i < gTotalPbookEntry; i++) {
			if (setEntryReq->list.index ==  phbEntries[i].index)
			break; 
		}
			
		strcpy((S8 *)phbEntries[i].alpha_id.name, (S8 *)setEntryReq->list.alpha_id.name);
		phbEntries[i].alpha_id.name_length = setEntryReq->list.alpha_id.name_length;
		strcpy((S8 *)phbEntries[i].tel.number, (S8 *)setEntryReq->list.tel.number);
		phbEntries[i].tel.length = setEntryReq->list.tel.length;
		WriteToFile(4);
	}

	/*If Entry Save Request is for Phone Book*/
	else if(setEntryReq->type == MMI_PHB_PHONEBOOK) {
		if (setEntryReq->list.record_index == 65535) {
			/*If Request Storage Type Is SIM*/
			if (setEntryReq->list.storage == MMI_SIM) {
				/*If Sim Entries Reaches To Its Maximum Value*/
				if(gTotalPbookEntrySim  == MAX_PB_ENTRIES_SIM) {
					setEntryRsp->result.flag = 1;
					setEntryRsp->result.cause = MMI_PHB_CAUSE_CAPACITY_EXCEEDED;
					return (void *)setEntryRsp;
				}
				/*If First Entry Intilize the Index And Record Index*/
				if (gTotalPbookEntrySim==0) {
					memcpy((void *)&phbEntriesSim[gTotalPbookEntrySim], (void *)&setEntryReq->list, sizeof(setEntryReq->list));
					phbEntriesSim[gTotalPbookEntrySim].index = 1;
 					phbEntriesSim[gTotalPbookEntrySim].record_index = 1;
 					memcpy((void *)setEntryRsp->list, (void *)&phbEntriesSim[gTotalPbookEntrySim], sizeof(setEntryRsp->list[0]));
				}
				/*Increment The Index And Record Index*/
				else if (setEntryReq->list.index == 65535 && gTotalPbookEntrySim> 0) {
					memcpy((void *)&phbEntriesSim[gTotalPbookEntrySim], (void *)&setEntryReq->list, sizeof(setEntryReq->list));
					phbEntriesSim[gTotalPbookEntrySim].index = phbEntriesSim[gTotalPbookEntrySim - 1].index + 1;
 					phbEntriesSim[gTotalPbookEntrySim].record_index  = phbEntriesSim[gTotalPbookEntrySim - 1].record_index + 1;
 					memcpy((void *)setEntryRsp->list, (void *)&phbEntriesSim[gTotalPbookEntrySim], sizeof(setEntryRsp->list[0]));
				}
				gTotalPbookEntrySim++;
				/*Write The Phone Book Data Base to File for SIM*/
				WriteToFile(MMI_SIM);
			}
			/*If Request Storage Type Is Phone*/
			if (setEntryReq->list.storage == MMI_NVRAM) {	
				/*If Phone Entries Reaches To Its Maximum Value*/
				if(gTotalPbookEntryPhone  == MAX_PB_ENTRIES_PHONE) {
					setEntryRsp->result.flag = 1;
					setEntryRsp->result.cause = MMI_PHB_CAUSE_CAPACITY_EXCEEDED;
					return (void *)setEntryRsp;
				}
				/*If First Entry Intilize the Index And Record Index*/
				if (gTotalPbookEntryPhone==0) {
					memcpy((void *)&phbEntriesPhone[gTotalPbookEntryPhone], (void *)&setEntryReq->list, sizeof(setEntryReq->list));
					phbEntriesPhone[gTotalPbookEntryPhone].index = 1;
 					phbEntriesPhone[gTotalPbookEntryPhone].record_index = 1;
 					memcpy((void *)setEntryRsp->list, (void *)&phbEntriesPhone[gTotalPbookEntryPhone], sizeof(setEntryRsp->list[0]));
				}	
				/*Increment The Index And Record Index*/
				else if (setEntryReq->list.index == 65535 && gTotalPbookEntryPhone> 0) {
					memcpy((void *)&phbEntriesPhone[gTotalPbookEntryPhone], (void *)&setEntryReq->list, sizeof(setEntryReq->list));
					phbEntriesPhone[gTotalPbookEntryPhone].index = phbEntriesPhone[gTotalPbookEntryPhone - 1].index + 1;
 					phbEntriesPhone[gTotalPbookEntryPhone].record_index  = phbEntriesPhone[gTotalPbookEntryPhone - 1].record_index + 1;
 					memcpy((void *)setEntryRsp->list, (void *)&phbEntriesPhone[gTotalPbookEntryPhone], sizeof(setEntryRsp->list[0]));
				}
				gTotalPbookEntryPhone++;
				/*Write The Phone Book Data Base to File for Phone*/
				WriteToFile(MMI_NVRAM);
			}
		}
		else {
			/*If new entry is for SIM*/
			if (setEntryReq->list.storage == MMI_SIM) {
				i=setEntryReq->list.record_index;
				i--;
				memcpy((void *)&phbEntriesSim[i], (void *)&setEntryReq->list, sizeof(setEntryReq->list));
				memcpy((void *)setEntryRsp->list, (void *)&phbEntriesSim[i], sizeof(setEntryRsp->list[0]));
				WriteToFile(MMI_SIM);
			}
			/*If new entry is for phone*/
			if (setEntryReq->list.storage == MMI_NVRAM) {	
				i=setEntryReq->list.record_index;
				i--;
				memcpy((void *)&phbEntriesPhone[i], (void *)&setEntryReq->list, sizeof(setEntryReq->list));
				memcpy((void *)setEntryRsp->list, (void *)&phbEntriesPhone[i], sizeof(setEntryRsp->list[0]));
				WriteToFile(MMI_NVRAM);
			}
		}
	}
	return (void *)setEntryRsp;
}

/*****************************************************************************
* FUNCTION
*	HandleSearchEntryRequest
* DESCRIPTION
*	This API search the phone book data base for a specific search expression.
* PARAMETERS
*	void
* RETURNS
*	void * -	MSG_PHB_GET_ENTRY_BY_NAME_RSP_STRUCT structure holds the entry 
*				corresponding 
*****************************************************************************/

void *HandleSearchEntryRequest() {
	/*Cast The Incoming Message to Name Req Structure*/
	MSG_PHB_GET_ENTRY_BY_NAME_REQ_STRUCT *searchEntryReq = (MSG_PHB_GET_ENTRY_BY_NAME_REQ_STRUCT*)gBuffer;
	S8 searchName[30];
	U16 i, searchCount;

	strcpy(searchName, (S8 *)searchEntryReq->name.name);
	searchEntryRsp = (MSG_PHB_GET_ENTRY_BY_NAME_RSP_STRUCT *)CMemAllocator::MyMallocDataPtr(sizeof(MSG_PHB_GET_ENTRY_BY_NAME_RSP_STRUCT));
	memset( searchEntryRsp, '\0', sizeof(MSG_PHB_GET_ENTRY_BY_NAME_RSP_STRUCT) );
	
	/*Search The Sub Name In Phone Book Database For Sim*/
	for (i = 0, searchCount = 0; i < gTotalPbookEntrySim; i++) {
		if (strstr((S8 *)phbEntriesSim[i].alpha_id.name, searchName)) {
			searchEntryRsp->list[searchCount] = phbEntriesSim[i];
			/*Increment The Count On Search Sucess*/
			searchCount++;
		}
	}
	/*Search The Sub Name In Phone Book Database For Phone*/
	for (i = 0; i < gTotalPbookEntryPhone; i++) {
		if (strstr((S8 *)phbEntriesPhone[i].alpha_id.name, searchName)) {
			searchEntryRsp->list[searchCount] = phbEntriesPhone[i];
			/*Increment The Count On Search Sucess*/
			searchCount++;
		}
	}

	/*If search is sucessfull fill the return structure*/
	if (searchCount) {
		searchEntryRsp->no_list = searchCount;
		searchEntryRsp->result.flag = 0;
		searchEntryRsp->result.cause = 0;
		S8 str[20];
		sprintf(str, "%d entries found", searchCount);
	}
	/*Else fill the return structure with Error Value*/
	else {
		searchEntryRsp->result.flag = 1;
		searchEntryRsp->result.cause =1;
	}
	return (void *)searchEntryRsp;
}

/*****************************************************************************
* FUNCTION
*	HandleDeleteEntryRequest
* DESCRIPTION
*	This API Delete The PhoneBook Entry Depending Upon The Index/Record Index
*	Value.
* PARAMETERS
*	void
* RETURNS
*	void * -	MSG_PHB_DELETE_ENTRY_RSP_STRUCT structure pointer hold delete 
*				request response for MMI.
*****************************************************************************/

void *HandleDeleteEntryRequest() {
	U16 i,j;
	MSG_PHB_DELETE_ENTRY_REQ_STRUCT* deleteEntryReq = (MSG_PHB_DELETE_ENTRY_REQ_STRUCT*)gBuffer;
	MSG_PHB_DELETE_ENTRY_RSP_STRUCT* deleteEntryRsp = NULL;

	/* If the request is for the FDN number */
	if(deleteEntryReq->type == MMI_PHB_FDN) {
		for (i = 0; i < gTotalFDNEntries; i++) {
			if (FDNEntries[i].index ==  deleteEntryReq->index)
				break;
		}
		for (j = i; j < gTotalFDNEntries - 1; j++)  FDNEntries[j] = FDNEntries[j+1];
		/*Reduce The Number Of Entries*/
		gTotalFDNEntries--;
		/*Write The Update Data To File*/
		WriteToFile(MMI_PHB_FDN);
	}
	/*If Delete Request Is For The Phone Book*/
	else {
		/*If Request is not for delete all*/
		if (deleteEntryReq->del_all == 0) {
			/*If Storage is for Sim*/
			if (deleteEntryReq->storage==1) {	
				for (i = 0; i < gTotalPbookEntrySim; i++)  {
					if ( phbEntriesSim[i].index ==  deleteEntryReq->index)
						break;
				}
				for (j = i; j < gTotalPbookEntrySim - 1; j++)  phbEntriesSim[j] = phbEntriesSim[j+1];
				/* Reduce Number Of Entries*/
				gTotalPbookEntrySim--;
				/*Write Updates Data To File*/
				WriteToFile(MMI_SIM);
			}
			/*Storage is for Phone*/
			if (deleteEntryReq->storage==2) {	
				for (i = 0; i < gTotalPbookEntryPhone; i++) {
					if ( phbEntriesPhone[i].index ==  deleteEntryReq->index)
						break;
				}
				for (j = i; j < gTotalPbookEntryPhone - 1; j++)  phbEntriesPhone[j] = phbEntriesPhone[j+1];

				gTotalPbookEntryPhone--;
				WriteToFile(MMI_NVRAM);
			}
		}
		/*If Delete All Request Is True*/
		else if (deleteEntryReq->del_all == 1) {
			/*For Sim*/
			if (deleteEntryReq->storage==1) {	
				gTotalPbookEntrySim=0;
				WriteToFile(1);
			}
			if (deleteEntryReq->storage==2) {	
			/*For Phone*/
				gTotalPbookEntryPhone=0;
				WriteToFile(2);
			}
		}
	}

	deleteEntryRsp = (MSG_PHB_DELETE_ENTRY_RSP_STRUCT *)CMemAllocator::MyMallocDataPtr(sizeof(MSG_PHB_DELETE_ENTRY_RSP_STRUCT));
	memset( deleteEntryRsp, '\0', sizeof(MSG_PHB_DELETE_ENTRY_RSP_STRUCT) );
	deleteEntryRsp->result.flag = 0;
	deleteEntryRsp->no_list = gTotalPbookEntry;
	return (void *) deleteEntryRsp;
}


/*****************************************************************************
* FUNCTION
*	HandleGetAllEntriesRequest
* DESCRIPTION
*	This API fill the phone book/fdn/sdn/msisdn database for simulator.
* PARAMETERS
*	void
* RETURNS
*	void * -	MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT structure pointer hold  
*				phone book data base.
*****************************************************************************/

void *HandleGetAllEntriesRequest() {
	U16 k;
	U16 i;
	MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT *readbyIndexReq = (MSG_PHB_GET_ENTRY_BY_INDEX_REQ_STRUCT *)gBuffer;

	ReadFromFile(readbyIndexReq->type);

	greadByIndexRsp = (MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT*)CMemAllocator::MyMallocDataPtr(sizeof(MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT));
	greadByIndexRsp->result.flag = 0;  //OK
	greadByIndexRsp->result.cause = 0; //L4C_NO_CAUSE

	/* Request is for phonebook database */
	if(readbyIndexReq->type == MMI_PHB_PHONEBOOK) {
		/* If request is for the SIM database fill the simulator structure for sim*/
		if (readbyIndexReq->storage == MMI_SIM) 
			greadByIndexRsp->no_list = gTotalPbookEntrySim;
		/*If request is for Phone database*/
		if (readbyIndexReq->storage == MMI_NVRAM) 
			greadByIndexRsp->no_list = gTotalPbookEntryPhone;
		/*If the request is for both */
		if (readbyIndexReq->storage == MMI_STORAGE_NONE) 
			greadByIndexRsp->no_list = gTotalPbookEntrySim+gTotalPbookEntryPhone;
		/*Fills the simultor structre for Sim Database*/
		for (i = 0,k=0; i < gTotalPbookEntrySim; i++,k++) {
			greadByIndexRsp->list[k] = phbEntriesSim[i];
		}
		/*Fills the simulator structre for Phone Database*/
		for (i = 0; i < gTotalPbookEntryPhone; i++,k++) {
			greadByIndexRsp->list[k] = phbEntriesPhone[i];
		}
	}
	/*If the database req is for the FDN number*/
	else if(readbyIndexReq->type == MMI_PHB_FDN) {
		if (readbyIndexReq->storage==1) 
			greadByIndexRsp->no_list = gTotalFDNEntries;
		for (i = 0,k=0; i < gTotalFDNEntries; i++,k++) {
			greadByIndexRsp->list[k] = FDNEntries[i];
		}
	}
	/*If database req is for MSISDN*/
	else if(readbyIndexReq->type == MMI_PHB_MSISDN) {	
		greadByIndexRsp->no_list = gTotalPbookEntryMSISDN;
		for (i = 0;i < gTotalPbookEntryMSISDN;i++)
			greadByIndexRsp->list[i] = phbEntries[i];
	}
	/*If Database req is for SDN*/
	else if(readbyIndexReq->type == MMI_PHB_SDN) {	
		greadByIndexRsp->no_list = gTotalPbookEntrySDN;
		for (i = 0;i < gTotalPbookEntrySDN;i++)
			greadByIndexRsp->list[i] = phbEntries[i];
	}
	/*If database req is for ECC*/
	else if(readbyIndexReq->type == MMI_PHB_ECC) {
		greadByIndexRsp->no_list = 5;
		for (i = 0; i < 5; i++) {
			greadByIndexRsp->list[i].storage = MMI_SIM;
			greadByIndexRsp->list[i].type	= 0;
			greadByIndexRsp->list[i].record_index = i+1;
			greadByIndexRsp->list[i].index = i+1;
			greadByIndexRsp->list[i].tel.type = 129;
			greadByIndexRsp->list[i].tel.length = 3;
			greadByIndexRsp->list[i].alpha_id.name_length = 0;
			greadByIndexRsp->list[i].alpha_id.name_dcs = 0;
			memset(greadByIndexRsp->list[i].alpha_id.name, 0, 2);
		}
		strcpy((PS8)greadByIndexRsp->list[0].tel.number, "112");
		strcpy((PS8)greadByIndexRsp->list[1].tel.number, "113");
		strcpy((PS8)greadByIndexRsp->list[2].tel.number, "114");
		strcpy((PS8)greadByIndexRsp->list[3].tel.number, "115");
		strcpy((PS8)greadByIndexRsp->list[4].tel.number, "116");
	}

	return (void *) greadByIndexRsp;	
}

/*****************************************************************************
* FUNCTION
*	RepHandleGetAllEntriesRequest
* DESCRIPTION
*	This API returns the phone book/fdn/sdn/msisdn database to MMI.
* PARAMETERS
*	void
* RETURNS
*	void * -	MSG_PHB_GET_ENTRY_BY_INDEX_RSP_STRUCT structure pointer hold  
*				phone book data base.
*****************************************************************************/
void *RepHandleGetAllEntriesRequest()
{
	return (void *) greadByIndexRsp;
}

/*****************************************************************************
* FUNCTION
*	HandleGetStorageRequest
* DESCRIPTION
*	This API returns the used/max entires of phone book perfferd storage 
*	location.
* PARAMETERS
*	void
* RETURNS
*	void * -	MSG_PHB_GET_PREFERRED_STORAGE_RSP_STRUCT structure holds
*				the used/max entires of phone book perfferd storage location.
*****************************************************************************/
void * HandleGetStorageRequest() { 
	MSG_PHB_GET_PREFERRED_STORAGE_RSP_STRUCT *getStorageRsp;

	getStorageRsp = (MSG_PHB_GET_PREFERRED_STORAGE_RSP_STRUCT *)CMemAllocator::MyMallocDataPtr(sizeof(MSG_PHB_GET_PREFERRED_STORAGE_RSP_STRUCT));
	memset( getStorageRsp, '\0', sizeof(MSG_PHB_GET_PREFERRED_STORAGE_RSP_STRUCT) );
	getStorageRsp->result = PB_TRUE;
	getStorageRsp->storage = PreferredStorageLocation;
	/* If Prefferd Storage Location Is SIM*/
	if (getStorageRsp->storage==1) {	
		getStorageRsp->used = gTotalPbookEntrySim;
		getStorageRsp->total = MAX_PB_ENTRIES_SIM;
	}
	/* If Prefferd Storage Location Is Phone*/
	if (getStorageRsp->storage==2) {	
		getStorageRsp->used = gTotalPbookEntryPhone;
		getStorageRsp->total = MAX_PB_ENTRIES_PHONE;
	}
	
	return (void*)getStorageRsp;
}

/*****************************************************************************
* FUNCTION
*	HandleSetStorageRequest
* DESCRIPTION
*	This API save the phone book perfferd storage location.
* PARAMETERS
*	void
* RETURNS
*	void * -	MSG_PHB_SET_PREFERRED_STORAGE_RSP_STRUCT structure holds
*				Sucess or Failure of the request.
*****************************************************************************/
void * HandleSetStorageRequest() {
	MSG_PHB_SET_PREFERRED_STORAGE_REQ_STRUCT *setPBookStorageReq = (MSG_PHB_SET_PREFERRED_STORAGE_REQ_STRUCT *)gBuffer;
	MSG_PHB_SET_PREFERRED_STORAGE_RSP_STRUCT *setStorageRsp;
	static int i;
	setStorageRsp = (MSG_PHB_SET_PREFERRED_STORAGE_RSP_STRUCT *)CMemAllocator::MyMallocDataPtr(sizeof(MSG_PHB_SET_PREFERRED_STORAGE_RSP_STRUCT));	

	memset( setStorageRsp, '\0', sizeof(MSG_PHB_SET_PREFERRED_STORAGE_RSP_STRUCT) );
	if (!strcmp((const char*)setPBookStorageReq->pin, "11111")) {
		setStorageRsp->result = PB_TRUE;
		PreferredStorageLocation = (PHB_STORAGE_LOCATION_ENUM)setPBookStorageReq->storage;
	}
	else 
		setStorageRsp->result = PB_FALSE;
	if (i==0) 
	{	PreferredStorageLocation=MMI_SIM;
		i=1;}
	else if (i==1) 
	{	PreferredStorageLocation=MMI_NVRAM;
		i=0;
	}
	

	return setStorageRsp;
}

/*****************************************************************************
* FUNCTION
*	ReadFromFile
* DESCRIPTION
*	Read database from file for phonebook/fdn/msisdn/sdn
* PARAMETERS
*	U8 - Type Of Database phonebook/fdn/msisdn/sdn
* RETURNS
*	void
*****************************************************************************/
void ReadFromFile(U8 PbookType) {
	S8 str[100];
	S8 *token;
	gTotalPbookEntryPhone = 0;
	gTotalPbookEntrySim=0;
	gTotalFDNEntries=0;
	gTotalPbookEntryMSISDN=0;
	gTotalPbookEntry=0;
	gTotalPbookEntrySDN = 0;

	/* If the read request is for the FDN number*/
	if(PbookType == MMI_PHB_FDN) {
		if((pb = fopen("Phonebook_fdn.txt", "r")) == NULL) 
			return;
		else {
			while (fgets(str,100,pb)) {
				str[strlen(str) - 1]='\0';
				token = strtok(str, ",");
				FDNEntries[gTotalFDNEntries].index = atoi(token);
				token = strtok(NULL, ",");
 				FDNEntries[gTotalFDNEntries].record_index = atoi(token);
				token = strtok(NULL, ",");
				if(!strcmp(token,"NULL"))
					FDNEntries[gTotalFDNEntries].alpha_id.name[0] ='\0';
				else
					strcpy((S8 *)FDNEntries[gTotalFDNEntries].alpha_id.name, token);
				token = strtok(NULL, ",");
				FDNEntries[gTotalFDNEntries].alpha_id.name_dcs = atoi(token);
				token = strtok(NULL, ",");
				FDNEntries[gTotalFDNEntries].alpha_id.name_length = atoi(token);
				token = strtok(NULL, ",");
				FDNEntries[gTotalFDNEntries].tel.length = atoi(token);
				token = strtok(NULL, ",");
				FDNEntries[gTotalFDNEntries].tel.type = atoi(token);
				token = strtok(NULL, ",");
				if(!strcmp(token,"NULL"))
					FDNEntries[gTotalFDNEntries].tel.number[0] =  '\0';
				else
					strcpy((S8 *)FDNEntries[gTotalFDNEntries].tel.number, token);
				token = strtok(NULL, ",");
				FDNEntries[gTotalFDNEntries].storage = atoi(token);
			
				gTotalFDNEntries++;
			}
		}
		fclose(pb);

		if((pb = fopen("Phonebook_phone.txt", "r")) == NULL)
			return;
		else {
			while (fgets(str,100,pb)) {
				str[strlen(str) - 1]='\0';
				token = strtok(str, ",");
				phbEntriesPhone[gTotalPbookEntryPhone].index = atoi(token);
				token = strtok(NULL, ",");
	 			phbEntriesPhone[gTotalPbookEntryPhone].record_index = atoi(token);
 				token = strtok(NULL, ",");
				if(!strcmp(token,"NULL"))
					phbEntriesPhone[gTotalPbookEntryPhone].alpha_id.name[0] ='\0';
				else
					strcpy((S8 *)phbEntriesPhone[gTotalPbookEntryPhone].alpha_id.name, token);
				token = strtok(NULL, ",");
				phbEntriesPhone[gTotalPbookEntryPhone].alpha_id.name_dcs = atoi(token);
				token = strtok(NULL, ",");
				phbEntriesPhone[gTotalPbookEntryPhone].alpha_id.name_length = atoi(token);
				token = strtok(NULL, ",");
				phbEntriesPhone[gTotalPbookEntryPhone].tel.length = atoi(token);
				token = strtok(NULL, ",");
				phbEntriesPhone[gTotalPbookEntryPhone].tel.type = atoi(token);
				token = strtok(NULL, ",");
				if(!strcmp(token,"NULL"))
					phbEntriesPhone[gTotalPbookEntryPhone].tel.number[0] =  '\0';
				else
					strcpy((S8 *)phbEntriesPhone[gTotalPbookEntryPhone].tel.number, token);
				token = strtok(NULL, ",");
				phbEntriesPhone[gTotalPbookEntryPhone].storage = atoi(token);
			
				gTotalPbookEntryPhone++;
			}
		}
	
		fclose(pb);
	}
	/*If the read request is for the phonebook*/
	else if(PbookType == MMI_PHB_PHONEBOOK) {
		if((pb = fopen("Phonebook_sim.txt", "r")) == NULL) 
			return;
		else {
			while (fgets(str,100,pb)) {
				str[strlen(str) - 1]='\0';
				token = strtok(str, ",");
				phbEntriesSim[gTotalPbookEntrySim].index = atoi(token);
				token = strtok(NULL, ",");
	 			phbEntriesSim[gTotalPbookEntrySim].record_index = atoi(token);
 				token = strtok(NULL, ",");
				if(!strcmp(token,"NULL"))
					phbEntriesSim[gTotalPbookEntrySim].alpha_id.name[0] ='\0';
				else
					strcpy((S8 *)phbEntriesSim[gTotalPbookEntrySim].alpha_id.name, token);
				token = strtok(NULL, ",");
				phbEntriesSim[gTotalPbookEntrySim].alpha_id.name_dcs = atoi(token);
				token = strtok(NULL, ",");
				phbEntriesSim[gTotalPbookEntrySim].alpha_id.name_length = atoi(token);
				token = strtok(NULL, ",");
				phbEntriesSim[gTotalPbookEntrySim].tel.length = atoi(token);
				token = strtok(NULL, ",");
				phbEntriesSim[gTotalPbookEntrySim].tel.type = atoi(token);
				token = strtok(NULL, ",");
				if(!strcmp(token,"NULL"))
					phbEntriesSim[gTotalPbookEntrySim].tel.number[0] =  '\0';
				else
					strcpy((S8 *)phbEntriesSim[gTotalPbookEntrySim].tel.number, token);
				token = strtok(NULL, ",");
				phbEntriesSim[gTotalPbookEntrySim].storage = atoi(token);
			
				gTotalPbookEntrySim++;
			}
		}
		fclose(pb);

		if((pb = fopen("Phonebook_phone.txt", "r")) == NULL) 
			return;
		else {
			while (fgets(str,100,pb)) {
				str[strlen(str) - 1]='\0';
				token = strtok(str, ",");
				phbEntriesPhone[gTotalPbookEntryPhone].index = atoi(token);
				token = strtok(NULL, ",");
	 			phbEntriesPhone[gTotalPbookEntryPhone].record_index = atoi(token);
 				token = strtok(NULL, ",");
				if(!strcmp(token,"NULL"))
					phbEntriesPhone[gTotalPbookEntryPhone].alpha_id.name[0] ='\0';
				else
					strcpy((S8 *)phbEntriesPhone[gTotalPbookEntryPhone].alpha_id.name, token);
				token = strtok(NULL, ",");
				phbEntriesPhone[gTotalPbookEntryPhone].alpha_id.name_dcs = atoi(token);
				token = strtok(NULL, ",");
				phbEntriesPhone[gTotalPbookEntryPhone].alpha_id.name_length = atoi(token);
				token = strtok(NULL, ",");
				phbEntriesPhone[gTotalPbookEntryPhone].tel.length = atoi(token);
				token = strtok(NULL, ",");
				phbEntriesPhone[gTotalPbookEntryPhone].tel.type = atoi(token);
				token = strtok(NULL, ",");
				if(!strcmp(token,"NULL"))
					phbEntriesPhone[gTotalPbookEntryPhone].tel.number[0] =  '\0';
				else
					strcpy((S8 *)phbEntriesPhone[gTotalPbookEntryPhone].tel.number, token);
				token = strtok(NULL, ",");
				phbEntriesPhone[gTotalPbookEntryPhone].storage = atoi(token);
			
				gTotalPbookEntryPhone++;
			}
		}
		fclose(pb);
	}
	/* If the read request is for MSISDN */
	else if(PbookType == MMI_PHB_MSISDN) {
		if((pb = fopen("PBookMSISDN.txt", "r")) == NULL) {
			//create the file with null entries
			pb = fopen("PBookMSISDN.txt", "w");
			if(pb == NULL) exit(0);
			for(U16 i = 0; i < 4; i++) {
				sprintf(str, "%d,%d,%s,%d,%d,%d,%d,%s,%d\n",
				i,
 				0,
				"NULL",
				0,
				0,
				0,
				0,
				"NULL",
				0);
				fputs(str, pb);
			}
			fclose(pb);
		}

		if((pb = fopen("PBookMSISDN.txt", "r")) != NULL) {
			while (fgets(str,100,pb)) {
				str[strlen(str) - 1]='\0';
				token = strtok(str, ",");
				phbEntries[gTotalPbookEntry].index = atoi(token);
				token = strtok(NULL, ",");
	 			phbEntries[gTotalPbookEntry].record_index = atoi(token);
 				token = strtok(NULL, ",");
				if(!strcmp(token,"NULL"))
					phbEntries[gTotalPbookEntry].alpha_id.name[0] ='\0';
				else
					strcpy((S8 *)phbEntries[gTotalPbookEntry].alpha_id.name, token);
				token = strtok(NULL, ",");
				phbEntries[gTotalPbookEntry].alpha_id.name_dcs = atoi(token);
				token = strtok(NULL, ",");
				phbEntries[gTotalPbookEntry].alpha_id.name_length = atoi(token);
				token = strtok(NULL, ",");
				phbEntries[gTotalPbookEntry].tel.length = atoi(token);
				token = strtok(NULL, ",");
				phbEntries[gTotalPbookEntry].tel.type = atoi(token);
				token = strtok(NULL, ",");
				if(!strcmp(token,"NULL"))
					phbEntries[gTotalPbookEntry].tel.number[0] =  '\0';
				else
					strcpy((S8 *)phbEntries[gTotalPbookEntry].tel.number, token);
				token = strtok(NULL, ",");
				phbEntries[gTotalPbookEntry].storage = atoi(token);			

				gTotalPbookEntryMSISDN++;
				gTotalPbookEntry++;
			}		
		
			fclose(pb);
		}
	}
	/* If the read request is for the SDN number */
	else if(PbookType == MMI_PHB_SDN) {
		if((pb = fopen("PBookSDN.txt", "r")) == NULL) {
		//create the file with null entries
			pb = fopen("PBookSDN.txt", "w");
			if(pb == NULL) exit(0);
			for(U16 i = 0; i < 10; ++i) {
				if(i%2)
					sprintf(str, "%d,%d,%s,%d,%d,%d,%d,%s,%d\n",
						(i+1), (i+1), "Pranav", MMI_PHB_ASCII, 6, 4, 129, "9811601293",MMI_SIM);
				else
					sprintf(str, "%d,%d,%s,%d,%d,%d,%d,%s,%d\n",
						(i+1), (i+1), "NULL", MMI_PHB_ASCII, 0, 0, 129, "NULL", MMI_SIM);

				fputs(str, pb);
			}
			fclose(pb);
		}

		if((pb = fopen("PBookSDN.txt", "r")) != NULL) {
			while (fgets(str,100,pb)) {
				str[strlen(str) - 1]='\0';
				token = strtok(str, ",");
				phbEntries[gTotalPbookEntry].index = atoi(token);
				token = strtok(NULL, ",");
				phbEntries[gTotalPbookEntry].record_index = atoi(token);
				token = strtok(NULL, ",");
				if(!strcmp(token,"NULL"))
					phbEntries[gTotalPbookEntry].alpha_id.name[0] ='\0';
				else
					strcpy((S8 *)phbEntries[gTotalPbookEntry].alpha_id.name, token);
				token = strtok(NULL, ",");
				phbEntries[gTotalPbookEntry].alpha_id.name_dcs = atoi(token);
				token = strtok(NULL, ",");
				phbEntries[gTotalPbookEntry].alpha_id.name_length = atoi(token);
				token = strtok(NULL, ",");
				phbEntries[gTotalPbookEntry].tel.length = atoi(token);
				token = strtok(NULL, ",");
				phbEntries[gTotalPbookEntry].tel.type = atoi(token);
				token = strtok(NULL, ",");
				if(!strcmp(token,"NULL"))
					phbEntries[gTotalPbookEntry].tel.number[0] =  '\0';
				else
					strcpy((S8 *)phbEntries[gTotalPbookEntry].tel.number, token);
				token = strtok(NULL, ",");
				phbEntries[gTotalPbookEntry].storage = atoi(token);			
				gTotalPbookEntrySDN++;
				gTotalPbookEntry++;
			}		
			fclose(pb);
		}
	}
}

/*****************************************************************************
* FUNCTION
*	WriteToFile
* DESCRIPTION
*	Write database from file for phonebook/fdn/msisdn/sdn
* PARAMETERS
*	U8 - Type Of Database phonebook/fdn/msisdn/sdn
* RETURNS
*	void
*****************************************************************************/
void WriteToFile(U8 PbookType) 
{	
	U16 i;
	S8 str[200];
 	MSG_PHB_SET_ENRTY_REQ_STRUCT *setEntryReq = (MSG_PHB_SET_ENRTY_REQ_STRUCT*)gBuffer;
	/*If the write request is for the FDN*/
	
	if(setEntryReq->type != MMI_PHB_PHONEBOOK )
	{
		if(PbookType == MMI_PHB_FDN) 
		{	
			pb = fopen("Phonebook_fdn.txt", "w+");
			if(pb == NULL)  
				exit(0);
		
			for (i=0;i<gTotalFDNEntries;i++) 
			{	
				if(strlen((const char*)FDNEntries[i].alpha_id.name) == 0)
					strcpy((char*)FDNEntries[i].alpha_id.name,"NULL");
				if(strlen((const char*)FDNEntries[i].tel.number) == 0)
					strcpy((char*)FDNEntries[i].tel.number, "NULL");

				sprintf(str, "%d,%d,%s,%d,%d,%d,%d,%s,%d\n",
					FDNEntries[i].index,
					FDNEntries[i].record_index,
					FDNEntries[i].alpha_id.name,
					FDNEntries[i].alpha_id.name_dcs,
					FDNEntries[i].alpha_id.name_length,
					FDNEntries[i].tel.length,
					FDNEntries[i].tel.type,
					FDNEntries[i].tel.number,
					FDNEntries[i].storage);

				fputs(str, pb);
			}
		fclose(pb);
		}
		/*If the write req is for the MSISDN*/
		if(PbookType == MMI_PHB_MSISDN) 
		{
			pb = fopen("PBookMSISDN.txt", "w");
			if(pb == NULL) exit(0);
			
			for(U16 i = 0; i < gTotalPbookEntry; i++) 
			{
				if(strlen((const char*)phbEntries[i].alpha_id.name) == 0)
					strcpy((char*)phbEntries[i].alpha_id.name,"NULL");
				if(strlen((const char*)phbEntries[i].tel.number) == 0)
					strcpy((char*)phbEntries[i].tel.number, "NULL");
				
	 			sprintf(str, "%d,%d,%s,%d,%d,%d,%d,%s,%d\n",
					phbEntries[i].index,
 					phbEntries[i].record_index,
					phbEntries[i].alpha_id.name,
					phbEntries[i].alpha_id.name_dcs,
					phbEntries[i].alpha_id.name_length,
					phbEntries[i].tel.length,
					phbEntries[i].tel.type,
					phbEntries[i].tel.number,
					phbEntries[i].storage);

				fputs(str, pb);
			}
		fclose(pb);
		}
	}
	
	/*If the request is for PhoneBook-SIM*/
	if( setEntryReq->type == MMI_PHB_PHONEBOOK )
	{
		if (PbookType==1) 
		{	
				if( gTotalPbookEntrySim == 0 )
				{
					pb = fopen("Phonebook_sim.txt", "w");
					fclose(pb);
					return;
				}
			pb = fopen("Phonebook_sim.txt", "w+");
			if(pb == NULL)  exit(0);
			
			for (i=0;i<gTotalPbookEntrySim;i++) 
			{	
				if(strlen((const char*)phbEntriesSim[i].alpha_id.name) == 0)
					strcpy((char*)phbEntriesSim[i].alpha_id.name,"NULL");
				if(strlen((const char*)phbEntriesSim[i].tel.number) == 0)
					strcpy((char*)phbEntriesSim[i].tel.number, "NULL");

				sprintf(str, "%d,%d,%s,%d,%d,%d,%d,%s,%d\n",
					phbEntriesSim[i].index,
 					phbEntriesSim[i].record_index,
					phbEntriesSim[i].alpha_id.name,
					phbEntriesSim[i].alpha_id.name_dcs,
					phbEntriesSim[i].alpha_id.name_length,
					phbEntriesSim[i].tel.length,
					phbEntriesSim[i].tel.type,
					phbEntriesSim[i].tel.number,
					phbEntriesSim[i].storage);

				fputs(str, pb);
			}
		}
		/*If the request is for the PhoneBook-Phone*/
		if (PbookType==2) 
		{	
				if( gTotalPbookEntryPhone == 0 )
				{
					pb = fopen("Phonebook_phone.txt", "w");
					fclose(pb);
					return;
				}
			pb = fopen("Phonebook_phone.txt", "w+");
			if(pb == NULL)  exit(0);
			for (i=0;i<gTotalPbookEntryPhone;i++) 
			{	
				if(strlen((const char*)phbEntriesPhone[i].alpha_id.name) == 0)
					strcpy((char*)phbEntriesPhone[i].alpha_id.name,"NULL");
				if(strlen((const char*)phbEntriesPhone[i].tel.number) == 0)
					strcpy((char*)phbEntriesPhone[i].tel.number, "NULL");

				sprintf(str, "%d,%d,%s,%d,%d,%d,%d,%s,%d\n",
					phbEntriesPhone[i].index,
 					phbEntriesPhone[i].record_index,
					phbEntriesPhone[i].alpha_id.name,
					phbEntriesPhone[i].alpha_id.name_dcs,
					phbEntriesPhone[i].alpha_id.name_length,
					phbEntriesPhone[i].tel.length,
					phbEntriesPhone[i].tel.type,
					phbEntriesPhone[i].tel.number,
					phbEntriesPhone[i].storage);

				fputs(str, pb);
			}
		}
	fclose(pb);
	}
}

/*****************************************************************************
* FUNCTION
*	SetBufferForPbook
* DESCRIPTION
*	Fills the incoming message in phone book global buffer
* PARAMETERS
*	void * - input buffer pointer.
* RETURNS
*	void
*****************************************************************************/
void SetBufferForPbook(void* dataBuf) {
	gBuffer = dataBuf;
}

