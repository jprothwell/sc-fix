#ifndef _CAMERA_ASM_H_
#define _CAMERA_ASM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !

#ifndef CT_ASM
#error "You are trying to use in a normal C code the assembly H description of 'camera'."
#endif



//==============================================================================
// camera
//------------------------------------------------------------------------------
/// 
//==============================================================================
#define REG_CAMERA_BASE             0x00B99000

#define REG_CAMERA_BASE_HI          BASE_HI(REG_CAMERA_BASE)
#define REG_CAMERA_BASE_LO          BASE_LO(REG_CAMERA_BASE)

#define REG_CAMERA_APBI_CTRL_CAMERA REG_CAMERA_BASE_LO + 0x00000000
#define REG_CAMERA_APBI_FIFO_RX     REG_CAMERA_BASE_LO + 0x00000008
#define REG_CAMERA_INTERRUPT_CLEAR_REGISTER REG_CAMERA_BASE_LO + 0x00000800
#define REG_CAMERA_INTERRUPT_STATUS_REGISTER REG_CAMERA_BASE_LO + 0x00000804
#define REG_CAMERA_INTERRUPT_CAUSE_REGISTER REG_CAMERA_BASE_LO + 0x00000808
#define REG_CAMERA_INTERRUPT_MASK_REGISTER REG_CAMERA_BASE_LO + 0x0000080C
#define REG_CAMERA_CAMERA_CONTROL_REGISTER REG_CAMERA_BASE_LO + 0x00000810

//apbi_ctrl_camera
#define CAMERA_L_ENDIAN(n)          (((n)&3)<<0)
#define CAMERA_SOFT_RST_L           (1<<2)

//APBI_FIFO_Rx
#define CAMERA_DATA_OUT(n)          (((n)&0xFFFFFFFF)<<0)

//Interrupt_CLEAR_register
#define CAMERA_OVFL                 (1<<0)
#define CAMERA_FSTART               (1<<1)
#define CAMERA_FEND                 (1<<2)
#define CAMERA_VSYNC                (1<<3)

//Interrupt_STATUS_register
//#define CAMERA_OVFL               (1<<0)
//#define CAMERA_FSTART             (1<<1)
//#define CAMERA_FEND               (1<<2)
//#define CAMERA_VSYNC              (1<<3)

//Interrupt_CAUSE_register
//#define CAMERA_OVFL               (1<<0)
//#define CAMERA_FSTART             (1<<1)
//#define CAMERA_FEND               (1<<2)
//#define CAMERA_VSYNC              (1<<3)

//Interrupt_MASK_register
//#define CAMERA_OVFL               (1<<0)
//#define CAMERA_FSTART             (1<<1)
//#define CAMERA_FEND               (1<<2)
//#define CAMERA_VSYNC              (1<<3)

//Camera_CONTROL_register
#define CAMERA_MODE                 (1<<0)
#define CAMERA_PWDON                (1<<1)
#define CAMERA_RESET                (1<<2)
#define CAMERA_ENCAMERA             (1<<3)
#define CAMERA_TEST                 (1<<4)




#endif
