#include <cswtype.h>
#include <errorcode.h>
#include "api_msg.h"
#include <cfw.h>
#include <base.h>
#include <cfw_prv.h>
#include <event.h>
#include <sul.h>
#include <cos.h>
#include <ts.h>

#include "cfw_emod_tsm.h"
#include <drv_prv.h>
#include <dm.h>
#include <pm.h>

#include "chip_id.h"

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
#include "hal.h"
#include "target.h"
#include "hal_calib.h"
#include "lib_version.h"
#else
#include "tgt_m.h"
#include "memd_m.h"
#include "aud_m.h"
#include "csw_csp.h"
#include "calib_m.h"
#endif

#ifndef RFM_FLASH_WDB
#define RFM_FLASH_WDB    0x02
#endif


factory_settings_t* factory_settings_p = NULL;

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
extern struct hal_AudioCfg audio_cfg;
#else
extern HAL_AIF_STREAM_T audioStream;
extern AUD_LEVEL_T        audio_cfg;
extern AUD_ITF_T        audioItf;
#endif

static  UINT8 AudioMod;

//BOOL FS_Format(const unsigned char *image_name);




BOOL CFW_EmodGetBatteryInfo(CFW_EMOD_BATTERY_INFO* pBatInfo )
{
	UINT32 result=0x00000000;
	SUL_MemSet8(pBatInfo,0xff, SIZEOF(CFW_EMOD_BATTERY_INFO));
	result=PM_GetBatteryState((PM_BATTERY_INFO*)(&pBatInfo->nBatState));
	if(result)
	{
		result=PM_GetBatteryInfo((UINT8*)(&pBatInfo->nChargeInfo.nBcs),(UINT8*)(&pBatInfo->nChargeInfo.nBcl),(UINT16*)(&pBatInfo->nChargeInfo.nMpc));
		if(result)
		{
			result=PM_BatteryChemistry((UINT8*)&pBatInfo->nChemistry);
			//if(result)
		}
		else
			return result;
	}
	else
		return result;
    return TRUE;
}


void CFW_EmodAudioTestStart(UINT8 type)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
	switch(audio_cfg.micSel)
	{
		case HAL_AUDIO_MIC:
			AudioMod=DM_AUDIO_MODE_EARPIECE;
			break;
		case HAL_AUDIO_MIC_AUX:
			AudioMod=DM_AUDIO_MODE_HANDSET;
			break;
		case HAL_AUDIO_MIC_LOUD:
			AudioMod=DM_AUDIO_MODE_LOUDSPEAKER;
			break;
	}
#else
    audioStream.startAddress = NULL;
    audioStream.length = 0;
    audioStream.sampleRate = HAL_AIF_FREQ_8000HZ;
    audioStream.channelNb = HAL_AIF_MONO;
    audioStream.voiceQuality = TRUE;
    audioStream.halfHandler = NULL;
    audioStream.endHandler = NULL;

	switch(audioItf)
	{
		case AUD_ITF_RECEIVER:
			AudioMod=DM_AUDIO_MODE_EARPIECE;
			break;
		case AUD_ITF_EAR_PIECE:
			AudioMod=DM_AUDIO_MODE_HANDSET;
			break;
		case AUD_ITF_LOUD_SPEAKER:
			AudioMod=DM_AUDIO_MODE_LOUDSPEAKER;
			break;
        default:
			AudioMod=DM_AUDIO_MODE_EARPIECE;
            break;
	}
