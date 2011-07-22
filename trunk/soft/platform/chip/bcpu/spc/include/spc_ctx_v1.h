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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/bcpu/spc/include/spc_ctx_v1.h $                                                                   
//  $Author: admin $                                                                    
//  $Date: 2010-07-07 20:28:03 +0800 (Wed, 07 Jul 2010) $                                                                      
//  $Revision: 269 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
/// Contains stuctures and function definitions for SPC use.
//
//==============================================================================

#ifndef _SPC_CTX_V1_H_
#define _SPC_CTX_V1_H_


#include "cs_types.h"
#include "spp_m.h"
#if (CHIP_EDGE_SUPPORTED == 1)
    #include "spc_define_edge.h"
#else
    #include "spc_define_std.h"
#endif
#include "baseband_defs.h"
#include "spc_mailbox_v1.h"
#include "spp_gsm.h"

// =============================================================================
//  MACROS
// =============================================================================
/// SCH symbol buffer offset (aVOID overwriting during late SCH windows)
#define SCH_BUF_OFFSET                           ((BB_BURST_TOTAL_SIZE*4))
/// Interleaver Buffer sizes
#define SPC_ITLV_BUFF_TX_FACCH                   ((8*4))
/// 4burst*4word*4bytes
#define SPC_ITLV_BUFF_TX_CTRL                    ((4*4))
#define SPC_ITLV_BUFF_RX_CTRL                    ((4*32))
#define SPC_ITLV_BUFF_SIZE                       (SPC_ITLV_BUFF_TX_FACCH + SPC_ITLV_BUFF_TX_CTRL + SPC_ITLV_BUFF_TX_DEDICATED + SPC_ITLV_BUFF_RX_CTRL + SPC_ITLV_BUFF_RX_DEDICATED)
#define SPC_STATIC_BUF_SIZE                      (4*BB_BURST_TOTAL_SIZE + 6*MBOX_MON_WIN_SIZE)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// SPC_ITLV_BUFFERS_T
// -----------------------------------------------------------------------------
/// Interleaver Buffer Structure
// =============================================================================
typedef struct
{
    UINT32                         RxCtrl[SPC_ITLV_BUFF_RX_CTRL]; //0x00000000
    UINT32                         RxDedicated[SPC_ITLV_BUFF_RX_DEDICATED]; //0x00000200
    UINT32                         TxCtrl[SPC_ITLV_BUFF_TX_CTRL]; //0x00000D00
    UINT32                         TxDedicated[SPC_ITLV_BUFF_TX_DEDICATED]; //0x00000D40
    UINT32                         TxFacch[SPC_ITLV_BUFF_TX_FACCH]; //0x00000EA0
} SPC_ITLV_BUFFERS_T; //Size : 0xF20



// ============================================================================
// SPC_STATIC_BUFFERS_T
// -----------------------------------------------------------------------------
/// Overall structure : Rx (Ifc)Buffer + Vitac Buffers
// =============================================================================
typedef struct
{
    UINT32                         RxIfcBuffer[4*(BB_BURST_ACTIVE_SIZE+2*BB_RX_NBURST_MARGIN)]; //0x00000000
    UINT32                         MonIfcBuffer[6*MBOX_MON_WIN_SIZE]; //0x000009C0
} SPC_STATIC_BUFFERS_T; //Size : 0xCC0



// ============================================================================
// SPC_DATA_BLOCK_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef UINT32 SPC_DATA_BLOCK_T[14];


// ============================================================================
// SPC_TX_SPEECH_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef struct
{
    UINT32*                        InputBuffer;                  //0x00000000
    UINT16                         CodecModeInd;                 //0x00000004
    UINT8                          TxFrameType;                  //0x00000006
} SPC_TX_SPEECH_T; //Size : 0x8



// ============================================================================
// SPC_RX_LOCALS_T
// -----------------------------------------------------------------------------
/// structure for local rx variables
// =============================================================================
typedef struct
{
    UINT32*                        ItlvBuffInput;                //0x00000000
    UINT32*                        BlockDecodedAdd;              //0x00000004
    UINT16                         Copy_ChMode;                  //0x00000008
    UINT8                          burstIdx;                     //0x0000000A
    UINT8                          buffIdx;                      //0x0000000B
    UINT8                          slotIdx;                      //0x0000000C
    UINT8                          RxBuffOffset;                 //0x0000000D
    UINT8                          itlv_mode;                    //0x0000000E
    UINT8                          StealingResult;               //0x0000000F
    UINT8                          voc_bfi;                      //0x00000010
    UINT8                          voc_sid;                      //0x00000011
    UINT8                          voc_taf;                      //0x00000012
    UINT8                          voc_ufi;                      //0x00000013
    UINT8                          RxFrameType;                  //0x00000014
    UINT8                          RX_decode;                    //0x00000015
} SPC_RX_LOCALS_T; //Size : 0x18



