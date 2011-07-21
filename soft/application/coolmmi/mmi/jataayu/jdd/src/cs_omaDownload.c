 /***************************************************************************
 * System Include Files
 **************************************************************************/
#ifdef MMI_ON_HARDWARE_P
#include "cs_types.h"
#else
#include "cswtype.h"
#endif
#include "fs.h"
#include "errorcode.h"
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/
#include <ddl.h>
#include <jcal.h>
#include "jdi_cutils_tcsstr.h"
#include "jwap_mms_common.h"
#include "fat_fs.h"
#include "fmt_def.h"
#include "ucs2prot.h"


extern fmgr_filter_struct fmgr_filter[FMGR_MAX_FILTER_COUNT];
extern UINT32 fmgr_supporttype[];
extern UINT32 cs_getSupportCnt(void);
extern UINT32 cs_getSupporttype(UINT32 index);

JC_UINT32 cs_RenameFile (JC_CHAR* pmFilePath, JC_CHAR* pmRenamedName)
{
	return FS_Rename((PCSTR)pmFilePath,(PCSTR)pmRenamedName);
}
JC_UINT32 cs_DeleteFile (JC_CHAR* pmFilePath)
{
	return FS_Delete((PCSTR)pmFilePath);
}

void cs_IsSupportedContentType (JC_CHAR* pmMimeType, JC_BOOLEAN * bIsSupported)  
{
		JC_CHAR *tp,extension[20];
		UINT8 tmp1[100];
		INT32 vi=0;
		UINT32 type=FMGR_TYPE_UNKNOW;
		//get jataayu file type 
		JC_UINT16  slash ='/';
		tp=(JC_CHAR*)UCS2Strchr(( const S8 *)pmMimeType,slash);
		UCS2Strcpy((S8*)extension, (const S8 *) tp);
		UnicodeToAnsii(tmp1,(S8*)extension);
		mmi_trace(1,"chenhe,tmp1 is %s",tmp1);
		//make upper
			while( vi<strlen((const S8 *)tmp1) )
			{
				if(tmp1[vi] >= 'a'&&tmp1[vi]<='z')
				{
					tmp1[vi] += 'A' - 'a';
				}
				vi++;
			}
	mmi_trace(1,"chenhe,cs_IsSupportedContentType,1,tmp1 is %s",tmp1);
		//get type
		for(vi = 0 ; vi < FMGR_MAX_FILTER_COUNT ; vi++)
		{

			if((tmp1[0] == fmgr_filter[vi].ext[0]) &&
			(tmp1[1] == fmgr_filter[vi].ext[1]) &&
			(tmp1[2] == fmgr_filter[vi].ext[2]))
			{
				type = fmgr_filter[vi].type;
				break;
			}
		}
	mmi_trace(1,"chenhe,cs_IsSupportedContentType,2");

		for(vi=0;vi++<cs_getSupportCnt();vi++)
		{

			if(type == cs_getSupporttype(vi))
			{
				*bIsSupported = TRUE;
				return;
			}
		}
	mmi_trace(1,"chenhe,cs_IsSupportedContentType,3");

		*bIsSupported = FALSE;
		
}

