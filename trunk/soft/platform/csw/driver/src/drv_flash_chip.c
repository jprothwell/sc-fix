#include <cswtype.h>
#include <errorcode.h>
#include <ts.h>
#include <cos.h>
#include "chip_id.h"

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
#include "hal.h"
#else
#include "memd_m.h"
#include "tgt_memd_cfg.h"
#endif
#include <sxr_sbx.h>
#include <sxs_io.h>
#include <drv_flash.h>
#include <dbg.h>
#include "csw_csp.h"
#include "fs.h"
#include "drv_debug.h"

#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER) 
#define  DRV_TakeSemaphore 
#define  DRV_ReleaseSemaphore
#define  DRV_FreeSemaphore 
#define DRV_NewSemaphore 

#else
#define  DRV_TakeSemaphore sxr_TakeSemaphore
#define  DRV_ReleaseSemaphore sxr_ReleaseSemaphore
#define  DRV_FreeSemaphore sxr_FreeSemaphore
#define DRV_NewSemaphore sxr_NewSemaphore
PRIVATE HANDLE g_hSem = 0;
#endif

#define REB_SMS_SECTOR_NUM   4  // 3
#define KBYTES 1024
#define MBYTES 1024 * (KBYTES)

#if (CHIP_ASIC_ID != CHIP_ASIC_ID_JADE)
MEMD_FLASH_LAYOUT_T* g_drvMemdFlashLayout;
#endif



#ifndef SXS_NO_PRINTF
#define DRV_Trace sxs_fprintf
#else
#define DRV_Trace
#endif


// ---------------------------------------------------
// Flash initialize function ,Reserved.
// ---------------------------------------------------
BOOL DRV_FlashInit(void)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
#else
    g_drvMemdFlashLayout = memd_FlashOpen(tgt_GetMemdFlashConfig());
    // FIXME and why not also init the ram here ?
    memd_RamOpen(tgt_GetMemdRamConfig());

#endif
  return TRUE;

}

UINT32 DRV_FlashRegionMap(DRV_FLASH_DEV_INFO* pParam)
{
    UINT32  iBaseAddr = 0;
    UINT32  iTotalSize = 0;
    UINT32 iSectorSize = 0;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
     memd_FlashGetUserFieldInfo(&iBaseAddr,&iTotalSize,&iSectorSize);
#else
    UINT32 iBlockSize = 512;
    memd_FlashGetUserFieldInfo(&iBaseAddr,&iTotalSize,&iSectorSize,&iBlockSize);
#endif    
    pParam->pBaseAddr = (BYTE*)iBaseAddr;
    pParam->total_size = iTotalSize;
    pParam->sector_size = iSectorSize;
   // pParam->sector_size = 0x20000;
  //  pParam->pBaseAddr = (BYTE *)0xE00000; //_user_data_start;    
  //  pParam->total_size = (0x200000 - 0x20000);//_user_data_size;   
  //  gLogicSectorSize = pParam->sector_size;
    return ERR_SUCCESS;

}

UINT32 DRV_EraseAllUserDataSector(VOID)
{
    UINT32 i;
    UINT32 iSectorCount = 0;
    UINT32 iAddrOffset = 0;     
    INT32   iResult = 0;
    UINT32  iBaseAddr = 0;
    UINT32  iTotalSize = 0;
    UINT32 iSectorSize = 0;
    UINT32 iBlockSize = 0;
    UINT32 cri_status = 0;
    
#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
    cri_status = hal_EnterCriticalSection();
#else
    cri_status = 0;
    if (g_hSem == 0)
    {
        return ERR_DRV_INVALID_INITIALIZATION; 
    }
    DRV_TakeSemaphore(g_hSem);
#endif
    // Get user data information in flash.
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
     iBlockSize = iBlockSize;
     memd_FlashGetUserFieldInfo(&iBaseAddr,&iTotalSize,&iSectorSize);
#else
 
    memd_FlashGetUserFieldInfo(&iBaseAddr,&iTotalSize,&iSectorSize,&iBlockSize);
#endif 
    
    iSectorCount = iTotalSize/iSectorSize;
    // Erase all sector of user region.    
    for(i = 0; i < iSectorCount ; i++)
    {
         iAddrOffset = i*iSectorSize + (UINT32)iBaseAddr;       

	  // We erase flash chip using logic sector unit  
	  iResult = hal_flash_Erase((UINT8*)iAddrOffset, (UINT8*)(iAddrOffset + iSectorSize));
	  if (iResult)
	  {
	      break;
	  }
    }
    
#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
    if(cri_status)
    {
        hal_SysExitCriticalSection(cri_status); 
    }
#else
    DRV_ReleaseSemaphore(g_hSem);
#endif
    return ERR_SUCCESS;
}

