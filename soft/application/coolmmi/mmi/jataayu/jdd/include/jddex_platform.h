/***************************************************************************
*
* File Name : jddex_platform.h
*
*
***************************************************************************
*
*
* File Description
* ----------------
*
* Purpose			:	declaration for platform adapatation
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
#ifndef _JDDEX_PLATFORM_
#define _JDDEX_PLATFORM_

#ifdef __cplusplus
    extern   "C"
    {
#endif
typedef enum e_line_style
{
    E_SOLID_LINE, /**< Specifies the solid line style where the line
						needs to be continuously drawn. */
    E_DOTTED_LINE, /**< Specifies the dotted line style where the line
						needs to be drawn continuously. */
    E_DASHED_LINE, /**< Specifies dashed line. Can be Rendered as solid. */
    E_DOUBLE_LINE, /**< Specifies two line. The width of the two lines are the same as the border-width value. */
    E_GROOVE_LINE, /**< Specifies a grooved line. The effect depends on the line-color value. */
    E_RIDGE_LINE, /**< Specifies a ridge line. The effect depends on the line-color value. */
    E_INSET_LINE, /**< Specifies a recessed line. The effect depends on the line-color value. */
    E_OUTSET_LINE /**< Specifies a raised line. The effect depends on the line-color value. */
} E_LINE_STYLE ;


typedef enum e_image_property
{
	E_IMAGE_NORMAL = -1,
	E_IMAGE_CENTER,
	E_IMAGE_STRETCH, /**< Specifies the image property for the image, which needs to be rendered for the full screen of the window, the image need to be stretched/resized if the actual width and height of the image is not equal to the window's width and height. */
	E_IMAGE_TILE, /**< Specifies the image property for the image which needs to be tiled across vertical and horizontal for the full width and height of the window. */
	E_IMAGE_VERTICAL_TILE, /**< Specifies the image property for the image which needs to be tiled across vertical for the full height of the window. */
	E_IMAGE_HORIZONTAL_TILE /**< Specifies the image property for the image which needs to be tiled across horizontal for the full width of the window. */
} EImageProperty, E_IMAGE_PROPERTY ;



typedef struct tag_device_char
{
	JC_UINT32 uiActualWidth ;
	JC_UINT32 uiActualHeight ;
	JC_UINT32 uiLeft ;
	JC_UINT32 uiTop ;
	JC_UINT32 uiRenderingWidth ;
	JC_UINT32 uiRenderingHeight ;
}JDDEX_DEVICE_CHAR;

typedef enum e_app_type
{

	JDDEX_BROWSER_APP,
    JDDEX_MMS_APP,
}E_APP_TYPE;

typedef enum e_border_name
{
	E_JDDEX_LEFT_BORDER,
	E_JDDEX_TOP_BORDER,
	E_JDDEX_RIGHT_BORDER,
	E_JDDEX_BOTTOM_BORDER
}E_BORDER_NAME;

JC_RETCODE jddex_MMIInitialize (void * pvReference);

// font

JC_RETCODE jddex_PlatformDrawText(const JC_CHAR* pszText ,JC_RECT stRect, 
					JC_STYLE stTextStyle, JC_BOOLEAN IgnBackColor);

JC_RETCODE jddex_PlatformGetTextMetrics ( const JC_STYLE *pStyle,
								  const JC_CHAR *psTextValue,
								  JC_UINT32 uiWidth,
								  JC_TEXTMETRICS *pTextMetrics);

//basic  shapes

JC_RETCODE jddex_PlatformDrawLine(JC_POINT* pstStart, JC_POINT* pstEnd,
							E_LINE_STYLE eStyle, JC_INT32 iColor, 
							JC_INT32 uiBorderWidth, E_BORDER_NAME eBorderName);

JC_RETCODE jddex_PlatformEraseRect(const JC_RECT * pstRect);


JC_RETCODE jddex_PlatformDrawCircle(JC_POINT* pstCentre, JC_INT32 iRadius,
									JC_INT32 iBorderColor, JC_INT32 thickness );

JC_RETCODE jddex_PlatformDrawPixel(JC_POINT* pstPoint, JC_INT32 iColor );

JC_RETCODE jddex_PlatformDrawRect(const JC_RECT * pstRect,
			 JC_INT32 iBorderColor, JC_INT32 iFillColor);
