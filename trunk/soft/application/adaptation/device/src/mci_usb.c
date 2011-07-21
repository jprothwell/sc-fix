
#include "cs_types.h"
//added for USB module
#include "hal_usb.h"
#include "hal_sys.h"
#include "uctls_m.h"
#include "sxs_io.h"
#include "pmd_m.h"
#include "usb_interface.h"
#include "cos.h"
#include "errorcode.h"
#include "fs.h"

#include "mmi_trace.h"
#include"mmi_features.h"

int checkUsbstate(void);

#ifdef __MMI_EBOOK_READER__
	extern void mmi_ebook_usb_mode_off(void);
	extern void mmi_ebook_usb_mode_on(void);
#endif	

// When the macro CSW_EXTENDED_API_PMD_USB is defined, the code to handle the USB
// is in CSW USB and PMD, not in MDI anymore.
#if (CSW_EXTENDED_API_PMD_USB == 0)

//#warning Using USB and power management of MCI

VOID USB_PreClose(VOID)
{
    //pmd_UsbEnable(0); /// deprecated function, removed
    //pmd_EnablePower(PMD_POWER_USB,FALSE);

}

volatile BOOL gUsbDetected;
extern volatile BOOL g_doFsProcess;
volatile BOOL USBinitOK=0;
UINT8 g_UsbBootMode;

VOID MCI_USBInit(UINT8 mailBoxId)
{
    if (USBinitOK)
        return;
    gUsbDetected = 0;
    uctls_Open(mailBoxId, 0x0000, 0x0000, 0x0000, "USB Controller 1.0");
    USBinitOK = 1;
    //CFW_ShellControl(CFW_CONTROL_CMD_POWER_OFF);
    mmi_trace(1,"-------------------------------after uctls_Open, mmi_Mbox ID is %d-------------------------------------\n",mailBoxId);
    //uctls_ChargerStatus(UCTLS_CHARGER_STATUS_CONNECTED);
}

USB_DETECTED_STATUS MCI_USBDetect(VOID) //chenhe for jasperII,for  can't speach on when calling
{
    UINT32 ticks;
    BOOL result;
    if (checkUsbstate() || gUsbDetected)
    {
      return USB_DETECTED_OK;
    }
	while (!USBinitOK)
	{
       mmi_trace(1,"---------------------------wait for usb init---------------------------\n");
		COS_Sleep(100);
	}
    uctls_ChargerStatus(UCTLS_CHARGER_STATUS_CONNECTED);
    ticks =  hal_TimGetUpTime();
    while (!gUsbDetected && (ticks+(16384*6/10))>hal_TimGetUpTime())//use 600ms for detected
    {
        mmi_trace(1,"---------------------------USB decting---------------------------\n");
        if (g_doFsProcess)
        {
            uctls_ChargerStatus(UCTLS_CHARGER_STATUS_DISCONNECTED);
			  return USB_DETECTED_BUSY;
        }
        COS_Sleep(100);
    }
    mmi_trace(1,"MCI_USBDetect!!! ---- %d ----!!!!!!!!!!!!!!\n",gUsbDetected);
    result = gUsbDetected;
    //if (!result)
    {
        uctls_ChargerStatus(UCTLS_CHARGER_STATUS_DISCONNECTED);
    }
    return result;

}
BOOL USBIsIn(void)
{
	if(	USB_DETECTED_OK == MCI_USBDetect() )
		{
			return TRUE;
		}
	else
		{
			return FALSE;
		}
}
#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */


extern void PhnsetGetCurScrSvr(U16 *screenSaver);
extern void PhnsetRstScrSvrDefaultTmp(void);
extern void PhnsetScrSvrRestore(U16 SsID,BOOL IsRestore);


/* for fixing bug-12621 by liuxl on 20090623-Begin*/
extern void PhnsetGetCurWallPaper(U16 *wallPaper);
extern void PhnsetRstWallPaperDefaultTmp(void);
extern void PhnsetWallPaperRestore(U16 WpID,BOOL IsRestore);
extern BOOL IsPhnsetWallPaperFromExternal(U16 WpID, U8 Type);

