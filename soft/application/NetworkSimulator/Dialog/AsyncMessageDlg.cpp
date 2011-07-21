// AsyncMessageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetworkSimulator.h"
#include "AsyncMessageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAsyncMessageDlg dialog


CAsyncMessageDlg::CAsyncMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsyncMessageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAsyncMessageDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAsyncMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAsyncMessageDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAsyncMessageDlg, CDialog)
	//{{AFX_MSG_MAP(CAsyncMessageDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAsyncMessageDlg message handlers
