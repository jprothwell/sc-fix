/*************************************************************************/
/*                                                                       */
/*              Copyright (C) 2007, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                        */
/*                                                                       */
/*  This source code is the property of Coolsand Technologies and is     */
/*  confidential. Any modification, distribution, reproduction or        */
/*  exploitation of any content of this file is totally forbidden,       */
/*  except with the written permission of Coolsand Technologies.         */
/*                                                                       */
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      xcv.c                                                            */
/*                                                                       */
/* DESCRIPTION                                                           */
/*      This file contain the RDA6220  related functions.                */
/*                                                                       */
/*************************************************************************/

#include "cs_types.h"
#include "hal_rfspi.h"
#include "hal_tcu.h"
#include "hal_ana_afc.h"
#include "hal_sys.h"
#include "hal_timers.h"
#include "drv_xcv.h"
#include "drv_xcv_calib.h"
#include "sxs_io.h"
#include "rfd_cfg.h"
#include "rfd_xcv.h"
#include "rfd_defs.h"
#include "gsm.h"
#include "baseband_defs.h"

#include "calib_m.h"

#if ((CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE) || (CHIP_ASIC_ID == CHIP_ASIC_ID_ESPERITE))
 #define XCV_DIGRF_RX_CLK_POL                      RFD_DIGRF_INV_CLK_POL
 #define XCV_SAMPLE_WIDTH                                             12
 #define XCV_ADC2DBM_GAIN                                             58
#else
 #define XCV_DIGRF_RX_CLK_POL                     RFD_DIGRF_NORM_CLK_POL
 #define XCV_SAMPLE_WIDTH                                             16
 #define XCV_ADC2DBM_GAIN                                             70
#endif

#define XCV_SET_PDN                                   SET_TCO(XCV->PDN)
#define XCV_CLR_PDN                                   CLR_TCO(XCV->PDN)


// Forward declaration of internal functions
VOID xcvBuildSpiCmd (HAL_RFSPI_CMD_T *cmd, UINT8 address, UINT32 data);
VOID xcvSetArfcn (GSM_RFBAND_T dir, GSM_RFBAND_T band, UINT16 arfcn);
VOID xcvReadReg (UINT16 address, UINT8 *output);

INLINE VOID xcvConfigure(VOID);
CONST XCV_CONFIG_T* XCV;

# if 0
UINT32 g_tmp1 =0;
UINT32 g_tmp2 =0;
UINT32 g_tmp3 =0;
UINT32 g_tmp4 =0;
UINT32 g_tmp5 =0;
#endif

//--------------------------------------------------------------------//
//                                                                    //
//                        Internal Structures                         //
//                                                                    //
//--------------------------------------------------------------------//

// GLOBAL variables
INT32 g_CDAC;
INT32 g_AfcOffset = 0;
UINT16 g_afcDacVal = RDA6220_AFC_MID;
RFD_LP_MODE_T g_lastSleepMode = RFD_POWER_OFF;

PROTECTED CALIB_CALIBRATION_T* g_xcvCalibPtr     = 0 ;

CONST XCV_RX_GAIN_T AgcTab_EGSM[AGC_QTY]       = EGSM_AGC_DEFAULT;
CONST XCV_RX_GAIN_T AgcTab_DCS[AGC_QTY]        = DCS_AGC_DEFAULT;
CONST XCV_APC_T     ApcTab_GSM850[APC850_QTY]  = GSM_850_APC;
CONST XCV_APC_T     ApcTab_GSM900[APC900_QTY]  = GSM_900_APC;  

// RF Commands Context
XCV_COMMANDS_T g_xcvCmdCtx = {0, 0, 0};


// -------------------------------------------------------------------//
//                      Macros for SPI timings                        //
// -------------------------------------------------------------------//
// 1 Qb = 928 ns
#ifndef __RF_SPI_SLOW_CLK__
// 1 Cmd = 35 clocks (i.e. 1346 ns @26MHz)
//       = 1.45 Qb @26
#define CMD_DURATION(n) (((n) * 146) / 100 +1)
#else // 6.5M
// 1 Cmd = 35 clocks (i.e. 5385 ns @6.5MHz)
//       = 5.08 Qb @26
#define CMD_DURATION(n) (((n) * 508) / 100 +1)
#endif

//--------------------------------------------------------------------//
//                                                                    //
//                RF transceiver Serial link management               //
//                                                                    //
//--------------------------------------------------------------------//

// Generates commands according to the Xcver SPI serial format.
//
// Rda6220 RF Transceiver:
//  1 write command is:
//  1 bit equal to 0 for write + 18 bits for data + 6 bits for address = 25 bits
//           0AAAAAAD | DDDDDDDD | DDDDDDDD | D0000000
//      MSB    Byte0     Byte 1     Byte 2      Byte 3  LSB
//      
//  1 read command is:
//  1 bit equal to 1 for read + 6 bits for address + 2 bits for clk turnaround + 18 bits for data = 27 bits
//           1AAAAAAD | DDDDDDDD | DDDDDDDD | D0000000
//      MSB    Byte0     Byte 1       Byte 2     Byte 3  LSB

VOID xcvBuildSpiCmd (HAL_RFSPI_CMD_T *cmd, UINT8 address, UINT32 data)
{
    UINT8 array[RDA6220_ONE_CMD_BYTE_QTY];
    UINT32 cmdWord = RDA6220_WRITE_FMT(address, data);

    // Hardcoded
    array[0] = (cmdWord >> (3 * BYTE_SIZE)) & BYTE_MASK;
    array[1] = (cmdWord >> (2 * BYTE_SIZE)) & BYTE_MASK;
    array[2] = (cmdWord >> (BYTE_SIZE)) & BYTE_MASK;
    array[3] =  cmdWord & BYTE_MASK;

    hal_RfspiPushData(cmd,array,RDA6220_ONE_CMD_BYTE_QTY);
}

VOID xcvReadReg (UINT16 address, UINT8 *output)
{
    UINT8  array[RDA6220_ONE_CMD_BYTE_QTY];
    UINT32 cmdWord;
    
    cmdWord = RDA6220_READ_FMT(address, 0);

    array[0] = (cmdWord >> (3 * BYTE_SIZE)) & BYTE_MASK;
    array[1] = (cmdWord >> (2 * BYTE_SIZE)) & BYTE_MASK;
    array[2] = (cmdWord >> (BYTE_SIZE)) & BYTE_MASK;
    array[3] =  cmdWord & BYTE_MASK;
    hal_RfspiDigRfRead(array,output);
}


