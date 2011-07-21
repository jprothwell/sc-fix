#ifndef MMI_ON_HARDWARE_P
#include "stdafx.h"
#include "mci.h"
#include "usb_interface.h"

VOID MCI_USBInit(UINT8 mailBoxId)
{
}
UINT32  MCI_USBConnect(VOID)
{
  return 0;
}
USB_DETECTED_STATUS  MCI_USBDetect(VOID)
{
  return 0;
}	
UINT32  MCI_USBDisconnect(VOID)
{
  return 0;
}	
VOID    USB_PreClose(VOID)
{
}	
void Usb_stop_application(void)
{
}

#endif //MMI_ON_HARDWARE_P
