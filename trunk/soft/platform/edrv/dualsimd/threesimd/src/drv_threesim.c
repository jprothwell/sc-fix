//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
//            Copyright (C) 2003-2008, Coolsand Technologies, Inc.              //
//                            All Rights Reserved                               //
//                                                                              //
//      This source code is the property of Coolsand Technologies and is        //
//      confidential.  Any  modification, distribution,  reproduction or        //
//      exploitation  of  any content of this file is totally forbidden,        //
//      except  with the  written permission  of  Coolsand Technologies.        //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
//  $HeadURL:                                                                   //
//	$Author:                                                                    //
//	$Date: 2008-11-06                                                           //
//	$Revision:                                                                  //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
///  @file drv_dualsim.c                                                        //
///  This file contains the dual sim switch 6302 module driver source code      //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////



#include "cs_types.h"
#include "hal_sys.h"
#include "stdlib.h"
#include "sxs_io.h"
#include "hal_spi.h"

#include "hal_timers.h"
#include "drv_dualsim.h"
#include "dualsim_config.h"
#include "edrvp_debug.h"
#include "rda1203.h"
#include "hal_gpio.h"
#include "sxr_tls.h"


#define        DS_NULLVOLT_DLY          (20) // delay in 1/1000 sec
#define        DS_SETVOLT_DLY           (50) // delay in 1/1000 sec

#define        DS_SETCLK_DLY            (1)
#define        DS_SETDATA_DLY           (1)
#define        DS_SETRST_DLY            (1)
#define        DS_SETSIMEN_DLY          (1)


#ifndef DUALSIM_SWAP
#define        DUALSIM_CARD0_VSEL       (1<<0)
#define        DUALSIM_CARD1_VSEL       (1<<1)
#define        DUALSIM_CARD0_VCCEN      (1<<2)
#define        DUALSIM_CARD1_VCCEN      (1<<3)

#define        DUALSIM_CARD0_RSTSEL     (1<<0)
#define        DUALSIM_CARD1_RSTSEL     (1<<1)
#define        DUALSIM_CARD0_RSTVAL     (1<<2)
#define        DUALSIM_CARD1_RSTVAL     (1<<3)

#define        DUALSIM_CARD0_CPOL       (1<<0)
#define        DUALSIM_CARD1_CPOL       (1<<1)
#define        DUALSIM_CARD0_CPOH       (1<<2)
#define        DUALSIM_CARD1_CPOH       (1<<3)

#define        DUALSIM_CARD0_DATAEN     (1<<0)
#define        DUALSIM_CARD1_DATAEN     (1<<1)
#define        DUALSIM_CARD0_DATA_L     (1<<2)
#define        DUALSIM_CARD1_DATA_L     (1<<3)
#else
#define        DUALSIM_CARD1_VSEL       (1<<0)
#define        DUALSIM_CARD0_VSEL       (1<<1)
#define        DUALSIM_CARD1_VCCEN      (1<<2)
#define        DUALSIM_CARD0_VCCEN      (1<<3)

#define        DUALSIM_CARD1_RSTSEL     (1<<0)
#define        DUALSIM_CARD0_RSTSEL     (1<<1)
#define        DUALSIM_CARD1_RSTVAL     (1<<2)
#define        DUALSIM_CARD0_RSTVAL     (1<<3)

#define        DUALSIM_CARD1_CPOL       (1<<0)
#define        DUALSIM_CARD0_CPOL       (1<<1)
#define        DUALSIM_CARD1_CPOH       (1<<2)
#define        DUALSIM_CARD0_CPOH       (1<<3)

#define        DUALSIM_CARD1_DATAEN     (1<<0)
#define        DUALSIM_CARD0_DATAEN     (1<<1)
#define        DUALSIM_CARD1_DATA_L     (1<<2)
#define        DUALSIM_CARD0_DATA_L     (1<<3)
#endif

BOOL  g_threesimInitDone = FALSE;
BOOL  g_dualsimInitDone = FALSE;

enum  et6302_reg_map{
    reset_ctrl_reg = 0,
    clock_ctrl_reg,
    data_ctrl_reg,
    vcc_ctrl_reg,
    ldo_ctrl_reg,
    bandgap_ctrl_reg,
};

#define TGT_THREESIM_CONFIG                  \
{                                                                   \
        HAL_SPI,                                                \
        HAL_SPI_CS0                                         \
}


