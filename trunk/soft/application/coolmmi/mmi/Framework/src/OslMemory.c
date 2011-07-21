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
 *  OslMemory.c
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: OslMemory.c

  	PURPOSE		: Memory Wrapper

 

 

	DATE		: Jan' 21, 2003

**************************************************************/



/*--------------------------------------------------------------------
Include Files
----------------------------------------------------------------------*/

#include		"oslmemory.h"
#include		"datetimetype.h"
#include		"datetimegprot.h"
#include		"mmi_data_types.h"
#include		"stdc.h"
#include		"debuginitdef.h"


#ifdef OSL_MEMORY_DUMP		// if OSL_MEMORY_DUMP is not enabled then don't compile
#define BUFFER_POOLS 16
#define MEMORY_LIST_LENGTH 50

/*--------------------------------------------------------------------
Data Structure Declerations
-----------------------------------------------------------------------*/



typedef struct oslmemalloc {				// info
	S8 fileName[20];
	S32 lineNumber;
	S32	bytes;
	void *ptr;
	U8 isAllocated;
} OSLMEMALLOC;

S32 goslMemCount = 0;						// Total Allocated heap mem
S32 MAX_BUFF =0; 
S32 MaxDynamicMemUsed=0;

static OSLMEMALLOC memoryList[MEMORY_LIST_LENGTH];
static U16 bufferCount[BUFFER_POOLS];
static U8 memoryListCount=0;	
static U8 MallocFirstTime=1;

/*--------------------------------------------------------------------
Function Declerations
-----------------------------------------------------------------------*/

void *OslMallocCHK (long nob, char* fileName, int lineNumber, ...);
void OslMfreeCHK(void* frp);
void OslIntMemoryStart();
void OslIntDumpDataInFile(void);
long OslIntMemCounter(void);

/*-------------------------------------------------------------------
Function Name	:	ByteCounter
Description		:	This inline function keeps track of heap memory 
					allocations in bytes
Input			:	Pointer
					Flag : 1 to add no. of bytes
						  -1 to delete no. of bytes
Output			:	None
--------------------------------------------------------------------*/
void ByteCounter(void*	ptr,int	flag, int size)	// -1 free, +1 alloc
{
	goslMemCount += (long) (size * flag);
	if(MaxDynamicMemUsed<goslMemCount)
		MaxDynamicMemUsed=goslMemCount;
	return;
}
										
										
/*-------------------------------------------------------------------
Function Name	:	OslMallocCHK 
Description		:	This function allocates memory and maintain the
					link list info of file name and line number
Input			:	no of bytes
					file name
					line number
Output			:	void pointer					
-------------------------------------------------------------------*/

void* OslMallocCHK (S32 nob, char* fileName, int lineNumber, ...)
{
	void* ptr = NULL;
	U8 j,i;
	S32 nob1;//=nob;
	void *ptrAct;
	U16 maskingByte;
	OSLMEMALLOC *memData;
	if(MallocFirstTime)
	{
		OslIntMemoryStart();
		MallocFirstTime=0;
	}
	if(!nob) return NULL;
	if(nob<8)
		nob=8;
	nob1=nob;
	maskingByte=1;
	i=0;
	for(j=0;j<16;j++)
	{
		if(nob1 & maskingByte) i++;

		if(!(nob1 & 0xfffe)) break;
		nob1>>=1;
	}

	if(i>1)
	{
		nob1=1;
		nob1<<=++j;
	}
	else
		nob1=nob;	
	
	bufferCount[j-3]++;

	#ifdef MMI_ON_WIN32
		ptrAct = (void*)malloc(nob1); 
	#else
		ptrAct = (void*)OslMalloc(nob1); 
	#endif	
		
	if(memoryListCount<MEMORY_LIST_LENGTH)
		{
		for(j=0;memoryList[j].isAllocated;j++);
		memData=&memoryList[j];
		memoryListCount++;
		for(j=strlen(fileName);j>1;j--)
		{
			if(fileName[j-1]=='\\') break;
		}
		memcpy(memData->fileName,&fileName[j],strlen(fileName)-j+1);
		//strcpy(memData.fileName, (char *)fileName);
		memData->lineNumber = lineNumber;	
		memData->bytes = nob1;
		memData->ptr=ptrAct;
		memData->isAllocated=1;
		ByteCounter(ptr,1,nob1);
		
	}
	else
	{
		PRINT_INFORMATION(("File size exceeded")); 
	
	}

	if (MAX_BUFF < nob1)
		MAX_BUFF = nob1;
	return ptrAct;
}

