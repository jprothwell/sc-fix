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

	FILENAME	: GCMLDefines.h

  	PURPOSE		: Decalaration file

	REMARKS		: nil

	AUTHOR		: Kuldeep

	DATE		: Aug 5,03

**************************************************************/

#ifndef _GCML_HANDLER_H_
#define _GCML_HANDLER_H_

#define	TAG_NWSIMULATOR		_T("nwsimulator")
#define	TAG_APPLICATION		_T("application")
#define	TAG_DATATYPE		_T("datatype")
#define	TAG_NAME			_T("name")
#define	TAG_SIZE			_T("size")
#define	TAG_ID				_T("id")
#define	TAG_FORMATS			_T("formats")
#define TAG_TOTAL			_T("total")
#define TAG_VALUE			_T("value")
#define TAG_TYPE			_T("type")
#define TAG_SOURCE			_T("source")
#define TAG_INCOMINGMSG		_T("incomingmsg")
#define TAG_OUTGOINGMSG		_T("outgoingmsg")
#define TAG_APPID			_T("appid")
#define TAG_FORMATNAME		_T("fmtname")
#define TAG_PLUGIN			_T("plugin")
#define TAG_DATALISTNAME	_T("datalstname")
#define TAG_DLGNAME			_T("dlgname")
#define TAG_DATA			_T("data")
#define TAG_COUNT				_T("count")
#define TAG_VALUESIZE			_T("valuesize")
#define TAG_DATALIST			_T("datalist")
#define TAG_ASYNCMSGSEQ			_T("asyncmsgseq")
#define TAG_ASYNCMSG			_T("asyncmsg")
#define TAG_DESCRIPTION     _T("description")
#define TAG_TIMER           _T("timer")
#define TAG_FREQUENCY       _T("frequency")
#define TAG_DEFAULT         _T("default")
#define TAG_INPUTMODE       _T("input")
#define TAG_RESPONSE       _T("response")
#define TAG_BOOTUPMSG      _T("bootupmsgseq")
#define TAG_FREQUSED      _T("freqmsgseq")
#define TAG_INPUT_RESPONSE_SEQ  _T("respseq")
#define TAG_BOOTMESG				_T("bootmesg")

// Add by zhuoxz, 2009-09-28
#ifdef _DUAL_SIM_
#define TAG_INCOMINGMSG2		_T("incomingmsg2")
#define TAG_OUTGOINGMSG2		_T("outgoingmsg2")
#define TAG_ASYNCMSGSEQ2		_T("asyncmsgseq2")
#define TAG_BOOTUPMSG2      _T("bootupmsgseq2")
#endif

#endif
