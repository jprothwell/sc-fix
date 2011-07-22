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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/bcpu/include/spc_mailbox_v0.h $                                                                   
//  $Author: admin $                                                                    
//  $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//  $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
/// @file spc_mailbox.h
///  This File contains SPC API
///  
///  @mainpage Modem2G Signal Processing Control API
///  @page spc_mainpage Modem2G Signal Processing Control API
///  
///  Modem2G Signal Processing Control (SPC) API description.
///  @author Coolsand Technologies, Inc.
///  @version 1.0
///  @date march 2007
///  
///  @par Purpose
///  
///  This document describes the Modem2G software interface between SPC &amp; PAL.
///  This interface is realized by means of different structures
///  contained in a part of the memory that we call mailbox.
///  Each involved layer, i.e. SPC and PAL have a dedicated structure to
///  transfer information to the other part.
///  
///  @par
///  
///  SPC API is composed of the following division :
///  - @ref spc2pal "SPC to PAL structures " \n
///  - @ref pal2spc "PAL to SPC structures " \n
///  - @ref mbox "main mailbox structure" \n
///  
///  This mailbox allows the XCPU to progam different processings on BCPU
///  and to retrieve associated results.
///  
///  @defgroup mbox 
///  @{
//
//==============================================================================

#ifndef _SPC_MAILBOX_V0_H_
#define _SPC_MAILBOX_V0_H_


#include "cs_types.h"

// =============================================================================
//  MACROS
// =============================================================================
/// FCCH search related constants: do not change unless you know what you're doing
/// ! Granularity of FCCH search in symbols.
#define BB_FCCH_SIZE                             (142)
#define BB_FCCH_NB_WIN                           (((625*9)/(BB_FCCH_SIZE*4) + 1))
#define BB_FCCH_PRE_OPENING                      ((BB_FCCH_NB_WIN*BB_FCCH_SIZE - ((625*9)/4)))
/// number of context duplicates
#define MBOX_CTX_SNAP_QTY                        (3)
/// max number of windows in a burts
#define MBOX_WINDOWS_QTY                         (3)
/// max number of windows index
#define MBOX_WIN_IDX_QTY                         (MBOX_WINDOWS_QTY)
#define MBOX_MON_WIN_SIZE                        (72)
/// max number of monitoring windows
#define MBOX_MONIT_WIN_QTY                       (MBOX_WINDOWS_QTY)
/// max number of slots for an interference measurement window
#define MBOX_INTRF_SLOT_QTY                      (6)
/// max number of synchonization burst window
#define MBOX_SCH_QTY                             (1)
/// number of data byte in a decoded SCH burst
#define MBOX_SCH_DATA_BYTE_QTY                   (4)
/// number of data output address
#define MBOX_NBURST_DATA_QTY                     (2)
/// max number of slots in a window
#define MBOX_MAX_TS_QTY                          (4)
/// max number of buffer index
#define MBOX_MAX_BUF_IDX_QTY                     (5)
/// max number of words in a compressed speech frame
#define MBOX_MAX_SPEECH_FRAME_SIZE               (18)
/// Channel Mode comming from stack
#define MBOX_NO_TCH                              (0X00)
#define MBOX_TCH_FR                              (0X04)
#define MBOX_TCH_EFR                             (0X14)
#define MBOX_TCH_HR                              (0X05)
#define MBOX_TCH_AFS                             (0X24)
#define MBOX_TCH_AHS                             (0X25)
#define MBOX_TCH_F48                             (0X48)
#define MBOX_TCH_F96                             (0X88)
#define MBOX_TCH_F144                            (0XC8)
/// BCPU starting handshake
#define MBOX_XCPU_HELLO_WORD                     (0XBB08E770)
#define MBOX_BCPU_ACKNOWLEDGE_WORD               (0XBB04EAD1)
#define MBOX_BCPU_ERROR_WORD                     (0XBB0E4404)
/// Ciphering algorithm
#define MBOX_CIPH_MODE_A51                       (0)
#define MBOX_CIPH_MODE_A52                       (1)
/// Loop modes see 3GPP 44.014 for more details
#define MBOX_LOOP_CLOSE_A                        (1)
#define MBOX_LOOP_CLOSE_B                        (2)
#define MBOX_LOOP_CLOSE_C                        (3)
#define MBOX_LOOP_CLOSE_D                        (4)
#define MBOX_LOOP_CLOSE_E                        (5)
#define MBOX_LOOP_CLOSE_F                        (6)
#define MBOX_LOOP_SPCH_DECOD                     (7)
#define MBOX_LOOP_SPCH_ENCOD                     (8)
#define MBOX_LOOP_ACOUSTIC_TEST                  (9)
#define MBOX_LOOP_OPEN                           (10)
#define MBOX_LOOP_CLOSE_I                        (11)

