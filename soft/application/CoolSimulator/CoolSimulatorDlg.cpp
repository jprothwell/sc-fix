// CoolSimulatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CoolSimulator.h"
#include "CoolSimulatorDlg.h"
#include "Screen.h"
#include "TraceModule.h"    // Trace Module

extern "C" 
{
	//#include "mmi_data_types.h"
	//#include "MMI_features.h"
	#include "GlobalConstants.h" /* KEY_INVALID */
	#include "frameworkstruct.h"
	#include "taskinit.h"
  #include "lcd_sw_inc.h"   // Get LCD_WIDTH,LCD_HEIGHT

 	extern void InitApplication();
	extern void lcd_screen_init();
	extern void	UI_update(void);
#ifdef TOUCH_PANEL_SUPPORT
	extern void mmi_pen_simulator_button_down_hdlr(S16 x, S16 y);
	extern void mmi_pen_simulator_button_move_hdlr(S16 x, S16 y);
	extern void mmi_pen_simulator_button_up_hdlr(S16 x, S16 y);
#endif
	HWND	g_hSimWnd ;
}

extern "C" void UpdateScreenFun(unsigned int *simulator_buffer, BITMAPINFO *lpbitmap_info)
{
   
  ((CCoolSimulatorDlg*)(AfxGetApp()->m_pMainWnd))->m_Phone.Screen().UpdateScreen((long*)simulator_buffer,(long*)lpbitmap_info);

}

extern "C" void UpdateLayerFun(int nLayer, unsigned int *simulator_buffer, BITMAPINFO *lpbitmap_info)
{
  ((CCoolSimulatorDlg*)(AfxGetApp()->m_pMainWnd))->m_LayerDlg.UpdateScreen(nLayer,simulator_buffer,lpbitmap_info);
}


#ifdef __MMI_BLACKBERRY_QWERTY__
static const int SimKeyPadMap[] = {
	KEY_0,
	KEY_1,	
	KEY_2,
	KEY_3,	
	KEY_4,	
	KEY_5,	
	KEY_6,	
	KEY_7,	
	KEY_8,	
	KEY_9,	
	KEY_LSK,	//10		
	KEY_RSK,			
	KEY_CSK,
	KEY_UP_ARROW,			
	KEY_DOWN_ARROW,		
	KEY_LEFT_ARROW,	//15	
	KEY_RIGHT_ARROW,		
	KEY_SEND,					
	KEY_END,					
	KEY_CLEAR,				
	KEY_STAR,	          
	KEY_POUND,	
	KEY_VOL_UP,				
	KEY_VOL_DOWN,			
	KEY_CAMERA,
	KEY_ENTER,
	KEY_MSN,
	KEY_DOLLAR,
	KEY_SYM,
	KEY_WAP,
	KEY_IP,
	KEY_EXTRA_1,
	KEY_EXTRA_2,
#ifdef __SENDKEY2_SUPPORT__
    KEY_SEND1,
    KEY_SEND2,
    KEY_SEND3,
    KEY_SEND4,
#endif
	KEY_BACKSPACE1,
	KEY_SMS,
	KEY_CLOG,	
	KEY_B,
	KEY_G,
	KEY_H,
	KEY_I,//also with '-'
	KEY_J,
	KEY_K,//ALSO WITH '_'
	KEY_L,	//ALSO WITH '\'
	KEY_M,
	KEY_N,
	KEY_O,//ALSO WITH '+'
	KEY_P,//ALSO WITH '='
	KEY_T,//also with '!' 
	KEY_U,
	KEY_V,
	KEY_Y,
											

	KEY_COMMA,										
	KEY_FULLSTOP,	
	KEY_SHIFT1,
	KEY_SHIFT_INPUTM,
	KEY_SPACE1,
	KEY_AND,
	KEY_INTEGRR,
	KEY_CTRL1,
	KEY_NEWLINE,
	KEY_QQ,
    KEY_QF,
	KEY_TV,
	MAX_KEYS
};
#else
static const int SimKeyPadMap[] = 
{
KEY_0,
KEY_1,	
KEY_2,	
KEY_3,
KEY_4,		
KEY_5,
KEY_6,
KEY_7,
KEY_8,				
KEY_9,
KEY_LSK,			     //10
KEY_RSK,	         //11  
KEY_UP_ARROW,      //12
KEY_DOWN_ARROW,    //13
KEY_LEFT_ARROW,    //14 
KEY_RIGHT_ARROW, //15
KEY_SEND,          //16
KEY_END,	         //17
KEY_CLEAR,	       //18
KEY_STAR,          //19
KEY_POUND,         //20
KEY_VOL_UP,        //21 
KEY_VOL_DOWN,      //22
KEY_INVALID,		   //23
KEY_QUICK_ACS,	   //24
KEY_ENTER,         //25
KEY_EXTRA_1,       //26
KEY_EXTRA_2,       //27
KEY_INVALID,       //28
KEY_INVALID,         //29
KEY_BACKSPACE1,
KEY_SMS,
KEY_CLOG,	
KEY_A, //33
KEY_H,
KEY_I,//also with '-'
KEY_J,
KEY_K,//ALSO WITH '_'
KEY_L,	//ALSO WITH '\'
KEY_M, //39
KEY_O,//ALSO WITH '+'
KEY_P,//ALSO WITH '='
KEY_Q,//also with '!' 
KEY_S,

KEY_U,
KEY_W,//also with ' " '
KEY_Y,
KEY_Z,											
										

KEY_COMMA,		//48								
KEY_FULLSTOP,	
KEY_SHIFT1,
KEY_SHIFT_INPUTM,
KEY_SPACE1,
KEY_AND,
KEY_INTEGRR,
KEY_CTRL1,
KEY_NEWLINE,
};
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
	static char THIS_FILE[] = __FILE__ ;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolSimulatorDlg dialog

