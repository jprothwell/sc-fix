#ifndef __TEST_TYPE2_CONTROL_C__
#define __TEST_TYPE2_CONTROL_C__

#include "testtype2control.h"
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT

U8 simulator_map_sms_sim2[SIMULATOR_MAX_MAP_SMS_SIM2_LEN];
U8 simulator_map_sms_me2[ SIMULATOR_MAX_MAP_SMS_ME2_LEN];

U8 simulator_map_phb_sim2[SIMULATOR_MAX_MAP_PHB_LEN];

static INT32 simulator_map_phb_total_count = SIMULATOR_MAX_MAP_PHB_LEN;
static INT32 simulator_map_phb_used_count = 16;

void simulator_init_sms_storage()
{
	simulator_init_sms_sim2_storage();

	simulator_init_sms_me2_storage();

	simulator_init_map_sms_sim2();
	simulator_init_map_sms_me2();
}

void simulator_init_phb_storage()
{
	simulator_init_phb_sim2_storage();
	simulator_init_map_phb_sim2();
}

void simulator_init_map_sms_sim2()
{

	INT32 i;

	for (i = 0; i < SIMULATOR_MAX_MAP_SMS_SIM2_LEN; i++)
	{
		simulator_map_sms_sim2[i] = i;

		if(i > 20)
		{
			simulator_map_sms_sim2[i] = SIMULATOR_INVALID_INDEX;
		}
	}
}

void simulator_init_map_sms_me2()
{
	INT32 i;

	for (i = 0; i < SIMULATOR_MAX_MAP_SMS_ME2_LEN; i++)
	{
		simulator_map_sms_me2[i] = i;

		if(i > 20)
		{
			simulator_map_sms_me2[i] = SIMULATOR_INVALID_INDEX;
		}
	}
}

U8* simulator_list_map_sms_sim2(U8 startIndex, U8 count, U8* len)
{
	INT32 i = 0;
	INT32 length = 0;


	for (i = startIndex; i < startIndex + count; i++)
	{
		if (simulator_map_sms_sim2[i] != SIMULATOR_INVALID_INDEX)
		{
			length++;
		}
	}

	*len = length;

	return simulator_map_sms_sim2 + startIndex;
}

U8* simulator_list_map_sms_me2(U8 startIndex, U8 count,U8* len)
{
	INT32 i = 0;
	INT32 length = 0;
	
	for (i = startIndex; i < startIndex + count; i++)
	{
		if (simulator_map_sms_me2[i] != SIMULATOR_INVALID_INDEX)
		{
			length++;
		}
	}

	*len = length;

	return simulator_map_sms_me2 + startIndex;
}

CFW_SMS_NODE* simulator_get_sms_node(U8* listHead, U8 mapIndex)
{
	if (listHead >= &simulator_map_sms_sim2[0] && listHead <=  &simulator_map_sms_sim2[SIMULATOR_MAX_MAP_SMS_SIM2_LEN-1])
	{
		return simulator_get_sms_sim2_node(listHead, mapIndex);
	}
	else if (listHead >=& simulator_map_sms_me2[0] && listHead <= &simulator_map_sms_me2[ SIMULATOR_MAX_MAP_SMS_ME2_LEN-1])
	{
		return simulator_get_sms_me2_node(listHead, mapIndex);
	}

	return NULL;
}
#include "mmi_trace.h"
CFW_SMS_NODE* simulator_get_sms_sim2_node(U8* listHead, U8 mapIndex)
{
	UINT32 index; 

	if (listHead == NULL)
	{
		return NULL;
	}

	if (mapIndex >= SIMULATOR_MAX_MAP_SMS_SIM2_LEN)
	{
		return NULL;
	}

	index = listHead[mapIndex];

	if (index < g_simulator_sms_sim2.length)
	{
		return &g_simulator_sms_sim2.sms_node[index];
	}
	else
	{
		return NULL;
	}
}


CFW_SMS_NODE* simulator_get_sms_me2_node(U8* listHead, U8 mapIndex)
{
	UINT32 index; 

	if (listHead == NULL)
	{
		return NULL;
	}

	if (mapIndex >= SIMULATOR_MAX_MAP_SMS_SIM2_LEN)
	{
		return NULL;
	}

	index = listHead[mapIndex];

	if (index < g_simulator_sms_me2.length)
	{
		return &g_simulator_sms_me2.sms_node[index];
	}
	else
	{
		return NULL;
	}
}

