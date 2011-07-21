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
//  $HeadURL: http://10.10.100.14/svn/developing/Sources/platform/branches/Total_BJ/calib/include/calib_m.h $                                                                   
//  $Author: xuml $                                                                    
//  $Date$                                                                      
//  $Revision: 36808 $                                                                  
//                                                                              
//==============================================================================
//
/// @file
/// <!-- TODO Add a complete comment header, with @mainpage, etc --> @defgroup calib
/// Calibration Driver
///  @{
/// 
//
//==============================================================================

#ifndef _CALIB_M_H_
#define _CALIB_M_H_


#include "cs_types.h"
#include "gsm.h"

// =============================================================================
//  MACROS
// =============================================================================
/// Version of the calibration stub and code (on 8 bits).
#define CALIB_MAJ_VERSION                        (2)
#define CALIB_MIN_VERSION                        (2)
#define CALIB_MARK_VERSION                       (0XCA1B0000)
#define CALIB_VERSION_NUMBER                     (( CALIB_MARK_VERSION | CALIB_MAJ_VERSION << 8 | CALIB_MIN_VERSION ))
#define CALIB_STUB_VERSION                       (0XCA5B0001)
/// Number of calib timings for the transceiver.
#define CALIB_XCV_TIME_QTY                       (20)
/// Number of calib timings for the PA.
#define CALIB_PA_TIME_QTY                        (15)
/// Number of calib timings for the switch.
#define CALIB_SW_TIME_QTY                        (15)
/// Number of calib timings for PAL.
#define CALIB_PAL_TIME_QTY                       (20)
/// Number of generic parameters for the transceiver.
#define CALIB_XCV_PARAM_QTY                      (20)
/// Number of generic parameters for the PA.
#define CALIB_PA_PARAM_QTY                       (15)
/// Number of generic parameters for the switch.
#define CALIB_SW_PARAM_QTY                       (15)
/// Mask for transceiver RF name.
#define CALIB_XCV_MASK                           ((1 << 24))
/// Mask for PA RF name.
#define CALIB_PA_MASK                            ((1 << 25))
/// Mask for switch RF name.
#define CALIB_SW_MASK                            ((1 << 26))
#define CALIB_GSM_PCL_QTY                        (15)
#define CALIB_DCS_PCL_QTY                        (17)
#define CALIB_PCS_PCL_QTY                        (18)
#define CALIB_PADAC_PROF_INTERP_QTY              (16)
#define CALIB_PADAC_PROF_QTY                     (1024)
#define CALIB_PADAC_RAMP_QTY                     (32)
#define CALIB_LOW_VOLT_QTY                       (6)
/// Number of coefficiens in the MDF FIR filter.
#define CALIB_VOC_MDF_QTY                        (64)
/// Number of coefficiens in the SDF FIR filter.
#define CALIB_VOC_SDF_QTY                        (64)
/// Mask for echo cancelation enable (to be used with audio VoC enable).
#define CALIB_EC_ON                              ((1 << 0))
/// Mask for MDF FIR filter enable (to be used with audio VoC enable).
#define CALIB_MDF_ON                             ((1 << 1))
/// Mask for SDF FIR filter enable (to be used with audio VoC enable).
#define CALIB_SDF_ON                             ((1 << 2))
/// Number of audio gain steps.
#define CALIB_AUDIO_GAIN_QTY                     (8)
/// Number of misc audio parameters.
#define CALIB_AUDIO_PARAM_QTY                    (8)
/// Number of power management driver parameters.
#define CALIB_PMD_QTY                            (2)
/// Unrealistic values meaning that the power measure is not complete yet.
#define CALIB_STUB_SEARCH_POWER                  (0X0)
/// Unrealistic values meaning that the FOf measure is not complete yet.
#define CALIB_STUB_SEARCH_FOF                    (-2000000)
/// For communication between Calib Stub and calibration tools.
#define CALIB_STUB_XTAL_IDLE                     (-2000001)
/// For communication between Calib Stub and calibration tools.
#define CALIB_STUB_PA_PROF_IDLE                  (-2000002)
/// For communication between Calib Stub and calibration tools.
#define CALIB_STUB_ILOSS_IDLE                    (-128)
/// For communication between Calib Stub and calibration tools.
#define CALIB_STUB_DCO_IDLE                      (-32768)
/// For communication between Calib Stub and calibration tools.
#define CALIB_STUB_DCO_ERROR                     (-32767)
/// For communication between Calib Stub and calibration tools.
#define CALIB_STUB_GPADC_ERROR                   (0XFFFF)
/// For communication between Calib Stub and calibration tools.
#define CALIB_STUB_GPADC_IDLE                    (0XFFFE)
/// Number of cells used for measurement averages
#define CALIB_NB_CELLS                           (5)
/// Maximum number of different Audio Interfaces supported by this calibration structure.
/// The value of CALIB_AUDIO_ITF_QTY must be the same as AUD_ITF_QTY!
#define CALIB_AUDIO_ITF_QTY                      (6)
/// The number of GP ADC channels.
#define CALIB_GPADC_CH_QTY                       (4)
#define CALIB_GPADC_ACC_COUNT                    (8)
#define CALIB_GPADC_ACC_COUNT_MAX                (128)
#define CALIB_AUDIO_DICTA_REC                    (1)
#define CALIB_AUDIO_DICTA_PLAY                   (2)