// ============================================================================
// SPC_MBOX_DSP_ITR_T
// -----------------------------------------------------------------------------
/// BCPU to XCPU interrupts
// =============================================================================
typedef enum
{
/// no interrupt
    MBOX_DSP_NO_ITR                             = 0x00000000,
/// synchro burst decoding done
    MBOX_DSP_SCH_DECODING_COMPLETE              = 0x00000001,
/// dummy PCH detection done
    MBOX_DSP_PCH_READY                          = 0x00000002,
/// USF decoding done for GPRS
    MBOX_DSP_USF_READY                          = 0x00000004,
/// normal burst equalization done
    MBOX_DSP_EQUALIZATION_COMPLETE              = 0x00000008,
/// dsp task complete
    MBOX_DSP_TASK_COMPLETE                      = 0x00000010
} SPC_MBOX_DSP_ITR_T;


// ============================================================================
// SPC_MBOX_XCPU_ITR_T
// -----------------------------------------------------------------------------
/// XCPU to BCPU interrupts in the mailbox regs are from bits 8 to 15
// =============================================================================
typedef enum
{
/// no interrupt from XCPU
    MBOX_XCPU_NO_ITR                            = 0x00000000,
/// audio interrupt from XCPU
    MBOX_XCPU_AUDIO                             = 0x00000100
} SPC_MBOX_XCPU_ITR_T;


// ============================================================================
// SPC_MBOX_CHAN_TYPES_T
// -----------------------------------------------------------------------------
/// GSM/GPRS logical channels types
// =============================================================================
typedef enum
{
/// control channel
    MBOX_CCH_CHN                                = 0x00000000,
/// packet data channel
    MBOX_PDCH_CHN                               = 0x00000001,
/// traffic channel full rate
    MBOX_TCHF_CHN                               = 0x00000002,
/// traffic channel half rate
    MBOX_TCHH_CHN                               = 0x00000003,
/// fast associated control channel
    MBOX_FACCH_CHN                              = 0x00000004,
/// circuit switched data channel
    MBOX_CDATA_CHN                              = 0x00000005,
/// random access channel (not defined in palcgsm.h)
    MBOX_CHAN_TYPE_RACH                         = 0x00000006
} SPC_MBOX_CHAN_TYPES_T;


// ============================================================================
// MBOX_SPEECH_MODE_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef enum
{
    MBOX_AMR475_DEC_MODE                        = 0x000000F0,
    MBOX_AMR475_ENC_MODE                        = 0x0000000F,
    MBOX_AMR515_DEC_MODE                        = 0x000000F1,
    MBOX_AMR515_ENC_MODE                        = 0x0000001F,
    MBOX_AMR59_DEC_MODE                         = 0x000000F2,
    MBOX_AMR59_ENC_MODE                         = 0x0000002F,
    MBOX_AMR67_DEC_MODE                         = 0x000000F3,
    MBOX_AMR67_ENC_MODE                         = 0x0000003F,
    MBOX_AMR74_DEC_MODE                         = 0x000000F4,
    MBOX_AMR74_ENC_MODE                         = 0x0000004F,
    MBOX_AMR795_DEC_MODE                        = 0x000000F5,
    MBOX_AMR795_ENC_MODE                        = 0x0000005F,
    MBOX_AMR102_DEC_MODE                        = 0x000000F6,
    MBOX_AMR102_ENC_MODE                        = 0x0000006F,
    MBOX_AMR122_DEC_MODE                        = 0x000000F7,
    MBOX_AMR122_ENC_MODE                        = 0x0000007F,
    MBOX_EFR_MODE                               = 0x00000177,
    MBOX_FR_MODE                                = 0x00001FFF,
    MBOX_HR_MODE                                = 0x00002FFF,
    MBOX_INVALID_MODE                           = 0x0000FFFF
} MBOX_SPEECH_MODE_T;


