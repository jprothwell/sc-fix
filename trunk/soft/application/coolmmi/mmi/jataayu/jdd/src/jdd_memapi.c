#define MODULE_NAME "JDD_MEMAPI" 
#define FILE_NAME "jdd_memapi.c"
 /***************************************************************************
 *
 * File Name jdd_memapi.c
 *
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software Ltd., Bangalore ("Jataayu") 
 * or Jataayu's suppliers as the case may be. The Software is protected by 
 * copyright, including without limitation by applicable copyright laws, 
 * international treaty provisions, other intellectual property laws and 
 * applicable laws in the country in which the Software is being used. 
 * You shall not modify, adapt or translate the Software, without prior 
 * express written consent from Jataayu. You shall not reverse engineer, 
 * decompile, disassemble or otherwise alter the Software, except and 
 * only to the extent that such activity is expressly permitted by 
 * applicable law notwithstanding this limitation. Unauthorized reproduction 
 * or redistribution of this program or any portion of it may result in severe 
 * civil and criminal penalties and will be prosecuted to the maximum extent 
 * possible under the law. Jataayu reserves all rights not expressly granted. 
 * 
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY 
 * KIND AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY ACCURACY OF INFORMATIONAL CONTENT, AND/OR FITNESS 
 * FOR A PARTICULAR PURPOSE OR USE, TITLE OR INFRINGEMENT ARE EXPRESSLY 
 * DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. YOU ASSUME THE 
 * ENTIRE RISK AS TO THE ACCURACY AND THE USE OF THIS SOFTWARE. JATAAYU 
 * SHALL NOT BE LIABLE FOR ANY CONSEQUENTIAL, INCIDENTAL, INDIRECT, 
 * EXEMPLARY, SPECIAL OR PUNITIVE DAMAGES INCLUDING WITHOUT LIMITATION 
 * ANY LOSS OF DATA, OR; LOSS OF PROFIT, SAVINGS BUSINESS OR GOODWILL 
 * OR OTHER SIMILAR LOSS RESULTING FROM OR OUT OF THE USE OR INABILITY 
 * TO USE THIS SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 ***************************************************************************/
 
 /** 
 * @file	jdd_memapi.c
 * @ingroup	jdd_memapi
 * @brief	Describes the memory related API's.
 **/
  
 /***************************************************************************
 * System Include Files
 **************************************************************************/
#ifdef MMI_ON_HARDWARE_P
#include "cs_types.h"
#include "sxr_mem.h"
#else
#include "cswtype.h"
#endif
#include "cos.h"
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/
 #include <ddl.h>
 #include <jcal.h>
 /***************************************************************************
 * Macros
 **************************************************************************/
#ifdef JDD_LOG_ENABLED
#define __MODULE_ID__					MEM_MODULE
#else
#define __MODULE_ID__					0
#endif
 
/* 
  * By default, logs are disabled, because log module uses memory module.
  * If logs are necessary for Memory Module, PLATFORM_LOG0 should be mapped
  * to platform trace function which doesn't uses jdd_Mem Functions.
  */
 #define PLATFORM_LOG0(pcMessage)




#ifdef ENABLE_MEM_LEAK_FINDER

