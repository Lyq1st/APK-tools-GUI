
#pragma once

#ifndef __AFXWIN_H__
	#error "inlcude stdafx.h to generaet PCH file"
#endif

#include "resource.h"	


class CApkToolApp : public CWinAppEx
{
public:
	CApkToolApp();

	public:
	virtual BOOL InitInstance();


	DECLARE_MESSAGE_MAP()
};

extern CApkToolApp theApp;