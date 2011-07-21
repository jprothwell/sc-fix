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
 * ProfileMgr.c
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#include "mmi_features.h"
#include "mmi_trace.h"

#ifdef __MMI_PROFILE_EXTMELODY_SUPPORT__

/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"    /* data type */
#include "globalconstants.h"
#include "globaldefs.h"
#include "globalstruct.h"
#include "debuginitdef.h"
#include "wgui.h"
#include "wgui_categories.h"
#include "commonscreens.h"  /* display popup */

#include "unicodexdcl.h"        /* unicode transform functions */
#include "nvramtype.h"          /* nvram data type */
#include "nvramprot.h"          /* nvram access fucntions */
#include "nvramenum.h"          /* nvram id define */
#include "filemgr.h"    
#include "profilemgr.h"         /* profile magager general include file*/
#include "profilemgrgprot.h"    /* profile magager extern include file*/
#include "phonebookgprot.h" /* mmi_phb_update_delete_audio/callergroup */
#include "profilegprots.h" /* updatedeleteextmldringtone() */
#include "eventsgprot.h"  /* display popup */
#include "filesystemdef.h"   /* pfopen() */
#undef __NEWSIMULATOR 


/***************************************
 * Globol Variable
 ***************************************/
EXTMELODY_CONTEXT gExtMelodyCnxt;
EXTMELODY_CONTEXT *gpExtMelodyCnxt = &gExtMelodyCnxt;
DYNEXTMELODYINFO  gPmgExtMelodyInfo;

static BOOL is_target_short_filename = FALSE;
void PmgSetShortFileName( BOOL is_short )
{
	is_target_short_filename = is_short;
}

/***************************************
 * Function Body
 ***************************************/
void PmgInitExtMelodyStruct(void)
{
   S16 pError=NVRAM_READ_FAIL;

   ReadRecord( NVRAM_EF_EXT_MELODY_INFO_LID, 1, &gPmgExtMelodyInfo, sizeof(gPmgExtMelodyInfo) , &pError);
   if( pError == NVRAM_READ_FAIL || gPmgExtMelodyInfo.TotleNum == 0xFF || gPmgExtMelodyInfo.ValidBit == 0xFF )
   {
      memset( &gPmgExtMelodyInfo, 0, sizeof(gPmgExtMelodyInfo) );
      WriteRecord(NVRAM_EF_EXT_MELODY_INFO_LID, 1, &gPmgExtMelodyInfo, sizeof(gPmgExtMelodyInfo) , &pError);
   }
   
#if 0 /* Scan all files to see if exist */
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /* 0 */

}

void PmgFmgrAddExtMelodyFunc( S8 *pathfileName )
{
   PMG_ADDFILE_ERRNO err;
   
   if( PmgCheckImyMidFileSize(pathfileName) == PMG_ADDFILE_ERR_FILE_TOO_LARGE )
      err = PMG_ADDFILE_ERR_FILE_TOO_LARGE;
   else
      err = PmgAddExtMelodyByPathFileName( pathfileName );
   	  mmi_trace(g_sw_FMGR, "chenxia!!! fun:PmgFmgrAddExtMelodyFunc; return value is %d", err);
   
   switch( err )
   {
      case PMG_ADDFILE_ERR_SUCCESS:
         DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), 
                                                 IMG_GLOBAL_ACTIVATED, 1,UI_POPUP_NOTIFYDURATION_TIME,SUCCESS_TONE); 
         break;
      case PMG_ADDFILE_ERR_STR_TOO_LONG:
      case PMG_ADDFILE_ERR_STRFILE_TOO_LONG:
      case PMG_ADDFILE_ERR_STRPATH_TOO_LONG:
          DisplayPopup((PU8)GetString(STR_PROFILES_RING_TONE_SETUP_NAMETOOLONG), \
                                                IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE); 
         break;
      case PMG_ADDFILE_ERR_SLOT_FULL:
          /*Jinzh Modify Start For 6662 Ver: TBM780  on 2007-9-13 11:51 */
          DisplayPopup((PU8)GetString(STR_PROFILES_RING_TONE_SETUP_LISTFULL), \
                                                IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,ERROR_TONE/*WARNING_TONE*/);
          /*Jinzh Modify End  For 6662 Ver: TBM780  on 2007-9-13 11:51 */
         break;

      case PMG_ADDFILE_ERR_FILE_TOO_LARGE:
          DisplayPopup((PU8)GetString(STR_PROFILES_RING_TONE_SETUP_FILETOOLARGE), \
                                                IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE); 
          break;

      case PMG_ADDFILE_ERR_DUPLICATED:
          /*Jinzh Modify Start For 6662 Ver: TBM780  on 2007-9-13 11:50 */
          DisplayPopup((PU8)GetString(STR_PROFILES_RING_TONE_SETUP_DUPLICATED), \
                                                IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,ERROR_TONE/*WARNING_TONE*/);
          /*Jinzh Modify End  For 6662 Ver: TBM780  on 2007-9-13 11:50 */
         break;

      default:
          DisplayPopup((PU8)GetString(STR_GLOBAL_UNFINISHED), \
                                                IMG_GLOBAL_UNFINISHED, 1,UI_POPUP_NOTIFYDURATION_TIME,WARNING_TONE); 
         break;
   }

}

