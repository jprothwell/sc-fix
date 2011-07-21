
/*************************************************************************/
/*                                                                                                                            */
/*            Copyright (C) 2007, Coolsand Technologies, Inc.                                          */
/*                            All Rights Reserved                                                                    */
/*                                                                                                                            */
/*************************************************************************/
/* BY jixj******************************************************************/


//#include "hal.h"

#include "fm_drv_rda5800.h"
#include "fm_drv_i2c.h"

//#include "sxr_ops.h"
//#include "sxs_io.h"
//#include "sxr_mem.h"

// Define
#define DEBUG 0
#define RDA5800_I2C_ADDR  0x20
// Typedef

extern VOID DRV_ResetOut(BOOL);

// Global variant 
u16  gFMShadowRegMap[45];
const u16	 gFMDefaultRegMap[45] = {
	0x0000,
	0x0000,	
	0xd881,	//0x02
	0x0000,
	0x4400,	//0x04	
	0x13ff,     //0x10ff, //0x13ff, lilin, 06/22, 0x10f8, //0x05
	0x0000,
	0x00cd,
	0x0096,
	0x0020,
	0x4163,
	0x0806,
	0x5800,
	0x5800,
	0x5800,
	0x5800,
	0x4c17,	//lilin, 08/13, seek_singlestep, 0x4817,
	0x20a2,
	0x0000,
	0x000f,
	0x06de,
	0xecc0,
	0x0200,
	0x5383,
	0x95a4,
	0xe848,
	0x0500,	//0x0500, lilin, 06/22, 0x0500,
	0x00a4, //0x00a4, lilin, 06/22, 0x00a4,
	0x889b,
	0x0d84,
	0x4f04,
	0x8832,
	0x7f71,
	0x0660,
	0x4010,
	0x6002,
	0x1808,
	0x6458,
	0x787f,
	0x0100,
	0xc040,
	0xc020,
	0x0024,
	0x0400,
	0x0020,
	};

#if DEBUG

//-------------------------------------------------------------------
// Function:  DumpAllRegister
// Purpose:  Show the wr buffer and rd buffer
// In:		
//                              
// Return:   

//-------------------------------------------------------------------

void DumpAllRegister(void)

{
	u8 iCount = 0;
	
	for(iCount = 0;iCount < 45;iCount++)
	{	
//		SXS_TRACE(TSTDOUT,"gFMShadowRegMap[%d]: 0x%x\n",iCount,gFMShadowRegMap[iCount]);
	}	


}
#endif


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
	u16 iCount0 = 0,iCount1 = 0;

	
	for(iCount0 = 0;iCount0 <iMs * 10;iCount0 ++)
	for(iCount1 = 0;iCount1 <1000;iCount1++)
		;

}
//
//-------------------------------------------------------------------
// Function:  FM_ChanToFreq
// Purpose:  This function convert channel value to freq value
// In:		 
// Return:    Freq value

//-------------------------------------------------------------------
static u16  FM_ChanToFreq(u16 channel) 
{
	u16 iChannelSpacing = 0,iBottomOfBand = 0,iFrequency = 0;

	// First get the lower Freq edge
	if ( (gFMShadowRegMap[3] & 0x0002) == 0x0000) 
		iBottomOfBand = 875;
	else 	
		iBottomOfBand = 760;
	
	// Get the spacing value
	if ( (gFMShadowRegMap[3] & 0x0001) == 0x0000) 
		iChannelSpacing = 1;
	else if ((gFMShadowRegMap[5] & 0x0001) == 0x0001) 
		iChannelSpacing = 2;
	else 
		iChannelSpacing = 1;

	iFrequency = (iBottomOfBand + iChannelSpacing * channel);
	return (iFrequency);
}


//
//-------------------------------------------------------------------
// Function:  FM_FreqToChan
// Purpose:  This function converts freq value to channel value
// In:		 
// Return:    Channel value

