//==============================================================================
//                                                                              
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.              
//                            All Rights Reserved                               
//                                                                              
//      This source code is the property of Coolsand Technologies and is        
//      confidential.  Any  modification, distribution,  reproduction or        
//      exploitation  of  any content of this file is totally forbidden,        
//      except  with the  written permission  of  Coolsand Technologies.        
//                                                                              
//==============================================================================
//                                                                              
//    THIS FILE WAS GENERATED FROM ITS CORRESPONDING XML VERSION WITH COOLXML.  
//                                                                              
//                       !!! PLEASE DO NOT EDIT !!!                             
//                                                                              
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/bcpu/spp/include/spp_gsm.h $                                                                   
//  $Author: kongdq $                                                                    
//  $Date: 2011-04-11 19:51:51 +0800 (Mon, 11 Apr 2011) $                                                                      
//  $Revision: 7030 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
/// This document describes the structures and functions provided to SPC for both
/// baseband processing and low-level HW control. Includes the definition of the
/// various buffers used by HW modules during Rx and Tx.
//
//==============================================================================

#ifndef _SPP_GSM_H_
#define _SPP_GSM_H_


#include "cs_types.h"
#include "gsm.h"

// =============================================================================
//  MACROS
// =============================================================================
/// size of the memory used for saving the IR blocks
#define SPP_EGPRS_IR_MEM_LEN                     (52*1024)
/// TBF modulo
#define SPP_EGPRS_IR_SNS                         (2048)
/// Size in bytes of on one Tx burst buffer
#define SPP_EGPRS_TX_BURST_LEN                   (44)
/// size in bytes of one received MCS1 to MCS4 block prior to decoding
#define MCS_1_TO_4_UNCOMPRESSED_BLOCK_LEN        (372)
/// size in bytes of one received MCS5 to MCS6 block prior to decoding
#define MCS_5_TO_6_UNCOMPRESSED_BLOCK_LEN        (1248)
/// size in bytes of one received MCS7 to MCS9 block prior to decoding
#define MCS_7_TO_9_UNCOMPRESSED_BLOCK_LEN        (612)
/// size in bytes of one compressed MCS1 to MCS4 block prior to decoding
#define MCS_1_TO_4_BLOCK_LEN                     (MCS_1_TO_4_UNCOMPRESSED_BLOCK_LEN/2)
/// size in bytes of one compressed MCS5 to MCS6 block prior to decoding
#define MCS_5_TO_6_BLOCK_LEN                     (MCS_5_TO_6_UNCOMPRESSED_BLOCK_LEN/2)
/// size in bytes of one compressed MCS7 to MCS9 block prior to decoding
#define MCS_7_TO_9_BLOCK_LEN                     (MCS_7_TO_9_UNCOMPRESSED_BLOCK_LEN/2)
/// number of compressed MCS1 to MCS4 blocks that can be stored in the IR memory
#define MCS_1_TO_4_BUFFER_LEN                    (SPP_EGPRS_IR_MEM_LEN/MCS_1_TO_4_BLOCK_LEN)
/// number of compressed MCS5 to MCS6 blocks that can be stored in the IR memory
#define MCS_5_TO_6_BUFFER_LEN                    (SPP_EGPRS_IR_MEM_LEN/MCS_5_TO_6_BLOCK_LEN)
/// number of compressed MCS7 to MCS9 blocks that can be stored in the IR memory
#define MCS_7_TO_9_BUFFER_LEN                    (SPP_EGPRS_IR_MEM_LEN/MCS_7_TO_9_BLOCK_LEN)
/// maximum size of one received blocks regardless of the mode
#define MCS_MAX_BLOCK_LEN                        (MCS_5_TO_6_BLOCK_LEN)
/// maximum number of blocks that can contain the IR memory regardless of the mode
#define MCS_MAX_BUFFER_LEN                       (MCS_1_TO_4_BUFFER_LEN)
/// size of table that gives the correct coding scheme between MCS7, MCS8 and MCS9
/// after the header decoding
#define SPP_EGPRS_SIZE_HEAD_DECODE_MCS789        (29)
/// size of table that gives the correct coding scheme between MCS5 and MCS6 after
/// the header decoding
#define SPP_EGPRS_SIZE_HEAD_DECODE_MCS56         (6)
/// size of table that gives the correct coding scheme between MCS1, MCS2, MCS3 and
/// MCS4 after the header decoding
#define SPP_EGPRS_SIZE_HEAD_DECODE_MCS1234       (13)
/// length of the training sequence
#define TRAIN_SEQ_LENGTH                         (26)
/// length of the training sequence
#define HALF_BURST_LEN                           (61)
/// length of the training sequence
#define SPP_LSE_TAP_NB                           (8)
/// quality factor for the prefilter
#define QP                                       (10)
/// number of taps used in the DDFSE treillis
#define MAIN_TAP_LEN                             (2)
/// number of taps used in the DDFSE equalizer
#define ALL_TAP_LEN                              (6)
/// size of the treillis for 8PSK equal to 8**(MAIN_TAP_LEN-1)
#define STATE_LEN                                (8)
/// length of the HASH TABLE (for IR)
#define HASH_LEN                                 (64)

// ============================================================================
// SPP_EGPRS_HEADER_DATA_DISCRIMINATOR_T
// -----------------------------------------------------------------------------
/// discrimination between header and data for Egprs
// =============================================================================
typedef enum
{
    HEAD_COD                                    = 0x00000000,
    DATA_COD                                    = 0x00000001
} SPP_EGPRS_HEADER_DATA_DISCRIMINATOR_T;

/// crc decoding successful
#define SUCCESS                                  (0)
/// crc decoding failing
#define FAIL                                     (1)

