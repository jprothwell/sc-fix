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
/**************************************************************

	FILENAME	: GlobalDefines.h

  	PURPOSE		: Decalaration file

	REMARKS		: nil

	AUTHOR		: Kuldeep

	DATE		: Aug 5,03

**************************************************************/

#ifndef __GLOBALDEFINES__H_
#define __GLOBALDEFINES__H_


#define		AND			&&

#define		MAX_FILE_SIZE		2048000
#define	DELETE_MEM(x) if(x) delete x; x=NULL;


#define	DELETE_MEM_ARR(x) if(x) delete [] x; x=NULL;
#define	ALLOCATE_MEM_ARR(p,x,size) p=new x[size];\
								ASSERT(p); 
#define	ALLOCATE_MEM(p,x) p=new x;\
								ASSERT(p); 
#define FREE_LIST(x)	if(x)\
						{		\
							int iCount = ((CPtrList*)x)->GetCount();\
							while(iCount)\
							{\
								LPVOID pNodeInfo = (LPVOID)((CPtrList*)x)->RemoveHead();\
								DELETE_MEM(pNodeInfo);\
								iCount--;\
							}\
							delete x; x=NULL;\
						}

#define	RELEASE_OBJECT(x) if(x) x->Release(); x= NULL;

enum MESSAGE_TYPE{ ERROR_MESSAGE = 0,INFORMATION_MESSAGE,INFORMATION_STRING_MESSAGE};
enum ERR{SUCCESS =0,FAILURE,UNSUFFICIENT_SIZE,CD_NOT_OPENFILE,ALREADY_EXISTS,OVERLAPPING_RANGE};
enum TYPE{INTIGAR =0,FLOT,CARACTER};
//#define	GCML_FILE_NAME	_T("gcml.txt")
//#define	GCML_DIR_NAME	_T(".\\Debug")
//#define	GCML_FILE_NAME	_T("..\\..\\Customer\\Customize\\GCML.txt")
#define	XML_ERROR_FILE_NAME	_T("Report\\XMLERROR.txt")
//#define	XML_CONFIG_INI_FILE_NAME _T("..\\..\\Customer\\Customize\\XMLConfig.ini")
#define INI_FILE_PATH  _T("InitFileSimulator.ini")
typedef	unsigned char	BYTE;
typedef	unsigned short	UINT16;
typedef	short			INT16;
typedef char			INT8;
typedef	BYTE			UINT8;
typedef unsigned long	ULONG;


/*typedef	unsigned char	BYTE;
typedef	unsigned short	UINT16;
typedef	short			INT16;
typedef	BYTE			UINT8;
typedef unsigned long	ULONG;*/
typedef struct MYQUEUE1
{
	short nEventType;
	short nMsgType;
	short size;
	void *nDataPtr;
}MYQUEUE1;

typedef struct INPTRLIST
{
	LONG index;
	MYQUEUE1 stMessage;
}INPTRLIST;


struct PopUpMsgStruct
{
	CString strMsgId;
	CString strMsgResponse;
	CString strMsgValue;
	UINT32 nMsgId;
};

typedef enum
{
	MMI_RAC_OK = 0,
	MMI_RAC_NO_CELL,
	MMI_RAC_LIMITED_SERVICE,
	MMI_RAC_ERROR 
	
} MMIRACRESPONSEENUM;
typedef struct NWSSockRead {
	short nEventType;
	short nMsgType;
	short size;
}NWSSOCKREAD;

#endif
