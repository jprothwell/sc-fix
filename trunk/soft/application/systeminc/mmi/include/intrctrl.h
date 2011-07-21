/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   intrCtrl.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Common type and structure definition for COOLSAND GSM/GPRS software
 *
 * Author:
 * -------
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#ifndef _INTRCTRL_H
#define _INTRCTRL_H

/*******************************************************************************
 * Include header files
 *******************************************************************************/
#include "kal_release.h"

/*******************************************************************************
 * CIRQ Registers *
 *******************************************************************************/
#if defined(MT6228) || defined(MT6229)

   #define IRQ_SEL0              ((volatile UINT32P)(CIRQ_base+0x0000)) /* Source selection 0 to 4 */
   #define IRQ_SEL1              ((volatile UINT32P)(CIRQ_base+0x0004)) /* Source selection 5 to 9 */
   #define IRQ_SEL2              ((volatile UINT32P)(CIRQ_base+0x0008)) /* Source selection 10 to 14 */
   #define IRQ_SEL3              ((volatile UINT32P)(CIRQ_base+0x000C)) /* Source selection 15 to 19 */
   #define IRQ_SEL4              ((volatile UINT32P)(CIRQ_base+0x0010)) /* Source selection 20 to 24 */
   #define IRQ_SEL5              ((volatile UINT32P)(CIRQ_base+0x0014)) /* Source selection 25 to 29 */
   #define IRQ_SEL6              ((volatile UINT32P)(CIRQ_base+0x0018)) /* Source selection 30 to 34 */
   #define IRQ_SEL7              ((volatile UINT32P)(CIRQ_base+0x001c)) /* Source selection 35 to 39 */
   #define IRQ_SEL8              ((volatile UINT32P)(CIRQ_base+0x0020)) /* Source selection 40 to 45 */
   
   #define FIQ_SEL               ((volatile UINT32P)(CIRQ_base+0x0034)) /* FIQ Source selection */
                                 
   #define IRQ_MASKL             ((volatile UINT32P)(CIRQ_base+0x0038)) /* IRQ Mask 0 to 31 */
   #define IRQ_MASKH             ((volatile UINT32P)(CIRQ_base+0x003c)) /* IRQ Mask 32 to 38*/
                                 
   #define IRQ_MASK_CLRL         ((volatile UINT32P)(CIRQ_base+0x0040)) /* IRQ Mask Clear Register (LSB) */
   #define IRQ_MASK_CLRH         ((volatile UINT32P)(CIRQ_base+0x0044)) /* IRQ Mask Clear Register (MSB) */
                                 
   #define IRQ_MASK_SETL         ((volatile UINT32P)(CIRQ_base+0x0048)) /* IRQ Mask Set Register (LSB) */
   #define IRQ_MASK_SETH         ((volatile UINT32P)(CIRQ_base+0x004c)) /* IRQ Mask Set Register (MSB) */
                                 
   #define IRQ_STAL              ((volatile UINT32P)(CIRQ_base+0x0050)) /* IRQ Source Status Register (LSB) */
   #define IRQ_STAH              ((volatile UINT32P)(CIRQ_base+0x0054)) /* IRQ Source Status Register (MSB) */
                                 
   #define IRQ_EOIL              ((volatile UINT32P)(CIRQ_base+0x0058)) /* End of Interrupt Register (LSB) */
   #define IRQ_EOIH              ((volatile UINT32P)(CIRQ_base+0x005c)) /* End of Interrupt Register (MSB) */
                                 
   #define IRQ_SENSL             ((volatile UINT32P)(CIRQ_base+0x0060)) /* IRQ Sensitivity Register (LSB) */
   #define IRQ_SENSH             ((volatile UINT32P)(CIRQ_base+0x0064)) /* IRQ Sensitivity Register (MSB) */
                                 
   #define IRQ_SOFTL             ((volatile UINT32P)(CIRQ_base+0x0068)) /* Software Interrupt Register (LSB) */
   #define IRQ_SOFTH             ((volatile UINT32P)(CIRQ_base+0x006c)) /* Software Interrupt Register (MSB) */
                                 
   #define FIQ_CON               ((volatile UINT32P)(CIRQ_base+0x0070)) /* FIQ Control Register */
   #define FIQ_EOI               ((volatile UINT32P)(CIRQ_base+0x0074)) /* FIQ EOI Register */
                                 
   #define IRQ_STS2              ((volatile UINT32P)(CIRQ_base+0x0078)) /* Binary Code Value of IRQ_STATUS */
   #define IRQ_EOI2              ((volatile UINT32P)(CIRQ_base+0x007c)) /* Binary Code Value of IRQ_EOI */
   #define IRQ_SOFT2             ((volatile UINT32P)(CIRQ_base+0x0080)) /* Binary Code Value of IRQ_SOFT  */
                                 
                                 
   #define EINT_STATUS           ((volatile UINT32P)(CIRQ_base+0x0100)) /*  external interrupt control */   
   #define EINT_MASK             ((volatile UINT32P)(CIRQ_base+0x0104)) /*  external interrupt control */
   #define EINT_MASK_CLR         ((volatile UINT32P)(CIRQ_base+0x0108)) /*  external interrupt control */
   #define EINT_MASK_SET         ((volatile UINT32P)(CIRQ_base+0x010C)) /*  external interrupt control */         
   #define EINT_INTACK           ((volatile UINT32P)(CIRQ_base+0x0110)) /*  external interrupt control */
   #define EINT_SENS             ((volatile UINT32P)(CIRQ_base+0x0114)) /*  external interrupt control */   
   #define EINT0_CON             ((volatile UINT32P)(CIRQ_base+0x0120)) /*  external interrupt control */
   #define EINT1_CON             ((volatile UINT32P)(CIRQ_base+0x0130)) /*  external interrupt control */
   #define EINT2_CON             ((volatile UINT32P)(CIRQ_base+0x0140)) /*  external interrupt control */
   #define EINT3_CON             ((volatile UINT32P)(CIRQ_base+0x0150)) /*  external interrupt control */
   #define EINT4_CON             ((volatile UINT32P)(CIRQ_base+0x0160)) /*  external interrupt control */
   #define EINT5_CON             ((volatile UINT32P)(CIRQ_base+0x0170)) /*  external interrupt control */         
   #define EINT6_CON             ((volatile UINT32P)(CIRQ_base+0x0180)) /*  external interrupt control */         
   #define EINT7_CON             ((volatile UINT32P)(CIRQ_base+0x0190)) /*  external interrupt control */         

   /*
    * CIRQ Interrupt Sources
    */
   #define IRQ_GPI_CODE          (0)   /* General Purpose IO */   
   #define IRQ_CTIRQ1_CODE       (1)   /* TDMA Sequencer Unit */
   #define IRQ_CTIRQ2_CODE       (2)   /* TDMA Sequencer Unit */
   #define IRQ_DSP2CPU_CODE      (3)   /* DSP Sub-system */   
   #define IRQ_SIM_CODE          (4)   /* SIM Interrupt */
   #define IRQ_DMA_CODE          (5)   /* DMA Peripheral */
   #define IRQ_TDMA_CODE         (6)   /* wakeup unit Peripheral */            
   #define IRQ_UART1_CODE        (7)   /* UART1 Peripheral */
   #define IRQ_KPAD_CODE         (8)   /* Keypad Scanner Peripheral */
   #define IRQ_UART2_CODE        (9)   /* UART2 Peripheral */
   #define IRQ_GPT_CODE          (10)  /* General Purpose Time Int  */   
   #define IRQ_EIT_CODE          (11)  /* external Interrupt */
   #define IRQ_USB_CODE          (12)  /* USB Peripheral */
   #define IRQ_MSDC_CODE         (13)  /* Mass Storage Unit */                  
   #define IRQ_RTC_CODE          (14)  /* RTC Peripheral */
   #define IRQ_IrDA_CODE         (15)  /* IrDA Unit */   
   #define IRQ_LCD_CODE          (16)  /* LCD Peripheral */
   #define IRQ_UART3_CODE        (17)  /* UART3 Peripheral */   
   #define IRQ_GPII_CODE         (18)  /* GPI II */   
   #define IRQ_WDT_CODE          (19)  /* Watchdog */   
   #define IRQ_SWDBG_CODE        (20)  /* Software Debug Interface */
   #define IRQ_CHE_CODE          (21)  /* CHE */
   #define IRQ_NFI_CODE          (22)  /* Nandflash */
   #define IRQ_B2PSI_CODE        (23)  /* SPI */
   #define IRQ_IMGDMA_CODE       (24)  /* Image DMA */
   #define IRQ_GIF_CODE          (25)  /* GIF */
   #define IRQ_PNG_CODE          (26)  /* reserved */
   #define IRQ_SCCB_CODE         (27)  /* SCCB */
   #define IRQ_G2D_CODE          (28)  /* Graphic 2D engine */
   #define IRQ_IMGPROC_CODE      (29)  /* image processing */
   #define IRQ_CAMERA_CODE       (30)  /* camera interface */
   #define IRQ_PFC_CODE          (31)  /* PFC */
   #define IRQ_MPEG4_DEC_CODE    (32)  /* MPEG4 Decoder */
   #define IRQ_MPEG4_ENC_CODE    (33)  /* MPEG4 Encoder */
   #define IRQ_JPEG_DEC_CODE     (34)  /* JPEG Decoder */
   #define IRQ_JPEG_ENC_CODE     (35)  /* JPEG Encoder */
   #define IRQ_RESIZER_CRZ_CODE  (36)  /* Resizer crz */
   #define IRQ_RESIZER_DRZ_CODE  (37)  /* Resizer drz */
   #define IRQ_RESIZER_PRZ_CODE  (38)  /* Resizer prz */
   #define IRQ_TVOUT_CODE        (39)  /* TVOUT */

