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
 *	NITZ.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file defines prototypes and structure of NITZ application.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************
	FILENAME	: NITZ.h
  	PURPOSE		: NITZ Inlcude File
 
 
	DATE		: Restructured on Dec-01-2003
**************************************************************/
#ifndef _MMI_NITZ_H
#define _MMI_NITZ_H

#include "mmi_features.h"
#if defined(__MMI_NITZ__)
#include "mmi_data_types.h"
#include "datetimetype.h"


/* 
** Define
*/
#define NITZ_NOTIFY_LENGTH 250
#ifndef NEGATIVE
	#define NEGATIVE 0
#endif

#ifndef POSITIVE
	#define POSITIVE 1
#endif

#define MAX_NW_LEN 24
#define MMI_NITZ_SIM_MASK   0x0F	 
#define MMI_NITZ_STATE_MASK 0x10	/* NITZ state flag */
/* 
** Typedef 
*/
typedef struct
{
    S16 NITZTimeZone;	/* current time zone and save this value in NVRAM */
    MYTIME NITZTime;	/* date and time in NITZ message */
    MYTIME RecvTime;    /* received time of NITZ message */
    S8 *Message;		/* hint string in NITZ popup screen */
    U8 IsNITZPending;	 
    S8 IsAccepted;		/* user accept NITZ request or not */
    U8 NWFullName[MAX_NW_LEN *ENCODING_LENGTH];		/* operator full name in NITZ message */
    U8 NWShortName[MAX_NW_LEN *ENCODING_LENGTH];	/* operator short name in NITZ message */
    BOOL AddCI;			/* receive from NITZ message */
    U8 isValidTZ;		 
#ifdef __MMI_NITZ_TZ_SUPPORT__
    S16 NITZDST;		/* NITZ day light saving parameter information */
    U8 TimeZoneP;		/* update time zone information to mobile or not */
    U8 DSTP;			/* update NITZ day light saving information to mobile or not */
#endif
    U8 state_flag;	/* nitz state */
    U8 near_flag;	/* the two nitz it too closer */    
}nitz_context_struct;

typedef struct
{
    U8 plmn[6];  //#define MAX_PLMN_LEN_MMI 6 in SimDetectionDef.h
    U8 name[MAX_NW_LEN *ENCODING_LENGTH];
    U8 add_ci;
} nitz_nw_name_struct;

typedef enum 
{
	SCR_ID_NITZ = NITZ_BASE+1
}SCR_ID_NITZ_ENUM;

typedef enum
{	
	CHANGE_TIME_CAPTION = NITZ_BASE+1,
	NITZTIMECHANGESTR	
}STR_ID_NITZ_ENUM;

typedef enum
{
	CHANGE_TIME_IMAGE	 = NITZ_BASE+1	
}IMG_ID_NITZ_ENUM;


/* 
** Extern Global Variable
*/
extern nitz_context_struct g_nitz_cntx;

/* 
** Extern Global Function
*/

extern void NITZHandler(void*);
extern void EntryNitzScreen(void);
extern void ExitNitzScreen(void);
extern void NITZIgnore(void);
extern void NITZAccept(void);
extern S16 NITZGetTimeZone(U8);
extern U8 NITZConvertTime(U8);
extern S8 *NITZMessageString(void);
extern void NITZTimeZoneString(S16 timezone, S8* string);


#endif	// __MMI_NITZ__ 
#endif


