
#include "stdafx.h"
#include "ApkTool.h"
#include "ProgressDlg.h"

WCHAR *szProgress[]={
	L"",
	L".",
	L"..",
	L"..."
};

IMPLEMENT_DYNAMIC(CProgressDlg, CDialog)

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDlg::IDD, pParent)
{
	m_hEvent = ::CreateEvent(NULL,TRUE,FALSE,NULL);
}

CProgressDlg::~CProgressDlg()
{
	CloseHandle(m_hEvent);
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_STN_CLICKED(IDC_TXT_MSG, &CProgressDlg::OnStnClickedTxtMsg)
END_MESSAGE_MAP()

BOOL CProgressDlg::OnInitDialog()
{

	CDialog::OnInitDialog();
	m_progress =0;

	GetDlgItem(IDC_TXT_MSG)->SetWindowText(m_strMsg);
	SetTimer(1, 500, NULL);
	SetEvent(m_hEvent);
	return TRUE;
}
void CProgressDlg::OnTimer(UINT_PTR nIDEvent)
{

	CString msg;
	msg.Format(L"%s%s",m_strMsg, szProgress[m_progress%4]);

	GetDlgItem(IDC_TXT_MSG)->SetWindowText(msg);
	CDialog::OnTimer(nIDEvent);
	m_progress++;
}

void CProgressDlg::Close()
{
	DWORD wait = WaitForSingleObject(m_hEvent,INFINITE);
	ResetEvent(m_hEvent);
	KillTimer(1);
	//this->PostNcDestroy();
}
void CProgressDlg::OnClose()
{
	CDialog::OnClose();
	delete this;
}
void CProgressDlg::PostNcDestroy()
{
		//delete this;
		CDialog::PostNcDestroy();
}


void CProgressDlg::OnStnClickedTxtMsg()
{
	// TODO: Add your control notification handler code here
}
