/***************************************************************************
*
* File Name : jdd_platform.cpp
*
* IMPORTANT NOTICE
*
* Please note that any and all title and/or intellectual property rights
* in and to this Software or any part of this (including without limitation
* any images, photographs, animations, video, audio, music, text and/or
* "applets," incorporated into the Software), herein mentioned to as
* "Software", the accompanying printed materials, and any copies of the
* Software, are owned by Jataayu Software (P) Ltd., Bangalore ("Jataayu")
* or Jataayu's suppliers as the case may be. The Software is protected by
* copyright, including without limitation by applicable copyright laws,
* international treaty provisions, other intellectual property laws and
* applicable laws in the country in which the Software is being used.
* You shall not modify, adapt or translate the Software, without prior
* express written consent from Jataayu. You shall not reverse engineer,
* decompile, disassemble or otherwise alter the Software, except and
* only to the extent that such activity is expressly permitted by
* applicable law notwithstanding this limitation. Unauthorized reproduction
* or redistribution of this program or any portion of it may result in severe
* civil and criminal penalties and will be prosecuted to the maximum extent
* possible under the law. Jataayu reserves all rights not expressly granted.
*
* THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY KIND
* AND ANY AND ALL REPRESENTATION
*
***************************************************************************
*
*
* File Description
* ----------------
*
* Purpose			:	Adaptation layer for platform
*						
* Created By		:
* Created Date		:
*
*
*
* Current Revision	:
*
***************************************************************************
*
*
* Revision Details
* ----------------
*
* 1. Modified By	:
*    Modified Date	:
*	 Purpose		:
*
*
*
*
***************************************************************************/
#include <jddex_platformdefines.h>
#include "wgui_categories.h"
#include <gui_data_types.h>

#include <ddl.h>
#include <jcal.h>
#include <jddex_platform.h>
#include <jddex_wnddefaults.h>
#include <jdi_cutils.h>
#include <jddex_mmieventhandler.h>
#include <jddex_wnddefaults.h>
#include <jdi_browserengine.h>

/* Following #define is a Temporary Fix to make widgets compile using jwap include files */
#define __JATAAYU_APP__

#include "jwap_interface.h"

#define     JATAAYU_WAP_MAX_URL_NAME_SIZE      100
#define     JATAAYU_WAP_MAX_BOOKMARK_NAME_SIZE      100

#define JDDEX_LEFT_PADDING 1
#define JDDEX_DASH_LINE_WIDTH 5
#define JDDEX_DOTTED_LINE_PIXEL_WIDTH 1


// global varables for list box/combo box handling
U8* g_field_state;
JDDEX_LIST_BOX	   g_pstListBox;
U8**  g_TempListItems;
//the control id of the primay MMI control 
// whose functionality is achieved in secondary MMI e.g input box,list box, combo box
JC_UINT32 g_uiControlId = 0;
//input box globals
JC_CHAR* g_pszInputText ;
JC_CHAR* g_pszInputOrigText ;
JC_UINT32 g_uiMaxlen ;



#define M_STR_CRLF '\r'
#define convert2rgb(r,g,b) ((((unsigned char)(r)|((unsigned int)((unsigned char)(g))<<8))|((( unsigned int)(unsigned char)(b))<<16)))
#define GetRComp(rgb) ((unsigned char)(rgb))
#define GetGComp(rgb) ((unsigned char)(((unsigned int)(rgb)) >> 8))
#define GetBComp(rgb) ((unsigned char)((rgb)>>16))

#define JDDEX_MIN(v0,v1) ((v0>v1) ? v1 : v0)
#define JDDEX_MAX(v0,v1) ((v0>v1) ? v0 : v1)


#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __MMI_GB__
extern void GBSetInputboxLSKFunction(FuncPtr f); 
#endif

#ifdef __cplusplus
}
#endif




void converttoplatformrgb(JC_UINT32 uiColor, color_t* color)
{
	color->r = GetRComp(uiColor);
	color->g = GetGComp(uiColor);
	color->b = GetBComp(uiColor);
	color->alpha = 0;
}

/**
 * @brief		Initialize the mmiex module. 
 * @param[in]	pvMmiReference		handle to instance of the application.
 * @return		Appropriate error code.
 *
 * It initializes the mmiex module. 
 * 
 */

JC_RETCODE jddex_MMIInitialize (void * pvReference)
{

	return JC_OK;
}

/**
 * @brief		Draws the font considering the parent recatangle
 * @param[in]	pszText			text to be displayed
 * @param[in]	stRect			reactanle co-ordianates in
 *								which text has to be displayed
 * @param[in]	stTextStyle		style of the text
 * @return		Appropriate error code.
 *
 * This function displays a font text specified location with the
 * given font, style and color
 *
 */
JC_RETCODE jddex_PlatformDrawLabel(const JC_CHAR* pszText ,JC_RECT stRect,JC_RECT stParentRect,
					JC_STYLE stTextStyle, JC_BOOLEAN IgnBackColor)

{
	JC_UINT32 uiRenderingWidth, uiWidthInNonDispArea, uiFits ;
	 JC_CHAR* pszDispString = JC_NULL, *pszTemp;
	 JC_TEXTMETRICS TextMetrics ;
	 JC_UINT32 uiLen = 0;
	 color_t color;	

	 
	 
	 // ignore drawing if the label starts outside parent rectangle	 
	 if(stRect.iLeft >0 && 
	 	stRect.iLeft >= (JC_INT32)(stParentRect.iLeft + stParentRect.uiWidth) )
	 {
		return JC_OK;
	 }

	 if(stRect.iLeft >= stParentRect.iLeft)
	 {
		//uiRenderingWidth = stRect.uiWidth ;// - stRect.iLeft ;
		uiRenderingWidth = stParentRect.iLeft + stParentRect.uiWidth  - stRect.iLeft ;
		pszDispString = (JC_CHAR*)pszText;
	 }
	 else
	 {
		uiLen = jc_wstrlen(pszText) ;
		uiWidthInNonDispArea = stParentRect.iLeft - stRect.iLeft; // to make the value positive
		stRect.iLeft = stParentRect.iLeft ;
		jddex_PlatformGetTextMetrics ( &stTextStyle,
								  pszText,
								  uiWidthInNonDispArea,
								  &TextMetrics);
		uiFits = TextMetrics.uiNumCharacters;
		uiRenderingWidth = stParentRect.uiWidth ;
		if(uiFits >= uiLen)
		{
			return JC_OK;
		}
		else
		{
			pszDispString = (JC_CHAR*) &pszText[uiFits];
		}

	 }

	 //strip the string if it has new line character
	 if((pszTemp = (JC_CHAR* )jc_wstrchr(pszDispString,M_STR_CRLF )) != NULL)
	 {
		*pszTemp = '\0';
	 }

	 /* Change control character to 0x20 */
	 pszTemp = pszDispString;
	 while (*pszTemp)
	 {
		 if (0xA0 == *pszTemp) *pszTemp = 0x20;
		 ++pszTemp;
	 }

	 //eFont = GetMappedFont(stTextStyle.uiFontSize);

	 //JC_UINT32 uiWidth, uiActualWidth=0, uiLen ;

	 //uiLen = jc_wstrlen(pszText) ;

	 

	 jddex_PlatformGetTextMetrics ( &stTextStyle,
								  pszDispString,
								  uiRenderingWidth,
								  &TextMetrics);
	 uiLen = TextMetrics.uiNumCharacters;


	
	 if(stTextStyle.iForegroundColor < 0)
	  {
		 	stTextStyle.iForegroundColor = JDDEX_DEFAULT_BLACK;
	 }
	if(!IgnBackColor &&stTextStyle.iBackgroundColor >= 0)	 
	{	   
		//draw rect with given bg color
		converttoplatformrgb(stTextStyle.iBackgroundColor, &color);
		gui_fill_rectangle (stRect.iLeft, stRect.iTop, 
				stRect.uiWidth+stRect.iLeft, stRect.uiHeight+stRect.iTop, color);
	 	
	}	

	stRect.iLeft += JDDEX_LEFT_PADDING ;
	//pstWidgetPoint.x = stRect.iLeft ;
	//pstWidgetPoint.y = stRect.iTop;
	converttoplatformrgb(stTextStyle.iForegroundColor, &color);

	switch(stTextStyle.eTextDecoration)	  
	 {		  
		case E_DECN_NONE:			  
		break; 	  
		case E_DECN_UNDERLINE: 		  
		{
			gui_line( stRect.iLeft,
						 stRect.iTop + stRect.uiHeight-1 ,
						 stRect.iLeft + stRect.uiWidth ,
						 stRect.iTop + stRect.uiHeight-1 ,
						 color);
		 
		}			  
		break;
		case E_DECN_OVERLINE:
		{
			gui_line( stRect.iLeft, stRect.iTop,
						 stRect.iLeft + stRect.uiWidth ,
						 stRect.iTop, color);

			stRect.iTop += 1;
		}
		break;
		case E_DECN_LINE_THROUGH:
		{
			gui_line(stRect.iLeft  ,
						 stRect.iTop + + stRect.uiWidth/2,
						 stRect.iLeft + stRect.uiWidth ,
						 stRect.iTop + + stRect.uiWidth/2,
						 color);
		}		 
		break;		 
	 }

	gui_move_text_cursor(stRect.iLeft, stRect.iTop);
	converttoplatformrgb(stTextStyle.iForegroundColor, &color);
	gui_set_text_color (color) ;		
	gui_print_text (pszDispString) ;

	return JC_OK;
}


