/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
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
 * DataAccountGProt.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for ¡K.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _DATA_ACCOUNT_GPROT_H
#define _DATA_ACCOUNT_GPROT_H

#define MAX_DATA_ACCOUNT_NAME_LEN            31

#define DATA_ACCOUNT_BEARER_CSD  0x01
#define DATA_ACCOUNT_BEARER_GPRS 0x02
#define DATA_ACCOUNT_BEARER_WIFI 0x04
#define DATA_ACCOUNT_BEARER_ALL        DATA_ACCOUNT_BEARER_CSD | DATA_ACCOUNT_BEARER_GPRS | DATA_ACCOUNT_BEARER_WIFI
#define DATA_ACCOUNT_BEARER_PS_ONLY    DATA_ACCOUNT_BEARER_GPRS | DATA_ACCOUNT_BEARER_WIFI      /* Packet Switching Bearer only */
#define DATA_ACCOUNT_BEARER_CS_ONLY    DATA_ACCOUNT_BEARER_CSD  /* Circuit Switching Bearer only */
#define DATA_ACCOUNT_BEARER_TOTAL   3

extern void InitDataAccountApp(void);
extern void InitializeDataAccountRecords(void);
extern BOOL mmi_dtcnt_get_apn(U8 account_id, S8 *dest, U8 len);
extern BOOL mmi_dtcnt_get_user_pass(U8 account_id, S8 *user,S8 * pass,U8 len);
extern BOOL mmi_dtcnt_get_account_name(U8 account_id, S8 *dest, U8 len);
extern void mmi_dtcnt_select_account(FuncPtrShort callback, U16 AppMenuID, U8 bearers);
extern void mmi_dtcnt_select_account_ext(FuncPtrShort callback, U16 AppMenuID, U8 bearers, U8 account_id);
extern U8 mmi_dtcnt_get_first_account_id(U8 bearers);
extern void DataAccountReadyCheck(FuncPtr ready_callback);
extern void DataAccountGPRSReadyCheck(FuncPtr ready_callback);
extern void EntryDTCNTMain(void);

#ifdef __MMI_GPRS_FEATURES__
extern void InitializeGPRSDataAccountRecords(void);
#endif /* __MMI_GPRS_FEATURES__ */ 

#endif /* _DATA_ACCOUNT_GPROT_H */ 