// ============================================================================
// CALIB_METHOD_T
// -----------------------------------------------------------------------------
/// Calib process method type.
// =============================================================================
typedef enum
{
    CALIB_METH_DEFAULT                          = 0xCA11E700,
    CALIB_METH_COMPILATION                      = 0xCA11E701,
    CALIB_METH_MANUAL                           = 0xCA11E702,
    CALIB_METH_AUTOMATIC                        = 0xCA11E703,
    CALIB_METH_SIMULATION                       = 0xCA11E704
} CALIB_METHOD_T;


// ============================================================================
// CALIB_H_ENUM_0_T
// -----------------------------------------------------------------------------
/// Used for autonomous calib processes, results from Calib Stub to remote.
// =============================================================================
typedef enum
{
    CALIB_PROCESS_STOP                          = 0x00000000,
    CALIB_PROCESS_CONTINUE                      = 0x00000001,
    CALIB_PROCESS_PENDING                       = 0x00000002,
    CALIB_PROCESS_NEED_CALM                     = 0x00000003,
    CALIB_PROCESS_ERR_BAD_POW                   = 0x000000F0,
    CALIB_PROCESS_ERR_NO_MONO_POW               = 0x000000F1,
    CALIB_PROCESS_ERR_ZERO_DAC                  = 0x000000F2,
    CALIB_PROCESS_ERROR                         = 0x000000FF
} CALIB_H_ENUM_0_T;


// ============================================================================
// CALIB_STUB_BAND_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef enum
{
    CALIB_STUB_BAND_GSM850                      = 0x00000000,
    CALIB_STUB_BAND_GSM900                      = 0x00000001,
    CALIB_STUB_BAND_DCS1800                     = 0x00000002,
    CALIB_STUB_BAND_PCS1900                     = 0x00000003,
    CALIB_STUB_BAND_QTY                         = 0x00000004
} CALIB_STUB_BAND_T;


// ============================================================================
// CALIB_STUB_CMDS_T
// -----------------------------------------------------------------------------
/// Values used to define the contexts of the Calib Stub.
// =============================================================================
typedef enum
{
    CALIB_STUB_NO_STATE                         = 0x00000000,
    CALIB_STUB_MONIT_STATE                      = 0x00000001,
    CALIB_STUB_FOF_STATE                        = 0x00000002,
    CALIB_STUB_TX_STATE                         = 0x00000003,
    CALIB_STUB_PA_STATE                         = 0x00000004,
    CALIB_STUB_AUDIO_OUT                        = 0x00000005,
    CALIB_STUB_AUDIO_IN                         = 0x00000006,
    CALIB_STUB_AUDIO_SIDE                       = 0x00000007,
    CALIB_STUB_SYNCH_STATE                      = 0x00000008,
    CALIB_STUB_IDLE_STATE                       = 0x00000009
} CALIB_STUB_CMDS_T;


// ============================================================================
// CALIB_STUB_AFC_BOUND_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef enum
{
    CALIB_STUB_AFC_BOUND_CENTER                 = 0x00000000,
    CALIB_STUB_AFC_BOUND_NEG_FREQ               = 0x00000001,
    CALIB_STUB_AFC_BOUND_POS_FREQ               = 0x00000002,
    CALIB_STUB_AFC_BOUND_NO                     = 0x00000003
} CALIB_STUB_AFC_BOUND_T;

/// Value that defines the number of measure to do before the DC offset average is
/// considered valid.
#define CALIB_DCO_ACC_COUNT                      (32)
/// This magiv tag is used as a parameter to the boot loader to force is to run the
/// calibration stub
#define CALIB_MAGIC_TAG                          (0XCA1BCA1B)

