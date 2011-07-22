////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/pal/src/pal_sim.c $
//    $Author: yanshengzhang $                                                         
//    $Date: 2011-05-06 21:29:02 +0800 (Fri, 06 May 2011) $                        
//    $Revision: 7456 $                                                            
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file pal_sim.c
/// This file contains the Sim PAL functions
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifdef PAL_SIMD_IMPLEMENTATION

#include "hal_timers.h"
#include "pal_sim.h"
#include "pmd_m.h"
#include "tgt_simd_cfg.h"
#include "simd_m.h"
#include "sxr_tls.h"

#define         NULLVOLT_DLY     (20)           // delay in 1/1000 sec
#define         SETVOLT_DLY      (50)           // delay in 1/1000 sec

//UINT8 g_SimIndex     = 0;

// ============================================================================
//  pal_EnableSimCard
// ----------------------------------------------------------------------------
// This is used to select  sim-card , it enables switching between SIM 0 and 
//SIM 1 as well as setting parameters.
/// @paramsim_Index: the sim card index. 0 corresponding to sim card 0, 
///and 1 corresponding to sim card 1.
/// @return value _should_ be always true in single SIM platforms
// ============================================================================
PUBLIC BOOL pal_EnableSimCard(UINT8 sim_Index)
{
    //g_SimIndex = sim_Index;
    return (simd_SetSimEnable(sim_Index));
}

// ============================================================================
// pal_SimInit
// ----------------------------------------------------------------------------
// ============================================================================
PUBLIC VOID pal_SimInit(VOID)
{
    // SCI init
    // Configure PMIC for level shift
    //pmd_SetLevel(PMD_LEVEL_SIM, PMD_SIM_VOLTAGE_0V0);
    simd_Open(tgt_GetSimdConfig());

    // TODO: This should not be done here.  Should be done by stack which
    // calls pal_EnableSimCard.
    pal_EnableSimCard(0);

    simd_SetSimVoltage(SIMD_VOLTAGE_NULL);
}

PUBLIC VOID pal_SimDelayAfterEpps()
{
    simd_DelayAfterEpps();
}

PUBLIC VOID pal_SimTxByteRxByte(
             UINT16 nTxByte, CONST UINT8* pTxByte,
             UINT16 nRxByte, UINT8* pRxByte,
             BOOL stopClock)
{
    simd_TxByteRxByte(nTxByte,pTxByte,nRxByte,pRxByte,stopClock);
}

PUBLIC UINT16 pal_SimGetNumRxBytes(void)    // hal_SimGetNumRxBytes
{
    return simd_GetNumRxBytes();
} 

PUBLIC VOID pal_SimSetClockStopMode(PAL_SIM_CLOCK_STOP_MODE_T mode)
{
    simd_SetClockStopMode(mode);
}

PUBLIC VOID pal_SimPowerDown(VOID)
{
    simd_PowerDown();
}

PUBLIC VOID pal_SimSetEtuDuration(PAL_SIM_ETU_DURATION_T fAndD)
{
    simd_SetEtuDuration(fAndD);
}

PUBLIC VOID pal_SimSetWorkWaitingTime(UINT8 wi)
{
    simd_SetWorkWaitingTime(wi);
}

PUBLIC VOID pal_SimReset(UINT8* pRxBytes)
{
    simd_Reset(pRxBytes);
}

// ============================================================================
// pal_SimClose
// ----------------------------------------------------------------------------
// ============================================================================
PUBLIC VOID pal_SimClose(VOID)
{
    simd_Close();

    // SCI init
    // Configure PMIC for level shift
    //pmd_SetLevel(PMD_LEVEL_SIM, PMD_SIM_VOLTAGE_0V0);
    simd_SetSimVoltage(SIMD_VOLTAGE_NULL);
}

// ============================================================================
// pal_SimSetVoltage
// ----------------------------------------------------------------------------
/// WARNING
/// This function is called under IRQ. Fancy things like
/// call to sxr_Sleep or mailbox creating are FORBIDDEN
/// This function sets the SIM voltage.
/// @param level Voltage to set.
// ============================================================================
PUBLIC BOOL pal_SimSetSimVoltage(UINT8 level)
{
    UINT32 delay;

    // Level 0 = Off
    // Level 1 = Class C (1.8V)
    // Level 2 = Class B (2.7 - 3.3V)
    // Level 3 = Class A (5V)

    delay = SETVOLT_DLY;
    
    switch (level) 
    {
        case PAL_SIM_VOLTAGE_CLASS_A:
            //pmd_SetLevel(PMD_LEVEL_SIM, PMD_SIM_VOLTAGE_5V0);
            simd_SetSimVoltage(SIMD_VOLTAGE_CLASS_A);
        break;
        case PAL_SIM_VOLTAGE_CLASS_B:
            //pmd_SetLevel(PMD_LEVEL_SIM, PMD_SIM_VOLTAGE_3V0);
            simd_SetSimVoltage(SIMD_VOLTAGE_CLASS_B);
        break;
        case PAL_SIM_VOLTAGE_CLASS_C:
            //pmd_SetLevel(PMD_LEVEL_SIM, PMD_SIM_VOLTAGE_1V8);
            simd_SetSimVoltage(SIMD_VOLTAGE_CLASS_C);
        break;
        case PAL_SIM_VOLTAGE_NULL:
            //pmd_SetLevel(PMD_LEVEL_SIM, PMD_SIM_VOLTAGE_0V0);
            simd_SetSimVoltage(SIMD_VOLTAGE_NULL);
            // Add extra delay for voltage 0
            delay += NULLVOLT_DLY;
        break;
    }
    
    hal_TimDelay(delay MS_WAITING);
    return TRUE;
}