#if defined(MT6229)

	#define IRQ_DSP22CPU_CODE     (40)  /* DPS2 2 CPU */
   #define NUM_IRQ_SOURCES       (41)
   
#else

   #define NUM_IRQ_SOURCES       (40)
   
#endif   

   /*
    * IRQ Selection Register mapping
    */
  
   #define IRQ0_CODE             IRQ_CTIRQ1_CODE        /* IRQ_CTIRQ1_CODE */
   #define IRQ1_CODE             IRQ_CTIRQ2_CODE        /* IRQ_CTIRQ2_CODE */
   #define IRQ2_CODE             IRQ_DSP2CPU_CODE       /* IRQ_DSP2CPU_CODE */
   #define IRQ3_CODE             IRQ_SIM_CODE           /* IRQ_SIM_CODE */
   #define IRQ4_CODE             IRQ_DMA_CODE           /* IRQ_DMA_CODE */
   #define IRQ5_CODE             IRQ_TDMA_CODE          /* IRQ_TDMA_CODE */
   #define IRQ6_CODE             IRQ_UART1_CODE         /* IRQ_UART1_CODE */
   #define IRQ7_CODE             IRQ_KPAD_CODE          /* IRQ_KPAD_CODE */
   #define IRQ8_CODE             IRQ_UART2_CODE         /* IRQ_UART2_CODE */
   #define IRQ9_CODE             IRQ_GPT_CODE           /* IRQ_GPT_CODE */
   #define IRQA_CODE             IRQ_EIT_CODE           /* IRQ_EIT_CODE */
   #define IRQB_CODE             IRQ_USB_CODE           /* IRQ_USB_CODE */
   #define IRQC_CODE             IRQ_MSDC_CODE          /* IRQ_MSDC_CODE */
   #define IRQD_CODE             IRQ_RTC_CODE           /* IRQ_RTC_CODE */
   #define IRQE_CODE             IRQ_IrDA_CODE          /* IRQ_IrDA_CODE */
   #define IRQF_CODE             IRQ_LCD_CODE           /* IRQ_LCD_CODE */
   #define IRQ10_CODE            IRQ_UART3_CODE         /* IRQ_UART3_CODE */
   #define IRQ11_CODE            IRQ_GPI_CODE           /* IRQ_GPI_CODE */
   #define IRQ12_CODE            IRQ_GPII_CODE          /* GPI II */
   #define IRQ13_CODE            IRQ_WDT_CODE           /* Watchdog */
   #define IRQ14_CODE            IRQ_SWDBG_CODE         /* Software Debug Interface */
   #define IRQ15_CODE            IRQ_CHE_CODE           /* CHE */
   #define IRQ16_CODE            IRQ_NFI_CODE           /* Nandflash */
   #define IRQ17_CODE            IRQ_B2PSI_CODE         /* SPI */
   #define IRQ18_CODE            IRQ_IMGDMA_CODE        /* Image DMA */
   #define IRQ19_CODE            IRQ_GIF_CODE           /* GIF */
   #define IRQ1A_CODE            IRQ_PNG_CODE           /* PNG */
   #define IRQ1B_CODE            IRQ_SCCB_CODE          /* SCCB */
   #define IRQ1C_CODE            IRQ_G2D_CODE           /* Graphic 2D engine */
   #define IRQ1D_CODE            IRQ_IMGPROC_CODE       /* image processing */
   #define IRQ1E_CODE            IRQ_CAMERA_CODE        /* camera interface */
   #define IRQ1F_CODE            IRQ_PFC_CODE           /* PFC */
   #define IRQ20_CODE            IRQ_MPEG4_DEC_CODE     /* MPEG4 Encoder */
   #define IRQ21_CODE            IRQ_MPEG4_ENC_CODE     /* MPEG4 Encoder */
   #define IRQ22_CODE            IRQ_JPEG_DEC_CODE      /* JPEG Decoder */
   #define IRQ23_CODE            IRQ_JPEG_ENC_CODE      /* JPEG Encoder */
   #define IRQ24_CODE            IRQ_RESIZER_CRZ_CODE   /* Resizer crz */
   #define IRQ25_CODE            IRQ_RESIZER_DRZ_CODE   /* Resizer drz */
   #define IRQ26_CODE            IRQ_RESIZER_PRZ_CODE   /* Resizer prz */
   #define IRQ27_CODE            IRQ_TVOUT_CODE         /* Resizer prz */

   /*
    * IRQ code value in Selection Register
    */
   #define IRQ0_SEL0             (IRQ0_CODE <<  0)
   #define IRQ1_SEL0             (IRQ1_CODE <<  6)
   #define IRQ2_SEL0             (IRQ2_CODE <<  12)
   #define IRQ3_SEL0             (IRQ3_CODE <<  18)
   #define IRQ4_SEL0             (IRQ4_CODE <<  24)
                                 
   #define IRQ5_SEL1             (IRQ5_CODE <<  0)
   #define IRQ6_SEL1             (IRQ6_CODE <<  6)
   #define IRQ7_SEL1             (IRQ7_CODE <<  12)
   #define IRQ8_SEL1             (IRQ8_CODE <<  18)
   #define IRQ9_SEL1             (IRQ9_CODE <<  24)
                                 
   #define IRQA_SEL2             (IRQA_CODE <<  0)
   #define IRQB_SEL2             (IRQB_CODE <<  6)
   #define IRQC_SEL2             (IRQC_CODE <<  12)
   #define IRQD_SEL2             (IRQD_CODE <<  18)
   #define IRQE_SEL2             (IRQE_CODE <<  24)
                                 
   #define IRQF_SEL3             (IRQF_CODE <<  0)
   #define IRQ10_SEL3            (IRQ10_CODE <<  6)
   #define IRQ11_SEL3            (IRQ11_CODE <<  12)  
   #define IRQ12_SEL3            (IRQ12_CODE <<  18)
   #define IRQ13_SEL3            (IRQ13_CODE <<  24)
                                 
   #define IRQ14_SEL4            (IRQ14_CODE <<  0)
   #define IRQ15_SEL4            (IRQ15_CODE <<  6)
   #define IRQ16_SEL4            (IRQ16_CODE <<  12)
   #define IRQ17_SEL4            (IRQ17_CODE <<  18)  
   #define IRQ18_SEL4            (IRQ18_CODE <<  24)
                                 
   #define IRQ19_SEL5            (IRQ19_CODE <<  0)
   #define IRQ1A_SEL5            (IRQ1A_CODE <<  6)
   #define IRQ1B_SEL5            (IRQ1B_CODE <<  12)
   #define IRQ1C_SEL5            (IRQ1C_CODE <<  18)
   #define IRQ1D_SEL5            (IRQ1D_CODE <<  24)  
                                 
   #define IRQ1E_SEL6            (IRQ1E_CODE <<  0)
   #define IRQ1F_SEL6            (IRQ1F_CODE <<  6)
   #define IRQ20_SEL6            (IRQ20_CODE <<  12)
   #define IRQ21_SEL6            (IRQ21_CODE <<  18)
   #define IRQ22_SEL6            (IRQ22_CODE <<  24)
                                 
   #define IRQ23_SEL7            (IRQ23_CODE <<  0)
   #define IRQ24_SEL7            (IRQ24_CODE <<  6)
   #define IRQ25_SEL7            (IRQ25_CODE <<  12)
   #define IRQ26_SEL7            (IRQ26_CODE <<  18)
   #define IRQ27_SEL7            (IRQ27_CODE <<  24)
                                 
   #define FIQ_CODE              IRQ_CTIRQ1_CODE