//--------------------------------------------------------------------//
//                                                                    //
//                        RF Commands Generation                      //
//                                                                    //
//--------------------------------------------------------------------//
INT16 Pcl2dBm (GSM_RFBAND_T band,UINT16 arfcn,UINT8 Pcl)
{
    INT16 dBm_level;
    UINT16 nb_arfcn_in_band;
    CALIB_ARFCN_T *pcl2dbm_o_arfcn;
    INT16 arfcn_s = arfcn;

    if ((band == GSM_BAND_GSM900)||(band == GSM_BAND_GSM850))
    {
        // GSM
        pcl2dbm_o_arfcn = (CALIB_ARFCN_T *)
                            g_xcvCalibPtr->pa->palcust.pcl2dbmArfcnG;
        // GSM-P band (0->124)
        if (    (arfcn_s >= RFD_ARFCN_EGSM_RX_MIN1) && 
                (arfcn_s <= RFD_ARFCN_EGSM_RX_MAX1))
        {
            arfcn_s -= RFD_ARFCN_EGSM_RX_MIN1;
        }
        // GSM-E band (975->1023)
        else if (   (arfcn >= RFD_ARFCN_EGSM_TX_MIN3) &&
                    (arfcn <= RFD_ARFCN_EGSM_TX_MAX3))
        {
            arfcn_s -= (RFD_ARFCN_EGSM_TX_MAX3 + 1);
        }
        // TODO: Tx offset not handled for now in GSM850 (set to the min)
        // GSM850 (128->251)
        else if((arfcn_s >= RFD_ARFCN_GSM850_MIN) &&
                (arfcn_s <= RFD_ARFCN_GSM850_MAX))
        {
            arfcn_s = RFD_ARFCN_EGSM_RX_MAX1;
        }
        nb_arfcn_in_band = RFD_ARFCN_EGSM_RX_MAX1 - RFD_ARFCN_EGSM_RX_MIN1;
            
        if (Pcl < 5)
            Pcl = 5; 
        else if (Pcl > 19)
            Pcl = 19;
        Pcl -= 5;
    }
    else if (band == GSM_BAND_DCS1800)
    {
        // DCS
        pcl2dbm_o_arfcn = (CALIB_ARFCN_T *)
                            g_xcvCalibPtr->pa->palcust.pcl2dbmArfcnD;
        arfcn_s -= RFD_ARFCN_DCS_MIN;
        nb_arfcn_in_band = RFD_ARFCN_DCS_MAX - RFD_ARFCN_DCS_MIN;
            
        if (Pcl >= 29)
        {
            Pcl = CALIB_DCS_PCL_QTY - 1;
        }
        else
        {
            if (Pcl > 15) Pcl = 15;
        }
    }
    else
    {
        // PCS
        pcl2dbm_o_arfcn = (CALIB_ARFCN_T *)
                            g_xcvCalibPtr->pa->palcust.pcl2dbmArfcnP;
        arfcn_s -= RFD_ARFCN_PCS_MIN;
        nb_arfcn_in_band = RFD_ARFCN_PCS_MAX - RFD_ARFCN_PCS_MIN;
            
        if ((Pcl >=22) && (Pcl <= 30))
        {
            Pcl = CALIB_PCS_PCL_QTY - 2;
        }
        else if (Pcl == 31)
        {
            Pcl = CALIB_PCS_PCL_QTY - 1;
        }
        else
        {
            if (Pcl > 15) Pcl = 15;
        }
    }
    dBm_level = pcl2dbm_o_arfcn[Pcl][0] + // Min level for lower PCL
            ((arfcn_s * (pcl2dbm_o_arfcn[Pcl][1] - pcl2dbm_o_arfcn[Pcl][0]) /
              nb_arfcn_in_band)); // Linear interpolation upon arfcn

    return dBm_level;
}




UINT32 g_If = 0x4000;

VOID xcvSetArfcn (GSM_RFBAND_T dir, GSM_RFBAND_T band, UINT16 arfcn)
{
    // Configure register 08h and 30h
    // FIXE ME (30H or 08 needs to be configured??)

    // If rx set the 11th bit, if tx the 10th bit
    UINT16 rxTxMode = (dir == RFD_DIR_RX)?0x800:0x400; 

    // According to the band set the corresponding bit
    UINT32 selBand  = (band == GSM_BAND_GSM850)  ? 0x8000 :
                      (band == GSM_BAND_GSM900)  ? 0x4000 :
                      (band == GSM_BAND_DCS1800) ? 0x2000 :
                                                   0x1000 ; // GSM_BAND_PCS1900

    // 16 bit set by default for 08H
    g_xcvCmdCtx.arfcn = (0x10000 | selBand | rxTxMode | (arfcn & RDA6220_ARFCN_MASK)); 
    
    // Value for 30h
    g_xcvCmdCtx.band  = (0x0a200| (selBand >> 8) | (rxTxMode >> 8)) | 0x1 | g_If;  // TODO: Use define for rst bit
                

}