// ============================================================================
// pal_SetTS
// ----------------------------------------------------------------------------
/// This function used to set ATR TS value.
/// @param TS is TS value from the ATR response
// ============================================================================
PUBLIC BOOL pal_SetTS(UINT8 TS)
{
    if(TS == 0x23 || TS == 0x3F)
    {
        // This only needs to be called in dual SIM mode
        simd_Switch2Manual(TS);
    }
    return FALSE;
}


#else // !PAL_SIMD_IMPLEMENTATION

#ifdef GALLITE_IS_CT1129

// Common includes
#include "cs_types.h"


//PAL includes
#include "pal_sim.h"
#include "palp_cfg.h"
#include "pal.h"

// HAL includes
#include "hal_timers.h"
#include "hal_sim.h"
#include "sxs_io.h"
#include "pmd_m.h"

// Dual sim includes
#include "drv_dualsim.h"

#include "assert.h"
#include "hal_host.h"

#define         NULLVOLT_DLY     (20)           // delay in 1/1000 sec
#define         SETVOLT_DLY      (50)           // delay in 1/1000 sec

//#define DSM_HstSendEvent  hal_HstSendEvent
#define pal_HstSendEvent 


UINT8 g_SimIndex 	 = 0;

UINT8 g_Sim0VoltLevel = PAL_SIM_VOLTAGE_0V;		//initial voltage
UINT8 g_Sim1VoltLevel = PAL_SIM_VOLTAGE_0V;
UINT8 g_Sim2VoltLevel = PAL_SIM_VOLTAGE_0V;		
UINT8 g_Sim3VoltLevel = PAL_SIM_VOLTAGE_0V;


UINT16 g_palEtuParaF[PAL_MAX_SIM] = {0};
UINT8   g_palEtuParaD[PAL_MAX_SIM] = {0};
UINT8  g_palEtuClkInvFlag[4]= {1,1,1,1};


UINT8 g_TS[PAL_MAX_SIM] = {0x3B, 0x3B, 0x3B, 0x3B};
HAL_SIM_CLOCK_STOP_MODE_T g_palSimClkStopMode[PAL_MAX_SIM] = {HAL_SIM_CLOCK_NO_STOP, HAL_SIM_CLOCK_NO_STOP, HAL_SIM_CLOCK_NO_STOP, HAL_SIM_CLOCK_NO_STOP};


#ifdef USER_SIM_SWAP

typedef  enum 
{
	PAL_SIM_ORDER_SIM_1,
	PAL_SIM_ORDER_SIM_2,
	PAL_SIM_ORDER_SIM_3,
	PAL_SIM_ORDER_SIM_4,
	PAL_SIM_ORDER_SIM_NUM
}PAL_SIM_LIST_T;

PAL_SIM_LIST_T g_palSimList[] = { PAL_SIM_LIST };

#endif

extern VOID hal_SimSetDataConvFormat(UINT32 Ts);


PUBLIC BOOL pal_SimInit(VOID)
{
    hal_SimOpen();

    PAL_TRACE(PAL_LEV(16), 0, "(pal_SimInit) GALLITE_8806");

    // set sim 0 as default sim
//    dualsim_Open();

    // Select null as default. Cannot call pal_EnableSimCard here, for
    // 1. DUALSIM_SIM_SELECT_NULL is not a valid SIM index;
    // 2. hal_SimSetDataConvFormat will set SCI_RESET, which will trigger a SIM reset

//    foursim_SetSimEnable_8806(DUALSIM_SIM_SELECT_NULL);
    pal_SimSetSimVoltage(PAL_SIM_VOLTAGE_NULL);

    return TRUE;
}



PUBLIC BOOL pal_SimClose(VOID)
{
    hal_SimClose();

    pal_SimSetSimVoltage(PAL_SIM_VOLTAGE_NULL);
//    dualsim_Close();

    return TRUE;
}