// ============================================================================
// SPC_DIGRF_CLKPOL_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef enum
{
    SPC_DIGRF_NORM_CLK_POL                      = 0x00000000,
    SPC_DIGRF_INV_CLK_POL                       = 0x00000001
} SPC_DIGRF_CLKPOL_T;


// ============================================================================
// SPC_DIGRF_OVERSAMP_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef enum
{
    SPC_DIGRF_1_SAMPLE_PER_SYMB                 = 0x00000000,
    SPC_DIGRF_2_SAMPLE_PER_SYMB                 = 0x00000001
} SPC_DIGRF_OVERSAMP_T;


// ============================================================================
// SPC_DIGRF_TX_MODE_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef enum
{
    SPC_DIGRF_TX_STREAM                         = 0x00000000,
    SPC_DIGRF_TX_BLOCK                          = 0x00000001
} SPC_DIGRF_TX_MODE_T;


// ============================================================================
// SPC_DCOC_MODE_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef enum
{
    SPC_NO_DCOC                                 = 0x00000000,
    SPC_HW_DCOC_ENABLED                         = 0x00000001,
    SPC_SW_DCOC_ENABLED                         = 0x00000002
} SPC_DCOC_MODE_T;


// ============================================================================
// SPC_MBOX_DBG_REQ_T
// -----------------------------------------------------------------------------
/// The debug mechanism is "block based" i.e. it allows to dump samples (or use external
/// ones) on 4 consecutives bursts constituting a block. Debug requests must be done
/// through PAL, which is handling the frame based mechanism.
// =============================================================================
typedef enum
{
    MBOX_DBG_EXT_SAMPLES                        = 0x00000001,
    MBOX_DBG_REC_SAMPLES                        = 0x00000002,
    MBOX_DBG_REC_REF                            = 0x00000004,
    MBOX_DBG_REC_TAPS                           = 0x00000008,
    MBOX_DBG_BST_PARAM                          = 0x00000010
} SPC_MBOX_DBG_REQ_T;


// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// SPC_MBOX_SPEECH_ENC_OUT_T
// -----------------------------------------------------------------------------
/// Structure used to configure VOC frame encode
// =============================================================================
typedef struct
{
    /// Mode of the voice coded output frame
    UINT16                         encMode;                      //0x00000000
    /// Type of the coded output frame (AMR only)
    UINT16                         encFrameType;                 //0x00000002
    /// Voice Activity Detection flag (all except AMR)
    UINT16                         vad;                          //0x00000004
    /// Speech flag (all except AMR)
    UINT16                         sp;                           //0x00000006
    /// Encoder output buffer (coded frame in enc_mode)
    UINT16                         encOutBuf[MBOX_MAX_SPEECH_FRAME_SIZE]; //0x00000008
} SPC_MBOX_SPEECH_ENC_OUT_T; //Size : 0x2C



// ============================================================================
// SPC_MBOX_SPEECH_DEC_IN_T
// -----------------------------------------------------------------------------
/// Structure used to configure VOC frame decode
// =============================================================================
typedef struct
{
    /// DTX allowed by the network, active-&amp;gt;1
    UINT16                         dtxOn;                        //0x00000000
    /// mode of the next encoder speech frame output and the current decoder speech
    /// frame input
    UINT16                         codecMode;                    //0x00000002
    /// Type of the coded input frame (AMR only)
    UINT16                         decFrameType;                 //0x00000004
    /// BFI flag (all vocoders except AMR)
    UINT16                         bfi;                          //0x00000006
    /// SID flag (all vocoders except AMR)
    UINT16                         sid;                          //0x00000008
    /// TAF flag (all vocoders except AMR)
    UINT16                         taf;                          //0x0000000A
    /// UFI flag (HR only)
    UINT16                         ufi;                          //0x0000000C
    /// reserved (for alignment)
    UINT16                         reserved;                     //0x0000000E
    /// Decoder input buffer (coded frame in dec_mode)
    UINT16                         decInBuf[MBOX_MAX_SPEECH_FRAME_SIZE]; //0x00000010
} SPC_MBOX_SPEECH_DEC_IN_T; //Size : 0x34