#endif

	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_EMOD_AudioTestStart()  AudioMod=%d!\n"),AudioMod);	
	switch(type)
	{
		case 0:
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
			audio_cfg.micSel=HAL_AUDIO_MIC;
			audio_cfg.micGain=HAL_AUDIO_MIC_45dB;
			audio_cfg.spkGain=HAL_AUDIO_SPK_3dB;
			audio_cfg.spkSel=HAL_AUDIO_SPK;
			audio_cfg.sideGain=HAL_AUDIO_SIDE_6dB;
#else
			audioItf = AUD_ITF_RECEIVER;
			audio_cfg.micLevel=AUD_MIC_ENABLE;
			audio_cfg.spkLevel=AUD_SPK_VOL_7;                  //wangrui modify for fixbug10475 20081202
			audio_cfg.sideLevel=AUD_SIDE_VOL_TEST;          //wangrui modify for fixbug10475 20081202
#endif
			break;

		case 1:
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
			audio_cfg.micSel=HAL_AUDIO_MIC_AUX;
			audio_cfg.micGain=HAL_AUDIO_MIC_45dB;
			audio_cfg.spkGain=HAL_AUDIO_SPK_3dB;
			audio_cfg.spkSel=HAL_AUDIO_SPK_AUX;
			audio_cfg.sideGain=HAL_AUDIO_SIDE_6dB;
#else
			audioItf = AUD_ITF_EAR_PIECE;
			audio_cfg.micLevel=AUD_MIC_ENABLE;
			audio_cfg.spkLevel=AUD_SPK_VOL_7;                  //wangrui modify for fixbug10475 20081202
			audio_cfg.sideLevel=AUD_SIDE_VOL_TEST;                  //wangrui modify for fixbug10475 20081202
#endif
			break;

		case 2:
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
			audio_cfg.micGain=HAL_AUDIO_MIC_45dB;
			audio_cfg.spkGain=HAL_AUDIO_SPK_3dB;
			audio_cfg.sideGain=HAL_AUDIO_SIDE_6dB;
			audio_cfg.micSel=HAL_AUDIO_MIC_LOUD;
			audio_cfg.spkSel=HAL_AUDIO_SPK_LOUD;
#else
			audioItf = AUD_ITF_LOUD_SPEAKER;
			audio_cfg.micLevel=AUD_MIC_ENABLE;
			audio_cfg.spkLevel=AUD_SPK_VOL_4;
			audio_cfg.sideLevel=AUD_SIDE_VOL_6;
#endif
			break;

		default:
			break;		
	}
	
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
	while (!hal_audio_StopStream());
	while (!hal_audio_StopRecord());
 	while (!hal_audio_StopSpeech());
	hal_audio_MicOpen();
    hal_audio_SpeakerOpen();
    hal_audio_SideTest(TRUE, &audio_cfg);
#else
    aud_TestModeSetup(audioItf, &audioStream, &audio_cfg, AUD_TEST_SIDE_TEST);
#endif
	
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_EMOD_AudioTestStart()  success!\n"));
	
}

void CFW_EmodAudioTestEnd(void)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
	audio_cfg.sideGain=HAL_AUDIO_SIDE_MUTE;
	while (!hal_audio_StopStream());
	while (!hal_audio_StopRecord());
 	while (!hal_audio_StopSpeech());
	hal_audio_SideTest(FALSE, &audio_cfg);
    hal_audio_MicClose();       
    hal_audio_SpeakerClose();   
#else
    audio_cfg.sideLevel = AUD_SIDE_MUTE;
    aud_TestModeSetup(audioItf, &audioStream, &audio_cfg, AUD_TEST_NO);
#endif
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_EMOD_AudioTestEnd()  AudioMod=%d!\n"),AudioMod);	
//	while(!DM_SetAudioMode(AudioMod));
//	The return value of this function is always true. it is not necessary to call it in a loop;
	DM_SetAudioMode(AudioMod);
    CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_EMOD_AudioTestEnd()  success!\n"));
}

BOOL CFW_EmodClearUserInfo(void)
{

    // Todo
    return FALSE;

}

#ifdef CFW_MULTI_SIM
void CFW_EmodGetIMEI(UINT8* pImei,UINT8* pImeiLen,CFW_SIM_ID nSimID)
#else
void CFW_EmodGetIMEI(UINT8* pImei,UINT8* pImeiLen)
#endif
{
	UINT8 nChar[9];
	UINT8 i;
#ifdef CFW_MULTI_SIM	
	if( nSimID >=CFW_SIM_COUNT)
	{
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("ERROR ! SIM id error CFW_EmodGetIMEI()\r\n"));	
		return ;
	}
#endif
	factory_settings_p=(factory_settings_t*)&_factory_start;
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" valid_marker=%x,IMEISV[0]=%x,IMEISV[1]=%x,MB_SN=%x,MP_SN=%x.\n"),
		&factory_settings_p->valid_marker,factory_settings_p->IMEISV[0],factory_settings_p->IMEISV[1],factory_settings_p->MB_SN,factory_settings_p->MP_SN);	
#ifdef CFW_MULTI_SIM	
	SUL_MemCopy8(nChar,factory_settings_p->IMEISV[nSimID], 9);
#else
	SUL_MemCopy8(nChar,factory_settings_p->IMEISV[0], 9);
#endif
	pImei[0]=((nChar[0]&0xf0)>>4)+0x30;	
	for(i=1;i<8;i++)
	{
		pImei[2*i-1]=(nChar[i]&0x0f)+0x30;
		pImei[2*i]=((nChar[i]&0xf0)>>4)+0x30;
	}
	*pImeiLen=15;

	return ;
}