//-------------------------------------------------------------------
static u8 Fm_FreqToChan(u16 iFrequency)  
{
	u16 iChannelSpacing,iBottomOfBand,iChannel;

	if ((gFMShadowRegMap[3] & 0x0002) == 0x0000) 
		iBottomOfBand = 875;
	else 	
		iBottomOfBand = 760;
	if ((gFMShadowRegMap[3] & 0x0001) == 0x0000) 
		iChannelSpacing = 1;
	else if ((gFMShadowRegMap[5] & 0x0001) == 0x0001) 
		iChannelSpacing = 2;
	else 
		iChannelSpacing = 1;

	iChannel = (iFrequency - iBottomOfBand) / iChannelSpacing;
	return (iChannel);
}

//
//-------------------------------------------------------------------
// Function:  FM_SingleStepSeek
// Purpose:  This function is used to tune the RDA5800 in single step
// In:	     boolean
// Return:    boolean

//-------------------------------------------------------------------		
static bool  FM_SingleStepSeek(bool isSeekUp) 
{
	u8 iWrBuff[8],iCount = 0;
//to do	

	gFMShadowRegMap[2] &= ~0x0300;
	if(!isSeekUp) 
		
	{
		gFMShadowRegMap[2] |= 0x0100;
		iWrBuff[0] = ((gFMShadowRegMap[2]>>8) | 0x01);	       // Seek down
	}
	else 
	{
		gFMShadowRegMap[2] |= 0x0300;
		iWrBuff[0] = ((gFMShadowRegMap[2]>>8) | 0x03); 	// Seek up
	}
	iWrBuff[1] = gFMShadowRegMap[2];
	
	if( !I2CSendData(RDA5800_I2C_ADDR,0,iWrBuff,2,&iCount,FALSE))
		return FALSE;

	// Wait 100ms for seek complete (STC = 1) 
	FM_Delay(100);
	
	return TRUE;
	
}




//-------------------------------------------------------------------
//
// External function
//
//
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
// Function: FM_RDA5800_init
// Purpose:  This function is used to init RDA5800
// In:		 
// Return:   boolean

//-------------------------------------------------------------------
bool FM_RDA5800_init(u8 scl,u8 sda)     
{
	DRV_ResetOut(0);
#ifdef FM_DRIVER_ON_LILY
	return MCI_FM_RDA5800_init(scl,sda);
#else	

	u8 iWrBuff[86] = {0};
	u8 iCount = 0;

	// #SEN=1 use I2C bus,reset the chip

	// First init the I2C bus
        HWInitI2C(scl,sda);

	
	// Fill the shadow register
	for(iCount = 0; iCount < 45;iCount ++)
		gFMShadowRegMap[iCount] = gFMDefaultRegMap[iCount];
		

	// REG 0x02

	// |15:DHIZ|14:DMUTE|13:MONO|12:BASS|11:LAB_MODE |10:RESV|9:SEEKUP|8:SEEK|
	// |7:XTALEN|6-1:RESV|0:ENABLE| 

	//  DHIZ=Audio out hi-z,DUTE= Normal operation,MONO= stereo,BASS= Bass boost enabled
	//  LAB_MODE = acces all REG 00H-3AH,XTAL= EN,ENABLE=EN
	// Update shadow register
	//gFMShadowRegMap[2] &=  ~0x0001; 
	iWrBuff[0] = gFMShadowRegMap[2] >> 8;
	iWrBuff[1] = gFMShadowRegMap[2];

	// Power up the chip and EN the OSC
	if( !I2CSendData(RDA5800_I2C_ADDR,0,iWrBuff,2,&iCount,FALSE))
		return FALSE;

	// Use DCXO,should wait 500ms for RCLK stable
	FM_Delay(500);
	
	for(iCount = 0; iCount < 43; iCount++)
	{
		iWrBuff[iCount * 2] = (gFMShadowRegMap[iCount + 2] >> 8);
		iWrBuff[iCount * 2 + 1] = (gFMShadowRegMap[iCount + 2]);
	}
		
	if( !I2CSendData(RDA5800_I2C_ADDR,0,iWrBuff,86,&iCount,FALSE))
		return FALSE;

	
	// Wait for 100ms to guarantee STC to clear

	FM_Delay(100);

	// Close all register windows
	gFMShadowRegMap[2] &= ~0x0800;


	return TRUE;
#endif
}



//
//-------------------------------------------------------------------
// Function:  FM_RDA5800_sleepIn
// Purpose:  This function is used to close RDA5800 
// In:	     
// Return:    boolean