// ============================================================================
// SPP_CODING_SCHEME_T
// -----------------------------------------------------------------------------
/// CS that doesn't need CRC/CONV params are packed toghether at the END to reduce
/// tables
// =============================================================================
typedef enum
{
    CS_RACH                                     = 0x00000000,
    CS_PRACH                                    = 0x00000001,
    CS_SCH                                      = 0x00000002,
    CS_CS1                                      = 0x00000003,
    CS_CS2                                      = 0x00000004,
    CS_CS3                                      = 0x00000005,
    CS_CS4                                      = 0x00000006,
    CS_HEAD_1                                   = 0x00000007,
    CS_HEAD_2                                   = 0x00000008,
    CS_HEAD_3                                   = 0x00000009,
    CS_MCS1                                     = 0x0000000A,
    CS_MCS2                                     = 0x0000000B,
    CS_MCS3                                     = 0x0000000C,
    CS_MCS4                                     = 0x0000000D,
    CS_MCS5                                     = 0x0000000E,
    CS_MCS6                                     = 0x0000000F,
    CS_MCS7                                     = 0x00000010,
    CS_MCS8                                     = 0x00000011,
    CS_MCS9                                     = 0x00000012,
    CS_HR                                       = 0x00000013,
    CS_FR                                       = 0x00000014,
    CS_EFR                                      = 0x00000015,
    CS_H24                                      = 0x00000016,
    CS_H48                                      = 0x00000017,
    CS_F24                                      = 0x00000018,
    CS_F48                                      = 0x00000019,
    CS_F96                                      = 0x0000001A,
    CS_F144                                     = 0x0000001B,
    CS_AFS_SPEECH_122                           = 0x0000001C,
    CS_AFS_SPEECH_102                           = 0x0000001D,
    CS_AFS_SPEECH_795                           = 0x0000001E,
    CS_AFS_SPEECH_74                            = 0x0000001F,
    CS_AFS_SPEECH_67                            = 0x00000020,
    CS_AFS_SPEECH_59                            = 0x00000021,
    CS_AFS_SPEECH_515                           = 0x00000022,
    CS_AFS_SPEECH_475                           = 0x00000023,
    CS_AHS_SPEECH_795                           = 0x00000024,
    CS_AHS_SPEECH_74                            = 0x00000025,
    CS_AHS_SPEECH_67                            = 0x00000026,
    CS_AHS_SPEECH_59                            = 0x00000027,
    CS_AHS_SPEECH_515                           = 0x00000028,
    CS_AHS_SPEECH_475                           = 0x00000029,
    CS_AFS_SID_UPDATE                           = 0x0000002A,
    CS_AFS_RATSCCH                              = 0x0000002B,
    CS_AHS_SID_UPDATE                           = 0x0000002C,
    CS_AHS_RATSCCH_DATA                         = 0x0000002D,
    CS_AFS_SID_FIRST                            = 0x0000002E,
    CS_AFS_ONSET                                = 0x0000002F,
    CS_AHS_SID_UPDATE_INH                       = 0x00000030,
    CS_AHS_SID_FIRST_P1                         = 0x00000031,
    CS_AHS_SID_FIRST_P2                         = 0x00000032,
    CS_AHS_SID_FIRST_INH                        = 0x00000033,
    CS_AHS_ONSET                                = 0x00000034,
    CS_AHS_RATSCCH_MARKER                       = 0x00000035,
    CS_QTY                                      = 0x00000036
} SPP_CODING_SCHEME_T;

/// Offset for the SRC tables
#define CS_NEED_SRC                              (CS_AFS_SPEECH_122)
/// Offset for Post SRC tables
#define CS_END_NEED_SRC                          (CS_AFS_SID_FIRST)
/// size of the CRC/CONV table
#define CS_PARAM_QTY                             (CS_END_NEED_SRC)
/// size of the SRC table
#define CS_SRC_PARAM_QTY                         (CS_END_NEED_SRC - CS_NEED_SRC)

// ============================================================================
// SPP_SPEECH_MODE_T
// -----------------------------------------------------------------------------
/// vocoder mode
// =============================================================================
typedef enum
{
    SPP_MR475_MODE                              = 0x00000000,
    SPP_MR515_MODE                              = 0x00000001,
    SPP_MR59_MODE                               = 0x00000002,
    SPP_MR67_MODE                               = 0x00000003,
    SPP_MR74_MODE                               = 0x00000004,
    SPP_MR795_MODE                              = 0x00000005,
    SPP_MR102_MODE                              = 0x00000006,
    SPP_MR122_MODE                              = 0x00000007,
    SPP_FR_MODE                                 = 0x00000008,
    SPP_HR_MODE                                 = 0x00000009,
    SPP_EFR_MODE                                = 0x0000000A,
    SPP_INVALID_MODE                            = 0x0000000B
} SPP_SPEECH_MODE_T;


// ============================================================================
// SPP_RX_FRAME_TYPE_T
// -----------------------------------------------------------------------------
/// AMR enum
// =============================================================================
typedef enum
{
    SPP_RX_SPEECH_GOOD                          = 0x00000000,
    SPP_RX_SPEECH_DEGRADED                      = 0x00000001,
    SPP_RX_ONSET                                = 0x00000002,
    SPP_RX_SPEECH_BAD                           = 0x00000003,
    SPP_RX_SID_FIRST                            = 0x00000004,
    SPP_RX_SID_UPDATE                           = 0x00000005,
    SPP_RX_SID_BAD                              = 0x00000006,
    SPP_RX_NO_DATA                              = 0x00000007
} SPP_RX_FRAME_TYPE_T;


// ============================================================================
// SPP_TX_FRAME_TYPE_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef enum
{
    SPP_TX_SPEECH                               = 0x00000000,
    SPP_TX_SID_FIRST                            = 0x00000001,
    SPP_TX_SID_UPDATE                           = 0x00000002,
    SPP_TX_NO_DATA                              = 0x00000003
} SPP_TX_FRAME_TYPE_T;


// ============================================================================
// SPP_AFS_TYPE_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef enum
{
    SPP_AFS_SID_UPDATE                          = 0x00000000,
    SPP_AFS_SID_FIRST                           = 0x00000001,
    SPP_AFS_ONSET                               = 0x00000002,
    SPP_AFS_SPEECH                              = 0x00000003,
    SPP_AFS_RATSCCH                             = 0x00000004,
    SPP_AFS_NO_DATA                             = 0x00000005
} SPP_AFS_TYPE_T;


// ============================================================================
// SPP_AHS_TYPE_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef enum
{
    SPP_AHS_SID_UPDATE                          = 0x00000000,
    SPP_AHS_SID_UPDATE_INH                      = 0x00000001,
    SPP_AHS_SID_FIRST_P1                        = 0x00000002,
    SPP_AHS_SID_FIRST_P2                        = 0x00000003,
    SPP_AHS_SID_FIRST_INH                       = 0x00000004,
    SPP_AHS_ONSET                               = 0x00000005,
    SPP_AHS_SPEECH                              = 0x00000006,
    SPP_AHS_RATSCCH_MARKER                      = 0x00000007,
    SPP_AHS_RATSCCH_DATA                        = 0x00000008,
    SPP_AHS_NO_DATA                             = 0x00000009
} SPP_AHS_TYPE_T;


// ============================================================================
// SPP_FCCH_RETURN_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef enum
{
    FCCH_FOUND                                  = 0x00000000,
    FCCH_NOT_FOUND                              = 0x00000001,
    FCCH_INVALID                                = 0x00000002
} SPP_FCCH_RETURN_T;


// ============================================================================
// SPP_FCCH_MODE_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef enum
{
    FCCH_SLICED                                 = 0x00000000,
    FCCH_FULL                                   = 0x00000001
} SPP_FCCH_MODE_T;


// ============================================================================
// SPP_ITLV_TYPE_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef enum
{
    ITLV_TYPE_1A                                = 0x00000000,
    ITLV_TYPE_1B                                = 0x00000001,
    ITLV_TYPE_1C                                = 0x00000002,
    ITLV_TYPE_2A                                = 0x00000003,
    ITLV_TYPE_2B                                = 0x00000004,
    ITLV_TYPE_3                                 = 0x00000005,
    ITLV_TYPE_H1                                = 0x00000006,
    ITLV_TYPE_H2                                = 0x00000007,
    ITLV_TYPE_H3                                = 0x00000008,
    ITLV_TYPE_H4                                = 0x00000009,
    ITLV_TYPE_D1                                = 0x0000000A,
    ITLV_TYPE_D2                                = 0x0000000B,
    ITLV_TYPE_D3                                = 0x0000000C
} SPP_ITLV_TYPE_T;


