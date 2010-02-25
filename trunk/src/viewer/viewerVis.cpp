// viewerView.cpp : implementation of the CviewerView class
//

#include "stdafx.h"

#include "viewerVis.h"

#include "vis/vis_error.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



IMPLEMENT_DYNCREATE(CViewerVis, CVisModule)

BEGIN_MESSAGE_MAP(CViewerVis, CVisModule)
END_MESSAGE_MAP()


CViewerVis::CViewerVis()
{
}

CViewerVis::~CViewerVis()
{
}

BOOL CViewerVis::PreCreateWindow(CREATESTRUCT& cs)
{
	return CVisModule::PreCreateWindow(cs);
}


BOOL CViewerVis::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	try
	{
		CVisModule::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	}

	catch (CVIPError e) 
	{
		CString e1;

		e1.Format("EEEEEEKKKKKKSSS !!!\n\nfile: %s\nline: %i\n\nerror message:\n%s",e.GetFile(),e.GetLine(),e.GetErrorMessage());

		AfxMessageBox(e1,MB_ICONSTOP|MB_OK);
		exit(1);
	}

	return TRUE;
}
