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

	FILENAME	: Message.h

  	PURPOSE		: interface for the CMessage class.
	REMARKS		: nil

	AUTHOR		: Vikram

	DATE		: Aug 5,03

**************************************************************/



// Message.h: 
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGE_H__22D0A2DA_6D65_4B78_9A9A_D242A4C68949__INCLUDED_)
#define AFX_MESSAGE_H__22D0A2DA_6D65_4B78_9A9A_D242A4C68949__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMessage  
{
public:
	CMessage();
	virtual ~CMessage();
public:
	UINT32 GetMessageId(){return m_nMessageId;}
	UINT32 GetAppId(){return m_nAppId;}
	CString	GetMessageName(){return m_csMessageName;}
	CString	GetFormat(){return m_csFormat;}
	CString	GetPluginName(){return m_csPlugin;}
	CString GetResponseMode(){return m_csResponseMode;}
	void SetMessageId(UINT32 nMessageId){m_nMessageId = nMessageId;}
	void SetAppId(UINT32 nAppId){m_nAppId = nAppId;}
	void SetMessageName(CString& csMessageName){m_csMessageName=csMessageName;}
	void SetFormat(CString& csFormat){m_csFormat = csFormat;}
	void SetPluginName(CString& csPlugin){m_csPlugin = csPlugin;}
	void SetResponseMode(CString csResponseMode){m_csResponseMode = csResponseMode;}
private:
	UINT32 m_nMessageId;
	UINT32 m_nAppId;
	CString m_csMessageName;
	CString m_csPlugin;
	CString	m_csFormat;
	CString m_csResponseMode;

};

#endif // !defined(AFX_MESSAGE_H__22D0A2DA_6D65_4B78_9A9A_D242A4C68949__INCLUDED_)