/**
 * @brief		Draws the font 
 * @param[in]	pszText			text to be displayed
 * @param[in]	stRect			reactanle co-ordianates in
 *								which text has to be displayed
 * @param[in]	stTextStyle		style of the text
 * @return		Appropriate error code.
 *
 * This function displays a font text specified location with the
 * given font, style and color
 *
 */
JC_RETCODE jddex_PlatformDrawText(const JC_CHAR* pszText ,JC_RECT stRect, 
					JC_STYLE stTextStyle, JC_BOOLEAN IgnBackColor)

{
	 JC_CHAR* pszDispString = JC_NULL, *pszTemp;
	 JC_TEXTMETRICS TextMetrics ;
	 JC_UINT32 uiLen = 0;
	 color_t color;	 

	 JC_UINT32 uiRenderingWidth, uiWidthInNonDispArea, uiFits ;
	 if(stRect.iLeft >= 0)
	 {
		uiRenderingWidth = stRect.uiWidth ;// - stRect.iLeft ;
		pszDispString = (JC_CHAR*)pszText;
	 }
	 else
	 {
		uiLen = jc_wstrlen(pszText) ;
		uiWidthInNonDispArea = -1 * stRect.iLeft; // to make the value positive
		stRect.iLeft = 0 ;
		uiRenderingWidth = jddex_GetDeviceRenderWidth();
		jddex_PlatformGetTextMetrics ( &stTextStyle,
								  pszText,
								  uiWidthInNonDispArea,
								  &TextMetrics);
		uiFits = TextMetrics.uiNumCharacters;
		if(uiFits >= uiLen)
		{
			return JC_OK;
		}
		else
		{
			pszDispString = (JC_CHAR*) &pszText[uiFits];
		}

	 } 

	 //strip the string if it has new line character
	 if((pszTemp = (JC_CHAR* )jc_wstrchr(pszDispString,M_STR_CRLF )) != NULL)
	 {
		*pszTemp = '\0';
	 }

	 /* Change control character to 0x20 */
	 pszTemp = pszDispString;
	 while (*pszTemp)
	 {
		 if (0xA0 == *pszTemp) *pszTemp = 0x20;
		 ++pszTemp;
	 }

	 //eFont = GetMappedFont(stTextStyle.uiFontSize);

	 //JC_UINT32 uiWidth, uiActualWidth=0, uiLen ;

	 //uiLen = jc_wstrlen(pszText) ;
	 jddex_PlatformGetTextMetrics ( &stTextStyle,
								  pszDispString,
								  uiRenderingWidth,
								  &TextMetrics);
		 uiLen = TextMetrics.uiNumCharacters;	 
		 
		 if(stTextStyle.iForegroundColor < 0)
		 	{
		 	stTextStyle.iForegroundColor = JDDEX_DEFAULT_BLACK;
		 	}
		 if(!IgnBackColor &&stTextStyle.iBackgroundColor >= 0)	 
		 	{	   
		 	//draw rect with given bg color
		 	converttoplatformrgb(stTextStyle.iBackgroundColor, &color);
		 	 gui_fill_rectangle (stRect.iLeft, stRect.iTop, 
			 	stRect.uiWidth+stRect.iLeft, stRect.uiHeight+stRect.iTop, color);
		 	}	

	 stRect.iLeft += JDDEX_LEFT_PADDING ;
	 //pstWidgetPoint.x = stRect.iLeft ;
	// pstWidgetPoint.y = stRect.iTop;
		 converttoplatformrgb(stTextStyle.iForegroundColor, &color);
		 
		 switch(stTextStyle.eTextDecoration)	 
		 	{		 
		 	case E_DECN_NONE:			 
				break;		 
				case E_DECN_UNDERLINE:			 
			 {
				 gui_line( stRect.iLeft,
								stRect.iTop + stRect.uiHeight-1 ,
								stRect.iLeft + stRect.uiWidth ,
								stRect.iTop + stRect.uiHeight-1 ,
								color);
					
					}			 
				break;
				case E_DECN_OVERLINE:
					{
				 gui_line( stRect.iLeft, stRect.iTop,
								stRect.iLeft + stRect.uiWidth ,
								stRect.iTop, color);
					stRect.iTop += 1;
					}
				break;
				case E_DECN_LINE_THROUGH:
					{
				 gui_line(stRect.iLeft  ,
								stRect.iTop + + stRect.uiWidth/2,
								stRect.iLeft + stRect.uiWidth ,
								stRect.iTop + + stRect.uiWidth/2,
								color);
					}		
				break;
				
			}
		 
	gui_move_text_cursor(stRect.iLeft, stRect.iTop);
	converttoplatformrgb(stTextStyle.iForegroundColor, &color);
	gui_set_text_color (color) ;	   
	gui_print_text (pszDispString) ;

	return JC_OK;
}

/**
 * @brief		Gets the text metrics. 
 * @param[in]	pStyle			pointer to font style information
 * @param[in]	psTextValue		pointer to text String
 * @param[in]   uiWidth			width of the window.
 * @param[out]  pTextMetrics	pointer to text metric structure.
 * @return		Appropriate error code.
 *
 * This function gets the text metrics of a font with the given style .
 *
 */
