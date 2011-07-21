#ifdef __ZYX_PLATFORM_SUPPORT__

#include "zyxconfig.h"
#include "zyxsys.h"
#include "zyxstring.h"
#include "zyxmisc.h"
#include "zyxextern.h"

extern S8  gstrIMEI[];
extern S8 gHomePLMNNum[];
#ifdef __MMI_DUAL_SIM__
extern S8 gHomePLMNNum_2[];
#endif
extern void PhnsetWpUsrDefFreeBuf(void);
extern S8 *phnset_strp ;  
extern void adp_start_get_cell_info(void);
extern void adp_end_get_cell_info(void);

static char Zyx_scNumber[24] = {0};
#ifdef __MMI_DUAL_SIM__
static char Zyx_scNumber2[24] = {0};
#endif

void ZyxGetUserinfo(ZyxUserInfoPtr userinfo)
{
    memset(userinfo,0,sizeof(ZyxUserInfo));
    
    strcpy(userinfo->UpdateSerial,UPDATE_SERIAL);
    strcpy(userinfo->MidVer,MIDWARE_VERSION);
    strcpy(userinfo->factory,FACTORY_ID);
    strcpy(userinfo->modelid,MODEL_ID);

    if (ZyxUsableSim == CFW_SIM_1)
    {
        strcpy(userinfo->scnumber,Zyx_scNumber);
    }
#ifdef __MMI_DUAL_SIM__
    else
    {
        strcpy(userinfo->scnumber,Zyx_scNumber2);
    }
#endif    
}

char* ZyxGetImei(void)
{
#ifdef __MMI_DUAL_SIM__
      int sim = ZyxUsableSim;

      if((sim <0)||(sim >=MMI_SIM_MAX) )
	  return NULL;
	return (char *)&gstrIMEI[sim];
#else
       return (char *)gstrIMEI;
#endif
}

char* ZyxGetImsi(void)
{
#ifdef __MMI_DUAL_SIM__
      int sim = ZyxUsableSim;

      if((sim <0)||(sim >=MMI_SIM_MAX) )
	  return NULL;
      if (sim == MMI_SIM_2)
	return (char *)gHomePLMNNum_2;
#endif
       return (char *)gHomePLMNNum;
}

#include "Adp_events.h"
extern adp_sms_context_t *adp_pSMSContext;
char* ZyxGetSc(void)
{
   CFW_SMS_PARAMETER smsInfo; 
#ifdef __MMI_DUAL_SIM__ 
        int sim = ZyxUsableSim;
        if (ERR_SUCCESS == CFW_CfgGetSmsParam(&smsInfo, 0, sim)) 
#else 
        if (ERR_SUCCESS == CFW_CfgGetSmsParam(&smsInfo, 0)) 
#endif 
    { 
        if(smsInfo.nNumber[0] == 0) 
        { 
           return NULL; 
        } 

       if (smsInfo.nNumber[0] != 0) 
        { 
#ifdef __MMI_DUAL_SIM__ 
               if (sim == 2)
                {
                    SUL_GsmBcdToAscii((UINT8 *)&(smsInfo.nNumber[2]), smsInfo.nNumber[0] -1, Zyx_scNumber2); 
                    return Zyx_scNumber2; 
                }
               else
#endif         
            {
              SUL_GsmBcdToAscii((UINT8 *)&(smsInfo.nNumber[2]), smsInfo.nNumber[0] -1, Zyx_scNumber); 
               return Zyx_scNumber; 
            }
        }//wangwb @2007.06.16 

    } 

        return NULL; 
}    

void ZyxEnableBacklight(void)
{
    //SetBacklightModeExe(TRUE);
    TurnOnBacklight(0);
	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);
	mdi_audio_suspend_background_play();
}

void ZyxDisableBacklight(void)
{
    //SetBacklightModeExe(FALSE);
    TurnOffBacklight();
	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_ENABLED); 
	mdi_audio_resume_background_play();
}

void ZyxSetWallPaperExit(int timer_id)
{
   PhnsetWpUsrDefFreeBuf();
}

extern void PhnsetSetWallpaperByFileName( S8 *path );
void ZyxSetWallPaper(char* file_name)
{
#if 1
    U16 UFileName[128] = {0};
    if(file_name)
    {
        if (phnset_strp == NULL)
             phnset_strp = ( S8*)OslMalloc(FMGR_PATH_BUFFER_SIZE);

         if (file_name[1]) //ascii
        {
            ZyxUTF8ToUCS(file_name,UFileName,128);
            ZyxUCSStrcpy((U16*)phnset_strp,UFileName);
        }
        else
        {
            ZyxUCSStrcpy((U16*)phnset_strp,(U16*)file_name);
        }
       
           PhnsetSetWallpaperByFileName((S8*)phnset_strp);
             DisplayPopup(
            (PU8) GetString(STR_GLOBAL_DONE),
            IMG_GLOBAL_ACTIVATED,
            1,
            UI_POPUP_NOTIFYDURATION_TIME,
            SUCCESS_TONE);
        }
       // ZyxStartTimer(10,ZyxSetWallPaperExit);
		PhnsetWpUsrDefFreeBuf();
        #endif
}

void ZyxSetPhoneTone(char* file_name)
{
    PmgSetShortFileName( 0); // let PMGR know whether it's short filename
    PmgFmgrAddExtMelodyFunc(file_name);
}

static ZyxCellInfoCb CellInfoCbBak = NULL;
void ZyxUpdateCellInfoStart()
{
    CFW_TSM_CURR_CELL_INFO pCurrCellInf = {0,};
    CFW_TSM_ALL_NEBCELL_INFO pNeighborCellInfo ={0,};
#ifdef __MMI_DUAL_SIM__
    CFW_GetCellInfo(&pCurrCellInf,&pNeighborCellInfo, 0);
#else
    CFW_GetCellInfo(&pCurrCellInf,&pNeighborCellInfo);
#endif
}

void ZyxGetCellInfo(ZyxCellInfoCb cb)
{
    adp_start_get_cell_info();
      SetProtocolEventHandler(ZyxUpdateCellInfoStart,PRT_MSG_ID_MMI_GET_CELL_INFO_RSP_IND);
      CellInfoCbBak = cb;
}

void ZyxStopGetCell ()
{
  adp_end_get_cell_info();
}

U32 INT_SwitchStackToRun(void *stack_start, U32 stack_size, stack_func_ptr func, U32 argc, ...)
{
    return 0;
}
#ifdef WIN32
void ZyxTcmAddrAndSize(PU8 *addr,U32 *size)
{
    return;
}
#else
void ZyxTcmAddrAndSize(PU8 *addr,U32 *size)
{
    extern UINT32 _sys_sram_overlay_start,_sys_sram_overlay_start,_sys_sram_overlay_end;
    
   addr =(PU8*)_sys_sram_overlay_start;
   *size = _sys_sram_overlay_end-_sys_sram_overlay_start;
}
#endif

#endif
