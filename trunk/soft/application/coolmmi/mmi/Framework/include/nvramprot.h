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
 *	NVRAMProt.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for NVRAM function prototype.
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
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#ifndef _NVRM_PROT_H
#define _NVRM_PROT_H


#include "mmi_data_types.h"

/***************************************************************************** 
* Define
*****************************************************************************/

#define __NEW_NVRAMINTERFACE__
 
#if 0
#if defined( __NEW_NVRAMINTERFACE__) && defined(MMI_ON_HARDWARE_P)
#define WriteRecord(nFileId,nRecordId,pBuffer,nBufferSize,pError)\
	AccessRecordInt(nFileId,nRecordId,pBuffer,nBufferSize,pError,  MMI_TRUE, __FILE__,__LINE__)

#define ReadRecord(nFileId,nRecordId,pBuffer,nBufferSize,pError)\
	AccessRecordInt(nFileId,nRecordId,pBuffer,nBufferSize,pError, MMI_FALSE, __FILE__,__LINE__)

#define WriteRecordInt(nFileId,nRecordId,pBuffer,nBufferSize,pError, fileName,lineNumber)\
	AccessRecordInt(nFileId,nRecordId,pBuffer,nBufferSize,pError, MMI_TRUE, fileName,lineNumber)

#define ReadRecordInt(nFileId,nRecordId,pBuffer,nBufferSize,pError,fileName,lineNumber)\
	AccessRecordInt(nFileId,nRecordId,pBuffer,nBufferSize,pError,  MMI_FALSE, fileName,lineNumber)

#else
#define WriteRecord(nFileId,nRecordId,pBuffer,nBufferSize,pError)\
	WriteRecordInt(nFileId,nRecordId,pBuffer,nBufferSize,pError, __FILE__,__LINE__)

#define ReadRecord(nFileId,nRecordId,pBuffer,nBufferSize,pError)\
	ReadRecordInt(nFileId,nRecordId,pBuffer,nBufferSize,pError,__FILE__,__LINE__)
#endif

#define ReadValue(nId,pBuffer,nDataType,pError)\
		ReadValueInt(nId,pBuffer,nDataType,pError,__FILE__,__LINE__)


#define WriteValue(nId,pBuffer,nDataType,pError)\
		WriteValueInt(nId,pBuffer,nDataType,pError,__FILE__,__LINE__)


/***************************************************************************** 
* Typedef 
*****************************************************************************/

/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/
extern void NvramDeInit(void);
extern pBOOL NvramInit(void);
extern void SendNVRAMReadReq(U16 nFileId,U16 nRecordId);
extern void SendNVRAMWriteReq(U16 nFileId,U16 nRecordId,void *pBuffer, U16 nBufferSize);

#if defined( __NEW_NVRAMINTERFACE__) && defined(MMI_ON_HARDWARE_P)
extern S32 AccessRecordInt(U16 nFileId,U16 nRecordId, void *pBuffer, U16 nBufferSize,S16 *pError, pBOOL KAL_FALSE, S8* fileName, S32 lineNumber);
#if 0
/* under construction !*/
#endif

#else
extern S32 WriteRecordInt(U16 nFileId,U16 nRecordId,void *pBuffer,U16 nBufferSize,S16 *pError, S8* fileName, S32 lineNumber);

extern S32 ReadRecordInt(U16 nFileId,U16 nRecordId, void *pBuffer, U16 nBufferSize,S16 *pError, S8* filename, S32 lineNumber);
#endif

extern S32 ReadValueInt(U8 nId, void *pBuffer, U8 nDataType , S16 *pError, S8* fileName, S32 lineNumber);

extern S32 WriteValueInt(U8 nId, void *pBuffer, U8 nDataType , S16 *pError, S8* fileName, S32 lineNumber);


extern U8 SetFlashNVRAMCatchData(U8 nDataType, U8 l_DataWriteFlashFlag);
extern U8 SetFlashAllNVRAMCatchData(U8 l_DataWriteFlashFlag);
extern S32 WriteFlashNVRAMCatchData(U8 nDataType);
extern S32 WriteFlashAllNVRAMCatchData(void);

extern U8 IsInNVRAMProcedure(void);
#endif
 
#define WriteRecord(nFileId,nRecordId,pBuffer,nBufferSize,pError)\
	WriteRecordInt(nFileId,nRecordId,pBuffer,nBufferSize,pError, __FILE__,__LINE__)

#define ReadRecord(nFileId,nRecordId,pBuffer,nBufferSize,pError)\
	ReadRecordInt(nFileId,nRecordId,pBuffer,nBufferSize,pError,__FILE__,__LINE__)

#define ReadValue(nId,pBuffer,nDataType,pError)\
		ReadValueInt(nId,pBuffer,nDataType,pError,__FILE__,__LINE__)


#define WriteValue(nId,pBuffer,nDataType,pError)\
		WriteValueInt(nId,pBuffer,nDataType,pError,__FILE__,__LINE__)

extern S32 WriteRecordInt(U16 nFileId,U16 nRecordId,void *pBuffer,U16 nBufferSize,S16 *pError, S8* fileName, S32 lineNumber);

extern S32 ReadRecordInt(U16 nFileId,U16 nRecordId, void *pBuffer, U16 nBufferSize,S16 *pError, S8* filename, S32 lineNumber);
extern S32 ReadValueInt(U8 nId, void *pBuffer, U8 nDataType , S16 *pError, S8* fileName, S32 lineNumber);

extern S32 WriteValueInt(U8 nId, void *pBuffer, U8 nDataType , S16 *pError, S8* fileName, S32 lineNumber);
void initNvramFolder(void);

#define NVRAM_DEFAULT_FOLDER "NVRAM"

#endif 