void simulator_init_map_phb_sim2()
{
	INT32 i;

	for (i = 0; i < SIMULATOR_MAX_MAP_PHB_LEN; i++)
	{
		simulator_map_phb_sim2[i] = SIMULATOR_INVALID_INDEX;
	}
	
	for (i = 0; i < simulator_map_phb_used_count; i++)
	{
		simulator_map_phb_sim2[i] = i;
	}

}

CFW_SIM_PBK_ENTRY_INFO* simulator_get_phb_sim2_node(INT32 mapIndex)
{
	UINT32 index;

	if (mapIndex >= SIMULATOR_MAX_MAP_PHB_LEN)
	{
		return NULL;
	}

	index = simulator_map_phb_sim2[mapIndex];

	if (index != SIMULATOR_INVALID_INDEX)
	{
		if (index < g_simulator_phb_sim2.length)
		{
		
			return &g_simulator_phb_sim2.phb_node[index]; 
		}
		else
		{
			return NULL;
		}
	}
	else
	{
			return NULL;
	}
}

void simulator_get_sms_storage_info(CFW_SMS_STORAGE_INFO* storageInfo, UINT32 storageID)
{
	UINT8 i = 0;
	UINT8 index = 0;
	//UINT8 len = 0;
	CFW_SMS_NODE* pNode = NULL;
	
	memset(storageInfo, 0, sizeof(CFW_SMS_STORAGE_INFO));

	if (storageID == SIMULATOR_STORAGE_SIM2)
	{

		storageInfo->storageId = SIMULATOR_STORAGE_SIM2;
		storageInfo->totalSlot = SIMULATOR_MAX_MAP_SMS_SIM2_LEN;

		for (i = 0; i < SIMULATOR_MAX_MAP_SMS_SIM2_LEN; i++)
		{
			index = simulator_map_sms_sim2[i];
			if (index != SIMULATOR_INVALID_INDEX)
			{
				storageInfo->usedSlot++;
				pNode = &g_simulator_sms_sim2.sms_node[index];

				switch(pNode->nStatus)
				{
				case CFW_SMS_STORED_STATUS_READ:
					storageInfo->readRecords++;
					break;
				case CFW_SMS_STORED_STATUS_UNREAD:
					storageInfo->unReadRecords++;
					break;
				case CFW_SMS_STORED_STATUS_UNSENT:
					storageInfo->unsentRecords++;
					break;
				default: //the others type are not processed.
					break;
				}
			}

		}
	}
	else if (storageID == SIMULATOR_STORAGE_ME2)
	{
		storageInfo->storageId = SIMULATOR_STORAGE_ME2;
		storageInfo->totalSlot = SIMULATOR_MAX_MAP_SMS_ME2_LEN;
	
		for (i = 0; i < SIMULATOR_MAX_MAP_SMS_ME2_LEN; i++)
		{
			index = simulator_map_sms_me2[i];

			if (index != SIMULATOR_INVALID_INDEX)
			{
				storageInfo->usedSlot++;
				pNode = &g_simulator_sms_me2.sms_node[index];

				switch(pNode->nStatus)
				{
				case CFW_SMS_STORED_STATUS_READ:
					storageInfo->readRecords++;
					break;
				case CFW_SMS_STORED_STATUS_UNREAD:
					storageInfo->unReadRecords++;
					break;
				case CFW_SMS_STORED_STATUS_UNSENT:
					storageInfo->unsentRecords++;
					break;
				default: //the others type are not processed.
					break;
				}
			}
		

		}
	}
}

CFW_SMS_NODE* simulator_write_sms_node(CFW_SMS_WRITE* pSmsInfo)
{
	INT32 i;

	if (pSmsInfo->nStorage == SIMULATOR_STORAGE_SIM2)
	{
		for (i = 0; i < SIMULATOR_MAX_MAP_SMS_SIM2_LEN; i++)
		{
			if (simulator_map_sms_sim2[i] == SIMULATOR_INVALID_INDEX)
			{
				break;
			}
		}
		
		if (pSmsInfo->nStatus == CFW_SMS_STORED_STATUS_UNREAD)
		{
			simulator_map_sms_sim2[i] = i % SIMULATOR_SMS_SIM2_DELIVER;
		}
		else
		{
			simulator_map_sms_sim2[i] = SIMULATOR_SMS_SIM2_DELIVER + i % SIMULATOR_SMS_SIM2_SUBMIT;
		}

		if (simulator_map_sms_sim2[i] < g_simulator_sms_sim2.length)
		{
			return &g_simulator_sms_sim2.sms_node[simulator_map_sms_sim2[i]];
		}
		else
		{
			return NULL;
		}
	}
	else if (pSmsInfo->nStorage == SIMULATOR_STORAGE_ME2)
	{
		for (i = 0; i < SIMULATOR_MAX_MAP_SMS_ME2_LEN; i++)
		{
			if (simulator_map_sms_me2[i] == SIMULATOR_INVALID_INDEX)
			{
				break;
			}
		}

		if (pSmsInfo->nStatus == CFW_SMS_STORED_STATUS_UNREAD)
		{
			simulator_map_sms_me2[i] = i % SIMULATOR_SMS_ME2_DELIVER;
		}
		else
		{
			simulator_map_sms_me2[i] = SIMULATOR_SMS_ME2_DELIVER + i % SIMULATOR_SMS_ME2_SUBMIT;
		}
		
		if (simulator_map_sms_me2[i] < g_simulator_sms_me2.length)
		{
			return &g_simulator_sms_me2.sms_node[simulator_map_sms_me2[i]];
		}
		else
		{
			return NULL;
		}
		
	}

	return NULL;
	
}

