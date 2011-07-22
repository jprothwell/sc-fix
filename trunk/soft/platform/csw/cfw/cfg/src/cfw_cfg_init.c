///*************************************************************
//hameina 2006.8.21 modified
//
//notice: this version is based on reg systerm
//
//modify record:
//hameina 2006.9.13 [mod] CFW_CfgInit,  get INDEVENT when power on, avoid 
//          useing reg read too frequently.
//2006.9.29 hameina [mod] cfg_readdefault() the type of function changed:bool->UINT32
///**************************************************************

#include <sul.h>
#include <cfw.h>
#include "cfw_cfg.h"
#include "csw_debug.h"
//
// Initialize CFG global variables when power on.
//
#ifdef CFW_MULTI_SIM
extern UINT16 IND_EVENT_general[];
#else
extern UINT16 IND_EVENT_general;
#endif
UINT32 CFW_CfgInit()
{
	UINT32 ret = ERR_SUCCESS;
	UINT32 param=0;

	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgInit);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgInit\n");

#ifdef CFW_MULTI_SIM
  UINT8 i = 0x00;
  for( ; i <= CFW_SIM_NUMBER; i++ )
  {
    IND_EVENT_general[i] = 0xFF;      
  }
#else
  IND_EVENT_general = 0xFF;
#endif
	CFW_CfgSimParamsInit();
	ret = CFW_CfgGetUserProfile ( );

	ret = cfg_read_hex("general", "IND_EVENT", &param, 2);
	if(ERR_SUCCESS != ret)
	{
	    CSW_TRACE(CFW_CFG_TS_ID, "cfg_read_hex, error ret =0x%x\n",ret);
	    ret = cfg_readdefault("general", "IND_EVENT", &param);
	    if(ERR_SUCCESS != ret)
	    {
		    CSW_TRACE(CFW_CFG_TS_ID, "cfg_readdefault, error ret =0x%x\n",ret);
		    CSW_PROFILE_FUNCTION_EXIT(CFW_CfgInit);
		    return ret;
	    }
	}

#ifdef CFW_MULTI_SIM
i = 0x00;
  for( ; i<= CFW_SIM_NUMBER; i++ )
  {
    IND_EVENT_general[i] = param;      
  }

#else
	IND_EVENT_general = (UINT16)param;
#endif

	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgInit, exit ret =0x%x\n",ret);
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgInit);
	return ret;
}

