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
 *  PhoneSetupGprots.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#ifndef PHONESETUP_GPROTS_H_
#define PHONESETUP_GPROTS_H_


/** 
  *   Default encoding type enum
  *   Notice:  This order should match with menu item!!
  **/
typedef enum {
   PHNSET_ENCOD_BIG5 = 0,
   PHNSET_ENCOD_GB2312 = 1,
   PHNSET_ENCOD_UCS2 = 2,

   PHNSET_ENCOD_LAST
} PHNSET_ENCODING_TYPE;

#ifdef __MMI_TOUCH_SCREEN__

/* Calibration state machine */
typedef enum 
{
	CALIBRATION_NULL_STATE,
	CALIBRATION_WELCOME_STATE,
	CALIBRATION_1ST_POINT_STATE, /* Don't change the order of these three enums */
	CALIBRATION_2ND_POINT_STATE,
	CALIBRATION_3RD_POINT_STATE,
    CALIBRATION_VERIFY_STATE,
	CALIBRATION_MAX_STATE
} PHNSET_CALIBRATION_STATE_ENUM;

typedef enum
{
	PHNSET_HANDWRITING_FAST,
	PHNSET_HANDWRITING_MEDIUM,
	PHNSET_HANDWRITING_SLOW
} PHNSET_HANDWRITING_SPEED_ENUM;

extern color_t PhnsetGetHandwritingColor(void);
extern PHNSET_HANDWRITING_SPEED_ENUM PhnsetGetHandwritingSpeed(void);

extern void PhnsetReadNvramCalibrationData(void);
extern void EntryPhnseFactoryDefaultCalibrationScr(void);
BOOL PhnsetNeedFactoryDefaultCalibration(void);

#endif /* __MMI_TOUCH_SCREEN__ */

extern void HighlightPhnsetScr(void);
extern void HighlightPhnsetTimeAndDate(void);


extern U8 PhnsetGetTimeFormat(void);
extern U8 PhnsetGetDateFormat(void);
extern U8 PhnsetGetHomeCity(void);
extern void PhnsetInitTimeNDate(void);
extern void PhnsetSetDTFillInlineStruct( void * );
extern void PhnsetReinitHomeCityIdx(void);
extern void PhnsetMainLCDContrast(U8 level);
extern void PhnsetSubLCDContrast(U8 level);
extern void PhnsetChangeMainLCDContrastValue( U8 level );
extern void PhnsetChangeSubLCDContrastValue( U8 level );
extern void PhnsetSetShowDTStatus( U8 status );
extern MMI_BOOL PhnsetRestoreToNvramContrast(void);
extern MMI_BOOL PhnsetSetLangByNvramValue(void);
#ifdef __MMI_AUTO_LANG_EN__
extern void PhnsetSetLP( U8 LP );
extern U8 PhnsetGetLang(void);
extern void PhnsetPLPInd( U8 *LPArr );
extern U16 PhnsetGetAutoLangIdx(void);
#endif


extern U8 PhnsetGetDefEncodingType(void);
/*****************************************************************************
* FUNCTION
*  mmi_phnset_disable_partial_ss
* DESCRIPTION
*  export function for other application disable partial screen saver
* PARAMETERS
*  void
* RETURNS
*  void
 *****************************************************************************/
extern void mmi_phnset_disable_partial_ss(void);

/*****************************************************************************
* FUNCTION
*  mmi_phnset_enable_partial_ss
* DESCRIPTION
*  export function for other application enable partial screen saver
* PARAMETERS
*  void
* RETURNS
*  void
 *****************************************************************************/
extern void mmi_phnset_enable_partial_ss(void);

#endif