//-------------------------------------------------------------------		
bool FM_RDA5800_close(void) 
{
#ifdef FM_DRIVER_ON_LILY
	return MCI_FM_RDA5800_close();
#else	

	u8 iWrBuff[2];
	u8 iCount = 0;

	// Reg 0x02
	// |15:DHIZ|14:DMUTE|13:MONO|12:BASS|11:LAB_MODE |10:RESV|9:SEEKUP|8:SEEK|
	// |7:XTALEN|6-1:RESV|0:ENABLE| 

	//  DHIZ=Audio out hi-z,DUTE= Normal operation,MONO= stereo,BASS= Bass boost enabled
	//  LAB_MODE = acces partial Reg,XTAL= EN,ENABLE= Disable



	// Update shadow register
	gFMShadowRegMap[2] &=  ~0x0001; 
	iWrBuff[0] = gFMShadowRegMap[2] >> 8;
	iWrBuff[1] = gFMShadowRegMap[2];
	
	
	
	if( !I2CSendData(RDA5800_I2C_ADDR,0,iWrBuff,2,&iCount,FALSE))
		return FALSE;

	return TRUE;
#endif
	
}


//
//-------------------------------------------------------------------
// Function:  FM_RDA5800_open
// Purpose:  This function is used to open RDA5800 
// In:	     bool True, use the freq last open 
//                      False,open the device from the bottom of the freq;
// Return:    boolean

//-------------------------------------------------------------------		
bool FM_RDA5800_open(bool bUseLastFreq) 
{
#ifdef FM_DRIVER_ON_LILY
	return MCI_FM_RDA5800_open(bUseLastFreq);
#else	

	u8 iRdBuff[4],iWrBuff[4];
	u8 iCount = 0;
	
	// REG 0x02
	// |15:DHIZ|14:DMUTE|13:MONO|12:BASS|11:LAB_MODE |10:RESV|9:SEEKUP|8:SEEK|
	// |7:XTALEN|6-1:RESV|0:ENABLE| 

	//  DHIZ=Audio out hi-z,DUTE= Normal operation,MONO= stereo,BASS= Bass boost enabled
	//  LAB_MODE = acces all Reg,XTAL= EN,ENABLE= Disable

	// Update shadow register
	gFMShadowRegMap[2] |=  0x0001; 
	iWrBuff[0] = gFMShadowRegMap[2] >> 8;
	iWrBuff[1] = gFMShadowRegMap[2];
		
	if( !I2CSendData(RDA5800_I2C_ADDR,0,iWrBuff,2,&iCount,FALSE))
		return FALSE;

	// Reg 0x03
	// |15-8:CHAN|2:BAND|1:SPACE|
	
	// Open,we set the freq to the initial Freq
	if(!bUseLastFreq)
	gFMShadowRegMap[3] &=  ~0xff00;

	iWrBuff[2] = gFMShadowRegMap[3] >> 8; 		 
	iWrBuff[3] = gFMShadowRegMap[3]; 
	
	if( !I2CSendData(RDA5800_I2C_ADDR,0,iWrBuff,4,&iCount,FALSE))
		return FALSE;		
	
	//wait 100ms for Tune complete (STC = 1) 
	FM_Delay(100);
	
		
	if (!I2CRecvData(RDA5800_I2C_ADDR,0,iRdBuff,4,FALSE)) 
		return FALSE;

	gFMShadowRegMap[10] = iRdBuff[0] * 0x100 + iRdBuff[1];
	gFMShadowRegMap[11] = iRdBuff[2] * 0x100 + iRdBuff[3];  
	
	return TRUE;;		
#endif
}
//
//-------------------------------------------------------------------
// Function:  FM_RDA5800_tune
// Purpose:  This function is used to tune the RDA5800
// In:	     Freq value	 
// Return:    boolean

