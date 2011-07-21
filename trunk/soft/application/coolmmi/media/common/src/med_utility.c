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
 *	med_utility.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file includes common used functions of media task.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

#ifndef MED_NOT_PRESENT


/*==== INCLUDES =========*/

#include "kal_release.h" 
#include "stack_common.h"  
#include "stack_msgs.h"
#include "app_ltlcom.h"       	/* task message communiction */
/* Buffer Management */
#include "stacklib.h"        	/* basic type for dll, evshed, stacktimer */
#include "event_shed.h"       	/* event scheduler */
#include "stack_timer.h"      	/* stack timer */
/* for tst inject message */
//#include "tst_sap.h" /* wufasong delete 2007.06.13 */


/* global includes */
#include "l1audio.h"
#include "device.h"
#include "resource_audio.h"
#include "nvram_struct.h"
#include "nvram_user_defs.h"
#include "nvram_data_items.h"
#include "custom_nvram_editor_data_item.h"

/* local includes */
#include "med_main.h"
#include "med_global.h"
#include "aud_defs.h"
#include "med_struct.h"
#include "med_api.h"
#include "med_context.h"
#include "fat_fs.h"
#include "med_smalloc.h"
#include "med_utility.h"
#include "med_status.h"   //add by weid

#include "mmi_trace.h"
#include "media_others.h"
#include "oslmemory.h"


HANDLE GetMmiTaskHandle(module_type mod);

typedef struct 
{
	eventid event_id;
	kal_timer_func_ptr callback_func;
	void *arg;
} 
timer_table_struct;

timer_table_struct med_timer_table[MAX_NUM_OF_MED_TIMER];

#ifndef MED_LOW

const kal_uint8 utf8_bytes_per_char[16] = 
{
  1, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 0, 0, 2, 2, 3, 4
};

/*****************************************************************************
* FUNCTION
*     med_util_utf8_to_ucs2
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     kal_uint8 *dest
*     kal_uint32 dest_size (>=4)
*     kal_uint8* src
*     kal_uint32 src_size (>=1) it is used to prevent from infinte loop when src data are not utf8
* RETURNS
*     kal_int32
* GLOBALS AFFECTED
*     
*****************************************************************************/
kal_int32 med_util_utf8_to_ucs2( kal_uint8 *dest, kal_uint32 dest_size, kal_uint8* src, kal_uint32 src_size )
{
	kal_uint8 cnt, c;
	kal_uint16 ucs2;
	kal_uint32 dest_count = 0;
	kal_uint32 src_count = 0;

	dest_size -= ((dest_size % 2) + 2); // make it a even number + null terminator
	
	while( dest_count < dest_size && src_count < src_size && (c = *src) )
	{
		cnt = utf8_bytes_per_char[c >> 4];
		switch( cnt )
		{
		case 1:
			dest[dest_count] = c;
			dest[dest_count+1] = 0;
			dest_count += 2;
			src++;
			src_count++;
			break;
		case 2:
			ucs2 = ((kal_uint16)(c & 0x1F) << 6) | (kal_uint16)(src[1] & 0x3F);
			dest[dest_count] = ucs2 & 0xFF;
			dest[dest_count+1] = ucs2 >> 8;
			dest_count += 2;
			src += 2;
			src_count += 2;
			break;
		case 3:
			ucs2 = ((kal_uint16)(c & 0x0F) << 12) | ((kal_uint16)(src[1] & 0x3F) << 6) | (kal_uint16)(src[2] & 0x3F);
			dest[dest_count] = ucs2 & 0xFF;
			dest[dest_count+1] = ucs2 >> 8;
			dest_count += 2;
			src += 3;
			src_count += 3;
			break;
		case 4:
			src += 4; // skip it since we don't support UCS4
			src_count += 4;
			break;
		case 0:
			src++; // something wrong with the src, skip it
			src_count++;
			break;
		}
	}
	dest[dest_count] = 0;
	dest[dest_count + 1] = 0;
	return dest_count + 2;
}

