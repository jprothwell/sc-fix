#include "dsm_cf.h"
#include "dsm_dbg.h"
#include "reg.h"
#include "fs.h"
#include "sms_dm.h"

#ifdef _MS_VC_VER_
INT32 g_dsm_errno;
BOOL  wfcheat = FALSE;
#endif

UINT32 g_dsm_DbgSwtich[ NR_SUBMODULE ] = {1, 1, 1,1,1};
UINT32 g_dsm_DbgLevel[ NR_SUBMODULE ] = {DL_ERROR, DL_ERROR, DL_ERROR,DL_ERROR,DL_ERROR};

#define DSM_MOD_VDS           0
#define DSM_MOD_FAT           1
#define DSM_MOD_VFS           2
#define DSM_MOD_REG          3
#define DSM_MOD_SMS          4



char *g_dsm_ModName[ NR_SUBMODULE ] =
{
    "VDS", 
    "FAT", 
    "VFS",
    "REG",
    "SMS"
};

#ifdef _MS_VC_VER_
int dsm_Printf( int index, const char *format, ... )
{
    /*
    这里假设展开后的字符串的长度不超过1024.
    例如: dsm_Printf( index, "%s %x", "gaokejun", 0xDEADBEEF );
    则展开后的字符串是"gaokejun DEADBEEF".
    */
    UINT8 cTemp[ 1024 + LEN_FOR_NULL_CHAR ];
    va_list	ap;

    index = 0;
    va_start( ap, format );
    ( void ) vsprintf( cTemp, format , ap );
    va_end( ap );

    printf( "%s", cTemp );
    return 0;
}
#endif


UINT8 g_dsm_DbgBuf[ VDS_DEBUG_BUF_SIZE ];

VOID dsm_Debugout( UINT32 ulLevel, CONST UINT8* szFormat, ... )
{
    va_list va_format;
    UINT32 lLen = 0;
    UINT32 iSubModuleIndex = 0;

    iSubModuleIndex = ulLevel / DL_NUM;
    if(NR_SUBMODULE <= iSubModuleIndex)
    {
         iSubModuleIndex = 0;
    }
    
    if ( !g_dsm_DbgSwtich[ iSubModuleIndex ] || g_dsm_DbgLevel[ iSubModuleIndex ] < ( UINT32 ) ulLevel % DL_NUM )
    {
        return ;
    }

    DSM_MemSet( g_dsm_DbgBuf, 0, VDS_DEBUG_BUF_SIZE );

    va_start( va_format, szFormat );
#ifdef _MS_VC_VER_

    lLen = ( long ) _vsnprintf( g_dsm_DbgBuf, ( VDS_DEBUG_BUF_SIZE - 1 ), szFormat, va_format );
#else

    lLen = ( long ) DSM_StrVPrint( g_dsm_DbgBuf, szFormat, va_format );
#endif

    if ( lLen < 0 )
    {
        va_end( va_format );
        return ;
    }
    va_end( va_format );


    switch ( ulLevel % DL_NUM )
    {
        case DL_ERROR:
            dsm_Printf( BASE_DSM_TS_ID, "%s: [ERROR] %s\r\n", g_dsm_ModName[ iSubModuleIndex ], g_dsm_DbgBuf );
            break;
        case DL_WARNING:
            dsm_Printf( BASE_DSM_TS_ID, "%s: [WARNING] %s\r\n", g_dsm_ModName[ iSubModuleIndex ], g_dsm_DbgBuf );
            break;
        case DL_BRIEF:
            dsm_Printf( BASE_DSM_TS_ID, "%s: [brief] %s\r\n", g_dsm_ModName[ iSubModuleIndex ], g_dsm_DbgBuf );
            break;
        case DL_DETAIL:
            dsm_Printf( BASE_DSM_TS_ID, "%s: [detail] %s\r\n", g_dsm_ModName[ iSubModuleIndex ], g_dsm_DbgBuf );
            break;
        default:
            break;
    }
    return ;
}
#ifdef _T_UPGRADE_PROGRAMMER
VOID DSM_GetVersion( INT32 iModId, CHAR* pVersion )
{
        DSM_StrCpy(pVersion,"0x0");
}

#else


/* It is the caller's reponsibility to allocate memory for pVersion */
VOID DSM_GetVersion( INT32 iModId, CHAR* pVersion )
{ 
    
    switch ( iModId )
    {
    case DSM_MOD_VDS:
        DSM_StrPrint(pVersion,"0x%x", VDS_GetVersion());
        break;
    case DSM_MOD_REG:
       DSM_StrPrint(pVersion,"0x%x",REG_GetVersion());
        break;
    case DSM_MOD_SMS:
        DSM_StrPrint(pVersion,"0x%x", SMS_DM_GetVersion());
        break;
    case DSM_MOD_FAT:
        DSM_StrPrint(pVersion,"0x%x", FS_GetVersion() & 0xffff0000);
        break;
    case DSM_MOD_VFS:
        DSM_StrPrint(pVersion,"0x%x", FS_GetVersion() & 0xffff);
    default:
        break;
    }

    return;
}

#endif
