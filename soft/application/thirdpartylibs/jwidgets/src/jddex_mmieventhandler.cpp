/***************************************************************************
*
* File Name : jdd_mmieventhandler.cpp
*
*
***************************************************************************
*
*
* File Description
* ----------------
*
* Purpose			:	Implimentation to handle specfic events for wingtech
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


#include "eventsgprot.h"
#include <jddex_mmieventhandler.h>
#include <jddex_platformdefines.h>
#include "jdi_browserengine.h"
#include <jddex_int.h>


/**
 * @brief 		Event handler to be called for a key events on a browser primary MMI. 
 * @retval		E_TRUE
 * Handles the key evnts on controls
 */
//#ifdef JATAAYU_SUPPORT_WAP
JC_BOOLEAN jddex_widgetHandleKeyPress (JC_HANDLE	vHandle,
		   JC_UINT32 uiEventType, void	*pvEventInfo, JC_RECT *pStylusPosition)

{
	JC_BOOLEAN bRetCode ;
	JC_UINT32 uiMappedEventType;

	bRetCode = jddex_MapPlatformEvents(uiEventType, &uiMappedEventType);

	if (bRetCode == E_TRUE)
	{
		bRetCode = jddex_WidgetHandleEvent(vHandle,
				 uiMappedEventType, pvEventInfo, pStylusPosition);
	}
	return bRetCode ;
}

//#endif

/**
 * @brief 		gives the status of secondary MMI. 
 * @param[out]	TRUE/FALSE
 * @retval		JC_OK
 * checks if the secondar is in focus or out of focus
 */
JC_RETCODE jddex_IsSecondaryMMIUp(JC_BOOLEAN *isSecMMIUp)
{
	*isSecMMIUp = jApp_isSecMmiActive () ;
   	return JC_OK;
}


/**
 * @brief 		maps wingtech evnts to widget events
 * @param[in]	uiInputEventType    wingtech event
 * @param[out]	puiMappedEventType  mapped widget evnt
 * @retval		E_TRUE -if mapped, E_FALSE- on failure
 *
 */

JC_BOOLEAN jddex_MapPlatformEvents(JC_UINT32 uiInputEventType,JC_UINT32* puiMappedEventType)
{
	JC_BOOLEAN retCode = E_TRUE;

	switch(uiInputEventType)
	{

	case E_BE_SCROLL_UP	:
		*puiMappedEventType = E_JDDEX_KEY_UP;
		break;
	case E_BE_SCROLL_DOWN:
		*puiMappedEventType = E_JDDEX_KEY_DOWN;
		break;
	case E_BE_SCROLL_RIGHT:
		*puiMappedEventType = E_JDDEX_KEY_RIGHT;
		break;
	case E_BE_SCROLL_LEFT:
		*puiMappedEventType = E_JDDEX_KEY_LEFT;
		break;
	case E_BE_ACTION:
		*puiMappedEventType = E_JDDEX_KEY_SELECT;
		break;
/*
	case MSG_APP_CANCEL:
		*puiMappedEventType = E_JDDEX_KEY_CANCEL;
		break;
*/

    case E_WIDGET_INPUT_DONE:
		*puiMappedEventType = E_WIDGET_INPUT_DONE;
		/* jataayu to handle here*/
		break;
    case E_WIDGET_INPUT_CANCEL:
		*puiMappedEventType = E_WIDGET_INPUT_DONE;
		/* jataayu to handle here*/
		break;
	default:
		retCode = E_FALSE;
		break;
	}

	return retCode;

}