#elif defined(MT6219) || defined(MT6226) || defined(MT6227)    

   #define IRQ_SEL0              ((volatile UINT32P)(CIRQ_base+0x0000))   /* Source selection 0 to 3 */
   #define IRQ_SEL1              ((volatile UINT32P)(CIRQ_base+0x0004))   /* Source selection 4 to 7 */
   #define IRQ_SEL2              ((volatile UINT32P)(CIRQ_base+0x0008))   /* Source selection 8 to 11 */
   #define IRQ_SEL3              ((volatile UINT32P)(CIRQ_base+0x000C))   /* Source selection 12 to 15 */
   #define IRQ_SEL4              ((volatile UINT32P)(CIRQ_base+0x0010))   /* Source selection 16 to 19 */
   #define IRQ_SEL5              ((volatile UINT32P)(CIRQ_base+0x0014))   /* Source selection 20 to 23 */      
                                 
   #define FIQ_SEL               ((volatile UINT32P)(CIRQ_base+0x0018))   /* FIQ Source selection */
   #define IRQ_MASK              ((volatile UINT32P)(CIRQ_base+0x001C))   /* IRQ Mask */
   #define IRQ_MASK_CLR          ((volatile UINT32P)(CIRQ_base+0x0020))   /* IRQ Mask Disable Register */
   #define IRQ_MASK_SET          ((volatile UINT32P)(CIRQ_base+0x0024))   /* IRQ Mask Enable Register */      

   #define IRQ_STS               ((volatile UINT32P)(CIRQ_base+0x0028))   /* IRQ Source Status */
   #define IRQ_EOI               ((volatile UINT32P)(CIRQ_base+0x002C))   /* End of Interrupt */
   #define IRQ_SENS              ((volatile UINT32P)(CIRQ_base+0x0030))   /* IRQ Sensitivity level */
   #define IRQ_SOFT              ((volatile UINT32P)(CIRQ_base+0x0034))   /* Soft Interrupt */
                                 
   #define FIQ_CON               ((volatile UINT32P)(CIRQ_base+0x0038))   /* FIQ Control */
   #define FIQ_EOI               ((volatile UINT32P)(CIRQ_base+0x003C))   /* FIQ EOI */

   #define IRQ_STS2              ((volatile UINT32P)(CIRQ_base+0x0040))   /* Binary-coded IRQ STS2 */
   #define IRQ_EOI2              ((volatile UINT32P)(CIRQ_base+0x0044))   /* Binary-coded IRQ EOI2 */
                                 
   #define EINT_STATUS           ((volatile UINT32P)(CIRQ_base+0x0100))   /*  external interrupt control */   
   #define EINT_MASK             ((volatile UINT32P)(CIRQ_base+0x0104))   /*  external interrupt control */
   #define EINT_MASK_CLR         ((volatile UINT32P)(CIRQ_base+0x0108))   /*  external interrupt control */
   #define EINT_MASK_SET         ((volatile UINT32P)(CIRQ_base+0x010C))   /*  external interrupt control */         
   #define EINT_INTACK           ((volatile UINT32P)(CIRQ_base+0x0110))   /*  external interrupt control */
   #define EINT_SENS             ((volatile UINT32P)(CIRQ_base+0x0114))   /*  external interrupt control */   
   #define EINT0_CON             ((volatile UINT32P)(CIRQ_base+0x0120))   /*  external interrupt control */
   #define EINT1_CON             ((volatile UINT32P)(CIRQ_base+0x0130))   /*  external interrupt control */
   #define EINT2_CON             ((volatile UINT32P)(CIRQ_base+0x0140))   /*  external interrupt control */
   #define EINT3_CON             ((volatile UINT32P)(CIRQ_base+0x0150))   /*  external interrupt control */
   #define EINT4_CON             ((volatile UINT32P)(CIRQ_base+0x0160))   /*  external interrupt control */
   #define EINT5_CON             ((volatile UINT32P)(CIRQ_base+0x0170))   /*  external interrupt control */         
   #define EINT6_CON             ((volatile UINT32P)(CIRQ_base+0x0180))   /*  external interrupt control */         
   #define EINT7_CON             ((volatile UINT32P)(CIRQ_base+0x0190))   /*  external interrupt control */         