/*-------------------------------------------------------------------
Function Name	:	osl_Mfree
Description		:	This osl_Mfree's the poinetr along with node from the link 
					list if present.
Input			:	Ponter to be freed
Output			:	None
-------------------------------------------------------------------*/
void OslMfreeCHK(void* frp)
{
	U8 i=0,j=0,k=0;
	U16 maskingByte;
	for(j=0;memoryList[j].ptr != frp && j <= MEMORY_LIST_LENGTH;j++);
	if(j!=MEMORY_LIST_LENGTH)
	{
		maskingByte=1;
		for(i=0;i<16;i++)
		{
			if(memoryList[j].bytes & maskingByte) 
				break;
			maskingByte<<=1;
		}
		if(i<16)
			bufferCount[i-3]--;
		ByteCounter(frp,-1, memoryList[j].bytes);
		memset(&memoryList[j],0, sizeof(OSLMEMALLOC));
		memoryListCount--;
	}
	#ifdef MMI_ON_WIN32
		free(frp);
	#else
		OslMfree(frp);
	#endif
	
	return;
}

/*-------------------------------------------------------------------
Function Name	:	osl_MemoryCHK
Description		:	This flag starts / stops building of the Memory 
					link list
Output			:	None
-------------------------------------------------------------------*/
void OslIntMemoryStart()
{
#ifdef OSL_MEMORY_DUMP
		memset(&memoryList,0, sizeof(memoryList));
		memset(&bufferCount,0,sizeof(bufferCount));
		memoryListCount=0;
#endif
	return;
}


/*-------------------------------------------------------------------
Function Name	:	OslMemCounter
Description		:	This flag starts / stops building of the Memory 
					link list
Input			:	TRUE : TO start Mem link list
					FALSE : To stop Mem link list
Output			:	None
-------------------------------------------------------------------*/
long OslIntMemCounter()
{
#ifdef OSL_MEMORY_DUMP
	return goslMemCount;
#else
	return 0;	
#endif
}

