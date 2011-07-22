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
#include "cs_types.h"
#include "hal_sys.h"
#include "stdlib.h"
#include "sxs_io.h"
#include "hal_spi.h"

#include "hal_timers.h"
#include "drv_dualsim.h"
#include "dualsim_config.h"
#include "edrvp_debug.h"

#include "sxr_tls.h"

#define TGT_DUALSIM_CONFIG                                              \
    {                                                                   \
        HAL_SPI,                                                        \
        HAL_SPI_CS1                                                     \
    }

#define        DS_NULLVOLT_DLY          (20) // delay in 1/1000 sec
#define        DS_SETVOLT_DLY           (50) // delay in 1/1000 sec

#define        DS_SETCLK_DLY            (1)
#define        DS_SETDATA_DLY           (1)
#define        DS_SETRST_DLY            (1)
#define        DS_SETSIMEN_DLY          (1)

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


BOOL  g_dualsimInitDone = FALSE;

enum  et6302_reg_map{
    reset_ctrl_reg = 0,
    clock_ctrl_reg,
    data_ctrl_reg,
    vcc_ctrl_reg,
    ldo_ctrl_reg,
    bandgap_ctrl_reg,
};

/// Configuration structure for TARGET
PROTECTED CONST DUALSIM_CONFIG_T* g_dualsimConfig;