static U16 screensaver_ID_backup;
static BOOL IsSSNeedRestore = FALSE;
static void usb_stop_screensaver()
{
	U16 SsID;
	PhnsetGetCurScrSvr(&SsID);
	screensaver_ID_backup = SsID;
	mmi_trace(g_sw_SETTING, "LXL-- %s entry %s function, File: %s, Line: %d,screensaver_ID_backup=%d", PRE_IDENTIFIER, __FUNCTION__, __FILE__, __LINE__,screensaver_ID_backup);
	

	if(IsPhnsetWallPaperFromExternal(screensaver_ID_backup, 1))//MMI_PHNSET_TYPE_SS
	{
		IsSSNeedRestore = TRUE;
		mmi_trace(g_sw_SETTING, "LXL-- %s entry %s function, File: %s, Line: %d,IsSSNeedRestore=%d", PRE_IDENTIFIER, __FUNCTION__, __FILE__, __LINE__,IsSSNeedRestore);	
		PhnsetRstScrSvrDefaultTmp();
	}
	else
	{
		IsSSNeedRestore = FALSE;
	}
}

void usb_restart_screensaver()
{
	mmi_trace(g_sw_SETTING, "LXL-- %s entry %s function, File: %s, Line: %d,IsSSNeedRestore=%d", PRE_IDENTIFIER, __FUNCTION__, __FILE__, __LINE__,IsSSNeedRestore);	
	PhnsetScrSvrRestore(screensaver_ID_backup,IsSSNeedRestore);
	IsSSNeedRestore = FALSE;
}

static U16 wallpaper_ID_backup =0;
static BOOL IsNeedRestore = FALSE;
/*save the file of T card which was wall paper once and set default file as current wall paper*/
static void usb_stop_wallpaper()
{
	U16 WpID;
	PhnsetGetCurWallPaper(&WpID);
	wallpaper_ID_backup = WpID;
	mmi_trace(g_sw_SETTING, "LXL-- %s entry %s function, File: %s, Line: %d,wallpaper_ID_backup=%d", PRE_IDENTIFIER, __FUNCTION__, __FILE__, __LINE__,wallpaper_ID_backup);
	if(IsPhnsetWallPaperFromExternal(wallpaper_ID_backup, 0))//MMI_PHNSET_TYPE_WP
	{
		IsNeedRestore = TRUE;
		mmi_trace(g_sw_SETTING, "LXL-- %s entry %s function, File: %s, Line: %d,IsNeedRestore=%d", PRE_IDENTIFIER, __FUNCTION__, __FILE__, __LINE__,IsNeedRestore);	
		PhnsetRstWallPaperDefaultTmp();
	}
	else
	{
		IsNeedRestore = FALSE;
	}
}
/*restore the file of T card which was wall paper once as current wall paper*/
void usb_restart_wallpaper()
{
	mmi_trace(g_sw_SETTING, "LXL-- %s entry %s function, File: %s, Line: %d,IsNeedRestore=%d", PRE_IDENTIFIER, __FUNCTION__, __FILE__, __LINE__,IsNeedRestore);	
	mmi_trace(g_sw_SETTING, "LXL-- %s entry %s function, File: %s, Line: %d,wallpaper_ID_backup=%d", PRE_IDENTIFIER, __FUNCTION__, __FILE__, __LINE__,wallpaper_ID_backup);
	if((wallpaper_ID_backup!=0)&&IsNeedRestore)
	{
            PhnsetWallPaperRestore(wallpaper_ID_backup,IsNeedRestore);
            IsNeedRestore = FALSE;
       }
}
/* for fixing bug-12621 by liuxl on 20090623-End*/
extern U16 GetRingToneID(void);
extern S16  SetRingToneID(U16 ringtoneid);
extern u16 GetAlarmToneType(void);
extern S16  SetAlarmToneID(U16 id);
extern U16 gstartMidiId;
static U16 incoming_call_ring_tone_setting_backup;
static U16 alam_tone_setting_backup;