void CFW_EmodReadBindSIM(CFW_EMOD_READ_BIND_SIM* pReadBindSim)
{
	
	UINT8 	Restora[BIND_INFO_SIZE];
	UINT8 i=0;
	UINT8 nNum=0;
	UINT8 nCounter=0;
	UINT32 result;
	BIND_SIM_NM* temp=NULL;
	CFW_EMOD_BIND_SIM *pBindSim=NULL;
	
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_EmodReadBindSIM()  1!\n"));
	temp=(BIND_SIM_NM*)CSW_SIM_MALLOC(SIZEOF(BIND_SIM_NM));
	result= Cfg_Emod_ReadSIMNm(temp);
	SUL_MemCopy8(Restora+1, temp->simNo, 90);
	CSW_SIM_FREE(temp);
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_EmodReadBindSIM()  2!\n"));
	#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    if(result==HAL_FLASH_OK)
#else
    if(result==MEMD_ERR_NO)
#endif
	{
		for(i=0;i<10;i++)
		{
			if(Restora[BIND_SIMNO+i*9]!=0xff)
			{
				nNum++;
			}
		}
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_EmodReadBindSIM()  number=%d!\n"),nNum);
		if(nNum==0)	
			pReadBindSim->nNumber=0;
		else
		{
			pBindSim=(CFW_EMOD_BIND_SIM*)CSW_TMS_MALLOC(nNum*(SIZEOF(CFW_EMOD_BIND_SIM)));
			for(i=0;i<10;i++)
			{
				if(Restora[BIND_SIMNO+i*9]!=0xff)
				{
					Emod_IMSItoASC(Restora+BIND_SIMNO+i*9, pBindSim->pImsiNum,&pBindSim->nImsiLen);
					pBindSim->nIndex=i+1;
					nCounter++;
					pBindSim=(CFW_EMOD_BIND_SIM*)(&pBindSim->nIndex+20);
				}
			}
		}
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_EmodReadBindSIM()  3!\n"));
		pReadBindSim->nNumber=nNum;
		pReadBindSim->nPointer=(UINT32)&pBindSim->nIndex-20*nCounter;
	}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_EmodReadBindSIM()  4!\n"));
	
}

void CFW_EmodReadBindNW(CFW_EMOD_READ_BIND_NW* pReadBindNw)
{
	UINT8 	Restora[BIND_INFO_SIZE];
	UINT8 i=0;
	UINT8 nNum=0;
	UINT8 nCounter=0;
	UINT32 result;
	BIND_NW_NM* temp=NULL;
	CFW_EMOD_BIND_NW *pBindNw=NULL;

	
	temp=(BIND_NW_NM*)CSW_SIM_MALLOC(SIZEOF(BIND_NW_NM));
	result= Cfg_Emod_ReadNWNm(temp);
	SUL_MemCopy8(Restora+92, temp->nwNo, 15);
	CSW_SIM_FREE(temp);
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    if(result==HAL_FLASH_OK)
#else
    if(result==MEMD_ERR_NO)
#endif
	{
		for(i=0;i<5;i++)
		{
			if(Restora[BIND_NWNO+i*3]!=0xff)
			{
				nNum++;
			}
		}
		if(nNum==0)	
			pReadBindNw->nNumber=0;
		else
		{
			pBindNw=(CFW_EMOD_BIND_NW*)CSW_TMS_MALLOC(nNum*(SIZEOF(CFW_EMOD_BIND_NW)));
			for(i=0;i<5;i++)
			{
				if(Restora[BIND_NWNO+i*3]!=0xff)
				{
					SUL_MemCopy8(pBindNw->pNccNum,Restora+BIND_NWNO+i*3, 3);
					pBindNw->nIndex=i+1;
					nCounter++;
					pBindNw=(CFW_EMOD_BIND_NW*)(&pBindNw->nIndex+4);
				}
			}
		}
		pReadBindNw->nNumber=nNum;
		pReadBindNw->nPointer=(UINT32)&pBindNw->nIndex-4*nCounter;
	}
}

UINT32 CFW_EmodSetBindInfo(BOOL bSim,BOOL bNetWork)
{
	
	UINT32 ret;
	ret=Cfg_Emod_UpdateSign(bSim, bNetWork);
	if(ret==ERR_SUCCESS)
	{
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("success=%x.\n"),ret);
		return(ERR_SUCCESS);
	}
	else
	{
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("failure=%x..\n"),ret);
		return ERR_CMS_TEMPORARY_FAILURE;
	}
		
	
}

