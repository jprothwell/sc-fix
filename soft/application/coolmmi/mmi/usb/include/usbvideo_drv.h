#include "usbvideo_custom.h"
#include "uvideos_stream_m.h"

typedef struct 
{
	LOCAL_PARA_HDR
	kal_uint8 	index;
}usbvideo_still_dma_struct; 

extern kal_uint8* USBVideo_Get_Video_Buffer(void);
extern void USBVideo_Complete_Video_Buffer(kal_uint8* addr, kal_uint32 size, kal_uint32 time, USBVIDEO_VIDEO_SIZE_TYPE video_size_type);
extern void USBVideo_Send_Msg(UVIDEOS_MSG_TYPE usb_msg_type, kal_uint32 param1, kal_uint32 param2);


