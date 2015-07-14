/********************************************************************
	Created:	2015/07/14:   10:34
	FileName:	ApkToolDlg.cpp
	Author:		Lyq (iamlyq4pub@gmail.com, admin@droid-sec.com)
	Version:	1.0
	Purpose:	
*********************************************************************/
#include "stdafx.h"
#include "ApkTool.h"
#include "ApkToolDlg.h"
#include "shlobj.h"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


CApkToolDlg::CApkToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CApkToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pprogressDlg=NULL;
}

void CApkToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DECODE, m_editDecode);
	DDX_Control(pDX, IDC_EDIT_BUILD, m_editBuild);
	DDX_Control(pDX, IDC_LIST_LOG, m_log);
	DDX_Control(pDX, IDC_EDIT_BAKSMALI, m_editBaksmali);
	DDX_Control(pDX, IDC_EDIT_DEX2JAR, m_editDex2jar);
	DDX_Control(pDX, IDC_EDIT_SMALI, m_editSmali);
	DDX_Control(pDX, IDC_EDIT_SIGN, m_editSign);
	DDX_Control(pDX, IDC_EDIT_JAR2JAVA, m_editJar2Java);
	DDX_Control(pDX, IDC_EDIT_GETCER, m_editCert);
	DDX_Control(pDX, IDC_EDIT_GETXML, m_editXML);
}

BEGIN_MESSAGE_MAP(CApkToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_DECODE, &CApkToolDlg::OnBnClickedBtnDecode)
	ON_BN_CLICKED(IDC_BTN_BUILD, &CApkToolDlg::OnBnClickedBtnBuild)
	ON_BN_CLICKED(IDC_BTN_SHOW_LOG, &CApkToolDlg::OnBnClickedBtnShowLog)
	ON_BN_CLICKED(IDC_BTN_CLEAR_LOG, &CApkToolDlg::OnBnClickedBtnClearLog)
	ON_BN_CLICKED(IDC_BTN_BAKSMALI, &CApkToolDlg::OnBnClickedBtnBaksmali)
	ON_BN_CLICKED(IDC_BTN_SMALI, &CApkToolDlg::OnBnClickedBtnSmali)
	ON_BN_CLICKED(IDC_BTN_DEX2JAR, &CApkToolDlg::OnBnClickedBtnDex2jar)
	ON_BN_CLICKED(IDC_BTN_SIGN, &CApkToolDlg::OnBnClickedBtnSign)
	ON_BN_CLICKED(IDC_BTN_EXPORT_LOG, &CApkToolDlg::OnBnClickedBtnExportLog)
	ON_BN_CLICKED(IDC_BTN_JAR2JAVA, &CApkToolDlg::OnBnClickedBtnJar2java)
	ON_BN_CLICKED(IDC_BTN_GETCER, &CApkToolDlg::OnBnClickedBtnGetcer)
	ON_BN_CLICKED(IDC_BTN_GETXML, &CApkToolDlg::OnBnClickedBtnGetXML)
	ON_WM_SIZE()
END_MESSAGE_MAP()


BOOL CApkToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	InitializeCriticalSection(&m_csCtlList);

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	Update();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	m_tooltip.Create(this);

	m_tooltip.SetDelayTime(100);
	m_tooltip.SetDelayTime(TTDT_AUTOPOP,10000);
	m_tooltip.SetTipBkColor(RGB(0,255,255));
	m_tooltip.SetMaxTipWidth(300);
	m_tooltip.AddTool(&m_editDecode, L"Drag APK file!(cautions: no space or Chinese symbols in the path)");
	m_tooltip.AddTool(&m_editBuild, L"Drag directory!(cautions: no space or Chinese symbols in the path)");

	m_tooltip.AddTool(&m_editSign, L"Drag file!");
	m_tooltip.AddTool(&m_editBaksmali, L"Drag dex or odex file!(cautions:odex file need extra library, get these files from \\system\\framework, then put it in the same directory as odex file)");
	m_tooltip.AddTool(&m_editSmali, L"Drag directory!");
	m_tooltip.AddTool(&m_editDex2jar, L"Drag file!");
	m_tooltip.AddTool(&m_editJar2Java, L"Drag file!");
	m_tooltip.AddTool(&m_editCert,L"Drag APK file to here!");
	m_tooltip.AddTool(&m_editXML,L"Drag APK file to here!");

	m_log.SetHorizontalExtent(1000);

	m_showLog = TRUE;
	OnBnClickedBtnShowLog();
	m_log.SendMessage(WM_VSCROLL,SB_BOTTOM,NULL); 
	return TRUE;
}

void CApkToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


void CApkToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CApkToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CApkToolDlg::PreTranslateMessage(MSG* pMsg) 
{
	m_tooltip.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}

void CApkToolDlg::OnBnClickedBtnShowLog()
{
	RECT rc;
	GetWindowRect(&rc);
	if (m_showLog)
	{
		GetDlgItem(IDC_BTN_SHOW_LOG)->SetWindowText(L"Log >>");
		SetWindowPos(NULL, 0, 0, 470, 500, SWP_NOMOVE);
		GetDlgItem(IDC_BTN_CLEAR_LOG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_EXPORT_LOG)->ShowWindow(SW_SHOW);
	} 
	else
	{
		GetDlgItem(IDC_BTN_SHOW_LOG)->SetWindowText(L"Log <<");
		SetWindowPos(NULL, 0, 0, 470, 350, SWP_NOMOVE);
		GetDlgItem(IDC_BTN_CLEAR_LOG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_EXPORT_LOG)->ShowWindow(SW_HIDE);
	}
	m_showLog = !m_showLog;
}

