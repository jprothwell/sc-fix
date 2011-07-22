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
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/edrv/trunk/gsensor/mma7660fc_gpio/include/mma7660fc.h $ //
//	$Author: huazeng $                                                        // 
//	$Date: 2010-08-23 13:47:34 +0800 (Mon, 23 Aug 2010) $                     //   
//	$Revision: 1532 $                                                        //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//       
/// @file mma7660fc.h
/// That file describes the MMA7660FC chip register.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _MMAA7660FC_M_H_
#define _MMAA7660FC_M_H_



// =============================================================================
// GSS_INTERRUPT_T corresponding the int register
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef union
{
    struct {
        BOOL  FBint:1;  // Front/Back position change  causes an interrupt
        BOOL  PLint:1;  // UP/Down/Right/Left position change causes an interrupt
        BOOL  PDint:1;  // Successful tap detection causes an interrupt
        BOOL  ASint:1;  // ExitAtuo_sleep causes an interrupt
        BOOL  Gint:1;   // There is an automatic interrupt after every measurement.
        BOOL  SHintZ:1; // Shake on the X-axis causes an interrupt
        BOOL  SHintY:1; // Shake on the Y-axis causes an interrupt 
        BOOL  SHintX:1; // Shake on the Z-axis causes an interrupt
            };
    UINT8 intsu_reg;
} MMA7660FC_INTSU_CFG_T;




// ============================================================================
// MMA7660FC_REG_MAP_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef enum
{
    MMA7660FC_ADDR_XOUT                         = 0x00,
    MMA7660FC_ADDR_YOUT                         = 0x01,
    MMA7660FC_ADDR_ZOUT                         = 0x02,
    MMA7660FC_ADDR_TILT                         = 0x03,
    MMA7660FC_ADDR_SRST                         = 0x04,
    MMA7660FC_ADDR_SPCNT                        = 0x05,
    MMA7660FC_ADDR_INTSU                        = 0x06,
    MMA7660FC_ADDR_MODE                         = 0x07,
    MMA7660FC_ADDR_SR                           = 0x08,
    MMA7660FC_ADDR_PDET                         = 0x09,
    MMA7660FC_ADDR_PD                           = 0x0a,
    MMA7660FC_ADDR_FACTORY                      = 0x0b,
    MMA7660FC_ADDR_QTY                          = 0x1f
} MMA7760FC_REG_MAP_T;


//Xout
#define MMA7660FC_XOUT(n)           (((n)&0x3f)<<0)
#define MMA7660FC_XOUT_MASK         (0x3f)
#define MMA7660FC_XOUT_ALERT        (1<<6)
//Yout
#define MMA7660FC_YOUT(n)           (((n)&0x3f)<<0)
#define MMA7660FC_YOUT_MASK         (0x3f)
#define MMA7660FC_YOUT_ALERT        (1<<6)

//Zout
#define MMA7660FC_ZOUT(n)           (((n)&0x3f)<<0)
#define MMA7660FC_ZOUT_MASK         (0x3f)
#define MMA7660FC_ZOUT_ALERT        (1<<6)

//TILT Tilt Status
#define MMA7660FC_BA_FRO(n)         (((n)&3)<<0)
#define MMA7660FC_BA_FRO_MASK       (3<<0)
#define MMA7660FC_BA_FRO_SHIFT      (0)

#define MMA7660FC_BA_FRO_UNKNOW       (0x0<<0)
#define MMA7660FC_BA_FRO_FRONT        (0x1<<0)
#define MMA7660FC_BA_FRO_BACK         (0x2<<0)

#define MMA7660FC_PO_LA(n)         (((n)&7)<<2)
#define MMA7660FC_PO_LA_MASK       (7<<2)
#define MMA7660FC_PO_LA_SHIFT      (2)


#define MMA7660FC_PO_LA_MASK        (7<<2)
#define MMA7660FC_PO_LA_UNKNOW      (0x0<<2)
#define MMA7660FC_PO_LA_LEFT        (0x1<<2)
#define MMA7660FC_PO_LA_RIGHT       (0x2<<2)
#define MMA7660FC_PO_LA_DWON        (0x5<<2)
#define MMA7660FC_PO_LA_UP          (0x6<<2)

#define MMA7660FC_TAP               (1<<5)
#define MMA7660FC_TAP_HAPPEN        (0x0<<5)
#define MMA7660FC_TAP_NO_HAPPEN     (0x1<<5)

#define MMA7660FC_ALERT             (1<<6)
#define MMA7660FC_SHAKE             (1<<7)
#define MMA7660FC_SHAKE_HAPPEN      (1<<7)
#define MMA7660FC_SHAKE_NO_HAPPEN   (0<<7)
#define MMA7660FC_SHAKE_MASK        (1<<7)


//SRST Sampling Rate Status
#define MMA7660FC_AMSRS             (1<<0)
#define MMA7660FC_AWSRS             (1<<1)

// SPCNT Sleep Count
#define MMA7660FC_SC(n)             ((n)&0xff)

// INTSU interrupt Setup
#define MMA7660FC_FBINT             (1<<0)
#define MMA7660FC_PLINT             (1<<1)
#define MMA7660FC_PDINT             (1<<2)
#define MMA7660FC_ASINT             (1<<3)
#define MMA7660FC_GINT              (1<<4)
#define MMA7660FC_SHINTZ             (1<<5)
#define MMA7660FC_SHINTY             (1<<6)
#define MMA7660FC_SHINTX             (1<<7)

//MODE 
#define MMA7660FC_MODE             (1<<0)
#define MMA7660FC_TON             (1<<2)
#define MMA7660FC_AWE             (1<<3)
#define MMA7660FC_ASE             (1<<4)
#define MMA7660FC_SCPS             (1<<5)
#define MMA7660FC_IPP             (1<<6)
#define MMA7660FC_IAH             (1<<7)

//SR auto-wakeup/sleep and portrait/landscape samples per seconds and debounce filter
#define MMA7660FC_AMSR(n)         (((n)&7)<<0)
#define MMA7660FC_AMSR_MASK       (7<<0)
#define MMA7660FC_AMSR_SHIFT      (0)




#define MMA7660FC_AWSR(n)         (((n)&3)<<3)
#define MMA7660FC_AWSR_MASK       (3<<3)
#define MMA7660FC_AWSR_SHIFT      (3)



#define MMA7660FC_FILT(n)         (((n)&7)<<5)
#define MMA7660FC_FILT_MASK       (7<<5)
#define MMA7660FC_FILT_SHIFT      (5)


//PDET tap detection
#define MMA7660FC_PDTH(n)         (((n)&0x1f)<<0)
#define MMA7660FC_PDTH_MASK       (0x1f<<0)
#define MMA7660FC_PDTH_SHIFT       (0)
#define MMA7660FC_XDA             (1<<5)
#define MMA7660FC_YDA             (1<<6)
#define MMA7660FC_ZDA             (1<<7)

//PD tap debounce count
#define MMA7660FC_PD             (0x1f<<0)


typedef struct
{
  INT8 x_adc; //X_OUT
  INT8 y_adc; //Y_OUT
  INT8 z_adc; //Z_OUT
  UINT8 tilt; //tilt status
  UINT8 srsr; // sampling rate status
  UINT8 spcnt; //sleep count
  UINT8 intsu; //interupt setup
  UINT8 mode; // MODe
  UINT8 sr; // Auto-wake/sleep and P/L samles per serconds and debounce
  UINT8 pdet; // tap detection
  UINT8 pd; // tap debounce count
}GSS_ACC_DATE_T;





#endif








