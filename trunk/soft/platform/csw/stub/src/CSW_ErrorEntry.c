#include "chip_id.h"

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
#include "hal.h"
VOID CSW_ErrorEntry(UINT32 cpuid,enum hal_Error_Code Error_Code, UINT32 status, t_hal_Error_ctx* ctx)
{
}
#endif

