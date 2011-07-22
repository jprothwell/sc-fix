
#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)

#include "dsm_cf.h"
#ifdef _MS_VC_VER_
#include "flash_simu.h"
#else
#include "hal_lps.h"
#include "hal_comregs.h"
#include "hal_fint.h"
#include "hal_timers.h"
#include "hal_dma.h"
#include <base_prv.h>
#include <drv_flash.h>
#include "ts.h"
#include "csw_csp.h"
#include "cfw_multi_sim.h"
#endif
#include "dsm_dbg.h"
#include "vds_cd.h"
#include "vds_dbg.h"
#include "vds_local.h"
#include "vds_tools.h"
#include "vds_api.h"
#include "vds_cache.h"

#ifdef _MS_VC_VER_
#define COS_FREE free
#define COS_MALLOC malloc
#define hal_SysExitCriticalSection(x) do{}while(0)
#define hal_EnterCriticalSection() 0 
#endif

// #define VDS_CACHE_MODE_INCREMENTAL  

// extern function.
extern void sxs_RegisterFsIdleHookFunc(void (*funcPtr)(void));
extern VOID hal_BootRegisterBeforeMonCallback(INT32 (*funcPtr)(VOID));
extern UINT32 SRVAPI CFW_CcGetCallStatus(
#ifdef CFW_DUAL_SIM
                                                            CFW_SIM_ID nSimId
#endif
                                                          );
extern BOOL mmi_vdoply_is_in_app(void);
extern BOOL mmi_audply_is_in_app(void);
extern PUBLIC BOOL hal_GetVocDma(void);
#ifdef __PRJ_WITH_SPIFLSH__
extern PUBLIC BOOL flash_spi_busy(void);
#endif
// local function.
VOID vds_CacheSemInit(VOID);
VOID vds_CacheWaitForSem(VOID);
VOID vds_CacheReleaseSem(VOID);
INT32 vds_FlushCacheNode(UINT32 flush_nr);

// macro.
#define VDS_CACHE_TRACE                                         13
#define VDS_CACHE_WRITE_TIME_FUTURE                (~0)

#define VDS_CACHE_STATUS_FREE                             0x00
#define VDS_CACHE_STATUS_WRITE                          0x01
#define VDS_CACHE_STATUS_REVERT                         0x02

// data structure.
typedef struct _vds_cache_node
{
    UINT32 partid;    
    UINT32 blknr;     
    UINT32 status;
    UINT8 pbuff[VDS_BLOCK_SIZE];
    struct _vds_cache_node* next;
}VDS_CACHE_NODE;

typedef struct _vds_block_cache
{
    UINT32 blkcnt;    
    VDS_CACHE_NODE* pnode;
}VDS_BLOCK_CACHE;

// global variable.
HANDLE g_hVdsCacheSem = 0;
VDS_BLOCK_CACHE g_cachenode = {0,NULL};

// Todo...

#define vds_CacheTrace CSW_TRACE

// api fuction body.

INT32 VDS_InitCache(VOID)
{
    vds_CacheSemInit();
    vds_InitCache();
    vds_SetFlushUnAlloc();          
    DSM_HstSendEvent(0xA1000000);
    return ERR_SUCCESS;
}


VOID VDS_CacheFlushAlloc(VOID)
{
    vds_SetFlushAlloc();
    sxs_RegisterFsIdleHookFunc(VDS_FlushCache);
	hal_BootRegisterBeforeMonCallback(VDS_FlushCacheAll);
    DSM_HstSendEvent(0xA2000000); 
}


