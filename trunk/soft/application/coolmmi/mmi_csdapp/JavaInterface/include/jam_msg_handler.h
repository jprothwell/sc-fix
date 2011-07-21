#include "app_ltlcom.h"

typedef struct _mmi_java_show_alert_user_dialog_req_struct
{
    LOCAL_PARA_HDR char message[2]; /* this pointer should not be free */
} mmi_java_show_alert_user_dialog_req_struct;

typedef struct _mmi_java_play_tone_req_struct
{
	unsigned char tone_or_audio;
	ALL_TONE_ENUM tone_type;
	void * audio_date;
	unsigned int data_len;
    	U8 volume;
	mdi_callback handler;
} mmi_java_play_tone_req_struct;

typedef struct _mmi_java_install_mids_req_struct_t
{
    LOCAL_PARA_HDR
    U8 *download_url;
    U8 *local_file_path;
} mmi_java_install_mids_req_struct;

