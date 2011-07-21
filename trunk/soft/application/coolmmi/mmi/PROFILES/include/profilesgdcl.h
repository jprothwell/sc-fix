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
 * ProfilesGdcl.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Profiles application global variables
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

/**************************************************************

	FILENAME	: ProfilesGdcl.h

  	PURPOSE		: Profiles application global variables

 

	AUTHOR		: Ashima Bajaj

	DATE		: 

**************************************************************/
#ifndef _PIXCOM_PROFILESGDCL_H
#define _PIXCOM_PROFILESGDCL_H

S32 gcurrHiliteIndexOne = 0;					/* Current hilite index value */
//S32 gcurrHiliteIndextwo = 0;
U16 g_profiles_melody_list_index = 0;
S32 gcommonvolumehandler = 0;
U8 gprofilenvram = 0;
//S8 gprofileNameArr[MAX_ELEMENTS-2][20 * ENCODING_LENGTH]; //array for profilenames
U8  gSec = 0 ;
U8 gprofileId = 0,gactivatedprofile = 0;
U8 gcurrprofileID = 0;
U8 glastactivatedprofile =0;
S8 tempstring[52];
S32 gringvol_level = 0;
S32 gkeypadvol_level = 0;
S32 gearphonevol_level = 0;
U8 fieldStates[3];
U8 fieldStatesExtraTone[4];
//U8 gextratonestring[40][40];
S32 gcommonvolumelevel=0;
U8 gpopupToneId;
U16 gcurrentaudioId=0;
U16 glastaudioId=0;
U8 glastColorID = 0;
U8 gcurrentColorId = 0;
U16 gstartRingId;
U16 gtotalRingId;
U16 gstartMidiId;
U16 gtotalMidiId;
U16 gstartGeneralToneId;
U16 gtotalGeneralToneId;
U16 gstartMiscToneId;
U16 gtotalMiscToneId;

#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__
U32 gstartExtMelodyId;
U32 gtotalExtMelodyId;
#endif

PROFILE gprofiles[MAX_ELEMENTS];
PROFILE gcurrentprofile;
pBOOL gloudSpeaker = 0;
U8 gprofileheadset= 0;
U8 gloudsplastprofile = 0;
pBOOL gPlayReqToneFlag=TRUE;

#endif




