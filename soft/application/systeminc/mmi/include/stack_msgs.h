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
 *   stack_msgs.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file provides a template for header files. This space
 *   should be used to describe the file contents
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/*********************************************************************
   (C) _____ (year of first publication) Sasken communication
   Technologies Limited, All rights reserved.
*   This file provides a template for .c files. This space
*   should be used to describe the file contents
*   Component-specific prefix : xxxx
*********************************************************************/

#ifndef _STACK_MSGS_H
#define _STACK_MSGS_H

#include "mmi_features.h"

#define ADD_MAG(name) name
typedef enum {
   MSG_ID_INVALID_TYPE = 0,

#if defined(CSD_TARGET) || defined (CSD_KAL_MNT)

#ifndef __L1_STANDALONE__

#if 0
   #include "mm_sap.h"
   MSG_ID_MM_CODE_CHECK_POINT,

   #include "cc_sap.h"
   MSG_ID_CC_CODE_CHECK_POINT,

   #include "dt_sap.h"
   MSG_ID_DT_CODE_CHECK_POINT,

   #include "ciss_sap.h"
   MSG_ID_CISS_CODE_CHECK_POINT,

   #include "sms_sap.h"
   MSG_ID_SMS_CODE_CHECK_POINT,

   #include "sim_sap.h"
   MSG_ID_SIM_CODE_CHECK_POINT,

   #include "l4_sap.h"
   MSG_ID_L4_CODE_CHECK_POINT,
#endif 

   #include "mmi_sap.h"
   MSG_ID_MMI_CODE_CHECK_POINT,

#if 0
   #include "rr_sap.h"
   MSG_ID_RR_CODE_CHECK_POINT,

   #include "llc_sap.h"
   MSG_ID_LLC_CODE_CHECK_POINT,

   #include "sndcp_sap.h"
   MSG_ID_SND_CODE_CHECK_POINT,

   #include "sm_sap.h"
   MSG_ID_SM_CODE_CHECK_POINT,

   #include "ppp_sap.h"
   MSG_ID_PPP_CODE_CHECK_POINT,

   #include "data_sap.h"
   MSG_ID_DATA_CODE_CHECK_POINT,
#endif 

   #include "mmiapi_sap.h"
   MSG_ID_MMIAPI_CODE_CHECK_POINT,

   #include "med_sap.h"
   MSG_ID_MED_CODE_CHECK_POINT,

#if 0
   #include "abm_sap.h"
   MSG_ID_ABM_CODE_CHECK_POINT,


   #include "tcpip_sap.h"
   MSG_ID_TCPIP_CODE_CHECK_POINT,
#endif 

#ifdef MMI_ON_HARDWARE_P
   #include "soc_sap.h"
   MSG_ID_SOC_CODE_CHECK_POINT,
#endif

   #include "wap_sap.h"  
   MSG_ID_WAP_CODE_CHECK_POINT,

#if 0
   #include "em_sap.h"
   MSG_ID_EM_CODE_CHECK_POINT,

   #include "irda_sap.h"
   MSG_ID_IRDA_CODE_CHECK_POINT,

   #include "obex_sap.h"
   MSG_ID_OBEX_CODE_CHECK_POINT,

   #include "ircomm_sap.h"
   MSG_ID_IRCOMM_CODE_CHECK_POINT,

#ifndef MED_V_NOT_PRESENT
    
   MSG_ID_MED_V_CODE_CHECK_POINT,
#endif /* !MED_V_NOT_PRESENT */
#endif 

   #include "j2me_sap.h"
   MSG_ID_J2ME_CODE_CHECK_POINT,

#if 0
   #include "email_sap.h"
   MSG_ID_EMAIL_CODE_CHECK_POINT,
#endif 

#if ( !defined(FMT_NOT_PRESENT) && !defined(__L1_STANDALONE__) && !defined(__MAUI_BASIC__) )
   #include "fmt_sap.h"
   MSG_ID_FMT_CODE_CHECK_POINT,
#endif /* !FMT_NOT_PRESENT */

#endif /* !__L1_STANDALONE__ */

   #include "drv_sap.h"
   MSG_ID_DRV_CODE_CHECK_POINT,

   #include "nvram_sap.h"
   MSG_ID_NVRAM_CODE_CHECK_POINT,
   
#endif /* CSD_TARGET or CSD_KAL_MNT */

#if 0
#ifdef DUMMY_PROTOCOL
   #include "dps_sap.h"
   MSG_ID_DPS_CODE_CHECK_POINT,
#endif /* DUMMY_PROTOCOL */
   #include "l1_sap.h"
   MSG_ID_L1_CODE_CHECK_POINT,

   #include "l1hisr_sap.h"
   MSG_ID_L1HISR_CODE_CHECK_POINT,

   #include "ft_sap.h"
   MSG_ID_FT_CODE_CHECK_POINT,

#if defined(CSD_TARGET) || defined (CSD_KAL_MNT)

   #include "tstsap.h"
   MSG_ID_TST_CODE_CHECK_POINT,
  
 //  #include "sysservice_sap.h"
  // MSG_ID_SYSSERVICE_CODE_CHECK_POINT,
   
   #include "cmux_sap.h"
   MSG_ID_CMUX_CODE_CHECK_POINT,
   
   #include "imps_sap.h"
   MSG_ID_IMPS_CODE_CHECK_POINT,
   
   #include "sip_sap.h"
   MSG_ID_SIP_CODE_CHECK_POINT,

   #include "poc_sap.h"
   MSG_ID_POC_CODE_CHECK_POINT,
  
  

/*************************************************************************
 * Custom to add message id here, and maximum allow 1000 messages which 
 * limited by END_MSG_CODE_BEGIN 
 *************************************************************************/
   #include "custom_sap.h"   
#endif /* CSD_TARGET or CSD_KAL_MNT */
   
#endif 

   #include "sysservice_sap.h"
   MSG_ID_SYSSERVICE_CODE_CHECK_POINT,
   

   #ifdef __MMI_BT_SUPPORT__
     #include "bluetooth_sap.h"
   MSG_ID_BT_CODE_CHECK_POINT,
   #endif

   MSG_ID_END
} msg_type_t;

#undef ADD_MAG

#endif /* _STACK_MSGS_H */

