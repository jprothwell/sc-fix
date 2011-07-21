// CoolSimulatorDlg.h : header file
//
//{{AFX_INCLUDES()
#include "coolphone.h"
//}}AFX_INCLUDES

#if !defined(AFX_COOLSIMULATORDLG_H__4E18A583_06BD_4401_99DE_B2856FB8E703__INCLUDED_)
#define AFX_COOLSIMULATORDLG_H__4E18A583_06BD_4401_99DE_B2856FB8E703__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCoolSimulatorDlg dialog
#include "SimulatorStart.h"	// Added by ClassView
#ifdef _DEBUG
#pragma comment(lib,"Lib\\NetworkSimulatorD.lib")
#else
#pragma comment(lib,"Lib\\NetWorkSimulatorR.lib")
#endif

#include "LayerDlg.h"	// Added by ClassView
//#include "SIMULATORLAYER\FIRMWARE\INCLUDE\cswtype.h"	// Added by ClassView
class CCoolSimulatorDlg : public CDialog
{
// Construction
public:
	CLayerDlg m_LayerDlg;
	CCoolSimulatorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCoolSimulatorDlg)
	enum { IDD = IDD_COOLSIMULATOR_DIALOG };
	CCoolPhone	m_Phone;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolSimulatorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCoolSimulatorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnChangeSizeCoolphonectrl(long nWidth, long nHeight);
	afx_msg void OnSIMKeyDownCoolphonectrl(LPCTSTR lpszKeyName, long nKeyCode);
	afx_msg void OnSIMKeyUpCoolphonectrl(LPCTSTR lpszKeyName, long nKeyCode);
	afx_msg void OnPenDownCoolphonectrl(long xPos, long yPos);
	afx_msg void OnPenUpCoolphonectrl(long xPos, long yPos);
	afx_msg void OnPenMoveCoolphonectrl(long xPos, long yPos, long nFlag);
	afx_msg void OnUpdateSkinCoolphonectrl();
	afx_msg void OnKeyDownCoolphonectrl(short FAR* KeyCode, short Shift);
	afx_msg void OnKeyUpCoolphonectrl(short FAR* KeyCode, short Shift);
	afx_msg void OnFILEExit();
	afx_msg void OnFileNetworkSimulator();
	afx_msg void OnFileSaveWindowPos();
	afx_msg void OnToolLoadSkin();
	afx_msg void OnToolInfoMonitor();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnOK();
	afx_msg void OnToolsExtKeypad();
	afx_msg void OnToolsLayerWatch();
	afx_msg void OnUpdateToolsLayerWatch(CCmdUI* pCmdUI);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CSimulatorStart m_SimulatorStart;
private:
	BOOL m_bLayerShow;
	BOOL m_bMouseDown;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLSIMULATORDLG_H__4E18A583_06BD_4401_99DE_B2856FB8E703__INCLUDED_)