#ifndef CFW_MULTI_SIM
UINT32 CFW_Cfg_test()
{
#ifdef CFW_GPRS_SUPPORT

	//UINT32 ret = 0;
	UINT32 err_code =0;
	UINT8 uClip=0;
	UINT8 nCid=1;
	CFW_GPRS_PDPCONT_INFO sPdpCont;
	UINT8 apn[8];
	UINT8 add[10];
	CFW_SMS_PARAMETER   sInfo;
	UINT8 nIndex;
	CFW_SMS_STORAGE_INFO sStorageInfo;
	UINT16 nStorage=0;
//test clip, this value was saved in general part
 
	err_code = CFW_CfgGetClip(&uClip);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 1, call CFW_CfgGetClip = 0x%x, clip=%d\n",err_code, uClip);
	uClip = 0;
	err_code = CFW_CfgSetClip(uClip);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 2, call CFW_CfgSetClip = 0x%x, clip=%d\n",err_code, uClip);
	err_code = CFW_CfgGetClip(&uClip);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 3, call CFW_CfgGetClip = 0x%x, clip=%d\n\n",err_code, uClip);
	
// test colp, this value was saved in cm part
	err_code = CFW_CfgGetColp(&uClip);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 4, call CFW_CfgGetColp = 0x%x, Colp=%d\n",err_code, uClip);
	uClip = 1;
	err_code = CFW_CfgSetColp(uClip);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 5, call CFW_CfgSetColp = 0x%x, Colp=%d\n",err_code, uClip);
	err_code = CFW_CfgGetColp(&uClip);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 6, call CFW_CfgGetColp = 0x%x, Colp=%d\n\n",err_code, uClip);
	
// test audiomode, device part,this value was saved in a global.

	err_code = CFW_CfgGetAudioAudioMode(&uClip);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 7, call CFW_CfgGetAudioAudioMode = 0x%x, Mode=%d\n",err_code, uClip);
	uClip = 2;
	err_code = CFW_CfgSetAudioAudioMode(uClip);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 8, call CFW_CfgSetAudioAudioMode = 0x%x, Mode=%d\n",err_code, uClip);
	err_code = CFW_CfgGetAudioAudioMode(&uClip);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 9, call CFW_CfgGetAudioAudioMode = 0x%x, Mode=%d\n\n",err_code, uClip);

// test frequency band, nw part
	err_code = CFW_CfgNwGetFrequencyBand(&uClip);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 10, call CFW_CfgNwGetFrequencyBand = 0x%x, Band=%d\n",err_code, uClip);
	uClip = 0x48;
	err_code = CFW_CfgNwSetFrequencyBand(uClip);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 11, call CFW_CfgNwSetFrequencyBand = 0x%x, Band=%d\n",err_code, uClip);
	err_code = CFW_CfgNwGetFrequencyBand(&uClip);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 12, call CFW_CfgNwGetFrequencyBand = 0x%x, Band=%d\n\n",err_code, uClip);

// test PDDcontext, gprs part
	sPdpCont.pApn = apn;
	sPdpCont.pPdpAddr = add;
	sPdpCont.nApnSize = 8;
	sPdpCont.nPdpAddrSize = 10;
	err_code = CFW_CfgGprsGetPdpCxt(nCid, &sPdpCont);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 13, call CFW_CfgGprsGetPdpCxt = 0x%x, nCid=%d\n",err_code, nCid);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 14,  nPdpType = %d, nHComp=%d\n",
			sPdpCont.nPdpType, sPdpCont.nHComp);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 15,  nDComp = %d, nPdpAddrSize=%d\n",
			sPdpCont.nDComp, sPdpCont.nPdpAddrSize);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 16,  pPdpAddr = %s, nApnSize=%d,pApn=%s\n\n",
			sPdpCont.pPdpAddr, sPdpCont.nApnSize,sPdpCont.pApn);
			
	nCid = 7;
	sPdpCont.nPdpType =2;
	sPdpCont.nHComp = 3;
	sPdpCont.nDComp = 1;
	sPdpCont.nPdpAddrSize =9;
	SUL_StrCopy(sPdpCont.pPdpAddr, "ABwwCGDEF");
	sPdpCont.nApnSize = 7;
	SUL_StrCopy(sPdpCont.pApn, "1002567");
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, ####, call CFW_CfgGprsSetPdpCxt = 0x%x, nCid=%d\n\n",err_code, nCid);
	err_code = CFW_CfgGprsSetPdpCxt(nCid, &sPdpCont);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 17, call CFW_CfgGprsSetPdpCxt = 0x%x, nCid=%d\n\n",err_code, nCid);
	sPdpCont.pApn = apn;
	sPdpCont.pPdpAddr = add;
	SUL_ZeroMemory8(apn, 8);
	SUL_ZeroMemory8(add, 10);
	sPdpCont.nApnSize = 8;
	sPdpCont.nPdpAddrSize = 10;
	err_code = CFW_CfgGprsGetPdpCxt(nCid, &sPdpCont);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 18, call CFW_CfgGprsGetPdpCxt = 0x%x, nCid=%d\n",err_code, nCid);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 19,  nPdpType = %d, nHComp=%d\n",
			sPdpCont.nPdpType, sPdpCont.nHComp);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 20,  nDComp = %d, nPdpAddrSize=%d\n",
			sPdpCont.nDComp, sPdpCont.nPdpAddrSize);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 21,  pPdpAddr = %s, nApnSize=%d,pApn=%s\n\n",
			sPdpCont.pPdpAddr, sPdpCont.nApnSize,sPdpCont.pApn);

// test sms param, sms part
	nIndex = 1;
	err_code = CFW_CfgGetSmsParam(&sInfo, nIndex);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 22, call CFW_CfgGetSmsParam = 0x%x, nIndex=%d\n",err_code, nIndex);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 23  bSimChange = %d, vp=%d\n",
			sInfo.bSimChange, sInfo.dcs, sInfo.vp);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 24,  bearer = %d, bReserved=%d\n",
			sInfo.bearer, sInfo.bReserved);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 25,  mr = %d, mti=%d,nNumber=%s\n",
			sInfo.mr, sInfo.mti,sInfo.nNumber);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 26,  pid = %d, rp=%d,ssr=%d\n",
			sInfo.pid, sInfo.rp,sInfo.ssr);
	nIndex = 2;
	sInfo.bearer =1;
	sInfo.bReserved = 3;
	sInfo.bSimChange = 1;
	sInfo.dcs =8;
	SUL_StrCopy(sInfo.nNumber, "12365487");
	sInfo.mr = 6;
	sInfo.mti = 4;
	sInfo.pid = 23;
	sInfo.rp  = 43;
	sInfo.ssr =10;
	sInfo.vp = 11;
	err_code = CFW_CfgSetSmsParam(&sInfo, nIndex);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 27, call CFW_CfgSetSmsParam = 0x%x, nIndex=%d,nNumber=%s\n",err_code, nIndex,sInfo.nNumber);
	SUL_ZeroMemory8(&sInfo, sizeof(CFW_SMS_PARAMETER));

	err_code = CFW_CfgGetSmsParam(&sInfo, nIndex);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 28, call CFW_CfgGetSmsParam = 0x%x, nIndex=%d\n",err_code, nIndex);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 29,  bSimChange = %d, vp=%d\n",
			sInfo.bSimChange, sInfo.dcs, sInfo.vp);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 30,  bearer = %d, bReserved=%d\n",
			sInfo.bearer, sInfo.bReserved);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 31,  mr = %d, mti=%d,nNumber=%s\n",
			sInfo.mr, sInfo.mti,sInfo.nNumber);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 32,  pid = %d, rp=%d,ssr=%d\n\n",
			sInfo.pid, sInfo.rp,sInfo.ssr);
			