// ============================================================================
// SPC_MBOX_FCCH_RESULT_T
// -----------------------------------------------------------------------------
/// Result structure used for the frequency burst search the BCPU writes it when
/// the FB has been found or when the reception window is over.
// =============================================================================
typedef struct
{
    /// frequency offset
    UINT16                         fof;                          //0x00000000
    /// timing offset
    INT16                          tof;                          //0x00000002
    /// indicates if FCCH has been found
    BOOL                           valid;                        //0x00000004
} SPC_MBOX_FCCH_RESULT_T; //Size : 0x6



// ============================================================================
// SPC_MBOX_SCH_RESULT_T
// -----------------------------------------------------------------------------
/// Result structure used for the synchronization burst reception the BCPU writes
/// it after the SB has been processed.
// =============================================================================
typedef struct
{
    /// signal to noise ratio in dB
    UINT16                         snr;                          //0x00000000
    /// frequency offset computed on the burst
    INT16                          fof;                          //0x00000002
    /// data of the synchro burst
    UINT8                          data[MBOX_SCH_DATA_BYTE_QTY]; //0x00000004
    /// indicates if the SCH has been decoded correctly
    BOOL                           valid;                        //0x00000008
    /// timing offset
    INT8                           tof;                          //0x00000009
    /// power computed on the burst
    UINT8                          power;                        //0x0000000A
} SPC_MBOX_SCH_RESULT_T; //Size : 0xC



// ============================================================================
// SPC_MBOX_MON_RESULT_T
// -----------------------------------------------------------------------------
/// Result structure used for monitoring and interference windows
// =============================================================================
typedef struct
{
    UINT8                          step;                         //0x00000000
    UINT8                          power;                        //0x00000001
} SPC_MBOX_MON_RESULT_T; //Size : 0x2



// ============================================================================
// SPC_MBOX_NBLOCK_RESULT_T
// -----------------------------------------------------------------------------
/// Result structure used for block decoding when the BCPU has received enough bursts
/// it decodes a block and writes related results
// =============================================================================
typedef struct
{
    /// pointer on decoded data
    UINT8*                         data[MBOX_NBURST_DATA_QTY];   //0x00000000
    /// number of corrected bit in the block
    UINT16                         bitError;                     //0x00000008
    /// number of protected bit in the block
    UINT16                         bitTotal;                     //0x0000000A
    /// bit error probability on the block
    UINT8                          meanBep;                      //0x0000000C
    /// coefficient of variance of meanBep
    UINT8                          cvBep;                        //0x0000000D
    /// bad frame indicator
    UINT8                          bfi;                          //0x0000000E
    /// decoded coding scheme in GPRS
    UINT8                          cs;                           //0x0000000F
    /// indicates the silence frame detection
    UINT8                          sid;                          //0x00000010
} SPC_MBOX_NBLOCK_RESULT_T; //Size : 0x14



// ============================================================================
// SPC_MBOX_NBURST_RESULT_T
// -----------------------------------------------------------------------------
/// Result structure used for normal burst reception
// =============================================================================
typedef struct
{
    /// DC offset on I samples
    INT16                          dco_I;                        //0x00000000
    /// DC offset on Q samples
    INT16                          dco_Q;                        //0x00000002
    /// frequency offset measured on the burst
    INT16                          fof;                          //0x00000004
    /// timing offset
    INT8                           tof;                          //0x00000006
    /// signal to noise ratio in dB
    UINT8                          snr;                          //0x00000007
    /// power measured on the burst
    UINT8                          pwr;                          //0x00000008
    /// indicates if the burst belongs to a silence frame
    UINT8                          sid;                          //0x00000009
} SPC_MBOX_NBURST_RESULT_T; //Size : 0xA



// ============================================================================
// SPC_MBOX_WIN_SPC_TO_PAL_RX_T
// -----------------------------------------------------------------------------
/// Structure used for burst related Rx results from SPC to PAL
// =============================================================================
typedef struct
{
    /// Used for normal burst reception results
    SPC_MBOX_NBURST_RESULT_T       burstRes;                     //0x00000000
    /// Used for both multiple Monitoring windows and Interference window
    SPC_MBOX_MON_RESULT_T          monRes[MBOX_INTRF_SLOT_QTY];  //0x0000000A
} SPC_MBOX_WIN_SPC_TO_PAL_RX_T; //Size : 0x16