#ifdef __MMI_DUAL_PROFILE_SUPPORT__ 
static U16 incoming2_call_ring_tone_setting_backup;
extern S16 SetCard2RingToneID(U16 ringtoneid);
extern U16 GetCard2RingToneID(void);
#endif

extern INT32 mmi_getCurrentPowerMode(void);
extern U8 isIdleScreenReached;

static void usb_tone_setup_save()
{
#ifdef __MMI_DUAL_PROFILE_SUPPORT__ 
      incoming2_call_ring_tone_setting_backup = GetCard2RingToneID();
#endif
    incoming_call_ring_tone_setting_backup = GetRingToneID();
    alam_tone_setting_backup = GetAlarmToneType();
    if((mmi_getCurrentPowerMode() != 4)&&(mmi_getCurrentPowerMode() != 2)&&(isIdleScreenReached == 1 )) //POWER_ON_USB/CHARGER == 4
    {
        SetRingToneID(gstartMidiId);
        SetAlarmToneID(gstartMidiId+1);
#ifdef __MMI_DUAL_PROFILE_SUPPORT__ 
        SetCard2RingToneID(gstartMidiId);
#endif
    }
}

static void usb_tone_setup_restore()
{
	SetRingToneID(incoming_call_ring_tone_setting_backup);
	SetAlarmToneID(alam_tone_setting_backup);
#ifdef __MMI_DUAL_PROFILE_SUPPORT__ 
        SetCard2RingToneID(incoming2_call_ring_tone_setting_backup);
#endif
}

extern BOOL g_hasMountedTFlashFS;
extern BOOL g_hasMountedFlashFS;
extern BOOL g_UsbChargerPowerOn;

#if defined(DUAL_TFLASH_SUPPORT)
extern BOOL g_hasMountedTFlash2FS;
#endif

void InitSoundRecorderApp(void);
void Usb_stop_application(void)
{
	usb_stop_screensaver();
	usb_tone_setup_save();
	/*save the file of T card which was wall paper once and set default file as current wall paper*/
	usb_stop_wallpaper();
}
	
void Usb_resume_application(void)
{
	usb_tone_setup_restore();
	usb_restart_screensaver();
	/*restore the file of T card which was wall paper once as current wall paper*/
	usb_restart_wallpaper();
}

 extern U16 AnsiiToUnicodeString(S8 *pOutBuffer, S8 *pInBuffer );
 UINT32 MCI_USBConnect(VOID) //chenhe for jasperII,for  can't speach on when calling
{
	UINT8 pMmc0DevName[] = {'/',0,'T',0,0,0};
#if defined(DUAL_TFLASH_SUPPORT)
       UINT8 pMmc1DevName[] = {'/',0,'T',0,'1',0,0,0};
#endif
	INT32 iResult;
	
	char* asciiHome = "/home";
	char uniHome[20] = {0,};
       
    
	mmi_trace(1,"---------------------------USB Connect!\n");

        AnsiiToUnicodeString(uniHome, asciiHome);
        iResult = FS_Unmount(uniHome, TRUE);
        if( iResult != ERR_SUCCESS )
        {
            mmi_trace(1,"Failed to unmount VDS1!\n");
        }
        else
        {
            g_hasMountedFlashFS = FALSE;
            mmi_trace(1,"Unmount VDS1 OK!\n");
        }
	
	if ( g_hasMountedTFlashFS )
	{
		iResult = FS_Unmount(pMmc0DevName, TRUE);
		if(ERR_SUCCESS == iResult)
		{
			mmi_trace(1,"UnMount MMC0 OK!\n");
			g_hasMountedTFlashFS = FALSE;
		}
		else
		{
			mmi_trace(1,"UnMount MMC0 failed!error code = %d.\n",iResult); 
		}
	}

#if defined(DUAL_TFLASH_SUPPORT)
     if ( g_hasMountedTFlash2FS )
	{
		iResult = FS_Unmount(pMmc1DevName, TRUE);
		if(ERR_SUCCESS == iResult)
		{
			mmi_trace(1,"UnMount MMC1 OK!\n");
			g_hasMountedTFlash2FS = FALSE;
		}
		else
		{
			mmi_trace(1,"UnMount MMC1 failed!error code = %d.\n",iResult); 
		}
	}
#endif

#if (CSW_EXTENDED_API_PMD_USB == 0)
	uctls_SetMode(UCTLS_ID_1);
	uctls_ChargerStatus(UCTLS_CHARGER_STATUS_CONNECTED);
#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */
#ifdef __MMI_EBOOK_READER__
	mmi_ebook_usb_mode_on(); //chenhe add
#endif
	return 1;
}

