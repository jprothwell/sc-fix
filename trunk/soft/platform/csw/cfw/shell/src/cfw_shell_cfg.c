#include <base.h>
#include <cfw.h>
#include <stdkey.h>
#include <sul.h>
#include "api_msg.h"
#include <ts.h>
#include <cfw_prv.h>
#include <event_prv.h>
#include <shell.h>
#include "csw_debug.h"

UINT32 g_ShellDisableModules = 0;

BOOL SHL_InitGlobalVariables(VOID)
{
    CSW_PROFILE_FUNCTION_ENTER(SHL_InitGlobalVariables);
    g_ShellDisableModules = 0;
    CSW_PROFILE_FUNCTION_EXIT(SHL_InitGlobalVariables);
    return TRUE;
}

BOOL SHL_DisableModules(UINT32 nDisableModule)
{
    CSW_PROFILE_FUNCTION_ENTER(SHL_DisableModules);
    g_ShellDisableModules |= nDisableModule;
    CSW_PROFILE_FUNCTION_EXIT(SHL_DisableModules);
    return TRUE;
}

UINT32 SHL_GetDisableModule(VOID)
{
    CSW_PROFILE_FUNCTION_ENTER(SHL_GetDisableModule);
    CSW_PROFILE_FUNCTION_EXIT(SHL_GetDisableModule);
    return g_ShellDisableModules;
}

#if 0
UINT32 SHL_CleanupData()
{
    CSW_PROFILE_FUNCTION_ENTER(SHL_CleanupData);
    CSW_PROFILE_FUNCTION_EXIT(SHL_CleanupData);
    FS_Format(NULL);
    // Registry import.
}
#endif