UINT32 DRV_EraseFlashSector(UINT32 ulAddrOffset)
{
    UINT32  iBaseAddr = 0;
    UINT32  iTotalSize = 0;
    UINT32 iSectorSize = 0;
    UINT32 iBlockSize = 512;
    UINT32 ret = ERR_SUCCESS;
    UINT32 cri_status = 0;
    
#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
    
#else
    cri_status = 0;
    if (g_hSem == 0)
    {
        return ERR_DRV_INVALID_INITIALIZATION;
    }
    DRV_TakeSemaphore(g_hSem);
#endif

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
    iBlockSize  = iBlockSize;
    memd_FlashGetUserFieldInfo(&iBaseAddr,&iTotalSize,&iSectorSize);
#else

    memd_FlashGetUserFieldInfo(&iBaseAddr,&iTotalSize,&iSectorSize,&iBlockSize);
#endif 
#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
    cri_status = hal_EnterCriticalSection();
#endif
    // We erase flash chip using logic sector unit  
    ret = hal_flash_Erase((UINT8*)ulAddrOffset, (UINT8*)(ulAddrOffset + iSectorSize));
#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
    if(cri_status)
    {
        hal_SysExitCriticalSection(cri_status); 
    }
#else
    DRV_ReleaseSemaphore(g_hSem);
#endif
    if(ret != 0)
    {
        hal_HstSendEvent(0x91000003);
        hal_HstSendEvent(ulAddrOffset);
        hal_HstSendEvent(0-ret);
        DBG_ASSERT(ret != 0, "memd_FlashErase failed. addr = 0x%x,err code = %d.",ulAddrOffset,ret);
    }
    return ret;
}

#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
UINT32 DRV_EraseFlashSectorFirst(UINT32 ulAddrOffset,UINT32 uTime)
{
    UINT32  iBaseAddr = 0;
    UINT32  iTotalSize = 0;
    UINT32 iSectorSize = 0;
    UINT32 iBlockSize = 512;
    UINT32 ret = ERR_SUCCESS;
    UINT32 cri_status = 0;  
    DRV_PROFILE_FUNCTION_ENTER(DRV_EraseFlashSectorFirst);
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
    iBlockSize  = iBlockSize;
    memd_FlashGetUserFieldInfo(&iBaseAddr,&iTotalSize,&iSectorSize);
#else

    memd_FlashGetUserFieldInfo(&iBaseAddr,&iTotalSize,&iSectorSize,&iBlockSize);
#endif 
    //for(int daley = 0; daley < 100000;daley++) daley = daley;
    cri_status = hal_EnterCriticalSection();

    // We erase flash chip using logic sector unit  
    ret = memd_FlashErase_First((UINT8*)ulAddrOffset, (UINT8*)(ulAddrOffset + iSectorSize),uTime);

    if(cri_status)
    {
        hal_SysExitCriticalSection(cri_status); 
    }
    DRV_PROFILE_FUNCTION_EXIT(DRV_EraseFlashSectorFirst);
    if(MEMD_ERR_SUSPEND == ret)
    {
       ret = ERR_FLASH_ERASE_SUSPEND;
    }
   
    return ret;
}


UINT32 DRV_EraseFlashSectorResume(UINT32 ulAddrOffset,UINT32 uTime)
{
    
    UINT32 ret = ERR_SUCCESS;
    UINT32 cri_status = 0;  
    DRV_PROFILE_FUNCTION_ENTER(DRV_EraseFlashSectorResume);
    //for(int daley = 0; daley < 100000;daley++) daley = daley;
    cri_status = hal_EnterCriticalSection();
    // We erase flash chip using logic sector unit  
    ret = memd_FlashErase_Continue((UINT8*)ulAddrOffset, NULL,uTime);

    if(cri_status)
    {
        hal_SysExitCriticalSection(cri_status); 
    }
    DRV_PROFILE_FUNCTION_EXIT(DRV_EraseFlashSectorResume);
    if(MEMD_ERR_SUSPEND == ret)
    {
       ret = ERR_FLASH_ERASE_SUSPEND;
    }
    return ret;
}

#endif


UINT32 DRV_ReadFlash(UINT32 ulAddrOffset, BYTE* pBuffer, UINT32 nNumberOfBytesToRead, INT32* pNumberOfBytesRead)
{
    UINT32 n  = 0;
    INT32 ret = 0; 
    UINT32 cri_status = 0;

#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
     cri_status = hal_EnterCriticalSection();
#else
    cri_status = 0;
    if (g_hSem == 0)
        return ERR_DRV_INVALID_INITIALIZATION;
    DRV_TakeSemaphore(g_hSem);
#endif
    ret = hal_flash_Read((UINT8*)ulAddrOffset, nNumberOfBytesToRead, &n, pBuffer);
#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
     if(cri_status)
    {
        hal_SysExitCriticalSection(cri_status); 
    }
#else
    DRV_ReleaseSemaphore(g_hSem);
#endif

    
    if (ret != 0)
    {       
        hal_HstSendEvent(0x91000001);
        hal_HstSendEvent(ulAddrOffset);
        hal_HstSendEvent(0-ret);
        DBG_ASSERT(ret != 0, "memd_FlashRead failed. addr = 0x%x,err code = %d.",ulAddrOffset,ret);
        return ret;
    }
    if (pNumberOfBytesRead != NULL)
    {
        *pNumberOfBytesRead = n;
        if (nNumberOfBytesToRead != n)
            return ret;
    }
    return ERR_SUCCESS;
    
}