JC_RETCODE jddex_PlatformGetTextMetrics ( const JC_STYLE *pStyle, 
								  const JC_CHAR *psTextValue, 
								  JC_UINT32 uiWidth,
								  JC_TEXTMETRICS *pTextMetrics)
{

	JC_UINT32 uiLen = 0, uiCnt;
	uiLen = jc_wstrlen(psTextValue);	
	
	
	if(uiWidth == 0)
	{
	    mmi_trace(1,(PCSTR)"JDD_LOG: jddex_PlatformGetTextMetrics");
		gui_measure_string((JC_CHAR *)psTextValue, (S32*)&pTextMetrics->uiWidth, (S32*)&pTextMetrics->uiHeight);
		pTextMetrics->uiNumCharacters = uiLen;	
	}
	else
	{
		JC_UINT32 uiCurrWidth = 0, uiPrevWidth =0 ;
		JC_UINT32 uiMaxHeight = 0,uiCharHeight =0;
		JC_BOOLEAN bIsExceed = E_FALSE;
		S32 sWidth;

		for(uiCnt = 0 ; uiCnt < uiLen ; uiCnt++)
		{
			uiPrevWidth = uiCurrWidth ;

			gui_measure_character(psTextValue[uiCnt], &sWidth, 
										(S32*)&uiCharHeight);
			if(uiCharHeight > uiMaxHeight)
				uiMaxHeight = uiCharHeight;

			uiCurrWidth += sWidth;

			if(uiCurrWidth > uiWidth)
			{
				bIsExceed = E_TRUE;
				pTextMetrics->uiNumCharacters = uiCnt;
				break ;
			}

		}
		pTextMetrics->uiHeight = uiMaxHeight;
		if(bIsExceed)
		{
			pTextMetrics->uiWidth = uiPrevWidth ;
		}
		else
		{
			pTextMetrics->uiNumCharacters = uiLen ;
			pTextMetrics->uiWidth = uiCurrWidth ;

		}
		//mmi_trace(1,(PCSTR)"jddex_PlatformGetTextMetrics %d",uiLen);


	}

	if ( pStyle->eTextDecoration == E_DECN_UNDERLINE )
	{
		pTextMetrics->uiHeight +=1 ;
	}	
	
	//2011-6-21 comment follow line for "Unkonwn error" issue
	//pTextMetrics->uiWidth += JDDEX_LEFT_PADDING; // to make the background color look better if any.

	
	//mmi_trace(1,(PCSTR)"pTextMetrics->uiWidth %d",pTextMetrics->uiWidth);
	return JC_OK;
}

/**
 * @brief		Draws a line on LCD with a given thickness. 
 * @param[in]	pstStart		co-ordiantes of start point
 * @param[in]	pstEnd			co-ordiantes of end point
 * @param[in]	stTextStyle		style of the line
 * @param[in]	iColor			color of the line
 * @return		Appropriate error code.
 *
 * This function draws a line joining two points with specified 
 * line style, color  
 *
 */

JC_RETCODE jddex_PlatformDrawLineEx(JC_POINT* pstStart, JC_POINT* pstEnd, 
							  JC_INT32 iColor,JC_INT32 iWidth )
{
	color_t stColor;

	converttoplatformrgb(iColor, &stColor);
	gui_wline ( (S32)pstStart->uiX, (S32)pstStart->uiY, (S32)pstEnd->uiX, (S32)pstEnd->uiY, stColor, (S32)iWidth) ;
	return JC_OK;
}
/**
 * @brief		Draws a line on LCD. 
 * @param[in]	pstStart		co-ordiantes of start point
 * @param[in]	pstEnd			co-ordiantes of end point
 * @param[in]	stTextStyle		style of the line
 * @param[in]	iColor			color of the line
 * @return		Appropriate error code.
 *
 * This function draws a line joining two points with specified 
 * line style, color  
 *
 */

JC_RETCODE jddex_PlatformDrawLine(JC_POINT* pstStart, JC_POINT* pstEnd,
							E_LINE_STYLE eStyle, JC_INT32 iColor, 
							JC_INT32 uiBorderWidth, E_BORDER_NAME eBorderName)

{
	if (JC_NULL != pstStart && JC_NULL != pstEnd)
	{
		S32 x1 = pstStart->uiX;			//x coordinate of first point
		S32 y1 = pstStart->uiY;			//y coordinage of first point
		S32 x2 = pstEnd->uiX;			//x coordinate of second point
		S32 y2 = pstEnd->uiY;			//y coordinate of sconcd point
		JC_UINT32 uiWidth = (pstEnd->uiX > pstStart->uiX) ? (pstEnd->uiX - pstStart->uiX):(pstStart->uiX-pstEnd->uiX);
		JC_UINT32 uiHeight = (pstEnd->uiY > pstStart->uiY) ? (pstEnd->uiY - pstStart->uiY):(pstStart->uiY-pstEnd->uiY);
		color_t stColor;

		converttoplatformrgb(iColor, &stColor);
		if (iColor < 0)
			iColor = JDDEX_DEFAULT_BLACK;

		/* confirm to display area */

		if (x1 == x2 && y1 == y2)
		{
			// add code for pixcel
			gui_putpixel(x1, y1, stColor);	
		}
		else
		{
			switch(eStyle)
			{
			case E_SOLID_LINE:
			case E_GROOVE_LINE:
			case E_RIDGE_LINE:
			case E_INSET_LINE:
			case E_OUTSET_LINE:
				//gui_line ( x1, y1, x2, y2, stColor) ;
				gui_wline ( x1, y1, x2, y2, stColor, uiBorderWidth) ;
				break;
			case E_DOTTED_LINE:
				{
					JC_UINT32 uiCount,uiPixelCnt;
					
				      
					if( y1 == y2 ) // horizontal line
					{
						uiPixelCnt = (uiWidth+1) / 2;
						for( uiCount = 0 ; uiCount < uiPixelCnt ; uiCount++)
						{
							//mpDvrDrawPixel(MAINLCD_ID, x1, y1, iColor);
							gui_putpixel(x1, y1, stColor);
							x1 = x1 + 2 * JDDEX_DOTTED_LINE_PIXEL_WIDTH;
						}
					}
					else if( x1 == x2 ) // vertical line
					{
						uiPixelCnt = (uiHeight+1) / 2;
						for( uiCount = 0 ; uiCount < uiPixelCnt ; uiCount++)
						{
							gui_putpixel(x1, y1, stColor);
							//mpDvrDrawPixel(MAINLCD_ID, x1, y1, iColor);
							y1 = y1 + 2 * JDDEX_DOTTED_LINE_PIXEL_WIDTH;
						}
					}


				}
				break;
			case E_DASHED_LINE:
				{
					JC_UINT32 uiCount,uiDashCnt, uiRemainingPixels;

					if( y1 == y2 ) // horizontal line
					{
						if( uiWidth <= JDDEX_DASH_LINE_WIDTH)
						{
							gui_wline ( x1, y1, x2, y2, stColor, uiBorderWidth) ;
							//mpDvrDrawLine(MAINLCD_ID, x1, y1, x2, y2, iColor, uiBorderWidth);
							break;
						}
						uiDashCnt = uiWidth / (JDDEX_DASH_LINE_WIDTH*2) ;
						uiRemainingPixels = uiWidth % (JDDEX_DASH_LINE_WIDTH*2) ;
                    
						x2 = x1 + JDDEX_DASH_LINE_WIDTH;
						for( uiCount=0 ; uiCount < uiDashCnt ; uiCount++)
						{
							//mpDvrDrawLine(MAINLCD_ID, x1, y1, x2, y2, iColor, uiBorderWidth);
							gui_wline ( x1, y1, x2, y2, stColor, uiBorderWidth) ;
							x1 = x2 + JDDEX_DASH_LINE_WIDTH;
							x2 = x1 + JDDEX_DASH_LINE_WIDTH;
						}

						x2 = x2 - JDDEX_DASH_LINE_WIDTH +uiRemainingPixels;
						//mpDvrDrawLine(MAINLCD_ID, x1, y1, x2, y2, iColor, uiBorderWidth);
						gui_wline ( x1, y1, x2, y2, stColor, uiBorderWidth) ;
					}
					else if( x1 == x2 ) // vertical line
					{
						if( uiHeight <= JDDEX_DASH_LINE_WIDTH)
						{
							//mpDvrDrawLine(MAINLCD_ID, x1, y1, x2, y2, iColor, uiBorderWidth);
							gui_wline ( x1, y1, x2, y2, stColor, uiBorderWidth) ;
							break;
						}
						uiDashCnt = uiHeight / (JDDEX_DASH_LINE_WIDTH*2) ;
						uiRemainingPixels = uiHeight % (JDDEX_DASH_LINE_WIDTH*2) ;
                    
						y2 = y1 + JDDEX_DASH_LINE_WIDTH;
						for( uiCount=0 ; uiCount < uiDashCnt ; uiCount++)
						{
							//mpDvrDrawLine(MAINLCD_ID, x1, y1, x2, y2, iColor, uiBorderWidth);
							gui_wline ( x1, y1, x2, y2, stColor, uiBorderWidth) ;
							y1 = y2 + JDDEX_DASH_LINE_WIDTH;
							y2 = y1 + JDDEX_DASH_LINE_WIDTH;
						}

						y2 = y2 - JDDEX_DASH_LINE_WIDTH + uiRemainingPixels;
						//mpDvrDrawLine(MAINLCD_ID, x1, y1, x2, y2, iColor, uiBorderWidth);
						gui_wline ( x1, y1, x2, y2, stColor, uiBorderWidth) ;
					}
					
				}
				break;
			case E_DOUBLE_LINE:
				{
					JC_INT32 uiGap = uiBorderWidth - 2 ; // 2- two lines of occupying one pixile each
					if( y1 == y2 ) // horizontal line
					{
						//mpDvrDrawLine(MAINLCD_ID, x1, y1, x2, y2, iColor, 0);
						gui_line ( x1, y1, x2, y2, stColor) ;
						if( uiGap > 0 )
						{
							if( eBorderName == E_JDDEX_TOP_BORDER)
							{
								y1 = y1 + uiGap;
								y2 = y2 + uiGap;
								x1 = x1 + uiGap;
								x2 = x2 - uiGap;
								//mpDvrDrawLine(MAINLCD_ID, x1, y1, x2, y2, iColor, 0);
								gui_line ( x1, y1, x2, y2, stColor) ;
							}
							else if( eBorderName == E_JDDEX_BOTTOM_BORDER)
							{
								y1 = y1 - uiGap;
								y2 = y2 - uiGap;
								x1 = x1 + uiGap;
								x2 = x2 - uiGap;

								//mpDvrDrawLine(MAINLCD_ID, x1, y1, x2, y2, iColor, 0);
								gui_line ( x1, y1, x2, y2, stColor) ;
							}

						}

					}
					else if( x1 == x2 ) // vertical line
					{
						//mpDvrDrawLine(MAINLCD_ID, x1, y1, x2, y2, iColor, 1);
						gui_line ( x1, y1, x2, y2, stColor) ;
						if( uiGap > 0 )
						{
							if( eBorderName == E_JDDEX_LEFT_BORDER)
							{
								x1 = x1 + uiGap;
								x2 = x2 + uiGap;
								y1 = y1 + uiGap;
								y2 = y2 - uiGap;
								//mpDvrDrawLine(MAINLCD_ID, x1, y1, x2, y2, iColor, 1);
								gui_line ( x1, y1, x2, y2, stColor) ;
							}
							else if( eBorderName == E_JDDEX_RIGHT_BORDER)
							{
								x1 = x1 - uiGap;
								x2 = x2 - uiGap;
								y1 = y1 + uiGap;
								y2 = y2 - uiGap;
								//mpDvrDrawLine(MAINLCD_ID, x1, y1, x2, y2, iColor, 1);
								gui_line ( x1, y1, x2, y2, stColor) ;
							}
						}
					}
					
				}
				break;
			}
		}
	}
	return JC_OK;
}