PMG_ADDFILE_ERRNO PmgAddExtMelodyByPathFileName(S8 *pathfileName) /* fileName is unicode */
{
   U8 freeSlot=0;
   freeSlot = PmgFindFirstEmptySlot();
   mmi_trace(g_sw_FMGR, "chenxia!!! fun:PmgAddExtMelodyByPathFileName; freeSlot is %d", freeSlot);
   if( freeSlot == PMG_INVAILD_SLOT )
      return PMG_ADDFILE_ERR_SLOT_FULL;
   else
      return PmgAddExtMelodyBySlotPathFileName(pathfileName,  freeSlot);
   
}

PMG_ADDFILE_ERRNO PmgReplaceExtMelodyBySlotPathFileName(S8 *pathfileName, U8 freeSlot) /* fileName is unicode */
{
   //U8 freeSlot=0;
   S16 pError;
   U8 i;
   PMG_ADDFILE_ERRNO res=MMI_FALSE;
   DYNEXTMELODYELEM target;

   if( pfnUnicodeStrlen( pathfileName ) >= (MAX_EXT_MELODY_FULLNAME_WIDTH/ENCODING_LENGTH) )
      return PMG_ADDFILE_ERR_STR_TOO_LONG;
   res = PmgSeparateFilePathName( (S8 *)pathfileName, (S8 *) target.filepath, sizeof(target.filepath), (S8 *) target.filename, sizeof(target.filename) );
   if( res != PMG_ADDFILE_ERR_SUCCESS )
      return res;

    
   if( is_target_short_filename )
      MARK_EXTMELODY_SHORTNAME( target.filepath );

   /* Check Duplicate */
   for( i=0; i< MAXIMUM_EXT_MELODY_NUM; i++ )
   {
		if( (i != freeSlot) 
			&& (PmgIsEmptySlot( i ) == MMI_FALSE )
			&& (pfnUnicodeStrcmp( (S8 *)gPmgExtMelodyInfo.info[i].filepath, (S8 *)target.filepath ) == 0 )
			&& (pfnUnicodeStrcmp( (S8 *)gPmgExtMelodyInfo.info[i].filename, (S8 *)target.filename ) == 0 )
			)
		{
		   return PMG_ADDFILE_ERR_DUPLICATED;
		}
   }

   pfnUnicodeStrcpy( (S8*)gPmgExtMelodyInfo.info[freeSlot].filepath, (S8*)target.filepath );
   pfnUnicodeStrcpy( (S8*)gPmgExtMelodyInfo.info[freeSlot].filename, (S8*)target.filename );
   
   PMG_ASSERT( NVRAM_EF_EXT_MELODY_INFO_SIZE == sizeof(gPmgExtMelodyInfo) );
   WriteRecord(NVRAM_EF_EXT_MELODY_INFO_LID, 1, &gPmgExtMelodyInfo, sizeof(gPmgExtMelodyInfo) , &pError);
   PMG_ASSERT( pError == NVRAM_WRITE_SUCCESS );
   
   return PMG_ADDFILE_ERR_SUCCESS;
}