INT32 VDS_WriteCacheBlock( UINT32 iPartId, UINT32 iBlkNr, UINT8*pBuff )
{  
    VDS_CACHE_NODE* pnode = NULL;    
    VDS_CACHE_NODE* pprenode = NULL;
    VDS_CACHE_NODE* pnewnode = NULL;    
    INT32 result;
    INT32 ret = ERR_SUCCESS;
    UINT32 cri_status = 0;
    
    if(FALSE == vds_IsCacheInited())
    {
        vds_CacheTrace(VDS_CACHE_TRACE, "VDS_WriteCacheBlock: vds cache uninit,write to flash.");
        DSM_HstSendEvent(0xB1000000 |(iPartId <<16) | iBlkNr );
        return VDS_WriteBlock( iPartId, iBlkNr, pBuff );
    }    
    
    vds_CacheWaitForSem();  
    pnode = g_cachenode.pnode;
    pprenode = pnode;
    do
    {
        if(NULL == pnode)
        {
            pnewnode = DSM_MAlloc(sizeof(VDS_CACHE_NODE));
            if(NULL == pnewnode)
            {
                // vds_CacheTrace(VDS_CACHE_TRACE, "VDS_WriteCacheBlock: malloc failed. partid = 0x%x,blknr = 0x%x,nodenr = 0x%x.",iPartId,iBlkNr,g_cachenode.blkcnt);
                DSM_HstSendEvent(0xB2000000 |(iPartId <<16) | iBlkNr );
                ret = ERR_VDS_CACHE_VDS_MALLOC_FIALED;
                DSM_ASSERT(0,"VDS_WriteCacheBlock: 1.malloc(0x%x) failed.",(sizeof(VDS_CACHE_NODE))); 
                break;
            }    
            pnewnode->partid = iPartId;
            pnewnode->blknr = iBlkNr;
            pnewnode->status = VDS_CACHE_STATUS_WRITE;
            DSM_MemCpy(pnewnode->pbuff,pBuff,VDS_BLOCK_SIZE);
            pnewnode->next = NULL;   
            
            if(NULL == pprenode)
            {  
               // vds_CacheTrace(VDS_CACHE_TRACE, "VDS_WriteCacheBlock: add newnode(0x%x,0x%x) to header.",iPartId,iBlkNr);
                g_cachenode.pnode = pnewnode;   
                DSM_HstSendEvent(0xB3000000 |(iPartId <<16) | iBlkNr );
            }
            else
            {                
              //  vds_CacheTrace(VDS_CACHE_TRACE, "VDS_WriteCacheBlock: add newnode(0x%x,0x%x) to tail.",iPartId,iBlkNr);
                DSM_HstSendEvent(0xB4000000 |(iPartId<<16) | iBlkNr );
                pprenode->next = pnewnode;
            }   
            g_cachenode.blkcnt ++;
            break;
        }
        else if(pnode->partid == iPartId && pnode->blknr == iBlkNr)
        {        
           pnode->status = VDS_CACHE_STATUS_WRITE;
           DSM_MemCpy(pnode->pbuff,pBuff,VDS_BLOCK_SIZE);            
         //  vds_CacheTrace(VDS_CACHE_TRACE, "VDS_WriteCacheBlock: replace(0x%x,0x%x).",iPartId,iBlkNr);
           DSM_HstSendEvent(0xB5000000 |(iPartId<<16) | iBlkNr);
           break;
        }
        else
        {
            pprenode = pnode;
            pnode = pprenode->next;       
        }        
    }while(1);       

       
    
    if(FALSE == vds_IsFlushAlloc())
    {
        vds_CacheTrace(BASE_VDS_TS_ID, "VDS_FlushCache: vds cache flush unalloc.");
        DSM_HstSendEvent(0xBC0000EF);        
    }  
    else
    {
#ifndef __PRJ_WITH_SPIFLSH__
        cri_status = hal_EnterCriticalSection();
        do
        {               
            if(NULL == g_cachenode.pnode ||  mmi_vdoply_is_in_app() || mmi_audply_is_in_app() ||
                hal_GetVocDma() ||

               // flash_spi_busy() ||

                (!hal_DmaDone()))
            {
                break;
            }               

            if(hal_LpsRemainingFramesReady(VDS_CACHE_FLUSH_REMAIN_FRAMES_NUMBER))
            {                      
                
                result = vds_FlushCacheNode(VDS_CACHE_FLUSH_NODE_NUMBER);                           
                if(result == ERR_VDS_CACHE_NODE_NULL)
                {
                     break;
                }  
            }
            else
            {
                 break;               
            }            
        }while(0);
        
        hal_SysExitCriticalSection(cri_status); 
#endif        
    }
    
    vds_CacheReleaseSem();
    return ret;
}