UINT32 MCI_USBDisconnect(VOID) //chenhe for jasperII,for  can't speach on when calling
{
	UINT8 pMmcDevName[] = {'/',0,'T',0,0,0};
	INT32 iResult;
       FS_INFO  sFSInfo;
#if defined(DUAL_TFLASH_SUPPORT)
       UINT8 pMmc1DevName[] = {'/',0,'T',0,'1',0,0,0};
#endif

    
	mmi_trace(1,"---------------------------USB DisConnect!\n");
#ifdef __MMI_EBOOK_READER__
	mmi_ebook_usb_mode_off();//chenhe add
#endif	
#if (CSW_EXTENDED_API_PMD_USB == 0)
	uctls_ChargerStatus(UCTLS_CHARGER_STATUS_DISCONNECTED);
#endif /* (CSW_EXTENDED_API_PMD_USB == 0) */

#if 1
	char* asciiHome = "/home";
	char uniHome[20] = {0,};
	
	AnsiiToUnicodeString(uniHome, asciiHome);
	if( ERR_SUCCESS == FS_Mount("VDS1", uniHome, 0, FS_TYPE_FAT))
	{
	       g_hasMountedFlashFS = TRUE; 
		mmi_trace(1,"Mount VDS1 OK!\n");
	}
	else
	{
		mmi_trace(1,"Failed to mount VDS1!\n");
	}
#endif
	
	if(g_hasMountedTFlashFS == FALSE)
	{
		iResult = FS_Mount("MMC0",pMmcDevName, 0,FS_TYPE_FAT);
		if(ERR_SUCCESS == iResult)
		{
			mmi_trace(1,"Mount MMC0 OK!\n");
			g_hasMountedTFlashFS = TRUE;
			FS_GetFSInfo("MMC0",&sFSInfo);	
		}
		else
		{
			mmi_trace(1,"Mount MMC0 failed!error code = %d.\n",iResult); 
		}
              
	}
 #if defined(DUAL_TFLASH_SUPPORT)
       if(g_hasMountedTFlash2FS == FALSE)
	{
		iResult = FS_Mount("MMC1",pMmc1DevName, 0,FS_TYPE_FAT);
		if(ERR_SUCCESS == iResult)
		{
			mmi_trace(1,"Mount MMC1 OK!\n");
			g_hasMountedTFlash2FS = TRUE;
			FS_GetFSInfo("MMC1",&sFSInfo);	
		}
		else
		{
			mmi_trace(1,"Mount MMC1 failed!error code = %d.\n",iResult); 
		}
              
	}
 #endif
	usb_tone_setup_restore();
	usb_restart_screensaver();
	/*restore the file of T card which was wall paper once as current wall paper*/
	usb_restart_wallpaper();
	#ifdef __MMI_SOUND_RECORDER__
	if (g_UsbChargerPowerOn)
		InitSoundRecorderApp();
	#endif
            
	return 1;
}