/*-------------------------------------------------------------------
Function Name	:	OslDumpDataInFile
Description		:	This Dumps memory link list data in the file 
					C:\oslMemoryDump
Input			:	None
Output			:	None
-------------------------------------------------------------------*/
void OslIntDumpDataInFile()
{
	MYTIME t;
	char fileName[128];
	U8 i;
#ifdef MMI_ON_WIN32
	FILE *fp;
#endif
	OSLMEMALLOC *ptr = NULL;
	
	strcpy(fileName, "C:\\oslMemoryDump.txt");

#ifdef MMI_ON_WIN32
	if((fp = fopen(fileName, "a+")) == NULL)
	{
		return;
	}
	GetDateTime(&t);
	fprintf(fp, "____________________________________________________________________________________________________________________________\n");
	fprintf(fp, "Memory Dump at time %d:%d:%d on %d-%d-%d\n",t.nHour,t.nMin,t.nSec,t.nDay,t.nMonth,t.nYear);
	fprintf(fp, "Maximum Buffer Allocated %ld\n", MAX_BUFF); 
	fprintf(fp, "Maximum Dynamic Memory used %ld\n",MaxDynamicMemUsed);
	fprintf(fp, "Total Memory In Use %ld\n",goslMemCount);
#endif
	PRINT_INFORMATION(("____________________________________________________________________________________________________________________________"));	
	PRINT_INFORMATION(("Memory Dump at time %d:%d:%d on %d-%d-%d \n \n",t.nHour,t.nMin,t.nSec,t.nDay,t.nMonth,t.nYear));
	PRINT_INFORMATION(("Maximum Buffer Allocated %ld", MAX_BUFF)); 
	PRINT_INFORMATION(("Maximum Dynamic Memory used %ld",MaxDynamicMemUsed));
	PRINT_INFORMATION(("Total Memory In Use %ld",goslMemCount));

	for(i=0;i<MEMORY_LIST_LENGTH;i++)
	{
		ptr=&memoryList[i];
		if(!ptr->isAllocated) continue;
			#ifdef MMI_ON_WIN32
				fprintf(fp, "File: [%s]        Line: [%d]        Size: [%d]\n",ptr->fileName, ptr->lineNumber,	ptr->bytes);
			#endif
			PRINT_INFORMATION(("File: [%s]        Line: [%d]        Size: [%d]",ptr->fileName, ptr->lineNumber,	ptr->bytes));
	}
	
	
	#ifdef MMI_ON_WIN32
		fprintf(fp, "BUFFER VISE ALLOCATION\n"); 
		fprintf(fp, "Number of buffers of size 8: [%d]\n",bufferCount[0]);
		fprintf(fp, "Number of buffers of size 16: [%d]\n",bufferCount[1]);
		fprintf(fp, "Number of buffers of size 32: [%d]\n",bufferCount[2]);
		fprintf(fp, "Number of buffers of size 64: [%d]\n",bufferCount[3]);
		fprintf(fp, "Number of buffers of size 128: [%d]\n",bufferCount[4]);
		fprintf(fp, "Number of buffers of size 256: [%d]\n",bufferCount[5]);
		fprintf(fp, "Number of buffers of size 512: [%d]\n",bufferCount[6]);
		fprintf(fp, "Number of buffers of size 1024: [%d]\n",bufferCount[7]);
		fprintf(fp, "Number of buffers of size 2048: [%d]\n",bufferCount[8]);
		fprintf(fp, "Number of buffers of size 4096: [%d]\n",bufferCount[9]);
		fprintf(fp, "Number of buffers of size 8192: [%d]\n",bufferCount[10]);
		fprintf(fp, "Number of buffers of size 16384: [%d]\n",bufferCount[11]);
		fprintf(fp, "Number of buffers of size 32768: [%d]\n",bufferCount[12]);
		fprintf(fp, "Number of buffers of size 65536: [%d]\n",bufferCount[13]);
	#endif
		PRINT_INFORMATION(("Number of buffers of size 8: [%d]",bufferCount[0]));
		PRINT_INFORMATION(("Number of buffers of size 16: [%d]",bufferCount[1]));
		PRINT_INFORMATION(("Number of buffers of size 32: [%d]",bufferCount[2]));
		PRINT_INFORMATION(("Number of buffers of size 64: [%d]",bufferCount[3]));
		PRINT_INFORMATION(("Number of buffers of size 128: [%d]",bufferCount[4]));
		PRINT_INFORMATION(("Number of buffers of size 256: [%d]",bufferCount[5]));
		PRINT_INFORMATION(("Number of buffers of size 512: [%d]",bufferCount[6]));
		PRINT_INFORMATION(("Number of buffers of size 1024: [%d]",bufferCount[7]));
		PRINT_INFORMATION(("Number of buffers of size 2048: [%d]",bufferCount[8]));
		PRINT_INFORMATION(("Number of buffers of size 4096: [%d]",bufferCount[9]));
		PRINT_INFORMATION(("Number of buffers of size 8192: [%d]",bufferCount[10]));
		PRINT_INFORMATION(("Number of buffers of size 16384: [%d]",bufferCount[11]));
		PRINT_INFORMATION(("Number of buffers of size 32768: [%d]",bufferCount[12]));
		PRINT_INFORMATION(("Number of buffers of size 65536: [%d]",bufferCount[13]));
	
	PRINT_INFORMATION(("____________________________________________________________________________________________________________________________"));
#ifdef MMI_ON_WIN32
	fclose(fp);
#endif
	return;
}

#endif
/*-------------------------------------------------------------------
				end of file OslMemory.C
---------------------------------------------------------------------*/


