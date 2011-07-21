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

/*******************************************************************************
 * Filename:
 * ---------
 *	  VdoPlyStrEnum.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Video Player String ID Enum.
 *
 * Author:
 * -------
 *	  Bear Wang
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 * $Log$
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef _MMI_VDOPLYSTRENUM_H
#define _MMI_VDOPLYSTRENUM_H

typedef enum {
	/* app */
	STR_ID_VDOPLY_APP_NAME	= VDOPLY_BASE +1,	

	/* menu */
	STR_ID_VDOPLY_STORAGE,  //40002
	STR_ID_VDOPLY_STORAGE_PHONE,	
	STR_ID_VDOPLY_STORAGE_MEMORY_CARD,	
	/*+ zhouqin modify for dual t-flash card 20110503*/
    #if defined(DUAL_TFLASH_SUPPORT)
	STR_ID_VDOPLY_STORAGE_MEMORY_SEC_CARD,	
    #endif
    /*- zhouqin modify for dual t-flash card 20110503*/

	/* player */	
	STR_ID_VDOPLY_PLAY, //40005
	STR_ID_VDOPLY_STOP,
	STR_ID_VDOPLY_PAUSE,
	STR_ID_VDOPLY_RESUME,

	/* Notify */
	STR_ID_VDOPLY_NOTIFY_NO_MEMORY_CARD,  //40009
	STR_ID_VDOPLY_NOTIFY_INVALID_VIDEO_FILE,  //40010
	STR_ID_VDOPLY_NOTIFY_WRITE_PROTECTION,  //40011
	STR_ID_VDOPLY_NOTIFY_SNAPSHOT_FAILED  //40012

} STR_ID_VDOPLY_ENUM;


	

#endif /* _MMI_VDOPLYSTRENUM_H*/


