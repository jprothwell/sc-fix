#define    GSENSOR_APP_SHAKE_AUDIOPLAYER 	0x01
#define    GSENSOR_APP_SHAKE_WALLPAPER	    0x02
#define    GSENSOR_APP_SHAKE_DICE			0x04
#define    GSENSOR_APP_TURN_MUTE			0x08
#define    GSENSOR_APP_TOTATE_F1			0x10
#define	   GSENSOR_APP_SHAKE_DJ				0x20
#define    GSENSOR_APP_IMAGE_VIEWER 	0x40

typedef enum
{
	GSENSOR_ROCATE_TYPE_CLOCKWISE,
	GSENSOR_ROCATE_TYPE_ANTICLOCKWISE,
	GSENSOR_ROCATE_TYPE_MAX,
}GSENSOR_ROTATE_TYPE_E;

extern void motion_sensor_set_app_id(kal_uint32 flag);
extern void motion_sensor_clear_app_id(kal_uint32 flag);
extern void motion_sensor_shake(kal_bool enable);