// ============================================================================
// SPC_MBOX_WIN_SPC_TO_PAL_T
// -----------------------------------------------------------------------------
/// Structure defined only to have the same organization as for PAL to SPC
// =============================================================================
typedef struct
{
    /// Structure used for burst related Rx results from SPC to PAL
    SPC_MBOX_WIN_SPC_TO_PAL_RX_T   rx;                           //0x00000000
} SPC_MBOX_WIN_SPC_TO_PAL_T; //Size : 0x16



// ============================================================================
// SPC_MBOX_STATIC_SPC_TO_PAL_RX_T
// -----------------------------------------------------------------------------
/// Structure used for block related Rx results from SPC to PAL
// =============================================================================
typedef struct
{
    /// fcch results structure
    SPC_MBOX_FCCH_RESULT_T         fcch;                         //0x00000000
    /// sch results structure
    SPC_MBOX_SCH_RESULT_T          sch[MBOX_SCH_QTY];            //0x00000006
    /// normal block results structure
    SPC_MBOX_NBLOCK_RESULT_T       nBlock[MBOX_MAX_BUF_IDX_QTY]; //0x00000014
    /// array of decoded USF in GPRS
    UINT8                          decodedUsf[MBOX_MAX_TS_QTY];  //0x00000078
} SPC_MBOX_STATIC_SPC_TO_PAL_RX_T; //Size : 0x7C



// ============================================================================
// SPC_MBOX_STATIC_SPC_TO_PAL_T
// -----------------------------------------------------------------------------
/// Structure defined only to have the same organization as for PAL to SPC
// =============================================================================
typedef struct
{
    /// Structure used for block related Rx results from SPC to PAL
    SPC_MBOX_STATIC_SPC_TO_PAL_RX_T rx;                          //0x00000000
} SPC_MBOX_STATIC_SPC_TO_PAL_T; //Size : 0x7C



// ============================================================================
// SPC_MBOX_SPC_TO_PAL_T
// -----------------------------------------------------------------------------
/// main structure for SPC to PAL
// =============================================================================
typedef struct
{
    /// window related results
    SPC_MBOX_WIN_SPC_TO_PAL_T      win[MBOX_CTX_SNAP_QTY];       //0x00000000
    /// block related results
    SPC_MBOX_STATIC_SPC_TO_PAL_T   statWin;                      //0x00000044
    /// decoding structure for VOC
    SPC_MBOX_SPEECH_DEC_IN_T       speechDecIn;                  //0x000000C0
    /// array of pointer given by SPC to store the data to encode +1 for CSD block
    /// to encode
    UINT8*                         encBuff[MBOX_MAX_BUF_IDX_QTY+1]; //0x000000F4
    /// used by SPC to warn PAL that DTX uplink has been used
    BOOL                           dtxUsed;                      //0x0000010C
    /// debug for BCPU
    UINT32                         debug[16];                    //0x00000110
} SPC_MBOX_SPC_TO_PAL_T; //Size : 0x150



// ============================================================================
// SPC_MBOX_AMR_CFG_T
// -----------------------------------------------------------------------------
/// Structure used for the AMR management, it contains all the configuration parameters
/// relative to the AMR modes and link adaptation. The XCPU writes it at init but
/// can also read it to retrieve the last AMR configuration.
// =============================================================================
typedef struct
{
    UINT8                          version;                      //0x00000000
    UINT8                          noiseSuppression;             //0x00000001
    UINT8                          startModeIdx;                 //0x00000002
    UINT8                          nbActiveCodecSet;             //0x00000003
    UINT8                          activeCodecSet[4];            //0x00000004
    UINT8                          hysteresis[3];                //0x00000008
    UINT8                          threshold[3];                 //0x0000000B
    UINT8                          CMIPhase;                     //0x0000000E
} SPC_MBOX_AMR_CFG_T; //Size : 0xF



