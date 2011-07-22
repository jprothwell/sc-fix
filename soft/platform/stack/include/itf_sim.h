//------------------------------------------------------------------------------
//  $Log: itf_sim.h,v $
//  Revision 1.6  2006/06/28 12:28:33  aba
//  Access to EF_IMG file
//
//  Revision 1.5  2006/06/27 10:22:59  jrh
//  copyright update
//
//  Revision 1.4  2006/06/23 14:59:43  aba
//  Access to the EF_IMG file
//
//  Revision 1.3  2006/06/09 15:31:58  aba
//  *** empty log message ***
//
//  Revision 1.2  2005/12/29 17:38:44  pca
//  Log keyword added to all files
//
//------------------------------------------------------------------------------
/*
================================================================================

  This source code is property of StackCom. The information contained in this
  file is confidential. Distribution, reproduction, as well as exploitation,or
  transmission of any content of this file is not allowed except if expressly
  permitted. Infringements result in damage claims.

  Copyright StackCom (C) 2003-2006

================================================================================
*/

/*
================================================================================
  File       : itf_sim.h
--------------------------------------------------------------------------------

  Scope      : Interface of the sim software unit

  History    :
--------------------------------------------------------------------------------
  Jun 09 06  |  ABA   | Add EF_ICCID and EF_ELP
--------------------------------------------------------------------------------
  Nov 02 05  |  ABA   | Change Fetch length to 16 bits
--------------------------------------------------------------------------------
  Jun 06 05  |  ABA   | ISSUE 19: poll interval for simtoolkit
--------------------------------------------------------------------------------
  Sep 21 03  |  ABA   | Creation
================================================================================
*/
#ifndef __ITF_SIM_H__
#define __ITF_SIM_H__

#ifdef __SIM_C__
#define DefExtern
#else
#define DefExtern extern
#endif

#include "sxs_type.h"
#include "cmn_defs.h"

#define SIM_RESET_REQ             ( HVY_SIM + 1  )
#define SIM_RUN_GSM_ALGO_REQ      ( HVY_SIM + 2  )
#define SIM_READ_BINARY_REQ       ( HVY_SIM + 3  )
#define SIM_UPDATE_BINARY_REQ     ( HVY_SIM + 4  )
#define SIM_READ_RECORD_REQ       ( HVY_SIM + 5  )
#define SIM_UPDATE_RECORD_REQ     ( HVY_SIM + 6  )
#define SIM_INVALIDATE_REQ        ( HVY_SIM + 7  )
#define SIM_REHABILITATE_REQ      ( HVY_SIM + 8  )
#define SIM_VERIFY_CHV_REQ        ( HVY_SIM + 9  )
#define SIM_CHANGE_CHV_REQ        ( HVY_SIM + 10 )
#define SIM_DISABLE_CHV_REQ       ( HVY_SIM + 11 )
#define SIM_ENABLE_CHV_REQ        ( HVY_SIM + 12 )
#define SIM_UNBLOCK_CHV_REQ       ( HVY_SIM + 13 )
#define SIM_SEEK_REQ              ( HVY_SIM + 14 )
#define SIM_INCREASE_REQ          ( HVY_SIM + 15 )
#define SIM_DEDIC_FILE_STATUS_REQ ( HVY_SIM + 16 )
#define SIM_ELEM_FILE_STATUS_REQ  ( HVY_SIM + 17 )
#define SIM_TOOL_KIT_REQ          ( HVY_SIM + 18 )
#define SIM_TERMINAL_PROFILE_REQ  ( HVY_SIM + 19 )
#define SIM_ENVELOPE_REQ          ( HVY_SIM + 20 )
#define SIM_TERMINAL_RESPONSE_REQ ( HVY_SIM + 21 )
#define SIM_STOP_REQ              ( HVY_SIM + 22 )
#define SIM_POLL_REQ              ( HVY_SIM + 23 )
#define SIM_POLL_OFF_REQ          ( HVY_SIM + 24 )