PMG_ADDFILE_ERRNO PmgAddExtMelodyBySlotPathFileName(S8 *pathfileName, U8 freeSlot) /* fileName is unicode */
{
   //U8 freeSlot=0;
   S16 pError;
   U8 i;
   PMG_ADDFILE_ERRNO res=MMI_FALSE;
   
   if( PmgGetExtMelodyNum() >= MAXIMUM_EXT_MELODY_NUM )
      return PMG_ADDFILE_ERR_SLOT_FULL;

   if( pfnUnicodeStrlen( pathfileName ) >= (MAX_EXT_MELODY_FULLNAME_WIDTH/ENCODING_LENGTH) )
      return PMG_ADDFILE_ERR_STR_TOO_LONG;

   if( PmgIsEmptySlot( freeSlot ) == MMI_FALSE )
      return PMG_ADDFILE_ERR_SLOT_OCCUPIED;
   
   res = PmgSeparateFilePathName( (S8 *)pathfileName,
                            (S8 *) gPmgExtMelodyInfo.info[freeSlot].filepath, sizeof(gPmgExtMelodyInfo.info[freeSlot].filepath), \
                            (S8 *) gPmgExtMelodyInfo.info[freeSlot].filename, sizeof(gPmgExtMelodyInfo.info[freeSlot].filename) \
                           );
   mmi_trace(g_sw_FMGR, "chenxia!!! fun:PmgAddExtMelodyBySlotPathFileName; res is %d", res);

    
   if( is_target_short_filename )
      MARK_EXTMELODY_SHORTNAME( gPmgExtMelodyInfo.info[freeSlot].filepath );

   if( res != PMG_ADDFILE_ERR_SUCCESS )
      return res;
   
   /* Check Duplicate */
   for( i=0; i< MAXIMUM_EXT_MELODY_NUM; i++ )
   {
		if( (i != freeSlot) 
			&& (PmgIsEmptySlot( i ) == MMI_FALSE )
			&& (pfnUnicodeStrcmp( (S8 *)gPmgExtMelodyInfo.info[i].filepath, (S8 *)gPmgExtMelodyInfo.info[freeSlot].filepath ) == 0 )
			&& (pfnUnicodeStrcmp( (S8 *)gPmgExtMelodyInfo.info[i].filename, (S8 *)gPmgExtMelodyInfo.info[freeSlot].filename ) == 0 )
			)
		{
		   return PMG_ADDFILE_ERR_DUPLICATED;
		}
   }
   	
   
   gPmgExtMelodyInfo.info[freeSlot].id = PMG_EXT_MELODY_BEGIN+freeSlot;
   gPmgExtMelodyInfo.TotleNum++;

   PMG_BIT_ON( gPmgExtMelodyInfo.ValidBit, freeSlot );
   mmi_trace(g_sw_FMGR, "chenxia!!! freeSlot = %d;ValidBit = %d;", freeSlot,gPmgExtMelodyInfo.ValidBit);
   
   PMG_ASSERT( NVRAM_EF_EXT_MELODY_INFO_SIZE == sizeof(gPmgExtMelodyInfo) );
   WriteRecord(NVRAM_EF_EXT_MELODY_INFO_LID, 1, &gPmgExtMelodyInfo, sizeof(gPmgExtMelodyInfo) , &pError);
   mmi_trace(g_sw_FMGR, "chenxia!!! sizeof(gPmgExtMelodyInfo) = %d;pError = %d;NVRAM_WRITE_SUCCESS = %d", sizeof(gPmgExtMelodyInfo),pError,NVRAM_WRITE_SUCCESS);
   PMG_ASSERT( pError == NVRAM_WRITE_SUCCESS );
   
   return PMG_ADDFILE_ERR_SUCCESS;
}

void PmgDelExtMelodyBySlot( U8 slot )  /* root function for delete */
{
   S16 pError;
   if( gPmgExtMelodyInfo.TotleNum > 0 )
      gPmgExtMelodyInfo.TotleNum--;
   else
      return;

   /* Update phone book entries */
   mmi_phb_update_delete_audio(gPmgExtMelodyInfo.info[slot].id);
   mmi_phb_update_delete_audio_caller_group(gPmgExtMelodyInfo.info[slot].id);
   
   /* set default tone when selected extented Melody Ring tone is deleted. */
   UpdateDeleteExtMldToneSetup( (U16)(PMG_EXT_MELODY_BEGIN+slot) );

   /* Delete solt mask */
   PMG_BIT_OFF(gPmgExtMelodyInfo.ValidBit, slot);

   /* Delete data in nvram */
   memset( &gPmgExtMelodyInfo.info[slot], 0, sizeof(gPmgExtMelodyInfo.info[slot]) );
   PMG_ASSERT( NVRAM_EF_EXT_MELODY_INFO_SIZE == sizeof(gPmgExtMelodyInfo) );
   WriteRecord(NVRAM_EF_EXT_MELODY_INFO_LID, 1, &gPmgExtMelodyInfo, sizeof(gPmgExtMelodyInfo) , &pError);
   PMG_ASSERT( pError == NVRAM_WRITE_SUCCESS );

}

