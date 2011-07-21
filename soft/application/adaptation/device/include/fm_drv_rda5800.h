/*************************************************************************/
/*                                                                                                                            */
/*            Copyright (C) 2007, Coolsand Technologies, Inc.                                          */
/*                            All Rights Reserved                                                                    */
/*                                                                                                                            */
/*************************************************************************/
/* BY jixj******************************************************************/

#ifndef _FM_DRV_RDA5800_H

#define _FM_DRV_RDA5800_H
#include "resdevice.h"
// Typedef
typedef struct {
	u16 iLoFreqEdge; // the bottom of seek freq
	u16 iHiFreqEdge; // the top of seek freq	
	u16 iCurFreq;       // Current Freq	
	u8  iVol;               //  Current volume
	bool isStereo;      // Stereo or Mono?


	}RDA5800_STATUS, *PRDA5800_STATUS;

// Function  prototype
//
//-------------------------------------------------------------------
// Function: FM_RDA5800_init
// Purpose:  This function is used to init RDA5800
// In:		 
// Return:   boolean

//-------------------------------------------------------------------
bool FM_RDA5800_init(u8 scl, u8 sda);

//
//-------------------------------------------------------------------
// Function:  FM_RDA5800_close
// Purpose:  This function is used to close RDA5800 
// In:	     
// Return:    boolean

//-------------------------------------------------------------------		
bool FM_RDA5800_close(void) ;
//
//-------------------------------------------------------------------
// Function:  FM_RDA5800_open
// Purpose:  This function is used to open RDA5800 
// In:	     bool True, use the freq last open 
//                      False,open the device from the bottom of the freq;
// Return:    boolean

//-------------------------------------------------------------------			
bool FM_RDA5800_open(bool bUseLastFreq) ;
//
//-------------------------------------------------------------------
// Function:  FM_RDA5800_tune
// Purpose:  This function is used to tune the RDA5800
// In:	     Freq value	 isStereo stereo or not?
// Return:    boolean

//-------------------------------------------------------------------
bool  FM_RDA5800_tune(u16 iFrequency,bool isStereo) ;
//
//-------------------------------------------------------------------
// Function:  FM_RDA5800_mute
// Purpose:  This function is used to set RDA5800 mute or not
// In:	     
// Return:    boolean

//-------------------------------------------------------------------	
bool FM_RDA5800_mute(bool isMute);
//
//-------------------------------------------------------------------
// Function:  FM_RDA5800_getCurStatus
// Purpose:  This function is used to get the status of RDA5800
// In:	     
// Return:   RDA5800 status
// Note :     This function only return right value after the call of initial function 


//-------------------------------------------------------------------	
bool FM_RDA5800_getCurStatus(RDA5800_STATUS * pRDA5800Status);
//
//-------------------------------------------------------------------
// Function:  FM_RDA5800_setVolume
// Purpose:  This function is used to set RDA5800 volume level
// In:	     vol:0-15
// Return:    boolean

//-------------------------------------------------------------------	
bool FM_RDA5800_setVolume(u8 iVol);
//
//-------------------------------------------------------------------
// Function:  FM_RDA5800_seek
// Purpose:  This function is used to seek a valid station
//                If it return true,a valid station is found and it's freq is stored in iCurFreq
//                Else a valid station is not  found and return current freq for MMI use.
//                Every call of this function will increase/decrease current  frequency in 100kHz

// In:	     boolean isSeekUp  seek direction
// Return:    iCurFreq current freq in 100K unit
//                boolean

//-------------------------------------------------------------------	
bool  FM_RDA5800_seek(bool isSeekUp,u16 * iCurFreq) ;
//
//-------------------------------------------------------------------
// Function:  FM_RDA5800_autoSeek
// Purpose:  This function is used to start a auto seek action
// In:	     pNumOfStation, The count of seek stations,
//                pStationBuff, the frequency of seek stations
// Return:    boolean

//-------------------------------------------------------------------		
bool  FM_RDA5800_autoSeek (u8 *pNumOfStation,u16 *pStationBuff) ;


//
//
//-------------------------------------------------------------------
// Function:  FM_RDA5800_I2S
// Purpose:  This function is used to open RDA5800 i2s to send to the pcm data 
// In:	     
// Return:    boolean

//-------------------------------------------------------------------	
bool FM_RDA5800_I2S(void);

//
//-------------------------------------------------------------------
// Function:  FM_RDA5800_STOPI2S
// Purpose:  This function is used to disable RDA5800 i2s
// In:	     
// Return:    boolean

//-------------------------------------------------------------------	
bool FM_RDA5800_STOPI2S(void);

#endif