#define SIM_STATUS_ERROR_IND      ( HVY_SIM + 25 )
#define SIM_FETCH_IND             ( HVY_SIM + 26 )

#define SIM_RESET_CNF             ( HVY_SIM + 27 )
#define SIM_RUN_GSM_ALGO_CNF      ( HVY_SIM + 28 )
#define SIM_READ_BINARY_CNF       ( HVY_SIM + 29 )
#define SIM_UPDATE_BINARY_CNF     ( HVY_SIM + 30 )
#define SIM_READ_RECORD_CNF       ( HVY_SIM + 31 )
#define SIM_UPDATE_RECORD_CNF     ( HVY_SIM + 32 )
#define SIM_INVALIDATE_CNF        ( HVY_SIM + 33 )
#define SIM_REHABILITATE_CNF      ( HVY_SIM + 34 )
#define SIM_VERIFY_CHV_CNF        ( HVY_SIM + 35 )
#define SIM_CHANGE_CHV_CNF        ( HVY_SIM + 36 )
#define SIM_DISABLE_CHV_CNF       ( HVY_SIM + 37 )
#define SIM_ENABLE_CHV_CNF        ( HVY_SIM + 38 )
#define SIM_UNBLOCK_CHV_CNF       ( HVY_SIM + 39 )
#define SIM_SEEK_CNF              ( HVY_SIM + 40 )
#define SIM_INCREASE_CNF          ( HVY_SIM + 41 )
#define SIM_DEDIC_FILE_STATUS_CNF ( HVY_SIM + 42 )
#define SIM_ELEM_FILE_STATUS_CNF  ( HVY_SIM + 43 )
#define SIM_TERMINAL_PROFILE_CNF  ( HVY_SIM + 44 )
#define SIM_ENVELOPE_CNF          ( HVY_SIM + 45 )
#define SIM_TERMINAL_RESPONSE_CNF ( HVY_SIM + 46 )
#define SIM_STATUS_TIMER_EXP     (( HVY_SIM + 47 )| SXS_TIMER_EVT_ID )
#define SIM_SLEEP_AFTER_EPPS      ( HVY_SIM + 48 )

// DON'T CHANGE THE PARAMETER ORDER  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// The sim software process all request in the same way so the paramter
// must be in the same order for the confirm message

#define SW1_NO_SIM_ANSWER 0xFF
#define SW2_NO_SIM_ANSWER 0xFF

#define SIM_NOT_PRESENT  0
#define SIM_NOT_READY    1
#define SIM_READY        2

typedef struct
{
  u8 Status;
} sim_NotifInd_t;


typedef struct
{
  u8 DfAddress[2];  // DfAddress[0] = upper, DfAddress[1] = lower
  u8 EfAddress[2];
}EfAddress_t;

typedef struct
{
  u8 DfAddress[2];  // DfAddress[0] = upper, DfAddress[1] = lower
}DfAddress_t;

// Elementary file name. These names must be used in all
// sim request message.
// Warning, if this list change, update also itf_api.h
#define  EF_ADN     0
#define  EF_FDN     1
#define  EF_SMS     2
#define  EF_CCP     3
#define  EF_MSISDN  4
#define  EF_SMSP    5
#define  EF_SMSS    6
#define  EF_LND     7
#define  EF_SMSR    8
#define  EF_SDN     9
#define  EF_EXT1    10
#define  EF_EXT2    11
#define  EF_EXT3    12
#define  EF_BDN     13
#define  EF_EXT4    14
#define  EF_LP        15
#define  EF_IMSI      16
#define  EF_KC        17
#define  EF_PLMNSEL   18
#define  EF_HPLMN     19
#define  EF_ACMMAX    20
#define  EF_SST       21
#define  EF_ACM       22
#define  EF_GID1      23
#define  EF_GID2      24
#define  EF_PUCT      25
#define  EF_CBMI      26
#define  EF_SPN       27
#define  EF_CBMID     28
#define  EF_BCCH      29
#define  EF_ACC       30
#define  EF_FPLMN     31
#define  EF_LOCI      32
#define  EF_AD        33
#define  EF_PHASE     34
#define  EF_VGCS      35
#define  EF_VGCSS     36
#define  EF_VBS       37
#define  EF_VBSS      38
#define  EF_EMLPP     39
#define  EF_AAEM      40
#define  EF_ECC       41
#define  EF_CBMIR     42
#define  EF_NIA       43
#define  EF_KCGPRS    44
#define  EF_LOCIGPRS  45
#define  EF_SUME      46
#define  EF_PLMNWACT  47
#define  EF_OPLMNWACT 48
#define  EF_HPLMNACT  49
#define  EF_CPBCCH    50
#define  EF_INVSCAN   51
#define  EF_RPLMNAC   52
#define  EF_ICCID     53
#define  EF_ELP       54
#define  EF_IMG       55
#define  EF_MBDN      56
#define  NB_ELEM_FILE 57