// ============================================================================
// SPC_8UINT8_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef UINT8 SPC_8UINT8_T[8];


// ============================================================================
// SPC_CONTEXT_T
// -----------------------------------------------------------------------------
/// Global structure of Context statically placed in BB_SRAM Should be referenced
/// by a pointer placed in a reserved register to ease address calculation and reduce
/// code size.
// =============================================================================
typedef struct
{
    /// FCCH Result
    SPP_FCCH_RESULT_T              FCCH_Result;                  //0x00000000
    /// SCH Result
    SPP_SCH_RESULT_T               SCH_Result;                   //0x00000004
    /// Normal Burst Result
    SPP_N_BURST_RESULT_T           NBurst_result;                //0x00000010
    /// Normal block Result
    SPP_N_BLOCK_RESULT_T           NBlock_result;                //0x0000001C
    /// Header structure for Egprs
    SPP_HEADER_RESULT_T            Header_result;                //0x00000026
    /// AMR Config
    SPP_AMR_CFG_T                  AMR_Cfg_Tx;                   //0x00000030
    /// AMR Config
    SPP_AMR_CFG_T                  AMR_Cfg_Rx;                   //0x0000003F
    UINT32                         Rx_Hu_TCH_N_1;                //0x00000050
    UINT32                         Rx_Hu_TCH_N;                  //0x00000054
    UINT32                         Rx_Hl_TCH_N;                  //0x00000058
    /// Tx buffers
    UINT32                         ABurstBuffer[5];              //0x0000005C
    SPC_DATA_BLOCK_T               CSD_NBlockDataIn;             //0x00000070
    SPC_DATA_BLOCK_T               CCH_NBlockDataIn;             //0x000000A8
    SPC_PDCH_DATA_BLOCK_T          DCH_NBlockDataIn[MBOX_MAX_BUF_IDX_QTY-1]; //0x000000E0
    UINT32*                        NBlockDataIn[MBOX_MAX_BUF_IDX_QTY+1]; //0x000001C0
    /// Rx buffers
    SPC_MBOX_SPEECH_ENC_OUT_T      speechEncOut;                 //0x000001D8
    SPC_DATA_BLOCK_T               CSD_NBlockDataOut;            //0x00000204
    SPC_DATA_BLOCK_T               CCH_NBlockDataOut;            //0x0000023C
    SPC_PDCH_DATA_BLOCK_T          DCH_NBlockDataOut[MBOX_MAX_BUF_IDX_QTY-1]; //0x00000274
    UINT32*                        NBlockDataOut[MBOX_MAX_BUF_IDX_QTY+1]; //0x00000354
    /// interleaver input buffer pointers
    UINT32*                        ItlvBufRxCtrl;                //0x0000036C
    UINT32*                        ItlvBufRxDedicated;           //0x00000370
    UINT32*                        ItlvBufTxCtrl;                //0x00000374
    UINT32*                        ItlvBufTxDedicated;           //0x00000378
    UINT32*                        ItlvBufTxFacch;               //0x0000037C
    /// interleaver output buffer
    VOLATILE UINT32*               Malloc;                       //0x00000380
    /// next address must be aligned on 128 bytes
    UINT32*                        ItlvBufRxFacch;               //0x00000384
    UINT8*                         loopC_buffer;                 //0x00000388
    INT8*                          CIest;                        //0x0000038C
    UINT32                         BaseAddress;                  //0x00000390
    UINT16                         FCCH_Offset;                  //0x00000394
    UINT16                         ChMode;                       //0x00000396
    UINT16                         PrevChMode;                   //0x00000398
    BOOL                           DedicatedActive;              //0x0000039A
    /// Structure used to describe the logical channel associated with a Rx buffer
    /// index and to indicate wether a decoding interruption is asked or not.
    SPC_MBOX_STATWIN_RX_CFG_T      statWinRxCfg[MBOX_MAX_BUF_IDX_QTY]; //0x0000039B
    BOOL                           Tx_off;                       //0x000003AA
    BOOL                           Init_FCCH;                    //0x000003AB
    INT16                          BlockSnR;                     //0x000003AC
    INT16                          LastBlockSnR;                 //0x000003AE
    SPC_8UINT8_T                   Rx_q_CCH[4];                  //0x000003B0
    /// Was UINT8 Rx_q_CCH[4][8];, but coolXml has its own ...
    UINT8                          Rx_CS[4];                     //0x000003D0
    UINT8                          BurstNb;                      //0x000003D4
    UINT8                          currentSnap;                  //0x000003D5
    UINT8                          TxBlockOffset;                //0x000003D6
    UINT8                          RxBlockOffset;                //0x000003D7
    UINT8                          cs_identifier[4];             //0x000003D8
    UINT8                          Tx_Hu;                        //0x000003DC
    UINT8                          Tx_Hl;                        //0x000003DD
    UINT8                          FacchEncoded;                 //0x000003DE
    UINT8                          DTX_on;                       //0x000003DF
    UINT32                         RfIfCtrl;                     //0x000003E0
    BOOL                           DCOC_on;                      //0x000003E4
    UINT8                          FcchFound;                    //0x000003E5
    UINT8                          txQty;                        //0x000003E6
    UINT8                          rxQty;                        //0x000003E7
    UINT8                          Taf_Flag;                     //0x000003E8
    UINT8                          DTX_dwnlk_flag;               //0x000003E9
    UINT8                          DTX_dwnlk_count;              //0x000003EA
    UINT8                          BFI_count;                    //0x000003EB
    UINT8                          sendEqItr;                    //0x000003EC
    UINT8                          resIdx;                       //0x000003ED
    UINT8                          frame_over;                   //0x000003EE
    UINT8                          loop_bufIdx;                  //0x000003EF
    UINT8                          CodecModeRequest;             //0x000003F0
    UINT8                          ratscch_to_encode;            //0x000003F1
    UINT8                          ACK_Activation;               //0x000003F2
    UINT8                          REQ_Activation;               //0x000003F3
    UINT8                          RATSCCH_REQ;                  //0x000003F4
    UINT8                          ratscch_detected;             //0x000003F5
    UINT8                          sidupdate_ctr;                //0x000003F6
    UINT8                          AFNmod104;                    //0x000003F7
    /// old defines moved to variables for romming purpose
    INT16                          FacchThreshold;               //0x000003F8
    UINT8                          BfiThreshold;                 //0x000003FA
    UINT8                          UfiThreshold;                 //0x000003FB
    UINT8                          FsBfiBerThresh;               //0x000003FC
    UINT8                          FsBfiBer2Thresh;              //0x000003FD
    INT8                           FsBfiSnrThresh;               //0x000003FE
    UINT8                          HsBfiBerThresh;               //0x000003FF
    UINT8                          HsBfiBer2Thresh;              //0x00000400
    INT8                           HsBfiSnrThresh;               //0x00000401
    UINT8                          AFS_ber1_threshold[8];        //0x00000402
    UINT8                          AHS_ber1_threshold[6];        //0x0000040A
    UINT8                          AFS_ber2_threshold[8];        //0x00000410
    UINT8                          AHS_ber2_threshold[6];        //0x00000418
    /// extern volatile UINT32 bb_irq_cause;
    VOLATILE BOOL                  equ_hburst_mode;              //0x0000041E
    VOLATILE UINT32                bcpuIrqCause;                 //0x00000420
    VOLATILE UINT8                 FcchInit;                     //0x00000424
    VOLATILE UINT8                 FcchActive;                   //0x00000425
    VOLATILE UINT8                 FcchSet;                      //0x00000426
    VOLATILE UINT8                 rx_count;                     //0x00000427
    VOLATILE UINT8                 sched_rx_count;               //0x00000428
    VOLATILE UINT8                 slot_count;                   //0x00000429
    VOLATILE UINT8                 sched_slot_count;             //0x0000042A
    VOLATILE UINT8                 ifc2_pending;                 //0x0000042B
    VOLATILE UINT8                 rxslotNb;                     //0x0000042C
    VOLATILE UINT8                 tx_left;                      //0x0000042D
    VOLATILE UINT32                burst_to_send[10];            //0x00000430
    VOLATILE UINT32*               SilentFrame;                  //0x00000458
    /// enable for DTX uplink
    VOLATILE UINT32                voc_dtx_en;                   //0x0000045C
    /// AMR codec variables
    VOLATILE UINT32                TxCodecMode;                  //0x00000460
    VOLATILE UINT32                RxCodecMode;                  //0x00000464
    /// used for EGPRS only
    SPP_MODULATION_TYPE_T          tx_modulation;                //0x00000468
    SPP_MODULATION_TYPE_T          rx_modulation;                //0x0000046C
    SPP_HEADER_TYPE_T              tx_headertype[4];             //0x00000470
    SPP_HEADER_TYPE_T              rx_headertype[4];             //0x00000480
    /// Pointer to a function called during the idle time of the BCPU scheduler.
    VOLATILE VOID*                 schedulerHook;                //0x00000490
    /// Pointer to a function called by the BCPU IRQ handler.
    VOLATILE VOID*                 irqHook;                      //0x00000494
} SPC_CONTEXT_T; //Size : 0x498







register SPC_CONTEXT_T* g_spcCtx asm("$16");
EXPORT PROTECTED SPC_ITLV_BUFFERS_T g_spcItlvBufTab;
EXPORT PROTECTED SPC_STATIC_BUFFERS_T g_spcStaticBufTab;



#endif