// ============================================================================
// SPC_MBOX_RX_NBURST_T
// -----------------------------------------------------------------------------
/// Structure used by PAL to parameter a Rx window
// =============================================================================
typedef struct
{
    /// buffer index of the normal burst (from 0 to 4)
    UINT8                          bufIdx;                       //0x00000000
    /// burst index of the burst (from 0 to 3)
    UINT8                          bstIdx;                       //0x00000001
    /// color of the training sequence
    UINT8                          tsc;                          //0x00000002
    /// indicates which slots are used in the RX window
    UINT8                          bitMap;                       //0x00000003
} SPC_MBOX_RX_NBURST_T; //Size : 0x4



// ============================================================================
// SPC_MBOX_STATWIN_RX_CFG_T
// -----------------------------------------------------------------------------
/// Structure used to describe the logical channel associated with a Rx buffer index
/// and to indicate wether a decoding interruption is asked or not.
// =============================================================================
typedef struct
{
    /// logical channel type
    UINT8                          type;                         //0x00000000
    /// indicates if the decoding interruption is requested by PAL
    UINT8                          itr;                          //0x00000001
} SPC_MBOX_STATWIN_RX_CFG_T; //Size : 0x2



// ============================================================================
// SPC_MBOX_WIN_TX_ACCESS_ENC_T
// -----------------------------------------------------------------------------
/// Structure dedicated to AB encoding
// =============================================================================
typedef struct
{
    /// data to encode
    UINT16                         data;                         //0x00000000
    /// base transceiver station identity code
    UINT8                          bsic;                         //0x00000002
    /// size of the data in bits
    UINT8                          size;                         //0x00000003
} SPC_MBOX_WIN_TX_ACCESS_ENC_T; //Size : 0x4



// ============================================================================
// SPC_MBOX_STATWIN_TX_CTX_T
// -----------------------------------------------------------------------------
/// Structure used to describe the logical channel and coding scheme in case of GPRS
/// associated with a Tx buffer index and also to indicates if the data have been
/// encoded or not.
// =============================================================================
typedef struct
{
    /// logical channel type associated with a buffer index
    UINT8                          type;                         //0x00000000
    /// set by PAL to ask for encoding of the block and reset by SPC when encoding
    /// has been done
    UINT8                          encDone;                      //0x00000001
    /// block coding scheme in GPRS
    UINT8                          cs;                           //0x00000002
} SPC_MBOX_STATWIN_TX_CTX_T; //Size : 0x3



// ============================================================================
// SPC_MBOX_WIN_PAL_TO_SPC_TX_T
// -----------------------------------------------------------------------------
/// Structure used for windows emission programming
// =============================================================================
typedef struct
{
    /// this array gives the buffer indexes associated with the slots in the Tx window
    /// (it contains more than one usefull value only in case of GPRS multislot window)
    UINT8                          routeMap[MBOX_MAX_TS_QTY];    //0x00000000
    /// index of the Tx window in the frame
    UINT8                          winIdx;                       //0x00000004
    /// burst index (from 0 to 3)
    UINT8                          bstIdx;                       //0x00000005
    /// training sequence color
    UINT8                          tsc;                          //0x00000006
    /// number of programmed Tx windows
    UINT8                          qty;                          //0x00000007
    /// indicates which slots are used in the Tx window
    UINT8                          bitMap;                       //0x00000008
    /// indicates if I and Q have to be swapped
    BOOL                           swapIQ;                       //0x00000009
} SPC_MBOX_WIN_PAL_TO_SPC_TX_T; //Size : 0xA



// ============================================================================
// SPC_MBOX_WIN_PAL_TO_SPC_RX_T
// -----------------------------------------------------------------------------
/// Structure used for window related Rx parameters, from PAL to SPC
// =============================================================================
typedef struct
{
    /// number of programmed Rx window
    UINT8                          qty;                          //0x00000000
    /// type of the Rx window (FB, SB, NB or monitoring) only one FB window can be
    /// programmed per TDMA frame only one SB window can be programmed per TDMA frame
    /// only one NB window can be programmed per TDMA frame
    UINT8                          rxTypeWin[MBOX_WINDOWS_QTY];  //0x00000001
    /// index of the programmed monitoring windows used for both monitoring and interference
    /// windows
    UINT8                          monWinIdx[MBOX_WINDOWS_QTY];  //0x00000004
    /// parameters of the programmed NB Rx window
    SPC_MBOX_RX_NBURST_T           nBurst;                       //0x00000007
    /// indicates for each Rx window if I and Q have to be swapped
    BOOL                           swapIQ[MBOX_WINDOWS_QTY];     //0x0000000B
} SPC_MBOX_WIN_PAL_TO_SPC_RX_T; //Size : 0xE