void CFW_EmodGetBindInfo(BOOL* bSim,BOOL* bNetWork)
{
	
	if (ERR_SUCCESS==Cfg_Emod_ReadSign(bSim,bNetWork))
	{
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_EMOD_GetBindInfo sucessful.\n"));
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("CFW_EMOD_GetBindInfo sucessful and the bsim=%d,bNetWork=%d.\n"),*bSim,*bNetWork);
	}
}

//*********************************************************************************
//discreption: this function used to bind sim to this phone
//---------------------------------------------------------------------------------
//params:	nIndex[in], 		when pBindSimNum == null, range:1~10, delete an exist one
//							when pBindSimNum != null,  0 means find a free location to save
//													range1~10, means updata an exist one.
//							can bind 10 sim cards totally.
//---------------------------------------------------------------------------------
//			pBindSimNum[in], 	value of sim number, if this pointer is null, function will init this sim 
//				number field according to the index. otherwise we will updata the sim number field.
//------------------------------------------------------------------------------------
//			nLen[in], 			length of pBindSimNum. range 0~15
//*********************************************************************************
CFW_EMOD_UPDATE_RETURN CFW_EmodUpdateSimBind(UINT8 nIndex,UINT8* pBindSimNum,UINT8 nLen)
{
	CFW_EMOD_UPDATE_RETURN nReturn;
	UINT32 j;
	UINT8   nIMSITemp[9];
	UINT8 simnm[90];
	nReturn.nErrCode=0;
	nReturn.nIndex=0;
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("@@@@@@@@@@@11.\n"));

	if(pBindSimNum==NULL)
	{
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("@@@@@@@@@@@delete start .\n"));
		if((nIndex<1)||(nIndex>10))
		{
			nReturn.nErrCode=ERR_INVALID_INDEX;
			return nReturn;
		}
		else
		{
			Cfg_Emod_ReadSIMNm((BIND_SIM_NM*)simnm);
			SUL_MemSet8(simnm+(nIndex-1)*9, 0xFF, 9);
	
			if(ERR_SUCCESS!=Cfg_Emod_UpdateSIMNm((BIND_SIM_NM*)simnm))
			{
				nReturn.nErrCode=ERR_CMS_TEMPORARY_FAILURE;
				return nReturn;
			}
		}
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("@@@@@@@@@@@delete end .\n"));
	}
	else
	{
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("@@@@@@@@@@@12.\n"));
		if(nLen>15)
		{
			nReturn.nErrCode=ERR_CFW_INVALID_PARAMETER;
			return nReturn;
		}
			
		Emod_ASCtoIMSI(pBindSimNum, nIMSITemp, nLen);
		 
		if( ERR_SUCCESS!=Cfg_Emod_ReadSIMNm((BIND_SIM_NM*)simnm))
		{
			nReturn.nErrCode=ERR_CMS_TEMPORARY_FAILURE;
			return nReturn;
		}
		
		if(nIndex==0)
		{
			j=0;
			while((simnm[j]!=0xff)&&(nReturn.nIndex<10))
			{
				j+=9;
				nReturn.nIndex++;
			}
			if(nReturn.nIndex<10)
			{
				SUL_MemCopy8(simnm+9*nReturn.nIndex, nIMSITemp, 9);
				nReturn.nIndex++;//hameina[+] 2007.7.26, output index range from 1~10
				
			}
			else if(nReturn.nIndex==10)
			{
				UINT8 temp[81];
				SUL_MemCopy8(temp, simnm+9, 81);
				SUL_MemCopy8(simnm,temp, 81);
				SUL_MemCopy8(simnm+81,nIMSITemp, 9);
				
				nReturn.nIndex=10;
			}
				
		}
		else
		{
			if((nIndex<1)||((nIndex>10)))
			{
				nReturn.nErrCode=ERR_INVALID_INDEX;
				return nReturn;
			}
			else
			{
				SUL_MemCopy8(simnm+9*(nIndex-1),nIMSITemp, 9);
				nReturn.nIndex=nIndex;
				
			}
		}
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("@@@@@@@@@@@13.\n"));
		if(ERR_SUCCESS!=Cfg_Emod_UpdateSIMNm((BIND_SIM_NM*)simnm))
		{
			nReturn.nErrCode=ERR_CMS_TEMPORARY_FAILURE;
			return nReturn;
		}
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("@@@@@@@@@@@14.\n"));
	}
	return nReturn;
	
}

