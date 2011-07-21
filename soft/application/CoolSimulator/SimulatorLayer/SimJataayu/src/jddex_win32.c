/* zhuoxz,2009-4-29 */

#include "cswtype.h"
#include "ddlbase.h"
#include "jcutils.h"
#include "jcerror.h"
#include "jcevent.h"
#include "jdd_mmidatatype.h"
typedef enum e_app_type
{

	JDDEX_BROWSER_APP,
	JDDEX_MMS_APP,
}E_APP_TYPE;

typedef struct tag_device_char
{
	JC_UINT32 uiActualWidth ;
	JC_UINT32 uiActualHeight ;
	JC_UINT32 uiLeft ;
	JC_UINT32 uiTop ;
	JC_UINT32 uiRenderingWidth ;
	JC_UINT32 uiRenderingHeight ;
}JDDEX_DEVICE_CHAR;

JC_BOOLEAN jddex_widgetHandleKeyPress(JC_HANDLE	vHandle, JC_UINT32 uiEventType, void	*pvEventInfo)
{
  return E_FALSE;
}

void jddex_MMIBrwSetInfo(JC_UINT32  uiWindowID, SCROLL_INFO* pstVScrollInfo)
{
  return;
}

JC_INT32 jddex_GetDeviceRenderHeight()
{
  return 0;
}

void jddex_FreePlatformWidgets()
{
}

void jddex_GetWidgetRect(JC_UINT32 uicontrolId, JC_RECT* pstRect )
{

}
JC_INT32 jddex_GetDeviceProp(E_APP_TYPE eApp, JDDEX_DEVICE_CHAR *pstDeviceChar)
{

}