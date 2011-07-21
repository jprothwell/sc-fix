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
 *    adc.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for GPT driver.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _ADC_H
#define _ADC_H 

#include "adc_channel.h"
#if ( (defined(MT6205)) || (defined(MT6208)) || (defined(FPGA)))
   /*******************
    * GPADC Registers *
    *******************/
   #define AUXADC_DATA  (AUXADC_base+0x0000)	/* Sampled data             */
   #define AUXADC_CTRL  (AUXADC_base+0x0004)	/* Control of A/D Converter */
   #define AUXADC_STAT  (AUXADC_base+0x0008)	/* A/D Status..reg=0,if write AUXADC_CTRL reg*/
   #define AUXADC_CTRL2 (AUXADC_base+0x000c)	/* Special Control of A/D Converter */
   
   #define AUXADC_STAT_RDY             0x0001      /*ADC ready*/
   
   #define AUXADC_CTRL2_CALI           0x0001      /*ADC Calibration*/
   #define AUXADC_CTRL2_MON            0x0020      /*DACMON*/
   #define AUXADC_CTRL2_BYP            0x0040      /*DACBYP*/
   
   /*ADC pin selection, ADC phy. channel*/
  
   #define ADC_MAX_CHANNEL  5
   #define ADC_ERR_CHANNEL_NO       50
#endif    
/*************************************************************************/
#if ( (defined(MT6205B)) || (defined(MT6218)) || (defined(MT6218B)) || (defined(MT6219)) || (defined(MT6217))|| (defined(MT6228))|| (defined(MT6229))|| (defined(MT6226))|| (defined(MT6227)) )
   #define AUXADC_SYNC                    (AUXADC_base+0x0000)
   #define AUXADC_IMM                     (AUXADC_base+0x0004)
   #define AUXADC_SYN                     (AUXADC_base+0x0008)
   #define AUXADC_CON                     (AUXADC_base+0x000c)
   #define AUXADC_DAT(_line)              (AUXADC_base+0x0010+(4*_line))
   #if defined(MT6226)||defined(MT6227)
   #define AUXADC_TDMA_EVENT0             (TDMA_base+0x1c0)
   #define AUXADC_TDMA_EVENT1             (TDMA_base+0x1c4)
   #else
   #define AUXADC_TDMA_EVENT0             (TDMA_base+0x1b0)
   #define AUXADC_TDMA_EVENT1             (TDMA_base+0x1b4)
   #endif
   #define AUXADC_TDMA_EN                 (TDMA_base+0x164)
   
   /*AUXADC_SYNC*/
   #define AUXADC_SYNC_CHAN(_line)        (0x0001<<_line)   /*Time event 1*/
   
   /*AUXADC_IMM*/
   #define AUXADC_IMM_CHAN(_line)         (0x0001<<_line)
   
   /*AUXADC_SYN*/
   #define AUXADC_SYN_BIT                 (0x0001)          /*Time event 0*/
   
   /*AUXADC_CON*/
   #define AUXADC_CON_RUN                 (0x0001)
   #define AUXADC_CON_CALI_MASK           (0x007c)
   #define AUXADC_CON_TESTDACMON          (0x0080)
   #define AUXADC_CON_AUTOCLR0            (0x0100)
   #define AUXADC_CON_AUTOCLR1            (0x0200)
   #define AUXADC_CON_PUWAIT_EN           (0x0800)
   #define AUXADC_CON_AUTOSET             (0x8000)

   /*AUXADC_TDMA_EN*/
   #define AUXADC_TDMA_EN_EVT0            (0x0001)
   #define AUXADC_TDMA_EN_EVT1            (0x0002)

   #ifdef MT6205B
      #define ADC_MAX_CHANNEL  5
   #endif    
   
   #if ( defined(MT6218) || defined(MT6218B)  || (defined(MT6219)) || (defined(MT6217))|| (defined(MT6228))|| (defined(MT6229))|| (defined(MT6226))|| (defined(MT6227)) )
      #define ADC_MAX_CHANNEL  7
   #endif    
   
   #define ADC_ERR_CHANNEL_NO       50
#endif    

#ifdef MT6218
   typedef struct
   {
      kal_int32 ADCSlope[ADC_MAX_CHANNEL];
      kal_int32 ADCOffset[ADC_MAX_CHANNEL];
   }ADC_CALIDATA;
#else
   typedef struct
   {
      kal_int32 ADCSlope[ADC_MAX_CHANNEL];
      kal_int32 ADCOffset[ADC_MAX_CHANNEL];
   }ADC_CALIDATA;
#endif
/*************************************************************************/
extern kal_uint16 ADC_GetData(kal_uint8 sel);
#endif   /*_ADC_H*/

