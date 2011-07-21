#if defined(MOTION_SENSOR_SUPPORT)
#include "kal_non_specific_general_types.h"
#include "mmi_trace.h"
#include "gsensor_m.h"
#include "gsensorapp.h"
#include "globalconstants.h"
#include "app_ltlcom.h"
#include "mmi_msg_struct.h"
#include "stack_config.h"
#include "protocolevents.h"
#include "oslmemory.h"
#include "queuegprot.h"



kal_uint32 motion_sensor_app_id;


void motion_sensor_set_app_id(kal_uint32 flag)
{
	motion_sensor_app_id|=flag;

}

void motion_sensor_clear_app_id(kal_uint32 flag)
{
	motion_sensor_app_id &=~flag;
}

static void Send_Key_Event_Ind(kal_uint16 Keycode,kal_uint16 Keyevent)
{
	ilm_struct  ilm_ptr;
	msg_kbd_data  *msg_key;
	memset(&ilm_ptr,0,sizeof(ilm_ptr));
	msg_key = OslMalloc(sizeof(msg_kbd_data));

	ilm_ptr.local_para_ptr = (local_para_struct *)msg_key ;
	ilm_ptr.dest_mod_id = (module_type)MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;
	ilm_ptr.msg_id = MSG_ID_MMI_EQ_KEYPAD_DETECT_IND;
	msg_key->Keycode = Keycode;
	msg_key->Keyevent = Keyevent;
	OslMsgSendExtQueue(&ilm_ptr);
}

static void Send_Wallpaper_Change_Ind(void)
{
	mmi_trace(4,"Send_Wallpaper_Change_Ind");

	ilm_struct  ilm_ptr;
	ilm_ptr.local_para_ptr = (local_para_struct*) NULL;
	ilm_ptr.dest_mod_id = (module_type)MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;
	ilm_ptr.msg_id = MSG_ID_MMI_EQ_WALLPAPER_CHANGE_IND;

	OslMsgSendExtQueue(&ilm_ptr);
}
static void Send_ImageViewer_Change_Ind(kal_uint32 direction)
{
	mmi_trace(4,"Send_ImageViewer_Change_Ind");
	ilm_struct  ilm_ptr;
	mmi_eq_gsensor_direction_ind_struct  *msg_direction;
	memset(&ilm_ptr,0,sizeof(ilm_ptr));
	msg_direction = OslMalloc(sizeof(mmi_eq_gsensor_direction_ind_struct));

	ilm_ptr.local_para_ptr = (local_para_struct *)msg_direction ;
	ilm_ptr.dest_mod_id = (module_type)MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;
	ilm_ptr.msg_id = MSG_ID_MMI_EQ_IMAGEVIEWER_CHANGE_IND;
	msg_direction->direction = direction;
	OslMsgSendExtQueue(&ilm_ptr);

}
static void Send_Rotate_Ind(kal_uint32 rotate_type)
{
	ilm_struct  ilm_ptr;
	mmi_eq_gsensor_rotate_ind_struct  *msg_rotate;
	memset(&ilm_ptr,0,sizeof(ilm_ptr));
	msg_rotate = OslMalloc(sizeof(mmi_eq_gsensor_rotate_ind_struct));

	ilm_ptr.local_para_ptr = (local_para_struct *)msg_rotate ;
	ilm_ptr.dest_mod_id = (module_type)MOD_MMI;
	ilm_ptr.src_mod_id = MOD_L4C;
	ilm_ptr.msg_id = MSG_ID_MMI_EQ_GSENSOR_ROTATE_IND;
	msg_rotate->rotate = rotate_type;
	OslMsgSendExtQueue(&ilm_ptr);
}