CFW_EMOD_UPDATE_RETURN CFW_EmodUpdateNwBind(UINT8 nIndex,UINT8* pBindNwNum,UINT8 nLen)
{
	UINT32 j;
	CFW_EMOD_UPDATE_RETURN nReturn;
	UINT8 nwnm[15];
	nReturn.nErrCode=0;
	nReturn.nIndex=0;
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("@@@@@@@@@@@11.\n"));
	if(pBindNwNum==NULL)
	{
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("@@@@@@@@@@@delete start .\n"));
		if((nIndex<1)||(nIndex>5))
		{
			nReturn.nErrCode=ERR_INVALID_INDEX;
			return nReturn;
		}
		else
		{
			Cfg_Emod_ReadNWNm((BIND_NW_NM*)nwnm);
			SUL_MemSet8(nwnm+(nIndex-1)*3, 0xFF, 3);
	
			if(ERR_SUCCESS!=Cfg_Emod_UpdateNWNm((BIND_NW_NM*)nwnm))
			{
				nReturn.nErrCode=ERR_CMS_TEMPORARY_FAILURE;
				return nReturn;
			}
			
		}
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("@@@@@@@@@@@delete end .\n"));
	}
	else
	{
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("@@@@@@@@@@@12.\n"));
		if(nLen>3)
		{
			nReturn.nErrCode=ERR_CFW_INVALID_PARAMETER;
			return nReturn;
		}
					 
		if( ERR_SUCCESS!=Cfg_Emod_ReadNWNm((BIND_NW_NM*)nwnm))
		{
			nReturn.nErrCode=ERR_CMS_TEMPORARY_FAILURE;
			return nReturn;
		}
		
		if(nIndex==0)
		{
			j=0;
			while((nwnm[j]!=0xff)&&(nReturn.nIndex<5))
			{
				j+=3;
				nReturn.nIndex++;
			}
			if(nReturn.nIndex<5)
			{
				SUL_MemCopy8(nwnm+3*nReturn.nIndex, pBindNwNum, 3);
				nReturn.nIndex+=1;//qiao+07.11.1 return logic index , which is increased one base on physical index 
				
			}
			else if(nReturn.nIndex==5)
			{
				UINT8 temp[12];
				SUL_MemCopy8(temp, nwnm+3, 12);
				SUL_MemCopy8(nwnm,temp, 12);
				SUL_MemCopy8(nwnm+12,pBindNwNum, 3);
				
				nReturn.nIndex=5;
			}
				
		}
		else
		{
			if((nIndex<1)||((nIndex>5)))
			{
				nReturn.nErrCode=ERR_INVALID_INDEX;
				return nReturn;
			}
			else
			{
				SUL_MemCopy8(nwnm+3*(nIndex-1),pBindNwNum,nLen);
				nReturn.nIndex=nIndex;
				
			}
		}
		CSW_TRACE(CFW_SIM_TS_ID, TSTXT("@@@@@@@@@@@13.\n"));
		if(ERR_SUCCESS!=Cfg_Emod_UpdateNWNm((BIND_NW_NM*)nwnm))
		{
			nReturn.nErrCode=ERR_CMS_TEMPORARY_FAILURE;
			return nReturn;
		}
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT("@@@@@@@@@@@14.\n"));
	}
	return nReturn;
}

void CFW_EmodGetProductInfo(CFW_EMOD_LOCT_PRODUCT_INFO* pProduct)
{
	factory_settings_p=(factory_settings_t*)&_factory_start;
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" ############valid_marker=%x,IMEISV=%x,MB_SN=%x,MP_SN=%x.\n"),
		&factory_settings_p->valid_marker,factory_settings_p->IMEISV[0],factory_settings_p->MB_SN,factory_settings_p->MP_SN);
	#if 0
	while(i<64)
	{
		if(factory_settings_p->MB_SN[i]==0xff)
		{
			pProduct->nBoardSNLen=i;
			SUL_MemCopy8(pProduct->nBoardSN, factory_settings_p->MB_SN, i);
			i=64;
			
		}
		i++;
		
	}
	
	while(j<64)
	{
		if(factory_settings_p->MP_SN[j]==0xff)
		{
			pProduct->nPhoneSNLen=j;
			SUL_MemCopy8(pProduct->nPhoneSN, factory_settings_p->MP_SN, j);
			j=64;
			
		}
		j++;
	}
	#endif
	pProduct->nBoardSNLen=factory_settings_p->MB_SN[0]-1;
	SUL_MemCopy8(pProduct->nBoardSN, factory_settings_p->MB_SN+1, factory_settings_p->MB_SN[0]-1);
	pProduct->nPhoneSNLen=factory_settings_p->MP_SN[0]-1;
	SUL_MemCopy8(pProduct->nPhoneSN, factory_settings_p->MP_SN+1, factory_settings_p->MP_SN[0]-1);
}


