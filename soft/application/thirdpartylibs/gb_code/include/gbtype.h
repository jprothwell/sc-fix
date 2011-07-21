/***************************************************************************
* File Name    :  gbtype.h
* Corporation  :  Guobi Technology Ltd. GuangZhou China
* Copyright    :  Copyright Guobi Technology Ltd. And Subject to Confidentiality 
*                 Restrictions. This document is the confidential and proprietary 
*                 property of Guobi Technology Ltd.It is neither the document                  *                 itself nor any part of it can be shared with any party  without 
*                 the written  permission. 
* Author       :  chenbinghui
* Date         :  2009-6-18
* Purpose      :  v5 GB API 可移植的类型定义
* Version      :  V1.0
* Last Changed List
* Name         |   date   |  Discription
****************************************************************************/

#ifndef __GBTYPE_H__
#define __GBTYPE_H__

#include "eb_types.h"

/* adding for GinWave */
#ifndef _gb_far
#ifdef _C166
#define _gb_far _huge
#else
#define _gb_far
#endif
#endif

typedef GBUINT8							GBU8;
typedef GBUINT16    						GBU16;
typedef GBUINT32						GBU32;
typedef GBINT							GBI;
typedef GBUINT							GBU;
typedef GBINT8							GBI8;
typedef GBINT16							GBI16;
typedef GBINT32							GBI32;

//应用类型定义
typedef GBINT							GBBOOL;
typedef GBU16							GBKEY;
typedef GBU16							GBLANG;
typedef GBU16							GBSUBLANG;
//typedef GBWCHAR							GBEVENT;
typedef GBU8							GBINPUTMODE;
typedef GBU32							GBUDBID;
typedef GBINT							GBHRESULT; ///<键盘处理函数的处理结果

#define GBTrue							1
#define GBFalse							0

//start[2010-4-15 zhj]
typedef GBINT							GBEVENT;
//end[2010-4-15 zhj]

////start[2010-4-15 zhj]
//typedef void * pGBFILE;
////[2010-4-15 zhj]


//start[2010-4-15 haj]
typedef void *							 GBIMAGEHANDLE;
typedef void *							 GBFILEHANDLE;
//end[2010-4-15 haj]



#endif