/* CIRQ Interrupt Sources  */
   #define IRQ_GPI_CODE          (0)   /* General Purpose IO */
   #define IRQ_CTIRQ1_CODE       (1)   /* TDMA Sequencer Unit */
   #define IRQ_CTIRQ2_CODE       (2)   /* TDMA Sequencer Unit */
   #define IRQ_DSP2CPU_CODE      (3)   /* DSP Sub-system */
   #define IRQ_SIM_CODE          (4)   /* SIM Interrupt */
   #define IRQ_DMA_CODE          (5)   /* DMA Peripheral */
   #define IRQ_TDMA_CODE         (6)   /* wakeup unit Peripheral */
   #define IRQ_UART1_CODE        (7)   /* UART1 Peripheral */
   #define IRQ_KPAD_CODE         (8)   /* Keypad Scanner Peripheral */
   #define IRQ_UART2_CODE        (9)   /* UART2 Peripheral */
   #define IRQ_GPT_CODE          (10)  /* General Purpose Time Int */
   #define IRQ_EIT_CODE          (11)  /* external Interrupt */
   #define IRQ_USB_CODE          (12)  /* USB Peripheral */
   #define IRQ_MSDC_CODE         (13)  /* Mass Storage Unit */
   #define IRQ_RTC_CODE          (14)  /* RTC Peripheral */
   #define IRQ_IrDA_CODE         (15)  /* IrDA Unit */
   #define IRQ_LCD_CODE          (16)  /* LCD Peripheral */
   #define IRQ_UART3_CODE        (17)  /* UART3 Peripheral */
   #define IRQ_GPII_CODE         (18)  /* GPI II */
   #define IRQ_WDT_CODE          (19)  /* Watchdog */
   #define IRQ_JPEG_CODE         (20)  /* JPEG */
   #define IRQ_RESZ_CODE         (21)  /* Resizer */
   #define IRQ_NFI_CODE          (22)  /* Nandflash */
   #define IRQ_B2PSI_CODE        (23)  /* SPI */
   #define IRQ_IMGDMA_CODE       (24)  /* Image DMA */
   #define IRQ_GIF_CODE          (25)  /* GIF */
   #define IRQ_MSDC_CD_CODE      (26)  /* reserved */
   #define IRQ_SCCB_CODE         (27)  /* SCCB */
   #define IRQ_G2D_CODE          (28)  /* Graphic 2D engine */
   #define IRQ_IMGPROC_CODE      (29)  /* image processing */
   #define IRQ_CAMERA_CODE       (30)  /* camera interface */
   #define IRQ_MPEG4_CODE        (31)  /* MPEG4 */
                                 
   #define NUM_IRQ_SOURCES       (32)

/* IRQ Selection Register mapping */
#if defined(MT6219)

   #define IRQ0_CODE             IRQ_CAMERA_CODE   
   #define IRQ1_CODE             IRQ_CTIRQ1_CODE   /* IRQ_CTIRQ1_CODE */
   #define IRQ2_CODE             IRQ_CTIRQ2_CODE   /* IRQ_CTIRQ2_CODE */
   #define IRQ3_CODE             IRQ_DSP2CPU_CODE  /* IRQ_DSP2CPU_CODE */
   #define IRQ4_CODE             IRQ_SIM_CODE      /* IRQ_SIM_CODE */
   #define IRQ5_CODE             IRQ_DMA_CODE      /* IRQ_DMA_CODE */
   #define IRQ6_CODE             IRQ_TDMA_CODE     /* IRQ_TDMA_CODE */      
   #define IRQ7_CODE             IRQ_UART1_CODE    /* IRQ_UART1_CODE */
   #define IRQ8_CODE             IRQ_KPAD_CODE     /* IRQ_KPAD_CODE */
   #define IRQ9_CODE             IRQ_UART2_CODE    /* IRQ_UART2_CODE */   
   #define IRQA_CODE             IRQ_GPT_CODE      /* IRQ_GPT_CODE */
   #define IRQB_CODE             IRQ_EIT_CODE      /* IRQ_EIT_CODE */
   #define IRQC_CODE             IRQ_USB_CODE      /* IRQ_USB_CODE */   
   #define IRQD_CODE             IRQ_MSDC_CODE     /* IRQ_MSDC_CODE */
   #define IRQE_CODE             IRQ_RTC_CODE      /* IRQ_RTC_CODE */   
   #define IRQF_CODE             IRQ_IrDA_CODE     /* IRQ_IrDA_CODE */      
   #define IRQ10_CODE            IRQ_LCD_CODE      /* IRQ_LCD_CODE */      
   #define IRQ11_CODE            IRQ_UART3_CODE    /* IRQ_UART3_CODE */      
   #define IRQ12_CODE            IRQ_GPI_CODE      /* IRQ_GPI_CODE */         
   #define IRQ13_CODE            IRQ_GPII_CODE     /* GPI II */   
   #define IRQ14_CODE            IRQ_WDT_CODE      /* Watchdog */   
   #define IRQ15_CODE            IRQ_JPEG_CODE     /* JPEG */      
   #define IRQ16_CODE            IRQ_RESZ_CODE                    
   #define IRQ17_CODE            IRQ_NFI_CODE                     
   #define IRQ18_CODE            IRQ_B2PSI_CODE                   
   #define IRQ19_CODE            IRQ_IMGDMA_CODE                  
   #define IRQ1A_CODE            IRQ_GIF_CODE                     
   #define IRQ1B_CODE            IRQ_MSDC_CD_CODE                 
   #define IRQ1C_CODE            IRQ_SCCB_CODE                    
   #define IRQ1D_CODE            IRQ_G2D_CODE                     
   #define IRQ1E_CODE            IRQ_IMGPROC_CODE                 
   #define IRQ1F_CODE            IRQ_MPEG4_CODE
   
#else

   #define IRQ0_CODE             IRQ_CTIRQ1_CODE   /* IRQ_CTIRQ1_CODE */    
   #define IRQ1_CODE             IRQ_CTIRQ2_CODE   /* IRQ_CTIRQ2_CODE */  
   #define IRQ2_CODE             IRQ_DSP2CPU_CODE  /* IRQ_DSP2CPU_CODE */ 
   #define IRQ3_CODE             IRQ_SIM_CODE      /* IRQ_SIM_CODE */     
   #define IRQ4_CODE             IRQ_DMA_CODE      /* IRQ_DMA_CODE */     
   #define IRQ5_CODE             IRQ_TDMA_CODE     /* IRQ_TDMA_CODE */    
   #define IRQ6_CODE             IRQ_UART1_CODE    /* IRQ_UART1_CODE */     
   #define IRQ7_CODE             IRQ_KPAD_CODE     /* IRQ_KPAD_CODE */    
   #define IRQ8_CODE             IRQ_UART2_CODE    /* IRQ_UART2_CODE */   
   #define IRQ9_CODE             IRQ_GPT_CODE      /* IRQ_GPT_CODE */     
   #define IRQA_CODE             IRQ_EIT_CODE      /* IRQ_EIT_CODE */     
   #define IRQB_CODE             IRQ_USB_CODE      /* IRQ_USB_CODE */     
   #define IRQC_CODE             IRQ_MSDC_CODE     /* IRQ_MSDC_CODE */    
   #define IRQD_CODE             IRQ_RTC_CODE      /* IRQ_RTC_CODE */     
   #define IRQE_CODE             IRQ_IrDA_CODE     /* IRQ_IrDA_CODE */    
   #define IRQF_CODE             IRQ_LCD_CODE      /* IRQ_LCD_CODE */       
   #define IRQ10_CODE            IRQ_UART3_CODE    /* IRQ_UART3_CODE */    
   #define IRQ11_CODE            IRQ_GPI_CODE      /* IRQ_GPI_CODE */        
   #define IRQ12_CODE            IRQ_GPII_CODE     /* GPI II */               
   #define IRQ13_CODE            IRQ_WDT_CODE      /* Watchdog */         
   #define IRQ14_CODE            IRQ_JPEG_CODE     /* JPEG */             
   #define IRQ15_CODE            IRQ_RESZ_CODE                            
   #define IRQ16_CODE            IRQ_NFI_CODE                             
   #define IRQ17_CODE            IRQ_B2PSI_CODE                           
   #define IRQ18_CODE            IRQ_IMGDMA_CODE                          
   #define IRQ19_CODE            IRQ_GIF_CODE                             
   #define IRQ1A_CODE            IRQ_MSDC_CD_CODE                         
   #define IRQ1B_CODE            IRQ_SCCB_CODE                            
   #define IRQ1C_CODE            IRQ_G2D_CODE                             
   #define IRQ1D_CODE            IRQ_IMGPROC_CODE                         
   #define IRQ1E_CODE            IRQ_CAMERA_CODE                           
   #define IRQ1F_CODE            IRQ_MPEG4_CODE
   