void CApkToolDlg::OnBnClickedBtnClearLog()
{
	m_log.ResetContent();
}

void CApkToolDlg::OnBnClickedBtnExportLog()
{
	CString strLogFile;
	WCHAR szModuleFileName[MAX_PATH];
	GetModuleFileName(NULL, szModuleFileName, MAX_PATH);
//	strLogFile.Format(L"%s%sapktool.log",smt::GetDriveByFullPath(szModuleFileName),smt::GetDirByFullPath(szModuleFileName));
	CFileDialog hFileDlg(FALSE, L"*.log", strLogFile, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, L"Text file(*.log)|*.log|All file(*.*)|*.*||", this);

	hFileDlg.m_ofn.hwndOwner = m_hWnd;
	hFileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME);
	hFileDlg.m_ofn.lpstrTitle = TEXT("export log...\0");
	hFileDlg.m_ofn.nMaxFile = MAX_PATH;

	if(hFileDlg.DoModal() == IDOK)
	{
		strLogFile = hFileDlg.GetPathName();
		CFile file;
		file.Open(strLogFile,CFile::modeCreate|CFile::modeWrite);
		CString str;

		int n=0;
		for(int i=0;i<m_log.GetCount();i++)
		{
			n = m_log.GetTextLen(i);
			m_log.GetText(i, str);

			file.Write(CW2A(str), strlen(CW2A(str)));
			file.Write("\r\n",2);
		}
		file.Close();
		MessageBox(L"export log finished");
	}
}
void CApkToolDlg::OnBnClickedBtnDecode()
{
	m_editDecode.GetWindowText(m_decodeFileName);

	if(m_decodeFileName.IsEmpty() || !PathFileExists(m_decodeFileName))
	{
		MessageBox(L"Input error!");
		return;
	}
		ShowProgressDlg(L"decompiling, waiting...");
	AfxBeginThread(DecodeThread, this);

}

void CApkToolDlg::OnBnClickedBtnBuild()
{
	m_editBuild.GetWindowText(m_bulidDirectory);
	if(m_bulidDirectory.IsEmpty() || !PathFileExists(m_bulidDirectory))
	{
		MessageBox(L"Input error!");
		return;
	}
	ShowProgressDlg(L"compiling, waiting...");
	AfxBeginThread(BulidThread, this);

}

void CApkToolDlg::OnBnClickedBtnSign()
{
	m_editSign.GetWindowText(m_signFileName);
	if(m_signFileName.IsEmpty() || !PathFileExists(m_signFileName))
	{
		MessageBox(L"Input error!");
		return;
	}
	ShowProgressDlg(L"Signing, waiting...");
	AfxBeginThread(SignThread, this);

}

void CApkToolDlg::OnBnClickedBtnBaksmali()
{
	m_editBaksmali.GetWindowText(m_baksmaliFileName);
	if(m_baksmaliFileName.IsEmpty() || !PathFileExists(m_baksmaliFileName))
	{
		MessageBox(L"Input error!");
		return;
	}
	ShowProgressDlg(L"Baksmali processing, waiting");
	AfxBeginThread(BaksmaliThread, this);
}

void CApkToolDlg::OnBnClickedBtnSmali()
{
	m_editSmali.GetWindowText(m_smaliDirectory);
	if(m_smaliDirectory.IsEmpty() || !PathFileExists(m_smaliDirectory))
	{
		MessageBox(L"Input error!");
		return;
	}
	ShowProgressDlg(L"Smali processing, waiting");
	AfxBeginThread(SmaliThread, this);
}

void CApkToolDlg::OnBnClickedBtnDex2jar()
{
	m_editDex2jar.GetWindowText(m_dex2jarFileName);
	if(m_dex2jarFileName.IsEmpty() || !PathFileExists(m_dex2jarFileName))
	{
		MessageBox(L"Input error!");
		return;
	}
	ShowProgressDlg(L"Dex2jar processing, waiting");
	AfxBeginThread(Dex2jarThread, this);
	//::CreateThread(0, 0,(LPTHREAD_START_ROUTINE)Dex2jarThread, (void *)this, 0, 0);
}
void CApkToolDlg::OnBnClickedBtnJar2java()
{
	m_editJar2Java.GetWindowText(m_dojadFileName);
	if(m_dojadFileName.IsEmpty() || !PathFileExists(m_dojadFileName))
	{
		MessageBox(L"Input error!");
		return;
	}
	ShowProgressDlg(L"Jad processing, waiting");
	AfxBeginThread(DoJadThread, this);
	return;
}
void CApkToolDlg::OnBnClickedBtnGetcer()
{
	// TODO: Add your control notification handler code here
	m_editCert.GetWindowText(m_getCerApkName);
	if(m_getCerApkName.IsEmpty() || !PathFileExists(m_getCerApkName))
	{
		MessageBox(L"Input error!");
		return;
	}
	ShowProgressDlg(L"Get APK signature processing, waiting");
	AfxBeginThread(GetApkCerThread, this);
	return;
}
void CApkToolDlg::OnBnClickedBtnGetXML()
{
	// TODO: Add your control notification handler code here
	m_editXML.GetWindowText(m_getXMLApkName);
	if(m_getXMLApkName.IsEmpty() || !PathFileExists(m_getXMLApkName))
	{
		MessageBox(L"Input error!");
		return;
	}
	ShowProgressDlg(L"Get APK androidmanifest.xml processing, waiting");
	AfxBeginThread(GetApkXMLThread, this);
	return;
}
void CApkToolDlg::ShowProgressDlg(LPCWSTR lpszMsg)
{
// 	m_progressDlg.m_strMsg = lpszMsg;
// 	if (m_progressDlg.m_hWnd==NULL)
// 	{
// 		m_progressDlg.Create(IDD_DLG_PROGRESS,this);
// 	}
// 	m_progressDlg.ShowWindow(TRUE);
// 	m_progressDlg.CenterWindow();
//	m_progressDlg.DoModal();

// 	if (m_pprogressDlg==NULL)
// 	{
// 		m_pprogressDlg=new CProgressDlg();
// 	}
// 	m_pprogressDlg->m_strMsg=lpszMsg;
// 	if (!m_pprogressDlg->GetSafeHwnd())
// 	{
// 		if (!m_pprogressDlg->Create(IDD_DLG_PROGRESS,this))
// 		{
// 			return;
// 		}
// 		
// 	}
// 	m_pprogressDlg->ShowWindow(TRUE);
// 	m_pprogressDlg->CenterWindow();
	
}

