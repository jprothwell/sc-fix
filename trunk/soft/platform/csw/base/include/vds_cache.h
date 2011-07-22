#ifndef _VDS_CACHE_H_
#define _VDS_CACHE_H_

#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)

#if (NUMBER_OF_SIM > 3)

#define VDS_CACHE_FLUSH_NODE_NUMBER                                  2
#define VDS_CACHE_FLUSH_REMAIN_FRAMES_NUMBER               8

#elif (NUMBER_OF_SIM > 2)

#define VDS_CACHE_FLUSH_NODE_NUMBER                                8
#define VDS_CACHE_FLUSH_REMAIN_FRAMES_NUMBER              20

#else

#define VDS_CACHE_FLUSH_NODE_NUMBER                                 20
#define VDS_CACHE_FLUSH_REMAIN_FRAMES_NUMBER               60

#endif


// error code define.
// vds uninit.
#define ERR_VDS_CACHE_VDS_UNINIT                 -1

// malloc failed.
#define ERR_VDS_CACHE_VDS_MALLOC_FIALED          -2

// read failed.
#define ERR_VDS_CACHE_READ_FAILED                -3

// write failed.
#define ERR_VDS_CACHE_WRITE_FAILED               -4

// cache node is null.
#define ERR_VDS_CACHE_NODE_NULL                  -5

// vds cache uninit.
#define ERR_VDS_CACHE_UNINIT                     -6

// unallowed flush
#define ERR_VDS_CACHE_UNALLOWED_FLUSH            -7

//// no more free pb
#define _ERR_VDS_CACHE_NO_MORE_FREE_PB           -8

#define ERR_VDS_CACHE_GCING                                  -9
#define ERR_VDS_FLUSH_TIMEOUT                              -10

INT32 VDS_InitCache(VOID);
INT32 VDS_WriteCacheBlock( UINT32 iPartId, UINT32 iBlkNr, UINT8*pBuff );
INT32 VDS_ReadCacheBlock( UINT32 iPartId, UINT32 iBlkNr, UINT8* pBuff );
INT32 VDS_RevertCacheBlock( UINT32 iPartId, UINT32 iBlkNr);
//INT32 VDS_FlushCacheBlock( UINT32 iPartId);
VOID VDS_FlushCache(VOID);
INT32 VDS_FlushCacheAll(VOID);
VOID VDS_CacheFlushAlloc(VOID);
#else


#endif
#endif