/**
 * @brief		Erases a rectangular area on LCD. 
 * @param[in]	pstRect			co-ordiantes of rectangular region
 * @return		Appropriate error code.
 *
 * This function erases a rectangular region specified
 * as parameter
 *
 */

JC_RETCODE jddex_PlatformEraseRect(const JC_RECT * pstRect)
{
	if (JC_NULL != (void*)pstRect && 0 < pstRect->uiWidth && 0 < pstRect->uiHeight)
	{
		color_t stColor ={255,255,255,0};// white color we have to add
		gui_fill_rectangle (pstRect->iLeft, pstRect->iTop, 
			pstRect->uiWidth+pstRect->iLeft, pstRect->uiHeight+pstRect->iTop, stColor);
		
		//jddex_PlatformFillRect(pstRect, iFillColor);
	}
	return JC_OK;
}

/**
 * @brief		Draws a circle on LCD. 
 * @param[in]	pstCentre			co-ordiantes of centre of circle
 * @param[in]	iFillColor			color with which circle to be filled
 * @param[in]	iBorderColor		color of circle border
 * @return		Appropriate error code.
 *
 * This function draws a circle at a given location with specified
 * border color and fill color
 *
 */

JC_RETCODE jddex_PlatformDrawCircle(JC_POINT* pstCentre, JC_INT32 iRadius,
									JC_INT32 iBorderColor, JC_INT32 thickness )
{

	return JC_OK;
}

/**
 * @brief		Draws a pixel on LCD. 
 * @param[in]	pstPoint		co-ordiantes of pixel location
 * @param[in]	iColor			color with which pixel to be drawn
 * @return		Appropriate error code.
 *
 * This function draws a pixel at a given location with specified
 * color
 *
 */

JC_RETCODE jddex_PlatformDrawPixel(JC_POINT* pstPoint, JC_INT32 iColor )
{

	return JC_OK;
}


JC_RETCODE jddex_PlatformGetObjectSize (JC_OBJECT_INFO		*pstObjectInfo,
							JC_UINT32 *puiWidth, JC_UINT32 *puiHeight)
{	
	JC_RETCODE rCode = JC_ERR_GENERAL_FAILURE ;
	S32 gdiResult = 0;
	JC_UINT8 image_identifier = 0;
	
	if (E_OBJECT_IMAGE == pstObjectInfo->eType)
	{		
		if (E_FALSE == pstObjectInfo->bIsFileContent)
		{
#ifdef USE_FILE_FOR_IMGBUFF_DRAW
			mmi_trace (1, (PCSTR)"JDD_LOG:jddex_PlatformGetObjectSize buffer based\n");
			JDD_FILE hTempFile  = JC_NULL;
			JC_CHAR *pmFName = JC_NULL ;
			jdi_CUtilsCharToTchar (JC_NULL, JMMS_TEMP_IMAGEFILE, &pmFName) ;
	   		
			hTempFile = jdd_FSOpen (JC_NULL, pmFName, E_CREATE_WRITE_MODE) ;	
			jdd_FSWrite ((void *)(pstObjectInfo->uBuffer.pucBuffer), pstObjectInfo->uiObjectLen, 1, hTempFile);
			jdd_FSClose(hTempFile);
			gdiResult = gdi_image_get_dimension_file((S8*) pmFName,(S32*)puiWidth,(S32*)puiHeight);
			jdd_FSDelete(NULL,pmFName);
			jdd_MemFree(pmFName);
#else		
			/*gdiResult = gdi_image_get_dimension((U8*)pstObjectInfo->uBuffer.pucBuffer,\
							(S32*)puiWidth,(S32*)puiHeight) ;*/
			//jddex_PlatformGetImageType(pstObjectInfo->psMimeType, &image_identifier);
			image_identifier = gdi_get_image_type(pstObjectInfo->uBuffer.pucBuffer);
			gdiResult = gdi_image_get_dimension_mem(image_identifier,pstObjectInfo->uBuffer.pucBuffer,
							pstObjectInfo->uiObjectLen,(S32*)puiWidth,(S32*)puiHeight);

#endif

				
			mmi_trace(1,(PCSTR)"JDD_LOG:gdi_image_get_dimension returns width[%d] height[%d]", *puiWidth,*puiHeight);
		}
		else 
		{
			mmi_trace(1, (PCSTR)"JDD_LOG:jddex_PlatformGetObjectSize file based\n");
			gdiResult = gdi_image_get_dimension_file((S8*) pstObjectInfo->uBuffer.pmFileName,(S32*)puiWidth,(S32*)puiHeight);
			mmi_trace(1,(PCSTR)"JDD_LOG:gdi_image_get_dimensionfile returns width[%d] height[%d]", *puiWidth,*puiHeight);
		}		
	}
	else  if (E_OBJECT_VIDEO == pstObjectInfo->eType)
	{
		//system specific height width api should be called.
		*puiWidth = jwap_GetLcdWidth();
		*puiHeight = jwap_GetLcdHeight()-jwap_GetTitleHeight()-jwap_GetSoftkeyHeight();
	}
	if(0 == gdiResult)
	{
		rCode = JC_OK;
	}	
	return rCode ;
}