INT32 VDS_ReadCacheBlock( UINT32 iPartId, UINT32 iBlkNr, UINT8* pBuff )
{
    UINT32 nodenr = 0;
    VDS_CACHE_NODE* pnode = NULL;
    VDS_CACHE_NODE* pprenode = NULL;
    //BOOL bFine = FALSE;
    INT32 ret = ERR_SUCCESS;

    if(FALSE == vds_IsCacheInited())
    {
        ret = VDS_ReadBlock( iPartId, iBlkNr, pBuff );       
        DSM_HstSendEvent(0xC1000000 |(iPartId<<16) | iBlkNr);
        return ret;
    }

    if(g_cachenode.pnode == NULL)
    {           
        ret = VDS_ReadBlock( iPartId, iBlkNr, pBuff );
        return ret;
    }
       
    vds_CacheWaitForSem();
    
    pnode = g_cachenode.pnode;    
    while(pnode != NULL)
    {
        if(pnode->partid == iPartId && pnode->blknr == iBlkNr)
        {
            DSM_MemCpy(pBuff,pnode->pbuff,VDS_BLOCK_SIZE);
            //vds_CacheTrace(VDS_CACHE_TRACE, "VDS_ReadCacheBlock: memcpy(0x%x,0x%x).",iPartId,iBlkNr);
            break;
        }
        pprenode = pnode;
        pnode = pprenode->next;
        nodenr ++;
    }  
    if(NULL == pnode)       
    {
        //vds_CacheTrace(VDS_CACHE_TRACE, "VDS_ReadCacheBlock:can't find node,VDS_ReadBlock(0x%x,0x%x)",iPartId,iBlkNr); 
        ret = VDS_ReadBlock(iPartId,iBlkNr,pBuff);
    }   
    
    vds_CacheReleaseSem();
    return ret;
}


INT32 VDS_RevertCacheBlock( UINT32 iPartId, UINT32 iBlkNr)
{
    //return 0;
    UINT32 nodenr = 0;
    VDS_CACHE_NODE* pnode = NULL;
    VDS_CACHE_NODE* pprenode = NULL;
    
    return ERR_SUCCESS; 

    if(FALSE == vds_IsCacheInited())
    {
        vds_CacheTrace(VDS_CACHE_TRACE, "VDS_RevertCacheBlock: vds cache uninit.");
        return VDS_RevertBlock(iPartId, iBlkNr);
    }

    vds_CacheTrace(VDS_CACHE_TRACE, "VDS_RevertCacheBlock: partid = 0x%x,blknr = 0x%x.",iPartId,iBlkNr);
    
    vds_CacheWaitForSem();
    pnode = g_cachenode.pnode;    
    while(pnode != NULL)
    {
        if(pnode->partid == iPartId && pnode->blknr == iBlkNr)
        {
            pnode->status = VDS_CACHE_STATUS_REVERT;
            DSM_MemSet(pnode->pbuff,0,VDS_BLOCK_SIZE);
            break;
        }
        pprenode = pnode;
        pnode = pprenode->next;
        nodenr ++;
    }
     if(NULL == pnode)
    {
        pnode = COS_MALLOC(sizeof(VDS_CACHE_NODE));
        if(NULL == pnode)
        {
            vds_CacheTrace(VDS_CACHE_TRACE, "VDS_WriteCacheBlock: malloc failed. partid = 0x%x,blknr = 0x%x,nodenr = 0x%x.",iPartId,iBlkNr,nodenr);
            vds_CacheReleaseSem();
            return ERR_VDS_CACHE_VDS_MALLOC_FIALED;
        }    
        pnode->partid = iPartId;
        pnode->blknr = iBlkNr;
        pnode->status = VDS_CACHE_STATUS_REVERT;     
        DSM_MemSet(pnode->pbuff,0,VDS_BLOCK_SIZE);
        pnode->next = NULL;      
        
         if(NULL != pprenode)
        {  
            pprenode->next = pnode;
        }
        if(NULL == g_cachenode.pnode)
        {
            g_cachenode.pnode = pnode;
        }   
        g_cachenode.blkcnt ++;
    }    
    vds_CacheReleaseSem();
  
    return ERR_SUCCESS;
}