#endif   

/* IRQ code value in Selection Register */
   #define IRQ0_SEL0             (IRQ0_CODE <<  0)
   #define IRQ1_SEL0             (IRQ1_CODE <<  5)
   #define IRQ2_SEL0             (IRQ2_CODE <<  10)
   #define IRQ3_SEL0             (IRQ3_CODE <<  15)
   #define IRQ4_SEL0             (IRQ4_CODE <<  20)
   #define IRQ5_SEL0             (IRQ5_CODE <<  25)
   #define IRQ6_SEL1             (IRQ6_CODE <<  0)
   #define IRQ7_SEL1             (IRQ7_CODE <<  5)
   #define IRQ8_SEL1             (IRQ8_CODE <<  10)
   #define IRQ9_SEL1             (IRQ9_CODE <<  15)
   #define IRQA_SEL1             (IRQA_CODE <<  20)
   #define IRQB_SEL1             (IRQB_CODE <<  25)
   #define IRQC_SEL2             (IRQC_CODE <<  0)
   #define IRQD_SEL2             (IRQD_CODE <<  5)
   #define IRQE_SEL2             (IRQE_CODE <<  10)
   #define IRQF_SEL2             (IRQF_CODE <<  15)
   #define IRQ10_SEL2            (IRQ10_CODE <<  20)
   #define IRQ11_SEL2            (IRQ11_CODE <<  25)  
   #define IRQ12_SEL3            (IRQ12_CODE <<  0)
   #define IRQ13_SEL3            (IRQ13_CODE <<  5)
   #define IRQ14_SEL3            (IRQ14_CODE <<  10)
   #define IRQ15_SEL3            (IRQ15_CODE <<  15)
   #define IRQ16_SEL3            (IRQ16_CODE <<  20)
   #define IRQ17_SEL3            (IRQ17_CODE <<  25)  
   #define IRQ18_SEL4            (IRQ18_CODE <<  0)
   #define IRQ19_SEL4            (IRQ19_CODE <<  5)
   #define IRQ1A_SEL4            (IRQ1A_CODE <<  10)
   #define IRQ1B_SEL4            (IRQ1B_CODE <<  15)
   #define IRQ1C_SEL4            (IRQ1C_CODE <<  20)
   #define IRQ1D_SEL4            (IRQ1D_CODE <<  25)  
   #define IRQ1E_SEL5            (IRQ1E_CODE <<  0)
   #define IRQ1F_SEL5            (IRQ1F_CODE <<  5)
                                 
   #define FIQ_CODE              IRQ_CTIRQ1_CODE

#elif defined(MT6218) || defined(MT6218B) || defined(MT6217)     

   #define IRQ_SEL0              ((volatile UINT32P)(CIRQ_base+0x0000))   /* Source selection 0 to 3 */
   #define IRQ_SEL1              ((volatile UINT32P)(CIRQ_base+0x0004))   /* Source selection 4 to 7 */
   #define IRQ_SEL2              ((volatile UINT32P)(CIRQ_base+0x0008))   /* Source selection 8 to 11 */
   #define IRQ_SEL3              ((volatile UINT32P)(CIRQ_base+0x000C))   /* Source selection 12 to 15 */
   #define IRQ_SEL4              ((volatile UINT32P)(CIRQ_base+0x0010))   /* Source selection 16 to 19 */
   #define IRQ_SEL5              ((volatile UINT32P)(CIRQ_base+0x0014))   /* Source selection 20 to 23 */      
                                 
   #define FIQ_SEL               ((volatile UINT32P)(CIRQ_base+0x0018))   /* FIQ Source selection */
   #define IRQ_MASK              ((volatile UINT32P)(CIRQ_base+0x001C))   /* IRQ Mask */
   #define IRQ_MASK_CLR          ((volatile UINT32P)(CIRQ_base+0x0020))   /* IRQ Mask Disable Register */
   #define IRQ_MASK_SET          ((volatile UINT32P)(CIRQ_base+0x0024))   /* IRQ Mask Enable Register */      

   #define IRQ_STS               ((volatile UINT32P)(CIRQ_base+0x0028))   /* IRQ Source Status */
   #define IRQ_EOI               ((volatile UINT32P)(CIRQ_base+0x002C))   /* End of Interrupt */
   #define IRQ_SENS              ((volatile UINT32P)(CIRQ_base+0x0030))   /* IRQ Sensitivity level */
   #define IRQ_SOFT              ((volatile UINT32P)(CIRQ_base+0x0034))   /* Soft Interrupt */
                                 
   #define FIQ_CON               ((volatile UINT32P)(CIRQ_base+0x0038))   /* FIQ Control */
   #define FIQ_EOI               ((volatile UINT32P)(CIRQ_base+0x003C))   /* FIQ EOI */

   #define IRQ_STS2              ((volatile UINT32P)(CIRQ_base+0x0040))   /* Binary-coded IRQ STS2 */
   #define IRQ_EOI2              ((volatile UINT32P)(CIRQ_base+0x0044))   /* Binary-coded IRQ EOI2 */
                                 
   #define EINT_STATUS           ((volatile UINT32P)(CIRQ_base+0x0100))   /*  external interrupt control */   
   #define EINT_MASK             ((volatile UINT32P)(CIRQ_base+0x0104))   /*  external interrupt control */
   #define EINT_MASK_CLR         ((volatile UINT32P)(CIRQ_base+0x0108))   /*  external interrupt control */
   #define EINT_MASK_SET         ((volatile UINT32P)(CIRQ_base+0x010C))   /*  external interrupt control */         
   #define EINT_INTACK           ((volatile UINT32P)(CIRQ_base+0x0110))   /*  external interrupt control */
   #define EINT_SENS             ((volatile UINT32P)(CIRQ_base+0x0114))   /*  external interrupt control */   
   #define EINT0_CON             ((volatile UINT32P)(CIRQ_base+0x0120))   /*  external interrupt control */
   #define EINT1_CON             ((volatile UINT32P)(CIRQ_base+0x0130))   /*  external interrupt control */
   #define EINT2_CON             ((volatile UINT32P)(CIRQ_base+0x0140))   /*  external interrupt control */
   #define EINT3_CON             ((volatile UINT32P)(CIRQ_base+0x0150))   /*  external interrupt control */
   #define EINT4_CON             ((volatile UINT32P)(CIRQ_base+0x0160))   /*  external interrupt control */
   #define EINT5_CON             ((volatile UINT32P)(CIRQ_base+0x0170))   /*  external interrupt control */         
   #define EINT6_CON             ((volatile UINT32P)(CIRQ_base+0x0180))   /*  external interrupt control */         
   #define EINT7_CON             ((volatile UINT32P)(CIRQ_base+0x0190))   /*  external interrupt control */         

