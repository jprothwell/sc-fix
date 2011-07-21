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
 * ProfileMgrGProt.h
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
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#include "mmi_features.h"

#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__

#ifndef _PROFILE_MGR_GPROT_H_
#define _PROFILE_MGR_GPROT_H_

/* Check dependence header files exist */
#ifndef _FILE_MGR_H_
#error "FileMgr.h should be included"
#endif

/***************************************
 * Defination
 ***************************************/
#define MAXIMUM_EXT_MELODY_NUM            5//20   //Modified by Jinzh:20070617  原来值为0，导致除了固有的铃音之外别无选择
#define PMG_EXT_MELODY_BEGIN                  (NV_RAM_FILE_EXT_MELODY_ID_BASE)
#define PMG_EXT_MELODY_END                     (PMG_EXT_MELODY_BEGIN + MAXIMUM_EXT_MELODY_NUM-1)

/*
#define MAX_EXT_MELODY_EXTENSION_WIDTH    ((5+1)*ENCODING_LENGTH)
#define MAX_EXT_MELODY_PREFIX_WIDTH       ((31+1)*ENCODING_LENGTH)
*/

#define MAX_EXT_MELODY_PATH_WIDTH        ( ((FMGR_MAX_PATH_LEN>>1)+1) * ENCODING_LENGTH)
#if 0//modified by gdm 081128 fix bug10498 
#define MAX_EXT_MELODY_NAME_WIDTH         ( (FMGR_MAX_FILE_LEN+1) * ENCODING_LENGTH )
#else
#define MAX_EXT_MELODY_NAME_WIDTH        MAX_EXT_MELODY_PATH_WIDTH
#endif
#define MAX_EXT_MELODY_FULLNAME_WIDTH        ( MAX_EXT_MELODY_PATH_WIDTH +\
                                                                                   MAX_EXT_MELODY_NAME_WIDTH - \
                                                                                   ENCODING_LENGTH ) /* Every width has a null termainate character */

/*
* handle short & long filename for ext melody
*/
#define IS_EXTMELODY_SHORTNAME(x) ((x)[1] != 0)
#define MARK_EXTMELODY_SHORTNAME(x) ((x)[1] = 1)
#define UNMARK_EXTMELODY_SHORTNAME(x) ((x)[1] = 0)

/***************************************
 * Type Defination
 ***************************************/
typedef struct {
   U8 filepath[MAX_EXT_MELODY_PATH_WIDTH];
   U8 filename[MAX_EXT_MELODY_NAME_WIDTH];
   U16 id;
} DYNEXTMELODYELEM;

typedef struct {
   U32 ValidBit;
   U8 TotleNum;
   DYNEXTMELODYELEM info[MAXIMUM_EXT_MELODY_NUM];
} DYNEXTMELODYINFO;


typedef enum {
   PMG_ADDFILE_ERR_UNKNOWN,
   PMG_ADDFILE_ERR_SUCCESS,
   PMG_ADDFILE_ERR_STR_TOO_LONG,
   PMG_ADDFILE_ERR_STRFILE_TOO_LONG,
   PMG_ADDFILE_ERR_STRPATH_TOO_LONG,
   PMG_ADDFILE_ERR_SLOT_FULL,
   PMG_ADDFILE_ERR_SLOT_OCCUPIED,
   PMG_ADDFILE_ERR_INVAILD_NAME,
   PMG_ADDFILE_ERR_FILE_TOO_LARGE,
   PMG_ADDFILE_ERR_DUPLICATED,
   PMG_ADDFILE_ERR_DRM_PROHIBTED,

   PMG_ADDFILE_ERR_LAST
     
} PMG_ADDFILE_ERRNO;

typedef enum {
   PMG_FMGR_DEL_UNKNOW,

   PMG_FMGR_DEL_CERTAIN,
   PMG_FMGR_DEL_ALL,

   PMG_FMGR_DEL_LAST
     
} PMG_DELFILE_ACTION;


/***************************************
 * Extern Function Declaration
 ***************************************/
pBOOL PmgIsEmptySlot(U8 slot);
BOOL PmgIsIdExist(U32 soundId);
BOOL PmgQueryFileExistById(U32 soundId);
BOOL PmgQueryFileExistBySlot(U8 slot );
U8 PmgFindFirstEmptySlot(void);
U8 PmgGetExtMelodyNum(void);
U8 PmgGetExtMelodyInfo(DYNEXTMELODYINFO ** ExtMelodInfo);
pBOOL PmgDelExtMelodyByPathFileName(S8 *pathfileName);
void PmgFmgrAddExtMelodyFunc( S8 *pathfileName );
BOOL PmgFmgrDelExtMelodyFunc(S8 *pathfileName, PMG_DELFILE_ACTION action);
PMG_ADDFILE_ERRNO PmgAddExtMelodyByPathFileName(S8 *pathfileName); /* fileName is unicode */
PMG_ADDFILE_ERRNO PmgReplaceExtMelodyBySlotPathFileName(S8 *pathfileName, U8 freeSlot); /* fileName is unicode */
PMG_ADDFILE_ERRNO PmgAddExtMelodyBySlotPathFileName(S8 *pathfileName, U8 freeSlot); /* fileName is unicode */
PMG_ADDFILE_ERRNO PmgSeparateFilePathName( S8 *pathfileName, S8 *pathname, U16 pathsize, S8* filename, U16 filesize);
U8 PmgGetSlotByPathFileName(S8 *pathfileName);
S8* PmgGetFileNameByFilePath( S8 *pathfileName );
void PmgGetExtMelodyRange(U32 *startId, U32*totalId);
pBOOL PmgGetFilePathNameByAudioId(S8 *buf, U16 size, U16 AudioId);
//void PmgSendPlayExtMelodyMsg(S8 *filename);
//void PmgSendStopExtMelodyMsg(void);
void PmgDelExtMelodyBySlot( U8 slot );
pBOOL PmgDelExtMelodyById( U16 id );
U8 PmgGetSlotById( U16 Id );
void PmgConstructPathFileName( S8 *buf, U16 size,  S8* path, S8 *file);
S8 *PmgGetFileExt( S8 *buf );
PMG_ADDFILE_ERRNO PmgCheckImyMidFileSize( S8 *path );
void PmgRemoveFileExt( S8 *buf );  /* Notice! This function will change the buffer context */
void PmgRemoveFileDAFExt( S8 *buf );  /* Notice! This function will change the buffer context */
void PmgSetShortFileName( BOOL is_short );


#endif /* _PROFILE_MGR_GPROT_H_ */

#endif /* __MMI_PROFILE_EXTMELODY_SUPPORT__ */