UINT32 DRV_WriteFlash(UINT32 ulAddrOffset,
                      CONST BYTE* pBuffer, UINT32 nNumberOfBytesToWrite, UINT32* pNumberOfBytesWritten)
{
    UINT32 n  = 0;
    INT32 ret = 0;
    BOOL  lock_status;
    INT32 retVal;
    UINT32 cri_status = 0;

#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)

    cri_status = hal_EnterCriticalSection();

#else
    cri_status = 0;
    if (g_hSem == 0)
            return ERR_DRV_INVALID_INITIALIZATION;    
    DRV_TakeSemaphore(g_hSem);
#endif

    ret = hal_flash_Write((UINT8*)ulAddrOffset, nNumberOfBytesToWrite, &n, pBuffer);

#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
    if(cri_status)
    {
        hal_SysExitCriticalSection(cri_status); 
    }
#else
    DRV_ReleaseSemaphore(g_hSem);
#endif   
    
    if (ret != 0)
    {
        hal_HstSendEvent(0x91000002);
        hal_HstSendEvent(ulAddrOffset);
        hal_HstSendEvent(0-ret);
        DBG_ASSERT(ret != 0, "memd_FlashWrite failed. addr = 0x%x,err code = %d.",ulAddrOffset,ret);
        DRV_Trace(BASE_FFS_TS_ID, "DRV_WriteFlash:hal_flash_Write failed.ret is %d.\n",ret);
        retVal = memd_FlashGetLockStatus((UINT8 *) ulAddrOffset, &lock_status);
        DRV_Trace(BASE_FFS_TS_ID, "retVal is %d,addr = 0x%x,lock_status is %d.\n",retVal,ulAddrOffset,lock_status);
        
        DBG_ASSERT(ret != 0, "memd_FlashRead failed. addr = 0x%x,err code = %d.",ulAddrOffset,ret);
        return ret;
    }

    if (pNumberOfBytesWritten != NULL)
    {
        *pNumberOfBytesWritten = n;
        if (nNumberOfBytesToWrite != n)
        return ret;
    }

    return ERR_SUCCESS;
}

UINT32 DRV_FlashLock(UINT32 ulStartAddr, UINT32 ulEndAddr)
{
  INT32 ret = 0;
 
#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)

#else
    if (g_hSem == 0)
            return ERR_DRV_INVALID_INITIALIZATION;    
    DRV_TakeSemaphore(g_hSem);
#endif

  ret = hal_flash_Lock((UINT8*)ulStartAddr, (UINT8*)ulEndAddr);
#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)

#else
   DRV_ReleaseSemaphore(g_hSem);
#endif

  

  return ret;
}

UINT32 DRV_FlashUnLock(UINT32 ulStartAddr, UINT32 ulEndAddr)
{
  INT32 ret = 0;

#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)

#else
    if (g_hSem == 0)
            return ERR_DRV_INVALID_INITIALIZATION;    
    DRV_TakeSemaphore(g_hSem);
#endif

  ret = hal_flash_Unlock((UINT8*)ulStartAddr, (UINT8*)ulEndAddr);
#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)

#else
   DRV_ReleaseSemaphore(g_hSem);
#endif
return ret;
}

UINT8* DRV_FlashGetAddress(UINT32 ulAddrOffset)
{

  return (UINT8*)hal_flash_GetGlobalAddress((UINT8*)ulAddrOffset);
}

#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)

UINT32 DRV_FlashPowerUp(VOID)
{
  return ERR_SUCCESS;
}


UINT32 DRV_FlashPowerDown(VOID)
{  
  return ERR_SUCCESS;
}

#else

UINT32 DRV_FlashPowerUp(  
                         VOID
  )
{
  // TODO... 
  if (g_hSem)
    DRV_FreeSemaphore(g_hSem);

  g_hSem = DRV_NewSemaphore(1);  // for 1 user.

  return ERR_SUCCESS;
}


UINT32 DRV_FlashPowerDown(  
                           VOID
  )
{

  if(g_hSem)
  {
     DRV_FreeSemaphore(g_hSem);
     g_hSem = 0;
  }
  return ERR_SUCCESS;
}

#endif