// ============================================================================
// CALIB_COMMAND_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef enum
{
/// Command used by the Calibration Embedded Stub to inform HST that the command
/// is done.
    CALIB_CMD_DONE                              = 0xCA11B042,
    CALIB_CMD_NOT_ACCESSIBLE                    = 0xCA11B043,
    CALIB_CMD_UPDATE                            = 0xCA11B044,
    CALIB_CMD_UPDATE_ERROR                      = 0xCA11B045,
    CALIB_CMD_PA_PROFILE_GSM                    = 0xCA11B046,
    CALIB_CMD_PA_PROFILE_DCSPCS                 = 0xCA11B047,
    CALIB_CMD_FLASH_BURN                        = 0xCA11B048,
    CALIB_CMD_FLASH_ERASE                       = 0xCA11B049,
    CALIB_CMD_FLASH_ERROR                       = 0xCA11B04A,
    CALIB_CMD_RESET                             = 0xCA11B04B,
    CALIB_CMD_RF_RESET                          = 0xCA11B04C,
    CALIB_CMD_BB_RESET                          = 0xCA11B04D
} CALIB_COMMAND_T;


// ============================================================================
// CALIB_PARAM_STATUS_T
// -----------------------------------------------------------------------------
/// Calibration parameter type identifier.
// =============================================================================
typedef enum
{
    CALIB_PARAM_DEFAULT                         = 0xCA11B042,
    CALIB_PARAM_DEFAULT_RF_MIS                  = 0xCA11B043,
    CALIB_PARAM_CALIBRATED                      = 0x00DEF011,
    CALIB_PARAM_INIT_ERROR                      = 0xCA10DEAD
} CALIB_PARAM_STATUS_T;


// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// CALIB_GLOBALS_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================



// ============================================================================
// CALIB_GPADC_ALL_CH_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef UINT16 CALIB_GPADC_ALL_CH_T[CALIB_GPADC_CH_QTY];


// ============================================================================
// CALIB_VERSION_TAG_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef UINT32 CALIB_VERSION_TAG_T;


// ============================================================================
// CALIB_OP_INFO_T
// -----------------------------------------------------------------------------
/// Calib process method and date type.
// =============================================================================
typedef struct
{
    UINT32                         date;                         //0x00000000
    CALIB_METHOD_T                 method;                       //0x00000004
} CALIB_OP_INFO_T; //Size : 0x8



// ============================================================================
// CALIB_XCV_TIMES_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef INT16 CALIB_XCV_TIMES_T[CALIB_XCV_TIME_QTY];


// ============================================================================
// CALIB_PA_TIMES_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef INT16 CALIB_PA_TIMES_T[CALIB_PA_TIME_QTY];


// ============================================================================
// CALIB_SW_TIMES_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef INT16 CALIB_SW_TIMES_T[CALIB_SW_TIME_QTY];


// ============================================================================
// CALIB_PAL_TIMES_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef INT16 CALIB_PAL_TIMES_T[CALIB_PAL_TIME_QTY];


// ============================================================================
// CALIB_XCV_PARAM_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef INT32 CALIB_XCV_PARAM_T[CALIB_XCV_PARAM_QTY];


// ============================================================================
// CALIB_PA_PARAM_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef INT32 CALIB_PA_PARAM_T[CALIB_PA_PARAM_QTY];


// ============================================================================
// CALIB_SW_PARAM_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef INT32 CALIB_SW_PARAM_T[CALIB_SW_PARAM_QTY];


// ============================================================================
// CALIB_RF_CHIP_NAME_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef UINT32 CALIB_RF_CHIP_NAME_T;


// ============================================================================
// CALIB_RXTX_FREQ_OFFSET_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef INT16 CALIB_RXTX_FREQ_OFFSET_T[CALIB_STUB_BAND_QTY];


// ============================================================================
// CALIB_RXTX_TIME_OFFSET_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef INT16 CALIB_RXTX_TIME_OFFSET_T;


// ============================================================================
// CALIB_RXTX_IQ_TIME_OFFSET_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef INT16 CALIB_RXTX_IQ_TIME_OFFSET_T;


// ============================================================================
// CALIB_DCO_CAL_TIME_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef INT16 CALIB_DCO_CAL_TIME_T;


// ============================================================================
// CALIB_XCV_PALCUST_T
// -----------------------------------------------------------------------------
/// XCV PAL custom types.
// =============================================================================
typedef struct
{
    CALIB_RF_CHIP_NAME_T           name;                         //0x00000000
    CALIB_RXTX_FREQ_OFFSET_T       rxTxFreqOffset;               //0x00000004
    CALIB_RXTX_TIME_OFFSET_T       rxTxTimeOffset;               //0x0000000C
    CALIB_RXTX_IQ_TIME_OFFSET_T    rxIqTimeOffset;               //0x0000000E
    CALIB_RXTX_IQ_TIME_OFFSET_T    txIqTimeOffset;               //0x00000010
    CALIB_DCO_CAL_TIME_T           dcoCalTime;                   //0x00000012
    INT32                          spare[7];                     //0x00000014
} CALIB_XCV_PALCUST_T; //Size : 0x30