// ============================================================================
// SPP_MODULATION_TYPE_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef enum
{
    SPP_UNKNOWN_MOD                             = 0x00000000,
    SPP_GMSK_MOD                                = 0x00000001,
    SPP_8PSK_MOD                                = 0x00000002
} SPP_MODULATION_TYPE_T;


// ============================================================================
// SPP_HEADER_TYPE_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef enum
{
    SPP_HEADER_TYPE1                            = 0x00000000,
    SPP_HEADER_TYPE2                            = 0x00000001,
    SPP_HEADER_TYPE3                            = 0x00000002,
    SPP_HEADER_UNKNOWN                          = 0x00000003
} SPP_HEADER_TYPE_T;


// ============================================================================
// SPP_DUMMY_PCH_RESULT_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef enum
{
    SPP_DUMMY_PCH_NOT_DETECTED                  = 0x00000000,
    SPP_DUMMY_PCH_A_DETECTED                    = 0x00000001,
    SPP_DUMMY_PCH_AB_DETECTED                   = 0x00000002,
    SPP_PCH_DECODING_COMPLETE                   = 0x00000003,
    SPP_DUMMY_PCH_NO_RESULT                     = 0x000000FF
} SPP_DUMMY_PCH_RESULT_T;


// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// SPP_FCCH_RESULT_T
// -----------------------------------------------------------------------------
/// FCCH Result
// =============================================================================
typedef struct
{
    INT16                          FOf;                          //0x00000000
    INT16                          TOf;                          //0x00000002
} SPP_FCCH_RESULT_T; //Size : 0x4



// ============================================================================
// SPP_FCCH_PARAM_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef struct
{
    INT32                          Acc_Energy;                   //0x00000000
    INT32                          Acc_Corr_I;                   //0x00000004
    INT32                          Acc_Corr_Q;                   //0x00000008
    UINT32                         Max_Abs_Corr;                 //0x0000000C
    INT32                          Corr_order_I;                 //0x00000010
    INT32                          Corr_order_Q;                 //0x00000014
    INT16                          samples_left;                 //0x00000018
    INT16                          max_index;                    //0x0000001A
    UINT8                          FCCH_detected;                //0x0000001C
    UINT8                          counter_high;                 //0x0000001D
    UINT8                          counter_low;                  //0x0000001E
    UINT8                          lastIT;                       //0x0000001F
    UINT8                          debug;                        //0x00000020
} SPP_FCCH_PARAM_T; //Size : 0x24



// ============================================================================
// SPP_COMPLEX_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef struct
{
    INT16                          i;                            //0x00000000
    INT16                          q;                            //0x00000002
} SPP_COMPLEX_T; //Size : 0x4



// ============================================================================
// SPP_SCH_RESULT_T
// -----------------------------------------------------------------------------
/// SCH Result
// =============================================================================
typedef struct
{
    UINT16                         Pwr;                          //0x00000000
    INT16                          Snr;                          //0x00000002
    INT16                          FOf;                          //0x00000004
    UINT8                          TOf;                          //0x00000006
    UINT8                          Data[4];                      //0x00000007
} SPP_SCH_RESULT_T; //Size : 0xC



// ============================================================================
// SPP_N_BURST_RESULT_T
// -----------------------------------------------------------------------------
/// Normal Burst Result
// =============================================================================
typedef struct
{
    UINT16                         Pwr;                          //0x00000000
    INT16                          Snr;                          //0x00000002
    INT16                          FOf;                          //0x00000004
    UINT8                          TOf;                          //0x00000006
    SPP_COMPLEX_T                  Dco;                          //0x00000008
} SPP_N_BURST_RESULT_T; //Size : 0xC



// ============================================================================
// SPP_N_BLOCK_RESULT_T
// -----------------------------------------------------------------------------
/// Normal block Result
// =============================================================================
typedef struct
{
    BOOL                           Bfi;                          //0x00000000
    UINT8                          Cs;                           //0x00000001
    UINT8                          Voc_Sid;                      //0x00000002
    UINT8                          MeanBEP;                      //0x00000003
    UINT8                          CvBEP;                        //0x00000004
    UINT16                         BitError;                     //0x00000006
    UINT16                         BitTotal;                     //0x00000008
} SPP_N_BLOCK_RESULT_T; //Size : 0xA



// ============================================================================
// SPP_HEADER_RESULT_T
// -----------------------------------------------------------------------------
/// Header structure for Egprs
// =============================================================================
typedef struct
{
    UINT16                         bsn[2];                       //0x00000000
    UINT8                          mcs;                          //0x00000004
    UINT8                          cps[2];                       //0x00000005
    UINT8                          spb;                          //0x00000007
    UINT8                          bfi;                          //0x00000008
} SPP_HEADER_RESULT_T; //Size : 0xA



// ============================================================================
// SPP_NB_VITAC_T
// -----------------------------------------------------------------------------
/// Viterbi NB Equalisation Buffers Structure SPAL_ISI is fixed, equal to 5
// =============================================================================
typedef struct
{
    /// 2068 bytes
    VOLATILE UINT32                PathMetric[2*16/2];           //0x00000000
    VOLATILE UINT32                path_diff_bkw[61*16/4];       //0x00000040
    VOLATILE UINT32                path_diff_fwd[61*16/4];       //0x00000410
    VOLATILE INT16                 Xcorr[2*(8+5+1)];             //0x000007E0
} SPP_NB_VITAC_T; //Size : 0x818



// ============================================================================
// SPP_SCH_VITAC_T
// -----------------------------------------------------------------------------
/// Viterbi SCH Equalisation Buffers Structure
// =============================================================================
typedef struct
{
    /// 1643 bytes
    VOLATILE UINT32                PathMetric[2*16/2];           //0x00000000
    VOLATILE UINT32                path_diff_bkw[42*16/4];       //0x00000040
    VOLATILE UINT32                path_diff_fwd[42*16/4];       //0x000002E0
    VOLATILE INT32                 Xcorr_I[24+5];                //0x00000580
    VOLATILE INT32                 Xcorr_Q[24+5];                //0x000005F4
} SPP_SCH_VITAC_T; //Size : 0x668



// ============================================================================
// SPP_DECODE_VITAC_T
// -----------------------------------------------------------------------------
/// Viterbi Decoding Buffers Structure
// =============================================================================
typedef struct
{
    /// 2144 bytes
    VOLATILE UINT32                RxItlvBufferOut[114];         //0x00000000
    VOLATILE UINT32                ViterbiOut[16];               //0x000001C8
    VOLATILE UINT32                SurvivorMEM[(64*171)/(16*2)]; //0x00000208
    VOLATILE UINT32                PathMetric[64];               //0x00000760
} SPP_DECODE_VITAC_T; //Size : 0x860



