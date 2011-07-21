#ifndef __GBAPP_H__
#define __GBAPP_H__
#include "mmi_features.h"

#ifdef __MMI_GB_V5__
#ifdef WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef WIN32
#include <conio.h>
#include <Windows.h>
#endif

#include "gbim.h"
#include "gbapi.h"
#ifndef WIN32
#include "gbwchar.h"
#endif
#include "misc.h"

#undef FALSE
#define	FALSE 0

#undef TRUE
#define TRUE 1

#ifdef __cplusplus
extern "C" {
#endif	// end __cplusplus

	enum MULTITAP_STATE
	{
		NOT_MultiTap_STATE
		,MultiTap_Cap	///<大写的字母候选
		,MultiTap_Lower	///<小写的字母候选

		,MultiTap_End
	};

	// 是否选中候选字
#define GBIS_Is_CandSelected(p)	((p)->outputInfo.nStatusFlag & GBSF_Selected)
	// 是否有下一个音节
#define GBIS_HaveNextSyllable(p)	((p)->pAuxInfo->nArrowFlags & GBTA_Right)
	// 是否有上一个音节
#define GBIS_HavePrevSyllable(p)	((p)->pAuxInfo->nArrowFlags & GBTA_Left)
	// 是否为输入模式
#define GBIS_Is_Input(p)			((p)->nCandType == GBCT_Normal)
	/**
	* 国笔输入法引擎V5版数据文件引用
	*/
	extern GBU16			g_nDataCount;
	extern GBInputStruct	_huge g_gbis;
	extern GBAuxInfo		g_aux;
	extern GBLDB			g_ldbs[32];
#if 0
	///UDB的RAM空间
	extern GBU32			_huge g_udb_buffer[6 * 1024];
	///当前UDB ID
	extern GBI32			g_nCurUDBID;
	extern GBI32			g_nLangId;

	extern GBINT			g_currIM[GBIM_Max][MultiTap_End];
	extern GBINT			g_imCount;

	// 为无缝切换时提供Alphabetic语言的UDB
	extern GBI32			g_nAlpUDBID;
	extern GBI32			g_nAlpLangID;
	extern GBU32			_huge g_pAlpUDBBuff[4 * 1024];

	extern GBINT			g_MultiTap_Lower_Cap;//0为非MultiTap模式，1为小写，2为大写
#endif
	// 定义输入候选框的类型
	#define GB_CANDIDATE_TYPE	1	//候选行数

	// 定义输入法切换结构体
	typedef struct tagInputModeSwitch
	{
		GBU16				index;
		GBLANG				nLang;		
		GBSUBLANG			nSubLang;	
		GBU16				inputMode;
	}GBInputModeSwitch;

#ifdef __cplusplus
}
#endif	// end __cplusplus
#endif /*__MMI_GB_V5__*/
#endif	// end __GBAPP_H__