CCoolSimulatorDlg::CCoolSimulatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCoolSimulatorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCoolSimulatorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bMouseDown = FALSE;
  m_bLayerShow = FALSE;
}

void CCoolSimulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCoolSimulatorDlg)
	DDX_Control(pDX, IDC_COOLPHONECTRL, m_Phone);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCoolSimulatorDlg, CDialog)
	//{{AFX_MSG_MAP(CCoolSimulatorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_Exit, OnFILEExit)
	ON_COMMAND(IDM_FILE_NETWORK_SIMULATOR, OnFileNetworkSimulator)
	ON_COMMAND(IDM_FILE_SAVE_WINDOW_POS, OnFileSaveWindowPos)
	ON_COMMAND(IDM_TOOL_LOAD_SKIN, OnToolLoadSkin)
	ON_COMMAND(IDM_TOOL_INFOMONITOR, OnToolInfoMonitor)
	ON_WM_TIMER()
	ON_COMMAND(IDM_TOOLS_EXT_KEYPAD, OnToolsExtKeypad)
	ON_COMMAND(IDM_TOOLS_LAYER_WATCH, OnToolsLayerWatch)
	ON_UPDATE_COMMAND_UI(IDM_TOOLS_LAYER_WATCH, OnUpdateToolsLayerWatch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolSimulatorDlg message handlers

BOOL CCoolSimulatorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

  // 
  m_LayerDlg.Create(IDD_LAYER_DLG,this);
  m_LayerDlg.ShowWindow(SW_HIDE);
  m_LayerDlg.SetScreenSize(CSize(LCD_WIDTH,LCD_HEIGHT));
  

  // Add Trace Module
  CTraceModule::DebugWindowAlloc(this->m_hWnd);

  // Get module file name
  TCHAR szFileName[_MAX_PATH] = {0};
  TCHAR szDir[_MAX_PATH] = {0};
  TCHAR szDriver[_MAX_PATH] = {0};
  GetModuleFileName(NULL, szFileName, _MAX_PATH);
  _tsplitpath(szFileName, szDriver, szDir, NULL, NULL);
  
  //获得模拟器Skin配置列表
  CString str = CString(_T("PhoneSkins\\PhoneList.ini"));
  //  str.LoadString(IDS_STRING_PHONE_LIST);
  CString strExeDir = CString(szDriver) + szDir+str;
  
  this->m_Phone.LoadSkinProfile(strExeDir);
  
  g_hSimWnd = this->m_hWnd;
  InitApplication();
  lcd_screen_init();

  // 设置Layer Watch对话框的初始位置
  CRect rect;
  GetWindowRect(&rect);
  m_LayerDlg.SetWindowPos(NULL,rect.right,rect.top,0,0,SWP_NOSIZE);
  
  return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCoolSimulatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCoolSimulatorDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCoolSimulatorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BEGIN_EVENTSINK_MAP(CCoolSimulatorDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CCoolSimulatorDlg)
	ON_EVENT(CCoolSimulatorDlg, IDC_COOLPHONECTRL, 1 /* ChangeSize */, OnChangeSizeCoolphonectrl, VTS_I4 VTS_I4)
	ON_EVENT(CCoolSimulatorDlg, IDC_COOLPHONECTRL, 8 /* SIMKeyDown */, OnSIMKeyDownCoolphonectrl, VTS_BSTR VTS_I4)
	ON_EVENT(CCoolSimulatorDlg, IDC_COOLPHONECTRL, 7 /* SIMKeyUp */, OnSIMKeyUpCoolphonectrl, VTS_BSTR VTS_I4)
	ON_EVENT(CCoolSimulatorDlg, IDC_COOLPHONECTRL, 3 /* PenDown */, OnPenDownCoolphonectrl, VTS_I4 VTS_I4)
	ON_EVENT(CCoolSimulatorDlg, IDC_COOLPHONECTRL, 4 /* PenUp */, OnPenUpCoolphonectrl, VTS_I4 VTS_I4)
	ON_EVENT(CCoolSimulatorDlg, IDC_COOLPHONECTRL, 5 /* PenMove */, OnPenMoveCoolphonectrl, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CCoolSimulatorDlg, IDC_COOLPHONECTRL, 6 /* UpdateSkin */, OnUpdateSkinCoolphonectrl, VTS_NONE)
	ON_EVENT(CCoolSimulatorDlg, IDC_COOLPHONECTRL, -602 /* KeyDown */, OnKeyDownCoolphonectrl, VTS_PI2 VTS_I2)
	ON_EVENT(CCoolSimulatorDlg, IDC_COOLPHONECTRL, -604 /* KeyUp */, OnKeyUpCoolphonectrl, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCoolSimulatorDlg::OnChangeSizeCoolphonectrl(long nWidth, long nHeight) 
{
	// TODO: Add your control notification handler code here
  CRect rect1,rect2;
  this->GetWindowRect(rect1);
  this->GetClientRect(rect2);
  this->MoveWindow(100,100, rect1.Width(), rect1.Height());
	this->m_Phone.MoveWindow(0,0,nWidth,nHeight);
  this->SetWindowPos(NULL,0,0,nWidth+(rect1.Width()-rect2.Width()),nHeight+(rect1.Height()-rect2.Height()),SWP_NOMOVE);

  GetDlgItem(IDC_STATIC_COORDINATE)->MoveWindow(nWidth-56,1,55,20);
}

/*****************************************************************************
* FUNCTION
*  writeKeyMsgOnCondition
* DESCRIPTION
*  If message queue is not too crowed, write key message. Also onsider key up pairing.
*
* PARAMETERS
*	void
* RETURNS
*  wrote to queue or not
*  
* GLOBALS AFFECTED
*  void
*****************************************************************************/
BOOL writeKeyMsgOnCondition(MYQUEUE *Message)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   BOOL        toSend = TRUE;
   U8          num;
   static BOOL waitKeyUpMsg = TRUE;
   /* for debugging */
   static int  keyNumSkipped = 0;

	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   /* check sending critera */
   if ((num = OslNumOfQMsgs() < (MAX_PRT_NODES - (MAX_PRT_NODES/4.0))) ||
      ((waitKeyUpMsg == TRUE) && ((Message->oslMsgId == WM_KEYUP) || (Message->oslMsgId == WM_LBUTTONUP))))
   {
      toSend = TRUE;
   }
   else
   {
      toSend = FALSE;
   }

   /* send the msg */
   if (toSend == TRUE)
   {
      if ((Message->oslMsgId != WM_KEYUP) && (Message->oslMsgId != WM_LBUTTONUP))
         waitKeyUpMsg = TRUE;
      else
         waitKeyUpMsg = FALSE;

      OslMsgSendExtQueue(Message);
      return TRUE;
   }
   else
   {
      keyNumSkipped ++;
      return FALSE;
   }
}
void CCoolSimulatorDlg::OnSIMKeyDownCoolphonectrl(LPCTSTR lpszKeyName, long nKeyCode) 
{
	// TODO: Add your control notification handler code here
	UINT				i;
	void				*tempKeyBrdMsg;
	static MYQUEUE	Message;
	KEYBRD_MESSAGE	*KeyBrdMsg;


	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
  i = SimKeyPadMap[nKeyCode];
  if(i >= MAX_KEYS) i = KEY_INVALID;
	if(i != KEY_INVALID)
	{
		MessageBeep(0xFFFFFFFF);
		Message.oslSrcId = 0xFFFF;
		Message.oslDestId = MOD_PRT;
		Message.oslMsgId = WM_LBUTTONDOWN;
		/* OslFreeDataPtr(Message.oslDataPtr); */
		tempKeyBrdMsg=OslConstructDataPtr(sizeof(KEYBRD_MESSAGE));
		KeyBrdMsg =(KEYBRD_MESSAGE *)tempKeyBrdMsg;
		/* KeyBrdMsg = (KEYBRD_MESSAGE *)OslConstructDataPtr(sizeof(KEYBRD_MESSAGE)); */
		KeyBrdMsg->nKeyCode = i;
		Message.oslDataPtr = KeyBrdMsg;
     if (writeKeyMsgOnCondition(&Message) != TRUE)
	 {
		 OslFreeDataPtr(tempKeyBrdMsg);
	 }
		
	}
}

void CCoolSimulatorDlg::OnSIMKeyUpCoolphonectrl(LPCTSTR lpszKeyName, long nKeyCode) 
{
	// TODO: Add your control notification handler code here
	int   i;
	void				*tempKeyBrdMsg;
	static MYQUEUE	Message;
	KEYBRD_MESSAGE	*KeyBrdMsg;


	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	  i = SimKeyPadMap[nKeyCode];
  if(i >= MAX_KEYS) i = KEY_INVALID;

	/* MessageBeep(0xFFFFFFFF); */
	if(i != KEY_INVALID)
	{
		{
			Message.oslSrcId = 0xFFFF;
			Message.oslDestId=MOD_PRT;
			Message.oslMsgId =WM_LBUTTONUP;
			/* OslFreeDataPtr(Message.oslDataPtr); */
			tempKeyBrdMsg = OslConstructDataPtr(sizeof(KEYBRD_MESSAGE));
			KeyBrdMsg = (KEYBRD_MESSAGE *)tempKeyBrdMsg;
			/* KeyBrdMsg = (KEYBRD_MESSAGE *)OslConstructDataPtr(sizeof(KEYBRD_MESSAGE)); */
			KeyBrdMsg->nKeyCode = i;
			Message.oslDataPtr=KeyBrdMsg;
         if (writeKeyMsgOnCondition(&Message) != TRUE)
            OslFreeDataPtr(tempKeyBrdMsg);
		}
	}
}

void CCoolSimulatorDlg::OnPenDownCoolphonectrl(long xPos, long yPos) 
{
	// TODO: Add your control notification handler code here
#ifdef TOUCH_PANEL_SUPPORT
		mmi_pen_simulator_button_down_hdlr((S16)xPos, (S16)yPos);
#endif
		m_bMouseDown = TRUE;
}

void CCoolSimulatorDlg::OnPenMoveCoolphonectrl(long xPos, long yPos, long nFlag) 
{
	// TODO: Add your control notification handler code here
	if (m_bMouseDown)
	{
#ifdef TOUCH_PANEL_SUPPORT
  mmi_pen_simulator_button_move_hdlr((S16)xPos, (S16)yPos);
#endif
	}

  CString str;
  str.Format(_T("%ld,%ld"),xPos,yPos);
	SetDlgItemText(IDC_STATIC_COORDINATE,str);
}
void CCoolSimulatorDlg::OnPenUpCoolphonectrl(long xPos, long yPos) 
{
	// TODO: Add your control notification handler code here
	if(m_bMouseDown)
	{
#ifdef TOUCH_PANEL_SUPPORT
		mmi_pen_simulator_button_up_hdlr((S16)xPos, (S16)yPos);
#endif
		m_bMouseDown = FALSE;
	}
}

void CCoolSimulatorDlg::OnUpdateSkinCoolphonectrl() 
{
	// TODO: Add your control notification handler code here
		UI_update();
	
}

void CCoolSimulatorDlg::OnKeyDownCoolphonectrl(short FAR* KeyCode, short Shift) 
{
	// TODO: Add your control notification handler code here
	
}

void CCoolSimulatorDlg::OnKeyUpCoolphonectrl(short FAR* KeyCode, short Shift) 
{
	// TODO: Add your control notification handler code here
	
}

void CCoolSimulatorDlg::OnFILEExit() 
{
	// TODO: Add your command handler code here
	SendMessage(WM_CLOSE);
}

void CCoolSimulatorDlg::OnFileNetworkSimulator() 
{
	// TODO: Add your command handler code here

	if (m_SimulatorStart.IsInit())
	{
		m_SimulatorStart.ShowSimulator();
	}
	else
	{
    //InitPCSimulatorBootup();

  
    CRect rect;
    GetWindowRect(&rect);
    m_SimulatorStart.InitSimulator(rect.right,rect.top);
    m_SimulatorStart.SetReadQueueHandler((NWRECEIVEQ)OslReceiveMsgExtQ);
    m_SimulatorStart.SetWriteQueueHandler((NWWRITEQ)OslMsgSendExtQueue);
    m_SimulatorStart.SetReadQueueHandle((oslMsgqid_handle)task_info_g1[MOD_PST].task_ext_qid);
    m_SimulatorStart.SetWriteQueueHandle((oslMsgqid_handle)task_info_g1[MOD_PRT].task_ext_qid);
    m_SimulatorStart.Bootup();

	}

  SetForegroundWindow();
}

void CCoolSimulatorDlg::OnFileSaveWindowPos() 
{
	// TODO: Add your command handler code here

}


void CCoolSimulatorDlg::OnToolLoadSkin() 
{
	// TODO: Add your command handler code here
  m_Phone.ShowSkinSetupDlg();
}

void CCoolSimulatorDlg::OnToolInfoMonitor() 
{
	// TODO: Add your command handler code here
}

void CCoolSimulatorDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
		oslTimerid		timerid;
	
	timerid = (oslTimerid)nIDEvent;
	timerid->tmr_callback(timerid->tmr_callback_arg);

	CDialog::OnTimer(nIDEvent);
}

