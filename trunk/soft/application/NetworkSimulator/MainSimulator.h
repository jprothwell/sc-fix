// MainSimulator.h: interface for the CMainSimulator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINSIMULATOR_H__B3709426_AE78_47FD_9365_0DC4F3C927E7__INCLUDED_)
#define AFX_MAINSIMULATOR_H__B3709426_AE78_47FD_9365_0DC4F3C927E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Afxmt.h"

#include "Simulator\GlobalConstants.h"
#include "Simulator\GlobalDefines.h"
#include "Simulator\ListDrop.h"
#include "Simulator\QueueGprot.h"
#include "Simulator\NWGCMLHandler.h"
#include "Simulator\SimAppToolkit.h"
#include "Simulator\MessageManager.h"
#include "SimulatorStart.h"
#define MAXBUTTONS    10

class CMainSimulator  
{
public:
	BOOL IsInit();
	BOOL ExitSimulator();
  BOOL InitSimulator();
	void SetListCtrl(CListDrop* plstMsgSend,CListDrop* plstMsgFrom);
	void SetWnd(CWnd* pWnd);
	CMainSimulator();
	virtual ~CMainSimulator();
protected:
  UINT32 MsgIdArray[MAXBUTTONS];
	CToolBarCtrl m_MostFreqMsgSeqBar;
	CWnd* m_pWnd;
	BOOL m_bIsInit;
  CListDrop* m_lstIncomingMessages;
	CListDrop* m_lstOutGoingMessages;
public:
  CString GetSeqNameForButton(UINT32 nID);
	void* PluginFunctionForOutGoingMessage(CString pLugin, CPtrList *pIncomingMsgDataList = NULL);
	void* GetDataMemoryForMessage(CMessage *pMessage,UINT32 *iSize,CString strDataSet);
	void CreateTimer(SendMessageDetails *messagedetailpacket);
	void SendMessageFreq(CMessage *clMessage,CData* dataset);
	void SendMessageFreq(CMessage *clMessage,CString strDataList);
	void SendMessageToMMI(CMessage *clMessage,CString strDataSet, U32 ModeType);
	void SendMsg(UINT32 MsgId, CPtrList *plstOutGoingMsg, U32 ModeType);
  BOOL SendBootupMessage();
public:
	void SetWriteQueueHandle(oslMsgqid handle);
	void SetWriteQueueHandler(NWWRITEQ pWriteHandler);
	void SetReadQueueHandler(NWRECEIVEQ pRecvHandler);
	void SetReadQueueHandle(oslMsgqid handle);
	CPtrList* GetAllAsyncEventsForApp(CPtrList *plstAsyncMsg, CString strRecvd);
public:
  BOOL OnRecieveMessage(int nDestId);
	BOOL OnSendMessage(MYQUEUE1* pOutGoingMsg);
	BOOL OnSendAsyncMessage(U32 ModeType, PopUpMsgStruct* pstPopMsg);
	void SendAsyncMessageToMMI(U32 ModeType, CString strRecvd);
	void OnSendMessages();
	BOOL OnModifyBootMsg();
public:
  void SendMsgSequence(U32 ModeType, UINT32 MessageSequenceId);
  CPtrList* GetAllResponseMsgs(UINT16 iMsgId, CPtrList *plstIncomingMsg, CPtrList *plstOutGoingMsg);
  void PluginFunctionForMessage(CString pLugin, CPtrList *pIncomingMsgDataList);
	CToolBarCtrl* GetToolBar();
	BOOL InitialiseToolBar();
	void CheckDeleteMessageIfExist(MYQUEUE1 stIncomingMsg, CPtrList *pIncomingMsgDataList);
	void AddMessageToList(MYQUEUE1 stIncomingMsg, CPtrList *pIncomingMsgDataList);
	CString GetSystemTimeString();
	void SetTheLibraryIdentifier(HINSTANCE lib_id);
	void DisplayMessageInList(MYQUEUE1 msg, BOOL Flag, int nEventMode);
	void* RetDataPointerForInMessage(int iMsgId, CPtrList *pIncomingMsgDataList);
  CMessageManager *m_pMessageManager;
  CPtrList *m_pIncomingMsgDataList;
#ifdef _DUAL_SIM_
  CPtrList *m_pIncomingMsgDataList2;
#endif /*_DUAL_SIM_*/
  int m_RsponseMode;
  int m_IsClickOnIncomingMsgList;
  int m_ClickedMsgId;
  CSimAppToolkit m_SAT;
  LPARAM m_IndexInIncomingMsgList;
  HINSTANCE library_identifier;    //identifies the resource filr...used for loading strings from resource file....
  MYQUEUE1 m_IncomingMsg,*m_OutGoingMsg;
  CString stringID_to_string;
  HIMAGELIST hNormalImageList;

  // temporarily variable
private:
  CPtrList* m_ptrAyncEventsList;
  SendMessageDetails m_MessageDetailPacket;
  int m_MessageId;// onmessagereceive will set this and used by GetBufferFromIncomingMessage function
};

#endif // !defined(AFX_MAINSIMULATOR_H__B3709426_AE78_47FD_9365_0DC4F3C927E7__INCLUDED_)