JC_RETCODE jddex_PlatformGetImageType(JC_INT8 *pcImageMimeType, JC_UINT8 *uiImageFormat)
{

	if(jc_strstr(pcImageMimeType , "image/wbmp") ||
		jc_strstr(pcImageMimeType,"image/vnd.wap.wbmp"))
	{
			*uiImageFormat = GDI_IMAGE_TYPE_WBMP ;

	}
	else if(jc_strstr(pcImageMimeType , "image/bmp") ||
		jc_strstr(pcImageMimeType,"image/x-bmp") )
	{
			*uiImageFormat = GDI_IMAGE_TYPE_BMP ;
	}
	else if(jc_strstr(pcImageMimeType, "image/gif"))
	{
			*uiImageFormat = GDI_IMAGE_TYPE_GIF ;
	}
	else if(jc_strstr(pcImageMimeType, "image/png") ||
		jc_strstr(pcImageMimeType,"image/x-png"))
	{
			*uiImageFormat = GDI_IMAGE_TYPE_PNG ;
	}
	else if(jc_strstr(pcImageMimeType, "image/jpeg") ||
			jc_strstr(pcImageMimeType, "image/jpg"))
	{
			*uiImageFormat = GDI_IMAGE_TYPE_JPG ;
	}
	else
	{
		*uiImageFormat = GDI_IMAGE_TYPE_INVALID ;
	}
	mmi_trace(1,(PCSTR)"jddex_PlatformGetImageType Image Format=%d \n",*uiImageFormat);
	return JC_OK ;
}


/**
 * @brief		Draws the image on the LCD.
 * @param[in]   pvDecodedBuff		decode buffer.
 * @param[in]  pstDispRect			co-ordiantes of the rectangle in which 
 *									image has to be alligned
 * @param[in]  pstClippedRect         rectangle area of the image to display    
 * @param[in]  eImageProperty		Property of the image
 * @return		Appropriate error code.
 *
 * This function draws part of the image on LCD at given co-ornaites
 * 
 */

//#define DRAW_FROM_FILE



JC_RETCODE jddex_PlatformDrawImage (JC_OBJECT_INFO		m_stObjectInfo,JC_RECT * pstDispRect, 
									JC_RECT * pstClippedRect,  EImageProperty eImageProperty,
									JC_CHAR* m_pmTempImageFile, JC_UINT32* puiAnimHnd)
{
	JC_RETCODE rCode = JC_OK;
	S32 gdiResult = 0;
//#ifndef USE_FILE_FOR_IMGBUFF_DRAW	
	JC_UINT8 image_identifier = 0;
//#endif
	if ( JC_NULL != pstDispRect /*&& JC_NULL != pstImageRect
	 	&& 0 != pstImageRect->uiWidth && 0 != pstImageRect->uiHeight*/
	 	&& 0 != pstDispRect->uiWidth &&  0 != pstDispRect->uiHeight
		)
	{	
	   if (E_TRUE == m_stObjectInfo.bIsFileContent)
		{
			image_identifier = (JC_UINT8)gdi_image_get_type_from_file((S8*)m_stObjectInfo.uBuffer.pmFileName);
			mmi_trace(1,(PCSTR)"widget_log: gdi_image_get_type_from_file returns %d", image_identifier);
			//if(jc_strstr(m_stObjectInfo.psMimeType, "image/gif"))
			if(image_identifier == GDI_IMAGE_TYPE_GIF_FILE)
			{
			 	gdi_handle hANimHandle ;
				if(*puiAnimHnd)
				{
					mmi_trace(1,(PCSTR)"widget_log: image exists file draw delete");
					gdi_image_stop_animation((gdi_handle)*puiAnimHnd);
					*puiAnimHnd =  JC_NULL;					
				}
				gdiResult = gdi_image_draw_animation_resized_file (
					pstDispRect->iLeft,pstDispRect->iTop, \
					pstDispRect->uiWidth, pstDispRect->uiHeight, \
					(S8*)m_stObjectInfo.uBuffer.pmFileName, &hANimHandle);
				*puiAnimHnd = (JC_UINT32)hANimHandle ;
				
				mmi_trace(1,(PCSTR)"widget_log: File Content gif image display [%d], Handle[%x] ", \
							gdiResult, hANimHandle);
			}
			else
			{
				gdiResult = gdi_image_draw_resized_file (
						pstDispRect->iLeft,pstDispRect->iTop, \
						pstDispRect->uiWidth, 
						pstDispRect->uiHeight, \
						(S8*)m_stObjectInfo.uBuffer.pmFileName) ;
			}
			mmi_trace(1,(PCSTR)"widget_log: gdi_image_draw_resized_file x[%d]y[%d]w[%d]h[%d] ", \
					pstDispRect->iLeft,pstDispRect->iTop,
					pstDispRect->uiWidth,pstDispRect->uiHeight);

		}
	   else
	   	{
		
#ifdef USE_FILE_FOR_IMGBUFF_DRAW
			
			if(jc_strstr(m_stObjectInfo.psMimeType, "image/gif"))
			{
				gdi_handle hANimHandle ;
				if(*puiAnimHnd)
				{
					gdi_image_stop_animation((gdi_handle)*puiAnimHnd);
					*puiAnimHnd =  JC_NULL;					
				}
				gdiResult = gdi_image_draw_animation_resized_file (
					pstDispRect->iLeft,pstDispRect->iTop, \
						pstDispRect->uiWidth, pstDispRect->uiHeight, \
						(S8*)m_pmTempImageFile, (gdi_handle*)&hANimHandle);
				*puiAnimHnd = (JC_UINT32)hANimHandle ;			
			}
			else
			{
			   mmi_trace(1,(PCSTR)"widget_log: decoding API invoked");
				gdiResult = gdi_image_draw_resized_file (
						pstDispRect->iLeft,pstDispRect->iTop, \
						pstDispRect->uiWidth, pstDispRect->uiHeight, \
						(S8*)m_pmTempImageFile) ;
				mmi_trace(1,(PCSTR)"widget_log: image display [%d]", gdiResult);
			}		
#else
			
			//jddex_PlatformGetImageType(m_stObjectInfo.psMimeType, &image_identifier);
			image_identifier = gdi_get_image_type(m_stObjectInfo.uBuffer.pucBuffer);
			if(image_identifier != GDI_IMAGE_TYPE_INVALID)
			{
				mmi_trace(1,(PCSTR)"widget_log: decoding API invoked");

				if(image_identifier == GDI_IMAGE_TYPE_GIF)
				{
					gdi_handle hANimHandle ;
					if(*puiAnimHnd)
					{
						gdi_image_stop_animation((gdi_handle)*puiAnimHnd);
						*puiAnimHnd =  JC_NULL;					
					}
					gdiResult = gdi_anim_draw_mem_resized (
								  	pstDispRect->iLeft,pstDispRect->iTop, \
									pstDispRect->uiWidth, pstDispRect->uiHeight, m_stObjectInfo.uBuffer.pucBuffer,
									image_identifier, m_stObjectInfo.uiObjectLen,(gdi_handle*)&hANimHandle);
					*puiAnimHnd = (JC_UINT32)hANimHandle ;			
				}
				else
				{
					gdiResult = gdi_image_draw_resized_ext(pstDispRect->iLeft,pstDispRect->iTop, \
											pstDispRect->uiWidth,pstDispRect->uiHeight,
											m_stObjectInfo.uBuffer.pucBuffer,image_identifier,
											m_stObjectInfo.uiObjectLen
											);
				}
				
				mmi_trace(1,(PCSTR)"widget_log: gdi_image_draw_resized_ext obj len [%d],totlen [%d] ", \
									m_stObjectInfo.uiObjectLen, m_stObjectInfo.uiTotalLen);
				mmi_trace(1,(PCSTR)"widget_log: gdi_image_draw_resized_ext x[%d]y[%d]w[%d]h[%d] ", \
									pstDispRect->iLeft,pstDispRect->iTop,
									pstDispRect->uiWidth,pstDispRect->uiHeight);
			}
			else
			{
			    mmi_trace(1,(PCSTR)"GDI_IMAGE_TYPE_INVALID");
				gdiResult = GDI_IMAGE_TYPE_INVALID;
				
			}
#endif		
			mmi_trace(1, (PCSTR)"widget_log: DrawImage noFileContent rCode is [%d], Ht [%d], Wt [%d]", \
						gdiResult, pstDispRect->uiHeight, pstDispRect->uiWidth);
			
	   	}
	}
	if(gdiResult != 0 )
	{
		U8 * imgptr = JC_NULL;		
		imgptr = (U8*)GetImage(IMG_MMS_ALT_ICON);		
		gdiResult = gdi_image_draw_resized(pstDispRect->iLeft,pstDispRect->iTop, \
						pstDispRect->uiWidth, pstDispRect->uiHeight,imgptr);
		mmi_trace(1, (PCSTR)"widget_log: display altimage imgptr[%d], gdiResult[%d]", imgptr,gdiResult);		

	}
	rCode = gdiResult;	
	return rCode;
}