extern DUALSIM_ERR_T  foursim_SetSimEnable_8806(DUALSIM_SIM_CARD_T number);


// ============================================================================
//  pal_EnableSimCard
// ----------------------------------------------------------------------------
// This is used to select  sim-card , it enables switching between SIM 0 and 
//SIM 1 as well as setting parameters.
/// @paramsim_Index: the sim card index. 0 corresponding to sim card 0, 
///and 1 corresponding to sim card 1. 
// ============================================================================
PUBLIC BOOL pal_EnableSimCard(UINT8 sim_Index)    
{
	BOOL flag = 0;

//	UINT32 Enable_status = 0x51aa0000;
       UINT32 startTime;
	   
#ifdef USER_SIM_SWAP
	sim_Index = g_palSimList[sim_Index];
#endif

	PAL_TRACE(PAL_LEV(16), 0, "(pal_EnableSimCard) GALLITE_8806 sim ID = %d", sim_Index);
//	pal_HstSendEvent(Enable_status + sim_Index);
	
	pal_SetSimIndex(sim_Index);

	hal_SimSetEtuDuration(g_palEtuParaF[g_SimIndex], g_palEtuParaD[g_SimIndex], g_palEtuClkInvFlag[g_SimIndex]);	
	hal_SimSetClockStopMode(g_palSimClkStopMode[g_SimIndex]);
	hal_SimSetDataConvFormat(g_TS[g_SimIndex]);

    
      startTime = hal_TimGetUpTime(); 

      while(hal_SimGetClkStatus() && hal_SimGetClkStopStatus())//wait for clock stop
      {
          if (hal_TimGetUpTime() - startTime > 16) // if time > 1ms; break
          { 
              break; 
          } 
      }
  

	flag = foursim_SetSimEnable_8806(sim_Index);

	return (!flag);
}


// ============================================================================
// pal_SetSimIndex
// ----------------------------------------------------------------------------
/// This function used to set SIM index.
/// @param sim_Index is SIM index 
// ============================================================================
VOID pal_SetSimIndex(UINT8 sim_Index)
{
	g_SimIndex = sim_Index;
}



// ============================================================================
// pal_GetSimIndex
// ----------------------------------------------------------------------------
/// This function used to set SIM index.
/// @param sim_Index is SIM index 
// ============================================================================
VOID pal_GetSimIndex(UINT8 *sim_Index)
{
	*sim_Index = g_SimIndex;
}



PUBLIC VOID pal_SimSetClockStopMode(HAL_SIM_CLOCK_STOP_MODE_T mode)
{
    g_palSimClkStopMode[g_SimIndex] = mode;
    hal_SimSetClockStopMode(mode);
}

extern DUALSIM_ERR_T  foursim_SetSimVoltage_8806(DUALSIM_SIM_VOLTAGE_T sim0Level,DUALSIM_SIM_VOLTAGE_T sim1Level,
							DUALSIM_SIM_VOLTAGE_T sim2Level, DUALSIM_SIM_VOLTAGE_T sim3Level);


// ============================================================================
// pal_SimSetSimVoltage
// ----------------------------------------------------------------------------
/// WARNING
/// This function is called under IRQ. Fancy things like
/// call to sxr_Sleep or mailbox creating are FORBIDDEN
/// This function sets the SIM voltage.
/// four card case: need two step, step 1 set gallite 1203 voltage, step to set 6302 switch voltage
/// @param level Voltage to set.
// ============================================================================

PUBLIC BOOL pal_SimSetSimVoltage(UINT8 level)
{

	BOOL flag;
	UINT8 sim0Level = 0;
	UINT8 sim1Level = 0;
	UINT8 sim2Level = 0;
	UINT8 sim3Level = 0;

//	UINT32 sim_status = 0x61aa0000;
//	UINT32 Volt_status = 0x61bb0000;

	PAL_TRACE(PAL_LEV(16), 0, "(pal_SimSetSimVoltage) GALLITE_8806 power level = %d, g_SimIndex = %d", level, g_SimIndex);

//	pal_HstSendEvent(sim_status + g_SimIndex);
//	pal_HstSendEvent(Volt_status + level);
	
	if (g_SimIndex == 0)
	{
		sim0Level = level;
		sim1Level = g_Sim1VoltLevel;
		sim2Level = g_Sim2VoltLevel;
		sim3Level = g_Sim3VoltLevel;
			
		g_Sim0VoltLevel = level;
	}
	else if (g_SimIndex == 1)
	{
		sim0Level = g_Sim0VoltLevel;
		sim1Level = level;
		sim2Level = g_Sim2VoltLevel;
		sim3Level = g_Sim3VoltLevel;

		g_Sim1VoltLevel = level;
	}
	else if (g_SimIndex == 2)
	{
		sim0Level = g_Sim0VoltLevel;
		sim1Level = g_Sim1VoltLevel;
		sim2Level = level;
		sim3Level = g_Sim3VoltLevel;

		g_Sim2VoltLevel = level;
	}
	else if (g_SimIndex == 3)
	{
		sim0Level = g_Sim0VoltLevel;
		sim1Level = g_Sim1VoltLevel;
		sim2Level = g_Sim2VoltLevel;
		sim3Level = level;

		g_Sim3VoltLevel = level;
	}
	
	flag = foursim_SetSimVoltage_8806(sim0Level, sim1Level, sim2Level, sim3Level);

	return (!flag);
	
}


