#ifndef RESOURCE_ANALOGTV_SKIN_H
#define  RESOURCE_ANALOGTV_SKIN_H

#include "mmi_features.h"
#if defined(__MMI_ANALOGTV__)
#include "mmi_data_types.h"
#include "analogtvapp.h"


typedef enum
{
	NORMAL,
	PRESSED,
	RELEASED,
	DISABLED,
	MAX_STATUS
}Button_Status;

typedef struct
{
	U16 x;
	U16 y;
}analogtv_pos;

typedef struct
{
	U16 x;
	U16 y;
	U16 width;
	U16 height;
}analogtv_rect;

typedef struct
{
	U16 x;
	U16 y;
	U16 width;
	U16 height;
	U16 image_id;	
	
}analogtv_static_pic;

typedef struct
{
	U16 x;
	U16 y;
	U16 width;
	U16 height;
	BOOL is_pressed;
	Button_Status status;
	U16 image_id[MAX_STATUS];

}analogtv_button;

typedef struct
{
	U8 level;
	analogtv_static_pic speaker_voice;
	analogtv_static_pic speaker_mute;
	analogtv_button vol_dec;
	analogtv_button vol_inc;
	analogtv_rect vol_indication;
	U16 level_imgid[MAX_VOL_LEVEL];
}analogtv_button_volume;

typedef struct
{
	U16 tic;
	analogtv_static_pic progress_bg;
	analogtv_static_pic progress_tic;
}analogtv_search_progress;
	
typedef struct
{
	analogtv_static_pic bg_pannel;
	analogtv_button channel;
	analogtv_button channel_pre;
	analogtv_button channel_next;
	analogtv_button_volume vol;
	analogtv_button mychannel;
	analogtv_button brightness;
	analogtv_button color_depth;
	analogtv_button record;
}AnalogTV_Preview_ControlPanel_Struct;


typedef struct
{
	analogtv_rect play_wnd;
	AnalogTV_Preview_ControlPanel_Struct panel;
	analogtv_static_pic searchpannel;
	analogtv_search_progress searchprogress;
	analogtv_button lsk;
	analogtv_button rsk;
	analogtv_button ck;
}AnalogTV_OSD_Preview_Layout_struct;

extern AnalogTV_OSD_Preview_Layout_struct g_AnalogTV_Preview_Layout_skin;

#endif /*__MMI_ANALOGTV__*/
#endif /*RESOURCE_ANALOGTV_SKIN_H*/