static void motion_sensor_shake_callback(GSS_SAMPLE_DATA_T* gsensor_data)
{

	switch(gsensor_data->shake)
	{
		case GS_MOTION_SHAKE_X_L_AXIS:
		case GS_MOTION_SHAKE_Y_L_AXIS:
		if(motion_sensor_app_id&0x01)
		{
			Send_Key_Event_Ind(KEY_LEFT_ARROW,KEY_EVENT_DOWN);
			Send_Key_Event_Ind(KEY_LEFT_ARROW,KEY_EVENT_UP);
		}
		else if(motion_sensor_app_id&0x02)
		{
			Send_Wallpaper_Change_Ind();
		}
		else if(motion_sensor_app_id&0x40)
		{
			Send_ImageViewer_Change_Ind(1);
		}
		
		break;

		case GS_MOTION_SHAKE_X_R_AXIS:
		case GS_MOTION_SHAKE_Y_R_AXIS:
		if(motion_sensor_app_id&0x01)
		{
			Send_Key_Event_Ind(KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
			Send_Key_Event_Ind(KEY_RIGHT_ARROW,KEY_EVENT_UP);
			break;
		}
		else if(motion_sensor_app_id&0x02)
		{
			Send_Wallpaper_Change_Ind();
		}
		else if(motion_sensor_app_id&0x40)
		{
			Send_ImageViewer_Change_Ind(2);
		}		
		break;
	}
}

static UINT8 pre_rotate_status = GS_MOTION_ROTATE_UP;
static void motion_sensor_rotate_callback(GSS_SAMPLE_DATA_T* gsensor_data)
{
	mmi_trace(4,"motion_sensor_rotate_callback->gsensor_data.rotate=%x",gsensor_data->rotate);
	
    if (gsensor_data->rotate == pre_rotate_status)
        return;
	
	switch(gsensor_data->rotate)
	{
		case GS_MOTION_ROTATE_UP://0
			if(pre_rotate_status == GS_MOTION_ROTATE_RIGHT)
				Send_Rotate_Ind(GSENSOR_ROCATE_TYPE_ANTICLOCKWISE);
			break;
			
		case GS_MOTION_ROTATE_DOWN://180
			break;

		case GS_MOTION_ROTATE_LEFT://270
			break;
			
		case GS_MOTION_ROTATE_RIGHT://90
			if(pre_rotate_status == GS_MOTION_ROTATE_UP)
				Send_Rotate_Ind(GSENSOR_ROCATE_TYPE_CLOCKWISE);
			break;
			
		default:
			break;
	}
	
	pre_rotate_status = gsensor_data->rotate;

}

static void motion_sensor_turn_callback(GSS_SAMPLE_DATA_T* gsensor_data)
{

}

/*
* FUNCTION                                                            
*	motion_sensor_shake
*
* DESCRIPTION                                                           
*   	This function is to enable or disable sample function
*
* CALLS  
*
* PARAMETERS
*	enable: enable or disable
*	
* RETURNS
*	None
*/  
void motion_sensor_shake(kal_bool enable)
{
	mmi_trace(4,"motion_sensor_shake->enable=%d",enable);
	if(enable)/*ON*/
	{
		gsensor_SetCallback(motion_sensor_shake_callback);
		gsensor_start_monitor(35);
	}  
	else/*OFF*/
	{
		gsensor_SetCallback(NULL);
		gsensor_stop_monitor();
	}   
}   
/*
* FUNCTION                                                            
*	motion_sensor_turn
*
* DESCRIPTION                                                           
*   	This function is to enable or disable sample function
*
* CALLS  
*
* PARAMETERS
*	enable: enable or disable
*	
* RETURNS
*	None
*/  
void motion_sensor_turn(kal_bool enable)
{
	mmi_trace(4,"motion_sensor_turn->enable=%d",enable);

   if(enable)/*ON*/
   {
		gsensor_SetCallback(motion_sensor_turn_callback);
		gsensor_start_monitor(35);
   }  
   else/*OFF*/
   {
		gsensor_SetCallback(NULL);
		gsensor_stop_monitor();
   }   
}
/*
* FUNCTION                                                            
*	motion_sensor_rotate
*
* DESCRIPTION                                                           
*   	This function is to enable or disable sample function
*
* CALLS  
*
* PARAMETERS
*	enable: enable or disable
*	
* RETURNS
*	None
*/  
void motion_sensor_rotate(kal_bool enable)
{
	mmi_trace(4,"motion_sensor_rotate->enable=%d",enable);
   if(enable)/*ON*/
   {
		gsensor_SetCallback(motion_sensor_rotate_callback);
		gsensor_start_monitor(35);
   }  
   else/*OFF*/
   {
		gsensor_SetCallback(NULL);
		gsensor_stop_monitor();
   }
}

#endif
