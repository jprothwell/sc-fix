/*************************************************************************
*	
*	文件：DevFunc.c
* 描述：终端设备相关函数	
* 日期：2005-8-31   	
* 作者：czm  	    	
* 
*	Copyright (c) 2005 WEP All Rights Reserved.
************************************************************************/

#ifdef MMI_ON_WIN32
//#include "windows.h" 
#endif 
#include "devcfg.h"
#include "devdef.h"
#include "interface.h"
#include "devdecl.h"
#include "med_utility.h"

//bruce100113
//#include "mmi_features_camera.h"        /* features */
//#include "cameraapp.h"
#include "mdi_datatype.h"
#include "mdi_datatype.h"
#include "mdi_camera.h"
#include "mdi_audio.h"
#include "med_utility.h"
#include "med_main.h"
#include "wepcam.h"

#include "dual_sim.h"
#include "ucs2prot.h"

#include "med_utility.h"
#ifndef WIN32
//#include "mmc_vdochat.h"
extern int   mmc_vdochat_powerOn(BOOL bIsRGB);
extern void mmc_vdochat_powerOff(void);

extern void mmc_vdochat_previewStop(void);

//extern UINT8* mmc_vdochat_getData(void);
extern UINT8 *mmc_vdochat_getData(UINT16 width, UINT16 height);
extern int MMC_EncodeFrame(unsigned char  * RgbData, int RgbDataSize,int image_width,int image_height,int out_jpeg_width, int out_jpeg_height, 
	unsigned char *p_Out,int bufsize,int q);
#endif
extern void* med_alloc_ext_mem(kal_int32 size);
extern void med_free_ext_mem(void** pointer);

#ifdef _KK_SUPPORT_ 
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
DeviceData * s_DeviceData = NULL;

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void plat_init(DeviceData * pDeviceData)
{
	s_DeviceData = pDeviceData;
 }
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////