void CApkToolDlg::CloseProgressDlg()
{
//  	m_progressDlg.Close();
// 	m_progressDlg.SendMessage(WM_CLOSE,0,0);
// 	::PostMessage(m_progressDlg.GetSafeHwnd(),WM_CLOSE,0,0);
// 	if (m_progressDlg.m_hWnd)
// 	{
// 		::DestroyWindow(m_progressDlg.m_hWnd);
// 	}
// 	m_pprogressDlg->Close();
// 	//::SendMessage(m_pprogressDlg->GetSafeHwnd(),WM_CLOSE,0,0);
// 	::DestroyWindow(m_pprogressDlg->m_hWnd);
// 	if (m_pprogressDlg!=NULL)
// 	{
// 	    delete m_pprogressDlg;
// 		m_pprogressDlg=NULL;
//  	}
	
}

void CApkToolDlg::Update()
{
	// chk env
	WCHAR szTempPath[MAX_PATH];	
	CString csPathCheck;
	GetCurrentDirectory(MAX_PATH,szTempPath);
	wcscat_s(szTempPath,MAX_PATH, L"\\Bin\\");
	m_strLibPath = szTempPath;

	m_apktool_jarPath.Format(L"%sapktool.jar", szTempPath);
	m_aapt_exePath.Format(L"%saapt.exe",szTempPath);
	m_signapk_jarPath.Format(L"%ssignapk.jar", szTempPath);
	m_testkey_pk8Path.Format(L"%stestkey.pk8", szTempPath);
	m_testkey_x509_pemPath.Format(L"%stestkey.x509.pem", szTempPath);
	m_baksmali_jarPath.Format(L"%sbaksmali.jar", szTempPath);
	m_smali_jarPath.Format(L"%ssmali.jar", szTempPath);
	m_dex2jar_jarPath.Format(L"%sdex2jar.jar", szTempPath);
	m_jadexe_path.Format(L"%sJad.exe",szTempPath);
	m_dex2jar_batPath.Format(L"%sdex2jar\\dex2jar.bat", szTempPath);
	m_7zdll_path.Format(L"%s7z.dll", szTempPath);
	m_7zexe_path.Format(L"%s7z.exe", szTempPath);
	m_AXMLPrinter_path.Format(L"%sAXMLPrinter2.jar", szTempPath);
	m_opensslexe_path.Format(L"%sopenssl\\openssl.exe", szTempPath);
	SHCreateDirectoryEx(NULL,szTempPath,NULL);
	if(!PathFileExists(m_apktool_jarPath))
	{
		csPathCheck.Format(L"Load apktool.jar path:%s failed",m_apktool_jarPath);
		m_log.AddString(csPathCheck);
		MessageBox(csPathCheck);
	}
	else{
		csPathCheck.Format(L"Load apktool.jar path:%s success",m_apktool_jarPath);
		m_log.AddString(csPathCheck);
	}
	if(!PathFileExists(m_aapt_exePath) ) 
	{
		csPathCheck.Format(L"Load aapt.exe path:%s failed",m_aapt_exePath);
		m_log.AddString(csPathCheck);
		MessageBox(csPathCheck);
	}
	else{
		csPathCheck.Format(L"Load aapt.exe path:%s success",m_aapt_exePath);
		m_log.AddString(csPathCheck);
	}
	if(!PathFileExists(m_signapk_jarPath))
	{
		csPathCheck.Format(L"Load signapk.jar path:%s failed",m_signapk_jarPath);
		m_log.AddString(csPathCheck);
		MessageBox(csPathCheck);
	}
	else{
		csPathCheck.Format(L"Load signapk.jar path:%s success",m_signapk_jarPath);
		m_log.AddString(csPathCheck);
	}
	if(!PathFileExists(m_testkey_pk8Path))
	{
		csPathCheck.Format(L"Load testkey.pk8 path:%s failed",m_testkey_pk8Path);
		m_log.AddString(csPathCheck);
		MessageBox(csPathCheck);
	}
	else{
		csPathCheck.Format(L"Load testkey.pk8 path:%s success",m_testkey_pk8Path);
		m_log.AddString(csPathCheck);
	}
	if(!PathFileExists(m_testkey_x509_pemPath))
	{
		csPathCheck.Format(L"Load testkey.x509.pem path:%s failed",m_testkey_x509_pemPath);
		m_log.AddString(csPathCheck);
		MessageBox(csPathCheck);
	}
	else{
		csPathCheck.Format(L"Load testkey.x509.pem path:%s success",m_testkey_x509_pemPath);
		m_log.AddString(csPathCheck);
	}
	if(!PathFileExists(m_baksmali_jarPath))
	{
		csPathCheck.Format(L"Load baksmali.jar path:%s failed",m_baksmali_jarPath);
		m_log.AddString(csPathCheck);
		MessageBox(csPathCheck);
	}
	else{
		csPathCheck.Format(L"Load baksmali.jar path:%s success",m_baksmali_jarPath);
		m_log.AddString(csPathCheck);
	}	
	if(!PathFileExists(m_smali_jarPath))
	{
		csPathCheck.Format(L"Load smali.jar path:%s failed",m_smali_jarPath);
		m_log.AddString(csPathCheck);
		MessageBox(csPathCheck);
	}
	else{
		csPathCheck.Format(L"Load smali.jar path:%s success",m_smali_jarPath);
		m_log.AddString(csPathCheck);
	}	
// 	if(!PathFileExists(m_dex2jar_jarPath))
// 	{
// 		csPathCheck.Format(L"Load dex2jar.jar path:%s failed",m_dex2jar_jarPath);
// 		m_log.AddString(csPathCheck); 
// 		MessageBox(csPathCheck);
// 	}
// 	else{
// 		csPathCheck.Format(L"Load dex2jar.jar path:%s success",m_dex2jar_jarPath);
// 		m_log.AddString(csPathCheck);
// 	}	
	if(!PathFileExists(m_jadexe_path))
	{
		csPathCheck.Format(L"Load Jad.exe path:%s failed",m_jadexe_path);
		m_log.AddString(csPathCheck);
		MessageBox(csPathCheck);
	}
	else{
		csPathCheck.Format(L"Load Jad.exe path:%s success",m_jadexe_path);
		m_log.AddString(csPathCheck);
	}
	if(!PathFileExists(m_dex2jar_batPath))
	{
		csPathCheck.Format(L"Load dex2jar.bat path:%s failed",m_dex2jar_batPath);
		m_log.AddString(csPathCheck);
		MessageBox(csPathCheck);
	}
	else{
		csPathCheck.Format(L"Load dex2jar.bat path:%s success",m_dex2jar_batPath);
		m_log.AddString(csPathCheck);
	}
	if(!PathFileExists(m_7zdll_path))
	{
		csPathCheck.Format(L"Load 7z.dll path:%s failed",m_7zdll_path);
		m_log.AddString(csPathCheck);
		MessageBox(csPathCheck);
	}
	else{
		csPathCheck.Format(L"Load 7z.dll path:%s success",m_7zdll_path);
		m_log.AddString(csPathCheck);
	}
	if(!PathFileExists(m_7zexe_path))
	{
		csPathCheck.Format(L"Load 7z.exe path:%s failed",m_7zexe_path);
		m_log.AddString(csPathCheck);
		MessageBox(csPathCheck);
	}
	else{
		csPathCheck.Format(L"Load 7z.exe path:%s success",m_7zexe_path);
		m_log.AddString(csPathCheck);
	}
	if(!PathFileExists(m_opensslexe_path))
	{
		csPathCheck.Format(L"Load openssl.exe path:%s failed",m_opensslexe_path);
		m_log.AddString(csPathCheck);
		MessageBox(csPathCheck);
	}
	else{
		csPathCheck.Format(L"Load openssl.exe path:%s success",m_opensslexe_path);
		m_log.AddString(csPathCheck);
	}
	if(!PathFileExists(m_AXMLPrinter_path))
	{
		csPathCheck.Format(L"Load AXMLPrinter2.jar path:%s failed",m_AXMLPrinter_path);
		m_log.AddString(csPathCheck);
		MessageBox(csPathCheck);
	}
	else{
		csPathCheck.Format(L"Load AXMLPrinter2.jar path:%s success",m_AXMLPrinter_path);
		m_log.AddString(csPathCheck);
	}
	m_log.AddString(L"-----------------------------------------------------------------------------------------");
	m_log.AddString(L"-----------------------APKTool Bin Load completely--------------------------------------");
	m_log.AddString(L"-----------------------------------------------------------------------------------------");
}
BOOL CApkToolDlg::UpdateFile(LPCWSTR lpszFileName, ULONGLONG uFileSize)
{
	CFile file;
	if(!file.Open(lpszFileName, CFile::modeRead))
	{
		return TRUE;
	}
	if(file.GetLength()!=uFileSize)
	{
		file.Close();
		DeleteFile(lpszFileName);
		return TRUE;
	}
	file.Close();
	return FALSE;
}

 BOOL CApkToolDlg::DelDirFileOpt(CString csPath)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	CString sFullPath;
	CString sFindFilter;
	DWORD dwAttributes = 0;

	sFindFilter = csPath;
	sFindFilter += _T("\\*.*");
	if ((hFind = FindFirstFile(sFindFilter, &wfd)) == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	do
	{
		if (_tcscmp(wfd.cFileName, _T(".")) == 0 || 
			_tcscmp(wfd.cFileName, _T("..")) == 0 )
		{
			continue;
		}

		sFullPath = csPath;
		sFullPath += _T('\\');
		sFullPath += wfd.cFileName;

		dwAttributes = GetFileAttributes(sFullPath);
		if (dwAttributes & FILE_ATTRIBUTE_READONLY)
		{
			dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(sFullPath, dwAttributes);
		}
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			DelDirFileOpt(sFullPath);
			RemoveDirectory(sFullPath);
		}
		else
		{
			if ( _tcsicmp(wfd.cFileName, _T("index.dat")) == 0)
			{
				//WipeFile(szPath, wfd.cFileName);
			}
			DeleteFile(sFullPath);
		}
	}while (FindNextFile(hFind, &wfd));
	FindClose(hFind);
	return TRUE;
}
BOOL CApkToolDlg::MyFindFile(CString folderPath, CString NameToFind,CString &hasfound)
 {
	 WIN32_FIND_DATA  FileInfo;
	 HANDLE Handle;
	 CString strfind = folderPath + L"\\*";
	 Handle = FindFirstFile(strfind, &FileInfo);
	 if (Handle == INVALID_HANDLE_VALUE)
	 {
		 return FALSE;
	 }
	 do{
		 if (FileInfo.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)    
		 {
			 if( (_tcscmp(FileInfo.cFileName,L".") != 0 ) &&(_tcscmp(FileInfo.cFileName,L"..") != 0))   
			 {
				 CString newPath = folderPath + L"\\" + FileInfo.cFileName;
				 MyFindFile(newPath, NameToFind,hasfound);
			 }
		 }
		 else  
		 {
			if(wcsstr(FileInfo.cFileName,NameToFind)){
				hasfound.Format(L"%s\\%s",folderPath,FileInfo.cFileName);
				return TRUE;
			}
		 }
	 }while (FindNextFile(Handle, &FileInfo));
	 FindClose(Handle);
	 return FALSE;
 }
