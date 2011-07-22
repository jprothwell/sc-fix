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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/aud/maxim/src/audp_maxim.h $ //
//	$Author: huazeng $                                                        // 
//	$Date: 2010-08-23 13:47:34 +0800 (Mon, 23 Aug 2010) $                     //   
//	$Revision: 1532 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file audp_maxim.h                                                        //
/// That file is the private header for the maxime implementation of the AUD  //
/// interface.                                                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////






// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================



// =============================================================================
//  MACROS
// =============================================================================
/// Address on the I2C bus.
#define MAXIM_ADDR 0x10

/// Maxim's register Map
#define REG_MAXIM_STATUS_0              0x00
#define REG_MAXIM_STATUS_1              0x01
#define REG_MAXIM_INT_ENABLE            0x02
    #define INT_ICLD    (1<<7)
    #define INT_ISLD    (1<<6)
    #define INT_IULK    (1<<5)
    #define INT_IHSD    (1<<1)
    #define INT_IFAULT  (1<<0)
    
#define REG_MAXIM_S1_IF_MODE_0          0x03
    #define SERIAL_DATA_OUTPUT_ENABLE   (1<<7)
    #define SERIAL_DATA_INPUT_ENABLE    (1<<6)
    #define SERIAL_INPUT_MONO_MIX_ENABLE (1<<5)
    #define MODE_VOICE                  (0xa<<0)
    #define STEREO_8K                   (0x1<<0)
    #define STEREO_11K                  (0x2<<0)
    #define STEREO_12K                  (0x3<<0)
    #define STEREO_16K                  (0x4<<0)
    #define STEREO_22K                  (0x5<<0)
    #define STEREO_24K                  (0x6<<0)
    #define STEREO_32K                  (0x7<<0)
    #define STEREO_44K                  (0x8<<0)
    #define STEREO_48K                  (0x9<<0)


#define REG_MAXIM_S1_IF_MODE_1          0x04
    #define S1DLY       (1<<3)
    #define S1MAS       (1<<7)

#define REG_MAXIM_S2_IF_MODE_0          0x05
#define REG_MAXIM_S2_IF_MODE_1          0x06
#define REG_MAXIM_DIGITAL_FILTERS       0x07
    #define ADC_BAND_PASS_FILTER_ENABLE (1<<5)
    #define DAC_BAND_PASS_FILTER_ENABLE (1<<4)

#define REG_MAXIM_DIGITAL_MIXERS        0x08
    #define S1_LEFT_LEFT_DAC    (2<<4)
    #define S1_LEFT_RIGHT_DAC   (2<<0)
    #define S1_RIGHT_RIGHT_DAC  (1<<0)
    
#define REG_MAXIM_TDAC_MUX              0x09

#define REG_MAXIM_ADC_IN_MIX            0x0a
    #define RIGHT_MIC_LEFT_ADC  (1<<4)
    #define RIGHT_MIC_RIGHT_ADC (1<<0)
    #define LEFT_MIC_LEFT_ADC   (2<<4)

#define REG_MAXIM_AUDIO_OUTPUT_MIX      0x0b
    #define LEFT_DAC_O_LEFT_AU_O    (1<<4)
    #define RIGHT_DAC_O_RIGHT_AU_O  (1<<0)

#define REG_MAXIM_S1_GAIN               0x0c
#define REG_MAXIM_S2_GAIN               0x0d
#define REG_MAXIM_L1_IN_GAIN            0x0e
#define REG_MAXIM_L2_IN_GAIN            0x0f

#define REG_MAXIM_MIC_L_IN_GAIN         0x10
    #define MIC_L_PA                (1<<5)

#define REG_MAXIM_MIC_R_IN_GAIN         0x11
    #define MIC_R_PA                (1<<5)

#define REG_MAXIM_MIC_MODE              0x12
    #define EXT_MIC_INPUT           (1<<2)
    #define RESISTOR_BLAS_ENABLE    (1<<1)
    #define MIC_MUTE                (1<<3)

#define REG_MAXIM_SIDE_TONE_MODE        0x13

#define REG_MAXIM_H_R_LEFT_VOL          0x14
    #define H_R_VOL_m20DB       (0x14<<0)
    #define H_R_MUTE            (0x1<<6)

#define REG_MAXIM_H_R_RIGHT_VOL         0x15
//    #define H_R_VOL_m20DB       (0x14<<0)


#define REG_MAXIM_SPK_L_VOL             0x16
#define REG_MAXIM_SPK_R_VOL             0x17

#define REG_MAXIM_AUDIO_OUT_MODE        0x18
    #define VOLUME_SMOOTHING        (1<<6)
    #define ZERO_CROSS_DETECT       (1<<5)
    #define STEREO_HEADPHONE_MODE   (4<<0)


#define REG_MAXIM_TDAC_DETECT           0x19
#define REG_MAXIM_SYSTEM                0x1a
    #define CHARGE_PUMP_SELECT      ((1<<0) | (1<<2))
    #define CHARGE_PUMP_EN          ((1<<4) | (1<<2))
    #define LOW_FREQ_OSC_EN         ((1<<5) | (1<<2))
    #define CLOCK_IN_EN             ((1<<6) | (1<<2))
    #define START_H_STOP_L          ((1<<7) | (1<<2))

#define REG_MAXIM_SHUTDOWN              0x1b
    #define DAC_LEFT_EN             (1<<7)
    #define DAC_RIGHT_EN            (1<<6)
    #define ADC_LEFT_EN             (1<<5)
    #define ADC_RIGHT_EN            (1<<4)
    #define MIC_LEFT_EN             (1<<1)
    #define MIC_RIGHT_EN            (1<<0)

/// Configuration delay, to wait before the maxim chip is usable
#define AUD_MAXIM_CFG_DELAY         0x400



// =============================================================================
//  TYPES
// =============================================================================


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

//  

// =============================================================================
//  FUNCTIONS
// =============================================================================

