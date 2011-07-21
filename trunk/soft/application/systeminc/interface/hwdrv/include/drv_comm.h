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
 *    drv_comm.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for driver common interface.
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
#ifndef _DRV_COMM_H
#define _DRV_COMM_H

#include "kal_release.h"
//#include "drvpdn.h"

typedef enum {
    	io_low=0,
	   io_high
} IO_level;

typedef enum {
    	DEV_DMA=0,
    	DEV_USB,
    	DEV_GCU,
    	DEV_CHE,
    	DEV_PPP,
    	DEV_GPT,
    	DEV_KP,
    	DEV_GPIO,
    	DEV_UART1,
    	DEV_UART2,
    	DEV_UART3,
    	DEV_SIM,
    	DEV_PWM1,
    	DEV_PWM2,
    	DEV_ALTER,
    	DEV_LCD,
    	DEV_MSDC,
    	DEV_TRC,
    	DEV_NFI,
    	DEV_B2SPI,
    	DEV_IRDA,
    	DEV_ADC,
    	DEV_RTC,
    	DEV_DIV,
    	DEV_FCS,
    	DEV_RZLB,
    	DEV_CRZ,
    	DEV_TV_OUT,
    	DEV_IPP,
    	DEV_PNG,
    	DEV_GIF,
    	DEV_GCMQ,
    	DEV_G2D,
    	DEV_MP4,
    	DEV_JPEG,
    	DEV_PRZ,
    	DEV_RESZ,
    	DEV_ISP,
    	DEV_DCT,
    	DEV_IMGDMA,
    	DEV_DRZ,
    	DEV_MAX_ID
} DEVICE_ID_ENUM;

typedef void (* VOID_FUNC)(void);
#define DRV_WriteReg(addr,data)     ((*(volatile kal_uint16 *)(addr)) = (kal_uint16)(data))
#define DRV_Reg(addr)               (*(volatile kal_uint16 *)(addr))
#define DRV_WriteReg32(addr,data)     ((*(volatile kal_uint32 *)(addr)) = (kal_uint32)(data))
#define DRV_Reg32(addr)               (*(volatile kal_uint32 *)(addr))
#define DRV_WriteReg8(addr,data)     ((*(volatile kal_uint8 *)(addr)) = (kal_uint8)(data))
#define DRV_Reg8(addr)               (*(volatile kal_uint8 *)(addr))

#define DRV_BuildPrimitive(_ilm,_srcid,_dstid,_msgid,_data) \
{\
   _ilm = allocate_ilm(_srcid);\
   _ilm->src_mod_id = _srcid;\
   _ilm->sap_id = DRIVER_PS_SAP;\
   _ilm->dest_mod_id = _dstid;\
   _ilm->msg_id = _msgid;\
   _ilm->local_para_ptr = (local_para_struct *)_data;\
   _ilm->peer_buff_ptr = NULL;\
}

#define DRV_SendPrimitive(_ilm,_srcid,_dstid,_msgid,_data, _sap_id) \
{\
   _ilm = allocate_ilm(_srcid);\
   _ilm->src_mod_id = _srcid;\
   _ilm->sap_id = _sap_id;\
   _ilm->dest_mod_id = _dstid;\
   _ilm->msg_id = _msgid;\
   _ilm->local_para_ptr = (local_para_struct *)_data;\
   _ilm->peer_buff_ptr = NULL;\
}

extern void Fast_Memcpy(void *srcaddr, void *dstaddr, kal_uint32 leng);
extern void Fast_SherifWrite(void *srcaddr,void *dstaddr,kal_uint32 len);
extern void Fast_SherifRead(void *srcaddr,void *dstaddr,kal_uint32 len);
extern kal_uint32 SaveAndSetIRQMask(void);
extern void RestoreIRQMask(kal_uint32);

#ifdef DRV_DEBUG
extern void dbg_print(char *fmt,...);
extern void dbg_printWithTime(char *fmt,...);
#endif   /*DRV_DEBUG*/

#ifdef DRV_TIMING_DEBUG
#define MAX_DRV_DBG_INFO_SIZE 2048
typedef enum {
   NAND_READ_START,
   NAND_READ_STOP,
   DRV_DBG_MAX_ID
} DRV_DBG_ID;

typedef struct{
   DRV_DBG_ID      tag;
   kal_uint32      time;
   kal_uint32      data1;
   kal_uint32      data2;
}DRV_DBG_DATA;

typedef struct{
   DRV_DBG_DATA   dbg_data[MAX_DRV_DBG_INFO_SIZE];
   kal_uint16     dbg_data_idx;
}DRV_DBG_STRUCT;
#endif   /*DRV_TIMING_DEBUG*/

#if ( (defined(MT6205B)) || (defined(MT6218)) || (defined(MT6218B))  || (defined(MT6219)) || (defined(MT6217))|| (defined(MT6228))|| (defined(MT6229))|| (defined(MT6226))|| (defined(MT6227)) )
   #define DRV_COMM_REG1                  0x80210058
   #define DRV_COMM_REG2                  0x8021005c
   #define DRV_COMM_REG2_NORMAL_RESET     0x0080
   #define DRV_COMM_REG2_CHRPWRON         0x0040
   #define DRV_COMM_REG2_USBMS_PWRON      0x0020
   #define DRV_COMM_REG2_RTCPWRON         0x0010
   #define DRV_COMM_REG2_USBMS_PWROFF     0x0008
   #define DRV_COMM_REG1_META_RESET_RTC   0x0080
#endif    

#endif   /*_DRV_COMM_H*/