// ============================================================================
// pal_SimSetEtuDuration
// ----------------------------------------------------------------------------
/// Select the clock rate.
/// @param etuDuration F param to set.
/// @param etuDuration D param to set.
// ============================================================================
PUBLIC VOID pal_SimSetEtuDuration (UINT16 F, UINT8 D)
{
	hal_SimSetEtuDuration(F, D, g_palEtuClkInvFlag[g_SimIndex]);
	
	g_palEtuParaF[g_SimIndex] = F;
	g_palEtuParaD[g_SimIndex] = D;
}

// ============================================================================
// pal_SetTS
// ----------------------------------------------------------------------------
/// This function used to set ATR TS value.
/// @param TS is TS value from the ATR response
// ============================================================================
PUBLIC BOOL  pal_SetTS(UINT8 TS)
{
    extern UINT32 hal_SimGetDataConvFormat();
    UINT32 simDataConv = hal_SimGetDataConvFormat();
    BOOL needReset = FALSE;
    
    if(simDataConv == 1)
    {
        if(TS == 0x23)
        {
            TS = 0x3B;
            hal_SimSetDataConvFormat(TS);
            needReset = TRUE;
        }
    }
    else if(simDataConv == 2)
    {
        if(TS == 0x03)
        {
            TS = 0x3F;
            hal_SimSetDataConvFormat(TS);
            needReset = TRUE;
        }
    }
    else
    {
        PAL_ASSERT(FALSE, "Unsupported SIM data convention");
    }

    if(TS == 0x3B || TS == 0x3F)
    {
       g_TS[g_SimIndex] = TS;
    }
    else
    {
       SXS_TRACE(_SIM, " Bad TS \n");
    }

    return needReset;
}

// ============================================================================
// pal_getTS
// ----------------------------------------------------------------------------
/// This function used to get ATR TS value.
/// @param g_TS is TS value 
// ============================================================================
UINT8 pal_getTS()
{
	return g_TS[g_SimIndex];	
}



PUBLIC VOID pal_SimClkInv()
{
 	//PAL_TRACE(PAL_LEV(16), 0, "(pal_SimClkInv)");
 //	hal_HstSendEvent(0x12345678);
 g_palEtuClkInvFlag[g_SimIndex] = 1 - g_palEtuClkInvFlag[g_SimIndex];
hal_SimSetEtuDuration(g_palEtuParaF[g_SimIndex], g_palEtuParaD[g_SimIndex],  g_palEtuClkInvFlag[g_SimIndex]);	
}

#else //!GALLITE_IS_CT1129


// Common includes
#include "cs_types.h"


//PAL includes
#include "pal_sim.h"
#include "palp_cfg.h"
#include "pal.h"

// HAL includes
#include "hal_timers.h"
#include "hal_sim.h"
#include "sxs_io.h"
#include "pmd_m.h"

// Dual sim includes
#include "drv_dualsim.h"

#include "assert.h"

#define         NULLVOLT_DLY     (20)           // delay in 1/1000 sec
#define         SETVOLT_DLY      (50)           // delay in 1/1000 sec


UINT8 g_SimIndex 	 = 0;

UINT8 g_Sim0VoltLevel = PAL_SIM_VOLTAGE_0V;		//initial voltage
UINT8 g_Sim1VoltLevel = PAL_SIM_VOLTAGE_0V;
UINT8 g_Sim2VoltLevel = PAL_SIM_VOLTAGE_0V;		
UINT8 g_Sim3VoltLevel = PAL_SIM_VOLTAGE_0V;

UINT16 g_palEtuParaF[4] = {0};
UINT8  g_palEtuParaD[4] = {0};
UINT8  g_palEtuClkInvFlag[4]= {1,1,1,1};
UINT8  g_TS[4] = {0x3B, 0x3B, 0x3B, 0x3B};
HAL_SIM_CLOCK_STOP_MODE_T g_palSimClkStopMode[4] = {HAL_SIM_CLOCK_NO_STOP, HAL_SIM_CLOCK_NO_STOP, HAL_SIM_CLOCK_NO_STOP, HAL_SIM_CLOCK_NO_STOP};


