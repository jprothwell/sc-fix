/* zhuoxz,2009-4-29 */

#include "cswtype.h"
#include "ddlbase.h"
#include "jcutils.h"
#include "jcerror.h"
#include "jcevent.h"
#ifdef JATAAYU_SUPPORT
#include "jwap_str_id.h"
#endif
#include "jdd_mmidatatype.h"
#include "messagingdatatypes.h"
#include "jdi_displayinfo.h"
#include "jdi_ppp.h"
#include "jdi_pushmsg.h"


void jdd_MMIEndDisplay (JC_UINT32 uiWindowID, JC_BOOLEAN bRedraw)
{
  return;
}

JC_RETCODE jdd_MMIShowWindow (JC_UINT32 uiWindowID, JC_BOOLEAN bIsVisible)
{
  return 0;
}

JC_RETCODE jdd_MMICreateWindow (JC_WINDOW_PROPERTIES	*pstWinProp,
								const JC_RECT			*pstRect,
								JC_UINT32				*puiWindowID)
{
  return 0;
}

JC_RETCODE jdd_MMIInitialize (void *pvMMIReference)
{
  return 0;
}

JC_RETCODE jdd_MMICreateControl (JC_UINT32 uiWindowID, EControlType eControlType, 
								 JC_CONTROL_PROPERTIES *pstControlProperty,
								 JC_RECT *pstRect, JC_UINT32 *puiControlID)
{
  return 0;
}

JC_RETCODE jdd_MMIDestroyWindow (JC_UINT32 uiWindowID)
{
  return 0;
}

JC_RETCODE jdi_MsgGetTotalPages (MSG_HANDLE hMsg, JC_UINT8 *pPageCount)
{
  return 0;
}

JC_RETCODE jdi_MsgGetCreationMode (MSG_HANDLE hMsg, ECreationMode *pCreationMode)
{
  return 0;
}

JC_RETCODE jdi_MsgGetContentClass (MSG_HANDLE hMsg, EContentClass *pContentClass)
{
  return 0;
}

JC_RETCODE jdi_MsgGetCurrentPageIndex (MSG_HANDLE hMsg, JC_UINT8 *pPageIndex)
{
  return 0;
}

JC_BOOLEAN jdi_CUtilsStrStartsWith (const JC_INT8	*pcSrc, 
									const JC_INT8	*pcSubStr)
{
  return E_FALSE;
}
JC_RETCODE jdd_MMIGetScrollCarInfo (JC_UINT32 uiWindowID,
		JC_UINT32 uiControlID, JC_RECT *pScrollInfo)
{
	  return 0;
}
JC_RETCODE jdd_MMIGetScrollInfo (JC_UINT32		uiWindowID,
								 JC_UINT32		uiControlID,
								 SCROLL_INFO	*pstScrollInfo) 
{
		  return 0;
}
								 	
JC_INT32 jdi_CUtilsStrLastIndexOf (const JC_INT8	*pcSrc,
								   JC_INT8			cChar,
								   JC_UINT32		uiBefore) 
{
	return 0;
}			
U32 Get_Audio_duration(JC_CHAR *pFileName) 
{
	return 0;
}					   
								
// JC_UINT32 jdd_TimerSystemGetTickCount (void)
// {
//   return E_FALSE;
// }

void jmms_goto_primaryMMI()
{
#ifdef JATAAYU_SUPPORT
	GoBackToHistory (SCR_ID_WAP_HOMEPAGE_SCREEN);
#endif
}

JC_UINT32 jdd_GetScrollOffsetFactor(JC_UINT32 uiWindowID)
{
    return 0;
}

JC_RETCODE jdd_GetScrollInfoForWindow (JC_UINT32 uiWindowID, JC_RECT *pstScrollCarRect,
	JC_RECT *pstScrollBarRect)
{
    return 0;
}
JC_RETCODE jdi_BEGetFocusedControlMimeInfo (JC_HANDLE			vHandle,
											ST_BE_URL_CONTENT	*pstCtrlMimeInfo)
{
	return 0;
}
JC_RETCODE jdi_BEExecuteReadPPP (JC_HANDLE					vHandle,
								 E_PPP_READ_PLUGIN			eType,
								 JC_HANDLE					vPPPArg) 
{
	return 0;
}

JC_RETCODE jdi_PushMsgGetList	(JC_HANDLE			vHandle,
								 ST_PUSH_INFO		**ppstPushInfo) 
{
	return 0;
}
JC_RETCODE jdi_PushMsgInitialize (JC_CHAR		*pmFileName,
								  JC_UINT32		uiMaxPushCount,
								  JC_HANDLE		vMemHandle,
								  JC_HANDLE		*pvHandle) 
{
	return 0;
}
JC_RETCODE jdi_PushMsgDeInitialize (JC_HANDLE		vHandle)
{
	return 0;
}
JC_RETCODE jdi_PushMsgStorePersistent (JC_HANDLE		vHandle) 
{
	return 0;
}
JC_RETCODE jdi_PushMsgAdd (JC_HANDLE			vHandle,
						   const ST_PUSH_INFO	*pstPushInfo)
{
	return 0;
}
JC_RETCODE jdi_PushMsgParse (JC_HANDLE		vHandle, 
							 JC_UINT8		*pucBuffer,
							 JC_INT32		iLength,
							 JC_INT8		*pcContentType,
							 const JC_INT8	*pcServerAddress,
							 ST_PUSH_INFO	*pstPushInfo)
{
	return 0;
}
void jdi_PushMsgStatistics (JC_HANDLE		vHandle,
							JC_UINT32		*puiTotalMsg,
							JC_UINT32		*puiUnreadMsg)
{
	return;
}
JC_RETCODE jdi_PushMsgSetReadStatus (JC_HANDLE				vHandle,
									 JC_BOOLEAN				bIsRead,
									 const ST_PUSH_INFO		*pstPushInfo) 
{
	return 0;
}
void jdi_PushMsgDelete (JC_HANDLE				vHandle,
						const ST_PUSH_INFO		*pstPushInfo)
{
	return;
}
void jdi_PushMsgDeleteAll (JC_HANDLE		vHandle)
{
	return;
}

JC_UINT32 jwap_GetLcdWidth()
{
  return 0;
}
JC_UINT32 jwap_GetLcdHeight()
{
  return 0;
}
JC_UINT32 jwap_GetSoftkeyHeight()
{
  return 0;
}
JC_UINT32 jwap_GetTitleHeight()
{
  return 0;
}
JC_UINT32 jwap_GetScrollbarbarArrowHight()
{
  return 0;
}
JC_UINT32 jwap_GetScrollbarbarArrowWidth()
{
  return 0;
}
JC_UINT32 jwap_GetScrollbarWidth()
{
  return 0;
}