// ============================================================================
// CALIB_ARFCN_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef INT16 CALIB_ARFCN_T[2];


// ============================================================================
// CALIB_PCL2DBM_ARFCN_G_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef CALIB_ARFCN_T CALIB_PCL2DBM_ARFCN_G_T[CALIB_GSM_PCL_QTY];


// ============================================================================
// CALIB_PCL2DBM_ARFCN_D_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef CALIB_ARFCN_T CALIB_PCL2DBM_ARFCN_D_T[CALIB_DCS_PCL_QTY];


// ============================================================================
// CALIB_PCL2DBM_ARFCN_P_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef CALIB_ARFCN_T CALIB_PCL2DBM_ARFCN_P_T[CALIB_PCS_PCL_QTY];


// ============================================================================
// CALIB_PADAC_PROFILE_INTERP_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef UINT16 CALIB_PADAC_PROFILE_INTERP_T[CALIB_PADAC_PROF_INTERP_QTY];


// ============================================================================
// CALIB_PADAC_PROFILE_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef UINT16 CALIB_PADAC_PROFILE_T[CALIB_PADAC_PROF_QTY];


// ============================================================================
// CALIB_PADAC_PROFILE_EXTREM_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef INT16 CALIB_PADAC_PROFILE_EXTREM_T;


// ============================================================================
// CALIB_PADAC_RAMP_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef UINT16 CALIB_PADAC_RAMP_T[CALIB_PADAC_RAMP_QTY];


// ============================================================================
// CALIB_PADAC_RAMP_SWAP_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef UINT16 CALIB_PADAC_RAMP_SWAP_T;


// ============================================================================
// CALIB_PADAC_LOW_VOLT_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef UINT16 CALIB_PADAC_LOW_VOLT_T[CALIB_LOW_VOLT_QTY];


// ============================================================================
// CALIB_PADAC_LOW_DAC_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef UINT16 CALIB_PADAC_LOW_DAC_T[CALIB_LOW_VOLT_QTY];


// ============================================================================
// CALIB_PA_PALCUST_T
// -----------------------------------------------------------------------------
/// PA PAL custom types.
// =============================================================================
typedef struct
{
    CALIB_RF_CHIP_NAME_T           name;                         //0x00000000
    CALIB_PCL2DBM_ARFCN_G_T        pcl2dbmArfcnG;                //0x00000004
    CALIB_PCL2DBM_ARFCN_D_T        pcl2dbmArfcnD;                //0x00000040
    CALIB_PCL2DBM_ARFCN_P_T        pcl2dbmArfcnP;                //0x00000084
    CALIB_PADAC_PROFILE_INTERP_T   profileInterpG;               //0x000000CC
    CALIB_PADAC_PROFILE_INTERP_T   profileInterpDp;              //0x000000EC
    CALIB_PADAC_PROFILE_T          profileG;                     //0x0000010C
    CALIB_PADAC_PROFILE_T          profileDp;                    //0x0000090C
    CALIB_PADAC_PROFILE_EXTREM_T   profileDbmMinG;               //0x0000110C
    CALIB_PADAC_PROFILE_EXTREM_T   profileDbmMinDp;              //0x0000110E
    CALIB_PADAC_PROFILE_EXTREM_T   profileDbmMaxG;               //0x00001110
    CALIB_PADAC_PROFILE_EXTREM_T   profileDbmMaxDp;              //0x00001112
    CALIB_PADAC_RAMP_T             rampLow;                      //0x00001114
    CALIB_PADAC_RAMP_T             rampHigh;                     //0x00001154
    CALIB_PADAC_RAMP_SWAP_T        rampSwapG;                    //0x00001194
    CALIB_PADAC_RAMP_SWAP_T        rampSwapDp;                   //0x00001196
    CALIB_PADAC_LOW_VOLT_T         lowVoltLimit;                 //0x00001198
    CALIB_PADAC_LOW_DAC_T          lowDacLimit;                  //0x000011A4
    INT32                          spare[8];                     //0x000011B0
} CALIB_PA_PALCUST_T; //Size : 0x11D0



// ============================================================================
// CALIB_SW_PALCUST_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef struct
{
    CALIB_RF_CHIP_NAME_T           name;                         //0x00000000
    INT32                          spare[8];                     //0x00000004
} CALIB_SW_PALCUST_T; //Size : 0x24



