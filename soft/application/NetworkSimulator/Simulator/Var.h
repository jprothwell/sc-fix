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

	FILENAME	: Value.h

  	PURPOSE		: Header file for CVar class

	REMARKS		: nil

	AUTHOR		: Vikram

	DATE		: 10 oct

**************************************************************/


#if !defined(AFX_VAR_H__F65C1A86_52EC_4195_8CCE_D15E9CCA62D7__INCLUDED_)
#define AFX_VAR_H__F65C1A86_52EC_4195_8CCE_D15E9CCA62D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
enum Var_DataType{
	TYPE_INT=0,
	TYPE_CHAR,
	TYPE_STRING,
	TYPE_INT_ARRAY,
	TYPE_CHAR_ARRAY
}; 

#include	"DataType.h"
class CVar  
{
public:
	CVar();
	virtual ~CVar();

	void SetName(CString csName){m_csName = csName;}
	void SetDataType(CDataType	dataType){m_dataType = dataType;}
	void SetCount(UINT32 nCount){m_nCount = nCount;}
	void SetValueSize(UINT32 nValSize){m_nValSize = nValSize;}
	void SetDescription(CString csDescription){m_csDescription = csDescription;}

	CString GetName(){return m_csName;}
	CDataType& GetDataType(){return m_dataType;}
	UINT32 GetCount(){return m_nCount;}
	UINT32 GetValueSize(){return m_nValSize;}
	CString GetDescription(){return m_csDescription;}

private:
	CString m_csName;
	CDataType	m_dataType;
	UINT32 m_nCount;
	UINT32 m_nValSize;
	CString m_csDescription;
};

#endif // !defined(AFX_VAR_H__F65C1A86_52EC_4195_8CCE_D15E9CCA62D7__INCLUDED_)