/*****************************************************************************
* FUNCTION
*     med_util_ucs2_to_utf8
* DESCRIPTION
*     
* IMPACT 
*     
* PARAMETERS
*     kal_uint8 *dest
*     kal_uint32 dest_size (>=1)
*     kal_uint8* src
*     kal_uint32 src_size (>=2)
* RETURNS
*     kal_int32
* GLOBALS AFFECTED
*     
*****************************************************************************/
kal_int32 med_util_ucs2_to_utf8( kal_uint8 *dest, kal_uint32 dest_size, kal_uint8* src, kal_uint32 src_size )
{
	kal_uint8 lb, hb;
	kal_uint32 dest_count = 0;
	kal_uint32 src_count = 0;

	dest_size -= 1; // one byte space for null terminator

	lb = src[0];
	hb = src[1];
	while( dest_count < dest_size && src_count < src_size && // not exceed limited size
		(lb != 0 || hb != 0) ) // src does not encounter null terminator
	{
		if( hb == 0 && (lb & 0x80) == 0 )
			dest[dest_count++] = lb;
		else
		{
			if(( hb & 0xF8)  == 0 )
				dest[dest_count++] = 0xC0 | (hb << 2) | (lb >> 6);
			else
			{
				dest[dest_count++] = 0xE0 | (hb >> 4);
				dest[dest_count++] = 0x80 | ((hb & 0x0F) << 2) | (lb >> 6);
			}
			dest[dest_count++] = 0x80 | (lb & 0x3F);
		}
		src_count += 2;
		lb = src[src_count];
		hb = src[src_count+1];
	}
	dest[dest_count] = 0;
	return dest_count + 1;
}

#endif // MED_LOW

kal_bool med_utility_init(void)
{
   kal_uint16 i;

	mmi_trace(TRUE, "Excute func : mid_utility_init()");
	
   for(i=0;i<MAX_NUM_OF_MED_TIMER;i++)
      med_timer_table[i].event_id = NULL;

   /* init internal memory */
	med_set_int_memory_pool((unsigned char*)med_context_p->int_mem_p, MED_INT_MEM_SIZE);

   /* init external memory */
	med_set_ext_memory_pool((unsigned char*)med_context_p->ext_mem_p, MED_EXT_MEM_SIZE);


   return KAL_TRUE;
}

void med_get_buffer_req_hdlr(ilm_struct *ilm_ptr)
{
   media_get_buffer_req_struct *req_p;

   req_p = (media_get_buffer_req_struct*)ilm_ptr->local_para_ptr;

   if(req_p->location==0)
      *(req_p->buffer_p)=med_alloc_int_mem(req_p->buffer_size);
   else
      *(req_p->buffer_p)=med_alloc_ext_mem(req_p->buffer_size);

   MED_SET_EVENT(MED_EVT_GET_BUFFER);
}

void med_free_buffer_req_hdlr(ilm_struct *ilm_ptr)
{
   media_free_buffer_req_struct *req_p;

   req_p = (media_free_buffer_req_struct*)ilm_ptr->local_para_ptr;
   if(req_p->location==0)
      med_free_int_mem(req_p->buffer_p);
   else
      med_free_ext_mem(req_p->buffer_p);

   MED_SET_EVENT(MED_EVT_FREE_BUFFER);
}


void med_start_base_timer(void *base_timer_ptr, kal_uint32 time_out) 
{
   stack_start_timer((stack_timer_struct *)base_timer_ptr,
                 MED_BASE_TIMER_ID,
                 time_out);
   return;
}


void med_stop_base_timer(void *base_timer_ptr)
{
   stack_stop_timer((stack_timer_struct *)base_timer_ptr);
   return;
}

void med_timer_expiry_callback(void *arg)
{
	kal_uint8 *timer_id = (kal_uint8 *)arg;
	if ((kal_uint32)timer_id < (kal_uint32)MAX_NUM_OF_MED_TIMER)
	{
		med_timer_table[(kal_uint32)(timer_id)].callback_func(med_timer_table[(kal_uint32)(timer_id)].arg);
	}

}

void med_start_timer(kal_uint8 timer_id, kal_uint32 period, kal_timer_func_ptr timer_expiry, void *arg)
{
	med_timer_table[timer_id].callback_func = timer_expiry;
	med_timer_table[timer_id].arg = arg;

	COS_SetTimer(GetMmiTaskHandle(MOD_MED), timer_id, 0, period*16);
	mmi_trace(g_sw_MED,"The start_timer is TRUE");
}

void med_stop_timer(kal_uint8 timer_id)
{

	COS_KillTimer(GetMmiTaskHandle(MOD_MED), timer_id);
	mmi_trace(g_sw_MED,"Excut funs: med_stop_timer is TRUE");
}



void* med_alloc_int_mem(kal_int32 size)
{
#if 0
	void *p = (void*) med_int_smalloc((size_type) size);
#else
	void *p = (void*)OslMalloc(size);
#endif
	if(p==NULL)
	{
		ASSERT(p!=NULL);
	}
	return p;
}

void med_free_int_mem(void** pointer)
{
#if 0
	med_int_sfree((address_t) *pointer);
#else
	OslMfree(*pointer);
#endif
	*pointer=NULL;
}

