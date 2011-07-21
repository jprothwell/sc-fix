/******************************************************************************/
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    adp_NW_Event.h                                                */
/*                                                                            */
/* Description:                                                               */
/*     Add this header file for solving.                                      */
/*     @ZXB 2009-02-06                                                        */
/******************************************************************************/

#ifndef _ADP_NW_EVENT_H
#define _ADP_NW_EVENT_H

#include "mmi_features.h"
#include <csw.h>
#include "mmi.h"
#include "app_ltlcom.h"

// Force to include adp_events.h if not.
#ifndef _ADP_EVENTS_H
#include "adp_events.h"
#endif

#ifdef __ADP_MULTI_SIM__

typedef struct _ADP_NW_CONTEXT_T
{
	ADP_SIM_ID_T sim_id;
	// TODO: add dual-sim related global variables here...
} ADP_NW_CONTEXT_T;

extern ADP_NW_CONTEXT_T g_NwContext[];
extern ADP_NW_CONTEXT_T *g_pNwContext;

extern void CswPLMN2CSDPLMN(UINT8 *pCSDPLMN,  UINT8 *pCswPLMN,  UINT8 nLength);
extern void CSDPLMN2CswPLMN(UINT8 *pCswPLMN,  UINT8 *pCSDPLMN,  UINT8 nLength);

#endif

#endif