INT32 VDS_FlushCacheAll(VOID)
{
    VDS_CACHE_NODE* pnode = NULL;
    VDS_CACHE_NODE* ptmpnode = NULL;
    INT32 result = 0 ;
    UINT32 cri_status = 0;
    UINT32 flush_count = 0;
    UINT32 wait_time = 0;
    INT32 ret = ERR_SUCCESS;
       
    // Uninit.      
    if(FALSE == vds_IsCacheInited() )
    {        
        vds_CacheTrace(BASE_VDS_TS_ID, "VDS_FlushCacheAll: vds cache uninit.");
        DSM_HstSendEvent(0xF100001E);
        return ERR_VDS_CACHE_UNINIT;
    }

    if(g_cachenode.pnode == NULL)
    {
        DSM_HstSendEvent(0xF100003E);
        return ERR_VDS_CACHE_NODE_NULL;
    }
    
    
    hal_FintIrqSetMask(FALSE);
    hal_ComregsClrMask(HAL_COMREGS_XCPU_IRQ_SLOT_0, hal_ComregsGetMask(HAL_COMREGS_XCPU_IRQ_SLOT_0));
    hal_ComregsClrMask(HAL_COMREGS_XCPU_IRQ_SLOT_1, hal_ComregsGetMask(HAL_COMREGS_XCPU_IRQ_SLOT_1));
  //  vds_SetFlushUnAlloc();
    hal_TimWatchDogClose();
    vds_CacheWaitForSem(); 
    cri_status = hal_EnterCriticalSection();    
    wait_time = hal_TimGetUpTime();
    do
    {
          //10 second.
          if((hal_TimGetUpTime() - wait_time) > (20*1000 * (16384/1000)))
          {              
                DSM_HstSendEvent(0xF100004E);
                hal_SysExitCriticalSection(cri_status);                   
                vds_CacheReleaseSem();
                return ERR_VDS_FLUSH_TIMEOUT;
          }
          if(g_cachenode.pnode == NULL)
          {
             
                hal_SysExitCriticalSection(cri_status);               
                vds_CacheReleaseSem();
                DSM_HstSendEvent(0xF100003E);
                return ERR_VDS_CACHE_NODE_NULL;
         }
         if(mmi_vdoply_is_in_app() || 
            mmi_audply_is_in_app() || 
            hal_GetVocDma()             ||
#ifdef __PRJ_WITH_SPIFLSH__           
            flash_spi_busy() || 
#endif        
            (!hal_DmaDone()))
        {
            //sxr_Sleep(20);
            continue;
        }
        else
        {
            break;
        }
    }while(1);
    
    DSM_HstSendEvent(0xF2000000 | g_cachenode.blkcnt);     
    pnode = g_cachenode.pnode;    
    wait_time = hal_TimGetUpTime();
    do
    {
         //20 second.
         if((hal_TimGetUpTime() - wait_time) > (20*1000 * (16384/1000)))
         {
               DSM_HstSendEvent(0xF100005E);
               break;
         }
        if(TRUE == vds_GcIsActive())
        {
            //   DSM_HstSendEvent(0xF20000A0); 
            result = vds_GcExt();
             if(_ERR_VDS_GC_FAILED == result)
            {
               DSM_HstSendEvent(0xF100006E);
               break;
            }
             
            if(_ERR_VDS_GC_FINISH == result)
            {
                vds_GcDeactive();
                DSM_HstSendEvent(0xF20000A1); 
            }                
            continue;
        }
        
        if(pnode != NULL)
        {     
            if(VDS_CACHE_STATUS_WRITE == pnode->status)
            {
                result = VDS_WriteBlock(pnode->partid,pnode->blknr,pnode->pbuff);
                if(result == ERR_SUCCESS)
                {        
                    vds_CacheTrace(BASE_VDS_TS_ID, "VDS_FlushCache: VDS_WriteBlock(0x%x,0x%x) .",pnode->partid,pnode->blknr);
                    DSM_HstSendEvent(0xF3000000 |(pnode->partid << 16) | pnode->blknr );
                    ptmpnode = pnode; 
                    pnode = pnode->next;
                    DSM_Free(ptmpnode);
                    g_cachenode.pnode = pnode;
                    g_cachenode.blkcnt --;
                    flush_count ++;                    
                }
                else
                {
                    if(ERR_VDS_SPACE_FULL == result)
                    {
                        vds_CacheTrace(BASE_VDS_TS_ID, "VDS_FlushCache: writeblock(0x%x,0x%x)  no more free pb.",pnode->partid,pnode->blknr);     
                        DSM_HstSendEvent(0xF30000EF);
                      
                    }
                    else
                    {
                        ret = ERR_VDS_CACHE_WRITE_FAILED;
                        vds_CacheTrace(BASE_VDS_TS_ID, "VDS_FlushCache: writeblock(0x%x,0x%x)  write vds error.result = %d.",pnode->partid,pnode->blknr,result);     
                        DSM_HstSendEvent(0xF30000EE);
                        break;
                    }
                    
                }
            }
            else
            {
                g_cachenode.pnode = g_cachenode.pnode->next; 
                pnode = g_cachenode.pnode; 
                DSM_HstSendEvent(0xF40000EF);
            }
        }
        else
        {
            DSM_HstSendEvent(0xF50000EF);
            break;
        }
           
    }while(pnode != NULL);
    
    hal_SysExitCriticalSection(cri_status);  
    vds_CacheReleaseSem();
    vds_CacheTrace(VDS_CACHE_TRACE, "VDS_FlushCache: flush node count = 0x%x.",flush_count);
    DSM_HstSendEvent(0xF5000000 | flush_count);
   
    return flush_count;
}