void* med_alloc_ext_mem(kal_int32 size)
{
	void *p;
	if(size==0)
	{
		return NULL;
	}
#if 0
	p = (void*) med_ext_smalloc((size_type) size);
#else
	p = (void*)OslMalloc(size);
#endif
	//if(p==NULL)
	//{
		//ASSERT(p!=NULL);
	//}
	return p;
}

void med_free_ext_mem(void** pointer)
{
#if 0
	med_ext_sfree((address_t) *pointer);
#else
	OslMfree(*pointer);
#endif
	*pointer=NULL;
}

//void * kal_mem_set ( void*    dest,kal_uint8     value,kal_uint32     size);
//void * kal_mem_cpy(void* dest, const void* src, kal_uint32 size);

kal_int32 med_check_disc_space(kal_wchar* dir_name, kal_int32 free_byte)
{
   kal_int32 result;
   FS_DiskInfo disc_info;
   kal_wchar dir[3];

   kal_mem_set(dir, 0, 6);
   kal_mem_cpy(dir, dir_name, 4);
   /* check the available free space */
   result=MMI_FS_GetDiskInfo((UINT8*)dir, &disc_info, FS_DI_BASIC_INFO|FS_DI_FREE_SPACE);

   if(result&(FS_DI_BASIC_INFO|FS_DI_FREE_SPACE))
   {
      if((disc_info.FreeClusters*disc_info.SectorsPerCluster*disc_info.BytesPerSector)<free_byte)
         return MED_RES_DISC_FULL;
      else
         return MED_RES_OK;
   }
   else
   {
      return MED_RES_NO_DISC;
   }
}

extern S8 *UCS2Strcpy(S8 *strDestination, const S8 *strSource );
extern S32 UCS2Strlen(const S8 *arrOut);
extern U16 UnicodeToAnsii(S8 *pOutBuffer, S8 *pInBuffer );