//-------------------------------------------------------------------
bool  FM_RDA5800_tune(u16 iFrequency,bool isStereo) 
{
#ifdef FM_DRIVER_ON_LILY
	return MCI_FM_RDA5800_tune(iFrequency,isStereo);
#else	

	u8 iRdBuff[32],iWrBuff[4];
	u8 iCount = 0;

	// Reg 0x02
	// |15:DHIZ|14:DMUTE|13:MONO|12:BASS|11:LAB_MODE |10:RESV|9:SEEKUP|8:SEEK|
	// |7:XTALEN|6-1:RESV|0:ENABLE| 

	//  DHIZ=Audio out hi-z,DMUTE= Normal operation,MONO= stereo,BASS= Bass boost enabled
	//  LAB_MODE = only acces Reg 00H-0FH,XTAL= EN,ENABLE=EN

	// Update shadow register


	
	if(isStereo)
		gFMShadowRegMap[2] &=  ~0x2000;
	else
		gFMShadowRegMap[2] |=  0x2000;
		
	iWrBuff[0] = gFMShadowRegMap[2] >> 8;
	iWrBuff[1] = gFMShadowRegMap[2];

	
	// Reg 0x03
	// |15-8:CHAN|2:BAND|1:SPACE|
	gFMShadowRegMap[3] &=  ~0xff00;
	gFMShadowRegMap[3] |=  Fm_FreqToChan(iFrequency) << 8;
	iWrBuff[2] = gFMShadowRegMap[3] >> 8; 		 
	iWrBuff[3] = gFMShadowRegMap[3]; 
	
	
	
	if( !I2CSendData(RDA5800_I2C_ADDR,0,iWrBuff,4,&iCount,FALSE))
		return FALSE;	
	
	//wait 100ms for Tune complete (STC = 1) 
	FM_Delay(100);
	
		
	//read REG0A&0B	
#ifdef DEBUG

	//for(iCount = 0;iCount < 4;iCount++)
	//	SXS_TRACE(TSTDOUT,"=======iWrBuff[%d]=%x======= ",iCount,iWrBuff[iCount]);
	if (!I2CRecvData(RDA5800_I2C_ADDR,0,iRdBuff,32,FALSE)) 
		return FALSE;


#else	
	if (!I2CRecvData(RDA5800_I2C_ADDR,0,iRdBuff,4,FALSE)) 
		return FALSE;
#endif

	gFMShadowRegMap[10] = iRdBuff[0] * 0x100 + iRdBuff[1];
	gFMShadowRegMap[11] = iRdBuff[2] * 0x100 + iRdBuff[3];  
	
#if DEBUG
	gFMShadowRegMap[12] = iRdBuff[4]*0x100 + iRdBuff[5];
	gFMShadowRegMap[13] = iRdBuff[6]*0x100 + iRdBuff[7]; 
	gFMShadowRegMap[14] = iRdBuff[8]*0x100 + iRdBuff[9];
	gFMShadowRegMap[15] = iRdBuff[10]*0x100 + iRdBuff[11]; 
	gFMShadowRegMap[0]  = iRdBuff[12]*0x100 + iRdBuff[13];
	gFMShadowRegMap[1] = iRdBuff[14]*0x100 + iRdBuff[15]; 
	gFMShadowRegMap[2] = iRdBuff[16]*0x100 + iRdBuff[17];
	gFMShadowRegMap[3] = iRdBuff[18]*0x100 + iRdBuff[19]; 
	gFMShadowRegMap[4] = iRdBuff[20]*0x100 + iRdBuff[21];
	gFMShadowRegMap[5] = iRdBuff[22]*0x100 + iRdBuff[23]; 
	gFMShadowRegMap[6] = iRdBuff[24]*0x100 + iRdBuff[25];
	gFMShadowRegMap[7] = iRdBuff[26]*0x100 + iRdBuff[27]; 
	gFMShadowRegMap[8] = iRdBuff[28]*0x100 + iRdBuff[29];
	gFMShadowRegMap[9] = iRdBuff[30]*0x100 + iRdBuff[31];

	// Clear the STC with write operation,wait 50 ms
	if( !I2CSendData(RDA5800_I2C_ADDR,0,iWrBuff,2,&iCount,FALSE))
		return FALSE;
	FM_Delay(50);
#endif	
	return TRUE;
#endif
}




//
//-------------------------------------------------------------------
// Function:  FM_RDA5800_mute
// Purpose:  This function is used to set RDA5800 mute or not
// In:	     
// Return:    boolean

