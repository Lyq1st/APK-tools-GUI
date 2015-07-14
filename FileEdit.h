#pragma once


// CFileEdit

class CFileEdit : public CEdit
{
	DECLARE_DYNAMIC(CFileEdit)

public:
	CFileEdit();
	virtual ~CFileEdit();
	afx_msg void OnDropFiles(HDROP hDropInfo);
protected:
	DECLARE_MESSAGE_MAP()
};