/* CIRQ Interrupt Sources  */
   #define IRQ_GPI_CODE          (0)   /* General Purpose IO */   
   #define IRQ_CTIRQ1_CODE       (1)   /* TDMA Sequencer Unit */
   #define IRQ_CTIRQ2_CODE       (2)   /* TDMA Sequencer Unit */
   #define IRQ_DSP2CPU_CODE      (3)   /* DSP Sub-system */   
   #define IRQ_SIM_CODE          (4)   /* SIM Interrupt */
   #define IRQ_DMA_CODE          (5)   /* DMA Peripheral */
   #define IRQ_TDMA_CODE         (6)   /* wakeup unit Peripheral */            
   #define IRQ_UART1_CODE        (7)   /* UART1 Peripheral */
   #define IRQ_KPAD_CODE         (8)   /* Keypad Scanner Peripheral */
   #define IRQ_UART2_CODE        (9)   /* UART2 Peripheral */
   #define IRQ_GPT_CODE          (10)  /* General Purpose Time Int  */   
   #define IRQ_EIT_CODE          (11)  /* external Interrupt */
   #define IRQ_USB_CODE          (12)  /* USB Peripheral */
   #define IRQ_MSDC_CODE         (13)  /* Mass Storage Unit */                  
   #define IRQ_RTC_CODE          (14)  /* RTC Peripheral */
   #define IRQ_IrDA_CODE         (15)  /* IrDA Unit */   
   #define IRQ_LCD_CODE          (16)  /* LCD Peripheral */
   #define IRQ_UART3_CODE        (17)  /* UART3 Peripheral */   
   #define IRQ_GPII_CODE         (18)  /* GPI II */   
   #define IRQ_WDT_CODE          (19)  /* Watchdog */   
   
#if defined(MT6218)

   #define NUM_IRQ_SOURCES       (20)   
   
#elif defined(MT6218B) || defined(MT6217)   

   #define IRQ_JPEG_CODE         (20)  /* JPEG */   
   #define IRQ_RESZ_CODE         (21)  /* Resizer */
   #define IRQ_NFI_CODE          (22)  /* Nandflash */
   #define IRQ_B2PSI_CODE        (23)  /* SPI */

#if defined(MT6218B_FN) || defined(MT6217)

   #define IRQ_RESERVED1         (24)
   #define IRQ_RESERVED2         (25)
   #define IRQ_MSDC_CD_CODE      (26)  /* MSDC card detecton  */
   
#endif    

#if defined(MT6218B_FN) || defined(MT6217)

   #define NUM_IRQ_SOURCES       (27)
   
#else    

   #define NUM_IRQ_SOURCES       (24)
   
#endif    

#endif    

/* IRQ Selection Register mapping */
   #define IRQ0_CODE             IRQ_GPI_CODE      /* IRQ_GPI_CODE */   
   #define IRQ1_CODE             IRQ_CTIRQ1_CODE   /* IRQ_CTIRQ1_CODE */
   #define IRQ2_CODE             IRQ_CTIRQ2_CODE   /* IRQ_CTIRQ2_CODE */
   #define IRQ3_CODE             IRQ_DSP2CPU_CODE  /* IRQ_DSP2CPU_CODE */
   #define IRQ4_CODE             IRQ_SIM_CODE      /* IRQ_SIM_CODE */
   #define IRQ5_CODE             IRQ_DMA_CODE      /* IRQ_DMA_CODE */
   #define IRQ6_CODE             IRQ_TDMA_CODE     /* IRQ_TDMA_CODE */      
   #define IRQ7_CODE             IRQ_UART1_CODE    /* IRQ_UART1_CODE */
   #define IRQ8_CODE             IRQ_KPAD_CODE     /* IRQ_KPAD_CODE */
   #define IRQ9_CODE             IRQ_UART2_CODE    /* IRQ_UART2_CODE */   
   #define IRQA_CODE             IRQ_GPT_CODE      /* IRQ_GPT_CODE */
   #define IRQB_CODE             IRQ_EIT_CODE      /* IRQ_EIT_CODE */
   #define IRQC_CODE             IRQ_USB_CODE      /* IRQ_USB_CODE */   
   #define IRQD_CODE             IRQ_MSDC_CODE     /* IRQ_MSDC_CODE */
   #define IRQE_CODE             IRQ_RTC_CODE      /* IRQ_RTC_CODE */   
   #define IRQF_CODE             IRQ_IrDA_CODE     /* IRQ_IrDA_CODE */      
   #define IRQ10_CODE            IRQ_LCD_CODE      /* IRQ_LCD_CODE */      
   #define IRQ11_CODE            IRQ_UART3_CODE    /* IRQ_UART3_CODE */      

#if ( defined(MT6218B) || defined(MT6217) )

   #define IRQ12_CODE            IRQ_GPII_CODE      /* GPI II */   
   #define IRQ13_CODE            IRQ_WDT_CODE       /* Watchdog */   
   #define IRQ14_CODE            IRQ_JPEG_CODE      /* JPEG */   
   #define IRQ15_CODE            IRQ_RESZ_CODE
   #define IRQ16_CODE            IRQ_NFI_CODE
   #define IRQ17_CODE            IRQ_B2PSI_CODE
   
#endif  

#if defined(MT6218B_FN) || defined(MT6217)

   #define IRQ18_CODE            IRQ_RESERVED1
   #define IRQ19_CODE            IRQ_RESERVED2
   #define IRQ1A_CODE            IRQ_MSDC_CD_CODE
   
#endif    

/* IRQ code value in Selection Register */
   #define IRQ0_SEL0             (IRQ0_CODE <<  0)
   #define IRQ1_SEL0             (IRQ1_CODE <<  5)
   #define IRQ2_SEL0             (IRQ2_CODE <<  10)
   #define IRQ3_SEL0             (IRQ3_CODE <<  15)
   #define IRQ4_SEL0             (IRQ4_CODE <<  20)
   #define IRQ5_SEL0             (IRQ5_CODE <<  25)
   #define IRQ6_SEL1             (IRQ6_CODE <<  0)
   #define IRQ7_SEL1             (IRQ7_CODE <<  5)
   #define IRQ8_SEL1             (IRQ8_CODE <<  10)
   #define IRQ9_SEL1             (IRQ9_CODE <<  15)
   #define IRQA_SEL1             (IRQA_CODE <<  20)
   #define IRQB_SEL1             (IRQB_CODE <<  25)
   #define IRQC_SEL2             (IRQC_CODE <<  0)
   #define IRQD_SEL2             (IRQD_CODE <<  5)
   #define IRQE_SEL2             (IRQE_CODE <<  10)
   #define IRQF_SEL2             (IRQF_CODE <<  15)
   #define IRQ10_SEL2            (IRQ10_CODE <<  20)
   #define IRQ11_SEL2            (IRQ11_CODE <<  25)  

#if defined(MT6218B) || defined(MT6217)

   #define IRQ12_SEL3            (IRQ12_CODE <<  0)
   #define IRQ13_SEL3            (IRQ13_CODE <<  5)
   #define IRQ14_SEL3            (IRQ14_CODE <<  10)
   #define IRQ15_SEL3            (IRQ15_CODE <<  15)
   #define IRQ16_SEL3            (IRQ16_CODE <<  20)
   #define IRQ17_SEL3            (IRQ17_CODE <<  25)  

#if defined(MT6218B_FN) || defined(MT6217)

   #define IRQ18_SEL4            (IRQ18_CODE <<  0)
   #define IRQ19_SEL4            (IRQ19_CODE <<  5)
   #define IRQ1A_SEL4            (IRQ1A_CODE <<  10)
   
#endif    

#endif    

   #define FIQ_CODE              IRQ_CTIRQ1_CODE