#ifdef USER_SIM_SWAP

typedef  enum 
{
	PAL_SIM_ORDER_SIM_1,
	PAL_SIM_ORDER_SIM_2,
	PAL_SIM_ORDER_SIM_3,
	PAL_SIM_ORDER_SIM_4,
	PAL_SIM_ORDER_SIM_NUM
}PAL_SIM_LIST_T;

PAL_SIM_LIST_T g_palSimList[] = { PAL_SIM_LIST };

#endif

extern VOID hal_SimSetDataConvFormat(UINT32 Ts);

// ============================================================================
// pal_SimInit
// ----------------------------------------------------------------------------
// ============================================================================
//#if (NUMBER_OF_SIM==3)  || (NUMBER_OF_SIM==4)  

#ifdef SIM_SWITCH_USED

PUBLIC BOOL pal_SimInit(VOID)
{
    hal_SimOpen();

    PAL_TRACE(PAL_LEV(16), 0, "(pal_SimInit) FOUR_SIM_MODE");

#ifdef TGT_THREE_SIM
//    PAL_TRACE(PAL_LEV(16)|TSTDOUT, 0, "(TGT_THREE_SIM)  pal_SimInit ");
    threesim_Open();
#else
    foursim_Open();
#endif

    // Select null as default. Cannot call pal_EnableSimCard here, for
    // 1. DUALSIM_SIM_SELECT_NULL is not a valid SIM index;
    // 2. hal_SimSetDataConvFormat will set SCI_RESET, which will trigger a SIM reset
    PAL_TRACE(PAL_LEV(16), 0, "(foursim_SetSimEnable) ");

#ifdef TGT_THREE_SIM
    if(threesim_SelectSwitch(FOURSIM_SWITCH_0))	// select switch
    	return FALSE;
    if(threesim_SetSimEnable(DUALSIM_SIM_SELECT_NULL))
    	return FALSE;

    pal_SetSimIndex(PAL_SIM_0);
    pal_SimSetSimVoltage(PAL_SIM_VOLTAGE_NULL);	//initial sim 0,1 voltage

    if(threesim_SelectSwitch(FOURSIM_SWITCH_1))	// select switch
    	return FALSE;

    pal_SetSimIndex(PAL_SIM_2);
    pal_SimSetSimVoltage(PAL_SIM_VOLTAGE_NULL);	//initial sim 0,1 voltage

#else
    if(foursim_SelectSwitch(FOURSIM_SWITCH_0))	// select switch
    	return FALSE;
    if(foursim_SetSimEnable(DUALSIM_SIM_SELECT_NULL, FOURSIM_SWITCH_0))
    	return FALSE;

    pal_SetSimIndex(PAL_SIM_0);
    pal_SimSetSimVoltage(PAL_SIM_VOLTAGE_NULL);	//initial sim 0,1 voltage


    if(foursim_SelectSwitch(FOURSIM_SWITCH_1))
    	return FALSE;
    if(foursim_SetSimEnable(DUALSIM_SIM_SELECT_NULL, FOURSIM_SWITCH_1))
    	return FALSE;

    pal_SetSimIndex(PAL_SIM_2);
    pal_SimSetSimVoltage(PAL_SIM_VOLTAGE_NULL);	//initial sim 2,3 voltage

#endif

    return TRUE;	
}
#else 

PUBLIC BOOL pal_SimInit(VOID)
{
    hal_SimOpen();

#if (NUMBER_OF_SIM == 2)
    PAL_TRACE(PAL_LEV(16), 0, "(pal_SimInit) DUAL_SIM_MODE");

    // set sim 0 as default sim
    dualsim_Open();
    // Select null as default. Cannot call pal_EnableSimCard here, for
    // 1. DUALSIM_SIM_SELECT_NULL is not a valid SIM index;
    // 2. hal_SimSetDataConvFormat will set SCI_RESET, which will trigger a SIM reset
    dualsim_SetSimEnable(DUALSIM_SIM_SELECT_NULL);
    pal_SimSetSimVoltage(PAL_SIM_VOLTAGE_NULL);
#else
    sxs_fprintf(_SIM, "PAL: pal_SimInit SINGLE SIM Mode\n");
    // SCI init
    // Configure PMIC for level shift
    pmd_SetLevel(PMD_LEVEL_SIM, PMD_SIM_VOLTAGE_0V0);
#endif
    return TRUE;
}


#endif

// ============================================================================
// pal_SimClose
// ----------------------------------------------------------------------------
// ============================================================================
//#if (NUMBER_OF_SIM==3)  || (NUMBER_OF_SIM==4) 
#ifdef SIM_SWITCH_USED