/// Configuration structure for TARGET
PROTECTED CONST DUALSIM_CONFIG_T* g_dualsimConfig;
PROTECTED CONST DUALSIM_CONFIG_T g_threesimConfig = TGT_THREESIM_CONFIG;


extern DUALSIM_ERR_T pmd_DualSimSetVoltageClass(DUALSIM_SIM_VOLTAGE_T voltClass0,DUALSIM_SIM_VOLTAGE_T voltClass1);
extern BOOL pmd_SelectSimCard(UINT8 sim_card);
extern UINT16 pmd_OpalSpiRead(RDA_REG_MAP_T regIdx);
#define TEST_ASSERT(BOOL, format, ...)                   \
    if (!(BOOL)) {            \
        hal_DbgAssert(format, ##__VA_ARGS__);                             \
    }

//#if (NUMBER_OF_SIM==3) 
// ======================================================================
// threesim_Open
// ----------------------------------------------------------------------
/// This function Open Spi for threesim
// ======================================================================
PUBLIC VOID threesim_Open(VOID)
{
      // Configure SPI
    HAL_SPI_CFG_T threesim_spiCfg =
    {
//        .enabledCS    = HAL_SPI_CS1,
        .csActiveLow    = TRUE,
        .inputEn        = FALSE,
        .clkFallEdge    = FALSE,
        .clkDelay       = HAL_SPI_HALF_CLK_PERIOD_1,
        .doDelay        = HAL_SPI_HALF_CLK_PERIOD_0,
        .diDelay        = HAL_SPI_HALF_CLK_PERIOD_2,
        .csDelay        = HAL_SPI_HALF_CLK_PERIOD_1,
        .csPulse        = HAL_SPI_HALF_CLK_PERIOD_0,
        .frameSize      = 8,
        .oeRatio        = 0,
        .spiFreq        = 500000,
        .rxTrigger      = HAL_SPI_RX_TRIGGER_4_BYTE,
        .txTrigger      = HAL_SPI_TX_TRIGGER_1_EMPTY,
        .rxMode         = HAL_SPI_DIRECT_POLLING,
        .txMode         = HAL_SPI_DIRECT_POLLING,
        .mask           = {0,0,0,0,0},
        .handler        = NULL
    };

	HAL_APO_ID_T gpio ;
	
	gpio.gpoId= HAL_GPO_3 ;
		
       // reset gpio_3
       hal_GpioClr(gpio);	
	hal_TimDelay(160);
        hal_GpioSet(gpio);
			
			
    EDRV_TRACE(EDRV_SIMD_TRC, 0, "(threesim_Open) ");
			

    EDRV_TRACE(EDRV_SIMD_TRC, 0, "g_threesimConfig spiBus =  %d, spiCs = %d", g_threesimConfig.spiBus, g_threesimConfig.spiCs);
	   
	
    if(FALSE == g_threesimInitDone)
    {
        threesim_spiCfg.enabledCS = g_threesimConfig.spiCs;
        hal_SpiOpen(g_threesimConfig.spiBus,g_threesimConfig.spiCs,&threesim_spiCfg);

	 g_threesimInitDone = TRUE;
    }
    

}

// ======================================================================
// threesim_Close
// ----------------------------------------------------------------------
/// This function Close threesim Spi
// ======================================================================

PUBLIC VOID  threesim_Close(VOID)
{
    EDRV_TRACE(EDRV_SIMD_TRC, 0, "(threesim_Close) ");

     if(TRUE == g_threesimInitDone)
     {
          hal_SpiClose(g_threesimConfig.spiBus, g_threesimConfig.spiCs);
	   g_threesimInitDone=FALSE;
     }  

}


// ======================================================================
// threesim_SpiActivateCs
// ----------------------------------------------------------------------
/// This function ActivateCs threesim Spi 
// ======================================================================
PROTECTED FOURSIM_ERR_T  threesim_SpiActivateCs()
{
	EDRV_TRACE(EDRV_SIMD_TRC, 0, "(threesim_SpiActivateCs) g_threesimInitDone = %d. g_threesimConfig.spiBus = %d, g_threesimConfig.spiCs = %d", 
			g_threesimInitDone, g_threesimConfig.spiBus, g_threesimConfig.spiCs);
		
	
	if(FALSE == g_threesimInitDone)
	{
		return FOURSIM_ERR_NOT_INIT;
	}
	

	if (hal_SpiActivateCs(g_threesimConfig.spiBus, g_threesimConfig.spiCs))
	{
    		return FOURSIM_ERR_NO;
	}
	else
	{
	 	return FOURSIM_ERR_RESOURCE_BUSY;
	}
		
}


//=============================================================================
// threesim_SpiDeActivateCs
//-----------------------------------------------------------------------------
/// Deactivate foursiml SPI. refer to hal_SpiDeActivateCs() for details.
//=============================================================================
PROTECTED VOID threesim_SpiDeActivateCs()
{   
    EDRV_TRACE(EDRV_SIMD_TRC, 0, "(foursim_SpiDeActivateCs) ");

    if(TRUE == g_threesimInitDone)
    {
	hal_SpiDeActivateCs(g_threesimConfig.spiBus, g_threesimConfig.spiCs);
    }
}



// ======================================================================
// threesim_SendRegData
// ----------------------------------------------------------------------
/// This function send data to 6302 Reg
// ======================================================================
PROTECTED VOID threesim_SendRegData(enum et6302_reg_map regIdx, UINT8 data)
{   
    u8 iTempData[3] = {0x0, 0x0, 0x0};
    u16 iCount = 0;

    EDRV_TRACE(EDRV_SIMD_TRC,0,"(foursim_SendRegData) Reg:%d Data:0x%02x",regIdx, data);
 
     iTempData[0] = (regIdx << 5) | data;

    hal_SpiSendData(g_threesimConfig.spiBus, g_threesimConfig.spiCs, iTempData, 1);

    //wait until any previous transfers have ended
    while(!hal_SpiTxFinished(g_threesimConfig.spiBus, g_threesimConfig.spiCs));
	
    for(iCount = 0;iCount < 800;iCount++);

}

// =======================================================================
// threesim_SetSim3VVoltage
// -----------------------------------------------------------------------
/// This function set sim1 Voltage and set sim0 Voltage 3V
// =======================================================================
PROTECTED VOID threesim_SetSim3VVoltage(DUALSIM_SIM_VOLTAGE_T level)
{
   
    UINT8  RegConfig;

    // Level 0 = Off
    // Level 1 = Class C (1.8V)
    // Level 2 = Class B (2.7 - 3.3V)
    // Level 3 = Class A (5V)

    EDRV_TRACE(EDRV_SIMD_TRC, 0, "(dualsim_SetSim3VVoltage)");

    switch (level)
    {
        case DUALSIM_SIM_VOLTAGE_CLASS_A:
            RegConfig=DUALSIM_CARD1_VCCEN|DUALSIM_CARD1_VSEL|DUALSIM_CARD0_VCCEN|DUALSIM_CARD0_VSEL;
            threesim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
		
        case DUALSIM_SIM_VOLTAGE_CLASS_B:
            RegConfig=DUALSIM_CARD1_VCCEN|DUALSIM_CARD1_VSEL|DUALSIM_CARD0_VCCEN|DUALSIM_CARD0_VSEL;
            threesim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
		
        case DUALSIM_SIM_VOLTAGE_CLASS_C:
            RegConfig=DUALSIM_CARD1_VCCEN|DUALSIM_CARD0_VCCEN|DUALSIM_CARD0_VSEL;
            threesim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
		
        case DUALSIM_SIM_VOLTAGE_NULL:
            RegConfig=DUALSIM_CARD0_VCCEN|DUALSIM_CARD0_VSEL;
            threesim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
    }
}


// ===================================================================
// threesim_SetSim1V8Voltage
// -------------------------------------------------------------------
/// This function set sim1 Voltage and set sim0 Voltage 1.8V
// ===================================================================

PROTECTED VOID threesim_SetSim1V8Voltage(DUALSIM_SIM_VOLTAGE_T level)
{
    UINT8  RegConfig;

    // Level 0 = Off
    // Level 1 = Class C (1.8V)
    // Level 2 = Class B (2.7 - 3.3V)
    // Level 3 = Class A (5V)
    EDRV_TRACE(EDRV_SIMD_TRC, 0, "(dualsim_SetSim1V8Voltage) ");

    switch (level)
    {
       
        case DUALSIM_SIM_VOLTAGE_CLASS_A:
            RegConfig=DUALSIM_CARD1_VCCEN|DUALSIM_CARD1_VSEL|DUALSIM_CARD0_VCCEN;
            threesim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
		
        case DUALSIM_SIM_VOLTAGE_CLASS_B:
            RegConfig=DUALSIM_CARD1_VCCEN|DUALSIM_CARD1_VSEL|DUALSIM_CARD0_VCCEN;
            threesim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
		
        case DUALSIM_SIM_VOLTAGE_CLASS_C:
            RegConfig=DUALSIM_CARD1_VCCEN|DUALSIM_CARD0_VCCEN;
            threesim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
		
        case DUALSIM_SIM_VOLTAGE_NULL:
            RegConfig=DUALSIM_CARD0_VCCEN;
            threesim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
    }
    
}


// ====================================================================
// threesim_SetSim0VVoltage
// --------------------------------------------------------------------
/// This function set sim1 Voltage and set sim0 Voltage 0V
// ====================================================================

PROTECTED VOID threesim_SetSim0VVoltage(DUALSIM_SIM_VOLTAGE_T level)
{
    UINT8  RegConfig;

    // Level 0 = Off
    // Level 1 = Class C (1.8V)
    // Level 2 = Class B (2.7 - 3.3V)
    // Level 3 = Class A (5V)
    EDRV_TRACE(EDRV_SIMD_TRC, 0, "(dualsim_SetSim0VVoltage)");

    switch (level)
    {       
        case DUALSIM_SIM_VOLTAGE_CLASS_A:
            RegConfig=DUALSIM_CARD1_VCCEN|DUALSIM_CARD1_VSEL;
            threesim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
		
        case DUALSIM_SIM_VOLTAGE_CLASS_B:
            RegConfig=DUALSIM_CARD1_VCCEN|DUALSIM_CARD1_VSEL;
            threesim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
		
        case DUALSIM_SIM_VOLTAGE_CLASS_C:
            RegConfig=DUALSIM_CARD1_VCCEN;
            threesim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
		
        case DUALSIM_SIM_VOLTAGE_NULL:
            RegConfig=0x00;
            threesim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
    }
    
}



// ==========================================================================
// threesim_SetSimVoltage
// --------------------------------------------------------------------------
/// This function set sim0 and sim1 Voltage 
// ==========================================================================

PUBLIC DUALSIM_ERR_T  threesim_SetSimVoltage(DUALSIM_SIM_VOLTAGE_T sim0Level,DUALSIM_SIM_VOLTAGE_T sim1Level)
{ 
    UINT32 delay;
    DUALSIM_ERR_T  errStatus;
    // Level 0 = Off
    // Level 1 = Class C (1.8V)
    // Level 2 = Class B (2.7 - 3.3V)
    // Level 3 = Class A (5V)  //2.8V
    errStatus = threesim_SpiActivateCs();

    EDRV_TRACE(EDRV_SIMD_TRC, 0, "(threesim_SetSimVoltage)  sim0Level = %d, sim1Level = %d, spi Status = %d", sim0Level, sim1Level, errStatus);
	
    if(DUALSIM_ERR_NO==errStatus)
    {
       delay = DS_SETVOLT_DLY;
       switch (sim0Level)
       {
         case DUALSIM_SIM_VOLTAGE_CLASS_A:
            threesim_SetSim3VVoltage(sim1Level);
            break;
            
         case DUALSIM_SIM_VOLTAGE_CLASS_B:
            threesim_SetSim3VVoltage(sim1Level);
            break;
            
         case DUALSIM_SIM_VOLTAGE_CLASS_C:
            threesim_SetSim1V8Voltage(sim1Level);
            break;
            
         case DUALSIM_SIM_VOLTAGE_NULL:
            threesim_SetSim0VVoltage(sim1Level);
            break;
      }

	   
	threesim_SpiDeActivateCs();   
	hal_TimDelay(delay MS_WAITING);
	
  }

      return errStatus;
 
}


// ==========================================================================
// foursim_SetSimVoltage
// --------------------------------------------------------------------------
/// This function set sim0 and sim1 Voltage 
// ==========================================================================

PUBLIC DUALSIM_ERR_T  threesim_SetGalliteVoltage(DUALSIM_SIM_VOLTAGE_T sim0Level,DUALSIM_SIM_VOLTAGE_T sim1Level)
{ 
    UINT32 delay;
    DUALSIM_ERR_T  errStatus;
    // Level 0 = Off
    // Level 1 = Class C (1.8V)
    // Level 2 = Class B (2.7 - 3.3V)
    // Level 3 = Class A (5V)  //2.8V

#ifdef DUALSWITCH_SWAP
    DUALSIM_SIM_VOLTAGE_T temp;
    temp = sim1Level;
    sim1Level = sim0Level;
    sim0Level = temp;
#endif // DUALSIM_SWAP

    errStatus = pmd_DualSimSetVoltageClass(sim0Level, sim1Level);

    EDRV_TRACE(EDRV_SIMD_TRC, 0, "(threesim_SetGalliteVoltage) sim0Level= %d, sim1Level= %d, errStatus = %d", sim0Level, sim1Level, errStatus);

    if (errStatus == DUALSIM_ERR_NO)
    {
        delay = DS_SETVOLT_DLY;
        hal_TimDelay(delay MS_WAITING);
    }
    
    return errStatus;
}



// ======================================================================
// threesim_SelectSwitch
// ----------------------------------------------------------------------
/// This function select the special 6302 switch.
//   parameter: number =0: select switch 0
// 			   number =1: select switch 1
// ======================================================================
PUBLIC FOURSIM_ERR_T  threesim_SelectSwitch(FOURSIM_SWITCH_T number)
{
    UINT32  delay;
	
    EDRV_TRACE(EDRV_SIMD_TRC, 0, "(threesim_SelectSwitch) switch ID= %d", number);

    if ((number != FOURSIM_SWITCH_0) &&  (number != FOURSIM_SWITCH_1))
    {
        return FOURSIM_ERR_NO;
    }

#ifdef FOURSIM_SWITCH_SWAP
    if (number == FOURSIM_SWITCH_0)
    {
        number = FOURSIM_SWITCH_1;
    }
    else if (number == FOURSIM_SWITCH_1)
    {
        number = FOURSIM_SWITCH_0;
    }
#endif 

    if (pmd_SelectSimCard(number))
    {
        EDRV_TRACE(EDRV_SIMD_TRC, 0, "pmd_SelectSimCard  DONE");
    
        delay = DS_SETSIMEN_DLY;
        hal_TimDelay(delay MS_WAITING);
		
        return FOURSIM_ERR_NO;
    }
    
    return FOURSIM_ERR_RESOURCE_BUSY;
}

// ======================================================================
// dualsim_SetSimEnable
// ----------------------------------------------------------------------
/// This function set sim0 or  sim1 Enalbe 
// ======================================================================
//            CARD0    CARD1
// RSTREG     1010     0101
// CLKREG     0001     0010
// DATAREG    0001     0010
// ======================================================================
PUBLIC FOURSIM_ERR_T  threesim_SetSimEnable(DUALSIM_SIM_CARD_T number)
{
    UINT32 delay;
    FOURSIM_ERR_T  errStatus;

    errStatus = threesim_SpiActivateCs();

    EDRV_TRACE(EDRV_SIMD_TRC, 0, "(foursim_SetSimEnable) sim card ID =  %d", number);
    EDRV_TRACE(EDRV_SIMD_TRC, 0, "spi active state =  %d", errStatus);
	
    if(FOURSIM_ERR_NO==errStatus)
    {
         delay = DS_SETSIMEN_DLY;
         switch (number)
        {
         case FOURSIM_SIM_SELECT_CARD_0:
            threesim_SendRegData(reset_ctrl_reg,DUALSIM_CARD1_RSTSEL|DUALSIM_CARD1_RSTVAL);
            threesim_SendRegData(clock_ctrl_reg,DUALSIM_CARD0_CPOL|DUALSIM_CARD1_CPOL);
            threesim_SendRegData(data_ctrl_reg,DUALSIM_CARD0_DATAEN);
            break;
				
         case FOURSIM_SIM_SELECT_CARD_1:
            threesim_SendRegData(reset_ctrl_reg,DUALSIM_CARD0_RSTSEL|DUALSIM_CARD0_RSTVAL);
            threesim_SendRegData(clock_ctrl_reg,DUALSIM_CARD1_CPOL|DUALSIM_CARD0_CPOL);
            threesim_SendRegData(data_ctrl_reg,DUALSIM_CARD1_DATAEN);
            break;

         case FOURSIM_SIM_SELECT_NULL:
            threesim_SendRegData(reset_ctrl_reg,0x0F);
            threesim_SendRegData(clock_ctrl_reg,0x00);
            threesim_SendRegData(data_ctrl_reg,0x00);
            break;

         default:
              break;
         }
		 
         threesim_SpiDeActivateCs();
         hal_TimDelay(delay MS_WAITING);	  
	
     }
   
         return errStatus;
  
}