pBOOL PmgDelExtMelodyById( U16 id ) /* Finally, call PmgDelExtMelodyBySlot() */
{
   U8 slot = PmgGetSlotById( id );
   
   if( slot == PMG_INVAILD_SLOT )
      return MMI_FALSE;
   else
   {
      PmgDelExtMelodyBySlot( slot );
      return MMI_TRUE;
   }
   
}

pBOOL PmgDelExtMelodyByPathFileName(S8 *pathfileName) /* Finally, call PmgDelExtMelodyBySlot() */
{
   U8 slot=0;
  
   if( (pfnUnicodeStrlen( pathfileName ) >= (MAX_EXT_MELODY_FULLNAME_WIDTH/ENCODING_LENGTH) )
	   || (gPmgExtMelodyInfo.TotleNum == 0) )
      return MMI_FALSE;

   slot = PmgGetSlotByPathFileName(pathfileName);
   if( slot == PMG_INVAILD_SLOT )
      return MMI_FALSE;
   else
      PmgDelExtMelodyBySlot( slot );

   return MMI_TRUE;
}

extern UINT8* UniPathUpr(UINT8* pUniPath);
BOOL PmgFmgrDelExtMelodyFunc(S8 *pathfileName, PMG_DELFILE_ACTION action) /* Finally, call PmgDelExtMelodyBySlot() */
{
      BOOL res=MMI_TRUE;
	switch(action)
	{
		case PMG_FMGR_DEL_CERTAIN:
		   res =  PmgDelExtMelodyByPathFileName( pathfileName );
		   break;
		case PMG_FMGR_DEL_ALL:
	       {
		    U8 i;
		    for( i=0; i< MAXIMUM_EXT_MELODY_NUM; i++)
		    {
			    /*Jinzh Modify Start For 6738 Ver: TBM780  on 2007-9-24 14:44 */
			    S8 pathfileNameTemp[FMGR_MAX_PATH_LEN*ENCODING_LENGTH+ENCODING_LENGTH] = {0};
				S8 extMelodyPathName[MAX_EXT_MELODY_PATH_WIDTH] = {0};
				
				pfnUnicodeStrcpy(pathfileNameTemp,pathfileName);
				UniPathUpr(pathfileNameTemp);
				
				pfnUnicodeStrcpy(extMelodyPathName,(S8 *)gPmgExtMelodyInfo.info[i].filepath);
				pfnUnicodeStrcat(extMelodyPathName,(S8 *)L"/");
				
				//if(pathfileName[0] == gPmgExtMelodyInfo.info[i].filepath[0] && 
					//pfnUnicodeStrcmp( pathfileName+2,(S8 *)(gPmgExtMelodyInfo.info[i].filepath+2) ) == 0 )
				if(
				      (pfnUnicodeStrcmp(pathfileNameTemp,(S8 *)gPmgExtMelodyInfo.info[i].filepath) == 0) 
				   || (pfnUnicodeStrcmp(pathfileNameTemp,(S8 *)extMelodyPathName) == 0)
				  )				
			    /*Jinzh Modify End  For 6738 Ver: TBM780  on 2007-9-24 14:44 */
		        {
		           if( PmgDelExtMelodyById( gPmgExtMelodyInfo.info[i].id ) == MMI_FALSE )
			       res = MMI_FALSE;
		        }
		    }
		    break;
	       }
		default:
		   PMG_ASSERT(0);
		   break;
	}
	return res;
}


U8 PmgGetExtMelodyInfo(DYNEXTMELODYINFO ** ExtMelodInfo)
{
   *ExtMelodInfo = &gPmgExtMelodyInfo;
   return gPmgExtMelodyInfo.TotleNum;
}


U8 PmgGetExtMelodyNum(void)
{
   return   gPmgExtMelodyInfo.TotleNum;
}

