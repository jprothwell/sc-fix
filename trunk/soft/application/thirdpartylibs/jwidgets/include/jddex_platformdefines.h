/***************************************************************************
*
* File Name : jddex_platform.h
*
*
***************************************************************************
*
*
* File Description
* ----------------
*
* Purpose			:	includes all platform header files
*						
* Created By		:
* Created Date		:
*
*
*
* Current Revision	:
*
***************************************************************************
*
*
* Revision Details
* ----------------
*
* 1. Modified By	:
*    Modified Date	:
*	 Purpose		:
*
*
*
*
***************************************************************************/

#ifndef _JDDEX_PLATFORMDEFINES_
#define _JDDEX_PLATFORMDEFINES_
#ifdef __cplusplus
    extern   "C"
    {
#endif
#ifdef MMI_ON_HARDWARE_P
#include "cs_types.h"
#endif



#include "globaldefs.h"
#include "eventsgprot.h"
#include "jwap_str_id.h"
#include "ddl.h"
#include "jcal.h"

#include "historygprot.h"
#include "globalmenuitems.h"
#include "globaldefs.h"
#include "gui.h"
#include "gdi_include.h"
#include "lcd_sw_inc.h"
#include "commonscreens.h"
#include "jmms_str_id.h"
#ifdef __cplusplus
    }
#endif

#endif //_JDDEX_PLATFORMDEFINES_