PUBLIC BOOL pal_SimClose(VOID)
{
    PAL_TRACE(PAL_LEV(16), 0, "(pal_SimClose) FOUR_SIM_MODE");

    hal_SimClose();

#ifdef TGT_THREE_SIM
    if(threesim_SelectSwitch(FOURSIM_SWITCH_0))		// select switch 0
    	return FALSE;
    pal_SetSimIndex(PAL_SIM_0);
    pal_SimSetSimVoltage(PAL_SIM_VOLTAGE_NULL);

    if(threesim_SelectSwitch(FOURSIM_SWITCH_1))		// select switch 0
    	return FALSE;
    pal_SetSimIndex(PAL_SIM_2);
    pal_SimSetSimVoltage(PAL_SIM_VOLTAGE_NULL);

    threesim_Close();

#else
    if(foursim_SelectSwitch(FOURSIM_SWITCH_0))		// select switch 0
    	return FALSE;
    pal_SetSimIndex(PAL_SIM_0);
    pal_SimSetSimVoltage(PAL_SIM_VOLTAGE_NULL);

    if(foursim_SelectSwitch(FOURSIM_SWITCH_1))		// select switch 0
    	return FALSE;
    pal_SetSimIndex(PAL_SIM_2);
    pal_SimSetSimVoltage(PAL_SIM_VOLTAGE_NULL);

    foursim_Close();
#endif

    return TRUE;
}


#else
   
PUBLIC BOOL pal_SimClose(VOID)
{
    hal_SimClose();

#if (NUMBER_OF_SIM==2) 
    pal_SimSetSimVoltage(PAL_SIM_VOLTAGE_NULL);
    dualsim_Close();
#else
    // SCI init
    // Configure PMIC for level shift
    pmd_SetLevel(PMD_LEVEL_SIM, PMD_SIM_VOLTAGE_0V0);
#endif

    return TRUE;
}

#endif


#if (NUMBER_OF_SIM != 1) 
// ============================================================================
//  pal_EnableSimCard
// ----------------------------------------------------------------------------
// This is used to select  sim-card , it enables switching between SIM 0 and 
//SIM 1 as well as setting parameters.
/// @paramsim_Index: the sim card index. 0 corresponding to sim card 0, 
///and 1 corresponding to sim card 1. 
// ============================================================================
PUBLIC BOOL pal_EnableSimCard(UINT8 sim_Index)    
{
	BOOL flag = 0;
//#if (NUMBER_OF_SIM==3)  || (NUMBER_OF_SIM==4) 

#ifdef SIM_SWITCH_USED
	UINT8 SwitchNum ;
	UINT8 SimNum;
#endif	

#ifdef USER_SIM_SWAP
	sim_Index = g_palSimList[sim_Index];
#endif

#ifdef SIM_SWITCH_USED

	SwitchNum = sim_Index & 0x02;
	SimNum     = sim_Index & 0x01;

	if(SwitchNum ==2)
		SwitchNum = 1;
	
	if(sim_Index > FOURSIM_SIM_SELECT_CARD_3)
		return FALSE;
#endif	
	
	PAL_TRACE(PAL_LEV(16), 0, "(pal_EnableSimCard) sim ID = %d", sim_Index);

	pal_SetSimIndex(sim_Index);

	hal_SimSetEtuDuration(g_palEtuParaF[g_SimIndex], g_palEtuParaD[g_SimIndex], g_palEtuClkInvFlag[g_SimIndex]);
	hal_SimSetClockStopMode(g_palSimClkStopMode[g_SimIndex]);
	hal_SimSetDataConvFormat(g_TS[g_SimIndex]);

    {
	    UINT32 startTime = hal_TimGetUpTime(); 

	    while(hal_SimGetClkStatus() && hal_SimGetClkStopStatus())//wait for clock stop
	    {
	        if (hal_TimGetUpTime() - startTime > 16) // if time > 1ms; break
	        { 
	            break; 
	        } 
	    }
    }
		
//#if (NUMBER_OF_SIM==3)  || (NUMBER_OF_SIM==4) 
#ifdef SIM_SWITCH_USED

#ifdef TGT_THREE_SIM
//	PAL_TRACE(PAL_LEV(16)|TSTDOUT, 0, "(TGT_THREE_SIM) @ pal_EnableSimCard");

	if(SwitchNum == 0)
	{
		if(threesim_SelectSwitch(SwitchNum))	// select switch
			return FALSE;
		flag = threesim_SetSimEnable(SimNum);
	}
	else
		flag = threesim_SelectSwitch(SwitchNum);
	
#else
	if(foursim_SelectSwitch(SwitchNum))	// select switch
		return FALSE;
	flag = foursim_SetSimEnable(SimNum, SwitchNum);
#endif

#else
	flag = dualsim_SetSimEnable(sim_Index);
#endif

	return (!flag);
}
#endif

