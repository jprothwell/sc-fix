////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                    //
//            Copyright (C) 2003-2008, Coolsand Technologies, Inc.                                         //
//                            All Rights Reserved                                                                            //
//                                                                                                                                    //
//      This source code is the property of Coolsand Technologies and is                                 //
//      confidential.  Any  modification, distribution,  reproduction or                                       //
//      exploitation  of  any content of this file is totally forbidden,                                          //
//      except  with the  written permission  of  Coolsand Technologies.                                  //
//                                                                                                                                   //
////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                    //
//  $HeadURL:                                                                                                                  //
//	$Author:                                                                                                                    // 
//	$Date: 2008-11-06                                                                                                    //   
//	$Revision:                                                                                                                 //   
//                                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                    //
///  @file drv_dualsim.h                                                                                                    //
///  This file contains the dual sim switch 6302 module driver API.                                        //
//                                                                                                                                   //
////////////////////////////////////////////////////////////////////////////////



#ifndef  _DRV_DUAL_SIM_H_
#define  _DRV_DUAL_SIM_H_


typedef enum
{
    /// Set the voltage class A (5 V)
    DUALSIM_SIM_VOLTAGE_CLASS_A = 0,
    /// Set the voltage class B (3 V)
    DUALSIM_SIM_VOLTAGE_CLASS_B = 1,
    /// Set the voltage class C (1,8 V)
    DUALSIM_SIM_VOLTAGE_CLASS_C = 2,
    /// Set the Sim voltage to  0 V
    DUALSIM_SIM_VOLTAGE_NULL    = 3
} DUALSIM_SIM_VOLTAGE_T;

typedef enum
{   
     //only select card 0   
     DUALSIM_SIM_SELECT_CARD_0 = 0,
     //only select card 1 
     DUALSIM_SIM_SELECT_CARD_1 = 1,
     //select card 0 and card 1
     DUALSIM_SIM_SELECT_CARD_0_1=2,
     //card0 and card1 CLK on
     DUALSIM_SIM_CLK_ON=3,
     // card0 and card1 CLK on ,select sim0 Data
     DUALSIM_SIM_CLK_ON_CARD_DATA_0=4,
     // card0 and card1 CLK on ,select sim1 Data
     DUALSIM_SIM_CLK_ON_CARD_DATA_1=5,
     //select NULL card
     DUALSIM_SIM_SELECT_NULL = 6

}DUALSIM_SIM_CARD_T;

// =============================================================================
// DUALSIM_ERR_T
// -----------------------------------------------------------------------------
/// Type used to describe the error status of the DUALSIM driver.
// =============================================================================
typedef enum
{
      /// No error occured
    DUALSIM_ERR_NO,

    /// An attempt to access a busy resource failed
    /// (Resource use not possible at the time)
    DUALSIM_ERR_RESOURCE_BUSY,

    /// Attempt to open a resource already opened,
    /// (no need to open again to use).
    DUALSIM_ERR_ALREADY_OPENED,
    
    ///  trying to access the resource not init
    DUALSIM_ERR_NOT_INIT,
    
    /// Invalid parameter
    DUALSIM_ERR_BAD_PARAMETER,

    /// The specified interface does not exist
    DUALSIM_ERR_NO_ITF,

    /// no sim card
    DUALSIM_ERR_NO_CARD,
    
    DUALSIM_ERR_QTY
}DUALSIM_ERR_T;

//#if (NUMBER_OF_SIM==3) ||(NUMBER_OF_SIM==4) 
typedef enum
{   
     FOURSIM_SIM_SELECT_CARD_0 = 0,     	//only select card 0   

     FOURSIM_SIM_SELECT_CARD_1 = 1,     	//only select card 1 

     FOURSIM_SIM_SELECT_CARD_2 = 2,     	//only select card 2   

     FOURSIM_SIM_SELECT_CARD_3 = 3,     	//only select card 3 

//     FOURSIM_SIM_SELECT_CARD_0_1=4,     //select card 0 and card 1

//     FOURSIM_SIM_CLK_ON=5,     			//card0 and card1 CLK on

     FOURSIM_SIM_SELECT_NULL = 6     		//select NULL card
}FOURSIM_SIM_CARD_T;

