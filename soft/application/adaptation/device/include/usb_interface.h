#ifndef __USB_INTERFACE_H__
#define __USB_INTERFACE_H__
#include "cs_types.h"
typedef enum
{
    USB_DETECTED_FALSE,
    USB_DETECTED_OK,
    USB_DETECTED_BUSY,
} USB_DETECTED_STATUS;

VOID MCI_USBInit(UINT8 mailBoxId);
UINT32  MCI_USBConnect(VOID);
USB_DETECTED_STATUS  MCI_USBDetect(VOID);
UINT32  MCI_USBDisconnect(VOID);
VOID    USB_PreClose(VOID);

// Misc
INT32   mci_webcamera_start();
#ifdef DIRTY_WORKAROUND_FOR_USBDOWNLOAD
#define USB_BOOT_MODE_UTRACE 0xcc
#define USB_BOOT_MODE_UMASS	0xaa
#endif /* DIRTY_WORKAROUND_FOR_USBDOWNLOAD */

#endif
