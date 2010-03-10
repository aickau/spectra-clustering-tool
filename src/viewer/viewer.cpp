// viewer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "viewer.h"
#include "MainFrm.h"

#include "MainFrm.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CviewerApp

BEGIN_MESSAGE_MAP(CViewerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CViewerApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
END_MESSAGE_MAP()


// CviewerApp construction

CViewerApp::CViewerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CviewerApp object

CViewerApp theApp;


// CviewerApp initialization

BOOL CViewerApp::InitInstance()
{
	AfxEnableControlContainer();

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	CMDIFrameWnd* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;
	
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_VIS_TETYPE));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_VIS_TETYPE));


	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();


	return TRUE;
}


int CViewerApp::ExitInstance() 
{
	if (m_hMDIMenu != NULL)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);

	return CWinApp::ExitInstance();
}

void CViewerApp::OnFileNew() 
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);

	if (pFrame)
	{

	pFrame->CreateNewChild(
		RUNTIME_CLASS(CChildFrame), IDR_VIS_TETYPE, m_hMDIMenu, m_hMDIAccel);
		}


}


void CViewerApp::Close()
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);

	if ((pFrame) && ((void*)pFrame != (void*)this))
	{
		pFrame->GetActiveFrame()->SendMessage(WM_CLOSE);
	}
}


BOOL CViewerApp::OnIdle(LONG lCount )
{
	CWinApp::OnIdle(lCount);

	return TRUE;
}


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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

// App command to run the dialog
void CViewerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CviewerApp message handlers