// ============================================================================
// CALIB_AUDIO_VOC_EC_T
// -----------------------------------------------------------------------------
/// Echo Cancelling for VoC.
// =============================================================================
typedef struct
{
    /// Enables Echo Canceller algorithm when >0.
    UINT8                          ecMu;                         //0x00000000
    /// Echo Canceller REL parameter (0,+16).
    UINT8                          ecRel;                        //0x00000001
    /// Echo Canceller MIN parameter (0, 32).
    UINT8                          ecMin;                        //0x00000002
    /// Enable Echo Suppressor algorithm (0,1).
    UINT8                          esOn;                         //0x00000003
    /// Double talk threshold for Echo Suppressor algo (0,32).
    UINT8                          esDtd;                        //0x00000004
    /// Donwlink vad threshold for Echo Suppressor algo (0,32).
    UINT8                          esVad;                        //0x00000005
    /// Enable for echo cancelling.
    UINT32                         enableField;                  //0x00000008
} CALIB_AUDIO_VOC_EC_T; //Size : 0xC



// ============================================================================
// CALIB_AUDIO_VOC_FILTERS_T
// -----------------------------------------------------------------------------
/// VoC anti-distortion filters.
// =============================================================================
typedef struct
{
    /// VoC needs the MDF coeffs to be 32-bit aligned.
    UINT16                         mdfFilter[CALIB_VOC_MDF_QTY]; //0x00000000
    /// SDF coeffs must stay right after MDF.
    UINT16                         sdfFilter[CALIB_VOC_SDF_QTY]; //0x00000080
} CALIB_AUDIO_VOC_FILTERS_T; //Size : 0x100



// ============================================================================
// CALIB_AUDIO_GAINS_T
// -----------------------------------------------------------------------------
/// Calib audio gain types.
// =============================================================================
typedef struct
{
    /// Params accessible by the API.
    UINT32                         inGain;                       //0x00000000
    UINT32                         outGain[CALIB_AUDIO_GAIN_QTY]; //0x00000004
    UINT32                         sideTone[CALIB_AUDIO_GAIN_QTY]; //0x00000024
    UINT32                         amplGain[CALIB_AUDIO_GAIN_QTY]; //0x00000044
} CALIB_AUDIO_GAINS_T; //Size : 0x64



// ============================================================================
// CALIB_AUDIO_PARAMS_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef struct
{
    UINT32                         MicDigitalGainInVoC;          //0x00000000
    INT8				     AecEnbleFlag;
    INT8				     PreFilterEnbleFlag;	
    INT8				     SoftLimiterEnbleFlag;
    INT8				     resevered;
    UINT32                         AudioParams[CALIB_AUDIO_PARAM_QTY-1-1]; //0x00000004
} CALIB_AUDIO_PARAMS_T; //Size : 0x20



// ============================================================================
// CALIB_AUDIO_ITF_T
// -----------------------------------------------------------------------------
/// Calibration of an audio interface. It gathers the audio gains and VoC calibrations
/// data
// =============================================================================
typedef struct
{
    /// Echo Cancelling for VoC.
    CALIB_AUDIO_VOC_EC_T           vocEc;                        //0x00000000
    /// VoC anti-distortion filters.
    CALIB_AUDIO_VOC_FILTERS_T      vocFilters;                   //0x0000000C
    /// Calib audio gain types.
    CALIB_AUDIO_GAINS_T            audioGains;                   //0x0000010C
    CALIB_AUDIO_PARAMS_T           audioParams;                  //0x00000170
} CALIB_AUDIO_ITF_T; //Size : 0x190



// ============================================================================
// CALIB_GPADC_T
// -----------------------------------------------------------------------------
/// Calib GPADC analog type.
// =============================================================================
typedef struct
{
    UINT8                          bandgap;                      //0x00000000
    UINT16                         sensorGainA;                  //0x00000002
    UINT16                         sensorGainB;                  //0x00000004
} CALIB_GPADC_T; //Size : 0x6



// ============================================================================
// CALIB_RF_ANALOG_T
// -----------------------------------------------------------------------------
/// Calib RF analog type.
// =============================================================================
typedef struct
{
    UINT8                          txRefVolt;                    //0x00000000
    UINT8                          txDacCurQ;                    //0x00000001
    UINT8                          txDacCurI;                    //0x00000002
    UINT8                          rxRefCal;                     //0x00000003
} CALIB_RF_ANALOG_T; //Size : 0x4



// ============================================================================
// CALIB_PMD_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef UINT32 CALIB_PMD_T[CALIB_PMD_QTY];


// ============================================================================
// CALIB_XCV_T
// -----------------------------------------------------------------------------
/// Transceiver calibration.
// =============================================================================
typedef struct
{
    CALIB_XCV_TIMES_T              times;                        //0x00000000
    CALIB_XCV_PARAM_T              param;                        //0x00000028
    /// XCV PAL custom types.
    CALIB_XCV_PALCUST_T            palcust;                      //0x00000078
} CALIB_XCV_T; //Size : 0xA8