void CFW_EmodGetRFCalibrationInfo(CFW_EMOD_RF_CALIB_INFO *pRfCalib)
{
	UINT32 size;
	CT_calibration_t *calib_p=NULL;
#if (CHIP_ASIC_ID == CHIP_ASIC_JADE)
	calib_p=(CT_calibration_t*)&_calib_start;
#else
    calib_p = calib_GetPointers();
#endif
	size=SIZEOF(CT_calib_pcl2dbm_arfcn_g_t);
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" ############CT_calib_pcl2dbm_arfcn_g_t LEN=%d.\n"),size);
	size=SIZEOF(CT_calib_rf_chip_name_t);
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" ############CT_calib_rf_chip_name_t LEN=%d.\n"),size);
	size=SIZEOF(CT_calib_padac_profile_interp_t);
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" ############CT_calib_padac_profile_interp_t LEN=%d.\n"),size);
	size=SIZEOF(CT_calib_padac_profile_t);
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" ############CT_calib_padac_profile_t LEN=%d.\n"),size);
	size=SIZEOF(CT_calib_padac_profile_extrem_t);
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" ############CT_calib_padac_profile_extrem_t LEN=%d.\n"),size);
	size=SIZEOF(CT_calib_padac_ramp_t);
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" ############CT_calib_padac_ramp_t LEN=%d.\n"),size);
	size=SIZEOF(CT_calib_padac_ramp_swap_t);
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" ############CT_calib_padac_ramp_swap_t LEN=%d.\n"),size);
	size=SIZEOF(CT_calib_padac_low_volt_t);
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" ############CT_calib_padac_low_volt_t LEN=%d.\n"),size);
	size=SIZEOF(CT_calib_padac_low_dac_t);
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" ############CT_calib_padac_low_dac_t LEN=%d.\n"),size);
	size=SIZEOF(CT_calib_pa_t);
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" ############CT_calib_pa_t LEN=%d.\n"),size);
	
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" ############CT_calibration=%x.\n"),&calib_p->code_version);	
	
	SUL_MemCopy8(pRfCalib->nLowDacLimit,calib_p->pa->palcust.low_dac_limit, 12);

	SUL_MemCopy8(pRfCalib->nLowVoltLimit,calib_p->pa->palcust.low_volt_limit, 12);

	SUL_MemCopy8(&pRfCalib->nName,&calib_p->pa->palcust.name, 4);

	SUL_MemCopy8(pRfCalib->nPcl2dbmArfcn_d,calib_p->pa->palcust.pcl2dbm_arfcn_d, 60);

	SUL_MemCopy8(pRfCalib->nPcl2dbmArfcn_g,calib_p->pa->palcust.pcl2dbm_arfcn_g, 60);

	SUL_MemCopy8(pRfCalib->nPcl2dbmArfcn_p,calib_p->pa->palcust.pcl2dbm_arfcn_p, 60);

	pRfCalib->nProfiledBmMax_dp=calib_p->pa->palcust.profile_dBm_max_dp;

	pRfCalib->nProfiledBmMax_g=calib_p->pa->palcust.profile_dBm_max_g;

	pRfCalib->nProfiledBmMin_dp=calib_p->pa->palcust.profile_dBm_min_dp;

	pRfCalib->nProfiledBmMin_g=calib_p->pa->palcust.profile_dBm_min_g;

//	SUL_MemCopy8(pRfCalib->nProfile_dp,calib_p->pa->palcust.profile_dp, 2*1024);

	SUL_MemCopy8(pRfCalib->nProfileInterp_g,calib_p->pa->palcust.profile_interp_dp, 2*16);

