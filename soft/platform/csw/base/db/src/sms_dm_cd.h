#ifndef _SMS_DM_COMMON_DEFINE_
#define _SMS_DM_COMMON_DEFINE_

#ifdef _MS_VC_VER_
#define SMS_DM_MAX_REC_COUNT     g_CSW_Dm_MaxRecCount   // The max record count.
#define SMS_DM_SZ_RECORD_DATA    g_CSW_Dm_RecDataSize     // The data size
#else
//#include "csw_ver.h"
#include "cfw_config_prv.h"
#define SMS_DM_MAX_REC_COUNT  CFW_SMS_MAX_REC_COUNT   // The max record count.
#define SMS_DM_SZ_RECORD_DATA CFW_SMS_REC_DATA_SIZE     // The data size
#endif

#ifdef CFW_MULTI_SIM
#include "cfw_multi_sim.h"
#endif
#define SMS_INITIALIZED                     0xFFFF
#define SMS_UNINIT                             0x0000

#define SMS_DM_BASE_ADDR               0x0
#define SMS_DM_RST_ADDR          (SMS_DM_BASE_ADDR + SIZEOF(SMS_DM_H_INFO))
#define SMS_DM_RECORD_DATA_START_ADDR (SMS_DM_RST_ADDR + SMS_DM_MAX_REC_COUNT * SMS_DM_SZ_RECORD_STATUS)

#define SMS_DM_STATUS_REC_FREE          0x0000      // Record block is free 
#define SMS_DM_STATUS_REC_VALID        0xFFFF      // Record block is valid
#define SMS_DM_SZ_RECORD_STATUS       2


#define SMS_DM_MAGIC                        0x20070125      // magic number 
#define SMS_DM_SZ_MAGIC                   4                    // the magic size

#define SMS_DM_DATA_STATUS_POS       0                // the status flag position
#define SMS_DM_SZ_DATA_STATUS         1                // the status flag position
#define SMS_REC_IVALID_STATUS           0x00            // Invalid status of record

#define SMS_DM_NR_RST_GROUP            50

#define SMS_DM_LOG_STRAT_ADDR (SMS_DM_RECORD_DATA_START_ADDR\
                           + SMS_DM_MAX_REC_COUNT * SMS_DM_SZ_RECORD_DATA)

#define SMS_DM_FG_LOG_ACTIVA         0xFFFF
#define SMS_DM_FG_LOG_STILL            0x0000
#define SMS_DM_SZ_LOG_FLAG             2
#define SMS_DM_LOG_FLAG_POS           0
#define SMS_DM_LOG_DATA_POS          (SIZEOF(SMS_DM_LOG))
#define SMS_DM_SZ_LOG                      (SIZEOF(SMS_DM_LOG) + SMS_DM_SZ_RECORD_DATA)


// sms data manage file header information
typedef struct _sms_dm_h_info
{
  UINT32  iHSize;                                      // header size.
  UINT32  iVer;	                                    // version number. 
  UINT32  iMagic ;                                    // Magic number.
  UINT8   szServed[4];                             // served field. 
}SMS_DM_H_INFO;


// sms dm log structural
typedef struct _sms_dm_log
{
  UINT16  iLogFlag;	
  UINT16  iRecordIndex;                                    
  UINT16  iRecordNewStatus;         
  UINT8   szServed[2];                             // served field. 
}SMS_DM_LOG;


#define SMS_DM_SZ_H_INFO          SIZEOF(SMS_DM_H_INFO)
#define SMS_DM_SZ_RST                (SMS_DM_SZ_RECORD_STATUS * SMS_DM_MAX_REC_COUNT)
#define SMS_DM_SZ_MEDIA_USE \
            (SMS_DM_SZ_H_INFO + SMS_DM_SZ_RST \
             + SMS_DM_MAX_REC_COUNT * SMS_DM_SZ_RECORD_DATA + SMS_DM_SZ_LOG)


#define ERR_RETURN( expr, rv, err_info )  \
{\
    if( ( expr) )\
    {\
        dsm_Printf( BASE_DSM_TS_ID, "[ERROR]: %s(%d) :%s\n", __FILE__, __LINE__, err_info ? err_info : "NULL" );\
        return (rv);\
    }\
}

/*EH:Error Handle*/
#define ERR_GOTO( expr, value, label, err_info )     \
{\
    if( (expr) )\
    {\
        iRet = value;\
        dsm_Printf( BASE_DSM_TS_ID, "[ERROR]: %s(%d): %s\n", __FILE__, __LINE__, err_info ? err_info : "NULL" );\
        goto label;\
    }\
}




#endif //_SMS_DM_COMMON_DEFINE_