/**
 * @brief		decodes the the image to native bitmap
 * @param[in]   stObjectInfo	Object Info.
 * @param[in]   pstRect			co-ordiantes of the rectangle in which 
 *									image has to be alligned
 * @param[in]   ppvDecodedBuff  decode buffer
 * @return		Appropriate error code.
 *
 * 
 */

JC_RETCODE jddex_PlatformDecodeImage (JC_OBJECT_INFO	stObjectInfo, JC_RECT * pstRect,
						void ** ppvDecodedBuff, JC_UINT32 *puiDecodedWidth , 
						JC_UINT32 *puiDecodedHeight)
{

	return JC_OK ;
}

JC_UINT32 jddex_PlatformGetRGBPalette (JC_UINT8 uiRedColor, JC_UINT8 uiGreenColor, 
								JC_UINT8 uiBlueColor) 
{
	JC_UINT32 uiRGB;
	if( (uiBlueColor >= 247) /*&& (uiBlueColor <= 255)*/)
		uiBlueColor = 246;

	uiRGB = convert2rgb(uiRedColor,uiGreenColor,uiBlueColor);
	return uiRGB;
}
/**
 * @brief		draws a rectangle
 * @param[in]   pstRect			rectangle co-ordiantes.
 * @param[in]   iBorderColor	border color
 * @param[in]   iFillColor		fill color
 * @return		JC_OK
 *
 */
JC_RETCODE jddex_PlatformDrawRect(const JC_RECT * pstRect, 
			 JC_INT32 iBorderColor, JC_INT32 iFillColor)
{
	color_t stColor;

	converttoplatformrgb(iBorderColor, &stColor);
	if (JC_NULL != (void*)pstRect && 0 < pstRect->uiWidth && 0 < pstRect->uiHeight)
	{	
		jddex_PlatformFillRect(pstRect, iFillColor);
		gui_draw_rectangle (pstRect->iLeft, pstRect->iTop, 
			pstRect->uiWidth+pstRect->iLeft-1, pstRect->uiHeight+pstRect->iTop-1, 
			stColor) ;
	}
	return JC_OK;
}

/**
 * @brief		upadtes the LCD
 * @return		None
 *
 */
void jddex_PlatformUpdateScreen()
{
	JDDEX_DEVICE_CHAR stDeviceChar;
			
	jddex_GetDeviceProp((E_APP_TYPE) 0, &stDeviceChar);

	gui_lock_double_buffer();
	
	gui_reset_clip();
	gui_push_clip();	
	gui_set_clip(stDeviceChar.uiLeft, stDeviceChar.uiTop, 
		stDeviceChar.uiLeft+stDeviceChar.uiRenderingWidth , 
		stDeviceChar.uiTop+stDeviceChar.uiRenderingHeight);
	
	gui_pop_clip(); 	
	gui_unlock_double_buffer();
					
	gui_BLT_double_buffer(stDeviceChar.uiLeft, stDeviceChar.uiTop, 
		stDeviceChar.uiLeft+stDeviceChar.uiRenderingWidth , 
		stDeviceChar.uiTop+stDeviceChar.uiRenderingHeight);

}

/**
 * @brief		fills a rectangle with given color
 * @param[in]   pstRect			rectangle co-ordiantes.
 * @param[in]   iFillColor		fill color
 * @return		JC_OK
 *
 */
JC_RETCODE jddex_PlatformFillRect(const JC_RECT * pstRect, JC_INT32 iFillColor)
{
	color_t stColor;

	converttoplatformrgb(iFillColor, &stColor);
	gui_fill_rectangle(pstRect->iLeft, pstRect->iTop, 
		pstRect->uiWidth+pstRect->iLeft-1, pstRect->uiHeight+pstRect->iTop-1 , stColor) ;

	return JC_OK;
}

JC_RETCODE jddex_GetDeviceProp(E_APP_TYPE eApp, JDDEX_DEVICE_CHAR *pstDeviceChar)
{
	pstDeviceChar->uiActualHeight = CST_LCD_HEIGHT;
	pstDeviceChar->uiActualWidth = CST_LCD_WIDTH;
	pstDeviceChar->uiLeft = 0;
	pstDeviceChar->uiTop = CST_TITLE_HEIGHT ;
	pstDeviceChar->uiRenderingWidth = CST_LCD_WIDTH ;
	pstDeviceChar->uiRenderingHeight = CST_LCD_HEIGHT - CST_TITLE_HEIGHT - CST_SOFTKEY_HEIGHT ;

	#if 0
	mmi_trace (1, (PCSTR)"$$$jddex_GetDeviceProp CST_LCD_WIDTH[%d], CST_LCD_HEIGHT[%d],\
				CST_TITLE_HEIGHT[%d], CST_render_HEIGHT[%d] @@@",\
				pstDeviceChar->uiActualHeight, pstDeviceChar->uiActualWidth,\
				pstDeviceChar->uiTop, pstDeviceChar->uiRenderingHeight) ;
	#endif
	return 0;
}

JC_INT32 jddex_GetDevicePropTop()
{
    return CST_TITLE_HEIGHT ;
}

JC_INT32 jddex_GetDevicePropLeft()
{
	return (0);
}

JC_INT32 jddex_GetDeviceRenderWidth()
{
	DEVICE_CHARACTERISTICS 	stDeviceChar = {0, } ;
	jdd_MMIGetDeviceCharacteristics (&stDeviceChar) ;
	return stDeviceChar.uiRenderingWidth ;
}

JC_INT32 jddex_GetDeviceRenderHeight()
{
	return CST_LCD_HEIGHT - CST_TITLE_HEIGHT - CST_SOFTKEY_HEIGHT ;
}