// ============================================================================
// SPP_EGPRS_PREFILTER_BUFS_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef struct
{
    SPP_COMPLEX_T                  InArray[157];                 //0x00000000
    SPP_COMPLEX_T                  RLS_W[8];                     //0x00000274
    SPP_COMPLEX_T                  PrefiltMin[10+8];             //0x00000294
    SPP_COMPLEX_T                  PrefiltMax[10+8];             //0x000002DC
    SPP_COMPLEX_T                  LpChan[8];                    //0x00000324
    SPP_COMPLEX_T                  A1[8];                        //0x00000344
    SPP_COMPLEX_T                  A2[10];                       //0x00000364
    SPP_COMPLEX_T                  P_Array[10+1];                //0x0000038C
    SPP_COMPLEX_T                  P_Array_New[10];              //0x000003B8
    SPP_COMPLEX_T                  H_CorArray[2*8-1];            //0x000003E0
    SPP_COMPLEX_T                  LocalTsc[16];                 //0x0000041C
    SPP_COMPLEX_T                  TscSym[26];                   //0x0000045C
    SPP_COMPLEX_T                  DcIqBufs[11];                 //0x000004C4
} SPP_EGPRS_PREFILTER_BUFS_T; //Size : 0x4F0



// ============================================================================
// SPP_EGPRS_DDFSE_BUFS_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef struct
{
    SPP_COMPLEX_T                  InArray[157];                 //0x00000000
    SPP_COMPLEX_T                  RLS_W[8];                     //0x00000274
    SPP_COMPLEX_T                  PrefiltMin[10+8];             //0x00000294
    SPP_COMPLEX_T                  PrefiltMax[10+8];             //0x000002DC
    SPP_COMPLEX_T                  LpChan[8];                    //0x00000324
    SPP_COMPLEX_T                  BmmlZfMetric[6/2 * 8 * 8 + 1]; //0x00000344
    UINT8                          SoftOutputFwd[8*61*4];        //0x00000648
    UINT8                          SoftOutputBck[8*61*4];        //0x00000DE8
    UINT8                          TscSym[26];                   //0x00001588
} SPP_EGPRS_DDFSE_BUFS_T; //Size : 0x15A2



// ============================================================================
// SPP_EGPRS_DECODE_VITAC_T
// -----------------------------------------------------------------------------
/// Viterbi Decoding Buffers Structure
// =============================================================================
typedef struct
{
    VOLATILE UINT32                CombinedData[459];            //0x00000000
    VOLATILE UINT32                SurvivorData[(64*612)/(16*2)]; //0x0000072C
    VOLATILE UINT32                PunctTable[58];               //0x00001A4C
    VOLATILE UINT32                PathMetric[64];               //0x00001B34
    VOLATILE UINT32                DecodedData[20];              //0x00001C34
} SPP_EGPRS_DECODE_VITAC_T; //Size : 0x1C84



// ============================================================================
// SPP_AMR_CFG_T
// -----------------------------------------------------------------------------
/// AMR Config
// =============================================================================
typedef struct
{
    UINT8                          activeCodecSet[4];            //0x00000000
    UINT8                          hysteresis[3];                //0x00000004
    UINT8                          threshold[3];                 //0x00000007
    UINT8                          version;                      //0x0000000A
    UINT8                          noiseSuppression;             //0x0000000B
    UINT8                          startModeIdx;                 //0x0000000C
    UINT8                          nbActiveCodecSet;             //0x0000000D
    UINT8                          phase;                        //0x0000000E
} SPP_AMR_CFG_T; //Size : 0xF






//-------------------
// SPP IR structures
//-------------------

typedef union
{
    struct
    {
        UINT32 bsn : 11;  // BSN number
        UINT32 mod : 1;   // Modulation (1 GMSK, 0 8PSK)
        UINT32 mcs : 4;   // MCS (+ SPB for GMSK only)
    };

    UINT16 ID;

} SPP_HEADER_CTX_T;

typedef union
{
    struct
    {
        UINT32 bsnresi  : 5;   // residuel BSN number
        UINT32 spb      : 2;   // SPB for GMSK only
        UINT32 next     : 9;   // index of next bock
    };

    UINT16 ID_H;

} SPP_EGPRS_IR_HASH;


typedef struct
{
    SPP_HEADER_CTX_T header[MCS_MAX_BUFFER_LEN];
    UINT32           receiveBufState[(MCS_MAX_BUFFER_LEN+31)/32];
    UINT8            cps[(MCS_MAX_BUFFER_LEN+3)/4];
    UINT8             ReceiveState[(SPP_EGPRS_IR_SNS+3)/4];
    UINT16             hashEntry[HASH_LEN];
    SPP_EGPRS_IR_HASH  tabHash[MCS_MAX_BUFFER_LEN];
    UINT16             MaxBufferLength;
    UINT16             BufLeft;
    UINT16             pw;
    UINT16             Vq;
    UINT16             Vr;
    UINT16             CptSearch;
    UINT8             OverFlow;
    UINT8            irOn;

} SPP_EGPRS_IR_CTX_T;


typedef union
{
    UINT8 mcs1To4[MCS_1_TO_4_BUFFER_LEN][MCS_1_TO_4_BLOCK_LEN];
    UINT8 mcs5To6[MCS_5_TO_6_BUFFER_LEN][MCS_5_TO_6_BLOCK_LEN];
    UINT8 mcs7To9[MCS_7_TO_9_BUFFER_LEN][MCS_7_TO_9_BLOCK_LEN];

} SPP_EGPRS_IR_BUF_T;


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------

// SPP PUBLIC STRUCTURES/ ENUMS
///@defgroup gsm_struct GSM Structures and enums
///@{

//---------------
// SPP structures
//---------------

//volatile UINT8 decoded_out[5];

// Overlap Vitac Buffer Mapping (sequential use only)
typedef union
{
       SPP_NB_VITAC_T               NbVitac;
       SPP_SCH_VITAC_T              SchVitac;
       SPP_DECODE_VITAC_T           DecodeVitac;
       SPP_EGPRS_PREFILTER_BUFS_T   EgprsPrefilterBufs;
       SPP_EGPRS_DDFSE_BUFS_T       EgprsDdfseBufs;
       SPP_EGPRS_DECODE_VITAC_T     EgprsDecodeVitac;
} SPP_UNCACHED_BUFFERS_T;
//----------
// SPP Enums
//----------

///@}

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

// SPP PUBLIC FUNCTIONS
///@defgroup gsm_rx_func Rx processing functions
///@{

//======================================================================
// FCCH Detection function
//----------------------------------------------------------------------
/// This function implements FCCH detection over large reception windows
/// (usually 9 Time Slots in @b sliced mode or 12 TDMA frames in @b Full
/// mode). For both modes, the reception window must be sliced in sub
/// parts. The function must be called for each sub part of the window
/// and will return an FCCH detection status each time. See FCCH
/// detection memory requirements.
/// @param RxBuffer UINT32*. Start address of the reception buffer.
/// @param Init BOOL. Takes following values:
/// - TRUE : first call for this FCCH window,
/// - FALSE: subsequent calls for a same FCCH window.
/// .
/// @param Mode SPP_FCCH_MODE_T. Takes following values:
/// - #FCCH_SLICED for detection during dedicated mode,
/// - #FCCH_FULL for initial cell syncronization process.
/// .
/// @param Res SPP_FCCH_RESULT_T*. Pointer to the FCCH Result structure.
///
/// @return Takes following values:
/// - #FCCH_FOUND if fcch has been detected (results valid),
/// - #FCCH_NOT_FOUND if no fcch has been detected,
/// - #FCCH_INVALID if fcch has been detected but conditions
/// of detection (window boundaries, suspect frequency error
/// ...) are imposing to discard the detection results.
/// .
//======================================================================
PUBLIC SPP_FCCH_RETURN_T spp_SearchFcch(UINT32* RxBuffer,
    BOOL Init,
    SPP_FCCH_MODE_T Mode,
    SPP_FCCH_RESULT_T* Res);