CString CApkToolDlg::MyPathStripPath(CString csFileName){
	WCHAR *wszWideString;
	wszWideString=csFileName.AllocSysString();
	PathStripPath(wszWideString);
	PathRemoveExtension(wszWideString);
	CString strRet(wszWideString);
	return strRet;
}
CString CApkToolDlg::MyPathFindExtension(CString csFileName){
	WCHAR *wszWideString;
	wszWideString=csFileName.AllocSysString();
	if(PathFindExtension(wszWideString)){	
		CString strRet(PathFindExtension(wszWideString));
		return strRet;
	}
	else{
		return L"";
	}
}
CString CApkToolDlg::GetFileOutPutPath(CString csFileName){
	WCHAR *wszWideString;
	wszWideString=csFileName.AllocSysString();
	PathRemoveFileSpec(wszWideString);
	PathAddBackslash(wszWideString);
	CString strRet(wszWideString);
	strRet.Format(L"%s%s",wszWideString,MyPathStripPath(csFileName));
	return strRet;
}
CString CApkToolDlg::GetFileDiretory(CString csFileName){
	WCHAR *wszWideString;
	wszWideString=csFileName.AllocSysString();
	PathRemoveFileSpec(wszWideString);
	CString strRet(wszWideString);
	return strRet;
}
UINT CApkToolDlg::DecodeThread(LPVOID lpParam)
{
	CApkToolDlg* pDlg = (CApkToolDlg*)lpParam;
	pDlg->Decode();
	pDlg->CloseProgressDlg();
	return 0;
}