//UINT32 g_iFlushTimes = 0;
VOID VDS_FlushCache(VOID)
{     
    UINT32 cri_status = 0;

    cri_status = hal_EnterCriticalSection();
    if(NULL == g_cachenode.pnode || mmi_vdoply_is_in_app() || mmi_audply_is_in_app() || 
        hal_GetVocDma() || 
#ifdef __PRJ_WITH_SPIFLSH__        
        flash_spi_busy() || 
#endif        
        (!hal_DmaDone()))
    {        
        hal_SysExitCriticalSection(cri_status);          
        return;
    }
    
    if(hal_LpsRemainingFramesReady(VDS_CACHE_FLUSH_REMAIN_FRAMES_NUMBER))// && hal_LpsIsResourceInactive())
    {        
        vds_FlushCacheNode(VDS_CACHE_FLUSH_NODE_NUMBER);        
    }

    hal_SysExitCriticalSection(cri_status);     
   
}




VOID vds_CacheSemInit(VOID)
{
   
    g_hVdsCacheSem = sxr_NewSemaphore(1);
    if((HANDLE)NULL == g_hVdsCacheSem)
    {
        D( ( DL_WARNING, "VDS_ModuleSemInit failed."));
    }
}


VOID vds_CacheWaitForSem(VOID)
{    
    
    if((HANDLE)NULL != g_hVdsCacheSem)
    {
        sxr_TakeSemaphore(g_hVdsCacheSem);
    }
    else
    {
        D( ( DL_WARNING, "VDS_ModuleDown failed."));
    }

}


VOID vds_CacheReleaseSem(VOID)
{
    
    if((HANDLE)NULL != g_hVdsCacheSem)
    {
        sxr_ReleaseSemaphore(g_hVdsCacheSem);
    }
    else
    {
        D( ( DL_WARNING, "VDS_ModuleUp failed."));
    }
}