BOOL PmgIsIdExist(U32 soundId)
{
   U8 i;

   if(soundId>=PMG_EXT_MELODY_BEGIN && soundId<=PMG_EXT_MELODY_END)
   {
      for(i=0; i<MAXIMUM_EXT_MELODY_NUM; i++ )
         if( gPmgExtMelodyInfo.info[i].id == soundId)
            return MMI_TRUE;
   }
   else
      return MMI_TRUE;

   return MMI_FALSE;
}

BOOL PmgQueryFileExistById(U32 soundId)
{
   U8 i;

   if(soundId>=PMG_EXT_MELODY_BEGIN && soundId<=PMG_EXT_MELODY_END)
   {
      for(i=0; i<MAXIMUM_EXT_MELODY_NUM; i++ )
         if( gPmgExtMelodyInfo.info[i].id == soundId)
            return PmgQueryFileExistBySlot( i );
   }
   else
      return MMI_TRUE;

   return MMI_FALSE;
}

BOOL PmgQueryFileExistBySlot(U8 slot )
{
   S8 buf[MAX_EXT_MELODY_FULLNAME_WIDTH];
   FILE_HANDLE h;

   if( slot > MAXIMUM_EXT_MELODY_NUM )
      return MMI_FALSE;

   PmgConstructPathFileName(buf, sizeof(buf), (S8 *) gPmgExtMelodyInfo.info[slot].filepath, (S8 *) gPmgExtMelodyInfo.info[slot].filename );
   //h= pfopen(buf, PFS_READ, 0);
   h = MMI_FS_Open((UINT8 *) buf, FS_READ_ONLY);   //changed by jinzh:20070726

   if( h  >= 0 ) //changed by jinzh:20070726
   {
      //pfclose(h);
       MMI_FS_Close(h);	//changed by jinzh:20070726
      return MMI_TRUE;
   }
   /*jinzh Modify Start For 6838 Ver: tbm780  on 2007-10-26 14:46 */
   else
   {
   	 return MMI_FALSE; 
   }
   /*jinzh Modify End  For 6838 Ver: tbm780  on 2007-10-26 14:46 */
}

S8 *PmgGetFileExt( S8 *buf )
{
   U16 len = (U16)pfnUnicodeStrlen( buf );
   S16 *pbuf = (S16 *)buf;
   S16 deter = '.';

   while( len > 0 )
   {
      if( memcmp( (pbuf+len), &deter, 2) == 0 )
      {
         return (S8 *)(pbuf+len+1);
      }
      len--;
   }
   return NULL;
   
}

PMG_ADDFILE_ERRNO PmgCheckImyMidFileSize( S8 *path )
{
#ifdef MMI_ON_HARDWARE_P 
      S8 *file_ext_p;
      file_ext_p = PmgGetFileExt( path );
      if( pfnUnicodeStrncmp( file_ext_p, (S8 *)L"mid", 3 ) == 0  || pfnUnicodeStrncmp( file_ext_p, (S8 *)L"MID", 3 ) == 0 \
         || pfnUnicodeStrncmp( file_ext_p, (S8 *)L"imy", 3) == 0 || pfnUnicodeStrncmp( file_ext_p, (S8 *)L"IMY", 3 ) == 0 )
      {
         FS_HANDLE h;
         INT32 file_size=0; //U32 file_size=0;
            
         h = MMI_FS_Open( (U8*)path, FS_READ_ONLY);
         if( h >= 0 )
         {
            MMI_FS_GetFileSize(h, &file_size );
            MMI_FS_Close(h);
         }
         else
            return PMG_ADDFILE_ERR_UNKNOWN;

         if( file_size > PMG_IMY_MID_SIZE_LIMIT /* 64 K */ )
            return PMG_ADDFILE_ERR_FILE_TOO_LARGE;
      }
#endif
        return PMG_ADDFILE_ERR_SUCCESS;
}


void PmgRemoveFileExt( S8 *buf )  /* Notice! This function will change the buffer context */
{
   U16 len = (U16)pfnUnicodeStrlen( buf );
   S16 *pbuf = (S16 *)buf;
   S16 deter = '.';

   while( len > 0 )
   {
      if( memcmp( (pbuf+len), &deter, 2) == 0 )
      {
         *(pbuf+len) = 0x0000;
         break;
      }
      len--;
   }
}