INLINE
UINT8 xcvSetRxGain (UINT8 gain, GSM_RFBAND_T band, UINT16 arfcn)
{
    UINT8 iLoss = 0;
    UINT8 XcvGain = 0;
    UINT8 GainIdx;
    CONST XCV_RX_GAIN_T* agcTable=  ((band == GSM_BAND_GSM850)|| (band == GSM_BAND_GSM900))?
                                    AgcTab_EGSM:
                                    AgcTab_DCS;

    
    
    switch (band)
    {
    	    	// GSM850 uses 5 points          128  -- 251

        case GSM_BAND_GSM850:
            if (arfcn > 128 && arfcn < 141 ) // 128 - 140
                iLoss = XCV_PARAM(ILOSS(band))& 0xf;
            else if (arfcn < 166 && arfcn > 140) // 141 - 165
                iLoss = (XCV_PARAM(ILOSS(band)) >> 4)& 0xf;
    	    else if (arfcn < 191 && arfcn > 165) // 166 - 190
    		iLoss = (XCV_PARAM(ILOSS(band)) >> 8)& 0xf;
    	    else if (arfcn < 226 && arfcn > 190) // 191 - 225
    		iLoss = (XCV_PARAM(ILOSS(band)) >> 12)& 0xf;
    	    else 
    	        iLoss = (XCV_PARAM(ILOSS(band)) >> 16)& 0xf;;
        break;
        // GSM uses 5 points
        case GSM_BAND_GSM900:
            if (arfcn < 42) // 0-40
                iLoss = XCV_PARAM(ILOSS(band))& 0xf;
            else if (arfcn < 107 && arfcn > 41) // 41-82
                iLoss = (XCV_PARAM(ILOSS(band)) >> 4)& 0xf;
    	    else if (arfcn < 125 && arfcn > 106) // 83-124
    		iLoss = (XCV_PARAM(ILOSS(band)) >> 8)& 0xf;
    	    else if (arfcn < 1006 && arfcn > 974) // 975-999
    		iLoss = (XCV_PARAM(ILOSS(band)) >> 12)& 0xf;
    	    else // 1000-1023
    		iLoss = (XCV_PARAM(ILOSS(band)) >> 16)& 0xf;;
        break;
        // DCS uses 8 points
        case GSM_BAND_DCS1800:
            if (arfcn < 553) // 512-558
                iLoss = XCV_PARAM(ILOSS(band))& 0xf;
            else if (arfcn < 583 && arfcn > 552) // 559-604
                iLoss = (XCV_PARAM(ILOSS(band)) >> 4)& 0xf;
    	    else if (arfcn < 663 && arfcn > 582) // 605-650
    		iLoss = (XCV_PARAM(ILOSS(band)) >> 8)& 0xf;	
    	    else if (arfcn < 693 && arfcn > 662) // 651-696
    		iLoss = (XCV_PARAM(ILOSS(band)) >> 12)& 0xf;
    	    else if (arfcn < 718 && arfcn > 692) // 697-742
    		iLoss = (XCV_PARAM(ILOSS(band)) >> 16)& 0xf;
    	    else if (arfcn < 735 && arfcn > 717) // 743-788
    		iLoss = (XCV_PARAM(ILOSS(band)) >> 20)& 0xf;
    	    else if (arfcn < 848 && arfcn > 734) // 789-834
    		iLoss = (XCV_PARAM(ILOSS(band)) >> 24)& 0xf;
    	    else // 835-885
    		iLoss = (XCV_PARAM(ILOSS(band)) >> 28)& 0xf;
    	break;
    	// PCS uses 8 points
    	case GSM_BAND_PCS1900:
            if (arfcn < 550) // 512-549
                iLoss = XCV_PARAM(ILOSS(band))& 0xf;
            else if (arfcn < 587 && arfcn > 549) // 550-586
                iLoss = (XCV_PARAM(ILOSS(band)) >> 4)& 0xf;
    	    else if (arfcn < 623 && arfcn > 586) // 587-622
    		iLoss = (XCV_PARAM(ILOSS(band)) >> 8)& 0xf;	
    	    else if (arfcn < 660 && arfcn > 622) // 623-659
    		iLoss = (XCV_PARAM(ILOSS(band)) >> 12)& 0xf;
    	    else if (arfcn < 697 && arfcn > 659) // 660-696
    		iLoss = (XCV_PARAM(ILOSS(band)) >> 16)& 0xf;
    	    else if (arfcn < 734 && arfcn > 696) // 697-733
    		iLoss = (XCV_PARAM(ILOSS(band)) >> 20)& 0xf;
    	    else if (arfcn < 771 && arfcn > 733) // 734-770
    		iLoss = (XCV_PARAM(ILOSS(band)) >> 24)& 0xf;
    	    else // 771-810
    		iLoss = (XCV_PARAM(ILOSS(band)) >> 28)& 0xf;
    	break;
    	
    	default:
    	break;
    }

    if (gain == RFD_AGC)
    {
        // FIXE ME: What is XCVER_AGC_TARGET
        gain = XCVER_AGC_TARGET + iLoss;
    }
    else
    {
        gain += iLoss;
        // Boundaries check
        gain = ((gain>RDA6220_MIN_IN_POWER)?RDA6220_MIN_IN_POWER:gain);
        gain = ((gain<RDA6220_MAX_IN_POWER)?RDA6220_MAX_IN_POWER:gain);
    }
		

    GainIdx = (gain-15);  
             
    XcvGain = agcTable[GainIdx].totGain; 
        
    g_xcvCmdCtx.digGain    =  agcTable[GainIdx].digGain;
    g_xcvCmdCtx.analogGain =  agcTable[GainIdx].analogGain;         

       
    return (XcvGain - iLoss);
}

//--------------------------------------------------------------------//
//                                                                    //
//                    Internal Functions                              //
//                                                                    //
//--------------------------------------------------------------------//

// *** xcvBuildRxSpiCmd (UINT16 arfcn, UINT8 gain) ***
// Prepare the transceiver commands for reception
INLINE
UINT8 xcvBuildRxSpiCmd(UINT16 arfcn, GSM_RFBAND_T band, UINT8 gain, HAL_RFSPI_CMD_T *spiCmd)
{
    UINT8 XcvGain;

    // Set arfcn & band
    xcvSetArfcn (RFD_DIR_RX, band, arfcn);
    
    // Set Gain
    XcvGain = xcvSetRxGain (gain, band, arfcn);

    // Set DigitalAfc, 14 bit is set for digital AFC
    g_xcvCmdCtx.digAfc = (g_afcDacVal & RDA6220_AFC_MASK);

    // set page = 0
    xcvBuildSpiCmd (spiCmd, RDA6220_PAGE_REG,   0x00000);
    // 30h
    xcvBuildSpiCmd (spiCmd, RDA6220_SYS_CTRL_REG,    g_xcvCmdCtx.band);
    // 08h
    xcvBuildSpiCmd (spiCmd, RDA6220_FREQ_CTRL_REG,   g_xcvCmdCtx.arfcn); 
    // 01h
    xcvBuildSpiCmd (spiCmd, RDA6220_ANALOG_GAIN_REG, (0x23f84 | ((g_xcvCmdCtx.analogGain& 0x7) << 14)));
    
    // 02h
    xcvBuildSpiCmd (spiCmd, RDA6220_DIG_GAIN_REG,    (0x0f00 | g_xcvCmdCtx.digGain));
    
    xcvBuildSpiCmd (spiCmd, RDA6220_TX_GAIN_SET_REG, 0x03008); // TxGainSetting

    // 1Fh
    xcvBuildSpiCmd (spiCmd, RDA6220_AFC_REG,         (0x14000 | g_xcvCmdCtx.digAfc));

    // FIXE ME: Do i need the following configuration?
    //xcvBuildSpiCmd (spiCmd, 0x3f,   0x00002);
    //xcvBuildSpiCmd (spiCmd, 0x96,   0x01fcd); // Bypass tx filter
    //xcvBuildSpiCmd (spiCmd, 0x3f,   0x00000);



    //RFD_TRACE(TSTDOUT,0,"Write  0x%x @ 0x%x", g_xcvCmdCtx.band ,RDA6220_SYS_CTRL_REG);
    //RFD_TRACE(TSTDOUT,0,"Write  0x%x @ 0x%x", g_xcvCmdCtx.arfcn, RDA6220_FREQ_CTRL_REG);
    //RFD_TRACE(TSTDOUT,0,"Write  0x%x @ 0x%x", (0x23f84 | ((g_xcvCmdCtx.analogGain& 0x7) << 14)),
    //                                          RDA6220_ANALOG_GAIN_REG);
    //RFD_TRACE(TSTDOUT,0,"Write  0x%x @ 0x%x", (0x0f00 | g_xcvCmdCtx.digGain), RDA6220_DIG_GAIN_REG);
    //RFD_TRACE(TSTDOUT,0,"Write  0x%x @ 0x%x", (0x14000 | g_xcvCmdCtx.digAfc), RDA6220_AFC_REG);
   
    //RFD_TRACE(TSTDOUT,0,"Write  0x00002 @ 0x3f ");
    //RFD_TRACE(TSTDOUT,0,"Write  0x01fcd @ 0x96 ");
    //RFD_TRACE(TSTDOUT,0,"Write  0x00000 @ 0x3f ");

    return XcvGain;

}