INT32 vds_FlushCacheNode(UINT32 flush_nr)
{
    UINT32 flush_count = 0;
    VDS_CACHE_NODE* pnode = NULL;
    VDS_CACHE_NODE* ptmpnode = NULL;
    INT32 result;
    INT32 ret = ERR_SUCCESS;
    
    
    // Uninit.      
    if(FALSE == vds_IsCacheInited() )
    {
        vds_CacheTrace(BASE_VDS_TS_ID, "VDS_FlushCache: vds cache uninit.");
        DSM_HstSendEvent(0xE10000AE);
        return flush_count;
    }

    // unallowed flush
    if(FALSE == vds_IsFlushAlloc())
    {
        vds_CacheTrace(BASE_VDS_TS_ID, "VDS_FlushCache: vds cache flush unalloc.");
        DSM_HstSendEvent(0xE2000000|g_cachenode.blkcnt);
        return flush_count;
    }  

    if(TRUE == vds_GcIsActive())
    {  
        
        result = vds_GcExt();
        if(_ERR_VDS_GC_FINISH == result)
        {
            DSM_HstSendEvent(0xE3000000|g_cachenode.blkcnt);  
            CSW_TRACE(BASE_DSM_TS_ID, "vds_FlushCacheNode: vds_GcExt() ok.");
            vds_GcDeactive();
        }
        else
        {
           // donoting.
        }        
        return ERR_VDS_CACHE_GCING;
    }
    else
    {                
        pnode = g_cachenode.pnode;
        while(pnode != NULL && flush_count < flush_nr)
        {
            if(VDS_CACHE_STATUS_WRITE == pnode->status)
            {                
                result = VDS_WriteBlock(pnode->partid,pnode->blknr,pnode->pbuff);         
                if(ERR_SUCCESS == result)
                {
                    //vds_CacheTrace(BASE_VDS_TS_ID, "VDS_FlushCache: writeblock(0x%x,0x%x) .",pnode->partid,pnode->blknr);
                    //DSM_HstSendEvent(0xE4000000 | (pnode->partid << 16) |pnode->blknr);
                    ptmpnode = pnode;
                    pnode =   pnode->next;  
                    DSM_Free(ptmpnode);
                    g_cachenode.pnode = pnode; 
                    g_cachenode.blkcnt --;
                    flush_count ++;                      
                } 
                else
                {
                    if(ERR_VDS_SPACE_FULL == result)
                    {
                        //vds_CacheTrace(BASE_VDS_TS_ID, "VDS_FlushCache: writeblock(0x%x,0x%x)  no more free pb,to gc.",pnode->partid,pnode->blknr);     
                        // DSM_HstSendEvent(0xE5000000 | (pnode->partid << 16) |pnode->blknr);
                        ret = ERR_SUCCESS;
                    }
                    else
                    {
                        //vds_CacheTrace(BASE_VDS_TS_ID, "VDS_FlushCache: writeblock(0x%x,0x%x)  write vds error.result = %d.",pnode->partid,pnode->blknr,result);     
                        // DSM_HstSendEvent(0xE6000000 | (pnode->partid << 16) |pnode->blknr);
                        ret = ERR_VDS_CACHE_WRITE_FAILED;
                    }
                    break;
                }
            }            
            else
            {        
                vds_CacheTrace(BASE_VDS_TS_ID, "VDS_FlushCache: free unkown statues block(0x%x,0x%x) .",pnode->partid,pnode->blknr);
                DSM_HstSendEvent(0xE7000000 | (pnode->partid << 16) |pnode->blknr);        
                ptmpnode = pnode;
                pnode =   pnode->next;  
                DSM_Free(ptmpnode);
                g_cachenode.pnode = pnode; 
                g_cachenode.blkcnt --;
                flush_count ++;  
                        
            }
            
        }
        if(NULL == g_cachenode.pnode)
        {
             ret = ERR_VDS_CACHE_NODE_NULL;
             DSM_HstSendEvent(0xE8000000|g_cachenode.blkcnt);
        }
        else
        {
             DSM_HstSendEvent(0xE9000000|g_cachenode.blkcnt);  
        }
        
     } 
     return ret;
}


#endif