typedef enum
{   
     FOURSIM_SWITCH_0 = 0,     	//only select switch 0   

     FOURSIM_SWITCH_1 = 1,     	//only select switch 1 

     FOURSIM_BOTH_SWITCH = 3,    	//select switch 1 & 2
     
     FOURSIM_SELECT_NULL = 6	//select NULL card
}FOURSIM_SWITCH_T;


// =============================================================================
// DUALSIM_ERR_T
// -----------------------------------------------------------------------------
/// Type used to describe the error status of the DUALSIM driver.
// =============================================================================
typedef enum
{
    FOURSIM_ERR_NO ,      /// No error occured

    FOURSIM_ERR_RESOURCE_BUSY = 11,    /// An attempt to access a busy resource failed (Resource use not possible at the time)

    FOURSIM_ERR_ALREADY_OPENED,    	/// Attempt to open a resource already opened,  (no need to open again to use).
    
    FOURSIM_ERR_NOT_INIT,    		///  trying to access the resource not init
 
    FOURSIM_ERR_BAD_PARAMETER,    	/// Invalid parameter

    FOURSIM_ERR_NO_ITF,    			/// The specified interface does not exist

    FOURSIM_ERR_QTY
}FOURSIM_ERR_T;


//#endif



PUBLIC VOID dualsim_Open(VOID );
PUBLIC VOID dualsim_Close(VOID );
PUBLIC DUALSIM_ERR_T dualsim_SetSimVoltage(DUALSIM_SIM_VOLTAGE_T sim0Level,DUALSIM_SIM_VOLTAGE_T sim1Level);
PUBLIC DUALSIM_ERR_T dualsim_SetSimEnable(DUALSIM_SIM_CARD_T number);
//PROTECTED VOID dualsim_SendRegData(enum et6302_reg_map regIdx, UINT8 data);


//#if (NUMBER_OF_SIM==3) ||(NUMBER_OF_SIM==4) 

PUBLIC VOID threesim_Open(VOID);
PUBLIC VOID  threesim_Close(VOID);
PROTECTED FOURSIM_ERR_T  threesim_SpiActivateCs();
PROTECTED VOID threesim_SpiDeActivateCs();
PROTECTED VOID threesim_SetSim3VVoltage(DUALSIM_SIM_VOLTAGE_T level);
PROTECTED VOID threesim_SetSim1V8Voltage(DUALSIM_SIM_VOLTAGE_T level);
PROTECTED VOID threesim_SetSim0VVoltage(DUALSIM_SIM_VOLTAGE_T level);
PUBLIC DUALSIM_ERR_T  threesim_SetSimVoltage(DUALSIM_SIM_VOLTAGE_T sim0Level,DUALSIM_SIM_VOLTAGE_T sim1Level);
PUBLIC DUALSIM_ERR_T  threesim_SetGalliteVoltage(DUALSIM_SIM_VOLTAGE_T sim0Level,DUALSIM_SIM_VOLTAGE_T sim1Level);
PUBLIC FOURSIM_ERR_T  threesim_SelectSwitch(FOURSIM_SWITCH_T number);
PUBLIC FOURSIM_ERR_T  threesim_SetSimEnable(DUALSIM_SIM_CARD_T number);



PUBLIC VOID foursim_Open(VOID);
PUBLIC VOID foursim_Close(VOID );
PROTECTED FOURSIM_ERR_T  foursim_SpiActivateCs(UINT8 CsIdx);
PROTECTED VOID foursim_SpiDeActivateCs(UINT8 CsIdx);
PUBLIC DUALSIM_ERR_T foursim_SetSimVoltage(DUALSIM_SIM_VOLTAGE_T sim0Level,DUALSIM_SIM_VOLTAGE_T sim1Level, UINT8 CsIdx);
VOID dualsim_GetRegData(UINT8* data, UINT32 length, UINT8 CsIdx);
//UINT16 Drv_OpalSpiRead(UINT8 regIdx);

PUBLIC DUALSIM_ERR_T  foursim_SetGalliteVoltage(DUALSIM_SIM_VOLTAGE_T sim0Level,DUALSIM_SIM_VOLTAGE_T sim1Level);
PUBLIC FOURSIM_ERR_T  foursim_SelectSwitch(FOURSIM_SWITCH_T number);
PUBLIC FOURSIM_ERR_T foursim_SetSimEnable(DUALSIM_SIM_CARD_T number, UINT8 CsIdx);

//#endif

#endif