// *** xcvBuildTxSpiCmd  ***
// Prepare the transceiver commands for emission
INLINE
VOID xcvBuildTxSpiCmd (RFD_WIN_T *Win, UINT16 arfcn, GSM_RFBAND_T band, HAL_RFSPI_CMD_T *spiCmd)
{
   
    CONST XCV_APC_T* pApcTable = (band == GSM_BAND_GSM850)? ApcTab_GSM850 : ApcTab_GSM900;
    UINT16 i = 0;
    INT16  dBm = 0 ;
    UINT8  TabRampReg[5] = {0x3b, 0x3c, 0x3d, 0x3e, 0x3a};
    UINT16 rampFactor[4] = {0};

    UINT8 WinBitMap = Win->bitmap;
   
       
    // Set DigitalAfc, 14 bit is set for digital AFC
    g_xcvCmdCtx.digAfc     = (g_afcDacVal & RDA6220_AFC_MASK);

    // Set Band & arfcn
    xcvSetArfcn (RFD_DIR_TX, band, arfcn);

    xcvBuildSpiCmd (spiCmd, RDA6220_PAGE_REG,                    0x00000); // page =0
    // 30h
    xcvBuildSpiCmd (spiCmd, RDA6220_SYS_CTRL_REG,    g_xcvCmdCtx.band);
    // 08h
    xcvBuildSpiCmd (spiCmd, RDA6220_FREQ_CTRL_REG,   g_xcvCmdCtx.arfcn);
    // 1Fh
    xcvBuildSpiCmd (spiCmd, RDA6220_AFC_REG,         (0x14000 | g_xcvCmdCtx.digAfc));
   
    //xcvBuildSpiCmd (spiCmd, RDA6220_DIG_GAIN_REG,    ((g_xcvCmdCtx.digGain & 0xf) << 8));
    xcvBuildSpiCmd (spiCmd, RDA6220_DIG_GAIN_REG,    0x00310);

    xcvBuildSpiCmd (spiCmd, RDA6220_TX_GAIN_SET_REG, 0x03008); // TxGainSetting

   
    dBm = Pcl2dBm(band, arfcn, Win->gain[0])/100;
    g_xcvCmdCtx.dacBit  = ((pApcTable[dBm-5].dacBit & RDA6220_RAMP_DAC_MASK) << RDA6220_RAMP_DAC_OFFSET);
    xcvBuildSpiCmd (spiCmd, 0x07, g_xcvCmdCtx.dacBit | 0x3ff);
    
    xcvBuildSpiCmd (spiCmd, RDA6220_PAGE_REG,                    0x00001); // page=1

    

    while(WinBitMap)
    {
         // If active slot
         if (WinBitMap & 0x1)
         {
            dBm = Pcl2dBm(band, arfcn, Win->gain[i])/100;
            rampFactor[i] = pApcTable[dBm-5].rampSet;
 
            // If first slot or previous slot not active
            if ((i == 0) || (!(Win->bitmap & (1 << (i-1)))))
            {
                g_xcvCmdCtx.rampSet=(rampFactor[i] & RDA6220_RAMP_FACTOR_MASK)<<RDA6220_RAMP_FACTOR_OFFSET;
                
            }       
            else // If previous slot active
            {
                g_xcvCmdCtx.rampSet = (rampFactor[i-1] > rampFactor[i])?
                   (1<<14) | (( (rampFactor[i-1]-rampFactor[i]) & RDA6220_RAMP_FACTOR_MASK) 
                                                               << RDA6220_RAMP_FACTOR_OFFSET):
                   (((rampFactor[i] - rampFactor[i-1]) & RDA6220_RAMP_FACTOR_MASK) 
                                                               << RDA6220_RAMP_FACTOR_OFFSET);
            } 

         }
         else // if current slot not active
         {
            g_xcvCmdCtx.rampSet =   (1 << 14) | ((rampFactor[i-1] & RDA6220_RAMP_FACTOR_MASK)
                                                               << RDA6220_RAMP_FACTOR_OFFSET);
         }

         xcvBuildSpiCmd (spiCmd, TabRampReg[i], g_xcvCmdCtx.rampSet);

         WinBitMap >>= 1;
         i++;

    }

    // That was last slot active => put ramp down.
    g_xcvCmdCtx.rampSet =   (1 << 14) | ((rampFactor[i-1] & RDA6220_RAMP_FACTOR_MASK) 
                                                         << RDA6220_RAMP_FACTOR_OFFSET);

    xcvBuildSpiCmd (spiCmd, TabRampReg[i], g_xcvCmdCtx.rampSet);
  
    xcvBuildSpiCmd (spiCmd, RDA6220_PAGE_REG, 0x00000); // page=0  
                           
}


//--------------------------------------------------------------------//
//                                                                    //
//               Functions for GLOBAL PAL API                         //
//                                                                    //
//--------------------------------------------------------------------//

RFD_XCV_PARAMS_T g_xcvParams =
{
    // Misc Modem params
    .dcocMode= RFD_SW_DCOC_ENABLED,
    //.dcocMode= RFD_NO_DCOC,
    .lowRla = 100,
    .dtxTcoSettings = 0,
    .rxOnTcoSettings = 0,
    
    // PowerUp params
    .wakeUpFrameNbr = 0, // 1, // activating low power optimisation
    .pu2VcoEnDelay = 1,
    .vcoEn2VcoStableDelay = 60,
    .vrfBehaviour = RFD_LDO_BEHAVIOR_ALWAYS_HIGH,

    // Window bounds
    // NOTE : these timings will be wrong if changed by a calibration
    // file, they should be updated at execution time
    .winBounds = {
        .rxSetup = DEFAULT_CALIB_SPI_CMD - CMD_DURATION(7),
        .rxHold  = DEFAULT_CALIB_PDN_RX_DN,
        .txSetup = DEFAULT_CALIB_SPI_CMD - CMD_DURATION(11),
        .txHold  = DEFAULT_CALIB_PDN_TX_DN
    },

    // DigRf params
    .digRf = {
        .used           = TRUE,
        .rxRate         = RFD_DIGRF_1_SAMPLE_PER_SYMB,
        .rxSampleWidth  = XCV_SAMPLE_WIDTH,
        .rxSampleAlign  = RFD_DIGRF_SAMPLE_ALIGN_MSB,
        .rxAdc2DbmGain  = XCV_ADC2DBM_GAIN,
        .rxClkPol       = XCV_DIGRF_RX_CLK_POL,
        .txClkPol       = RFD_DIGRF_INV_CLK_POL,
#ifdef __TX_BLOCK_MODE__
        .txMode         = RFD_DIGRF_TX_BLOCK
#else
        .txMode         = RFD_DIGRF_TX_STREAM
#endif
    },

    .txBstMap = {
        .preamble  = 4,
        .guardStart= 4,
        .postamble = 4
    }

};

const RFD_XCV_PARAMS_T* rfd_XcvGetParameters(VOID)
{

    // Get the calibration pointer
    g_xcvCalibPtr = calib_GetPointers();

    g_xcvParams.dtxTcoSettings    = MASK_TCO(XCV->PDN) | HAL_TCU_MASK_TCO_GMSK_ON | HAL_TCU_MASK_DIGRF_STROBE | HAL_TCU_MASK_RF_SPI_MARKED_CMD;
    g_xcvParams.rxOnTcoSettings = MASK_TCO(XCV->PDN) | HAL_TCU_MASK_TCO_RX_DEC_ON;
    g_xcvParams.winBounds.rxSetup = XCV_TIME(SPI_CMD) - CMD_DURATION(7);
    g_xcvParams.winBounds.rxHold  = XCV_TIME(PDN_RX_DN);
    g_xcvParams.winBounds.txSetup = XCV_TIME(SPI_CMD) - CMD_DURATION(11);
    g_xcvParams.winBounds.txHold  = XCV_TIME(PDN_TX_DN);
    return &g_xcvParams;
}

