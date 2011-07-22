/*********************************************************
 *
 * File Name
 *	at_cp.h
 * Author
 * 	ZY,ZXB
 * Date
 * 	2008/04/17
 * Descriptions:
 *	Coolprofile tool stub definition
 * 	
 *********************************************************/
#include <csw.h>

#ifndef _PPP_CP_H
#define _PPP_CP_H

#include "cswtype.h"

// Switch of enable or disable CoolProfile support. 1:enable; 0:diable.
// Hints: A CoolProfile lib must be present when it's enabled!
#define PPP_CP_ENABLE 0

// Defines the CP ID range from every module.
typedef enum _AT_CP_ID_T
{
	//CPID_PPP_START_ = 0x8000,
		CPID_Aps_PppProc = 0x2001,
	//CPID_PPP_END_ = 0x8999,
} PPP_CP_ID_T;

#if (PPP_CP_ENABLE == 1)
	#define PPP_CP_MASK 0x3FFF
	#define PPP_CP_EXIT_FLAG 0x8000 
	extern void hal_PXTS_SendProfilingCode(UINT16 code);
	#define PPP_CP_ENTRY(id) hal_PXTS_SendProfilingCode((id) & PPP_CP_MASK)
	#define PPP_CP_EXIT(id) hal_PXTS_SendProfilingCode(((id) & PPP_CP_MASK) | PPP_CP_EXIT_FLAG)
#else
	#define PPP_CP_ENTRY(id)
	#define PPP_CP_EXIT(id)
#endif

#endif

