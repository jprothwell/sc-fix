#include "drv_comm.h"
#include "stack_common.h"
#include "stack_msgs.h"
#include "app_ltlcom.h"
#include "stack_config.h"
#include "usbvideo_drv.h"
#include "usbvideo_state.h"
#include "usbvideo_custom.h"
#include "usbvideo_if.h"




void USBVideo_Send_Msg(UVIDEOS_MSG_TYPE usb_msg_type, kal_uint32 param1, kal_uint32 param2)
{
	usbvideo_video_size_struct *start_cnf;
	usbvideo_still_size_struct *still_ind;
	//usbvideo_still_dma_struct *still_dma;
	usbvideo_result_struct *result;
	ilm_struct *send_ilm = NULL;
	module_type dest_mod;
	
	dest_mod = MOD_MED;

	if(usb_msg_type == UVIDEOS_MSG_MED_START_SUCCESS_CNF)
	{
		start_cnf = (usbvideo_video_size_struct*)construct_local_para(
							sizeof(usbvideo_video_size_struct), TD_CTRL);
		start_cnf->video_size_type = (USBVIDEO_VIDEO_SIZE_TYPE)param1;	
		start_cnf->video_compression_type = (USBVIDEO_COMPRESSION_TYPE)param2;
		start_cnf->result = KAL_TRUE;

		DRV_BuildPrimitive(send_ilm,
						MOD_USB,
						dest_mod,
						MSG_ID_USB_MED_START_VIDEO_CNF,
						start_cnf);
	}
	else if(usb_msg_type == UVIDEOS_MSG_MED_START_FAIL_CNF)
	{
		start_cnf = (usbvideo_video_size_struct*)construct_local_para(
							sizeof(usbvideo_video_size_struct), TD_CTRL);
		start_cnf->video_size_type = (USBVIDEO_VIDEO_SIZE_TYPE)param1;	
		start_cnf->video_compression_type = (USBVIDEO_COMPRESSION_TYPE)param2;
		start_cnf->result = KAL_FALSE; /*  because USB cable plug out */

		DRV_BuildPrimitive(send_ilm,
						MOD_USB,
						dest_mod,
						MSG_ID_USB_MED_START_VIDEO_CNF,
						start_cnf);
	}
	else if(usb_msg_type == UVIDEOS_MSG_MED_CHANGE_VIDEO_IND)
	{
		start_cnf = (usbvideo_video_size_struct*)construct_local_para(
							sizeof(usbvideo_video_size_struct), TD_CTRL);
		start_cnf->video_size_type = (USBVIDEO_VIDEO_SIZE_TYPE)param1;
		start_cnf->video_compression_type = (USBVIDEO_COMPRESSION_TYPE)param2;
		start_cnf->result = KAL_TRUE;

		DRV_BuildPrimitive(send_ilm,
						MOD_USB,
						dest_mod,
						MSG_ID_USB_MED_CHANGE_VIDEO_IND,
						start_cnf);
	}
	else if(usb_msg_type == UVIDEOS_MSG_MED_STILL_CAPTURE_IND)
	{
		still_ind = (usbvideo_still_size_struct*)construct_local_para(
							sizeof(usbvideo_still_size_struct), TD_CTRL);
		still_ind->still_size_type = (USBVIDEO_STILL_SIZE_TYPE)param1;
		still_ind->still_compression_type = (USBVIDEO_COMPRESSION_TYPE)param2;

		DRV_BuildPrimitive(send_ilm,
						MOD_USB,
						dest_mod,
						MSG_ID_USB_MED_START_STILL_IND,
						still_ind);
	}
	else if(usb_msg_type == UVIDEOS_MSG_MED_START_IND)
	{
		start_cnf = (usbvideo_video_size_struct*)construct_local_para(
							sizeof(usbvideo_video_size_struct), TD_CTRL);
		start_cnf->video_size_type = (USBVIDEO_VIDEO_SIZE_TYPE)param1;
		start_cnf->video_compression_type = (USBVIDEO_COMPRESSION_TYPE)param2;
		start_cnf->result = KAL_TRUE;

		DRV_BuildPrimitive(send_ilm,
						MOD_USB,
						dest_mod,
						MSG_ID_USB_MED_START_VIDEO_IND,  /* restart video after still capture */
						start_cnf);
	}
	else if(usb_msg_type == UVIDEOS_MSG_MED_DEVICE_STILL_CNF)  /* be used at HW trigger */
	{
		result = (usbvideo_result_struct*)construct_local_para(
							sizeof(usbvideo_result_struct), TD_CTRL);
		result->result = (kal_bool) param1;

		DRV_BuildPrimitive(send_ilm,
						MOD_USB,
						dest_mod,
						MSG_ID_USB_MED_DEVICE_STILL_CNF,
						result);
	}
	else if(usb_msg_type == UVIDEOS_MSG_MED_PAUSE_CNF)
	{
		result = (usbvideo_result_struct*)construct_local_para(
							sizeof(usbvideo_result_struct), TD_CTRL);
		result->result = (kal_bool) param1;

		DRV_BuildPrimitive(send_ilm,
						MOD_USB,
						dest_mod,
						MSG_ID_USB_MED_PAUSE_CNF,
						result);
	}
	else if(usb_msg_type == UVIDEOS_MSG_MED_RESUME_SUCCESS_CNF)
	{
		start_cnf = (usbvideo_video_size_struct*)construct_local_para(
							sizeof(usbvideo_video_size_struct), TD_CTRL);
		start_cnf->video_size_type = (USBVIDEO_VIDEO_SIZE_TYPE)param1;
		start_cnf->video_compression_type = (USBVIDEO_COMPRESSION_TYPE)param2;
		start_cnf->result = KAL_TRUE;

		DRV_BuildPrimitive(send_ilm,
						MOD_USB,
						dest_mod,
						MSG_ID_USB_MED_RESUME_VIDEO_CNF,
						start_cnf);
	}
	else if(usb_msg_type == UVIDEOS_MSG_MED_RESUME_FAIL_CNF)
	{
		start_cnf = (usbvideo_video_size_struct*)construct_local_para(
							sizeof(usbvideo_video_size_struct), TD_CTRL);
		start_cnf->video_size_type = (USBVIDEO_VIDEO_SIZE_TYPE)param1;
		start_cnf->video_compression_type = (USBVIDEO_COMPRESSION_TYPE)param2;
		start_cnf->result = KAL_FALSE;

		DRV_BuildPrimitive(send_ilm,
						MOD_USB,
						dest_mod,
						MSG_ID_USB_MED_RESUME_VIDEO_CNF,
						start_cnf);
	}
	else if(usb_msg_type == UVIDEOS_MSG_MED_STOP_IND) /*stop because of cable plugging out */
	{
		DRV_BuildPrimitive(send_ilm,
						MOD_USB,
						dest_mod,
						MSG_ID_USB_MED_STOP_IND,
						NULL);
	}
	else if(usb_msg_type == UVIDEOS_MSG_MED_STOP_CNF)  /* stop request from media task */
	{
		result = (usbvideo_result_struct*)construct_local_para(
							sizeof(usbvideo_result_struct), TD_CTRL);
		result->result = (kal_bool) param1;

		DRV_BuildPrimitive(send_ilm,
						MOD_USB,
						dest_mod,
						MSG_ID_USB_MED_STOP_CNF,
						result);
	}
	else if(usb_msg_type == UVIDEOS_MSG_MED_ABORT_IND) /* OS sleep then wakeup, webcam must be abort */
	{
		DRV_BuildPrimitive(send_ilm,
						MOD_USB,
						dest_mod,
						MSG_ID_USB_MED_ABORT_IND,
						NULL);
	}
#if 0//JiaShuo Delete for webcam
	else if(usb_msg_type == USBVIDEO_MSG_USB_ENUM_DONE)
	{
		DRV_BuildPrimitive(send_ilm,
						MOD_DRV_HISR,
						MOD_USB,
						MSG_ID_USB_ENUM_DONE,
						NULL);
	}
	else if(usb_msg_type == USBVIDEO_MSG_USB_COMPLETE_VIDEO)
	{
		/* !!! Note that source need to modify after camera phase in*/
		DRV_BuildPrimitive(send_ilm,
						MOD_GPT_HISR,/* not correct */
						MOD_USB,
						MSG_ID_USB_VIDEO_COMPLETE,
						NULL);
	}
	else if(usb_msg_type == USBVIDEO_MSG_USB_DMA_DONE)
	{
		DRV_BuildPrimitive(send_ilm,
						(module_type)param1,
						MOD_USB,
						MSG_ID_USB_VIDEO_DMA_DONE,
						NULL);
	}
	else if(usb_msg_type == USBVIDEO_MSG_USB_CHANGE_SIZE)
	{
		DRV_BuildPrimitive(send_ilm,
						MOD_DRV_HISR,
						MOD_USB,
						MSG_ID_USB_VIDEO_CHANGE_SIZE,
						NULL);
	}
	else if(usb_msg_type == USBVIDEO_MSG_USB_STILL_CAPTURE)
	{
		DRV_BuildPrimitive(send_ilm,
						MOD_DRV_HISR,
						MOD_USB,
						MSG_ID_USB_STILL_CAPTURE,
						NULL);
	}
	else if(usb_msg_type == USBVIDEO_MSG_USB_COMPLETE_STILL)
	{
		/* !!! Note that source need to modify after camera phase in*/
		DRV_BuildPrimitive(send_ilm,
						MOD_GPT_HISR,/* not correct */
						MOD_USB,
						MSG_ID_USB_STILL_COMPLETE,
						NULL);
	}
	else if(usb_msg_type == USBVIDEO_MSG_USB_STILL_DMA_DONE)
	{
		DRV_BuildPrimitive(send_ilm,
						(module_type)param1,
						MOD_USB,
						MSG_ID_USB_VIDEO_STILL_DMA_DONE,
						NULL);
	}
	else if(usb_msg_type == USBVIDEO_MSG_USB_STILL_DMA_PARTIAL)
	{
		still_dma = (usbvideo_still_dma_struct*)construct_local_para(
							sizeof(usbvideo_still_dma_struct), TD_CTRL);
		still_dma->index = param1;
		DRV_BuildPrimitive(send_ilm,
						MOD_DRV_HISR,
						MOD_USB,
						MSG_ID_USB_VIDEO_STILL_DMA_PARTIAL,
						still_dma);
	}
	else if(usb_msg_type == USBVIDEO_MSG_USB_INCALL_DMA_DONE)
	{
		DRV_BuildPrimitive(send_ilm,
						(module_type)param1,
						MOD_USB,
						MSG_ID_USB_VIDEO_INCALL_DMA_DONE,
						NULL);
	}
	else if(usb_msg_type == USBVIDEO_MSG_USB_INCALL_TIMEOUT)
	{
		DRV_BuildPrimitive(send_ilm,
						MOD_GPT_HISR,
						MOD_USB,
						MSG_ID_USB_VIDEO_INCALL_TIMEOUT,
						NULL);
	}
	else if(usb_msg_type == USBVIDEO_MSG_USB_INCALL_CHANGE_SIZE)
	{
		DRV_BuildPrimitive(send_ilm,
						MOD_DRV_HISR,
						MOD_USB,
						MSG_ID_USB_VIDEO_INCALL_CHANGE_SIZE,
						NULL);
	}
#endif
/*
	else if(usb_msg_type == USBVIDEO_MSG_USB_ABORT)
	{
		DRV_BuildPrimitive(send_ilm,
						MOD_DRV_HISR,
						MOD_USB,
						MSG_ID_USB_VIDEO_ABORT,
						NULL);
	}
*/
	else
	{
		send_ilm = NULL;
#if 0//JiaShuo Delete for webcam
		EXT_ASSERT(0, (kal_uint32)usb_msg_type, 0, 0);
#endif
	}

	msg_send_ext_queue(send_ilm);
}
#if 0//JiaShuo Delete for webcam