const HAL_RFSPI_CFG_T g_spiCfg = {
    TRUE,  // Enable
    TRUE, // csActiveHi
    TRUE,  // modeDigRfRead
    FALSE, // modeClockBack2Back
    FALSE, // inputEn
    FALSE, // clkFallEdge
    HAL_RFSPI_HALF_CLK_PERIOD_1, // clkDelay
    HAL_RFSPI_HALF_CLK_PERIOD_2, // doDelay
    HAL_RFSPI_HALF_CLK_PERIOD_1, // diDelay
    HAL_RFSPI_HALF_CLK_PERIOD_2, // csDelay
    HAL_RFSPI_HALF_CLK_PERIOD_0, // csEndHold
    HAL_RFSPI_HALF_CLK_PERIOD_2, // csEndPulse
    2,  // turnAroundCycles
    24, // frameSize
    17, // inputFrameSize
}; 



//-----------------------------//
// Window Open/Close Functions //
//-----------------------------//
UINT8 rfd_XcvRxOn(RFD_WIN_T *Win, INT16 start)
{
    UINT8 XcvGain;
    HAL_RFSPI_CMD_T *spiCmd;
    UINT8 i =0;   
    UINT8 Winbitmap = (Win->bitmap & 0xf) << 1;
    UINT8 nbSlots = (Winbitmap & 0x10)? 4:
                    (Winbitmap & 0x8)? 3:
                    (Winbitmap & 0x4)? 2: 1;

    RFD_TRACE(RFD_INFO_TRC,0,"xcvRxOn @ %d Qb",start);
    
    // Allocate the spi command 
    spiCmd = hal_RfspiNewCmd(  (start + XCV_TIME(SPI_CMD) - CMD_DURATION(7)),
                                Win->winIdx );

    // Build the command
    XcvGain = xcvBuildRxSpiCmd (    Win->arfcn,
                                    Win->band,
                                    Win->gain[0],
                                    spiCmd);

    for(i =0; i<nbSlots; i++)
    {  

        // If current slot active
        if (Winbitmap & 0x2)
        {

            //If previous slot not active
            if(!(Winbitmap & 0x1))
            { 
                hal_TcuSetEvent(HAL_TCU_START_RFIN_RECORD, start + XCV_TIME(PDN_RX_UP));
                hal_TcuSetEvent (XCV_SET_PDN, start + XCV_TIME(PDN_RX_UP));
            }
        }
        else
        {
             hal_TcuSetEvent(XCV_CLR_PDN, start + XCV_TIME(PDN_RX_DN));
             hal_TcuSetEvent(HAL_TCU_STOP_RFIN_RECORD, start + XCV_TIME(PDN_RX_DN));

        }       
                            
        Winbitmap >>= 1;  
        start += GSM_SLOT_QS_DURATION;
    }

    Win->swapIQ = FALSE;

  
    return XcvGain;
}

VOID rfd_XcvRxOff(RFD_WIN_T *Win,INT16 stop)
{
    // Transceiver in PowerDown
    hal_TcuSetEvent (XCV_CLR_PDN, stop + XCV_TIME(PDN_RX_DN));

    // stop the recording
    hal_TcuSetEvent (HAL_TCU_STOP_RFIN_RECORD, stop + XCV_TIME(PDN_RX_DN));

    RFD_TRACE(RFD_INFO_TRC,0,"xcvRxOff @ %d Qb",stop);
}

VOID rfd_XcvTxOn(RFD_WIN_T *Win, INT16 start)
{
    HAL_RFSPI_CMD_T *spiCmd;
    UINT8 i =0;   
    UINT8 Winbitmap = (Win->bitmap & 0xf) << 1;
    UINT8 nbSlots = (Winbitmap & 0x10)? 4:
                    (Winbitmap & 0x8)? 3:
                    (Winbitmap & 0x4)? 2: 1; 
    

    // Needs an Spi Command
    spiCmd = hal_RfspiNewCmd(  (start + XCV_TIME(SPI_CMD)- CMD_DURATION(11)),
                                Win->winIdx);

    // mark for DTX
    spiCmd->maskable = TRUE;

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)    
    hal_TcuSetEvent (XCV_SET_PDN, start + XCV_TIME(PDN_TX_UP));
#ifndef _TX_BLOCK_MODE 
    hal_TcuSetEvent(HAL_TCU_START_GMSK, start + XCV_TIME(TX_GMSK_ON));
#endif
#endif

    for(i =0; i<nbSlots; i++)
    {  
        
        // If current slot active
        if (Winbitmap & 0x2)
        {

            hal_TcuSetEvent (HAL_TCU_DIGRF_STB, start + XCV_TIME(TX_STROBE_ON));

#if ((CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE) || (CHIP_ASIC_ID == CHIP_ASIC_ID_ESPERITE))
            if(!(Winbitmap & 0x1))
            {
#ifndef _TX_BLOCK_MODE 
                hal_TcuSetEvent(HAL_TCU_START_GMSK, start + XCV_TIME(TX_GMSK_ON));
#endif
                hal_TcuSetEvent(XCV_SET_PDN, start + XCV_TIME(PDN_TX_UP)); 
            }
#endif
            // If next slot not active 
            if(!(Winbitmap & 0x4))
            {
                UINT16 stop = ((Win->type == GSM_WIN_TYPE_TXABURST)?
                              (start + GSM_ABURST_ACTIVE_QS_DURATION + 4*g_xcvParams.txBstMap.postamble):
                              (start + GSM_NBURST_ACTIVE_QS_DURATION + 4*g_xcvParams.txBstMap.postamble));

                hal_TcuSetEvent (HAL_TCU_DIGRF_STB, stop + XCV_TIME(TX_STROBE_OFF));

#if ((CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE) || (CHIP_ASIC_ID == CHIP_ASIC_ID_ESPERITE))
#ifndef _TX_BLOCK_MODE              
                hal_TcuSetEvent(HAL_TCU_STOP_GMSK, stop + XCV_TIME(TX_GMSK_OFF));
#endif                
                hal_TcuSetEvent (XCV_CLR_PDN, stop + XCV_TIME(PDN_TX_DN));
            
#endif

            }
        
       }
       else
       { 
            if(!(Winbitmap & 0x4))
            {
                hal_TcuSetEvent (HAL_TCU_DIGRF_STB, start + XCV_TIME(TX_STROBE_ON));

            }


       }
      
       start += (Win->type == GSM_WIN_TYPE_TXABURST)?
                               (GSM_ABURST_ACTIVE_QS_DURATION + 4*8):  // active access burst + 8 guard bits.
                                GSM_SLOT_QS_DURATION;

                  
                                 
       Winbitmap >>= 1;            

    }

    // Build the command
    xcvBuildTxSpiCmd (Win,
                      Win->arfcn,
                      Win->band,
                      spiCmd);


    Win->swapIQ = FALSE;

    RFD_TRACE(RFD_INFO_TRC,0,"xcvTxOn @ %d Qb",start);

  
}

