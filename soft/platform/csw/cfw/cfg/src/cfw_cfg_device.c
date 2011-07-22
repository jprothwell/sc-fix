///*************************************************************
//hameina 2006.8.21 modified
//
//notice: this version is based on reg systerm
//
//modify record:
///**************************************************************

#include <cfw.h>
#include "cfw_cfg.h"
#include "csw_debug.h"

UINT32 CFW_CfgSetAudioOutputParam (
  CFW_AUD_OUT_PARAMETER* pAudOutParam
)
{
	return ERR_CFW_NOT_SUPPORT;
}

UINT32 CFW_CfgGetAudioOutputParam (
  CFW_AUD_OUT_PARAMETER* pAudOutParam
)
{
	UINT32 ret = ERR_CFW_NOT_SUPPORT;
	return ret;
}

UINT8 gDevAudioMode = 1;//hameina[+]2006.8.23
UINT32 CFW_CfgSetAudioAudioMode ( UINT8 nAudioMode)
{
       UINT32 ret = ERR_SUCCESS;
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgSetAudioAudioMode);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetAudioAudioMode, nAudioMode = 0x%x\n", nAudioMode);
	ret = CFW_CfgNumRangeCheck(nAudioMode, DE_AUDIOMODE_RANGE);
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgNumRangeCheck, error ret = 0x%x\n", ret);
		ret = ERR_CFG_PARAM_OUT_RANGR;
	}
	else
	{
	    //ret = cfg_write_8("device", "AUDIO_MODE", nAudioMode);
	    gDevAudioMode = nAudioMode;
	}
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgSetAudioAudioMode);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgSetAudioAudioMode  exit\n");
       return ret;
}

UINT32 CFW_CfgGetAudioAudioMode ( UINT8* pAudioMode )
{
	CSW_PROFILE_FUNCTION_ENTER(CFW_CfgGetAudioAudioMode);
    *pAudioMode = gDevAudioMode;
	CSW_PROFILE_FUNCTION_EXIT(CFW_CfgGetAudioAudioMode);
	CSW_TRACE(CFW_CFG_TS_ID, "CFW_CfgGetAudioAudioMode, *pAudioMode = 0x%x\n", *pAudioMode);
    return ERR_SUCCESS;
}


UINT32 CFW_CfgSetLoudspeakerVolumeLevel(
  UINT8 nVolumeLevel
)
{
   // WRITE_NUMBER_SETTING_8( "SPEAK_V", nVolumeLevel, "device" );    
    return ERR_CFW_NOT_SUPPORT;
}

UINT32 CFW_CfgGetLoudspeakerVolumeLevel(
  UINT8* pVolumeLevel
)
{
    //READ_NUMBER_SETTING_8( "SPEAK_V", *pVolumeLevel, "device" );    
    return ERR_CFW_NOT_SUPPORT;
}

UINT32 CFW_CfgGetLoudspeakerVolumeLevelRange(
  UINT8* pMinVolumeLevel,
  UINT8* pMaxVolumeLevel
)
{
    UINT32 ret = ERR_CFW_NOT_SUPPORT;
    
   // *pMinVolumeLevel = 0;
   // *pMaxVolumeLevel = 7;

    return ret;
}

UINT32 CFW_CfgDeviceSwithOff (VOID)
{
    UINT32 ret = ERR_CFW_NOT_SUPPORT;

    return ret;
}

