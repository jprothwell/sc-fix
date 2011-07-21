// GenericInput.cpp : implementation file
//

#include "stdafx.h"
#include "NetworkSimulator.h"
#include "GenericInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGenericInput dialog


CGenericInput::CGenericInput(CWnd* pParent /*=NULL*/)
	: CDialog(CGenericInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGenericInput)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
  
  m_pMessagemanager = NULL;
}


void CGenericInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenericInput)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGenericInput, CDialog)
	//{{AFX_MSG_MAP(CGenericInput)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_DATASET_DETAILS, OnListDropEditHandler)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DATASET_DETAILS, OnLvnItemchangedListDatasetDetails)
	ON_BN_CLICKED(ID_BUTTON_MAKEBUFFER, OnButtonMakebuffer)
	ON_BN_CLICKED(IDC_BUTTON_ONCLOSE, OnButtonOnclose)
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenericInput message handlers

BOOL CGenericInput::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGenericInput::OnListDropEditHandler(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CGenericInput::OnLvnItemchangedListDatasetDetails(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CGenericInput::OnButtonMakebuffer() 
{
	// TODO: Add your control notification handler code here
	
}

void CGenericInput::OnButtonOnclose() 
{
	// TODO: Add your control notification handler code here
	
}

void CGenericInput::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here
	
}