//	SUL_MemCopy8(pRfCalib->nProfile_g,calib_p->pa->palcust.profile_g, 2*1024);

	SUL_MemCopy8(pRfCalib->nProfileInterp_g,calib_p->pa->palcust.profile_interp_g, 2*16);

	SUL_MemCopy8(pRfCalib->nRampHigh,calib_p->pa->palcust.ramp_high, 2*32);


	SUL_MemCopy8(pRfCalib->nRampLow,calib_p->pa->palcust.ramp_low, 2*32);

	pRfCalib->nRampSwap_dp=calib_p->pa->palcust.ramp_swap_dp;

	pRfCalib->nRampSwap_g=calib_p->pa->palcust.ramp_swap_g;


	SUL_MemCopy8(pRfCalib->nRaram,calib_p->pa->param, 4*15);


	SUL_MemCopy8(pRfCalib->nTimes,calib_p->pa->times, 2*15);
	
	CSW_TRACE(CFW_SIM_TS_ID, TSTXT(" ############CT_calibration end.\n"));	
}

VOID Emod_IMSItoASC(UINT8 *InPut,UINT8 *OutPut, UINT8 *OutLen)
{
    UINT8 i;
    OutPut[0] = ((InPut[1] & 0xF0 ) >> 4)+0x30;
    for (i = 2; i <InPut[0] +1 ; i++)
    {
        OutPut[2*(i-1)-1] = (InPut[i] & 0x0F)+0x30; 
        OutPut[2*(i-1)]   = ((InPut[i] & 0xF0 ) >> 4)+0x30;          
    }
	
    *OutLen =  2*InPut[0]-1;
}

VOID Emod_ASCtoIMSI(UINT8 *InPut,UINT8 *OutPut,UINT8 InLen)
{
    UINT8 i;
	OutPut[0]=(InLen+1)/2;
    OutPut[1] = ((InPut[0] & 0x0f ) << 4)+0x01;
    for (i = 2; i <OutPut[0]+1; i++)
    {
        OutPut[i] = (InPut[2*(i-2)+1] & 0x0f)+((InPut[2*(i-1)] & 0x0f ) << 4);        
    }
	if((OutPut[0]+1)<9)
	{
		for(i=(OutPut[0]+1);i<10;i++)
			OutPut[i]=0xff;
	}
}




#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
typedef struct {
    const u32 config_version;
    const lib_version_t phy;
    CT_calibration_t * calibration;
    const lib_version_t * const platform_version;
    const lib_version_t * const stack_version;
    const lib_version_t * const mmi_version;
    void * pJDSMsg;
    const lib_version_t * const phy_cfg_version;
} config_stub_t;
#endif


/* -------------------------------------------------- */
/* ----- XCV identifiers. */

enum {
    DEFAULT_CALIB_XCV_AERO1PLUS = CT_CALIB_XCV_MASK,
    DEFAULT_CALIB_XCV_AERO2,
    DEFAULT_CALIB_XCV_SKY74117,
    DEFAULT_CALIB_XCV_RDA6210,
    DEFAULT_CALIB_XCV_QTY
};

/* -------------------------------------------------- */
/* ----- XCV names. */

#define DEFAULT_CALIB_XCV_NAMES_STR     "XCV SiLab Aero 1 Plus", \
                                        "XCV SiLab Aero 2", \
                                        "XCV Skyworks 74117", \
                                        "XCV RDA 6210"

/* -------------------------------------------------- */
/* ----- PA identifiers. */

enum {
    DEFAULT_CALIB_PA_AWT6146 = CT_CALIB_PA_MASK,
    DEFAULT_CALIB_PA_RF7115,
    DEFAULT_CALIB_PA_TQM6M4002,
    DEFAULT_CALIB_PA_SKY77506,
    DEFAULT_CALIB_PA_SKY77318,
    DEFAULT_CALIB_PA_TQM6M4002_RDA,
    DEFAULT_CALIB_PA_RF3166,
    DEFAULT_CALIB_PA_QTY
};

/* -------------------------------------------------- */
/* ----- PA names. */

#define DEFAULT_CALIB_PA_NAMES_STR      "PA Anadigics AWT6146", \
                                        "PA RFMD RF71115", \
                                        "PA Triquint TQM6M4002", \
                                        "PA Skyworks 77506", \
                                        "PA Skyworks 77318", \
                                        "PA Triquint TQM6M4002_RDA", \
                                        "PA RFMD RF3166"
                                        
/* -------------------------------------------------- */
/* ----- SW identifiers. */

enum {
    DEFAULT_CALIB_SW_LMSP7CHA = CT_CALIB_SW_MASK,
    DEFAULT_CALIB_SW_RF7115,
    DEFAULT_CALIB_SW_TQM6M4002,
    DEFAULT_CALIB_SW_SKY77506,
    DEFAULT_CALIB_SW_ASM4518806T,
    DEFAULT_CALIB_SW_TQM6M4002_RDA,
    DEFAULT_CALIB_SW_ASWMF46KAAACT,
    DEFAULT_CALIB_SW_ATB4532GC3T0,
    DEFAULT_CALIB_SW_LMSP33QA,
    DEFAULT_CALIB_SW_LRT613S5,
    DEFAULT_CALIB_SW_QTY
};