//This table indicates the path of each elementary file. Do not insert or delete
// an element of this table without updating the sim_EfName_t table
DefExtern EfAddress_t sim_ElementaryFileAddress[NB_ELEM_FILE]
#ifdef __SIM_C__
=
{
  /* EF_ADN         */ {{0x7F,0x10},{0x6F,0x3A}},
  /* EF_FDN         */ {{0x7F,0x10},{0x6F,0x3B}},
  /* EF_SMS         */ {{0x7F,0x10},{0x6F,0x3C}},
  /* EF_CCP         */ {{0x7F,0x10},{0x6F,0x3D}},
  /* EF_MSISDN      */ {{0x7F,0x10},{0x6F,0x40}},
  /* EF_SMSP        */ {{0x7F,0x10},{0x6F,0x42}},
  /* EF_SMSS        */ {{0x7F,0x10},{0x6F,0x43}},
  /* EF_LND         */ {{0x7F,0x10},{0x6F,0x44}},
  /* EF_SMSR        */ {{0x7F,0x10},{0x6F,0x47}},
  /* EF_SDN,        */ {{0x7F,0x10},{0x6F,0x49}},
  /* EF_EXT1        */ {{0x7F,0x10},{0x6F,0x4A}},
  /* EF_EXT2        */ {{0x7F,0x10},{0x6F,0x4B}},
  /* EF_EXT3        */ {{0x7F,0x10},{0x6F,0x4C}},
  /* EF_BDN         */ {{0x7F,0x10},{0x6F,0x4D}},
  /* EF_EXT4        */ {{0x7F,0x10},{0x6F,0x4E}},
  /* EF_LP          */ {{0x7F,0x20},{0x6F,0x05}},
  /* EF_IMSI        */ {{0x7F,0x20},{0x6F,0x07}},
  /* EF_KC          */ {{0x7F,0x20},{0x6f,0x20}},
  /* EF_PLMNSEL     */ {{0x7F,0x20},{0x6F,0x30}},
  /* EF_HPLMN       */ {{0x7F,0x20},{0x6F,0x31}},
  /* EF_ACMMAX      */ {{0x7F,0x20},{0x6F,0x37}},
  /* EF_SST         */ {{0x7F,0x20},{0x6F,0x38}},
  /* EF_ACM         */ {{0x7F,0x20},{0x6F,0x39}},
  /* EF_GID1        */ {{0x7F,0x20},{0x6F,0x3E}},
  /* EF_GID2        */ {{0x7F,0x20},{0x6F,0x3F}},
  /* EF_PUCT        */ {{0x7F,0x20},{0x6F,0x41}},
  /* EF_CBMI        */ {{0x7F,0x20},{0x6F,0x45}},
  /* EF_SPN         */ {{0x7F,0x20},{0x6F,0x46}},
  /* EF_CBMID       */ {{0x7F,0x20},{0x6F,0x48}},
  /* EF_BCCH        */ {{0x7F,0x20},{0x6F,0x74}},
  /* EF_ACC         */ {{0x7F,0x20},{0x6F,0x78}},
  /* EF_FPLMN       */ {{0x7F,0x20},{0x6F,0x7B}},
  /* EF_LOCI        */ {{0x7F,0x20},{0x6F,0x7E}},
  /* EF_AD          */ {{0x7F,0x20},{0x6F,0xAD}},
  /* EF_PHASE       */ {{0x7F,0x20},{0x6F,0xAE}},
  /* EF_VGCS        */ {{0x7F,0x20},{0x6F,0xB1}},
  /* EF_VGCSS       */ {{0x7F,0x20},{0x6F,0xB2}},
  /* EF_VBS         */ {{0x7F,0x20},{0x6F,0xB3}},
  /* EF_VBSS        */ {{0x7F,0x20},{0x6F,0xB4}},
  /* EF_EMLPP       */ {{0x7F,0x20},{0x6F,0xB5}},
  /* EF_AAEM        */ {{0x7F,0x20},{0x6F,0xB6}},
  /* EF_ECC         */ {{0x7F,0x20},{0x6F,0xB7}},
  /* EF_CBMIR       */ {{0x7F,0x20},{0x6F,0x50}},
  /* EF_NIA         */ {{0x7F,0x20},{0x6F,0x51}},
  /* EF_KCGPRS      */ {{0x7F,0x20},{0x6F,0x52}},
  /* EF_LOCIGPRS    */ {{0x7F,0x20},{0x6F,0x53}},
  /* EF_SUME        */ {{0x7F,0x20},{0x6F,0x54}},
  /* EF_PLMNWACT    */ {{0x7F,0x20},{0x6F,0x60}},
  /* EF_OPLMNWACT   */ {{0x7F,0x20},{0x6F,0x61}},
  /* EF_HPLMNACT    */ {{0x7F,0x20},{0x6F,0x62}},
  /* EF_CPBCCH      */ {{0x7F,0x20},{0x6F,0x63}},
  /* EF_INVSCAN     */ {{0x7F,0x20},{0x6F,0x64}},
  /* EF_RPLMNACT    */ {{0x7F,0x20},{0x6F,0x65}},
  /* EF_ICCID       */ {{0x3F,0x00},{0x2F,0xE2}},
  /* EF_ELP         */ {{0x3F,0x00},{0x2F,0x05}},
  /* EF_IMG         */ {{0x7F,0x10},{0x4F,0x20}},
  /* EF_MBDN        */ {{0x7F,0x20},{0x6F,0xC7}}
};
#else
;
#endif