VOID rfd_XcvTxOff(RFD_WIN_T *Win,INT16 stop)
{
    // Put Transceiver in Powerdown
    if (Win->type==GSM_WIN_TYPE_TXABURST)
        stop+=16;    
    
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE)
    // Transceiver out of PowerDown
    hal_TcuSetEvent (XCV_CLR_PDN, stop + XCV_TIME(PDN_TX_DN)); 
#ifndef _TX_BLOCK_MODE
    hal_TcuSetEvent(HAL_TCU_STOP_GMSK, (stop + XCV_TIME(TX_GMSK_OFF)));
#endif
#endif

    RFD_TRACE(RFD_INFO_TRC,0,"xcvTxOff @ %d Qb",stop);
}

// *** UINT8 xcvGetAgcGain
UINT8 rfd_XcvGetAgcGain(UINT8 step, GSM_RFBAND_T band, UINT16 arfcn)
{
    UINT8 xcver_gain = 0;
    UINT8 gain = XCVER_AGC_TARGET + XCV_PARAM(ILOSS(band));
    UINT8 index = (gain - 15);
   
    CONST XCV_RX_GAIN_T* agcTable =  ((band == GSM_BAND_GSM850)|| (band == GSM_BAND_GSM900))?
                                     AgcTab_EGSM:
                                     AgcTab_DCS;

   xcver_gain = agcTable[index].totGain - (XCV_PARAM(ILOSS(band))& 0xf);

   return xcver_gain;
}


VOID rfd_XcvFrequencyTuning(INT16 FOf, GSM_RFBAND_T band)
{

    // AFC DAC gain is in tenth's of Hertz
 
    INT16 DacOffset = (10*FOf) / XCV_PARAM(AFC_GAIN(band));

    if ((DacOffset + g_afcDacVal) < RDA6220_AFC_MIN)
    {
        g_afcDacVal = RDA6220_AFC_MIN;
    }
    else if ((DacOffset + g_afcDacVal) > RDA6220_AFC_MAX)
    {
        g_afcDacVal = RDA6220_AFC_MAX;
    }
    else
    {
        g_afcDacVal += DacOffset;
    }
	
    //RFD_TRACE(RFD_INFO_TRC|TSTDOUT,0,"g_afcDacVal: %#x ", g_afcDacVal);

}

//-----------------------------------//
// Crystal Aging detection stuff     //
//-----------------------------------//
#define AGING_DAC_LIMIT     0x9F // 1.5 CDAC unit ~= 159 Dac units
#define DAC_MID_RANGE       RDA6220_AFC_MID
INT32 rfd_XcvGetAfcOffset(VOID)
{
    //return (g_AfcOffset + (g_afcDacVal - DAC_MID_RANGE));
#ifdef __USE_DIGITAL_AFC__
#define AGING_LIMIT  0xff

    INT32 AfcOffset = 0xffff / AGING_LIMIT;

    if (AfcOffset != 0)
    {
        return (AfcOffset);
    }
    else
    {
        return (PAL_XTAL_NO_AGING);
    }
#endif
return 0;
}


//-----------------------------------//
// Open/Close & Low Power Functions  //
//-----------------------------------//
VOID rfd_XcvOpen(INT32 AfcOffset, CONST XCV_CONFIG_T* xcvConfig)
{
    XCV = xcvConfig;
    // Get the calibration pointer
    g_xcvCalibPtr = calib_GetPointers();


    // Open RF Spi, ask for a 19.5MHz spi clock (Max speed)
    hal_RfspiOpen((HAL_RFSPI_CFG_T*)&g_spiCfg, HAL_RFSPI_19_5M_CLK);

    // Derive Aging compensated CDAC value
    // Xtal triming capacitor register to default (calibrated)
    g_CDAC = RDA6220_CDAC_DEFAULT;
    g_AfcOffset = AfcOffset;
    // Replace the default only if aging compensation happened
    // TODO: to be tested
    if (g_AfcOffset > AGING_DAC_LIMIT)
    {
        g_CDAC += (g_AfcOffset/AGING_DAC_LIMIT);
    }
    else if (g_AfcOffset < AGING_DAC_LIMIT)
    {
        g_CDAC -= (g_AfcOffset/AGING_DAC_LIMIT);
    }


    // turn on AFC DAC - mid range    
    g_afcDacVal = RDA6220_AFC_MID;

    xcvConfigure();


}