void mmi_multiple_selection_listbox(JDDEX_LIST_ITEM*  pstListItems)
{	
	U8**  pListItems = JC_NULL;
	U8 *fieldStates = JC_NULL;
	JC_UINT32 index_item;
	U8 *guiBuffer = JC_NULL;
	
	pListItems	 = (U8**)jdd_MemAlloc (g_pstListBox.uiItemCnt , sizeof (U8*)) ;
	fieldStates = (U8 *)jdd_MemAlloc(g_pstListBox.uiItemCnt,sizeof (U8));
	
	for (index_item = 0; index_item< g_pstListBox.uiItemCnt; index_item++)
	{
		pListItems[index_item] = (U8*)jdd_MemAlloc ((jc_wstrlen(pstListItems[index_item].psText)+1), 
											sizeof (JC_CHAR)) ;
		jc_wstrcpy((UINT16*)pListItems[index_item],(UINT16*)(pstListItems[index_item].psText));
		if(	(pstListItems[index_item].IsSelected) == E_TRUE)
		{								
			fieldStates[index_item] = pstListItems[index_item].IsSelected;					
		}
	}					
					
	g_field_state = fieldStates;
	g_TempListItems = pListItems;	
					
	SetCheckboxToggleRightSoftkeyFunctions(settoggle,GoBackHistory);
	ShowCategory140Screen(	STR_ID_WAP_HOMEPAGE,0,
							g_pstListBox.uiItemCnt, (U8**)pListItems,
							fieldStates, 0, guiBuffer);

}


JC_RETCODE jddex_PlatformCreateListBox(JC_UINT32 uiControlId,JDDEX_LIST_BOX*  pstListBox)
{
	U8 *guiBuffer = JC_NULL;
	JC_RETCODE rCode = JC_OK;
	int hlted_item = 0;
	JC_UINT32 temp_index;
	JDDEX_LIST_ITEM * pstListItems;
	   
	g_pstListBox = *pstListBox; 
	g_uiControlId = uiControlId;
	pstListItems = *g_pstListBox.pstListItems;	
	EntryNewScreen (SCR_ID_WIDGET_LISTBOX, JC_NULL, JC_NULL, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_ID_WIDGET_LISTBOX);
	SetParentHandler(0);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	if (g_pstListBox.listStyle == E_SINGLE_SELECTION)
	{
		for ( temp_index = 0; temp_index < g_pstListBox.uiItemCnt ; temp_index++)
		{
			if( (pstListItems[temp_index].IsSelected) == E_TRUE)
			{
				hlted_item = temp_index;
			}
		}	
		ShowCategory184Screen(	STR_ID_WAP_HOMEPAGE,0,
								STR_GLOBAL_OK,IMG_GLOBAL_OK,
								STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
								pstListBox->uiItemCnt, get_list_name,
								0, hlted_item,
								guiBuffer);  
		
		SetLeftSoftkeyFunction(jddex_ListBoxLSKHandler, KEY_EVENT_UP);
		SetRightSoftkeyFunction(jddex_RSKBackHandler, KEY_EVENT_UP);
	}
	else
	{
		mmi_multiple_selection_listbox(pstListItems);
	}
	SetKeyHandler(GoBackHistory, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);	
	return rCode;
}


void jmms_goto_primaryMMI()
{
	GoBackToHistory (SCR_ID_WAP_HOMEPAGE_SCREEN);
}

void settoggle(void)
{
	DisplayConfirm(STR_GLOBAL_YES, IMG_GLOBAL_YES,
					STR_GLOBAL_NO, IMG_GLOBAL_NO,
					get_string(STR_GLOBAL_SAVE), IMG_GLOBAL_QUESTION, JC_NULL);

	SetLeftSoftkeyFunction(mmi_field_save,KEY_EVENT_UP);
	SetRightSoftkeyFunction(jmms_goto_primaryMMI,KEY_EVENT_UP);
}

JC_RETCODE jddex_PlatformCreateInputBox(JC_UINT32 uiControlId, JC_INPUT_BOX  stInputBox, 
								   ERenderingStyle eRenderingStyle,JC_CHAR* pszInputText) 
{	
	JC_RETCODE rCode = JC_OK ;
	g_uiControlId = uiControlId;

	if((stInputBox.iMaxLength > JDDEX_MAX_INPUT_CHAR) || \
				(stInputBox.iMaxLength == 0))
	{
		g_uiMaxlen = JDDEX_MAX_INPUT_CHAR + 1 ;
	}
	else
	{
		g_uiMaxlen = stInputBox.iMaxLength + 1 ; 
	}

	g_pszInputText = (JC_CHAR*)jdd_MemAlloc (JDDEX_MAX_INPUT_CHAR+1,2) ;
	jc_wstrcpy(g_pszInputText,pszInputText );
	g_pszInputOrigText = pszInputText ;
	
	//mmi_trace(1,(PCSTR)"$$$PwdInputBox style %d",stInputBox.einputBoxStyle );
	if (stInputBox.einputBoxStyle == E_INPUT_MASK)
	{		
		jddex_platformPwdInputBoxEntryHandler () ;
	}
	else
	{		
		jddex_platformInputBoxEntryHandler () ;
	}
	//mmi_trace(1, (PCSTR)" inputbox controlID %d", uiControlId);
	return rCode ;
}




void Cancelgotoprimarymmi()
{
	g_uiMaxlen = 0 ;
	
	if(g_pszInputText)
	{
		jdd_MemFree (g_pszInputText) ;
		g_pszInputText = JC_NULL;
	}
	jmms_goto_primaryMMI();
}

pBOOL get_list_name( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask )
{
	JDDEX_LIST_ITEM * pstListItems;
	
	pstListItems = *g_pstListBox.pstListItems;
	if((item_index < 0) || ((JC_UINT32)item_index >= g_pstListBox.uiItemCnt))
		return FALSE;
	
	jc_wstrcpy((UINT16*)str_buff,(UINT16*)(pstListItems[item_index].psText));	 
	*img_buff_p = (UI_image_type) GetImage( (U16)(IMG_GLOBAL_L1 + item_index) );	

	return TRUE;
}

void jddex_inputBox_highlightHanlder (S32 index)
{
	switch(index)
	{
		case 0:
			SetLeftSoftkeyFunction (Donegotoprimarymmi, KEY_EVENT_UP ) ;
			break;
			
		case 1:
			SetLeftSoftkeyFunction (Cancelgotoprimarymmi, KEY_EVENT_UP ) ;
			break;

		case 2:
			SetLeftSoftkeyFunction (EntryInputMethodScreen, KEY_EVENT_UP ) ;
			break ;
	}
	
}

void jddex_inputBoxLskHandler ()
{
	U16 item_texts[] = {STR_GLOBAL_DONE, \
						STR_GLOBAL_CANCEL, \
						STR_GLOBAL_INPUT_METHOD} ;
	
	U16 item_icons[] = {IMG_GLOBAL_L1, \
						IMG_GLOBAL_L2, \
						IMG_GLOBAL_L3} ;
	
	EntryNewScreen (SCR_ID_WIDGET_INPUTBOX_OPTION, 0, 0, 0) ;
	SetParentHandler (SCR_ID_WIDGET_INPUTBOX);
	
	RegisterHighlightHandler (jddex_inputBox_highlightHanlder);

	ShowCategory52Screen (STR_GLOBAL_OPTIONS, 0, 
							STR_GLOBAL_OK, 0,
							STR_GLOBAL_BACK, 0, 
							3, item_texts, item_icons,
							0, 0, 0, 0);
	
	// register LSK, RSK handlers
	//SetLeftSoftkeyFunction (jddex_inputBoxOption_lskHandler, KEY_EVENT_UP ) ;
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP ) ;
}