UINT CApkToolDlg::BulidThread(LPVOID lpParam)
{
	CApkToolDlg* pDlg = (CApkToolDlg*)lpParam;
	pDlg->Bulid();
	pDlg->CloseProgressDlg();
	return 0;
}

UINT CApkToolDlg::SignThread(LPVOID lpParam)
{
	CApkToolDlg* pDlg = (CApkToolDlg*)lpParam;
	pDlg->Sign();
	pDlg->CloseProgressDlg();
	return 0;
}

UINT CApkToolDlg::BaksmaliThread(LPVOID lpParam)
{
	CApkToolDlg* pDlg = (CApkToolDlg*)lpParam;
	pDlg->Baksmali();
	pDlg->CloseProgressDlg();
	return 0;
}

UINT CApkToolDlg::SmaliThread(LPVOID lpParam)
{
	CApkToolDlg* pDlg = (CApkToolDlg*)lpParam;
	pDlg->Smali();
	pDlg->CloseProgressDlg();
	return 0;
}

UINT Dex2jarThread(LPVOID lpParam)
{
	CApkToolDlg* pDlg = (CApkToolDlg*)lpParam;
	pDlg->Dex2jar();
	pDlg->CloseProgressDlg();
	return 0;
}
UINT CApkToolDlg::DoJadThread(LPVOID lpParam)
{
	CApkToolDlg* pDlg = (CApkToolDlg*)lpParam;
	pDlg->DoJad();
	pDlg->CloseProgressDlg();
	return 0;
}
UINT CApkToolDlg::GetApkCerThread(LPVOID lpParam)
{
	CApkToolDlg* pDlg = (CApkToolDlg*)lpParam;
	pDlg->GetApkCer();
	pDlg->CloseProgressDlg();
	return 0;
}

UINT CApkToolDlg::GetApkXMLThread(LPVOID lpParam)
{
	CApkToolDlg* pDlg = (CApkToolDlg*)lpParam;
	pDlg->getManifest();
	pDlg->CloseProgressDlg();
	return 0;
}