PUBLIC BOOL spp_FcchDetect(UINT32* Ptr_old,
                      UINT32* Ptr_new,
                      SPP_FCCH_PARAM_T* Params,
                      SPP_FCCH_RESULT_T* Res);

PUBLIC BOOL spp_FcchDetectAsm(UINT32* Ptr_old,
                      UINT32* Ptr_new,
                      SPP_FCCH_PARAM_T* Params,
                      SPP_FCCH_RESULT_T* Res);

PUBLIC BOOL spp_FcchDetectAsm_gallite(UINT32* Ptr_old,
                      UINT32* Ptr_new,
                      SPP_FCCH_PARAM_T* Params,
                      SPP_FCCH_RESULT_T* Res);

//======================================================================
// SCH Equalize and Decode function
//----------------------------------------------------------------------
/// Performs SCH burst equalization including:
/// - DC offset estimation and correction,
/// - channel estimation,
/// - SNR estimation,
/// - Viterbi equalization and traceback,
/// - Frequency Offset estimation.
/// .
/// @param RxBuffer : Pointer to the beginning of the RF samples buffer
/// @param UncachedBuffers : uncached working memory space
/// @param Res : Pointer to the SCH Result structure
//======================================================================
PUBLIC BOOL spp_EqualizeDecodeSch(UINT32* RxBuffer,
    SPP_UNCACHED_BUFFERS_T* UncachedBuffers,
    SPP_SCH_RESULT_T* Res);

//======================================================================
// spp_EqualizeNBurst
//----------------------------------------------------------------------
/// Performs normal burst equalization including:
/// - DC offset estimation and correction,
/// - channel estimation,
/// - SNR estimation,
/// - Viterbi equalization and traceback,
/// - Frequency Offset estimation.
/// .
///
/// @param RxBuffer (UINT32*): Pointer to the beginning of the RF samples buffer
/// @param Tsc (UINT8): Training sequence number
/// @param EqBufferOut (UINT32*): Pointer to the equalized output
/// @param UncachedBuffers : uncached working memory space
/// @param Res (SPP_N_BURST_RESULT_T*): Pointer to the Normal Burst Result structure
//======================================================================
PUBLIC VOID spp_EqualizeNBurst(UINT32* RxBuffer,
    UINT8 Tsc,
    UINT32* EqBufferOut,
    SPP_UNCACHED_BUFFERS_T* UncachedBuffers,
    SPP_N_BURST_RESULT_T* Res);

//======================================================================
// spp_DecodeStealingBits
//----------------------------------------------------------------------
/// Decode Stealing Flags of Normal Bursts.
///
/// @param RxBuffer UINT32*. Pointer to the beginning of the RF samples buffer.
/// @param StealMode BOOL. Takes following values:
/// - TRUE for PDCH Steal flags type,
/// - FALSE for TCH Steal flags type.
/// .
/// @param StealingResult UINT8*. Output pointer.
/// @param Threshold INT16. Decision threshold (for TCH only).
//======================================================================
PUBLIC VOID spp_DecodeStealingBits(UINT32* RxBuffer,
    BOOL StealMode,
    UINT8* StealingResult,
    INT16 Threshold);

//======================================================================
// spp_DeinterleaveBlock
//----------------------------------------------------------------------
/// DeInterleaving of a data block
///
/// @param BufferIn UINT32*. INPUT. Pointer to equalized bursts buffer.
/// @param InterleaverType UINT8. INPUT. InterleaverType may take the following values:
/// - 0 : TYPE_1A. Valid for TCH/FS, TCH/EFS, FACCH/F, TCH/F2.4,
/// TCH/AFS (speech, ratscch, sid_first).
/// - 1 : TYPE_1B. Valid for SACCH, SDCCH, TCH/AFS(sid_update), PDTCH(CS-1 to CS-4),
/// BCCH, PCH, AGCH, PACCH, PBCCH, PAGCH, PPCH, PNCH, PTCCH/D.
/// - 2 : TYPE_1C. Valid for FACCH/H.
/// - 3 : TYPE_2A. Valid for TCH/HS and TCH/AHS (speech, ratscch, sid_first).
/// - 4 : TYPE_2B. Valid for TCH/AHS (sid_update).
/// - 5 : TYPE_3. Valid for TCH/F14.4, TCH/F9.6, TCH/F4.8, TCH/H4.8
/// and TCH/H2.4.
/// - 6 : TYPE_H1. Valid for EGPRS HEADER TYPE2 in Rx.
/// - 7 : TYPE_H2. Valid for EGPRS HEADER TYPE2 in Tx.
/// - 8 : TYPE_H3. Valid for EGPRS HEADER TYPE1 in Rx.
/// - 9 : TYPE_H4. Valid for EGPRS HEADER TYPE1 in Tx.
/// - 10 : TYPE_D1. Valid for EGPRS MCS5/6.
/// - 11 : TYPE_D2. Valid for EGPRS MCS7.
/// - 12 : TYPE_D3. Valid for EGPRS MCS8/9.
/// @param BurstOffset UINT8. INPUT. Offset position for deinterleaving (see ITLV
/// documentation).
/// @param BufferOut UINT32*. OUTPUT. Output pointer.
//======================================================================
PUBLIC VOID spp_DeinterleaveBlock(UINT32* BufferIn,
    UINT8 InterleaverType,
    UINT8 BurstOffset,
    UINT32* BufferOut);

//======================================================================
// spp_DecodeBlock
//----------------------------------------------------------------------
/// Decoding of a data block
///
/// @param BufferIn UINT32*. Pointer to the deinterleaved data frame.
/// @param ChMode UINT16. Channel Type (PAL API format).
/// @param AmrFrameType UINT8. Type of AMR frame (set to 0 excepted for AMR).
/// @param AmrMode UINT8. Codec Mode (set to 0 excepted for AMR).
/// @param BufferOut UINT32*. Output pointer.
/// @param Usf UINT8. Uplink State Flag.
/// @param UncachedBuffers : uncached working memory space
/// @param Res SPP_N_BLOCK_RESULT_T*. Pointer to the Block Result structure.
//======================================================================
PUBLIC VOID spp_DecodeBlock(UINT32* BufferIn,
    UINT16 ChMode,
    UINT8 AmrFrameType,
    UINT8 AmrMode,
    UINT32* BufferOut,
    UINT8 Usf,
    SPP_UNCACHED_BUFFERS_T* UncachedBuffers,
    SPP_N_BLOCK_RESULT_T* Res);

