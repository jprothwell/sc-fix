// SimulatorStart.h: interface for the CSimulatorStart class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMULATORSTART_H__C1E9D920_58D4_4913_840D_11A899439429__INCLUDED_)
#define AFX_SIMULATORSTART_H__C1E9D920_58D4_4913_840D_11A899439429__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _EXPORT_NETWORK_SIMULATOR
#define EXPORT_NETWORK_SIMULATOR __declspec(dllexport)
#else
#define EXPORT_NETWORK_SIMULATOR
#endif

typedef void* oslMsgqid_handle;
typedef void* MYQUEUE_HANDLE;
typedef unsigned int (*NWRECEIVEQ)(oslMsgqid_handle msgqid, MYQUEUE_HANDLE Message);
typedef void (*NWWRITEQ)(MYQUEUE_HANDLE Message);



class EXPORT_NETWORK_SIMULATOR CSimulatorStart  
{
public:
	CSimulatorStart();
	virtual ~CSimulatorStart();
public:
	BOOL Bootup();
	void ShowSimulator();
	BOOL InitSimulator(int nXPos,int nYPos);
  BOOL IsInit();
  void SetReadQueueHandle(oslMsgqid_handle handle);
	void SetReadQueueHandler(NWRECEIVEQ pRecvHandler);
	void SetWriteQueueHandler(NWWRITEQ pWriteHandler);
	void SetWriteQueueHandle(oslMsgqid_handle handle);
};

#endif // !defined(AFX_SIMULATORSTART_H__C1E9D920_58D4_4913_840D_11A899439429__INCLUDED_)