//-------------------------------------------------------------------	
bool FM_RDA5800_mute(bool isMute)
{
#ifdef FM_DRIVER_ON_LILY
	return MCI_FM_RDA5800_mute(isMute);
#else	

	u8  iWrBuff[2],iCount = 0;

	// Update shadow register	
	if(isMute)
		gFMShadowRegMap[2] &= ~0x4000; 		
	else
		gFMShadowRegMap[2] |= 0x4000; 	
		
	iWrBuff[0] = (gFMShadowRegMap[2] >> 8);
	iWrBuff[1] = (gFMShadowRegMap[2]);
		
	if( !I2CSendData(RDA5800_I2C_ADDR,0,iWrBuff,2,&iCount,FALSE))
		return FALSE;

	return TRUE;
#endif
}

//
//-------------------------------------------------------------------
// Function:  FM_RDA5800_getCurStatus
// Purpose:  This function is used to get the status of RDA5800
// In:	     
// Return:   RDA5800 status
// Note :     This function only return right value after the call of initial function 


//-------------------------------------------------------------------	
bool FM_RDA5800_getCurStatus(RDA5800_STATUS * pRDA5800Status)
{
	u8 iRdBuff[4];

	if ( (gFMShadowRegMap[3] & 0x0002) == 0x0000) 
	{
		pRDA5800Status->iLoFreqEdge = 875;
		pRDA5800Status->iHiFreqEdge = 1080;
	}
	else 
	{
		pRDA5800Status->iLoFreqEdge = 760;
		pRDA5800Status->iHiFreqEdge = 1080;
	}

	if (!I2CRecvData(RDA5800_I2C_ADDR,0,iRdBuff,4,FALSE)) 
		return FALSE;

	gFMShadowRegMap[10] = iRdBuff[0] * 0x100 + iRdBuff[1];
	gFMShadowRegMap[11] = iRdBuff[2] * 0x100 + iRdBuff[3];  

	pRDA5800Status->iCurFreq = FM_ChanToFreq(iRdBuff[1]);

	pRDA5800Status->isStereo = gFMShadowRegMap[2] & 0x2000 ? FALSE : TRUE;

	pRDA5800Status->iVol = (u8) (gFMShadowRegMap[5] & 0x000f);


	return TRUE;

}
//
//-------------------------------------------------------------------
// Function:  FM_RDA5800_setVolume
// Purpose:  This function is used to set RDA5800 mute or not
// In:	     
// Return:    boolean

//-------------------------------------------------------------------	
bool FM_RDA5800_setVolume(u8 iVol)
{
#ifdef FM_DRIVER_ON_LILY
	return MCI_FM_RDA5800_setVolume(iVol);
#else	

	u8 iWrBuff[8],iCount = 0;


	if(iVol >= 16)
		iVol = 0x0f;

	// We only adjust DAC gain 
	iVol |= 0xf0;

	// REG 0x02
	// |15:DHIZ|14:DMUTE|13:MONO|12:BASS|11:LAB_MODE |10:RESV|9:SEEKUP|8:SEEK|
	// |7:XTALEN|6-1:RESV|0:ENABLE| 

	//  DHIZ=Audio out hi-z,DUTE= Normal operation,MONO= stereo,BASS= Bass boost enabled
	//  LAB_MODE = acces all Reg,XTAL= EN,ENABLE= Disable
	
	// Set tune_freq_diff(bit 4 of REG 0x02), to avoid trigger tune
	//SXS_TRACE(TSTDOUT,"gFMShadowRegMap[2]: 0x%x\n",gFMShadowRegMap[2]);	
	//gFMShadowRegMap[2] = 0xd091;
	gFMShadowRegMap[2] |= 0x0010; 
	gFMShadowRegMap[5] &= 0xff00;
	gFMShadowRegMap[5] |= iVol;
	//SXS_TRACE(TSTDOUT,"gFMShadowRegMap[2]: 0x%x\n",gFMShadowRegMap[2]);
		
	iWrBuff[0] = (gFMShadowRegMap[2] >> 8);
	iWrBuff[1] = (gFMShadowRegMap[2]);
	iWrBuff[2] = (gFMShadowRegMap[3] >> 8);
	iWrBuff[3] = (gFMShadowRegMap[3]);
	iWrBuff[4] = (gFMShadowRegMap[4] >> 8);
	iWrBuff[5] = (gFMShadowRegMap[4]);
	iWrBuff[6] = (gFMShadowRegMap[5] >> 8);
	iWrBuff[7] = gFMShadowRegMap[5];

	if( !I2CSendData(RDA5800_I2C_ADDR,0,iWrBuff,8,&iCount,FALSE))
		return FALSE;

	// Update shadow register
	gFMShadowRegMap[2] &= ~0x0010; 

	//SXS_TRACE(TSTDOUT,"gFMShadowRegMap[2]: 0x%x\n",gFMShadowRegMap[2]);
	
	iWrBuff[0] = (gFMShadowRegMap[2] >> 8);
	iWrBuff[1] = (gFMShadowRegMap[2]);
	
	if( !I2CSendData(RDA5800_I2C_ADDR,0,iWrBuff,2,&iCount,FALSE))
		return FALSE;

	return TRUE;
#endif		
}


	
//
//-------------------------------------------------------------------
// Function:  FM_RDA5800_seek
// Purpose:  This function is used to seek a valid station
//                If it return true,a valid station is found and it's freq is store in iCurFreq
//                Else a valid station is not  found and return current freq for MMI use.
//                This function should be called until it returns ture.