//======================================================================
// spp_GetUSF
//----------------------------------------------------------------------
/// Uplink State Flag extraction
///
/// @param BufferIn UINT32*. Pointer to the beginning of the RF samples buffer.
/// @param CS UINT8. Coding scheme.
///
/// @return UINT8. Uplink State Flag
//======================================================================
PUBLIC UINT8 spp_GetUSF(UINT32* BufferIn,
    UINT8 CS);

//======================================================================
// spp_DetectDummyPCH
//----------------------------------------------------------------------
/// Dummy PCH detection
///
/// @param BufferIn UINT32* Pointer to the beginning of the softbits.
/// @param Idx      UINT8   Index of the received burst
/// @param Thresh_a UINT8   Threshold for vector A detection
/// @param Thresh_b UINT8   Threshold for vector B detection
//
/// @return         UINT8   0xFF: false, 0x1: only A, 0x2: AB detected
//======================================================================
UINT8 spp_DetectDummyPCH(UINT32 *Buffer,
                         UINT8 Idx,
                         UINT8 Thresh_a,
                         UINT8 Thresh_b);

//======================================================================
// spp_GetMonPower
//----------------------------------------------------------------------
/// Calculate Power for Monitoring (and Interference) windows.
///
/// @param MonIdx UINT8. Index of the monitoring (or interference) window.
/// @param RxBuffer UINT32*. Pointer to the Rx buffer.
/// @param power UINT32*. Pointer to the resulting calculated power.
//======================================================================
PUBLIC VOID spp_GetMonPower(UINT8 MonIdx,
    UINT32* RxBuffer,
    UINT32* power);

//======================================================================
// Decoding of AMR Inband Data (DTX and RATSCCH frames)
//----------------------------------------------------------------------
/// This function decodes inband data for AFS or AHS
/// deinterleaved softbit frames (SPEECH, SIDUPDATE, RATSCCH)
///
/// @param softBit : Pointer to the decoded data
/// @param mode : type of frame: 0 -&amp;gt; AFS, 1 -&amp;gt; AHS, 2 -&amp;gt; SIDUPDATE/RATSCCH
///
/// @return UINT8. Inband Data.
//======================================================================
PUBLIC UINT8 spp_GetAmrIc(UINT8 *softBit,
    UINT8 mode);

//======================================================================
// Search AMR frame markers
//----------------------------------------------------------------------
/// This function searches AMR frame markers and returns the most
/// likely frame type. It decode also the associated in band data.
///
/// @param BufferIn UINT32*. Pointer to the deinterleaved decoded data.
/// @param id1 UINT8*. Pointer to ther first decoded in band data.
/// @param id2 UINT8*. Pointer to the second decoded in band data.
/// @param mode UINT8. TCH rate: 0 -&amp;gt; AFS, 1 -&amp;gt; AHS.
///
/// @return UINT8. Most likely frame type.
//======================================================================
PUBLIC UINT8 spp_SearchAmrMarker(UINT32 *BufferIn,
    UINT8 *id1,
    UINT8 *id2,
    UINT8 mode);

//======================================================================
// spp_EgprsBlindDetection
//----------------------------------------------------------------------
/// Performs modulation blind detection and TOf estimation:
///.
/// @param RxBuffer (UINT32*): Pointer to the beginning of the RF samples buffer
/// @param burstIdx (UINT8): burst index number
/// @param tsc UINT8 : training sequence Id
/// @param modulation (SPP_MODULATION_TYPE_T*): Pointer to modulation result
/// @param TOf_in_Qb (UINT8*): Pointer to timing offset result in quarter of bit
/// @param TOf (UINT8*): Pointer to timing offset result
//======================================================================
PUBLIC VOID spp_EgprsBlindDetection(UINT32* RxBuffer,
    UINT8 burstIdx,
    UINT8 tsc,
    SPP_MODULATION_TYPE_T* modulation,
    UINT8* TOf_in_Qb,
    UINT8* TOf);

//======================================================================
// spp_EgprsEqualizeNBurst
//----------------------------------------------------------------------
/// Performs normal burst equalization including:
/// - DC offset estimation and correction,
/// - IQ imbalance estimation and correction,
/// - channel estimation,
/// - SNR estimation,
/// - Viterbi equalization and traceback,
/// - Frequency Offset estimation.
/// .
///
/// @param RxBuffer (UINT32*): Pointer to the beginning of the RF samples buffer
/// @param Tsc (UINT8): Training sequence number
/// @param EqBufferOut (UINT32*): Pointer to the equalized output
/// @param UncachedBuffers : uncached working memory space
/// @param Res (SPP_N_BURST_RESULT_T*): Pointer to the Normal Burst Result structure
//======================================================================
PUBLIC VOID spp_EgprsEqualizeNBurst(UINT32* RxBuffer,
    UINT8 Tsc,
    UINT32* EqBufferOut,
    SPP_UNCACHED_BUFFERS_T* UncachedBuffers,
    SPP_N_BURST_RESULT_T* Res);

//======================================================================
// spp_EgprsBurstDemapping
//----------------------------------------------------------------------
/// Performs burst demapping for EDGE:
/// .
/// @param BufferIn (UINT32*): Pointer to the sofbit buffer
/// @param headertype (SPP_HEADER_TYPE_T): haeder type of the block
/// @param BufferOut (UINT32*): Pointer to the block's softbits (MCS1-4)
/// @param HeaderBufferOut (UINT32*): Pointer to the Header sofbits (MCS5-9)
/// @param DataBufferOut (UINT32*): Pointer to the Data sofbits (MCS5-9)
//======================================================================
PUBLIC VOID spp_EgprsBurstDemapping(UINT32* BufferIn,
    UINT32* HeaderBufferOut,
    UINT32* DataBufferOut,
    SPP_HEADER_TYPE_T headerType);

//======================================================================
// spp_EgprsDecodeStealingBits
//----------------------------------------------------------------------
/// Decode Stealing Flags of Normal Bursts.
///
/// @param RxBuffer UINT32*. Pointer to the beginning of the RF samples buffer.
/// @param headertype SPP_HEADER_TYPE_T*. Output pointer.
//======================================================================
PUBLIC VOID spp_EgprsDecodeStealingBits(UINT32* RxBuffer,
    SPP_HEADER_TYPE_T* headertype);

//======================================================================
// spp_EgprsDecodeHeader
//----------------------------------------------------------------------
/// Decoding of a data block
///
/// @param BufferIn UINT8*. Pointer to the deinterleaved data frame.
/// @param header_type SPP_HEADER_TYPE_T. Channel Type (PAL API format).
/// @param Usf UINT8 3 decoded usf bit .
/// @param BufferOut UINT8*. Output pointer.
/// @param UncachedBuffers : uncached working memory space
/// @param Res SPP_HEADER_RESULT_T*. Pointer to the Header Result structure.
//======================================================================
PUBLIC VOID spp_EgprsDecodeHeader(UINT8 *BufferIn,
                                    SPP_HEADER_TYPE_T HeadType,
                                    UINT8 Usf,
                                    UINT8 *BufferOut,
                                    SPP_UNCACHED_BUFFERS_T* UncachedBuffers,
                                    SPP_HEADER_RESULT_T* Res);