// ============================================================================
// SPC_MBOX_WIN_PAL_TO_SPC_T
// -----------------------------------------------------------------------------
/// Structure all window related from PAL to SPC
// =============================================================================
typedef struct
{
    /// Structure used for windows emission programming
    SPC_MBOX_WIN_PAL_TO_SPC_TX_T   tx;                           //0x00000000
    /// Structure used for window related Rx parameters, from PAL to SPC
    SPC_MBOX_WIN_PAL_TO_SPC_RX_T   rx;                           //0x0000000A
} SPC_MBOX_WIN_PAL_TO_SPC_T; //Size : 0x18



// ============================================================================
// SPC_MBOX_STATIC_PAL_TO_SPC_TX_T
// -----------------------------------------------------------------------------
/// Structure used for blocks related Tx parameters, from PAL to SPC
// =============================================================================
typedef struct
{
    /// Structure dedicated to AB encoding
    SPC_MBOX_WIN_TX_ACCESS_ENC_T   enc;                          //0x00000000
    /// Structure used to describe the logical channel and coding scheme in case
    /// of GPRS associated with a Tx buffer index and also to indicates if the data
    /// have been encoded or not.
    SPC_MBOX_STATWIN_TX_CTX_T      ctx[MBOX_MAX_BUF_IDX_QTY];    //0x00000004
} SPC_MBOX_STATIC_PAL_TO_SPC_TX_T; //Size : 0x14



// ============================================================================
// SPC_MBOX_STATIC_PAL_TO_SPC_RX_T
// -----------------------------------------------------------------------------
/// Structure used for blocks related Rx parameters, from PAL to SPC
// =============================================================================
typedef struct
{
    /// Structure used to describe the logical channel associated with a Rx buffer
    /// index and to indicate wether a decoding interruption is asked or not.
    SPC_MBOX_STATWIN_RX_CFG_T      cfg[MBOX_MAX_BUF_IDX_QTY];    //0x00000000
} SPC_MBOX_STATIC_PAL_TO_SPC_RX_T; //Size : 0xA



// ============================================================================
// SPC_MBOX_STATIC_PAL_TO_SPC_DEDICATED_T
// -----------------------------------------------------------------------------
/// Structure concerning the current dedicated TCH
// =============================================================================
typedef struct
{
    /// AMR configuration
    SPC_MBOX_AMR_CFG_T             AMRCfg;                       //0x00000000
    /// indicates if the AMR configuration has been modified by PAL
    UINT8                          changedAMRCfg;                //0x0000000F
    /// current mode of the TCH as defined in PAL interface
    UINT8                          mode;                         //0x00000010
    /// indicates if the TCH is started or stopped (respectively 1 and 0)
    BOOL                           active;                       //0x00000011
    /// indicates if the parameters of the TCH has to be reinitialized
    BOOL                           initTch;                      //0x00000012
} SPC_MBOX_STATIC_PAL_TO_SPC_DEDICATED_T; //Size : 0x13



// ============================================================================
// SPC_MBOX_STATIC_PAL_TO_SPC_T
// -----------------------------------------------------------------------------
/// Structure used for blocks related parameters, from PAL to SPC
// =============================================================================
typedef struct
{
    /// Structure used for blocks related Tx parameters, from PAL to SPC
    SPC_MBOX_STATIC_PAL_TO_SPC_TX_T tx;                          //0x00000000
    /// Structure used for blocks related Rx parameters, from PAL to SPC
    SPC_MBOX_STATIC_PAL_TO_SPC_RX_T rx;                          //0x00000014
    /// Structure concerning the current dedicated TCH
    SPC_MBOX_STATIC_PAL_TO_SPC_DEDICATED_T dedicated;            //0x0000001E
    /// mode for test loop see 3GPP 44.014 for more details
    UINT8                          loopMode;                     //0x00000031
    /// mode of ciphering if enabled
    UINT8                          ciphMode;                     //0x00000032
} SPC_MBOX_STATIC_PAL_TO_SPC_T; //Size : 0x34



