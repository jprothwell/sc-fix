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

	FILENAME	: Format.h:

  	PURPOSE		: interface for the CFormat class.

	REMARKS		: nil

	AUTHOR		: Vikram

	DATE		: Aug 5,03

*/
// Format.h: interface for the CFormat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FORMAT_H__2AAA2B03_D98E_4F7F_BAC7_A8FBF4BF816D__INCLUDED_)
#define AFX_FORMAT_H__2AAA2B03_D98E_4F7F_BAC7_A8FBF4BF816D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>		// MFC socket extensions

#include "Var.h"
#include "DataList.h"
class CFormat  
{
public:
	CFormat();
	virtual ~CFormat();
	CString GetInputMode(){return m_csInputMode;}
	CString GetName(){return m_csName;}
	CString GetPlugin(){return m_csPlugin;}
	CDataList GetDataList(){return m_DataList;}
	CString GetDialog(){ return m_csDlg;}

	CList<CVar,CVar&>& GetVarList(){return m_VarList;}
	void AddVariable(CVar& varObj);
	void SetName(CString csName){m_csName = csName;}
	void SetPlugin(CString csPlugin){ m_csPlugin = csPlugin;}
	void SetDataList(CDataList& dataList);
	void SetInputMode(CString csInputMode){m_csInputMode = csInputMode;}
	void SetDialog(CString csDlg){ m_csDlg = csDlg;} 
private:
	CString m_csInputMode;
	CString	m_csName;
	CString	m_csPlugin;
	CDataList m_DataList;
	CString m_csDlg;
	CList<CVar,CVar&> m_VarList;

};

#endif // !defined(AFX_FORMAT_H__2AAA2B03_D98E_4F7F_BAC7_A8FBF4BF816D__INCLUDED_)