//======================================================================
// spp_EgprsDecodeData
//----------------------------------------------------------------------
/// Decoding of a data block
///
/// @param BufferIn UINT8*. Pointer to the deinterleaved data frame.
/// @param Header SPP_HEADER_RESULT_T. decoded Header structure.
/// @param BufferOut0 UINT8*. Output pointer for the first RLC data block.
/// @param BufferOut1 UINT8*. Output pointer for the second RLC data block (MCS 7,8,9 only).
/// @param irCtx : uncached IR context memory space
/// @param irBuf : uncached IR buffer memory space
/// @param UncachedBuffers : uncached working memory space
/// @param Res SPP_N_BLOCK_RESULT_T*. Pointer to the Block Result structure.
//======================================================================
PUBLIC VOID spp_EgprsDecodeData(UINT8                       *BufferIn,
                                SPP_HEADER_RESULT_T             Header,
                                UINT8                        *BufferOut0,
                                UINT8                        *BufferOut1,
                                SPP_EGPRS_IR_CTX_T            *irCtx,
                                SPP_EGPRS_IR_BUF_T            *irBuf,
                                SPP_UNCACHED_BUFFERS_T        *UncachedBuffers,
                                SPP_N_BLOCK_RESULT_T        *Res);

//======================================================================
// spp_EgprsGetUSF
//----------------------------------------------------------------------
/// Uplink State Flag extraction
///
/// @param BufferIn UINT32*. Pointer to the beginning of the RF samples buffer.
/// @return UINT8. Uplink State Flag
//======================================================================
PUBLIC UINT8 spp_EgprsGetUSF(UINT32* BufferIn);

//======================================================================
// spp_EgprsDeinterleaveBlock
//----------------------------------------------------------------------
/// DeInterleaving of a data block
///
/// @param BufferIn UINT32*. INPUT. Pointer to equalized bursts buffer.
/// @param InterleaverType UINT8. INPUT. InterleaverType may take the following values:
/// - 6 : TYPE_H1. Valid for EGPRS HEADER TYPE2 in Rx.
/// - 7 : TYPE_H2. Valid for EGPRS HEADER TYPE2 in Tx.
/// - 8 : TYPE_H3. Valid for EGPRS HEADER TYPE1 in Rx.
/// - 9 : TYPE_H4. Valid for EGPRS HEADER TYPE1 in Tx.
/// - 10 : TYPE_D1. Valid for EGPRS MCS5/6.
/// - 11 : TYPE_D2. Valid for EGPRS MCS7.
/// - 12 : TYPE_D3. Valid for EGPRS MCS8/9.
/// @param BufferOut UINT32*. OUTPUT. Output pointer.
//======================================================================
PUBLIC VOID spp_EgprsDeinterleaveBlock(UINT32* BufferIn,
    SPP_ITLV_TYPE_T InterleaverType,
    UINT32* BufferOut);



//=============================================================================
//  spp_EgprsIrInitCtx
//-----------------------------------------------------------------------------
/// for IR initial.
///
/// @param IR SPP_EGPRS_IR_CTX_T*. Incremental Redundancy related structure.
/// @param ChSubMode GSM_CS_T. Channel mode.
///
/// @author zhangchangchun
/// @date 2007/09/18
//=============================================================================
PUBLIC VOID spp_EgprsIrInitCtx(SPP_EGPRS_IR_CTX_T *IR, GSM_CS_T ChSubMode);



///@}

///@defgroup gsm_tx_func Tx processing functions
///@{
//-----------------------
// Transmission Functions
//-----------------------
//======================================================================
// spp_EncodeABurst
//----------------------------------------------------------------------
/// Access Burst encoding
///
/// @param BufferIn UINT32*. Pointer to the data to be encoded.
/// @param Bsic UINT8. Base Station BSIC.
/// @param size UINT8. 8 or 11 bits Access Burst.
/// @param BufferOut UINT32*. Encoded data output pointer.
//======================================================================
PUBLIC VOID spp_EncodeABurst(UINT32* BufferIn,
    UINT8 Bsic,
    UINT8 size,
    UINT32* BufferOut);

//======================================================================
// spp_EncodeBlock
//----------------------------------------------------------------------
/// Normal BLOCK encoding
///
/// @param InTxBuffer UINT32*. Pointer to the data to be encoded.
/// @param ChMode UINT16. Channel Type (PAL API format).
/// @param AmrFrameType UINT8. Type of AMR frame (set to 0 excepted for AMR).
/// @param AmrMode UINT8. Codec Mode (set to 0 excepted for AMR).
/// @param AmrInBandData UINT8*. AMR in Band Data (set to NULL excepted for AMR).
/// @param BurstOffset UINT8. Offset position for interleaving. Interleaving
/// is performed over a number of Tx bursts. The Offset position corresponds to
/// the offset number of bursts in the interleaving buffer. For example, in
/// FR mode, BurstOffset can be 0 or 4 as one coded block is interleaved over
/// 8 consecutive bursts, and each coded block has 4 bursts worth of payload.
/// (see ITLV documentation).
/// @param InterleaverType UINT8. INPUT. InterleaverType may take the following values:
/// - 0 : TYPE_1A. Valid for TCH/FS, TCH/EFS, FACCH/F, TCH/F2.4,
/// TCH/AFS (speech, ratscch, sid_first).
/// - 1 : TYPE_1B. Valid for SACCH, SDCCH, TCH/AFS(sid_update), PDTCH(CS-1 to CS-4),
/// BCCH, PCH, AGCH, PACCH, PBCCH, PAGCH, PPCH, PNCH, PTCCH/D.
/// - 2 : TYPE_1C. Valid for FACCH/H.
/// - 3 : TYPE_2A. Valid for TCH/HS and TCH/AHS (speech, ratscch, sid_first).
/// - 4 : TYPE_2B. Valid for TCH/AHS (sid_update).
/// - 5 : TYPE_3. Valid for TCH/F14.4, TCH/F9.6, TCH/F4.8, TCH/H4.8
/// and TCH/H2.4.
/// .
/// @param BufferOut UINT32*. Encoded data output pointer. The function
/// stores the output (the interleaved and encoded bits) in the Tx interleaver buffer.
/// BufferOut should point to the beginning of the Tx interleaver buffer.
//======================================================================
PUBLIC VOID spp_EncodeBlock(UINT32* InTxBuffer,
    UINT16 ChMode,
    UINT8 AmrFrameType,
    UINT8 AmrMode,
    UINT8* AmrInBandData,
    UINT8 BurstOffset,
    UINT8 InterleaverType,
    UINT32* BufferOut);

//======================================================================
// spp_BuildNBurst
//----------------------------------------------------------------------
/// Normal Burst building and mapping
///
/// @param TxBuffer UINT32*. Pointer to the encoded data.
/// @param Tsc UINT8. Training sequence number.
/// @param StealFlag UINT8. Stealing Flags.
/// @param BufferOut UINT32*. Burst output pointer.
//======================================================================
PUBLIC VOID spp_BuildNBurst(UINT32* TxBuffer,
    UINT8 Tsc,
    UINT8 StealFlag,
    UINT32* BufferOut);

