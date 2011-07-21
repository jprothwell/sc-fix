#ifndef MMI_ON_HARDWARE_P
#include "stdafx.h"
#include "mci.h"

void set_mmc_camera_preview()
{
}
void set_multicaputure(int i)
{
}
UINT32 MCI_CamPlayBack ()
{
	return 0;
}
UINT32 MCI_CamCancelLastSave(VOID)
{
	return 0;
}
#if (CSW_EXTENDED_API_CAMERA == 1)
INT32  MCI_CamSavePhoto (UINT8*)
{
	return 0;
}
#else
INT32  MCI_CamSavePhoto (VOID)
{
	return 0;
}
#endif /* CSW_EXTENDED_API_CAMERA */

#ifndef __MMI_DISABLE_DUAL_SENSOR__
void camera_SwitchSensorReset()
{ 
}
#endif

#endif //MMI_ON_HARDWARE_P