/* -------------------------------------------------- */
/* ----- SW names. */

#define DEFAULT_CALIB_SW_NAMES_STR      "SW LMSP7CHA", \
                                        "SW RFMD RF71115", \
                                        "SW Triquint TQM6M4002", \
                                        "SW Skyworks 77506", \
                                        "SW TDK ASM4518806t", \
                                        "SW Triquint TQM6M4002_RDA", \
                                        "SW SANYO ASWMF46KAAACT", \
                                        "SW PILKOR ATB4532GC3T0", \
                                        "SW MURATA LMSP33QA", \
                                        "SW KYOCERA LRT613S5"

/*
================================================================================
  Function   : CFW_EmodGetSoftVersion
--------------------------------------------------------------------------------
  Scope      : 
  Parameters : 
  Return     :  
================================================================================
*/

VOID CFW_EmodGetSoftVersion( UINT8 nMoudle, Soft_Version *pSV )
{
    // FIXME Implement
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    // CAT LOL
  UINT32 *p =  (UINT32 *)0x8000107c;  
  config_stub_t *pCS = (config_stub_t *)(*p); 
  
  if( NULL == pSV)
  {
    return;
  }
  
  SUL_ZeroMemory8(pSV->nVersion, 30);
  
  switch(nMoudle)
  {
    case PHY_VERSION:
    {
      SUL_MemCopy8( pSV->nVersion, pCS->phy.version, 30 );
      pSV->nDate = pCS->phy.date ; 
      break;
    }
    case STACK_VERSION:
    {
      SUL_MemCopy8( pSV->nVersion, pCS->stack_version->version, 30 );
      pSV->nDate = pCS->stack_version->date ; 
      break;
    }
    case PHY_CFG_VERSION:
    {
      SUL_MemCopy8( pSV->nVersion, pCS->phy_cfg_version->version, 30 );
      pSV->nDate = pCS->phy_cfg_version->date ; 
      break;
    }
    case CSW_VERSION:
    {
      SUL_MemCopy8( pSV->nVersion, pCS->platform_version->version, 30 );
      pSV->nDate = pCS->platform_version->date ; 
      break;
    }  
    case MMI_VERSION:
    {
      SUL_MemCopy8( pSV->nVersion, pCS->mmi_version->version, 30 );
      pSV->nDate = pCS->mmi_version->date ; 
      break;
    }      
    default:
      break;
  }
  return;
#endif
}


/*
================================================================================
  Function   : CFW_EmodGetHWVersion
--------------------------------------------------------------------------------
  Scope      : 
  Parameters : 
  Return     :  
================================================================================
*/

VOID CFW_EmodGetHWVersion( UINT8 nMoudle, HW_Version *pHWV )
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    // CAT LOL
  UINT32 *p =  (UINT32 *)0x8000107c;  
  config_stub_t *pCS = (config_stub_t *)(*p); 
	static const char *xcv_names_str[] = {DEFAULT_CALIB_XCV_NAMES_STR};
	static const char *pa_names_str[] =  {DEFAULT_CALIB_PA_NAMES_STR};
	static const char *sw_names_str[] =  {DEFAULT_CALIB_SW_NAMES_STR};

  if( NULL == pHWV )
  {
    return;
  }
  
  SUL_ZeroMemory8(pHWV->nVersion, 30);
  
  switch(nMoudle)
  {
    case XCV_VERSION:
    {
      //char **xcv_name_str;
      UINT32 nXCV = pCS->calibration->xcv->palcust.name;

      SUL_MemCopy8(pHWV->nVersion,xcv_names_str[nXCV & ~CT_CALIB_XCV_MASK],30);
      
      break;
    }
    case PA_VERSION:
    {
      UINT32 nPA = pCS->calibration->pa->palcust.name;

      SUL_MemCopy8(pHWV->nVersion,pa_names_str[nPA & ~CT_CALIB_PA_MASK],30);
      break;
    }
    case SW_VERSION:
    {
      UINT32 nSW = pCS->calibration->sw->palcust.name;

      SUL_MemCopy8(pHWV->nVersion,sw_names_str[nSW & ~CT_CALIB_SW_MASK],30);
      break;
    }
    default:
      break;
  }
  return;
#endif
}