//#if (NUMBER_OF_SIM==3)  || (NUMBER_OF_SIM==4) 

#ifdef SIM_SWITCH_USED

BOOL  pal_SelectSwitch(UINT8 number)
{
//	PAL_TRACE(PAL_LEV(16), 0, "(pal_SelectSwitch) number = %d", number);

#ifdef TGT_THREE_SIM
    threesim_SelectSwitch(number);
#else
    foursim_SelectSwitch(number);
#endif
    return TRUE;
}

#endif



// ============================================================================
// pal_SetSimIndex
// ----------------------------------------------------------------------------
/// This function used to set SIM index.
/// @param sim_Index is SIM index 
// ============================================================================
VOID pal_SetSimIndex(UINT8 sim_Index)
{
	g_SimIndex = sim_Index;
}


// ============================================================================
// pal_GetSimIndex
// ----------------------------------------------------------------------------
/// This function used to set SIM index.
/// @param sim_Index is SIM index 
// ============================================================================
VOID pal_GetSimIndex(UINT8 *sim_Index)
{
	*sim_Index = g_SimIndex;
}



// TODO: rename pal_SimSetClockStopMode_new as pal_SimSetClockStopMode, and 
// take the place of original declaration in .h file
PUBLIC VOID pal_SimSetClockStopMode(HAL_SIM_CLOCK_STOP_MODE_T mode)
{
    g_palSimClkStopMode[g_SimIndex] = mode;
    hal_SimSetClockStopMode(mode);
}


// ============================================================================
// pal_SimSetVoltage
// ----------------------------------------------------------------------------
/// WARNING
/// This function is called under IRQ. Fancy things like
/// call to sxr_Sleep or mailbox creating are FORBIDDEN
/// This function sets the SIM voltage.
/// @param level Voltage to set.
// ============================================================================
PUBLIC VOID pal_SetSimVoltage1(UINT8 level)
{
    UINT32 delay;

    // Level 0 = Off
    // Level 1 = Class C (1.8V)
    // Level 2 = Class B (2.7 - 3.3V)
    // Level 3 = Class A (5V)

    delay = SETVOLT_DLY;
    
    switch (level) 
    {
        case PAL_SIM_VOLTAGE_CLASS_A:
            pmd_SetLevel(PMD_LEVEL_SIM, PMD_SIM_VOLTAGE_5V0);
        break;
        case PAL_SIM_VOLTAGE_CLASS_B:
            pmd_SetLevel(PMD_LEVEL_SIM, PMD_SIM_VOLTAGE_3V0);
        break;
        case PAL_SIM_VOLTAGE_CLASS_C:
            pmd_SetLevel(PMD_LEVEL_SIM, PMD_SIM_VOLTAGE_1V8);
        break;
        case PAL_SIM_VOLTAGE_NULL:
            pmd_SetLevel(PMD_LEVEL_SIM, PMD_SIM_VOLTAGE_0V0);
            // Add extra delay for voltage 0
            delay += NULLVOLT_DLY;
        break;
    }
    
    hal_TimDelay(delay MS_WAITING);
}

// ============================================================================
// pal_SimSetSimVoltage
// ----------------------------------------------------------------------------
/// WARNING
/// This function is called under IRQ. Fancy things like
/// call to sxr_Sleep or mailbox creating are FORBIDDEN
/// This function sets the SIM voltage.
/// four card case: need two step, step 1 set gallite 1203 voltage, step to set 6302 switch voltage
/// @param level Voltage to set.
// ============================================================================

//#if (NUMBER_OF_SIM==3)  || (NUMBER_OF_SIM==4) 
#ifdef SIM_SWITCH_USED

