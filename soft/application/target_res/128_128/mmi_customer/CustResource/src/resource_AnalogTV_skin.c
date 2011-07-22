////////////////////////////////////////////////////////


#include "mmi_features.h"
#if defined(__MMI_ANALOGTV__)
#include "resource_analogtv_skin.h"
#include "analogtvresdefs.h"

#if defined( __MMI_MAINLCD_240X320__)
AnalogTV_OSD_Preview_Layout_struct g_AnalogTV_Preview_Layout_skin=
{

	{//AnalogTV_Preview_Playwnd_struct
		0,0,240,180
	},
	{//SIGNAL
		5,7,25,16,IMG_ID_ANALOGTV_SIGNAL
	},

	{//AnalogTV_Preview_ControlPanel_Struct

		{0,0,240,320,IMG_ID_ANALOGTV_PANNEL_BG},
		//channel	
		{
			20,216,35,20,NORMAL,
			{
				IMG_ID_ANALOGTV_CHANNEL_NORMAL,
				IMG_ID_ANALOGTV_CHANNEL_PRESSED,
				0,
				0
			}
		},
		//channel pre
		{
			15,218,20,20,NORMAL,
//			0,211,52,24,NORMAL,
			{
				
				IMG_ID_ANALOGTV_CHANNEL_PRE_NORMAL,
				IMG_ID_ANALOGTV_CHANNEL_PRE_PRESSED,
				0,
				0
			}
		},
		//channel next
		{
			75,218,20,20,NORMAL,
//			190,211,50,24,NORMAL,	
			{
				
				IMG_ID_ANALOGTV_CHANNEL_NEXT_NORMAL,
				IMG_ID_ANALOGTV_CHANNEL_NEXT_PRESSED,
				0,
				0
			}	
		},
		//my channel
		{
			40,251,35,20,NORMAL,
//			203,180,37,31,NORMAL,				
			{
				IMG_ID_ANALOGTV_MY_CHANNEL_NORMAL,
				IMG_ID_ANALOGTV_MY_CHANNEL_PRESSED,
				0,
				0
			}
		},
		//vol
		{
			0,//level
			{102,257,18,13,IMG_ID_ANALOGTV_SPEAKER_VOICE},
			{102,257,18,13,IMG_ID_ANALOGTV_SPEAKER_MUTE},		
			{//dec
				122,257,12,12,NORMAL,					
				{
					IMG_ID_ANALOGTV_VOL_DEC_NORMAL,
					IMG_ID_ANALOGTV_VOL_DEC_PRESSED,
					0,
					0
				}
			},
			{//inc
				210,257,12,12,NORMAL,					
				{
					IMG_ID_ANALOGTV_VOL_INC_NORMAL,
					IMG_ID_ANALOGTV_VOL_INC_PRESSED,
					0,
					0
				}				
			},
			{//vol indication
				134,256,75,14,IMG_ID_ANALOGTV_VOL_LEVEL
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
		//brightness
		{
			0,
			{//dec
				//7,235,26,36,NORMAL,	
				122,221,12,12,NORMAL,				
				{
					IMG_ID_ANALOGTV_BRIGHTNESS_DEC_NORMAL,
					0,
					0,
					0
				}
			},
			{//inc
				210,221,12,12,NORMAL,					
				{
					IMG_ID_ANALOGTV_BRIGHTNESS_INC_NORMAL,
					0,
					0,
					0
				}				
			},
			{//vol indication
				134,218,76,18,IMG_ID_ANALOGTV_BRIGHTNESS_LEVEL
			}
		}
		
	},
//end

//search pannel
	{0,0,240,320,IMG_ID_ANALOGTV_PANNEL_BG_SEARCH},
//search progress	
	{
		{
			29,156,203,20,IMG_ID_ANALOGTV_SEARCH_PROGRESS_BG
		},
		{
			27,150,7,16,IMG_ID_ANALOGTV_SEARCH_PROGRESS_TIC
//			29,156,7,16,IMG_ID_ANALOGTV_SEARCH_PROGRESS_TIC
		}
	},
//LSK
	{
		0,290,100,30,NORMAL,
		{
				
			IMG_ID_ANALOGTV_LSK_NORMAL,
			IMG_ID_ANALOGTV_LSK_PRESSED,
			0,
			0
		}		
	},
//RSK
	{
		130,290,110,30,NORMAL,
		{
			
			IMG_ID_ANALOGTV_RSK_NORMAL,
			IMG_ID_ANALOGTV_RSK_PRESSED,
			0,
			0
		}	
	}
};

#elif defined (__MMI_MAINLCD_176X220__)
AnalogTV_OSD_Preview_Layout_struct g_AnalogTV_Preview_Layout_skin=
{

	{//AnalogTV_Preview_Playwnd_struct
/*Suntongce anaolog tv 20080821_01*/
	//	0,8,240,188
		0,0,176,136   // xuyonggang 0,0,240,196  
/*Suntongce anaolog tv 20080821_01*/
	},
	{//SIGNAL
		15,133,25,16,IMG_ID_ANALOGTV_SIGNAL
	},

	{//AnalogTV_Preview_ControlPanel_Struct

		{0,0,176,200,IMG_ID_ANALOGTV_PANNEL_BG},
		//channel	
		{
			20,216,35,20,FALSE,NORMAL,
			{
				IMG_ID_ANALOGTV_CHANNEL_NORMAL,
				IMG_ID_ANALOGTV_CHANNEL_PRESSED,
				0,
				0
			}
		},
		//channel next
		{
			35,152,12,12,FALSE,NORMAL,
			{
				
				IMG_ID_ANALOGTV_CHANNEL_NEXT_NORMAL,
				IMG_ID_ANALOGTV_CHANNEL_NEXT_PRESSED,
				0,
				0
			}	
		},
		//channel pre
		{
			2,152,12,12,FALSE,NORMAL,
			{
				
				IMG_ID_ANALOGTV_CHANNEL_PRE_NORMAL,
				IMG_ID_ANALOGTV_CHANNEL_PRE_PRESSED,
				0,
				0
			}
		},
		//my channel
		{
			13,170,21,21,FALSE,NORMAL,
			{
				IMG_ID_ANALOGTV_MY_CHANNEL_NORMAL,
				IMG_ID_ANALOGTV_MY_CHANNEL_PRESSED,
				0,
				0
			}
		},
		//vol
		{
			0,//level
			
			{56,179,13,12,IMG_ID_ANALOGTV_SPEAKER_VOICE},
			
			{56,179,13,12,IMG_ID_ANALOGTV_SPEAKER_MUTE},
			
			{//dec
				65,179,12,12,FALSE,NORMAL,
				{
					IMG_ID_ANALOGTV_VOL_DEC_NORMAL,
					IMG_ID_ANALOGTV_VOL_DEC_PRESSED,
					0,
					0
				}
			},
			{//inc
				164,179,12,12,FALSE,NORMAL,
				{
					IMG_ID_ANALOGTV_VOL_INC_NORMAL,
					IMG_ID_ANALOGTV_VOL_INC_PRESSED,
					0,
					0
				}				
			},
			{//vol indication
				77,180,87,9//,IMG_ID_ANALOGTV_VOL_LEVEL
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
		//brightness
                {
			4,
			{//dec
				//7,235,26,36,NORMAL,	
				71,154,12,12,FALSE,NORMAL,				
				{
					IMG_ID_ANALOGTV_BRIGHTNESS_DEC_NORMAL,
					0,
					0,
					0
				}
			},
			{//inc
				166,154,12,12,FALSE,NORMAL,					
				{
					IMG_ID_ANALOGTV_BRIGHTNESS_INC_NORMAL,
					0,
					0,
					0
				}				
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

               //color dept
		{
        		65,152,12,12,FALSE,NORMAL,
        		{
        			IMG_ID_ANALOGTV_BRIGHTNESS_DEC_NORMAL,
        			0,
        			0,
        			0
        		}
		
	     },
	//record
		{
			65,152,12,12,FALSE,NORMAL,
			{
				IMG_ID_ANALOGTV_BRIGHTNESS_DEC_NORMAL,
				0,
				0,
				0
			}
		}
		
	}, //end AnalogTV_Preview_ControlPanel_Struct

//search pannel
	{0,0,176,200,IMG_ID_ANALOGTV_PANNEL_BG_SEARCH},
//search progress	
	{   
	       0,
		{
			18,176,203,20,IMG_ID_ANALOGTV_SEARCH_PROGRESS_BG
		},
		{
			28,176,222,175,IMG_ID_ANALOGTV_SEARCH_PROGRESS_TIC
		}
	},
//LSK
	{
		0,200,60,20,FALSE,NORMAL,
		{
				
			IMG_ID_ANALOGTV_LSK_NORMAL,
			IMG_ID_ANALOGTV_LSK_PRESSED,
			0,
			0
		}		
	},
//RSK
	{
		116,200,60,20,FALSE,NORMAL,
		{
			
			IMG_ID_ANALOGTV_RSK_NORMAL,
			IMG_ID_ANALOGTV_RSK_PRESSED,
			0,
			0
		}	
	}
};
#endif

#endif

