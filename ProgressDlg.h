#pragma once


class CProgressDlg : public CDialog
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = NULL);
	virtual ~CProgressDlg();

	enum { IDD = IDD_DLG_PROGRESS };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_strMsg;
	void Close();
private:
	int m_progress;
	CDialog *m_Dialog;
	HANDLE m_hEvent;
public:
	afx_msg void OnClose();
	virtual void PostNcDestroy();
	//void Create();
	afx_msg void OnStnClickedTxtMsg();
};
