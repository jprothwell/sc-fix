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
#include "stdlib.h"
#include "sxs_io.h"

#include "hal_timers.h"
#include "drv_dualsim.h"

#include "sxr_tls.h"

#define        DS_SETVOLT_DLY           (50) // delay in 1/1000 sec
#define        DS_SETSIMEN_DLY          (1)


extern DUALSIM_ERR_T pmd_DualSimSetVoltageClass(DUALSIM_SIM_VOLTAGE_T voltClass0,DUALSIM_SIM_VOLTAGE_T voltClass1);
extern BOOL pmd_SelectSimCard(UINT8 sim_card);


// ======================================================================
// dualsim_Open
// ----------------------------------------------------------------------
/// This function Open Spi for dualsim
// ======================================================================
PUBLIC VOID dualsim_Open(VOID)
{
}
   
// ======================================================================
// dualsim_Close
// ----------------------------------------------------------------------
/// This function Close dualsim Spi
// ======================================================================

PUBLIC VOID  dualsim_Close(VOID)
{
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

#ifdef DUALSIM_SWAP
    DUALSIM_SIM_VOLTAGE_T temp;
    temp = sim1Level;
    sim1Level = sim0Level;
    sim0Level = temp;
#endif // DUALSIM_SWAP

    errStatus = pmd_DualSimSetVoltageClass(sim0Level, sim1Level);
    if (errStatus == DUALSIM_ERR_NO)
    {
        delay = DS_SETVOLT_DLY;
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

    if (number != DUALSIM_SIM_SELECT_CARD_0 &&
        number != DUALSIM_SIM_SELECT_CARD_1)
    {
        return DUALSIM_ERR_NO;
    }

#ifdef DUALSIM_SWAP
    if (number == DUALSIM_SIM_SELECT_CARD_0)
    {
        number = DUALSIM_SIM_SELECT_CARD_1;
    }
    else if (number == DUALSIM_SIM_SELECT_CARD_1)
    {
        number = DUALSIM_SIM_SELECT_CARD_0;
    }
#endif // DUALSIM_SWAP

	
    if (pmd_SelectSimCard(number))
    {
        delay = DS_SETSIMEN_DLY;
        hal_TimDelay(delay MS_WAITING);
        
        return DUALSIM_ERR_NO;
    }
    
    return DUALSIM_ERR_RESOURCE_BUSY;
}



