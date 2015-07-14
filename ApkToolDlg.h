
#pragma once
#include "afxwin.h"
#include "FileEdit.h"
#include "ProgressDlg.h"

typedef enum _APKTOOL_OPERATION
{  
	APK_DECODE,
	APK_REBUILD,
	APK_SIGN,
	APK_TOSMAIL,
	APK_TODEX,
	APK_DEX2JAR,
	APK_2JAVA,
	UNZIP_FILE,
	APK_GETCER,
	APK_GETXML,
	APK_CLEAN,
}APKTOOL_OPERATION;
class CApkToolDlg : public CDialog
{
	typedef struct THREADPARAM
	{
		THREADPARAM(CApkToolDlg* pDlg, CString strMsg)
		{
			m_pDlg = pDlg;
			m_strMsg = strMsg;
		}
		CApkToolDlg* m_pDlg;
		CString m_strMsg;
	}ThreadParam,*PThreadParam;

public:
	CApkToolDlg(CWnd* pParent = NULL);


	enum { IDD = IDD_APKTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);


protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtnShowLog();
	afx_msg void OnBnClickedBtnClearLog();
	afx_msg void OnBnClickedBtnDecode();
	afx_msg void OnBnClickedBtnBuild();
	afx_msg void OnBnClickedBtnSign();
	afx_msg void OnBnClickedBtnBaksmali();
	afx_msg void OnBnClickedBtnSmali();
	afx_msg void OnBnClickedBtnDex2jar();
	afx_msg void OnBnClickedBtnJar2java();
	afx_msg void OnBnClickedBtnExportLog();
	afx_msg void OnBnClickedBtnGetcer();
	afx_msg void OnBnClickedBtnGetXML();
	DECLARE_MESSAGE_MAP()

private:
	void ShowProgressDlg(LPCWSTR lpszMsg);
	void CloseProgressDlg();

	void Update();
	BOOL UpdateFile(LPCWSTR lpszFileName, ULONGLONG uFileSize);

	UINT static DecodeThread(LPVOID lpParam);
	UINT static BulidThread(LPVOID lpParam);
	UINT static SignThread(LPVOID lpParam);
	UINT static BaksmaliThread(LPVOID lpParam);
	UINT static SmaliThread(LPVOID lpParam);
	friend UINT Dex2jarThread(LPVOID lpParam);
	UINT static DoJadThread(LPVOID lpParam);
	UINT static GetApkCerThread(LPVOID lpParam);
	UINT static GetApkXMLThread(LPVOID lpParam);

	void getManifest();
	void Decode();
	void Bulid();
	void Sign();
	void Baksmali();
	void Smali();
	void Dex2jar();
	void DoJad();
	void GetApkCer();
	CString ClearOldCer(CString signFileName);
	bool stdCopyFile(WCHAR *Sourcefile,WCHAR *Destinationfile);
	
	CString m_apktool_jarPath;	// apktool.jar path 
	CString m_aapt_exePath;		// r04
	CString m_signapk_jarPath;	// signapk.jar path 
	CString m_testkey_pk8Path;		// testkey.pk8 path
	CString m_testkey_x509_pemPath;		// testkey.x509.pem path
	CString m_baksmali_jarPath; // baksmali.jar path 
	CString m_smali_jarPath;	// smali.jar path 
	CString m_dex2jar_jarPath;		// dex2jar.jar path
	CString m_dex2jar_batPath;
	CString m_jadexe_path;
	CString m_7zdll_path;
	CString m_7zexe_path;
	CString m_opensslexe_path;
	CString m_AXMLPrinter_path;

	CString m_decodeFileName;
	CString m_bulidDirectory;
	CString m_signFileName;
	CString m_baksmaliFileName;
	CString m_smaliDirectory;
	CString m_dex2jarFileName;
	CString m_dojadFileName;
	CString m_getCerApkName;
	CString m_getXMLApkName;

	CString m_strTitle;

	CFileEdit m_editDecode;
	CFileEdit m_editBuild;
	CFileEdit m_editSign;
	CFileEdit m_editBaksmali;
	CFileEdit m_editSmali;
	CFileEdit m_editDex2jar;
	CFileEdit m_editJar2Java;
	CFileEdit m_editCert;
	CFileEdit m_editXML;
	CToolTipCtrl m_tooltip;
	
	CProgressDlg* m_pprogressDlg;
	CProgressDlg m_progressDlg;

	CListBox m_log;		
	BOOL m_showLog;

	CString m_strLibPath;
	
public:
	CString MyPathStripPath(CString csFileName);
	CString MyPathFindExtension(CString csFileName);
	CString GetFileOutPutPath(CString csFileName);
	CString GetFileDiretory(CString csFileName);
	BOOL	MyFindFile(CString folderPath, CString NameToFind,CString &hasfound);
	BOOL	DelDirFileOpt(CString csPath);
	void MyCreateProcess(CString cmd,APKTOOL_OPERATION eOperation,CString startTips,CString special=NULL);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CRITICAL_SECTION             m_csCtlList;


};