void PmgRemoveFileDAFExt( S8 *buf )  /* Notice! This function will change the buffer context */
{
   U16 len = (U16)pfnUnicodeStrlen( buf );
   S16 *pbuf = (S16 *)buf;
   S16 deter = '.';
   S16 ExtWord_U[5];
   S16 ExtWord_L[5];

   memset( ExtWord_U, 0, sizeof( ExtWord_U ) );
   memset( ExtWord_L, 0, sizeof( ExtWord_L ) );
   ExtWord_U[0] = 0x002E;   ExtWord_L[0] = 0x002E;
   ExtWord_U[1] = 0x004D;   ExtWord_L[1] = 0x006D;
   ExtWord_U[2] = 0x0050;   ExtWord_L[2] = 0x0070;
   ExtWord_U[3] = 0x0033;   ExtWord_L[3] = 0x0033;
   ExtWord_U[4] = 0x0000;   ExtWord_L[4] = 0x0000;

   while( len > 0 )
   {
      if( memcmp( (pbuf+len), &deter, 2) == 0 )
      {
         if( (pfnUnicodeStrncmp( (S8 *)(pbuf+len),  (S8 *) ExtWord_U,  4 ) == 0 )
            || (pfnUnicodeStrncmp( (S8 *)(pbuf+len), (S8 *) ExtWord_L,  4 ) == 0)  )
            *(pbuf+len) = 0x0000;
         break;
      }
      len--;
   }
}



void PmgConstructPathFileName( S8 *buf, U16 size,  S8* path, S8 *file)
{
	if( size < MAX_EXT_MELODY_FULLNAME_WIDTH )
	{
		buf[0] = 0x0; buf[1] = 0x0;
		return;
	}
	pfnUnicodeStrcpy( buf,  (S8 *) path );
	//pfnUnicodeStrcat( buf,  (S8 *) L"\\" );
	pfnUnicodeStrcat( buf,  (S8 *) L"/" );		//Changed by jinzh:20070726
	pfnUnicodeStrcat( buf,  (S8 *) file );
	UNMARK_EXTMELODY_SHORTNAME( buf );
}

U8 PmgGetSlotById( U16 Id )
{
   U8 i;
   
   if( (Id < PMG_EXT_MELODY_BEGIN) ||( Id > PMG_EXT_MELODY_BEGIN+ MAXIMUM_EXT_MELODY_NUM) )
      return MMI_FALSE;

   for( i=0; i<  MAXIMUM_EXT_MELODY_NUM; i++ )
   {
      if( gPmgExtMelodyInfo.info[i].id == Id )
         return i;
   }

   return PMG_INVAILD_SLOT;
   
}

U8 PmgGetSlotByPathFileName(S8 *pathfileName)
{
   U8 i;
   S8 tmpStr[MAX_EXT_MELODY_FULLNAME_WIDTH];

   if( pfnUnicodeStrlen( pathfileName ) >= (MAX_EXT_MELODY_FULLNAME_WIDTH ) )
      return MMI_FALSE;

   for( i=0; i<MAXIMUM_EXT_MELODY_NUM; i++)
   {
         PmgConstructPathFileName(tmpStr, sizeof(tmpStr), (S8 *) gPmgExtMelodyInfo.info[i].filepath, (S8 *) gPmgExtMelodyInfo.info[i].filename );
         if( pfnUnicodeStrcmp(pathfileName, tmpStr ) == 0 )
            return i;
   }
   return PMG_INVAILD_SLOT;

}

pBOOL PmgGetFilePathNameByAudioId(S8 *buf, U16 size, U16 AudioId)
{
   U8 i=0;

   if( size < MAX_EXT_MELODY_FULLNAME_WIDTH )
      return MMI_FALSE;
   
   memset( buf, 0, size);
   for( i=0; i<MAXIMUM_EXT_MELODY_NUM; i++)
   {
      if( gPmgExtMelodyInfo.info[i].id == AudioId )
         {
            PmgConstructPathFileName(buf, size, (S8 *) gPmgExtMelodyInfo.info[i].filepath, (S8 *) gPmgExtMelodyInfo.info[i].filename );
            return MMI_TRUE;
         }
   }

   return MMI_FALSE;
}