void * DEV_Malloc(int32_t moduleId, int32_t sizeInBytes)
{
//	DeviceData * pDd = s_DeviceData;

#if 1
	void * ret=NULL;

	ret = (void *) OslMalloc((U32)sizeInBytes);

#ifdef WEP_DEBUG
	if (ret == NULL)
	{
		WM_DebugPrint("!!!!!!!!!!!!!!!!!!!!!!!!!!!!! DEV_Malloc FAIL !!!!!!!!!!!!!!!!!!!!!!");
		WM_DebugPrint("!!!!!!!!!!!!!!!!!!!!!!!!!!!!! DEV_Malloc FAIL !!!!!!!!!!!!!!!!!!!!!!");
		WM_DebugPrint("!!!!!!!!!!!!!!!!!!!!!!!!!!!!! DEV_Malloc FAIL !!!!!!!!!!!!!!!!!!!!!!");
	} 
	else
	{
		WM_DebugPrint("DEV_Malloc: moduleId =%d sizeInBytes = %d, %u",moduleId, sizeInBytes, ret);
	}
#endif

	return ret;

#else


	int32_t maxSize = 0 ;
	int32_t memPos = 0;
	int32_t index = 0;
	
#ifdef WEP_DEBUG
	WM_DebugPrint("DEV_Malloc: moduleId =%d sizeInBytes = %d",moduleId, sizeInBytes);
#endif

	switch(moduleId)
	{
	case MALLOC_MODULE_LOGIC_MAIN:
		maxSize = MALLOC_MODULE_LOGIC_MAIN_INIT_SIZE;
		memPos = MALLOC_MODULE_LOGIC_MAIN_BASE;
		break;

	case MALLOC_MODULE_LOGIC_RESPONSEBUFFER:
		maxSize = MALLOC_MODULE_LOGIC_RESPONSEBUFFER_INIT_SIZE;
		memPos = MALLOC_MODULE_LOGIC_RESPONSEBUFFER_BASE;
		break;

	case MALLOC_MODULE_LOGIC_UI:
		maxSize = MALLOC_MODULE_LOGIC_UI_INIT_SIZE;
		memPos = MALLOC_MODULE_LOGIC_UI_BASE;
		break;

	case MALLOC_MODULE_LOGIC_NET:
		maxSize = MALLOC_MODULE_LOGIC_NET_INIT_SIZE;
		memPos = MALLOC_MODULE_LOGIC_NET_BASE;
		break;

	case MALLOC_MODULE_LOCAL_DATA:
		maxSize = MALLOC_MODULE_LOCAL_DATA_INIT_SIZE;
		memPos = MALLOC_MODULE_LOCAL_DATA_BASE;
		break;

	case MALLOC_MODULE_DATA_PACKED:
		maxSize = MALLOC_MODULE_DATA_PACKED_INIT_SIZE;
		memPos = MALLOC_MODULE_DATA_PACKED_BASE;
		break;

	case MALLOC_MODULE_BUILDER:
		maxSize = MALLOC_MODULE_BUILDER_INIT_SIZE;
		memPos = MALLOC_MODULE_BUILDER_BASE;
		break;

	case MALLOC_MODULE_NET:
		maxSize = MALLOC_MODULE_NET_INIT_SIZE;
		memPos = MALLOC_MODULE_NET_BASE;
		break;

	case MALLOC_MODULE_DATA_UNPACKED:
		maxSize = MALLOC_MODULE_DATA_UNPACKED_INIT_SIZE;
		memPos = MALLOC_MODULE_DATA_UNPACKED_BASE;
		break;

	case MALLOC_MODULE_CHATDATA:
		maxSize = MALLOC_MODULE_CHATDATA_INIT_SIZE;
		memPos = MALLOC_MODULE_CHATDATA_BASE;
		break;
	case MALLOC_MODULE_IMAGE_ATTRIBUTE_DATA:
		maxSize = MALLOC_MODULE_IMAGE_ATTRIBUTE_DATA_SIZE;
		memPos = MALLOC_MODULE_IMAGE_ATTRIBUTE_DATA_BASE;
		break;

	default:
		switch(sizeInBytes % 4)
		{
			case 0:
				sizeInBytes += 4;
				break;
			case 1:
				sizeInBytes += 3;
				break;
			case 2:
				sizeInBytes += 2;
				break;
			case 3:
				sizeInBytes += 1;
				break;
			default:
				break;
		}
		index = moduleId - MALLOC_MODULE_IMAGE_DATA;
		if((index >=0) && (index <10))
		{
			memPos = MALLOC_MODULE_IMAGE_DATA_BASE;
			if(index == 0)
			{
				pDd->imgUsedBufLen = 0;
				DEV_MemSet(&pDd->memoryPool[memPos],0x0,MALLOC_MODULE_IMAGE_DATA_SIZE);
			}	
			maxSize = MALLOC_MODULE_IMAGE_DATA_SIZE - pDd->imgUsedBufLen;
			if(sizeInBytes <= maxSize)
			{
				memPos += pDd->imgUsedBufLen;
				pDd->imgUsedBufLen += sizeInBytes;
				break;
			}
		}

		index = moduleId - MALLOC_MODULE_PICTURE_DATA;
		if((index >=0) && (index <UI_MAX_PICTUIRE_NUM))
		{
			memPos = MALLOC_MODULE_PICTURE_DATA_BASE;
			if(index == 0)
			{
				pDd->picUsedBufLen = 0;
				DEV_MemSet(&pDd->memoryPool[memPos],0x0,MALLOC_MODULE_PICTURE_DATA_SIZE);
			}
// 			else if(sizeInBytes == 0 && pDd->picUsedBufLenExt>0)
// 			{
// 				pDd->picUsedBufLen -= pDd->picUsedBufLenExt;
// 				pDd->picUsedBufLenExt = 0;
// 				DEV_MemSet(&pDd->memoryPool[memPos]+pDd->picUsedBufLen,0x0,MALLOC_MODULE_PICTURE_DATA_SIZE-pDd->picUsedBufLen);
// 			}
			maxSize = MALLOC_MODULE_PICTURE_DATA_SIZE - pDd->picUsedBufLen;
			if(sizeInBytes <= maxSize)
			{
				memPos += pDd->picUsedBufLen;
				pDd->picUsedBufLen += sizeInBytes;
				break;
			}
		}
		if(memPos == 0)
		{
			return NULL;
		}
		break;
	}

WM_DebugPrint("maxSize = %d,sizeInBytes = %d",maxSize,sizeInBytes);

	
	if(sizeInBytes <= maxSize)
	{
#ifdef WEP_DEBUG
	WM_DebugPrint("DEV_Malloc: sucess maxMemSize = %d",maxSize);
#endif
		return 	&pDd->memoryPool[memPos];
	}
#ifdef WEP_DEBUG
	WM_DebugPrint("DEV_Malloc: FAIL!!!!!!!!!!!!!!!!!!!!!!!!");
#endif

	return NULL;
#endif
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void DEV_Free(void * p)
{
	if(p != NULL) {
	WM_DebugPrint(">>>>>>>DEV_Free: %u",p);
		OslMfree((void*)p);
// 		mmi_frm_scrmem_free(p);
	}
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void * DEV_MemCpy(void *dest, const void *src, int32_t count)
{
	return gui_memcpy(dest, src, count);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void * DEV_MemSet(void *dest, int32_t c, int32_t count)
{
	return memset(dest, c, count);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// 	compare characters in two buffers
//	  p1:    string 1 to be compared
//		p2:    string 2 to be compared
//		count: length to be compared
//	Return:
//		< 0   if p1 less than p2
//		= 0   if p1 identical to p2
//		> 0   if p1 greater than p2
int32_t DEV_MemCmp(const void * p1, const void * p2, int32_t count)
{
	return memcmp(p1, p2, count);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//	Find a the first occurence of specific data in a memory buffer
//	  pBuf: memory to search in
//		nBufLen: size of memory (bytes)
//		pFind: data to be found
//		nFindLen: size of data to be found (bytes)
//	Return:
//		< 0 if error
//		the first occurence position
int32_t DEV_MemFind(const byte_t * pBuf, int32_t nBufLen,
										 const byte_t * pFind, int32_t nFindLen)
{
	int32_t i;

	if(nBufLen < nFindLen) {
		return -1;	// length error
	}

	for(i=0;i<=nBufLen-nFindLen;i++)
	{
		if(0 == DEV_MemCmp(pBuf+i, pFind, nFindLen) ) {
			return i;
		}
	}

	return -2;	// not found
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// convert net order data to local string ( for BREW, local string is UCS-2 )
//	  pDest:   buffer to keep local string(GB)
//		pSrc:    net order data to be converted
//		nSrcLen: size of net order data (bytes)
//	Return:
//		length of local string (chars)
//	EG:  0x4E 0x2D 0x65 0x87 0x00 0x31 0x00 0x00 => 0x4E2D 0x6587 0x0031 0x0000
int32_t DEV_NetUniByte2LocStr(UI_character_type * pDest, const byte_t * pSrc, int32_t nSrcLen)
{
	int32_t i,ssLen = nSrcLen / 2;

	if(pDest == NULL || pSrc == NULL)
		return 0;

	*pDest = 0x00;

	for(i=0;i<ssLen;i++)
	{
		pDest[i] = (U16)(((pSrc[i*2] & 0xff) << 8) | (pSrc[i*2+1] & 0xff));
		if((pSrc[i*2] == 0) && (pSrc[i*2+1] == 0))
			break;
	}
	pDest[i] = 0x0000;

	return i;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//convert local string to net order data( for BREW, local string is UCS-2 )
//	  pDest:   buffer to keep net order data
//		pSrc:    local string(GB) to be converted
//	Return:
//		length of net order data (bytes)
//	EG:  0x4E2D 0x6587 0x0031 0x0000 => 0x4E 0x2D 0x65 0x87 0x00 0x30 0x00 0x00
int32_t DEV_LocStr2NetUniByte(byte_t * pDest, const UI_character_type * pSrc, int32_t nSrcLen)
{
	int32_t i=0;
	uint16_t c;
	
	if(pSrc == NULL || pDest == NULL)
		return 0;

	for(i=0;i<nSrcLen;i++)
	{
		c = pSrc[i];
		pDest[i*2] = (c >> 8) & 0xff;
		pDest[i*2+1] = c & 0xff;
	}
	pDest[i*2] = 0x00;
	pDest[i*2+1] = 0x00;

	return (i*2);
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
static void DEV_Timer0CB(void)
{
	DeviceData * pDd = s_DeviceData;
	pDd->pfnTimerCB[TIMER_CONNECT_INFO](pDd->pTimerData[TIMER_CONNECT_INFO]);
}
static void DEV_Timer1CB(void)
{
	DeviceData * pDd = s_DeviceData;
	pDd->pfnTimerCB[TIMER_VITAL_MSG](pDd->pTimerData[TIMER_VITAL_MSG]);
}
static void DEV_Timer2CB(void)
{
	DeviceData * pDd = s_DeviceData;
	pDd->pfnTimerCB[TIMER_MYTURN_PROMPT](pDd->pTimerData[TIMER_MYTURN_PROMPT]);
}
static void DEV_Timer3CB(void)
{
	DeviceData * pDd = s_DeviceData;
	pDd->pfnTimerCB[TIMER_NET_TIMEOUT](pDd->pTimerData[TIMER_NET_TIMEOUT]);
}
#ifdef SUPPORT_ANIMATION
static void DEV_Timer5CB(void)
{
	DeviceData * pDd = s_DeviceData;
	pDd->pfnTimerCB[TIMER_ANIMATION](pDd->pTimerData[TIMER_ANIMATION]);
}

#endif

#ifdef SUPPORT_PROXY
static void DEV_Timer9CB(void)
{
	DeviceData * pDd = s_DeviceData;
	pDd->pfnTimerCB[TIMER_HTTP_SEND](pDd->pTimerData[TIMER_HTTP_SEND]);
}

#endif

static void DEV_Timer10CB(void)
{
	DeviceData * pDd = s_DeviceData;
#ifdef WEP_DEBUG
	WM_DebugPrint("DEV_Timer10CB = %d",TIMER_VIDEOCHAT_CAMERA_GET);
#endif
	pDd->pfnTimerCB[TIMER_VIDEOCHAT_CAMERA_GET](pDd->pTimerData[TIMER_VIDEOCHAT_CAMERA_GET]);
}

#ifdef VIDEO_CACHE
static void DEV_Timer11CB(void)
{
	DeviceData * pDd = s_DeviceData;
#ifdef WEP_DEBUG
	WM_DebugPrint("DEV_Timer11CB = %d",TIMER_VIDEOCHAT_VIDEO_SHOW);
#endif
	pDd->pfnTimerCB[TIMER_VIDEOCHAT_VIDEO_SHOW](pDd->pTimerData[TIMER_VIDEOCHAT_VIDEO_SHOW]);
}
#endif

#ifdef SUPPORT_RECORD_VOICE
static void DEV_Timer12CB(void)
{
	DeviceData * pDd = s_DeviceData;
#ifdef WEP_DEBUG
	WM_DebugPrint("DEV_Timer12CB = %d",TIMER_RECORD_PROMPT);
#endif
	pDd->pfnTimerCB[TIMER_RECORD_PROMPT](pDd->pTimerData[TIMER_RECORD_PROMPT]);
}
#endif


// callback function define
 
typedef void (* TIMER_PROC)(void);
//#endif

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
int32_t DEV_TimerStart(void * pDevData, int32_t timerId,
							int32_t nElapseMs, PFNTIMERCALLBACK pfn, void * pUserData)
{
	TIMER_PROC pfnTimerProc[MAX_TIMER_NUM] = {
		(TIMER_PROC)DEV_Timer0CB,
		(TIMER_PROC)DEV_Timer1CB,
		(TIMER_PROC)DEV_Timer2CB,
		(TIMER_PROC)DEV_Timer3CB,
#ifdef SUPPORT_ANIMATION
		(TIMER_PROC)DEV_Timer5CB,
#endif
#ifdef SUPPORT_PROXY
		(TIMER_PROC)DEV_Timer9CB,
#endif
		(TIMER_PROC)DEV_Timer10CB,
#ifdef VIDEO_CACHE
		(TIMER_PROC)DEV_Timer11CB,
#endif
#ifdef SUPPORT_RECORD_VOICE
		(TIMER_PROC)DEV_Timer12CB,
#endif
	};
	DeviceData * pDd = s_DeviceData;
	if(timerId < MAX_TIMER_NUM) {
//		WEPAppData * pWep = (WEPAppData *)pDd->pAppData;
//		if (pWep->FTP_InProcess)//tmp 
		{
#ifdef SUPPORT_PROXY
			if (timerId == TIMER_HTTP_SEND)
			{
				return 0;
			}
#endif
		}
		pDd->pfnTimerCB[timerId] = pfn;
		pDd->pTimerData[timerId] = pUserData;

		gui_start_timer(nElapseMs, pfnTimerProc[timerId]);

		return 0; // OK
	}
	return 1;	// failed
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
int32_t DEV_TimerStop(void * pDevData, int32_t timerId)
{
	TIMER_PROC pfnTimerProc[MAX_TIMER_NUM] = {
		(TIMER_PROC)DEV_Timer0CB,
		(TIMER_PROC)DEV_Timer1CB,
		(TIMER_PROC)DEV_Timer2CB,
		(TIMER_PROC)DEV_Timer3CB,
#ifdef SUPPORT_ANIMATION
		(TIMER_PROC)DEV_Timer5CB,
#endif
#ifdef SUPPORT_PROXY
		(TIMER_PROC)DEV_Timer9CB,
#endif
		(TIMER_PROC)DEV_Timer10CB,
#ifdef VIDEO_CACHE
		(TIMER_PROC)DEV_Timer11CB,
#endif
#ifdef SUPPORT_RECORD_VOICE
		(TIMER_PROC)DEV_Timer12CB,
#endif
	};
//	DeviceData * pDd = s_DeviceData;

	if(timerId < MAX_TIMER_NUM) {
		gui_cancel_timer(pfnTimerProc[timerId]);
		return 0;	// OK
	}
	return 1;	// failed
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
int32_t DEV_LocalIconInfoGet(void * pDevData, byte_t buf[], int32_t bufLen)
{
	return 0;

} // end DEV_LocalIconInfoGet()

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
int32_t DEV_RandNumGet()
{
   int ret = 100;
	UI_time t;
	/* Seed the random-number generator with current time so that
	* the numbers will be different every time we run.
	*/
	/*Set random seed */
	GetDateTime(&t);
	srand(t.nHour*3600 + t.nMin*60 + t.nSec); 
	 /* Display 10 numbers. */
	 while((ret = rand()) == 0);
	 return ret;
} // end DEV_RandNumGet()

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
int32_t DEV_GetDiskFreeBytes(const WCHAR *fileName)
{
	FS_DiskInfo DiskInfo;
	S32 free_bytes = 0;
	
	MMI_FS_GetDiskInfo((const UINT8 *)fileName, &DiskInfo, FS_DI_FREE_SPACE | FS_DI_BASIC_INFO);
	free_bytes = DiskInfo.FreeClusters * DiskInfo.SectorsPerCluster * DiskInfo.BytesPerSector;
	return free_bytes;
}

//BN1102 b
uint32_t DEV_GetPlatSWVer( void )
{
	return 72; 
}

uint32_t DEV_GetPlatBBVer( void )
{
	return 12;
}
//BN1012 e

//export unicode func //BN10047 must init
int (*pfnKKUnicodeStrlen) (const char *arrOut) = UCS2Strlen;
char* (*pfnKKUnicodeStrcpy) (char *strDestination, const char *strSource) = UCS2Strcpy;
char* (*pfnKKUnicodeStrncpy) (char *strDestination, const char *strSource, uint32_t size) = UCS2Strncpy;
int (*pfnKKUnicodeStrcmp) (const char *string1, const char *string2) = UCS2Strcmp;
int32_t (*pfnKKUnicodeStrncmp) (const char *string1, const char *string2, uint32_t size) = UCS2Strncmp ; 
char* (*pfnKKUnicodeStrcat) (char *strDestination, const char *strSource) = UCS2Strcat;
char* (*pfnKKUnicodeStrncat) (char *strDestination, const char *strSource, uint32_t size) = mmi_ucs2ncat;

uint16_t (*pfnKKAnsiiToUnicodeString)(char *pOutBuffer, char *pInBuffer) = mmi_asc_to_ucs2;
uint16_t (*pfnKKUnicodeToAnsii)(char *pOutBuffer, char *pInBuffer) = UnicodeToAnsii;

void DevInitUnicodeSupport(void)
{
	#if 0
	//pfnKKUnicodeToEncodingScheme = mmi_wc_to_ucs2;
	//pfnKKEncodingSchemeToUnicode = mmi_ucs2_to_wc;
	pfnKKUnicodeStrlen  = mmi_ucs2strlen;	//pfnUnicodeStrlen
	pfnKKUnicodeStrcpy  = mmi_ucs2cpy;	//pfnUnicodeStrcpy
	pfnKKUnicodeStrcmp  = mmi_ucs2cmp;	//pfnUnicodeStrcmp
	pfnKKUnicodeStrncpy = mmi_ucs2ncpy;	//	pfnUnicodeStrncpy
	pfnKKUnicodeStrncmp = mmi_ucs2ncmp;	//pfnUnicodeStrncmp
	pfnKKUnicodeStrcat  = mmi_ucs2cat;	//pfnUnicodeStrcat
	pfnKKUnicodeStrncat = mmi_ucs2ncat; //	pfnUnicodeStrncat

	pfnKKAnsiiToUnicodeString = mmi_asc_to_ucs2;
	pfnKKUnicodeToAnsii = mmi_ucs2_to_asc;
	#endif
}

#if 0
uint16_t KKAnsiiToUnicodeString(char *pOutBuffer, char *pInBuffer)
{
#if (KK_MTKXXXX_SWVER<KK_MTKXXXX_V08A)
	return AnsiiToUnicodeString(pOutBuffer, pInBuffer);
#elif (KK_MTKXXXX_SWVER<KK_MTKXXXX_V09A)
	return mmi_asc_to_ucs2(pOutBuffer, pInBuffer);
#else
	return AnsiiToUnicodeString(pOutBuffer, pInBuffer);
#endif

}

uint16_t KKUnicodeToAnsii(char *pOutBuffer, char *pInBuffer)
{
#if (KK_MTKXXXX_SWVER<KK_MTKXXXX_V08A)
	return UnicodeToAnsii(pOutBuffer, pInBuffer);
#elif (KK_MTKXXXX_SWVER<KK_MTKXXXX_V09A)
	return mmi_ucs2_to_asc(pOutBuffer, pInBuffer);
#else
	return UnicodeToAnsii(pOutBuffer, pInBuffer);
#endif

}
#endif

 
uint8_t DEV_IsMtkCodec( void )
{
#if defined(_KK_MTKCODEC_) && !defined(WIN32)// && (KK_MTKXXXX_SWVER < KK_MTKXXXX_V08A)
	return 1;
#else
	return 0;
#endif
}

uint8_t DEV_IsKKDec( void )
{
#if defined(WIN32)// || (KK_MTKXXXX_SWVER >= KK_MTKXXXX_V08A)
	return 1;
#else
	return 0;
#endif
}

int Dev_DecodeFrame(unsigned char  * inputData, int inputDataSize,int image_width,int image_height,unsigned char *p_Out,int bufsize)
{
	//return 0;
#if defined(WIN32)// || (KK_MTKXXXX_SWVER >= KK_MTKXXXX_V08A)
	return Dev_UJDecodeFrame(inputData, inputDataSize, image_width, image_height, p_Out, bufsize);
#else
	return 0;
#endif
}

int Dev_EncodeFrame(unsigned char  * RgbData, int RgbDataSize,int image_width,int image_height,int out_jpeg_width, int out_jpeg_height, \
	unsigned char *p_Out,int bufsize,int q)
{
	//return 0;
#if (!defined(_KK_MTKCODEC_) /*&& (KK_MTKXXXX_SWVER >= KK_MTKXXXX_V08A)*/) ||defined(WIN32)
#if defined(WIN32)
	return Dev_UJEncodeFrame(RgbData, RgbDataSize, image_width, image_height, out_jpeg_width,  out_jpeg_height, p_Out, bufsize, q);
	//return 3000;
#else
	return MMC_EncodeFrame(RgbData, RgbDataSize, image_width, image_height, out_jpeg_width,  out_jpeg_height, p_Out, bufsize, q);
#endif
#else
	return 0;
#endif
}

//bruce0912010
#ifdef __KK_UI_V2__
void DEV_GeVideoResolution( video_resol_id *local, video_resol_id *remote )
{
	//note: local <= VIDEO_RESOL03_96X96, remote < VIDEO_RESOL_VER1_MAX
	//test  resolution
#if 0	
	*local = VIDEO_RESOL02_80X80;
	*remote = VIDEO_RESOL04_112X112;
	return;
#endif

	//auto adjust resolution
#if 1
	if( (UI_device_width==128 && UI_device_height==128) || \
		(UI_device_width==128 && UI_device_height==160))
	{
		*local = VIDEO_RESOL00_48X48;
		*remote = VIDEO_RESOL00_48X48;
	}
	else if( UI_device_width==160 && UI_device_height==128 )
	{
		*local = VIDEO_RESOL00_48X48;
		*remote = VIDEO_RESOL01_64X64;
	}
	else if( UI_device_width==176 && UI_device_height==220 )
	{
#if defined(__MMI_TOUCH_SCREEN__)	
		*local = VIDEO_RESOL00_48X48;
		*remote = VIDEO_RESOL01_64X64;
#else
		*local = VIDEO_RESOL01_64X64;
		*remote = VIDEO_RESOL02_80X80;
#endif
	}
	else if( UI_device_width==220 && UI_device_height==176 )
	{
#if defined(__MMI_TOUCH_SCREEN__)	
		*local = VIDEO_RESOL01_64X64;
		*remote = VIDEO_RESOL01_64X64;
#else
		*local = VIDEO_RESOL01_64X64;
		*remote = VIDEO_RESOL02_80X80;
#endif
	}
	else if( UI_device_width==240 && UI_device_height==320 )
	{
		*local = VIDEO_RESOL01_64X64;
		*remote = VIDEO_RESOL03_96X96;
	}
	else if( UI_device_width==240 && (UI_device_height==400 || UI_device_height==432) )
	{
		*local = VIDEO_RESOL01_64X64;
		*remote = VIDEO_RESOL03_96X96;
	}
	else if( (UI_device_width==320 || UI_device_width==400 || UI_device_width==432) && UI_device_height==240 )
	{
#if defined(__MMI_TOUCH_SCREEN__)	
		*local = VIDEO_RESOL01_64X64;
		*remote = VIDEO_RESOL01_64X64;
#else
		*local = VIDEO_RESOL01_64X64;
		*remote = VIDEO_RESOL02_80X80;
#endif
	}
	else if( UI_device_width==320 && UI_device_height==480 )
	{
		*local = VIDEO_RESOL02_80X80;
		*remote = VIDEO_RESOL04_112X112;
	}
	else if( UI_device_width==480 && UI_device_height==320 )
	{
		*local = VIDEO_RESOL02_80X80;
		*remote = VIDEO_RESOL04_112X112;
	}
#endif

	//user resolution configuration
#if 0
#if defined(__MMI_MAINLCD_176X220__) || defined(__MMI_MAINLCD_220X176__)
	*local = VIDEO_RESOL01_64X64;
	*remote = VIDEO_RESOL02_80X80;
#elif defined(__MMI_MAINLCD_320X240__) || defined(__MMI_MAINLCD_400X240__) || defined(__MMI_MAINLCD_432X240__)
	*local = VIDEO_RESOL01_64X64;
	*remote = VIDEO_RESOL02_80X80;
#elif defined(__MMI_MAINLCD_240X320__) || defined(__MMI_MAINLCD_240X400__) || defined(__MMI_MAINLCD_240X432__)
	*local = VIDEO_RESOL01_64X64;
	*remote = VIDEO_RESOL03_96X96;
#elif defined(__MMI_MAINLCD_320X480__) || defined(__MMI_MAINLCD_480X320__)
	*local = VIDEO_RESOL03_96X96;
	*remote = VIDEO_RESOL04_112X112;
#elif defined(__MMI_MAINLCD_220X176__)
	*local = VIDEO_RESOL01_64X64;
	*remote = VIDEO_RESOL01_64X64;	
#else
	*local = VIDEO_RESOL01_64X64;
	*remote = VIDEO_RESOL03_96X96;
#endif
#endif
}

uint8_t DEV_IsConfirmScreen( void )
{
	if(GetActiveScreenId() == SCR_CONFIRM_SCREEN)
		return 1;
	else
		return 0;
}

void DEV_SetVideoStartTime( void )
{
//	g_wep_start_time = applib_dt_get_rtc_time();//app_getcurrtime();
}

//bruce for adjust horizontal x offset
uint8_t DEV_SetVideoOffsetX( void )
{
	if( UI_device_width==176 && UI_device_height==220 )
		return 8;
	else
		return 0;
}

//for get global image id for kk
uint16_t DEV_GetImgId( uint16_t kk_img_id )
{
	uint16_t sys_img_id = 0;
	switch(kk_img_id)
	{
		case KK_IMG_VIEWCHAT_000:
			sys_img_id = IMG_VIEWCHAT_000;
			break;
		case KK_IMG_VIEWCHAT_001:
			sys_img_id = IMG_VIEWCHAT_001;
			break;
		case KK_IMG_VIEWCHAT_002:
			sys_img_id = IMG_VIEWCHAT_002;
			break;
		case KK_IMG_VIEWCHAT_CHECKBOX1:
			sys_img_id = IMG_VIEWCHAT_CHECKBOX1;
			break;
		case KK_IMG_VIEWCHAT_CHECKBOX2:
			sys_img_id = IMG_VIEWCHAT_CHECKBOX2;
			break;
		case KK_IMG_VIEWCHAT_WAITING:
			sys_img_id = IMG_VIEWCHAT_WAITING;
			break;
		case KK_IMG_VIEWCHAT_SLOGO:
			sys_img_id = IMG_VIEWCHAT_SLOGO;
			break;
		case KK_IMG_VIEWCHAT_LOGO:
			sys_img_id = IMG_VIEWCHAT_LOGO;
			break;
		case KK_IMG_VIEWCHAT_UP_ARROW:
			sys_img_id = IMG_VIEWCHAT_UP_ARROW;
			break;
		case KK_IMG_VIEWCHAT_DOWN_ARROW:
			sys_img_id = IMG_VIEWCHAT_DOWN_ARROW;
			break;
		case KK_IMG_VIEWCHAT_V2_KK_TITLE:
			sys_img_id = IMG_VIEWCHAT_V2_KK_TITLE;
			break;
		case KK_IMG_VIEWCHAT_V2_KK_TITLE_MIDDLE:
			sys_img_id = IMG_VIEWCHAT_V2_KK_TITLE_MIDDLE;
			break;
		case KK_IMG_VIEWCHAT_V2_KK_TITLE_MINI:
			sys_img_id = IMG_VIEWCHAT_V2_KK_TITLE_MINI;
			break;
		case KK_IMG_VIEWCHAT_V2_KK_VIDEO_TITLE_CH:
			sys_img_id = IMG_VIEWCHAT_V2_KK_VIDEO_TITLE_CH;
			break;
		case KK_IMG_VIEWCHAT_V2_KK_VIDEO_TITLE_EN:
			sys_img_id = IMG_VIEWCHAT_V2_KK_VIDEO_TITLE_EN;
			break;
		case KK_IMG_VIEWCHAT_V2_CHAT_WAITING:
			sys_img_id = IMG_VIEWCHAT_V2_CHAT_WAITING;
			break;
		case KK_IMG_VIEWCHAT_V2_CHAT_WAITING_MINI:
			sys_img_id = IMG_VIEWCHAT_V2_CHAT_WAITING_MINI;
			break;
		case KK_IMG_VIEWCHAT_V2_BUDDY_NAME:
			sys_img_id = IMG_VIEWCHAT_V2_BUDDY_NAME;
			break;
		case KK_IMG_VIEWCHAT_V2_DURING_TIME:
			sys_img_id = IMG_VIEWCHAT_V2_DURING_TIME;
			break;
		case KK_IMG_VIEWCHAT_V2_GPRS_SIG00:
			sys_img_id = IMG_VIEWCHAT_V2_GPRS_SIG00;
			break;
		case KK_IMG_VIEWCHAT_V2_GPRS_SIG11:
			sys_img_id = IMG_VIEWCHAT_V2_GPRS_SIG11;
			break;
		case KK_IMG_VIEWCHAT_V2_GPRS_SIG22:
			sys_img_id = IMG_VIEWCHAT_V2_GPRS_SIG22;
			break;
		case KK_IMG_VIEWCHAT_V2_GPRS_SIG33:
			sys_img_id = IMG_VIEWCHAT_V2_GPRS_SIG33;
			break;
		case KK_IMG_VIEWCHAT_V2_GPRS_SIG44:
			sys_img_id = IMG_VIEWCHAT_V2_GPRS_SIG44;
			break;
		default:
			sys_img_id = 0;
			break;

	}

	return sys_img_id;
}


#endif


 
void DEV_CameraCaptureFromDsp(unsigned  char *buf, unsigned  int *buf_size)
{
#ifndef WIN32
#if (KK_MTKXXXX_BBCHIP == KK_MTKXXXX_MT6223)
#ifdef _KK_DSP_GS500_SUPPORT_
	gs500_kk_camera_capture(buf, buf_size);
#endif
#endif
#endif
}


MDI_RESULT DEV_CameraPreviewStart(  mdi_camera_preview_struct *preview_p,  mdi_camera_setting_struct *setting_p)
{
	MDI_RESULT  result = MED_RES_FAIL ;
	
#ifndef WIN32
#if (KK_MTKXXXX_BBCHIP == KK_MTKXXXX_MT6223)
#ifdef _KK_DSP_GS500_SUPPORT_
	gs500_kk_camera_preview();
	result = MED_RES_OK;
#endif
#else //! (KK_MTKXXXX_BBCHIP == KK_MTKXXXX_MT6223)
	result = mdi_camera_preview_start(preview_p->preview_layer_handle,
								   preview_p->preview_wnd_offset_x,
								   preview_p->preview_wnd_offset_y,
								   preview_p->preview_wnd_width,
								   preview_p->preview_wnd_height,
								   preview_p->blt_layer_flag,
								   preview_p->preview_layer_flag,
								   GDI_COLOR_BLACK,
								   TRUE,
								    setting_p);
#endif
#endif

	return result;
}

MDI_RESULT DEV_CameraPowerOn(void)
{
	MDI_RESULT  result = MDI_RES_CAMERA_SUCCEED;
	
#ifndef WIN32
#if (KK_MTKXXXX_BBCHIP == KK_MTKXXXX_MT6223)
#ifdef _KK_DSP_GS500_SUPPORT_
	gs500_kk_camera_pwron();
	result = MDI_RES_CAMERA_SUCCEED;
#endif
#else //! (KK_MTKXXXX_BBCHIP == KK_MTKXXXX_MT6223)
//	result = mdi_camera_power_on();
	mmc_vdochat_powerOn(FALSE);
	result = MDI_RES_CAMERA_SUCCEED;
#endif
#endif

	return result;
}

void DEV_CameraPowerOff(void)
{
#ifndef WIN32
#if (KK_MTKXXXX_BBCHIP == KK_MTKXXXX_MT6223)
#ifdef _KK_DSP_GS500_SUPPORT_
	gs500_kk_camera_pwroff();
#endif
#else //! (KK_MTKXXXX_BBCHIP == KK_MTKXXXX_MT6223)
//	mdi_camera_preview_stop();
//	mdi_camera_power_off();
	mmc_vdochat_previewStop() ;
	mmc_vdochat_powerOff();
#endif
#endif
}

uint8_t *DEV_CameraGetFrameSrc(void)
{
	uint8_t *frame_src_ptr = NULL;
	
#ifdef WIN32
	frame_src_ptr = (PU8)g_wepcam.preview_layer_buf_ptr1;
#else
#if (KK_MTKXXXX_BBCHIP == KK_MTKXXXX_MT6223)
#ifdef _KK_DSP_GS500_SUPPORT_
	frame_src_ptr = (PU8)g_wepcam.preview_layer_buf_ptr; 
#endif
#else //! (KK_MTKXXXX_BBCHIP == KK_MTKXXXX_MT6223)
if(kk_is_support_horizon_cam() > 0)
	frame_src_ptr = (PU8)g_wepcam.preview_layer_buf_ptr1; 
else	
//	frame_src_ptr = (PU8)g_wepcam.preview_layer_buf_ptr;//isp_preview_config_data.frame_buffer_address;
	frame_src_ptr = (PU8)mmc_vdochat_getData(96,96) ;
#endif
#endif

	return frame_src_ptr;
}

void DEV_CameraGetResolution(uint16_t *cap_width, uint16_t *cap_height, uint16_t *transf_width, uint16_t *transf_height)
{
#ifndef WIN32
#if (KK_MTKXXXX_BBCHIP == KK_MTKXXXX_MT6223)
#ifdef _KK_DSP_GS500_SUPPORT_
	//from cmos sensor
	*cap_width = KK_DSP_CAPTURE_WIDTH;
	*cap_height = KK_DSP_CAPTURE_HEIGHT;
	
	*transf_width = KK_DSP_TRANSFER_WIDTH;
	*transf_height = KK_DSP_TRANSFER_HEIGHT;
#endif
#endif
#endif
}

//BN10010 b
#ifdef __CAMERA_FEATURE_ZOOM__	//bruce100113
extern int g_camera_feature_zoom[];
#endif

extern const U8 camera_wb_command_map[];
extern const U8 camera_ev_command_map[];

extern const U8 camera_banding_command_map[];
extern const U8 camera_flash_command_map[];
extern const U8 camera_iso_command_map[];
extern const U8 camera_ae_meter_command_map[];
extern const U8 camera_dsc_mode_command_map[];
extern const U8 camera_autofocus_mode_command_map[];
extern const U8 camera_autofocus_command_map[];

extern const U8 camera_effect_command_map[];
extern const U8 camera_image_qty_command_map[];

#if  0
void mmi_camera_get_preview_settings_for_wep(mdi_camera_setting_struct *camera_setting_data)
{
#if (KK_MTKXXXX_BBCHIP != KK_MTKXXXX_MT6223)	//bruce100113
    camera_setting_data->zoom = g_camera_feature_zoom[g_camera_cntx.setting.zoom_idx];
#else
    camera_setting_data->zoom = 10;
#endif
    camera_setting_data->wb = camera_wb_command_map[g_camera_cntx.setting.wb];
    camera_setting_data->manual_wb = FALSE;
    camera_setting_data->ev = camera_ev_command_map[g_camera_cntx.setting.ev];
    camera_setting_data->banding = camera_banding_command_map[g_camera_cntx.setting.banding];
    camera_setting_data->flash = camera_flash_command_map[g_camera_cntx.setting.flash];
    camera_setting_data->osd_rotate = g_camera_cntx.osd_rotate;
    camera_setting_data->night = g_camera_cntx.setting.night;

    camera_setting_data->iso = camera_iso_command_map[g_camera_cntx.setting.iso];
    camera_setting_data->ae_meter = camera_ae_meter_command_map[g_camera_cntx.setting.ae_meter];
    camera_setting_data->dsc_mode = camera_dsc_mode_command_map[g_camera_cntx.setting.dsc_mode];
    camera_setting_data->af_mode = camera_autofocus_mode_command_map[g_camera_cntx.setting.af_mode];
    camera_setting_data->af_metering_mode = camera_autofocus_command_map[g_camera_cntx.setting.af_meter];
    camera_setting_data->shutter_pri = 0;
    camera_setting_data->aperture_pri = 0;

    /* default effect */
    camera_setting_data->effect = camera_effect_command_map[g_camera_cntx.setting.effect];
    camera_setting_data->brightness = 128;
    camera_setting_data->saturation = 128;
    camera_setting_data->contrast = 128;
    camera_setting_data->hue = 0;

    camera_setting_data->effect = camera_effect_command_map[CAMERA_SETTING_EFFECT_NORMAL];
    camera_setting_data->brightness = g_camera_cntx.setting.fx_brightness[0];
    camera_setting_data->saturation = g_camera_cntx.setting.fx_saturation[0];
    camera_setting_data->contrast = g_camera_cntx.setting.fx_contrast[0];
    camera_setting_data->hue = g_camera_cntx.setting.fx_hue[0];

    camera_setting_data->overlay_frame_mode = FALSE;
    camera_setting_data->overlay_frame_depth = 0;
    camera_setting_data->overlay_frame_width = 0;
    camera_setting_data->overlay_frame_height = 0;
    camera_setting_data->overlay_frame_source_key = 0;
    camera_setting_data->overlay_frame_buffer_address = 0;


//bruce091210    
#ifdef __KK_UI_V2__
{
    uint16_t preview_w =0;
    uint16_t preview_h =0;
    uint16_t img_w =0;
    uint16_t img_h =0;
    extern void WepGetCameraSettings( uint16_t *preview_w,  uint16_t *preview_h, uint16_t *img_w, uint16_t *img_h );
		
    WepGetCameraSettings( &preview_w,  &preview_h, &img_w, &img_h );

    camera_setting_data->preview_width = preview_w;
    camera_setting_data->preview_height = preview_h;

    camera_setting_data->image_width = img_w;
    camera_setting_data->image_height = img_h;
}

#else
    camera_setting_data->preview_width = WEP_PREVIEW_WIDTH;
    camera_setting_data->preview_height = WEP_PREVIEW_HEIGHT;

    camera_setting_data->image_width = WEP_IMAGE_WIDTH;
    camera_setting_data->image_height = WEP_IMAGE_HEIGHT;
#endif

    /* capture parameters */
    camera_setting_data->image_qty = camera_image_qty_command_map[CAMERA_SETTING_IMAGE_QTY_LOW];

    camera_setting_data->lcm = MDI_CAMERA_PREVIEW_LCM_MAINLCD; 
    camera_setting_data->preview_rotate = CAMERA_PREVIEW_MAINLCD_ROTATE; //BN10010 for open interface to get
    camera_setting_data->lcm_rotate = CAMERA_LCM_MAINLCD_ROTATE; //BN10010 for open interface to get

    camera_setting_data->continue_capture = FALSE;

}
#endif
//BN10112 b
void DEV_BitBlt(S32 x1, S32 y1, S32 x2, S32 y2)
{

	gdi_layer_blt_ext(
	//	dm_get_layer_handle(0),//GDI_LCD->blt_handle[0],	
		g_wepcam.g_wepcam_base_layer,//GDI_LCD->blt_handle[1],		
		GDI_LCD->blt_handle[1],	/* layer 1 */
		GDI_LCD->blt_handle[2],
		0,										/* layer 3 */
#ifdef GDI_6_LAYERS
		0, 0,
#endif 
		x1, y1, x2, y2);
	
}
//BN10112 e

//BN10117
int DEV_IsNoneCamera( void )
{
#if defined(SUPPORT_DISABLE_PREVIEW) 
	return 1;
#else
	return 0;
#endif
}

#endif