// ============================================================================
// SPC_MBOX_GSM_COUNTERS_T
// -----------------------------------------------------------------------------
/// Structure used to store GSM TDMA frame counters
// =============================================================================
typedef struct
{
    UINT16                         T1;                           //0x00000000
    UINT8                          T2;                           //0x00000002
    UINT8                          T3;                           //0x00000003
} SPC_MBOX_GSM_COUNTERS_T; //Size : 0x4



// ============================================================================
// SPC_MBOX_RF_PARAMS_T
// -----------------------------------------------------------------------------
/// Structure containing all RF-control related parameters
// =============================================================================
typedef struct
{
    /// settings for TCO during DTX uplink
    UINT32                         dtxTcoSettings;               //0x00000000
    /// DigRf settings
    BOOL                           digrfEnable;                  //0x00000004
    UINT8                          digrfRxRate;                  //0x00000005
    UINT8                          digrfRxSampleShift;           //0x00000006
    UINT8                          digrfRxClkPol;                //0x00000007
    UINT8                          digrfTxMode;                  //0x00000008
    UINT8                          digrfTxClkPol;                //0x00000009
    UINT8                          digrfPreambleBits;            //0x0000000A
    UINT8                          digrfPostambleBits;           //0x0000000B
    /// Enable Automatic DC Offset calibration mechanism
    BOOL                           dcoCalEnable;                 //0x0000000C
} SPC_MBOX_RF_PARAMS_T; //Size : 0x10



// ============================================================================
// SPC_MBOX_PAL_TO_SPC_T
// -----------------------------------------------------------------------------
/// main Structure for PAL to SPC
// =============================================================================
typedef struct
{
    /// window related programmations
    SPC_MBOX_WIN_PAL_TO_SPC_T      win[MBOX_CTX_SNAP_QTY];       //0x00000000
    /// block related programmations
    SPC_MBOX_STATIC_PAL_TO_SPC_T   statWin;                      //0x00000048
    /// GSM TDMA counter
    SPC_MBOX_GSM_COUNTERS_T        counters[MBOX_CTX_SNAP_QTY];  //0x0000007C
    /// encoding structure comming from VOC
    SPC_MBOX_SPEECH_ENC_OUT_T      speechEncOut;                 //0x00000088
    /// indicates to SPC if the DTX uplink is allowed for the current TCH
    BOOL                           dtxAllowed;                   //0x000000B4
    /// RF parameters
    SPC_MBOX_RF_PARAMS_T           rf;                           //0x000000B8
} SPC_MBOX_PAL_TO_SPC_T; //Size : 0xC8



// ============================================================================
// SPC_MBOX_DEBUG_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef struct
{
    UINT32*                        ext_samples;                  //0x00000000
    UINT32*                        samples;                      //0x00000004
    UINT32*                        ref;                          //0x00000008
    UINT32*                        taps;                         //0x0000000C
    UINT32                         req;                          //0x00000010
} SPC_MBOX_DEBUG_T; //Size : 0x14



// ============================================================================
// SPC_MAILBOX_T
// -----------------------------------------------------------------------------
/// The main structure which gathers the three different parts of the mailbox.
// =============================================================================
typedef struct
{
    /// structure used to program BCPU and give context parameters values
    SPC_MBOX_PAL_TO_SPC_T          pal2spc;                      //0x00000000
    /// structure used to exchange BCPU processing results
    SPC_MBOX_SPC_TO_PAL_T          spc2pal;                      //0x000000C8
    /// #ifdef __MBOX_DEBUG__ This is used by the embedded debug stub
    SPC_MBOX_DEBUG_T               dbg[MBOX_CTX_SNAP_QTY];       //0x00000218
    UINT32                         palDbgReq;                    //0x00000254
    UINT32                         palDbgAck;                    //0x00000258
} SPC_MAILBOX_T; //Size : 0x25C





/// g_mailbox is a global variable of type SPC_MAILBOX_T which is used
extern volatile SPC_MAILBOX_T g_mailbox;


// =============================================================================
// bcpu_main
// -----------------------------------------------------------------------------
/// BCPU main function.
// =============================================================================
PUBLIC VOID bcpu_main(VOID);

/// @}


#endif