void CApkToolDlg::getManifest(){
	CString output;
	CString cmd;
	CString nextOutput;
	CString xmlName=L"AndroidManifest.xml";
	CString startTips=L"Start get Androidmanifest.xml file...";
	if (0==MyPathFindExtension(m_getXMLApkName).CompareNoCase(L".xml")){

	}
	else{
		WCHAR szTempPath[MAX_PATH];	
		GetCurrentDirectory(MAX_PATH,szTempPath);
		wcscat_s(szTempPath,MAX_PATH, L"\\Temp\\XMLTemp\\");
		DelDirFileOpt(szTempPath);
		output.Format(L"%s%s%s",szTempPath,MyPathStripPath(m_getXMLApkName),L"_xml");
		cmd.Format(L"\"%s\" e \"%s\" %s -y -o\"%s\"",		
			m_7zexe_path,
			m_getXMLApkName,
			xmlName,
			output
			);
		MyCreateProcess(cmd,UNZIP_FILE,startTips,m_getXMLApkName);
		output.Append(L"\\");
		output.Append(xmlName);
		if(PathFileExists(output))
		{
			nextOutput.Format(L"%s%s",GetFileOutPutPath(m_getXMLApkName),L"_AndroidManifest.xml");
			startTips=L"xml printer format AndroidManifest file...";
			cmd.Format(L"cmd /c java -jar \"%s\" \"%s\" > \"%s\"", m_AXMLPrinter_path, output, nextOutput);
			MyCreateProcess(cmd, APK_GETXML, startTips);
 			DelDirFileOpt(szTempPath);
		}
	}
}

void CApkToolDlg::Decode()
{
	CString startTips=L"Start decompiling...";
	CString output=GetFileOutPutPath(m_decodeFileName);
	CString cmd;
	cmd.Format(L"java -jar \"%s\" d \"%s\" \"%s\"", m_apktool_jarPath, m_decodeFileName, output);
	MyCreateProcess(cmd,APK_DECODE,startTips);
}

