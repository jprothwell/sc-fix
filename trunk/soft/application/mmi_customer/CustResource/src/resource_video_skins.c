// dingjian 20071217 for 176*220
#include "mmi_features.h"

#include "mmi_data_types.h"
#include "gdi_lcd_config.h"
#include "mmi_features_video.h"
#include "resource_video_skins.h"

#ifdef LCDSIZE_240_320 // dingjian 20071217 for 176*220

		#ifdef __VDOREC_FEATURE_TOUCH_SCREEN__		

			#ifdef __VDOREC_FEATURE_HORIZONTAL_VIEW__
				/***************************************************************************** 
				* 240X320 [TouchScreen][Horizontal] Video Recoder Skin
				*****************************************************************************/
			  const	vdorec_osd_vdorec_layout_struct g_vdorec_osd_cntx =
				{
					/************************************/
					/* 			  BASIC SKIN 				*/
					/************************************/	
					/****** title ******/
					{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,FALSE,{0,0,0,0},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
					/****** preview_wnd ******/
					{{0,0,248,203}},
					/****** panel ******/
					{{259,91},			/* ev */
					{259,41},			/* zoom */
			       		{44,213},			/* timer */
		  			 {114,268},		/* state */
					{127,215}},	     /*status "Recording" or "Priviewing"*/
					/****** status ******/
					{TRUE,{16,11,20,100},
             				{TRUE,16,2},       /* nigth icon */
            				{TRUE,2,2},        /* led highlight icon */
            				{TRUE,37,2},       /* record aud icon */   
					 {FALSE,0,0},		/* size limit icon */
					 {FALSE,0,0}},		/* time limit icon */
					/****** softkey ******/
				 	{FALSE,					/* is_icon_softkey */
					 {0,0}, 					/* icon_lsk_pos */
					 {0,0}, 					/* icon_rsk_pos */			
					 {0,298,240,22},
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 	
					/****** hint ******/
					{TRUE,{0,100,240,40},
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* text_style */

					/************************************/
					/* 		  TOUCHSCREEN SKIN			*/
					/************************************/	
					/* touch screen addon */
					{285,114},		/* ev_inc */
					{259,114},		/* ev_dec */
					{285,64},		/* zoom_inc */
					{259,64},		/* zoom_dec */
					{79,249},		/* rec */
					{47,265}			/* stop */
				};
				/* End of 176x220 Video Recorder Skin */

			#else /* !__VDOREC_FEATURE_HORIZONTAL_VIEW__ */
			
				/***************************************************************************** 
				* 240X320 [TouchScreen] Video Recoder Skin
				*****************************************************************************/
				//jassmine touch
				const vdorec_osd_vdorec_layout_struct g_vdorec_osd_cntx =
				{
					/************************************/
					/* 			  BASIC SKIN 				*/
					/************************************/	
					/****** title ******/
					{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,TRUE,FALSE,
					{10,0,230,27},
					{255,255,255,0,0,0,VIDEO_TEXT_STYLE_BORDER}},
					/****** preview_wnd ******/
					{{0,36,240,190}},
					/****** panel ******/
					{{115,257},			/* ev */
					 {187,257},			/* zoom */
					 {17,266},			/* timer */
		  			 {187,230},		/* state */
					{101,297}},	     /*status "Recording" or "Priviewing"*/
					/****** status ******/
					{TRUE,{5,33,166,18},
					 {TRUE,16,2},		/* nigth icon */
					 {TRUE,2,2},		/* led highlight icon */
					 {TRUE,37,2},		/* record aud icon */	
					 {FALSE,0,0},		/* size limit icon */
					 {FALSE,0,0}},		/* time limit icon */
					/****** softkey ******/
				 	{FALSE,					/* is_icon_softkey */
					 {0,0}, 					/* icon_lsk_pos */
					 {0,0}, 					/* icon_rsk_pos */			
					 {0,296,240,25},
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 	
					/****** hint ******/
					{TRUE,{26,57,189,139},
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* text_style */

					/************************************/
					/* 		  TOUCHSCREEN SKIN			*/
					/************************************/	
					/* touch screen addon */
					{141,260},		/* ev_inc */
					{94,260},		/* ev_dec */
					{214,260},		/* zoom_inc */
					{166,260},		/* zoom_dec */
					{101,297},		/* rec */
					{101,297}		/* stop */
				};
				/* End of 240X320 Video Recorder Skin */

			#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */
				
		#else /* ! __VDOREC_FEATURE_TOUCH_SCREEN__ */

			#ifdef __VDOREC_FEATURE_HORIZONTAL_VIEW__
				/***************************************************************************** 
				* 240X320 [Horizontal] Video Recoder Skin
				*****************************************************************************/
				const vdorec_osd_vdorec_layout_struct g_vdorec_osd_cntx =
				{
					/************************************/
					/* 			  BASIC SKIN 				*/
					/************************************/	
					/****** title ******/
					{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,TRUE,FALSE,{3,0,120,16},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
					/****** preview_wnd ******/
					{{0,42,240,195}},
					/****** panel ******/
					{{196,258},			/* ev */
					 {196,278},			/* zoom */
			       {13,252},			/* timer */
		  			 {114,268},		/* state */
					{196,239}},	     /*status "Recording" or "Priviewing"*/
					/****** status ******/
					{TRUE,{5,44,166,18},
					 {TRUE,16,2},		/* nigth icon */
					 {TRUE,2,2},		/* led highlight icon */
					 {TRUE,37,2},		/* record aud icon */	
					 {FALSE,0,0},		/* size limit icon */
					 {FALSE,0,0}},		/* time limit icon */
					/****** softkey ******/
				 	{FALSE,					/* is_icon_softkey */
					 {0,0}, 					/* icon_lsk_pos */
					 {0,0}, 					/* icon_rsk_pos */			
					 {0,298,240,2},
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 	
					/****** hint ******/
					{TRUE,{0,100,240,40},
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}}	/* text_style */
				};
				/* End of 240X320 Video Recorder Skin */

			#else /* !__VDOREC_FEATURE_HORIZONTAL_VIEW__ */
				/***************************************************************************** 
				* 240X320 Video Recoder Skin
				*****************************************************************************/
				//jasper
				const vdorec_osd_vdorec_layout_struct g_vdorec_osd_cntx =
				{
					/************************************/
					/* 			  BASIC SKIN 				*/
					/************************************/	
					/****** title ******/
					{VIDEO_ALIGN_CENTER,VIDEO_ALIGN_CENTER,TRUE,FALSE,{66,5,108,25},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
					/****** preview_wnd ******/
					{{0,40,240,180}},
					/****** panel ******/
					{{66,224},			/* ev */
					 {147,224},			/* zoom */
			       	{97,224},			/* timer */
		  			 {114,268},		/* state */
					{196,239}},	     /*status "Recording" or "Priviewing"*/
					/****** status ******/
					{TRUE,{5,44,166,18},
					 {TRUE,16,2},		/* nigth icon */
					 {TRUE,2,2},		/* led highlight icon */
					 {TRUE,37,2},		/* record aud icon */	
					 {FALSE,0,0},		/* size limit icon */
					 {FALSE,0,0}},		/* time limit icon */
					/****** softkey ******/
				 	{FALSE,					/* is_icon_softkey */
					 {0,0}, 					/* icon_lsk_pos */
					 {0,0}, 					/* icon_rsk_pos */			
					 {0,298,240,20},
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 	
					/****** hint ******/
					{TRUE,{0,100,240,40},
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}}	/* text_style */
				};
				/* End of 240X320 Video Recorder Skin */


			#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */
				
		#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ */



		#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__

			#ifdef __VDOREC_FEATURE_HORIZONTAL_VIEW__
		
				/***************************************************************************** 
				* 240X320 [TouchScreen][Horizontal] Video Player Skin
				*****************************************************************************/
				const vdoply_osd_vdoply_layout_struct g_vdoply_osd_cntx =
				{
					/************************************/
					/* 			  BASIC SKIN 				*/
					/************************************/			
					/****** title ******/
					{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,TRUE,{0,0,0,0},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
					/****** play wnd ******/
					{{0,41,240,195}},
					/****** panel ******/
					{{214,262},			/* speed */
        		   		 {167,262},			/* audio */				
        		   		 {181,242},			/* timer */
					 {0,0},				/* status */ /* not used */
					 {{0,236},{34,241},{34,241}}},	/* progress timer bar, bg, fill, empty */
					/****** softkey ******/
				 	{FALSE,					/* is_icon_softkey */
					 {0,0}, 					/* icon_lsk_pos */
					 {0,0}, 					/* icon_rsk_pos */			
					 {170,299,70,21}, 	/* rect */
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 			 	

					/************************************/
					/* 		  TOUCHSCREEN SKIN			*/
					/************************************/	
		   		/* touch screen addon */
					{141,262},	/* touch_speed_inc */
					{141,279},	/* touch_speed_dec */
					{189,262},	/* touch_volume_inc */
					{189,279},	/* touch_volume_dec */
					{147,236},	/* touch_progress_inc */
					{0,236},		/* touch_progress_dec */
					{30,276},	/* touch_snap */
					{3,276},		/* touch_fullscreen */
					{87,259},	/* touch_play */
					{53,276},	/* touch_stop */

					#ifdef __VDOPLY_FEATURE_FULLSCREEN__
						/************************************/
						/* 			FULLSCREEN SKIN			*/
						/************************************/	
						/****** fullscreen softkey ******/
					 	{FALSE,					/* is_icon_softkey */
						 {0,0}, 					/* icon_lsk_pos */
						 {0,0}, 					/* icon_rsk_pos */	
						 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 	 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */
					 	/****** fullscreen panel ******/	
						{{5,195,308,40}, 	/* full_panel rect */
			  			 {272,198},			/* full_speed */
			   		 {225,198},			/* full_audio */				
			   		 {21,219},			/* full_timer */
						 {0,0},				/* full_status */	/* not used */
						 {{12,196},{45,200},{45,200}}},	/* progress timer bar, bg, fill, empty */

						/************************************/
						/*   TOUCHSCREEN FULLSCREEN SKIN		*/
						/************************************/	
		 				/* touch screen addon */	   		 
						{246,198},	/* full_touch_speed_inc */
						{246,215},	/* full_touch_speed_dec */
						{198,198},	/* full_touch_volume_inc */
						{198,215},	/* full_touch_volume_dec */
						{158,197},	/* full_touch_progress_inc */
						{11,197},	/* full_touch_progress_dec */
						{78,215},	/* full_touch_play */
						{134,215},	/* full_touch_stop */
			   		 
					#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
					
					0 /* dummy_end */
				};
				/* End of 240X320 Video Player Skin */
				
			#else /* ! __VDOREC_FEATURE_HORIZONTAL_VIEW__ */

				/***************************************************************************** 
				* 240X320 [TouchScreen] Video Player Skin
				*****************************************************************************/
				//jasssmine touch
				const vdoply_osd_vdoply_layout_struct g_vdoply_osd_cntx =
				{
					/************************************/
					/* 			  BASIC SKIN 				*/
					/************************************/			
					/****** title ******/
          {VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,TRUE,
          {10,0,230,27},
          {255,255,255,0,0,0,VIDEO_TEXT_STYLE_BORDER}},
					/****** play wnd ******/
					{{0,27,240,196}},
					/****** panel ******/
					{{138,258},             /* speed */
		   		 {198,258},//{195,280},	/* volume */ 				
		   		 {56,266},              /* timer */
					 {100,297},//{71,258},			/* status */
					 {{32,230}/*{1,237}*/,{32,230},{32,230}}},	/* progress timer bar, bg, fill, empty */
					/****** softkey ******/
				 	{FALSE,					/* is_icon_softkey */
					 {0,0}, 					/* icon_lsk_pos */
					 {0,0}, 					/* icon_rsk_pos */	
					 {0,296,240,25}, 		/* rect */
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 			 	

					/************************************/
					/* 		  TOUCHSCREEN SKIN			*/
					/************************************/	
		   		/* touch screen addon */
					{211,260},	/* touch_speed_inc */
					{162,260},	/* touch_speed_dec */
					{223,260},	/* touch_volume_inc */
					{182,260},	/* touch_volume_dec */
					{211,227},	/* touch_progress_inc */
					{5,227},		/* touch_progress_dec */
					{30,276},	/* touch_snap */
					{3,265},		/* touch_fullscreen */
					{103,295},	/* touch_play */
					{103,295},	/* touch_stop */

					#ifdef __VDOPLY_FEATURE_FULLSCREEN__
						/************************************/
						/* 			FULLSCREEN SKIN			*/
						/************************************/	
						/****** fullscreen softkey ******/
					 	{FALSE,					/* is_icon_softkey */
						 {0,0}, 					/* icon_lsk_pos */
						 {0,0}, 					/* icon_rsk_pos */	
						 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 	 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */
					 	/****** fullscreen panel ******/	
						{{5,195,308,40}, 	/* full_panel rect */
			  			 {272,198},			/* full_speed */
			   		 {225,198},			/* full_audio */				
			   		 {21,219},			/* full_timer */
						 {0,0},				/* full_status */	/* not used */
						 {{12,196},{45,200},{45,200}}},	/* progress timer bar, bg, fill, empty */

						/************************************/
						/*   TOUCHSCREEN FULLSCREEN SKIN		*/
						/************************************/	
		 				/* touch screen addon */	   		 
						{246,198},	/* full_touch_speed_inc */
						{246,215},	/* full_touch_speed_dec */
						{198,198},	/* full_touch_volume_inc */
						{198,215},	/* full_touch_volume_dec */
						{158,197},	/* full_touch_progress_inc */
						{11,197},	/* full_touch_progress_dec */
						{78,215},	/* full_touch_play */
						{134,215},	/* full_touch_stop */
			   		 
					#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
					
					0 /* dummy_end */
				};
				/* End of 240X320 Video Player Skin */

			#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */

				
		#else /* !__VDOREC_FEATURE_TOUCH_SCREEN__ */

			#ifdef __VDOREC_FEATURE_HORIZONTAL_VIEW__
		
				/***************************************************************************** 
				* 240X320 [Horizontal] Video Player Skin
				*****************************************************************************/
				const vdoply_osd_vdoply_layout_struct g_vdoply_osd_cntx =
				{
					/************************************/
					/* 			  BASIC SKIN 				*/
					/************************************/			
					/****** title ******/
					{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,TRUE,{3,0,120,16},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
					/****** play wnd ******/
					{{0,41,240,195}},
					/****** panel ******/
					{{195,258},			/* speed */
		   		 {195,280},			/* audio */				
		   		 {8,268},			/* timer */
					 {71,258},			/* status */
					 {{1,237},{37,237},{37,237}}},	/* progress timer bar, bg, fill, empty */
					/****** softkey ******/
				 	{FALSE,					/* is_icon_softkey */
					 {0,0}, 					/* icon_lsk_pos */
					 {0,0}, 					/* icon_rsk_pos */									
					 {0,298,240,22}, 		/* rect */
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 			 	

					#ifdef __VDOPLY_FEATURE_FULLSCREEN__
						/************************************/
						/* 			FULLSCREEN SKIN			*/
						/************************************/	
						/****** fullscreen softkey ******/
					 	{FALSE,					/* is_icon_softkey */
						 {0,0}, 					/* icon_lsk_pos */
						 {0,0}, 					/* icon_rsk_pos */	
						 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 	 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */
					 	/****** fullscreen panel ******/	
						{{5,200,227,35}, 	/* full_panel rect */
			  			 {130,215},			/* full_speed */
			   		 {178,215},			/* full_audio */				
			   		 {20,219},			/* full_timer */
						 {0,0},				/* full_status */	/* not used */
						 {{5,200},{25,204},{25,204}}},	/* progress timer bar, bg, fill, empty */
					#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
					
					0 /* dummy_end */
				};
				/* End of 240X320 Video Player Skin */
				
			#else /* !__VDOREC_FEATURE_HORIZONTAL_VIEW__ */
// dingjian 20071103 add for JasperP2			
 				/***************************************************************************** 
				* 240X320 Video Player Skin
				*****************************************************************************/
				const vdoply_osd_vdoply_layout_struct g_vdoply_osd_cntx =
				{
					/************************************/
					/* 			  BASIC SKIN 				*/
					/************************************/			
					/****** title ******/
					{VIDEO_ALIGN_CENTER,VIDEO_ALIGN_CENTER,TRUE,FALSE,{66,5,108,25},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
					/****** play wnd ******/
					{{0,37,240,188}},
					/****** panel ******/
					{{195,258},			/* speed */
		   		 {193,232},//{195,280},			/* audio */				
		   		 {8,235},//{8,268},			/* timer */
					 {115,267},//{71,258},			/* status */
					 {{75,225}/*{1,237}*/,{75,225},{75,225}}},	/* progress timer bar, bg, fill, empty */
					/****** softkey ******/
				 	{FALSE,					/* is_icon_softkey */
					 {0,0}, 					/* icon_lsk_pos */
					 {0,0}, 					/* icon_rsk_pos */	
					 {0,298,240,22}, 		/* rect */
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 			 	

					#ifdef __VDOPLY_FEATURE_FULLSCREEN__
						/************************************/
						/* 			FULLSCREEN SKIN			*/
						/************************************/	
						/****** fullscreen softkey ******/
					 	{FALSE,					/* is_icon_softkey */
						 {0,0}, 					/* icon_lsk_pos */
						 {0,0}, 					/* icon_rsk_pos */	
						 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 	 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */
					 	/****** fullscreen panel ******/	
						{{5,200,227,35}, 	/* full_panel rect */
			  			 {130,215},			/* full_speed */
			   		 {178,215},			/* full_audio */				
			   		 {20,219},			/* full_timer */
						 {0,0},				/* full_status */	/* not used */
						 {{5,200},{25,204},{25,204}}},	/* progress timer bar, bg, fill, empty */
					#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
					
					0 /* dummy_end */
				};
				/* End of 240X320 Video Player Skin */


			#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */
				
		#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */
#elif defined  LCDSIZE_176_220 
/***************************************************************************** 
* 176x220 Video Recoder Skin
*****************************************************************************/
			const vdorec_osd_vdorec_layout_struct g_vdorec_osd_cntx =
			{
				/************************************/
				/* 			  BASIC SKIN 				*/
				/************************************/	
				/****** title ******/
				{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,TRUE,TRUE,{3,0,120,16},{0,0,0,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
				/****** preview_wnd ******/
				{{0,15,176,150}},
				/****** panel ******/
				{{93,182},			/* ev */
				{142,182},			/* zoom */
				{6,181},			/* timer */
				{148,167},			/* state */
				{142,182}},			/* status */	  			 

				/****** status ******/
				{TRUE,{5,22,166,18},
				{FALSE,0,0},		/* nigth icon */
				{TRUE,2,2},		/* led highlight icon */
				{FALSE,0,0},		/* record aud icon */	
				{FALSE,0,0},		/* size limit icon */
				{FALSE,0,0}},		/* time limit icon */
				/****** softkey ******/
				{FALSE,					/* is_icon_softkey */
				{0,0}, 					/* icon_lsk_pos */
				{0,0}, 					/* icon_rsk_pos */		
				{0,200,176,20},
				{0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
				{0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 	
				/****** hint ******/
				{TRUE,{33,74,110,40},
				{0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}}	/* text_style */
#ifdef __VDOREC_FEATURE_TOUCH_SCREEN__
				,{110,183},		//touch_ev_inc;;
				{77,183},		//touch_ev_dec;	
				{160,183},		//touch_zoom_inc;
				{127,183},		//touch_zoom_dec;
				{80,202},		//touch_rec;
				{80,202}		//touch_stop;		
#endif
			};
			/* End of 176x220 Video Recorder Skin */


			/***************************************************************************** 
			* 176x220 Video Player Skin
			*****************************************************************************/
			//CS621 //lin modify used to cs621
			//jasper
			const vdoply_osd_vdoply_layout_struct g_vdoply_osd_cntx =
			{
				/************************************/
				/* 			  BASIC SKIN 				*/
				/************************************/			
				/****** title ******/
				{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,TRUE,{3,0,170,16},{0,0,0,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
				/****** play wnd ******/
				{{0,20,176,150}},
				/****** panel ******/
				{{54,187},			/* speed */
	   		 	{145,182},			/* audio */				
	   		 	{39,184},			/* timer */
				{83,201},			/* status */
				{{20,169},{20,169},{20,169}}},	/* progress timer bar, bg, fill, empty */
				/****** softkey ******/
				{FALSE,					/* is_icon_softkey */
				 {0,0}, 					/* icon_lsk_pos */
				 {0,0}, 					/* icon_rsk_pos */		
				 {0,200,176,20}, /* rect */
	 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 			 	
#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__

				/************************************/
				/* 		  TOUCHSCREEN SKIN			*/
				/************************************/	
	   		/* touch screen addon */
				{141,262},	/* touch_speed_inc */
				{141,279},	/* touch_speed_dec */
				{163,183},//{111,123},	/* touch_volume_inc */
				{134,183},//{108,182},	/* touch_volume_dec */
				{157,167},	/* touch_progress_inc */
				{4,167},		/* touch_progress_dec */
				{30,276},	/* touch_snap */
				{3,276},		/* touch_fullscreen */
				{83,201},	/* touch_play */
				{83,201},	/* touch_stop */
#endif
#ifdef __VDOPLY_FEATURE_FULLSCREEN__
		/************************************/
		/* 			FULLSCREEN SKIN			*/
		/************************************/	
		//video_osd_full_softkey_struct			//full_softkey;

				/****** fullscreen softkey ******/
				{FALSE,             /* is_lsk_icon */           
				{0,0},             /* icon_lsk_pos */
				{0,0},             /* icon_rsk_pos */     
				{0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},   /* lsk */
				{0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},  /* rsk */  
				//video_osd_vodply_full_panel_struct	//full_panel;	        
				/****** fullscreen panel ******/    
				{{61,68,100,43},        /* rect *///
				{99,145},          /* speed */          
				{129,145},         /* volume */
				{6,159},           /* timer */
				{0,0},  /* status */
				{{1,141},           /* bg */ 
				{20,146},          /* progress fill */
				{20,146}}},          /* progress empty */
              
		#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
			/************************************/
			/*   TOUCHSCREEN FULLSCREEN SKIN		*/
			/************************************/	
			{116,146},         /* speed_inc */
			{116,159},         /* speed_dec */       
			{146,146},         /* volume_inc */
			{146,159},         /* volume_dec */
			{81,144},          /* progress_inc */
      {5,144},          /* progress_dec */
			{90,146},		//full_touch_play;	
			{90,146},		//full_touch_stop;		
		#endif /* 	__VDOPLY_FEATURE_TOUCH_SCREEN__ */

	#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
				0 /* dummy_end */
			};
			
/*cong.li added for product QPAD[220*176] on 2009.08.11. */			
#elif defined  LCDSIZE_220_176 
			/***************************************************************************** 
			* 220x176 Video Recoder Skin
			*****************************************************************************/
			const vdorec_osd_vdorec_layout_struct g_vdorec_osd_cntx =
			{
				/************************************/
				/* 			  BASIC SKIN 				*/
				/************************************/	
				/****** title ******/
				{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,FALSE,{3,0,120,16},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
				/****** preview_wnd ******/
				{{5,6,176,144}},
				/****** panel ******/
				{{182,70},			/* ev */
				{182,32},			/* zoom */
				{58,156},				/* timer */
				{125,157},			/* state */
				{182,28}},			/* status */	  			 

				/****** status ******/
				{FALSE,{5,22,166,18},
				{FALSE,0,0},		/* nigth icon */
				{FALSE,2,2},		/* led highlight icon */
				{FALSE,0,0},		/* record aud icon */	
				{FALSE,0,0},		/* size limit icon */
				{FALSE,0,0}},		/* time limit icon */
				/****** softkey ******/
				{FALSE,					/* is_icon_softkey */
				{0,0}, 					/* icon_lsk_pos */
				{0,0}, 					/* icon_rsk_pos */		
				{0,160,220,16},
				{0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
				{0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 	
				/****** hint ******/
				{TRUE,{33,74,110,40},
				{0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}}	/* text_style */
				
			};
			/* End of 220x176 Video Recorder Skin */


			/***************************************************************************** 
			* 220x176 Video Player Skin
			*****************************************************************************/
			const vdoply_osd_vdoply_layout_struct g_vdoply_osd_cntx =
			{	
				/************************************/
				/* 			  BASIC SKIN 				*/
				/************************************/			
				/****** title ******/
				{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,FALSE,{3,0,170,16},{0,0,0,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
				/****** play wnd ******/
				{{6,6,176,144}},
				/****** panel ******/
				{{73,150},			/* speed */
	   		 	{182,29},			/* volume */				
	   		 	{127,156},			/* timer */
				{182,108},			/* state */
				{{7,151},{5,150},{7,151}}},	/* progress timer bar, bg, fill, empty */
				/****** softkey ******/
				{FALSE,					/* is_icon_softkey */
				 {0,0}, 					/* icon_lsk_pos */
				 {0,0}, 					/* icon_rsk_pos */		
				 {0,160,176,20}, /* rect */
	 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 			 	
#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__

				/************************************/
				/* 		  TOUCHSCREEN SKIN			*/
				/************************************/	
	   			/* touch screen addon */
				{141,262},	/* touch_speed_inc */
				{141,279},	/* touch_speed_dec */
				{155,186},//{111,123},	/* touch_volume_inc */
				{116,186},//{108,182},	/* touch_volume_dec */
				{112,180},	/* touch_progress_inc */
				{4,180},		/* touch_progress_dec */
				{30,276},	/* touch_snap */
				{3,276},		/* touch_fullscreen */
				{77,190},	/* touch_play */
				{77,190},	/* touch_stop */
#endif
#ifdef __VDOPLY_FEATURE_FULLSCREEN__
				/************************************/
				/* 			FULLSCREEN SKIN			*/
				/************************************/	
				//video_osd_full_softkey_struct			//full_softkey;

				/****** fullscreen softkey ******/
				{FALSE,             /* is_lsk_icon */           
				{0,0},             /* icon_lsk_pos */
				{0,0},             /* icon_rsk_pos */     
				{0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},   /* lsk */
				{0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},  /* rsk */  
				//video_osd_vodply_full_panel_struct	//full_panel;	        
				/****** fullscreen panel ******/    
				{{61,68,100,43},        /* rect *///
				{99,145},          /* speed */          
				{129,145},         /* volume */
				{6,159},           /* timer */
				{0,0},  /* status */
				{{1,141},           /* bg */ 
				{20,146},          /* progress fill */
				{20,146}}},          /* progress empty */
              
#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
				/************************************/
				/*   TOUCHSCREEN FULLSCREEN SKIN		*/
				/************************************/	
				{116,146},         /* speed_inc */
				{116,159},         /* speed_dec */       
				{146,146},         /* volume_inc */
				{146,159},         /* volume_dec */
				{81,144},          /* progress_inc */
	      			{5,144},          /* progress_dec */
				{90,146},		//full_touch_play;	
				{90,146},		//full_touch_stop;		
#endif /* 	__VDOPLY_FEATURE_TOUCH_SCREEN__ */

#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
				0 /* dummy_end */
			};			
#else
#if defined(CAMERA_MODULE)
/***************************************************************************** 
*
*
* MT6218B, MT6217
*
*
*****************************************************************************/

	#if defined(PAP1301) || defined(PAP1302)
	/***************************************************************************** 
	*
	* Start of PAP1301, PAP1302
	*
	*****************************************************************************/
	/***************************************************************************** 
	* OSD
	*****************************************************************************/
	
		#if ((LCD_WIDTH == 128) && (LCD_HEIGHT == 128))
		/***************************************************************************** 
		*
		* [PAP1301][PAP1302][128x128]
		*
		*****************************************************************************/

			/***************************************************************************** 
			* 128x128 Video Recoder Skin
			*****************************************************************************/
			const vdorec_osd_vdorec_layout_struct g_vdorec_osd_cntx =
			{
				/************************************/
				/* 			  BASIC SKIN 				*/
				/************************************/	
				/****** title ******/
				{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,FALSE,{0,0,0,0},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
				/****** preview_wnd ******/
				{{6,0,116,95}},
				/****** panel ******/
				{{5,97},				/* ev */
				 {33,97},			/* zoom */
		       {77,97},			/* timer */
				 {67,99}},			/* state */
				/****** status ******/
				{TRUE,{6,0,116,18},
				 {FALSE,0,0},		/* nigth icon */
				 {TRUE,2,2},		/* led highlight icon */
				 {FALSE,0,0},		/* record aud icon */	
				 {FALSE,0,0},		/* size limit icon */
				 {FALSE,0,0}},		/* time limit icon */
				/****** softkey ******/
				{FALSE,					/* is_icon_softkey */
				 {0,0}, 					/* icon_lsk_pos */
				 {0,0}, 					/* icon_rsk_pos */				 	
				 {0,110,128,18},		/* rect */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 
				/****** hint ******/
				{TRUE,{9,35,110,40},
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}}	/* text_style */
			};
			/* End of 128x128 Video Recorder Skin */


			/***************************************************************************** 
			* 128x128 Video Player Skin
			*****************************************************************************/
			const vdoply_osd_vdoply_layout_struct g_vdoply_osd_cntx =
			{
				/************************************/
				/* 			  BASIC SKIN 				*/
				/************************************/			
				/****** title ******/
				{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,FALSE,{0,0,0,0},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
				/****** play wnd ******/
				{{6,0,116,95}},
				/****** panel ******/
				{{5,98},				/* speed */
				 {93,98},			/* audio */				
				 {50,101},			/* timer */
				 {41,103},			/* status */
				 {{38,97},{43,98},{43,98}}},	/* progress timer bar, bg, fill, empty */
				/****** softkey ******/
				{FALSE,					/* is_icon_softkey */
				 {0,0}, 					/* icon_lsk_pos */
				 {0,0}, 					/* icon_rsk_pos */		
				 {0,110,128,18}, /* rect */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 			 	

				/* 128x128 do not have full screen display */
				
				0 /* dummy_end */
			};
			/* End of 128x128 Video Player Skin */

		#elif ((LCD_WIDTH == 128) && (LCD_HEIGHT == 160))
		/***************************************************************************** 
		*
		* [PAP1301][128x160]
		*
		*****************************************************************************/

			/***************************************************************************** 
			* 128x160 Video Recoder Skin
			*****************************************************************************/
			const vdorec_osd_vdorec_layout_struct g_vdorec_osd_cntx =
			{
				/************************************/
				/* 			  BASIC SKIN 				*/
				/************************************/	
				/****** title ******/
				{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,FALSE,{0,0,0,0},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
				/****** preview_wnd ******/
				{{0,6,128,104}},
				/****** panel ******/
				{{101,131},			/* ev */
				 {3,130},			/* zoom */
		       {76,116},			/* timer */
				 {57,113}},			/* state */
				/****** status ******/
				{TRUE,{0,7,60,18},
				 {FALSE,0,0},		/* nigth icon */
				 {TRUE,2,2},		/* led highlight icon */
				 {FALSE,37,2},		/* record aud icon */	
				 {FALSE,0,0},		/* size limit icon */
				 {FALSE,0,0}},		/* time limit icon */
				/****** softkey ******/
				{FALSE,					/* is_icon_softkey */
				 {0,0}, 					/* icon_lsk_pos */
				 {0,0}, 					/* icon_rsk_pos */		
				 {0,140,128,20},
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 	
				/****** hint ******/
				{TRUE,{9,35,110,40},
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}}	/* text_style */
			};
			/* End of 128x160 Video Recorder Skin */


			/***************************************************************************** 
			* 128x160 Video Player Skin
			*****************************************************************************/
			const vdoply_osd_vdoply_layout_struct g_vdoply_osd_cntx =
			{
				/************************************/
				/* 			  BASIC SKIN 				*/
				/************************************/			
				/****** title ******/
				{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,FALSE,{0,0,0,0},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
				/****** play wnd ******/
				{{0,6,128,104}},
				/****** panel ******/
				{{1,121},			/* speed */
				 {103,123},			/* audio */				
				 {47,130},			/* timer */
				 {28,121},			/* status */
				 {{0,110},{7,112},{7,112}}},	/* progress timer bar, bg, fill, empty */
				/****** softkey ******/
				{FALSE,					/* is_icon_softkey */
				 {0,0}, 					/* icon_lsk_pos */
				 {0,0}, 					/* icon_rsk_pos */		
				 {0,140,128,20}, /* rect */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 			 	

				0 /* dummy_end */
			};
			/* End of 128x160 Video Player Skin */
		#endif /* (LCD_WIDTH == XXX && LCD_HEIGHT == xxx) */
		
	#else	/* unsupported chip */

		#if ((LCD_WIDTH == 128) && (LCD_HEIGHT == 128))
		/***************************************************************************** 
		*
		* [Unsupport][128x128]
		*
		*****************************************************************************/

			/***************************************************************************** 
			* 128x128 Video Recoder Skin
			*****************************************************************************/
			const vdorec_osd_vdorec_layout_struct g_vdorec_osd_cntx =
			{
				/************************************/
				/* 			  BASIC SKIN 				*/
				/************************************/	
				/****** title ******/
				{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,FALSE,{0,0,0,0},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
				/****** preview_wnd ******/
				{{8,2,112,91}},
				/****** panel ******/
				{{5,97},				/* ev */
				 {33,97},			/* zoom */
		       {77,97},			/* timer */
				 {67,99}},			/* state */
				/****** status ******/
				{TRUE,{6,0,116,18},
				 {FALSE,0,0},		/* nigth icon */
				 {TRUE,2,2},		/* led highlight icon */
				 {FALSE,0,0},		/* record aud icon */	
				 {FALSE,0,0},		/* size limit icon */
				 {FALSE,0,0}},		/* time limit icon */
				/****** softkey ******/
				{FALSE,					/* is_icon_softkey */
				 {0,0}, 					/* icon_lsk_pos */
				 {0,0}, 					/* icon_rsk_pos */		
				 {0,110,128,18},
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 	
				/****** hint ******/
				{TRUE,{9,35,110,40},
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}}	/* text_style */
			};
			/* End of 128x128 Video Recorder Skin */


			/***************************************************************************** 
			* 128x128 Video Player Skin
			*****************************************************************************/
			const vdoply_osd_vdoply_layout_struct g_vdoply_osd_cntx =
			{
				/************************************/
				/* 			  BASIC SKIN 				*/
				/************************************/			
				/****** title ******/
				{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,FALSE,{0,0,0,0},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
				/****** play wnd ******/
				{{6,0,116,95}},
				/****** panel ******/
				{{5,98},				/* speed */
				 {93,98},			/* audio */				
				 {50,101},			/* timer */
				 {41,103},			/* status */
				 {{38,97},{43,98},{43,98}}},	/* progress timer bar, bg, fill, empty */
				/****** softkey ******/
				{FALSE,					/* is_icon_softkey */
				 {0,0}, 					/* icon_lsk_pos */
				 {0,0}, 					/* icon_rsk_pos */							
				 {0,110,128,18}, /* rect */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 			 	

				/* 128x128 do not have full screen display */
				
				0 /* dummy_end */
			};
			/* End of 128x128 Video Player Skin */

		#elif ((LCD_WIDTH == 128) && (LCD_HEIGHT == 160))
		/***************************************************************************** 
		*
		* [Unsupport][128x160]
		*
		*****************************************************************************/

			/***************************************************************************** 
			* 128x160 Video Recoder Skin
			*****************************************************************************/
			const vdorec_osd_vdorec_layout_struct g_vdorec_osd_cntx =
			{
				/************************************/
				/* 			  BASIC SKIN 				*/
				/************************************/	
				/****** title ******/
				{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,FALSE,{0,0,0,0},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
				/****** preview_wnd ******/
				{{0,10,128,100}},
				/****** panel for greenstone ******/
				{{55,125},			/* ev */
				 {90,125},			/* zoom */
		       	 {11,128},			/* timer */
				 {90,112}},			/* state */
				#if 0
				/****** panel ******/
				{{101,131},			/* ev */
				 {3,130},			/* zoom */
		       {77,116},			/* timer */
				 {57,113}},			/* state */
				 #endif
				/****** status ******/
				{TRUE,{0,7,60,18},
				 {FALSE,0,0},		/* nigth icon */
				 {TRUE,2,2},		/* led highlight icon */
				 {FALSE,37,2},		/* record aud icon */	
				 {FALSE,0,0},		/* size limit icon */
				 {FALSE,0,0}},		/* time limit icon */
				/****** softkey ******/
				{FALSE,					/* is_icon_softkey */
				 {0,0}, 					/* icon_lsk_pos */
				 {0,0}, 					/* icon_rsk_pos */							
				 {0,140,128,20},
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 	
				/****** hint ******/
				{TRUE,{9,35,110,40},
				 {67,87,153,255,255,255,VIDEO_TEXT_STYLE_BORDER}}	/* text_style */
			};
			/* End of 128x160 Video Recorder Skin */


			/***************************************************************************** 
			* 128x160 Video Player Skin
			*****************************************************************************/
			//TBM780 use
			const vdoply_osd_vdoply_layout_struct g_vdoply_osd_cntx =
			{
				/************************************/
				/* 			  BASIC SKIN 				*/
				/************************************/			
				/****** title ******/
				{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,FALSE,{0,0,0,0},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
				/****** play wnd ******/
				{{0,6,128,104}},
				/****** panel ******/
				{{1,121},			/* speed */
				 {99,124},			/* audio */				
				 {30,128},			/* timer */
				 {55,141},			/* status */
				 {{16,113},{16,113},{16,113}}},	/* progress timer bar, bg, fill, empty */
				/****** softkey ******/
				{FALSE,					/* is_icon_softkey */
				 {0,0}, 					/* icon_lsk_pos */
				 {0,0}, 					/* icon_rsk_pos */							
				 {0,140,128,20}, 		/* rect */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 			 	
				#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
				{141,262},	/* touch_speed_inc */
				{141,279},	/* touch_speed_dec */
				{189,262},	/* touch_volume_inc */
				{189,279},	/* touch_volume_dec */
				{147,236},	/* touch_progress_inc */
				{0,236},		/* touch_progress_dec */
				{30,276},	/* touch_snap */
				{3,276},		/* touch_fullscreen */
				{87,259},	/* touch_play */
				{53,276},	/* touch_stop */	
				#endif /* 	__VDOPLY_FEATURE_TOUCH_SCREEN__ */	
				
				/************************************/
				/* 			FULLSCREEN SKIN			*/
				/************************************/
				#ifdef __VDOPLY_FEATURE_FULLSCREEN__
				/****** fullscreen softkey ******/
				{FALSE,					/* is_icon_softkey */
				{0,0}, 					/* icon_lsk_pos */
				{0,0}, 					/* icon_rsk_pos */	
				{0,0,0,0,0,0,0},	/* lsk */
				 {0,0,0,0,0,0,0}},	/* rsk */
				/****** fullscreen panel ******/	
				{{0,0,0,0}, 	/* full_panel rect */
				 {0,0},			/* full_speed */
				{0,0},			/* full_audio */				
				{0,0},			/* full_timer */
				{0,0},				/* full_status */	/* not used */
				{{0,0},{0,0},{0,0}}},	/* progress timer bar, bg, fill, empty */

				/************************************/
				/*   TOUCHSCREEN FULLSCREEN SKIN		*/
				/************************************/	
				/* touch screen addon */	
				#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__
				{0,0},	/* full_touch_speed_inc */
				{0,0},	/* full_touch_speed_dec */
				{0,0},	/* full_touch_volume_inc */
				{0,0},	/* full_touch_volume_dec */
				{0,0},	/* full_touch_progress_inc */
				{0,0},	/* full_touch_progress_dec */
				{0,0},	/* full_touch_play */
				{0,0},	/* full_touch_stop */
				#endif
				#endif
				0 /* dummy_end */
			};
			/* End of 128x160 Video Player Skin */
		
		#endif /* (LCD_WIDTH == XXX && LCD_HEIGHT == xxx) */

	#endif /* end of backend chip */


#elif defined(ISP_SUPPORT)
/***************************************************************************** 
*
*
* MT6219
*
*
*****************************************************************************/

	#if ((LCD_WIDTH == 128) && (LCD_HEIGHT == 128))
	/***************************************************************************** 
	*
	* [MT6219][128x128]
	*
	*****************************************************************************/

		/***************************************************************************** 
		* 128x128 Video Recoder Skin
		*****************************************************************************/
		const vdorec_osd_vdorec_layout_struct g_vdorec_osd_cntx =
		{
			/************************************/
			/* 			  BASIC SKIN 				*/
			/************************************/	
			/****** title ******/
			{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,FALSE,{0,0,0,0},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
			/****** preview_wnd ******/
			{{6,0,116,95}},
			/****** panel ******/
			{{5,97},				/* ev */
			 {33,97},			/* zoom */
	       {77,97},			/* timer */
			 {67,99}},			/* state */
			/****** status ******/
			{TRUE,{6,0,116,18},
			 {TRUE,16,2},		/* nigth icon */
			 {TRUE,2,2},		/* led highlight icon */
			 {TRUE,37,2},		/* record aud icon */	
			 {FALSE,0,0},		/* size limit icon */
			 {FALSE,0,0}},		/* time limit icon */
			/****** softkey ******/
			{FALSE,					/* is_icon_softkey */
			 {0,0}, 					/* icon_lsk_pos */
			 {0,0}, 					/* icon_rsk_pos */		
			 {0,110,128,18},
			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 	
			/****** hint ******/
			{TRUE,{9,35,110,40},
			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}}	/* text_style */
		};
		/* End of 128x128 Video Recorder Skin */


		/***************************************************************************** 
		* 128x160 Video Player Skin
		*****************************************************************************/
		const vdoply_osd_vdoply_layout_struct g_vdoply_osd_cntx =
		{
			/************************************/
			/* 			  BASIC SKIN 				*/
			/************************************/			
			/****** title ******/
			{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,FALSE,{0,0,0,0},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
			/****** play wnd ******/
			{{6,0,116,95}},
			/****** panel ******/
			{{5,98},				/* speed */
			 {93,98},			/* audio */				
			 {50,101},			/* timer */
			 {41,103},			/* status */
			 {{38,97},{43,98},{43,98}}},	/* progress timer bar, bg, fill, empty */
			/****** softkey ******/
			{FALSE,					/* is_icon_softkey */
			 {0,0}, 					/* icon_lsk_pos */
			 {0,0}, 					/* icon_rsk_pos */		
			 {0,140,128,20}, /* rect */
			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 			 	

			/* 128x128 do not have full screen display */
			
			0 /* dummy_end */
		};
		/* End of 128x128 Video Player Skin */


	#elif ((LCD_WIDTH == 128) && (LCD_HEIGHT == 160))
	/***************************************************************************** 
	*
	* [MT6219][128x160]
	*
	*****************************************************************************/

		#ifndef __VDOPLY_FEATURE_HORIZONTAL_VIEW__
			/***************************************************************************** 
			* 128x160 Video Recoder Skin [Normal View]
			*****************************************************************************/
			const vdorec_osd_vdorec_layout_struct g_vdorec_osd_cntx =
			{
				/************************************/
				/* 			  BASIC SKIN 				*/
				/************************************/	
				/****** title ******/
				{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,FALSE,{0,0,0,0},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
				/****** preview_wnd ******/
				{{0,6,128,104}},
				/****** panel ******/
				{{101,131},			/* ev */
				 {3,130},			/* zoom */
		       {77,116},			/* timer */
				 {57,113}},			/* state */
				/****** status ******/
				{TRUE,{0,7,128,18},
				 {TRUE,16,2},		/* nigth icon */
				 {TRUE,2,2},		/* led highlight icon */
				 {TRUE,37,2},		/* record aud icon */	
				 {FALSE,0,0},		/* size limit icon */
				 {FALSE,0,0}},		/* time limit icon */
				/****** softkey ******/
				{FALSE,					/* is_icon_softkey */
				 {0,0}, 					/* icon_lsk_pos */
				 {0,0}, 					/* icon_rsk_pos */							
				 {0,140,128,20}, 		/* rect */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 	
				/****** hint ******/
				{TRUE,{9,35,110,40},
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}}	/* text_style */
			};
			/* End of 128x160 Video Recorder Skin */


			/***************************************************************************** 
			* 128x160 Video Player Skin [Normal View]
			*****************************************************************************/
			const vdoply_osd_vdoply_layout_struct g_vdoply_osd_cntx =
			{
				/************************************/
				/* 			  BASIC SKIN 				*/
				/************************************/			
				/****** title ******/
				{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,FALSE,{0,0,0,0},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
				/****** play wnd ******/
				{{0,6,128,104}},
				/****** panel ******/
				{{1,121},			/* speed */
				 {103,123},			/* audio */				
				 {48,124},			/* timer */
				 {28,121},			/* status */
				 {{0,110},{7,112},{7,112}}},	/* progress timer bar, bg, fill, empty */
				/****** softkey ******/
				{FALSE,					/* is_icon_softkey */
				 {0,0}, 					/* icon_lsk_pos */
				 {0,0}, 					/* icon_rsk_pos */		
				 {0,140,128,20}, /* rect */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 			 	

				#ifdef __VDOPLY_FEATURE_FULLSCREEN__
					/************************************/
					/* 			FULLSCREEN SKIN			*/
					/************************************/	
					/****** fullscreen softkey ******/
				 	{FALSE,					/* is_icon_softkey */
					 {0,0}, 					/* icon_lsk_pos */
					 {0,0}, 					/* icon_rsk_pos */	
					 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
				 	 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */
				 	/****** fullscreen panel ******/	
					{{2,93,106,31}, 	/* full_panel rect */
		  			 {82,109},			/* full_speed */
		   		 {9,110},			/* full_audio */
		   		 {45,109},			/* full_timer */
					 {0,0},				/* full_status */	/* not used */
					 {{6,96},{13,98},{13,98}}},	/* progress timer bar, bg, fill, empty */
				#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
				
				0 /* dummy_end */
			};
			/* End of 128x160 Video Player Skin */

		#else /* horizontal */
			/***************************************************************************** 
			* 128x160 Video Recoder Skin [Horizontal View]
			*****************************************************************************/
			const vdorec_osd_vdorec_layout_struct g_vdorec_osd_cntx =
			{
				/************************************/
				/* 			  BASIC SKIN 				*/
				/************************************/	
				/****** title ******/
				{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,FALSE,{0,0,0,0},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
				/****** preview_wnd ******/
				{{0,6,128,104}},
				/****** panel ******/
				{{101,131},			/* ev */
				 {3,130},			/* zoom */
		       {77,116},			/* timer */
				 {57,113}},			/* state */
				/****** status ******/
				{TRUE,{0,7,128,18},
				 {TRUE,16,2},		/* nigth icon */
				 {TRUE,2,2},		/* led highlight icon */
				 {TRUE,37,2},		/* record aud icon */	
				 {FALSE,0,0},		/* size limit icon */
				 {FALSE,0,0}},		/* time limit icon */
				/****** softkey ******/
				{FALSE,					/* is_icon_softkey */
				 {0,0}, 					/* icon_lsk_pos */
				 {0,0}, 					/* icon_rsk_pos */							
				 {0,110,128,18}, /* rect */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 	
				/****** hint ******/
				{TRUE,{9,35,110,40},
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}}	/* text_style */
			};
			/* End of 128x160 Video Recorder Skin */


			/***************************************************************************** 
			* 128x160 Video Player Skin [Horizontal View]
			*****************************************************************************/
			const vdoply_osd_vdoply_layout_struct g_vdoply_osd_cntx =
			{
				/************************************/
				/* 			  BASIC SKIN 				*/
				/************************************/			
				/****** title ******/
				{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,FALSE,{0,0,0,0},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
				/****** play wnd ******/
				{{0,6,128,104}},
				/****** panel ******/
				{{1,121},			/* speed */
				 {103,123},			/* audio */				
				 {48,124},			/* timer */
				 {28,121},			/* status */
				 {{0,110},{7,112},{7,112}}},	/* progress timer bar, bg, fill, empty */
				/****** softkey ******/
				{FALSE,					/* is_icon_softkey */
				 {0,0}, 					/* icon_lsk_pos */
				 {0,0}, 					/* icon_rsk_pos */							
				 {0,140,128,20}, /* rect */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
				 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 			 	

				#ifdef __VDOPLY_FEATURE_FULLSCREEN__
					/************************************/
					/* 			FULLSCREEN SKIN			*/
					/************************************/	
					/****** fullscreen softkey ******/
					{FALSE,					/* is_icon_softkey */
					 {0,0}, 					/* icon_lsk_pos */
					 {0,0}, 					/* icon_rsk_pos */	
					 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */
				 	/****** fullscreen panel ******/	
					{{2,93,106,31}, 	/* full_panel rect */
		  			 {82,109},			/* full_speed */
		   		 {10,100},			/* full_audio */				
		   		 {45,109},			/* full_timer */
					 {0,0},				/* full_status */	/* not used */
					 {{7,98},{13,98},{13,98}}},	/* progress timer bar, bg, fill, empty */
				#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
				
				0 /* dummy_end */
			};
			/* End of 128x160 Video Player Skin */

		#endif /* __VDOPLY_FEATURE_HORIZONTAL_VIEW__ */

	#elif ( (LCD_WIDTH == 240) && (LCD_HEIGHT == 320) )
	/***************************************************************************** 
	*
	* [MT6219][240x320]
	*
	*****************************************************************************/

		#ifdef __VDOREC_FEATURE_TOUCH_SCREEN__		

			#ifdef __VDOREC_FEATURE_HORIZONTAL_VIEW__
				/***************************************************************************** 
				* 240X320 [TouchScreen][Horizontal] Video Recoder Skin
				*****************************************************************************/
				const vdorec_osd_vdorec_layout_struct g_vdorec_osd_cntx =
				{
					/************************************/
					/* 			  BASIC SKIN 				*/
					/************************************/	
					/****** title ******/
					{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,FALSE,{0,0,0,0},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
					/****** preview_wnd ******/
					{{0,42,240,195}},
					/****** panel ******/
					{{166,257},			/* ev */
					 {166,277},			/* zoom */
			       {20,247},			/* timer */
		  			 {0,0}},				/* state */ /* not used */
					/****** status ******/
					{TRUE,{5,2,166,18},
					 {TRUE,16,44},		/* nigth icon */
					 {TRUE,2,44},		/* led highlight icon */
					 {TRUE,37,44},		/* record aud icon */	
					 {FALSE,0,0},		/* size limit icon */
					 {FALSE,0,0}},		/* time limit icon */
					/****** softkey ******/
				 	{FALSE,					/* is_icon_softkey */
					 {0,0}, 					/* icon_lsk_pos */
					 {0,0}, 					/* icon_rsk_pos */			
					 {0,298,240,22},
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 	
					/****** hint ******/
					{TRUE,{0,100,240,40},
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* text_style */

					/************************************/
					/* 		  TOUCHSCREEN SKIN			*/
					/************************************/	
					/* touch screen addon */
					{209,257},		/* ev_inc */
					{140,257},		/* ev_dec */
					{209,278},		/* zoom_inc */
					{140,278},		/* zoom_dec */
					{79,249},		/* rec */
					{47,265}			/* stop */
				};
				/* End of 176x220 Video Recorder Skin */

			#else /* !__VDOREC_FEATURE_HORIZONTAL_VIEW__ */
			
				/***************************************************************************** 
				* 240X320 [TouchScreen] Video Recoder Skin
				*****************************************************************************/
				const vdorec_osd_vdorec_layout_struct g_vdorec_osd_cntx =
				{
					/************************************/
					/* 			  BASIC SKIN 				*/
					/************************************/	
					/****** title ******/
					{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,TRUE,FALSE,{0,18,240,22},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
					/****** preview_wnd ******/
					{{0,42,240,195}},
					/****** panel ******/
					{{166,257},			/* ev */
					 {166,277},			/* zoom */
			       {20,247},			/* timer */
		  			 {0,0}},				/* state */ /* not used */
					/****** status ******/
					{TRUE,{5,2,166,18},
					 {TRUE,16,44},		/* nigth icon */
					 {TRUE,2,44},		/* led highlight icon */
					 {TRUE,37,44},		/* record aud icon */	
					 {FALSE,0,0},		/* size limit icon */
					 {FALSE,0,0}},		/* time limit icon */
					/****** softkey ******/
				 	{FALSE,					/* is_icon_softkey */
					 {0,0}, 					/* icon_lsk_pos */
					 {0,0}, 					/* icon_rsk_pos */			
					 {0,298,240,22},
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 	
					/****** hint ******/
					{TRUE,{0,100,240,40},
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* text_style */

					/************************************/
					/* 		  TOUCHSCREEN SKIN			*/
					/************************************/	
					/* touch screen addon */
					{209,257},		/* ev_inc */
					{140,257},		/* ev_dec */
					{209,278},		/* zoom_inc */
					{140,278},		/* zoom_dec */
					{79,249},		/* rec */
					{47,265}			/* stop */
				};
				/* End of 176x220 Video Recorder Skin */

			#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */
				
		#else /* ! __VDOREC_FEATURE_TOUCH_SCREEN__ */

			#ifdef __VDOREC_FEATURE_HORIZONTAL_VIEW__
				/***************************************************************************** 
				* 240X320 [Horizontal] Video Recoder Skin
				*****************************************************************************/
				const vdorec_osd_vdorec_layout_struct g_vdorec_osd_cntx =
				{
					/************************************/
					/* 			  BASIC SKIN 				*/
					/************************************/	
					/****** title ******/
					{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,TRUE,FALSE,{3,0,120,16},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
					/****** preview_wnd ******/
					{{0,42,240,195}},
					/****** panel ******/
					{{196,258},			/* ev */
					 {196,278},			/* zoom */
			       {13,252},			/* timer */
		  			 {80,246}},			/* state */
					/****** status ******/
					{TRUE,{5,44,166,18},
					 {TRUE,16,2},		/* nigth icon */
					 {TRUE,2,2},		/* led highlight icon */
					 {TRUE,37,2},		/* record aud icon */	
					 {FALSE,0,0},		/* size limit icon */
					 {FALSE,0,0}},		/* time limit icon */
					/****** softkey ******/
				 	{FALSE,					/* is_icon_softkey */
					 {0,0}, 					/* icon_lsk_pos */
					 {0,0}, 					/* icon_rsk_pos */			
					 {0,298,240,2},
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 	
					/****** hint ******/
					{TRUE,{0,100,240,40},
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}}	/* text_style */
				};
				/* End of 240X320 Video Recorder Skin */

			#else /* !__VDOREC_FEATURE_HORIZONTAL_VIEW__ */
				/***************************************************************************** 
				* 240X320 Video Recoder Skin
				*****************************************************************************/
				const vdorec_osd_vdorec_layout_struct g_vdorec_osd_cntx =
				{
					/************************************/
					/* 			  BASIC SKIN 				*/
					/************************************/	
					/****** title ******/
					{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,TRUE,FALSE,{3,0,120,16},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
					/****** preview_wnd ******/
					{{0,42,240,195}},
					/****** panel ******/
					{{196,258},			/* ev */
					 {196,278},			/* zoom */
			       {13,252},			/* timer */
		  			 {80,246}},			/* state */
					/****** status ******/
					{TRUE,{5,44,166,18},
					 {TRUE,16,2},		/* nigth icon */
					 {TRUE,2,2},		/* led highlight icon */
					 {TRUE,37,2},		/* record aud icon */	
					 {FALSE,0,0},		/* size limit icon */
					 {FALSE,0,0}},		/* time limit icon */
					/****** softkey ******/
				 	{FALSE,					/* is_icon_softkey */
					 {0,0}, 					/* icon_lsk_pos */
					 {0,0}, 					/* icon_rsk_pos */			
					 {0,298,240,2},
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 	
					/****** hint ******/
					{TRUE,{0,100,240,40},
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}}	/* text_style */
				};
				/* End of 240X320 Video Recorder Skin */


			#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */
				
		#endif /* __VDOREC_FEATURE_TOUCH_SCREEN__ */



		#ifdef __VDOPLY_FEATURE_TOUCH_SCREEN__

			#ifdef __VDOREC_FEATURE_HORIZONTAL_VIEW__
		
				/***************************************************************************** 
				* 240X320 [TouchScreen][Horizontal] Video Player Skin
				*****************************************************************************/
				const vdoply_osd_vdoply_layout_struct g_vdoply_osd_cntx =
				{
					/************************************/
					/* 			  BASIC SKIN 				*/
					/************************************/			
					/****** title ******/
					{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,TRUE,{0,0,0,0},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
					/****** play wnd ******/
					{{0,41,240,195}},
					/****** panel ******/
					{{214,262},			/* speed */
		   		 {167,262},			/* audio */				
		   		 {181,242},			/* timer */
					 {0,0},				/* status */ /* not used */
					 {{0,236},{34,241},{34,241}}},	/* progress timer bar, bg, fill, empty */
					/****** softkey ******/
				 	{FALSE,					/* is_icon_softkey */
					 {0,0}, 					/* icon_lsk_pos */
					 {0,0}, 					/* icon_rsk_pos */			
					 {170,299,70,21}, 	/* rect */
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 			 	

					/************************************/
					/* 		  TOUCHSCREEN SKIN			*/
					/************************************/	
		   		/* touch screen addon */
					{141,262},	/* touch_speed_inc */
					{141,279},	/* touch_speed_dec */
					{189,262},	/* touch_volume_inc */
					{189,279},	/* touch_volume_dec */
					{147,236},	/* touch_progress_inc */
					{0,236},		/* touch_progress_dec */
					{30,276},	/* touch_snap */
					{3,276},		/* touch_fullscreen */
					{87,259},	/* touch_play */
					{53,276},	/* touch_stop */

					#ifdef __VDOPLY_FEATURE_FULLSCREEN__
						/************************************/
						/* 			FULLSCREEN SKIN			*/
						/************************************/	
						/****** fullscreen softkey ******/
					 	{FALSE,					/* is_icon_softkey */
						 {0,0}, 					/* icon_lsk_pos */
						 {0,0}, 					/* icon_rsk_pos */	
						 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 	 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */
					 	/****** fullscreen panel ******/	
						{{5,195,308,40}, 	/* full_panel rect */
			  			 {272,198},			/* full_speed */
			   		 {225,198},			/* full_audio */				
			   		 {21,219},			/* full_timer */
						 {0,0},				/* full_status */	/* not used */
						 {{12,196},{45,200},{45,200}}},	/* progress timer bar, bg, fill, empty */

						/************************************/
						/*   TOUCHSCREEN FULLSCREEN SKIN		*/
						/************************************/	
		 				/* touch screen addon */	   		 
						{246,198},	/* full_touch_speed_inc */
						{246,215},	/* full_touch_speed_dec */
						{198,198},	/* full_touch_volume_inc */
						{198,215},	/* full_touch_volume_dec */
						{158,197},	/* full_touch_progress_inc */
						{11,197},	/* full_touch_progress_dec */
						{78,215},	/* full_touch_play */
						{134,215},	/* full_touch_stop */
			   		 
					#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
					
					0 /* dummy_end */
				};
				/* End of 240X320 Video Player Skin */
				
			#else /* ! __VDOREC_FEATURE_HORIZONTAL_VIEW__ */

				/***************************************************************************** 
				* 240X320 [TouchScreen] Video Player Skin
				*****************************************************************************/
				const vdoply_osd_vdoply_layout_struct g_vdoply_osd_cntx =
				{
					/************************************/
					/* 			  BASIC SKIN 				*/
					/************************************/			
					/****** title ******/
					{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,TRUE,{0,18,240,22},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
					/****** play wnd ******/
					{{0,41,240,195}},
					/****** panel ******/
					{{214,262},			/* speed */
		   		 {167,262},			/* audio */				
		   		 {181,242},			/* timer */
					 {0,0},				/* status */ /* not used */
					 {{0,236},{34,241},{34,241}}},	/* progress timer bar, bg, fill, empty */
					/****** softkey ******/
				 	{FALSE,					/* is_icon_softkey */
					 {0,0}, 					/* icon_lsk_pos */
					 {0,0}, 					/* icon_rsk_pos */									
					 {170,299,70,21}, /* rect */
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 			 	

					/************************************/
					/* 		  TOUCHSCREEN SKIN			*/
					/************************************/	
		   		/* touch screen addon */
					{189,262},	/* touch_speed_inc */
					{189,279},	/* touch_speed_dec */
					{141,262},	/* touch_volume_inc */
					{141,279},	/* touch_volume_dec */
					{147,236},	/* touch_progress_inc */
					{0,236},		/* touch_progress_dec */
					{30,276},	/* touch_snap */
					{3,276},		/* touch_fullscreen */
					{87,259},	/* touch_play */
					{53,276},	/* touch_stop */

					#ifdef __VDOPLY_FEATURE_FULLSCREEN__
						/************************************/
						/* 			FULLSCREEN SKIN			*/
						/************************************/	
						/****** fullscreen softkey ******/
					 	{FALSE,					/* is_icon_softkey */
						 {0,0}, 					/* icon_lsk_pos */
						 {0,0}, 					/* icon_rsk_pos */	
						 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 	 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */
					 	/****** fullscreen panel ******/	
						{{5,195,308,40}, 	/* full_panel rect */
			  			 {272,198},			/* full_speed */
			   		 {225,198},			/* full_audio */				
			   		 {21,219},			/* full_timer */
						 {0,0},				/* full_status */	/* not used */
						 {{12,196},{45,200},{45,200}}},	/* progress timer bar, bg, fill, empty */

						/************************************/
						/*   TOUCHSCREEN FULLSCREEN SKIN		*/
						/************************************/	
		 				/* touch screen addon */	   		 
						{246,198},	/* full_touch_speed_inc */
						{246,215},	/* full_touch_speed_dec */
						{198,198},	/* full_touch_volume_inc */
						{198,215},	/* full_touch_volume_dec */
						{158,197},	/* full_touch_progress_inc */
						{11,197},	/* full_touch_progress_dec */
						{78,215},	/* full_touch_play */
						{134,215},	/* full_touch_stop */
			   		 
					#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
					
					0 /* dummy_end */
				};
				/* End of 240X320 Video Player Skin */

			#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */

				
		#else /* !__VDOREC_FEATURE_TOUCH_SCREEN__ */

			#ifdef __VDOREC_FEATURE_HORIZONTAL_VIEW__
		
				/***************************************************************************** 
				* 240X320 [Horizontal] Video Player Skin
				*****************************************************************************/
				const vdoply_osd_vdoply_layout_struct g_vdoply_osd_cntx =
				{
					/************************************/
					/* 			  BASIC SKIN 				*/
					/************************************/			
					/****** title ******/
					{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,TRUE,{3,0,120,16},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
					/****** play wnd ******/
					{{0,41,240,195}},
					/****** panel ******/
					{{195,258},			/* speed */
		   		 {195,280},			/* audio */				
		   		 {8,268},			/* timer */
					 {71,258},			/* status */
					 {{1,237},{37,237},{37,237}}},	/* progress timer bar, bg, fill, empty */
					/****** softkey ******/
				 	{FALSE,					/* is_icon_softkey */
					 {0,0}, 					/* icon_lsk_pos */
					 {0,0}, 					/* icon_rsk_pos */									
					 {0,298,240,22}, 		/* rect */
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 			 	

					#ifdef __VDOPLY_FEATURE_FULLSCREEN__
						/************************************/
						/* 			FULLSCREEN SKIN			*/
						/************************************/	
						/****** fullscreen softkey ******/
					 	{FALSE,					/* is_icon_softkey */
						 {0,0}, 					/* icon_lsk_pos */
						 {0,0}, 					/* icon_rsk_pos */	
						 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 	 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */
					 	/****** fullscreen panel ******/	
						{{5,200,227,35}, 	/* full_panel rect */
			  			 {130,215},			/* full_speed */
			   		 {178,215},			/* full_audio */				
			   		 {20,219},			/* full_timer */
						 {0,0},				/* full_status */	/* not used */
						 {{5,200},{25,204},{25,204}}},	/* progress timer bar, bg, fill, empty */
					#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
					
					0 /* dummy_end */
				};
				/* End of 240X320 Video Player Skin */
				
			#else /* !__VDOREC_FEATURE_HORIZONTAL_VIEW__ */
			
 				/***************************************************************************** 
				* 240X320 Video Player Skin
				*****************************************************************************/
				const vdoply_osd_vdoply_layout_struct g_vdoply_osd_cntx =
				{
					/************************************/
					/* 			  BASIC SKIN 				*/
					/************************************/			
					/****** title ******/
					{VIDEO_ALIGN_LEFT,VIDEO_ALIGN_CENTER,FALSE,TRUE,{3,0,120,16},{255,255,255,0,0,0,VIDEO_TEXT_STYLE_NORMAL}},
					/****** play wnd ******/
					{{0,41,240,195}},
					/****** panel ******/
					{{195,258},			/* speed */
		   		 {195,280},			/* audio */				
		   		 {8,268},			/* timer */
					 {71,258},			/* status */
					 {{1,237},{37,237},{37,237}}},	/* progress timer bar, bg, fill, empty */
					/****** softkey ******/
				 	{FALSE,					/* is_icon_softkey */
					 {0,0}, 					/* icon_lsk_pos */
					 {0,0}, 					/* icon_rsk_pos */	
					 {0,298,240,22}, 		/* rect */
		 			 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */ 			 	

					#ifdef __VDOPLY_FEATURE_FULLSCREEN__
						/************************************/
						/* 			FULLSCREEN SKIN			*/
						/************************************/	
						/****** fullscreen softkey ******/
					 	{FALSE,					/* is_icon_softkey */
						 {0,0}, 					/* icon_lsk_pos */
						 {0,0}, 					/* icon_rsk_pos */	
						 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER},	/* lsk */
					 	 {0,0,0,255,255,255,VIDEO_TEXT_STYLE_BORDER}},	/* rsk */
					 	/****** fullscreen panel ******/	
						{{5,200,227,35}, 	/* full_panel rect */
			  			 {130,215},			/* full_speed */
			   		 {178,215},			/* full_audio */				
			   		 {20,219},			/* full_timer */
						 {0,0},				/* full_status */	/* not used */
						 {{5,200},{25,204},{25,204}}},	/* progress timer bar, bg, fill, empty */
					#endif /* __VDOPLY_FEATURE_FULLSCREEN__ */
					
					0 /* dummy_end */
				};
				/* End of 240X320 Video Player Skin */

			#endif /* __VDOREC_FEATURE_HORIZONTAL_VIEW__ */
				
		#endif /* __VDOPLY_FEATURE_TOUCH_SCREEN__ */

	#endif /* (LCD_WIDTH == XXX && LCD_HEIGHT == xxx) */
	
#endif /* CAMERA_MODULE or ISP_SUPPORT */

#endif /* __MMI_VIDEO__ */