#define  DF_GSM        0
#define  DF_TELECOM    1
#define  NB_DEDIC_FILE 2

//This table indicates the path of each elementary file. Do not insert or delete
// an element of this table without updating the sim_EfName_t table
DefExtern DfAddress_t sim_DedicatedFileAddress[NB_DEDIC_FILE]
#ifdef __SIM_C__
=
{
  /* DF_GSM         */ {{0x7F,0x20}},
  /* DF_TELECOM     */ {{0x7F,0x10}}
};
#else
;
#endif



// ************************* SIM_RESET
//typedef struct
//{
//}sim_ResetReq_t;
#define NO_RESET_RESPONSE             0
#define WRONG_ATR                     1
#define RESET_SUCCESS_FIRST_ATTEMPT   2
#define RESET_SUCCESS_SEVERAL_ATTEMPT 3

typedef struct
{
  u8 ResetResult;
  u8 Sw1;
  u8 Sw2;
  u8 Data[256]; // Status of the DF_TELECOM
}sim_ResetCnf_t;

// ************************* SIM_RUN_ALGO
typedef struct
{
  u8 SrcMbx;             // Used to send the response
  u8 CKSN;
  u8 Rand[16];
}sim_RunGsmAlgoReq_t;

typedef struct
{
  u8 Sw1;
  u8 Sw2;
  u8 SRES[4];
  u8 Kc[8];
}sim_RunGsmAlgoCnf_t;