INLINE VOID xcvConfigure(VOID)
{
    HAL_RFSPI_CMD_T* confCmd = hal_RfspiNewSingleCmd();
   

    xcvBuildSpiCmd (confCmd, 0x3f, 0x00000);

    // Reset & Configure RDA6220
    xcvBuildSpiCmd (confCmd, 0x06, 0x3f333); // SPI- Mode Select
    hal_RfspiSendCmd(confCmd);
 
    xcvBuildSpiCmd (confCmd, 0x30, 0x08a00); // Soft reset   
    hal_RfspiSendCmd(confCmd);
    hal_TimDelay(20);                        // Soft reset period (Delay time=1ms) 
  
    xcvBuildSpiCmd (confCmd, 0x30, 0x08a01); // Release soft reset
    hal_RfspiSendCmd(confCmd);
    hal_TimDelay(120);

//    xcvBuildSpiCmd (confCmd, 0x24, 0x0757F); // Force GPIO enables????
//    hal_RfspiSendCmd(confCmd);
//    hal_TimDelay(120);

    xcvBuildSpiCmd (confCmd, 0x08, 0x00000); // Mode Control by 30H
    xcvBuildSpiCmd (confCmd, 0x3e, 0x28b28); // On PA, SW, VGA Timing
    xcvBuildSpiCmd (confCmd, 0x22, 0x0717f);
    xcvBuildSpiCmd (confCmd, 0x30, 0x0aa01); // Freq_Mode=1, Zf_Mode=1
    hal_RfspiSendCmd(confCmd);

    xcvBuildSpiCmd (confCmd, 0x26, 0x0041c); 
    xcvBuildSpiCmd (confCmd, 0x3f, 0x00001); 
    xcvBuildSpiCmd (confCmd, 0x42, 0x074e8); 
    xcvBuildSpiCmd (confCmd, 0x43, 0x144e8); 
    xcvBuildSpiCmd (confCmd, 0x44, 0x004e8); 
    xcvBuildSpiCmd (confCmd, 0x52, 0x08c2a); 
    xcvBuildSpiCmd (confCmd, 0x5e, 0x03920); 
    xcvBuildSpiCmd (confCmd, 0x3f, 0x00000);
    hal_RfspiSendCmd(confCmd);
 
    xcvBuildSpiCmd (confCmd, 0x0e, 0x10088);
    xcvBuildSpiCmd (confCmd, 0x28, 0x0c00b); 
    xcvBuildSpiCmd (confCmd, 0x38, 0x1019c); 
    xcvBuildSpiCmd (confCmd, 0x3b, 0x00040); 
    xcvBuildSpiCmd (confCmd, 0x3f, 0x00001); 
    xcvBuildSpiCmd (confCmd, 0x5f, 0x01453); 
    xcvBuildSpiCmd (confCmd, 0x65, 0x04ea1); 
    xcvBuildSpiCmd (confCmd, 0x3f, 0x00000); 
    xcvBuildSpiCmd (confCmd, 0x32, 0x05875); 
    xcvBuildSpiCmd (confCmd, 0x33, 0x00004); 
    hal_RfspiSendCmd(confCmd);

    xcvBuildSpiCmd (confCmd, 0x01, 0x23f84); 
    xcvBuildSpiCmd (confCmd, 0x0d, 0x13e1a); 
    xcvBuildSpiCmd (confCmd, 0x17, 0x05b5e); 
    xcvBuildSpiCmd (confCmd, 0x3f, 0x00001);
    xcvBuildSpiCmd (confCmd, 0x75, 0x0131f); 
    xcvBuildSpiCmd (confCmd, 0x76, 0x04237); 
    xcvBuildSpiCmd (confCmd, 0x3f, 0x00000); 
    xcvBuildSpiCmd (confCmd, 0x3f, 0x00002); 
    xcvBuildSpiCmd (confCmd, 0x94, 0x092d8);
    xcvBuildSpiCmd (confCmd, 0x96, 0x01fcd);
    xcvBuildSpiCmd (confCmd, 0x3f, 0x00000); 
    hal_RfspiSendCmd(confCmd);

    // Self calibration
    xcvBuildSpiCmd (confCmd, 0x05, 0x00248); // enter self-cal state
    xcvBuildSpiCmd (confCmd, 0x06, 0x27628); // lowband dc offset calibration 
    xcvBuildSpiCmd (confCmd, 0x30, 0x0aa43);
    hal_RfspiSendCmd(confCmd);
    // Turn on PDN for 10ms
    hal_TcuPulseTco(XCV->PDN, 170);


    xcvBuildSpiCmd (confCmd, 0x05, 0x00239); // enter self-cal state
    xcvBuildSpiCmd (confCmd, 0x06, 0x0ec4e); // highband dc offset calibration 
    xcvBuildSpiCmd (confCmd, 0x30, 0x0aa23);
    hal_RfspiSendCmd(confCmd);
    // Turn on PDN for 10ms
    hal_TcuPulseTco(XCV->PDN, 170);

    xcvBuildSpiCmd (confCmd, 0x05, 0x00248); // enter self-cal state
    xcvBuildSpiCmd (confCmd, 0x06, 0x27628); // edge TX offset offset calibration 
    xcvBuildSpiCmd (confCmd, 0x30, 0x06a43);
    hal_RfspiSendCmd(confCmd);
    // Turn on PDN for 10ms
    hal_TcuPulseTco(XCV->PDN, 170);
        
    xcvBuildSpiCmd (confCmd, 0x30, 0x0e201); 
    xcvBuildSpiCmd (confCmd, 0x08, 0x10000);
    
    xcvBuildSpiCmd (confCmd, 0x3f, 0x00002); 
    xcvBuildSpiCmd (confCmd, 0x94, 0x192d8);
    xcvBuildSpiCmd (confCmd, 0x3f, 0x00000); 
    hal_RfspiSendCmd(confCmd); 

#ifdef FPGA 
    xcvBuildSpiCmd  (confCmd, 0x1b, 0x2ffc7); // Modify clk from sin to square wave form.
#endif 
    
    xcvBuildSpiCmd  (confCmd, RDA6220_CDAC_REG, (0x28004 | g_CDAC <<4));

    hal_RfspiSendCmd(confCmd);


#if 0 // Debug: Read out POR settings


    xcvReadReg (0x0, (UINT8 *)(&g_tmp5));
    RFD_TRACE(TSTDOUT,0,"First read Reg 0x00 = 0x%x",g_tmp5);
    //hal_TimDelay(120);

    xcvReadReg (0x30, (UINT8 *)(&g_tmp1));
    xcvReadReg (0x08, (UINT8 *)(&g_tmp2));
    xcvReadReg (0x06, (UINT8 *)(&g_tmp3));
    xcvReadReg (0x3e, (UINT8 *)(&g_tmp4));

    RFD_TRACE(TSTDOUT,0,"Second read Reg 0x30 = 0x%x",g_tmp1);
    RFD_TRACE(TSTDOUT,0,"Reg 0x08 = 0x%x",g_tmp2);
    RFD_TRACE(TSTDOUT,0,"Reg 0x06 = 0x%x",g_tmp3);
    RFD_TRACE(TSTDOUT,0,"Reg 0x3E = 0x%x",g_tmp4);

    //XHALT;

   
#endif

}

VOID rfd_XcvClose(VOID)
{
    // Nothing to be done for the Xcver...
    hal_RfspiClose();
}

VOID rfd_XcvWakeUp (VOID)
{
    if (g_lastSleepMode == RFD_POWER_OFF)
    {

        HAL_RFSPI_CMD_T* confCmd = hal_RfspiNewSingleCmd();
        INT32 bitfield = (0x28004 | g_CDAC <<4);

        xcvBuildSpiCmd  (confCmd, RDA6220_CDAC_REG, bitfield);
        hal_RfspiSendCmd(confCmd);
    
        g_lastSleepMode = RFD_POWER_ON;
    }
}

// *** xcvSleep (bool toDeepPD) ***
VOID rfd_XcvSleep (RFD_LP_MODE_T lp_mode)
{
    g_lastSleepMode = lp_mode;
    
    if(RFD_LOW_POWER == lp_mode){
        hal_TcuClrTco(HAL_TCU_TCO_RX_DEC_ON);
        hal_TcuClrTco(XCV->PDN);        
    }

}

//----------------------//
// Calibration Related  //
//----------------------//

#define CALIB_DONE      0
#define CALIB_NOTDONE   1
#define CALIB_ERROR     0xFF
// State for Xtal calibration
enum {GUESS_CDAC,SEARCH_CDAC,DONE_CDAC};
UINT8 xtal_state = GUESS_CDAC;
// State of the iloss calibration
BOOL g_rfdILossInit = TRUE;

const CALIB_XCV_T xcv_calib_default =
{
    DEFAULT_CALIB_TIMES,
    DEFAULT_CALIB_PARAM,
    DEFAULT_CALIB_PALCUST
};

const CALIB_XCV_T * rfd_XcvCalibGetDefault(VOID)
{
    return &xcv_calib_default;
}

VOID rfd_XcvCalibUpdateValues(VOID)
{
    return;
}

VOID rfd_XcvCalibResetValues(VOID)
{
    // In this case, we reset the CDAC to default value.
    HAL_RFSPI_CMD_T *spiCmd = hal_RfspiNewCmd(0,0);
    UINT32 bitfield = 0x28004;
    
    // When the current CDAC is different from the init CDAC value,
    // we reset the CDAC value.
    if (g_CDAC != RDA6220_CDAC_DEFAULT)
    {
        // Xtal triming capacitor register to default (calibrated)
        g_CDAC = RDA6220_CDAC_DEFAULT;
        bitfield |= g_CDAC << 4;
        xcvBuildSpiCmd (spiCmd, RDA6220_CDAC_REG, bitfield);
    }

    // Reset Xtal calib state
    xtal_state = GUESS_CDAC;
    // Reset iloss calib state
    g_rfdILossInit = TRUE;
    return;
}

