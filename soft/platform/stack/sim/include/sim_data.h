//------------------------------------------------------------------------------
//  $Log: sim_data.h,v $
//  Revision 1.3  2006/05/22 20:51:18  aba
//  Update of the copyright
//
//  Revision 1.2  2005/12/29 17:39:16  pca
//  Log keyword added to all files
//
//------------------------------------------------------------------------------
/*
================================================================================

  This source code is property of StackCom. The information contained in this
  file is confidential. Distribution, reproduction, as well as exploitation,or
  transmisison of any content of this file is not allowed except if expressly
  permitted. Infringements result in damage claims.

  Copyright StackCom (C) 2003 2004 2005 2006

================================================================================
*/

/*
================================================================================
  File       : sim_data.h
--------------------------------------------------------------------------------

  Scope      : Declaration of SIM context variables

  History    :
--------------------------------------------------------------------------------
  Jun 06 05  |  ABA   | ISSUE 19: poll interval for simtoolkit
--------------------------------------------------------------------------------
  Sep 28 03  |  ABA   | Creation
  
  --------------------------------------------------------------------------------
  Oct. 10 08  |  Wangxd   | Modify structure variable  sim_Data to pointer vatiable pSimCtx
  --------------------------------------------------------------------------------
  Oct. 21 08  |  Wangxd   | Add new feature for dual sim function
  
================================================================================
*/
#ifndef __SIM_DATA_H__
#define __SIM_DATA_H__

#include "sxr_cnf.h"

#include "sim_msg.h"
#include "sim_prot.h"

#ifdef __SIM_DATA__
#define DefExtern
#else
#define DefExtern extern
#endif
typedef struct
{
  u8 Upper;
  u8 Lower;
}DFAddress_t;
typedef struct
{
  Msg_t       * RecMsg    ;         // ptr to the current received message; NIL if evt
  Msg_t       * OutMsg    ;
  InEvtDsc_t  * IEvtDsc   ;         // Incoming event
  OutEvtDsc_t * OEvtDsc   ;         // Outgoing event

  bool          KeepMsg   ;         // TRUE if RecMsg must not be deleted after fsm_Run

#ifndef STK_SINGLE_SIM
  u32           SimId     ;         // Identifier the number of SIM card
  u8            CheckSwitch;
#else
  u8            Semaphore ;         // To avoid simultanneous sim request processing
#endif

  Instruction_t MainInstruction;
  Instruction_t CurrentInstruction;
  u32           ResponseId;
  u8            EfFile;
  SimState_t    SimState;
  u8            SrcMbx;             // Used to send the cnf
  u16           NbByteToReceive;
  u16           NbReceivedByte;
  u8          * PtRxBuffer;         // Dynamic memory used to store the data received from
                                    // the sim (also used by pal)
  u8          * PtRxDataCnf;        // Pointer on the data for request with data receiving
  u16           NbRxDataCnf;
  u8            GetResponseLength;  // SW2 when SW1=0x9F
  // SIM TOOL KIT
  bool          SimToolKitActivate; // Used to send or not the fetch commùand
  u8            SimToolKitDestMbx;  // Destination of the fetch commands
  u16           FetchLength;        // SW2 when SW1=0x91
  // STATUS TIMER (sim presence and simtoolkit)
  bool          StatusTimer;
  DFAddress_t   CurrentDFAddress;
  DFAddress_t   CurrentEFAddress;
  u8            StatusP3;           // Used for initial status, periodic status and status request
  u32           StatusTimerDuration; // For periodical status (unit is ms)
  // SIM RESET
  SimResetState_t SimResetState;
  u8            VoltageClass;
  u8            NbReset;
  u8            InterruptCause;
  bool          ResetDone;
  bool          InitialStatus;       // TRUE during processing of the status of the sim reset
  bool          CnfWithFile;         // TRUE if the cnf message must indicates the file number
  bool          ExtraRxInt;
  u8            NbEnhancedPPS;
  bool          ExpectedInt;         // TRUE if the interrupt is expected, unexpected interrupt are discarded
  bool          Timer1sRunning;
  u8            ClockStopMode;
  u8            ResendTimes;

} sim_Data_t ;


DefExtern sim_Data_t *pSimCtx ;


#ifndef STK_SINGLE_SIM

#define SIM_ID_0     0
#define SIM_ID_1     1

DefExtern sim_Data_t sim_Data[STK_MAX_SIM] ;

DefExtern u8 g_Semaphore ;          // global semaphore variable to avoid simultaneous sim request processing.
#else
DefExtern sim_Data_t sim_Data ;
#endif



#undef DefExtern

#endif // __SIM_DATA_H__