// ============================================================================
// CALIB_PA_T
// -----------------------------------------------------------------------------
/// Power Amplifier RF calibration.
// =============================================================================
typedef struct
{
    CALIB_PA_TIMES_T               times;                        //0x00000000
    CALIB_PA_PARAM_T               param;                        //0x00000020
    /// PA PAL custom types.
    CALIB_PA_PALCUST_T             palcust;                      //0x0000005C
} CALIB_PA_T; //Size : 0x122C



// ============================================================================
// CALIB_SW_T
// -----------------------------------------------------------------------------
/// Switch calibration.
// =============================================================================
typedef struct
{
    CALIB_SW_TIMES_T               times;                        //0x00000000
    CALIB_SW_PARAM_T               param;                        //0x00000020
    CALIB_SW_PALCUST_T             palcust;                      //0x0000005C
} CALIB_SW_T; //Size : 0x80



// ============================================================================
// CALIB_BB_T
// -----------------------------------------------------------------------------
/// Baseband calibration.
// =============================================================================
typedef struct
{
    CALIB_PAL_TIMES_T              times;                        //0x00000000
    /// Audio calibration, for each interface
    CALIB_AUDIO_ITF_T              audio[CALIB_AUDIO_ITF_QTY];   //0x00000028
    /// Analog macros calibration: GPADC.
    CALIB_GPADC_T                  gpadc;                        //0x00000988
    /// Analog macros calibration: RF modules.
    CALIB_RF_ANALOG_T              rfAnalog;                     //0x0000098E
    CALIB_PMD_T                    pmd;                          //0x00000994
} CALIB_BB_T; //Size : 0x99C



// ============================================================================
// CALIB_BUFFER_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef struct
{
    /// Information about this calib buffer.
    CALIB_VERSION_TAG_T            versionTag;                   //0x00000000
    /// Calib process method and date type.
    CALIB_OP_INFO_T                opInfo;                       //0x00000004
    /// Transceiver calibration.
    CALIB_XCV_T                    xcv;                          //0x0000000C
    /// Power Amplifier RF calibration.
    CALIB_PA_T                     pa;                           //0x000000B4
    /// Switch calibration.
    CALIB_SW_T                     sw;                           //0x000012E0
    /// Baseband calibration.
    CALIB_BB_T                     bb;                           //0x00001360
} CALIB_BUFFER_T; //Size : 0x1CFC


#define VOLATILE                    volatile

