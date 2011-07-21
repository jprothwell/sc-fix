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
 * BTMMIObex.h
 *
 * Project:
 * --------
 *   MMI
 *
 * Description:
 * ------------
 *   
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#ifndef __BT_MMI_OBEX_H__
#define __BT_MMI_OBEX_H__
#ifndef __MMI_FEATURES__
#error "MMI_features.h should be included"
#endif 

//#ifdef __MMI_BT_SEND_SUPPORT__
/***************************************************************************** 
* Constant
*****************************************************************************/
/* Send data bearer */
typedef enum
{
    BTSEND_USE_BUFF,
    BTSEND_USE_FILE
} btsend_obj_enum;

/* Send profile selection */
typedef enum
{
    BTSEND_PROF_OPP = 0x01,
    BTSEND_PROF_FTP = 0x02,
    BTSEND_PROF_BIP = 0x04,
    BTSEND_PROF_ALL = 0xFF
} btsend_prof_enum;

/* Send option */
typedef enum
{
    BTSEND_OPT_NORMAL,
#ifdef __MMI_BT_AUTO_PROMPT_SWITCH__
    BTSEND_OPT_DEFAULT_DEVICE,
#endif
    BTSEND_OPT_END
} btsend_opt_enum;

/* Send result code */
typedef enum
{
    BTSEND_RET_READY,
    BTSEND_RET_SUCCESS,
    BTSEND_RET_FAIL,
    BTSEND_RET_INVALID_PARA,
    BTSEND_RET_SERV_NOT_AVAILABLE,
    BTSEND_RET_DEVICE_BLOCKED,
    BTSEND_RET_BAD_OBJECT,
    BTSEND_RET_USER_ABORT,
    BTSEND_RET_TIMEOUT,
    BTSEND_RET_REJECT,
    BTSEND_RET_DISCONNECT,
    BTSEND_RET_FORBIDDEN,
    BTSEND_RET_UNSUPPORT_FILE_TYPE,
    BTSEND_RET_DONE
} btsend_ret_enum;

typedef enum {
	BTSEND_STATUS_READY,
	BTSEND_STATUS_SUCC,
	BTSEND_STATUS_FAIL,
	BTSEND_STATUS_DONE
} btsend_status_enum;

typedef void (*btsend_callback) (U32, void *);
typedef void (*btsend_notify)(U32 status, void *handle, void *user_data);

/***************************************************************************** 
* Structure
*****************************************************************************/
typedef struct 
{
    U16 id;
    btsend_callback callback;
} btsend_app_struct;

typedef struct
{
    U16 appID;
    btsend_notify fNotify;
    void *userData;
} btsend_app_info_struct;

typedef struct
{
    U8 mime_type;
    U16 mime_subtype;
    S8 * mime_string;
} btsend_obj_type_struct;

typedef struct
{
    btsend_obj_enum obj_type;
    union {
        struct {
            void* addr;
            U32 length;
        } buff;
        struct {
            U16* path;
            MMI_BOOL is_sfn;
        } file;
    } u;
} btsend_obj_data_struct;

typedef struct
{
    U16 * name;
    btsend_obj_type_struct type;
    btsend_obj_data_struct data;
} btsend_obj_struct;

typedef struct {
    U16 total;
    btsend_obj_struct *obj;
} btsend_obj_info_struct;

/***************************************************************************** 
* Prototype
*****************************************************************************/
extern U32 mmi_bt_send_object(btsend_obj_struct* obj,
                            btsend_app_struct* app,
                            btsend_prof_enum prof,
                            btsend_opt_enum option);
extern void* mmi_bt_send_begin(
                btsend_obj_info_struct *objs,
                btsend_app_info_struct *app,
                btsend_prof_enum profile,
                btsend_opt_enum option);
extern U32 mmi_bt_send_obj(
                void *hSend, 
                btsend_obj_struct *obj);
extern U32 mmi_bt_send_end(void *hSend);

//#endif /* __MMI_BT_SEND_SUPPORT__ */

#if defined (__MMI_FTS_SUPPORT__) || defined(__MMI_FTC_SUPPORT__) || defined(__MMI_OPP_SUPPORT__)

/******************************************************************************
    MMI FTP FUNCTIONS
******************************************************************************/
extern void mmi_bt_obex_event_hdlr_init(void);

#endif /* (__MMI_FTS_SUPPORT__) || (__MMI_FTC_SUPPORT__) || (__MMI_OPP_SUPPORT__) */
#endif /* __BT_MMI_OBEX_H__ */ 