// In:	     boolean isSeekUp  seek direction
// Return:    iCurFreq current freq in 100K unit
//                boolean

//-------------------------------------------------------------------	
bool  FM_RDA5800_seek(bool isSeekUp,u16 * iCurFreq) 
{
#ifdef FM_DRIVER_ON_LILY
	return MCI_FM_RDA5800_seek(isSeekUp);
#else	

	u8 iRdBuff[4],iWrBuff[8],iCount = 0;
	bool iRetVal = FALSE;
			
	*iCurFreq = 0;
	        
	if(!FM_SingleStepSeek(isSeekUp)) 
		return FALSE;

	// Read Reg 0x0a&0b
	if (!I2CRecvData(RDA5800_I2C_ADDR,0,iRdBuff,4,FALSE)) 
		return FALSE;
    	//Update shdow register	
	gFMShadowRegMap[10] = iRdBuff[0] * 0x100 + iRdBuff[1];
	gFMShadowRegMap[11] = iRdBuff[2] * 0x100 + iRdBuff[3];

	*iCurFreq = FM_ChanToFreq(iRdBuff[1]);

	
	
	//check whether SF=1
	if((iRdBuff[0] & 0x20) == 0) 
	//check station RSSI again, if RSSI<SEEKTH, then falseStation, continue seek
	if(iRdBuff[2] >= (gFMShadowRegMap[5]>>8)) 
	{
		iRetVal = TRUE;
		

#if DEBUG
//		SXS_TRACE(TSTDOUT,"=====================================");
//		SXS_TRACE(TSTDOUT,"gFMShadowRegMap[2]: 0x%x\n",gFMShadowRegMap[2]);	

//		SXS_TRACE(TSTDOUT,"gFMShadowRegMap[5]: 0x%x\n",gFMShadowRegMap[5]);

//		SXS_TRACE(TSTDOUT,"iRdBuff[2]: 0x%x\n",iRdBuff[2]);
//		SXS_TRACE(TSTDOUT,"iCurFreq: %d\n",*iCurFreq);
//		SXS_TRACE(TSTDOUT,"=====================================");
#endif 	

	}
	// Update the shadow register
	gFMShadowRegMap[3] &=  ~0xff00;
	gFMShadowRegMap[3] |=  iRdBuff[1] << 8;

	// Clear the STC
	gFMShadowRegMap[2] &= ~0x0300;

	
 	iWrBuff[0] = gFMShadowRegMap[2] >> 8;
	iWrBuff[1] = gFMShadowRegMap[2];
	if( !I2CSendData(RDA5800_I2C_ADDR,0,iWrBuff,2,&iCount,FALSE))
		return FALSE;
	FM_Delay(50);

	return iRetVal;
#endif		
				
	
		
}


//
//-------------------------------------------------------------------
// Function:  FM_RDA5800_autoSeek
// Purpose:  This function is used to start a auto seek action
// In:	     pNumOfStation, The count of seek stations,
//                pStationBuff, the frequency of seek stations
// Return:    boolean

