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

  	PURPOSE		: Header file for CValue class

	REMARKS		: nil

	AUTHOR		: Vikram

	DATE		: 10 oct

**************************************************************/

#if !defined(AFX_VALUE_H__8D95FB90_6181_4201_A46D_3D85699C6472__INCLUDED_)
#define AFX_VALUE_H__8D95FB90_6181_4201_A46D_3D85699C6472__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
enum SourceType{
	CONSTANT=0,
		DLG,
		FL,
		REQ};
class CValue  
{
public:
	CValue();
	virtual ~CValue();
	CString GetName(){return m_csName;}
	CString GetDescription(){return m_csDescription;}
	BYTE GetSourceType(){return m_nSourceType;}
	CString GetSourceValue() {return m_csSourceValue;}
	UINT32 GetCount(){return m_nCount;}
	UINT32 GetValueSize(){return m_nSize;}

	void SetName(CString csName){m_csName = csName;}
	void SetDescription(CString csDescription){m_csDescription = csDescription;}
	void SetSourceType(BYTE nSourceType) {m_nSourceType = nSourceType;}
	void SetSourceValue(CString csSourceValue) {m_csSourceValue = csSourceValue;}
	void SetCount(UINT32 nCount){m_nCount=nCount;}
	void SetValueSize(UINT32 nSize){m_nSize = nSize;}
private:
	CString	m_csName;
	CString m_csSourceValue;
	CString	m_csDescription;
	BYTE	m_nSourceType;
	UINT32	m_nSize;
	UINT32	m_nCount;
};

#endif // !defined(AFX_VALUE_H__8D95FB90_6181_4201_A46D_3D85699C6472__INCLUDED_)