PUBLIC BOOL pal_SimSetSimVoltage(UINT8 level)
{

	BOOL flag = 0;
	UINT8 sim0Level = 0;
	UINT8 sim1Level = 0;
	UINT8 CsIdx = 0;

	PAL_TRACE(PAL_LEV(16), 0, "(pal_SimSetSimVoltage) FOUR CARD power level = %d, g_SimIndex = %d", level, g_SimIndex);

#ifdef TGT_THREE_SIM
	if(g_SimIndex < 2)
		flag = threesim_SetGalliteVoltage(DUALSIM_SIM_VOLTAGE_CLASS_B, g_Sim2VoltLevel);
	else
		flag = threesim_SetGalliteVoltage(DUALSIM_SIM_VOLTAGE_CLASS_B, level);
#else
	flag = foursim_SetGalliteVoltage(DUALSIM_SIM_VOLTAGE_CLASS_B, DUALSIM_SIM_VOLTAGE_CLASS_B);
#endif

	if(flag)
	    return (!flag);


	if (g_SimIndex == 0)
	{
		sim0Level = level;
		sim1Level = g_Sim1VoltLevel;
			
		g_Sim0VoltLevel = level;
//		CsIdx = 0;
	}
	else if (g_SimIndex == 1)
	{
		sim0Level = g_Sim0VoltLevel;
		sim1Level = level;

		g_Sim1VoltLevel = level;
//		CsIdx = 0;
	}
	else if (g_SimIndex == 2)
	{
		sim0Level = level;
		sim1Level = g_Sim3VoltLevel;

		g_Sim2VoltLevel = level;
		CsIdx = 1;
	}
	else if (g_SimIndex == 3)
	{
		sim0Level = g_Sim2VoltLevel;
		sim1Level = level;

		g_Sim3VoltLevel = level;
		CsIdx = 1;
	}

#ifdef TGT_THREE_SIM
	if(g_SimIndex < 2)
		flag = threesim_SetSimVoltage(sim0Level, sim1Level);
//	else
//		flag = threesim_SetGalliteVoltage(DUALSIM_SIM_VOLTAGE_CLASS_B, level);
#else
	flag = foursim_SetSimVoltage(sim0Level, sim1Level, CsIdx);
#endif

	return (!flag);

}

#else 

PUBLIC BOOL pal_SimSetSimVoltage(UINT8 level)
{
#if (NUMBER_OF_SIM == 1)
	pal_SetSimVoltage1(level);
	return TRUE; //Just satisfy compiler
#else

	BOOL flag;
	UINT8 sim0Level = 0;
	UINT8 sim1Level = 0;

	PAL_TRACE(PAL_LEV(16), 0, "(pal_SimSetSimVoltage) power level = %d, g_SimIndex = %d", level, g_SimIndex);
	
	if (g_SimIndex == 0)
	{
		sim0Level = level;
		sim1Level = g_Sim1VoltLevel;
			
		g_Sim0VoltLevel = level;
	}
	else if (g_SimIndex == 1)
	{
		sim0Level = g_Sim0VoltLevel;
		sim1Level = level;

		g_Sim1VoltLevel = level;
	}
	
	flag = dualsim_SetSimVoltage(sim0Level, sim1Level);

	return (!flag);
	
#endif

}

#endif

// ============================================================================
// pal_SimSetEtuDuration
// ----------------------------------------------------------------------------
/// Select the clock rate.
/// @param etuDuration F param to set.
/// @param etuDuration D param to set.
// ============================================================================
PUBLIC VOID pal_SimSetEtuDuration (UINT16 F, UINT8 D)
{
	hal_SimSetEtuDuration(F, D, g_palEtuClkInvFlag[g_SimIndex]);
	
	g_palEtuParaF[g_SimIndex] = F;
	g_palEtuParaD[g_SimIndex] = D;
}

// ============================================================================
// pal_SetTS
// ----------------------------------------------------------------------------
/// This function used to set ATR TS value.
/// @param TS is TS value from the ATR response
// ============================================================================
PUBLIC BOOL  pal_SetTS(UINT8 TS)
{
    extern UINT32 hal_SimGetDataConvFormat();
    UINT32 simDataConv = hal_SimGetDataConvFormat();
    BOOL needReset = FALSE;
    
    if(simDataConv == 1)
    {
        if(TS == 0x23)
        {
            TS = 0x3B;
            hal_SimSetDataConvFormat(TS);
            needReset = TRUE;
        }
    }
    else if(simDataConv == 2)
    {
        if(TS == 0x03)
        {
            TS = 0x3F;
            hal_SimSetDataConvFormat(TS);
            needReset = TRUE;
        }
    }
    else
    {
        PAL_ASSERT(FALSE, "Unsupported SIM data convention");
    }

    if(TS == 0x3B || TS == 0x3F)
    {
       g_TS[g_SimIndex] = TS;
    }
    else
    {
       SXS_TRACE(_SIM, " Bad TS \n");
    }

    return needReset;
}

// ============================================================================
// pal_getTS
// ----------------------------------------------------------------------------
/// This function used to get ATR TS value.
/// @param g_TS is TS value 
// ============================================================================
UINT8 pal_getTS()
{
	return g_TS[g_SimIndex];	
}
PUBLIC VOID pal_SimClkInv()
{
 	//PAL_TRACE(PAL_LEV(16), 0, "(pal_SimClkInv)");
 //	hal_HstSendEvent(0x12345678);
 g_palEtuClkInvFlag[g_SimIndex] = 1 - g_palEtuClkInvFlag[g_SimIndex];
hal_SimSetEtuDuration(g_palEtuParaF[g_SimIndex], g_palEtuParaD[g_SimIndex],  g_palEtuClkInvFlag[g_SimIndex]);	
}
#endif // GALLITE_IS_CT1129
#endif // PAL_SIMD_IMPLEMENTATION