#else    

   #define IRQ_SEL0              ((volatile UINT16P)(CIRQ_base+0x0000))   /* Source selection 0 to 3   */
   #define IRQ_SEL1              ((volatile UINT16P)(CIRQ_base+0x0004))   /* Source selection 4 to 7   */
   #define IRQ_SEL2              ((volatile UINT16P)(CIRQ_base+0x0008))   /* Source selection 8 to 11  */
   #define IRQ_SEL3              ((volatile UINT16P)(CIRQ_base+0x000C))   /* Source selection 12 to 14 */
   #define FIQ_SEL               ((volatile UINT16P)(CIRQ_base+0x0010))   /* FIQ Source selection      */
   #define IRQ_MASK              ((volatile UINT16P)(CIRQ_base+0x0014))   /* IRQ Mask                  */
   #define IRQ_STS               ((volatile UINT16P)(CIRQ_base+0x0018))   /* IRQ Source Status         */
   #define IRQ_EOI               ((volatile UINT16P)(CIRQ_base+0x001C))   /* End of Interrupt          */
   #define IRQ_SENS              ((volatile UINT16P)(CIRQ_base+0x0020))   /* IRQ Sensitivity level     */
   #define IRQ_SOFT              ((volatile UINT16P)(CIRQ_base+0x0024))   /* Soft Interrupt            */
                                 
   #define FIQ_CON               ((volatile UINT16P)(CIRQ_base+0x0028))   /* FIQ Control */
   #define FIQ_EOI               ((volatile UINT16P)(CIRQ_base+0x002C))   /* FIQ EOI */
                                 
   #define IRQ_STS2              ((volatile UINT16P)(CIRQ_base+0x0038))   /* Binary-coded IRQ STS2 */
   #define IRQ_EOI2              ((volatile UINT16P)(CIRQ_base+0x003C))   /* Binary-coded IRQ EOI2 */
                                 
   #define EINT_MASK             ((volatile UINT16P)(CIRQ_base+0x0044))   /*  external interrupt  control  */
   #define EINT_STATUS           ((volatile UINT16P)(CIRQ_base+0x0040))   /*  external interrupt  control  */
   #define EINT_INTACK           ((volatile UINT16P)(CIRQ_base+0x0048))   /*  external interrupt  control  */
   #define EINT0_CON             ((volatile UINT16P)(CIRQ_base+0x0050))   /*  external interrupt  control  */
   #define EINT1_CON             ((volatile UINT16P)(CIRQ_base+0x0060))   /*  external interrupt  control  */
   #define EINT2_CON             ((volatile UINT16P)(CIRQ_base+0x0070))   /*  external interrupt  control  */
                                 
#ifdef MT6205B                   
   #define IRQ_MASK_CLR          ((volatile UINT16P)(CIRQ_base+0x00a0))   /* Clear Control for IRQ Mask Register */
   #define EINT_MASK_CLR         ((volatile UINT16P)(CIRQ_base+0x00a8))   /* Clear Control for EINT Mask Register */
   #define IRQ_MASK_SET          ((volatile UINT16P)(CIRQ_base+0x00a4))   /* Set Control for IRQ Mask Register */
   #define EINT_MASK_SET         ((volatile UINT16P)(CIRQ_base+0x00ac))   /* Set Control for EINT Mask Register */
#endif  

/* CIRQ Interrupt Sources  */
   #define IRQ_CTIRQ1_CODE       (0)   /* TDMA Sequencer Unit */
   #define IRQ_CTIRQ2_CODE       (1)   /* TDMA Sequencer Unit */
   #define IRQ_UART1_CODE        (2)   /* UART1 Peripheral */
   #define IRQ_RTC_CODE          (3)   /* RTC Peripheral */
   #define IRQ_LCD_CODE          (4)   /* LCD Peripheral */
   #define IRQ_SIM_CODE          (5)   /* SIM Interrupt */
   #define IRQ_DSP2CPU_CODE      (6)   /* DSP Sub-system */
   #define IRQ_KPAD_CODE         (7)   /* Keypad Scanner Peripheral */
   #define IRQ_USB_CODE          (8)   /* USB Peripheral */
   #define IRQ_GPT_CODE          (9)   /* General Purpose Time Int  */
   #define IRQ_EIT_CODE          (10)  /* external Interrupt */
   #define IRQ_DMA_CODE          (11)  /* DMA Peripheral */
   #define IRQ_TDMA_CODE         (12)  /*  wakeup unit Peripheral */
   #define IRQ_UART2_CODE        (13)  /* UART2 Peripheral */
   #define IRQ_MMC_CODE          (14)  /* MMC peripheral */
   #define IRQ_EINT2_CODE        (14)  /* External Interrupt 2 */
   #define IRQ_EXT_NIRQ_CODE     (15)  /* External NIRQ */
   #define NUM_IRQ_SOURCES       (16)  

/* IRQ Selection Register mapping */
   #define IRQ0_CODE             IRQ_CTIRQ1_CODE      /* IRQ_CTIRQ1_CODE */
   #define IRQ1_CODE             IRQ_CTIRQ2_CODE      /* IRQ_CTIRQ2_CODE */
   #define IRQ2_CODE             IRQ_UART1_CODE       /* IRQ_DSP2CPU_CODE */
   #define IRQ3_CODE             IRQ_RTC_CODE         /* IRQ_SIM_CODE */
   #define IRQ4_CODE             IRQ_LCD_CODE         /* IRQ_DMA_CODE */
   #define IRQ5_CODE             IRQ_SIM_CODE         /* IRQ_TDMA_CODE */
   #define IRQ6_CODE             IRQ_DSP2CPU_CODE     /* IRQ_UART1_CODE */
   #define IRQ7_CODE             IRQ_KPAD_CODE        /* IRQ_KPAD_CODE */
   #define IRQ8_CODE             IRQ_USB_CODE         /* IRQ_UART2_CODE */
   #define IRQ9_CODE             IRQ_GPT_CODE         /* IRQ_GPT_CODE */
   #define IRQA_CODE             IRQ_EIT_CODE         /* IRQ_EIT_CODE */
   #define IRQB_CODE             IRQ_DMA_CODE         /* IRQ_RTC_CODE */
   #define IRQC_CODE             IRQ_TDMA_CODE        /* IRQ_LCD_CODE */
   #define IRQD_CODE             IRQ_UART2_CODE       /* IRQ_USB_CODE */
   #define IRQE_CODE             IRQ_MMC_CODE         /* IRQ_MMC_CODE */
   #define IRQF_CODE             IRQ_EXT_NIRQ_CODE    /* IRQ_EXT_NIRQ */

/* IRQ code value in Selection Register */
   #define IRQ0_SEL0             (IRQ0_CODE << 0)
   #define IRQ1_SEL0             (IRQ1_CODE << 4)
   #define IRQ2_SEL0             (IRQ2_CODE << 8)
   #define IRQ3_SEL0             (IRQ3_CODE << 12)
   #define IRQ4_SEL1             (IRQ4_CODE << 0)
   #define IRQ5_SEL1             (IRQ5_CODE << 4)
   #define IRQ6_SEL1             (IRQ6_CODE << 8)
   #define IRQ7_SEL1             (IRQ7_CODE << 12)
   #define IRQ8_SEL2             (IRQ8_CODE << 0)
   #define IRQ9_SEL2             (IRQ9_CODE << 4)
   #define IRQA_SEL2             (IRQA_CODE << 8)
   #define IRQB_SEL2             (IRQB_CODE << 12)
   #define IRQC_SEL3             (IRQC_CODE << 0)
   #define IRQD_SEL3             (IRQD_CODE << 4)
   #define IRQE_SEL3             (IRQE_CODE << 8)
   #define IRQF_SEL3             (IRQF_CODE << 12)
   #define FIQ_CODE              IRQ_CTIRQ1_CODE   

#endif   

#define EDGE_SENSITIVE           KAL_TRUE
#define LEVEL_SENSITIVE          KAL_FALSE

  
/*******************************************************************************
 * Special for display on SWDBG - MCU profiling
 *******************************************************************************/
#if ( defined(MT6205B) || defined(MT6208) ) 