// ============================================================================
// CALIB_STUB_CTX_T
// -----------------------------------------------------------------------------
/// Used to send calibration context change requests from the remote calibration
/// tools to the Calib Stub.
// =============================================================================
typedef struct
{
    /// Commands from the remote calibration tools.
    VOLATILE CALIB_STUB_CMDS_T     state;                        //0x00000000
    VOLATILE BOOL                  firstFint;                    //0x00000004
    /// Subcommands from the remote calibration tools.
    VOLATILE INT32                 setXtalFreqOffset;            //0x00000008
    VOLATILE UINT8                 setAfcBound;                  //0x0000000C
    VOLATILE INT32                 setAfcFreqOffset;             //0x00000010
    VOLATILE INT8                  setILossOffset;               //0x00000014
    VOLATILE INT32                 setPAProfMeas;                //0x00000018
    VOLATILE INT8                  setCalibUpdate;               //0x0000001C
    VOLATILE UINT8                 setRestartGpadcMeasure;       //0x0000001D
    /// Cells information
    VOLATILE UINT16                arfcn[CALIB_NB_CELLS];        //0x0000001E
    VOLATILE UINT8                 power[CALIB_NB_CELLS];        //0x00000028
    VOLATILE BOOL                  isPcs[CALIB_NB_CELLS];        //0x0000002D
    VOLATILE UINT8                 bsic;                         //0x00000032
    VOLATILE UINT32                fn;                           //0x00000034
    VOLATILE UINT8                 t2;                           //0x00000038
    VOLATILE UINT8                 t3;                           //0x00000039
    VOLATILE UINT16                qbOf;                         //0x0000003A
    VOLATILE UINT16                pFactor;                      //0x0000003C
    VOLATILE INT32                 tOf;                          //0x00000040
    VOLATILE INT32                 fOf;                          //0x00000044
    VOLATILE UINT16                snR;                          //0x00000048
    VOLATILE UINT8                 bitError;                     //0x0000004A
    VOLATILE UINT8                 monPower;                     //0x0000004B
    VOLATILE UINT8                 nbPower;                      //0x0000004C
    VOLATILE UINT8                 monBitmap;                    //0x0000004D
    VOLATILE INT32                 meanFOf;                      //0x00000050
    /// This is initialized by HST
    VOLATILE UINT8                 xtalCalibDone;                //0x00000054
    /// This is initialized by HST
    VOLATILE UINT16                paProfNextDacVal;             //0x00000056
    VOLATILE UINT8                 paProfCalibDone;              //0x00000058
    VOLATILE INT16                 dcoAverI;                     //0x0000005A
    VOLATILE INT16                 dcoAverQ;                     //0x0000005C
    /// Can go up to 2 * CT_CALIB_DCO_ACC_COUNT - 1.
    VOLATILE UINT8                 dcoAccCount;                  //0x0000005E
    VOLATILE INT16                 dcoI[CALIB_DCO_ACC_COUNT];    //0x00000060
    VOLATILE INT16                 dcoQ[CALIB_DCO_ACC_COUNT];    //0x000000A0
    /// Status of the iloss calibration porcess. This is initialized by HST.
    UINT8                          iLossCalibDone;               //0x000000E0
    /// ARFCN for which to measure the insertion loss. This is not initialized.
    UINT16                         iLossNextArfcn;               //0x000000E2
    UINT16                         gpadcAver[CALIB_GPADC_CH_QTY]; //0x000000E4
    /// Can go up to 2*CALIB_GPADC_ACC_COUNT-1.
    UINT8                          gpadcAccCount;                //0x000000EC
    CALIB_GPADC_ALL_CH_T           gpadcAcc[CALIB_GPADC_ACC_COUNT_MAX]; //0x000000EE
    /// Parameters for Tx commands from the remote calibration tools.
    VOLATILE UINT16                txArfcn;                      //0x000004EE
    VOLATILE UINT8                 txPcl;                        //0x000004F0
    VOLATILE UINT16                txDac;                        //0x000004F2
    VOLATILE UINT8                 txBand;                       //0x000004F4
    VOLATILE UINT8                 txTsc;                        //0x000004F5
    /// Parameters for monitoring commands from the remote calibration tools.
    VOLATILE UINT16                monArfcn;                     //0x000004F6
    VOLATILE UINT8                 monBand;                      //0x000004F8
    VOLATILE UINT8                 monExpPow;                    //0x000004F9
    /// Parameters for audio commands from the remote calibration tools. This parameter
    /// is used to select the Audio Interface to calibrate
    VOLATILE UINT8                 itfSel;                       //0x000004FA
    /// This field selects which input (microphone) is used on the Audio Interface
    /// defined by the itfSel field.
    VOLATILE UINT8                 inSel;                        //0x000004FB
    VOLATILE UINT8                 inGain;                       //0x000004FC
    VOLATILE UINT8                 inUart;                       //0x000004FD
    /// This field selects which input (speaker) is used on the Audio Interface defined
    /// by the itfSel field.
    VOLATILE UINT8                 outSel;                       //0x000004FE
    VOLATILE UINT8                 outGain;                      //0x000004FF
    VOLATILE UINT8                 polyGain;                     //0x00000500
    VOLATILE UINT8                 sideGain;                     //0x00000501
    VOLATILE UINT16                outFreq;                      //0x00000502
    VOLATILE UINT8                 outAmpl;                      //0x00000504
    VOLATILE UINT8                 startAudioDictaphone;         //0x00000505
    VOLATILE UINT8                 audioDictaphoneStatus;        //0x00000506
    /// Command to start the custom calibration of the PMD. This value is given to
    /// the pmd_CustomCalibration() function. This is initialized by the stub and
    /// written by the HST.
    VOLATILE UINT8                 pmdCustomCalibStart;          //0x00000507
    /// Status of the custom calibration of the PMD. This is initialized by the HST
    /// and returned by the stub.
    VOLATILE UINT32                pmdCustomCalibStatus;         //0x00000508
} CALIB_STUB_CTX_T; //Size : 0x50C



// ============================================================================
// CALIB_CALIBRATION_T
// -----------------------------------------------------------------------------
/// This struct will contain pointers to the calibration info and to the struct where
/// to put the calibration context requests. It also contains the address of the
/// calibration sector in flash.
// =============================================================================
typedef struct
{
    CALIB_VERSION_TAG_T            codeVersion;                  //0x00000000
    CALIB_PARAM_STATUS_T           paramStatus;                  //0x00000004
    CALIB_COMMAND_T                command;                      //0x00000008
    CALIB_OP_INFO_T*               opInfo;                       //0x0000000C
    CALIB_XCV_T*                   xcv;                          //0x00000010
    CALIB_PA_T*                    pa;                           //0x00000014
    CALIB_SW_T*                    sw;                           //0x00000018
    CALIB_BB_T*                    bb;                           //0x0000001C
    VOLATILE CALIB_OP_INFO_T*      hstOpInfo;                    //0x00000020
    VOLATILE CALIB_XCV_T*          hstXcv;                       //0x00000024
    VOLATILE CALIB_PA_T*           hstPa;                        //0x00000028
    VOLATILE CALIB_SW_T*           hstSw;                        //0x0000002C
    VOLATILE CALIB_BB_T*           hstBb;                        //0x00000030
    VOLATILE CALIB_STUB_CTX_T*     stubCtx;                      //0x00000034
    CONST CALIB_BUFFER_T*          flash;                        //0x00000038
} CALIB_CALIBRATION_T; //Size : 0x3C