void CApkToolDlg::Bulid()
{
	CString startTips=L"Start compiling...";
	CString output;
	CString signedAPK;
	CString strFileName =MyPathStripPath(m_bulidDirectory);
	output.Format(L"%s\\dist\\%s.apk", m_bulidDirectory, strFileName);
	signedAPK.Format(L"%s(signed).apk", m_bulidDirectory);
	CString cmd;
	cmd.Format(L"java -jar \"%s\" b \"%s\"", m_apktool_jarPath, m_bulidDirectory);
	MyCreateProcess(cmd,APK_REBUILD,startTips,output);
	if(PathFileExists(output))
	{
		CString  unsignedAPK;
		unsignedAPK.Format(L"%s(no_signed).apk", m_bulidDirectory);
		CopyFile(output, unsignedAPK, TRUE);
		startTips=L"Start signing...";
		Sleep(1000);
		cmd.Format(L"java -jar \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"",m_signapk_jarPath, m_testkey_x509_pemPath, m_testkey_pk8Path, output, signedAPK);
		MyCreateProcess(cmd,APK_SIGN,startTips,signedAPK);
		DeleteFile(output);
	}
}

CString CApkToolDlg::ClearOldCer(CString signFileName)
{
	CString intermediate = signFileName+(L".zip");
	stdCopyFile(signFileName.AllocSysString(), intermediate.AllocSysString());
	CString cmd;
	CString output;
	CString startTips=L"start clean apk...";
	cmd.Format(L"\"%s\" d \"%s\" \"%s\"",m_7zexe_path,intermediate, TEXT("META-INF"));
	//ShellExecute();
	MyCreateProcess(cmd,APK_CLEAN,startTips,output);
	return intermediate;
}

bool CApkToolDlg::stdCopyFile(WCHAR *Sourcefile,WCHAR *Destinationfile)
{
	HANDLE hSourceFile=CreateFile(Sourcefile, GENERIC_READ, 0, NULL, OPEN_ALWAYS, 0, NULL);
	HANDLE hDestFile=CreateFile(Destinationfile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if( hSourceFile==INVALID_HANDLE_VALUE||hDestFile==INVALID_HANDLE_VALUE)
	{
		return false;   
	}
	DWORD dwRemainSize=GetFileSize(hSourceFile,NULL);
	char buffer[1024];
	while(dwRemainSize>0)
	{
		DWORD dwActualRead=0;
		ReadFile(hSourceFile,buffer,1024,&dwActualRead,NULL);
		dwRemainSize-=dwActualRead;
		DWORD dwActualWrote=0;
		DWORD dwOnceWrote=0;     
		WriteFile(hDestFile,buffer,dwActualRead,&dwOnceWrote,NULL);  
	}
	CloseHandle(hSourceFile);
	CloseHandle(hDestFile);
	return true;
}

void CApkToolDlg::Sign()
{	
	CString output;

	CString intermediate = ClearOldCer(m_signFileName);

	CString strFileName = MyPathStripPath(MyPathStripPath(intermediate));
	WCHAR *wszWideString;
	wszWideString=m_signFileName.AllocSysString();
	PathRemoveFileSpec(wszWideString);

	CString startTips=L"start signing...";
	output.Format(L"%s\\%s(signed).apk", wszWideString,strFileName);
	CString cmd;
	cmd.Format(L"java -jar \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"",m_signapk_jarPath, m_testkey_x509_pemPath, m_testkey_pk8Path, intermediate, output);
	MyCreateProcess(cmd,APK_SIGN,startTips,output);
	DeleteFile(intermediate);
}
void CApkToolDlg::Baksmali()
{
	CString output=GetFileOutPutPath(m_baksmaliFileName);
	CString cmd;
	CString startTips;
	if (0==MyPathFindExtension(m_baksmaliFileName).CompareNoCase(L".odex"))
	{
		startTips=L"Start decompiling odex file...";
		cmd.Format(L"java -jar \"%s\" -x \"%s\" -o \"%s\"", m_baksmali_jarPath, m_baksmaliFileName, output);
	}else if(0==MyPathFindExtension(m_baksmaliFileName).CompareNoCase(L".dex"))
	{
		startTips=L"Start decompiling dex file...";
		cmd.Format(L"java -jar \"%s\" \"%s\" -o \"%s\"", m_baksmali_jarPath, m_baksmaliFileName, output);
	}
	MyCreateProcess(cmd,APK_TOSMAIL,startTips);
}

void CApkToolDlg::Smali()
{
	CString startTips=L"Start building dex file...";
	CString output;
	output.Format(L"%s.dex", m_smaliDirectory);
	CString cmd;
	cmd.Format(L"java -Xmx512M -jar \"%s\" \"%s\" -o \"%s\"", m_smali_jarPath, m_smaliDirectory, output);
	MyCreateProcess(cmd,APK_TODEX,startTips);
}
void CApkToolDlg::Dex2jar()
{	
	CString output;
	CString cmd;
	CString startTips=L"Start Dex2jar...";
	cmd.Format(L"%s \"%s\"",
		m_dex2jar_batPath,
		m_dex2jarFileName
		);
	MyCreateProcess(cmd,APK_DEX2JAR,startTips);
}
void CApkToolDlg::DoJad(){
	CString output;
	CString cmd;
	CString startTips;
	CString nextInput;
	if (0==MyPathFindExtension(m_dojadFileName).CompareNoCase(L".jar"))
	{
		startTips=L"Start uncompress jar to temporary directory...";
		WCHAR szTempPath[MAX_PATH];	
		GetCurrentDirectory(MAX_PATH,szTempPath);
		wcscat_s(szTempPath,MAX_PATH, L"\\Temp\\JarTemp\\");
		DelDirFileOpt(szTempPath);
		output.Format(L"%s%s",szTempPath,MyPathStripPath(m_dojadFileName));
		cmd.Format(L"\"%s\" x \"%s\" -y -o\"%s\"", m_7zexe_path, m_dojadFileName, output);
		MyCreateProcess(cmd,UNZIP_FILE,startTips,m_dojadFileName);
		if(PathFileExists(output))
		{
			nextInput.Format(L"%s",output);
			output.Format(L"%s%s",GetFileOutPutPath(m_dojadFileName),L"_java");		
			startTips=L"Start decompiling temporary *.class files...";
			cmd.Format(L"\"%s\" -o -r  -sjava -d \"%s\" -o \"%s\\**\\*.class\"", m_jadexe_path, output,nextInput);
			MyCreateProcess(cmd,APK_2JAVA,startTips);
			DelDirFileOpt(szTempPath);
		}
	}
	else if((0==MyPathFindExtension(m_dojadFileName).CompareNoCase(L".class")))
	{
		startTips=L"Start decompiling *.class file...";
		output.Format(L"%s",GetFileDiretory(m_dojadFileName));
		cmd.Format(L"\"%s\" -o -d \"%s\" -s .java -8 \"%s\"", m_jadexe_path,output,m_dojadFileName);
		MyCreateProcess(cmd,APK_2JAVA,startTips);
	}

	else{
		output.Format(L"%s%s",GetFileOutPutPath(m_dojadFileName),L"_java");
		startTips=L"Start decompiling *.class file directory...";
		cmd.Format(L"\"%s\" -o -r  -sjava -d \"%s\" -o \"%s\\**\\*.class\"", m_jadexe_path, output, m_dojadFileName);
		MyCreateProcess(cmd,APK_2JAVA,startTips);
	}
}
void CApkToolDlg::GetApkCer(){
	CString output;
	CString cmd;
	CString nextInput;
	CString metainfo=L"META-INF";
	CString startTips=L"Start get certificate file...";
	if (0==MyPathFindExtension(m_getCerApkName).CompareNoCase(L".RSA")&&0==MyPathFindExtension(m_getCerApkName).CompareNoCase(L".DSA")){

	}
	else{
		WCHAR szTempPath[MAX_PATH];	
		GetCurrentDirectory(MAX_PATH,szTempPath);
		wcscat_s(szTempPath,MAX_PATH, L"\\Temp\\CerTemp\\");
		DelDirFileOpt(szTempPath);
		output.Format(L"%s%s%s",szTempPath,MyPathStripPath(m_getCerApkName),L"_meta-info");
		cmd.Format(L"\"%s\" e \"%s\" %s -y -o\"%s\"",		
			m_7zexe_path,
			m_getCerApkName,
			metainfo,
			output
			);
		MyCreateProcess(cmd,UNZIP_FILE,startTips,m_getCerApkName);
		if(PathFileExists(output))
		{
			if(MyFindFile(output, L".RSA",nextInput)||MyFindFile(output, L".DSA",nextInput)){
				output.Format(L"%s%s",GetFileOutPutPath(m_getCerApkName),L"_cer.txt");
				startTips=L"keytool formate certificate file...";
				CString keytool;
				cmd.Format(L"keytool -printcert -file \"%s\"",nextInput);
				MyCreateProcess(cmd,APK_GETCER,startTips);
				startTips=L"openssl formate certificate file...";
				cmd.Format(L"\"%s\" pkcs7 -inform DER -in \"%s\"  -noout -print_certs -text -out \"%s\"",m_opensslexe_path,nextInput,output);
				MyCreateProcess(cmd,APK_GETCER,startTips);
			}
			DelDirFileOpt(szTempPath);
		}
	}
}

void CApkToolDlg::MyCreateProcess(CString cmd,APKTOOL_OPERATION eOperation,CString startTips,CString special){
	EnterCriticalSection(&m_csCtlList);
	m_log.AddString(startTips);
	m_log.SendMessage(WM_VSCROLL,SB_BOTTOM,NULL); 
	LeaveCriticalSection(&m_csCtlList);
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead,hWrite;	
	hRead=NULL;
	hWrite=NULL;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	CString tResult;

	if(!CreatePipe(&hRead,&hWrite,&sa,0))
	{
		MessageBox(L"Error On CreatePipe()");
	} 
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow=SW_HIDE;
	si.dwFlags=STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	DWORD dwExitCode;
	char buffer[4096] = {0};
	CString strLog;
	bool bSuccessFlag=false;
	if(CreateProcess(NULL,(LPWSTR)(LPCWSTR)cmd,NULL,NULL,TRUE,0,NULL,m_strLibPath,&si,&pi))
	{
		CloseHandle(hWrite);
		DWORD bytesRead;
		while (true)
		{
			memset(buffer,0,4096);
			if(ReadFile(hRead, buffer, 4095, &bytesRead, NULL)==NULL)
				break;
			strLog= CA2W(buffer);
			if(strLog.Find(L"Done.")>=0){
				bSuccessFlag=true;
			}
			int curPos= 0;
			CAtlString resToken;
			resToken= strLog.Tokenize(L"\r\n",curPos);
			while (resToken != "")
			{
				EnterCriticalSection(&m_csCtlList);
				m_log.AddString(resToken);
				m_log.SendMessage(WM_VSCROLL,SB_BOTTOM,NULL); 
				LeaveCriticalSection(&m_csCtlList);
				resToken= strLog.Tokenize(L"\r\n",curPos);
			};
		}
		CloseHandle(hRead);
		hWrite = NULL;
		hRead = NULL;
	}
	else
	{
		DWORD ret = GetLastError();
		startTips = L"failed\n";
	}
	WaitForSingleObject(pi.hThread,INFINITE);
	GetExitCodeThread(pi.hThread, &dwExitCode);
	switch (eOperation)
	{
	case APK_DECODE:
		if(bSuccessFlag)
		{
			tResult.Format(L"%s.apk decompile success!", m_decodeFileName);
		}
		else
		{
			tResult.Format(L"%s.apk decompile failed!", m_decodeFileName);
		}
		break;
	case APK_REBUILD:
		if(!PathFileExists(special))
		{
			m_strTitle.Format(L"%s build failed!", special);
		}
		else
		{
			m_strTitle.Format(L"%s build success!", special);
		}
		break;
	case APK_SIGN:
		if (PathFileExists(special))//dwExitCode == 0)
		{
			m_strTitle.Format(L"%s.apk sign success!", special);
		}else
		{
			m_strTitle.Format(L"%s.apk sign failed!",special);
		}
		break;
	case APK_TOSMAIL:
		if(dwExitCode == 0)
		{
			tResult.Format(L"%s decompile success!", m_baksmaliFileName);

		}else
		{
			tResult.Format(L"%s decompile failed!", m_baksmaliFileName);
		}
		break;
	case APK_TODEX:
		if(dwExitCode == 0)
		{
			tResult.Format(L"%s build dex success!", m_smaliDirectory);

		}else
		{
			tResult.Format(L"%s build dex failed!", m_smaliDirectory);
		}
		break;
	case APK_DEX2JAR:
		if(bSuccessFlag)
		{
			tResult.Format(L"%s dex to jar  success!", m_dex2jarFileName);
		}
		else
		{
			tResult.Format(L"%s dex to jar  failed!", m_dex2jarFileName);
		}
		break;
	case APK_2JAVA:
		if(dwExitCode == 0)
		{
			tResult.Format(L"%s decompile jar/class success!", m_dojadFileName);
		}
		else
		{
			tResult.Format(L"%s decompile jar/class failed!", m_dojadFileName);
		}
		break;
	case UNZIP_FILE:
		if(dwExitCode == 0)
		{
			tResult.Format(L"%s uncompress temporary file success!", special);
		}
		else
		{
			tResult.Format(L"%s uncompress temporary file failed!", special);
		}
		break;
	case APK_GETCER:
		if(dwExitCode == 0)
		{
			tResult.Format(L"%s get certificate success!", m_getCerApkName);
		}
		else
		{
			tResult.Format(L"%s get certificate failed!", m_getCerApkName);
		}
		break;
	case APK_GETXML:
		if(dwExitCode == 0)
		{
			tResult.Format(L"%s get AndroidManifest.xml success!", m_getXMLApkName);
		}
		else
		{
			tResult.Format(L"%s get AndroidManifest.xml failed!", m_getXMLApkName);
		}
		break;
	case APK_CLEAN:
		if(dwExitCode == 0)
		{
			tResult.Format(L"apk is cleaned!");
		}
		else
		{
			tResult.Format(L"apk cleaned failed");
		}
		break;
	default:
		break;
	}
	EnterCriticalSection(&m_csCtlList);
	m_log.AddString(tResult);
	m_log.SendMessage(WM_VSCROLL,SB_BOTTOM,NULL); 
	LeaveCriticalSection(&m_csCtlList);
}
void CApkToolDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (m_log.GetSafeHwnd())
	{
		CRect itemRect,ListRect,TabRect,ProgRect;

		GetClientRect(&itemRect);
		m_log.GetClientRect(&ListRect);
		ListRect.left = itemRect.left;
		ListRect.right = itemRect.right;
		ListRect.top = itemRect.top + 350;
		ListRect.bottom = itemRect.bottom ;
		m_log.MoveWindow(&ListRect);
	}
}