// ************************* SIM_READ_BINARY
typedef struct
{
  u8  SrcMbx;             // Used to send the response
  u8  File;
  u16 Offset;
  u8  NbByte;
}sim_ReadBinaryReq_t;

typedef struct
{
  u8 Sw1;
  u8 Sw2;
  u8 File;
  u8 Data[256];
}sim_ReadBinaryCnf_t;

// ************************* SIM_UPDATE_BINARY
typedef struct
{
  u8  SrcMbx;             // Used to send the response
  u8  File;
  u16 Offset;
  u8  NbByte;
  u8  Data[256];
}sim_UpdateBinaryReq_t;

typedef struct
{
  u8 Sw1;
  u8 Sw2;
  u8 File;
}sim_UpdateBinaryCnf_t;

// ************************* SIM_READ_RECORD

#define SIM_NEXT_RECORD                   2
#define SIM_PREVIOUS_RECORD               3
#define SIM_ABSOLUTE_MODE_OR_CURRENT_MODE 4

typedef struct
{
  u8  SrcMbx;             // Used to send the response
  u8  File;
  u8  RecordNb;
  u8  Mode;
  u8  NbByte;
}sim_ReadRecordReq_t;

typedef struct
{
  u8  Sw1;
  u8  Sw2;
  u8  File;
  u8  Data[256];
}sim_ReadRecordCnf_t;

// ************************* SIM_UPDATE_RECORD
typedef struct
{
  u8  SrcMbx;             // Used to send the response
  u8  File;
  u8  RecordNb;
  u8  Mode;
  u8  NbByte;
  u8  Data[256];
}sim_UpdateRecordReq_t;

typedef struct
{
  u8 Sw1;
  u8 Sw2;
  u8 File;
}sim_UpdateRecordCnf_t;

// ************************* SIM_INVALIDATE
typedef struct
{
  u8  SrcMbx;             // Used to send the response
  u8  File;
}sim_InvalidateReq_t;

typedef struct
{
  u8 Sw1;
  u8 Sw2;
  u8 File;
}sim_InvalidateCnf_t;

// ************************* SIM_REHABILITATE
typedef struct
{
  u8  SrcMbx;             // Used to send the response
  u8  File;
}sim_RehabilitateReq_t;

typedef struct
{
  u8 Sw1;
  u8 Sw2;
  u8 File;
}sim_RehabilitateCnf_t;

// ************************* SIM_SEEK
typedef struct
{
  u8  SrcMbx;             // Used to send the response
  u8  TypeMode;
  u8  File;
  u8  PatternNbByte;
  u8  Pattern[256];
}sim_SeekReq_t;

typedef struct
{
  u8 Sw1;
  u8 Sw2;
  u8 RecordNb;  // only for type 2 seek (meaningless for type 1 seek)
}sim_SeekCnf_t;

// ************************* SIM_INCREASE
typedef struct
{
  u8  SrcMbx;             // Used to send the response
  u8  ValueToBeAdded[3];
  u8  File;
}sim_IncreaseReq_t;

typedef struct
{
  u8 Sw1;
  u8 Sw2;
  u8 File;
  u8 RecordDataAndAddedValue[256+3];
}sim_IncreaseCnf_t;

// ************************* SIM_VERIFY_CHV
typedef struct
{
  u8 SrcMbx;             // Used to send the response
  u8 CHVNumber;
  u8 CHVValue[8];
}sim_VerifyCHVReq_t;

typedef struct
{
  u8 Sw1;
  u8 Sw2;
}sim_VerifyCHVCnf_t;

// ************************* SIM_CHANGE_CHV
typedef struct
{
  u8 SrcMbx;             // Used to send the response
  u8 CHVNumber;
  u8 OldCHVValue[8];
  u8 NewCHVValue[8];
}sim_ChangeCHVReq_t;

typedef struct
{
  u8 Sw1;
  u8 Sw2;
}sim_ChangeCHVCnf_t;

// ************************* SIM_DISABLE_CHV
typedef struct
{
  u8 SrcMbx;             // Used to send the response
  u8 CHV1Value[8];
}sim_DisableCHVReq_t;