/* File name length */
#define MAX_JDDMEM_FILE_NAME	24
 
 /* Data structure stores memory allocation details in linked list	*/
 typedef struct _st_MemNode
 {
	  void	  *pv_Memory;								 /* Memory pointer */
	  JC_UINT32 us_AllocSize;					 /* Memory allocated size */
	  JC_UINT32 us_LineNo;							 /* Callers line number  */   
	  JC_UINT32 us_PrevLineNo;						 /* Previous caller number */
	  JC_UINT8   sz_FileName[MAX_JDDMEM_FILE_NAME];		 /* Callers file name */
	  JC_UINT8   sz_PrevFileName[MAX_JDDMEM_FILE_NAME]; /* previous Callers file name */
	  struct  _st_MemNode *pNextmemNode;		 /* Next node in the list */
 }t_st_MemNode;
 
 /* Start node identifier */
 static t_st_MemNode * G_pst_StartMemnode ;
 /* Flag stores the status of mem leak finder tool	*/
 static JC_UINT32 G_uc_EnableMemLeakFinder = 0;
 
 
 void jddmem_EnableMemLeakFinder (void);
 void jddmem_DisableMemLeakFinder (void);
 void jddmem_DisplayMemLeakDetails (JC_UINT8 ucMeamLeakStat);
 JC_UINT32 jddmem_Append2MemNodeList(t_st_MemNode **q, void *pv_Memory,JC_UINT32 us_AllocSize,
							   JC_UINT32 us_LineNo, JC_UINT8* pc_FileName );
  void jddmem_DeleteAllMemNodes( t_st_MemNode *q);
 void jddmem_DeleteMemNode( t_st_MemNode **q, void *pv_Memory);
 
#endif /* ENABLE_MEM_LEAK_FINDER */


 
 /***************************************************************************
 * Global Variable declarations and definitions
 **************************************************************************/
 
/**
* @brief  g_uiMaxSize defines the maximum memory size in Bytes that can be 
* allocated at an instance. If user wants to use the platform defined limit, 
* the value should be configured from jdd_MemInit().
*/
/*JC_UINT32 g_uiMaxSize = <To be configured; for e.g. 100> ; */

 /***************************************************************************
 * Local Function Prototypes
 **************************************************************************/
 
 
 /***************************************************************************
 * All Local Function Definitions
 **************************************************************************/
 
 
 /***************************************************************************
 * All Global Function Definitions
 **************************************************************************/

/**
 * @ingroup		jdd_memapi
 * @brief 		This function is used to initialize the memory layer. 
 * @retval		JC_OK on success
 * @retval		JC_ERR_MEM_INIT on failure
 *
 * This function needs to be invoked by the application before calling any of 
 * the memory API calls specified in this document or invoking the Jataayu 
 * core product engine. If the platform does not require any initialization 
 * this call can return the success value. 
 * @see 		#jdd_MemDeinit 
 */
 
