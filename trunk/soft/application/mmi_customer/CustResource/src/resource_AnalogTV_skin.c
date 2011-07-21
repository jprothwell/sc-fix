////////////////////////////////////////////////////////


#include "mmi_features.h"
#if defined(__MMI_ANALOGTV__)
#include "resource_analogtv_skin.h"
#include "analogtvresdefs.h"

AnalogTV_OSD_Preview_Layout_struct g_AnalogTV_Preview_Layout_skin=
{
//AnalogTV_Preview_Playwnd_struct
	{
		#ifdef __MMI_MAINLCD_240X320__ /*NJia for skins change*/
		0,20,240,192	//vincent modify for tv 20071122	//vincent modify 20080116
		#else
		0,0,240,196
		#endif
	},
//end
//AnalogTV_Preview_ControlPanel_Struct
	{
		#ifdef __MMI_MAINLCD_240X320__ /*NJia for skins change*/
		{12,195+31,212,54,IMG_ID_ANALOGTV_PANNEL_BG},	//vincent modify for tv 20071122
		#else
		{12,195,212,54,IMG_ID_ANALOGTV_PANNEL_BG},
		//channel	
		#endif
		{
		#ifdef __MMI_MAINLCD_240X320__ /*NJia for skins change*/
			40,207+31,35,35,FALSE,DISABLED,	//vincent modify for tv 20071122
		#else
			40,207,35,35,FALSE,DISABLED,
		#endif
			{
				IMG_ID_ANALOGTV_CHANNEL_NORMAL,
				IMG_ID_ANALOGTV_CHANNEL_PRESSED,
				0,
				0
			}
		},
		//channel pre
		{
		#ifdef __MMI_MAINLCD_240X320__ /*NJia for skins change*/
			17+33+38,195+5+31,38,44,FALSE,NORMAL,	//vincent modify for tv 20071122
		#else
			17+33+38,195+5,38,44,FALSE,NORMAL,
		#endif
			{
				
				IMG_ID_ANALOGTV_CHANNEL_PRE_NORMAL,
				IMG_ID_ANALOGTV_CHANNEL_PRE_PRESSED,
				0,
				0
			}
		},
		//channel next
		{
		#ifdef __MMI_MAINLCD_240X320__ /*NJia for skins change*/
			17+33,195+5+31,38,44,FALSE,NORMAL,	//vincent modify for tv 20071122
		#else
			17+33,195+5,38,44,FALSE,NORMAL,
		#endif
			{
				
				IMG_ID_ANALOGTV_CHANNEL_NEXT_NORMAL,
				IMG_ID_ANALOGTV_CHANNEL_NEXT_PRESSED,
				0,
				0
			}	
		},
		//vol
		{
			5,
			{93,257,0,0, 0/*IMG_ID_ANALOGTV_SPEAKER_VOICE*/},
			{93,257,0,0, 0/*IMG_ID_ANALOGTV_SPEAKER_MUTE*/},
			{//dec
		#ifdef __MMI_MAINLCD_240X320__ /*NJia for skins change*/
				17+33+38+38 ,195+5+31,36,44,FALSE,NORMAL,	//vincent modify for tv 20071122
		#else
				17+33+38+38 ,195+5,36,44,FALSE,NORMAL,
		#endif
				{
					IMG_ID_ANALOGTV_VOL_DEC_NORMAL,
					IMG_ID_ANALOGTV_VOL_DEC_PRESSED,
					0,
					0
				}
			},
			{//inc
		#ifdef __MMI_MAINLCD_240X320__ /*NJia for skins change*/
				17+33+38+38+36+18,195+5+31,36,44,FALSE,NORMAL,	//vincent modify for tv 20071122
		#else
				17+33+38+38+36+18,195+5,36,44,FALSE,NORMAL,
		#endif
				{
					IMG_ID_ANALOGTV_VOL_INC_NORMAL,
					IMG_ID_ANALOGTV_VOL_INC_PRESSED,
					0,
					0
				}				
			},
			{
		#ifdef __MMI_MAINLCD_240X320__ /*NJia for skins change*/
			17+33+38+38+36,195+5+31,18,44	//vincent modify for tv 20071122
		#else
			17+33+38+38+36,195+5,18,44
		#endif
			},
			{				
				IMG_ID_ANALOGTV_VOL_LEVEL1,
				IMG_ID_ANALOGTV_VOL_LEVEL2,
				IMG_ID_ANALOGTV_VOL_LEVEL3,
				IMG_ID_ANALOGTV_VOL_LEVEL4,
				IMG_ID_ANALOGTV_VOL_LEVEL5,
				IMG_ID_ANALOGTV_VOL_LEVEL6,
				IMG_ID_ANALOGTV_VOL_LEVEL7
			}
		},
		//my channel
		{
			31,245,35,35,FALSE,NORMAL,
			{
				IMG_ID_ANALOGTV_MY_CHANNEL_NORMAL,
				IMG_ID_ANALOGTV_MY_CHANNEL_PRESSED,
				0,
				0
			}
		},
		//brightness
		{
			145,210,1,1,FALSE,NORMAL,
			{
				
				0,
				0,
				0,
				0
			}
		},
		//color_depth
		{
			111,210,1,1,FALSE,NORMAL,
			{
				
				0,
				0,
				0,
				0
			}	
		},
		//record
		{
			179,210,1,1,FALSE,NORMAL,
			{
				
				0,
				0,
				0,
				0
			}		
		}
	},
//end

//search pannel
	#ifdef __MMI_MAINLCD_240X320__ /*NJia for skins change*/
	{0,169+51,240,32,0}, 	//vincent add for tv search progressbar 20071225
	#else
	{0,169,240,32,0}, //vincent
	#endif
	//{8,169+32,222,32,0}, //vincent
//search progress	
	{
		3,
		{
	#ifdef __MMI_MAINLCD_240X320__ /*NJia for skins change*/
			9,169+51,222,32,IMG_ID_ANALOGTV_SEARCH_PROGRESS_BG	//vincent add for tv search progressbar 20071225
	#else
			9,169,222,32,IMG_ID_ANALOGTV_SEARCH_PROGRESS_BG
	#endif
		},
		{
	#ifdef __MMI_MAINLCD_240X320__ /*NJia for skins change*/
			16,176+51,7,16,IMG_ID_ANALOGTV_SEARCH_PROGRESS_TIC	//vincent add for tv search progressbar 20071225
	#else
			16,176,7,16,IMG_ID_ANALOGTV_SEARCH_PROGRESS_TIC
	#endif
		}
	},
//LSK
#if 0
{
	U16 x;
	U16 y;
	U16 width;
	U16 height;
	BOOL is_pressed;
	Button_Status status;
	U16 image_id[MAX_STATUS];

}
#endif
	{
		0,290,100,30,FALSE,NORMAL,/*orange for LSK change*/
		{
				
			IMG_ID_ANALOGTV_LSK_NORMAL,
			IMG_ID_ANALOGTV_LSK_PRESSED,
			0,
			0
		}		
	},
//RSK
	{
		140,290,100,30,FALSE,NORMAL,/*orange for RSK change*/
		{
			
			IMG_ID_ANALOGTV_RSK_NORMAL,
			IMG_ID_ANALOGTV_RSK_PRESSED,
			0,
			0
		}	
	},
//CK
	{
		100,297,43,23,FALSE,NORMAL,
		{
			
			0,
			0,
			0,
			0
		}	
	}
};
	
#endif