typedef struct
{
  u8 Sw1;
  u8 Sw2;
}sim_DisableCHVCnf_t;

// ************************* SIM_ENABLE_CHV
typedef struct
{
  u8 SrcMbx;             // Used to send the response
  u8 CHV1Value[8];
}sim_EnableCHVReq_t;

typedef struct
{
  u8 Sw1;
  u8 Sw2;
}sim_EnableCHVCnf_t;

// ************************* SIM_UNBLOCK_CHV
typedef struct
{
  u8 SrcMbx;             // Used to send the response
  u8 CHVNumber;
  u8 UnblockCHVValue[8];
  u8 NewCHVValue[8];
}sim_UnblockCHVReq_t;

typedef struct
{
  u8 Sw1;
  u8 Sw2;
}sim_UnblockCHVCnf_t;

// ************************* SIM_ELEM_STATUS
typedef struct
{
  u8  SrcMbx;             // Used to send the response
  u8  File;
}sim_ElemFileStatusReq_t;

typedef struct
{
  u8 Sw1;
  u8 Sw2;
  u8 File;
  u8 Status[256];
}sim_ElemFileStatusCnf_t;

// ************************* SIM_DEDIC_STATUS
typedef struct
{
  u8  SrcMbx;             // Used to send the response
  u8  File;
}sim_DedicFileStatusReq_t;

typedef struct
{
  u8 Sw1;
  u8 Sw2;
  u8 File;
  u8 Status[256];
}sim_DedicFileStatusCnf_t;

// ************************* SIM_TOOLKIT
typedef struct
{
  u8   FetchMbx;             // Used to send fetch command
  bool Activate;             // TRUE => Activate    FALSE => Desactivate
}sim_ToolKitReq_t;

// ************************* SIM_TERMINAL_PROFILE_REQ
typedef struct
{
  u8 SrcMbx;             // Used to send the response
  u8 Length;
  u8 ProfileData[256];
}sim_TerminalProfileReq_t;

// ************************* SIM_TERMINAL_PROFILE_CNF
typedef struct
{
   u8 Sw1;
   u8 Sw2;
}sim_TerminalProfileCnf_t;

// ************************* SIM_ENVELOPE_REQ
typedef struct
{
  u8 SrcMbx;             // Used to send the response
  u8 Length;
  u8 EnvelopeData[256];
}sim_EnvelopeReq_t;

// ************************* SIM_ENVELOPE_CNF
typedef struct
{
  u8 Sw1;
  u8 Sw2;   
  u8 NbByte;
  u8 EnvelopeData[256];
}sim_EnvelopeCnf_t;

// ************************* SIM_TERMINAL_RESPONSE_REQ/
typedef struct
{
  u8 SrcMbx;             // Used to send the response
  u8 Length;
  u8 Data[256];
}sim_TerminalResponseReq_t;

// ************************* SIM_TERMINAL_RESPONSE_CNF
typedef struct
{
   u8 Sw1;
   u8 Sw2;
}sim_TerminalResponseCnf_t;

// ************************* SIM_FETCH
typedef struct
{
  u16 Len            ;
  u8  FetchData[260] ;
}sim_FetchInd_t;

// ************************* SIM_POLL_REQ
typedef struct
{
  u8 SrcMbx;             // Used to send the response
  u32 Duration;
}sim_PollReq_t;

// ************************* SIM_STATUS_ERROR
#define NO_SIM_ANSWER    0
#define BAD_SIM_RESPONSE 1
typedef struct
{
  u8 StatusError;
}sim_StatusErrorInd_t;

typedef struct
{
  u8 Sw1;
  u8 Sw2;
  u8 Data[260];
}sim_Cnf_t;

typedef struct
{
  u8 Sw1;
  u8 Sw2;
  u8 File;
  u8 Data[260];
}sim_CnfWithFile_t;


#undef DefExtern

#endif  // __ITF_SIM_H__