// test CFW_CfgSetSmsStorageInfo, sms part
	nStorage = CFW_SMS_STORAGE_SM;
	err_code = CFW_CfgGetSmsStorageInfo(&sStorageInfo, nStorage);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 33, call CFW_CfgGetSmsStorageInfo = 0x%x, nStorage=%d\n",err_code, nStorage);

	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 34, readRecords=%d, sentRecords=%d, storageId=%d\n",
		sStorageInfo.readRecords, sStorageInfo.sentRecords,sStorageInfo.storageId);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 35, totalSlot=%d, unknownRecords=%d, unReadRecords=%d\n",
		sStorageInfo.totalSlot, sStorageInfo.unknownRecords,sStorageInfo.unReadRecords);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 36, unsentRecords=%d, usedSlot=%d\n\n",
		sStorageInfo.unsentRecords, sStorageInfo.usedSlot);
		
	nStorage = CFW_SMS_STORAGE_ME;
	sStorageInfo.readRecords = 20;
	sStorageInfo.sentRecords =30;
	sStorageInfo.storageId = CFW_SMS_STORAGE_ME;
	sStorageInfo.totalSlot =1000;
	sStorageInfo.unknownRecords = 111;
	sStorageInfo.unReadRecords = 222;
	sStorageInfo.unsentRecords =33;
	sStorageInfo.usedSlot =500;
	err_code = CFW_CfgSetSmsStorageInfo(&sStorageInfo, nStorage);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 37, call CFW_CfgSetSmsStorageInfo = 0x%x, nStorage=%d\n\n",err_code, nStorage);

	nStorage = CFW_SMS_STORAGE_ME;
	err_code = CFW_CfgGetSmsStorageInfo(&sStorageInfo, nStorage);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 38, call CFW_CfgGetSmsStorageInfo = 0x%x, nStorage=%d\n",err_code, nStorage);

	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 39, readRecords=%d, sentRecords=%d, storageId=%d\n",
		sStorageInfo.readRecords, sStorageInfo.sentRecords,sStorageInfo.storageId);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 40, totalSlot=%d, unknownRecords=%d, unReadRecords=%d\n",
		sStorageInfo.totalSlot, sStorageInfo.unknownRecords,sStorageInfo.unReadRecords);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 41, unsentRecords=%d, usedSlot=%d\n\n",
		sStorageInfo.unsentRecords, sStorageInfo.usedSlot);
// test cssn param, cm part
	uClip =0;
	nCid =0;
	err_code = CFW_CfgGetSSN(&uClip, &nCid);
	CSW_TRACE(CFW_CFG_TS_ID, "in cfg_test, 42, cssi = %d, cssu=%d\n\n",uClip,nCid);
#endif
	return ERR_SUCCESS;
}
UINT32 CFW_Cfg_test2()
{
	UINT32 err_code = ERR_SUCCESS;
	UINT16 uInd=0;
	UINT8 sOption;
	UINT8 sNewSmsStorage;
	CFW_IND_EVENT_INFO sIndInfo;
	//general indicator
	err_code = CFW_CfgGetIndicatorEvent(&uInd);
	CSW_TRACE(CFW_CFG_TS_ID, "1, call CFW_CfgGetIndicatorEvent,ret=0x%x, uInd=0x%x\n",err_code, uInd);
	uInd = 0x3b;
	err_code = CFW_CfgSetIndicatorEvent(uInd);
	CSW_TRACE(CFW_CFG_TS_ID, "2, call CFW_CfgSetIndicatorEvent,ret=0x%x, uInd=0x%x\n\n",err_code, uInd);

	err_code = CFW_CfgGetIndicatorEventInfo(&sIndInfo);
	CSW_TRACE(CFW_CFG_TS_ID, "3, call CFW_CfgGetIndicatorEventInfo,ret=0x%x, bc=%d,bit_err=%d,sq=%d\n",
		err_code, sIndInfo.bc,sIndInfo.bit_err,sIndInfo.sq);
	CSW_TRACE(CFW_CFG_TS_ID, "4,call=%d, nw_srv=%d,roam=%d,sms_full=%d, sms_receive=%d,sms_receive=%d\n\n",
		sIndInfo.call, sIndInfo.nw_srv,sIndInfo.roam,sIndInfo.sms_full, sIndInfo.sms_receive,sIndInfo.sms_receive);

// sms smsstorageOption
	sOption =0;
	sNewSmsStorage = 0;
	err_code = CFW_CfgGetNewSmsOption(&sOption,&sNewSmsStorage);
	//CSW_TRACE(CFW_CFG_TS_ID, PCSTR pFormat,...)
	//CFW_CfgSetNewSmsOption
	return ERR_SUCCESS;
}
UINT32 CFW_CfgExit()
{

    return ERR_SUCCESS;
}
#endif