kal_int16 med_get_media_type(kal_wchar* file_name)
{
//chenhe ++
	S8 ext[32]={0};
	S8 ansiiName[16] = {0};

	UCS2Strcpy(ext, (S8 *)(file_name + UCS2Strlen((UINT8 *)file_name) - 4));
	UnicodeToAnsii(ansiiName, ext);
	
	//TRACE_EF(1, "med_get_media_type>>>>>FileName Len1= %d  FileName Len2= %d",UCS2Strlen(file_name),strlen(ansiiName));
       //TRACE_EF(1, "med_get_media_type>>>>>FileName ext= %s", ansiiName);

	if(strstr(ansiiName,".vm") ||strstr(ansiiName,".VM") )
		return MED_TYPE_GSM_EFR;
	
	if(strstr(ansiiName,".amr") ||strstr(ansiiName,".AMR") )
		return MED_TYPE_AMR;
	
	if(strstr(ansiiName,".mp3") ||strstr(ansiiName,".MP3") )
		return MED_TYPE_DAF;
	/*****U would add others below****/

	if(strstr(ansiiName,".wav") ||strstr(ansiiName,".WAV") )
		return MED_TYPE_WAV;
	
	if(strstr(ansiiName,".wma") ||strstr(ansiiName,".WMA") )
		return MED_TYPE_WMA;
	
	if(strstr(ansiiName,".aac") ||strstr(ansiiName,".AAC") )
		return MED_TYPE_AAC;
	if(strstr(ansiiName,".avi") ||strstr(ansiiName,".AVI") )
	return MED_TYPE_MJPG;
	if(strstr(ansiiName,".mp4") ||strstr(ansiiName,".MP4") )
	return MED_TYPE_MP4;
	if(strstr(ansiiName,".3gp") ||strstr(ansiiName,".3GP") )
	return MED_TYPE_3GP;	
#ifndef TARGET_MIDI_SUPPORT_OFF
	if(strstr(ansiiName,".mid") ||strstr(ansiiName,".MID") )
	return MED_TYPE_MID;
#endif		
    memset(ext,0x0,32);
    memset(ansiiName,0x0,16);
	UCS2Strcpy(ext, (S8 *)(file_name + UCS2Strlen((UINT8 *)file_name) - 3));
	UnicodeToAnsii(ansiiName, ext);

	if(strstr(ansiiName,".rm") ||strstr(ansiiName,".RM"))
		return MED_TYPE_RM;	
    
    memset(ext,0x0,32);
    memset(ansiiName,0x0,16);
	UCS2Strcpy(ext, (S8 *)(file_name + UCS2Strlen((UINT8 *)file_name) - 5));
	UnicodeToAnsii(ansiiName, ext);
	if(strstr(ansiiName,".rmvb") ||strstr(ansiiName,".RMVB"))
		return MED_TYPE_RM;	
	return MED_TYPE_NONE;   
	//chenhe --

#if 0
   kal_wchar *p;
   kal_wchar temp[6];
   kal_uint8 i;

   p = file_name+kal_wstrlen(file_name)-5;

	kal_mem_cpy(temp, p, 2*6);

	for(i = 0 ; i < 5 ; i++)
	{
		if(temp[i] >= 97/*'a'*/ && temp[i] <= 122/*'z'*/)
			temp[i] -= 32/*('a'-'A')*/;
	}

	p=&temp[2];	
   if(kal_wstrcmp(p, L".VM")==0)
      return MED_TYPE_GSM_EFR;
#ifndef MT6205B 
   if(kal_wstrcmp(p, L".AU")==0)
      return MED_TYPE_AU;
#endif

   p=&temp[1];	
   if(kal_wstrcmp(p, L".IMY")==0)
      return MED_TYPE_IMELODY;
   if(kal_wstrcmp(p, L".MID")==0)
      return MED_TYPE_SMF;
   if(kal_wstrcmp(p, L".WAV")==0)
      return MED_TYPE_WAV;
#if defined(YAMAHA_MA3) || defined(MMF_DECODE)
   if(kal_wstrcmp(p, L".MMF")==0)
      return MED_TYPE_MMF;
#endif
   if(kal_wstrcmp(p, L".PCM")==0)
      return MED_TYPE_PCM_8K;
   if(kal_wstrcmp(p, L".DVI")==0)
      return MED_TYPE_DVI_ADPCM;
#ifndef MT6205B 
#ifdef DAF_DECODE
   if(kal_wstrcmp(p, L".MP3")==0)
      return MED_TYPE_DAF;
#endif
   if(kal_wstrcmp(p, L".AMR")==0)
      return MED_TYPE_AMR;
#if defined(AMRWB_DECODE) || defined(AMRWB_ENCODE)
   if(kal_wstrcmp(p, L".AWB")==0)
      return MED_TYPE_AMR_WB;
#endif      
#ifdef AAC_DECODE
   if(kal_wstrcmp(p, L".AAC")==0)
      return MED_TYPE_AAC;
#endif      
#ifdef MP4_CODEC
   if(kal_wstrcmp(p, L".M4A")==0)
      return MED_TYPE_M4A;
   if(kal_wstrcmp(p, L".MP4")==0)
      return MED_TYPE_MP4;
   if(kal_wstrcmp(p, L".3GP")==0)
      return MED_TYPE_3GP;
#endif      
#if defined(JPG_DECODE) || defined(JPG_ENCODE)
   if(kal_wstrcmp(p, L".JPG")==0)
      return MED_TYPE_JPG;
#endif      
#if defined(GIF_DECODE) 
   if(kal_wstrcmp(p, L".JPG")==0)
      return MED_TYPE_GIF;
#endif      
   if(kal_wstrcmp(p, L".AIF")==0)
      return MED_TYPE_AIFF;
   if(kal_wstrcmp(p, L".SND")==0)
      return MED_TYPE_AU;

#ifdef __MED_VR_MOD__
   if(kal_wstrcmp(p, L".VRD")==0)
      return MED_TYPE_VR;
#endif

#ifdef WMA_DECODE
   if(kal_wstrcmp(p, L".WMA")==0)
      return MED_TYPE_WMA;
#endif

   p=&temp[0];	
   if(kal_wstrcmp(p, L".AIFF")==0||
      kal_wstrcmp(p, L".AIFC")==0)
      return MED_TYPE_AIFF;
   if(kal_wstrcmp(p, L".MIDI")==0)
      return MED_TYPE_SMF;
#endif

   return MED_TYPE_NONE;   
#endif //chenhe
   
}


kal_bool med_retrieve_file_path(kal_wchar *file_name, kal_wchar *path)
{
   kal_wchar* p;

   kal_wstrcpy(path, file_name);
   p=path+kal_wstrlen(path);
   while(p>path)
   {
      if (*p==(kal_wchar)_T_MMI'\\')
      {
         *(p+1)=0;
         return KAL_TRUE;
      }
      p--;
   }
   return KAL_FALSE;
}

