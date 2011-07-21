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
		,MultiTap_Cap	///<��д����ĸ��ѡ
		,MultiTap_Lower	///<Сд����ĸ��ѡ

		,MultiTap_End
	};

	// �Ƿ�ѡ�к�ѡ��
#define GBIS_Is_CandSelected(p)	((p)->outputInfo.nStatusFlag & GBSF_Selected)
	// �Ƿ�����һ������
#define GBIS_HaveNextSyllable(p)	((p)->pAuxInfo->nArrowFlags & GBTA_Right)
	// �Ƿ�����һ������
#define GBIS_HavePrevSyllable(p)	((p)->pAuxInfo->nArrowFlags & GBTA_Left)
	// �Ƿ�Ϊ����ģʽ
#define GBIS_Is_Input(p)			((p)->nCandType == GBCT_Normal)
	/**
	* �������뷨����V5�������ļ�����
	*/
	extern GBU16			g_nDataCount;
	extern GBInputStruct	_huge g_gbis;
	extern GBAuxInfo		g_aux;
	extern GBLDB			g_ldbs[32];
#if 0
	///UDB��RAM�ռ�
	extern GBU32			_huge g_udb_buffer[6 * 1024];
	///��ǰUDB ID
	extern GBI32			g_nCurUDBID;
	extern GBI32			g_nLangId;

	extern GBINT			g_currIM[GBIM_Max][MultiTap_End];
	extern GBINT			g_imCount;

	// Ϊ�޷��л�ʱ�ṩAlphabetic���Ե�UDB
	extern GBI32			g_nAlpUDBID;
	extern GBI32			g_nAlpLangID;
	extern GBU32			_huge g_pAlpUDBBuff[4 * 1024];

	extern GBINT			g_MultiTap_Lower_Cap;//0Ϊ��MultiTapģʽ��1ΪСд��2Ϊ��д
#endif
	// ���������ѡ�������
	#define GB_CANDIDATE_TYPE	1	//��ѡ����

	// �������뷨�л��ṹ��
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