void  jddex_platformInputBoxEntryHandler () 
{
	U8* guiBuffer = JC_NULL;	
	
	EntryNewScreen (SCR_ID_WIDGET_INPUTBOX, JC_NULL, \
		jddex_platformInputBoxEntryHandler, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WIDGET_INPUTBOX);		
	SetParentHandler (SCR_ID_WAP_HOMEPAGE_SCREEN);
	ShowCategory5Screen (0, 0,
		STR_GLOBAL_OPTIONS, 0,
		STR_GLOBAL_BACK, 0,
		INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
		(U8*)g_pszInputText, g_uiMaxlen, 
		guiBuffer);
	
#ifdef __MMI_GB__
		GBSetInputboxLSKFunction (jddex_inputBoxLskHandler);
#endif

	SetLeftSoftkeyFunction (jddex_inputBoxLskHandler, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction (Cancelgotoprimarymmi, KEY_EVENT_UP);
	RegisterInputMethodScreenCloseFunction (GoBackHistory );
}


void  jddex_platformPwdInputBoxEntryHandler () 
{
	U8* guiBuffer = JC_NULL;
	S16 required_input_modes[] = {
					INPUT_MODE_MULTITAP_UPPERCASE_ABC ,
					INPUT_MODE_MULTITAP_LOWERCASE_ABC,
					INPUT_MODE_123,
					-1
					} ;	
	
	EntryNewScreen (SCR_ID_WIDGET_INPUTBOX, JC_NULL, \
		jddex_platformPwdInputBoxEntryHandler, NULL);
	guiBuffer = GetCurrGuiBuffer (SCR_ID_WIDGET_INPUTBOX);		
	SetParentHandler (SCR_ID_WAP_HOMEPAGE_SCREEN);		

	ShowCategory5Screen_ext (0, 0,
		STR_GLOBAL_OPTIONS, 0,
		STR_GLOBAL_BACK, 0,
		INPUT_TYPE_ALPHANUMERIC_PASSWORD,
		(U8*)g_pszInputText, g_uiMaxlen, 
		guiBuffer,0, required_input_modes, 0);

#ifdef __MMI_GB__
	GBSetInputboxLSKFunction (jddex_inputBoxLskHandler);
#endif

	SetLeftSoftkeyFunction (jddex_inputBoxLskHandler, KEY_EVENT_UP);
	SetCategory5RightSoftkeyFunction (Cancelgotoprimarymmi, KEY_EVENT_UP);
	RegisterInputMethodScreenCloseFunction (GoBackHistory );
}



void jddex_RSKBackHandler()
{
	jmms_goto_primaryMMI();
}

JC_RETCODE jddex_PlatformCreateComboBox(JC_UINT32 uiControlId,
					JDDEX_LIST_BOX*  pstListBox)
{
	U8 *guiBuffer = JC_NULL;
	JC_RETCODE rCode = JC_OK;
	int hlted_item = 0;
	JC_UINT32 temp_index;
	JDDEX_LIST_ITEM * pstListItems;
   
	g_pstListBox = *pstListBox;	
	pstListItems = *g_pstListBox.pstListItems;
	g_uiControlId = uiControlId;
	
    EntryNewScreen (SCR_ID_WIDGET_LISTBOX, JC_NULL, JC_NULL, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_ID_WIDGET_LISTBOX);
    SetParentHandler(0);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	
	for ( temp_index = 0; temp_index < g_pstListBox.uiItemCnt ; temp_index++)
	{
		if(	(pstListItems[temp_index].IsSelected) == E_TRUE)
		{
			hlted_item = temp_index;
		}
	}
    ShowCategory184Screen( 	STR_ID_WAP_HOMEPAGE,0,
            				STR_GLOBAL_OK,IMG_GLOBAL_OK,
            				STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
            				pstListBox->uiItemCnt, get_list_name,
            				0, hlted_item,
            				guiBuffer);  
	
	ChangeLeftSoftkey(STR_GLOBAL_OK, 0);
	ChangeRightSoftkey(STR_GLOBAL_BACK, 0);
	SetLeftSoftkeyFunction(jddex_ComboBoxLSKHandler, KEY_EVENT_UP);
	SetRightSoftkeyFunction(jddex_RSKBackHandler, KEY_EVENT_UP);

	return rCode;
}

void jddex_FreePlatformWidgets()
{
	JC_UINT32 temp_index;

	if(g_pszInputText)
	{
		jdd_MemFree (g_pszInputText) ;
		g_pszInputText = JC_NULL;
	}
	if(g_TempListItems)
	{
		for (temp_index = 0; temp_index< g_pstListBox.uiItemCnt; temp_index++)
		{
			jdd_MemFree(g_TempListItems[temp_index]) ;
		
		}	
		jdd_MemFree(g_TempListItems);
		g_TempListItems = JC_NULL;
	}
}

JC_UINT32 jddex_PlatformGetTextHeight(const JC_STYLE *pStyle,
								  const JC_CHAR *psTextValue)
{
	JC_UINT32 uiHeight=0, uiWidth = 0;

	//WidgetsFontType eFontType = GetMappedFont(pStyle->uiFontSize);

	gui_measure_string((JC_CHAR *)psTextValue, (S32*)&uiWidth, \
						(S32*)&uiHeight);

	//uiHeight = widgetsFtGetFontHeight(eFontType, E_TRUE);

	if ( pStyle->eTextDecoration == E_DECN_UNDERLINE )
	{
		uiHeight +=1 ;
	}
  return uiHeight;
}

extern "C" void TurnOnBacklight(U8 TimerFlag);
void jddex_DrawScrollBarControl(JC_RECT stScrollBarRect, JC_RECT stScrollCarRect )
{
	//color_t scrollcarbordercolor ={255,255,255,0};// white color we have to add
	//color_t scrollcarfillcolor ={0,0,0,0};// black color we have to add

	s32 x1,x2,y1,y2;
	JC_UINT8 * uiScrollBarUpImgptr = JC_NULL;
	JC_UINT8 * uiScrollBarDownImgptr = JC_NULL;	

	TurnOnBacklight(1);//turn on the screen
	
	stScrollCarRect.iLeft		= stScrollBarRect.iLeft + 2;  //+1
	stScrollCarRect.uiWidth		= stScrollBarRect.uiWidth -4;    

	jddex_DrawFormattedRect(&stScrollBarRect,JDDEX_DEFAULT_BLACK, \
	 			JDDEX_DEFAULT_BLACK, 0);

	// currently using some exiting icons . replace with new icon
	uiScrollBarUpImgptr = (U8*)GetImage(IMG_MMS_UP_ARROW);	
	gdi_image_draw_resized(stScrollBarRect.iLeft, stScrollBarRect.iTop, \
			jwap_GetScrollbarbarArrowWidth(), jwap_GetScrollbarbarArrowHight(), uiScrollBarUpImgptr);
	
	uiScrollBarDownImgptr = (U8*)GetImage(IMG_MMS_DOWN_ARROW);	
	gdi_image_draw_resized(stScrollBarRect.iLeft, 
				stScrollBarRect.uiHeight+stScrollBarRect.iTop - jwap_GetScrollbarbarArrowHight(), 
				jwap_GetScrollbarbarArrowWidth(), jwap_GetScrollbarbarArrowHight(), uiScrollBarDownImgptr);
   

	x1 = stScrollCarRect.iLeft;
	y1 = stScrollCarRect.iTop ;
	x2 = stScrollCarRect.uiWidth+stScrollCarRect.iLeft-1;
	y2 = stScrollCarRect.uiHeight+stScrollCarRect.iTop-1;

	//gui_fill_rectangle(x1 , y1 , x2 , y2 , scrollcarfillcolor);
	//gui_draw_rectangle(x1 , y1 , x2 , y2 , scrollcarbordercolor);
	mmi_trace(1,(PCSTR)"widget_log: drawing rounded rectangle");

	gdi_draw_round_rect(x1 , y1 , x2 , y2, GDI_COLOR_BLACK, GDI_COLOR_WHITE,1);

	
}