bool						isShutDownSystem = true;

/*****************************************************************************
* FUNCTION
*	KillWindowObject
* DESCRIPTION
*	Distroy the window
*
* PARAMETERS
*  nil
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
extern "C"{
	void KillWindowObject(void)
	{
		/*----------------------------------------------------------------*/
		/* Local Variables                                                */
		/*----------------------------------------------------------------*/

		/*----------------------------------------------------------------*/
		/* Code Body                                                      */
		/*----------------------------------------------------------------*/
//     ((CCoolSimulatorDlg*)(AfxGetApp()->m_pMainWnd))->m_SimulatorStart.SetReadQueueHandler(NULL);
//     ((CCoolSimulatorDlg*)(AfxGetApp()->m_pMainWnd))->m_SimulatorStart.SetWriteQueueHandler(NULL);
//     ((CCoolSimulatorDlg*)(AfxGetApp()->m_pMainWnd))->m_SimulatorStart.SetReadQueueHandle(NULL);
//     ((CCoolSimulatorDlg*)(AfxGetApp()->m_pMainWnd))->m_SimulatorStart.SetWriteQueueHandle(NULL);

		if(isShutDownSystem == true)
		{
			isShutDownSystem = false;
			SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd, WM_CLOSE, 0, 0);
		}
	} /* end of KillWindowObject */
}


void CCoolSimulatorDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}

void CCoolSimulatorDlg::OnToolsExtKeypad() 
{
	// TODO: Add your command handler code here
  CRect rect;
  GetWindowRect(&rect);
	this->m_Phone.ShowExtendKeyboardEx(rect.right,rect.top+300,TRUE);
}

void CCoolSimulatorDlg::OnToolsLayerWatch() 
{
	// TODO: Add your command handler code here
  m_bLayerShow = m_bLayerShow?FALSE:TRUE;
  m_LayerDlg.ShowWindow(m_bLayerShow?SW_SHOW:SW_HIDE);
}

void CCoolSimulatorDlg::OnUpdateToolsLayerWatch(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
  pCmdUI->SetCheck(m_bLayerShow?1:0);
}