kal_bool med_remove_file_name(kal_wchar *file_name)
{
   kal_wchar* p;

   p=file_name+kal_wstrlen(file_name);
   while(p>file_name)
   {
      if (*p==(kal_wchar)_T_MMI'\\')
      {
         *(p+1)=0;
         return KAL_TRUE;
      }
      p--;
   }
   return KAL_FALSE;
}

/* crc */

static
const kal_uint32  crc_table[256] = {
  0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL,
  0x076dc419L, 0x706af48fL, 0xe963a535L, 0x9e6495a3L,
  0x0edb8832L, 0x79dcb8a4L, 0xe0d5e91eL, 0x97d2d988L,
  0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L, 0x90bf1d91L,
  0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
  0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L,
  0x136c9856L, 0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL,
  0x14015c4fL, 0x63066cd9L, 0xfa0f3d63L, 0x8d080df5L,

  0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L, 0xa2677172L,
  0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
  0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L,
  0x32d86ce3L, 0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L,
  0x26d930acL, 0x51de003aL, 0xc8d75180L, 0xbfd06116L,
  0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L, 0xb8bda50fL,
  0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
  0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL,

  0x76dc4190L, 0x01db7106L, 0x98d220bcL, 0xefd5102aL,
  0x71b18589L, 0x06b6b51fL, 0x9fbfe4a5L, 0xe8b8d433L,
  0x7807c9a2L, 0x0f00f934L, 0x9609a88eL, 0xe10e9818L,
  0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
  0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL,
  0x6c0695edL, 0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L,
  0x65b0d9c6L, 0x12b7e950L, 0x8bbeb8eaL, 0xfcb9887cL,
  0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L, 0xfbd44c65L,

  0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
  0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL,
  0x4369e96aL, 0x346ed9fcL, 0xad678846L, 0xda60b8d0L,
  0x44042d73L, 0x33031de5L, 0xaa0a4c5fL, 0xdd0d7cc9L,
  0x5005713cL, 0x270241aaL, 0xbe0b1010L, 0xc90c2086L,
  0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
  0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L,
  0x59b33d17L, 0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL,

  0xedb88320L, 0x9abfb3b6L, 0x03b6e20cL, 0x74b1d29aL,
  0xead54739L, 0x9dd277afL, 0x04db2615L, 0x73dc1683L,
  0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
  0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L,
  0xf00f9344L, 0x8708a3d2L, 0x1e01f268L, 0x6906c2feL,
  0xf762575dL, 0x806567cbL, 0x196c3671L, 0x6e6b06e7L,
  0xfed41b76L, 0x89d32be0L, 0x10da7a5aL, 0x67dd4accL,
  0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,

  0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L,
  0xd1bb67f1L, 0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL,
  0xd80d2bdaL, 0xaf0a1b4cL, 0x36034af6L, 0x41047a60L,
  0xdf60efc3L, 0xa867df55L, 0x316e8eefL, 0x4669be79L,
  0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
  0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL,
  0xc5ba3bbeL, 0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L,
  0xc2d7ffa7L, 0xb5d0cf31L, 0x2cd99e8bL, 0x5bdeae1dL,

  0x9b64c2b0L, 0xec63f226L, 0x756aa39cL, 0x026d930aL,
  0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
  0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L,
  0x92d28e9bL, 0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L,
  0x86d3d2d4L, 0xf1d4e242L, 0x68ddb3f8L, 0x1fda836eL,
  0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L, 0x18b74777L,
  0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
  0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L,

  0xa00ae278L, 0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L,
  0xa7672661L, 0xd06016f7L, 0x4969474dL, 0x3e6e77dbL,
  0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L, 0x37d83bf0L,
  0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
  0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L,
  0xbad03605L, 0xcdd70693L, 0x54de5729L, 0x23d967bfL,
  0xb3667a2eL, 0xc4614ab8L, 0x5d681b02L, 0x2a6f2b94L,
  0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL, 0x2d02ef8dL
};

/*
 * NAME:	crc->calculate()
 * DESCRIPTION:	compute CRC-32 value (ISO 3309)
 */
kal_uint32 med_crc_calculate(kal_uint8 const *data, kal_uint32 length)
{
  register kal_uint32 crc;

  for (crc = 0xffffffffL; length >= 8; length -= 8) {
    crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
    crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
    crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
    crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
    crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
    crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
    crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
    crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
  }

  switch (length) {
  case 7: crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
  case 6: crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
  case 5: crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
  case 4: crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
  case 3: crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
  case 2: crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
  case 1: crc = crc_table[(crc ^ *data++) & 0xff] ^ (crc >> 8);
  case 0: break;
  }

  return crc ^ 0xffffffffL;
}

#endif