// ======================================================================
// dualsim_Open
// ----------------------------------------------------------------------
/// This function Open Spi for dualsim
// ======================================================================
PUBLIC VOID dualsim_Open(VOID)
{
      // Configure SPI
    HAL_SPI_CFG_T dualsim_spiCfg =
    {
        //.enabledCS    = HAL_SPI_CS2,
        .csActiveLow    = FALSE,
        .inputEn        = FALSE,
        .clkFallEdge    = FALSE,
        .clkDelay       = HAL_SPI_HALF_CLK_PERIOD_1,
        .doDelay        = HAL_SPI_HALF_CLK_PERIOD_0,
        .diDelay        = HAL_SPI_HALF_CLK_PERIOD_2,
        .csDelay        = HAL_SPI_HALF_CLK_PERIOD_0,
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

    g_dualsimConfig = TGT_DUALSIM_CONFIG;

    dualsim_spiCfg.enabledCS = g_dualsimConfig->spiCs;
	
    if(FALSE==g_dualsimInitDone)
    {
        hal_SpiOpen(g_dualsimConfig->spiBus,g_dualsimConfig->spiCs,&dualsim_spiCfg);
	 g_dualsimInitDone = TRUE;
    }
    
}
   
// ======================================================================
// dualsim_Close
// ----------------------------------------------------------------------
/// This function Close dualsim Spi
// ======================================================================

PUBLIC VOID  dualsim_Close(VOID)
{
     if(TRUE==g_dualsimInitDone)
     {
          hal_SpiClose(g_dualsimConfig->spiBus,g_dualsimConfig->spiCs);
	   g_dualsimInitDone=FALSE;
     }  
}


// ======================================================================
// dualsim_SpiActivateCs
// ----------------------------------------------------------------------
/// This function ActivateCs dualsim Spi 
// ======================================================================


PROTECTED DUALSIM_ERR_T  dualsim_SpiActivateCs(VOID)
{
     if(FALSE==g_dualsimInitDone)
     {
         return DUALSIM_ERR_NOT_INIT;
     }
     if (hal_SpiActivateCs(g_dualsimConfig->spiBus,g_dualsimConfig->spiCs))
    {
        return DUALSIM_ERR_NO;
    }
    else
    {
        return DUALSIM_ERR_RESOURCE_BUSY;
    }
     
}


//=============================================================================
// dualsim_SpiDeActivateCs
//-----------------------------------------------------------------------------
/// Deactivate dualsiml SPI. refer to hal_SpiDeActivateCs() for details.
//=============================================================================
PROTECTED VOID dualsim_SpiDeActivateCs(VOID)
{   
    if(TRUE==g_dualsimInitDone)
    {
       hal_SpiDeActivateCs(g_dualsimConfig->spiBus,g_dualsimConfig->spiCs);
    }
}


// ======================================================================
// dualsim_SendRegData
// ----------------------------------------------------------------------
/// This function send data to 6302 Reg
// ======================================================================

PROTECTED VOID dualsim_SendRegData(enum et6302_reg_map regIdx, UINT8 data)
{   

    u8 iTempData[3] = {0x0, 0x0, 0x0};
    u16 iCount = 0;
 
     iTempData[0] = (regIdx << 5) | data;
	 
    hal_SpiSendData(g_dualsimConfig->spiBus,g_dualsimConfig->spiCs,iTempData, 1);
		
    EDRV_TRACE(EDRV_DBG_TRC|TSTDOUT,0,"dualsim_Send Reg:%d Data:0x%02x",regIdx, data);
		
    //wait until any previous transfers have ended
    while(!hal_SpiTxFinished(g_dualsimConfig->spiBus,g_dualsimConfig->spiCs));
    for(iCount = 0;iCount < 800;iCount++);
}


// =======================================================================
// dualsim_SetSim3VVoltage
// -----------------------------------------------------------------------
/// This function set sim1 Voltage and set sim0 Voltage 3V
// =======================================================================

PROTECTED VOID dualsim_SetSim3VVoltage(DUALSIM_SIM_VOLTAGE_T level)
{
   
    UINT8  RegConfig;

    // Level 0 = Off
    // Level 1 = Class C (1.8V)
    // Level 2 = Class B (2.7 - 3.3V)
    // Level 3 = Class A (5V)

    switch (level)
    {
        case DUALSIM_SIM_VOLTAGE_CLASS_A:
            RegConfig=DUALSIM_CARD1_VCCEN|DUALSIM_CARD1_VSEL|DUALSIM_CARD0_VCCEN|DUALSIM_CARD0_VSEL;
            dualsim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
		
        case DUALSIM_SIM_VOLTAGE_CLASS_B:
            RegConfig=DUALSIM_CARD1_VCCEN|DUALSIM_CARD1_VSEL|DUALSIM_CARD0_VCCEN|DUALSIM_CARD0_VSEL;
            dualsim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
		
        case DUALSIM_SIM_VOLTAGE_CLASS_C:
            RegConfig=DUALSIM_CARD1_VCCEN|DUALSIM_CARD0_VCCEN|DUALSIM_CARD0_VSEL;
            dualsim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
		
        case DUALSIM_SIM_VOLTAGE_NULL:
            RegConfig=DUALSIM_CARD0_VCCEN|DUALSIM_CARD0_VSEL;
            dualsim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
    }
    

}


// ===================================================================
// dualsim_SetSim1V8Voltage
// -------------------------------------------------------------------
/// This function set sim1 Voltage and set sim0 Voltage 1.8V
// ===================================================================

PROTECTED VOID dualsim_SetSim1V8Voltage(DUALSIM_SIM_VOLTAGE_T level)
{
    UINT8  RegConfig;

    // Level 0 = Off
    // Level 1 = Class C (1.8V)
    // Level 2 = Class B (2.7 - 3.3V)
    // Level 3 = Class A (5V)

    switch (level)
    {
       
        case DUALSIM_SIM_VOLTAGE_CLASS_A:
            RegConfig=DUALSIM_CARD1_VCCEN|DUALSIM_CARD1_VSEL|DUALSIM_CARD0_VCCEN;
            dualsim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
		
        case DUALSIM_SIM_VOLTAGE_CLASS_B:
            RegConfig=DUALSIM_CARD1_VCCEN|DUALSIM_CARD1_VSEL|DUALSIM_CARD0_VCCEN;
            dualsim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
		
        case DUALSIM_SIM_VOLTAGE_CLASS_C:
            RegConfig=DUALSIM_CARD1_VCCEN|DUALSIM_CARD0_VCCEN;
            dualsim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
		
        case DUALSIM_SIM_VOLTAGE_NULL:
            RegConfig=DUALSIM_CARD0_VCCEN;
            dualsim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
    }
    
}


// ====================================================================
// dualsim_SetSim0VVoltage
// --------------------------------------------------------------------
/// This function set sim1 Voltage and set sim0 Voltage 0V
// ====================================================================

PROTECTED VOID dualsim_SetSim0VVoltage(DUALSIM_SIM_VOLTAGE_T level)
{
    UINT8  RegConfig;

    // Level 0 = Off
    // Level 1 = Class C (1.8V)
    // Level 2 = Class B (2.7 - 3.3V)
    // Level 3 = Class A (5V)

    switch (level)
    {       
        case DUALSIM_SIM_VOLTAGE_CLASS_A:
            RegConfig=DUALSIM_CARD1_VCCEN|DUALSIM_CARD1_VSEL;
            dualsim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
		
        case DUALSIM_SIM_VOLTAGE_CLASS_B:
            RegConfig=DUALSIM_CARD1_VCCEN|DUALSIM_CARD1_VSEL;
            dualsim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
		
        case DUALSIM_SIM_VOLTAGE_CLASS_C:
            RegConfig=DUALSIM_CARD1_VCCEN;
            dualsim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
		
        case DUALSIM_SIM_VOLTAGE_NULL:
            RegConfig=0x00;
            dualsim_SendRegData(vcc_ctrl_reg,RegConfig);
            break;
    }
    
}


// ==========================================================================
// dualsim_SetSimVoltage
// --------------------------------------------------------------------------
/// This function set sim0 and sim1 Voltage 
// ==========================================================================

PUBLIC DUALSIM_ERR_T  dualsim_SetSimVoltage(DUALSIM_SIM_VOLTAGE_T sim0Level,DUALSIM_SIM_VOLTAGE_T sim1Level)
{ 
    UINT32 delay;
    DUALSIM_ERR_T  errStatus;
    // Level 0 = Off
    // Level 1 = Class C (1.8V)
    // Level 2 = Class B (2.7 - 3.3V)
    // Level 3 = Class A (5V)  //2.8V
    errStatus=dualsim_SpiActivateCs();
	
    if(DUALSIM_ERR_NO==errStatus)
    {
       delay = DS_SETVOLT_DLY;
       switch (sim0Level)
       {
         case DUALSIM_SIM_VOLTAGE_CLASS_A:
            dualsim_SetSim3VVoltage(sim1Level);
            break;
            
         case DUALSIM_SIM_VOLTAGE_CLASS_B:
            dualsim_SetSim3VVoltage(sim1Level);
            break;
            
         case DUALSIM_SIM_VOLTAGE_CLASS_C:
            dualsim_SetSim1V8Voltage(sim1Level);
            break;
            
         case DUALSIM_SIM_VOLTAGE_NULL:
            dualsim_SetSim0VVoltage(sim1Level);
            break;
        }
	   
	dualsim_SpiDeActivateCs( );   
	hal_TimDelay(delay MS_WAITING);
	
  }

      return errStatus;
 
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
PUBLIC DUALSIM_ERR_T  dualsim_SetSimEnable(DUALSIM_SIM_CARD_T number)
{
    UINT32 delay;
    DUALSIM_ERR_T  errStatus;

    errStatus=dualsim_SpiActivateCs();
	
    if(DUALSIM_ERR_NO==errStatus)
    {
         delay = DS_SETSIMEN_DLY;
         switch (number)
        {
         case DUALSIM_SIM_SELECT_CARD_0:
            dualsim_SendRegData(reset_ctrl_reg,DUALSIM_CARD1_RSTSEL|DUALSIM_CARD1_RSTVAL);
            dualsim_SendRegData(clock_ctrl_reg,DUALSIM_CARD0_CPOL|DUALSIM_CARD1_CPOL);
            dualsim_SendRegData(data_ctrl_reg,DUALSIM_CARD0_DATAEN);
            break;
				
         case DUALSIM_SIM_SELECT_CARD_1:
            dualsim_SendRegData(reset_ctrl_reg,DUALSIM_CARD0_RSTSEL|DUALSIM_CARD0_RSTVAL);
            dualsim_SendRegData(clock_ctrl_reg,DUALSIM_CARD1_CPOL|DUALSIM_CARD0_CPOL);
            dualsim_SendRegData(data_ctrl_reg,DUALSIM_CARD1_DATAEN);
            break;
				
         case DUALSIM_SIM_CLK_ON:
            dualsim_SendRegData(clock_ctrl_reg,DUALSIM_CARD0_CPOL|DUALSIM_CARD1_CPOL);
            break;
		  
         case DUALSIM_SIM_CLK_ON_CARD_DATA_0:
            dualsim_SendRegData(data_ctrl_reg,DUALSIM_CARD0_DATAEN);
            break;
		  
         case DUALSIM_SIM_CLK_ON_CARD_DATA_1:
            dualsim_SendRegData(data_ctrl_reg,DUALSIM_CARD1_DATAEN);
            break;
     
         case DUALSIM_SIM_SELECT_NULL:
            dualsim_SendRegData(reset_ctrl_reg,0x0F);
            dualsim_SendRegData(clock_ctrl_reg,0x00);
            dualsim_SendRegData(data_ctrl_reg,0x00);
            break;
				
          default:
              break;
         }
		 
         dualsim_SpiDeActivateCs( );
         hal_TimDelay(delay MS_WAITING);	  
	
     }
   
         return errStatus;
  
}



