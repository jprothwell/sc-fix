/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*******************************************************************************
 *
 * Filename:
 * ---------
 * fm_radio.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *
 *      FM Radio Driver
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#include "l1audio.h"

#include "fmd_m.h"
#include "hal_timers.h"
#include "sxr_tim.h"
#include "sxr_tls.h"

#include "tgt_fmd_cfg.h"

#include "mmi_trace.h"
#include "cos.h"

#include "mci.h"

//#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)
#include "device.h"
//#endif // (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)


#if (defined(FM_RADIO_ENABLE))
 bool isFmActive = FALSE;
//static bool isSpeakerOn = false;

void FMR_Init(uint16 aud_id);

//-------------------------------------------------------------------
//
// Internal function
//
//
//-------------------------------------------------------------------

//
//-------------------------------------------------------------------
// Function:  FM_Delay
// Purpose:  This function delay in ms unit.
// In:		 
// Return:    

//-------------------------------------------------------------------

static void FM_Delay(u16 iMs)
{

//    sxr_Sleep(iMs * HAL_TICK1S / 1000);
    COS_Sleep(iMs);

}

void FMR_PowerOn()
{
	FMD_ERR_T fmdError = FMD_ERR_NO;

	// Configure RDA
	if(isFmActive == FALSE)
	{
		FMR_TRACE();

	    fmdError = fmd_Open(tgt_GetFmdConfig(), FMD_BAND_US_EUROPE, NULL, NULL);
 
		if (fmdError != FMD_ERR_NO)
		{
			FMR_TRACE_ARG("FMD OPENING FAILED");
		}
		else
		{
			FMR_TRACE_ARG("FMD OPENING SUCCESS");
		}
		isFmActive = TRUE;
	}

}

void FMR_PowerOff()
{
	if(isFmActive == TRUE)
	{
		FMR_TRACE();
		
		isFmActive = FALSE;

        fmd_Close();
	}
}

 /* signal level range is 0 ~ 15 , 5767 has 16 levels ADC */
kal_uint8 FMR_ValidStop(kal_int16 freq, kal_int8 signalvl, kal_bool is_step_up)
{
	return ( fmd_ValidStop(freq * 100) );
}
 
void FMR_SetFreq( int16 curf ) /* input value: 875 - 1080 ( 87.5 MHz - 108.0 MHz)*/
{
#if 0
  uint16 sig_val;
	FMR_TRACE();
	FM_RDA5800_mute(FALSE);
	sig_val=FM_RDA5800_tune(curf, FALSE) ;                             //shizh 2007-12-20
#endif
	if(isFmActive == TRUE)
	{
		FMR_TRACE_ARG("FMR_SetFreq->curf:%d",curf);
	    FMD_ERR_T fmdError = FMD_ERR_NO;
		
        fmdError = fmd_Tune(curf * 100);

		FM_Delay(50);

		FMR_TRACE_ARG("FMR_SetFreq->fmdError:%d",fmdError);
	}
}

uint8 FMR_GetSignalLevel( int16 curf )
{
#if 0
    uint16 sig_val;
    sig_val=FM_RDA5800_tune(curf, FALSE) ; 
    //sig_val : 1--15
    if(sig_val > 25)
      sig_val = 15;
    else if(sig_val > 10)
      sig_val = sig_val-10;
    else
      sig_val = 1;
	  return (uint8)sig_val;
#endif
	UINT32 rssi = 0;


    fmd_GetRssi(&rssi);

    if (875==curf)    rssi=1;
    
	FMR_TRACE_ARG("FMR_GetSignalLevel->rssi:%d",rssi);
	return rssi;
	
}

void FMR_Mute( uint8 mute )
{
#if 0
	FMR_TRACE();
	if (mute)
	{
	    FM_RDA5800_mute(TRUE); //shizh 2007-12-20
	}
	else
	{
	    FM_RDA5800_mute(FALSE); //shizh 2007-12-20
	}
#endif
}

void FMR_Init(uint16 aud_id)
{

}
/*------------------------------------------------------------------------------*/

void FMR_SetOutputDevice( uint8 device )
{
    FMR_TRACE();

#if (CSW_EXTENDED_API_AUDIO_VIDEO == 0)
    MCI_AUDIO_PATH_T out = MCI_PATH_FM_HP;
    if (device == AUDIO_DEVICE_SPEAKER2)
    {
        out = MCI_PATH_FM_HP;
    }
    else if (device == AUDIO_DEVICE_LOUDSPEAKER)
    {
        out = MCI_PATH_FM_LSP_HP;
    }
    else if (device == AUDIO_DEVICE_SPEAKER_BOTH)
    {
        out = MCI_PATH_FM_LSP_HP;
    }

    MCI_FmSetOutputDevice(out);
#endif // (CSW_EXTENDED_API_AUDIO_VIDEO == 0)

    //DM_SetAudioMode(DM_AUDIO_MODE_EARPIECE);//used to turn off mp3 output

#if !defined(MT6205B) && !defined(MT6208)
    //AFE_SetOutputDevice( L1SP_FM_RADIO, device );
#else
    // The output path of FM on MT6205 platform is fixed.
#endif
}

void FMR_SetOutputVolume( uint8 volume1, uint8 volume2 )
{
#if 0
	FMR_TRACE();
//	FM_RDA5800_setVolume(volume1); //shizh 2007-12-20
	FM_RDA5800_setVolume(volume1*2 + 1); //yuzl 2008-2-25
#if !defined(MT6205B) && !defined(MT6208)
	//AFE_SetOutputVolume( L1SP_FM_RADIO, volume1, volume2 );
#else
	//FMDrv_SetVolumeLevel(volume1/20);
#endif
#endif
	FMR_TRACE_ARG("FMR_SetOutputVolume:%d,%d",volume1,volume2);
	if(isFmActive == TRUE)
	{
		#if (CSW_EXTENDED_API_AUDIO_VIDEO == 0)
		MCI_FmSetup(volume2);
		#endif
		fmd_SetVolume(volume2, FALSE, FALSE);
	}
}

/*
  FMR_IsChipValid could be called to confirm if FM chip dose exist.
  Return ture if FM chip is valid, return false otherwise.
*/

bool FMR_IsChipValid(void)
{
	FMR_TRACE();
	return TRUE;
	//return FMDrv_IsChipValid();
}

bool FMR_IsActive(void)
{
	FMR_TRACE();
	return isFmActive;
}

void FMR_EvaluateRSSIThreshold(void)
{
	FMR_TRACE();
	
#if defined(MT6189A1) || defined(MT6188A1)
	    //    FMDrv_EvaluateRSSIThreshold();
#endif
}

void FMR_Radio_EngineerMode(kal_uint16 group_idx, kal_uint16 item_idx, kal_uint32 item_value)
{
	FMR_TRACE();
	
#if defined(MT6189A1) || defined(MT6188A1)
	//FMDrv_radio_item_info(group_idx, item_idx, item_value);
#endif
}

#endif // defined(FM_RADIO_ENABLE)

