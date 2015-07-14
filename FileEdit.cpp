
#include "stdafx.h"
#include "ApkTool.h"
#include "FileEdit.h"


IMPLEMENT_DYNAMIC(CFileEdit, CEdit)

CFileEdit::CFileEdit()
{

}

CFileEdit::~CFileEdit()
{
}


BEGIN_MESSAGE_MAP(CFileEdit, CEdit)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

void CFileEdit::OnDropFiles(HDROP hDropInfo)
{

	WCHAR szFileName[MAX_PATH + 1];
	int nFileCount;  
	nFileCount=::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, MAX_PATH);
	if(nFileCount == 1)
	{
		DragQueryFile(hDropInfo, 0, szFileName, MAX_PATH);
		SetWindowText(szFileName); 
	}
	else
	{
		SetWindowText(L"no more file!");
	}
	::DragFinish(hDropInfo);
	CEdit::OnDropFiles(hDropInfo);
}
