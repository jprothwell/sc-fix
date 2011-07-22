#ifndef _DSM_DBG_H_
#define _DSM_DBG_H_


#define _DSM_COPYRIGHT_INFO_ "Copyright(c) 2003-2005 by Coolsand Technologies, Inc"


#define DL_NUM                 5
#define NR_SUBMODULE           5

// DSM Module id.
#define DSM_MOD_VDS           0
#define DSM_MOD_FAT           1
#define DSM_MOD_VFS           2
#define DSM_MOD_REG          3
#define DSM_MOD_SMS          4


#define DL_ERROR     			(0x0)                                     /*MUST be set 0*/
#define DL_WARNING   	        (DL_ERROR + 1 )
#define DL_BRIEF     			(DL_WARNING + 1 )
#define DL_DETAIL     			(DL_BRIEF + 1 )
#define DL_MAX       			(DL_DETAIL + 1 )

#define DL_VDSERROR     			(0x0)
#define DL_VDSWARNING   	        (DL_VDSERROR + 1 )
#define DL_VDSBRIEF     			(DL_VDSWARNING + 1 )
#define DL_VDSDETAIL     			(DL_VDSBRIEF + 1 )
#define DL_VDSMAX       			(DL_VDSDETAIL + 1 )

#define DL_FATERROR     			(DL_VDSMAX+1)
#define DL_FATWARNING   	        (DL_FATERROR + 1 )
#define DL_FATBRIEF     			(DL_FATWARNING + 1 )
#define DL_FATDETAIL     			(DL_FATBRIEF + 1 )
#define DL_FATMAX       			(DL_FATDETAIL + 1 )

#define DL_VFSERROR     			(DL_FATMAX + 1)  
#define DL_VFSWARNING   	       (DL_VFSERROR + 1) 
#define DL_VFSBRIEF     			(DL_VFSWARNING + 1)
#define DL_VFSDETAIL     			(DL_VFSBRIEF + 1)
#define DL_VFSMAX       			(DL_VFSDETAIL + 1) 

#define DL_REGERROR                      (DL_VFSMAX + 1)
#define DL_REGWARNING   	       (DL_REGERROR + 1) 
#define DL_REGBRIEF     			(DL_REGWARNING + 1)
#define DL_REGDETAIL     			(DL_REGBRIEF + 1)
#define DL_REGMAX       			(DL_REGDETAIL + 1) 

#define DL_SMSDMERROR                 (DL_REGMAX + 1)
#define DL_SMSDMWARNING   	        (DL_SMSDMERROR + 1) 
#define DL_SMSDMBRIEF     		 (DL_SMSDMWARNING + 1)
#define DL_SMSDMDETAIL     		 (DL_SMSDMBRIEF + 1)
#define DL_SMSDMMAX       		 (DL_SMSDMDETAIL + 1) 

#define VDS_DEBUG_BUF_SIZE                 256

#ifdef _MS_VC_VER_

#define __FUNCTION__ __FILE__
#define CSW_PROFILE_FUNCTION_EXIT(fp)
#define CSW_PROFILE_FUNCTION_ENTER(fp)
#define _ASSERT_(x)     assert(x)

#define SUL_SetLastError( errno )  \
do { \
    wfcheat = FALSE;     \
    g_dsm_errno = errno; \
}while ( wfcheat )

int dsm_Printf( int index, const char *format, ... );
#else
#define _ASSERT_(x)  DS_ASSERT(x)
#define dsm_Printf CSW_TRACE

#endif


VOID dsm_Debugout( UINT32 ulLevel, CONST UINT8* szFormat, ... );
#ifndef CSW_NO_TRACE
#define D( x )         dsm_Debugout x 
#else
#define D( x ) 
#endif

VOID DSM_GetVersion( INT32 iModId, CHAR* pVersion );


#endif