UINT8 rfd_XcvCalibXtalFreqOffset(INT32 FOf)
{

// 1 CDAC unit =~ 800Hz 
#define CDAC_UNIT 300
#define FIRST_FOF 0x7FFF
#define OPPOSITE_SIGN(n,m) (((n>0)&&(m<0))||((m>0)&&(n<0)))
//#define ABS(x)   (((x)>0)?(x):-(x))

    static INT16 last_FOf = FIRST_FOF;
    static UINT8 last_Cdac = 0;

    HAL_RFSPI_CMD_T *spiCmd;
    UINT32 bitfield = 0x28004;

    switch (xtal_state)
    {
        case GUESS_CDAC:
            last_FOf = FIRST_FOF;
            last_Cdac = g_CDAC;

            g_CDAC -= (FOf / CDAC_UNIT);
            xtal_state = SEARCH_CDAC;


            // Check CDAC boundaries
            if ((g_CDAC > RDA6220_CDAC_MAX)||(g_CDAC < RDA6220_CDAC_MIN))
            {
                //xcv_fprintf((RFD_INFO_TRC,"xcvCDAC @ %d Qb",g_CDAC));
                return CALIB_ERROR;
            }
        break;

        case SEARCH_CDAC:
            // Check if we changed sign
            if (OPPOSITE_SIGN(FOf,last_FOf) && (last_FOf != FIRST_FOF))
            {
                // We have the good CDAC
                if (ABS(FOf) < ABS(last_FOf))
                {
                    return CALIB_DONE;
                }
                else // Last CDAC was the good one
                {
                    g_CDAC = last_Cdac;
                    xtal_state = DONE_CDAC;
                }
            }
            else // still searching...
            {
                last_Cdac = g_CDAC;
                if (FOf < 0) g_CDAC++;
                else g_CDAC--;

                // Check CDAC boundaries
                if ((g_CDAC > RDA6220_CDAC_MAX)||(g_CDAC < RDA6220_CDAC_MIN))
                    return CALIB_ERROR;
            }
            last_FOf = FOf;
        break;
    }

    // Program CDAC change early next frame
    spiCmd = hal_RfspiNewCmd(0,0);
    bitfield |= (g_CDAC << 4);
    
    //RFD_TRACE(TSTDOUT,0,"XTAL xcvCDAC @ %d Qb",g_CDAC);
    xcvBuildSpiCmd (spiCmd, RDA6220_CDAC_REG , bitfield);

    // Update calibration structure
    XCV_PARAM(XTAL) = g_CDAC;
    if (g_xcvCalibPtr->hstXcv != NULL)
        HST_XCV_PARAM(XTAL) = g_CDAC;

    // We can stop right now are we are sure that at least one
    // FINT will happen util the stub goes to another task, so
    // the CDAC value will be changed.
    if (xtal_state == DONE_CDAC) return CALIB_DONE;

    return CALIB_NOTDONE;
}

VOID rfd_XcvCalibSetAfcBound(RFD_AFC_BOUND_T bound)
{
    HAL_RFSPI_CMD_T *spiCmd = hal_RfspiNewCmd(0,0);
    
    //UINT32 bitfield = 0;
//#define __USE_DIGITAL_AFC__
    if (RFD_AFC_BOUND_CENTER == bound)
        g_afcDacVal = RDA6220_AFC_MID;
    else if (RFD_AFC_BOUND_POS == bound)
        g_afcDacVal = RDA6220_AFC_BOUND_HIGH;
    else
        g_afcDacVal = RDA6220_AFC_BOUND_LOW;
    //bitfield = 0x4000;
//#ifndef __USE_DIGITAL_AFC__
    // The analog AFC for RDA6220b is not a linear
    //hal_AnaAfcOpen();
   // hal_AnaAfcForceValue(g_afcDacVal);
//#else

    //RFD_TRACE(TSTDOUT,0,"AFC Dac 0x%x",g_afcDacVal);
    
    xcvBuildSpiCmd(spiCmd, RDA6220_AFC_REG, (0x14000|g_afcDacVal));
   
//#endif
}

VOID rfd_XcvCalibCalcAfcGain(INT32 FreqOffset, GSM_RFBAND_T band)
{
    INT32 gain = (FreqOffset * 10) / (2 * RDA6220_AFC_HRANGE);

    // Update calibration structure
    XCV_PARAM(AFC_GAIN(band)) = gain;
    if (g_xcvCalibPtr->hstXcv != NULL)  
        HST_XCV_PARAM(AFC_GAIN(band)) = gain;
}

// ILoss ARFCN Table
PRIVATE UINT16 g_rfdILossArfcnTab850[6] = RFD_DEFAULT_CALIB_ILOSS_ARFCN_850;
PRIVATE UINT16 g_rfdILossArfcnTabGSM[6] = RFD_DEFAULT_CALIB_ILOSS_ARFCN_GSM;
PRIVATE UINT16 g_rfdILossArfcnTabDCS[9] = RFD_DEFAULT_CALIB_ILOSS_ARFCN_DCS;
PRIVATE UINT16 g_rfdILossArfcnTabPCS[9] = RFD_DEFAULT_CALIB_ILOSS_ARFCN_PCS;
 
PRIVATE UINT8    g_rfdILossNum;
PRIVATE UINT16*  g_rfdTmpArfcn;
 
PRIVATE UINT32   g_rfdTmpOffset;

UINT8 rfd_XcvCalibSetILoss(INT8 offset, GSM_RFBAND_T band, UINT16* nextArfcn)
{
// First call of this function after the restart of the system or after
// after a call to the xcv_CalibResetValues function.

     
    if (TRUE == g_rfdILossInit) 
    {
        // iloss calibration initialization. Choose the band which need calibration.
        g_rfdTmpArfcn = (band == GSM_BAND_GSM850 )? g_rfdILossArfcnTab850:
                        (band == GSM_BAND_DCS1800)? g_rfdILossArfcnTabDCS:
                        (band == GSM_BAND_PCS1900)? g_rfdILossArfcnTabPCS:
                        g_rfdILossArfcnTabGSM;
      
 
        g_rfdTmpOffset = 0;
        g_rfdILossNum = 0;
        g_rfdILossInit = FALSE; 
        *nextArfcn = g_rfdTmpArfcn[g_rfdILossNum];
        return CALIB_PROCESS_CONTINUE; 
    }

    offset += ((XCV_PARAM(ILOSS(band)) >> (4*g_rfdILossNum)) & 0xF);
    g_rfdTmpOffset |= (offset << (4*g_rfdILossNum));
    *nextArfcn = g_rfdTmpArfcn[++g_rfdILossNum];

    // Judge the sign of iloss calibration.
    if(*nextArfcn == 0)
    {
		g_rfdILossInit = TRUE;
		
		XCV_PARAM(ILOSS(band)) = g_rfdTmpOffset;
		if (g_xcvCalibPtr->hstXcv != NULL)
			HST_XCV_PARAM(ILOSS(band)) = g_rfdTmpOffset;
		
		return CALIB_PROCESS_STOP;
    }
    
    return CALIB_PROCESS_CONTINUE;
}