S8* PmgGetFileNameByFilePath( S8 *pathfileName )
{
   U16 NameLength = (U16)pfnUnicodeStrlen( pathfileName );
   S16 *UniPathName = (S16 *) pathfileName;
   //U16 deter = '\\';
   U16 deter = '/';   //Changed by jinzh:20070726
   
   if( NameLength >= MAX_EXT_MELODY_NAME_WIDTH/ENCODING_LENGTH  )
      return NULL;

   if( (NameLength & 0x1) == 1 )
      return NULL;

   while( NameLength > 0 )
   {
      NameLength-=2;
      if( memcmp( UniPathName+NameLength, &deter, 2 ) == 0 )
         break;
   }
   return (pathfileName+NameLength);

}

PMG_ADDFILE_ERRNO PmgSeparateFilePathName( S8 *pathfileName, S8 *pathname, U16 pathsize, S8* filename, U16 filesize)
{
   U16 i=0, j=0, StoredNameLength, NameLength;
   U16 deter = '/'; //added by liwh
   mmi_trace(g_sw_FMGR, "chenxia!!! fun:PmgSeparateFilePathName; pathsize is %d; filesize is %d", pathsize, filesize);

   StoredNameLength =NameLength = (pfnUnicodeStrlen( pathfileName )+1) * ENCODING_LENGTH;
   mmi_trace(g_sw_FMGR, "chenxia!!! fun:PmgSeparateFilePathName; StoredNameLength is %d; NameLength is %d, ENCODING_LENGTH is %d", StoredNameLength, NameLength, ENCODING_LENGTH);   
   if( NameLength >= (MAX_EXT_MELODY_FULLNAME_WIDTH) ) /* Unicode byte */
      return PMG_ADDFILE_ERR_STR_TOO_LONG;

   if( (NameLength & 0x1) == 1 )
      return PMG_ADDFILE_ERR_INVAILD_NAME;

   while( NameLength > 0 )
   {
      NameLength-=2;
      if( memcmp( pathfileName+NameLength, &deter, 2 ) == 0 )
            break;
    }
   if( NameLength == 0 )
      return PMG_ADDFILE_ERR_INVAILD_NAME;
   else
   {
       if( pathsize < NameLength  )
         return PMG_ADDFILE_ERR_STRPATH_TOO_LONG;
       
       if( filesize < (StoredNameLength-NameLength ) )
         return PMG_ADDFILE_ERR_STRFILE_TOO_LONG;
       
       for( i=0; i< StoredNameLength; i++ )
       {
         if( i<(NameLength+ENCODING_LENGTH) )
            *(pathname+i) = *(pathfileName+i);
         else
            *(filename+j++) = *(pathfileName+i); /* filename has been terminated because it is copied from pathfilename */
       }
       *(pathname+NameLength) = 0x0;
       *(pathname+NameLength+1) = 0x0;
    }
  
   return PMG_ADDFILE_ERR_SUCCESS;

}


U8 PmgFindFirstEmptySlot(void)
{
   U8 i;
   for( i=0;  i < (sizeof(gPmgExtMelodyInfo.ValidBit)*8); i++ )
   {
      if( PMG_BIT_TEST(gPmgExtMelodyInfo.ValidBit,  i) == 0 )
         break;
   }
   if( i >= MAXIMUM_EXT_MELODY_NUM )
      i=PMG_INVAILD_SLOT; //i= (MAXIMUM_EXT_MELODY_NUM - 1); /* zero based */

   return i; 
}

pBOOL PmgIsEmptySlot(U8 slot)  /* slot is zero based */
{
   if( PMG_BIT_TEST(gPmgExtMelodyInfo.ValidBit,  slot) == 0 )
      return MMI_TRUE;
   else
      return MMI_FALSE;
}

void PmgGetExtMelodyRange(U32 *startId, U32*totalId)
{
	*startId = PMG_EXT_MELODY_BEGIN;
	*totalId = PMG_EXT_MELODY_BEGIN + MAXIMUM_EXT_MELODY_NUM;
}


//---- Temporery solution -----------------------------------------------------
#if 0
/* under construction !*/
#ifdef MMI_ON_HARDWARE_P
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#ifdef MMI_ON_HARDWARE_P
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#ifdef MMI_ON_HARDWARE_P
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif   
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#ifdef MMI_ON_HARDWARE_P
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif   
/* under construction !*/
#endif /* 0 */
//---- End of Temporery solution -----------------------------------------------------

#endif /* __MMI_PROFILE_EXTMELODY_SUPPORT__ */