JC_RETCODE jdd_MemInit (void)
{
	JC_RETCODE	rCode ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	PLATFORM_LOG0("jdd_MemInit : Entry") ;

	/*--- Platform Specific Implementation Start ---*/

	/* g_uiMaxSize = (Platform call to get the memory block size limit ) */
	
	rCode = JC_OK ;
	/*--- Platform Specific Implementation End ---*/
	
	PLATFORM_LOG0("jdd_MemInit : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup		jdd_memapi
 * @brief		This function is used to de-initialize the Memory layer. 
 * @retval		JC_OK on success
 * @see			#jdd_MemInit 
 */

JC_RETCODE jdd_MemDeinit (void)
{	
	JC_RETCODE	rCode ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	PLATFORM_LOG0("jdd_MemDeinit : Entry") ;

	/*--- Platform Specific Implementation Start ---*/

	rCode = JC_OK ;	
	/*--- Platform Specific Implementation End ---*/

	PLATFORM_LOG0("jdd_MemDeinit : Exit") ;
	return (rCode) ;
}

/**
 * @ingroup		jdd_memapi
 * @brief		This function is used to allocate memory
 * @param[in]	uiItemSize	Specifies the number of elements
 * @param[in]	uiSize	Specifies the length in bytes of each elements
 * @retval		Valid memory reference
 * @retval		NULL in case of error.
 *
 * This function will allocate the required memory if avaliable
 * else will return null if not avaliable. 
 */
#ifdef ENABLE_MEM_LEAK_FINDER
void *	jdd_MemAlloc_Debug(JC_UINT32 uiItemSize, JC_UINT32 uiSize,const JC_UINT8 *pc_fname, JC_INT32 l_line)
#else
void *jdd_MemAlloc (JC_UINT32 uiItemSize, JC_UINT32 uiSize)
#endif
{
	void *vpPtr = JC_NULL ;
	JC_UINT32 memsize = 0;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/
	     u32  sxr_CallerAdd;
   COS_GET_RA(&sxr_CallerAdd);

	PLATFORM_LOG0 ("jdd_MemAlloc : Entry") ;	

	jdd_AssertOnFail ((0 != uiItemSize) && (0 != uiSize), \
						"jdd_MemAlloc: Invalid Input Params") ;
	
	/*--- Platform Specific Implementation Start ---*/

	/*	
	 * If platform insists limit on maximum block size that can be 
	 * allocated at once, following else-if statement can be uncommented.
	 * g_uiMaxSize can be initialized to platform defined value in 
	 * jdd_MemInitialize
	 */
	 memsize = uiItemSize * uiSize;

	vpPtr = COS_SHMEMMALLOC (uiItemSize * uiSize);

	if(vpPtr)
	{
		jc_memset(vpPtr,0,uiItemSize * uiSize) ;
	      COS_UpdateMemCallAdd(vpPtr,sxr_CallerAdd);

#ifdef ENABLE_MEM_LEAK_FINDER
		if(G_uc_EnableMemLeakFinder)
		{
			/* Add the memory allocated details in to the mem leak finder list */
			jddmem_Append2MemNodeList(&G_pst_StartMemnode, vpPtr, uiItemSize * uiSize, l_line, (JC_UINT8*)pc_fname) ;
		}
#endif /* ENABLE_MEM_LEAK_FINDER */
	}
	else
	{
		INT i ;
		for (i=0; i <= 50; i++)
		{
			mmi_trace (1, "!!!!!!! MEM FAIL for %d B !!!!!!!!", uiItemSize * uiSize) ;
		}

		/*INT i ;
		INT *p = 0 ;
		
		for (i=0; i <= 500; i++)
		{
			mmi_trace (1, "!!!!!!! MEM FAIL !!!!!!!!") ;
		}

		*p = i ;
		*/
	}

	/*	
	if ((uiItemSize * uiSize) > g_uiMaxSize) 
	{
		vpPtr = JC_NULL ;
	}
		
	else 
	*/
	{
		/* if the platform supports calloc use following statement */
		
		/* vpPtr =  PlatForm Specific Function for calloc*/
		
		/* if the platform doesn't support calloc, use following statements */
		
		/* vpPtr =  PlatForm Specific Function for malloc
		jc_memset(vpPtr, 0, sizeof (JC_CHAR)) ;*/
		/*if it is not working sizeof(JC_CHAR) properly with platform then 
		   set all the bytes to zero (ie uiItemSize * uiSize)*/
	}

	/*--- Platform Specific Implementation End ---*/

	PLATFORM_LOG0 ("jdd_MemAlloc : Exit") ; 
	return (vpPtr) ;
}

/**
 * @ingroup		jdd_memapi
 * @brief 		This function deallocates the specified memory block
 * @param[in]	vpPtr Specifies the memory reference which needs to 
 *				be deallocated
 *
 * The memory reference allocated using jdd_MemAlloc shall be freed using
 * this function. After calling this function, the memory reference becomes 
 * invalid and the application should not refer it.
 * @see			#jdd_MemAlloc 
 */
#ifdef ENABLE_MEM_LEAK_FINDER
void jdd_MemFree_Debug(void *vpPtr, const JC_UINT8 *pc_fname, JC_INT32 l_line)
#else
void jdd_MemFree (void *vpPtr)
#endif
{
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	PLATFORM_LOG0("jdd_MemFree : Entry") ;

	/*--- Platform Specific Implementation Start ---*/
	
	/* if (JC_NULL != vpPtr) */  /*This check might be an overhead */
		/* PlatForm Specific Function for memory free */
	if (vpPtr) COS_SHMEMFREE (vpPtr);
	
#ifdef ENABLE_MEM_LEAK_FINDER
			if(G_uc_EnableMemLeakFinder)
			{
				/* Remove the memory freed details from the mem leak finder list */
				 jddmem_DeleteMemNode(&G_pst_StartMemnode, vpPtr) ;
			}
#endif /* ENABLE_MEM_LEAK_FINDER */

	/*--- Platform Specific Implementation End ---*/

	PLATFORM_LOG0("jdd_MemFree : Exit") ;
}

/**
 * @ingroup		jdd_memapi
 * @brief 		This function re-allocates the memory.
 * @param[in] 	vpPtr Specifies memory reference which needs to be reallocated
 * @param[in] 	uiSize Specifies the new size that is required.
 * @retval		Valid memory reference
 * @retval		NULL in case of error.
 *
 * The requested memory is reallocated and returned by this function. 
 * On success the starting address of the memory allocated is returned, 
 * otherwise NULL is returned. 
 * @warning		Recommended not to use this function.
 * @see			#jdd_MemAlloc 
 */ 

void * jdd_MemRealloc (void* vpPtr, JC_UINT32 uiSize)
{
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	PLATFORM_LOG0("jdd_MemRealloc : Entry") ;
	jdd_AssertOnFail ((0 ), "jdd_MemRealloc: Function not implemented") ;

	/*--- Platform Specific Implementation Start ---*/
	/*	
	if ((0 == uiSize) && (JC_NULL != vpPtr))
	{
		jdd_MemFree (vpPtr) ;
		vpPtr = JC_NULL ;
	}
	*/
/*	If platform insists limit on maximum block size that 
	can be allocated at once, following else-if statement
	can be uncommented. g_uiMaxSize can be initialized to
	platform defined value in jdd_MemInitialize*/
	/*
	else if (  uiSize > g_uiMaxSize) 
	{
		vpPtr = JC_NULL ;
	}
*/	
	/*else*/
	{
		/* Platform Specific Call for re-allocating memory  */
	}
	
	/*--- Platform Specific Implementation End ---*/

	PLATFORM_LOG0("jdd_MemRealloc : Exit") ;
	return (vpPtr) ;
}



#ifdef ENABLE_MEM_LEAK_FINDER

enum
{
	JMMS_MEMLEAK_REINIT,
	JMMS_MEMLEAK_CONTINUE
} ;

void jddmem_EnableMemLeakFinder (void)
{
	mmi_trace(1,"JDD_MEM:Enabling Leak finder") ;
	/* Enable the mem leak finder */
	G_uc_EnableMemLeakFinder = 1;
}

void jddmem_DisableMemLeakFinder (void)
{
	mmi_trace(1,"JDD_MEM:Disabling Leak finder\n") ;
    /* Disable the mem leak finder */
	G_uc_EnableMemLeakFinder = 0;
	/* Dispaly the memory leak statics on console */
	jddmem_DisplayMemLeakDetails (JMMS_MEMLEAK_REINIT) ;
}

void jddmem_ShowMemLeakFinder (void)
{
	mmi_trace(1,"JDD_MEM:Disabling Leak finder\n") ;

   /* Dispaly the memory leak statics on console */
	jddmem_DisplayMemLeakDetails (JMMS_MEMLEAK_CONTINUE) ;
}

void jddmem_DisplayMemLeakDetails (JC_UINT8 ucMeamLeakStat)
{
	t_st_MemNode *prev = NULL, *temp = G_pst_StartMemnode , *prevprev = NULL;

	if(temp == NULL)
	{
    	mmi_trace(1,"JDD_MEM:*** No Memory Leaks Found ***") ;
	    return;
	}
	    
	mmi_trace(1,"JDD_MEM:*** Stats of Memory Leaks ****\n");

	// replace the loop with file write to log the result in a file
	while ( temp != NULL )
	{
		prevprev = prev;
		prev = temp;
		temp = temp ->pNextmemNode ;
		mmi_trace(1,"JDD_MEM: Memory Allocated in Line %d,  File %s of size %d is not freed. prev allocation is in Line %d, file:%s",
						prev->us_LineNo, prev->sz_FileName,prev->us_AllocSize,prev->us_PrevLineNo, prev->sz_PrevFileName);
		//mmi_trace(1," JDD_MEM: prevprev alloc %d , %s ", prevprev->us_LineNo, prevprev->sz_FileName );
	   //COS_SHMEMFREE (prev);
	}

	if(ucMeamLeakStat == JMMS_MEMLEAK_REINIT)
	{
		// remove the nodes which are leaked , as the leak  information is generated
		//jddmem_DeleteAllMemNodes(G_pst_StartMemnode);
		G_pst_StartMemnode = NULL ;
	}
	else
	{
		// do nothing
	}

	mmi_trace(1,"JDD_MEM:*** End of Memory Leak status***\n");
}



JC_UINT32 jddmem_Append2MemNodeList(t_st_MemNode **q, void *pv_Memory,JC_UINT32 us_AllocSize,
							  JC_UINT32 us_LineNo, JC_UINT8* pc_FileName )
{

	t_st_MemNode *temp = *q, *prev ;
	JC_UINT8 *tempFileNameptr = NULL ;
	JC_UINT8  tempFileName[MAX_JDDMEM_FILE_NAME] = {0,};
    JC_UINT32	uc_FileNameLen =0 ;

	uc_FileNameLen = jc_strlen (pc_FileName) ;

	if(uc_FileNameLen > MAX_JDDMEM_FILE_NAME )
	{
	   tempFileNameptr = pc_FileName + (uc_FileNameLen - MAX_JDDMEM_FILE_NAME );
	   jc_strncpy (tempFileName, tempFileNameptr, MAX_JDDMEM_FILE_NAME -1);
	}
	else
	{
	    jc_strncpy (tempFileName, pc_FileName , MAX_JDDMEM_FILE_NAME -1);
	}    
	

	/* if list is empty, create first node */
 	if ( *q == NULL )
	{
		*q = (t_st_MemNode *) COS_SHMEMMALLOC (sizeof(t_st_MemNode));
  		if( *q == NULL)
		  {
		     goto error;
		  }         
		jc_memset(*q, 0 , sizeof(t_st_MemNode));
		temp = *q ;
	}
	else
	{
		/* go to the last node */
		while (temp -> pNextmemNode != NULL )
			temp = temp -> pNextmemNode ;

		/* add node at the end */

		temp -> pNextmemNode = (t_st_MemNode *)COS_SHMEMMALLOC (sizeof(t_st_MemNode));
  		if(!temp->pNextmemNode)
			goto error;      
		jc_memset(temp -> pNextmemNode, 0 , sizeof(t_st_MemNode));
		prev = temp;
		temp = temp ->pNextmemNode ;
		temp->us_PrevLineNo = prev->us_LineNo;
		jc_strcpy (temp->sz_PrevFileName , prev->sz_FileName );

	}

	temp->pv_Memory = pv_Memory ;
	temp->us_AllocSize = us_AllocSize;
	temp->us_LineNo = us_LineNo;
	jc_strcpy(temp->sz_FileName, tempFileName);
	temp->pNextmemNode = NULL ;
	return 0 ; 

error:
	return -1 ;

}



void jddmem_DeleteAllMemNodes( t_st_MemNode *q)
{
	 t_st_MemNode *prev = NULL ;

	if( q== NULL)
		return;

	while ( q != NULL )
	{
		prev = q;
		q = q ->pNextmemNode ;
		if (prev != NULL)
		{
			COS_SHMEMFREE (prev);
		}
	}
}

void jddmem_DeleteMemNode( t_st_MemNode **q, void *pv_Memory)
{
        t_st_MemNode *prev = NULL, * temp = *q ;
	if( *q== NULL)
		return;

	while ( temp != NULL )
	{
		if (temp ->pv_Memory  == pv_Memory)
		{
			/* If first node in the list*/
			if(temp == *q)
			{
			   *q = temp ->pNextmemNode ;
			}
			else
			{
			    prev->pNextmemNode = temp ->pNextmemNode;
			}
			COS_SHMEMFREE (temp);
			break;
		}
		prev = temp;
		temp = temp->pNextmemNode ;
	}
	
}

#endif


/* End of File */
