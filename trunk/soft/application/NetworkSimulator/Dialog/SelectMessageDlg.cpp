// SelectMessageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\NetworkSimulator.h"
#include "SelectMessageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectMessageDlg dialog


CSelectMessageDlg::CSelectMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectMessageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectMessageDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSelectMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectMessageDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectMessageDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectMessageDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectMessageDlg message handlers