void simulator_delete_sms_node(U8 storageIndex, U8 storageType, U8 status)
{
	INT32 i;
	INT32 index;

	if (storageIndex != 0)
	{

		if (storageType == SIMULATOR_STORAGE_SIM2)
		{
	
			for (i = 0; i < SIMULATOR_MAX_MAP_SMS_SIM2_LEN; i++)
			{
				if (simulator_map_sms_sim2[i] == storageIndex)
				{
					break;
				}
			}

			simulator_map_sms_sim2[i] = SIMULATOR_INVALID_INDEX;
		
		} 
		else if (storageType == SIMULATOR_STORAGE_ME2)
		{
			for (i = 0; i < SIMULATOR_MAX_MAP_SMS_ME2_LEN; i++)
			{
				
				if (simulator_map_sms_me2[i] == storageIndex)
				{
					break;
				}
			}

			simulator_map_sms_me2[i] = SIMULATOR_INVALID_INDEX;
		}
	}
	else
	{
		if (storageType == SIMULATOR_STORAGE_SIM2)
		{
	
			for (i = 0; i < SIMULATOR_MAX_MAP_SMS_SIM2_LEN; i++)
			{
				index = simulator_map_sms_sim2[i];
				if (index != SIMULATOR_INVALID_INDEX && 
					(g_simulator_sms_sim2.sms_node[index].nStatus & status))
				{
					simulator_map_sms_sim2[i] = SIMULATOR_INVALID_INDEX;
				}
			}
			
		}
		else if (storageType == SIMULATOR_STORAGE_ME2)
		{
			for (i = 0; i < SIMULATOR_MAX_MAP_SMS_ME2_LEN; i++)
			{
				index = simulator_map_sms_me2[i];
				if (index != SIMULATOR_INVALID_INDEX && 
					(g_simulator_sms_me2.sms_node[index].nStatus & status))
				{
					simulator_map_sms_me2[i] = SIMULATOR_INVALID_INDEX;
				}
			}
			
		}
	}
	
}


INT32 simulator_add_phb_sim2_node(CFW_SIM_PBK_ENTRY_INFO* pPbkNode)
{
	INT32 i = 0 ;
	INT32 index = 0;
	CFW_SIM_PBK_ENTRY_INFO* pNode = NULL;

	for (i = 0; i < simulator_map_phb_total_count; i++)
	{
		if (simulator_map_phb_sim2[i] == SIMULATOR_INVALID_INDEX)
		{
			index = i;
			break;
		}
	}

	if (index == simulator_map_phb_total_count)
	{
		return -1;
	}
	
	simulator_map_phb_sim2[index] = index;

	pNode =  &g_simulator_phb_sim2.phb_node[index];

	pNode->nType = pPbkNode->nType;
	memcpy(pNode->pFullName, pPbkNode->pFullName, 4);
	
	if (pPbkNode->iFullNameSize > 4)
	{
		pNode->iFullNameSize = 4;
	}
	else
	{
		pNode->iFullNameSize = pPbkNode->iFullNameSize;
	}

	pNode->pFullName[pNode->iFullNameSize] = '\0';
	memcpy(pNode->pNumber, pPbkNode->pNumber, pPbkNode->nNumberSize);
	pNode->nNumberSize = pPbkNode->nNumberSize;

	pNode->phoneIndex = index + 1;

	return index;
}

void simulator_del_phb_sim2_node(INT32 mapIndex)
{
	simulator_map_phb_sim2[mapIndex] = SIMULATOR_INVALID_INDEX;
}


INT32 simulator_get_phb_total_count()
{
	return simulator_map_phb_total_count;
}

INT32 simulator_get_phb_used_count()
{
	return simulator_map_phb_used_count;
}

#endif

#endif
