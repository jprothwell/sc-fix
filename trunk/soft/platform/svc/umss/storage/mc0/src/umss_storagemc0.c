////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2008, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//
//  $HeadURL$
//  $Author$
//  $Date$
//  $Revision$
//
////////////////////////////////////////////////////////////////////////////////
//
/// @file umss_storagemc.c
///
/// Storage memory card function
//
////////////////////////////////////////////////////////////////////////////////


// =============================================================================
// HEADERS                                                                      
// =============================================================================

#include "umss_config.h"
#include "umssp_debug.h"
#include "sxr_mem.h"
#include "sxs_io.h"

#include "mcd_m.h"

#include "hal_host.h"

#include <string.h>

// =============================================================================
// MACROS                                                                       
// =============================================================================


// =============================================================================
// TYPES                                                                        
// =============================================================================

// =============================================================================
// UMSS_STORAGE_MC_HANDLER_T
// -----------------------------------------------------------------------------
/// Mc handler
// =============================================================================
typedef struct
{
    UMSS_STORAGE_SIZE_T  size;
} UMSS_STORAGE_MC_HANDLER_T;

// =============================================================================
// PROTOTYPE FUNCTIONS                                                          
// =============================================================================

PRIVATE VOID*
umss_StorageMcOpen   (CONST UMSS_STORAGE_PARAMETERS_T* cfg);

PRIVATE VOID
umss_StorageMcClose  (VOID* handler);

PRIVATE INT32
umss_StorageMcRead   (VOID* handler, VOID* data, UINT32 lba, UINT32 size);

PRIVATE INT32
umss_StorageMcWrite  (VOID* handler, VOID* data, UINT32 lba, UINT32 size);

PRIVATE UMSS_STORAGE_SIZE_T*
umss_StorageMcGetsize(VOID* handler);

PRIVATE BOOL
umss_StorageMcPresent(VOID* handler);
#ifdef DUAL_TFLASH_SUPPORT
extern PUBLIC MCD_ERR_T mcd_Open(MCD_CARD_ID mcdId,MCD_CSD_T* mcdCsd, MCD_CARD_VER mcdVer);
extern PUBLIC MCD_ERR_T mcd_Close(MCD_CARD_ID mcdId);
#else
extern PUBLIC MCD_ERR_T mcd_Open(MCD_CSD_T* mcdCsd, MCD_CARD_VER mcdVer);
extern PUBLIC MCD_ERR_T mcd_Close(VOID);
#endif
// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================

PUBLIC CONST UMSS_STORAGE_CALLBACK_T g_umssStorageMc0Callback = {
    .open    = umss_StorageMcOpen,
    .close   = umss_StorageMcClose,
    .read    = umss_StorageMcRead,
    .write   = umss_StorageMcWrite,
    .getsize = umss_StorageMcGetsize,
    .present = umss_StorageMcPresent
};

// for umss_StorageMcGetsize return, the function return a point of structure,we have to define a global variable to return.
PRIVATE UMSS_STORAGE_MC_HANDLER_T g_handlerMc;