//======================================================================
// spp_Modulation
//----------------------------------------------------------------------
/// This function handles the modulation of the (multiple) burst(s)
/// constituting the full Tx window.
/// The following is true for Granite TC and Greenstone :
/// resulting modulated structure :
/// - Preamble
/// - 1st burst (156 bits)
/// - eventual 2nd burst (156 bits)
/// - eventual 3nd burst (156 bits)
/// - eventual 4nd burst (156 bits)
/// - Postamble (enough for 32 bits)
///
/// Preamble are filled with '1'
/// Postamble are filled with '1'
/// Eventual "Holes" in burst Bitmap are filled with '1' because
/// multislot transmission is continuous from GMSK point of view
///
/// For Emerald:
/// depending on modulation one symbol is either 1 or 3 bits.
/// Modulation can change for each slot. 'Holes' are not sent.
/// resulting modulated structure if burst N is the first active
/// burst or if burst N-1 is inactive:
///
/// - bstN definition for RF_IF
/// - Preamble symb                 |
/// - bstN active part (148 symb)   |
///                                 | in burst N modulation
/// then if bstN+1 is active:       |
/// - (8-guardStart) symb           |
/// - bstN+1 definition for RF_IF
/// - guardStart symb               |
/// - bstN+1 active part (148 symb) | in burst N+1 modulation
/// - ... and so on until inactive burst
///
/// else if bstN+1 is inactive:
/// - Postamble symb                | in burst N modulation
/// - End of transmit for RF_IF
///
/// @param BufferIn pointer to the mapped burst(s) data
/// @param Bitmap Tx window bitmap allocation (first active bit is
/// considered as 1st burst)
/// @param preamble Burst Mapping parameters
/// @param postamble Burst Mapping parameters
/// @param guardStart Burst Mapping parameters
/// @param BufferOut Pointer to the output sw fifo (if relevant
//         for HW implementation)
/// @param BstMod Type of the modulation, 8PSK or GMSK.
/// @param NormalBst 0:Access Burst 1:Normal Burst
/// @todo  change modulation field to a bitmap to handle multislot.
/// @todo  do a structure for Burst Mapping parameters
/// @todo  include the burst type (AB/NB) in the modulation bitmap...
//======================================================================

PUBLIC VOID spp_Modulation(UINT32* BufferIn,
    UINT8 Bitmap,
                            UINT16 preamble,
                            UINT16 postamble,
                            UINT16 guardStart,
    UINT32* BufferOut,
                            SPP_MODULATION_TYPE_T BstMod,
                            BOOL NormalBst);

//======================================================================
// spp_EgprsEncodeBlock
//----------------------------------------------------------------------
/// Normal BLOCK encoding
///
/// @param InData0 UINT32*. Pointer to the 1st Tx buffer (for all MCS) to be encoded.
/// @param InData0 UINT32*. Pointer to the 2nd Tx buffer (for MCS > MCS6) to be encoded.
/// @param InHeader UINT32*. Pointer to the header buffer (for all MCS).
/// @param  header_type SPP_HEADER_TYPE_T. type of the header.
/// @param BufferOut UINT32*. Encoded data output pointer. The function
/// stores the output (the interleaved and encoded bits) in the Tx interleaver buffer.
/// BufferOut should point to the beginning of the Tx interleaver buffer.
//======================================================================
PUBLIC VOID spp_EgprsEncodeBlock(UINT32* InData0,
                                 UINT32* InData1,
                                 UINT32* InHeader,
                                 SPP_HEADER_TYPE_T header_type,
                                 UINT32* BufferOut);

//======================================================================
// spp_EgprsBuildNBurst
//----------------------------------------------------------------------
/// Normal Burst building and mapping
///
/// @param TxBuffer UINT32*. Pointer to the encoded data.
/// @param Tsc UINT8. Training sequence number.
/// @header_type SPP_HEADER_TYPE_T. type of the header.
/// @param BufferOut UINT32*. Burst output pointer.
//======================================================================
PUBLIC VOID spp_EgprsBuildNBurst(UINT32* TxBuffer,
    UINT8 Tsc,
    SPP_HEADER_TYPE_T headerType,
    UINT32* BufferOut);

//-----------------------
// Ciphering Functions
//-----------------------
///@defgroup gsm_ciphering_func GSM/GPRS Miscellanous Functions
///@{

//======================================================================
// spp_CipherCipherNB
//----------------------------------------------------------------------
/// This function ciphers 114 bits for normal burst.
///
/// @param BufferIn UINT32*. Pointer to input buffer.
/// @param BufferOut UINT32*. Pointer to output buffer.
///
//======================================================================
PUBLIC VOID spp_CipherCipherNb(UINT32* BufferIn, UINT32* BufferOut);

//======================================================================
// spp_CipherCipherNB
//----------------------------------------------------------------------
/// This function deciphers 114 bits for normal burst.
///
/// @param BufferIn UINT32*. Pointer to input buffer.
/// @param BufferOut UINT32*. Pointer to output buffer.
///
//======================================================================
PUBLIC VOID spp_CipherDecipherNb(UINT32* BufferIn, UINT32* BufferOut);

//======================================================================
// Initialize the count register
//----------------------------------------------------------------------
/// This function initializes the count register.
///
/// @param count UINT32. Counter register.
//======================================================================
PUBLIC UINT8 spp_CipherCountInit(UINT32 count);

//======================================================================
// spp_CipherStart
//----------------------------------------------------------------------
/// Start the encipherment.
///
/// @param algo UINT8.
///
/// @return UINT8. Error or no error???
//======================================================================
PUBLIC UINT8 spp_CipherStart(UINT8 algo);

/// @}


//-----------------------
// Miscellanous Functions
//-----------------------
///@defgroup gsm_misc_func GSM/GPRS Miscellanous Functions
///@{

// =============================================================================
// spp_FullBurstReady
// -----------------------------------------------------------------------------
/// This function MUST be in RAM and not in ROM !
/// This function returns the number of completely received (and not treated)
/// bursts in the active frame.
/// @return
// =============================================================================
PUBLIC UINT8 spp_FullBurstReady(VOID);

// =============================================================================
// spp_ResetFullBurstReady
// -----------------------------------------------------------------------------
/// This function MUST be in RAM and not in ROM !
/// This function resets the number of completely received bursts in the active
/// frame.
// =============================================================================
PUBLIC VOID spp_ResetFullBurstReady(VOID);

// =============================================================================
// spp_OneFullBurstReady
// -----------------------------------------------------------------------------
/// This function MUST be in RAM and not in ROM !
/// This function flags that a burst has been completely received in the active
/// frame.
// =============================================================================
PUBLIC VOID spp_OneFullBurstReady(VOID);

// =============================================================================
// spp_OneFullBurstDone
// -----------------------------------------------------------------------------
/// This function MUST be in RAM and not in ROM !
/// This function flags that a burst has been fully processed in the active
/// frame.
// =============================================================================
PUBLIC VOID spp_OneFullBurstDone(VOID);

/// @}




#endif