//-------------------------------------------------------------------		
bool  FM_RDA5800_autoSeek (u8 *pNumOfStation,u16 *pStationBuff) 
{

	u16 iFrequency = 0,iCurFreq = 0;
	u16 iCount = 0;
	u16 iOldFreq = 0, iNewFreq = 0;
	
	
	// Tune to the bottom of the band
	iFrequency = FM_ChanToFreq(0);
	if (!FM_RDA5800_tune(iFrequency,FALSE))
		return FALSE;
	
	// Seek through the band
       iNewFreq = iFrequency;
	while(1)
	{
		
		if(FM_RDA5800_seek(TRUE,&iCurFreq))
		{
			pStationBuff[iCount] = iCurFreq;
			iCount++;		
		}

		iOldFreq = iNewFreq;
		iNewFreq = iCurFreq;
              // Wrapped
		if(iNewFreq <= iOldFreq)
			break;

		//sxr_Sleep(1000);
		
	}

	*pNumOfStation = iCount;
	return TRUE;

	
}

//
//-------------------------------------------------------------------
// Function:  FM_RDA5800_I2S
// Purpose:  This function is used to open RDA5800 i2s
// In:	     
// Return:    boolean

//-------------------------------------------------------------------	
bool FM_RDA5800_I2S(void)
{
#ifdef FM_DRIVER_ON_LILY
	return MCI_FM_RDA5800_I2S();
#else
	u8 iWrBuff[10],iCount = 0;

	gFMShadowRegMap[2] |= 0x0400; 
	gFMShadowRegMap[4] |= 0x0740; 
	gFMShadowRegMap[6] |= 0x02f3;

	iWrBuff[0] = (gFMShadowRegMap[2] >> 8);
	iWrBuff[1] = (gFMShadowRegMap[2]);
	iWrBuff[2] = (gFMShadowRegMap[3] >> 8);
	iWrBuff[3] = (gFMShadowRegMap[3]);
	iWrBuff[4] = (gFMShadowRegMap[4] >> 8);
	iWrBuff[5] = (gFMShadowRegMap[4]);
	iWrBuff[6] = (gFMShadowRegMap[5] >> 8);
	iWrBuff[7] = (gFMShadowRegMap[5]);
	iWrBuff[8] = (gFMShadowRegMap[6] >> 8);
	iWrBuff[9] = (gFMShadowRegMap[6]);	

	if( !I2CSendData(RDA5800_I2C_ADDR,0,iWrBuff,10,&iCount,FALSE))
		return FALSE;

	return TRUE;
#endif
		
}

//
//-------------------------------------------------------------------
// Function:  FM_RDA5800_STOPI2S
// Purpose:  This function is used to disable RDA5800 i2s
// In:	     
// Return:    boolean

//-------------------------------------------------------------------	
bool FM_RDA5800_STOPI2S(void)
{
#ifdef FM_DRIVER_ON_LILY
	return TRUE;
	//return MCI_FM_RDA5800_STOPI2S();
#else

	u8 iWrBuff[10],iCount = 0;

	gFMShadowRegMap[2] &= ~0x0400; 
	gFMShadowRegMap[4] &= ~0x0740; 
	gFMShadowRegMap[6] &= ~0x02f3;

	iWrBuff[0] = (gFMShadowRegMap[2] >> 8);
	iWrBuff[1] = (gFMShadowRegMap[2]);
	iWrBuff[2] = (gFMShadowRegMap[3] >> 8);
	iWrBuff[3] = (gFMShadowRegMap[3]);
	iWrBuff[4] = (gFMShadowRegMap[4] >> 8);
	iWrBuff[5] = (gFMShadowRegMap[4]);
	iWrBuff[6] = (gFMShadowRegMap[5] >> 8);
	iWrBuff[7] = (gFMShadowRegMap[5]);
	iWrBuff[8] = (gFMShadowRegMap[6] >> 8);
	iWrBuff[9] = (gFMShadowRegMap[6]);	

	if( !I2CSendData(RDA5800_I2C_ADDR,0,iWrBuff,10,&iCount,FALSE))
		return FALSE;

	return TRUE;
#endif
		
}


