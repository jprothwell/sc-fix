#include "sxs_srl.h"
#include "sxs_type.h"
#include "sxs_lib.h"
#include "sxr_ops.h"
#include "errorcode.h"
#include "sxr_mem.hp"
//#define HAVE_NO_TS_IF
#include "ts.h"
#include "cswtype.h"
#include "csw.h"
#include "csw_config.h"
#include "sxs_io.h"
#include "sxr_mem.h"
#include "sxr_usrmem.h"

extern MMI_Default_Value g_MMI_Default_Value ;
//extern INT32 CSW_TRACE(UINT32 id, CONST INT8 *fmt, ...);
extern PUBLIC CONST CSW_CONFIG_T* tgt_GetCswConfig(VOID);

//#define     CSW_HEAP_SIZE_MALLOC      (180*1024)

#define     CSW_HEAP_INDEX             2
#define     COS_HEAP_INDEX             3



PVOID   g_CosBaseAdd    = NULL ;
PVOID   g_CswBaseAdd    = NULL ;


UINT32  g_CosHeapSize   = 0 ;
UINT32  g_CswHeapSize   = 0 ;



UINT32  g_CswMallocNum  = 0 ;
UINT32  g_CosMallocNum  = 0 ;
UINT32  g_MDIMallocNum  = 0 ;



VOID  COS_UpdateMemCallAdd( VOID *pMemeAddr, UINT32 nCallAdd)
{
	sxr_UpMemCallerAdd(pMemeAddr, nCallAdd);
}


PVOID Stack_MsgMalloc  (
  UINT32 nSize
)
{
  PVOID pointer = 0x00;
  UINT32  nCallerAdd = 0x00;
  COS_GET_RA(&nCallerAdd);

  pointer = _sxr_HMalloc(nSize, 0xFF);
  if (pointer == NULL)
  {
    CSW_TRACE(BASE_BAL_TS_ID, "Stack_MsgMalloc failed size 0x%x\n", nSize);
  }
  else
    COS_UpdateMemCallAdd(pointer, nCallerAdd);
  
  return pointer;
   
}
PVOID CSW_Malloc(UINT32 nSize)
{

  PVOID pointer = NULL;
  UINT32  nCallerAdd;
  COS_GET_RA(&nCallerAdd);

  if (nSize >= g_CswHeapSize)
    return NULL;
  g_CswMallocNum++;
  pointer = _sxr_HMalloc(nSize, CSW_HEAP_INDEX);
  if (pointer == NULL)
  {
    CSW_TRACE(BASE_BAL_TS_ID, "CSW_Malloc failed size 0x%x\n", nSize);
  }
  else
     COS_UpdateMemCallAdd(pointer,nCallerAdd);
  

  return pointer;
}

BOOL CSW_Free ( 
  PVOID pMemBlock
)
{

  if ((((UINT32)(UINT32*)pMemBlock) >= (UINT32)g_CswBaseAdd)
      && (((UINT32)(UINT32*)pMemBlock) < (UINT32)(g_CswBaseAdd + g_CswHeapSize)))
  {
    g_CswMallocNum--;

    // CSW_TRACE(BASE_COS_TS_ID, TSTXT("CSW_Free pMemBlock = %x g_CswMallocNum %d\n"), pMemBlock, g_CswMallocNum );
    sxr_Free(pMemBlock);
    return TRUE;
  }
  return FALSE;
}
BOOL Stack_MsgFree ( 
  PVOID pMemBlock
)
{
    sxr_Free(pMemBlock);
    return TRUE ;
}

PVOID  CSW_MallocDbg (UINT32 nSize, CONST UINT8* pszFileName, UINT32 nLine)
{
	UINT8* p = CSW_Malloc(nSize);
	CSW_TRACE(BASE_COS_TS_ID, "CSW_MallocDbg+: 0x%08x, size %d, %s[%d]   %d\n", p, nSize, pszFileName, nLine, g_CswMallocNum);
    return (PVOID)p;
}

BOOL CSW_FreeDbg (VOID* pMemAddr, CONST UINT8* pszFileName, UINT32 nLine)
{
    CSW_Free(pMemAddr);
    CSW_TRACE(BASE_COS_TS_ID, "CSW_FreeDbg-: 0x%08x, %s[%d]   %d \n", pMemAddr, pszFileName, nLine, g_CswMallocNum);
    return TRUE ;
}


// ============================================================================= 
// COS_Malloc 
// ----------------------------------------------------------------------------- 
/// For application malloc
// ============================================================================= 

PVOID COS_Malloc(UINT32 nSize, COS_HEAP_ID nHeapID)
{
   UINT32 nCallerAdd;
   COS_GET_RA(&nCallerAdd);

  if(( 0x02 == nHeapID) || ( 0x01 == nHeapID))
  {
    nHeapID = COS_MMI_HEAP;
  }
  switch(nHeapID)  
  {
    case COS_MMI_HEAP:
    {
      if (nSize >= g_CosHeapSize)
        return NULL;

      PVOID p = sxr_UsrMalloc(nSize);
    	if(!p)
    	{
    		p = _sxr_HMalloc(nSize, COS_HEAP_INDEX);
    	}
      if (p == NULL)
      {
        CSW_TRACE(BASE_BAL_TS_ID, "COS_Malloc Malloc failed, And the size is 0x%x\n", nSize);
      }
      else
	  	COS_UpdateMemCallAdd(p,nCallerAdd);

      return p;

    }
    break;
    


    default:
      return NULL;
  }
  

}