JC_RETCODE jddex_PlatformGetImageSize (JC_IMAGE_INFO		*pstImageInfo,
							JC_UINT32 *puiWidth, JC_UINT32 *puiHeight);

JC_RETCODE jddex_PlatformGetObjectSize (JC_OBJECT_INFO		*pstObjectInfo,
							JC_UINT32 *puiWidth, JC_UINT32 *puiHeight) ;
JC_RETCODE jddex_PlatformGetImageType(JC_INT8 *pcImageMimeType, JC_UINT8 *uiImageFormat);

JC_RETCODE jddex_PlatformDecodeImage (JC_OBJECT_INFO	stObjectInfo, JC_RECT * pstRect,
						void ** ppvDecodedBuff, JC_UINT32 *puiDecodedWidth , 
						JC_UINT32 *puiDecodedHeight);

JC_RETCODE jddex_PlatformDrawImage (JC_OBJECT_INFO		m_stObjectInfo, 
							JC_RECT * pstImageRect, JC_RECT * pstDispRect,
							EImageProperty eImageProperty,JC_CHAR* m_pmTempImageFile,
							JC_UINT32* puiAnimHnd);
JC_UINT32 jddex_PlatformGetRGBPalette (JC_UINT8 uiRedColor, JC_UINT8 uiGreenColor,
								JC_UINT8 uiBlueColor) ;

JC_RETCODE jddex_PlatformFillRect(const JC_RECT * pstRect, JC_INT32 iFillColor);

JC_RETCODE jddex_PlatformDrawLineEx(JC_POINT* pstStart, JC_POINT* pstEnd,
							  JC_INT32 iColor,JC_INT32 iWidth );

JC_RETCODE jddex_Platform3DFrame(const JC_RECT * pstRect, JC_INT32 iFillColor, JC_BOOLEAN bIsup);
void jddex_PlatformUpdateScreen();

JC_INT32 jddex_GetDevicePropTop() ;
JC_INT32 jddex_GetDevicePropLeft();
JC_RETCODE jddex_GetDeviceProp(E_APP_TYPE eApp, JDDEX_DEVICE_CHAR *pstDeviceChar) ;
JC_INT32 jddex_GetDeviceRenderWidth();	
pBOOL get_list_name( S32 item_index, UI_string_type str_buff, UI_image_type* img_buff_p, U8 str_img_mask );

JC_RETCODE jddex_PlatformCreateInputBox(JC_UINT32 uiControlId, JC_INPUT_BOX  stInputBox, 
								   ERenderingStyle eRenderingStyle,JC_CHAR* pszInputText);
JC_RETCODE jddex_MMICreateScrollBar(void * poWindow, EControlType eControlType,
								JC_CONTROL_PROPERTIES *pstControlProperty,
								JC_RECT *pstRect, JC_UINT32 *puiControlID);
JC_RETCODE jddex_PlatformDrawLabel(const JC_CHAR* pszText ,JC_RECT stRect,JC_RECT stParentRect,
					JC_STYLE stTextStyle, JC_BOOLEAN IgnBackColor);
JC_UINT32 jddex_PlatformGetTextHeight(const JC_STYLE *pStyle,
								  const JC_CHAR *psTextValue);

JC_INT32 jddex_GetDeviceRenderWidth();
JC_INT32 jddex_GetDeviceRenderHeight();
void jddex_platformPwdInputBoxEntryHandler ();
void jddex_platformInputBoxEntryHandler ();
void Cancelgotoprimarymmi();
void settoggle(void);
void jmms_goto_primaryMMI(void);
void jddex_RSKBackHandler();
void jddex_FreePlatformWidgets();

void jddex_ListBoxLSKHandler();
void jddex_ComboBoxLSKHandler();
void Donegotoprimarymmi();
void jddex_MMIBrwSetInfo(JC_UINT32  uiWindowID, SCROLL_INFO* pstVScrollInfo);
void mmi_field_save(void);
void jddex_LSKDoneHandler();
void jddex_DrawScrollBarControl(JC_RECT stScrollBarRect, JC_RECT stScrollCarRect );
void jddex_DrawFormattedRect(JC_RECT * pstChildRect,JC_INT32 iBorderColor, JC_INT32 iFillColor,
							 JC_INT32 iWidth);

void jddex_GetWidgetRect(JC_UINT32 uicontrolId, JC_RECT* pstRect );

#ifdef __cplusplus
    }
#endif

#endif //_JDDEX_PLATFORM_