// =============================================================================
// PRIVATE FUNCTIONS                                                            
// =============================================================================
PRIVATE BOOL umss_StorageMcPresent(VOID* handler)
{
    UMSS_STORAGE_MC_HANDLER_T* handlerMc;
    MCD_CSD_T                  csd;
    MCD_CARD_SIZE_T            cardSize;
    MCD_CARD_VER mcdVer = MCD_CARD_V2;
    MCD_ERR_T mcdErr = MCD_ERR_NO;

    
    UMSS_TRACE(UMSS_INFO_TRC, 0, "umss_StorageMcPresent() 0,begin.");

    if(NULL == handler)
    {
        return FALSE;
    }
    
    handlerMc = (UMSS_STORAGE_MC_HANDLER_T*) handler;
    
    switch(mcd_CardStatus(MCD_CARD_ID_0))
    {
    case MCD_STATUS_NOTOPEN_PRESENT:
#ifdef DUAL_TFLASH_SUPPORT        
        mcdErr = mcd_Open(MCD_CARD_ID_0,&csd,mcdVer);
        if(mcdErr != MCD_ERR_NO)
        {           
            mcd_Close(MCD_CARD_ID_0);
            mcdVer = MCD_CARD_V1;
            mcdErr = mcd_Open(MCD_CARD_ID_0,&csd,mcdVer);
            if(mcdErr != MCD_ERR_NO)
            {
                mcd_Close(MCD_CARD_ID_0);                
                mcdErr = mcd_Open(MCD_CARD_ID_0,&csd,mcdVer);
                if(mcdErr != MCD_ERR_NO)
                {
                    UMSS_TRACE(UMSS_INFO_TRC, 0,"umss_StorageMcPresent() 0,mcd_Open failed,mcdErr = %d.",mcdErr);                    
                }                
            } 

        }
        if(MCD_ERR_NO == mcdErr)
        {
            mcdErr = mcd_SwitchOperationTo(MCD_CARD_ID_0);
            if(MCD_ERR_NO == mcdErr)
            {
                mcd_GetCardSize(&cardSize);
                handlerMc->size.sizeBlock = cardSize.blockLen;
                handlerMc->size.nbBlock   = cardSize.nbBlock;
                UMSS_TRACE(UMSS_INFO_TRC, 0,"umss_StorageMcPresent() 0,mcd_Open OK.");
                return TRUE;
            }
            else
            {
                handlerMc->size.sizeBlock = 0;
                handlerMc->size.nbBlock   = 0;
                return FALSE;
            }
            
        }
        else
        {
            handlerMc->size.sizeBlock = 0;
            handlerMc->size.nbBlock   = 0;
            return FALSE;
        }
#else
        mcdErr =(mcd_Open(&csd,mcdVer);
        if(mcdErr != MCD_ERR_NO)
        {
            mcd_Close();
            mcdErr = mcd_Open(&csd,mcdVer);
            if(mcdErr != MCD_ERR_NO)
            {
                mcd_Close();
                mcdVer = MCD_CARD_V1;
                mcdErr = mcd_Open(&csd,mcdVer);
                if(mcdErr != MCD_ERR_NO)
                {
                    return FALSE;
                }               
            }
        }
        if(MCD_ERR_NO == mcdErr)
        {
            mcd_GetCardSize(&cardSize);
            handlerMc->size.sizeBlock = cardSize.blockLen;
            handlerMc->size.nbBlock   = cardSize.nbBlock;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
#endif
        // Caution: There is no break here!
        // The following statement is executed as well.
    case MCD_STATUS_OPEN:
#ifdef DUAL_TFLASH_SUPPORT          
        mcdErr = mcd_SwitchOperationTo(MCD_CARD_ID_0);
#endif
         if(MCD_ERR_NO == mcdErr)
         {
            mcd_GetCardSize(&cardSize);
            handlerMc->size.sizeBlock = cardSize.blockLen;
            handlerMc->size.nbBlock   = cardSize.nbBlock;
            return TRUE;
         }
         else
         {
            handlerMc->size.sizeBlock =  handlerMc->size.nbBlock   = 0; 
            return FALSE;
         }
        
    case MCD_STATUS_NOTPRESENT:
        handlerMc->size.sizeBlock =  handlerMc->size.nbBlock   = 0;
        return FALSE;
    case MCD_STATUS_OPEN_NOTPRESENT:
        // ensure the mcd is closed
#ifdef DUAL_TFLASH_SUPPORT        
        mcd_Close(MCD_CARD_ID_0);
#else
        mcd_Close();
#endif
        handlerMc->size.sizeBlock =  handlerMc->size.nbBlock   = 0;
        return FALSE;
    }

    return(FALSE);
}


PRIVATE VOID*  umss_StorageMcOpen(CONST UMSS_STORAGE_PARAMETERS_T* cfg)
{
    UMSS_STORAGE_MC_HANDLER_T* handlerMc = NULL;
    MCD_ERR_T mcdErr;
    MCD_CARD_VER mcdVer = MCD_CARD_V2;
    MCD_CSD_T                  csd;
    
    cfg = cfg;
#ifdef DUAL_TFLASH_SUPPORT
    mcdErr = mcd_Open(MCD_CARD_ID_0,&csd,mcdVer);
    if(mcdErr != MCD_ERR_NO)
    {           
        mcd_Close(MCD_CARD_ID_0);
        mcdVer = MCD_CARD_V1;
        mcdErr = mcd_Open(MCD_CARD_ID_0,&csd,mcdVer);
        if(mcdErr != MCD_ERR_NO)
        {
            mcd_Close(MCD_CARD_ID_0);            
            mcdErr = mcd_Open(MCD_CARD_ID_0,&csd,mcdVer);
            if(mcdErr != MCD_ERR_NO)
            {
                UMSS_TRACE(UMSS_INFO_TRC, 0,"umss_StorageMcOpen() 0,mcd_Open failed.");                    
            }                
        } 

    }
    if(MCD_ERR_NO == mcdErr)
    {
        // mcd_Close(MCD_CARD_ID_0);
        handlerMc = (UMSS_STORAGE_MC_HANDLER_T*)sxr_Malloc(sizeof(UMSS_STORAGE_MC_HANDLER_T));                
        UMSS_TRACE(UMSS_INFO_TRC, 0,"umss_StorageMcOpen() 0,mcd_Open ok.");    
    }  
    else
    {
        handlerMc = NULL;
        UMSS_TRACE(UMSS_INFO_TRC, 0,"umss_StorageMcOpen() 0,mcd_Open failed.mcdErr = %d.",mcdErr);    
    }
#else
    mcdErr = mcd_Open(&csd,mcdVer);
    if(mcdErr != MCD_ERR_NO)
    {           
        mcd_Close();
        mcdErr = mcd_Open(&csd,mcdVer);
        if(mcdErr != MCD_ERR_NO)
        {
            mcd_Close();
            mcdVer = MCD_CARD_V1;
            mcdErr = mcd_Open(&csd,mcdVer);
            if(mcdErr != MCD_ERR_NO)
            {
                UMSS_TRACE(UMSS_INFO_TRC, 0,"umss_StorageMcOpen() 0,mcd_Open failed.");                    
            }                
        } 

    }
    if(MCD_ERR_NO == mcdErr)
    {
        // mcd_Close();
        handlerMc = (UMSS_STORAGE_MC_HANDLER_T*)sxr_Malloc(sizeof(UMSS_STORAGE_MC_HANDLER_T));                
        UMSS_TRACE(UMSS_INFO_TRC, 0,"umss_StorageMcOpen() 0,mcd_Open ok.");    
    }  
    else
    {
        handlerMc = NULL;
        UMSS_TRACE(UMSS_INFO_TRC, 0,"umss_StorageMcOpen() 0,mcd_Open failed.mcdErr = %d.",mcdErr);    
    }
#endif    
    return((VOID*)(handlerMc));
}

PRIVATE VOID   umss_StorageMcClose(VOID* handler)
{
    if(NULL == handler)
    {
        return;
    }
#ifdef DUAL_TFLASH_SUPPORT        
        mcd_Close(MCD_CARD_ID_0);
#else
        mcd_Close();
#endif

    if(handler)
    {
        sxr_Free(handler);
    }
}

PRIVATE INT32 umss_StorageMcRead(VOID* handler, VOID* data,
                                 UINT32 lba, UINT32 nbBlock)
{
    UMSS_STORAGE_MC_HANDLER_T* handlerMc;
    UINT32                     size;
    MCD_ERR_T                  ret;
    
    if(NULL == handler)
    {
        UMSS_TRACE(UMSS_INFO_TRC, 0,"umss_StorageMcRead() 0,handle = NULL.");    
        return -1;
    }
    
    handlerMc = (UMSS_STORAGE_MC_HANDLER_T*) handler;
    size             = handlerMc->size.sizeBlock*nbBlock;
#ifdef DUAL_TFLASH_SUPPORT
    ret = mcd_SwitchOperationTo(MCD_CARD_ID_0);
    if(MCD_ERR_NO != ret)
    {
        return (-2);
    }
#endif
    ret = mcd_Read(lba*handlerMc->size.sizeBlock, data, size);
    if(ret != MCD_ERR_NO)
    {
        switch(ret)
        {
        case MCD_ERR_DMA_BUSY:
        case MCD_ERR_SPI_BUSY:
            return(-2);
        case MCD_ERR_CARD_TIMEOUT:
        case MCD_ERR_BLOCK_LEN:
        case MCD_ERR_CARD_NO_RESPONSE:
        case MCD_ERR_CARD_RESPONSE_BAD_CRC:
        case MCD_ERR_CMD:
        case MCD_ERR_UNUSABLE_CARD:
        case MCD_ERR_NO_CARD:
        default:
#ifdef DUAL_TFLASH_SUPPORT        
        mcd_Close(MCD_CARD_ID_0);
#else
        mcd_Close();
#endif
            return(-3);
        }
    }

    return(nbBlock);
}

PRIVATE INT32 umss_StorageMcWrite(VOID* handler, VOID* data,
                                  UINT32 lba, UINT32 nbBlock)
{
    UMSS_STORAGE_MC_HANDLER_T*  handlerMc;
    UINT32                      size;
    MCD_ERR_T                   ret;
    
    if(NULL == handler)
    {
        UMSS_TRACE(UMSS_INFO_TRC, 0,"umss_StorageMcWrite() 0,handle = NULL.");    
        return -1;
    }
   
    handlerMc = (UMSS_STORAGE_MC_HANDLER_T*) handler;
    size             = handlerMc->size.sizeBlock*nbBlock;
#ifdef DUAL_TFLASH_SUPPORT     
    ret = mcd_SwitchOperationTo(MCD_CARD_ID_0);
    if(MCD_ERR_NO != ret)
    {
        return (-1);
    }
#endif
    
    ret = mcd_Write(lba*handlerMc->size.sizeBlock, data, size);
    if(ret != MCD_ERR_NO)
    {
        switch(ret)
        {
        case MCD_ERR_DMA_BUSY:
        case MCD_ERR_SPI_BUSY:
            return(-2);
        case MCD_ERR_CARD_TIMEOUT:
        case MCD_ERR_BLOCK_LEN:
        case MCD_ERR_CARD_NO_RESPONSE:
        case MCD_ERR_CARD_RESPONSE_BAD_CRC:
        case MCD_ERR_CMD:
        case MCD_ERR_UNUSABLE_CARD:
        default:
#ifdef DUAL_TFLASH_SUPPORT        
           mcd_Close(MCD_CARD_ID_0);
#else
           mcd_Close();
#endif
            return(-1);
        }
    }

    return(nbBlock);
}


PRIVATE UMSS_STORAGE_SIZE_T* umss_StorageMcGetsize(VOID* handler)
{
    UMSS_STORAGE_MC_HANDLER_T* phandlerMc;
    
    if(NULL == handler)
    {
       phandlerMc = &g_handlerMc;
       
       phandlerMc->size.sizeBlock = 0;
       phandlerMc->size.nbBlock = 0;
       UMSS_TRACE(UMSS_INFO_TRC, 0,"umss_StorageMcGetsize() 0 error. handle = NULL,size = 0.");           
    }
    else
    {         
         phandlerMc = (UMSS_STORAGE_MC_HANDLER_T*) handler;
         UMSS_TRACE(UMSS_INFO_TRC, 0,"umss_StorageMcGetsize() 0 ok.handle = 0x%x.");             
    }
    
    return(&phandlerMc->size);
}

// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================
