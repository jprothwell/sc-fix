/***********************************************
	FILENAME	:Coolprofile.h
	PURPOSE		:Coolprofile tool stub define
	REMARKS		:For TBM780
	DATE		:2007/07/03
	AUTHOR		:ZY
************************************************/

#ifndef _COOLPROFILE_H
#define _COOLPROFILE_H

#include "cswtype.h"
#ifdef MMI_ON_WIN32
	#define TBM_ENTRY(id)
	#define TBM_EXIT(id)

#else 
#include "chip_id.h"



#define COOLPROFILE_ENABLE 0 // 1:�򿪿���; 0:�رտ���,�رպ����е�Coolprofile������ʧЧ

#if (COOLPROFILE_ENABLE == 0)
	#define TBM_ENTRY(id)
	#define TBM_EXIT(id)
#else
	#define CPMASK 0x3fff
	#define CPEXITFLAG 0x8000 

#include "hal_debug.h"
//FIXME Define a real name for that enum value.
    #define  TBM_ENTRY(id)  hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_RESERVED_9, id)
    #define  TBM_EXIT(id)  hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_RESERVED_9, id)
#endif

#endif//end MMI_ON_WIN32

#endif