/* camera driver use this function to get the current video buffer addr */
kal_uint8* USBVideo_Get_Video_Buffer(void)
{
	kal_uint8 *addr;
	
	addr = g_USBVideo.video_control.video_buffer_addr[g_USBVideo.video_control.video_buffer_write_index];
	addr += USBVIDEO_PAYLOAD_HEADER_LENGTH;	/* header offset */

	return addr;
}

/* camera driver use this function to notify USB driver that it complete filling this buffer  */
void USBVideo_Complete_Video_Buffer
	(kal_uint8* addr, kal_uint32 size, kal_uint32 time, USBVIDEO_VIDEO_SIZE_TYPE video_size_type)
{
	USBVideo_Payload_Header *pHeader;
	kal_uint32 savedMask;
	// for debug
	static kal_uint8 complete_video_buff_write_index;
	static kal_uint8 complete_video_buff_read_index;
	static kal_bool complete_video_buff_full_flag;


	pHeader = (USBVideo_Payload_Header*)g_USBVideo.video_control.video_buffer_addr[g_USBVideo.video_control.video_buffer_write_index];
	pHeader->dwPTS[0] = time&0xff;
	pHeader->dwPTS[1] = (time>>8)&0xff;
	pHeader->dwPTS[2] = (time>>16)&0xff;
	pHeader->dwPTS[3] = (time>>24)&0xff;	
	pHeader->bHeaderLength = sizeof(USBVideo_Payload_Header);
	pHeader->bmHeaderInfo = (1<<USBVIDEO_PAYLOAD_PTS)|(1<<USBVIDEO_PAYLOAD_SCR)|(1<<USBVIDEO_PAYLOAD_EOH);
	/* dwSCR will be filled when configring DMA to send */

	USBVideo_Send_Msg(USBVIDEO_MSG_USB_COMPLETE_VIDEO, 0, 0);		
}

#endif
