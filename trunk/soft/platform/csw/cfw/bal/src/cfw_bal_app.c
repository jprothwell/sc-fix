#include <cfw.h>
#include <cos.h>

typedef struct _APP_NODE {
    PFN_CFW_EV_PORC pfnProc;
    PVOID pUserData;
}APP_NODE,*PAPP_NODE ;

PRIVATE APP_NODE g_app_node          = {0, };
//PRIVATE PFN_CFW_EV_PORC g_pfnCfwProc = NULL;

UINT32 CFW_BalSetEventPorc( VOID* pUserData,
						    PFN_CFW_EV_PORC pfnProc)
{

    UINT32 ret = ERR_INVALID_PARAMETER;

    if(pfnProc)
    {
        g_app_node.pUserData = pUserData;
        g_app_node.pfnProc  = pfnProc; 
        ret = ERR_SUCCESS;
    }

    return ret;
}

UINT32 BAL_Run(CFW_EVENT* pEvent)
{
	if(g_app_node.pfnProc)
	{
		return g_app_node.pfnProc(pEvent, g_app_node.pUserData);
	}
	else
    {
        UINT16 n = MAKEUINT16(pEvent->nFlag, pEvent->nType);
        COS_EVENT ev;
        
        ev.nEventId = pEvent->nEventId;
        ev.nParam1  = pEvent->nParam1;
        ev.nParam2  = pEvent->nParam2;
        ev.nParam3  = MAKEUINT32(n, pEvent->nUTI);
        COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
		return BAL_PROC_DONT_FREE_MEM;
    }
}

PRIVATE PFN_BOOL pfnCfwSetupQueryEvent = NULL;

UINT32 SRVAPI CFW_SetupQueryEvent(PFN_BOOL pfnCfwQueryEvent) 
{
	pfnCfwSetupQueryEvent = pfnCfwQueryEvent;

	return 0;
}

BOOL CFW_QueryEvent(VOID)
{
	if( pfnCfwSetupQueryEvent )
		return pfnCfwSetupQueryEvent();
	else
		return FALSE;
}