//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------

#define CALIB_MAJ_MASK(x) ((x >> 8) & 0xFF)
#define CALIB_MIN_MASK(x) (x & 0xFF)
#define CALIB_MARK_MASK(x) (x & 0xFFFF0000)
#define CALIB_VERSION(maj, min) (CALIB_MARK_VERSION | maj << 8 | min << 0) 

//------------------------------------------------------------------------------
// Typedefs
//------------------------------------------------------------------------------

//=============================================================================
// calib_DaemonInit
//-----------------------------------------------------------------------------
/// At boot time, if the CALIB_TAG_CALIBRATED is in the validTag field of
/// the calibration flash sector, Calib uses the calibrated parameters
/// (i.e. the parameters present in the calib flash sector), else Calib
/// uses the default parameters.
/// @par
/// If the code has been compiled with the calibration mode flag set, at boot
/// time, this function also uses the remote parameters RAM buffer and copy 
/// the default parameters in it.
/// @par
/// This must be called in the PAL initialization, after the initialization
/// of the RF drivers.
//=============================================================================
PUBLIC VOID calib_DaemonInit(VOID);

//=============================================================================
// calib_DaemonProcess
//-----------------------------------------------------------------------------
/// Execute the calibration commands received by the remote calib tools. This
/// will, usually, update the calibration buffers with new values provided by
/// the user controlling the calib tools.
/// @par
/// This must be called in the PAL start frame.
//=============================================================================
PUBLIC VOID calib_DaemonProcess(VOID);

//=============================================================================
// calib_GetPointers
//-----------------------------------------------------------------------------
/// Access to the calibration parameters.
///
/// @return The global pointers used by the code to access the calibrated
/// parameters. When the target's flash does not contain a flash sector,
/// the calib parameters are initialized with default parameters during
/// the calibration init. Also returns the pointers used by the remote
/// calibration tools to get the version number of the calibration and to 
/// find the calibration buffers.
//=============================================================================
PUBLIC CALIB_CALIBRATION_T* calib_GetPointers(VOID);



//=============================================================================
// calib_SetStubContext
//-----------------------------------------------------------------------------
/// Used by the Calib Stub to define the calibration context structure that
/// the remote calibration tools must use to communicate with the Calib Stub.
///
/// @param ctx Pointer to the context where the remote calib tools must put
/// its calibration command to be executed by the Calib Stub.
//=============================================================================
PUBLIC VOID calib_SetStubContext(CONST CALIB_STUB_CTX_T *ctx);



//=============================================================================
// calib_SetXcvDefaults
//-----------------------------------------------------------------------------
/// Used by the transceiver RF driver to set its default calibration parameters
/// (see #calib_DaemonInit to know when they can be used). It must be called 
/// during the RF transceiver driver initialization.
///
/// @param param Pointer to a structure containing the default values to be
/// used by this transceiver model.
//=============================================================================
PUBLIC VOID calib_SetXcvDefaults(CONST CALIB_XCV_T* param);



//=============================================================================
// calib_SetPaDefaults
//-----------------------------------------------------------------------------
/// Used by the PA RF driver to set its default calibration parameters
/// (see #calib_DaemonInit to know when they can be used). It must be called 
/// during the RF PA driver initialization.
///
/// @param param Pointer to a structure containing the default values to be
/// used for this PA model.
//=============================================================================
PUBLIC VOID calib_SetPaDefaults(CONST CALIB_PA_T* param);



//=============================================================================
// calib_SetSwDefaults
//-----------------------------------------------------------------------------
/// Used by the switch RF driver to set the default calibration parameters
/// (see #calib_DaemonInit to know when they can be used). It must be called 
/// during the RF switch driver initialization.
///
/// @param param Pointer to a structure containing the default values to be
/// used for this switch model.
//=============================================================================
PUBLIC VOID calib_SetSwDefaults(CONST CALIB_SW_T* param);



//=============================================================================
// calib_DaemonTracePrint
//-----------------------------------------------------------------------------
/// Display in the trace the current calibration status, used for debug.
//=============================================================================
PUBLIC VOID calib_DaemonTracePrint(VOID);



// =============================================================================
// calib_DaemonSetContext
// -----------------------------------------------------------------------------
/// Defines the context structure that HST and CES must use.
/// @param ctx Pointer to the context to use.
// =============================================================================
PUBLIC VOID calib_DaemonSetContext(VOID* ctx);


/// @} // end of calib group



#endif