typedef enum   
{
   LISR_CTIRQ1    = IRQ_CTIRQ1_CODE,
   LISR_CTIRQ2    = IRQ_CTIRQ2_CODE,
   LISR_UART1     = IRQ_UART1_CODE,
   LISR_RTC       = IRQ_RTC_CODE,
   LISR_LCD       = IRQ_LCD_CODE,
   LISR_SIM       = IRQ_SIM_CODE,
   LISR_DSP2CPU   = IRQ_DSP2CPU_CODE,
   LISR_KPAD      = IRQ_KPAD_CODE,
   LISR_USB       = IRQ_USB_CODE,
   LISR_GPT       = IRQ_GPT_CODE,
   LISR_EIT       = IRQ_EIT_CODE,
   LISR_DMA       = IRQ_DMA_CODE,
   LISR_TDMA      = IRQ_TDMA_CODE,
   LISR_UART2     = IRQ_UART2_CODE,
   LISR_MMC       = IRQ_MMC_CODE,
   LISR_EINT2     = IRQ_EINT2_CODE,
   LISR_NIRQ      = IRQ_EXT_NIRQ_CODE
} LISR_IRQ_NO;

#else

typedef enum
{
   LISR_GPI       = IRQ_GPI_CODE,
   LISR_CTIRQ1    = IRQ_CTIRQ1_CODE,
   LISR_CTIRQ2    = IRQ_CTIRQ2_CODE,
   LISR_DSP2CPU   = IRQ_DSP2CPU_CODE,
   LISR_SIM       = IRQ_SIM_CODE,
   LISR_DMA       = IRQ_DMA_CODE,
   LISR_TDMA      = IRQ_TDMA_CODE,
   LISR_UART1     = IRQ_UART1_CODE,
   LISR_KPAD      = IRQ_KPAD_CODE,
   LISR_UART2     = IRQ_UART2_CODE,
   LISR_GPT       = IRQ_GPT_CODE,
   LISR_EIT       = IRQ_EIT_CODE,
   LISR_USB       = IRQ_USB_CODE,
   LISR_MSDC      = IRQ_MSDC_CODE,
   LISR_RTC       = IRQ_RTC_CODE,
   LISR_IrDA      = IRQ_IrDA_CODE,
   LISR_LCD       = IRQ_LCD_CODE,
   LISR_UART3     = IRQ_UART3_CODE,
   LISR_GPII      = IRQ_GPII_CODE,
   LISR_WDT       = IRQ_WDT_CODE,
   
#if defined(MT6218B) || defined(MT6217)

   LISR_JPEG      = IRQ_JPEG_CODE,
   LISR_RESZ      = IRQ_RESZ_CODE,
   LISR_NFI       = IRQ_NFI_CODE,
   LISR_B2PSI     = IRQ_B2PSI_CODE

#if defined(MT6218B_FN) || defined(MT6217)
   , LISR_MSDC_CD = IRQ_MSDC_CD_CODE
#endif

#elif defined(MT6219) || defined(MT6226) || defined(MT6227)

   LISR_JPEG      = IRQ_JPEG_CODE,
   LISR_RESZ      = IRQ_RESZ_CODE,
   LISR_NFI       = IRQ_NFI_CODE,
   LISR_B2PSI     = IRQ_B2PSI_CODE,
   LISR_IMGDMA    = IRQ_IMGDMA_CODE,
   LISR_GIF       = IRQ_GIF_CODE,
   LISR_MSDC_CD   = IRQ_MSDC_CD_CODE,
   LISR_SCCB      = IRQ_SCCB_CODE,
   LISR_G2D       = IRQ_G2D_CODE,
   LISR_IMGPROC   = IRQ_IMGPROC_CODE,
   LISR_CAMERA    = IRQ_CAMERA_CODE,
   LISR_MPEG4     = IRQ_MPEG4_CODE

#elif defined(MT6228) || defined(MT6229)

   LISR_SWDBG        = IRQ_SWDBG_CODE,
   LISR_CHE          = IRQ_CHE_CODE,
   LISR_NFI          = IRQ_NFI_CODE,
   LISR_B2PSI        = IRQ_B2PSI_CODE,
   LISR_IMGDMA       = IRQ_IMGDMA_CODE,
   LISR_GIF          = IRQ_GIF_CODE,
   LISR_PNG          = IRQ_PNG_CODE,
   LISR_SCCB         = IRQ_SCCB_CODE,
   LISR_G2D          = IRQ_G2D_CODE,
   LISR_IMGPROC      = IRQ_IMGPROC_CODE,
   LISR_CAMERA       = IRQ_CAMERA_CODE,
   LISR_PFC          = IRQ_PFC_CODE,
   LISR_MPEG4_DEC    = IRQ_MPEG4_DEC_CODE,
   LISR_MPEG4_ENC    = IRQ_MPEG4_ENC_CODE,
   LISR_JPEG_DEC     = IRQ_JPEG_DEC_CODE,
   LISR_JPEG_ENC     = IRQ_JPEG_ENC_CODE,
   LISR_RESIZER_CRZ  = IRQ_RESIZER_CRZ_CODE,
   LISR_RESIZER_DRZ  = IRQ_RESIZER_DRZ_CODE,
   LISR_RESIZER_PRZ  = IRQ_RESIZER_PRZ_CODE,
   LISR_TVOUT        = IRQ_TVOUT_CODE
   
#endif    

} LISR_IRQ_NO;

#endif  


/*******************************************************************************
 * Declarations and Definitions
 *******************************************************************************/
 
typedef struct IRQ_MASK_VALUE_STRUCT
{
   kal_uint32 irq_maskl;
   kal_uint32 irq_maskh;
} IRQ_MASK_VALUE_T;

extern void initINTR(void);
extern void IRQSel(void);
extern void FIQSel(void);
extern void IRQMaskAll(void);
extern void IRQDirectMaskAll(void);
extern void FIQMask(void);
extern void IRQMask(kal_uint8);
extern void IRQUnmask(kal_uint8);
extern void FIQUnmask(void);
extern void IRQClearAllInt(void);
extern void IRQClearInt(kal_uint8);
extern void IRQSensitivity(kal_uint8, kal_bool);
extern kal_uint32 SaveAndSetIRQMask(void);
extern void RestoreIRQMask(kal_uint32);
extern void CTIRQ1_2_MaskAll(IRQ_MASK_VALUE_T *val);
extern void CTIRQ1_2_Restore(IRQ_MASK_VALUE_T *val);

/* Hardware register write during interrupts disabled */
#define REG_WRITE(reg_addr, value)  { \
   kal_uint32 _savedMask; \
   _savedMask = SaveAndSetIRQMask(); \
   *(volatile kal_uint16*)(reg_addr) = (kal_uint16)(value); \
   RestoreIRQMask(_savedMask); \
}

/* Hardware register read during interrupts disabled */
#define REG_READ(reg_addr, value) { \
   kal_uint32 _savedMask; \
   _savedMask = SaveAndSetIRQMask(); \
   (kal_uint16)(value) = *(volatile kal_uint16*)(reg_addr); \
   RestoreIRQMask(_savedMask); \
}

extern void IRQClearInt2(kal_uint8 irq_idx);
extern const kal_uint8 irqLine[NUM_IRQ_SOURCES];
extern kal_uint8 IRQCode2Line[NUM_IRQ_SOURCES];
extern void IRQ_Register_LISR(kal_uint32 irqvector, void (*reg_lisr)(void), char* description);
extern kal_uint16 INT_BootMode(void);

#endif /* _INTRCTRL_H */