BOOL COS_Free(PVOID pMemBlock)
{

  if ((((UINT32)(UINT32*)pMemBlock) >= (UINT32)g_CosBaseAdd)
      && (((UINT32)(UINT32*)pMemBlock) < (UINT32)(g_CosBaseAdd + g_CosHeapSize)))
  {
    g_CosMallocNum--;
    sxr_UsrFree(pMemBlock);
    return TRUE;
  }
  if ((((UINT32)(UINT32*)pMemBlock) >= (UINT32)g_CswBaseAdd)
      && (((UINT32)(UINT32*)pMemBlock) < (UINT32)(g_CswBaseAdd + g_CswHeapSize)))
  {
    g_CswMallocNum--;
    sxr_Free(pMemBlock);
    return TRUE;
  }


  return FALSE;
}

PVOID  COS_MallocDbg (UINT32 nSize, COS_HEAP_ID nHeapID, CONST UINT8* pszFileName, UINT32 nLine)
{
	UINT8* p = COS_Malloc(nSize,nHeapID);
    
	CSW_TRACE(BASE_BAL_TS_ID, "COS_MallocDbg+: 0x%08x, size %d, %s[%d] \n", p, nSize, pszFileName, nLine);
    
    return (PVOID)p;
}

BOOL COS_FreeDbg (VOID* pMemAddr, CONST UINT8* pszFileName, UINT32 nLine)
{
    CSW_TRACE(BASE_BAL_TS_ID, "COS_FreeDbg-: 0x%08x, %s[%d] \n", pMemAddr, pszFileName, nLine);
    
    return COS_Free(pMemAddr);
}

VOID csw_MemIint(VOID)
{

    CONST CSW_CONFIG_T* cswConfig; 

    cswConfig = tgt_GetCswConfig();   

    g_CswHeapSize   = cswConfig->cswHeapSize; 
    g_CosHeapSize   = cswConfig->cosHeapSize;
	
    g_CswBaseAdd    = sxr_HMalloc(g_CswHeapSize) ;
    g_CosBaseAdd    = sxr_HMalloc(g_CosHeapSize) ;
	
    CSW_TRACE(BASE_BAL_TS_ID, "g_CswHeapSize 0x%x\n", g_CswHeapSize);
    CSW_TRACE(BASE_BAL_TS_ID, "g_CosHeapSize 0x%x\n", g_CosHeapSize);

    _sxr_NewHeap (g_CswBaseAdd, g_CswHeapSize, CSW_HEAP_INDEX);
    _sxr_NewHeap (g_CosBaseAdd, g_CosHeapSize, COS_HEAP_INDEX);


    sxr_NewUsrCluster(10, 250, COS_HEAP_INDEX);
    sxr_NewUsrCluster(24, 250, COS_HEAP_INDEX);
    sxr_NewUsrCluster(28, 250, COS_HEAP_INDEX);
    sxr_NewUsrCluster(32, 250, COS_HEAP_INDEX);

    sxr_NewUsrCluster(52, 250, COS_HEAP_INDEX);  
    sxr_NewUsrCluster(100, 200, COS_HEAP_INDEX);        
    sxr_NewUsrCluster(156, 200, COS_HEAP_INDEX);     
    sxr_NewUsrCluster(200, 20, COS_HEAP_INDEX);     
    sxr_NewUsrCluster(700, 50, COS_HEAP_INDEX);     
    sxr_NewUsrCluster(1024, 50, COS_HEAP_INDEX);     

}


// =============================================================================
// COS_PageProtectSetup
// -----------------------------------------------------------------------------
/// This function setups a protection page
///
/// @param nPageNum Name of the page we want to configure  
/// @param nMode Protection mode
/// @param nStartAddr Address of the beginning of the page
/// @param nEndAddr  End address of the page. This address is not included in 
/// the page
// =============================================================================  
PUBLIC UINT32 COS_PageProtectSetup( COS_PAGE_NUM nPageNum, COS_PAGE_SPY_MODE nMode, UINT32 nStartAddr, UINT32 nEndAddr )
{
  if(( nPageNum < COS_PAGE_NUM_4 )||( nPageNum > COS_PAGE_NUM_5 )|| ( nMode < COS_PAGE_NO_TRIGGER) || ( nMode > COS_PAGE_READWRITE_TRIGGER ))
  {
    return ERR_INVALID_PARAMETER;
  }
  
  hal_DbgPageProtectSetup( nPageNum, nMode, nStartAddr, nEndAddr );

  return ERR_SUCCESS;
}


// =============================================================================
// COS_PageProtectEnable
// -----------------------------------------------------------------------------
/// Enable the protection of a given page.
///
/// @param nPageNum Page to enable
// =============================================================================
PUBLIC UINT32 COS_PageProtectEnable( COS_PAGE_NUM nPageNum )
{
  if(( nPageNum < COS_PAGE_NUM_4 )||( nPageNum > COS_PAGE_NUM_5 ))
  {
    return ERR_INVALID_PARAMETER;
  }
  
  hal_DbgPageProtectEnable(nPageNum);

  return ERR_SUCCESS;
}


// =============================================================================
// COS_PageProtectDisable
// -----------------------------------------------------------------------------
/// Disable the protection of a given page
///
/// @param nPageNum Page to disable
// =============================================================================

PUBLIC UINT32 COS_PageProtectDisable( COS_PAGE_NUM nPageNum )
{
  if(( nPageNum < COS_PAGE_NUM_4 )||( nPageNum > COS_PAGE_NUM_5 ))
  {
    return ERR_INVALID_PARAMETER;
  }

  hal_DbgPageProtectDisable(nPageNum);

  return ERR_SUCCESS;
}


